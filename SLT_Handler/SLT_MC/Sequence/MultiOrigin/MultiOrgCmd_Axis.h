#pragma once
#include "..\Dll_Src\LIB_IPC_TCP\LIB_IPC_SOCK\PerformanceTime.h"

class CBaseSeqCmd;

class CMultiOrgCmd_Axis :public CBaseSeqCmd
{
public:
	CMultiOrgCmd_Axis(void);
	~CMultiOrgCmd_Axis(void);

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

	int             m_nCmdStage; // enum eMultiOrgType{
	CTimerTickCheck m_tcTimeout;
	int             m_nRetryCount;
	int				m_nOrgRetryCount;

	int				m_nErrUnit;

	int				m_nBackVibratorStep;
private:
	std::vector<CTaskPressUnit*>   m_vAutoCmdMultiOrgPress;
	std::vector<CTaskTestPP*>      m_vAutoCmdMultiOrgTestPp;
	std::vector<CTaskLoadTable*>   m_vAutoCmdMultiOrgLoadTable;
	std::vector<CBaseTaskManager*> m_vAutoCmdMultiOrgEtc; // stacker, transfer
	std::vector<CTaskTrayBasePP*>  m_vAutoCmdMultiOrgTrayPp;
	std::vector<CTaskTrayFeeder*>  m_vAutoCmdMultiOrgTrayFeeder;

	//Load Table Stop Sensor Y Move
	std::vector<CTaskLoadTable*>	m_vDetectedStopSenTable;

	std::vector<CPoint> m_vErrPicker;

	CPerformanceTime tmr;

	enum eVibratorStep
	{
		do_Table_Vibrator_On = 35000,
		do_Table_Vibrator_On_Wait,
		do_Table_Vibrator_Off,
		do_Table_Vibrator_Off_Wait,
		Table_Vibrator_complete,
	};
};
