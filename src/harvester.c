#include <stdio.h>

#include "harvester.h"

void *harvest(void *arg) {
  struct harvest_config *config = arg;

  printf("Hello %s:%d %s\n", config->host, config->port, config->path);
  for (;;) {
  }
}

