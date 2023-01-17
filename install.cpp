#include <stdio.h>
#include <string.h>
#include"format.cpp"
#include "userabout.cpp"
#include"shell.cpp"
#include"log.cpp"
#include"halt.cpp"
#include "filesys.h"
#include "access.cpp"
#include"ballfre.cpp"
#include"close.cpp"
#include"creat.cpp"
#include"delete.cpp"
#include"dir.cpp"
#include"iallfre.cpp"
#include"igetput.cpp"
#include "name.cpp"
#include "open.cpp"
#include"rdwt.cpp"

void install(){
	int i,j;

	/* 1. read the filsys from the superblock*/   //xiao
	memcpy(&filsys,disk+BLOCKSIZ,sizeof(struct filsys));//void *memcpy(void *dest, const void *src, size_t n);
	//从源src所指的内存地址的起始位置开始拷贝n个字节到目标dest所指的内存地址的起始位置中

	/* 2. initialize the inode hash chain*/
	for (i=0; i<NHINO; i++){
		hinode[i].i_forw = NULL;
	}

	/*3. initialize the sys_ofile*/
	for (i=0; i<SYSOPENFILE; i++){
		sys_ofile[i].f_count = 0;
		sys_ofile[i].f_inode = NULL;
	}

	/*4. initialize the user*/
	for (i=0; i<USERNUM; i++){
		user[i].u_uid = 0;
		user[i].u_gid = 0;
		for (j=0; j<NOFILE; j++)
			user[i].u_ofile[j] = SYSOPENFILE+1;
	}

	/*5. read the main directory to initialize the dir*/
	cur_path_inode = iget(1);
	dir.size = cur_path_inode->di_size/(DIRSIZ+4);//xiao 2-->4

	for (i=0; i<DIRNUM; i++){
		strcpy(dir.direct[i].d_name,"             ");
		dir.direct[i].d_ino = 0;
	}

	for (i=0; i<dir.size/(BLOCKSIZ/(DIRSIZ+4)); i++){
		memcpy(&dir.direct[(BLOCKSIZ/(DIRSIZ+4))*i],
		disk+DATASTART+BLOCKSIZ*cur_path_inode->di_addr[i], DINODESIZ);
	}

	memcpy(&dir.direct[(BLOCKSIZ)/(DIRSIZ+4)*i],
	disk+DATASTART+BLOCKSIZ*cur_path_inode->di_addr[i], DINODESIZ);
	return;
}
