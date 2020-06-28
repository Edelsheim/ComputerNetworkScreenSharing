#pragma once
#include <afxsock.h>
#include "DrawingQueue.h"

class CClient :
	public CSocket
{
private:
	CAsyncSocket* serverSocket;
	DrawingQueue* queue;
public:
	CClient();
	~CClient();
	void SetListenSocket(CAsyncSocket* socket, DrawingQueue* queue);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
};

