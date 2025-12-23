#pragma once

class CStackerCmd_FullTray : public CBaseSeqCmd
{
public:
	CStackerCmd_FullTray();
	~CStackerCmd_FullTray();

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

private:
	int m_nLast_Do_Step;

	enum eExcutingStep {
		eExcutingStep_Start = 0,

		eExcutingStep_FullTrayCheck,

		eExcutingStep_End,
	};
};







