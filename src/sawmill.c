#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>
#include <pthread.h>

#include "config.h"
#include "insist.h"

#include "harvester.h"

typedef enum {
  opt_help = 'h',
  opt_version = 'v',
  opt_field,
  opt_host,
  opt_port,
  opt_user,
  opt_password
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
    "The hostname of the AMQP server" },
  { "port", required_argument, opt_port,
    "The port of the AMQP server" },
  { "user", required_argument, opt_user,
    "The AMQP user to connect with (Default: guest)" },
  { "password", required_argument, opt_password,
    "The AMQP password to connect with (Default: guest)" },
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

int main(int argc, char **argv) {
  int c, i;
  struct option *getopt_options = NULL;

  struct kv *extra_fields = NULL;
  size_t extra_fields_len = 0;

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
  char *host = 0;
  char *user = 0;
  char *password = 0;
  int port = 0;
  while (i = -1, c = getopt_long_only(argc, argv, "+hv", getopt_options, &i), c != -1) {
    switch (c) {
      case opt_version:
        printf("%s\n", PACKAGE_STRING);
        return 0;
      case opt_help:
        usage(argv[0]);
        return 0;
      case opt_host:
        host = strdup(optarg);
        break;
      case opt_user:
        user = strdup(optarg);
        break;
      case opt_password:
        password = strdup(optarg);
        break;
      case opt_port:
        port = (short)atoi(optarg);
        break;
      case opt_field:
        tmp = strchr(optarg, '=');
        if (tmp == NULL) {
          printf("Invalid --field setting, expected 'foo=bar' form, " \
                 "didn't see '=' in '%s'", optarg);
          usage(argv[0]);
          exit(1);
        }
        extra_fields_len += 1;
        extra_fields = realloc(extra_fields, extra_fields_len * sizeof(struct kv));
        *tmp = '\0'; // turn '=' into null terminator
        tmp++; /* skip to first char of value */
        extra_fields[extra_fields_len - 1].key = strdup(optarg);
        extra_fields[extra_fields_len - 1].key_len = strlen(optarg);
        extra_fields[extra_fields_len - 1].value = strdup(tmp);
        extra_fields[extra_fields_len - 1].value_len = strlen(tmp);
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

  if (host == NULL) {
    printf("Missing --host flag\n");
    usage(argv[0]);
    return 1;
  }

  if (port == 0) {
    printf("Missing --port flag\n");
    usage(argv[0]);
    return 1;
  }

  if (user == NULL) {
    user = "guest";
  }

  if (password == NULL) {
    password = "guest";
  }

  /* I'll handle write failures; no signals please */
  signal(SIGPIPE, SIG_IGN);

  insist(argc > 0, "No arguments given. What log files do you want shipped?");

  pthread_t *harvesters = calloc(argc, sizeof(pthread_t));

  for (i = 0; i < argc; i++) {
    struct harvest_config *harvester = calloc(1, sizeof(struct harvest_config));
    harvester->path = argv[i];
    harvester->host = host;
    harvester->port = port;
    harvester->user = user;
    harvester->password = password;
    harvester->fields = extra_fields;
    harvester->fields_len = extra_fields_len;
    pthread_create(&harvesters[i], NULL, harvest, harvester);
  }

  for (i = 0; i < argc; i++) {
    pthread_join(harvesters[i], NULL);
  }

  printf("All harvesters completed. Exiting.\n");

  return 1;
}
