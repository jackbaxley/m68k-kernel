CC=m68k-elf-gcc
CFLAGS= -c -Wall -m68030 -O0 -I src/include/

ASM=m68k-elf-as
AFLAGS=-m68030

LD=m68k-elf-ld
LFLAGS=-T ram.ld

SRCDIR = src
OBJDIR = obj
BINDIR = bin

C_SRC = $(wildcard $(SRCDIR)/*.c)\
		$(wildcard $(SRCDIR)/core/*.c)\
		$(wildcard $(SRCDIR)/memory/*.c)\
		$(wildcard $(SRCDIR)/process/*.c)\
		$(wildcard $(SRCDIR)/lib/*.c)\
		$(wildcard $(SRCDIR)/driver/*.c)
C_OBJ = $(C_SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

ASM_SRC = $(wildcard $(SRCDIR)/*.s)\
		$(wildcard $(SRCDIR)/core/*.s)\
		$(wildcard $(SRCDIR)/memory/*.s)\
		$(wildcard $(SRCDIR)/process/*.s)\
		$(wildcard $(SRCDIR)/lib/*.s)\
		$(wildcard $(SRCDIR)/driver/*.s)	
ASM_OBJ = $(ASM_SRC:$(SRCDIR)/%.s=$(OBJDIR)/%.o)

OUTPUT = kernel.bin

all: $(ASM_SRC) $(C_SRC) $(OUTPUT)

init:
	mkdir $(OBJDIR)
	mkdir $(OBJDIR)/core
	mkdir $(OBJDIR)/memory
	mkdir $(OBJDIR)/process
	mkdir $(OBJDIR)/lib
	mkdir $(OBJDIR)/driver

$(OUTPUT): $(C_OBJ) $(ASM_OBJ)
	@$(LD) -o $(OUTPUT) $(LFLAGS) $(C_OBJ) $(ASM_OBJ)
	@echo "Built "$<" successfully!"

$(C_OBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) $< -o $@
	@echo "Compiled "$<" successfully!"

$(ASM_OBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.s
	@$(ASM) $(AFLAGS) $< -o $@
	@echo "Compiled "$<" successfully!"

clean:
	rm -rf $(C_OBJ)
	rm -rf $(ASM_OBJ)
	rm -rf $(OUTPUT)