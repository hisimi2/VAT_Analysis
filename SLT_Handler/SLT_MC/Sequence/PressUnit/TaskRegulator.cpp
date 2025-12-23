#include "StdAfx.h"
#include "TaskRegulator.h"
#include "PressUnitConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static const double PI = 3.141592;

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CTaskRegulator::CTaskRegulator(void)
{
	m_dForce_TrimValue_gf = 0.0;
	m_dSetPressure = 0.0;
	m_nRegulatorIdx = 0;

	m_mapDO.SetAt(yREGUL_VV_ON_OFF, "REGUL_VV_ON_OFF");
	//m_mapDO.SetAt(yREGUL_VV_OFF, "REGUL_VV_OFF");

	m_mapAI.SetAt(aiREGUL_IN, "REGUL_IN");

	m_mapAO.SetAt(aoREGUL_OUT, "REGUL_OUT");

	m_dCylinderRadius_mm = DEF_PRESS_UNIT_CYL_DIAMETER_DEFAULT / 2.0;
	m_dCylinderRadius_cm = m_dCylinderRadius_mm/10.0;

	m_nCylinder_Size = eContactCylinderSize_40mm;
	m_dForce_Offset_gf[eContactCylinderSize_32mm] = 0.0;
	m_dForce_Offset_gf[eContactCylinderSize_40mm] = 0.0;
	m_dForce_Offset_gf[eContactCylinderSize_55mm] = 0.0;
	m_dContactForceSlop_gf[eContactCylinderSize_32mm] = 1.0;
	m_dContactForceSlop_gf[eContactCylinderSize_40mm] = 0.9437;
	m_dContactForceSlop_gf[eContactCylinderSize_55mm] = 1.0;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CTaskRegulator::~CTaskRegulator(void)
{
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskRegulator::Initialize(CWnd* cWnd, int nIdx, char* szDriverFileName)
{
	m_ConfigData.SetFileName(szDriverFileName);
	char szSection[128] = {0,};

	//m_nRegulatorIdx = nIdx;

	int nIdxNo = -1;

	switch (nIdx)
	{
	case ePressUnit_Down_1 :
	case ePressUnit_Down_2 :
	case ePressUnit_Down_3 :
	case ePressUnit_Down_4 :	
// 	case ePressUnit_Up_1   :
// 	case ePressUnit_Up_2   :
// 	case ePressUnit_Up_3   :
// 	case ePressUnit_Up_4   :
		{
			m_nRegulatorIdx = 0;
			
		}break;
// 	case ePressUnit_Up_5   :
// 	case ePressUnit_Up_6   :
// 	case ePressUnit_Up_7   :
// 	case ePressUnit_Up_8   :
	case ePressUnit_Down_5:
	case ePressUnit_Down_6:
	case ePressUnit_Down_7:
	case ePressUnit_Down_8:
		{
			m_nRegulatorIdx = 1;
		}break;
	}

	switch (nIdx)
	{
	case ePressUnit_Down_1: case ePressUnit_Down_5: { nIdxNo = 1; }break;
	case ePressUnit_Down_2: case ePressUnit_Down_6: { nIdxNo = 2; }break;
	case ePressUnit_Down_3: case ePressUnit_Down_7: { nIdxNo = 3; }break;
	case ePressUnit_Down_4: case ePressUnit_Down_8: { nIdxNo = 4; }break;
// 	case ePressUnit_Up_1:   case ePressUnit_Up_5:   { nIdxNo = 5; }break;
// 	case ePressUnit_Up_2:  	case ePressUnit_Up_6:   { nIdxNo = 6; }break;
// 	case ePressUnit_Up_3:	case ePressUnit_Up_7:   { nIdxNo = 7; }break;
// 	case ePressUnit_Up_4:	case ePressUnit_Up_8:   { nIdxNo = 8; }break;
	}

	CString strUpDown;
	if(m_nRegulatorIdx == 0){
		strUpDown = _T("LEFT");
	}else{
		strUpDown = _T("RIGHT");
	}

	// Digital Output
	memset(szSection, 0x00, sizeof(szSection));
	sprintf_s(szSection, sizeof(szSection), "TASK REGULATOR %s %d CONFIG - DO", strUpDown.GetString(), nIdxNo);
	int nDoCount = m_mapDO.GetCount();
	for(int i=0; i<nDoCount; i++){
		char szKey[128] = {0,};
		CString strName;
		m_mapDO.Lookup(i, strName);
		sprintf_s(szKey, sizeof(szKey), "%s", strName.GetString() );
		int nIdx = m_ConfigData.GetInt(szSection, szKey, "0");
		m_vY.push_back(nIdx);
	}

	m_vAI.push_back(nIdx);
	//// Analog Input
	//memset(szSection, 0x00, sizeof(szSection));
	//sprintf_s(szSection, sizeof(szSection), "TASK REGULATOR %s %d CONFIG - AI", strUpDown.GetString(), nIdxNo +1);
	//int nAiCount = m_mapAI.GetCount();
	//for(int i=0; i<nAiCount; i++){
	//	char szKey[128] = {0,};
	//	CString strName;
	//	m_mapDO.Lookup(i, strName);
	//	sprintf_s(szKey, sizeof(szKey), "%s", strName.GetString() );
	//	int nIdx = m_ConfigData.GetInt(szSection, szKey, "0");
	//	m_vAI.push_back(nIdx);
	//}
	m_vAO.push_back(nIdx);
	//// Analog Output
	//memset(szSection, 0x00, sizeof(szSection));
	//sprintf_s(szSection, sizeof(szSection), "TASK REGULATOR %s %d CONFIG - AO", strUpDown.GetString(), nIdxNo +1);
	//int nAoCount = m_mapAO.GetCount();
	//for(int i=0; i<nAoCount; i++){
	//	char szKey[128] = {0,};
	//	CString strName;
	//	m_mapDO.Lookup(i, strName);
	//	sprintf_s(szKey, sizeof(szKey), "%s", strName.GetString() );
	//	int nIdx = m_ConfigData.GetInt(szSection, szKey, "0");
	//	m_vAO.push_back(nIdx);
	//}

// 	// Regulator
// 	memset(szSection, 0x00, sizeof(szSection));
// 	sprintf_s(szSection, sizeof(szSection), "TASK REGULATOR %s %d CONFIG", strUpDown, (m_nRegulatorIdx%8)+1);
// 
// 	m_stRegulData.btParity = (BYTE)m_ConfigData.GetInt(szSection, "Regulator Parity", "0");
// 	m_stRegulData.btSize   = (BYTE)m_ConfigData.GetInt(szSection, "Regulator Size", "0");
// 	m_stRegulData.btStop   = (BYTE)m_ConfigData.GetInt(szSection, "Regulator Stop", "0");
// 	m_stRegulData.dwRate   = (DWORD)m_ConfigData.GetInt(szSection, "Regulator Rate", "0");
// 	m_stRegulData.nPort    = m_ConfigData.GetInt(szSection, "Regulator Port", "0");
// 
// 	m_RegulatorIF.Initialize(cWnd, m_stRegulData, "D:\techwing\regulator" );

	Read_Cylinder_Config();

	return CBaseTaskManager::Initialize();
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CTaskRegulator::Finalize()
{
	//do_SetRegulator(0.0);
	//Sleep(500);
	//m_RegulatorIF.Finalize();
	CBaseTaskManager::Finalize();
}

void CTaskRegulator::Read_Cylinder_Config()
{
	char szSection[128] = {0,};
	CString strUpDown;
	if( m_nRegulatorIdx == 0){
		strUpDown = _T("LEFT");
	}else{
		strUpDown = _T("RIGHT");
	}

	memset(szSection, 0x00, sizeof(szSection));
	sprintf_s(szSection, sizeof(szSection), "TASK REGULATOR %s CONFIG", strUpDown.GetString() );
	int nCylinder_Size = m_ConfigData.GetInt(szSection, "CYLINDER SIZE (1:32mm,2:40mm,3:55mm)", "2") - 1;
	SetCylinderRadious(nCylinder_Size);
	m_dContactForceSlop_gf[eContactCylinderSize_32mm] = m_ConfigData.GetDouble(szSection, "Contact Force Slop (32mm)", "0");
	m_dContactForceSlop_gf[eContactCylinderSize_40mm] = m_ConfigData.GetDouble(szSection, "Contact Force Slop (40mm)", "0");
	m_dContactForceSlop_gf[eContactCylinderSize_55mm] = m_ConfigData.GetDouble(szSection, "Contact Force Slop (55mm)", "0");
	m_dForce_Offset_gf[eContactCylinderSize_32mm] = m_ConfigData.GetDouble(szSection, "Contact Force Offset gf (32mm)", "0");
	m_dForce_Offset_gf[eContactCylinderSize_40mm] = m_ConfigData.GetDouble(szSection, "Contact Force Offset gf (40mm)", "0");
	m_dForce_Offset_gf[eContactCylinderSize_55mm] = m_ConfigData.GetDouble(szSection, "Contact Force Offset gf (55mm)", "0");
}


void CTaskRegulator::OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{

}


double CTaskRegulator::GetCylinderRadious()
{
	return m_dCylinderRadius_mm;
}

void CTaskRegulator::SetCylinderRadious(int nCylinderRadious)
{
	double dRadious = 40.0;
	if( nCylinderRadious == eContactCylinderSize_32mm ){
		dRadious = 32.0;
	}else if( nCylinderRadious == eContactCylinderSize_55mm ){
		dRadious = 55.0;
	}else{
		dRadious = 40.0;
	}
	m_nCylinder_Size = nCylinderRadious;
	m_dCylinderRadius_mm = dRadious / 2.0;
	m_dCylinderRadius_cm = m_dCylinderRadius_mm / 10.0;
}


//==============================================================================
// 도착한 명령을 큐에서 꺼내어 실행 시킨다.
//==============================================================================
void CTaskRegulator::ThreadProc1()
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
  AUTHOR   : 나명진  [2015-09-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskRegulator::OnCommand(CBaseSeqCmd* pSeqCmd, int nEqpCtrlCmdType/*=eEqpCtrlMode_Auto*/)
{
	int nChkCmd = 0;
	if(pSeqCmd == NULL)
	{
		return eSeqCmdAck_InvalidCmd;
	}

	int nChkParam = pSeqCmd->CheckParam();
	if( nChkParam > 0 ){
		delete pSeqCmd;
		pSeqCmd = NULL;
		return eSeqCmdAck_InvalidParam;
	}


	CBaseTaskManager::AddSeqCmd(pSeqCmd, nEqpCtrlCmdType);
	return eSeqCmdAck_OK;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskRegulator::OnEvent(const CBaseTaskManager* pSender,int nEventId, CParamList* pDataList/*=NULL*/)
{
	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskRegulator::OnCommandRsp(CBaseSeqCmd* pRespCmd, int nEventId, WPARAM wParam, LPARAM lParam)
{
	return ERR_NO_ERROR;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-08-27]
  NAME     : 
  PARAMS   : dPressure = [MPa]
  RETURN   : 
  COMMENTS : 레귤레이터 압력 변경하고 싶을 때 이함수 사용 하면 됨.
==============================================================================*/
int CTaskRegulator::do_SetRegulator(double dPressure)
{
	if( dPressure > 0 ){

		if( dPressure < 0.1){
			if( g_IO.GetDriverType() != _DIO_TYPE_SIMUL_ ){
				if( g_RegulatorIF[m_nRegulatorIdx].IsConnected() == FALSE){
					g_RegulatorIF[m_nRegulatorIdx].SetReconnected();
				}
				g_RegulatorIF[m_nRegulatorIdx].SetRegulator(0.1);
			}
		}
		Sleep(100);
		//do_RegulatorValve( DEF_OPEN );
		NEXT;
	}

	int nErr = Interlock_SetRegulator(dPressure);
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}

	if( g_IO.GetDriverType() != _DIO_TYPE_SIMUL_ ){
		if( g_RegulatorIF[m_nRegulatorIdx].IsConnected() == FALSE){
			g_RegulatorIF[m_nRegulatorIdx].SetReconnected();
		}
		g_RegulatorIF[m_nRegulatorIdx].SetRegulator(dPressure);
	}else{
		int nIndex = m_vAO[aoREGUL_OUT];
		g_AIO.SetPhysicalValue(nIndex, dPressure);
	}

	Sleep(100); // ??imsi
//	do_RegulatorValve( DEF_CLOSE );
// 	if( dPressure > 0 ){
// 		int nRet = do_RegulatorValve( DEF_CLOSE );
// 		if( nRet != ERR_NO_ERROR ){
// 			goto RETRY_LOOP
// 		}
// 	}
	
	m_dSetPressure = dPressure;

	return ERR_NO_ERROR;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-08-27]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskRegulator::Interlock_SetRegulator(double dPressure)
{
	return ERR_NO_ERROR;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-08-27]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 레귤레이터의 현재 압력을 읽고 싶을 때 이 함수를 사용 하면 됨.
             값의 단위는 DB에 설정되어 있는 것을 기초로 계산 함.
==============================================================================*/
double CTaskRegulator::GetCurrentPressure()
{
	double dPressure = 0.0;
	CString strUnit = "";
	int nIndex = m_vAI[aiREGUL_IN];

	if( g_IO.GetDriverType() != _DIO_TYPE_SIMUL_ ){
		g_RegulatorIF[m_nRegulatorIdx].GetRecvData(NULL, &dPressure);//GetPress(0);
	}else{
		g_AIO.GetPhysicalValue(_tAIO_LS::eAI, nIndex, &dPressure, strUnit); 
	}

	return dPressure;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : 
  RETURN   : FALSE = ERROR
             TRUE  = NO ERROR
  COMMENTS : 레귤레이터의 셋팅 값대비 오차 범위를 주면 그 내에 있는지 확인.
             Default (0) = +-5%
==============================================================================*/
BOOL CTaskRegulator::ChkPermitNotOver(double dRegulatorPermit )
{
	BOOL bRet = TRUE;
	if( g_IO.GetDriverType() != _DIO_TYPE_SIMUL_ ){
		bRet = g_RegulatorIF[m_nRegulatorIdx].GetPress( dRegulatorPermit );
	}

	return bRet;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-08-27]
  NAME     : 
  PARAMS   : nUnit : 0 = MPa, 1 = KGF, 2 = NEWTON
  RETURN   : 
  COMMENTS : 레귤레이터의 현재 설정 압력을 읽고 싶을 때 이 함수를 사용 하면 됨.
             값의 단위는 DB에 설정되어 있는 것을 기초로 계산 함.
==============================================================================*/
double CTaskRegulator::GetCommandPressure()
{
	double dPressure = 0.0;
	CString strUnit = "";
	int nIndex = m_vAO[aoREGUL_OUT];

	if( g_IO.GetDriverType() != _DIO_TYPE_SIMUL_ ){
		dPressure = g_RegulatorIF[m_nRegulatorIdx].GetSettingMpa();
	}else{
		g_AIO.GetPhysicalValue(_tAIO_LS::eAO, nIndex, &dPressure, strUnit);
	}
	

	return dPressure;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-28]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskRegulator::do_RegulatorValve(BOOL bOpen)
{
//	int nErr = Interlock_RegulatorValve(bOpen);
//	if( nErr != ERR_NO_ERROR ){
//		return nErr;
//	}

	g_IO.out(m_vY[CTaskRegulator::yREGUL_VV_ON_OFF], bOpen);
	//g_IO.out(m_vY[CTaskRegulator::yREGUL_VV_OFF], !bOpen);
	return ERR_NO_ERROR;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-28]
  NAME     : 
  PARAMS   : 
  RETURN   : 0 = OK, 
  COMMENTS : 레귤레이터 제어를 위해서는 Valve가 열려 있어야 한다.
             Valve가 닫혀 있는 상태에서 레귤레이터 압력 제어 시도하게 되면 레귤레이터 고장의 원인이 된다.
==============================================================================*/
int CTaskRegulator::Interlock_RegulatorValve(BOOL bOpen)
{
	if( bOpen == DEF_CLOSE ){
		if( m_dSetPressure <= 0 ){
			return ERR_PRESS_UNIT_REG_INTERLOCK;
		}
	}

	return ERR_NO_ERROR;
}

int CTaskRegulator::do_SetForcePerPin(double dGram)
{
	int nPinCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_DvcContactForce_Pin);
	double dRegSetPressure = ConvertForcePerPin_To_MPa(dGram, nPinCount);
	int nErr = do_SetRegulator(dRegSetPressure);
	return nErr;
}


//==============================================================================
// RETURN : gf/cm2
//==============================================================================
double CTaskRegulator::GetCurrentForcePerPin()
{
	int nPinCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_DvcContactForce_Pin);
	double dCurPress = GetCurrentPressure();
	double dForcePerPin = ConvertMPa_To_ForcePerPin(dCurPress, nPinCount)-m_dForce_TrimValue_gf/1000.0;
	return dForcePerPin;
}

double CTaskRegulator::ConvertMPa_To_ForcePerPin(double a_dMPa, int nPinCount)
{
	CPressUnitConvert cvt(CPressUnitConvert::MPa, a_dMPa);
	double kgf_per_cm2 = cvt.ConvertTo(CPressUnitConvert::KgFPcm2);
	double kgf_per_cyl = kgf_per_cm2 * ( m_dCylinderRadius_cm * m_dCylinderRadius_cm * PI);
	double kgf_per_pin = kgf_per_cyl / (double)nPinCount;
	return kgf_per_pin*1000.0;
}

double CTaskRegulator::ConvertForcePerPin_To_MPa(double a_gf, int nPinCount)
{
	double dTrimedForce = GetTrimForceValue_gf(a_gf * (double)nPinCount);
	double dSetForce_kgf = (dTrimedForce / 1000.0) / ( m_dCylinderRadius_cm * m_dCylinderRadius_cm * PI); // gf/cm2
	double dRegSetPressure;	
	CPressUnitConvert cvt(CPressUnitConvert::KgFPcm2, dSetForce_kgf);
	dRegSetPressure = cvt.ConvertTo(CPressUnitConvert::MPa);	
	return dRegSetPressure;
}

double CTaskRegulator::GetTrimForceValue_gf(double a_dForce_gf, int nCylinderSize)
{	
	ASSERT(nCylinderSize>=0 && nCylinderSize<eMaxContactCylinderSizeCount);

	if( nCylinderSize < 0 ) nCylinderSize = 0;
	if( nCylinderSize >= eMaxContactCylinderSizeCount ) nCylinderSize = eContactCylinderSize_55mm;

 	double dTrimForce_gf;
	m_dForce_TrimValue_gf = (a_dForce_gf * m_dContactForceSlop_gf[nCylinderSize]) + m_dForce_Offset_gf[nCylinderSize];
	dTrimForce_gf = a_dForce_gf + m_dForce_TrimValue_gf; 
	return dTrimForce_gf;
}

double CTaskRegulator::GetTrimForceValue_gf(double a_dForce_gf)
{
	return GetTrimForceValue_gf(a_dForce_gf, m_nCylinder_Size);
}
