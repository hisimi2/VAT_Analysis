#pragma once

// Stacker에서 Initial은 사용하지 않음 (향후에 필요시 구현할 것)
class CStackerCmd_Initial : public CBaseSeqCmd
{	
public:
	CStackerCmd_Initial();
	~CStackerCmd_Initial();

	int CheckParam();

	int  OnStop();
	int  OnRetry();
	int  OnSkip();
	int  OnPause();
	int  OnResume();

	BOOL State_Setup();
	BOOL State_Executing();
	BOOL State_Pause();
	BOOL State_Recovery();
	BOOL State_NormalComplete();
	BOOL State_Alarm();
	BOOL State_AlarmComplete();
	BOOL State_Aborting();
	BOOL State_AbortComplete();

	CTimerTickCheck	m_swTimeout;
	int			m_nOrgRetryCount;
};





