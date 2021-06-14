// gcc -z norelro -z execstack  -fno-stack-protector -no-pie -m32 -g printfRunner.c -o printfRunner
#include <stdio.h>

#pragma GCC diagnostic ignored "-Wformat-security"


void fn() {
    puts("Enter a format string!");
    char buffer[50];                 // 8
    
    fgets(buffer, 50, stdin);

    printf(buffer);
}

int main() {
    fn();
}