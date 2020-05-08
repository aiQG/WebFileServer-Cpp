#include "ServerMenu.h"

ServerMenu::ServerMenu(){
    //创建套接字
    this->serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //将套接字和IP、端口绑定
    memset(&this->serv_addr, 0, sizeof(this->serv_addr));     //每个字节都用0填充
    this->serv_addr.sin_family = AF_INET;                     //使用IPv4地址
    this->serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //具体的IP地址
    this->serv_addr.sin_port = htons(1234);                   //端口
    bind(this->serv_sock, (struct sockaddr *)&this->serv_addr, sizeof(this->serv_addr));

    //进入监听状态，等待用户发起请求(缓冲区长度20)
    listen(this->serv_sock, 20);

    //接收客户端请求
    this->clnt_addr_size = sizeof(this->clnt_addr);
    this->clnt_sock = accept(this->serv_sock, (struct sockaddr *)&this->clnt_addr, &this->clnt_addr_size);

    recv(this->clnt_sock, &this->selectedFunction, 4, 0);
    std::cout << "function: " << this->selectedFunction << std::endl;
}

ServerMenu::~ServerMenu(){
    char buffer[BUFFER_SIZE] = {0};
    //文件读取完毕, 断开输出流, 发送FIN包
    shutdown(this->clnt_sock, SHUT_WR);
    //阻塞等待客户端返回ACK包
    recv(this->clnt_sock, buffer, BUFFER_SIZE, 0);

    //关闭套接字
    close(clnt_sock);
    close(serv_sock);
}

FILE* ServerMenu::catFile(char* filename){
    //尝试读取文件
    FILE *fp = fopen(filename, "rb");                   //二进制读
    if(fp == NULL){
        char x = 'N';
        send(this->clnt_sock, &x, 1, 0);                    //发送结果
        return NULL;
    } else {
        char x = 'Y';
        send(this->clnt_sock, &x, 1, 0);
        return fp;
    }
}

int ServerMenu::sendFile(){
    char buffer[BUFFER_SIZE] = {0};                     //缓冲区
    //接收客户端发送的文件名
    int recvLen = recv(this->clnt_sock, buffer, BUFFER_SIZE, 0);
    FILE* fp = this->catFile(buffer);
    if(fp == NULL){
        return -1;
    }

    //向客户端发送数据
    memset(buffer, 0, BUFFER_SIZE);                     //清空缓冲区
    int nCount = 0;
    while ((nCount = fread(buffer, 1, BUFFER_SIZE, fp)) > 0){
        send(this->clnt_sock, buffer, nCount, 0);
    }
    fclose(fp);

    //文件读取完毕, 断开输出流, 发送FIN包
    shutdown(this->clnt_sock, SHUT_WR);
    //阻塞等待客户端返回ACK包
    recv(this->clnt_sock, buffer, BUFFER_SIZE, 0);
    return 0;
}