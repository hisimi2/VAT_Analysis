#include "StdAfx.h"
#include "TrayLoadPPCmd_Initial.h"

CTrayLoadPPCmd_Initial::CTrayLoadPPCmd_Initial( void )
{
	m_nCmdId = eSeqCmd_TrayLoadPP_Initial;
	m_nRetryCount = 0;
	m_nOrgRetryCount = 0;
	sprintf_s( m_szCmdName, sizeof( m_szCmdName ), "TrayLoadPPCmd_Initial" );
}

CTrayLoadPPCmd_Initial::~CTrayLoadPPCmd_Initial( void )
{
}

int CTrayLoadPPCmd_Initial::CheckParam()
{
	return ERR_NO_ERROR;
}

int CTrayLoadPPCmd_Initial::OnStop()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Aborting );
	return ERR_NO_ERROR;
}

int CTrayLoadPPCmd_Initial::OnRetry()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

int CTrayLoadPPCmd_Initial::OnSkip()
{
	ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_AlarmComplete );
	return ERR_NO_ERROR;
}

int CTrayLoadPPCmd_Initial::OnPause()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Pause );
	return ERR_NO_ERROR;
}

int CTrayLoadPPCmd_Initial::OnResume()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

BOOL CTrayLoadPPCmd_Initial::State_Setup()
{
	m_tcTimeout.SetCheckTime( MOTOR_TIME_OUT_MIDDLE );
	m_tcTimeout.StartTimer();

	return CBaseSeqCmd::State_Setup();
}

BOOL CTrayLoadPPCmd_Initial::State_Executing()
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

	OneAxis* pMotorX = pTaskTrayLoadPP->GetAxis( CTaskTrayBasePP::eAxisX );
	OneAxis* pMotorZ = pTaskTrayLoadPP->GetAxis( CTaskTrayBasePP::eAxisZ );
	OneAxis* pMotorPX = pTaskTrayLoadPP->GetAxis( CTaskTrayBasePP::eAxisPitchX );
	OneAxis* pMotorPY = pTaskTrayLoadPP->GetAxis( CTaskTrayBasePP::eAxisPitchY );

	switch (m_nStep)
	{
	case start:
	{
		pTaskTrayLoadPP->m_bInitialized = FALSE;
		m_nStep = z_clear;
	}break;

	//--------------------------------------------------------------------------
	// z축 원점
	case z_clear:
	{
		pMotorZ->ClearOrgFlag();
		pMotorZ->SetStop();
		Sleep(100);
		pMotorZ->AlarmReset();
		Sleep(100);
		pMotorZ->ServoOn();
		Sleep(100);
		pMotorZ->EnableSWLimit(DEF_DISABLE);
		m_tcTimeout.SetCheckTime(TIME_OUT_SHORT * 2);
		m_tcTimeout.ResetStartTime();
		m_nStep = z_org;
	}break;

	case z_org:
	{
		int nMaxRetry = MAX_MOTOR_RETRY_COUNT;
		if (m_tcTimeout.CheckOverTime() == TRUE) {
			if (m_nRetryCount > nMaxRetry) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				CBaseSeqCmd::ReportAlarm(ERR_TRAY_PP_1_MOTOR_ORG_TIMEOUT, AlarmData);
			}
			else {
				m_nRetryCount++;
				m_nStep = z_clear;
				break;
			}
		}

		int nErrOrg = 0;
		nErrOrg = pMotorZ->Origin();
		if (nErrOrg != ERR_NO_ERROR) {
			m_nOrgRetryCount++;
			if (m_nOrgRetryCount >= 2) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				ReportAlarm(nErrOrg, AlarmData);
				break;
			}

			m_nStep = z_clear;
			break;
		}
		else
		{
			BOOL bCompleted = pMotorZ->IsOrgCompleted(&nErrOrg);
			if (bCompleted) {
				pMotorZ->EnableSWLimit(DEF_ENABLE);
				int nErr = pMotorZ->ChkMotorUsable();
				if (nErr == ERR_NO_ERROR) {
					m_nStep = z_move_safety;
					break;
				}
			}
			else {

				if (m_tcTimeout.CheckOverTime() == TRUE) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("%s Motor Org Fail", pMotorZ->GetAxisName());
					ReportAlarm(ERR_TRAY_PP_1_MOTOR_ORG_FAIL, AlarmData);
					break;
				}

				// 					else {
				// 						CParamList AlarmData;
				// 						AlarmData.m_fnAddItemFormat( "%s Axis", pMotorZ->GetAxisName() );
				// 						ReportAlarm( nErrOrg, AlarmData );
				// 						break;
				// 					}
			}
		}
	}break;

	// z축 안전 위치로 이동.
	case z_move_safety:
	{
		std::vector<CPoint> vPkr;
		pTaskTrayLoadPP->MakeAllPicker(vPkr);
	
		int nErr = pTaskTrayLoadPP->do_MovePos(LOC_IDX_TRAY_PP_SAFETY, CPoint(0, 0), vPkr, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);

		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray Unload PP z axis move safety position command error");
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
			ReportAlarm(nErr, AlarmData);
			break;
		}

		m_nStep = z_move_safety_chk;
		m_tcTimeout.SetCheckTime(TIME_OUT_SHORT);
		m_tcTimeout.ResetStartTime();
	}break;

	case z_move_safety_chk:
	{
		std::vector<CPoint> vPkr;
		pTaskTrayLoadPP->MakeAllPicker(vPkr);
		m_vErrPicker.clear();
		int nErrSen = pTaskTrayLoadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);
		int nAxisDone = pMotorZ->AxisDone(vPkr, m_vErrPicker);
		if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR) {
			m_nStep = pitch_clear;
			m_nRetryCount = 0;
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
	//--------------------------------------------------------------------------
	// y, pitch x, pitch y axis org
	case pitch_clear:
	{
		OneAxis* pMotor[2] = { pMotorPX, pMotorPY };
		for (int i = 0; i < _countof(pMotor); i++) {
			pMotor[i]->ClearOrgFlag();
			pMotor[i]->SetStop();
			Sleep(100);
			pMotor[i]->AlarmReset();
			Sleep(100);
			pMotor[i]->ServoOn();
			Sleep(100);
			pMotor[i]->EnableSWLimit(DEF_DISABLE);
		}
		m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
		m_tcTimeout.ResetStartTime();
		m_nStep = pitch_org;
	}break;

	case pitch_org:
	{
		OneAxis* pMotor[2] = { pMotorPX, pMotorPY };
		int nMaxRetry = MAX_MOTOR_RETRY_COUNT;
		if (m_tcTimeout.CheckOverTime() == TRUE) {
			if (m_nRetryCount > nMaxRetry) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorPX->GetAxisName());
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorPY->GetAxisName());
				CBaseSeqCmd::ReportAlarm(ERR_TRAY_PP_1_MOTOR_ORG_FAIL, AlarmData);
			}
			else {
				m_nRetryCount++;
				m_nStep = pitch_clear;
				break;
			}
		}



		int nErrOrg = 0;
		int nComp = 0;
		for (int i = 0; i < _countof(pMotor); i++) {
			nErrOrg = pMotor[i]->Origin();
			if (nErrOrg != ERR_NO_ERROR) {
				m_nOrgRetryCount++;
				if (m_nOrgRetryCount >= 2) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
					ReportAlarm(nErrOrg, AlarmData);
					break;
				}

				m_nStep = pitch_clear;
				break;
			}
			else {
				BOOL bCompleted = pMotor[i]->IsOrgCompleted(&nErrOrg);
				if (bCompleted) {
					pMotor[i]->EnableSWLimit(DEF_ENABLE);
					int nErr = pMotor[i]->ChkMotorUsable();
					if (nErr == ERR_NO_ERROR) {
						nComp++;
					}
				}
				else if (nErrOrg != ERR_MOTOR_CMN_IN_MOVE_STATE) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("%s Axis", pMotor[i]->GetAxisName());
					ReportAlarm(nErrOrg, AlarmData);
					break;
				}
			}
		}// for end

		if (nComp == _countof(pMotor)) {
			m_nStep = x_clear;
		}
	}break;

	//--------------------------------------------------------------------------
	// x org
	case x_clear:
	{
		OneAxis* pMotor[1] = { pMotorX };
		for (int i = 0; i < _countof(pMotor); i++) {
			pMotor[i]->ClearOrgFlag();
			pMotor[i]->SetStop();
			Sleep(100);
			pMotor[i]->AlarmReset();
			Sleep(100);
			pMotor[i]->ServoOn();
			Sleep(100);
			pMotor[i]->EnableSWLimit(DEF_DISABLE);
		}
		m_tcTimeout.ResetStartTime();
		m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
		m_nStep = x_org;
	}break;
	case x_org:
	{
		int nMaxRetry = MAX_MOTOR_RETRY_COUNT;
		if (m_tcTimeout.CheckOverTime() == TRUE) {
			if (m_nRetryCount > nMaxRetry) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorX->GetAxisName());
				CBaseSeqCmd::ReportAlarm(ERR_TRAY_PP_1_MOTOR_ORG_FAIL, AlarmData);
				break;
			}
			else {
				m_nRetryCount++;
				m_nStep = x_clear;
				break;
			}
		}

		OneAxis* pMotor[1] = { pMotorX };

		int nErrOrg = 0;
		int nComp = 0;
		for (int i = 0; i < _countof(pMotor); i++) {
			nErrOrg = pMotor[i]->Origin();
			if (nErrOrg != ERR_NO_ERROR) {
				m_nOrgRetryCount++;
				if (m_nOrgRetryCount >= 2) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
					ReportAlarm(nErrOrg, AlarmData);
					break;
				}

				m_nStep = pitch_clear;
				break;
			}
			else {
				BOOL bCompleted = pMotor[i]->IsOrgCompleted(&nErrOrg);
				if (bCompleted) {
					pMotor[i]->EnableSWLimit(DEF_ENABLE);
					int nErr = pMotor[i]->ChkMotorUsable();
					if (nErr == ERR_NO_ERROR) {
						nComp++;
					}
				}
				else if (nErrOrg != ERR_MOTOR_CMN_IN_MOVE_STATE) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("%s Axis", pMotor[i]->GetAxisName());
					ReportAlarm(nErrOrg, AlarmData);
					break;
				}
			}
		}// for end

		if (nComp == _countof(pMotor)) {
			m_nStep = org_complete;
		}
	}break;
	case org_complete:
	{
		ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
	}break;
	}
	return CBaseSeqCmd::State_Executing();
}

BOOL CTrayLoadPPCmd_Initial::State_Pause()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = ( CTaskTrayLoadPP* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTaskTrayLoadPP->MakeLog( "[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d] ", m_szCmdName, m_nStep, m_nLast_Executing_Step );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			int nAxisCount = CTaskTrayLoadPP::eAxisMax;
			for( int i = 0; i < nAxisCount; i++ ) {
				OneAxis* pMotor = ( OneAxis* )pTaskTrayLoadPP->GetAxis( ( CTaskTrayLoadPP::eMotor )i );
				if( pMotor == nullptr ) {
					continue;
				}
				pMotor->SetStop();
				pMotor->OriginStop();
			}
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

BOOL CTrayLoadPPCmd_Initial::State_Recovery()
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
	case 0: // 현재 위치와 마지막 위치를 비교 한다.
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

BOOL CTrayLoadPPCmd_Initial::State_NormalComplete()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = ( CTaskTrayLoadPP* )m_pReceive;
	pTaskTrayLoadPP->m_bInitialized = TRUE;

	return CBaseSeqCmd::State_NormalComplete();
}

BOOL CTrayLoadPPCmd_Initial::State_Alarm()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = ( CTaskTrayLoadPP* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTaskTrayLoadPP->MakeLog( "[CMD: %s] [ALARM] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0: // alarm 발생 했을 때 처리해 줘야 할꺼 처리 해줌
		{
			int nAxisCount = CTaskTrayLoadPP::eAxisMax;
			for( int i = 0; i < nAxisCount; i++ ) {
				OneAxis* pMotor = ( OneAxis* )pTaskTrayLoadPP->GetAxis( ( CTaskTrayLoadPP::eMotor )i );
				if( pMotor == nullptr ) {
					continue;
				}
				pMotor->SetStop();
				pMotor->OriginStop();
			}
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

BOOL CTrayLoadPPCmd_Initial::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete();
}

BOOL CTrayLoadPPCmd_Initial::State_Aborting()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = ( CTaskTrayLoadPP* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTaskTrayLoadPP->MakeLog( "[CMD: %s] [ABORT] [Step=%8d]", m_szCmdName, m_nStep );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			int nAxisCount = CTaskTrayLoadPP::eAxisMax;
			for( int i = 0; i < nAxisCount; i++ ) {
				OneAxis* pMotor = ( OneAxis* )pTaskTrayLoadPP->GetAxis( ( CTaskTrayLoadPP::eMotor )i );
				if( pMotor == nullptr ) {
					continue;
				}
				pMotor->SetStop();
				pMotor->OriginStop();
				pMotor->EnableSWLimit( DEF_ENABLE );
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

BOOL CTrayLoadPPCmd_Initial::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}

CString CTrayLoadPPCmd_Initial::act_string( int act )
{
	BEGIN_CASE_TO_STR( act );
	CASE_TO_STR( start );
	CASE_TO_STR( z_clear );
	CASE_TO_STR( z_org );
	CASE_TO_STR( z_move_safety );
	CASE_TO_STR( z_move_safety_chk );
	CASE_TO_STR( pitch_clear );
	CASE_TO_STR( pitch_org );
	CASE_TO_STR( x_clear );
	CASE_TO_STR(x_org); 
	CASE_TO_STR(org_complete);
	CASE_TO_STR( end );
	ELSE_CASE( _T( "unkown_step" ) );
	END_CASE_TO_STR();
}
