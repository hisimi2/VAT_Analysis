#pragma once
class CVatLoadTblCmd_MovePos : public CBaseSeqCmd
{
public:
	CVatLoadTblCmd_MovePos(int nMode);
	~CVatLoadTblCmd_MovePos();

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
	enum _eLoadTbl_ExecuteStep
	{
		move_start = 0,

		
		target_site_chk_before_move,

		move_table_y_prepos,
		move_table_y_prepos_chk,

		move_table_rotate_target_pos,
		move_table_rotate_target_pos_chk,

		move_table_y_target_pos,
		move_table_y_target_pos_chk,

		move_end,
	};

public:
	int m_nMode;
	int m_nTableIdx;

	double m_dCmdPos[VAT_MAX_AXIS_YROTATE];

	CString				m_strCmdStage; //Log°ü·Ã.
public:
	CTimerTickCheck     m_tcTimeout;

	CString act_string(int act);
};

