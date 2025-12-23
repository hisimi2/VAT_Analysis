#include "stdafx.h"
#include "SystemCmd_AxisJogMove.h"


CSystemCmd_AxisJogMove::CSystemCmd_AxisJogMove(int nAxisIdx, int nDir, double dSpeed)
{
	m_nAxisIdx = nAxisIdx;
	m_dir = nDir;
	m_spd_ums = dSpeed;

	m_nCmdId = eSeqCmd_SystemJogMove;
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));
}


CSystemCmd_AxisJogMove::~CSystemCmd_AxisJogMove()
{
}

int CSystemCmd_AxisJogMove::CheckParam()
{
	return ERR_NO_ERROR;
}

int CSystemCmd_AxisJogMove::OnStop()
{
	OneAxis* pMotor = (OneAxis*)g_Motor[m_nAxisIdx];
	if (pMotor != NULL)
	{
		pMotor->SetSlowStop();
		pMotor->OriginStop();
	}

	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

int CSystemCmd_AxisJogMove::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

int CSystemCmd_AxisJogMove::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

int CSystemCmd_AxisJogMove::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

int CSystemCmd_AxisJogMove::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

BOOL CSystemCmd_AxisJogMove::State_Setup()
{
	m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_LONG);
	m_tcTimeout.StartTimer();

	return CBaseSeqCmd::State_Setup();
}

BOOL CSystemCmd_AxisJogMove::State_Executing()
{
	// check alarm
	CTaskSystemCtrl* pTask = (CTaskSystemCtrl*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d]"), m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;


	OneAxis* pMotor = (OneAxis*)g_Motor[m_nAxisIdx];
	if (pMotor == NULL)
	{
		CParamList AlarmData;
		//CString strError;
		//strError.Format("%s Axis invalid error", pMotor->GetAxisName());
		//AlarmData.m_fnAddItemFormat((LPSTR)(LPCSTR)strError);
		ReportAlarm(ERR_SYS_CMD_JOG_INVALID_MOTOR, AlarmData);
		return FALSE;
	}

	double dSpd_umps = m_spd_ums;
// 	switch (m_spd)
// 	{
// 	case 0/*Low*/: { dSpd_umps = pMotor->GetManualMoveSpeed_Low(); } break;
// 	case 1/*Mid*/: { dSpd_umps = pMotor->GetManualMoveSpeed_Middle(); } break;
// 	case 2/*High*/: { dSpd_umps = pMotor->GetManualMoveSpeed_High(); } break;
// 	default: { dSpd_umps = pMotor->GetManualMoveSpeed_Low(); } break;
// 	}

	switch (m_nStep)
	{
	case 0:			// check interlock
	{
		if (m_nAxisIdx >= eAXIS_PRESS_UNIT_DN1 && m_nAxisIdx <= eAXIS_PRESS_UNIT_DN8)
		{
			int nInterlockErr = g_TaskSystemCtrl.CheckManualable(true);
			if (nInterlockErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				ReportAlarm(nInterlockErr, AlarmData);
				break;
			}
		}

		m_nStep = 1000;
	}break;
	case 1000:
	{
		int nRet = pMotor->JogMove(m_dir, dSpd_umps, 100);
		if (nRet != ERR_SUCCESS)
		{
			CParamList AlarmData;
			CString strError;
			strError.Format("%s Axis Jog Move command error", pMotor->GetAxisName());
			AlarmData.m_fnAddItemFormat((LPSTR)(LPCSTR)strError);
			ReportAlarm(nRet, AlarmData);
			return FALSE;
		}
		m_nStep++;
	}break;
	case 1001:	// Jog 동작 중 Interlock / Jog Move Done 확인
	{
		int nChk = pMotor->MotionDone();
		if (nChk == ERR_SUCCESS) {
			m_nStep = 1002;
			break;
		}
		else
		{
			m_nStep = 1032;
		}
	}break;
	case 1002:
	{
		m_nStep = 99999;
	}break;
	case 1032: // Jog Move Interlock
	{
		//Interlock
 		if (pMotor->m_cbInterLock_Jog != NULL)
 		{
 			ERR_CODE nInterLockErr = pMotor->m_cbInterLock_Jog(m_dir, dSpd_umps, FALSE);
 			if (nInterLockErr != ERR_SUCCESS) {
 				CParamList AlarmData;
 				CString strError;
 				strError.Format("%s Axis Jog interlock fail", pMotor->GetAxisName());
 				AlarmData.m_fnAddItemFormat((LPSTR)(LPCSTR)strError);
 				ReportAlarm(nInterLockErr, AlarmData);
 				return FALSE;
 			}
 		}
 		else
 		{
 			CParamList AlarmData;
 			CString strError;
 			strError.Format("%s Axis Jog interlock invalid", pMotor->GetAxisName());
 			AlarmData.m_fnAddItemFormat((LPSTR)(LPCSTR)strError);
 			ReportAlarm(ERR_SYS_CMD_JOG_INVALID_MOTOR_INTERLOCK, AlarmData);
 			return FALSE;
 		}
		m_nStep = 1001;
	}break;
	case 99999:
	{
		ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
	}break;
	}
	return CBaseSeqCmd::State_Executing();
}

BOOL CSystemCmd_AxisJogMove::State_Pause()
{
	CTaskSystemCtrl* pTask = (CTaskSystemCtrl*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTask->MakeLog(_T("[CMD: %s] [PAUSE] [Step=%8d]"), m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0:
	{
		OneAxis* pMotor = (OneAxis*)g_Motor[m_nAxisIdx];
		if (pMotor != NULL)
		{
			pMotor->SetSlowStop();
			pMotor->OriginStop();
		}

		m_nStep++;
	}break;
	case 1:
	{
		// Initialize 명령의 경우 Pause가 들어 오면 명령을 완료 시키고 삭제한다.
		SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	}break;
	}

	return CBaseSeqCmd::State_Pause();
}

BOOL CSystemCmd_AxisJogMove::State_Recovery()
{
	CTaskSystemCtrl* pTask = (CTaskSystemCtrl*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTask->MakeLog(_T("[CMD: %s] [RECOVERY] [Step=%8d]"), m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0:
	{
		SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing);
	}break;
	}
	return CBaseSeqCmd::State_Recovery();
}

BOOL CSystemCmd_AxisJogMove::State_NormalComplete()
{
	ClearMotorCmd();
	CTaskSystemCtrl* pTask = (CTaskSystemCtrl*)m_pReceive;
	return CBaseSeqCmd::State_NormalComplete();
}

BOOL CSystemCmd_AxisJogMove::State_Alarm()
{
	CTaskSystemCtrl* pTask = (CTaskSystemCtrl*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTask->MakeLog(_T("[CMD: %s] [ALARM] [Step=%8d]"), m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0: // alarm 발생 했을 때 처리해 줘야 할꺼 처리 해줌
	{
		OneAxis* pMotor = (OneAxis*)g_Motor[m_nAxisIdx];
		if (pMotor != NULL)
		{
			pMotor->SetSlowStop();
			pMotor->OriginStop();
		}
		m_nStep++;
	}break;
	case 1:
	{
		// Initialize 명령의 경우 Alarm이 발생되면 명령을 완료 시키고 삭제한다.
		SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	}break;
	}
	return CBaseSeqCmd::State_Alarm();
}

BOOL CSystemCmd_AxisJogMove::State_AlarmComplete()
{
	ClearMotorCmd();
	return CBaseSeqCmd::State_AlarmComplete();
}

BOOL CSystemCmd_AxisJogMove::State_Aborting()
{
	CTaskSystemCtrl* pTask = (CTaskSystemCtrl*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTask->MakeLog(_T("[CMD: %s] [ABORT] [Step=%8d]"), m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0:
	{
		OneAxis* pMotor = (OneAxis*)g_Motor[m_nAxisIdx];
		if (pMotor != NULL)
		{
			pMotor->SetSlowStop();
			pMotor->OriginStop();
		}
		m_nStep++;
	}break;
	case 1:
	{
		ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AbortComplete); // 명령 진행 중 STOP 명령을 받아 명령이 중단 됨.
	}break;
	}
	return CBaseSeqCmd::State_Aborting();
}

BOOL CSystemCmd_AxisJogMove::State_AbortComplete()
{
	ClearMotorCmd();
	return CBaseSeqCmd::State_AbortComplete();
}

void CSystemCmd_AxisJogMove::ClearMotorCmd()
{
	OneAxis* pMotor = (OneAxis*)g_Motor[m_nAxisIdx];
	pMotor->ClearStatus();
	pMotor->FramesClear();
	Sleep(200);
}
