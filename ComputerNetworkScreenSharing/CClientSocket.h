#pragma once
#include <afxsock.h>

#include "DrawingQueue.h"
class CClientSocket :
	public CSocket
{
public:
	CClientSocket(DrawingQueue &queue);
	~CClientSocket();
	bool isAlive;
	DrawingQueue* queue;
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
};

