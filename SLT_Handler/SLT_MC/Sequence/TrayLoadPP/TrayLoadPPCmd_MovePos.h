#pragma once
class CBaseSeqCmd;

class CTrayLoadPPCmd_MovePos : public CBaseSeqCmd
{
public:
	CTrayLoadPPCmd_MovePos( void );
	~CTrayLoadPPCmd_MovePos( void );

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


	eLocIdxTrayPP           eStage;
	CPoint                  Pocket;
	std::vector<CPoint>     vPicker;

private:
	CTimerTickCheck         m_tcTimeout;
	CTimerTickCheck         m_tcTimeoutPicker;
	int                     m_nRetryCount;

	std::vector<CPoint> m_vErrPicker;

	enum step {
		start,

		z_move_safety,
		z_move_safety_chk,
		chk_z_safety_sen_all,
		x_px_py_move,
		x_px_py_move_chk,


		end = 99999,
	};

	CString act_string(/*step*/int act );
};

