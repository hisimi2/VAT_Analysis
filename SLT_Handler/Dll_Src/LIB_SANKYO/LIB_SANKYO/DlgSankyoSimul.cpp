// DlgSankyoSimul.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgSankyoSimul.h"
#include "afxdialogex.h"
#include "DlgInputNum.h"
#include "SankyoDrv_Imp.h"

// CDlgSankyoSimul 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgSankyoSimul, CDialogEx)

CDlgSankyoSimul::CDlgSankyoSimul(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSankyoSimul::IDD, pParent)
{
	m_pOwnerWnd = NULL;
	m_pOwnerWnd = pParent;
	for(int i=0; i<Sankyo::MAX_STATION_COUNT; i++){
		m_vTorque.push_back(999.0);
	}
}

CDlgSankyoSimul::~CDlgSankyoSimul()
{
}

void CDlgSankyoSimul::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DRIVERS_FOR_SIMUL, m_listctrlMotorDriver);
}


BEGIN_MESSAGE_MAP(CDlgSankyoSimul, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DRIVERS_FOR_SIMUL, &CDlgSankyoSimul::OnNMDblclkListDriversForSimul)
	ON_BN_CLICKED(IDOK, &CDlgSankyoSimul::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDlgSankyoSimul::OnBnClickedButtonApply)
END_MESSAGE_MAP()


// CDlgSankyoSimul 메시지 처리기입니다.


BOOL CDlgSankyoSimul::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	int i;
	LVITEM lvItem;
	char szItemText[32]={0};

	CString strHeading;

	DWORD dwStyle = m_listctrlMotorDriver.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT;
	m_listctrlMotorDriver.SetExtendedStyle(dwStyle);

	//-- make column
	strHeading.Format("Column");
	m_listctrlMotorDriver.InsertColumn(0, (LPCTSTR)"Station", LVCFMT_CENTER, 50);
	m_listctrlMotorDriver.InsertColumn(1, (LPCTSTR)"Torque", LVCFMT_CENTER, 100);

	for( i=0; i<Sankyo::MAX_STATION_COUNT; i++)
	{		
		m_listctrlMotorDriver.InsertItem(i,"");
		memset(szItemText,0,sizeof(szItemText));
		sprintf_s(szItemText, sizeof(szItemText), "%02d",i+1);

		lvItem.iItem    = i;
		lvItem.iSubItem = 0;
		lvItem.mask     = LVIF_TEXT;
		lvItem.pszText  = szItemText;
		m_listctrlMotorDriver.SetItem(&lvItem);

		memset(szItemText,0,sizeof(szItemText));
		sprintf_s(szItemText, sizeof(szItemText), "%.2f",m_vTorque[i]);
		m_listctrlMotorDriver.SetItemText(i, 1, szItemText);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgSankyoSimul::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CDlgSankyoSimul::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::OnNotify(wParam, lParam, pResult);
}


BOOL CDlgSankyoSimul::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgSankyoSimul::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}


void CDlgSankyoSimul::OnNMDblclkListDriversForSimul(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nMark = m_listctrlMotorDriver.GetSelectionMark();

	char szValue[128]={0,};
	m_listctrlMotorDriver.GetItemText(nMark,1,szValue,sizeof(szValue));

	CDlgInputNum dlg(atoi(szValue), this);
	if( dlg.DoModal() == IDOK ){
		dlg.GetNum(szValue, sizeof(szValue));
		m_listctrlMotorDriver.SetItemText(nMark,1,szValue);
		m_vTorque[nMark] = atof(szValue);
	}
	*pResult = 0;
}


void CDlgSankyoSimul::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CDlgSankyoSimul::OnBnClickedButtonApply()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for(int i=0; i<Sankyo::MAX_STATION_COUNT; i++){
		char szValue[128]={0,};
		m_listctrlMotorDriver.GetItemText(i, 1, szValue, sizeof(szValue));
		m_vTorque[i] = atof(szValue);
	}
}

double CDlgSankyoSimul::GetTorque(int nStation)
{
	ASSERT(nStation>0 && nStation<=Sankyo::MAX_STATION_COUNT);
	return m_vTorque[nStation-1];
}
