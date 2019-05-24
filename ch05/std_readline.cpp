#include <stdio.h>
#include <stdlib.h>

int main(){
    char buf[512];

    /*
    ctrl + D : means EOF
    */
    while(fgets(buf, 512, stdin) != NULL){
        fputs(buf, stdout);
    }

    if(ferror(stdin)){
        printf("stdin error!\n");
    }

    exit(0);
}