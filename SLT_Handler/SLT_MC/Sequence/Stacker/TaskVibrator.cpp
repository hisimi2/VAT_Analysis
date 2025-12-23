#include "StdAfx.h"
#include "TaskVibrator.h"
#include "ConfigData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CTaskVibrator::CTaskVibrator(void)
{
	m_nCurIntensity = 0;

	m_mapDI.SetAt(xVIBRATOR_SEN1,"VIBRATOR_SEN1");
	m_mapDI.SetAt(xVIBRATOR_SEN2,"VIBRATOR_SEN2");
	m_mapDI.SetAt(xVIBRATOR_SEN3,"VIBRATOR_SEN3");
	m_mapDI.SetAt(xVIBRATOR_SEN4,"VIBRATOR_SEN4");

	m_mapDO.SetAt(yVIBRATOR_1     ,"VIBRATOR_1");
	m_mapDO.SetAt(yVIBRATOR_2     ,"VIBRATOR_2");
	m_mapDO.SetAt(yVIBRATOR_3     ,"VIBRATOR_3");
	m_mapDO.SetAt(yVIBRATOR_4     ,"VIBRATOR_4");
	m_mapDO.SetAt(yVIBRATOR_5     ,"VIBRATOR_5");
	m_mapDO.SetAt(yVIBRATOR_6     ,"VIBRATOR_6");
	m_mapDO.SetAt(yVIBRATOR_7     ,"VIBRATOR_7");
	m_mapDO.SetAt(yVIBRATOR_8	  ,"VIBRATOR_8");
}




/*=============================================================================
  AUTHOR   : 나명진  [2015-09-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CTaskVibrator::~CTaskVibrator(void)
{
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskVibrator::Initialize(char* szDriverFileName)
{
	CConfigData ConfigData(szDriverFileName);
	char szSection[128] = {0,};

	// Digital Input
 	memset(szSection, 0x00, sizeof(szSection));
 	sprintf_s(szSection, sizeof(szSection), "TASK VIBRATOR CONFIG - DI");
 	int nDiCount = m_mapDI.GetCount();
 	for(int i=0; i<nDiCount; i++){
 		char szKey[128] = {0,};
 		CString strName;
 		m_mapDI.Lookup(i, strName);
 		sprintf_s(szKey, sizeof(szKey), "%s", strName.GetString() );
 		int nIdx = ConfigData.GetInt(szSection, szKey, "0");
 		m_vX.push_back(nIdx);
 	}

	// Digital Output
	memset(szSection, 0x00, sizeof(szSection));
	sprintf_s(szSection, sizeof(szSection), "TASK VIBRATOR CONFIG - DO");
	int nDoCount = m_mapDO.GetCount();
	for(int i=0; i<nDoCount; i++){
		char szKey[128] = {0,};
		CString strName;
		m_mapDO.Lookup(i, strName);
		sprintf_s(szKey, sizeof(szKey), "%s", strName.GetString() );
		int nIdx = ConfigData.GetInt(szSection, szKey, "0");
		m_vY.push_back(nIdx);
	}

	return CBaseTaskManager::Initialize();
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CTaskVibrator::Finalize()
{
}

//==============================================================================
// 도착한 명령을 큐에서 꺼내어 실행 시킨다.
//==============================================================================
void CTaskVibrator::ThreadProc1()
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

void CTaskVibrator::OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{

}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-14]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskVibrator::OnCommand(CBaseSeqCmd* pSeqCmd, int nEqpCtrlCmdType/*=eEqpCtrlMode_Auto*/)
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
int CTaskVibrator::OnEvent(const CBaseTaskManager* pSender,int nEventId, CParamList* pDataList/*=NULL*/)
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
int CTaskVibrator::OnCommandRsp(CBaseSeqCmd* pRespCmd, int nEventId, WPARAM wParam, LPARAM lParam)
{
	return ERR_NO_ERROR;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-17]
  NAME     : 
  PARAMS   : nCH : 0 = off, 0 > = 비트셋트를 이용하여 위치 지정
  RETURN   : 
  COMMENTS : 지정된 위치의 바이브레이션을 실행 한다.
             신호가 들어가 있는 동안 계속 바이브레이션 된다.
==============================================================================*/
int CTaskVibrator::do_Vibration(int nCH, BOOL bOnOff)
{
// 	//clear data & action mode
// 	for(int i = 0; i < yMAX_COUNT; i++) {
// 		g_IO.out(m_vY[yVIBRATOR_1 + i], DEF_OFF);
// 		NEXT;
// 	}

	g_IO.out(m_vY[yVIBRATOR_1+nCH], bOnOff);
	return ERR_NO_ERROR;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-17]
  NAME     : 
  PARAMS   : nValue : 0 ~100
  RETURN   : 
  COMMENTS : 바이브레이션 강도를 조정한다.
==============================================================================*/
int CTaskVibrator::do_SetIntensity(int nCh, int nValue)
{
	// all off
	for(int i = 0; i < yMAX_COUNT; i++){
		g_IO.out(m_vY[yVIBRATOR_1 + i], DEF_OFF);
	}	
	//Sleep(10);
	// mode on
	Sleep(50);

    g_IO.out(m_vY[yVIBRATOR_8], DEF_ON); 
	Sleep(50);
	// set position
	switch(nCh)
	{
	case 0:
		{
			g_IO.out(m_vY[yVIBRATOR_1], DEF_ON);
		}break;
	case 1:
		{
			g_IO.out(m_vY[yVIBRATOR_2], DEF_ON);
		}break;
	case 2:
		{
			g_IO.out(m_vY[yVIBRATOR_1], DEF_ON);
			g_IO.out(m_vY[yVIBRATOR_2], DEF_ON);
		}break;
	case 3:
		{
			g_IO.out(m_vY[yVIBRATOR_3], DEF_ON);
		}break;
	default: { g_IO.out(m_vY[yVIBRATOR_1], DEF_ON); }break;
	}

	// Save Position data
	//Sleep(100);
	g_IO.out(m_vY[yVIBRATOR_6], DEF_ON); 
	Sleep(50);
	g_IO.out(m_vY[yVIBRATOR_6], DEF_OFF); 
	// position all off
	for(int i = 0; i < yVIBRATOR_5; i++){
		g_IO.out(m_vY[yVIBRATOR_1 + i], DEF_OFF);
	}
	
	// set level mode change
	g_IO.out(m_vY[yVIBRATOR_7], DEF_ON);
	Sleep(50);
	// set level [ 1 - 
	switch(nValue)
	{
	case 0: // 1
		{
			g_IO.out(m_vY[yVIBRATOR_1], DEF_ON);
		}break;
	case 1: // 6
		{
			g_IO.out(m_vY[yVIBRATOR_2], DEF_ON);
			g_IO.out(m_vY[yVIBRATOR_3], DEF_ON);
		}break;
	case 2: // 11
		{
			g_IO.out(m_vY[yVIBRATOR_1], DEF_ON);
			g_IO.out(m_vY[yVIBRATOR_2], DEF_ON);
			g_IO.out(m_vY[yVIBRATOR_4], DEF_ON);
		}break;
	case 3: // 16
		{
			g_IO.out(m_vY[yVIBRATOR_5], DEF_ON);
		}break;
	case 4: // 21
		{
			g_IO.out(m_vY[yVIBRATOR_1], DEF_ON);
			g_IO.out(m_vY[yVIBRATOR_4], DEF_ON);
			g_IO.out(m_vY[yVIBRATOR_5], DEF_ON);
		}break;
	case 5: // 26
		{
			g_IO.out(m_vY[yVIBRATOR_2], DEF_ON);
			g_IO.out(m_vY[yVIBRATOR_4], DEF_ON);
			g_IO.out(m_vY[yVIBRATOR_5], DEF_ON);
		}break;
	case 6: // 31
		{
			g_IO.out(m_vY[yVIBRATOR_1], DEF_ON);
			g_IO.out(m_vY[yVIBRATOR_2], DEF_ON);
			g_IO.out(m_vY[yVIBRATOR_3], DEF_ON);
			g_IO.out(m_vY[yVIBRATOR_4], DEF_ON);
			g_IO.out(m_vY[yVIBRATOR_5], DEF_ON);
		}break;
	default:
		{
			g_IO.out(m_vY[yVIBRATOR_1], DEF_ON);
		}break;
	}

	// Save level data
	//Sleep(100);
	g_IO.out(m_vY[yVIBRATOR_6], DEF_ON);
	Sleep(50);
	g_IO.out(m_vY[yVIBRATOR_6], DEF_OFF);
	
	// all off
	for(int i = 0; i < yMAX_COUNT; i++){
		g_IO.out(m_vY[yVIBRATOR_1 + i], DEF_OFF);
	}
	Sleep(50);

	m_nCurIntensity = nValue;

	return ERR_NO_ERROR;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-17]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskVibrator::GetIntensity()
{
	return m_nCurIntensity;
}

void CTaskVibrator::MakeLog(LPCTSTR fmt, ...)
{
    TCHAR tmpMsg[1024] = {0,};
    if(fmt)
    {
        va_list argList;
        va_start(argList, fmt);
        _vstprintf_s(tmpMsg, fmt, argList);
        va_end(argList);
    }
    TWLOG.MakeLog(static_cast<int>(eLogID_Vibrator), Debug, "", __LINE__, NULL, tmpMsg);
}

int CTaskVibrator::IsAction(int nCh, BOOL bOnOff)
{
	if( g_IO.GetDriverType() == _DIO_TYPE_SIMUL_ ){
		g_IO.SetSimulDi(m_vX[xVIBRATOR_SEN1 + nCh], bOnOff);
		Sleep(SIMUL_SLEEP_TIME);
	}
	if(g_IO.in(m_vX[xVIBRATOR_SEN1 + nCh]) == bOnOff)
	{
		return ERR_NO_ERROR;
	}

	return -1;
}