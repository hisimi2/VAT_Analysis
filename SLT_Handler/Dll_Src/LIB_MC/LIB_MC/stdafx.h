// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 클래스입니다.
#include <afxodlgs.h>       // MFC OLE 대화 상자 클래스입니다.
#include <afxdisp.h>        // MFC 자동화 클래스입니다.
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC 데이터베이스 클래스입니다.
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO 데이터베이스 클래스입니다.
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxcontrolbars.h>


#include <iostream>
#include <sstream>
#include <bitset>
#include <string>
#include "io.h"
#include "DEF_COMMON.h"
#include "ThreadBase.h"
#include "DatabaseIF.h"
#include "SYCON_DRV.h"
#include "AnalogIn.h"
#include "AnalogOut.h"
#include "AI_SYCON.h"
#include "AO_SYCON.h"
#include "AI_SIMUL.h"
#include "AO_SIMUL.h"
#include "AIO_SIMUL_DRV.h"
#include "DIO.h"
#include "DIO_SIMUL.h"
#include "DIO_Cont_Imp.h"
#include "AIO_Cont_Imp.h"
#include "SingleDatabase.h"
#include "Motion_Cont_Imp.h"
#include "IO_Cont_Imp.h"

// #include "SoftServoControl.h"
// #include "SoftServoEtherCAT/IOApi.h"
// #include "SoftServoEtherCAT/WMX3Api.h"
// 
// #include "SoftServoEtherCAT/EcApi.h"
// #include "SoftServoEtherCAT/CoreMotionApi.h"
// #include "SoftServoEtherCAT/AdvancedMotionApi.h"
// #include "SoftServoEtherCAT/PMMotionApi.h"

#include "SoftServoControl.h"
#include "SoftServoEtherCAT\EcApi.h"
#include "SoftServoEtherCAT\IOApi.h"
#include "SoftServoEtherCAT\WMX3Api.h"
#include "SoftServoEtherCAT\CoreMotionApi.h"
#include "SoftServoEtherCAT\AdvancedMotionApi.h"
#include "SoftServoEtherCAT\PMMotionApi.h"

#include "BBXMNP\nmiMNApi.h"
#include "BbxmnpWnd.h"
#include "DefineBbxmnp.h"
#include "BbxmnpClient.h"
#include "AABRuleCheck.h"
#include "DefineBbxmnp.h"

#pragma  comment(lib, "..\\..\\SLT_Handler\\LibFiles\\SoftServoEtherCAT\\IOApi.lib")
#pragma comment(lib, "..\\..\\SLT_Handler\\LibFiles\\SoftServoEtherCAT\\WMX3Api.lib")

//#pragma comment(lib, "..\\..\\SLT_Handler\\LibFiles\\SoftServoEtherCAT\\EcApi.lib")
#pragma comment(lib, "..\\..\\SLT_Handler\\LibFiles\\SoftServoEtherCAT\\CoreMotionApi.lib")
#pragma comment(lib, "..\\..\\SLT_Handler\\LibFiles\\SoftServoEtherCAT\\AdvancedMotionApi.lib")
#pragma comment(lib, "..\\..\\SLT_Handler\\LibFiles\\SoftServoEtherCAT\\PMMotionApi.lib")

#pragma comment(lib, "..\\..\\SLT_Handler\\LibFiles\\SoftServoEtherCAT\\IMDll.lib")
#pragma comment(lib, "..\\..\\SLT_Handler\\LibFiles\\BBXMNP\\nmiMNApi.lib")

using namespace wmx3Api;
using namespace ecApi;

extern WMX3Api			g_Wmx3Lib;
extern Io*				g_pWmx3Lib_Io;
extern CoreMotion*		g_pWmx3Lib_cm;
extern AdvancedMotion*	g_pAdvMotion;
extern Ecat*			g_pecApi;

extern int g_nDIO_Type;


extern CDatabaseIF*		 g_pDbIf;
extern CSingleDatabase   g_SingleDB;
extern CSYCON_DRV        g_SyconDrv;
extern CDIO_SIMUL_DRV*   g_pDioSimDrv;
extern CAIO_SIMUL_DRV*   g_pAioSimDrv;

extern CDIO_Cont_Imp*	 g_pDIOContImp;
extern CAIO_Cont_Imp*    g_pAioContImp;
extern CMotion_Cont_Imp* g_pMotion_Cont_Imp;
extern CIO_Cont_Imp      g_IOContImp;

extern CBbxmnpClient	 g_bbxmnpClient;
