// DrawingView.cpp: 구현 파일
//

#include "pch.h"
#include "ComputerNetworkScreenSharing.h"
#include "DrawingView.h"

#include "MessageQueue.h"
#include "DrawingQueue.h"

// DrawingView

IMPLEMENT_DYNCREATE(DrawingView, CFormView)

DrawingView::DrawingView()
	: CFormView(IDD_Draw)
{
	this->point.x = -1;
	this->point.y = -1;
	this->receivePoint.x = -1;
	this->receivePoint.y = -1;
	isClient = false;
	threadReceiveQueue = nullptr;
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
	this->receivePoint.x = -1;
	this->receivePoint.y = -1;

	threadReceiveQueue = AfxBeginThread(threadReceiveQeueuRunner, this);
}

BOOL DrawingView::DestroyWindow()
{
	if (threadReceiveQueue != nullptr)
	{
		threadReceiveQueue->ExitInstance();
		threadReceiveQueue = nullptr;
	}

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

	if (!isClient)
	{
		dc.MoveTo(point.x, point.y);
		this->point.x = point.x;
		this->point.y = point.y;
	}
	
	// push to type is 'click'
	DrawingQueue::GetSendQueue()->Push(point, 'c');
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
		DrawingQueue::GetSendQueue()->Push(point, 'm');
		if (!isClient)
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

	// check this is first
	if (this->receivePoint.x == -1)
		this->receivePoint = point.x;
	if (this->receivePoint.y == -1)
		this->receivePoint.y = point.y;

	if (point.type == 'c') // check type is 'click'
	{
		dc.MoveTo(point.x, point.y);
		this->receivePoint.x = point.x;
		this->receivePoint.y = point.y;
	}
	else if (point.x <= my_rect.left + 3)
	{
		dc.MoveTo(my_rect.left, point.y);
		dc.LineTo(point.x, point.y);
		this->receivePoint.x = point.x;
		this->receivePoint.y = point.y;
	}
	else if (point.x >= my_rect.right - 3)
	{
		dc.MoveTo(my_rect.right, point.y);
		dc.LineTo(point.x, point.y);
		this->receivePoint.x = point.x;
		this->receivePoint.y = point.y;
	}
	else if (point.y <= my_rect.top + 3)
	{
		dc.MoveTo(point.x, my_rect.top);
		dc.LineTo(point.x, point.y);
		this->receivePoint.x = point.x;
		this->receivePoint.y = point.y;
	}
	else if (point.y >= my_rect.bottom - 3)
	{
		dc.MoveTo(point.x, my_rect.bottom);
		dc.LineTo(point.x, point.y);
		this->receivePoint.x = point.x;
		this->receivePoint.y = point.y;
	}
	else
	{
		dc.MoveTo(this->receivePoint.x, this->receivePoint.y);
		dc.LineTo(point.x, point.y);
		this->receivePoint.x = point.x;
		this->receivePoint.y = point.y;
	}

	if (!isClient)
	{
		DrawingQueue::GetSendQueue()->Push(point);
	}

	return 0;
}

UINT DrawingView::threadReceiveQeueuRunner(LPVOID param)
{
	DrawingView* thisView = (DrawingView*)param;
	MessageQueue::GetInstance()->Push(L"Thread Receive Qeueu Runner");
	while (1)
	{
		PostMessageA(thisView->m_hWnd, WM_DRAWPOP, NULL, NULL);
		Sleep(1);
	}
	return 0;
}

void DrawingView::ClientRun()
{
	isClient = true;
	MessageQueue::GetInstance()->Push(L"Drawing View Client Run");
}