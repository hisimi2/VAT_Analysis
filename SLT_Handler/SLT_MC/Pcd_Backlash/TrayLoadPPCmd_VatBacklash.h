#pragma once
#include "DlgPcdBacklash.h"
class CBaseSeqCmd;

class CTrayLoadPPCmd_VatBacklash :public CBaseSeqCmd
{
public:
	CTrayLoadPPCmd_VatBacklash(CDlgPcdBacklash *pParent);
	~CTrayLoadPPCmd_VatBacklash(void);

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

	std::deque<int> m_qSpeed;
	int m_nLoopCnt;
	int m_nAxisNum;
	BOOL m_bBacklashTest;

	double m_dTrayPP_Z_Safety;

	BOOL m_bRepeat;
	int m_nRepeatCnt;
	int m_nRepeatTempCnt;

	//int m_nStandardPicker;

private:
	CDlgPcdBacklash* m_pParentWnd;
	CTime m_cTime;
	// local step control
	CTimerTickCheck m_tcTimeout;
	CTimerTickCheck m_tcTimeoutVsn;
	CTimerTickCheck m_tcTimeoutPicker;

	int m_nChkLoopCnt;
	int m_nSpeed;

	std::vector<CPointF> m_vStartToTargetOffset;
	std::vector<CPointF> m_vEndToTargetOffset;
	std::vector<CPointF> m_vBacklashAdjust;

	std::vector<CPoint> m_vErrPicker;

	enum execute_step_define
	{
		exe_start = 0,
		exe_backlash_loop_entry=999,

		exe_hand_z_safety_move=1000,
		exe_hand_z_safety_move_check,

		exe_hand_xy_pitch_safety_move,
		exe_hand_xy_pitch_safety_move_check,

		exe_hand_xy_move_target_position=2000,
		exe_hand_xy_move_target_position_check,

		exe_hand_z_move_target_safety_position,
		exe_hand_z_move_target_safety_position_check,

		exe_hand_xy_move_target_to_start_position,
		exe_hand_xy_move_target_to_start_position_check,

		exe_hand_xy_move_start_to_target_position,
		exe_hand_xy_move_start_to_target_position_check,

		exe_hand_z_move_start_to_target_position,
		exe_hand_z_move_start_to_target_position_check,
		
		exe_visn_recon_start_to_target_pos,
		exe_visn_recon_start_to_target_pos_check,
		
		exe_hand_z_start_to_target_safety_move,
		exe_hand_z_start_to_target_safety_move_check,

		exe_hand_xy_move_target_to_end_position,
		exe_hand_xy_move_target_to_end_position_check,

		exe_hand_xy_move_end_to_target_position,
		exe_hand_xy_move_end_to_target_position_check,

		exe_hand_z_move_end_to_target_position,
		exe_hand_z_move_end_to_target_position_check,

		exe_visn_recon_end_to_target_pos,
		exe_visn_recon_end_to_target_pos_check,

		exe_hand_z_end_to_target_safety_move,
		exe_hand_z_end_to_target_safety_move_check,

		exe_complete=9000,
	};
	inline void NextStep( execute_step_define next );
	inline void NextStepTimer( execute_step_define next , double tm);
	inline void SimpleAlarm( int alarm_id );
	BOOL CheckMeasureResult( ST_PCD_BACKLASH_RCV_DATA& result );
	void CalcBacklashOffset(int nSpeed, BOOL bIsBacklashTest);
	CString GetBacklashAxisInfo(int nAxis, int nSpd, double dOfs);
	void InitLoopStatus();
	void WriteRepeatOffsetXY(std::vector<CPointF> End2TargetOfs, std::vector<CPointF> Start2TargetOfs, int spd);



	void WriteMotorEncoder(int speed);
public:
	CLogger m_BacklashXTestLog;
	CLogger m_BacklashYTestLog;
};
