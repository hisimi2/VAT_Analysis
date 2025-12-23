// DlgManuTrayPP.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgManuTrayPP.h"
#include "afxdialogex.h"

#include "../Include/OcxFiles/dsspanel.h"
// CDlgManuTrayPP 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgManuTrayPP, CDialogEx)

CDlgManuTrayPP::CDlgManuTrayPP(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgManuTrayPP::IDD, pParent)
{
	m_nCylManualOpActState = eCylManualOper_None;
	m_nCylManualActionStep = 0;
	m_nCmdCycleCount = 0;
	m_dTimeCheck = 0.0;

	m_nCmdManualCyl = 0;
	m_nCmdOperation = 0;
	m_nCmdParam_Time = 0;
	m_dCylActionTimeRecord = 0.0;
	m_nCylActionTimeRetry = 0;
}

CDlgManuTrayPP::~CDlgManuTrayPP()
{
	m_nCmdManualCyl = eCylManualCmd_None;
	m_nCmdParam_Time = 0;
	m_vCmdParam_Picker.clear();

	m_cBmpExit.DeleteObject();
}

void CDlgManuTrayPP::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_TRAY_PP_ALL_VACUUM, m_BtnVacAll);
	DDX_Control(pDX, IDC_BUTTON_TRAY_PP_ALL_BLOW, m_BtnBlwAll);
	DDX_Control(pDX, IDC_BUTTON_TRAY_PP_HAND_VACUUM, m_BtnVacEach);
	DDX_Control(pDX, IDC_BUTTON_TRAY_PP_HAND_BLOW, m_BtnBlwEach);
// 	DDX_Control(pDX, IDC_BUTTON_TRAY_PP_ALL_UP, m_BtnUpAll);
// 	DDX_Control(pDX, IDC_BUTTON_TRAY_PP_ALL_DOWN, m_BtnAllDown);
// 	DDX_Control(pDX, IDC_BUTTON_TRAY_PP_HAND_UP, m_BtnUpEach);
// 	DDX_Control(pDX, IDC_BUTTON_TRAY_PP_HAND_DOWN, m_BtnDownEach);
	DDX_Control(pDX, IDC_COMBO_TRAY_PP_OPERATION, m_cbOperationSel);
	DDX_Control(pDX, IDC_COMBO_TRAY_PP_CONTINUE_TIME, m_cbTimeSel);
	DDX_Control(pDX, IDC_COMBO_TRAY_PP_SEL, m_cbPickerSel);
	DDX_Control(pDX, IDC_BUTTON_TRAY_PP_CYL_ACT_STOP, m_BtnStop);

	// exit
	DDX_Control(pDX, IDOK, m_btnExit);
}


BEGIN_MESSAGE_MAP(CDlgManuTrayPP, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
//	ON_BN_CLICKED(IDC_BUTTON_TRAY_PP_ALL_UP, &CDlgManuTrayPP::OnBnClickedButtonTrayPpAllUp)
//	ON_BN_CLICKED(IDC_BUTTON_TRAY_PP_ALL_DOWN, &CDlgManuTrayPP::OnBnClickedButtonTrayPpAllDown)
//	ON_BN_CLICKED(IDC_BUTTON_TRAY_PP_HAND_UP, &CDlgManuTrayPP::OnBnClickedButtonTrayPpHandUp)
//	ON_BN_CLICKED(IDC_BUTTON_TRAY_PP_HAND_DOWN, &CDlgManuTrayPP::OnBnClickedButtonTrayPpHandDown)
	ON_BN_CLICKED(IDC_BUTTON_TRAY_PP_ALL_VACUUM, &CDlgManuTrayPP::OnBnClickedButtonTrayPpAllVacuum)
	ON_BN_CLICKED(IDC_BUTTON_TRAY_PP_ALL_BLOW, &CDlgManuTrayPP::OnBnClickedButtonTrayPpAllBlow)
	ON_BN_CLICKED(IDC_BUTTON_TRAY_PP_HAND_VACUUM, &CDlgManuTrayPP::OnBnClickedButtonTrayPpHandVacuum)
	ON_BN_CLICKED(IDC_BUTTON_TRAY_PP_HAND_BLOW, &CDlgManuTrayPP::OnBnClickedButtonTrayPpHandBlow)
	ON_BN_CLICKED(IDC_BUTTON_TRAY_PP_CYL_ACT_STOP, &CDlgManuTrayPP::OnBnClickedButtonTrayPpCylActStop)
	ON_BN_CLICKED(IDOK, &CDlgManuTrayPP::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgManuTrayPP 메시지 처리기입니다.

BOOL CDlgManuTrayPP::PreTranslateMessage(MSG* pMsg)
{
	if((pMsg->message==WM_SYSKEYDOWN) && (pMsg->wParam==VK_F4) )
	{
		return TRUE;
	}
	if((pMsg->message==WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		return TRUE;
	}
	if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE) )
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CDlgManuTrayPP::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// exit
	m_cBmpExit.LoadBitmap(IDB_EXIT_CYLINDER);
	m_btnExit.SetBitmap(m_cBmpExit);

	CString strTitle = _T("");
	strTitle.Format("Tray PP #%d", m_nHandIdx + 1);
	::SetWindowText(this->m_hWnd, strTitle);

	SetDlgItemText(IDC_STATIC_HEAD, strTitle);

	m_cbOperationSel.Clear();
	m_cbOperationSel.AddString("1: Only One Action & No Check Sensor");
	m_cbOperationSel.AddString("2: Only One Action & Check Sensor");
	m_cbOperationSel.AddString("3: Continue Action & No Check Sensor");
	m_cbOperationSel.AddString("4: Continue Action & Check Sensor");
	m_cbOperationSel.AddString("5: Time Check Action (Once & Sensor Check)");
	m_cbOperationSel.SetCurSel(0);

	m_cbPickerSel.Clear();
	m_cbPickerSel.AddString("1: y1 x1" );
	m_cbPickerSel.AddString("2: y1 x2");
	m_cbPickerSel.AddString("3: y1 x3");
	m_cbPickerSel.AddString("4: y1 x4");
	m_cbPickerSel.AddString("5: y2 x1");
	m_cbPickerSel.AddString("6: y2 x2");
	m_cbPickerSel.AddString("7: y2 x3");
	m_cbPickerSel.AddString("8: y2 x4");
	m_cbPickerSel.AddString( "9: Front All" );
	m_cbPickerSel.AddString( "10: Rear All" );
	m_cbPickerSel.SetCurSel(0);

	m_mapPickerSel.SetAt(ePickerSel_y1_x1, CPoint(0,0));
	m_mapPickerSel.SetAt(ePickerSel_y1_x2, CPoint(1,0));
	m_mapPickerSel.SetAt(ePickerSel_y1_x3, CPoint(2,0));
	m_mapPickerSel.SetAt(ePickerSel_y1_x4, CPoint(3,0));
	m_mapPickerSel.SetAt(ePickerSel_y2_x1, CPoint(0,1));
	m_mapPickerSel.SetAt(ePickerSel_y2_x2, CPoint(1,1));
	m_mapPickerSel.SetAt(ePickerSel_y2_x3, CPoint(2,1));
	m_mapPickerSel.SetAt(ePickerSel_y2_x4, CPoint(3,1));

	m_cbTimeSel.SetCurSel(0);

	DisplayHandSenIdx();

	SetBtnDisplay();
	SetTimer(0, DEF_SENSOR_DISP_TM, NULL);

	CreateThread(0);
	
	MakeLog("Tray PP Cylinder & Ejector & Sensor Dialog Show");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgManuTrayPP::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(0);
	CloseThread();
}


void CDlgManuTrayPP::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}



void CDlgManuTrayPP::OnTimer(UINT_PTR nIDEvent)
{
	CDSSPanel*   chkbt    = NULL;

	if(nIDEvent == 0)
	{
		KillTimer(nIDEvent);

		// Vacuum Sensor display
		int nSenID[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = {
			/*REAR*/	   {IDC_COLORCHECKBTCTRL1, IDC_COLORCHECKBTCTRL2, IDC_COLORCHECKBTCTRL3, IDC_COLORCHECKBTCTRL4}
			/*FRONT*/	 ,{IDC_COLORCHECKBTCTRL9, IDC_COLORCHECKBTCTRL10,IDC_COLORCHECKBTCTRL11,IDC_COLORCHECKBTCTRL12}};

		CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
		for( int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++ ) {
			for( int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++ ){
				chkbt = ( CDSSPanel* )GetDlgItem( nSenID[ nY ][ nX ] );		
				if( pTaskTrayPP[m_nHandIdx]->GetVacuum(CPoint(nX, nY)) == eOn) {
					chkbt->On();
				}
				else {
					chkbt->Off();
				}
			}
		}

		// Safty
		chkbt = (CDSSPanel*)GetDlgItem(IDC_COLORCHECKBTCTRL_TRAYPP_Z_SAFETY);
		bool bSafetySenZ = (m_nHandIdx == eTrayPP_1_LD) ? g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisZ)->IsHomeSensorOn() : g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisZ)->IsHomeSensorOn();
		if (bSafetySenZ == true) { chkbt->On(); }
		else { chkbt->Off(); }

		chkbt = (CDSSPanel*)GetDlgItem(IDC_COLORCHECKBTCTRL_TRAYPP_Y_SAFETY);
		if (g_TaskTrayUnloadPP.get_TrayUnload_Y_Safety_sen(DEF_ON)) { chkbt->On(); }
		else { chkbt->Off(); }

		//-- sequence busy display
		chkbt = (CDSSPanel*)GetDlgItem(IDC_COLORCHECKBTCTRL_BUSY);
		if( m_nCylManualOpActState != eCylManualOper_None ){
			chkbt->On();
		}else{
			chkbt->Off();
		}

		SetTimer(0, DEF_SENSOR_DISP_TM, NULL);
	}

	CDialogEx::OnTimer(nIDEvent);
}

// void CDlgManuTrayPP::OnBnClickedButtonTrayPpAllUp()
// {
// 	ButtonClickLog(eClickBtn_All_Up);
// 	SetCommand(eCylManualCmd_Picker_AllUp);
// }


// void CDlgManuTrayPP::OnBnClickedButtonTrayPpAllDown()
// {
// 	ButtonClickLog(eClickBtn_All_Down);
// 	SetCommand(eCylManualCmd_Picker_AllDown);
// }

void CDlgManuTrayPP::GetPickerSel(std::vector<CPoint>& rSelectedPicker)
{
	int nCurSel = m_cbPickerSel.GetCurSel();
	if( nCurSel < 0 ){
		return;
	}
	if( nCurSel < ePickerSel_Front_All ){
		CPoint SelPicker;
		m_mapPickerSel.Lookup(nCurSel, SelPicker);
		rSelectedPicker.push_back(SelPicker);
	}else if( nCurSel == ePickerSel_Front_All ){ 
		for(int nX=0; nX<MAX_TRAY_PP_CNT_X; nX++){
			rSelectedPicker.push_back(CPoint(nX, 1));
		}
	}else if( nCurSel == ePickerSel_Rear_All ){ 
		for( int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++ ) {
			rSelectedPicker.push_back( CPoint( nX, 0 ) );
		}
	}else{
	}
}

// void CDlgManuTrayPP::OnBnClickedButtonTrayPpHandUp()
// {
// 	ButtonClickLog(eClickBtn_Each_Up);
// 	SetCommand(eCylManualCmd_Picker_EachUp);
// }


// void CDlgManuTrayPP::OnBnClickedButtonTrayPpHandDown()
// {
// 	ButtonClickLog(eClickBtn_Each_Down);
// 	SetCommand(eCylManualCmd_Picker_EachDown);
// }


void CDlgManuTrayPP::OnBnClickedButtonTrayPpAllVacuum()
{
	ButtonClickLog(eClickBtn_All_Vacuum);
	CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
	int nErr = pTaskTrayPP[m_nHandIdx]->do_VacBlow_All(eVacBlwSt_VacOn);
	if( nErr != ERR_NO_ERROR ){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, NULL);
	}
}


void CDlgManuTrayPP::OnBnClickedButtonTrayPpAllBlow()
{
	ButtonClickLog(eClickBtn_All_Blow);
	CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
	int nErr = pTaskTrayPP[m_nHandIdx]->do_VacBlow_All(eVacBlwSt_BlwOn);
	if( nErr != ERR_NO_ERROR ){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, NULL);
	}
}


void CDlgManuTrayPP::OnBnClickedButtonTrayPpHandVacuum()
{
	std::vector<CPoint> vPicker;
	GetPickerSel(vPicker);

	ButtonClickLog(eClickBtn_Each_Vacuum);
	CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
	int nErr = pTaskTrayPP[m_nHandIdx]->do_VacBlow(eVacBlwSt_VacOn, vPicker);
	if( nErr != ERR_NO_ERROR ){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, NULL);
	}
}


void CDlgManuTrayPP::OnBnClickedButtonTrayPpHandBlow()
{
	std::vector<CPoint> vPicker;
	GetPickerSel(vPicker);

	ButtonClickLog(eClickBtn_Each_Blow);
	CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
	int nErr = pTaskTrayPP[m_nHandIdx]->do_VacBlow(eVacBlwSt_BlwOn, vPicker);
	if( nErr != ERR_NO_ERROR ){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, NULL);
	}
}

void CDlgManuTrayPP::SetBtnDisplay()
{
	m_BtnVacAll.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnVacAll.SetIcon(IDI_BTN_VAC);
	m_BtnVacAll.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnBlwAll.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnBlwAll.SetIcon(IDI_BTN_BLOW);
	m_BtnBlwAll.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnVacEach.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnVacEach.SetIcon(IDI_BTN_VAC);
	m_BtnVacEach.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnBlwEach.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnBlwEach.SetIcon(IDI_BTN_BLOW);
	m_BtnBlwEach.SetAlign(CButtonST::ST_ALIGN_VERT);

// 	m_BtnUpAll.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
// 	m_BtnUpAll.SetIcon(IDI_BTN_UP);
// 	m_BtnUpAll.SetAlign(CButtonST::ST_ALIGN_VERT);
// 
// 	m_BtnAllDown.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
// 	m_BtnAllDown.SetIcon(IDI_BTN_DOWN);
// 	m_BtnAllDown.SetAlign(CButtonST::ST_ALIGN_VERT);
// 
// 	m_BtnUpEach.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
// 	m_BtnUpEach.SetIcon(IDI_BTN_UP);
// 	m_BtnUpEach.SetAlign(CButtonST::ST_ALIGN_VERT);
// 
// 	m_BtnDownEach.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
// 	m_BtnDownEach.SetIcon(IDI_BTN_DOWN);
// 	m_BtnDownEach.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnStop.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnStop.SetIcon(IDI_STOP);
	m_BtnStop.SetAlign(CButtonST::ST_ALIGN_VERT);
}


void CDlgManuTrayPP::OnBnClickedButtonTrayPpCylActStop()
{
	ButtonClickLog(eClickBtn_Stop);
	m_nCylManualOpActState = eCylManualOper_None;
	ClearCommand();
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-10-06]
  NAME     : 
  PARAMS   : nCmd    : enum eCylManualCmd
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CDlgManuTrayPP::SetCommand(int nCmd)
{
	if( m_nCylManualOpActState != eCylManualOper_None ){
		return;
	}
	if( m_cbOperationSel.GetCurSel() < 0 ){
		return;
	}
	if( m_cbTimeSel.GetCurSel() < 0 ){
		return;
	}
	if( m_cbPickerSel.GetCurSel() < 0 ){
		return;
	}

	m_nCmdManualCyl = nCmd;
	m_nCmdOperation = m_cbOperationSel.GetCurSel();  // enum eCylManualOper
	m_nCmdParam_Time = m_cbTimeSel.GetCurSel()+1;

	m_vCmdParam_Picker.clear();
	if( (nCmd == eCylManualCmd_Picker_AllUp) || (nCmd == eCylManualCmd_Picker_AllDown))
	{
		for(int nY=0; nY<MAX_TRAY_PP_CNT_Y; nY++){
			for(int nX=0; nX<MAX_TRAY_PP_CNT_X; nX++) {
				m_vCmdParam_Picker.push_back(CPoint(nX,nY));
			}
		}	
	}
	else
	{
		GetPickerSel(m_vCmdParam_Picker);
	}

	m_nCylManualActionStep = 0;
	m_nCylManualOpActState = m_nCmdOperation;
}


//==============================================================================
//
//==============================================================================
void CDlgManuTrayPP::ClearCommand()
{
	m_nCmdManualCyl = eCylManualCmd_None;
	m_nCmdOperation = eCylManualOper_None;
	m_nCmdParam_Time = -1;
	m_vCmdParam_Picker.clear();
}

int CDlgManuTrayPP::do_CylCommand(BOOL bUndo/*=FALSE*/)
{
	int nErrCode=0;
	CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
	switch ( m_nCmdManualCyl )
	{
	case eCylManualCmd_Picker_EachUp:
	case eCylManualCmd_Picker_AllUp:
		{
			// int nCmd = (bUndo==FALSE)?DEF_UP:DEF_DOWN;			
			// nErrCode = pTaskTrayPP[m_nHandIdx]->do_Picker(nCmd, m_vCmdParam_Picker);
		}break;
	case eCylManualCmd_Picker_EachDown:
	case eCylManualCmd_Picker_AllDown:
		{
			// int nCmd = (bUndo==FALSE)?DEF_DOWN:DEF_UP;			
			// nErrCode = pTaskTrayPP[m_nHandIdx]->do_Picker(nCmd, m_vCmdParam_Picker);
		}break;
	}
	return nErrCode;
}

int CDlgManuTrayPP::Check_CylSensor(BOOL bUndo/*=FALSE*/)
{
	int nErrCode=0;
	switch ( m_nCmdManualCyl )
	{
	case eCylManualCmd_Picker_EachUp:
	case eCylManualCmd_Picker_AllUp:
		{
			// int nCmd = (bUndo==FALSE)?DEF_UP:DEF_DOWN;			
			// nErrCode = g_TaskTrayPP[m_nHandIdx].ChkPicker(nCmd, m_vCmdParam_Picker);
		}break;
	case eCylManualCmd_Picker_EachDown:
	case eCylManualCmd_Picker_AllDown:
		{
			// int nCmd = (bUndo==FALSE)?DEF_DOWN:DEF_UP;			
			// nErrCode = g_TaskTrayPP[m_nHandIdx].ChkPicker(nCmd, m_vCmdParam_Picker);
		}break;
	}
	return nErrCode;
}


//==============================================================================
//
//==============================================================================
void CDlgManuTrayPP::CylManualOper_None()
{
	switch( m_nCylManualActionStep )
	{
	case 0: // 아무것도 하지 않고 대기 한다.
		{
			ClearCommand();
			m_nCylManualActionStep++;
		}break;
	case 1:
		{
		}break;
	}
}

//==============================================================================
// cylinder 한번만 동작하고 센서체크 하지 않음.
//==============================================================================
void CDlgManuTrayPP::CylManualOper_Only_One_Action_and_No_Check_Sensor()
{
	switch( m_nCylManualActionStep )
	{
	case 0:
		{
			int nErr = do_CylCommand();
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep = 9000;
				break;
			}
			m_nCylManualActionStep++;
		}break;
	case 1:
		{
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	case 9000:
		{
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	}
}


//==============================================================================
// cylinder 한번만 동작하고 센서체크 함.
//==============================================================================
void CDlgManuTrayPP::CylManualOper_Only_One_Action_and_Check_Sensor()
{
	switch( m_nCylManualActionStep )
	{
	case 0:
		{
			int nErr = do_CylCommand();
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep = 9000;
				break;
			}
			m_dTimeCheck = (double)GetTickCount64();
			m_nCylManualActionStep++;
		}break;
	case 1:
		{
			double dSpanTime = (double)GetTickCount64() - m_dTimeCheck;
			if( dSpanTime > ((double)m_nCmdParam_Time*1000.0) )
			{
				m_nCylManualActionStep++;
			}
		}break;
	case 2:
		{
			int nErr = Check_CylSensor();
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep++;
				break;
			}
			m_nCylManualActionStep++;
		}break;
	case 3:
		{
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	case 9000:
		{
			m_nCylManualOpActState = eCylManualOper_None;
		}break;

	}
}
//==============================================================================
// 작업자가  stop하기 전까지 cylinder 동작하고 센서체크 하지 않음.
//==============================================================================
void CDlgManuTrayPP::CylManualOper_Continue_Action_and_No_Check_Sensor()
{
	switch( m_nCylManualActionStep )
	{
	case 0:
		{
			m_nCmdCycleCount=0;
			m_nCylManualActionStep = 1000;
		}break;
	case 1000:
		{
			int nErr = do_CylCommand();
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep = 9000;
				break;
			}
			m_dTimeCheck = (double)GetTickCount64();
			m_nCylManualActionStep++;
		}break;
	case 1001:
		{
			double dSpanTime = (double)GetTickCount64() - m_dTimeCheck;
			if( dSpanTime > ((double)m_nCmdParam_Time*1000.0) )
			{
				m_nCmdCycleCount++;
				m_nCylManualActionStep = 2000;
			}
		}break;
	case 2000:
		{
			int nErr = do_CylCommand(TRUE);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep = 9000;
				break;
			}
			m_dTimeCheck = (double)GetTickCount64();
			m_nCylManualActionStep++;
		}break;
	case 2001:
		{
			double dSpanTime = (double)GetTickCount64() - m_dTimeCheck;
			if( dSpanTime > ((double)m_nCmdParam_Time*1000.0) )
			{
				m_nCylManualActionStep = 1000;
			}
		}break;
	case 9000:
		{
			m_nCylManualOpActState = eCylManualOper_None;
		}break;

	}
}
//==============================================================================
// 작업자가 stop하기 전까지 cylinder 동작하고 센서체크 함.
//==============================================================================
void CDlgManuTrayPP::CylManualOper_Continue_Action_and_Check_Sensor()
{
	switch( m_nCylManualActionStep )
	{
	case 0:
		{
			m_nCmdCycleCount = 0;
			m_nCylManualActionStep = 1000;
		}break;
	case 1000:
		{
			int nErr = do_CylCommand();
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep = 9000;
				break;
			}
			m_dTimeCheck = (double)GetTickCount64();
			m_nCylManualActionStep++;
		}break;
	case 1001:
		{
			double dSpanTime = (double)GetTickCount64() - m_dTimeCheck;
			if( dSpanTime > ((double)m_nCmdParam_Time*1000.0) )
			{
				m_nCylManualActionStep++;
			}
		}break;
	case 1002:
		{
			int nErr = Check_CylSensor();
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep=9000;
				break;
			}
			m_nCmdCycleCount++;
			m_nCylManualActionStep = 2000;
		}break;

	case 2000:
		{
			int nErr = do_CylCommand(TRUE);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep = 9000;
				break;
			}
			m_dTimeCheck = (double)GetTickCount64();
			m_nCylManualActionStep++;
		}break;
	case 2001:
		{
			double dSpanTime = (double)GetTickCount64() - m_dTimeCheck;
			if( dSpanTime > ((double)m_nCmdParam_Time*1000.0) )
			{
				m_nCylManualActionStep++;
			}
		}break;
	case 2002:
		{
			int nErr = Check_CylSensor(TRUE);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep = 9000;
				break;
			}
			m_nCylManualActionStep = 1000;
		}break;

	case 9000:
		{
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	}
}
//==============================================================================
// 실린더 한번 동작 시키고 센서 들어오는 시간 계산 함.
//==============================================================================
void CDlgManuTrayPP::CylManualOper_Time_Check_Action_Once_and_Sensor_Check()
{
	switch( m_nCylManualActionStep )
	{
	case 0:
		{
			m_nCylActionTimeRetry = 0;
			m_dCylActionTimeRecord = 0.0;
			DisplayCylinderActionTime(TRUE);
			m_nCylManualActionStep = 1000;
		}break;
		//실린더 동작
	case 1000: 
		{
			int nErr = do_CylCommand();
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nCylManualActionStep = 9000;
				break;
			}
			m_dTimeCheck = (double)GetTickCount64();			
			m_nCylManualActionStep++;
		}break;
		// 센서 체크
	case 1001:
		{
			double dSpanTime = (double)GetTickCount64() - m_dTimeCheck;
			if( dSpanTime > 5000.0 )
			{
// 				if( m_nCylActionTimeRetry < 1)
// 				{
// 					m_nCylManualActionStep = 1000;
// 					m_nCylActionTimeRetry++;
// 					break;
// 				}
// 				else
// 				{
					CParamList AlarmData;
					g_Error.AlarmReport(ERR_TRAY_PP_1_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData, NULL);
					m_nCylManualActionStep = 9000;
					break;
//				}
			}
			int nChk = Check_CylSensor();
			if( nChk == ERR_NO_ERROR )
			{
				m_dCylActionTimeRecord = dSpanTime/1000.0;
				m_nCylManualActionStep++;
				break;
			}
		}break;
	case 1002:
		{
			DisplayCylinderActionTime();
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	case 9000:
		{
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	}
}

void CDlgManuTrayPP::DisplayCylinderActionTime(BOOL bClear/*=FALSE*/)
{
	CString strTime;
	strTime.Format("%.3f", m_dCylActionTimeRecord);
	if( bClear ) strTime = "";
	SetDlgItemText(UNITSETDLL_IDC_STATIC_TIME, strTime);
}


void CDlgManuTrayPP::CylinderManualAction()
{
	switch( m_nCylManualOpActState )
	{
	case eCylManualOper_None:
		{
			CylManualOper_None();
		}break;
	case eCylManualOper_Only_One_Action_and_No_Check_Sensor :
		{
			CylManualOper_Only_One_Action_and_No_Check_Sensor();
		}break;
	case eCylManualOper_Only_One_Action_and_Check_Sensor:
		{
			CylManualOper_Only_One_Action_and_Check_Sensor();
		}break;
	case eCylManualOper_Continue_Action_and_No_Check_Sensor:
		{
			CylManualOper_Continue_Action_and_No_Check_Sensor();
		}break;
	case eCylManualOper_Continue_Action_and_Check_Sensor:
		{
			CylManualOper_Continue_Action_and_Check_Sensor();
		}break;
	case eCylManualOper_Time_Check_Action_Once_and_Sensor_Check:
		{
			CylManualOper_Time_Check_Action_Once_and_Sensor_Check();
		}break;
	}
}

void CDlgManuTrayPP::ThreadProc1()
{
	SetThreadInterval(0, 50);
	while( GetThreadAliveFlag(0) ){
		if( GetPauseFlag(0) ) {
			NEXT;
			continue;
		}
		CylinderManualAction();
		NEXT;
	}
}

//-----------------------------------------------------
// Tray PP Manual 동작 Log
//-----------------------------------------------------
void CDlgManuTrayPP::ButtonClickLog(int nBtnNum)
{
	char szOperations[64] ={0,}, szPicker[32] ={0,}, szBtnName[32]={0,};

	switch(nBtnNum)
	{
		case eClickBtn_All_Up: sprintf_s(szBtnName,sizeof(szBtnName),"All Up"); break;
		case eClickBtn_All_Down: sprintf_s(szBtnName,sizeof(szBtnName),"All Down"); break;
		case eClickBtn_Each_Up: sprintf_s(szBtnName,sizeof(szBtnName),"Each Up"); break;
		case eClickBtn_Each_Down: sprintf_s(szBtnName,sizeof(szBtnName),"Each Down"); break;
		case eClickBtn_All_Vacuum: sprintf_s(szBtnName,sizeof(szBtnName),"All Vacuum"); break;
		case eClickBtn_All_Blow: sprintf_s(szBtnName,sizeof(szBtnName),"All Blow"); break;
		case eClickBtn_Each_Vacuum: sprintf_s(szBtnName,sizeof(szBtnName),"Each Vacuum"); break;
		case eClickBtn_Each_Blow: sprintf_s(szBtnName,sizeof(szBtnName),"Each Blow"); break;
		case eClickBtn_Stop: MakeLog("Stop Button Click"); break;
	}

	if(nBtnNum != eClickBtn_Stop){
		int nTime =m_cbTimeSel.GetCurSel()+1;

		switch(m_cbOperationSel.GetCurSel())
		{
			case 0: sprintf_s(szOperations,sizeof(szOperations),"Only One Action & No Check Sensor"); break;
			case 1: sprintf_s(szOperations,sizeof(szOperations),"Only One Action & Check Sensor"); break;
			case 2: sprintf_s(szOperations,sizeof(szOperations),"Continue Action & No Check Sensor"); break;
			case 3: sprintf_s(szOperations,sizeof(szOperations),"Continue Action & Check Sensor"); break;
			case 4: sprintf_s(szOperations,sizeof(szOperations),"Time Check Action (Once & Sensor Check)"); break;
		}

		if( nBtnNum == eClickBtn_Each_Up || nBtnNum == eClickBtn_Each_Down || nBtnNum == eClickBtn_Each_Vacuum || nBtnNum == eClickBtn_Each_Blow){
			switch(m_cbPickerSel.GetCurSel())
			{
				case 0: sprintf_s(szPicker,sizeof(szPicker),"Select Picker: y1 x1,"); break;
				case 1: sprintf_s(szPicker,sizeof(szPicker),"Select Picker: y1 x2,"); break;
				case 2: sprintf_s(szPicker,sizeof(szPicker),"Select Picker: y1 x3,"); break;
				case 3: sprintf_s(szPicker,sizeof(szPicker),"Select Picker: y1 x4,"); break;
				case 4: sprintf_s(szPicker,sizeof(szPicker),"Select Picker: y2 x1,"); break;
				case 5: sprintf_s(szPicker,sizeof(szPicker),"Select Picker: y2 x2,"); break;
				case 6: sprintf_s(szPicker,sizeof(szPicker),"Select Picker: y2 x3,"); break;
				case 7: sprintf_s(szPicker,sizeof(szPicker),"Select Picker: y2 x4,"); break;
				case 8: sprintf_s(szPicker,sizeof(szPicker),"Select Picker: Front All,"); break;
				case 9: sprintf_s(szPicker,sizeof(szPicker),"Select Picker: Rear All,"); break;
			}
		}
		MakeLog("%s Button Click Operations: %s, %s Time: %d sec", szBtnName, szOperations, szPicker, nTime);
	}
}

void CDlgManuTrayPP::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = {0,};
	if(fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf_s(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_SYSTEM), Debug, "", __LINE__, NULL, tmpMsg);
}

void CDlgManuTrayPP::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

void CDlgManuTrayPP::DisplayHandSenIdx()
{
	int nSenID[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = {
		/*REAR*/{ IDC_COLORCHECKBTCTRL1, IDC_COLORCHECKBTCTRL2, IDC_COLORCHECKBTCTRL3, IDC_COLORCHECKBTCTRL4 }
	    /*FRONT*/,{ IDC_COLORCHECKBTCTRL9, IDC_COLORCHECKBTCTRL10,IDC_COLORCHECKBTCTRL11,IDC_COLORCHECKBTCTRL12 } };

	int nMaxTrayPpCnt = MAX_TRAY_PP_CNT_X*MAX_TRAY_PP_CNT_Y;

	std::vector<CString> vstrPkrIdx;
	vstrPkrIdx.clear();
	for (int i = 0; i < nMaxTrayPpCnt; i++) {
		CString strTemp = _T("");
		strTemp.Format("%d", i + 1);
		vstrPkrIdx.push_back(strTemp);
	}

	for (int y = 0; y < MAX_TRAY_PP_CNT_Y; y++){
		for (int x = 0; x < MAX_TRAY_PP_CNT_X; x++) {
			int nIdx = x + 4 * y;

			SetDlgItemText(nSenID[y][x], vstrPkrIdx[nIdx]);
		}
	}
	

}