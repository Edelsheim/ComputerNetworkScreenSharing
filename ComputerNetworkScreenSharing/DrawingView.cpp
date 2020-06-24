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
	isClient = false;
	serverRunning = false;
	clientRunning = false;
	threadReceiveQueue = nullptr;
	threadSendQueue = nullptr;
	server = nullptr;
	client = nullptr;
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
	HANDLE threades[2] = { threadReceiveQueue , threadSendQueue};
	WaitForMultipleObjects(2, threades, TRUE, 1000);

	if (threadReceiveQueue != nullptr)
	{
		//threadReceiveQueue->ExitInstance();
		threadReceiveQueue = nullptr;
	}

	if (threadSendQueue != nullptr)
	{
		threadSendQueue = nullptr;
	}

	if (server != nullptr)
	{
		server->Close();
		delete server;
		server = nullptr;
	}
	if (client != nullptr)
	{
		client->Close();
		delete client;
		client = nullptr;
	}

	receivePointes.clear();

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

	if (server != nullptr && serverRunning)
	{
		dc.MoveTo(point.x, point.y);
		this->point.x = point.x;
		this->point.y = point.y;
	}

	// push to type is 'click'

	std::string id;
	if (server != nullptr && serverRunning)
	{
		id = ClientMap::GetClientMap()->GetValue(L"server");
		PointDataList::GetQueue()->Insert(this->Name, point, CLICK_DATA, id);
	}
	else
		id = "";

	DrawingQueue::GetSendQueue()->Push(point, CLICK_DATA, id);
	CFormView::OnLButtonDown(nFlags, point);
}

void DrawingView::OnMouseMove(UINT nFlags, CPoint point)
{
	CClientDC dc(this);

	// check  clicked mouse left button
	if ((nFlags && MK_LBUTTON) == MK_LBUTTON)
	{
		RECT my_rect;
		GetClientRect(&my_rect);

		// push to type is 'move'
		std::string id;
		if (server != nullptr && serverRunning)
		{
			id = ClientMap::GetClientMap()->GetValue(L"server");
			PointDataList::GetQueue()->Insert(this->Name, point, MOVE_DATA, id);
		}
		else
			id = "";

		DrawingQueue::GetSendQueue()->Push(point, MOVE_DATA, id);
		PointDataList::GetQueue()->Insert(this->Name, point, MOVE_DATA, id);

		if (server != nullptr && serverRunning)
		{
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
		}
	}
	CFormView::OnMouseMove(nFlags, point);
}

afx_msg LRESULT DrawingView::OnDrawpop(WPARAM wParam, LPARAM lParam)
{
	PointData point = DrawingQueue::GetReceiveQueue()->Pop();
	if (point.x <= -1 || point.y <= -1)
		return 1;

	CClientDC dc(this);

	// get this view rect
	RECT my_rect;
	GetClientRect(&my_rect);

	// get client id
	std::string client_id = std::string(point.id);
	PointDataList::GetQueue()->Insert(this->Name, point);

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

	if (server != nullptr && serverRunning)
	{
		DrawingQueue::GetSendQueue()->Push(point);
	}

	return 0;
}

UINT DrawingView::threadReceiveQeueuRunner(LPVOID param)
{
	DrawingView* thisView = (DrawingView*)param;
	MessageQueue::GetInstance()->Push(L"Thread Receive Queue Runner");
	while (1)
	{
		PostMessageA(thisView->m_hWnd, WM_DRAWPOP, NULL, NULL);
		Sleep(1);
	}
	return 0;
}

UINT DrawingView::threadSendQueueRunner(LPVOID param)
{
	DrawingView* dv = (DrawingView*)param;
	MessageQueue::GetInstance()->Push(L"Thread Send Queue Runner");
	while (1)
	{
		PostMessageA(dv->m_hWnd, WM_SENDDRAW, NULL, NULL);
		Sleep(1);
	}
	return 0;
}

bool DrawingView::ClientRun(CString ip, UINT port)
{
	if (server != nullptr && serverRunning)
	{
		clientRunning = false;
		return false;
	}

	client = new CClientSocket;
	client->Create();

	BOOL check = client->Connect(ip, port);
	if (check)
	{
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
			if (server->Listen(100))
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
		server->Close();
		delete server;
		server = nullptr;
	}
	else
	{
		server = nullptr;
	}

	serverRunning = false;
	return true;
}

afx_msg LRESULT DrawingView::OnSenddraw(WPARAM wParam, LPARAM lParam)
{
	PointData point = DrawingQueue::GetSendQueue()->Pop();
	if (point.x == -1 || point.y == -1)
	{
		return 1;
	}

	std::string message_str = point.ToString();

	if (server != nullptr && serverRunning)
	{
		server->BroadCast((char*)message_str.c_str(), DATA_SIZE + CLIENT_NAME_SIZE);
	}

	if (clientRunning)
	{
		char message[DATA_SIZE] = { 0, };
		int i = 0;
		for (i = 0; i != DATA_SIZE; i++)
		{
			if (message_str.c_str()[i] == '\0')
			{
				message[i] = '\0';
				break;
			}
			message[i] = message_str.c_str()[i];
		}
		client->Send(message, DATA_SIZE);
	}
	return 0;
}
