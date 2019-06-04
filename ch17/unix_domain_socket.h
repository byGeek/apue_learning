#include <unistd.h>

#define SERVER_PATH "/var/tmp/mytest.socket"

int server_listen(const char* name);

int server_accept(int listenfd, uid_t* puid);

int client_connect(const char* name);