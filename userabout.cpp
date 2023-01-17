#include "filesys.h"

int usernum() {
    int i;
    for (i = 0; i < PWDNUM; i++) {
        if(pwd[i].p_gid==0)
            return i;
    }
    return -1;
}

void adduser(int user_id,unsigned short username) { //权限为4 
    FILE *fpWrite = fopen("passwd.txt", "a");
    if (user[user_id].u_gid==4) {
        int num = usernum(),i;
        for (i = 0; i < PWDNUM; i++) {
            if(pwd[i].p_uid==username)
                break;
        }
        if (i!=PWDNUM) {
            printf("该用户名已被使用，请重新创建\n");
        }
        else{
            if (num >= 0) {
                pwd[num].p_uid = username;
                pwd[num].p_gid = 2;
                strcpy(pwd[num].password, "1234");

                char p_uid[1024];
                char p_gid[1024];
                int a=strlen(itoa(pwd[num].p_uid,p_uid,10));
                int b=strlen(itoa(pwd[num].p_gid,p_gid,10));
                int c=strlen(pwd[num].password);
                fprintf(fpWrite,"\n");
                for(i=0;i<a+1+b+1+c;i++){
                    if(i<a){
                        fprintf(fpWrite,"%c",itoa(pwd[num].p_uid,p_uid,10)[i]);
                    }//增加用户名
                    if(i==a || i==a+1+b){
                        fprintf(fpWrite,"%c",'-');
                    }//增加分隔符
                    if(i>=a+1 && i<=a+b){
                        fprintf(fpWrite,"%c",itoa(pwd[num].p_gid,p_gid,10)[i-a-1]);
                    }//增加用户权限
                    if(i>=a+b+2 && i<a+b+2+c){
                        fprintf(fpWrite,"%c",pwd[num].password[i-a-b-2]);
                    }//增加用户密码
                }
                fclose(fpWrite);
                printf("用户%hu创建成功，初始密码为：1234，初始组号为：2\n", username);
            }
            else
                printf("用户数量超出上限，不能继续添加\n");
        }

    }
    else
        printf("权限不足，请联系管理员更改权限\n");
}

void userdel(int user_id,unsigned short username) {
    //权限为4 
    FILE *fpWrite = fopen("passwd.txt", "w");
    if (user[user_id].u_gid==4) {
        int i;
        if(user[user_id].u_uid==username) {
            printf("你不能删除自己！\n");
            return;
        }

        for (i = 0; i < PWDNUM; i++) {
            if (username == pwd[i].p_uid) {
                //pwd
                for (int j = i; j < PWDNUM; j++) {
                    pwd[j].p_uid = pwd[j + 1].p_uid;
                    pwd[j].p_gid = pwd[j + 1].p_gid;
                    strcpy(pwd[j].password, pwd[j + 1].password);
                }
                printf("删除完毕\n");
                break;
            }
        }
        if (i == PWDNUM)
            printf("未找到该用户\n");
        else{
            for (i = 0; i < PWDNUM; i++) {
                if(pwd[i].p_gid!=0){
                    char p_uid[1024];
                    char p_gid[1024];
                    int a=strlen(itoa(pwd[i].p_uid,p_uid,10));
                    int b=strlen(itoa(pwd[i].p_gid,p_gid,10));
                    int c=strlen(pwd[i].password);
                    for(int j=0;j<a+1+b+1+c;j++){
                        if(j<a){
                            fprintf(fpWrite,"%c",itoa(pwd[i].p_uid,p_uid,10)[j]);
                            //printf("%c",itoa(pwd[i].p_uid,p_uid,10)[j]);
                        }
                        if(j==a || j==a+1+b){
                            fprintf(fpWrite,"%c",'-');
                            //printf("%c",'-');
                        }
                        if(j>=a+1 && j<=a+b){
                            fprintf(fpWrite,"%c",itoa(pwd[i].p_gid,p_gid,10)[j-a-1]);
                            //printf("%c",itoa(pwd[i].p_gid,p_gid,10)[j-a-1]);
                        }
                        if(j>=a+b+2 && j<a+b+2+c){
                            fprintf(fpWrite,"%c",pwd[i].password[j-a-b-2]);
                            //printf("%c",pwd[i].password[j-a-b-2]);
                        }
                    }
                    fprintf(fpWrite,"\n");
                }
            }
        }
        fclose(fpWrite);

    } else
        printf("权限不足，请联系管理员更改权限\n");
}

void modgroup(int user_id,unsigned short username,unsigned short auth){
    FILE *fpWrite = fopen("passwd.txt", "w");
    if (user[user_id].u_gid==4) {
        int i;
        for (i = 0; i < PWDNUM; i++) {
            if (username == pwd[i].p_uid) {
                pwd[i].p_gid = auth;
                printf("修改组成功\n");
                break;
            }
        }
        if (i == PWDNUM)
            printf("未找到该用户\n");
        else{
            for(i =0; i < PWDNUM; i++) {
                if(pwd[i].p_gid!=0){
                    char p_uid[1024];
                    char p_gid[1024];
                    int a=strlen(itoa(pwd[i].p_uid,p_uid,10));
                    int b=strlen(itoa(pwd[i].p_gid,p_gid,10));
                    int c=strlen(pwd[i].password);
                    for(int j=0;j<a+1+b+1+c;j++){
                        if(j<a){
                            fprintf(fpWrite,"%c",itoa(pwd[i].p_uid,p_uid,10)[j]);
                            //printf("%c",itoa(pwd[i].p_uid,p_uid,10)[j]);
                        }
                        if(j==a || j==a+1+b){
                            fprintf(fpWrite,"%c",'-');
                            //printf("%c",'-');
                        }
                        if(j>=a+1 && j<=a+b){
                            fprintf(fpWrite,"%c",itoa(pwd[i].p_gid,p_gid,10)[j-a-1]);
                            //printf("%c",itoa(pwd[i].p_gid,p_gid,10)[j-a-1]);
                        }
                        if(j>=a+b+2 && j<a+b+2+c){
                            fprintf(fpWrite,"%c",pwd[i].password[j-a-b-2]);
                            //printf("%c",pwd[i].password[j-a-b-2]);
                        }
                    }
                    fprintf(fpWrite,"\n");
                }
            }
        }
        fclose(fpWrite);

    } else
        printf("权限不足，请联系管理员更改权限\n");
}


void modpwd(int user_id,char* passwd) {
    //修改当前用户密码，每个用户只能改自己的密码
    FILE *fpWrite = fopen("passwd.txt", "w");//fufu
    int i,j;
    for (i = 0; i < PWDNUM; ++i) {
        if(pwd[i].p_uid==user[user_id].u_uid) {
            printf("请再次输入您的密码：\n");
            char repasswd[PWDSIZ];
            scanf("%s", repasswd);
            while (strcmp(repasswd, passwd)) {
                printf("密码与第一次不同，请再次输入您的密码：\n");
                scanf("%s", repasswd);
            };
            strcpy(pwd[i].password,repasswd);
            printf("密码修改成功!\n");
            break;
        }
    }
    if (i==PWDNUM) {
        printf("系统错误\n");
    }
    else{
        for (i = 0; i < PWDNUM; ++i) {
            if(pwd[i].p_gid!=0){
                char p_uid[1024];
                char p_gid[1024];
                int a=strlen(itoa(pwd[i].p_uid,p_uid,10));
                int b=strlen(itoa(pwd[i].p_gid,p_gid,10));
                int c=strlen(pwd[i].password);

                for(int j=0;j<a+1+b+1+c;j++){
                    if(j<a){
                        fprintf(fpWrite,"%c",itoa(pwd[i].p_uid,p_uid,10)[j]);
                        //printf("%c",itoa(pwd[i].p_uid,p_uid,10)[j]);
                    }
                    if(j==a || j==a+1+b){
                        fprintf(fpWrite,"%c",'-');
                        //printf("%c",'-');
                    }
                    if(j>=a+1 && j<=a+b){
                        fprintf(fpWrite,"%c",itoa(pwd[i].p_gid,p_gid,10)[j-a-1]);
                        //printf("%c",itoa(pwd[i].p_gid,p_gid,10)[j-a-1]);
                    }
                    if(j>=a+b+2 && j<a+b+2+c){
                        fprintf(fpWrite,"%c",pwd[i].password[j-a-b-2]);
                        //printf("%c",pwd[i].password[j-a-b-2]);
                    }
                }
                fprintf(fpWrite,"\n");
            }
        }
    }
    fclose(fpWrite);

}

void usershow(int user_id) { //权限等于4 
    if (user[user_id].u_gid==4) {
        printf("用户名\t密码\t\t权限\n");
        for(int i = 0;i<usernum();i++){
            printf("%hu\t%s\t%hu\n",pwd[i].p_uid,pwd[i].password,pwd[i].p_gid);
        }
    }
    else
        printf("权限不足，请联系管理员更改权限\n");
}
void who(int user_id) /*显示当前用户的所有信息，包括这个用户的id号、登录密码、权限*/
{
    int i;
    printf("用户名\t密码\t权限\n");
    for (i = 0; i < PWDNUM; ++i)
    {
        if(pwd[i].p_uid==user[user_id].u_uid)
        {
            printf("%hu\t%s\t%hu\n",pwd[i].p_uid,pwd[i].password,pwd[i].p_gid);
        }
    }
}

