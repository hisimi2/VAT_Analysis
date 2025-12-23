#pragma once

class CTransferCmd_MovePickPlace : public CBaseSeqCmd
{	
public:
	CTransferCmd_MovePickPlace(int nTargetConveyorIndex, int nPickPlace );
	~CTransferCmd_MovePickPlace();

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

	CTimerTickCheck	m_swTimeout;

private:
	int m_nLast_Do_Step;
	
	int m_nTargetConveyorIndex;
	int m_nPickPlace;

	CTimerTickCheck	m_TrayInterlock;

	enum _eExcutingStep {
		eExcutingStep_Start = 0,

		eExcutingStep_Do_Pre_ClampUnClamp,

		eExcutingStep_Do_Pre_Z_Up,

		eExcutingStep_Move_X,
		eExcutingStep_WaitEnd_Move_X,

		eExcutingStep_Check_Conveyor_State,

		eExcutingStep_Do_Pre_Tray_Align_ForBack,

		eExcutingStep_Check_Conveyor_State_Retry,

		eExcutingStep_Do_Z_Down,	
		eExcutingStep_Do_ClampUnclamp,

		eExcutingStep_Move_Conveyor_for_Jig,

		eExcutingStep_Do_Tray_Align_ForBack,
		eExcutingStep_Do_Z_Up,

		eExcutingStep_Check_Data,
		eExcutingStep_Check_Sensor_And_Process_Data,

		eExcutingStep_End = 99999,
	};
};



