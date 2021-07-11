#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int vuln() {
  char buf[8];
  read(0, buf, 1337);
  return 0;
}

int main() {
  setbuf(stdout, NULL);
  printf("LIBC Address Leak: %p\n", &printf);
  vuln();
  return 0;
}
