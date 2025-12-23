#pragma once

class CBaseTaskManager;
class CBaseAutoSeqManager;
class CHw_Bin_Sort_Item;

class CTaskAutoTrayUnloadPP : public CTaskAutoTrayBasePP
{
private:
	enum {
		eTHREAD_1 = 0,		// SeqCmd running
	};
	enum eRobotWorkDir {
		eRobotWorkDirTopLeft = 0,
		eRobotWorkDirBottomLeft,
	};

private:
	enum _eExcutingStep {
		eExcutingStep_Start = 0,
		eExcutingStep_End,
	};

private:
	enum _eFindTableOpt {
		_eFindTableOpt_Normal = 0,
		_eFindTableOpt_PickFromTableUArea_byUnloadPicker,
		_eFindTableOpt_PlaceToTableLArea_byUnloadPicker,
		_eFindTableOpt_PickFromTableAll_byUnloadPicker
	};

	enum eTrayPpPickPlaceSubCmd {
		eTrayPpPickPlaceSubCmd_Idle,
		eTrayPpPickPlaceSubCmd_PlacePlate,
		eTrayPpPickPlaceSubCmd_WaitFindTable,
		eTrayPpPickPlaceSubCmd_TblMoveWait,
		eTrayPpPickPlaceSubCmd_PickTbl,
	};

	enum eAutoTrayExecuteSteps {
		eAutoTrayStep_Run_Start = 0,
		eAutoTrayStep_Run_ResetSchedule,

		eAutoTrayStep_Run_Table_Vacuum_Check_job,
		eAutoTrayStep_Run_Table_Vacuum_Check_Start,
		eAutoTrayStep_Run_Table_Vacuum_Check_Wait,
		eAutoTrayStep_Run_Table_Vacuum_Check_Safety_move,
		eAutoTrayStep_Run_Table_Vacuum_Check_Safety_end,

		eAutoTrayStep_Run_MakeSchedule,
		eAutoTrayStep_Run_TrayPpPickPlaceStart,
		eAutoTrayStep_Run_TrayPpPickPlaceWait,
		eAutoTrayStep_Run_TrayPreMoveLoadTblStart,
		eAutoTrayStep_Run_TrayPP_MoveSafetyPos,
		eAutoTrayStep_Run_TrayPP_MoveTableWaitPos,
		eAutoTrayStep_Run_TrayPpComptCheck,
		eAutoTrayStep_Run_End,
	};

	enum eAutoTrayExecuteJobSteps {
		eExcutingStep_FindJob = 0,
		eExcutingStep_ErrorJob = -1,
		eExcutingStep_WaitOtherJob = -2,
	};

// Function
public:
	CTaskAutoTrayUnloadPP(void);
	~CTaskAutoTrayUnloadPP(void);

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
	void AutoState_Execute_Normal();

	void AutoState_NormalComplete();
	void AutoState_AlarmStop();
	void AutoState_Resume();

	void AutoState_CleanOut();


	//=================================================================================
	// Variable
	//=================================================================================	
private:
	int                        m_nPickPlaceStep;
	int                        m_nPickPlaceBackUpStep;	// 마지막으로 작업하였던 Sequance 확인.
	int                        m_nLoadTrayAutoSkipCount;
	int                        m_nChkSafetyPosCnt;
	int                        m_nTblReworkNo;

	CTimerTickCheck            m_swTimeout;
						       
	CPoint                     m_cJobPocket;
	std::vector<CPoint>        m_vJobPicker_Pick;
	std::vector<CPoint>        m_vJobPicker_Place;
	int		                   m_nFindWorkingShuttle;
	
	//=================================================================================
	// Auto Sequence Schedule Management Functions
	//=================================================================================
private:
	void         FreeAllSchedule();
	CBaseSeqCmd* GetNextSchedule();
	
	int		     MakeTrayPpSchedule();	
	int		     MakeTrayPpScheduleGrr();
	int          MakeTrayPpScheduleLoop();
	int          MakeTrayPpScheduleAudit();
	int		     MakeTrayPpScheduleQaSample();
	int		     ChkAxisZ_SafetyPos();
	int		     WaitPreMoveLoadTblPos();

	// 8picker option 
	int		     MakeTrayPpSchedule8Picker();
	int		     _MakeJobPlaceToSetplate();
	int		     _MakeJobPickFromSetplate(int nWorkingTable);
	int		     _MakeJobPlaceToTable(int nIdxLoadTbl);
	int		     _MakeJobPickFromTable(int nIdxLoadTbl);
	int		     _FindPickPlaceFromTable8Picker(int nLocIdx, int nTableDvc[VIRTUAL_POCKET_Y_MAX][VIRTUAL_POCKET_X_MAX], int nPickerPattern[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X], BOOL bPickPlace, int * n_Xpos, int * n_YPos	, int nWorkPkr[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X]);
	int          _MakeSchedule_TrayPp_Place_LoadTable(int nWorkingTable);
	int		     MakeJobTblIndex();
	int          SetPlatePickEnableChk();
			     
	int          _MakeJobPlaceToBuffer();
	int          _MakeJobPickFromBuffer();
	int		     _MakeSchedule_TrayPp_Place_SetPlate();
	int		     _MakeSchedule_TrayPp_Pick_LoadTable( int nIdxLoadTbl );
	int		     _MakeSchedule_TrayPp_Pick_SetPlate(CPoint &vPocket, std::vector<CPoint> &vPicker);
	int		     _MakeSchedule_TrayPp_Place_SetPlate_QaEndRemainDevice();
	int		     _MakeScheduleGrr_TrayPp_Pick_SetPlate(eStacker_Idx _conveyor_idx);
	int		     _MakeScheduleGrr_TrayPp_Pick_LoadTable( int nIdxLoadTbl, bool bAutoGrrCancel = false );
	int		     _MakeScheduleGrr_TrayPp_Place_LoadTable( int nIdxLoadTbl, _eUsePicker ePicker );
	int		     _MakeScheduleGrr_TrayPp_Place_UnloadSetPlate();
			     
	int		     GetDeviceCnt_LoadPicker();
	int		     GetGrrEndDeviceCnt_FromUnloadPicker();
	void	     GetExceptSiteGroup_FromUnloadPicker( std::vector<int>* vExceptSiteGroupNo, int nTblNo);
	int		     GetWorkable_CTrayHand_forLoadTable( BOOL bPickPlace, int nBuffer[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ]
			     									, _eUsePicker ePickerArea = _eUsePicker_Unload );
			     
	int		     ConvertTableIndex( _ePressUnit eTestSiteIdx );
	int		     CalcUsableSiteGroupCnt();
			     
	int		     FindTableIndex( _eFindTableOpt eFindTableOpt = _eFindTableOpt_Normal/*, std::vector<int> vExceptSiteGroupNo = std::vector<int>( 0 )*/ );
	int		     FindPickerWork( BOOL bPickPlace, int nPocketCntX, int nPocketCntY, int** pnArrDeviceData
			     				, int nWorkablePicker[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ]
			     				, POINT& ptPocket, int nArrPickerWork[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ], BOOL bUnloadPickerToTable = FALSE
			     				, int nTableIndex = -1 );
			     


	/*
	*	device 정보; empty, partial, full 조사 (setplate)
	*/
	eDeviceFullEmptyCheckCode SetplateDevFullEmptyCheck(eDeviceDataStage DvcLoc);

	int                       get_load_picker_exist_mask();

	int convert_sw_bin_to_hw_bin_multi(CHw_Bin_Sort_Item* pArrHwBins, int nArrSize, const char *pszBinResult);
	int convert_ErrDisrip_to_hw_bin_multi(CHw_Bin_Sort_Item* pArrHwBins, int nArrSize, const char *pszErrStr);
	int GetDeviceExist_CTrayHand(BOOL bPickPlace, eDeviceDataStage WorkBin, int* pnBuffer, int nBufSizeX, int nBufSizeY);
	int FillDeviceData_VirtualCTray(int nStageLoc, int* pnBuffer, int nBufSizeX, int nBufSizeY, int nVtl0X, int nVtl0Y);

	int FindSortToSetplate(int nWorkPlace,
		                   int nArrDvcData_VirtualCTray[VIRTUAL_POCKET_Y_MAX][VIRTUAL_POCKET_X_MAX],
		                   int nPickerDvcPattern[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X],
		                   BOOL bPickPlace,
		                   int *n_Xpos, int *n_YPos,
		                   int nArrHandWork[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X],
		                   int nWorkDir);

	int GetWorkable_CTrayHand(BOOL bPickPlace, int pnBuffer[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X], bool bAutoGrrCancel = false);

	
	int FindPickFromSetplate(int nWorkPlace,														//eDeviceDataStage
		                     int nArrDvcData_Container[VIRTUAL_POCKET_Y_MAX][VIRTUAL_POCKET_X_MAX],	//(100,100) based setplate device model
		                     int nPickerDvcPattern[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X],			//작업 가능 picker pattern
		                     BOOL bPickPlace,														//DEF_PICK or DEF_PLACE
		                     int *n_Xpos, int *n_YPos,												//Pocket
		                     int nArrHandWork[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X],			    //추가 Picker pattern
		                     int nWorkDir,
		                     BOOL bChkQaSampleCnt);


	//loading table device map -> bit mask (array)
	void get_loadtable_mask_for_load(int* pArrLdTblMask, int nArrSize = MAX_LD_TBL_CNT);


	int FindSortToUnloadSetplateGrr(int    nPocketCntX,
		                            int    nPocketCntY,
		                            int    nWorkablePicker[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X],
		                            POINT& ptPocket,
		                            int    nArrPickerWork[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X]);

	int FindUnloadigSetplate(eDeviceDataStage *pArrDvcHwBins, int nMaxArrDvcHwBins);

	int get_unload_picker_empty_mask();

	int FindUnloadigSetplateQaRemainDevice(eDeviceDataStage *pArrDvcHwBins, int nMaxArrDvcHwBins);

	int GetDeviceExist_CTrayHandQaRemainDevice(BOOL bPickPlace, eDeviceDataStage WorkBin, int* pnBuffer, int nBufSizeX, int nBufSizeY);
};

