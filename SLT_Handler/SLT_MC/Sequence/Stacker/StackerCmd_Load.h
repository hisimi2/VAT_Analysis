#pragma once

class CStackerCmd_Load : public CBaseSeqCmd
{	
public:
	CStackerCmd_Load( bool bCheckSlowSensor = true, bool bManual = false );
	~CStackerCmd_Load();

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
	bool m_bManual;
	bool m_bCheckSlowSensor;

	enum eExcutingStep {
		eExcutingStep_Start = 0,

		eExcutingStep_Backward_Tray_Align_Cylinder,

		eExcutingStep_Move_Conveyor,
		eExcutingStep_Check_Rear_Slow_Sensor,
		eExcutingStep_Slow,
		eExcutingStep_Check_Rear_Stop_Sensor,

		eExcutingStep_Forward_Tray_Align_Cylinder,
		eExcutingStep_Stop,
		eExcutingStep_CheckTrayData,

		eExcutingStep_End,
	};

	int              m_nDeviceMode;
};







