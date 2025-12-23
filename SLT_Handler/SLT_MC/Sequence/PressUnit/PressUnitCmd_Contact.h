#pragma once

class CBaseSeqCmd;

class CPressUnitCmd_Contact :	public CBaseSeqCmd
{
public:
	CPressUnitCmd_Contact(void);
	~CPressUnitCmd_Contact(void);

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

	double		m_dInPosRange[CTaskPressUnit::eMaxAxisCount];
	double      m_dLastPos[CTaskPressUnit::eMaxAxisCount];
	double      m_dLastPosErrRange[CTaskPressUnit::eMaxAxisCount];

	int m_nCmdStage;
	int m_nRetryCount;
	int m_nChkExist;	// need exist checking

	int m_nCmdSafetyStage;

	CTimerTickCheck m_tcTimeout;
	CTimerTickCheck m_tcAntiVibratorTimeout;

	CPerformanceTime tmr;

	CTimerTickCheck m_tcBlowTimeout;

	CPerformanceTime m_cPertm;

	int m_nVibCurTryCnt;
	int	m_nBackVibratorStep;

	int m_nReContactCnt;

	BOOL        m_bBlowOff;

	enum eVibratorStep
	{
		do_Vibrator_On = 25000,
		do_Vibrator_On_Wait,
		do_Vibrator_Off,
		do_Vibrator_Off_Wait,
		Vibrator_complete,
	};



private:
	int m_nUnitIdx;
	_tVisionMsg m_tVisionMsgMeasure;
	_tVisionMsg m_tVisionMsgMeasureResult;

	int m_nSiteDivX;
	int m_nSiteDivY;
	int    m_nTorqueAxisNo;

	BOOL m_bVisionErrSkip[STATION_MAX_X_SITE][STATION_MAX_Y_SITE];
};

