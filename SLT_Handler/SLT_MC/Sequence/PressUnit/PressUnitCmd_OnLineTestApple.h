#pragma once

class CBaseSeqCmd;

class CPressUnitCmd_OnLineTestApple : public CBaseSeqCmd
{
public:
	CPressUnitCmd_OnLineTestApple(void);
	~CPressUnitCmd_OnLineTestApple(void);

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

	

    int    m_nCmdStage;
	double m_tcTimeout;			// SOT-EOT Time
	double m_tcMaxTestTimeout;  // user set over time
	double m_tcSotDelayTime;
    double m_tcTesterTestTime;	// check test time over

	double m_tcNotTestingTimeout;  // user set over time
	double m_tcTesterNotTestingTime;	// check test time over
private:
	int m_nUnitIdx;
    SYSTEMTIME m_sysTmTestStart;
    BOOL m_bSotErr;         // don't send SOT flag
    BOOL m_bWaitResultErr;  // Wait Result but not answer tester flag
    BOOL m_bNotRevData;     // Send to sot & tester start signal, don't recieve data flag
    BOOL m_b2dMisMatch;
	BOOL m_bTesterErr;
    void FlagReset();

	int  m_nSiteCntX;
	int  m_nSiteCntY;
	int m_nBinSortType;

	int m_nHE_CategoryIdx;
	BOOL m_bSocketDvc[STATION_MAX_X_SITE][STATION_MAX_Y_SITE]; 	//Socket Device Data Flag
	ST_DD_DEVICE m_stDvcData[STATION_MAX_X_SITE][STATION_MAX_Y_SITE];

};

