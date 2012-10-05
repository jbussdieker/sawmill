#ifndef _HARVESTER_H_
#define _HARVESTER_H_

#include "emitter.h"

struct harvest_config {
  char *path;
  char *host;
  char *exchange;
  int port;
  char *user;
  char *password;

  struct kv *fields;
  size_t fields_len;
};

void *harvest(void *);

#endif /* _HARVESTER_H_ */
