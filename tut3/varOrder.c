// gcc -o varOrder -m32 -g varOrder.c
#include <stdio.h>

#pragma GCC diagnostic ignored "-Wformat-security"

char buffer[50];

void fn() {
    char* someString = "Hello there!";        // 4
    void* somePtrToSomeString = &someString;  // 7
    int someInt = 1032;                       // 6
    char* s="AAAAAAAAAAAA";                   // 5

    fgets(buffer, 50, stdin);
    printf(buffer);
}

int main() {
    fn();
}