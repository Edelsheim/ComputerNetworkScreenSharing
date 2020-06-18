
// ComputerNetworkScreenSharingDlg.h: 헤더 파일
//

#pragma once

#include "CListenSocket.h"
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

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnPop(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	CListenSocket* server;
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
	CWinThread* client_thread;
	CWinThread* server_thread;

public:
	afx_msg void OnBnClickedServerrun();
	afx_msg void OnClickedConnectbutton();
	afx_msg LRESULT OnSenddraw(WPARAM wParam, LPARAM lParam);

	// thread function
	static UINT OnLogThread(LPVOID param);
	static UINT OnServerThread(LPVOID param);

};
