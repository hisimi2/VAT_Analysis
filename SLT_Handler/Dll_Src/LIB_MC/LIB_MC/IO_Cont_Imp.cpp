#include "StdAfx.h"
#include "IO_Cont_Imp.h"


CIO_Cont_Imp::CIO_Cont_Imp(void)
{
	m_nRefObjCount = 0;
	m_bStopThread = FALSE;
}


CIO_Cont_Imp::~CIO_Cont_Imp(void)
{
}

int CIO_Cont_Imp::Initialize( const char* szDriverIniFileName, int nFileNameLen )
{
	if(m_nRefObjCount == 0){
		CreateThread(0);
		m_bStopThread = FALSE;
		CWinThread* pThread = AfxBeginThread(ThreadProc_Running_IO, this);
		pThread->SetThreadPriority(THREAD_PRIORITY_TIME_CRITICAL);
	}
	m_nRefObjCount++;
	return 0;
}

void CIO_Cont_Imp::Finalize()
{
	m_nRefObjCount--;
	if( m_nRefObjCount == 0 ){
		CloseThread();
		m_bStopThread = TRUE;
		::WaitForSingleObject(m_evtTerminated, 500);
	}
}

UINT CIO_Cont_Imp::ThreadProc_Running_IO(LPVOID lpVoid)
{
	CIO_Cont_Imp* pSelf = (CIO_Cont_Imp*)lpVoid;
	//TRACE(_T("void IO_Cont_Imp::ThreadProc1() START.\n"));
	//SetThreadInterval(0, 50);
	while( !pSelf->m_bStopThread )//GetThreadAliveFlag(0) )
	{
		//if( GetPauseFlag(0) ) {
		//	NEXT;
		//	continue;
		//}

		if( g_pDIOContImp != NULL ){
			if( g_pDIOContImp->IsInitBoard() )
			{
				g_pDIOContImp->Write_DO_All();
				Sleep(1);
				g_pDIOContImp->Read_DI_All();
			}
		}
		if( g_pAioContImp != NULL ){
			if( g_pAioContImp->IsInitBoard() )
			{
				g_pAioContImp->Write_AO_All();
				g_pAioContImp->Read_AI_All();
			}
		}

// 		if(g_pDIOContImp != NULL && g_pAioContImp != NULL){
// 			if(g_SyconDrv.IsConnected() == FALSE){
// 				g_SyconDrv.CloseDriver();
// 				g_SyconDrv.OpenDriver();
// 			}
// 		}
		
        Sleep(1);
	}
	//TRACE(_T("void IO_Cont_Imp::ThreadProc1() END.\n"));
	
	pSelf->m_evtTerminated.SetEvent();
	return 0;
}
