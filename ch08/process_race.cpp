#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void dump_str(const char* str);

int main(void){
    pid_t pid;

    if((pid = fork()) < 0){
        printf("fork failed\n");
        _exit(0);
    }else if(pid == 0){
        //child process
        dump_str("hello,world\n");
    }else{
        //parent process
        dump_str("hello,world\n");
    }

    exit(0);
}

void dump_str(const char* str){
    setbuf(stdout, NULL);  //set stdout no buff, every putc will call  write system call
    char ch;
    while((ch = *str++) != '\0'){
        //printf("%c", ch);
        putc(ch, stdout);
    }
}