// DlgSankyoManual.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgSankyoManual.h"
#include "afxdialogex.h"
#include "SankyoDrv_Imp.h"
#include <vector>
#include "SankyoMotorDrv.h"

// CDlgSankyoManual 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgSankyoManual, CDialogEx)

CDlgSankyoManual::CDlgSankyoManual(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSankyoManual::IDD, pParent)
{
	m_pOwnerWnd = NULL;
	m_pOwnerWnd = pParent;
}

CDlgSankyoManual::~CDlgSankyoManual()
{
}

void CDlgSankyoManual::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DRIVERS, m_listctrlMotorDriver);
}


BEGIN_MESSAGE_MAP(CDlgSankyoManual, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_AUTO_MANUAL, &CDlgSankyoManual::OnBnClickedButtonAutoManual)
	ON_BN_CLICKED(IDC_BUTTON_GET_TORQUE, &CDlgSankyoManual::OnBnClickedButton_Send_GetTorqueMsg)
	ON_MESSAGE(UM_SANKYO_LOG, OnReceiveLogMsg)
END_MESSAGE_MAP()


// CDlgSankyoManual 메시지 처리기입니다.


BOOL CDlgSankyoManual::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	((CSankyoDrv_Imp*)m_pOwnerWnd)->AddCommLogView(this->m_hWnd);

	int i;
	LVITEM lvItem;
	char szItemText[32]={0};

	CString strHeading;

	DWORD dwStyle = m_listctrlMotorDriver.GetExtendedStyle();
	dwStyle |= LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT;
	m_listctrlMotorDriver.SetExtendedStyle(dwStyle);

	//-- make column
	strHeading.Format("Column");
	m_listctrlMotorDriver.InsertColumn(0, (LPCTSTR)"Station", LVCFMT_CENTER, 50);
	m_listctrlMotorDriver.InsertColumn(1, (LPCTSTR)"Torque", LVCFMT_CENTER, 100);

	for( i=0; i<8; i++)
	{		
		m_listctrlMotorDriver.InsertItem(i,"");
		memset(szItemText,0,sizeof(szItemText));
		sprintf_s(szItemText, sizeof(szItemText), "%02d",i+1);

		lvItem.iItem    = i;
		lvItem.iSubItem = 0;
		lvItem.mask     = LVIF_TEXT;
		lvItem.pszText  = szItemText;

		m_listctrlMotorDriver.SetItem(&lvItem);
	}

	SetTimer(1, 500, NULL);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgSankyoManual::OnDestroy()
{
	CDialogEx::OnDestroy();

	((CSankyoDrv_Imp*)m_pOwnerWnd)->RemoveLogView(this->m_hWnd);

	KillTimer(1);
}


void CDlgSankyoManual::OnTimer(UINT_PTR nIDEvent)
{
	switch( nIDEvent )
	{
	case 1:
		{
			KillTimer(1);
			DisplayUpdate();
			SetTimer(1, 500, NULL);
		}break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CDlgSankyoManual::OnBnClickedButtonAutoManual()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlgSankyoManual::OnBnClickedButton_Send_GetTorqueMsg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nMark = m_listctrlMotorDriver.GetSelectionMark();
	if( nMark < 0 ) return;
	std::vector<unsigned short> sParam;
	sParam.push_back(Sankyo::_eStsCode_CommandTorque);
	((CSankyoDrv_Imp*)m_pOwnerWnd)->SendPacket(nMark+1, Sankyo::_eCmd_Code_GetStateValue2, sParam);	
}


void CDlgSankyoManual::DisplayUpdate()
{
	// Torque Value
	std::vector<double> vTorq;
	((CSankyoDrv_Imp*)m_pOwnerWnd)->GetTorqueAll(vTorq);
	if( vTorq.empty() ) return;
	for(std::vector<double>::size_type i=0; i<vTorq.size(); i++){
		CString strVal;
		strVal.Format("%.2f", vTorq[i]);
		m_listctrlMotorDriver.SetItemText(i,1,strVal);
	}

	// Heart Bit
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_AUTO_RUN_LED);
	int nCheck = ((CSankyoDrv_Imp*)m_pOwnerWnd)->GetAutoRunningHeartBit();
	pBtn->SetCheck(nCheck);
}

LRESULT CDlgSankyoManual::OnReceiveLogMsg(WPARAM wParam, LPARAM lParam)
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMM_LOG);
	if( pEdit->GetLineCount() > 20 ){
		pEdit->SetWindowText("");
	}

	CString strTimeString;
	GetTimeString( strTimeString );

	CString strTemp;
	pEdit->GetWindowText(strTemp);
	if( strTemp != "" ){
		strTemp += _T("\r\n");
	}
	strTemp += strTimeString + _T(" ");
	strTemp += (char*)lParam;
	pEdit->SetWindowText( strTemp );
	
	return 0L;
}

void CDlgSankyoManual::GetTimeString(CString& strTimeString)
{
	SYSTEMTIME t;
	::GetLocalTime(&t);
	strTimeString.Format("[%02d:%02d.%03d]", t.wMinute, t.wSecond, t.wMilliseconds);
}
