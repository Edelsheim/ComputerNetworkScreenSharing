#include "pch.h"
#include "CClientSocket.h"

#include "DrawingQueue.h"
#include "MessageQueue.h"
#include <string>

CClientSocket::CClientSocket() : CSocket()
{
}

CClientSocket::~CClientSocket()
{
}

void CClientSocket::OnConnect(int nErrorCode)
{
	CSocket::OnConnect(nErrorCode);
}


void CClientSocket::OnClose(int nErrorCode)
{
	ShutDown();
	Close();
	CSocket::OnClose(nErrorCode);
}


void CClientSocket::OnReceive(int nErrorCode)
{
	/*
	CString peerIP = _T("");
	UINT peerPort = 0;

	GetPeerName(peerIP, peerPort);
	*/

	char data[DATA_SIZE + CLIENT_NAME_SIZE] = { 0, };
	int len = 0;

	if ((len = Receive(data, sizeof(char) * (DATA_SIZE + CLIENT_NAME_SIZE))) > 0)
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
		for (int i = 0; i != CLIENT_NAME_SIZE; i++)
			point_data.id[i] = data[11 + i];
		point_data.x = point_x;
		point_data.y = point_y;
		DrawingQueue::GetReceiveQueue()->Push(point_data);
	}
	CSocket::OnReceive(nErrorCode);
}


void CClientSocket::OnSend(int nErrorCode)
{
	CSocket::OnSend(nErrorCode);
}


int CClientSocket::Send(const void* lpBuf, int nBufLen, int nFlags)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return CSocket::Send(lpBuf, nBufLen, nFlags);
}
