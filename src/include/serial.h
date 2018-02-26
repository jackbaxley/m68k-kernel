#ifndef __serial_h
#define __serial_h

#include "cdefs.h"

//#define NO_IRQ

#ifdef NO_IRQ

#define TX_BUFFER_SIZE 0
#define RX_BUFFER_SIZE 0

#else

#define TX_BUFFER_SIZE 256
#define RX_BUFFER_SIZE 256

#endif

typedef struct{
	
    char rx_buffer[RX_BUFFER_SIZE];
    char tx_buffer[TX_BUFFER_SIZE];
    short tx_begin;
    short tx_end;
    short rx_begin;
    short rx_end;
	
	char id;
	char used;
	
} serial_interface;

void serial_clear(volatile serial_interface* si);
char serial_get_c(volatile serial_interface* si);
char serial_check_c(volatile serial_interface* si);
void serial_write_c(volatile serial_interface* si,char c);
void serial_write_s(volatile serial_interface* si,char *s);
void serial_init();
int get_time();
volatile serial_interface* serial_get_interface(char id);


#endif