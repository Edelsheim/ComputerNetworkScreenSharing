
// ComputerNetworkScreenSharingDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ComputerNetworkScreenSharing.h"
#include "ComputerNetworkScreenSharingDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "MessageQueue.h"
#include "DrawingQueue.h"

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CComputerNetworkScreenSharingDlg 대화 상자



CComputerNetworkScreenSharingDlg::CComputerNetworkScreenSharingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Main_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	dwView = nullptr;

	isClose = false;
	log_thread = nullptr;
	client_thread = nullptr;
	server_thread = nullptr;
}

void CComputerNetworkScreenSharingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ServerPort, ServerPort);
	DDX_Control(pDX, IDC_ConnectIP, ConnectIP);
	DDX_Control(pDX, IDC_ConnectPort, ConnectPort);
	DDX_Control(pDX, IDC_ConnectButton, ConnectButton);
	DDX_Control(pDX, IDC_ServerRunButton, ServerRunButton);
	DDX_Control(pDX, IDC_LogList, LogList);
}

BEGIN_MESSAGE_MAP(CComputerNetworkScreenSharingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_POP, &CComputerNetworkScreenSharingDlg::OnPop)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_ServerRunButton, &CComputerNetworkScreenSharingDlg::OnBnClickedServerrun)
	ON_BN_CLICKED(IDC_ConnectButton, &CComputerNetworkScreenSharingDlg::OnClickedConnectbutton)
END_MESSAGE_MAP()


// CComputerNetworkScreenSharingDlg 메시지 처리기

void CComputerNetworkScreenSharingDlg::InitCustomDialog()
{
	InitDrawingView();
}

void CComputerNetworkScreenSharingDlg::InitEditValue()
{
	CString defaultServerPortStr;
	BOOL check = defaultServerPortStr.LoadStringW(IDS_DefaultServerPort);
	ASSERT(check);
	ServerPort.SetWindowTextW(defaultServerPortStr);

	CString defaultConnectIPStr;
	check = defaultConnectIPStr.LoadStringW(IDS_DefaultConnectIP);
	ASSERT(check);
	ConnectIP.SetWindowTextW(defaultConnectIPStr);

	CString defaultConnectPortStr;
	check = defaultConnectPortStr.LoadStringW(IDS_DefaultConnectPort);
	ASSERT(check);
	ConnectPort.SetWindowTextW(defaultConnectPortStr);
}


BOOL CComputerNetworkScreenSharingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	// init Socket
	AfxSocketInit();

	InitCustomDialog();
	InitEditValue();

	log_thread = AfxBeginThread(OnLogThread, this);
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CComputerNetworkScreenSharingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CComputerNetworkScreenSharingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CComputerNetworkScreenSharingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

afx_msg LRESULT CComputerNetworkScreenSharingDlg::OnPop(WPARAM wParam, LPARAM lParam)
{
	std::wstring message_wstr = MessageQueue::GetInstance()->Pop();
	
	if (!message_wstr.empty())
		LogList.InsertString(0, message_wstr.c_str());

	return 0;
}

void CComputerNetworkScreenSharingDlg::InitDrawingView()
{
	CCreateContext context;
	ZeroMemory(&context, sizeof(context));

	// get picture control size
	CRect pic_con_rect;
	GetDlgItem(IDC_PicCon)->GetWindowRect(&pic_con_rect);
	ScreenToClient(&pic_con_rect);

	//drawing view create
	dwView = new DrawingView();
	dwView->Create(NULL, NULL, WS_CHILDWINDOW, pic_con_rect, this, IDD_Draw, &context);
	dwView->OnInitialUpdate();
	dwView->ShowWindow(SW_SHOW);

	// remove picture control
	GetDlgItem(IDC_PicCon)->DestroyWindow();
}

void CComputerNetworkScreenSharingDlg::ServerRun()
{
	CString portStr;
	UINT port = 0;
	ServerPort.GetWindowTextW(portStr);
	port = _ttoi(portStr);

	if (dwView->ServerRun(port))
	{
		// success log
		MessageQueue::GetInstance()->Push(L"Socket Server Open");
		ServerRunButton.SetWindowTextW(_T("Server Close"));
		ServerRunButton.EnableWindow(TRUE);
	}
	else
	{
		// error log
		dwView->ServerClose();
		ServerRunButton.EnableWindow(TRUE);
		//MessageQueue::GetInstance()->Push(L"Error Code(" + std::to_wstring(error_code) + L")");
	}
}

void CComputerNetworkScreenSharingDlg::OnBnClickedServerrun()
{
	if (dwView->serverRunning)
	{
		ServerRunButton.EnableWindow(FALSE);

		dwView->ServerClose();

		MessageQueue::GetInstance()->Push(L"Socket Server Close");
		ServerRunButton.SetWindowTextW(_T("Server Run"));
		ServerRunButton.EnableWindow(TRUE);
	}
	else
	{
		ServerRunButton.EnableWindow(FALSE);
		ServerRun();
	}
}

void CComputerNetworkScreenSharingDlg::OnClickedConnectbutton()
{
	BYTE a, b, c, d;
	CString ip;
	ConnectIP.GetAddress(a, b, c, d);
	ip.Format(_T("%d.%d.%d.%d"), a, b, c, d);

	CString port;
	ConnectPort.GetWindowTextW(port);

	std::wstring str = L"Server ";
	str.append(ip);
	str.append(L":");
	str.append(port);

	// check dwView is new process or old process
	if (dwView->ClientRun(ip, _ttoi(port)))
	{
		str.append(L" connected");
		MessageQueue::GetInstance()->Push(str);
	}
	else
	{
		str.append(L" fail");
		MessageQueue::GetInstance()->Push(str);
	}
}

UINT CComputerNetworkScreenSharingDlg::OnLogThread(LPVOID param)
{
	CComputerNetworkScreenSharingDlg* dlg = (CComputerNetworkScreenSharingDlg*)param;

	while (1)
	{
		PostMessageA(dlg->m_hWnd, WM_POP, NULL, NULL);
		Sleep(130);
	}
	return 0;
}


BOOL CComputerNetworkScreenSharingDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	isClose = true;

	HANDLE threades[3] = { log_thread, client_thread, server_thread };
	WaitForMultipleObjects(3, threades, TRUE, 500);

	if (log_thread != nullptr)
	{
		log_thread = nullptr;
	}
	if (client_thread != nullptr)
	{
		client_thread = nullptr;
	}
	if (server_thread != nullptr)
	{
		server_thread = nullptr;
	}
	if (dwView != nullptr)
	{
		dwView->DestroyWindow();
		// delete dwView;
		dwView = nullptr;
	}
	return CDialogEx::DestroyWindow();
}
