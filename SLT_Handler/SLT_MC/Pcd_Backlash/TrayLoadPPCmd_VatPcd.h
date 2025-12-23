#pragma once

class CBaseSeqCmd;

class CTrayLoadPPCmd_VatPcd :public CBaseSeqCmd
{
public:
	CTrayLoadPPCmd_VatPcd(void);
	~CTrayLoadPPCmd_VatPcd(void);

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

	int                 m_nRetryCount;

public:
	// Command Parameters
	// 제어상수
	CPoint              Pocket;
	std::vector<CPoint> vPicker;
	std::vector<CPoint> m_vSafetyAllPicker;

	int m_nAxisNum;
	int m_nDistance;
private:
	// local step control

	CPointF m_VsnOfs;
	CPointF m_StartPos_Offset;
	CPointF m_TargetPos_Offset;

	CPointF m_StartPos;
	CPointF m_TargetPos;

	CTimerTickCheck m_tcTimeout;
	CTimerTickCheck m_tcTimeoutVsn;
	CTimerTickCheck         m_tcTimeoutPicker;

	int m_nChkCount;
	double m_dTrayPP_Z_Safety;

	CPointF m_Cmd_PassRangeUm;

	std::vector<CPoint> m_vErrPicker;

	enum execute_step_define
	{
		exe_start = 0,
		exe_pcd_loop_entry=999,

		exe_hand_z_safety_move=1000,
		exe_hand_z_safety_move_check,

		exe_hand_xy_pitch_safety_move,
		exe_hand_xy_pitch_safety_move_check,

		exe_hand_xy_start_position_move_100mm=2000,
		exe_hand_xy_start_position_move_100mm_check,

		exe_hand_xy_start_position_move,
		exe_hand_xy_start_position_move_check,

		exe_hand_z_start_position_move,
		exe_hand_z_start_position_move_check,

		exe_visn_recon_start_position,
		exe_visn_recon_start_position_check,

		exe_hand_z_start_position_safety_move,
		exe_hand_z_start_position_safety_move_check,
		exe_hand_z_start_position_safety_picker_check,

		exe_hand_xy_target_position_move_100mm,
		exe_hand_xy_target_position_move_100mm_check,

		exe_hand_xy_target_position_move,
		exe_hand_xy_target_position_move_check,

		exe_hand_z_target_position_move,
		exe_hand_z_target_position_move_check,

		exe_visn_recon_target_position,
		exe_visn_recon_target_position_check,

		exe_final_z_safety_move,
		exe_final_z_safety_move_check,

		exe_complete=9000,
	};
	inline void NextStep( execute_step_define next );
	inline void NextStepTimer( execute_step_define next , double tm);
	inline void SimpleAlarm( int alarm_id );
	BOOL CheckMeasureResult( ST_PCD_BACKLASH_RCV_DATA& result );
};
