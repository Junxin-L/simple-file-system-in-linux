#include <stdio.h>
#include "filesys.h"

/*
void delete(char *filename){
	unsigned int dirid;
	struct inode *inode;

	dirid = namei(filename);
	if (dirid == -1){
		printf("文件不存在，请检查!\n");
		return;
	}
	inode = iget(dir.direct[dirid].d_ino);
	if(!(inode->di_mode &DIFILE)){
		printf("对象不是文件，请检查！\n");
		iput(inode);
		return;
	}
	dir.direct[dirid].d_ino = DIEMPTY;
	dir.size --;
	inode->di_number--;
	iput(inode);
	return;
}*/

//删除目录
void delete_dir(char *filename)
{
    int dirid,dirpos;
    int i;
    struct inode *inode;
    struct inode *temp_inode;
	struct direct buf[BLOCKSIZ/(DIRSIZ+4)];
	char *str="..";
    dirid=namei(filename);//获取目录项的标号i
         //printf("x");
    inode = iget(dir.direct[dirid].d_ino);
    chdir(filename);
    if(dir.size==2){//判断是否为空目录
        chdir(str);
        //删除目录,获取当前目录项的编号，修改目录名和目录号，使目录项在检查时失效
        //printf("%s",filename);
        dirpos = namei(filename);
        //printf("%d",dirpos);
        dir.direct[dirpos].d_ino =0;
        dir.size--;
        ifree(inode->i_ino);//目录数--
        iput(inode);
    }else{//判断文件还是目录，目录是否有子目录 递归
            for(i=2;i<DIRNUM;i++)
            {
                //printf("%d",DIRNUM);
                //_dir();
                //printf("l");
                //printf("%d",i);
                if (dir.direct[i].d_ino != DIEMPTY){
                    //printf("%d\n",dir.direct[i].d_ino);
                    temp_inode=iget(dir.direct[i].d_ino);//获取当前节点信息
                   // printf("%d",temp_inode->i_ino);
                   // _dir();
                    if(temp_inode->di_mode & DIFILE)//删除文件
                    {
                       // printf("x");
                        dir.direct[i].d_ino = DIEMPTY;//修改目录权限为空
                        dir.size --;//长度-1
                        temp_inode->di_number--;//节点关联文件数-1
                        iput(temp_inode);//回收节点
                    }else{//子目录是否为空
                            chdir(dir.direct[i].d_name);
                            if(dir.size==2)//子目录空直接删除
                            {
                                 // printf("jh\n");
                                temp_inode = iget(dir.direct[i].d_ino);
                                chdir(str);
                                dir.direct[i].d_ino = 0;
                                dir.size--;									//目录数--
                                ifree(inode->i_ino);
                                iput(temp_inode);

                            }else{//不为空就递归
                                //需要先回到上一级
                                chdir(str);
                                delete_dir(dir.direct[i].d_name);
                            }
                    }
                    chdir(str);
                }
            }
    }
}

void delete(char *filename){
	int dirid;
	char flag;
	struct inode *inode;

	dirid = namei(filename);
	if (dirid == -1){
		printf("文件不存在，请检查!\n");
		return;
	}
	inode = iget(dir.direct[dirid].d_ino);
	if(!(inode->di_mode &DIFILE)){
		printf("删除文件为目录请确认是否删除，输入Y或者N:\n");
		flag=getchar();
		if(flag=='Y'){
            delete_dir(filename);
            //printf("1");
		}
		iput(inode);
		return;
	}
	dir.direct[dirid].d_ino = DIEMPTY;//修改目录权限为空
	dir.size --;//长度-1
	inode->di_number--;//节点关联文件数-1
	iput(inode);//回收节点
	return;
}
