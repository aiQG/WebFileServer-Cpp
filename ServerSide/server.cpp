// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <sys/socket.h>
// #include <netinet/in.h>

// #define BUFFER_SIZE 1024

#include "ServerMenu.h"

int main()
{

    ServerMenu SM = ServerMenu();

    char buffer[BUFFER_SIZE] = {0}; //缓冲区
    switch (SM.selectedFunction)
    {
    case 1:
        SM.recvFile();
        break;

    case 2:
        SM.sendFile();
        break;

    case 3:
        //接收客户端发送的文件名
        recv(SM.clnt_sock, buffer, BUFFER_SIZE, 0);
        SM.catFile(buffer);
        break;

    case 4:
        SM.deleteFile();
        break;

    default:
        break;
    }

    SM.~ServerMenu();

    return 0;
}