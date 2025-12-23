#pragma once
class CVatTrayLoadPPCmd_MovePos : public CBaseSeqCmd
{
public:
	CVatTrayLoadPPCmd_MovePos(int nMode);
	~CVatTrayLoadPPCmd_MovePos();

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
	enum _eTrayLoadPp_ExecuteStep
	{
		move_start = 0,

		z_move_safety,
		z_move_safety_chk,
		x_px_py_move_target_pos,
		x_px_py_move_target_pos_chk,
		z_move_target_pos,
		z_move_target_pos_chk,

		move_end,
	};

public:
	//값 받아 와야 함.
	//{
	int m_nMode;

	CString				m_strCmdStage; //Log관련.

	std::vector<CPoint> m_vCmdPicker;

	//Mode에 따라 Base Position위치 다르며, XY Pitch도 Mode에 따라 다름.
	double m_dCmdPos[VAT_MAX_AXIS];
	//}
	
	BOOL m_bZOnlyMove;

	//정의된 Base 값
	CPoint              m_cBasePocket;
	std::vector<CPoint> m_vBasePicker; //8ea
	std::vector<CPoint> m_vErrPicker;

public:
	CTimerTickCheck     m_tcTimeout;
	

	BOOL IsParamExist();

	CString act_string(int act);	
};

