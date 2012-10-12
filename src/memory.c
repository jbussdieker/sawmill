#include <stdio.h>
#include <stdlib.h>

#include "memory.h"

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

char *replace(const char *s, const char *old, const char *new)
{
  char *ret;
  int i, count = 0;
  size_t newlen = strlen(new);
  size_t oldlen = strlen(old);

  for (i = 0; s[i] != '\0'; i++) {
    if (strstr(&s[i], old) == &s[i]) {
      count++;
      i += oldlen - 1;
    }
  }

  ret = sawmill_malloc(i + count * (newlen - oldlen) + 1);
  if (ret == NULL)
    exit(EXIT_FAILURE);

  i = 0;
  while (*s) {
    if (strstr(s, old) == s) {
      strcpy(&ret[i], new);
      i += newlen;
      s += oldlen;
    } else {
      ret[i++] = *s++;
    }
  }

  ret[i] = '\0';

  return ret;
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
