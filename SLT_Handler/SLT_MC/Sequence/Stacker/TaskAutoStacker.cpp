
#include "stdafx.h"
#include "TaskAutoStacker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace base_seq_cmd;
using namespace std;

CTaskAutoStacker::CTaskAutoStacker( void )
{
	m_nAutoRunMainStep = eAutoLotState_InitNeed;
	m_nAutoRunSubStep = 0;
	m_nAutoRunBackUpMainStep = eAutoLotState_InitNeed;
	m_nAutoRunBackUpSubStep = 0;
	m_eStackerIndex = eSTACKER_01;
	m_nCurrentOccuredEventID = CEID_SEQ_EVENT_NONE;
	m_nCurrentVibCnt = 1;
	m_bPassConveyor = false;

	m_mapExcuteState.SetAt( eExcutingStep_Start, _T( "Start" ) );

	m_mapExcuteState.SetAt( eExcutingStep_TrayDown, _T( "TrayDown" ) );
	m_mapExcuteState.SetAt( eExcutingStep_WaitEnd_TrayDown, _T( "WaitEnd_TrayDown" ) );
	m_mapExcuteState.SetAt( eExcutingStep_Load, _T( "Load" ) );
	m_mapExcuteState.SetAt( eExcutingStep_WaitEnd_Load, _T( "WaitEnd_Load" ) );

	m_mapExcuteState.SetAt(eExcutingStep_ReadyTo_PlaceCenter, _T("ReadyTo_PlaceCenter"));
	m_mapExcuteState.SetAt(eExcutingStep_WaitEnd_PlaceCenter, _T("WaitEnd_PlaceCenter"));
	m_mapExcuteState.SetAt(eExcutingStep_ReadyTo_PickCenter, _T("ReadyTo_PickCenter"));
	m_mapExcuteState.SetAt(eExcutingStep_WaitEnd_PickCenter, _T("WaitEnd_PickCenter"));

	m_mapExcuteState.SetAt(eExcutingStep_Do_Vibration, _T("Vibration"));
	m_mapExcuteState.SetAt(eExcutingStep_WaitEnd_Vibration, _T("waitEnd_Vibration"));

	m_mapExcuteState.SetAt( eExcutingStep_Check_Unload, _T( "Check_Unload" ) );
	m_mapExcuteState.SetAt( eExcutingStep_Unload, _T( "Unload" ) );
	m_mapExcuteState.SetAt( eExcutingStep_WaitEnd_Unload, _T( "WaitEnd_Unload" ) );

	m_mapExcuteState.SetAt( eExcutingStep_Check_TrayUp, _T( "Check_TrayUp" ) );
	m_mapExcuteState.SetAt( eExcutingStep_TrayUp, _T( "TrayUp" ) );
	m_mapExcuteState.SetAt( eExcutingStep_WaitEnd_TrayUp, _T( "WaitEnd_TrayUp" ) );
	m_mapExcuteState.SetAt(eExcutingStep_FullTrayCheck, _T("FullTrayCheck"));
	m_mapExcuteState.SetAt(eExcutingStep_WaitEnd_FullTrayCheck, _T("WaitEnd_FullTrayCheck"));

	m_mapExcuteState.SetAt( eExcutingStep_End, _T( "End" ) );

	m_eCenterStageTransferState = eTransferState_Blocked;
	::InitializeCriticalSection(&m_csCenterStageTransferState);

}


CTaskAutoStacker::~CTaskAutoStacker( void )
{
	::DeleteCriticalSection(&m_csCenterStageTransferState);
}

int CTaskAutoStacker::Initialize( TCHAR* szDriverFileName, eStacker_Idx eStackerIndex, eStacker_Type eType )
{
	m_eStackerIndex = eStackerIndex;
	return CBaseTaskManager::Initialize();
}

void CTaskAutoStacker::Finalize()
{
	CBaseTaskManager::Finalize();
}

eStacker_Type CTaskAutoStacker::GetStackerType()
{
	return g_TaskStacker[m_eStackerIndex].GetStackerType();
}

int CTaskAutoStacker::OnEvent( const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList/*=NULL*/ )
{
	if( ( GetStackerType() == eStackerType_Empty_Unload )
		&& m_nAutoRunMainStep == eAutoLotState_Executing
		&& m_nAutoRunSubStep == eExcutingStep_Start )
	{
		if( nEventId == CEID_TRANSFER_REQUEST_EMPTY_TRAY ) {
			m_nCurrentOccuredEventID = CEID_TRANSFER_REQUEST_EMPTY_TRAY;
		}
	}

	return ERR_NO_ERROR;
}

int CTaskAutoStacker::OnCommandRsp( CBaseSeqCmd * pRspCmd, int nEventId, WPARAM wParam, LPARAM lParam, std::vector<CPoint>& vPocket_Picker )
{
	if( nEventId == CMD_EVT_ALARM )
	{
		if( pRspCmd->m_pSender == this )
			ChangeMainStateByRspAlarm();
	}

	return CBaseTaskManager::OnCommandRsp( pRspCmd, nEventId, wParam, lParam, vPocket_Picker );
}

void CTaskAutoStacker::OnOperatorCommand( int nOperatorCommand, WPARAM wParam, LPARAM lParam )
{
}

void CTaskAutoStacker::ThreadProc1()
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

void CTaskAutoStacker::MakeLog( LPCTSTR fmt, ... )
{
	TCHAR tmpMsg[ 1024 ] = { 0, };
	if( fmt )
	{
		va_list argList;
		va_start( argList, fmt );
		_vstprintf_s( tmpMsg, fmt, argList );
		va_end( argList );
	}
	TWLOG.MakeLog( static_cast< int >( eLogID_Auto_Stacker1 + m_eStackerIndex ), Debug, "", __LINE__, NULL, tmpMsg );
}

void CTaskAutoStacker::AutoState_Idle()
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

void CTaskAutoStacker::AutoState_InitNeed()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
		case 2000:
			{
				g_TaskStacker[ m_eStackerIndex ].SetProcessCommand( eProcCmd_Stop );
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

void CTaskAutoStacker::AutoState_Initialize()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
		case 3000:
			{
				m_nCurrentVibCnt = 1;



				ChangeSubState( 3001 );
			}break;
		case 3001:
			{
				CStackerCmd_Initial* pSeqCmd = new CStackerCmd_Initial();
				if( SendCommand( g_TaskStacker[ m_eStackerIndex ], pSeqCmd ) == eSeqCmdAck_OK ) {
					ChangeSubState( 3002 );
				}
				else {
					ChangeMainState( eAutoLotState_InitNeed );
				}
			}break;
		case 3002:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready ) {

					m_swTimeout.SetCheckTime( TIME_OUT_MIDDLE );
					m_swTimeout.StartTimer();
					ChangeSubState( 3100 );
				}
			}break;

		case 3100:
			{
				int nRearExist = g_TaskStacker[ m_eStackerIndex ].GetExistTray_RearPos_DvcMode( DEF_EXIST );

				if( m_swTimeout.CheckOverTime() == TRUE ) {
					CParamList AlarmData;
					g_Error.AlarmReport( nRearExist == ERR_NO_ERROR ? ERR_STACKER_1_TRAY_ALIGN_FWD_TIMEOUT + m_eStackerIndex : ERR_STACKER_1_TRAY_ALIGN_BKWD_TIMEOUT + m_eStackerIndex, AlarmData, this );
					g_TaskSystemCtrl.OnLotCommand( eAutoLotCmd_AlarmPause, NULL, NULL );
					ChangeMainState( eAutoLotState_InitNeed );
					break;
				}

				g_TaskStacker[ m_eStackerIndex ].do_cyl_TrayAlign( nRearExist == ERR_NO_ERROR ? DEF_FORW : DEF_BACK );
				int nAlignErr = g_TaskStacker[ m_eStackerIndex ].chk_cyl_TrayAlign( nRearExist == ERR_NO_ERROR ? DEF_FORW : DEF_BACK );
				if( nAlignErr == ERR_NO_ERROR ) {
					ChangeSubState( 3999 );
					break;
				}
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

void CTaskAutoStacker::AutoState_Pause()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d] [last step=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nLast_Excuting_Step);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
		case 4000:
			{
				g_TaskStacker[ m_eStackerIndex ].SetProcessCommand( eProcCmd_Pause );
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

void CTaskAutoStacker::AutoState_Execute()
{
	eStacker_Type stacker_type = GetStackerType();

	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		if ((stacker_type != eStackerType_Load))
		{
			MakeLog(_T("[MainStep=%s] [SubStep=%s]"), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr());
		}
			
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	if( m_nAutoRunSubStep > eExcutingStep_End ) {
		m_nAutoRunSubStep = m_nLast_Excuting_Step;
	}
	m_nLast_Excuting_Step = m_nAutoRunSubStep;
	
	switch( stacker_type )
	{
		case eStackerType_Load:	this->AutoState_Execute_LoadStacker(); break;
		case eStackerType_Unload:this->AutoState_Execute_UnloadStacker(); break;
		case eStackerType_Empty_Unload: this->AutoState_Execute_EmptyUnloadStacker(); break;
		case eStackerType_Empty_Load: this->AutoState_Execute_EmptyLoadStacker(); break;
		default:
			ASSERT( false );
			break;
	}
	m_nLast_Excuting_Step = m_nAutoRunSubStep;
}

void CTaskAutoStacker::AutoState_Execute_LoadStacker()
{
	switch( m_nAutoRunSubStep )
	{
		case eExcutingStep_Start:
			{
			if (g_TaskStacker[m_eStackerIndex].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready
				|| g_Error.IsSetAlarm(ERR_LOT_MSG_REQ_ADD_LOAD_TRAY) == TRUE
				|| g_Error.IsSetAlarm(ERR_LOT_CMD_LOAD_TRAY_IS_SHORTAGE) == TRUE) {
					break;
				}	
				

				int  nDeviceMode        = g_DMCont.m_dmHandlerOpt.GN( NDM11_SYS_MD_DEVICE_CHK );
				bool bEmptyTray         = g_TaskStacker[m_eStackerIndex].GetEmptyTray();
				int  nSenSts            = g_TaskStacker[m_eStackerIndex].GetLoadTrayStatus();
				
				int  nCenterNotExistErr = g_TaskTrayFeeder.GetTrayExist_TransferArea_DvcMode(eNot_Exist);
				int  nFrontNotExistErr  = g_TaskStacker[m_eStackerIndex].GetExistTray_FrontPos_DvcMode(DEF_NOT_EXIST);
				int  nRearNotExistErr   = g_TaskStacker[m_eStackerIndex].GetExistTray_RearPos_DvcMode(DEF_NOT_EXIST);
				bool bLoadTraySupplyEnd = g_TaskAllAutoStacker.GetLoadTraySupplyEnd();
				BOOL bAutoGrrUse        = g_TaskSystemCtrl.GetAutoGrrUse();
				BOOL bAutoLoopUse       = g_TaskSystemCtrl.GetAutoLoopUse();

				int nFTRCQC = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
				
				if ( bAutoGrrUse == TRUE || bAutoLoopUse == TRUE || (nFTRCQC == eStartMD_QA || nFTRCQC == eStartMD_QA_Continue))
				{
					int nCurLotLoadDvcCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt);   // 현재 Lot

					if( nCurLotLoadDvcCnt > 0 || g_TaskTrayLoadPP.GetTrayPpLoadDvcCnt() > 0 || nRearNotExistErr != ERR_NO_ERROR || nFrontNotExistErr != ERR_NO_ERROR )
					{
						g_TaskAllAutoStacker.SetLoadTraySupplyEnd(true); // 1장이게됨.
						bLoadTraySupplyEnd = true;
					}						
				}

				if( ( (bEmptyTray == false && nSenSts == CDelaySensor::eDelaySenSts_On ) || nDeviceMode == eNoDeviceNoTray )
					&& nFrontNotExistErr == ERR_NO_ERROR
					&& bLoadTraySupplyEnd == false )
				{
					if (bAutoGrrUse == TRUE || (nFTRCQC == eStartMD_QA || nFTRCQC == eStartMD_QA_Continue))
						g_TaskAllAutoStacker.SetLoadTraySupplyEnd(true); // 1장이게됨.

					ChangeSubState( eExcutingStep_TrayDown );
					break;
				}

				if( nFrontNotExistErr != ERR_NO_ERROR && nRearNotExistErr == ERR_NO_ERROR && nCenterNotExistErr == ERR_NO_ERROR ) {
					ChangeSubState( eExcutingStep_Load );
					break;
				}

// 				// Load Stacker 공급이 끝났는대, Load Stacker가 감지 되고 있으면 알람 발생 한다.
// 				if (g_TaskAllAutoStacker.GetLoadTraySupplyEnd() != true && g_TaskStacker[m_eStackerIndex].GetEmptyTray() == false)
// 				{
// 					CParamList AlarmList;
// 					g_Error.AlarmReport(ERR_LOT_SUPPLY_QUANTITY_CANCLE_STACKER3_TRAY_EXIST, AlarmList, this);
// 				}
				// feeder가 tray 1장을 들고 작업을 하고 있고 load side에는 tray가 1장 준비되어 있으며 
				// front side에는 tray를 1장 내려 놓은 상태라면 feeder가 tray index 작업 완료 후 cv에 내려 놓을 때까지 기다린다.
				bool bReadyToPlaceCondition = check_condition_center_stage_ready_to_place();
				if (bReadyToPlaceCondition)
				{
					SetCenterStageTransferState(eTransferState_ReadyToLoad);
					ChangeSubState(eExcutingStep_ReadyTo_PlaceCenter);
					break;
				}
				
			
				if ( ((nFrontNotExistErr == ERR_NO_ERROR && nRearNotExistErr == ERR_NO_ERROR && bLoadTraySupplyEnd == false && nSenSts == CDelaySensor::eDelaySenSts_Off)
					|| ((bEmptyTray == false && nSenSts != CDelaySensor::eDelaySenSts_On) && nFrontNotExistErr == ERR_NO_ERROR && bLoadTraySupplyEnd == false ))
					&& nDeviceMode != eNoDeviceNoTray)
				{
					if (g_Error.IsSetAlarm(ERR_LOT_CMD_LOAD_TRAY_IS_SHORTAGE) == FALSE)
					{
						MakeLog(_T("DelaySensts = %d "), nSenSts);
						CParamList AlarmList;
						g_Error.AlarmReport(ERR_LOT_CMD_LOAD_TRAY_IS_SHORTAGE, AlarmList, this);
					}
				}



			}break;

		case eExcutingStep_TrayDown:
			{
				CStackerCmd_TrayDn* pSeqCmd = new CStackerCmd_TrayDn();
				if( SendCommand( g_TaskStacker[ m_eStackerIndex ], pSeqCmd ) == eSeqCmdAck_OK ) {
					ChangeSubState( eExcutingStep_WaitEnd_TrayDown );
					break;
				}
			}break;

		case eExcutingStep_WaitEnd_TrayDown:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready ) {
					break;
				}

				int  nCenterNotExistErr = g_TaskTrayFeeder.GetTrayExist_TransferArea_DvcMode(eNot_Exist);
				int nRearErr  = g_TaskStacker[ m_eStackerIndex ].GetExistTray_RearPos_DvcMode( DEF_NOT_EXIST );
				int nFrontErr = g_TaskStacker[ m_eStackerIndex ].GetExistTray_FrontPos_DvcMode( DEF_EXIST );
				if( nRearErr == ERR_NO_ERROR && nFrontErr == ERR_NO_ERROR && nCenterNotExistErr == ERR_NO_ERROR) {
					ChangeSubState( eExcutingStep_Load );
					break;
				}
				else {
					ChangeSubState( eExcutingStep_Start );
					break;
				}
			}break;

		case eExcutingStep_Load:
			{
				int transfer_work_conv_idx = g_TaskTransfer.GetSeqWorkCmdStage();
				if (g_TaskTransfer.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready && transfer_work_conv_idx == m_eStackerIndex) {
					break;
				}

				CStackerCmd_Load* pSeqCmd = new CStackerCmd_Load( true );
				if( SendCommand( g_TaskStacker[ m_eStackerIndex ], pSeqCmd ) == eSeqCmdAck_OK ) {
					ChangeSubState( eExcutingStep_WaitEnd_Load );
					break;
				}
			}break;

		case eExcutingStep_WaitEnd_Load:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready ) {
					ChangeSubState( eExcutingStep_Start );
					break;
				}
			}break;

		case eExcutingStep_ReadyTo_PlaceCenter:
		{
			bool bWaitTrayPlace = check_condition_center_stage_tray_place();
			if (bWaitTrayPlace == true)
			{
				ChangeSubState(eExcutingStep_WaitEnd_PlaceCenter);
				break;
			}
		}break;

		case eExcutingStep_WaitEnd_PlaceCenter:
		{
			if (g_TaskTrayFeeder.GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready) {
				SetCenterStageTransferState(eTransferState_ReadyToUnload);
				ChangeSubState(eExcutingStep_ReadyTo_PickCenter);
				break;
			}
		}break;

		case eExcutingStep_ReadyTo_PickCenter:
		{
			bool bWaitTrayRemov = check_condition_center_stage_tray_remove();
			if(bWaitTrayRemov == true )
			{
				ChangeSubState(eExcutingStep_Start);
				break;
			}
		}break;

		case eExcutingStep_WaitEnd_PickCenter:
		{
			if (g_TaskTransfer.GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready) {
				SetCenterStageTransferState(eTransferState_Blocked);
				ChangeSubState(eExcutingStep_Start);
				break;
			}
		}break;

		case eExcutingStep_End:
			{
				ChangeMainState( eAutoLotState_NormalComplete );
			}break;
	}
}

void CTaskAutoStacker::AutoState_Execute_UnloadStacker()
{
	switch( m_nAutoRunSubStep )
	{
		case eExcutingStep_Start:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready ) {
					break;
				}

				bool bVibrationWork = check_vibration_work_condition();
				if( bVibrationWork == true ) {
					ChangeSubState( eExcutingStep_Do_Vibration );
					break;
				}

				bool bDeviceFullTray = ConveyorIsFullTray();

				bool bFinish = g_TaskSystemCtrl.IsRunningFinish();
				
				if( bDeviceFullTray == true && bFinish != true) {
					ChangeSubState( eExcutingStep_Unload );
					break;
				}
			}break;

		case eExcutingStep_Do_Vibration:
			{
				CTaskTrayBasePP* pTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
				for (int i = 0; i < eMaxTrayPPCount; i++)
				{
					if (pTrayPP[i]->GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
						break;
				}
			

				if (g_TaskStacker[m_eStackerIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
				{
					double delay_time_sec = g_DMCont.m_dmConveyor.GD(DDM6_VIBRATOR_ACT_DELAY_SEC);;
					int act_times = g_DMCont.m_dmConveyor.GN(NDM6_VIBRATOR_ACT_TIMES);
					CStackerCmd_Vibration* pSeqCmd = new CStackerCmd_Vibration(delay_time_sec, act_times);
					if (SendCommand(g_TaskStacker[m_eStackerIndex], pSeqCmd) == eSeqCmdAck_OK) {
						ChangeSubState(eExcutingStep_WaitEnd_Vibration);
						break;
					}
				}				
			}break;
		case eExcutingStep_WaitEnd_Vibration:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready ) {
					ChangeSubState( eExcutingStep_Start );
					break;
				}
			}break;

		case eExcutingStep_Unload:
			{
				if (g_TaskStacker[m_eStackerIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
				{
					CStackerCmd_Unload* pSeqCmd = new CStackerCmd_Unload(true);
					if (SendCommand(g_TaskStacker[m_eStackerIndex], pSeqCmd) == eSeqCmdAck_OK) {
						m_nCurrentVibCnt = 1;
						ChangeSubState(eExcutingStep_WaitEnd_Unload);
						break;
					}
				}				
			}break;
		case eExcutingStep_WaitEnd_Unload:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready ) {
					ChangeSubState( eExcutingStep_Check_TrayUp );
					break;
				}
			}break;
		case eExcutingStep_Check_TrayUp:
			{
				//int nErr = g_TaskStacker[ m_eStackerIndex ].GetExistTray_FrontPos_DvcMode( DEF_EXIST );
				//if( nErr == ERR_NO_ERROR ) {
					ChangeSubState( eExcutingStep_TrayUp );
				//	break;
				//}
			}break;
		case eExcutingStep_TrayUp:
			{
				if (g_TaskStacker[m_eStackerIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
				{
					int nErr = g_TaskStacker[m_eStackerIndex].GetExistTray_FrontPos_DvcMode(DEF_EXIST);
					if (nErr == ERR_NO_ERROR) {
						CStackerCmd_TrayUp* pSeqCmd = new CStackerCmd_TrayUp(true);
						if (SendCommand(g_TaskStacker[m_eStackerIndex], pSeqCmd) == eSeqCmdAck_OK) {
							ChangeSubState(eExcutingStep_WaitEnd_TrayUp);
							break;
						}
					}					
				}				
			}break;
		case eExcutingStep_WaitEnd_TrayUp:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready ) {
					ChangeSubState(eExcutingStep_FullTrayCheck);
					break;
				}
			}break;
		case eExcutingStep_FullTrayCheck:
		{
			if (g_TaskStacker[m_eStackerIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready) 
			{
				CStackerCmd_FullTray* pSeqCmd = new CStackerCmd_FullTray();
				if (SendCommand(g_TaskStacker[m_eStackerIndex], pSeqCmd) == eSeqCmdAck_OK) 
				{
					ChangeSubState(eExcutingStep_WaitEnd_FullTrayCheck);
					break;
				}
			}	
		}break;
		case eExcutingStep_WaitEnd_FullTrayCheck:
		{
			if (g_TaskStacker[m_eStackerIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready) {
				ChangeSubState(eExcutingStep_Start);
				break;
			}
		}break;


		case eExcutingStep_End:
			{
				ChangeMainState( eAutoLotState_NormalComplete );
			}break;
	}
}

void CTaskAutoStacker::AutoState_Execute_EmptyLoadStacker()
{
	switch( m_nAutoRunSubStep )
	{
		case eExcutingStep_Start:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready ) {
					break;
				}

				int nErr_TrayFrontExist = g_TaskStacker[ m_eStackerIndex ].GetExistTray_FrontPos_DvcMode( DEF_EXIST );
				if( nErr_TrayFrontExist == ERR_NO_ERROR ) {
					ChangeSubState( eExcutingStep_TrayUp );
					break;
				}


				// transfer가 load 끝난 tray 내려놓고 다시 올라가면 동작 한다.
				int nErr_TrayRearExist     = g_TaskStacker[ m_eStackerIndex ].GetExistTray_RearPos_DvcMode( DEF_EXIST );
				int nErr_TrayFrontNotExist = g_TaskStacker[ m_eStackerIndex ].GetExistTray_FrontPos_DvcMode( DEF_NOT_EXIST );
				int nErr_TransferUp        = g_TaskTransfer.get_cyl_UpDown( DEF_UP );

				if( nErr_TrayRearExist == ERR_NO_ERROR
					&& nErr_TrayFrontNotExist == ERR_NO_ERROR
					&& nErr_TransferUp == ERR_NO_ERROR 
					&&  g_TaskTransfer.GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)// 0318 SJI Transfer 가 확인도 하지 않았는데 작업 진행 
				{
					ChangeSubState( eExcutingStep_Unload );
					break;
				}

			}break;

		case eExcutingStep_Unload:
			{
				int transfer_work_conv_idx = g_TaskTransfer.GetSeqWorkCmdStage();
				if (g_TaskTransfer.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready && transfer_work_conv_idx == m_eStackerIndex) {
					break;
				}

				if (g_TaskStacker[m_eStackerIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
				{
					CStackerCmd_Unload* pSeqCmd = new CStackerCmd_Unload(false);
					if (SendCommand(g_TaskStacker[m_eStackerIndex], pSeqCmd) == eSeqCmdAck_OK) {
						ChangeSubState(eExcutingStep_WaitEnd_Unload);
						break;
					}
				}
			}break;

		case eExcutingStep_WaitEnd_Unload:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready ) {
					ChangeSubState( eExcutingStep_TrayUp );
					break;
				}
			}break;

		case eExcutingStep_TrayUp:
			{
				if (g_TaskStacker[m_eStackerIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
				{
					int nErr = g_TaskStacker[m_eStackerIndex].GetExistTray_FrontPos_DvcMode(DEF_EXIST);
					if (nErr == ERR_NO_ERROR) {
						CStackerCmd_TrayUp* pSeqCmd = new CStackerCmd_TrayUp(true);
						if (SendCommand(g_TaskStacker[m_eStackerIndex], pSeqCmd) == eSeqCmdAck_OK) {
							ChangeSubState(eExcutingStep_WaitEnd_TrayUp);
							break;
						}
					}
				}
			}break;
		case eExcutingStep_WaitEnd_TrayUp:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready ) {
					ChangeSubState(eExcutingStep_FullTrayCheck);
					break;
				}
			}break;
		case eExcutingStep_FullTrayCheck:
		{
			if (g_TaskStacker[m_eStackerIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				CStackerCmd_FullTray* pSeqCmd = new CStackerCmd_FullTray();
				if (SendCommand(g_TaskStacker[m_eStackerIndex], pSeqCmd) == eSeqCmdAck_OK)
				{
					ChangeSubState(eExcutingStep_WaitEnd_FullTrayCheck);
					break;
				}
			}
		}break;
		case eExcutingStep_WaitEnd_FullTrayCheck:
		{
			if (g_TaskStacker[m_eStackerIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready) {
				ChangeSubState(eExcutingStep_Start);
				break;
			}
		}break;

		case eExcutingStep_End:
			{
				ChangeMainState( eAutoLotState_NormalComplete );
			}break;
	}
}

void CTaskAutoStacker::AutoState_Execute_EmptyUnloadStacker()
{
	switch( m_nAutoRunSubStep )
	{
		case eExcutingStep_Start:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready ) {
					break;
				}

				int nRearErr = g_TaskStacker[ m_eStackerIndex ].GetExistTray_RearPos_DvcMode( DEF_NOT_EXIST );
				int nFrontErr = g_TaskStacker[ m_eStackerIndex ].GetExistTray_FrontPos_DvcMode( DEF_NOT_EXIST );
				bool bEmptyTray = g_TaskStacker[m_eStackerIndex].GetEmptyTray();
				int nSenSts = g_TaskStacker[m_eStackerIndex].GetLoadTrayStatus();
				int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
				int nExistTray = g_DMCont.m_dmDevice.GetTrayExist(eDeviceDataStage::eDeviceStage_STACKER_CONV_1 + m_eStackerIndex);

				
				// transfer가 OnEvent 함수로 empty tray request하면 eExcutingStep_TrayDown 시작한다.
				if( nRearErr == ERR_NO_ERROR && nFrontErr == ERR_NO_ERROR && ((bEmptyTray == false && nSenSts == CDelaySensor::eDelaySenSts_On) || nDeviceMode == eNoDeviceNoTray)) {
					m_nCurrentOccuredEventID = CEID_SEQ_EVENT_NONE;
					ChangeSubState( eExcutingStep_TrayDown );
					break;
				}

				if ( (nSenSts == CDelaySensor::eDelaySenSts_Off 
					 || (nRearErr == ERR_NO_ERROR && nFrontErr == ERR_NO_ERROR && (bEmptyTray == false && nSenSts != CDelaySensor::eDelaySenSts_On))) //Tray Down 안되는 현상 때문
					&& nDeviceMode != eNoDeviceNoTray) {
					if (g_Error.IsSetAlarm(ERR_LOT_CMD_EMPTY_TRAY_IS_SHORTAGE) == FALSE)
					{
						MakeLog(_T("DelaySensts = %d "), nSenSts);
						CParamList AlarmList;
						g_Error.AlarmReport(ERR_LOT_CMD_EMPTY_TRAY_IS_SHORTAGE, AlarmList, this);
					}
				}

				if (nFrontErr != ERR_NO_ERROR && nRearErr == ERR_NO_ERROR && nExistTray != DEF_EXIST) {
					ChangeSubState(eExcutingStep_Load);
					MakeLog(_T("eExcutingStep_Start -> eExcutingStep_Load "));
					break;
				}



				//// LotEnd 되었는데, Tray 나와있으면, 다시 stacker 쪽으로 unload 해야 한다.
				//bool bLotEnd = g_TaskAllAutoStacker.GetTrayEndFeed() == DEF_LOTEND_TRAYFEED;
				//if( bLotEnd == true ) {
				//	if( nRearErr != ERR_NO_ERROR || nFrontErr != ERR_NO_ERROR )
				//	{
				//		ChangeSubState( eExcutingStep_Unload );
				//		break;
				//	}
				//	else {
				//		ChangeSubState( eExcutingStep_End );
				//		break;
				//	}
				//}

			}break;

		case eExcutingStep_TrayDown:
			{
				if (g_TaskStacker[m_eStackerIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
				{
					int nFrontErr = g_TaskStacker[m_eStackerIndex].GetExistTray_FrontPos_DvcMode(DEF_NOT_EXIST);
					if (nFrontErr == ERR_NO_ERROR)
					{
						CStackerCmd_TrayDn* pSeqCmd = new CStackerCmd_TrayDn();
						if (SendCommand(g_TaskStacker[m_eStackerIndex], pSeqCmd) == eSeqCmdAck_OK) {
							ChangeSubState(eExcutingStep_WaitEnd_TrayDown);
							break;
						}
					}					
				}				
			}break;
		case eExcutingStep_WaitEnd_TrayDown:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready ) {
					ChangeSubState( eExcutingStep_Load );
					break;
				}
			}break;
		case eExcutingStep_Load:
			{
				int transfer_work_conv_idx = g_TaskTransfer.GetSeqWorkCmdStage();
				if (g_TaskTransfer.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready && transfer_work_conv_idx == m_eStackerIndex) {
					break;
				}

				if (g_TaskStacker[m_eStackerIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
				{
					CStackerCmd_Load* pSeqCmd = new CStackerCmd_Load(false);
					if (SendCommand(g_TaskStacker[m_eStackerIndex], pSeqCmd) == eSeqCmdAck_OK) {
						ChangeSubState(eExcutingStep_WaitEnd_Load);
						break;
					}
				}				
			}break;
		case eExcutingStep_WaitEnd_Load:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready ) {
					ChangeSubState( eExcutingStep_Start );
					break;
				}
			}break;

		case eExcutingStep_Unload:
			{
				int transfer_work_conv_idx = g_TaskTransfer.GetSeqWorkCmdStage();
				if (g_TaskTransfer.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready && transfer_work_conv_idx == m_eStackerIndex) {
					break;
				}

				if (g_TaskStacker[m_eStackerIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
				{
					CStackerCmd_Unload* pSeqCmd = new CStackerCmd_Unload(false);
					if (SendCommand(g_TaskStacker[m_eStackerIndex], pSeqCmd) == eSeqCmdAck_OK) {
						ChangeSubState(eExcutingStep_WaitEnd_Unload);
						break;
					}
				}				
			}break;
		case eExcutingStep_WaitEnd_Unload:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready ) {
					ChangeSubState( eExcutingStep_TrayUp );
					break;
				}
			}break;
		case eExcutingStep_TrayUp:
			{
				if (g_TaskStacker[m_eStackerIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
				{
					int nErr = g_TaskStacker[m_eStackerIndex].GetExistTray_FrontPos_DvcMode(DEF_EXIST);
					if (nErr == ERR_NO_ERROR) {
						CStackerCmd_TrayUp* pSeqCmd = new CStackerCmd_TrayUp(true);
						if (SendCommand(g_TaskStacker[m_eStackerIndex], pSeqCmd) == eSeqCmdAck_OK) {
							ChangeSubState(eExcutingStep_WaitEnd_TrayUp);
							break;
						}
					}					
				}				
			}break;
		case eExcutingStep_WaitEnd_TrayUp:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready ) {
					ChangeSubState( eExcutingStep_Start );
					break;
				}
			}break;

		case eExcutingStep_End:
			{
				ChangeMainState( eAutoLotState_NormalComplete );
			}break;
	}
}

void CTaskAutoStacker::AutoState_NormalComplete()
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

void CTaskAutoStacker::AutoState_AlarmStop()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d] [last step=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nLast_Excuting_Step);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
		case 7000:
			{
				g_TaskStacker[m_eStackerIndex].SetProcessCommand(base_seq_cmd::eProcCmd_Pause);
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

void CTaskAutoStacker::AutoState_Resume()
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
				g_TaskStacker[ m_eStackerIndex ].SetProcessCommand( eProcCmd_Resume );
				ChangeSubState( 8001 );
			}break;
		case 8001:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready )
					break;

				m_swTimeout.SetCheckTime( TIME_OUT_MIDDLE );
				m_swTimeout.StartTimer();
				ChangeSubState( 8100 );
			}break;

		case 8100:
			{
				int transfer_cmd_status = g_TaskTransfer.GetSeqCmdStatus();
				int transfer_work_conv_idx = g_TaskTransfer.GetSeqWorkCmdStage();

				if( transfer_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready
					&& transfer_work_conv_idx == m_eStackerIndex )
				{
					// transfer가 자신의 conveyor에서 작업 중일 경우, 
					// tray align cylinder recovery는 transfer에게 맡긴다.
					ChangeSubState( 8999 );
					break;
				}


				int nRearExist = g_TaskStacker[ m_eStackerIndex ].GetExistTray_RearPos_DvcMode( DEF_EXIST );

				if( m_swTimeout.CheckOverTime() == TRUE ) {
					CParamList AlarmData;
					g_Error.AlarmReport( nRearExist == ERR_NO_ERROR ? ERR_STACKER_1_TRAY_ALIGN_FWD_TIMEOUT + m_eStackerIndex : ERR_STACKER_1_TRAY_ALIGN_BKWD_TIMEOUT + m_eStackerIndex, AlarmData, this );
					g_TaskSystemCtrl.OnLotCommand( eAutoLotCmd_AlarmPause, NULL, NULL );
					ChangeMainState( eAutoLotState_AlarmStop );
					break;
				}

				g_TaskStacker[ m_eStackerIndex ].do_cyl_TrayAlign( nRearExist == ERR_NO_ERROR ? DEF_FORW : DEF_BACK );
				int nAlignErr = g_TaskStacker[ m_eStackerIndex ].chk_cyl_TrayAlign( nRearExist == ERR_NO_ERROR ? DEF_FORW : DEF_BACK );
				if( nAlignErr == ERR_NO_ERROR ) {
					ChangeSubState( 8999 );
					break;
				}
			}break;

		case 8999:
			{
				int nEndFeed = g_TaskAllAutoStacker.GetTrayEndFeed();
				switch( nEndFeed )
				{
					case DEF_LOTEND_TRAYEND:
					case DEF_LOTEND_TRAYFEED:
						{
							ChangeMainState( eAutoLotState_CleanOut );
						}break;
					default: ChangeMainState( eAutoLotState_Executing, m_nLast_Excuting_Step ); break;
				}
			}break;
		default:
			ChangeSubState( 8000 );
			break;
	}
}

void CTaskAutoStacker::AutoState_CleanOut()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	eStacker_Type stacker_type = GetStackerType();

	switch( stacker_type )
	{
		case eStackerType_Load:	this->AutoState_CleanOut_LoadStacker(); break;
		case eStackerType_Unload:this->AutoState_CleanOut_UnloadStacker(); break;
		case eStackerType_Empty_Unload: this->AutoState_CleanOut_EmptyUnloadStacker(); break;
		case eStackerType_Empty_Load: this->AutoState_CleanOut_EmptyLoadStacker(); break;
		default:
			ASSERT( false );
			break;
	}
}

void CTaskAutoStacker::AutoState_CleanOut_LoadStacker()
{
	switch( m_nAutoRunSubStep )
	{
		case eCleanoutStep_Start:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready )
					break;
				ChangeSubState(eCleanoutStep_CheckCondition);
			}break;

		case eCleanoutStep_CheckCondition:
			{
			if (g_TaskStacker[m_eStackerIndex].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
				break;
			if (g_TaskTrayFeeder.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
				break;

			Sleep(500);

			bool feeder_tray_exist_data         = g_TaskTrayFeeder.GetTrayExist_Data();
			bool transfer_area_tray_not_exist   = (g_TaskTrayFeeder.GetTrayExist_TransferArea_Data() != true) && (g_TaskTrayFeeder.GetTrayExist_TransferArea_DvcMode(eNot_Exist) == eOff);
			bool front_tray_not_exist           = (g_TaskStacker[m_eStackerIndex].GetExistTray_FrontPos_DvcMode(DEF_NOT_EXIST) == ERR_NO_ERROR ) && (g_TaskStacker[m_eStackerIndex].ChkExistTray_FrontPos_Sen( DEF_NOT_EXIST ) == ERR_NO_ERROR);
			bool rear_tray_not_exist            = g_TaskStacker[m_eStackerIndex].GetExistTray_RearPos_DvcMode(DEF_NOT_EXIST) == ERR_NO_ERROR && (g_TaskStacker[m_eStackerIndex].ChkExistTray_RearPos_Sen(DEF_NOT_EXIST) == ERR_NO_ERROR);

			if (front_tray_not_exist != true)
			{
				ChangeSubState(eCleanoutStep_TrayUp);
				break;
			}

			// rear 에 tray가 있고 transfer area에는 tray가 없는 경우
			if (rear_tray_not_exist != true && transfer_area_tray_not_exist == true)
			{
				ChangeSubState(eCleanoutStep_Unload);
				break;
			}
			// transfer area 에 tray가 있고 rear 에는 tray가 없는 경우
			else if (rear_tray_not_exist == true && transfer_area_tray_not_exist != true)
			{
				ChangeSubState(eCleanoutStep_Unload);
				break;
			}

			// feeder가 tray를 들고 있다면 기다린다.
			if (feeder_tray_exist_data == true)
			{
				ChangeSubState(eCleanoutStep_CheckCondition);
				break;
			}

			// cv 위와 feeder가 들고 있는 tray가 없다면 clean out 완료
			if (front_tray_not_exist == true && rear_tray_not_exist == true && transfer_area_tray_not_exist == true && feeder_tray_exist_data != true) {
				ChangeSubState(eCleanoutStep_End);
				break;
			}

		}break;

		case eCleanoutStep_TrayUp:
			{
				int FrontExistErr = g_TaskStacker[ m_eStackerIndex ].GetExistTray_FrontPos_DvcMode( DEF_NOT_EXIST );
				if( FrontExistErr == ERR_NO_ERROR ) {
					ChangeSubState(eCleanoutStep_Wait_TrayUp_Comp);
					break;
				}

				CStackerCmd_TrayUp* pSeqCmd = new CStackerCmd_TrayUp(true);
				if( SendCommand( g_TaskStacker[ m_eStackerIndex ], pSeqCmd ) == eSeqCmdAck_OK ) {
					ChangeSubState(eCleanoutStep_Wait_TrayUp_Comp);
					break;
				}
			}break;

		case eCleanoutStep_Wait_TrayUp_Comp:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready ) {
					ChangeSubState(eCleanoutStep_CheckCondition);
					break;
				}
			}break;

		case eCleanoutStep_Unload:
			{
				// int RearExistErr = g_TaskStacker[ m_eStackerIndex ].GetExistTray_RearPos_DvcMode( DEF_EXIST );
				// if( RearExistErr == ERR_NO_ERROR ) {
				// 	ChangeSubState(eCleanoutStep_Wait_Unload_Comp);
				// 	break;
				// }

				CStackerCmd_Unload* pSeqCmd = new CStackerCmd_Unload();
				if( SendCommand( g_TaskStacker[ m_eStackerIndex ], pSeqCmd ) == eSeqCmdAck_OK ) {
					ChangeSubState(eCleanoutStep_Wait_Unload_Comp);
					break;
				}
			}break;

		case eCleanoutStep_Wait_Unload_Comp:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready ) {
					break;
				}

				// cv 위와 feeder가 가지고 있는 tray까지 모두 제거 되어야 cleanout 완료.
				bool feeder_tray_exist_data = g_TaskTrayFeeder.GetTrayExist_Data();
				bool transfer_area_tray_not_exist_data = !g_TaskTrayFeeder.GetTrayExist_TransferArea_Data();
				bool front_not_exist_data = g_TaskStacker[ m_eStackerIndex ].GetExistTray_FrontPos_DvcMode( DEF_NOT_EXIST ) == ERR_NO_ERROR;
				bool rear_not_exist_data = g_TaskStacker[ m_eStackerIndex ].GetExistTray_RearPos_DvcMode( DEF_NOT_EXIST ) == ERR_NO_ERROR;

				if( front_not_exist_data != true|| rear_not_exist_data != true || transfer_area_tray_not_exist_data != true  || feeder_tray_exist_data == true) {
					ChangeSubState(eCleanoutStep_Start);
					break;
				}
				ChangeSubState(eCleanoutStep_End);
			}break;

		case eCleanoutStep_End:
			{
				ChangeMainState( eAutoLotState_NormalComplete );
			}break;

		default:
		    {
		    	ChangeSubState(eCleanoutStep_Start);
		    }break;
	}
}

void CTaskAutoStacker::AutoState_CleanOut_UnloadStacker()
{
	switch( m_nAutoRunSubStep )
	{
		case 9000:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready )
					break;

				ChangeSubState( 9001 );
			}break;
		case 9001:
			{
				int RearExistErr = g_TaskStacker[ m_eStackerIndex ].GetExistTray_RearPos_DvcMode( DEF_NOT_EXIST );
				if( RearExistErr == ERR_NO_ERROR ) {
					ChangeSubState( 9002 );
					break;
				}


				int SendCmdAck = -1;
				int nEndFeed = g_TaskAllAutoStacker.GetTrayEndFeed();
				switch( nEndFeed )
				{
					case DEF_LOTEND_TRAYEND: // PASS제외하고, 나머지 tray 배출.
						{
							bool pass_conveyor = GetPassFailConveyor();
							if( pass_conveyor == true ) {
								SendCmdAck = eSeqCmdAck_OK;
							}
							else {
								CStackerCmd_Unload* pSeqCmd = new CStackerCmd_Unload();
								SendCmdAck = SendCommand( g_TaskStacker[ m_eStackerIndex ], pSeqCmd );
							}
						}break;
					case DEF_LOTEND_TRAYFEED: // 전부 배출.
						{
							CStackerCmd_Unload* pSeqCmd = new CStackerCmd_Unload();
							SendCmdAck = SendCommand( g_TaskStacker[ m_eStackerIndex ], pSeqCmd );
						}break;
					default:
						ASSERT( false );
						break;
				}

				if( SendCmdAck == eSeqCmdAck_OK ) {
					ChangeSubState( 9002 );
					break;
				}
			}break;
		case 9002:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready ) {
					ChangeSubState( 9010 );
					break;
				}
			}break;

		case 9010:
			{
				int FrontExistErr = g_TaskStacker[ m_eStackerIndex ].GetExistTray_FrontPos_DvcMode( DEF_NOT_EXIST );
				if( FrontExistErr == ERR_NO_ERROR ) {
					ChangeSubState( 9011 );
					break;
				}

				CStackerCmd_TrayUp* pSeqCmd = new CStackerCmd_TrayUp(true);
				if( SendCommand( g_TaskStacker[ m_eStackerIndex ], pSeqCmd ) == eSeqCmdAck_OK ) {
					ChangeSubState( 9011 );
					break;
				}
			}break;
		case 9011:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready ) {
					ChangeSubState( 9999 );
					break;
				}
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

void CTaskAutoStacker::AutoState_CleanOut_EmptyLoadStacker()
{
	switch( m_nAutoRunSubStep )
	{
		case 9000:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready )
					break;

				ChangeSubState(9001);
			}break;
		case 9001:
		{
			int FrontExistErr = g_TaskStacker[m_eStackerIndex].GetExistTray_FrontPos_DvcMode(DEF_NOT_EXIST);
			if (FrontExistErr == ERR_NO_ERROR) {
				ChangeSubState(9002);
				break;
			}

			CStackerCmd_TrayUp* pSeqCmd = new CStackerCmd_TrayUp(true);
			if (SendCommand(g_TaskStacker[m_eStackerIndex], pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(9002);
				break;
			}
		}break;
		case 9002:
		{
			if (g_TaskStacker[m_eStackerIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready) {
				ChangeSubState(9010);
				break;
			}
		}break;
		case 9010:
		{
			int RearExistErr = g_TaskStacker[m_eStackerIndex].GetExistTray_RearPos_DvcMode(DEF_NOT_EXIST);
			if (RearExistErr == ERR_NO_ERROR) {
				ChangeSubState(9011);
				break;
			}

			CStackerCmd_Unload* pSeqCmd = new CStackerCmd_Unload();
			if (SendCommand(g_TaskStacker[m_eStackerIndex], pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(9011);
				break;
			}
		}break;
		case 9011:
		{
			if (g_TaskStacker[m_eStackerIndex].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			int FrontExistErr = g_TaskStacker[m_eStackerIndex].GetExistTray_FrontPos_DvcMode(DEF_NOT_EXIST);
			int RearExistErr = g_TaskStacker[m_eStackerIndex].GetExistTray_RearPos_DvcMode(DEF_NOT_EXIST);
			if (FrontExistErr != ERR_NO_ERROR || RearExistErr != ERR_NO_ERROR) {
				ChangeSubState(9000);
				break;
			}


			ChangeSubState(9999);
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

void CTaskAutoStacker::AutoState_CleanOut_EmptyUnloadStacker()
{
	// tray가 rear 부분에 나와있으면 다시 stacker쪽으로 집어 넣는다.
	switch( m_nAutoRunSubStep )
	{
		case 9000:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready )
					break;

				ChangeSubState( 9001 );
			}break;
		case 9001:
			{
				int FrontExistErr = g_TaskStacker[ m_eStackerIndex ].GetExistTray_FrontPos_DvcMode( DEF_NOT_EXIST );
				if( FrontExistErr == ERR_NO_ERROR ) {
					ChangeSubState( 9002 );
					break;
				}

				CStackerCmd_TrayUp* pSeqCmd = new CStackerCmd_TrayUp(true);
				if( SendCommand( g_TaskStacker[ m_eStackerIndex ], pSeqCmd ) == eSeqCmdAck_OK ) {
					ChangeSubState( 9002 );
					break;
				}
			}break;
		case 9002:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready ) {
					ChangeSubState( 9010 );
					break;
				}
			}break;

		case 9010:
			{
				int RearExistErr = g_TaskStacker[ m_eStackerIndex ].GetExistTray_RearPos_DvcMode( DEF_NOT_EXIST );
				if( RearExistErr == ERR_NO_ERROR ) {
					ChangeSubState( 9011 );
					break;
				}

				CStackerCmd_Unload* pSeqCmd = new CStackerCmd_Unload();
				if( SendCommand( g_TaskStacker[ m_eStackerIndex ], pSeqCmd ) == eSeqCmdAck_OK ) {
					ChangeSubState( 9011 );
					break;
				}
			}break;
		case 9011:
			{
				if( g_TaskStacker[ m_eStackerIndex ].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready ) {
					break;
				}

				int FrontExistErr = g_TaskStacker[ m_eStackerIndex ].GetExistTray_FrontPos_DvcMode( DEF_NOT_EXIST );
				int RearExistErr = g_TaskStacker[ m_eStackerIndex ].GetExistTray_RearPos_DvcMode( DEF_NOT_EXIST );
				if( FrontExistErr != ERR_NO_ERROR || RearExistErr != ERR_NO_ERROR ) {
					ChangeSubState( 9000 );
					break;
				}


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

eStacker_Conveyor_State CTaskAutoStacker::GetConveyorState()
{
	CTaskStacker* pCurrentStacker = &g_TaskStacker[ m_eStackerIndex ];

	int  status                  = pCurrentStacker->GetSeqCmdStatus();				                 
	int  nTrayExist              = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_CONV_1 + m_eStackerIndex );
	int  nTrayExistBtm           = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_BTM_1 + m_eStackerIndex);
	bool bDeviceExist            = g_DMCont.m_dmDevice.IsEmptyTray(eDeviceStage_STACKER_CONV_1 + m_eStackerIndex ) == FALSE;
	bool bTrayEmptyPocketRemaind = ConveyorIsFullTray() == false;



	bool bTrayFrontExist         = pCurrentStacker->GetExistTray_FrontPos_DvcMode( DEF_EXIST ) == ERR_NO_ERROR ? true : false;
	bool bTrayRearExist          = pCurrentStacker->GetExistTray_RearPos_DvcMode( DEF_EXIST ) == ERR_NO_ERROR ? true : false;
	bool bTrayAlignCylFwd        = pCurrentStacker->get_cyl_TrayAlign( DEF_FORW ) == ERR_NO_ERROR ? true : false;
	bool bTrayAlignCylBwd        = pCurrentStacker->get_cyl_TrayAlign( DEF_BACK ) == ERR_NO_ERROR ? true : false;
						         
	bool bConveyorMoving         = pCurrentStacker->IsConveyorMoving();

	bool bTrayExist_TransferArea_Sen = g_TaskTrayFeeder.GetTrayExist_TransferArea_DvcMode(eExist) == ERR_NO_ERROR;
	int  nTrayExistConv3_Transfer_Area_Data = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_CONV_3_TRANSFER_AREA);
	bool bIsEmptyTray_TransferArea_Data = g_DMCont.m_dmDevice.IsEmptyTray(eDeviceStage_STACKER_CONV_3_TRANSFER_AREA);

	int nTrayFeeder_Data = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_TRAY_FEEDER);
	bool bIsEmptyTrayFeeder_Data = g_DMCont.m_dmDevice.IsEmptyTray(eDeviceStage_TRAY_FEEDER);


	eStacker_Type stacker_type = GetStackerType();
	switch( stacker_type )
	{
		case eStackerType_Load:
			{
				if( status != CBaseSeqCmd::eSeqCmdStatus_Ready || bConveyorMoving == true)
				{
					return Blocked;
				}
			
				if (nTrayExistConv3_Transfer_Area_Data == true /*&& bIsEmptyTray_TransferArea_Data == true*/ && bTrayExist_TransferArea_Sen == true)
				{
					return ReadyToUnload_Tray;
				}

				if (nTrayFeeder_Data == true && bIsEmptyTrayFeeder_Data == true && bTrayExist_TransferArea_Sen == false)
				{
					return PickEnable_Device;
				}
			}break;
		case eStackerType_Unload:
			{
				//VAT시, Unload Tray 처리.
				BOOL vat_in_process = g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_USE);
				if (vat_in_process == TRUE)
				{
					if (bTrayRearExist == true && bTrayAlignCylFwd == true && nTrayExist == DEF_EXIST)
						return ReadyToUnload_Tray;

					if (bConveyorMoving == false && bTrayRearExist == false && bTrayAlignCylBwd == true /*&& nTrayExist != DEF_EXIST*/)
						return ReadyToLoad_Tray;
				}


				if( status != CBaseSeqCmd::eSeqCmdStatus_Ready
					&& pCurrentStacker->GetActiveSeqCmdName() == _T( "CStackerCmd_Vibration" )
					&& pCurrentStacker->GetActiveSeqCmdName() == _T("CStackerCmd_Unload"))
				{
					return Blocked;
				}

				if( bTrayEmptyPocketRemaind == true && bTrayRearExist == true && bTrayAlignCylFwd == true ) {
					return PlaceEnable_Device;
				}

				if( status == CBaseSeqCmd::eSeqCmdStatus_Ready && bConveyorMoving == false
					&& bTrayRearExist == false && bTrayAlignCylBwd == true && nTrayExist != DEF_EXIST)
				{
					return ReadyToLoad_Tray;
				}
			}break;
		case eStackerType_Empty_Unload:
			{
				if( status != CBaseSeqCmd::eSeqCmdStatus_Ready || bConveyorMoving == true ) {
					return Blocked;
				}

				if( bTrayRearExist == true && bTrayAlignCylFwd == true && nTrayExist == DEF_EXIST) {
					return ReadyToUnload_Tray;
				}
			}break;

		case eStackerType_Empty_Load:
			{
				if( status != CBaseSeqCmd::eSeqCmdStatus_Ready || bConveyorMoving == true ) {
					return Blocked;
				}

				if( bTrayFrontExist == false && bTrayRearExist == false && bTrayAlignCylBwd == true 
					&& nTrayExist != DEF_EXIST && nTrayExistBtm != DEF_EXIST) {
					return ReadyToLoad_Tray;
				}
			}break;
		default:
			ASSERT( false );
			break;
	}

	return Blocked;
}


bool CTaskAutoStacker::ConveyorIsFullTray()
{
	int tray_x_cnt = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
	int tray_y_cnt = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);
	int device_cnt = GetDeviceCount_In_ConveyorTray();

	return device_cnt == tray_y_cnt * tray_x_cnt;
}

int CTaskAutoStacker::GetDeviceCount_In_ConveyorTray()
{
	int tray_x_cnt = g_DMCont.m_dmTrayPP.GN( NDM5_TrayDiv_X );
	int tray_y_cnt = g_DMCont.m_dmTrayPP.GN( NDM5_TrayDiv_Y );
	int device_cnt = 0;
	for( int y = 0; y < tray_y_cnt; y++ ) {
		for( int x = 0; x < tray_x_cnt; x++ ) {
			device_cnt += g_DMCont.m_dmDevice.GetDeviceExist( eDeviceStage_STACKER_CONV_1 + m_eStackerIndex, x, y );
		}
	}

	return device_cnt;
}

bool CTaskAutoStacker::check_vibration_work_condition()
{
	int nRearExistErr = g_TaskStacker[ m_eStackerIndex ].GetExistTray_RearPos_DvcMode( DEF_EXIST );
	if( nRearExistErr != ERR_NO_ERROR )
		return false;

	int current_dvc_cnt = g_TaskStacker[ m_eStackerIndex ].GetConveyorTrayDeviceCnt();
	int vib_act_cnt = g_DMCont.m_dmConveyor.GN( NDM6_CNT_PLACE_DVC_TRAY_FOR_VIBRATION_ACT );

	if( vib_act_cnt <= 0 )
		return false;

	double dGap = 100000.0;
	CPoint pocket_0_0(0, 0);

	std::vector<CPoint> vPicker_0_0;
	vPicker_0_0.push_back(CPoint(0, 0));

	OneAxis* pMotorX = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisX);
	OneAxis* pMotorY = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisY);

	double dMotorX = 0;
	pMotorX->GetCurrentPos(dMotorX);
	double dMotorY = 0;
	pMotorY->GetCurrentPos(dMotorY);
	double dTrayPosX = g_TaskTrayUnloadPP.GetTeachPos(LOC_IDX_TRAY_PP_CONV_4, pocket_0_0, vPicker_0_0, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX);
	double dTrayPosY = g_TaskTrayUnloadPP.GetTeachPos(LOC_IDX_TRAY_PP_CONV_4, pocket_0_0, vPicker_0_0, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisY);


	// Tray Hand에 Unload Device가 있으면 Vibrator 치지 않는다.  [1/6/2022 donghyun.shin]
// 	if(g_TaskTrayPP[m_eTrayIdx].GetTrayPpUnloadDvcCnt() != 0)
// 		return false;

	int check_dvc_cnt = current_dvc_cnt / vib_act_cnt;
	if( m_nCurrentVibCnt < check_dvc_cnt ) {
		m_nCurrentVibCnt = check_dvc_cnt;
		return true;
	}

	return false;
}



eTransferState CTaskAutoStacker::GetCenterStageTransferState()
{
	::EnterCriticalSection(&m_csCenterStageTransferState);

	eTransferState state = m_eCenterStageTransferState;

	::LeaveCriticalSection(&m_csCenterStageTransferState);

	return state;
}

void CTaskAutoStacker::SetCenterStageTransferState(eTransferState state)
{
	::EnterCriticalSection(&m_csCenterStageTransferState);

	m_eCenterStageTransferState = state;

	::LeaveCriticalSection(&m_csCenterStageTransferState);
}


// feeder가 tray 1장을 들고 작업을 하고 있고 load side에는 tray가 1장 준비되어 있으며 
// front side에는 tray를 1장 내려 놓은 상태라면 feeder가 tray index 작업 완료 후 cv에 내려 놓을 때까지 기다린다.
bool CTaskAutoStacker::check_condition_center_stage_ready_to_place()
{
	bool bEmptyTray         = g_TaskStacker[m_eStackerIndex].GetEmptyTray();
	bool bFeederTrayExist   = g_TaskTrayFeeder.GetTrayExist_Data();
	int  nCenterNotExistErr = g_TaskTrayFeeder.GetTrayExist_TransferArea_DvcMode(eNot_Exist);
	int  nFrontNotExistErr  = g_TaskStacker[m_eStackerIndex].GetExistTray_FrontPos_DvcMode(DEF_NOT_EXIST);
	int  nRearNotExistErr   = g_TaskStacker[m_eStackerIndex].GetExistTray_RearPos_DvcMode(DEF_NOT_EXIST);

	if( (bFeederTrayExist == true) && (nCenterNotExistErr == ERR_NO_ERROR) )
	{
		// Load Side가 비어 있는데 load 할 tray가 있는가?
		if (nRearNotExistErr == ERR_NO_ERROR)
		{
			if (nFrontNotExistErr != ERR_NO_ERROR || bEmptyTray == false )
			{
				return false;
			}
		}

		// CV에 Tray가 없는데 Stacker에 바닦에 내려 놓을 tray가 남아 있는가?
		if (nFrontNotExistErr == ERR_NO_ERROR)
		{
			if (bEmptyTray == false)
			{
				return false;
			}
		}
		return true;
	}

	return false;
}

// tray feeder에는 tray가 없어졌고 center cv에는 tray가 놓였다면 놓인 것으로 본다.
bool CTaskAutoStacker::check_condition_center_stage_tray_place()
{
	bool bFeederTrayExist = g_TaskTrayFeeder.GetTrayExist_TransferArea_Data();
	int  nCenterNotExistErr = g_TaskTrayFeeder.GetTrayExist_TransferArea_DvcMode(eNot_Exist);

	if (bFeederTrayExist == false && nCenterNotExistErr == ERR_NO_ERROR)
	{
		return true;
	}

	return false;
}

// transfer에는 tray가 있고 center cv에는 tray가 놓였다면 놓인 것으로 본다.
bool CTaskAutoStacker::check_condition_center_stage_tray_remove()
{
	int  nCenterNotExistErr = g_TaskTrayFeeder.GetTrayExist_TransferArea_DvcMode(eNot_Exist);
	int  nTransferNotExistErr = g_TaskTransfer.ChkTrayExist(eNot_Exist);

	if ( nCenterNotExistErr == ERR_NO_ERROR && nTransferNotExistErr == ERR_NO_ERROR)
	{
		return true;
	}

	return false;
}


bool CTaskAutoStacker::IsPrePickEmptyTrayPossible()
{
	if (ConveyorIsFullTray() == true && g_TaskSystemCtrl.IsRunningFinish() == false)
		return true;

	return false;
}

