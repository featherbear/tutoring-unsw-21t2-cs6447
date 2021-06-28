CC         := gcc
CFLAGS     := -ggdb -O0 -std=gnu99 -m32 -no-pie
VULN_STACK := -fno-stack-protector
INTEL_ASM  := -masm=intel

all: clean build

build:
	$(CC) $(INTEL_ASM) $(VULN_STACK) $(CFLAGS)  -o ropme ropme.c

clean:
	rm -f ropme





