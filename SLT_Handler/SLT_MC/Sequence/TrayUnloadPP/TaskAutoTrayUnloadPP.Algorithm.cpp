#include "stdafx.h"
#include "TaskAutoTrayLoadPP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace base_seq_cmd;
using namespace std;

void CTaskAutoTrayUnloadPP::FreeAllSchedule()
{
	for each(auto v in m_listTrayPpSchedule) {
		CBaseSeqCmd* pCmd = v;
		SAFE_DELETE(pCmd);
	}
	m_listTrayPpSchedule.clear();
}

CBaseSeqCmd * CTaskAutoTrayUnloadPP::GetNextSchedule()
{
	CBaseSeqCmd* pReturnCmd = nullptr;

	if (m_listTrayPpSchedule.size() > 0)
	{
		pReturnCmd = m_listTrayPpSchedule.front();
		m_listTrayPpSchedule.pop_front();
	}

	return pReturnCmd;
}

int CTaskAutoTrayUnloadPP::MakeTrayPpSchedule()
{
	if( m_pTaskTrayPP->GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready ) {
		return m_listTrayPpSchedule.size();
	}

 	CString strMsg = _T( "" );
	std::vector<CString> vErrString; vErrString.clear();
	int nChkErr = m_pTaskTrayPP->CheckDevice( strMsg, vErrString);
	if( nChkErr != ERR_NO_ERROR ) {
		CParamList AlarmData;
		if (strMsg.IsEmpty() != TRUE) { AlarmData.m_fnAddItemFormat(strMsg); }
		for (int i = 0; i < (int)vErrString.size(); i++)
		{
			AlarmData.m_fnAddItemFormat("%s", vErrString[i]);
		}
		g_Error.AlarmReport( nChkErr, AlarmData, &g_TaskSystemCtrl, m_pTaskTrayPP->GetFailPicker() );
		g_TaskSystemCtrl.OnLotCommand( eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto );

		return m_listTrayPpSchedule.size();
	}

	int nCntUnloadPicker = 0, nCntLoadPicker = 0;

	nCntLoadPicker   = m_pTaskTrayPP->GetTrayPpLoadDvcCnt();
	nCntUnloadPicker = m_pTaskTrayPP->GetTrayPpUnloadDvcCnt();

	// pick/place의 순서를 결정 ( C방향으로 돌아야 한다 )
	int nListOldCount = m_listTrayPpSchedule.size();
	if( m_nPickPlaceStep != eTrayPpPickPlaceSubCmd_PlacePlate ) {
		m_nPickPlaceBackUpStep = -1;
	}

	switch( m_nPickPlaceStep ) 
	{
		case eTrayPpPickPlaceSubCmd_Idle:
			{
				m_nPickPlaceStep = eTrayPpPickPlaceSubCmd_PlacePlate;
			}break;

		case eTrayPpPickPlaceSubCmd_PlacePlate:
			{
				int nJobCount = 0;
				// 마지막으로 했던 작업이 Unloading인데 잔여 unloading device가 남아있으면 처리한다.
				if( m_nPickPlaceStep == m_nPickPlaceBackUpStep && nCntUnloadPicker != 0 )
				{
					nJobCount = _MakeSchedule_TrayPp_Place_SetPlate();
					if( nJobCount > 0 ) {
						break;
					}
				}

				nJobCount = _MakeSchedule_TrayPp_Place_SetPlate();
				if( nJobCount > 0 ) { // record histoy
					m_nPickPlaceBackUpStep = m_nPickPlaceStep;
				}

				if( nJobCount == 0 || nCntUnloadPicker == 0 ) {
					m_nPickPlaceStep = eTrayPpPickPlaceSubCmd_PickTbl;
				}
			}break;
		case eTrayPpPickPlaceSubCmd_PickTbl:
			{
				// supply index desc
				int nRetLoadTblWorkIdx = MakeLoadTblWorkingIdx();

				if (nRetLoadTblWorkIdx >= 0)
				{
					int max_Unload_pocket_cnt = g_TaskLoadTbl[nRetLoadTblWorkIdx].m_LoadTbl_pocket_setting.get_item_cnt('U');
					if (g_TaskLoadTbl[nRetLoadTblWorkIdx].GetUnloadDeviceCnt() == max_Unload_pocket_cnt && nCntUnloadPicker > 0)
					{
						m_nPickPlaceStep = eTrayPpPickPlaceSubCmd_Idle;
						break;
					}

					int max_unload_pkr_cnt = m_pTaskTrayPP->m_TrayPP_picker_setting.get_item_cnt('U');
					//if ( nCntUnloadPicker < max_unload_pkr_cnt) {
					if(nCntUnloadPicker == 0){
						if (g_TaskLoadTbl[nRetLoadTblWorkIdx].GetUnloadDeviceCnt() > 0) {
							_MakeSchedule_TrayPp_Pick_LoadTable(nRetLoadTblWorkIdx);
						}
					}
				}

				// no more job & change status
				if( nListOldCount == m_listTrayPpSchedule.size() || nCntUnloadPicker > 0/*nRetLoadTblWorkIdx < 0*/) {
					_MakeSchedule_TrayPp_Place_SetPlate();
					m_nPickPlaceStep = eTrayPpPickPlaceSubCmd_Idle;
				}
			}break;
		default: { m_nPickPlaceStep = eTrayPpPickPlaceSubCmd_Idle; }break;
	}

	//새롭게 등록된 스케줄 횟수 리턴
	return m_listTrayPpSchedule.size();

}
int CTaskAutoTrayUnloadPP::MakeTrayPpScheduleGrr()
{
	if (m_pTaskTrayPP->GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready) {
		return m_listTrayPpSchedule.size();
	}

	CString strMsg = _T("");
	std::vector<CString> vErrString; vErrString.clear();
	int nChkErr = m_pTaskTrayPP->CheckDevice(strMsg, vErrString);
	if (nChkErr != ERR_NO_ERROR) {
		CParamList AlarmData;
		if (strMsg.IsEmpty() != TRUE) { AlarmData.m_fnAddItemFormat(strMsg); }
		for (int i = 0; i < (int)vErrString.size(); i++)
		{
			AlarmData.m_fnAddItemFormat("%s", vErrString[i]);
		}
		g_Error.AlarmReport(nChkErr, AlarmData, &g_TaskSystemCtrl, m_pTaskTrayPP->GetFailPicker());
		g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);

		return m_listTrayPpSchedule.size();
	}

	int nCntUnloadPicker = 0, nCntLoadPicker = 0;

	nCntLoadPicker = m_pTaskTrayPP->GetTrayPpLoadDvcCnt();
	nCntUnloadPicker = m_pTaskTrayPP->GetTrayPpUnloadDvcCnt();

	// pick/place의 순서를 결정 ( C방향으로 돌아야 한다 )
	int nListOldCount = m_listTrayPpSchedule.size();
	if (m_nPickPlaceStep != eTrayPpPickPlaceSubCmd_PlacePlate) {
		m_nPickPlaceBackUpStep = -1;
	}

	switch (m_nPickPlaceStep)
	{
	case eTrayPpPickPlaceSubCmd_Idle:
	{
		m_nPickPlaceStep = eTrayPpPickPlaceSubCmd_PlacePlate;
	}break;

	case eTrayPpPickPlaceSubCmd_PlacePlate:
	{
		int nJobCount = 0;
		// 마지막으로 했던 작업이 Unloading인데 잔여 unloading device가 남아있으면 처리한다.
		if (m_nPickPlaceStep == m_nPickPlaceBackUpStep && nCntUnloadPicker != 0)
		{
			nJobCount = _MakeScheduleGrr_TrayPp_Place_UnloadSetPlate();
			if (nJobCount > 0) {
				break;
			}
		}

		nJobCount = _MakeScheduleGrr_TrayPp_Place_UnloadSetPlate();
		if (nJobCount > 0) { // record histoy
			m_nPickPlaceBackUpStep = m_nPickPlaceStep;
		}

		if (nJobCount == 0 || nCntUnloadPicker == 0) {
			m_nPickPlaceStep = eTrayPpPickPlaceSubCmd_PickTbl;
		}
	}break;
	case eTrayPpPickPlaceSubCmd_PickTbl:
	{
		// supply index desc
		int nRetLoadTblWorkIdx = MakeLoadTblWorkingIdx();

		if (nRetLoadTblWorkIdx >= 0)
		{
			int max_Unload_pocket_cnt = g_TaskLoadTbl[nRetLoadTblWorkIdx].m_LoadTbl_pocket_setting.get_item_cnt('U');
			if (g_TaskLoadTbl[nRetLoadTblWorkIdx].GetUnloadDeviceCnt() == max_Unload_pocket_cnt && nCntUnloadPicker > 0)
			{
				m_nPickPlaceStep = eTrayPpPickPlaceSubCmd_Idle;
				break;
			}

			int max_unload_pkr_cnt = m_pTaskTrayPP->m_TrayPP_picker_setting.get_item_cnt('U');
			if (nCntUnloadPicker < max_unload_pkr_cnt) {
				if (g_TaskLoadTbl[nRetLoadTblWorkIdx].GetUnloadDeviceCnt() > 0) {
					_MakeSchedule_TrayPp_Pick_LoadTable(nRetLoadTblWorkIdx);
				}
			}
		}

		// no more job & change status
		if (nListOldCount == m_listTrayPpSchedule.size() || nRetLoadTblWorkIdx < 0) {
			_MakeScheduleGrr_TrayPp_Place_UnloadSetPlate();
			m_nPickPlaceStep = eTrayPpPickPlaceSubCmd_Idle;
		}
	}break;
	default: { m_nPickPlaceStep = eTrayPpPickPlaceSubCmd_Idle; }break;
	}

	//새롭게 등록된 스케줄 횟수 리턴
	return m_listTrayPpSchedule.size();

}

int CTaskAutoTrayUnloadPP::FindUnloadigSetplate(eDeviceDataStage *pArrDvcHwBins, int nMaxArrDvcHwBins)
{
	int nRet_HwBinCount = 0;

	eDeviceDataStage NowLoadSetplateDvcLoc = eDeviceStage_Undefine;
	ST_DD_DEVICE stDvc;

	int MAX_UNLOAD_SETPLATE = g_unload_stackers.size();
	CHw_Bin_Sort_Item *pArrTempHwBins = new CHw_Bin_Sort_Item[MAX_UNLOAD_SETPLATE];

	int row_cnt = m_pTaskTrayPP->m_TrayPP_picker_setting.max_row_cnt();
	int col_cnt = m_pTaskTrayPP->m_TrayPP_picker_setting.max_col_cnt();
	for (int nPickY = 0; nPickY < row_cnt; nPickY++) {
		for (int nPickX = 0; nPickX < col_cnt; nPickX++) {
			auto setting = m_pTaskTrayPP->m_TrayPP_picker_setting[nPickY][nPickX];
			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TRAY_PP_1 + m_eTrayIdx, nPickX, nPickY, &stDvc);

			if (setting != 'U' || stDvc.sExist != DEF_EXIST)		
				continue;

			//배열이 모두 차면 더 이상 하지 말라!
			if (nMaxArrDvcHwBins <= nRet_HwBinCount) { goto goto_pArrDvcHwBins_full; }

			int nTempHwBinCount = 0;
			int nSortType = g_DMCont.m_dmEQP.GN(NDM0_SortType);
			if (nSortType == eSortType_SwBin) {
				nTempHwBinCount = convert_sw_bin_to_hw_bin_multi(pArrTempHwBins, MAX_UNLOAD_SETPLATE, stDvc.szBinResult);
			}
			else if (nSortType == eSortType_ErrStr) {
				nTempHwBinCount = convert_ErrDisrip_to_hw_bin_multi(pArrTempHwBins, MAX_UNLOAD_SETPLATE, stDvc.szErrString);
			}

			for (int nTempHwBin_I = 0; nTempHwBin_I < nTempHwBinCount; nTempHwBin_I++)
			{
				NowLoadSetplateDvcLoc = pArrTempHwBins[nTempHwBin_I].eHwBin;
				//bin 결과를 모르는 경우는 일단 skip
				if (NowLoadSetplateDvcLoc == (-1)) { continue; }

				//모든 것이 완벽하다면, 중복 검사 실시.
				BOOL bDuplicated = FALSE;
				for (int nDuplicateChkI = 0; nDuplicateChkI < nRet_HwBinCount; nDuplicateChkI++)
				{
					if (pArrDvcHwBins[nDuplicateChkI] == NowLoadSetplateDvcLoc) { bDuplicated = TRUE; break; }
				}

				//중복 검사에 발견 되지 않은 값이면 추가!
				if (bDuplicated == FALSE) {
					pArrDvcHwBins[nRet_HwBinCount] = NowLoadSetplateDvcLoc;
					++nRet_HwBinCount;
				}
			}
		}
	}

goto_pArrDvcHwBins_full:

	//priority : 1 step HwBin Decending
	for (int nRef_I = 0; nRef_I < nRet_HwBinCount; nRef_I++) {
		for (int nCmp_I = 1; nCmp_I < nRet_HwBinCount; nCmp_I++) {
			if (pArrTempHwBins[nRef_I].eHwBin < pArrTempHwBins[nCmp_I].eHwBin) {
				CHw_Bin_Sort_Item Temp = pArrTempHwBins[nRef_I];
				pArrTempHwBins[nRef_I] = pArrTempHwBins[nCmp_I];
				pArrTempHwBins[nCmp_I] = Temp;
			}
		}
	}

	int nSortType = g_DMCont.m_dmEQP.GN(NDM0_SortType);// enum eSortingType{   
	if (nSortType == eSortType_SwBin) {
		//priority : 2 step LoadingTime Accending (where same SW Bin)
		for (int nRef_I = 0; nRef_I < nRet_HwBinCount; nRef_I++) {
			for (int nCmp_I = 1; nCmp_I < nRet_HwBinCount; nCmp_I++) {
				//same SW bin
				if (strcmp(pArrTempHwBins[nRef_I].szSwBin, pArrTempHwBins[nCmp_I].szSwBin) == 0) {
					if (pArrTempHwBins[nRef_I].dLoadingTime > pArrTempHwBins[nCmp_I].dLoadingTime) {
						CHw_Bin_Sort_Item Temp = pArrTempHwBins[nRef_I];
						pArrTempHwBins[nRef_I] = pArrTempHwBins[nCmp_I];
						pArrTempHwBins[nCmp_I] = Temp;
					}
				}
			}
		}
	}
	else if (nSortType == eSortType_ErrStr) {
		//priority : 2 step LoadingTime Accending (where same ErrStr)
		for (int nRef_I = 0; nRef_I < nRet_HwBinCount; nRef_I++) {
			for (int nCmp_I = 1; nCmp_I < nRet_HwBinCount; nCmp_I++) {
				//same ErrStr
				if (strcmp(pArrTempHwBins[nRef_I].szErrStr, pArrTempHwBins[nCmp_I].szErrStr) == 0) {
					if (pArrTempHwBins[nRef_I].dLoadingTime > pArrTempHwBins[nCmp_I].dLoadingTime) {
						CHw_Bin_Sort_Item Temp = pArrTempHwBins[nRef_I];
						pArrTempHwBins[nRef_I] = pArrTempHwBins[nCmp_I];
						pArrTempHwBins[nCmp_I] = Temp;
					}
				}
			}
		}
	}

	//Output HW bin info
	for (int nRef_I = 0; nRef_I < nRet_HwBinCount; nRef_I++) {
		pArrDvcHwBins[nRef_I] = pArrTempHwBins[nRef_I].eHwBin;
	}

	//SAFE_DELETE(pDvc);
	SAFE_DELETE_ARRAY(pArrTempHwBins);

	return nRet_HwBinCount;
}


int CTaskAutoTrayUnloadPP::_MakeJobPlaceToSetplate()
{
	int nSchdCount = m_listTrayPpSchedule.size();
	int nSetplateObj_Index = (-1);

	/*
	*	picker가 들고 있는 디바이스의 HW BIN 정보를 취합한다.
	*/
	int nArrHwBinCount = 0;
	eDeviceDataStage *pArrHwBins = new eDeviceDataStage[g_BinCategory.nIndexCount];
	for (int i = 0; i < g_BinCategory.nIndexCount; i++)
		pArrHwBins[i] = (eDeviceDataStage)(-1);

	nArrHwBinCount = FindUnloadigSetplate(pArrHwBins, g_BinCategory.nIndexCount);

	int nHandDevice[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = { 0, };
	ZeroMemory(nHandDevice, sizeof(nHandDevice));
	ST_DD_DEVICE stDeviceHand;
	for (int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++)
	{
		for (int nX = 0; nX < (MAX_TRAY_PP_CNT_X); nX++)
		{	
			char setting = m_pTaskTrayPP->m_TrayPP_picker_setting[nY][nX];
			int nExist = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TRAY_PP_1+ m_eTrayIdx, nX, nY);

			if (setting == 'U' && nExist == DEF_EXIST) {
				nHandDevice[nY][nX] = DEF_EXIST;
			}
			else if (setting == 'U' && nExist == DEF_NOT_EXIST) {
				nHandDevice[nY][nX] = DEF_NOT_EXIST;
			}			
		}
	}
	/*
	*	취합된 HW BIN 정보대로 스케줄을 생성한다.
	*/
	for (int nHwBin_Index = 0; nHwBin_Index < nArrHwBinCount; nHwBin_Index++)
	{
		eDeviceDataStage NowHwBin = pArrHwBins[nHwBin_Index];
		if (NowHwBin == (-1)) { continue; }
		//place 스케줄 만들기.
		/* 주의!
		*	스케줄이 확정된 Stage로 모션을 지정할 경우 eLocIdxTrayPP 값을 사용해야 함.
		*/
		eLocIdxTrayPP TrayPpLoc = (eLocIdxTrayPP)(LOC_IDX_TRAY_PP_CONV_1 + (NowHwBin - eDeviceStage_STACKER_CONV_1));
		int nPockX, nPockY;
		int nArrPickerAssign[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X];

		/*
		*	Picker와 일치하는 pattern을 찾기 위해
		*	300x300 버퍼 공간에 C-Tray device 정보를 채운다.
		*	C-Tray (0,0) = 버퍼 공간 (100,100) 위치
		*/
		int nArrVirtualCTray[VIRTUAL_POCKET_Y_MAX][VIRTUAL_POCKET_X_MAX]; // data 검색용 버퍼
		int nHandAvailable[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = { 0, };
		memset(nArrVirtualCTray, 0xFF, sizeof(nArrVirtualCTray));

		/*
		*	작업 가능한 picker가 없으면 볼 것도 없다.
		*/
		int nAvailableCount = GetDeviceExist_CTrayHand(DEF_PLACE, NowHwBin, (int*)nHandAvailable, MAX_TRAY_PP_CNT_X, MAX_TRAY_PP_CNT_Y);
		int nRemainDevice = 0;
		for (int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++)
		{
			for (int nX = 0; nX < (MAX_TRAY_PP_CNT_X); nX++)
			{
				char setting = m_pTaskTrayPP->m_TrayPP_picker_setting[nY][nX];
				if(setting == 'U'){
					nHandAvailable[nY][nX] = DEF_NOT_EXIST;
				}

				if (setting == 'U') {
					nRemainDevice++;
				}
			}
		}

		if ( /*nAvailableCount*/nRemainDevice <= 0) { continue; }

		/*
		*	가상의 공간에 C-Tray 디바이스 맵을 배치한다.
		*/
		FillDeviceData_VirtualCTray(NowHwBin, (int*)nArrVirtualCTray, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, VIRTUAL_POCKET_X_START, VIRTUAL_POCKET_Y_START);

		/*
		*	place 할 수 있는 pattern을 찾는다.
		*/
		int nRetCode = 0;
		// 		for(int no_infinite=0; no_infinite<8; no_infinite++ /*anti-no_infinite loop, max 8 schedules*/) 
		// 		{
		nRetCode = FindSortToSetplate(NowHwBin, nArrVirtualCTray, nHandAvailable, DEF_PLACE, &nPockX, &nPockY, nArrPickerAssign, CTaskAutoTrayUnloadPP::eRobotWorkDirTopLeft);
		if (nRetCode > 0) {
			CTrayUnloadPPCmd_MovePickPlace* pCmd = new CTrayUnloadPPCmd_MovePickPlace;
			pCmd->m_Cmd.eStage = TrayPpLoc;

			for (int nPickY = 0; nPickY < MAX_TRAY_PP_CNT_Y; nPickY++)
			{
				for (int nPickX = 0; nPickX < (MAX_TRAY_PP_CNT_X); nPickX++)
				{
					char setting = m_pTaskTrayPP->m_TrayPP_picker_setting[nPickY][nPickX];
					if (setting == 'U') {
						pCmd->m_Cmd.vPicker.push_back(CPoint(nPickX, nPickY));
						nHandDevice[nPickY][nPickX] = DEF_NOT_EXIST;
					}
				}
			}

			pCmd->m_Cmd.Pocket.SetPoint(nPockX, nPockY);
			pCmd->m_Cmd.ePickPlace = ePick;

			// find same command
			BOOL bSameCmd = FALSE;

			std::find_if(m_listTrayPpSchedule.begin(), m_listTrayPpSchedule.end()
				, [&](CBaseSeqCmd* _pTrayPpCmd) -> bool
			{
				CTrayUnloadPPCmd_MovePickPlace* pTrayPpCmd = (CTrayUnloadPPCmd_MovePickPlace*)_pTrayPpCmd;
				if (pTrayPpCmd->m_Cmd.eStage == TrayPpLoc && pTrayPpCmd->m_Cmd.Pocket == pCmd->m_Cmd.Pocket) {
					bSameCmd = TRUE;
					return true;
				}
				return false;
			});

			// no search same command
			if (bSameCmd == FALSE) {
				m_listTrayPpSchedule.push_back(pCmd);
				break;
			}
			else {
				delete pCmd;
			}
		}
		else {
			/*
			*	더이상 만들 수 없음.
			*/

			break;
		}
		//		}/*for(int no_infinite*/
	}/*for( nHwBin_Index*/
	delete[] pArrHwBins;

	//새롭게 등록된 스케줄 횟수 리턴
	return m_listTrayPpSchedule.size() - nSchdCount;
}

int CTaskAutoTrayUnloadPP::_MakeSchedule_TrayPp_Place_SetPlate()
{
	int nSchdCount = m_listTrayPpSchedule.size();
	int nSetplateObj_Index = ( -1 );

	/*
	*	picker가 들고 있는 디바이스의 HW BIN 정보를 취합한다.
	*/
	int nArrHwBinCount = 0;
	eDeviceDataStage *pArrHwBins = new eDeviceDataStage[ g_BinCategory.nIndexCount ];
	for( int i = 0; i < g_BinCategory.nIndexCount; i++ )
		pArrHwBins[ i ] = ( eDeviceDataStage )( -1 );

	nArrHwBinCount = FindUnloadigSetplate( pArrHwBins, g_BinCategory.nIndexCount );

	int nHandDevice[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ] = { 0, };
	for( int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++ )
	{
		for( int nX = 0; nX < ( MAX_TRAY_PP_CNT_X ); nX++ )
		{
			char setting = m_pTaskTrayPP->m_TrayPP_picker_setting[ nY ][ nX ];
			int nExist = g_DMCont.m_dmDevice.GetDeviceExist( eDeviceStage_TRAY_PP_1+ m_eTrayIdx, nX, nY );

			if( setting == 'U' && nExist == DEF_EXIST ) {
				nHandDevice[ nY ][ nX ] = DEF_EXIST;
			}
			else if( setting == 'U' && nExist == DEF_NOT_EXIST ) {
				nHandDevice[ nY ][ nX ] = DEF_NOT_EXIST;
			}
		}
	}
	/*
	*	취합된 HW BIN 정보대로 스케줄을 생성한다.
	*/
	for( int nHwBin_Index = 0; nHwBin_Index < nArrHwBinCount; nHwBin_Index++ )
	{
		eDeviceDataStage NowHwBin = pArrHwBins[ nHwBin_Index ];
		if( NowHwBin == ( -1 ) ) { continue; }
		//place 스케줄 만들기.
		/* 주의!
		*	스케줄이 확정된 Stage로 모션을 지정할 경우 eLocIdxTrayPP 값을 사용해야 함.
		*/
		eLocIdxTrayPP TrayPpLoc = ( eLocIdxTrayPP )( LOC_IDX_TRAY_PP_CONV_1 + ( NowHwBin - eDeviceStage_STACKER_CONV_1 ) );
		int nPockX, nPockY;
		int nArrPickerAssign[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ];

		/*
		*	Picker와 일치하는 pattern을 찾기 위해
		*	300x300 버퍼 공간에 C-Tray device 정보를 채운다.
		*	C-Tray (0,0) = 버퍼 공간 (100,100) 위치
		*/
		int nArrVirtualCTray[ VIRTUAL_POCKET_Y_MAX ][ VIRTUAL_POCKET_X_MAX ]; // data 검색용 버퍼
		int nHandAvailable[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ] = { 0, };
		memset( nArrVirtualCTray, 0xFF, sizeof( nArrVirtualCTray ) );

		/*
		*	작업 가능한 picker가 없으면 볼 것도 없다.
		*/
		int nAvailableCount = GetDeviceExist_CTrayHand( DEF_PLACE, NowHwBin, ( int* )nHandAvailable, MAX_TRAY_PP_CNT_X, MAX_TRAY_PP_CNT_Y );
		int nRemainDevice = 0;
		for( int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++ )
		{
			for( int nX = 0; nX < ( MAX_TRAY_PP_CNT_X ); nX++ )
			{
				char setting = m_pTaskTrayPP->m_TrayPP_picker_setting[ nY ][ nX ];
				if( setting == 'U' && nHandDevice[ nY ][ nX ] == DEF_NOT_EXIST ) {
					nHandAvailable[ nY ][ nX ] = DEF_NOT_EXIST;
				}

				if( setting == 'U' && nHandAvailable[ nY ][ nX ] ) {
					nRemainDevice++;
				}
			}
		}

		if( /*nAvailableCount*/nRemainDevice <= 0 ) { continue; }

		/*
		*	가상의 공간에 C-Tray 디바이스 맵을 배치한다.
		*/
		FillDeviceData_VirtualCTray( NowHwBin, ( int* )nArrVirtualCTray, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, VIRTUAL_POCKET_X_START, VIRTUAL_POCKET_Y_START );

		/*
		*	place 할 수 있는 pattern을 찾는다.
		*/
		int nRetCode = 0;
		// 		for(int no_infinite=0; no_infinite<8; no_infinite++ /*anti-no_infinite loop, max 8 schedules*/) 
		// 		{
		nRetCode = FindSortToSetplate( NowHwBin, nArrVirtualCTray, nHandAvailable, DEF_PLACE, &nPockX, &nPockY, nArrPickerAssign, CTaskAutoTrayUnloadPP::eRobotWorkDirTopLeft );
		if( nRetCode > 0 ) {
			CTrayUnloadPPCmd_MovePickPlace* pCmd = new CTrayUnloadPPCmd_MovePickPlace;
			pCmd->m_Cmd.eStage = TrayPpLoc;

			for( int nPickY = 0; nPickY < MAX_TRAY_PP_CNT_Y; nPickY++ )
			{
				for( int nPickX = 0; nPickX < ( MAX_TRAY_PP_CNT_X ); nPickX++ )
				{
					char setting = m_pTaskTrayPP->m_TrayPP_picker_setting[ nPickY ][ nPickX ];
					if( setting == 'U' && nArrPickerAssign[ nPickY ][ nPickX ] != 0 ) {
						pCmd->m_Cmd.vPicker.push_back( CPoint( nPickX, nPickY ) );
						nHandDevice[ nPickY ][ nPickX ] = DEF_NOT_EXIST;
					}
				}
			}

			pCmd->m_Cmd.Pocket.SetPoint( nPockX, nPockY );
			pCmd->m_Cmd.ePickPlace = ePlace;

			// find same command
			BOOL bSameCmd = FALSE;

			std::find_if( m_listTrayPpSchedule.begin(), m_listTrayPpSchedule.end()
						  , [ & ] ( CBaseSeqCmd* _pTrayPpCmd ) -> bool
			{
				CTrayUnloadPPCmd_MovePickPlace* pTrayPpCmd = ( CTrayUnloadPPCmd_MovePickPlace* )_pTrayPpCmd;
				if( pTrayPpCmd->m_Cmd.eStage == TrayPpLoc && pTrayPpCmd->m_Cmd.Pocket == pCmd->m_Cmd.Pocket ) {
					bSameCmd = TRUE;
					return true;
				}
				return false;
			} );

			// no search same command
			if( bSameCmd == FALSE ) {
				m_listTrayPpSchedule.push_back( pCmd );
				break;
			}
			else {
				delete pCmd;
			}
		}
		else {
			/*
			*	더이상 만들 수 없음.
			*/

			break;
		}
		//		}/*for(int no_infinite*/
	}/*for( nHwBin_Index*/
	delete[] pArrHwBins;

	//새롭게 등록된 스케줄 횟수 리턴
	return m_listTrayPpSchedule.size() - nSchdCount;
}

int CTaskAutoTrayUnloadPP::WaitPreMoveLoadTblPos()
{
	int nTableReserve[eMaxLoadTblCount] = { RID_INIT_IDLE_RESERVE, };
	// Check Pre Move Interlock
	// Case 1 :: Not Ready Tray PP	
	if (m_pTaskTrayPP->GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
		return 1;
	// Case 2 :: All of one load table arrived loadSide is not action
	int nSiteReadyCnt = 0;
	for (int i = 0; i < MAX_LD_TBL_CNT; i++) {
		nTableReserve[i] = g_TaskLoadTbl[i].GetWorkReserve();
		if (RID_TRAY_PP_(RIDX_TRAY_PP_1) == nTableReserve[i]) {
			return 1;
		}
		if (RID_TEST_PP_(i) == nTableReserve[i] || RID_NOT_RESERVE == nTableReserve[i])
			nSiteReadyCnt++;
	}
	// table is all not ready
	if (nSiteReadyCnt == MAX_LD_TBL_CNT)
		return 1;

	// Case 3 :: Tray PP Get LoadTbl Loading pocket count same?
	int nCntUnloadPicker = m_pTaskTrayPP->GetTrayPpUnloadDvcCnt();
	int nCntLoadPicker   = m_pTaskTrayPP->GetTrayPpLoadDvcCnt();
	if (nCntLoadPicker % 4 == 0 || nCntUnloadPicker > 0)
		return 1;

	// case 4 : grr is not use pre move table
	BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
	if (bAutoGrrUse == TRUE)
		return 1;

	//CTime cCurTime = CTime::GetCurrentTime();
	CTime dTime[MAX_LD_TBL_CNT] = { 0, };
	//int nTblWorkNo[eMaxLoadTblCount] = { eLoadTbl_1, eLoadTbl_2 };
	vector<CTime> vArrivedTblTm; vArrivedTblTm.clear();

	// make data	
	// scan loadtable & make data
	int nPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	int nPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

	vector<CTime>::iterator vdRetMin, vdRetMax;
	for (int i = 0; i < MAX_LD_TBL_CNT; i++)
	{
		vArrivedTblTm.push_back(0);
		dTime[i] = vArrivedTblTm[i] = g_TaskLoadTbl[i].GetMoveSetplateTm();
	}
	vdRetMax = max_element(begin(vArrivedTblTm), end(vArrivedTblTm));
	for (int i = 0; i < MAX_LD_TBL_CNT; i++)
	{
		if (g_TaskAutoTestPp[i].GetModuleSocketOnCnt() == 0
			|| RID_TEST_PP_(i) == nTableReserve[i]
			|| RID_NOT_RESERVE == nTableReserve[i])
			dTime[i] = vArrivedTblTm[i] = (*vdRetMax + 100);
	}
	vdRetMin = min_element(begin(vArrivedTblTm), end(vArrivedTblTm));


	// find work load table
	int nLoadTblWorkIdx = -1;
	for (int i = 0; i < MAX_LD_TBL_CNT; i++) {
		if (dTime[i] == *vdRetMin && dTime[i] != *vdRetMax + 100)
			nLoadTblWorkIdx = i;
	}
	// last check interlock
	if (nLoadTblWorkIdx == -1) { return 1; }

	// check device & set picker
	vector<CPoint> vCmdPicker;
	vCmdPicker.push_back(CPoint(1, 0));

	BOOL bPosChkErr = FALSE;
	for (int i = 0; i < MAX_LD_TBL_CNT; i++) {
		int nChkTbl_X = m_pTaskTrayPP->ChkMotorPos(eLocIdxTrayPP(LOC_IDX_TRAY_PP_TBL_1 + i), CPoint(0, 0), vCmdPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX);
		int nChkTbl_Y = m_pTaskTrayPP->ChkMotorPos(eLocIdxTrayPP(LOC_IDX_TRAY_PP_TBL_1 + i), CPoint(0, 0), vCmdPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisY);
		int nChkTbl_Z = m_pTaskTrayPP->ChkMotorPos(eLocIdxTrayPP(LOC_IDX_TRAY_PP_TBL_1 + i), CPoint(0, 0), vCmdPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);

		if (nChkTbl_X == ERR_NO_ERROR && nChkTbl_Y == ERR_NO_ERROR && nChkTbl_Z == ERR_NO_ERROR) {
			bPosChkErr = TRUE;
		}
	}
	if (bPosChkErr == TRUE) { return 1; }
	else {
		if (m_pTaskTrayPP->GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready) {
			CTrayUnloadPPCmd_MovePos *pCmd = new CTrayUnloadPPCmd_MovePos;
			pCmd->m_Cmd.eStage  = eLocIdxTrayPP(LOC_IDX_TRAY_PP_TBL_1 + nLoadTblWorkIdx);
			pCmd->m_Cmd.Pocket  = CPoint(0, 0);
			pCmd->m_Cmd.vPicker = vCmdPicker;
			m_listTrayPpSchedule.push_back(pCmd);
		}
		return ERR_NO_ERROR;
	}

	return 1;
}

/*=============================================================================
	DESCRIPT : CTrayPP picker 상의 정보를 2차원 배열에 채워준다.
	KEY WORD :
	ARGUMENT : pnBuffer : 2차원 버퍼

	           Auto GRR은 Bin분류가 없다. Bin분류가 목적이 아니고 Tester에 대한 점검이 목적이기 때문

	RETURN   : 0 : NO Available picker
==============================================================================*/
int CTaskAutoTrayUnloadPP::GetWorkable_CTrayHand(BOOL bPickPlace, int pnBuffer[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X], bool bAutoGrrCancel /*= false*/)
{
	int nWorkableCnt = 0;
	int nExist = 0;
	int nPickerCount_X = MAX_TRAY_PP_CNT_X;
	int nPickerCount_Y = MAX_TRAY_PP_CNT_Y;

	ASSERT(bPickPlace == DEF_PICK || bPickPlace == DEF_PLACE);


	for (int nPkrY = 0; nPkrY < MAX_TRAY_PP_CNT_Y; nPkrY++)
	{
		for (int nPkrX = 0; nPkrX < MAX_TRAY_PP_CNT_X; nPkrX++)
		{
			char setting = m_pTaskTrayPP->m_TrayPP_picker_setting[nPkrY][nPkrX];
			int device_exist = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TRAY_PP_1 + m_eTrayIdx, nPkrX, nPkrY);

			switch (bPickPlace) {
			case DEF_PICK:
			{
				if (setting == 'L') {
					pnBuffer[nPkrY][nPkrX] = DEF_WORKABLE;
					if (device_exist == DEF_NOT_EXIST) {
						nWorkableCnt++;
					}
				}
				else {
					// pick 진행 시 동작 하지 말아야 하는 picker 
					pnBuffer[nPkrY][nPkrX] = DEF_UNWORKABLE;
				}
			}break;
			case DEF_PLACE:
			{
				if (setting == 'U') {
					pnBuffer[nPkrY][nPkrX] = DEF_WORKABLE;
					if (device_exist == DEF_EXIST) {
						nWorkableCnt++;
					}
				}
				else {
					// place 진행 시 동작 하지 말아야 하는 picker 
					pnBuffer[nPkrY][nPkrX] = DEF_UNWORKABLE;
				}
			}break;
			}
		}
	}

	return nWorkableCnt;
}


//  By hyb [2015-10-20]
/*=============================================================================
	DESCRIPT :
	KEY WORD :
	ARGUMENT : nWorkPlace   : 작업 스테이지 ex) buffer, c-tray  enum ePickPlaceWorkplace
	           bPickPlace   : pick 인지 place 0 : pick 1 : place
	           n_Xpos		: 작업할 pocket x 시작 위치
	           n_YPos		: 작업할 pocket y 시작 위치
	           nArrHandWork : 핸드 작업 할것인지 안할 것인지 지정 0 : 직업 X 1 : 작업 O
	           nWorkDir     : 작업 방향 enum eRobotWorkDir

	RETURN   : -1 : 작업 할 부분 없음
==============================================================================*/
int CTaskAutoTrayUnloadPP::FindPickFromSetplate(  int nWorkPlace,
	                                          int nArrDvcData_VirtualCTray[ VIRTUAL_POCKET_Y_MAX ][ VIRTUAL_POCKET_X_MAX ],
	                                          int nPickerDvcPattern[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ],
	                                          BOOL bPickPlace,
	                                          int *n_Xpos, int *n_YPos,
	                                          int nArrHandWork[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ],
	                                          int nWorkDir,
	                                          BOOL bChkQaSampleCnt )
{
	int nSearchY = 0, nSearchX = 0;
	int nSearchHandY = 0, nSearchHandX = 0;

	/*DEF_WORK_FREE or DEF_WORK_RESERVED*/
	int nArrHandWorkSource[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ] = { DEF_WORK_FREE, };
	int nArrHandWorkCopy[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ] = { DEF_WORK_FREE, };

	int nPocketCntY = 0;
	int nPocketCntX = 0;

	int nTotalPossibleCnt = 0;	// 가장 Pick 할 수 있는 수량
	int nCurrentCnt = 0;	// 현재 까지 Pick 할수 있는 수량
	int nCXPosition = 0;
	int nCYPosition = 0;
	// 1Y, 2Y는 계산 한 대로 가져 온다.

	int nCheckCTrayData = DEF_NOT_EXIST;
	int nHadData = DEF_NOT_EXIST;

	int nContainerPitch_X_Cnt = 0;	// pocket pitch와 
	int nContainerPitch_Y_Cnt = 0;

	if( bPickPlace == DEF_PICK ) {	// Pick 일 때 조건
		nCheckCTrayData = DEF_EXIST;
		nHadData = DEF_NOT_EXIST;
	}
	else {							// Place 일 때 조건		
		nCheckCTrayData = DEF_NOT_EXIST;
		nHadData = DEF_EXIST;
	}

	// [2x-pitch]
	int nX_PitchType = 1;
	int nY_PitchType = 1;

	// tray hand get load device count
	int nCntLoadPicker = m_pTaskTrayPP->GetTrayPpLoadDvcCnt();
	int nQaSampleCnt = g_DMCont.m_dmHandlerOpt.GN( NDM11_QaSampleCount );
	int nLoadDeviceCnt = g_DMCont.m_dmEQP.GN( NDM0_QaSampleLoadCnt ) + nCntLoadPicker;

	switch( nWorkPlace )
	{
		//case eDeviceStage_STACKER_CONV_2:
		case eDeviceStage_STACKER_CONV_3:
		case eDeviceStage_STACKER_CONV_4:
		case eDeviceStage_STACKER_CONV_5:
		case eDeviceStage_STACKER_CONV_6:
		case eDeviceStage_STACKER_CONV_7:
			{
				if( g_DMCont.m_dmTrayPP.GN( NDM5_Tray_Pitch_X ) < DEF_TRAY_PP_MAX_X_PITCH_NARROW ) {
					nX_PitchType = 2;
				}
				if( g_DMCont.m_dmTrayPP.GN( NDM5_Tray_Pitch_Y ) < DEF_TRAY_PP_MAX_Y_PITCH_NARROW ) {
					nY_PitchType = 2;
				}

				nPocketCntX = g_DMCont.m_dmTrayPP.GN( NDM5_TrayDiv_X );
				nPocketCntY = g_DMCont.m_dmTrayPP.GN( NDM5_TrayDiv_Y );

				nContainerPitch_X_Cnt = nX_PitchType; // x pitch
				nContainerPitch_Y_Cnt = nY_PitchType; // y pitch
			}break;
		case eDeviceStage_TRAY_BUFF:
			{
				nPocketCntX = 4;
				nPocketCntY = 4;

				nContainerPitch_X_Cnt = nX_PitchType; // x pitch
				nContainerPitch_Y_Cnt = nY_PitchType; // y pitch
			}break;
		default:
			ASSERT( false );
			break;
	}

	// CTray에서 첫 디바이스가 발견되는 Y+1 줄까지만 검색 하도록!
	int nSearchY_Limit = ( VIRTUAL_POCKET_Y_MAX - 1 );
	BOOL bFindDevice = FALSE;

	if (nWorkPlace != eDeviceStage_TRAY_BUFF)
	{
		for (nSearchY = VIRTUAL_POCKET_Y_START; nSearchY < (VIRTUAL_POCKET_Y_START + nPocketCntY); nSearchY++)
		{
			for (nSearchX = VIRTUAL_POCKET_X_START; nSearchX < (VIRTUAL_POCKET_X_START + nPocketCntX); nSearchX++)
			{
				if (nArrDvcData_VirtualCTray[nSearchY][nSearchX] == nCheckCTrayData) {
					nSearchY_Limit = nSearchY + (nContainerPitch_Y_Cnt - 1);
					bFindDevice = TRUE;
					break;
				}
			}
			if (bFindDevice == TRUE) { break; }
		}
	}
	

	if( nWorkDir == CTaskAutoTrayUnloadPP::eRobotWorkDirTopLeft )
	{
		for( nSearchY = VIRTUAL_POCKET_Y_START - ( MAX_TRAY_PP_CNT_Y - 1 ) * nContainerPitch_Y_Cnt; nSearchY < VIRTUAL_POCKET_Y_START + nPocketCntY; nSearchY++ )
		{
			if(nSearchY >= VIRTUAL_POCKET_Y_START && nSearchY > nSearchY_Limit /*+1*/ ) { break; } // CTray에서 첫 디바이스가 발견되는 Y+1 줄까지만 검색 하도록!

			for( nSearchX = VIRTUAL_POCKET_X_START - ( MAX_TRAY_PP_CNT_X - 1 ) * nContainerPitch_X_Cnt; nSearchX < VIRTUAL_POCKET_X_START + nPocketCntX; nSearchX++ )
			{
				nCurrentCnt = 0;

				memset( nArrHandWorkSource, 0, sizeof( nArrHandWorkSource ) );

				for( nSearchHandY = 0; nSearchHandY < MAX_TRAY_PP_CNT_Y; nSearchHandY++ )
				{
					for( nSearchHandX = 0; nSearchHandX < MAX_TRAY_PP_CNT_X; nSearchHandX++ )
					{
						int nYPocket = 0, nXPocket = 0;
						nYPocket = nSearchY + nSearchHandY * nContainerPitch_Y_Cnt;
						nXPocket = nSearchX + nSearchHandX * nContainerPitch_X_Cnt;

						if( nArrDvcData_VirtualCTray[ nYPocket ][ nXPocket ] == nCheckCTrayData
							&&  nPickerDvcPattern[ nSearchHandY ][ nSearchHandX ] == nHadData )
						{
							nArrHandWorkSource[ nSearchHandY ][ nSearchHandX ] = DEF_WORK_RESERVED;
							nCurrentCnt++;
						}
					}
				}

				double dTeaching_X = 0.0; // Tray PP의 해당 Stage의 (0,0) 티칭값Y um
				double dTeaching_Y = 0.0; // Tray PP의 해당 Stage의 (0,0) 티칭값Y um
				double dCTrayXpitch = 0.0;
				double dCTrayYpitch = 0.0;
				double dLimitCheckValue_X = 0.0; // 점검을 위한 계산된 값.
				double dLimitCheckValue_Y = 0.0; // 점검을 위한 계산된 값.

				BOOL bLimitCheckOK = GetAreaProblem( nSearchX - VIRTUAL_POCKET_X_START, nSearchY - VIRTUAL_POCKET_Y_START, ( eDeviceDataStage )nWorkPlace, nArrHandWorkSource ); // todo slt : 영역 점검을 위한 함수 구현 필요.
				if( bLimitCheckOK == TRUE )
				{
					if( ( nCurrentCnt > nTotalPossibleCnt && bChkQaSampleCnt == FALSE )
						|| ( bChkQaSampleCnt == TRUE && nCurrentCnt > nTotalPossibleCnt && nQaSampleCnt >= nLoadDeviceCnt + nCurrentCnt ) )
					{
						//작업해라.
						nTotalPossibleCnt = nCurrentCnt;
						memcpy_s( nArrHandWorkCopy, sizeof( nArrHandWorkCopy ), nArrHandWorkSource, sizeof( nArrHandWorkCopy ) );
						nCXPosition = nSearchX;
						nCYPosition = nSearchY;
					}
				}
			}
		}
	}
	else
	{
		for( nSearchY = VIRTUAL_POCKET_Y_START + ( nPocketCntY - 1 ); nSearchY >= VIRTUAL_POCKET_Y_START - ( MAX_TRAY_PP_CNT_Y - 1 ) * nContainerPitch_Y_Cnt; nSearchY-- )
		{
			if( nSearchY > nSearchY_Limit ) { break; } // CTray에서 첫 디바이스가 발견되는 Y+1 줄까지만 검색 하도록!

			for( nSearchX = VIRTUAL_POCKET_X_START - ( MAX_TRAY_PP_CNT_X - 1 ) * nContainerPitch_X_Cnt; nSearchX < VIRTUAL_POCKET_X_START + nPocketCntX; nSearchX++ )
			{
				nCurrentCnt = 0;

				memset( nArrHandWorkSource, 0, sizeof( nArrHandWorkSource ) );

				for( nSearchHandY = MAX_TRAY_PP_CNT - 1; nSearchHandY >= 0; nSearchHandY-- )
				{
					for( nSearchHandX = 0; nSearchHandX < MAX_TRAY_PP_CNT_X; nSearchHandX++ )
					{
						int nYPocket = 0, nXPocket = 0;
						nYPocket = nSearchY + nSearchHandY * nContainerPitch_Y_Cnt;
						nXPocket = nSearchX + nSearchHandX * nContainerPitch_X_Cnt;

						if( nArrDvcData_VirtualCTray[ nYPocket ][ nXPocket ] == nCheckCTrayData
							&&  nPickerDvcPattern[ nSearchHandY ][ nSearchHandX ] == nHadData )
						{
							nArrHandWorkSource[ nSearchHandY ][ nSearchHandX ] = DEF_WORK_RESERVED;
							nCurrentCnt++;
						}
					}
				}

				double dTeaching_X = 0.0; // Tray PP의 해당 Stage의 (0,0) 티칭값Y um
				double dTeaching_Y = 0.0; // Tray PP의 해당 Stage의 (0,0) 티칭값Y um
				double dCTrayXpitch = 0.0;
				double dCTrayYpitch = 0.0;
				double dLimitCheckValue_X = 0.0; // 점검을 위한 계산된 값.
				double dLimitCheckValue_Y = 0.0; // 점검을 위한 계산된 값.

				BOOL bLimitCheckOK = GetAreaProblem( nSearchX - VIRTUAL_POCKET_X_START, nSearchY - VIRTUAL_POCKET_Y_START, ( eDeviceDataStage )nWorkPlace, nArrHandWorkSource ); // todo slt : 영역 점검을 위한 함수 구현 필요.
				if( bLimitCheckOK == TRUE )
				{
					if( ( nCurrentCnt > nTotalPossibleCnt && bChkQaSampleCnt == FALSE )
						|| ( bChkQaSampleCnt == TRUE && nCurrentCnt > nTotalPossibleCnt && nQaSampleCnt >= nLoadDeviceCnt + nCurrentCnt ) )
					{
						//작업해라.
						nTotalPossibleCnt = nCurrentCnt;
						memcpy_s( nArrHandWorkCopy, sizeof( nArrHandWorkCopy ), nArrHandWorkSource, sizeof( nArrHandWorkCopy ) );
						nCXPosition = nSearchX;
						nCYPosition = nSearchY;
					}
				}
			}
		}
	}

	memcpy_s( nArrHandWork, sizeof( nArrHandWorkCopy ), nArrHandWorkCopy, sizeof( nArrHandWorkCopy ) );

	//0번 피커부터 할당되지 않은 포켓은 트림한다.
	int nPockOffsetX = 0;
	int nPockOffsetY = 1;

	//X방향 첫 할당 위치가 X옵셋
	for( ; nPockOffsetX < MAX_TRAY_PP_CNT_X; nPockOffsetX++ )
	{
		if( nArrHandWork[ 0 ][ nPockOffsetX ] != DEF_WORK_FREE ) break;
		if( nArrHandWork[ 1 ][ nPockOffsetX ] != DEF_WORK_FREE ) break;
	}
	//X방향 첫 할당 위치가 1개라도 있으면 Y옵셋은 0
	for( int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++ )
	{
		if( nArrHandWork[ 0 ][ nX ] != DEF_WORK_FREE ) {
			nPockOffsetY = 0;
			break;
		}
	}

	/*
	*	스케쥴을 생성한 디바이스는 가상 버퍼에서 지우고 다음 pick 스케줄 생성을 시도한다.
	*/

	for( int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++ )
	{
		for( int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++ )
		{
			if( nArrHandWork[ nY ][ nX ] != 0 ) {
				int nVirX = nCXPosition + ( nX*nContainerPitch_X_Cnt );
				int nVirY = nCYPosition + ( nY*nContainerPitch_Y_Cnt );

				if( bPickPlace == DEF_PICK ) {
					nArrDvcData_VirtualCTray[ nVirY ][ nVirX ] = DEF_NOT_EXIST;
					nPickerDvcPattern[ nY ][ nX ] = DEF_EXIST;
				}
				else {
					nArrDvcData_VirtualCTray[ nVirY ][ nVirX ] = DEF_EXIST;
					nPickerDvcPattern[ nY ][ nX ] = DEF_NOT_EXIST;
				}
			}
		}
	}

	nCXPosition -= VIRTUAL_POCKET_X_START;
	nCYPosition -= VIRTUAL_POCKET_Y_START;

	*n_Xpos = nCXPosition + nPockOffsetX*nContainerPitch_X_Cnt;
	*n_YPos = nCYPosition + nPockOffsetY*nContainerPitch_Y_Cnt;

	return nTotalPossibleCnt;
}

int CTaskAutoTrayUnloadPP::_MakeScheduleGrr_TrayPp_Pick_SetPlate(eStacker_Idx _conveyor_idx)
{
	int nSchdCount = m_listTrayPpSchedule.size();

	int nWorkablePicker[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = { DEF_UNWORKABLE };
	int nWorkablePikcerCnt = GetWorkable_CTrayHand(DEF_PICK, nWorkablePicker);

	auto conv_state = g_TaskAutoStacker[eSTACKER_02].GetConveyorState();
	if (conv_state != PickEnable_Device) {
		return m_listTrayPpSchedule.size() - nSchdCount;
	}

	if (nWorkablePikcerCnt > 0)
	{
		int nPocketCntX = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
		int nPocketCntY = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);

		// Device에 대한 2차원 배열 동적할당
		int** nArrDeviceData = new int*[nPocketCntY];
		for (int i = 0; i < nPocketCntY; i++)
		{
			nArrDeviceData[i] = new int[nPocketCntX];
			memset(nArrDeviceData[i], 0, sizeof(int)*nPocketCntX);
		}

		// Device Data를 넣어줌
		for (int j = 0; j < nPocketCntY; j++)
		{
			for (int i = 0; i < nPocketCntX; i++)
			{
				nArrDeviceData[j][i] = g_DMCont.m_dmDevice.GetDeviceExist(_conveyor_idx + eDeviceStage_STACKER_CONV_1, i, j);
			}
		}

		POINT ptPocket;
		int nReserveCnt = 0;
		int nArrPickWork[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = { DEF_WORK_FREE };
		for (int no_infinite = 0; no_infinite < 8; no_infinite++ )
		{
			// Init Array
			for (int i = 0; i < MAX_TRAY_PP_CNT_Y; i++)
			{
				memset(nArrPickWork[i], DEF_WORK_FREE, sizeof(int)*MAX_TRAY_PP_CNT_X);
			}

			nReserveCnt = FindPickerWork(DEF_PICK, nPocketCntX, nPocketCntY, nArrDeviceData, nWorkablePicker, ptPocket, nArrPickWork, FALSE, -2);
			if (nReserveCnt > 0)
			{
				// Make Command Start
				CTrayUnloadPPCmd_MovePickPlace *pCmd = new CTrayUnloadPPCmd_MovePickPlace;
				pCmd->m_Cmd.eStage = eLocIdxTrayPP(_conveyor_idx + LOC_IDX_TRAY_PP_CONV_1);

				for (int nPickY = 0; nPickY < MAX_TRAY_PP_CNT_Y; nPickY++) {
					for (int nPickX = 0; nPickX < (MAX_TRAY_PP_CNT_X); nPickX++) {
						char setting = m_pTaskTrayPP->m_TrayPP_picker_setting[nPickY][nPickX];
						if (setting == 'L' && nArrPickWork[nPickY][nPickX] == DEF_WORK_RESERVED) {
							pCmd->m_Cmd.vPicker.push_back(CPoint(nPickX, nPickY));
						}
					}
				}
				pCmd->m_Cmd.Pocket.SetPoint(ptPocket.x, ptPocket.y);
				pCmd->m_Cmd.ePickPlace = ePPCmd::ePick;
				// Make Command End

				// find same command
				BOOL bSameCmd = FALSE;
				std::find_if(m_listTrayPpSchedule.begin(), m_listTrayPpSchedule.end()
					, [&](CBaseSeqCmd* _pTrayPpCmd) -> bool
				{
					CTrayUnloadPPCmd_MovePickPlace* pTrayPpCmd = (CTrayUnloadPPCmd_MovePickPlace*)_pTrayPpCmd;
					if (pTrayPpCmd->m_Cmd.eStage == _conveyor_idx + LOC_IDX_TRAY_PP_CONV_1 && pTrayPpCmd->m_Cmd.Pocket == pCmd->m_Cmd.Pocket) {
						bSameCmd = TRUE;
						return true;
					}
					return false;
				});

				if (bSameCmd == FALSE && g_TaskSystemCtrl.IsOneCycleModeOn() == false) {
					m_listTrayPpSchedule.push_back(pCmd);
				}
				else {
					delete pCmd;
				}
			}
			else
			{
				break;
			}
		}

		// Device 2차원 배열 할당해제
		for (int i = 0; i < nPocketCntY; i++)
		{
			delete[] nArrDeviceData[i];
		}
		delete[] nArrDeviceData;
	}

	//새롭게 등록된 스케줄 횟수 리턴
	return m_listTrayPpSchedule.size() - nSchdCount;
}

int CTaskAutoTrayUnloadPP::ChkAxisZ_SafetyPos()
{
	int nJobCount = m_listTrayPpSchedule.size();
	if (m_pTaskTrayPP->GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready) { return nJobCount; }

	vector<CPoint> vCmdPicker;
	CPoint cPoint0_0(0, 0);
	vCmdPicker.push_back(cPoint0_0);

	int nRetLoadTbl_Z = m_pTaskTrayPP->ChkMotorPos(LOC_IDX_TRAY_PP_CONV_1, cPoint0_0, vCmdPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
	if (nRetLoadTbl_Z == ERR_NO_ERROR) { return nJobCount; }
	else {
		CTrayUnloadPPCmd_MovePos *pCmd = new CTrayUnloadPPCmd_MovePos;
		pCmd->m_Cmd.eStage = LOC_IDX_TRAY_PP_SAFETY;
		pCmd->m_Cmd.Pocket = cPoint0_0;
		pCmd->m_Cmd.vPicker = vCmdPicker;
		m_listTrayPpSchedule.push_back(pCmd);
	}

	return (m_listTrayPpSchedule.size() - nJobCount);
}

int CTaskAutoTrayUnloadPP::GetDeviceCnt_LoadPicker()
{
	int nRetDvcCnt = 0;

	for (int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++)
	{
		for (int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++)
		{
			// check Tray PP load picker Data
			if (g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TRAY_PP_1 + m_eTrayIdx, nX, nY) == TRUE && nX < MAX_TRAY_PP_CNT_X / 2) {
				nRetDvcCnt++;
			}
		}
	}

	return nRetDvcCnt;
}


/************************************************************************/
/* device 정보; empty, partial, full 조사 (setplate)                     */
/************************************************************************/
eDeviceFullEmptyCheckCode CTaskAutoTrayUnloadPP::SetplateDevFullEmptyCheck(eDeviceDataStage DvcLoc)
{
	int nRemainDvcCount = 0;
	int nMaxTrayXCount = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
	int nMaxTrayYCount = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);
	ASSERT(eDeviceStage_STACKER_CONV_1 <= DvcLoc && DvcLoc <= eDeviceStage_STACKER_CONV_7);

	eStacker_Idx stacker_idx = (eStacker_Idx)(eSTACKER_01 + DvcLoc - eDeviceStage_STACKER_CONV_1);
	eStacker_Conveyor_State state = g_TaskAutoStacker[stacker_idx].GetConveyorState();
	if (state == PickEnable_Device || state == PlaceEnable_Device) {
		for (int nY = 0; nY < nMaxTrayYCount; nY++)
		{
			for (int nX = 0; nX < nMaxTrayXCount; nX++)
			{
				if (g_DMCont.m_dmDevice.GetDeviceExist(DvcLoc, nX, nY) == DEF_EXIST) nRemainDvcCount++;
			}
		}
	}

	BOOL bAutoGrrUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);
	int nAutoGrrUnitCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrUnitCount);
	int nCompareCount = bAutoGrrUse == TRUE ? nAutoGrrUnitCount : nMaxTrayXCount*nMaxTrayYCount;

	if (nRemainDvcCount == 0) {
		return eEmpty;
	}
	else if (nRemainDvcCount == (nCompareCount)) {
		return eFull;
	}

	return ePatial;
}

//==============================================================================
// sub funtion for [_MakeSchedule_TrayPp_Place_LoadTable]
//==============================================================================
//picker device map -> bit mask
//==============================================================================
int CTaskAutoTrayUnloadPP::get_load_picker_exist_mask()
{
	int nPickerMask = 0x00;

	int bit_idx = 0;
	for (int row = 0; row < MAX_TRAY_PP_CNT_Y; row++) {
		for (int col = 0; col < MAX_TRAY_PP_CNT_X; col++) {

			bool device_exist = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TRAY_PP_1 + m_eTrayIdx, col, row) == DEF_EXIST;
			if (device_exist == true) {
				SETBIT(nPickerMask, bit_idx);
			}
			else {
				CLRBIT(nPickerMask, bit_idx);
			}

			bit_idx++;
		}
	}

	return nPickerMask;
}

int	CTaskAutoTrayUnloadPP::MakeJobTblIndex()
{
	int nTblWorkNo[eMaxLoadTblCount] = { eLoadTbl_4, eLoadTbl_3, eLoadTbl_2, eLoadTbl_1 };
	double dTime = (double)GetTickCount64();
	double dTblTime[eMaxLoadTblCount] = { dTime, dTime };

	int nLoadTblPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	int nLoadTblPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

	int nTblLoadDeviceCnt[MAX_LD_TBL_CNT];
	nTblLoadDeviceCnt[eLoadTbl_1] = g_TaskLoadTbl[eLoadTbl_1].GetLoadDeviceCnt();
	nTblLoadDeviceCnt[eLoadTbl_2] = g_TaskLoadTbl[eLoadTbl_2].GetLoadDeviceCnt();
	nTblLoadDeviceCnt[eLoadTbl_3] = g_TaskLoadTbl[eLoadTbl_3].GetLoadDeviceCnt();
	nTblLoadDeviceCnt[eLoadTbl_4] = g_TaskLoadTbl[eLoadTbl_4].GetLoadDeviceCnt();

	// scan loadtable & make data
	for (int i = 0; i < MAX_LD_TBL_CNT; i++)
	{
		int nTblRes = g_TaskLoadTbl[i].GetWorkReserve();
		int nTblWorkIdx = g_DMCont.m_dmShuttle.GB(BDM3_LdTbl1_WorkingEnd + i);
		if (nTblRes == RID_INIT_IDLE_RESERVE || nTblRes == RID_TRAY_PP_(RIDX_TRAY_PP_1) || nTblWorkIdx > 0)
		{// tray hand
			bool all_site_is_off = true;
			int  nStationDeviceCount = 0;
			std::vector< _ePressUnit > presses = m_test_modules_press_idx[i];
			for each(auto press_idx in presses) {
				all_site_is_off &= g_TaskPressUnit[press_idx].GetSocketOnCnt() == 0;
				nStationDeviceCount += g_TaskPressUnit[press_idx].ChkSiteInDvc();
			}

			if (all_site_is_off == true)
			{
				dTblTime[i] += 1000.0;
				continue;
			}

			int LoadDeviceCnt = g_TaskLoadTbl[i].GetLoadDeviceCnt();
			int UnloadDeviceCnt = g_TaskLoadTbl[i].GetUnloadDeviceCnt();

			if (LoadDeviceCnt == (MAX_LD_TBL_POCKET_CNT_Y*MAX_LD_TBL_POCKET_CNT_X / 2) && UnloadDeviceCnt == 0)
				dTblTime[i] += 100.0;
			else if ((g_TaskSystemCtrl.IsOneCycleModeOn() == true || g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == true) && nStationDeviceCount == 0
				/* @@@ : && g_TaskTestPP[i].GetHandDeviceCnt() == 0*/ && g_TaskTestPP[i].GetBufferTableCnt() == 0
				&& UnloadDeviceCnt == 0)
			{
				dTblTime[i] += 200.0;
			}
			else
			{
				if (nTblRes == RID_TEST_PP_(i))
				{
					dTblTime[i] += 4.0;
				}
				else
				{
					if (nTblLoadDeviceCnt[i] != 0)
						dTblTime[i] += 2.0;
					else
						dTblTime[i] += 3.0;
				}
			}
		}
		else
		{// test hand
			dTblTime[i] += 100.0;
		}
	}

	int nRetTblIndex = -1;
	if (dTblTime[eLoadTbl_1] <= (dTime + 10.0) || dTblTime[eLoadTbl_2] <= (dTime + 10.0))
	{
		if (dTblTime[eLoadTbl_1] == dTblTime[eLoadTbl_2])
		{
			vector<CTime> vArrivedTblTm;
			vArrivedTblTm.push_back(g_TaskLoadTbl[eLoadTbl_1].GetMoveSetplateTm());
			vArrivedTblTm.push_back(g_TaskLoadTbl[eLoadTbl_2].GetMoveSetplateTm());

			int nMinIndex = min_element(vArrivedTblTm.begin(), vArrivedTblTm.end()) - vArrivedTblTm.begin();

			nRetTblIndex = nMinIndex;
		}
		else if (dTblTime[eLoadTbl_1] > dTblTime[eLoadTbl_2])
		{
			nRetTblIndex = eLoadTbl_2;
		}
		else
		{
			nRetTblIndex = eLoadTbl_1;
		}

		m_nTblReworkNo = nRetTblIndex;
		return nRetTblIndex;

	}

	return -1;
}

int CTaskAutoTrayUnloadPP::_MakeSchedule_TrayPp_Pick_LoadTable(int nIdxLoadTbl)
{
	int nSchdCount = m_listTrayPpSchedule.size();

	//_FindPickToLoadTable( nIdxLoadTbl );
	eLocIdxTrayPP loc = static_cast<eLocIdxTrayPP>(nIdxLoadTbl);// == 0 ? LOC_IDX_TRAY_PP_TBL_1 : LOC_IDX_TRAY_PP_TBL_2;
	SetNewTrayPP_Schedule(DEF_PICK, loc, _eUsePicker_Unload);

	//새롭게 등록된 스케줄 횟수 리턴
	return (m_listTrayPpSchedule.size() - nSchdCount);
}


int CTaskAutoTrayUnloadPP::_MakeJobPickFromSetplate(int nWorkingTable)
{
	int nSchdCount = m_listTrayPpSchedule.size();

	eDeviceDataStage ArrLoadSetplateLoc[1] = { eDeviceStage_STACKER_CONV_3 };

	//setplate 1,2의 로딩 시간을 비교한다.
	//double nSetplateTime[2] = { 0,0 };
	//nSetplateTime[0] = g_DMCont.m_dmConveyor.GD(DDM6_TRAY_LOAD_TIME1);

	int nPickerDvcPattern[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = { DEF_NOT_EXIST, };
	int nAvailableCount = GetDeviceExist_CTrayHand(DEF_PICK, (eDeviceDataStage)0, (int*)nPickerDvcPattern, MAX_TRAY_PP_CNT_X, MAX_TRAY_PP_CNT_Y);

	// loading table의 위치에 따른 hand 작업 불가하도록 만들어주여야 함.
	int nNotAvailableCount = 0;

	//if (DEF_8PICKER_PRE_PICK_MODE == 0)
	{
		for (int nY = MAX_LD_TBL_POCKET_CNT_Y / 2; nY < MAX_LD_TBL_POCKET_CNT_Y; nY++)
		{
			for (int nX = 0; nX < MAX_LD_TBL_POCKET_CNT_X; nX++)
			{
				int nExist = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_LD_TBL_1 + nWorkingTable, nX, nY);
				// table에 있고 hand에 비어있으면 있는걸로 인식하게 만들어서 지운다.
				if (nExist == TRUE)
				{
					// 퐁당퐁당이므로 tbl pocket 위치대비 hand 위치는 엇갈려야 하므로 재계산
					int nCalX = (nX % 3 == 0 ? nX : nX + (nX % 2 == 1 ? 1 : -1));

					// hand에 없는 경우에만 counting 있는 경우면 이미 제외
					if (nPickerDvcPattern[nY - 2][nCalX] == eDeviceData_None)
						nNotAvailableCount++;

					nPickerDvcPattern[nY - 2][nCalX] = eDeviceData_Exist;
				}
			}
		}
		nAvailableCount -= nNotAvailableCount;
	}

	//nWorkingTable
	/*
	*	작업 가능한 picker가 없으면 볼 것도 없다.
	*/
	if (nAvailableCount > 0) {
		// 순서대로 스케줄을 뽑아 보자
		for (int i = 0; i < _countof(ArrLoadSetplateLoc); i++) {
			eDeviceDataStage SelectedSetplateDvcLoc = ArrLoadSetplateLoc[i];

			/*	Setplate가 레디 상태일 경우에만.~ */
			eStacker_Idx nSetplateObj_index = (eStacker_Idx)(/*_eSetplateIdx::*/eSTACKER_03 + (SelectedSetplateDvcLoc - eDeviceStage_STACKER_CONV_3));
			eStacker_Conveyor_State conveyor_state = g_TaskAutoStacker[nSetplateObj_index].GetConveyorState();
			if (conveyor_state != PickEnable_Device)
				continue;

			/* 주의!
			*	스케줄이 확정된 Stage로 모션을 지정할 경우 eLocIdxTrayPP 값을 사용해야 함.
			*/
			eLocIdxTrayPP TrayPpLoc = (eLocIdxTrayPP)(LOC_IDX_TRAY_PP_CONV_1 + (SelectedSetplateDvcLoc - eDeviceStage_STACKER_CONV_1));
			int nPockX, nPockY;

			int nArrPickerAssign[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X];

			/*
			*	Picker와 일치하는 pattern을 찾기 위해
			*	300x300 버퍼 공간에 C-Tray device 정보를 채운다.
			*	C-Tray (0,0) = 버퍼 공간 (100,100) 위치
			*/
			int nArrVirtualCTray[VIRTUAL_POCKET_Y_MAX][VIRTUAL_POCKET_X_MAX]; // data 검색용 버퍼
			memset(nArrVirtualCTray, 0xFF, sizeof(nArrVirtualCTray));

			FillDeviceData_VirtualCTray(SelectedSetplateDvcLoc, (int*)nArrVirtualCTray, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, VIRTUAL_POCKET_X_START, VIRTUAL_POCKET_Y_START);

			/*
			* 가능한 많은 picker에 할당하기 위한 loop
			*/
			int nRetCode = 0;
			// 			for(int no_infinite=0; no_infinite<8; no_infinite++ /*anti-no_infinite loop, max 8 schedules*/) 
			// 			{
			nRetCode = FindPickFromSetplate(SelectedSetplateDvcLoc, nArrVirtualCTray, nPickerDvcPattern, DEF_PICK, &nPockX, &nPockY, nArrPickerAssign, eRobotWorkDirTopLeft, FALSE);
			if (nRetCode > 0) {
				CTrayUnloadPPCmd_MovePickPlace* pCmd = new CTrayUnloadPPCmd_MovePickPlace();
				pCmd->m_Cmd.eStage = TrayPpLoc;

				for (int nPickY = 0; nPickY < MAX_TRAY_PP_CNT_Y; nPickY++) {
					for (int nPickX = 0; nPickX < (MAX_TRAY_PP_CNT_X); nPickX++) {
						if (nArrPickerAssign[nPickY][nPickX] != 0) {
							pCmd->m_Cmd.vPicker.push_back(CPoint(nPickX, nPickY));
						}
					}
				}

				pCmd->m_Cmd.Pocket.SetPoint(nPockX, nPockY);
				pCmd->m_Cmd.ePickPlace = ePick;

				// find same command
				BOOL bSameCmd = FALSE;
				std::find_if(m_listTrayPpSchedule.begin(), m_listTrayPpSchedule.end()
					, [&](CBaseSeqCmd* _pTrayPpCmd) -> bool
				{
					CTrayUnloadPPCmd_MovePickPlace* pTrayPpCmd = (CTrayUnloadPPCmd_MovePickPlace*)_pTrayPpCmd;
					if (pTrayPpCmd->m_Cmd.eStage == TrayPpLoc && pTrayPpCmd->m_Cmd.Pocket == pCmd->m_Cmd.Pocket) {
						bSameCmd = TRUE;
						return true;
					}
					return false;
				});

				// no search same command
				if (bSameCmd == FALSE && g_TaskSystemCtrl.IsOneCycleModeOn() == false) {
					m_listTrayPpSchedule.push_back(pCmd);
					break;
				}
				else {
					delete pCmd;
				}
			}
			else {
				/*
				*	더이상 만들 수 없음.
				*/
				break;
			}
			//			}/*for(int no_infinite*/
		}/*for(int x*/
	}/*if( nAvailableCount > 0)*/

	 //새롭게 등록된 스케줄 횟수 리턴
	return m_listTrayPpSchedule.size() - nSchdCount;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CTaskAutoTrayUnloadPP::_MakeSchedule_TrayPp_Pick_SetPlate(CPoint &vPocket, std::vector<CPoint> &vPicker)
{
	eDeviceDataStage ArrLoadSetplateLoc[ 1 ] = { eDeviceStage_STACKER_CONV_3 };

	//setplate 1,2의 로딩 시간을 비교한다.
	double nSetplateTime[ 2 ] = { 0,0 };
	nSetplateTime[ 0 ] = g_DMCont.m_dmConveyor.GD( DDM6_TRAY_LOAD_TIME1 );

	int nPickerDvcPattern[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ] = { DEF_NOT_EXIST, };
	int nAvailableCount = GetDeviceExist_CTrayHand( DEF_PICK, ( eDeviceDataStage )0, ( int* )nPickerDvcPattern, MAX_TRAY_PP_CNT_X, MAX_TRAY_PP_CNT_Y );
	/*
	*	작업 가능한 picker가 없으면 볼 것도 없다.
	*/
	if( nAvailableCount > 0 ) {
		// 순서대로 스케줄을 뽑아 보자
		for( int i = 0; i < _countof( ArrLoadSetplateLoc ); i++ ) {
			eDeviceDataStage SelectedSetplateDvcLoc = ArrLoadSetplateLoc[ i ];

			/*	Setplate가 레디 상태일 경우에만.~ */
			eStacker_Idx nSetplateObj_index = ( eStacker_Idx )(/*_eSetplateIdx::*/eSTACKER_03 + ( SelectedSetplateDvcLoc - eDeviceStage_STACKER_CONV_3) );
			eStacker_Conveyor_State conveyor_state = g_TaskAutoStacker[ nSetplateObj_index ].GetConveyorState();
			if( conveyor_state != PickEnable_Device )
				continue;

			/* 주의!
			*	스케줄이 확정된 Stage로 모션을 지정할 경우 eLocIdxTrayPP 값을 사용해야 함.
			*/
			eLocIdxTrayPP TrayPpLoc = ( eLocIdxTrayPP )( LOC_IDX_TRAY_PP_CONV_1 + ( SelectedSetplateDvcLoc - eDeviceStage_STACKER_CONV_1 ) );
			int nPockX, nPockY;

			int nArrPickerAssign[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ];

			/*
			*	Picker와 일치하는 pattern을 찾기 위해
			*	300x300 버퍼 공간에 C-Tray device 정보를 채운다.
			*	C-Tray (0,0) = 버퍼 공간 (100,100) 위치
			*/
			int nArrVirtualCTray[ VIRTUAL_POCKET_Y_MAX ][ VIRTUAL_POCKET_X_MAX ]; // data 검색용 버퍼
			memset( nArrVirtualCTray, 0xFF, sizeof( nArrVirtualCTray ) );

			FillDeviceData_VirtualCTray( SelectedSetplateDvcLoc, ( int* )nArrVirtualCTray, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, VIRTUAL_POCKET_X_START, VIRTUAL_POCKET_Y_START );

			/*
			* 가능한 많은 picker에 할당하기 위한 loop
			*/
			int nRetCode = 0;
			nRetCode = FindPickFromSetplate( SelectedSetplateDvcLoc, nArrVirtualCTray, nPickerDvcPattern, DEF_PICK, &nPockX, &nPockY, nArrPickerAssign, eRobotWorkDirTopLeft, 0);
			if( nRetCode > 0 ) {
				std::vector<CPoint> vPkr;
				for( int nPickY = 0; nPickY < MAX_TRAY_PP_CNT_Y; nPickY++ ) {
					for( int nPickX = 0; nPickX < ( MAX_TRAY_PP_CNT_X ); nPickX++ ) {
						if( nArrPickerAssign[ nPickY ][ nPickX ] != 0 ) {
							vPkr.push_back( CPoint( nPickX, nPickY ) );
						}
					}
				}
				vPicker = vPkr;
				vPocket = CPoint(nPockX, nPockY);
				return eExcutingStep_FindJob;
			}	
		}
	}

	return eExcutingStep_WaitOtherJob;
}

int	CTaskAutoTrayUnloadPP::_MakeJobPickFromTable(int nIdxLoadTbl)
{
	int nSchdCount = m_listTrayPpSchedule.size();

	int nAvailable = 0;

	ST_DD_DEVICE stHandDvc;

	int nStdPicker = 1;

RETRY_CHECKPLACE:

	int nPickerDvcPattern[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = { 0, };
	for (int nPkrY = 0; nPkrY < MAX_TRAY_PP_CNT_Y; nPkrY++)
	{
		for (int nPkrX = 0; nPkrX < MAX_TRAY_PP_CNT_X; nPkrX++)
		{
			if (nStdPicker == (nPkrX % 2))
			{
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TRAY_PP_1 + m_eTrayIdx, nPkrX, nPkrY, &stHandDvc);
				if (stHandDvc.sExist == eDeviceData_Exist)
				{
					nPickerDvcPattern[nPkrY][nPkrX] = eDeviceData_Exist;

				}
				else
				{
					int nCalxPos = (nPkrX % 3 == 0 ? nPkrX : nPkrX + (nPkrX % 2 == 1 ? 1 : -1));
					int nExist = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_LD_TBL_1 + nIdxLoadTbl, nCalxPos, nPkrY);
					if (nExist)
						nAvailable++;
					else
						nPickerDvcPattern[nPkrY][nPkrX] = eDeviceData_Exist;
				}

			}
			else
			{
				nPickerDvcPattern[nPkrY][nPkrX] = eDeviceData_Exist;
			}
		}
	}

	if (nAvailable == 0)
	{
		nStdPicker--;

		if (nStdPicker >= 0)
			goto RETRY_CHECKPLACE;
	}

	if (nAvailable) {
		int nArrVirtualTbl[VIRTUAL_POCKET_Y_MAX][VIRTUAL_POCKET_X_MAX];
		memset(nArrVirtualTbl, /*nPickPlace == DEF_PICK ?*/ 0x00 /*: 0xFF*/, sizeof(nArrVirtualTbl));
		FillDvc_VirtualTable((eDeviceDataStage)(eDeviceStage_LD_TBL_1 + nIdxLoadTbl), (int*)nArrVirtualTbl, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, DEF_PICK);

		int nUseJobPkr[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = { 0, };
		int nPockX = -1, nPockY = -1;
		int nRetCode = _FindPickPlaceFromTable8Picker(LOC_IDX_TRAY_PP_TBL_1 + nIdxLoadTbl, nArrVirtualTbl, nPickerDvcPattern, DEF_PICK, &nPockX, &nPockY, nUseJobPkr);
		if (nRetCode > 0) {
			std::vector<CPoint> vPkr;
			for (int nPickX = 0; nPickX < MAX_TEST_PP_PKR_CNT_X; nPickX++) {
				for (int nPickY = 0; nPickY < MAX_TEST_PP_PKR_CNT_Y; nPickY++) {
					if (nUseJobPkr[nPickY][nPickX] != 0) {
						vPkr.push_back(CPoint(nPickX, nPickY));
					}
				}
			}

			CTrayUnloadPPCmd_MovePickPlace* pCmd = new CTrayUnloadPPCmd_MovePickPlace;
			pCmd->m_Cmd.eStage     = eLocIdxTrayPP(LOC_IDX_TRAY_PP_TBL_1 + nIdxLoadTbl);
			pCmd->m_Cmd.vPicker    = vPkr;
			pCmd->m_Cmd.Pocket     = CPoint(nPockX, nPockY);
			pCmd->m_Cmd.ePickPlace = ePick;

			// find same command
			BOOL bSameCmd = FALSE;
			std::find_if(m_listTrayPpSchedule.begin(), m_listTrayPpSchedule.end()
				, [&](CBaseSeqCmd* _pTrayPpCmd) -> bool
			{
				CTrayUnloadPPCmd_MovePickPlace* pTrayPpCmd = (CTrayUnloadPPCmd_MovePickPlace*)_pTrayPpCmd;
				if (pTrayPpCmd->m_Cmd.eStage == LOC_IDX_TRAY_PP_TBL_1 + nIdxLoadTbl && pTrayPpCmd->m_Cmd.Pocket == pCmd->m_Cmd.Pocket) {
					bSameCmd = TRUE;
					return true;
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

	//새롭게 등록된 스케줄 횟수 리턴
	return (m_listTrayPpSchedule.size() - nSchdCount);
}

int	CTaskAutoTrayUnloadPP::_MakeJobPlaceToTable(int nIdxLoadTbl)
{
	int nSchdCount = m_listTrayPpSchedule.size();

	int nAvailable = 0;
	int nPickerDvcPattern[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = { 0, };
	ST_DD_DEVICE stHandDvc;

	for (int nPkrY = 0; nPkrY < MAX_TRAY_PP_CNT_Y; nPkrY++)
	{
		for (int nPkrX = 0; nPkrX < MAX_TRAY_PP_CNT_X; nPkrX++)
		{
			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TRAY_PP_1 + m_eTrayIdx, nPkrX, nPkrY, &stHandDvc);
			if (stHandDvc.sExist == eDeviceData_Exist && strcmp(stHandDvc.szBinResult, DEF_NEW_LOAD_DEVICE) == 0)
			{
				nPickerDvcPattern[nPkrY][nPkrX] = eDeviceData_Exist;
				nAvailable++;
			}
		}
	}

	if (nAvailable) {
		int nArrVirtualTbl[VIRTUAL_POCKET_Y_MAX][VIRTUAL_POCKET_X_MAX];
		memset(nArrVirtualTbl, /*nPickPlace == DEF_PICK ? 0x00 :*/ 0xFF, sizeof(nArrVirtualTbl));
		FillDvc_VirtualTable((eDeviceDataStage)(eDeviceStage_LD_TBL_1 + nIdxLoadTbl), (int*)nArrVirtualTbl, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, DEF_PLACE);

		int nUseJobPkr[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = { 0, };
		int nPockX = -1, nPockY = -1;
		int nRetCode = _FindPickPlaceFromTable8Picker(LOC_IDX_TRAY_PP_TBL_1 + nIdxLoadTbl, nArrVirtualTbl, nPickerDvcPattern, DEF_PLACE, &nPockX, &nPockY, nUseJobPkr);
		if (nRetCode > 0) {
			std::vector<CPoint> vPkr;
			for (int nPickX = 0; nPickX < MAX_TEST_PP_PKR_CNT_X; nPickX++) {
				for (int nPickY = 0; nPickY < MAX_TEST_PP_PKR_CNT_Y; nPickY++) {
					if (nUseJobPkr[nPickY][nPickX] != 0) {
						vPkr.push_back(CPoint(nPickX, nPickY));
					}
				}
			}

			CTrayUnloadPPCmd_MovePickPlace* pCmd = new CTrayUnloadPPCmd_MovePickPlace;
			pCmd->m_Cmd.eStage = eLocIdxTrayPP(LOC_IDX_TRAY_PP_TBL_1 + nIdxLoadTbl);
			pCmd->m_Cmd.vPicker = vPkr;
			pCmd->m_Cmd.Pocket = CPoint(nPockX, nPockY);;
			pCmd->m_Cmd.ePickPlace = ePick;

			// find same command
			BOOL bSameCmd = FALSE;
			std::find_if(m_listTrayPpSchedule.begin(), m_listTrayPpSchedule.end()
				, [&](CBaseSeqCmd* _pTrayPpCmd) -> bool
			{
				CTrayUnloadPPCmd_MovePickPlace* pTrayPpCmd = (CTrayUnloadPPCmd_MovePickPlace*)_pTrayPpCmd;
				if (pTrayPpCmd->m_Cmd.eStage == LOC_IDX_TRAY_PP_TBL_1 + nIdxLoadTbl && pTrayPpCmd->m_Cmd.Pocket == pCmd->m_Cmd.Pocket) {
					bSameCmd = TRUE;
					return true;
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

	//새롭게 등록된 스케줄 횟수 리턴
	return (m_listTrayPpSchedule.size() - nSchdCount);
}

int CTaskAutoTrayUnloadPP::_MakeSchedule_TrayPp_Place_LoadTable(int nIdxLoadTbl)
{
	int nSchdCount = m_listTrayPpSchedule.size();

	eLocIdxTrayPP loc = static_cast<eLocIdxTrayPP>(nIdxLoadTbl);// == 0 ? LOC_IDX_TRAY_PP_TBL_1 : LOC_IDX_TRAY_PP_TBL_2;
	SetNewTrayPP_Schedule(DEF_PLACE, loc, _eUsePicker_Load);

	//새롭게 등록된 스케줄 횟수 리턴
	return (m_listTrayPpSchedule.size() - nSchdCount);
}

/*=============================================================================
	DESCRIPT : Table이 속한 Site Group의 예약이 없고, Table이 TrayPP쪽으로 와 있는 것을 찾는다.
	           여러 개 찾아진다면 도착한 시간이 빠르면서 제일 좌측에 있는 순서로 정한다.
	           제외하고 싶은 SiteGroupNo가 있다면 인자로 넘겨받는다.
	           (Site Group은 Table 번호와 같다고 생각해도 된다)
	KEY WORD :
	ARGUMENT : vExceptSiteGroupNo = 진행을 이미 해서 제외하고 싶은 SiteGroupNo의 배열
	RETURN   : nRetTableIdx = Table Index (-1: Not found or 0 ~ 3)
==============================================================================*/
int CTaskAutoTrayUnloadPP::FindTableIndex(_eFindTableOpt eFindTableOpt /*= _eFindTableOpt_Normal*//*, std::vector<int> vExceptSiteGroupNo*/ /*= std::vector<int>(0)*/)
{
	int nRetTableIdx = -1;
	vector<int> vTempFindIndex, vTableIndex;
	vTempFindIndex.clear();
	vTableIndex.clear();
	BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();

	// Find - Site Group이 예약되지 않고, TrayPP쪽으로 와있는 Table의 Index
	for (int i = 0; i < MAX_LD_TBL_CNT; i++)
	{
		bool all_site_is_off = true;
		std::vector< _ePressUnit > presses = m_test_modules_press_idx[i];
		for each(auto press_idx in presses) {
			if (bAutoGrrUse == TRUE)
				all_site_is_off &= g_TaskPressUnit[press_idx].GetSocketOnCntGrr() == 0;
			else
				all_site_is_off &= g_TaskPressUnit[press_idx].GetSocketOnCnt() == 0;
		}

		int site_group_reserved = g_DMCont.m_dmTestPP.GN(NDM04_SITEGROUP1_RESERVED + i);
		int work_reserve = g_TaskLoadTbl[i].GetWorkReserve();
		if (site_group_reserved == DEF_WORK_FREE
			&& work_reserve == RID_TRAY_PP_(RIDX_TRAY_PP_1)
			&& all_site_is_off != true
			&& g_TaskLoadTbl[i].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready
			&& g_TaskLoadTbl[i].ChkMotorPos(LOC_IDX_LD_TBL_TEST_SIDE_1, CTaskLoadTable::eAxisAll) == ERR_NO_ERROR)
		{
			vTempFindIndex.push_back(i);
		}
	}

	// Not found table
	if (vTempFindIndex.empty() == true)
	{
		return nRetTableIdx;
	}

	int nTblIdx = 0;
	int nLoadDeviceCnt = 0, nUnloadDeviceCnt = 0;
	switch (eFindTableOpt)
	{
	case _eFindTableOpt_PickFromTableUArea_byUnloadPicker:
	{
		for (vector<int>::size_type i = 0; i < vTempFindIndex.size(); i++)
		{
			nTblIdx = vTempFindIndex[i];
			nUnloadDeviceCnt = g_TaskLoadTbl[nTblIdx].GetUnloadDeviceCnt();

			if (nUnloadDeviceCnt > 0)
			{
				vTableIndex.push_back(nTblIdx);
			}
		}
	}break;

	case _eFindTableOpt_PickFromTableAll_byUnloadPicker:
	{
		for (vector<int>::size_type i = 0; i < vTempFindIndex.size(); i++)
		{
			nTblIdx = vTempFindIndex[i];
			nLoadDeviceCnt = g_TaskLoadTbl[nTblIdx].GetLoadDeviceCnt();
			nUnloadDeviceCnt = g_TaskLoadTbl[nTblIdx].GetUnloadDeviceCnt();

			if (nLoadDeviceCnt > 0 || nUnloadDeviceCnt > 0)
			{
				vTableIndex.push_back(nTblIdx);
			}
		}
	}break;

	case _eFindTableOpt_PlaceToTableLArea_byUnloadPicker:
	{
		bool bFindExcept = false;
		for (vector<int>::size_type i = 0; i < vTempFindIndex.size(); i++)
		{
			nTblIdx = vTempFindIndex[i];
			int nLoadEmptyPocketCnt = g_TaskLoadTbl[nTblIdx].GetLoadAreaEmptyPocketCnt();

			vector<int> vExceptSiteGroupNo;
			vExceptSiteGroupNo.clear();
			GetExceptSiteGroup_FromUnloadPicker(&vExceptSiteGroupNo, nTblIdx);

			// 제외할 Table Index인지 확인한다(제외할 Index는 이미 진행이 완료된 놈)
			bFindExcept = false;
			for (vector<int>::size_type j = 0; j < vExceptSiteGroupNo.size(); j++)
			{
				if (nTblIdx == vExceptSiteGroupNo[j])
				{
					bFindExcept = true;
					break;
				}
			}

			if (nLoadEmptyPocketCnt > 0 && bFindExcept != true)
			{
				vTableIndex.push_back(nTblIdx);
			}
		}
	}break;

	case _eFindTableOpt_Normal:
	
	default:
	{
		vTableIndex = vTempFindIndex;
	}break;
	}

	int nTblFastIdx = -1;
	CTime cCurTime = CTime::GetCurrentTime();
	cCurTime = cCurTime + 9999;
	if (vTableIndex.size() > 1)
	{
		int nCompareValue = INT_MAX;
		for (vector<int>::size_type i = 0; i < vTableIndex.size(); i++)
		{
			nTblIdx = vTableIndex[i];
			if (cCurTime > g_TaskLoadTbl[nTblIdx].GetMoveSetplateTm())
			{
				cCurTime = g_TaskLoadTbl[nTblIdx].GetMoveSetplateTm();
				nTblFastIdx = nTblIdx;
			}
		}

		nRetTableIdx = nTblFastIdx;
	}
	else if (vTableIndex.size() == 1)	// 1개만 찾은 경우
	{
		nRetTableIdx = vTableIndex[0];
	}

	return nRetTableIdx;
}


int CTaskAutoTrayUnloadPP::_MakeScheduleGrr_TrayPp_Pick_LoadTable(int nIdxLoadTbl, bool bAutoGrrCancel/* = false*/)
{
	int nSchdCount = m_listTrayPpSchedule.size();

	eLocIdxTrayPP loc = static_cast<eLocIdxTrayPP>(nIdxLoadTbl);// == 0 ? LOC_IDX_TRAY_PP_TBL_1 : LOC_IDX_TRAY_PP_TBL_2;
	SetNewTrayPP_Schedule(DEF_PICK, loc, _eUsePicker_Unload);
	int nWorkablePicker[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ] = { DEF_UNWORKABLE };
	int nWorkablePikcerCnt = GetWorkable_CTrayHand_forLoadTable( DEF_PICK, nWorkablePicker );

	if( nWorkablePikcerCnt > 0 )
	{
		int nPocketCntX = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Div_X );
		int nPocketCntY = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Div_Y );

		// Device에 대한 2차원 배열 동적할당
		int** nArrDeviceData = new int*[ nPocketCntY ];
		for( int i = 0; i < nPocketCntY; i++ )
		{
			nArrDeviceData[ i ] = new int[ nPocketCntX ];
			memset( nArrDeviceData[ i ], 0, sizeof( int )*nPocketCntX );
		}

		// Device Data를 넣어줌
		for( int nPocketY = 0; nPocketY < nPocketCntY; nPocketY++ )
		{
			for( int nPocketX = 0; nPocketX < nPocketCntX; nPocketX++ )
			{
				// AutoGRR Cancel일 경우에는 Unload picker로 Table의 모든 영역을 Pick해야 한다
				if( bAutoGrrCancel != true )
				{
					// Unload picker로 Table의 UnloadArea에서 Pick을 하므로
					// LoadArea에는 Device가 없다고 가상으로 체운다.
					char setting = g_TaskLoadTbl[ nIdxLoadTbl ].m_LoadTbl_pocket_setting[ nPocketY ][ nPocketX ];
					if( setting == 'L' ) {
						nArrDeviceData[ nPocketY ][ nPocketX ] = DEF_NOT_EXIST;
						continue;
					}
				}

				nArrDeviceData[ nPocketY ][ nPocketX ] = g_DMCont.m_dmDevice.GetDeviceExist( eDeviceStage_LD_TBL_1 + nIdxLoadTbl, nPocketX, nPocketY );
			}
		}

		POINT ptPocket;
		int nReserveCnt = 0;
		int nArrPickWork[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ] = { DEF_WORK_FREE };
		for( int no_infinite = 0; no_infinite < 1; no_infinite++ )
		{
			// Init Array
			memset( nArrPickWork, DEF_WORK_FREE, sizeof( int[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ] ) );

			nReserveCnt = FindPickerWork( DEF_PICK, nPocketCntX, nPocketCntY, nArrDeviceData, nWorkablePicker, ptPocket, nArrPickWork );
			if( nReserveCnt > 0 )
			{
				// Make Command Start
				CTrayUnloadPPCmd_MovePickPlace* pCmd = new CTrayUnloadPPCmd_MovePickPlace();
				pCmd->m_Cmd.eStage = ( eLocIdxTrayPP )( LOC_IDX_TRAY_PP_TBL_1 + nIdxLoadTbl );

				BOOL bFindExeption = FALSE;

				for( int nPickY = 0; nPickY < MAX_TRAY_PP_CNT_Y; nPickY++ ) {
					//for(int nPickX=MAX_TRAY_PP_CNT_X/2; nPickX<MAX_TRAY_PP_CNT_X; nPickX++){ Device Data를 조작해서 넣으므로 의미 없음
					for( int nPickX = 0; nPickX < MAX_TRAY_PP_CNT_X; nPickX++ ) {
						if( nArrPickWork[ nPickY ][ nPickX ] == DEF_WORK_RESERVED ) {
							pCmd->m_Cmd.vPicker.push_back( CPoint( nPickX, nPickY ) );
						}
					}
				}
				// Make Command End

				int nPocketPosX = 0, nPocketPosY = 0;
				nPocketPosX = ptPocket.x;
				nPocketPosY = ptPocket.y;

				pCmd->m_Cmd.Pocket.SetPoint( nPocketPosX, nPocketPosY/*ptPocket.x, ptPocket.y*/ );
				pCmd->m_Cmd.ePickPlace = ePick;

				// find same command
				BOOL bSameCmd = FALSE;
				std::find_if( m_listTrayPpSchedule.begin(), m_listTrayPpSchedule.end()
							  , [ & ] ( CBaseSeqCmd* _pTrayPpCmd ) -> bool
				{
					CTrayUnloadPPCmd_MovePickPlace* pTrayPpCmd = ( CTrayUnloadPPCmd_MovePickPlace* )_pTrayPpCmd;
					if( pTrayPpCmd->m_Cmd.eStage == LOC_IDX_TRAY_PP_TBL_1 + nIdxLoadTbl && pTrayPpCmd->m_Cmd.Pocket == pCmd->m_Cmd.Pocket ) {
						bSameCmd = TRUE;
						return true;
					}
					return false;
				} );

				// not add same command
				if( bSameCmd == FALSE ) {
					m_listTrayPpSchedule.push_back( pCmd );
				}
				else {
					delete pCmd;
				}
			}
			else
			{
				break;
			}
		}

		// Device 2차원 배열 할당해제
		for( int i = 0; i < nPocketCntY; i++ )
		{
			delete[] nArrDeviceData[ i ];
		}
		delete[] nArrDeviceData;
	}

	//새롭게 등록된 스케줄 횟수 리턴
	return m_listTrayPpSchedule.size() - nSchdCount;
}


int CTaskAutoTrayUnloadPP::_MakeScheduleGrr_TrayPp_Place_LoadTable(int nIdxLoadTbl, _eUsePicker ePicker)
{
	ASSERT(nIdxLoadTbl >= eLoadTbl_1 && nIdxLoadTbl <= eLoadTbl_4);

	int nSchdCount = m_listTrayPpSchedule.size();

	eLocIdxTrayPP loc = static_cast<eLocIdxTrayPP>(nIdxLoadTbl);// == 0 ? LOC_IDX_TRAY_PP_TBL_1 : LOC_IDX_TRAY_PP_TBL_2;
	SetNewTrayPP_Schedule(DEF_PLACE, loc, ePicker);

	return m_listTrayPpSchedule.size() - nSchdCount;
}


int CTaskAutoTrayUnloadPP::CalcUsableSiteGroupCnt()
{
	int nRetCnt = 0;
	for (int i = 0; i < MAX_TEST_PP_CNT; i++)
	{
		bool all_site_is_off = true;
		std::vector< _ePressUnit > presses = m_test_modules_press_idx[i];
		for each(auto press_idx in presses) {
			all_site_is_off &= g_TaskPressUnit[press_idx].GetSocketOnCntGrr() == 0;
		}

		if (all_site_is_off != true)
		{
			nRetCnt++;
		}
	}

	return nRetCnt;
}

int CTaskAutoTrayUnloadPP::GetGrrEndDeviceCnt_FromUnloadPicker()
{
	int nRetCnt = 0;
	ST_DD_DEVICE stDevice;
	for (int y = 0; y < MAX_TRAY_PP_CNT_Y; y++)
	{
		for (int x = 0; x < MAX_TRAY_PP_CNT_X; x++)
		{
			char setting = m_pTaskTrayPP->m_TrayPP_picker_setting[y][x];
			if (setting != 'U') continue;

			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TRAY_PP_1 + m_eTrayIdx, x, y, &stDevice);

			if (stDevice.sExist == eDvcExist && (stDevice.chkAutoGrrCompelte() == TRUE || g_TaskSystemCtrl.GetAutoGrrCancel() == TRUE))
			{
				nRetCnt++;
			}
		}
	}

	return nRetCnt;
}


int CTaskAutoTrayUnloadPP::_MakeScheduleGrr_TrayPp_Place_UnloadSetPlate()
{
	int nSchdCount = m_listTrayPpSchedule.size();

	int nHandAvailable[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = { 0, };
	int nRemainDevice = 0;
	for (int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++)
	{
		for (int nX = 0; nX < (MAX_TRAY_PP_CNT_X); nX++)
		{
			char setting = m_pTaskTrayPP->m_TrayPP_picker_setting[nY][nX];
			int nExist = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TRAY_PP_1 + m_eTrayIdx, nX, nY);

			if (setting == 'U' && nExist == DEF_EXIST) {
				nHandAvailable[nY][nX] = DEF_EXIST;
				nRemainDevice++;
			}
		}
	}

	if (nRemainDevice > 0)
	{
		eStacker_Conveyor_State state = g_TaskAutoStacker[eSTACKER_04].GetConveyorState();
		if ( state != PlaceEnable_Device )
			return m_listTrayPpSchedule.size() - nSchdCount;

		int nPockX = -1, nPockY = -1;
		int nArrPickerAssign[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = { 0, };

		/*
		*	Picker와 일치하는 pattern을 찾기 위해
		*	300x300 버퍼 공간에 C-Tray device 정보를 채운다.
		*	C-Tray (0,0) = 버퍼 공간 (100,100) 위치
		*/
		int nArrVirtualCTray[VIRTUAL_POCKET_Y_MAX][VIRTUAL_POCKET_X_MAX]; // data 검색용 버퍼
		//int nHandAvailable[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = { 0, };
		memset(nArrVirtualCTray, 0xFF, sizeof(nArrVirtualCTray));

		/*
		*	가상의 공간에 C-Tray 디바이스 맵을 배치한다.
		*/
		FillDeviceData_VirtualCTray(eDeviceStage_STACKER_CONV_4, (int*)nArrVirtualCTray, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, VIRTUAL_POCKET_X_START, VIRTUAL_POCKET_Y_START);

		/*
		*	place 할 수 있는 pattern을 찾는다.
		*/
		int nRetCode = 0;
		nRetCode = FindSortToSetplate(eDeviceStage_STACKER_CONV_4, nArrVirtualCTray, nHandAvailable, DEF_PLACE, &nPockX, &nPockY, nArrPickerAssign, CTaskAutoTrayUnloadPP::eRobotWorkDirTopLeft);
		if (nRetCode > 0) {
			CTrayUnloadPPCmd_MovePickPlace* pCmd = new CTrayUnloadPPCmd_MovePickPlace;
			pCmd->m_Cmd.eStage = LOC_IDX_TRAY_PP_CONV_4;

			for (int nPickY = 0; nPickY < MAX_TRAY_PP_CNT_Y; nPickY++)
			{
				for (int nPickX = 0; nPickX < (MAX_TRAY_PP_CNT_X); nPickX++)
				{
					char setting = m_pTaskTrayPP->m_TrayPP_picker_setting[nPickY][nPickX];
					if (setting == 'U' && nArrPickerAssign[nPickY][nPickX] != 0) {
						pCmd->m_Cmd.vPicker.push_back(CPoint(nPickX, nPickY));
					}
				}
			}

			pCmd->m_Cmd.Pocket.SetPoint(nPockX, nPockY);
			pCmd->m_Cmd.ePickPlace = ePlace;

			// find same command
			BOOL bSameCmd = FALSE;

			std::find_if(m_listTrayPpSchedule.begin(), m_listTrayPpSchedule.end()
				, [&](CBaseSeqCmd* _pTrayPpCmd) -> bool
			{
				CTrayUnloadPPCmd_MovePickPlace* pTrayPpCmd = (CTrayUnloadPPCmd_MovePickPlace*)_pTrayPpCmd;
				if (pTrayPpCmd->m_Cmd.eStage == LOC_IDX_TRAY_PP_CONV_4 && pTrayPpCmd->m_Cmd.Pocket == pCmd->m_Cmd.Pocket) {
					bSameCmd = TRUE;
					return true;
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

	//새롭게 등록된 스케줄 횟수 리턴
	return m_listTrayPpSchedule.size() - nSchdCount;
}

/*=============================================================================
DESCRIPT : Unload Picker의 Device들은 Site Group단위로 옮겨다니므로
그 중에 한 개의 Device에서 이미 테스트가 완료된 Site Group을 찾아서 반환한다
KEY WORD :
ARGUMENT : vExceptSiteGroupNo = 테스트가 완료된 Site Group의 Index
RETURN   :
==============================================================================*/
void CTaskAutoTrayUnloadPP::GetExceptSiteGroup_FromUnloadPicker(std::vector<int> *vExceptSiteGroupNo, int nTblNo)
{
	vector<int> vExceptIndex;
	vExceptIndex.clear();

	BOOL bFound = FALSE;
	ST_DD_DEVICE stDevice;
	for (int y = 0; y < MAX_TRAY_PP_CNT_Y; y++)
	{
		for (int x = 0; x < MAX_TRAY_PP_CNT_X; x++)
		{
			char setting = m_pTaskTrayPP->m_TrayPP_picker_setting[y][x];

			// Unload Picker니까..
			if (setting == 'U')
			{
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TRAY_PP_1 + m_eTrayIdx, x, y, &stDevice);

				if (stDevice.sExist == eDvcExist)
				{
					bFound = TRUE;
					goto ESCAPE_LOOP;
				}
			}
		}
	}

ESCAPE_LOOP:

	if (bFound == FALSE)
	{
		*vExceptSiteGroupNo = vExceptIndex;
		return;
	}

	// TableIndex와 SiteGroup Index는 같다
	int nTempIndex = -1, nTableIndex = 0;
	int nPressUnitIdx = 0;
	for (int i = 0; i < stDevice.nTotalStationCnt; i++)
	{
		// Count GRR Testend device
		if (stDevice.stGrrTestInfo[i].chkTestEnd() == TRUE && g_TaskPressUnit[i].GetSocketOnCntGrr() > 0)
		{
			nPressUnitIdx = atoi(stDevice.stGrrTestInfo[i].szTestSiteNum);
			nTableIndex = ConvertTableIndex(_ePressUnit(nPressUnitIdx));

			if (nTempIndex != nTblNo)
			{
				vExceptIndex.push_back(nTableIndex);
				nTempIndex = nTableIndex;
			}
		}
	}

	*vExceptSiteGroupNo = vExceptIndex;
}

//==============================================================================
// convert device.bin_result -> setplate loc
// retrun : eDeviceDataStage
//==============================================================================
int CTaskAutoTrayUnloadPP::convert_sw_bin_to_hw_bin_multi(CHw_Bin_Sort_Item* pArrHwBins, int nArrSize, const char *pszBinResult)
{
	CHw_Bin_Sort_Item *arrSetplates_Loc = new CHw_Bin_Sort_Item[10];
	int nSelectCount = 0;
	int nConformCount = 0;

	//sw bin이 동일한 setplate 목록을 추려본다.
	for (int nIdx = 0; nIdx < g_BinCategory.nIndexCount; nIdx++)
	{
		ST_CATE_SUB_INFO *pNow = &g_BinCategory.aCateInfo[nIdx];
		int cmp = strcmp(pszBinResult, pNow->szSWBin);
		if (cmp == 0) {
			int nHwBinCode = atoi(pNow->szHWBin);
			eDeviceDataStage eSetplateNo;
			switch (nHwBinCode)
			{
			case 1: { eSetplateNo = eDeviceStage_STACKER_CONV_4; } break;
			case 2: { eSetplateNo = eDeviceStage_STACKER_CONV_5; } break;
			case 3: { eSetplateNo = eDeviceStage_STACKER_CONV_6; } break;
			case 4: { eSetplateNo = eDeviceStage_STACKER_CONV_7; } break;
			default: ASSERT(0); break;
			}
			arrSetplates_Loc[nSelectCount].eHwBin = eSetplateNo;
			strcpy_s(arrSetplates_Loc[nSelectCount].szSwBin, sizeof(arrSetplates_Loc[nSelectCount].szSwBin), pszBinResult);
			arrSetplates_Loc[nSelectCount].dLoadingTime = g_DMCont.m_dmConveyor.GD(DDM6_TRAY_LOAD_TIME3 + (nHwBinCode - 1));
			nSelectCount++;
		}
	}

	//filtering available setplate
	for (int i = 0; i < nSelectCount; i++) {
		//준비가 안된 setplate는 skip
		int nSetplateObj_Index = (/*_eSetplateIdx::*/eSTACKER_01 + (arrSetplates_Loc[i].eHwBin - eDeviceStage_STACKER_CONV_1));
		eStacker_Conveyor_State state = g_TaskAutoStacker[nSetplateObj_Index].GetConveyorState();
		if (state == PickEnable_Device || state == PlaceEnable_Device) {
			pArrHwBins[nConformCount++] = arrSetplates_Loc[i];
		}
		else {
			continue;
		}
	}

	delete[] arrSetplates_Loc;

	return nConformCount;
}

/*=============================================================================
AUTHOR   : 김선진
NAME     :
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
int CTaskAutoTrayUnloadPP::convert_ErrDisrip_to_hw_bin_multi(CHw_Bin_Sort_Item* pArrHwBins, int nArrSize, const char *pszErrStr)
{
	CHw_Bin_Sort_Item *arrSetplates_Loc = new CHw_Bin_Sort_Item[10];
	int nSelectCount = 0;
	int nConformCount = 0;

	//sw bin이 동일한 setplate 목록을 추려본다.
	for (int nIdx = 0; nIdx < g_BinCategory.nIndexCount; nIdx++)
	{
		ST_CATE_SUB_INFO *pNow = &g_BinCategory.aCateInfo[nIdx];
		int cmp = strcmp(pszErrStr, pNow->szDesc);
		if (cmp == 0) {
			int nHwBinCode = atoi(pNow->szHWBin);
			eDeviceDataStage eSetplateNo;
			switch (nHwBinCode)
			{
				/*case 1: { eSetplateNo = / *eDeviceDataStage::* /eDeviceStage_STACKER_CONV_4; } break;*/
			case 1: { eSetplateNo = /*eDeviceDataStage::*/eDeviceStage_STACKER_CONV_5; } break;
			case 2: { eSetplateNo = /*eDeviceDataStage::*/eDeviceStage_STACKER_CONV_6; } break;
			case 3: { eSetplateNo = /*eDeviceDataStage::*/eDeviceStage_STACKER_CONV_7; } break;
			default: ASSERT(0); break;
			}
			arrSetplates_Loc[nSelectCount].eHwBin = eSetplateNo;
			strcpy_s(arrSetplates_Loc[nSelectCount].szErrStr, sizeof(arrSetplates_Loc[nSelectCount].szErrStr), pszErrStr);
			arrSetplates_Loc[nSelectCount].dLoadingTime = g_DMCont.m_dmConveyor.GD(DDM6_TRAY_LOAD_TIME3 + (nHwBinCode - 1));
			nSelectCount++;
		}
	}

	//filtering available setplate
	for (int i = 0; i < nSelectCount; i++) {
		//준비가 안된 setplate는 skip
		int nSetplateObj_Index = (/*_eSetplateIdx::*/eSTACKER_01 + (arrSetplates_Loc[i].eHwBin - eDeviceStage_STACKER_CONV_1));
		auto conv_state = g_TaskAutoStacker[nSetplateObj_Index].GetConveyorState();
		if (conv_state != PlaceEnable_Device) {
			continue;
		}
		else {
			pArrHwBins[nConformCount++] = arrSetplates_Loc[i];
		}
	}

	delete[] arrSetplates_Loc;

	return nConformCount;
}

/*=============================================================================
	DESCRIPT : CTrayPP picker 상의 정보를 2차원 배열에 채워준다.
	KEY WORD :
	ARGUMENT : pnBuffer : 버퍼
	           nBufSizeX : 버퍼의 1차 사이즈 (X)
	           nBufSizeY : 버퍼의 2차 사이즈 (Y)
	           pnPickerCount_X : TrayPP Picker X 열수 리턴
	           pnPickerCount_Y : TrayPP Picker Y 열수 리턴

	RETURN   : 0 : NO Available picker
==============================================================================*/
int CTaskAutoTrayUnloadPP::GetDeviceExist_CTrayHand(BOOL bPickPlace, eDeviceDataStage WorkBin, int* pnBuffer, int nBufSizeX, int nBufSizeY)
{
	int nRet_ExistCount = 0;
	int nExist = 0;

	ASSERT(nBufSizeX >= MAX_TRAY_PP_CNT_X); //버퍼 사이즈가 picker x 수량보다 많아야..
	ASSERT(nBufSizeY >= MAX_TRAY_PP_CNT_Y); //버퍼 사이즈가 picker y 수량보다 많아야..
	ASSERT((bPickPlace == DEF_PICK || bPickPlace == DEF_PLACE) == TRUE); //pick or place 여야만 함.

	ST_DD_DEVICE stDvcData;
	BOOL bOnebyOnePlace = FALSE;

	int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
	if (bPickPlace == DEF_PICK)
	{
		for (int Y = 0; Y < MAX_TRAY_PP_CNT_Y; Y++)
		{
			for (int X = 0; X < MAX_TRAY_PP_CNT_X; X++)
			{
				char setting = m_pTaskTrayPP->m_TrayPP_picker_setting[Y][X];
				/*nExist = */g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TRAY_PP_1 + m_eTrayIdx, X, Y, &stDvcData);
				if ( /*setting == 'L'*/setting == 'L') {
					pnBuffer[X + (Y*nBufSizeX)] = stDvcData.sExist;
					nRet_ExistCount += (stDvcData.sExist == DEF_NOT_EXIST) ? 1 : 0;
				}
				if (setting == 'U') { //pick일땐 unload용 picker 사용 못함 (디바이스가 있다고 치자)
					pnBuffer[X + (Y*nBufSizeX)] = DEF_EXIST;
				}
			}
		}
	}
	else if (bPickPlace == DEF_PLACE) {	
		for (int nY = 0; nY < MAX_TRAY_PP_CNT_Y && bOnebyOnePlace == FALSE; nY++)
		{
			for (int nX = 0; nX < MAX_TRAY_PP_CNT_X && bOnebyOnePlace == FALSE; nX++) {
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TRAY_PP_1 + m_eTrayIdx, nX, nY, /*pDev*/&stDvcData);

				auto setting = m_pTaskTrayPP->m_TrayPP_picker_setting[nY][nX];
				if (setting == 'L') {
					//place일땐 load용 picker 사용 못함 (디바이스가 없다고 치자)
					pnBuffer[nX + (nY*nBufSizeX)] = DEF_NOT_EXIST;
					continue;
				}

				int MAX_UNLOAD_SETPLATE = g_unload_stackers.size();
				CHw_Bin_Sort_Item *pArrTempHwBins = new CHw_Bin_Sort_Item[MAX_UNLOAD_SETPLATE];
				BOOL bBinCmp = FALSE;

				//디바이스가 있고, 디바이스 BIN결과가 동일한 것만 있다고 한다.
				//nExist = g_DMCont.m_dmDevice.GetDeviceExist( eDeviceStage_TRAY_PP, nX, nY );

				if (stDvcData.sExist) {
					// revice to cord
					int nRetHwBinCnt = 0;
					int nSortType = g_DMCont.m_dmEQP.GN(NDM0_SortType);// enum eSortingType{   
					if (nSortType == eSortType_SwBin) {
						nRetHwBinCnt = convert_sw_bin_to_hw_bin_multi(pArrTempHwBins, MAX_UNLOAD_SETPLATE,/*pDev->*/stDvcData.szBinResult);
					}
					else if (nSortType == eSortType_ErrStr) {
						nRetHwBinCnt = convert_ErrDisrip_to_hw_bin_multi(pArrTempHwBins, MAX_UNLOAD_SETPLATE,/*pDev->*/stDvcData.szErrString);
					}

					//priority : 1 step HwBin Decending
					for (int nRef_I = 0; nRef_I < nRetHwBinCnt; nRef_I++) {
						for (int nCmp_I = 1; nCmp_I < nRetHwBinCnt; nCmp_I++) {
							if (pArrTempHwBins[nRef_I].eHwBin < pArrTempHwBins[nCmp_I].eHwBin) {
								CHw_Bin_Sort_Item Temp = pArrTempHwBins[nRef_I];
								pArrTempHwBins[nRef_I] = pArrTempHwBins[nCmp_I];
								pArrTempHwBins[nCmp_I] = Temp;
							}
						}
					}

					if (nSortType == eSortType_SwBin) {
						//priority : 2 step LoadingTime Accending (where same SW Bin)
						for (int nRef_I = 0; nRef_I < nRetHwBinCnt; nRef_I++) {
							for (int nCmp_I = 1; nCmp_I < nRetHwBinCnt; nCmp_I++) {
								//same SW bin
								if (strcmp(pArrTempHwBins[nRef_I].szSwBin, pArrTempHwBins[nCmp_I].szSwBin) == 0) {
									if (pArrTempHwBins[nRef_I].dLoadingTime > pArrTempHwBins[nCmp_I].dLoadingTime) {
										CHw_Bin_Sort_Item Temp = pArrTempHwBins[nRef_I];
										pArrTempHwBins[nRef_I] = pArrTempHwBins[nCmp_I];
										pArrTempHwBins[nCmp_I] = Temp;
									}
								}
							}
						}
					}
					else if (nSortType == eSortType_ErrStr) {
						//priority : 2 step LoadingTime Accending (where same ErrStr)
						for (int nRef_I = 0; nRef_I < nRetHwBinCnt; nRef_I++) {
							for (int nCmp_I = 1; nCmp_I < nRetHwBinCnt; nCmp_I++) {
								//same ErrStr
								if (strcmp(pArrTempHwBins[nRef_I].szErrStr, pArrTempHwBins[nCmp_I].szErrStr) == 0) {
									if (pArrTempHwBins[nRef_I].dLoadingTime > pArrTempHwBins[nCmp_I].dLoadingTime) {
										CHw_Bin_Sort_Item Temp = pArrTempHwBins[nRef_I];
										pArrTempHwBins[nRef_I] = pArrTempHwBins[nCmp_I];
										pArrTempHwBins[nCmp_I] = Temp;
									}
								}
							}
						}
					}

					BOOL bAutoQAUse = g_TaskSystemCtrl.GetAutoQAUse();
					for (int i = 0; i < nRetHwBinCnt; i++) {
						if (pArrTempHwBins[i].eHwBin == WorkBin) {
							bBinCmp = TRUE;
							if ((g_TaskSystemCtrl.IsOneCycleModeOn() == TRUE || g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == true) &&
								 nDeviceMode == eDeviceMode &&
								bAutoQAUse == FALSE)
							{
								bOnebyOnePlace = TRUE;
							}							
							break;
						}
						else {
							bBinCmp = FALSE;
						}
					}
				}
				if (stDvcData.sExist && bBinCmp) {
					pnBuffer[nX + (nY*nBufSizeX)] = DEF_EXIST;
					nRet_ExistCount++;
				}
				else {
					pnBuffer[nX + (nY*nBufSizeX)] = DEF_NOT_EXIST;
				}

				//delete[] pArrTempHwBins;
				SAFE_DELETE_ARRAY(pArrTempHwBins);
			}//////////////////////////////
		}
		//SAFE_DELETE(pDev);
	}

	return nRet_ExistCount;
}

int CTaskAutoTrayUnloadPP::FillDeviceData_VirtualCTray(int nStageLoc, int* pnBuffer, int nBufSizeX, int nBufSizeY, int nVtl0X, int nVtl0Y)
{
	int nExist = 0;
	int nPocketCount_X = 0;
	int nPocketCount_Y = 0;

	switch (nStageLoc)
	{
		//case eDeviceStage_STACKER_CONV_2:
	case eDeviceStage_STACKER_CONV_3:
	case eDeviceStage_STACKER_CONV_4:
	case eDeviceStage_STACKER_CONV_5:
	case eDeviceStage_STACKER_CONV_6:
	case eDeviceStage_STACKER_CONV_7:
	{
		nPocketCount_X = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
		nPocketCount_Y = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);
	}break;
	case eDeviceStage_TRAY_BUFF:
	{
		nPocketCount_X = 4;
		nPocketCount_Y = 4;
	}break;
	default:ASSERT(false); break;
	}

	ASSERT(nBufSizeX >= nPocketCount_X); //버퍼 사이즈가 picker x 수량보다 많아야..
	ASSERT(nBufSizeY >= nPocketCount_Y); //버퍼 사이즈가 picker y 수량보다 많아야..

	for (int nY = 0; nY < nPocketCount_Y; nY++)
	{
		for (int nX = 0; nX < nPocketCount_X; nX++)
		{
			int nBuffer_Index = (nX + nVtl0X)/*X*/ + ((nY + nVtl0Y)*nBufSizeX)/*Y*/;
			nExist = g_DMCont.m_dmDevice.GetDeviceExist(nStageLoc, nX, nY);
			pnBuffer[nBuffer_Index] = nExist;
		}
	}

	return ERR_NO_ERROR;
}

int CTaskAutoTrayUnloadPP::FindSortToSetplate( int nWorkPlace,
	                                           int nArrDvcData_VirtualCTray[ VIRTUAL_POCKET_Y_MAX ][ VIRTUAL_POCKET_X_MAX ],
	                                           int nPickerDvcPattern[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ],
	                                           BOOL bPickPlace,
	                                           int *n_Xpos, int *n_YPos,
	                                           int nArrHandWork[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ],
	                                           int nWorkDir )
{
	int nSearchY = 0, nSearchX = 0;
	int nSearchHandY = 0, nSearchHandX = 0;

	/*DEF_WORK_FREE or DEF_WORK_RESERVED*/
	int nArrHandWorkSource[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ] = { DEF_WORK_FREE, };
	int nArrHandWorkCopy[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ] = { DEF_WORK_FREE, };

	int nPocketCntY = 0;
	int nPocketCntX = 0;

	int nTotalPossibleCnt = 0;	// 가장 Pick 할 수 있는 수량
	int nCurrentCnt = 0;	// 현재 까지 Pick 할수 있는 수량
	int nCXPosition = 0;
	int nCYPosition = 0;
	// 1Y, 2Y는 계산 한 대로 가져 온다.

	int nCheckCTrayData = DEF_NOT_EXIST;
	int nHadData = DEF_NOT_EXIST;

	int nContainerPitch_X_Cnt = 0;	// pocket pitch와 
	int nContainerPitch_Y_Cnt = 0;

	if( bPickPlace == DEF_PICK ) {	// Pick 일 때 조건
		nCheckCTrayData = DEF_EXIST;
		nHadData = DEF_NOT_EXIST;
	}
	else {							// Place 일 때 조건		
		nCheckCTrayData = DEF_NOT_EXIST;
		nHadData = DEF_EXIST;
	}

	// [2x-pitch]
	int nX_PitchType = 1;
	int nY_PitchType = 1;

	switch( nWorkPlace )
	{
		//case eDeviceStage_STACKER_CONV_2:
		case eDeviceStage_STACKER_CONV_3:
		case eDeviceStage_STACKER_CONV_4:
		case eDeviceStage_STACKER_CONV_5:
		case eDeviceStage_STACKER_CONV_6:
		case eDeviceStage_STACKER_CONV_7:
			{
				if( g_DMCont.m_dmTrayPP.GN( NDM5_Tray_Pitch_X ) < DEF_TRAY_PP_MAX_X_PITCH_NARROW ) {
					nX_PitchType = 2;
				}
				if( g_DMCont.m_dmTrayPP.GN( NDM5_Tray_Pitch_Y ) < DEF_TRAY_PP_MAX_Y_PITCH_NARROW ) {
					nY_PitchType = 2;
				}
				nPocketCntX = g_DMCont.m_dmTrayPP.GN( NDM5_TrayDiv_X );
				nPocketCntY = g_DMCont.m_dmTrayPP.GN( NDM5_TrayDiv_Y );

				nContainerPitch_X_Cnt = nX_PitchType; // x pitch
				nContainerPitch_Y_Cnt = nY_PitchType; // y pitch
			}break;
		default: ASSERT( false ); break;
	}

	// CTray에서 첫 디바이스가 발견되는 Y+1 줄까지만 검색 하도록!
	int nSearchY_Limit = ( VIRTUAL_POCKET_Y_MAX - 1 );
	BOOL bFindDevice = FALSE;
	for( nSearchY = VIRTUAL_POCKET_Y_START; nSearchY < ( VIRTUAL_POCKET_Y_START + nPocketCntY ); nSearchY++ )
	{
		for( nSearchX = VIRTUAL_POCKET_X_START; nSearchX < ( VIRTUAL_POCKET_X_START + nPocketCntX ); nSearchX++ )
		{
			if( nArrDvcData_VirtualCTray[ nSearchY ][ nSearchX ] == nCheckCTrayData ) {
				nSearchY_Limit = nSearchY + ( nContainerPitch_Y_Cnt - 1 );
				bFindDevice = TRUE;
				break;
			}
		}
		if( bFindDevice == TRUE ) { break; }
	}


	if( nWorkDir == CTaskAutoTrayUnloadPP::eRobotWorkDirTopLeft )
	{
		for( nSearchY = VIRTUAL_POCKET_Y_START - ( MAX_TRAY_PP_CNT_Y - 1 ) * nContainerPitch_Y_Cnt; nSearchY < VIRTUAL_POCKET_Y_START + nPocketCntY; nSearchY++ )
		{
			for( nSearchX = VIRTUAL_POCKET_X_START - ( MAX_TRAY_PP_CNT_X - 1 ) * nContainerPitch_X_Cnt; nSearchX < VIRTUAL_POCKET_X_START + nPocketCntX; nSearchX++ )
			{
				int nOffSetPos = (g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == false ? 2 : 0);
				if( nSearchY > nSearchY_Limit+ nOffSetPos) { break; } // CTray에서 첫 디바이스가 발견되는 Y+1 줄까지만 검색 하도록!

				int nYPocket = 0, nXPocket = 0;
				nCurrentCnt = 0;
				memset( nArrHandWorkSource, 0, sizeof( nArrHandWorkSource ) );

				for( nSearchHandX = 0; nSearchHandX < MAX_TRAY_PP_CNT_X; nSearchHandX++ )
				{
					for( nSearchHandY = 0; nSearchHandY < MAX_TRAY_PP_CNT_Y; nSearchHandY++ )
					{
						nYPocket = nSearchY + nSearchHandY * nContainerPitch_Y_Cnt;
						nXPocket = nSearchX + nSearchHandX * nContainerPitch_X_Cnt;

						if( nArrDvcData_VirtualCTray[ nYPocket ][ nXPocket ] == nCheckCTrayData
							&&  nPickerDvcPattern[ nSearchHandY ][ nSearchHandX ] == nHadData )
						{
							nArrHandWorkSource[ nSearchHandY ][ nSearchHandX ] = DEF_WORK_RESERVED;
							nCurrentCnt++;
						}
					}
				}

				BOOL bLimitCheckOK = GetAreaProblem( nSearchX - VIRTUAL_POCKET_X_START, nSearchY - VIRTUAL_POCKET_Y_START, ( eDeviceDataStage )nWorkPlace, nArrHandWorkSource ); // todo slt : 영역 점검을 위한 함수 구현 필요.
				if( bLimitCheckOK == TRUE )
				{
					if( nCurrentCnt > nTotalPossibleCnt )
					{
						//작업해라.
						nTotalPossibleCnt = nCurrentCnt;
						memcpy_s( nArrHandWorkCopy, sizeof( nArrHandWorkCopy ), nArrHandWorkSource, sizeof( nArrHandWorkCopy ) );
						nCXPosition = nSearchX;
						nCYPosition = nSearchY;

					}
				}
			}
		}
	}
	else
	{   //지우지 마세요~
		/*for(nSearchY = VIRTUAL_POCKET_Y_START + (nPocketCntY - 1); nSearchY >= VIRTUAL_POCKET_Y_START - ( MAX_TRAY_PP_CNT_Y - 1 ) * nContainerPitch_Y_Cnt; nSearchY--)
		{
		for(nSearchX = VIRTUAL_POCKET_X_START - ( MAX_TRAY_PP_CNT_X - 1 ) * nContainerPitch_X_Cnt; nSearchX < VIRTUAL_POCKET_X_START + nPocketCntX; nSearchX++)
		{
		nCurrentCnt = 0;

		memset(nArrHandWorkSource, 0, sizeof(nArrHandWorkSource));

		for(nSearchHandY = MAX_TRAY_PP_CNT - 1; nSearchHandY >= 0; nSearchHandY--)
		{
		for(nSearchHandX = 0; nSearchHandX < MAX_TRAY_PP_CNT_X; nSearchHandX++)
		{
		int nYPocket = 0, nXPocket = 0;
		nYPocket = nSearchY + nSearchHandY * nContainerPitch_Y_Cnt;
		nXPocket = nSearchX + nSearchHandX * nContainerPitch_X_Cnt;

		if(     nArrDvcData_VirtualCTray[ nYPocket ][ nXPocket ]	==	nCheckCTrayData
		&&  nPickerDvcPattern[nSearchHandY][nSearchHandX]		==  nHadData)
		{
		nArrHandWorkSource[nSearchHandY][nSearchHandX] = DEF_WORK_RESERVED;
		nCurrentCnt++;
		}
		}
		}

		int   nLimitCheckOK = 0;
		nLimitCheckOK = GetAreaProblem(dLimitCheckValue_X, dLimitCheckValue_Y); // todo slt : 영역 점검을 위한 함수 구현 필요.

		if(nLimitCheckOK == 1)
		{
		if( nCurrentCnt > nTotalPossibleCnt )
		{
		//작업해라.
		nTotalPossibleCnt = nCurrentCnt;
		memcpy_s(nArrHandWorkCopy, sizeof(nArrHandWorkCopy), nArrHandWorkSource, sizeof(nArrHandWorkCopy));
		nCXPosition = nSearchX;
		nCYPosition = nSearchY;
		}
		}
		}
		}*/
	}

	memcpy_s( nArrHandWork, sizeof( nArrHandWorkCopy ), nArrHandWorkCopy, sizeof( nArrHandWorkCopy ) );

	//0번 피커부터 할당되지 않은 포켓은 트림한다.
	int nPockOffsetX = 0;
	int nPockOffsetY = 1;

	//X방향 첫 할당 위치가 X옵셋
	for( ; nPockOffsetX < MAX_TRAY_PP_CNT_X; nPockOffsetX++ )
	{
		if( nArrHandWork[ 0 ][ nPockOffsetX ] != DEF_WORK_FREE ) break;
		if( nArrHandWork[ 1 ][ nPockOffsetX ] != DEF_WORK_FREE ) break;
	}
	//X방향 첫 할당 위치가 1개라도 있으면 Y옵셋은 0
	for( int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++ )
	{
		if( nArrHandWork[ 0 ][ nX ] != DEF_WORK_FREE ) {
			nPockOffsetY = 0;
			break;
		}
	}

	/*
	*	스케쥴을 생성한 디바이스는 가상 버퍼에서 지우고 다음 pick 스케줄 생성을 시도한다.
	*/

	for( int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++ )
	{
		for( int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++ )
		{
			if( nArrHandWork[ nY ][ nX ] != 0 ) {
				int nVirX = nCXPosition + nX*nContainerPitch_X_Cnt;
				int nVirY = nCYPosition + nY*nContainerPitch_Y_Cnt;

				if( bPickPlace == DEF_PICK ) {
					nArrDvcData_VirtualCTray[ nVirY ][ nVirX ] = DEF_NOT_EXIST;
					nPickerDvcPattern[ nY ][ nX ] = DEF_EXIST;
				}
				else {
					nArrDvcData_VirtualCTray[ nVirY ][ nVirX ] = DEF_EXIST;
					nPickerDvcPattern[ nY ][ nX ] = DEF_NOT_EXIST;
				}
			}
		}
	}

	nCXPosition -= VIRTUAL_POCKET_X_START;
	nCYPosition -= VIRTUAL_POCKET_Y_START;

	*n_Xpos = nCXPosition + nPockOffsetX*nContainerPitch_X_Cnt;
	*n_YPos = nCYPosition + nPockOffsetY*nContainerPitch_Y_Cnt;

	return nTotalPossibleCnt;
}

/*=============================================================================
	DESCRIPT : Device의 배열(Setplate, LoadTable)에서 Picking 정보를 찾는다. 기준이 되는 Pocket위치에서
	           nArrPickerWork의 위치대로 Pick을 진행하면 된다.
	KEY WORD :
	ARGUMENT : bPickPlace               : PICK인지 PLACE인지에 따라 pnArrDeviceData에서 구분해서 찾는 용도(PICK=EXIST, PLACE=NOT EXIST)
	           nPocketCntX, nPocketCntY : pnArrDeviceData의 배열 x, y 개수
	           pnArrDeviceData          : Setplate의 Device 배열 정보(1 or 0)
	           nWorkablePicker          : 사용할 수 있는 Picker의 배열정보(1 or 0)
	           ptPocket                 : 기준이 되는 Pocket 위치(시작 Pocket x,y)
	           nArrPickerWork           : 찾아낸 Picker의 작업에 대한 배열
	           bUnloadPickerToTable     : Auto GRR은 특수하게 Unload picker에서 Table로 옮기는 경우가 있다.(Table -> Table)
	           nTableIndex              : Table -> Table Place 동작일 경우에 Place할 Table의 Index
	                                      -2 = Setplate 에서 작업하는 경우에 해당한다 ( 2x pitch 구분용으로 사용함 )
	RETURN   : 찾아낸 Picker의 작업예약 개수
==============================================================================*/
int CTaskAutoTrayUnloadPP::FindPickerWork( BOOL bPickPlace,
	                                       int nPocketCntX, int nPocketCntY, int** pnArrDeviceData,
	                                       int nWorkablePicker[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X],
	                                       POINT& ptPocket,
	                                       int nArrPickerWork[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X],
	                                       BOOL bUnloadPickerToTable/* = FALSE*/,
	                                       int nTableIndex/* = -1*/)
{
	int nRetReserveCnt = 0;
	int nOrgX = 0, nOrgY = 0;
	int nTotalCnt = 0;
	int nIdx = 0;
	ST_DD_DEVICE stDeviceInfo;
	ST_PICKER_INFO stPickerInfo[MAX_TRAY_PP_CNT_X*MAX_TRAY_PP_CNT_Y];
	bool bChkFirst = false;

	// [2x-pitch]
	int nX_PitchType = 1;
	int nY_PitchType = 1;


	// Target position is setplate!
	if (bUnloadPickerToTable == FALSE && nTableIndex == -2) {
		nX_PitchType = m_pTaskTrayPP->GetHandPitchType(CTaskTrayBasePP::eAxisPitchX, LOC_IDX_TRAY_PP_CONV_2);
		nY_PitchType = m_pTaskTrayPP->GetHandPitchType(CTaskTrayBasePP::eAxisPitchY, LOC_IDX_TRAY_PP_CONV_2);
	}


	int nNextSiteNo = 0, nNextTableIdx = 0;
	for (int j = 0; j < MAX_TRAY_PP_CNT_Y; j++)
	{
		for (int i = 0; i < MAX_TRAY_PP_CNT_X; i++)
		{
			if (nWorkablePicker[j][i] == DEF_WORKABLE)
			{
				// Special case
				if (bUnloadPickerToTable == TRUE)
				{
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TRAY_PP_1 + m_eTrayIdx, i, j, &stDeviceInfo);
					if (stDeviceInfo.chkAutoGrrCompelte() == TRUE || g_TaskSystemCtrl.GetAutoGrrCancel() == TRUE) continue;
					nNextSiteNo = stDeviceInfo.findNextTestSite();
					nNextTableIdx = ConvertTableIndex(_ePressUnit(nNextSiteNo));

					if (nNextTableIdx == nTableIndex)
					{
						if (bChkFirst == false)
						{
							nOrgX = i;
							nOrgY = j;

							bChkFirst = true;
						}

						stPickerInfo[nIdx].nRow = j;
						stPickerInfo[nIdx].nCol = i;
						stPickerInfo[nIdx].nPlusX = i - nOrgX;
						stPickerInfo[nIdx].nPlusY = j - nOrgY;
						nIdx++;
					}
				}
				// Normal case
				else
				{
					if (bChkFirst == false)
					{
						nOrgX = i;
						nOrgY = j;

						bChkFirst = true;
					}

					stPickerInfo[nIdx].nRow = j;
					stPickerInfo[nIdx].nCol = i;
					stPickerInfo[nIdx].nPlusX = i - nOrgX;
					stPickerInfo[nIdx].nPlusY = j - nOrgY;
					nIdx++;
				}
			}
		}
	}
	nTotalCnt = nIdx;

	bChkFirst = false;
	int nFirstX = 0, nFirstY = 0;
	BOOL bChkDevice = DEF_NOT_EXIST;
	// Pick이면 Device가 존재하는 것을, Place면 Device가 존재하지 않는 것을 확인해야함
	if (bPickPlace == DEF_PICK)
		bChkDevice = DEF_EXIST;
	else
		bChkDevice = DEF_NOT_EXIST;

	// 첫번째 기준이 될 위치를 찾는다.
	for (int j = 0; j < nPocketCntY; j++)
	{
		if (bChkFirst == true)
			break;

		for (int i = 0; i < nPocketCntX; i++)
		{
			if (pnArrDeviceData[j][i] == bChkDevice)
			{
				if (bChkFirst == false)
				{
					nFirstY = j;
					nFirstX = i;

					bChkFirst = true;
					break;
				}
			}

		}
	}

	// 기준위치에서 한번에 pick할 수 있는 것을 작업예약한다.
	int nX = 0, nY = 0;
	for (int i = 0; i < nTotalCnt; i++)
	{
		nX = nFirstX + (stPickerInfo[i].nPlusX*nX_PitchType);
		nY = nFirstY + (stPickerInfo[i].nPlusY*nY_PitchType);

		if (nX < 0 || nX >= nPocketCntX) continue;
		if (nY < 0 || nY >= nPocketCntY) continue;

		if (pnArrDeviceData[nY][nX] == bChkDevice)
		{
			nArrPickerWork[stPickerInfo[i].nRow][stPickerInfo[i].nCol] = DEF_WORK_RESERVED;
			nRetReserveCnt++;

			// Picker의 작업가능 정보를 변경
			nWorkablePicker[stPickerInfo[i].nRow][stPickerInfo[i].nCol] = DEF_UNWORKABLE;

			// bChkDevice가 DEF_EXIST면 NOT_EXIST, NOT_EXIST면 DEF_EXIST로 반대로 바꿔준다.
			pnArrDeviceData[nY][nX] = 1 - bChkDevice;
		}
	}

	ptPocket.x = nFirstX;
	ptPocket.y = nFirstY;

	return nRetReserveCnt;
}

int CTaskAutoTrayUnloadPP::GetWorkable_CTrayHand_forLoadTable(BOOL bPickPlace, int nBuffer[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X], _eUsePicker ePickerArea)
{
	int nWorkableCnt = 0;
	int nExist = 0;
	int nPickerCount_X = MAX_TRAY_PP_CNT_X;
	int nPickerCount_Y = MAX_TRAY_PP_CNT_Y;

	ASSERT(bPickPlace == DEF_PICK || bPickPlace == DEF_PLACE);

	for (int nPkrY = 0; nPkrY < MAX_TRAY_PP_CNT_Y; nPkrY++)
	{
		for (int nPkrX = 0; nPkrX < MAX_TRAY_PP_CNT_X; nPkrX++)
		{
			char setting = m_pTaskTrayPP->m_TrayPP_picker_setting[nPkrY][nPkrX];
			int device_exist = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TRAY_PP_1 + m_eTrayIdx, nPkrX, nPkrY);

			switch (bPickPlace) {
			case DEF_PICK:
			{
				if (setting == 'U') {
					nBuffer[nPkrY][nPkrX] = DEF_WORKABLE;
					if (device_exist == DEF_NOT_EXIST) {
						nWorkableCnt++;
					}
				}
				else {
					// pick 진행 시 동작 하지 말아야 하는 picker 
					nBuffer[nPkrY][nPkrX] = DEF_UNWORKABLE;
				}
			}break;
			case DEF_PLACE:
			{
				if (setting == 'L') {
					nBuffer[nPkrY][nPkrX] = DEF_WORKABLE;
					if (device_exist == DEF_EXIST) {
						nWorkableCnt++;
					}
				}
				else {
					// place 진행 시 동작 하지 말아야 하는 picker 
					nBuffer[nPkrY][nPkrX] = DEF_UNWORKABLE;
				}
			}break;
			}
		}
	}



	//if( bPickPlace == DEF_PICK )
	//{
	//	for( int nY = 0; nY < nPickerCount_Y; nY++ )
	//	{
	//		for( int nX = 0; nX < nPickerCount_X; nX++ )
	//		{
	//			//LoadTable에서 pick할때는 unload용 picker 사용하므로
	//			// Load picker는 사용안함으로 체움
	//			if( nX < nPickerCount_X / 2 )
	//			{
	//				nBuffer[ nY ][ nX ] = DEF_UNWORKABLE;
	//				continue;
	//			}

	//			nExist = g_DMCont.m_dmDevice.GetDeviceExist( eDeviceStage_TRAY_PP, nX, nY );
	//			if( nExist )
	//			{
	//				nBuffer[ nY ][ nX ] = DEF_UNWORKABLE;
	//			}
	//			else {
	//				nBuffer[ nY ][ nX ] = DEF_WORKABLE;
	//				nWorkableCnt++;
	//			}
	//		}
	//	}
	//}
	//// DEF_PLACE
	//else
	//{
	//	for( int nY = 0; nY < nPickerCount_Y; nY++ )
	//	{
	//		for( int nX = 0; nX < nPickerCount_X; nX++ )
	//		{
	//			// Load Table에 Place를 AutoGRR에서는 Load/Unload picker 모두 사용하고 
	//			// Load Picker로 Place하는 경우		: LoadSetplate #1 -> Table #1
	//			// Unload Picker로 Place하는 경우	: Table -> Table
	//			if( ePickerArea == _eUsePicker_Load ) {
	//				if( nX >= nPickerCount_X / 2 )
	//				{
	//					nBuffer[ nY ][ nX ] = DEF_UNWORKABLE;
	//					continue;
	//				}
	//			}
	//			else
	//			{
	//				if( nX < nPickerCount_X / 2 )
	//				{
	//					nBuffer[ nY ][ nX ] = DEF_UNWORKABLE;
	//					continue;
	//				}
	//			}

	//			nExist = g_DMCont.m_dmDevice.GetDeviceExist( eDeviceStage_TRAY_PP, nX, nY );
	//			if( nExist )
	//			{
	//				nBuffer[ nY ][ nX ] = DEF_WORKABLE;
	//				nWorkableCnt++;
	//			}
	//			else {
	//				nBuffer[ nY ][ nX ] = DEF_UNWORKABLE;
	//			}
	//		}
	//	}
	//}

	return nWorkableCnt;
}


int	CTaskAutoTrayUnloadPP::_FindPickPlaceFromTable8Picker(int nLocIdx, int nTableDvc[VIRTUAL_POCKET_Y_MAX][VIRTUAL_POCKET_X_MAX]
	, int nPickerPattern[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X], BOOL bPickPlace, int * n_Xpos, int * n_YPos, int nWorkPkr[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X])
{
	ASSERT(LOC_IDX_TRAY_PP_TBL_1 <= nLocIdx && nLocIdx <= LOC_IDX_TRAY_PP_TBL_2);

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


	int nTotalPossibleCnt = 0;	// 가장 Pick 할 수 있는 수량
	int nCurrentCnt = 0;	// 현재 까지 Pick 할수 있는 수량
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

	for (nSearchY = VIRTUAL_POCKET_Y_START + (bPickPlace == DEF_PICK ? 0 : 2); nSearchY < VIRTUAL_POCKET_Y_START + (bPickPlace == DEF_PICK ? 1 : 3); nSearchY++)
	{
		for (nSearchX = VIRTUAL_POCKET_X_START /*- (MAX_TRAY_PP_CNT_X - 1) * nPitchOff_X*/; nSearchX < VIRTUAL_POCKET_X_START + 3 /*+ nPocketCntX*/; nSearchX += 2)
		{
			nCurrentCnt = 0;
			memset(nArrHandWorkSource, 0, sizeof(nArrHandWorkSource));

			for (nSearchHandY = 0; nSearchHandY < MAX_TRAY_PP_CNT_Y; nSearchHandY++)
			{
				int nSkipXCnt = (nSearchX % VIRTUAL_POCKET_X_START == 0 ? 0 : 1);
				for (nSearchHandX = 0; nSearchHandX < MAX_TRAY_PP_CNT_X; nSearchHandX++)
				{
					if (nSearchX % VIRTUAL_POCKET_X_START != 0 && nSearchHandX % 2 == 0) continue; // 짝수열은 1,3 홀수열은 2,4
					if (nSearchX % VIRTUAL_POCKET_X_START == 0 && nSearchHandX % 2 != 0) continue; // 짝수열은 1,3 홀수열은 2,4

					int nYPocket = nSearchY + nSearchHandY * nPitchOff_Y;
					int nXPocket = nSearchX + (nSearchHandX - nSkipXCnt)  * nPitchOff_X;// -nSkipXCnt*nShtOffX;

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

					nSkipXCnt++;
				}

			}

			BOOL bLimitCheckOK = GetAreaProblem(nSearchX - VIRTUAL_POCKET_X_START, nSearchY - VIRTUAL_POCKET_Y_START, (eDeviceDataStage)(eDeviceStage_LD_TBL_1 + nLocIdx - LOC_IDX_TRAY_PP_TBL_1), nArrHandWorkSource); // todo slt : 영역 점검을 위한 함수 구현 필요.
			if (bLimitCheckOK == TRUE)
			{
				if (nCurrentCnt > nTotalPossibleCnt)
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

	*n_Xpos = nCXPosition + (nPockOffsetX / nShtOffX)*nPitchOff_X;
	*n_YPos = nCYPosition + nPockOffsetY*nPitchOff_Y;

	return nTotalPossibleCnt;
}

int CTaskAutoTrayUnloadPP::FindSortToUnloadSetplateGrr(int nPocketCntX, int nPocketCntY, int nWorkablePicker[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X], POINT & ptPocket, int nArrPickerWork[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X])
{
	int nRetReserveCnt = 0;
	int nIdx = 0, nOrgX = 0, nOrgY = 0, nTotalCnt = 0;
	ST_DD_DEVICE stDevice;
	ST_PICKER_INFO stPickerInfo[MAX_TRAY_PP_CNT_X*MAX_TRAY_PP_CNT_Y];
	bool bChkFirst = false;

	for (int j = 0; j < MAX_TRAY_PP_CNT_Y; j++)
	{
		for (int i = 0; i < MAX_TRAY_PP_CNT_X; i++)
		{
			if (nWorkablePicker[j][i] == DEF_WORKABLE)
			{
				if (bChkFirst == false)
				{
					nOrgX = i;
					nOrgY = j;

					bChkFirst = true;
				}

				stPickerInfo[nIdx].nRow = j;
				stPickerInfo[nIdx].nCol = i;
				stPickerInfo[nIdx].nPlusX = i - nOrgX;
				stPickerInfo[nIdx].nPlusY = j - nOrgY;
				nIdx++;
			}
		}
	}
	nTotalCnt = nIdx;

	int nFirstPocketX = 0, nFirstPocketY = 0;
	// 기준이 되는 TrayPP Picker의 Device에 대한 원래 C-Tray에서의 위치를 가져옴
	g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TRAY_PP_1 + m_eTrayIdx, stPickerInfo[0].nCol, stPickerInfo[0].nRow, &stDevice);
	nFirstPocketX = stDevice.nCol;
	nFirstPocketY = stDevice.nRow;

	int nX = 0, nY = 0;
	for (int i = 0; i < nTotalCnt; i++)
	{
		nX = nFirstPocketX + stPickerInfo[i].nPlusX;
		nY = nFirstPocketY + stPickerInfo[i].nPlusY;

		if (nX < 0 || nX >= nPocketCntX) continue;
		if (nY < 0 || nY >= nPocketCntY) continue;

		memset(&stDevice, 0, sizeof(ST_DD_DEVICE));
		g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TRAY_PP_1 + m_eTrayIdx, stPickerInfo[i].nCol, stPickerInfo[i].nRow, &stDevice);

		// 원래 C-Tray에서의 위치와 동일한지 확인 (동일하다면 같이 작업할 수 있다.)
		// 해당 Device가 AutoGRR이 완료된 놈인지도 확인
		if (nX == stDevice.nCol && nY == stDevice.nRow && (stDevice.chkAutoGrrCompelte() == TRUE || g_TaskSystemCtrl.GetAutoGrrCancel() == TRUE))
		{
			nArrPickerWork[stPickerInfo[i].nRow][stPickerInfo[i].nCol] = DEF_WORK_RESERVED;
			nRetReserveCnt++;

			// Picker의 작업가능 정보를 변경
			nWorkablePicker[stPickerInfo[i].nRow][stPickerInfo[i].nCol] = DEF_UNWORKABLE;
		}
	}

	ptPocket.x = nFirstPocketX;
	ptPocket.y = nFirstPocketY;
	return nRetReserveCnt;
}

//==============================================================================
//loading table device map -> bit mask (array)
//==============================================================================
void CTaskAutoTrayUnloadPP::get_loadtable_mask_for_load(int* pArrLdTblMask, int nArrSize/*=MAX_LD_TBL_CNT*/)
{
	ASSERT(pArrLdTblMask);
	ASSERT(nArrSize > 0);

	eDeviceDataStage DvcLdTbl_Loc = eDeviceStage_LD_TBL_1; //device 자료구조의 location
	int nLdTbl_Index = 0; //g_TaskLoadTbl[] index

	for (; DvcLdTbl_Loc <= eDeviceStage_LD_TBL_2; DvcLdTbl_Loc = (eDeviceDataStage)(DvcLdTbl_Loc + 1), nLdTbl_Index++)
	{
		ASSERT((0 <= nLdTbl_Index) && (nLdTbl_Index < nArrSize));
		std::vector<int> vTchPosList;

		int nRetStatus = g_TaskLoadTbl[nLdTbl_Index].GetWorkReserve();	// Check Reserve Status
		if (RID_TRAY_PP_(RIDX_TRAY_PP_1) != nRetStatus) {
			continue;
		}

		//loading table이 load_side에 있을 경우에만 본다.
		g_TaskLoadTbl[nLdTbl_Index].WhereIsMotorPos(vTchPosList);

		ASSERT(vTchPosList.size() == 1);
		if ((vTchPosList[0] != CTaskLoadTable::eYPOS_LoadSide)) {
			continue;
		}

		//test site all socket off check; if all socket = skip
		_eTestPPIdx TestSite_Index = (_eTestPPIdx)(/*_eTestPPIdx::*/eTestPP_1 + (DvcLdTbl_Loc - eDeviceStage_LD_TBL_1));

		bool all_site_is_off = true;
		std::vector< _ePressUnit > presses = g_TaskAutoTrayUnloadPP.m_test_modules_press_idx[TestSite_Index];
		for each(auto press_idx in presses) {
			all_site_is_off &= g_TaskPressUnit[press_idx].GetSocketOnCnt() == 0;
		}
		if (all_site_is_off)
			continue;

		// load table pocket 변경 (place to loadtable in loading pocket )
		// U U U U
		// U U U U
		// L L L L
		// L L L L
		for (int nY = 2; nY < MAX_LD_TBL_POCKET_CNT_Y; nY++)
		{
			for (int nX = 0; nX < MAX_LD_TBL_POCKET_CNT_X; nX++)
			{
				int nBitPos = (nX)+((nY - 2)*(MAX_LD_TBL_POCKET_CNT_X));
				g_DMCont.m_dmDevice.GetDeviceExist(DvcLdTbl_Loc, nX, nY) ?
					CLRBIT(pArrLdTblMask[nLdTbl_Index], nBitPos) : SETBIT(pArrLdTblMask[nLdTbl_Index], nBitPos);
			}
		}
	}
}


int CTaskAutoTrayUnloadPP::_MakeSchedule_TrayPp_Place_SetPlate_QaEndRemainDevice()
{
	int nSchdCount = m_listTrayPpSchedule.size();
	int nSetplateObj_Index = ( -1 );

	/*
	*	picker가 들고 있는 디바이스의 HW BIN 정보를 취합한다.
	*/
	int nArrHwBinCount = 0;
	eDeviceDataStage *pArrHwBins = new eDeviceDataStage[ g_BinCategory.nIndexCount ];
	nArrHwBinCount = FindUnloadigSetplateQaRemainDevice( pArrHwBins, g_BinCategory.nIndexCount );

	int nHandDevice[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ] = { 0, };
	for( int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++ )
	{
		for( int nX = 0; nX < ( MAX_TRAY_PP_CNT_X ); nX++ )
		{
			char setting = m_pTaskTrayPP->m_TrayPP_picker_setting[ nY ][ nX ];
			if( setting != 'L' )
				continue;

			int nExist = g_DMCont.m_dmDevice.GetDeviceExist( eDeviceStage_TRAY_PP_1+m_eTrayIdx, nX, nY );
			if( nExist ) {
				nHandDevice[ nY ][ nX ] = DEF_EXIST;
			}
			else {
				nHandDevice[ nY ][ nX ] = DEF_NOT_EXIST;
			}
		}
	}
	/*
	*	취합된 HW BIN 정보대로 스케줄을 생성한다.
	*/
	for( int nHwBin_Index = 0; nHwBin_Index < nArrHwBinCount; nHwBin_Index++ )
	{
		eDeviceDataStage NowHwBin = pArrHwBins[ nHwBin_Index ];
		if( NowHwBin == ( -1 ) ) { continue; }
		//place 스케줄 만들기.
		/* 주의!
		*	스케줄이 확정된 Stage로 모션을 지정할 경우 eLocIdxTrayPP 값을 사용해야 함.
		*/
		eLocIdxTrayPP TrayPpLoc = ( eLocIdxTrayPP )( LOC_IDX_TRAY_PP_CONV_1 + ( NowHwBin - eDeviceStage_STACKER_CONV_1 ) );
		int nPockX, nPockY;
		int nArrPickerAssign[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ];

		/*
		*	Picker와 일치하는 pattern을 찾기 위해
		*	300x300 버퍼 공간에 C-Tray device 정보를 채운다.
		*	C-Tray (0,0) = 버퍼 공간 (100,100) 위치
		*/
		int nArrVirtualCTray[ VIRTUAL_POCKET_Y_MAX ][ VIRTUAL_POCKET_X_MAX ]; // data 검색용 버퍼
		int nHandAvailable[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ] = { 0, };
		memset( nArrVirtualCTray, 0xFF, sizeof( nArrVirtualCTray ) );

		/*
		*	작업 가능한 picker가 없으면 볼 것도 없다.
		*/
		int nAvailableCount = GetDeviceExist_CTrayHandQaRemainDevice( DEF_PLACE, NowHwBin, ( int* )nHandAvailable, MAX_TRAY_PP_CNT_X, MAX_TRAY_PP_CNT_Y );
		int nRemainDevice = 0;
		for( int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++ )
		{
			for( int nX = 0; nX < ( MAX_TRAY_PP_CNT_X ); nX++ )
			{
				char setting = m_pTaskTrayPP->m_TrayPP_picker_setting[ nY ][ nX ];
				if( setting != 'L' )
					continue;

				if( nHandDevice[ nY ][ nX ] == DEF_NOT_EXIST ) {
					nHandAvailable[ nY ][ nX ] = DEF_NOT_EXIST;
				}

				if( nHandAvailable[ nY ][ nX ] ) {
					nRemainDevice++;
				}
			}
		}

		if( nRemainDevice <= 0 ) { continue; }

		/*
		*	가상의 공간에 C-Tray 디바이스 맵을 배치한다.
		*/
		FillDeviceData_VirtualCTray( NowHwBin, ( int* )nArrVirtualCTray, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, VIRTUAL_POCKET_X_START, VIRTUAL_POCKET_Y_START );

		/*
		*	place 할 수 있는 pattern을 찾는다.
		*/
		int nRetCode = FindSortToSetplate( NowHwBin, nArrVirtualCTray, nHandAvailable, DEF_PLACE, &nPockX, &nPockY, nArrPickerAssign, CTaskAutoTrayUnloadPP::eRobotWorkDirTopLeft );
		if( nRetCode > 0 ) {
			CTrayUnloadPPCmd_MovePickPlace* pCmd = new CTrayUnloadPPCmd_MovePickPlace();
			pCmd->m_Cmd.eStage = TrayPpLoc;

			for( int nPickY = 0; nPickY < MAX_TRAY_PP_CNT_Y; nPickY++ )
			{
				for( int nPickX = 0; nPickX < ( MAX_TRAY_PP_CNT_X ); nPickX++ )
				{
					char setting = m_pTaskTrayPP->m_TrayPP_picker_setting[ nPickY ][ nPickX ];
					if( setting != 'L' )
						continue;

					if( nArrPickerAssign[ nPickY ][ nPickX ] != 0 ) {
						pCmd->m_Cmd.vPicker.push_back( CPoint( nPickX, nPickY ) );
						nHandDevice[ nPickY ][ nPickX ] = DEF_NOT_EXIST;
					}
				}
			}

			pCmd->m_Cmd.Pocket.SetPoint( nPockX, nPockY );
			pCmd->m_Cmd.ePickPlace = ePick;

			// find same command
			BOOL bSameCmd = FALSE;
			std::find_if( m_listTrayPpSchedule.begin(), m_listTrayPpSchedule.end()
						  , [ & ] ( CBaseSeqCmd* _pTrayPpCmd ) -> bool
			{
				CTrayUnloadPPCmd_MovePickPlace* pTrayPpCmd = ( CTrayUnloadPPCmd_MovePickPlace* )_pTrayPpCmd;
				if( pTrayPpCmd->m_Cmd.eStage == TrayPpLoc && pTrayPpCmd->m_Cmd.Pocket == pCmd->m_Cmd.Pocket ) {
					bSameCmd = TRUE;
					return true;
				}
				return false;
			} );

			// no search same command
			if( bSameCmd == FALSE ) {
				m_listTrayPpSchedule.push_back( pCmd );
				break;
			}
			else {
				delete pCmd;
			}
		}
		else {
			/*
			*	더이상 만들 수 없음.
			*/
			break;
		}
		//		}/*for(int no_infinite*/
	}/*for( nHwBin_Index*/
	delete[] pArrHwBins;

	//새롭게 등록된 스케줄 횟수 리턴
	return m_listTrayPpSchedule.size() - nSchdCount;
}

/*=============================================================================
	DESCRIPT : Test Site(PressUnit) Index에 대한 Load Table Index를 찾는다.
	KEY WORD :
	ARGUMENT :
	RETURN   : 찾아낸 Table의 Index (-1 값이라면 찾지 못한 경우인데 이런 경우는 애초에 이 함수를 사용하면 안된다!)
==============================================================================*/
int CTaskAutoTrayUnloadPP::ConvertTableIndex(_ePressUnit eTestSiteIdx)
{
	int nRetTableIndex = -1;

	switch (eTestSiteIdx)
	{
		// Load Table #1
	case ePressUnit_Down_1:
	case ePressUnit_Down_2:
		//case ePressUnit_Up_1:
		//case ePressUnit_Up_2:
	case ePressUnit_Down_3:
	case ePressUnit_Down_4:
		//case ePressUnit_Up_3:
		//case ePressUnit_Up_4:
		nRetTableIndex = eLoadTbl_1;
		break;
	case ePressUnit_Down_5:
	case ePressUnit_Down_6:
		//case ePressUnit_Up_5:
		//case ePressUnit_Up_6:
	case ePressUnit_Down_7:
	case ePressUnit_Down_8:
		//case ePressUnit_Up_7:
		//case ePressUnit_Up_8:
		nRetTableIndex = eLoadTbl_2;
		break;
	}

	ASSERT(nRetTableIndex >= 0);
	return nRetTableIndex;
}


int CTaskAutoTrayUnloadPP::get_unload_picker_empty_mask()
{
	int nPickerMask = 0x00;

	if (g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TRAY_PP_2 + m_eTrayIdx, 1, 0)) { CLRBIT(nPickerMask, 0); }
	else { SETBIT(nPickerMask, 0); }
	if (g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TRAY_PP_2 + m_eTrayIdx, 3, 0)) { CLRBIT(nPickerMask, 1); }
	else { SETBIT(nPickerMask, 1); }
	if (g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TRAY_PP_2 + m_eTrayIdx, 1, 1)) { CLRBIT(nPickerMask, 2); }
	else { SETBIT(nPickerMask, 2); }
	if (g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TRAY_PP_2 + m_eTrayIdx, 3, 1)) { CLRBIT(nPickerMask, 3); }
	else { SETBIT(nPickerMask, 3); }
	// 강제로 있도록하여 pick 하지 못하도록 한다.
	CLRBIT(nPickerMask, 4);	CLRBIT(nPickerMask, 5);	CLRBIT(nPickerMask, 6);	CLRBIT(nPickerMask, 7);

	return nPickerMask;
}

int CTaskAutoTrayUnloadPP::FindUnloadigSetplateQaRemainDevice(eDeviceDataStage *pArrDvcHwBins, int nMaxArrDvcHwBins)
{
	int nRet_HwBinCount = 0;

	eDeviceDataStage NowLoadSetplateDvcLoc = (eDeviceDataStage)(-1);
	ST_DD_DEVICE stDvc;

	int MAX_UNLOAD_SETPLATE = g_unload_stackers.size();
	CHw_Bin_Sort_Item *pArrTempHwBins = new CHw_Bin_Sort_Item[MAX_UNLOAD_SETPLATE];

	//Unload Picker가 들고있는 device에 대해서 조사하자.
	for (int nPickY = 0; nPickY < MAX_TRAY_PP_CNT_Y; nPickY++)
	{
		for (int nPickX = 0; nPickX < MAX_TRAY_PP_CNT_X; nPickX++)
		{
			char setting = m_pTaskTrayPP->m_TrayPP_picker_setting[nPickY][nPickX];
			if (setting != 'U')
				continue;

			//배열이 모두 차면 더 이상 하지 말라!
			if (nMaxArrDvcHwBins <= nRet_HwBinCount) { goto goto_pArrDvcHwBins_full; }

			if (g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TRAY_PP_1 + m_eTrayIdx, nPickX, nPickY) != 0) {
				//hand가 들고 있는 디바이스 HW bin을 알아낸다. (동일 SW BIN 에 대한 여러 HW BIN)
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TRAY_PP_1 + m_eTrayIdx, nPickX, nPickY, /*pDvc*/&stDvc);

				int nTempHwBinCount = 0;
				int nSortType = g_DMCont.m_dmEQP.GN(NDM0_SortType);// enum eSortingType{   
				if (nSortType == eSortType_SwBin) {
					nTempHwBinCount = convert_sw_bin_to_hw_bin_multi(pArrTempHwBins, MAX_UNLOAD_SETPLATE, stDvc.szBinResult);
				}
				else if (nSortType == eSortType_ErrStr) {
					nTempHwBinCount = convert_ErrDisrip_to_hw_bin_multi(pArrTempHwBins, MAX_UNLOAD_SETPLATE, stDvc.szErrString);
				}

				for (int nTempHwBin_I = 0; nTempHwBin_I < nTempHwBinCount; nTempHwBin_I++)
				{
					NowLoadSetplateDvcLoc = pArrTempHwBins[nTempHwBin_I].eHwBin;
					//bin 결과를 모르는 경우는 일단 skip
					if (NowLoadSetplateDvcLoc == (-1)) { continue; }

					//모든 것이 완벽하다면, 중복 검사 실시.
					BOOL bDuplicated = FALSE;
					for (int nDuplicateChkI = 0; nDuplicateChkI < nRet_HwBinCount; nDuplicateChkI++)
					{
						if (pArrDvcHwBins[nDuplicateChkI] == NowLoadSetplateDvcLoc) { bDuplicated = TRUE; break; }
					}

					//중복 검사에 발견 되지 않은 값이면 추가!
					if (bDuplicated == FALSE) {
						pArrDvcHwBins[nRet_HwBinCount] = NowLoadSetplateDvcLoc;
						++nRet_HwBinCount;
					}
				}
			}/*if(GetDeviceExist*/
		}/*for(nPickX*/
	}/*for(nPickY*/


goto_pArrDvcHwBins_full:

	//priority : 1 step HwBin Decending
	for (int nRef_I = 0; nRef_I < nRet_HwBinCount; nRef_I++) {
		for (int nCmp_I = 1; nCmp_I < nRet_HwBinCount; nCmp_I++) {
			if (pArrTempHwBins[nRef_I].eHwBin < pArrTempHwBins[nCmp_I].eHwBin) {
				CHw_Bin_Sort_Item Temp = pArrTempHwBins[nRef_I];
				pArrTempHwBins[nRef_I] = pArrTempHwBins[nCmp_I];
				pArrTempHwBins[nCmp_I] = Temp;
			}
		}
	}

	int nSortType = g_DMCont.m_dmEQP.GN(NDM0_SortType);// enum eSortingType{   
	if (nSortType == eSortType_SwBin) {
		//priority : 2 step LoadingTime Accending (where same SW Bin)
		for (int nRef_I = 0; nRef_I < nRet_HwBinCount; nRef_I++) {
			for (int nCmp_I = 1; nCmp_I < nRet_HwBinCount; nCmp_I++) {
				//same SW bin
				if (strcmp(pArrTempHwBins[nRef_I].szSwBin, pArrTempHwBins[nCmp_I].szSwBin) == 0) {
					if (pArrTempHwBins[nRef_I].dLoadingTime > pArrTempHwBins[nCmp_I].dLoadingTime) {
						CHw_Bin_Sort_Item Temp = pArrTempHwBins[nRef_I];
						pArrTempHwBins[nRef_I] = pArrTempHwBins[nCmp_I];
						pArrTempHwBins[nCmp_I] = Temp;
					}
				}
			}
		}
	}
	else if (nSortType == eSortType_ErrStr) {
		//priority : 2 step LoadingTime Accending (where same ErrStr)
		for (int nRef_I = 0; nRef_I < nRet_HwBinCount; nRef_I++) {
			for (int nCmp_I = 1; nCmp_I < nRet_HwBinCount; nCmp_I++) {
				//same ErrStr
				if (strcmp(pArrTempHwBins[nRef_I].szErrStr, pArrTempHwBins[nCmp_I].szErrStr) == 0) {
					if (pArrTempHwBins[nRef_I].dLoadingTime > pArrTempHwBins[nCmp_I].dLoadingTime) {
						CHw_Bin_Sort_Item Temp = pArrTempHwBins[nRef_I];
						pArrTempHwBins[nRef_I] = pArrTempHwBins[nCmp_I];
						pArrTempHwBins[nCmp_I] = Temp;
					}
				}
			}
		}
	}

	//Output HW bin info
	for (int nRef_I = 0; nRef_I < nRet_HwBinCount; nRef_I++) {
		pArrDvcHwBins[nRef_I] = pArrTempHwBins[nRef_I].eHwBin;
	}

	//SAFE_DELETE(pDvc);
	SAFE_DELETE_ARRAY(pArrTempHwBins);

	return nRet_HwBinCount;
}

int CTaskAutoTrayUnloadPP::GetDeviceExist_CTrayHandQaRemainDevice(BOOL bPickPlace, eDeviceDataStage WorkBin, int* pnBuffer, int nBufSizeX, int nBufSizeY)
{
	int nRet_ExistCount = 0;
	int nExist = 0;

	ASSERT(nBufSizeX >= MAX_TRAY_PP_CNT_X); //버퍼 사이즈가 picker x 수량보다 많아야..
	ASSERT(nBufSizeY >= MAX_TRAY_PP_CNT_Y); //버퍼 사이즈가 picker y 수량보다 많아야..

	if (bPickPlace == DEF_PICK)
	{
		for (int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++)
		{
			for (int nX = 0; nX < (MAX_TRAY_PP_CNT_X); nX++)
			{
				char setting = m_pTaskTrayPP->m_TrayPP_picker_setting[nY][nX];
				if (setting != 'L') {
					//pick일땐 unload용 picker 사용 못함 (디바이스가 있다고 치자)
					pnBuffer[nX + (nY*nBufSizeX)] = DEF_EXIST;
					continue;
				}

				nExist = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TRAY_PP_1 + m_eTrayIdx, nX, nY);
				if (nExist) {
					pnBuffer[nX + (nY*nBufSizeX)] = DEF_EXIST;
				}
				else {
					pnBuffer[nX + (nY*nBufSizeX)] = DEF_NOT_EXIST;
					nRet_ExistCount++;
				}
			}
		}
	}
	else if (bPickPlace == DEF_PLACE) {
		ST_DD_DEVICE stDvcData;
		for (int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++)
		{
			for (int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++)
			{
				char setting = m_pTaskTrayPP->m_TrayPP_picker_setting[nY][nX];
				if (setting == 'L') {
					//place일땐 load용 picker 사용 못함 (디바이스가 없다고 치자)
					pnBuffer[nX + (nY*nBufSizeX)] = DEF_NOT_EXIST;
					continue;
				}

				int MAX_UNLOAD_SETPLATE = g_unload_stackers.size();
				CHw_Bin_Sort_Item *pArrTempHwBins = new CHw_Bin_Sort_Item[MAX_UNLOAD_SETPLATE];
				BOOL bBinCmp = FALSE;

				//디바이스가 있고, 디바이스 BIN결과가 동일한 것만 있다고 한다.
				nExist = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TRAY_PP_1 + m_eTrayIdx, nX, nY);
				if (nExist) {
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TRAY_PP_1 + m_eTrayIdx, nX, nY, &stDvcData);

					// revice to cord
					int nRetHwBinCnt = 0;
					int nSortType = g_DMCont.m_dmEQP.GN(NDM0_SortType);// enum eSortingType{   
					if (nSortType == eSortType_SwBin) {
						nRetHwBinCnt = convert_sw_bin_to_hw_bin_multi(pArrTempHwBins, MAX_UNLOAD_SETPLATE, stDvcData.szBinResult);
					}
					else if (nSortType == eSortType_ErrStr) {
						nRetHwBinCnt = convert_ErrDisrip_to_hw_bin_multi(pArrTempHwBins, MAX_UNLOAD_SETPLATE, stDvcData.szErrString);
					}

					//priority : 1 step HwBin Decending
					for (int nRef_I = 0; nRef_I < nRetHwBinCnt; nRef_I++) {
						for (int nCmp_I = 1; nCmp_I < nRetHwBinCnt; nCmp_I++) {
							if (pArrTempHwBins[nRef_I].eHwBin < pArrTempHwBins[nCmp_I].eHwBin) {
								CHw_Bin_Sort_Item Temp = pArrTempHwBins[nRef_I];
								pArrTempHwBins[nRef_I] = pArrTempHwBins[nCmp_I];
								pArrTempHwBins[nCmp_I] = Temp;
							}
						}
					}

					if (nSortType == eSortType_SwBin) {
						//priority : 2 step LoadingTime Accending (where same SW Bin)
						for (int nRef_I = 0; nRef_I < nRetHwBinCnt; nRef_I++) {
							for (int nCmp_I = 1; nCmp_I < nRetHwBinCnt; nCmp_I++) {
								//same SW bin
								if (strcmp(pArrTempHwBins[nRef_I].szSwBin, pArrTempHwBins[nCmp_I].szSwBin) == 0) {
									if (pArrTempHwBins[nRef_I].dLoadingTime > pArrTempHwBins[nCmp_I].dLoadingTime) {
										CHw_Bin_Sort_Item Temp = pArrTempHwBins[nRef_I];
										pArrTempHwBins[nRef_I] = pArrTempHwBins[nCmp_I];
										pArrTempHwBins[nCmp_I] = Temp;
									}
								}
							}
						}
					}
					else if (nSortType == eSortType_ErrStr) {
						//priority : 2 step LoadingTime Accending (where same ErrStr)
						for (int nRef_I = 0; nRef_I < nRetHwBinCnt; nRef_I++) {
							for (int nCmp_I = 1; nCmp_I < nRetHwBinCnt; nCmp_I++) {
								//same ErrStr
								if (strcmp(pArrTempHwBins[nRef_I].szErrStr, pArrTempHwBins[nCmp_I].szErrStr) == 0) {
									if (pArrTempHwBins[nRef_I].dLoadingTime > pArrTempHwBins[nCmp_I].dLoadingTime) {
										CHw_Bin_Sort_Item Temp = pArrTempHwBins[nRef_I];
										pArrTempHwBins[nRef_I] = pArrTempHwBins[nCmp_I];
										pArrTempHwBins[nCmp_I] = Temp;
									}
								}
							}
						}
					}

					for (int i = 0; i < nRetHwBinCnt; i++) {
						if (pArrTempHwBins[i].eHwBin == WorkBin) {
							bBinCmp = TRUE;
							break;
						}
						else {
							bBinCmp = FALSE;
						}
					}
				}
				if (nExist && bBinCmp) {
					pnBuffer[nX + (nY*nBufSizeX)] = DEF_EXIST;
					nRet_ExistCount++;
				}
				else {
					pnBuffer[nX + (nY*nBufSizeX)] = DEF_NOT_EXIST;
				}

				SAFE_DELETE_ARRAY(pArrTempHwBins);
			}
		}
	}

	return nRet_ExistCount;
}

int CTaskAutoTrayUnloadPP::SetPlatePickEnableChk()
{
	int nReadyload = 0;
	for (std::vector<CTaskAutoStacker*>::size_type i = 0; i < g_load_stackers.size(); i++)
	{
		int nIdx = g_load_stackers[i]->GetStackerIndex();
		auto conv_state = g_TaskAutoStacker[nIdx].GetConveyorState();
		if (conv_state == PickEnable_Device)
			nReadyload++;
	}
	if (nReadyload == (int)g_load_stackers.size())
	{
		return nReadyload;
	}

	return nReadyload;
}
