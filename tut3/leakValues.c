#include <stdio.h>

#pragma GCC diagnostic ignored "-Wformat-security"

void fn() {
    puts("Enter a format string!");
    int someValue = 0x4f7295;        // 11
    char buffer[16];                 // 7
                                     // 7 = 11 + -4 stack movements (4*4)
    fgets(buffer, 16, stdin);

    printf(buffer);

    puts("\n-------------------------");
    printf("someValue = %u\n-------------------------\n", someValue);
}

int main() {
    fn();
}