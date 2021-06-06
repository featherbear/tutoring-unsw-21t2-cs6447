---
title: "Week 2"
layout: "bundle"
outputs: ["Reveal"]
date: 2021-06-06T22:58:39+10:00
---

<script defer type="application/javascript" src="fuzz.js" ></script>
<style>
.mono {
    font-family: monospace; 
}
.fuzzTarget {
    font-family: monospace;
}
</style>



{{< slide class="center" >}}

## Week 2

<span class="mono">$></span> <span class="fuzzTarget">shellcode</span>

---

# Today's Agenda

* Kahoot'nt
* Content Review
  * Assembly Code
  * cdecl
  * Shellcode
  * Egg Hunters
  * `/bin/////////sh`
  * `NOP` sleds
* Activities

---

# Kahoot'nt

> [joinmyquiz.com](http://joinmyquiz.com)

<!-- How did Wargames 1 go?

* hahaha(rd) segfault go brrr
* I found it alright
* Too easy -->

<!-- Any questions? -->
<!-- If you don't, just enter something and press next -->



---

{{< slide content="secedu.goodfaith" >}}

---

{{< slide class="center" >}}

# Content Review

---

## Assembly

Your common assembly instructions
```bash
mov a, b        # a = b
add a, value    # a = a + value
sub a, value    # a = a - value
xor a, b        # a = a ^ b
and a, b        # a = a & b
push a          # push a onto the stack, then dec esp - 4
pop a           # pop into a from the stack, then add esp + 4
int 0x80        # syscall
```
&nbsp;  
## Syscalls

[Syscall Table](https://featherbear.cc/UNSW-COMP6447/syscall)

---

## cdecl

> void mycoolfunc(char *a, int b, char *c)

What assembly would I write to call this function?

* {{% fragment %}}`push address_c`{{% /fragment %}}
* {{% fragment %}}`push value_b`{{% /fragment %}}
* {{% fragment %}}`push address_a`{{% /fragment %}}
* {{% fragment %}} `call mycoolfunc`{{% /fragment %}}

---

## Shellcode TL;DR

{{% section %}}

Shellcode is a sequence of assembly instructions that carry out a task. If we can make the instruction pointer (`EIP`) point to our shellcode, we have code execution!

{{% fragment %}}
* Functions are just assembly
* Assembly are just bytes
* We can send bytes :)
{{% /fragment %}}

{{% fragment %}}
Generally we want to pop a _shell_
{{% /fragment %}}

---

### Demo

> `./runner`

{{% /section %}}

--- 

## 🥚 Egghunter

{{% section %}}
Sometimes we do not have enough space in an execute-vulnerable buffer to inject our payload.

Instead, we put our payload (the 🥚) somewhere else, and write an 'egg hunter' - assembly instructions that finds and execute the 🥚.

---

#### My Little Pwn-y

Demo time!

<div style="font-size: 0.73em">
{{% code file="/static/week2/eggHunterDemo-symlink.c" language="c" %}}
</div>

{{% /section %}}

---

## 🍳 Omelette Egghunter

A fancy egghunter that finds multiple payloads / eggs and combines them together.

<img src="gudetama.png" style="margin: 0; border: 0; background: initial; box-shadow: none" />

Useful when we have _multiple_ regions of controllable memory that can collectively hold the entire payload.

---

## `/bin/////////sh`

Sometimes we cannot inject `/bin/sh` directly.

{{% section %}}

> Why?

* An input parser that stops after the `NULL` character
  * _How do we check if we are at the end of a string?_
* An input sanitiser that removes the `NULL` character

```
                                      !! uh oh, null char!
                                      vv
_hs/ | push 0x0068732f | 68 2f 73 68 [00]
nib/ | push 0x6e69622f | 68 2f 62 69  6e 
```

---

> We have ways around this!

Pad our `/bin/sh` to `/bin//sh`

```bash
// BEFORE
_hs/ | push 0x0068732f | 68 2f 73 68 00
nib/ | push 0x6e69622f | 68 2f 62 69 6e
```

```bash
// AFTER
hs// | push 0x68732f2f | 68 2f 2f 73 68
nib/ | push 0x6e69622f | 68 2f 62 69 6e
```

---

```bash
some shellcode yadda yadda / b i n / / s h ... int 0x80 
```

> How do we push `0`'s onto the stack?

`xor eax, eax`  
`push eax`

etc...

---

> Sometime in the future...

Maybe `/bin/sh` is already in the program???

&nbsp;  

idk `¯\_(ツ)_/¯`

{{% /section %}}


---

## NOPsleds

{{% section %}}

NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP  
NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP  
NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP  
NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP  
NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP  
NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP  
NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP  
NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP  
NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP NOP  

---

In order to execute our shellcode, we need to know the exact start address of it... sometimes we can't do that, but we can jump near it and `nop`-sled away.

> The `nop` instruction (`\x90`) is a **single byte** instruction that does nothing

```bash 
Where we start execution      Where our shellcode actually is
|                             |
v                             v 
\x90 \x90 \x90 \x90 \x90 \x90 \x68 \x2f \x2f \x73 \x68 \x68..
```

{{% /section %}}

---

# Activities

> Lab 2 - Shellcode Exercices
