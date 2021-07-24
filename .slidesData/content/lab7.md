---
title: "Lab 7 Exercise Walkthrough"
layout: "bundle"
outputs: ["Reveal"]
date: 2021-07-24T19:04:42+10:00
---

{{< slide class="center" >}}

## Lab 7 Exercise Walkthrough

#### TL;DR

```bash
allocate
double free    (leak the address)
rewrite        (override second malloc)
dummy allocate 
pwn allocate   (got custom address region)
$$$
```

> [GitHub:lab7](https://github.com/featherbear/tutoring-unsw-21t2-cs6447/tree/master/lab7)

---

{{% section %}}

{{< slide background-image="1.png" class="center" >}}

```md
# allocate + double free
The free chunk points to itself!
```

---

{{< slide background-image="2.png" class="center" >}}

```md
# rewrite
Use the leak and write address+8 (effective hint address)
```

---

{{< slide background-image="3.png" class="center" >}}

```md
# dummy allocate
Get back the original chunk, pop it off the free list
```

---

{{< slide background-image="4.png" class="center" >}}

```md
# pwn allocate
Allocate a new chunk (with forged address) and write address
```

---

{{< slide background-image="5.png" class="center" >}}

🔥🔥🔥

{{% /section %}}