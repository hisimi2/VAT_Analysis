#pragma once

class CBaseAutoSeqManager
{	

public:
	CBaseAutoSeqManager();
	~CBaseAutoSeqManager();

	virtual int OnLotCommand( eAutoLotCmd eLotCmd, WPARAM wParam = NULL, LPARAM lParam = NULL );
		// 현재 AutoSeq의 Lot State 를 반환한다.
	virtual eAutoLotState	GetCurrentLotState();
	CString GetLotStateToString( eAutoLotState nState );
	CString GetLotStateToString();

	//Auto Run Mode
	virtual void AutoState_Idle()=0;
	virtual void AutoState_InitNeed()=0;
	virtual void AutoState_Initialize()=0;
	virtual void AutoState_Pause()=0;
	virtual void AutoState_Execute()=0;
	virtual void AutoState_NormalComplete()=0;
	virtual void AutoState_AlarmStop()=0;
	virtual void AutoState_Resume()=0;
	virtual void AutoState_CleanOut()=0;


	//Step function
	void ChangeMainState( eAutoLotState nAutoRunMainStep, int nAutoRunSubStep = 0 );
	void ChangeSubState( int nAutoRunSubStep );
protected:
	int m_nLast_Excuting_Step;

	//CString GetLotCmdToString( eAutoLotCmd eLotCmd );

	// current Step
	eAutoLotState m_nAutoRunMainStep;
	int m_nAutoRunSubStep;
	// back up step
	eAutoLotState m_nAutoRunBackUpMainStep;
	int m_nAutoRunBackUpSubStep;

	// AutoRunSubState
	CString GetExcuteStateStr();	
	int	    GetExcuteState();

	// running
	void    AutoRunning();

	void ChangeMainStateByRspAlarm();

	CMap<int, int, CString, CString> m_mapExcuteState;

	CCriticalSection   m_csAutoSeqCmdStatus;
};

