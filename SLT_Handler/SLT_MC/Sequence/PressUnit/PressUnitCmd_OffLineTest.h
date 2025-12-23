#pragma once

class CBaseSeqCmd;

class CPressUnitCmd_OffLineTest :public CBaseSeqCmd
{
public:
	CPressUnitCmd_OffLineTest(void);
	~CPressUnitCmd_OffLineTest(void);

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

	double dOffSetTime;

	int         m_nCmdStage;
	double      m_tcTimeout;
	double      m_tcCheckTime;
	int         m_nRetryCount;
	int         m_nUnitIdx; // RIDX_PRESS_UNIT_1, RIDX_PRESS_UNIT_2 .....

	std::vector<int>  m_numbers;
private:
	SYSTEMTIME m_sysTmTestStart;
	double      m_tcSotDelayTime;

	int m_nSiteDivX;
	int m_nSiteDivY;
	int m_nBinSortType;

	double m_tcMaxTestTimeout;  // user set over time

	bool m_bChkPassFail;


	BOOL m_bSocketDvc[STATION_MAX_X_SITE][STATION_MAX_Y_SITE]; 	//Socket Device Data Flag
	ST_DD_DEVICE m_stDvcData[STATION_MAX_X_SITE][STATION_MAX_Y_SITE];
};

