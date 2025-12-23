#pragma once
class CVatTestPPCmd_MovePos: public CBaseSeqCmd
{
public:
	CVatTestPPCmd_MovePos(int nMode);
	~CVatTestPPCmd_MovePos();

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
	enum _eTestPp_ExecuteStep
	{
		move_start = 0,

		z_move_safety,
		z_move_safety_chk,
		x1_x2_move_safety,
		x1_x2_move_safety_chk,
		y_px_py_move_target_pos,
		y_px_py_move_target_pos_chk,
		x_move_target_pos,
		x_move_target_pos_chk,
		z_move_target_pos,
		z_move_target_pos_chk,

		move_end,
	};

public:
	//값 받아와야 함.
	int m_nMode;

	int m_nTestPPIdx;
	int m_nTestPPType;

	double m_dCmdPos[VAT_MAX_AXIS];

	std::vector<CPoint> m_vCmdPicker;
	
	BOOL m_bZOnlyMove;

	CString				m_strCmdStage; //Log관련.

	//정의된 Base 값
	CPoint              m_cBasePocket;
	std::vector<CPoint> m_vBasePicker; //8ea
	std::vector<CPoint> m_vErrPicker;
	
public:
	CTimerTickCheck     m_tcTimeout;

	BOOL IsParamExist();

	CString act_string(int act);



};

