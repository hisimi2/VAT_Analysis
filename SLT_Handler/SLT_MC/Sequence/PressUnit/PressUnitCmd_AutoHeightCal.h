#pragma once

class CBaseSeqCmd;

class CPressUnitCmd_AutoHeightCal :public CBaseSeqCmd
{
public:
	CPressUnitCmd_AutoHeightCal(void);
	~CPressUnitCmd_AutoHeightCal(void);

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

	int             m_nCmdStage;
	CTimerTickCheck m_tcTimeout;
	int             m_nRetryCount;
private:
	int    m_nUnitIdx;
	double m_dLimitTorque;       // max volt
	double m_dContactDownDist; // contact force checking position
	int    m_nTorqueAxisNo;
	double m_dFindContactPos;
	double m_dPressDownOffset;
};

