#pragma once


// dialog add to form view
// https://luckygg.tistory.com/177

// change background
// http://egloos.zum.com/mindol/v/214184

// drawing
// https://blog.naver.com/rkttndk/221434286381

// DrawingView 폼 보기

class DrawingView : public CFormView
{
	DECLARE_DYNCREATE(DrawingView)

private:
	CPoint point; // draw point

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Draw };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
	DECLARE_MESSAGE_MAP()
public:
	DrawingView();
	virtual ~DrawingView();

	// call protected's CFormView methods
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate(); // init update

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor); // change background color

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


