// DlgDebugAutoStacker.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgDebugAutoStacker.h"
#include "afxdialogex.h"
#include "DlgDeviceInfoEditor.h"
#include <algorithm>
#include <unordered_map>

// CDlgDebugAutoStacker 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgDebugAutoStacker, CDialogEx)

CDlgDebugAutoStacker::CDlgDebugAutoStacker(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgDebugAutoStacker::IDD, pParent)
{
	m_nWorkSetplate = 0;
	::ZeroMemory( m_nFrontConveyorExistCtrlId, sizeof( m_nFrontConveyorExistCtrlId ) );
	::ZeroMemory( m_nRearConveyorExistCtrlId, sizeof(m_nRearConveyorExistCtrlId) );
	::ZeroMemory(m_nFrontConveyorDataCtrlId, sizeof(m_nFrontConveyorDataCtrlId));
	::ZeroMemory(m_nRearConveyorDataCtrlId, sizeof(m_nRearConveyorDataCtrlId));

	m_bUPH_Start = FALSE;

	m_nOnOffDi_Val = 0;
	m_pInputValueTargetWnd = NULL;
	
	int nTrayFullCtrlID[eMaxStackerCount] = {
		IDC_CHECK_STACKER_FULL_SEN1, IDC_CHECK_STACKER_FULL_SEN2, IDC_CHECK_STACKER_FULL_SEN3,
		IDC_CHECK_STACKER_FULL_SEN4,IDC_CHECK_STACKER_FULL_SEN5,IDC_CHECK_STACKER_FULL_SEN6,
		IDC_CHECK_STACKER_FULL_SEN7
	};
	memcpy( m_nTrayFullCtrlId, nTrayFullCtrlID, sizeof( int ) * eMaxStackerCount );

	int nTrayExistCtrlId[eMaxStackerCount] = {
		IDC_CHECK_STACKER_EXIST_SEN1, IDC_CHECK_STACKER_EXIST_SEN2, IDC_CHECK_STACKER_EXIST_SEN3,
		IDC_CHECK_STACKER_EXIST_SEN4,IDC_CHECK_STACKER_EXIST_SEN5,IDC_CHECK_STACKER_EXIST_SEN6,
		IDC_CHECK_STACKER_EXIST_SEN7
	};
	memcpy( m_nTrayExistCtrlId, nTrayExistCtrlId, sizeof( int ) * eMaxStackerCount );

	int nFrontConveyorExistCtrlId[ eMaxStackerCount ] = {
		IDC_CHECK_CONVEYOR_EXIST_1,IDC_CHECK_CONVEYOR_EXIST_2,IDC_CHECK_CONVEYOR_EXIST_3,
		IDC_CHECK_CONVEYOR_EXIST_4,IDC_CHECK_CONVEYOR_EXIST_5,IDC_CHECK_CONVEYOR_EXIST_6,
		IDC_CHECK_CONVEYOR_EXIST_7
	};
	memcpy( m_nFrontConveyorExistCtrlId, nFrontConveyorExistCtrlId, sizeof( int ) * eMaxStackerCount );

	//int nFrontConveyorSlowCtrlId[ eMaxStackerCount ] = {
	//	IDC_CHECK_CONVEYOR_F_SLOW_SEN_1,IDC_CHECK_CONVEYOR_F_SLOW_SEN_2,IDC_CHECK_CONVEYOR_F_SLOW_SEN_3,
	//	IDC_CHECK_CONVEYOR_F_SLOW_SEN_4,IDC_CHECK_CONVEYOR_F_SLOW_SEN_5,IDC_CHECK_CONVEYOR_F_SLOW_SEN_6,
	//	IDC_CHECK_CONVEYOR_F_SLOW_SEN_7
	//};
	//memcpy( m_nFrontConveyorSlowCtrlId, nFrontConveyorSlowCtrlId, sizeof( int ) * eMaxStackerCount );
	//
	//int nRearConveyorSlowCtrld[ eMaxStackerCount ] = {
	//	IDC_CHECK_CONVEYOR_R_SLOW_SEN_1,IDC_CHECK_CONVEYOR_R_SLOW_SEN_2,IDC_CHECK_CONVEYOR_R_SLOW_SEN_3,
	//	IDC_CHECK_CONVEYOR_R_SLOW_SEN_4,IDC_CHECK_CONVEYOR_R_SLOW_SEN_5,IDC_CHECK_CONVEYOR_R_SLOW_SEN_6,
	//	IDC_CHECK_CONVEYOR_R_SLOW_SEN_7
	//};
	//memcpy( m_nRearConveyorSlowCtrld, nRearConveyorSlowCtrld, sizeof( int ) * eMaxStackerCount );

	int nRearConveyorExistCtrlId[ eMaxStackerCount ] = {
		IDC_CHECK_CONVEYOR_R_EXIST_SEN_1,IDC_CHECK_CONVEYOR_R_EXIST_SEN_2,IDC_CHECK_CONVEYOR_R_EXIST_SEN_3,
		IDC_CHECK_CONVEYOR_R_EXIST_SEN_4,IDC_CHECK_CONVEYOR_R_EXIST_SEN_5,IDC_CHECK_CONVEYOR_R_EXIST_SEN_6,
		IDC_CHECK_CONVEYOR_R_EXIST_SEN_7
	};
	memcpy( m_nRearConveyorExistCtrlId, nRearConveyorExistCtrlId, sizeof( int ) * eMaxStackerCount );

	int nFrontConveyorDataCtrlId[eMaxStackerCount] = {
		IDC_CHECK_CONVEYOR_F_DATA1,IDC_CHECK_CONVEYOR_F_DATA2,IDC_CHECK_CONVEYOR_F_DATA3,
		IDC_CHECK_CONVEYOR_F_DATA4,IDC_CHECK_CONVEYOR_F_DATA5,IDC_CHECK_CONVEYOR_F_DATA6,
		IDC_CHECK_CONVEYOR_F_DATA7
	};
	memcpy(m_nFrontConveyorDataCtrlId, nFrontConveyorDataCtrlId, sizeof(int) * eMaxStackerCount);

	int nRearConveyorDataCtrlId[ eMaxStackerCount ] = {
		IDC_CHECK_CONVEYOR_R_DATA1,IDC_CHECK_CONVEYOR_R_DATA2,IDC_CHECK_CONVEYOR_R_DATA3,
		IDC_CHECK_CONVEYOR_R_DATA4,IDC_CHECK_CONVEYOR_R_DATA5,IDC_CHECK_CONVEYOR_R_DATA6,
		IDC_CHECK_CONVEYOR_R_DATA7
	};
	memcpy( m_nRearConveyorDataCtrlId, nRearConveyorDataCtrlId, sizeof( int ) * eMaxStackerCount );
}

CDlgDebugAutoStacker::~CDlgDebugAutoStacker()
{
}

//==============================================================================
// 도착한 명령을 큐에서 꺼내어 실행 시킨다.
//==============================================================================
void CDlgDebugAutoStacker::ThreadProc1()
{
	SetThreadInterval(0, 50);
	while (GetThreadAliveFlag(0)) {
		if (GetPauseFlag(0)) {
			NEXT;
			continue;
		}
		//for (int i = 0; i < MAX_THREAD_SCAN_COUNT; i++) {
			CBaseTaskManager::SeqCmdRunning();
		//}
		NEXT_10;
	}
}

void CDlgDebugAutoStacker::OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{

}

int CDlgDebugAutoStacker::OnEvent(const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList/*=NULL*/)
{
	return 0;
	CString strMsg;
	g_DMCont.GetEvtMessage(nEventId, strMsg);
	UpdateEventMessage(strMsg);
	if (pDataList != NULL)
	{
		int nItemCount = pDataList->m_fnGetCount();
		for (int i = 0; i < nItemCount; i++)
		{
			CString strData = pDataList->m_fnGetItemString(i);
			UpdateEventMessage(strData);
		}
	}

	// event를 각 unit 자리에 쓴다. 보기 편하게.
	if (pSender == &g_TaskTrayLoadPP)
	{
		SetDlgItemText(IDC_STATIC_TASK_TRAYPP_EVT, strMsg);
	}
	else if (pSender == &g_TaskTrayUnloadPP)
	{
		// @@@ : UI 작업 필요.
		// SetDlgItemText(IDC_STATIC_TASK_TRAYPP_EVT, strMsg);
	}
	else if (pSender == &g_TaskLoadTbl[eLoadTbl_1])
	{
		CString sLdMsg;
		sLdMsg.Format(_T("%s(%d)"), (LPCTSTR)strMsg, g_TaskLoadTbl[eLoadTbl_1].GetWorkReserve());
		SetDlgItemText(IDC_STATIC_TASK_LOADTBL1_EVT, sLdMsg);
	}
	else if (pSender == &g_TaskLoadTbl[eLoadTbl_2])
	{
		CString sLdMsg;
		sLdMsg.Format(_T("%s(%d)"), (LPCTSTR)strMsg, g_TaskLoadTbl[eLoadTbl_2].GetWorkReserve());
		SetDlgItemText(IDC_STATIC_TASK_LOADTBL2_EVT, sLdMsg);
	}
	else if (pSender == &g_TaskTestPP[eTestPP_1])
	{
		SetDlgItemText(IDC_STATIC_TASK_TESTPP1_EVT, strMsg);
	}
	else if (pSender == &g_TaskTestPP[eTestPP_2])
	{
		SetDlgItemText(IDC_STATIC_TASK_TESTPP2_EVT, strMsg);
	}
// 	else if (pSender == &g_TaskPressUnit[ePressUnit_Up_1])
// 	{
// 		SetDlgItemText(IDC_STATIC_TASK_PRESS_1_UL_EVT, strMsg);
// 	}
// 	else if (pSender == &g_TaskPressUnit[ePressUnit_Up_2])
// 	{
// 		SetDlgItemText(IDC_STATIC_TASK_PRESS_1_UR_EVT, strMsg);
// 	}
// 	else if (pSender == &g_TaskPressUnit[ePressUnit_Up_3])
// 	{
// 		SetDlgItemText(IDC_STATIC_TASK_PRESS_2_UL_EVT, strMsg);
// 	}
// 	else if (pSender == &g_TaskPressUnit[ePressUnit_Up_4])
// 	{
// 		SetDlgItemText(IDC_STATIC_TASK_PRESS_2_UR_EVT, strMsg);
// 	}
// 	else if (pSender == &g_TaskPressUnit[ePressUnit_Up_5])
// 	{
// 		SetDlgItemText(IDC_STATIC_TASK_PRESS_3_UL_EVT, strMsg);
// 	}
// 	else if (pSender == &g_TaskPressUnit[ePressUnit_Up_6])
// 	{
// 		SetDlgItemText(IDC_STATIC_TASK_PRESS_3_UR_EVT, strMsg);
// 	}
// 	else if (pSender == &g_TaskPressUnit[ePressUnit_Up_7])
// 	{
// 		SetDlgItemText(IDC_STATIC_TASK_PRESS_4_UL_EVT, strMsg);
// 	}
// 	else if (pSender == &g_TaskPressUnit[ePressUnit_Up_8])
// 	{
// 		SetDlgItemText(IDC_STATIC_TASK_PRESS_4_UR_EVT, strMsg);
// 	}
	else if (pSender == &g_TaskPressUnit[ePressUnit_Down_1])
	{
		SetDlgItemText(IDC_STATIC_TASK_PRESS_1_DL_EVT, strMsg);
	}
	else if (pSender == &g_TaskPressUnit[ePressUnit_Down_2])
	{
		SetDlgItemText(IDC_STATIC_TASK_PRESS_1_DR_EVT, strMsg);
	}
	else if (pSender == &g_TaskPressUnit[ePressUnit_Down_3])
	{
		SetDlgItemText(IDC_STATIC_TASK_PRESS_2_DL_EVT, strMsg);
	}
	else if (pSender == &g_TaskPressUnit[ePressUnit_Down_4])
	{
		SetDlgItemText(IDC_STATIC_TASK_PRESS_2_DR_EVT, strMsg);
	}
	else if (pSender == &g_TaskPressUnit[ePressUnit_Down_5])
	{
		SetDlgItemText(IDC_STATIC_TASK_PRESS_3_DL_EVT, strMsg);
	}
	else if (pSender == &g_TaskPressUnit[ePressUnit_Down_6])
	{
		SetDlgItemText(IDC_STATIC_TASK_PRESS_3_DR_EVT, strMsg);
	}
	else if (pSender == &g_TaskPressUnit[ePressUnit_Down_7])
	{
		SetDlgItemText(IDC_STATIC_TASK_PRESS_4_DL_EVT, strMsg);
	}
	else if (pSender == &g_TaskPressUnit[ePressUnit_Down_8])
	{
		SetDlgItemText(IDC_STATIC_TASK_PRESS_4_DR_EVT, strMsg);
	}

	return 0;
}

int CDlgDebugAutoStacker::OnCommandRsp(CBaseSeqCmd* pRespCmd, int nEventId, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CDlgDebugAutoStacker::UpdateEventMessage(const char* fmt, ...)
{
	char szEventMessage[1024] = { 0, };
	va_list va;
	va_start(va, fmt);
	vsprintf_s(szEventMessage, sizeof(szEventMessage), fmt, va);
	va_end(va);

	if (m_editManuOperEvent.GetLineCount() > 100) {
		m_editManuOperEvent.SetWindowText("");
	}
	CString strTimeString;
	GetTimeString(strTimeString);

	CString strTemp;
	m_editManuOperEvent.GetWindowText(strTemp);
	if (strTemp != "") {
		strTemp += _T("\r\n");
	}
	strTemp += strTimeString + _T(" ");
	strTemp += szEventMessage;

	m_editManuOperEvent.SetWindowText(strTemp);
}

void CDlgDebugAutoStacker::GetTimeString(CString& strTimeString)
{
	SYSTEMTIME t;
	::GetLocalTime(&t);
	strTimeString.Format("[%02d:%02d.%03d]", t.wMinute, t.wSecond, t.wMilliseconds);
}


void CDlgDebugAutoStacker::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MANU_OPER_EVENT, m_editManuOperEvent);
	DDX_Control(pDX, IDC_STATIC_TOWER_R, m_StaticTowerR);
	DDX_Control(pDX, IDC_STATIC_TOWER_G, m_StaticTowerG);
	DDX_Control(pDX, IDC_STATIC_TOWER_Y, m_StaticTowerY);
	DDX_Control(pDX, IDC_SPREAD_TESTER_SELECT4, m_SpreadClrInfo);
	DDX_Control( pDX, IDC_SPREAD_EXAMPLE, m_SpreadClrTest );
	DDX_Control( pDX, IDC_SPREAD_TEST_PP1_L, m_SPRD_TestPP1_L );
	DDX_Control(pDX, IDC_SPREAD_TEST_PP1_U, m_SPRD_TestPP1_U);
	DDX_Control(pDX, IDC_SPREAD_TEST_PP2_L, m_SPRD_TestPP2_L);
	DDX_Control(pDX, IDC_SPREAD_TEST_PP2_U, m_SPRD_TestPP2_U);

	DDX_Control(pDX, IDC_STATIC_SWITCH_COVER, m_StaticSwitchCover);

	//DDX_Control( pDX, IDC_SPREAD_ART_TBL1, m_SPRD_Retest_Buff_Table1 );
	DDX_Control( pDX, IDC_SPREAD_CLEAN_TBL1, m_SPRD_Socketclean_Buff_Table1 );
	//DDX_Control( pDX, IDC_SPREAD_ART_TBL2, m_SPRD_Retest_Buff_Table2 );
	DDX_Control( pDX, IDC_SPREAD_CLEAN_TBL2, m_SPRD_Socketclean_Buff_Table2 ); 
	DDX_Control( pDX, IDC_SPREAD_LOAD_TBL1, m_SPRD_Load_Table1 );
	DDX_Control( pDX, IDC_SPREAD_LOAD_TBL2, m_SPRD_Load_Table2 );
	DDX_Control(pDX, IDC_SPREAD_LOAD_TBL3, m_SPRD_Load_Table3);
	DDX_Control(pDX, IDC_SPREAD_LOAD_TBL4, m_SPRD_Load_Table4);

	DDX_Control(pDX, IDC_STATIC_LD1_TEST_SIDE1, m_StaticTblTestSidePos[0][0]);
	DDX_Control(pDX, IDC_STATIC_LD1_TEST_SIDE2, m_StaticTblTestSidePos[0][1]);
	DDX_Control(pDX, IDC_STATIC_LD1_TEST_SIDE3, m_StaticTblTestSidePos[0][2]);
	DDX_Control(pDX, IDC_STATIC_LD1_TEST_SIDE4, m_StaticTblTestSidePos[0][3]);

	DDX_Control(pDX, IDC_STATIC_LD2_TEST_SIDE1, m_StaticTblTestSidePos[1][0]);
	DDX_Control(pDX, IDC_STATIC_LD2_TEST_SIDE2, m_StaticTblTestSidePos[1][1]);
	DDX_Control(pDX, IDC_STATIC_LD2_TEST_SIDE3, m_StaticTblTestSidePos[1][2]);
	DDX_Control(pDX, IDC_STATIC_LD2_TEST_SIDE4, m_StaticTblTestSidePos[1][3]);

	DDX_Control(pDX, IDC_STATIC_LD3_TEST_SIDE1, m_StaticTblTestSidePos[2][0]);
	DDX_Control(pDX, IDC_STATIC_LD3_TEST_SIDE2, m_StaticTblTestSidePos[2][1]);
	DDX_Control(pDX, IDC_STATIC_LD3_TEST_SIDE3, m_StaticTblTestSidePos[2][2]);
	DDX_Control(pDX, IDC_STATIC_LD3_TEST_SIDE4, m_StaticTblTestSidePos[2][3]);

	DDX_Control(pDX, IDC_STATIC_LD4_TEST_SIDE1, m_StaticTblTestSidePos[3][0]);
	DDX_Control(pDX, IDC_STATIC_LD4_TEST_SIDE2, m_StaticTblTestSidePos[3][1]);
	DDX_Control(pDX, IDC_STATIC_LD4_TEST_SIDE3, m_StaticTblTestSidePos[3][2]);
	DDX_Control(pDX, IDC_STATIC_LD4_TEST_SIDE4, m_StaticTblTestSidePos[3][3]);

	DDX_Control(pDX, IDC_STATIC_LD1_LOAD_SIDE1, m_StaticTblLoadSidePos[0]);
	DDX_Control(pDX, IDC_STATIC_LD2_LOAD_SIDE1, m_StaticTblLoadSidePos[1]);
	DDX_Control(pDX, IDC_STATIC_LD3_LOAD_SIDE1, m_StaticTblLoadSidePos[2]);
	DDX_Control(pDX, IDC_STATIC_LD4_LOAD_SIDE1, m_StaticTblLoadSidePos[3]);

	DDX_Control(pDX, IDC_STATIC_RESERVE_TRANSFER, m_StaticReserve[0]);
	DDX_Control(pDX, IDC_STATIC_RESERVE_FEEDER, m_StaticReserve[1]);
	DDX_Control(pDX, IDC_STATIC_RESERVE_UNLOAD_TRAY_PP, m_StaticReserve[2]);

}

BEGIN_EVENTSINK_MAP( CDlgDebugAutoStacker, CDialogEx )
//	ON_EVENT( CDlgDebugAutoStacker, IDC_SPREAD_LOAD_TBL1, 5, CDlgDebugAutoStacker::ClickSpreadLoadTbl1, VTS_I4 VTS_I4 )
//	ON_EVENT( CDlgDebugAutoStacker, IDC_SPREAD_LOAD_TBL2, 5, CDlgDebugAutoStacker::ClickSpreadLoadTbl2, VTS_I4 VTS_I4 )
//	ON_EVENT( CDlgDebugAutoStacker, IDC_SPREAD_LOAD_TBL1, DISPID_MOUSEDOWN, CDlgDebugAutoStacker::MouseDownSpreadLoadTbl1, VTS_I2 VTS_I2 VTS_I4 VTS_I4 )
	ON_EVENT( CDlgDebugAutoStacker, IDC_SPREAD_LOAD_TBL1, DISPID_KEYDOWN, CDlgDebugAutoStacker::KeyDownSpreadLoadTbl1, VTS_PI2 VTS_I2 )
	ON_EVENT( CDlgDebugAutoStacker, IDC_SPREAD_LOAD_TBL2, DISPID_KEYDOWN, CDlgDebugAutoStacker::KeyDownSpreadLoadTbl2, VTS_PI2 VTS_I2 )
	ON_EVENT( CDlgDebugAutoStacker, IDC_SPREAD_LOAD_TBL1, 22, CDlgDebugAutoStacker::RightClickSpreadLoadTbl1, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_I4 )
	ON_EVENT( CDlgDebugAutoStacker, IDC_SPREAD_LOAD_TBL2, 22, CDlgDebugAutoStacker::RightClickSpreadLoadTbl2, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_I4 )
END_EVENTSINK_MAP()

BEGIN_MESSAGE_MAP(CDlgDebugAutoStacker, CDialogEx)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_AUTO_STACKER_INITIAL, &CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerInitial)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_STACKER_LOT_START, &CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerLotStart)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_STACKER_LOT_PAUSE, &CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerLotPause)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_STACKER_LOT_RESUME, &CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerLotResume)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_STACKER_LOT_ONECYCLE, &CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerLotOnecycle)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_STACKER_LOT_RETRY, &CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerLotRetry)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_STACKER_LOT_SKIP, &CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerLotSkip)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_STACKER_LOT_CLEANOUT, &CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerLotCleanout)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_STACKER_AUTO, &CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerAuto)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_STACKER_MANUAL, &CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerManual)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_STACKER_START, &CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerStart)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_STACKER_STOP, &CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerStop)
	ON_COMMAND( ID_SIMULATION_INPUTSIMULATIONVALUE, &CDlgDebugAutoStacker::OnSimulationInputsimulationvalue )
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_SIMULATION_ON, &CDlgDebugAutoStacker::OnSimulationOn)
	ON_COMMAND(ID_SIMULATION_OFF, &CDlgDebugAutoStacker::OnSimulationOff)
	ON_COMMAND( ID_SIMULATION_ALLON_REAREXISTSEN, &CDlgDebugAutoStacker::OnSimulationConveyorRearAllOn)
	ON_COMMAND( ID_SIMULATION_ALLOFF_REAREXISTSEN, &CDlgDebugAutoStacker::OnSimulationConveyorRearAllOff)
	ON_COMMAND( ID_SIMULATION_ALLON_FRONTEXISTSEN, &CDlgDebugAutoStacker::OnSimulationConveyorFrontAllOn )
	ON_COMMAND( ID_SIMULATION_ALLOFF_FRONTEXISTSEN, &CDlgDebugAutoStacker::OnSimulationConveyorFrontAllOff )
	ON_COMMAND( ID_SIMULATION_ALLON_UNLOADEXIST, &CDlgDebugAutoStacker::OnSimulationUnloadExistAllOn )
	ON_COMMAND( ID_SIMULATION_ALLOFF_UNLOADEXIST, &CDlgDebugAutoStacker::OnSimulationUnloadExistAllOff )


	ON_BN_CLICKED(IDC_BUTTON_OP_PANEL_LAMP_SW, &CDlgDebugAutoStacker::OnBnClickedButtonOpPanelLampSw)
	ON_BN_CLICKED(IDC_BUTTON_OP_PANEL_SOUND_SW, &CDlgDebugAutoStacker::OnBnClickedButtonOpPanelSoundSw)
	ON_BN_CLICKED(IDC_BUTTON_OP_PANEL_COVER_SW, &CDlgDebugAutoStacker::OnBnClickedButtonOpPanelCoverSw)
	ON_BN_CLICKED(IDC_BTN_UPH, &CDlgDebugAutoStacker::OnBnClickedBtnUph)
	ON_BN_CLICKED(IDC_BTN_UPH_STOP, &CDlgDebugAutoStacker::OnBnClickedBtnUphStop)
	ON_BN_CLICKED(IDC_BTN_JAM_LOT_DATA_CLEAR, &CDlgDebugAutoStacker::OnBnClickedBtnJamLotDataClear)
	ON_BN_CLICKED(IDOK, &CDlgDebugAutoStacker::OnBnClickedOk)
	ON_BN_CLICKED( IDC_BUTTON_MOTION_DB_RELOAD, &CDlgDebugAutoStacker::OnBnClickedButtonMotionDbReload )
	ON_BN_CLICKED( IDC_BUTTON_AUTO_STACKER_LOAD_TABLE_1_INIT_IDLE_RESERVE, &CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerLoadTable1InitIdleReserve )
	ON_BN_CLICKED( IDC_BUTTON_AUTO_STACKER_LOAD_TABLE_2_INIT_IDLE_RESERVE, &CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerLoadTable2InitIdleReserve )
	ON_BN_CLICKED(IDC_BUTTON_EVENT_LOG_ALL_DELETE, &CDlgDebugAutoStacker::OnBnClickedButtonEventLogAllDelete)

	ON_BN_CLICKED(IDC_BUTTON_AUTO_STACKER_MANUAL3, &CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerManual3)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_STACKER_MANUAL4, &CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerManual4)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_STACKER_LOAD_TABLE_3_INIT_IDLE_RESERVE, &CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerLoadTable3InitIdleReserve)
	ON_STN_DBLCLK(IDC_STATIC_TASK_LOADTBL1_CMD_STAGE, &CDlgDebugAutoStacker::OnStnDblclickStaticTaskLoadtbl1CmdStage)
	ON_STN_DBLCLK(IDC_STATIC_TASK_LOADTBL4_CMD_STAGE, &CDlgDebugAutoStacker::OnStnDblclickStaticTaskLoadtbl4CmdStage)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_STACKER_MANUAL5, &CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerManual5)
END_MESSAGE_MAP()


// CDlgDebugAutoStacker 메시지 처리기입니다.


BOOL CDlgDebugAutoStacker::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgDebugAutoStacker::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CBaseTaskManager::Initialize();

	m_StaticTowerR.SetTextSize(10);
	m_StaticTowerG.SetTextSize(10);
	m_StaticTowerY.SetTextSize(10);
	m_StaticSwitchCover.SetTextSize(10);

	InitSpreadSheet();

	SetTimer(1, 500, NULL);

	if (g_IO.GetDriverType() != _DIO_TYPE_SIMUL_) {
		GetDlgItem(IDC_BUTTON_AUTO_STACKER_INITIAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_AUTO_STACKER_LOT_ONECYCLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_AUTO_STACKER_LOT_CLEANOUT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_OP_PANEL_LAMP_SW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_AUTO_STACKER_LOT_START)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_AUTO_STACKER_LOT_PAUSE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_AUTO_STACKER_LOT_RESUME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_AUTO_STACKER_START)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_AUTO_STACKER_STOP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_OP_PANEL_COVER_SW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_AUTO_STACKER_LOT_RETRY)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_AUTO_STACKER_LOT_SKIP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_AUTO_STACKER_AUTO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_AUTO_STACKER_MANUAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_OP_PANEL_SOUND_SW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_AUTO_STACKER_MANUAL3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_AUTO_STACKER_MANUAL4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_AUTO_STACKER_MANUAL5)->ShowWindow(SW_HIDE);
	
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgDebugAutoStacker::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(500);

	CBaseTaskManager::Finalize();
}

void CDlgDebugAutoStacker::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
		case 1:
			{
				KillTimer(1);
				UpdateStatus();
				SetTimer(1, 500, NULL);
			}break;
		case 500:
			{
				KillTimer(500);

				CString strMsg;
				SYSTEMTIME time;
				::GetLocalTime(&time);
				strMsg.Format("UPH[%04d-%02d-%02d %02d:%02d:%02d.%03d] - %d Ea"
					, time.wYear, time.wMonth, time.wDay
					, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds
					, g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt));

				UpdateEventMessage(strMsg);

				SetTimer(500, 600000, NULL);
			}break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgDebugAutoStacker::UpdateStatus()
{
	int nEqpCtrlMode = CBaseTaskManager::GetEqpCtrlMode();
	if (nEqpCtrlMode == eEqpCtrlMode_Auto) {
		((CButton*)GetDlgItem(IDC_CHECK_EQP_CTRL_MODE_AUTO))->SetCheck(1);
	}
	else {
		((CButton*)GetDlgItem(IDC_CHECK_EQP_CTRL_MODE_AUTO))->SetCheck(0);
	}


	int nLDTblLoadSideCtrlID[ MAX_LD_TBL_CNT ] = { IDC_CHECK_LD1_LD_SIDE,IDC_CHECK_LD2_LD_SIDE,IDC_CHECK_LD3_LD_SIDE,IDC_CHECK_LD4_LD_SIDE };
	int nLDTblTestSideCtrlID[ MAX_LD_TBL_CNT ] = { IDC_CHECK_LD1_TEST_SIDE,IDC_CHECK_LD2_TEST_SIDE,IDC_CHECK_LD3_TEST_SIDE,IDC_CHECK_LD4_TEST_SIDE };
	int nLDTblTestSide2DCtrlID[ MAX_LD_TBL_CNT ] = { IDC_CHECK_LD1_TEST_SIDE_2D,IDC_CHECK_LD2_TEST_SIDE_2D,IDC_CHECK_LD3_TEST_SIDE_2D,IDC_CHECK_LD4_TEST_SIDE_2D };
//	m_load
// 	int nLDTblTestSidePosCtrlID[MAX_LD_TBL_CNT] = { IDC_STATIC_LD1_TEST_SIDE1,IDC_STATIC_LD1_TEST_SIDE2,IDC_STATIC_LD1_TEST_SIDE3,IDC_STATIC_LD1_TEST_SIDE4 };




	for (int i = 0; i < MAX_LD_TBL_CNT; i++) {
		int nLoadTblLoadSidePos = g_TaskLoadTbl[i].ChkMotorPos(LOC_IDX_TBL_LD_SIDE, CTaskLoadTable::eAxisY );
		if( nLoadTblLoadSidePos == ERR_NO_ERROR )
		{
			((CButton*)GetDlgItem(nLDTblLoadSideCtrlID[i]))->SetCheck(1);
		}
		else{
			((CButton*)GetDlgItem(nLDTblLoadSideCtrlID[i]))->SetCheck(0);
		}

		int nLoadTblTestSide2DPos = g_TaskLoadTbl[i].ChkMotorPos(LOC_IDX_LD_TBL_TEST_SIDE_2D, CTaskLoadTable::eAxisY );
		if( nLoadTblTestSide2DPos == ERR_NO_ERROR )
		{
			((CButton*)GetDlgItem(nLDTblTestSide2DCtrlID[i]))->SetCheck(1);
		}
		else{
			((CButton*)GetDlgItem(nLDTblTestSide2DCtrlID[i]))->SetCheck(0);
		}	
	}


	int nLoadTblTestSiedePos = 0;
	for (int i = 0; i < MAX_LD_TBL_CNT; i++) {
		int nCount = 0;
		for (int nPos = 0; nPos < MAX_LD_TBL_CNT; nPos++)
		{
			nLoadTblTestSiedePos = g_TaskLoadTbl[i].ChkMotorPos(LOC_IDX_LD_TBL_TEST_SIDE_1 + nPos, CTaskLoadTable::eAxisY);
			if (nLoadTblTestSiedePos == ERR_NO_ERROR)
			{
				nCount++;
			}
			
			if (nCount > 0 )
			{
				((CButton*)GetDlgItem(nLDTblTestSideCtrlID[i]))->SetCheck(1);
			}
			else {
				((CButton*)GetDlgItem(nLDTblTestSideCtrlID[i]))->SetCheck(0);
			}
		}
	}

	//Load Table Test Site Pos
	for (int i = 0; i < MAX_LD_TBL_CNT; i++) 
	{
		for (int nPos = 0; nPos < LOC_IDX_LD_TBL_TEST_SIDE_4; nPos++)
		{
			int nLoadTblTestSitePos = g_TaskLoadTbl[i].ChkMotorPos(LOC_IDX_LD_TBL_TEST_SIDE_1+nPos, CTaskLoadTable::eAxisY);
			if (nLoadTblTestSitePos == ERR_NO_ERROR)
			{
				m_StaticTblTestSidePos[i][nPos].SetBackColor(COLOR_GREEN);			
			}
			else
			{
				m_StaticTblTestSidePos[i][nPos].SetBackColor(COLOR_DDARKGRAY);
			}
		}
	}



	for (int i = 0; i < MAX_LD_TBL_CNT; i++) {
		int nLoadTblLoadSidePos = g_TaskLoadTbl[i].ChkMotorPos(LOC_IDX_TBL_LD_SIDE, CTaskLoadTable::eAxisY);
		if (nLoadTblLoadSidePos == ERR_NO_ERROR)
		{
			m_StaticTblLoadSidePos[i].SetBackColor(COLOR_GREEN);
		}
		else
		{
			m_StaticTblLoadSidePos[i].SetBackColor(COLOR_DDARKGRAY);
		}
	}

	CBaseTaskManager* pConveyorTask[] = { &g_TaskAutoTransfer,&g_TaskAutoTrayUnloadPP };
	CStaticTrans* pStaticReserve[] = { &m_StaticReserve[0] , &m_StaticReserve[2] };
	for (int i = 0; i < _countof(pConveyorTask); i++) {
		if (pConveyorTask[i] == g_conveyor_work_area.get_reserved_task())
		{
			pStaticReserve[i]->SetBackColor(COLOR_GREEN);
		}		
		else
		{
			pStaticReserve[i]->SetBackColor(COLOR_DDARKGRAY);
		}
	}

	CBaseTaskManager* pTransfer_FeederTask[] = { &g_TaskAutoTransfer,&g_TaskAutoTrayFeeder };
	CStaticTrans* pStaticReserve2[] = { &m_StaticReserve[0] , &m_StaticReserve[1] };
	for (int i = 0; i < _countof(pTransfer_FeederTask); i++) {
		if (pTransfer_FeederTask[i] == g_Transfer_Feeder_work_area.get_reserved_task())
		{
			pStaticReserve2[i]->SetBackColor(COLOR_GREEN);
		}
		else
		{
			pStaticReserve2[i]->SetBackColor(COLOR_DDARKGRAY);
		}
	}

	// Press Unit
	int nAutoPressUnitCtrlID[eMaxPressUnitCount] = {
		IDC_STATIC_TASK_TESTSITE1_CMD_ID, IDC_STATIC_TASK_TESTSITE2_CMD_ID,IDC_STATIC_TASK_TESTSITE3_CMD_ID, IDC_STATIC_TASK_TESTSITE4_CMD_ID,
		IDC_STATIC_TASK_TESTSITE5_CMD_ID ,IDC_STATIC_TASK_TESTSITE6_CMD_ID ,IDC_STATIC_TASK_TESTSITE7_CMD_ID ,IDC_STATIC_TASK_TESTSITE8_CMD_ID };

	int nAutoPressUnitCmdStageID[ eMaxPressUnitCount ] = { 
		IDC_STATIC_TASK_TESTSITE1_STAGE, IDC_STATIC_TASK_TESTSITE2_STAGE,IDC_STATIC_TASK_TESTSITE3_STAGE, IDC_STATIC_TASK_TESTSITE4_STAGE,
		IDC_STATIC_TASK_TESTSITE5_STAGE ,IDC_STATIC_TASK_TESTSITE6_STAGE ,IDC_STATIC_TASK_TESTSITE7_STAGE ,IDC_STATIC_TASK_TESTSITE8_STAGE};

	for( int i = 0; i < eMaxPressUnitCount; i++ )
	{
		SetDlgItemText( nAutoPressUnitCtrlID[ i ], g_TaskPressUnit[ i ].GetActiveSeqCmdName() );
		SetDlgItemText( nAutoPressUnitCmdStageID[ i ], g_TaskAutoPress[ i ].GetLotStateToString() );
	}

	// Test PP
	int nAutoTestPPCtrlID[ MAX_TEST_PP_CNT ] = { IDC_STATIC_TASK_TESTPP1_CMD_ID, IDC_STATIC_TASK_TESTPP2_CMD_ID };
	int nAutoTestPpCmdStageID[ MAX_TEST_PP_CNT ] = { IDC_STATIC_TASK_TESTPP1_CMD_STAGE, IDC_STATIC_TASK_TESTPP2_CMD_STAGE };
	CString strAutoTestPpCmdStageID = _T("");
	for( int i = 0; i < MAX_TEST_PP_CNT; i++ )
	{
		SetDlgItemText( nAutoTestPPCtrlID[ i ], g_TaskTestPP[ i ].GetActiveSeqCmdName() );
		strAutoTestPpCmdStageID.Format("%s / step:%d / job:%d", g_TaskAutoTestPp[i].GetLotStateToString(), g_DMCont.m_dmTestPP.GN(NDM4_TestPP1_Auto_Excuting_Step + i), g_DMCont.m_dmTestPP.GN(NDM04_TESTHAND1_WORKING_IDX + i));
		SetDlgItemText( nAutoTestPpCmdStageID[ i ], strAutoTestPpCmdStageID);
	}

	// Load Tbl
	int nAutoLoadTblCtrlID[ MAX_LD_TBL_CNT ] = { IDC_STATIC_TASK_LOADTBL1_CMD_ID, IDC_STATIC_TASK_LOADTBL2_CMD_ID,IDC_STATIC_TASK_LOADTBL3_CMD_ID,IDC_STATIC_TASK_LOADTBL4_CMD_ID };
	int nAutoLoadTbleCmdStageId[ MAX_LD_TBL_CNT ] = { IDC_STATIC_TASK_LOADTBL1_CMD_STAGE, IDC_STATIC_TASK_LOADTBL2_CMD_STAGE,IDC_STATIC_TASK_LOADTBL3_CMD_STAGE,IDC_STATIC_TASK_LOADTBL4_CMD_STAGE };
	for( int i = 0; i < MAX_LD_TBL_CNT; i++ )
	{
		//int nLdTblCmdStageIdx = g_TaskAutoTestSite[i].m_nWorkingLoadTblCmdStage;
		SetDlgItemText( nAutoLoadTblCtrlID[ i ], g_TaskLoadTbl[ i ].GetActiveSeqCmdName() );
		SetDlgItemText( nAutoLoadTbleCmdStageId[ i ], g_TaskAutoLdTable[ i ].GetLotStateToString() );
	}

	// Tray PP

	BOOL tempdata = g_TaskTrayUnloadPP.get_TrayUnload_Y_Safety_sen(DEF_ON);
	((CButton*)GetDlgItem(IDC_CHECK_TRAYPP_Y_SAFETY_SEN))->SetCheck(g_TaskTrayUnloadPP.get_TrayUnload_Y_Safety_sen(DEF_ON));


	SetDlgItemText(IDC_STATIC_TASK_TRAY_LOADPP_CMD_ID, g_TaskTrayLoadPP.GetActiveSeqCmdName());
	SetDlgItemText(IDC_STATIC_TASK_TRAY_UNLOADPP_CMD_ID, g_TaskTrayUnloadPP.GetActiveSeqCmdName());
	
	SetDlgItemText(IDC_STATIC_TASK_TRAY_LOADPP_CMD_STAGE, g_TaskAutoTrayLoadPP.GetLotStateToString());
	SetDlgItemText(IDC_STATIC_TASK_TRAY_UNLOADPP_CMD_STAGE, g_TaskAutoTrayUnloadPP.GetLotStateToString());

	// Transfer Cmd
	SetDlgItemText( IDC_STATIC_TASK_TRANSFER_CMD_ID, g_TaskTransfer.GetActiveSeqCmdName() );
	SetDlgItemText( IDC_STATIC_TASK_TRANSFER_CMD_STAGE, g_TaskAutoTransfer.GetLotStateToString() );
	( ( CButton* )GetDlgItem( IDC_CHECK_TRANSFER_EXIST_SEN ) )->SetCheck( g_TaskTransfer.GetDI( CTaskTransfer::xTRANSFER_TRAY_EXIST_SEN) );

	int nTransferData = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_Transfer);
	((CButton*)GetDlgItem(IDC_CHECK_TRANSFER_EXIST_DATA))->SetCheck(nTransferData);

	CString Transfer_State = _T( "" );
	if( g_TaskTransfer.get_cyl_UpDown( DEF_UP ) == ERR_NO_ERROR ) {
		Transfer_State = _T( "UP" );
	}else if( g_TaskTransfer.get_cyl_UpDown( DEF_DOWN ) == ERR_NO_ERROR ){
		Transfer_State = _T( "DOWN" );
	}else {
		Transfer_State = _T( "" );
	}
	SetDlgItemText( IDC_STATIC_TASK_TRANSFER_UP, Transfer_State );

	if( g_TaskTransfer.get_cyl_ClampUnclamp( DEF_CLAMP ) == ERR_NO_ERROR ) {
		Transfer_State = _T( "CLAMP" );
	}
	else if( g_TaskTransfer.get_cyl_ClampUnclamp( DEF_UNCLAMP ) == ERR_NO_ERROR ){
		Transfer_State = _T( "UNCLAMP" );
	}else {
		Transfer_State = _T( "" );
	}
	SetDlgItemText( IDC_STATIC_TASK_TRANSFER_CLAMP, Transfer_State );


	// Tray Feeder Cmd
	SetDlgItemText(IDC_STATIC_TASK_TRAY_FEEDER_CMD_ID, g_TaskTrayFeeder.GetActiveSeqCmdName());
	SetDlgItemText(IDC_STATIC_TASK_TRAY_FEEDER_CMD_STAGE, g_TaskAutoTrayFeeder.GetLotStateToString());
	((CButton*)GetDlgItem(IDC_CHECK_TRAY_FEEDER_EXIST_DATA))->SetCheck(g_TaskTrayFeeder.GetTrayExist_Data());

	BOOL bExistSen = g_TaskTrayFeeder.GetTrayExist_TransferArea_Sen(eExist) == ERR_NO_ERROR;
	((CButton*)GetDlgItem(IDC_CHECK_TRANSFER_AREA_TRAY_EXIST_SEN))->SetCheck(bExistSen);

	BOOL bExistData = g_TaskTrayFeeder.GetTrayExist_TransferArea_Data();
	((CButton*)GetDlgItem(IDC_CHECK_TRANSFER_AREA_TRAY_EXIST_DATA))->SetCheck(bExistData);

	CString Tray_Feeder_State = _T("");
	if (g_TaskTrayFeeder.get_cyl_UpDown(eUp) == ERR_NO_ERROR) {
		Tray_Feeder_State = _T("UP");
	}
	else if (g_TaskTrayFeeder.get_cyl_UpDown(eDown) == ERR_NO_ERROR) {
		Tray_Feeder_State = _T("DOWN");
	}
	else {
		Tray_Feeder_State = _T("");
	}
	SetDlgItemText(IDC_STATIC_TASK_TRAY_FEEDER_UP, Tray_Feeder_State);

	if (g_TaskTrayFeeder.get_cyl_ClampUnclamp(eClamp) == ERR_NO_ERROR) {
		Tray_Feeder_State = _T("CLAMP");
	}
	else if (g_TaskTransfer.get_cyl_ClampUnclamp(eUnclamp) == ERR_NO_ERROR) {
		Tray_Feeder_State = _T("UNCLAMP");
	}
	else {
		Tray_Feeder_State = _T("");
	}
	SetDlgItemText(IDC_STATIC_TASK_TRAY_FEEDER_CLAMP, Tray_Feeder_State);

	CString tray_row_step = _T("");;
	tray_row_step.Format("Step : %d", g_TaskTrayFeeder.GetTrayDeviceExistRowStep());
	SetDlgItemText(IDC_STATIC_TASK_TRAY_FEEDER_ROW, tray_row_step);

	// Stacker
	int nAutoStackerCtrlID[ eMaxStackerCount ] = {
		IDC_STATIC_AUTO_STACKER_CMD_ID_1, IDC_STATIC_AUTO_STACKER_CMD_ID_2,IDC_STATIC_AUTO_STACKER_CMD_ID_3,IDC_STATIC_AUTO_STACKER_CMD_ID_4,
		IDC_STATIC_AUTO_STACKER_CMD_ID_5, IDC_STATIC_AUTO_STACKER_CMD_ID_6,IDC_STATIC_AUTO_STACKER_CMD_ID_7 };
	int nAutoStackerCmdStageID[ eMaxStackerCount ] = {
		IDC_STATIC_AUTO_STACKER_STATE_1,IDC_STATIC_AUTO_STACKER_STATE_2,IDC_STATIC_AUTO_STACKER_STATE_3,IDC_STATIC_AUTO_STACKER_STATE_4,
		IDC_STATIC_AUTO_STACKER_STATE_5,IDC_STATIC_AUTO_STACKER_STATE_6,IDC_STATIC_AUTO_STACKER_STATE_7 };

	// 임시 Reserve 
	//CString sLdMsg;
	//int nLdTblWorkReserveID[MAX_LD_TBL_CNT] = {IDC_STATIC_TASK_LOADTBL1_RESERVE,IDC_STATIC_TASK_LOADTBL2_RESERVE, IDC_STATIC_TASK_LOADTBL3_RESERVE,IDC_STATIC_TASK_LOADTBL4_RESERVE};
	//for( int i=0; i < MAX_LD_TBL_CNT; i++ )
	//{
	//	int nLdTblReserve = g_TaskLoadTbl[i].GetWorkReserve();
	//	sLdMsg.Format("%s(%d)", GetSeqLoadTblReserveStr(nLdTblReserve), nLdTblReserve);
	//	SetDlgItemText(nLdTblWorkReserveID[i], sLdMsg);
	//}

	// Stacker Full
	// Stacker Exist
	// Front Conveyor Exist
	// Rear Conveyor Exist
	// Front Conveyor Data
	// Read Conveyor Data
	int Sen_State = 0, check = 0;
	BOOL bEmptyConveyor = FALSE;
	for( int i = 0; i < eMaxStackerCount; i++ ) {

		SetDlgItemText(nAutoStackerCtrlID[i], g_TaskStacker[i].GetActiveSeqCmdName());
		SetDlgItemText(nAutoStackerCmdStageID[i], g_TaskAutoStacker[i].GetLotStateToString());

		check = g_TaskStacker[i].GetFullTray();
		((CButton*)GetDlgItem(m_nTrayFullCtrlId[i]))->SetCheck(check);

		((CButton*)GetDlgItem(m_nTrayExistCtrlId[i]))->SetCheck(!g_TaskStacker[i].GetEmptyTray());

		g_TaskStacker[ i ].GetExistTray_RearPos_Sen( DEF_EXIST ) == ERR_NO_ERROR ? Sen_State = DEF_ON : Sen_State = DEF_OFF;
		( ( CButton* )GetDlgItem( m_nRearConveyorExistCtrlId[ i ] ) )->SetCheck( Sen_State );

		g_TaskStacker[i].GetExistTray_FrontPos_Sen(DEF_EXIST) == ERR_NO_ERROR ? Sen_State = DEF_ON : Sen_State = DEF_OFF;
		((CButton*)GetDlgItem(m_nFrontConveyorExistCtrlId[i]))->SetCheck(Sen_State);

		bEmptyConveyor = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_CONV_1 + i);
		((CButton*)GetDlgItem(m_nRearConveyorDataCtrlId[i]))->SetCheck(bEmptyConveyor);

		bEmptyConveyor = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_BTM_1 + i);
		((CButton*)GetDlgItem(m_nFrontConveyorDataCtrlId[i]))->SetCheck(bEmptyConveyor);
	}

	if (!g_COVER_SW.GetStatus()) {
		m_StaticSwitchCover.SetBackColor(COLOR_YELLOW);
	}
	else {
		m_StaticSwitchCover.SetBackColor(COLOR_DDARKGRAY);
	}

	if (g_IO.out(DO_TOWER_RED) == DEF_ON) {
		m_StaticTowerR.SetBackColor(COLOR_RED);
	}
	else {
		m_StaticTowerR.SetBackColor(COLOR_DDARKGRAY);
	}	

	if (g_IO.out(DO_TOWER_GREEN) == DEF_ON) {
		m_StaticTowerG.SetBackColor(COLOR_GREEN);
	}
	else {
		m_StaticTowerG.SetBackColor(COLOR_DDARKGRAY);
	}

	if (g_IO.out(DO_TOWER_YELLOW) == DEF_ON) {
		m_StaticTowerY.SetBackColor(COLOR_YELLOW);
	}
	else {
		m_StaticTowerY.SetBackColor(COLOR_DDARKGRAY);
	}

	UpdateTrayDataExist();
}

void CDlgDebugAutoStacker::UpdateTrayDataExist()
{
	
	std::vector<int> vPressCtrlID; //Press 컨트롤 ID 추가할 것.
	vPressCtrlID.push_back( IDC_SPREAD_TEST_SITE1 );
	vPressCtrlID.push_back( IDC_SPREAD_TEST_SITE2 );
	vPressCtrlID.push_back( IDC_SPREAD_TEST_SITE3 );
	vPressCtrlID.push_back( IDC_SPREAD_TEST_SITE4 );
	vPressCtrlID.push_back( IDC_SPREAD_TEST_SITE5 );
	vPressCtrlID.push_back( IDC_SPREAD_TEST_SITE6 );
	vPressCtrlID.push_back( IDC_SPREAD_TEST_SITE7 );
	vPressCtrlID.push_back( IDC_SPREAD_TEST_SITE8 );

	std::vector<int> vPressDataStageIdx;
	vPressDataStageIdx.push_back( eDeviceStage_TEST_SITE_1 );
	vPressDataStageIdx.push_back( eDeviceStage_TEST_SITE_2 );
	vPressDataStageIdx.push_back( eDeviceStage_TEST_SITE_3 );
	vPressDataStageIdx.push_back( eDeviceStage_TEST_SITE_4 );
	vPressDataStageIdx.push_back( eDeviceStage_TEST_SITE_5 );
	vPressDataStageIdx.push_back( eDeviceStage_TEST_SITE_6 );
	vPressDataStageIdx.push_back( eDeviceStage_TEST_SITE_7 );
	vPressDataStageIdx.push_back( eDeviceStage_TEST_SITE_8 );

	//Test Unit Data
	int nTest_Site_Col = g_DMCont.m_dmTestPP.GN( NDM4_TestSite_Div_X );
	int nTest_Site_Row = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	for (std::vector<int>::size_type i = 0; i < vPressCtrlID.size(); i++)
	{
		SetDisplayPressStageInfo(vPressCtrlID[i], i, vPressDataStageIdx[i], nTest_Site_Col, nTest_Site_Row);
	}
	


	std::vector<int> vTestPpCtrlID_L; //Test PP 컨트롤 추가할 것.
	vTestPpCtrlID_L.push_back(IDC_SPREAD_TEST_PP1_L);
	vTestPpCtrlID_L.push_back(IDC_SPREAD_TEST_PP2_L);

	std::vector<int> vTestPpDataStageIdx;
	vTestPpDataStageIdx.push_back(eDeviceStage_TEST_PP_1);
	vTestPpDataStageIdx.push_back(eDeviceStage_TEST_PP_2);

	//Test PP Data _L
	for (std::vector<int>::size_type i = 0; i < vTestPpCtrlID_L.size(); i++)
	{
		SetDisplayStageInfo(vTestPpCtrlID_L[i], vTestPpDataStageIdx[i], MAX_TEST_PP_PKR_CNT_X, MAX_TEST_PP_PKR_CNT_Y );
	}

	std::vector<int> vTestPpCtrlID_U; //Test PP 컨트롤 추가할 것.
	vTestPpCtrlID_U.push_back(IDC_SPREAD_TEST_PP1_U);
	vTestPpCtrlID_U.push_back(IDC_SPREAD_TEST_PP2_U);

	//Test PP Data _U
	for (std::vector<int>::size_type i = 0; i < vTestPpCtrlID_U.size(); i++)
	{
		SetDisplayStageInfo(vTestPpCtrlID_U[i], vTestPpDataStageIdx[i], MAX_TEST_PP_PKR_CNT_X, MAX_TEST_PP_PKR_CNT_Y, TEST_UD_PKR_Y);
	}

	std::vector<int> vSOT_EOT; 
	vSOT_EOT.push_back(IDC_SPREAD_SOT_EOT_MODULE1);
	vSOT_EOT.push_back(IDC_SPREAD_SOT_EOT_MODULE2);

	for (std::vector<int>::size_type i = 0; i < vSOT_EOT.size(); i++)
	{
		SetDisplaySOTEOTInfo(vSOT_EOT[i], STATION_MAX_Y_SITE, STATION_MAX_X_SITE, i);
	}
	//SOT EOT



// 	std::vector<int> vARTTblSpreadCtrlID;
// 	vARTTblSpreadCtrlID.push_back( IDC_SPREAD_ART_TBL1 );
// 	vARTTblSpreadCtrlID.push_back( IDC_SPREAD_ART_TBL2 );

// 	std::vector<int> vARTTblDataStageIdx;
// 
// 	vARTTblDataStageIdx.push_back(eDeviceStage_TEST_PP_1_ART_BUFF);
// 	vARTTblDataStageIdx.push_back(eDeviceStage_TEST_PP_2_ART_BUFF);
// 
// 	//ART Data
// 	int nArtBuffCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestBuffTbl_Div_X);
// 	int nArtBuffCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestBuffTbl_Div_Y);
// 	for (std::vector<int>::size_type i = 0; i < vARTTblSpreadCtrlID.size(); i++)
// 	{
// 		SetDisplayStageInfo(vARTTblSpreadCtrlID[i], vARTTblDataStageIdx[i], nArtBuffCntX, nArtBuffCntY);
// 	}

	std::vector<int> vCleanTblSpreadCtrlID;
	vCleanTblSpreadCtrlID.push_back( IDC_SPREAD_CLEAN_TBL1 );
	vCleanTblSpreadCtrlID.push_back( IDC_SPREAD_CLEAN_TBL2 );

	std::vector<int> vCleanTblDataStageIdx;

	vCleanTblDataStageIdx.push_back(eDeviceStage_TEST_PP_1_CLEAN_BUFF);
	vCleanTblDataStageIdx.push_back(eDeviceStage_TEST_PP_2_CLEAN_BUFF);

	//Clean Data
	int nCleanBuffCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_X);
	int nCleanBuffCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_Y);
	for (std::vector<int>::size_type i = 0; i < vCleanTblSpreadCtrlID.size(); i++)
	{
		SetDisplayStageInfo(vCleanTblSpreadCtrlID[i], vCleanTblDataStageIdx[i], nCleanBuffCntX, nCleanBuffCntY);
	}

	std::vector<int> vLoadTblCtrlID; //Load Table 컨트롤 추가할 것.
	vLoadTblCtrlID.push_back( IDC_SPREAD_LOAD_TBL1 );
	vLoadTblCtrlID.push_back( IDC_SPREAD_LOAD_TBL2 );
	vLoadTblCtrlID.push_back(IDC_SPREAD_LOAD_TBL3 );
	vLoadTblCtrlID.push_back(IDC_SPREAD_LOAD_TBL4 );
	std::vector<int> vLoadTblDataStageIdx;
	vLoadTblDataStageIdx.push_back( eDeviceStage_LD_TBL_1 );
	vLoadTblDataStageIdx.push_back( eDeviceStage_LD_TBL_2 );
	vLoadTblDataStageIdx.push_back(eDeviceStage_LD_TBL_3);
	vLoadTblDataStageIdx.push_back(eDeviceStage_LD_TBL_4);

	//Load Table Data
	int nLoadTbl_Div_X = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Div_X );
	int nLoadTbl_Div_Y = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Div_Y );
	for( std::vector<int>::size_type i = 0; i < vLoadTblCtrlID.size(); i++ )
	{
		SetDisplayStageInfo( vLoadTblCtrlID[ i ], vLoadTblDataStageIdx[ i ], nLoadTbl_Div_X, nLoadTbl_Div_Y );
	}

	// Tray PP Data
	SetDisplayStageInfo(IDC_SPREAD_TRAY_LOAD_PP, eDeviceStage_TRAY_PP_1, MAX_TRAY_PP_CNT_X, MAX_TRAY_PP_CNT_Y );
	SetDisplayStageInfo(IDC_SPREAD_TRAY_UNLOAD_PP, eDeviceStage_TRAY_PP_2, MAX_TRAY_PP_CNT_X, MAX_TRAY_PP_CNT_Y);

}

void CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerInitial()
{
	eAutoLotCmd eLotCmd = eAutoLotCmd_Initial;
	
	g_TaskAllAutoStacker.OnLotCommand(eLotCmd, NULL, NULL);
	
	for(int i=0; i < eMaxTestPPCount; i++ )
	{
		g_TaskAutoTestPp[eTestPP_1+i].OnLotCommand(eLotCmd, NULL, NULL);	
	}

	for (int i = 0; i < eMaxLoadTblCount; i++)
	{
		g_TaskAutoLdTable[eLoadTbl_1 + i].OnLotCommand(eLotCmd, NULL, NULL);
	}

	for(int i=0; i < eMaxPressUnitCount; i++ )
		g_TaskAutoPress[ePressUnit_Down_1+i].OnLotCommand(eLotCmd, NULL, NULL);

	g_TaskAutoTrayLoadPP.OnLotCommand(eLotCmd, NULL, NULL);
	g_TaskAutoTrayUnloadPP.OnLotCommand(eLotCmd, NULL, NULL);
}


void CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerLotStart()
{
	eAutoLotCmd eLotCmd = eAutoLotCmd_Start;
	
	g_TaskAllAutoStacker.OnLotCommand(eLotCmd, NULL, NULL);
	
	for(int i=0; i < eMaxTestPPCount; i++ )
	{
		g_TaskAutoTestPp[eTestPP_1+i].OnLotCommand(eLotCmd, NULL, NULL);
		g_TaskAutoLdTable[eLoadTbl_1+i].OnLotCommand(eLotCmd, NULL, NULL);
	}
	for(int i=0; i < eMaxPressUnitCount; i++ )
		g_TaskAutoPress[ePressUnit_Down_1+i].OnLotCommand(eLotCmd, NULL, NULL);

	g_TaskAutoTrayLoadPP.OnLotCommand(eLotCmd, NULL, NULL);
	g_TaskAutoTrayUnloadPP.OnLotCommand(eLotCmd, NULL, NULL);

	g_DMCont.m_dmEQP.SN(NDM0_LotStatus, eAutoLotState_Executing);
}


void CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerLotPause()
{
	eAutoLotCmd eLotCmd = eAutoLotCmd_Pause;
	g_TaskAllAutoStacker.OnLotCommand(eLotCmd, NULL, NULL);
//	g_TaskAutoTestSite[eTestPP_1].OnLotCommand(eLotCmd, NULL, NULL);
//	g_TaskAutoTestSite[eTestPP_2].OnLotCommand(eLotCmd, NULL, NULL);
//	g_TaskAutoTestSite[eTestPP_3].OnLotCommand(eLotCmd, NULL, NULL);
//	g_TaskAutoTestSite[eTestPP_4].OnLotCommand(eLotCmd, NULL, NULL);
	
	g_TaskAutoTrayLoadPP.OnLotCommand(eLotCmd, NULL, NULL);
	g_TaskAutoTrayUnloadPP.OnLotCommand(eLotCmd, NULL, NULL);
}


void CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerLotResume()
{
	eAutoLotCmd eLotCmd = eAutoLotCmd_Resume;
	g_TaskAllAutoStacker.OnLotCommand(eLotCmd, NULL, NULL);
//	g_TaskAutoTestSite[eTestPP_1].OnLotCommand(eLotCmd, NULL, NULL);
//	g_TaskAutoTestSite[eTestPP_2].OnLotCommand(eLotCmd, NULL, NULL);
//	g_TaskAutoTestSite[eTestPP_3].OnLotCommand(eLotCmd, NULL, NULL);
//	g_TaskAutoTestSite[eTestPP_4].OnLotCommand(eLotCmd, NULL, NULL);
	
	g_TaskAutoTrayLoadPP.OnLotCommand(eLotCmd, NULL, NULL);
	g_TaskAutoTrayUnloadPP.OnLotCommand(eLotCmd, NULL, NULL);
}


void CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerLotOnecycle()
{
	// 미사용
	//if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse) == TRUE)
	//{
	//	g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AutoGrr_OneCycle, NULL, NULL);
	//}
	//else
	//{
	//	int  nLotCtrlMode = g_DMCont.m_dmEQP.GN(NDM0_LotStatus);
	//	if (nLotCtrlMode == eAutoLotState_QaSample)
	//	{
	//		g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_QaSample_OneCycle, NULL, NULL);
	//	}
	//	else {
	//		g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_OneCycle, NULL, NULL);
	//	}
	//}
}


void CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerLotRetry()
{
	eAutoLotCmd eLotCmd = eAutoLotCmd_Retry;
	g_TaskSystemCtrl.OnLotCommand(eLotCmd, NULL, NULL);
}


void CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerLotSkip()
{
	eAutoLotCmd eLotCmd = eAutoLotCmd_Skip;
	g_TaskSystemCtrl.OnLotCommand(eLotCmd, NULL, NULL);
}


void CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerLotCleanout()
{
	eAutoLotCmd eLotCmd = eAutoLotCmd_CleanOut;
	
	g_TaskAllAutoStacker.OnLotCommand( eLotCmd, NULL, NULL );
	
	for(int i=0; i < eMaxTestPPCount; i++ )
	{
		g_TaskAutoTestPp[eTestPP_1+i].OnLotCommand(eLotCmd, NULL, NULL);	
	}
	for (int i = 0; i < eMaxLoadTblCount; i++)
	{
		g_TaskAutoLdTable[eLoadTbl_1 + i].OnLotCommand(eLotCmd, NULL, NULL);
	}

	for(int i=0; i < eMaxPressUnitCount; i++ )
		g_TaskAutoPress[ePressUnit_Down_1+i].OnLotCommand(eLotCmd, NULL, NULL);

	g_TaskAutoTrayLoadPP.OnLotCommand(eLotCmd, NULL, NULL);
	g_TaskAutoTrayUnloadPP.OnLotCommand(eLotCmd, NULL, NULL);
}


void CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerAuto()
{
	g_Motor.Destroy();

	char szDriverFileName[MAX_PATH] = { 0, };
	char szTemp[512] = { 0, };
	::GetCurrentDirectory(sizeof(szTemp), szTemp);
	sprintf_s(szDriverFileName, sizeof(szDriverFileName), "%s\\sys_data\\%s", szTemp, SZ_DRIVER_FILE_NAME);
	g_SoftServoCtrl.CreateDev(szDriverFileName, MAX_PATH);

	g_Motor.Create(szDriverFileName, strlen(szDriverFileName));


	//CBaseTaskManager::SetEqpCtrlMode(eEqpCtrlMode_Auto);
}


void CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerManual()
{
	CBaseTaskManager::SetEqpCtrlMode(eEqpCtrlMode_Manual);
}


void CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerStart()
{
// 	BOOL bImpossibleAlarm = g_Error.IsImpossibleStartAlarm();
// 	if (bImpossibleAlarm != TRUE)
// 	{
// 		g_TaskSystemCtrl.OnPanelButtonStart();
// 	}
g_START_SW.m_nOperationStep = 1000;

}


void CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerStop()
{
	g_TaskSystemCtrl.OnPanelButtonStop();
}


void CDlgDebugAutoStacker::OnSimulationInputsimulationvalue()
{
	char szValue_New[128] = { 0, };
	char szValue_Old[128] = { 0, };
	char szTitle[128] = { 0, };
	char szMin[128] = { 0, };
	char szMax[128] = { 0, };

	ST_DD_DEVICE pDev;//= new ST_DD_DEVICE;
	ST_DD_DEVICE pNoDev;//= new ST_DD_DEVICE;

	int nLoadTblCtrlId[4] = {
		IDC_STATIC_TASK_LOADTBL1_EVT, IDC_STATIC_TASK_LOADTBL2_EVT, IDC_STATIC_TASK_LOADTBL3_EVT, IDC_STATIC_TASK_LOADTBL4_EVT
	};

	CWnd* pWndLoadTbl[4];
	for (int i = 0; i < 4; i++) {
		pWndLoadTbl[i] = (CWnd*)GetDlgItem(nLoadTblCtrlId[i]);
	}

	for (int i = 0; i < 4; i++) {
		if (m_pInputValueTargetWnd == pWndLoadTbl[i])
		{
			sprintf_s(szValue_Old, sizeof(szValue_Old), "030");
			sprintf_s(szMax, sizeof(szMax), "90");
			sprintf_s(szMin, sizeof(szMin), "0");

			int nRet = g_LibCommCtrl.GetNumberBox(this, szValue_New, sizeof(szValue_New), szValue_Old, szTitle, szMax, szMin);
			if (nRet == RETURN_OK) {

				for (int y = 0; y < 2; y++)
				{
					for (int x = 0; x < 2; x++)
					{
						g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1 + i, x, y, &pDev);
						pDev.szBinResult[0] = '1';
						pDev.szBinResult[1] = '\0';
						g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_LD_TBL_1 + i, x + 2, y, &pDev);
						g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_LD_TBL_1 + i, x, y, &pNoDev);
					}
				}
				g_TaskLoadTbl[i].SetWorkReserve(atoi(szValue_New));
			}
			return;
		}
	}

	sprintf_s(szValue_Old, sizeof(szValue_Old), "030");
	sprintf_s(szMax, sizeof(szMax), "30");
	sprintf_s(szMin, sizeof(szMin), "0");

	int nRet = g_LibCommCtrl.GetNumberBox(this, szValue_New, sizeof(szValue_New), szValue_Old, szTitle, szMax, szMin);
	if (nRet == RETURN_OK) {
		m_pInputValueTargetWnd->SetWindowText(szValue_New);
	}
}


void CDlgDebugAutoStacker::OnContextMenu(CWnd* pWnd, CPoint ptMousePos)
{
	// if Shift-F10
	if (ptMousePos.x == -1 && ptMousePos.y == -1)
	{
		ptMousePos = (CPoint)GetMessagePos();
	}
	ScreenToClient(&ptMousePos);

	CMenu menu;
	CMenu* pPopup;
	m_nOnOffDi_Idx.clear();

	CWnd* pWndTrayExist[eMaxStackerCount];
	for (int i = 0; i < eMaxStackerCount; i++) {
		pWndTrayExist[i] = (CWnd*)GetDlgItem(m_nTrayExistCtrlId[i]);
	}

	CWnd* pWndTrayFull[eMaxStackerCount];
	for (int i = 0; i < eMaxStackerCount; i++) {
		pWndTrayFull[i] = (CWnd*)GetDlgItem(m_nTrayFullCtrlId[i]);
	}

	CWnd* pWndFrontConveyorExist[ eMaxStackerCount ];
	for( int i = 0; i < eMaxStackerCount; i++ ) {
		pWndFrontConveyorExist[ i ] = ( CWnd* )GetDlgItem( m_nFrontConveyorExistCtrlId[ i ] );
	}

	CWnd* pWndRearConveyorExist[ eMaxStackerCount ];
	for( int i = 0; i < eMaxStackerCount; i++ ) {
		pWndRearConveyorExist[ i ] = ( CWnd* )GetDlgItem( m_nRearConveyorExistCtrlId[ i ] );
	}

	CWnd* pWndRearTraySafety = ( CWnd* )GetDlgItem( IDC_CHECK_TRAYPP_Y_SAFETY_SEN );

	CWnd* pWndRearTransferSafety = ( CWnd* )GetDlgItem( IDC_CHECK_TRANSFER_X_SAFETY_SEN );
	CWnd* pWndRearTransferExist = ( CWnd* )GetDlgItem( IDC_CHECK_TRANSFER_EXIST_SEN );

	CWnd* pWndLoadStackerTransferAreaTrayExist = (CWnd*)GetDlgItem(IDC_CHECK_TRANSFER_AREA_TRAY_EXIST_SEN);


	m_pInputValueTargetWnd = NULL;
	BOOL bUsableCtrl = FALSE;
	int nPos = 0;

	for (int i = 0; i < eMaxStackerCount; i++)
	{
		if (pWnd == pWndTrayExist[i])
		{
			nPos = 4;
			m_nOnOffDi_Idx.push_back(g_TaskStacker[i].m_vX[ CTaskStacker::xC_TRAY_STACKER_EXIST_SEN ]);
			menu.LoadMenu(IDR_MAINFRAME);
			pPopup = menu.GetSubMenu(nPos);
			pPopup->EnableMenuItem(ID_SIMULATION_INPUTSIMULATIONVALUE, MF_DISABLED);
			ClientToScreen(&ptMousePos);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN, ptMousePos.x, ptMousePos.y, this);
			break;
		}
	}
	for (int i = 0; i < eMaxStackerCount; i++)
	{
		if (pWnd == pWndTrayFull[i])
		{
			nPos = 4;
			m_nOnOffDi_Idx.push_back(g_TaskStacker[ i ].m_vX[ CTaskStacker::xC_TRAY_STACKER_FULL_SEN ]);// .ChkDI( CTaskStacker::xC_TRAY_STACKER_FULL_SEN, TRUE );
			menu.LoadMenu(IDR_MAINFRAME);
			pPopup = menu.GetSubMenu(nPos);
			pPopup->EnableMenuItem(ID_SIMULATION_INPUTSIMULATIONVALUE, MF_DISABLED);
			ClientToScreen(&ptMousePos);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN, ptMousePos.x, ptMousePos.y, this);
			break;
		}
	}

	for( int i = 0; i < eMaxStackerCount; i++ )
	{
		if( pWnd == pWndFrontConveyorExist[ i ] )
		{
			nPos = 4;
			m_nOnOffDi_Idx.push_back( g_TaskStacker[ i ].m_vX[ CTaskStacker::xC_TRAY_CV_FRONT_EXIST_SEN ] );
			m_nOnOffDi_Idx.push_back( g_TaskStacker[ i ].m_vX[ CTaskStacker::xC_TRAY_CV_FRONT_SLOW_SEN ] );
			menu.LoadMenu( IDR_MAINFRAME );
			pPopup = menu.GetSubMenu( nPos );
			pPopup->EnableMenuItem( ID_SIMULATION_INPUTSIMULATIONVALUE, MF_DISABLED );
			ClientToScreen( &ptMousePos );
			pPopup->TrackPopupMenu( TPM_LEFTALIGN, ptMousePos.x, ptMousePos.y, this );
			break;
		}
	}

	for( int i = 0; i < eMaxStackerCount; i++ )
	{
		if( pWnd == pWndRearConveyorExist[ i ] )
		{
			nPos = 4;
			m_nOnOffDi_Idx.push_back(g_TaskStacker[ i ].m_vX[ CTaskStacker::xC_TRAY_CV_REAR_EXIST_SEN ]);
			m_nOnOffDi_Idx.push_back( g_TaskStacker[ i ].m_vX[ CTaskStacker::xC_TRAY_CV_REAR_SLOW_SEN ] );

			menu.LoadMenu( IDR_MAINFRAME );
			pPopup = menu.GetSubMenu( nPos );
			pPopup->EnableMenuItem( ID_SIMULATION_INPUTSIMULATIONVALUE, MF_DISABLED );
			ClientToScreen( &ptMousePos );
			pPopup->TrackPopupMenu( TPM_LEFTALIGN, ptMousePos.x, ptMousePos.y, this );
			break;
		}
	}

	if( pWnd == pWndRearTraySafety )
	{
// 		nPos = 4;
// 		m_nOnOffDi_Idx.push_back( g_TaskTrayUnloadPP.m_vX[ CTaskTrayPP::xC_TRAY_HAND_Y_SAFETY_AREA_SEN ] );
// 		menu.LoadMenu( IDR_MAINFRAME );
// 		pPopup = menu.GetSubMenu( nPos );
// 		pPopup->EnableMenuItem( ID_SIMULATION_INPUTSIMULATIONVALUE, MF_DISABLED );
// 		ClientToScreen( &ptMousePos );
// 		pPopup->TrackPopupMenu( TPM_LEFTALIGN, ptMousePos.x, ptMousePos.y, this );
	}

	if( pWnd == pWndRearTransferSafety )
	{
		nPos = 4;
		// m_nOnOffDi_Idx.push_back( g_TaskTransfer.m_vX[ CTaskTransfer::xTRANSFER_X_SAFETY_AREA_SEN ] );
		menu.LoadMenu( IDR_MAINFRAME );
		pPopup = menu.GetSubMenu( nPos );
		pPopup->EnableMenuItem( ID_SIMULATION_INPUTSIMULATIONVALUE, MF_DISABLED );
		ClientToScreen( &ptMousePos );
		pPopup->TrackPopupMenu( TPM_LEFTALIGN, ptMousePos.x, ptMousePos.y, this );
	}

	if( pWnd == pWndRearTransferExist )
	{ 
		nPos = 4;
		m_nOnOffDi_Idx.push_back( g_TaskTransfer.m_vX[ CTaskTransfer::xTRANSFER_TRAY_EXIST_SEN ] );
		menu.LoadMenu( IDR_MAINFRAME );
		pPopup = menu.GetSubMenu( nPos );
		pPopup->EnableMenuItem( ID_SIMULATION_INPUTSIMULATIONVALUE, MF_DISABLED );
		ClientToScreen( &ptMousePos );
		pPopup->TrackPopupMenu( TPM_LEFTALIGN, ptMousePos.x, ptMousePos.y, this );
	}

	if (pWnd == pWndLoadStackerTransferAreaTrayExist)
	{
		nPos = 4;
		m_nOnOffDi_Idx.push_back(g_TaskTrayFeeder.GetDiExtNum(CTaskTrayFeeder::xTRAY_FEEDER_TRANSFER_AREA_TRAY_DETECT_SEN));
		menu.LoadMenu(IDR_MAINFRAME);
		pPopup = menu.GetSubMenu(nPos);
		pPopup->EnableMenuItem(ID_SIMULATION_INPUTSIMULATIONVALUE, MF_DISABLED);
		ClientToScreen(&ptMousePos);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN, ptMousePos.x, ptMousePos.y, this);
	}
}


void CDlgDebugAutoStacker::OnSimulationOn()
{
	for( int i = 0; i < (int)m_nOnOffDi_Idx.size(); i++ ) {
		g_IO.SetSimulDi( m_nOnOffDi_Idx[i], DEF_ON );
	}
	
}


void CDlgDebugAutoStacker::OnSimulationOff()
{
	for( int i = 0; i < (int)m_nOnOffDi_Idx.size(); i++ ) {
		g_IO.SetSimulDi( m_nOnOffDi_Idx[i], DEF_OFF );
	}
}


void CDlgDebugAutoStacker::OnSimulationConveyorRearAllOn()
{
	std::vector<int> OnOffDi_Idx;
	for( int i = 0; i < eMaxStackerCount; i++ ) {
		OnOffDi_Idx.push_back( g_TaskStacker[ i ].m_vX[ CTaskStacker::xC_TRAY_CV_REAR_EXIST_SEN ] );
		OnOffDi_Idx.push_back( g_TaskStacker[ i ].m_vX[ CTaskStacker::xC_TRAY_CV_REAR_SLOW_SEN ] );
	}

	for( int i = 0; i < ( int )OnOffDi_Idx.size(); i++ ) {
		g_IO.SetSimulDi( OnOffDi_Idx[ i ], DEF_ON );
	}
}


void CDlgDebugAutoStacker::OnSimulationConveyorRearAllOff()
{
	std::vector<int> OnOffDi_Idx;
	for( int i = 0; i < eMaxStackerCount; i++ ) {
		OnOffDi_Idx.push_back( g_TaskStacker[ i ].m_vX[ CTaskStacker::xC_TRAY_CV_REAR_EXIST_SEN ] );
		OnOffDi_Idx.push_back( g_TaskStacker[ i ].m_vX[ CTaskStacker::xC_TRAY_CV_REAR_SLOW_SEN ] );
	}
	
	for (int i = 0; i < ( int )OnOffDi_Idx.size(); i++) {
		g_IO.SetSimulDi( OnOffDi_Idx[i], DEF_OFF);
	}
}

void CDlgDebugAutoStacker::OnSimulationConveyorFrontAllOn()
{
	std::vector<int> OnOffDi_Idx;
	for( int i = 0; i < eMaxStackerCount; i++ ) {
		OnOffDi_Idx.push_back( g_TaskStacker[ i ].m_vX[ CTaskStacker::xC_TRAY_CV_FRONT_EXIST_SEN ] );
		OnOffDi_Idx.push_back( g_TaskStacker[ i ].m_vX[ CTaskStacker::xC_TRAY_CV_FRONT_SLOW_SEN ] );
	}

	for( int i = 0; i < ( int )OnOffDi_Idx.size(); i++ ) {
		g_IO.SetSimulDi( OnOffDi_Idx[ i ], DEF_ON );
	}
}

void CDlgDebugAutoStacker::OnSimulationConveyorFrontAllOff()
{
	std::vector<int> OnOffDi_Idx;
	for( int i = 0; i < eMaxStackerCount; i++ ) {
		OnOffDi_Idx.push_back( g_TaskStacker[ i ].m_vX[ CTaskStacker::xC_TRAY_CV_FRONT_EXIST_SEN ] );
		OnOffDi_Idx.push_back( g_TaskStacker[ i ].m_vX[ CTaskStacker::xC_TRAY_CV_FRONT_SLOW_SEN ] );
	}

	for( int i = 0; i < ( int )OnOffDi_Idx.size(); i++ ) {
		g_IO.SetSimulDi( OnOffDi_Idx[ i ], DEF_OFF );
	}
}

void CDlgDebugAutoStacker::OnSimulationUnloadExistAllOn()
{
	std::vector<int> OnOffDi_Idx;
	for( int i = eSTACKER_04; i < eMaxStackerCount; i++ ) {
		OnOffDi_Idx.push_back( g_TaskStacker[ i ].m_vX[ CTaskStacker::xC_TRAY_STACKER_EXIST_SEN ] );
	}

	for( int i = 0; i < ( int )OnOffDi_Idx.size(); i++ ) {
		g_IO.SetSimulDi( OnOffDi_Idx[ i ], DEF_ON );
	}
}

void CDlgDebugAutoStacker::OnSimulationUnloadExistAllOff()
{
	std::vector<int> OnOffDi_Idx;
	for( int i = eSTACKER_04; i < eMaxStackerCount; i++ ) {
		OnOffDi_Idx.push_back( g_TaskStacker[ i ].m_vX[ CTaskStacker::xC_TRAY_STACKER_EXIST_SEN ] );
	}

	for( int i = 0; i < ( int )OnOffDi_Idx.size(); i++ ) {
		g_IO.SetSimulDi( OnOffDi_Idx[ i ], DEF_OFF );
	}
}


void CDlgDebugAutoStacker::OnBnClickedButtonOpPanelLampSw()
{
	g_IO.SetSimulDi(DI_LAMP_SW_SEN, TRUE);
	Sleep(200);
	g_IO.SetSimulDi(DI_LAMP_SW_SEN, FALSE);
}

void CDlgDebugAutoStacker::OnBnClickedButtonOpPanelSoundSw()
{
	g_IO.SetSimulDi(DI_SOUND_SW_SEN, TRUE);
	Sleep(200);
	g_IO.SetSimulDi(DI_SOUND_SW_SEN, FALSE);
}

void CDlgDebugAutoStacker::OnBnClickedButtonOpPanelCoverSw()
{
	if (g_COVER_SW.GetStatus()) {
		g_COVER_SW.Cover_Unlock();
		if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
			g_IO.SetSimulDi(DI_COVER_SW_SEN, FALSE);
			Sleep(SIMUL_SLEEP_TIME);
		}
	}
	else {
		g_COVER_SW.Cover_Lock();
		if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
			g_IO.SetSimulDi(DI_COVER_SW_SEN, TRUE);
			Sleep(SIMUL_SLEEP_TIME);
		}
	}
}


void CDlgDebugAutoStacker::OnBnClickedBtnUph()
{
	CString strMsg;
	::GetLocalTime(&m_SystemTime);

	SYSTEMTIME time;
	time = m_SystemTime;
	strMsg.Format("UPH START :: %04d-%02d-%02d %02d:%02d:%02d.%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	UpdateEventMessage(strMsg);
	GetDlgItem(IDC_BTN_UPH)->EnableWindow(FALSE);
	SetTimer(500, 1, NULL);
}


void CDlgDebugAutoStacker::OnBnClickedBtnUphStop()
{
	KillTimer(500);
	GetDlgItem(IDC_BTN_UPH)->EnableWindow(TRUE);
}


void CDlgDebugAutoStacker::OnBnClickedBtnJamLotDataClear()
{
	int nCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotJamCount);
	if (nCnt > 0) {
		nCnt = nCnt - 1;
		g_DMCont.m_dmEQP.SN(NDM0_CurLotJamCount, nCnt);
	}

	return;
	// ??imsi uph 측정을 위한 생산팀 요청 < start >
	g_DMCont.m_dmEQP.SN(NDM0_CurLotLoadingCnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotSortingCnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotPassBinCnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotFailBinCnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotOutput1Cnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotOutput2Cnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotOutput3Cnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotOutput4Cnt, 0);

	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_LoadingCnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_SortingCnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_PassBinCnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_FailBinCnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_Output1Cnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_Output2Cnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_Output3Cnt, 0);
	g_DMCont.m_dmEQP.SN(NDM0_CurLotRT_Output4Cnt, 0);

	int nTestSite_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nTestSite_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);

	for (int i = 0; i < eMaxPressUnitCount; i++)
	{
		//g_DMCont.m_dmEQP.SN(NDM0_ContactCnt_Press1+i, 0);
		g_DMCont.m_dmEQP.SN(NDM0_CleaningContact_Press1 + i, 0);
		for( int nY = 0; nY < nTestSite_Y; nY++)
		{
			for (int nX = 0; nX < nTestSite_X; nX++) {
				g_DMCont.m_dmContac_Dn.SN(NDM1_GdBinCnt_Press1_1 + nX + (nTestSite_X * nY), 0);
				g_DMCont.m_dmContac_Dn.SN(NDM1_ClrAfterGdBinCntPress1_1 + nX + (nTestSite_X * nY), 0);
				g_DMCont.m_dmContac_Dn.SN(NDM1_FailBinCnt_Press1_1 + nX + (nTestSite_X * nY), 0);
				g_DMCont.m_dmContac_Dn.SN(NDM1_ClsAfterFailBinCntPress1_1 + nX + (nTestSite_X * nY), 0);
				g_DMCont.m_dmContac_Up.SN(NDM2_1STFailBinCnt_Press1_1 + nX + (nTestSite_X * nY), 0);
				g_DMCont.m_dmContac_Up.SN(NDM2_2DIDFailBinCnt_Press1_1 + nX + (nTestSite_X * nY), 0);
			}
		}
	}

	g_DMCont.m_dmEQP.SN(NDM0_CurLotJamCount, 0);
	g_Error.ResetCurrentLotJamCount();
	// ??imsi uph 측정을 위한 생산팀 요청 < end >
}


void CDlgDebugAutoStacker::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CDlgDebugAutoStacker::InitSpreadSheet()
{
	m_SpreadClrInfo.SetTextMatrix(1, 1, "First Test");
	m_SpreadClrInfo.SetForeColor(COLOR_GREEN);
	m_SpreadClrInfo.SetTextMatrix(1, 2, "Second Test");
	m_SpreadClrInfo.SetForeColor(COLOR_DARKYELLOW);
	m_SpreadClrInfo.SetTextMatrix(1, 3, "Final Test");
	m_SpreadClrInfo.SetForeColor(COLOR_RED);

	m_SpreadClrTest.SetTextMatrix(1, 1, "Normal");
	m_SpreadClrTest.SetBackColor(RGB(255,255,255));
	m_SpreadClrTest.SetTextMatrix(1, 2, "Testing");
	m_SpreadClrTest.SetBackColor(RGB(255,255,0));
	m_SpreadClrTest.SetTextMatrix(1, 3, "Contact");
	m_SpreadClrTest.SetBackColor(RGB(0, 255, 255));

	m_SPRD_TestPP1_L.SetTextMatrix( 0, 0, "LD");
	m_SPRD_TestPP1_U.SetTextMatrix( 0, 0, "UD");
	m_SPRD_TestPP2_L.SetTextMatrix(0, 0, "LD");
	m_SPRD_TestPP2_U.SetTextMatrix(0, 0, "UD");
	//m_SPRD_Retest_Buff_Table1.SetTextMatrix( 0, 0, "R1" );
	m_SPRD_Socketclean_Buff_Table1.SetTextMatrix( 0, 0, "S1" );
	//m_SPRD_Retest_Buff_Table2.SetTextMatrix( 0, 0, "R2" );
	m_SPRD_Socketclean_Buff_Table2.SetTextMatrix( 0, 0, "S2" );

	m_SPRD_Load_Table1.SetTextMatrix( 0, 0, "L1" );
	m_SPRD_Load_Table2.SetTextMatrix( 0, 0, "L2" );
	m_SPRD_Load_Table3.SetTextMatrix(0, 0, "L3");
	m_SPRD_Load_Table4.SetTextMatrix(0, 0, "L4");

	//헤더 병합.

	//CString msg = "";
	//long SPREADHEADER = -1000;
	//
	//int spreadID[] = { IDC_SPREAD_TEST_SITE1, IDC_SPREAD_TEST_SITE2, IDC_SPREAD_TEST_SITE3, IDC_SPREAD_TEST_SITE4, 
	//	               IDC_SPREAD_TEST_SITE5, IDC_SPREAD_TEST_SITE6, IDC_SPREAD_TEST_SITE7,IDC_SPREAD_TEST_SITE8 };
	//
	//int spreadID2[] = { IDC_SPREAD_TEST_SITE9, IDC_SPREAD_TEST_SITE10, IDC_SPREAD_TEST_SITE11, IDC_SPREAD_TEST_SITE12, 
	//					IDC_SPREAD_TEST_SITE13, IDC_SPREAD_TEST_SITE14, IDC_SPREAD_TEST_SITE15,IDC_SPREAD_TEST_SITE16 };

	//m_SPRD_EnableSite.SetRow(SPREADHEADER);
	//m_SPRD_EnableSite2.SetRow(SPREADHEADER);

	//for (int x = 0; x < sizeof(spreadID) / sizeof(spreadID[0]); x++)
	//{
	//	// #1 ~ #8
	//	CSpreadSheet* spread = (CSpreadSheet*)GetDlgItem(spreadID[x]);
	//	spread->SetRow(SPREADHEADER);
	//	spread->AddCellSpan(1, 0, g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X), 1);
	//	spread->SetCol(1);
	//	msg.Format("Station%d", x + 1);
	//	spread->SetText(msg);
	//
	//	// #8 ~ #16
	//	spread = (CSpreadSheet*)GetDlgItem(spreadID2[x]);
	//	spread->SetRow(SPREADHEADER);
	//	spread->AddCellSpan(1, 0, g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X), 1);
	//	spread->SetCol(1);
	//	msg.Format("Station%d", x + 9);
	//	spread->SetText(msg);
	//}
}

void CDlgDebugAutoStacker::SetDisplayStageInfo(int spreadCtrlID, int stage, int columnLength, int rowLength,int rowstart)
{
	CSpreadSheet* spread = (CSpreadSheet*)GetDlgItem(spreadCtrlID);
	ST_DD_DEVICE stDeviceData;

	for (int nRows = 0; nRows < rowLength; nRows++)
	{
		for (int nCols = 0; nCols < columnLength; nCols++)
		{
			g_DMCont.m_dmDevice.GetDeviceData(stage, nCols, nRows+ rowstart, &stDeviceData);

			if (stDeviceData.sExist == DEF_EXIST) {
				CString str = _T("");
				str.Format("%s", stDeviceData.szBinResult);
				spread->SetTextMatrix(nRows + 1, nCols + 1, str);
			} 
			else {
				spread->SetTextMatrix(nRows + 1, nCols + 1, "");
			}

			if (stDeviceData.nTestingCnt == eARB_1stTestEnd) {
				spread->SetForeColor(COLOR_GREEN);
			}
			else if (stDeviceData.nTestingCnt == eARB_2stTestEnd) {
				spread->SetForeColor(COLOR_DARKYELLOW);
			}
			else if (stDeviceData.nTestingCnt == eARB_3stTestEnd) {
				spread->SetForeColor(COLOR_RED);
			}
			else {
				spread->SetForeColor(COLOR_BBLACK);
			}

		}
	}

	spread = NULL;
}

void CDlgDebugAutoStacker::SetDisplayPressStageInfo(int spreadCtrlID, int site, int stage, int columnLength, int rowLength)
{
	CSpreadSheet* spread = (CSpreadSheet*)GetDlgItem(spreadCtrlID);
	ST_DD_DEVICE stDeviceData;

	BOOL bIsStationTesting = g_DMCont.m_dmEQP.GB(BDM0_TEST_STATUS_SITE1 + site);	
	BOOL bIsStationContact = g_DMCont.m_dmEQP.GB(BDM0_PRESS_CONTACT_STATUS_SITE1 + site);

	for( int nRow = 0; nRow < rowLength; nRow++ )
	{
		for (int nCols = 0; nCols < columnLength; nCols++)
		{
			spread->SetRow(1 + nRow);
			spread->SetCol(nCols + 1);
			g_DMCont.m_dmDevice.GetDeviceData(stage, nCols , nRow, &stDeviceData);

			//down
			if (stDeviceData.sExist == DEF_EXIST)
			{
				CString str = _T("");
				str.Format("%s", stDeviceData.szBinResult);
				spread->SetTextMatrix(1 + nRow, nCols + 1, str);
			}
			else
			{
				spread->SetTextMatrix(1 + nRow, nCols + 1, "");
			}

			if (stDeviceData.nTestingCnt == eARB_1stTestEnd) {
				spread->SetForeColor(COLOR_GREEN);
			}
			else if (stDeviceData.nTestingCnt == eARB_2stTestEnd) {
				spread->SetForeColor(COLOR_DARKYELLOW);
			}
			else if (stDeviceData.nTestingCnt == eARB_3stTestEnd) {
				spread->SetForeColor(COLOR_RED);
			}
			else {
				spread->SetForeColor(COLOR_BBLACK);
			}

			if (bIsStationTesting == TRUE)
			{
				spread->SetBackColor(RGB(255, 255, 0));
			}
			else if (bIsStationContact == TRUE)
			{
				spread->SetBackColor(RGB(0, 255, 255));
			}
			else {
				spread->SetBackColor(RGB(255, 255, 255));
			}

		}
	}

	spread = NULL;
}


void CDlgDebugAutoStacker::SetDisplaySOTEOTInfo(int spreadCtrlID, int columnLength, int rowLength, int nModule)
{
	CSpreadSheet* spread = (CSpreadSheet*)GetDlgItem(spreadCtrlID);
	ST_DD_DEVICE stDeviceData;
	SYSTEMTIME sysTmTestStart;

	SYSTEMTIME sysCompTestTm[2];
	for (int nCols = 0; nCols < columnLength; nCols++)
	{
		::GetLocalTime(&sysCompTestTm[nCols]);
	}
	int nCol_loc[2] = { 0 ,}, nRow_Loc[2] = { 0 , };
	int nRowReverse = 3; // Station 번호 역방향으로 계산
	for (int nRows = 0; nRows < rowLength; nRows++)
	{
		for (int nCols = 0; nCols < columnLength; nCols++)
		{	
			int nStationNo = nRows + (nModule * (STATION_MAX_PARA / 2));
			sysTmTestStart = nCols == 0 ? g_SOT[nStationNo] : g_EOT[nStationNo];
			if (g_TaskPressUnit[nStationNo].GetSocketOnCnt() == 0)
			{
				spread->SetTextMatrix(nRowReverse - nRows + 1, nCols + 1, "socket off");
				continue;
			}
		

			int nCompResult = CompSystemTime(sysCompTestTm[nCols], sysTmTestStart);
			// Station 비교 후, 제일 빠른 시간
			if (nCompResult == 1) // sysTestTm < sysCompTestTm
			{
				sysCompTestTm[nCols] = sysTmTestStart;
				nCol_loc[nCols] = nCols;
				nRow_Loc[nCols] = nRowReverse - nRows;
			}

			CString strStartTm = _T("");
			strStartTm.Format("%02d:%02d:%02d:%03d", sysTmTestStart.wHour, sysTmTestStart.wMinute, sysTmTestStart.wSecond, sysTmTestStart.wMilliseconds);

			spread->SetTextMatrix(nRowReverse - nRows + 1, nCols + 1, strStartTm);
		}
	}

	for (int nRows = 0; nRows < rowLength; nRows++)
	{
		for (int nCols = 0; nCols < columnLength; nCols++)
		{
			int nCalcRow = nRowReverse - nRows;
			spread->SetRow(nRowReverse - nRows + 1);
			spread->SetCol(nCols + 1);

			if (nCalcRow == nRow_Loc[nCols] && nCols == nCol_loc[nCols])
			{
				spread->SetBackColor(COLOR_GREEN);
			}
			else {
				spread->SetBackColor(COLOR_WWHITE);
			}
			
		}
	}

	spread = NULL;
}

void CDlgDebugAutoStacker::OnBnClickedButtonMotionDbReload()
{
	CMyList m_ListAxisInfoDb;
	int nErr = g_DB.SELECT_MOTORPARAM_TBL( m_ListAxisInfoDb ); // 모터 정보를 Logical Idx 오름 차순으로 불러 온다.
	if( nErr != ERR_NO_ERROR ) {
		AfxMessageBox( _T( "SELECT_MOTORPARAM_TBL fail" ) );
		return;
	}
		
	int nAxisCount = g_Motor.GetAxisCount();
	for( int i = 0; i < nAxisCount; i++ )
	{
		BOOL bDegreeCtl = FALSE;
		_tMOTOR_PARAM* pAxisInfo = (_tMOTOR_PARAM*)m_ListAxisInfoDb.m_fnGetData(i);
		g_Motor[i]->SetAxisInitInfo((LPSTR)(LPCTSTR)pAxisInfo->strAxisName,
			pAxisInfo->nAxisNo_Physical,
			pAxisInfo->nAxisNo_Logical,
			pAxisInfo->dScaleFactor,
			pAxisInfo->nDir_ZR,
			pAxisInfo->nMMC_Encoder_Dir,
			pAxisInfo->nDirMove,
			pAxisInfo->nEncoder_Type,
			pAxisInfo->nMMC_Coordinate_Dir,
			pAxisInfo->nDirPulse,
			pAxisInfo->nDouble_Chk_Pulse,
			(MOTOR_TYPE)pAxisInfo->nMotorType,
			(MOTOR_SYNC_TYPE)pAxisInfo->nSyncType,
			pAxisInfo->nSlaveNo,
			bDegreeCtl,
			pAxisInfo->nPulsePerRound,
			pAxisInfo->dHwInPositionWidth);

		g_Motor[i]->SetVelAcc(pAxisInfo->nSpd_Normal,
			pAxisInfo->nAcc_Normal,
			pAxisInfo->nDec_Normal,
			pAxisInfo->nSpd_ZR,
			pAxisInfo->nAcc_ZR,
			pAxisInfo->dLimit_Posi,
			pAxisInfo->dLimit_Nega);
	}
	AfxMessageBox( _T( "Motion DB Reload Success" ) );
}

void CDlgDebugAutoStacker::ClickSpreadLoadTbl( _eLoadTblIdx _idx, int _col, int _row )
{
	if (_col <= 0 || _row <= 0)
		return;

	CDlgDeviceInfoEditor dlg( eDeviceStage_LD_TBL_1 + _idx, _col - 1, _row - 1 );
	dlg.DoModal(); 
}

void CDlgDebugAutoStacker::RightClickSpreadLoadTbl1( short ClickType, long Col, long Row, long MouseX, long MouseY )
{
	ClickSpreadLoadTbl( eLoadTbl_1, Col, Row );
}


void CDlgDebugAutoStacker::RightClickSpreadLoadTbl2( short ClickType, long Col, long Row, long MouseX, long MouseY )
{
	ClickSpreadLoadTbl( eLoadTbl_2, Col, Row );
}


void load_table_init_idle_reserve( _eLoadTblIdx _idx ) {

	int ret = AfxMessageBox( _T( "Do you want to change the load table reserve state?" ), MB_OKCANCEL );
	if( ret != IDOK )
		return;

	g_TaskLoadTbl[ _idx ].SetWorkReserve( RID_INIT_IDLE_RESERVE );
	//g_TaskAutoTestPp[ _idx ].SetSiteGroupReserve( false );
}

void CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerLoadTable1InitIdleReserve()
{
	load_table_init_idle_reserve( eLoadTbl_1 );
}


void CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerLoadTable2InitIdleReserve()
{
	load_table_init_idle_reserve( eLoadTbl_2 );
}

void CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerLoadTable3InitIdleReserve()
{
	load_table_init_idle_reserve(eLoadTbl_3);
}


void CDlgDebugAutoStacker::do_device_data_clip( int _device_stage_idx, CRect _ltrb, int _key )
{
	if( _ltrb.left < 0 || _ltrb.top < 0 )
		return;

	try
	{
		switch( _key ) {
			case 67: // c : copy
			case 88: // x : cut
				{
					m_DeviceDataClipBoard.clear();

					for( int row = _ltrb.top; row <= _ltrb.bottom; row++ ) {
						for( int col = _ltrb.left; col <= _ltrb.right; col++ ) {
							ST_DD_DEVICE device_data;
							g_DMCont.m_dmDevice.GetDeviceData( _device_stage_idx, col, row, &device_data );
							m_DeviceDataClipBoard.push_back( device_data );

							if( _key == 88 ) {
								ST_DD_DEVICE empty_device_data;
								g_DMCont.m_dmDevice.SetDeviceData( _device_stage_idx, col, row, &empty_device_data );
							}
						}
					}
				}break;
			case 86: // v : paste
				{
					int idx = 0;
					for( int row = _ltrb.top; row <= _ltrb.bottom; row++ ) {
						for( int col = _ltrb.left; col <= _ltrb.right; col++ ) {
							
							if( idx >= (int)m_DeviceDataClipBoard.size() ) {
								break;
							}

							ST_DD_DEVICE device_data = m_DeviceDataClipBoard[ idx++ ];
							g_DMCont.m_dmDevice.SetDeviceData( _device_stage_idx, col, row, &device_data );
						}
					}
				}break;
		}
	}
	catch( ... )
	{

	}
}


void CDlgDebugAutoStacker::KeyDownSpreadLoadTbl1( short* KeyCode, short Shift )
{
	VARIANT col_start, col_end, row_start, row_end;
	
	col_start.intVal = m_SPRD_Load_Table1.GetSelBlockCol();
	row_start.intVal = m_SPRD_Load_Table1.GetSelBlockRow();
	col_end.intVal = m_SPRD_Load_Table1.GetSelBlockCol2();
	row_end.intVal = m_SPRD_Load_Table1.GetSelBlockRow2();

	if( col_start.intVal < 1 || row_start.intVal < 1 ) {
		return;
	}

	CRect ltrb = CRect( col_start.intVal - 1, row_start.intVal - 1, col_end.intVal - 1, row_end.intVal - 1 );
	do_device_data_clip( eDeviceStage_LD_TBL_1, ltrb, *KeyCode );
}


void CDlgDebugAutoStacker::KeyDownSpreadLoadTbl2( short* KeyCode, short Shift )
{
	VARIANT col_start, col_end, row_start, row_end;

	col_start.intVal = m_SPRD_Load_Table2.GetSelBlockCol();
	row_start.intVal = m_SPRD_Load_Table2.GetSelBlockRow();
	col_end.intVal = m_SPRD_Load_Table2.GetSelBlockCol2();
	row_end.intVal = m_SPRD_Load_Table2.GetSelBlockRow2();

	if( col_start.intVal < 1 || row_start.intVal < 1 ) {
		return;
	}

	CRect ltrb = CRect( col_start.intVal - 1, row_start.intVal - 1, col_end.intVal - 1, row_end.intVal - 1 );
	do_device_data_clip( eDeviceStage_LD_TBL_2, ltrb, *KeyCode );
}


void CDlgDebugAutoStacker::OnBnClickedButtonEventLogAllDelete()
{
// 	CParamList AlarmList;
// 	g_Error.AlarmReport(ERR_LOT_CMD_EMPTY_TRAY_IS_SHORTAGE, AlarmList, this);
//	g_DBErr.DELETE_LOG_TBL_ALL();
}

void CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerManual3()
{
	// FWHID Mismatch

// 	std::vector<ST_FW_VERSION> vFWHIDMisMatch;
// 	ST_FW_VERSION stVesrion;
// 	stVesrion.nIndex = 1;
// 	stVesrion.sVersion = "tt";
// 	vFWHIDMisMatch.push_back(stVesrion);
// 	stVesrion.clear();
// 	stVesrion.nIndex = 3;
// 	stVesrion.sVersion = "a";
// 	vFWHIDMisMatch.push_back(stVesrion);
// 	stVesrion.clear();
// 	stVesrion.nIndex = 30;
// 	stVesrion.sVersion = "aaa";
// 	vFWHIDMisMatch.push_back(stVesrion);
// 	stVesrion.clear();
// 	stVesrion.nIndex = 30;
// 	stVesrion.sVersion = "aaa";
// 	vFWHIDMisMatch.push_back(stVesrion);
// 
// 	std::unordered_map<std::string, size_t> mapFWHIDMisMatch;
// 	for (size_t i = 0; i < vFWHIDMisMatch.size(); ++i) {
// 		if (mapFWHIDMisMatch.find(vFWHIDMisMatch[i].sVersion) == mapFWHIDMisMatch.end()) {
// 			mapFWHIDMisMatch[vFWHIDMisMatch[i].sVersion] = vFWHIDMisMatch[i].nIndex;
// 		}
// 	}
// 
// 	if (mapFWHIDMisMatch.size() > 0)
// 	{
// 		CParamList AlarmData;
// 		AlarmData.m_fnAddItemFormat("Tester FW HID Version MisMatch");
// 		for (std::unordered_map<std::string, size_t>::const_iterator it = mapFWHIDMisMatch.begin();
// 			it != mapFWHIDMisMatch.end(); ++it)
// 		{
// 			AlarmData.m_fnAddItemFormat("[Tester=%s] =%d", it->first.c_str(),it->second);
// 			
// 			//return ERR_RECEIVE_TESTER;
// 		}
// 		g_Error.AlarmReport(ERR_RECEIVE_TESTER, AlarmData, this);
// 	}
// 	return;

	int nRetError = 0;
	nRetError |= ERR_TEST_PP_1_Z1_DEVICE_PICK_FAIL;
	nRetError |= ERR_TEST_PP_1_Z1_CLEAN_DEVICE_PICK_FAIL;

	std::vector<CPoint> vAxisDonePkr;
//  	vAxisDonePkr.push_back(CPoint(3,1));
//  	vAxisDonePkr.push_back(CPoint(0, 1));
	std::vector<CPoint> m_vAxisDonePkr;
	int nPickerIdx = 0;
	int nReverseIdx = 0;
	CString strErrTempPkr, strErrPkr;
	for (auto it = vAxisDonePkr.begin(); it != vAxisDonePkr.end(); it++)
	{
		nPickerIdx = (*it).x + MAX_PICKER_X_CNT * (*it).y;
		nReverseIdx = abs(MAX_PICKER_Z_AXIS_CNT - nPickerIdx -1 );
		m_vAxisDonePkr.push_back(CPoint(nReverseIdx % MAX_PICKER_X_CNT, nReverseIdx / MAX_PICKER_X_CNT));

	}

	for (auto it = m_vAxisDonePkr.begin(); it != m_vAxisDonePkr.end(); it++) {
		strErrTempPkr.Format("[X:%d, Y:%d]", (*it).x + 1, (*it).y + 1);
		strErrPkr += strErrTempPkr;
	}


	{
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Test PP %d %s", 0 + 1, "");
		g_Error.AlarmReport(ERR_TEST_PP_1_Z1_DEVICE_PICK_FAIL, AlarmData, &g_TaskSystemCtrl);
	}

	return;

	std::vector<ST_FW_VERSION> vFWHIDMisMatch;
	ST_FW_VERSION stVesrion;
	stVesrion.nIndex = 1;
	stVesrion.sVersion = "tt";
	vFWHIDMisMatch.push_back(stVesrion);
	stVesrion.clear();
	stVesrion.nIndex = 3;
	stVesrion.sVersion = "a";
	vFWHIDMisMatch.push_back(stVesrion);
	stVesrion.clear();
	stVesrion.nIndex = 30;
	stVesrion.sVersion = "aaa";
	vFWHIDMisMatch.push_back(stVesrion);
	stVesrion.clear();
	stVesrion.nIndex = 30;
	stVesrion.sVersion = "aaa";
	vFWHIDMisMatch.push_back(stVesrion);

	std::unordered_map<std::string, size_t> mapFWHIDMisMatch;
	for (size_t i = 0; i < vFWHIDMisMatch.size(); ++i) {
		if (mapFWHIDMisMatch.find(vFWHIDMisMatch[i].sVersion) == mapFWHIDMisMatch.end()) {
			mapFWHIDMisMatch[vFWHIDMisMatch[i].sVersion] = vFWHIDMisMatch[i].nIndex;
		}
	}

	if (mapFWHIDMisMatch.size() > 0)
	{
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Tester FW HID Version MisMatch");
		for (std::unordered_map<std::string, size_t>::const_iterator it = mapFWHIDMisMatch.begin();
			it != mapFWHIDMisMatch.end(); ++it)
		{
			AlarmData.m_fnAddItemFormat("[Tester=%s] =%d", it->first.c_str(),it->second);
			
			//return ERR_RECEIVE_TESTER;
		}
		g_Error.AlarmReport(ERR_RECEIVE_TESTER, AlarmData, this);
	}
	return;

// 	char sendBuffer[100] = "Hello World";
// 
// 	int resultCode = ::sendto(g_UDPSockBinSolution, sendBuffer, sizeof(sendBuffer), 0,
// 		(SOCKADDR*)&g_udpserverAddr, sizeof(g_udpserverAddr));

// 	CString strBinSolutionData = _T("");
// 	g_TaskPressUnit[1].SetTextBinSolution(strBinSolutionData, 1);
// 
// 	int resultCode = ::sendto(g_UDPSockBinSolution, strBinSolutionData, strBinSolutionData.GetLength(), 0,
// 		(SOCKADDR*)&g_udpserverAddr, sizeof(g_udpserverAddr));
// 
// 	//delete[] pcBinSoultionData;
// 	bool bWork = g_TestPP_Table_work_area[eTestPP_1].reserve_work_area(&g_TaskAutoTestPp[eTestPP_1]);
// 	if (bWork != true)
// 		int a = 1;
// 
// 	bWork = g_TestPP_Table_work_area[eTestPP_1].reserve_work_area(&g_TaskAutoLdTable[1]);
// 	if (bWork != true)
// 		int a = 1;
// 	g_TestPP_Table_work_area[eTestPP_1].release_work_area(&g_TaskAutoTestPp[eTestPP_1]);
// 	g_TestPP_Table_work_area[eTestPP_1].release_work_area(&g_TaskAutoLdTable[eLoadTbl_2]);
// 	g_TestPP_Table_work_area[eTestPP_2].release_work_area(&g_TaskAutoTestPp[eTestPP_2]);
// 	g_TestPP_Table_work_area[eTestPP_2].release_work_area(&g_TaskAutoLdTable[eLoadTbl_3]);
// 
// 	return;
  	ST_DD_DEVICE stDvcHand;
 	int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
 	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
// 
// 	int nCntX = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
// 	for (int i = 0; i < nCntX; i++)
// 	{
// 		sprintf_s(stDvcHand.szBinResult, sizeof(stDvcHand.szBinResult), "20");
// 		g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TRAY_FEEDER, i, 33, &stDvcHand);
// 	}
	
// 
// 	for (int nY = 0; nY < nSiteDivY; nY++)
// 	{
// 		for (int nX = 0; nX < nSiteDivX; nX++)
// 		{
// 			stDvcHand.clear();
// 			/*stDvcHand.sExist = DEF_NOT_EXIST;*/
// 			// 			stDvcHand.nTestingCnt = 3;
// 			// 			stDvcHand.nBinColor = 1;
// 
// 			/*			sprintf_s(stDvcHand.szBinResult, sizeof(stDvcHand.szBinResult), "");*/
// 			int nDvc = g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_SITE_1, nX, nY, &stDvcHand);
// 		}
// 	}

 	std::vector<CPoint> Picker;
 	Picker.push_back(CPoint(0,0));
 	CParamList AlarmData;
/*	AlarmData.m_fnAddItemFormat("Picker [x=%d,y=%d]", Picker[0].x +1, Picker[0].y +1);*/
	//AlarmData.m_fnAddItemFormat("Clean Table #%d Clean Device Life is over \n", 0 + 1);


	g_Error.AlarmReport(11001, AlarmData, NULL/*, &g_TaskSystemCtrl, Picker*/);


	//g_Error.AlarmReport(ERR_TRAY_PP_1_PLACE_POS_SAFETY_SENSING, AlarmData, &g_TaskSystemCtrl, Picker);
	return;
// 	AlarmData.m_fnAddItemFormat("Test PP %d %s", 0 + 1, "");
// 	g_Error.AlarmReport(ERR_TRAY_PP_1_DEVICE_AUTO_SKIP_PICK_FAIL_OVER, AlarmData, &g_TaskSystemCtrl);
//  	return;
//	g_pConectSocket->Reconnect();
// 	int nSize_T = 8;
// 	char str[] = _T("ddddddd");
// 	unsigned long long txt_size = sizeof(str);
// 	unsigned long long packet_size = 1 + nSize_T + txt_size;
// 	char* buf = new char[packet_size];
// 	memset(buf, 0, packet_size);
// 	strcpy(&buf[0], "1");
// 	memcpy(&buf[1], &txt_size, nSize_T); // byte 7~38
// 	memcpy(&buf[1 + nSize_T], &str[0], txt_size); // byte 7~38


	//unicode > multibyte
	/*
	wchar_t strUnicode[256] = { 0, };
	char    strMultibyte[256] = { 0, };
	wcscpy_s(strUnicode, 256, L"유니코드");
	int len = WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, NULL, 0, NULL, NULL);   
	WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, strMultibyte, len, NULL, NULL);
	*/
	//multibyte > unicode
	char strMultibyte[] = _T("TEST");
	strcpy_s(strMultibyte, sizeof(strMultibyte), strMultibyte);
	wchar_t strUnicode[17] = { 0, };
	int nLen = MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), strUnicode, nLen);
	

	//g_pConectSocket->Send(strUnicode, sizeof(strUnicode));

	return;

	
// 	if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
// 	{
		g_DMCont.m_dmDevice.SetTrayExist(eDeviceStage_TRAY_FEEDER, DEF_EXIST);
	//}
	//g_Error.AlarmReport(ERR_LOT_MSG_REQ_ADD_LOAD_TRAY, AlarmData, &g_TaskSystemCtrl);
	return;
	g_TaskSystemCtrl.SetErrTestPpIdx(0, 1);
	

	std::vector<CPoint> vPicker;
	vPicker.push_back(CPoint(0, 0));
	AlarmData.m_fnAddItemFormat("Test PP %d %s", 0 + 1, "");
	g_Error.AlarmReport(ERR_TEST_PP_1_X1_VAC_SEN_ERR, AlarmData, &g_TaskSystemCtrl, vPicker);
	g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);


	int nPocketX = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_X);
	int nPocketY = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_X);
	int nDvcCnt = 0;

	ST_DD_DEVICE Dvc;

	for (int nY = 0; nY < nPocketY; nY++) {
		for (int nX = 0; nX < nPocketX; nX++) {
			Dvc.clear();
			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1_CLEAN_BUFF, nX, nY, &Dvc);
			Dvc.sExist = eDvcExist;
			sprintf_s(Dvc.szBinResult, sizeof(Dvc.szBinResult), DEF_SOCKET_CLEAN_DEVICE);
			g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_PP_1_CLEAN_BUFF, nX, nY, &Dvc);
		}
	}

	nPocketX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	nPocketY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

	for (int nY = 0; nY < nPocketY; nY++) {
		for (int nX = 0; nX < nPocketX; nX++) {
			Dvc.clear();
		
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1, nX, nY, &Dvc);
				Dvc.sExist = eDvcEmpty;
				sprintf_s(Dvc.szBinResult, sizeof(Dvc.szBinResult), "");
				g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_LD_TBL_1 , nX, nY, &Dvc);
			

		}
	}

	nPocketX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	nPocketY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

	for (int nY = 0; nY < nPocketY; nY++) {
		for (int nX = 0; nX < nPocketX; nX++) {
			Dvc.clear();

			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_4, nX, nY, &Dvc);
			Dvc.sExist = eDvcEmpty;
			sprintf_s(Dvc.szBinResult, sizeof(Dvc.szBinResult), "");
			g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_LD_TBL_4, nX, nY, &Dvc);


		}
	}


	for (int nY = 0; nY < nPocketY; nY++) {
		for (int nX = 0; nX < nPocketX; nX++) {
			Dvc.clear();

			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1, nX, nY, &Dvc);
			Dvc.sExist = eDvcExist;
			sprintf_s(Dvc.szBinResult, sizeof(Dvc.szBinResult), DEF_NEW_LOAD_DEVICE);
			g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_LD_TBL_1, nX, nY, &Dvc);
			

		}
	}

	for (int nY = 0; nY < nPocketY; nY++) {
		for (int nX = 0; nX < nPocketX; nX++) {
			Dvc.clear();
			if(/*(nX == 0 || nX == 2) && */nY ==2)
				continue;

			if (nY == 0 || nY == 2)
			{
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1+2, nX, nY, &Dvc);
				Dvc.sExist = eDvcExist;
				sprintf_s(Dvc.szBinResult, sizeof(Dvc.szBinResult), DEF_NEW_LOAD_DEVICE);
				g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_LD_TBL_1 + 2, nX, nY, &Dvc);
			}
	
		}
	}


	// load picker 정보
	for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
	{
		for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
		{
			stDvcHand.clear();
			Dvc.sExist = eDvcExist;
			sprintf_s(Dvc.szBinResult, sizeof(Dvc.szBinResult), DEF_NEW_LOAD_DEVICE);
			g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_PP_1, nX, nY+2, &stDvcHand);

		}
	}

	for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
	{
		for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
		{
			stDvcHand.clear();
			stDvcHand.sExist = DEF_NOT_EXIST;
			sprintf_s(stDvcHand.szBinResult, sizeof(stDvcHand.szBinResult), "");
			g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_PP_1, nX, nY, &stDvcHand);

		}
	}
	// unload picker 정보
	for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
	{
 		for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
 		{
			stDvcHand.clear();
			stDvcHand.sExist = DEF_EXIST;
			stDvcHand.nTestingCnt = 2;
			stDvcHand.nBinColor = 0;
			sprintf_s(stDvcHand.szBinResult, sizeof(stDvcHand.szBinResult), "2");
			g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_PP_1, nX, nY +2, &stDvcHand);

		}
	}

	for (int nY = 0; nY < nSiteDivY; nY++)
	{
		for (int nX = 0; nX < nSiteDivX; nX++)
		{
			stDvcHand.clear();
			/*stDvcHand.sExist = DEF_NOT_EXIST;*/
// 			stDvcHand.nTestingCnt = 3;
// 			stDvcHand.nBinColor = 1;
			stDvcHand.sExist = eDvcExist;
			sprintf_s(stDvcHand.szBinResult, sizeof(stDvcHand.szBinResult), DEF_SOCKET_CLEAN_DEVICE);
/*			sprintf_s(stDvcHand.szBinResult, sizeof(stDvcHand.szBinResult), "");*/
			int nDvc = g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_SITE_1, nX, nY,&stDvcHand);
		}
	}

	CPoint m_cJobPocket;
	std::vector<CPoint> m_vJobPicker_Pick;
	m_vJobPicker_Pick.clear();
	std::vector<CPoint> m_vJobPicker_Place;
	m_vJobPicker_Place.clear();
	
	//int nJob = g_TaskAutoTestPp[0].CheckJob_CleanBuff(1, DEF_PICK, m_cJobPocket, m_vJobPicker_Pick);

	int nX_PitchType = g_TaskTestPP[0].GetHandPitchType(CTaskTestPP::eAxisPitch_HW_X1_SW_Y1, LOC_IDX_TEST_PP_TBL_1_LD);
	int nY_PitchType = g_TaskTestPP[0].GetHandPitchType(CTaskTestPP::eAxisPitch_HW_Y1_SW_X1, LOC_IDX_TEST_PP_TBL_1_UD);

	
//  	int nJob = g_TaskAutoTestPp[1].CheckJob_LoadTbl(1, DEF_PICK, m_cJobPocket, m_vJobPicker_Pick);
// 	int nnJob = g_TaskAutoTestPp[1].CheckJob_LoadTbl(1, DEF_PLACE, m_cJobPocket, m_vJobPicker_Place);
	m_vJobPicker_Pick.clear();
	m_vJobPicker_Place.clear();
// 	int nnJob = g_TaskAutoTestPp[0].CheckJob_Station(0, DEF_PICK, m_cJobPocket, m_vJobPicker_Pick);
// 	int Job = g_TaskAutoTestPp[0].CheckJob_ArtBuff(1, DEF_PLACE, m_cJobPocket, m_vJobPicker_Place);
	m_vJobPicker_Pick.clear();
	m_vJobPicker_Place.clear();

	int nPocketCount_X = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
	int nPocketCount_Y = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);

	for (int nY = 0; nY < nPocketCount_Y; nY++)
	{
		for (int nX = 0; nX < nPocketCount_X; nX++)
		{	
			int nExist = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TRAY_FEEDER, nX, nY);
			if (nExist == 0)
				int a = 0;
		}
	}
	
	int aaa = 1;


	//g_TaskAutoTestPp[0].m_nJobAtsIdx = 0;
// 	int jonb = g_TaskAutoTestPp[0].CheckJobAlreadyPickDvcPlacetoArtBuff(0, m_cJobPocket, m_vJobPicker_Place);
// 	int nCont = g_TaskTestPP[1].GetBufferTablePocketCnt();
// 	int a = 1;

// 	CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
// 	pSeqCmd->m_nCmdStage = LOC_IDX_LD_TBL_TEST_SIDE_1 + 0;
// 	pSeqCmd->m_cCmdPocket = m_cJobPocket;
// 	pSeqCmd->m_nCmdPick = DEF_PICK;
// 	pSeqCmd->m_vCmdPicker = m_vJobPicker_Pick;
// 	pSeqCmd->m_nCmdHandType = TEST_LD_HAND;
// 	//MakeLog(_T("[LD_TBL PICK][x=%d,y=%d][%s]"), m_cJobPocket.x + 1, m_cJobPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Pick));
// 
// 	if (SendCommand(g_TaskTestPP[0], pSeqCmd,0) == eSeqCmdAck_OK) {
// 	}

	///OnBnClickedBtnZip();
//  	CString strErrPos, strErrMsg, strFailMsg;
// 	int nContiFailCnt =1;
// 	std::vector<CPoint> vFailPocket;
// 	std::vector<int> vTestPPAreaStartEnd;
// 	vTestPPAreaStartEnd.push_back(eTestPP_3);
// 	vTestPPAreaStartEnd.push_back(eTestPP_4);
// 
// 	for (auto i : vTestPPAreaStartEnd)
// 	{
// 		int a = i;
// 	}

// 
// 	for (size_t a = 0; a < 16; a++)
// 	{
// 		for (size_t i = 0; i < 16; i++)
// 		{
// 			vFailPocket.push_back(CPoint(1, 0));
// 			vFailPocket.push_back(CPoint(1, 1));
//  			
// 			strErrPos.Format(_T("[Station=%d Site=%d Count=%d]\n"), i + 1, (1 + 1 + (1 * STATION_MAX_X_SITE)), nContiFailCnt);
// 			strErrMsg += strErrPos;
// 			AlarmData.m_fnAddItemFormat("Occurred consecutive Failure Alarm!!!!");
// 			AlarmData.m_fnAddItemFormat(strFailMsg);
// 			g_Error.AlarmReport(ERR_TESTER_CONSECUTIVE_FAIL_STATION_1+i, AlarmData, this, vFailPocket);
// 		}
// 	}
// 	CParamList AlarmData;
// 	AlarmData.m_fnAddItemFormat("Occurred consecutive Failure Alarm!!!!");
// 	AlarmData.m_fnAddItemFormat(strFailMsg);
// 	g_Error.AlarmReport(ERR_TESTER_MAX_TEST_TIME_OVER_DN1, AlarmData, this, vFailPocket);

// 	CParamList AlarmData;
// 	AlarmData.m_fnAddItemFormat("Test PP %d place picker is sensing, check device", 0 + 1);
// 	AlarmData.m_fnAddItemFormat("[CmdPicker = %s, Stage = %s, Pocket = %s]", "", "", "");
// 	g_Error.AlarmReport(ERR_TEST_PP_1_DEVICE_PLACE_FAIL, AlarmData, this, vFailPocket);


//	g_LogZip.CreateLogZip();
// 	CString str;
// 	for (int i = 0; i < 100; i++)
// 	{
// 		str += "[MakeSocketIDContactCnt] stCountInfo.vSocketCountactInfo,stCountInfo.nIndexCount";
// 	}
// 		
// 
// 	g_TaskSystemCtrl.MakeLog(str);
}

void CDlgDebugAutoStacker::OnBnClickedBtnZip()
{
// 	HZIP hz;
// 	ZRESULT zr;
// 	CString strPathZip = "D:\\ZIP\\test.zip";
// 	CString strPathIni = "D:\\test.ini";
// 
// 	//D:\ZIP\ 경로에 test.zip이란 파일을 생성한다.
// 	CreateDirectory("D:\\ZIP", NULL);
// 	hz = CreateZip(strPathZip, 0);
// 
// 	//zip파일 생성 실패시 처리
// 	if (hz == 0)
// 	{
// 		AfxMessageBox("Error: Failed to create Zip");
// 		return;
// 	}
// 
// 	int i = 0, nLen = 0;
// 	CString strTmp = "", strTmpPath = "", strTmpName = "";
// 
// 	//INI 파일을 활용하여 압축할 파일 정보를 저장한다.
// 	//D드라이브에 test.ini란 파일이름으로 저장한다.
// 	//FTP란 섹션에 Total 키에는 파일 갯수, 그 이후부터는 순번대로 파일 이름을 넣는다.
// 	//INI파일은 D드라이브에 test.ini란 파일이름으로 저장한다.
// 	::WritePrivateProfileString("FTP", "Total", "1", strPathIni);
// 	::WritePrivateProfileString("FTP", "1", "D:\\test.txt", strPathIni);
// 
// 	//ZipAdd 명령으로 D:\test.ini를 idx.ini로 바꾸어 D:\ZIP\test.zip 파일에 추가한다.
// 	zr = ZipAdd(hz, "idx.ini", strPathIni);	//압축할 파일 명"idx.ini"
// 
// 											//ZipAdd 명령 실패시 처리	
// 	if (zr != ZR_OK)
// 	{
// 		AfxMessageBox("Error: Failed to add Zip");
// 		zr = CloseZip(hz);
// 		return;
// 	}
// 
// 	//ZipAdd 명령으로 다른 파일을 추가한다.
// 	{
// 		strTmpName = "test.txt";			//Zip파일에 저장할 파일이름
// 		strTmpPath = "D:\\test.txt";	//압축할 파일 위치
// 		zr = ZipAdd(hz, strTmpName, strTmpPath);
// 
// 		if (zr != ZR_OK)
// 		{
// 			AfxMessageBox("Error: Failed to add Zip");
// 			zr = CloseZip(hz);
// 			return;
// 		}
// 	}
// 
// 	//Zip닫기
// 	zr = CloseZip(hz);
// 
// 	if (zr != ZR_OK)
// 	{
// 		AfxMessageBox("Error: Failed to close zip");
// 		return;
// 	}
}


void CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerManual4()
{
	OnSimulationConveyorRearAllOn();
	Sleep(10);
	OnSimulationConveyorRearAllOff();
	Sleep(10);
	OnSimulationConveyorFrontAllOn();
	Sleep(10);
	OnSimulationConveyorFrontAllOff();



	g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_FTRTQCContinue, eStartMD_Initial);

// 	std::vector<CPoint> m_v2D_DUP_COORDINATE;
// 
// 	for (int i = 2; i <4; i++)
// 	{
// 		for (int j = 0; j < 4; j++)
// 		{
// 			m_v2D_DUP_COORDINATE.push_back(CPoint(j, i));
// 		}
// 	}
// 	
// 
// 	if ((int)m_v2D_DUP_COORDINATE.size() > 0) {
// 		for (int i = 0; i < (int)m_v2D_DUP_COORDINATE.size(); i++)
// 		{
// 			int nIdx = 0;
// 			BOOL bChk = FALSE;
// 			ST_DD_DEVICE tEmptyDeviceData;
// 			CString strErr = _T("");
// 			int n2dReadErrorCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLot2D_Read_Error_Cnt);
// 
// 			for (int i = 0; i < g_BinCategory.nIndexCount; i++) {
// 				// 받은 Error code랑 User가 Setting한 Error Code랑 비교
// 				strErr.Format("%s", g_BinCategory.aCateInfo[i].szDesc);
// 				strErr.MakeUpper();
// 				if (strcmp(strErr, DEF_2D_READ_ERR_DEVICE) == 0) {
// 					nIdx = i;
// 					bChk = TRUE;
// 					break;
// 				}
// 			}
// 		
// 			if (bChk) {
// 				//2D Barcode Read Error sorting 관련
// 				for (int i = 0; i < (int)m_v2D_DUP_COORDINATE.size(); i++) {
// 					//tEmptyDeviceData.clear();
// 					tEmptyDeviceData.sExist = DEF_EXIST;
// 					sprintf_s(tEmptyDeviceData.szBinResult, sizeof(tEmptyDeviceData.szBinResult), "%s", g_BinCategory.aCateInfo[nIdx].szSWBin);
// 					sprintf_s(tEmptyDeviceData.szErrString, sizeof(tEmptyDeviceData.szErrString), "%s", g_BinCategory.aCateInfo[nIdx].szDesc);
// 					tEmptyDeviceData.nBinColor = atoi(g_BinCategory.aCateInfo[nIdx].szPassFail);
// 					tEmptyDeviceData.nTestingCnt = eARB_3stTestEnd;
// 					g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_LD_TBL_1, m_v2D_DUP_COORDINATE[i].x, m_v2D_DUP_COORDINATE[i].y, &tEmptyDeviceData);
// 					n2dReadErrorCnt++;
// 				}
// 				g_DMCont.m_dmEQP.SN(NDM0_CurLot2D_Read_Error_Cnt, n2dReadErrorCnt);
// 			}
// 			
// 		}
// 	}
// 	m_v2D_DUP_COORDINATE.clear();

// 	CParamList AlarmData;
// 	g_Error.AlarmReport(70, AlarmData, this);
// 	int nPocketCntX = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
// 	int nPocketCntY = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);
// 	ST_DD_DEVICE stDevice;
// 
// // 	bool bFindAutoSkipDevice = false;
// // 	std::for_each(g_load_stackers.begin(), g_load_stackers.end(),
// // 		[&](CTaskAutoStacker* _load_stacker)
// // 	{
// // 		if (_load_stacker->GetConveyorState() == ReadyToUnload)
// // 		{
// //			int stacker_idx = _load_stacker->GetStackerIndex();
// 
// 			for (int j = 0; j < nPocketCntY; j++)
// 			{
// 				for (int i = 0; i < nPocketCntX; i++)
// 				{
// 					stDevice.clear();
// //					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_STACKER_CONV_1 + stacker_idx, i, j, &stDevice);
// //					if (stDevice.sExist == eDeviceData_None && strcmp(stDevice.szBinResult, DEF_TRAY_PICK_ERR_DEVICE) == 0)
// 					if(i == 0 && j == 0)
// 					{
// 						StringCchPrintf(stDevice.szBinResult, _countof(stDevice.szBinResult), DEF_TRAY_PICK_ERR_DEVICE);
// 						stDevice.sExist = eDeviceData_Exist;
// 						g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_STACKER_CONV_2 , i, j, &stDevice);
// 					}
// 					else {
// 						StringCchPrintf(stDevice.szBinResult, _countof(stDevice.szBinResult), "");
// 						stDevice.sExist = eDeviceData_None;
// 						g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_STACKER_CONV_2, i, j, &stDevice);
// 					}
// 				}
// 			}
// //		}
// 	//});
}




void CDlgDebugAutoStacker::OnStnDblclickStaticTaskLoadtbl1CmdStage()
{
	std::vector<CPoint> pocket = { {0,0},{ 1,0 },{ 2,0 },{ 3,0 },{ 0,2 },{ 1,2 },{ 2,2 },{ 3,2 } };

	ST_DD_DEVICE stDeviceData;

	stDeviceData.sExist = DEF_EXIST;
	sprintf_s(stDeviceData.szBinResult, sizeof(stDeviceData.szBinResult), "1");
	stDeviceData.nTestingCnt = eARB_1stTestEnd;
	
	for (auto it : pocket)
	{
		g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_LD_TBL_1, it.x, it.y, &stDeviceData);
		g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_LD_TBL_4, it.x, it.y, &stDeviceData);
	}
}


void CDlgDebugAutoStacker::OnStnDblclickStaticTaskLoadtbl4CmdStage()
{
}


void CDlgDebugAutoStacker::OnBnClickedButtonAutoStackerManual5()
{
	int nErr = (int)ShellExecute(NULL, "open", "D:\\Git\\TWSL421\\SLT_Handler\\SLT_TESTER_SIM\\ApTester\\Debug\\ApTester.exe", NULL, NULL, SW_SHOWNORMAL);
}

int CDlgDebugAutoStacker::CompSystemTime(SYSTEMTIME SysTime1, SYSTEMTIME SysTime2) //기존, 신규
{
	//FileTime으로 변환 후, 시간 비교.
	FILETIME FileTime1, FileTime2;
	SystemTimeToFileTime(&SysTime1, &FileTime1); //Time1 변환.
	SystemTimeToFileTime(&SysTime2, &FileTime2); //Time2 변환.

	int nRet = CompareFileTime(&FileTime1, &FileTime2);

	//millisecond 비교
	//Station이 동시에 끝날 경우 초단위 동일.
	if (nRet == 0)
	{
		if (SysTime1.wMilliseconds > SysTime2.wMilliseconds) {
			nRet = 1;
		}
		else if (SysTime1.wMilliseconds < SysTime2.wMilliseconds) {
			nRet = -1;
		}
		else {
			nRet = 0;
		}
	}
	return nRet;
}