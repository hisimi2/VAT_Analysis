#pragma once

class CBaseSeqCmd;

class CPressUnitCmd_SocketCleanContact :public CBaseSeqCmd
{
public:
    CPressUnitCmd_SocketCleanContact(void);
    ~CPressUnitCmd_SocketCleanContact(void);

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

	enum eVibratorStep
	{
		do_Vibrator_On = 25000,
		do_Vibrator_On_Wait,
		do_Vibrator_Off,
		do_Vibrator_Off_Wait,
		Vibrator_complete,
	};

	CPerformanceTime tmr;
    int             m_nCmdStage;
    CTimerTickCheck m_tcTimeout;
    int             m_nRetryCount;

	int m_nVibCurTryCnt;
	int	m_nBackVibratorStep;

private:
	int m_nSetSocketCleanCount;
	_tVisionMsg m_tVisionMsgMeasure;
	_tVisionMsg m_tVisionMsgMeasureResult;
	
	int m_nUnitIdx;

	int m_nSiteDivX;
	int m_nSiteDivY;
	BOOL m_bVisionErrSkip[STATION_MAX_X_SITE][STATION_MAX_Y_SITE];
};

