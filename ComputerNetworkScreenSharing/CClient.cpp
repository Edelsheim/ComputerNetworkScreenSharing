#include "pch.h"
#include "CClient.h"
#include "CListenSocket.h"

#include "MessageQueue.h"

CClient::CClient()
{
	serverSocket = nullptr;
}

CClient::~CClient()
{

}

void CClient::SetListenSocket(CAsyncSocket* socket)
{
	serverSocket = socket;
}

void CClient::OnClose(int nErrorCode)
{
	CListenSocket* serverSocket = (CListenSocket*)this->serverSocket;
	serverSocket->CloseClientSocket(this);
	CSocket::OnClose(nErrorCode);
}

void CClient::OnReceive(int nErrorCode)
{
	CString peerIP = _T("");
	UINT peerPort = 0;
	
	GetPeerName(peerIP, peerPort);

	std::wstring message = peerIP.operator LPCWSTR();
	message.append(L":");
	message.append(std::to_wstring(peerPort));
	StaticQueue::GetMessageQueue()->Push(L"Client on receive : " + message);

	char data[DATA_SIZE] = { 0, };
	int len = 0;

	if ((len = Receive(data, sizeof(char) * DATA_SIZE)) > 0)
	{
		char type = data[0];
		char x = data[1];
		LONG point_x = (data[2] - '0') * 1000;
		point_x += (data[3] - '0') * 100;
		point_x += (data[4] - '0') * 10;
		point_x += data[5] - '0';

		char y = data[6];
		LONG point_y = (data[7] - '0') * 1000;
		point_y += (data[8] - '0') * 100;
		point_y += (data[9] - '0') * 10;
		point_y += data[10] - '0';

		PointData point_data;
		point_data.type = type;
		point_data.x = point_x;
		point_data.y = point_y;

		StaticQueue::GetReceiveQueue()->Push(point_data);
	}

	CSocket::OnReceive(nErrorCode);
}


void CClient::OnSend(int nErrorCode)
{

	CSocket::OnSend(nErrorCode);
}
