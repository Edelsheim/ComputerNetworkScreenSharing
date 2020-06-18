#include "pch.h"
#include "CClientSocket.h"

#include "DrawingQueue.h"
#include "MessageQueue.h"
#include <string>

//
// https://afsdzvcx123.tistory.com/99
//

CClientSocket::CClientSocket() : CSocket()
{
}

CClientSocket::~CClientSocket()
{
}

void CClientSocket::OnConnect(int nErrorCode)
{
	MessageQueue::GetInstance()->Push(std::to_wstring(nErrorCode));
	CSocket::OnConnect(nErrorCode);
}


void CClientSocket::OnClose(int nErrorCode)
{
	ShutDown();
	Close();
	CSocket::OnClose(nErrorCode);
}


void CClientSocket::OnReceive(int nErrorCode)
{
	CString peerIP = _T("");
	UINT peerPort = 0;

	GetPeerName(peerIP, peerPort);
	CPoint point;
	int len = 0;

	if ((len = Receive(&point, sizeof(point))) > 0)
	{
		MessageQueue::GetInstance()->Push(L"receive size : " + std::to_wstring(len));
		DrawingQueue::GetQueue()->Push(point);
	}
	CSocket::OnReceive(nErrorCode);
}


void CClientSocket::OnSend(int nErrorCode)
{
	CSocket::OnSend(nErrorCode);
}
