
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


//========================================== Common header include
#include "resource.h"

// =======  =======  ======= Dll Lib header
// dll common header
#include "DEF_COMMON.h"
#include "DEF_MOTION.h"
#include "DEF_LIB_COMMCTRL.h"
#include "DEF_LIB_GUI.h"
#include "DEF_SLT_GUI.h"
#include "DEF_SLT_MC.h"
#include "DEF_DB_TBL.h"
#include "DEF_COK_STRUCT.h"
#include "DEF_LIB_IPC_SOCK.h"

#include "LibCommCtrl.h"
#include "Motion_Cont.h"
#include "ThreadBase.h"
#include "WinRs232c.h"
#include "CokFileManager.h"

// Socket dll
#include "IPCSock.h"
#include "MCComm.h"

// Log Macro
#include "Log.h"
#define  TWLOG CLog::getInstance()

// modify data header
#include "SharedMem.h"
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
#include "ParamList.h"
#include "MyList.h"
#include "ConfigData.h"
#include "Token.h"
#include "DeviceDataMem.h"
#include "DataMemCont.h"

// DB header
#include "DatabaseIF.h"
#include "DataMem.h"
#include "sqlite3.h"

// modify gui header
#include "DEF_LIB_GUI.h"
#include "ShadeButtonST.h"
#include "BtnST.h"
#include "EditTrans.h"
#include "CeXDib.h"
#include "StaticTrans.h"
#include "../Include/OcxFiles/spreadsheet.h"
#include "../Include/OcxFiles/font.h"

// =======  =======  ======= Project Header
#include "MainLowerDlg.h"
#include "UserOper.h"
#include "SetupDlg.h"
#include "EngrDlg.h"
#include "SupervisorDlg.h"
#include "CategorySetupBinDlg.h"
#include "CategorySetupDlg.h"
#include "DeviceSetup.h"
#include "EventLogDlg.h"
#include "TrayFileSetupDlg.h"
#include "TrayMapDlg.h"
#include "YieldSetDlg.h"
#include "SltPgmSetDlg.h"
#include "SPD_TM_TRAY_PP.h"
#include "SPD_TM_LoadTbl.h"
#include "SPD_TM_PressUnit.h"
#include "SPD_TM_TEST_PP.h"
#include "SPD_TM_Transfer.h"
#include "SPD_TM_FEEDER.h"
#include "SPD_MotorAll.h"
#include "RegisterCustomer.h"
#include "RegisterDeviceName.h"
#include "RegisterPgmName.h"
#include "RegisterTrayFile.h"
#include "SystemData.h"
// #include "TrayViewLoad.h"
// #include "TrayViewUnload.h"
// #include "TrayViewHandTestSite.h"
#include "MCComm.h"
#include "VibratorDlg.h"
#include "YieldMonitorDlg.h"
#include "CCDSetupDlg.h"
#include "SocketCleaning.h"
#include "DlgTesterPgmSet.h"
#include "AutoGrrSetDlg.h"
#include "DataAssignDlg.h"
#include "AutoGrrResultDlg.h"
#include "FilePathDlg.h"
#include "LotInfoAmkorDlg.h"
#include "LotInfoAmkorAPLDlg.h"

#include "UpdateHistory.h"
#include "RegisterCetagory.h"
#include "SiteSetDlg_128Para.h"
#include "ClearCntDlg_64Para.h"
#include "UserOffSetTestPp_MP.h"
#include "DotMatrix_Drv.h"
#include "DailyReport.h"
#include "CBSpeedListCtrl.h"
#include "AutoLoopSetDlg.h"
#include "OEE.h"
#include "BarcodeValidateSetupDlg.h"

// Common extern
extern CLibCommCtrl			g_LibCommCtrl;
extern CDataMemCont			g_DMCont;
extern CDatabaseIF			g_DB;
extern CDatabaseIF			g_DB_LOG;
extern CDatabaseIF			g_DB_Teach;
extern CDatabaseIF			g_DB_LotHistory;
extern CCokFileManager      g_COK_FileMgr;
extern CDeviceDataMem       g_DvcDataMem;
extern CMCComm				g_McComm;
extern CDotMatrix_Drv		g_DotMatrix;

// extern int 	eAXIS_TEST_PP_1_X_PITCH;
// extern int 	eAXIS_TEST_PP_1_X2_PITCH;
// extern int 	eAXIS_TEST_PP_1_Y_PITCH;
// extern int 	eAXIS_TEST_PP_1_Y2_PITCH;
// extern int 	eAXIS_TEST_PP_1_X;
// extern int 	eAXIS_TEST_PP_1_X2;
// extern int 	eAXIS_TEST_PP_1_Y;
// 
// extern int 	eAXIS_PRESS_UNIT_DN1;
// extern int 	eAXIS_PRESS_UNIT_DN2;
// extern int 	eAXIS_PRESS_UNIT_DN3;
// extern int 	eAXIS_PRESS_UNIT_DN4;
// 
// extern int 	eAXIS_TRAY_PP_1_X;
// extern int 	eAXIS_TRAY_PP_1_X_PITCH;
// extern int 	eAXIS_TRAY_PP_1_Y_PITCH;
// 
// extern int 	eAXIS_TRAY_PP_2_X;
// extern int 	eAXIS_TRAY_PP_2_Y;
// extern int 	eAXIS_TRAY_PP_2_X_PITCH;
// extern int 	eAXIS_TRAY_PP_2_Y_PITCH;
// 
// extern int 	eAXIS_TRANSFER_X;
// 
// extern int 	eAXIS_TRAY_FEEDER_Y;
// extern int 	eAXIS_TRAY_FEEDER_END_STOPPER;
// 
// extern int 	eAXIS_LOAD_TABLE_1_ROTATE;
// extern int 	eAXIS_LOAD_TABLE_2_ROTATE;
// extern int 	eAXIS_LOAD_TABLE_3_ROTATE;
// extern int 	eAXIS_LOAD_TABLE_4_ROTATE;
// 
// extern int 	eAXIS_LOAD_TABLE_1_Y;
// extern int 	eAXIS_LOAD_TABLE_2_Y;
// extern int 	eAXIS_LOAD_TABLE_3_Y;
// extern int 	eAXIS_LOAD_TABLE_4_Y;
// 
// extern int 	eAXIS_TEST_PP_2_Y;
// extern int 	eAXIS_TEST_PP_2_X;
// extern int 	eAXIS_TEST_PP_2_X2;
// extern int 	eAXIS_TEST_PP_2_X_PITCH;
// extern int 	eAXIS_TEST_PP_2_X2_PITCH;
// extern int 	eAXIS_TEST_PP_2_Y_PITCH;
// extern int 	eAXIS_TEST_PP_2_Y2_PITCH;
// 
// extern int 	eAXIS_PRESS_UNIT_DN5;
// extern int 	eAXIS_PRESS_UNIT_DN6;
// extern int 	eAXIS_PRESS_UNIT_DN7;
// extern int 	eAXIS_PRESS_UNIT_DN8;
// 
// extern int 	eAXIS_TRAY_PP_1_Z;
// extern int 	eAXIS_TEST_PP_1_Z;
// extern int 	eAXIS_TEST_PP_1_Z2;
// extern int 	eAXIS_TRAY_PP_2_Z;
// extern int 	eAXIS_TEST_PP_2_Z;
// extern int 	eAXIS_TEST_PP_2_Z2;
// 
// extern int 	eMAX_AXIS_COUNT;



void SetDotMatrixUpdate();

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


