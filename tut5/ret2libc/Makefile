
# CFLAGS=-w -O0 -ggdb -std=c99 -static -D_FORTIFY_SOURCE=0 .intel_syntax noprefix -m32

CC         := gcc
CFLAGS     := -ggdb -O0 -std=c99 -m32
VULN_STACK := -fno-stack-protector
INTEL_ASM  := -masm=intel

all: clean build

build:
	$(CC) $(INTEL_ASM) $(VULN_STACK) $(CFLAGS)  -o ret2libc ret2libc.c

clean:
	rm -f ret2libc

aslr:
	sudo sysctl -w kernel.randomize_va_space=1	

