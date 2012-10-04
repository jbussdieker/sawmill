#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>
#include <pthread.h>

#include "insist.h"

typedef enum {
  opt_help = 'h',
  opt_version = 'v',
  opt_field,
  opt_host,
  opt_port,
} optlist_t;

struct option_doc {
  const char *name;
  int         has_arg;
  int         val;
  const char *documentation;
};

static struct option_doc options[] = {
  { "help", no_argument, opt_help, 
    "show this help" },
  { "version", no_argument, opt_version, 
    "show the version of sawmill" },
  { "field", required_argument, opt_field, 
    "Add a custom key-value mapping to every line emitted" },
  { "host", required_argument, opt_host,
    "The hostname to send messages to" },
  { "port", required_argument, opt_port,
    "The port to send messages to" },
  { NULL, 0, 0, NULL },
};

void usage(const char *prog) {
  printf("Usage: %s [options] /path/to/file [/path/to/file2 ...]\n", prog);

  int i;
  for (i = 0; options[i].name != NULL; i++) {
    printf("  --%s%s %.*s %s\n", options[i].name,
           options[i].has_arg ? " VALUE" : "",
           (int)(20 - strlen(options[i].name) - (options[i].has_arg ? 6 : 0)),
           "                                   ",
           options[i].documentation);
  }
}

void harvest(int index) {
  printf("Hello %d\n", index);
  for (;;) {
  }
}

int main(int argc, char **argv) {
  int c, i;
  struct option *getopt_options = NULL;

  for (i = 0; options[i].name != NULL; i++) {
    getopt_options = realloc(getopt_options, (i+1) * sizeof(struct option));
    getopt_options[i].name = options[i].name;
    getopt_options[i].has_arg = options[i].has_arg;
    getopt_options[i].flag = NULL;
    getopt_options[i].val = options[i].val;
  }

  getopt_options = realloc(getopt_options, (i+1) * sizeof(struct option));
  getopt_options[i].name = NULL;

  char *tmp;
  while (i = -1, c = getopt_long_only(argc, argv, "+hv", getopt_options, &i), c != -1) {
    switch (c) {
      case opt_version:
        printf("0.0.1\n");
        break;
      case opt_help:
        usage(argv[0]);
        return 0;
      case opt_host:
        //emitter_config.host = strdup(optarg);
        break;
      case opt_port:
        //emitter_config.port = (short)atoi(optarg);
        break;
      default:
        insist(i == -1, "Flag (--%s%s%s) known, but someone forgot to " \
               "implement handling of it! This is certainly a bug.",
               options[i].name, 
               options[i].has_arg ? " " : "",
               options[i].has_arg ? optarg : "");

        usage(argv[0]);
        return 1;
    }
  }

  free(getopt_options);

  argc -= optind;
  argv += optind;

  /* I'll handle write failures; no signals please */
  signal(SIGPIPE, SIG_IGN);

  insist(argc > 0, "No arguments given. What log files do you want shipped?");

  pthread_t *harvesters = calloc(argc, sizeof(pthread_t));

  for (i = 0; i < argc; i++) {
    pthread_create(&harvesters[i], NULL, harvest, 123);
  }

  for (i = 0; i < argc; i++) {
    pthread_join(harvesters[i], NULL);
  }

  printf("All harvesters completed. Exiting.\n");

  return 1;
}
