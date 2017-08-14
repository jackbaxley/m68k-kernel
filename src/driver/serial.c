#include "serial.h"

//#define NO_IRQ

#ifdef NO_IRQ

#define IRQ_ON 0x00
#define IRQ_OFF 0x00

#else

//#define IRQ_ON 0x0B
#define IRQ_ON 0x03
//#define IRQ_OFF 0x0A
#define IRQ_OFF 0x02

#endif

#ifdef NO_IRQ

#define TX_BUFFER_SIZE 0
#define RX_BUFFER_SIZE 0

#else

#define TX_BUFFER_SIZE 256
#define RX_BUFFER_SIZE 256

#endif

#define MRA		(*((char*)0x80000000))
#define SRA		(*((char*)0x80000001))
#define CSRA 	(*((char*)0x80000001))
#define BGR 	(*((char*)0x80000002))
#define CRA 	(*((char*)0x80000002))
#define RHRA	(*((char*)0x80000003))
#define THRA 	(*((char*)0x80000003))
#define IPCR 	(*((char*)0x80000004))
#define ACR 	(*((char*)0x80000004))
#define ISR 	(*((char*)0x80000005))
#define IMR 	(*((char*)0x80000005))
#define Dx6 	(*((char*)0x80000006))
#define Dx7 	(*((char*)0x80000006))
#define DxE 	(*((char*)0x8000000E))
#define STCC 	(*((char*)0x8000000F))



volatile char rx_buffer[RX_BUFFER_SIZE];
volatile char tx_buffer[TX_BUFFER_SIZE];
volatile short tx_begin;
volatile short tx_end;
volatile short rx_begin;
volatile short rx_end;

volatile int time;


void flush_rx(){
	rx_begin=rx_end;
}


void serial_init(){
	
	
	
	tx_begin=0;
	tx_end=0;
	rx_begin=0;
	rx_end=0;
	time=0;
	
	//initilize timer
	/*
	IMR = 0x00;
	Dx6 = 0x0F;
	Dx7 = 0xFF;
	ACR = 0x70;
	char c=DxE;
	*/
	
	/*
	(*((char*)0x80000005)) = 0x00;
	(*((char*)0x80000006)) = 0x0F;
	(*((char*)0x80000007)) = 0xFF;
	(*((char*)0x80000004)) = 0x70;
	char c=(*((char*)0x8000000E));
	*/
	//enable interrrupts
	
	//IMR=IRQ_ON;
	(*((char*)0x80000005))=IRQ_ON;
}

int get_time(){
	return time;
}

void serial_interrupt(){
	
	
	
	char isr=(*((char*)0x80000005));
	
	
	if(isr&0x08){
		volatile char c = *((char*)0x8000000F);
		time++;
		
	}
	if(isr&0x02){//rx irq
		//ubuf_put_c('R');
		char c=*((char*)0x80000001);
		while(c&0x01){
			c=*((char*)0x80000003);
			rx_buffer[rx_end]=c;
			rx_end++;
			if(rx_end>=RX_BUFFER_SIZE)rx_end-=RX_BUFFER_SIZE;//loop over
			if(rx_end==rx_begin){//if overflow
				//ubuf_put_s("RX_OVERFLOW");
				while(1){
					
				}
			}
			c=*((char*)0x80000001);
		}
	}
	if(isr&0x01){//tx irq
		*((char*)0x80000005)=IRQ_OFF;//disable duart tx irq
		//ubuf_put_c('T');
		while(((*((char*)0x80000001))&0x04) && tx_end != tx_begin){// while tx is rdy & buffer not empty
			char c=tx_buffer[tx_begin];
			tx_begin++;
			if(tx_begin>=TX_BUFFER_SIZE)tx_begin-=TX_BUFFER_SIZE;//loop over
			*((char*)0x80000003) = c;//send data
			
		}
		if(tx_end != tx_begin)//if buffer not empty
			*((char*)0x80000005)=IRQ_ON;//re enable duart tx irq
	}
}

void serial_write_c(char c){
	if(c=='\n'){
		serial_write_c(0x1B);
		//serial_write_c('[');
		serial_write_c('E');
		return;
	}
	
	
	#ifdef NO_IRQ
	ubuf_put_c(c);
	return;
	#endif
	
	
	//should i disable irq?
	//asm("or.w #0x0700, %sr");
	*((char*)0x80000005)=IRQ_OFF;
	if(tx_end==tx_begin && (*((char*)0x80000001))&0x04 ){// if buf is emty & rdy to tx
		//just send it straight to the duart
		*((char*)0x80000003)=c;
		
	}else{//otherwise put it in buffer
		tx_buffer[tx_end]=c;
		tx_end++;
		if(tx_end>=TX_BUFFER_SIZE)tx_end-=TX_BUFFER_SIZE;		
		short tx_next=tx_end+1;
		if(tx_next>=TX_BUFFER_SIZE)tx_next-=TX_BUFFER_SIZE;
		//if(tx_end==tx_begin){
		//	ubuf_put_s("TX_OVERFLOW");
		//	while(1);
		//}
		*((char*)0x80000005)=IRQ_ON;//re enable duart tx irq
		//asm("and.w #0xF8FF, %sr");
		while(tx_next == tx_begin){
			asm("nop");
		}
		
	}
	//asm("and.w #0xF8FF, %sr");
}

void serial_write_s(char *s){
	short i=0;
	char c=s[i++];
	while(c){
		serial_write_c(c);
		c=s[i++];
	}
	
}

char serial_get_c(){
	
	#ifdef NO_IRQ
	return ubuf_get_c();
	#endif
	
	//asm("or.w #0x0700, %sr");
	char c;
	while(rx_end==rx_begin){
		asm(
			"stop #0x3000"
		);
	}
	c=rx_buffer[rx_begin];
	rx_begin++;
	if(rx_begin>=RX_BUFFER_SIZE)rx_begin-=RX_BUFFER_SIZE;
	//asm("and.w #0xF8FF, %sr");
	return c;
}

char serial_check_c(){
	
	#ifdef NO_IRQ
	return ubuf_check_c();
	#endif
	
	//asm("or.w #0x0700, %sr");
	char c;
	if(rx_end==rx_begin){
		//asm("and.w #0xF8FF, %sr");
		return 0;
	}
	c=rx_buffer[rx_begin];
	rx_begin++;
	if(rx_begin>=RX_BUFFER_SIZE)rx_begin-=RX_BUFFER_SIZE;
	//asm("and.w #0xF8FF, %sr");
	return c;
}

void serial_clear(){
	serial_write_s("\x1B[2J\x1B[;H");
}

