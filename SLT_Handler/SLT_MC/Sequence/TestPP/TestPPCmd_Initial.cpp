#include "StdAfx.h"
#include "TestPPCmd_Initial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//==============================================================================
//
//==============================================================================
CTestPPCmd_Initial::CTestPPCmd_Initial(void)
{
	m_nCmdId = eSeqCmd_TestPP_Initial;
	m_nRetryCount = 0;
	m_nOrgRetryCount = 0;
	m_nHandType = 0;
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));
}




//==============================================================================
//
//==============================================================================
CTestPPCmd_Initial::~CTestPPCmd_Initial(void)
{
}

//==============================================================================
//
//==============================================================================
int CTestPPCmd_Initial::CheckParam()
{
	return ERR_NO_ERROR;
}


//==============================================================================
//
//==============================================================================
int  CTestPPCmd_Initial::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTestPPCmd_Initial::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTestPPCmd_Initial::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTestPPCmd_Initial::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTestPPCmd_Initial::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_Initial::State_Setup()
{
	double dTimeout = MOTOR_TIME_OUT_LONG;
	m_tcTimeout.SetCheckTime( dTimeout );
	m_tcTimeout.StartTimer();

	return CBaseSeqCmd::State_Setup();
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_Initial::State_Executing()
{
	// check alarm
	CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		//pTaskTestPP->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

 	int i=0;
	OneAxis* pMotorX1 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisX1);
	OneAxis* pMotorX2 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisX2);
	OneAxis* pMotorY = pTaskTestPP->GetAxis(CTaskTestPP::eAxisY);
	OneAxis* pMotorZ1 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ1);
	OneAxis* pMotorZ2 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ2);
	OneAxis* pMotorX1Pitch = pTaskTestPP->GetAxis(CTaskTestPP::eAxisPitch_HW_X1_SW_Y1);
	OneAxis* pMotorY1Pitch = pTaskTestPP->GetAxis(CTaskTestPP::eAxisPitch_HW_Y1_SW_X1);
	OneAxis* pMotorX2Pitch = pTaskTestPP->GetAxis(CTaskTestPP::eAxisPitch_HW_X2_SW_Y2);
	OneAxis* pMotorY2Pitch = pTaskTestPP->GetAxis(CTaskTestPP::eAxisPitch_HW_Y2_SW_X2);

	switch( m_nStep )
	{		
	case 0:			// check interlock
		{
			//pTaskTestPP->SendEvent(CEID_TEST_PP_INIT_START);
			m_nStep = 1000;
			m_nRetryCount = 0;
			pTaskTestPP->m_bInitialized = FALSE;
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d]", m_szCmdName, m_nStep);
		}break;
	// Z1,Z2 Origin
	case 1000:
	{
		pTaskTestPP->MakeLog("[%s] [EXEC=%8d][Z1,Z2 Axis Motor Reset](STR)", m_szCmdName, m_nStep);
		OneAxis* pMotor[2] = { pMotorZ1, pMotorZ2 };
		for (i = 0; i < 2; i++) {
			pMotor[i]->ClearOrgFlag();
			pMotor[i]->SetStop();
			Sleep(100);
			pMotor[i]->AlarmReset();
			Sleep(100);
			pMotor[i]->ServoOn();
			Sleep(100);
			pMotor[i]->EnableSWLimit(DEF_DISABLE);
		}
		pTaskTestPP->MakeLog("[%s] [EXEC=%8d][Z1,Z2 Axis Motor Reset](END)", m_szCmdName, m_nStep);
		m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_LONG);
		m_tcTimeout.StartTimer();
		m_nStep++;
		pTaskTestPP->MakeLog("[%s] [EXEC=%8d][Z1,Z2 Axis Motor Origin](STR)", m_szCmdName, m_nStep);
	}break;
	case 1001:
	{
		int nMaxRetry = MAX_MOTOR_RETRY_COUNT;

		OneAxis* pMotor[2] = { pMotorZ1, pMotorZ2 };
		int nErrOrg = 0;
		int nComp = 0;
		for (i = 0; i < 2; i++) {
			nErrOrg = pMotor[i]->Origin();
			if (nErrOrg != ERR_NO_ERROR) {
				m_nOrgRetryCount++;
				if (m_nOrgRetryCount >= 2) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("TestPP %d Z1,Z2 Axis origin retry over [retry : 2]", pTaskTestPP->m_nTestPPIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_MOTOR_ORG_FAIL + pTaskTestPP->m_nTestPPIdx, AlarmData);
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
						if (pMotor[i]->IsHomeSensorOn() == DEF_ON) {
							nComp++;
						}
					}
				}
			}
		}// for end

		if (nComp == 2) {
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d]Z1,Z2 Axis Motor Origin](END)", m_szCmdName, m_nStep);
			m_nStep++;
			//ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
		}
	}break;
	case 1002:
	{
		std::vector<CPoint> vPkr;
		pTaskTestPP->MakeAllPicker(vPkr);
		pTaskTestPP->MakeLog("[%s] [EXEC=%8d][Z1,Z2 Axis Move Up](STR)", m_szCmdName, m_nStep);
		OneAxis* pMotorZ[2] = { pMotorZ1 ,pMotorZ2 };
		int nAxisNo[2] = { CTaskTestPP::eAxisZ1 ,CTaskTestPP::eAxisZ2 };
		for (int i = 0; i < _countof(pMotorZ); i++)
		{
			int nErr = pTaskTestPP->do_MovePos(LOC_IDX_TEST_PP_HOME, i, CPoint(0, 0), vPkr, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i]);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d all picker up command, but this was wrong ", i + 1);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ[i]->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
		}
		m_tcTimeout.SetCheckTime(TIME_OUT_SHORT);
		m_tcTimeout.StartTimer();
		m_nStep++;
	}break;
	case 1003:
	{
		OneAxis* pMotorZ1 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ1);
		OneAxis* pMotorZ2 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ2);

		OneAxis* pMotor[2] = { pMotorZ1 ,pMotorZ2 };
		int nComp = 0;
		for (int i = 0; i < _countof(pMotor); i++)
		{
			std::vector<CPoint> vPkr;
			pTaskTestPP->MakeAllPicker(vPkr);
			m_vErrPicker.clear();
			int nErrSen = pTaskTestPP->Chk_Z_Safety_Sen_Check_Type(vPkr,i);
			int nAxisDone = pMotor[i]->AxisDone(vPkr, m_vErrPicker);
			if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR) {

				nComp++;
			}
			else
			{
				std::vector<CPoint> vErrPkr; vErrPkr.clear();
				CString sAlarmData = _T("");
				int nErr = pTaskTestPP->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, pTaskTestPP->GetSafetyFailPicker(i, ePickerSafetySensorCheckType::eSafetySensorCheck), i, vErrPkr, sAlarmData);
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("%s", sAlarmData);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
					ReportAlarm(nErr, AlarmData, vErrPkr);
					break;
				}

			}
		}
		if (nComp == _countof(pMotor))
		{
			m_nStep = 2000;
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z%d Axis Move to Safety Pos](END)", m_szCmdName, m_nStep, m_nHandType + 1);
		}
	}break;
		// x1,x2 axis org
	case 2000:
		{
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d][X1,X2 Axis Motor Reset](STR)", m_szCmdName, m_nStep);
			OneAxis* pMotor[2] = {pMotorX1, pMotorX2};
			for (int i = 0; i < _countof(pMotor); i++)
			{
				pMotor[i]->ClearOrgFlag();
				pMotor[i]->SetStop();
				Sleep(100);
				pMotor[i]->AlarmReset();
				Sleep(100);
				pMotor[i]->ServoOn();
				Sleep(100);
				pMotor[i]->EnableSWLimit(DEF_DISABLE);
			}
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d][X1,X2 Axis Motor Reset](END)",m_szCmdName ,m_nStep);
			m_tcTimeout.SetCheckTime( MOTOR_TIME_OUT_LONG );
			m_tcTimeout.StartTimer();
			m_nStep++;
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d][X1,X2 Axis Motor Origin](STR)",m_szCmdName ,m_nStep);
		}break;

	case 2001:
		{
			int nMaxRetry = MAX_MOTOR_RETRY_COUNT;

			int nErrOrg = 0;
			int nComp = 0;
			OneAxis* pMotor[2] = { pMotorX1, pMotorX2};
			for (int i = 0; i < _countof(pMotor); i++)
			{
				nErrOrg = pMotor[i]->Origin();
				if (nErrOrg != ERR_NO_ERROR) {
					m_nOrgRetryCount++;
					if (m_nOrgRetryCount >= 2) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("TestPP %d X1,X2 Orgin Retry over", pTaskTestPP->m_nTestPPIdx + 1);
						if (nErrOrg == ERR_MOT_INTERLOCK_IS_NOT_ORG) {
							AlarmData.m_fnAddItemFormat("Module #%d Press is not Orgin", pTaskTestPP->m_nTestPPIdx + 1);
						}
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
						ReportAlarm(/*ERR_TEST_PP_MOTOR_ORG_FAIL*/nErrOrg, AlarmData);
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
						if (nErr == ERR_NO_ERROR)
						{
							if (pMotor[i]->IsHomeSensorOn() == DEF_ON)
							{
								m_nRetryCount = 0;
								m_nOrgRetryCount = 0;
								nComp++;
							}
							else {
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat("TestPP %d TestPP X1,X2 Origin finish!!! But HomeSensor is not detected!!", pTaskTestPP->m_nTestPPIdx + 1);
								AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
								ReportAlarm(ERR_TEST_PP_1_MOTOR_ORG_FAIL + pTaskTestPP->m_nTestPPIdx, AlarmData);
								break;
							}
						}
					}					
					else {
						if (nErrOrg == ERR_MOTOR_CMN_IN_MOVE_STATE)
						{
							if (m_tcTimeout.CheckOverTime() == TRUE)
							{			
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat("TestPP %d X1,X2 axis origin time over", pTaskTestPP->m_nTestPPIdx + 1);
								AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
								CBaseSeqCmd::ReportAlarm(ERR_TEST_PP_1_MOTOR_ORG_TIMEOUT + pTaskTestPP->m_nTestPPIdx, AlarmData);
								break;
							}
						}
						else {
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("TestPP %d X1,X2 Motor usable err", pTaskTestPP->m_nTestPPIdx + 1);
							AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
							ReportAlarm(/*ERR_TEST_PP_MOTOR_ORG_FAIL*/nErrOrg, AlarmData);
							break;
						}				
					}
				}
				
			}

			if (nComp == _countof(pMotor))
			{
				m_nStep = 2500;
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d][X1,X2 Axis Motor Origin](END)", m_szCmdName, m_nStep);
				break;
			}
		}break;	
			// x1,x2, Y1,Y2 Pitch axis org
	case 2500:
		{
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d][X1,X2 Pitch Y1,Y2 Pitch Axis Motor Reset](STR)", m_szCmdName, m_nStep);
			OneAxis* pMotor[4] = { pMotorX1Pitch, pMotorX2Pitch, pMotorY1Pitch, pMotorY2Pitch };
			for (int i = 0; i < _countof(pMotor); i++)
			{
				pMotor[i]->ClearOrgFlag();
				pMotor[i]->SetStop();
				Sleep(100);
				pMotor[i]->AlarmReset();
				Sleep(100);
				pMotor[i]->ServoOn();
				Sleep(100);
				pMotor[i]->EnableSWLimit(DEF_DISABLE);
			}
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d][X1,X2 Pitch Y1,Y2 Pitch Motor Reset](END)", m_szCmdName, m_nStep);
			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_LONG);
			m_tcTimeout.StartTimer();
			m_nStep++;
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d][X1,X2 Pitch Y1,Y2 Pitch Axis Motor Origin](STR)", m_szCmdName, m_nStep);
		}break;

	case 2501:
	{
		int nMaxRetry = MAX_MOTOR_RETRY_COUNT;

		int nErrOrg = 0;
		int nComp = 0;
		OneAxis* pMotor[4] = { pMotorX1Pitch, pMotorX2Pitch, pMotorY1Pitch, pMotorY2Pitch };
		for (int i = 0; i < _countof(pMotor); i++)
		{
			nErrOrg = pMotor[i]->Origin();
			if (nErrOrg != ERR_NO_ERROR) {
				m_nOrgRetryCount++;
				if (m_nOrgRetryCount >= 2) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("TestPP %d X1,X2 Pitch Y1,Y2 Pitch Orgin Retry over", pTaskTestPP->m_nTestPPIdx + 1);
					if (nErrOrg == ERR_MOT_INTERLOCK_IS_NOT_ORG) {
						AlarmData.m_fnAddItemFormat("Module #%d Press is not Orgin", pTaskTestPP->m_nTestPPIdx + 1);
					}
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
					ReportAlarm(/*ERR_TEST_PP_MOTOR_ORG_FAIL*/nErrOrg, AlarmData);
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
						if (pMotor[i]->IsHomeSensorOn() == DEF_ON)
						{
							m_nRetryCount = 0;
							m_nOrgRetryCount = 0;
							nComp++;
						}
						else
						{
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("TestPP %d TestPP X1,X2 Pitch Y1,Y2 Pitch Origin finish!!! But HomeSensor is not detected!!", pTaskTestPP->m_nTestPPIdx + 1);
							AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
							ReportAlarm(ERR_TEST_PP_1_MOTOR_ORG_FAIL + pTaskTestPP->m_nTestPPIdx, AlarmData);
							break;
						}
					}
				}
				else {
					if (nErrOrg == ERR_MOTOR_CMN_IN_MOVE_STATE)
					{
						if (m_tcTimeout.CheckOverTime() == TRUE)
						{
							if (m_nRetryCount > nMaxRetry)
							{
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat("TestPP %d X1,X2 Pitch Y1,Y2 Pitch axis origin time over", pTaskTestPP->m_nTestPPIdx + 1);
								AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
								CBaseSeqCmd::ReportAlarm(ERR_TEST_PP_1_MOTOR_ORG_TIMEOUT + pTaskTestPP->m_nTestPPIdx, AlarmData);
								break;
							}						
						}
					}
					else
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("TestPP %d X1,X2 Pitch Y1,Y2 Pitch Motor usable err", pTaskTestPP->m_nTestPPIdx + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
						ReportAlarm(/*ERR_TEST_PP_MOTOR_ORG_FAIL*/nErrOrg, AlarmData);
						break;
					}
				}
			}
		}

		if (nComp == _countof(pMotor))
		{
			m_nStep = 3000;
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d][X1,X2 Pitch Y1,Y2 Pitch Axis Motor Origin](END)", m_szCmdName, m_nStep);
			break;
		}
	}break;
		// Y axis org
	case 3000:
		{	
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d][Y Axis Motor Reset](STR)",m_szCmdName ,m_nStep);
			OneAxis* pMotor[1] = { pMotorY};
			for(i=0; i<_countof(pMotor); i++){
				pMotor[i]->ClearOrgFlag();
				pMotor[i]->SetStop();
				Sleep(100);
				pMotor[i]->AlarmReset();	
				Sleep(100);
				pMotor[i]->ServoOn();
				Sleep(100);
				pMotor[i]->EnableSWLimit(DEF_DISABLE);
			}
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d][Y Axis Motor Reset](END)",m_szCmdName ,m_nStep);
			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_LONG);
			m_tcTimeout.StartTimer();
			m_nStep++;
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d][Y Axis Motor Origin](STR)",m_szCmdName ,m_nStep);
		}break;
	case 3001:
		{
			int nMaxRetry = MAX_MOTOR_RETRY_COUNT;

			OneAxis* pMotor[1] = { pMotorY };
			int nErrOrg = 0;
			int nComp   = 0;
			for(i=0; i<_countof(pMotor); i++){
				nErrOrg = pMotor[i]->Origin();
				if( nErrOrg != ERR_NO_ERROR ){
					m_nOrgRetryCount++;
					if( m_nOrgRetryCount >= 2 ){
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("TestPP %d Y axis origin retry over [retry : 2]", pTaskTestPP->m_nTestPPIdx+1);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
						ReportAlarm(ERR_TEST_PP_1_MOTOR_ORG_FAIL + pTaskTestPP->m_nTestPPIdx, AlarmData);
						break;
					}
					m_nStep--;
				}else 
				{
					BOOL bCompleted = pMotor[i]->IsOrgCompleted(&nErrOrg);
					if (bCompleted)
					{
						pMotor[i]->EnableSWLimit(DEF_ENABLE);
						int nErr = pMotor[i]->ChkMotorUsable();
						if (nErr == ERR_NO_ERROR) {
							if (pMotor[i]->IsHomeSensorOn() == DEF_ON) {
								nComp++;
							}
						}
					}
					else{
						if (nErrOrg == ERR_MOTOR_CMN_IN_MOVE_STATE)
						{
							if (m_tcTimeout.CheckOverTime() == TRUE) 
							{								
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat("TestPP %d Y origin time over", pTaskTestPP->m_nTestPPIdx + 1);
								AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
								CBaseSeqCmd::ReportAlarm(ERR_TEST_PP_1_MOTOR_ORG_TIMEOUT + pTaskTestPP->m_nTestPPIdx, AlarmData);
								break;
							}						
							
						}
						else {
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("TestPP %d Y move is not usable", pTaskTestPP->m_nTestPPIdx + 1);
							AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
							ReportAlarm(ERR_TEST_PP_1_MOTOR_ORG_FAIL + pTaskTestPP->m_nTestPPIdx, AlarmData);
							break;
						}						
					}
				}
			}// for end

			if( nComp == _countof(pMotor)){
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d][Y Axis Motor Origin](END)",m_szCmdName ,m_nStep);
				m_nStep = 6000;
				//ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
			}
		}break;
	case 6000:
		{
			ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
		}break;
	}
	return CBaseSeqCmd::State_Executing();
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_Initial::State_Pause()
{
	CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskTestPP->MakeLog("[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			int nAxisCount = pTaskTestPP->m_vMotor.size();
			for(int i=0; i<nAxisCount; i++){
				OneAxis* pMotor = pTaskTestPP->GetAxis(i);
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

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_Initial::State_Recovery()
{
	CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskTestPP->MakeLog("[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
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
BOOL CTestPPCmd_Initial::State_NormalComplete()
{
	CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
	pTaskTestPP->m_bInitialized = TRUE;
	//pTaskTestPP->SendEvent(CEID_TEST_PP_INIT_END);

	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_Initial::State_Alarm()
{
	CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskTestPP->MakeLog("[CMD: %s] [ALARM] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0: // alarm 발생 했을 때 처리해 줘야 할꺼 처리 해줌
		{
			int nAxisCount = pTaskTestPP->m_vMotor.size();
			for(int i=0; i<nAxisCount; i++){
				OneAxis* pMotor = pTaskTestPP->GetAxis(i);
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

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_Initial::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete(); 
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_Initial::State_Aborting()
{
	CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskTestPP->MakeLog("[CMD: %s] [ABORT] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			int nAxisCount = pTaskTestPP->m_vMotor.size();
			for(int i=0; i<nAxisCount; i++){
				OneAxis* pMotor = pTaskTestPP->GetAxis(i);
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

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_Initial::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete(); 
}
