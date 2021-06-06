#include <stdio.h>   // Compile with:
#include <stdlib.h>  //   gcc -o eggHunterDemo -z execstack -m32 -g eggHunterDemo.c
#include <time.h>    //                        \__________/
#include <unistd.h>  // -- Andrew                   | allow stack execution

int main() {
    char mem[256] = { 0 }, shellcode[512] = { 0 };
    srand(time(NULL));

    // Set some random position in mem to [0, 255]
    mem[(u_int8_t)rand()] = 0xFF;  // eggu desu ~

    puts("Find that egg!\nEnter up to 512 bytes of shellcode");
    read(0, shellcode, 512);
    // Your shellcode should accept a buffer (esp+4), and return an index via eax
    u_int8_t i = ((u_int8_t(*)(u_int8_t * a)) shellcode)(mem);

    printf("mem[i = %hhu] = 0x%2x\n", i, mem[i] & 0xFF);
    puts(mem[i] & 0xFF == 0xFF ? "nice!" : "nope.");
}