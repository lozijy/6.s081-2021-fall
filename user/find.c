#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}
    char buf[512],*p;
void recursion(char *path,char *pwd,char *tar_str){

    int fd;
      struct dirent de;
  struct stat st;
    if((fd = open(pwd, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", pwd);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", pwd);
    close(fd);
    return;
  }
    printf("%s--%s--%s--%d--%d\n",pwd,fmtname(pwd),tar_str,strcmp(fmtname(pwd),tar_str)==0,st.type);
switch(st.type){
  case T_FILE:
    if(strcmp(fmtname(pwd),tar_str)==0){
        printf("%s\n",pwd);
    }
    break;

  case T_DIR:

    printf("dir\n");
    if(strlen(pwd) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: pwd too long\n");
    }
    strcpy(buf, pwd);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      strcpy(pwd,buf);
      if(strcmp(fmtname(pwd),".             ")!=0 && strcmp(fmtname(pwd),"..            ")!=0){
        printf("can ");
        recursion(path,pwd,tar_str);
      }
    }
    break;
  
}
}
int
main(int argc, char *argv[])
{
    char*name=argv[1];
    char *pwd=".";
    static char tar_str[DIRSIZ+1];
    memmove(tar_str, argv[2], strlen(argv[2]));
    memset(tar_str+strlen(argv[2]), ' ', DIRSIZ-strlen(argv[2]));

    recursion(name,pwd,tar_str);
    exit(0);
}
