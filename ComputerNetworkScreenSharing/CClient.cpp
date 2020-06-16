#include "pch.h"
#include "CClient.h"
#include "CListenSocket.h"

//
// https://afsdzvcx123.tistory.com/98
//
CClient::CClient()
{
	serverSocket = nullptr;
}

CClient::~CClient()
{
}

void CClient::SetListenSocket(CAsyncSocket* socket)
{
	serverSocket = socket;
}

void CClient::OnClose(int nErrorCode)
{
	CListenSocket* serverSocket = (CListenSocket*)this->serverSocket;
	serverSocket->CloseClientSocket(this);
	CSocket::OnClose(nErrorCode);
}

void CClient::OnReceive(int nErrorCode)
{
	CString peerIP = _T("");
	UINT peerPort = 0;
	
	GetPeerName(peerIP, peerPort);

	// receive

	//
	CSocket::OnReceive(nErrorCode);
}
