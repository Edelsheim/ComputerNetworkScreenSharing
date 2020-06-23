#pragma once
#include <afxsock.h>

static LONG PlayerIndex;

class CListenSocket :
	public CAsyncSocket
{
public:
	CListenSocket();
	~CListenSocket();

	CPtrList clientSocketList;
	void OnAccept(int nErrorCode);
	void CloseClientSocket(CSocket* client);
	void BroadCast(void* message, int len);
	virtual void OnReceive(int nErrorCode);
};