#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

//返回path中最后一个/后的字符串
char*
fmtname(char *path)
{
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  return p;
}

//在路径path下查找文件f
void
find(char *path, char *f)
{
  char buf[512], *p;
  int fd;
  struct dirent de;//一个link,结构体包含文件名和文件名对应的node编号
  struct stat st;//一个node,结构体包含该实体文件对象的相关信息

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s#\n", path);
    return;
  }
  // else{
  //   printf("find open %s with fd%d#\n", path, fd);
  // }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot fstat %s#\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    if(strcmp(fmtname(path), f) == 0){
      printf("%s\n", path);
    }
    //递归调用返回时注意close掉当前fd,否则超出15个后无法再open新的文件
    close(fd);
    return;
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      //0表示已被删除还未被抹除的文件
      if(de.inum == 0 || !strcmp(de.name, ".") || !strcmp(de.name, ".."))
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      //递归调用
      find(buf, f);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc < 3){
    fprintf(2,"2 arguments needed\n");
    exit(1);
  }
  find(argv[1], argv[2]);
  exit(0);
}
