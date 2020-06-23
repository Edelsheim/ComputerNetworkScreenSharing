#pragma once
#include <afxsock.h>
class CClient :
	public CSocket
{
private:
	CAsyncSocket* serverSocket;

public:
	CClient();
	~CClient();
	void SetListenSocket(CAsyncSocket* socket);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
};

