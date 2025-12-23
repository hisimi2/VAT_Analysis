#pragma once
class CStackerCmd_TrayOut : public CBaseSeqCmd
{
public:
	CStackerCmd_TrayOut();
	~CStackerCmd_TrayOut();

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

private:
	CTimerTickCheck	m_swTimeout;

	enum _eExecuteStep
	{
		eExecuteStep_Chk_Tray_Sensor = 0,

		eExecuteStep_Stcker_Align_Cylinder_Forward,
		eExecuteStep_Stcker_Align_Cylinder_Backward,

		eExecuteStep_Feeder_Move_Y,
		eExecuteStep_Chk_Feeder_Move_Y,

		eExecuteStep_Feeder_Align_Cyl_Back,
		eExecuteStep_Chk_Feeder_Align_Cyl_Back,

		eExecuteStep_Feeder_Cyl_Down,
		eExecuteStep_Chk_Feeder_Cyl_Down,

		eExecuteStep_Feeder_Unclamp,
		eExecuteStep_Chk_Feeder_Unclamp,

		eExecuteStep_Feeder_Up,
		eExecuteStep_Chk_Feeder_Up,


		eExecuteStep_Conveyor_Move,
		eExecuteStep_Chk_Front_Sen,

		eExecuteStep_Main_Pusher_Up,
		eExecuteStep_Chk_Main_Pusher_Up,

		eExecuteStep_Conveyor_Move_Stop,

		eExecuteStep_Stacker_Unclamp,
		eExecuteStep_Chk_Stacker_Unclamp,

		eExecuteStep_Sub_Pusher_Up,
		eExecuteStep_Chk_Sub_Pusher_Up,

		eExecuteStep_Stacker_Clamp,
		eExecuteStep_Chk_Stacker_Clamp,

		eExecuteStep_Sub_Pusher_Down,
		eExecuteStep_Chk_Sub_Pusher_Down,

		eExecuteStep_Main_Pusher_Down,
		eExecuteStep_Chk_Main_Pusher_Down,

		eExecuteStep_End,
	};
};

