// TCPServer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include<Winsock2.h>
#include<vector>
#include<algorithm>
//#include<string>
using namespace std;

#define MAX_BUFFER_SIZE 256

void err_quit(const char* msg);
void err_display(const char* msg);
DWORD WINAPI ProcessClient(LPVOID arg);
vector<SOCKET> g_Sockets;
HANDLE g_hMutex;

#pragma pack(1)
struct CHAT_LOG
{
    char buf[MAX_BUFFER_SIZE + 1];
    //string chat
};
#pragma pack()

int main()
{
    g_hMutex = CreateMutex(NULL, false, NULL);
    if (NULL == g_hMutex) return -1;
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        CloseHandle(g_hMutex);
        return -1;
    }

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return -1;

    SOCKET listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (INVALID_SOCKET == listen_socket) err_quit("socket");

    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(9000);
    serveraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);    

    if (bind(listen_socket, (SOCKADDR*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR)
    {
        closesocket(listen_socket);
        WSACleanup();
        err_quit("bind");
    }

    if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR)
    {
        closesocket(listen_socket);
        WSACleanup();
        err_quit("listen");
    }

    SOCKADDR_IN clientaddr;
    int addrlen = sizeof(SOCKADDR_IN);
    SOCKET client_socket;
    HANDLE hThread;
    while (1)
    {
        ZeroMemory(&clientaddr, addrlen);
        client_socket = accept(listen_socket, (SOCKADDR*)&clientaddr, &addrlen);
        if (INVALID_SOCKET == client_socket) continue;

        printf("\n[TCP 서버] 클라이언트 접속 : IP 주소=%s, 포트 번호=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_socket, 0, NULL);
        if (NULL == hThread) std::cout << "[오류] 스레드 생성 실패!" << std::endl;
        else CloseHandle(hThread);
    }

    closesocket(listen_socket);
    WSACleanup();

    CloseHandle(g_hMutex);
}

DWORD WINAPI ProcessClient(LPVOID arg)
{
    SOCKET client_sock = (SOCKET)arg;

    WaitForSingleObject(g_hMutex, INFINITE);
    g_Sockets.push_back(client_sock);
    ReleaseMutex(g_hMutex);


    SOCKADDR_IN clientaddr;
    int addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen); // 클라이언트 정보 얻기.

    int retval;
    //char buf[MAX_BUFFER_SIZE + 1];
    char buf[sizeof(CHAT_LOG)];
    while (1)
    {
       
      
        //CHAT_LOG send_packet;      
        //send(sock, (char*)&send_packet, sizeof(CHAT_LOG), 0);




        ZeroMemory(buf, sizeof(buf));
        retval = recv(client_sock, buf, sizeof(buf), 0);
        if (SOCKET_ERROR == retval) break;
        else if (0 == retval) break;

        //buf[retval - 1] = '\0';
        CHAT_LOG* recv_packet = (CHAT_LOG*)buf;
        printf("\n[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), recv_packet->buf);

        //CHAT_LOG send_packet = *recv_packet;
        WaitForSingleObject(g_hMutex, INFINITE);
        for (const auto& sock : g_Sockets)
        {
            retval = send(sock, (char*)recv_packet, sizeof(CHAT_LOG), 0);
            //retval = send(sock, buf, retval, 0);
            if (SOCKET_ERROR == retval) break;
        }
        ReleaseMutex(g_hMutex);
    }

    WaitForSingleObject(g_hMutex, INFINITE);
    auto iter = find(g_Sockets.begin(), g_Sockets.end(), client_sock);
    if(g_Sockets.end() != iter) g_Sockets.erase(iter);
    ReleaseMutex(g_hMutex);

    //for (auto iter = g_Sockets.begin(); iter != g_Sockets.end(); iter++)
    //{
    //    if (*iter == client_sock)
    //    {
    //        g_Sockets.erase(iter);
    //        break;
    //    }
    //}
    closesocket(client_sock);
    printf("\n[TCP 서버] 클라이언트 종료 : IP 주소=%s, 포트 번호=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    return 0;
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





// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
