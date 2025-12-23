// VAT_MainDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "VAT_MainDlg.h"
#include "afxdialogex.h"
#include "DlgManuMotor.h"

// CVAT_MainDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CVAT_MainDlg, CDialogEx)

CVAT_MainDlg::CVAT_MainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VAT_MAINDLG, pParent)
{
	m_pDlgLog = NULL;
	//m_bManualCmdExcute = FALSE;
	m_pDlgHomeCheckStatus = NULL;

	m_nSeqMode = -1;
	m_nSeqModeBack = -1;

	m_strCurStatus = _T("");

	m_bLastExecuteStatus = FALSE;
}

CVAT_MainDlg::~CVAT_MainDlg()
{
	if (m_pDlgLog != NULL)
	{
		m_pDlgLog->DestroyWindow();
		delete m_pDlgLog;
		m_pDlgLog = NULL;
	}

	//Multi Origin Status Dlg 삭제
	if (m_pDlgHomeCheckStatus != NULL)
	{
		m_pDlgHomeCheckStatus->DestroyWindow();
		delete m_pDlgHomeCheckStatus;
		m_pDlgHomeCheckStatus = NULL;
	}

	//Bitmap 삭제
	//DeleteBitmap();

	g_VatTaskSystemCtrl.Finalize();

	g_DMCont.m_dmVAT.SB(BDM14_VAT_TEACHING_USE, false);

	g_DMCont.m_dmVAT.SB(BDM14_VAT_UI_OPEN_HSITORY, FALSE);
}

void CVAT_MainDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	//Timer 삭제
	for (int i = 0; i < eTimer_Max; i++) {
		KillTimer(i);
	}	
}

void CVAT_MainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_SEQ_START, m_BtnStart);
	DDX_Control(pDX, IDC_BTN_SEQ_PAUSE, m_BtnStop);
	DDX_Control(pDX, IDC_BTN_SEQ_INITIAL, m_BtnInitial);
	DDX_Control(pDX, IDCANCEL, m_BtnExit);
	DDX_Control(pDX, IDC_VAT_MAIN_TAB, m_tabTeachMode);
	DDX_Control(pDX, IDC_SHADE_BTN_LOG, m_BtnLog);
	DDX_Control(pDX, IDC_SHADE_BTN_MULTIORIGIN, m_BtnMultiOrg);
	DDX_Control(pDX, IDC_SHADE_BTN_SERVO, m_BtnServoMotor);
	DDX_Control(pDX, IDC_STATIC_VAT_RUNSTOP, m_StaticRunStop);
	DDX_Control(pDX, IDC_BTN_VAT_MANU_VISION_SETTING, m_BtnVisionSet);

	//Manual Button
	DDX_Control(pDX, IDC_BTN_VAT_MANU_TEST_PP_1, m_BtnTestPP1);
	DDX_Control(pDX, IDC_BTN_VAT_MANU_TEST_PP_2, m_BtnTestPP2);
	DDX_Control(pDX, IDC_BTN_VAT_MANU_TABLE_1, m_BtnTable1);
	DDX_Control(pDX, IDC_BTN_VAT_MANU_TABLE_2, m_BtnTable2);
	DDX_Control(pDX, IDC_BTN_VAT_MANU_TABLE_3, m_BtnTable3);
	DDX_Control(pDX, IDC_BTN_VAT_MANU_TABLE_4, m_BtnTable4);
	DDX_Control(pDX, IDC_BTN_VAT_MANU_FEEDER, m_BtnFeeder);
	DDX_Control(pDX, IDC_BTN_VAT_MANU_TRAY_PP_1, m_BtnTrayPP1);
	DDX_Control(pDX, IDC_BTN_VAT_MANU_TRAY_PP_2, m_BtnTrayPP2);
	DDX_Control(pDX, IDC_BTN_VAT_MANU_TRANSFER, m_BtnTransfer);
	DDX_Control(pDX, IDC_BTN_VAT_MANU_STACKER3, m_BtnStacker3);
	DDX_Control(pDX, IDC_BTN_VAT_MANU_STACKER4, m_BtnStacker4);
	DDX_Control(pDX, IDC_BTN_VAT_MANU_STACKER5, m_BtnStacker5);
	DDX_Control(pDX, IDC_BTN_VAT_MANU_STACKER6, m_BtnStacker6);
	DDX_Control(pDX, IDC_BTN_VAT_MANU_STACKER7, m_BtnStacker7);

	DDX_Control(pDX, IDC_BUTTON_VAT_MANU_CMD_SKIP, m_BtnCmd_Skip);
	DDX_Control(pDX, IDC_BUTTON_VAT_MANU_CMD_STOP, m_BtnCmd_Stop);
}


BEGIN_MESSAGE_MAP(CVAT_MainDlg, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SHADE_BTN_LOG, &CVAT_MainDlg::OnBnClickedShadeBtnLog)
	ON_BN_CLICKED(IDC_SHADE_BTN_MULTIORIGIN, &CVAT_MainDlg::OnBnClickedShadeBtnMultiorigin)
	ON_BN_CLICKED(IDC_SHADE_BTN_SERVO, &CVAT_MainDlg::OnBnClickedShadeBtnServo)
	ON_BN_CLICKED(IDCANCEL, &CVAT_MainDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_SEQ_START, &CVAT_MainDlg::OnBnClickedBtnSeqStart)
	ON_BN_CLICKED(IDC_BTN_SEQ_PAUSE, &CVAT_MainDlg::OnBnClickedBtnSeqPause)
	ON_BN_CLICKED(IDC_BTN_SEQ_INITIAL, &CVAT_MainDlg::OnBnClickedBtnSeqInitial)
	ON_BN_CLICKED(IDC_BTN_VAT_MANU_TEST_PP_1, &CVAT_MainDlg::OnBnClickedBtnVatManuTestPp1)
	ON_BN_CLICKED(IDC_BTN_VAT_MANU_TEST_PP_2, &CVAT_MainDlg::OnBnClickedBtnVatManuTestPp2)
	ON_BN_CLICKED(IDC_BTN_VAT_MANU_TABLE_1, &CVAT_MainDlg::OnBnClickedBtnVatManuTable1)
	ON_BN_CLICKED(IDC_BTN_VAT_MANU_TABLE_2, &CVAT_MainDlg::OnBnClickedBtnVatManuTable2)
	ON_BN_CLICKED(IDC_BTN_VAT_MANU_TABLE_3, &CVAT_MainDlg::OnBnClickedBtnVatManuTable3)
	ON_BN_CLICKED(IDC_BTN_VAT_MANU_TABLE_4, &CVAT_MainDlg::OnBnClickedBtnVatManuTable4)
	ON_BN_CLICKED(IDC_BTN_VAT_MANU_TRAY_PP_1, &CVAT_MainDlg::OnBnClickedBtnVatManuTrayPp1)
	ON_BN_CLICKED(IDC_BTN_VAT_MANU_TRAY_PP_2, &CVAT_MainDlg::OnBnClickedBtnVatManuTrayPp2)
	ON_BN_CLICKED(IDC_BTN_VAT_MANU_FEEDER, &CVAT_MainDlg::OnBnClickedBtnVatManuFeeder)
	ON_BN_CLICKED(IDC_BTN_VAT_MANU_TRANSFER, &CVAT_MainDlg::OnBnClickedBtnVatManuTransfer)
	ON_BN_CLICKED(IDC_BUTTON_VAT_MANU_CMD_SKIP, &CVAT_MainDlg::OnBnClickedButtonVatManuCmdSkip)
	ON_BN_CLICKED(IDC_BUTTON_VAT_MANU_CMD_STOP, &CVAT_MainDlg::OnBnClickedButtonVatManuCmdStop)
	ON_BN_CLICKED(IDC_BTN_VAT_MANU_VISION_SETTING, &CVAT_MainDlg::OnBnClickedBtnVatManuVisionSetting)
	ON_WM_DESTROY()
	ON_WM_SIZE()

	ON_BN_CLICKED(IDC_BTN_VAT_MANU_STACKER3, &CVAT_MainDlg::OnBnClickedBtnVatManuStacker3)
	ON_BN_CLICKED(IDC_BTN_VAT_MANU_STACKER4, &CVAT_MainDlg::OnBnClickedBtnVatManuStacker4)
	ON_BN_CLICKED(IDC_BTN_VAT_MANU_STACKER5, &CVAT_MainDlg::OnBnClickedBtnVatManuStacker5)
	ON_BN_CLICKED(IDC_BTN_VAT_MANU_STACKER6, &CVAT_MainDlg::OnBnClickedBtnVatManuStacker6)
	ON_BN_CLICKED(IDC_BTN_VAT_MANU_STACKER7, &CVAT_MainDlg::OnBnClickedBtnVatManuStacker7)
END_MESSAGE_MAP()


// CVAT_MainDlg 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CVAT_MainDlg, CDialogEx)
	ON_EVENT(CVAT_MainDlg, IDC_SSCMD_Z_AUTO_FOCUS, 22, CVAT_MainDlg::ClickSscmdZAutoFocus, VTS_NONE)
	ON_EVENT(CVAT_MainDlg, IDC_SSCMD_CAM_CENTER_CAL, 22, CVAT_MainDlg::ClickSscmdCamCenterCal, VTS_NONE)
	ON_EVENT(CVAT_MainDlg, IDC_SSCMD_PICKER_ALIGN, 22, CVAT_MainDlg::ClickSscmdPickerAlign, VTS_NONE)
	ON_EVENT(CVAT_MainDlg, IDC_SSCMD_VAT, 22, CVAT_MainDlg::ClickSscmdVat, VTS_NONE)
	ON_EVENT(CVAT_MainDlg, IDC_SSCMD_Z_AUTO_TEACHING, 22, CVAT_MainDlg::ClickSscmdZAutoTeaching, VTS_NONE)
END_EVENTSINK_MAP()

BOOL CVAT_MainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	g_DMCont.m_dmVAT.SB(BDM14_VAT_UI_OPEN_HSITORY, TRUE);

	//VAT 측정 시, Motor 동작/Auto 동작/Sensor 확인 처리를 위함.
	g_DMCont.m_dmVAT.SB(BDM14_VAT_TEACHING_USE, true);

	//
	//HWND hOperDlg = this->GetSafeHwnd();
	g_VatTaskSystemCtrl.Initialize();
	
	m_vSSCmdSeqModeId.clear();
	m_vSSCmdSeqModeId.push_back(IDC_SSCMD_Z_AUTO_FOCUS); m_vSSCmdSeqModeId.push_back(IDC_SSCMD_CAM_CENTER_CAL);
	m_vSSCmdSeqModeId.push_back(IDC_SSCMD_PICKER_ALIGN); m_vSSCmdSeqModeId.push_back(IDC_SSCMD_Z_AUTO_TEACHING); m_vSSCmdSeqModeId.push_back(IDC_SSCMD_VAT);


	//Log Setting
	char *pszFilePath = nullptr;
	pszFilePath = "D:\\techwing\\VAT_LOG\\VAT_GUI_Controll.log";
	m_GuiLogger.SetFileName(pszFilePath);
	m_GuiLogger.LOG("[VAT Main Dlg Start]");

	//Display Log
	m_pDlgLog = new CVAT_DisplayLog;
	m_pDlgLog->Create(IDD_VAT_DISPLAY_LOG, this);

	//Tab 하단 Bar 제거
	m_tabTeachMode.ModifyStyle(TCS_BOTTOM, 0);

	//Tab Setting
	m_pgZAutoFocus.Create(CVATMain_ZAutoFocus::IDD, &m_tabTeachMode);
	m_pgCamCenterCal.Create(CVATMain_CamCenterCalibration::IDD, &m_tabTeachMode);
	m_pgPickerAlign.Create(CVATMain_PickerAlign::IDD, &m_tabTeachMode);
	m_pgZAutoTeaching.Create(CVATMain_ZAutoTeaching::IDD, &m_tabTeachMode);
	m_pgVisionAutoTeaching.Create(CVATMain_VisionAutoTeaching::IDD, &m_tabTeachMode);


	CRect rect;
	m_tabTeachMode.GetWindowRect(rect);
	m_pgZAutoFocus.MoveWindow(0, 0, rect.Width(), rect.Height());

	SetCurrentMode(VAT_SEQ_MODE_Z_FOCUS);
	
	InitMainDlgBtn(); //Main 화면 버튼 초기화.

	InitOperateBtn(); //Operate 버튼 초기화.

	InitManualBtn(); // Manual 버튼 초기화.

	SetTimer(eTimer_LotState, DEF_SENSOR_DISP_TM, NULL);
	SetTimer(eTimer_UtilityState, DEF_SENSOR_DISP_TM, NULL);
	//Simul
	InitSimul();

	return TRUE;
}

void CVAT_MainDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
		case eTimer_LotState:
		{
			//KillTimer(nIDEvent);

			_tDisplayVATStatus();
			_tDisplayBusyAlarmStatus();
			_tDisplayOperBtn();
			
			//SetTimer(eTimer_LotState, 500, NULL);
		}break;
		case eTimer_UtilityState:
		{
			_tDisplayVisionConnect();
		}break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CVAT_MainDlg::_tDisplayVATStatus()
{
	int nCurLotState = g_VatTaskAutoSystem.GetCurrentLotState();

	CString strStatus = _T("");
	switch (nCurLotState)
	{
	case eAutoLotState_InitNeed:
	case eAutoLotState_Initialize:
	case eAutoLotState_Idle:
	case eAutoLotState_Resume:
	case eAutoLotState_NormalComplete:
	{
		strStatus.Format("IDLE");
		m_StaticRunStop.SetBackColor(COLOR_YELLOW);
	}break;
	case eAutoLotState_Executing:
	{
		strStatus.Format("START");
		m_StaticRunStop.SetBackColor(COLOR_GREEN);
	}break;
	case eAutoLotState_Pause:
	case eAutoLotState_AlarmStop:
	{
		strStatus.Format("STOP");
		m_StaticRunStop.SetBackColor(COLOR_RED);
	}break;
	case eAutoLotState_CleanOut:
	{
		strStatus.Format("CLEAN OUT");
		m_StaticRunStop.SetBackColor(COLOR_RED);
	}break;
	default:
		break;
	}

	m_StaticRunStop.SetTextSize(30);
	SetDlgItemText(IDC_STATIC_VAT_RUNSTOP, strStatus);

}


void CVAT_MainDlg::_tDisplayBusyAlarmStatus()
{
	BOOL bCmdBusySts = FALSE;
	BOOL bCmdAlarmSts = FALSE;

	//Manual을 위한 것.
	CheckSeqCmdStatus_BusyAlarm(bCmdBusySts, bCmdAlarmSts);

	BOOL bAutoBusySts = FALSE;
	BOOL bAutoAlarmSts = FALSE;

	//Auto Check는 따로 해야 할 것.
	CheckSeqAutoStatus_BusyAlarm(bAutoBusySts, bAutoAlarmSts);


	CDSSPanel* pSensor = NULL;

	//Cmd Busy 상태 확인.
	pSensor = (CDSSPanel*)GetDlgItem(IDC_VAT_AXIS_CMD_BUSY);
	if (bCmdBusySts || bAutoBusySts) {
		pSensor->On();
		g_bVATStatusExecute = TRUE;
	}
	else {
		pSensor->Off();
		g_bVATStatusExecute = FALSE;
	}

	//Cmd Error 상태 확인.
	pSensor = (CDSSPanel*)GetDlgItem(IDC_VAT_AXIS_CMD_ERROR);
	if (bCmdAlarmSts || bAutoAlarmSts) {
		pSensor->On();
	}
	else {
		pSensor->Off();
	}
}

//OnTimer에서 사용.
void CVAT_MainDlg::CheckSeqCmdStatus_BusyAlarm(BOOL &bBusy, BOOL &bAlarm)
{
	int nBusyStsCnt = 0;
	int nAlarmStsCnt = FALSE;

	//Tray PP
	CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
	for (int nIdx = 0; nIdx < eMaxTrayPPCount; nIdx++) {
		if (pTaskTrayPP[nIdx]->GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			nBusyStsCnt++;
		}
		else if (pTaskTrayPP[nIdx]->GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
			nAlarmStsCnt++;
		}
	}

	//Loading Table
	for (int nIdx = 0; nIdx < eMaxLoadTblCount; nIdx++) {
		if (g_TaskLoadTbl[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			nBusyStsCnt++;
		}
		else if (g_TaskLoadTbl[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
			nAlarmStsCnt++;
		}
	}

	//Stacker
	for (int nIdx = 0; nIdx < eMaxStackerCount; nIdx++) {
		if (g_TaskStacker[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			nBusyStsCnt++;
		}
		else if (g_TaskStacker[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
			nAlarmStsCnt++;
		}
	}

	//Feeder
	if (g_TaskTrayFeeder.GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
		nBusyStsCnt++;
	}
	else if (g_TaskTrayFeeder.GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
		nAlarmStsCnt++;
	}

	//Transfer
	if (g_TaskTransfer.GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
		nBusyStsCnt++;
	}
	else if (g_TaskTransfer.GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
		nAlarmStsCnt++;
	}

	//Test PP
	for (int nIdx = 0; nIdx < eMaxTestPPCount; nIdx++) {
		if (g_TaskTestPP[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			nBusyStsCnt++;
		}
		else if (g_TaskTestPP[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
			nAlarmStsCnt++;
		}
	}

	//Press
	for (int nIdx = 0; nIdx < eMaxPressUnitCount; nIdx++) {
		if (g_TaskPressUnit[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			nBusyStsCnt++;
		}
		else if (g_TaskPressUnit[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
			nAlarmStsCnt++;
		}
	}

	//MultiOrg
	if (g_MulitOrg.GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
		nBusyStsCnt++;
	}
	else if (g_MulitOrg.GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
		nAlarmStsCnt++;
	}
	else {
		//MultiOrg 끝나면 Dlg 종료
		if (m_pDlgHomeCheckStatus != NULL)
		{
			m_pDlgHomeCheckStatus->DestroyWindow();
			delete m_pDlgHomeCheckStatus;
			m_pDlgHomeCheckStatus = NULL;
		}
	}


	//Check Busy Status & Alarm Status
	if (nBusyStsCnt > 0) {
		bBusy = TRUE;
	}
	if (nAlarmStsCnt > 0) {
		bAlarm = TRUE;
	}
}


void CVAT_MainDlg::CheckSeqAutoStatus_BusyAlarm(BOOL &bBusy, BOOL &bAlarm)
{
	int nCurLotState = g_VatTaskAutoSystem.GetCurrentLotState();

	switch (nCurLotState)
	{
		case eAutoLotState_Initialize:
		case eAutoLotState_Idle:
		case eAutoLotState_Resume:
		case eAutoLotState_Executing:
		case eAutoLotState_CleanOut:
		case eAutoLotState_NormalComplete:
		{
			bBusy = TRUE;
			bAlarm = FALSE;
		}break;
		case eAutoLotState_InitNeed:
		case eAutoLotState_Pause:
		{
			bBusy = FALSE;
			bAlarm = FALSE;
		}break;
		case eAutoLotState_AlarmStop:
		{		
			bBusy = FALSE;
			bAlarm = TRUE;
		}break;
		default: {
			bBusy = FALSE;
			bAlarm = FALSE;
		}break;
	}
}


void CVAT_MainDlg::_tDisplayOperBtn()
{
	int nCurLotState = g_VatTaskAutoSystem.GetCurrentLotState();
	
	switch (nCurLotState)
	{
	case eAutoLotState_InitNeed:
	case eAutoLotState_NormalComplete:
	{
		EnableOperationButton(TRUE, FALSE, FALSE, TRUE);
	}break;
	case eAutoLotState_Initialize:
	case eAutoLotState_Idle:
	case eAutoLotState_Executing:
	case eAutoLotState_Resume:
	{
		EnableOperationButton(FALSE, TRUE, FALSE, FALSE);
	}break;
	case eAutoLotState_Pause:
	case eAutoLotState_AlarmStop:
	{
// 		if (g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_END) == TRUE) {
// 			EnableOperationButton(FALSE, FALSE, TRUE, FALSE);
// 		}
// 		else {
// 			EnableOperationButton(TRUE, FALSE, TRUE, FALSE);
// 		}

		EnableOperationButton(TRUE, FALSE, TRUE, TRUE/*FALSE*/);
	}break;
	case eAutoLotState_CleanOut:
	{
		EnableOperationButton(FALSE, FALSE, FALSE, TRUE/*FALSE*/);
	}break;
	default:
		break;
	}
}


void CVAT_MainDlg::_tDisplayVisionConnect()
{
	int nID[VAT_VISION_PC_MAX] = { IDC_SSPANEL_VISION_PC1_STATE , IDC_SSPANEL_VISION_PC2_STATE };
	CVisionComm* pVision[VAT_VISION_PC_MAX] = { g_pVisionTestHand_1 , g_pVisionTestHand_2 };

	for(int i = 0; i < VAT_VISION_PC_MAX; i++) {
		CDSSPanel *pVisionPanel = (CDSSPanel *)GetDlgItem(nID[i]);
		if (pVision[i]->IsConnected()) {
			pVisionPanel->SetWindowText(_T("Connected"));
			pVisionPanel->SetBackColor(LLIGHTGREEN);
		}
		else {
			pVisionPanel->SetWindowText(_T("Disconnect"));
			pVisionPanel->SetBackColor(LLIGHTRED);
		}
	}
}


void CVAT_MainDlg::OnBnClickedBtnSeqStart()
{
	m_GuiLogger.LOG("[VAT Main Dlg] [Click] [Start Button]");
	
	//cover, door, origin, vision connect, selected check
	CString strErrAxis = _T("");
	int nErr = CheckVatConditionForStart(strErrAxis);
	if (nErr != ERR_NO_ERROR)
	{
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("%s", strErrAxis);
		g_Error.AlarmReport(nErr, AlarmData, NULL);
		return;	
	}


	int nCurLotState = eAutoLotState_Unknown;
	std::vector<int> vLotState;

	nCurLotState = g_VatTaskAutoSystem.GetCurrentLotState();
	vLotState.push_back(nCurLotState);

	nCurLotState = g_VatTaskAutoTrayLoadPP.GetCurrentLotState();
	vLotState.push_back(nCurLotState);

	nCurLotState = g_VatTaskAutoTrayUnloadPP.GetCurrentLotState();
	vLotState.push_back(nCurLotState);

	nCurLotState = g_VatTaskAutoLoadTestPP1.GetCurrentLotState();
	vLotState.push_back(nCurLotState);

	nCurLotState = g_VatTaskAutoUnloadTestPP1.GetCurrentLotState();
	vLotState.push_back(nCurLotState);

	nCurLotState = g_VatTaskAutoLoadTestPP2.GetCurrentLotState();
	vLotState.push_back(nCurLotState);

	nCurLotState = g_VatTaskAutoUnloadTestPP2.GetCurrentLotState();
	vLotState.push_back(nCurLotState);

	for (std::vector<int>::size_type i = 0; i < vLotState.size(); i++)
	{
		if (vLotState[i] == eAutoLotState_InitNeed)
		{
			nCurLotState = eAutoLotState_InitNeed;
			break;
		}
		if (vLotState[i] == eAutoLotState_AlarmStop || vLotState[i] == eAutoLotState_Pause)
		{
			nCurLotState = eAutoLotState_Pause;
			break;
		}
	}

	//Lot State Unknown 체크
	for (std::vector<int>::size_type i = 0; i < vLotState.size(); i++) {
		if (eAutoLotState_Unknown == vLotState[i]) {
			nCurLotState = eAutoLotState_Unknown;
			break;
		}
	}

	
	eAutoLotCmd _eAutoLotCmd = eAutoLotCmd_None;
	switch (nCurLotState)
	{
		case eAutoLotState_Pause:
		{
			_eAutoLotCmd = eAutoLotCmd_Resume;
		}break;
		case eAutoLotState_InitNeed:
		{
			//측정 확인 Alarm Ignore.
			BOOL bIgnore = CheckVatAlarmIgnoreForStart();
			if (bIgnore != TRUE) {
				return;
			}

			//현재 Sequence Mode Setting
			g_VatTaskSystemCtrl.SetSeqCurMode(m_nSeqMode);

			//GUI Notify Init 
			g_VatTaskSystemCtrl.ClearMainGuiMeasureStatus();

			//초기 측정 시, Parameter Setting.
			InitModeParamForStart();

			//Main Tab Page로 변경.
			ChangeMeasurePage();

			//Log List Box Clear
			if (m_pDlgLog != NULL) {
				m_pDlgLog->ClearListBox();
			}

			//Simul
			InitSimul();

			_eAutoLotCmd = eAutoLotCmd_Initial;
		}break;
		default:
			break;
	}

	if (_eAutoLotCmd != eAutoLotCmd_None)
		g_VatTaskSystemCtrl.SetLotCommand(_eAutoLotCmd);
}

void CVAT_MainDlg::OnBnClickedBtnSeqPause()
{
	m_GuiLogger.LOG("[VAT Main Dlg] [Click] [Stop Button]");

	g_VatTaskAutoSystem.ChangeMainState(eAutoLotState_Pause);

	//NormalComplete 제외 하고 Auto State Pause
// 	for (auto _auto_seq : g_VatTaskAutoSystem.m_vVatAllAutoTasks) {
// 		if (_auto_seq->GetCurrentLotState() != eAutoLotState_NormalComplete) {
// 			_auto_seq->ChangeMainState(eAutoLotState_Pause);
// 		}
// 	}


// 	int nErr = g_VatTaskSystemCtrl.SetLotCommand(eAutoLotCmd_Pause);
// 	if (nErr != ERR_NO_ERROR) {
// 		CParamList AlarmData;
// 		g_Error.AlarmReport(nErr, AlarmData, NULL);
// 	}
}

void CVAT_MainDlg::OnBnClickedBtnSeqInitial()
{
	m_GuiLogger.LOG("[VAT Main Dlg] [Click] [Initial Button]");

	g_VatTaskAutoSystem.m_nDataSaveCase = eVat_Abort;

	g_VatTaskAutoSystem.ChangeMainState(eAutoLotState_CleanOut);


// 	for (auto _auto_seq : g_VatTaskAutoSystem.m_vVatAllAutoTasks) {
// 		if (_auto_seq->GetCurrentLotState() != eAutoLotState_NormalComplete) {
// 			_auto_seq->ChangeMainState(eAutoLotState_CleanOut);
// 		}
// 	}

// 	int nErr = g_VatTaskSystemCtrl.SetLotCommand(eAutoLotCmd_CleanOut);
// 	if (nErr != ERR_NO_ERROR) {
// 		CParamList AlarmData;
// 		g_Error.AlarmReport(nErr, AlarmData, NULL);
// 	}


// 	if (g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_END) == TRUE) {
// 		nErr = g_VatTaskSystemCtrl.SetLotCommand(eAutoLotCmd_Resume);
// 	}
// 	else {
// 		nErr = g_VatTaskSystemCtrl.SetLotCommand(eAutoLotCmd_CleanOut);
// 	}


}

void CVAT_MainDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void CVAT_MainDlg::InitMainDlgBtn()
{
	CFont BtnFont;
	BtnFont.CreatePointFont(120, "Fixedsys");

	//Log Btn
	m_BtnLog.SetFont(&BtnFont);
	m_BtnLog.SetWindowText(_T("Teaching Log"));
	m_BtnLog.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255)); // 8
	m_BtnLog.SetIcon(IDI_MAIN_EVENTLOG);
	m_BtnLog.SetAlign(CButtonST::ST_ALIGN_HORIZ); //ST_ALIGN_HORIZ

	//Multi Origin
	m_BtnMultiOrg.SetFont(&BtnFont);
	m_BtnMultiOrg.SetWindowText(_T("Multi Origin"));
	m_BtnMultiOrg.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255)); //SHS_VBUMP
	m_BtnMultiOrg.SetIcon(IDI_BTN_MOTOR);
	m_BtnMultiOrg.SetAlign(CButtonST::ST_ALIGN_HORIZ);

	//Servo Motor
	m_BtnServoMotor.SetFont(&BtnFont);
	m_BtnServoMotor.SetWindowText(_T("Servo Motor"));
	m_BtnServoMotor.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnServoMotor.SetIcon(IDI_BTN_MOTOR_MONITOR);
	m_BtnServoMotor.SetAlign(CButtonST::ST_ALIGN_HORIZ);

	//Vision Setting
	
	m_BtnVisionSet.SetFont(&BtnFont);
	m_BtnVisionSet.SetWindowText(_T("Vision Setting"));
	m_BtnVisionSet.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255)); //SHS_VBUMP
	//m_BtnVisionSet.SetIcon();
	m_BtnVisionSet.SetAlign(CButtonST::ST_ALIGN_HORIZ);
}

void CVAT_MainDlg::InitOperateBtn()
{
	//Operate Button 초기화.
	CFont OperBtnFont;
	OperBtnFont.CreatePointFont(150, "Arial");

	m_BtnStart.SetFont(&OperBtnFont);
	m_BtnStart.SetWindowText(_T("START"));
	m_BtnStart.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));

	m_BtnStop.SetFont(&OperBtnFont);
	m_BtnStop.SetWindowText(_T("STOP"));
	m_BtnStop.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));

	m_BtnInitial.SetFont(&OperBtnFont);
	m_BtnInitial.SetWindowText(_T("INITIAL"));
	m_BtnInitial.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));

	m_BtnExit.SetFont(&OperBtnFont);
	m_BtnExit.SetWindowText(_T("EXIT"));
	m_BtnExit.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));

	OperBtnFont.Detach();

}

void CVAT_MainDlg::InitManualBtn()
{
	m_cBmpTestPP1.LoadBitmap(IDB_VAT_MANU_TESTPP_1/*IDB_TESTPP_01*/);
	m_BtnTestPP1.SetBitmap(m_cBmpTestPP1);

	m_cBmpTestPP2.LoadBitmap(IDB_VAT_MANU_TESTPP_2/*IDB_TESTPP_02*/);
	m_BtnTestPP2.SetBitmap(m_cBmpTestPP2);

	m_cBmpTable1.LoadBitmap(IDB_VAT_MANU_UD_TABLE_1);
	m_BtnTable1.SetBitmap(m_cBmpTable1);

	m_cBmpTable2.LoadBitmap(IDB_VAT_MANU_LD_TABLE_1);
	m_BtnTable2.SetBitmap(m_cBmpTable2);

	m_cBmpTable3.LoadBitmap(IDB_VAT_MANU_LD_TABLE_2);
	m_BtnTable3.SetBitmap(m_cBmpTable3);

	m_cBmpTable4.LoadBitmap(IDB_VAT_MANU_UD_TABLE_2);
	m_BtnTable4.SetBitmap(m_cBmpTable4);

	m_cBmpFeeder.LoadBitmap(IDB_VAT_MANU_FEEDER);
	m_BtnFeeder.SetBitmap(m_cBmpFeeder);

	m_cBmpTrayPP1.LoadBitmap(IDB_VAT_MANU_LD_TRAY_PP);
	m_BtnTrayPP1.SetBitmap(m_cBmpTrayPP1);

	m_cBmpTrayPP2.LoadBitmap(IDB_VAT_MANU_UD_TRAY_PP);
	m_BtnTrayPP2.SetBitmap(m_cBmpTrayPP2);

	m_cBmpTransfer.LoadBitmap(IDB_VAT_MANU_TRANSFER);
	m_BtnTransfer.SetBitmap(m_cBmpTransfer);

	//Stacker
	m_BtnStacker3.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnStacker3.SetIcon(IDI_ICON_STACKER);
	m_BtnStacker3.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnStacker4.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnStacker4.SetIcon(IDI_ICON_STACKER);
	m_BtnStacker4.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnStacker5.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnStacker5.SetIcon(IDI_ICON_STACKER);
	m_BtnStacker5.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnStacker6.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnStacker6.SetIcon(IDI_ICON_STACKER);
	m_BtnStacker6.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnStacker7.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnStacker7.SetIcon(IDI_ICON_STACKER);
	m_BtnStacker7.SetAlign(CButtonST::ST_ALIGN_VERT);

	// stop
 	m_cBmpCmd_Skip.LoadBitmap(IDB_VAT_MANU_SKIP);	
 	m_BtnCmd_Skip.SetBitmap(m_cBmpCmd_Skip);
 	
 	// skip
	//m_cBmpCmd_Stop.SetBitmapDimension(100, 10);
 	m_cBmpCmd_Stop.LoadBitmap(IDB_VAT_MANU_STOP);	
 	m_BtnCmd_Stop.SetBitmap(m_cBmpCmd_Stop);
	
}

// void CVAT_MainDlg::DeleteBitmap()
// {
// 	m_cBmpTestPP1.DeleteObject();
// 	m_cBmpTestPP2.DeleteObject();
// 	m_cBmpTable1.DeleteObject();
// 	m_cBmpTable2.DeleteObject();
// 	m_cBmpTable3.DeleteObject();
// 	m_cBmpTable4.DeleteObject();
// 	m_cBmpFeeder.DeleteObject();
// 	m_cBmpTrayPP1.DeleteObject();
// 	m_cBmpTrayPP2.DeleteObject();
// 	m_cBmpTransfer.DeleteObject();
// 
// 	m_cBmpCmd_Skip.DeleteObject();
// 	m_cBmpCmd_Stop.DeleteObject();
// }

void CVAT_MainDlg::ClickSscmdZAutoFocus()
{
	//return; //개발 완료 후, 해제

	if (g_VatTaskAutoSystem.GetCurrentLotState() != eAutoLotState_InitNeed)
		return;
	

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_GuiLogger.LOG("[VAT Main][Start Mode][Z Auto Focus Click]");
	SetCurrentMode(VAT_SEQ_MODE_Z_FOCUS);
}


void CVAT_MainDlg::ClickSscmdCamCenterCal()
{
	//return; //개발 완료 후, 해제

	if (g_VatTaskAutoSystem.GetCurrentLotState() != eAutoLotState_InitNeed)
		return;

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_GuiLogger.LOG("[VAT Main][Start Mode][Cam Center Calibration Click]");
	SetCurrentMode(VAT_SEQ_MODE_CAM_CAL);
}


void CVAT_MainDlg::ClickSscmdPickerAlign()
{
	//return; //개발 완료 후, 해제

	if (g_VatTaskAutoSystem.GetCurrentLotState() != eAutoLotState_InitNeed)
		return;

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_GuiLogger.LOG("[VAT Main][Start Mode][Picker Align Click]");
	SetCurrentMode(VAT_SEQ_MODE_PICKER_ALIGN);
}


void CVAT_MainDlg::ClickSscmdVat()
{
	//return; //개발 완료 후, 해제

	if (g_VatTaskAutoSystem.GetCurrentLotState() != eAutoLotState_InitNeed)
		return;

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_GuiLogger.LOG("[VAT Main][Start Mode][Vision Auto Teaching Click]");
	SetCurrentMode(VAT_SEQ_MODE_VAT);
}


void CVAT_MainDlg::ClickSscmdZAutoTeaching()
{
	if (g_VatTaskAutoSystem.GetCurrentLotState() != eAutoLotState_InitNeed)
		return;

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_GuiLogger.LOG("[VAT Main][Start Mode][Z Auto Teaching Click]");
	SetCurrentMode(VAT_SEQ_MODE_Z_TEACHING);
}


void CVAT_MainDlg::EnableOperationButton(BOOL bStartBtn, BOOL bPauseBtn, BOOL bStopBtn, BOOL bExitBtn)
{
	GetDlgItem(IDC_BTN_SEQ_START)->EnableWindow(bStartBtn);
	GetDlgItem(IDC_BTN_SEQ_PAUSE)->EnableWindow(bPauseBtn);
	GetDlgItem(IDC_BTN_SEQ_INITIAL)->EnableWindow(bStopBtn);
	GetDlgItem(IDCANCEL)->EnableWindow(bExitBtn);
	GetDlgItem(IDC_BTN_VAT_MANU_VISION_SETTING)->EnableWindow(bExitBtn);
	
}

void CVAT_MainDlg::SetCurrentMode(int nMode)
{
	//이전 Mode 저장.
	m_nSeqModeBack = m_nSeqMode;

	m_nSeqMode = nMode;

	((CSSCommand *)GetDlgItem(IDC_SSCMD_Z_AUTO_FOCUS))->SetBackColor(WWHITE);
	((CSSCommand *)GetDlgItem(IDC_SSCMD_CAM_CENTER_CAL))->SetBackColor(WWHITE);
	((CSSCommand *)GetDlgItem(IDC_SSCMD_PICKER_ALIGN))->SetBackColor(WWHITE);
	((CSSCommand *)GetDlgItem(IDC_SSCMD_Z_AUTO_TEACHING))->SetBackColor(WWHITE);
	((CSSCommand *)GetDlgItem(IDC_SSCMD_VAT))->SetBackColor(WWHITE);

 	switch (nMode)
 	{
 		case VAT_SEQ_MODE_Z_FOCUS:
 		{
 			((CSSCommand *)GetDlgItem(IDC_SSCMD_Z_AUTO_FOCUS))->SetBackColor(LLIGHTGREEN);
 		}break;
 		case VAT_SEQ_MODE_CAM_CAL:
 		{
			((CSSCommand *)GetDlgItem(IDC_SSCMD_CAM_CENTER_CAL))->SetBackColor(LLIGHTGREEN);
 		}break;
		case VAT_SEQ_MODE_PICKER_ALIGN:
		{
			((CSSCommand *)GetDlgItem(IDC_SSCMD_PICKER_ALIGN))->SetBackColor(LLIGHTGREEN);
		}break;
		case VAT_SEQ_MODE_VAT:
		{
			((CSSCommand *)GetDlgItem(IDC_SSCMD_VAT))->SetBackColor(LLIGHTGREEN);
		}break;
		case VAT_SEQ_MODE_Z_TEACHING:
		{
			((CSSCommand *)GetDlgItem(IDC_SSCMD_Z_AUTO_TEACHING))->SetBackColor(LLIGHTGREEN);
		}break;
 	default:
		break;
 	}

	//이전 모드 불필요 파라미터 정리.
	PageMove_BeforeMode_Dispose();

	//해당 모드 초기화 필요한 파라미터 세팅.
	PageMove_CurMode_Init();

	//Page Change
	SelChangeMainPage();

	//Main Page에 대해 Handle을 넘김.
	SetMainPageHandle();
}


int CVAT_MainDlg::GetCurrentModeType()
{
	int nRetType = VAT_MODE_TYPE_NONE;

	if (m_nSeqMode == VAT_SEQ_MODE_Z_FOCUS || m_nSeqMode == VAT_SEQ_MODE_CAM_CAL) {
		nRetType = VAT_MODE_TYPE_HAND;
	}
	else if (m_nSeqMode == VAT_SEQ_MODE_PICKER_ALIGN) {
		nRetType = VAT_MODE_TYPE_PICKER;
	}
	else if(m_nSeqMode == VAT_SEQ_MODE_Z_TEACHING || m_nSeqMode == VAT_SEQ_MODE_VAT) {
		nRetType = VAT_MODE_TYPE_PART;
	}

	return nRetType;
}


void CVAT_MainDlg::PageMove_CurMode_Init()
{
 	switch (m_nSeqMode)
 	{
	case VAT_SEQ_MODE_Z_FOCUS:		{ m_pgZAutoFocus.PageMoveInit_Main(); }			break;
	case VAT_SEQ_MODE_CAM_CAL:		{ m_pgCamCenterCal.PageMoveInit_Main(); }		break;
	case VAT_SEQ_MODE_PICKER_ALIGN: { m_pgPickerAlign.PageMoveInit_Main(); }		break;
	case VAT_SEQ_MODE_Z_TEACHING:	{ m_pgZAutoTeaching.PageMoveInit_Main(); }		break;
	case VAT_SEQ_MODE_VAT:			{ m_pgVisionAutoTeaching.PageMoveInit_Main(); } break;
 	default:
		break;
 	}
}

void CVAT_MainDlg::PageMove_BeforeMode_Dispose()
{
	switch (m_nSeqModeBack)
	{
	case VAT_SEQ_MODE_Z_FOCUS:		{ m_pgZAutoFocus.PageMoveDispose(); }			break;
	case VAT_SEQ_MODE_CAM_CAL:		{ m_pgCamCenterCal.PageMoveDispose(); }			break;
	case VAT_SEQ_MODE_PICKER_ALIGN: { m_pgPickerAlign.PageMoveDispose_Main(); }			break;
	case VAT_SEQ_MODE_Z_TEACHING:	{ m_pgZAutoTeaching.PageMoveDispose_Main(); }		break;
	case VAT_SEQ_MODE_VAT:			{ m_pgVisionAutoTeaching.PageMoveDispose_Main(); } break;
	default:
		break;
	}
}


void CVAT_MainDlg::SelChangeMainPage()
{
	switch (m_nSeqMode)
	{
	case VAT_SEQ_MODE_Z_FOCUS:
	{
		m_pgZAutoFocus.ShowWindow(SW_SHOW);
		//m_pgZAutoFocus.OnSizeClipboard()
		m_pgCamCenterCal.ShowWindow(SW_HIDE);
		m_pgPickerAlign.ShowWindow(SW_HIDE);
		m_pgZAutoTeaching.ShowWindow(SW_HIDE);
		m_pgVisionAutoTeaching.ShowWindow(SW_HIDE);
	}break;
	case VAT_SEQ_MODE_CAM_CAL:
	{
		m_pgZAutoFocus.ShowWindow(SW_HIDE);
		m_pgCamCenterCal.ShowWindow(SW_SHOW);
		m_pgPickerAlign.ShowWindow(SW_HIDE);
		m_pgZAutoTeaching.ShowWindow(SW_HIDE);
		m_pgVisionAutoTeaching.ShowWindow(SW_HIDE);
	}break;
	case VAT_SEQ_MODE_PICKER_ALIGN:
	{
		m_pgZAutoFocus.ShowWindow(SW_HIDE);
		m_pgCamCenterCal.ShowWindow(SW_HIDE);
		m_pgPickerAlign.ShowWindow(SW_SHOW);
		m_pgZAutoTeaching.ShowWindow(SW_HIDE);
		m_pgVisionAutoTeaching.ShowWindow(SW_HIDE);
	}break;
	case VAT_SEQ_MODE_VAT:
	{
		m_pgZAutoFocus.ShowWindow(SW_HIDE);
		m_pgCamCenterCal.ShowWindow(SW_HIDE);
		m_pgPickerAlign.ShowWindow(SW_HIDE);
		m_pgZAutoTeaching.ShowWindow(SW_HIDE);
		m_pgVisionAutoTeaching.ShowWindow(SW_SHOW);
	}break;
	case VAT_SEQ_MODE_Z_TEACHING:
	{
		m_pgZAutoFocus.ShowWindow(SW_HIDE);
		m_pgCamCenterCal.ShowWindow(SW_HIDE);
		m_pgPickerAlign.ShowWindow(SW_HIDE);
		m_pgZAutoTeaching.ShowWindow(SW_SHOW);
		m_pgVisionAutoTeaching.ShowWindow(SW_HIDE);
	}break;
	default:
		break;
	}
}


void CVAT_MainDlg::SetMainPageHandle()
{
	HWND m_hMain = NULL;

	switch (m_nSeqMode)
	{
	case VAT_SEQ_MODE_Z_FOCUS:			
	{
		m_hMain = m_pgZAutoFocus.GetSafeHwnd();
	}break;
	case VAT_SEQ_MODE_CAM_CAL:			
	{
		m_hMain = m_pgCamCenterCal.GetSafeHwnd();
	}break;
	case VAT_SEQ_MODE_PICKER_ALIGN:		
	{
		BOOL bMeasureTabVisible = m_pgPickerAlign.m_pgMeasure.IsWindowVisible();
		BOOL bSettingTabVisible = m_pgPickerAlign.m_pgSetting.IsWindowVisible();

		if (bMeasureTabVisible) {
			m_hMain = m_pgPickerAlign.m_pgMeasure.GetSafeHwnd();
		}
		else if (bSettingTabVisible) {
			m_hMain = m_pgPickerAlign.m_pgSetting.GetSafeHwnd();
		}
		else {
			m_hMain = m_pgPickerAlign.m_pgMeasure.GetSafeHwnd();
		}

		//m_hMain = m_pgPickerAlign.m_pgMeasure.GetSafeHwnd();
	}break;
	case VAT_SEQ_MODE_VAT:				m_hMain = m_pgVisionAutoTeaching.m_pgMeasure.GetSafeHwnd();		break;
	case VAT_SEQ_MODE_Z_TEACHING:		m_hMain = m_pgZAutoTeaching.m_pgMeasure.GetSafeHwnd();	break;
	default:
		break;
	}

	// 여기에 SingleTone Class내의 Main Page Hand 저장. [9/4/2025 dohyeong.kim]
	CVAT_MeasureStatus::GetInstance().SetVatMainPageHandle(m_hMain);

	// 기존 Z Auto Teaching 사용할 때 임. [9/4/2025 dohyeong.kim]
	g_VatTaskSystemCtrl.SetVatMainPageHandle(m_hMain);
}


void CVAT_MainDlg::OnBnClickedShadeBtnLog()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_GuiLogger.LOG("[VAT Main Dlg] [Click] [Teaching Log]");

	m_pDlgLog->ShowWindow(SW_SHOW);
}

void CVAT_MainDlg::OnBnClickedShadeBtnMultiorigin()
{
	if (g_bVATStatusExecute) {
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	int nErr = g_TaskSystemCtrl.CheckEqpConditionForMultiOrg();
	if (nErr != ERR_NO_ERROR) {
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, &g_TaskSystemCtrl);
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable();
	if (nInterlockErr != ERR_NO_ERROR) {
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	int nRet = AfxMessageBox("Do you want Multi Origin Axis All Start?????", MB_YESNO);
	if (nRet == IDYES) {
		//CDlgHomeCheckStatus
		m_pDlgHomeCheckStatus = new CDlgHomeCheckStatus(this);
		m_pDlgHomeCheckStatus->Create(IDD_DIALG_HOME_CHECK_STATUS, this);
		m_pDlgHomeCheckStatus->ShowWindow(SW_SHOW);

		//g_DMCont.m_dmHandlerOpt.SN(NDM11_MULTIORIGIN_HISTORY, eMulti_Origin_Start);
		CMultiOrgCmd_Axis* pSeqCmd = new CMultiOrgCmd_Axis;
		pSeqCmd->m_nCmdStage = eMultiOrgType_AxisAll;
		g_TaskSystemCtrl.SendCommand(g_MulitOrg, pSeqCmd, eEqpCtrlMode_Manual);

// 		if (m_pDlgHomeCheckStatus->IsWindowVisible() == FALSE) {
// 			m_pDlgHomeCheckStatus->ShowWindow(SW_SHOW);
// 		}

		m_GuiLogger.LOG("Multi Origin(Axis All) Start");
	}

	double dForcePerPin = g_DMCont.m_dmHandlerOpt.GD(DDM11_DvcContactForce_GF);
	if (dForcePerPin <= 0.0) {
		dForcePerPin = 0.0;
	}
	for (int i = 0; i < eMaxPressUnitCount; i++)
	{
		g_TaskRegulator[i].do_SetForcePerPin(dForcePerPin);
		Sleep(10);
	}
}


void CVAT_MainDlg::OnBnClickedShadeBtnServo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (g_bVATStatusExecute) {
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	CDlgManuMotor dlg;
	dlg.DoModal();
}

void CVAT_MainDlg::OnBnClickedBtnVatManuTestPp1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBnClickedAxisManuTestPp(eTestPP_1);
}


void CVAT_MainDlg::OnBnClickedBtnVatManuTestPp2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBnClickedAxisManuTestPp(eTestPP_2);
}

void CVAT_MainDlg::OnBnClickedAxisManuTestPp(int nTestPpIdx)
{
	if (g_bVATStatusExecute) {
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
 	}

	if (!g_COVER_SW.GetStatus()) {
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK, AlarmData, NULL);
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable();
	if (nInterlockErr != ERR_NO_ERROR) {
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	m_GuiLogger.LOG("Test PP #%d Manu Dialog Show", nTestPpIdx + 1);
	CDlgCommand_TestPP CmdDlg;
	if (IDOK == CmdDlg.DoModal())
	{
		int nSelectedCmd = CmdDlg.GetSelectedSeqCmdId();
		switch (nSelectedCmd)
		{
			case eSeqCmd_TestPP_Initial:
			{
				int nRetCmd = ERR_NO_ERROR;
				nRetCmd |= g_TaskPressUnit[ePressUnit_Down_1 + (eMaxATS_Area * nTestPpIdx)].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON);
				nRetCmd |= g_TaskPressUnit[ePressUnit_Down_2 + (eMaxATS_Area * nTestPpIdx)].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON);
				nRetCmd |= g_TaskPressUnit[ePressUnit_Down_3 + (eMaxATS_Area * nTestPpIdx)].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON);
				nRetCmd |= g_TaskPressUnit[ePressUnit_Down_4 + (eMaxATS_Area * nTestPpIdx)].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON);

				if (nRetCmd != ERR_NO_ERROR) {
					AfxMessageBox("Press Unit is not safety pos!!!");
					return;
				}

				CTestPPCmd_Initial* pSeqCmd = new CTestPPCmd_Initial;
				int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTestPP[nTestPpIdx], pSeqCmd, eEqpCtrlMode_Manual);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					g_Error.AlarmReport(nErr, AlarmData, NULL);
				}
				else {
					m_GuiLogger.LOG("Test PP #%d Initial", nTestPpIdx + 1);
				}
			}break;
			case eSeqCmd_TestPP_MovePos:
			{
				CDlgAxisActionMove dlg;
				dlg.m_nStartPoint = eMANU_OPER_TEST_PP1 + nTestPpIdx;
				dlg.m_nSelectAction = eDlgActionOnlyMove;
				dlg.DoModal();
			}break;
			case eSeqCmd_TestPP_MovePickPlace:
			{
				CDlgAxisActionMove dlg;
				dlg.m_nStartPoint = eMANU_OPER_TEST_PP1 + nTestPpIdx;
				dlg.m_nSelectAction = eDlgActionPick;
				dlg.DoModal();
			}break;
			case eSeqCmd_TestPP_MoveExchange:
			{
				CDlgAxisActionMove dlg;
				dlg.m_nStartPoint = eMANU_OPER_TEST_PP1 + nTestPpIdx;
				dlg.m_nSelectAction = eDlgActionExchange;
				dlg.DoModal();
			}break;
		}
	}
}


void CVAT_MainDlg::OnBnClickedBtnVatManuTable1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBnClickedAxisManuLoadTbl(eLoadTbl_1);
}


void CVAT_MainDlg::OnBnClickedBtnVatManuTable2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBnClickedAxisManuLoadTbl(eLoadTbl_2);
}


void CVAT_MainDlg::OnBnClickedBtnVatManuTable3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBnClickedAxisManuLoadTbl(eLoadTbl_3);
}


void CVAT_MainDlg::OnBnClickedBtnVatManuTable4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBnClickedAxisManuLoadTbl(eLoadTbl_4);
}

void CVAT_MainDlg::OnBnClickedAxisManuLoadTbl(int nTableIdx)
{
 	if (g_bVATStatusExecute) {
 		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
 		return;
 	}

	if (!g_COVER_SW.GetStatus()) {
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK, AlarmData, NULL);
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable(TRUE);
	if (nInterlockErr != ERR_NO_ERROR) {
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	CDlgCommand_LoadTbl CmdDlg;
	m_GuiLogger.LOG("Load Table #%d Dialog Show", nTableIdx + 1);
	if (IDOK == CmdDlg.DoModal())
	{
		int nSelectedCmd = CmdDlg.GetSelectedSeqCmdId();
		switch (nSelectedCmd)
		{
			case eSeqCmd_LoadTbl_Initial:
			{
				CLoadTblCmd_Initial* pSeqCmd = new CLoadTblCmd_Initial;
				int nErr = g_TaskSystemCtrl.SendCommand(g_TaskLoadTbl[nTableIdx], pSeqCmd, eEqpCtrlMode_Manual);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					g_Error.AlarmReport(nErr, AlarmData, NULL);
				}
				else {
					m_GuiLogger.LOG("Load Table #%d Initial", nTableIdx + 1);
				}
			}break;
			case eSeqCmd_LoadTbl_MovePos:
			{
				CDlgAxisActionMove dlg;
				dlg.m_nStartPoint = eMANU_OPER_LD_TBL1 + nTableIdx;
				dlg.DoModal();
			}break;
		}
	}
}



void CVAT_MainDlg::OnBnClickedBtnVatManuTrayPp1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBnClickedAxisManuTrayPp(eTrayPP_1_LD);
}


void CVAT_MainDlg::OnBnClickedBtnVatManuTrayPp2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBnClickedAxisManuTrayPp(eTrayPP_2_UD);
}

void CVAT_MainDlg::OnBnClickedAxisManuTrayPp(int nTrayPpIdx)
{
 	if (g_bVATStatusExecute) {
 		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
 		return;
 	}

	if (!g_COVER_SW.GetStatus()) {
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK, AlarmData, NULL);
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable(TRUE);
	if (nInterlockErr != ERR_NO_ERROR) {
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}
	CTaskTrayBasePP* pTaskTrayPP = NULL;
	if (nTrayPpIdx == eTrayPP_1_LD) {
		pTaskTrayPP = &g_TaskTrayLoadPP;
	}
	else if (nTrayPpIdx == eTrayPP_2_UD) {
		pTaskTrayPP = &g_TaskTrayUnloadPP;
	}
	CDlgCommand_TrayPP CmdDlg;
	m_GuiLogger.LOG("Tray PP #%d Dialog Show", nTrayPpIdx + 1);
	if (IDOK == CmdDlg.DoModal())
	{
		int nSelectedCmd = CmdDlg.GetSelectedSeqCmdId();
		switch (nSelectedCmd)
		{
			case eSeqCmd_TrayPP_Initial:
			{
				CBaseSeqCmd* pSeqCmd = NULL;
				if (nTrayPpIdx == eTrayPP_1_LD) {
					pSeqCmd = new CTrayLoadPPCmd_Initial;
				}
				else if (nTrayPpIdx == eTrayPP_2_UD) {
					pSeqCmd = new CTrayUnloadPPCmd_Initial;
				}
				
				int nErr = g_TaskSystemCtrl.SendCommand(*pTaskTrayPP, pSeqCmd, eEqpCtrlMode_Manual);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					g_Error.AlarmReport(nErr, AlarmData, NULL);
				}
				else {
					m_GuiLogger.LOG("Tray PP #%d Initial", nTrayPpIdx + 1);
				}
			}break;

			case eSeqCmd_TrayPP_MovePos:
			{
				CDlgAxisActionMove dlg;
				dlg.m_nStartPoint = eMANU_OPER_TRAY_PP1_LD + nTrayPpIdx;
				dlg.m_nSelectAction = eDlgActionOnlyMove;
				dlg.DoModal();
			}break;

			case eSeqCmd_TrayPP_MovePickPlace:
			{
				CDlgAxisActionMove dlg;
				dlg.m_nStartPoint = eMANU_OPER_TRAY_PP1_LD + nTrayPpIdx;
				dlg.m_nSelectAction = eDlgActionPick;
				dlg.DoModal();
			}break;
		}
	}
}


void CVAT_MainDlg::OnBnClickedBtnVatManuFeeder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
 	if (g_bVATStatusExecute) {
 		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
 		return;
 	}

	if (!g_COVER_SW.GetStatus()) {
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK, AlarmData, NULL);
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable(TRUE);
	if (nInterlockErr != ERR_NO_ERROR) {
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	CDlgCommand_Feeder CmdDlg;
	m_GuiLogger.LOG("Feeder Command Dialog Show");
	if (IDOK == CmdDlg.DoModal())
	{
		int nSelectedCmd = CmdDlg.GetSelectedSeqCmdId();
		switch (nSelectedCmd)
		{
			case eSeqCmd_Feeder_Initial:
			{
				CTrayFeederCmd_Initial* pSeqCmd = new CTrayFeederCmd_Initial();
				int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTrayFeeder, pSeqCmd, eEqpCtrlMode_Manual);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					g_Error.AlarmReport(nErr, AlarmData, NULL);
				}
				else {
					m_GuiLogger.LOG("Feeder Initial");
				}
			}break;
			case eSeqCmd_Feeder_MovePos:
			{
				CDlgAxisActionMove dlg;
				dlg.m_nStartPoint = eMANU_OPER_FEEDER;
				dlg.m_nSelectAction = eDlgActionOnlyMove;
				dlg.DoModal();
			}break;
			case eSeqCmd_Feeder_MovePickPlace:
			{
				CDlgAxisActionMove dlg;
				dlg.m_nStartPoint = eMANU_OPER_FEEDER;
				dlg.m_nSelectAction = eDlgActionPick;
				dlg.DoModal();
			}break;
		}
	}
}


void CVAT_MainDlg::OnBnClickedBtnVatManuTransfer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (g_bVATStatusExecute) {
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
 	}

	if (!g_COVER_SW.GetStatus()) {
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK, AlarmData, NULL);
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable(TRUE);
	if (nInterlockErr != ERR_NO_ERROR) {
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	m_GuiLogger.LOG("Transfer Dialog Show");
	CDlgCommand_Transfer CmdDlg;
	if (IDOK == CmdDlg.DoModal())
	{
		int nSelectedCmd = CmdDlg.GetSelectedSeqCmdId();
		switch (nSelectedCmd)
		{
			case eSeqCmd_Transfer_Initial:
			{
				CTransferCmd_Initial* pSeqCmd = new CTransferCmd_Initial;
				int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTransfer, pSeqCmd, eEqpCtrlMode_Manual);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					g_Error.AlarmReport(nErr, AlarmData, NULL);
				}
				else {
					m_GuiLogger.LOG("Transfer Initial");
				}
			}break;

			case eSeqCmd_Transfer_MovePos:
			{
				CDlgAxisActionMove dlg;
				dlg.m_nStartPoint = eMANU_OPER_TRANSFER;
				dlg.m_nSelectAction = eDlgActionOnlyMove;
				dlg.DoModal();
			}break;
			case eSeqCmd_Transfer_MovePickPlace:
			{
				CDlgAxisActionMove dlg;
				dlg.m_nStartPoint = eMANU_OPER_TRANSFER;
				dlg.m_nSelectAction = eDlgActionPick;
				dlg.DoModal();
			}break;
		}
	}
}

void CVAT_MainDlg::OnBnClickedButtonVatManuCmdSkip()
{
	if (g_bVATStatusExecute) {
		AfxMessageBox("Not available while running");
		return;
	}

	m_GuiLogger.LOG("Manual Cmd SKIP Button Click");
	CBaseTaskManager::SendProcessCommand(base_seq_cmd::eProcCmd_Skip, eEqpCtrlMode_Manual, NULL);
}


void CVAT_MainDlg::OnBnClickedButtonVatManuCmdStop()
{
	if (g_bVATStatusExecute) {
		AfxMessageBox("Not available while running");
		return;
	}

	m_GuiLogger.LOG("Manual Cmd STOP Button Click");
	CBaseTaskManager::SendProcessCommand(base_seq_cmd::eProcCmd_Pause, eEqpCtrlMode_Manual, NULL);
}


void CVAT_MainDlg::OnBnClickedBtnVatManuVisionSetting()
{
	if (g_bVATStatusExecute) {
		AfxMessageBox("Not available while running");
		return;
	}

	CVAT_VisionSetting dlg;
	dlg.DoModal();

}


void CVAT_MainDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

// 	CRect rectTest;
// 	GetDesktopWindow()->GetClientRect(&rectTest);
// 	int screenWidth = rectTest.Width();
// 	int screenHeight = rectTest.Height();
// 
// 	SetWindowPos(NULL, 0, 0, screenWidth*0.8, screenHeight*0.8, SWP_NOZORDER | SWP_NOMOVE);
// 
// 
// 
// 
// 	CWnd* pButton = GetDlgItem(IDC_BTN_VAT_MANU_TEST_PP_1);
// 	if (pButton)
// 	{
// 		CRect buttonRect;
// 		pButton->GetWindowRect(buttonRect);
// 		ScreenToClient(buttonRect);
// 
// 		buttonRect.right = buttonRect.left + (buttonRect.Width()*0.8);
// 		buttonRect.bottom = buttonRect.top + (buttonRect.Height()*0.8);
// 		pButton->MoveWindow(buttonRect);
// 	}

}


int CVAT_MainDlg::CheckVatConditionForStart(CString &strErr)
{
	//cover, door, origin
	CString strAxis = _T("");
	int nErr = g_VatTaskSystemCtrl.CheckEqpCondition(strAxis);
	if (nErr != ERR_NO_ERROR) {
		strErr.Format("%s Axis Error", strAxis);
		return nErr;
	}

	//Vision Connect


	//Hand or Part Select 확인.
	if (IsHandOrPartSelected() != TRUE) {
		int nModeType = GetCurrentModeType();
		if (nModeType == VAT_MODE_TYPE_HAND || nModeType == VAT_MODE_TYPE_PICKER) {
			strErr.Format("Please Select the Hand!");
			return ERR_VAT_NOT_SELECTED_HAND;
		}
		else {
			strErr.Format("Please Select the Part!");
			return ERR_VAT_NOT_SELECTED_PART;
		}
	}	

	return ERR_VAT_NO_ERROR;
}



BOOL CVAT_MainDlg::CheckVatAlarmIgnoreForStart()
{
	switch (m_nSeqMode)
	{
		case VAT_SEQ_MODE_Z_FOCUS:
		{
			//Plate Jig 장착 확인.
			int nRes = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_PLATEJIG, ERR_VAT_TYPE_YESNO);
			if (nRes != IDYES) {
				return FALSE; //[사용자 취소]
			}
		}break;
		case VAT_SEQ_MODE_CAM_CAL: 
		case VAT_SEQ_MODE_PICKER_ALIGN:
		{
			//Jig 제거 확인.
			int nRes = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_PLATEJIG_REMOVE, ERR_VAT_TYPE_YESNO);
			if (nRes != IDYES) {
				return FALSE; //[사용자 취소]
			}
		}break;
		case VAT_SEQ_MODE_VAT:
		{
			//[Part 선택에 따라 Confirm Alarm 분기]
			//C.O.K 장착 확인.
			int nRes = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_COK, ERR_VAT_TYPE_YESNO);
			if (nRes != IDYES) {
				return FALSE; //[사용자 취소]
			}

			//Jig 장착 확인 - C_Tray Part 있을 경우.
			BOOL bIsSelected_CTray = FALSE;
			for (int nPart = VAT_IDX_TRAY_PP_CONV_1; nPart < VAT_IDX_TRAY_PP_CONV_7 && !bIsSelected_CTray; nPart++) {
				BOOL bIsSelected_CTray = m_pgZAutoTeaching.m_pgMeasure._Com_IsPartSelected(VAT_HAND_UNLOAD_TRAYPP, nPart);
				if (bIsSelected_CTray) {
					int nRes = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_C_TRAY_JIG, ERR_VAT_TYPE_YESNO);
					if (nRes != IDYES) {
						return FALSE;
					}
				}
			}

			//Jig 장착 확인 - Station Part 있을 경우.
			BOOL bIsSelected_Station = FALSE;
			for (int nHand = VAT_HAND_LOAD_TESTPP1; nHand <= VAT_HAND_UNLOAD_TESTPP2 &&!bIsSelected_Station; nHand++) {
				for (int nPart = VAT_IDX_STATION_1; nPart < VAT_IDX_STATION_4; nPart++) {
					BOOL bIsSelected_Station = m_pgZAutoTeaching.m_pgMeasure._Com_IsPartSelected(nHand, nPart);
					if (bIsSelected_Station) {
						int nRes = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_STATION_JIG, ERR_VAT_TYPE_YESNO);
						if (nRes != IDYES) {
							return FALSE;
						}

						break;
					}
				}
			}
		}break;
		case VAT_SEQ_MODE_Z_TEACHING:
		{
			//[여러개 확인 필요]
			//C.O.K 장착 확인.
			int nRes = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_COK, ERR_VAT_TYPE_YESNO);
			if (nRes != IDYES) {
				return FALSE; //[사용자 취소]
			}

			//Jig 장착 확인 - C_Tray Part 있을 경우.
			for (int nPart = VAT_IDX_TRAY_PP_CONV_1; nPart < VAT_IDX_TRAY_PP_CONV_7; nPart++) {
				BOOL bIsSelected = m_pgZAutoTeaching.m_pgMeasure._Com_IsPartSelected(VAT_HAND_UNLOAD_TRAYPP, nPart);
				if (bIsSelected) {
					int nRes = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_C_TRAY_JIG, ERR_VAT_TYPE_YESNO);
					if (nRes == IDYES) {
						return TRUE; //[사용자 확인]
					}
					else {
						return FALSE; //[사용자 취소]
					}
				}
			}
		}break;
		default:
			break;
	}

	return TRUE; //[사용자 모든 Confirm Alarm 확인]
}


void CVAT_MainDlg::InitModeParamForStart()
{
	//GUI에서 Setting한 Data를 옮김.(real data-> seq data)
	g_VATCommonData.DataCopyRealToSequence(m_nSeqMode);

	//GUI에서 선택 된 Hand 또는 Part Auto에 생성.
	CreateSelectedTargetByGUI();
}



BOOL CVAT_MainDlg::IsHandOrPartSelected()
{
	BOOL bIsSelected = FALSE;

	switch (m_nSeqMode)
	{
	case VAT_SEQ_MODE_Z_FOCUS:				bIsSelected = m_pgZAutoFocus._Com_IsAnyHandSelected();						 break;
	case VAT_SEQ_MODE_CAM_CAL:				bIsSelected = m_pgCamCenterCal._Com_IsAnyHandSelected();					 break;
	case VAT_SEQ_MODE_PICKER_ALIGN:			bIsSelected = m_pgPickerAlign.m_pgMeasure._Com_IsAnyHandSelected();						 break;
	case VAT_SEQ_MODE_VAT:					bIsSelected = m_pgVisionAutoTeaching.m_pgMeasure._Com_IsAnyPartSelected();	 break;
	case VAT_SEQ_MODE_Z_TEACHING:			bIsSelected = m_pgZAutoTeaching.m_pgMeasure._Com_IsAnyPartSelected();		 break;
	default:
		break;
	}

	return bIsSelected;
}


void CVAT_MainDlg::CreateSelectedTargetByGUI()
{
	switch (m_nSeqMode)
	{
		case VAT_SEQ_MODE_Z_FOCUS:				 
		case VAT_SEQ_MODE_CAM_CAL:				 		 
		{
			g_VatTaskSystemCtrl.ClearAllMeasureHand(); //Status 'INIT'

			CVATMain_Common* pHandSelectedPg[2] = { &m_pgZAutoFocus, &m_pgCamCenterCal};
			for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++) {
				BOOL bIsSelected = pHandSelectedPg[m_nSeqMode]->_Com_IsHandSelected(nHand);
			
				//Auto로 Selected Hand 설정.
				g_VatTaskSystemCtrl.SetMeasureHandToEachAuto(nHand, bIsSelected); //Status 'READY'
			}
		}break;
		case VAT_SEQ_MODE_PICKER_ALIGN:
		{
			g_VatTaskSystemCtrl.ClearAllMeasurePicker(); 

			for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++) {
				BOOL bIsHandSelected = m_pgPickerAlign.m_pgMeasure._Com_IsHandSelected(nHand);

				//Hand Selected 확인.
				if (bIsHandSelected)
				{
					for (int nPkr = 0; nPkr < VAT_PICKER_MAX; nPkr++) {			
						BOOL bIsPkrSelected = g_VATCommonData.m_stVATPickerAlignInfoSeq.nSelectedPicker[nHand][nPkr];
						g_VatTaskSystemCtrl.SetMeasurePickerToEachAuto(nHand, nPkr, bIsPkrSelected);	
					}
				}
			}
		}break;
		case VAT_SEQ_MODE_Z_TEACHING:	
		case VAT_SEQ_MODE_VAT:
		{
			g_VatTaskSystemCtrl.ClearAllMeasurePart();

			for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand <= VAT_HAND_MAX; nHand++) {
				int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
				for (int nPart = 0; nPart < nMaxPart; nPart++) {
					//해당 Hand의 Part Selected 확인.
					BOOL bIsSelected = FALSE;
					if (m_nSeqMode == VAT_SEQ_MODE_Z_TEACHING) {
						bIsSelected = m_pgZAutoTeaching.m_pgMeasure._Com_IsPartSelected(nHand, nPart);
					}
					else {
						bIsSelected = m_pgVisionAutoTeaching.m_pgMeasure._Com_IsPartSelected(nHand, nPart);
					}

					//Auto로 Selected Part 설정.
					g_VatTaskSystemCtrl.SetMeasurePartToEachAuto(nHand, nPart, bIsSelected);
				}
			}
		}break;
		default:
			break;
	}
}


void CVAT_MainDlg::ChangeMeasurePage()
{
	switch (m_nSeqMode)
	{			
		case VAT_SEQ_MODE_PICKER_ALIGN:			
		{
			m_pgPickerAlign.DisplayPickerAlignTabMode(CVATMain_PickerAlign::MTPI_PICKER_ALIGN_MEASURE);
		}break;
		case VAT_SEQ_MODE_VAT: 
		{
			m_pgVisionAutoTeaching.DisplayVisionAutoTeachingTabMode(CVATMain_VisionAutoTeaching::MTPI_VISION_AUTO_TEACHING_MEASURE);
		}break; 
		case VAT_SEQ_MODE_Z_TEACHING:
		{
			m_pgZAutoTeaching.DisplayZAutoTeachingTabMode(CVATMain_ZAutoTeaching::MTPI_Z_AUTO_TEACHING_MEASURE);
		}break;
		default: //VAT_SEQ_MODE_Z_FOCUS, VAT_SEQ_MODE_CAM_CAL
			break;
	}
}


void CVAT_MainDlg::InitSimul()
{
	//Simulation 관련 초기화.
	for (int nCam = 0; nCam < VAT_CAM_MAX; nCam++) {
		g_DMCont.m_dmVAT.SN(NDM14_VAT_SIMUL_CAM_1_RCV_CNT + nCam, 0);
	}
}

void CVAT_MainDlg::OnBnClickedBtnVatManuStacker3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	OnBnClickedAxisManuStacker(eSTACKER_03);
}


void CVAT_MainDlg::OnBnClickedBtnVatManuStacker4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	OnBnClickedAxisManuStacker(eSTACKER_04);
}


void CVAT_MainDlg::OnBnClickedBtnVatManuStacker5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	OnBnClickedAxisManuStacker(eSTACKER_05);
}


void CVAT_MainDlg::OnBnClickedBtnVatManuStacker6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	OnBnClickedAxisManuStacker(eSTACKER_06);
}


void CVAT_MainDlg::OnBnClickedBtnVatManuStacker7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	OnBnClickedAxisManuStacker(eSTACKER_07);
}

void CVAT_MainDlg::OnBnClickedAxisManuStacker(int nStackerIdx)
{
	BOOL bManualMode = FALSE;

	if (g_bVATStatusExecute) {
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	if (!g_COVER_SW.GetStatus()) {
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK, AlarmData, NULL);
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable(bManualMode);
	if (nInterlockErr != ERR_NO_ERROR) {
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	m_GuiLogger.LOG("Stacker#%d Dialog Show", nStackerIdx + 1);
	CDlgCommand_Stacker CmdDlg;
	CmdDlg.m_nStackerIdx = nStackerIdx;
	if (IDOK == CmdDlg.DoModal())
	{
		int nSelectedCmd = CmdDlg.GetSelectedSeqCmdId();
		switch (nSelectedCmd)
		{
		case eSeqCmd_Stacker_Initial:
		{

		}break;
		case eSeqCmd_Stacker_Load:
		{
			CStackerCmd_Load* pSeqCmd = new CStackerCmd_Load;
			int nErr = g_TaskSystemCtrl.SendCommand(g_TaskStacker[nStackerIdx], pSeqCmd, eEqpCtrlMode_Manual);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
			}
		}break;
		case eSeqCmd_Stacker_TrayDn:
		{
			CStackerCmd_TrayDn* pSeqCmd = new CStackerCmd_TrayDn;
			int nErr = g_TaskSystemCtrl.SendCommand(g_TaskStacker[nStackerIdx], pSeqCmd, eEqpCtrlMode_Manual);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
			}
		}break;
		case eSeqCmd_Stacker_TrayUp:
		{
			CStackerCmd_TrayUp* pSeqCmd = new CStackerCmd_TrayUp;
			int nErr = g_TaskSystemCtrl.SendCommand(g_TaskStacker[nStackerIdx], pSeqCmd, eEqpCtrlMode_Manual);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
			}
		}break;
		case eSeqCmd_Stacker_Unload:
		{
			CStackerCmd_Unload* pSeqCmd = new CStackerCmd_Unload;
			int nErr = g_TaskSystemCtrl.SendCommand(g_TaskStacker[nStackerIdx], pSeqCmd, eEqpCtrlMode_Manual);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
			}
		}break;
		case eSeqCmd_Stacker_TrayOut:
		{
			CStackerCmd_TrayOut* pSeqCmd = new CStackerCmd_TrayOut;
			int nErr = g_TaskSystemCtrl.SendCommand(g_TaskStacker[nStackerIdx], pSeqCmd, eEqpCtrlMode_Manual);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
			}
		}break;
		case eSeqCmd_Stacker_Vibration:
		{
			double Action_Delay_Sec = g_DMCont.m_dmConveyor.GD(DDM6_VIBRATOR_ACT_DELAY_SEC);
			int Action_Act_Time = g_DMCont.m_dmConveyor.GN(NDM6_VIBRATOR_ACT_TIMES);
			CStackerCmd_Vibration* pSeqCmd = new CStackerCmd_Vibration(Action_Delay_Sec, Action_Act_Time);
			int nErr = g_TaskSystemCtrl.SendCommand(g_TaskStacker[nStackerIdx], pSeqCmd, eEqpCtrlMode_Manual);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
			}
		}break;
		}
	}
}