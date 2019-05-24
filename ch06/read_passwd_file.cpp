#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <pwd.h>

int test_direct_access();
int test_use_api();

int main(){

    printf("use fopen to directly open /etc/passwd file\n");
    test_direct_access();

    printf("use setpwent to iterate /etc/passwd file\n");
    test_use_api();

}


int test_direct_access(){
    const char* pwfile = "/etc/passwd";

    FILE* pfile;

    pfile = fopen(pwfile, "r");
    if(pfile == NULL){
        printf("fopen failed: %s\n", strerror(errno));
        return -1;
    }

    char line[512];
    while(fgets(line, 512, pfile) != NULL){
        printf("%s", line);
    }

    fclose(pfile);
}


int test_use_api(){
    setpwent();

    struct passwd* ppw;

    while((ppw = getpwent()) != NULL){
        printf("%s\n", ppw->pw_name);
    }

    endpwent();

    return 0;
}
