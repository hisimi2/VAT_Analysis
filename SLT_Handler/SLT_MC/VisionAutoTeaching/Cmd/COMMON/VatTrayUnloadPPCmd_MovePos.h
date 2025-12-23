#pragma once
class CVatTrayUnloadPPCmd_MovePos: public CBaseSeqCmd
{
public:
	CVatTrayUnloadPPCmd_MovePos(int nMode);
	~CVatTrayUnloadPPCmd_MovePos();

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
		x_y_px_py_move_target_pos,
		x_y_px_py_move_target_pos_chk,
		z_move_target_pos,
		z_move_target_pos_chk,

		move_end,
	};

public:
	//값 받아와야 함.
	int m_nMode;

	CString				m_strCmdStage; //Log관련.

	double m_dCmdPos[VAT_MAX_AXIS];

 	std::vector<CPoint> m_vCmdPicker;
 	
	BOOL m_bZOnlyMove;

	//정의된 Base 값
	CPoint              m_cBasePocket;
	std::vector<CPoint> m_vBasePicker; //8ea
	std::vector<CPoint> m_vErrPicker;
	
public:
	CTimerTickCheck     m_tcTimeout;
	CTimerTickCheck	    m_TransferInterlock;

	//BOOL CheckCmdParam(std::vector<CPoint> vCmdPicker, double dCmdPosX, double dCmdPosY, double dCmdPosZ, double dCmdPosXP, double dCmdPosYP);
	BOOL IsParamExist();

	CString act_string(int act);
};

