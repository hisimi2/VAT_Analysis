
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

//#include <vld.h> 
#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.


#include <afxdisp.h>        // MFC 자동화 클래스입니다.

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원





#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

//=============================================================================================
// INCLUDE
//=============================================================================================
// Libs

// Log Macro
#include "Log.h"
#define  TWLOG CLog::getInstance()
#include <string>
#include <functional>
#include <memory>
#if _MSC_VER < 1900
#define __func__ std::string(__FUNCTION__).substr( ((int)std::string(__FUNCTION__).find("::")) > -1 ? ((int)std::string(__FUNCTION__).find("::"))+2 : 0 ).c_str()
#endif

#include <afx.h>
#include <strsafe.h>
#include <set>
#include <map>
#include "afxmt.h"
#include "../Include/OcxFiles/spreadsheet.h"

#include "DEF_DB_TBL.h"
#include "DEF_DM_0.h"
#include "DEF_DM_1.h"
#include "DEF_DM_2.h"
#include "DEF_DM_3.h"
#include "DEF_DM_4.h"
#include "DEF_DM_5.h"
#include "DEF_DM_6.h"
#include "DEF_DM_7.h"
#include "DEF_DM_8.h"
#include "DEF_DM_9.h"
#include "DEF_DM_10.h"
#include "DEF_DM_11.h"
#include "DEF_DM_12.h"
#include "DEF_DM_13.h"
#include "DEF_COMMON.h"
#include "DEF_ERROR_CODE.h"
#include "DEF_EVENT_CODE.h"
#include "DEF_SLT_MC.h"
#include "DEF_LIB_GUI.h"
#include "DEF_IO.h"
#include "DEF_EVENT_CODE.h"
#include "DEF_ERROR_CODE.h"
#include "DEF_DEVICE_DATA.h"

#include "DataMem.h"
#include "DeviceDataMem.h"
#include "TimeCheck.h"
#include "TimerTickCheck.h"
#include "MyList.h"
#include "ParamList.h"
#include "ThreadBase.h"
#include "DIO_Cont.h"
#include "AIO_Cont.h"
#include "Motion_Cont.h"
#include "DatabaseIF.h"
#include "LibCommCtrl.h"
#include "TeachingFile.h"
#include "CokFileManager.h"
#include "GUIComm.h"
#include "OPSwitch/OPSwitchAct.h"
#include "OPSwitch/OPSwitchAct_Cover.h"
#include "OPSwitch/OPSwitchAct_START.h"
#include "OPSwitch/OPSwitchAct_STOP.h"

#include "OPSwitch/OPSwitchAct_Lamp.h"
#include "OPSwitch/OPSwitchAct_StepTrayPP.h"
#include "OPSwitch/OPSwitchAct_StepTestPP.h"
#include "OPSwitch/OPSwitchAct_Feeder.h"

#include "RegulatorIF.h"
#include "AABRuleCheck.h"
#include "OEEFile.h"
#include "FtpManager.h"

// Data Object
#include "RamTeaching.h"
#include "RamMotorParam.h"
#include "DataMemcont.h"

#include "VisionIF/VisionComm_Barcode.h"
#include "VisionIF/VisionComm_TestHand_1.h"
#include "VisionIF/VisionComm_TestHand_2.h"
#include "VisionIF/Vision_2dBarcode.h"

// #include "VisionIF/VisionComm_VAT_PC1.h"
// #include "VisionIF/VisionComm_VAT_PC2.h"
#include "VisionIF/TriggerGenerator.h"

#include "AllAutoTestSite.h"
#include "TestHandWorkArea.h"

// Task Base Class
#include "BaseSeqCmd.h"
#include "BaseTaskManager.h"
#include "BaseAutoSeqManager.h"

// Component Task
#include "TaskTowerLight.h"
#include "TaskVibrator.h"
#include "TaskRegulator.h"

// Sequence & Task Object
#include "TaskStacker.h"
#include "AllAutoStacker.h"
#include "StackerCmd_Initial.h"
#include "StackerCmd_Load.h"
#include "StackerCmd_TrayDn.h"
#include "StackerCmd_Unload.h"
#include "StackerCmd_TrayUp.h"
#include "StackerCmd_Vibration.h"
#include "StackerCmd_TrayOut.h"
#include "StackerCmd_FullTray.h"

// Loading Table
#include "TaskLoadTable.h"
#include "LoadTblCmd_Initial.h"
#include "LoadTblCmd_MovePos.h"


// Press Unit
#include "TaskPressUnit.h"
#include "PressUnitCmd_Initial.h"
#include "PressUnitCmd_MovePos.h"
#include "PressUnitCmd_Contact.h"
#include "PressUnitCmd_AutoHeightCal.h"
#include "PressUnitCmd_OffLineTest.h"
#include "PressUnitCmd_OnLineTestApple.h"
#include "PressUnitCmd_QAEmptyTest.h"

#include "PressUnitCmd_SocketCleanContact.h"
#include "PressUnitCmd_OnLineLoopTest.h"
// Transfer
#include "TaskTransfer.h"
#include "TransferCmd_Initial.h"
#include "TransferCmd_MovePos.h"
#include "TransferCmd_MovePickPlace.h"

// Test PP
#include "TaskTestPP.h"
#include "TestPPCmd_Initial.h"
#include "TestPPCmd_MovePickPlace.h"
#include "TestPPCmd_MoveAlreadyPos.h"


// TrayBasePP
#include "TaskTrayBasePP.h"
#include "TaskAutoTrayBasePP.h"

// Unload PP
#include "TaskTrayUnloadPP.h"
#include "TrayUnloadPPCmd_Initial.h"
#include "TrayUnloadPPCmd_MovePickPlace.h"
#include "TrayUnloadPPCmd_MovePos.h"


// Load PP
#include "TaskTrayLoadPP.h"
#include "TrayLoadPPCmd_Initial.h"
#include "TrayLoadPPCmd_MovePickPlace.h"
#include "TrayLoadPPCmd_MovePos.h"



// Tray Feeder
#include "TaskTrayFeeder.h"
#include "TrayFeederCmd_Initial.h"
#include "TrayFeederCmd_MovePickPlace.h"
#include "TrayFeederCmd_MovePos.h"


#include "TaskMultiOrg.h"
#include "MultiOrgCmd_Axis.h"

// auto
#include "TaskAutoSystem.h"
#include "TaskSystemCtrl.h"
#include "TaskAutoPress.h"
#include "TaskAutoTestPp.h"
#include "TaskAutoStacker.h"
#include "TaskAutoLoadTable.h"
#include "TaskAutoTransfer.h"
#include "TaskAutoTrayFeeder.h"
#include "TaskAutoTrayLoadPP.h"
#include "TaskAutoTrayUnloadPP.h"



#include "DlgSimulDioSLT.h"

#include "SystemCmd_AxisJogMove.h"
#include "SystemCmd_Axis_Initial.h"

#include "TesterIF/TesterIF.h"

#include "Error.h"

// product
#include "ProcJamRate.h"

// mac network driver file copy
#include "NetDriver.h"
// ftp
#include "MmwinFtp.h"


#include "DlgDebugAutoStacker.h"

#include "DlgHomeCheckStatus.h"
#include "DlgMon.h"
#include "DlgContactAndSafety.h"

////////////////////////////////////////////////////vat
#include "DEF_DM_14.h"
#include "DEF_VAT_COMMON.h"
#include "../VisionAutoTeaching/VAT_FILE_SECTION.h"
#include "../VisionAutoTeaching/VAT_CommonData.h"
#include "../VisionAutoTeaching/VAT_CommonStaticData.h"

#include "../VisionAutoTeaching//VAT_DataCopy.h"

#include "../VisionAutoTeaching/VAT_MeasureStatus.h"

#include "../VisionAutoTeaching/Simul/VATSimul.h"

//vision
#include "../VisionAutoTeaching/VisionComm/VAT_VisionComm.h"
#include "../VisionAutoTeaching/VisionComm/VAT_VisionMgr.h"

//Dialogue
// #include "../VisionAutoTeaching/Main Dlg/VATMain_ZAutoFocus.h"
// #include "../VisionAutoTeaching/Main Dlg/VATMain_CamCenterCalibration.h"
// #include "../VisionAutoTeaching/Main Dlg/VATMain_PickerAlign.h"
// #include "../VisionAutoTeaching/Main Dlg/VATMain_ZAutoTeaching.h"
// #include "../VisionAutoTeaching/Main Dlg/VATMain_VisionAutoTeaching.h"


//Main dlg
#include "../VisionAutoTeaching/MainDlg/VATMain_Common.h"

//System
#include "../VisionAutoTeaching/System/VatBaseTask.h"
#include "../VisionAutoTeaching/System/VAT_TaskAutoSystem.h"
#include "../VisionAutoTeaching/System/VAT_TaskSystemCtrl.h"


//Auto
#include "../VisionAutoTeaching/Auto/VAT_TaskAutoTrayLoadPP.h"
#include "../VisionAutoTeaching/Auto/VAT_TaskAutoTrayUnloadPP.h"
#include "../VisionAutoTeaching/Auto/VAT_TaskAutoTestPP.h"


//Cmd
//Common
#include "../VisionAutoTeaching/Cmd/COMMON/VatTrayLoadPPCmd_MovePos.h"
#include "../VisionAutoTeaching/Cmd/COMMON/VatTrayUnloadPPCmd_MovePos.h"
#include "../VisionAutoTeaching/Cmd/COMMON/VatTestPPCmd_MovePos.h"
#include "../VisionAutoTeaching/Cmd/COMMON/VatLoadTblCmd_MovePos.h"
#include "../VisionAutoTeaching/Cmd/COMMON/VatTrayFeederCmd_MovePos.h"

#include "../VisionAutoTeaching/Cmd/VatTrayLoadPPFeederCmd_MovePos.h"
#include "../VisionAutoTeaching/Cmd/VatTrayLoadPPTableCmd_MovePos.h"

#include "../VisionAutoTeaching/Cmd/VatVisionCmd_CamMeasure.h"



//Z Auto Teaching
#include "../VisionAutoTeaching/Cmd/VatTrayLoadPPCmd_PickerHeightCal.h"
#include "../VisionAutoTeaching/Cmd/VatTrayUnloadPPCmd_PickerHeightCal.h"
#include "../VisionAutoTeaching/Cmd/VatTestPPCmd_PickerHeightCal.h"
//////////////////////////////////////////////////



//Pcd&Backlash
#include "../Pcd_Backlash/VatHwDesignDim.h"
#include "../Pcd_Backlash/TrayLoadPPCmd_VatPcd.h"
#include "../Pcd_Backlash/TrayUnloadPPCmd_VatPcd.h"

#include "../Pcd_Backlash/TrayLoadPPCmd_VatBacklash.h"
#include "../Pcd_Backlash/TrayUnloadPPCmd_VatBacklash.h"

#include "../Pcd_Backlash/TestPPCmd_VatPcd.h"
#include "../Pcd_Backlash/TestPPCmd_VatBacklash.h"

#include "MasterMac.h"
#include "DlgManuMotorTeaching.h"
#include "EditTrans.h"
#include "BarcodeList.h"


#include "LogZip.h"

#include "CylinderTimeCheck.h"
#include "../Dll_Src/LIB_MC/LIB_MC/SoftServoControl.h"

#include "../Dll_Src/LIB_MC/LIB_MC/BbxmnpClient.h"


//token
#include "Token.h"

CWnd* GetDlgItemChain(CWnd* container, int nID);

BOOL LessByMinX(const CPoint& lx, const CPoint& Rx);
BOOL LessByMinY(const CPoint& ly, const CPoint& Ry);

//=============================================================================================
// 
//=============================================================================================
//extern CmmwinFtp*                g_pFtpObj;

extern CDlgHomeCheckStatus*      g_pDlgHomeCheckStatus;
//extern CDlgManuMotorTeaching* g_pDlgManuMotorTeaching;

extern CLibCommCtrl			     g_LibCommCtrl;
extern CDataMemCont			     g_DMCont;
extern CDatabaseIF               g_DB;
extern CDatabaseIF               g_DBErr;
extern CDatabaseIF               g_DBTeach;
extern CDatabaseIF               g_DBLotHistory;
extern CDatabaseIF               g_DBVAT;
extern CMotion_Cont			     g_Motor;
extern CDIO_Cont			     g_IO;
extern CAIO_Cont			     g_AIO;
extern CCokFileManager		     g_CokMgr;
extern CVisionComm_Barcode*      g_pBarcode;

extern CVisionComm_TestHand_1* 	  g_pVisionTestHand_1;
extern CVisionComm_TestHand_2* 	  g_pVisionTestHand_2;
//extern CVisionComm_TestSiteUp*   g_pTestSiteInspUp;
//extern CVisionComm_TestSiteDown* g_pTestSiteInspDn;
//extern CVisionComm_VAT*          g_pVAT;
// extern CVisionComm_VAT_PC1*          g_pVAT_PC1;
// extern CVisionComm_VAT_PC2*          g_pVAT_PC2;


extern CGUIComm                  g_GuiComm;
extern CTesterIF		         g_AppleTestIF;

extern SoftServoControl			g_SoftServoCtrl;
							     
extern COPSwitchAct_Cover        g_COVER_SW;
extern COPSwitchAct_START        g_START_SW; // 여러개의 스위치 입력, 여러개의 LED출력, Stop switch와 그룹
extern COPSwitchAct_STOP         g_STOP_SW; // 여러개의 스위치 입력, 여러개의 LED출력, Start switch와 그룹
extern COPSwitchAct_Lamp         g_LAMP_SW;
extern COPSwitchAct              g_SOUND_SW;
extern COPSwitchAct              g_TowerR;
extern COPSwitchAct              g_TowerY;
extern COPSwitchAct              g_TowerG;
extern COPSwitchAct              g_TowerBuz;

extern COPSwitchAct_StepTrayPP   g_StepTrayPP_SW[eMaxTrayPPCount];
extern COPSwitchAct_StepTestPP   g_StepTestPP_SW[eMaxTestPPCount];

extern COPSwitchAct_Feeder		 g_Feeder_SW;

extern CError                    g_Error;
extern CTestHandWorkArea         g_testpp_work_area[eMaxTestPPCount];

//extern CTaskVibrator             g_TaskVibrator;
//extern CTaskUnderStacker	     g_TaskUnderStackerUD;
//extern CTaskSetplate		     g_TaskSetplate[eMaxSetplateCount];
//extern CTaskAutoTestSite         g_TaskAutoTestSite[eMaxTestPPCount];
extern CTaskSystemCtrl           g_TaskSystemCtrl;
extern CRegulatorIF				 g_RegulatorIF[eMaxPressRegul];

// Task
extern CTaskTowerLight           g_TaskTowerLight;
extern CTaskRegulator            g_TaskRegulator[eMaxPressUnitCount];
extern CTaskStacker				 g_TaskStacker[ eMaxStackerCount ];
extern CTaskPressUnit            g_TaskPressUnit[eMaxPressUnitCount];
extern CTaskTransfer             g_TaskTransfer;
extern CTaskTestPP               g_TaskTestPP[eMaxTestPPCount];
extern CTaskTrayLoadPP			 g_TaskTrayLoadPP;
extern CTaskTrayUnloadPP		 g_TaskTrayUnloadPP;
extern CTaskLoadTable            g_TaskLoadTbl[eMaxLoadTblCount];
extern CTaskTrayFeeder			 g_TaskTrayFeeder;


// Auto Task
extern CTaskAutoSystem			 g_TaskAutoSystem;
extern CTaskAutoTrayLoadPP		 g_TaskAutoTrayLoadPP;
extern CTaskAutoTrayUnloadPP     g_TaskAutoTrayUnloadPP;
extern CTaskAutoStacker          g_TaskAutoStacker[ eMaxStackerCount ];
extern CAllAutoStacker			 g_TaskAllAutoStacker;
extern CTaskAutoPress            g_TaskAutoPress[ eMaxPressUnitCount ];
extern CTaskAutoTestPp           g_TaskAutoTestPp[ eMaxTestPPCount];
extern CAllAutoTestSite          g_TaskAllTestSite;
extern CTaskAutoLoadTable        g_TaskAutoLdTable[ eMaxLoadTblCount];
extern CTaskAutoTransfer         g_TaskAutoTransfer;
extern CTaskAutoTrayFeeder		 g_TaskAutoTrayFeeder;



extern CDlgSimulDioSLT*          g_pDlgSimulDioSLT;
extern CDlgDebugAutoStacker*     g_pDlgDebugView;
//extern CDlgMon*                  g_pDlgMotorMonitor;


//VAT
extern BOOL						g_bVATStatusExecute;

//extern CVatBaseTask				 g_VatBaseTask;
extern CVAT_CommonData			 g_VATCommonData;
extern CVAT_CommonStaticData	 g_VATCommonStaticData;

extern CVAT_DataCopy			 g_VATDataCopy;

extern CVAT_TaskSystemCtrl		 g_VatTaskSystemCtrl;
extern CVAT_TaskAutoSystem		 g_VatTaskAutoSystem;

extern CVAT_TaskAutoTrayLoadPP   g_VatTaskAutoTrayLoadPP;
extern CVAT_TaskAutoTrayUnloadPP g_VatTaskAutoTrayUnloadPP;
extern CVAT_TaskAutoTestPP		  g_VatTaskAutoLoadTestPP1;
extern CVAT_TaskAutoTestPP		  g_VatTaskAutoUnloadTestPP1;
extern CVAT_TaskAutoTestPP		  g_VatTaskAutoLoadTestPP2;
extern CVAT_TaskAutoTestPP		  g_VatTaskAutoUnloadTestPP2;

extern CVAT_VisionMgr			 g_VatVisionMgr[VAT_CAM_MAX];
//////////////////////////////////////////////


extern CProcJamRate              g_JamRate;
extern ST_CATE_INFO              g_BinCategory; //bin category list

extern CNetDriver                g_NetDriver;
extern CVision_2dBarcode         g_2DBarcodeChk;
extern CTriggerGenerator		 g_trigger_generator;

//extern AABRuleCheck              g_ART_Rule;

//extern ST_VAT_3POINT             g_St3PointTeach;
extern CTaskMultiOrg	         g_MulitOrg;

extern CMasterMac				 g_MasterMac;

extern CBarcodeList				 g_BarcodeList;

extern CLogZip					 g_LogZip;

extern CCylinderTimeCheck		 g_CylinderTime;
//extern COEEFile					 g_OEEFile;
//extern CFtpManager g_Ftp;

extern ST_Set_DefaultBin         g_DefaultBinCategory;

extern ST_CONVEYOR_WORK_AREA	g_conveyor_work_area;

extern ST_TRANSFER_FEEDER_WORK_AREA	g_Transfer_Feeder_work_area;

extern ST_TESTPP_TABLE_WORK_AREA	g_TestPP_Table_work_area[eMaxTestPPCount];

extern std::vector< CTaskAutoStacker* > g_load_stackers;
extern std::vector< CTaskAutoStacker* > g_unload_stackers;
extern std::vector< CTaskAutoStacker* > g_empty_unload_stackers;
extern std::vector< CTaskAutoStacker* > g_empty_load_stackers;


extern CBbxmnpClient*			g_pBbxmnpClient;

//extern CConectSocket*			g_pConectSocket;

extern SOCKET			g_UDPSockBinSolution;
extern SOCKADDR_IN		g_udpserverAddr;
extern SYSTEMTIME g_SOT[eMaxPressUnitCount];
extern SYSTEMTIME g_EOT[eMaxPressUnitCount];


//InterLock
extern double  MIN_TRAY_PP_WORK_ENABLE_POS_um;
//Tray Load Hand
extern double INTERLOCK_LD_TRAYPP_X_WORK_ENABLE_FEEDER_START_POS;
extern double INTERLOCK_LD_TRAYPP_X_WORK_ENABLE_FEEDER_END_POS;
extern double INTERLOCK_LD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_LEFT;
extern double INTERLOCK_LD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_RIGHT;
 //Tray Unload Hand
extern	double INTERLOCK_UD_TRAYPP_Y_WORK_ENABLE_POS_TRANSFER;
extern	double INTERLOCK_UD_TRAYPP_X_WORK_ENABLE_FEEDER_START_POS;
extern	double INTERLOCK_UD_TRAYPP_X_WORK_ENABLE_FEEDER_END_POS;
extern	double INTERLOCK_UD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_LEFT;
extern	double INTERLOCK_UD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_RIGHT;

extern double INTERLOCK_BASE_TRAYPP_Y_POS_WITH_TRAYPP_Z_AND_TBL_POSSIBLE;

//Test Hand
extern	double INTERLOCK_TESTPP_1_X_STATION_START_POS;
extern	double INTERLOCK_TESTPP_2_X_STATION_START_POS;
extern	double INTERLOCK_TESTPP_X_POS_WITH_TESTPP_Z_AND_TBL_FRONT;
extern	double INTERLOCK_TESTPP_X_POS_WITH_TESTPP_Z_AND_TBL_REAR;
extern	double INTERLOCK_TESTPP_X_SAFETY_POS;

extern	double INTERLOCK_HAND_LENGTH;
extern	double INTERLOCK_STATION_GAP;
extern	double INTERLOCK_TESTPP_Y_STATION_1_START_POS_LOAD;
extern	double INTERLOCK_TESTPP_Y_STATION_1_END_POS_LOAD;
extern	double INTERLOCK_TESTPP_Y_STATION_2_START_POS_LOAD;
extern	double INTERLOCK_TESTPP_Y_STATION_2_END_POS_LOAD;
extern	double INTERLOCK_TESTPP_Y_STATION_3_START_POS_LOAD;
extern	double INTERLOCK_TESTPP_Y_STATION_3_END_POS_LOAD;
extern	double INTERLOCK_TESTPP_Y_STATION_4_START_POS_LOAD;
extern	double INTERLOCK_TESTPP_Y_STATION_4_END_POS_LOAD;

extern	double INTERLOCK_TESTPP_Y_STATION_1_START_POS_UNLOAD;
extern	double INTERLOCK_TESTPP_Y_STATION_1_END_POS_UNLOAD;
extern	double INTERLOCK_TESTPP_Y_STATION_2_START_POS_UNLOAD;
extern	double INTERLOCK_TESTPP_Y_STATION_2_END_POS_UNLOAD;
extern	double INTERLOCK_TESTPP_Y_STATION_3_START_POS_UNLOAD;
extern	double INTERLOCK_TESTPP_Y_STATION_3_END_POS_UNLOAD;
extern	double INTERLOCK_TESTPP_Y_STATION_4_START_POS_UNLOAD;
extern	double INTERLOCK_TESTPP_Y_STATION_4_END_POS_UNLOAD;

extern	double INTERLOCK_TESTPP1_X_CLEAN_TABLE_LIMIT;
extern	double INTERLOCK_TESTPP2_X_CLEAN_TABLE_LIMIT;

extern  double INTERLOCK_TESTPP_Y_CLEAN_TABLE_LIMIT;

	//Loading Table
extern	double INTERLOCK_LOADING_TBL_POS_BOUNDARY;
	//Press
extern	double INTERLOCK_PRESS_Z_SAFETY_POS;	//Press Z Safety Position

												//feeder
extern	double INTERLOCK_FEEDER_SAFETY_POS_ENC; //Tray Feeder 안전 위치 확인(Safety Sensor 감지 되는 위치).
extern	double INTERLOCK_FEEDER_POS_BOUNDARY; //Tray Feeder Unload Y축 간섭 시작 위치.

extern double INTERLOCK_LD_TRAY_PP_PKR1_FEEDER_LEFT_IMPACT_POS;
extern double INTERLOCK_LD_TRAY_PP_PKR2_FEEDER_LEFT_IMPACT_POS;
extern double INTERLOCK_LD_TRAY_PP_PKR3_FEEDER_LEFT_IMPACT_POS;
extern double INTERLOCK_LD_TRAY_PP_PKR4_FEEDER_LEFT_IMPACT_POS;
extern double INTERLOCK_LD_TRAY_PP_PKR1_FEEDER_RIGHT_IMPACT_POS;
extern double INTERLOCK_LD_TRAY_PP_PKR2_FEEDER_RIGHT_IMPACT_POS;
extern double INTERLOCK_LD_TRAY_PP_PKR3_FEEDER_RIGHT_IMPACT_POS;
extern double INTERLOCK_LD_TRAY_PP_PKR4_FEEDER_RIGHT_IMPACT_POS;

extern double INTERLOCK_UD_TRAY_PP_PKR1_FEEDER_LEFT_IMPACT_POS;
extern double INTERLOCK_UD_TRAY_PP_PKR2_FEEDER_LEFT_IMPACT_POS;
extern double INTERLOCK_UD_TRAY_PP_PKR3_FEEDER_LEFT_IMPACT_POS;
extern double INTERLOCK_UD_TRAY_PP_PKR4_FEEDER_LEFT_IMPACT_POS;
extern double INTERLOCK_UD_TRAY_PP_PKR1_FEEDER_RIGHT_IMPACT_POS;
extern double INTERLOCK_UD_TRAY_PP_PKR2_FEEDER_RIGHT_IMPACT_POS;
extern double INTERLOCK_UD_TRAY_PP_PKR3_FEEDER_RIGHT_IMPACT_POS;
extern double INTERLOCK_UD_TRAY_PP_PKR4_FEEDER_RIGHT_IMPACT_POS;


BOOL MinX(const CPoint& lx, const CPoint& Rx);
BOOL MinY(const CPoint& ly, const CPoint& Ry);
