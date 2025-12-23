#pragma once
#include <deque>
//using namespace std;

class CTaskAutoTrayBasePP : public CBaseTaskManager, public CBaseAutoSeqManager
{
public:
	CTaskAutoTrayBasePP();
	~CTaskAutoTrayBasePP();

public:
	int                       MakeLoadTblWorkingIdx(BOOL bReference =FALSE);
	void                      SetNewTrayPP_Schedule(int nPickPlace, eLocIdxTrayPP nTargetLocIdx, _eUsePicker _picker_type);
	int                       FillDvc_VirtualTable(eDeviceDataStage nDvcStageIdx, int* pnBuffer, int nBufSizeX, int nBufSizeY, int nPickPlace);
	int	                      FindPickPlaceFromTable(int nLocIdx, int nTableDvc[VIRTUAL_POCKET_Y_MAX][VIRTUAL_POCKET_X_MAX], int nPickerPattern[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X], BOOL bPickPlace, int * n_Xpos, int * n_YPos, int nWorkPkr[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X], BOOL bVacuumCheck = FALSE);
	BOOL                      GetAreaProblem(int nTargetPosX, int nTargetPosY, eDeviceDataStage WorkBin, int nArrHandWorkSource[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X]);
	CBaseSeqCmd*              GetNextSchedule();

	eDeviceFullEmptyCheckCode LoadTableFullEmptyCheck(eDeviceDataStage DvcLoc/*, int nTrayPickPlace*//*=DEF_PICK,DEF_PLACE*/);
	void                      LoadingTblSetReserve();

	//GRR
	int						  MakeTrayPPScheduleGrr(int nHandType);
	int						  _MakeScheduleGrr_TrayPp_Pick_Feeder();

	//Vaccum Check
	int						  SetVacuumCheck_TrayPP_Schedule(int nPickPlace, _eUsePicker _picker_type);
	int						  FillDvc_VacuumCheck_VirtualTable(eDeviceDataStage nDvcStageIdx, int* pnBuffer, int nBufSizeX, int nBufSizeY, int nPickPlace);
	int						  FindVacuumCheckFromTable(int nLocIdx, int nTableDvc[VIRTUAL_POCKET_Y_MAX][VIRTUAL_POCKET_X_MAX], int nPickerPattern[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X], BOOL bPickPlace, int * n_Xpos, int * n_YPos, int nWorkPkr[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X]);
public:
	typedef std::vector< std::vector< _ePressUnit > > PressUnit2xArray;
	PressUnit2xArray           m_test_modules_press_idx;

protected:
	int		                   m_nWorkRecLoadTbl;

	_eTrayPPIdx                m_eTrayIdx;
	CTaskTrayBasePP*           m_pTaskTrayPP;

	std::deque< CBaseSeqCmd* > m_listTrayPpSchedule;

	int m_nJobTbl;

	enum _eExcutingStepJob {
		eExcutingStep_FindJob = 0,
		eExcutingStep_ErrorJob = -1,
		eExcutingStep_WaitOtherJob = -2,	// table의 경우 기다릴 수 있다.
	};
};

