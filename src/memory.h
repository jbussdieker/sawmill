#ifndef _MEMORY_H_
#define _MEMORY_H_

void hex_dump(void *ptr, int size);
void *sawmill_malloc(int size);
void sawmill_free(void *p);

#endif /* _MEMORY_H_ */
