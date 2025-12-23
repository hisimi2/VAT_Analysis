#pragma once

class CStackerCmd_Vibration : public CBaseSeqCmd
{	
public:
	CStackerCmd_Vibration( double _act_delay_time_sec, int _act_times );
	~CStackerCmd_Vibration();

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

	double m_act_delay_time_sec;
	int m_act_times;

	enum eExcutingStep {
		eExcutingStep_Start = 0,

		eExcutingStep_Forward_Tray_Align_Cylinder,

		eExcutingStep_Do_Vibration,

		eExcutingStep_End,
	};
};







