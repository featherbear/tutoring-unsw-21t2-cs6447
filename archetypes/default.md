---
title: "{{ replace .Name "-" " " | title }}"
layout: "bundle"
outputs: ["Reveal"]
date: {{ .Date }}
draft: true
---

{{< slide class="center" >}}

## {{ replace .Name "-" " " | title }}

---

