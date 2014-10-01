# Makefile for JamesM's kernel tutorials.
# The C and C++ rules are already setup by default.
# The only one that needs changing is the assembler 
# rule, as we use nasm instead of GNU as.

C_FILES := $(wildcard *.c)
O_C := $(addprefix obj/,$(C_FILES:.c=.o))
S_FILES := $(wildcard *.s)
O_S := $(addprefix obj/,$(S_FILES:.s=.s.o))

CC=i686-elf-gcc
CFLAGS=-std=c99 -nostdlib -Wall -Werror -ffreestanding -fno-exceptions 
#-fno-asynchronous-unwind-tables
ASFLAGS=-felf
ASBIN=-fbin

all: kludge $(O_C) $(O_S) link

clean:
	-rm obj/*

kludge:
	nasm -fbin -o obj/kludge kludge16.sb

obj/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $^

obj/%.s.o: %.s
	nasm $(ASFLAGS) -o $@ $^

link:
#	gcc $(CFLAGS) -T link.ld -o  kernel -lgcc $(OBJ_FILES)
	$(CC) $(CFLAGS) -T link.ld -Wl,-Map,obj/foo.map -o obj/kernel $(O_C) $(O_S)
	$(CC) $(CFLAGS) -T link_unaligned.ld -Wl,-Map,obj/foo_unaligned.map -o obj/kernel_unaligned $(O_C) $(O_S)
	wc -c obj/kernel_unaligned

.sb.o:
	nasm $(ASBIN) $<

test:
	screen qemu-system-x86_64 -smp 8 -no-kvm -curses -kernel obj/kernel -net none -m 256
stop:
	killall qemu-system-x86_64
