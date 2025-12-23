#pragma once

#include "TesterIF/TesterIF.h"

class CBaseSeqCmd;

class CPressUnitCmd_OnLineLoopTest : public CBaseSeqCmd
{
public:
	CPressUnitCmd_OnLineLoopTest(void);
	~CPressUnitCmd_OnLineLoopTest(void);

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
	int    m_nLoopCount;
	double m_tcSotDelayTime;
	int m_nTesterIndex;
	BOOL m_bDutUseNotUse[STATION_MAX_PARA];
	ST_TESTER_ACTION_PARAM m_stActionParam[STATION_MAX_PARA];
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

	double m_tcTimeout;			// SOT-EOT Time
	double m_tcMaxTestTimeout;  // user set over time
	double m_tcTesterTestTime;	// check test time over

	int m_nRealTestCnt;
};

