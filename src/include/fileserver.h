#ifndef __fileserver_h
#define __fileserver_h

#include "mydefs.h"
#include "fs.h"

#define FILESERVER_SECTOR_SIZE 512



typedef void* interface_t;

typedef void (*interface_read_t) (interface_t,uint32_t,uint8_t*);
typedef void (*interface_write_t) (interface_t,uint32_t,uint8_t*);



void fileserver_write_sector(fs_node_t* dev, uint32_t sec, uint8_t* buffer);
void fileserver_read_sector(fs_node_t* dev, uint32_t sec, uint8_t* buffer);
void fileserver_create_node(fs_node_t* dev, fs_node_t* node);

#endif
