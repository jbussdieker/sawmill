#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "amqp.h"
#include "harvester.h"

char *replace(const char *s, const char *old, const char *new)
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
  void *conn = amqp_open(config->host, config->port, config->user, config->password);
  return conn;
}

void destroy_emitter(void *conn) {
  amqp_close(conn);
}

void emit(void *conn, char *line) {
  char *message;

  line = replace(line, "\"", "\\\"");
  message = malloc(strlen(line) + 50);
  sprintf(message, "{\"message\":\"%s\"}", line);

  amqp_publish(conn, "some_exchange", "logstash", message);
}
