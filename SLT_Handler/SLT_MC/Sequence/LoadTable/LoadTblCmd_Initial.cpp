#include "StdAfx.h"
#include "LoadTblCmd_Initial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//==============================================================================
//
//==============================================================================
CLoadTblCmd_Initial::CLoadTblCmd_Initial(void)
{
	m_nCmdId = eSeqCmd_LoadTbl_Initial;
	m_nOrgRetryCount = 0;
	m_nTriggerInitRetryCnt = 0;
	m_nVibCurTryCnt = 0;
	m_nBackVibratorStep = 0;
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));
}



//==============================================================================
//
//==============================================================================
CLoadTblCmd_Initial::~CLoadTblCmd_Initial(void)
{
}


//==============================================================================
//
//==============================================================================
int CLoadTblCmd_Initial::CheckParam()
{
	m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
	m_tcTimeout.StartTimer();

	return ERR_NO_ERROR;
}


//==============================================================================
//
//==============================================================================
int  CLoadTblCmd_Initial::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CLoadTblCmd_Initial::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CLoadTblCmd_Initial::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CLoadTblCmd_Initial::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CLoadTblCmd_Initial::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CLoadTblCmd_Initial::State_Setup()
{	
	return CBaseSeqCmd::State_Setup();
}

//==============================================================================
//
//==============================================================================
BOOL CLoadTblCmd_Initial::State_Executing()
{
	// check alarm
 	CTaskLoadTable* pTaskLoadTbl = (CTaskLoadTable*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		//pTaskLoadTbl->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{		
		case 0:	// check interlock
		{
			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d]", m_szCmdName, m_nStep);

			m_tcTimeout.SetCheckTime(TIME_OUT_SHORT);
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
		case 1: //checking Load Table Vibrator OFF
		{
			if (m_tcTimeout.CheckOverTime() == TRUE)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), pTaskLoadTbl->m_nTableIdx + 1);
				ReportAlarm(ERR_LOAD_TBL_1_VIBRATOR_CYLINDER_BWD_TIMEOUT + pTaskLoadTbl->m_nTableIdx, AlarmData);
				break;
			}

			if (g_DMCont.m_dmEQP.GN(NDM0_TABLE_VIBRATOR_OPTION) == DEF_ON)
			{
				int nErr = pTaskLoadTbl->chk_cyl_vibrator_onoff(DEF_OFF);
				if (nErr != ERR_NO_ERROR)
				{
					pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] Interlock Vibrator On -> Off", m_szCmdName, m_nStep);
					m_nBackVibratorStep = m_nStep;
					m_nStep = do_Vibrator_Off;
					break;
				}
			}

			m_nStep++;
		}break;
	case 2:			// check interlock
		{
			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d]",m_szCmdName ,m_nStep);
			pTaskLoadTbl->m_bInitialized = FALSE;
			pTaskLoadTbl->SendEvent(CEID_LOAD_TBL_INIT_START);
			if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
				pTaskLoadTbl->ChkDI(CTaskLoadTable::xLOAD_TABLE_ROTATE1_SEN, DEF_OFF);
				pTaskLoadTbl->ChkDI(CTaskLoadTable::xLOAD_TABLE_ROTATE2_SEN, DEF_OFF);
			}
			m_nOrgRetryCount = 0;
			//이전
			//m_nStep = 1000;

			//장비 No
//			int nEQPNo = g_DMCont.m_dmEQP.GN(NDM0_Handler_EQP_No);
//			if (nEQPNo > DEF_EQUIPMENT_UNIT1)
//			{
// 				int nTblType = pTaskLoadTbl->GetTableUsage();
// 
// 				if (nTblType == eLoadType)
// 				{
// 					m_nStep = 1000;
// 					break;
// 				}
// 				else
// 				{
// 					//Stacker 충돌 영역 확인 & 회피.
// 					if (pTaskLoadTbl->IsTableStackerImpactArea() != TRUE) {
// 						m_nStep = 1000;
// 					}
// 					else {
// 						m_nStep = 500;
// 					}
// 				}

				//Stacker 충돌 영역 확인 & 회피.
				if (pTaskLoadTbl->IsTableStackerImpactArea() != TRUE) {
					m_nStep = 1000;
				}
				else {
					m_nStep = 500;
				}
//			}
// 			else
// 			{
// 				m_nStep = 1000;
// 			}
		}break;
	// Stacker 충돌 영역 회피.
	case 500:
	{
		pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Motor Y Reset](STR)", m_szCmdName, m_nStep);
		OneAxis* pMotor = (OneAxis*)pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
		pMotor->ClearOrgFlag();
		pMotor->SetStop();
		Sleep(100);
		pMotor->AlarmReset();
		Sleep(100);
		pMotor->ServoOn();
		Sleep(100);
		pMotor->EnableSWLimit(DEF_DISABLE);

		m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
		m_tcTimeout.ResetStartTime();
		pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Motor Y Reset](END)", m_szCmdName, m_nStep);
		m_nStep++;
		pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Motor Y Move Stacker Impact Area](STR)", m_szCmdName, m_nStep);
	}break;
	case 501:
		{
			OneAxis* pMotor = (OneAxis*)pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
			pMotor->SetCommandPos(0.0);
			double dMtSpd = pMotor->GetSpeed();
			double dMtSpd5 = dMtSpd * 0.05; // 5%
			
			int nRetMove = pMotor->Move(LOADER_TABLE_STACKER_IMPACT_MOVE, dMtSpd5, 100, 100, FALSE, _eMotorIntertlock::eMotorInterlock_All_Ignore);

			if (nRetMove != ERR_NO_ERROR)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nRetMove, AlarmData);
				break;
			}

			m_tcTimeout.ResetStartTime();
			m_nStep = 502;
		}break;
	case 502:
		{
			OneAxis* pMotor = (OneAxis*)pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
			int nErr = pMotor->MotionDone();

			if (nErr == ERR_SUCCESS)
			{
				pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Motor Y Move Stacker Impact Area](END)", m_szCmdName, m_nStep);
				m_nStep = 1000; 
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
	case 1000: // Rotate org
		{
			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Motor Rotate Reset](STR)",m_szCmdName ,m_nStep);
			OneAxis* pMotor = (OneAxis*)pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisRotate);
			pMotor->ClearOrgFlag();
			pMotor->SetStop();
			Sleep(100);
			pMotor->AlarmReset();	
			Sleep(100);
			pMotor->ServoOn();
			Sleep(100);
			pMotor->EnableSWLimit(DEF_DISABLE);

			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
			m_tcTimeout.ResetStartTime();

			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Motor Rotate Reset](END)",m_szCmdName ,m_nStep);
			m_nStep++;
			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Motor Rotate Origin](STR)",m_szCmdName ,m_nStep);
			
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
	
			// 센서 감지 상태 확인 한다.
			int nSensorSts = 0;
			int nIOIdx = CTaskLoadTable::xLOAD_TABLE_ROTATE1_SEN;

			if (pTaskLoadTbl->m_nTableIdx == eLoadTbl_3 || pTaskLoadTbl->m_nTableIdx == eLoadTbl_4)
			{
				nIOIdx = CTaskLoadTable::xLOAD_TABLE_ROTATE2_SEN;
			}
			nSensorSts = pTaskLoadTbl->GetDI(nIOIdx); // 해당 IO가 감지되면 즉시 멈춘다.

			if (nSensorSts == DEF_ON)
			{
				m_nStep = 1002;
				pMotor->SetSlowStop();
				m_tcTimeout.ResetStartTime();
				break;
			}
			
		}break;
	case 1001:
		{				
			OneAxis* pMotor = (OneAxis*)pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisRotate);
			int nErrOrg = 0;
			int nComp = 0;
			nErrOrg = pMotor->Origin();
			if( nErrOrg != NO_ERROR ){
				m_nOrgRetryCount++;
				if( m_nOrgRetryCount >= 2 ){
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
				if (bCompleted)
				{
					pMotor->EnableSWLimit(DEF_ENABLE);
					int nErr = pMotor->ChkMotorUsable();
					if (nErr == ERR_NO_ERROR) {
						m_nStep = 1005;

						if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
							pTaskLoadTbl->ChkDI(CTaskLoadTable::xLOAD_TABLE_ROTATE1_SEN, DEF_OFF);
							pTaskLoadTbl->ChkDI(CTaskLoadTable::xLOAD_TABLE_ROTATE2_SEN, DEF_OFF);
							Sleep(SIMUL_SLEEP_TIME);
						}

						break;
					}
				}
				else
				{
					if (nErrOrg == ERR_MOTOR_CMN_IN_MOVE_STATE)
					{
						if (m_tcTimeout.CheckOverTime() == TRUE)
						{
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("Press Unit - %d", pTaskLoadTbl->m_nTableIdx + 1);
							AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
							ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + pTaskLoadTbl->m_nTableIdx, AlarmData);
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

				// 센서 감지 상태 확인 한다.
				int nSensorSts = 0;
				int nIOIdx = CTaskLoadTable::xLOAD_TABLE_ROTATE1_SEN;

				if (pTaskLoadTbl->m_nTableIdx == eLoadTbl_3 || pTaskLoadTbl->m_nTableIdx == eLoadTbl_4)
				{
					nIOIdx = CTaskLoadTable::xLOAD_TABLE_ROTATE2_SEN;
				}
				nSensorSts = pTaskLoadTbl->GetDI(nIOIdx); // 해당 IO가 감지되면 즉시 멈춘다.
				
				if (nSensorSts == DEF_ON)
				{
					m_nStep = 1002;
					pMotor->SetSlowStop();
					m_tcTimeout.ResetStartTime();
				}
			}
		}break;

	case 1002:
	{
		OneAxis* pMotor = (OneAxis*)pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisRotate);
		int nErr = pMotor->MotionDone();

		if (nErr == ERR_SUCCESS)
		{
			m_nStep = 1003; // 모터 정지 후 역방향으로 180도 이동 시킨다.
			pMotor->SetCommandPos(0.0);
			int nRetMove = pMotor->Move(LOADER_TABLE_REVERSE_SPIN_DEGREE, 20.0, 100, 100, FALSE,_eMotorIntertlock::eMotorInterlock_All_Ignore);

			if (nRetMove != ERR_NO_ERROR)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nRetMove, AlarmData);
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
		OneAxis* pMotor = (OneAxis*)pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisRotate);
		int nSensorSts = 0;
		int nIOIdx = CTaskLoadTable::xLOAD_TABLE_ROTATE2_SEN;

		if (pTaskLoadTbl->m_nTableIdx == eLoadTbl_3 || pTaskLoadTbl->m_nTableIdx == eLoadTbl_4)
		{
			nIOIdx = CTaskLoadTable::xLOAD_TABLE_ROTATE1_SEN;
		}

		nSensorSts = pTaskLoadTbl->GetDI(nIOIdx);
		bool bGetHomeSensorSts = pMotor->IsHomeSensorOn();
		int nErr = pMotor->MotionDone();
		if (nSensorSts == DEF_ON || bGetHomeSensorSts == true) // 해당 IO 또는 원점 센서가 감지되면 즉시 멈춘다.
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
		OneAxis* pMotor = (OneAxis*)pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisRotate);
		int nErr = pMotor->MotionDone();
		if (nErr == ERR_SUCCESS)
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

	case 1005: // roator 홈센서 도그 형항 변경으로 추가 시퀀스 (roator를 Station work position 이동)
	{
		//Loading Table이  Test Site에 존재할 경우, Rotate Test 방향 Move 

		double dTPos = pTaskLoadTbl->GetTeachPos(LOC_IDX_LD_TBL_TEST_SIDE_1, CTaskLoadTable::eAxisRotate);
		OneAxis* pMotor = (OneAxis*)pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisRotate);
		double dMtSpd = pMotor->GetSpeed();
		dMtSpd = dMtSpd * 0.1;
		int nErr = pMotor->Move(dTPos, dMtSpd, 100, 100, FALSE, _eMotorIntertlock::eMotorInterlock_All_Ignore);

		//int nErr = pTaskLoadTbl->do_MovePos(LOC_IDX_LD_TBL_TEST_SIDE_1, CTaskLoadTable::eAxisRotate);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Load Table %d, move rotate %s command error", pTaskLoadTbl->m_nTableIdx + 1, _T("LoadTblCmd_Initial"));
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(nErr, AlarmData);
			break;
		}

		m_tcTimeout.SetCheckTime((MOTOR_TIME_OUT_SHORT));
		m_tcTimeout.StartTimer();
		m_nStep++;

	}break;
	case 1006:
	{
		OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisRotate);
		int nAxisDone = pMotor->AxisDone();
		if (nAxisDone == ERR_NO_ERROR) {
			//Rotate Sensor 확인.
			if (pTaskLoadTbl->chk_rotate_direction(CTaskLoadTable::eDir_TestSite) != ERR_NO_ERROR && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Load Table %d, rotate direction state error, state : %s ", pTaskLoadTbl->m_nTableIdx + 1, _T("LoadTblCmd_Initial"));
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(ERR_LOAD_TBL_ROTATE_STATE_TEST_SITE_FAIL, AlarmData);
				break;
			}
			m_nStep = 2000;
		}
		else
		{
			if (m_tcTimeout.CheckOverTime() == TRUE)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), pTaskLoadTbl->m_nTableIdx + 1);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + pTaskLoadTbl->m_nTableIdx, AlarmData);
				break;
			}

			if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("%s Axis", pMotor->GetAxisName());
				ReportAlarm(nAxisDone, AlarmData);
			}
		}
	}break;
	case 2000: // y org
	{
		pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Motor Y Reset](STR)", m_szCmdName, m_nStep);
		OneAxis* pMotor = (OneAxis*)pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
		pMotor->ClearOrgFlag();
		pMotor->SetStop();
		Sleep(100);
		pMotor->AlarmReset();
		Sleep(100);
		pMotor->ServoOn();
		Sleep(100);
		pMotor->EnableSWLimit(DEF_DISABLE);

		m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
		m_tcTimeout.ResetStartTime();
		pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Motor Y Reset](END)", m_szCmdName, m_nStep);
		m_nStep++;
		pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Motor Y Origin](STR)", m_szCmdName, m_nStep);
	}break;
	case 2001:
	{
		int nErrOrg = 0;
		int nComp = 0;
		OneAxis* pMotor = (OneAxis*)pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
		nErrOrg = pMotor->Origin();
		if (nErrOrg != NO_ERROR) {
			m_nOrgRetryCount++;
			if (m_nOrgRetryCount >= 2) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("%s Axis", pMotor->GetAxisName());
				ReportAlarm(nErrOrg, AlarmData);
				break;
			}
			m_nStep = 2000;
			break;
		}
		else
		{
			BOOL bCompleted = pMotor->IsOrgCompleted(&nErrOrg);
			if (bCompleted)
			{
				pMotor->EnableSWLimit(DEF_ENABLE);
				int nErr = pMotor->ChkMotorUsable();
				if (nErr == ERR_NO_ERROR) {
					pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Motor Y Origin](END)", m_szCmdName, m_nStep);
					m_nStep = 3000;
					break;
				}
			}
			else {
				if (nErrOrg == ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					if (m_tcTimeout.CheckOverTime() == TRUE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Press Unit - %d", pTaskLoadTbl->m_nTableIdx + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + pTaskLoadTbl->m_nTableIdx, AlarmData);
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
		}
	}break;
	case 3000:
	{
		int nAxisCount = CTaskLoadTable::eMaxAxisCount;
		for (int i = 0; i < nAxisCount; i++) {
			OneAxis* pMotor = (OneAxis*)pTaskLoadTbl->GetAxis(i);
			if (pMotor == NULL) continue;
			pMotor->EnableSWLimit(DEF_ENABLE);
		}

		m_nStep = 3001;
	}break;
	case 3001:
	{
			if (pTaskLoadTbl->GetTableUsage() == eLoadType && g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use) == TRUE) {
				if (pTaskLoadTbl->m_nTableIdx == eLoadTbl_2) {
					g_TaskLoadTbl[eLoadTbl_2].InitialTrigger();
					pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [InitialTrigger] Table = %d(STR)", m_szCmdName, m_nStep, pTaskLoadTbl->m_nTableIdx + 1);
				}
				else {
					g_TaskLoadTbl[eLoadTbl_3].InitialTrigger();
					pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [InitialTrigger] Table = %d(STR)", m_szCmdName, m_nStep, pTaskLoadTbl->m_nTableIdx + 1);
				}

				if (g_IO.GetDriverType() != _DIO_TYPE_SIMUL_) {
					Sleep(200);
					int nTriggerIdx = pTaskLoadTbl->m_nTableIdx / 2;
					int nTriggerCurPos = g_trigger_generator.GetTriggerCurPos(nTriggerIdx);
					int nTester0 = g_trigger_generator.GetTriggerCurPos(0);
					int nTester1 = g_trigger_generator.GetTriggerCurPos(1);
					if (nTriggerCurPos > 20 || nTriggerCurPos < -20) //10 pulse 벗어 나면 초기화 안된 것.
					{
						m_nTriggerInitRetryCnt++;
						if (m_nTriggerInitRetryCnt >= 5) {
							pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Trigger Initial Fail][CNT : %d]", m_szCmdName, m_nStep, m_nTriggerInitRetryCnt);
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("Table %d Trigger Initial Fail. ", pTaskLoadTbl->m_nTableIdx + 1);
							ReportAlarm(ERR_TRIGGER_INITIAL_FAIL, AlarmData);
							break;
						}
						else {
							break;
						}
					}
					m_nTriggerInitRetryCnt = 0;
				}
			}

			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [InitialTrigger] Table = %d(End)", m_szCmdName, m_nStep, pTaskLoadTbl->m_nTableIdx + 1);
			ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
		}break;
		case do_Vibrator_On:
		{
			pTaskLoadTbl->do_SideVibratorOnOff(DEF_ON);

			tmr.StartTmChk();

			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [do_Vibrator_On]", m_szCmdName, m_nStep);
			m_nStep = do_Vibrator_On_Wait;

			break;
		}
		case do_Vibrator_On_Wait:
		{
			// Vibrator Delay Time 추가 [0보다 작거나 '-' 값이 들어가면 최소값 0.1로 만듬] [8/4/2020 donghyun.shin]
			double dVibDelayTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_LOADING_TABLE_VIBRATOR_DELAY_TIME);

			if (dVibDelayTime <= 0)
				dVibDelayTime = 0.05;

			tmr.EndTmChk();
			if (tmr.GetTmDistanceMS() < dVibDelayTime*1000.0)
				break;

			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [do_Vibrator_On_Wait]", m_szCmdName, m_nStep);
			m_nStep = do_Vibrator_Off;

			break;
		}
		case do_Vibrator_Off:
		{
			pTaskLoadTbl->do_SideVibratorOnOff(DEF_OFF);

			tmr.StartTmChk();
			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [do_Vibrator_Off]", m_szCmdName, m_nStep);
			m_nStep = do_Vibrator_Off_Wait;

			break;
		}
		case do_Vibrator_Off_Wait:
		{
			// Vibrator Delay Time 추가 [0보다 작거나 '-' 값이 들어가면 최소값 0.1로 만듬] [8/4/2020 donghyun.shin]
			double dVibDelayTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_LOADING_TABLE_VIBRATOR_DELAY_TIME);

			if (dVibDelayTime <= 0)
				dVibDelayTime = 0.05;

			tmr.EndTmChk();
			if (tmr.GetTmDistanceMS() < dVibDelayTime*1000.0)
				break;

			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [do_Vibrator_Off_Wait]", m_szCmdName, m_nStep);
			m_nStep = Vibrator_complete;
			break;
		}
		case Vibrator_complete:
		{
			m_nVibCurTryCnt++;
			m_nStep = m_nBackVibratorStep;
			break;
		}
	}
	return CBaseSeqCmd::State_Executing();
}

//==============================================================================
//
//==============================================================================
BOOL CLoadTblCmd_Initial::State_Pause()
{
	CTaskLoadTable* pTaskLoadTbl = (CTaskLoadTable*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskLoadTbl->MakeLog("[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			int nAxisCount = CTaskLoadTable::eMaxAxisCount;
			for(int i=0; i<nAxisCount; i++){
				OneAxis* pMotor = (OneAxis*)pTaskLoadTbl->GetAxis(i);
				if( pMotor != NULL )
				{
					pMotor->SetStop();
					pMotor->OriginStop();
				}
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

//==============================================================================
//
//==============================================================================
BOOL CLoadTblCmd_Initial::State_Recovery() // TODO SLT : 
{
    CTaskLoadTable* pTaskLoadTbl = (CTaskLoadTable*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskLoadTbl->MakeLog("[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
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

//==============================================================================
//
//==============================================================================
BOOL CLoadTblCmd_Initial::State_NormalComplete()
{
	CTaskLoadTable* pTaskLoadTbl = (CTaskLoadTable*)m_pReceive;
	pTaskLoadTbl->m_bInitialized = TRUE;
	pTaskLoadTbl->SendEvent(CEID_LOAD_TBL_INIT_END);
	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CLoadTblCmd_Initial::State_Alarm()
{
	CTaskLoadTable* pTaskLoadTbl = (CTaskLoadTable*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskLoadTbl->MakeLog("[CMD: %s] [ALARM] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0: // alarm 발생 했을 때 처리해 줘야 할꺼 처리 해줌
		{
			int nAxisCount = CTaskLoadTable::eMaxAxisCount;
			for(int i=0; i<nAxisCount; i++){
				OneAxis* pMotor = (OneAxis*)pTaskLoadTbl->GetAxis(i);
				if( pMotor != NULL )
				{
					pMotor->SetStop();
					pMotor->OriginStop();
				}
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

//==============================================================================
//
//==============================================================================
BOOL CLoadTblCmd_Initial::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete(); 
}

//==============================================================================
//
//==============================================================================
BOOL CLoadTblCmd_Initial::State_Aborting()
{
	CTaskLoadTable* pTaskLoadTbl = (CTaskLoadTable*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskLoadTbl->MakeLog("[CMD: %s] [ABORT] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			int nAxisCount = CTaskLoadTable::eMaxAxisCount;
			for(int i=0; i<nAxisCount; i++){
				OneAxis* pMotor = (OneAxis*)pTaskLoadTbl->GetAxis(i);
				if( pMotor != NULL )
				{
					pMotor->SetStop();
					pMotor->OriginStop();
					pMotor->EnableSWLimit(DEF_ENABLE);
				}
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

//==============================================================================
//
//==============================================================================
BOOL CLoadTblCmd_Initial::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete(); 
}
