// DrawingView.cpp: 구현 파일
//

#include "pch.h"
#include "ComputerNetworkScreenSharing.h"
#include "DrawingView.h"

#include "MessageQueue.h"
#include "DrawingQueue.h"
#include "ClientMap.h"
#include "PointDataList.h"

// DrawingView

IMPLEMENT_DYNCREATE(DrawingView, CFormView)

DrawingView::DrawingView()
	: CFormView(IDD_Draw)
{
	this->point.x = -1;
	this->point.y = -1;
	this->Name = _T("");

	threadReceiveQueue = nullptr;
	threadReceiveStatus = ThreadStatusNullptr;

	threadSendQueue = nullptr;
	threadSendStatus = ThreadStatusNullptr;

	server = nullptr;
	serverRunning = false;

	client = nullptr;
	clientRunning = false;
}

DrawingView::~DrawingView()
{

}

void DrawingView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(DrawingView, CFormView)
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_DRAWPOP, &DrawingView::OnDrawpop)
	ON_MESSAGE(WM_SENDDRAW, &DrawingView::OnSenddraw)
END_MESSAGE_MAP()


// DrawingView 진단

#ifdef _DEBUG
void DrawingView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void DrawingView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// DrawingView 메시지 처리기


BOOL DrawingView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void DrawingView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	this->point.x = -1;
	this->point.y = -1;

	threadReceiveQueue = AfxBeginThread(threadReceiveQeueuRunner, this);
	threadSendQueue = AfxBeginThread(threadSendQueueRunner, this);
}

BOOL DrawingView::DestroyWindow()
{
	threadReceiveStatus = ThreadStatusClose;
	threadSendStatus = ThreadStatusClose;

	HANDLE threades[2] = { threadReceiveQueue , threadSendQueue};
	WaitForMultipleObjects(2, threades, TRUE, 2000);

	/*
	if (threadReceiveQueue != nullptr)
	{
		threadReceiveQueue = nullptr;
	}

	if (threadSendQueue != nullptr)
	{
		threadSendQueue = nullptr;
	}
	*/

	if (server != nullptr)
	{
		server->CloseAllClient();
		server->ShutDown();
		server->Close();
		delete server;
	}
	serverRunning = false;
	server = nullptr;

	if (client != nullptr)
	{
		client->ShutDown();
		client->Close();
		delete client;
		client = nullptr;
	}
	clientRunning = false;
	client = nullptr;

	receivePointes.clear();

	PointDataList::GetQueue()->ClearWithKey(this->Name);

	return CFormView::DestroyWindow();
}

HBRUSH DrawingView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	HBRUSH hbr = CreateSolidBrush(RGB(255, 255, 255));
	return hbr;
}

void DrawingView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	std::string id = "";

	if (server != nullptr)
	{
		dc.MoveTo(point.x, point.y);
		this->point.x = point.x;
		this->point.y = point.y;
		
		// push to type is 'click'
		id = ClientMap::GetClientMap()->GetValue(L"server");
		DrawingQueue::GetSendQueue()->Push(point, CLICK_DATA, id, "server");
		PointDataList::GetQueue()->Insert(this->Name, point, CLICK_DATA, id);
	}
	else if (client != nullptr)
	{
		DrawingQueue::GetSendQueue()->Push(point, CLICK_DATA, "", "client");
	}

	CFormView::OnLButtonDown(nFlags, point);
}

void DrawingView::OnMouseMove(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	CPen pen;

	pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	dc.SelectObject(&pen);

	// check  clicked mouse left button
	if ((nFlags && MK_LBUTTON) == MK_LBUTTON)
	{
		RECT my_rect;
		GetClientRect(&my_rect);

		// push to type is 'move'
		if (server != nullptr)
		{
			std::string id = ClientMap::GetClientMap()->GetValue(L"server");
			DrawingQueue::GetSendQueue()->Push(point, MOVE_DATA, id, "server");

			if (point.x <= my_rect.left + 3)
			{
				dc.MoveTo(my_rect.left, point.y);
				dc.LineTo(point.x, point.y);
				this->point.x = point.x;
				this->point.y = point.y;
			}
			else if (point.x >= my_rect.right - 3)
			{
				dc.MoveTo(my_rect.right, point.y);
				dc.LineTo(point.x, point.y);
				this->point.x = point.x;
				this->point.y = point.y;
			}
			else if (point.y <= my_rect.top + 3)
			{
				dc.MoveTo(point.x, my_rect.top);
				dc.LineTo(point.x, point.y);
				this->point.x = point.x;
				this->point.y = point.y;
			}
			else if (point.y >= my_rect.bottom - 3)
			{
				dc.MoveTo(point.x, my_rect.bottom);
				dc.LineTo(point.x, point.y);
				this->point.x = point.x;
				this->point.y = point.y;
			}
			else
			{
				dc.MoveTo(this->point.x, this->point.y);
				dc.LineTo(point.x, point.y);
				this->point.x = point.x;
				this->point.y = point.y;
			}

			PointDataList::GetQueue()->Insert(this->Name, point, MOVE_DATA, id);
		}
		else if (client != nullptr)
		{
			DrawingQueue::GetSendQueue()->Push(point, MOVE_DATA, "", "client");
		}
	}
	pen.DeleteObject();
	CFormView::OnMouseMove(nFlags, point);
}

afx_msg LRESULT DrawingView::OnDrawpop(WPARAM wParam, LPARAM lParam)
{
	PointData point;
	if (server != nullptr)
		point = DrawingQueue::GetReceiveQueue()->Pop("server");
	else if (client != nullptr)
		point = DrawingQueue::GetReceiveQueue()->Pop("client");
	else
		return 1;

	if (point.x <= -1 || point.y <= -1)
		return 1;

	CClientDC dc(this);
	CPen pen;

	pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	dc.SelectObject(&pen);

	// get this view rect
	RECT my_rect;
	GetClientRect(&my_rect);

	// get client id
	std::string client_id = std::string(point.id);

	// new client
	if (this->receivePointes.find(client_id) == this->receivePointes.end())
	{
		CPoint data;
		data.x = point.x;
		data.y = point.y;
		this->receivePointes.insert(std::make_pair(client_id, data));
	}

	if (point.type == 'c') // check type is 'click'
	{
		dc.MoveTo(point.x, point.y);
		this->receivePointes.at(client_id).x = point.x;
		this->receivePointes.at(client_id).y = point.y;
	}
	else if (point.x <= my_rect.left + 2)
	{
		dc.MoveTo(my_rect.left, point.y);
		dc.LineTo(point.x, point.y);
		this->receivePointes.at(client_id).x = point.x;
		this->receivePointes.at(client_id).y = point.y;
	}
	else if (point.x >= my_rect.right - 3)
	{
		dc.MoveTo(my_rect.right, point.y);
		dc.LineTo(point.x, point.y);
		this->receivePointes.at(client_id).x = point.x;
		this->receivePointes.at(client_id).y = point.y;
	}
	else if (point.y <= my_rect.top + 3)
	{
		dc.MoveTo(point.x, my_rect.top);
		dc.LineTo(point.x, point.y);
		this->receivePointes.at(client_id).x = point.x;
		this->receivePointes.at(client_id).y = point.y;
	}
	else if (point.y >= my_rect.bottom - 3)
	{
		dc.MoveTo(point.x, my_rect.bottom);
		dc.LineTo(point.x, point.y);
		this->receivePointes.at(client_id).x = point.x;
		this->receivePointes.at(client_id).y = point.y;
	}
	else
	{
		LONG x = this->receivePointes.at(client_id).x;
		LONG y = this->receivePointes.at(client_id).y;

		dc.MoveTo(x, y);
		dc.LineTo(point.x, point.y);
		this->receivePointes.at(client_id).x = point.x;
		this->receivePointes.at(client_id).y = point.y;
	}
	pen.DeleteObject();

	if (server != nullptr)
	{
		DrawingQueue::GetSendQueue()->Push(point, "server");
	}

	PointDataList::GetQueue()->Insert(this->Name, point);
	return 0;
}

afx_msg LRESULT DrawingView::OnSenddraw(WPARAM wParam, LPARAM lParam)
{
	if (server != nullptr && client != nullptr)
		return 1;

	PointData point;
	if (server != nullptr)
		point = DrawingQueue::GetSendQueue()->Pop("server");
	else if (client != nullptr)
		point = DrawingQueue::GetSendQueue()->Pop("client");
	else
		return 1;

	if (point.x == -1 || point.y == -1)
	{
		return 1;
	}

	char send_message[DATA_SIZE + CLIENT_NAME_SIZE];
	point.GetData(send_message);
	if (server != nullptr)
	{
		server->BroadCast(send_message, DATA_SIZE + CLIENT_NAME_SIZE);
	}
	else if (client != nullptr)
	{
		char message[DATA_SIZE] = { 0, };
		int i = 0;
		for (i = 0; i != DATA_SIZE; i++)
		{
			message[i] = send_message[i];
		}
		client->Send(message, DATA_SIZE);
	}
	return 0;
}


UINT DrawingView::threadReceiveQeueuRunner(LPVOID param)
{
	DrawingView* dv = (DrawingView*)param;
	//MessageQueue::GetInstance()->Push(L"Thread Receive Queue Runner");
	dv->threadReceiveStatus = ThreadStatusRun;

	while (1)
	{
		if (dv->threadReceiveStatus == ThreadStatusClose)
			break;

		PostMessageA(dv->m_hWnd, WM_DRAWPOP, NULL, NULL);
		Sleep(1);
	}
	return 0;
}

UINT DrawingView::threadSendQueueRunner(LPVOID param)
{
	DrawingView* dv = (DrawingView*)param;
	//MessageQueue::GetInstance()->Push(L"Thread Send Queue Runner");
	dv->threadSendStatus = ThreadStatusRun;

	while (1)
	{
		if (dv->threadSendStatus == ThreadStatusClose)
			break;

		PostMessageA(dv->m_hWnd, WM_SENDDRAW, NULL, NULL);
		Sleep(1);
	}
	return 0;
}

bool DrawingView::ClientRun(CString ip, UINT port)
{
	if (server != nullptr)
	{
		clientRunning = false;
		return false;
	}

	client = new CClientSocket();
	client->Create();

	BOOL check = client->Connect(ip, port);
	if (check)
	{
		client->isAlive = true;
		clientRunning = true;
		return true;
	}
	else
	{
		clientRunning = false;
		delete client;
		client = nullptr;
		return false;
	}
}

bool DrawingView::ClientClose()
{
	if (client != nullptr)
	{
		client->Close();
		delete client;
	}
	client = nullptr;
	clientRunning = false;
	return true;
}

bool DrawingView::ServerRun(UINT port)
{
	if (client != nullptr && clientRunning)
	{
		serverRunning = false;
		return false;
	}

	if (server == nullptr)
	{
		server = new CListenSocket;
		if (server->Create(port, SOCK_STREAM))
		{
			if (server->Listen(5))
			{
				serverRunning = true;
				return true;
			}
			else
			{
				server->Close();
				delete server;
				server = nullptr;
				serverRunning = false;
				return false;
			}
		}
		else
		{
			delete server;
			server = nullptr;
			serverRunning = false;
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool DrawingView::ServerClose()
{
	if (server != nullptr)
	{
		server->CloseAllClient();
		server->ShutDown();
		server->Close();
		delete server;
	}

	server = nullptr;
	serverRunning = false;
	return true;
}

void DrawingView::DrawingViewStart()
{
	threadReceiveStatus = ThreadStatusRun;
	threadSendStatus = ThreadStatusRun;
}

void DrawingView::DrawingViewPause()
{
	threadReceiveStatus = ThreadStatusPause;
	threadSendStatus = ThreadStatusPause;
}

void DrawingView::DrawingPoint()
{
	CClientDC dc(this);
	CPen pen;

	pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	dc.SelectObject(&pen);


	// get this view rect
	RECT my_rect;
	GetClientRect(&my_rect);

	const PointDataVector dataVector = PointDataList::GetQueue()->GetData(this->Name);
	PointDataVector::const_iterator iterator = dataVector.cbegin();

	while (1)
	{
		if (iterator == dataVector.cend())
			break;

		PointData point = (*iterator);

		// get client id
		std::string client_id = std::string(point.id);

		// not find client_id, it is new client
		if (this->receivePointes.find(client_id) == this->receivePointes.end())
		{
			CPoint data;
			data.x = point.x;
			data.y = point.y;
			this->receivePointes.insert(std::make_pair(client_id, data));
		}

		if (point.type == 'c') // check type is 'click'
		{
			dc.MoveTo(point.x, point.y);
			this->receivePointes.at(client_id).x = point.x;
			this->receivePointes.at(client_id).y = point.y;
		}
		else if (point.x <= my_rect.left + 2)
		{
			dc.MoveTo(my_rect.left, point.y);
			dc.LineTo(point.x, point.y);
			this->receivePointes.at(client_id).x = point.x;
			this->receivePointes.at(client_id).y = point.y;
		}
		else if (point.x >= my_rect.right - 3)
		{
			dc.MoveTo(my_rect.right, point.y);
			dc.LineTo(point.x, point.y);
			this->receivePointes.at(client_id).x = point.x;
			this->receivePointes.at(client_id).y = point.y;
		}
		else if (point.y <= my_rect.top + 3)
		{
			dc.MoveTo(point.x, my_rect.top);
			dc.LineTo(point.x, point.y);
			this->receivePointes.at(client_id).x = point.x;
			this->receivePointes.at(client_id).y = point.y;
		}
		else if (point.y >= my_rect.bottom - 3)
		{
			dc.MoveTo(point.x, my_rect.bottom);
			dc.LineTo(point.x, point.y);
			this->receivePointes.at(client_id).x = point.x;
			this->receivePointes.at(client_id).y = point.y;
		}
		else
		{
			LONG x = this->receivePointes.at(client_id).x;
			LONG y = this->receivePointes.at(client_id).y;

			dc.MoveTo(x, y);
			dc.LineTo(point.x, point.y);
			this->receivePointes.at(client_id).x = point.x;
			this->receivePointes.at(client_id).y = point.y;
		}
		iterator++;
	}
	pen.DeleteObject();
}