#pragma once
class CBaseSeqCmd;

class CTrayLoadPPCmd_MovePickPlace : public CBaseSeqCmd
{
public:
	CTrayLoadPPCmd_MovePickPlace( void );
	~CTrayLoadPPCmd_MovePickPlace( void );

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

	eLocIdxTrayPP       eStage;
	CPoint              Pocket;
	std::vector<CPoint> vPicker;
	std::vector<CPoint> vSpecOutPicker;
	std::vector<CPoint> vFullCmdPkr;
	ePPCmd              ePickPlace;	    // 0=Pick, 1=Place
	BOOL				bNormalPkrEnd;
	BOOL				bSpeckOutPkrEnd;
	int				    nCntSpecOut;
	BOOL				bSpecOutPkrUse;
	BOOL				bVacuumCheckFuction;
private:
	// 제어상수
	double           m_dWaitTime;        // msec
	BOOL             m_bDownCheckEnable; // 0:Up 상태에서 Vac 점검. 1:Down 상태에서 Vac 점검
	double           m_dDownCheckTimeout;

	double           m_tcTimeout;
	double           m_tcTimeoutPicker;
	int              m_nRetryCount;
	int              m_nSetRetryCount;

	double           m_dInPosRange[ CTaskTrayBasePP::eAxisMax ];

	std::vector<CPoint> m_vErrPicker;

	eCheckAbnormalCase check_abnormal_case( int _PickPlace, CString& error_msg );

	CTimerTickCheck	m_TransferInterlock;

	enum step {
		start,

		do_blow,

		z_move_safety,
		z_move_safety_chk,
		x_px_py_move,
		x_px_py_move_chk,
		waiting_for_load_table,
		waiting_for_feeder,
		move_z_target_pos,
		move_z_target_soft_Pick_1stage_done,
		move_z_target_soft_Pick_2stage,
		checking_z_axis_done,
		pick_vac,
		move_z_safety,
		check_z_axis_done,
		check_vacuum_fail,
		picker_up_check_and_write_device_data,
		picker_up_check_and_write_device_data_table_vacuum_check,

		end = 99999,
	};

	CString act_string(/*step*/int act );
};

