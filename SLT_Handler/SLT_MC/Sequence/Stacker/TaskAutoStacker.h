#pragma once




class CTaskAutoStacker : public CBaseTaskManager, public CBaseAutoSeqManager
{
	enum {
		eTHREAD_1 = 0,		// SeqCmd running
	};
// Function
public:
	CTaskAutoStacker(void);
	virtual ~CTaskAutoStacker(void);

	int Initialize( TCHAR * szDriverFileName, eStacker_Idx eStackerIndex, eStacker_Type eType );
	void Finalize();

	int OnEvent(const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList=NULL);
	int OnCommandRsp( CBaseSeqCmd * pRspCmd, int nEventId, WPARAM wParam, LPARAM lParam, std::vector<CPoint>& vPocket_Picker = std::vector<CPoint>() );
	void OnOperatorCommand( int nOperatorCommand, WPARAM wParam, LPARAM lParam );

	void ThreadProc1();

	void MakeLog( LPCTSTR fmt, ... );

	//Auto Run Mode
	void AutoState_Idle();
	void AutoState_InitNeed();
	void AutoState_Initialize();
	void AutoState_Pause();

	void AutoState_Execute();
	void AutoState_Execute_LoadStacker();
	void AutoState_Execute_UnloadStacker();
	void AutoState_Execute_EmptyLoadStacker();
	void AutoState_Execute_EmptyUnloadStacker();

	void AutoState_NormalComplete();
	void AutoState_AlarmStop();
	void AutoState_Resume();

	void AutoState_CleanOut();
	void AutoState_CleanOut_LoadStacker();
	void AutoState_CleanOut_UnloadStacker();
	void AutoState_CleanOut_EmptyLoadStacker();
	void AutoState_CleanOut_EmptyUnloadStacker();


	eStacker_Conveyor_State GetConveyorState();
	eStacker_Type GetStackerType();
	eStacker_Idx GetStackerIndex() { return m_eStackerIndex; }

	bool ConveyorIsFullTray();
	int GetDeviceCount_In_ConveyorTray();

	void SetPassFailConveyor( bool _pass ) { m_bPassConveyor = _pass; }
	bool GetPassFailConveyor() { return m_bPassConveyor; }
// Variable
private:
	enum _eExcutingStep {
		eExcutingStep_Start = 0,

		eExcutingStep_TrayDown,
		eExcutingStep_WaitEnd_TrayDown,
		eExcutingStep_Load,
		eExcutingStep_WaitEnd_Load,

		eExcutingStep_ReadyTo_PlaceCenter,
		eExcutingStep_WaitEnd_PlaceCenter,

		eExcutingStep_ReadyTo_PickCenter,
		eExcutingStep_WaitEnd_PickCenter,

		eExcutingStep_Do_Vibration,
		eExcutingStep_WaitEnd_Vibration,
		eExcutingStep_Check_Unload,
		eExcutingStep_Unload,
		eExcutingStep_WaitEnd_Unload,
		eExcutingStep_Check_TrayUp,
		eExcutingStep_TrayUp,
		eExcutingStep_WaitEnd_TrayUp,
		eExcutingStep_FullTrayCheck,
		eExcutingStep_WaitEnd_FullTrayCheck,

		eExcutingStep_End,
	};

	enum _eCleanoutStep {
		eCleanoutStep_Start = 9000,
		eCleanoutStep_CheckCondition,
		eCleanoutStep_TrayUp,
		eCleanoutStep_Wait_TrayUp_Comp,
		eCleanoutStep_Unload,
		eCleanoutStep_Wait_Unload_Comp,
		eCleanoutStep_End,
	};

	CTimerTickCheck m_swTimeout;

	eStacker_Idx m_eStackerIndex;
	//eDeviceLoadSts m_eDeviceLoadState;
	bool m_bPassConveyor;
	int m_nCurrentOccuredEventID;

	int m_nCurrentVibCnt;
	bool check_vibration_work_condition();

private:
	bool check_condition_center_stage_ready_to_place();
	bool check_condition_center_stage_tray_place();
	bool check_condition_center_stage_tray_remove();

	//================================================
	// Transfer State (transfer state는 feeder, transfer,
	// stacker 이렇게 3개의 auto thread가 접근하게 된다.
	//================================================
public:
	eTransferState   GetCenterStageTransferState();
	bool			 IsPrePickEmptyTrayPossible();
private:
	void             SetCenterStageTransferState(eTransferState state);
	eTransferState   m_eCenterStageTransferState;
	CRITICAL_SECTION m_csCenterStageTransferState;

};
