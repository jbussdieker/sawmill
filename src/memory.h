#ifndef _MEMORY_H_
#define _MEMORY_H_

void hex_dump(void *ptr, int size);
char *replace(const char *s, const char *old, const char *new);
void *sawmill_malloc(int size);
void sawmill_free(void *p);

#endif /* _MEMORY_H_ */
