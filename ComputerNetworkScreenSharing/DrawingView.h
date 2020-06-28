#pragma once

// DrawingView 폼 보기

#include "CClientSocket.h"
#include "CListenSocket.h"

#include <iostream>
#include <concurrent_unordered_map.h>
typedef Concurrency::concurrent_unordered_map<std::string, CPoint> ClientCPointMap;

class DrawingView : public CFormView
{
	DECLARE_DYNCREATE(DrawingView)

private:
	CPoint point; // draw point
	ClientCPointMap receivePointes;

	CListenSocket* server;
	CClientSocket* client;

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

	bool serverRunning;
	bool clientRunning;

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
	afx_msg LRESULT OnSenddraw(WPARAM wParam, LPARAM lParam);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	// thread runner
	static UINT threadReceiveQeueuRunner(LPVOID param);
	static UINT threadSendQueueRunner(LPVOID param);

	// view thread runner call
	bool ClientRun(CString ip, UINT port);
	bool ClientClose();
	bool ServerRun(UINT port);
	bool ServerClose();

	void DrawingViewStart();
	void DrawingViewPause();
	void DrawingPoint();

	// getter, setter
	void SetName(std::wstring Name) { this->Name = std::wstring(Name.c_str()); };
	std::wstring GetName() { return this->Name; };

	enum ThreadStatus {
		ThreadStatusNullptr,
		ThreadStatusRun,
		ThreadStatusPause,
		ThreadStatusClose
	};
	ThreadStatus threadReceiveStatus;
	ThreadStatus threadSendStatus;

private:
	std::wstring Name;
	CWinThread* threadReceiveQueue;
	CWinThread* threadSendQueue;
};


