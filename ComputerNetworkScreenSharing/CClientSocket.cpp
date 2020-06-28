#include "pch.h"
#include "CClientSocket.h"

#include "DrawingQueue.h"
#include <string>

CClientSocket::CClientSocket(DrawingQueue& queue) : CSocket()
{
	isAlive = false;
	this->queue = &queue;
}

CClientSocket::~CClientSocket()
{
}

void CClientSocket::OnClose(int nErrorCode)
{
	isAlive = false;
	ShutDown();
	Close();
	CSocket::OnClose(nErrorCode);
}

void CClientSocket::OnReceive(int nErrorCode)
{
	char data[DATA_SIZE + CLIENT_NAME_SIZE] = { 0, };
	int len = 0;

	CString ip;
	UINT port = 0;

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
		queue->Push(point_data, "client");
	}
	CSocket::OnReceive(nErrorCode);
}

int CClientSocket::Send(const void* lpBuf, int nBufLen, int nFlags)
{
	if (isAlive)
		return CSocket::Send(lpBuf, nBufLen, nFlags);
	else
		return -1;
}
