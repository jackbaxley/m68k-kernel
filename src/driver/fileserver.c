#include "fileserver.h"

#define FILESERVER_CMD_SEEK 1
#define FILESERVER_CMD_READ 2
#define FILESERVER_CMD_WRITE 3
#define FILESERVER_CMD_CLOSE 4

void fileserver_read_sector(fs_node_t* dev, uint32_t sec, uint8_t* buffer){
	
	uint8_t cmd;
	cmd=FILESERVER_CMD_SEEK;
	dev->write(dev, 0, sizeof(cmd), &cmd);
	dev->write(dev, 0, sizeof(sec), (uint8_t*)&sec);
	cmd=FILESERVER_CMD_READ;
	dev->write(dev, 0, sizeof(cmd), &cmd);
	dev->read(dev, 0, FILESERVER_SECTOR_SIZE, buffer);

}
void fileserver_write_sector(fs_node_t* dev, uint32_t sec, uint8_t* buffer){
	
	uint8_t cmd;
	cmd=FILESERVER_CMD_SEEK;
	dev->write(dev, 0, sizeof(cmd), &cmd);
	dev->write(dev, 0, sizeof(sec), (uint8_t*)&sec);
	cmd=FILESERVER_CMD_WRITE;
	dev->write(dev, 0, sizeof(cmd), &cmd);
	dev->write(dev, 0, FILESERVER_SECTOR_SIZE, buffer);

}

uint32_t fileserver_read(fs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buffer){
	if(offset%FILESERVER_SECTOR_SIZE){//if offset is not sector alligned, return 0;
		//error
		return 0;
	}
	
	int s_sector = offset/FILESERVER_SECTOR_SIZE;//start sector
	int n_sectors = size/FILESERVER_SECTOR_SIZE;// number of sectors to read
	
	for(int i=0;i<n_sectors;i++){
		fileserver_read_sector(node->device,s_sector+i,buffer+FILESERVER_SECTOR_SIZE*i);
	}
	
	return FILESERVER_SECTOR_SIZE*n_sectors;
}

uint32_t fileserver_write(fs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buffer){
	if(offset%FILESERVER_SECTOR_SIZE){//if offset is not sector alligned, return 0;
		//error
		return 0;
	}
	
	int s_sector = offset/FILESERVER_SECTOR_SIZE;//start sector
	int n_sectors = size/FILESERVER_SECTOR_SIZE;// number of sectors to write
	
	for(int i=0;i<n_sectors;i++){
		fileserver_write_sector(node->device,s_sector+i,buffer+FILESERVER_SECTOR_SIZE*i);
	}
	
	return FILESERVER_SECTOR_SIZE*n_sectors;
}


void fileserver_create_node(fs_node_t* dev, fs_node_t* node){
	
	node->read=(read_t)fileserver_read;
	node->write=(write_t)fileserver_write;
	node->device=(device_t)dev;
	
	
}


