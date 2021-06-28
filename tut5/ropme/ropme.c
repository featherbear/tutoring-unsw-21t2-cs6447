/*

    ROP three different ways!

*/

#include <stdio.h>
#include <stdlib.h>

void ropme() {
  __asm__ __volatile__("pop ebx");
  __asm__ __volatile__("ret");
  __asm__ __volatile__("xor ecx, ecx");
  __asm__ __volatile__("ret");
  __asm__ __volatile__("xor edx, edx");
  __asm__ __volatile__("ret");
  __asm__ __volatile__("mov eax, 0xb");
  __asm__ __volatile__("int 0x80");
  __asm__ __volatile__("ret");
}

char str[] = "/bin/sh";

void gettime() { system("/bin/date"); }

int vuln() {
  char input[8];
  fgets(input, 1337, stdin);
}

int main() {
  printf("Whats the time?\n");
  gettime();
  printf("- %p -\n", &printf);
  printf("It's time to ROP!\n");
  vuln();
  puts("Bye!\n");
}
