#pragma once
class CBaseSeqCmd;

class CTrayFeederCmd_Initial : public CBaseSeqCmd
{
public:
	CTrayFeederCmd_Initial( void );
	~CTrayFeederCmd_Initial( void );

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

private:
	CTimerTickCheck	m_swTimeout;
	int m_nOrgRetryCount;
	enum step {
		start,

		org_set_clear,
		org_motor,
		conv_tray_align_cyl_work, 

		end = 99999,
	};
	CString act_string(/*step*/int act);
};

