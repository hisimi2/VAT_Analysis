#pragma once

class AFX_EXT_CLASS CTimerTickCheck
{
public:
	CTimerTickCheck(double dCheckTime=0.0, BOOL start_mode=FALSE);
	~CTimerTickCheck(void);

public:
	void ResetStartTime();
	double CompareTime(BOOL msec_opt=true);
	double GetCheckTime();
	void SetCheckTime(double dCheckTime);
	BOOL CheckOverTime();
	BOOL MoreThan(double dTime);
	BOOL LessThan(double dTime);
	void StartTimer();
	void SetTimerCheckEnd();
	BOOL IsChecking();

private:
	double m_dCheckTime;    // check 해야 할 시간.
	double m_dStartTime;    // check 시작 시간.
	double m_dEndTime;      // check 완료 시간.
	BOOL   m_bChecking;     // check 상태
};

