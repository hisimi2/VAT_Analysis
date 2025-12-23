// DlgManuTransfer.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgManuTransfer.h"
#include "afxdialogex.h"

#include "../Include/OcxFiles/dsspanel.h"
// CDlgManuTransfer 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgManuTransfer, CDialogEx)

CDlgManuTransfer::CDlgManuTransfer(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgManuTransfer::IDD, pParent)
{
	m_nCylManualOpActState = eCylManualOper_None;
	m_nCylManualActionStep = 0;
	m_nCmdCycleCount = 0;
	m_nCmdLoc    = -1;
	m_nCmdPos    = -1;
	m_dTimeCheck = 0.0;

	m_nCmdManualCyl = 0;
	m_nCmdOperation = 0;
	m_nCmdParam_Time = 0;
	m_dCylActionTimeRecord = 0.0;
	m_nCylActionTimeRetry = 0;
	m_nCylinderNum = 0;

	memset(&m_dCylinderSpec, 0x00, sizeof(m_dCylinderSpec));

	m_dCylinderSpec[eClickBtn_Clamp].dMax = 0.00;
	m_dCylinderSpec[eClickBtn_Clamp].dMin = 0.00;

	m_dCylinderSpec[eClickBtn_UnClamp].dMax = 0.00;
	m_dCylinderSpec[eClickBtn_UnClamp].dMin = 0.00;

	m_dCylinderSpec[eClickBtn_Up].dMax = 0.50;
	m_dCylinderSpec[eClickBtn_Up].dMin = 0.40;

	m_dCylinderSpec[eClickBtn_Down].dMax = 0.50;
	m_dCylinderSpec[eClickBtn_Down].dMin = 0.40;
}

CDlgManuTransfer::~CDlgManuTransfer()
{
	m_cBmpExit.DeleteObject();
}

void CDlgManuTransfer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_OPERATION, m_cbOperationSel);
	DDX_Control(pDX, IDC_COMBO_CONTINUE_TIME, m_cbTimeSel);

	DDX_Control(pDX, IDC_BTN_TRANSFER_UP1,		m_BtnTrans_Up);
	DDX_Control(pDX, IDC_BTN_TRANSFER_DOWN1,	m_BtnTrans_Down);
	DDX_Control(pDX, IDC_BTN_TRANSFER_CLAMP1,	m_BtnTrans_Clamp);
	DDX_Control(pDX, IDC_BTN_TRANSFER_UNCLAMP1, m_BtnTrans_Unclamp);

	DDX_Control(pDX, IDC_BUTTON_TRANS_CYL_ACT_STOP, m_BtnStop);

	// exit
	DDX_Control(pDX, IDOK, m_btnExit);
}


BEGIN_MESSAGE_MAP(CDlgManuTransfer, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_TRANS_CYL_ACT_STOP, &CDlgManuTransfer::OnBnClickedButtonTransCylActStop)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_TRANSFER_UP1, &CDlgManuTransfer::OnBnClickedBtnTransferUp1)
	ON_BN_CLICKED(IDC_BTN_TRANSFER_DOWN1, &CDlgManuTransfer::OnBnClickedBtnTransferDown1)
	ON_BN_CLICKED(IDC_BTN_TRANSFER_CLAMP1, &CDlgManuTransfer::OnBnClickedBtnTransferClamp1)
	ON_BN_CLICKED(IDC_BTN_TRANSFER_UNCLAMP1, &CDlgManuTransfer::OnBnClickedBtnTransferUnclamp1)
	ON_BN_CLICKED(IDOK, &CDlgManuTransfer::OnBnClickedOk)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgManuTransfer 메시지 처리기입니다.


BOOL CDlgManuTransfer::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// exit
	m_cBmpExit.LoadBitmap(IDB_EXIT_CYLINDER_TRANSFER);
	m_btnExit.SetBitmap(m_cBmpExit);

	m_cbOperationSel.Clear();
	m_cbOperationSel.AddString("1: Only One Action & No Check Sensor");
	m_cbOperationSel.AddString("2: Only One Action & Check Sensor");
	m_cbOperationSel.AddString("3: Continue Action & No Check Sensor");
	m_cbOperationSel.AddString("4: Continue Action & Check Sensor");
	m_cbOperationSel.AddString("5: Time Check Action (Once & Sensor Check)");
	m_cbOperationSel.SetCurSel(0);

	m_cbTimeSel.SetCurSel(0);

	m_BtnStop.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnStop.SetIcon(IDI_STOP);
	m_BtnStop.SetAlign(CButtonST::ST_ALIGN_VERT);

	SetTimer(0, DEF_SENSOR_DISP_TM, NULL);

	CreateThread(0);
	MakeLog("Transfer Dialog Show");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgManuTransfer::OnTimer(UINT_PTR nIDEvent)
{
	CDSSPanel * cchr  = NULL;

	if(nIDEvent == 0)
	{
		KillTimer(nIDEvent);

		BOOL Ret_Clamp = FALSE, Ret_Unclamp = FALSE;
		Ret_Clamp |= g_TaskTransfer.GetDI( CTaskTransfer::xTRANSFER_LEFT_CLAMP_SEN );
		Ret_Clamp |= g_TaskTransfer.GetDI( CTaskTransfer::xTRANSFER_RIGHT_CLAMP_SEN );

		Ret_Unclamp |= g_TaskTransfer.GetDI( CTaskTransfer::xTRANSFER_LEFT_UNCLAMP_SEN );
		Ret_Unclamp |= g_TaskTransfer.GetDI( CTaskTransfer::xTRANSFER_RIGHT_UNCLAMP_SEN );

		BOOL Ret_Up = g_TaskTransfer.GetDI( CTaskTransfer::xTRANSFER_UP_SEN );
		BOOL Ret_Down = g_TaskTransfer.GetDI( CTaskTransfer::xTRANSFER_DOWN_SEN );

		UpdateCylinderSts( IDC_BTN_TRANSFER_CLAMP1, Ret_Clamp );
		UpdateCylinderSts( IDC_BTN_TRANSFER_UNCLAMP1, Ret_Unclamp );
		UpdateCylinderSts( IDC_BTN_TRANSFER_UP1, Ret_Up );
		UpdateCylinderSts( IDC_BTN_TRANSFER_DOWN1, Ret_Down );
		
		// Sensor 
		cchr = ( CDSSPanel* )GetDlgItem( IDC_TRANS_EXIST_SEN );
		if( g_TaskTransfer.GetDI( CTaskTransfer::xTRANSFER_TRAY_EXIST_SEN ) )	cchr->On();
		else																	cchr->Off();

		//-- sequence busy display
		cchr = (CDSSPanel*)GetDlgItem(IDC_COLORCHECKBTCTRL_TRANS_BUSY);
		if( m_nCylManualOpActState != eCylManualOper_None ){
			cchr->On();
		}else{
			cchr->Off();
		}

		SetTimer(nIDEvent, DEF_SENSOR_DISP_TM, NULL);
	}

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CDlgManuTransfer::PreTranslateMessage(MSG* pMsg)
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


void CDlgManuTransfer::OnBnClickedButtonTransCylActStop()
{
	ButtonClickLog(eClickBtn_Stop);
	m_nCmdLoc = -1;
	m_nCmdPos = -1;
	m_nCylManualOpActState = eCylManualCmd_None;
}

void CDlgManuTransfer::ThreadProc1()
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

void CDlgManuTransfer::CylinderManualAction()
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

void CDlgManuTransfer::OnDestroy()
{
	CDialogEx::OnDestroy();
	KillTimer(0);
	CloseThread();	
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-10-06]
  NAME     : 
  PARAMS   : nCmd    : enum eCylManualCmd
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CDlgManuTransfer::SetCommand(int nCmd, int nPosition/*=0*/,int nIdx/*=0*/)
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
	m_nCmdPos = nPosition;
	m_nCmdLoc = nIdx;
	m_nCmdOperation = m_cbOperationSel.GetCurSel();
	m_nCmdParam_Time = m_cbTimeSel.GetCurSel()+1;

	m_nCylManualActionStep = 0;
	m_nCylManualOpActState = m_nCmdOperation;

	CylinderSpecUpdate();
}


//==============================================================================
//
//==============================================================================
void CDlgManuTransfer::ClearCommand()
{
	m_nCmdManualCyl = eCylManualCmd_None;
	m_nCmdParam_Time = -1;
	m_nCmdLoc    = -1;
	m_nCmdPos    = -1;
}

int CDlgManuTransfer::do_CylCommand(BOOL bUndo/*=FALSE*/)
{
	int nErrCode=0;
	switch ( m_nCmdManualCyl )
	{
	case eCylManualCmd_EachClamp:
		{			
			int nCmd = /*DEF_CLAMP;//*/(bUndo==FALSE)?DEF_CLAMP:DEF_UNCLAMP;	
			nErrCode = g_TaskTransfer.do_cyl_ClampUnclamp(nCmd);
		}break;
	case eCylManualCmd_EachUnclamp:
		{
			int nCmd = /*DEF_UNCLAMP;//*/(bUndo==FALSE)?DEF_UNCLAMP:DEF_CLAMP;		
			nErrCode = g_TaskTransfer.do_cyl_ClampUnclamp(nCmd);
		}break;
	case eCylManualCmd_EachUp:
		{
			int nCmd = /*DEF_UP;//*/(bUndo==FALSE)?DEF_UP:DEF_DOWN;		
			nErrCode = g_TaskTransfer.do_cyl_UpDown(nCmd);
		}break;
	case eCylManualCmd_EachDown:
		{
			int nCmd = /*DEF_DOWN;//*/(bUndo==FALSE)?DEF_DOWN:DEF_UP;		
			nErrCode = g_TaskTransfer.do_cyl_UpDown(nCmd);
		}break;
	}
	return nErrCode;
}

int CDlgManuTransfer::Check_CylSensor(BOOL bUndo/*=FALSE*/)
{
	int nErrCode=0;
	switch ( m_nCmdManualCyl )
	{
	case eCylManualCmd_EachClamp:
		{			
			int nCmd = (bUndo==FALSE)?DEF_CLAMP:DEF_UNCLAMP;	
			nErrCode = g_TaskTransfer.chk_cyl_ClampUnclamp(nCmd);
		}break;
	case eCylManualCmd_EachUnclamp:
		{
			int nCmd = (bUndo==FALSE)?DEF_UNCLAMP:DEF_CLAMP;		
			nErrCode = g_TaskTransfer.chk_cyl_ClampUnclamp(nCmd);
		}break;
	case eCylManualCmd_EachUp:
		{
			int nCmd = (bUndo==FALSE)?DEF_UP:DEF_DOWN;		
			nErrCode = g_TaskTransfer.chk_cyl_UpDown(nCmd);
		}break;
	case eCylManualCmd_EachDown:
		{
			int nCmd = (bUndo==FALSE)?DEF_DOWN:DEF_UP;		
			nErrCode = g_TaskTransfer.chk_cyl_UpDown(nCmd);
		}break;
	}
	return nErrCode;
}
//==============================================================================
//
//==============================================================================
void CDlgManuTransfer::CylManualOper_None()
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
void CDlgManuTransfer::CylManualOper_Only_One_Action_and_No_Check_Sensor()
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
			ClearCommand();
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	}
}


//==============================================================================
// cylinder 한번만 동작하고 센서체크 함.
//==============================================================================
void CDlgManuTransfer::CylManualOper_Only_One_Action_and_Check_Sensor()
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
			if( dSpanTime > ((double)m_nCmdParam_Time * 1000.0) )
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
			ClearCommand();
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	}
}
//==============================================================================
// 작업자가  stop하기 전까지 cylinder 동작하고 센서체크 하지 않음.
//==============================================================================
void CDlgManuTransfer::CylManualOper_Continue_Action_and_No_Check_Sensor()
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
			if( dSpanTime > ((double)m_nCmdParam_Time * 1000.0) )
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
			ClearCommand();
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	}
}
//==============================================================================
// 작업자가 stop하기 전까지 cylinder 동작하고 센서체크 함.
//==============================================================================
void CDlgManuTransfer::CylManualOper_Continue_Action_and_Check_Sensor()
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
			if( dSpanTime > ((double)m_nCmdParam_Time * 1000.0) )
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
			if( dSpanTime > ((double)m_nCmdParam_Time * 1000.0) )
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
			ClearCommand();
		}break;
	}
}
//==============================================================================
// 작업자가 stop하기 전까지 cylinder 동작하고 센서체크 함.
//==============================================================================
void CDlgManuTransfer::CylManualOper_Time_Check_Action_Once_and_Sensor_Check()
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
			if( dSpanTime > (100000.0) )
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
					g_Error.AlarmReport(ERR_TRNASFER_UP_TIMEOUT, AlarmData, NULL);
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
			g_DB.UPDATE_CYLINDER_TIME_TBL(m_nCylinderNum, m_dCylActionTimeRecord);
			m_nCylinderNum = 0;
			m_nCylManualOpActState = eCylManualOper_None;
		}break;
	case 9000:
		{
			m_nCylManualOpActState = eCylManualOper_None;
			ClearCommand();
		}break;
	}
}

void CDlgManuTransfer::DisplayCylinderActionTime(BOOL bClear/*=FALSE*/)
{
	CString strTime;
	strTime.Format("%.3f", m_dCylActionTimeRecord);
	if( bClear ) strTime = "";
	SetDlgItemText(UNITSETDLL_IDC_TRANS_TIME, strTime);
}

void CDlgManuTransfer::UpdateSensorSts(int nCtrlId, int nDiId)
{
	CDSSPanel*    pSensor = NULL;
	pSensor = (CDSSPanel*)GetDlgItem(nCtrlId);
	BOOL bVal = g_IO.in(nDiId);
	if( bVal ) pSensor->On();
	else       pSensor->Off();
}

// nState : 0:off, 1:on, -1:error
void CDlgManuTransfer::UpdateCylinderSts(int nCtrlId, int nStatus)
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

//-----------------------------------------------------
// Transfer Manual 동작 Log
//-----------------------------------------------------
void CDlgManuTransfer::ButtonClickLog(int nBtnNum)
{
	char szOperations[64] ={0,}, szBtnName[32]={0,};

	switch(nBtnNum)
	{
		case eClickBtn_Clamp    : sprintf_s(szBtnName,sizeof(szBtnName),"Clamp"); break;
		case eClickBtn_UnClamp  : sprintf_s(szBtnName,sizeof(szBtnName),"UnClamp"); break;
		case eClickBtn_Up		: sprintf_s(szBtnName,sizeof(szBtnName),"Up"); break;
		case eClickBtn_Down		: sprintf_s(szBtnName,sizeof(szBtnName),"Down"); break;
		case eClickBtn_Stop     : MakeLog("Stop Button Click"); break;
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

void CDlgManuTransfer::MakeLog(LPCTSTR fmt, ...)
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


void CDlgManuTransfer::OnBnClickedBtnTransferUp1()
{
	ButtonClickLog(eClickBtn_Up);
	SetCommand(eCylManualCmd_EachUp);
	CylinderNum(DEF_UP);
}


void CDlgManuTransfer::OnBnClickedBtnTransferDown1()
{
	ButtonClickLog(eClickBtn_Down);
	SetCommand(eCylManualCmd_EachDown);
	CylinderNum(DEF_DOWN);
}


void CDlgManuTransfer::OnBnClickedBtnTransferClamp1()
{
	ButtonClickLog(eClickBtn_Clamp);
	SetCommand(eCylManualCmd_EachClamp);
}


void CDlgManuTransfer::OnBnClickedBtnTransferUnclamp1()
{
	ButtonClickLog(eClickBtn_UnClamp);
	SetCommand(eCylManualCmd_EachUnclamp);
}


void CDlgManuTransfer::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

void CDlgManuTransfer::CylinderSpecIndex(int& nCmdManualCyl)
{
	switch (m_nCmdManualCyl)
	{
	case eCylManualCmd_EachClamp:
	{
		nCmdManualCyl = eClickBtn_Clamp;
	}
	break;
	case eCylManualCmd_EachUnclamp:
	{
		nCmdManualCyl = eClickBtn_UnClamp;
	}
	break;
	case eCylManualCmd_EachUp:
	{
		nCmdManualCyl = eClickBtn_Up;
	}
	break;
	case eCylManualCmd_EachDown:
	{
		nCmdManualCyl = eClickBtn_Down;
	}
	break;

	default:
		break;
	}
}

void CDlgManuTransfer::CylinderSpecInOut(CDC* pDC)
{
	int nCmdManualCyl = 0;

	CylinderSpecIndex(nCmdManualCyl);

	if (m_dCylinderSpec[nCmdManualCyl].dMin <= m_dCylActionTimeRecord &&
		m_dCylinderSpec[nCmdManualCyl].dMax >= m_dCylActionTimeRecord)
		pDC->SetTextColor(RGB(0, 0, 255));
	else
		pDC->SetTextColor(RGB(255, 0, 0));

}

void CDlgManuTransfer::CylinderSpecUpdate()
{
	int nCmdManualCyl = 0;

	CylinderSpecIndex(nCmdManualCyl);

	CString str;
	str.Format("%.2f ~ %.2f", m_dCylinderSpec[nCmdManualCyl].dMin, m_dCylinderSpec[nCmdManualCyl].dMax);
	GetDlgItem(UNITSETDLL_IDC_TRANS_SPEC)->SetWindowText(str);
}

HBRUSH CDlgManuTransfer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
		case CTLCOLOR_STATIC:
		{
			if (pWnd->GetDlgCtrlID() == UNITSETDLL_IDC_TRANS_TIME)
			{
				CylinderSpecInOut(pDC);
				//pDC->SetBkColor(RGB(0, 255, 0));
			}
		}
	}
	return hbr;
}

void CDlgManuTransfer::CylinderNum(BOOL bUpDown)
{
	m_nCylinderNum = eTransfer_Z_axis_Up + bUpDown;
}
