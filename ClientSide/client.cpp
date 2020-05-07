#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "menu.h"

#define BUFFER_SIZE 1024

int main()
{
    menu Menu = menu();
    Menu.showMenu();

    char fileName[100] = {0};
    switch(Menu.selectedFunction){
        case 1:
        break;

        case 2:
        printf("input filename:\n"); //输入新文件名称
        // gets(fileName);
        std::cin >> fileName;
        Menu.download(fileName);

        default:
        break;
    }
    

    //关闭套接字(发送FIN包)
    Menu.~menu();

    return 0;
}