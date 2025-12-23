#pragma once

class CStackerCmd_TrayDn : public CBaseSeqCmd
{	
public:
	CStackerCmd_TrayDn();
	~CStackerCmd_TrayDn();

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
	int m_nDeviceMode;
};







