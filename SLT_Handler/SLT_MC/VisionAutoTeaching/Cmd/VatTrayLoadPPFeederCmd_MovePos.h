#pragma once
class CVatTrayLoadPPFeederCmd_MovePos : public CBaseSeqCmd
{
public:
	CVatTrayLoadPPFeederCmd_MovePos(int nMode);
	~CVatTrayLoadPPFeederCmd_MovePos();

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
	enum _eTrayLoadPp_Feeder_ExecuteStep
	{
		move_start = 0,

		z_move_safety,					//LD Tray PP Safety Chk
		z_move_safety_chk,
		feeder_cyl_abnormal_chk,		//Feeder abnormal Chk
		feeder_cyl_up_chk,				//Feeder Cylinder Up Chk

		x_y_px_py_move_target_pos,		// [LD Tray PP X, X-Pitch, Y-Pitch Move] [Feeder Y Move]
		x_y_px_py_move_target_pos_chk,

		z_move_target_pos,
		z_move_target_pos_chk,

		move_end,
	};

public:	 
	CString		m_strCmdStage; //Log 관련.
	int m_nMode;

	std::vector<CPoint> m_vCmdPicker;

	double m_dCmdPos[VAT_MAX_AXIS];

	//내부 정의된 Base 값
	CPoint				m_cBasePocket;
	std::vector<CPoint> m_vBasePicker; //8ea
	std::vector<CPoint> m_vErrPicker;

public:
	CTimerTickCheck m_tcTimeout;

	BOOL IsParamExist();

	CString act_string(int act);
};

