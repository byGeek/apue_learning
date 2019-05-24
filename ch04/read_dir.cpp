#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

#define MAX_PATH 260

static void dopath(const char* path);

int main(int argc, char* argv[]){
    if(argc !=2 ){
        printf("Usage: ./a.out path");
        exit(-1);
    }

    dopath(argv[1]);
    return 0;
}

void dopath(const char* path){
    struct stat statbuff;

    if(lstat(path, &statbuff) == -1){
        return;
    }

    if(S_ISDIR(statbuff.st_mode) == 0){
        /*
        if is not directory, directly print its name
        */
        printf("file: %s\n",path);
        return;
    }

    //if is a directory
    DIR* dir = opendir(path);
    if(dir == NULL){
        printf("can not read directory: %s\n", path);
    }
    struct dirent* pdir;

    //char tpath[MAX_PATH];
    char* tpath = (char*)malloc(sizeof(char)*MAX_PATH);
    int len = strlen(path);

    strcpy(tpath, path);
    tpath[len] = '/';
    while((pdir = readdir(dir)) != NULL){
        if(strcmp(pdir->d_name, ".") == 0
        || strcmp(pdir->d_name, "..") == 0){
            continue;
        }

        strcpy(&tpath[len+1], pdir->d_name);
        tpath[len + 1  + strlen(pdir->d_name)] = 0;
        dopath(tpath);
    }

    if(closedir(dir) < 0){
        printf("can't close directory: %s\n", path);
    }

    free(tpath);
}