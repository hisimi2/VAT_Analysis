// DlgManuLoadTbl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgManuLoadTbl.h"
#include "afxdialogex.h"
#include "DlgVibrator.h"
#include "../Include/OcxFiles/dsspanel.h"

#define TBL1 (0)
#define TBL2 (1)

// CDlgManuLoadTbl 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgManuLoadTbl, CDialogEx)

CDlgManuLoadTbl::CDlgManuLoadTbl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgManuLoadTbl::IDD, pParent)
{
	m_nLoadTblIdx = -1;

	m_nCylManualOpActState = eCylManualOper_None;
	m_nCylManualActionStep = 0;
	m_nCmdCycleCount = 0;
	m_dTimeCheck = 0.0;

	m_nCmdManualCyl = 0;
	m_nCmdOperation = 0;
	m_nCmdParam_Time = 0;
	m_nHorVerTblSel = 0;
	m_dCylActionTimeRecord = 0.0;
	m_nCylActionTimeRetry = 0;
	m_nCylinderNum = 0;

	memset(&m_dCylinderSpec, 0x00, sizeof(m_dCylinderSpec));

	for (int i = 0; i < eMaxLoadTblCount; i++)
	{
		m_dCylinderSpec[eLoadTbl_1 + i][eLoadTblLeft].dMax = 0.25;
		m_dCylinderSpec[eLoadTbl_1 + i][eLoadTblLeft].dMin = 0.20;

		m_dCylinderSpec[eLoadTbl_1 + i][eLoadTblRight].dMax = 0.25;
		m_dCylinderSpec[eLoadTbl_1 + i][eLoadTblRight].dMin = 0.20;
	}

	m_nLEDIdx = -1;
	m_nLEDOnOff = -1;
}

CDlgManuLoadTbl::~CDlgManuLoadTbl()
{
	m_nCmdManualCyl = eCylManualCmd_None;
	m_nCmdParam_Time = 0;
}

void CDlgManuLoadTbl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDOK, m_btnExit);
	DDX_Control(pDX, IDC_BUTTON_TBL1_LEFT, m_BtnTbl_Left);
	DDX_Control(pDX, IDC_BUTTON_TBL1_RIGHT, m_BtnTbl_Right);
	DDX_Control(pDX, IDC_BUTTON_TBL2_LEFT ,m_BtnTb2_Left);
	DDX_Control(pDX, IDC_BUTTON_TBL2_RIGHT ,m_BtnTb2_Right);

	DDX_Control(pDX, IDC_COMBO_OPERATION, m_cbOperationSel);
	DDX_Control(pDX, IDC_COMBO_CONTINUE_TIME, m_cbTimeSel);
	DDX_Control(pDX, IDC_BUTTON_TBL_CYL_ACT_STOP, m_BtnStop);

//	DDX_Control(pDX, IDC_BUTTON_TBL1_BARCODE_LED, m_BtnTbl1_BarcodeLed);
//	DDX_Control(pDX, IDC_BUTTON_TBL2_BARCODE_LED, m_BtnTbl2_BarcodeLed);
}


BEGIN_MESSAGE_MAP(CDlgManuLoadTbl, CDialogEx)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_TBL_CYL_ACT_STOP, &CDlgManuLoadTbl::OnBnClickedButtonTblCylActStop)
//	ON_BN_CLICKED(IDOK, &CDlgManuLoadTbl::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_TBL1_LEFT, &CDlgManuLoadTbl::OnBnClickedButtonTbl1Left)
	ON_BN_CLICKED(IDC_BUTTON_TBL1_RIGHT, &CDlgManuLoadTbl::OnBnClickedButtonTbl1Right)
	ON_BN_CLICKED(IDC_BUTTON_TBL2_LEFT, &CDlgManuLoadTbl::OnBnClickedButtonTbl2Left)
	ON_BN_CLICKED(IDC_BUTTON_TBL2_RIGHT, &CDlgManuLoadTbl::OnBnClickedButtonTbl2Right)
	ON_BN_CLICKED(IDOK, &CDlgManuLoadTbl::OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_TBL1_LED_ON, &CDlgManuLoadTbl::OnBnClickedButtonTbl1LedOn)
	ON_BN_CLICKED(IDC_BUTTON_TBL1_LED_OFF, &CDlgManuLoadTbl::OnBnClickedButtonTbl1LedOff)
	ON_BN_CLICKED(IDC_BUTTON_TBL2_LED_ON, &CDlgManuLoadTbl::OnBnClickedButtonTbl2LedOn)
	ON_BN_CLICKED(IDC_BUTTON_TBL2_LED_OFF, &CDlgManuLoadTbl::OnBnClickedButtonTbl2LedOff)
	ON_BN_CLICKED(IDC_BUTTON_VIBRATOR, &CDlgManuLoadTbl::OnBnClickedButtonVibrator)
END_MESSAGE_MAP()

void CDlgManuLoadTbl::OnTimer(UINT_PTR nIDEvent)
{	
	CDialogEx::OnTimer(nIDEvent);

	if( nIDEvent == 0){
		KillTimer(nIDEvent);

		// 2D Barcode Cylinder

		BOOL bVal1_L = g_TaskLoadTbl[eLoadTbl_1].GetDI(CTaskLoadTable::x2D_BARCODE_LEFT_SEN);
		BOOL bVal1_R = g_TaskLoadTbl[eLoadTbl_1].GetDI(CTaskLoadTable::x2D_BARCODE_RIGHT_SEN);

		BOOL bVal2_L = g_TaskLoadTbl[eLoadTbl_3].GetDI(CTaskLoadTable::x2D_BARCODE_LEFT_SEN);
		BOOL bVal2_R = g_TaskLoadTbl[eLoadTbl_3].GetDI(CTaskLoadTable::x2D_BARCODE_RIGHT_SEN);

		UpdateCylinderSts(IDC_BUTTON_TBL1_LEFT , bVal1_L);
		UpdateCylinderSts(IDC_BUTTON_TBL1_RIGHT  , bVal1_R);
		UpdateCylinderSts(IDC_BUTTON_TBL2_LEFT , bVal2_L);
		UpdateCylinderSts(IDC_BUTTON_TBL2_RIGHT  , bVal2_R);


		int nArrive_Sen[4][eMaxLoadTblCount] = 
		{
			{IDC_CTRL_ARV_TBL1_DEVICE1,IDC_CTRL_ARV_TBL2_DEVICE1,IDC_CTRL_ARV_TBL3_DEVICE1,IDC_CTRL_ARV_TBL4_DEVICE1 },
			{IDC_CTRL_ARV_TBL1_DEVICE2,IDC_CTRL_ARV_TBL2_DEVICE2,IDC_CTRL_ARV_TBL3_DEVICE2,IDC_CTRL_ARV_TBL4_DEVICE2 },
			{IDC_CTRL_ARV_TBL1_DEVICE3,IDC_CTRL_ARV_TBL2_DEVICE3,IDC_CTRL_ARV_TBL3_DEVICE3,IDC_CTRL_ARV_TBL4_DEVICE3 },
			{IDC_CTRL_ARV_TBL1_DEVICE4,IDC_CTRL_ARV_TBL2_DEVICE4,IDC_CTRL_ARV_TBL3_DEVICE4,IDC_CTRL_ARV_TBL4_DEVICE4 }
		};

		// Arrived Sensor
		CDSSPanel*   chkbt    = NULL;
		for(int ni = 0; ni < 4; ni++){
			for(int nj = 0; nj < eMaxLoadTblCount; nj++){
				chkbt = (CDSSPanel*)GetDlgItem(nArrive_Sen[ni][nj]);
				if( g_TaskLoadTbl[nj].GetDI(CTaskLoadTable::xLOAD_TABLE_DEVICE1_SEN + ni) == DEF_ON ){
					chkbt->On();
				}else{
					chkbt->Off();
				}
			}
		}

		int nRotate_Sen[2][eMaxLoadTblCount] =
		{
			{ IDC_CTRL_TBL1_ROTATE1_SEN, IDC_CTRL_TBL2_ROTATE1_SEN,IDC_CTRL_TBL3_ROTATE1_SEN,IDC_CTRL_TBL4_ROTATE1_SEN },
			{ IDC_CTRL_TBL1_ROTATE2_SEN,IDC_CTRL_TBL2_ROTATE2_SEN,IDC_CTRL_TBL3_ROTATE2_SEN,IDC_CTRL_TBL4_ROTATE2_SEN },
		};

		// Rotate Sensor
		chkbt = NULL;
		for (int ni = 0; ni < 2; ni++) {
			for (int nj = 0; nj < eMaxLoadTblCount; nj++) {
				chkbt = (CDSSPanel*)GetDlgItem(nRotate_Sen[ni][nj]);
				if (g_TaskLoadTbl[nj].GetDI(CTaskLoadTable::xLOAD_TABLE_ROTATE1_SEN + ni) == DEF_ON) 
				{
					chkbt->On();
				}
				else 
				{
					chkbt->Off();
				}
			}
		}

		//-- sequence busy display
		//CDSSPanel*   chkbt    = NULL;
		chkbt = (CDSSPanel*)GetDlgItem(IDC_COLORCHECKBTCTRL_TBL_BUSY);
		if( m_nCylManualOpActState != eCylManualOper_None ){
			chkbt->On();
		}else{
			chkbt->Off();
		}

		SetTimer(nIDEvent, DEF_SENSOR_DISP_TM, NULL);
	}	
	else if (nIDEvent == 1) {
		KillTimer(nIDEvent);

		BOOL bIsDone = UpdateBarcodeLEDSts(m_nLEDOnOff, m_nLEDIdx);

		if(!bIsDone)
			SetTimer(nIDEvent, DEF_SENSOR_DISP_TM, NULL);
	}
}

// nState : 0:off, 1:on, -1:error
void CDlgManuLoadTbl::UpdateCylinderSts(int nCtrlId, int nStatus)
{
	CMFCButton*   pButton = (CMFCButton*)GetDlgItem(nCtrlId);
	if( nStatus == 0 ){
		pButton->SetFaceColor(COLOR_NOMAL, TRUE); 
	}else if( nStatus == 1 ){
		pButton->SetFaceColor(COLOR_GREEN, TRUE);
	}else{
		pButton->SetFaceColor(COLOR_DARKRED, TRUE); 
	}
}

BOOL CDlgManuLoadTbl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	MakeLog("Load Table Dialog Show");
// 	// exit
// 	m_cBmpExit.LoadBitmap(IDB_EXIT_CYLINDER_TESTPP);
// 	m_btnExit.SetBitmap(m_cBmpExit);

	CString strTitle = _T("");
	strTitle.Format("Loading Table Cylinder");
	::SetWindowText(this->m_hWnd, strTitle);

	m_cbOperationSel.Clear();
	m_cbOperationSel.AddString("1: Only One Action & No Check Sensor");
	m_cbOperationSel.AddString("2: Only One Action & Check Sensor");
	m_cbOperationSel.AddString("3: Continue Action & No Check Sensor");
	m_cbOperationSel.AddString("4: Continue Action & Check Sensor");
	m_cbOperationSel.AddString("5: Time Check Action (Once & Sensor Check)");
	m_cbOperationSel.SetCurSel(0);
	m_cbTimeSel.SetCurSel(0);

	CreateThread(0);
	SetTimer(0, DEF_SENSOR_DISP_TM, NULL);

	m_BtnStop.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnStop.SetIcon(IDI_STOP);
	m_BtnStop.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnExit.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnExit.SetIcon(IDI_BTN_EXIT);
	m_btnExit.SetAlign(CButtonST::ST_ALIGN_VERT);

	if (g_DMCont.m_dmEQP.GN(NDM0_TABLE_VIBRATOR_OPTION) == FALSE)
	{
		GetDlgItem(IDC_STATIC_VIBRATOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_VIBRATOR)->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgManuLoadTbl::CylinderManualAction()
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

//==============================================================================
//
//==============================================================================
void CDlgManuLoadTbl::CylManualOper_None()
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
void CDlgManuLoadTbl::CylManualOper_Only_One_Action_and_No_Check_Sensor()
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
void CDlgManuLoadTbl::CylManualOper_Only_One_Action_and_Check_Sensor()
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
void CDlgManuLoadTbl::CylManualOper_Continue_Action_and_No_Check_Sensor()
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
void CDlgManuLoadTbl::CylManualOper_Continue_Action_and_Check_Sensor()
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
void CDlgManuLoadTbl::CylManualOper_Time_Check_Action_Once_and_Sensor_Check()
{
	switch( m_nCylManualActionStep )
	{
	case 0:
		{
			m_nCylActionTimeRetry = 0;
			DisplayCylinderActionTime(TRUE);
			m_nCylManualActionStep = 1000;
			m_dCylActionTimeRecord = 0.0;
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
			if( dSpanTime > 10000.0 )
			{
// 				if( m_nCylActionTimeRetry < 1)
// 				{
// 					m_nCylManualActionStep = 1000;
// 					m_nCylActionTimeRetry++;
// 					break;
// 				}
// 				else
// 				{
					int nErr = Check_CylSensor();
					CParamList AlarmData;
					g_Error.AlarmReport(nErr, AlarmData, NULL);
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
//			g_DB.UPDATE_CYLINDER_TIME_TBL(m_nCylinderNum, m_dCylActionTimeRecord);
//			m_nCylinderNum = 0;
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	case 9000:
		{
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	}
}

void CDlgManuLoadTbl::DisplayCylinderActionTime(BOOL bClear/*=FALSE*/)
{
	CString strTime;
	strTime.Format("%.3f", m_dCylActionTimeRecord);
	if( bClear ) strTime = "";
	SetDlgItemText(UNITSETDLL_IDC_TBL_TIME, strTime);
}

void CDlgManuLoadTbl::ThreadProc1()
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

/*=============================================================================
  AUTHOR   : 나명진  [2015-10-06]
  NAME     : 
  PARAMS   : nCmd    : enum eCylManualCmd
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CDlgManuLoadTbl::SetCommand(int nCmd)
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

	m_nCmdManualCyl = nCmd;
	m_nCmdOperation = m_cbOperationSel.GetCurSel();
	m_nCmdParam_Time = m_cbTimeSel.GetCurSel()+1;

	m_nCylManualActionStep = 0;
	m_nCylManualOpActState = m_nCmdOperation;

// 현재 사용 안함.
//	CylinderSpecUpdate();
}

//==============================================================================
//
//==============================================================================
void CDlgManuLoadTbl::ClearCommand()
{
	m_nCmdManualCyl = eCylManualCmd_None;
	m_nCmdOperation = eCylManualOper_None;
	m_nCmdParam_Time = -1;
}

int CDlgManuLoadTbl::do_CylCommand(BOOL bUndo/*=FALSE*/)
{
	int nErrCode=0;
	switch ( m_nCmdManualCyl )
	{
	case eCylManualCmd_EachFwd:
		{
			int nCmd = (bUndo==FALSE)?DEF_FORW:DEF_BACK;
			nErrCode = g_TaskLoadTbl[m_nLoadTblIdx].do_cyl_2DBarcode_LeftRight(nCmd);
		}break;
	case eCylManualCmd_EachBak:
		{
			int nCmd = (bUndo==FALSE)?DEF_BACK:DEF_FORW;	
			nErrCode = g_TaskLoadTbl[m_nLoadTblIdx].do_cyl_2DBarcode_LeftRight(nCmd);
		}break;
	}
	return nErrCode;
}

int CDlgManuLoadTbl::Check_CylSensor(BOOL bUndo/*=FALSE*/)
{
	int nErrCode=0;
	switch ( m_nCmdManualCyl )
	{
	case eCylManualCmd_EachFwd:
		{
			int nCmd = (bUndo==FALSE)?DEF_FORW:DEF_BACK;
			nErrCode = g_TaskLoadTbl[m_nLoadTblIdx].chk_cyl_2DBarcode_LeftRight(nCmd);
		}break;
	case eCylManualCmd_EachBak:
		{
			int nCmd = (bUndo==FALSE)?DEF_BACK:DEF_FORW;
			nErrCode = g_TaskLoadTbl[m_nLoadTblIdx].chk_cyl_2DBarcode_LeftRight(nCmd);
		}break;
	}
	return nErrCode;
}

void CDlgManuLoadTbl::OnDestroy()
{
	CDialogEx::OnDestroy();
	KillTimer(0);
	CloseThread();	
}

void CDlgManuLoadTbl::OnBnClickedButtonTblCylActStop()
{
	ButtonClickLog(eClickBtn_Stop);
	m_nCylManualOpActState = eCylManualCmd_None;
}

void CDlgManuLoadTbl::ButtonClickLog(int nBtnNum)
{
	char szOperations[64] ={0,}, szBtnName[32]={0,};

	switch(nBtnNum)
	{
		case eClickBtn_Tbl1_2D_Left: sprintf_s(szBtnName,sizeof(szBtnName), "Tbl1 2D Barcode Cylinder Left"); break;
		case eClickBtn_Tbl1_2D_Right: sprintf_s(szBtnName,sizeof(szBtnName),"Tbl1 2D Barcode Cylinder Right"); break;
		case eClickBtn_Tbl2_2D_Left: sprintf_s(szBtnName,sizeof(szBtnName), "Tbl2 2D Barcode Cylinder Left"); break;
		case eClickBtn_Tbl2_2D_Right: sprintf_s(szBtnName,sizeof(szBtnName),"Tbl2 2D Barcode Cylinder Right"); break;
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
		MakeLog("%s Button Click Operations: %s, Time: %d sec", szBtnName, szOperations, nTime);
	}
}

void CDlgManuLoadTbl::MakeLog(LPCTSTR fmt, ...)
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
/*=============================================================================
  AUTHOR   : 손정일  [2017-05-27]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : Ver Arrive Safety Sensor사용 유무에 따라 Dialog 로 변경 되기 위해
==============================================================================*/
void CDlgManuLoadTbl::ChangeDialogSize(std::vector<int> vControl)
{
	GetDlgItem(IDC_STATIC_TXT12)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TXT13)->ShowWindow(SW_HIDE);
	///GetDlgItem(IDC_CTRL_ARV_TBL1_X1)->ShowWindow(SW_HIDE);

	for(int i=0; i<(int)vControl.size(); i++){
		HWND hwndBox = ::GetDlgItem(this->m_hWnd, vControl[i]);
		RECT rc1;
		::GetWindowRect(hwndBox, &rc1);

		GetDlgItem(vControl[i])->SetWindowPos(NULL,0,0,(rc1.right - rc1.left),(rc1.bottom - rc1.top)/2 + 15, SWP_NOMOVE);
	}
}

void CDlgManuLoadTbl::OnBnClickedButtonTbl1Left()
{
	ButtonClickLog(eClickBtn_Tbl1_2D_Left);
	m_nLoadTblIdx = eLoadTbl_1;
	SetCommand(eCylManualCmd_EachBak);
//	CylinderNum(DEF_LEFT);
}


void CDlgManuLoadTbl::OnBnClickedButtonTbl1Right()
{
	ButtonClickLog(eClickBtn_Tbl1_2D_Right);
	m_nLoadTblIdx = eLoadTbl_1;
	SetCommand(eCylManualCmd_EachFwd);
//	CylinderNum(DEF_RIGHT);
}


void CDlgManuLoadTbl::OnBnClickedButtonTbl2Left()
{
	ButtonClickLog(eClickBtn_Tbl2_2D_Left);
	m_nLoadTblIdx = eLoadTbl_3;
	SetCommand(eCylManualCmd_EachBak);
//	CylinderNum(DEF_LEFT);
}


void CDlgManuLoadTbl::OnBnClickedButtonTbl2Right()
{
	ButtonClickLog(eClickBtn_Tbl2_2D_Right);
	m_nLoadTblIdx = eLoadTbl_3;
	SetCommand(eCylManualCmd_EachFwd);
//	CylinderNum(DEF_RIGHT);
}



void CDlgManuLoadTbl::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}

void CDlgManuLoadTbl::CylinderSpecIndex(int& nCmdManualCyl)
{
	switch (m_nCmdManualCyl)
	{
	case eClickBtn_Tbl1_2D_Left:
	case eClickBtn_Tbl2_2D_Left:
	{
		nCmdManualCyl = eLoadTblLeft;
	}
	break;
	case eClickBtn_Tbl1_2D_Right:
	case eClickBtn_Tbl2_2D_Right:
	{
		nCmdManualCyl = eLoadTblRight;
	}
	break;

	default:
		break;
	}
}

void CDlgManuLoadTbl::CylinderSpecInOut(CDC* pDC)
{
	int nCmdManualCyl = 0;

	CylinderSpecIndex(nCmdManualCyl);

	if (m_dCylinderSpec[m_nLoadTblIdx][nCmdManualCyl].dMin <= m_dCylActionTimeRecord &&
		m_dCylinderSpec[m_nLoadTblIdx][nCmdManualCyl].dMax >= m_dCylActionTimeRecord)
		pDC->SetTextColor(RGB(0, 0, 255));
	else
		pDC->SetTextColor(RGB(255, 0, 0));

}

void CDlgManuLoadTbl::CylinderSpecUpdate()
{
	int nCmdManualCyl = 0;

	CylinderSpecIndex(nCmdManualCyl);

	CString str;
	str.Format("%.2f ~ %.2f", m_dCylinderSpec[m_nLoadTblIdx][nCmdManualCyl].dMin, m_dCylinderSpec[m_nLoadTblIdx][nCmdManualCyl].dMax);
	GetDlgItem(UNITSETDLL_IDC_STATIC_SPEC)->SetWindowText(str);
}

HBRUSH CDlgManuLoadTbl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
		case CTLCOLOR_STATIC:
		{
			if (pWnd->GetDlgCtrlID() == UNITSETDLL_IDC_TBL_TIME)
			{
				CylinderSpecInOut(pDC);
				//pDC->SetBkColor(RGB(0, 255, 0));
			}
		}
	}
	return hbr;
}

void CDlgManuLoadTbl::CylinderNum(BOOL bLeftRight)
{
	if(m_nLoadTblIdx == eLoadTbl_1)
		m_nCylinderNum = e2D_Barcode_Left_Left + m_nLoadTblIdx + bLeftRight;
	else
		m_nCylinderNum = e2D_Barcode_Left_Left + m_nLoadTblIdx + bLeftRight + 1;
}


void CDlgManuLoadTbl::BarcodeLED_OnOff(int nOnOff, int nLedIdx)
{
	m_nLEDOnOff = nOnOff;
	m_nLEDIdx = nLedIdx;

	if (nLedIdx == eBarcode_LED1) {
		g_pVisionTestHand_1->Cmd_ReqMeasure_BarcodeLED(nOnOff, nLedIdx);
	}
	else {
		g_pVisionTestHand_2->Cmd_ReqMeasure_BarcodeLED(nOnOff, nLedIdx);
	}

	SetTimer(1, DEF_SENSOR_DISP_TM, NULL);
}


BOOL CDlgManuLoadTbl::UpdateBarcodeLEDSts(int nOnOff, int nLedIdx)
{
	BOOL bChangeColor = FALSE;
	CMFCButton*   pBtnLedOn = (nLedIdx == eBarcode_LED1) ? (CMFCButton*)GetDlgItem(IDC_BUTTON_TBL1_LED_ON) : (CMFCButton*)GetDlgItem(IDC_BUTTON_TBL2_LED_ON);
	CMFCButton*   pBtnLedOff = (nLedIdx == eBarcode_LED1) ? (CMFCButton*)GetDlgItem(IDC_BUTTON_TBL1_LED_OFF) : (CMFCButton*)GetDlgItem(IDC_BUTTON_TBL2_LED_OFF);

	if (nLedIdx == eBarcode_LED1) {
		if (g_pVisionTestHand_1->m_BarcodeLightMsg.nReceived == eVisionMsgWaitState_Received) {
			bChangeColor = TRUE;
		}
	}
	else {
		if (g_pVisionTestHand_2->m_BarcodeLightMsg.nReceived == eVisionMsgWaitState_Received) {
			bChangeColor = TRUE;
		}
	}

	if (bChangeColor) {
		if (nOnOff == eBarcodeLight_ON) {
			pBtnLedOn->SetFaceColor(COLOR_GREEN, TRUE);
			pBtnLedOff->SetFaceColor(COLOR_NOMAL, TRUE);
		}
		else {
			pBtnLedOn->SetFaceColor(COLOR_NOMAL, TRUE);
			pBtnLedOff->SetFaceColor(COLOR_GREEN, TRUE);
		}
	}

	return bChangeColor;
}

void CDlgManuLoadTbl::OnBnClickedButtonTbl1LedOn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BarcodeLED_OnOff(eBarcodeLight_ON, eBarcode_LED1);

}


void CDlgManuLoadTbl::OnBnClickedButtonTbl1LedOff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BarcodeLED_OnOff(eBarcodeLight_OFF, eBarcode_LED1);
}


void CDlgManuLoadTbl::OnBnClickedButtonTbl2LedOn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BarcodeLED_OnOff(eBarcodeLight_ON, eBarcode_LED2);
}


void CDlgManuLoadTbl::OnBnClickedButtonTbl2LedOff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BarcodeLED_OnOff(eBarcodeLight_OFF, eBarcode_LED2);
}


void CDlgManuLoadTbl::OnBnClickedButtonVibrator()
{
	//Vibrator 다이얼로그 
	CDlgVibrator DlgVibrator;
	DlgVibrator.m_nUnit = eLoadTableVibratorUnit;
	DlgVibrator.DoModal();
}
