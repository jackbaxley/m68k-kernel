OUTPUT_FORMAT(binary)

MEMORY{
	ROM (rx) : ORIGIN = 0, LENGTH = 128K
	RAM (rwx) : ORIGIN = 0x40000000, LENGTH = 2M
	}

REGION_ALIAS("REGION_TEXT", RAM);
REGION_ALIAS("REGION_RODATA", RAM);
REGION_ALIAS("REGION_DATA", RAM);
REGION_ALIAS("REGION_BSS", RAM);

SEARCH_DIR(/home/jack/opt/cross/lib/gcc/m68k-elf/5.3.0/m68000);

SECTIONS{
	
	
	
	.text : {
		k_begin = . ;
		*(.text.init)
		*(.vector)
		*(.text)
	} > REGION_TEXT
	
	.rodata : {
		*(.rodata)
		*(.rodata.*)
	} > REGION_RODATA

	
	.data : {
		*(.data)
	} > REGION_DATA
	
	
	.bss : {
		*(.bss)
		*(.stack)
	} > REGION_BSS
	k_end = . ;
	.filler : AT(0x40004000-1){
		BYTE(0);
	}
	
}
