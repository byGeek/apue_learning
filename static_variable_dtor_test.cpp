#include <iostream>

class singleton
{
public:
    ~singleton()
    {
        delete (char *)data;
    }

    static singleton &get_instance()
    {
        static singleton self;
        return self;
    }

    singleton(const singleton &) = delete;
    singleton &operator=(const singleton &) = delete;

    void do_something() {}

private:
    singleton()
    {
        data = new char[1024];
    }
    void *data;
};

class foo
{
public:
    foo()
    {
        std::cout << "in foo ctor" << std::endl;
    }
    ~foo()
    {
        std::cout << "in foo dtor" << std::endl;
    }
};

void test()
{
    singleton &s = singleton::get_instance();
    s.do_something();

    foo f;
}

int main()
{
    test();
}