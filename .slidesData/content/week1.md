---
title: "Week 1"
layout: "bundle"
outputs: ["Reveal"]
date: 2021-05-31T23:56:30+10:00
---

{{< slide class="center" >}}

## Week 1

---

# Who, me?

{{% section %}}

![](../me.jpg)

---

`andrew.j.wong@unsw.edu.au`

* Teams ^
* SECedu Slack - `featherbear`

{{% /section %}}

---

### New number, who dis?


Share something interesting on your phone's photo gallery (or meme collection) that describes yourself

<img src="icebreakers.jpg" height=350 />  
<span style="font-size: 12px;">Any society plugs hint hint</span>

---

# Housekeeping

{{% section %}}

🍉 Food - Sure thing

🎥 Recordings - Yes, next slide

👩‍🏫 Lab Attendance - Not marked  

💡 This room has <span class="anim-text-flow" id="rgb">RGB lighting</span>

<link rel="stylesheet" type="text/css" href="rgb.css" />

<script>
document.getElementById("rgb").innerHTML = document.getElementById("rgb").innerText.trim().split("").map(s => `<span>${s}</span>`).join("")
</script>

---

## Resources

> [featherbear.cc/tutoring-unsw](https://featherbear.cc/tutoring-unsw)

{{% /section %}}

---

# Course Stuff

{{% section %}}

## W11A

The **W** in <u>W</u>11A stands for W**Tuesday**, duh

&nbsp;  
&nbsp;  

> _Don't @ me it's not my fault_

---

## Contact

> `cs6447@cse.unsw.edu.au`  

> Slack - seceduau.slack.com  
> `#cs6447`  
> `#cs6447-21t2-w11a`

---

## Assessments

* Weekly Wargames (30%)
* Fuzzer Assignment (20%)
* Mid-term Exam (10%)
* Final Examination (40%)

Check the course outline for the schedule

---

## RE: Wargames

They can be hard.. Start early!  

> "Most students should be able to do 75-80% of the challenges" - Adam 🍌

`correct flag != full mark`

Feel free to work together, but submit your own flags and _tooling_ - more on this later..

{{% /section %}}

---

{{< slide content="secedu.goodfaith" >}}

---

{{< slide class="center" >}}

# <s>Kahoot</s>

{{% fragment %}}

Kahoot now has a limit of 10 players on the free plan :(  
<span style="font-size: 12px; float: right">Should write my own eh.</span>

> [joinmyquiz.com](http://joinmyquiz.com)

{{% /fragment %}}

---

# Lecture Review

{{% section %}}

Questions?

---

* Endianness
* ASLR
* Register layout (next slide)
* Buffer Overflows
* Stack Canaries

{{% /section %}}

---

## x86 Registers

![](2021-06-01-00-32-40.png)

---

## `ebp`,  `esp`

![](2021-06-01-00-35-18.png)

--- 

## Buffer Overflows

{{% section %}}

```bash
[ | | | | | | | | | | | | | | | | | | | | | | | | | | ]blah
```

```c
gets(data)

$> abcdefg...xyz
```

```bash
[a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|-]blah
```

---

```bash
[ | | | | | | | | | | | | | | | | | | | | | | | | | | ]blah
```

```c
gets(data)

$> abcdefg...xyzAAAAAAA
```

```bash
[a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|A]AAAAAA
```

{{% /section %}}

---

## Your Hacker Environment

> [featherbear.cc/tutoring-unsw](https://featherbear.cc/tutoring-unsw)

* Linux (or WSL - maybe)
* Binary Ninja
* pwndbg
* pwntools
* `pwndocker`

---

# Demo

<!-- checksec, cyclic, gdb

pwndbg - context, stack, b, s, n, f, finish, c

p, x
x/5x $ebp

p32() pack 32 - little endian

cyclic 500
cyclic 

template script
tooling

from pwn import *
p = remote()
p = process()

sendline()

p.recvline()

p.interactive()


-->

--- 

# Your Turn

[WebCMS] > [Labs] > [Lab 1]

* One binary, two vulnerabilities
* Don't look at the source code, unless you're really stuck!

---

# Walkthrough

