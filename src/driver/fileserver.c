#include "fileserver.h"

#define FILESERVER_CMD_SEEK 1
#define FILESERVER_CMD_READ 2
#define FILESERVER_CMD_WRITE 3
#define FILESERVER_CMD_CLOSE 4

void fileserver_read_sector(fileserver_device_t* dev, uint32_t sec, uint8_t* buffer){
	
	uint8_t cmd;
	cmd=FILESERVER_CMD_SEEK;
	dev->write(dev->interface, sizeof(cmd), &cmd);
	dev->write(dev->interface, sizeof(sec), &sec);
	cmd=FILESERVER_CMD_READ;
	dev->write(dev->interface, sizeof(cmd), &cmd);
	dev->read(dev->interface, FILESERVER_SECTOR_SIZE, buffer);

}
void fileserver_write_sector(fileserver_device_t* dev, uint32_t sec, uint8_t* buffer){
	
	uint8_t cmd;
	cmd=FILESERVER_CMD_SEEK;
	dev->write(dev->interface, sizeof(cmd), &cmd);
	dev->write(dev->interface, sizeof(sec), &sec);
	cmd=FILESERVER_CMD_WRITE;
	dev->write(dev->interface, sizeof(cmd), &cmd);
	dev->write(dev->interface, FILESERVER_SECTOR_SIZE, buffer);

}


