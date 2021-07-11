#!/usr/bin/env python2
from pwn import *

p = process("./doublefree")
p.interactive()
