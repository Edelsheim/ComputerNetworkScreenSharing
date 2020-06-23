#include "pch.h"
#include "CClient.h"
#include "CListenSocket.h"

#include "MessageQueue.h"
#include "DrawingQueue.h"
#include "ClientMap.h"

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

	std::wstring peer_info = peerIP.operator LPCWSTR();
	peer_info.append(L":");
	peer_info.append(std::to_wstring(peerPort));

	//MessageQueue::GetInstance()->Push(L"Client on receive " + peer_info);

	// get client value (player index)
	std::string client_value = ClientMap::GetClientMap()->GetValue(peer_info);
	if (client_value.compare("") == 0)
	{
		// unknown client
		CString unknownClient;
		CString value_format = _T("%0");
		value_format.Append(std::to_wstring(CLIENT_NAME_SIZE - 1).c_str());
		value_format.Append(_T("ld"));
		unknownClient.Format(value_format, PlayerIndex);
		PlayerIndex++;

		// CString to string
		client_value = CT2CA(unknownClient);

		ClientMap::GetClientMap()->Insert(peer_info, client_value);
	}

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

		int i = 0;
		for (i = 0; i != CLIENT_NAME_SIZE - 1; i++)
			point_data.id[i] = client_value.c_str()[i];
		point_data.id[i] = '\0';
		point_data.x = point_x;
		point_data.y = point_y;

		DrawingQueue::GetReceiveQueue()->Push(point_data);
	}
	CSocket::OnReceive(nErrorCode);
}


void CClient::OnSend(int nErrorCode)
{
	CSocket::OnSend(nErrorCode);
}


int CClient::Send(const void* lpBuf, int nBufLen, int nFlags)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return CSocket::Send(lpBuf, nBufLen, nFlags);
}
