#include "kernel/types.h"
#include "user.h"

int main(){
    int p2c[2], c2p[2];//两组管道分别用于两个方向通信
    pipe(p2c);pipe(c2p);
    char a;
    if(fork() == 0){//子进程先读后写
        close(p2c[1]);
        read(p2c[0], &a, 1);
        close(p2c[0]);
        printf("%d: received ping\n",getpid());
        close(c2p[0]);
        write(c2p[1], "p", 1);
        close(c2p[1]);
    }else {//父进程先写后读
        close(p2c[0]);
        write(p2c[1], "c", 1);
        close(p2c[1]);
        close(c2p[1]);
        read(c2p[0], &a, 1);
        close(c2p[0]);
        printf("%d: received pong\n", getpid());
    }
    exit(0);
}