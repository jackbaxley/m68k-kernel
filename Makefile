all:
	m68k-elf-gcc -c -O0 -m68030 kernel.c
	m68k-elf-gcc -c -O0 -m68030 serial.c
	m68k-elf-gcc -c -O0 -m68030 memory.c
	m68k-elf-gcc -c -O0 -m68030 process.c
	
	m68k-elf-gcc -c -O2 -m68030 printf.c
	m68k-elf-gcc -c -O2 -m68030 abort.c
	m68k-elf-gcc -c -O2 -m68030 memmove.c
	m68k-elf-gcc -c -O2 -m68030 strlen.c
	m68k-elf-gcc -c -O2 -m68030 memcmp.c
	m68k-elf-gcc -c -O2 -m68030 memset.c
	m68k-elf-gcc -c -O2 -m68030 memcpy.c
	m68k-elf-gcc -c -O2 -m68030 puts.c
	m68k-elf-gcc -c -O2 -m68030 putchar.c
	
	m68k-elf-as -m68030 kernel_s.s -o kernel_s.o
	m68k-elf-as -m68030 vector.s -o vector.o
	m68k-elf-as -m68030 serial_s.s -o serial_s.o
	m68k-elf-as -m68030 process_s.s -o process_s.o
	
	m68k-elf-as -m68030 prog1.s -o prog1.o
	
	m68k-elf-ld -o kernel.bin -T ram.ld prog1.o kernel.o serial.o kernel_s.o vector.o serial_s.o memory.o process.o process_s.o printf.o abort.o memmove.o strlen.o memcmp.o memset.o memcpy.o puts.o putchar.o 