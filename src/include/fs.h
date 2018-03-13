#ifndef __fs_h
#define __fs_h

#include "mydefs.h"
#include "mydefs.h"

#define FS_TYPE_FILE 	1
#define FS_TYPE_DIR 	2

#define MAX_FILENAME_L 256
#define MAX_PATH_L 256


struct fs_node;

//							fs_node, 		  offset,	size,	  buffer
typedef uint32_t (*read_t) (struct fs_node *, uint32_t, uint32_t, uint8_t*);
typedef uint32_t (*write_t)(struct fs_node *, uint32_t, uint32_t, uint8_t*);
typedef uint32_t (*find_t)(struct fs_node *, char*,struct fs_node*);
typedef uint32_t (*index_dir_t)(struct fs_node *, uint32_t, char*);
typedef void* device_t;
typedef uint32_t inode_n_t;//inode number type
typedef uint32_t f_len_t;//file length type
typedef uint8_t node_type_t;


typedef struct fs_node{
	
	read_t read;
	write_t write;
	find_t find;
	index_dir_t index_dir;
	
	device_t device;
	inode_n_t inode;
	f_len_t length;
	node_type_t type;
	
} fs_node_t;


fs_node_t* fs_root_node();
void fs_find(fs_node_t *node, char* filename, fs_node_t * s_node);
uint32_t fs_read(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
uint32_t fs_write(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
void fs_index_dir(fs_node_t *node, uint32_t index, char* filename);

#endif
