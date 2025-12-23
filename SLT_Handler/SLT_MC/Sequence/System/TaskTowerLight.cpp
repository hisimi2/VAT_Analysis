#include "StdAfx.h"
#include "TaskTowerLight.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-17]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CTaskTowerLight::CTaskTowerLight(void)
{
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-17]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CTaskTowerLight::~CTaskTowerLight(void)
{
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-17]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskTowerLight::Initialize(char* szDriverFileName)
{
	return CBaseTaskManager::Initialize();
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-17]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CTaskTowerLight::Finalize()
{
	CBaseTaskManager::Finalize();
}

//==============================================================================
// 도착한 명령을 큐에서 꺼내어 실행 시킨다.
//==============================================================================
void CTaskTowerLight::ThreadProc1()
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

void CTaskTowerLight::OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{

}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-17]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskTowerLight::OnCommand(CBaseSeqCmd* pSeqCmd, int nEqpCtrlCmdType/*=eEqpCtrlMode_Auto*/)
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
  AUTHOR   : 나명진  [2015-09-17]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskTowerLight::OnEvent(const CBaseTaskManager* pSender,int nEventId, CParamList* pDataList/*=NULL*/)
{
	return ERR_NO_ERROR;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-17]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskTowerLight::OnCommandRsp(CBaseSeqCmd* pRespCmd, int nEventId, WPARAM wParam, LPARAM lParam)
{
	return ERR_NO_ERROR;
}


