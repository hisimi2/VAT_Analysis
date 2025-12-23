#include "StdAfx.h"
#include "PressUnitCmd_AutoHeightCal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PRESS_DOWN_PRE_POS		(250000.0)
#define GET_TORQUE_COUNT			(10)
CPressUnitCmd_AutoHeightCal::CPressUnitCmd_AutoHeightCal(void)
{
	m_nCmdId      = eSeqCmd_PressUnit_AutoHeightCal;
	m_nTorqueAxisNo = -1;
	m_nCmdStage   = 0;
	m_nRetryCount = 0;
	m_nUnitIdx    = 0;
	m_dLimitTorque     = 0.0;
	m_dContactDownDist = 0.0;
	m_dFindContactPos  = 0.0;
	m_dPressDownOffset = 0.0;
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));
}


CPressUnitCmd_AutoHeightCal::~CPressUnitCmd_AutoHeightCal(void)
{
}


//==============================================================================
//
//==============================================================================
int CPressUnitCmd_AutoHeightCal::CheckParam()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	m_nUnitIdx = pTaskPressUnit->m_nUnitIdx;

	if (m_nUnitIdx <= ePressUnit_Down_8) {
		m_nTorqueAxisNo = eAXIS_PRESS_UNIT_DN1 + m_nUnitIdx;
	}
// 	else {
// 		m_nTorqueAxisNo = eAXIS_PRESS_UNIT_UP1 + (m_nUnitIdx - ePressUnit_Up_1);
// 	}

//	g_MotorDriverComIF.start_comm_running(static_cast<eAxisNo>(m_nTorqueAxisNo));

	m_nWorkCmdStage = m_nCmdStage;

	return ERR_NO_ERROR;
}


//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_AutoHeightCal::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_AutoHeightCal::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_AutoHeightCal::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_AutoHeightCal::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_AutoHeightCal::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_AutoHeightCal::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_AutoHeightCal::State_Executing()
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

	OneAxis* pMotorZ  = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
	if( pMotorZ->ChkMotorUsable() != ERR_NO_ERROR ){
		CParamList AlarmData;	
		AlarmData.m_fnAddItemFormat("PressUnit %d check motor usable command Error", m_nUnitIdx+1);
		AlarmData.m_fnAddItemFormat("Axis=%s ", pMotorZ->GetAxisName());
		ReportAlarm(ERR_PRESS_UNIT_1_MOTOR_ALARM + m_nUnitIdx, AlarmData);
		return FALSE;
	}

	switch( m_nStep )
	{		
		case 0:			// check interlock
		{
			pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE]  [Step=%8d] check interlock", m_szCmdName, m_nStep);
			g_TaskSystemCtrl.SetAutoHeightCalSelect(DEF_AUTO_HEIGHT_CAL_UNK);
			m_dLimitTorque = 0.0;
			m_nRetryCount = 0;
			m_dFindContactPos = 0.0;
			m_nStep++;
		}break;
		case 1:
		{
			double dForceGF = g_DMCont.m_dmHandlerOpt.GD(DDM11_DvcContactForce_GF);
			double nForcePins = g_DMCont.m_dmHandlerOpt.GN(NDM11_DvcContactForce_Pin);
			
			pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE]  [Step=%8d] (ForceGF = %.2f) (ForcePins = %.2f) (Contact Force = %.3f)", m_szCmdName, m_nStep, dForceGF, nForcePins, (dForceGF * nForcePins) / 1000.0);

			m_nStep = 500;
		}break;
	case 500:
		{
			pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Z Axis Move Safety [SRT]", m_szCmdName, m_nStep);
			int nErr = pTaskPressUnit->do_MovePos( LOC_IDX_PRESS_UNIT_SAFETY, 30);
			if( nErr != ERR_NO_ERROR){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d Height cal move safety position error", pTaskPressUnit->m_nUnitIdx+1);
				OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
	case 501:
		{
			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Z Axis Move Safety [END]", m_szCmdName, m_nStep);
				m_nStep = 502;
			}
			else {
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move safety position Error", pTaskPressUnit->m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + pTaskPressUnit->m_nUnitIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move %s pos axis done Error [nAxisDone = %d]", pTaskPressUnit->m_nUnitIdx + 1, ((m_nCmdStage == LOC_IDX_PRESS_UNIT_SAFETY) ? _T("Safety") : _T("Contact")),nAxisDone);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + pTaskPressUnit->m_nUnitIdx, AlarmData);
				}
			}
		}break;
	case 502:
		{
			//10mm를 미리 내린다.
			pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Z Axis Move Safety 10mm Move [SRT]", m_szCmdName, m_nStep);
			int nErr = pTaskPressUnit->do_MovePos( LOC_IDX_PRESS_UNIT_SAFETY, 10, PRESS_DOWN_PRE_POS);
			if( nErr != ERR_NO_ERROR){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d Height cal move safety position error", pTaskPressUnit->m_nUnitIdx+1);
				OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
	case 503:
		{
			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Z Axis Move Safety 100mm Move [END]", m_szCmdName, m_nStep);
				m_nStep = 1000;
			}
			else {
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move safety +100mm position Error", pTaskPressUnit->m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + pTaskPressUnit->m_nUnitIdx, AlarmData);
					break;
				}
				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move %s pos axis done Error [nAxisDone = %d]", pTaskPressUnit->m_nUnitIdx + 1, ((m_nCmdStage == LOC_IDX_PRESS_UNIT_SAFETY) ? _T("Safety") : _T("Contact")), nAxisDone);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + pTaskPressUnit->m_nUnitIdx, AlarmData);
				}
			}
		}break;
	case 1000: // Regulator setting ( Max Mpa = 0.4Mpa )
		{
			pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Regulator setting ( Max Mpa = 0.4Mpa ) [SRT]", m_szCmdName, m_nStep);
			g_TaskRegulator[m_nUnitIdx].do_SetRegulator(0.4);
			Sleep(500);
			m_tcTimeout.SetCheckTime(3.0);
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
	case 1001: // check regulator
		{
			if(m_tcTimeout.CheckOverTime() == TRUE){
				if(m_nRetryCount < 1){ // one time retry
					m_nStep--;
					break;
				}
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("PressUnit %d regulator is not changed max Mpa. Setting : 0.4Mpa", m_nUnitIdx+1);						
				ReportAlarm(ERR_REGULATOR_OUT_OF_RANGE, AlarmData);
				break;	
			}
			BOOL bErr = g_TaskRegulator[m_nUnitIdx].ChkPermitNotOver(10.0);
			if( bErr == TRUE ){
				pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Regulator setting ( Max Mpa = 0.4Mpa ) [END]", m_szCmdName, m_nStep);
				m_tcTimeout.SetCheckTime(30.0);
				m_tcTimeout.StartTimer();
				m_nStep = 2000;
			}			
		}break;
	case 2000: // 정상적으로 torque 값이 읽어지고 있을 때 시작하도록 한다.
		{
			double dCurPos = 0;
			pMotorZ->GetCurrentPos(dCurPos);
//			double dTorque = g_MotorDriverComIF.get_torque(static_cast<eAxisNo>(m_nTorqueAxisNo));
			int nIdx = pMotorZ->GetAxisNo();
			double dTorque = pMotorZ->GetReadActualTorque_PDO(nIdx);
			pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] get_torque Read [GetEncoder = %.2f] [m_dLimitTorque = %.2f ]   [STR] ", m_szCmdName, m_nStep, dCurPos / 1000.0, dTorque);
			if( m_tcTimeout.CheckOverTime() == TRUE )
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d torque stabilized time over!! torqueData=%.2f", pTaskPressUnit->m_nUnitIdx+1, dTorque);
				ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + pTaskPressUnit->m_nUnitIdx, AlarmData);
				break;
			}

			if( dTorque < 300.0){
				m_nStep++;
			}
		}break;
	case 2001: // Get Default volt & Set limit volt // 첫 발생 위치를 찾기
		{			
//			int nErr = pMotorZ->JogMove(1, 500, 50);
			m_dPressDownOffset += 2000.0;

			double dOffset = PRESS_DOWN_PRE_POS + m_dPressDownOffset;
			int nErr = pTaskPressUnit->do_MovePos(LOC_IDX_PRESS_UNIT_SAFETY, 10, dOffset);
//			g_TaskSystemCtrl.MoveJog(m_pMotorZ->GetAxisLogicNo(), 1, m_vSpeed[m_nJogSpeed]);

			if (nErr != ERR_NO_ERROR)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("PressUnit %d can not move z, please check test pp & press unit", m_nUnitIdx + 1);
				OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}

			Sleep(1000);

			// 초기 값에 대비하여 확인해야 하기때문에 한번 값을 가져온다.
			//double dCurretnTorque = GetAvrVoltValue();
			//m_dLimitTorque = 50.0;//( dCurretnTorque >= 100.0 ? 100.0 : dCurretnTorque+10.0 );

//			m_dLimitTorque = GetAvrVoltValue();

			int nIdx = pMotorZ->GetAxisNo();
			double dTorque = pMotorZ->GetReadActualTorque_PDO(nIdx);

			m_dLimitTorque = dTorque;
			double dCurPos = 0;
			pMotorZ->GetCurrentPos(dCurPos);

			pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] V_Move & Volt [GetEncoder = %.2f] [STR] ", m_szCmdName, m_nStep, dCurPos / 1000.0);

			m_tcTimeout.SetCheckTime(25.0);
			m_tcTimeout.StartTimer();
			m_nStep++;
			/*m_nStep = 3000;*/
		}break;
	case 2002: //Move done check
	{
		OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
		int nAxisDone = pMotor->AxisDone();
		if (nAxisDone == ERR_NO_ERROR) {
			pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Z Axis Move 1st Move [END]", m_szCmdName, m_nStep);
			m_nStep = 3000;
		}
		else {
			if (m_tcTimeout.CheckOverTime() == TRUE)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d move  1st position Error", pTaskPressUnit->m_nUnitIdx + 1);
				AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
				ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + pTaskPressUnit->m_nUnitIdx, AlarmData);
				break;
			}
			if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d move %s pos axis done Error [nAxisDone = %d]", pTaskPressUnit->m_nUnitIdx + 1, ((m_nCmdStage == LOC_IDX_PRESS_UNIT_SAFETY) ? _T("Safety") : _T("Contact")), nAxisDone);
				AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
				ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + pTaskPressUnit->m_nUnitIdx, AlarmData);
			}
		}
	}break;
	case 3000: // check first position torque
		{
			double dCurrentPos = 0;
			pMotorZ->GetCurrentPos(dCurrentPos);
			double dLimitPos   = pMotorZ->LimitValueCheck(TRUE);

			if( m_tcTimeout.CheckOverTime() == TRUE )
			{
				pMotorZ->V_MoveStop();
				Sleep(500);

				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d action time over!! can not find first torque position.", pTaskPressUnit->m_nUnitIdx+1);
				ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + pTaskPressUnit->m_nUnitIdx, AlarmData);
				break;
			}

			// check limit
			if( dCurrentPos >= dLimitPos ){
//				pMotorZ->V_MoveStop();
				Sleep(500);

				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("PressUnit %d current position is same limit!! don't move over!!", m_nUnitIdx+1);
				AlarmData.m_fnAddItemFormat("Axis=%s ", pMotorZ->GetAxisName());
				ReportAlarm(ERR_PRESS_UNIT_1_MOTOR_ALARM + m_nUnitIdx, AlarmData);
				break;
			}

			// check regulator
			BOOL bErr = g_TaskRegulator[m_nUnitIdx].ChkPermitNotOver(10.0);
			if( bErr == FALSE ){
//				pMotorZ->V_MoveStop();
				Sleep(500);

				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("PressUnit %d regulator is not changed max Mpa. Setting : 0.4Mpa",m_nUnitIdx+1);
				ReportAlarm(ERR_REGULATOR_OUT_OF_RANGE, AlarmData);
				break;
			}

			// check volt
			Sleep(10);


//			double dCurretnVolt = GetAvrVoltValue();

			int nIdx = pMotorZ->GetAxisNo();
			double dTorque = pMotorZ->GetReadActualTorque_PDO(nIdx);

			int nSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
			int nSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

			double dCurrentVolt = dTorque;
			double dCurPos = 0;
			pMotorZ->GetCurrentPos(dCurPos);
			pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] V_Move & Volt [GetEncoder = %.2f] [dCurretnVolt = %.2f ]", m_szCmdName, m_nStep, dCurPos / 1000.0, dCurrentVolt);
			//TRACE("CurVolt = %.f, Limit =%.f \n", dCurretnVolt, m_dLimitTorque);
			if(dCurrentVolt >= 5.0/*dCurretnVolt > m_dLimitTorque+30.0 || dCurretnVolt > 150.0*/  || g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)//dCurretnVolt > 50.0/*m_dLimitTorque*/ ) // 압력 발생 시점.
			{
//				pMotorZ->V_MoveStop();          // 멈추고
				//TRACE("CurVolt Stop= %.f \n", dCurretnVolt);
				Sleep(500);                    // 0.5초 기달리고
				//dCurretnVolt = GetAvrVoltValue();  // 현재 동작 volt 값 재 확인.
				double dCurPos = 0;
				pMotorZ->GetCurrentPos(dCurPos);
				pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] V_Move & Volt V_MoveStop [GetEncoder = %.2f] [dCurretnVolt = %.2f ]", m_szCmdName, m_nStep, dCurPos / 1000.0, dCurrentVolt);
				//if( dCurretnVolt > m_dLimitTorque ) // 멈추고 보면서 0.2volt 넘는지 본다. //Torque값이 간혹 -로 나올 때 있음.
				{ 
					//TRACE(" Step3500under %2.f\n", dCurretnVolt);
					//m_dLimitTorque = 250.0;
					if (nSiteX * nSiteY == STATION_MAX_X_SITE * STATION_MAX_Y_SITE) {
						m_dLimitTorque = 53;
					}		
					else {
						m_dLimitTorque = 15;
					}
						
					pMotorZ->GetCurrentPos(m_dContactDownDist);
					m_dContactDownDist -= 2000.0;
					m_nStep = 4000;
					break;
				}
			}
			else {
				m_nStep = 2001;
				break;
			}
		}break;
	case 4000: // 안전 위치보다 낮으면 안되기 때문에 확인한다.
		{
			double dCurPos = 0;
			pMotorZ->GetCurrentPos(dCurPos);
			int nIdx = pMotorZ->GetAxisNo();
			double dTorque = pMotorZ->GetReadActualTorque_PDO(nIdx);
			pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] V_Move & Volt [GetEncoder = %.2f] [m_dLimitTorque = %.2f ] [END]", m_szCmdName, m_nStep, dCurPos / 1000.0, dTorque);
//			pMotorZ->V_MoveStop();
			Sleep(500);

			double dSafetyPos = 0.0;
			pTaskPressUnit->GetTeachPos(LOC_IDX_PRESS_UNIT_SAFETY, &dSafetyPos);

			if( m_dContactDownDist < dSafetyPos ){
				m_dContactDownDist = dSafetyPos;
			}
			
			m_nStep = 5000;
		}break;
	case 5000: // 압력 발생 시점 - 2mm로부터 시작하여 contact force 찾는다.
		{
			double dCurPos = 0;
			pMotorZ->GetCurrentPos(dCurPos);
			int nIdx = pMotorZ->GetAxisNo();
			double dTorque = pMotorZ->GetReadActualTorque_PDO(nIdx);
			pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] -2mm Down [GetEncoder = %.2f] [m_dLimitTorque = %.2f ] [SRT]", m_szCmdName, m_nStep, dCurPos / 1000.0, dTorque);
			m_dPressDownOffset -= 2000;
			m_nStep++;
		}break;
	case 5001: // 압력 발생 시점 - 2mm로부터 시작하여 contact force 찾는다.
		{			
			double dCurOffset = PRESS_DOWN_PRE_POS + m_dPressDownOffset;

//			pTaskPressUnit->do_MovePos( LOC_IDX_PRESS_UNIT_SAFETY, 10, m_dContactDownDist);

			int nErr = pTaskPressUnit->do_MovePos(LOC_IDX_PRESS_UNIT_SAFETY, 10, dCurOffset);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d Height cal move safety position error", pTaskPressUnit->m_nUnitIdx + 1);
				OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime(5.0);
			m_tcTimeout.StartTimer();			
			m_nStep++;
		}break;
	case 5002: // check move done & force
		{
			BOOL bErr = g_TaskRegulator[m_nUnitIdx].ChkPermitNotOver(10.0);
			if( bErr == FALSE ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("PressUnit %d regulator is not changed max Mpa. Setting : 0.4Mpa",m_nUnitIdx+1);	
				ReportAlarm(ERR_REGULATOR_OUT_OF_RANGE, AlarmData);
				break;
			}

			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				m_tcTimeout.SetTimerCheckEnd();
//				double dCurretnVolt = GetAvrVoltValue();  // 현재 동작 volt 값 재 확인.
				Sleep(500);					  //TRACE(" Cur =%.2f, Pos=%.2f\n", dCurretnVolt, pMotorZ->GetCurrentPos());
				int nIdx = pMotorZ->GetAxisNo();
				double dTorque[GET_TORQUE_COUNT] = {0,};
				double dCurretnVolt = 0.0;
				double dCurPos = 0;
				pMotorZ->GetCurrentPos(dCurPos);
				dCurretnVolt = pMotorZ->GetReadActualTorque_PDO(nIdx);
				for (int i = 0; i < GET_TORQUE_COUNT; i++)
				{				
					Sleep(100);
					dTorque[i] = pMotorZ->GetReadActualTorque_PDO(nIdx);
					if (dTorque[i] < dCurretnVolt)
					{
						dCurretnVolt = dTorque[i];
					}
					pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Torque Try[%d] [GetEncoder = %.2f] [dCurretnVolt = %.2f ] > [m_dLimitTorque = %.2f]", m_szCmdName, m_nStep, i + 1, dCurPos / 1000.0, dTorque[i], m_dLimitTorque);
			
				}
							
				pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] 0.1mm Down [GetEncoder = %.2f] [dCurretnVolt = %.2f ] > [m_dLimitTorque = %.2f]", m_szCmdName, m_nStep, dCurPos / 1000.0, dCurretnVolt, m_dLimitTorque);
				if (dCurretnVolt >= m_dLimitTorque || g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
					pMotorZ->GetCurrentPos(m_dFindContactPos);
					m_nStep = 6000;
					break;
				}
				else { // not find contact force position
					   //TRACE(" not find contact pos, cur pos = %.2f, dist = %.2f\n", pMotorZ->GetCurrentPos(), m_dContactDownDist);
					m_dPressDownOffset += 100.0;
//					m_dContactDownDist += 100.0;
					m_nStep = 5001;
					break;
				}
			}
			else {
				double dCurPos = 0;
				pMotorZ->GetCurrentPos(dCurPos);
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move %s pos Error", pTaskPressUnit->m_nUnitIdx + 1, ((m_nCmdStage == LOC_IDX_PRESS_UNIT_SAFETY) ? _T("Safety") : _T("Contact")));
					AlarmData.m_fnAddItemFormat("SetTime=%.2f, z cur pos=%.2f", m_tcTimeout.GetCheckTime(), dCurPos);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + pTaskPressUnit->m_nUnitIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move %s pos axis done Error [nAxisDone = %d]", pTaskPressUnit->m_nUnitIdx + 1, ((m_nCmdStage == LOC_IDX_PRESS_UNIT_SAFETY) ? _T("Safety") : _T("Contact")),nAxisDone);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + pTaskPressUnit->m_nUnitIdx, AlarmData);
				}
			}
		}break;

	case 6000:
		{
			double dCurPos = 0;
			pMotorZ->GetCurrentPos(dCurPos);

			int nIdx = pMotorZ->GetAxisNo();
			double dCurretnVolt = pMotorZ->GetReadActualTorque_PDO(nIdx);

			pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] 1mm Down [GetEncoder = %.2f] [m_dLimitTorque = %.2f ] [END]", m_szCmdName, m_nStep, dCurPos / 1000.0, dCurretnVolt);
			pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Z Axis Safety Move [SRT]", m_szCmdName, m_nStep);
			int nErr = pTaskPressUnit->do_MovePos( LOC_IDX_PRESS_UNIT_SAFETY, 50);
			if( nErr != ERR_NO_ERROR){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d Height cal move safety position error", pTaskPressUnit->m_nUnitIdx+1);
				OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
	case 6001:
		{
			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				m_nStep = 7000;
				pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Z Axis Safety Move [END]", m_szCmdName, m_nStep);
			}
			else
			{
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move safety position Error", pTaskPressUnit->m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + pTaskPressUnit->m_nUnitIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move %s pos axis done Error [nAxisDone = %d]", pTaskPressUnit->m_nUnitIdx + 1, ((m_nCmdStage == LOC_IDX_PRESS_UNIT_SAFETY) ? _T("Safety") : _T("Contact")),nAxisDone);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + pTaskPressUnit->m_nUnitIdx, AlarmData);
				}
			}
		}break;
	case 7000: // finish & ask save data? // cylinder 3mm에서 1mm는 눌러주어야 한다.
		{
			pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Auto Height Cal Finish", m_szCmdName, m_nStep);
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("[Press %d] Find Contact Position = %.2f mm", m_nUnitIdx+1, (double)(m_dFindContactPos/1000.0));
			//NotChangeStepReportAlarm(MSG_SYSTEM_AUTO_HEGITH_CAL_FINISH, AlarmData, &g_TaskSystemCtrl);
			g_Error.AlarmReport(ERR_SYSTEM_AUTO_HEGITH_CAL_FINISH, AlarmData, &g_TaskSystemCtrl);

			m_nStep++;
		}break;
	case 7001:
		{
			int nRet = g_TaskSystemCtrl.GetAutoHeightCalSelect();
			
			if( nRet != DEF_AUTO_HEIGHT_CAL_UNK ){
				if( nRet == DEF_AUTO_HEIGHT_CAL_SAVE ){
					pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Auto Height Cal Save", m_szCmdName, m_nStep);
					g_DMCont.m_Teach.SetTeachPos(m_nUnitIdx, 1, 0, m_dFindContactPos/*+1000.0*/);	// cylinder 3mm에서 1mm는 눌러주어야 한다.
					g_DMCont.m_Teach.Save();

					//Auto Height Cal Data File로 관리. [4/28/2025 dohyeong.kim]
					pTaskPressUnit->SaveFileHeightCalData(m_dFindContactPos);
				}
				m_nStep = 8000;
			}
		}break;
	case 8000:
		{
			pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Auto Height Cal END", m_szCmdName, m_nStep);
			g_TaskSystemCtrl.SetAutoHeightCalSelect(DEF_AUTO_HEIGHT_CAL_UNK);
			ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
		}break;
	}
	return CBaseSeqCmd::State_Executing();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_AutoHeightCal::State_Pause()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskPressUnit->MakeLog("[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep , m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			pMotor->V_MoveStop();
			Sleep(50);
			pMotor->SetStop();
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

	return CBaseSeqCmd::State_Pause();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_AutoHeightCal::State_Recovery() // TODO SLT : 
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
BOOL CPressUnitCmd_AutoHeightCal::State_NormalComplete()
{
//	g_MotorDriverComIF.stop_comm_running(static_cast<eAxisNo>(m_nTorqueAxisNo));

	double dForcePerPin = g_DMCont.m_dmHandlerOpt.GD(DDM11_DvcContactForce_GF);
	if (dForcePerPin <= 0.0) {
		dForcePerPin = 0.0;
	}

	g_TaskRegulator[m_nUnitIdx].do_SetForcePerPin(dForcePerPin);
	Sleep(500);


	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_AutoHeightCal::State_Alarm()
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
			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			pMotor->V_MoveStop();
			Sleep(50);
			pMotor->SetStop();
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

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_AutoHeightCal::State_AlarmComplete()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
	pMotor->V_MoveStop();
	pMotor->SetStop();

//	g_MotorDriverComIF.stop_comm_running(static_cast<eAxisNo>(m_nTorqueAxisNo));
	return CBaseSeqCmd::State_AlarmComplete(); 
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_AutoHeightCal::State_Aborting()
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
			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			pMotor->V_MoveStop();
			Sleep(50);
			pMotor->SetStop();
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
BOOL CPressUnitCmd_AutoHeightCal::State_AbortComplete()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
	pMotor->V_MoveStop();
	pMotor->SetStop();

//	g_MotorDriverComIF.stop_comm_running(static_cast<eAxisNo>(m_nTorqueAxisNo));

	double dForcePerPin = g_DMCont.m_dmHandlerOpt.GD(DDM11_DvcContactForce_GF);
	if (dForcePerPin <= 0.0) {
		dForcePerPin = 0.0;
	}

	g_TaskRegulator[m_nUnitIdx].do_SetForcePerPin(dForcePerPin);
	Sleep(500);

	return CBaseSeqCmd::State_AbortComplete(); 
}

