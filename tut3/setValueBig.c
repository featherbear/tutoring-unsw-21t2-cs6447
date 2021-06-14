#include <stdio.h>

#include "pika.h"

#define TARGET (long long unsigned)3735936685

#pragma GCC diagnostic ignored "-Wformat-security"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"

static long long unsigned someValue = 0x4f7295;

void fn() {
    char buffer[64];

    printf("A bigger birdie told me that someValue is at address %p\n", &someValue);
    printf("Currently someValue = %p\n", (void*)someValue);
    printf("Try set it to the value %llu (%p)\n\n", TARGET, (void*)TARGET);
    puts("Enter a format string!");

    fgets(buffer, 64, stdin);

    printf(buffer);

    puts("\n-------------------------");
    printf("someValue = %llu (%p)\n", someValue, (void*)someValue);
    puts("\n-------------------------");

    puts("aaaaaand");
    if (someValue != TARGET) {
        puts("nope.");
        return;
    }
    pika();
}

int main() {
    fn();
}