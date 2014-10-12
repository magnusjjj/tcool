C_FILES := $(wildcard *.c)
S_FILES := $(wildcard *.s)
O_S := $(addprefix obj/,$(S_FILES:.s=.s.o))
O_C := $(addprefix obj/,$(C_FILES:.c=.o))

CC=i686-elf-gcc
CFLAGS=-std=c99 -nostdlib -Wall -Werror -ffreestanding -fno-exceptions -pedantic -Wextra
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
	$(CC) $(CFLAGS) -T link.ld -Wl,-Map,obj/foo.map -o obj/kernel $(O_C) $(O_S)
	$(CC) $(CFLAGS) -T link_unaligned.ld -Wl,-Map,obj/foo_unaligned.map -o obj/kernel_unaligned $(O_C) $(O_S)
	wc -c obj/kernel_unaligned

.sb.o:
	nasm $(ASBIN) $<

test:
	screen qemu-system-x86_64 -smp 8 -no-kvm -curses -kernel obj/kernel -net none -m 256
testno:
	qemu-system-x86_64 -smp 8 -no-kvm -curses -kernel obj/kernel -net none -m 256
testcrash:
	qemu-system-x86_64 -smp 8 -no-kvm -nographic -kernel obj/kernel -net none -m 256 > allout.txt 2>&1
stop:
	killall qemu-system-x86_64
