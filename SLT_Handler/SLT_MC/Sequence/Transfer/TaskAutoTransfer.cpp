
#include "stdafx.h"
#include "TaskAutoTransfer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace base_seq_cmd;
using namespace std;

CTaskAutoTransfer::CTaskAutoTransfer( void )
{
	m_nAutoRunMainStep = eAutoLotState_InitNeed;
	m_nAutoRunSubStep = 0;
	m_nAutoRunBackUpMainStep = eAutoLotState_InitNeed;
	m_nAutoRunBackUpSubStep = 0;

	m_mapExcuteState.SetAt( eExcutingStep_Start, _T( "Start" ) );

	m_mapExcuteState.SetAt( eExcutingStep_Check_Work_Area_Enable, "Check_Work_Area_Enable" );

	m_mapExcuteState.SetAt( eExcutingStep_MovePick_Src_Conveyor_Pos, "MovePick_Src_Conveyor_Pos" );
	m_mapExcuteState.SetAt( eExcutingStep_WaitEnd_MovePick_Src_Conveyor_Pos, "WaitEnd_MovePick_Src_Conveyor_Pos" );
	m_mapExcuteState.SetAt( eExcutingStep_MovePlace_Dest_Conveyor_Pos, "MovePlace_Dest_Conveyor_Pos" );
	m_mapExcuteState.SetAt( eExcutingStep_WaitEnd_MovePlace_Dest_Conveyor_Pos, "WaitEnd_MovePlace_Dest_Conveyor_Pos" );

	m_mapExcuteState.SetAt( eExcutingStep_Move_SafetyPos, "Move_SafetyPos" );
	m_mapExcuteState.SetAt( eExcutingStep_WaitEnd_Move_SafetyPos, "WaitEnd_Move_SafetyPos" );

	m_mapExcuteState.SetAt(eExcutingStep_WaitEnd_Move_UnloadEmptyPos, "WaitEnd_Move_UnloadEmptyPos");
	
	m_mapExcuteState.SetAt( eExcutingStep_End, _T( "End" ) );

	m_bStepChangeFalg = TRUE;
}


CTaskAutoTransfer::~CTaskAutoTransfer( void )
{
}

int CTaskAutoTransfer::Initialize( TCHAR* szDriverFileName )
{
	return CBaseTaskManager::Initialize();
}

void CTaskAutoTransfer::Finalize()
{
	CBaseTaskManager::Finalize();
}


int CTaskAutoTransfer::OnEvent( const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList/*=NULL*/ )
{
	return ERR_NO_ERROR;
}

int CTaskAutoTransfer::OnCommandRsp( CBaseSeqCmd * pRspCmd, int nEventId, WPARAM wParam, LPARAM lParam, std::vector<CPoint>& vPocket_Picker )
{
	if( nEventId == CMD_EVT_ALARM )
	{
		if( pRspCmd->m_pSender == this )
			ChangeMainStateByRspAlarm();
	}

	return CBaseTaskManager::OnCommandRsp( pRspCmd, nEventId, wParam, lParam, vPocket_Picker );
}

void CTaskAutoTransfer::OnOperatorCommand( int nOperatorCommand, WPARAM wParam, LPARAM lParam )
{
}

void CTaskAutoTransfer::ThreadProc1()
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

void CTaskAutoTransfer::MakeLog( LPCTSTR fmt, ... )
{
	TCHAR tmpMsg[ 1024 ] = { 0, };
	if( fmt )
	{
		va_list argList;
		va_start( argList, fmt );
		_vstprintf_s( tmpMsg, fmt, argList );
		va_end( argList );
	}
	TWLOG.MakeLog( static_cast< int >( eLogID_Auto_Transfer ), Debug, "", __LINE__, NULL, tmpMsg );
}

void CTaskAutoTransfer::AutoState_Idle()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
		case 1000:
			{
				ChangeSubState( 1001 );
			}break;
		case 1001:
			{
			}break;
		default:
			ChangeSubState( 1000 );
			break;
	}
}

void CTaskAutoTransfer::AutoState_InitNeed()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
		case 2000:
			{
				g_TaskTransfer.SetProcessCommand( eProcCmd_Stop );
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

void CTaskAutoTransfer::AutoState_Initialize()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
		case 3000:
			{
				int nComp = 0;
				for (int nIdx = 0; nIdx < eMaxTrayPPCount; nIdx++)
				{
					CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
					if (pTaskTrayPP[nIdx]->GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready
						&& pTaskTrayPP[nIdx]->m_bInitialized == TRUE)
					{
						nComp++;
					}
					else
					{
						break;
					}
				}	


				if (nComp == eMaxTrayPPCount)
				{
					ChangeSubState(3001);
					break;
				}
					
			}break;
		case 3001:
		{
			if (g_TaskTrayFeeder.GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready
				&& g_TaskTrayFeeder.m_bInitialized == TRUE)
			{
				ChangeSubState(4000);
			}

		}break;
		case 4000:
			{
				CTransferCmd_Initial* pSeqCmd = new CTransferCmd_Initial();
				if( SendCommand( g_TaskTransfer, pSeqCmd ) == ERR_NO_ERROR ) {
					ChangeSubState( 4001 );
				}
				else {
					ChangeMainState( eAutoLotState_InitNeed );
				}
			}break;
		case 4001:
			{
				if( g_TaskTransfer.GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready
					&& g_TaskTransfer.m_bInitialized == TRUE )
					ChangeSubState( 4100 );
			}break;
		case 4100:
			{
				ChangeMainState( eAutoLotState_Idle );
			}break;
		default:
			ChangeSubState( 3000 );
			break;
	}
}

void CTaskAutoTransfer::AutoState_Pause()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
		case 4000:
			{
				g_TaskTransfer.SetProcessCommand( eProcCmd_Pause );
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

void CTaskAutoTransfer::AutoState_Execute()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%s]" ), GetLotStateToString( m_nAutoRunMainStep ), GetExcuteStateStr() );
		m_bStepChangeFalg = TRUE;
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	if( m_nAutoRunSubStep > eExcutingStep_End ) {
		m_nAutoRunSubStep = m_nLast_Excuting_Step;
	}
	m_nLast_Excuting_Step = m_nAutoRunSubStep;


	switch( m_nAutoRunSubStep )
	{
		case eExcutingStep_Start:
			{
				//Conveyor Sensor, Data 확인
				int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);				
				if (nDeviceMode != eNoDeviceNoTray) {
					bool bSetAlarm = false;
					for (int i = 0; i < eMaxStackerCount; i++) {
						if (g_TaskStacker[i].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready) {														
							int nRearExistTray        = -1;
							int nRearExistSensor      = -1;

							int nTrayAlignSensor      = g_TaskStacker[i].get_cyl_TrayAlign(DEF_FORW);
							int nFrontExistTray       = g_DMCont.m_dmDevice.GetTrayExist(eDeviceDataStage::eDeviceStage_STACKER_BTM_1 + i);
							int nFrontExistSensor     = g_TaskStacker[i].GetExistTray_FrontPos_Sen(DEF_EXIST);

							if (g_TaskAutoStacker[i].GetStackerType() == eStackerType_Load ) {
								nRearExistTray   = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_CONV_3_TRANSFER_AREA);
								nRearExistSensor = g_TaskTrayFeeder.GetTrayExist_TransferArea_DvcMode(eExist);
								nTrayAlignSensor = ERR_NO_ERROR;
							}
							else
							{
								nRearExistTray   = g_DMCont.m_dmDevice.GetTrayExist(eDeviceDataStage::eDeviceStage_STACKER_CONV_1 + i);
								nRearExistSensor = g_TaskStacker[i].GetExistTray_RearPos_Sen(DEF_EXIST);
							}

							// Rear Data O , Rear Sensor X
							if (nRearExistTray == DEF_EXIST && (nRearExistSensor != ERR_NO_ERROR || nTrayAlignSensor != ERR_NO_ERROR)) {
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat(_T("Conveyor rear tray data exist. But conveyor rear exist sensor not exist or Tray align sensor error"));
								int nErrId;
								if (g_TaskAutoStacker[i].GetStackerType() == eStackerType_Load) {
									nErrId = ERR_TRNASFER_FEEDER_AREA_SENSOR_NOT_EXIST_ON;
								}
								else {
									nErrId = (nRearExistSensor != ERR_NO_ERROR ? ERR_STACKER_1_REAR_TRAY_EXIST_SENSOR_DETECT_TIMEOUT + i : ERR_STACKER_1_TRAY_ALIGN_FWD_TIMEOUT + i);
								}

								g_Error.AlarmReport(nErrId, AlarmData, &g_TaskSystemCtrl);
								bSetAlarm = true;
							}
							// Rear Data X , Rear Sensor O
							else if (nRearExistTray != DEF_EXIST && nRearExistSensor == ERR_NO_ERROR) {
								if(i == eSTACKER_03 && g_TaskTrayFeeder.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready &&
									g_TaskTrayFeeder.GetActiveSeqCmdName() == _T("CTrayFeederCmd_MovePickPlace")){ 
								}
								else {
									CParamList AlarmData;
									AlarmData.m_fnAddItemFormat(_T("Conveyor rear tray data not exist. But conveyor rear exist sensor exist"));
									g_Error.AlarmReport(ERR_STACKER_1_TRAY_EXIST_REAR_POS + i, AlarmData, &g_TaskSystemCtrl);
									bSetAlarm = true;
								}
							}
							// Front Data O , Front Sensor X
							else if (nFrontExistTray == DEF_EXIST && nFrontExistSensor != ERR_NO_ERROR) {
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat(_T("Conveyor front tray data exist. But conveyor front exist sensor not exist"));
								g_Error.AlarmReport(ERR_STACKER_1_TRAY_NOT_EXIST_FRONT_POS + i, AlarmData, &g_TaskSystemCtrl);
								bSetAlarm = true;
							}
							// Front Data X , Front Sensor O
							else if (nFrontExistTray != DEF_EXIST && nFrontExistSensor == ERR_NO_ERROR) {
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat(_T("Conveyor front tray data not exist. But conveyor front exist sensor exist"));
								g_Error.AlarmReport(ERR_STACKER_1_TRAY_EXIST_FRONT_POS + i, AlarmData, &g_TaskSystemCtrl);
								bSetAlarm = true;
							}
						}

						if (bSetAlarm == true)
						{
							g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
							break;
						}	
					}
				}
// 				if (nDeviceMode == eDeviceMode)
// 				{
// 					bool bNeedAlarm = ChkRemainAutoSkipToDeviceAndAlarm();
// 					if (bNeedAlarm == true)
// 					{
// 						CParamList AlarmData;
// 						g_Error.AlarmReport(ERR_TRAY_PP_1_AUTO_SKIP_PICK_FAIL_DEVICE_REMAIN ,AlarmData, &g_TaskSystemCtrl);
// 						g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
// 						break;
// 					}
// 					if (g_Error.IsSetAlarm(ERR_TRAY_PP_1_AUTO_SKIP_PICK_FAIL_DEVICE_REMAIN) == TRUE || bNeedAlarm == true)
// 						break;
// 				}

				

				int nErrTrayNotExist = g_TaskTransfer.GetTrayExist_DvcMode(DEF_NOT_EXIST);
				bool bScheduleExist             = GetScheduleExist();
				BOOL bStackerUnloadTransHolding = ChkStackerUnloadTransHolding();
				int tray_row_step = g_TaskTrayFeeder.GetTrayDeviceExistRowStep();

				if( (nErrTrayNotExist != ERR_NO_ERROR || bScheduleExist == true)/*&& tray_row_step >= 0*//*!= -1*/) {
					// tray 있을 경우 작업 영역 확인.
					// 작업 가능한 tray 확인 될 경우 

					MakeLog( _T( "[MainStep=%s] [SubStep=%s] nErrTrayNotExist : %d / bScheduleExist : %d" )
							 , GetLotStateToString( m_nAutoRunMainStep ), GetExcuteStateStr()
							 , nErrTrayNotExist, bScheduleExist );

					ChangeSubState( eExcutingStep_Check_Work_Area_Enable );
					break;
				}
				else {
					bool work_Transfer_Feeder_enable = g_Transfer_Feeder_work_area.reserve_work_area(this);
					bool work_enable = g_conveyor_work_area.reserve_work_area( this );
					if( work_enable == true  && g_TaskTransfer.is_safety_state() == true )
						g_conveyor_work_area.release_work_area( this );
					if(work_Transfer_Feeder_enable == true  && g_TaskTransfer.is_safety_state() == true)
						g_Transfer_Feeder_work_area.release_work_area(this);
				}
			}break;

		case eExcutingStep_Check_Work_Area_Enable:
			{
				// source, destination 값 있어야 됨.
				int src_conveyor_idx  = g_DMCont.m_dmTransfer.GN( NDM9_Transfer_source_conveyor_index );
				int dest_conveyor_idx = g_DMCont.m_dmTransfer.GN( NDM9_Transfer_destination_conveyor_index );
				if( src_conveyor_idx == DEF_UNK || dest_conveyor_idx == DEF_UNK ) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( _T( "Src:%d -> Dest:%d" ), src_conveyor_idx, dest_conveyor_idx );
					g_Error.AlarmReport( ERR_TRNASFER_SOURCE_OR_DESTINATION_CONVEYOR_INDEX_UNKNOWN, AlarmData, this );
					g_TaskSystemCtrl.OnLotCommand( eAutoLotCmd_AlarmPause, NULL, NULL );
					ChangeMainState( eAutoLotState_AlarmStop );
					break;
				}

				bool work_enable = g_conveyor_work_area.reserve_work_area( this );
				bool work_Transfer_Feeder_enable = g_Transfer_Feeder_work_area.reserve_work_area(this);
				if( work_enable == true && work_Transfer_Feeder_enable == true) {
					ChangeSubState( eExcutingStep_MovePick_Src_Conveyor_Pos );
					break;
				}
			}break;

		case eExcutingStep_MovePick_Src_Conveyor_Pos:
			{
				int nErrTrayNotExist = g_TaskTransfer.GetTrayExist_DvcMode( DEF_NOT_EXIST );
				if( nErrTrayNotExist != ERR_NO_ERROR ) {
					// tray 있을 경우 pick skip하고 place 하도록.
					ChangeSubState( eExcutingStep_WaitEnd_MovePick_Src_Conveyor_Pos );
					break;
				}

				int src_conveyor_idx = g_DMCont.m_dmTransfer.GN( NDM9_Transfer_source_conveyor_index );
				CTransferCmd_MovePickPlace* pSeqCmd = new CTransferCmd_MovePickPlace( src_conveyor_idx, DEF_PICK );
				if( SendCommand( g_TaskTransfer, pSeqCmd ) == eSeqCmdAck_OK ) {
					MakeLog("Src stacker = %d", src_conveyor_idx);
					ChangeSubState( eExcutingStep_WaitEnd_MovePick_Src_Conveyor_Pos );
					break;
				}

			}break;

		case eExcutingStep_WaitEnd_MovePick_Src_Conveyor_Pos:
			{
				if( g_TaskTransfer.GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready ) {
					ChangeSubState( eExcutingStep_MovePlace_Dest_Conveyor_Pos );
					break;
				}
			}break;

		case eExcutingStep_MovePlace_Dest_Conveyor_Pos:
			{
				int dest_conveyor_idx = g_DMCont.m_dmTransfer.GN( NDM9_Transfer_destination_conveyor_index );
				CTransferCmd_MovePickPlace* pSeqCmd = new CTransferCmd_MovePickPlace( dest_conveyor_idx, DEF_PLACE );
				if( SendCommand( g_TaskTransfer, pSeqCmd ) == eSeqCmdAck_OK ) {
					MakeLog("Dest stacker = %d", dest_conveyor_idx);
					ChangeSubState( eExcutingStep_WaitEnd_MovePlace_Dest_Conveyor_Pos );
					break;
				}
			}break;

		case eExcutingStep_WaitEnd_MovePlace_Dest_Conveyor_Pos:
			{
				if( g_TaskTransfer.GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready ) {
					g_DMCont.m_dmTransfer.SN( NDM9_Transfer_source_conveyor_index, DEF_UNK );
					g_DMCont.m_dmTransfer.SN( NDM9_Transfer_destination_conveyor_index, DEF_UNK );

					ChangeSubState( eExcutingStep_Move_SafetyPos );
					break;
				}
			}break;

		case eExcutingStep_Move_SafetyPos:
			{
				int nErrTrayNotExist = g_TaskTransfer.GetTrayExist_DvcMode( DEF_NOT_EXIST );
				bool bScheduleExist = GetScheduleExist();
				if( nErrTrayNotExist != ERR_NO_ERROR && bScheduleExist == true ) {
					// tray 있을 경우 작업 영역 확인.
					// 작업 가능한 tray 확인 될 경우 

					MakeLog( _T( "[MainStep=%s] [SubStep=%s] nErrTrayNotExist : %d / bScheduleExist : %d" )
							 , GetLotStateToString( m_nAutoRunMainStep ), GetExcuteStateStr()
							 , nErrTrayNotExist, bScheduleExist );

					ChangeSubState( eExcutingStep_Check_Work_Area_Enable );
					break;
				}

				int nReadyUnload = 0;
				for (std::vector<CTaskAutoStacker*>::size_type i = 0; i < g_unload_stackers.size(); i++)
				{
					int nIdx = g_unload_stackers[i]->GetStackerIndex();
					auto conv_state = g_TaskAutoStacker[nIdx].GetConveyorState();
					if (conv_state == PlaceEnable_Device)
						nReadyUnload++;				
				}
				if (nReadyUnload == (int)g_unload_stackers.size())
				{
					CTransferCmd_MovePos* pSeqCmd = new CTransferCmd_MovePos(LOC_IDX_TRANSFER_SAFETY);
					if (SendCommand(g_TaskTransfer, pSeqCmd) == eSeqCmdAck_OK) {
						ChangeSubState(eExcutingStep_WaitEnd_Move_SafetyPos);
						break;
					}
				}

				if (nReadyUnload != (int)g_unload_stackers.size() && g_TaskSystemCtrl.IsOneCycleModeOn() != true)
				{
					CTransferCmd_MovePos* pSeqCmd = new CTransferCmd_MovePos(LOC_IDX_TRANSFER_CONV2);
					if (SendCommand(g_TaskTransfer, pSeqCmd) == eSeqCmdAck_OK) {
						ChangeSubState(eExcutingStep_WaitEnd_Move_UnloadEmptyPos);
						break;
					}
				}
				else {
					CTransferCmd_MovePos* pSeqCmd = new CTransferCmd_MovePos(LOC_IDX_TRANSFER_SAFETY);
					if (SendCommand(g_TaskTransfer, pSeqCmd) == eSeqCmdAck_OK) {
						ChangeSubState(eExcutingStep_WaitEnd_Move_SafetyPos);
						break;
					}
				}

			}break;
		case eExcutingStep_WaitEnd_Move_SafetyPos:
			{
				if( g_TaskTransfer.GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready ) {
					g_conveyor_work_area.release_work_area( this );
					ChangeSubState( eExcutingStep_Start );
					break;
				}
			}break;
		case eExcutingStep_WaitEnd_Move_UnloadEmptyPos:
		{
			if (g_TaskTransfer.GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready) {
				g_conveyor_work_area.release_work_area(this);
				ChangeSubState(eExcutingStep_Start);
				break;
			}
		}break;

		case eExcutingStep_End:
			{
				ChangeMainState( eAutoLotState_NormalComplete );
			}break;
	}
	m_nLast_Excuting_Step = m_nAutoRunSubStep;
}

void CTaskAutoTransfer::AutoState_NormalComplete()
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

void CTaskAutoTransfer::AutoState_AlarmStop()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
		case 7000:
			{
				g_TaskTransfer.SetProcessCommand(base_seq_cmd::eProcCmd_Pause);
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

void CTaskAutoTransfer::AutoState_Resume()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		//MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
		MakeLog(_T("[MainStep=%s] [SubStep=%d] [last step=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nLast_Excuting_Step);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
		case 8000:
			{
				g_TaskTransfer.SetProcessCommand( eProcCmd_Resume );

				// 작업 중이냐? 작업 중이지 않느냐에 따라서 recovery가 달라져야함.
				if( g_TaskTransfer.GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready )
				{
					ChangeSubState( 8002 );
				}
				else { // 작업 중이면 일하도록 시킨다.

					ChangeSubState( 8001 );
				}

				//m_swTimeout.SetCheckTime(TIME_OUT_MIDDLE);
				//m_swTimeout.StartTimer();				
				//ChangeSubState( 8001 );
			}break;
		case 8001:
			{
				if( g_TaskTransfer.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready )
					break;
				//if (m_swTimeout.CheckOverTime() == TRUE) {
				//	CParamList AlarmData;
				//	g_Error.AlarmReport(ERR_TRNASFER_WAITTING_TRAYPP_MOVE_SAFETY_POS_TIMEOUT, AlarmData, this);
				//	ChangeMainState(eAutoLotState_AlarmStop);
				//	break;
				//}

				ChangeSubState( 8005 );
			}break;
		case 8002:
			{
				//if (g_TaskTransfer.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
				//	break;

				// tray pp 안전 영역 이동 확인 후 
				// transfer 안전 위치 이동 시켜야 함.
		
				bool bTrayPPSafety = g_TaskTrayUnloadPP.is_safety_state();
				bool bTrayFeederSafety = g_TaskTrayFeeder.is_safety_state();
				
				//bool bTrayPPSeqReady = g_TaskTrayPP[m_eTrayIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready;
				if (bTrayPPSafety != true || bTrayFeederSafety != true)//|| bTrayPPSeqReady != true)
					break;

				
				if (g_TaskTransfer.is_safety_state() == true)
				{
					ChangeSubState(8003);
					break;
				}
		

				CTransferCmd_MovePos* pSeqCmd = new CTransferCmd_MovePos( LOC_IDX_TRANSFER_SAFETY );
				if( SendCommand( g_TaskTransfer, pSeqCmd ) == eSeqCmdAck_OK ) {
					ChangeSubState( 8003 );
				}
			}break;
		case 8003:
			{
				// transfer 안전 영역 이동 확인.
				if( g_TaskTransfer.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready )
					break;

				ChangeSubState( 8004 );
			}break;
		case 8004:
			{
				if (g_TaskTrayUnloadPP.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
					break;

				//ChangeMainState( eAutoLotState_Executing );
				int nEndFeed = g_TaskAllAutoStacker.GetTrayEndFeed();
				switch( nEndFeed )
				{
					case DEF_LOTEND_TRAYEND:
					case DEF_LOTEND_TRAYFEED:
						{
							ChangeMainState( eAutoLotState_CleanOut );
						}break;
					default: ChangeMainState( eAutoLotState_Executing, m_nLast_Excuting_Step); break;
				}
			}break;
		default:
			ChangeSubState( 8000 );
			break;
	}
}

void CTaskAutoTransfer::AutoState_CleanOut()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
		case 9000:
			{
				ChangeSubState( 9999 );
			}break;
		case 9999:
			{
				ChangeMainState( eAutoLotState_NormalComplete );
			}break;
		default:
			ChangeSubState( 9000 );
			break;
	}
}

bool CTaskAutoTransfer::GetScheduleExist()
{
	// Load Setplace에서 작업 할 일이 있는지 확인
	auto iter_pick_enable_load_conveyor =
		std::find_if( g_load_stackers.begin(), g_load_stackers.end()
					  , [] ( CTaskAutoStacker* _load ) -> bool
	{
		if( _load->GetConveyorState() == ReadyToUnload_Tray )
			return true;

		return false;
	} );
	
	// Load Empty Setplace에서 작업 할 일이 있는지 확인
	auto iter_place_enable_empty_conveyor =
		std::find_if( g_empty_load_stackers.begin(), g_empty_load_stackers.end()
					  , [] ( CTaskAutoStacker* _empty ) -> bool
	{
		int stacker_idx = _empty->GetStackerIndex();
		if( _empty->GetConveyorState() == ReadyToLoad_Tray && g_TaskStacker[ stacker_idx ].ChkFullTray( DEF_ON ) == true )
		{
			return true;
		}

		return false;
	} );

	if( g_empty_load_stackers.size() == 0 && iter_place_enable_empty_conveyor == g_empty_load_stackers.end() )
	{
		iter_place_enable_empty_conveyor =
			std::find_if( g_empty_unload_stackers.begin(), g_empty_unload_stackers.end()
						  , [] ( CTaskAutoStacker* _empty ) -> bool
		{
			int stacker_idx = _empty->GetStackerIndex();
			if( _empty->GetConveyorState() == ReadyToLoad_Tray && g_TaskStacker[ stacker_idx ].ChkFullTray( DEF_ON ) == true )
			{
				return true;
			}

			return false;
		} );
	}

	if( iter_pick_enable_load_conveyor != g_load_stackers.end() && iter_place_enable_empty_conveyor != g_empty_load_stackers.end() ) {
		int src_idx = ( *iter_pick_enable_load_conveyor )->GetStackerIndex();
		int dest_idx = ( *iter_place_enable_empty_conveyor )->GetStackerIndex();
		g_DMCont.m_dmTransfer.SN( NDM9_Transfer_source_conveyor_index, src_idx );
		g_DMCont.m_dmTransfer.SN( NDM9_Transfer_destination_conveyor_index, dest_idx );
		return true;
	}



	// Unload Empty Setplace에서 작업 할 일이 있는지 확인
	auto iter_pick_enable_empty_conveyor =
		std::find_if( g_empty_unload_stackers.begin(), g_empty_unload_stackers.end()
					  , [] ( CTaskAutoStacker* _empty ) -> bool
	{
		if( _empty->GetConveyorState() == ReadyToUnload_Tray ) {
			return true;
		}

		return false;
	} );

	if( iter_pick_enable_empty_conveyor == g_empty_unload_stackers.end() ) {
		return false;
	}

	// Unload Setplace에서 작업 할 일이 있는지 확인
	auto iter_place_enable_unload_conveyor =
		std::find_if( g_unload_stackers.begin(), g_unload_stackers.end()
					  , [] ( CTaskAutoStacker* _empty ) -> bool
	{
		int stacker_idx = _empty->GetStackerIndex();
		char szCompany[16] = { 0, };
		g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
	
		if (_empty->GetConveyorState() == ReadyToLoad_Tray || _empty->IsPrePickEmptyTrayPossible() == true)
		{
			return true;
		}
		return false;
	} );

	if( iter_place_enable_unload_conveyor != g_unload_stackers.end() ) {
		int src_idx  = ( *iter_pick_enable_empty_conveyor )->GetStackerIndex();
		int dest_idx = ( *iter_place_enable_unload_conveyor )->GetStackerIndex();
		g_DMCont.m_dmTransfer.SN( NDM9_Transfer_source_conveyor_index, src_idx );
		g_DMCont.m_dmTransfer.SN( NDM9_Transfer_destination_conveyor_index, dest_idx );
		return true;
	}

	return false;
}



// bool CTaskAutoTransfer::ChkRemainAutoSkipToDeviceAndAlarm()
// {
// 	//BOOL bAutoGrrUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);
// 	BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
// 	if (bAutoGrrUse == TRUE)
// 		return false;
// 
// 	BOOL bAutoSkip = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoSkipUse);
// 	if ( bAutoSkip == FALSE)
// 		return false;
// 
// 
// 	int nPocketCntX = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
// 	int nPocketCntY = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);
// 	ST_DD_DEVICE stDevice;
// 
// 	bool bFindAutoSkipDevice = false;
// 	//Stacker
// // 	std::for_each(g_load_stackers.begin(), g_load_stackers.end(),
// // 		[&](CTaskAutoStacker* _load_stacker)
// // 	{
// // 		if (_load_stacker->GetConveyorState() == ReadyToUnload)
// // 		{
// // 			int stacker_idx = _load_stacker->GetStackerIndex();
// // 
// // 			for (int j = 0; j < nPocketCntY; j++)
// // 			{
// // 				for (int i = 0; i < nPocketCntX; i++)
// // 				{
// // 					stDevice.clear();
// // 					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_STACKER_CONV_1 + stacker_idx, i, j, &stDevice);
// // 					if (stDevice.sExist == eDeviceData_None && strcmp(stDevice.szBinResult, DEF_TRAY_PICK_ERR_DEVICE) == 0)
// // 						bFindAutoSkipDevice = true;
// // 				}
// // 			}
// // 		}
// // 	});
// 	//Feeder
// 
// 	int nchk_feeder_tray_exist = g_TaskTrayFeeder.GetTrayExistSensor();
// 	bool bDeviceExist = g_DMCont.m_dmDevice.IsEmptyTray(eDeviceStage_TRAY_FEEDER) == FALSE;
// 	if (nchk_feeder_tray_exist == true && bDeviceExist == false)
// 	{
// 		for (int j = 0; j < nPocketCntY; j++)
// 		{
// 			for (int i = 0; i < nPocketCntX; i++)
// 			{
// 				stDevice.clear();
// 				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TRAY_FEEDER, i, j, &stDevice);
// 				if (stDevice.sExist == eDeviceData_None && strcmp(stDevice.szBinResult, DEF_TRAY_PICK_ERR_DEVICE) == 0)
// 					bFindAutoSkipDevice = true;
// 			}
// 		}
// 	}
// 
// 	return bFindAutoSkipDevice;
// }

/* [9/23/2021 donghyun.shin]
1. Test Site On 갯수가 Loading된 Device 갯수보다 커야 하고
2. OnCylce이 On이 되거나
3. 투입할 자재가 없거나
4. Loading Table에 Unload 할 Device가 있거나
5. Tray 갯수 보다 Loading 된 자재 가 많으면
6. Tray 자재가 없어서 Tray_END 하면
7. Auto Skip 으로 Load Empty 감지 되면

Unload 단에 Empty Stacker를 Transfer로 옮겨준다.
*/
BOOL CTaskAutoTransfer::ChkStackerUnloadTransHolding()
{
	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	int nDevLoadCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt);
	int nCntX = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
	int nCntY = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);
	int nFTRCQC = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	int m_nSlowUnloadStackerOption = g_DMCont.m_dmStacker.GN(NDM7_SLOW_UNLOAD_STACKER_OPTION);

	BOOL bLoadingBreak = g_TaskAllAutoStacker.GetLoadTraySupplyEnd();
	BOOL bSiteOnOff = 0;
	int nSiteOnOffCnt = 0;
	int nUnloadDeviceCnt = 0;
	int nDevExistCnt = 0;
	int nTrayToEnd = g_DMCont.m_dmTrayPP.GN(NDM5_TM_TrayPP_Tray_End);
	for (int nTblIdx = 0; nTblIdx < eMaxLoadTblCount; nTblIdx++)
	{
		nUnloadDeviceCnt += g_TaskLoadTbl[nTblIdx].GetUnloadDeviceCnt();
	}

	for (int i = 0; i < (STATION_MAX_PARA*eMaxPressUnitCount); i++) {
		bSiteOnOff = g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + i);
		if (bSiteOnOff == DEF_SITE_ON)
			nSiteOnOffCnt++;
	}

	bool bDeviceExist = g_DMCont.m_dmDevice.IsEmptyTray(eDeviceStage_STACKER_CONV_3) == FALSE;
	bool bStackerExist = g_TaskStacker[eSTACKER_01].GetEmptyTray() == true;
	
	if (nDevLoadCnt > 0)
	{
		if(bDeviceExist == false)
			return TRUE;
	}
		

	if (m_nSlowUnloadStackerOption == FALSE)
		return TRUE;

	if (nSiteOnOffCnt < nDevLoadCnt || g_TaskSystemCtrl.IsOneCycleModeOn() == TRUE
		|| bLoadingBreak == TRUE || nUnloadDeviceCnt > 0 || (nDevLoadCnt >= (nCntX*nCntY))
		|| nTrayToEnd == TRUE || bStackerExist == false)
	{
		return TRUE;
	}

	return FALSE;
}