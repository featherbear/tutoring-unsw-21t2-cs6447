#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define preAlloc 10

int main() {
  void *P[preAlloc];
  puts("Blitzing: ");
  for (int i = 0; i < preAlloc; i++) printf("  [*] %p\n", (P[i] = malloc(0x10)));
  for (int i = 0; i < preAlloc; i++) free(P[i]);

  char *chunk16 = malloc(0x10 /* 16 */);
  strncpy(chunk16, "AAAABBBBTEST__16", 0x10-1); chunk16[0x10-1] = '\0';
  printf("*(chunk16 = %p) = %s\n", chunk16, chunk16);

  char *chunk32 = malloc(0x20 /* 32 */);
  strncpy(chunk32, "AAAABBBBCCCCDDDDTEST__32", 0x20-1); chunk32[0x20-1] = '\0';
  printf("*(chunk32 = %p) = %s\n", chunk32, chunk32);

  puts("chunk16 and chunk32 allocated");

  free(chunk16);
  free(chunk32);

  puts("END");
}
