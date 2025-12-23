// DlgDIOSimul.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgDIOSimul.h"
#include "afxdialogex.h"


// CDlgDIOSimul 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgDIOSimul, CDialogEx)

CDlgDIOSimul::CDlgDIOSimul(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgDIOSimul::IDD, pParent)
{

}

CDlgDIOSimul::~CDlgDIOSimul()
{
}

void CDlgDIOSimul::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SIMUL_DI, m_listSimulDi);
	DDX_Control(pDX, IDC_LIST_SIMUL_DO, m_listSimulDo);
}


BEGIN_MESSAGE_MAP(CDlgDIOSimul, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_DIO_ON, &CDlgDIOSimul::OnDioOn)
	ON_COMMAND(ID_DIO_OFF, &CDlgDIOSimul::OnDioOff)
END_MESSAGE_MAP()


// CDlgDIOSimul 메시지 처리기입니다.


BOOL CDlgDIOSimul::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgDIOSimul::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitListCtrl(m_listSimulDi, _tDIO_LS::eDI);
	InitListCtrl(m_listSimulDo, _tDIO_LS::eDO);

	SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgDIOSimul::OnDestroy()
{
	CDialogEx::OnDestroy();
		
	KillTimer(1);
}


void CDlgDIOSimul::OnTimer(UINT_PTR nIDEvent)
{
	switch( nIDEvent )
	{
	case 1:
		{
			KillTimer(1);
			UpdateIoValue(m_listSimulDi, 0);
			UpdateIoValue(m_listSimulDo, 1);
			SetTimer(1, 100, NULL);
		}break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CDlgDIOSimul::InitListCtrl(CListCtrl& listCtrl, int nInOrOut)
{
	int i;
	CRect rect;
	LVITEM lvItem;
	CString strItemName;
	char    szItemName[128];

	listCtrl.GetWindowRect(&rect);
	listCtrl.InsertColumn(0, _T("No"), LVCFMT_CENTER, (rect.Width()-15)*2/10);
	listCtrl.InsertColumn(1, _T("Label"), LVCFMT_CENTER, (rect.Width()-15)*2/10);
	listCtrl.InsertColumn(2, _T("Name"), LVCFMT_CENTER, (rect.Width()-15)*6/10);

	DWORD dwStyle = listCtrl.GetExStyle();
	dwStyle |= LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT;
	listCtrl.SetExtendedStyle(dwStyle);

	int nPointMax = g_pDioSimDrv->GetPointMax(nInOrOut);
	for(i=0; i<nPointMax; i++)
	{
		listCtrl.InsertItem(i,_T(""));

		_tDIO_LS stIoInfo;
		g_pDIOContImp->GetIoInfo(nInOrOut, i, stIoInfo);

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
		stIoInfo.name.TrimLeft();
		stIoInfo.name.TrimRight();

// 		stIoInfo.label.TrimLeft();
// 		stIoInfo.label.TrimRight();

		sprintf_s(szItemName, sizeof(szItemName), "%s", stIoInfo.label.GetString() );
		lvItem.pszText = szItemName;
		listCtrl.SetItem(&lvItem);

		// Name
		lvItem.iItem = i;
		lvItem.iSubItem = 2;
		lvItem.mask = LVIF_TEXT;
		memset(szItemName, 0x00, sizeof(szItemName));
		stIoInfo.name.TrimLeft();
		stIoInfo.name.TrimRight();
		sprintf_s(szItemName, sizeof(szItemName), "%s", stIoInfo.name.GetString() );
		lvItem.pszText = szItemName;
		listCtrl.SetItem(&lvItem);
	}
}

void CDlgDIOSimul::UpdateIoValue(CListCtrl& listCtrl, int nInOrOut)
{
	int nPointMax = g_pDioSimDrv->GetPointMax(nInOrOut);
	for(int i=0; i<nPointMax; i++)
	{
		char szValue[128]={0,};
		BOOL bValue = FALSE;
		if( nInOrOut == _tDIO_LS::eDI ){
			bValue = g_pDioSimDrv->GetSimDataDi(i);
		}else{
			bValue = g_pDioSimDrv->GetSimDataDo(i);
		}
		
		listCtrl.SetCheck(i, bValue);
	}
}


void CDlgDIOSimul::OnContextMenu(CWnd* pWnd, CPoint ptMousePos)
{
	// if Shift-F10
	if (ptMousePos.x == -1 && ptMousePos.y == -1)
	{
		ptMousePos = (CPoint) GetMessagePos();
	}
	ScreenToClient(&ptMousePos);	

	CMenu menu;
	CMenu* pPopup;

	CWnd* pWndDiList = (CWnd*)GetDlgItem(IDC_LIST_SIMUL_DI);

	int nPos=0;
	if( pWnd == pWndDiList){
		nPos = 0;
	}else{
		return;
	}
	menu.LoadMenu(IDR_MENU_LIB_MC);
	pPopup = menu.GetSubMenu(nPos);
	ClientToScreen(&ptMousePos);
	pPopup->TrackPopupMenu( TPM_LEFTALIGN, ptMousePos.x, ptMousePos.y, this );
}


void CDlgDIOSimul::OnDioOn()
{
	int nMark = m_listSimulDi.GetSelectionMark();
	if( nMark < 0 ){
		return;
	}
	g_pDioSimDrv->SetSimDataDi(nMark, TRUE);
}


void CDlgDIOSimul::OnDioOff()
{
	int nMark = m_listSimulDi.GetSelectionMark();
	if( nMark < 0 ){
		return;
	}
	g_pDioSimDrv->SetSimDataDi(nMark, FALSE);
}
