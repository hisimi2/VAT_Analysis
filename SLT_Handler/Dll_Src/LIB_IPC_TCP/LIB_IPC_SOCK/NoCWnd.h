#pragma once

namespace NoCWndThread
{
	void SetTimer(int nID, UINT ms, LPVOID param);
	void KillTimer(int nID);
	LRESULT SendMessage(UINT Msg,WPARAM wParam,LPARAM lParam);
	LRESULT PostMessage(UINT Msg,WPARAM wParam,LPARAM lParam);
	void StartSockEvnetThread(LPVOID pThis);
	void KillSockEventThread();
}

