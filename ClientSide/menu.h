// #include <stdio.h>
// #include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <string>

#define BUFFER_SIZE 1024
class menu{
    private:
    // 套接字
    int sock;
    struct sockaddr_in serv_addr;

    // 客户端选择的功能
    unsigned char selectedFunction;


    public:
    menu();
    void showMenu();
    bool catFile(char*);
};