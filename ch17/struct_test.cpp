#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/*
https://stackoverflow.com/questions/1675351/typedef-struct-vs-struct-definitions

struct test_t is treated lick c++ struct which is like defining a c++ class.
so you can directly reference it without struct keyword

*/
struct test_t
{
    int abc;
    char name[128];
};

int
main()
{
    test_t t1;
    t1.abc = 1;

    struct test_t t2;
    strcpy(t2.name, "hello");
}