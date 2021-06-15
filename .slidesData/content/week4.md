---
title: "Week 4"
layout: "bundle"
outputs: ["Reveal"]
date: 2021-06-16T00:07:27+10:00
draft: true
---

{{< slide class="center" >}}

## Week 4

---

{{< slide content="secedu.goodfaith" >}}

---

## Wargames 2 Review

{{% section %}}

```
Overall Submissions: 54 / 80

Overall Completion
stack-dump	24	30%   +1
simple		51	64%   +1?
shellz		43	54%
find-me		36	45%
```

---

#### RE: RE

* Simplify your code!
  * `i += 1` --> `i++`
  * `i & 1 == 0` --> `i % 2`
* Check your return values
  * return
  * or... return 0;
  * or... return 1;
* Compile the code and check for similarities and differences
  * Probs turn off security protections and optimisations
* Tooling scripts, format specifier!
  * `\`\`\`python3`

---

#### e.g RE

{{% code file="/static/week4/re-feedback/returns.c" language="c" %}}

---

{{< slide class="center" >}}

#### Walkthrough

> ./stack-dump

{{% /section %}}

##

---

