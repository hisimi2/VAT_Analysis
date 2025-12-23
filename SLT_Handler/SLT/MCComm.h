#pragma once

#include "DEF_LIB_IPC_SOCK.h"
#include "IPCSock.h"

#define     UM_MC_COMM_SOCK_EVENT                         (WM_USER + 3000)
#define     UM_MC_COMM_LOG_MSG                            (WM_USER + 3001)

class CMCComm : public CIPCSock
{
public:
	CMCComm(void);
	~CMCComm(void);

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

	int Initialize(CWnd* pParentWnd, char* szDriverFileName);
	void Finalize();
	LRESULT OnTWIPCEvent(WPARAM wParam, LPARAM lParam);
	void SendLogMessageToView(const char* fmt, ...);

    // Lot Process control
    int CmdProcessControl(int nEvtSeqID);
	int CmdOnlyOneCmdNoParam(int nCmdID);
	int CmdVisionUseNouse(int nTargetPos/*=eVisionPos*/);
	int CmdYieldMonitorDataClear();
	int CmdBinCategoryRefresh();
	int CmdPackageConversion();
	int CmdYieldMonitorDataClear_Each(int nIdx, int nInlinePara,int nYieldMode,int nSocketNo);
	int CmdTesterOnlineModeChange(int nMode);
	int CmdModuleSiteOnPossible();
	int CmdTrayPPSafetyMovePos();
	int CmdMultiOrigin(int nMode);
	int CmdTesterBatchFileExcute();
	int CmdLoadSupply();

    int OnSendDlgShow(int nDlgIdx);
    int OnSndFinalizeMC();
	int CmdTeachingDataReLoad();//Contact 화면에서 Press Contact Teaching 값이 변경되기 때문
	int MakeSummary();
	int CmdDefaultBinCategoryRefresh();

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

