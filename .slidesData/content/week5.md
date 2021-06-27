---
title: "Week 5 - ROP"
layout: "bundle"
outputs: ["Reveal"]
date: 2021-06-28T00:07:11+10:00
---

{{< slide class="center" >}}

## Week 5

Return Oriented Programming

---

{{< slide content="secedu.goodfaith" >}}

---

### Midsem Exam

> Friday 2nd July (This week)

3 questions, 2 hours

> 10am - 6pm

---

## ROP

{{% section %}}

![](gru-rop.jpg)

---

<div style="font-size: 0.6em; white-space: pre-wrap; text-align: left">
ROP (Return Oriented Programming) Gadgets are small snippets of a few assembly instructions typically ending in a <code>ret</code> instruction which already exist as executable code within each binary or library. These gadgets may be used for binary exploitation and to subvert vulnerable executables.

<span style="text-decoration: underline dotted">When the addresses of many ROP Gadgets are written into a buffer we have formed a ROP Chain. If an attacker can move the stack pointer into this ROP Chain then control can be completely transferred to the attacker.</span>

Most executables contain enough gadgets to write a turing-complete ROP Chain. For those that don't, one can always use dynamic libraries contained in the same address-space (such as <code>libc</code>) once we know their addresses.

The beauty of using ROP Gadgets is that no new executable code needs to be written anywhere - an attacker may achieve their objective using only the code that already exists in the program. <span style="font-size: 0.8em; float: right; filter: brightness(60%)">Source: github.com/Ben-Lichtman/ropr</span>
</div>

---

#### TL;DR

* Instructions are multi-byte
    * Bytes inside an instruction could form another instruction...
* We call these unintended instructions 'gadgets'
* A ROP Chain is a series of ROP gadgets
* These gadgets are executable (circumvent NX) because they exist in the actual program code

{{% /section %}}

---

## What, how, I'm confused

Hi confused! I'm a diagram

```
         0xFFFF +-----------+
                |           |
                |           |
                |           |
                |           |
                |           |
                |           |
                |==  RET  ==|
                |           |   \
                |           |   |
                |           |   | Current function
Start of ---->  |           |   |
  buffer        |           |   /
         0x0000 +-----------+
```


---

## Tools of the trade

> [ropper](https://github.com/sashs/Ropper)  //  [gropper](https://featherbear.cc/UNSW-COMP6447/useful/gropper.txt)

&nbsp;  

> [ROPgadget](https://github.com/JonathanSalwan/ROPgadget)


&nbsp;  

> [ropr](https://github.com/Ben-Lichtman/ropr)

---

## Which Gadget?

Ultimately we want to use gadgets that form some desired shellcode e.g. call to `/bin/sh`

> <span style="font-size: 0.9em">`execve("/bin/sh", NULL, NULL)`</span>

```python
eax = 0xb (execve syscall)        # mov eax, 0xb
ebx = bin_sh_address              # ...
ecx = 0                           # xor ecx, ecx
edx = 0                           # xor edx, edx
syscall                           # int 0x80
```

---

## Useful Gadgets

* `inc ...`
* `mov ...`
* `add ...`
* `sub ...`
* `pop ...`

---

## Using `pop`

Your ROP exploit doesn't need to comprised of **just** addresses. We can pass values (NB: encoded correctly) and `pop` them off!

{{% section %}}

> i.e. Set EAX to 13

* `gdgt{mov eax, 13}`
* `gdgt{xor eax, eax} + gdgt{inc eax}` ...
* `gdgt{pop eax} + p32(0xd)`

---

> `pop ...` + value

```python
...                             |
payload += gdgt{pop ebx; ret;}  |
payload += p32(0xAABBCCDD)      | payload += ...
...                             | ...
# ebx = 0xAABBCCDD              | # 
```

{{% /section %}}

---

## Also don't forget `push esp`

If you need to get the address of the stack position... find a gadget that pushes esp, and pops it

> `push esp; pop ...` 

_Note: Generally both of these instructions need to be in the **same gadget**_

---

## ROP Gadgets + Functions

We can also add addresses in our ROP chain.

> i.e found a call to `system`? Use that!

* Consider the cdecl layout - last arguments first
* Consider the stack growth - function call first

```python
payload += p32(0x804aabb) # some system() function address
payload += p32(addr_to_binsh)

0xFFFF +-----------------+
       |                 |  # esp+4  ^ when popped     
       |  addr_to_binsh  |  # esp    
       |    0x804aabb    |  # esp-4  v when pushed
       |    ^-------- buffer start
0x0000 +-----------------+
```

---

What if a program doesn't have enough gadgets?

{{% fragment %}}
> A: <s>Find a better program</s>  

Use gadgets in dependent libraries

<span style="font-size: 0.4em; float: right; "><i>A: Uninstall, give up, cry, sleep</i></span>
{{% /fragment %}}

---

## ret2libc

{{% section %}}

`libc` provides functions used by most programs!

i.e basically literally any C function  
_puts, printf, gets, etc..._

---

##### Recipe

1) Find a base address  
2) Find the `libc` version  
3) Find a target address

> [libc](https://libc.nullbyte.cat/) [library](https://libc.rip/)

---

#### pwntools

You can set the base address of an ELF in pwntools!

> <span style="font-size: 0.9em">`<ELF>.address = base_address`</span>

```python
libc = ELF("libc_version.so")
libc.address = printf_leak - l.symbols['printf']

...

payload += p32(l.symbols['system']) # Offset adjusted!
```

_Note: Also works for the program base_

---

> **Note**  
Don't forget about ASLR / PIE

* We need to leak the address of each needed library.
  * library != segment <!-- show vmmap relativity -->
* Program base may need to also be leaked.

{{% /section %}}

---

### Midsem Exam

> Friday 2nd July (This week)

3 questions, 2 hours

> 10am - 6pm

---