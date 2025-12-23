#include "stdafx.h"
#include "StackerCmd_TrayUp.h"

CStackerCmd_TrayUp::CStackerCmd_TrayUp( bool bConveyorStop /*= true*/, bool bManual /*= false*/ )
	: m_bConveyorStop( bConveyorStop ), m_bManual( bManual )
{
	m_nCmdId = eSeqCmd_Stacker_TrayUp;
	StringCchCopy( m_szCmdName, _countof( m_szCmdName ), CString( __func__ ) );

	m_nDeviceMode = eDeviceMode;
	m_nLast_Do_Step = 0;
}

CStackerCmd_TrayUp::~CStackerCmd_TrayUp()
{
}

int CStackerCmd_TrayUp::CheckParam()
{
	m_nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);

	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_TrayUp::OnStop()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Aborting );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_TrayUp::OnRetry()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_TrayUp::OnSkip()
{
	ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_AlarmComplete );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_TrayUp::OnPause()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Pause );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_TrayUp::OnResume()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_TrayUp::State_Setup()
{
	m_swTimeout.SetCheckTime( TIME_OUT_MIDDLE );
	m_swTimeout.StartTimer();

	return CBaseSeqCmd::State_Setup();
}


//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_TrayUp::State_Executing()
{
	// check alarm
	CTaskStacker* pTask = ( CTaskStacker* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		//pTask->MakeLog( _T( "[CMD: %s] [EXECUTE] [Step=%8d]" ), m_szCmdName, m_nStep );
	}
	m_nStep_Pre = m_nStep;


	switch( m_nStep )
	{
		case 0: // check interlock
			{
				// Full Tray 즉각 감지 하도록 요청 건으로 위치 이동
// 				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] check stacker full and Exist front (STR)", m_szCmdName, m_nStep);
// 				if( pTask->ChkFullTray( DEF_ON ) != true ) {
// 					int nAlid = ERR_STACKER_1_TRAY_FULL + pTask->GetIndex();
// 					CParamList AlarmData;
// 					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
// 					ReportAlarm( nAlid, AlarmData );
// 					break;
// 				}
//				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] check stacker full and Exist front (END)", m_szCmdName, m_nStep);
				//int nFrontErr = pTask->GetExistTray_FrontPos_DvcMode( DEF_EXIST );
				//if( nFrontErr != ERR_NO_ERROR ) {
				//	int nAlid = ERR_STACKER_TRAY_NOT_EXIST_FRONT_POS;
				//	CParamList AlarmData;
				//	AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
				//	ReportAlarm( nAlid, AlarmData );
				//	break;
				//}
			

				m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep = 100;

				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] rear stacker tray exist c-tray align cylinder forward (STR)", m_szCmdName, m_nStep);
			}break;

		case 100:
			{
				if( m_swTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_STACKER_1_TRAY_ALIGN_BKWD_TIMEOUT + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				if( pTask->GetExistTray_RearPos_Sen( DEF_NOT_EXIST ) != ERR_NO_ERROR ) {
					pTask->do_cyl_TrayAlign( DEF_FORW );
					int nErr = pTask->chk_cyl_TrayAlign( DEF_FORW );
					if( nErr != ERR_NO_ERROR ) {
						break;
					}
				}

				m_nLast_Do_Step = m_nStep;
				m_nStep = 400;//200;
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] rear stacker tray exist c-tray align cylinder forward (END)", m_szCmdName, m_nStep);
			}break;

		case 200:
			{
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] conveyor moving fast backward (STR)", m_szCmdName, m_nStep);
				//int nErr = pTask->ConveyorMoving( eConveyorMove_BKWD, true );
				//if( nErr != ERR_NO_ERROR ) {
				//	CParamList AlarmData;
				//	AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
				//	ReportAlarm( nErr, AlarmData );
				//	break;
				//}

				m_swTimeout.SetCheckTime( TIME_OUT_MIDDLE );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep = 300;
			}break;

		case 300:
			{
				if( m_swTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_STACKER_1_REAR_SLOW_SENSOR_DETECT_TIMEOUT + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				if( m_nDeviceMode != eNoDeviceNoTray )
				{
					int nErr = pTask->ChkDI( CTaskStacker::xC_TRAY_CV_FRONT_SLOW_SEN, true );
					if( nErr != ERR_NO_ERROR ) {
						break;
					}
				}

				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] conveyor moving fast backward (END)", m_szCmdName, m_nStep);
				m_nStep = 400;
			}break;

		case 400:
			{
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] conveyor moving slow backward (STR)", m_szCmdName, m_nStep);
				int nErr = pTask->ConveyorMoving( eConveyorMove_BKWD, false );
				if( nErr != ERR_NO_ERROR ) {
					CParamList AlarmData;
					int nStackerIndex = pTask->GetIndex();
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), nStackerIndex + 1);
					ReportAlarm(ERR_STACKER_1_INTERLOCK_CONVEYOR_DRIVER_NOT_WORK + nStackerIndex, AlarmData );
					break;
				}

				m_swTimeout.SetCheckTime( TIME_OUT_SHORT*2.0 );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep = 500;
			}break;

		case 500:
			{
				if( m_swTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_STACKER_1_TRAY_NOT_ARRIVED_FRONT_POS + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				if (m_nDeviceMode != eNoDeviceNoTray)
				{
					int nErr = pTask->ChkDI(CTaskStacker::xC_TRAY_CV_FRONT_EXIST_SEN, true);
					if (nErr != ERR_NO_ERROR) {
						break;
					}
				}					

				int nSleepWait = g_DMCont.m_dmStacker.GN(NDM7_STK_CONVEYOR_UNLOAD_STOP_WAIT);
				if (0 < nSleepWait)
					Sleep(nSleepWait);
				else
					Sleep(200);
					
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] conveyor moving slow backward (END)", m_szCmdName, m_nStep);
				m_nStep = 1000;
			}break;

		// Main Pusher Up
		case 1000:
			{
				if (pTask->ChkFullTray(DEF_ON) != true) {
					int nAlid = ERR_STACKER_1_TRAY_FULL + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d tray full sensor detect!"), pTask->GetIndex() + 1);
					ReportAlarm(nAlid, AlarmData);
					break;
				}				

				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] main cylinder up (STR)", m_szCmdName, m_nStep);
				int nErr = pTask->do_cyl_MainPusher( DEF_UP );
				if( nErr != ERR_NO_ERROR ) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nErr, AlarmData );
					break;
				}

				int nSleep = g_DMCont.m_dmStacker.GN( NDM7_STK_MAIN_UPDN_WAITTIME );
				if( 0 < nSleep ) {
					Sleep( nSleep );
				}
				else
					Sleep(200);

				m_swTimeout.SetCheckTime( TIME_OUT_SHORT *2.0 );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep++;
			}break;
		case 1001:
			{
				if( m_swTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_STACKER_1_MAIN_PUSHER_UP_TIMEOUT + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				int nErr = pTask->chk_cyl_MainPusher( DEF_UP );
				if( nErr == ERR_NO_ERROR ) {
					pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] main cylinder up (END)", m_szCmdName, m_nStep);

					m_nStep = ( m_bConveyorStop == true ) ? 1010 : 1100;
				}
			}break;
		// Conveyor Stop (옵션이 켜져있을 때만..)
		case 1010:
			{
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] conveyor stop (STR)", m_szCmdName, m_nStep);
				pTask->ConveyorStop();
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] conveyor stop (END)", m_szCmdName, m_nStep);

				m_nLast_Do_Step = m_nStep;
				m_nStep = 1100;
			}break;
		// Unclamp
		case 1100:
			{
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] lever cylinder unclamp (STR)", m_szCmdName, m_nStep);
				int nErr = pTask->do_cyl_LockUnlock( DEF_UNCLAMP );
				if( nErr != ERR_NO_ERROR ) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nErr, AlarmData );
					break;
				}

				int nSleep = g_DMCont.m_dmStacker.GN( NDM7_STK_CLAMP_WAITTIME );
				if( 0 < nSleep ) {
					Sleep( nSleep );
				}
				else
					Sleep(200);

				m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep++;
			}break;
		case 1101:
			{
				if( m_swTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_STACKER_1_UNCLAMP_TIMEOUT + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				int nErr = pTask->chk_cyl_LockUnlock( DEF_UNCLAMP );
				if( nErr == ERR_NO_ERROR ) {
					pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] lever cylinder unclamp (END)", m_szCmdName, m_nStep);
					m_nStep = 1110;
				}
			}break;
		// Sub Pusher Up
		case 1110:
			{
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] sub cylinder up (STR)", m_szCmdName, m_nStep);
				int nErr = pTask->do_cyl_SubPusher( DEF_UP );
				if( nErr != ERR_NO_ERROR ) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nErr, AlarmData );
					break;
				}

				int nSleep = g_DMCont.m_dmStacker.GN( NDM7_STK_SUB_UPDN_WAITTIME );
				if( 0 < nSleep ) {
					Sleep( nSleep );
				}
				else
					Sleep(200);

				m_swTimeout.SetCheckTime( TIME_OUT_MIDDLE / 3 );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep++;
			}break;
		case 1111:
			{
				if( m_swTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_STACKER_1_SUB_PUSHER_UP_TIMEOUT + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				int nErr = pTask->chk_cyl_SubPusher( DEF_UP );
				if( nErr == ERR_NO_ERROR ) {
					pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] sub cylinder up (END)", m_szCmdName, m_nStep);
					m_nStep = 1120;
				}
			}break;
		// Clamp
		case 1120:
			{
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] lever cylinder clamp (STR)", m_szCmdName, m_nStep);
				int nErr = pTask->do_cyl_LockUnlock( DEF_CLAMP );
				if( nErr != ERR_NO_ERROR ) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nErr, AlarmData );
					break;
				}

				int nSleep = g_DMCont.m_dmStacker.GN( NDM7_STK_CLAMP_WAITTIME );
				if( 0 < nSleep ) {
					Sleep( nSleep );
				}
				else
					Sleep(200);

				m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep++;
			}break;
		case 1121:
			{
				if( m_swTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_STACKER_1_CLAMP_TIMEOUT + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				int nErr = pTask->chk_cyl_LockUnlock( DEF_CLAMP );
				if( nErr == ERR_NO_ERROR ) {
					pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] lever cylinder clamp (END)", m_szCmdName, m_nStep);
					m_nStep = 1130;
				}
			}break;
		// Sub Pusher Down
		case 1130:
			{
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d]  sub cylinder down (STR)", m_szCmdName, m_nStep);
				int nErr = pTask->do_cyl_SubPusher( DEF_DOWN );
				if( nErr != ERR_NO_ERROR ) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nErr, AlarmData );
					break;
				}

				int nSleep = g_DMCont.m_dmStacker.GN( NDM7_STK_SUB_UPDN_WAITTIME );
				if( 0 < nSleep ) {
					Sleep( nSleep );
				}
				else
					Sleep(200);

				m_swTimeout.SetCheckTime( TIME_OUT_MIDDLE / 3 );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep++;
			}break;
		case 1131:
			{
				if( m_swTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_STACKER_1_SUB_PUSHER_DN_TIMEOUT + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				int nErr = pTask->chk_cyl_SubPusher( DEF_DOWN );
				if( nErr == ERR_NO_ERROR ) {
					pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d]  sub cylinder down (END)", m_szCmdName, m_nStep);
					m_nStep = 1140;
				}
			}break;
		// Main Pusher Down
		case 1140:
			{
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d]  main cylinder down (STR)", m_szCmdName, m_nStep);
				int nErr = pTask->do_cyl_MainPusher( DEF_DOWN );
				if( nErr != ERR_NO_ERROR ) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nErr, AlarmData );
					break;
				}

				int nSleep = g_DMCont.m_dmStacker.GN( NDM7_STK_MAIN_UPDN_WAITTIME );
				if( 0 < nSleep ) {
					Sleep( nSleep );
				}
				else
					Sleep(200);

				m_swTimeout.SetCheckTime( TIME_OUT_MIDDLE / 3 );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep++;
			}break;
		case 1141:
			{
				if( m_swTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_STACKER_1_MAIN_PUSHER_DN_TIMEOUT + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				int nErr = pTask->chk_cyl_MainPusher( DEF_DOWN );
				if( nErr == ERR_NO_ERROR ) {
					pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d]  main cylinder down (END)", m_szCmdName, m_nStep);
					m_nStep = 1200;
				}
			}break;
		case 1200:
			{
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d]  data check (STR)", m_szCmdName, m_nStep);
				m_nLast_Do_Step = m_nStep;
				
				int nFrontErr = pTask->ChkExistTray_FrontPos_Sen( DEF_NOT_EXIST );
				if( nFrontErr != ERR_NO_ERROR ) {
					int nAlid = ERR_STACKER_1_TRAY_EXIST_FRONT_POS + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}
				// Cmd Tray Up 동작 한 Tray도 포함 하여 Full Tray인지 체크 한다.
				// 시퀀스 문제로 Tray Up Cmd 종료 후, 감지 Full Tray 감지 Cmd 추가
				if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Manual)
				{
					pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] check stacker full  (STR)", m_szCmdName, m_nStep);
					if (pTask->ChkFullTray(DEF_ON) != true) {
						int nAlid = ERR_STACKER_1_TRAY_FULL + pTask->GetIndex();
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
						ReportAlarm(nAlid, AlarmData);
						break;
					}
					pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] check stacker full (END)", m_szCmdName, m_nStep);
				}

				BOOL vat_in_process = g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_USE);
				if (m_nDeviceMode != eNoDeviceNoTray && vat_in_process != TRUE)
				{
					bool bStackerEmpty = pTask->ChkEmptyTray();
					if( bStackerEmpty != false ) {
						int nAlid = ERR_STACKER_1_TRAY_EMPTY + pTask->GetIndex();
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
						ReportAlarm( nAlid, AlarmData );
						break;
					}
				}

				if( CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto )
				{
					int Stacker_idx = pTask->GetIndex();
					g_DMCont.m_dmDevice.ClearDeviceData( eDeviceStage_STACKER_BTM_1 + Stacker_idx );
				}
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d]  data check (END)", m_szCmdName, m_nStep);
				m_nLast_Do_Step = m_nStep = 99999;
			}break;
		case 99999:
			{
				ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_NormalComplete );
			}break;
	}
	return CBaseSeqCmd::State_Executing();
}


//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_TrayUp::State_Pause()
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
BOOL CStackerCmd_TrayUp::State_Recovery()
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
				int nlockErr = pTask->chk_cyl_LockUnlock(DEF_LOCK);
				int nSubUpErr = pTask->chk_cyl_SubPusher(DEF_DOWN);
				int nMainUpErr = pTask->chk_cyl_MainPusher(DEF_UP);
				/*
				2. Clamp는 Lock 상태고
				3. SubPusher는 down 상태고
				4. Main Pusher도 Up 상태 일 때만
				Main Pusher를 Down 시켜 준다.
				*/
				if (nlockErr == ERR_NO_ERROR && nSubUpErr == ERR_NO_ERROR && nMainUpErr == ERR_NO_ERROR)
				{
					pTask->MakeLog("[CMD: %s] [RECOVERY] [Step=%8d] Main cylinder down (STR)", m_szCmdName, m_nStep);
					int nErr = pTask->do_cyl_MainPusher(DEF_DOWN, eNoInterLock_ON);
					if (nErr != ERR_NO_ERROR) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
						ReportAlarm(nErr, AlarmData);
						break;
					}

					int nSleep = g_DMCont.m_dmStacker.GN(NDM7_STK_MAIN_UPDN_WAITTIME);
					if (0 < nSleep) {
						Sleep(nSleep);
					}

					m_swTimeout.SetCheckTime(TIME_OUT_MIDDLE / 3);
					m_swTimeout.StartTimer();
					m_nStep++;
					m_nLast_Do_Step = 0;
					break;
				}
				else {
					m_nStep = 10;
					break;
				}			
			}break;
		case 1:
		{
			if (m_swTimeout.CheckOverTime() == TRUE) {
				int nAlid = ERR_STACKER_1_MAIN_PUSHER_DN_TIMEOUT + pTask->GetIndex();
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
				ReportAlarm(nAlid, AlarmData);
				break;
			}

			int nErr = pTask->chk_cyl_MainPusher(DEF_DOWN);
			if (nErr == ERR_NO_ERROR) {
				m_nStep = 10;
				pTask->MakeLog("[CMD: %s] [RECOVERY] [Step=%8d] Main cylinder down (END)", m_szCmdName, m_nStep);
			}
		}break;
		case 10:
		{
			m_swTimeout.ResetStartTime();
			SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing, m_nLast_Do_Step);
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
BOOL CStackerCmd_TrayUp::State_NormalComplete()
{
	CTaskStacker* pTask = ( CTaskStacker* )m_pReceive;
	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_TrayUp::State_Alarm()
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
BOOL CStackerCmd_TrayUp::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_TrayUp::State_Aborting()
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
				if (g_DMCont.m_dmHandlerOpt.GB(BDM11_VAT_MODE) == TRUE)
				{
					g_DMCont.m_dmHandlerOpt.SB(BDM11_VAT_ALARM, TRUE);
				}
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
BOOL CStackerCmd_TrayUp::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}






