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

