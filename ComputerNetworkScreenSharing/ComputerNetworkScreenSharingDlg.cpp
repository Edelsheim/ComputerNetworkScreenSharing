
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
#include "ConnectList.h"

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
}

void CComputerNetworkScreenSharingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ServerPort, ServerPort);
	DDX_Control(pDX, IDC_ServerRunButton, ServerRunButton);
	DDX_Control(pDX, IDC_LogList, LogList);
	DDX_Control(pDX, IDC_ViewMyProcess, ButtonViewMyProcess);
	DDX_Control(pDX, IDC_ViewProcess1, ButtonViewProcess1);
	DDX_Control(pDX, IDC_ViewProcess2, ButtonViewProcess2);
	DDX_Control(pDX, IDC_ViewProcess3, ButtonViewProcess3);
	DDX_Control(pDX, IDC_ViewProcess4, ButtonViewProcess4);
	DDX_Control(pDX, IDC_ViewProcess5, ButtonViewProcess5);
	DDX_Control(pDX, IDC_ViewProcess6, ButtonViewProcess6);
	DDX_Control(pDX, IDC_ViewProcess7, ButtonViewProcess7);
	DDX_Control(pDX, IDC_ViewProcess8, ButtonViewProcess8);
	DDX_Control(pDX, IDC_ViewProcess9, ButtonViewProcess9);
	DDX_Control(pDX, IDC_ViewProcess10, ButtonViewProcess10);
	DDX_Control(pDX, IDC_ClosePorcess1, ButtonCloseProcess1);
	DDX_Control(pDX, IDC_ClosePorcess3, ButtonCloseProcess2);
	DDX_Control(pDX, IDC_ClosePorcess2, ButtonCloseProcess3);
	DDX_Control(pDX, IDC_ClosePorcess4, ButtonCloseProcess4);
	DDX_Control(pDX, IDC_ClosePorcess5, ButtonCloseProcess5);
	DDX_Control(pDX, IDC_ClosePorcess6, ButtonCloseProcess6);
	DDX_Control(pDX, IDC_ClosePorcess7, ButtonCloseProcess7);
	DDX_Control(pDX, IDC_ClosePorcess8, ButtonCloseProcess8);
	DDX_Control(pDX, IDC_ClosePorcess9, ButtonCloseProcess9);
	DDX_Control(pDX, IDC_ClosePorcess10, ButtonCloseProcess10);
}

BEGIN_MESSAGE_MAP(CComputerNetworkScreenSharingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_POP, &CComputerNetworkScreenSharingDlg::OnPop)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_ServerRunButton, &CComputerNetworkScreenSharingDlg::OnBnClickedServerrun)
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

	/*
	CString defaultConnectIPStr;
	check = defaultConnectIPStr.LoadStringW(IDS_DefaultConnectIP);
	ASSERT(check);
	ConnectIP.SetWindowTextW(defaultConnectIPStr);

	CString defaultConnectPortStr;
	check = defaultConnectPortStr.LoadStringW(IDS_DefaultConnectPort);
	ASSERT(check);
	ConnectPort.SetWindowTextW(defaultConnectPortStr);
	*/
}

void CComputerNetworkScreenSharingDlg::InitProcessButton()
{
	ProcessName[0] = L"MyServer";
	ProcessName[1] = L"Process1";
	ProcessName[2] = L"Process2";
	ProcessName[3] = L"Process3";
	ProcessName[4] = L"Process4";
	ProcessName[5] = L"Process5";
	ProcessName[6] = L"Process6";
	ProcessName[7] = L"Process7";
	ProcessName[8] = L"Process8";
	ProcessName[9] = L"Process9";
	ProcessName[10] = L"Process10";

	ButtonViewProcessList[0] = &ButtonViewMyProcess;
	ButtonViewProcessList[1] = &ButtonViewProcess1;
	ButtonViewProcessList[2] = &ButtonViewProcess2;
	ButtonViewProcessList[3] = &ButtonViewProcess3;
	ButtonViewProcessList[4] = &ButtonViewProcess4;
	ButtonViewProcessList[5] = &ButtonViewProcess5;
	ButtonViewProcessList[6] = &ButtonViewProcess6;
	ButtonViewProcessList[7] = &ButtonViewProcess7;
	ButtonViewProcessList[8] = &ButtonViewProcess8;
	ButtonViewProcessList[9] = &ButtonViewProcess9;
	ButtonViewProcessList[10] = &ButtonViewProcess10;

	ButtonCloseProcessList[0] = nullptr;;
	ButtonCloseProcessList[1] = &ButtonCloseProcess1;
	ButtonCloseProcessList[2] = &ButtonCloseProcess2;
	ButtonCloseProcessList[3] = &ButtonCloseProcess3;
	ButtonCloseProcessList[4] = &ButtonCloseProcess4;
	ButtonCloseProcessList[5] = &ButtonCloseProcess5;
	ButtonCloseProcessList[6] = &ButtonCloseProcess6;
	ButtonCloseProcessList[7] = &ButtonCloseProcess7;
	ButtonCloseProcessList[8] = &ButtonCloseProcess8;
	ButtonCloseProcessList[9] = &ButtonCloseProcess9;
	ButtonCloseProcessList[10] = &ButtonCloseProcess10;

	for (int i = 0; i != MAX_PROCESS; i++)
	{
		ProcessActive[i] = false;
		dwViewList[i] = nullptr;
		ButtonViewProcessList[i]->EnableWindow(FALSE);
		if (ButtonCloseProcessList[i] != nullptr)
			ButtonCloseProcessList[i]->SetWindowTextW(ConnectServerWString);
	}
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
	InitProcessButton();

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

void CComputerNetworkScreenSharingDlg::ServerRun()
{
	CString portStr;
	UINT port = 0;
	ServerPort.GetWindowTextW(portStr);
	port = _ttoi(portStr);

	if (dwView->ServerRun(port))
	{
		// success log
		MessageQueue::GetInstance()->Push(L"화면 공유 작동 중");
		ServerRunButton.SetWindowTextW(_T("내 화면 공유 끄기"));
		ServerRunButton.EnableWindow(TRUE);
	}
	else
	{
		// error log
		dwView->ServerClose();
		ServerRunButton.EnableWindow(TRUE);
	}
}

void CComputerNetworkScreenSharingDlg::OnBnClickedServerrun()
{
	if (dwView->serverRunning)
	{
		ServerRunButton.EnableWindow(FALSE);

		dwView->ServerClose();

		MessageQueue::GetInstance()->Push(L"화면 공유 작동 중지");
		ServerRunButton.SetWindowTextW(_T("내 화면 공유 켜기"));
		ServerRunButton.EnableWindow(TRUE);
	}
	else
	{
		ServerRunButton.EnableWindow(FALSE);
		ServerRun();
	}
}
/*
void CComputerNetworkScreenSharingDlg::OnClickedConnectbutton()
{
	BYTE a, b, c, d;
	CString ip;
	ConnectIP.GetAddress(a, b, c, d);
	ip.Format(_T("%d.%d.%d.%d"), a, b, c, d);

	CString port;
	ConnectPort.GetWindowTextW(port);

	std::wstring str = L"프로세스 연결 ";
	str.append(ip);
	str.append(L":");
	str.append(port);

	std::wstring connect_info = ip;
	connect_info.append(L":");
	connect_info.append(port);
	
	// get not active process
	int i = 0;
	for(i = 0; i != MAX_PROCESS; i++)
	{
		if (ProcessActive[i] == false)
			break;
	}

	if (i == MAX_PROCESS)
	{
		MessageQueue::GetInstance()->Push(L"Process가 전부 활성화 중입니다.");
		return;
	}

	// insert return to true, insert done
	if (ConnectList::GetConnectList()->Insert(ProcessName[i], connect_info))
	{
		// check dwView is new process or old process
		if (dwView->ClientRun(ip, _ttoi(port)))
		{
			str.append(L" 접속 성공");
			ProcessActive[i] = true;
			MessageQueue::GetInstance()->Push(str);
		}
		else
		{
			str.append(L" 접속 실패");
			MessageQueue::GetInstance()->Push(str);
		}
	}
	else
	{
		// connect same process
		std::wstring process_name = ConnectList::GetConnectList()->FindProcessNameWithConnectInfo(connect_info);

		str.append(L" 이미 " + process_name + L"에서 접속 중입니다.");
		MessageQueue::GetInstance()->Push(str);
	}
}
*/

UINT CComputerNetworkScreenSharingDlg::OnLogThread(LPVOID param)
{
	CComputerNetworkScreenSharingDlg* dlg = (CComputerNetworkScreenSharingDlg*)param;

	while (1)
	{
		PostMessageA(dlg->GetSafeHwnd(), WM_POP, NULL, NULL);
		Sleep(200);
	}
	return 0;
}


BOOL CComputerNetworkScreenSharingDlg::DestroyWindow()
{
	isClose = true;

	if (log_thread != nullptr)
	{
		log_thread->ExitInstance();
		log_thread = nullptr;
	}

	if (dwView != nullptr)
	{
		dwView->DestroyWindow();
		dwView = nullptr;
	}
	return CDialogEx::DestroyWindow();
}