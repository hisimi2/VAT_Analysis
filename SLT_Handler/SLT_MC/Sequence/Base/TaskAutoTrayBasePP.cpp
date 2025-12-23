#include "stdafx.h"
#include "TaskAutoTrayBasePP.h"


CTaskAutoTrayBasePP::CTaskAutoTrayBasePP()
{
	std::vector< _ePressUnit > press_group_left_module;
	press_group_left_module.push_back(ePressUnit_Down_1);
	press_group_left_module.push_back(ePressUnit_Down_2);
	press_group_left_module.push_back(ePressUnit_Down_3);
	press_group_left_module.push_back(ePressUnit_Down_4);
	// 	press_group_left_module.push_back( ePressUnit_Up_1 );
	// 	press_group_left_module.push_back( ePressUnit_Up_2 );
	// 	press_group_left_module.push_back( ePressUnit_Up_3 );
	// 	press_group_left_module.push_back( ePressUnit_Up_4 );

	std::vector< _ePressUnit > press_group_right_module;
	press_group_right_module.push_back(ePressUnit_Down_5);
	press_group_right_module.push_back(ePressUnit_Down_6);
	press_group_right_module.push_back(ePressUnit_Down_7);
	press_group_right_module.push_back(ePressUnit_Down_8);
	// 	press_group_right_module.push_back( ePressUnit_Up_5 );
	// 	press_group_right_module.push_back( ePressUnit_Up_6 );
	// 	press_group_right_module.push_back( ePressUnit_Up_7 );
	// 	press_group_right_module.push_back( ePressUnit_Up_8 );

	m_test_modules_press_idx.push_back(press_group_left_module);
	m_test_modules_press_idx.push_back(press_group_right_module);


	m_nWorkRecLoadTbl = 0;
	m_pTaskTrayPP = nullptr;
	m_nJobTbl = -1;
}


CTaskAutoTrayBasePP::~CTaskAutoTrayBasePP()
{
}

int CTaskAutoTrayBasePP::MakeLoadTblWorkingIdx(BOOL bReference)
{
	std::vector<int> vLoadTblWorkIdx;
	vLoadTblWorkIdx.clear();

	switch (m_eTrayIdx) {
	case _eTrayPPIdx::eTrayPP_1_LD: 
	{
		vLoadTblWorkIdx.push_back(eLoadTbl_2);
		vLoadTblWorkIdx.push_back(eLoadTbl_3);
	}break;
	
	case _eTrayPPIdx::eTrayPP_2_UD: 
	{
		vLoadTblWorkIdx.push_back(eLoadTbl_1);
		vLoadTblWorkIdx.push_back(eLoadTbl_4);
	}break;
	}

	CTime cCurTime = CTime::GetCurrentTime();

	std::vector<CTime> vArrivedTblTm; vArrivedTblTm.clear();
	int nCntLoadPicker = m_pTaskTrayPP->GetTrayPpDvcCnt();
	int nCntUnloadPicker = 0;
	int nIdx = 0;
	// scan loadtable & make data
	for (auto i : vLoadTblWorkIdx)
	{
		vArrivedTblTm.push_back(0);
		// case1 :: Get site socket off data
		// case2 :: Check admin load table flag
		// case3 :: not ready load table
		// case4 :: checking motor position

		BOOL bGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
		bool all_site_is_off = true;
		std::vector< _ePressUnit > presses = m_test_modules_press_idx[i/2];
		for each(auto press_idx in presses) {
			all_site_is_off &= g_TaskPressUnit[press_idx].GetSocketOnCnt(bGrrUse) == 0;
		}

		int table_row_step = g_TaskLoadTbl[i].GetTableRowStep();
		if (m_eTrayIdx == eTrayPP_2_UD)
 			table_row_step = 0;


		int nWorkReserveTbl = g_TaskLoadTbl[i].GetWorkReserve();
		int nMotorPos       = g_TaskLoadTbl[i].ChkMotorPos(LOC_IDX_TBL_LD_SIDE, CTaskLoadTable::eAxisAll, table_row_step);
		int SeqCmdStatus    = g_TaskLoadTbl[i].GetSeqCmdStatus();
		int UnloadDeviceCnt = g_TaskLoadTbl[i].GetUnloadDeviceCnt();
		int LoadDeviceCnt   = g_TaskLoadTbl[i].GetLoadDeviceCnt();
		int nTrayPPReserve = RID_TRAY_PP_(m_eTrayIdx);
		if (all_site_is_off == true
			|| nTrayPPReserve != nWorkReserveTbl
			|| SeqCmdStatus != CBaseSeqCmd::eSeqCmdStatus_Ready
			|| nMotorPos != ERR_NO_ERROR
			|| (UnloadDeviceCnt == 0 && g_TaskSystemCtrl.IsOneCycleModeOn() == true && nCntLoadPicker == 0)
			|| (nCntLoadPicker == 0 && LoadDeviceCnt == 0 && UnloadDeviceCnt == 0))
		{
			vArrivedTblTm[nIdx] = (cCurTime + 99999);
			nIdx++;
			continue;
		}

		vArrivedTblTm[nIdx] = g_TaskLoadTbl[i].GetMoveSetplateTm();
		nIdx++;
	}

	// LoadTbl Arrived time calc
	std::vector<CTime>::iterator vdRetMin;
	vdRetMin = min_element(begin(vArrivedTblTm), end(vArrivedTblTm));

	if (*vdRetMin < cCurTime + 99999) {
		for (int i = 0; i < (int)vArrivedTblTm.size(); i++)
		{
			if (m_nWorkRecLoadTbl != -1) {
				if (vArrivedTblTm[i] == *vdRetMin) {
					if(bReference != TRUE)
						g_TaskLoadTbl[vLoadTblWorkIdx[i]].SetMoveSetPlateTm(cCurTime);

					return m_nWorkRecLoadTbl = vLoadTblWorkIdx[i];
				}
			}
		}

		for (int i = 0; i < (int)vLoadTblWorkIdx.size(); i++)
		{
			if (vArrivedTblTm[i] == *vdRetMin) {
				m_nWorkRecLoadTbl = vLoadTblWorkIdx[i];
				return vLoadTblWorkIdx[i];
			}
		}
	}

	return -1;
}


void CTaskAutoTrayBasePP::SetNewTrayPP_Schedule(int nPickPlace, eLocIdxTrayPP nTargetLocIdx, _eUsePicker _picker_type)
{
	bool Available = false;
	int nPickerDvcPattern[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = { 0, };

	eDeviceDataStage stage;
	switch (nTargetLocIdx) {
	case LOC_IDX_TRAY_PP_CONV_1:
	case LOC_IDX_TRAY_PP_CONV_2:
	case LOC_IDX_TRAY_PP_CONV_3:
	case LOC_IDX_TRAY_PP_CONV_4:
	case LOC_IDX_TRAY_PP_CONV_5:
	case LOC_IDX_TRAY_PP_CONV_6:
	case LOC_IDX_TRAY_PP_CONV_7:
	{
		stage = (eDeviceDataStage)(eDeviceStage_STACKER_CONV_1 + (nTargetLocIdx - LOC_IDX_TRAY_PP_CONV_1));

		for (int nPkrY = 0; nPkrY < MAX_TRAY_PP_CNT_Y; nPkrY++)
		{
			for (int nPkrX = 0; nPkrX < MAX_TRAY_PP_CNT_X; nPkrX++)
			{
				char setting = m_pTaskTrayPP->m_TrayPP_picker_setting[nPkrY][nPkrX];
				int device_exist = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TRAY_PP_1 + m_eTrayIdx, nPkrX, nPkrY);
				switch (nPickPlace) {
				case DEF_PICK:
				{
					//if( setting == 'L' ) {
					if (setting == _picker_type == _eUsePicker_Load ? 'L' : 'U') {
						nPickerDvcPattern[nPkrY][nPkrX] = device_exist;
						if (device_exist == DEF_NOT_EXIST) {
							Available = true;
						}
					}
					else {
						// pick 진행 시 동작 하지 말아야 하는 picker 
						nPickerDvcPattern[nPkrY][nPkrX] = DEF_EXIST;
					}
				}break;
				case DEF_PLACE:
				{
					//if( setting == 'U' ) {
					if (setting == _picker_type == _eUsePicker_Load ? 'L' : 'U') {
						nPickerDvcPattern[nPkrY][nPkrX] = device_exist;
						if (device_exist == DEF_EXIST) {
							Available = true;
						}
					}
					else {
						// place 진행 시 동작 하지 말아야 하는 picker 
						nPickerDvcPattern[nPkrY][nPkrX] = DEF_NOT_EXIST;
					}
				}break;
				}
			}
		}

	}break;
	case LOC_IDX_TRAY_PP_TBL_1:
	case LOC_IDX_TRAY_PP_TBL_2:
	case LOC_IDX_TRAY_PP_TBL_3:
	case LOC_IDX_TRAY_PP_TBL_4:
	{
		stage = (eDeviceDataStage)(eDeviceStage_LD_TBL_1 + (nTargetLocIdx - LOC_IDX_TRAY_PP_TBL_1));

		for (int nPkrY = 0; nPkrY < MAX_TRAY_PP_CNT_Y; nPkrY++)
		{
			for (int nPkrX = 0; nPkrX < MAX_TRAY_PP_CNT_X; nPkrX++)
			{
				char setting = m_pTaskTrayPP->m_TrayPP_picker_setting[nPkrY][nPkrX];
				int device_exist = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TRAY_PP_1 + m_eTrayIdx, nPkrX, nPkrY);

				switch (nPickPlace) {
				case DEF_PICK:
				{
					//if( setting == 'U' ) {
					if (setting == _picker_type == _eUsePicker_Load ? 'L' : 'U') {
						nPickerDvcPattern[nPkrY][nPkrX] = device_exist;
						if (device_exist == DEF_NOT_EXIST) {
							Available = true;
						}
					}
					else {
						// pick 진행 시 동작 하지 말아야 하는 picker 
						nPickerDvcPattern[nPkrY][nPkrX] = DEF_EXIST;
					}
				}break;
				case DEF_PLACE:
				{
					//if( setting == 'L' ) {
					if (setting == _picker_type == _eUsePicker_Load ? 'L' : 'U') {
						nPickerDvcPattern[nPkrY][nPkrX] = device_exist;
						if (device_exist == DEF_EXIST) {
							Available = true;
						}
					}
					else {
						// place 진행 시 동작 하지 말아야 하는 picker 
						nPickerDvcPattern[nPkrY][nPkrX] = DEF_NOT_EXIST;
					}
				}break;
				}
			}
		}
	}break;
	default:
		ASSERT(false);
		break;
	}

	int nArrVirtualTbl[VIRTUAL_POCKET_Y_MAX][VIRTUAL_POCKET_X_MAX];
	memset(nArrVirtualTbl, nPickPlace == DEF_PICK ? 0x00 : 0xFF, sizeof(nArrVirtualTbl));
	FillDvc_VirtualTable(stage, (int*)nArrVirtualTbl, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, nPickPlace);

	if (Available) {
		int nUseJobPkr[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = { 0, };
		int nPockX = -1, nPockY = -1;
		int nRetCode = FindPickPlaceFromTable(nTargetLocIdx, nArrVirtualTbl, nPickerDvcPattern, nPickPlace, &nPockX, &nPockY, nUseJobPkr);
		if (nRetCode > 0) {
			std::vector<CPoint> vPkr;
			for (int nPickX = 0; nPickX < MAX_TEST_PP_PKR_CNT_X; nPickX++) {
				for (int nPickY = 0; nPickY < MAX_TEST_PP_PKR_CNT_Y; nPickY++) {
					if (nUseJobPkr[nPickY][nPickX] != 0) {
						vPkr.push_back(CPoint(nPickX, nPickY));
					}
				}
			}

			//BOOL bGrrDvcSwap = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrDeviceSwapUse);

			///////////////////////////////////////////////
			//////////GRR 구현 후, 추가 
			///////////////////////////////////////////////
			if (g_TaskSystemCtrl.GetAutoGrrUse() == TRUE /*&& bGrrDvcSwap == FALSE */ && nPickPlace == DEF_PLACE && (nTargetLocIdx == LOC_IDX_TRAY_PP_TBL_2 || nTargetLocIdx == LOC_IDX_TRAY_PP_TBL_3))
			{
				if (vPkr.size() > 0) {
					vPkr.clear();
					for (int nPickY = 0; nPickY < MAX_TRAY_PP_CNT_Y; nPickY++) {
						for (int nPickX = 0; nPickX < MAX_TRAY_PP_CNT_X; nPickX++) {
							if (nUseJobPkr[nPickY][nPickX] != 0) {
								vPkr.push_back(CPoint(nPickX, nPickY));
							}
						}
					}
				}

				int nJobPkrSize = vPkr.size();
				int nIdx = (nTargetLocIdx == LOC_IDX_TRAY_PP_TBL_2) ? 0 : 1;
				int nCountInsert = g_DMCont.m_dmTestPP.GN(NDM04_SITEGROUP1_INSERTED_DEVICE_CNT + nIdx/*(nTargetLocIdx - LOC_IDX_TRAY_PP_TBL_1)*/);
				int nSetInDvc = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrUnitCount);

				int nModule1On = g_TaskAutoTestPp[eTestPP_1].GetModuleSocketOnCnt(TRUE);
				int nModule2On = g_TaskAutoTestPp[eTestPP_2].GetModuleSocketOnCnt(TRUE);
				if (nModule1On > 0 && nModule2On > 0)
				{
					if (nCountInsert >= nSetInDvc / 2)
					{
						return;
					}
					else if (nCountInsert + (int)vPkr.size() > nSetInDvc / 2)
					{
						for (int i = 0; i < (nCountInsert + nJobPkrSize - nSetInDvc / 2); i++)
							vPkr.pop_back();
					}
				}
				if (vPkr.size() == 0)
					return;
			}
			else if (g_TaskSystemCtrl.GetAutoLoopUse() == TRUE /*&& bGrrDvcSwap == FALSE */ && nPickPlace == DEF_PLACE && (nTargetLocIdx == LOC_IDX_TRAY_PP_TBL_1 || nTargetLocIdx == LOC_IDX_TRAY_PP_TBL_2))
			{
				int nCountInsert = g_DMCont.m_dmTestPP.GN(NDM04_SITEGROUP1_INSERTED_DEVICE_CNT + (nTargetLocIdx - LOC_IDX_TRAY_PP_TBL_1));
				int nSetInDvc = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoLoopUnitCount);

				int nModule1On = g_TaskAutoTestPp[eTestPP_1].GetModuleSocketOnCnt();
				int nModule2On = g_TaskAutoTestPp[eTestPP_2].GetModuleSocketOnCnt();
				if (nModule1On > 0 && nModule2On > 0)
				{
					if (nCountInsert >= nSetInDvc / 2)
					{
						return;
					}
					else if (nCountInsert + (int)vPkr.size() > nSetInDvc / 2)
					{
						for (int i = 0; i < (nCountInsert + (int)vPkr.size() - nSetInDvc / 2); i++)
							vPkr.pop_back();
					}
				}
				if (vPkr.size() == 0)
					return;
			}

			/////////////////////////////////////////////////////////////

			CBaseSeqCmd* pCmd = nullptr;
			if( m_eTrayIdx == eTrayPP_1_LD )
			{
				pCmd = new CTrayLoadPPCmd_MovePickPlace();
				static_cast<CTrayLoadPPCmd_MovePickPlace*>(pCmd)->eStage     = nTargetLocIdx;
				static_cast<CTrayLoadPPCmd_MovePickPlace*>(pCmd)->vPicker    = vPkr;
				static_cast<CTrayLoadPPCmd_MovePickPlace*>(pCmd)->Pocket     = CPoint(nPockX, nPockY);
				static_cast<CTrayLoadPPCmd_MovePickPlace*>(pCmd)->ePickPlace = (ePPCmd)nPickPlace;
			}
			else if (m_eTrayIdx == eTrayPP_2_UD)
			{
				pCmd = new CTrayUnloadPPCmd_MovePickPlace();
				static_cast<CTrayUnloadPPCmd_MovePickPlace*>(pCmd)->m_Cmd.eStage     = nTargetLocIdx;
				static_cast<CTrayUnloadPPCmd_MovePickPlace*>(pCmd)->m_Cmd.vPicker    = vPkr;
				static_cast<CTrayUnloadPPCmd_MovePickPlace*>(pCmd)->m_Cmd.Pocket     = CPoint(nPockX, nPockY);
				static_cast<CTrayUnloadPPCmd_MovePickPlace*>(pCmd)->m_Cmd.ePickPlace = (ePPCmd)nPickPlace;
			}

			// find same command
			BOOL bSameCmd = FALSE;
			std::find_if(m_listTrayPpSchedule.begin(), m_listTrayPpSchedule.end()
				, [&](CBaseSeqCmd* _pTrayPpCmd) -> bool
			{

				CBaseSeqCmd* pTrayPpCmd = nullptr;
				if( m_eTrayIdx == eTrayPP_1_LD )
				{
					pTrayPpCmd = static_cast<CTrayLoadPPCmd_MovePickPlace*>(_pTrayPpCmd);
					if (static_cast<CTrayLoadPPCmd_MovePickPlace*>(pTrayPpCmd)->eStage == nTargetLocIdx && static_cast<CTrayLoadPPCmd_MovePickPlace*>(pTrayPpCmd)->Pocket == static_cast<CTrayLoadPPCmd_MovePickPlace*>(pCmd)->Pocket) {
						bSameCmd = TRUE;
						return true;
					}
				}
				else if (m_eTrayIdx == eTrayPP_2_UD)
				{
					pTrayPpCmd = static_cast<CTrayUnloadPPCmd_MovePickPlace*>(_pTrayPpCmd);
					if (static_cast<CTrayUnloadPPCmd_MovePickPlace*>(pTrayPpCmd)->m_Cmd.eStage == nTargetLocIdx && static_cast<CTrayUnloadPPCmd_MovePickPlace*>(pTrayPpCmd)->m_Cmd.Pocket == static_cast<CTrayUnloadPPCmd_MovePickPlace*>(pCmd)->m_Cmd.Pocket) {
						bSameCmd = TRUE;
						return true;
					}
				}
				return false;
			});

			// no search same command
			if (bSameCmd == FALSE) {
				m_listTrayPpSchedule.push_back(pCmd);
			}
			else {
				delete pCmd;
			}
		}
	}
}

/*=============================================================================================
	DESCRIPT : FillDvc_VirtualTable
	KEY WORD : loadtable, ART buffer table, Socket Cleaning Table 의 device 정보
	           ARGUMENT : nDvcStageIdx       = device stage 정보
	           pnBuffer           = buffer table device 정보
	           nBufSizeX          = Buffer size max X
	           nBufSizeY          = Buffer size max Y
	           bPickPlace         = Pick / Place 동작
	RETURN   : 없음
===============================================================================================*/
int CTaskAutoTrayBasePP::FillDvc_VirtualTable(eDeviceDataStage nDvcStageIdx, int* pnBuffer, int nBufSizeX, int nBufSizeY, int nPickPlace)
{
	int nPocketCount_X = 0;
	int nPocketCount_Y = 0;

	BOOL bLoadTable = FALSE;
	switch (nDvcStageIdx)
	{
		//  	case eDeviceStage_STACKER_CONV_1:
		//  	case eDeviceStage_STACKER_CONV_2:
		//  	case eDeviceStage_STACKER_CONV_3:
		//  	case eDeviceStage_STACKER_CONV_4:
		//  	case eDeviceStage_STACKER_CONV_5:
		//  	case eDeviceStage_STACKER_CONV_6:
		//  	case eDeviceStage_STACKER_CONV_7:
		//  	case eDeviceStage_TRAY_FEEDER:
		//  		{
		//  			nPocketCount_X = g_DMCont.m_dmTrayPP.GN( NDM5_TrayDiv_X );
		//  			nPocketCount_Y = g_DMCont.m_dmTrayPP.GN( NDM5_TrayDiv_Y );
		//  		}break;
	case eDeviceStage_LD_TBL_1:
	case eDeviceStage_LD_TBL_2:
	case eDeviceStage_LD_TBL_3:
	case eDeviceStage_LD_TBL_4:
	{
		nPocketCount_X = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
		nPocketCount_Y = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);
		bLoadTable = TRUE;
	}break;
	case eDeviceStage_TRAY_BUFF:
	{
		nPocketCount_X = 4;
		nPocketCount_Y = 4;
	}break;
	default:
		ASSERT(false);
		break;
	}

	int nNumberOfLoadTable = g_DMCont.m_dmHandlerOpt.GN(NDM11_NUMBER_OF_LOADTABLE);
	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	for (int nY = 0; nY < nPocketCount_Y; nY++)
	{
		for (int nX = 0; nX < nPocketCount_X; nX++)
		{
			int nBuffer_Index = (nX + VIRTUAL_POCKET_X_START) + ((nY + VIRTUAL_POCKET_Y_START) * nBufSizeX);

			// Load Table의 1,3열 LD, 2,4열 Buffer이므로 Buffer부분은 작업 못하도록 해야한다.
			if (bLoadTable == TRUE)
			{
				char setting = g_TaskLoadTbl[nDvcStageIdx - eDeviceStage_LD_TBL_1].m_LoadTbl_pocket_setting[nY][nX];
				if (setting == 'B')
				{
					int nStatus = (nPickPlace == DEF_PICK) ? DEF_NOT_EXIST : DEF_EXIST;
					pnBuffer[nBuffer_Index] = nStatus;
					continue;
				}

				//상태 반대로 해줘야 함.
				if (nPickPlace == DEF_PICK && setting == 'L') {
					pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
					continue;
				}

				if (nPickPlace == DEF_PLACE && setting == 'U') {
					pnBuffer[nBuffer_Index] = DEF_EXIST;
					continue;
				}
				// 4Para 2열만 Load 자재 담기위해 예외처리 추가. [8/5/2025 dohyeong.kim]
				if (nPickPlace == DEF_PLACE && setting == 'L') {
					if (nSiteDivY == 1 && nY == 3 && nNumberOfLoadTable == _eNumberOfLoadTable::eNumberOfLoadTable_4Unit) {
						pnBuffer[nBuffer_Index] = DEF_EXIST;
						continue;
					}
				}
// 				if (nPickPlace == DEF_PLACE && setting == 'L') {
// 					pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
// 					continue;
// 				}
			}

			int nExist = g_DMCont.m_dmDevice.GetDeviceExist(nDvcStageIdx, nX, nY);
			if (nExist == DEF_EXIST) pnBuffer[nBuffer_Index] = DEF_EXIST;
			else                      pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
		}
	}

	return ERR_NO_ERROR;
}


/*=============================================================================================
	DESCRIPT : FindPickPlaceFromTable
	KEY WORD : loadtable, ART buffer table, Socket Cleaning Table 의 작업
	           ARGUMENT : nLocIdx            = 작업 할 Loc Index
	           nTableDvc          = Table의 device 정보
	           nPickerDvcPattern  = hand 작업 가능 패턴
	           bPickPlace         = pick / place 동작
	           n_Xpos             = 찾은 작업 X 위치
	           n_Ypos             = 찾은 작업 Y 위치
	           nWorkPkr           = 찾은 작업 picker pattern
	RETURN   : 작업 가능 수
===============================================================================================*/
int CTaskAutoTrayBasePP::FindPickPlaceFromTable(int         nLocIdx,
	                                            int         nTableDvc[VIRTUAL_POCKET_Y_MAX][VIRTUAL_POCKET_X_MAX], 
	                                            int         nPickerPattern[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X], 
	                                            BOOL        bPickPlace, 
	                                            int *       n_Xpos, 
	                                            int *       n_YPos, 
	                                            int         nWorkPkr[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X],
												BOOL		bVacuumCheck											)
{
	ASSERT(LOC_IDX_TRAY_PP_TBL_1 <= nLocIdx && nLocIdx <= LOC_IDX_TRAY_PP_TBL_4);


	int nSearchY = 0, nSearchX = 0;
	int nSearchHandY = 0, nSearchHandX = 0;

	/*DEF_WORK_FREE or DEF_WORK_RESERVED*/
	int nArrHandWorkSource[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = { DEF_WORK_FREE, };
	int nArrHandWorkCopy[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = { DEF_WORK_FREE, };

	int nPocketCntX = 0, nPocketCntY = 0;

	int nPitchType_X = m_pTaskTrayPP->GetHandPitchType(CTaskTrayBasePP::eAxisPitchX, nLocIdx);
	int nPitchType_Y = m_pTaskTrayPP->GetHandPitchType(CTaskTrayBasePP::eAxisPitchY, nLocIdx);

	int nShtOffX = 1, nShtOffY = 1, nPitchOff_X = 1, nPitchOff_Y = 1;

	// hand 보다 working하는 곳의 pitch가 더 크다
	if (nPitchType_X < 0)
		nShtOffX = abs(nPitchType_X);
	else
		nPitchOff_X = nPitchType_X;

	// hand y pitch보다 클 경우는 one by one만 지원해야한다.
	if (nPitchType_Y < 0)
		ASSERT(0);

	nPitchOff_Y = nPitchType_Y;

	nPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	nPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);


	//	int nTotalPossibleCnt = 0;	// 가장 Pick 할 수 있는 수량
	int nCurrentCnt = 0;	// 현재 까지 Pick 할수 있는 수량
	int nTotalPossibleCnt = 0;	// 가장 Pick 할 수 있는 수량
	int nCXPosition = 0, nCYPosition = 0;
	// 1Y, 2Y는 계산 한 대로 가져 온다.	


	int nChkCleanBuffer = DEF_NOT_EXIST;
	int nHadData = DEF_NOT_EXIST;
	if (bPickPlace == DEF_PICK) {	// Pick 일 때 조건
		nChkCleanBuffer = DEF_EXIST;
		nHadData = DEF_NOT_EXIST;
	}
	else {							// Place 일 때 조건	
		nChkCleanBuffer = DEF_NOT_EXIST;
		nHadData = DEF_EXIST;
	}


	//nSearchY = VIRTUAL_POCKET_Y_START + 1;

// 	nSearchY = VIRTUAL_POCKET_Y_START - 2;
// 	nSearchX = VIRTUAL_POCKET_X_START - (nPocketCntX - 1);

	
//	memset(nArrHandWorkSource, 0, sizeof(nArrHandWorkSource));

	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nDvcFullCnt = (nSiteDivY == 1) ? (MAX_TRAY_PP_CNT_X * MAX_TRAY_PP_CNT_Y / 2) : MAX_TRAY_PP_CNT_X * MAX_TRAY_PP_CNT_Y;
	int dvc_cnt = g_TaskTrayLoadPP.GetTrayPpDvcCnt();
	//Load Hand는 좌상단 edge ~ 우하단 edge 탐색
	//Unload Hand는 Y열로만 탐색
	// Station 4Para일 경우에 Tray Load PP Place 가변 없이 Load Table에 1:1로 내려놓도록 변경 [8/13/2025 seungtaek.oh]
	int nStartX = (m_eTrayIdx == _eTrayPPIdx::eTrayPP_1_LD) ? VIRTUAL_POCKET_X_START - (nPocketCntX - 1) : VIRTUAL_POCKET_X_START;
	if (m_eTrayIdx == _eTrayPPIdx::eTrayPP_1_LD && nSiteDivY == 1 && dvc_cnt == nDvcFullCnt && bPickPlace == DEF_PLACE && g_TaskSystemCtrl.GetAutoGrrUse() == FALSE)
	{
		nStartX = VIRTUAL_POCKET_X_START;
	}

	int nEndX = (m_eTrayIdx == _eTrayPPIdx::eTrayPP_1_LD) ? VIRTUAL_POCKET_X_START + nPocketCntX : VIRTUAL_POCKET_X_START + 1;

	int nSkipYCnt = 0;

	for (nSearchY = VIRTUAL_POCKET_Y_START - 2; nSearchY <= VIRTUAL_POCKET_Y_START + nPocketCntY - 1; nSearchY++)
	{
		for (nSearchX = nStartX; nSearchX < nEndX; nSearchX++)
		{
			nCurrentCnt = 0;
			memset(nArrHandWorkSource, 0, sizeof(nArrHandWorkSource));

			//Picker 8ea 탐색
			for (nSearchHandY = 0; nSearchHandY < MAX_TRAY_PP_CNT_Y; nSearchHandY++)
			{
				for (nSearchHandX = 0; nSearchHandX < MAX_TRAY_PP_CNT_X; nSearchHandX++)
				{
					/*
					return 0 : one by one //내려 놨으면 다 있다고 판단하여 1개만 진행, 다음 Picker one by one 진행.
					return 1 : PItch에 맞게 동시에 pp 가능
					return

					*/
					int nYPocket = nSearchY + nSearchHandY * nPitchOff_Y; //nPitchOff_Y == 1
					int nXPocket = nSearchX + nSearchHandX * nPitchOff_X; //nPitchOff_X == 1

					int TableExist = nTableDvc[nYPocket][nXPocket];
					int PickerExist = nPickerPattern[nSearchHandY][nSearchHandX];
					if (TableExist == nChkCleanBuffer &&  PickerExist == nHadData)
					{
						nArrHandWorkSource[nSearchHandY][nSearchHandX] = DEF_WORK_RESERVED;
						nCurrentCnt++;

						if (nPitchType_Y == 0)
						{
							nSearchHandY = MAX_TRAY_PP_CNT_Y;
						}
						if (nPitchType_X == 0)
						{
							nSearchHandX = MAX_TRAY_PP_CNT_X;
						}
					}
				}
				nSkipYCnt++;
			}

			BOOL bLimitCheckOK = GetAreaProblem(nSearchX - VIRTUAL_POCKET_X_START, nSearchY - VIRTUAL_POCKET_Y_START, (eDeviceDataStage)(eDeviceStage_LD_TBL_1 + nLocIdx - LOC_IDX_TRAY_PP_TBL_1), nArrHandWorkSource); // todo slt : 영역 점검을 위한 함수 구현 필요.
			if (bLimitCheckOK == TRUE)
			{
				if (/*nCurrentCnt > 0*/nTotalPossibleCnt < nCurrentCnt)
				{
					//작업해라.
					nTotalPossibleCnt = nCurrentCnt;
					memcpy_s(nArrHandWorkCopy, sizeof(nArrHandWorkCopy), nArrHandWorkSource, sizeof(nArrHandWorkSource));
					nCXPosition = nSearchX;
					nCYPosition = nSearchY;
				}
			}
		}
	}



	memcpy_s(nWorkPkr, sizeof(nArrHandWorkCopy), nArrHandWorkCopy, sizeof(nArrHandWorkCopy));

	//0번 피커부터 할당되지 않은 포켓은 트림한다.
	int nPockOffsetX = 0;
	int nPockOffsetY = 1;

	//X방향 첫 할당 위치가 X옵셋
	for (; nPockOffsetX < MAX_TRAY_PP_CNT_X; nPockOffsetX++)
	{
		if (nWorkPkr[0][nPockOffsetX] != DEF_WORK_FREE) break;
		if (nWorkPkr[1][nPockOffsetX] != DEF_WORK_FREE) break;
	}
	//X방향 첫 할당 위치가 1개라도 있으면 Y옵셋은 0
	for (int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++)
	{
		if (nWorkPkr[0][nX] != DEF_WORK_FREE) {
			nPockOffsetY = 0;
			break;
		}
	}

	nCXPosition -= VIRTUAL_POCKET_X_START;
	nCYPosition -= VIRTUAL_POCKET_Y_START;

	//Feeder 뒷열 부터 Pick 할 경우,
	//Front Picker(Y==1)기준으로 Table Row Step 결정.
	//if (nCYPosition < 0) {
	if(bVacuumCheck == TRUE)
		g_TaskLoadTbl[nLocIdx].SetTableVacuumCheckRowStep(nCYPosition);
	else
		g_TaskLoadTbl[nLocIdx].SetTableRowStep(nCYPosition);
	//}

	//Loading Table Working Pocket
	*n_Xpos = nCXPosition + nPockOffsetX*nPitchOff_X;
	*n_YPos = nCYPosition + nPockOffsetY*nPitchOff_Y;


	return nTotalPossibleCnt;
}

BOOL CTaskAutoTrayBasePP::GetAreaProblem(int nTargetPosX, int nTargetPosY, eDeviceDataStage WorkBin, int nArrHandWorkSource[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X])
{
	//0번 피커부터 할당되지 않은 포켓은 트림한다.
	int nPockOffsetX = 0;
	int nPockOffsetY = 1;

	//X방향 첫 할당 위치가 X옵셋
	//Loading Table Work Pocket 확인.
	for (; nPockOffsetX < MAX_TRAY_PP_CNT_X; nPockOffsetX++)
	{
		if (nArrHandWorkSource[0][nPockOffsetX] != DEF_WORK_FREE) break;
		if (nArrHandWorkSource[1][nPockOffsetX] != DEF_WORK_FREE) break;
	}

	//X방향 첫 할당 위치가 1개라도 있으면 Y옵셋은 0
	for (int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++)
	{
		if (nArrHandWorkSource[0][nX] != DEF_WORK_FREE) {
			nPockOffsetY = 0;
			break;
		}
	}

	int nPitchType_X = 1, nPitchType_Y = 1;
	switch (WorkBin)
	{
	case eDeviceStage_STACKER_CONV_1:
	case eDeviceStage_STACKER_CONV_2:
	case eDeviceStage_STACKER_CONV_3:
	case eDeviceStage_STACKER_CONV_4:
	case eDeviceStage_STACKER_CONV_5:
	case eDeviceStage_STACKER_CONV_6:
	case eDeviceStage_STACKER_CONV_7:
	case eDeviceStage_TRAY_FEEDER:
	{
		nPitchType_X = m_pTaskTrayPP->GetHandPitchType(CTaskTrayBasePP::eAxisPitchX, LOC_IDX_TRAY_PP_CONV_1);
		nPitchType_Y = m_pTaskTrayPP->GetHandPitchType(CTaskTrayBasePP::eAxisPitchY, LOC_IDX_TRAY_PP_CONV_1);
	}break;
	case eDeviceStage_LD_TBL_1:
	case eDeviceStage_LD_TBL_2:
	case eDeviceStage_LD_TBL_3:
	case eDeviceStage_LD_TBL_4:
	{
		nPitchType_X = m_pTaskTrayPP->GetHandPitchType(CTaskTrayBasePP::eAxisPitchX, LOC_IDX_TRAY_PP_TBL_1);
		nPitchType_Y = m_pTaskTrayPP->GetHandPitchType(CTaskTrayBasePP::eAxisPitchY, LOC_IDX_TRAY_PP_TBL_1);
	}break;
	default: ASSERT(0);
		break;
	}

	int nShtOffX = 1, nPitchOff_X = 1;
	if (nPitchType_X < 0)
		nShtOffX = abs(nPitchType_X);
	else
		nPitchOff_X = nPitchType_X;

	// hand y pitch보다 클 경우는 one by one만 지원해야한다.
	if (nPitchType_Y < 0)
		ASSERT(0);

	int nPitchOff_Y = nPitchType_Y;

	int n_Xpos = nTargetPosX + (nPockOffsetX / nShtOffX) * nPitchOff_X;
	int n_YPos = nTargetPosY + nPockOffsetY * nPitchOff_Y;

	std::vector<CPoint> vPicker;
	for (int nPkrY = 0; nPkrY < MAX_TEST_PP_PKR_CNT_Y; nPkrY++)
	{
		for (int nPkrX = 0; nPkrX < MAX_TEST_PP_PKR_CNT_X; nPkrX++)
		{
			if (nArrHandWorkSource[nPkrY][nPkrX] == DEF_WORK_RESERVED)
				vPicker.push_back(CPoint(nPkrX, nPkrY));
		}
	}

	eLocIdxTrayPP nLoc = eLocIdxTrayPP::LOC_IDX_TRAY_PP_Undefined;
	switch (WorkBin)
	{
	case eDeviceStage_STACKER_CONV_1:	nLoc = LOC_IDX_TRAY_PP_CONV_1; break;
	case eDeviceStage_STACKER_CONV_2:	nLoc = LOC_IDX_TRAY_PP_CONV_2; break;
	case eDeviceStage_STACKER_CONV_3:	nLoc = LOC_IDX_TRAY_PP_CONV_3; break;
	case eDeviceStage_STACKER_CONV_4:	nLoc = LOC_IDX_TRAY_PP_CONV_4; break;
	case eDeviceStage_STACKER_CONV_5:	nLoc = LOC_IDX_TRAY_PP_CONV_5; break;
	case eDeviceStage_STACKER_CONV_6:	nLoc = LOC_IDX_TRAY_PP_CONV_6; break;
	case eDeviceStage_STACKER_CONV_7:	nLoc = LOC_IDX_TRAY_PP_CONV_7; break;
	case eDeviceStage_TRAY_FEEDER:		nLoc = LOC_IDX_TRAY_PP_FEEDER; break;
	case eDeviceStage_LD_TBL_1:			nLoc = LOC_IDX_TRAY_PP_TBL_1; break;
	case eDeviceStage_LD_TBL_2:			nLoc = LOC_IDX_TRAY_PP_TBL_2; break;
	case eDeviceStage_LD_TBL_3:			nLoc = LOC_IDX_TRAY_PP_TBL_3; break;
	case eDeviceStage_LD_TBL_4:			nLoc = LOC_IDX_TRAY_PP_TBL_4; break;
	default: ASSERT(0);
		break;
	}

	OneAxis* pMotorX = m_pTaskTrayPP->GetAxis(CTaskTrayBasePP::eAxisX);
	double dSW_NegaLimit_X = pMotorX->LimitValueCheck(FALSE);
	double dSW_PosiLimit_X = pMotorX->LimitValueCheck(TRUE);


	double dSW_NegaLimit_Y = -99999;
	double dSW_PosiLimit_Y = 99999;
	OneAxis* pMotorY = m_pTaskTrayPP->GetAxis(CTaskTrayBasePP::eAxisY);
	if (pMotorY != nullptr) {
		dSW_NegaLimit_Y = pMotorY->LimitValueCheck(FALSE);
		dSW_PosiLimit_Y = pMotorY->LimitValueCheck(TRUE);
	}

	double dTpos[CTaskTrayBasePP::eAxisMax] = { 0, };
	m_pTaskTrayPP->GetTeachPos(nLoc,
		                       CPoint(n_Xpos, n_YPos),
		                       vPicker,
		                       CTaskTrayBasePP::eZPOS_SAFETY,
		                       &dTpos[CTaskTrayBasePP::eAxisX],
		                       &dTpos[CTaskTrayBasePP::eAxisY],
		                       &dTpos[CTaskTrayBasePP::eAxisZ],
		                       &dTpos[CTaskTrayBasePP::eAxisPitchX],
		                       &dTpos[CTaskTrayBasePP::eAxisPitchY]);

	if (dTpos[CTaskTrayBasePP::eAxisX] > dSW_PosiLimit_X || dTpos[CTaskTrayBasePP::eAxisX] < dSW_NegaLimit_X)
		return FALSE;

	if ((dTpos[CTaskTrayBasePP::eAxisY] > dSW_PosiLimit_Y || dTpos[CTaskTrayBasePP::eAxisY] < dSW_NegaLimit_Y) && pMotorY != nullptr)
		return FALSE;

	return TRUE;
}

CBaseSeqCmd* CTaskAutoTrayBasePP::GetNextSchedule()
{
	CBaseSeqCmd* pReturnCmd = nullptr;

	if (m_listTrayPpSchedule.size() > 0)
	{
		pReturnCmd = m_listTrayPpSchedule.front();
		m_listTrayPpSchedule.pop_front();
	}

	return pReturnCmd;
}

/************************************************************************/
/* device 정보; empty, partial, full 조사 (loading table)                */
/************************************************************************/
eDeviceFullEmptyCheckCode CTaskAutoTrayBasePP:: LoadTableFullEmptyCheck(eDeviceDataStage DvcLoc/*, int nTrayPickPlace*//*=DEF_PICK,DEF_PLACE*/)
{
	ASSERT(DvcLoc >= eDeviceStage_LD_TBL_1 || DvcLoc <= eDeviceStage_LD_TBL_4);

	int nRemainDvcCount = 0;
	for (int nY = 0; nY < MAX_LD_TBL_POCKET_CNT_Y; nY++) {
		for (int nX = 0; nX < MAX_LD_TBL_POCKET_CNT_X; nX++) {

			char setting = g_TaskLoadTbl[DvcLoc - eDeviceStage_LD_TBL_1].m_LoadTbl_pocket_setting[nY][nX];
			bool device_exist = g_DMCont.m_dmDevice.GetDeviceExist(DvcLoc, nX, nY) == DEF_EXIST;
			if (setting == 'L' /*&& nTrayPickPlace == DEF_PLACE*/ && device_exist == true) {
				nRemainDvcCount++;
			}
			else if (setting == 'U' /*&& nTrayPickPlace == DEF_PICK*/ && device_exist == true) {
				nRemainDvcCount++;
			}
		}
	}

	// 4Para 2열만 Load 자재 담기위해 예외처리 추가. [8/5/2025 dohyeong.kim]
	int nNumberOfLoadTable = g_DMCont.m_dmHandlerOpt.GN(NDM11_NUMBER_OF_LOADTABLE);
	int nTblIdx = DvcLoc - eDeviceStage_LD_TBL_1;
	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nFullCount = (MAX_LD_TBL_POCKET_CNT_Y / 2)*MAX_LD_TBL_POCKET_CNT_X;
	if (nSiteDivY == 1 && nNumberOfLoadTable == _eNumberOfLoadTable::eNumberOfLoadTable_4Unit &&
		(nTblIdx == eLoadTbl_2 || nTblIdx == eLoadTbl_3)) {
		nFullCount = ((MAX_LD_TBL_POCKET_CNT_Y / 2)*MAX_LD_TBL_POCKET_CNT_X)/2;
	}

	if (nRemainDvcCount == 0) {
		return eEmpty;
	}
	else if (nRemainDvcCount == nFullCount/*((MAX_LD_TBL_POCKET_CNT_Y /2 )*MAX_LD_TBL_POCKET_CNT_X)*/) {
		return eFull;
	}

	return ePatial;
}

void CTaskAutoTrayBasePP::LoadingTblSetReserve()
{
	BOOL bIsEmpty               = FALSE;
	BOOL bIsFull                = FALSE;
	int nPickPlace              = -1;
						        
	int nDeviceStage_START      = eDeviceStage_Undefine;
	int nDeviceStage_End        = eDeviceStage_Undefine;

	std::vector<int> vLoadTblDevStage;
	vLoadTblDevStage.clear();

	switch (m_eTrayIdx) 
	{ 
		case _eTrayPPIdx::eTrayPP_1_LD: 
		{			
			vLoadTblDevStage.push_back(eDeviceStage_LD_TBL_2);
			vLoadTblDevStage.push_back(eDeviceStage_LD_TBL_3);
			nPickPlace         = DEF_PLACE;
		}break;
		
		case _eTrayPPIdx::eTrayPP_2_UD: 
		{
			vLoadTblDevStage.push_back(eDeviceStage_LD_TBL_1);
			vLoadTblDevStage.push_back(eDeviceStage_LD_TBL_4);
			nPickPlace         = DEF_PICK;
		}break;
	}
	
	for (auto nLdTblDvcLoc : vLoadTblDevStage)
	{
		eDeviceFullEmptyCheckCode tblDeviceFullEmpty = LoadTableFullEmptyCheck((eDeviceDataStage)nLdTblDvcLoc);
		_eLoadTblIdx nLdTblObjIdx                    = (_eLoadTblIdx)((eLoadTbl_1)+(nLdTblDvcLoc - eDeviceStage_LD_TBL_1));
		_eTestPPIdx nTestSeqIdx                      = (_eTestPPIdx)((eTestPP_1)+(nLdTblDvcLoc - eDeviceStage_LD_TBL_1) / 2);
		BOOL tbl_work_reserve_tray_site              = (g_TaskLoadTbl[nLdTblObjIdx].GetWorkReserve() == RID_TRAY_PP_(RIDX_TRAY_PP_1 + m_eTrayIdx)) ? TRUE : FALSE;

		int device_cnt_in_all_dut = 0;
		std::vector< _ePressUnit > presses = m_test_modules_press_idx[nTestSeqIdx];

		for each(auto press_idx in presses) {
			device_cnt_in_all_dut += g_TaskPressUnit[press_idx].ChkSiteInDvc();
		}
		device_cnt_in_all_dut += g_TaskTestPP[nTestSeqIdx].GetBufferTableCnt();
		int nBufferDevCnt  = g_TaskTestPP[nTestSeqIdx].GetBufferTableCnt();
		BOOL bLoadingBreak = FALSE;
		int DvcCnt         = (m_eTrayIdx == eTrayPP_1_LD) ? g_TaskTrayLoadPP.GetTrayPpLoadDvcCnt() : g_TaskTrayUnloadPP.GetTrayPpUnloadDvcCnt();
 		BOOL bHandHasDvc   = (DvcCnt > 0 ? TRUE : FALSE);//( get_load_picker_exist_mask() != 0 ) ? TRUE : FALSE;
		BOOL bHasTestSdDvc = (device_cnt_in_all_dut > 0) || (g_TaskTestPP[nTestSeqIdx].GetUnloadPickerHaveDvcCnt() > 0 || g_TaskTestPP[nTestSeqIdx].GetLoadPickerDeviceCnt()) ? TRUE : FALSE;


		///////////////////////////////////////////////
		//////////GRR 구현 후, 추가 
		///////////////////////////////////////////////
		if (g_TaskSystemCtrl.GetAutoGrrUse() == TRUE) {
			int nTblType = g_TaskLoadTbl[nLdTblObjIdx].GetTableUsage();
			if (g_TaskSystemCtrl.GetAutoGrrCancel() == TRUE) {
				bLoadingBreak = TRUE;
			}
			else {
				bLoadingBreak = g_TaskAllAutoStacker.GetDevicePickEnableStackerCount() == 0 ? TRUE : FALSE;
				int tray_pp_status = (m_eTrayIdx == eTrayPP_1_LD) ? g_TaskTrayLoadPP.GetSeqCmdStatus() : g_TaskTrayUnloadPP.GetSeqCmdStatus();
				if (bHandHasDvc == TRUE || (CBaseSeqCmd::eSeqCmdStatus)tray_pp_status != CBaseSeqCmd::eSeqCmdStatus_Ready)
					bLoadingBreak = FALSE;
			}

			int nModule1On = g_TaskAutoTestPp[eTestPP_1].GetModuleSocketOnCnt(TRUE);
			int nModule2On = g_TaskAutoTestPp[eTestPP_2].GetModuleSocketOnCnt(TRUE);
			int nReservedDvcCnt = g_DMCont.m_dmTestPP.GN(NDM04_SITEGROUP1_INSERTED_DEVICE_CNT + nLdTblObjIdx);
			int nSetInDvc = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrUnitCount);
			if (nModule1On > 0 && nModule2On > 0)
			{
				nSetInDvc /= 2;
			}
			if (nReservedDvcCnt >= nSetInDvc) {
				bLoadingBreak = TRUE;
				bHandHasDvc = FALSE;
			}
		
			//Load Table Device까지 Check(투입된 Device 있는지)
			int load_device_cnt = (nTestSeqIdx == eTestPP_1) ? g_TaskLoadTbl[eLoadTbl_2].GetLoadDeviceCnt() : g_TaskLoadTbl[eLoadTbl_3].GetLoadDeviceCnt();
			if (load_device_cnt > 0) {
				bHasTestSdDvc = TRUE;
			}
		}
		else {
			bLoadingBreak = g_TaskAllAutoStacker.GetLoadTraySupplyEnd();
		}

		//Load Table Device까지 Check(투입된 Device 있는지)
		int load_device_cnt = (nTestSeqIdx == eTestPP_1) ? g_TaskLoadTbl[eLoadTbl_2].GetLoadDeviceCnt() : g_TaskLoadTbl[eLoadTbl_3].GetLoadDeviceCnt();
		if (load_device_cnt > 0 /*|| g_TaskTrayLoadPP.GetTrayPpLoadDvcCnt() > 0*/) {
			bHasTestSdDvc = TRUE;
		}

		//Unload Table Device Check
		int unload_device_cnt = (nTestSeqIdx == eTestPP_1) ? g_TaskLoadTbl[eLoadTbl_1].GetUnloadDeviceCnt() : g_TaskLoadTbl[eLoadTbl_4].GetUnloadDeviceCnt();
		if (unload_device_cnt > 0) {
			bHasTestSdDvc = TRUE;
		}

		
		BOOL bAutoQAUse = g_TaskSystemCtrl.GetAutoQAUse();
		int nQAStatus = g_TaskSystemCtrl.GetQAStatus();

		int nQaSampleCnt = g_TaskSystemCtrl.GetQASampleCount();
		int nLotLoadCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt);  //투입 된 수량

		///////////////////////////////////////////////////////////////  
// 		//[6/26/2023 dohyeong.kim]Grr 구현 전, 임시
// 		bLoadingBreak = g_TaskAllAutoStacker.GetLoadTraySupplyEnd();
		///////////////////////////////////////////////////////////////
		bool feeder_tray_empty_device = g_TaskTrayFeeder.IsEmptyDevice();

		int load_enable_stacker_count = g_TaskAllAutoStacker.GetLoadEnableStackerCount();
		if (tbl_work_reserve_tray_site)
		{
			//Load Table, Place
			if (nLdTblDvcLoc == eDeviceStage_LD_TBL_2 || nLdTblDvcLoc == eDeviceStage_LD_TBL_3) { //bLoadingBreak true, false 상관 없이 Full이면 reserve
				if (tblDeviceFullEmpty == eFull) {
					g_TaskLoadTbl[nLdTblObjIdx].SetWorkReserve(RID_NOT_RESERVE);
				}
				else if (bLoadingBreak && tblDeviceFullEmpty == ePatial && feeder_tray_empty_device == true) { //C-Tray Load 멈춤, Loading Table Partial
					g_TaskLoadTbl[nLdTblObjIdx].SetWorkReserve(RID_NOT_RESERVE);
				}
				else if (g_TaskSystemCtrl.IsOneCycleModeOn() == true && tblDeviceFullEmpty != eEmpty ) { //OneCycle Mode인데 Tray Empty가 아니면 reserve
					g_TaskLoadTbl[nLdTblObjIdx].SetWorkReserve(RID_NOT_RESERVE);
				}			
				else if (g_TaskSystemCtrl.IsOneCycleModeOn() == true && bHasTestSdDvc > 0) { //OneCycle Mode인데 Test Site에 Unload 자재 있을 경우.
					g_TaskLoadTbl[nLdTblObjIdx].SetWorkReserve(RID_NOT_RESERVE);
				}
				else if ((nBufferDevCnt > 0 || bHasTestSdDvc) && g_TaskTrayLoadPP.GetTrayPpLoadDvcCnt() == 0 && g_TaskSystemCtrl.IsOneCycleModeOn() == true) { // AAB 때문에 Load 자재 투입 할게 없어도 Buffer Table 용도로 인하여 reserve
					g_TaskLoadTbl[nLdTblObjIdx].SetWorkReserve(RID_NOT_RESERVE);
				}
				else if ((nBufferDevCnt > 0 || bHasTestSdDvc) && g_TaskTrayLoadPP.GetTrayPpLoadDvcCnt() == 0 && (bLoadingBreak && g_TaskAllAutoStacker.GetLoadEnableStackerCount() == 0)) {// AAB 때문에 Load 자재 투입 할게 없어도 Buffer Table 용도로 인하여 reserve
					g_TaskLoadTbl[nLdTblObjIdx].SetWorkReserve(RID_NOT_RESERVE);
				}
				else if ((nBufferDevCnt > 0 || bHasTestSdDvc) && g_TaskTrayLoadPP.GetTrayPpLoadDvcCnt() == 0 && (bLoadingBreak && g_TaskSystemCtrl.GetAutoGrrUse() == TRUE)) {
					g_TaskLoadTbl[nLdTblObjIdx].SetWorkReserve(RID_NOT_RESERVE);
				}
				else if ((nBufferDevCnt > 0 || bHasTestSdDvc) && g_TaskTrayLoadPP.GetTrayPpLoadDvcCnt() == 0 && (bLoadingBreak && bAutoQAUse == TRUE)) {// AAB 때문에 Load 자재 투입 할게 없어도 Buffer Table 용도로 인하여 reserve
					g_TaskLoadTbl[nLdTblObjIdx].SetWorkReserve(RID_NOT_RESERVE);
				}
				else if (g_TaskSystemCtrl.GetAutoGrrCancel() == TRUE && bHandHasDvc != TRUE/*(tblDeviceFullEmpty != eEmpty|| bHandHasDvc != TRUE)*/) { //Load Hand가 Device Pick 하고 있을 경우
					g_TaskLoadTbl[nLdTblObjIdx].SetWorkReserve(RID_NOT_RESERVE);
				}
				else if(bLoadingBreak && bHasTestSdDvc  && g_TaskTrayLoadPP.GetTrayPpLoadDvcCnt() == 0 && bAutoQAUse == TRUE && nQAStatus != eQAStatus::eQAStatus_End)
					g_TaskLoadTbl[nLdTblObjIdx].SetWorkReserve(RID_NOT_RESERVE);

				else if (nQaSampleCnt == nLotLoadCnt && g_TaskTrayLoadPP.GetTrayPpLoadDvcCnt() == 0 && bAutoQAUse == TRUE && nQAStatus == eQAStatus::eQAStatus_Load)
					g_TaskLoadTbl[nLdTblObjIdx].SetWorkReserve(RID_NOT_RESERVE);
			
			}
			else {
				if (bLoadingBreak && tblDeviceFullEmpty == eEmpty && bHasTestSdDvc) { //C-Tray Load 멈춤, Loading Table Full Empty, Test Hand가 Unload 자재 들고 있으면 reserve.
					g_TaskLoadTbl[nLdTblObjIdx].SetWorkReserve(RID_NOT_RESERVE);
				}
				else if (!bLoadingBreak && tblDeviceFullEmpty == eEmpty) { //Lot 진행 중, Full Empty이면 reserve
					g_TaskLoadTbl[nLdTblObjIdx].SetWorkReserve(RID_NOT_RESERVE);
				}
				else if (g_TaskSystemCtrl.IsOneCycleModeOn() == true && tblDeviceFullEmpty == eEmpty && bHasTestSdDvc) { // OneCycle인데 Full Empty이며 Test Site 자재 있으면 reserve.
					g_TaskLoadTbl[nLdTblObjIdx].SetWorkReserve(RID_NOT_RESERVE);
				}
				else if (g_TaskSystemCtrl.GetAutoGrrCancel() == TRUE && tblDeviceFullEmpty == eEmpty && bHasTestSdDvc) {
					g_TaskLoadTbl[nLdTblObjIdx].SetWorkReserve(RID_NOT_RESERVE);
				}
			}
		}
	}
}



int CTaskAutoTrayBasePP::MakeTrayPPScheduleGrr(int nHandType)
{
	int nCntLoadPicker = 0, nCntUnloadPicker = 0;
	for (int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++)
	{
		for (int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++)
		{
			char setting = (nHandType == eTrayPP_1_LD) ? g_TaskTrayLoadPP.m_TrayPP_picker_setting[nY][nX] : g_TaskTrayUnloadPP.m_TrayPP_picker_setting[nY][nX];
			int nLoc = (nHandType == eTrayPP_1_LD) ? eDeviceStage_TRAY_PP_1 : eDeviceStage_TRAY_PP_2;
			bool device_exist = g_DMCont.m_dmDevice.GetDeviceExist(nLoc, nX, nY) == DEF_EXIST;

			if (setting == 'L' && device_exist == true)
			{
				nCntLoadPicker++;
			}
			else if (setting == 'U' && device_exist == true)
			{
				nCntUnloadPicker++;
			}
		}
	}

	ASSERT(nCntLoadPicker >= 0 && nCntUnloadPicker >= 0);

	int nPocketCntX = MAX_TRAY_PP_CNT_X;
	int nPocketCntY = MAX_TRAY_PP_CNT_Y;
	int nTblLoadDeviceCnt = 0, nTblUnloadDeviceCnt = 0;
	int nTableIndex = -1;
	int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);

	if (nHandType == eTrayPP_1_LD)
	{
		if (nCntLoadPicker == 0)
		{
			bool bDevices_remain_to_be_supplied = false;
			if (g_DMCont.m_dmDevice.IsEmptyTray(eDeviceStage_TRAY_FEEDER) == false){
				bDevices_remain_to_be_supplied = true;
			}
			
			// 공급할게 남아 있는 경우
			if (bDevices_remain_to_be_supplied == true && g_TaskSystemCtrl.GetAutoGrrCancel() == FALSE)
			{

			}
			// 공급이 종료된 경우
			else if(bDevices_remain_to_be_supplied == false && g_TaskSystemCtrl.GetAutoGrrCancel() == TRUE)
			{

			}




		}
		else //nCntLoadPicker > 0
		{

		}



	}
	else //nHandType == eTrayPP_2_UD
	{

	}
	return ERR_NO_ERROR;
}


int CTaskAutoTrayBasePP::_MakeScheduleGrr_TrayPp_Pick_Feeder()
{
	return 0;
}


int CTaskAutoTrayBasePP::SetVacuumCheck_TrayPP_Schedule(int nPickPlace, _eUsePicker _picker_type)
{
	int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);

	if (g_DMCont.m_dmShuttle.GB(BDM3_UI_TABLE_VACUUM_CHECK_ONOFF) == FALSE || nDeviceMode != eDeviceMode)
	{
		return eExcutingStep_ErrorJob;
	}

	if (m_pTaskTrayPP->GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
	{
		return eExcutingStep_WaitOtherJob;
	}

	
	
	bool Available = false;
	int nPickerDvcPattern[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = { 0, };

	eDeviceDataStage stage;
	std::vector<int> vLoadTblStage;
	vLoadTblStage.clear();
	int nTrayPPIdx;

	int nTrayPPLoadDeviceCount = m_pTaskTrayPP->GetTrayPpDvcCnt();
	
	switch (m_eTrayIdx)
	{
	case _eTrayPPIdx::eTrayPP_1_LD:
	{
		vLoadTblStage.push_back(LOC_IDX_TRAY_PP_TBL_2);
		vLoadTblStage.push_back(LOC_IDX_TRAY_PP_TBL_3);
		nTrayPPIdx = RID_TRAY_PP_(eTrayPP_1_LD);		
	}break;

	case _eTrayPPIdx::eTrayPP_2_UD:
	{
		vLoadTblStage.push_back(LOC_IDX_TRAY_PP_TBL_1);
		vLoadTblStage.push_back(LOC_IDX_TRAY_PP_TBL_4);
		nTrayPPIdx = RID_TRAY_PP_(eTrayPP_2_UD);
	}break;
	}

	BOOL bTableDeivceIn = FALSE;
	for (auto nLdTblDvcLoc : vLoadTblStage)
	{
		bTableDeivceIn = g_TaskLoadTbl[nLdTblDvcLoc].GetTableDeviceInCnt();
		if (bTableDeivceIn == TRUE)
			break;
	}

	eLocIdxTrayPP nTargetLocIdx = LOC_IDX_TRAY_PP_Undefined;

	int nRetLoadTblWorkIdx = MakeLoadTblWorkingIdx();
	int nComp = 0;
	
	
	for (auto nLdTblDvcLoc : vLoadTblStage)
	{
		int nRowStep = g_TaskLoadTbl[nLdTblDvcLoc].GetTableRowStep();
		int nMotorLoadPos = g_TaskLoadTbl[nLdTblDvcLoc].ChkMotorPos(LOC_IDX_TBL_LD_SIDE, CTaskLoadTable::eAxisY, nRowStep);
		if (g_TaskLoadTbl[nLdTblDvcLoc].Get_Table_DM_Vacuum_All_Check() == FALSE)
		{
			if (g_TaskAutoTestPp[nLdTblDvcLoc / 2].ModuleUseAbleCheck() != TRUE)
			{
				nComp++;
				continue;
			}
			if (nMotorLoadPos == ERR_NO_ERROR && g_TaskLoadTbl[nLdTblDvcLoc].GetWorkReserve() == nTrayPPIdx && g_TaskAutoTestPp[nLdTblDvcLoc / 2].ModuleUseAbleCheck() == TRUE)
			{
				nTargetLocIdx = (eLocIdxTrayPP)nLdTblDvcLoc;
				break;
			}
			break;
		}		
		else if(g_TaskLoadTbl[nLdTblDvcLoc].Get_Table_DM_Vacuum_All_Check() == TRUE)
		{
			nComp++;
		}
	}

	// Tray에 Load Device Count가 1개 이상이 거나, Table에 Device Count가 1개 이상이면 진행 하지 않는다.
	// Lot 가동 중 Moudle On 진행 하는 문제로 [5/13/2025 donghyun.shin]
	if(nTrayPPLoadDeviceCount > 0 || bTableDeivceIn == TRUE)
		return eExcutingStep_ErrorJob;

	if (nComp == vLoadTblStage.size()) // 모두 Check 했으면 Error Job
		return eExcutingStep_ErrorJob;

	if (nTargetLocIdx == LOC_IDX_TRAY_PP_Undefined) // Table이 도착 하지 않았으면 WaitOtherJob (대기)
		return eExcutingStep_WaitOtherJob;

	g_DMCont.m_dmShuttle.SB(BDM3_TABLE_LOAD_VACUUM_CHECK + _picker_type, TRUE);

	switch (nTargetLocIdx) {
	case LOC_IDX_TRAY_PP_TBL_1:
	case LOC_IDX_TRAY_PP_TBL_2:
	case LOC_IDX_TRAY_PP_TBL_3:
	case LOC_IDX_TRAY_PP_TBL_4:
	{
		stage = (eDeviceDataStage)(eDeviceStage_LD_TBL_1 + (nTargetLocIdx - LOC_IDX_TRAY_PP_TBL_1));

		for (int nPkrY = 0; nPkrY < MAX_TRAY_PP_CNT_Y; nPkrY++)
		{
			for (int nPkrX = 0; nPkrX < MAX_TRAY_PP_CNT_X; nPkrX++)
			{			
				nPickerDvcPattern[nPkrY][nPkrX] = FALSE;
				Available = true;
			}
		}
	}break;
	default:
		ASSERT(false);
		break;
	}

	int nArrVirtualTbl[VIRTUAL_POCKET_Y_MAX][VIRTUAL_POCKET_X_MAX];
	memset(nArrVirtualTbl, nPickPlace == DEF_PICK ? 0x00 : 0xFF, sizeof(nArrVirtualTbl));
	FillDvc_VacuumCheck_VirtualTable(stage, (int*)nArrVirtualTbl, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, nPickPlace);

	if (Available) {
		int nUseJobPkr[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = { 0, };
		int nPockX = -1, nPockY = -1;
		int nRetCode = FindPickPlaceFromTable(nTargetLocIdx, nArrVirtualTbl, nPickerDvcPattern, nPickPlace, &nPockX, &nPockY, nUseJobPkr, _picker_type == _eUsePicker_Load ? TRUE : FALSE);
		if (nRetCode > 0) {
			std::vector<CPoint> vPkr;
			for (int nPickX = 0; nPickX < MAX_TEST_PP_PKR_CNT_X; nPickX++) {
				for (int nPickY = 0; nPickY < MAX_TEST_PP_PKR_CNT_Y; nPickY++) {
					if (nUseJobPkr[nPickY][nPickX] != 0) {
						vPkr.push_back(CPoint(nPickX, nPickY));
					}
				}
			}

			CBaseSeqCmd* pCmd = nullptr;
			if (m_eTrayIdx == eTrayPP_1_LD)
			{
				pCmd = new CTrayLoadPPCmd_MovePickPlace();
				static_cast<CTrayLoadPPCmd_MovePickPlace*>(pCmd)->eStage = nTargetLocIdx;
				static_cast<CTrayLoadPPCmd_MovePickPlace*>(pCmd)->vPicker = vPkr;
				static_cast<CTrayLoadPPCmd_MovePickPlace*>(pCmd)->Pocket = CPoint(nPockX, nPockY);
				static_cast<CTrayLoadPPCmd_MovePickPlace*>(pCmd)->ePickPlace = (ePPCmd)nPickPlace;
			}
			else if (m_eTrayIdx == eTrayPP_2_UD)
			{
				pCmd = new CTrayUnloadPPCmd_MovePickPlace();
				static_cast<CTrayUnloadPPCmd_MovePickPlace*>(pCmd)->m_Cmd.eStage = nTargetLocIdx;
				static_cast<CTrayUnloadPPCmd_MovePickPlace*>(pCmd)->m_Cmd.vPicker = vPkr;
				static_cast<CTrayUnloadPPCmd_MovePickPlace*>(pCmd)->m_Cmd.Pocket = CPoint(nPockX, nPockY);
				static_cast<CTrayUnloadPPCmd_MovePickPlace*>(pCmd)->m_Cmd.ePickPlace = (ePPCmd)nPickPlace;
			}

			// find same command
			BOOL bSameCmd = FALSE;
			std::find_if(m_listTrayPpSchedule.begin(), m_listTrayPpSchedule.end()
				, [&](CBaseSeqCmd* _pTrayPpCmd) -> bool
			{

				CBaseSeqCmd* pTrayPpCmd = nullptr;
				if (m_eTrayIdx == eTrayPP_1_LD)
				{
					pTrayPpCmd = static_cast<CTrayLoadPPCmd_MovePickPlace*>(_pTrayPpCmd);
					if (static_cast<CTrayLoadPPCmd_MovePickPlace*>(pTrayPpCmd)->eStage == nTargetLocIdx && static_cast<CTrayLoadPPCmd_MovePickPlace*>(pTrayPpCmd)->Pocket == static_cast<CTrayLoadPPCmd_MovePickPlace*>(pCmd)->Pocket) {
						bSameCmd = TRUE;
						return true;
					}
				}
				else if (m_eTrayIdx == eTrayPP_2_UD)
				{
					pTrayPpCmd = static_cast<CTrayUnloadPPCmd_MovePickPlace*>(_pTrayPpCmd);
					if (static_cast<CTrayUnloadPPCmd_MovePickPlace*>(pTrayPpCmd)->m_Cmd.eStage == nTargetLocIdx && static_cast<CTrayUnloadPPCmd_MovePickPlace*>(pTrayPpCmd)->m_Cmd.Pocket == static_cast<CTrayUnloadPPCmd_MovePickPlace*>(pCmd)->m_Cmd.Pocket) {
						bSameCmd = TRUE;
						return true;
					}
				}
				return false;
			});

			// no search same command
			if (bSameCmd == FALSE) {
				m_listTrayPpSchedule.push_back(pCmd);
			}
			else {
				delete pCmd;
			}
		}
	}

	int nRowStep = g_TaskLoadTbl[nTargetLocIdx].GetTableRowStep();
	int nMotorLoadPos = g_TaskLoadTbl[nTargetLocIdx].ChkMotorPos(LOC_IDX_TBL_LD_SIDE, CTaskLoadTable::eAxisY, nRowStep);
	if (nMotorLoadPos != ERR_NO_ERROR)
	{
		return eExcutingStep_WaitOtherJob;
	}

	return eExcutingStep_FindJob;
}

int CTaskAutoTrayBasePP::FillDvc_VacuumCheck_VirtualTable(eDeviceDataStage nDvcStageIdx, int* pnBuffer, int nBufSizeX, int nBufSizeY, int nPickPlace)
{
	int nPocketCount_X = 0;
	int nPocketCount_Y = 0;

	BOOL bLoadTable = FALSE;
	switch (nDvcStageIdx)
	{
	case eDeviceStage_LD_TBL_1:
	case eDeviceStage_LD_TBL_2:
	case eDeviceStage_LD_TBL_3:
	case eDeviceStage_LD_TBL_4:
	{
		nPocketCount_X = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
		nPocketCount_Y = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);
		bLoadTable = TRUE;
	}break;
	default:
		ASSERT(false);
		break;
	}

	for (int nY = 0; nY < nPocketCount_Y; nY++)
	{
		for (int nX = 0; nX < nPocketCount_X; nX++)
		{
			int nBuffer_Index = (nX + VIRTUAL_POCKET_X_START) + ((nY + VIRTUAL_POCKET_Y_START) * nBufSizeX);
			int nTableExist = g_TaskLoadTbl[nDvcStageIdx - eDeviceStage_LD_TBL_1].Get_Table_DM_Vacuum_Check(nX, nY);
			if (nTableExist == FALSE) pnBuffer[nBuffer_Index] = DEF_EXIST;
			else                      pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
		}
	}

	return ERR_NO_ERROR;
}

int CTaskAutoTrayBasePP::FindVacuumCheckFromTable(int         nLocIdx,
	int         nTableDvc[VIRTUAL_POCKET_Y_MAX][VIRTUAL_POCKET_X_MAX],
	int         nPickerPattern[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X],
	BOOL        bPickPlace,
	int *       n_Xpos,
	int *       n_YPos,
	int         nWorkPkr[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X])
{
	ASSERT(LOC_IDX_TRAY_PP_TBL_1 <= nLocIdx && nLocIdx <= LOC_IDX_TRAY_PP_TBL_4);


	int nSearchY = 0, nSearchX = 0;
	int nSearchHandY = 0, nSearchHandX = 0;

	/*DEF_WORK_FREE or DEF_WORK_RESERVED*/
	int nArrHandWorkSource[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = { DEF_WORK_FREE, };
	int nArrHandWorkCopy[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = { DEF_WORK_FREE, };

	int nPocketCntX = 0, nPocketCntY = 0;

	int nPitchType_X = m_pTaskTrayPP->GetHandPitchType(CTaskTrayBasePP::eAxisPitchX, nLocIdx);
	int nPitchType_Y = m_pTaskTrayPP->GetHandPitchType(CTaskTrayBasePP::eAxisPitchY, nLocIdx);

	int nShtOffX = 1, nShtOffY = 1, nPitchOff_X = 1, nPitchOff_Y = 1;

	// hand 보다 working하는 곳의 pitch가 더 크다
	if (nPitchType_X < 0)
		nShtOffX = abs(nPitchType_X);
	else
		nPitchOff_X = nPitchType_X;

	// hand y pitch보다 클 경우는 one by one만 지원해야한다.
	if (nPitchType_Y < 0)
		ASSERT(0);

	nPitchOff_Y = nPitchType_Y;

	nPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	nPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);


	//	int nTotalPossibleCnt = 0;	// 가장 Pick 할 수 있는 수량
	int nCurrentCnt = 0;	// 현재 까지 Pick 할수 있는 수량
	int nTotalPossibleCnt = 0;	// 가장 Pick 할 수 있는 수량
	int nCXPosition = 0, nCYPosition = 0;
	// 1Y, 2Y는 계산 한 대로 가져 온다.	


	int nChkCleanBuffer = DEF_NOT_EXIST;
	int nHadData = DEF_NOT_EXIST;
	if (bPickPlace == DEF_PICK) {	// Pick 일 때 조건
		nChkCleanBuffer = DEF_EXIST;
		nHadData = DEF_NOT_EXIST;
	}
	else {							// Place 일 때 조건	
		nChkCleanBuffer = DEF_NOT_EXIST;
		nHadData = DEF_EXIST;
	}


	//nSearchY = VIRTUAL_POCKET_Y_START + 1;

	// 	nSearchY = VIRTUAL_POCKET_Y_START - 2;
	// 	nSearchX = VIRTUAL_POCKET_X_START - (nPocketCntX - 1);


	//	memset(nArrHandWorkSource, 0, sizeof(nArrHandWorkSource));


	//Load Hand는 좌상단 edge ~ 우하단 edge 탐색
	//Unload Hand는 Y열로만 탐색
	int nStartX = (m_eTrayIdx == _eTrayPPIdx::eTrayPP_1_LD) ? VIRTUAL_POCKET_X_START - (nPocketCntX - 1) : VIRTUAL_POCKET_X_START;
	int nEndX = (m_eTrayIdx == _eTrayPPIdx::eTrayPP_1_LD) ? VIRTUAL_POCKET_X_START + nPocketCntX : VIRTUAL_POCKET_X_START + 1;

	int nSkipYCnt = 0;

	for (nSearchY = VIRTUAL_POCKET_Y_START - 2; nSearchY <= VIRTUAL_POCKET_Y_START + nPocketCntY - 1; nSearchY++)
	{
		for (nSearchX = nStartX; nSearchX < nEndX; nSearchX++)
		{
			nCurrentCnt = 0;
			memset(nArrHandWorkSource, 0, sizeof(nArrHandWorkSource));

			//Picker 8ea 탐색
			for (nSearchHandY = 0; nSearchHandY < MAX_TRAY_PP_CNT_Y; nSearchHandY++)
			{
				for (nSearchHandX = 0; nSearchHandX < MAX_TRAY_PP_CNT_X; nSearchHandX++)
				{
					/*
					return 0 : one by one //내려 놨으면 다 있다고 판단하여 1개만 진행, 다음 Picker one by one 진행.
					return 1 : PItch에 맞게 동시에 pp 가능
					return

					*/
					int nYPocket = nSearchY + nSearchHandY * nPitchOff_Y; //nPitchOff_Y == 1
					int nXPocket = nSearchX + nSearchHandX * nPitchOff_X; //nPitchOff_X == 1

					int TableExist = nTableDvc[nYPocket][nXPocket];
					int PickerExist = nPickerPattern[nSearchHandY][nSearchHandX];
					if (TableExist == nChkCleanBuffer &&  PickerExist == nHadData)
					{
						nArrHandWorkSource[nSearchHandY][nSearchHandX] = DEF_WORK_RESERVED;
						nCurrentCnt++;

						if (nPitchType_Y == 0)
						{
							nSearchHandY = MAX_TRAY_PP_CNT_Y;
						}
						if (nPitchType_X == 0)
						{
							nSearchHandX = MAX_TRAY_PP_CNT_X;
						}
					}
				}
				nSkipYCnt++;
			}

			BOOL bLimitCheckOK = GetAreaProblem(nSearchX - VIRTUAL_POCKET_X_START, nSearchY - VIRTUAL_POCKET_Y_START, (eDeviceDataStage)(eDeviceStage_LD_TBL_1 + nLocIdx - LOC_IDX_TRAY_PP_TBL_1), nArrHandWorkSource); // todo slt : 영역 점검을 위한 함수 구현 필요.
			if (bLimitCheckOK == TRUE)
			{
				if (/*nCurrentCnt > 0*/nTotalPossibleCnt < nCurrentCnt)
				{
					//작업해라.
					nTotalPossibleCnt = nCurrentCnt;
					memcpy_s(nArrHandWorkCopy, sizeof(nArrHandWorkCopy), nArrHandWorkSource, sizeof(nArrHandWorkSource));
					nCXPosition = nSearchX;
					nCYPosition = nSearchY;
				}
			}
		}
	}



	memcpy_s(nWorkPkr, sizeof(nArrHandWorkCopy), nArrHandWorkCopy, sizeof(nArrHandWorkCopy));

	//0번 피커부터 할당되지 않은 포켓은 트림한다.
	int nPockOffsetX = 0;
	int nPockOffsetY = 1;

	//X방향 첫 할당 위치가 X옵셋
	for (; nPockOffsetX < MAX_TRAY_PP_CNT_X; nPockOffsetX++)
	{
		if (nWorkPkr[0][nPockOffsetX] != DEF_WORK_FREE) break;
		if (nWorkPkr[1][nPockOffsetX] != DEF_WORK_FREE) break;
	}
	//X방향 첫 할당 위치가 1개라도 있으면 Y옵셋은 0
	for (int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++)
	{
		if (nWorkPkr[0][nX] != DEF_WORK_FREE) {
			nPockOffsetY = 0;
			break;
		}
	}

	nCXPosition -= VIRTUAL_POCKET_X_START;
	nCYPosition -= VIRTUAL_POCKET_Y_START;

	//Feeder 뒷열 부터 Pick 할 경우,
	//Front Picker(Y==1)기준으로 Table Row Step 결정.
	//if (nCYPosition < 0) {
	g_TaskLoadTbl[nLocIdx].SetTableRowStep(nCYPosition);
	//}

	//Loading Table Working Pocket
	*n_Xpos = nCXPosition + nPockOffsetX*nPitchOff_X;
	*n_YPos = nCYPosition + nPockOffsetY*nPitchOff_Y;


	return nTotalPossibleCnt;
}
