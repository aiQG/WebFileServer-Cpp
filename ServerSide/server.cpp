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


    // //创建套接字
    // int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // //将套接字和IP、端口绑定
    // struct sockaddr_in serv_addr;
    // memset(&serv_addr, 0, sizeof(serv_addr));           //每个字节都用0填充
    // serv_addr.sin_family = AF_INET;                     //使用IPv4地址
    // serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //具体的IP地址
    // serv_addr.sin_port = htons(1234);                   //端口
    // bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // //进入监听状态，等待用户发起请求(缓冲区长度20)
    // listen(serv_sock, 20);

    // //接收客户端请求
    // struct sockaddr_in clnt_addr;
    // socklen_t clnt_addr_size = sizeof(clnt_addr);
    // int clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    ServerMenu SM = ServerMenu();

    // char buffer[BUFFER_SIZE] = {0};                     //缓冲区
    // //接收客户端发送的文件名
    // int recvLen = recv(clnt_sock, buffer, BUFFER_SIZE, 0);
    // //尝试读取文件
    // FILE *fp = fopen(buffer, "rb");                   //二进制读
    // if(fp == NULL){
    //     printf("No such file\n");
    //     char x = 'N';
    //     send(clnt_sock, &x, 1, 0);                    //发送结果
    //     close(clnt_sock);
    //     close(serv_sock);
    //     exit(0);
    // } else {
    //     char x = 'Y';
    //     send(clnt_sock, &x, 1, 0);
    // }

    // //向客户端发送数据
    // // char str[] = "Hello World!";
    // // write(clnt_sock, str, sizeof(str));
    // memset(buffer, 0, BUFFER_SIZE);                     //清空缓冲区
    // int nCount = 0;
    // while ((nCount = fread(buffer, 1, BUFFER_SIZE, fp)) > 0){
    //     send(clnt_sock, buffer, nCount, 0);
    // }
    // fclose(fp);
    switch(SM.selectedFunction){
        case 1:
        break;

        case 2:
        SM.sendFile();
        break;

        default:
        break;
    }





    // //文件读取完毕, 断开输出流, 发送FIN包
    // shutdown(clnt_sock, SHUT_WR);
    // //阻塞等待客户端返回ACK包
    // recv(clnt_sock, buffer, BUFFER_SIZE, 0);


    // //关闭套接字
    // close(clnt_sock);
    // close(serv_sock);
    SM.~ServerMenu();
    
    return 0;
}