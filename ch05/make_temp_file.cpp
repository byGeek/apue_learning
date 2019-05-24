#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


int main(int argc, char* argv[]){

    FILE* pfile;
    pfile = tmpfile();
    if(pfile == NULL){
        printf("tmpfile failed!\n");
        exit(0);
    }

    char data[] = "hello,world";
    int cnt = fwrite(data, sizeof(data), 1, pfile);
    if(cnt != 1){
        printf("fwrite failed, reason: %s\n", strerror(errno));
        exit(0);
    }

    rewind(pfile);

    char tmp[512];
    cnt = fread(tmp, sizeof(data), 1, pfile);
    tmp[sizeof(data)*cnt] = 0;

    printf("data read: %s\n", tmp);
}