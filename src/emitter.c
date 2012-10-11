#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "amqp.h"
#include "harvester.h"

struct emitter {
  void *conn;
  struct harvest_config *config;
};

static char *replace(const char *s, const char *old, const char *new)
{
  char *ret;
  int i, count = 0;
  size_t newlen = strlen(new);
  size_t oldlen = strlen(old);

  for (i = 0; s[i] != '\0'; i++) {
    if (strstr(&s[i], old) == &s[i]) {
      count++;
      i += oldlen - 1;
    }
  }

  ret = malloc(i + count * (newlen - oldlen));
  if (ret == NULL)
    exit(EXIT_FAILURE);

  i = 0;
  while (*s) {
    if (strstr(s, old) == s) {
      strcpy(&ret[i], new);
      i += newlen;
      s += oldlen;
    } else {
      ret[i++] = *s++;
    }
  }

  ret[i] = '\0';

  return ret;
}

void *new_emitter(void *arg) {
  struct harvest_config *config = arg;
  struct emitter *emitter = calloc(1, sizeof(struct emitter));

  emitter->config = config;
  emitter->conn = amqp_open(config->host, config->port, config->user, config->password);

  return emitter;
}

void destroy_emitter(void *arg) {
  struct emitter *emitter = arg;
  amqp_close(emitter->conn);
  free(emitter);
}

void emit(void *arg, int line_len, char *dirp) {
  struct emitter *emitter = arg;
  char *message;

  char *dirp2 = malloc(line_len + 1);
  memcpy(dirp2, dirp, line_len);
  dirp2[line_len] = 0;

  char *line = replace(dirp2, "\"", "\\\"");
  free(dirp2);

  //line[strlen(line)-1] = 0;
  message = malloc(line_len + 256);
  sprintf(message, "{\"@fields\":{},\"@message\":\"%s\"", line);

  int i;
  for (i = 0; i < emitter->config->fields_len; i++) {
    char *key = emitter->config->fields[i].key;
    char *value = emitter->config->fields[i].value;
    char buf[128];
    sprintf(buf, ",\"%s\":\"%s\"", key, value);
    strcat(message, buf);
  }
  strcat(message, "}");

  if (emitter->config->debug == 1) {
    printf("DEBUG: [EMITTER] %s\n", message);
  }

  amqp_publish(emitter->conn, emitter->config->exchange, "logstash", message);

  free(line);
  free(message);
}
