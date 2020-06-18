#pragma once
#include <afxsock.h>
class CClientSocket :
	public CSocket
{
public:
	CClientSocket();
	~CClientSocket();
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
};

