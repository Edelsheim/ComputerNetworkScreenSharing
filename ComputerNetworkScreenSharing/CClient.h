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
};

