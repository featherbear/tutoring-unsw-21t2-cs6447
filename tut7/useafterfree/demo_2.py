#!/usr/bin/env python2
from pwn import *

p = process("./useafterfree")
p.interactive()
