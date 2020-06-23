#include "pch.h"
#include "CListenSocket.h"
#include "CClient.h"
#include "MessageQueue.h"
#include "DrawingQueue.h"
#include "ClientMap.h"

CListenSocket::CListenSocket() : CAsyncSocket()
{
	PlayerIndex = 1;
	
	// insert to server
	// server:000000000000
	CString key;
	CString cstring_format = _T("%0");
	cstring_format.Append(std::to_wstring(CLIENT_NAME_SIZE - 1).c_str());
	cstring_format.Append(_T("ld"));
	key.Format(cstring_format, 0);
	std::string value = CT2CA(key);
	ClientMap::GetClientMap()->Insert(L"server", value);
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
		MessageQueue::GetInstance()->Push(L"Accept new client index : " + std::to_wstring(PlayerIndex));
		//
		// On accept new client, add to client map(client list)
		//

		// get peer info
		CString peerIP = _T("");
		UINT peerPort = 0;
		client->GetPeerName(peerIP, peerPort);
		
		// make key
		std::wstring key = peerIP.operator LPCWSTR();
		key.append(L":");
		key.append(std::to_wstring(peerPort));
		
		// make value
		CString value_cstr;
		CString value_format = _T("%0");
		value_format.Append(std::to_wstring(CLIENT_NAME_SIZE - 1).c_str());
		value_format.Append(_T("ld"));
		value_cstr.Format(value_format, PlayerIndex);
		PlayerIndex++;

		// CString to string
		std::string value_str = CT2CA(value_cstr);
		
		// insert to client map(client list)
		ClientMap::GetClientMap()->Insert(key, value_str);
		MessageQueue::GetInstance()->Push(L"Accept new client info : " + key);
		
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

void CListenSocket::OnReceive(int nErrorCode)
{
	CAsyncSocket::OnReceive(nErrorCode);
}
