#include "pch.h"
#include "CClient.h"
#include "MFCApplication6Dlg.h"


void CClient::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	ShutDown();
	Close();
	CSocket::OnClose(nErrorCode);
	AfxMessageBox(_T("ERROR : Disconnected From Server!!"));
}



void CClient::OnReceive(int nErrorCode)
{
	char buf2[sizeof(CHAT_LOG)];
	//char buf[MAX_BUFFER_SIZE + 1];
	ZeroMemory(buf2, sizeof(buf2));
	if (Receive(buf2, sizeof(buf2)) > 0)
	{
		CHAT_LOG* recv_packet = (CHAT_LOG*)buf2;
		CMFCApplication6Dlg* pMain = (CMFCApplication6Dlg*)AfxGetMainWnd();
		CString str(recv_packet->buf);
		pMain->m_List.AddString(str);
	}
	CSocket::OnReceive(nErrorCode);
}
