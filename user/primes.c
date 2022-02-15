#include "kernel/types.h"
#include "user.h"

//从pl读取上一轮筛选后数据,从pr将本轮筛选后数据传输给下一轮
void childProcess(int pl[2]){
    int pr[2], i, curPrime;
    close(pl[1]);
    if(!read(pl[0], &curPrime, sizeof(curPrime))) exit(0);//递归终止条件
    
    pipe(pr);
    if(fork() == 0){
        childProcess(pr);
    }else {
        printf("prime %d\n", curPrime);
        //将符合条件的数传递给子进程进行进一步筛选
        close(pr[0]);
        while(read(pl[0], &i, sizeof(i))){
            if(i % curPrime != 0){
                write(pr[1], &i, sizeof(i));
            }
        }
        close(pl[0]);
        close(pr[1]);
        wait(0);
    }
    exit(0);
}

int main(){
    int p[2], i;

    pipe(p);
    if(fork() == 0){
        childProcess(p);
    } else{
        close(p[0]);
        for(i=2; i<=35; ++i){
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);
        wait(0);
    }
    exit(0);
}