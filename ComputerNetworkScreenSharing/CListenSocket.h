#pragma once
#include <afxsock.h>
#include "DrawingQueue.h"

static LONG PlayerIndex;

class CListenSocket :
	public CAsyncSocket
{
	DrawingQueue* queue;
public:
	CListenSocket(DrawingQueue& queue);
	~CListenSocket();

	CPtrList clientSocketList;
	void OnAccept(int nErrorCode);
	void CloseClientSocket(CSocket* client);
	void BroadCast(void* message, int len);
	void OnClose(int nErrorCode);
	void CloseAllClient();
};