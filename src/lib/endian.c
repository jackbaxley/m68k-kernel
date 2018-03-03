#include "stdlib.h"


//converts 32bit little endian to systems 32bit int
uint32_t readlendian32(uint8_t* in){
	uint32_t out=0;
	out|=in[0]<<0;
	out|=in[1]<<8;
	out|=in[2]<<16;
	out|=in[3]<<24;
	return out;
}

//converts 16bit little endian to systems 16bit int
uint16_t readlendian16(uint8_t* in){
	uint16_t out=0;
	out|=in[0]<<0;
	out|=in[1]<<8;
	return out;
}

void writelendian32(uint32_t in, uint8_t* out){
	out[0]=(in>> 0)&0xF;
	out[1]=(in>> 8)&0xF;
	out[2]=(in>>16)&0xF;
	out[3]=(in>>24)&0xF;
}

void writelendian16(uint16_t in, uint8_t* out){
	out[0]=(in>> 0)&0xF;
	out[1]=(in>> 8)&0xF;
}
