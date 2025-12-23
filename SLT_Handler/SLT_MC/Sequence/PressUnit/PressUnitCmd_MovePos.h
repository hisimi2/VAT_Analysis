#pragma once
#include "..\Dll_Src\LIB_IPC_TCP\LIB_IPC_SOCK\PerformanceTime.h"
class CBaseSeqCmd;

class CPressUnitCmd_MovePos :public CBaseSeqCmd
{
public:
	CPressUnitCmd_MovePos(void);
	~CPressUnitCmd_MovePos(void);

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

	int         m_nCmdStage;
	CTimerTickCheck m_tcTimeout;
	CTimerTickCheck m_tcBlowTimeout;
	CTimerTickCheck m_tcAntiVibratorTimeout;

	CPerformanceTime m_cPertm;

	int         m_nRetryCount;
	BOOL        m_bBlowOff;

	int m_nVibCurTryCnt;

	int	m_nBackVibratorStep;

	enum eVibratorStep
	{
		do_Vibrator_On = 25000,
		do_Vibrator_On_Wait,
		do_Vibrator_Off,
		do_Vibrator_Off_Wait,
		Vibrator_complete,
	};

	CPerformanceTime tmr;
private:
	int m_nUnitIdx;
	_tVisionMsg m_tVisionMsgMeasure;
	_tVisionMsg m_tVisionMsgMeasureResult;
	
	int m_nSiteDivX;
	int m_nSiteDivY;
	BOOL m_bVisionErrSkip[STATION_MAX_X_SITE][STATION_MAX_Y_SITE];
};

