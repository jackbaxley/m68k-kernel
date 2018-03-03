#include "ext2.h"
#include "stdlib.h"

#define EXT2_OFFSET_INODES_COUNT 		0
#define EXT2_OFFSET_BLOCKS_COUNT		4
#define EXT2_OFFSET_R_BLOCKS_COUNT		8
#define EXT2_OFFSET_FREE_BLOCKS_COUNT	12	
#define EXT2_OFFSET_FREE_INODES_COUNT	16
#define EXT2_OFFSET_FIRST_DATA_BLOCK	20
#define EXT2_OFFSET_LOG_BLOCK_SIZE		24
#define EXT2_OFFSET_LOG_FRAG_SIZE		28
#define EXT2_OFFSET_BLOCKS_PER_GROUP	32
#define EXT2_OFFSET_FRAGS_PER_GROUP		36
#define EXT2_OFFSET_INDOES_PER_GROUP	40
#define EXT2_OFFSET_MTIME				44
#define EXT2_OFFSET_WTIME				48
#define EXT2_OFFSET_MNT_COUNT			52
#define EXT2_OFFSET_MAX_MNT_COUNT		54
#define EXT2_OFFSET_MAGIC				56
#define EXT2_OFFSET_STATE				58
#define EXT2_OFFSET_ERRORS				60
#define EXT2_OFFSET_MINOR_REV_LEVEL		62
#define EXT2_OFFSET_LASTCHECK			64
#define EXT2_OFFSET_CHECKINTERVAL		68
#define EXT2_OFFSET_CREATOR_OS			72
#define EXT2_OFFSET_REV_LEVEL			76
#define EXT2_OFFSET_DEF_RESUID			80
#define EXT2_OFFSET_DEF_RESGID			82

#define SUPERBLOCK_BUFFER_S	512	
#define SUPERBLOCK_OFFSET	1024	



void ext2_read_superblock(ext2_system_t* filesystem){
	uint8_t buffer[SUPERBLOCK_BUFFER_S];
	filesystem->device->read(filesystem->device, SUPERBLOCK_OFFSET, SUPERBLOCK_BUFFER_S, buffer);
	
	filesystem->superblock.s_inodes_count		= readlendian32(buffer+EXT2_OFFSET_INODES_COUNT);
	filesystem->superblock.s_blocks_count		= readlendian32(buffer+EXT2_OFFSET_BLOCKS_COUNT);
	filesystem->superblock.s_r_blocks_count		= readlendian32(buffer+EXT2_OFFSET_R_BLOCKS_COUNT);
	filesystem->superblock.s_free_blocks_count	= readlendian32(buffer+EXT2_OFFSET_FREE_BLOCKS_COUNT);
	filesystem->superblock.s_free_inodes_count	= readlendian32(buffer+EXT2_OFFSET_FREE_INODES_COUNT);
	filesystem->superblock.s_first_data_block	= readlendian32(buffer+EXT2_OFFSET_FIRST_DATA_BLOCK);
	filesystem->superblock.s_log_block_size		= readlendian32(buffer+EXT2_OFFSET_LOG_BLOCK_SIZE);
	filesystem->superblock.s_log_frag_size		= readlendian32(buffer+EXT2_OFFSET_LOG_FRAG_SIZE);
	filesystem->superblock.s_blocks_per_group	= readlendian32(buffer+EXT2_OFFSET_BLOCKS_PER_GROUP);
	filesystem->superblock.s_frags_per_group	= readlendian32(buffer+EXT2_OFFSET_FRAGS_PER_GROUP);
	filesystem->superblock.s_inodes_per_group	= readlendian32(buffer+EXT2_OFFSET_INDOES_PER_GROUP);
	filesystem->superblock.s_mtime				= readlendian32(buffer+EXT2_OFFSET_MTIME);
	filesystem->superblock.s_wtime				= readlendian32(buffer+EXT2_OFFSET_WTIME);
	
	filesystem->superblock.s_mnt_count			= readlendian16(buffer+EXT2_OFFSET_MNT_COUNT);
	filesystem->superblock.s_max_mnt_count		= readlendian16(buffer+EXT2_OFFSET_MAX_MNT_COUNT);
	filesystem->superblock.s_magic				= readlendian16(buffer+EXT2_OFFSET_MAGIC);
	filesystem->superblock.s_state				= readlendian16(buffer+EXT2_OFFSET_STATE);
	filesystem->superblock.s_errors				= readlendian16(buffer+EXT2_OFFSET_ERRORS);
	filesystem->superblock.s_minor_rev_level	= readlendian16(buffer+EXT2_OFFSET_MINOR_REV_LEVEL);
	
	
	filesystem->superblock.s_lastcheck			= readlendian32(buffer+EXT2_OFFSET_LASTCHECK);
	filesystem->superblock.s_checkinterval		= readlendian32(buffer+EXT2_OFFSET_CHECKINTERVAL);
	filesystem->superblock.s_creator_os			= readlendian32(buffer+EXT2_OFFSET_CREATOR_OS);
	filesystem->superblock.s_rev_level			= readlendian32(buffer+EXT2_OFFSET_REV_LEVEL);
	
	filesystem->superblock.s_def_resuid			= readlendian16(buffer+EXT2_OFFSET_DEF_RESUID);
	filesystem->superblock.s_def_resgid			= readlendian16(buffer+EXT2_OFFSET_DEF_RESGID);
		
	
}

void ext2_make_system(ext2_system_t* filesystem, fs_node_t* device){
	
	filesystem->device=device;
	
}

