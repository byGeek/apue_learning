#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

size_t readn(int fd, void* buf, size_t len){
    int remain = len;
    int per_read;
    char* ptr = (char*)buf;

    while(remain>0){
        per_read = read(fd, ptr, remain);
        if(per_read < 0){
            if(remain == len){
                return -1;  //no data have been read, return false
            }else{
                break;  //return readed data number
            }
        }else if(per_read == 0){
            break;  //eof
        }
        remain -= per_read;
        ptr += per_read;
    }
    return len -remain;
}

size_t writen(int fd, const void* buf, size_t len){
    int per_writen;
    int remain = len;
    char* ptr = (char*)buf;
    while(remain >0){
        per_writen = write(fd, ptr, remain);
        if(per_writen < 0){
            if(remain == len){
                return -1;
            }else{
                break;
            }
        }else{
            remain -=per_writen;
            ptr+=per_writen;
        }
    }
    return len -remain;
}

int get_filesize(const char* filename){
    struct stat buf;
    if(stat(filename, &buf)<0){
        printf("error in stat\n");
        return 0;
    }
    return buf.st_size;
}

#define FILE_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

int main(){
    const char* filename = "testfile";
    int ofd = open(filename, O_RDWR | O_CREAT, FILE_MODE);
    if(ofd <0){
        printf("error in open file: %s\n", filename);
        exit(1);
    }

    const char* in_file = "/etc/services";
    int ifd = open(in_file, O_RDONLY);
    if(ifd <0){
        printf("can not open /etc/services");
        exit(1);
    }

    long size = get_filesize(in_file);
    char buf[size];

    size_t read_size = readn(ifd, buf, size);
    if(read_size != size){
        printf("short readn \n");
        exit(1);
    }

    size_t write_size = writen(ofd, buf, size);
    if(write_size != size){
        printf("short write\n");
        exit(1);
    }

    exit(0);
}