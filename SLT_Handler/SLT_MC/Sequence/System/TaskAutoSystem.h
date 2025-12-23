#pragma once

class CTaskAutoSystem : public CBaseTaskManager, public CBaseAutoSeqManager
{
	enum {
		eTHREAD_1 = 0,		// SeqCmd running
	};
// Function
public:
	CTaskAutoSystem(void);
	virtual ~CTaskAutoSystem(void);

	int Initialize( TCHAR * szDriverFileName );
	void Finalize();

	int OnEvent(const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList=NULL);
	int OnCommandRsp( CBaseSeqCmd * pRspCmd, int nEventId, WPARAM wParam, LPARAM lParam, std::vector<CPoint>& vPocket_Picker = std::vector<CPoint>() );
	void OnOperatorCommand( int nOperatorCommand, WPARAM wParam, LPARAM lParam );

	void ThreadProc1();

	void MakeLog( LPCTSTR fmt, ... );

	//Auto Run Mode
	void AutoState_Idle();
	void AutoState_InitNeed();
	void AutoState_Initialize();
	void AutoState_Pause();
	void AutoState_Execute();
	void AutoState_NormalComplete();
	void AutoState_AlarmStop();
	void AutoState_Resume();
	void AutoState_CleanOut();

// Variable
private:
	std::vector< CBaseAutoSeqManager* > m_vAllAutoTasks;

	enum _eExcutingStep {
		eExcutingStep_Start = 0,
		eExcutingStep_Command,
		eExcutingStep_Wait,
		eExcutingStep_Wait_User_LOTEND_Confirm,
		eExcutingStep_End,
	};

	
};

