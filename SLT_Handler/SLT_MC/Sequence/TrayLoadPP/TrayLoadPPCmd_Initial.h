#pragma once
class CBaseSeqCmd;

class CTrayLoadPPCmd_Initial : public CBaseSeqCmd
{
public:
	CTrayLoadPPCmd_Initial( void );
	~CTrayLoadPPCmd_Initial( void );

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
	CTimerTickCheck m_tcTimeout;
	int             m_nRetryCount;
	int             m_nOrgRetryCount;

	std::vector<CPoint> m_vErrPicker;
	std::vector<CPoint> m_vErrSafetyPicker;

	enum step {
		start,

		z_clear,
		z_org,
		z_move_safety,
		z_move_safety_chk,
		pitch_clear,
		pitch_org,
		x_clear,
		x_org,
		org_complete,


		end = 99999,
	};

	CString act_string(/*step*/int act );
};

