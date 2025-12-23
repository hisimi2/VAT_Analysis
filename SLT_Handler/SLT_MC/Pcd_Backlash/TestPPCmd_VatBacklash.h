#pragma once
#include "DlgPcdBacklash.h"
class CBaseSeqCmd;
class CTestPPCmd_VatBacklash :public CBaseSeqCmd
{
public:
	CTestPPCmd_VatBacklash(int nTestPpIdx, CDlgPcdBacklash *pParent);
	~CTestPPCmd_VatBacklash(void);

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

public:
	// Command Parameters
	BOOL m_bBacklashTest;
	std::deque<int> m_qSpeed;
	int m_nLoopCnt;
	int m_nDistanceCnt;
	int m_nAxisNum;

	std::vector<int> vStationNo;
	CPoint			 m_cCmdPocket;
	std::vector<CPoint> m_vCmdPicker;
	std::vector<CPoint> m_vSafetyAllPicker;
	std::vector<CPoint> m_vErrPicker;
	int              m_nHandType;	    // 0=LoadHand, 1=UnlondHand

	BOOL m_bRepeat;
	int m_nRepeatCnt;
	int m_nRepeatTempCnt;

private:
	CDlgPcdBacklash* m_pParentWnd;
	// local step control
	CTimerTickCheck m_tcTimeout;
	CTimerTickCheck m_tcTimeoutVsn;

	CPointF m_StartPos;
	CPointF m_TargetPos;
	CPointF m_EndPos;
	double  m_dLowerSafety;
	double  m_dUpperSafety;

	std::vector<CPointF> m_vStartToTargetOffset;
	std::vector<CPointF> m_vEndToTargetOffset;
	std::vector<CPointF> m_vBacklashAdjust;

	int m_nSpeed;
	int m_nChkLoopCnt;
	int m_nChkDistanceCnt;

	enum execute_step_define
	{
		exe_start = 0,
		exe_hand_all_pkr_up=1000,
		exe_hand_all_pkr_up_check, 

		exe_backlash_loop_entry,
		/* hand_confict_check - begin */
		exe_hand_confict_check,

		exe_hand_anti_conflict_z_move,
		exe_hand_anti_conflict_z_move_check,

		exe_hand_anti_conflict_x_home,
		exe_hand_anti_conflict_x_home_check,

		exe_hand_z_move_safety_down_position,
		exe_hand_z_move_safety_down_position_check,

		exe_hand_xy_pitch_move_safety_down_position,
		exe_hand_xy_pitch_move_safety_down_position_check,

		exe_hand_y_move_start_position,
		exe_hand_y_move_start_position_check,

		exe_hand_xy_move_start_position,
		exe_hand_xy_move_start_position_check,

		exe_hand_xy_move_start_to_target_position,
		exe_hand_xy_move_start_to_target_position_check,
		exe_hand_z_move_start_to_target_position,
		exe_hand_z_move_start_to_target_position_check,

		exe_visn_recon_start_to_target_pos,
		exe_visn_recon_start_to_target_pos_check,

		exe_hand_z_start_to_target_safety_move,
		exe_hand_z_start_to_target_safety_move_check,

		exe_hand_xy_move_end_position,
		exe_hand_xy_move_end_position_check,
		exe_hand_z_end_safety_move,
		exe_hand_z_end_safety_move_check,
		exe_hand_z_end_safety_picker_check,

		exe_hand_xy_move_end_to_target_position,
		exe_hand_xy_move_end_to_target_position_check,
		exe_hand_z_move_end_to_target_position,
		exe_hand_z_move_end_to_target_position_check,

		exe_visn_recon_end_to_target_pos,
		exe_visn_recon_end_to_target_pos_check,

		exe_hand_z_end_to_target_safety_move,
		exe_hand_z_end_to_target_safety_move_check,

		exe_final_x_safe_move,
		exe_final_x_safe_move_check,


		exe_complete=9000,
	};
	
	int              m_nRetryCount;
	int				 m_nTestPpIdx;
	BOOL			 m_bIsContinue;
	CTime			 m_cTime;

	inline void NextStep( execute_step_define next );
	inline void NextStepTimer( execute_step_define next , double tm);
	inline void SimpleAlarm( int alarm_id );
	BOOL CheckMeasureResult( ST_PCD_BACKLASH_RCV_DATA& result );

	void WriteRepeatOffsetXY(std::vector<CPointF> End2TargetOfs, std::vector<CPointF> Start2TargetOfs, int spd);

	void CalcBacklashOffset(int nSpeed, BOOL bIsBacklashTest,int HandType);
	void InitLoopStatus();
	CString GetBacklashAxisInfo(int nAxis, int nSpdIndex, double dOffset);

	CLogger m_BacklashXTestLog;
	CLogger m_BacklashYTestLog;

	CPointF m_StartOffset;
	CPointF m_EndOffset;
};
