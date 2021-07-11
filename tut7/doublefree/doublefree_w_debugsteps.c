#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  printf("------------------------------------\n");
  printf("-     T-Cache Fastbin Attack       -\n");
  printf("------------------------------------\n");
  getchar();

  /*
      pwndbg> att pid
      pwndbg> c
  */

  fprintf(stderr, "Allocating 16 bytes on the heap.\n");
  char *a = malloc(16);
  strcpy(a, "AAAAAAAA");
  fprintf(stderr, "malloc(16): %p\n", a);
  getchar();

  /*
      pwndbg> tcachebins
      tcachebins
      empty

      pwndbg> vis_heap_chunks 0x91fa578 1

      0x91fa578:	0x00000000	0x00000021
      0x91fa580:	0x41414141	0x41414141
      0x91fa588:	0x00000000	0x00000000
      0x91fa590:	0x00000000	0x00000000
      0x91fa598:	0x00000000	0x00020a69	 <-- Top chunk


  */

  fprintf(stderr, "Freeing once...\n");
  free(a);
  fprintf(stderr, "T-Cache Free List: [ %p <-- 0x0 ].\n", a);
  getchar();

  /*

      pwndbg> tcachebins
      tcachebins
      0x18 [  1]: 0x91fa580 ◂— 0x0



      pwndbg> vis_heap_chunks 0x91fa578 1

      0x91fa578:	0x00000000	0x00000021
      0x91fa580:	0x00000000	0x41414141
      0x91fa588:	0x00000000	0x00000000
      0x91fa590:	0x00000000	0x00000000
      0x91fa598:	0x00000000	0x00020a69	 <-- Top chunk


  */

  fprintf(stderr, "Freeing twice...\n");
  free(a);
  fprintf(stderr, "T-Cache Free List: [ %p <-- %p ].\n", a, a);
  getchar();

  /*

      pwndbg> tcachebins
      tcachebins
      0x18 [  2]: 0x91fa580 ◂— 0x91fa580

      pwndbg> vis_heap_chunks 0x91fa578 1

      0x91fa578:	0x00000000	0x00000021
      0x91fa580:	0x091fa580	0x41414141
      0x91fa588:	0x00000000	0x00000000
      0x91fa590:	0x00000000	0x00000000
      0x91fa598:	0x00000000	0x00020a69	 <-- Top chunk

      It now stores a pointer to the next chunk in the chunk itself!

  */

  fprintf(stderr, "Allocating a fresh 16 bytes on the heap.\n");
  a = malloc(16);
  strcpy(a, "AAAAAAAA");
  fprintf(stderr, "malloc(16): %p\n", a);
  fprintf(stderr, "T-Cache Free List: [ %p <-- 0x0 ].\n", a);
  getchar();

  /*

      pwndbg> tcachebins
      tcachebins
      0x18 [  1]: 0x91fa580 ◂— 'AAAAAAAA'


      pwndbg> vis_heap_chunks 0x91fa578 1

      0x91fa578:	0x00000000	0x00000021
      0x91fa580:	0x41414141	0x41414141
      0x91fa588:	0x00000000	0x00000000
      0x91fa590:	0x00000000	0x00000000
      0x91fa598:	0x00000000	0x00020a69	 <-- Top chunk

  */

  fprintf(stderr, "Allocating another 16 bytes on the heap.\n");
  char *b = malloc(16);
  strcpy(a, "BBBBBBBB");
  fprintf(stderr, "malloc(16): %p\n", a);

  fprintf(
      stderr,
      "Both are pointers to the same chunk!\n chunk_a = %p\n chunk b = %p\n", a,
      b);
  getchar();

  /*

      pwndbg> vis_heap_chunks 0x91fa578 1

      0x91fa578:	0x00000000	0x00000021
      0x91fa580:	0x42424242	0x42424242
      0x91fa588:	0x00000000	0x00000000
      0x91fa590:	0x00000000	0x00000000
      0x91fa598:	0x00000000	0x00020a69	 <-- Top chunk


      tcachebins
      0x18 [  0]: 0x41414141 ('AAAA')

  */

  b = malloc(16);
  getchar();

  /*
      Program received signal SIGSEGV (fault address 0x41414141)

      Why? Because it thought there was another fake chunk!

      Lets try and exploit this next!
  */

  return 0;
}
