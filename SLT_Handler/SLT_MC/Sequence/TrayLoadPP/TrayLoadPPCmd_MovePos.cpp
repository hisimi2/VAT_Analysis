#include "StdAfx.h"
#include "TrayLoadPPCmd_MovePos.h"

CTrayLoadPPCmd_MovePos::CTrayLoadPPCmd_MovePos( void )
{
	eStage = eLocIdxTrayPP::LOC_IDX_TRAY_PP_Undefined;
	vPicker.clear();

	m_nCmdId = eSeqCmd_TrayLoadPP_MovePos;
	m_nRetryCount = 0;
	sprintf_s( m_szCmdName, sizeof( m_szCmdName ), "TrayLoadPPCmd_MovePos" );
}

CTrayLoadPPCmd_MovePos::~CTrayLoadPPCmd_MovePos( void )
{
}

int CTrayLoadPPCmd_MovePos::CheckParam()
{
	double dTimeout = MOTOR_TIME_OUT_MIDDLE;
	m_tcTimeout.SetCheckTime( dTimeout );
	m_tcTimeout.StartTimer();

	return ERR_NO_ERROR;
}

int CTrayLoadPPCmd_MovePos::OnStop()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Aborting );
	return ERR_NO_ERROR;
}

int CTrayLoadPPCmd_MovePos::OnRetry()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

int CTrayLoadPPCmd_MovePos::OnSkip()
{
	ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_AlarmComplete );
	return ERR_NO_ERROR;
}

int CTrayLoadPPCmd_MovePos::OnPause()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Pause );
	return ERR_NO_ERROR;
}

int CTrayLoadPPCmd_MovePos::OnResume()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

BOOL CTrayLoadPPCmd_MovePos::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}

BOOL CTrayLoadPPCmd_MovePos::State_Executing()
{
	// check alarm
	CTaskTrayLoadPP* pTaskTrayLoadPP = ( CTaskTrayLoadPP* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTaskTrayLoadPP->MakeLog( _T( "[CMD: %s] [EXECUTE] [Step=%s]" ), m_szCmdName, act_string( m_nStep ) );
	}
	m_nStep_Pre = m_nStep;


	for( int i = 0; i < CTaskTrayBasePP::eAxisMax; i++ ) {
		OneAxis* pMotor = pTaskTrayLoadPP->GetAxis( ( CTaskTrayBasePP::eMotor )i );
		if( pMotor == nullptr ) continue;

		if( pMotor->ChkMotorUsable() != ERR_NO_ERROR ) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat( "Tray PP check motor usable Cmd Error" );
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm( ERR_TRAY_PP_1_MOTOR_ALARMED, AlarmData );
			return FALSE;
		}
	}


	OneAxis* pMotorX = pTaskTrayLoadPP->GetAxis( CTaskTrayBasePP::eAxisX );
	OneAxis* pMotorZ = pTaskTrayLoadPP->GetAxis( CTaskTrayBasePP::eAxisZ );
	OneAxis* pMotorPX = pTaskTrayLoadPP->GetAxis( CTaskTrayBasePP::eAxisPitchX );
	OneAxis* pMotorPY = pTaskTrayLoadPP->GetAxis( CTaskTrayBasePP::eAxisPitchY );

	switch( m_nStep )
	{
	case start:
		{
			m_nStep = z_move_safety;
		}break;

	case z_move_safety: // check interlock
		{
			std::vector<CPoint> vPkr;
			pTaskTrayLoadPP->MakeAllPicker(vPkr);
			pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [move safety and picker all up] [SRT]", m_szCmdName, m_nStep );
			int nErr = pTaskTrayLoadPP->do_MovePos( eStage, Pocket, vPkr, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ );
			if( nErr != ERR_NO_ERROR ) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm( nErr, AlarmData );
				break;
			}

			m_tcTimeout.SetCheckTime(TIME_OUT_SHORT);
			m_tcTimeout.StartTimer();
			m_nStep = z_move_safety_chk;
		}break;

	case z_move_safety_chk:
		{
			std::vector<CPoint> vPkr;
			pTaskTrayLoadPP->MakeAllPicker(vPkr);
			m_vErrPicker.clear();
			int nErrSen = pTaskTrayLoadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);
			int nAxisDone = pMotorZ->AxisDone(vPkr, m_vErrPicker);
			if( nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR) {
				pTaskTrayLoadPP->MakeLog("[%s EXEC=%8d] [move safety and picker all up] [END]", m_szCmdName, m_nStep);
				m_nStep = x_px_py_move;
				break;
			}
			else {
				std::vector<CPoint> vErrPkr; vErrPkr.clear();
				CString sAlarmData = _T("");
				int nErr = pTaskTrayLoadPP->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, pTaskTrayLoadPP->GetSafetyFailPicker(ePickerSafetySensorCheckType::eSafetySensorCheck), vErrPkr, sAlarmData);

				if (m_tcTimeout.CheckOverTime() == TRUE) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("%s", sAlarmData);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
					ReportAlarm(nErr, AlarmData, vErrPkr);
					break;
				}
			}		
		}break;
		// move x,y,xp,yp to target pos
	case x_px_py_move:
		{
			double dTPos[ 3 ] = { 0.0, };
			dTPos[ 0 ] = pTaskTrayLoadPP->GetTeachPos( eStage, Pocket, vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX );
			dTPos[ 1 ] = pTaskTrayLoadPP->GetTeachPos( eStage, Pocket, vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisPitchX );
			dTPos[ 2 ] = pTaskTrayLoadPP->GetTeachPos( eStage, Pocket, vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisPitchY );

			int nAxisNo[ 3 ] = { CTaskTrayBasePP::eAxisX, CTaskTrayBasePP::eAxisPitchX, CTaskTrayBasePP::eAxisPitchY };
			pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [x, Xpitch, Ypitch move %s x:%d / y:%d] [SRT]", m_szCmdName, m_nStep, pTaskTrayLoadPP->GetLocName( eStage ), Pocket.x + 1, Pocket.y + 1 );
			for( int i = 0; i < _countof( nAxisNo ); i++ ) {
				int nErr = pTaskTrayLoadPP->do_MovePos( eStage, Pocket, vPicker, CTaskTrayBasePP::eZPOS_SAFETY, ( CTaskTrayBasePP::eMotor )nAxisNo[ i ] );
				if( nErr != ERR_NO_ERROR ) {
					CParamList AlarmData;
					OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm( nErr, AlarmData );
					break;
				}
			}
			m_tcTimeout.SetCheckTime( MOTOR_TIME_OUT_MIDDLE );
			m_tcTimeout.ResetStartTime();
			m_nStep = x_px_py_move_chk;
		}break;

	case x_px_py_move_chk:
		{
			int nAxisNo[ 3 ] = { CTaskTrayBasePP::eAxisX, CTaskTrayBasePP::eAxisPitchX, CTaskTrayBasePP::eAxisPitchY };

			if( m_tcTimeout.CheckOverTime() == TRUE ) {
				CString strErrAxis = _T( "" );
				for( int i = 0; i < _countof( nAxisNo ); i++ ) {
					OneAxis* pMotor = pTaskTrayLoadPP->GetAxis( ( CTaskTrayBasePP::eMotor )nAxisNo[ i ] );
					int nAxisDone = pMotor->AxisDone();
					if( nAxisDone != ERR_NO_ERROR ) {
						if( strErrAxis.Compare( "" ) != 0 ) {
							strErrAxis += ", ";
						}
						strErrAxis += pMotor->GetAxisName();
					}
				}
				double dPosX = 0, dPosPX = 0, dPosPY = 0;
				pMotorX->GetCurrentPos(dPosX);
				pMotorPX->GetCurrentPos(dPosPX);
				pMotorPY->GetCurrentPos(dPosPY);

				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Tray PP axis operation is not complete" );
				AlarmData.m_fnAddItemFormat( "%s motor time over!", strErrAxis );
				AlarmData.m_fnAddItemFormat( "[current position] x=%.2f, x-pitch=%.2f, y-pitch=%.2f", dPosX / 1000.0, dPosPX / 1000.0, dPosPY / 1000.0 );
				ReportAlarm( ERR_TRAY_PP_1_MOTOR_MOVE_TIMEOUT, AlarmData );
				break;
			}

			int nChkComp = 0;
			for( int i = 0; i < _countof( nAxisNo ); i++ ) {
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis( ( CTaskTrayBasePP::eMotor )nAxisNo[ i ] );
				int nAxisDone = pMotor->AxisDone();
				if( nAxisDone == ERR_NO_ERROR ) {
					nChkComp++;
				}
				else if( nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE ) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( "Tray PP axis operation is not complete" );
					AlarmData.m_fnAddItemFormat( "[ check axis done ] x=%d, y=%d, x-pitch=%d, y-pitch=%d", pMotorX->AxisDone(), pMotorPX->AxisDone(), pMotorPY->AxisDone() );
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm( ERR_TRAY_PP_1_MOTOR_MOVE_FAIL, AlarmData );
					break;
				}
			}
			if( nChkComp == _countof( nAxisNo ) ) {
				pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [x, Xpitch, Ypitch move %s x:%d / y:%d] [END]", m_szCmdName, m_nStep, pTaskTrayLoadPP->GetLocName( eStage ), Pocket.x + 1, Pocket.y + 1 );
				m_nStep = end;
				break;
			}
		}break;

	case end:
		{
			// 혹시 안전 센서가 추가되면 마지막 안전 센서 확인 코드를 여기에 넣는다. 안전센서 삭제되어 코드 삭제 함.
			pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [Move Pos : %s, eSeqCmdStatus_NormalComplete]", m_szCmdName, m_nStep, pTaskTrayLoadPP->GetLocName( eStage ) );
			ReportCmdEnd( eSeqCmdStatus_NormalComplete );
		}break;

	}

	return CBaseSeqCmd::State_Executing();
}

BOOL CTrayLoadPPCmd_MovePos::State_Pause()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = ( CTaskTrayLoadPP* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTaskTrayLoadPP->MakeLog( "[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			int err = ERR_NO_ERROR;
			CString strErr = _T("");
			for( int i = 0; i < CTaskTrayLoadPP::eAxisMax; i++ ) {
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis( ( CTaskTrayLoadPP::eMotor )i );
				if (pMotor == nullptr) {
					continue;
				}

				int stop_result = pMotor->SetSlowStop();
				if( stop_result != ERR_NO_ERROR ) {
					err = stop_result;
					strErr += pMotor->GetAxisName();
				}
			}

			if( err != ERR_NO_ERROR ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Load PP Motor Stop Fail." );
				AlarmData.m_fnAddItemFormat("Axis=%s", strErr);
				ReportAlarm( ERR_TRAY_PP_1_MOTOR_MOVE_FAIL, AlarmData );
				break;
			}

			m_tcTimeout.SetCheckTime( TIME_OUT_SHORT );
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
	case 1:
		{
			CString strErr = _T("");
			int motion_done_cnt = 0;
			int motor_count = pTaskTrayLoadPP->GetMotorCount();
			for( int i = 0; i < CTaskTrayLoadPP::eAxisMax; i++ ) {
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis( ( CTaskTrayLoadPP::eMotor )i );
				if (pMotor == nullptr) {
					continue;
				}

				if (pMotor->MotionDone() == ERR_NO_ERROR)
					motion_done_cnt++;
				else
				{
					strErr += pMotor->GetAxisName();
				}				
			}

			if( motion_done_cnt == motor_count ) {
				m_nStep++;
				break;
			}

			if (m_tcTimeout.CheckOverTime()) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Load PP Motor Stop Timeout.");
				AlarmData.m_fnAddItemFormat("Axis=%s", strErr);
				ReportAlarm(ERR_TRAY_PP_1_MOTOR_MOVE_TIMEOUT, AlarmData);
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

BOOL CTrayLoadPPCmd_MovePos::State_Recovery()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = ( CTaskTrayLoadPP* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTaskTrayLoadPP->MakeLog( "[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			pTaskTrayLoadPP->MakeLog( "[%s RECOVERY=%8d] [z move safety & picker all up] [SRT]", m_szCmdName, m_nStep );
			std::vector<CPoint> vPkr;
			pTaskTrayLoadPP->MakeAllPicker(vPkr);
			int nErr = pTaskTrayLoadPP->do_MovePos( eStage, Pocket, vPkr, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ );
			if( nErr != ERR_NO_ERROR ) {
				
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Tray PP z axis move safety position command error" );
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm( nErr, AlarmData );
				break;
			}

			m_tcTimeout.SetCheckTime(TIME_OUT_SHORT);
			m_tcTimeout.ResetStartTime();
			m_nStep = 1000;
		}break;

	case 1000:
		{
			std::vector<CPoint> vPkr;
			pTaskTrayLoadPP->MakeAllPicker(vPkr);

			int nErrSen = pTaskTrayLoadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);
			OneAxis* pMotorZ = pTaskTrayLoadPP->GetAxis( CTaskTrayBasePP::eAxisZ );
			m_vErrPicker.clear();
			int nAxisDone = pMotorZ->AxisDone(vPkr, m_vErrPicker);
			if( nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR ) {
				pTaskTrayLoadPP->MakeLog( "[%s RECOVERY=%8d] [z move safety & picker all up] [END]", m_szCmdName, m_nStep );
				m_nStep = 2000;
				break;
			}
			else {
				std::vector<CPoint> vErrPkr; vErrPkr.clear();
				CString sAlarmData = _T("");
				int nErr = pTaskTrayLoadPP->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, pTaskTrayLoadPP->GetSafetyFailPicker(ePickerSafetySensorCheckType::eSafetySensorCheck), vErrPkr, sAlarmData);

				if (m_tcTimeout.CheckOverTime() == TRUE) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("%s", sAlarmData);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
					ReportAlarm(nErr, AlarmData, vErrPkr);
					break;
				}
			}	
		}break;

	case 2000:
		{
			std::vector<CPoint> vPicker;
			vPicker.push_back( CPoint( 0, 0 ) );

			int nErr = pTaskTrayLoadPP->do_MovePos( LOC_IDX_TRAY_PP_SAFETY, CPoint( 0, 0 ), vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX );
			if( nErr != ERR_NO_ERROR ) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisX);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm( nErr, AlarmData );
				break;
			}
			m_tcTimeout.SetCheckTime( MOTOR_TIME_OUT_MIDDLE );
			m_tcTimeout.ResetStartTime();
			m_nStep = 3000;
		}break;
	case 3000:
		{
			OneAxis* pMotorX = pTaskTrayLoadPP->GetAxis( CTaskTrayBasePP::eAxisX );

			if( m_tcTimeout.CheckOverTime() == TRUE ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Tray PP X axis move y safety time over" );
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorX->GetAxisName());
				ReportAlarm( ERR_TRAY_PP_1_MOTOR_MOVE_TIMEOUT, AlarmData );
				break;
			}

			int nAxisDone = pMotorX->AxisDone();
			if( nAxisDone == ERR_NO_ERROR ) {
				m_nStep = 99999;
				break;
			}
			else if( nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Tray PP x axis axis done fail" );
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorX->GetAxisName());
				ReportAlarm( ERR_TRAY_PP_1_MOTOR_MOVE_FAIL, AlarmData );
				break;
			}
		}break;

	case 99999:
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

BOOL CTrayLoadPPCmd_MovePos::State_NormalComplete()
{
	return CBaseSeqCmd::State_NormalComplete();
}

BOOL CTrayLoadPPCmd_MovePos::State_Alarm()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = ( CTaskTrayLoadPP* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTaskTrayLoadPP->MakeLog( "[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			int err = ERR_NO_ERROR;
			for( int i = 0; i < CTaskTrayLoadPP::eAxisMax; i++ ) {
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis( ( CTaskTrayLoadPP::eMotor )i );
				if (pMotor == nullptr) {
					continue;
				}

				int stop_result = pMotor->SetSlowStop();
				if( stop_result != ERR_NO_ERROR ) {
					err = stop_result;
				}
			}
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

BOOL CTrayLoadPPCmd_MovePos::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete();
}

BOOL CTrayLoadPPCmd_MovePos::State_Aborting()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = ( CTaskTrayLoadPP* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTaskTrayLoadPP->MakeLog( "[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			int nAxisCount = pTaskTrayLoadPP->GetMotorCount();
			for (int i = 0; i < nAxisCount; i++) {
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis(static_cast<CTaskTrayBasePP::eMotor>(i));
				if (pMotor != nullptr) {
					pMotor->SetSlowStop();
				}
			}
			m_nStep = 1;
		}break;
	case 1:
		{
			ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AbortComplete); // 명령 진행 중 STOP 명령을 받아 명령이 중단 됨.
		}break;
	default:
		{
			m_nStep = 0;
		}break;
	}

	return CBaseSeqCmd::State_Aborting();
}

BOOL CTrayLoadPPCmd_MovePos::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}

CString CTrayLoadPPCmd_MovePos::act_string( int act )
{
	BEGIN_CASE_TO_STR( act );
	CASE_TO_STR( start );
	CASE_TO_STR( z_move_safety );
	CASE_TO_STR( z_move_safety_chk );
	CASE_TO_STR( chk_z_safety_sen_all );
	CASE_TO_STR( x_px_py_move );
	CASE_TO_STR( x_px_py_move_chk );
	CASE_TO_STR( end );
	ELSE_CASE( _T( "unkown_step" ) );
	END_CASE_TO_STR();
}
