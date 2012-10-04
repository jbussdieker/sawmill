#ifndef _HARVESTER_H_
#define _HARVESTER_H_

struct harvest_config {
  char *path; /* the path to harvest */
  char *host;
  int port;
  struct kv *fields; /* any extra fields to add to each event */
  size_t fields_len; /* number of fields */
};

void *harvest(void *);

#endif /* _HARVESTER_H_ */
