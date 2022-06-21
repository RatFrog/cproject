// Test_Console.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <thread>
#include <cstdio>

using namespace std;

#pragma region 全局变量

SOCKET server;     // 服务端套接字
sockaddr_in sai_server;   // 服务端信息（ip、端口）

// 消息格式
struct umsg {
    int type;    // 协议(1：加入 2：退出 3：发消息)
    char name[64];   // 用户名字
    char text[512];   // 文本信息
};

// 客户端链表
typedef struct ucnode {
    sockaddr_in addr;  // 客户端的地址和端口号
    umsg msg;    // 客户端传来的消息
    ucnode* next;
} *ucnode_t;

#pragma endregion


#pragma region 依赖函数

// 链表插入数据
ucnode* insertNode(ucnode* head, sockaddr_in addr, umsg msg) {
    ucnode* newNode = new ucnode();
    newNode->addr = addr;
    newNode->msg = msg;
    ucnode* p = head;
    if (p == nullptr) {
        head = newNode;
    }
    else {
        while (p->next != nullptr) {
            p = p->next;
        }
        p->next = newNode;
    }
    return head;
}

// 链表删除数据
ucnode* deleteNode(ucnode* head, umsg msg) {
    ucnode* p = head;
    if (p == nullptr) {
        return head;
    }
    if (strcmp(p->msg.name, msg.name) == 0) {
        head = p->next;
        delete p;
        return head;
    }
    while (p->next != nullptr && strcmp(p->next->msg.name, msg.name) != 0) {
        p = p->next;
    }
    if (p->next == nullptr) {
        return head;
    }
    ucnode* deleteNode = p->next;
    p->next = deleteNode->next;
    delete deleteNode;
    return head;
}

#pragma endregion

int main()
{
    cout << "我是服务端" << endl;

    // 初始化 WSA ，激活 socket
    WSADATA wsaData;
    if (WSAStartup(
        MAKEWORD(2, 2),   // 规定 socket 版本为 2.2
        &wsaData    // 接收关于套接字的更多信息
    )) {
        cout << "WSAStartup failed : " << GetLastError() << endl;
    }

    // 初始化 socket、服务器信息
    server = socket(
        AF_INET,   // IPV4
        SOCK_DGRAM,  // UDP
        0    // 不指定协议
    );
    sai_server.sin_addr.S_un.S_addr = 0; // IP地址
    sai_server.sin_family = AF_INET;  // IPV4
    sai_server.sin_port = htons(8090);  // 传输协议端口

    // 本地地址关联套接字
    if (bind(server,      // 要与本地地址绑定的套接字
        (sockaddr*)&sai_server,  // 用来接收客户端消息的 sockaddr_in 结构体指针
        sizeof(sai_server)
    )) {
        cout << "bind failed : " << GetLastError() << endl;
        WSACleanup();
    }

    // 初始化客户端链表
    ucnode* listHead = new ucnode();
    listHead->next = nullptr;
    ucnode* lp = listHead;

    // 监听消息
    while (1) {
        // 接收来自客户端的消息
        umsg msg;
        int len_client = sizeof(sockaddr);
        recvfrom(
            server,      // 本地套接字
            (char*)&msg,     // 存放接收到的消息
            sizeof(msg),
            0,        // 不修改函数调用行为
            (sockaddr*)&sai_server,  // 接收客户端的IP、端口
            &len_client     // 接收消息的长度，必须初始化，否则默认为0 收不到消息    
        );

        // sin_addr 转 char[]（char[] 转 sin_addr 使用 inet_top）
        char arr_ip[20];
        inet_ntop(AF_INET, &sai_server.sin_addr, arr_ip, 16);

        // 处理消息（1：用户登录，2：用户退出，3：普通会话）
        switch (msg.type) {
        case 1:
            insertNode(listHead, sai_server, msg);
            cout << "[" << arr_ip << ":" << ntohs(sai_server.sin_port) << "] " << msg.name << ":" << "---登录---" << endl;
            break;
        case 2:
            deleteNode(listHead, msg);
            cout << "[" << arr_ip << ":" << ntohs(sai_server.sin_port) << "] " << msg.name << ":" << "---退出---" << endl;
            break;
        case 3:
            cout << "[" << arr_ip << ":" << ntohs(sai_server.sin_port) << "] " << msg.name << ":" << msg.text << endl;
            // 更新 msg.text
            lp = listHead;
            while (lp) {
                if (strcmp(lp->msg.name, msg.name) == 0) {
                    strncpy(lp->msg.text, msg.text, sizeof(msg.text));
                    lp->msg.type = msg.type;
                    break;
                }
                lp = lp->next;
            }
            // 向其他客户端广播（除自己之外）
            lp = listHead;
            while (lp) {
                if (strcmp(lp->msg.name, "") != 0 && strcmp(lp->msg.name, msg.name) != 0) {
                    sendto(
                        server,      // 本地套接字
                        (char*)&msg,     // 消息结构体
                        sizeof(msg),
                        0,        // 不修改函数调用行为
                        (sockaddr*)&lp->addr,  // 目标客户端地址
                        sizeof(lp->addr)
                    );
                }
                lp = lp->next;
            }
            break;
        }
    }

    // 禁用 socket
    WSACleanup();

    getchar();
    return 0;
}