#include "stdafx.h"
#include "StackerCmd_Load.h"


CStackerCmd_Load::CStackerCmd_Load( bool bCheckSlowSensor /*= true*/, bool bManual /*= false*/ )
	: m_bCheckSlowSensor( bCheckSlowSensor ), m_bManual( bManual )
{
	m_nCmdId = eSeqCmd_Stacker_Load;
	StringCchCopy( m_szCmdName, _countof( m_szCmdName ), CString( __func__ ) );
	m_nLast_Do_Step = 0;

	m_nDeviceMode = eDeviceMode;

}

CStackerCmd_Load::~CStackerCmd_Load()
{
}

int CStackerCmd_Load::CheckParam()
{
	m_nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);

	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_Load::OnStop()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Aborting );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_Load::OnRetry()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_Load::OnSkip()
{
	ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_AlarmComplete );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_Load::OnPause()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Pause );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_Load::OnResume()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_Load::State_Setup()
{
	m_swTimeout.SetCheckTime( TIME_OUT_MIDDLE );
	m_swTimeout.StartTimer();

	return CBaseSeqCmd::State_Setup();
}


//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_Load::State_Executing()
{
	// check alarm
	CTaskStacker* pTask = ( CTaskStacker* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	//if( m_nStep_Pre != m_nStep ) {
	//	pTask->MakeLog( _T( "[CMD: %s] [EXECUTE] [Step=%8d]" ), m_szCmdName, m_nStep );
	//}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
		case eExcutingStep_Start: // check interlock
			{
				// rear 부분에 tray 없어야 함.
				// main, sub cylinder down 상태여야 함.
				// stacker clamper clamp 상태여야 함.
				// align cylinder backward 상태여야 함.

				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Check start before (STR)", m_szCmdName, m_nStep);
				int nFrontErr = pTask->ChkExistTray_FrontPos_Sen( DEF_EXIST );
				if( nFrontErr != ERR_NO_ERROR ) {
					int nAlid = ERR_STACKER_1_TRAY_NOT_EXIST_FRONT_POS + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				int nErr = pTask->ChkExistTray_RearPos_Sen( DEF_NOT_EXIST );
				if( nErr != ERR_NO_ERROR ) {
					int nAlid = ERR_STACKER_1_TRAY_EXIST_REAR_POS + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}
				nErr = pTask->get_cyl_MainPusher( DEF_DOWN );
				if( nErr != ERR_NO_ERROR ) {
					int nAlid = ERR_STACKER_1_MAIN_PUSHER_IS_NOT_DOWN + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				nErr = pTask->get_cyl_SubPusher( DEF_DOWN );
				if( nErr != ERR_NO_ERROR ) {
					int nAlid = ERR_STACKER_1_SUB_PUSHER_IS_NOT_DOWN + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}
				nErr = pTask->chk_cyl_LockUnlock( DEF_LOCK );
				if( nErr != ERR_NO_ERROR ) {
					int nAlid = ERR_STACKER_1_CLAMPER_IS_NOT_CLAMP + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				nErr = pTask->chk_cyl_TrayAlign( DEF_BACK );
				if( nErr != ERR_NO_ERROR ) {
					int nAlid = ERR_STACKER_1_TRAY_ALIGN_IS_NOT_BKWD + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Check start before (END)", m_szCmdName, m_nStep);

				m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep = eExcutingStep_Backward_Tray_Align_Cylinder;
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] c-tray align cylinder backword (STR)", m_szCmdName, m_nStep);
			}break;

		case eExcutingStep_Backward_Tray_Align_Cylinder:
			{
				if( m_swTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_STACKER_1_TRAY_ALIGN_BKWD_TIMEOUT + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d Tray Align Cylinder backword fail"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				pTask->do_cyl_TrayAlign( DEF_BACK );
				int nErr = pTask->chk_cyl_TrayAlign( DEF_BACK );
				if( nErr != ERR_NO_ERROR ) {
					break;
				}
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] c-tray align cylinder backword (END)", m_szCmdName, m_nStep);

				m_nLast_Do_Step = m_nStep;
				m_nStep = eExcutingStep_Move_Conveyor;
			}break;

		case eExcutingStep_Move_Conveyor:
			{
				int nStackerIndex = pTask->GetIndex();
				if (m_nDeviceMode != eNoDeviceNoTray || g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
				{// Recovery 진행 할 때, Rear Tray Exist or Slow Sensor 이미 감지 되고 있으면 Conveyor Speed Low로 이동 하도록 Step 변경 / Device 튀는 현상 방지
					//int nRearErr = g_TaskStacker[nStackerIndex].ChkExistTray_RearPos_Sen(DEF_EXIST);
					int nErr = pTask->ChkDI(CTaskStacker::xC_TRAY_CV_REAR_SLOW_SEN, true);
					if (nErr == ERR_NO_ERROR)
					{
						pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] converyor moving slow & exist sensing", m_szCmdName, m_nStep);
						m_nStep = eExcutingStep_Slow;
						break;
					}
				}


				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] converyor moving fast foward (STR)", m_szCmdName, m_nStep);
				int nErr = pTask->ConveyorMoving( eConveyorMove_FWD, true );
				if( nErr != ERR_NO_ERROR ) {
					CParamList AlarmData;
					
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), nStackerIndex + 1);
					ReportAlarm(ERR_STACKER_1_INTERLOCK_CONVEYOR_DRIVER_NOT_WORK+ nStackerIndex, AlarmData );
					break;
				}

				m_swTimeout.SetCheckTime(TIME_OUT_SHORT*5.0);
				m_swTimeout.StartTimer();

				if( m_bCheckSlowSensor == true ) {
					m_nStep = eExcutingStep_Check_Rear_Slow_Sensor;
				}
				else {
					m_nStep = eExcutingStep_Check_Rear_Stop_Sensor;
				}
			}break;

		case eExcutingStep_Check_Rear_Slow_Sensor:
			{
				if( m_swTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_STACKER_1_REAR_SLOW_SENSOR_DETECT_TIMEOUT + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( _T( "Stacker : %d" ), pTask->GetIndex() + 1 );
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				if( m_nDeviceMode != eNoDeviceNoTray || g_IO.GetDriverType() == _DIO_TYPE_SIMUL_ || pTask->GetEqpCtrlMode() == eEqpCtrlMode_Manual)
				{				
					int nErr = pTask->ChkDI(CTaskStacker::xC_TRAY_CV_REAR_SLOW_SEN, true);
					if (nErr != ERR_NO_ERROR) {
						break;
					}
				}
				else {
					Sleep( 500 );
				}

				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] converyor moving fast foward (END)", m_szCmdName, m_nStep);
				m_nStep = eExcutingStep_Slow;
			}break;

		case eExcutingStep_Slow:
			{
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] converyor moving slow foward (str)", m_szCmdName, m_nStep);
				int nErr = pTask->ConveyorMoving( eConveyorMove_FWD, false );
				if( nErr != ERR_NO_ERROR ) {
					CParamList AlarmData;
					int nStackerIndex = pTask->GetIndex();
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), nStackerIndex + 1);
					ReportAlarm(ERR_STACKER_1_INTERLOCK_CONVEYOR_DRIVER_NOT_WORK + nStackerIndex, AlarmData );
					break;
				}

				m_swTimeout.SetCheckTime(TIME_OUT_SHORT*2);
				m_swTimeout.StartTimer();
				m_nStep = eExcutingStep_Check_Rear_Stop_Sensor;
			}break;

		case eExcutingStep_Check_Rear_Stop_Sensor:
			{
				if( m_swTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_STACKER_1_REAR_TRAY_EXIST_SENSOR_DETECT_TIMEOUT + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				
				int nFrontErr = pTask->ChkExistTray_FrontPos_Sen( DEF_NOT_EXIST );
				if( nFrontErr != ERR_NO_ERROR ) {
					break;
				}

				if (m_nDeviceMode != eNoDeviceNoTray || g_IO.GetDriverType() == _DIO_TYPE_SIMUL_ )
				{			
					int nErr = pTask->ChkDI(CTaskStacker::xC_TRAY_CV_REAR_EXIST_SEN, true);
					if (nErr != ERR_NO_ERROR) {
						break;
					}				
				}
					
				if (m_nDeviceMode == eNoDeviceNoTray)
					Sleep( 500 );

				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] converyor moving slow foward (END)", m_szCmdName, m_nStep);
				m_nStep = eExcutingStep_Forward_Tray_Align_Cylinder;
			}break;

		case eExcutingStep_Forward_Tray_Align_Cylinder:
			{
				if( m_swTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_STACKER_1_TRAY_ALIGN_FWD_TIMEOUT + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

// 				int nSleepWait = g_DMCont.m_dmStacker.GN(NDM7_STK_CONVEYOR_LOAD_STOP_WAIT);
// 				if (0 < nSleepWait)
// 					Sleep(nSleepWait);
// 				else
// 					Sleep(200);

				pTask->do_cyl_TrayAlign( DEF_FORW );

				int nSleepWait = g_DMCont.m_dmStacker.GN(NDM7_STK_CONVEYOR_LOAD_STOP_WAIT);
				if (0 < nSleepWait)
					Sleep(nSleepWait);
				else
					Sleep(200);


				int nErr = pTask->chk_cyl_TrayAlign( DEF_FORW );
				if( nErr != ERR_NO_ERROR ) {
					break;
				}

				m_swTimeout.SetCheckTime(TIME_OUT_SHORT * 2);
				m_swTimeout.StartTimer();

				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] converyor c-tray align cylinder foward (END)", m_szCmdName, m_nStep);
				m_nStep = eExcutingStep_Stop;
			}break;

			case eExcutingStep_Stop:
			{
				if (m_swTimeout.CheckOverTime() == TRUE) {
					int nAlid = ERR_STACKER_1_REAR_TRAY_EXIST_SENSOR_DETECT_TIMEOUT + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm(nAlid, AlarmData);
					break;
				}

				int nRearErr = pTask->ChkExistTray_RearPos_Sen(DEF_EXIST);
				if (nRearErr != ERR_NO_ERROR)
					break;
				
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] converyor moving stop (STR)", m_szCmdName, m_nStep);
				pTask->ConveyorStop();
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] converyor moving stop (END)", m_szCmdName, m_nStep);

				m_swTimeout.SetCheckTime(TIME_OUT_SHORT);
				m_swTimeout.StartTimer();
				m_nStep = eExcutingStep_CheckTrayData;
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] converyor c-tray align cylinder foward (STR)", m_szCmdName, m_nStep);
			}break;

		case eExcutingStep_CheckTrayData:
			{
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] check data (STR)", m_szCmdName, m_nStep);
				//if (m_nDeviceMode != eNoDeviceNoTray)
				//{
					int nRearErr = pTask->ChkExistTray_RearPos_Sen(DEF_EXIST);
					if (nRearErr != ERR_NO_ERROR) {
						int nAlid = ERR_STACKER_1_TRAY_NOT_EXIST_REAR_POS + pTask->GetIndex();
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
						ReportAlarm(nAlid, AlarmData);
						break;
					}
				
					int nFrontErr = pTask->ChkExistTray_FrontPos_Sen( DEF_NOT_EXIST );
					if( nFrontErr != ERR_NO_ERROR ) {
						int nAlid = ERR_STACKER_1_TRAY_EXIST_FRONT_POS + pTask->GetIndex();
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
						ReportAlarm( nAlid, AlarmData );
						break;
					}
				//}

				if( CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto )
				{
					eStacker_Idx idx = pTask->GetIndex();
					g_DMCont.m_dmDevice.SwapStageData( eDeviceStage_STACKER_BTM_1 + idx, eDeviceStage_STACKER_CONV_1 + idx );					
					g_TaskTrayLoadPP.ResetLoadTrayPickFailCnt();

					if (idx == eSTACKER_03)
					{
						int nTrayConv_Data = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_CONV_3);
						pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] eDeviceStage_STACKER_CONV_3 %d check data (END)", m_szCmdName, m_nStep, nTrayConv_Data);
					}
				
				}
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] check data (END)", m_szCmdName, m_nStep);
				m_nLast_Do_Step = m_nStep = eExcutingStep_End;
			}break;

		case eExcutingStep_End:
			{
				ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_NormalComplete );
			}break;
	}
	return CBaseSeqCmd::State_Executing();
}


//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_Load::State_Pause()
{
	CTaskStacker* pTask = ( CTaskStacker* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTask->MakeLog( _T( "[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]" ), m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
		case 0:
			{
				pTask->ConveyorSlowStop();
				m_nStep++;
			}break;
		case 1:
			{
			}break;
		default:
		{
			m_nStep = 0;
		}break;
	}

	return CBaseSeqCmd::State_Pause();
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_Load::State_Recovery()
{
	CTaskStacker* pTask = ( CTaskStacker* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTask->MakeLog( _T( "[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%d]" ), m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
		case 0:
			{
				m_swTimeout.ResetStartTime();
				SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Executing, m_nLast_Do_Step );
			}break;
		default:
		{
			m_nStep = 0;
		}break;
	}
	return CBaseSeqCmd::State_Recovery();
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_Load::State_NormalComplete()
{
	CTaskStacker* pTask = ( CTaskStacker* )m_pReceive;
	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_Load::State_Alarm()
{
	CTaskStacker* pTask = ( CTaskStacker* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTask->MakeLog( _T( "[CMD: %s] [ALARM] [Step=%8d] [LastStep=%d]" ), m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
		case 0: // alarm 발생 했을 때 처리해 줘야 할꺼 처리 해줌
			{
				pTask->ConveyorSlowStop();
				m_nStep++;
			}break;
		case 1:
			{
			}break;
		default:
		{
			m_nStep = 0;
		}break;
	}
	return CBaseSeqCmd::State_Alarm();
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_Load::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_Load::State_Aborting()
{
	CTaskStacker* pTask = ( CTaskStacker* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTask->MakeLog( _T( "[CMD: %s] [ABORT] [Step=%8d]" ), m_szCmdName, m_nStep );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
		case 0:
			{
				m_nStep++;
			}break;
		case 1:
			{
				ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_AbortComplete ); // 명령 진행 중 STOP 명령을 받아 명령이 중단 됨.
			}break;
		default:
		{
			m_nStep = 0;
		}break;
	}
	return CBaseSeqCmd::State_Aborting();
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_Load::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}