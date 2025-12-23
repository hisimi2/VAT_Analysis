// TrayMapDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "TrayMapDlg.h"
#include "afxdialogex.h"


// CTrayMapDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTrayMapDlg, CDialog)

CTrayMapDlg::CTrayMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTrayMapDlg::IDD, pParent)
{
	m_pWnd_Show = NULL;
}

CTrayMapDlg::~CTrayMapDlg()
{
}

void CTrayMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_CTRL, m_TabCtrl);
}


BEGIN_MESSAGE_MAP(CTrayMapDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_CTRL, &CTrayMapDlg::OnTcnSelchangingTabView)
	ON_NOTIFY(TCN_SELCHANGE,	IDC_TAB_CTRL, &CTrayMapDlg::OnTcnSelChangeTabView) 
	ON_BN_CLICKED(IDCANCEL, &CTrayMapDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTrayMapDlg 메시지 처리기입니다.


BOOL CTrayMapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString		strTabName[4]	= {"Loading Area", "Unloading Area", "Test Site", "Stacker Area"};//, "Hand"};
	CRect		rect;
	for(int i=0; i<4; i++){
		this->m_TabCtrl.InsertItem(i, strTabName[i], 0);
	}

	this->m_TabCtrl.GetClientRect(&rect);

	this->m_TabTrayViewLoad.Create(IDD_DIALOG_TRAYVIEW_LOAD,	&this->m_TabCtrl);
	this->m_TabTrayViewLoad.SetWindowPos(NULL, 5, 25, rect.Width() -10, rect.Height()-30, SWP_SHOWWINDOW |  SWP_NOZORDER);

	this->m_TabTrayViewUnload.Create(IDD_DIALOG_TRAYVIEW_UNLOAD,			&this->m_TabCtrl);
	this->m_TabTrayViewUnload.SetWindowPos(NULL, 5, 25, rect.Width() -10, rect.Height()-30, SWP_NOZORDER);

	this->m_TabTrayViewTestSite.Create(IDD_DIALOG_TRAYVIEW_TESTSITE,			&this->m_TabCtrl);
	this->m_TabTrayViewTestSite.SetWindowPos(NULL, 5, 25, rect.Width() -10, rect.Height()-30, SWP_NOZORDER);

	this->m_TabTrayViewStacker.Create(IDD_DIALOG_TRAYVIEW_STACKER, &this->m_TabCtrl);
	this->m_TabTrayViewStacker.SetWindowPos(NULL, 5, 25, rect.Width() - 10, rect.Height() - 30, SWP_NOZORDER);
	//this->m_TabTrayViewHand.Create(IDD_DIALOG_TRAYVIEW_HAND_TESTSITE,	&this->m_TabCtrl);
	//this->m_TabTrayViewHand.SetWindowPos(NULL, 5, 25, rect.Width()-10, rect.Height()-30, SWP_NOZORDER);

 	this->m_pWnd_Show = &this->m_TabTrayViewLoad;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTrayMapDlg::OnTcnSelchangingTabView(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(this->m_pWnd_Show != NULL)
	{
		this->m_pWnd_Show->ShowWindow(SW_HIDE);
		this->m_pWnd_Show = NULL;
	}

	int tabIndex = this->m_TabCtrl.GetCurSel();

	switch(tabIndex)
	{
	case 0: //Loading
		this->m_TabTrayViewLoad.ShowWindow(SW_SHOW);
		this->m_pWnd_Show = &this->m_TabTrayViewLoad;
		break;
	case 1: //Unloading
		this->m_TabTrayViewUnload.ShowWindow(SW_SHOW);
		this->m_pWnd_Show = &this->m_TabTrayViewUnload;
		break;
	case 2: // Test Site
		this->m_TabTrayViewTestSite.ShowWindow(SW_SHOW);
		this->m_pWnd_Show = &this->m_TabTrayViewTestSite;
		break;
	case 3:
		this->m_TabTrayViewStacker.ShowWindow(SW_SHOW);
		this->m_pWnd_Show = &this->m_TabTrayViewStacker;
		break;
// 	case 2: 
// 		this->m_TabTrayViewHand.ShowWindow(SW_SHOW);
// 		this->m_pWnd_Show = &this->m_TabTrayViewHand;
// 		break;
	}
	*pResult = 0;
}

/*==============================================================================
DESCRIPT : IF Monitor Menu Tab 변경 시 처리
KEY WORD : // By jhjang [150119] Add RS232C Log & Fix IF Monitor Code
ARGUMENT : None
RETURN   : None
==============================================================================*/
void CTrayMapDlg::OnTcnSelChangeTabView(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(this->m_pWnd_Show != NULL)
	{
		this->m_pWnd_Show->ShowWindow(SW_HIDE);
		this->m_pWnd_Show = NULL;
	}

	int tabIndex = this->m_TabCtrl.GetCurSel();

	switch(tabIndex)
	{
	case 0: 
		this->m_TabTrayViewLoad.ShowWindow(SW_SHOW);
		this->m_pWnd_Show = &this->m_TabTrayViewLoad;
		break;
	case 1: 
		this->m_TabTrayViewUnload.ShowWindow(SW_SHOW);
		this->m_pWnd_Show = &this->m_TabTrayViewUnload;
		break;
	case 2: 
		this->m_TabTrayViewTestSite.ShowWindow(SW_SHOW);
		this->m_pWnd_Show = &this->m_TabTrayViewTestSite;
		break;
	case 3:
		this->m_TabTrayViewStacker.ShowWindow(SW_SHOW);
		this->m_pWnd_Show = &this->m_TabTrayViewStacker;
		break;
// 	case 2: 
// 		this->m_TabTrayViewHand.ShowWindow(SW_SHOW);
// 		this->m_pWnd_Show = &this->m_TabTrayViewHand;
// 		break;
	}

	*pResult = 0;
}

void CTrayMapDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnCancel();
}
