#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

int main()
{

    //创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    //向服务器（特定的IP和端口）发起请求
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));           //每个字节都用0填充
    serv_addr.sin_family = AF_INET;                     //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //具体的IP地址
    serv_addr.sin_port = htons(1234);                   //端口
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    //读取服务器传回的数据
    // char buffer[40];
    // read(sock, buffer, sizeof(buffer) - 1);
    // printf("Message form server: %s\n", buffer);

    //向服务器发送文件名
    char fileName[100] = {0};
    printf("input filename:\n"); //输入新文件名称
    gets(fileName);
    send(sock, fileName, strlen(fileName), 0);

    //接收服务器结果
    char buffer[BUFFER_SIZE] = {0};
    recv(sock, buffer, 1, 0);
    if(*buffer == 'N'){
        printf("Server does not exist this file\n");
        close(sock);
        exit(0);
    }
    //创建文件
    FILE *fp = fopen(fileName, "wb");                   //二进制写(创建)
    if (fp == NULL)
    {
        printf("create file fail\n");
        exit(0);
    }
    sleep(1);                                           //防止粘包
    //循环接收数据, 直到传输完毕
    memset(buffer, 0, BUFFER_SIZE);                     //清空缓冲
    int nCount = 0;
    while ((nCount = recv(sock, buffer, BUFFER_SIZE, 0)) > 0)
    {
        fwrite(buffer, nCount, 1, fp);
    }
    puts("Success\n");

    fclose(fp);
    //关闭套接字(发送FIN包)
    close(sock);

    return 0;
}