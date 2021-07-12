---
title: "Week 7 - Heap Exploitation"
layout: "bundle"
outputs: ["Reveal"]
date: 2021-07-11T18:48:56+10:00
draft: true
---

{{< slide class="center" >}}

## Week 7

Heap Exploitation

---

{{< slide content="secedu.goodfaith" >}}

---

##### A summary of COMP6447

{{% section %}}

* Buffer Overflows
* Shellcode
* Format Strings
* Return Oriented Programming
* Today: Heap Exploitation

---

#### Buffer Overflows

* Vulnerable memory write functions that 
  * Allow arbitrary input lengths
  * Allow longer than allocated lengths
* Allows control of variables, stack frame, etc...
* Not always `gets` / `fgets`
  * `strcpy`
* Mitigation: Stack canaries
  * Defeat: Leak the stack canary
  * Guess it

---

#### Shellcode

* Program is tricked into running arbitrary instructions supplied by the user
* Pop a shell?
* Mitigation
    * NX bit
        * Defeat: ROP
        * Disable NX
  * Small buffer
        * Defeat: Egg Hunters

---

#### Format Strings

* `[..]printf` execution on user-controlled input
* Allows arbitrary memory read
* Allows arbitrary memory write
* Mitigation: Be a better programmer™
  * Defeat: Find bad programmers /s

---

#### Return Oriented Programming

* Exploits the multi-byte nature of instructions, where parts of instructions can be identified as their own instruction
* Bypasses the NX bit, as the instruction address are part of the program's `text` region (executable)
* Mitigation: Quite hard. PAC?
    * Be a better programmer™ x2

---

#### General Protections

* ASLR - System-level address randomisation
* PIE - Application-level address randomisation
* RELRO - Prevents modification of the GOT
* NX - Prevents execution in heap / stack

Leak!

---

#### Attacking the GOT

* If <span data-rgb="g">no RELRO</span> - trivial
* If <span data-rgb="y">partial RELRO</span> - overwrite an uninitialised entry
* If <span data-rgb="r">full RELRO</span> - ... overwrite a hook?

<!--
* r - #e41a5c
* y - #e5c06e
* g - green
-->

<link rel="stylesheet" type="text/css" href="moreRGB.css" />

<script>
{
    for (let elem of document.querySelectorAll('[data-rgb]')) {
        let type = elem.dataset['rgb'].toUpperCase()
        elem.innerHTML = elem.innerText.trim().split("").map(s => `<span>${s}</span>`).join("")
        elem.classList.add(`anim-text-flow${type}`)
    }
}
</script>

---

#### Uncertain Memory

* NOP-sled
* RET-sled
* Egg Hunters
    * ROP Hunters

{{% /section %}}

---

## Heap

##### Chunks

A chunk can be in two states **free**, or **in-use**

<img width="600" src="Snipaste_2021-07-11_21-23-47.png" alt='chonk' />

---

##### Header Structure

{{% section %}}

#### Chunk :: In Use

* <s>Previous Size</s>
* Size
* AMP*
* **Payload**

Note: `malloc` returns the address of the payload  

<div style="text-align: left;">

<span style="font-size: 0.8em; margin-left: 100px; text-decoration: underline">*AMP</span>
<div style="font-size: 0.8em;"> 

* **A**llocated Arena
* **M**emory Mapped
* **P**revious is Use
</div> 
<div>

---

##### Header Structure

#### Chunk :: Free

* Previous Size
* Size
* AMP
* fwd
* back
* <span style="font-size: 0.6em">fd_nextsize</span>
* <span style="font-size: 0.5em">fd_previoussize</span>
* <span style="font-size: 0.4em">previoussize</span>
* <span style="font-size: 0.3em">blah blah blah...</span>

---

![](Snipaste_2021-07-12_18-26-16.png)

* Chunks are at LEAST `4 * sizeof(void*)`
* Hmm mixing data and control 🤔

<!-- Where does it say if the chunk is in use?  
A: It doesn't  
E: It relies on its reference -->

<!-- Since chunks are byte-aligned to 8 bytes (0b1000)
the last three bits for the size is never used 

We use it for the AMP
A - Is it in the main arena
M - Was the chunk big enough to be allocated on the heap
P - is the previous chunk in use -->

<!-- ARENA - thread local data -->

<!-- PREV_SIZE NOT USED WHEN ALLOCATED - also appears in previous colour

IN USE CHUNKS ARE NOT TRACKED BY THE ARENA -->

{{% /section %}}

---

### free(chunk)

{{% section %}}

> Freeing chunks need to be fast!

* Instead of clearing the memory, we just unlink it!
* Programs use several different types of 'bins' to efficiently store information about free'd memory.

---

Programs use several different types of 'bins' to efficiently store information about free'd memory.

* Fast Bins
* Unsorted Bins
* Small Bin
* Large Bin
* TCache

---

##### Fast Bins

* There are 10 fastbins
* Small chunks are stored in size-specific bins
    * 16, 24, 32, 40, 48, 56, 64, 72, 80, 88 bytes
* Each fastbin is essentially a <u>single</u> linked list
* For each sized fastbin
    * Freed chunks added into to the <u>start</u> of the fastbin
    * Chunks are not combined with adjacent chunks
    * The first item in a fastbin is next allocated chunk
        * aka the last freed chunk of that fastbin

---

##### Unsorted Bins

* For large chunks, when `free`'d, chunks are stored in a single bin of varying chunk sizes
* Later sorted by malloc to be optimised

&nbsp;  

##### Other Bins

The normal bins are divided into 62 **small bins** (each bin has chunks of the same size), and two **large bins** (where each large bin has chunks of similar size)

<!-- These chunks combine together

![](https://media3.giphy.com/media/26gR2f01UTynjCPNS/giphy.gif) -->

---

### TCache

> Thread-local Cache

Faster than a global cache!

Arbitrarily sized bins that have a limit of 7 chunks (by default).  
If 7 chunks have been free'd, tcache won't be used

Note: `calloc` doesn't use the tcache

{{% /section %}}

---

#### Use After Free

When a chunk is freed, part of its contents is used as metadata... If we tamper with its contents, we can corrupt the linked list!
This allows us to control the addresses of a future malloc chunk!

_e.g. Modify the forward pointer..._  
_malloc returns our own address!_

```c
                         // Bin: NULL
free(chunk)              // Bin: chunk -> (HEAD = NULL)
chunk = 0x41414141       // Bin: chunk -> 0x41414141 -> ???
dummy = malloc(...)      // Bin: 0x41414141 -> ???
pwn = malloc(...) ///////// pwn = 0x41414141
```

---

#### Double Free

{{% section %}}
Systems check that you haven't `free`'d a memory address twice <u>**in a row**</u>.  

```c
free(a);
free(a);

// free(): double free detected
// Aborted (core dumped)
```

But... it doesn't prevent this

```c
free(a);
free(dummy);
free(a);

// ???
```

---

When a chunk is freed the second time...

```c
                      // Bin: NULL
free(chunk);          // Bin: chunk -> (HEAD = NULL)
free(chunk);          // Bin: chunk -> (HEAD = chunk) -> NULL
puts(chunk.next) /////// chunk

// $$$
```

or maybe

```c
                      // Bin: NULL
free(chunk);          // Bin: chunk -> (HEAD = NULL)
free(chunk);          // Bin: chunk -> (HEAD = chunk) -> NULL
malloc(...)     // some val
malloc(...)
malloc(...)     // the same val!???
// $$$
```

{{% /section %}}

---

#### pwndbg

* `vis_heap_chunks`
* `heap`
* `arena`
* `bins`
    * `___bins`

&nbsp;  

#### Demo time!

> Double Free  
> Use After Free

---

> [heap-exploitation.dhavalkapil.com](https://heap-exploitation.dhavalkapil.com)

---

### Dates

* Today, Tuesday, Week 7 @ 5.59pm - Wargames 5 Due
* This Sunday 18/07 @ 5.59pm - Mid-point Fuzzer Due
<!-- * Monday, Week 9 - Wargames 6 Due -->

&nbsp;  

### Activities

🐏 (s)heap 🐑
