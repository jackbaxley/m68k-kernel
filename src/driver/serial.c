#include "serial.h"

#ifdef NO_IRQ
	#define IRQ_ON 0x00
#else
	#define IRQ_ON 0x33
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

//68681 IRQ Flags used both for irq status and irq mask
#define IRQ_RXA 0x02
#define IRQ_TXA 0x01
#define IRQ_RXB 0x20
#define IRQ_TXB 0x10

serial_interface si_A;
serial_interface si_B;

volatile int time;
volatile char IMR_state;

void set_IMR_flag(char flag){
	IMR_state |= flag;
	IMR=IMR_state;
}
void reset_IMR_flag(char flag){
	IMR_state &= ~flag;
	IMR=IMR_state;
} 

void clear_IMR(){
	IMR=0x00;
}

void restore_IMR(){
	IMR=IMR_state;
}

void flush_rx(serial_interface* si){
	si->rx_begin=si->rx_end;
}


void serial_init(){
	IMR_state=0;

	si_A.tx_begin=0;
	si_A.tx_end=0;
	si_A.rx_begin=0;
	si_A.rx_end=0;
	si_A.used=0;
	si_A.id='A';
	
	si_B.tx_begin=0;
	si_B.tx_end=0;
	si_B.rx_begin=0;
	si_B.rx_end=0;
	si_B.used=0;
	si_B.id='B';
	
	
	time=0;
	
	//Turn in IRQ
	set_IMR_flag(IRQ_ON);
}

serial_interface* serial_get_interface(char id){
	serial_interface* ret;
	if(id=='A')ret=&si_A;
	else if(id=='B')ret=&si_B;
	else return NULL;
	
	if(ret->used) return NULL;
	ret->used=1;
	return ret;
}

int get_time(){
	return time;
}

void serial_interrupt(){

	char isr=ISR;
	
	if(isr&0x08){

		(void)STOCC;//read from STOCC to (reset timer?)
		time++;
		
	}
	if(isr&IRQ_RXA){//rx irq A
		char c=SRA;
		while(c&0x01){
			c=RHRA;
			si_A.rx_buffer[si_A.rx_end]=c;
			si_A.rx_end++;
			if(si_A.rx_end>=RX_BUFFER_SIZE)si_A.rx_end-=RX_BUFFER_SIZE;//loop over
			if(si_A.rx_end==si_A.rx_begin){//if overflow
				while(1){
					
				}
			}
			c=SRA;
		}
	}
	if(isr&IRQ_RXB){//rx irq B
		char c=SRB;
		while(c&0x01){
			c=RHRB;
			si_B.rx_buffer[si_B.rx_end]=c;
			si_B.rx_end++;
			if(si_B.rx_end>=RX_BUFFER_SIZE)si_B.rx_end-=RX_BUFFER_SIZE;//loop over
			if(si_B.rx_end==si_B.rx_begin){//if overflow
				while(1){
					
				}
			}
			c=SRB;
		}
	}
	if(isr&IRQ_TXA){//tx irq A
		reset_IMR_flag(IRQ_TXA);//disable duart tx irq
		while((SRA&0x04) && si_A.tx_end != si_A.tx_begin){// while tx is rdy & buffer not empty
			char c=si_A.tx_buffer[si_A.tx_begin];
			si_A.tx_begin++;
			if(si_A.tx_begin>=TX_BUFFER_SIZE)si_A.tx_begin-=TX_BUFFER_SIZE;//loop over
			THRA = c;//send data
			
		}
		if(si_A.tx_end != si_A.tx_begin)//if buffer not empty
			set_IMR_flag(IRQ_TXA);//re enable duart tx irq
	}
	
	if(isr&IRQ_TXB){//tx irq B
		reset_IMR_flag(IRQ_TXB);//disable duart tx irq
		while((SRB&0x04) && si_B.tx_end != si_B.tx_begin){// while tx is rdy & buffer not empty
			char c=si_B.tx_buffer[si_B.tx_begin];
			si_B.tx_begin++;
			if(si_B.tx_begin>=TX_BUFFER_SIZE)si_B.tx_begin-=TX_BUFFER_SIZE;//loop over
			THRB = c;//send data
			
		}
		if(si_B.tx_end != si_B.tx_begin)//if buffer not empty
			set_IMR_flag(IRQ_TXB);//re enable duart tx irq
	}
	
}

void serial_write_c(serial_interface* si,char c){
	if(c=='\n' && si->id=='A'){
		serial_write_c(si,0x1B);
		serial_write_c(si,'E');
		return;
	}
		
	#ifdef NO_IRQ
	ubuf_put_c(c);
	return;
	#endif
	
	char* thr=NULL;//pointer to tx holding register
	char sr=0;//contents of channels status register
	char irq_flag=0;//flag of transmit irq
	
	if(si->id=='A'){
		thr=&THRA;
		sr=SRA;
		irq_flag=IRQ_TXA;
	}else if(si->id=='B'){
		thr=&THRB;
		sr=SRB;
		irq_flag=IRQ_TXB;
	}else{
		return;
	}
		
	reset_IMR_flag(irq_flag);
	if(si->tx_end==si->tx_begin && sr&0x04 ){// if buf is emty & rdy to tx
		//just send it straight to the duart
		*thr=c;		
		
	}else{//otherwise put it in buffer
		
		
		si->tx_buffer[si->tx_end]=c;
		si->tx_end++;
		if(si->tx_end>=TX_BUFFER_SIZE)si->tx_end-=TX_BUFFER_SIZE;		
		short tx_next=si->tx_end+1;
		if(tx_next>=TX_BUFFER_SIZE)tx_next-=TX_BUFFER_SIZE;
		//if(tx_end==tx_begin){
		//	ubuf_put_s("TX_OVERFLOW");
		//	while(1);
		//}
		set_IMR_flag(irq_flag);//re enable duart tx irq
		while(tx_next == si->tx_begin){
			asm("nop");
		}
		
	}
	
}

void serial_write_s(serial_interface* si,char *s){
	short i=0;
	char c=s[i++];
	while(c){
		serial_write_c(si,c);
		c=s[i++];
	}
	
}

char serial_get_c(serial_interface* si){
	
	#ifdef NO_IRQ
	return ubuf_get_c();
	#endif
	
	char c;
	while(si->rx_end==si->rx_begin){
		asm(
			"stop #0x3000" // waits for and interrupt
		);
	}
	c=si->rx_buffer[si->rx_begin];
	si->rx_begin++;
	if(si->rx_begin>=RX_BUFFER_SIZE)si->rx_begin-=RX_BUFFER_SIZE;

	return c;
}

char serial_check_c(serial_interface* si){
	
	#ifdef NO_IRQ
	return ubuf_check_c();
	#endif
	
	char c;
	if(si->rx_end==si->rx_begin){
		return 0;
	}
	c=si->rx_buffer[si->rx_begin];
	si->rx_begin++;
	if(si->rx_begin>=RX_BUFFER_SIZE)si->rx_begin-=RX_BUFFER_SIZE;
	return c;
}

void serial_clear(serial_interface* si){
	serial_write_s(si,"\x1B[2J\x1B[;H");
}


uint32_t serial_write(fs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buffer){
	
	for(int i=0;i<size;i++){
		serial_write_c((serial_interface*)node->device,buffer[i]);
	}
	return size;
}
uint32_t serial_read(fs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buffer){
	for(int i=0;i<size;i++){
		buffer[i]=serial_get_c((serial_interface*)node->device);
	}
	return size;
}
void serial_create_node(serial_interface* si, fs_node_t* node){
	node->device=(device_t)si;
	node->read=(read_t)serial_read;
	node->write=(write_t)serial_write;
}