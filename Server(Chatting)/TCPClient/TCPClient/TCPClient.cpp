// TCPClient.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include<Winsock2.h>
using namespace std;

void err_quit(const char* msg);
void err_display(const char* msg);
#define MAX_BUFFER_SIZE 256

#pragma pack(1)
struct CHAT_LOG
{
    char buf[MAX_BUFFER_SIZE + 1];
    //string chat
};
#pragma pack()

int main()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return -1;
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (INVALID_SOCKET == sock) return -1;

    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(9000);
    serveraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    if (SOCKET_ERROR == connect(sock, (SOCKADDR*)&serveraddr,
        sizeof(serveraddr)))err_quit(" connect ");

    int len, retval;
    char buf2[sizeof(CHAT_LOG)];

    char buf[MAX_BUFFER_SIZE + 1];
    while (1)
    {
        ZeroMemory(buf, sizeof(buf));
        printf("\n[보낼 데이터]");
        if (fgets(buf, MAX_BUFFER_SIZE, stdin) == NULL) break;
        len = strlen(buf);
        if (buf[len - 1] == '\n') buf[len - 1] = '\0';
        if (strlen(buf) == 0) break;

        CHAT_LOG send_packet;
        //send_packet.buf = buf;
        sprintf_s(send_packet.buf, sizeof(buf), buf);
        //retval = send(sock, (char*)b, sizeof(CHAT_LOG), 0);

        retval = send(sock, (char*)&send_packet, sizeof(CHAT_LOG), 0);
        //retval = send(sock, buf, sizeof(buf), 0);
        if (SOCKET_ERROR == retval) break;
        printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);


        ZeroMemory(buf2, sizeof(buf2));
        retval = recv(sock, buf2, sizeof(buf2), 0);

        if (SOCKET_ERROR == retval) break;
        else if (0 == retval) break;

        CHAT_LOG* recv_packet = (CHAT_LOG*)buf2;
        //buf2[retval - 1] = '\0';
        printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
        printf("[받은 데이터]%s\n", recv_packet->buf);
       
    }
    closesocket(sock);

    WSACleanup();
}

void err_quit(const char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);

    MessageBoxA(NULL, (LPCSTR)lpMsgBuf, msg, MB_ICONERROR);

    LocalFree(lpMsgBuf);

    exit(-1);
}

void err_display(const char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);


    cout << msg << (LPCSTR)lpMsgBuf << endl;
    LocalFree(lpMsgBuf);
}