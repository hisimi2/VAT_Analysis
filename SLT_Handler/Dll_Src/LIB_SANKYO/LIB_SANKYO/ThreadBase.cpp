// threadbase.cpp: implementation of the CThreadBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ThreadBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThreadBase::CThreadBase()
{
	ResetParam (C_MAX_THREAD_COUNT);
}

CThreadBase::~CThreadBase()
{
	CloseThread (C_MAX_THREAD_COUNT);
}

// Thread 생성 부분
unsigned int __stdcall CThreadBase::ThreadSpawn1 (void *pParam)
{
	CThreadBase *pSelf		= static_cast<CThreadBase*>(pParam);	// Class Pointer cast
	pSelf->ThreadProc1 ();											// 실제 Thread 수행함수
	pSelf->m_bThreadAliveFlag[0] = FALSE;
	CloseHandle(pSelf->m_hThreadHandle[0]);							// clear thread handle
	pSelf->ResetParam (0);
	return 0;
}
unsigned int __stdcall CThreadBase::ThreadSpawn2 (void *pParam)
{
	CThreadBase *pSelf		= static_cast<CThreadBase*>(pParam);	// Class Pointer cast
	pSelf->ThreadProc2 ();											// 실제 Thread 수행함수
	pSelf->m_bThreadAliveFlag[1] = FALSE;
	CloseHandle(pSelf->m_hThreadHandle[1]);					// clear thread handle
	pSelf->ResetParam (1);
	return 0;
}
unsigned int __stdcall CThreadBase::ThreadSpawn3 (void *pParam)
{
	CThreadBase *pSelf		= static_cast<CThreadBase*>(pParam);	// Class Pointer cast
	pSelf->ThreadProc3 ();											// 실제 Thread 수행함수
	pSelf->m_bThreadAliveFlag[2] = FALSE;
	CloseHandle(pSelf->m_hThreadHandle[2]);					// clear thread handle
	pSelf->ResetParam (2);
	return 0;
}
unsigned int __stdcall CThreadBase::ThreadSpawn4 (void *pParam)
{
	CThreadBase *pSelf		= static_cast<CThreadBase*>(pParam);	// Class Pointer cast
	pSelf->ThreadProc4 ();											// 실제 Thread 수행함수
	pSelf->m_bThreadAliveFlag[3] = FALSE;
	CloseHandle(pSelf->m_hThreadHandle[3]);					// clear thread handle
	pSelf->ResetParam (3);
	return 0;
}
unsigned int __stdcall CThreadBase::ThreadSpawn5 (void *pParam)
{
	CThreadBase *pSelf		= static_cast<CThreadBase*>(pParam);	// Class Pointer cast
	pSelf->ThreadProc5 ();											// 실제 Thread 수행함수
	pSelf->m_bThreadAliveFlag[4] = FALSE;
	CloseHandle(pSelf->m_hThreadHandle[4]);					// clear thread handle
	pSelf->ResetParam (4);
	return 0;
}
unsigned int __stdcall CThreadBase::ThreadSpawn6 (void *pParam)
{
	CThreadBase *pSelf		= static_cast<CThreadBase*>(pParam);	// Class Pointer cast
	pSelf->ThreadProc6 ();											// 실제 Thread 수행함수
	pSelf->m_bThreadAliveFlag[5] = FALSE;
	CloseHandle(pSelf->m_hThreadHandle[5]);					// clear thread handle
	pSelf->ResetParam (5);
	return 0;
}
unsigned int __stdcall CThreadBase::ThreadSpawn7 (void *pParam)
{
	CThreadBase *pSelf		= static_cast<CThreadBase*>(pParam);	// Class Pointer cast
	pSelf->ThreadProc7 ();											// 실제 Thread 수행함수
	pSelf->m_bThreadAliveFlag[6] = FALSE;
	CloseHandle(pSelf->m_hThreadHandle[6]);					// clear thread handle
	pSelf->ResetParam (6);
	return 0;
}
unsigned int __stdcall CThreadBase::ThreadSpawn8 (void *pParam)
{
	CThreadBase *pSelf		= static_cast<CThreadBase*>(pParam);	// Class Pointer cast
	pSelf->ThreadProc8 ();											// 실제 Thread 수행함수
	pSelf->m_bThreadAliveFlag[7] = FALSE;
	CloseHandle(pSelf->m_hThreadHandle[7]);					// clear thread handle
	pSelf->ResetParam (7);
	return 0;
}
unsigned int __stdcall CThreadBase::ThreadSpawn9 (void *pParam)
{
	CThreadBase *pSelf		= static_cast<CThreadBase*>(pParam);	// Class Pointer cast
	pSelf->ThreadProc9 ();											// 실제 Thread 수행함수
	pSelf->m_bThreadAliveFlag[8] = FALSE;
	CloseHandle(pSelf->m_hThreadHandle[8]);					// clear thread handle
	pSelf->ResetParam (8);
	return 0;
}
unsigned int __stdcall CThreadBase::ThreadSpawn10 (void *pParam)
{
	CThreadBase *pSelf		= static_cast<CThreadBase*>(pParam);	// Class Pointer cast
	pSelf->ThreadProc10 ();											// 실제 Thread 수행함수
	pSelf->m_bThreadAliveFlag[9] = FALSE;
	CloseHandle(pSelf->m_hThreadHandle[9]);					// clear thread handle
	pSelf->ResetParam (9);
	return 0;
}

int CThreadBase::CreateThread (long lThread)
{
	if (lThread < 0 || lThread >= C_MAX_THREAD_COUNT)
		return -1;

	if (GetThreadAliveFlag (lThread) != TRUE)		// 아직 Thread가 활성화 되지 않은 상태
	{
		SetThreadAliveFlag	(lThread, TRUE);

		switch (lThread)
		{
		case 0:		m_hThreadHandle[lThread] = (HANDLE)_beginthreadex(NULL, 0, ThreadSpawn1,	this, 0, &(m_unThreadID[lThread]));	break;
		case 1:		m_hThreadHandle[lThread] = (HANDLE)_beginthreadex(NULL, 0, ThreadSpawn2,	this, 0, &(m_unThreadID[lThread]));	break;
		case 2:		m_hThreadHandle[lThread] = (HANDLE)_beginthreadex(NULL, 0, ThreadSpawn3,	this, 0, &(m_unThreadID[lThread]));	break;
		case 3:		m_hThreadHandle[lThread] = (HANDLE)_beginthreadex(NULL, 0, ThreadSpawn4,	this, 0, &(m_unThreadID[lThread]));	break;
		case 4:		m_hThreadHandle[lThread] = (HANDLE)_beginthreadex(NULL, 0, ThreadSpawn5,	this, 0, &(m_unThreadID[lThread]));	break;
		case 5:		m_hThreadHandle[lThread] = (HANDLE)_beginthreadex(NULL, 0, ThreadSpawn6,	this, 0, &(m_unThreadID[lThread]));	break;
		case 6:		m_hThreadHandle[lThread] = (HANDLE)_beginthreadex(NULL, 0, ThreadSpawn7,	this, 0, &(m_unThreadID[lThread]));	break;
		case 7:		m_hThreadHandle[lThread] = (HANDLE)_beginthreadex(NULL, 0, ThreadSpawn8,	this, 0, &(m_unThreadID[lThread]));	break;
		case 8:		m_hThreadHandle[lThread] = (HANDLE)_beginthreadex(NULL, 0, ThreadSpawn9,	this, 0, &(m_unThreadID[lThread]));	break;
		case 9:		m_hThreadHandle[lThread] = (HANDLE)_beginthreadex(NULL, 0, ThreadSpawn10,	this, 0, &(m_unThreadID[lThread]));	break;
		default:
			return -1;
		}
		
		//if ( m_hThreadHandle[lThread] < 0 )			// 생성 실패
		if (m_hThreadHandle[lThread] == NULL)			// 생성 실패
		{			
			SetThreadAliveFlag (lThread, FALSE);
			return -1;
		}
	}

	return 0;
}

int CThreadBase::CloseThread (long lThread)
{
	DWORD	dwResult	= 0;
	long	i;
	for (i=0; i<C_MAX_THREAD_COUNT; i++)
	{
		if (lThread == i || lThread == C_MAX_THREAD_COUNT)
		{
			if (GetThreadAliveFlag (i) == TRUE)		// Thread가 활성화 되어 있는 상태
			{
				SetThreadAliveFlag (i, FALSE);

				if (::WaitForSingleObject(m_hThreadHandle[i], (GetThreadInterval (i) * 2)) != 0)
					dwResult++;
			}
		}

		if (lThread != C_MAX_THREAD_COUNT)
			break;
	}
	return dwResult;
}

void CThreadBase::ResetParam (long lThread)
{
	long i;
	for (i=0; i<C_MAX_THREAD_COUNT; i++)
	{
		if (lThread == i || lThread == C_MAX_THREAD_COUNT)
		{
			m_hThreadHandle[i]			= NULL;
			m_bThreadAliveFlag[i]		= FALSE;
			m_bThreadPauseFlag[i]		= FALSE;
			m_unThreadID[i]				= 0;
			m_dwInterval[i]				= C_THREAD_DEFAULT_INTERVAL;
		}
		if (lThread != C_MAX_THREAD_COUNT)
			break;
	}
}


void CThreadBase::SetPauseFlag (long lThread, BOOL bPause)
{
	m_bThreadPauseFlag[lThread] = bPause;
}

BOOL CThreadBase::GetPauseFlag (long lThread)
{
	BOOL bPause = m_bThreadPauseFlag[lThread];
	return bPause;
}

BOOL CThreadBase::CheckTerminateThread()
{
	long i;
	for (i=0; i<C_MAX_THREAD_COUNT; i++)
	{
		if (m_hThreadHandle[i] != NULL)
			return FALSE;
	}

	return TRUE;
};

void CThreadBase::TerminateThread ()
{
	long i;
	for (i=0; i<C_MAX_THREAD_COUNT; i++)
	{
		SetThreadAliveFlag (i, FALSE);
	}
}