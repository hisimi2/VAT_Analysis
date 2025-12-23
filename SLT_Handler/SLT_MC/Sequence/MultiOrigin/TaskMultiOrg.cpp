#include "StdAfx.h"
#include "TaskMultiOrg.h"


CTaskMultiOrg::CTaskMultiOrg(void)
{
}


CTaskMultiOrg::~CTaskMultiOrg(void)
{
}

int CTaskMultiOrg::Initialize( char* szDriverFileName )
{
	//CreateThread(0);
	return CBaseTaskManager::Initialize();
}

void CTaskMultiOrg::Finalize()
{

}

//==============================================================================
// 도착한 명령을 큐에서 꺼내어 실행 시킨다.
//==============================================================================
void CTaskMultiOrg::ThreadProc1()
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

int CTaskMultiOrg::OnEvent( const CBaseTaskManager* pSender,int nEventId, CParamList* pDataList/*=NULL*/ )
{
	return 0;
}

int CTaskMultiOrg::OnCommandRsp( CBaseSeqCmd* pRespCmd, int nEventId, WPARAM wParam, LPARAM lParam )
{
	return 0;
}

void CTaskMultiOrg::OnOperatorCommand( int nOperatorCommand, WPARAM wParam, LPARAM lParam )
{

}
