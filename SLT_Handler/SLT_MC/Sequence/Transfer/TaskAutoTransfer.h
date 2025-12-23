#pragma once

class CTaskAutoTransfer : public CBaseTaskManager, public CBaseAutoSeqManager
{
	enum {
		eTHREAD_1 = 0,		// SeqCmd running
	};
// Function
public:
	CTaskAutoTransfer(void);
	virtual ~CTaskAutoTransfer(void);

	int Initialize( TCHAR * szDriverFileName );
	void Finalize();

	int OnEvent(const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList=NULL);
	int OnCommandRsp( CBaseSeqCmd * pRspCmd, int nEventId, WPARAM wParam, LPARAM lParam, std::vector<CPoint>& vPocket_Picker = std::vector<CPoint>() );
	void OnOperatorCommand( int nOperatorCommand, WPARAM wParam, LPARAM lParam );

	void ThreadProc1();

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

	BOOL ChkStackerUnloadTransHolding();
// Variable
private:
	void MakeLog( LPCTSTR fmt, ... );

	CTimerTickCheck m_swTimeout;
	BOOL m_bStepChangeFalg;

	enum _eExcutingStep {
		eExcutingStep_Start = 0,

		eExcutingStep_Check_Work_Area_Enable,

		eExcutingStep_MovePick_Src_Conveyor_Pos,
		eExcutingStep_WaitEnd_MovePick_Src_Conveyor_Pos,
		eExcutingStep_MovePlace_Dest_Conveyor_Pos,
		eExcutingStep_WaitEnd_MovePlace_Dest_Conveyor_Pos,

		eExcutingStep_Move_SafetyPos,
		eExcutingStep_WaitEnd_Move_SafetyPos,

		eExcutingStep_WaitEnd_Move_UnloadEmptyPos,

		eExcutingStep_End,
	};


	bool GetScheduleExist();

	//bool ChkRemainAutoSkipToDeviceAndAlarm();
	
};

