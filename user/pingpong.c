#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define RD 0 //读端
#define WT 1 //写端
int main(int argc,char *argv[]){

    int p2c[2];
    int c2p[2];
    char* buff="abc";
    pipe(p2c);//父亲到孩子的管道
    pipe(c2p);//孩子到父亲的管道
    int a=fork();
    if(a<0){
        fprintf(2,"fork error");
        exit(1);
    }
    //孩子
    else if(a==0){
        close(p2c[WT]);
        close(c2p[RD]);
        if(read(p2c[RD],buff,1)!=1){
            fprintf(2,"child read error");
            exit(1);
        }
        fprintf(1,"%d: received ping",getpid());
        if(write(c2p[WT],buff,1)!=1){
            fprintf(2,"child write error");
            exit(1);
        }
        close(p2c[RD]);
        close(c2p[WT]);
    }
    //父亲
    else{
        close(p2c[RD]);
        close(c2p[WT]);
        if(write(p2c[WT],buff,1)!=1){
            fprintf(2,"parent write error");
            exit(1);
        }
        if(read(c2p[RD],buff,1)!=1){
            fprintf(2,"parent read error");
            exit(1);            
        }
        fprintf(1,"%d: received pong",getpid());
        close(p2c[WT]);
        close(c2p[RD]);
    }
    exit(0);
}