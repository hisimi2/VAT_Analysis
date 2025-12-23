#pragma once
class CTaskAutoTrayFeeder : public CBaseTaskManager, public CBaseAutoSeqManager
{
	enum {
		eTHREAD_1 = 0,		// SeqCmd running
	};
// Function
public:
	CTaskAutoTrayFeeder(void);
	~CTaskAutoTrayFeeder(void);

	int Initialize();
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

private:
	int m_nFeederLastTrayRowStep;

	enum _eExcutingStep {
		eExcutingStep_Start = 0,

		eExcutingStep_CheckCondition,
		eExcutingStep_Pick,
		eExcutingStep_Place,
		eExcutingStep_Place_WaitEnd,
		eExcutingStep_SafetyPos,
		eExcutingStep_SafetyPos_WaitEnd,
		eExcutingStep_MoveLoadPos,
		eExcutingStep_MoveStep,
		eExcutingStep_MoveStep_WaitEnd,

		eExcutingStep_End,
	};

	enum _eCleanoutStep {
		eCleanoutStep_Start = 9000,
		eCleanoutStep_CheckCondition,
		eCleanoutStep_Place,
		eCleanoutStep_Wait_PlaceComp,
		eCleanoutStep_Move_Safety_pos,
		eCleanoutStep_End,
	};

	bool ChkRemainAutoSkipToDeviceAndAlarm();
	int  ChkRemainAutoSkipToDeviceCnt();

};

