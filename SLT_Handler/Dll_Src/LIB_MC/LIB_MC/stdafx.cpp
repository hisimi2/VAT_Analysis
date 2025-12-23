// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// LIB_MC.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"

int g_nDIO_Type = _DIO_TYPE_SIMUL_;

CDatabaseIF*		g_pDbIf      = NULL;
CSingleDatabase     g_SingleDB;
CSYCON_DRV          g_SyconDrv;
CDIO_SIMUL_DRV*     g_pDioSimDrv = NULL;
CAIO_SIMUL_DRV*     g_pAioSimDrv = NULL;
CDIO_Cont_Imp*	    g_pDIOContImp= NULL;
CAIO_Cont_Imp*      g_pAioContImp= NULL;
CMotion_Cont_Imp*   g_pMotion_Cont_Imp = NULL;
CIO_Cont_Imp        g_IOContImp;

//SoftServo
WMX3Api			g_Wmx3Lib;
Io*				g_pWmx3Lib_Io = nullptr;


CBbxmnpClient			g_bbxmnpClient;

Ecat*			g_pecApi;
CoreMotion*     g_pWmx3Lib_cm = nullptr;
AdvancedMotion*	g_pAdvMotion = nullptr;


