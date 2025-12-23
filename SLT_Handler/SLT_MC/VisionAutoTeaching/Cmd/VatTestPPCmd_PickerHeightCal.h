#pragma once
class CVatTestPPCmd_PickerHeightCal : public CBaseSeqCmd
{
public:
	CVatTestPPCmd_PickerHeightCal();
	~CVatTestPPCmd_PickerHeightCal();

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
	enum _eHeightCalStep
	{
		height_cal_start = 0,

//		height_cal_move_z_safety,
//		height_cal_move_z_safety_chk,
// 		//height_cal_chk_z_safety_sen_all,
// 
// 		height_cal_move_x1_x2_safety,
// 		height_cal_move_x1_x2_safety_chk,
// 
// 		height_cal_move_y_px_py_target_pos,
// 		height_cal_move_y_px_py_target_pos_chk,
// 
// 		height_cal_move_x_target_pos,
// 		height_cal_move_x_target_pos_chk,

		height_cal_all_vacuum_on,

		height_cal_picker_down_fast,
		height_cal_picker_down_slow,

		height_cal_picker_down_chk,
		height_cal_vacuum_chk,

		height_cal_save_fast_height_pos,
		height_cal_save_slow_height_pos_z_auto_focus,
		height_cal_save_slow_height_pos_z_auto_teaching,

		height_cal_all_picker_1mm_up,
		height_cal_all_picker_1mm_up_check,
		height_cal_vacuum_off,
		height_cal_all_picker_up,
		height_cal_all_picker_up_check,

		height_cal_end = 99999,
	};

public:
	CString act_string(int act);

public:
	//Cmd 생성 시, 값 받아 와야 함.
	int					m_nHand; //Test Pp Part는 Hand 4ea Auto에서 각각 관리. 
	int					m_nSpeed;
	double				move_offset_um;
	int					m_nStage;
	double				m_dTargetPosZ;
	int					m_nTestPpType;
	

	//Cmd 생성 시, Check Param에서 생성.
	std::vector<CPoint> m_vCmdPicker;
	std::vector<CPoint> m_vAllPicker;
	std::vector<int>	m_vBMAxis;

	int m_nChkAllVacuum;
	int					m_nTestPpIdx;



	CTimerTickCheck     m_tcTimeout;

	std::vector<CPoint> m_vErrPicker;
	BOOL m_bFindPos[VAT_PICKER_MAX];
	double m_dPickPos[VAT_PICKER_MAX];
};

