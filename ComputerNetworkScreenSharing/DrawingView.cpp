﻿// DrawingView.cpp: 구현 파일
//

#include "pch.h"
#include "ComputerNetworkScreenSharing.h"
#include "DrawingView.h"


// DrawingView

IMPLEMENT_DYNCREATE(DrawingView, CFormView)

DrawingView::DrawingView()
	: CFormView(IDD_Draw)
{

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

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	this->point.x = 0;
	this->point.y = 0;
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

	dc.MoveTo(point.x, point.y);
	this->point.x = point.x;
	this->point.y = point.y;
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
	CFormView::OnMouseMove(nFlags, point);
}