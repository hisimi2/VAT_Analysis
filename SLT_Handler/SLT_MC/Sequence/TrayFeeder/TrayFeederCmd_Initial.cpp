#include "StdAfx.h"
#include "TrayFeederCmd_Initial.h"

CTrayFeederCmd_Initial::CTrayFeederCmd_Initial( void )
{
	m_nOrgRetryCount = 0;
	StringCchCopy( m_szCmdName, _countof( m_szCmdName ), CString( __func__ ) );
}

CTrayFeederCmd_Initial::~CTrayFeederCmd_Initial( void )
{
}

int CTrayFeederCmd_Initial::CheckParam()
{
	return ERR_NO_ERROR;
}

int CTrayFeederCmd_Initial::OnStop()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Aborting );
	return ERR_NO_ERROR;
}

int CTrayFeederCmd_Initial::OnRetry()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

int CTrayFeederCmd_Initial::OnSkip()
{
	ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_AlarmComplete );
	return ERR_NO_ERROR;
}

int CTrayFeederCmd_Initial::OnPause()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Pause );
	return ERR_NO_ERROR;
}

int CTrayFeederCmd_Initial::OnResume()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

BOOL CTrayFeederCmd_Initial::State_Setup()
{
	m_swTimeout.SetCheckTime( TIME_OUT_MIDDLE );
	m_swTimeout.StartTimer();

	return CBaseSeqCmd::State_Setup();
}

BOOL CTrayFeederCmd_Initial::State_Executing()
{
	CTaskTrayFeeder* pTask = ( CTaskTrayFeeder* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTask->MakeLog( _T( "[CMD: %s] [EXECUTE] [Step=%s]" ), m_szCmdName, act_string( m_nStep ) );
	}
	m_nStep_Pre = m_nStep;

	CTaskTrayFeeder::eMotor motors[1] = {
		CTaskTrayFeeder::eMotor::eAxisY,
	};

	switch( m_nStep )
	{
	case start:
		{
			g_TaskTrayFeeder.m_bInitialized = FALSE;
			

			// 감지 O : Clamp -> Cylinder UP -> Algin for
			// 감지 X : Algin Back -> Un Clamp -> Cylinder Up
			// Tray Data는 보지 않는다. Sensor만 확인
			BOOL bFeederExistSen = pTask->ChkTrayExistSensor(eExist);
			if(bFeederExistSen == ERR_NO_ERROR)
			{
				//cyl clamp
				pTask->do_cyl_ClampUnclamp( eClamp );

				int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_CLAMP_WAITTIME);
				if (0 < nSleep) {
					Sleep(nSleep);
				}

				auto clamp_state = pTask->chk_cyl_ClampUnclamp( eClamp );
				if( clamp_state != ERR_NO_ERROR ) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( "Clamper is abnormal state." );
					AlarmData.m_fnAddItemFormat( clamp_state.msg.c_str() );
					ReportAlarm(ERR_FEEDER_CLAMP_CYLINDER_FAIL, AlarmData );
					break;
				}

				//cyl do_cyl Up
				pTask->do_cyl_UpDown(eUp);

				nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_UPDN_WAITTIME);
				if (0 < nSleep) {
					Sleep(nSleep);
				}

				auto cylinder_up_state = pTask->chk_cyl_UpDown(eUp);
				if (cylinder_up_state != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(cylinder_up_state.msg.c_str());
					ReportAlarm(ERR_FEEDER_UP_CYLINDER_FAIL, AlarmData);
					break;
				}

				// cyl align for
				pTask->do_cyl_align_ForBack(eForw);
				nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_ALIGN_WAITTIME);
				if (0 < nSleep) {
					Sleep(nSleep);
				}
				auto align_for_state = pTask->chk_cyl_align_ForBack(eForw);
				if (align_for_state != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(align_for_state.msg.c_str());
					ReportAlarm(ERR_FEEDER_ALIGN_CYLINDER_FOR_FAIL, AlarmData);
					break;
				}
			}
			else {
				// cyl align back
				pTask->do_cyl_align_ForBack( eBack );
				int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_ALIGN_WAITTIME);
				if (0 < nSleep) {
					Sleep(nSleep);
				}

				auto align_back_state = pTask->chk_cyl_align_ForBack( eBack );
				if( align_back_state != ERR_NO_ERROR ) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( align_back_state.msg.c_str() );
					ReportAlarm(ERR_FEEDER_ALIGN_CYLINDER_BACK_FAIL, AlarmData );
					break;
				}

				//cyl Unclamp
				pTask->do_cyl_ClampUnclamp( eUnclamp );

				nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_CLAMP_WAITTIME);
				if (0 < nSleep) {
					Sleep(nSleep);
				}

				auto unclamp_state = pTask->chk_cyl_ClampUnclamp( eUnclamp );
				if( unclamp_state != ERR_NO_ERROR ) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( unclamp_state.msg.c_str() );
					ReportAlarm(ERR_FEEDER_UNCLAMP_CYLINDER_FAIL, AlarmData );
					break;
				}

				//cyl do_cyl Up
				pTask->do_cyl_UpDown(eUp);

				nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_UPDN_WAITTIME);
				if (0 < nSleep) {
					Sleep(nSleep);
				}

				auto cylinder_up_state = pTask->chk_cyl_UpDown(eUp);
				if (cylinder_up_state != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(cylinder_up_state.msg.c_str());
					ReportAlarm(ERR_FEEDER_UP_CYLINDER_FAIL, AlarmData);
					break;
				}
			}
			
			m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
			m_swTimeout.StartTimer();
			m_nStep = org_set_clear;
		}break;
	case org_set_clear:
		{

			for( int i = 0; i < _countof( motors ); i++ ) {
				auto pMotor = pTask->GetAxis( motors[ i ] );
				if (pMotor == nullptr)
					continue;

				pMotor->ClearOrgFlag();
				pMotor->SetStop();
				Sleep( 100 );
				pMotor->AlarmReset();
				Sleep( 100 );
				pMotor->ServoOn();
				Sleep( 100 );
				pMotor->EnableSWLimit( DEF_DISABLE );
			}

			m_swTimeout.SetCheckTime( TIME_OUT_LONG );
			m_swTimeout.StartTimer();
			m_nStep = org_motor;
		}break;
	case org_motor:
		{
			if( m_swTimeout.CheckOverTime() == TRUE )
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Feeder Motor Origin Timeout." );
				ReportAlarm( ERR_FEEDER_MOTOR_ORG_TIMEOUT, AlarmData );
				break;
			}

			int nComp = 0;
			for( int i = 0; i < _countof( motors ); i++ )
			{
				int nErrOrg = 0;
				auto pMotor = pTask->GetAxis( motors[ i ] );
				if (pMotor == nullptr)
				{
					nComp++;
					continue;
				}
					

				nErrOrg = pMotor->Origin();
				if( nErrOrg != ERR_NO_ERROR ) {
					m_nOrgRetryCount++;
					if( m_nOrgRetryCount >= 2 ) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat( "Feeder Motor Origin Fail." );
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(nErrOrg, AlarmData );
						break;
					}

					m_nStep = org_set_clear;
					break;
				}
				else {
					BOOL bCompleted = pMotor->IsOrgCompleted( &nErrOrg );
					if( bCompleted ) {
						pMotor->EnableSWLimit( DEF_ENABLE );
						int nErr = pMotor->ChkMotorUsable();
						if( nErr == ERR_NO_ERROR ) {
							nComp++;
						}
					}
					else {
						if( nErrOrg == ERR_MOTOR_CMN_IN_MOVE_STATE ) {
							if( m_swTimeout.CheckOverTime() == TRUE ) {
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat( "%s Motor Org Fail", pMotor->GetAxisName() );
								ReportAlarm( ERR_FEEDER_MOTOR_ORG_FAIL, AlarmData );
								break;
							}
						}
						else {
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat( "%s Axis", pMotor->GetAxisName() );
							ReportAlarm( nErrOrg, AlarmData );
							break;
						}
					}
				}
			}

			if( nComp == _countof( motors ) ) {
				m_swTimeout.SetCheckTime( TIME_OUT_LONG );
				m_swTimeout.StartTimer();
				m_nStep = conv_tray_align_cyl_work;		
				break;
			}
		}break;
	case conv_tray_align_cyl_work : 
		{
			if( m_swTimeout.CheckOverTime() ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Tray Align Cylinder forward timeout" );
				ReportAlarm( ERR_STACKER_3_TRAY_ALIGN_FWD_TIMEOUT, AlarmData );
				break;
			}

			int nErr = g_TaskStacker[ eSTACKER_03 ].GetExistTray_RearPos_Sen( eNot_Exist );
			if( nErr != ERR_NO_ERROR ) {
				g_TaskStacker[ eSTACKER_03 ].do_cyl_TrayAlign( eForw );
				int err = g_TaskStacker[ eSTACKER_03 ].chk_cyl_TrayAlign( eForw );
				if( err == ERR_NO_ERROR ) {
					m_nStep = end;
					break;
				}
			}
			else {
				m_nStep = end;
				break;
			}
		}break;

	case end:
		{
			if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
			{
				pTask->ChkSafetySensor(eExist);
			}

			ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_NormalComplete );
		}break;
	}

	return CBaseSeqCmd::State_Executing();
}

BOOL CTrayFeederCmd_Initial::State_Pause()
{
	CTaskTrayFeeder* pTask = ( CTaskTrayFeeder* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTask->MakeLog( _T( "[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%s]" ), m_szCmdName, m_nStep, act_string( m_nLast_Executing_Step ) );
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
			// Initialize 명령의 경우 Pause가 들어 오면 명령을 완료 시키고 삭제한다.
			SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Aborting );
		}break;
	default:
		{
			m_nStep = 0;
		}break;
	}

	return CBaseSeqCmd::State_Pause();
}

BOOL CTrayFeederCmd_Initial::State_Recovery()
{
	CTaskTrayFeeder* pTask = ( CTaskTrayFeeder* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTask->MakeLog( _T( "[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%s]" ), m_szCmdName, m_nStep, act_string( m_nLast_Executing_Step ) );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Executing );
		}break;
	default:
		{
			m_nStep = 0;
		}break;
	}
	return CBaseSeqCmd::State_Recovery();
}

BOOL CTrayFeederCmd_Initial::State_NormalComplete()
{
	g_TaskTrayFeeder.m_bInitialized = TRUE;
	return CBaseSeqCmd::State_NormalComplete();
}

BOOL CTrayFeederCmd_Initial::State_Alarm()
{
	CTaskTrayFeeder* pTask = ( CTaskTrayFeeder* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTask->MakeLog( _T( "[CMD: %s] [ALARM] [Step=%8d] [LastStep=%s]" ), m_szCmdName, m_nStep, act_string( m_nLast_Executing_Step ) );
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
			// Initialize 명령의 경우 Alarm이 발생되면 명령을 완료 시키고 삭제한다.
			SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Aborting );
		}break;
	default:
		{
			m_nStep = 0;
		}break;
	}

	return CBaseSeqCmd::State_Alarm();
}

BOOL CTrayFeederCmd_Initial::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete();
}

BOOL CTrayFeederCmd_Initial::State_Aborting()
{
	CTaskTrayFeeder* pTask = ( CTaskTrayFeeder* )m_pReceive;
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
			auto pMotor = pTask->GetAxis( CTaskTrayFeeder::eAxisY );
			pMotor->OriginStop();
			pMotor->SetStop();
			pMotor->EnableSWLimit(DEF_ENABLE);
			m_nStep++;
		}break;
	case 1:
		{
// 			if( m_swTimeout.CheckOverTime() == TRUE )
// 			{
// 				CParamList AlarmData;
// 				AlarmData.m_fnAddItemFormat( "Feeder Motor Stop Timeout." );
// 				ReportAlarm( ERR_FEEDER_MOTOR_ORG_TIMEOUT, AlarmData );
// 				break;
// 			}
// 
// 			auto pMotor = pTask->GetAxis( CTaskTrayFeeder::eAxisY );
// 			if( pMotor->MotionDone() == ERR_NO_ERROR ) {
// 				m_nStep++;
// 				break;
			ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AbortComplete); // 명령 진행 중 STOP 명령을 받아 명령이 중단 됨.			
		}break;
	default:
		{
			m_nStep = 0;
		}break;
	}

	return CBaseSeqCmd::State_Aborting();
}

BOOL CTrayFeederCmd_Initial::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}

CString CTrayFeederCmd_Initial::act_string(/*step*/int act )
{
	BEGIN_CASE_TO_STR( act );
	CASE_TO_STR( start );
	CASE_TO_STR( org_set_clear );
	CASE_TO_STR( org_motor );
	CASE_TO_STR( end );
	ELSE_CASE( _T( "unkown_step" ) );
	END_CASE_TO_STR();
}
