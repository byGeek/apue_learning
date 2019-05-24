#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>
#include <limits.h>


unsigned long long g_count;
struct timeval g_endtime;

void run(const char* str){
    printf("%s enter\n", str);
    while(true){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    if(tv.tv_sec >= g_endtime.tv_sec &&
        tv.tv_usec >= g_endtime.tv_usec){
            printf("%s : count=%lld\n", str, g_count);
            break;
        }
        else{
            ++g_count;
        }
    }
    exit(0);
}

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("usage: ./a.out nice_num\n");
        exit(0);
    }

    int adj = strtol(argv[1], NULL, 10);
    char* str=NULL;

    int nzero = NZERO;

    setbuf(stdout, NULL);
    gettimeofday(&g_endtime, NULL);
    g_endtime.tv_sec += 5;  //run 10 seconds

    pid_t pid;
    if((pid = fork()) < 0){
        printf("fork failed with error: %d", errno);
        exit(0);
    }else if(pid == 0){
        //child process
        const char* str = "child";
        errno = 0;
        if(nice(adj) == -1 && errno !=0){
            printf("nice failed with %d\n", errno);
            exit(0);
        }
        printf("current nice value in child: %d\n", nice(0) + NZERO);
        run(str);
    }else{
        const char* str = "parent";
        printf("current nice value in parent: %d\n", nice(0) + NZERO);
        run(str);
    }
}