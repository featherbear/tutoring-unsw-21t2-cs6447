---
title: "Week 3"
layout: "bundle"
outputs: ["Reveal"]
date: 2021-06-13T19:44:40+10:00
---

{{< slide class="center" >}}

## Week 3

---

{{< slide content="secedu.goodfaith" >}}

---

{{< slide class="center" >}}

## cdecl, again

```c
mycoolfunc(arg1, arg2, arg3);
                               | 0xFF
push arg3                      |      arg3
push arg2                      |      arg2
push arg1                      |      arg1
call mycoolfunc                |      ret_addr  <-- esp
                               | 0x00
```

---

## Format Strings

{{% section %}}

> Used in C library functions

* `[s,sn,v,va,...]printf`
* `[s,sn,v,va,...]scanf`
*  etc

---

> `printf(format, ...vargs)`

&nbsp;  

`printf("%s world!", "hello");`  

&nbsp;  

`printf("👋 🌏!");`

{{% /section %}}

---

## Format Specifier

{{% section %}}

<span style="font-size: 0.75em;">`%<flags><width><precision><modifier>`<u><b>`<type>`</b></u></span>

* `%s` - print as string  
* `%x` - print as hex  
* `%p` - print as pointer  
* `%c` - print as char  
* `%d` - print as (signed) int  
* `%u` - print as (unsigned) int
* `%n` - <u>write</u> the number of printed bytes...

---

<span style="font-size: 0.75em;">`%<flags><width><precision>`<u><b>`<modifier>`</b></u>`<type>`</span>

* `%d` - print as (signed) int  
* `%hd` - print as half (signed) int  
* `%hhd` - print as half half (signed) int  

* `%n` - write as int
* `%hn` - write as half int
* `%hhn` - write as half half int

---

<span style="font-size: 0.75em;">`%<flags><width>`<u><b>`<precision>`</b></u>`<modifier><type>`</span>

&nbsp;  
Output **maximum** limit
&nbsp;  

```c
printf("%.3s", "abcdef");

$> "abc"
```

```c
printf("%.5f", 4 * 1.0 / 3);

$> 1.33333
```

---

<span style="font-size: 0.75em;">`%<flags>`<u><b>`<width>`</b></u>`<precision><modifier><type>`</span>

&nbsp;  
Output **minimum** limit
&nbsp;  

```c
printf("%d", 10);

$> 10
```

```c
printf("%5d", 10);

$>    10
```

```c
printf("%10d", 10);

$>         10
```

---

<span style="font-size: 0.75em;">`%`<u><b>`<flags>`</b></u>`<width><precision><modifier><type>`</span>

&nbsp;  
Modifiers, eh.
&nbsp; 

* `-` - Left-align the output
* `+` - Prepends a plus for +ve signed-numeric types
* ` ` - Prepends a space for +ve signed-numeric types
* `0` - Prepends zeros
* `'` - Show thousandths separators
* `#` - misc.

---

{{< slide class="center" >}}

jks one more

---

<span style="font-size: 0.75em;">`%`<u><b>`<parameter>`</b></u></span><span style="font-size: 0.5em;">`<flags><width><precision><modifier><type>`</span>

&nbsp;  
Allows you to specify a certain index from the vargs.  
&nbsp;  

> `%<idx>$...`

```c
printf("%3$c %1$s %2$d", "string", 50, 'c')

$> c string 50
```

{{% /section %}}

---

### Format String Vulnerabilities

What if you don't provide any arguments?  

```c
printf("%d %d %d %d %s)
```

* Responsibility of the caller to push the relevant data
* Program will just grab items off the stack

---

#### Read Primitive

{{% section %}}

For numbers...

* `%d` will read the next item on the stack
* `%20$d` will read the 20th item from the stack

For hex...

* `%p` will read the next item on the stack
* `%20$p` will read the 20th item from the stack

For addresses...

* `%s` will read the next item (as a pointer) on the stack
* `%20$s` will read the 20th item (pointer) on the stack

---

{{< slide class="center" >}}

> Demo: `./leakValues`

---

Your C code may not reflect the correct order of function locals _(bc compiler optimisations)_

> Demo: `./varOrder`

{{% /section %}}

---

#### Write Primitive

{{% section %}}

> `%n` format specifier

Writes the number of printed bytes to the next item (pointer) on the stack

&nbsp;  

> `%7$n` format specifier

Writes the number of printed bytes to the seventh item (pointer) on the stack

---

##### Alignment

Sometimes our values aren't aligned to the 4-byte boundary. We just need to pad our values then :)

> Demo: `./printfRunner`

---

{{< slide class="center" >}}

> Demo: `./setValue`

{{% /section %}}

---

## TL;Dr

{{% section %}}

* If our address is on the stack, we can read and write from that address
* If our format specifier buffer is on the stack, we can read and write from addresses in the buffer

> _i.e. write `0xdeadbeef` into our buffer, then write into that address_

---

* Find our buffer region (relative the format string)
* Pad the payload (if needed)
* Write the 4-byte address(es)
* Write bytes (`...` or `%...$c`)
* Write into the address (`%...$n / hn / hhn`)

{{% /section %}}

---

## Write Optimisation

* Write one 4-byte value (max = 2^32 = 4,294,967,296)
* Write two 2-byte values (max = 2 x 2^16 = 131,072)
* Or... write four 1-byte values (max = 4 x 2^8 = 1024)

Craft your write payload wisely!  
Sometimes you don't have alot of buffer space

> Demo: `./setValueBig` - better-ish  

> Demo: `./setValueBig` - better

---

## Where Write?

{{% section %}}

* Variables
* PLT / GOT (dynamic library function calls)
* Function hooks (malloc hook, free hook, __atexit)

---

#### GOT

GOT - Global Offset Table  
PLT - Process Linkage Table

* Calls to library functions go through a redirection.  
* Entries in the GOT are unset until first called
  * They initially point to some lookup handler
    * The lookup handler replaces the uninitialised value with the correct address

---


* `got` command - shows all of the linkable functions  
* `vmmap` command - shows all the memory regions
  * useful for figuring out offsets later on

{{% /section %}}

---

### Memory Protection

{{% section %}}

* Memory Corruption
    * stack reordering
    * random padding
    * stack canaries
    * FORTIFY
    * RELRO

---

* Post-corruption code execution
    * ASLR + PIE
    * NX
    * PAC
    * Hypervisor

---

* PIE - Program can only contain relative jumps
  * Requires ASLR to be on

* FORTIFY - Stop string vulnerabilities
  * Disables `%n` if the fmtstr is in writeable memory

* RELRO - Relocation Read-Only
    * GOT is read-only
    * Partial - GOT appears before BSS (globals)
    * Full RELRO - rip.

{{% /section %}}

---

## General Pointer Addresses

* `0x565` - Binary base with PIE enabled
* `0x804` - Binary base with PIE disabled
* `0xf7f` - Library base
* `0xff..` - Stack base

---

{{< slide class="center" >}}

# Activities

> Lab 3: `./format_prac`

also, wargames 2 are due tonight at 5.59pm!
