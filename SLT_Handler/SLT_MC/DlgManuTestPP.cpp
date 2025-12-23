// DlgManuTestPP.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgManuTestPP.h"
#include "afxdialogex.h"

#include "../Include/OcxFiles/dsspanel.h"

// CDlgManuTestPP 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgManuTestPP, CDialogEx)

CDlgManuTestPP::CDlgManuTestPP(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgManuTestPP::IDD, pParent)
{
	m_nHandIdx = -1;
	memset(m_nHandSenID    , 0x00, sizeof(m_nHandSenID));
	m_nCylManualOpActState = eCylManualOper_None;
	m_nCylManualActionStep = 0;
	m_nCmdCycleCount = 0;
	m_dTimeCheck = 0.0;

	m_nCmdManualCyl = 0;
	m_nCmdOperation = 0;
	m_nCmdParam_Time = 0;
	m_dCylActionTimeRecord = 0.0;
	m_nCylActionTimeRetry = 0;
	m_nCylRepeatStep = 0;
	m_nHandType = 0;
}

CDlgManuTestPP::~CDlgManuTestPP()
{
	m_nCmdManualCyl = eCylManualCmd_None;
	m_nCmdParam_Time = 0;
	m_vCmdParam_Picker.clear();

	m_cBmpExit.DeleteObject();
}

void CDlgManuTestPP::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LOAD_HAND_SEL, m_cbPickerSel);
	DDX_Control(pDX, IDC_BUTTON_ALL_VACUUM, m_BtnVacAll);
	DDX_Control(pDX, IDC_BUTTON_ALL_BLOW, m_BtnBlwAll);
	DDX_Control(pDX, IDC_BUTTON_TEST_PP_HAND_VACUUM, m_BtnVacEach);
	DDX_Control(pDX, IDC_BUTTON_TEST_PP_HAND_BLOW, m_BtnBlwEach);
	DDX_Control(pDX, IDC_BUTTON_TEST_PP_ARM_ALL_UP, m_BtnUpAll);
	DDX_Control(pDX, IDC_BUTTON_TEST_PP_ARM_ALL_DOWN, m_BtnAllDown);
	DDX_Control(pDX, IDC_BUTTON_TEST_PP_HAND_UP, m_BtnUpEach);
	DDX_Control(pDX, IDC_BUTTON_TEST_PP_HAND_DOWN, m_BtnDownEach);
	DDX_Control(pDX, IDC_COMBO_OPERATION, m_cbOperationSel);
	DDX_Control(pDX, IDC_COMBO_CONTINUE_TIME, m_cbTimeSel);
	DDX_Control(pDX, IDC_BUTTON_TEST_PP_CYL_ACT_STOP, m_BtnStop);

	// exit
	DDX_Control(pDX, IDOK, m_btnExit);
}


BEGIN_MESSAGE_MAP(CDlgManuTestPP, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ALL_VACUUM, &CDlgManuTestPP::OnBnClickedButtonAllVacuum)
	ON_BN_CLICKED(IDC_BUTTON_ALL_BLOW, &CDlgManuTestPP::OnBnClickedButtonAllBlow)
	ON_BN_CLICKED(IDC_BUTTON_TEST_PP_ARM_ALL_UP, &CDlgManuTestPP::OnBnClickedButtonTestPpArmAllUp)
	ON_BN_CLICKED(IDC_BUTTON_TEST_PP_ARM_ALL_DOWN, &CDlgManuTestPP::OnBnClickedButtonTestPpArmAllDown)
	ON_BN_CLICKED(IDC_BUTTON_TEST_PP_HAND_UP, &CDlgManuTestPP::OnBnClickedButtonTestPpHandUp)
	ON_BN_CLICKED(IDC_BUTTON_TEST_PP_HAND_DOWN, &CDlgManuTestPP::OnBnClickedButtonTestPpHandDown)
	ON_BN_CLICKED(IDC_BUTTON_TEST_PP_HAND_VACUUM, &CDlgManuTestPP::OnBnClickedButtonTestPpHandVacuum)
	ON_BN_CLICKED(IDC_BUTTON_TEST_PP_HAND_BLOW, &CDlgManuTestPP::OnBnClickedButtonTestPpHandBlow)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_TEST_PP_CYL_ACT_STOP, &CDlgManuTestPP::OnBnClickedButtonTestPpCylActStop)
	ON_BN_CLICKED(IDOK, &CDlgManuTestPP::OnBnClickedOk)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_HAND_1, IDC_RADIO_HAND_2, &CDlgManuTestPP::OnBnClickedRadioHandType)
END_MESSAGE_MAP()


// CDlgManuTestPP 메시지 처리기입니다.


void CDlgManuTestPP::OnBnClickedButtonAllVacuum()
{
	ButtonClickLog(eClickBtn_All_Vacuum);

	int nErr = g_TaskTestPP[m_nHandIdx].do_VacBlow_All(eVacBlwSt_VacOn,m_nHandType);
	if( nErr != ERR_NO_ERROR ){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, NULL);
	}
}


void CDlgManuTestPP::OnBnClickedButtonAllBlow()
{
	ButtonClickLog(eClickBtn_All_Blow);

	int nErr = g_TaskTestPP[m_nHandIdx].do_VacBlow_All(eVacBlwSt_BlwOn, m_nHandType);
	if( nErr != ERR_NO_ERROR ){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, NULL);
	}
}


void CDlgManuTestPP::OnBnClickedButtonTestPpArmAllUp()
{

}


void CDlgManuTestPP::OnBnClickedButtonTestPpArmAllDown()
{

}


void CDlgManuTestPP::OnBnClickedButtonTestPpHandUp()	// EACH
{

}


void CDlgManuTestPP::OnBnClickedButtonTestPpHandDown()   // EACH
{

}


void CDlgManuTestPP::OnBnClickedButtonTestPpHandVacuum() // EACH
{
	std::vector<CPoint> vPicker;
	GetPickerSel(vPicker);

	ButtonClickLog(eClickBtn_Each_Vacuum);

	int nErr = g_TaskTestPP[m_nHandIdx].do_VacBlow(eVacBlwSt_VacOn, m_nHandType, vPicker);
	if( nErr != ERR_NO_ERROR ){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, NULL);
	}
}


void CDlgManuTestPP::OnBnClickedButtonTestPpHandBlow()	// EACH
{
	std::vector<CPoint> vPicker;
	GetPickerSel(vPicker);

	ButtonClickLog(eClickBtn_Each_Blow);

	int nErr = g_TaskTestPP[m_nHandIdx].do_VacBlow(eVacBlwSt_BlwOn, m_nHandType, vPicker);
	if( nErr != ERR_NO_ERROR ){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, NULL);
	}
}

void CDlgManuTestPP::OnTimer(UINT_PTR nIDEvent)
{	
	CDSSPanel*   chkbt    = NULL;

	if(nIDEvent == 0)
	{
		KillTimer(nIDEvent);

		int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);

		for (int nHandType = 0; nHandType < eMaxTestPPTypeCount; nHandType++)
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{
				for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
				{
					//chkbt = (CDSSPanel*)GetDlgItem(m_nHandSenID[nHandType][nY][nX]);
					int nIdx = 0;
					if (nBoardType == eBoardDir_Forward) {
						nIdx = abs((nX + 4 * nY) - 7);
					}
					else { //nBoardType == eBoardDir_Backward			
						nIdx = nX + 4 * nY;
					}
					chkbt = (CDSSPanel*)GetDlgItem(m_nHandSenID[nHandType][nIdx]);
					if (g_TaskTestPP[m_nHandIdx].GetVacuum(nHandType, CPoint(nX, nY)) == DEF_ON) {
						chkbt->On();
					}
					else {
						chkbt->Off();
					}
				}
			}
		}



		chkbt = (CDSSPanel*)GetDlgItem(IDC_COLORCHECKBTCTRL_TESTPP_SAFETY_X1_POSITION);
 		if( g_TaskTestPP[m_nHandIdx].GetDI(CTaskTestPP::xX1AXIS_POSITION_SEN) ){
 			chkbt->On();
 		}else{
 			chkbt->Off();
 		}

		chkbt = (CDSSPanel*)GetDlgItem(IDC_COLORCHECKBTCTRL_TESTPP_SAFETY_X2_POSITION);
		if (g_TaskTestPP[m_nHandIdx].GetDI(CTaskTestPP::xX2AXIS_POSITION_SEN)) {
			chkbt->On();
		}
		else {
			chkbt->Off();
		}
		//-- sequence busy display
		chkbt = (CDSSPanel*)GetDlgItem(IDC_COLORCHECKBTCTRL_TESTPP_BUSY);
		if( m_nCylManualOpActState != eCylManualOper_None ){
			chkbt->On();
		}else{
			chkbt->Off();
		}

		CString strTitle = _T("");
		strTitle.Format("TEST PP #%d - %s", m_nHandIdx + 1, m_nHandType == eTestPPLoadType ? "Load Hand" : "Unload Hand");
		::SetWindowText(this->m_hWnd, strTitle);

		SetDlgItemText(IDC_STATIC_HEAD_TEST_PP, strTitle);


		SetTimer(0, DEF_SENSOR_DISP_TM, NULL);
	}

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CDlgManuTestPP::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// exit
	m_cBmpExit.LoadBitmap(IDB_EXIT_CYLINDER_TESTPP);
	m_btnExit.SetBitmap(m_cBmpExit);

	CString strTitle = _T("");
	strTitle.Format("TEST PP #%d", m_nHandIdx+1);
	::SetWindowText(this->m_hWnd, strTitle);

	SetDlgItemText(IDC_STATIC_HEAD_TEST_PP, strTitle);


	m_nHandSenID[eTestPPLoadType][0] = IDC_COLORCHECKBTCTRL1;
	m_nHandSenID[eTestPPLoadType][1] = IDC_COLORCHECKBTCTRL2;
	m_nHandSenID[eTestPPLoadType][2] = IDC_COLORCHECKBTCTRL3;
	m_nHandSenID[eTestPPLoadType][3] = IDC_COLORCHECKBTCTRL4;
	m_nHandSenID[eTestPPLoadType][4] = IDC_COLORCHECKBTCTRL5;
	m_nHandSenID[eTestPPLoadType][5] = IDC_COLORCHECKBTCTRL6;
	m_nHandSenID[eTestPPLoadType][6] = IDC_COLORCHECKBTCTRL7;
	m_nHandSenID[eTestPPLoadType][7] = IDC_COLORCHECKBTCTRL8;

	m_nHandSenID[eTestPPUnLoadType][0] = IDC_COLORCHECKBTCTRL9;
	m_nHandSenID[eTestPPUnLoadType][1] = IDC_COLORCHECKBTCTRL10;
	m_nHandSenID[eTestPPUnLoadType][2] = IDC_COLORCHECKBTCTRL11;
	m_nHandSenID[eTestPPUnLoadType][3] = IDC_COLORCHECKBTCTRL12;
	m_nHandSenID[eTestPPUnLoadType][4] = IDC_COLORCHECKBTCTRL13;
	m_nHandSenID[eTestPPUnLoadType][5] = IDC_COLORCHECKBTCTRL14;
	m_nHandSenID[eTestPPUnLoadType][6] = IDC_COLORCHECKBTCTRL15;
	m_nHandSenID[eTestPPUnLoadType][7] = IDC_COLORCHECKBTCTRL16;

// 	m_nHandSenID[eTestPPLoadType][0][0] = IDC_COLORCHECKBTCTRL1;
// 	m_nHandSenID[eTestPPLoadType][0][1] = IDC_COLORCHECKBTCTRL2;
// 	m_nHandSenID[eTestPPLoadType][0][2] = IDC_COLORCHECKBTCTRL3;
// 	m_nHandSenID[eTestPPLoadType][0][3] = IDC_COLORCHECKBTCTRL4;
// 	m_nHandSenID[eTestPPLoadType][1][0] = IDC_COLORCHECKBTCTRL5;
// 	m_nHandSenID[eTestPPLoadType][1][1] = IDC_COLORCHECKBTCTRL6;
// 	m_nHandSenID[eTestPPLoadType][1][2] = IDC_COLORCHECKBTCTRL7;
// 	m_nHandSenID[eTestPPLoadType][1][3] = IDC_COLORCHECKBTCTRL8;
// 
// 	m_nHandSenID[eTestPPUnLoadType][0][0] = IDC_COLORCHECKBTCTRL9;
// 	m_nHandSenID[eTestPPUnLoadType][0][1] = IDC_COLORCHECKBTCTRL10;
// 	m_nHandSenID[eTestPPUnLoadType][0][2] = IDC_COLORCHECKBTCTRL11;
// 	m_nHandSenID[eTestPPUnLoadType][0][3] = IDC_COLORCHECKBTCTRL12;
// 	m_nHandSenID[eTestPPUnLoadType][1][0] = IDC_COLORCHECKBTCTRL13;
// 	m_nHandSenID[eTestPPUnLoadType][1][1] = IDC_COLORCHECKBTCTRL14;
// 	m_nHandSenID[eTestPPUnLoadType][1][2] = IDC_COLORCHECKBTCTRL15;
// 	m_nHandSenID[eTestPPUnLoadType][1][3] = IDC_COLORCHECKBTCTRL16;

	DisplayHandSenIdx();

	m_cbOperationSel.Clear();
	m_cbOperationSel.AddString("1: Only One Action & No Check Sensor");
	m_cbOperationSel.AddString("2: Only One Action & Check Sensor");
	m_cbOperationSel.AddString("3: Continue Action & No Check Sensor");
	m_cbOperationSel.AddString("4: Continue Action & Check Sensor");
	m_cbOperationSel.AddString("5: Time Check Action (Once & Sensor Check)");
	m_cbOperationSel.SetCurSel(0);

	m_cbPickerSel.Clear();
	m_cbPickerSel.AddString("Hand1 1: y1 x1" );
	m_cbPickerSel.AddString("Hand1 2: y1 x2" );
	m_cbPickerSel.AddString("Hand1 3: y1 x3" );
	m_cbPickerSel.AddString("Hand1 4: y1 x4" );
	m_cbPickerSel.AddString("Hand1 5: y2 x1" );
	m_cbPickerSel.AddString("Hand1 6: y2 x2" );
	m_cbPickerSel.AddString("Hand1 7: y2 x3" );
	m_cbPickerSel.AddString("Hand1 8: y2 x4" );
	m_cbPickerSel.AddString("Hand1 9: Front All" );
	m_cbPickerSel.AddString("Hand1 10: Rear All" );
	//m_cbPickerSel.AddString("10: All");
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

	SetTimer(0, DEF_SENSOR_DISP_TM, NULL);

	SetBtnDisplay();

	CreateThread(0);


	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_HAND_1);
	pButton->SetCheck(true);

	MakeLog("Load Table & Test PP #%d Dialog Show",m_nHandIdx+1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgManuTestPP::SetBtnDisplay()
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

	m_BtnUpAll.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnUpAll.SetIcon(IDI_BTN_UP);
	m_BtnUpAll.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnAllDown.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnAllDown.SetIcon(IDI_BTN_DOWN);
	m_BtnAllDown.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnUpEach.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnUpEach.SetIcon(IDI_BTN_UP);
	m_BtnUpEach.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnDownEach.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnDownEach.SetIcon(IDI_BTN_DOWN);
	m_BtnDownEach.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnStop.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnStop.SetIcon(IDI_STOP);
	m_BtnStop.SetAlign(CButtonST::ST_ALIGN_VERT);
}

void CDlgManuTestPP::ThreadProc1()
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

void CDlgManuTestPP::CylinderManualAction()
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

void CDlgManuTestPP::OnDestroy()
{
	CDialogEx::OnDestroy();
	KillTimer(0);
	CloseThread();	
}


BOOL CDlgManuTestPP::PreTranslateMessage(MSG* pMsg)
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

/*=============================================================================
  AUTHOR   : 나명진  [2015-10-06]
  NAME     : 
  PARAMS   : nCmd    : enum eCylManualCmd
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CDlgManuTestPP::SetCommand(int nCmd)
{
	if( m_nCylManualOpActState != eCylManualCmd_None ){
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
	m_nCmdOperation = m_cbOperationSel.GetCurSel();
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

void CDlgManuTestPP::GetPickerSel(std::vector<CPoint>& rSelectedPicker)
{
	int nCurSel = m_cbPickerSel.GetCurSel();
	if( nCurSel < 0 ){
		return;
	}
	if( nCurSel < ePickerSel_Front_All ) 
	{
		CPoint SelPicker;
		m_mapPickerSel.Lookup(nCurSel, SelPicker);
		rSelectedPicker.push_back(SelPicker);
	}else if( nCurSel == ePickerSel_Front_All )
	{
		for( int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++ ) 
		{
			rSelectedPicker.push_back( CPoint( nX, 1 ) );
		}
	}else if( nCurSel == ePickerSel_Rear_All )
	{
		for( int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++ )
		{
			rSelectedPicker.push_back( CPoint( nX, 0 ) );
		}
	}else 
	{
		ASSERT( FALSE );
	}
}

//==============================================================================
//
//==============================================================================
void CDlgManuTestPP::ClearCommand()
{
	m_nCmdManualCyl = eCylManualCmd_None;
	m_nCmdOperation = eCylManualOper_None;
	m_nCmdParam_Time = -1;
	m_vCmdParam_Picker.clear();
}

int CDlgManuTestPP::do_CylCommand(BOOL bUndo/*=FALSE*/)
{
	int nErrCode=0;
	switch ( m_nCmdManualCyl )
	{
	case eCylManualCmd_Picker_EachUp:
	case eCylManualCmd_Picker_AllUp:
		{
// 			int nCmd = (bUndo==FALSE)?DEF_UP:DEF_DOWN;			
// 			nErrCode = g_TaskTestPP[m_nHandIdx].do_Picker(nCmd, m_vCmdParam_Picker);
		}break;
	case eCylManualCmd_Picker_EachDown:
	case eCylManualCmd_Picker_AllDown:
		{
// 			int nCmd = (bUndo==FALSE)?DEF_DOWN:DEF_UP;			
// 			nErrCode = g_TaskTestPP[m_nHandIdx].do_Picker(nCmd, m_vCmdParam_Picker);
		}break;
	}
	return nErrCode;
}

int CDlgManuTestPP::Check_CylSensor(BOOL bUndo/*=FALSE*/)
{
	int nErrCode=0;
	switch ( m_nCmdManualCyl )
	{
	case eCylManualCmd_Picker_EachUp:
	case eCylManualCmd_Picker_AllUp:
		{
// 			int nCmd = (bUndo==FALSE)?DEF_UP:DEF_DOWN;			
// 			nErrCode = g_TaskTestPP[m_nHandIdx].ChkPicker(nCmd, m_nHandType, m_vCmdParam_Picker);
		}break;
	case eCylManualCmd_Picker_EachDown:
	case eCylManualCmd_Picker_AllDown:
		{
// 			int nCmd = (bUndo==FALSE)?DEF_DOWN:DEF_UP;			
// 			nErrCode = g_TaskTestPP[m_nHandIdx].ChkPicker(nCmd, m_nHandType, m_vCmdParam_Picker);
		}break;
	}
	return nErrCode;
}

//==============================================================================
//
//==============================================================================
void CDlgManuTestPP::CylManualOper_None()
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
void CDlgManuTestPP::CylManualOper_Only_One_Action_and_No_Check_Sensor()
{
	switch( m_nCylManualActionStep )
	{
	case 0:
		{
			do_CylCommand();
			m_nCylManualActionStep++;
		}break;
	case 1:
		{
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	}
}


//==============================================================================
// cylinder 한번만 동작하고 센서체크 함.
//==============================================================================
void CDlgManuTestPP::CylManualOper_Only_One_Action_and_Check_Sensor()
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
void CDlgManuTestPP::CylManualOper_Continue_Action_and_No_Check_Sensor()
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
void CDlgManuTestPP::CylManualOper_Continue_Action_and_Check_Sensor()
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
// 작업자가 stop하기 전까지 cylinder 동작하고 센서체크 함.
//==============================================================================
void CDlgManuTestPP::CylManualOper_Time_Check_Action_Once_and_Sensor_Check()
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
					g_Error.AlarmReport(ERR_TEST_PP_1_Z1_PICKER_MOVE_TIMEOUT + g_TaskTestPP[m_nHandIdx].GetHandTypeIndex(m_nHandType), AlarmData, NULL);
					m_nCylManualActionStep = 9000;
					break;
//				}
			}
			int nChk = Check_CylSensor();
			if( nChk == ERR_NO_ERROR )
			{
				m_dCylActionTimeRecord = dSpanTime / 1000.0;
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

void CDlgManuTestPP::DisplayCylinderActionTime(BOOL bClear/*=FALSE*/)
{
	CString strTime;
	strTime.Format("%.3f", m_dCylActionTimeRecord);
	if( bClear ) strTime = "";
	SetDlgItemText(UNITSETDLL_IDC_STATIC_TIME, strTime);
}

void CDlgManuTestPP::OnBnClickedButtonTestPpCylActStop()
{
	ButtonClickLog(eClickBtn_Stop);
	m_nCylManualOpActState = eCylManualCmd_None;
}

//-----------------------------------------------------
// Test PP Manual 동작 Log
//-----------------------------------------------------
void CDlgManuTestPP::ButtonClickLog(int nBtnNum)
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
			case 8: sprintf_s( szPicker, sizeof( szPicker ), "Select Picker: Front All," ); break;
			case 9: sprintf_s( szPicker, sizeof( szPicker ), "Select Picker: Rear All," ); break;
			}

		}
		MakeLog("%s Button Click Operations: %s, %s Time: %d sec", szBtnName, szOperations, szPicker, nTime);
	}
}

void CDlgManuTestPP::MakeLog(LPCTSTR fmt, ...)
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

void CDlgManuTestPP::OnBnClickedOk()
{
	CDialogEx::OnOK();
}


void  CDlgManuTestPP::OnBnClickedRadioHandType(UINT uiID)
{
	switch (uiID)
	{
		case IDC_RADIO_HAND_1 :
		{
			m_nHandType = eTestPPLoadType;
		}break;
		case IDC_RADIO_HAND_2:
		{
			m_nHandType = eTestPPUnLoadType;
		}break;
	}
}

void CDlgManuTestPP::DisplayHandSenIdx()
{
	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);

 	std::vector<CString> vstrPkrIdx;
	vstrPkrIdx.clear();
 	for (int i = 0; i < MAX_TEST_PP_PKR_CNT; i++) {
		CString strTemp = _T("");
		strTemp.Format("%d", i + 1);
		vstrPkrIdx.push_back(strTemp);
 	}

	for (int nHandType = 0; nHandType < eMaxTestPPTypeCount; nHandType++) {
		for (int y = 0; y < MAX_TEST_PP_PKR_CNT_Y; y++)
		{
			for (int x = 0; x < MAX_TEST_PP_PKR_CNT_X; x++) {
				int nIdx = 0;
				if (nBoardType == eBoardDir_Forward) {
					nIdx = abs((x + 4*y) - 7);
				}
				else { //nBoardType == eBoardDir_Backward			
					nIdx = x + 4*y;
				}

				//Controll ID는 정방향이며, vstrPkrIdx를 보드 Type에 따라 정방향, 역방향 선택
				SetDlgItemText(m_nHandSenID[nHandType][x + 4 * y], vstrPkrIdx[nIdx]);
			}
		}
	}



}