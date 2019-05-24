#include <unistd.h>
#include <stdio.h>
#include <time.h>

void test_string_cat();

int main(void){

    /*
    time_t t;

    time(&t);

    struct tm* tmp;
    tmp = localtime(&t);

    char str[256];
    strftime(str, 256, "%F %X", tmp);
    printf("%s\n", str);


    struct tm reverse_tm;
    strptime(str, "%F %X", &reverse_tm);

    */
   test_string_cat();

    return 0;
}

void test_string_cat(){
    const char* s1 = "hello";
    const char* s2 = "world";

    const char* r = "abc" "aaa";
    printf("%s", r);
}