#pragma once

typedef std::vector<std::pair<double, int>> TestTime;

class CTaskAutoLoadTable  : public CBaseTaskManager, public CBaseAutoSeqManager
{
public:
	CTaskAutoLoadTable(void);
	~CTaskAutoLoadTable(void);

	int Initialize( TCHAR * szDriverFileName, _eLoadTblIdx eTblIndex);
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

	int FindJobState(int &nLocJobIdx);
	int GrrFindJobState(int &nLocJobIdx);

	int ChkMoveLoadSite();
	int ChkMoveTestSite();
	BOOL UseAbleCheck();

	int FindStation_CompSOT();
	int FindStation_CompEOT();

	int FindStation_CompTime( int nTimeType);
	int CompSystemTime(SYSTEMTIME SysTime1, SYSTEMTIME SysTime2);

	BOOL EnableMoveTimeIdx();
private:
	enum _eExcutingStep {
		eExcutingStep_Start = 0,

		eExcutingStep_Check_Job,

		eExcutingStep_Check_Work_Enable_LdSide,
		eExcutingStep_Move_LdSide,
		eExcutingStep_WaitEnd_LdSide,

		eExcutingStep_Check_Work_Enable_TestSide,
		eExcutingStep_Move_TestSide,
		eExcutingStep_WaitEnd_TestSide,

		eExcutingStep_End,

		eExcutingStep_ErrorJob = -1,
	};

	enum _eJobIndex {
		eJobIndex_None = -1,
		eJobIndex_LoadSite = 0,
		eJobIndex_TestSite1,
		eJobIndex_TestSite2,
		eJobIndex_TestSite3,
		eJobIndex_TestSite4,
	};

	enum _eTimeType {
		eTime_SOT = 0,
		eTime_EOT
	};

public:
	int				m_nLocJobIdx;
	int				m_nTablePos[eMaxLoadTblCount];

	_eLoadTblIdx    m_eTblIndex;
	_eTestPPIdx     m_eTestPPIdx;


	BOOL			m_bChkTime;
	CTimerTickCheck m_tcTimeout;
	
	int				m_nPreContactIdx;

};

