#pragma once

class CBaseSeqCmd;
class CTestPPCmd_VatPcd :public CBaseSeqCmd
{
public:
	CTestPPCmd_VatPcd(int nTestPpIdx);
	~CTestPPCmd_VatPcd(void);

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
	CPointF m_PcdStart_Pos;
	double  m_dLowerSafety;
	double  m_dUpperSafety;

	// Command Parameters
	CPoint			 m_cCmdPocket;
	std::vector<CPoint> m_vCmdPicker;
	std::vector<CPoint> m_vSafetyAllPicker;
	int              m_nHandType;	    // 0=LoadHand, 1=UnlondHand

	int m_nDistance;
	int m_nAxisNum;

	std::vector<int> vStationNo;
private:
	CTimerTickCheck m_tcTimeout;
	CTimerTickCheck m_tcFineAdjTimeout;
	
	CPointF m_StartPos_Offset;
	CPointF m_TargetPos_Offset;

	CPointF m_Cmd_PassRangeUm;

	std::vector<CPoint> m_vErrPicker;

	enum execute_step_define
	{
		exe_start = 0,
		exe_hand_all_pkr_up=1000,
		exe_hand_all_pkr_up_check, 

		/* hand_confict_check - begin */
		exe_hand_safety_x_check,
		exe_hand_safety_x_move,
		exe_hand_safety_x_move_check,

		exe_hand_z_move_safety_down_position,
		exe_hand_z_move_safety_down_position_check,

		exe_hand_xy_pitch_move_safety_down_position,
		exe_hand_xy_pitch_move_safety_down_position_check,

		exe_hand_y_move_start_position,
		exe_hand_y_move_check_start_position,

		exe_hand_xy_move_start_position_before_100mm,
		exe_hand_xy_move_check_start_position_before_100mm,

		exe_hand_xy_move_start_position,
		exe_hand_xy_move_check_start_position,
		/* hand_confict_check - end */

		exe_hand_z_start_position_move,
		exe_hand_z_start_position_move_check,
		
		exe_visn_recon_start_position,
		exe_visn_recon_start_position_check,

		exe_hand_z_start_position_safety_move,
		exe_hand_z_start_position_safety_move_check,

		exe_hand_xy_move_target_position_before_100mm,
		exe_hand_xy_move_check_target_position_before_100mm,

		exe_hand_xy_move_target_position,
		exe_hand_xy_move_check_target_position,

		exe_hand_z_target_position_move,
		exe_hand_z_target_position_move_check,

		exe_visn_recon_target_position,
		exe_visn_recon_target_position_check,

		exe_final_z_safety_move,
		exe_final_z_safety_move_check,

		exe_final_x_safe_move,
		exe_final_x_safe_move_check,
		exe_complete=9000,

	};
	
	/* 축 간섭 회피용 멤버 변수 */
	int              m_nRetryCount;
	int				 m_nTestPpIdx;
	double           m_dLastPos[CTaskTestPP::eAxisMax];
	double           m_dLastPosErrRange[CTaskTestPP::eAxisMax];
	double           m_dInPosRange[CTaskTestPP::eAxisMax];
	CPointF          m_absPos;//결과 값

	inline void NextStep( execute_step_define next );
	inline void NextStepTimer( execute_step_define next , double tm);
	inline void SimpleAlarm( int alarm_id );
	BOOL CheckMeasureResult( ST_PCD_BACKLASH_RCV_DATA& result );
	void MakeVatPreparePos(int nIndex, double &dX, double &dY, double dPrepareOffset);
};
