#include "fs.h"





fs_node_t root_node;



fs_node_t* fs_root_node(){
	return &root_node;
}




//finds a node in the fs by name
void fs_find(fs_node_t *node, char* filename, fs_node_t * s_node){
	if(!node)
		return;
	if(!node->find)
		return;
	
	node->find(node,filename,s_node);
	
}

//read from a node
uint32_t fs_read(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer){
	if(!node)
		return 0;
	if(!node->read)
		return 0;
	
	return node->read(node,offset,size,buffer);
	
}

//write to a node
uint32_t fs_write(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer){
	if(!node)
		return 0;
	if(!node->write)
		return 0;
	
	return node->write(node,offset,size,buffer);
	
}

//gets the nth entry in a directory
void fs_index_dir(fs_node_t *node, uint32_t index, char* filename){
	if(!node)
		return;
	
	if(node->type!=FS_TYPE_DIR)
		return;
	if(!node->index_dir)
		return;
	
	node->index_dir(node,index,filename);
}


