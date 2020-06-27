
// ComputerNetworkScreenSharingDlg.h: 헤더 파일
//

#pragma once

#include "CListenSocket.h"
#include "CClientSocket.h"
#include "DrawingView.h"

// CComputerNetworkScreenSharingDlg 대화 상자
class CComputerNetworkScreenSharingDlg : public CDialogEx
{
// 생성입니다.
public:
	CComputerNetworkScreenSharingDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Main_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	void InitCustomDialog();
	void InitEditValue();
	void InitProcessButton();
	void InitDrawingView();

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnPop(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	DrawingView* dwView;
	
	CEdit ServerPort;
	CButton ServerRunButton;

	CListBox LogList;

	void ServerRun();

	// thread
	CWinThread* log_thread;

	// list process
	// Drawing View
	DrawingView* dwViewList[MAX_PROCESS];
	
public:
	afx_msg void OnBnClickedServerrun();

	// thread function
	static UINT OnLogThread(LPVOID param);

private:
	std::wstring ProcessName[MAX_PROCESS];
	bool ProcessActive[MAX_PROCESS];
	CButton* ButtonViewProcessList[MAX_PROCESS];
	CButton* ButtonCloseProcessList[MAX_PROCESS];

	CButton ButtonViewMyProcess;
	CButton ButtonViewProcess1;
	CButton ButtonViewProcess2;
	CButton ButtonViewProcess3;
	CButton ButtonViewProcess4;
	CButton ButtonViewProcess5;
	CButton ButtonViewProcess6;
	CButton ButtonViewProcess7;
	CButton ButtonViewProcess8;
	CButton ButtonViewProcess9;
	CButton ButtonViewProcess10;
	CButton ButtonCloseProcess1;
	CButton ButtonCloseProcess2;
	CButton ButtonCloseProcess3;
	CButton ButtonCloseProcess4;
	CButton ButtonCloseProcess5;
	CButton ButtonCloseProcess6;
	CButton ButtonCloseProcess7;
	CButton ButtonCloseProcess8;
	CButton ButtonCloseProcess9;
	CButton ButtonCloseProcess10;
};

static BOOL isClose;