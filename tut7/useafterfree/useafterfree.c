#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void littleEndian(unsigned int *adr, unsigned int value) { *adr = value; }

int main() {
  printf("------------------------------------\n");
  printf("-  T-Cache Use-After-Free Attack   -\n");
  printf("------------------------------------\n");
  getchar();

  fprintf(stderr, "Allocating 16 bytes on the heap.\n");
  char *a = malloc(16);
  strcpy(a, "AAAAAAAA");
  fprintf(stderr, "malloc(16): %p\n", a);
  char adr[4];

  // Create little endian copy of address
  littleEndian((unsigned int *)adr, (unsigned int)a);
  getchar();

  fprintf(stderr, "Freeing once...\n");
  free(a);
  fprintf(stderr, "T-Cache Free List: [ %p <-- 0x0 ].\n", a);
  getchar();

  fprintf(stderr, "Instead of freeing again, we have a use-after-free...\n");
  fprintf(stderr,
          "So we modify the free 'next' chunk to point back at itself!...\n");
  strncpy(a, &main, 4); // Copy the chunk address into the chunk next pointer
  fprintf(stderr, "T-Cache Free List: [ %p <-- %p ].\n", a, a);
  getchar();

  fprintf(stderr, "Allocating a fresh 16 bytes on the heap.\n");
  a = malloc(16);
  fprintf(stderr, "malloc(16): %p\n", a);
  fprintf(stderr, "T-Cache Free List: [ %p <-- 0x0 ].\n", a);
  fprintf(stderr, "This time, we make sure NOT to write anything into the "
                  "first 4 bytes!\n");
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

  return 0;
}
