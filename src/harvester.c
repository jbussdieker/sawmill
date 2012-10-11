#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#include "insist.h"
#include "sleepdefs.h"

#include "harvester.h"
#include "backoff.h"
#include "amqp.h"

#define BUFFERSIZE 16384

void track_rotation(int *fd, const char *path) {
  struct stat pathstat, fdstat;
  int rc;
  fstat(*fd, &fdstat);
  rc = stat(path, &pathstat);
  if (rc == -1) {
    /* error stat'ing the file path, restart loop and try again */
    return;
  }

  if (pathstat.st_dev != fdstat.st_dev || pathstat.st_ino != fdstat.st_ino) {
    /* device or inode number changed, this file was renamed or rotated. */
    rc = open(path, O_RDONLY);
    if (rc == -1) {
      /* Error opening file, restart loop and try again. */
      return;
    }
    close(*fd);
    /* start reading the new file! */
    *fd = rc; 
  } else if (pathstat.st_size < fdstat.st_size) {
    /* the file was truncated, jump back to the beginning */
    lseek(*fd, 0, SEEK_SET);
  }
} /* track_rotation */

void *harvest(void *arg) {
  struct harvest_config *config = arg;
  int fd;

  if (strcmp(config->path, "-") == 0) {
    /* path is '-', use stdin */
    fd = 0;
  } else {
    fd = open(config->path, O_RDONLY);
    /* Start at the end of the file */
    lseek(fd, 0, SEEK_END);
  }
  insist(fd >= 0, "open(%s) failed: %s", config->path, strerror(errno));

  void *emitter = new_emitter(config);

  char *buf;
  ssize_t bytes;
  buf = calloc(BUFFERSIZE, sizeof(char));

  struct backoff sleeper;
  backoff_init(&sleeper, &MIN_SLEEP, &MAX_SLEEP);

  int offset = 0;
  for (;;) {
    bytes = read(fd, buf + offset, BUFFERSIZE - offset - 1);
    offset += bytes;
    if (bytes < 0) {
      /* error, maybe indicate a failure of some kind. */
      printf("read(%d '%s') failed: %s\n", fd,
             config->path, strerror(errno));
      break;
    } else if (bytes == 0) {
      backoff(&sleeper);
      if (strcmp(config->path, "-") == 0) {
        /* stdin gave EOF, close out. */
        break;
      }
      track_rotation(&fd, config->path);
    } else {
      /* Data read, handle it! */
      backoff_clear(&sleeper);

      char *line;
      char *septok = buf;
      char *start = NULL;
      while (start = septok, (line = strsep(&septok, "\n")) != NULL) {
        if (septok == NULL) {
          /* last token found, no terminator though */
          offset = offset - (line - buf);
          memmove(buf, line, strlen(line));
        } else {
          /* emit line as an event */
          /* 'septok' points at the start of the next token, so subtract one. */
          size_t line_len = septok - start - 1;
          if (config->debug == 1) {
            printf("DEBUG: [HARVESTER] %*s\n", (int)line_len, line);
          }
          emit(emitter, line_len, line);
        }
      }
    }
  }

  destroy_emitter(emitter);
}

