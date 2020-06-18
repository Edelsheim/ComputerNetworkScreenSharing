#include "pch.h"
#include "CListenSocket.h"
#include "CClient.h"
#include "MessageQueue.h"

//
// https://afsdzvcx123.tistory.com/98
//

CListenSocket::CListenSocket() : CAsyncSocket()
{

}

CListenSocket::~CListenSocket()
{
	POSITION pos;
	pos = clientSocketList.GetHeadPosition();
	CClient* client = nullptr;

	while (pos != NULL)
	{
		client = (CClient*)clientSocketList.GetNext(pos);
		if (client != nullptr)
		{
			client->Close();
		}
	}
	clientSocketList.RemoveAll();
}

void CListenSocket::OnAccept(int nErrorCode)
{
	CClient* client = new CClient();
	BOOL check = Accept(*client); // CAsyncSocket.Accept()

	if (check == FALSE)
	{
		delete client;
		MessageQueue::GetInstance()->Push(L"Accept new client fail");
		return;
	}
	else
	{
		MessageQueue::GetInstance()->Push(L"Accept new client");
		client->SetListenSocket(this); // set client's follw this socket
		clientSocketList.AddTail(client); // add to client list
		CAsyncSocket::OnAccept(nErrorCode);
	}
}

void CListenSocket::CloseClientSocket(CSocket* client)
{
	POSITION pos;
	pos = clientSocketList.Find(client);
	if (pos != NULL)
	{
		// close client
		client->ShutDown();
		client->Close();
	}

	// remove to list
	clientSocketList.RemoveAt(pos);
	delete client;
}

#include <string>
void CListenSocket::BroadCast(void* message, int len)
{
	if (clientSocketList.GetCount() == 0)
	{
		return;
	}

	POSITION pos;
	pos = clientSocketList.GetHeadPosition();
	CClient* client = nullptr;

	while (pos != NULL)
	{
		client = (CClient*)clientSocketList.GetNext(pos);
		if (client != nullptr)
		{
			client->Send(message, len);
		}
	}
}