#include "stdafx.h"
#include "TaskAutoTrayFeeder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace base_seq_cmd;
using namespace std;

CTaskAutoTrayFeeder::CTaskAutoTrayFeeder( void )
{
	m_nAutoRunMainStep = eAutoLotState_InitNeed;
	m_nAutoRunSubStep = 0;
	m_nAutoRunBackUpMainStep = eAutoLotState_InitNeed;
	m_nAutoRunBackUpSubStep = 0;

	m_nFeederLastTrayRowStep = 0;

	m_mapExcuteState.SetAt( eExcutingStep_Start, _T( "Start" ) );
	m_mapExcuteState.SetAt( eExcutingStep_CheckCondition, _T( "CheckCondition" ) );
	m_mapExcuteState.SetAt( eExcutingStep_Pick, _T( "Pick" ) );
	m_mapExcuteState.SetAt( eExcutingStep_Place, _T( "Place" ) );
	m_mapExcuteState.SetAt(eExcutingStep_Place_WaitEnd, _T("PlaceWaitEnd"));
	m_mapExcuteState.SetAt(eExcutingStep_SafetyPos, _T("SafetyPos"));
	m_mapExcuteState.SetAt(eExcutingStep_SafetyPos_WaitEnd, _T("SafetyPos_WaitEnd"));
	m_mapExcuteState.SetAt( eExcutingStep_MoveLoadPos, _T( "MoveLoadPos" ) );
	m_mapExcuteState.SetAt( eExcutingStep_MoveStep, _T( "MoveStep" ) );
	m_mapExcuteState.SetAt(eExcutingStep_MoveStep_WaitEnd, _T("MoveStep_WaitEnd"));
	m_mapExcuteState.SetAt( eExcutingStep_End, _T( "End" ) );
}

CTaskAutoTrayFeeder::~CTaskAutoTrayFeeder( void )
{
}

int CTaskAutoTrayFeeder::Initialize()
{
	return CBaseTaskManager::Initialize();
}

void CTaskAutoTrayFeeder::Finalize()
{
	CBaseTaskManager::Finalize();
}

int CTaskAutoTrayFeeder::OnEvent( const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList/*=NULL*/ )
{
	return ERR_NO_ERROR;
}

int CTaskAutoTrayFeeder::OnCommandRsp( CBaseSeqCmd* pRspCmd, int nEventId, WPARAM wParam, LPARAM lParam, std::vector<CPoint>& vPocket_Picker )
{
	if( nEventId == CMD_EVT_ALARM )
	{
		if( pRspCmd->m_pSender == this ) {
			ChangeMainStateByRspAlarm();
		}
	}

	return CBaseTaskManager::OnCommandRsp( pRspCmd, nEventId, wParam, lParam, vPocket_Picker );
}

void CTaskAutoTrayFeeder::OnOperatorCommand( int nOperatorCommand, WPARAM wParam, LPARAM lParam )
{
}

void CTaskAutoTrayFeeder::ThreadProc1()
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

void CTaskAutoTrayFeeder::MakeLog( LPCTSTR fmt, ... )
{
	TCHAR tmpMsg[ 1024 ] = { 0, };
	if( fmt )
	{
		va_list argList;
		va_start( argList, fmt );
		_vstprintf_s( tmpMsg, fmt, argList );
		va_end( argList );
	}
	TWLOG.MakeLog( static_cast< int >( eLogID_Auto_TrayFeeder ), Debug, "", __LINE__, NULL, tmpMsg );
}

void CTaskAutoTrayFeeder::AutoState_Idle()
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

void CTaskAutoTrayFeeder::AutoState_InitNeed()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 2000:
		{
			g_TaskTrayFeeder.SetProcessCommand( eProcCmd_Stop );
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

void CTaskAutoTrayFeeder::AutoState_Initialize()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 3000:
	{
			CBaseSeqCmd::eSeqCmdStatus Tray_Loadpp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			CBaseSeqCmd::eSeqCmdStatus Tray_UnLoadpp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (Tray_Loadpp_cmd_status == CBaseSeqCmd::eSeqCmdStatus_Ready && Tray_UnLoadpp_cmd_status == CBaseSeqCmd::eSeqCmdStatus_Ready
				&& g_TaskTrayLoadPP.m_bInitialized == TRUE && g_TaskTrayUnloadPP.m_bInitialized == TRUE)
			{
				ChangeSubState(3001);
			}
				
	}break;
	case 3001:
		{
			CTrayFeederCmd_Initial* pSeqCmd = new CTrayFeederCmd_Initial();
			if( SendCommand( g_TaskTrayFeeder, pSeqCmd ) == eSeqCmdAck_OK ) {
				ChangeSubState( 3002 );
				break;
			}
		}break;
	case 3002:
		{
			CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = ( CBaseSeqCmd::eSeqCmdStatus )g_TaskTrayFeeder.GetSeqCmdStatus();
			if( feeder_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready ) {
				break;
			}

			ChangeSubState( 3999 );
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

void CTaskAutoTrayFeeder::AutoState_Pause()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d] [last step=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep, m_nLast_Excuting_Step );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 4000:
		{
			g_TaskTrayFeeder.SetProcessCommand( eProcCmd_Pause );
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

void CTaskAutoTrayFeeder::AutoState_Execute()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%s]" ), GetLotStateToString( m_nAutoRunMainStep ), GetExcuteStateStr() );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case eExcutingStep_Start:
		{
			ChangeSubState( eExcutingStep_CheckCondition );
		}break;

	case eExcutingStep_CheckCondition:
		{
			// Tray Not Exist, Conveyor Tray Not Exist
			// -> move load pos

			// Tray Not Exist, Conveyor Tray Exist, conveyor not working
			// -> pick

			// Tray Exist, Device Exist
			// -> move step ( device 있는 step으로 이동 )

			// Tray Exist, Device Empty, Transfer Pick Area Empty, Transfer Not working, conveyor not working
			// -> place


			CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = ( CBaseSeqCmd::eSeqCmdStatus )g_TaskTrayFeeder.GetSeqCmdStatus();
			if( feeder_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready ) {
				break;
			}

			bool transfer_area_tray_exist = g_TaskTrayFeeder.GetTrayExist_TransferArea_DvcMode(eExist);
			auto nChkPos = g_TaskTrayFeeder.ChkMotorPos(CTaskTrayFeeder::eAxisY, eLocIdxFeeder::LOC_IDX_FEEDER_TRAY_PLACE_POS);
			if (feeder_cmd_status == CBaseSeqCmd::eSeqCmdStatus_Ready && nChkPos != ERR_NO_ERROR && transfer_area_tray_exist == ERR_NO_ERROR) {
				if (g_Transfer_Feeder_work_area.get_reserved_task() == this)
					g_Transfer_Feeder_work_area.release_work_area(this);
			}

			//Feeder Unit 영역일 때, Feeder Motion Done Check.
			double dTrayCurPosX = 0.0;
			OneAxis* pMotorX = g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisX);
			pMotorX->GetCurrentPos(dTrayCurPosX);

			int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
			if (nDeviceMode == eDeviceMode)
			{
				bool bNeedAlarm = ChkRemainAutoSkipToDeviceAndAlarm();
	
				if (bNeedAlarm == true)
				{
					int nRemindPickCnt = g_DMCont.m_dmTrayPP.GN(NDM5_TM_LDTrayPP_Remind_Pick_Count);
					CParamList AlarmData;					
					int nPickerrCnt = ChkRemainAutoSkipToDeviceCnt();
					if (nRemindPickCnt < 1 && (nPickerrCnt > 0 && nPickerrCnt <= 3)) // Remind Pick 1회 및 Pick Error 3회 이하 Retry
					{
						g_TaskSystemCtrl.OnRcv_Req_Remain_AutoSkipDevice_Ack(TWMB_ID_RETRY, (WPARAM)(NULL), (LPARAM)NULL);
						g_DMCont.m_dmTrayPP.SN(NDM5_TM_LDTrayPP_Remind_Pick_Count, ++nRemindPickCnt);
						break;
					}		
					g_Error.AlarmReport(ERR_TRAY_PP_1_AUTO_SKIP_PICK_FAIL_DEVICE_REMAIN, AlarmData, &g_TaskSystemCtrl);
					g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
					break;
				}
				if (g_Error.IsSetAlarm(ERR_TRAY_PP_1_AUTO_SKIP_PICK_FAIL_DEVICE_REMAIN) == TRUE || bNeedAlarm == true)
					break;
			}


			//변경 필요
// 			double dFeederPosStart = INTERLOCK_LD_TRAYPP_X_WORK_ENABLE_FEEDER_START_POS;
// 			double dFeederPosEnd = INTERLOCK_LD_TRAYPP_X_WORK_ENABLE_FEEDER_END_POS;
// 
// 			if (dFeederPosStart <= dTrayCurPosX && dTrayCurPosX <= dFeederPosEnd) //Feeder 영역.
// 			{
// 				//Motion Done Check.
// 				OneAxis* pMotorZ = g_TaskTrayLoadPP.GetAxis(CTaskTrayLoadPP::eAxisZ);
// 				if (pMotorZ->MotionDone() != ERR_NO_ERROR || g_TaskTrayLoadPP.Chk_Z_Safety_Sen_All() != ERR_NO_ERROR) {
// 					break;
// 				}
// 				if (g_TaskTrayLoadPP.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready 
// 					&& g_TaskTrayLoadPP.GetActiveSeqCmdName() == _T("TrayLoadPPCmd_MovePickPlace"))
// 				{
// 					break;
// 				}
// 			}
// 			int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
// 			if (nDeviceMode == eDeviceMode)
// 			{
// 				bool bNeedAlarm = ChkRemainAutoSkipToDeviceAndAlarm();
// 				if (bNeedAlarm == true)
// 				{
// 					CParamList AlarmData;
// 					g_Error.AlarmReport(ERR_TRAY_PP_1_AUTO_SKIP_PICK_FAIL_DEVICE_REMAIN ,AlarmData, &g_TaskSystemCtrl);
// 					g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
// 					break;
// 				}
// 				if (g_Error.IsSetAlarm(ERR_TRAY_PP_1_AUTO_SKIP_PICK_FAIL_DEVICE_REMAIN) == TRUE || bNeedAlarm == true)
// 					break;
// 			}


			bool feeder_tray_exist = g_TaskTrayFeeder.GetTrayExist_Data();
			bool conveyor_tray_not_exist = g_TaskStacker[ eSTACKER_03 ].GetExistTray_RearPos_DvcMode( eNot_Exist ) == ERR_NO_ERROR;
			auto ret = g_TaskTrayFeeder.ChkMotorPos(CTaskTrayFeeder::eAxisY, eLocIdxFeeder::LOC_IDX_FEEDER_LOAD_PICK_START_POS);
			if( feeder_tray_exist == false && conveyor_tray_not_exist && ret != ERR_NO_ERROR) {
				ChangeSubState( eExcutingStep_MoveLoadPos );
				MakeLog(_T("[MainStep=%s] [SubStep=%s] eExcutingStep_MoveLoadPos"), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr());
				break;
			}


			bool conveyor_tray_exist = g_TaskStacker[ eSTACKER_03 ].GetExistTray_RearPos_DvcMode( eExist ) == ERR_NO_ERROR;
			int nExistTray = g_DMCont.m_dmDevice.GetTrayExist(eDeviceDataStage::eDeviceStage_STACKER_CONV_1 + eSTACKER_03);
			int feeder_tray_not_exist_sen = g_TaskTrayFeeder.ChkTrayExistSensor(eNot_Exist);
			bool conveyor_moving = g_TaskStacker[ eSTACKER_03 ].IsConveyorMoving();
			if( feeder_tray_exist == false && conveyor_tray_exist && conveyor_moving == false && feeder_tray_not_exist_sen == ERR_NO_ERROR && nExistTray == DEF_EXIST) {
				ChangeSubState( eExcutingStep_Pick );
				MakeLog(_T("[MainStep=%s] [SubStep=%s] eExcutingStep_Pick"), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr());
				break;
			}


			bool feeder_tray_empty_device = g_TaskTrayFeeder.IsEmptyDevice();
			bool bIsEmptyTrayFeeder_Data = g_DMCont.m_dmDevice.IsEmptyTray(eDeviceStage_TRAY_FEEDER);
			bool transfer_area_tray_not_exist = g_TaskTrayFeeder.GetTrayExist_TransferArea_DvcMode( eNot_Exist ) == ERR_NO_ERROR;
			bool feeder_can_move = false;
			eLocIdxTrayPP LocIdxTrayPP;
			int nErrWhereIsMotorPosXY = g_TaskTrayLoadPP.WhereIsMotorPosXY( LocIdxTrayPP );
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = ( CBaseSeqCmd::eSeqCmdStatus )g_TaskTrayLoadPP.GetSeqCmdStatus();
			if( nErrWhereIsMotorPosXY == ERR_NO_ERROR ) {
				if( LocIdxTrayPP != eLocIdxTrayPP::LOC_IDX_TRAY_PP_FEEDER || 
					load_pp_cmd_status == CBaseSeqCmd::eSeqCmdStatus::eSeqCmdStatus_Ready || g_TaskTrayLoadPP.GetActiveSeqCmdName() == _T("TrayLoadPPCmd_MovePos"))
				{
					feeder_can_move = true;
				}
			}


			if( feeder_tray_exist && 
				feeder_tray_empty_device == false && 
				feeder_can_move ) 
			{				
				int tray_row_step = g_TaskTrayFeeder.GetTrayDeviceExistRowStep();
// 				if (tray_row_step <= -1) {
// 					ChangeSubState(eExcutingStep_CheckCondition);
// 					break;
// 				}
				int nRetFeederPos = g_TaskTrayFeeder.ChkMotorPos(CTaskTrayFeeder::eAxisY, LOC_IDX_FEEDER_LOAD_PICK_START_POS, tray_row_step);
				if( tray_row_step != m_nFeederLastTrayRowStep || nRetFeederPos != ERR_NO_ERROR) {
 					m_nFeederLastTrayRowStep = tray_row_step;
					if (m_nFeederLastTrayRowStep <= -1)
					{
						int a = 0;
					}
					ChangeSubState( eExcutingStep_MoveStep );
					MakeLog(_T("[MainStep=%s] [SubStep=%s] eExcutingStep_MoveStep = %d"), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr(), tray_row_step);
					break;
				}
			}

// 			int nSetInDvc = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrUnitCount);
// 			int nReservedDvcCnt = g_DMCont.m_dmTestPP.GN(NDM04_SITEGROUP1_INSERTED_DEVICE_CNT);
// 			if (g_TaskSystemCtrl.GetAutoGrrUse() == TRUE) {
// 				if (g_TaskSystemCtrl.GetAutoGrrCancel() != TRUE /*&& nSetInDvc != nReservedDvcCnt*/) {
// 					ChangeSubState(eExcutingStep_CheckCondition);
// 					break;
// 				}
// 			}

			int nCycleSortDvc = g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt);
			int nQaSampleCnt = g_TaskSystemCtrl.GetQASampleCount();

			int partial_tray_end = g_DMCont.m_dmTrayPP.GN(NDM5_TM_TrayPP_Tray_End);
			bool feeder_tray_exist_sen = g_TaskTrayFeeder.GetTrayExistSensor_DvcMode(eExist)/* == ERR_NO_ERROR*/;
			CBaseSeqCmd::eSeqCmdStatus transfer_cmd_status = ( CBaseSeqCmd::eSeqCmdStatus )g_TaskTransfer.GetSeqCmdStatus();
			bool bSafetyTransfer = g_TaskTransfer.is_safety_state();
			if( feeder_tray_exist && 
				feeder_tray_exist_sen&&
				(bIsEmptyTrayFeeder_Data || partial_tray_end)&&
				transfer_cmd_status == CBaseSeqCmd::eSeqCmdStatus::eSeqCmdStatus_Ready &&
				g_TaskStacker[eSTACKER_03].GetActiveSeqCmdName() != _T("CStackerCmd_Load") &&				
				transfer_area_tray_not_exist &&
				conveyor_moving == false &&
				bSafetyTransfer)
			{
// 				BOOL bAutoQAUse = g_TaskSystemCtrl.GetAutoQAUse();
// 				int nQAStatus = g_DMCont.m_dmEQP.GN(NDM0_QA_Status);
// 				if (bAutoQAUse == TRUE && (nCycleSortDvc >= nQaSampleCnt && (bIsEmptyTrayFeeder_Data || partial_tray_end)))
// 					break;
				
				BOOL bAutoQAUse = g_TaskSystemCtrl.GetAutoQAUse();
				if (bAutoQAUse == TRUE)
				{
					g_DMCont.m_dmFeeder.SB(BDM10_FEEDER_TRAY_END, TRUE);
				}
			
				ChangeSubState( eExcutingStep_Place );
				MakeLog(_T("[MainStep=%s] [SubStep=%s] eExcutingStep_Place"), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr());
				break;
			}


		}break;
	case eExcutingStep_Pick:
		{
			CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
			if (feeder_cmd_status == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{	
				CTrayFeederCmd_MovePickPlace* pSeqCmd = new CTrayFeederCmd_MovePickPlace();
				pSeqCmd->m_nCmdStage = eLocIdxFeeder::LOC_IDX_FEEDER_TRAY_PICK_POS;
				pSeqCmd->m_nCmdPickPlace = ePick;

				if( SendCommand( g_TaskTrayFeeder, pSeqCmd ) == eSeqCmdAck_OK ) {
				ChangeSubState( eExcutingStep_CheckCondition );
				break;
				}
			}
		}break;
	case eExcutingStep_Place:
		{
			CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
			if (feeder_cmd_status == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				bool work_enable = g_Transfer_Feeder_work_area.reserve_work_area(this);
				if (work_enable != true)
					break;

// 				bool bIsEmptyTrayFeeder_Data = g_DMCont.m_dmDevice.IsEmptyTray(eDeviceStage_TRAY_FEEDER);
// 				if (bIsEmptyTrayFeeder_Data != TRUE) {
// 					g_DMCont.m_dmTrayPP.SN(NDM5_TM_TrayPP_Tray_End, FALSE);
// 				}


				CTrayFeederCmd_MovePickPlace* pSeqCmd = new CTrayFeederCmd_MovePickPlace();
				pSeqCmd->m_nCmdStage = eLocIdxFeeder::LOC_IDX_FEEDER_TRAY_PLACE_POS;
				pSeqCmd->m_nCmdPickPlace = ePlace;

				if (SendCommand(g_TaskTrayFeeder, pSeqCmd) == eSeqCmdAck_OK) {
					ChangeSubState(eExcutingStep_Place_WaitEnd);
					break;
				}
			}
		}break;
	case eExcutingStep_Place_WaitEnd:
		{
			CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
			if (feeder_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				int nIsTrayEnd = g_DMCont.m_dmTrayPP.GN(NDM5_TM_TrayPP_Tray_End);
				if (nIsTrayEnd == TRUE)
				{
					g_DMCont.m_dmTrayPP.SN(NDM5_TM_TrayPP_Tray_End, FALSE);
				}
				ChangeSubState(eExcutingStep_SafetyPos);
				break;
			}
		}break;
	case eExcutingStep_SafetyPos:
	{
		CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
		if (feeder_cmd_status == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			CTrayFeederCmd_MovePos* pSeqCmd = new CTrayFeederCmd_MovePos();
			pSeqCmd->m_nCmdStage = eLocIdxFeeder::LOC_IDX_FEEDER_SAFETY;

			if (SendCommand(g_TaskTrayFeeder, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_SafetyPos_WaitEnd);
				break;
			}
		}
	}break;
	case eExcutingStep_SafetyPos_WaitEnd:
	{
		CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
		auto ret = g_TaskTrayFeeder.ChkMotorPos(CTaskTrayFeeder::eAxisY, eLocIdxFeeder::LOC_IDX_FEEDER_SAFETY);
		if (feeder_cmd_status == CBaseSeqCmd::eSeqCmdStatus_Ready && ret == ERR_NO_ERROR) {
			if (g_Transfer_Feeder_work_area.get_reserved_task() == this)
				g_Transfer_Feeder_work_area.release_work_area(this);

			ChangeSubState(eExcutingStep_CheckCondition);		
			break;
		}
	}break;
	case eExcutingStep_MoveLoadPos:
		{
			CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
			if (feeder_cmd_status == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				CTrayFeederCmd_MovePos* pSeqCmd = new CTrayFeederCmd_MovePos();
				pSeqCmd->m_nCmdStage = eLocIdxFeeder::LOC_IDX_FEEDER_LOAD_PICK_START_POS;

				if (SendCommand(g_TaskTrayFeeder, pSeqCmd) == eSeqCmdAck_OK) {
					ChangeSubState(eExcutingStep_CheckCondition);
					break;
				}
			}
		}break;
	case eExcutingStep_MoveStep:
		{
// 			int TrayDiv_Y = g_DMCont.m_dmTrayPP.GN( NDM5_TrayDiv_Y );
// 
// 			int nY_PitchType = 1;
// 			if (g_DMCont.m_dmTrayPP.GN(NDM5_Tray_Pitch_Y) < DEF_TRAY_PP_MAX_Y_PITCH_NARROW) {
// 				nY_PitchType = 2;
// 			}
// 
// 			//Pitch Type에 따른 Step Count
// 			int nRowStep = m_nFeederLastTrayRowStep
			
// 			if (m_nFeederLastTrayRowStep < 0)
// 			{			
// 				bool bReserveWork = g_Transfer_Feeder_work_area.reserve_work_area(this);
// 				if (bReserveWork != true) {
// 					break;
// 				}
// 			}

			CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();

			if (feeder_cmd_status == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				CTrayFeederCmd_MovePos* pSeqCmd = new CTrayFeederCmd_MovePos();
				pSeqCmd->m_nCmdStage = eLocIdxFeeder::LOC_IDX_FEEDER_LOAD_PICK_START_POS;
				//pSeqCmd->m_nTrayRowStep = TrayDiv_Y - m_nFeederLastTrayRowStep; // tray 아래 부터 load hand가 pick 함.
				pSeqCmd->m_nTrayRowStep = m_nFeederLastTrayRowStep; // tray 아래 부터 load hand가 pick 함.
				if (SendCommand(g_TaskTrayFeeder, pSeqCmd) == eSeqCmdAck_OK) {
					ChangeSubState(eExcutingStep_CheckCondition);
					break;
				}
			}
		}break;
// 	case eExcutingStep_MoveStep_WaitEnd:
// 	{
// 		CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
// 
// 		if (feeder_cmd_status == CBaseSeqCmd::eSeqCmdStatus_Ready)
// 		{
// 			bool bSafetyFeeder = g_TaskTrayFeeder.GetSafetySensor();
// 			if (g_Transfer_Feeder_work_area.get_reserved_task() == this)
// 			{
// 				if(bSafetyFeeder == true)
// 					g_Transfer_Feeder_work_area.release_work_area(this);
// 			}
// 				
// 			ChangeSubState(eExcutingStep_CheckCondition);
// 			break;
// 		}
// 	}break;

	case eExcutingStep_End:
		{
			ChangeMainState( eAutoLotState_NormalComplete );
		}break;
	}
}

void CTaskAutoTrayFeeder::AutoState_NormalComplete()
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

void CTaskAutoTrayFeeder::AutoState_AlarmStop()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d] [last step=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep, m_nLast_Excuting_Step );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 7000:
		{
			g_TaskTrayFeeder.SetProcessCommand( base_seq_cmd::eProcCmd_Pause );
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

void CTaskAutoTrayFeeder::AutoState_Resume()
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
			g_TaskTrayFeeder.SetProcessCommand( base_seq_cmd::eProcCmd_Resume );
			ChangeSubState( 8001 );
		}break;
	case 8001:
		{
			ChangeSubState(8002);
		}break;
	case 8002:
		{
			ChangeMainState( eAutoLotState_Executing );
		}break;

	default:
		ChangeSubState( 8000 );
		break;
	}
}

void CTaskAutoTrayFeeder::AutoState_CleanOut()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d] [SubStep=%s]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, GetExcuteStateStr());
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case eCleanoutStep_Start:
	{
		ChangeSubState(eCleanoutStep_CheckCondition);
	}break;

	case eCleanoutStep_CheckCondition:
	{		
		// Tray Exist, Device Empty, Transfer Pick Area Empty, Transfer Not working, conveyor not working
		// -> place

		CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
		if (feeder_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		bool feeder_tray_exist       = g_TaskTrayFeeder.GetTrayExist_Data();
		bool conveyor_tray_not_exist = g_TaskStacker[eSTACKER_03].GetExistTray_RearPos_Sen(eNot_Exist) == ERR_NO_ERROR;
		if (feeder_tray_exist == false && conveyor_tray_not_exist) {
			ChangeSubState(eCleanoutStep_End);
			break;
		}

		// stacker가 front, rear에 있는 tray를 모두 stacker로 적재한 이후에 feeder가 tray를 들고 있다면 cv에 내려 놓는다.
		bool FrontNoExist_data                         = g_TaskStacker[eSTACKER_03].GetExistTray_FrontPos_DvcMode(DEF_NOT_EXIST) == ERR_NO_ERROR;
		bool RearNoExist_data                          = g_TaskStacker[eSTACKER_03].GetExistTray_RearPos_DvcMode(DEF_NOT_EXIST) == ERR_NO_ERROR;

		bool conveyor_tray_exist_sen                   = g_TaskStacker[eSTACKER_03].GetExistTray_RearPos_Sen(eExist) == ERR_NO_ERROR;
		bool conveyor_moving                           = g_TaskStacker[eSTACKER_03].IsConveyorMoving();
		bool feeder_tray_empty_device_data             = g_TaskTrayFeeder.IsEmptyDevice();
		bool transfer_area_tray_not_exist_data         = !g_TaskTrayFeeder.GetTrayExist_TransferArea_Data();
		bool transfer_area_tray_not_exist_sen          = g_TaskTrayFeeder.GetTrayExist_TransferArea_DvcMode(eNot_Exist) == ERR_NO_ERROR;
		bool feeder_can_move                           = false;

		CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status  = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
		CBaseSeqCmd::eSeqCmdStatus transfer_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTransfer.GetSeqCmdStatus();

		if (feeder_tray_exist == true &&
			transfer_cmd_status == CBaseSeqCmd::eSeqCmdStatus::eSeqCmdStatus_Ready &&
			transfer_area_tray_not_exist_data == true &&
			conveyor_moving == false &&
			RearNoExist_data == true &&
			FrontNoExist_data == true)
		{
			ChangeSubState(eCleanoutStep_Place);
			break;
		}

		// loader cv와 feeder가 작업 할 tray가 없다면 cleanout 종료.
		if (feeder_tray_exist == false &&
			transfer_cmd_status == CBaseSeqCmd::eSeqCmdStatus::eSeqCmdStatus_Ready &&
			transfer_area_tray_not_exist_data == true &&
			conveyor_moving == false &&
			FrontNoExist_data == true &&
			RearNoExist_data == true)
		{
			ChangeSubState(eCleanoutStep_End);
			break;
		}
	}break;

	case eCleanoutStep_Place:
	{
		CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
		if (feeder_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		CTrayFeederCmd_MovePickPlace* pSeqCmd = new CTrayFeederCmd_MovePickPlace();
		pSeqCmd->m_nCmdStage = eLocIdxFeeder::LOC_IDX_FEEDER_TRAY_PLACE_POS;
		pSeqCmd->m_nCmdPickPlace = ePlace;

		if (SendCommand(g_TaskTrayFeeder, pSeqCmd) == eSeqCmdAck_OK) {
			ChangeSubState(eCleanoutStep_Wait_PlaceComp);
			break;
		}
	}break;

	case eCleanoutStep_Wait_PlaceComp:
	{
		CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
		if (feeder_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}
		ChangeSubState(eCleanoutStep_Move_Safety_pos);
	}break;
	case eCleanoutStep_Move_Safety_pos:
	{
		CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
		if (feeder_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		CTrayFeederCmd_MovePos* pSeqCmd = new CTrayFeederCmd_MovePos();
		pSeqCmd->m_nCmdStage = eLocIdxFeeder::LOC_IDX_FEEDER_SAFETY;

		if (SendCommand(g_TaskTrayFeeder, pSeqCmd) == eSeqCmdAck_OK) {
			ChangeSubState(eCleanoutStep_End);
			break;
		}
	}
	case eCleanoutStep_End:
	{
		CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
		if (feeder_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		ChangeMainState(eAutoLotState_NormalComplete);
	}break;

	default:
	{
		ChangeSubState(eCleanoutStep_Start);
	}break;
	}

}

bool CTaskAutoTrayFeeder::ChkRemainAutoSkipToDeviceAndAlarm()
{
	//BOOL bAutoGrrUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);
	BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
	if (bAutoGrrUse == TRUE)
		return false;

	BOOL bAutoSkip = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoSkipUse);

	if (bAutoSkip == FALSE)
		return false;


	int nPocketCntX = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
	int nPocketCntY = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);
	ST_DD_DEVICE stDevice;

	bool bFindAutoSkipDevice = false;
	//Stacker

	// 	std::for_each(g_load_stackers.begin(), g_load_stackers.end(),
	// 		[&](CTaskAutoStacker* _load_stacker)
	// 	{
	// 		if (_load_stacker->GetConveyorState() == ReadyToUnload)
	// 		{
	// 			int stacker_idx = _load_stacker->GetStackerIndex();
	// 
	// 			for (int j = 0; j < nPocketCntY; j++)
	// 			{
	// 				for (int i = 0; i < nPocketCntX; i++)
	// 				{
	// 					stDevice.clear();
	// 					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_STACKER_CONV_1 + stacker_idx, i, j, &stDevice);
	// 					if (stDevice.sExist == eDeviceData_None && strcmp(stDevice.szBinResult, DEF_TRAY_PICK_ERR_DEVICE) == 0)
	// 						bFindAutoSkipDevice = true;
	// 				}
	// 			}
	// 		}
	// 	});

	//Feeder

	int nchk_feeder_tray_exist = g_TaskTrayFeeder.GetTrayExistSensor();
	bool bDeviceExist = g_DMCont.m_dmDevice.IsEmptyTray(eDeviceStage_TRAY_FEEDER) == FALSE;
	if (nchk_feeder_tray_exist == true && bDeviceExist == false)
	{
		for (int j = 0; j < nPocketCntY; j++)
		{
			for (int i = 0; i < nPocketCntX; i++)
			{
				stDevice.clear();
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TRAY_FEEDER, i, j, &stDevice);
				if (stDevice.sExist == eDeviceData_None && strcmp(stDevice.szBinResult, DEF_TRAY_PICK_ERR_DEVICE) == 0)
					bFindAutoSkipDevice = true;
			}
		}
	}

	return bFindAutoSkipDevice;
}

int CTaskAutoTrayFeeder::ChkRemainAutoSkipToDeviceCnt()
{
	int nPocketCntX = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
	int nPocketCntY = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);
	ST_DD_DEVICE stDevice;

	int  nFindAutoSkipDeviceCnt = 0;

	int nchk_feeder_tray_exist = g_TaskTrayFeeder.GetTrayExistSensor();
	bool bDeviceExist = g_DMCont.m_dmDevice.IsEmptyTray(eDeviceStage_TRAY_FEEDER) == FALSE;
	if (nchk_feeder_tray_exist == true && bDeviceExist == false)
	{
		for (int j = 0; j < nPocketCntY; j++)
		{
			for (int i = 0; i < nPocketCntX; i++)
			{
				stDevice.clear();
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TRAY_FEEDER, i, j, &stDevice);
				if (stDevice.sExist == eDeviceData_None && strcmp(stDevice.szBinResult, DEF_TRAY_PICK_ERR_DEVICE) == 0)
					nFindAutoSkipDeviceCnt++;
			}
		}
	}

	return nFindAutoSkipDeviceCnt;
}
