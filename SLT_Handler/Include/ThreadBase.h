// threadbase.h: interface for the CThreadBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADBASE_H__97F2E3AC_B212_4869_A806_F89B00BB77A2__INCLUDED_)
#define AFX_THREADBASE_H__97F2E3AC_B212_4869_A806_F89B00BB77A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <process.h>


//==============================================================================
//
//==============================================================================
#define C_MAX_THREAD_COUNT				10
#define	C_THREAD_DEFAULT_INTERVAL		100


//==============================================================================
//
//==============================================================================
class AFX_EXT_CLASS CThreadBase  
{
public:
	CThreadBase();
	virtual ~CThreadBase();

// Attribute
protected:
	// Thread 관련 Flag's
	HANDLE	m_hThreadHandle[C_MAX_THREAD_COUNT];			// Thread Handle
	BOOL	m_bThreadAliveFlag[C_MAX_THREAD_COUNT];			// Thread Alive Flag
	BOOL	m_bThreadPauseFlag[C_MAX_THREAD_COUNT];			// Thread Pause Flag
	UINT	m_unThreadID[C_MAX_THREAD_COUNT];				// Thread ID
	DWORD	m_dwInterval[C_MAX_THREAD_COUNT];				// Thread sleep Interval

	static unsigned int __stdcall	ThreadSpawn1	(void* pParam);
	static unsigned int __stdcall	ThreadSpawn2	(void* pParam);
	static unsigned int __stdcall	ThreadSpawn3	(void* pParam);
	static unsigned int __stdcall	ThreadSpawn4	(void* pParam);
	static unsigned int __stdcall	ThreadSpawn5	(void* pParam);
	static unsigned int __stdcall	ThreadSpawn6	(void* pParam);
	static unsigned int __stdcall	ThreadSpawn7	(void* pParam);
	static unsigned int __stdcall	ThreadSpawn8	(void* pParam);
	static unsigned int __stdcall	ThreadSpawn9	(void* pParam);
	static unsigned int __stdcall	ThreadSpawn10	(void* pParam);
	virtual void ThreadProc1()		{};
	virtual void ThreadProc2()		{};
	virtual void ThreadProc3()		{};
	virtual void ThreadProc4()		{};
	virtual void ThreadProc5()		{};
	virtual void ThreadProc6()		{};
	virtual void ThreadProc7()		{};
	virtual void ThreadProc8()		{};
	virtual void ThreadProc9()		{};
	virtual void ThreadProc10()		{};

	// Child Class에서 사용하는 함수
	void	ResetParam			(long lThread = C_MAX_THREAD_COUNT);
	int		CreateThread		(long lThread);
	int		CloseThread			(long lThread = C_MAX_THREAD_COUNT);
	void	SetThreadAliveFlag	(long lThread,	BOOL bFlag)			{ m_bThreadAliveFlag[lThread] = bFlag;	}
	BOOL	GetThreadAliveFlag	(long lThread)						{ return m_bThreadAliveFlag[lThread];	}
	void	PauseThread			(long lThread)						{ SetPauseFlag (lThread, TRUE);			}
	void	ResumeThread		(long lThread)						{ SetPauseFlag (lThread, FALSE);		}
	void	SetPauseFlag		(long lThread, BOOL bPause);
	BOOL	GetPauseFlag		(long lThread);
	void	SetThreadInterval	(long lThread,	DWORD dwInterval)	{ m_dwInterval[lThread] = dwInterval;	}
	DWORD	GetThreadInterval	(long lThread)						{ return m_dwInterval[lThread];			}

public:
	BOOL	CheckTerminateThread();
	void	TerminateThread		();
};

#endif // !defined(AFX_THREADBASE_H__97F2E3AC_B212_4869_A806_F89B00BB77A2__INCLUDED_)
