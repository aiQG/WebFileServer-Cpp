// #include <stdio.h>
// #include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string>

#define BUFFER_SIZE 1024

class ServerMenu{
private:
    int serv_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;


public:
    int clnt_sock;
    
    ServerMenu();
    ~ServerMenu();
    FILE* catFile(char*);
    int sendFile();
    int recvFile();
    int deleteFile();
    
    int selectedFunction;
};