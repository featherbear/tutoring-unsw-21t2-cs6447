---
title: "Week 3"
layout: "bundle"
outputs: ["Reveal"]
date: 2021-06-13T19:44:40+10:00
draft: true
---

{{< slide class="center" >}}

## Week 3

---

{{< slide content="secedu.goodfaith" >}}

---

## cdecl, again

```
```

---

## Format Strings

vargs 

printf(blah)


%x print arg as hex
%n write the number of written bytes...

---

```c
#include <stdio.h>

int main() {
    char a[30];
    fgets(a, 32, stdin);
    printf(a);
}
```

`%d` -> Print the next item on the args list... what arg list  
`%d %s` -> Print the next item as a number, then the next as a string...?  

`%s` -> string :: print every byte as a character until NULLs

Stack canaries are on the stack!
--> How far is our vulnerable string from the stack


Write to a pointer, then use that pointer???
Write into the buffer. The buffer is on the stack.. soooo we can reach that address?

---

Find our controllable space

Pad
Address
Write primitive

width modifier

Writing many things

What should I overwrite?

%n total number - write smaller values first ugh
What about %256

Write alot of bytes...
Or write multiple bytes


`%<flags><width><precision><modifier><type>`

d
hd
hhd

n
hn
hhn

Write one 4-byte value (max = 2**32 = 4,294,967,296)
Write four 1-byte values (max = 4x 2**8 = 1024)

What if i need to write larger values
:shrug: 
What value will be written if I write 256 into a byte (max value = 255)

PLT / GOT - dynamic library function calls
dtors
Hooks (malloc hook, free hook)
__atexit


# GOT

GOT - global offset table
PLT - process linkage table

Process calls to library functions go through a redirection

The first time the function is called, the address is searched for, then stored in the GOT.


`got` command in pwndbg - shows all of the linkable functions  
`vmmap` 

* ADDRESSES IN THE GOT ARE UNITIALISED UNTIL CALLED FOR THE FIRST TIME - they initially point to some lookup handler
* override the address?


Function Pointers

---

# Memory Protection

* Memory Corruption
    * Stack reordering
    * random padding
    * stack canaries
    * FORTIFY
    * RELRO

* post-corruption code execution
    * ASLR + PIE
    * NX
    * PAC
    * Hypervisor

* PIE - Program can only contain relative jumps
  * Requires ASLR to be on

* FORTIFY - Stop string vulnerabilities
  * Disables %n if the format string is in writeable memory

* RELRO - Relocation Read-Only
    * GOT is read-only
    * Partial RELRO
        * GOT table appears before the BSS (globals stuff)
    * Full RELRO
        * rip.

## General Pointer Addresses

* 0x565 - Binary base with PIE enabled
* 0x804 - Binary base with PIE disabled
* 0xf7f - Library base
* 0xff.. - Stack base


checksec

Week 5 midsem

cdecl

printf

https://gist.github.com/adamtanana/3dae09744a38f718c530a3b2e3297c5c
tldr; show how they can leak stuff of stack with %x and %s, then show how they can leak arbritray with %4$s then show how they can write with %hhn


fmtstring - Don't talk about it :)



tldr; show how they can leak stuff of stack with %x and %s

then show how they can leak arbritray with %4$s then show how they can write with %hhn


