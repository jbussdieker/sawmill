#ifndef _EMITTER_H_
#define _EMITTER_H_

struct kv {
  char *key;
  size_t key_len;
  char *value;
  size_t value_len;
}; /* struct kv */

void *new_emitter(void *arg);
void emit(void *conn, char *line);

#endif /* _EMITTER_H_ */
