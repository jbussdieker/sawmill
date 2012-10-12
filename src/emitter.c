#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "amqp_emitter.h"
#include "harvester.h"

struct emitter {
  void *(*emit)(struct emitter *, char *);
  void *(*close)(struct emitter *);

  void *data;
  struct harvest_config *config;
};

///////////////////////////////////////////////////////////////////////////////
// STDOUT
///////////////////////////////////////////////////////////////////////////////
void emit_stdout(struct emitter *emitter, char *message) {
  printf("%s\n", message);
  fflush(stdout);
}

void close_stdout(struct emitter *emitter) {
}

struct emitter *new_stdout_emitter(struct harvest_config *config) {
  struct emitter *emitter = calloc(1, sizeof(struct emitter));
  emitter->config = config;

  emitter->emit = &emit_stdout;
  emitter->close = &close_stdout;
  return emitter;
}

///////////////////////////////////////////////////////////////////////////////
// AMQP
///////////////////////////////////////////////////////////////////////////////
void emit_amqp(struct emitter *emitter, char *message) {
  amqp_publish(emitter->data, emitter->config->exchange, "logstash", message);
}

void close_amqp(struct emitter *emitter) {
  amqp_close(emitter->data);
}

struct emitter *new_amqp_emitter(struct harvest_config *config) {
  struct emitter *emitter = calloc(1, sizeof(struct emitter));
  emitter->config = config;

  emitter->data = amqp_open(config->host, config->port, config->user, config->password);
  emitter->emit = &emit_amqp;
  emitter->close = &close_amqp;
  return emitter;
}

///////////////////////////////////////////////////////////////////////////////
// BASE
///////////////////////////////////////////////////////////////////////////////
void *new_emitter(void *arg) {
  struct harvest_config *config = arg;

  if (config->exchange == NULL) {
    if (config->debug == 1) printf("DEBUG: Creating emitter (STDOUT)\n");
    return new_stdout_emitter(config);
  } else {
    if (config->debug == 1) printf("DEBUG: Creating emitter (AMQP)\n");
    return new_amqp_emitter(config);
  }
}

void close_emitter(void *arg) {
  struct emitter *emitter = arg;
  emitter->close(emitter);
  free(emitter);
}

void emit(void *arg, int line_len, char *dirp) {
  struct emitter *emitter = arg;
  char *message;

  char *dirp2 = sawmill_malloc(line_len + 1);
  memcpy(dirp2, dirp, line_len);
  dirp2[line_len] = 0;

  char *line = replace(dirp2, "\"", "\\\"");
  sawmill_free(dirp2);

  message = sawmill_malloc(line_len + 256);
  sprintf(message, "{\"@fields\":{\"@message\":\"%s\"}", line);

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

  emitter->emit(emitter, message);

  sawmill_free(line);
  sawmill_free(message);
}

