// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <sys/socket.h>
// #include <netinet/in.h>

// #define BUFFER_SIZE 1024

#include "ServerMenu.h"

int main(){

    ServerMenu SM = ServerMenu();

    switch(SM.selectedFunction){
        case 1:
        SM.recvFile();
        break;

        case 2:
        SM.sendFile();
        break;

        default:
        break;
    }

    SM.~ServerMenu();
    
    return 0;
}