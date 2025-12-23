#pragma once

class CStackerCmd_TrayUp : public CBaseSeqCmd
{	
public:
	CStackerCmd_TrayUp(bool bConveyorStop = true, bool bManual = false);
	~CStackerCmd_TrayUp();

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
	bool m_bConveyorStop;	// Main pusher up하고 컨베이어 stop을 하도록 할 것인지
	bool m_bManual;

	int m_nDeviceMode;
};







