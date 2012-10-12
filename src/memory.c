#include <stdio.h>
#include <stdlib.h>

void hex_dump(void *ptr, int size) {
  int i;
  unsigned char *cur = (unsigned char *)ptr;

  for (i = 0; i < size; i++) {
    printf("%02X ", *cur);
    cur++;
  }
  
  cur = (unsigned char *)ptr;
  for (i = 0; i < size; i++) {
    if (*cur >= ' ' && *cur <= '~')
      printf("%c", *cur);
    else
      printf(".");
    
    cur++;
  }
  printf("\n");
}

void *sawmill_malloc(int size) {
  //void *ptr;
  //ptr = malloc(size+4);
  //*(unsigned long *)ptr = size;
  //printf("SAWMILL_MALLOC: %p\n", ptr);
  //hex_dump(ptr, size+4);
  //return ptr+4;

  return malloc(size);
}

void sawmill_free(void *ptr) {
  //printf("  SAWMILL_FREE: %p\n", ptr);
  //int size = *(unsigned long *)(ptr-4);
  //hex_dump(ptr-4, size+4);
  //return free(ptr-4);
  return free(ptr);
}
