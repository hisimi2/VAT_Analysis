#pragma once


//==============================================================================
//
//==============================================================================
#include <afxtempl.h>
#include <afxmt.h>

 
//==============================================================================
//
//==============================================================================
class CSeqCmdList
{
public:
	CSeqCmdList(void);
	~CSeqCmdList(void);

	void SetProcessCommand(base_seq_cmd::eProcessCommand a_eProcessCommand);
	base_seq_cmd::eCmdExecutorStatus GetCmdExecutorStatus(){ return m_eCmdExcutorStatus; };
	int GetSeqCmdStatus();
	CString GetActiveSeqCmdName();
	int GetSeqCmdWorkCmdStage();

	void Run();

	int  AddSeqCmd(CBaseSeqCmd* pCmd);
	int  GetSeqCmd();
	void DispatchSeqCmd();
	void DelCmd( CBaseSeqCmd* pCmd);
	void ClearCmd();

protected:
	CBaseSeqCmd*										m_pActiveCmd;
	CCriticalSection									m_csSeqCmd;
	CList<CBaseSeqCmd*,CBaseSeqCmd*>					m_SeqCmdQ;
	base_seq_cmd::eCmdExecutorStatus					m_eCmdExcutorStatus;
};

