#pragma once

#include <vector>

// #define TEST_UD_PKR_Y (0)	// station device pick, 그 외는 place
// #define TEST_LD_PKR_Y (1)	// station device place 그 외는 pick

#define TEST_LD_PKR_Y (0)	// station device pick, 그 외는 place
#define TEST_LD_PKR_Y2 (1)	// station device pick, 그 외는 place
#define TEST_UD_PKR_Y (2)	// station device place 그 외는 pick
#define TEST_UD_PKR_Y2 (3)	// station device place 그 외는 pick

#define TEST_LD_HAND (0)	// LD Test PP
#define TEST_UD_HAND (1)	// UD Test PP

#define LOAD_TABLE_LU_1	(0)
#define LOAD_TABLE_B_1	(1)
#define LOAD_TABLE_LU_2	(2)
#define LOAD_TABLE_B_2	(3)


#define TEST_PKR2_X_NOT 1



class CTaskAutoTestPp : public CBaseTaskManager, public CBaseAutoSeqManager
{
public:
	CTaskAutoTestPp(void);
	~CTaskAutoTestPp(void);

	int Initialize( TCHAR * szDriverFileName, _eTestPPIdx eTestIdx );
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
	void AutoState_Execute_Normal();
	void AutoState_Execute_AutoGrr();
	void AutoState_Execute_QaSample();
	void AutoState_Execute_AutoLoop();
	void AutoState_Execute_AutoAudit();

	void AutoState_NormalComplete();
	void AutoState_AlarmStop();
	void AutoState_Resume();
	void AutoState_CleanOut();

private:
	enum _eExcutingStep {
		eExcutingStep_Start = 0,

		eExcutingStep_Check_Job,

		// Clean buffer table pick - Load Hand
		eExcutingStep_Move_PickCleanTable,
		eExcutingStep_WaitEnd_PickCleanTable,

		// ART buffer table pick
		eExcutingStep_Move_PickArtBuffTable,
		eExcutingStep_WaitEnd_PickArtBuffTable,

		// Load Table Check Release
		eExcutingStep_Check_Load_Table_Release,
		eExcutingStep_WaitEnd_Check_Load_Table_Release,

		// Load table pick - Load Hand
		eExcutingStep_Move_PickLoadTable,
		eExcutingStep_WaitEnd_PickLoadTable,

		// 2D barcode error bin place
		eExcutingStep_Move_PlaceLoadTable_2dErrDvc,
		eExcutingStep_WaitEnd_PlaceLoadTable_2dErrDvc,

		//Load Table Pick Table Release
		eExcutingStep_Move_load_Table_Release,

		// Station pick - UnLoad Hand
		eExcutingStep_Move_PickStation,
		eExcutingStep_WaitEnd_PickStation,

		// Station Place AA Retest - UnLoad Hand
		eExcutingStep_Move_PlaceStation_AARetest,
		eExcutingStep_WaitEnd_PlaceStation_AARetest,


		//Unload Test PP가 Press와 충돌 위치 이면 Table Place먼저 진행.
		eExcutingStep_Check_PlaceArtBuffAndLoadTable,

		// Station place - Load Hand
		//Unload 자재 Table에 Place하기 전, Station에 Load 자재 미리 투입.
		eExcutingStep_Move_Already_PlaceStation,
		eExcutingStep_WaitEnd_Already_PlaceStation,


		// UnLoad Table Wait
		eExcutingStep_Move_Unload_Table_Wait,

		// Station Socket All Off로 인해, 해당 Picker Device Vacuum일 경우, Place 진행.
		eExcutingStep_Move_PlaceAllSiteOffDevice,
		eExcutingStep_WaitEnd_PlaceAllSiteOffDevice,

		// ART buffer table place - Unload Hand
		eExcutingStep_Move_PlaceArtBuffTable,
		eExcutingStep_WaitEnd_PlaceArtBuffTable,

		// Load table place - UnLoad Hand
		eExcutingStep_Move_PlaceLoadTable,
		eExcutingStep_WaitEnd_PlaceLoadTable,

		// UnLoad Table Release
		eExcutingStep_Move_Unload_Table_Release,

  		// Station place - Load Hand
  		eExcutingStep_Move_PlaceStation,
  		eExcutingStep_WaitEnd_PlaceStation,

		// Clean buffer table place - Load Hand
		eExcutingStep_Move_PlaceCleanTable,
		eExcutingStep_WaitEnd_PlaceCleanTable,
			
		eExcutingStep_Check_More_Job,
		
		// index time 증가를 위한 table 대기
		eExcutingStep_Move_Already_Table,
		eExcutingStep_WaitEnd_Already_Table,
		
		// socket clean job on, buffer table place - load hand
		eExcutingStep_Move_Already_Pick_Dvc_PlaceArtBuffTable,
		eExcutingStep_WaitEnd_Already_Pick_Dvc__PlaceArtBuffTable,

		// all test load table already pick - load hand
		eExcutingStep_Move_Already_All_Test_PickLoadTable,
		eExcutingStep_WaitEnd_Already_All_Test_PickLoadTable,

		// station already move pos - unload hand
		eExcutingStep_Move_Already_Station,
		eExcutingStep_WaitEnd_Already_Station,

		eExcutingStep_End,

		eExcutingStep_FindJob = 0,
		eExcutingStep_ErrorJob = -1,
		eExcutingStep_WaitOtherJob = -2,	// table의 경우 기다릴 수 있다.
	};

	enum _eExcutingStepGrr {
		eExcutingStepGrr_Start = 0,
		eExcutingStepGrr_WaitLoadTable,
		eExcutingStepGrr_Check_Job,

		eExcutingStepGrr_Move_PickCleanTable,
		eExcutingStepGrr_WaitEnd_PickCleanTable,

		//Wait Unload Table
		eExcutingStepGrr_WaitUnloadTable,

		//Pick ArtBuffer Swap Mode
		eExcutingStepGrr_Move_PickArtBuffTable_Swap,
		eExcutingStepGrr_Wait_PickArtBuffTable_Swap,

		eExcutingStepGrr_Move_PickArtBuffTable,
		eExcutingStepGrr_Wait_PickArtBuffTable,

		eExcutingStepGrr_Move_PickLoadTable,
		eExcutingStepGrr_Wait_PickLoadTable,

		//Load Table Pick Table Release
		eExcutingStepGrr_Move_load_Table_Release,

		//Station pick - Unload Hand
		eExcutingStepGrr_Move_PickStation,
		eExcutingStepGrr_Wait_PickStation,

		//Unload Hand Place
		eExcutingStepGrr_Move_SamePlaceStation,
		eExcutingStepGrr_Wait_SamePlaceStation,

		//Unload Table Wait
		eExcutingStepGrr_Move_Unload_Table_Wait,

		//ART Buffer Table Place - Unload Hand
		eExcutingStepGrr_Move_PlaceArtBuffTable,
		eExcutingStepGrr_WaitEnd_PlaceArtBuffTable,

		eExcutingStepGrr_Move_PlaceLoadTable,
		eExcutingStepGrr_Wait_PlaceLoadTable,

		//Unload Table Release
		eExcutingStepGrr_Move_Unload_Table_Release,

		//Station Place - Load Hand
		eExcutingStepGrr_Move_PlaceStation,
		eExcutingStepGrr_Wait_PlaceStation,

		eExcutingStepGrr_Move_PlaceCleanTable,
		eExcutingStepGrr_WaitEnd_PlaceCleanTable,

		eExcutingStepGrr_ChkMoreJob,

		eExcutingStepGrr_FindJob = 0,
		eExcutingStepGrr_ErrorJob = -1,
		eExcutingStepGrr_WaitOtherJob = -2,	// table의 경우 기다릴 수 있다.
	};

	int m_nJobAtsIdx;

	_eTestPPIdx m_eTestIdx;

	std::vector<int> m_vModuleSetPressIdx;
	std::vector<int> m_vModuleSetLoadTableIdx;

	int m_nCleanMode;
	int m_nLoadingTableInx;
	int m_nUnLoadingTableInx;
	CPoint m_cJobPocket;
	std::vector<CPoint> m_vJobPicker_Pick;
	std::vector<CPoint> m_vJobPicker_Place;

	BOOL m_bAlreadyPick;

	int m_nJobSiteRow;
	BOOL m_bNotChkPreContactPos;

	// interlock common
	int ChkInterlockAts(int nCmdStage);
	//BOOL ChkCleaningMode(int nChkAtsIdx);

	// check device
	int ChkAutoRetestBin( ST_DD_DEVICE stDeviceData );
	// ART table, Cleaning table, Load table ( 공통 사용 함수 )
	int FindPickPlaceFromTable( int nLocIdx, int nTableDvc[VIRTUAL_POCKET_X_MAX][VIRTUAL_POCKET_Y_MAX], int nPickerPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], BOOL bPickPlace, int *n_Xpos, int *n_YPos, int nWorkPkr[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nTableLoc = DEF_TABLE_TYPE_LOAD_UNLOAD);
	int FillDvc_VirtualTable(int nDvcStageIdx, int* pnBuffer, int nBufSizeX, int nBufSizeY, int bPickPlace, int nAtsIdx = -1, int nTableType = -1);
	
	// Station ( 공통 사용 함수 )
	int FindPickPlaceFromStation( int nAtsIdx, int nSiteDvc[VIRTUAL_POCKET_X_MAX][VIRTUAL_POCKET_Y_MAX], int nPickerPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], BOOL bPickPlace, int *n_Xpos, int *n_YPos, int nWorkPkr[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] );
	int FillDvc_VirtualStation(int nDvcStageIdx, int* pnBuffer, int nBufSizeX, int nBufSizeY, int bPickPlace, BOOL bExchFlag=FALSE);
	int GetAreaProblemStation(int nAtsIdx, int nPocketX, int nPocketY, int nArrHandWork[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nPitchType_X, int nPitchType_Y);

	// load table Hand assign
	int GetDvcLdTableHandAssign(BOOL bPickPlace, int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nWorkingAreaIdx );
	// cleaning table Hand assign
	int GetDvcCleanHandAssign(BOOL bPickPlace, int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nWorkingAreaIdx );
	// Station Hand assign
	int GetDvcStationHandAssign(BOOL bPickPlace, int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nWorkingAreaIdx, BOOL bChkUD_RetestBin);
	// Art Buffer Hand assign
	int GetDvcArtBuffHandAssign(BOOL bPickPlace, int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nWorkingAreaIdx);


	int ChkStationOnOffDvcCnt(BOOL bGrrMode=FALSE);


	// GRR
	int GetTableIdx(int nType);
	int ChkJobGrr_LoadTable(int nAtsIdx, BOOL bPickPlace, CPoint &vPocket, std::vector<CPoint> &vPicker);
	int ChkJobGrr_SamePlaceStation(CPoint &cPocket, std::vector<CPoint> &vPicker);
	int GetDvcStationHandAssignGrr(BOOL bPickPlace, int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], BOOL bChkUD_Grr = FALSE);
	int FillDvcGrr_SamePlaceVirtualStation(int nDvcStageIdx, int* pnBuffer, int nBufSizeX, int nBufSizeY);
	int FillDvcGrr_VirtualTable_Swap(int nDvcStageIdx, int* pnBuffer, int nBufSizeX, int nBufSizeY, int bPickPlace, int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nAtsIdx = -1);
	int FillDvcGrr_VirtualTable(int nDvcStageIdx, int* pnBuffer, int nBufSizeX, int nBufSizeY, int bPickPlace, int nAtsIdx = -1, int nTableType = -1);
	int FillDvcGrr_VirtualStation(int nDvcStageIdx, int* pnBuffer, int nBufSizeX, int nBufSizeY, int bPickPlace);
	int ChkJobGrr_CleanBuff(int nAtsIdx, BOOL bPickPlace, CPoint &vPocket, std::vector<CPoint> &vPicker);
	int ChkJobGrr_ArtBuff(int nAtsIdx, BOOL bPickPlace, CPoint &vPocket, std::vector<CPoint> &vPicker, int& nLocIdx);
	int ChkJobGrr_SwapArtBuff(int nAtsIdx, BOOL bPickPlace, CPoint &vPocket, std::vector<CPoint> &vPicker, int& nLocIdx);
	int GetDvcCleanGrrHandAssign(BOOL bPickPlace, int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nWorkingAreaIdx);
	int GetDvcArtBuffGrrHandAssign(BOOL bPickPlace, int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nWorkingAreaIdx, int nStationWorkRow);
	int GetDvcSwapArtBuffGrrHandAssign(BOOL bPickPlace, int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nWorkingAreaIdx);
	int ChkJobGrr_Station(int nAtsIdx, BOOL bPickPlace, CPoint &cPocket, std::vector<CPoint> &vPicker);
	int GrrTableChangeLoc(int nLoc);
	int FindPickPlaceFromTableSwap(int nLocIdx, int nTableDvc[VIRTUAL_POCKET_X_MAX][VIRTUAL_POCKET_Y_MAX], int nPickerPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], BOOL bPickPlace, int *n_Xpos, int *n_YPos, int nWorkPkr[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], CPoint* cSamePP_Pos = NULL, int nPocketOffset = 0);
	int FindPickPlaceFromCleanTable(int nTableDvc[VIRTUAL_POCKET_X_MAX][VIRTUAL_POCKET_Y_MAX], int nPickerPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], BOOL bPickPlace, int *n_Xpos, int *n_YPos, int nWorkPkr[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y]);
	int GetDvcLdTableHandAssignGrr(BOOL bPickPlace, int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nWorkingAreaIdx);
	BOOL ChkGrrAllSiteTestDone(int nDvcStage);

	int GetFindLoadTableSearchX(eDeviceDataStage nDevLoc, int nTableLoc, int nLocIdx, int nPitchType_X, BOOL bPickPlace);
	int GetFindLoadTableSearchY(eDeviceDataStage nDevLoc, int nTableLoc, int nLocIdx, int nPitchType_Y, BOOL bPickPlace);

public:
	std::vector<CPoint> m_vLossPkr[eMaxTestPPTypeCount];
	// AutoRetest Buffer
	int		CheckJob_ArtBuff(int nAtsIdx, BOOL bPickPlace, CPoint &vPocket, std::vector<CPoint> &vPicker, int& nLocIdx);
	// cleaning table
	int		CheckJob_CleanBuff(int nAtsIdx, BOOL bPickPlace, CPoint &vPocket, std::vector<CPoint> &vPicker);
	// load table
	int		CheckJob_LoadTbl(int nAtsIdx, BOOL bPickPlace, CPoint &vPocket, std::vector<CPoint> &vPicker);
	// Station
	int	 CheckJob_Station( int nAtsIdx, BOOL bPickPlace, CPoint &vPocket, std::vector<CPoint>&vPicker,BOOL bRetestPlace = FALSE);
	// 2d error bin 
	int		CheckJob_2dErrLoadTbl(CPoint &vPocket, std::vector<CPoint> &vPicker, BOOL bPickPlace);
	
	int		GetModuleSocketOnCnt(BOOL bGrrMode=FALSE);
	int		GetModuleGrrSocketOnCnt(BOOL bGrrMode);
	int		GetModuleAuditSocketOnCnt();

	int		ChkDevice(CString &strErrorMsg, int &rHandType, std::vector<CString>& vErrString);
	void	LossDevice(int nHandType);
	// interlock common
	BOOL	ChkCleaningMode(int nChkAtsIdx);
	BOOL	GetDvcStationCleanDevAssign(BOOL bPickPlace, int nWorkingAreaIdx);

	BOOL	PossibleAlreadyPickLoadTable(int nTestHandidx,int nJobIdx, int &nNextJobIdx);
	//Auto Socket Cleaning 예약 시 현재 들고 있는 Device를 Art Buffer Table에 Place
	int		CheckJobAlreadyPickDvcPlacetoArtBuff(int nAtsIdx, CPoint &vPocket, std::vector<CPoint> &vPicker,int& nLocInx);
	int		GetAlreadyPickDvcArtBuffHandAssign(int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nWorkingAreaIdx);

	int					ConvertPressIdx(int nIdx);
	int					ConvertLoadTableIdx(int nIdx);
	BOOL				ModuleUseAbleCheck();
	int					GetLoadTableInx(BOOL bPickPlace);
	int					GetReserveAndLoadTblWorkPos(int nLoadTableIdx);
	BOOL				GetHandAutoRetestBinCheck();

	void				SetSiteGroupReserve(bool bReserve);
	std::vector<CPoint> GetLossDevice();

	int					FindStation_CompEOT();
	int					CompSystemTime(SYSTEMTIME SysTime1, SYSTEMTIME SysTime2); //기존, 신규;
};