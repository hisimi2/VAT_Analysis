#pragma once
class CVatTrayLoadPPTableCmd_MovePos : public CBaseSeqCmd
{
public:
	CVatTrayLoadPPTableCmd_MovePos(int nMode);
	~CVatTrayLoadPPTableCmd_MovePos();

	int CheckParam();

	int OnStop();
	int OnRetry();
	int OnSkip();
	int OnPause();
	int OnResume();

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
	enum _eTrayLoadPp_Table_ExecuteStep
	{
		move_start = 0,

		z_move_safety,					//LD Tray PP Safety Chk
		z_move_safety_chk,
		table_target_site_chk_before_move,

		move_table_y_prepos,
		move_table_y_prepos_chk,

		move_table_rotate_target_pos,
		move_table_rotate_target_pos_chk,

		x_y_px_py_move_target_pos,		// [LD Tray PP X, X-Pitch, Y-Pitch Move] [Table Y Move]
		x_y_px_py_move_target_pos_chk,

		z_move_target_pos,
		z_move_target_pos_chk,

		move_end,
	};

public:
	CString		m_strCmdStage; //Log 관련.

	int m_nMode;
	int m_nTableIdx;

	std::vector<CPoint> m_vCmdPicker;

	double m_dCmdPos_Hand[VAT_MAX_AXIS];
	double m_dCmdPos_Table[VAT_MAX_AXIS_YROTATE];

	//내부 정의된 Base 값
	CPoint				m_cBasePocket;
	std::vector<CPoint> m_vBasePicker; //8ea
	std::vector<CPoint> m_vErrPicker;

public:
	CTimerTickCheck m_tcTimeout;

	CString act_string(int act);
};

