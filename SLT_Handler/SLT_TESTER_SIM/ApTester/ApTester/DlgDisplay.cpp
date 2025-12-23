// DlgDisplay.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ApTester.h"
#include "DlgDisplay.h"
#include "afxdialogex.h"


// CDlgDisplay 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgDisplay, CDialogEx)

CDlgDisplay::CDlgDisplay(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TesterInfo, pParent)
{

}

CDlgDisplay::~CDlgDisplay()
{
}

void CDlgDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CDlgDisplay, CDialogEx)
	ON_WM_MEASUREITEM()
END_MESSAGE_MAP()


// CDlgDisplay 메시지 처리기입니다.


BOOL CDlgDisplay::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rt;
	m_ListCtrl.GetWindowRect(&rt);

	m_ListCtrl.InsertColumn(0, TEXT(_T("")), LVCFMT_LEFT , rt.Width()*0.1);
	m_ListCtrl.InsertColumn(1, TEXT(_T("")), LVCFMT_CENTER, rt.Width()*0.15);
	m_ListCtrl.InsertColumn(2, TEXT(_T("")), LVCFMT_CENTER, rt.Width()*0.11);
	m_ListCtrl.InsertColumn(3, TEXT(_T("")), LVCFMT_CENTER, rt.Width()*0.1);
	m_ListCtrl.InsertColumn(4, TEXT(_T("")), LVCFMT_CENTER, rt.Width()*0.1);
	m_ListCtrl.InsertColumn(5, TEXT(_T("")), LVCFMT_CENTER, rt.Width()*0.1);
	m_ListCtrl.InsertColumn(6, TEXT(_T("")), LVCFMT_CENTER, rt.Width()*0.1);
	m_ListCtrl.InsertColumn(7, TEXT(_T("")), LVCFMT_CENTER, rt.Width()*0.1);
	m_ListCtrl.InsertColumn(8, TEXT(_T("")), LVCFMT_CENTER, rt.Width()*0.1);
	
	m_ListCtrl.InsertItem(0,0);
	m_ListCtrl.InsertItem(1,0);
	m_ListCtrl.InsertItem(2, 0);
	m_ListCtrl.InsertItem(3, 0);
	m_ListCtrl.InsertItem(4, 0);
	m_ListCtrl.InsertItem(5, 0);

	m_ListCtrl.SetItem(1, 0, LVIF_TEXT, TEXT("Station     :"),NULL,NULL,NULL,NULL);
	m_ListCtrl.SetItem(2, 0, LVIF_TEXT, TEXT("Status      :"), NULL, NULL, NULL, NULL);
	m_ListCtrl.SetItem(3, 0, LVIF_TEXT, TEXT("IP             :"), NULL, NULL, NULL, NULL);
	m_ListCtrl.SetItem(4, 0, LVIF_TEXT, TEXT("Socket ID :"), NULL, NULL, NULL, NULL);
	m_ListCtrl.SetItem(5, 0, LVIF_TEXT, TEXT("Version    :"), NULL, NULL, NULL, NULL);

	CFont font;
	font.CreatePointFont(150,"Arial");
	m_ListCtrl.SetFont(&font);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgDisplay::showTesterInfo(CConectSocket* sock, int nActionFlag)
{
	m_ListCtrl.SetItem(1, 1, LVIF_TEXT, TEXT( getStrStationNo(sock) ), NULL, NULL, NULL, NULL);
	m_ListCtrl.SetItem(2, 1, LVIF_TEXT, TEXT( getStrState(sock) ), NULL, NULL, NULL, NULL);
	m_ListCtrl.SetItem(2, 2, LVIF_TEXT, TEXT( getStrActMode(sock) ), NULL, NULL, NULL, NULL);
	m_ListCtrl.SetItem(3, 1, LVIF_TEXT, TEXT( getStrIp(sock)), NULL, NULL, NULL, NULL);
	
	for (int nI = 1; nI <= 8; nI++)
	{
		m_ListCtrl.SetItem(4, nI, LVIF_TEXT, TEXT(getStrSocketId(sock, nI)), NULL, NULL, NULL, NULL);
	}

	m_ListCtrl.SetItem(5, 1, LVIF_TEXT, TEXT(getStrVersion(sock)), NULL, NULL, NULL, NULL);

	//if (sock->GetState() == STATUS_IDLE)
	//	m_ListCtrl.SetItem(2, 2, LVIF_TEXT, TEXT("NONE"), NULL, NULL, NULL, NULL);
}


CString  CDlgDisplay::getStrStationNo(CConectSocket* sock) 
{
	CString strStationNo;

	strStationNo.Format(_T("%d 번"), sock->getStationId());

	return strStationNo;
}

CString  CDlgDisplay::getStrState(CConectSocket* sock)
{
	switch ( sock->GetState() )
	{
	case 0: return "INIT";
	case 1: return "IDLE";
	case 2: return "TESTING";
	case 3: return "ERROR";
	case 4: return "UNREGISTER";
	}
}

CString CDlgDisplay::getStrActMode(CConectSocket* sock )
{
	switch ( sock->getActionMode() )
	{
	case 0: return "Normal";   
	case 1: return  "GRR";
	case 2: return  "LoopTest";
	case 3: return  "Audit";
	}
	return "NONE";
}

CString  CDlgDisplay::getStrIp(CConectSocket* sock)
{
	CString strIp = sock->getIp();

	return strIp;
}

CString  CDlgDisplay::getStrSocketId(CConectSocket* sock , int nI)
{
	CString strSocketId;

	strSocketId.Format(_T("%d"), (sock->getSocketId() * 100 + nI ));

	return strSocketId;
}

CString  CDlgDisplay::getStrVersion(CConectSocket* sock)
{
	CString strVersion;

	strVersion.Format(_T("%s"), sock->getVersion());

	return strVersion;
}

