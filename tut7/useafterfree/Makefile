CC := gcc
CFLAGS := -fno-stack-protector -no-pie -Q -v -m32 -O0
INTEL_ASM := -masm=intel

all: clean build

build:
	$(CC) $(INTEL_ASM) $(CFLAGS) -o useafterfree useafterfree.c

clean:
	rm -f useafterfree


