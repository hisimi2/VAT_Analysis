#include "StdAfx.h"
#include "PressUnitCmd_Initial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPressUnitCmd_Initial::CPressUnitCmd_Initial(void)
{
	m_nCmdId = eSeqCmd_PressUnit_Initial;
	m_nOrgRetryCount = 0;
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));
}


CPressUnitCmd_Initial::~CPressUnitCmd_Initial(void)
{
}

//==============================================================================
//
//==============================================================================
int CPressUnitCmd_Initial::CheckParam()
{
	return ERR_NO_ERROR;
}


//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_Initial::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_Initial::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_Initial::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_Initial::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_Initial::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_Initial::State_Setup()
{
	m_tcTimeout.SetCheckTime( MOTOR_TIME_OUT_MIDDLE );
	m_tcTimeout.StartTimer();

	return CBaseSeqCmd::State_Setup();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_Initial::State_Executing()
{
	// check alarm
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		//pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{		
	case 0:			// check interlock
		{
			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d]",m_szCmdName ,m_nStep);
			pTaskPressUnit->m_bInitialized = FALSE;
			//pTaskPressUnit->SendEvent(CEID_PRESS_UNIT_INIT_START);
			m_nOrgRetryCount = 0;
			m_nStep = 1000;
		}break;

	case 1000: // org
		{
			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Motor Reset](STR)",m_szCmdName ,m_nStep);
			OneAxis* pMotor = (OneAxis*)pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			pMotor->ClearOrgFlag();
			pMotor->SetStop();
			Sleep(100);
			pMotor->AlarmReset();	
			Sleep(100);
			pMotor->ServoOn();
			Sleep(100);
			pMotor->EnableSWLimit(DEF_DISABLE);
			m_tcTimeout.ResetStartTime();
			m_nStep++;
			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Motor Reset](END)",m_szCmdName ,m_nStep);
			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Motor Origin](STR)",m_szCmdName ,m_nStep);
		}break;
	case 1001:
		{
			int nErrOrg = 0;
			int nComp = 0;
			OneAxis* pMotor = (OneAxis*)pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			nErrOrg = pMotor->Origin();
			if (nErrOrg != ERR_NO_ERROR)
			{
				m_nOrgRetryCount++;
				if (m_nOrgRetryCount >= MAX_MOTOR_RETRY_COUNT) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit - %d", pTaskPressUnit->m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_INIT_FAIL + pTaskPressUnit->m_nUnitIdx, AlarmData);
					break;
				}
				m_nStep = 1000;
			}
			else
			{
				BOOL bCompleted = pMotor->IsOrgCompleted(&nErrOrg);
				if (bCompleted)
				{
					m_nOrgRetryCount = 0;
					m_nStep++;
					pMotor->EnableSWLimit(DEF_ENABLE);
					pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Motor Origin](END)", m_szCmdName, m_nStep);
				}
				else
				{
					if (nErrOrg == ERR_MOTOR_CMN_IN_MOVE_STATE)
					{
						if (m_tcTimeout.CheckOverTime() == TRUE)
						{
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("Press Unit - %d", pTaskPressUnit->m_nUnitIdx + 1);
							AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
							ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + pTaskPressUnit->m_nUnitIdx, AlarmData);
							break;
						}
					}
					else
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Press Unit - %d", pTaskPressUnit->m_nUnitIdx + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(nErrOrg, AlarmData);
					}
				}
			}
		}break;
	case 1002:
		{
			OneAxis* pMotor = (OneAxis*)pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			pMotor->EnableSWLimit(DEF_ENABLE);

			if(g_DMCont.m_dmEQP.GB(BDM0_MD_VIBRATION_MODE) == DEF_ON){
				m_nStep++;
			}else{
				m_nStep = 2000;
			}
		}break;
	case 1003:
		{

			m_nStep = 2000;
		}break;
	case 2000:
		{
			// safety 위치보다 작을 수는 없다.
			double dTPosSafety = 0.0;
			pTaskPressUnit->GetTeachPos(LOC_IDX_PRESS_UNIT_SAFETY, &dTPosSafety);
			OneAxis* pMotor = (OneAxis*)pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			if (dTPosSafety <= 0.0) {
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Move to Safety Pos](STR)", m_szCmdName, m_nStep);
				
				pMotor->Move(dTPosSafety, TEACH_MOVE_SPD_LOW, 200, 200);

				m_tcTimeout.SetCheckTime((MOTOR_TIME_OUT_SHORT));
				m_tcTimeout.StartTimer();
				m_nStep++;
				break;
			}
			else {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d move Safety pos cmd Error", pTaskPressUnit->m_nUnitIdx + 1);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + pTaskPressUnit->m_nUnitIdx, AlarmData);
				break;
			}
			
		}break;
	case 2001:
		{
			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			double dCurPos = 0;
			pMotor->GetCurrentPos(dCurPos);

			
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Move to Safety Pos](END)", m_szCmdName, m_nStep);
				m_nStep = 3000;
			}
			else 
			{
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move Safety pos Error", pTaskPressUnit->m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("SetTime=%.2f, z cur pos=%.2f", m_tcTimeout.GetCheckTime(), dCurPos);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + pTaskPressUnit->m_nUnitIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move Safety pos axis done Error [nAxisDone = %d]", pTaskPressUnit->m_nUnitIdx + 1, nAxisDone);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + pTaskPressUnit->m_nUnitIdx, AlarmData);
				}
			}
		}break;
	case 3000:
		{
			ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
		}break;
	}
	return CBaseSeqCmd::State_Executing();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_Initial::State_Pause()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskPressUnit->MakeLog("[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			OneAxis* pMotor = (OneAxis*)pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			pMotor->SetStop();
			pMotor->OriginStop();
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
BOOL CPressUnitCmd_Initial::State_Recovery() // TODO SLT : 
{
    CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskPressUnit->MakeLog("[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
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
BOOL CPressUnitCmd_Initial::State_NormalComplete()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	pTaskPressUnit->m_bInitialized = TRUE;
	//pTaskPressUnit->SendEvent(CEID_PRESS_UNIT_INIT_END);

	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_Initial::State_Alarm()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskPressUnit->MakeLog("[CMD: %s] [ALARM] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0: // alarm 발생 했을 때 처리해 줘야 할꺼 처리 해줌
		{
			OneAxis* pMotor = (OneAxis*)pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			pMotor->SetStop();
			pMotor->OriginStop();
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
BOOL CPressUnitCmd_Initial::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete(); 
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_Initial::State_Aborting()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskPressUnit->MakeLog("[CMD: %s] [ABORT] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			OneAxis* pMotor = (OneAxis*)pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			pMotor->SetStop();
			pMotor->OriginStop();
			pMotor->EnableSWLimit(DEF_ENABLE);
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
BOOL CPressUnitCmd_Initial::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete(); 
}
