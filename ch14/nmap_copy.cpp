#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#define FILE_MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

#define FILE_SIZE 350*1024*1024  //why file size is 367M, not 350??

#define MAP_SIZE 100*1024*1024

#define error_print(msg) (printf("%s, reason:%s\n", msg, strerror(errno)))
//todo: mmap demo
//todo: mmap demo vs read/write performance

int get_filesize(const char* filename);

int write_dummy_data(const char* filename){
    //write some dummy data
    int fd = open(filename, O_RDWR, FILE_MODE);

    lseek(fd, 0, SEEK_SET);
    const char* d1 = "hello,world";
    const char* d2 = "deadend";

    if(write(fd, d1, strlen(d1)) != strlen(d1)){
        printf("write failed\n");
        exit(1);
    }
    lseek(fd, -strlen(d2), SEEK_END);
    if(write(fd, d2, strlen(d2)) != strlen(d2)){
        printf("write failed\n");
        exit(1);
    }

    close(fd);
    return 0;
}

int create_file(const char* dst_file, off_t size);

int mmap_copy(const char* dst_file, const char* src_file);

int normal_copy(const char* dst_file, const char* src_file);

int main(){

    int err;

    const char* src = "testfile";
    const char* dst = "dst_testfile";
    
    err = create_file(src, FILE_SIZE);
    if(err != 0){
        printf("create file failed\n");
        exit(1);
    }

    write_dummy_data(src);

    mmap_copy(dst, src);


}

int create_file(const char* dst_file, off_t size){
    int fd;

    fd = open(dst_file, O_RDWR | O_CREAT | O_TRUNC, FILE_MODE);
    if(fd < 0){
        printf("error create file: %s", dst_file);
        return -1;
    }

    if(ftruncate(fd, size) < 0){
        printf("ftruncate failed\n");
        return -1;
    }

    // struct stat buf;
    // if(fstat(fd, &buf)< 0){
    //     error_print("stat failed");
    //     exit(1);
    // }
    // printf("file size: %ld\n", buf.st_size);

    close(fd);
    return 0;
}

int get_filesize(const char* filename){
    struct stat buf;
    if(stat(filename, &buf)<0){
        printf("error in stat\n");
        return 0;
    }
    return buf.st_size;
}

int mmap_copy(const char* dst, const char* src){
    int sfd, dfd;

    sfd = open(src, O_RDONLY);
    if(sfd < 0){
        printf("open failed\n");
        return -1;
    }

    size_t filesize = get_filesize(src);

    create_file(dst, filesize);  //create dst file with same size with src

    dfd = open(dst, O_RDWR);
    if(dfd < 0){
        printf("open failed\n");
        return -1;
    }

    size_t remain = filesize;
    off_t offset = 0;

    while(remain > 0){
        size_t nsize;
        if(remain > MAP_SIZE){
            nsize = MAP_SIZE;
        }else{
            nsize = remain;
        }

        void* src_addr = mmap(0, nsize, PROT_READ, MAP_SHARED, sfd, offset);
        if(src_addr == MAP_FAILED){
            error_print("mmap failed");
            exit(1);
        }
        void* dst_addr = mmap(0, nsize, PROT_WRITE, MAP_SHARED, dfd, offset);
        if(dst_addr == MAP_FAILED){
            error_print("mmap failed");
            exit(1);
        }

        memcpy(dst_addr, src_addr, nsize);
        offset += nsize;
        remain -= nsize;

        // if(msync(dst_addr, nsize, MS_SYNC) <0){
        //     error_print("msync failed");
        //     exit(1);
        // }

        munmap(src_addr, nsize);
        munmap(dst_addr, nsize);
    }

}