#pragma once

class CBaseSeqCmd;

class CTestPPCmd_Initial : public CBaseSeqCmd
{
public:
	CTestPPCmd_Initial(void);
	~CTestPPCmd_Initial(void);

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

	CTimerTickCheck m_tcTimeout;
	int         m_nRetryCount;
	int         m_nOrgRetryCount;
	int			m_nHandType;
private:
	std::vector<CPoint> m_vErrPicker;
};

