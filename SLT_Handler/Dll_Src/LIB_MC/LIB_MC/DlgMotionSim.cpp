// DlgMotionSim.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgMotionSim.h"
#include "afxdialogex.h"
#include "OneAxis_Sim.h"
#include "DlgInputNum.h"

// CDlgMotionSim 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgMotionSim, CDialogEx)

CDlgMotionSim::CDlgMotionSim(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMotionSim::IDD, pParent)
{
	m_nCurAxis = 0;
	m_nEditSimulItemId = 0;
	m_pOwner = NULL;
}

CDlgMotionSim::~CDlgMotionSim()
{
}

void CDlgMotionSim::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LIB_MC_MOTION_SIM_AXIS_SELECT, m_cbAxisSelect);
	DDX_Control(pDX, IDC_LIST_LIB_MC_MOTION_SIM_AXIS_STATE, m_listSimMotorState);
	DDX_Control(pDX, IDC_COMBO_SIMUL_RESPONSE_MODE, m_cbMotionSimulResponseMode);
}


BEGIN_MESSAGE_MAP(CDlgMotionSim, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_CONTEXTMENU()
	ON_CBN_SELCHANGE(IDC_COMBO_LIB_MC_MOTION_SIM_AXIS_SELECT, &CDlgMotionSim::OnCbnSelchangeComboLibMcMotionSimAxisSelect)
	ON_COMMAND(ID_DIO_ON, &CDlgMotionSim::OnDioOn)
	ON_COMMAND(ID_DIO_OFF, &CDlgMotionSim::OnDioOff)
	ON_COMMAND(ID_KEYIN_EDITVALUE, &CDlgMotionSim::OnKeyinEditvalue)
	ON_CBN_SELCHANGE(IDC_COMBO_SIMUL_RESPONSE_MODE, &CDlgMotionSim::OnCbnSelchangeComboSimulResponseMode)
END_MESSAGE_MAP()


// CDlgMotionSim 메시지 처리기입니다.



BOOL CDlgMotionSim::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CDlgMotionSim::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int nAxisCount = g_pMotion_Cont_Imp->GetAxisCount();
	for(int i=0; i<nAxisCount; i++){
		_tMOTOR_PARAM rAxisInfo;
		g_pMotion_Cont_Imp->GetMotorParam(i, rAxisInfo);
		//CString strAxisName = rAxisInfo.szCaption;
		CString strAxisName = rAxisInfo.strAxisName;
		m_cbAxisSelect.AddString(strAxisName);
	}
	m_cbAxisSelect.SetCurSel(0);
	InitListCtrl(m_listSimMotorState);

	m_cbMotionSimulResponseMode.AddString("0:zero time");
	m_cbMotionSimulResponseMode.AddString("1:real time");
	m_cbMotionSimulResponseMode.SetCurSel(g_pMotion_Cont_Imp->m_nSimulResponseMode);

	SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgMotionSim::InitListCtrl(CListCtrl& listCtrl)
{
	int i;
	CRect rect;
	LVITEM lvItem;
	CString strItemName[MAX_DISPLAY_MOTION_SIM_BIT_STATE] = {"INPOS", "DONE", "SERVO", "POWER", "ORG SEN", "+LMT SEN", "-LMT SEN", "SEQ", "AMP FAULT", "MOTION" };
	char    szItemName[128];

	listCtrl.GetWindowRect(&rect);
	listCtrl.InsertColumn(0, _T("NAME"), LVCFMT_CENTER, (rect.Width()-15)*10/10);

	DWORD dwStyle = listCtrl.GetExStyle();
	dwStyle |= LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES |LVS_EX_FULLROWSELECT;
	listCtrl.SetExtendedStyle(dwStyle);

	for(i=0; i<MAX_DISPLAY_MOTION_SIM_BIT_STATE; i++)
	{
		listCtrl.InsertItem(i,_T(""));

		lvItem.iItem = i;
		lvItem.iSubItem = 0;
		lvItem.mask = LVIF_TEXT;
		memset(szItemName, 0x00, sizeof(szItemName));
		sprintf_s(szItemName, sizeof(szItemName), "%s", strItemName[i].GetString() );
		lvItem.pszText = szItemName;
		listCtrl.SetItem(&lvItem);
		listCtrl.SetCheck(i, FALSE);
	}
}

void CDlgMotionSim::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(1);
}


void CDlgMotionSim::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CDlgMotionSim::UpdateMotionSimulData()
{
	OneAxis_Sim* pMotor = NULL;
	pMotor = (OneAxis_Sim*)g_pMotion_Cont_Imp->GetAxis(m_nCurAxis);
	if( pMotor )
	{
		for(int i=0; i<MAX_DISPLAY_MOTION_SIM_BIT_STATE; i++){
			BOOL bValue = pMotor->m_SimMotorState.GetBitSts(i);		
			m_listSimMotorState.SetCheck(i, bValue);
		}	
		CString strVal;
		double dPos;
		strVal.Format("%f", pMotor->GetCommandPos(dPos) );
		SetDlgItemText(IDC_STATIC_LIB_MC_MOTION_SIM_TPOS, strVal);

		strVal.Format("%f", pMotor->GetSpeed() );
		SetDlgItemText(IDC_STATIC_LIB_MC_MOTION_SIM_TSPD, strVal);

		strVal.Format("%d", pMotor->GetAcc() );
		SetDlgItemText(IDC_STATIC_LIB_MC_MOTION_SIM_TACC, strVal);

		strVal.Format("%d", pMotor->GetDcc() );
		SetDlgItemText(IDC_STATIC_LIB_MC_MOTION_SIM_TDCC, strVal);

		strVal.Format("%f", pMotor->m_SimMotorState.dCurPos );
		SetDlgItemText(IDC_STATIC_LIB_MC_MOTION_SIM_CPOS, strVal);

		strVal.Format("%f", pMotor->m_SimMotorState.dCurSpeed );
		SetDlgItemText(IDC_STATIC_LIB_MC_MOTION_SIM_CSPD, strVal);

		strVal.Format("%d", pMotor->m_SimMotorState.nCurAcc );
		SetDlgItemText(IDC_STATIC_LIB_MC_MOTION_SIM_CACC, strVal);

		strVal.Format("%d", pMotor->m_SimMotorState.nCurDcc );
		SetDlgItemText(IDC_STATIC_LIB_MC_MOTION_SIM_CDCC, strVal);
	}
}

void CDlgMotionSim::OnTimer(UINT_PTR nIDEvent)
{
	switch( nIDEvent )
	{
	case 1:
		{
			KillTimer(1);
			UpdateMotionSimulData();
			SetTimer(1, 500, NULL);
		}break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CDlgMotionSim::OnContextMenu(CWnd* pWnd, CPoint ptMousePos)
{
	// if Shift-F10
	if (ptMousePos.x == -1 && ptMousePos.y == -1)
	{
		ptMousePos = (CPoint) GetMessagePos();
	}
	ScreenToClient(&ptMousePos);	

	CMenu menu;
	CMenu* pPopup;

	CWnd* pWndSimData_CurPos = (CWnd*)GetDlgItem(IDC_STATIC_LIB_MC_MOTION_SIM_CPOS);
	CWnd* pWndSimData_CurSpd = (CWnd*)GetDlgItem(IDC_STATIC_LIB_MC_MOTION_SIM_CSPD);
	CWnd* pWndSimData_CurAcc = (CWnd*)GetDlgItem(IDC_STATIC_LIB_MC_MOTION_SIM_CACC);
	CWnd* pWndSimData_CurDcc = (CWnd*)GetDlgItem(IDC_STATIC_LIB_MC_MOTION_SIM_CDCC);

	int nPos=0;
	if( pWnd == (CWnd*)GetDlgItem(IDC_LIST_LIB_MC_MOTION_SIM_AXIS_STATE)){
		nPos = 0;
	}else if( pWnd == pWndSimData_CurPos){
		nPos = 1;
		m_nEditSimulItemId = eEditSimulItemId_CPos;
	}else if( pWnd == pWndSimData_CurSpd){
		nPos = 1;
		m_nEditSimulItemId = eEditSimulItemId_CSpd;
	}else if( pWnd == pWndSimData_CurAcc){
		nPos = 1;
		m_nEditSimulItemId = eEditSimulItemId_CAcc;
	}else if( pWnd == pWndSimData_CurDcc){
		nPos = 1;
		m_nEditSimulItemId = eEditSimulItemId_CDcc;
	}else{
		return;
	}
	menu.LoadMenu(IDR_MENU_LIB_MC);
	pPopup = menu.GetSubMenu(nPos);
	ClientToScreen(&ptMousePos);
	pPopup->TrackPopupMenu( TPM_LEFTALIGN, ptMousePos.x, ptMousePos.y, this );
}


void CDlgMotionSim::OnCbnSelchangeComboLibMcMotionSimAxisSelect()
{
	if( m_cbAxisSelect.GetCurSel() >= 0 ){
		m_nCurAxis = m_cbAxisSelect.GetCurSel();
	}
}


void CDlgMotionSim::StatusOnOff(BOOL bOnOff)
{
	int nMark = m_listSimMotorState.GetSelectionMark();
	if( nMark < 0 ){
		return;
	}
	OneAxis_Sim* pMotor = NULL;
	pMotor = (OneAxis_Sim*)g_pMotion_Cont_Imp->GetAxis(m_nCurAxis);
	if( pMotor )
	{
		pMotor->m_SimMotorState.SetBitSts(nMark, bOnOff);
	}
}

void CDlgMotionSim::OnDioOn()
{
	StatusOnOff(TRUE);
}


void CDlgMotionSim::OnDioOff()
{
	StatusOnOff(FALSE);
}


void CDlgMotionSim::OnKeyinEditvalue()
{
	CDlgInputNum dlg;
	int nRet = dlg.DoModal();
	if( nRet == IDOK )
	{
		CString strVal;
		dlg.GetValue(strVal);

		double dValue = atof( strVal );

		OneAxis_Sim* pMotor = NULL;
		pMotor = (OneAxis_Sim*)g_pMotion_Cont_Imp->GetAxis(m_nCurAxis);
		if( pMotor )
		{
			switch( m_nEditSimulItemId )
			{
			case eEditSimulItemId_CPos: pMotor->m_SimMotorState.dCurPos = dValue; break;
			case eEditSimulItemId_CSpd: pMotor->m_SimMotorState.dCurSpeed = dValue; break;
			case eEditSimulItemId_CAcc: pMotor->m_SimMotorState.nCurAcc = (int)dValue; break;
			case eEditSimulItemId_CDcc: pMotor->m_SimMotorState.nCurDcc = (int)dValue; break;
			}			
		}
	}
}


void CDlgMotionSim::OnCbnSelchangeComboSimulResponseMode()
{
	g_pMotion_Cont_Imp->m_nSimulResponseMode = m_cbMotionSimulResponseMode.GetCurSel();
}
