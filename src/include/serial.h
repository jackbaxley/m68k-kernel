#ifndef __serial_h
#define __serial_h

#include "cdefs.h"
#include "fs.h"

//#define NO_IRQ

#ifdef NO_IRQ

#define TX_BUFFER_SIZE 0
#define RX_BUFFER_SIZE 0

#else

#define TX_BUFFER_SIZE 256
#define RX_BUFFER_SIZE 256

#endif

typedef struct{
	
    volatile char rx_buffer[RX_BUFFER_SIZE];
    volatile char tx_buffer[TX_BUFFER_SIZE];
    volatile short tx_begin;
    volatile short tx_end;
    volatile short rx_begin;
    volatile short rx_end;
	
	char id;
	char used;
	
} serial_interface;

void serial_clear(serial_interface* si);
char serial_get_c(serial_interface* si);
char serial_check_c(serial_interface* si);
void serial_write_c(serial_interface* si,char c);
void serial_write_s(serial_interface* si,char *s);
void serial_init();
int get_time();
serial_interface* serial_get_interface(char id);

uint32_t serial_write(fs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buffer);
uint32_t serial_read(fs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buffer);
void serial_create_node(serial_interface* si, fs_node_t* node);


#endif