#include "stdafx.h"
#include "TaskAutoTrayLoadPP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace base_seq_cmd;
using namespace std;

CTaskAutoTrayLoadPP::CTaskAutoTrayLoadPP( void )
{
	m_nAutoRunMainStep = eAutoLotState_InitNeed;
	m_nAutoRunSubStep = 0;
	m_nAutoRunBackUpMainStep = eAutoLotState_InitNeed;
	m_nAutoRunBackUpSubStep = 0;


	std::vector< _ePressUnit > press_group_left_module;
	press_group_left_module.push_back( ePressUnit_Down_1 );
	press_group_left_module.push_back( ePressUnit_Down_2 );
	press_group_left_module.push_back( ePressUnit_Down_3 );
	press_group_left_module.push_back( ePressUnit_Down_4 );
// 	press_group_left_module.push_back( ePressUnit_Up_1 );
// 	press_group_left_module.push_back( ePressUnit_Up_2 );
// 	press_group_left_module.push_back( ePressUnit_Up_3 );
// 	press_group_left_module.push_back( ePressUnit_Up_4 );

	std::vector< _ePressUnit > press_group_right_module;
	press_group_right_module.push_back( ePressUnit_Down_5 );
	press_group_right_module.push_back( ePressUnit_Down_6 );
	press_group_right_module.push_back( ePressUnit_Down_7 );
	press_group_right_module.push_back( ePressUnit_Down_8 );
// 	press_group_right_module.push_back( ePressUnit_Up_5 );
// 	press_group_right_module.push_back( ePressUnit_Up_6 );
// 	press_group_right_module.push_back( ePressUnit_Up_7 );
// 	press_group_right_module.push_back( ePressUnit_Up_8 );

	m_test_modules_press_idx.push_back( press_group_left_module );
	m_test_modules_press_idx.push_back( press_group_right_module );

	m_mapExcuteState.SetAt(eExcutingStep_Start, _T("Start"));

	m_mapExcuteState.SetAt(eExcutingStep_Table_Vacuum_Check_job, "eExcutingStep_Table_Vacuum_Check_job");
	m_mapExcuteState.SetAt(eExcutingStep_Table_Vacuum_Check_Start, "eExcutingStep_Table_Vacuum_Check_Start");
	m_mapExcuteState.SetAt(eExcutingStep_Table_Vacuum_Check_Wait, "eExcutingStep_Table_Vacuum_Check_Wait");
	m_mapExcuteState.SetAt(eExcutingStep_Table_Vacuum_Check_end, "eExcutingStep_Table_Vacuum_Check_end");

	m_mapExcuteState.SetAt(eExcutingStep_CheckCondition, "eExcutingStep_CheckCondition");

	m_mapExcuteState.SetAt(eExcutingStep_Pick, "eExcutingStep_Pick");
	m_mapExcuteState.SetAt(eExcutingStep_Place, "eExcutingStep_Place");
	m_mapExcuteState.SetAt(eExcutingStep_PickPlaceWait, "eExcutingStep_PickPlaceWait");
	m_mapExcuteState.SetAt(eExcutingStep_LoadTrayPpComptCheck, "eExcutingStep_LoadTrayPpComptCheck");
	m_mapExcuteState.SetAt(eExcutingStep_MoveAlreadyPos, "eExcutingStep_MoveAlreadyPos");
	m_mapExcuteState.SetAt(eExcutingStep_MoveLoadPos, "eExcutingStep_MoveLoadPos");
	m_mapExcuteState.SetAt(eExcutingStep_LoadTableWaitPos, "eExcutingStep_LoadTableWaitPos");

	m_mapExcuteState.SetAt(eExcutingStep_End, _T("End"));


	m_nWorkRecLoadTbl = 0;
}


CTaskAutoTrayLoadPP::~CTaskAutoTrayLoadPP( void )
{
}

int CTaskAutoTrayLoadPP::Initialize()
{
	m_eTrayIdx = eTrayPP_1_LD;
	m_pTaskTrayPP = &g_TaskTrayLoadPP;
	m_nJobTbl = -1;
	return CBaseTaskManager::Initialize();
}

void CTaskAutoTrayLoadPP::Finalize()
{
	CBaseTaskManager::Finalize();
}

int CTaskAutoTrayLoadPP::OnEvent( const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList/*=NULL*/ )
{
	return ERR_NO_ERROR;
}

int CTaskAutoTrayLoadPP::OnCommandRsp( CBaseSeqCmd* pRspCmd, int nEventId, WPARAM wParam, LPARAM lParam, std::vector<CPoint>& vPocket_Picker )
{
	if( nEventId == CMD_EVT_ALARM )
	{
		if( pRspCmd->m_pSender == this )
			ChangeMainStateByRspAlarm();
	}

	return CBaseTaskManager::OnCommandRsp( pRspCmd, nEventId, wParam, lParam, vPocket_Picker );
}

void CTaskAutoTrayLoadPP::OnOperatorCommand( int nOperatorCommand, WPARAM wParam, LPARAM lParam )
{
}

void CTaskAutoTrayLoadPP::ThreadProc1()
{
	SetThreadInterval( 0, 50 );
	while( GetThreadAliveFlag( 0 ) ) {
		if( GetPauseFlag( 0 ) ) {
			NEXT;
			continue;
		}
		AutoRunning();
		NEXT;
	}
}

void CTaskAutoTrayLoadPP::MakeLog( LPCTSTR fmt, ... )
{
	TCHAR tmpMsg[ 1024 ] = { 0, };
	if( fmt )
	{
		va_list argList;
		va_start( argList, fmt );
		_vstprintf_s( tmpMsg, fmt, argList );
		va_end( argList );
	}
	TWLOG.MakeLog( static_cast< int >(eLogID_Auto_Tray_1), Debug, "", __LINE__, NULL, tmpMsg );
}

void CTaskAutoTrayLoadPP::AutoState_Idle()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 1000:
		{
			m_nLast_Excuting_Step = 0;
			g_DMCont.m_dmTrayPP.SN(NDM5_TM_TrayPP_Tray_End, FALSE);
			ChangeSubState( 1001 );
		}break;
	case 1001:
		{

		}break;
	case 1002:
		{

		}break;
	default:
		ChangeSubState( 1000 );
		break;
	}
}

void CTaskAutoTrayLoadPP::AutoState_InitNeed()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 2000:
		{
			g_TaskTrayLoadPP.SetProcessCommand( eProcCmd_Stop );
			ChangeSubState( 2001 );
		}break;
	case 2001:
		{
		}break;
	default:
		ChangeSubState( 2000 );
		break;
	}
}

void CTaskAutoTrayLoadPP::AutoState_Initialize()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 3000:
		{
			CTrayLoadPPCmd_Initial* pSeqCmd = new CTrayLoadPPCmd_Initial();

			if( SendCommand( g_TaskTrayLoadPP, pSeqCmd ) == eSeqCmdAck_OK ) {
				ChangeSubState( 3001 );
				break;
			}
		}break;

	case 3001:
		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = ( CBaseSeqCmd::eSeqCmdStatus )g_TaskTrayLoadPP.GetSeqCmdStatus();
			if( load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready ) {
				break;
			}

			ChangeSubState( 3002 );
		}break;
	case 3002: // Blow Warming up 기능 추가
	{
		int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
		int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);

		if ((nLotStartMode == eStartMD_Initial || nLotStartMode == eStartMD_Retest || nLotStartMode == eStartMD_QA || nLotStartMode == eStartMD_AutoLoop || nLotStartMode == eStartMD_AutoGRR))
		{
			if (nDeviceMode == eDeviceMode && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
			{
				for (int y = 0; y < MAX_TRAY_PP_CNT_Y; y++) {
					for (int x = 0; x < MAX_TRAY_PP_CNT_X; x++) {
						eOnOffState state = g_TaskTrayLoadPP.GetVacuum(CPoint(x, y));
						if (state == eOff) {
							g_TaskTrayLoadPP.do_VacBlow(eVacBlwSt_BlwOn, CPoint(x, y));
						}
					}
				}

				int nSleep = g_DMCont.m_dmTrayPP.GN(NDM5_TM_LDTrayPP_Blow);
				if (nSleep > 0) {
					Sleep(nSleep);
				}

				for (int y = 0; y < MAX_TRAY_PP_CNT_Y; y++) {
					for (int x = 0; x < MAX_TRAY_PP_CNT_X; x++) {
						eOnOffState state = g_TaskTrayLoadPP.GetVacuum(CPoint(x, y));
						if (state == eOff) {
							g_TaskTrayLoadPP.do_VacBlow(eVacBlwSt_Off, CPoint(x, y));
						}
					}
				}
			}
		}
		ChangeSubState(3999);
	}break;
	case 3999:
		{
			ChangeMainState( eAutoLotState_Idle );
		}break;

	default:
		ChangeSubState( 3000 );
		break;
	}
}

void CTaskAutoTrayLoadPP::AutoState_Pause()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d] [last step=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep, m_nLast_Excuting_Step );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 4000:
		{
			g_TaskTrayLoadPP.SetProcessCommand( eProcCmd_Pause );
			ChangeSubState( 4001 );
		}break;
	case 4001:
		{
		}break;
	default:
		ChangeSubState( 4000 );
		break;
	}
}

void CTaskAutoTrayLoadPP::AutoState_Execute()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%s]" ), GetLotStateToString( m_nAutoRunMainStep ), GetExcuteStateStr() );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case eExcutingStep_Start:
		{
			ChangeSubState(eExcutingStep_Table_Vacuum_Check_job);
		}break;
	case eExcutingStep_Table_Vacuum_Check_job:
	{
		int nJob = SetVacuumCheck_TrayPP_Schedule(DEF_PICK, _eUsePicker_Load);
		if (nJob == eExcutingStep_FindJob) {	
			ChangeSubState(eExcutingStep_Table_Vacuum_Check_Start);
		}
		
		else if (nJob == eExcutingStep_ErrorJob)
			ChangeSubState(eExcutingStep_Table_Vacuum_Check_end);
	}break;
	case eExcutingStep_Table_Vacuum_Check_Start:
	{
		CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
		if (load_pp_cmd_status == CBaseSeqCmd::eSeqCmdStatus_Ready) {
			//CTrayLoadPPCmd_MovePickPlace* pSeqCmd = ( CTrayLoadPPCmd_MovePickPlace* )m_listTrayPpSchedule.front();
			CTrayLoadPPCmd_MovePickPlace* pSeqCmd = (CTrayLoadPPCmd_MovePickPlace*)GetNextSchedule();
			pSeqCmd->bVacuumCheckFuction = TRUE;
			g_TaskAutoTrayLoadPP.MakeLog("[EXEC] [Step=%8d] [Work :: CmdStage=%s, Xpos:%d, Ypos:%d, PickerCnt=%d, [%s] [Pick/Place=%s]"
				, m_nAutoRunSubStep, m_pTaskTrayPP->GetLocName(pSeqCmd->eStage)
				, pSeqCmd->Pocket.x + 1, pSeqCmd->Pocket.y + 1
				, (int)pSeqCmd->vPicker.size(), m_pTaskTrayPP->GetPickerName(pSeqCmd->vPicker), (pSeqCmd->ePickPlace == ePick ? _T("Pick") : _T("Place")));

			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_Table_Vacuum_Check_Wait);
				break;
			}
		}
	
	}break;
	case eExcutingStep_Table_Vacuum_Check_Wait:
	{
		CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
		if (load_pp_cmd_status == CBaseSeqCmd::eSeqCmdStatus_Ready) {
			ChangeSubState(eExcutingStep_Table_Vacuum_Check_job);
		}
	}break;
	case eExcutingStep_Table_Vacuum_Check_end:
	{
		g_DMCont.m_dmShuttle.SB(BDM3_TABLE_LOAD_VACUUM_CHECK, FALSE);
		if(g_DMCont.m_dmShuttle.GB(BDM3_TABLE_LOAD_VACUUM_CHECK) == FALSE && g_DMCont.m_dmShuttle.GB(BDM3_TABLE_UNLOAD_VACUUM_CHECK) == FALSE)
			ChangeSubState(eExcutingStep_CheckCondition);
	}break;
	case eExcutingStep_CheckCondition:
		{
			// picker에 device 비어있을 경우
			// pick

			// picker에 모두 device 붙어 있을 경우
			// place



			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = ( CBaseSeqCmd::eSeqCmdStatus )g_TaskTrayLoadPP.GetSeqCmdStatus();
			if( load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready ) {
				break;
			}

			LoadingTblSetReserve();


			CString strMsg = _T("");
			std::vector<CString> vErrString; vErrString.clear();
			int nChkErr = m_pTaskTrayPP->CheckDevice(strMsg, vErrString);
			if (nChkErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat(strMsg);
				g_Error.AlarmReport(nChkErr, AlarmData, &g_TaskSystemCtrl, m_pTaskTrayPP->GetFailPicker());
				g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
				break;
			}

			if (g_TaskSystemCtrl.GetAutoGrrUse() == TRUE) {
				int nReservedModule1 = g_DMCont.m_dmTestPP.GN(NDM04_SITEGROUP1_INSERTED_DEVICE_CNT);
				int nReservedModule2 = g_DMCont.m_dmTestPP.GN(NDM04_SITEGROUP2_INSERTED_DEVICE_CNT);
				int nSetInDvc = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrUnitCount);

				if (nSetInDvc == (nReservedModule1 + nReservedModule2)) {
					ChangeSubState(eExcutingStep_CheckCondition);
					break;
				}
			}

			// [7/9/2023 dohyeong.kim]
			//tray 공급도 확인 되어야 됨.
			BOOL bLoadingBreak = g_TaskAllAutoStacker.GetLoadTraySupplyEnd();
			int dvc_cnt = g_TaskTrayLoadPP.GetTrayPpDvcCnt();
			BOOL bAutoQAUse = g_TaskSystemCtrl.GetAutoQAUse();
			bool feeder_tray_empty_device = g_TaskTrayFeeder.IsEmptyDevice();
			//BOOL bIsTrayLastRow = g_TaskTrayFeeder.GetIsTrayLastRow();

 			ST_DD_DEVICE stDvc;
			int rear_picker_device = 0;
			int front_picker_device = 0;

			for (int nY = 0; nY < MAX_TRAY_PP_CNT_X; nY++)
			{
				for (int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++)
				{
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TRAY_PP_1, nX, nY, &stDvc);

					if (nY == 0)
					{
						if (stDvc.sExist == DEF_EXIST)
							front_picker_device++;
					}
					else
					{
						if (stDvc.sExist == DEF_EXIST)
							rear_picker_device++;
					}
				}
			}

			//  [7/31/2024 dohyeong.kim]1호기 처리 사항
			/*
				Feeder 마지막 2열 Pick은 Rear Picker로 진행.
				Rear Picker로 집을 경우, 8개 Pick 안해도 Place 하도록 예외처리

				2호기 첫번째 줄 Rear Picker 부터 Pick하게 되면 8개 못집는 현상 확인 되어 예외처리 진행 .
			*/
			BOOL bPick_Step_Continue = FALSE;

			// 1호기 Feeder 개선으로 주석 처리
			//  [4/23/2025 donghyun.shin]
//			int nEQPNo = g_DMCont.m_dmEQP.GN(NDM11_Handler_EQP_No);

// 			if (front_picker_device == 0 && rear_picker_device == (MAX_TRAY_PP_CNT_X * MAX_TRAY_PP_CNT_Y/2) && nEQPNo == DEF_EQUIPMENT_UNIT1)
// 			{
// 				bPick_Step_Continue = TRUE;
// // 				bPick_Step_Continue = TRUE;
// // 
// // 				int nGrrModeUse = g_TaskSystemCtrl.GetAutoGrrUse();
// // 
// // 				if (nEQPNo > DEF_EQUIPMENT_UNIT1 && nGrrModeUse == TRUE)
// // 					bPick_Step_Continue = FALSE;
// // 				else
// // 					bPick_Step_Continue = TRUE;	
// 			}


			int TrayDiv_Y = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);

			int nY_PitchType = 1;
			if (g_DMCont.m_dmTrayPP.GN(NDM5_Tray_Pitch_Y) < DEF_TRAY_PP_MAX_Y_PITCH_NARROW) {
				nY_PitchType = 2;
			}

			int nBaseRow = TrayDiv_Y - nY_PitchType; 
			int nFeederJobStep = nBaseRow - (-VIRTUAL_POCKET_X_START + 1);
			int nFrontDevCnt = g_TaskTrayLoadPP.GetTrayPpFrontPickerDvcCnt();
			int nFeederStep = g_TaskTrayFeeder.GetTrayDeviceExistRowStep();
		

			BOOL bTrayDevicePlace = (nFeederStep >= nFeederJobStep && (nFrontDevCnt == MAX_TRAY_PP_CNT_X*MAX_TRAY_PP_CNT_Y / 2));

			int nQAStatus = g_TaskSystemCtrl.GetQAStatus();
			int nQaSampleAddSumLoadCnt = g_DMCont.m_dmEQP.GN(NDM0_QaSampleAddSumLoadCnt);

			int nQaSampleCnt = g_TaskSystemCtrl.GetQASampleCount() + nQaSampleAddSumLoadCnt;
			int nLotLoadCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt);  //투입 된 수량
			nLotLoadCnt += g_TaskTrayLoadPP.GetTrayPpLoadDvcCnt();
			int nQaSampleLoadCnt = g_DMCont.m_dmEQP.GN(NDM0_QaSampleLoadCnt);


			BOOL isQALoadPickEnd = g_TaskTrayLoadPP.IsQALoadPickEnd(nQaSampleCnt, nLotLoadCnt, bAutoQAUse, (eQAStatus)nQAStatus);
	
			int nDvcFullCnt = g_TaskTrayLoadPP.GetDeviceFullCount();
			if((( (dvc_cnt != nDvcFullCnt && (bTrayDevicePlace == FALSE && feeder_tray_empty_device == false)) && g_TaskSystemCtrl.IsOneCycleModeOn() == FALSE && g_TaskSystemCtrl.GetAutoGrrCancel() == FALSE) && g_TaskAllAutoStacker.GetLoadEnableStackerCount() != 0) && feeder_tray_empty_device == false && bPick_Step_Continue != TRUE && isQALoadPickEnd != TRUE)
			{
				CTaskTrayFeeder::eWorkSite work_site = g_TaskTrayFeeder.WhereIsWorkSite();
				CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = ( CBaseSeqCmd::eSeqCmdStatus )g_TaskTrayFeeder.GetSeqCmdStatus();
				if( work_site != CTaskTrayFeeder::eWorkSite_NotAvaliable &&
					feeder_cmd_status == CBaseSeqCmd::eSeqCmdStatus_Ready &&
					g_DMCont.m_dmTrayPP.GN(NDM5_TM_TrayPP_Tray_End) == FALSE)
				{
					ChangeSubState( eExcutingStep_Pick );
					break;
				}
 				else {				//Tray에 Load Device가 있는 상태에서 알고리즘 탐색이 안되면 다시 탐색 하도록 변경
					if (feeder_tray_empty_device == false && nFeederStep >= nFeederJobStep)
					{
						eDeviceDataStage work_stage = eDeviceStage_Undefine;
						CTrayLoadPPCmd_MovePickPlace* pSeqCmd = new CTrayLoadPPCmd_MovePickPlace();
						work_stage = eDeviceDataStage::eDeviceStage_TRAY_FEEDER;
						auto ExecutingStep = _MakeSchedule_TrayPp_Pick_SetPlate(_eTrayPPIdx::eTrayPP_1_LD, work_stage, pSeqCmd->Pocket, pSeqCmd->vPicker,TRUE);
						SAFE_DELETE(pSeqCmd);
					}
					ChangeSubState(eExcutingStep_CheckCondition);
					break;
					
 				}
			}

			int nIdxLoadIdx = MakeLoadTblWorkingIdx();			

			if (nIdxLoadIdx != -1)
			{
				int new_place_schedule_cnt = _MakeSchedule_TrayPp_Place_LoadTable(_eTrayPPIdx::eTrayPP_1_LD, (eLocIdxTrayPP)nIdxLoadIdx);
				if (new_place_schedule_cnt > 0 || m_listTrayPpSchedule.size() > 0) {				
					ChangeSubState(eExcutingStep_Place);
				}
				else {
					ChangeSubState(eExcutingStep_MoveLoadPos);
				}

				g_TaskTrayFeeder.SetIsTrayLastRow(FALSE);
				break;
			}

			// Load Tray Load Deivce를 전부 들고 있을때, Test Side 영역에서 Table Reserve 해제 되면 미리 이동 하도록 추가
			// 단, Table 대기 위치가 동일 하면 그대로 있는다


			if (dvc_cnt == nDvcFullCnt/*MAX_TRAY_PP_CNT_X * MAX_TRAY_PP_CNT_Y*/ && feeder_tray_empty_device == false)
			{
				int nWorkReserveTbl2 = g_TaskLoadTbl[eLoadTbl_2].GetWorkReserve();
				int nWorkReserveTbl3 = g_TaskLoadTbl[eLoadTbl_3].GetWorkReserve();

				int nWorkReserveTbl[2] = { nWorkReserveTbl2 ,nWorkReserveTbl3 };
				int nLoadTableIdx[2] = { eLoadTbl_2 ,eLoadTbl_3 };
				int nTestPPIdx = 0;
				int nJobTbl = -1;

				if(g_TaskSystemCtrl.IsOneCycleModeOn() == DEF_ON || g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == true)
				{
					ChangeSubState(eExcutingStep_CheckCondition);
					break;
				}

				if (RID_INIT_IDLE_RESERVE == nWorkReserveTbl2 || RID_INIT_IDLE_RESERVE == nWorkReserveTbl3) 
				{
					
					if (RID_INIT_IDLE_RESERVE == nWorkReserveTbl2 && RID_INIT_IDLE_RESERVE == nWorkReserveTbl3)
					{
						break;
					}
					if (RID_TRAY_PP_(eTrayPP_1_LD) == nWorkReserveTbl2 || RID_TRAY_PP_(eTrayPP_1_LD) == nWorkReserveTbl3)
					{
						break;
					}

					for (int i = 0; i < _countof(nLoadTableIdx); i++) 
					{
						if (g_TaskAutoTestPp[nTestPPIdx].GetModuleSocketOnCnt() > 0 && nWorkReserveTbl[i] == RID_INIT_IDLE_RESERVE
							&&  g_TaskLoadTbl[nLoadTableIdx[i]].GetLoadDeviceCnt() != ((nDvcFullCnt/*MAX_LD_TBL_POCKET_CNT_X * MAX_LD_TBL_POCKET_CNT_Y*/)/2))
						{
							//cTime = g_TaskLoadTbl[nUnloadTableIdx[i]].GetMoveSetplateTm();
							nJobTbl = nLoadTableIdx[i];
						}
						nTestPPIdx++;
					}

					if (nJobTbl >= 0)
					{
						CPoint pocket(0, 0);
						std::vector< CPoint > vPicker = { CPoint(0, 0) };
						int nChkTbl_X = m_pTaskTrayPP->ChkMotorPos(eLocIdxTrayPP(LOC_IDX_TRAY_PP_TBL_1 + nJobTbl), pocket, vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX);
						if (nChkTbl_X != ERR_NO_ERROR)
						{
							MakeLog(_T("eAutoTrayStep_MoveTableWaitPos [MainStep=%s] [SubStep=%s] Before=%d, After=%d"), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr(), m_nJobTbl, m_nJobTbl);
							m_nJobTbl = nJobTbl;
							ChangeSubState(eAutoTrayStep_MoveTableWaitPos);
							break;
						}
					}
				}
			}
		}break;

	case eExcutingStep_Pick:
		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CTrayLoadPPCmd_MovePickPlace* pSeqCmd = new CTrayLoadPPCmd_MovePickPlace();

			CTaskTrayFeeder::eWorkSite work_site = g_TaskTrayFeeder.WhereIsWorkSite();
			eDeviceDataStage work_stage = eDeviceStage_Undefine;

			switch( work_site )
			{
			case CTaskTrayFeeder::eWorkSite_Conveyor:
				{
					pSeqCmd->eStage = eLocIdxTrayPP::LOC_IDX_TRAY_PP_CONV_3;
					work_stage = eDeviceDataStage::eDeviceStage_STACKER_CONV_3;
				}break;
			case CTaskTrayFeeder::eWorkSite_Feeder:
				{
					bool feeder_tray_empty_device = g_TaskTrayFeeder.IsEmptyDevice();
					if (feeder_tray_empty_device) {
						ChangeSubState(eExcutingStep_CheckCondition);
						break;
					}

					pSeqCmd->eStage = eLocIdxTrayPP::LOC_IDX_TRAY_PP_FEEDER;
					work_stage = eDeviceDataStage::eDeviceStage_TRAY_FEEDER;
				}break;
			default:
				{
					SAFE_DELETE( pSeqCmd );
					ChangeSubState( eExcutingStep_CheckCondition );
				}return;
			}

			auto ExecutingStep = _MakeSchedule_TrayPp_Pick_SetPlate(_eTrayPPIdx::eTrayPP_1_LD, work_stage, pSeqCmd->Pocket, pSeqCmd->vPicker);
			if( ExecutingStep == eExcutingStep_CheckCondition ) {
				SAFE_DELETE(pSeqCmd);
				ChangeSubState( eExcutingStep_CheckCondition );
				break;
			}

			int nFeederStep = g_TaskTrayFeeder.GetTrayDeviceExistRowStep();

			int nRetFeederPos = g_TaskTrayFeeder.ChkMotorPos(CTaskTrayFeeder::eAxisY, LOC_IDX_FEEDER_LOAD_PICK_START_POS, nFeederStep);
			if (nRetFeederPos != ERR_NO_ERROR || g_TaskTrayFeeder.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				ChangeSubState(eExcutingStep_CheckCondition);
				if (pSeqCmd != NULL) {
					SAFE_DELETE(pSeqCmd);
				}
				break;
			}

			pSeqCmd->vSpecOutPicker;
			pSeqCmd->vFullCmdPkr;
			pSeqCmd->ePickPlace = ePick;
			pSeqCmd->bNormalPkrEnd;
			pSeqCmd->bSpeckOutPkrEnd;
			pSeqCmd->nCntSpecOut;
			pSeqCmd->bSpecOutPkrUse;

			g_TaskAutoTrayLoadPP.MakeLog("[EXEC] [Step=%8d] [Work :: CmdStage=%s, Xpos:%d, Ypos:%d, PickerCnt=%d, [%s], [Pick/Place=%s]"
				, m_nAutoRunSubStep, m_pTaskTrayPP->GetLocName(pSeqCmd->eStage)
				, pSeqCmd->Pocket.x + 1, pSeqCmd->Pocket.y + 1
				, (int)pSeqCmd->vPicker.size(), m_pTaskTrayPP->GetPickerName(pSeqCmd->vPicker), (pSeqCmd->ePickPlace == ePick ? _T("Pick") : _T("Place")));


			if( SendCommand( g_TaskTrayLoadPP, pSeqCmd ) == eSeqCmdAck_OK ) {
				ChangeSubState(eExcutingStep_LoadTableWaitPos);
				break;
			}
		}break;
	case eExcutingStep_Place:
	{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status == CBaseSeqCmd::eSeqCmdStatus_Ready) {		
				//CTrayLoadPPCmd_MovePickPlace* pSeqCmd = ( CTrayLoadPPCmd_MovePickPlace* )m_listTrayPpSchedule.front();
				CTrayLoadPPCmd_MovePickPlace* pSeqCmd = (CTrayLoadPPCmd_MovePickPlace*)GetNextSchedule();

				g_TaskAutoTrayLoadPP.MakeLog("[EXEC] [Step=%8d] [Work :: CmdStage=%s, Xpos:%d, Ypos:%d, PickerCnt=%d, [%s] [Pick/Place=%s]"
					, m_nAutoRunSubStep, m_pTaskTrayPP->GetLocName(pSeqCmd->eStage)
					, pSeqCmd->Pocket.x + 1, pSeqCmd->Pocket.y + 1
					, (int)pSeqCmd->vPicker.size(), m_pTaskTrayPP->GetPickerName(pSeqCmd->vPicker), (pSeqCmd->ePickPlace == ePick ? _T("Pick") : _T("Place")));
	
				if( SendCommand( g_TaskTrayLoadPP, pSeqCmd ) == eSeqCmdAck_OK ) {
					ChangeSubState(/*eExcutingStep_MoveLoadPos*/eExcutingStep_MoveAlreadyPos);
					break;
				}
			}
	}break;
	case eExcutingStep_MoveAlreadyPos:
	{
		CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
		if (load_pp_cmd_status == CBaseSeqCmd::eSeqCmdStatus_Ready) {
			CTrayLoadPPCmd_MovePos* pSeqCmd = new CTrayLoadPPCmd_MovePos();
			pSeqCmd->eStage = eLocIdxTrayPP::LOC_IDX_TRAY_PP_FEEDER;
			auto ExecutingStep = _MakeSchedule_TrayPp_Pick_SetPlate(_eTrayPPIdx::eTrayPP_1_LD, eDeviceDataStage::eDeviceStage_TRAY_FEEDER, pSeqCmd->Pocket, pSeqCmd->vPicker);
			if (ExecutingStep == eExcutingStep_CheckCondition) {
				std::vector<CPoint> vPicker = { CPoint(0,0) };
				pSeqCmd->Pocket = CPoint(0, 0);
			}

			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
			}

			ChangeSubState(eExcutingStep_CheckCondition);
			break;
		}
	}break;
	case eExcutingStep_MoveLoadPos:
		{
		//Grr Cancel 이면 safety 위치 해도 됨
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status == CBaseSeqCmd::eSeqCmdStatus_Ready) {
				std::vector<CPoint> vPicker = { CPoint(0,0) };
							
				CTrayLoadPPCmd_MovePos* pSeqCmd = new CTrayLoadPPCmd_MovePos();
				pSeqCmd->eStage = eLocIdxTrayPP::LOC_IDX_TRAY_PP_CONV_3;
				pSeqCmd->Pocket = CPoint(0, 0);

				if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
					CTrayLoadPPCmd_MovePickPlace* pCmd = new CTrayLoadPPCmd_MovePickPlace();
					_MakeSchedule_TrayPp_Pick_SetPlate(_eTrayPPIdx::eTrayPP_1_LD, eDeviceDataStage::eDeviceStage_TRAY_FEEDER, pCmd->Pocket, pCmd->vPicker);
					g_TaskTrayFeeder.SetIsTrayLastRow(FALSE);
					SAFE_DELETE(pCmd);
				}
				
				ChangeSubState(eExcutingStep_PickPlaceWait);
				break;
			}
		}break;
	case eExcutingStep_LoadTableWaitPos:
	{
		int nHandStatus = g_TaskTrayLoadPP.GetSeqCmdStatus();
		if (nHandStatus == CBaseSeqCmd::eSeqCmdStatus_Ready) 
		{
			CTime cTime = CTime::GetCurrentTime();
			int nJobTbl = -1;

			int nDvcFullCnt = g_TaskTrayLoadPP.GetDeviceFullCount();

			if (g_TaskTrayLoadPP.GetTrayPpDvcCnt() == nDvcFullCnt)
			{
				int nWorkReserveTbl2 = g_TaskLoadTbl[eLoadTbl_2].GetWorkReserve();
				int nWorkReserveTbl3 = g_TaskLoadTbl[eLoadTbl_3].GetWorkReserve();

				if (RID_TRAY_PP_(eTrayPP_1_LD) == nWorkReserveTbl2 || RID_TRAY_PP_(eTrayPP_1_LD) == nWorkReserveTbl3) {
					nJobTbl = MakeLoadTblWorkingIdx(TRUE);
				}			
				else
				{
					if ((g_TaskSystemCtrl.IsOneCycleModeOn() == DEF_OFF)
						|| g_TaskSystemCtrl.IsOneCycleModeOn() == DEF_ON || g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == true)
					{
						int nUnloadTableIdx[2] = { eLoadTbl_2 ,eLoadTbl_3 };
						int nTestPPIdx = 0;
						for (int i = 0; i < _countof(nUnloadTableIdx); i++) {
							if (g_TaskLoadTbl[nUnloadTableIdx[i]].GetMoveSetplateTm() < cTime && g_TaskAutoTestPp[nTestPPIdx].GetModuleSocketOnCnt() > 0) {
								cTime = g_TaskLoadTbl[nUnloadTableIdx[i]].GetMoveSetplateTm();
								nJobTbl = nUnloadTableIdx[i];
							}		
							nTestPPIdx++;
						}
					}
				}

				CTrayLoadPPCmd_MovePos* pSeqCmd = new CTrayLoadPPCmd_MovePos();
				if (nJobTbl != -1) {
					pSeqCmd->eStage = (eLocIdxTrayPP)(LOC_IDX_TRAY_PP_TBL_1 + nJobTbl);
					pSeqCmd->Pocket = CPoint(0, 0);
					if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
					}
				}
				else
				{
					if (pSeqCmd != NULL) {
						SAFE_DELETE(pSeqCmd);
					}
				}
			}
			
			ChangeSubState(eExcutingStep_CheckCondition);
			break;
		}

	}break;
	case eAutoTrayStep_MoveTableWaitPos:
	{
		int nHandStatus = m_pTaskTrayPP->GetSeqCmdStatus();
		if (nHandStatus == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			//Tray PP 에 Load Device 가 있으면 Table 위치로 이동 
			CTrayLoadPPCmd_MovePos* pCmd = NULL;

			pCmd = new CTrayLoadPPCmd_MovePos;

			if (m_nJobTbl != -1) {
				pCmd->eStage = (eLocIdxTrayPP)(LOC_IDX_TRAY_PP_TBL_1 + m_nJobTbl);
			}
			else {
				SAFE_DELETE(pCmd);
				ChangeSubState(eExcutingStep_PickPlaceWait);
				break;
			}

			CPoint pocket(0, 0);
			std::vector< CPoint > vPicker = { CPoint(0, 0) };
			pCmd->Pocket = pocket;
			pCmd->vPicker = vPicker;

			if (SendCommand(*m_pTaskTrayPP, pCmd) == eSeqCmdAck_OK) {
				CString strCmdName = _T("");
				if (m_nJobTbl != -1) {
					strCmdName.Format(_T("Alreay Table %d"), m_nJobTbl + 1);
				}
				else {
					strCmdName.Format(_T("Safety"));
				}
				MakeLog(_T("[MainStep=%s] [SubStep=%s] [Move %s Pos]"), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr(), strCmdName);
				ChangeSubState(eExcutingStep_PickPlaceWait);
				break;
			}
		}
	}

	case eExcutingStep_PickPlaceWait:
	{
		int nHandStatus = g_TaskTrayLoadPP.GetSeqCmdStatus();
		if (nHandStatus == CBaseSeqCmd::eSeqCmdStatus_Ready) {
			ChangeSubState(eExcutingStep_LoadTrayPpComptCheck);
			break;
		}
	}break;
	case eExcutingStep_LoadTrayPpComptCheck:
	{
		ChangeSubState(eExcutingStep_CheckCondition);
	}break;
	case eExcutingStep_End:
		{
			ChangeMainState( eAutoLotState_NormalComplete );
		}break;
	}
}

void CTaskAutoTrayLoadPP::AutoState_NormalComplete()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 6000:
		{
			ChangeSubState( 6001 );
		}break;
	case 6001:
		{

		}break;
	default:
		ChangeSubState( 6000 );
		break;
	}
}

void CTaskAutoTrayLoadPP::AutoState_AlarmStop()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d] [last step=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep, m_nLast_Excuting_Step );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 7000:
		{
			g_TaskTrayLoadPP.SetProcessCommand( base_seq_cmd::eProcCmd_Pause );
			ChangeSubState( 7001 );
		}break;
	case 7001:
		{

		}break;
	default:
		ChangeSubState( 7000 );
		break;
	}
}

void CTaskAutoTrayLoadPP::AutoState_Resume()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		//MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
		MakeLog( _T( "[MainStep=%s] [SubStep=%d] [last step=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep, m_nLast_Excuting_Step );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 8000:
		{
			g_TaskTrayLoadPP.SetProcessCommand(eProcCmd_Resume);
			ChangeSubState( 8001 );
		}break;

	case 8001:
		{
			ChangeMainState( eAutoLotState_Executing );
		}break;

	default:
		ChangeSubState( 8000 );
		break;
	}
}

void CTaskAutoTrayLoadPP::AutoState_CleanOut()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 9000:
	{
		int nHandStatus = g_TaskTrayLoadPP.GetSeqCmdStatus();
		if (nHandStatus == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			CTrayLoadPPCmd_MovePos* pSeqCmd = new CTrayLoadPPCmd_MovePos();
			pSeqCmd->eStage = eLocIdxTrayPP::LOC_IDX_TRAY_PP_CONV_3;
			pSeqCmd->Pocket = CPoint(0, 0);

			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(9999);
			}
		}
	}break;
	case 9999:
	{
		if (g_TaskTrayLoadPP.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
			break;
		// Lot가 진행 되면 Blow를 꺼준다 [9/16/2020 donghyun.shin]
		m_pTaskTrayPP->do_VacBlow_All(eVacBlwSt_Off);

		ChangeMainState(eAutoLotState_NormalComplete);

	}break;
	default:
		ChangeSubState(9000);
		break;
	}
}

CTaskAutoTrayLoadPP::_eExcutingStep CTaskAutoTrayLoadPP::_MakeSchedule_TrayPp_Pick_SetPlate( _eTrayPPIdx TrayPPIdx, eDeviceDataStage stage, CPoint& vPocket, std::vector<CPoint>& vPicker, BOOL bSearchingAlogrithm)
{
	if(g_TaskSystemCtrl.IsOneCycleModeOn() == true)
		return eExcutingStep_CheckCondition;

	int nPickerDvcPattern[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ] = { DEF_NOT_EXIST, };
	int nAvailableCount = GetDeviceExist_CTrayHand( TrayPPIdx, DEF_PICK, ( eDeviceDataStage )0, ( int* )nPickerDvcPattern, MAX_TRAY_PP_CNT_X, MAX_TRAY_PP_CNT_Y );

	if( nAvailableCount <= 0 ) {
		return eExcutingStep_CheckCondition;
	}

	if (bSearchingAlogrithm == TRUE)
	{
		switch (stage)
		{
		case eDeviceStage_STACKER_CONV_3:
		{
			/*	Setplate가 레디 상태일 경우에만.~ */
			// 			eStacker_Idx nSetplateObj_index = ( eStacker_Idx )( eSTACKER_03 );
			// 			eStacker_Conveyor_State conveyor_state = g_TaskAutoStacker[ nSetplateObj_index ].GetConveyorState();
			// 			if( conveyor_state != PickEnable ) {
			return eExcutingStep_CheckCondition;
			//			}
		}break;
		case eDeviceStage_TRAY_FEEDER:
		{
			CTaskTrayFeeder::eWorkSite work_site = g_TaskTrayFeeder.WhereIsWorkSite();
			if (work_site != CTaskTrayFeeder::eWorkSite_Feeder) {
				return eExcutingStep_CheckCondition;
			}
		}break;
		default:
		{
			return eExcutingStep_CheckCondition;
		}break;
		}
	}
	


	/* 주의!
	*	스케줄이 확정된 Stage로 모션을 지정할 경우 eLocIdxTrayPP 값을 사용해야 함.
	*/
	int nPockX, nPockY;

	int nArrPickerAssign[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ];

	/*
	*	Picker와 일치하는 pattern을 찾기 위해
	*	300x300 버퍼 공간에 C-Tray device 정보를 채운다.
	*	C-Tray (0,0) = 버퍼 공간 (100,100) 위치
	*/
	int nArrVirtualCTray[ VIRTUAL_POCKET_Y_MAX ][ VIRTUAL_POCKET_X_MAX ]; // data 검색용 버퍼
	memset( nArrVirtualCTray, 0xFF, sizeof( nArrVirtualCTray ) );

	FillDeviceData_VirtualCTray( stage, ( int* )nArrVirtualCTray, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, VIRTUAL_POCKET_X_START, VIRTUAL_POCKET_Y_START );

	/*
	* 가능한 많은 picker에 할당하기 위한 loop
	*/
	int nRetCode = 0;

	//장비 No
	int nEQPNo = g_DMCont.m_dmEQP.GN(NDM0_Handler_EQP_No);
	//1호기 Feeder 개선 건으로 무조건 Top Left로 탐색 하도록 변경
	//  [4/23/2025 donghyun.shin]
	//int nWorkDir = (nEQPNo > 1) ? eRobotWorkDirTopLeft : eRobotWorkDirBottomLeft;
	int nWorkDir = eRobotWorkDirTopLeft;
	//임시
	//nWorkDir = eRobotWorkDirTopLeft;


	nRetCode = FindPickFromSetplate( TrayPPIdx, stage, nArrVirtualCTray, nPickerDvcPattern, DEF_PICK, &nPockX, &nPockY, nArrPickerAssign, nWorkDir/*eRobotWorkDirBottomLeft*/, 0 );
	if( nRetCode > 0 ) {
		std::vector<CPoint> vPkr;
		for( int nPickY = 0; nPickY < MAX_TRAY_PP_CNT_Y; nPickY++ ) {
			for( int nPickX = 0; nPickX < ( MAX_TRAY_PP_CNT_X ); nPickX++ ) {
				if( nArrPickerAssign[ nPickY ][ nPickX ] != 0 ) {
					vPkr.push_back( CPoint( nPickX, nPickY ) );
				}
			}
		}

// 		BOOL bAutoQAUse = g_TaskSystemCtrl.GetAutoQAUse();
// 		if (bAutoQAUse == TRUE && g_TaskSystemCtrl.IsOneCycleModeOn() == false)
// 		{
// 			char szRetBuff[MAX_PATH] = { 0, };
// 			g_DMCont.m_dmHandlerOpt.GS(SDM11_QTY, szRetBuff, MAX_PATH);
// 			int nQA_Qty = atoi(szRetBuff);
// 			int nLotLoadCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt);  //투입 된 수량
// 			int nLoadTrayPPCnt = g_TaskTrayLoadPP.GetTrayPpLoadDvcCnt();   // Hand에 Pick 되어 있는 수량
// 			int nLoadDeviceSum = nLotLoadCnt + nLoadTrayPPCnt + vPkr.size();
// 			int nQAStatus = g_DMCont.m_dmEQP.GN(NDM0_QA_Status);
// 
// 			switch (nQAStatus)
// 			{
// 				case eQAStatus::eQAStatus_Load:
// 				{
// 					if (nQA_Qty < nLoadDeviceSum)
// 					{
// 						int nOverCount = nQA_Qty - (nLotLoadCnt + nLoadTrayPPCnt);
// 						do  
// 						{ 
// 							if (vPkr.size() == 0)
// 								return eExcutingStep_CheckCondition;
// 							else {
// 								vPkr.pop_back();
// 							}													
// 						} while (vPkr.size() != nOverCount);					
// 					}
// 				}break;
// 				case eQAStatus::eQAStatus_LoadEmpty:{}break;
// 				case eQAStatus::eQAStatus_End:{}break;
// 				default:
// 					break;
// 			}
// 		}

		vPicker = vPkr;
		vPocket = CPoint( nPockX, nPockY );
		return eExcutingStep_Pick;
	}

	return eExcutingStep_CheckCondition;
}

int CTaskAutoTrayLoadPP::GetDeviceExist_CTrayHand( _eTrayPPIdx TrayPPIdx, BOOL bPickPlace, eDeviceDataStage WorkBin, int* pnBuffer, int nBufSizeX, int nBufSizeY )
{
	CTaskTrayBasePP* TaskTrayPP = nullptr;
	switch( TrayPPIdx ) {
	case _eTrayPPIdx::eTrayPP_1_LD: { TaskTrayPP = &g_TaskTrayLoadPP; }break;
	case _eTrayPPIdx::eTrayPP_2_UD: { TaskTrayPP = &g_TaskTrayUnloadPP; }break;
	}

	int nRet_ExistCount = 0;
	int nExist = 0;

	ASSERT( nBufSizeX >= MAX_TRAY_PP_CNT_X ); //버퍼 사이즈가 picker x 수량보다 많아야..
	ASSERT( nBufSizeY >= MAX_TRAY_PP_CNT_Y ); //버퍼 사이즈가 picker y 수량보다 많아야..
	ASSERT( ( bPickPlace == DEF_PICK || bPickPlace == DEF_PLACE ) == TRUE ); //pick or place 여야만 함.

	ST_DD_DEVICE stDvcData;

	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nNumberOfLoadTable = g_DMCont.m_dmHandlerOpt.GN(NDM11_NUMBER_OF_LOADTABLE);

	if( bPickPlace == DEF_PICK )
	{
		for( int Y = 0; Y < MAX_TRAY_PP_CNT_Y; Y++ )
		{
			for( int X = 0; X < MAX_TRAY_PP_CNT_X; X++ )
			{
				char setting = TaskTrayPP->m_TrayPP_picker_setting[ Y ][ X ];
				/*nExist = */g_DMCont.m_dmDevice.GetDeviceData( eDeviceStage_TRAY_PP_1 + TrayPPIdx, X, Y, &stDvcData );
				if(setting == 'L' ) {
					// 4Para 2열만 Load 자재 담기위해 예외처리 추가. [8/5/2025 dohyeong.kim]
					if (nSiteDivY == Y && nNumberOfLoadTable == _eNumberOfLoadTable::eNumberOfLoadTable_4Unit) {
						pnBuffer[X + (Y * nBufSizeX)] = DEF_EXIST;
						continue;
					}

					pnBuffer[ X + ( Y * nBufSizeX ) ] = stDvcData.sExist;
					nRet_ExistCount += ( stDvcData.sExist == DEF_NOT_EXIST ) ? 1 : 0;
				}
				if( setting == 'U' ){ //pick일땐 unload용 picker 사용 못함 (디바이스가 있다고 치자)
					pnBuffer[ X + ( Y * nBufSizeX ) ] = DEF_EXIST;
				}
			}
		}
	}
	else if( bPickPlace == DEF_PLACE ) {
		for( int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++ )
		{
			for( int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++ ) {
				g_DMCont.m_dmDevice.GetDeviceData( eDeviceStage_TRAY_PP_1 + TrayPPIdx, nX, nY, /*pDev*/&stDvcData );

				auto setting = TaskTrayPP->m_TrayPP_picker_setting[ nY ][ nX ];
				if(setting == 'L' ){
					//place일땐 load용 picker 사용 못함 (디바이스가 없다고 치자)
					pnBuffer[ nX + ( nY * nBufSizeX ) ] = DEF_NOT_EXIST;
					continue;
				}

				int MAX_UNLOAD_SETPLATE = g_unload_stackers.size();
				CHw_Bin_Sort_Item* pArrTempHwBins = new CHw_Bin_Sort_Item[ MAX_UNLOAD_SETPLATE ];
				BOOL bBinCmp = FALSE;

				//디바이스가 있고, 디바이스 BIN결과가 동일한 것만 있다고 한다.
				//nExist = g_DMCont.m_dmDevice.GetDeviceExist( eDeviceStage_TRAY_PP, nX, nY );

				if( stDvcData.sExist ) {


					// revice to cord
					int nRetHwBinCnt = 0;
					int nSortType = g_DMCont.m_dmEQP.GN( NDM0_SortType );// enum eSortingType{   
					if( nSortType == eSortType_SwBin ) {
						nRetHwBinCnt = convert_sw_bin_to_hw_bin_multi( pArrTempHwBins, MAX_UNLOAD_SETPLATE,/*pDev->*/stDvcData.szBinResult );
					}
					else if( nSortType == eSortType_ErrStr ) {
						nRetHwBinCnt = convert_ErrDisrip_to_hw_bin_multi( pArrTempHwBins, MAX_UNLOAD_SETPLATE,/*pDev->*/stDvcData.szErrString );
					}


					//priority : 1 step HwBin Decending
					for( int nRef_I = 0; nRef_I < nRetHwBinCnt; nRef_I++ ) {
						for( int nCmp_I = 1; nCmp_I < nRetHwBinCnt; nCmp_I++ ) {
							if( pArrTempHwBins[ nRef_I ].eHwBin < pArrTempHwBins[ nCmp_I ].eHwBin ) {
								CHw_Bin_Sort_Item Temp = pArrTempHwBins[ nRef_I ];
								pArrTempHwBins[ nRef_I ] = pArrTempHwBins[ nCmp_I ];
								pArrTempHwBins[ nCmp_I ] = Temp;
							}
						}
					}

					if( nSortType == eSortType_SwBin ) {
						//priority : 2 step LoadingTime Accending (where same SW Bin)
						for( int nRef_I = 0; nRef_I < nRetHwBinCnt; nRef_I++ ) {
							for( int nCmp_I = 1; nCmp_I < nRetHwBinCnt; nCmp_I++ ) {
								//same SW bin
								if( strcmp( pArrTempHwBins[ nRef_I ].szSwBin, pArrTempHwBins[ nCmp_I ].szSwBin ) == 0 ) {
									if( pArrTempHwBins[ nRef_I ].dLoadingTime > pArrTempHwBins[ nCmp_I ].dLoadingTime ) {
										CHw_Bin_Sort_Item Temp = pArrTempHwBins[ nRef_I ];
										pArrTempHwBins[ nRef_I ] = pArrTempHwBins[ nCmp_I ];
										pArrTempHwBins[ nCmp_I ] = Temp;
									}
								}
							}
						}
					}
					else if( nSortType == eSortType_ErrStr ) {
						//priority : 2 step LoadingTime Accending (where same ErrStr)
						for( int nRef_I = 0; nRef_I < nRetHwBinCnt; nRef_I++ ) {
							for( int nCmp_I = 1; nCmp_I < nRetHwBinCnt; nCmp_I++ ) {
								//same ErrStr
								if( strcmp( pArrTempHwBins[ nRef_I ].szErrStr, pArrTempHwBins[ nCmp_I ].szErrStr ) == 0 ) {
									if( pArrTempHwBins[ nRef_I ].dLoadingTime > pArrTempHwBins[ nCmp_I ].dLoadingTime ) {
										CHw_Bin_Sort_Item Temp = pArrTempHwBins[ nRef_I ];
										pArrTempHwBins[ nRef_I ] = pArrTempHwBins[ nCmp_I ];
										pArrTempHwBins[ nCmp_I ] = Temp;
									}
								}
							}
						}
					}

					for( int i = 0; i < nRetHwBinCnt; i++ ) {
						if( pArrTempHwBins[ i ].eHwBin == WorkBin ) {
							bBinCmp = TRUE;
							break;
						}
						else {
							bBinCmp = FALSE;
						}
					}
				}
				if( stDvcData.sExist && bBinCmp ) {
					pnBuffer[ nX + ( nY * nBufSizeX ) ] = DEF_EXIST;
					nRet_ExistCount++;
				}
				else {
					pnBuffer[ nX + ( nY * nBufSizeX ) ] = DEF_NOT_EXIST;
				}

				//delete[] pArrTempHwBins;
				SAFE_DELETE_ARRAY( pArrTempHwBins );
			}//////////////////////////////
		}
		//SAFE_DELETE(pDev);
	}

	return nRet_ExistCount;
}

int CTaskAutoTrayLoadPP::FillDeviceData_VirtualCTray( eDeviceDataStage nStageLoc, int* pnBuffer, int nBufSizeX, int nBufSizeY, int nVtl0X, int nVtl0Y )
{
	int nExist = 0;
	int nPocketCount_X = 0;
	int nPocketCount_Y = 0;

	switch( nStageLoc )
	{
		//case eDeviceStage_STACKER_CONV_2:
	case eDeviceStage_STACKER_CONV_3:
	case eDeviceStage_STACKER_CONV_4:
	case eDeviceStage_STACKER_CONV_5:
	case eDeviceStage_STACKER_CONV_6:
	case eDeviceStage_STACKER_CONV_7:
	case eDeviceStage_TRAY_FEEDER:
		{
			nPocketCount_X = g_DMCont.m_dmTrayPP.GN( NDM5_TrayDiv_X );
			nPocketCount_Y = g_DMCont.m_dmTrayPP.GN( NDM5_TrayDiv_Y );
		}break;
	case eDeviceStage_TRAY_BUFF:
		{
			nPocketCount_X = 4;
			nPocketCount_Y = 4;
		}break;
	default:ASSERT( false ); break;
	}

	ASSERT( nBufSizeX >= nPocketCount_X ); //버퍼 사이즈가 picker x 수량보다 많아야..
	ASSERT( nBufSizeY >= nPocketCount_Y ); //버퍼 사이즈가 picker y 수량보다 많아야..

	for( int nY = 0; nY < nPocketCount_Y; nY++ )
	{
		for( int nX = 0; nX < nPocketCount_X; nX++ )
		{
			int nBuffer_Index = ( nX + nVtl0X )/*X*/ + ( ( nY + nVtl0Y ) * nBufSizeX )/*Y*/;
			nExist = g_DMCont.m_dmDevice.GetDeviceExist( nStageLoc, nX, nY );
			pnBuffer[ nBuffer_Index ] = nExist;

			if (nY == 31)
				int aa = 1;
		}
	}

	return ERR_NO_ERROR;
}

int CTaskAutoTrayLoadPP::convert_sw_bin_to_hw_bin_multi( CHw_Bin_Sort_Item* pArrHwBins, int nArrSize, const char* pszBinResult )
{
	CHw_Bin_Sort_Item* arrSetplates_Loc = new CHw_Bin_Sort_Item[ 10 ];
	int nSelectCount = 0;
	int nConformCount = 0;

	//sw bin이 동일한 setplate 목록을 추려본다.
	for( int nIdx = 0; nIdx < g_BinCategory.nIndexCount; nIdx++ )
	{
		ST_CATE_SUB_INFO* pNow = &g_BinCategory.aCateInfo[ nIdx ];
		int cmp = strcmp( pszBinResult, pNow->szSWBin );
		if( cmp == 0 ) {
			int nHwBinCode = atoi( pNow->szHWBin );
			eDeviceDataStage eSetplateNo;
			switch( nHwBinCode )
			{
			case 1: { eSetplateNo = eDeviceStage_STACKER_CONV_4; } break;
			case 2: { eSetplateNo = eDeviceStage_STACKER_CONV_5; } break;
			case 3: { eSetplateNo = eDeviceStage_STACKER_CONV_6; } break;
			case 4: { eSetplateNo = eDeviceStage_STACKER_CONV_7; } break;
			default: ASSERT( 0 ); break;
			}
			arrSetplates_Loc[ nSelectCount ].eHwBin = eSetplateNo;
			strcpy_s( arrSetplates_Loc[ nSelectCount ].szSwBin, sizeof( arrSetplates_Loc[ nSelectCount ].szSwBin ), pszBinResult );
			arrSetplates_Loc[ nSelectCount ].dLoadingTime = g_DMCont.m_dmConveyor.GD( DDM6_TRAY_LOAD_TIME3 + ( nHwBinCode - 1 ) );
			nSelectCount++;
		}
	}

	//filtering available setplate
	for( int i = 0; i < nSelectCount; i++ ) {
		//준비가 안된 setplate는 skip
		int nSetplateObj_Index = (/*_eSetplateIdx::*/eSTACKER_01 + ( arrSetplates_Loc[ i ].eHwBin - eDeviceStage_STACKER_CONV_1 ) );
		eStacker_Conveyor_State state = g_TaskAutoStacker[ nSetplateObj_Index ].GetConveyorState();
		if( state == PickEnable_Device || state == PlaceEnable_Device ) {
			pArrHwBins[ nConformCount++ ] = arrSetplates_Loc[ i ];
		}
		else {
			continue;
		}
	}

	delete[] arrSetplates_Loc;

	return nConformCount;
}

int CTaskAutoTrayLoadPP::convert_ErrDisrip_to_hw_bin_multi( CHw_Bin_Sort_Item* pArrHwBins, int nArrSize, const char* pszErrStr )
{
	CHw_Bin_Sort_Item* arrSetplates_Loc = new CHw_Bin_Sort_Item[ 10 ];
	int nSelectCount = 0;
	int nConformCount = 0;

	//sw bin이 동일한 setplate 목록을 추려본다.
	for( int nIdx = 0; nIdx < g_BinCategory.nIndexCount; nIdx++ )
	{
		ST_CATE_SUB_INFO* pNow = &g_BinCategory.aCateInfo[ nIdx ];
		int cmp = strcmp( pszErrStr, pNow->szDesc );
		if( cmp == 0 ) {
			int nHwBinCode = atoi( pNow->szHWBin );
			eDeviceDataStage eSetplateNo;
			switch( nHwBinCode )
			{
				/*case 1: { eSetplateNo = / *eDeviceDataStage::* /eDeviceStage_STACKER_CONV_4; } break;*/
			case 1: { eSetplateNo = /*eDeviceDataStage::*/eDeviceStage_STACKER_CONV_5; } break;
			case 2: { eSetplateNo = /*eDeviceDataStage::*/eDeviceStage_STACKER_CONV_6; } break;
			case 3: { eSetplateNo = /*eDeviceDataStage::*/eDeviceStage_STACKER_CONV_7; } break;
			default: ASSERT( 0 ); break;
			}
			arrSetplates_Loc[ nSelectCount ].eHwBin = eSetplateNo;
			strcpy_s( arrSetplates_Loc[ nSelectCount ].szErrStr, sizeof( arrSetplates_Loc[ nSelectCount ].szErrStr ), pszErrStr );
			arrSetplates_Loc[ nSelectCount ].dLoadingTime = g_DMCont.m_dmConveyor.GD( DDM6_TRAY_LOAD_TIME3 + ( nHwBinCode - 1 ) );
			nSelectCount++;
		}
	}

	//filtering available setplate
	for( int i = 0; i < nSelectCount; i++ ) {
		//준비가 안된 setplate는 skip
		int nSetplateObj_Index = (/*_eSetplateIdx::*/eSTACKER_01 + ( arrSetplates_Loc[ i ].eHwBin - eDeviceStage_STACKER_CONV_1 ) );
		auto conv_state = g_TaskAutoStacker[ nSetplateObj_Index ].GetConveyorState();
		if( conv_state != PlaceEnable_Device) {
			continue;
		}
		else {
			pArrHwBins[ nConformCount++ ] = arrSetplates_Loc[ i ];
		}
	}

	delete[] arrSetplates_Loc;

	return nConformCount;
}

int CTaskAutoTrayLoadPP::FindPickFromSetplate(
	_eTrayPPIdx TrayPPIdx,
	int nWorkPlace,
	int nArrDvcData_VirtualCTray[VIRTUAL_POCKET_Y_MAX][VIRTUAL_POCKET_X_MAX],
	int nPickerDvcPattern[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X],
	BOOL bPickPlace,
	int* n_Xpos, int* n_YPos,
	int nArrHandWork[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X],
	int nWorkDir,
	BOOL bChkQaSampleCnt)
{
	CTaskTrayBasePP* TaskTrayPP = nullptr;
	switch (TrayPPIdx) {
	case _eTrayPPIdx::eTrayPP_1_LD: { TaskTrayPP = &g_TaskTrayLoadPP; }break;
	case _eTrayPPIdx::eTrayPP_2_UD: { TaskTrayPP = &g_TaskTrayUnloadPP; }break;
	}

	int nSearchY = 0, nSearchX = 0;
	int nSearchHandY = 0, nSearchHandX = 0;

	/*DEF_WORK_FREE or DEF_WORK_RESERVED*/
	int nArrHandWorkSource[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = { DEF_WORK_FREE, };
	int nArrHandWorkCopy[MAX_TRAY_PP_CNT_Y][MAX_TRAY_PP_CNT_X] = { DEF_WORK_FREE, };

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

	if (bPickPlace == DEF_PICK) {	// Pick 일 때 조건
		nCheckCTrayData = DEF_EXIST;
		nHadData = DEF_NOT_EXIST;
	}
	else {							// Place 일 때 조건		
		nCheckCTrayData = DEF_NOT_EXIST;
		nHadData = DEF_EXIST;
	}

	// [1x-pitch]
	int nX_PitchType = 1;
	int nY_PitchType = 1;

	// tray hand get load device count
	int nCntLoadPicker = TaskTrayPP->GetTrayPpDvcCnt();

	int nQAStatus = g_TaskSystemCtrl.GetQAStatus();
	BOOL bAutoQAUse = g_TaskSystemCtrl.GetAutoQAUse();
	if (bAutoQAUse == TRUE && g_TaskSystemCtrl.IsOneCycleModeOn() == false && nQAStatus == eQAStatus::eQAStatus_Load || nQAStatus == eQAStatus::eQAStatus_LoadEnd)
	{
		bChkQaSampleCnt = TRUE;   //Q/A Mode Load Status
	}
	
	int nQaSampleCnt = g_TaskSystemCtrl.GetQASampleCount(); //Q/A Mode 설정 수량

	int nLotLoadCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt);  //투입 된 수량
	int nLoadTrayPPCnt = g_TaskTrayLoadPP.GetTrayPpLoadDvcCnt();   // Hand에 Pick 되어 있는 수량

	int nLoadDeviceCnt = g_DMCont.m_dmEQP.GN(NDM0_QaSampleLoadCnt) + nLoadTrayPPCnt;

	switch (nWorkPlace)
	{
		//case eDeviceStage_STACKER_CONV_2:
	case eDeviceStage_STACKER_CONV_3:
	case eDeviceStage_STACKER_CONV_4:
	case eDeviceStage_STACKER_CONV_5:
	case eDeviceStage_STACKER_CONV_6:
	case eDeviceStage_STACKER_CONV_7:
	case eDeviceStage_TRAY_FEEDER:
	{
		// [2x-pitch]
		if (g_DMCont.m_dmTrayPP.GN(NDM5_Tray_Pitch_X) < DEF_TRAY_PP_MAX_X_PITCH_NARROW) {
			nX_PitchType = 2;
		}
		if (g_DMCont.m_dmTrayPP.GN(NDM5_Tray_Pitch_Y) < DEF_TRAY_PP_MAX_Y_PITCH_NARROW) {
			nY_PitchType = 2;
		}

		nPocketCntX = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
		nPocketCntY = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);

		nContainerPitch_X_Cnt = nX_PitchType; // x pitch
		nContainerPitch_Y_Cnt = nY_PitchType; // y pitch

		//GetDeviceExist_CTrayHand(bPickPlace, (eDeviceDataStage)nWorkPlace, (int*)nPickerDvcPattern, MAX_TRAY_PP_CNT_X, MAX_TRAY_PP_CNT_Y);
	}break;
	case eDeviceStage_TRAY_BUFF:
	{
		nPocketCntX = 4;
		nPocketCntY = 4;

		nContainerPitch_X_Cnt = nX_PitchType; // x pitch
		nContainerPitch_Y_Cnt = nY_PitchType; // y pitch
	}break;
	default:
		ASSERT(false);
		break;
	}

	// CTray에서 첫 디바이스가 발견되는 Y+1 줄까지만 검색 하도록!
	int nSearchY_Limit = (VIRTUAL_POCKET_Y_MAX - 1);
	BOOL bFindDevice = FALSE;

	if (nWorkDir == CTaskAutoTrayLoadPP::eRobotWorkDirTopLeft)
	{
		for (nSearchY = VIRTUAL_POCKET_Y_START; nSearchY < (VIRTUAL_POCKET_Y_START + nPocketCntY); nSearchY++)
		{
			for (nSearchX = VIRTUAL_POCKET_X_START; nSearchX < (VIRTUAL_POCKET_X_START + nPocketCntX); nSearchX++)
			{
				if (nArrDvcData_VirtualCTray[nSearchY][nSearchX] == nCheckCTrayData) {
					//nSearchY_Limit = nSearchY + (nContainerPitch_Y_Cnt - 1);
					nSearchY_Limit = nSearchY + (nContainerPitch_Y_Cnt - 1);
					bFindDevice = TRUE;
					break;
				}
			}
			if (bFindDevice == TRUE) { break; }
		}
	}
	else
	{
		for (nSearchY = VIRTUAL_POCKET_Y_START + nPocketCntY; nSearchY > VIRTUAL_POCKET_Y_START-(MAX_TRAY_PP_CNT_Y - 1) * nContainerPitch_Y_Cnt; nSearchY--)
		{
			for (nSearchX = VIRTUAL_POCKET_X_START; nSearchX < (VIRTUAL_POCKET_X_START + nPocketCntX); nSearchX++)
			{
				if (nArrDvcData_VirtualCTray[nSearchY][nSearchX] == nCheckCTrayData) {
					// 					int nGapIdx = (nContainerPitch_Y_Cnt != 1) ? 1 : 2;
					// 					nSearchY_Limit = nSearchY + (nContainerPitch_Y_Cnt + 1);
//					nSearchY_Limit = nSearchY - nContainerPitch_Y_Cnt;
					nSearchY_Limit = nSearchY - ((nContainerPitch_Y_Cnt + 1) + 2);
					bFindDevice = TRUE;
					break;
				}
			}
			if (bFindDevice == TRUE) { break; }
		}
	}


	if (nWorkDir == CTaskAutoTrayLoadPP::eRobotWorkDirTopLeft)
	{
		for (nSearchY = VIRTUAL_POCKET_Y_START - (MAX_TRAY_PP_CNT_Y - 1) * nContainerPitch_Y_Cnt; nSearchY < VIRTUAL_POCKET_Y_START + nPocketCntY; nSearchY++)
		{
			if (nSearchY >= VIRTUAL_POCKET_Y_START && nSearchY > nSearchY_Limit /*+1*/) { break; } // CTray에서 첫 디바이스가 발견되는 Y+1 줄까지만 검색 하도록!

			for (nSearchX = VIRTUAL_POCKET_X_START - (MAX_TRAY_PP_CNT_X - 1) * nContainerPitch_X_Cnt; nSearchX < VIRTUAL_POCKET_X_START + nPocketCntX; nSearchX++)
			{
				nCurrentCnt = 0;

				memset(nArrHandWorkSource, 0, sizeof(nArrHandWorkSource));

				for (nSearchHandY = 0; nSearchHandY < MAX_TRAY_PP_CNT_Y; nSearchHandY++)
				{
					for (nSearchHandX = 0; nSearchHandX < MAX_TRAY_PP_CNT_X; nSearchHandX++)
					{
						int nYPocket = 0, nXPocket = 0;
						nYPocket = nSearchY + nSearchHandY * nContainerPitch_Y_Cnt;
						nXPocket = nSearchX + nSearchHandX * nContainerPitch_X_Cnt;

						if (nArrDvcData_VirtualCTray[nYPocket][nXPocket] == nCheckCTrayData
							&& nPickerDvcPattern[nSearchHandY][nSearchHandX] == nHadData)
						{
							nArrHandWorkSource[nSearchHandY][nSearchHandX] = DEF_WORK_RESERVED;
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

				BOOL bLimitCheckOK = GetAreaProblem(nSearchX - VIRTUAL_POCKET_X_START, nSearchY - VIRTUAL_POCKET_Y_START, (eDeviceDataStage)nWorkPlace, nArrHandWorkSource); // todo slt : 영역 점검을 위한 함수 구현 필요.
				if (bLimitCheckOK == TRUE)
				{
					if ((nCurrentCnt > nTotalPossibleCnt && bChkQaSampleCnt == FALSE)
						|| (bChkQaSampleCnt == TRUE && nCurrentCnt > nTotalPossibleCnt && nQaSampleCnt >= nLoadDeviceCnt + nCurrentCnt))
					{
						//작업해라.
						nTotalPossibleCnt = nCurrentCnt;
						memcpy_s(nArrHandWorkCopy, sizeof(nArrHandWorkCopy), nArrHandWorkSource, sizeof(nArrHandWorkCopy));
						nCXPosition = nSearchX;
						nCYPosition = nSearchY;
					}
				}
			}
		}

		memcpy_s(nArrHandWork, sizeof(nArrHandWorkCopy), nArrHandWorkCopy, sizeof(nArrHandWorkCopy));

		//0번 피커부터 할당되지 않은 포켓은 트림한다.
		int nPockOffsetX = 0;
		int nPockOffsetY = 1;

		//X방향 첫 할당 위치가 X옵셋
		for (; nPockOffsetX < MAX_TRAY_PP_CNT_X; nPockOffsetX++)
		{
			if (nArrHandWork[0][nPockOffsetX] != DEF_WORK_FREE) break;
			if (nArrHandWork[1][nPockOffsetX] != DEF_WORK_FREE) break;
		}



		//X방향 첫 할당 위치가 1개라도 있으면 Y옵셋은 0
		for (int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++)
		{
			/* Rear로 집도록 변경 한 것은 기존 Front에서 찾은 놈이기 때문에 nPockOffsetY는 0으로 고정  */
			if (nArrHandWork[0][nX] != DEF_WORK_FREE /*|| nCYPosition > nBaseRow*/) {
				nPockOffsetY = 0;
				break;
			}
		}

		/*
		*	스케쥴을 생성한 디바이스는 가상 버퍼에서 지우고 다음 pick 스케줄 생성을 시도한다.
		*/

		for (int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++)
		{
			for (int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++)
			{
				if (nArrHandWork[nY][nX] != 0) {
					int nVirX = nCXPosition + (nX * nContainerPitch_X_Cnt);
					int nVirY = nCYPosition + (nY * nContainerPitch_Y_Cnt);

					if (bPickPlace == DEF_PICK) {
						nArrDvcData_VirtualCTray[nVirY][nVirX] = DEF_NOT_EXIST;
						nPickerDvcPattern[nY][nX] = DEF_EXIST;
					}
					else {
						nArrDvcData_VirtualCTray[nVirY][nVirX] = DEF_EXIST;
						nPickerDvcPattern[nY][nX] = DEF_NOT_EXIST;
					}
				}
			}
		}

		nCXPosition -= VIRTUAL_POCKET_X_START;
		nCYPosition -= VIRTUAL_POCKET_Y_START;

		g_TaskTrayFeeder.SetTrayDeviceExistRowStep(nCYPosition, nPockOffsetY, nWorkDir); //실제 Pocket Idx


		*n_Xpos = nCXPosition + nPockOffsetX * nContainerPitch_X_Cnt; //0 base //실제 Pick 하는 첫번째 pocket
		*n_YPos = nCYPosition + nPockOffsetY * nContainerPitch_Y_Cnt; //0 base

		return nTotalPossibleCnt;
	}
	else
	{
		int tray_y_cnt = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);
		int nBaseRow = (tray_y_cnt - 1) - nY_PitchType + VIRTUAL_POCKET_Y_START;

		int nSearchYStart = VIRTUAL_POCKET_Y_START + tray_y_cnt;
		int nSearchYEnd = VIRTUAL_POCKET_Y_START - (MAX_TRAY_PP_CNT_Y - 1) * nContainerPitch_Y_Cnt;

		for (nSearchY = VIRTUAL_POCKET_Y_START + tray_y_cnt; nSearchY >= VIRTUAL_POCKET_Y_START - (MAX_TRAY_PP_CNT_Y - 1) * nContainerPitch_Y_Cnt; nSearchY--)
		{
			if (nSearchY < nSearchY_Limit) { break; }
			for (nSearchX = VIRTUAL_POCKET_X_START - (MAX_TRAY_PP_CNT_X - 1) * nContainerPitch_X_Cnt; nSearchX < VIRTUAL_POCKET_X_START + nPocketCntX; nSearchX++)
			{
				nCurrentCnt = 0;

				memset(nArrHandWorkSource, 0, sizeof(nArrHandWorkSource));

				for (nSearchHandY = 0; nSearchHandY < MAX_TRAY_PP_CNT_Y; nSearchHandY++) {
					for (nSearchHandX = 0; nSearchHandX < MAX_TRAY_PP_CNT_X; nSearchHandX++) {
						int nYPocket = 0, nXPocket = 0;
						nYPocket = nSearchY + nSearchHandY * nContainerPitch_Y_Cnt;
						nXPocket = nSearchX + nSearchHandX * nContainerPitch_X_Cnt;

						if (nArrDvcData_VirtualCTray[nYPocket][nXPocket] == nCheckCTrayData
							&&  nPickerDvcPattern[nSearchHandY][nSearchHandX] == nHadData)
						{
							// Tray FEEDER Step 0번 일때, Tray PP Rear Picker로 Pick 못하게 변경
// 							if (nSearchY == (VIRTUAL_POCKET_Y_START + tray_y_cnt -1) && nArrHandWorkSource[0][nSearchHandX] == DEF_WORK_FREE)
// 								continue;
							
							
							if (nSearchY > nBaseRow)
							{
								ST_DD_DEVICE stDvcData;
								g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TRAY_PP_1 + TrayPPIdx, nSearchHandX, 1/*nY*/, &stDvcData);
								if (stDvcData.sExist) {
									continue;
								}
								else {
									//첫번째 2열은 Front로만 탐색 가능
									if (nSearchHandY == 0) {
										//rear로 집도록 변경
										nArrHandWorkSource[nSearchHandY + 1][nSearchHandX] = DEF_WORK_RESERVED;
									}
								}
							}
							else
							{
								nArrHandWorkSource[nSearchHandY][nSearchHandX] = DEF_WORK_RESERVED;
							}

							//nArrHandWorkSource[nSearchHandY][nSearchHandX] = DEF_WORK_RESERVED;
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

				BOOL bLimitCheckOK = GetAreaProblem(nSearchX - VIRTUAL_POCKET_X_START, nSearchY - VIRTUAL_POCKET_Y_START, (eDeviceDataStage)nWorkPlace, nArrHandWorkSource); // todo slt : 영역 점검을 위한 함수 구현 필요.
				if (bLimitCheckOK == TRUE)
				{
// 					if ((nCurrentCnt > nTotalPossibleCnt)
// 						|| (nCurrentCnt > nTotalPossibleCnt && nQaSampleCnt >= nLoadDeviceCnt + nCurrentCnt))
					if ((nCurrentCnt > nTotalPossibleCnt && bChkQaSampleCnt == FALSE)
						|| (bChkQaSampleCnt == TRUE && nCurrentCnt > nTotalPossibleCnt && nQaSampleCnt >= nLoadDeviceCnt + nCurrentCnt))
					{
						//작업해라.
						nTotalPossibleCnt = nCurrentCnt;
						memcpy_s(nArrHandWorkCopy, sizeof(nArrHandWorkCopy), nArrHandWorkSource, sizeof(nArrHandWorkCopy));
						nCXPosition = nSearchX; //실제 찾은 Pocket Position
						nCYPosition = nSearchY;
					}
				}

			}
		}

		memcpy_s(nArrHandWork, sizeof(nArrHandWorkCopy), nArrHandWorkCopy, sizeof(nArrHandWorkCopy));

		//0번 피커부터 할당되지 않은 포켓은 트림한다.
		int nPockOffsetX = 0;
		int nPockOffsetY = 1;

		//X방향 첫 할당 위치가 X옵셋
		for (; nPockOffsetX < MAX_TRAY_PP_CNT_X; nPockOffsetX++)
		{
			if (nArrHandWork[0][nPockOffsetX] != DEF_WORK_FREE) break;
			if (nArrHandWork[1][nPockOffsetX] != DEF_WORK_FREE) break;
		}


		//X방향 첫 할당 위치가 1개라도 있으면 Y옵셋은 0
		for (int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++)
		{
			/* Rear로 집도록 변경 한 것은 기존 Front에서 찾은 놈이기 때문에 nPockOffsetY는 0으로 고정  */
			if (nArrHandWork[0][nX] != DEF_WORK_FREE || nCYPosition > nBaseRow) {
				nPockOffsetY = 0;
				break;
			}
 		}

		/*
		*	스케쥴을 생성한 디바이스는 가상 버퍼에서 지우고 다음 pick 스케줄 생성을 시도한다.
		*/

		for (int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++)
		{
			for (int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++)
			{
				if (nArrHandWork[nY][nX] != 0) {
					int nVirX = nCXPosition + (nX * nContainerPitch_X_Cnt);
					int nVirY = nCYPosition + (nY * nContainerPitch_Y_Cnt);

					if (bPickPlace == DEF_PICK) {
						nArrDvcData_VirtualCTray[nVirY][nVirX] = DEF_NOT_EXIST;
						nPickerDvcPattern[nY][nX] = DEF_EXIST;
					}
					else {
						nArrDvcData_VirtualCTray[nVirY][nVirX] = DEF_EXIST;
						nPickerDvcPattern[nY][nX] = DEF_NOT_EXIST;
					}
				}
			}
		}

		nCXPosition -= VIRTUAL_POCKET_X_START;
		nCYPosition -= VIRTUAL_POCKET_Y_START;

/*		if(nCYPosition >= 0)*/
//		if (nTotalPossibleCnt > 0)
//		{
		g_TaskTrayFeeder.SetTrayDeviceExistRowStep(nCYPosition + 1, nPockOffsetY, nWorkDir); //실제 Pocket Idx
//		}
		
//		g_TaskTrayFeeder.SetTrayDeviceExistRowStep(nCYPosition + 1, nPockOffsetY); //실제 Pocket Idx


		*n_Xpos = nCXPosition + nPockOffsetX * nContainerPitch_X_Cnt; //0 base //실제 Pick 하는 첫번째 pocket
		*n_YPos = nCYPosition + nPockOffsetY * nContainerPitch_Y_Cnt; //0 base


		//Base Row 보다 아래 있는 열은 Rear Pick로 집도록 변경.
// 		int nBaseRow = tray_y_cnt/*Division*/ - nY_PitchType/*1 or 2Pitch*/; /*+ VIRTUAL_POCKET_Y_START/ *30 buffer base* /;*/
// 		if (nCYPosition + 1 > nBaseRow) //nBaseRow == 30 / Pocket Y : 31, 32
// 		{
// 			for (int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++)
// 			{
// 				if (nArrHandWork[0][nX] != DEF_WORK_FREE) {
// 					ST_DD_DEVICE stDvcData;
// 					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TRAY_PP_1 + TrayPPIdx, nX, 1/*nY*/, &stDvcData);
// 					if (stDvcData.sExist) {
// 						nArrHandWork[0][nX] = DEF_WORK_FREE;
// 						nPickerDvcPattern[0][nX] = DEF_EXIST;
// 						nTotalPossibleCnt--;
// 						continue;
// 					}
// 
// 					nArrHandWork[0][nX] = DEF_WORK_FREE;
// 					nArrHandWork[1][nX] = DEF_WORK_RESERVED;
// 				}
// 
// 				if (nPickerDvcPattern[0][nX] == DEF_EXIST)
// 				{
// 					nPickerDvcPattern[0][nX] = DEF_NOT_EXIST;
// 					nPickerDvcPattern[1][nX] = DEF_EXIST;
// 				}
// 			}
//  		}

		return nTotalPossibleCnt;
	}
}

// 		int nPocketStartY = VIRTUAL_POCKET_Y_START + nDeviceExistRow;
// 		for (nSearchX = VIRTUAL_POCKET_X_START - (MAX_TRAY_PP_CNT_X - 1) * nContainerPitch_X_Cnt; nSearchX < VIRTUAL_POCKET_X_START + nPocketCntX; nSearchX++)
// 		{
// 			nCurrentCnt = 0;
// 
// 			memset(nArrHandWorkSource, 0, sizeof(nArrHandWorkSource));
// 		}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 		
// 		for( nSearchY = VIRTUAL_POCKET_Y_START +  nPocketCntY; nSearchY >= VIRTUAL_POCKET_Y_START - ( MAX_TRAY_PP_CNT_Y - 1 ) * nContainerPitch_Y_Cnt; nSearchY-- )
// 		{
// 			if( nSearchY > nSearchY_Limit ) { break; } // CTray에서 첫 디바이스가 발견되는 Y+1 줄까지만 검색 하도록!
// 
// 			for( nSearchX = VIRTUAL_POCKET_X_START - ( MAX_TRAY_PP_CNT_X - 1 ) * nContainerPitch_X_Cnt; nSearchX < VIRTUAL_POCKET_X_START + nPocketCntX; nSearchX++ )
// 			{
// 				nCurrentCnt = 0;
// 
// 				memset( nArrHandWorkSource, 0, sizeof( nArrHandWorkSource ) );
// 
// 				for (nSearchHandY = 0; nSearchHandY < MAX_TRAY_PP_CNT_Y; nSearchHandY++)
// 				{
// 					for( nSearchHandX = 0; nSearchHandX < MAX_TRAY_PP_CNT_X; nSearchHandX++ )
// 					{
// 						int nYPocket = 0, nXPocket = 0;
// 						nYPocket = nSearchY + nSearchHandY * nContainerPitch_Y_Cnt;
// 						nXPocket = nSearchX + nSearchHandX * nContainerPitch_X_Cnt;
// 
// 						if( nArrDvcData_VirtualCTray[ nYPocket ][ nXPocket ] == nCheckCTrayData
// 							&& nPickerDvcPattern[ nSearchHandY ][ nSearchHandX ] == nHadData )
// 						{
// 							nArrHandWorkSource[ nSearchHandY ][ nSearchHandX ] = DEF_WORK_RESERVED;
// 							nCurrentCnt++;
// 						}
// 					}
// 				}
// 
// 				double dTeaching_X = 0.0; // Tray PP의 해당 Stage의 (0,0) 티칭값Y um
// 				double dTeaching_Y = 0.0; // Tray PP의 해당 Stage의 (0,0) 티칭값Y um
// 				double dCTrayXpitch = 0.0;
// 				double dCTrayYpitch = 0.0;
// 				double dLimitCheckValue_X = 0.0; // 점검을 위한 계산된 값.
// 				double dLimitCheckValue_Y = 0.0; // 점검을 위한 계산된 값.
// 
// 				BOOL bLimitCheckOK = GetAreaProblem(nSearchX - VIRTUAL_POCKET_X_START, nSearchY - VIRTUAL_POCKET_Y_START, ( eDeviceDataStage )nWorkPlace, nArrHandWorkSource ); // todo slt : 영역 점검을 위한 함수 구현 필요.
// 				if( bLimitCheckOK == TRUE )
// 				{
// 					if( ( nCurrentCnt > nTotalPossibleCnt)
// 						|| (nCurrentCnt > nTotalPossibleCnt && nQaSampleCnt >= nLoadDeviceCnt + nCurrentCnt ) )
// 					{
// 						//작업해라.
// 						nTotalPossibleCnt = nCurrentCnt;
// 						memcpy_s( nArrHandWorkCopy, sizeof( nArrHandWorkCopy ), nArrHandWorkSource, sizeof( nArrHandWorkCopy ) );
// 						nCXPosition = nSearchX;
// 						nCYPosition = nSearchY;
// 					}
// 				}
// 			}
// 		}
// 	}
// 
// 	memcpy_s( nArrHandWork, sizeof( nArrHandWorkCopy ), nArrHandWorkCopy, sizeof( nArrHandWorkCopy ) );
// 
// 	//0번 피커부터 할당되지 않은 포켓은 트림한다.
// 	int nPockOffsetX = 0;
// 	int nPockOffsetY = 1;
// 
// 	//X방향 첫 할당 위치가 X옵셋
// 	for( ; nPockOffsetX < MAX_TRAY_PP_CNT_X; nPockOffsetX++ )
// 	{
// 		if( nArrHandWork[ 0 ][ nPockOffsetX ] != DEF_WORK_FREE ) break;
// 		if( nArrHandWork[ 1 ][ nPockOffsetX ] != DEF_WORK_FREE ) break;
// 	}
// 	//X방향 첫 할당 위치가 1개라도 있으면 Y옵셋은 0
// 	for( int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++ )
// 	{
// 		if( nArrHandWork[ 0 ][ nX ] != DEF_WORK_FREE ) {
// 			nPockOffsetY = 0;
// 			break;
// 		}
// 	}
// 
// 	/*
// 	*	스케쥴을 생성한 디바이스는 가상 버퍼에서 지우고 다음 pick 스케줄 생성을 시도한다.
// 	*/
// 
// 	for( int nY = 0; nY < MAX_TRAY_PP_CNT_Y; nY++ )
// 	{
// 		for( int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++ )
// 		{
// 			if( nArrHandWork[ nY ][ nX ] != 0 ) {
// 				int nVirX = nCXPosition + ( nX * nContainerPitch_X_Cnt );
// 				int nVirY = nCYPosition + ( nY * nContainerPitch_Y_Cnt );
// 
// 				if( bPickPlace == DEF_PICK ) {
// 					nArrDvcData_VirtualCTray[ nVirY ][ nVirX ] = DEF_NOT_EXIST;
// 					nPickerDvcPattern[ nY ][ nX ] = DEF_EXIST;
// 				}
// 				else {
// 					nArrDvcData_VirtualCTray[ nVirY ][ nVirX ] = DEF_EXIST;
// 					nPickerDvcPattern[ nY ][ nX ] = DEF_NOT_EXIST;
// 				}
// 			}
// 		}
// 	}
// 
// 	nCXPosition -= VIRTUAL_POCKET_X_START;
// 	nCYPosition -= VIRTUAL_POCKET_Y_START;
// 
// 	*n_Xpos = nCXPosition + nPockOffsetX * nContainerPitch_X_Cnt;
// 	*n_YPos = nCYPosition + nPockOffsetY * nContainerPitch_Y_Cnt;
// 
// 	return nTotalPossibleCnt;
	
//}

int CTaskAutoTrayLoadPP::FillDvc_VirtualTable( eDeviceDataStage nDvcStageIdx, int* pnBuffer, int nBufSizeX, int nBufSizeY, int nPickPlace )
{
	int nPocketCount_X = 0;
	int nPocketCount_Y = 0;

	BOOL bLoadTable = FALSE;
	switch( nDvcStageIdx )
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
			nPocketCount_X = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Div_X );
			nPocketCount_Y = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Div_Y );
			bLoadTable = TRUE;
		}break;
	case eDeviceStage_TRAY_BUFF:
		{
			nPocketCount_X = 4;
			nPocketCount_Y = 4;
		}break;
	default:
		ASSERT( false );
		break;
	}

	for( int nY = 0; nY < nPocketCount_Y; nY++ )
	{
		for( int nX = 0; nX < nPocketCount_X; nX++ )
		{
			int nBuffer_Index = ( nX + VIRTUAL_POCKET_X_START ) + ( ( nY + VIRTUAL_POCKET_Y_START ) * nBufSizeX );

			// Load Table의 1,3열 LD, 2,4열 Buffer이므로 Buffer부분은 작업 못하도록 해야한다.
			if( bLoadTable == TRUE )
			{
				char setting = g_TaskLoadTbl[ nDvcStageIdx - eDeviceStage_LD_TBL_1 ].m_LoadTbl_pocket_setting[ nY ][ nX ];			
				if (setting == 'B')
				{
					pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
					continue;
				}

				//상태 반대로 해줘야 함.
				if( nPickPlace == DEF_PICK && setting == 'L' ) {
					pnBuffer[ nBuffer_Index ] = DEF_NOT_EXIST;
					continue;
				}

				if( nPickPlace == DEF_PLACE && setting == 'U' ) {
					pnBuffer[ nBuffer_Index ] = DEF_EXIST;
					continue;
				}
			}

			int nExist = g_DMCont.m_dmDevice.GetDeviceExist( nDvcStageIdx, nX, nY );
			if( nExist == DEF_EXIST ) pnBuffer[ nBuffer_Index ] = DEF_EXIST;
			else                      pnBuffer[ nBuffer_Index ] = DEF_NOT_EXIST;
		}
	}

	return ERR_NO_ERROR;
}

int CTaskAutoTrayLoadPP::_MakeSchedule_TrayPp_Place_LoadTable( _eTrayPPIdx TrayPPIdx, eLocIdxTrayPP nIdxLoadTbl )
{
	int nSchdCount = m_listTrayPpSchedule.size();

	switch( nIdxLoadTbl )
	{
	case eLocIdxTrayPP::LOC_IDX_TRAY_PP_TBL_1:
	case eLocIdxTrayPP::LOC_IDX_TRAY_PP_TBL_2:
	case eLocIdxTrayPP::LOC_IDX_TRAY_PP_TBL_3:
	case eLocIdxTrayPP::LOC_IDX_TRAY_PP_TBL_4:
		{
			SetNewTrayPP_Schedule(DEF_PLACE, nIdxLoadTbl, _eUsePicker_Load);
/*			SetNewTrayPP_Schedule( TrayPPIdx, DEF_PLACE, nIdxLoadTbl, _eUsePicker_Load );*/

			//새롭게 등록된 스케줄 횟수 리턴
			return ( m_listTrayPpSchedule.size() - nSchdCount );
		}break;

	default: ASSERT( false ); return ERR_UNKNOWN;
	}
}

// void CTaskAutoTrayLoadPP::SetNewTrayPP_Schedule( _eTrayPPIdx TrayPPIdx, int nPickPlace, eLocIdxTrayPP nTargetLocIdx, _eUsePicker _picker_type )
// {
// 	CTaskTrayBasePP* TaskTrayPP = nullptr;
// 	switch( TrayPPIdx ) {
// 	case _eTrayPPIdx::eTrayPP_1_LD: { TaskTrayPP = &g_TaskTrayLoadPP; }break;
// 	case _eTrayPPIdx::eTrayPP_2_UD: { TaskTrayPP = &g_TaskTrayUnloadPP; }break;
// 	}
// 
// 	bool Available = false;
// 	int nPickerDvcPattern[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ] = { 0, };
// 
// 	eDeviceDataStage stage;
// 	switch( nTargetLocIdx ) {
// 	case LOC_IDX_TRAY_PP_CONV_1:
// 	case LOC_IDX_TRAY_PP_CONV_2:
// 	case LOC_IDX_TRAY_PP_CONV_3:
// 	case LOC_IDX_TRAY_PP_CONV_4:
// 	case LOC_IDX_TRAY_PP_CONV_5:
// 	case LOC_IDX_TRAY_PP_CONV_6:
// 	case LOC_IDX_TRAY_PP_CONV_7:
// 		{
// 			stage = ( eDeviceDataStage )( eDeviceStage_STACKER_CONV_1 + ( nTargetLocIdx - LOC_IDX_TRAY_PP_CONV_1 ) );
// 
// 			for( int nPkrY = 0; nPkrY < MAX_TRAY_PP_CNT_Y; nPkrY++ )
// 			{
// 				for( int nPkrX = 0; nPkrX < MAX_TRAY_PP_CNT_X; nPkrX++ )
// 				{
// 					char setting = TaskTrayPP->m_TrayPP_picker_setting[ nPkrY ][ nPkrX ];
// 					int device_exist = g_DMCont.m_dmDevice.GetDeviceExist( eDeviceStage_TRAY_PP_1 + TrayPPIdx, nPkrX, nPkrY );
// 					switch( nPickPlace ) {
// 					case DEF_PICK:
// 						{
// 							//if( setting == 'L' ) {
// 							if( setting == _picker_type == _eUsePicker_Load ? 'L' : 'U' ) {
// 								nPickerDvcPattern[ nPkrY ][ nPkrX ] = device_exist;
// 								if( device_exist == DEF_NOT_EXIST ) {
// 									Available = true;
// 								}
// 							}
// 							else {
// 								// pick 진행 시 동작 하지 말아야 하는 picker 
// 								nPickerDvcPattern[ nPkrY ][ nPkrX ] = DEF_EXIST;
// 							}
// 						}break;
// 					case DEF_PLACE:
// 						{
// 							//if( setting == 'U' ) {
// 							if( setting == _picker_type == _eUsePicker_Load ? 'L' : 'U' ) {
// 								nPickerDvcPattern[ nPkrY ][ nPkrX ] = device_exist;
// 								if( device_exist == DEF_EXIST ) {
// 									Available = true;
// 								}
// 							}
// 							else {
// 								// place 진행 시 동작 하지 말아야 하는 picker 
// 								nPickerDvcPattern[ nPkrY ][ nPkrX ] = DEF_NOT_EXIST;
// 							}
// 						}break;
// 					}
// 				}
// 			}
// 
// 		}break;
// 	case LOC_IDX_TRAY_PP_TBL_1:
// 	case LOC_IDX_TRAY_PP_TBL_2:
// 	case LOC_IDX_TRAY_PP_TBL_3:
// 	case LOC_IDX_TRAY_PP_TBL_4:
// 		{
// 			stage = ( eDeviceDataStage )( eDeviceStage_LD_TBL_1 + ( nTargetLocIdx - LOC_IDX_TRAY_PP_TBL_1 ) );
// 
// 			for( int nPkrY = 0; nPkrY < MAX_TRAY_PP_CNT_Y; nPkrY++ )
// 			{
// 				for( int nPkrX = 0; nPkrX < MAX_TRAY_PP_CNT_X; nPkrX++ )
// 				{
// 					char setting = TaskTrayPP->m_TrayPP_picker_setting[ nPkrY ][ nPkrX ];
// 					int device_exist = g_DMCont.m_dmDevice.GetDeviceExist( eDeviceStage_TRAY_PP_1 + TrayPPIdx, nPkrX, nPkrY );
// 
// 					switch( nPickPlace ) {
// 					case DEF_PICK:
// 						{
// 							//if( setting == 'U' ) {
// 							if( setting == _picker_type == _eUsePicker_Load ? 'L' : 'U' ) {
// 								nPickerDvcPattern[ nPkrY ][ nPkrX ] = device_exist;
// 								if( device_exist == DEF_NOT_EXIST ) {
// 									Available = true;
// 								}
// 							}
// 							else {
// 								// pick 진행 시 동작 하지 말아야 하는 picker 
// 								nPickerDvcPattern[ nPkrY ][ nPkrX ] = DEF_EXIST;
// 							}
// 						}break;
// 					case DEF_PLACE:
// 						{
// 							//if( setting == 'L' ) {
// 							if( setting == _picker_type == _eUsePicker_Load ? 'L' : 'U' ) {
// 								nPickerDvcPattern[ nPkrY ][ nPkrX ] = device_exist;
// 								if( device_exist == DEF_EXIST ) {
// 									Available = true;
// 								}
// 							}
// 							else {
// 								// place 진행 시 동작 하지 말아야 하는 picker 
// 								nPickerDvcPattern[ nPkrY ][ nPkrX ] = DEF_NOT_EXIST;
// 							}
// 						}break;
// 					}
// 				}
// 			}
// 		}break;
// 	default:
// 		ASSERT( false );
// 		break;
// 	}
// 
// 	int nArrVirtualTbl[ VIRTUAL_POCKET_Y_MAX ][ VIRTUAL_POCKET_X_MAX ];
// 	memset( nArrVirtualTbl, nPickPlace == DEF_PICK ? 0x00 : 0xFF, sizeof( nArrVirtualTbl ) );
// 	FillDvc_VirtualTable( stage, ( int* )nArrVirtualTbl, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, nPickPlace );
// 
// 	if( Available ) {
// 		int nUseJobPkr[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ] = { 0, };
// 		int nPockX = -1, nPockY = -1;
// 		int nRetCode = FindPickPlaceFromTable( TrayPPIdx, nTargetLocIdx, nArrVirtualTbl, nPickerDvcPattern, nPickPlace, &nPockX, &nPockY, nUseJobPkr);
// 		if( nRetCode > 0 ) {
// 			std::vector<CPoint> vPkr;
// 			for( int nPickX = 0; nPickX < MAX_TEST_PP_PKR_CNT_X; nPickX++ ) {
// 				for( int nPickY = 0; nPickY < MAX_TEST_PP_PKR_CNT_Y; nPickY++ ) {
// 					if( nUseJobPkr[ nPickY ][ nPickX ] != 0 ) {
// 						vPkr.push_back( CPoint( nPickX, nPickY ) );
// 					}
// 				}
// 			}
// 
// 			//BOOL bGrrDvcSwap = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrDeviceSwapUse);
// 
// 			///////////////////////////////////////////////
// 			//////////GRR 구현 후, 추가 
// 			///////////////////////////////////////////////
// 			if( g_TaskSystemCtrl.GetAutoGrrUse() == TRUE /*&& bGrrDvcSwap == FALSE */ && nPickPlace == DEF_PLACE && ( nTargetLocIdx == LOC_IDX_TRAY_PP_TBL_1 || nTargetLocIdx == LOC_IDX_TRAY_PP_TBL_2 ) )
// 			{
// 				int nCountInsert = g_DMCont.m_dmTestPP.GN( NDM04_SITEGROUP1_INSERTED_DEVICE_CNT + ( nTargetLocIdx - LOC_IDX_TRAY_PP_TBL_1 ) );
// 				int nSetInDvc = g_DMCont.m_dmHandlerOpt.GN( NDM11_MD_AutoGrrUnitCount );
// 
// 				int nModule1On = g_TaskAutoTestPp[ eTestPP_1 ].GetModuleSocketOnCnt( TRUE );
// 				int nModule2On = g_TaskAutoTestPp[ eTestPP_2 ].GetModuleSocketOnCnt( TRUE );
// 				if( nModule1On > 0 && nModule2On > 0 )
// 				{
// 					if( nCountInsert >= nSetInDvc / 2 )
// 					{
// 						return;
// 					}
// 					else if( nCountInsert + ( int )vPkr.size() > nSetInDvc / 2 )
// 					{
// 						for( int i = 0; i < ( nCountInsert + ( int )vPkr.size() - nSetInDvc / 2 ); i++ )
// 							vPkr.pop_back();
// 					}
// 				}
// 				if( vPkr.size() == 0 )
// 					return;
// 			}
// 			else if( g_TaskSystemCtrl.GetAutoLoopUse() == TRUE /*&& bGrrDvcSwap == FALSE */ && nPickPlace == DEF_PLACE && ( nTargetLocIdx == LOC_IDX_TRAY_PP_TBL_1 || nTargetLocIdx == LOC_IDX_TRAY_PP_TBL_2 ) )
// 			{
// 				int nCountInsert = g_DMCont.m_dmTestPP.GN( NDM04_SITEGROUP1_INSERTED_DEVICE_CNT + ( nTargetLocIdx - LOC_IDX_TRAY_PP_TBL_1 ) );
// 				int nSetInDvc = g_DMCont.m_dmHandlerOpt.GN( NDM11_MD_AutoLoopUnitCount );
// 
// 				int nModule1On = g_TaskAutoTestPp[ eTestPP_1 ].GetModuleSocketOnCnt();
// 				int nModule2On = g_TaskAutoTestPp[ eTestPP_2 ].GetModuleSocketOnCnt();
// 				if( nModule1On > 0 && nModule2On > 0 )
// 				{
// 					if( nCountInsert >= nSetInDvc / 2 )
// 					{
// 						return;
// 					}
// 					else if( nCountInsert + ( int )vPkr.size() > nSetInDvc / 2 )
// 					{
// 						for( int i = 0; i < ( nCountInsert + ( int )vPkr.size() - nSetInDvc / 2 ); i++ )
// 							vPkr.pop_back();
// 					}
// 				}
// 				if( vPkr.size() == 0 )
// 					return;
// 			}
// 			else if( g_TaskSystemCtrl.GetAutoAuditUse() == TRUE /*&& bGrrDvcSwap == FALSE */ && nPickPlace == DEF_PLACE && ( nTargetLocIdx == LOC_IDX_TRAY_PP_TBL_1 || nTargetLocIdx == LOC_IDX_TRAY_PP_TBL_2 ) )
// 			{
// 				int nCountInsert = g_DMCont.m_dmTestPP.GN( NDM04_SITEGROUP1_INSERTED_DEVICE_CNT + ( nTargetLocIdx - LOC_IDX_TRAY_PP_TBL_1 ) );
// 				int nSetInDvc = g_DMCont.m_dmHandlerOpt.GN( NDM11_MD_AutoAuditUnitCount );
// 
// 				int nModule1On = g_TaskAutoTestPp[ eTestPP_1 ].GetModuleAuditSocketOnCnt();
// 				int nModule2On = g_TaskAutoTestPp[ eTestPP_2 ].GetModuleAuditSocketOnCnt();
// 				if( nModule1On > 0 && nModule2On > 0 )
// 				{
// 					if( nCountInsert >= nSetInDvc / 2 )
// 					{
// 						return;
// 					}
// 					else if( nCountInsert + ( int )vPkr.size() > nSetInDvc / 2 )
// 					{
// 						for( int i = 0; i < ( nCountInsert + ( int )vPkr.size() - nSetInDvc / 2 ); i++ )
// 							vPkr.pop_back();
// 					}
// 				}
// 				if( vPkr.size() == 0 )
// 					return;
// 			}
// 			/////////////////////////////////////////////////////////////
// 
// 			CTrayLoadPPCmd_MovePickPlace* pCmd = new CTrayLoadPPCmd_MovePickPlace();
// 			pCmd->eStage = nTargetLocIdx;
// 			pCmd->vPicker = vPkr;
// 			pCmd->Pocket = CPoint( nPockX, nPockY );
// 			pCmd->ePickPlace = ( ePPCmd )nPickPlace;
// 
// 			// find same command
// 			BOOL bSameCmd = FALSE;
// 			std::find_if( m_listTrayPpSchedule.begin(), m_listTrayPpSchedule.end()
// 						  , [ & ]( CBaseSeqCmd* _pTrayPpCmd ) -> bool
// 			{
// 				CTrayLoadPPCmd_MovePickPlace* pTrayPpCmd = ( CTrayLoadPPCmd_MovePickPlace* )_pTrayPpCmd;
// 				if( pTrayPpCmd->eStage == nTargetLocIdx && pTrayPpCmd->Pocket == pCmd->Pocket ) {
// 					bSameCmd = TRUE;
// 					return true;
// 				}
// 				return false;
// 			} );
// 
// 			// no search same command
// 			if( bSameCmd == FALSE ) {
// 				m_listTrayPpSchedule.push_back( pCmd );
// 			}
// 			else {
// 				delete pCmd;
// 			}
// 		}
// 	}
// }

// int CTaskAutoTrayLoadPP::FindPickPlaceFromTable( _eTrayPPIdx TrayPPIdx
// 												 , int nLocIdx
// 												 , int nTableDvc[ VIRTUAL_POCKET_Y_MAX ][ VIRTUAL_POCKET_X_MAX ]
// 												 , int nPickerPattern[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ]
// 												 , BOOL bPickPlace, int* n_Xpos, int* n_YPos
// 												 , int nWorkPkr[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ]
// 												 , _eUsePicker _picker_type )
// {
// 	CTaskTrayBasePP* TaskTrayPP = nullptr;
// 	switch( TrayPPIdx ) {
// 	case _eTrayPPIdx::eTrayPP_1_LD: { TaskTrayPP = &g_TaskTrayLoadPP; }break;
// 	case _eTrayPPIdx::eTrayPP_2_UD: { TaskTrayPP = &g_TaskTrayUnloadPP; }break;
// 	}
// 
// 
// 	ASSERT( LOC_IDX_TRAY_PP_TBL_1 <= nLocIdx && nLocIdx <= LOC_IDX_TRAY_PP_CONV_7 );
// 
// 	int nSearchY = 0, nSearchX = 0;
// 	int nSearchHandY = 0, nSearchHandX = 0;
// 
// 	/*DEF_WORK_FREE or DEF_WORK_RESERVED*/
// 	int nArrHandWorkSource[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ] = { DEF_WORK_FREE, };
// 	int nArrHandWorkCopy[ MAX_TRAY_PP_CNT_Y ][ MAX_TRAY_PP_CNT_X ] = { DEF_WORK_FREE, };
// 
// 	int nPocketCntX = 0, nPocketCntY = 0;
// 
// 	int nPitchType_X = TaskTrayPP->GetHandPitchType( CTaskTrayBasePP::eAxisPitchX, nLocIdx );
// 	int nPitchType_Y = TaskTrayPP->GetHandPitchType( CTaskTrayBasePP::eAxisPitchY, nLocIdx );
// 
// 	int nShtOffX = 1, nShtOffY = 1, nPitchOff_X = 1, nPitchOff_Y = 1;
// 
// 	// hand 보다 working하는 곳의 pitch가 더 크다
// 	if( nPitchType_X < 0 )
// 		nShtOffX = abs( nPitchType_X );
// 	else
// 		nPitchOff_X = nPitchType_X;
// 
// 	// hand y pitch보다 클 경우는 one by one만 지원해야한다.
// 	if( nPitchType_Y < 0 )
// 		ASSERT( 0 );
// 
// 	nPitchOff_Y = nPitchType_Y;
// 
// 	switch( nLocIdx )
// 	{
// 	case LOC_IDX_TRAY_PP_CONV_1:
// 	case LOC_IDX_TRAY_PP_CONV_2:
// 	case LOC_IDX_TRAY_PP_CONV_3:
// 	case LOC_IDX_TRAY_PP_CONV_4:
// 	case LOC_IDX_TRAY_PP_CONV_5:
// 	case LOC_IDX_TRAY_PP_CONV_6:
// 	case LOC_IDX_TRAY_PP_CONV_7:
// 		{
// 			nPocketCntX = g_DMCont.m_dmTrayPP.GN( NDM5_TrayDiv_X );
// 			nPocketCntY = g_DMCont.m_dmTrayPP.GN( NDM5_TrayDiv_Y );
// 		}break;
// 	case LOC_IDX_TRAY_PP_TBL_1:
// 	case LOC_IDX_TRAY_PP_TBL_2:
// 	case LOC_IDX_TRAY_PP_TBL_3:
// 	case LOC_IDX_TRAY_PP_TBL_4:
// 		{
// 			nPocketCntX = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Div_X );
// 			nPocketCntY = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Div_Y );
// 		}break;
// 	default:
// 		ASSERT( false );
// 		break;
// 	}
// 
// 
// 	int nTotalPossibleCnt = 0;	// 가장 Pick 할 수 있는 수량
// 	int nCurrentCnt = 0;	// 현재 까지 Pick 할수 있는 수량
// 	int nCXPosition = 0, nCYPosition = 0;
// 	// 1Y, 2Y는 계산 한 대로 가져 온다.	
// 
// 
// 	int nChkCleanBuffer = DEF_NOT_EXIST;
// 	int nHadData = DEF_NOT_EXIST;
// 	if( bPickPlace == DEF_PICK ) {	// Pick 일 때 조건
// 		nChkCleanBuffer = DEF_EXIST;
// 		nHadData = DEF_NOT_EXIST;
// 	}
// 	else {							// Place 일 때 조건	
// 		nChkCleanBuffer = DEF_NOT_EXIST;
// 		nHadData = DEF_EXIST;
// 	}
// 
// 	for( nSearchY = VIRTUAL_POCKET_Y_START - ( MAX_TRAY_PP_CNT_Y - 1 ) * nPitchOff_Y; nSearchY < VIRTUAL_POCKET_Y_START + nPocketCntY; nSearchY++ )
// 	{
// 		for( nSearchX = VIRTUAL_POCKET_X_START - ( MAX_TRAY_PP_CNT_X - 1 ) * nPitchOff_X; nSearchX < VIRTUAL_POCKET_X_START + nPocketCntX; nSearchX++ )
// 		{
// 			nCurrentCnt = 0;
// 			memset( nArrHandWorkSource, 0, sizeof( nArrHandWorkSource ) );
// 
// 			for( nSearchHandY = 0; nSearchHandY < MAX_TRAY_PP_CNT_Y; nSearchHandY++ )
// 			{
// 				int nSkipXCnt = 0;
// 				for( nSearchHandX = 0; nSearchHandX < MAX_TRAY_PP_CNT_X; nSearchHandX++ )
// 				{
// 					// loading picker / unloading picker setting interlock
// 					char setting = TaskTrayPP->m_TrayPP_picker_setting[ nSearchHandY ][ nSearchHandX ];
// 					if( setting != ( _picker_type == _eUsePicker_Load ? 'L' : 'U' ) ) {
// 						if( nPitchType_X < 0 ) nSkipXCnt++;
// 						continue;
// 					}
// 
// 					int nYPocket = nSearchY + nSearchHandY * nPitchOff_Y;
// 					int nXPocket = nSearchX + ( nSearchHandX - nSkipXCnt ) * nPitchOff_X;// -nSkipXCnt*nShtOffX;
// 
// 					int TableExist = nTableDvc[ nYPocket ][ nXPocket ];
// 					int PickerExist = nPickerPattern[ nSearchHandY ][ nSearchHandX ];
// 					if( TableExist == nChkCleanBuffer && PickerExist == nHadData )
// 					{
// 						nArrHandWorkSource[ nSearchHandY ][ nSearchHandX ] = DEF_WORK_RESERVED;
// 						nCurrentCnt++;
// 
// 						if( nPitchType_Y == 0 )
// 						{
// 							nSearchHandY = MAX_TRAY_PP_CNT_Y;
// 						}
// 						if( nPitchType_X == 0 )
// 						{
// 							nSearchHandX = MAX_TRAY_PP_CNT_X;
// 						}
// 					}
// 				}
// 			}
// 
// 			BOOL bLimitCheckOK = GetAreaProblem( TrayPPIdx, nSearchX - VIRTUAL_POCKET_X_START, nSearchY - VIRTUAL_POCKET_Y_START, ( eDeviceDataStage )( eDeviceStage_LD_TBL_1 + nLocIdx - LOC_IDX_TRAY_PP_TBL_1 ), nArrHandWorkSource ); // todo slt : 영역 점검을 위한 함수 구현 필요.
// 			if( bLimitCheckOK == TRUE )
// 			{
// 				if( nCurrentCnt > nTotalPossibleCnt )
// 				{
// 					//작업해라.
// 					nTotalPossibleCnt = nCurrentCnt;
// 					memcpy_s( nArrHandWorkCopy, sizeof( nArrHandWorkCopy ), nArrHandWorkSource, sizeof( nArrHandWorkSource ) );
// 					nCXPosition = nSearchX;
// 					nCYPosition = nSearchY;
// 				}
// 			}
// 		}
// 	}
// 
// 	memcpy_s( nWorkPkr, sizeof( nArrHandWorkCopy ), nArrHandWorkCopy, sizeof( nArrHandWorkCopy ) );
// 
// 	//0번 피커부터 할당되지 않은 포켓은 트림한다.
// 	int nPockOffsetX = 0;
// 	int nPockOffsetY = 1;
// 
// 	//X방향 첫 할당 위치가 X옵셋
// 	for( ; nPockOffsetX < MAX_TRAY_PP_CNT_X; nPockOffsetX++ )
// 	{
// 		if( nWorkPkr[ 0 ][ nPockOffsetX ] != DEF_WORK_FREE ) break;
// 		if( nWorkPkr[ 1 ][ nPockOffsetX ] != DEF_WORK_FREE ) break;
// 	}
// 	//X방향 첫 할당 위치가 1개라도 있으면 Y옵셋은 0
// 	for( int nX = 0; nX < MAX_TRAY_PP_CNT_X; nX++ )
// 	{
// 		if( nWorkPkr[ 0 ][ nX ] != DEF_WORK_FREE ) {
// 			nPockOffsetY = 0;
// 			break;
// 		}
// 	}
// 
// 	nCXPosition -= VIRTUAL_POCKET_X_START;
// 	nCYPosition -= VIRTUAL_POCKET_Y_START;
// 
// 	*n_Xpos = nCXPosition + ( nPockOffsetX / nShtOffX ) * nPitchOff_X;
// 	*n_YPos = nCYPosition + nPockOffsetY * nPitchOff_Y;
// 
// 	return nTotalPossibleCnt;
// }

/*
int CTaskAutoTrayLoadPP::MakeLoadTblWorkingIdx( _eTrayPPIdx TrayPPIdx )
{
	CTaskTrayBasePP* TaskTrayPP = nullptr;
	std::vector<int> vLoadTblWorkIdx;
	vLoadTblWorkIdx.clear();

	switch (TrayPPIdx) {
	case _eTrayPPIdx::eTrayPP_1_LD: {
		TaskTrayPP = &g_TaskTrayLoadPP;
		vLoadTblWorkIdx.push_back(eLoadTbl_2);
		vLoadTblWorkIdx.push_back(eLoadTbl_3);
	}break;
	case _eTrayPPIdx::eTrayPP_2_UD: {
		TaskTrayPP = &g_TaskTrayUnloadPP;
		vLoadTblWorkIdx.push_back(eLoadTbl_1);
		vLoadTblWorkIdx.push_back(eLoadTbl_4);
	}break;
	}

	CTime cCurTime = CTime::GetCurrentTime();

	std::vector<CTime> vArrivedTblTm; vArrivedTblTm.clear();
	int nCntLoadPicker = TaskTrayPP->GetTrayPpDvcCnt();
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

		bool all_site_is_off = true;
		std::vector< _ePressUnit > presses = (TrayPPIdx == eTrayPP_1_LD) ? m_test_modules_press_idx[eTrayPP_1_LD] : m_test_modules_press_idx[eTrayPP_2_UD];
		for each(auto press_idx in presses) {
			all_site_is_off &= g_TaskPressUnit[press_idx].GetSocketOnCnt() == 0;
		}

		int nWorkReserveTbl = g_TaskLoadTbl[i].GetWorkReserve();
		int nMotorPos = g_TaskLoadTbl[i].ChkMotorPos(LOC_IDX_TBL_LD_SIDE, CTaskLoadTable::eAxisAll);
		int SeqCmdStatus = g_TaskLoadTbl[i].GetSeqCmdStatus();
		int UnloadDeviceCnt = g_TaskLoadTbl[i].GetUnloadDeviceCnt();
		int LoadDeviceCnt = g_TaskLoadTbl[i].GetLoadDeviceCnt();

		if (all_site_is_off == true
			|| RID_TRAY_PP_(RIDX_TRAY_PP_1) != nWorkReserveTbl
			|| SeqCmdStatus != CBaseSeqCmd::eSeqCmdStatus_Ready
			|| nMotorPos != ERR_NO_ERROR
			|| (UnloadDeviceCnt == 0 && g_TaskSystemCtrl.IsOneCycleModeOn() == true && nCntLoadPicker == 0)
			|| (nCntLoadPicker == 0 && LoadDeviceCnt == 0 && UnloadDeviceCnt == 0))
		{
			vArrivedTblTm[nIdx] = (cCurTime + 99999);
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
*/