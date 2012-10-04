#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#include "insist.h"

#include "harvester.h"
#include "amqp.h"

#define BUFFERSIZE 16384

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

  printf("Hello %s:%d %s\n", config->host, config->port, config->path);
  void *conn = amqp_open(config->host, config->port, "guest", "guest");

  char *buf;
  ssize_t bytes;
  buf = calloc(BUFFERSIZE, sizeof(char));

  int offset = 0;
  for (;;) {
    bytes = read(fd, buf + offset, BUFFERSIZE - offset - 1);
    offset += bytes;

    if (bytes < 0) {
      //printf("BYTES < 0\n");
    } else if (bytes == 0) {
      struct timespec sleep = { 0, 10000000 };
      nanosleep(&sleep, NULL);
      //printf("BYTES == 0\n");
    } else {
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
        
          amqp_publish(conn, "stash_rawlogs", "logstash", line);
          //printf("DEBUG: %*s\n", (int)line_len, line);
        }
      }
    }
  }

  amqp_close(conn);
}

