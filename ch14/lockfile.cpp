#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>


int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len){
    struct flock lock;

    lock.l_type = type;
    lock.l_start = offset;
    lock.l_whence = whence;
    lock.l_len = len;

    return (fcntl(fd, cmd, &lock));
}

int lockfile(int fd){
    return lock_reg(fd, F_SETLK, F_WRLCK, 0, SEEK_SET, 0);
}

int unlockfile(int fd){
    return lock_reg(fd, F_SETLK, F_UNLCK, 0, SEEK_SET, 0);
}

int lockfile_wait(int fd){
    return lock_reg(fd, F_SETLKW, F_WRLCK, 0, SEEK_SET, 0);
}

int main(){
    pid_t pid;
    int fd = open("testfile", O_RDWR|O_CREAT, S_IRUSR| S_IWUSR);
    if(fd < 0){
        printf("create file failed\n");
        exit(1);
    }

    if((pid=fork())<0){
        printf("error in fork\n");
        exit(1);
    }else if(pid == 0){
        printf("child want to lock file\n");
        int err =lockfile(fd);
        if(err<0){
            printf("child lock failed\n");
            exit(1);
        }
        printf("child locked file\n");
        sleep(5);
        if((err = unlockfile(fd)) < 0){
            printf("child unlock file failed\n");
            exit(1);
        }
        printf("child unlock file successful\n");
        sleep(5);

    }else{
        sleep(1);  //let child start first
        printf("parent want to lock file\n");
        int err;
        //if((err = lockfile(fd))<0){
        if((err = lockfile_wait(fd)) <0){  //wait will pause parent process until file unlocked
            printf("parent lock file failed\n");
            exit(1);
        }
        printf("parent locked file\n");
    }

    exit(0);
}