#include "StdAfx.h"
#include "TrayFeederCmd_MovePos.h"

CTrayFeederCmd_MovePos::CTrayFeederCmd_MovePos( void )
{
	m_nTrayRowStep = 0;
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));
}

CTrayFeederCmd_MovePos::~CTrayFeederCmd_MovePos( void )
{
}

int CTrayFeederCmd_MovePos::CheckParam()
{
	if(m_nCmdStage != eLocIdxFeeder::LOC_IDX_FEEDER_LOAD_PICK_START_POS &&
		m_nCmdStage != eLocIdxFeeder::LOC_IDX_FEEDER_TRAY_PICK_POS &&
		m_nCmdStage != eLocIdxFeeder::LOC_IDX_FEEDER_TRAY_PLACE_POS &&
		m_nCmdStage != eLocIdxFeeder::LOC_IDX_FEEDER_SAFETY ) 
	{
		return ERR_CMD_PARAM_INVALID;
	}

	return 0;
}

int CTrayFeederCmd_MovePos::OnStop()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Aborting );
	return ERR_NO_ERROR;
}

int CTrayFeederCmd_MovePos::OnRetry()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

int CTrayFeederCmd_MovePos::OnSkip()
{
	ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_AlarmComplete );
	return ERR_NO_ERROR;
}

int CTrayFeederCmd_MovePos::OnPause()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Pause );
	return ERR_NO_ERROR;
}

int CTrayFeederCmd_MovePos::OnResume()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

BOOL CTrayFeederCmd_MovePos::State_Setup()
{
	m_swTimeout.SetCheckTime( TIME_OUT_MIDDLE );
	m_swTimeout.StartTimer();

	return CBaseSeqCmd::State_Setup();
}

BOOL CTrayFeederCmd_MovePos::State_Executing()
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

	switch( m_nStep )
	{
	case start:
		{
			auto align_for_state = pTask->get_cyl_align_ForBack( eForw );
			auto align_back_state = pTask->get_cyl_align_ForBack( eBack );
			if( align_for_state != ERR_NO_ERROR && align_back_state != ERR_NO_ERROR ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Align cylinder is abnormal state." );
				AlarmData.m_fnAddItemFormat( align_for_state.msg.c_str() );
				AlarmData.m_fnAddItemFormat( align_back_state.msg.c_str() );
				ReportAlarm(ERR_FEEDER_ALIGN_CYLINDER_ABNORMAL, AlarmData );
				break;
			}

			auto clamp_state = pTask->get_cyl_ClampUnclamp( eClamp );
			auto unclamp_state = pTask->get_cyl_ClampUnclamp( eUnclamp );
			if( clamp_state != ERR_NO_ERROR && unclamp_state != ERR_NO_ERROR ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Clamper is abnormal state." );
				AlarmData.m_fnAddItemFormat( clamp_state.msg.c_str() );
				AlarmData.m_fnAddItemFormat( unclamp_state.msg.c_str() );
				ReportAlarm(ERR_FEEDER_CLAMP_CYLINDER_ABNORMAL, AlarmData );
				break;
			}

			m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
			m_swTimeout.StartTimer();
			m_nStep = do_chk_up_cyl_start;
		}break;

	case do_chk_up_cyl_start:
		{
			if (m_swTimeout.CheckOverTime()) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Feeder Up Timeout.");
				ReportAlarm(ERR_FEEDER_UP_DOWN_CYLINDER_TIMEOUT, AlarmData);
				break;
			}

			auto chk = pTask->chk_cyl_UpDown(eUp);
			if (chk != ERR_NO_ERROR) {
				pTask->do_cyl_UpDown(eUp);

				int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_UPDN_WAITTIME);
				if (0 < nSleep) {
					Sleep(nSleep);
				}
			}
	
			chk = pTask->chk_cyl_UpDown(eUp);
			if (chk == ERR_NO_ERROR) {
				m_nStep = do_move;
				break;
			}
		}break;

	case do_move:
		{
			int err = pTask->do_MovePos( CTaskTrayFeeder::eAxisY, m_nCmdStage, m_nTrayRowStep);
			if( err != ERR_NO_ERROR ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Feeder Move Fail." );
				OneAxis* pMotor = pTask->GetAxis(CTaskTrayFeeder::eAxisY);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(err, AlarmData );
				break;
			}

			m_swTimeout.SetCheckTime( TIME_OUT_LONG );
			m_swTimeout.StartTimer();
			m_nStep = chk_move;
		}break;
	case chk_move:
		{
			if (m_swTimeout.CheckOverTime()) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Feeder Motor Move Timeout.");
				ReportAlarm(ERR_FEEDER_MOTOR_MOVE_TIMEOUT, AlarmData);
				break;
			}

			OneAxis* pMotorY = pTask->GetAxis(CTaskTrayFeeder::eAxisY);
			int nAxisDone = pMotorY->AxisDone();

			int err = pTask->ChkMotorPos( CTaskTrayFeeder::eAxisY, m_nCmdStage, m_nTrayRowStep);			
			if( err == ERR_NO_ERROR && nAxisDone == ERR_NO_ERROR) {
				m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
				m_swTimeout.StartTimer();
				m_nStep = end;
				break;
			}
			else
			{			
				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Feeder Motor Move Timeout.");
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotorY->GetAxisName());
					ReportAlarm(ERR_FEEDER_MOTOR_MOVE_FAIL, AlarmData);
					break;
				}
			}
		}break;

	case end:
		{
			ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_NormalComplete );
		}break;
	}

	return CBaseSeqCmd::State_Executing();
}

BOOL CTrayFeederCmd_MovePos::State_Pause()
{
	CTaskTrayFeeder* pTask = ( CTaskTrayFeeder* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTask->MakeLog( _T( "[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]" ), m_szCmdName, m_nStep, m_nLast_Executing_Step );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			int err = ERR_NO_ERROR;
			for( int i = 0; i < CTaskTrayFeeder::eAxisMax; i++ ) {
				OneAxis* pMotor = pTask->GetAxis( ( CTaskTrayFeeder::eMotor )i );
				if (pMotor != nullptr)
				{
					int stop_result = pMotor->SetSlowStop();
					if (stop_result != ERR_NO_ERROR) {
						err = stop_result;
					}
				}
			}

// 			if( err != ERR_NO_ERROR ) {
// 				CParamList AlarmData;
// 				AlarmData.m_fnAddItemFormat( "Feeder Motor Stop Fail." );
// 				ReportAlarm( ERR_FEEDER_MOTOR_STOP_FAIL, AlarmData );
// 				break;
// 			}

			m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
			m_swTimeout.StartTimer();
			m_nStep++;
		}break;
	case 1:
		{
			if( m_swTimeout.CheckOverTime() ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Feeder Motor Stop Timeout." );
				ReportAlarm( ERR_FEEDER_MOTOR_STOP_TIMEOUT, AlarmData );
				break;
			}

			int motion_done_cnt = 0;
			for( int i = 0; i < CTaskTrayFeeder::eAxisMax; i++ ) {
				OneAxis* pMotor = pTask->GetAxis( ( CTaskTrayFeeder::eMotor )i );
				if (pMotor == nullptr)
				{
					motion_done_cnt++;
					continue;
				}
				motion_done_cnt += pMotor->MotionDone() == ERR_NO_ERROR ? 1 : 0;
			}

			if( motion_done_cnt == CTaskTrayFeeder::eAxisMax ) {
				m_nStep++;
				break;
			}
		}break;
	case 2:
		{

		}break;
	default:
		{
			m_nStep = 0;
		}break;
	}

	return CBaseSeqCmd::State_Pause();
}

BOOL CTrayFeederCmd_MovePos::State_Recovery()
{
	CTaskTrayFeeder* pTask = ( CTaskTrayFeeder* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTask->MakeLog( "[CMD: %s] [RECOVERY] [Step=%8d]", m_szCmdName, m_nStep );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			auto align_for_state = pTask->get_cyl_align_ForBack( eForw );
			auto align_back_state = pTask->get_cyl_align_ForBack( eBack );
			if( align_for_state != ERR_NO_ERROR && align_back_state != ERR_NO_ERROR ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Align cylinder is abnormal state." );
				AlarmData.m_fnAddItemFormat( align_for_state.msg.c_str() );
				AlarmData.m_fnAddItemFormat( align_back_state.msg.c_str() );
				ReportAlarm(ERR_FEEDER_ALIGN_CYLINDER_ABNORMAL, AlarmData );
				break;
			}

			auto clamp_state = pTask->get_cyl_ClampUnclamp( eClamp );
			auto unclamp_state = pTask->get_cyl_ClampUnclamp( eUnclamp );
			if( clamp_state != ERR_NO_ERROR && unclamp_state != ERR_NO_ERROR ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Clamper is abnormal state." );
				AlarmData.m_fnAddItemFormat( clamp_state.msg.c_str() );
				AlarmData.m_fnAddItemFormat( unclamp_state.msg.c_str() );
				ReportAlarm(ERR_FEEDER_CLAMP_CYLINDER_ABNORMAL, AlarmData );
				break;
			}

			auto up_state = pTask->get_cyl_UpDown( eUp );
			auto down_state = pTask->get_cyl_UpDown( eDown );
			if( up_state != ERR_NO_ERROR && down_state != ERR_NO_ERROR ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Up Down is abnormal state." );
				AlarmData.m_fnAddItemFormat( up_state.msg.c_str() );
				AlarmData.m_fnAddItemFormat( down_state.msg.c_str() );
				ReportAlarm(ERR_FEEDER_UP_DOWN_CYLINDER_ABNORMAL, AlarmData );
				break;
			}

			m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
			m_swTimeout.StartTimer();
			m_nStep++;
		}break;
	case 1:
		{
			pTask->do_cyl_UpDown( eUp );

			int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_UPDN_WAITTIME);
			if (0 < nSleep) {
				Sleep(nSleep);
			}

			auto chk = pTask->chk_cyl_UpDown( eUp );
			if( m_swTimeout.CheckOverTime() ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Feeder Up Timeout." );
				AlarmData.m_fnAddItemFormat( chk.msg.c_str() );
				ReportAlarm(ERR_FEEDER_UP_DOWN_CYLINDER_TIMEOUT, AlarmData );
				break;
			}

			if( chk == ERR_NO_ERROR ) {
				m_nStep++;
				m_swTimeout.SetCheckTime(TIME_OUT_MIDDLE);
				m_swTimeout.StartTimer();
				break;
			}
		}break;
	case 2:
		{
			if (m_swTimeout.CheckOverTime()) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Feeder Move Timeout.");
				ReportAlarm(ERR_FEEDER_MOTOR_MOVE_TIMEOUT, AlarmData);
				break;
			}

			auto ret = pTask->do_MovePos( CTaskTrayFeeder::eAxisY, eLocIdxFeeder::LOC_IDX_FEEDER_SAFETY, 0, 50 );
			if( ret != ERR_NO_ERROR ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Feeder Move Fail." );
				OneAxis* pMotor = pTask->GetAxis(CTaskTrayFeeder::eAxisY);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(ret, AlarmData );
				break;
			}

			m_swTimeout.SetCheckTime( TIME_OUT_LONG );
			m_swTimeout.StartTimer();
			m_nStep++;
		}break;
	case 3:
		{
			auto ret = pTask->ChkMotorPos( CTaskTrayFeeder::eAxisY, eLocIdxFeeder::LOC_IDX_FEEDER_SAFETY );
			if( m_swTimeout.CheckOverTime() ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Feeder Move Timeout." );
				ReportAlarm( ERR_FEEDER_MOTOR_MOVE_TIMEOUT, AlarmData );
				break;
			}

			if( ret == ERR_NO_ERROR ) {
				ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_Executing);
				m_nStep = 99999;
				break;
			}
		}break;
	case 99999:
		{

		}break;
	}

	return CBaseSeqCmd::State_Recovery();
}

BOOL CTrayFeederCmd_MovePos::State_NormalComplete()
{
	return CBaseSeqCmd::State_NormalComplete();
}

BOOL CTrayFeederCmd_MovePos::State_Alarm()
{
	CTaskTrayFeeder* pTask = ( CTaskTrayFeeder* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTask->MakeLog( "[CMD: %s] [ALARM] [Step=%8d]", m_szCmdName, m_nStep );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			int err = ERR_NO_ERROR;
			for( int i = 0; i < CTaskTrayFeeder::eAxisMax; i++ ) {
				OneAxis* pMotor = pTask->GetAxis( ( CTaskTrayFeeder::eMotor )i );
				if (pMotor != nullptr)
				{
					int stop_result = pMotor->SetSlowStop();
					if (stop_result != ERR_NO_ERROR) {
						err = stop_result;
					}
				}
			}

// 			if( err != ERR_NO_ERROR ) {
// 				CParamList AlarmData;
// 				AlarmData.m_fnAddItemFormat( "Feeder Motor Stop Fail." );
// 				ReportAlarm( ERR_FEEDER_MOTOR_STOP_FAIL, AlarmData );
// 				break;
// 			}

			m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
			m_swTimeout.StartTimer();
			m_nStep++;
		}break;
	case 1:
		{
			if( m_swTimeout.CheckOverTime() ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Feeder Motor Stop Timeout." );
				ReportAlarm( ERR_FEEDER_MOTOR_STOP_TIMEOUT, AlarmData );
				break;
			}

			int motion_done_cnt = 0;
			for( int i = 0; i < CTaskTrayFeeder::eAxisMax; i++ ) {
				OneAxis* pMotor = pTask->GetAxis( ( CTaskTrayFeeder::eMotor )i );
				if (pMotor == nullptr)
				{
					motion_done_cnt++;
					continue;
				}
				motion_done_cnt += pMotor->MotionDone() == ERR_NO_ERROR ? 1 : 0;
			}

			if( motion_done_cnt == CTaskTrayFeeder::eAxisMax ) {
				m_nStep++;
				break;
			}
		}break;
	case 2:
		{
			ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_AlarmComplete ); // 명령 진행 중 STOP 명령을 받아 명령이 중단 됨.
		}break;
	default:
		{
			m_nStep = 0;
		}break;
	}

	return CBaseSeqCmd::State_Alarm();
}

BOOL CTrayFeederCmd_MovePos::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete();
}

BOOL CTrayFeederCmd_MovePos::State_Aborting()
{
	CTaskTrayFeeder* pTask = ( CTaskTrayFeeder* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTask->MakeLog( "[CMD: %s] [ABORT] [Step=%8d]", m_szCmdName, m_nStep );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			int err = ERR_NO_ERROR;
			for( int i = 0; i < CTaskTrayFeeder::eAxisMax; i++ ) {
				OneAxis* pMotor = pTask->GetAxis( ( CTaskTrayFeeder::eMotor )i );
				if (pMotor != nullptr)
				{
					int stop_result = pMotor->SetSlowStop();
					if (stop_result != ERR_NO_ERROR) {
						err = stop_result;
					}
				}
			}

// 			if( err != ERR_NO_ERROR ) {
// 				CParamList AlarmData;
// 				AlarmData.m_fnAddItemFormat( "Feeder Motor Stop Fail." );
// 				ReportAlarm( ERR_FEEDER_MOTOR_STOP_FAIL, AlarmData );
// 				break;
// 			}

			m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
			m_swTimeout.StartTimer();
			m_nStep++;
		}break;
	case 1:
		{
			if( m_swTimeout.CheckOverTime() ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Feeder Motor Stop Timeout." );
				ReportAlarm( ERR_FEEDER_MOTOR_STOP_TIMEOUT, AlarmData );
				break;
			}

			int motion_done_cnt = 0;
			for( int i = 0; i < CTaskTrayFeeder::eAxisMax; i++ ) {
				OneAxis* pMotor = pTask->GetAxis( ( CTaskTrayFeeder::eMotor )i );
				if (pMotor == nullptr)
				{
					motion_done_cnt++;
					continue;
				}
				motion_done_cnt += pMotor->MotionDone() == ERR_NO_ERROR ? 1 : 0;

			}

			if( motion_done_cnt == CTaskTrayFeeder::eAxisMax ) {
				m_nStep++;
				break;
			}
		}break;
	case 2:
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

BOOL CTrayFeederCmd_MovePos::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}

CString CTrayFeederCmd_MovePos::act_string( int act )
{
	BEGIN_CASE_TO_STR( act );
	CASE_TO_STR( start );
	CASE_TO_STR( do_chk_up_cyl_start );
	CASE_TO_STR( do_move );
	CASE_TO_STR( chk_move );
	CASE_TO_STR( end );
	ELSE_CASE( _T( "unkown_step" ) );
	END_CASE_TO_STR();
}
