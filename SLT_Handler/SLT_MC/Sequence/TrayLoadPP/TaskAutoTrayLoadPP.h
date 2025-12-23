#pragma once

class CTaskAutoTrayLoadPP : public CTaskAutoTrayBasePP

{
private:
	enum _eExcutingStep {
		eExcutingStep_Start = 0,
		//Table Vacuum Check step
		eExcutingStep_Table_Vacuum_Check_job,
		eExcutingStep_Table_Vacuum_Check_Start,
		eExcutingStep_Table_Vacuum_Check_Wait,
		eExcutingStep_Table_Vacuum_Check_end,

		eExcutingStep_CheckCondition,
		eExcutingStep_Pick,
		eExcutingStep_Place,
		eExcutingStep_PickPlaceWait,
		eExcutingStep_LoadTrayPpComptCheck,
		eExcutingStep_MoveAlreadyPos,
		eExcutingStep_MoveLoadPos,
		eExcutingStep_LoadTableWaitPos,
		eAutoTrayStep_MoveTableWaitPos,
		eExcutingStep_End,
	};

	enum eRobotWorkDir {
		eRobotWorkDirTopLeft = 0,
		eRobotWorkDirBottomLeft,
	};


private:
	enum {
		eTHREAD_1 = 0,		// SeqCmd running
	};

// Function
public:
	CTaskAutoTrayLoadPP(void);
	~CTaskAutoTrayLoadPP(void);

	//=================================================================================
	// Basic Auto Sequence Management Overide Function
	//=================================================================================
	int  Initialize();
	void Finalize();

	int  OnEvent(const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList=NULL);
	int  OnCommandRsp( CBaseSeqCmd * pRspCmd, int nEventId, WPARAM wParam, LPARAM lParam, std::vector<CPoint>& vPocket_Picker = std::vector<CPoint>() );
	void OnOperatorCommand( int nOperatorCommand, WPARAM wParam, LPARAM lParam );

	void ThreadProc1();

	void MakeLog( LPCTSTR fmt, ... );

	//Auto Run Mode
	void AutoState_Idle();
	void AutoState_InitNeed();
	void AutoState_Initialize();
	void AutoState_Pause();

	void AutoState_Execute();

	void AutoState_NormalComplete();
	void AutoState_AlarmStop();
	void AutoState_Resume();

	void AutoState_CleanOut();

	int GetDeviceExist_CTrayHand( _eTrayPPIdx TrayPPIdx, BOOL bPickPlace, eDeviceDataStage WorkBin, int* pnBuffer, int nBufSizeX, int nBufSizeY );
	int FillDeviceData_VirtualCTray( eDeviceDataStage nStageLoc, int* pnBuffer, int nBufSizeX, int nBufSizeY, int nVtl0X, int nVtl0Y );
	int convert_sw_bin_to_hw_bin_multi( CHw_Bin_Sort_Item* pArrHwBins, int nArrSize, const char* pszBinResult );
	int convert_ErrDisrip_to_hw_bin_multi( CHw_Bin_Sort_Item* pArrHwBins, int nArrSize, const char* pszErrStr );
	int FindPickFromSetplate( _eTrayPPIdx TrayPPIdx, int nWorkPlace, int nArrDvcData_VirtualCTray[ VIRTUAL_POCKET_Y_MAX ][ VIRTUAL_POCKET_X_MAX ], int nPickerDvcPattern[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ], BOOL bPickPlace, int* n_Xpos, int* n_YPos, int nArrHandWork[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ], int nWorkDir, BOOL bChkQaSampleCnt );
	int FillDvc_VirtualTable( eDeviceDataStage nDvcStageIdx, int* pnBuffer, int nBufSizeX, int nBufSizeY, int nPickPlace );

//	int MakeLoadTblWorkingIdx( _eTrayPPIdx TrayPPIdx );
//	void SetNewTrayPP_Schedule( _eTrayPPIdx TrayPPIdx, int nPickPlace, eLocIdxTrayPP nTargetLocIdx, _eUsePicker _picker_type );
	int _MakeSchedule_TrayPp_Place_LoadTable( _eTrayPPIdx TrayPPIdx, eLocIdxTrayPP nIdxLoadTbl );

	_eExcutingStep _MakeSchedule_TrayPp_Pick_SetPlate( _eTrayPPIdx TrayPPIdx, eDeviceDataStage stage, CPoint& vPocket, std::vector<CPoint>& vPicker,BOOL bSearchingAlogrithm = FALSE);


};

