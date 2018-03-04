#ifndef __fs_h
#define __fs_h

#include "mydefs.h"
#include "mydefs.h"

struct fs_node;

//							fs_node, 		  offset,	size,	  buffer
typedef uint32_t (*read_t) (struct fs_node *, uint32_t, uint32_t, uint8_t*);
typedef uint32_t (*write_t)(struct fs_node *, uint32_t, uint32_t, uint8_t*);
typedef void* device_t;
typedef uint32_t inode_n_t;//inode number type
typedef uint32_t f_len_t;//file length type


typedef struct fs_node{
	
	read_t read;
	write_t write;
	device_t device;
	inode_n_t inode;
	f_len_t length;
	
} fs_node_t;


fs_node_t* fs_root_node();

#endif
