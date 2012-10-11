#ifndef _EMITTER_H_
#define _EMITTER_H_

struct kv {
  char *key;
  size_t key_len;
  char *value;
  size_t value_len;
}; /* struct kv */

void *new_emitter(void *emitter);
void destroy_emitter(void *emitter);
void emit(void *emitter, int line_len, char *line);

#endif /* _EMITTER_H_ */
