// DlgManuFeeder.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgManuFeeder.h"
#include "afxdialogex.h"
#include "../Include/OcxFiles/dsspanel.h"

// CDlgManuFeeder 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgManuFeeder, CDialogEx)

CDlgManuFeeder::CDlgManuFeeder(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MANU_FEEDER, pParent)
{
	m_nCylManualOpActState = eCylManualOper_None;
	m_nCylManualActionStep = 0;

	m_nCmdManualCyl = 0;
	m_nCmdOperation = 0;
	m_nCmdParam_Time = 0;
	m_dTimeCheck = 0.0;
	m_dCylActionTimeRecord = 0.0;
}

CDlgManuFeeder::~CDlgManuFeeder()
{
}

void CDlgManuFeeder::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnExit);
	DDX_Control(pDX, IDC_BUTTON_FEEDER_CYL_ACT_STOP, m_BtnStop);
	DDX_Control(pDX, IDC_COMBO_OPERATION, m_cbOperationSel);
	DDX_Control(pDX, IDC_COMBO_CONTINUE_TIME, m_cbTimeSel);

	DDX_Control(pDX, IDC_BTN_FEEDER_UP, m_BtnFeeder_Up);
	DDX_Control(pDX, IDC_BTN_FEEDER_DOWN, m_BtnFeeder_Down);
	DDX_Control(pDX, IDC_BTN_FEEDER_CLAMP, m_BtnFeeder_Clamp);
	DDX_Control(pDX, IDC_BTN_FEEDER_UNCLAMP, m_BtnFeeder_Unclamp);
	DDX_Control(pDX, IDC_BTN_FEEDER_LOCK, m_BtnFeeder_Lock);
	DDX_Control(pDX, IDC_BTN_FEEDER_UNLOCK, m_BtnFeeder_Unlock);
}


BEGIN_MESSAGE_MAP(CDlgManuFeeder, CDialogEx)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_FEEDER_UP, &CDlgManuFeeder::OnBnClickedBtnFeederUp)
	ON_BN_CLICKED(IDC_BTN_FEEDER_DOWN, &CDlgManuFeeder::OnBnClickedBtnFeederDown)
	ON_BN_CLICKED(IDC_BTN_FEEDER_CLAMP, &CDlgManuFeeder::OnBnClickedBtnFeederClamp)
	ON_BN_CLICKED(IDC_BTN_FEEDER_UNCLAMP, &CDlgManuFeeder::OnBnClickedBtnFeederUnclamp)
	ON_BN_CLICKED(IDC_BTN_FEEDER_LOCK, &CDlgManuFeeder::OnBnClickedBtnFeederLock)
	ON_BN_CLICKED(IDC_BTN_FEEDER_UNLOCK, &CDlgManuFeeder::OnBnClickedBtnFeederUnlock)
	ON_BN_CLICKED(IDC_BUTTON_FEEDER_CYL_ACT_STOP, &CDlgManuFeeder::OnBnClickedButtonFeederCylActStop)
	ON_BN_CLICKED(IDOK, &CDlgManuFeeder::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_TRAY_OUT, &CDlgManuFeeder::OnBnClickedBtnTrayOut)
END_MESSAGE_MAP()


// CDlgManuFeeder 메시지 처리기입니다.
BOOL CDlgManuFeeder::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	MakeLog("Feeder Dialog Show");

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

	m_BtnStop.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnStop.SetIcon(IDI_STOP);
	m_BtnStop.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnExit.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_btnExit.SetIcon(IDI_BTN_EXIT);
	m_btnExit.SetAlign(CButtonST::ST_ALIGN_VERT);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}




void CDlgManuFeeder::OnDestroy()
{
	CDialogEx::OnDestroy();
	KillTimer(0);
	CloseThread();
}

void CDlgManuFeeder::OnTimer(UINT_PTR nIDEvent)
{
	CDSSPanel* cchr = NULL;
	if (nIDEvent == 0)
	{
		KillTimer(nIDEvent);

		//eOnOffState Ret_Clamp = eOnOffState_Unknown, Ret_Unclamp = eOnOffState_Unknown;
		BOOL Ret_Clamp = g_TaskTrayFeeder.chk_cyl_ClampUnclamp(eClamp); //return 값 확인.
		BOOL Ret_Unclamp = g_TaskTrayFeeder.chk_cyl_ClampUnclamp(eUnclamp);
		
		BOOL Ret_Up = g_TaskTrayFeeder.chk_cyl_UpDown(eUp);
		BOOL Ret_Down = g_TaskTrayFeeder.chk_cyl_UpDown(eDown);

		BOOL Ret_Align_Forward = g_TaskTrayFeeder.chk_cyl_align_ForBack(eForw);
		BOOL Ret_Align_Backward = g_TaskTrayFeeder.chk_cyl_align_ForBack(eBack);

		UpdateCylinderSts(IDC_BTN_FEEDER_CLAMP, Ret_Clamp);
		UpdateCylinderSts(IDC_BTN_FEEDER_UNCLAMP, Ret_Unclamp);
		UpdateCylinderSts(IDC_BTN_FEEDER_UP, Ret_Up);
		UpdateCylinderSts(IDC_BTN_FEEDER_DOWN, Ret_Down);
		UpdateCylinderSts(IDC_BTN_FEEDER_LOCK, Ret_Align_Forward);
		UpdateCylinderSts(IDC_BTN_FEEDER_UNLOCK, Ret_Align_Backward);

		//Safety
		cchr = (CDSSPanel*)GetDlgItem(IDC_FEEDER_Y_SAFETY_SEN);
		if (g_TaskTrayFeeder.GetSafetySensor()) cchr->On();
		else									cchr->Off();

		//Sensor, Tray Exist(Transfer Area)
		cchr = (CDSSPanel*)GetDlgItem(IDC_FEEDER_TRANSFER_AREA_TRAY_DETECT_SEN);
		if (g_TaskTrayFeeder.GetTrayExist_TransferArea_Sen(eExist) == ERR_NO_ERROR) cchr->On();
		else																   cchr->Off();

		//Sensor, Tray Exist(feeder)
		cchr = (CDSSPanel*)GetDlgItem(IDC_FEEDER_TRAY_EXIST);
		if (g_TaskTrayFeeder.GetTrayExistSensor())				cchr->On();
		else											cchr->Off();

		//Sensor, Tray Exist(conveyor3 Rear)
		cchr = (CDSSPanel*)GetDlgItem(IDC_STACKER3_REAR_DETECT_SEN);
		if (g_TaskStacker[eSTACKER_03].GetExistTray_RearPos_Sen(eExist) == ERR_NO_ERROR) cchr->On();
		else																	cchr->Off();

		//busy 
		cchr = (CDSSPanel*)GetDlgItem(IDC_COLORCHECKBTCTRL_FEEDER_BUSY);
		if (m_nCylManualOpActState != eCylManualOper_None)				cchr->On();
		else															cchr->Off();
		
		SetTimer(nIDEvent, DEF_SENSOR_DISP_TM, NULL);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgManuFeeder::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = { 0, };
	if (fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf_s(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_SYSTEM), Debug, "", __LINE__, NULL, tmpMsg);
}

void CDlgManuFeeder::ThreadProc1()
{
	SetThreadInterval(0, 50);
	while (GetThreadAliveFlag(0)) {
		if (GetPauseFlag(0)) {
			NEXT;
			continue;
		}
		CylinderManualAction();
		NEXT;
	}
}

void CDlgManuFeeder::UpdateCylinderSts(int nCtrlId, int nStatus)
{
	CMFCButton*   pButton = (CMFCButton*)GetDlgItem(nCtrlId);
// 	if (nStatus == 0) {
// 		pButton->SetFaceColor(COLOR_NOMAL, TRUE);
// 	}
// 	else if (nStatus == 1) {
// 		pButton->SetFaceColor(COLOR_GREEN, TRUE);
// 	}
// 	else {
// 		pButton->SetFaceColor(COLOR_DARKRED, TRUE);
// 	}

	if (nStatus == 0) //ERR_NO_ERR
	{
		pButton->SetFaceColor(COLOR_GREEN, TRUE);
	}
	else
	{
		pButton->SetFaceColor(COLOR_NOMAL, TRUE);
	}
}

void CDlgManuFeeder::OnBnClickedBtnFeederUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Up);
	SetCommand(eCylManualCmd_EachUp);
}


void CDlgManuFeeder::OnBnClickedBtnFeederDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Down);

	////Feeder Down Interlock 추가/////
// 	int nChkLoadPos = g_TaskTrayFeeder.ChkMotorPos(CTaskTrayFeeder::eMotor::eAxisY, eLocIdxFeeder::LOC_IDX_FEEDER_TRAY_PICK_POS);
// 	int nChkUnloadPos = g_TaskTrayFeeder.ChkMotorPos(CTaskTrayFeeder::eMotor::eAxisY, eLocIdxFeeder::LOC_IDX_FEEDER_TRAY_PLACE_POS);
	int nchk_feeder_tray_exist = g_TaskTrayFeeder.GetTrayExistSensor();
	int nChkCurPos = g_TaskTrayFeeder.WhereisMotorPos();

	//Load, Unload 위치가 아닐 경우 Gripper Unclamp 확인.
	if (nChkCurPos != CTaskTrayFeeder::ePosY_LoadPos && nChkCurPos != CTaskTrayFeeder::ePosY_UnloadPos)
	{
		auto chk_unclamp = g_TaskTrayFeeder.get_cyl_ClampUnclamp(eUnclamp);
		if (chk_unclamp != ERR_NO_ERROR) {
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_FEEDER_DOWN_CYLINDER_FAIL_BECAUSE_GRIPPER_NOT_UNCLAMP, AlarmData, NULL);
			return;
		}
 	}

	//Load 위치 일경우, conveyor#3에 Tray 있는지 확인.
	if (nChkCurPos == CTaskTrayFeeder::ePosY_LoadPos)
	{
		int nErr = g_TaskStacker[eSTACKER_03].GetExistTray_RearPos_Sen(eExist);
		
		if (nErr == ERR_NO_ERROR && nchk_feeder_tray_exist == TRUE) {
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_STACKER_3_TRAY_EXIST_REAR_POS, AlarmData, NULL);
			return;
		}

		int nchk_conveyor_align_cylinder = g_TaskStacker[eSTACKER_03].get_cyl_TrayAlign(DEF_BACK);
		if (nchk_feeder_tray_exist == TRUE && nchk_conveyor_align_cylinder != ERR_NO_ERROR)
		{
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_LOT_CMD_CONVEYOR_TRAY_ALIGN_IS_NOT_BACK_STACKER_3, AlarmData, NULL);
			return;
		}
	}

	//Unload 위치 일경우, conveyor#3에 Tray 있는지 확인.
	if (nChkCurPos == CTaskTrayFeeder::ePosY_UnloadPos)
	{
		int nErr = g_TaskTrayFeeder.GetTrayExist_TransferArea_Sen(eExist);
		if (nErr == ERR_NO_ERROR && nchk_feeder_tray_exist == TRUE) {
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_FEEDER_TRANSFER_AREA_TRAY_DETECTED, AlarmData, NULL);
			return;
		}
	}

	//Feeder Tray 감지될 때, Align Cylinder Back 아니면 Down 안되도록 Interlock
	auto align_back_state = g_TaskTrayFeeder.get_cyl_align_ForBack(eBack);
	if (nchk_feeder_tray_exist == TRUE && align_back_state != ERR_NO_ERROR)
	{
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_FEEDER_DOWN_CYLINDER_FAIL_BECAUSE_ALIGN_CYLINDER_IS_NOT_BACK, AlarmData, NULL);
		return;
	}

	SetCommand(eCylManualCmd_EachDown);
}


void CDlgManuFeeder::OnBnClickedBtnFeederClamp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Clamp);

	////Clamp Interlock
	////Feeder Down Interlock 추가/////
// 	int nChkLoadPos = g_TaskTrayFeeder.ChkMotorPos(CTaskTrayFeeder::eMotor::eAxisY, eLocIdxFeeder::LOC_IDX_FEEDER_TRAY_PICK_POS);
// 	int nChkUnloadPos = g_TaskTrayFeeder.ChkMotorPos(CTaskTrayFeeder::eMotor::eAxisY, eLocIdxFeeder::LOC_IDX_FEEDER_TRAY_PLACE_POS);

	int nChkCurPos = g_TaskTrayFeeder.WhereisMotorPos();

	//Load, Unload 위치가 아닐 경우 Gripper Unclamp 확인.
	int nchk_feeder_down = g_TaskTrayFeeder.get_cyl_UpDown(eDown);
	if ( nchk_feeder_down == ERR_NO_ERROR && nChkCurPos != CTaskTrayFeeder::ePosY_LoadPos && nChkCurPos != CTaskTrayFeeder::ePosY_UnloadPos)
	{
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_FEEDER_GRIPPER_CLAMP_FAIL_BECUASE_LOCATION_IS_FAIL, AlarmData, NULL);
		return;
	}

	SetCommand(eCylManualCmd_EachClamp);
}


void CDlgManuFeeder::OnBnClickedBtnFeederUnclamp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Unclamp);

	////Feeder Unclamp Interlock.
	//1. Feeder Up일 경우, Tray 존재하면 Unclamp 불가.
	int nChkUp = g_TaskTrayFeeder.get_cyl_UpDown(eUp);
	int nChkTrayExist = g_TaskTrayFeeder.GetTrayExistSensor(); //B접
	if (nChkUp == ERR_NO_ERROR && nChkTrayExist == TRUE){
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_FEEDER_TRAY_DETECTED, AlarmData, NULL);
		return;
	}

	SetCommand(eCylManualCmd_EachUnclamp);
}


void CDlgManuFeeder::OnBnClickedBtnFeederLock()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Forward);
	SetCommand(eCylManualCmd_EachFwd);
}


void CDlgManuFeeder::OnBnClickedBtnFeederUnlock()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MANUAL, ERR_VAT_TYPE_YESNO);
	if (nRes == IDNO)
		return;

	ButtonClickLog(eClickBtn_Backward);

	////Feeder Align Cylinder Backward Interlock.
	//1. Feeder Up & Gripper Unclamp 상태일 때, Align Cylinder Backward 불가.
	int nChkUp = g_TaskTrayFeeder.get_cyl_UpDown(eUp);
	int nChkUnclamp = g_TaskTrayFeeder.get_cyl_ClampUnclamp(eUnclamp);
	int nChkTrayExist = g_TaskTrayFeeder.GetTrayExistSensor(); //B접
	if (nChkUp == ERR_NO_ERROR && nChkUnclamp == TRUE && nChkTrayExist == TRUE) {
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_FEEDER_TRAY_ALIGN_CYLINDER_BACK_FAIL_BECAUSE_TRAY_EXIST_AND_GRIPPER_IS_UNCLAMP, AlarmData, NULL);
		return;
	}

	SetCommand(eCylManualCmd_EachBak);
}


void CDlgManuFeeder::OnBnClickedButtonFeederCylActStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ButtonClickLog(eClickBtn_Stop);
	m_nCylManualOpActState = eCylManualCmd_None;

	ButtonLockUnlock(TRUE);
}


void CDlgManuFeeder::ButtonClickLog(int nBtnNum)
{
	char szOperations[64] = { 0, }, szBtnName[32] = { 0, };

	switch (nBtnNum)
	{
	case eClickBtn_Up: sprintf_s(szBtnName, sizeof(szBtnName), "Up"); break;
	case eClickBtn_Down: sprintf_s(szBtnName, sizeof(szBtnName), "Down"); break;
	case eClickBtn_Clamp: sprintf_s(szBtnName, sizeof(szBtnName), "Clamp"); break;
	case eClickBtn_Unclamp: sprintf_s(szBtnName, sizeof(szBtnName), "UnClamp"); break;
	case eClickBtn_Forward: sprintf_s(szBtnName, sizeof(szBtnName), "Forward"); break;
	case eClickBtn_Backward: sprintf_s(szBtnName, sizeof(szBtnName), "Backward"); break;
	case eClickBtn_Stop: MakeLog("Stop Button Click"); break;
	}

	if (nBtnNum != eClickBtn_Stop) {
		int nTime = m_cbTimeSel.GetCurSel() + 1;
		switch (m_cbOperationSel.GetCurSel())
		{
		case 0: sprintf_s(szOperations, sizeof(szOperations), "Only One Action & No Check Sensor"); break;
		case 1: sprintf_s(szOperations, sizeof(szOperations), "Only One Action & Check Sensor"); break;
		case 2: sprintf_s(szOperations, sizeof(szOperations), "Continue Action & No Check Sensor"); break;
		case 3: sprintf_s(szOperations, sizeof(szOperations), "Continue Action & Check Sensor"); break;
		case 4: sprintf_s(szOperations, sizeof(szOperations), "Time Check Action (Once & Sensor Check)"); break;
		}
		MakeLog("%s Button Click Operations: %s, Time: %d sec", szBtnName, szOperations, nTime);
	}
}

void CDlgManuFeeder::SetCommand(int nCmd, int nPosition /*= 0*/, int nIdx /*= 0*/)
{
	if (m_nCylManualOpActState != eCylManualCmd_None) {
		return;
	}
	if (m_cbOperationSel.GetCurSel() < 0) {
		return;
	}
	if (m_cbTimeSel.GetCurSel() < 0) {
		return;
	}

	m_nCmdManualCyl = nCmd;
	m_nCmdOperation = m_cbOperationSel.GetCurSel();
	m_nCmdParam_Time = m_cbTimeSel.GetCurSel() + 1;

	m_nCylManualActionStep = 0;
	m_nCylManualOpActState = m_nCmdOperation;
}

void CDlgManuFeeder::CylinderManualAction()
{
	switch (m_nCylManualOpActState)
	{
	case eCylManualOper_None:
	{
		CylManualOper_None();
	}break;
	case eCylManualOper_Only_One_Action_and_No_Check_Sensor:
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
	case 100: //Tray Out
	{
		CylManualOper_C_Tray_Out_Action();
	}
	}
}

//==============================================================================
// 아무것도 하지 않고 대기
//==============================================================================
void CDlgManuFeeder::CylManualOper_None()
{
	switch (m_nCylManualActionStep)
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
void CDlgManuFeeder::CylManualOper_Only_One_Action_and_No_Check_Sensor()
{
	switch (m_nCylManualActionStep)
	{
	case 0:
	{
		int nErr = do_CylCommand();
		if (nErr != ERR_NO_ERROR) {
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
void CDlgManuFeeder::CylManualOper_Only_One_Action_and_Check_Sensor()
{
	switch (m_nCylManualActionStep)
	{
	case 0:
	{
		int nErr = do_CylCommand();
		if (nErr != ERR_NO_ERROR) {
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
		if (dSpanTime > ((double)m_nCmdParam_Time * 1000.0))
		{
			m_nCylManualActionStep++;
		}
	}break;
	case 2:
	{
		int nErr = Check_CylSensor();
		if (nErr != ERR_NO_ERROR) {
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
void CDlgManuFeeder::CylManualOper_Continue_Action_and_No_Check_Sensor()
{
	switch (m_nCylManualActionStep)
	{
	case 0:
	{
//		m_nCmdCycleCount = 0;
		m_nCylManualActionStep = 1000;
	}break;
	case 1000:
	{
		int nErr = do_CylCommand();
		if (nErr != ERR_NO_ERROR) {
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
		if (dSpanTime > ((double)m_nCmdParam_Time * 1000.0))
		{
//			m_nCmdCycleCount++;
			m_nCylManualActionStep = 2000;
		}
	}break;
	case 2000:
	{
		int nErr = do_CylCommand(TRUE);
		if (nErr != ERR_NO_ERROR) {
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
		if (dSpanTime > ((double)m_nCmdParam_Time*1000.0))
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
void CDlgManuFeeder::CylManualOper_Continue_Action_and_Check_Sensor()
{
	switch (m_nCylManualActionStep)
	{
	case 0:
	{
//		m_nCmdCycleCount = 0;
		m_nCylManualActionStep = 1000;
	}break;
	case 1000:
	{
		int nErr = do_CylCommand();
		if (nErr != ERR_NO_ERROR) {
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
		if (dSpanTime > ((double)m_nCmdParam_Time * 1000.0))
		{
			m_nCylManualActionStep++;
		}
	}break;
	case 1002:
	{
		int nErr = Check_CylSensor();
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			g_Error.AlarmReport(nErr, AlarmData, NULL);
			m_nCylManualActionStep = 9000;
			break;
		}
//		m_nCmdCycleCount++;
		m_nCylManualActionStep = 2000;
	}break;

	case 2000:
	{
		int nErr = do_CylCommand(TRUE);
		if (nErr != ERR_NO_ERROR) {
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
		if (dSpanTime > ((double)m_nCmdParam_Time * 1000.0))
		{
			m_nCylManualActionStep++;
		}
	}break;
	case 2002:
	{
		int nErr = Check_CylSensor(TRUE);
		if (nErr != ERR_NO_ERROR) {
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
void CDlgManuFeeder::CylManualOper_Time_Check_Action_Once_and_Sensor_Check()
{
	switch (m_nCylManualActionStep)
	{
	case 0:
	{
/*		m_nCylActionTimeRetry = 0;*/
		m_dCylActionTimeRecord = 0.0;
		DisplayCylinderActionTime(TRUE);
		m_nCylManualActionStep = 1000;
	}break;
	//실린더 동작
	case 1000:
	{
		int nErr = do_CylCommand();
		if (nErr != ERR_NO_ERROR) {
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
		if (dSpanTime > (10000.0))
		{
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_FEEDER_CYLINDER_TIMEOUT, AlarmData, NULL);
			m_nCylManualActionStep = 9000;
			break;
		}
		int nChk = Check_CylSensor();
		if (nChk == ERR_NO_ERROR)
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
		ClearCommand();
	}break;
	}
}

void CDlgManuFeeder::CylManualOper_C_Tray_Out_Action()
{
	BOOL bAlarmChk = FALSE;
	switch (m_nCylManualActionStep)
	{
	case 0: //Chk Tray Sensor
	{
		//순서 변경 하지 말것
		//1. Stacker Front에 C-Tray 있는지 확인.
		int nErr = g_TaskStacker[eSTACKER_03].ChkExistTray_FrontPos_Sen(eExist);
//		int nErr = g_TaskStacker[eSTACKER_03].ChkDI(CTaskStacker::xC_TRAY_CV_FRONT_EXIST_SEN, true);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep = 201;
			break;
		}

		//2. Stacker Rear에 C-Tray 있는지 확인.
		nErr = g_TaskStacker[eSTACKER_03].ChkExistTray_RearPos_Sen(eExist);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep = 200;
			break;
		}

		//3. Feeder에 C-Tray 있는지 확인.
		nErr = g_TaskTrayFeeder.GetTrayExistSensor(); //B접
		if (nErr == TRUE) {
			m_nCylManualActionStep = 100;
			break;
		}

		//4. Tray 아무것도 없으면 Out.
		m_nCylManualActionStep = 400;
	}break;
	case 100: //Feeder Move Y Load Site
	{
// 		if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
// 
// 		}
		int nErr = g_TaskTrayFeeder.do_MovePos(CTaskTrayFeeder::eAxisY, eLocIdxFeeder::LOC_IDX_FEEDER_TRAY_PICK_POS, 0, 50);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Feeder Move Fail.");
			g_Error.AlarmReport(nErr, AlarmData, NULL);
			bAlarmChk = TRUE;
			break;
		}
		else {
			m_nCylManualActionStep++;
			m_swTimeout.SetCheckTime(TIME_OUT_SHORT*2.0);
			m_swTimeout.StartTimer();
			break;
		}
	}break;
	case 101: //Feeder Y Move Done
	{
		OneAxis* pMotor = g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eAxisY);
		if (m_swTimeout.CheckOverTime() == TRUE) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			g_Error.AlarmReport(ERR_FEEDER_MOTOR_MOVE_TIMEOUT, AlarmData, NULL);
			bAlarmChk = TRUE;
			break;
		}

		int nErr = g_TaskTrayFeeder.ChkMotorPos(CTaskTrayFeeder::eAxisY, LOC_IDX_FEEDER_TRAY_PICK_POS);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep++;
		}
	}break;
	case 102: //Feeder Align Back
	{
		int nErr = g_TaskTrayFeeder.do_cyl_align_ForBack(eBack);
		Sleep(200);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep++;
			m_swTimeout.SetCheckTime(TIME_OUT_SHORT*2.0);
			m_swTimeout.StartTimer();
		}
	}break;
	case 103: //Feeder Align Back Check
	{
		if (m_swTimeout.CheckOverTime() == TRUE) {
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_FEEDER_ALIGN_CYLINDER_TIMEOUT, AlarmData, NULL);
			bAlarmChk = TRUE;
			break;
		}

		auto nErr = g_TaskTrayFeeder.chk_cyl_align_ForBack(eBack);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep++;
		}
	}break;
	case 104: //Feeder Cylinder Down
	{
		int nErr = g_TaskTrayFeeder.do_cyl_UpDown(eDown);
		Sleep(200);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep++;
			m_swTimeout.SetCheckTime(TIME_OUT_SHORT*2.0);
			m_swTimeout.StartTimer();
		}
	}break;
	case 105: //Feeder Cylinder Down Check
	{
		if (m_swTimeout.CheckOverTime() == TRUE) {
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_FEEDER_UP_DOWN_CYLINDER_TIMEOUT, AlarmData, NULL);
			bAlarmChk = TRUE;
			break;
		}

		auto nErr = g_TaskTrayFeeder.chk_cyl_UpDown(eDown);
		if (nErr == ERR_NO_ERROR){
			m_nCylManualActionStep++;
		}
	}break;
	case 106: //Feeder Unclamp
	{
		int nErr = g_TaskTrayFeeder.do_cyl_ClampUnclamp(eUnclamp);
		Sleep(200);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep++;
			m_swTimeout.SetCheckTime(TIME_OUT_SHORT*2.0);
			m_swTimeout.StartTimer();
		}
	}break;
	case 107: //Feeder Unclamp Check
	{
		if (m_swTimeout.CheckOverTime() == TRUE) {
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_FEEDER_CLAMP_CYLINDER_TIMEOUT, AlarmData, NULL);
			bAlarmChk = TRUE;
			break;
		}

		auto nchk_unclamp = g_TaskTrayFeeder.chk_cyl_ClampUnclamp(eUnclamp);
		int ntray_exist = g_TaskStacker[eSTACKER_03].ChkExistTray_RearPos_Sen(eExist);
		if (nchk_unclamp == ERR_NO_ERROR && ntray_exist == ERR_NO_ERROR) {
			m_nCylManualActionStep++;
			break;
		}
	}break;
	case 108: //Feeder Up
	{
		int nErr = g_TaskTrayFeeder.do_cyl_UpDown(eUp);
		Sleep(200);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep++;
			m_swTimeout.SetCheckTime(TIME_OUT_SHORT*2.0);
			m_swTimeout.StartTimer();
		}
	}break;
	case 109: //Feeder Up Check
	{
		if (m_swTimeout.CheckOverTime() == TRUE) {
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_FEEDER_UP_DOWN_CYLINDER_TIMEOUT, AlarmData, NULL);
			bAlarmChk = TRUE;
			break;
		}

		auto nErr = g_TaskTrayFeeder.chk_cyl_UpDown(eUp);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep = 200;
		}
	}break;
	case 200: //Stacker#3 Align Cylinder Back
	{
		g_TaskStacker[eSTACKER_03].do_cyl_TrayAlign(DEF_BACK);
		Sleep(200);
		int nErr = g_TaskStacker[eSTACKER_03].chk_cyl_TrayAlign(DEF_BACK);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep++;
		}
	}break;
	case 201: //Stacker#3 Conveyor Move
	{
		int nErr = g_TaskStacker[eSTACKER_03].ConveyorMoving(eConveyorMove_BKWD, true);
		if (nErr == ERR_NO_ERROR) {
			nErr = g_TaskStacker[eSTACKER_03].ChkExistTray_RearPos_Sen(eExist);
			if (nErr == ERR_NO_ERROR) {
				g_TaskStacker[eSTACKER_03].do_cyl_TrayAlign(DEF_FORW);
				Sleep(200);
			}
			m_nCylManualActionStep++;
			m_swTimeout.SetCheckTime(TIME_OUT_SHORT*2.0);
			m_swTimeout.StartTimer();
		}
		else
		{
			CParamList AlarmData;
			//AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), g_TaskStacker[eSTACKER_03].GetIndex() + 1);
			g_Error.AlarmReport(ERR_STACKER_3_INTERLOCK_CONVEYOR_DRIVER_NOT_WORK, AlarmData,NULL);
			break;
		}
	}break;
	case 202: //Stacker#3 Conveyor Front Slow Sensor Check
	{
		if (m_swTimeout.CheckOverTime() == TRUE) {
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_STACKER_3_REAR_SLOW_SENSOR_DETECT_TIMEOUT, AlarmData, NULL);
			bAlarmChk = TRUE;
			break;
		}

		int nErr = g_TaskStacker[eSTACKER_03].ChkDI(CTaskStacker::xC_TRAY_CV_FRONT_SLOW_SEN, true);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep++;
		}
	}break;
	case 203: //conveyor move slow
	{
		int nErr = g_TaskStacker[eSTACKER_03].ConveyorMoving(eConveyorMove_BKWD, false);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep++;
			m_swTimeout.SetCheckTime(TIME_OUT_SHORT*2.0);
			m_swTimeout.StartTimer();
		}
		else
		{
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_STACKER_3_INTERLOCK_CONVEYOR_DRIVER_NOT_WORK, AlarmData, NULL);
			break;
		}
	}break;
	case 204: //Stacker#3 Conveyor Front Sensor Check
	{
		if (m_swTimeout.CheckOverTime() == TRUE) {
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_STACKER_3_REAR_TRAY_EXIST_SENSOR_DETECT_TIMEOUT, AlarmData, NULL);
			bAlarmChk = TRUE;
			break;
		}

		int nErr = g_TaskStacker[eSTACKER_03].ChkDI(CTaskStacker::xC_TRAY_CV_FRONT_EXIST_SEN, true);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep++;
		}
	}break;
	case 205: //conveyor stop
	{
		g_TaskStacker[eSTACKER_03].ConveyorSlowStop();
		Sleep(200);
		m_nCylManualActionStep = 300;
	}break;
	case 300: //Main Pusher Up
	{
		int nErr = g_TaskStacker[eSTACKER_03].do_cyl_MainPusher(DEF_UP);
		Sleep(200);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep++;
			m_swTimeout.SetCheckTime(TIME_OUT_SHORT *2.0);
			m_swTimeout.StartTimer();
		}
	}
	case 301: //Chk Main Pusher Up
	{
		if (m_swTimeout.CheckOverTime() == TRUE) {
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_STACKER_3_MAIN_PUSHER_UP_TIMEOUT, AlarmData, NULL);
			bAlarmChk = TRUE;
			break;
		}

		int nErr = g_TaskStacker[eSTACKER_03].chk_cyl_MainPusher(DEF_UP);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep++;
		}
	}
	case 302: // Unclamp
	{
		int nErr = g_TaskStacker[eSTACKER_03].do_cyl_LockUnlock(DEF_UNCLAMP);
		Sleep(200);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep++;
			m_swTimeout.SetCheckTime(TIME_OUT_SHORT);
			m_swTimeout.StartTimer();
		}
	}break;
	case 303: // Chk Unclamp
	{
		if (m_swTimeout.CheckOverTime() == TRUE) {
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_STACKER_3_UNCLAMP_TIMEOUT, AlarmData, NULL);
			bAlarmChk = TRUE;
			break;
		}

		int nErr = g_TaskStacker[eSTACKER_03].chk_cyl_LockUnlock(DEF_UNCLAMP);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep++;
		}
	}break;
	case 304: // Sub Pusher Up
	{
		int nErr = g_TaskStacker[eSTACKER_03].do_cyl_SubPusher(DEF_UP);
		Sleep(200);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep++;
			m_swTimeout.SetCheckTime(TIME_OUT_MIDDLE / 3);
			m_swTimeout.StartTimer();
		}
	}break;
	case 305: // Sub Pusher Up Check
	{
		if (m_swTimeout.CheckOverTime() == TRUE) {
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_STACKER_3_SUB_PUSHER_UP_TIMEOUT, AlarmData, NULL);
			bAlarmChk = TRUE;
			break;
		}

		int nErr = g_TaskStacker[eSTACKER_03].chk_cyl_SubPusher(DEF_UP);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep++;
		}
	}break;
	case 306: // Clamp
	{
		int nErr = g_TaskStacker[eSTACKER_03].do_cyl_LockUnlock(DEF_CLAMP);
		Sleep(200);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep++;
			m_swTimeout.SetCheckTime(TIME_OUT_MIDDLE / 3);
			m_swTimeout.StartTimer();
		}
	}break;
	case 307: //Clamp Check
	{
		if (m_swTimeout.CheckOverTime() == TRUE) {
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_STACKER_3_CLAMP_TIMEOUT, AlarmData, NULL);
			bAlarmChk = TRUE;
			break;
		}

		int nErr = g_TaskStacker[eSTACKER_03].chk_cyl_LockUnlock(DEF_CLAMP);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep++;
		}
	}
	case 308: //Sub Pusher Down
	{
		int nErr = g_TaskStacker[eSTACKER_03].do_cyl_SubPusher(DEF_DOWN);
		Sleep(200);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep++;
			m_swTimeout.SetCheckTime(TIME_OUT_MIDDLE / 3);
			m_swTimeout.StartTimer();
		}
	}
	case 309: //Sub Pusher Down Check
	{
		if (m_swTimeout.CheckOverTime() == TRUE) {
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_STACKER_3_SUB_PUSHER_DN_TIMEOUT, AlarmData, NULL);
			bAlarmChk = TRUE;
			break;
		}

		int nErr = g_TaskStacker[eSTACKER_03].chk_cyl_SubPusher(DEF_DOWN);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep++;
		}
	}
	case 310: //Main Pusher Down
	{
		int nErr = g_TaskStacker[eSTACKER_03].do_cyl_MainPusher(DEF_DOWN);
		Sleep(200);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep++;
			m_swTimeout.SetCheckTime(TIME_OUT_MIDDLE / 3);
			m_swTimeout.StartTimer();
		}
	}break;
	case 311: //Main Pusher Down Check
	{
		if (m_swTimeout.CheckOverTime() == TRUE) {
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_STACKER_3_MAIN_PUSHER_DN_TIMEOUT, AlarmData, NULL);
			bAlarmChk = TRUE;
			break;
		}

		int nErr = g_TaskStacker[eSTACKER_03].chk_cyl_MainPusher(DEF_DOWN);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep = 400;		
		}
	}break;
	case 400: //End Step
	{
		Sleep(200);
		
		//1. Stacker#3 Rear에 C-Tray 있는지 확인.
		int nErr = g_TaskStacker[eSTACKER_03].ChkExistTray_RearPos_Sen(eExist);
		if (nErr == ERR_NO_ERROR) {
			m_nCylManualActionStep = 200;
			break;
		}

		//2. Feeder에 C-Tray 있는지 확인.
		nErr = g_TaskTrayFeeder.GetTrayExistSensor(); //B접
		if (nErr == TRUE) {
			m_nCylManualActionStep = 100;
			break;
		}

		m_nCylManualOpActState = eCylManualOper_None;
		m_nCylManualActionStep = 0;

		ButtonLockUnlock(TRUE);
	}
	default:
		break;
	}

	if (bAlarmChk == TRUE) {
		m_nCylManualOpActState = eCylManualOper_None;
		m_nCylManualActionStep = 0;

		ButtonLockUnlock(TRUE);
	}
}

void CDlgManuFeeder::ClearCommand()
{
	m_nCmdManualCyl = eCylManualCmd_None;
	m_nCmdParam_Time = -1;
}

int CDlgManuFeeder::do_CylCommand(BOOL bUndo /*= FALSE*/)
{
	int nErrCode = 0;
	switch (m_nCmdManualCyl)
	{
	case eCylManualCmd_EachUp:
	{
		eUpDownCmd eCmd = /*DEF_UP;//*/(bUndo == FALSE) ? eUp : eDown;
		nErrCode = g_TaskTrayFeeder.do_cyl_UpDown(eCmd);
	}break;
	case eCylManualCmd_EachDown:
	{
		eUpDownCmd eCmd = /*DEF_DOWN*/(bUndo == FALSE) ? eDown : eUp;
		nErrCode = g_TaskTrayFeeder.do_cyl_UpDown(eCmd);
	}break;
	case eCylManualCmd_EachClamp:
	{
		eClampCmd eCmd = /*DEF_CLAMP*/(bUndo == FALSE) ? eClamp : eUnclamp;
		nErrCode = g_TaskTrayFeeder.do_cyl_ClampUnclamp(eCmd);
	}break;
	case eCylManualCmd_EachUnclamp:
	{
		eClampCmd eCmd = /*DEF_UNCLAMP*/(bUndo == FALSE) ? eUnclamp : eClamp;
		nErrCode = g_TaskTrayFeeder.do_cyl_ClampUnclamp(eCmd);
	}break;
	case eCylManualCmd_EachFwd:
	{
		eForwBackCmd eCmd = /*DEF_LOCK*/(bUndo == FALSE) ? eForw : eBack;
		nErrCode = g_TaskTrayFeeder.do_cyl_align_ForBack(eCmd);
	}break;
	case eCylManualCmd_EachBak:
	{
		eForwBackCmd eCmd = /*DEF_UNLOCK*/(bUndo == FALSE) ? eBack : eForw;
		nErrCode = g_TaskTrayFeeder.do_cyl_align_ForBack(eCmd);
	}break;
	}
	return nErrCode;
}

int CDlgManuFeeder::Check_CylSensor(BOOL bUndo/* = FALSE*/)
{
	int nErrCode = 0;
	switch (m_nCmdManualCyl)
	{
	case eCylManualCmd_EachUp:
	{
		eUpDownCmd eCmd = /*DEF_UP;//*/(bUndo == FALSE) ? eUp : eDown;
		nErrCode = g_TaskTrayFeeder.chk_cyl_UpDown(eCmd);
	}break;
	case eCylManualCmd_EachDown:
	{
		eUpDownCmd eCmd = /*DEF_DOWN*/(bUndo == FALSE) ? eDown : eUp;
		nErrCode = g_TaskTrayFeeder.chk_cyl_UpDown(eCmd);
	}break;
	case eCylManualCmd_EachClamp:
	{
		eClampCmd eCmd = /*DEF_CLAMP*/(bUndo == FALSE) ? eClamp : eUnclamp;
		nErrCode = g_TaskTrayFeeder.chk_cyl_ClampUnclamp(eCmd);
	}break;
	case eCylManualCmd_EachUnclamp:
	{
		eClampCmd eCmd = /*DEF_UNCLAMP*/(bUndo == FALSE) ? eUnclamp : eClamp;
		nErrCode = g_TaskTrayFeeder.chk_cyl_ClampUnclamp(eCmd);
	}break;
	case eCylManualCmd_EachFwd:
	{
		eForwBackCmd eCmd = /*DEF_LOCK*/(bUndo == FALSE) ? eForw : eBack;
		nErrCode = g_TaskTrayFeeder.chk_cyl_align_ForBack(eCmd);
	}break;
	case eCylManualCmd_EachBak:
	{
		eForwBackCmd eCmd = /*DEF_UNLOCK*/(bUndo == FALSE) ? eBack : eForw;
		nErrCode = g_TaskTrayFeeder.chk_cyl_align_ForBack(eCmd);
	}break;
	}
	return nErrCode;
}

void CDlgManuFeeder::DisplayCylinderActionTime(BOOL bClear/*=FALSE*/)
{
	CString strTime;
	strTime.Format("%.3f", m_dCylActionTimeRecord);
	if (bClear) strTime = "";
	SetDlgItemText(UNITSETDLL_IDC_FEEDER_TIME, strTime);
}

void CDlgManuFeeder::OnBnClickedOk()
{
	CDialog::OnOK();
}


void CDlgManuFeeder::OnBnClickedBtnTrayOut()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nCylManualActionStep = 0;
	m_nCylManualOpActState = 100;

	ButtonLockUnlock(FALSE);
}

void CDlgManuFeeder::ButtonLockUnlock(BOOL bState)
{
	GetDlgItem(IDC_BTN_FEEDER_UP)->EnableWindow(bState);
	GetDlgItem(IDC_BTN_FEEDER_DOWN)->EnableWindow(bState);
	GetDlgItem(IDC_BTN_FEEDER_CLAMP)->EnableWindow(bState);
	GetDlgItem(IDC_BTN_FEEDER_UNCLAMP)->EnableWindow(bState);
	GetDlgItem(IDC_BTN_FEEDER_LOCK)->EnableWindow(bState);
	GetDlgItem(IDC_BTN_FEEDER_UNLOCK)->EnableWindow(bState);
}