#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"
#define MAXLEN 512


int main(int argc, char *argv[]){
    char c, buf[MAXLEN];//c为读取字符缓冲区,buf为一个命令行参数缓冲区
    char *newArgv[MAXARG];//newArgv为exec执行的程序的参数列表
    int k = argc-1, j = 0;

    for(int i=1; i<argc; ++i) newArgv[i-1] = argv[i];//每行输入的前几个参数固定为xargs后的几个参数
    while(read(0, &c, sizeof(c))){
        // printf("+%c+\n",c);
        if(c == ' ' || c == '\t' || c == '\n'){
            buf[j] = '\0';
            newArgv[k++] = buf;
            // printf("-%s,%s-",buf, newArgv[k-1]);
            j = 0;
            if(c == '\n'){
                if(fork() == 0){
                    // for(int i=0; i<k; i++)printf("#%s#\n", newArgv[i]);
                    newArgv[k] = 0;
                    exec(argv[1], newArgv);
                }
                else{
                    wait(0);
                    k = argc-1;
                }
            }
        }
        else{
            buf[j++] = c;
        }
    }
    exit(0);
}