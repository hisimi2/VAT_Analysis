#pragma once
class CBaseSeqCmd;

class CTrayFeederCmd_MovePickPlace : public CBaseSeqCmd
{
public:
	CTrayFeederCmd_MovePickPlace( void );
	~CTrayFeederCmd_MovePickPlace( void );

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

	eLocIdxFeeder	m_nCmdStage;
	ePPCmd			m_nCmdPickPlace;

private:
	CTimerTickCheck	m_swTimeout;
	int m_nLast_Do_Step;
	int m_nAlignRetry;
	BOOL m_bDo_FeederUp;

	enum step {
		start,

		do_chk_up_cyl_start,

		do_chk_Interlock_before_move,
		do_move,
		chk_move,
		do_chk_down_cyl,

		// pick
		pick_unclamp,
		pick_before_conv_align_back,
		pick_conv_align,
		pick_clamp,
		pick_after_conv_align_back,
		pick_do_chk_up_cyl_start,
		pick_align,
		pick_align_back,
	
	
		// place
		place_dealign,
		place_down_cyl,
		place_unclamp,

		do_chk_up_cyl_end,

		transfer_area_sensor_chk,  	// Transfer Area Place 하고 나서 Data 넘기기 전에 Transfer Area Sensor 감지 되고 있는지 확인 해준다.

		check_tray,
		
		end = 99999,
	};
	CString act_string(/*step*/int act );
};

