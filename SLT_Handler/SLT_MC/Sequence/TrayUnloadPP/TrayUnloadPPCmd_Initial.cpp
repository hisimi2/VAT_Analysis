#include "StdAfx.h"
#include "TrayUnloadPPCmd_Initial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CTrayUnloadPPCmd_Initial::CTrayUnloadPPCmd_Initial( void )
{
	m_nCmdId = eSeqCmd_TrayUnloadPP_Initial;
	m_nRetryCount = 0;
	m_nOrgRetryCount = 0;
	sprintf_s(m_szCmdName, sizeof(m_szCmdName), "TrayUnloadPPCmd_Initial");
}

CTrayUnloadPPCmd_Initial::~CTrayUnloadPPCmd_Initial( void )
{
}

int CTrayUnloadPPCmd_Initial::CheckParam()
{
	return ERR_NO_ERROR;
}

int CTrayUnloadPPCmd_Initial::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

int CTrayUnloadPPCmd_Initial::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

int CTrayUnloadPPCmd_Initial::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

int CTrayUnloadPPCmd_Initial::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

int CTrayUnloadPPCmd_Initial::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

BOOL CTrayUnloadPPCmd_Initial::State_Setup()
{
	m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
	m_tcTimeout.StartTimer();

	return CBaseSeqCmd::State_Setup();
}

BOOL CTrayUnloadPPCmd_Initial::State_Executing()
{
	// check alarm
	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	m_nStep_Pre = m_nStep;
	int i = 0;

	OneAxis* pMotorX = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisX);
	OneAxis* pMotorY = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisY);
	OneAxis* pMotorZ = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisZ);
	OneAxis* pMotorPX = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisPitchX);
	OneAxis* pMotorPY = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisPitchY);

	switch (m_nStep)
	{
	case 0:			// check interlock
	{
		pTaskTrayUnloadPP->m_bInitialized = FALSE;
		m_nStep = 2000;
	}break;

	//--------------------------------------------------------------------------
	// z축 원점
	case 2000:
	{
		pMotorZ->ClearOrgFlag();
		pMotorZ->SetStop();
		Sleep(100);
		pMotorZ->AlarmReset();
		Sleep(100);
		pMotorZ->ServoOn();
		Sleep(100);
		pMotorZ->EnableSWLimit(DEF_DISABLE);
		m_tcTimeout.SetCheckTime(TIME_OUT_SHORT*2);
		m_tcTimeout.ResetStartTime();
		m_nStep++;
	}break;

	case 2001:
	{
		int nMaxRetry = MAX_MOTOR_RETRY_COUNT;

		int nErrOrg = 0;
		nErrOrg = pMotorZ->Origin();
		if (nErrOrg != NO_ERROR) 
		{
			m_nOrgRetryCount++;
			if (m_nOrgRetryCount >= 2) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				ReportAlarm(nErrOrg, AlarmData);
				break;
			}
			m_nStep--;
		}
		else
		{
			BOOL bCompleted = pMotorZ->IsOrgCompleted(&nErrOrg);
			if (bCompleted)
			{
				pMotorZ->EnableSWLimit(DEF_ENABLE);
				int nErr = pMotorZ->ChkMotorUsable();
				if (nErr == ERR_NO_ERROR) {
					m_nStep++; //= 2500;
				}
			}
			else
			{
				if (m_tcTimeout.CheckOverTime() == TRUE) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
					CBaseSeqCmd::ReportAlarm(ERR_TRAY_PP_2_MOTOR_ORG_TIMEOUT, AlarmData);
					break;
				}
			}		
		}
	}break;

	// z축 안전 위치로 이동.
	case 2002:
	{
		std::vector<CPoint> vPkr;
		pTaskTrayUnloadPP->MakeAllPicker(vPkr);

		int nErr = pTaskTrayUnloadPP->do_MovePos(LOC_IDX_TRAY_PP_SAFETY, CPoint(0, 0), vPkr, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);

		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray Unload PP z axis move safety position command error");
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
			ReportAlarm(nErr, AlarmData);
			break;
		}

		m_nStep++;
		m_tcTimeout.ResetStartTime();
	}break;

	case 2003: // Picker Safety Move Pos Done
	{
		std::vector<CPoint> vPkr;
		pTaskTrayUnloadPP->MakeAllPicker(vPkr);
		int nErrSen = pTaskTrayUnloadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);
		m_vErrPicker.clear();
		int nAxisDone = pMotorZ->AxisDone(vPkr, m_vErrPicker);
		if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR /*&& nErrPicker == ERR_NO_ERROR */) {
			m_nStep = 2500;
			break;
		}
		else
		{
			std::vector<CPoint> vErrPkr; vErrPkr.clear();
			CString sAlarmData = _T("");
			int nErr = pTaskTrayUnloadPP->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, pTaskTrayUnloadPP->GetSafetyFailPicker(ePickerSafetySensorCheckType::eSafetySensorCheck), vErrPkr, sAlarmData);

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
	case 2500:
	{
		OneAxis* pMotor[3] = { pMotorY, pMotorPX, pMotorPY };
		for (i = 0; i < _countof(pMotor); i++) {
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
		m_nStep++;
	}break;

	case 2501:
	{
		int nMaxRetry = MAX_MOTOR_RETRY_COUNT;

		OneAxis* pMotor[3] = { pMotorY, pMotorPX, pMotorPY };

		int nErrOrg = 0;
		int nComp = 0;
		for (i = 0; i < _countof(pMotor); i++) {
			nErrOrg = pMotor[i]->Origin();
			if (nErrOrg != NO_ERROR) {
				m_nOrgRetryCount++;
				if (m_nOrgRetryCount >= 2) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
					ReportAlarm(nErrOrg, AlarmData);
					break;
				}
				m_nStep--;
			}
			else
			{
				BOOL bCompleted = pMotor[i]->IsOrgCompleted(&nErrOrg);
				if (bCompleted)
				{
					pMotor[i]->EnableSWLimit(DEF_ENABLE);
					int nErr = pMotor[i]->ChkMotorUsable();
					if (nErr == ERR_NO_ERROR) {
						nComp++;
					}
				}				
				else 
				{
					if (nErrOrg == ERR_MOTOR_CMN_IN_MOVE_STATE)
					{
						if (m_tcTimeout.CheckOverTime() == TRUE) {
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
							CBaseSeqCmd::ReportAlarm(ERR_TRAY_PP_2_MOTOR_ORG_FAIL, AlarmData);
							break;
						}
					}
					else
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
						ReportAlarm(ERR_TRAY_PP_2_MOTOR_ORG_FAIL, AlarmData);
						break;
					}					
				}
			}
		}// for end

		if (nComp == _countof(pMotor)) {
			m_nStep = 3000;

			if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
			{
				pTaskTrayUnloadPP->chk_TrayUnload_Y_Safety_sen(DEF_ON);
			}
		}
	}break;
	//--------------------------------------------------------------------------
	// x org
	case 3000:
	{
		OneAxis* pMotor[1] = { pMotorX };
		for (i = 0; i < _countof(pMotor); i++) {
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
		m_nStep++;
	}break;
	case 3001:
	{
		int nMaxRetry = MAX_MOTOR_RETRY_COUNT;

		OneAxis* pMotor[1] = { pMotorX };

		int nErrOrg = 0;
		int nComp = 0;
		for (i = 0; i < _countof(pMotor); i++) {
			nErrOrg = pMotor[i]->Origin();
			if (nErrOrg != ERR_NO_ERROR) {
				m_nOrgRetryCount++;
				if (m_nOrgRetryCount >= 2) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
					ReportAlarm(nErrOrg, AlarmData);
					break;
				}
				m_nStep--;
			}
			else 
			{
				BOOL bCompleted = pMotor[i]->IsOrgCompleted(&nErrOrg);
				if (bCompleted)
				{
					pMotor[i]->EnableSWLimit(DEF_ENABLE);
					int nErr = pMotor[i]->ChkMotorUsable();
					if (nErr == ERR_NO_ERROR) {
						nComp++;
					}
				}
				else {
					if (nErrOrg == ERR_MOTOR_CMN_IN_MOVE_STATE)
					{
						if (m_tcTimeout.CheckOverTime() == TRUE) 
						{
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
							CBaseSeqCmd::ReportAlarm(ERR_TRAY_PP_2_MOTOR_ORG_FAIL, AlarmData);
							break;
						}
					}
					else {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
						ReportAlarm(ERR_TRAY_PP_2_MOTOR_ORG_FAIL, AlarmData);
						break;
					}				
				}
			}
		}// for end

		if (nComp == _countof(pMotor)) {
			m_nStep = 4000;
			//ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
		}
	}break;
	case 4000:
		{	
			ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
		}break;
	}
	return CBaseSeqCmd::State_Executing();
}

BOOL CTrayUnloadPPCmd_Initial::State_Pause()
{
	CTaskTrayUnloadPP* pTaskTrayPP = (CTaskTrayUnloadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTrayPP->MakeLog("[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d] ", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0:
	{
		int nAxisCount = pTaskTrayPP->GetMotorCount();
		for (int i = 0; i < nAxisCount; i++) {
			OneAxis* pMotor = (OneAxis*)pTaskTrayPP->GetAxis(CTaskTrayBasePP::eMotor(i));
			pMotor->SetStop();
			pMotor->OriginStop();
		}
		m_nStep++;
	}break;
	case 1:
	{
		// Initialize 명령의 경우 Pause가 들어 오면 명령을 완료 시키고 삭제한다.
		SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	}break;
	default:
	{
		m_nStep = 0;
	}break;
	}

	return CBaseSeqCmd::State_Pause();
}

BOOL CTrayUnloadPPCmd_Initial::State_Recovery()
{
	CTaskTrayUnloadPP* pTaskTrayPP = (CTaskTrayUnloadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTrayPP->MakeLog("[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0: // 현재 위치와 마지막 위치를 비교 한다.
	{
		SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing);
	}break;
	default:
	{
		m_nStep = 0;
	}break;
	}

	return CBaseSeqCmd::State_Recovery();
}

BOOL CTrayUnloadPPCmd_Initial::State_NormalComplete()
{
	CTaskTrayUnloadPP* pTaskTrayPP = (CTaskTrayUnloadPP*)m_pReceive;
	pTaskTrayPP->m_bInitialized = TRUE;

	return CBaseSeqCmd::State_NormalComplete();
}

BOOL CTrayUnloadPPCmd_Initial::State_Alarm()
{
	CTaskTrayUnloadPP* pTaskTrayPP = (CTaskTrayUnloadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTrayPP->MakeLog("[CMD: %s] [ALARM] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0: // alarm 발생 했을 때 처리해 줘야 할꺼 처리 해줌
	{
		int nAxisCount = pTaskTrayPP->GetMotorCount();
		for (int i = 0; i < nAxisCount; i++) {
			OneAxis* pMotor = pTaskTrayPP->GetAxis(CTaskTrayBasePP::eMotor(i));
			pMotor->SetStop();
			pMotor->OriginStop();
		}
		m_nStep++;
	}break;
	case 1:
	{
		// Initialize 명령의 경우 Alarm이 발생되면 명령을 완료 시키고 삭제한다.
		SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	}break;
	default:
	{
		m_nStep = 0;
	}break;
	}
	return CBaseSeqCmd::State_Alarm();
}

BOOL CTrayUnloadPPCmd_Initial::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete();
}

BOOL CTrayUnloadPPCmd_Initial::State_Aborting()
{
	CTaskTrayUnloadPP* pTaskTrayPP = (CTaskTrayUnloadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTrayPP->MakeLog("[CMD: %s] [ABORT] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0:
	{
		int nAxisCount = pTaskTrayPP->GetMotorCount();
		for (int i = 0; i < nAxisCount; i++) {
			OneAxis* pMotor = pTaskTrayPP->GetAxis(CTaskTrayBasePP::eMotor(i));
			pMotor->SetStop();
			pMotor->OriginStop();
			pMotor->EnableSWLimit(DEF_ENABLE);
		}
		m_nStep++;
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

BOOL CTrayUnloadPPCmd_Initial::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}
