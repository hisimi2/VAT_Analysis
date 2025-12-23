// ModalessMessageBox.cpp : implementation file
//

#include "stdafx.h"
#include "ShowMessageBox.h"
#include "LibCommCtrlDrv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CLibCommCtrlDrv g_LibCommCtrlDrv;

/////////////////////////////////////////////////////////////////////////////

CShowMsgBox::CShowMsgBox(CWnd* pParent /*=NULL*/)
	: CDialog(CShowMsgBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModalessMessageBox)
	//}}AFX_DATA_INIT
	mBt_type = 0;
	m_bCloseMode = FALSE;
	m_pMsgInfo = NULL;
}


void CShowMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModalessMessageBox)
	DDX_Control(pDX, IDC_TEXTBOX1, m_szMessage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShowMsgBox, CDialog)
	//{{AFX_MSG_MAP(CModalessMessageBox)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON2_CLOSE, OnButton2Close)
	ON_BN_CLICKED(IDC_BUTTON2_CLOSE2, OnButton2Close2)
	ON_WM_CLOSE()	// By hyb [150529] [BUGFIX-SITE] 온도표시 안정화 디버깅 하면서 수정
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModalessMessageBox message handlers

BOOL CShowMsgBox::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_szTitle.Format("%s",m_pMsgInfo->szCode);
	m_szTempMessage.Format("%s", m_pMsgInfo->szMessage);

	// TODO: Add extra initialization here
	this->SetWindowText(m_szTitle);	//20131123 ksj Title Bar Error Code 추가
	m_szMessage.SetText(m_szTempMessage);
	m_bCloseMode = TRUE;


	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CShowMsgBox::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		CenterWindow();
		this->SetWindowText(m_szTitle);	//20131123 ksj Title Bar Error Code 추가
		m_szMessage.SetText(m_szTempMessage);
		m_szMessage.SetBackColor(RGB(255,255,255));

		GetDlgItem(IDC_BUTTON_CLOSE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_BUTTON2_CLOSE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON2_CLOSE2)->ShowWindow(SW_HIDE);        
		UpdateData(FALSE);
	}
}
void CShowMsgBox::OnButtonClose() 
{
	OnClose();
}

void CShowMsgBox::OnButton2Close() 
{
	OnClose();
}

void CShowMsgBox::OnButton2Close2() 
{
	OnClose();
}
void CShowMsgBox::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//CDialog::OnClose();
	this->DestroyWindow();
}


void CShowMsgBox::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	g_LibCommCtrlDrv.DelShowMsgBox(m_pMsgInfo);
	delete this;

	CDialog::PostNcDestroy();
}
