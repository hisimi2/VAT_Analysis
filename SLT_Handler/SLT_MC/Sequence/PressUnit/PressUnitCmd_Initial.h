#pragma once

class CBaseSeqCmd;

class CPressUnitCmd_Initial :	public CBaseSeqCmd
{
public:
	CPressUnitCmd_Initial(void);
	~CPressUnitCmd_Initial(void);

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

	CTimerTickCheck m_tcTimeout;
	int         m_nOrgRetryCount;
};

