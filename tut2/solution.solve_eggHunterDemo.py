#!/usr/bin/python3

from pwn import *

p = process('./eggHunterDemo')

shellcode = asm(f'''
// By convention, functions return a value by setting
// the eax register to the value.

// We want to write code that finds a signature in a 
// buffer, and return the index location

mov eax, [esp+4] # Grab the first argument [cdecl]
                 ### eax = buffer address

######################################################
push ebx  ## Push ebx as to not break the demo code ##
          ## i.e when this shellcode returns        ##
######################################################

mov ecx, eax     # ecx = current search address
mov ebx, 0xFF    # ebx = search signature

loop:
  cmp [ecx], ebx   # *addr == 0xFF ??? 
  je done          # if equ goto done 
  inc ecx          # addr += 1
  jmp loop         # goto loop

done:
  sub ecx, eax     # offset = current address - buffer address
  mov eax, ecx     # return value = ofset

  ####################################################
  pop ebx  ## Pop ebx as to not break the demo code ##
  ####################################################

  ret   # Return to the calling function
''')

p.sendline(shellcode)
p.interactive()


