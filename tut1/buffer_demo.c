#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int failpwn = 0;

int main(void) {
  char input[40];
  int passchk = 0;

  printf("Enter your password: ");
  gets(input);

  if (strcmp("secretpassword123", input) == 0) {
    failpwn = passchk = 1;
  }

  if (passchk) {
    printf("-----------------------\n");
    printf("-- LOGIN SUCCESSFULL --\n");
    printf("-----------------------\n");

    if (failpwn) {
      printf("Try to login WITHOUT entering the correct password...\n");
    } else {
      printf("-----------------------\n");
      printf("--    LEET HACKER    --\n");
      printf("-----------------------\n");
    }
  } else {
    printf("-----------------------\n");
    printf("--    LOGIN FAILED   --\n");
    printf("-----------------------\n");
  }

  return 0;
}
