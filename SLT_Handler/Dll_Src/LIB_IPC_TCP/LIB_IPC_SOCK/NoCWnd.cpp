#include "StdAfx.h"
#include "NoCWnd.h"
#include "IPCSock.h"

#include <deque>
#include "PerformanceTime.h"

namespace NoCWndThread
{
	DWORD WINAPI g_fnSockEventThread(LPVOID lpThreadParameter);


	static CRITICAL_SECTION g_csSockEventQ; /*message Q C.S*/
	static std::deque<MSG> g_dqSockEventQ; /*message Q*/
	static BOOL g_bSockEventThreadKill = FALSE; /*TRUE = message loop thread kill*/
	static HANDLE g_hSockEventThread = NULL; /*message loop thread handle*/
	static HANDLE g_hSockEventQed = NULL; /*exec message loop*/
	static CPerformanceTime g_tmr; /*timer stopwatch*/
	/*timer simulation*/
	static int g_tmrID = 0;
	static UINT g_tmrMS = 0;
	static LPVOID g_tmrParam = NULL;

	void SetTimer(int nID, UINT ms, LPVOID param)
	{
		g_tmrID = nID;
		g_tmrMS = ms;
		g_tmrParam = param;
		g_tmr.StartTmChk();
	}

	void KillTimer(int nID)
	{
		g_tmrID = nID;
		g_tmrMS = 0;
		g_tmrParam = NULL;
	}


	void StartSockEvnetThread(LPVOID pThis)
	{
		g_bSockEventThreadKill = FALSE;
		if(g_hSockEventThread == NULL)
		{
			::InitializeCriticalSection(&g_csSockEventQ);
			g_hSockEventThread = CreateThread(NULL, 0, NoCWndThread::g_fnSockEventThread, pThis, 0, NULL);
			ResumeThread(g_hSockEventThread);
		}
	}

	void KillSockEventThread()
	{
		g_bSockEventThreadKill = TRUE;

		if(g_hSockEventThread)
			WaitForSingleObject(g_hSockEventThread, INFINITE);

		g_hSockEventThread = NULL;
		::DeleteCriticalSection(&g_csSockEventQ);
	}

	/*blocking send message*/
	LRESULT SendMessage(UINT Msg,WPARAM wParam,LPARAM lParam)
	{
		if(g_hSockEventThread == NULL)
			return 0;

		MSG m;
		m.hwnd = (HWND)CreateEvent(NULL, FALSE, FALSE, NULL);
		m.wParam = wParam;
		m.lParam = lParam;
		m.message = Msg;

		EnterCriticalSection(&g_csSockEventQ);
		g_dqSockEventQ.push_back(m);
		LeaveCriticalSection(&g_csSockEventQ);
		
		SetEvent(g_hSockEventQed);

		WaitForSingleObject(m.hwnd, INFINITE);
		CloseHandle(m.hwnd);
		return 0;
	}

	/*non-blocking send message*/
	LRESULT PostMessage(UINT Msg,WPARAM wParam,LPARAM lParam)
	{
		MSG m;
		m.hwnd = NULL;
		m.wParam = wParam;
		m.lParam = lParam;
		m.message = Msg;

		EnterCriticalSection(&g_csSockEventQ);
		g_dqSockEventQ.push_back(m);
		LeaveCriticalSection(&g_csSockEventQ);

		SetEvent(g_hSockEventQed);

		return 0;
	}
	DWORD WINAPI g_fnSockEventThread(LPVOID lpThreadParameter)
	{
		CIPCSock* pThis = (CIPCSock*)lpThreadParameter;
		g_hSockEventQed = CreateEvent(NULL, TRUE, FALSE, NULL);
		while(g_bSockEventThreadKill == FALSE)
		{
			/*wait message Q*/
			if(WaitForSingleObject(g_hSockEventQed, 1) == WAIT_OBJECT_0)
			{
				EnterCriticalSection(&g_csSockEventQ);

				while(g_dqSockEventQ.empty() == false) {
					/*deQ fron message*/
					MSG msg = g_dqSockEventQ.front();
					g_dqSockEventQ.pop_front();
					LeaveCriticalSection(&g_csSockEventQ);


					/*call message handler*/
					switch(msg.message)
					{
					case UM_TWIPCSOCK_RCV_EVENT: pThis->OnTWIPCEvent(msg.wParam, msg.lParam);
					default: break;
					}

					/*resolve for sendmessage blocking*/
					if(msg.hwnd)
						SetEvent(msg.hwnd);
				}//while(g_dqSockEventQ

				LeaveCriticalSection(&g_csSockEventQ);

				ResetEvent(g_hSockEventQed);
			}
			/*no message -> timer*/
			else{
				if(g_tmrMS != 0){
					if(g_tmr.GetTmDistanceMS() > g_tmrMS){
						pThis->OnTimer(g_tmrID);
						g_tmr.StartTmChk();
					}
				}
			}
		}

		return 0;
	}

}
