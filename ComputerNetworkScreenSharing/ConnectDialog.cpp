// ConnectDialog.cpp: 구현 파일
//

#include "pch.h"
#include "ComputerNetworkScreenSharing.h"
#include "ConnectDialog.h"
#include "afxdialogex.h"


// ConnectDialog 대화 상자

IMPLEMENT_DYNAMIC(ConnectDialog, CDialogEx)

ConnectDialog::ConnectDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ConnectDialog, pParent)
{

}

ConnectDialog::~ConnectDialog()
{
}

void ConnectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ServerIP, ConnectIP);
	DDX_Control(pDX, IDC_ServerPort, ConnectPort);
}


BEGIN_MESSAGE_MAP(ConnectDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &ConnectDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// ConnectDialog 메시지 처리기


BOOL ConnectDialog::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::DestroyWindow();
}


BOOL ConnectDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	BOOL check;

	CString defaultConnectIPStr;
	check = defaultConnectIPStr.LoadStringW(IDS_DefaultConnectIP);
	ASSERT(check);
	ConnectIP.SetWindowTextW(defaultConnectIPStr);

	CString defaultConnectPortStr;
	check = defaultConnectPortStr.LoadStringW(IDS_DefaultConnectPort);
	ASSERT(check);
	ConnectPort.SetWindowTextW(defaultConnectPortStr);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void ConnectDialog::OnBnClickedOk()
{
	BYTE a, b, c, d;
	ConnectIP.GetAddress(a, b, c, d);
	ip.Format(_T("%d.%d.%d.%d"), a, b, c, d);

	ConnectPort.GetWindowTextW(port);
	CDialogEx::OnOK();
}
