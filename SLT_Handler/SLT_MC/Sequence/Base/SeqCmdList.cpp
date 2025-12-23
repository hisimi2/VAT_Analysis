#include "StdAfx.h"
#include "SeqCmdList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace base_seq_cmd;

//==============================================================================
//
//==============================================================================
CSeqCmdList::CSeqCmdList()
{
	m_pActiveCmd = NULL;
	m_eCmdExcutorStatus = eCmdExecutorStatus_Idle;
}


//==============================================================================
//
//==============================================================================
CSeqCmdList::~CSeqCmdList()
{
	ClearCmd();
}


//==============================================================================
//
//==============================================================================
void CSeqCmdList::SetProcessCommand(eProcessCommand a_eProcessCommand)
{
	if( m_eCmdExcutorStatus != eCmdExecutorStatus_Executing){
		return;
	}
	if( m_pActiveCmd != NULL )
	{
		if( a_eProcessCommand == eProcCmd_Pause ){
			m_pActiveCmd->OnPause();
		}else if( a_eProcessCommand == eProcCmd_Resume ){
			if(GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Pause || GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Alarm){
				m_pActiveCmd->OnResume();
			}			
		}else if( a_eProcessCommand == eProcCmd_Retry ){
			m_pActiveCmd->OnRetry();
		}else if( a_eProcessCommand == eProcCmd_Skip ){
			m_pActiveCmd->OnSkip();
		}else if( a_eProcessCommand == eProcCmd_Stop ){
			m_pActiveCmd->OnStop();
		}
	}
}

//==============================================================================
//
//==============================================================================
void CSeqCmdList::Run()
{
	switch( m_eCmdExcutorStatus )
	{
	case eCmdExecutorStatus_Idle:
		{
			if( GetSeqCmd() > 0 ){
				DispatchSeqCmd();
				m_eCmdExcutorStatus = eCmdExecutorStatus_Executing;
			}
		}break;

	case eCmdExecutorStatus_Executing:
		{
			/*YBD Break : m_pActiveCmd->m_pReceive == &g_TaskTrayPP*/
			if( m_pActiveCmd->Running() == TRUE ){
				m_eCmdExcutorStatus = eCmdExecutorStatus_Complete;
			}
		}break;

	case eCmdExecutorStatus_Complete:
		{
			DelCmd( m_pActiveCmd );
			m_eCmdExcutorStatus = eCmdExecutorStatus_Idle;
		}break;
	}
}


//==============================================================================
//
//==============================================================================
int CSeqCmdList::AddSeqCmd(CBaseSeqCmd* pCmd)
{
	m_csSeqCmd.Lock();
	
	pCmd->SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Queue);

	if(m_SeqCmdQ.GetCount() == 0) {
		m_pActiveCmd = pCmd;
	}

	m_SeqCmdQ.AddTail( pCmd );
	m_csSeqCmd.Unlock();
	return ERR_NO_ERROR;
}

//==============================================================================
// 수신된 SeqCmd가 있는지 본다.
//==============================================================================
int CSeqCmdList::GetSeqCmd()
{
	m_csSeqCmd.Lock();
	int nCount = m_SeqCmdQ.GetCount();
	m_csSeqCmd.Unlock();
	return nCount;
}


//==============================================================================
//
//==============================================================================
void CSeqCmdList::DispatchSeqCmd()
{
	m_csSeqCmd.Lock();
	m_pActiveCmd = (CBaseSeqCmd*)m_SeqCmdQ.GetHead();
	m_pActiveCmd->SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Setup);
	m_csSeqCmd.Unlock();
}


//==============================================================================
//
//==============================================================================
void CSeqCmdList::DelCmd( CBaseSeqCmd* pCmd)
{
	m_csSeqCmd.Lock();
	m_SeqCmdQ.RemoveHead();
	delete m_pActiveCmd;
	m_pActiveCmd = NULL;
	m_csSeqCmd.Unlock();
}


//==============================================================================
//
//==============================================================================
void CSeqCmdList::ClearCmd()
{
	m_csSeqCmd.Lock();
	int nCount = m_SeqCmdQ.GetCount();
	POSITION pos = m_SeqCmdQ.GetHeadPosition();
	for(int i=0; i<nCount; i++){
		CBaseSeqCmd* pItem = m_SeqCmdQ.GetNext(pos);
		if( pItem != NULL ){
			delete pItem;
			pItem = NULL;
		}
	}
	m_SeqCmdQ.RemoveAll();
	m_csSeqCmd.Unlock();
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-10-09]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 현재 실행 중인 시퀀스 커맨드의 상태 정보를 반환 한다.
==============================================================================*/
int CSeqCmdList::GetSeqCmdStatus()
{
	m_csSeqCmd.Lock();
	int nCommandStatus = CBaseSeqCmd::eSeqCmdStatus_Ready;
	if( m_pActiveCmd != NULL ){
		nCommandStatus = m_pActiveCmd->GetCmdStatus();
	}
	m_csSeqCmd.Unlock();
	return nCommandStatus;
}

CString CSeqCmdList::GetActiveSeqCmdName()
{
	m_csSeqCmd.Lock();
	CString pSeqName = _T( "" );
	if( m_pActiveCmd != NULL )
	{
		pSeqName = m_pActiveCmd->GetCmdName();
	}
	m_csSeqCmd.Unlock();
	return pSeqName;
}

int CSeqCmdList::GetSeqCmdWorkCmdStage()
{
	m_csSeqCmd.Lock();
	int nCommandStatus = -1;
	if (m_pActiveCmd != NULL) {
		nCommandStatus = m_pActiveCmd->GetWorkCmdStage();
	}
	m_csSeqCmd.Unlock();
	return nCommandStatus;
}