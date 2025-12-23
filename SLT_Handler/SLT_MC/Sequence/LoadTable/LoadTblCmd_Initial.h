#pragma once
#include "..\Dll_Src\LIB_IPC_TCP\LIB_IPC_SOCK\PerformanceTime.h"

class CBaseSeqCmd;
class CTaskLoadTable;


class CLoadTblCmd_Initial :	public CBaseSeqCmd
{
public:
	CLoadTblCmd_Initial(void);
	~CLoadTblCmd_Initial(void);

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
	int             m_nOrgRetryCount;
	int				m_nTriggerInitRetryCnt;
	
	CPerformanceTime tmr;
	int				m_nBackVibratorStep;
	int				m_nVibCurTryCnt;

	enum eVibratorStep
	{
		do_Vibrator_On = 35000,
		do_Vibrator_On_Wait,
		do_Vibrator_Off,
		do_Vibrator_Off_Wait,
		Vibrator_complete,
	};
};

