---
title: "Week 4 - Source Auditing"
layout: "bundle"
outputs: ["Reveal"]
date: 2021-06-16T00:07:27+10:00
---

{{< slide class="center" >}}

## Week 4

##### Source Auditing

---

{{< slide content="secedu.goodfaith" >}}

---

{{% section %}}

### Wargames 2 Review

```
Overall Submissions: 57 / 74

2-stack-dump  24	32%
2-simple      54	73%
2-shellz      44	59%
2-find-me     36	49%
```

### Wargames 3 Review

```
Overall Submissions: 49 / 74

3-meme       47 	64%
3-formatrix  44 	59%
3-tetris     35 	47%
3-elonmusk   26 	35%
```

As of 2AM 22/06/2021

---

#### RE: RE

* Simplify your code!
  * `i += 1`
    * `i++`
  * `i & 1 == 0`
    * `i % 2`
* Check your return values
  * return
  * or... return 0;
  * or... return 1;

---

#### e.g RE

{{% code file="/static/week4/re-feedback/returns.c" language="c" %}}

---

#### RE: RE

* Compile the code and check for similarities and differences
  * Probs turn off security protections and optimisations
* Tooling scripts, format specifier!
  * \`\`\``python3`
  * `blah`
  * \`\`\`

---

{{< slide class="center" >}}

#### Walkthrough

> ./stack-dump

---

#### Walkthrough

> ./stack-dump

&nbsp;  

&nbsp;  

&nbsp;  

##### Tips

* `fit() / flat()`
* `elf.symbols[funcName]`

{{% /section %}}

---

{{< slide class="center" >}}

# Week 4

## Source Auditing

---


```c
int flags = 1;
if (flags & FLAG ) {
    printf("TRUE\n");
} else {
    printf("FALSE\n");
}
```

{{% fragment %}}

```c
int flags = 1;
if (flags & FLAG != 0) {     //
    printf("TRUE\n");        // What could possibly go wrong?
} else {                     // works on my machine TM
    printf("FALSE\n");       //
}
```

{{% /fragment %}}

{{% fragment %}}

```c
if (flags & (FLAG != 0)) {
    printf("TRUE\n");
} else {
    printf("FALSE\n");
}
```

> Operation Precedence

{{% /fragment %}}

---

## Operation Precedence

```c
flags = flags + 1 % 2;

printf("Flags: %d\n",flags);
```

* A. `(flags + 1) % 2;`
* B. `flags + (1 % 2);`
* C. None of the above
* D. All of the belo-_wait what_

{{% fragment %}}

> ~ it be B 🐝

B for <s>bee</s> <s>buzzz</s> <s>bug</s> brrrrrrrrrr
{{% /fragment %}}

---

### Some Real World <s>Bug</s> brrrr

{{% section %}}

<style>
code {font-size: 0.7em;}
</style>

```c
char pt[] = "THISISSOMEDATAOFSOMESORT";
char env_script_name[] = "USER_CONTROLLED_ENV1";
char env_path_info[] =  "USER_CONTROLLED_ENV2";

int ptlen = strlen(pt) - strlen(env_script_name);
int path_translated_len = ptlen + env_path_info ? strlen(env_path_info) : 0;
char *path_translated = NULL;
path_translated = (char *) malloc(path_translated_len + 1);
memcpy(path_translated, pt, ptlen);
if (env_path_info) {
    memcpy(path_translated + ptlen, env_path_info, path_translated_len - ptlen);
}
```

> What's wrong with this code?

---

```c
int path_translated_len = ptlen + (env_path_info ? strlen(env_path_info) : 0);
```

###### How it started
```c
if (env_path_info) {
    path_translated_len = ptlen + strlen(env_path_info);
} else {
    path_translated_len = ptlen;
}
```

###### How it's going
```c
if (ptlen + env_path_info) {                            // bit of a rip.
    path_translated_len = strlen(env_path_info);
} else {
    path_translated_len = 0;
}
```

---

###### SO WHAT?

```c
int path_translated_len = (ptlen + env_path_info) ? strlen(env_path_info) : 0;
...
if (env_path_info) {
    memcpy(path_translated + ptlen, env_path_info, path_translated_len - ptlen);
}
```

* if `ptlen = -env_path_info`
  * then `path_translated_len = 0`
* `path_translated_len - ptlen`
  * ...
  * `0 - ptlen`
  * `-ptlen`   ... negative param in `memcpy`?

---

###### valve pls fix 👉🥺👈

&nbsp;  

```c
int path_translated_len = ptlen + (env_path_info ? strlen(env_path_info) : 0);
```

&nbsp;  

```
                            ____________________________
                            < Brackets people, brackets! >
                            ----------------------------
                                    \   ^__^
                                    \  (oo)\_______
                                        (__)\       )\/\
                                        :p ||----w |
                                            ||     ||
```

{{% /section %}}

---

## Dangling Braces

```c
if (x == 0) {
    if (y == 0) error();
else {
    z = x + y;
    fclose(&z);
}
```

{{% fragment %}}
```c
if (x == 0){
    if (y == 0)
        error();
    else {
        z = x + y;
        fclose(&z);
    }
}
```
{{% /fragment %}}

---

## Order of Operations

```c
i = 0
while (i < n) {
    y[i] = x[i++];
}
```

What will happen here?

{{% fragment %}}
# 🤷

The compiler decides
{{% /fragment %}}

---

{{% fragment %}}
## Integer Overflow
{{% /fragment %}}

```c
u_int strLen = strlen(userinput);
int buffsize = strLen + 11;

char *mem = malloc(buffsize);
strncpy(mem, "this/path/", 10);
strncpy(mem[10], userinput, strLen);
```

What can go wrong?

{{% fragment %}}
1 2 3 4 5 6 7 8 9 0 anddd iiii oooop!
{{% /fragment %}}

---

## Format Strings

```
char *var;
printf(var);
fprintf(stderr, var);
vsnprintf(var2, strlen(var2), var);
```

(Week 3 content)

---

## Heap

##### Use after free

```c
char *var = malloc(10);
free(var);
printf("%s\n", var);          // !!!!!
```

##### Double Free

```c
char *var = malloc(10);
free(var);                    // !!!!!
char *var2 = malloc(10);
free(var2);
free(var);                    // !!!!!
```

More on these later on!

---

## NULL Mapping

```c
char *a;
vuln_syscall_sets_a_NULL(a);
char b[] = "string";
strcpy(a, b);                 // strcpy(NULL, b);
```

Usually we would get a segfault...  
But what if we did a memory map (`mmap`)?

&nbsp;  

&nbsp;  

```c 
mem = mmap(NULL, 0x1000, PROT READ | PROT WRITE | PROT EXEC, MAP FIXED | MAP ANONYMOUS | MAP PRIVATE, 0, 0);
```

privilege escalation maybe?

---

## OBOE

```c
char *var = malloc(10);
if (var == NULL) return;

for (int i = 0; i <= 10; i++) {         // This will loop 11 times!
    var[i] = argv[2][i];
}
```

---

### TL;DR

![](2021-06-22-03-40-41.png)

---

### Midsem Exam

Friday 2nd July

* 1x Buffer Overflow
* 1x Format String
* 1x something else (sorry in advanced)

2 hour exam, open from 10am - 6pm

Practice!

---

### Know Your Tools

* Resource Page: [https://featherbear.cc/tutoring-unsw/](featherbear.cc/tutoring-unsw/)
* pwntools
  * Templates
  * p32 / pause / attach / fit / ELF.got / ELF.symbols
  * `DEBUG`
* gdb / pwndgb
  * got, stack, vmmap, break, next, finish, continue

---

{{< slide class="center" >}}

## Any questions?

* Anything else?
* Fuzz?????

&nbsp;  

&nbsp;  

## Activities

* Find some bugs in the lab exercise
* Do your wargames
