#include "ServerMenu.h"

ServerMenu::ServerMenu()
{
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

    while (1)
    {
        //接收客户端请求
        this->clnt_addr_size = sizeof(this->clnt_addr);
        this->clnt_sock = accept(this->serv_sock, (struct sockaddr *)&this->clnt_addr, &this->clnt_addr_size);

        recv(this->clnt_sock, &this->selectedFunction, 4, 0);
        std::cout << "function: " << this->selectedFunction << std::endl;

        std::thread t(&ServerMenu::runThread, this, this->clnt_sock, this->selectedFunction);
        t.detach();
        // runThread(this->clnt_sock, this->selectedFunction);
    }
}

void ServerMenu::runThread(int cSocket, int selFunc)
{
    char buffer[BUFFER_SIZE] = {0}; //缓冲区
    switch (selFunc)
    {
    case 1:
        this->recvFile(cSocket);
        break;

    case 2:
        this->sendFile(cSocket);
        break;

    case 3:
        //接收客户端发送的文件名
        recv(cSocket, buffer, BUFFER_SIZE, 0);
        this->catFile(buffer, cSocket);
        break;

    case 4:
        this->deleteFile(cSocket);
        break;

    default:
        break;
    }
    //关闭当前连接
    this->closeThread(cSocket);
    return;
}

void ServerMenu::closeThread(int cSocket)
{
    char buffer[BUFFER_SIZE] = {0};
    //文件读取完毕, 断开输出流, 发送FIN包
    shutdown(cSocket, SHUT_WR);
    //阻塞等待客户端返回ACK包
    recv(cSocket, buffer, BUFFER_SIZE, 0);
    //关闭套接字
    close(cSocket);
    return;
}

ServerMenu::~ServerMenu()
{
    char buffer[BUFFER_SIZE] = {0};
    //文件读取完毕, 断开输出流, 发送FIN包
    shutdown(this->clnt_sock, SHUT_WR);
    //阻塞等待客户端返回ACK包
    recv(this->clnt_sock, buffer, BUFFER_SIZE, 0);

    //关闭套接字
    close(this->clnt_sock);
    close(this->serv_sock);
}

FILE *ServerMenu::catFile(char *filename, int cSocket)
{
    //尝试读取文件
    FILE *fp = fopen(filename, "rb"); //二进制读
    if (fp == NULL)
    {
        char x = 'N';
        send(cSocket, &x, 1, 0); //发送结果
        return NULL;
    }
    else
    {
        char x = 'Y';
        send(cSocket, &x, 1, 0);
        return fp;
    }
}

int ServerMenu::sendFile(int cSocket)
{
    char buffer[BUFFER_SIZE] = {0}; //缓冲区
    //接收客户端发送的文件名
    int recvLen = recv(cSocket, buffer, BUFFER_SIZE, 0);
    FILE *fp = this->catFile(buffer, cSocket);
    if (fp == NULL)
    {
        return -1;
    }

    //向客户端发送数据
    memset(buffer, 0, BUFFER_SIZE); //清空缓冲区
    int nCount = 0;
    while ((nCount = fread(buffer, 1, BUFFER_SIZE, fp)) > 0)
    {
        send(cSocket, buffer, nCount, 0);
    }
    fclose(fp);

    //文件读取完毕, 断开输出流, 发送FIN包
    shutdown(cSocket, SHUT_WR);
    //阻塞等待客户端返回ACK包
    recv(cSocket, buffer, BUFFER_SIZE, 0);
    return 0;
}

int ServerMenu::recvFile(int cSocket)
{
    char buffer[BUFFER_SIZE] = {0};                      //缓冲区
    int recvLen = recv(cSocket, buffer, BUFFER_SIZE, 0); //接收文件名
    if (*buffer == '\xFF')
    {
        return -1;
    }

    FILE *fp = fopen(buffer, "wb");
    if (fp == NULL)
    {
        char x = 'N';
        send(cSocket, &x, 1, 0);
        std::cout << "Can not create file" << std::endl;
        return -1;
    }
    else
    {
        char x = 'Y';
        send(cSocket, &x, 1, 0);
    }
    sleep(1);                       // 防止粘包
    memset(buffer, 0, BUFFER_SIZE); //清空缓冲
    int nCount = 0;
    while ((nCount = recv(cSocket, buffer, BUFFER_SIZE, 0)) > 0)
    {
        fwrite(buffer, nCount, 1, fp);
    }
    fclose(fp);
    std::cout << "Download success" << std::endl;

    return 0;
}

int ServerMenu::deleteFile(int cSocket)
{
    char buffer[BUFFER_SIZE] = {0}; //缓冲区
    //接收客户端发送的文件名
    int recvLen = recv(cSocket, buffer, BUFFER_SIZE, 0);
    FILE *fp = this->catFile(buffer, cSocket);
    if (fp == NULL)
    {
        return -1;
    }
    fclose(fp);
    char cmd[BUFFER_SIZE + 3] = {0};
    strcpy(cmd, "rm ");
    strcat(cmd, buffer);
    system(cmd);
    return 0;
}