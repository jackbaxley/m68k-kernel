#include "ext2.h"
#include "stdlib.h"

#define EXT2_OFFSET_S_INODES_COUNT 			0
#define EXT2_OFFSET_S_BLOCKS_COUNT			4
#define EXT2_OFFSET_S_R_BLOCKS_COUNT		8
#define EXT2_OFFSET_S_FREE_BLOCKS_COUNT		12	
#define EXT2_OFFSET_S_FREE_INODES_COUNT		16
#define EXT2_OFFSET_S_FIRST_DATA_BLOCK		20
#define EXT2_OFFSET_S_LOG_BLOCK_SIZE		24
#define EXT2_OFFSET_S_LOG_FRAG_SIZE			28
#define EXT2_OFFSET_S_BLOCKS_PER_GROUP		32
#define EXT2_OFFSET_S_FRAGS_PER_GROUP		36
#define EXT2_OFFSET_S_INDOES_PER_GROUP		40
#define EXT2_OFFSET_S_MTIME					44
#define EXT2_OFFSET_S_WTIME					48
#define EXT2_OFFSET_S_MNT_COUNT				52
#define EXT2_OFFSET_S_MAX_MNT_COUNT			54
#define EXT2_OFFSET_S_MAGIC					56
#define EXT2_OFFSET_S_STATE					58
#define EXT2_OFFSET_S_ERRORS				60
#define EXT2_OFFSET_S_MINOR_REV_LEVEL		62
#define EXT2_OFFSET_S_LASTCHECK				64
#define EXT2_OFFSET_S_CHECKINTERVAL			68
#define EXT2_OFFSET_S_CREATOR_OS			72
#define EXT2_OFFSET_S_REV_LEVEL				76
#define EXT2_OFFSET_S_DEF_RESUID			80
#define EXT2_OFFSET_S_DEF_RESGID			82


#define EXT2_OFFSET_BG_BLOCK_BITMAP			0
#define EXT2_OFFSET_BG_INODE_BITMAP			4
#define EXT2_OFFSET_BG_INODE_TABLE			8
#define EXT2_OFFSET_BG_FREE_BLOCKS_COUNT	12
#define EXT2_OFFSET_BG_FREE_INODES_COUNT	14
#define EXT2_OFFSET_BG_USED_DIRS_COUNT		16


#define EXT2_OFFSET_I_MODE			0
#define EXT2_OFFSET_I_UID			2
#define EXT2_OFFSET_I_SIZE			4
#define EXT2_OFFSET_I_ATIME			8
#define EXT2_OFFSET_I_CTIME			12
#define EXT2_OFFSET_I_MTIME			16
#define EXT2_OFFSET_I_DTIME			20
#define EXT2_OFFSET_I_GID			24
#define EXT2_OFFSET_I_LINKS_COUNT	26
#define EXT2_OFFSET_I_BLOCKS		28
#define EXT2_OFFSET_I_FLAGS			32
#define EXT2_OFFSET_I_OSD1			36
#define EXT2_OFFSET_I_BLOCK			40
#define EXT2_OFFSET_I_GENERATION	100
#define EXT2_OFFSET_I_FILE_ACL		104
#define EXT2_OFFSET_I_DIR_ACL		108
#define EXT2_OFFSET_I_FADDR			112

#define SUPERBLOCK_BUFFER_S	512	
#define INODE_BUFFER_S		512	
#define BGD_BUFFER_S		512	
#define SUPERBLOCK_OFFSET	1024	

#define BGD_SIZE 32
#define INODE_SIZE 128

#define EXT2_ROOT_INO 2

void ext2_read_superblock(ext2_system_t* filesystem){
	uint8_t buffer[SUPERBLOCK_BUFFER_S];
	filesystem->device->read(filesystem->device, SUPERBLOCK_OFFSET, SUPERBLOCK_BUFFER_S, buffer);
	
	filesystem->superblock.s_inodes_count		= readlendian32(buffer+EXT2_OFFSET_S_INODES_COUNT);
	filesystem->superblock.s_blocks_count		= readlendian32(buffer+EXT2_OFFSET_S_BLOCKS_COUNT);
	filesystem->superblock.s_r_blocks_count		= readlendian32(buffer+EXT2_OFFSET_S_R_BLOCKS_COUNT);
	filesystem->superblock.s_free_blocks_count	= readlendian32(buffer+EXT2_OFFSET_S_FREE_BLOCKS_COUNT);
	filesystem->superblock.s_free_inodes_count	= readlendian32(buffer+EXT2_OFFSET_S_FREE_INODES_COUNT);
	filesystem->superblock.s_first_data_block	= readlendian32(buffer+EXT2_OFFSET_S_FIRST_DATA_BLOCK);
	filesystem->superblock.s_log_block_size		= readlendian32(buffer+EXT2_OFFSET_S_LOG_BLOCK_SIZE);
	filesystem->superblock.s_log_frag_size		= readlendian32(buffer+EXT2_OFFSET_S_LOG_FRAG_SIZE);
	filesystem->superblock.s_blocks_per_group	= readlendian32(buffer+EXT2_OFFSET_S_BLOCKS_PER_GROUP);
	filesystem->superblock.s_frags_per_group	= readlendian32(buffer+EXT2_OFFSET_S_FRAGS_PER_GROUP);
	filesystem->superblock.s_inodes_per_group	= readlendian32(buffer+EXT2_OFFSET_S_INDOES_PER_GROUP);
	filesystem->superblock.s_mtime				= readlendian32(buffer+EXT2_OFFSET_S_MTIME);
	filesystem->superblock.s_wtime				= readlendian32(buffer+EXT2_OFFSET_S_WTIME);
	
	filesystem->superblock.s_mnt_count			= readlendian16(buffer+EXT2_OFFSET_S_MNT_COUNT);
	filesystem->superblock.s_max_mnt_count		= readlendian16(buffer+EXT2_OFFSET_S_MAX_MNT_COUNT);
	filesystem->superblock.s_magic				= readlendian16(buffer+EXT2_OFFSET_S_MAGIC);
	filesystem->superblock.s_state				= readlendian16(buffer+EXT2_OFFSET_S_STATE);
	filesystem->superblock.s_errors				= readlendian16(buffer+EXT2_OFFSET_S_ERRORS);
	filesystem->superblock.s_minor_rev_level	= readlendian16(buffer+EXT2_OFFSET_S_MINOR_REV_LEVEL);
	
	
	filesystem->superblock.s_lastcheck			= readlendian32(buffer+EXT2_OFFSET_S_LASTCHECK);
	filesystem->superblock.s_checkinterval		= readlendian32(buffer+EXT2_OFFSET_S_CHECKINTERVAL);
	filesystem->superblock.s_creator_os			= readlendian32(buffer+EXT2_OFFSET_S_CREATOR_OS);
	filesystem->superblock.s_rev_level			= readlendian32(buffer+EXT2_OFFSET_S_REV_LEVEL);
	
	filesystem->superblock.s_def_resuid			= readlendian16(buffer+EXT2_OFFSET_S_DEF_RESUID);
	filesystem->superblock.s_def_resgid			= readlendian16(buffer+EXT2_OFFSET_S_DEF_RESGID);
		
	
}

uint32_t ext2_block_size(ext2_system_t* filesystem){
	ext2_superblock_t *s=&filesystem->superblock;
	uint32_t block_size=1024<<s->s_log_block_size;
	return block_size;
}

//reads a single block group descriptor from bgdt
void ext2_read_bgd(ext2_system_t* filesystem, ext2_bg_n_t bg_n, ext2_bgd_t *bgd){
	
	uint8_t buffer[BGD_BUFFER_S];
	
	//figuring out where the blockgroupdescriptor table is
	//first get block size
	uint32_t block_size=ext2_block_size(filesystem);
	//bgdt starts block after superblock
	uint32_t bgdt_block=(SUPERBLOCK_OFFSET/block_size)+1;
	
	uint32_t bgd_sector = bgdt_block*block_size/BGD_BUFFER_S + bg_n*BGD_SIZE/BGD_BUFFER_S;
	
	uint32_t bgd_addr=(bg_n*BGD_SIZE)%BGD_BUFFER_S;
	
	filesystem->device->read(filesystem->device,bgd_sector*BGD_BUFFER_S,BGD_BUFFER_S,buffer);
	
	bgd->bg_block_bitmap 		= readlendian32(buffer+bgd_addr+EXT2_OFFSET_BG_BLOCK_BITMAP);
	bgd->bg_inode_bitmap 		= readlendian32(buffer+bgd_addr+EXT2_OFFSET_BG_INODE_BITMAP);
	bgd->bg_inode_table 		= readlendian32(buffer+bgd_addr+EXT2_OFFSET_BG_INODE_TABLE);
	bgd->bg_free_blocks_count 	= readlendian16(buffer+bgd_addr+EXT2_OFFSET_BG_FREE_BLOCKS_COUNT);
	bgd->bg_free_blocks_count 	= readlendian16(buffer+bgd_addr+EXT2_OFFSET_BG_FREE_INODES_COUNT);
	bgd->bg_used_dirs_count 	= readlendian16(buffer+bgd_addr+EXT2_OFFSET_BG_USED_DIRS_COUNT);
			
}

void ext2_read_inode(ext2_system_t* filesystem, inode_n_t inode_n, ext2_inode_t* inode){
	
	
	uint8_t buffer[INODE_BUFFER_S];
	ext2_bgd_t bgd;
	
	ext2_superblock_t *s=&filesystem->superblock;
	uint32_t block_size=ext2_block_size(filesystem);
	
	ext2_bg_n_t block_group = (inode_n-1)/s->s_inodes_per_group;
	inode_n_t local_inode_index=(inode_n-1)/s->s_inodes_per_group;//inode index within bg
	
	ext2_read_bgd(filesystem,block_group,&bgd);
	
	uint32_t inode_sector = (local_inode_index*INODE_SIZE)/INODE_BUFFER_S + bgd.bg_inode_table*(block_size/INODE_BUFFER_S);
	//addr within buffer
	uint32_t inode_addr=(local_inode_index*INODE_SIZE)%INODE_BUFFER_S;
	
	filesystem->device->read(filesystem->device,inode_sector*INODE_BUFFER_S,INODE_BUFFER_S,buffer);
	
	inode->i_mode		= readlendian16(buffer+inode_addr+EXT2_OFFSET_I_MODE);
	inode->i_uid		= readlendian16(buffer+inode_addr+EXT2_OFFSET_I_UID);
	inode->i_size		= readlendian32(buffer+inode_addr+EXT2_OFFSET_I_SIZE);
	inode->i_atime		= readlendian32(buffer+inode_addr+EXT2_OFFSET_I_ATIME);
	inode->i_ctime		= readlendian32(buffer+inode_addr+EXT2_OFFSET_I_CTIME);
	inode->i_mtime		= readlendian32(buffer+inode_addr+EXT2_OFFSET_I_MTIME);
	inode->i_dtime		= readlendian32(buffer+inode_addr+EXT2_OFFSET_I_DTIME);
	inode->i_gid		= readlendian16(buffer+inode_addr+EXT2_OFFSET_I_GID);
	inode->i_links_count= readlendian16(buffer+inode_addr+EXT2_OFFSET_I_LINKS_COUNT);
	inode->i_blocks		= readlendian32(buffer+inode_addr+EXT2_OFFSET_I_BLOCKS);
	inode->i_flags		= readlendian32(buffer+inode_addr+EXT2_OFFSET_I_FLAGS);
	inode->i_osd1		= readlendian32(buffer+inode_addr+EXT2_OFFSET_I_OSD1);
	
	for(int i=0;i<15;i++)
		inode->i_block[i]= readlendian32(buffer+inode_addr+EXT2_OFFSET_I_BLOCK+i*4);
	
	inode->i_generation	= readlendian32(buffer+inode_addr+EXT2_OFFSET_I_GENERATION);
	inode->i_file_acl	= readlendian32(buffer+inode_addr+EXT2_OFFSET_I_FILE_ACL);
	inode->i_dir_acl	= readlendian32(buffer+inode_addr+EXT2_OFFSET_I_DIR_ACL);
	inode->i_faddr		= readlendian32(buffer+inode_addr+EXT2_OFFSET_I_FADDR);	
	
}

uint32_t ext2_read(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer){
	return 0;
}

uint32_t ext2_write(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer){
	return 0;
}

//opt_inode is optional, will be read from fs otherwise
void ext2_read_inode_block(ext2_system_t *filesystem, inode_n_t inode_n,ext2_inode_t* opt_inode){
	ext2_inode_t* inode;
	ext2_inode_t bu_inode;//backup inode structure
	
	//entries per block
	uint32_t epb=ext2_block_size(filesystem)/4;
	
	if(opt_inode){
		inode=opt_inode;
	}else{
		ext2_read_inode(filesystem,inode_n,&bu_inode);
		inode=&bu_inode;
	}
	
	uint32_t block_id;
	
	if(inode_n < 12){// direct link
		block_id=inode->i_block[inode_n];
	}else if(inode_n-12 < epb){// single link
		
	}else if(inode_n-12-epb < epb*epb){// double link
		
	}else if(inode_n-12-epb-epb*epb < epb*epb*epb){// triple link
		
	}else{
		//error
		return;
	}
	
}

void ext2_index_dir(fs_node_t *node, uint32_t index){
	ext2_inode_t inode;
	ext2_read_inode(node->device, node->inode, &inode);
	
	
}

void ext2_make_system(ext2_system_t* filesystem, fs_node_t* device){
	
	filesystem->device=device;
	
}

void ext2_mount_root(ext2_system_t* filesystem, fs_node_t* mnt){
	mnt->read=	(read_t)ext2_read;
	mnt->write=	(write_t)ext2_write;
	mnt->device=(device_t)filesystem;
	mnt->inode=	EXT2_ROOT_INO;
	
	ext2_inode_t inode;
	
	ext2_read_inode(filesystem, mnt->inode, &inode);
	
	mnt->length=inode.i_size;
	
	
}

