#ifndef __fs_h
#define __fs_h

#include "mydefs.h"
#include "mydefs.h"

struct fs_node;

//							fs_node, 		  offset,	size,	  buffer
typedef uint32_t (*read_t) (struct fs_node *, uint32_t, uint32_t, uint8_t*);
typedef uint32_t (*write_t)(struct fs_node *, uint32_t, uint32_t, uint8_t*);
typedef void* device_t;


typedef struct fs_node{
	
	read_t read;
	write_t write;
	device_t device;
	
} fs_node_t;

	

#endif
