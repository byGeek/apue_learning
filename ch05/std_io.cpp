#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    char ch;

    while((ch = fgetc(stdin)) != EOF){
        fputc(ch, stdout);
        if(ferror(stdout)){
            break;
        }
    }

    printf("done!\n");
    exit(0);
}