#include "StdAfx.h"
#include "TaskSystemCtrl.h"
#include "ConfigData.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int CTaskSystemCtrl::ChkComInterlock()
{
	int nEqpOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_EQP_No);
	// 순서 변경하지 말것.
	//emo 추가
	if (g_IO.in(DI_REAR_LEFT_EMO_SW_SEN) != DEF_ON || g_IO.in(DI_FRONT_LEFT_EMO_SW_SEN) != DEF_ON ||
		g_IO.in(DI_FRONT_RIGHT_EMO_SW_SEN) != DEF_ON || g_IO.in(DI_REAR_RIGHT_EMO_SW_SEN) != DEF_ON) {
		return ERR_SYS_EMO_IS_ON;
	}

	if (nEqpOption >= 4) //4호기 부터 Middle EMO추가
	{
		if (g_IO.in(DI_MIDDLE_LEFT_EMO_SW_SEN) != DEF_ON || g_IO.in(DI_MIDDLE_RIGHT_EMO_SW_SEN) != DEF_ON)
		{
			return ERR_SYS_EMO_IS_ON;
		}
	}

	// Main 전원 off 상태에서 이동 명령 금지
	if (g_IO.in(DI_MAIN_POWER_ON_SEN1) != DEF_ON ) {
		return ERR_MOT_INTERLOCK_MAIN1_POW_IS_NOT_ON;
	}

	if (g_IO.in(DI_MAIN_POWER_ON_SEN2) != DEF_ON) {
		return ERR_MOT_INTERLOCK_MAIN2_POW_IS_NOT_ON;
	}


 	// Cover Switch
 	//if (g_IO.in(DI_COVER_SW_SEN) == DEF_ON) {
 	//	return ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK;
 	//}

	if( g_COVER_SW.GetStatus() != TRUE )
	{
		return ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK;
	}

	// Door Interlock
	if (g_IO.in(DI_FRONT_LEFT_DOORLOCK_SEN) != DEF_ON) {
		return ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK_FRONT_SLIDE_LEFT;
	}
	if (g_IO.in(DI_FRONT_RIGHT_DOORLOCK_SEN) != DEF_ON) {
		return ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK_FRONT_SLIDE_RIGHT;
	}
	if (g_IO.in(DI_HINGED_RIGHT_DOORLOCK_SEN) != DEF_ON) {
		return ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK_HINGED_RIGHT;
	}
	if (g_IO.in(DI_SLIDE_RIGHT_F1_DOORLOCK_SEN) != DEF_ON) {
		return ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK_SLIDE_RIGHT_FRONT;
	}
	if (g_IO.in(DI_SLIDE_RIGHT_R2_DOORLOCK_SEN) != DEF_ON) {
		return ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK_SLIDE_RIGHT_REAR;
	}
	if (g_IO.in(DI_REAR_RIGHT_DOORLOCK_SEN) != DEF_ON) {
		return ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK_REAR_LEFT;
	}
	if (g_IO.in(DI_REAR_LEFT_DOORLOCK_SEN) != DEF_ON) {
		return ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK_REAR_RIGHT;
	}
	if (g_IO.in(DI_SLIDE_LEFT_R2_DOORLOCK_SEN) != DEF_ON) {
		return ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK_SLIDE_LEFT_FRONT;
	}
	if (g_IO.in(DI_SLIDE_LEFT_F1_DOORLOCK_SEN) != DEF_ON) {
		return ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK_SLIDE_LEFT_REAR;
	}
	if (g_IO.in(DI_HINGED_LEFT_DOORLOCK_SEN) != DEF_ON) {
		return ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK_HINGED_LEFT;
	}

	
	if (nEqpOption > DEF_EQUIPMENT_UNIT1 )
	{
		if (g_IO.in(DI_FRONT_LEFT2_DOORLOCK) != DEF_ON) {
			return ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK_FRONT_LEFT2;
		}
	}

	BOOL bOtionDoor = g_DMCont.m_dmEQP.GB(BDM0_MD_FRONT_SLIDE_DOOR);
	if (bOtionDoor == TRUE) {
		if (g_IO.in(DI_FRONT_LEFT_DOORLOCK_SEN) != DEF_ON) {
			return ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK_FRONT_SLIDE_LEFT;
		}
		if (g_IO.in(DI_FRONT_RIGHT_DOORLOCK_SEN) != DEF_ON) {
			return ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK_FRONT_SLIDE_RIGHT;
		}
	}


	// Servo Power off 상태에서 이동 명령 금지
	if (g_IO.in(DI_SERVO_POWER_ON_SEN1) != DEF_ON) {
		return ERR_MOT_INTERLOCK_SERVO_POW_IS_NOT_ON;
	}

	if (g_IO.in(DI_MAIN_AIR1_SEN) != DEF_ON) {
		return ERR_SYS_PRESS_TEST_MAIN_IS_ALARM;
	}
	if (g_IO.in(DI_MAIN_AIR2_SEN) != DEF_ON) {
		return ERR_SYS_STACKER_TRAY_IS_ALARM;
	}

	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CTaskSystemCtrl::CTaskSystemCtrl(void)
{
	// ======= DI
	m_mapDI.SetAt( MAIN_AIR_PRESS_TEST_SEN,					"MAIN_AIR_PRESS_TEST_SEN" );
	m_mapDI.SetAt( MAIN_AIR_STACKER_CTRAY_SEN,				"MAIN_AIR_STACKER_CTRAY_SEN" );
															
	m_mapDI.SetAt(IONIZER_1_ALARM,							"IONIZER_1_ALARM");
	m_mapDI.SetAt(IONIZER_1_STATE,							"IONIZER_1_STATE");
	m_mapDI.SetAt(IONIZER_2_ALARM,							"IONIZER_2_ALARM");
	m_mapDI.SetAt(IONIZER_2_STATE,							"IONIZER_2_STATE");
	m_mapDI.SetAt(IONIZER_3_ALARM,							"IONIZER_3_ALARM");
	m_mapDI.SetAt(IONIZER_3_STATE,							"IONIZER_3_STATE");
	m_mapDI.SetAt(IONIZER_4_ALARM,							"IONIZER_4_ALARM");
	m_mapDI.SetAt(IONIZER_4_STATE,							"IONIZER_4_STATE");
	m_mapDI.SetAt(IONIZER_5_ALARM,							"IONIZER_5_ALARM");
	m_mapDI.SetAt(IONIZER_5_STATE,							"IONIZER_5_STATE");
	m_mapDI.SetAt(IONIZER_6_ALARM,							"IONIZER_6_ALARM");
	m_mapDI.SetAt(IONIZER_6_STATE,							"IONIZER_6_STATE");
	m_mapDI.SetAt(IONIZER_7_ALARM,							"IONIZER_7_ALARM");
	m_mapDI.SetAt(IONIZER_7_STATE,							"IONIZER_7_STATE");
	m_mapDI.SetAt(IONIZER_8_ALARM,							"IONIZER_8_ALARM");
	m_mapDI.SetAt(IONIZER_8_STATE,							"IONIZER_8_STATE");
	m_mapDI.SetAt(PRESS1_IONIZER_ALARM,						"PRESS1_IONIZER_ALARM");
	m_mapDI.SetAt(PRESS1_IONIZER_STATE,						"PRESS1_IONIZER_STATE");
	m_mapDI.SetAt(PRESS2_IONIZER_ALARM,						"PRESS2_IONIZER_ALARM");
	m_mapDI.SetAt(PRESS2_IONIZER_STATE,						"PRESS2_IONIZER_STATE");
	m_mapDI.SetAt(PRESS3_IONIZER_ALARM,						"PRESS3_IONIZER_ALARM");
	m_mapDI.SetAt(PRESS3_IONIZER_STATE,						"PRESS3_IONIZER_STATE");
	m_mapDI.SetAt(PRESS4_IONIZER_ALARM,						"PRESS4_IONIZER_ALARM");
	m_mapDI.SetAt(PRESS4_IONIZER_STATE,						"PRESS4_IONIZER_STATE");
	m_mapDI.SetAt(PRESS5_IONIZER_ALARM,						"PRESS5_IONIZER_ALARM");
	m_mapDI.SetAt(PRESS5_IONIZER_STATE,						"PRESS5_IONIZER_STATE");
	m_mapDI.SetAt(PRESS6_IONIZER_ALARM,						"PRESS6_IONIZER_ALARM");
	m_mapDI.SetAt(PRESS6_IONIZER_STATE,						"PRESS6_IONIZER_STATE");
	m_mapDI.SetAt(PRESS7_IONIZER_ALARM,						"PRESS7_IONIZER_ALARM");
	m_mapDI.SetAt(PRESS7_IONIZER_STATE,						"PRESS7_IONIZER_STATE");
	m_mapDI.SetAt(PRESS8_IONIZER_ALARM,						"PRESS8_IONIZER_ALARM");
	m_mapDI.SetAt(PRESS8_IONIZER_STATE,						"PRESS8_IONIZER_STATE");

	m_mapDI.SetAt(TEST1_PRESS1_SERVO_CP_TRIP_ALARM_SEN,		"TEST1_PRESS1_SERVO_CP_TRIP_ALARM_SEN");
	m_mapDI.SetAt(LOAD_UNLOAD_TRANSFER_SERVO_CP_TRIP_SEN,	"LOAD_UNLOAD_TRANSFER_SERVO_CP_TRIP_SEN");
	m_mapDI.SetAt(TABLE_SERVO_CP_TRIP_SEN,					"TABLE_SERVO_CP_TRIP_SEN");
	m_mapDI.SetAt(TEST2_PRESS2_CP_TRIP_SEN,					"TEST2_PRESS2_CP_TRIP_SEN");

															
	m_mapDI.SetAt( START_SW_SEN1,							"START_SW_SEN1" );
	m_mapDI.SetAt( START_SW_SEN2,							"START_SW_SEN2" );
	m_mapDI.SetAt( STOP_SW_SEN,								"STOP_SW_SEN" );
	m_mapDI.SetAt( COVER_SW_SEN,							"COVER_SW_SEN" );
	m_mapDI.SetAt( SOUND_SW_SEN,							"SOUND_SW_SEN" );
	m_mapDI.SetAt( LAMP_SW_SEN,								"LAMP_SW_SEN" );
	m_mapDI.SetAt(TRAY_FEEDER_STEP_SW,						"TRAY_FEEDER_STEP_SW" );
	m_mapDI.SetAt(TRAY_LOADER_HAND_STEP_SW,					"TRAY_LOADER_HAND_STEP_SW");
	m_mapDI.SetAt(TRAY_UNLOADER_HAND_STEP_SW,				"TRAY_UNLOADER_HAND_STEP_SW");
															
	m_mapDI.SetAt(FRONT_LEFT_DOORLOCK_SEN,					"FRONT_LEFT_DOORLOCK_SEN" );
	m_mapDI.SetAt( FRONT_RIGHT_DOORLOCK_SEN,				"FRONT_RIGHT_DOORLOCK_SEN" );
	m_mapDI.SetAt(HINGED_RIGHT_DOORLOCK_SEN,				"HINGED_RIGHT_DOORLOCK_SEN" );
	m_mapDI.SetAt(SLIDE_RIGHT_F1_DOORLOCK_SEN,				"SLIDE_RIGHT_F1_DOORLOCK_SEN" );
	m_mapDI.SetAt(SLIDE_RIGHT_R2_DOORLOCK_SEN,				"SLIDE_RIGHT_R2_DOORLOCK_SEN" );
	m_mapDI.SetAt(REAR_RIGHT_DOORLOCK_SEN,					"REAR_RIGHT_DOORLOCK_SEN" );
	m_mapDI.SetAt(REAR_LEFT_DOORLOCK_SEN,					"REAR_LEFT_DOORLOCK_SEN" );
	m_mapDI.SetAt(SLIDE_LEFT_R2_DOORLOCK_SEN,				"SLIDE_LEFT_R2_DOORLOCK_SEN" );
	m_mapDI.SetAt(SLIDE_LEFT_F1_DOORLOCK_SEN,				"SLIDE_LEFT_F1_DOORLOCK_SEN");
	m_mapDI.SetAt(HINGED_LEFT_DOORLOCK_SEN,					"HINGED_LEFT_DOORLOCK_SEN");
	m_mapDI.SetAt(FRONT_LEFT2_DOORLOCK_SEN,					"FRONT_LEFT2_DOORLOCK_SEN");
	

	m_mapDI.SetAt(REAR_LEFT_EMO_SW_SEN,							"REAR_LEFT_EMO_SW_SEN" );
	m_mapDI.SetAt(FRONT_LEFT_EMO_SW_SEN,					"FRONT_LEFT_EMO_SW_SEN" );
	m_mapDI.SetAt(FRONT_RIGHT_EMO_SW_SEN,					"FRONT_RIGHT_EMO_SW_SEN" );
	m_mapDI.SetAt(REAR_RIGHT_EMO_SW_SEN,						"REAR_RIGHT_EMO_SW_SEN" );

	m_mapDI.SetAt(MIDDLE_LEFT_EMO_SW_SEN,					"MIDDLE_LEFT_EMO_SW_SEN"); 	//4호기 부터 적용
	m_mapDI.SetAt(MIDDLE_RIGHT_EMO_SW_SEN,					"MIDDLE_RIGHT_EMO_SW_SEN");	//4호기 부터 적용
			
	m_mapDI.SetAt(MAIN_POWER_ON_SEN1,						"MAIN_POWER_ON_SEN1" );
	m_mapDI.SetAt(MAIN_POWER_ON_SEN2,						"MAIN_POWER_ON_SEN2" );
	m_mapDI.SetAt(POWER_BOX_CP_TRIP_ALARM_SEN,				"POWER_BOX_CP_TRIP_ALARM_SEN" );
	m_mapDI.SetAt(ALL_DOOR_LOCK_SEN,						"ALL_DOOR_LOCK_SEN" );	// 닫히면 on / 열리면 off
	m_mapDI.SetAt(SERVO_POWER_ON_SEN1,						"SERVO_POWER_ON_SEN" );
	m_mapDI.SetAt(AC_FAN_CP_TRIP_SEN,						"AC_FAN_CP_TRIP_SEN");

	m_mapDI.SetAt(REAR_LEFT_START_SW_SEN1,					"REAR_LEFT_START_SW_SEN1" );
	m_mapDI.SetAt(REAR_LEFT_START_SW_SEN2,					"REAR_LEFT_START_SW_SEN2" );
	m_mapDI.SetAt(REAR_LEFT_STOP_SW_SEN,					"REAR_LEFT_STOP_SW_SEN" );
	m_mapDI.SetAt(REAR_LEFT_COVER_SW_SEN,					"REAR_LEFT_COVER_SW_SEN" );
	m_mapDI.SetAt(REAR_LEFT_SOUND_SW_SEN,					"REAR_LEFT_SOUND_SW_SEN" );
	m_mapDI.SetAt(REAR_LEFT_LAMP_SW_SEN,					"REAR_LEFT_LAMP_SW_SEN" );
	m_mapDI.SetAt(SELECT_FRONT_SW_SEN,						"SELECT_FRONT_SW_SEN" );
	m_mapDI.SetAt(SELECT_REAR_SW_SEN,						"SELECT_REAR_SW_SEN" );
	m_mapDI.SetAt(REAR_RIGHT_START_SW_SEN1,					"REAR_RIGHT_START_SW_SEN1");
	m_mapDI.SetAt(REAR_RIGHT_START_SW_SEN2,					"REAR_RIGHT_START_SW_SEN2");
	m_mapDI.SetAt(REAR_RIGHT_STOP_SW_SEN,					"REAR_RIGHT_STOP_SW_SEN");
	m_mapDI.SetAt(REAR_RIGHT_COVER_SW_SEN,					"REAR_RIGHT_COVER_SW_SEN");
	m_mapDI.SetAt(TEST_HAND1_STEP_SW,						"TEST_HAND1_STEP_SW");
	m_mapDI.SetAt(TEST_HAND2_STEP_SW,						"TEST_HAND2_STEP_SW");
	

	// ======= DO
	m_mapDO.SetAt( START_SW_LAMP,					"START_SW_LAMP" );
	m_mapDO.SetAt( STOP_SW_LAMP,					"STOP_SW_LAMP" );
	m_mapDO.SetAt( COVER_SW_LAMP,					"COVER_SW_LAMP" );
	m_mapDO.SetAt( SOUND_SW_LAMP,					"SOUND_SW_LAMP" );
	m_mapDO.SetAt( LAMP_SW_LAMP,					"LAMP_SW_LAMP" );
	m_mapDO.SetAt(TRAY_FEEDER_STEP_SW_LAMP,			"TRAY_FEEDER_STEP_SW_LAMP");
	m_mapDO.SetAt(TRAY_LOADER_HAND_STEP_SW_LAMP,	"TRAY_LOADER_HAND_STEP_SW_LAMP");
	m_mapDO.SetAt(TRAY_UNLOADER_HAND_STEP_SW_LAMP,	"TRAY_UNLOADER_HAND_STEP_SW_LAMP");
												
	m_mapDO.SetAt(FRONT_LEFT_DOORLOCK,				"FRONT_LEFT_DOORLOCK" );
	m_mapDO.SetAt(FRONT_RIGHT_DOORLOCK,				"FRONT_RIGHT_DOORLOCK" );
	m_mapDO.SetAt(HINGED_RIGHT_DOORLOCK,			"HINGED_RIGHT_DOORLOCK" );
	m_mapDO.SetAt(SLIDE_RIGHT_F1_DOORLOCK,			"SLIDE_RIGHT_F1_DOORLOCK" );
	m_mapDO.SetAt(SLIDE_RIGHT_R2_DOORLOCK,			"SLIDE_RIGHT_R2_DOORLOCK" );

	m_mapDO.SetAt(REAR_RIGHT_DOORLOCK,				"REAR_RIGHT_DOORLOCK" );
	m_mapDO.SetAt(REAR_LEFT_DOORLOCK,				"REAR_LEFT_DOORLOCK" );
	m_mapDO.SetAt(SLIDE_LEFT_R2_DOORLOCK,			"SLIDE_LEFT_R2_DOORLOCK" );
	m_mapDO.SetAt(SLIDE_LEFT_F1_DOORLOCK,			"SLIDE_LEFT_F1_DOORLOCK");
	m_mapDO.SetAt(HINGED_LEFT_DOORLOCK,				"HINGED_LEFT_DOORLOCK");

	m_mapDO.SetAt( FRONT_LAMP_ON,					"FRONT_LAMP_ON" );
	m_mapDO.SetAt( TEST_SITE_LAMP_ON,				"TEST_SITE_LAMP_ON" );
	m_mapDO.SetAt( COVER_RESET,						"COVER_RESET" );
	m_mapDO.SetAt( SERVO_POWER_ON,					"SERVO_POWER_ON" );
													
	m_mapDO.SetAt( REAR_LEFT_START_SW_LAMP,			"REAR_LEFT_START_SW_LAMP" );
	m_mapDO.SetAt( REAR_LEFT_STOP_SW_LAMP,			"REAR_LEFT_STOP_SW_LAMP" );
	m_mapDO.SetAt( REAR_LEFT_COVER_SW_LAMP,			"REAR_LEFT_COVER_SW_LAMP" );
	m_mapDO.SetAt( REAR_LEFT_SOUND_SW_LAMP,			"REAR_LEFT_SOUND_SW_LAMP" );
	m_mapDO.SetAt( REAR_LEFT_LAMP_SW_LAMP,			"REAR_LEFT_LAMP_SW_LAMP" );

	m_mapDO.SetAt( REAR_RIGHT_START_SW_LAMP,		"REAR_RIGHT_START_SW_LAMP" );
	m_mapDO.SetAt( REAR_RIGHT_STOP_SW_LAMP,			"REAR_RIGHT_STOP_SW_LAMP" );
	m_mapDO.SetAt( REAR_RIGHT_COVER_SW_LAMP,		"REAR_RIGHT_COVER_SW_LAMP" );
	m_mapDO.SetAt( REAR_RIGHT_SOUND_SW_LAMP,		"REAR_RIGHT_SOUND_SW_LAMP");
	m_mapDO.SetAt( REAR_RIGHT_LAMP_SW_LAMP,			"REAR_RIGHT_LAMP_SW_LAMP");

	m_mapDO.SetAt( TEST_HAND1_STEP_SW_LAMP,			"TEST_HAND1_STEP_SW_LAMP" );
	m_mapDO.SetAt( TEST_HAND2_STEP_SW_LAMP,			"TEST_HAND2_STEP_SW_LAMP" );
												
	m_mapDO.SetAt( TOWER_RED,						"TOWER_RED" );
	m_mapDO.SetAt( TOWER_YELLOW,					"TOWER_YELLOW" );
	m_mapDO.SetAt( TOWER_GREEN,						"TOWER_GREEN" );
													
	m_mapDO.SetAt( BUZZER1,							"BUZZER1" );
	m_mapDO.SetAt( BUZZER2,							"BUZZER2" );
	m_mapDO.SetAt( BUZZER3,							"BUZZER3" );
	m_mapDO.SetAt( BUZZER4,							"BUZZER4" );

	m_intList.RemoveAll();

	m_dTotalStopTm     = 0.0;
	m_dTotalRunTm      = 0.0;
	m_nRecSortCnt      = 0;
	m_dBestTestTime    = 0.0;
	m_bAutoGrrUse = FALSE;
	m_nAutoHeightCalSelect = DEF_AUTO_HEIGHT_CAL_UNK;
	
	for(int i=0; i<eMaxTestPPCount; i++)
		m_nErrTestPpIdx[i] = -1;

    for(int i=0; i<eMaxPressUnitCount; i++){
        m_nCleanIntervalCnt[i] = 0;
		m_tIndexTime[i].SetTimerCheckEnd();
    }

	m_nRetryRegulator = 0;
	m_bFrontMode      = -1;
	m_nLotEndTrayEndFeed = DEF_LOTEND_UNK;

	m_vModuleStationNo[eTestPP_1].push_back(ePressUnit_Down_1);
	m_vModuleStationNo[eTestPP_1].push_back(ePressUnit_Down_2);
	m_vModuleStationNo[eTestPP_1].push_back(ePressUnit_Down_3);
	m_vModuleStationNo[eTestPP_1].push_back(ePressUnit_Down_4);

	m_vModuleStationNo[eTestPP_2].push_back(ePressUnit_Down_5);
	m_vModuleStationNo[eTestPP_2].push_back(ePressUnit_Down_6);
	m_vModuleStationNo[eTestPP_2].push_back(ePressUnit_Down_7);
	m_vModuleStationNo[eTestPP_2].push_back(ePressUnit_Down_8);

	m_SocketCountCnt.clear();
	memset(m_nSiteOnOff, 0x00, sizeof(m_nSiteOnOff));

	m_dqDailyData.clear();
	m_vDisConnectionStationIdx.clear();

	m_2DBarcodeUse = FALSE;
	m_bAutoLoopUse = FALSE;

	memset(m_nSiteSocketOnOff, 0x00, sizeof(m_nSiteSocketOnOff));

	m_strLastDuplicate = _T("");
	m_nWM3ReconnectSts = 0;
	m_bAutoQAUse = FALSE;
	m_nZPickerAxisHeightCalSelect = DEF_ZAXIS_PICKER_HEIGHT_CAL_UNK;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CTaskSystemCtrl::~CTaskSystemCtrl(void)
{
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskSystemCtrl::Initialize(char* szDriverFileName)
{
	CConfigData ConfigData(szDriverFileName);
	char szSection[128] = {0,};

	BOOL bOtionDoor = g_DMCont.m_dmEQP.GB(BDM0_MD_FRONT_SLIDE_DOOR);

	// Digital Input
	CString strName;
	memset(szSection, 0x00, sizeof(szSection));
	sprintf_s(szSection, sizeof(szSection), "TASK SYSTEM CONTROL CONFIG - DI");
	int nDiCount = m_mapDI.GetCount();
	for(int i=0; i<nDiCount; i++){
		char szKey[128] = {0,};
		strName = _T("");
		m_mapDI.Lookup(i, strName);
		sprintf_s(szKey, sizeof(szKey), "%s", strName.GetString() );
		int nIdx = ConfigData.GetInt(szSection, szKey, "0");
		m_vX.push_back(nIdx);
	}

	// Digital Output
	memset(szSection, 0x00, sizeof(szSection));
	sprintf_s(szSection, sizeof(szSection), "TASK SYSTEM CONTROL CONFIG - DO");
	int nDoCount = m_mapDO.GetCount();
	for(int i=0; i<nDoCount; i++){		
		char szKey[128] = {0,};
		strName = _T("");
		m_mapDO.Lookup(i, strName);
		sprintf_s(szKey, sizeof(szKey), "%s", strName.GetString() );
		int nIdx = ConfigData.GetInt(szSection, szKey, "0");
		m_vY.push_back(nIdx);
	}

	g_COVER_SW.Initialize();

	// Start Switch // 0, 0
	g_START_SW.OPSwitchSet(_T("START"),m_vX[ START_SW_SEN2 ], m_vY[ START_SW_LAMP ], eOP_KEEP);
	// g_START.Add_In(m_vX[xSYSCTRL_START_SW_2]); // TODO SLT : SW2은 B접점임. 추후 구현.
// 	g_START_SW.Add_In(m_vX[xSYSCTRL_REAR_START_SW_1]); //416
// 	g_START_SW.Add_Out(m_vY[ySYSCTRL_REAR_START_SW_LAMP]); // 400
	g_START_SW.SetGroupSw(&g_STOP_SW);

	// Stop Switch
	g_STOP_SW.OPSwitchSet     (_T("STOP"),m_vX[ STOP_SW_SEN ], m_vY[ STOP_SW_LAMP ] , eOP_KEEP);
//	g_STOP_SW.Add_In(m_vX[xSYSCTRL_REAR_STOP_SW_1]);
//	g_STOP_SW.Add_Out(m_vY[ySYSCTRL_REAR_STOP_SW_LAMP]);
	g_STOP_SW.SetGroupSw(&g_START_SW);

	// Lamp Switch
	g_LAMP_SW.OPSwitchSet(_T("LAMP"),m_vX[ LAMP_SW_SEN ] , m_vY[ LAMP_SW_LAMP ] , eOP_TOGGLE, FALSE,  COPSwitchAct::LED_A);

	// Sound Switch
	g_SOUND_SW.OPSwitchSet(_T("SOUND"),m_vX[ SOUND_SW_SEN ] , m_vY[ SOUND_SW_LAMP ] , eOP_NORMAL);
 	g_SOUND_SW.Add_In(DI_REAR_LEFT_SOUND_SW_SEN);
	g_SOUND_SW.Add_In(DI_REAR_RIGHT_SOUND_SW_SEN);
 	g_SOUND_SW.Add_Out(DO_REAR_LEFT_SOUND_SW_SEN);
	g_SOUND_SW.Add_Out(DO_REAR_RIGHT_SOUND_SW_LAMP);

	// Cover Switch
	g_COVER_SW.OPSwitchSet(_T("COVER"), m_vX[ COVER_SW_SEN ] , m_vY[ COVER_SW_LAMP ] , eOP_TOGGLE, FALSE, COPSwitchAct::LED_B);

	// Step Switch
	g_StepTrayPP_SW[eTrayPP_1_LD].OPSwitchSet(_T("STEP_TRAY1"),m_vX[ TRAY_LOADER_HAND_STEP_SW ] , m_vY[TRAY_LOADER_HAND_STEP_SW_LAMP ] , eOP_TOGGLE, FALSE, COPSwitchAct::LED_A);
	g_StepTrayPP_SW[eTrayPP_2_UD].OPSwitchSet(_T("STEP_TRAY2"), m_vX[TRAY_UNLOADER_HAND_STEP_SW], m_vY[TRAY_UNLOADER_HAND_STEP_SW_LAMP], eOP_TOGGLE, FALSE, COPSwitchAct::LED_A);

	g_StepTestPP_SW[eTestPP_1].OPSwitchSet(_T("STEP_TESTPP1"),m_vX[ TEST_HAND1_STEP_SW ] , m_vY[ TEST_HAND1_STEP_SW_LAMP ] , eOP_TOGGLE, FALSE, COPSwitchAct::LED_A, eTestPP_1);
	g_StepTestPP_SW[eTestPP_2].OPSwitchSet(_T("STEP_TESTPP2"),m_vX[ TEST_HAND2_STEP_SW ] , m_vY[ TEST_HAND2_STEP_SW_LAMP ] , eOP_TOGGLE, FALSE, COPSwitchAct::LED_A, eTestPP_2);

	g_Feeder_SW.OPSwitchSet(_T("FEEDER"), m_vX[TRAY_FEEDER_STEP_SW], m_vY[TRAY_FEEDER_STEP_SW_LAMP], eOP_TOGGLE, FALSE, COPSwitchAct::LED_A);

	int nEqpOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_EQP_No);
	// emo     
	m_vEMO_Sig.push_back(m_vX[REAR_LEFT_EMO_SW_SEN]);
	m_vEMO_Sig.push_back(m_vX[ FRONT_LEFT_EMO_SW_SEN ]);
	m_vEMO_Sig.push_back(m_vX[ FRONT_RIGHT_EMO_SW_SEN ]);
	m_vEMO_Sig.push_back(m_vX[REAR_RIGHT_EMO_SW_SEN]);

	if (nEqpOption >= DEF_EQUIPMENT_UNIT4_N && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
	{
		m_vEMO_Sig.push_back(m_vX[MIDDLE_LEFT_EMO_SW_SEN]);
		m_vEMO_Sig.push_back(m_vX[MIDDLE_RIGHT_EMO_SW_SEN]);
	}
	// main power
	m_vMainPow_Sig.push_back(m_vX[MAIN_POWER_ON_SEN1]);
	m_vMainPow_Sig.push_back(m_vX[MAIN_POWER_ON_SEN2]);
	// servo power
	m_vServoPow_Sig.push_back(m_vX[ SERVO_POWER_ON_SEN1]);


	// Ionizer Alarm	
	m_vIonAlarm_Sig.push_back( m_vX[IONIZER_1_STATE] );
	m_vIonAlarm_Sig.push_back( m_vX[IONIZER_2_STATE] );
	m_vIonAlarm_Sig.push_back( m_vX[IONIZER_3_STATE] );
	m_vIonAlarm_Sig.push_back( m_vX[IONIZER_4_STATE] );
	m_vIonAlarm_Sig.push_back( m_vX[IONIZER_5_STATE] );
	m_vIonAlarm_Sig.push_back( m_vX[IONIZER_6_STATE] );
	m_vIonAlarm_Sig.push_back( m_vX[IONIZER_7_STATE] );
	m_vIonAlarm_Sig.push_back( m_vX[IONIZER_8_STATE] );
	m_vIonAlarm_Sig.push_back( m_vX[PRESS1_IONIZER_STATE] );
	m_vIonAlarm_Sig.push_back( m_vX[PRESS2_IONIZER_STATE] );
	m_vIonAlarm_Sig.push_back( m_vX[PRESS3_IONIZER_STATE] );
	m_vIonAlarm_Sig.push_back( m_vX[PRESS4_IONIZER_STATE] );
	m_vIonAlarm_Sig.push_back( m_vX[PRESS5_IONIZER_STATE] );
	m_vIonAlarm_Sig.push_back( m_vX[PRESS6_IONIZER_STATE] );
	m_vIonAlarm_Sig.push_back( m_vX[PRESS7_IONIZER_STATE] );
	m_vIonAlarm_Sig.push_back( m_vX[PRESS8_IONIZER_STATE] );
	
	// Ionizer Fan Alarm
	m_vIonFanAlarm_Sig.push_back(m_vX[IONIZER_1_ALARM]);
	m_vIonFanAlarm_Sig.push_back(m_vX[IONIZER_2_ALARM]);
	m_vIonFanAlarm_Sig.push_back(m_vX[IONIZER_3_ALARM]);
	m_vIonFanAlarm_Sig.push_back(m_vX[IONIZER_4_ALARM]);
	m_vIonFanAlarm_Sig.push_back(m_vX[IONIZER_5_ALARM]);
	m_vIonFanAlarm_Sig.push_back(m_vX[IONIZER_6_ALARM]);
	m_vIonFanAlarm_Sig.push_back(m_vX[IONIZER_7_ALARM]);
	m_vIonFanAlarm_Sig.push_back(m_vX[IONIZER_8_ALARM]);
	m_vIonFanAlarm_Sig.push_back(m_vX[PRESS1_IONIZER_ALARM]);
	m_vIonFanAlarm_Sig.push_back(m_vX[PRESS2_IONIZER_ALARM]);
	m_vIonFanAlarm_Sig.push_back(m_vX[PRESS3_IONIZER_ALARM]);
	m_vIonFanAlarm_Sig.push_back(m_vX[PRESS4_IONIZER_ALARM]);
	m_vIonFanAlarm_Sig.push_back(m_vX[PRESS5_IONIZER_ALARM]);
	m_vIonFanAlarm_Sig.push_back(m_vX[PRESS6_IONIZER_ALARM]);
	m_vIonFanAlarm_Sig.push_back(m_vX[PRESS7_IONIZER_ALARM]);
	m_vIonFanAlarm_Sig.push_back(m_vX[PRESS8_IONIZER_ALARM]);

	
	// main air
	m_vMainAir_PressTest_Sig.push_back(m_vX[MAIN_AIR_PRESS_TEST_SEN]);
	m_vMainAir_StackerCtray_Sig.push_back(m_vX[MAIN_AIR_STACKER_CTRAY_SEN]);

	// CP Trip
	m_vCPTrip_Sig.push_back( m_vX[TEST1_PRESS1_SERVO_CP_TRIP_ALARM_SEN] );
	m_vCPTrip_Sig.push_back( m_vX[LOAD_UNLOAD_TRANSFER_SERVO_CP_TRIP_SEN] );
	m_vCPTrip_Sig.push_back( m_vX[TABLE_SERVO_CP_TRIP_SEN] );
	m_vCPTrip_Sig.push_back( m_vX[TEST2_PRESS2_CP_TRIP_SEN] );
	m_vCPTrip_Sig.push_back(m_vX[MAIN_POWER_ON_SEN1]);
	m_vCPTrip_Sig.push_back(m_vX[MAIN_POWER_ON_SEN2]);
	m_vCPTrip_Sig.push_back(m_vX[POWER_BOX_CP_TRIP_ALARM_SEN]);
	m_vCPTrip_Sig.push_back(m_vX[AC_FAN_CP_TRIP_SEN]);
	//m_vCPTrip_Sig.push_back(m_vX[SERVO_POWER_ON_SEN1]);

	// Down Door
	m_vDownDoor_Sig.push_back( m_vX[ FRONT_LEFT_DOORLOCK_SEN ] );
	m_vDownDoor_Sig.push_back( m_vX[ FRONT_RIGHT_DOORLOCK_SEN ] );
	m_vDownDoor_Sig.push_back( m_vX[HINGED_RIGHT_DOORLOCK_SEN] );
	m_vDownDoor_Sig.push_back( m_vX[SLIDE_RIGHT_F1_DOORLOCK_SEN] );
	m_vDownDoor_Sig.push_back( m_vX[SLIDE_RIGHT_R2_DOORLOCK_SEN] );
	m_vDownDoor_Sig.push_back( m_vX[REAR_RIGHT_DOORLOCK_SEN] );
	m_vDownDoor_Sig.push_back( m_vX[REAR_LEFT_DOORLOCK_SEN] );
	m_vDownDoor_Sig.push_back( m_vX[SLIDE_LEFT_R2_DOORLOCK_SEN] );
	m_vDownDoor_Sig.push_back(m_vX[SLIDE_LEFT_F1_DOORLOCK_SEN]);
	m_vDownDoor_Sig.push_back(m_vX[HINGED_LEFT_DOORLOCK_SEN]);
	
	if (nEqpOption > DEF_EQUIPMENT_UNIT1 && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
	{
		m_vDownDoor_Sig.push_back(m_vX[FRONT_LEFT2_DOORLOCK_SEN]);
	}

	m_vVisionBoardReset_Sig.push_back(DO_VISION_BD1_RESET);
	m_vVisionBoardReset_Sig.push_back(DO_VISION_BD2_RESET);
	m_vVisionBoardReset_Sig.push_back(DO_VISION_BD3_RESET);
	m_vVisionBoardReset_Sig.push_back(DO_VISION_BD4_RESET);
	m_vVisionBoardReset_Sig.push_back(DO_VISION_BD5_RESET);
	m_vVisionBoardReset_Sig.push_back(DO_VISION_BD6_RESET);
	m_vVisionBoardReset_Sig.push_back(DO_VISION_BD7_RESET);

	// Signal Tower
	g_TowerR.OPSwitchSet   (_T("TOWER R"), -1 , m_vY[ TOWER_RED ] , eOP_TOGGLE);
	g_TowerY.OPSwitchSet   (_T("TOWER Y"), -1 , m_vY[ TOWER_YELLOW ] , eOP_TOGGLE);
	g_TowerG.OPSwitchSet   (_T("TOWER G"), -1 , m_vY[ TOWER_GREEN ] , eOP_TOGGLE);
	g_TowerBuz.OPSwitchSet (_T("BUZZER"), -1 , -1 , eOP_NORMAL);

 	g_START_SW.Run();
 	g_STOP_SW.Run();

	g_START_SW.SetStatus(DEF_OFF);
 	g_STOP_SW.SetStatus(DEF_ON);

	g_LAMP_SW.Run();
	//g_LAMP_SW.SetStatus(DEF_OFF);
 
	g_SOUND_SW.Run();
	//g_SOUND_SW.SetStatus(DEF_ON);

 	g_COVER_SW.Run();
	for (int i = 0; i<eMaxTrayPPCount; i++)
		g_StepTrayPP_SW[i].Run();
	for( int i=0; i<eMaxTestPPCount; i++ )
		g_StepTestPP_SW[i].Run();

	g_Feeder_SW.Run();

	g_TowerR.Run();
 	g_TowerY.Run();
 	g_TowerG.Run();
 	g_TowerBuz.Run();

	// Get signal tower data from Shared memory
	SetSignalTowerDataFromSM();
	//g_TaskSystemCtrl.ActionTowerAndSound(eSIGNAL_PAUSE);
	//ActionTowerAndSound(eSIGNAL_PAUSE);

	//g_VatHW.Load();

	CreateThread(1);
	CreateThread(2);
	CreateThread(3);

    g_DMCont.m_dmEQP.SB(BDM0_ONECYCLE_HISTORY, TRUE);
	SetOneCycleMode( DEF_OFF );
	SetCleanDeviceSkipOneCycleMode(DEF_OFF);


	m_VisionBoardReset.SetCheckTime(3.0);
	m_VisionBoardReset.StartTimer();

	return CBaseTaskManager::Initialize();
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CTaskSystemCtrl::Finalize()
{
	g_START_SW.Stop();
	g_STOP_SW.Stop();
	g_LAMP_SW.Stop();
	g_SOUND_SW.Stop();
	g_COVER_SW.Stop();
	g_TowerR.Stop();
	g_TowerY.Stop();
	g_TowerG.Stop();
	g_TowerBuz.Stop();
	for (int i = 0; i<eMaxTrayPPCount; i++)
		g_StepTrayPP_SW[i].Stop();
	for( int i=0; i<eMaxTestPPCount; i++ )
		g_StepTestPP_SW[i].Stop();

	g_Feeder_SW.Stop();

	int nMotorCount = g_Motor.GetAxisCount();
	for(int i=0; i<nMotorCount; i++)
	{
		g_Motor[i]->AlarmReset();
		Sleep(1);
		g_Motor[i]->ServoOff();
		Sleep(10);
	}
	Sleep(100);

	g_IO.out(m_vY[ SERVO_POWER_ON ], DEF_OFF);

	ClearLotInfo();

	CBaseTaskManager::Finalize();
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskSystemCtrl::OnEvent(const CBaseTaskManager* pSender,int nEventId, CParamList* pDataList/*=NULL*/)
{
    BOOL bEventRpt = FALSE;
	switch( nEventId )
	{
    case CEID_SOUND_SW_DOWN : { On_Sound_Switch_Down(pDataList); bEventRpt = TRUE; } break;
    case CEID_SOUND_SW_UP   : { On_Sound_Switch_Up(pDataList);   bEventRpt = TRUE; } break;
    case CEID_START_SW_ON   : { On_Start_Switch_On(pDataList);   bEventRpt = TRUE; } break;
    case CEID_STOP_SW_ON    : { On_Stop_Switch_On(pDataList);    bEventRpt = TRUE; } break;
	}

    if(bEventRpt == TRUE){
        g_GuiComm.Send_EventReport(nEventId);
    }

	return ERR_NO_ERROR;
}

void CTaskSystemCtrl::GetSiteSocketStatusFromDM()
{
	ZeroMemory(m_nSiteSocketOnOff, sizeof(m_nSiteSocketOnOff));

	int nTestSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nTestSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	for( int nPressIdx=0; nPressIdx < eMaxPressUnitCount; nPressIdx++ )
	{
		for( int nY=0; nY < STATION_MAX_Y_SITE; nY++ )
		{
			for( int nX=0; nX < STATION_MAX_X_SITE; nX++ )
			{
				int nAdrr = NDM11_SocketOnOff_Site1_1 + (nPressIdx*STATION_MAX_PARA) + nX + (nY*STATION_MAX_X_SITE);
				if( nX < nTestSiteX && nY < nTestSiteY )
				{					
					m_nSiteSocketOnOff[nPressIdx][nY][nX] = g_DMCont.m_dmHandlerOpt.GN(nAdrr);
				}else{
					m_nSiteSocketOnOff[nPressIdx][nY][nX] = DEF_SITE_OFF;
					g_DMCont.m_dmHandlerOpt.SN(nAdrr, DEF_SITE_OFF);
				}
			}
		}
	}
}

BOOL CTaskSystemCtrl::ChkAllStationOFF()
{
	int nTestSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nTestSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	for( int nPressIdx=0; nPressIdx < eMaxPressUnitCount; nPressIdx++ )
	{
		for( int nY=0; nY < nTestSiteY; nY++ )
		{
			for( int nX=0; nX < nTestSiteX; nX++ )
			{
				if (m_nSiteSocketOnOff[nPressIdx][nY][nX] == DEF_ON)
				{
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

int CTaskSystemCtrl::ChkSocketOnOffInterlock(CParamList &AlarmData)
{
	GetSiteSocketStatusFromDM();
	AlarmData.m_fnClear();
	
	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);


	int nTestSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nTestSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	// 전체 site socket off 확인
	if (ChkAllStationOFF() == TRUE)
		return ERR_CAN_NOT_START_ALL_SOCKET_OFF;
	
	CString strErrMsg;
	CString strErrStation = _T("");
	std::vector<int> vErrEachOneSock;
	std::vector<int> vDownSocket;
	std::vector<int> vUpSocket;
	int nHalf_Site = eMaxATS_Area / 2;

	for( int nTotal_cnt = 0; nTotal_cnt < eMaxTestPPCount; nTotal_cnt++ )
	{
		vDownSocket.clear();
		vUpSocket.clear();

		strErrStation.Format(_T("%d, %d, %d, %d, %d, %d, %d, %d"), 
			(nTotal_cnt * nHalf_Site ) + 1,
			(nTotal_cnt * nHalf_Site ) + 2,
			(nTotal_cnt * nHalf_Site ) + 3,
			(nTotal_cnt * nHalf_Site ) + 4,
			(nTotal_cnt * nHalf_Site ) + 9,
			(nTotal_cnt * nHalf_Site ) + 10,
			(nTotal_cnt * nHalf_Site ) + 11,
			(nTotal_cnt * nHalf_Site ) + 12 );

		for( int nStation = 0; nStation < nHalf_Site; nStation++ ) {
			for( int nRow = 0; nRow < nTestSiteY; nRow++) {
				for (int nCol = 0; nCol < nTestSiteX; nCol++) {
					vUpSocket.push_back( m_nSiteSocketOnOff  [ nStation + (nTotal_cnt * nHalf_Site ) + (eMaxPressUnitCount/2) ][nRow][nCol]);
					vDownSocket.push_back( m_nSiteSocketOnOff[ nStation + (nTotal_cnt * nHalf_Site ) ][nRow][nCol]);
				}
			}
		}

		if (Chk8StationSocketOff(vDownSocket, vUpSocket) == FALSE) //8개 Station이 OFF 라면 FALSE 검사 시작.
		{
			if (ChkEachOneMoreSocketOn(vDownSocket, vUpSocket) == TRUE) //8개 Station Sokcet이 1개이상 씩 켜져 있다면 TRUE 아니라면 FALSE
			{
				if (ChkRetestTwoMoreStaionOn(vDownSocket, vUpSocket) == FALSE) {
 					strErrMsg.Format(_T("Please turn on more than 2 stations in retest mode.\r\nError Station : %s"), (LPCTSTR)strErrStation);
 					AlarmData.m_fnAddItemFormat("%s", strErrMsg);
	 				//return ERR_CAN_NOT_START_NEED_TWO_MORE_STATION_IN_RETEST;
				}
			}
			else {
				ChkErrEachOneMoreSocketOn(vDownSocket, vUpSocket, vErrEachOneSock);
	 				strErrMsg.Format(_T("Please turn on at least one socket with 8 stations.\r\nError Station : %s\r\nError Socket : "), (LPCTSTR)strErrStation);
	 				for (size_t k = 0; k < vErrEachOneSock.size(); k++) {
	 					CString temp = _T("");
	 					if (k != vErrEachOneSock.size() - 1)
	 						temp.Format(_T("%d, "), vErrEachOneSock[k]);
	 					else
	 						temp.Format(_T("%d"), vErrEachOneSock[k]);
	 					strErrMsg += temp;
	 				}
	 				
	 				AlarmData.m_fnAddItemFormat("Error Station : %s", strErrMsg);
	 				//return ERR_CAN_NOT_START_NEED_EACH_ONE_SOCKET_IN_8STATION;
			}
		}
	}

	vDownSocket.clear();
	vUpSocket.clear();

	return ERR_NO_ERROR;
}

BOOL CTaskSystemCtrl::Chk8StationSocketOff(std::vector<int> vDown, std::vector<int> vUp)
{
	int nSum = 0;
	for (size_t i = 0; i < vDown.size(); i++)
	{
		if (vDown[i] == DEF_ON) { nSum++; }
		if (vUp[i] == DEF_ON) { nSum++; }
	}

	return nSum == 0 ? TRUE : FALSE;

	
}

void CTaskSystemCtrl::ChkEachOneMoreSocketState(int _Downstation,int _Upstation, int& _Socket_number)
{
	if (_Downstation == DEF_ON) { _Socket_number++; }
	if (_Upstation == DEF_ON) { _Socket_number++; }
}

BOOL CTaskSystemCtrl::ChkEachOneMoreSocketOn(std::vector<int> vDown, std::vector<int> vUp)
{
	int nTestSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nTestSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	int szSock[STATION_MAX_PARA] = {0,};

	int nSocketOnCount = 0;
	int nStation_Max_para = nTestSiteX * nTestSiteY;

	for ( size_t i = 0; i < vDown.size(); i++) {
		switch (i % nStation_Max_para)
		{
		case 0:	ChkEachOneMoreSocketState( vDown[i], vUp[i],  szSock[0] );	break;
		case 1:	ChkEachOneMoreSocketState( vDown[i], vUp[i],  szSock[1] );	break;
		case 2:	ChkEachOneMoreSocketState( vDown[i], vUp[i],  szSock[2] );	break;
		case 3:	ChkEachOneMoreSocketState( vDown[i], vUp[i],  szSock[3] );	break;
		case 4:	ChkEachOneMoreSocketState( vDown[i], vUp[i],  szSock[4] );	break;
		case 5:	ChkEachOneMoreSocketState( vDown[i], vUp[i],  szSock[5] );	break;
		case 6:	ChkEachOneMoreSocketState( vDown[i], vUp[i],  szSock[6] );	break;
		case 7:	ChkEachOneMoreSocketState( vDown[i], vUp[i],  szSock[7] );	break;
		}
	}

	for( int nCnt = 0; nCnt < nStation_Max_para; nCnt++ ) {
		if( szSock[nCnt] > 0 )
			nSocketOnCount++;
	}

	if ( nSocketOnCount == nStation_Max_para )
		return TRUE;
	else
		return FALSE;
}

BOOL CTaskSystemCtrl::ChkRetestTwoMoreStaionOn(std::vector<int> vDown, std::vector<int> vUp)
{
	BOOL bIsAutoRetest =  g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
	if (bIsAutoRetest == FALSE)
		return TRUE;

	int nDownStation[eMaxATS_Area/2] = {0,};
	int nUpStation[eMaxATS_Area/2] = {0,};
	int nSum = 0;

	for (int i = 0; i < eMaxATS_Area/2; i++)
		ChkRetestTwoMoreStaionState(i, vDown, vUp,nDownStation[i], nUpStation[i]);

	for( int nCnt = 0; nCnt < (eMaxATS_Area/2); nCnt++) {
		nSum += nDownStation[nCnt];
		nSum += nUpStation[nCnt];
	}

	if (2 <= nSum)
		return TRUE;
	else
		return FALSE;
}

void CTaskSystemCtrl::ChkRetestTwoMoreStaionState( int _index, std::vector<int> _vDown, std::vector<int> _vUp, int& _DownStation, int& _UpStation)
{
	int nTestSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nTestSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	int DownSocket = 0, UpSocket = 0;
	int TestSite_Div = nTestSiteX * nTestSiteY;
	for( int i = 0; i < TestSite_Div; i++ )
	{
		if( _vDown[ ( _index * ( nTestSiteX * nTestSiteY ) ) + i ] == DEF_ON ) { DownSocket++; }
		if( _vUp[ ( _index * ( nTestSiteX * nTestSiteY ) ) + i ] == DEF_ON ) { UpSocket++; }
	}

	if( DownSocket != FALSE )
		_DownStation = TRUE;
	if( UpSocket != FALSE )
		_UpStation = TRUE;
}

void CTaskSystemCtrl::ChkErrEachOneMoreSocketOn(std::vector<int> vDown, std::vector<int> vUp, std::vector<int> &vSocket)
{
	int nSock[STATION_MAX_PARA] = {0,};

	int nTestSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nTestSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	int nStation_Max_Para = nTestSiteX * nTestSiteY;

	for (int i = 0; i < nStation_Max_Para; i++)
	{
		switch (i % nStation_Max_Para)
		{
		case 0 :ChkErrEachOneMoreSocketState(i, vDown, vUp, nSock[0]); break;
		case 1 :ChkErrEachOneMoreSocketState(i, vDown, vUp, nSock[1]); break;
		case 2 :ChkErrEachOneMoreSocketState(i, vDown, vUp, nSock[2]); break;
		case 3 :ChkErrEachOneMoreSocketState(i, vDown, vUp, nSock[3]); break;
		case 4 :ChkErrEachOneMoreSocketState(i, vDown, vUp, nSock[4]); break;
		case 5 :ChkErrEachOneMoreSocketState(i, vDown, vUp, nSock[5]); break;
		case 6 :ChkErrEachOneMoreSocketState(i, vDown, vUp, nSock[6]); break;
		case 7 :ChkErrEachOneMoreSocketState(i, vDown, vUp, nSock[7]); break;
		}
	}

	for( int nCnt = 0; nCnt < nStation_Max_Para; nCnt++ ) {
		if( nSock[nCnt] < 1 )
			vSocket.push_back( nCnt + 1 );
	}
}

void CTaskSystemCtrl::ChkErrEachOneMoreSocketState(int _index, std::vector<int> _vDown, std::vector<int> _vUp, int &_Socket )
{
	int nTestSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nTestSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	if (_vDown[_index] == DEF_ON) { _Socket++; }
	if (_vDown[_index + (nTestSiteX*nTestSiteY)] == DEF_ON) { _Socket++; }
// 	if (_vDown[_index + ((nTestSiteX*nTestSiteY) * 2)] == DEF_ON) { _Socket++; }
// 	if (_vDown[_index + ((nTestSiteX*nTestSiteY) * 3)] == DEF_ON) { _Socket++; }

// 	if (_vUp[_index] == DEF_ON) { _Socket++; }
// 	if (_vUp[_index + (nTestSiteX*nTestSiteY)] == DEF_ON) { _Socket++; }
// 	if (_vUp[_index + ((nTestSiteX*nTestSiteY) * 2)] == DEF_ON) { _Socket++; }
// 	if (_vUp[_index + ((nTestSiteX*nTestSiteY) * 3)] == DEF_ON) { _Socket++; }
}

//==============================================================================
// 도착한 명령을 큐에서 꺼내어 실행 시킨다.
//==============================================================================
void CTaskSystemCtrl::ThreadProc1()
{
	SetThreadInterval(0, 50);
	while( GetThreadAliveFlag(0) ){
		if( GetPauseFlag(0) ) { 
			NEXT;
			continue;
		}
		//for(int i=0; i<MAX_THREAD_SCAN_COUNT; i++){ 
			CBaseTaskManager::SeqCmdRunning(); 
		//}
		NEXT_10;
	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CTaskSystemCtrl::ThreadProc2()
{
	//TRACE(_T("void CTaskSystemCtrl::ThreadProc2() START.\n"));
	SetThreadInterval(1, 50);
	while( GetThreadAliveFlag(1) )
	{
		if( GetPauseFlag(1) ) {
			NEXT;
			continue;
		}
		Switch_Action();
		NEXT;
	}
	//TRACE(_T("void CTaskSystemCtrl::ThreadProc2() END.\n"));
}
void CTaskSystemCtrl::ThreadProc3()
{
	// WMX3 reconeect 기능

	SetThreadInterval(2, 50);

	_eMainPowerRecoveryState State = eIdle;
	_eMainPowerRecoveryState CpTripState = eIdle;
	while (GetThreadAliveFlag(2))
	{
		
		if (GetPauseFlag(2)) {
			NEXT;
			continue;
		}
		
		// 설비 가동중일땐 작업 하지 않는다.
		if (g_START_SW.GetStatus() == TRUE)
		{
			NEXT;
			continue;
		}
		


		if (GetCPTrip() == eOnOffState::eOn)
		{
			if (CpTripState == _eMainPowerRecoveryState::eCPTripOff)
			{
				CpTripState = eCPTripOn;
			}
		}
		else {
			g_DMCont.m_dmEQP.SN(NDM0_MAIN_POWER_CONTROLER_CONNETION_STATUS, eCommIdle);
			CpTripState = eCPTripOff;
		}


		if (GetMainPower() == eOnOffState::eOn)
		{
			if (State == _eMainPowerRecoveryState::eMainPowerDown || CpTripState == eCPTripOn)
			{
				MakeLog(_T("!!! Main Power Down -> Up"));
				printf("!!! Main Power Down -> Up\n");

				m_tcMainPowerRecoveryWait.SetCheckTime(TIME_MAIN_POWER_RECOVERY_PRE_DELAY_SEC); // delay 후에 servo recovery 처리
				m_tcMainPowerRecoveryWait.StartTimer();
				State = eMainPowerInRecovery_WMX;
				CpTripState = eIdle; //cp trip 초기화
			}
			else if (State == eMainPowerInRecovery_WMX)
			{
				// 시간 계수를 해서 Main 전원이 안정적으로 인가 되고 부터 ReConnection 진행 한다.
 				if (m_tcMainPowerRecoveryWait.CheckOverTime() == TRUE)
 				{
					// 여기에서 SoftServo 재 초기화를 한다.
					// ...
					Sleep(1);
					MakeLog(_T("!!! Main Power Recovery..."));
					printf("!!! Main Power Recovery...\n");
					g_DMCont.m_dmEQP.SN(NDM0_MAIN_POWER_CONTROLER_CONNETION_STATUS, eCommReconnectProcessing_WMX);
					if (ReinitializeAxis(eAxisServoConfiguration::Full) == ERR_SUCCESS)
					{
						MakeLog(_T("!!! WMX Reconnection OK."));
						printf("!!! WMX Reconnection OK.\n");
						g_DMCont.m_dmEQP.SN(NDM0_MAIN_POWER_CONTROLER_CONNETION_STATUS, eCommReconnectProcessed_WMX);
					}
					else
					{
						MakeLog(_T("!!! WMX Reconnection Failed."));
						printf("!! WMX Reconnection Failed.\n");
					}		
					if (g_DMCont.m_dmEQP.GN(NDM0_MAIN_POWER_CONTROLER_CONNETION_STATUS) == eCommReconnectProcessed_WMX) {
						State = eMainPowerInRecovery_MotorPicker;
					}
					else
					{
						State = eIdle;
						g_DMCont.m_dmEQP.SN(NDM0_MAIN_POWER_CONTROLER_CONNETION_STATUS, eCommIdle);
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("WMX Reconnection Fail");
						g_Error.AlarmReport(ERR_SERVO_MOTOR_CONTROLER_RECONECTION_FAIL, AlarmData, &g_TaskSystemCtrl);
					}						
				}
			}
			else if (State == eMainPowerInRecovery_MotorPicker)
			{
				g_DMCont.m_dmEQP.SN(NDM0_MAIN_POWER_CONTROLER_CONNETION_STATUS, eCommReconnectProcessing_MotorPicker);
				BOOL bConnection = g_pBbxmnpClient->ReConnection();
				if (bConnection == ERR_NO_ERROR)
				{
					MakeLog(_T("!!! bbxmnp Reconnection OK."));
					printf("!!! bbxmnp Reconnection OK.\n");
					g_DMCont.m_dmEQP.SN(NDM0_MAIN_POWER_CONTROLER_CONNETION_STATUS, eCommReconnectOK);
					// Servo 재 초기화 성공				
				}
				else {
					MakeLog(_T("!!! bbxmnp Reconnection Failed."));
					printf("!! bbxmnp Reconnection Failed.\n");
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Motor Picker Reconnection Fail");
					g_Error.AlarmReport(ERR_SERVO_MOTOR_CONTROLER_RECONECTION_FAIL, AlarmData, &g_TaskSystemCtrl);
				}

				if (g_DMCont.m_dmEQP.GN(NDM0_MAIN_POWER_CONTROLER_CONNETION_STATUS) != eCommReconnectOK)
				{
					g_DMCont.m_dmEQP.SN(NDM0_MAIN_POWER_CONTROLER_CONNETION_STATUS, eCommIdle);
				}

				// 스위치 OFF
				for (int i = 0; i < eMaxTestPPCount; i++)
				{
					g_StepTestPP_SW[i].SetChangeStatusOnOff(DEF_OFF);
					g_StepTestPP_SW[i].SetChangeStatusOff();
				}
				
				for (int i = 0; i < eMaxTrayPPCount; i++)
				{
					g_StepTrayPP_SW[i].SetChangeStatusOnOff(DEF_OFF);
					g_StepTrayPP_SW[i].SetChangeStatusOff();
				}
				

				g_Feeder_SW.SetChangeStatusOnOff(DEF_OFF);
				g_Feeder_SW.SetChangeStatusOff();

				State = eIdle;
			}
		}
		else
		{
			if (State != eMainPowerDown)
			{
				MakeLog(_T("!!! Main Power Up -> Down"));
				printf("!!! Main Power Up -> Down\n");
			}

			// Power down이므로 bitmask를 모두 clear한다.
			g_DMCont.m_dmEQP.SN(NDM0_MAIN_POWER_CONTROLER_CONNETION_STATUS, eCommIdle);

			State = eMainPowerDown;
		}

// 		if (eNowWMX3ReconnectSts == _eWMX3Reconnect::eCommReconnectProcess)
// 		{
// 			Sleep(1);
// 			MakeLog(_T("!!! Main Power Recovery..."));
// 
// 			if (ReinitializeAxis(eAxisServoConfiguration::Full) == ERR_SUCCESS)
// 			{
// 				MakeLog(_T("!!! Done."));
// 			}
// 			else
// 			{
// 				MakeLog(_T("!!! Failed."));
// 			}
// 
// 			SetWMX3ReconnectSts(_eWMX3Reconnect::eCommReconnectOK);
// 
// 		}
// 		else if(eNowWMX3ReconnectSts == _eWMX3Reconnect::eCommReconnectOK)
// 		{
// 			SetWMX3ReconnectSts(_eWMX3Reconnect::eCommIdle);
// 		}
		
		NEXT;
	}

}

int CTaskSystemCtrl::GetMainPower()
{
	int nRet = eOnOffState::eOn;
	if (g_IO.in(m_vX[MAIN_POWER_ON_SEN1]) == eOnOffState::eOff
		&& g_IO.in(m_vX[MAIN_POWER_ON_SEN2]) == eOnOffState::eOff
		/*&& g_IO.GetDriverType() != _DIO_TYPE_SIMUL_*/)
	{
		nRet = eOnOffState::eOff;
	}
	return nRet;
}

int CTaskSystemCtrl::GetCPTrip()
{
	int nRet = eOnOffState::eOn;
	if (g_IO.in(m_vX[TEST1_PRESS1_SERVO_CP_TRIP_ALARM_SEN]) == eOnOffState::eOff
		|| g_IO.in(m_vX[LOAD_UNLOAD_TRANSFER_SERVO_CP_TRIP_SEN]) == eOnOffState::eOff
		|| g_IO.in(m_vX[TABLE_SERVO_CP_TRIP_SEN]) == eOnOffState::eOff
		|| g_IO.in(m_vX[TEST2_PRESS2_CP_TRIP_SEN]) == eOnOffState::eOff
		/*&& g_IO.GetDriverType() != _DIO_TYPE_SIMUL_*/)
	{
		nRet = eOnOffState::eOff;
	}
	return nRet;
}

int CTaskSystemCtrl::ReinitializeAxis(eAxisServoConfiguration Configuration)
{
	int nAxisCount = g_Motor.GetAxisCount();

	int nEngineFlags = (int)(eAxisServoConfiguration::RestartEngine | eAxisServoConfiguration::ReconnectCommunication);

	eAxisServoConfiguration EngineConfiguration = eAxisServoConfiguration((int)Configuration & nEngineFlags);

	// Create Device 주석
	char szDriverFileName[MAX_PATH] = { 0, };
	
	g_SoftServoCtrl.CloseDevice();
	Sleep(2000);

	char szTemp[512] = { 0, };
	::GetCurrentDirectory(sizeof(szTemp), szTemp);
	sprintf_s(szDriverFileName, sizeof(szDriverFileName), "%s\\sys_data\\%s", szTemp, SZ_DRIVER_FILE_NAME);
	g_SoftServoCtrl.CreateDev(szDriverFileName, MAX_PATH); //WMX Init Motor에서

	for (int i = 0; i < nAxisCount; i++)
	{
		auto* pAxis = g_Motor[i];
		bool bResult = false;

		pAxis->Set_disable(TRUE);

// 		if (i == 0)
// 			bResult = pAxis->ApplyConfiguration(eAxisServoConfiguration::RestartEngine);
// 		else
			bResult = pAxis->ApplyConfiguration(eAxisServoConfiguration((int)Configuration & ~nEngineFlags));

		if (bResult == false)
			return ERR_FAILURE;
		else
			pAxis->Set_disable(FALSE);
	}

	return ERR_SUCCESS;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-17]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CTaskSystemCtrl::ActionTowerAndSound(int kinds)
{
	m_csTowerAction.Lock();

	if(!m_intList.Find(kinds))
	{
		m_intList.AddHead(kinds);
		ActTower();
	}

	m_csTowerAction.Unlock();
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-17]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CTaskSystemCtrl::DeActionTowerAndSound(int kinds)
{
	m_csTowerAction.Lock();

	POSITION pos = m_intList.Find(kinds);

	if(pos)
	{
		m_intList.RemoveAt(pos);
		ActTower();
	}
	m_csTowerAction.Unlock();

	do_Buzzer(0);
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-17]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
POSITION CTaskSystemCtrl::GetTowerActionCheck(int kinds)
{
	m_csTowerAction.Lock();
	POSITION pos;
	pos = m_intList.Find(kinds);
	m_csTowerAction.Unlock();
	return pos;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-17]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CTaskSystemCtrl::DeleteActionTower()
{
	int nArryError[eSIGNAL_MAX] = {
		eSIGNAL_RUNNING,
		eSIGNAL_JAM_CALL,
		eSIGNAL_CYCLE_STOP,
		eSIGNAL_FINISH,
		eSIGNAL_RESET,
		eSIGNAL_PAUSE,
		eSIGNAL_REQUEST,
		eSIGNAL_TEMP_WAITING,
		eSIGNAL_TEST_RUNNING,
		eSIGNAL_START,
		eSIGNAL_TEMP_STABILIZE};//{0,};//{eSIGNAL_REQUEST, eSIGNAL_CYCLE_STOP, eSIGNAL_STABLE, eSIGNAL_MESSAGE, 0, };
	int nTower = 0;

	m_csTowerAction.Lock();
	for(nTower = 0; nTower < eSIGNAL_MAX; nTower++)
	{
		POSITION pPos = GetTowerActionCheck(nArryError[nTower]);
		if(pPos)
		{
			DeActionTowerAndSound(nArryError[nTower]);
		}
	}
	m_csTowerAction.Unlock();
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-17]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CTaskSystemCtrl::ActTower()
{
	int nFirst=eSIGNAL_MAX, nFounded=0;
	POSITION pos = m_intList.GetHeadPosition();

	if(pos) 
	{
		for(int i=0; i<m_intList.GetCount(); i++)
		{
			int tmp = m_intList.GetNext(pos);
			if(nFirst > m_stTowerSetting.nTower[tmp][eSIGNAL_PRIORTY])
			{
				nFirst = m_stTowerSetting.nTower[tmp][eSIGNAL_PRIORTY];
				nFounded = tmp;	
			}
		}
	}

	// Action tower lamp[Red/Yellow/Green] switch status 
	switch(m_stTowerSetting.nTower[nFounded][eSIGNAL_RED])
	{
	case eLAMP_OFF:
		g_TowerR.SetStatus(FALSE);
		break;
	case eLAMP_ON:
		g_TowerR.SetBlink(FALSE);
		g_TowerR.SetStatus(TRUE);
		break;
	case eLAMP_BLINK:
		g_TowerR.SetBlink(TRUE);
		g_TowerR.SetStatus(TRUE);
		break;
	}
	switch(m_stTowerSetting.nTower[nFounded][eSIGNAL_YELLOW])
	{
	case eLAMP_OFF:
		g_TowerY.SetStatus(FALSE);
		break;
	case eLAMP_ON:
		g_TowerY.SetBlink(FALSE);
		g_TowerY.SetStatus(TRUE);
		break;
	case eLAMP_BLINK:
		g_TowerY.SetBlink(TRUE);
		g_TowerY.SetStatus(TRUE);
		break;
	}
	switch(m_stTowerSetting.nTower[nFounded][eSIGNAL_GREEN])
	{
	case eLAMP_OFF:
		g_TowerG.SetStatus(FALSE);
		break;
	case eLAMP_ON:
		g_TowerG.SetBlink(FALSE);
		g_TowerG.SetStatus(TRUE);
		break;
	case eLAMP_BLINK:
		g_TowerG.SetBlink(TRUE);
		g_TowerG.SetStatus(TRUE);
		break;
	}

	// Action Buzzer
	if(m_stTowerSetting.nTower[nFounded][eSIGNAL_SOUND])
	{
		int nBuzzer = m_stTowerSetting.nTower[nFounded][eSIGNAL_SOUND];
		Sleep(10);
		do_Buzzer(nBuzzer);
	}
}

void CTaskSystemCtrl::do_Buzzer(int nBuzAction)
{
	if(nBuzAction == 0){
		g_IO.out(m_vY[ BUZZER1 ], DEF_OFF);
		g_IO.out(m_vY[ BUZZER2 ], DEF_OFF);
		g_IO.out(m_vY[ BUZZER3 ], DEF_OFF);
		g_IO.out(m_vY[ BUZZER4 ], DEF_OFF);
	}
	else
	{
		g_IO.out(m_vY[ BUZZER1 ], (nBuzAction==1));
		g_IO.out(m_vY[ BUZZER2 ], (nBuzAction==2));
		g_IO.out(m_vY[ BUZZER3 ], (nBuzAction==3));
		g_IO.out(m_vY[ BUZZER4 ], (nBuzAction==4));
	}	
}



int CTaskSystemCtrl::SetSignalTowerDataFromSM()
{
	m_csTowerAction.Lock();

	m_intList.RemoveAll();

	int nMainTitle[eSIGNAL_MAX] = {eSIGNAL_RUNNING, eSIGNAL_JAM_CALL, eSIGNAL_CYCLE_STOP, eSIGNAL_FINISH, eSIGNAL_RESET,
		eSIGNAL_PAUSE, eSIGNAL_REQUEST, eSIGNAL_TEMP_WAITING, eSIGNAL_TEST_RUNNING, eSIGNAL_START,
		eSIGNAL_TEMP_STABILIZE};
	int nAddressSM[eSIGNAL_MAX] = {NDM0_Tower_Running_P, NDM0_Tower_JamCall_P, NDM0_Tower_Cycle_P, NDM0_Tower_Finish_P 
		,NDM0_Tower_Reset_P, NDM0_Tower_Pause_P, NDM0_Tower_Request_P, NDM0_Tower_TempWait_P 
		,NDM0_Tower_TestRun_P, NDM0_Tower_Start_P, NDM0_Tower_TempStabil_P};

	for(int i=0; i<eSIGNAL_MAX; i++)
	{
		m_stTowerSetting.nTower[nMainTitle[i]][eSIGNAL_PRIORTY] = g_DMCont.m_dmEQP.GN(nAddressSM[i]);
		m_stTowerSetting.nTower[nMainTitle[i]][eSIGNAL_RED]	    = g_DMCont.m_dmEQP.GN(nAddressSM[i]+1);
		m_stTowerSetting.nTower[nMainTitle[i]][eSIGNAL_YELLOW]  = g_DMCont.m_dmEQP.GN(nAddressSM[i]+2);
		m_stTowerSetting.nTower[nMainTitle[i]][eSIGNAL_GREEN]	= g_DMCont.m_dmEQP.GN(nAddressSM[i]+3);
		m_stTowerSetting.nTower[nMainTitle[i]][eSIGNAL_SOUND]	= g_DMCont.m_dmEQP.GN(nAddressSM[i]+4);
	}

	m_csTowerAction.Unlock();

	ActionTowerAndSound(eSIGNAL_PAUSE);

	return ERR_NO_ERROR;
}

void CTaskSystemCtrl::ResetUPHtm()
{
	m_tcUPH.SetTimerCheckEnd();
	m_tcStop.SetTimerCheckEnd();
	m_tcCycleUPH.SetTimerCheckEnd();
	m_dTotalStopTm = 0.0;
	m_dTotalRunTm  = 0.0;
}

int CTaskSystemCtrl::CalcUPH()
{
	double dUPH = 0.0, dCycleUPH = 0.0;

	if(m_tcUPH.IsChecking() == TRUE){
		double dSortDvc  = (double)g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt);
		double dSpanTime = m_tcUPH.CompareTime(FALSE) + m_dTotalRunTm; // current time - lot start time

		dUPH = ((dSortDvc/*-m_nRecSortCnt*/) / (dSpanTime-(m_dTotalStopTm + (m_tcStop.IsChecking() ? m_tcStop.CompareTime(FALSE) : 0 )))) * 3600.0;
		if( dUPH == 0 ){ dUPH = 0; }
		g_DMCont.m_dmEQP.SN(NDM0_UPH, (int)dUPH);

		dUPH = ((dSortDvc/*-m_nRecSortCnt*/) / dSpanTime) * 3600.0;
		if( dUPH == 0 ){ dUPH = 0; }
		g_DMCont.m_dmEQP.SN(NDM0_GROSS_UPH, (int)dUPH);

		// Cycle UPH [1/6/2022 donghyun.shin]
		double dCycleStartTime = (double)g_DMCont.m_dmEQP.GN(NDM0_CycleLotStartTime);
		double dCycleSortDvc = (double)g_DMCont.m_dmEQP.GN(NDM0_CycleCurLotSortingCnt);

		double dCycleSpanTime = m_tcUPH.CompareTime(FALSE) + m_dTotalRunTm - dCycleStartTime; // current time - lot start time
		//double dCycleSpanTime = m_tcCycleUPH.CompareTime(FALSE) + m_dTotalRunTm /*- dCycleStartTime*/; // current time - lot start time
		//dCycleUPH = ((dSortDvc - dCycleSortDvc) / (dCycleSpanTime - (m_dTotalStopTm + (m_tcStop.IsChecking() ? m_tcStop.CompareTime(FALSE) : 0)))) * 3600.0;
		dCycleUPH = ((dSortDvc > dCycleSortDvc ? dSortDvc - dCycleSortDvc : dSortDvc) / (dCycleSpanTime - (m_dTotalStopTm + (m_tcStop.IsChecking() ? m_tcStop.CompareTime(FALSE) : 0)))) * 3600.0;
		if (dCycleUPH == 0) { dCycleUPH = 0; }
		g_DMCont.m_dmEQP.SN(NDM0_CYCLE_UPH, (int)dCycleUPH);
	}

	return (int)dUPH;
}



void CTaskSystemCtrl::UpdateStopRunTm()
{
	if( m_tcUPH.IsChecking() == TRUE )
	{
		double dSortDvc  = (double)g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt);

		g_DMCont.m_dmEQP.SN(NDM0_LotStopTime, (int)(m_dTotalStopTm + (m_tcStop.IsChecking() ? m_tcStop.CompareTime(FALSE) : 0 )) );
		g_DMCont.m_dmEQP.SN(NDM0_LotRunTime , (int)(m_dTotalRunTm  + (m_tcUPH.IsChecking() ? m_tcUPH.CompareTime(FALSE) : 0 )) );

		if(m_tcStop.IsChecking() == TRUE){
			double dSpanTime = m_tcUPH.CompareTime(FALSE) + m_dTotalRunTm; // current time - lot start time
			int dUPH = (int)(((dSortDvc/*-m_nRecSortCnt*/) / dSpanTime) * 3600.0);
			if( dUPH == 0 ){ dUPH = 0; }
			g_DMCont.m_dmEQP.SN(NDM0_GROSS_UPH, (int)dUPH);	
		}
	}
	if(m_tcUPH.IsChecking() != TRUE){		
		//			m_nRecSortCnt = (int)dSortDvc;
		g_DMCont.m_dmEQP.SN(NDM0_GROSS_UPH, 0);
		g_DMCont.m_dmEQP.SN(NDM0_UPH, 0);
		g_DMCont.m_dmEQP.SN(NDM0_CYCLE_UPH, 0);
	}
}


void CTaskSystemCtrl::UpdateCycleUPH()
{	
	for (int i =0; i < eMaxPressUnitCount; i++)
	{
		int bCycleUPH = g_DMCont.m_dmEQP.GB(BDM0_CYCLE_UPH_CHECK);

		if (bCycleUPH == FALSE)
		{
			int nIndexCount = g_DMCont.m_dmContac_Dn.GN(NDM1_INDEX_RECORD_IGNORE_STATION1 + i);
			if (nIndexCount >= MAX_IGNORE_INDEX_REC_COUNT)
			{
				g_DMCont.m_dmEQP.SB(BDM0_CYCLE_UPH_CHECK, TRUE);

				int dSortDvc = g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt);
				g_DMCont.m_dmEQP.SN(NDM0_CycleCurLotSortingCnt,dSortDvc);
				g_DMCont.m_dmEQP.SN(NDM0_CycleLotStartTime, (m_tcCycleUPH.IsChecking() ? m_tcCycleUPH.CompareTime(FALSE) : 0));				
			}
		}	
	}
}

void CTaskSystemCtrl::SetAutoGrrCancel( bool bSet )
{
	g_DMCont.m_dmEQP.SB( BDM0_SYS_AUTO_GRR_CANCEL, bSet );
}

BOOL CTaskSystemCtrl::GetAutoGrrCancel()
{
	BOOL bRet = g_DMCont.m_dmEQP.GB( BDM0_SYS_AUTO_GRR_CANCEL );
	return bRet;
}

int CTaskSystemCtrl::ChkAllSiteOffGrr()
{
	int nCnt = 0;

	int nSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	for (int i = 0; i < eMaxPressUnitCount; i++)
	{
		for (int nX = 0; nX < nSiteX; nX++)
		{
			for (int nY = 0; nY < nSiteY; nY++)
			{
				int nPos = nX + nY*STATION_MAX_X_SITE + i *STATION_MAX_PARA;
				if (g_DMCont.m_dmHandlerOpt.GN(NDM11_GRR_SocketOnOff_Site1_1 + nPos) == DEF_SITE_ON)
				{
					nCnt++;
				}
			}
		}
	}

	if (nCnt == 0)
		return ERR_TEST_SITE_ALL_OFF_GRR;

	return ERR_NO_ERROR;
}
void CTaskSystemCtrl::LotInfoDataClear()
{
	g_DMCont.m_dmHandlerOpt.SS(SDM11_LotNo,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_CustomerName,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_DeviceName,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_ScheduleNo,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_RetestCode,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_ProgramName,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_Operation,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_Temp,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_QTY,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_ART,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_TP_REV,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_Soak_Time,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_FUSE,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_HANDLER,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_OperatorID,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_LOAD_BD_NO,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_SOCKER_ID,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_C_KIT_ID,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_FOUNDRY_LOTID,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_Assembly_startdate,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_Assembly_enddate,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_ASSY_vendor_lotid,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_TEST_vendor_lotid,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_BOT_SUB_Vendor,"",MAX_PATH);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_Nickname,"",MAX_PATH);
}

//void CTaskSystemCtrl::CalcYieldVarianceMonitoring()
//{
//	//BOOL bMD_YieldMonitor  = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_YieldCtrl);
//	//if(bMD_YieldMonitor != DEF_ON)
//	//	return;
//	//
//	//
//	//double dTotalYield[eMaxPressUnitCount][STATION_MAX_Y_SITE][STATION_MAX_X_SITE];
//	//ZeroMemory(dTotalYield, sizeof(dTotalYield));
//	//int    nPreSetCnt        = g_DMCont.m_dmHandlerOpt.GN(NDM11_Preset_Counter);
//	//int    nSamplingCnt      = g_DMCont.m_dmHandlerOpt.GN(NDM11_Sample_Quantity);
//	//int    nLotStartMode     = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
//	//BOOL   bYieldRetestUse   = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Retest_Yield);
//	//double dSetVarianceYield = g_DMCont.m_dmHandlerOpt.GD(DDM11_VarianceYield);
//	//BOOL   bOccurAlarm       = FALSE;
//	//double dTopYield = 0.0;
//	//
//	//int nSiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
//	//int nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
//	//
//	//m_csYieldVariance.Lock();
//	//
//	//// Get all site yield
//	//for(int nAreaIdx = 0; nAreaIdx<eMaxPressUnitCount; nAreaIdx++ )
//	//{	
//	//	for(int nY=0; nY < nSiteCntY; nY++ )
//	//	{
//	//		for(int nX=0; nX < nSiteCntX; nX++ )
//	//		{
//	//			if( (int)m_vVarPF_Yield_PreSet[nAreaIdx][nY][nX].size()>= nPreSetCnt && (int)m_dqVarPF_Yield[nAreaIdx][nY][nX].size() >= nSamplingCnt)
//	//			{
//	//				int nPassCnt = 0;
//	//				for(deque< BOOL >::size_type i=0; i < (int)m_dqVarPF_Yield[nAreaIdx][nY][nX].size() ; i++ ){
//	//					if(m_dqVarPF_Yield[nAreaIdx][nY][nX][i] == eGoodBin){ nPassCnt++; }
//	//				}
//	//
//	//				dTotalYield[nAreaIdx][nY][nX] = ((double)nPassCnt / (double)nSamplingCnt) * 100.0;
//	//			}else{
//	//				dTotalYield[nAreaIdx][nY][nX] = -1;
//	//			}
//	//		}
//	//	}
//	//}
//	//// Get High yield
//	//for(int nTestSite = 0; nTestSite< eMaxPressUnitCount; nTestSite++)
//	//{
//	//	for(int nY=0; nY < nSiteCntY; nY++ )
//	//	{
//	//		for(int nX=0; nX < nSiteCntX; nX++ )
//	//		{
//	//			if(dTopYield < dTotalYield[nTestSite][nY][nX]){
//	//				dTopYield = dTotalYield[nTestSite][nY][nX];
//	//			}
//	//		}
//	//	}
//	//}
//	//
//	//CParamList AlarmData;
//	//
//	//for(int i =0; i< eMaxPressUnitCount; i++)
//	//{
//	//	CString strSite = _T("");
//	//	for(int nY=0; nY < nSiteCntY; nY++ )
//	//	{
//	//		for(int nX=0; nX < nSiteCntX; nX++ )
//	//		{
//	//			if( (int)m_vVarPF_Yield_PreSet[i][nY][nX].size() >= nPreSetCnt && (int)m_dqVarPF_Yield[i][nY][nX].size() >= nSamplingCnt)
//	//			{
//	//				if(dTotalYield[i][nY][nX] != -1){
//	//					if(dTopYield - dTotalYield[i][nY][nX] > dSetVarianceYield){
//	//						if( (bYieldRetestUse == TRUE && (nLotStartMode == eStartMD_Retest || nLotStartMode == eStartMD_RetestContinue) )
//	//							|| (nLotStartMode != eStartMD_Retest && nLotStartMode != eStartMD_RetestContinue) )
//	//						{
//	//							CString strTemp = _T("");
//	//							strTemp.Format("Station%d[%d-%d], ",i+1, nX+1, nY+1);
//	//							strSite += strTemp;
//	//
//	//							YieldVarianceClear(i, nX, nY);
//	//							bOccurAlarm = TRUE;
//	//						}
//	//					}
//	//					/*else{
//	//						m_dqVarPF_Yield[i][nY][nX].pop_front();
//	//					}*/
//	//				}
//	//			}
//	//		}
//	//	}
//	//
//	//	if(strSite.IsEmpty() == false)
//	//		AlarmData.m_fnAddItemFormat("%s", strSite);
//	//}
//	//
//	//m_csYieldVariance.Unlock();
//	//	
//	//BOOL bYieldMonitor   = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Event_Type);
//	//if(bOccurAlarm == TRUE){
//	//	// function alarm stop		
//	//	AlarmData.m_fnAddItemFormat( "Variance Failure Alarm!!!!");
//	//	g_Error.AlarmReport(ERR_TESTER_LOW_YIELD, AlarmData,&g_TaskSystemCtrl);
//	//
//	//	if(eRetestYieldFunc_Stop == bYieldMonitor){
//	//		g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
//	//	}
//	//}
//}

//void CTaskSystemCtrl::YieldVarianceClear()
//{
//	for(int i=0; i<eMaxPressUnitCount; i++){
//		for(int nX=0; nX < STATION_MAX_X_SITE; nX++ ){
//			for(int nY=0; nY < STATION_MAX_Y_SITE; nY++ ){
//				YieldVarianceClear(i, nX, nY);
//			}
//		}
//	}
//}

//void CTaskSystemCtrl::YieldVarianceClear(int nIdx, int nX, int nY )
//{
//	ASSERT( nIdx >= ePressUnit_Down_1 && nIdx < eMaxPressUnitCount );
//
//	m_csYieldVariance.Lock();
//	
//	m_dqVarPF_Yield[nIdx][nY][nX].clear();
//	m_vVarPF_Yield_PreSet[nIdx][nY][nX].clear();
//
//	m_csYieldVariance.Unlock();
//}

//void CTaskSystemCtrl::AddYieldVariance(int nIdx, BOOL bPassFailData, int nX, int nY)
//{
//	int nPreSetCnt  = g_DMCont.m_dmHandlerOpt.GN(NDM11_Preset_Counter);
//	int nSampleCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_Sample_Quantity);
//
//	m_csYieldVariance.Lock();
//
//	if ((int)m_vVarPF_Yield_PreSet[nIdx][nY][nX].size() >= nPreSetCnt)
//	{
//		if ((int)m_dqVarPF_Yield[nIdx][nY][nX].size() >= nSampleCnt)
//		{
//			m_dqVarPF_Yield[nIdx][nY][nX].pop_front();
//			m_dqVarPF_Yield[nIdx][nY][nX].push_back(bPassFailData);
//		}
//		else {
//			m_dqVarPF_Yield[nIdx][nY][nX].push_back(bPassFailData);
//		}
//	}		
//	else
//		m_vVarPF_Yield_PreSet[nIdx][nY][nX].push_back(bPassFailData);
//
//	m_csYieldVariance.Unlock();
//}

void CTaskSystemCtrl::CalcYieldContiFailMonitoring(int nPressIndex, std::vector<CPoint> &vAlarmSet)
{
	BOOL   bMD_YieldMonitor  = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_YieldCtrl);
	if(bMD_YieldMonitor != DEF_ON) {return;}

	m_csYieldContiFail.Lock();

	int nSiteInfo = 0;

	int   nFailSetCnt       = g_DMCont.m_dmHandlerOpt.GN(NDM11_ContinueFail_FailCnt);
	BOOL  bOccurAlarm       = FALSE;
	BOOL  bFailCntOver		= FALSE;
	int   nSiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int   nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	CParamList AlarmData;
	BOOL bAlarmPos[eMaxPressUnitCount][STATION_MAX_Y_SITE][STATION_MAX_X_SITE] = { FALSE, };
	CString strErrMsg = _T("");
	CString strFailMsg = _T("");

	BOOL   bIsPossibleSiteOff = FALSE;
	BOOL   bIsImPossibleSiteOff = FALSE;
	BOOL   bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
	ST_DD_DEVICE stDvcData;
	BOOL bRetFunc = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Consecutive_Fail_AutoKill);
	
	std::vector<int> vSetSiteKind;
	vSetSiteKind.clear();

	std::vector<CPoint> vFailPocket;

	BOOL bSetSiteOff = FALSE;
	std::vector<CPoint> vSetSiteOff;
	int	   nSocketOffKind = DEF_SITE_OFF_YIELD;
	//for(int nIdx = 0; nIdx<eMaxPressUnitCount; nIdx++ )
	//{	
		bFailCntOver = FALSE;
		for(int nY=0; nY < nSiteCntY; nY++ )
		{
			for(int nX=0; nX < nSiteCntX; nX++ )
			{
				bool bfindpos = false;
				for (int i = 0; i < (int)vAlarmSet.size(); i++)
				{
					if (vAlarmSet[i] == CPoint(nX, nY))
					{
						bfindpos = true;
						break;
					}
				}
				if (bfindpos == true)
					YieldContiFailClear(nPressIndex, nX, nY);

				// ==================================
				// checking consecutive Failure
				// ==================================
				if( (int)m_dqContiFail[nPressIndex][nY][nX].size() >= nFailSetCnt ){
					int nContiFailCnt = 0;
					for(deque< BOOL >::size_type i=0; i < m_dqContiFail[nPressIndex][nY][nX].size(); i++ ){
						if( m_dqContiFail[nPressIndex][nY][nX][i] != eGoodBin )
						{ 
							nContiFailCnt++;							
							if(nFailSetCnt == nContiFailCnt){
								//bAlarmPos[nIdx][nY][nX] = TRUE;		

								stDvcData.clear();
								g_TaskPressUnit[nPressIndex].GetSiteDeviceData(&stDvcData, nX, nY);

								BOOL bSiteOff = TRUE;
								if (bAutoRetestUseMode == DEF_ON && stDvcData.sExist == DEF_EXIST && stDvcData.nTestingCnt == eARB_1stTestEnd && stDvcData.nBinColor == eFailBin) {
									bSiteOff = FALSE;
								}

								CString strErrPos = _T("");
								if (bRetFunc == DEF_ON) {
									bIsPossibleSiteOff = CheckAutoRetestRuleSite(nPressIndex, nX, nY);
									if (bIsPossibleSiteOff == FALSE) {
										bIsImPossibleSiteOff = TRUE;
										strErrPos.Format(_T("[Station=%d Site=%d]"), nPressIndex + 1, (nX + 1 + (nY * STATION_MAX_X_SITE)));
										strFailMsg += strErrPos;
									}
									else {
										if (bSiteOff == TRUE) {
											nSocketOffKind = DEF_SITE_OFF_YIELD;
										}
										else {
											nSocketOffKind = DEF_SITE_OFF_RESERVATION_YIELD;

										}
										//consecutive Fail 시 Site Off
										if (bfindpos == false)
										{
											g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + nX + (nY*STATION_MAX_X_SITE) + (nPressIndex * STATION_MAX_PARA), nSocketOffKind);
											vSetSiteOff.push_back(CPoint(nX, nY));
											vSetSiteKind.push_back(nSocketOffKind);
											bSetSiteOff = TRUE;
									    }
									}

									strErrPos.Format(_T("[Station=%d Site=%d Count=%d]\n"), nPressIndex + 1, (nX + 1 + (nY * STATION_MAX_X_SITE)), nContiFailCnt);
									strErrMsg += strErrPos;
									bFailCntOver = TRUE;
									YieldContiFailClear(nPressIndex, nX, nY);
									vFailPocket.push_back(CPoint(nX, nY));

									if (bfindpos == false)
										vAlarmSet.push_back(CPoint(nX, nY));
								}
							}
						}
						else{
							nContiFailCnt = 0;
						}
					}
				}
			}
		}
		if (bFailCntOver) {
			bOccurAlarm = TRUE;
			if (bSetSiteOff) {
				g_TaskSystemCtrl.SetSiteOnOffData(nPressIndex, vSetSiteOff, vSetSiteKind);
			}
			AlarmData.m_fnClear();
			AlarmData.m_fnAddItemFormat("Occurred consecutive Failure Alarm!!!!");
			AlarmData.m_fnAddItemFormat(strErrMsg);
			g_Error.AlarmReport(ERR_TESTER_CONSECUTIVE_FAIL_STATION_1 + nPressIndex, AlarmData, this, vFailPocket);
			strErrMsg.Empty();
		}

		if (bIsImPossibleSiteOff) {
			AlarmData.m_fnClear();
			AlarmData.m_fnAddItemFormat("No more site off!! Please Check it");
			AlarmData.m_fnAddItemFormat(strFailMsg);
			g_Error.AlarmReport(ERR_INTERLOCK_SITE_OFF_ERROR_STATION_1 + nPressIndex, AlarmData, &g_TaskSystemCtrl);
			g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
		}

	//}

	m_csYieldContiFail.Unlock();
	
	if(bOccurAlarm == TRUE || bIsImPossibleSiteOff){
		BOOL bYieldMonitor = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Event_Type);
		// function alarm stop
		if(eRetestYieldFunc_Stop == bYieldMonitor || bIsImPossibleSiteOff){
			g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
		}
	}
}

void CTaskSystemCtrl::YieldContiFailClear()
{
	for(int i=0; i<eMaxPressUnitCount; i++){
		for(int nX=0; nX < STATION_MAX_X_SITE; nX++ ){
			for(int nY=0; nY < STATION_MAX_Y_SITE; nY++ ){
				YieldContiFailClear(i, nX, nY);
			}
		}
	}
}

void CTaskSystemCtrl::YieldContiFailClear(int nIdx, int nX, int nY)
{
	ASSERT( nIdx >= ePressUnit_Down_1 && nIdx < eMaxPressUnitCount );

	m_csYieldContiFail.Lock();

	m_dqContiFail[nIdx][nY][nX].clear();

	m_csYieldContiFail.Unlock();
}

void CTaskSystemCtrl::AddYieldContiFail(int nIdx, BOOL bPassFailData, int nX, int nY)
{
	m_csYieldContiFail.Lock();

	m_dqContiFail[nIdx][nY][nX].push_back(bPassFailData);

	m_csYieldContiFail.Unlock();
}

void CTaskSystemCtrl::CalcYieldSampleMonitoring(int nPressIndex, std::vector<CPoint> &vAlarmSet)
{
	BOOL   bMD_YieldMonitor  = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_YieldCtrl);
	if(bMD_YieldMonitor != DEF_ON) {return;}

	m_csYieldSample.Lock();
	 
	int nSiteInfo = 0;

	int    nPreSetCnt        = g_DMCont.m_dmHandlerOpt.GN(NDM11_Preset_Counter);
	int    nSamplingCnt      = g_DMCont.m_dmHandlerOpt.GN(NDM11_Sample_Quantity);
	int    nFailSetCnt       = g_DMCont.m_dmHandlerOpt.GN(NDM11_ContinueFail_FailCnt);
	int    nLotStartMode     = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	double dSetLowYield      = g_DMCont.m_dmHandlerOpt.GD(DDM11_Low_Yield);
	BOOL   bYieldRetestUse   = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Retest_Yield);
	BOOL   bOccurAlarm       = FALSE;
	BOOL   bLowYield		 = FALSE;
	int nSiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	BOOL   bIsPossibleSiteOff = FALSE;
	BOOL   bIsImPossibleSiteOff = FALSE;

	BOOL   bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
	ST_DD_DEVICE stDvcData;

	CParamList AlarmData;
	BOOL bRetFunc = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoKill);

	std::vector<CPoint> vFailPocket;
	CString strErrMsg = _T("");
	CString strFailMsg = _T("");

	BOOL bSetSiteOff = FALSE;
	std::vector<CPoint> vSetSiteOff;

	std::vector<int> vSetSiteKind;
	vSetSiteKind.clear();

	int	   nSocketOffKind = DEF_SITE_OFF_YIELD;
	//for(int nIdx = 0; nIdx<eMaxPressUnitCount; nIdx++ )
	//{	
		bLowYield = FALSE;
		for(int nY=0; nY < nSiteCntY; nY++ )
		{
			for(int nX=0; nX < nSiteCntX; nX++ )
			{
				bool bfindpos = false;
				for (int i = 0; i < (int)vAlarmSet.size(); i++)
				{
					if (vAlarmSet[i] == CPoint(nX, nY))
					{
						bfindpos = true;
						break;
					}
				}

				if (bfindpos == true)
					YieldSampleClear(nPressIndex, nX, nY);

				//===================================
				// 1. function on
				// 2. checking test end
				// 3. checking preset count over
				// 4. checking sampling count over
				//===================================
				if(/*nSiteInfo == eTestSite_TestEnd 
					&&*/ ( (int)m_vSamplePF_Yield_PreSet[nPressIndex][nY][nX].size()   >= nPreSetCnt   )
					&& ( (int)m_dqSamplePF_Yield[nPressIndex][nY][nX].size() >= nSamplingCnt ))
				{
					int nPassCnt = 0;
					for(deque< BOOL >::size_type i=0; i < (int)m_dqSamplePF_Yield[nPressIndex][nY][nX].size() ; i++ ){
						if(m_dqSamplePF_Yield[nPressIndex][nY][nX][i] == eGoodBin){ nPassCnt++; }
					}

					double dTotalYield = ((double)nPassCnt / (double)nSamplingCnt) * 100.0;
					if( dTotalYield < dSetLowYield )
					{
						// inspection retest mode alarm & normal mode alarm
						if( (bYieldRetestUse == TRUE && (nLotStartMode == eStartMD_Retest || nLotStartMode == eStartMD_RetestContinue) )
							|| (nLotStartMode != eStartMD_Retest && nLotStartMode != eStartMD_RetestContinue) )
						{
							//AlarmData.m_fnAddItemFormat("Station=%d Site=%d , Set LowYield[%.2f], PassYield[%.2f]", nIdx + 1, (nX + 1 + (nY *STATION_MAX_X_SITE)), dSetLowYield, dTotalYield);
							
							bLowYield = TRUE;

							stDvcData.clear();
							g_TaskPressUnit[nPressIndex].GetSiteDeviceData(&stDvcData, nX, nY);
							BOOL bSiteOff = TRUE;
							if (bAutoRetestUseMode == DEF_ON && stDvcData.sExist == DEF_EXIST && stDvcData.nTestingCnt == eARB_1stTestEnd && stDvcData.nBinColor == eFailBin) {
								bSiteOff = FALSE;
							}
							CString strTmp = _T("");

							if (bRetFunc == DEF_ON) {
								bIsPossibleSiteOff = CheckAutoRetestRuleSite(nPressIndex, nX, nY);
								if (bIsPossibleSiteOff == FALSE) {
									bIsImPossibleSiteOff = TRUE;
									strTmp.Format(_T("[Station=%d Site=%d]"), nPressIndex + 1, (nX + 1 + (nY *STATION_MAX_X_SITE)));
									strFailMsg += strTmp;
								}
								else {
									if (bSiteOff == TRUE) {
										nSocketOffKind = DEF_SITE_OFF_YIELD;
									}
									else {
										nSocketOffKind = DEF_SITE_OFF_RESERVATION_YIELD;

									}
									//Low Yield시 Site Off
									g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + nX + (nY*STATION_MAX_X_SITE) + (nPressIndex * STATION_MAX_PARA), nSocketOffKind);
									vSetSiteOff.push_back(CPoint(nX, nY));
									vSetSiteKind.push_back(nSocketOffKind);
									bSetSiteOff = TRUE;
								}
							}

							vFailPocket.push_back(CPoint(nX, nY));
							strTmp.Format(_T("Station=%d Site=%d, PassYield[%.2f] \n"), nPressIndex + 1, (nX + 1 + (nY *STATION_MAX_X_SITE)), dTotalYield);
							strErrMsg += strTmp;

							if (bfindpos == false)
								vAlarmSet.push_back(CPoint(nX, nY));

							YieldSampleClear(nPressIndex, nX, nY);
						}
					}
					/*else{
						m_dqSamplePF_Yield[nIdx][nY][nX].pop_front();
					}*/
				}
			}
		}
		if (bLowYield) {
			bOccurAlarm = TRUE;
			if (bSetSiteOff) {
				g_TaskSystemCtrl.SetSiteOnOffData(nPressIndex, vSetSiteOff, vSetSiteKind);
			}
			AlarmData.m_fnClear();
			AlarmData.m_fnAddItemFormat("Set LowYield[%.2f]", dSetLowYield);
			AlarmData.m_fnAddItemFormat(strErrMsg);
			g_Error.AlarmReport(ERR_TESTER_LOW_YIELD_STATION_1+ nPressIndex, AlarmData, this, vFailPocket);
		}

		if (bIsImPossibleSiteOff) {
			AlarmData.m_fnAddItemFormat("No more site off!! Please Check it");
			AlarmData.m_fnAddItemFormat(strFailMsg);
			g_Error.AlarmReport(ERR_INTERLOCK_SITE_OFF_ERROR_STATION_1 + nPressIndex, AlarmData, &g_TaskSystemCtrl);
		}

	//}	

	m_csYieldSample.Unlock();


	BOOL bYieldMonitor   = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Event_Type);
	if(bOccurAlarm == TRUE || bIsImPossibleSiteOff){
		// function alarm stop
		if(eRetestYieldFunc_Stop == bYieldMonitor || bIsImPossibleSiteOff){
			g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
		}
	}
}




void CTaskSystemCtrl::YieldSampleClear()
{
	for(int i=0; i<eMaxPressUnitCount; i++){
		for(int nX=0; nX < STATION_MAX_X_SITE; nX++ ){
			for(int nY=0; nY < STATION_MAX_Y_SITE; nY++ ){
				YieldSampleClear(i, nX, nY);
			}
		}
	}
}

void CTaskSystemCtrl::YieldSampleClear(int nIdx, int nX, int nY )
{
	ASSERT( nIdx >= ePressUnit_Down_1 && nIdx < eMaxPressUnitCount );

	m_csYieldSample.Lock();

	m_dqSamplePF_Yield[nIdx][nY][nX].clear();
	m_vSamplePF_Yield_PreSet[nIdx][nY][nX].clear();

	m_csYieldSample.Unlock();
}

void CTaskSystemCtrl::AddYieldSample(int nIdx, BOOL bPassFailData, int nX, int nY)
{
	int nPreSetCnt  = g_DMCont.m_dmHandlerOpt.GN(NDM11_Preset_Counter);
	int nSampleCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_Sample_Quantity);

	m_csYieldSample.Lock();

	if ((int)m_vSamplePF_Yield_PreSet[nIdx][nY][nX].size() >= nPreSetCnt)
	{
		if ((int)m_dqSamplePF_Yield[nIdx][nY][nX].size() >= nSampleCnt)
		{
			m_dqSamplePF_Yield[nIdx][nY][nX].pop_front();
			m_dqSamplePF_Yield[nIdx][nY][nX].push_back(bPassFailData);
		}
		else {
			m_dqSamplePF_Yield[nIdx][nY][nX].push_back(bPassFailData);
		}
	}
		
	else
		m_vSamplePF_Yield_PreSet[nIdx][nY][nX].push_back(bPassFailData);

	//m_dqContiFail[nIdx][nY][nX].push_back(bPassFailData);

	m_csYieldSample.Unlock();
}


// 1st Fail
void CTaskSystemCtrl::CalcYield1stFailMonitoring(int nPressIndex, std::vector<CPoint> &vAlarmSet)
{
	return;
	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
	int    n1stFailSetCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_1ST_FailCnt);

	BOOL   bMD_YieldMonitor = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_YieldCtrl);
	if (bMD_YieldMonitor != DEF_ON || n1stFailSetCnt == 0) { return; }

	m_csYield1stFail.Lock();

	int nSiteInfo = 0;
	
	
	int    nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	double dSetLowYield = g_DMCont.m_dmHandlerOpt.GD(DDM11_Low_Yield);
	BOOL   bYieldRetestUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Retest_Yield);
	BOOL   bOccurAlarm = FALSE;
	BOOL   bLowYield = FALSE;
	int nSiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	BOOL   bIsPossibleSiteOff = FALSE;
	BOOL   bIsImPossibleSiteOff = FALSE;

	BOOL   bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
	ST_DD_DEVICE stDvcData;

	CParamList AlarmData;
	BOOL bRetFunc = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoKill_1st_Fail);

	std::vector<CPoint> vFailPocket;
	CString strErrMsg = _T("");
	CString strFailMsg = _T("");

	BOOL bSetSiteOff = FALSE;
	std::vector<CPoint> vSetSiteOff;

	//for(int nIdx = 0; nIdx<eMaxPressUnitCount; nIdx++ )
	//{	
	bLowYield = FALSE;
	for (int nY = 0; nY < nSiteCntY; nY++)
	{
		for (int nX = 0; nX < nSiteCntX; nX++)
		{
			bool bfindpos = false;
			for (int i = 0; i < (int)vAlarmSet.size(); i++)
			{
				if (vAlarmSet[i] == CPoint(nX, nY))
				{
					bfindpos = true;
					break;
				}
			}

			if (bfindpos == true)
				Yield1stFailClear(nPressIndex, nX, nY);

			//===================================
			// 1. function on
			// 2. checking test end
			// 3. checking preset count over
			// 4. checking sampling count over
			//===================================
			if (((int)m_dq1stFail_Yield[nPressIndex][nY][nX].size() >= n1stFailSetCnt))
			{
				int nFailCnt = 0;
				for (deque< BOOL >::size_type i = 0; i < (int)m_dq1stFail_Yield[nPressIndex][nY][nX].size(); i++) {
					if (m_dq1stFail_Yield[nPressIndex][nY][nX][i] == eFailBin) { nFailCnt++; }
				}
				if (nFailCnt >= n1stFailSetCnt)
				{
					// inspection retest mode alarm & normal mode alarm
					if ((bYieldRetestUse == TRUE && (nLotStartMode == eStartMD_Retest || nLotStartMode == eStartMD_RetestContinue))
						|| (nLotStartMode != eStartMD_Retest && nLotStartMode != eStartMD_RetestContinue))
					{
						//AlarmData.m_fnAddItemFormat("Station=%d Site=%d , Set LowYield[%.2f], PassYield[%.2f]", nIdx + 1, (nX + 1 + (nY *STATION_MAX_X_SITE)), dSetLowYield, dTotalYield);

						bLowYield = TRUE;

						stDvcData.clear();
						g_TaskPressUnit[nPressIndex].GetSiteDeviceData(&stDvcData, nX, nY);
						BOOL bSiteOff = TRUE;
						if (bAutoRetestUseMode == DEF_ON && stDvcData.sExist == DEF_EXIST && stDvcData.nTestingCnt == eARB_1stTestEnd && stDvcData.nBinColor == eFailBin) {
							bSiteOff = FALSE;
						}
						CString strTmp = _T("");

						if (bRetFunc == DEF_ON /*&& bSiteOff == TRUE*/) {
							bIsPossibleSiteOff = CheckAutoRetestRuleSite(nPressIndex, nX, nY);
							if (bIsPossibleSiteOff == FALSE) {
								bIsImPossibleSiteOff = TRUE;
								strTmp.Format(_T("[Station=%d Site=%d]"), nPressIndex + 1, (nX + 1 + (nY *STATION_MAX_X_SITE)));
								strFailMsg += strTmp;
							}
							else {
								//Low Yield시 Site Off
								g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + nX + (nY*STATION_MAX_X_SITE) + (nPressIndex * STATION_MAX_PARA), DEF_SITE_OFF_RESERVATION);
								vSetSiteOff.push_back(CPoint(nX, nY));
								bSetSiteOff = TRUE;
							}
						}

						vFailPocket.push_back(CPoint(nX, nY));
						strTmp.Format(_T("Station=%d Site=%d, 1st Fail[%d] \n"), nPressIndex + 1, (nX + 1 + (nY *STATION_MAX_X_SITE)), nFailCnt);
						strErrMsg += strTmp;

						if (bfindpos == false)
							vAlarmSet.push_back(CPoint(nX, nY));

						Yield1stFailClear(nPressIndex, nX, nY);
					}
				}
			}
		}
	}
	if (bLowYield) {
		bOccurAlarm = TRUE;
		if (bSetSiteOff) {
			g_TaskSystemCtrl.SetSiteOnOffData(nPressIndex, vSetSiteOff, DEF_SITE_OFF_RESERVATION);
		}
		AlarmData.m_fnClear();
		AlarmData.m_fnAddItemFormat("1st Fail Set Count [%d]", n1stFailSetCnt);
		AlarmData.m_fnAddItemFormat(strErrMsg);
		g_Error.AlarmReport(ERR_TESTER_1ST_FAIL_COUNT_STATION_1 + nPressIndex, AlarmData, this, vFailPocket);
	}

	if (bIsImPossibleSiteOff) {
		AlarmData.m_fnAddItemFormat("No more site off!! Please Check it");
		AlarmData.m_fnAddItemFormat(strFailMsg);
		g_Error.AlarmReport(ERR_INTERLOCK_SITE_OFF_ERROR_STATION_1 + nPressIndex, AlarmData, &g_TaskSystemCtrl);
	}

	//}	

	m_csYield1stFail.Unlock();


	BOOL bYieldMonitor = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Event_Type);
	if (bOccurAlarm == TRUE || bIsImPossibleSiteOff) {
		// function alarm stop
		if (eRetestYieldFunc_Stop == bYieldMonitor || bIsImPossibleSiteOff) {
			g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
		}
	}
}


void CTaskSystemCtrl::AddYield1stFailSample(int nIdx, BOOL bPassFailData, int nX, int nY, int nTestCnt)
{
	int n1stFailCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_1ST_FailCnt);

	m_csYield1stFail.Lock();



	if (nTestCnt == 0 && bPassFailData == eFailBin)
	{
		if (n1stFailCnt > 0)
		{
			if ((int)m_dq1stFail_Yield[nIdx][nY][nX].size() >= n1stFailCnt)
			{
				m_dq1stFail_Yield[nIdx][nY][nX].pop_front();
				m_dq1stFail_Yield[nIdx][nY][nX].push_back(bPassFailData);
			}
			else
				m_dq1stFail_Yield[nIdx][nY][nX].push_back(bPassFailData);

		}
	}
		
			
	m_csYield1stFail.Unlock();
}

void CTaskSystemCtrl::Yield1stFailClear()
{
	for (int i = 0; i < eMaxPressUnitCount; i++) {
		for (int nX = 0; nX < STATION_MAX_X_SITE; nX++) {
			for (int nY = 0; nY < STATION_MAX_Y_SITE; nY++) {
				Yield1stFailClear(i, nX, nY);
			}
		}
	}
}

void CTaskSystemCtrl::Yield1stFailClear(int nIdx, int nX, int nY)
{
	ASSERT(nIdx >= ePressUnit_Down_1 && nIdx < eMaxPressUnitCount);

	m_csYield1stFail.Lock();

	m_dq1stFail_Yield[nIdx][nY][nX].clear();

	m_csYield1stFail.Unlock();
}

BOOL CTaskSystemCtrl::ModuleSiteOffCheck(int nIdx)
{
	BOOL bIsSiteOff = FALSE;
	//int nSocketOffCnt = 0;
	//
	//if( g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1+(nIdx*2)+0) != DEF_SITE_ON ) nSocketOffCnt++;
	//if( g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1+(nIdx*2)+1) != DEF_SITE_ON ) nSocketOffCnt++;
	//if( g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site9+(nIdx*2)+0) != DEF_SITE_ON ) nSocketOffCnt++;
	//if( g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site9+(nIdx*2)+1) != DEF_SITE_ON ) nSocketOffCnt++;
	//
	//if(nSocketOffCnt == 4){
	//	bIsSiteOff = TRUE;
	//}
	return bIsSiteOff;
}



void CTaskSystemCtrl::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[8000] = {0,};
	if(fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf_s(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_SYSTEM), Debug, "", __LINE__, NULL, tmpMsg);
}

void CTaskSystemCtrl::FtpErrorCallBack(CString strErrMsg)
{
	CParamList AlarmData;
	AlarmData.m_fnAddItemFormat("%s",strErrMsg);
	g_Error.AlarmReport(ERR_FTP_ERROR, AlarmData,&g_TaskSystemCtrl);
}

void CTaskSystemCtrl::SetUseUnloadSetplate(ST_CATE_INFO BinCategory)
{
	BOOL bAutoGrrUse      = g_TaskSystemCtrl.GetAutoGrrUse();
	BOOL bAutoLoopUse     = g_TaskSystemCtrl.GetAutoLoopUse();

	if(bAutoGrrUse == TRUE || bAutoLoopUse == TRUE){
		g_unload_stackers.clear();
		g_unload_stackers.push_back(&g_TaskAutoStacker[eSTACKER_04]);
	}
	else {
		//HW Bin 설정 되어있지 않는 Setplate Tray Load 금지
		int nHWBinCount[/*MAX_UNLOAD_SETPLATE*/4] = { 0,0 };

		std::vector<int> vUseUnloadSetplate;
		int nIdx = 0;
		for (int i = 0; i < BinCategory.nIndexCount; i++) {
			nIdx = atoi(BinCategory.aCateInfo[i].szHWBin);

			if (nIdx <= 4 && nIdx > 0) {
				nHWBinCount[nIdx - 1]++;
			}
		}

		g_unload_stackers.clear();

		for (int i = 0; i </*MAX_UNLOAD_SETPLATE*/4; i++) {
			if (nHWBinCount[i] != 0) {
				g_unload_stackers.push_back(&g_TaskAutoStacker[eSTACKER_04 + i]);
			}
		}
	}
}

bool CTaskSystemCtrl::IsOneCycleModeOn()
{
	BOOL bOneCycle = g_DMCont.m_dmEQP.GB( BDM0_SYS_ONE_CYCLE );
	return bOneCycle == TRUE ? true : false;
}

void CTaskSystemCtrl::SetOneCycleMode( int _OnOff )
{
	g_DMCont.m_dmEQP.SB( BDM0_SYS_ONE_CYCLE, _OnOff == DEF_ON ? TRUE : FALSE );
}

void CTaskSystemCtrl::SetCleanDeviceSkipOneCycleMode(int _OnOff)
{
	g_DMCont.m_dmEQP.SB(BDM0_CLEAN_DEVICE_SKIP_SYS_ONE_CYCLE, _OnOff == DEF_ON ? TRUE : FALSE);
}

bool CTaskSystemCtrl::IsCleanDeviceSkipOneCycleModeOn()
{
	BOOL bOneCycle = g_DMCont.m_dmEQP.GB(BDM0_CLEAN_DEVICE_SKIP_SYS_ONE_CYCLE);
	return bOneCycle == TRUE ? true : false;
}


BOOL CTaskSystemCtrl::ChkVisionPosResult(CString strPosResult, CString& strErrMsg, std::vector<CPoint> vPicker, CPoint cTarget, std::vector<CPoint>& vFailPocket,  BOOL* bVisionErrSkip/*= NULL*/)
{
	CString strTemp =_T(""),strErrTemp=_T("");
	int nCnt = 0;
	int nResult = 0;
	int nPosResult[MAX_TEST_PP_PKR_CNT] = {-1,};
//	for(int i =0; i<strPosResult.GetLength(); i++){
// 		if(strPosResult[i] == ','){
// 			nCnt++;
// 		}
//	}

	if(strPosResult.GetLength() != MAX_TEST_PP_PKR_CNT)
		return FALSE;

	for (int i = 0; i<strPosResult.GetLength(); i++)
	{
		nPosResult[i] = atoi(strPosResult.Mid(i,1));
	}
	//for(int i=0; i < MAX_TEST_PP_PKR_CNT_X; i++ )
	//{
		for (std::vector<CPoint>::iterator it = vPicker.begin() ; it != vPicker.end(); ++it)
		{
			//if( it->x == i )
			//{
				//AfxExtractSubString(strTemp, strPosResult, it->x,',');
				//nResult = atoi(strTemp);
				int nIdx = it->x + (it->y * STATION_MAX_X_SITE);
				nPosResult[nIdx];

				if(nPosResult[nIdx] != eCamResult_OK){
					strErrTemp.Format("[%d],", it->x+1 + (it->y * STATION_MAX_X_SITE));
					strErrMsg += strErrTemp;
					vFailPocket.push_back(CPoint(it->x, it->y));
					if( bVisionErrSkip != NULL )
						bVisionErrSkip[nIdx] = TRUE;
				}
			//}
		}
	//}
	

	return TRUE;
}

int CTaskSystemCtrl::CheckAutoRetestRuleSite(int nStationNo, int nSiteX, int nSiteY)
{
	int nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	m_csSiteKill.Lock();

	int nChkSite[STATION_MAX_X_SITE][STATION_MAX_Y_SITE] = { {0,}, };
	int nChkStation = 0;
	int nCnt = 0;
	int nSiteOnOff = DEF_OFF;
	BOOL bRet = FALSE;
	BOOL bIsSiteOK = TRUE, bIsStationOK = TRUE;

	BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
	int  nMaxSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int  nMaxSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	int nTestHandNo = -1;
	switch (nStationNo)
	{
	case ePressUnit_Down_1: case ePressUnit_Down_2: case ePressUnit_Down_3: case ePressUnit_Down_4:
/*	case ePressUnit_Up_1: case ePressUnit_Up_2: case ePressUnit_Up_3: case ePressUnit_Up_4:*/
		{
			nTestHandNo = eTestPP_1;
		}break;
	case ePressUnit_Down_5: case ePressUnit_Down_6: case ePressUnit_Down_7: case ePressUnit_Down_8:
/*	case ePressUnit_Up_5: case ePressUnit_Up_6: case ePressUnit_Up_7: case ePressUnit_Up_8:*/
		{
			nTestHandNo = eTestPP_2;
		}break;
	default:
		break;
	}
	

	for (int nStation = 0; nStation < (int)m_vModuleStationNo[nTestHandNo].size(); nStation++)
	{
		nCnt = 0;
		for (int nY = 0; nY < nMaxSiteY; nY++)
		{
			for (int nX = 0; nX < nMaxSiteX; nX++)
			{
				if (m_vModuleStationNo[nTestHandNo][nStation] == nStationNo && nX == nSiteX && nY == nSiteY ) {
					continue;
				}
				else {
					nSiteOnOff = g_TaskPressUnit[ m_vModuleStationNo[nTestHandNo][nStation] ].GetSocketOnOff(nX, nY);
					if (nSiteOnOff == DEF_SITE_ON) {
						nChkSite[nX][nY] += nSiteOnOff;
						nCnt += 1;
					}
				}
			}
		}
		if (nCnt > 0) {
			nChkStation++;
		}
	}

	for (int nY = 0; nY < nMaxSiteY; nY++)
	{
		for (int nX = 0; nX < nMaxSiteX; nX++)
		{
			if (nChkSite[nX][nY] <= 0) {
				bIsSiteOK = FALSE;
			}
		}
	}

	//Auto Retest Use 일때만 Station 상태를 본다
	//Auto Retest 아니여도 2개 Station 이 있어야 한다.
	//if (bAutoRetestUseMode) {
		if (nChkStation <= 1) {
			bIsStationOK = FALSE;
		}
	//}

	if (/*bIsSiteOK && */bIsStationOK) {
		bRet = TRUE;
	}

	m_csSiteKill.Unlock();

	return bRet;
}

bool CTaskSystemCtrl::checkUphMode()
{
	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
	BOOL bAutoRetetsMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
	BOOL bCleanOption = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_SocketClean);

	if (nRunMode == eRUN_MODE_OFFLINE && bAutoRetetsMode == eOPTION_NOT_USE && bCleanOption == eOPTION_NOT_USE)
		return true;

	return false;
}

void CTaskSystemCtrl::LoadSiteOnOffData()
{
	char szCokName[512] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
	memset(m_nSiteOnOff, 0x00, sizeof(m_nSiteOnOff));

	g_CokMgr.LoadSiteOnOffInfo(szCokName, (int*)m_nSiteOnOff);
}
void CTaskSystemCtrl::SetSiteOnOffData(int nStation, std::vector<CPoint>vPocket, int nSiteOnOff)
{
	char szCokName[512] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));

	for (int i = 0; i < (int)vPocket.size(); i++) {
		m_nSiteOnOff[nStation][vPocket[i].y][vPocket[i].x] = nSiteOnOff;
	}
	g_CokMgr.SaveSiteOnOffInfo(szCokName,nStation, m_nSiteOnOff);
}

void CTaskSystemCtrl::SetSiteOnOffData(int nStation, std::vector<CPoint>vPocket, std::vector<int> vSiteOnOff)
{
	char szCokName[512] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));

	for (int i = 0; i < (int)vPocket.size(); i++) {
		m_nSiteOnOff[nStation][vPocket[i].y][vPocket[i].x] = vSiteOnOff[i];
	}
	g_CokMgr.SaveSiteOnOffInfo(szCokName, nStation, m_nSiteOnOff);
}

void CTaskSystemCtrl::ChangeSiteOffData()
{
	std::vector<CPoint> vSite;

	vSite.clear();

	for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++) {
		for (int nY = 0; nY < STATION_MAX_Y_SITE; nY++)
		{
			for (int nX = 0; nX < STATION_MAX_X_SITE; nX++)
			{
				int nOnOff = g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + nX + nY*STATION_MAX_X_SITE + (nStation * STATION_MAX_PARA));
				if (nOnOff != DEF_SITE_ON && nOnOff != DEF_SITE_OFF && nOnOff != DEF_SITE_SOCKET_HOLD && nOnOff != DEF_SITE_SOCKET_LOCK
					&& nOnOff != DEF_SITE_OFF_RESERVATION_YIELD && nOnOff != DEF_SITE_OFF_RESERVATION) {
					vSite.push_back(CPoint(nX,nY));
					g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + nX + (nY *STATION_MAX_X_SITE) + (nStation * STATION_MAX_PARA), DEF_SITE_OFF);
				}
			}
		}
		if ((int)vSite.size() > 0) {
			SetSiteOnOffData(nStation, vSite, DEF_SITE_OFF);
		}
	}
}

int CTaskSystemCtrl::ChkSocketOnOffInterlock_GRR(CParamList &AlarmData)
{
	int nModuleCnt = 0;
	int nAutoGrrUnitCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrUnitCount);
	BOOL m_bGRR_Audit_loop = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrAuditLoopUse);
	int nModule1On = g_TaskAutoTestPp[eTestPP_1].GetModuleGrrSocketOnCnt(TRUE);
	int nModule2On = g_TaskAutoTestPp[eTestPP_2].GetModuleGrrSocketOnCnt(TRUE);

	for (int i = 0; i < MAX_TEST_PP_CNT; i++) {
		if (g_TaskAutoTestPp[i].GetModuleSocketOnCnt(TRUE) > 0) {
			nModuleCnt++;
		}
	}
	
	if (m_bGRR_Audit_loop != TRUE) {
		if (nModuleCnt <= 0 || nModuleCnt > MAX_TEST_PP_CNT) {
			//Module Count Error
			AlarmData.m_fnAddItemFormat(_T("Station On/Off Datat Error!!"));
			return ERR_GRR_SITE_ERROR;
		}
	}
	
	
// 	if (m_bGRR_Audit_loop) {
// 		if (nModule1On + nModule2On > nAutoGrrUnitCount) {
// 			AlarmData.m_fnAddItemFormat(_T("Audit Device Count Error!!"));
// 			return ERR_GRR_DEVICE_COUNT_ERROR;
// 		}	
// 	}

	int nModuleDvcCnt = (nModuleCnt == 1 ? 8 : 16);

	if (nAutoGrrUnitCount % nModuleCnt != 0) {
		//Device Count Error
		AlarmData.m_fnAddItemFormat(_T("Grr Device Count Error!!"));
		return ERR_GRR_DEVICE_COUNT_ERROR;
	}

	//Site OnOff 한번더 보기

	int nStation_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nStation_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nStationSiteOnCnt = 0;

	
	for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++) {
		nStationSiteOnCnt = 0;
		for (int nY = 0; nY < nStation_Div_Y; nY++) {
			for (int nX = 0; nX < nStation_Div_X; nX++) {
				if (g_DMCont.m_dmHandlerOpt.GN(NDM11_GRR_SocketOnOff_Site1_1 + (nStation * (STATION_MAX_PARA)) + (nY * STATION_MAX_X_SITE) + nX) == DEF_SITE_ON) {
					nStationSiteOnCnt++;
				}
			}
		}
		if (m_bGRR_Audit_loop != TRUE) {
			if (nStationSiteOnCnt != 0 && nStationSiteOnCnt != nStation_Div_Y* nStation_Div_X) {
				AlarmData.m_fnAddItemFormat(_T("Station %d On/Off Datat Error!!"), nStation + 1);
				return ERR_GRR_SITE_ERROR;
			}
		}
	}

	return ERR_NO_ERROR;
}

int CTaskSystemCtrl::ChkSocketOnOffInterlock_AUDIT(CParamList &AlarmData)
{
	int nModuleCnt = 0;
	int nAutoAuditUnitCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoAuditUnitCount);

	int nModule1On = g_TaskAutoTestPp[eTestPP_1].GetModuleAuditSocketOnCnt();
	int nModule2On = g_TaskAutoTestPp[eTestPP_2].GetModuleAuditSocketOnCnt();

	for (int i = 0; i < MAX_TEST_PP_CNT; i++) {
		if (g_TaskAutoTestPp[i].GetModuleAuditSocketOnCnt() > 0) {
			nModuleCnt++;
		}
	}

	if (nModuleCnt <= 0 || nModuleCnt > MAX_TEST_PP_CNT) {
		//Module Count Error
		AlarmData.m_fnAddItemFormat(_T("Station On/Off Data Error!!"));
		return ERR_AUDIT_SITE_ERROR;
	}


	//Site OnOff 한번더 보기

	int nStation_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nStation_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nStationSiteOnCnt = 0;


	for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++) {
		for (int nY = 0; nY < nStation_Div_Y; nY++) {
			for (int nX = 0; nX < nStation_Div_X; nX++) {
				if (g_DMCont.m_dmHandlerOpt.GN(NDM11_Audit_SocketOnOff_Site1_1 + (nStation * (STATION_MAX_PARA)) + (nY * STATION_MAX_X_SITE) + nX) == DEF_SITE_ON) {
					nStationSiteOnCnt++;
				}
			}
		}
	}


	if (nAutoAuditUnitCount > nStationSiteOnCnt) {
		AlarmData.m_fnAddItemFormat(_T("Audit Unit Count = %d, AuditSocketOnOff = %d MISS MATCH"), nAutoAuditUnitCount, nStationSiteOnCnt);
		return ERR_AUDIT_DEVICE_COUNT_ERROR;
	}

	return ERR_NO_ERROR;
}

void CTaskSystemCtrl::CalcYieldContiFailMonitoring_Station(int nPressIndex, std::vector<CPoint> &vAlarmSet)
{
	BOOL   bMD_YieldMonitor = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_YieldCtrl);
	if (bMD_YieldMonitor != DEF_ON) { return; }

	m_csYieldContiFail.Lock();

	int nSiteInfo = 0;

	int   nFailSetCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_ContinueFail_FailCnt_Station);
	BOOL  bRetFunc = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Consecutive_Fail_AutoKill_Station);

	BOOL  bOccurAlarm = FALSE;
	BOOL  bFailCntOver = FALSE;
	BOOL  bImPossibleSiteOff = FALSE;

	int   nSiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int   nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	BOOL   bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);

	int nStationAllFailCnt = g_TaskPressUnit[nPressIndex].GetAllStationFailCnt();
	BOOL bSiteOff = TRUE;
	ST_DD_DEVICE stDvcData;
	BOOL   bIsPossibleSiteOff = FALSE;

	std::vector<CPoint> vFailPocket;
	CString strErrMsg = _T("");
	CString strFailMsg = _T("");
	
	BOOL bSetSiteOff = FALSE;
	std::vector<CPoint> vSetSiteOff;

	int nSiteOnOff = DEF_SITE_OFF;

	int	   nSocketOffKind = DEF_SITE_OFF_YIELD;

	std::vector<int> vSetSiteKind;
	vSetSiteKind.clear();

	if (nStationAllFailCnt >= nFailSetCnt) {
		for (int nY = 0; nY < nSiteCntY; nY++)
		{
			for (int nX = 0; nX < nSiteCntX; nX++)
			{
				bool bfindpos = false;
				for (int i = 0; i < (int)vAlarmSet.size(); i++)
				{
					if (vAlarmSet[i] == CPoint(nX, nY))
					{
						bfindpos = true;
						break;
					}
				}
				if (bfindpos == true)
					YieldContiFailClear(nPressIndex, nX, nY);

				stDvcData.clear();
				g_TaskPressUnit[nPressIndex].GetSiteDeviceData(&stDvcData, nX, nY);

				nSiteOnOff = g_TaskPressUnit[nPressIndex].GetSocketOnOff(nX, nY);

				if (nSiteOnOff == DEF_SITE_ON) {
					bSiteOff = TRUE;
					if (bAutoRetestUseMode == DEF_ON && stDvcData.sExist == DEF_EXIST && stDvcData.nTestingCnt == eARB_1stTestEnd && stDvcData.nBinColor == eFailBin) {
						bSiteOff = FALSE;
					}

					CString strErrPos = _T("");
					if (bRetFunc == DEF_ON) {
						bIsPossibleSiteOff = CheckAutoRetestRuleSite(nPressIndex, nX, nY);
						if (bIsPossibleSiteOff == FALSE) {
							bImPossibleSiteOff = TRUE;
							strErrPos.Format(_T("[Station=%d Site=%d]"), nPressIndex + 1, (nX + 1 + (nY * STATION_MAX_X_SITE)));
							strFailMsg += strErrPos;
						}
						else {
							if (bSiteOff == TRUE) {
								nSocketOffKind = DEF_SITE_OFF_YIELD;
							}
							else {
								nSocketOffKind = DEF_SITE_OFF_RESERVATION_YIELD;

							}

							//consecutive Fail 시 Site Off
							g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + nX + (nY*STATION_MAX_X_SITE) + (nPressIndex * STATION_MAX_PARA), nSocketOffKind);
							vSetSiteOff.push_back(CPoint(nX, nY));
							vSetSiteKind.push_back(nSocketOffKind);
							bSetSiteOff = TRUE;
						}

					

						strErrPos.Format(_T("[Station=%d Site=%d Count=%d]\n"), nPressIndex + 1, (nX + 1 + (nY * STATION_MAX_X_SITE)), nStationAllFailCnt);
						strErrMsg += strErrPos;
						bFailCntOver = TRUE;
						YieldContiFailClear(nPressIndex, nX, nY);
						vFailPocket.push_back(CPoint(nX, nY));

						if (bfindpos == false)
							vAlarmSet.push_back(CPoint(nX, nY));
					}
				}
			}//nX
		}//nY
	}

	CParamList AlarmData;

	if (bFailCntOver) {
		bOccurAlarm = TRUE;
		if (bSetSiteOff) {
			g_TaskSystemCtrl.SetSiteOnOffData(nPressIndex, vSetSiteOff, vSetSiteKind);
		}

		g_TaskPressUnit[nPressIndex].ClearAllStationFailCnt();
		AlarmData.m_fnClear();
		AlarmData.m_fnAddItemFormat("Occurred consecutive Station (All Site) Failure Alarm!!!!");
		AlarmData.m_fnAddItemFormat(strErrMsg);
		g_Error.AlarmReport(ERR_TESTER_CONSECUTIVE_FAIL_STATION_1 + nPressIndex, AlarmData, this, vFailPocket);
		strErrMsg.Empty();
	}

	if (bImPossibleSiteOff)
	{
		AlarmData.m_fnClear();
		AlarmData.m_fnAddItemFormat("No more site off!!Please Check it");
		AlarmData.m_fnAddItemFormat(strFailMsg);
		g_Error.AlarmReport(ERR_INTERLOCK_SITE_OFF_ERROR_STATION_1 + nPressIndex, AlarmData, &g_TaskSystemCtrl);
	}

	m_csYieldContiFail.Unlock();

	if (bOccurAlarm == TRUE || bImPossibleSiteOff) {
		BOOL bYieldMonitor = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Event_Type);
		// function alarm stop
		if (eRetestYieldFunc_Stop == bYieldMonitor || bImPossibleSiteOff) {
			g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
		}
	}
}

void CTaskSystemCtrl::UpdateDailyReport()
{
	if (m_dqDailyData.empty()) {
		m_csDailyReport.Lock();
		deque<ST_DAILY_REPORT>().swap(m_dqDailyData);
		m_csDailyReport.Unlock();
		return;
	}

	CTime cTmCurrent = CTime::GetCurrentTime();
	CString strCurTime = _T("");
	strCurTime.Format("%04d-%02d-%02d", cTmCurrent.GetYear(), cTmCurrent.GetMonth(), cTmCurrent.GetDay());

	m_csDailyReport.Lock();
	ST_DAILY_REPORT Data = m_dqDailyData.front();
	m_dqDailyData.pop_front();

	m_csDailyReport.Unlock();

	g_DB.UPDATE_DAILY_DATA_TBL(strCurTime, Data);	
}

void CTaskSystemCtrl::AddDailyReportData(ST_DAILY_REPORT Data)
{
	m_csDailyReport.Lock();

	m_dqDailyData.push_back(Data);

	m_csDailyReport.Unlock();
}

CString CTaskSystemCtrl::GetHistoryStageName(int nCmdStage)
{
	CString strLoc = _T("");
	if (nCmdStage >= HISTORY_LOC_IDX_TRAY_PP_SHT_1 && nCmdStage <= HISTORY_LOC_IDX_TEST_PP2_CLEAN_BUFFER) {
		switch (nCmdStage) 
		{
			case HISTORY_LOC_IDX_TRAY_PP_CONV_1: { strLoc.Format("Conveyor1"); } break;
			case HISTORY_LOC_IDX_TRAY_PP_CONV_2: { strLoc.Format("Conveyor2"); } break;
			case HISTORY_LOC_IDX_TRAY_PP_CONV_3: { strLoc.Format("Conveyor3"); } break;
			case HISTORY_LOC_IDX_TRAY_PP_CONV_4: { strLoc.Format("Conveyor4"); } break;
			case HISTORY_LOC_IDX_TRAY_PP_CONV_5: { strLoc.Format("Conveyor5"); } break;
			case HISTORY_LOC_IDX_TRAY_PP_CONV_6: { strLoc.Format("Conveyor6"); } break;
			case HISTORY_LOC_IDX_TRAY_PP_CONV_7: { strLoc.Format("Conveyor7"); } break;
			case HISTORY_LOC_IDX_TRAY_PP_SHT_1 : { strLoc.Format("LoadTable1"); } break;
			case HISTORY_LOC_IDX_TRAY_PP_SHT_2 : { strLoc.Format("LoadTable2"); } break;
			
			case HISTORY_LOC_IDX_TEST_PP1_LD_TBL_1 :{ strLoc.Format("LOAD TABLE 2, LOCATION 1"); } break;
			case HISTORY_LOC_IDX_TEST_PP1_LD_TBL_2 :{ strLoc.Format("LOAD TABLE 2, LOCATION 2"); } break;
			case HISTORY_LOC_IDX_TEST_PP1_LD_TBL_3 :{ strLoc.Format("LOAD TABLE 2, LOCATION 3"); } break;
			case HISTORY_LOC_IDX_TEST_PP1_LD_TBL_4 :{ strLoc.Format("LOAD TABLE 2, LOCATION 4"); } break;

			case HISTORY_LOC_IDX_TEST_PP1_UD_TBL_1: { strLoc.Format("LOAD TABLE 1, LOCATION 1"); } break;
			case HISTORY_LOC_IDX_TEST_PP1_UD_TBL_2: { strLoc.Format("LOAD TABLE 1, LOCATION 2"); } break;
			case HISTORY_LOC_IDX_TEST_PP1_UD_TBL_3: { strLoc.Format("LOAD TABLE 1, LOCATION 3"); } break;
			case HISTORY_LOC_IDX_TEST_PP1_UD_TBL_4: { strLoc.Format("LOAD TABLE 1, LOCATION 4"); } break;


			case HISTORY_LOC_IDX_TEST_PP1_STATION_DN_1 :{ strLoc.Format("Station 1"); } break;
			case HISTORY_LOC_IDX_TEST_PP1_STATION_DN_2 :{ strLoc.Format("Station 2"); } break;
			case HISTORY_LOC_IDX_TEST_PP1_STATION_DN_3 :{ strLoc.Format("Station 3"); } break;
			case HISTORY_LOC_IDX_TEST_PP1_STATION_DN_4 :{ strLoc.Format("Station 4"); } break;
			case HISTORY_LOC_IDX_TEST_PP1_CLEAN_BUFFER :{ strLoc.Format("CLEAN BUFFER"); } break;

			case HISTORY_LOC_IDX_TEST_PP2_LD_TBL_1: { strLoc.Format("LOAD TABLE 3, LOCATION 1"); } break;
			case HISTORY_LOC_IDX_TEST_PP2_LD_TBL_2: { strLoc.Format("LOAD TABLE 3, LOCATION 2"); } break;
			case HISTORY_LOC_IDX_TEST_PP2_LD_TBL_3: { strLoc.Format("LOAD TABLE 3, LOCATION 3"); } break;
			case HISTORY_LOC_IDX_TEST_PP2_LD_TBL_4: { strLoc.Format("LOAD TABLE 3, LOCATION 4"); } break;
										
			case HISTORY_LOC_IDX_TEST_PP2_UD_TBL_1: { strLoc.Format("LOAD TABLE 4, LOCATION 1"); } break;
			case HISTORY_LOC_IDX_TEST_PP2_UD_TBL_2: { strLoc.Format("LOAD TABLE 4, LOCATION 2"); } break;
			case HISTORY_LOC_IDX_TEST_PP2_UD_TBL_3: { strLoc.Format("LOAD TABLE 4, LOCATION 3"); } break;
			case HISTORY_LOC_IDX_TEST_PP2_UD_TBL_4: { strLoc.Format("LOAD TABLE 4, LOCATION 4"); } break;

			case HISTORY_LOC_IDX_TEST_PP2_STATION_DN_1 :{ strLoc.Format("Station 5"); } break;
			case HISTORY_LOC_IDX_TEST_PP2_STATION_DN_2 :{ strLoc.Format("Station 6"); } break;
			case HISTORY_LOC_IDX_TEST_PP2_STATION_DN_3 :{ strLoc.Format("Station 7"); } break;
			case HISTORY_LOC_IDX_TEST_PP2_STATION_DN_4 :{ strLoc.Format("Station 8"); } break;
			case HISTORY_LOC_IDX_TEST_PP2_CLEAN_BUFFER :{ strLoc.Format("CLEAN BUFFER"); } break;
			default: {strLoc.Format("NONE"); }break;
		}

	}
	else {
		strLoc.Format("NONE");
	}

	return strLoc;
}

void CTaskSystemCtrl::SetAutoLoopCancel(bool bSet)
{
	g_DMCont.m_dmEQP.SB(BDM0_SYS_AUTO_LOOP_CANCEL, bSet);
}

BOOL CTaskSystemCtrl::GetAutoLoopCancel()
{
	BOOL bRet = g_DMCont.m_dmEQP.GB(BDM0_SYS_AUTO_LOOP_CANCEL);
	return bRet;
}

int CTaskSystemCtrl::GetErrTestPPType(int nIdx)
{
	int nHandType = 0;
	switch (nIdx)
	{
	case 0: 
	case 2: 
	{nHandType = eTestPPLoadType; break; }	
	case 1: 
	case 3: 
	{nHandType = eTestPPUnLoadType; break; }
		
	default:
		break;
	}

	return nHandType;
}

int CTaskSystemCtrl::MoveJog(int nAxis, int nMoveDir, double dMoveSpeed/*um/s*/)
{
	ASSERT(0 <= nAxis && nAxis < g_Motor.GetAxisCount());
	ASSERT(-1 <= nMoveDir && nMoveDir <= 1);
	//ASSERT(0 <= nMoveSpeed && nMoveSpeed <= 2);

	if (g_Motor[nAxis] == NULL)
		return ERR_FAILURE;

	CSystemCmd_AxisJogMove* pCmd = new CSystemCmd_AxisJogMove(nAxis, nMoveDir, dMoveSpeed);

	//jog 다른 커맨드 실행시 return.
 	int nTaskCount = CBaseTaskManager::GetObserverCount();
 	for (int i = 0; i < nTaskCount; i++) {
 		CBaseTaskManager* pTask = CBaseTaskManager::GetTaskByIndex(i);
 		int ntest = pTask->GetSeqCmdStatus(eEqpCtrlMode_Manual);
 		if (pTask->GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
 			return ERR_SUCCESS;
 		}
 	}
// 	if (g_Motor[nAxis]->MotionDone() != ERR_NO_ERROR)
// 	{
// 		return ERR_SUCCESS;
// 	}

	if (SendCommand(g_TaskSystemCtrl, pCmd, eEqpCtrlMode_Manual) != ERR_SUCCESS)
	{
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Axis=%s", g_Motor[nAxis]->GetAxisName());
		g_Error.AlarmReport(ERR_SYS_CMD_JOG_FAIL, AlarmData, &g_TaskSystemCtrl);		
		return ERR_FAILURE;
	}
	else
		return ERR_SUCCESS;
}

int CTaskSystemCtrl::StopJog()
{
	g_TaskSystemCtrl.SetProcessCommand(base_seq_cmd::eProcCmd_Stop, eEqpCtrlMode_Manual);
	return ERR_SUCCESS;
}

int CTaskSystemCtrl::MoveInitial(int nAxis)
{
	ASSERT(0 <= nAxis && nAxis < g_Motor.GetAxisCount());
	if (g_Motor[nAxis] == NULL)
		return ERR_FAILURE;

	CSystemCmd_Axis_Initial* pCmd = new CSystemCmd_Axis_Initial(nAxis);
	if (SendCommand(g_TaskSystemCtrl, pCmd, eEqpCtrlMode_Manual) != ERR_SUCCESS)
	{
		CParamList AlarmData;
		int nAlarm =  ERR_MOTOR_CMN_ORIGIN_FAILURE;
		AlarmData.m_fnAddItemFormat("Axis=%s", g_Motor[nAxis]->GetAxisName());
		g_Error.AlarmReport(nAlarm, AlarmData, &g_TaskSystemCtrl);
		return ERR_FAILURE;
	}
	else
		return ERR_SUCCESS;

}


// main power drop 시에는 soft servo re connect 해야한다.
int CTaskSystemCtrl::ReConnectSoftServoDriver(eAxisServoConfiguration Configuration)
{
	int nAxisCount = g_Motor.GetAxisCount();
	int nEngineFlags = (int)(eAxisServoConfiguration::RestartEngine | eAxisServoConfiguration::ReconnectCommunication);

	eAxisServoConfiguration EngineConfiguration = eAxisServoConfiguration((int)Configuration & nEngineFlags);

	for (int i = 0; i < nAxisCount; i++)
	{
		auto* pAxis = g_Motor[i];
		bool bResult = false;

		bResult = pAxis->ApplyConfiguration(eAxisServoConfiguration((int)Configuration & ~nEngineFlags), TRUE);

		if (bResult == false)
			return -1;
	}

	return ERR_NO_ERROR;
}

void CTaskSystemCtrl::SetQAStatus(eQAStatus newStatus)
{
	g_DMCont.m_dmEQP.SN(NDM0_QA_Status, newStatus);
}

int CTaskSystemCtrl::GetQAStatus()
{
	return g_DMCont.m_dmEQP.GN(NDM0_QA_Status);
}

int CTaskSystemCtrl::GetQASampleCount()
{
	char szLotSize[64] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_QTY, szLotSize, sizeof(szLotSize));
	int nQaSampleCnt = atoi(szLotSize);
	return nQaSampleCnt;
}


void CTaskSystemCtrl::ChkFeederTrayEnd(BOOL bTrayEmptyCheck)
{
	bool feeder_tray_empty_device = g_TaskTrayFeeder.IsEmptyDevice();
	bool bIsEmptyTrayFeeder_Data = g_DMCont.m_dmDevice.IsEmptyTray(eDeviceStage_TRAY_FEEDER) == FALSE;

	BOOL bFeederTrayEnd = g_DMCont.m_dmFeeder.GB(BDM10_FEEDER_TRAY_END);

	if (feeder_tray_empty_device && bTrayEmptyCheck == bIsEmptyTrayFeeder_Data || bFeederTrayEnd)
	{
		SetQAStatus(eQAStatus::eQAStatus_End);
	}
}


void CTaskSystemCtrl::QaProcessLoad(int nQaSampleCnt)
{
	int nQaSampleAddSumLoadCount = g_DMCont.m_dmEQP.GN(NDM0_QaSampleAddSumLoadCnt);
	int nCycleSortDvc = g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt);
	if (nCycleSortDvc >= nQaSampleCnt + nQaSampleAddSumLoadCount)
	{
		SetQAStatus(eQAStatus::eQAStatus_LoadEnd);
	}

	ChkFeederTrayEnd(DEF_EXIST);
}

void CTaskSystemCtrl::QaProcessLoadEnd(int nQaSampleCnt)
{
	int nCurPassCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotPassBinCnt);

	if (nQaSampleCnt != nCurPassCnt)
	{
		ChkFeederTrayEnd(DEF_EXIST);

		int nQaSampleAddLoadCount = CalcQaSampleAddLoadCount(nQaSampleCnt, nCurPassCnt);
		UpdateQaSampleLoadCount(nQaSampleAddLoadCount);

		SetQAStatus(eQAStatus::eQAStatus_Load);
	}
	else
		SetQAStatus(eQAStatus::eQAStatus_EmptyLoad);
}

void CTaskSystemCtrl::QaProcessLoadEmpty()
{
	ChkFeederTrayEnd(DEF_NOT_EXIST);
}

int CTaskSystemCtrl::CalcQaSampleAddLoadCount(int nQaSampleCnt, int nCurPassCnt)
{
	return nQaSampleCnt - nCurPassCnt;
}

void CTaskSystemCtrl::UpdateQaSampleLoadCount(int nQaSampleAddLoadCount)
{
	//QA 설정 수량 Pass 수량 계산 하여 추가 투입 수량 계산
	int nQaSampleLoadCnt = g_DMCont.m_dmEQP.GN(NDM0_QaSampleLoadCnt);
	g_DMCont.m_dmEQP.SN(NDM0_QaSampleLoadCnt, nQaSampleLoadCnt - nQaSampleAddLoadCount);

	//QA 총 추가 투입 누적 수량 계산
	int nQaSampleAddSumLoadCount = g_DMCont.m_dmEQP.GN(NDM0_QaSampleAddSumLoadCnt);
	g_DMCont.m_dmEQP.SN(NDM0_QaSampleAddSumLoadCnt, nQaSampleAddSumLoadCount + nQaSampleAddLoadCount);
}






