#pragma once
#include <afxsock.h>
class CClientSocket :
	public CSocket
{
public:
	CClientSocket();
	~CClientSocket();
	bool isAlive;
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
};

