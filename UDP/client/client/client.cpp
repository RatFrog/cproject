// Test_Console_2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <thread>
#include <cstdio>
#include <string>

#pragma comment(lib,"ws2_32.lib")

using namespace std;

#pragma region 全局变量

SOCKET client;     // 客户端套接字  
sockaddr_in sai_client;   // 存放客户端地址、端口
sockaddr_in sai_server;   // 存放服务端发送的消息

// 发送和接收的信息体
struct umsg {
    int type;     // 协议（1：登录，2：退出，3：发消息）
    char name[64];    // 用户名字
    char text[512];    // 文本
};

#pragma endregion

#pragma region 依赖函数

// 监听服务器消息
void recvMessage()
{
    while (1) {
        umsg msg;
        int len_server = sizeof(sockaddr);
        int len = recvfrom(client, (char*)&msg, sizeof(msg), 0, (sockaddr*)&sai_server, &len_server);

        cout << msg.name << ": " << msg.text << endl;
    }
}

#pragma endregion

int main()
{
    cout << "我是客户端" << endl;

    // 初始化 WSA ，激活 socket
    WSADATA wsaData;
    if (WSAStartup(
        MAKEWORD(2, 2),  // 规定 socket 版本
        &wsaData   // 接收 socket 的更多信息
    )) {
        cout << "WSAStartup failed : " << GetLastError() << endl;
    }

    // 初始化 socket、客户端信息
    client = socket(
        AF_INET,  // IPV4
        SOCK_DGRAM,  // UDP
        0    // 不指定协议
    );
    sai_client.sin_family = AF_INET;         // IPV4
    inet_pton(AF_INET, "192.168.100.215", &sai_client.sin_addr);   // 服务器 IP地址
    sai_client.sin_port = htons(8090);         // 端口

    // 输入用户名
    string name;
    getline(cin, name);

    // 发送登录消息
    umsg msg;
    msg.type = 1;
    strncpy_s(msg.name, sizeof(msg.name), name.c_str(), 64);
    strncpy_s(msg.text, sizeof(msg.text), "", 512);
    sendto(
        client,       // 本地套接字
        (char*)&msg,      // 发送的消息
        sizeof(msg),
        0,         // 不修改函数调用行为
        (sockaddr*)&sai_client,  // 消息目标
        sizeof(sai_client)
    );

    // 接收服务器消息
    HANDLE h_recvMes = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)recvMessage, 0, 0, 0);
    if (!h_recvMes) { cout << "CreateThread failed :" << GetLastError() << endl; }

    // 发送消息
    while (1) {
        string content;
        getline(cin, content);

        // 如果是退出消息
        if (content == "quit") {
            msg.type = 2;
            sendto(client, (char*)&msg, sizeof msg, 0, (struct sockaddr*)&sai_client, sizeof(sai_client));
            closesocket(client);
            WSACleanup();
            return 0;
        }

        // 如果是会话消息
        msg.type = 3;
        strncpy_s(msg.text, sizeof(msg.text), content.c_str(), 512);
        sendto(
            client,       // 本地套接字
            (char*)&msg,      // 要发送的消息
            sizeof(msg),
            0,         // 不修改函数调用行为
            (sockaddr*)&sai_client,   // 发送目标
            sizeof(sai_client)
        );
    }
    getchar();
    return 0;
}