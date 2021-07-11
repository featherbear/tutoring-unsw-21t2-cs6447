#!/usr/bin/python3

from pwn import *

p = process("./chunkExample")
pause()

p.interactive()
