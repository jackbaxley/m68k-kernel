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


//68681 Duart Registers
#define MRA		(*((char*)0x80000000)) //RW	Mode Register A
#define SRA		(*((char*)0x80000001)) //R	Status Register A
#define CSRA 	(*((char*)0x80000001)) //W	Clock Select Register A
#define BGR 	(*((char*)0x80000002)) //R 	BRG Test
#define CRA 	(*((char*)0x80000002)) //W	Command Register A
#define RHRA	(*((char*)0x80000003)) //R	Rx Holding Register A
#define THRA 	(*((char*)0x80000003)) //W	Tx Holding Register A
#define IPCR 	(*((char*)0x80000004)) //R	Input Port Change Register
#define ACR 	(*((char*)0x80000004)) //W	Aux. Control Register
#define ISR 	(*((char*)0x80000005)) //R	Interrupt Status Register
#define IMR 	(*((char*)0x80000005)) //W	Interrupt Mask Register
#define CTU 	(*((char*)0x80000006)) //R	Counter/Timer Upper Value
#define CRUR 	(*((char*)0x80000006)) //W	C/T Upper Preset Value
#define CTL 	(*((char*)0x80000007)) //R	C/T Lower Value
#define	CTLR 	(*((char*)0x80000007)) //W	C/T Lower Preset Value
#define	MRB 	(*((char*)0x80000008)) //RW	Mode Register B
#define	SRB 	(*((char*)0x80000009)) //R	Status Register B
#define	CSRB 	(*((char*)0x80000009)) //W	Clock Select Register B
#define	XTEST 	(*((char*)0x8000000A)) //R	1x/16x Test
#define	CRB 	(*((char*)0x8000000A)) //W	Command Register B
#define	RHRB 	(*((char*)0x8000000B)) //R	Rx Holding Register B
#define	THRB 	(*((char*)0x8000000B)) //W	Tx Holding Register B
#define	IVR 	(*((char*)0x8000000C)) //RW	Interrupt Vector Register
#define	IP06 	(*((char*)0x8000000D)) //R	Input Ports IP0 to IP6
#define	OPCR 	(*((char*)0x8000000D)) //W	Output Port Conf Register
#define STACC 	(*((char*)0x8000000E)) //R	Start Counter Command
#define SOPBC 	(*((char*)0x8000000E)) //W	Set Output Port Bits Command
#define STOCC 	(*((char*)0x8000000F)) //R	Stop Counter Command
#define ROPBC 	(*((char*)0x8000000F)) //W	Reset Output Port Bits Command



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
	IMR=IRQ_ON;
}

volatile serial_interface* serial_get_interface(char id){
	volatile serial_interface* ret;
	//if(id=='A')ret=&si_A;
	//else if(id=='B')ret=&si_B;
	//else return NULL;
	
	//if(ret->used) return NULL;
	
	ret=NULL;
	return ret;
}

int get_time(){
	return time;
}

void serial_interrupt(){
	
	
	
	char isr=ISR;
	
	
	if(isr&0x08){
		volatile char c = STOCC;
		time++;
		
	}
	if(isr&0x02){//rx irq
		//ubuf_put_c('R');
		char c=SRA;
		while(c&0x01){
			c=RHRA;
			rx_buffer[rx_end]=c;
			rx_end++;
			if(rx_end>=RX_BUFFER_SIZE)rx_end-=RX_BUFFER_SIZE;//loop over
			if(rx_end==rx_begin){//if overflow
				//ubuf_put_s("RX_OVERFLOW");
				while(1){
					
				}
			}
			c=SRA;
		}
	}
	if(isr&0x01){//tx irq
		IMR=IRQ_OFF;//disable duart tx irq
		//ubuf_put_c('T');
		while((SRA&0x04) && tx_end != tx_begin){// while tx is rdy & buffer not empty
			char c=tx_buffer[tx_begin];
			tx_begin++;
			if(tx_begin>=TX_BUFFER_SIZE)tx_begin-=TX_BUFFER_SIZE;//loop over
			THRA = c;//send data
			
		}
		if(tx_end != tx_begin)//if buffer not empty
			IMR=IRQ_ON;//re enable duart tx irq
	}
}

void serial_write_c(volatile serial_interface* si,char c){
	if(c=='\n'){
		serial_write_c(si,0x1B);
		//serial_write_c('[');
		serial_write_c(si,'E');
		return;
	}
	
	
	#ifdef NO_IRQ
	ubuf_put_c(c);
	return;
	#endif
	
	
	//should i disable irq?
	//asm("or.w #0x0700, %sr");
	IMR=IRQ_OFF;
	if(tx_end==tx_begin && SRA&0x04 ){// if buf is emty & rdy to tx
		//just send it straight to the duart
		THRA=c;
		
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
		IMR=IRQ_ON;//re enable duart tx irq
		//asm("and.w #0xF8FF, %sr");
		while(tx_next == tx_begin){
			asm("nop");
		}
		
	}
	//asm("and.w #0xF8FF, %sr");
}

void serial_write_s(volatile serial_interface* si,char *s){
	short i=0;
	char c=s[i++];
	while(c){
		serial_write_c(si,c);
		c=s[i++];
	}
	
}

char serial_get_c(volatile serial_interface* si){
	
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

char serial_check_c(volatile serial_interface* si){
	
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

void serial_clear(volatile serial_interface* si){
	serial_write_s(si,"\x1B[2J\x1B[;H");
}
