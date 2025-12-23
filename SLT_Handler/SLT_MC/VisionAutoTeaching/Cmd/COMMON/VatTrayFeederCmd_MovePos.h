#pragma once
class CVatTrayFeederCmd_MovePos : public CBaseSeqCmd
{
public:
	CVatTrayFeederCmd_MovePos(int nMode);
	~CVatTrayFeederCmd_MovePos();

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

	int m_nMode;

	double m_dCmdpos; //Y Axis
	//double m_dCmdOffset;
private:
	enum _eTrayFeeder_ExecuteStep
	{
		move_start = 0,

		feeder_cyl_up_chk,

		move_feeder_y,
		move_feeder_y_chk,
		
		move_end,
	};

	CTimerTickCheck	m_swTimeout;
	CString act_string(/*step*/int act);
};

