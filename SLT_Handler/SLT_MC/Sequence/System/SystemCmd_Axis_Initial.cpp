#include "StdAfx.h"
#include "SystemCmd_Axis_Initial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//==============================================================================
//
//==============================================================================
CSystemCmd_Axis_Initial::CSystemCmd_Axis_Initial(int nAxisIdx)
{
	m_nCmdId = eSeqCmd_SystemOrigin;

	m_nAxisIdx = nAxisIdx;
	m_nOrgRetryCount = 0;
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));
}



//==============================================================================
//
//==============================================================================
CSystemCmd_Axis_Initial::~CSystemCmd_Axis_Initial(void)
{
}


//==============================================================================
//
//==============================================================================
int CSystemCmd_Axis_Initial::CheckParam()
{
	return ERR_SUCCESS;
}


//==============================================================================
//
//==============================================================================
int  CSystemCmd_Axis_Initial::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_SUCCESS;
}

//==============================================================================
//
//==============================================================================
int  CSystemCmd_Axis_Initial::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_SUCCESS;
}

//==============================================================================
//
//==============================================================================
int  CSystemCmd_Axis_Initial::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_SUCCESS;
}

//==============================================================================
//
//==============================================================================
int  CSystemCmd_Axis_Initial::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_SUCCESS;
}

//==============================================================================
//
//==============================================================================
int  CSystemCmd_Axis_Initial::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_SUCCESS;
}

//==============================================================================
//
//==============================================================================
BOOL CSystemCmd_Axis_Initial::State_Setup()
{
	m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_LONG);
	m_tcTimeout.StartTimer(); 

	return CBaseSeqCmd::State_Setup();
}

//==============================================================================
//
//==============================================================================
BOOL CSystemCmd_Axis_Initial::State_Executing()
{
	// check alarm
	CTaskSystemCtrl* pTask = (CTaskSystemCtrl*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		//pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d]"), m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	OneAxis* pMotor = (OneAxis*)g_Motor[m_nAxisIdx];
	if (pMotor == NULL)
	{
		CParamList AlarmData;
		ReportAlarm(ERR_MOTOR_CMN_ORIGIN_FAILURE, AlarmData);
		return FALSE;
	}

	switch (m_nStep)
	{
	case 0:		// check interlock
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

		pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] [Axis=%s]"), m_szCmdName, m_nStep, pMotor->GetAxisName());
		if (0 <= m_nAxisIdx && m_nAxisIdx < g_Motor.GetAxisCount())
		{
			m_nOrgRetryCount = 0;
			m_nStep = 1000;
		}
		else
		{
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(ERR_MOTOR_CMN_ORIGIN_FAILURE, AlarmData);
			break;
		}
	}break;
	case 1000: // org
	{
		pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] [RESET] [STR] "), m_szCmdName, m_nStep);
		pMotor->ClearOrgFlag();
		pMotor->SetStop();
		Sleep(100);
		pMotor->EnableSWLimit(DEF_DISABLE);
		pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] [RESET] [END] "), m_szCmdName, m_nStep);
		m_tcTimeout.ResetStartTime();
		m_nStep++;
		pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] [ORIGIN] [STR] "), m_szCmdName, m_nStep);
		
		if (m_nAxisIdx >= eAXIS_LOAD_TABLE_1_ROTATE && m_nAxisIdx <= eAXIS_LOAD_TABLE_4_ROTATE)	// rotator 축은 DI 확인 한다.
		{
			//2호기 부터 Rotate Stop Sensor 감지 확인.
			int nEQPNo = g_DMCont.m_dmEQP.GN(NDM0_Handler_EQP_No);
			if (nEQPNo > DEF_EQUIPMENT_UNIT1)
			{
				int nTblIdx = m_nAxisIdx - eAXIS_LOAD_TABLE_1_ROTATE;
				if (g_TaskLoadTbl[nTblIdx].IsTableStackerImpactArea() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("%s Axis", pMotor->GetAxisName());
					ReportAlarm(ERR_LOAD_TBL_1_POS_STACKER_IMPACT_AREA + nTblIdx, AlarmData);
					break;
				}
			}


			int nInterLockErr = ERR_NO_ERROR;
			if (pMotor->m_cbInterLock_Org != nullptr) {
				nInterLockErr = pMotor->m_cbInterLock_Org();

				if( nInterLockErr != ERR_NO_ERROR)
				{				
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("%s Axis", pMotor->GetAxisName());
					ReportAlarm(nInterLockErr, AlarmData);
					break;
				}
			}

			int nLdStopSenIONo[MAX_LOAD_TABLE] = { DI_LOADER_TABLE_1_ROTATOR_1_SEN,
												   DI_LOADER_TABLE_2_ROTATOR_1_SEN,
												   DI_LOADER_TABLE_3_ROTATOR_2_SEN,
												   DI_LOADER_TABLE_4_ROTATOR_2_SEN };


			// Loader table 1,2 축은 2번 IO가 감지되면 역방향으로 회전,
			//              3,4 축은 1번 IO가 감지되면 역방향으로 회전 한다. 

			if (g_IO.in(nLdStopSenIONo[m_nAxisIdx - eAXIS_LOAD_TABLE_1_ROTATE]) == DEF_ON)
			{
				m_nStep = 1002;
				m_tcTimeout.ResetStartTime();
				break;
			}
		}

	}break;
	case 1001:
	{

		int nErrOrg = pMotor->Origin();
		if (nErrOrg != ERR_SUCCESS)
		{
			m_nOrgRetryCount++;
			if (m_nOrgRetryCount >= 2)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("%s Axis", pMotor->GetAxisName());
				ReportAlarm(nErrOrg, AlarmData);
				break;
			}
			m_nStep = 1000;
			break;
		}
		else
		{
			BOOL bCompleted = pMotor->IsOrgCompleted(&nErrOrg);
			if (bCompleted == TRUE)
			{
				pMotor->EnableSWLimit(DEF_ENABLE);
				pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] [ORIGIN] [END] "), m_szCmdName, m_nStep);
				ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
				break;
			}
			else
			{
				if (nErrOrg == ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					if (m_tcTimeout.CheckOverTime())
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(ERR_MOTOR_CMN_TIMEOUT, AlarmData);
						break;
					}
				}
				else
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("%s Axis", pMotor->GetAxisName());
					ReportAlarm(nErrOrg, AlarmData);
					break;
				}				
			}
			
			if (m_nAxisIdx >= eAXIS_LOAD_TABLE_1_ROTATE && m_nAxisIdx <= eAXIS_LOAD_TABLE_4_ROTATE)	// rotator 축은 DI 확인 한다.
			{
				int nLdStopSenIONo[MAX_LOAD_TABLE] = { DI_LOADER_TABLE_1_ROTATOR_1_SEN,
													   DI_LOADER_TABLE_2_ROTATOR_1_SEN,
													   DI_LOADER_TABLE_3_ROTATOR_2_SEN,
													   DI_LOADER_TABLE_4_ROTATOR_2_SEN };
				
				
				// Loader table 1,2 축은 2번 IO가 감지되면 역방향으로 회전,
				//              3,4 축은 1번 IO가 감지되면 역방향으로 회전 한다. 

				if (g_IO.in(nLdStopSenIONo[m_nAxisIdx - eAXIS_LOAD_TABLE_1_ROTATE]) == DEF_ON)
				{
					// servo 정지 시킨다. 
					pMotor->SetSlowStop();
					m_nStep = 1002;
					m_tcTimeout.ResetStartTime();
				}
			}
		}
	}break;

	case 1002:
	{
		int nErr = pMotor->MotionDone();
		if (nErr == ERR_SUCCESS)
		{
			m_nStep = 1003; // 모터 정지 후 역방향으로 180도 이동 시킨다.
			pMotor->SetCommandPos(0.0);
			int nErr = pMotor->Move(LOADER_TABLE_REVERSE_SPIN_DEGREE, 20.0, 100, 100, FALSE, _eMotorIntertlock::eMotorInterlock_All_Ignore);

			if (nErr != ERR_NO_ERROR)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("%s Axis", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.ResetStartTime();
			break;
		}
		else if (nErr != ERR_MOTOR_CMN_IN_MOVE_STATE)
		{
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(nErr, AlarmData);
			break;
		}
		if (m_tcTimeout.CheckOverTime())
		{
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(ERR_MOTOR_CMN_TIMEOUT, AlarmData);
			break;
		}

	}break;

	case 1003:
	{
		int nLdStopSenIONo[MAX_LOAD_TABLE] = { DI_LOADER_TABLE_1_ROTATOR_2_SEN,
											   DI_LOADER_TABLE_2_ROTATOR_2_SEN,
											   DI_LOADER_TABLE_3_ROTATOR_1_SEN,
											   DI_LOADER_TABLE_4_ROTATOR_1_SEN };
		
		// 2번째 IO센서 신호 또는 home sensor가 감지 되기를 기다린다.

		int nErr = pMotor->MotionDone();
		if (g_IO.in(nLdStopSenIONo[m_nAxisIdx - eAXIS_LOAD_TABLE_1_ROTATE]) == DEF_ON || pMotor->IsHomeSensorOn() == true)
		{
			// motor 정지 한다.
			pMotor->SetSlowStop();
			m_tcTimeout.ResetStartTime();
			m_nStep = 1004;
			break;
		}
		else if (nErr == ERR_SUCCESS)
		{
			m_nStep = 0; // 다시 처음 부터 시작.
			break;
		}
		else if (nErr != ERR_MOTOR_CMN_IN_MOVE_STATE)
		{
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(nErr, AlarmData);
			break;
		}

		if (m_tcTimeout.CheckOverTime())
		{
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(ERR_MOTOR_CMN_TIMEOUT, AlarmData);
		}

	}break;
	
	case 1004:
	{
		int nErr = pMotor->MotionDone();

		if (nErr == ERR_SUCCESS)
		{
			m_nStep = 0; // 다시 처음 부터 시작.
			break;
		}
		else if(nErr != ERR_MOTOR_CMN_IN_MOVE_STATE)
		{
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(nErr, AlarmData);
			break;
		}

		if (m_tcTimeout.CheckOverTime())
		{
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(ERR_MOTOR_CMN_TIMEOUT, AlarmData);
		}
	}break;

	} // switch 
	return CBaseSeqCmd::State_Executing();
}

//==============================================================================
//
//==============================================================================
BOOL CSystemCmd_Axis_Initial::State_Pause()
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
			pMotor->SetStop();
			pMotor->OriginStop();
			pMotor->EnableSWLimit(DEF_ENABLE);
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

//==============================================================================
//
//==============================================================================
BOOL CSystemCmd_Axis_Initial::State_Recovery()
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

//==============================================================================
//
//==============================================================================
BOOL CSystemCmd_Axis_Initial::State_NormalComplete()
{
	//CTask_System* pTask = (CTask_System*)m_pReceive;
	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CSystemCmd_Axis_Initial::State_Alarm()
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
			pMotor->SetStop();
			pMotor->OriginStop();
			pMotor->EnableSWLimit(DEF_ENABLE);
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

//==============================================================================
//
//==============================================================================
BOOL CSystemCmd_Axis_Initial::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CSystemCmd_Axis_Initial::State_Aborting()
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
	}
	return CBaseSeqCmd::State_Aborting();
}

//==============================================================================
//
//==============================================================================
BOOL CSystemCmd_Axis_Initial::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}