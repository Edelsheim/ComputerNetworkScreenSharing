#pragma once

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
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();

	// change background color
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	// control button
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	// message call
	afx_msg LRESULT OnDrawpop(WPARAM wParam, LPARAM lParam);

	// thread runner
	static UINT threadReceiveQeueuRunner(LPVOID param);

	// view thread runner call
	void ClientRun();

private:
	CWinThread* threadReceiveQueue;
};


