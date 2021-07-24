#!/usr/bin/env python2
from pwn import *

global p
p = process("./prac")


def menu():
    p.recvuntil("Choice: ")


def make(index, name):
    log.info("Make: {}".format(index))
    p.sendline("a")
    p.recvuntil("Clone ID:", timeout=0.1)
    p.sendline(str(index))
    p.recvuntil("Enter Name")
    p.sendline(name)
    menu()


def edit(index, name):
    log.info("Edit: {}".format(index))
    p.sendline("c")
    p.recvuntil("Clone ID: ", timeout=0.1)
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
    p.recvuntil("Clone ID: ", timeout=0.1)
    p.sendline(str(index))
    p.recvuntil("Name: ", timeout=0.1)
    result = p.recvline()
    menu()
    return result


def hint(index):
    log.info("Hint: {}".format(index))
    p.sendline("h")
    p.recvuntil("Clone ID: ", timeout=0.1)
    p.sendline(str(index))
    return p.recvline()
