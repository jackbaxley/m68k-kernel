CC=m68k-elf-gcc
CFLAGS= -c -Wall -m68030 -O2 -I src/include/

ASM=m68k-elf-as
AFLAGS=-m68030

LD=m68k-elf-ld
LFLAGS=-T ram.ld

C_SRC = $(wildcard src/*.c)\
		$(wildcard src/core/*.c)\
		$(wildcard src/memory/*.c)\
		$(wildcard src/process/*.c)\
		$(wildcard src/lib/*.c)\
		$(wildcard src/driver/*.c)
C_OBJ = $(C_SRC:.c=.o)
ASM_SRC = $(wildcard src/*.s)\
		$(wildcard src/core/*.s)\
		$(wildcard src/memory/*.s)\
		$(wildcard src/process/*.s)\
		$(wildcard src/lib/*.s)\
		$(wildcard src/driver/*.s)
		
ASM_OBJ = $(ASM_SRC:.s=.o)

OUTPUT = kernel.bin

all: $(ASM_SRC) $(C_SRC) $(OUTPUT)

$(OUTPUT): $(C_OBJ) $(ASM_OBJ)
	$(LD) -o $(OUTPUT) $(LFLAGS) $(C_OBJ) $(ASM_OBJ)

.c.o:
	$(CC) $(CFLAGS) $< -o $@
.s.o:
	$(ASM) $(AFLAGS) $< -o $@

clean:
	rm -rf $(C_OBJ)
	rm -rf $(ASM_OBJ)
	rm -rf $(OUTPUT)