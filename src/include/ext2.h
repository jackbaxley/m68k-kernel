#ifndef __ext2_h
#define __ext2_h

#include "mydefs.h"
#include "fs.h"

typedef struct ext2_superblock{
	uint32_t s_inodes_count;
	uint32_t s_blocks_count;
	uint32_t s_r_blocks_count;
	uint32_t s_free_blocks_count;
	uint32_t s_free_inodes_count;
	uint32_t s_first_data_block;
	uint32_t s_log_block_size;
	uint32_t s_log_frag_size;
	uint32_t s_blocks_per_group;
	uint32_t s_frags_per_group;
	uint32_t s_inodes_per_group;
	uint32_t s_mtime;
	uint32_t s_wtime;
	
	uint16_t s_mnt_count;
	uint16_t s_max_mnt_count;
	uint16_t s_magic;
	uint16_t s_state;
	uint16_t s_errors;
	uint16_t s_minor_rev_level;
	
	uint32_t s_lastcheck;
	uint32_t s_checkinterval;
	uint32_t s_creator_os;
	uint32_t s_rev_level;
	
	uint16_t s_def_resuid;
	uint16_t s_def_resgid;
	
} ext2_superblock_t;

typedef struct ext2_system{
	ext2_superblock_t superblock;
	fs_node_t* device;
	
	
} ext2_system_t;

void ext2_read_superblock(ext2_system_t* filesystem);
void ext2_make_system(ext2_system_t* filesystem, fs_node_t* device);


	

#endif
