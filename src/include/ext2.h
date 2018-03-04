#ifndef __ext2_h
#define __ext2_h

#include "mydefs.h"
#include "fs.h"


typedef uint32_t ext2_bg_n_t;

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


typedef struct ext2_bgd{
	uint32_t bg_block_bitmap;
	uint32_t bg_inode_bitmap;
	uint32_t bg_inode_table;
	uint16_t bg_free_blocks_count;
	uint16_t bg_free_inodes_count;
	uint16_t bg_used_dirs_count;
}ext2_bgd_t;

typedef struct ext2_inode{
	uint16_t i_mode;
	uint16_t i_uid;
	uint32_t i_size;
	uint32_t i_atime;
	uint32_t i_ctime;
	uint32_t i_mtime;
	uint32_t i_dtime;
	uint16_t i_gid;
	uint16_t i_links_count;
	uint32_t i_blocks;
	uint32_t i_flags;
	uint32_t i_osd1;
	uint32_t i_block[15];
	uint32_t i_generation;
	uint32_t i_file_acl;
	uint32_t i_dir_acl;
	uint32_t i_faddr;
	
} ext2_inode_t;


typedef struct ext2_system{
	ext2_superblock_t superblock;
	fs_node_t* device;	
} ext2_system_t;

void ext2_read_superblock(ext2_system_t* filesystem);
void ext2_make_system(ext2_system_t* filesystem, fs_node_t* device);
void ext2_mount_root(ext2_system_t* filesystem, fs_node_t* mnt);

	

#endif
