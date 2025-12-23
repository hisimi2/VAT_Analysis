#pragma once

#include "DEF_LIB_IPC_SOCK.h"
#include "IPCSock.h"
#include "Logger.h"

#define     UM_GUI_COMM_SOCK_EVENT                         (WM_USER + 3000)
#define     UM_GUI_COMM_LOG_MSG                            (WM_USER + 3001)
#define     UM_GUI_COMM_DLG_EVENT                          (WM_USER + 3002)
#define     UM_GUI_COMM_FINALIZE_PGM                       (WM_USER + 3003)
#define     UM_GUI_MULTI_ORIGIN							   (WM_USER + 3004)

// 127.0.0.1 : 8000, GUI:client, MC:server
class CGUIComm : public CIPCSock
{
public:
	CGUIComm(void);
	~CGUIComm(void);

	static ST_MESSAGE_MAP MessageMap[];

	static int OnReceiveS2F3(CPacket* pPacket);
	static int OnReceiveS2F4(CPacket* pPacket);
	static int OnReceiveS2F41(CPacket* pPacket);
	static int OnReceiveS2F42(CPacket* pPacket);
	static int OnReceiveS5F1 (CPacket* pPacket);
	static int OnReceiveS5F2 (CPacket* pPacket);
	static int OnReceiveS6F11(CPacket* pPacket);
	static int OnReceiveS6F12(CPacket* pPacket);
	static int OnReceiveS107F1(CPacket* pPacket);
	static int OnReceiveS107F2(CPacket* pPacket);
	static int OnReceiveS107F3(CPacket* pPacket);
	static int OnReceiveS107F4(CPacket* pPacket);
	static int OnReceiveS107F5(CPacket* pPacket);
	static int OnReceiveS107F6(CPacket* pPacket);

	LRESULT OnTWIPCEvent(WPARAM wParam, LPARAM lParam);
	int Initialize(CWnd* pParentWnd, char* szDriverFileName);
	void Finalize();
	void SendLogMessageToView(const char* fmt, ...);

	int Send_EventReport(int nEvtId);
	int Send_AlarmReport(int nAlid, int nAlst);

	int EnableAutoGrrCancel(BOOL bEnable = TRUE);
	int SendDataAssignShow(int nSetplateNum = 0);

	int SendTestPpOffSetShow();
	int SendLotClearEnd();
	int SendLotinfoShow();
	int SendQAQTYShow();
	int SendDoorStatusView(BOOL bCoverLock);
public:
	int AttachObserver(CWnd* pObserver);
	int DetachObserver(CWnd* pObserver);

protected:
	void ClearObserver();
	int GetObserverCount();
	CWnd* GetObserverByIndex(int nIndex);
	BOOL CheckDuplicate(CWnd* pItem);

protected:
	CCriticalSection	m_csObserver;
	CList<CWnd*, CWnd*>	m_listObserver;
	CLogger             m_Logger;


};

