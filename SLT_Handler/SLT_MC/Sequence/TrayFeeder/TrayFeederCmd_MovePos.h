#pragma once
class CBaseSeqCmd;

class CTrayFeederCmd_MovePos : public CBaseSeqCmd
{
public:
	CTrayFeederCmd_MovePos( void );
	~CTrayFeederCmd_MovePos( void );

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

	eLocIdxFeeder	m_nCmdStage;
	int             m_nTrayRowStep;

private:
	CTimerTickCheck	m_swTimeout;

	enum step {
		start,

		do_chk_up_cyl_start,

		do_move,
		chk_move,

		end = 99999,
	};
	CString act_string(/*step*/int act );
};

