
// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// SLT_MC.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"


// 해당 class는 auto thread 보다 먼저 생성 되도록 함.
CDlgHomeCheckStatus*      g_pDlgHomeCheckStatus=NULL;
CDlgManuMotorTeaching*      g_pDlgManuMotorTeaching = NULL;

CLibCommCtrl			  g_LibCommCtrl;
CDataMemCont			  g_DMCont;
CDatabaseIF				  g_DB;
CDatabaseIF				  g_DBErr;
CDatabaseIF				  g_DBTeach;
CDatabaseIF				  g_DBLotHistory;
CDatabaseIF				  g_DBVAT;

CMotion_Cont			  g_Motor;
CDIO_Cont				  g_IO;
CAIO_Cont				  g_AIO;
CCokFileManager			  g_CokMgr;
CGUIComm                  g_GuiComm;
CVisionComm_Barcode*      g_pBarcode = NULL;
CVisionComm_TestHand_1*	  g_pVisionTestHand_1;
CVisionComm_TestHand_2*	  g_pVisionTestHand_2;

// CVisionComm_VAT_PC1*          g_pVAT_PC1 = NULL;
// CVisionComm_VAT_PC2*          g_pVAT_PC2 = NULL;


CTesterIF			      g_AppleTestIF;

//SoftServo
SoftServoControl			g_SoftServoCtrl;

COPSwitchAct_Cover        g_COVER_SW;
COPSwitchAct_START        g_START_SW;
COPSwitchAct_STOP         g_STOP_SW;
COPSwitchAct_Lamp         g_LAMP_SW;
COPSwitchAct              g_SOUND_SW;
COPSwitchAct              g_FR_SELECT_SW;
COPSwitchAct              g_TowerR;
COPSwitchAct              g_TowerY;
COPSwitchAct              g_TowerG;
COPSwitchAct              g_TowerBuz;
//COPSwitchAct_UndStackerUnload g_UnderStackerUnload_SW;
COPSwitchAct_StepTrayPP   g_StepTrayPP_SW[eMaxTrayPPCount];
COPSwitchAct_StepTestPP   g_StepTestPP_SW[eMaxTestPPCount];

COPSwitchAct_Feeder		  g_Feeder_SW;

CError                    g_Error;


//CTaskAutoTestSite         g_TaskAutoTestSite[eMaxTestPPCount];
//CTaskUnderStacker	      g_TaskUnderStackerUD;
//CTaskSetplate		      g_TaskSetplate[eMaxSetplateCount];

CRegulatorIF			  g_RegulatorIF[eMaxPressRegul];

CTaskSystemCtrl           g_TaskSystemCtrl;
CTestHandWorkArea		  g_testpp_work_area[eMaxTestPPCount];

// Task
CTaskPressUnit            g_TaskPressUnit[eMaxPressUnitCount];
CTaskTransfer			  g_TaskTransfer;
CTaskTestPP               g_TaskTestPP[eMaxTestPPCount];
CTaskTrayLoadPP			  g_TaskTrayLoadPP;
CTaskTrayUnloadPP		  g_TaskTrayUnloadPP;
CTaskLoadTable            g_TaskLoadTbl[eMaxLoadTblCount];
CTaskTowerLight           g_TaskTowerLight;
CTaskRegulator            g_TaskRegulator[eMaxPressUnitCount];
CTaskVibrator             g_TaskVibrator;
CTaskStacker			  g_TaskStacker[ eMaxStackerCount ];
CTaskTrayFeeder			  g_TaskTrayFeeder;

// Auto Task				
CTaskAutoSystem			  g_TaskAutoSystem;
CTaskAutoTrayLoadPP		  g_TaskAutoTrayLoadPP;
CTaskAutoTrayUnloadPP     g_TaskAutoTrayUnloadPP;
CTaskAutoStacker          g_TaskAutoStacker[ eMaxStackerCount ];
CAllAutoStacker			  g_TaskAllAutoStacker;
CTaskAutoPress            g_TaskAutoPress[ eMaxPressUnitCount ];
CTaskAutoTestPp           g_TaskAutoTestPp[ eMaxTestPPCount];
CAllAutoTestSite          g_TaskAllTestSite;
CTaskAutoLoadTable        g_TaskAutoLdTable[ eMaxLoadTblCount];
CTaskAutoTransfer         g_TaskAutoTransfer;
CTaskAutoTrayFeeder		  g_TaskAutoTrayFeeder;

//VAT
//CVatBaseTask			  g_VatBaseTask;
BOOL					  g_bVATStatusExecute; //Manual 및 Auto에서 동작 Execute 상태 확인.

CVAT_CommonData			  g_VATCommonData;
CVAT_CommonStaticData	  g_VATCommonStaticData;

CVAT_DataCopy			  g_VATDataCopy;

CVAT_TaskSystemCtrl		  g_VatTaskSystemCtrl;
CVAT_TaskAutoSystem		  g_VatTaskAutoSystem;

CVAT_TaskAutoTrayLoadPP   g_VatTaskAutoTrayLoadPP;
CVAT_TaskAutoTrayUnloadPP g_VatTaskAutoTrayUnloadPP;
CVAT_TaskAutoTestPP		  g_VatTaskAutoLoadTestPP1; //Part상태 독립적으로 관리하기 위해 Test pp Hand 분기.
CVAT_TaskAutoTestPP		  g_VatTaskAutoUnloadTestPP1;
CVAT_TaskAutoTestPP		  g_VatTaskAutoLoadTestPP2;
CVAT_TaskAutoTestPP		  g_VatTaskAutoUnloadTestPP2;

CVAT_VisionMgr			 g_VatVisionMgr[VAT_CAM_MAX];

//////////

CDlgSimulDioSLT*          g_pDlgSimulDioSLT = NULL;
CDlgDebugAutoStacker*     g_pDlgDebugView = NULL;

//CDlgMon*				  g_pDlgMotorMonitor = NULL;

CProcJamRate              g_JamRate;
ST_CATE_INFO              g_BinCategory; //bin category list

CNetDriver                g_NetDriver;
//CmmwinFtp*                g_pFtpObj=NULL;
CVision_2dBarcode         g_2DBarcodeChk;
CTriggerGenerator		 g_trigger_generator;
//AABRuleCheck              g_ART_Rule;

//ST_VAT_3POINT             g_St3PointTeach;
CTaskMultiOrg		      g_MulitOrg;


CMasterMac				 g_MasterMac;

CBarcodeList			g_BarcodeList;


CLogZip					g_LogZip;

CCylinderTimeCheck	 g_CylinderTime;
//COEEFile				  g_OEEFile;
//CFtpManager				  g_Ftp;

ST_Set_DefaultBin         g_DefaultBinCategory;

ST_CONVEYOR_WORK_AREA	g_conveyor_work_area;

ST_TRANSFER_FEEDER_WORK_AREA	g_Transfer_Feeder_work_area;

ST_TESTPP_TABLE_WORK_AREA	g_TestPP_Table_work_area[eMaxTestPPCount];

std::vector< CTaskAutoStacker* > g_load_stackers;
std::vector< CTaskAutoStacker* > g_unload_stackers;
std::vector< CTaskAutoStacker* > g_empty_unload_stackers;
std::vector< CTaskAutoStacker* > g_empty_load_stackers;



CBbxmnpClient*			g_pBbxmnpClient;

/*CConectSocket*			g_pConectSocket;*/
SOCKET				g_UDPSockBinSolution;
SOCKADDR_IN			g_udpserverAddr;

SYSTEMTIME g_SOT[eMaxPressUnitCount];
SYSTEMTIME g_EOT[eMaxPressUnitCount];


///////////////Interlock 절대 위치 정의 Define -> Global 변수로 이동 

double  MIN_TRAY_PP_WORK_ENABLE_POS_um = (-50000);


//Tray Load Hand
double INTERLOCK_LD_TRAYPP_X_WORK_ENABLE_FEEDER_START_POS = (292000.0); //Tray Load X Feeder Left 간섭 구간.
double INTERLOCK_LD_TRAYPP_X_WORK_ENABLE_FEEDER_END_POS = (741500.0); //Tray Load X Feeder Right 간섭 구간.
double INTERLOCK_LD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_LEFT = (292000.0); //Tray Load X Feeder Left 간섭 구간.
double INTERLOCK_LD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_RIGHT = (741500.0); //Tray Load X Feeder Right 간섭 구간.

//Tray Unload Hand
double INTERLOCK_UD_TRAYPP_Y_WORK_ENABLE_POS_TRANSFER = (17300.0); //Tray Unload Y Transfer 간섭 구간.

//Base Tray Y Table Z축 작업 가능 영역
double INTERLOCK_BASE_TRAYPP_Y_POS_WITH_TRAYPP_Z_AND_TBL_POSSIBLE = (-90000); // Base Tray Y Table Z 간섭 구간

double INTERLOCK_UD_TRAYPP_X_WORK_ENABLE_FEEDER_START_POS = (324000.0); //Tray Unload X Feeder Left 간섭 구간.
double INTERLOCK_UD_TRAYPP_X_WORK_ENABLE_FEEDER_END_POS = (781500.0); //Tray Unload X Feeder Right 간섭 구간.
//double INTERLOCK_UD_TRAYPP_Y_WORK_ENABLE_TABLE
//double INTERLOCK_UD_TRAYPP_X_WORK_

double INTERLOCK_UD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_LEFT = (324000.0); //Tray Load X Feeder Left 간섭 구간.
double INTERLOCK_UD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_RIGHT=(781500.0); //Tray Load X Feeder Right 간섭 구간.

//Test Hand
double INTERLOCK_TESTPP_1_X_STATION_START_POS = (-285000.0); //-136000.0 * 2							 
double INTERLOCK_TESTPP_2_X_STATION_START_POS = (282000.0); //136000.0 * 2
double INTERLOCK_TESTPP_X_POS_WITH_TESTPP_Z_AND_TBL_FRONT = (182500.0 * 2); //Table Front 위치
double INTERLOCK_TESTPP_X_POS_WITH_TESTPP_Z_AND_TBL_REAR = (66300 * 2); //Table Rear 위치 //81350.0
double INTERLOCK_TESTPP_X_SAFETY_POS = (133000.0 * 2); //282 //133000.0 *2

double INTERLOCK_HAND_LENGTH = (274000.0);
double INTERLOCK_STATION_GAP = (10000.0);
double INTERLOCK_TESTPP_Y_STATION_1_START_POS_LOAD = (-1300000.0); // Test PP Load Y축 Station 1 간섭 시작 위치. 
double INTERLOCK_TESTPP_Y_STATION_1_END_POS_LOAD = (-712000.0); // Test PP Load Y축 Station 1 간섭 종료 위치. // -722
double INTERLOCK_TESTPP_Y_STATION_2_START_POS_LOAD = (-930000.0); // Test PP Load Y축 Station 2 간섭 시작 위치. 
double INTERLOCK_TESTPP_Y_STATION_2_END_POS_LOAD = (-369000.0); // Test PP Load Y축 Station 2 간섭 종료 위치. // -379
double INTERLOCK_TESTPP_Y_STATION_3_START_POS_LOAD = (-580000.0); // Test PP Load Y축 Station 3 간섭 시작 위치. 
double INTERLOCK_TESTPP_Y_STATION_3_END_POS_LOAD = (-33000.0); // Test PP Load Y축 Station 3 간섭 종료 위치. // -43
double INTERLOCK_TESTPP_Y_STATION_4_START_POS_LOAD = (-247000.0); // Test PP Load Y축 Station 4 간섭 종료 위치. 
double INTERLOCK_TESTPP_Y_STATION_4_END_POS_LOAD = (40000.0); // Test PP Load Y축 Station 4 간섭 종료 위치. 

double INTERLOCK_TESTPP_Y_STATION_1_START_POS_UNLOAD = (-1300000.0); // Test PP Unload Y축 Station 1 간섭 시작 위치. 
double INTERLOCK_TESTPP_Y_STATION_1_END_POS_UNLOAD = (-959000.0); // Test PP Unload Y축 Station 1 간섭 종료 위치. //-969
double INTERLOCK_TESTPP_Y_STATION_2_START_POS_UNLOAD = (-1214000.0); // Test PP Unload Y축 Station 2 간섭 시작 위치. 
double INTERLOCK_TESTPP_Y_STATION_2_END_POS_UNLOAD = (-609000.0); // Test PP Unload Y축 Station 2 간섭 종료 위치. //-619
double INTERLOCK_TESTPP_Y_STATION_3_START_POS_UNLOAD = (-870000.0); // Test PP Unload Y축 Station 3 간섭 시작 위치. 
double INTERLOCK_TESTPP_Y_STATION_3_END_POS_UNLOAD = (-273000.0); // Test PP Unload Y축 Station 3 간섭 종료 위치. //-283
double INTERLOCK_TESTPP_Y_STATION_4_START_POS_UNLOAD = (-529000.0); // Test PP Unload Y축 Station 4 간섭 종료 위치. 
double INTERLOCK_TESTPP_Y_STATION_4_END_POS_UNLOAD = (40000.0); // Test PP Unload Y축 Station 4 간섭 종료 위치. 

double INTERLOCK_TESTPP1_X_CLEAN_TABLE_LIMIT = (-375000.0);
double INTERLOCK_TESTPP2_X_CLEAN_TABLE_LIMIT = (375000.0);

double INTERLOCK_TESTPP_Y_CLEAN_TABLE_LIMIT = (-1200000.0);

//Loading Table
double INTERLOCK_LOADING_TBL_POS_BOUNDARY = (-1700000.0);

//Press
double INTERLOCK_PRESS_Z_SAFETY_POS = (0.0);	//Press Z Safety Position

//feeder
double INTERLOCK_FEEDER_SAFETY_POS_ENC = (-330000.0); //Tray Feeder 안전 위치 확인(Safety Sensor 감지 되는 위치).
double INTERLOCK_FEEDER_POS_BOUNDARY = (-148000.0); //Tray Feeder Unload Y축 간섭 시작 위치.

double INTERLOCK_LD_TRAY_PP_PKR1_FEEDER_LEFT_IMPACT_POS = (0.0);
double INTERLOCK_LD_TRAY_PP_PKR2_FEEDER_LEFT_IMPACT_POS = (0.0);
double INTERLOCK_LD_TRAY_PP_PKR3_FEEDER_LEFT_IMPACT_POS = (0.0);
double INTERLOCK_LD_TRAY_PP_PKR4_FEEDER_LEFT_IMPACT_POS = (0.0);
double INTERLOCK_LD_TRAY_PP_PKR1_FEEDER_RIGHT_IMPACT_POS = (0.0);
double INTERLOCK_LD_TRAY_PP_PKR2_FEEDER_RIGHT_IMPACT_POS = (0.0);
double INTERLOCK_LD_TRAY_PP_PKR3_FEEDER_RIGHT_IMPACT_POS = (0.0);
double INTERLOCK_LD_TRAY_PP_PKR4_FEEDER_RIGHT_IMPACT_POS = (0.0);


double INTERLOCK_UD_TRAY_PP_PKR1_FEEDER_LEFT_IMPACT_POS = (0.0);
double INTERLOCK_UD_TRAY_PP_PKR2_FEEDER_LEFT_IMPACT_POS = (0.0);
double INTERLOCK_UD_TRAY_PP_PKR3_FEEDER_LEFT_IMPACT_POS = (0.0);
double INTERLOCK_UD_TRAY_PP_PKR4_FEEDER_LEFT_IMPACT_POS = (0.0);
double INTERLOCK_UD_TRAY_PP_PKR1_FEEDER_RIGHT_IMPACT_POS = (0.0);
double INTERLOCK_UD_TRAY_PP_PKR2_FEEDER_RIGHT_IMPACT_POS = (0.0);
double INTERLOCK_UD_TRAY_PP_PKR3_FEEDER_RIGHT_IMPACT_POS = (0.0);
double INTERLOCK_UD_TRAY_PP_PKR4_FEEDER_RIGHT_IMPACT_POS = (0.0);





CWnd* GetDlgItemChain(CWnd* container, int nID)
{
	CWnd* wnd;

	if( !container ) return NULL;
	if( IsWindow( container->m_hWnd ) == FALSE ) return NULL;
	if( (wnd = container->GetDlgItem( nID )) == NULL )
	{
		CWnd* pa = container->GetParent();
		if( pa == NULL ) return NULL;
		wnd = GetDlgItemChain( pa, nID );
	}
	return wnd;
}

BOOL LessByMinX(const CPoint& lx, const CPoint& Rx)
{ 
	return (lx.x < Rx.x); 
};
BOOL LessByMinY(const CPoint& ly, const CPoint& Ry)
{ 
	return (ly.y < Ry.y); 
};


BOOL MinX(const CPoint& lx, const CPoint& Rx) { return (lx.x < Rx.x); };
BOOL MinY(const CPoint& ly, const CPoint& Ry) { return (ly.y < Ry.y); };
