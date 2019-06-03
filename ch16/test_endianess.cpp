#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int is_bigendian(){
    int x = 0x12345678;
    char* p = (char*)&x;

    if(*p == 0x12){
        printf("big endian\n");
        return 1;
    }else if(*p == 0x78){
        printf("little endian\n");
        return 0;
    }else{
        printf("error??");
        exit(1);
    }
}


int get_process_number(){
    pid_t pid;



    if((pid = fork())<0){
        printf("fork failed\n");
        exit(1);
    }else if(pid == 0){
        //child
        // int fd;
        // fd = dup(STDOUT_FILENO);
        // // close(STDOUT_FILENO);

        // int err = execl("/bin/sh", "sh", "-c", "'ps'", (char *)0);
        // if (err < 0)
        // {
        //     printf("execl failed, %s\n", strerror(errno));
        //     exit(1);
        // }

        FILE* pf;
        if((pf = popen("/bin/sh -c 'ps aux'", "r") )== NULL){
            printf("popen failed\n");
            exit(1);
        }
        char line[1024];
        int cnt =0;

        while(fgets(line, sizeof(line), pf) != NULL){
            ++cnt;
        }
        pclose(pf);

        //kill(getppid(), SIGUSR1);
        printf("child: process %d\n", cnt);
        exit(cnt);  //pass the process num to parent
    }
    int status;

    if(waitpid(pid, &status, 0) <0){
        printf("waitpid failed\n");
        exit(1);
    }

    printf("parent: process %d\n", status/255);

}

int main(){
    is_bigendian();
    get_process_number();
}