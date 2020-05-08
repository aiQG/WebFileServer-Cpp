#include "menu.h"


menu::menu(){
    //初始化请求
    this->sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&this->serv_addr, 0, sizeof(this->serv_addr));           //每个字节都用0填充
    this->serv_addr.sin_family = AF_INET;                           //使用IPv4地址
    this->serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");       //具体的IP地址
    this->serv_addr.sin_port = htons(1234);                         //端口
    connect(this->sock, (struct sockaddr *)&this->serv_addr, sizeof(this->serv_addr));

    this->selectedFunction = 0;
}

menu::~menu(){
    close(this->sock);
}

void menu::showMenu(){
    std::cout << "功能选择:\n"
            << "1. 上传文件\n"
            << "2. 下载文件\n"
            << "3. 查找文件\n"
            << "4. 删除文件\n"
            << "选择: ";
    std::cin >> this->selectedFunction;
    std::cin.clear();
    send(this->sock, &this->selectedFunction, 4, 0);
    return;
}


bool menu::catFile(char* filename){
    send(this->sock, filename, strlen(filename), 0);

    char buffer[BUFFER_SIZE] = {0};
    recv(sock, buffer, 1, 0);
    if(*buffer == 'N'){
        return false;
    } else {
        return true;
    }
}

int menu::download(char* filename){
    if(!catFile(filename)) {
        std::cout << "Server does not exist this file" 
                << std::endl;
        return -1;
    }

    //创建文件
    FILE *fp = fopen(filename, "wb");                   //二进制写(创建)
    if (fp == NULL)
    {
        printf("create file fail\n");
        exit(0);
    }
    sleep(1);                                           //防止粘包
    //循环接收数据, 直到传输完毕
    char buffer[BUFFER_SIZE] = {0};
    memset(buffer, 0, BUFFER_SIZE);                     //清空缓冲
    int nCount = 0;
    while ((nCount = recv(sock, buffer, BUFFER_SIZE, 0)) > 0)
    {
        fwrite(buffer, nCount, 1, fp);
    }
    fclose(fp);
    std::cout << "Download success" << std::endl;

    return 0;
}