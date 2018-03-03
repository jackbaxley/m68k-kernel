#ifndef __fileserver_h
#define __fileserver_h

#include "mydefs.h"
#include "fs.h"

#define FILESERVER_SECTOR_SIZE 512



typedef void* interface_t;

typedef void (*interface_read_t) (interface_t,uint32_t,uint8_t*);
typedef void (*interface_write_t) (interface_t,uint32_t,uint8_t*);


typedef struct fileserver_device{
	interface_t interface; //pointer to interface device (Fileserver medium, usually serial)
	interface_read_t read;
	interface_write_t write;
	
}fileserver_device_t;




void fileserver_write_sector(fileserver_device_t* dev, uint32_t sec, uint8_t* buffer);
void fileserver_read_sector(fileserver_device_t* dev, uint32_t sec, uint8_t* buffer);
void fileserver_create_node(fileserver_device_t* dev, fs_node_t* node);

#endif
