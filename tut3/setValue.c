// gcc -z norelro -z execstack  -fno-stack-protector -no-pie -m32 -g

#include <stdio.h>

#include "pika.h"

#pragma GCC diagnostic ignored "-Wformat-security"

static int someValue = 0x4f7295;

void fn() {
    char buffer[32];

    printf("A little birdie told me that someValue is at address %p\n", &someValue);
    printf("Currently someValue = %p\n", (void*)someValue);
    printf("Try set it to the value 50 (dec)\n\n");
    puts("Enter a format string!");

    fgets(buffer, 32, stdin);

    printf(buffer);

    puts("\n-------------------------");
    printf("someValue = %u\n", someValue);
    puts("\n-------------------------");

    puts("aaaaaand");
    if (someValue != 50) {
        puts("nope.");
        return;
    }
    pika();
}

int main() {
    fn();
}