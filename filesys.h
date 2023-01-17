#ifndef _FILESYS_H
#define _FILESYS_H

#define delete 		dele     		//delete 在c++里是保留字  by tangfl
//#define exit(a)		return			//建议使用 return         by tangfl

//All Defines
#define BLOCKSIZ        512			//每个块的大小
#define SYSOPENFILE     40
#define DIRNUM          128			//一个目录下 最多 子目录数
#define DIRSIZ          12			//目录名长度 windows32下int长度为4 xiao 14->12
#define PWDSIZ 			12			//密码最大长度
#define PWDNUM 			32
#define NOFILE   		20			//用户最多打开文件数
#define NADDR  			10			//i节点内存储
#define NHINO 			128			//hash数  ?????????/* must be power of 2 */
#define USERNUM 		10
#define DINODESIZ 		52			//di节点的大小 int=4 应为2*5++4*10 此处可能系统为了对齐内存，故如此调整


/*filesys*/
#define DINODEBLK  		32							//i节点占用的块数
#define FILEBLK   		512							//数据块数
#define NICFREE  		50							//超级块内空闲块堆栈大小
#define NICINOD  		50							//超级块内空闲i节点数组大小
#define DINODESTART  	(2*BLOCKSIZ)				//i节点开始地址 空出1024，第一个为引导区，第二块为超级块
#define DATASTART  		((2+DINODEBLK)*BLOCKSIZ)	//数据区开始地址 DINODESTART+DINODEBLK*BLOCKSIZ	/*d:17408    0x4400*/


/*di_mode*/
#define DIEMPTY   		00000						//空权限
#define DIFILE      	01000						//类型 文件
#define DIDIR     		02000						//类型 目录

#define UDIREAD 		00001						//用户权限
#define UDIWRITE  		00002
#define UDIEXICUTE  	00004
#define GDIREAD   		00010						//用户组权限
#define GDIWRITE  		00020
#define GDIEXICUTE  	00040
#define ODIREAD  		00100						//pubilc权限
#define ODIWRITE 		00200
#define ODIEXICUTE 		00400


#define READ  			1
#define WRITE 			2
#define EXICUTE 		3


#define DEFAULTMODE 	00777					//默认权限


/* i_flag */
#define  IUPDATE  		00002

/* s_fmod */
#define SUPDATE  		00001

/* f_flag */
#define FREAD   		00001
#define FWRITE   		00002
#define FAPPEND   		00004

//Includes
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

/* error */
#define DISKFULL  		65535

/* fseek origin */
//#define SEEK_SET  		0


struct inode//内存i节点
{
    struct inode  	*i_forw;
    struct inode  	*i_back;
    char 			i_flag;
    int  	i_ino;          /*磁盘i 节点标志*/
    int  	i_count;     	/*引用计数*/
    short  di_number; 		/*关联文件数。当为0 时，则删除该文件*/
    short  di_mode;  		/*存取权限*/
    short  di_uid;
    short  di_gid;
    short  di_size;   		/*文件大小*/
    int   	di_addr[NADDR];   /*物理块号*/
};

struct dinode//磁盘i节点
{
    short 	di_number; 		/*关联文件数*/
    short 	di_mode; 		/*存取权限*/
    short 	di_uid;
    short 	di_gid;
    short 	di_size;  		/*文件大小*/
    int 	di_addr[NADDR];   /*物理块号*/
};


struct direct
{
    char d_name[DIRSIZ];
    int d_ino;
};

struct filsys
{
    short  	s_isize;   			/*i节点块块数*/
    long   	s_fsize;   			/*数据块块数*/
    int   		s_nfree;    		/*空闲块块数*/
    short  	s_pfree;  			/*空闲块指针*/
    int  		s_free[NICFREE];  	/*空闲块堆栈*/

    int  		s_ninode;  			/*number of free inode in s_inode*/
    short int 			s_pinode;  			/*pointer of the sinode*/
    int  		s_inode[NICINOD];   /*空闲i节点数组*/
    int 		s_rinode;    		/*remember inode*/

    char 				s_fmod;  			/*超级块修改标志*/
};

struct pwd
{
    short 		p_uid;
    short 		p_gid;
    char 				password [PWDSIZ];
};

struct dir
{
    struct direct 		direct[DIRNUM];
    int 				size;  				/*当前目录大小*/
};

struct hinode
{
    struct inode 		*i_forw;   /*HASH表指针，查找内存索引节点*/
};

struct file
{
    char 			f_flag;    		/*文件操作标志*/
    int 	f_count;  		/*引用计数*/
    struct inode 	*f_inode; 		/*指向内存i节点*/
    long 	f_off;   		/*read/write character pointer*/
};

struct user
{
    short 	u_default_mode;
    short 	u_uid;
    short 	u_gid;
    short 	u_ofile[NOFILE];   /*用户打开文件表*/
};


//all variables
extern struct hinode   hinode[NHINO];
extern struct dir      dir;           /*当前目录(在内存中全部读入)*/
extern struct file     sys_ofile[SYSOPENFILE];
extern struct filsys   filsys;        /*内存中的超级块*/
extern struct pwd      pwd[PWDNUM];
extern struct user     user[USERNUM];
//extern struct file     *fd;           /*the file system column of all the system*/    //xiao
extern struct inode    *cur_path_inode;
extern int             user_id;
extern char            disk[(DINODEBLK+FILEBLK+2)*BLOCKSIZ];

// all functions
extern struct        inode *iget(int);
extern void          iput(struct inode *);
extern int  balloc(int);
extern int  balloc();
extern void          bfree(int);
extern struct inode* ialloc();
extern void          ifree(int);
extern int			 namei(char *);
extern short iname(char *);
extern int  access(int,struct inode *,short);
extern void          _dir();
extern void          mkdir(char *);
extern void          chdir(char *);
extern short		 open(int,char*,char);
extern int           creat(int,char *,short);
extern int  read(int fd, char *buf, int size);
extern int  write(int fd,char *buf,int size);
extern int           login(short, char *);
extern int           logout(short);
extern void          install();
extern void          format();
extern void          close(int,short);
extern void          halt();
extern void          delete(char *);
extern int 			 shell(int user_id,char *str);
extern void          adduser(int,unsigned short);
extern void          userdel(int,unsigned short);
extern void          modgroup(int,unsigned short,unsigned short);
extern void          modpwd(int,char*);
extern void          usershow(int);
extern void          who(int);/*显示当前用户的所有信息，包括这个用户的id号、登录密码、所属用户组*/
#endif
