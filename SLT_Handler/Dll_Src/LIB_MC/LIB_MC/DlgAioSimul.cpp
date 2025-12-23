// DlgAioSimul.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgAioSimul.h"
#include "afxdialogex.h"
#include "DlgInputNum.h"

// CDlgAioSimul 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgAioSimul, CDialogEx)

CDlgAioSimul::CDlgAioSimul(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAioSimul::IDD, pParent)
{
}

CDlgAioSimul::~CDlgAioSimul()
{
}

void CDlgAioSimul::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SIMUL_AI, m_listSimulAI);
	DDX_Control(pDX, IDC_LIST_SIMUL_AO, m_listSimulAO);
}


BEGIN_MESSAGE_MAP(CDlgAioSimul, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_SIM_VALUE_INPUT, &CDlgAioSimul::OnBnClickedButtonChangeSimValueInput)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_SIM_VALUE_OUTPUT, &CDlgAioSimul::OnBnClickedButtonChangeSimValueOutput)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_KEYIN_EDITVALUE, &CDlgAioSimul::OnKeyinEditvalue)
END_MESSAGE_MAP()


// CDlgAioSimul 메시지 처리기입니다.


BOOL CDlgAioSimul::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDlgAioSimul::InitControl()
{
	InitListCtrl(m_listSimulAI, 0);
	InitListCtrl(m_listSimulAO, 1);
}

void CDlgAioSimul::UpdateValue(CListCtrl& listCtrl, int nInOrOut)
{
	int nPointMax = g_pAioSimDrv->GetPointMax(nInOrOut);
	for(int i=0; i<nPointMax; i++)
	{
		char szValue[128]={0,};
		double dValue=0.0;
		if( nInOrOut == _tAIO_LS::eAI ){
			dValue = g_pAioSimDrv->GetSimDataAi(i);
		}else{
			dValue = g_pAioSimDrv->GetSimDataAo(i);
		}
		sprintf_s(szValue, sizeof(szValue),"%f", dValue);
		listCtrl.SetItemText(i,2,szValue);
	}
}


void CDlgAioSimul::InitListCtrl(CListCtrl& listCtrl, int nInOrOut)
{
	int i;
	CRect rect;
	LVITEM lvItem;
	CString strItemName;
	char    szItemName[128];

	listCtrl.GetWindowRect(&rect);
	listCtrl.InsertColumn(0, _T("No"), LVCFMT_CENTER, (rect.Width()-15)*2/10);
	listCtrl.InsertColumn(1, _T("Label"), LVCFMT_CENTER, (rect.Width()-15)*2/10);
	listCtrl.InsertColumn(2, _T("Name"), LVCFMT_CENTER, (rect.Width()-15)*4/10);
	listCtrl.InsertColumn(3, _T("Value"), LVCFMT_CENTER, (rect.Width()-15)*2/10);

	DWORD dwStyle = listCtrl.GetExStyle();
	dwStyle |= LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT;
	listCtrl.SetExtendedStyle(dwStyle);

	int nPointMax = g_pAioSimDrv->GetPointMax(nInOrOut);
	for(i=0; i<nPointMax; i++)
	{
		listCtrl.InsertItem(i,_T(""));

		_tAIO_LS stAioDbInfo;
		g_pAioContImp->GetIoDbInfo(nInOrOut, i, &stAioDbInfo);

		// No.
		lvItem.iItem = i;
		lvItem.iSubItem = 0;
		lvItem.mask = LVIF_TEXT;
		memset(szItemName, 0x00, sizeof(szItemName));
		sprintf_s(szItemName, sizeof(szItemName), "%d", i);
		lvItem.pszText = szItemName;
		listCtrl.SetItem(&lvItem);

		// Label
		lvItem.iItem = i;
		lvItem.iSubItem = 1;
		lvItem.mask = LVIF_TEXT;
		memset(szItemName, 0x00, sizeof(szItemName));
		stAioDbInfo.strName.TrimLeft();
		stAioDbInfo.strName.TrimRight();
		sprintf_s(szItemName, sizeof(szItemName), "%s", stAioDbInfo.strLabel.GetString() );
		lvItem.pszText = szItemName;
		listCtrl.SetItem(&lvItem);

		// Name
		lvItem.iItem = i;
		lvItem.iSubItem = 2;
		lvItem.mask = LVIF_TEXT;
		memset(szItemName, 0x00, sizeof(szItemName));
		stAioDbInfo.strName.TrimLeft();
		stAioDbInfo.strName.TrimRight();
		sprintf_s(szItemName, sizeof(szItemName), "%s", stAioDbInfo.strName.GetString() );
		lvItem.pszText = szItemName;
		listCtrl.SetItem(&lvItem);

		// Value
		lvItem.iItem = i;
		lvItem.iSubItem = 3;
		lvItem.mask = LVIF_TEXT;
		memset(szItemName, 0x00, sizeof(szItemName));
		sprintf_s(szItemName, sizeof(szItemName), "%d", 0);
		lvItem.pszText = szItemName;
		listCtrl.SetItem(&lvItem);
	}
}

BOOL CDlgAioSimul::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitControl();

	SetTimer(1, 500, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgAioSimul::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(1);
}


void CDlgAioSimul::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch( nIDEvent )
	{
	case 1:
		{
			KillTimer(1);
			UpdateValue(m_listSimulAI, 0);
			UpdateValue(m_listSimulAO, 1);
			SetTimer(1, 500, NULL);
		}break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CDlgAioSimul::OnBnClickedButtonChangeSimValueInput()
{
	char szValue[128]={0,};
	GetDlgItemText(IDC_EDIT_SIM_VALUE_INPUT, szValue, sizeof(szValue));
	double dValue = atof(szValue);
	int nIdx = m_listSimulAI.GetSelectionMark();
	if( nIdx >= 0 ){
		g_pAioSimDrv->SetSimDataAi(nIdx, dValue);
	}
}


void CDlgAioSimul::OnBnClickedButtonChangeSimValueOutput()
{
	char szValue[128]={0,};
	GetDlgItemText(IDC_EDIT_SIM_VALUE_OUTPUT, szValue, sizeof(szValue));
	double dValue = atof(szValue);
	int nIdx = m_listSimulAO.GetSelectionMark();
	if( nIdx >= 0 ){
		g_pAioSimDrv->SetSimDataAo(nIdx, dValue);
	}
}


void CDlgAioSimul::OnContextMenu(CWnd* pWnd, CPoint ptMousePos)
{
	// if Shift-F10
	if (ptMousePos.x == -1 && ptMousePos.y == -1)
	{
		ptMousePos = (CPoint) GetMessagePos();
	}
	ScreenToClient(&ptMousePos);	

	CMenu menu;
	CMenu* pPopup;

	int nPos=0;
	if( pWnd == (CWnd*)GetDlgItem(IDC_LIST_SIMUL_AI)){
		nPos = 1;
	}else{
		return;
	}
	menu.LoadMenu(IDR_MENU_LIB_MC);
	pPopup = menu.GetSubMenu(nPos);
	ClientToScreen(&ptMousePos);
	pPopup->TrackPopupMenu( TPM_LEFTALIGN, ptMousePos.x, ptMousePos.y, this );
}

void CDlgAioSimul::EditSimulData()
{
	int nMark = m_listSimulAI.GetSelectionMark();
	if( nMark < 0 ){
		return;
	}
	CDlgInputNum dlg;
	int nRet = dlg.DoModal();
	if( nRet == IDOK )
	{
		CString strVal;
		dlg.GetValue(strVal);
		double dValue = atof( strVal );
		g_pAioSimDrv->SetSimDataAi(nMark, dValue);
	}
}


void CDlgAioSimul::OnKeyinEditvalue()
{
	EditSimulData();
}
