#pragma once

#include "ThreadBase.h"
#include "SeqCmdList.h"
#include "Logger.h"
#include <afxmt.inl>

//==============================================================================
//
//==============================================================================
class CParamList;
class CThreadBase;
class CBaseSeqCmd;
class CCriticalSection;

enum eSeqMode{
	eSeqMode_Manual = 0,
	eSeqMode_Auto,
	eMaxSeqMode,
};

//==============================================================================
//
//==============================================================================
typedef int (*SeqCmdProcCallback)(void* lpVoid);
typedef struct _ST_SEQ_CMD_MAP{
	int					nSeqCmdId;
	SeqCmdProcCallback  SeqCmdProc;
}ST_SEQ_CMD_MAP;

typedef void (*TaskEventCallback)(void* lpVoid);
typedef struct _ST_SEQ_EVENT_MAP{
	int					nEventId;
	TaskEventCallback   TaskEventProc;
}ST_SEQ_EVENT_MAP;



//==============================================================================
// CBaseTaskManager
//==============================================================================
class CBaseTaskManager : public CThreadBase
{
public:
	CBaseTaskManager(void);
	virtual ~CBaseTaskManager(void);

	virtual int Initialize();
	virtual void Finalize();

	// Task간의 통신 기능 지원. (명령, 이벤트 전달)
	virtual int SendCommand(CBaseTaskManager& TargetTask, CBaseSeqCmd* pSeqCmd, int nEqpCtrlCmdType=eEqpCtrlMode_Auto);
	virtual int  OnEvent(const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList=NULL);
	virtual BOOL IsInitNeed() {return TRUE;};
	virtual void OnOperatorCommand( int nOperatorCommand, WPARAM wParam, LPARAM lParam ) = 0;

	void SetProcessCommand(base_seq_cmd::eProcessCommand a_eProcessCommand, int nEqpCtrlCmdType=eEqpCtrlMode_Auto); //enum eProcessCommand
	static void SendProcessCommand(base_seq_cmd::eProcessCommand a_eProcessCommand, int nEqpCtrlCmdType, CBaseTaskManager* pTargetTaskManager=NULL);
	int  OnCommand( CBaseSeqCmd* pSeqCmd, int nEqpCtrlCmdType = eEqpCtrlMode_Auto );
	int SendEvent( int nEvtId, CBaseTaskManager* pTargetTask = NULL, CParamList* pEventData = NULL );

	static int SetEqpCtrlMode( int nCtrlMode );
	static BOOL GetEqpCtrlMode();

	int GetSeqCmdStatus(int nEqpCtrlCmdType=eEqpCtrlMode_Auto);
	CString GetActiveSeqCmdName( int nEqpCtrlMode = eEqpCtrlMode_Auto);
	int GetSeqWorkCmdStage(int nEqpCtrlMode = eEqpCtrlMode_Auto);

protected:
	// 단위 명령 처리
	void ThreadProc1()=0;
	void SeqCmdRunning();
	int  AddSeqCmd(CBaseSeqCmd* pCmd,int nEqpCtrlCmdType);
	CSeqCmdList m_SeqCmdListAuto;
	CSeqCmdList m_SeqCmdListManual;

public:
	CLogger m_Logger;
	int     m_nLastWorkingLoc;

	virtual void		MakeVatLog(LPCTSTR fmt, ...) {};
public:
	virtual int OnCommandRsp( CBaseSeqCmd * pRspCmd, int nEventId, WPARAM wParam, LPARAM lParam, std::vector<CPoint>& vPocket_Picker = std::vector<CPoint>() );
	// Task간의 통신을 위한 Observer List 관리
	static int AttachObserver(CBaseTaskManager* pObserver);
	static int DetachObserver(CBaseTaskManager* pObserver);
	static void ClearObserver();
	static int GetObserverCount();
	static CBaseTaskManager* GetTaskByIndex(int nIndex);
	static BOOL CheckDuplicate(CBaseTaskManager* pTask);

	static CCriticalSection								m_csObserver;
	static CList<CBaseTaskManager*, CBaseTaskManager*>	m_listObserver;
	static BOOL m_bEqpControlMode;
};
