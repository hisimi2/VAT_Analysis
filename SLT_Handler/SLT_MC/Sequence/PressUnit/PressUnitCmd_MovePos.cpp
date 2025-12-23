#include "StdAfx.h"
#include "PressUnitCmd_MovePos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPressUnitCmd_MovePos::CPressUnitCmd_MovePos(void)
{
	m_nCmdId = eSeqCmd_PressUnit_MovePos;
	m_nCmdStage = 0;
	m_nRetryCount = 0;
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));
	m_bBlowOff = FALSE;
	m_nUnitIdx = 0;
	
	for(int nY=0; nY < STATION_MAX_Y_SITE; nY++ )
	{
		for(int nX=0; nX < STATION_MAX_Y_SITE; nX++ )
		{
			m_bVisionErrSkip[nX][nY] = FALSE;
		}
	}

	m_nSiteDivX = 0;
	m_nSiteDivY = 0;
	m_nBackVibratorStep = 0;
	m_nVibCurTryCnt = 0;
}


CPressUnitCmd_MovePos::~CPressUnitCmd_MovePos(void)
{
}


//==============================================================================
//
//==============================================================================
int CPressUnitCmd_MovePos::CheckParam()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	m_tcTimeout.SetCheckTime((MOTOR_TIME_OUT_MIDDLE / 3.0));
	m_tcTimeout.StartTimer();

	m_nUnitIdx = pTaskPressUnit->m_nUnitIdx;
	m_bBlowOff = FALSE;

	m_nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	m_nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	m_nWorkCmdStage = m_nCmdStage;

	m_nVibCurTryCnt = 0;

	return ERR_NO_ERROR;
}


//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_MovePos::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_MovePos::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_MovePos::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_MovePos::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_MovePos::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_MovePos::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_MovePos::State_Executing()
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
		AlarmData.m_fnAddItemFormat("Press Unit %d check motor usable Cmd Error", m_nUnitIdx+1);
		AlarmData.m_fnAddItemFormat("Axis=%s ", pMotorZ->GetAxisName());
		ReportAlarm(ERR_PRESS_UNIT_1_MOTOR_ALARM + m_nUnitIdx, AlarmData);
		return FALSE;
	}

	switch( m_nStep )
	{		
	case 0:			// check interlock
		{
			pTaskPressUnit->MakeLog("[%s] m_nCmdStage = %d [EXEC=%8d]",m_szCmdName , m_nCmdStage,m_nStep);
			if(m_nCmdStage == LOC_IDX_PRESS_UNIT_SAFETY){
				//pTaskPressUnit->SendEvent(CEID_PRESS_UNIT_DISCONTACT_START);
				m_nStep = 5000;
			}else{
				//pTaskPressUnit->SendEvent(CEID_PRESS_UNIT_CONTACT_START);

				// movepos는 manual에서만 contact 위치 움직이는 용도로 사용해야함
				// PressUnitCmd_Contact class로 auto 에서 동작 해야 한다.
				m_nStep = 1000;
			}
		}break;

		//===========================================//
		// Move to Contact Position
		//===========================================//
	case 1000: // arrived sensor [ 안착 센서 ]
		{
			if(g_DMCont.m_dmHandlerOpt.GB(BDM11_PRS_DEVICE_ARRIVE_ENABLE) == DEF_ON)
			{
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Arrived Sensor Check](STR)",m_szCmdName ,m_nStep);

				CString strErr = _T("");

				std::vector<CPoint> vErrPocket;
				vErrPocket.clear();

				std::vector<CString> vErrHistory;
				int nDeviceType = 0;
				int nErrArrivedSen = pTaskPressUnit->ChkSocketArrivedSen_All(strErr,vErrPocket, vErrHistory, nDeviceType);
				
				int nRetPos = pTaskPressUnit->ChkMotorPos(LOC_IDX_PRESS_UNIT_CONTACT, CTaskPressUnit::eAxisZ);
				if( nErrArrivedSen != ERR_NO_ERROR && nRetPos != ERR_NO_ERROR){
					if (g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_VIBRATOR_OPTION) == DEF_ON)
					{
						int nVibRetryCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_VIBRATOR_RETRY_CNT);
						if (m_nVibCurTryCnt >= nVibRetryCnt)   // Vibrator를 Retry 횟수만큼 완료
						{
							m_nVibCurTryCnt = 0;
						}
						else     //Vibrator를 Retry 시작
						{
							m_nBackVibratorStep = m_nStep;
							m_nStep = do_Vibrator_On; // Vibrator 동작 Case								
							break;
						}
					}

					int nErr = nDeviceType == eDeviceType::_eDeviceType_Normal ? ERR_PRESS_UNIT_1_ARRIVED_SENSOR : ERR_PRESS_UNIT_1_CLEAN_DEVICE_ARRIVED_SENSOR;

					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Station =%d, Site =%s", m_nUnitIdx+1, strErr);
					ReportAlarm(nErr + m_nUnitIdx, AlarmData,vErrPocket);
					break;
				}
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Arrived Sensor Check](END)",m_szCmdName ,m_nStep);
			}

			m_nStep = 3000;
			
		}break;
	case 3000: // move 1 step -15mm
		{
			double dTeachPosZ = 0.0, dContactOffSet = 0.0;
			pTaskPressUnit->GetTeachPos(LOC_IDX_PRESS_UNIT_CONTACT, &dTeachPosZ);
			int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

			dContactOffSet = PRESS_MARLIN_CONTACT_DIST_OFFSET;
	
			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Move to %s Pos](STR)",m_szCmdName ,m_nStep,((m_nCmdStage==LOC_IDX_PRESS_UNIT_SAFETY)? _T("Safety") : _T("Contact -5mm")));
			int nErr = pTaskPressUnit->do_MovePos( m_nCmdStage , 100, dContactOffSet);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d move %s pos cmd Error", m_nUnitIdx+1,((m_nCmdStage==LOC_IDX_PRESS_UNIT_SAFETY)? _T("Safety") : _T("Contact")) );
				OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime( (MOTOR_TIME_OUT_MIDDLE / 3.0) );
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
	case 3001: // check move complete
		{
			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR)
			{
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Move to %s Pos](END)", m_szCmdName, m_nStep, ((m_nCmdStage == LOC_IDX_PRESS_UNIT_SAFETY) ? _T("Safety") : _T("Contact -5mm")));
				m_nStep = 3500;
			}
			else
			{
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d  move %s pos Error", m_nUnitIdx + 1, ((m_nCmdStage == LOC_IDX_PRESS_UNIT_SAFETY) ? _T("Safety") : _T("Contact")));
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + m_nUnitIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d  move %s pos axis done Error [nAxisDone = %d]", m_nUnitIdx + 1, ((m_nCmdStage == LOC_IDX_PRESS_UNIT_SAFETY) ? _T("Safety") : _T("Contact")), nAxisDone);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + m_nUnitIdx, AlarmData);
				}
			}
		}break;
	case 3500: // check arrived sen
		{
			if( g_DMCont.m_dmHandlerOpt.GB(BDM11_PRS_DEVICE_ARRIVE_ENABLE) == DEF_ON )
			{
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Arrived Sensor Check](STR)",m_szCmdName ,m_nStep);

				CString strErr = _T("");

				std::vector<CPoint> vErrPocket;
				vErrPocket.clear();

				std::vector<CString> vErrHistory;
				int nDeviceType = 0;
				int nErrArrivedSen = pTaskPressUnit->ChkSocketArrivedSen_All(strErr,vErrPocket, vErrHistory, nDeviceType);

				int nRetPos = pTaskPressUnit->ChkMotorPos(LOC_IDX_PRESS_UNIT_CONTACT, CTaskPressUnit::eAxisZ);
				if( nErrArrivedSen != ERR_NO_ERROR && nRetPos != ERR_NO_ERROR){
					if (g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_VIBRATOR_OPTION) == DEF_ON)
					{
						int nVibRetryCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_VIBRATOR_RETRY_CNT);
						if (m_nVibCurTryCnt >= nVibRetryCnt)   // Vibrator를 Retry 횟수만큼 완료
						{
							m_nVibCurTryCnt = 0;
						}
						else     //Vibrator를 Retry 시작
						{
							m_nBackVibratorStep = m_nStep;
							m_nStep = do_Vibrator_On; // Vibrator 동작 Case								
							break;
						}
					}

					int nErr = nDeviceType == eDeviceType::_eDeviceType_Normal ? ERR_PRESS_UNIT_1_ARRIVED_SENSOR : ERR_PRESS_UNIT_1_CLEAN_DEVICE_ARRIVED_SENSOR;

					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Station =%d, Site =%s", m_nUnitIdx+1, strErr);
					ReportAlarm(nErr + m_nUnitIdx, AlarmData,vErrPocket);
					break;
				}
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Arrived Sensor Check](END)",m_szCmdName ,m_nStep);
			}
			m_nStep = 4000;
		}break;
	case 4000: // move 2Step // Target 위치 대비 5mm 이전부터 Target까지 기존 속도 대비 10%로 움직임.
		{
			int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
			int nSpeed = 0;

			nSpeed = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_CONTACT_2ST_SPEED);

			if (nSpeed < PRESS_MIN_CONTACT_SPEED) // 5%보다 낮으면 최소값 5% 고정
				nSpeed = PRESS_MIN_CONTACT_SPEED;

			if (nSpeed > PRESS_MAX_CONTACT_SPEED) // 15%보다 높으면 최대값 15% 고정
				nSpeed = PRESS_MAX_CONTACT_SPEED;

			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Move to %s Pos](STR)",m_szCmdName ,m_nStep,((m_nCmdStage==LOC_IDX_PRESS_UNIT_SAFETY)? _T("Safety") : _T("Contact")));
			int nErr = pTaskPressUnit->do_MovePos( m_nCmdStage , nSpeed);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d move %s pos cmd Error", m_nUnitIdx+1, ((m_nCmdStage==LOC_IDX_PRESS_UNIT_SAFETY)? _T("Safety") : _T("Contact")) );
				OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime( (MOTOR_TIME_OUT_MIDDLE / 3.0) );
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
	case 4001:
		{
			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR)
			{
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Move to %s Pos](END)", m_szCmdName, m_nStep, ((m_nCmdStage == LOC_IDX_PRESS_UNIT_SAFETY) ? _T("Safety") : _T("Contact")));
				m_nStep = 6000;
			}
			else
			{
				double dCurPos = 0;
				pMotor->GetCurrentPos(dCurPos);
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move %s pos Error", m_nUnitIdx + 1, ((m_nCmdStage == LOC_IDX_PRESS_UNIT_SAFETY) ? _T("Safety") : _T("Contact")));
					AlarmData.m_fnAddItemFormat("SetTime=%.2f, z cur pos=%.2f", m_tcTimeout.GetCheckTime(), dCurPos);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + m_nUnitIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move %s pos axis done Error [nAxisDone = %d]", m_nUnitIdx + 1, ((m_nCmdStage == LOC_IDX_PRESS_UNIT_SAFETY) ? _T("Safety") : _T("Contact")), nAxisDone);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + m_nUnitIdx, AlarmData);
				}
			}
		}break;

		//===========================================//
		// Move to Safety Position
		//===========================================//
	case 5000:
		{			
			m_nStep = 5100;			
		}break;
	case 5100: // Blow Contact Pos 
		{		
			int nRetPos = pTaskPressUnit->ChkMotorPos(LOC_IDX_PRESS_UNIT_SAFETY, CTaskPressUnit::eAxisZ);
			int nContactOption = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_BLOW_POS_OPTION);

			if (nContactOption == CHECK_CONTACT_POS_NOT_USE || nRetPos == ERR_NO_ERROR)
			{
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Blow](STR)", m_szCmdName, m_nStep);
				int nErrVac = pTaskPressUnit->do_VacBlow(eVacBlwSt_BlwOn);
				if (nErrVac != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d  blow on command error", m_nUnitIdx + 1);
					ReportAlarm(nErrVac, AlarmData);
					break;

				}
				m_nStep = 5103;
				break;
			}
			double nContactPos = 0.0;
			nContactPos = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS1_BLOW_POS + m_nUnitIdx) * 1000.0;

			int nErr = pTaskPressUnit->do_MovePos(LOC_IDX_PRESS_UNIT_CONTACT, 100, nContactPos);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d move z contact pos cmd Error", m_nUnitIdx + 1);
				OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}

			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [User Contact Pos Move Start](STR)", m_szCmdName, m_nStep);

			m_tcTimeout.SetCheckTime((MOTOR_TIME_OUT_MIDDLE / 3.0));
			m_tcTimeout.StartTimer();

			m_nStep++;
		}
		break;
	case 5101: // Blow Contact Pos Done 
		{
			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR)
			{
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Move to %s Pos](END)", m_szCmdName, m_nStep, ((m_nCmdStage == LOC_IDX_PRESS_UNIT_CONTACT) ? _T("Safety") : _T("Contact")));
				m_cPertm.StartTmChk();
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Blow Press Wait Start](STR)", m_szCmdName, m_nStep);
				m_nStep++; //= 6000;
			}
			else
			{
				double dCurPos = 0;
				pMotor->GetCurrentPos(dCurPos);
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move %s pos Error", m_nUnitIdx + 1, ((m_nCmdStage == LOC_IDX_PRESS_UNIT_CONTACT) ? _T("Safety") : _T("Contact")));
					AlarmData.m_fnAddItemFormat("SetTime=%.2f, z cur pos=%.2f", m_tcTimeout.GetCheckTime(), dCurPos);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + m_nUnitIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move %s pos axis done Error [nAxisDone = %d]", m_nUnitIdx + 1, ((m_nCmdStage == LOC_IDX_PRESS_UNIT_CONTACT) ? _T("Safety") : _T("Contact")),nAxisDone);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + m_nUnitIdx, AlarmData);
				}
			}
		}
		break;

	case 5102: // Contact Pos Offset 위치에서 Blow Time적용. Contact Pos Offset -> Safety 위치로 1단동작  
		{			
			double dBlowTime = g_DMCont.m_dmHandlerOpt.GN(NDM11_TM_PRESS1_BLOW_WAIT + m_nUnitIdx);

			int nErrVac = pTaskPressUnit->do_VacBlow(eVacBlwSt_BlwOn);
			if (nErrVac != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d  blow off command error", m_nUnitIdx + 1);
				ReportAlarm(nErrVac, AlarmData);
				break;
			}
			m_cPertm.EndTmChk();

			if (m_cPertm.GetTmDistanceMS() < dBlowTime)
				break;

			nErrVac = pTaskPressUnit->do_VacBlow(eVacBlwSt_Off);
			if (nErrVac != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d  blow off command error", m_nUnitIdx + 1);
				ReportAlarm(nErrVac, AlarmData);
				break;
			}

			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Blow Press Wait End](STR)", m_szCmdName, m_nStep);
			m_nStep++;

		}
	break; 
	case 5103: // Safety Pos 이동 
		{				
			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Move to %s Pos](STR)", m_szCmdName, m_nStep, ((m_nCmdStage == LOC_IDX_PRESS_UNIT_SAFETY) ? _T("Safety") : _T("Contact")));
			int	nErr = pTaskPressUnit->do_MovePos(m_nCmdStage);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d move %s pos cmd Error", m_nUnitIdx + 1, ((m_nCmdStage == LOC_IDX_PRESS_UNIT_SAFETY) ? _T("Safety") : _T("Contact")));
				OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime((MOTOR_TIME_OUT_MIDDLE / 3.0));
			m_tcTimeout.StartTimer();
			double dBlowWait = g_DMCont.m_dmHandlerOpt.GN(NDM11_TM_PRESS1_BLOW_WAIT + m_nUnitIdx) / 1000.0;
			m_tcBlowTimeout.SetCheckTime(dBlowWait);
			m_tcBlowTimeout.StartTimer();
			m_bBlowOff = FALSE;
			m_nStep++;
		}
		break;
	case 5104:
		{
			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			int nAxisDone = pMotor->AxisDone();
			int nChkSenSafety = pTaskPressUnit->ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON);
			if (nAxisDone == ERR_NO_ERROR && nChkSenSafety == ERR_NO_ERROR)
			{
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Move to %s Pos](END)", m_szCmdName, m_nStep, ((m_nCmdStage == LOC_IDX_PRESS_UNIT_SAFETY) ? _T("Safety") : _T("Contact")));
				int nErrVac = pTaskPressUnit->do_VacBlow(eVacBlwSt_Off);
				if (nErrVac != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d  blow off command error", m_nUnitIdx + 1);
					ReportAlarm(nErrVac, AlarmData);
					break;
				}
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Blow](END)", m_szCmdName, m_nStep);
				m_nStep = 5500;
			}
			else
			{
				double dCurPos = 0;
				pMotor->GetCurrentPos(dCurPos);
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move %s pos Error", m_nUnitIdx + 1, ((m_nCmdStage == LOC_IDX_PRESS_UNIT_SAFETY) ? _T("Safety") : _T("Contact")));
					AlarmData.m_fnAddItemFormat("SetTime=%.2f, z cur pos=%.2f", m_tcTimeout.GetCheckTime(), dCurPos);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + m_nUnitIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE && nAxisDone != ERR_NO_ERROR)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move %s pos axis done Error [nAxisDone = %d]", m_nUnitIdx + 1, ((m_nCmdStage == LOC_IDX_PRESS_UNIT_SAFETY) ? _T("Safety") : _T("Contact")),nAxisDone);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + m_nUnitIdx, AlarmData);
				}
			}
		}
		break;
	case 5500:
		{
			if (g_DMCont.m_dmHandlerOpt.GB(BDM11_PRS_DEVICE_ARRIVE_ENABLE) == DEF_ON)
			{
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Arrived Sensor Check](STR)", m_szCmdName, m_nStep);

				CString strErr = _T("");

				std::vector<CPoint> vErrPocket;
				vErrPocket.clear();

				std::vector<CString> vErrHistory;
				int nDeviceType = 0;
				int nErrArrivedSen = pTaskPressUnit->ChkSocketArrivedSen_All(strErr, vErrPocket, vErrHistory, nDeviceType, TRUE);

				int nRetPos = pTaskPressUnit->ChkMotorPos(LOC_IDX_PRESS_UNIT_CONTACT, CTaskPressUnit::eAxisZ);
				if (nErrArrivedSen != ERR_NO_ERROR && nRetPos != ERR_NO_ERROR) 
				{
					if (g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_VIBRATOR_OPTION) == DEF_ON)
					{
						int nVibRetryCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_VIBRATOR_RETRY_CNT);
						if (m_nVibCurTryCnt >= nVibRetryCnt)   // Vibrator를 Retry 횟수만큼 완료
						{
							m_nVibCurTryCnt = 0;
						}
						else     //Vibrator를 Retry 시작
						{
							m_nBackVibratorStep = m_nStep;
							m_nStep = do_Vibrator_On; // Vibrator 동작 Case								
							break;
						}
					}

					int nErr = nDeviceType == eDeviceType::_eDeviceType_Normal ? ERR_PRESS_UNIT_1_ARRIVED_SENSOR_MOVE_SAFETY : ERR_PRESS_UNIT_1_CLEAN_DEVICE_ARRIVED_SENSOR_MOVE_SAFETY;

					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Station =%d, Site =%s", m_nUnitIdx + 1, strErr);
					ReportAlarm(nErr + m_nUnitIdx, AlarmData, vErrPocket);
					break;
				}
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Arrived Sensor Check](END)", m_szCmdName, m_nStep);
			}
			m_nStep++;
		}break;
	case 5501:
		{
			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [ xZAXIS_SAFETY_SEN Check](STR)", m_szCmdName, m_nStep);
			
			int nChkSenSafety = pTaskPressUnit->ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON);

			if (nChkSenSafety != ERR_NO_ERROR)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d safety No Detect", m_nUnitIdx + 1);
				ReportAlarm(ERR_PRESS_UNIT_1_NOT_DETECT_SAFETY_SEN + m_nUnitIdx, AlarmData);
				break;
			}
			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [ xZAXIS_SAFETY_SEN Check](END)", m_szCmdName, m_nStep);

			m_nStep = 6000;
		}break;
	case do_Vibrator_On:
	{
		g_TaskPressUnit[m_nUnitIdx].do_SideVibratorOnOff(DEF_ON);

		tmr.StartTmChk();

		pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [do_Vibrator_On]", m_szCmdName, m_nStep);
		m_nStep = do_Vibrator_On_Wait;

		break;
	}
	case do_Vibrator_On_Wait:
	{
		// Vibrator Delay Time 추가 [0보다 작거나 '-' 값이 들어가면 최소값 0.1로 만듬] [8/4/2020 donghyun.shin]
		double dVibDelayTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_PRESS_VIBRATOR_DELAY_TIME);

		if (dVibDelayTime <= 0)
			dVibDelayTime = 0.05;

		tmr.EndTmChk();
		if (tmr.GetTmDistanceMS() < dVibDelayTime*1000.0)
			break;

		pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [do_Vibrator_On_Wait]", m_szCmdName, m_nStep);
		m_nStep = do_Vibrator_Off;

		break;
	}
	case do_Vibrator_Off:
	{
		g_TaskPressUnit[m_nUnitIdx].do_SideVibratorOnOff(DEF_OFF);

		tmr.StartTmChk();
		pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [do_Vibrator_Off]", m_szCmdName, m_nStep);
		m_nStep = do_Vibrator_Off_Wait;

		break;
	}
	case do_Vibrator_Off_Wait:
	{
		// Vibrator Delay Time 추가 [0보다 작거나 '-' 값이 들어가면 최소값 0.1로 만듬] [8/4/2020 donghyun.shin]
		double dVibDelayTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_PRESS_VIBRATOR_DELAY_TIME);

		if (dVibDelayTime <= 0)
			dVibDelayTime = 0.05;

		tmr.EndTmChk();
		if (tmr.GetTmDistanceMS() < dVibDelayTime*1000.0)
			break;

		pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [do_Vibrator_Off_Wait]", m_szCmdName, m_nStep);
		m_nStep = Vibrator_complete;
		break;
	}
	case Vibrator_complete:
	{
		m_nVibCurTryCnt++;
		m_nStep = m_nBackVibratorStep;
		break;
	}
		// complete
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
BOOL CPressUnitCmd_MovePos::State_Pause()
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
			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			//pMotor->SetSlowStop();
			bool bCloseLimit = pMotor->CheckLimitClose();
			double dCurPos = 0;
			pMotor->GetCurrentPos(dCurPos);
			if (bCloseLimit == false)
			{
				pTaskPressUnit->MakeLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
				pMotor->SetSlowStop();
			}

			pTaskPressUnit->do_VacBlow(eVacBlwSt_Off);
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
BOOL CPressUnitCmd_MovePos::State_Recovery()
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
	case 0:	// 현재 위치와 마지막 위치를 비교 한다.
		{
			m_nStep = 1000;
		}break;
	case 1000: // Move
		{
			pTaskPressUnit->do_VacBlow(eVacBlwSt_BlwOn);

			int nErr = pTaskPressUnit->do_MovePos( LOC_IDX_PRESS_UNIT_SAFETY );
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d move Safety position cmd Error", m_nUnitIdx+1);
				OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime( (MOTOR_TIME_OUT_MIDDLE / 3.0) );
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;

	case 1001: // check move complete
		{
			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR)
			{
				pTaskPressUnit->do_VacBlow(eVacBlwSt_Off);
				if (g_DMCont.m_dmEQP.GB(BDM0_MD_VIBRATION_MODE) == DEF_ON) {
					m_nStep = 2000;
				}
				else {
					m_nStep = 3000;
				}
			}
			else
			{
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move Safety position time over error", m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + m_nUnitIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move Safety position axis done Error [nAxisDone = %d]", m_nUnitIdx + 1, nAxisDone);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + m_nUnitIdx, AlarmData);
				}
			}
		}break;
	case 2000:
		{
// 			pTaskPressUnit->MakeLog("[%s] [RECOVERY=%8d] [Anti-Vibrator cylinder Check](STR)", m_szCmdName, m_nStep);
// 			int nErr = pTaskPressUnit->ChkDI(CTaskPressUnit::xZAXIS_VIBRATION_CYL_UP_SEN, DEF_ON);
// 			pTaskPressUnit->MakeLog("[%s] [RECOVERY=%8d] [Anti-Vibrator cylinder Check](END)", m_szCmdName, m_nStep);
// 
// 			if (nErr == ERR_NO_ERROR) {
// 				m_nStep = 3000;
// 			}
// 			else {
// 				pTaskPressUnit->MakeLog("[%s] [RECOVERY=%8d] [Anti-Vibrator cylinder Up](STR)", m_szCmdName, m_nStep);
// 				pTaskPressUnit->do_VibratorCylUpDown(eCyl_Up);
// 
// 				m_tcAntiVibratorTimeout.SetCheckTime((MOTOR_TIME_OUT_SHORT));
// 				m_tcAntiVibratorTimeout.StartTimer();
// 				m_nStep++;
// 			}
			m_nStep = 3000;
		}break;
	case 3000:
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
BOOL CPressUnitCmd_MovePos::State_NormalComplete()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;

	if( CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto ){
		if(m_nCmdStage == LOC_IDX_PRESS_UNIT_CONTACT){
			//pTaskPressUnit->SendEvent(CEID_PRESS_UNIT_CONTACT_END);
			int nContactCnt = g_DMCont.m_dmEQP.GN(NDM0_CleaningContact_Press1+m_nUnitIdx);
			g_DMCont.m_dmEQP.SN(NDM0_CleaningContact_Press1+m_nUnitIdx, ++nContactCnt);
		}else{ // LOC_IDX_PRESS_UNIT_SAFETY
			//pTaskPressUnit->SendEvent(CEID_PRESS_UNIT_DISCONTACT_END);
			BOOL bModeAutoRetest = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
			ST_DD_DEVICE stDvcData;
			
			//Site Off Reservation 되어 있을 경우 Site Off 가능 유무 확인 하여 Site Off 진행
			char szCompany[16] = { 0, };
			g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

			int nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
			int nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
			
			std::vector<CPoint> vSetSiteOff;
			CString strSiteNum = _T("");
			strSiteNum.Format(_T("%d"), m_nUnitIdx);
			std::vector<int> vSetSiteKind;

			for (int nY = 0; nY < nTestSite_Div_Y; nY++) {
				for (int nX = 0; nX < nTestSite_Div_X; nX++) {
					//if (g_DMCont.m_dmHandlerOpt.GB(BDM11_Reservation_SocketOnOff_Site1_1 + (m_nUnitIdx * (nTestSite_Div_X * nTestSite_Div_Y)) + ((nY)* nTestSite_Div_X) + (nX)) == TRUE) {
					int nSite = g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + (m_nUnitIdx * (STATION_MAX_PARA)) + ((nY)* nTestSite_Div_X) + (nX));
					int nSocketOffKind = DEF_SITE_OFF;

					if (nSite == DEF_SITE_OFF_RESERVATION_YIELD)
						nSocketOffKind = DEF_SITE_OFF_YIELD;

					if (nSite == DEF_SITE_OFF_RESERVATION || nSite == DEF_SITE_OFF_RESERVATION_YIELD) {
					//if (g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + (m_nUnitIdx * (nTestSite_Div_X * nTestSite_Div_Y)) + ((nY)* nTestSite_Div_X) + (nX)) == DEF_SITE_OFF_RESERVATION) {
					// Site Off 유무 확인
						BOOL bSiteOff = TRUE;

						pTaskPressUnit->GetSiteDeviceData(&stDvcData, nX, nY);

						BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stDvcData);

						if ((bModeAutoRetest == DEF_ON && stDvcData.sExist == DEF_EXIST && stDvcData.nTestingCnt == eARB_1stTestEnd && stDvcData.nBinColor == eFailBin && (strcmp(stDvcData.szErrString, DEF_TEST_RESULT_TP101) != 0 && bIsDefaultBin != TRUE) )
							|| ( bModeAutoRetest == DEF_ON && stDvcData.sExist == DEF_EXIST && strcmp(stDvcData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0) 
							|| (stDvcData.sExist == DEF_EXIST && strcmp(stDvcData.szTestSiteNum, strSiteNum) !=0 ) ) {
							bSiteOff = FALSE;
						}

						CString strErrPos = _T("");
						if (bSiteOff == TRUE) {
							BOOL bIsPossibleSiteOff = g_TaskSystemCtrl.CheckAutoRetestRuleSite(m_nUnitIdx, nX, nY);
							if (bIsPossibleSiteOff == TRUE) {
								//Site Off 
								g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + (m_nUnitIdx * (STATION_MAX_PARA)) + ((nY)* nTestSite_Div_X) + (nX), nSocketOffKind);
								vSetSiteOff.push_back(CPoint(nX, nY));
								vSetSiteKind.push_back(nSocketOffKind);
								if (nSocketOffKind == DEF_SITE_OFF_RESERVATION)
									pTaskPressUnit->MakeLog("[Reservation Site Off(%d,%d)]", nX + 1, nY + 1);
								else if (nSocketOffKind == DEF_SITE_OFF_RESERVATION_YIELD)
									pTaskPressUnit->MakeLog("[Reservation Site YIELD Off(%d,%d)]", nX + 1, nY + 1);
							}
						}
					}
				}
			}
			if ((int)vSetSiteOff.size() > 0) {
				g_TaskSystemCtrl.SetSiteOnOffData(m_nUnitIdx, vSetSiteOff, vSetSiteKind);
			}

			BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();//g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);
			BOOL bSocketClean = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_SocketClean);
			BOOL bAutoLoopUse = g_TaskSystemCtrl.GetAutoLoopUse();


			if (bSocketClean == TRUE /*&& bAutoGrrUse == FALSE && bAutoLoopUse == FALSE */) 
			{
				BOOL bInterval = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_CleanIntervalCnt);
				int nContactCnt = g_DMCont.m_dmEQP.GN(NDM0_CleaningContact_Press1 + m_nUnitIdx);
				int nSetCleanCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_SockClnIntervalCnt);
				if (bInterval == TRUE && nSetCleanCnt != 0 && ((nContactCnt) % nSetCleanCnt) == 0)
				{
					g_DMCont.m_dmEQP.SB((BDM0_SOCKETCLEAN_SITE1_RESERVE + m_nUnitIdx), TRUE);
				}

				BOOL bAtoAPickPlace = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_ART_TEST_HAND_PICK_PLACE);
				int nCleanMode = g_DMCont.m_dmEQP.GB((BDM0_SOCKETCLEAN_SITE1_RESERVE + m_nUnitIdx));
				int m_bCleaningManuRev = g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE1 + m_nUnitIdx);
				if (nCleanMode == TRUE || m_bCleaningManuRev == TRUE)
				{
					bAtoAPickPlace = FALSE;
				}

				//AA 일때 Device Status eTestSite_DvcLoading로 변경 
				if (g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE1_RESERVE + m_nUnitIdx) != TRUE && bAtoAPickPlace != eOPTION_USE && bModeAutoRetest == eOPTION_USE) {

					int nPocketCount_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
					int nPocketCount_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

					ST_DD_DEVICE stSite;
					for (int nY = 0; nY < nPocketCount_Y; nY++)
					{
						for (int nX = 0; nX < nPocketCount_X; nX++)
						{
							stSite.clear();

							g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &stSite);

							BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stSite);

							if (stSite.nBinColor == eFailBin && stSite.nTestingCnt == eARB_1stTestEnd && strcmp(stSite.szErrString, DEF_TEST_RESULT_TP101) != 0 && bIsDefaultBin != TRUE)
							{
								stSite.nTestingInfo = eTestSite_DvcLoading;
								g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &stSite);
							}
						}
					}
				}//if (g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE1_RESERVE + m_nUnitIdx) != TRUE && bAtoAPickPlace != eOPTION_USE && bModeAutoRetest == eOPTION_USE)

			}//if( bSocketClean == TRUE && bAutoGrrUse == FALSE )



			if (bAutoGrrUse == TRUE) {

				if (bSocketClean == TRUE) {
					BOOL bInterval = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_CleanIntervalCnt);
					int nContactCnt = g_DMCont.m_dmEQP.GN(NDM0_CleaningContact_Press1 + m_nUnitIdx);
					int nSetCleanCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_SockClnIntervalCnt);

					BOOL bAtoAPickPlace = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_ART_TEST_HAND_PICK_PLACE);
					int nCleanMode = g_DMCont.m_dmEQP.GB((BDM0_SOCKETCLEAN_SITE1_RESERVE + m_nUnitIdx));
					int m_bCleaningManuRev = g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE1 + m_nUnitIdx);
					if (nCleanMode == TRUE || m_bCleaningManuRev == TRUE)
					{
						bAtoAPickPlace = FALSE;
					}

					//AA 일때 Device Status eTestSite_DvcLoading로 변경 
					if (g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE1_RESERVE + m_nUnitIdx) != TRUE && bAtoAPickPlace != eOPTION_USE && bModeAutoRetest == eOPTION_USE) {

						int nPocketCount_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
						int nPocketCount_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

						ST_DD_DEVICE stSite;
						for (int nY = 0; nY < nPocketCount_Y; nY++)
						{
							for (int nX = 0; nX < nPocketCount_X; nX++)
							{
								stSite.clear();

								g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &stSite);

								BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stSite);

								if (stSite.nBinColor == eFailBin && stSite.nTestingCnt == eARB_1stTestEnd && strcmp(stSite.szErrString, DEF_TEST_RESULT_TP101) != 0 && bIsDefaultBin != TRUE)
								{
									stSite.nTestingInfo = eTestSite_DvcLoading;
									g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &stSite);
								}
							}
						}
					}//if (g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE1_RESERVE + m_nUnitIdx) != TRUE && bAtoAPickPlace != eOPTION_USE && bModeAutoRetest == eOPTION_USE)

				}//if( bSocketClean == TRUE && bAutoGrrUse == FALSE )
				else {
					//GRR Mode에 따른 Device 상태 변경
					ST_DD_DEVICE stSite;
					int nPocketCount_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
					int nPocketCount_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
					int nGrrMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrMode);

					for (int nY = 0; nY < nPocketCount_Y; nY++)
					{
						for (int nX = 0; nX < nPocketCount_X; nX++)
						{
							pTaskPressUnit->GetSiteDeviceData(&stSite, nX, nY);

							if (stSite.sExist == DEF_EXIST) {
								int nFindIdx = 0;
								nFindIdx = stSite.findIndexTestSite(stSite.szTestSiteNum);
								ASSERT(nFindIdx >= 0);

								if (stSite.stGrrTestInfo[nFindIdx].chkTestEnd() != TRUE && nGrrMode != eNormal_Mode) {
									pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_DvcLoading);
								}
							}
						}
					}
				}
			}
			else if (bAutoLoopUse == TRUE) {
				ST_DD_DEVICE stSite;
				int nPocketCount_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
				int nPocketCount_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
				int nLoopMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoLoopMode);

				for (int nY = 0; nY < nPocketCount_Y; nY++)
				{
					for (int nX = 0; nX < nPocketCount_X; nX++)
					{
						pTaskPressUnit->GetSiteDeviceData(&stSite, nX, nY);

						if (stSite.sExist == DEF_EXIST) {

							if(strcmp(szCompany, DEF_COMPANY_AMKOR) == 0)
							{
								if (stSite.stLoopTestInfo[m_nUnitIdx].chkTestEnd() != TRUE && nLoopMode != eNormal_Mode) {
									pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_DvcLoading);
								}
							}
							else {
								if (stSite.stLoopTestInfo[m_nUnitIdx].chkTestEnd() != TRUE && stSite.stLoopTestInfo[m_nUnitIdx].chkRePickPlace() == FALSE) {
									pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_DvcLoading);
								}
							}							
						}
					}
				}
			}
		}
	}
	

	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_MovePos::State_Alarm()
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
	case 0: // alarm 발생 했을 때 처리해5 줘야 할꺼 처리 해줌
		{
			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			//pMotor->SetSlowStop();
			bool bCloseLimit = pMotor->CheckLimitClose();
			double dCurPos = 0;
			pMotor->GetCurrentPos(dCurPos);
			if (bCloseLimit == false)
			{
				pTaskPressUnit->MakeLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
				pMotor->SetSlowStop();
			}

			pTaskPressUnit->do_VacBlow(eVacBlwSt_Off);
			m_nStep++;
		}break;
	case 1: // Retry, Skip을 기다림.
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
BOOL CPressUnitCmd_MovePos::State_AlarmComplete()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskPressUnit->MakeLog("[CMD: %s] [ALARM COMP] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);

	switch( m_nStep )
	{
	case 0: // Move
		{
			int nErrVac = pTaskPressUnit->do_VacBlow(eVacBlwSt_Off);
			if(nErrVac != ERR_NO_ERROR){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d blow on command error", m_nUnitIdx+1);
				ReportAlarm(nErrVac, AlarmData);
				break;
			}

			int nErr = pTaskPressUnit->do_MovePos( LOC_IDX_PRESS_UNIT_SAFETY );
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d move safety pos cmd Error", m_nUnitIdx+1);
				OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime( (MOTOR_TIME_OUT_MIDDLE/3.0) );
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
	case 1: // check move complete
		{			
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR)
			{
				ST_DD_DEVICE stEmptyDevice; stEmptyDevice.clear();
				for (int nY = 0; nY < m_nSiteDivY; nY++)
				{
					for (int nX = 0; nX < m_nSiteDivX; nX++)
					{
						if (m_bVisionErrSkip[nX][nY] == TRUE) {
							g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &stEmptyDevice);
							g_JamRate.PushInOutJam(eJamType_Input, 1);
							m_bVisionErrSkip[nX][nY] = FALSE;
						}
					}
				}
				if (g_DMCont.m_dmEQP.GB(BDM0_MD_VIBRATION_MODE) == DEF_ON) {
					m_nStep = 1000;
				}
				else {
					m_nStep = 2000;
				}
				return CBaseSeqCmd::State_AlarmComplete();
			}
			else
			{
				double dCurPos = 0;
				pMotor->GetCurrentPos(dCurPos);
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move Safety pos Error", m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("SetTime=%.2f, z cur pos=%.2f", m_tcTimeout.GetCheckTime(), dCurPos);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + m_nUnitIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move Safety pos axis done Error [nAxisDone = %d]", m_nUnitIdx + 1, nAxisDone);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + m_nUnitIdx, AlarmData);
				}
			}
		}break;
	case 1000:
		{
			m_nStep = 2000;
		}break;
	case 2000:
		{
			return CBaseSeqCmd::State_AlarmComplete();
		}break;
	default: {
		m_nStep = 0;
	}break;
	}

	return CBaseSeqCmd::State_AlarmComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_MovePos::State_Aborting()
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
			pMotor->SetSlowStop();
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
BOOL CPressUnitCmd_MovePos::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete(); 
}
