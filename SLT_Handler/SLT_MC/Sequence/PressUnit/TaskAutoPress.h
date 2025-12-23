#pragma once

class CTaskAutoPress  : public CBaseTaskManager, public CBaseAutoSeqManager
{
public:
	CTaskAutoPress(void);
	~CTaskAutoPress(void);

	int Initialize( TCHAR * szDriverFileName, _ePressUnit ePressIndex);
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

	int FindJobState();
	void SetCurExecutingStep(int nStep);
	int GetCurExecutingStep();

public:
	int  m_nAutoCurExecutingStep;
private:
	enum _eExcutingStep {
		eExcutingStep_Start = 0,

		eExcutingStep_Check_Job,

		eExcutingStep_Move_SafetyPos,
		eExcutingStep_WaitEnd_SafetyPos,

		eExcutingStep_Check_Work_Area_Enable_Contact,
		eExcutingStep_Move_Contact,
		eExcutingStep_WaitEnd_Contact,
		eExcutingStep_TestStart,
		eExcutingStep_WaitTestEnd,
		eExcutingStep_Move_SafetyPos_TestEnd,
		eExcutingStep_WaitEnd_SafetyPos_TestEnd,

		eExcutingStep_Check_Work_Area_Enable_Cleaning,
		eExcutingStep_Move_Cleaning,
		eExcutingStep_WaitEnd_Cleaning,
		eExcutingStep_Move_SafetyPos_Cleaning,
		eExcutingStep_WaitEnd_SafetyPos_Cleaning,

		eExcutingStep_End,

		eExcutingStep_ErrorJob = -1,
	};

	_ePressUnit              m_ePressIndex;
	_eTestPPIdx              m_eTestPpIndex;
	eTaskAutoTestSiteAreaLoc m_eATSIdx;

	// 같은 모듈 내의 press unit의 index 들
	std::vector<int> m_vModuleSetPressIdx;

	void RecordDataOEE();
	BOOL UseAbleCheck();
};

