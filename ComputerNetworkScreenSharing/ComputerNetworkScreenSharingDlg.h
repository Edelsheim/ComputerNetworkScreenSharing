
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

	CIPAddressCtrl ConnectIP;
	CEdit ConnectPort;
	CButton ConnectButton;

	CListBox LogList;

	void InitDrawingView();
	void ServerRun();

	// thread
	CWinThread* log_thread;

	// list process
	// Drawing View

	
public:
	afx_msg void OnBnClickedServerrun();
	afx_msg void OnClickedConnectbutton();

	// thread function
	static UINT OnLogThread(LPVOID param);

	afx_msg void OnBnClickedMyServer();
	afx_msg void OnBnClickedProcess1();
	afx_msg void OnBnClickedProcess2();
	afx_msg void OnBnClickedProcess3();
	afx_msg void OnBnClickedProcess4();
	afx_msg void OnBnClickedProcess5();
	afx_msg void OnBnClickedProcess6();
	afx_msg void OnBnClickedProcess7();
	afx_msg void OnBnClickedProcess8();
	afx_msg void OnBnClickedProcess9();
	afx_msg void OnBnClickedProcess10();
private:
	CButton ButtonMyServer;
	CButton ButtonProcess1;
	CButton ButtonProcess2;
	CButton ButtonProcess3;
	CButton ButtonProcess4;
	CButton ButtonProcess5;
	CButton ButtonProcess6;
	CButton ButtonProcess7;
	CButton ButtonProcess8;
	CButton ButtonProcess9;
	CButton ButtonProcess10;
};

static BOOL isClose;