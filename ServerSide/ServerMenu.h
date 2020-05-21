#include <stdio.h>
// #include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include <thread>

#define BUFFER_SIZE 1024

class ServerMenu
{
private:
    int serv_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    int clnt_sock;
    int selectedFunction;

    void runThread(int, int);
    void closeThread(int);

    FILE *catFile(char *, int);
    int sendFile(int);
    int recvFile(int);
    int deleteFile(int);

public:
    ServerMenu();
    ~ServerMenu();

};