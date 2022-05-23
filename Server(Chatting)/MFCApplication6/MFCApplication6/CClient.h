#pragma once
#include <afxsock.h>

#define MAX_BUFFER_SIZE 256

#pragma pack(1)
struct CHAT_LOG
{
    char buf[MAX_BUFFER_SIZE + 1];
    //string chat
};
#pragma pack()

class CClient :
    public CSocket
{
public:
    virtual void OnClose(int nErrorCode);
    virtual void OnReceive(int nErrorCode);
};

