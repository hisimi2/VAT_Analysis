#include "StdAfx.h"
#include "BaseTaskManager.h"
#include "DEF_COMMON.h"
#include "DEF_ERROR_CODE.h"
#include "BaseSeqCmd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace base_seq_cmd;

//==============================================================================
// initialize static members
//==============================================================================
CCriticalSection							CBaseTaskManager::m_csObserver;
CList<CBaseTaskManager*, CBaseTaskManager*>	CBaseTaskManager::m_listObserver;
BOOL CBaseTaskManager::m_bEqpControlMode = eEqpCtrlMode_Manual;

//==============================================================================
//
//==============================================================================
CBaseTaskManager::CBaseTaskManager(void)
{
	m_nLastWorkingLoc = 0;
}


//==============================================================================
//
//==============================================================================
CBaseTaskManager::~CBaseTaskManager(void)
{
}


//==============================================================================
//
//==============================================================================
int CBaseTaskManager::SetEqpCtrlMode(int nCtrlMode)
{
	m_bEqpControlMode = nCtrlMode;
	return ERR_NO_ERROR;
}


//==============================================================================
//
//==============================================================================
BOOL CBaseTaskManager::GetEqpCtrlMode()
{
	BOOL bEqpCtrlMode = m_bEqpControlMode;
	return bEqpCtrlMode;
}



//==============================================================================
//
//==============================================================================
int CBaseTaskManager::Initialize()
{
	AttachObserver( this );
	CThreadBase::CreateThread(0);
	return ERR_NO_ERROR;
}


//==============================================================================
//
//==============================================================================
void CBaseTaskManager::Finalize()
{
	DetachObserver( this );
	CThreadBase::CloseThread();
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-10-08]
  NAME     : 
  PARAMS   : TargetTask      : TaskManager 객체를 레퍼런스 타입으로 넘겨 준다.
             pSeqCmd         : SeqCmd를 동적으로 생성하여 넘겨 준다.
			 nEqpCtrlCmdType : enum eEqpCtrlMode
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CBaseTaskManager::SendCommand(CBaseTaskManager& TargetTask, CBaseSeqCmd* pSeqCmd, int nEqpCtrlCmdType/*=eEqpCtrlMode_Auto*/)
{
	int nRetErr = 0;
	pSeqCmd->m_pReceive = &TargetTask;
	pSeqCmd->m_pSender  = this;
	pSeqCmd->m_nCmdSeqMode = nEqpCtrlCmdType;

	if( nEqpCtrlCmdType != GetEqpCtrlMode()){
		delete pSeqCmd;
		pSeqCmd = NULL;
		return ERR_LOT_CMD_EQP_MODE_IS_INVALID;
	}

	if( nEqpCtrlCmdType == eEqpCtrlMode_Auto ){
		// 실행 중인 명령이 있으면 명령을 접수하지 않음.
		if(m_SeqCmdListAuto.GetCmdExecutorStatus() != eCmdExecutorStatus_Idle ){
			delete pSeqCmd;
			pSeqCmd = NULL;
			return ERR_LOT_CMD_CANNOT_PERFORM_NOW;
		}

		//int nTaskCount = CBaseTaskManager::GetObserverCount();
		//for( int i = 0; i<nTaskCount; i++ ) {
		//	CBaseTaskManager* pTask = CBaseTaskManager::GetTaskByIndex( i );
		//	if( pTask->GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready ) {
		//		delete pSeqCmd;
		//		pSeqCmd = NULL;
		//		return ERR_LOT_CMD_CANNOT_PERFORM_NOW;
		//	}
		//}
	}else{
		// Manual Mode에서는 Task한개만 명령 실행 가능 함.
		int nTaskCount = CBaseTaskManager::GetObserverCount();
		for(int i=0; i<nTaskCount; i++){
			CBaseTaskManager* pTask = CBaseTaskManager::GetTaskByIndex(i);
			if( pTask->GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready ){
				delete pSeqCmd;
				pSeqCmd = NULL;
				return ERR_LOT_CMD_CANNOT_PERFORM_NOW;
			}
		}
	}
	
	nRetErr = TargetTask.OnCommand(pSeqCmd, nEqpCtrlCmdType);

	if(nRetErr != eSeqCmdAck_OK){
		ASSERT(0);
	}
	return nRetErr;
}


//==============================================================================
// enum eProcessCommand
// eProcCmd_Pause = 0, ...
//==============================================================================
void CBaseTaskManager::SetProcessCommand(eProcessCommand a_eProcessCommand, int nEqpCtrlCmdType/*=eEqpCtrlMode_Auto*/)
{
	if( nEqpCtrlCmdType == eEqpCtrlMode_Auto ){
		m_SeqCmdListAuto.SetProcessCommand( a_eProcessCommand );
	}else{
		m_SeqCmdListManual.SetProcessCommand( a_eProcessCommand );
	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-10-08]
  NAME     : 
  PARAMS   : pTaskManager :  만약에 NULL이면 모든 TaskManager 객체에게 명령을 전달 한다.
  RETURN   : 
  COMMENTS : 다른 TaskManager 객체에게 프로세스 커맨드를 전달 한다.
==============================================================================*/
void CBaseTaskManager::SendProcessCommand(eProcessCommand a_eProcessCommand, int nEqpCtrlCmdType, CBaseTaskManager* pTargetTaskManager/*=NULL*/)
{
	if( pTargetTaskManager == NULL ){
		int nCount = GetObserverCount();
		for(int i=0; i<nCount; i++){
			CBaseTaskManager* pTask = GetTaskByIndex(i);
			if( pTask != NULL ){
				pTask->SetProcessCommand(a_eProcessCommand, nEqpCtrlCmdType);
			}
		}
	}else{
		pTargetTaskManager->SetProcessCommand(a_eProcessCommand, nEqpCtrlCmdType);
	}
}


//==============================================================================
//
//==============================================================================
int CBaseTaskManager::SendEvent(int nEvtId, CBaseTaskManager* pTargetTask/*=NULL*/, CParamList* pEventData/*=NULL*/)
{
	if( pTargetTask != NULL ){
		pTargetTask->OnEvent(this, nEvtId, pEventData);
	}else{
		int nCount = GetObserverCount();
		for( int i=0; i<nCount; i++){
			CBaseTaskManager* pItem = (CBaseTaskManager*)GetTaskByIndex(i);
			if( pItem != NULL ){
				pItem->OnEvent(this, nEvtId, pEventData);
			}
		}
	}
	return 0;
}

int CBaseTaskManager::OnCommand( CBaseSeqCmd * pSeqCmd, int nEqpCtrlCmdType )
{
	int nChkCmd = 0;
	if( pSeqCmd == NULL )
	{
		return eSeqCmdAck_InvalidCmd;
	}

	int nChkParam = pSeqCmd->CheckParam();
	if( nChkParam > 0 ) {
		delete pSeqCmd;
		pSeqCmd = NULL;
		return eSeqCmdAck_InvalidParam;
	}


	CBaseTaskManager::AddSeqCmd( pSeqCmd, nEqpCtrlCmdType );
	return eSeqCmdAck_OK;
}

//==============================================================================
//
//==============================================================================
int CBaseTaskManager::OnEvent(const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList/*=NULL*/)
{
	return 0;
}


//==============================================================================
//
//==============================================================================
int CBaseTaskManager::OnCommandRsp( CBaseSeqCmd* pRspCmd, int nEventId, WPARAM wParam, LPARAM lParam, std::vector<CPoint>& vPocket_Picker)
{
	if( nEventId == CMD_EVT_ALARM ) {
		g_Error.AlarmReport( wParam, *( CParamList* )lParam, pRspCmd->m_pReceive, vPocket_Picker );
		//g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, wParam, lParam);

  		BOOL vat_in_process = g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_USE);
  		if (vat_in_process != TRUE)
  			g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, wParam, lParam);
  		else
  			g_VatTaskSystemCtrl.SetLotCommand(eAutoLotCmd_AlarmPause);
	}
		
	return 0;
}

//==============================================================================
// static member function
//==============================================================================
int CBaseTaskManager::AttachObserver(CBaseTaskManager* pObserver)
{
	CBaseTaskManager::m_csObserver.Lock();
	if( CheckDuplicate(pObserver) == FALSE ){ // 중복이 아니면 리스트에 추가 함.
		m_listObserver.AddTail(pObserver);
	}
	CBaseTaskManager::m_csObserver.Unlock();
	return 0;
}


//==============================================================================
// static member function
//==============================================================================
int CBaseTaskManager::DetachObserver(CBaseTaskManager* pObserver)
{
	CBaseTaskManager::m_csObserver.Lock();
	POSITION pos = CBaseTaskManager::m_listObserver.GetHeadPosition();
	POSITION pos_pre = NULL;
	int nCount = CBaseTaskManager::m_listObserver.GetCount();
	for(int i=0; i<nCount; i++){
		pos_pre = pos;
		CBaseTaskManager* pItem = CBaseTaskManager::m_listObserver.GetNext(pos);
		if( pItem != NULL ){
			if( pItem == pObserver ){
				CBaseTaskManager::m_listObserver.RemoveAt(pos_pre);
				break;
			}
		}
	}
	CBaseTaskManager::m_csObserver.Unlock();
	return 0;
}

//==============================================================================
// static member function
// 프로그램 종료 시에 호출 바랍니다.
//==============================================================================
void CBaseTaskManager::ClearObserver()
{
	CBaseTaskManager::m_csObserver.Lock();
	POSITION pos = CBaseTaskManager::m_listObserver.GetHeadPosition();
	int nCount = CBaseTaskManager::m_listObserver.GetCount();
	for(int i=0; i<nCount; i++){
		CBaseTaskManager* pItem = CBaseTaskManager::m_listObserver.GetNext(pos);
		if( pItem != NULL ){
			delete pItem;
			pItem = NULL;
		}
	}
	CBaseTaskManager::m_listObserver.RemoveAll();
	CBaseTaskManager::m_csObserver.Unlock();
}

//==============================================================================
// static member function
//==============================================================================
int CBaseTaskManager::GetObserverCount()
{
	CBaseTaskManager::m_csObserver.Lock();
	int nCount = CBaseTaskManager::m_listObserver.GetCount();
	CBaseTaskManager::m_csObserver.Unlock();
	return nCount;
}

//==============================================================================
// static member function
//==============================================================================
CBaseTaskManager* CBaseTaskManager::GetTaskByIndex(int nIndex)
{

	CBaseTaskManager::m_csObserver.Lock();
	POSITION pos = CBaseTaskManager::m_listObserver.FindIndex(nIndex);
	if(pos != NULL){
		CBaseTaskManager* pItem = (CBaseTaskManager*)CBaseTaskManager::m_listObserver.GetAt(pos);
		if( pItem != NULL ){
			CBaseTaskManager::m_csObserver.Unlock();
			return pItem;
		}
	}	
	CBaseTaskManager::m_csObserver.Unlock();
	return NULL;
};


//==============================================================================
// RETURN   : TRUE = duplicate, FALSE = not duplicate
//==============================================================================
BOOL CBaseTaskManager::CheckDuplicate(CBaseTaskManager* pTask)
{
	CBaseTaskManager::m_csObserver.Lock();
	POSITION pos = CBaseTaskManager::m_listObserver.GetHeadPosition();
	int nCount = CBaseTaskManager::m_listObserver.GetCount();
	for(int i=0; i<nCount; i++){
		CBaseTaskManager* pItem = (CBaseTaskManager*)CBaseTaskManager::m_listObserver.GetNext(pos);
		if( pItem != NULL ){
			if( pItem == pTask){
				m_csObserver.Unlock();
				return TRUE;
			}
		}
	}
	CBaseTaskManager::m_csObserver.Unlock();
	return FALSE;
};

//==============================================================================
// 도착한 명령을 큐에서 꺼내어 실행 시킨다.
//==============================================================================
// void CBaseTaskManager::ThreadProc1()
// {
// 	while( GetThreadAliveFlag(0) ){
// 		if( GetPauseFlag(0) ) { 
// 			NEXT;
// 			continue;
// 		}
// 		if( m_bEqpControlMode == eEqpCtrlMode_Auto){
// 			m_SeqCmdListAuto.Run();
// 		}else{
// 			m_SeqCmdListManual.Run();
// 		}
// 		NEXT;
// 	}
// }

void CBaseTaskManager::SeqCmdRunning()
{
    // imsi
	MSG message;
	if(::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
	{
		::TranslateMessage(&message);
		::DispatchMessage(&message);
	}

	if( m_bEqpControlMode == eEqpCtrlMode_Auto){
		m_SeqCmdListAuto.Run();
	}else{
		m_SeqCmdListManual.Run();
	}
}

//==============================================================================
//
//==============================================================================
int CBaseTaskManager::AddSeqCmd(CBaseSeqCmd* pCmd, int nEqpCtrlCmdType)
{
	if( nEqpCtrlCmdType == eEqpCtrlMode_Auto ){
		m_SeqCmdListAuto.AddSeqCmd( pCmd );
	}else{
		m_SeqCmdListManual.AddSeqCmd( pCmd );
	}
	return ERR_NO_ERROR;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-10-09]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CBaseTaskManager::GetSeqCmdStatus(int nEqpCtrlCmdType/*=eEqpCtrlMode_Auto*/)
{
	int nCommandSts = 0;
	if( nEqpCtrlCmdType == eEqpCtrlMode_Auto ){
		nCommandSts = m_SeqCmdListAuto.GetSeqCmdStatus();
	}else{
		nCommandSts = m_SeqCmdListManual.GetSeqCmdStatus();
	}
	return nCommandSts;
}

CString CBaseTaskManager::GetActiveSeqCmdName( int nEqpCtrlMode )
{
	CString sCmdName = _T( "" );
	if( nEqpCtrlMode == eEqpCtrlMode_Auto ) {
		sCmdName = m_SeqCmdListAuto.GetActiveSeqCmdName().GetString();
	}
	else {
		sCmdName = m_SeqCmdListManual.GetActiveSeqCmdName().GetString();
	}
	return sCmdName;
}

int CBaseTaskManager::GetSeqWorkCmdStage(int nEqpCtrlCmdType)
{
	int nCommandStage = 0;
	if (nEqpCtrlCmdType == eEqpCtrlMode_Auto) {
		nCommandStage = m_SeqCmdListAuto.GetSeqCmdWorkCmdStage();
	}
	else {
		nCommandStage = m_SeqCmdListManual.GetSeqCmdWorkCmdStage();
	}
	return nCommandStage;
}