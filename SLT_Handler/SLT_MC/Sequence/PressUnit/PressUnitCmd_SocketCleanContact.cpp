#include "StdAfx.h"
#include "PressUnitCmd_SocketCleanContact.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPressUnitCmd_SocketCleanContact::CPressUnitCmd_SocketCleanContact(void)
{
    m_nCmdId              = eSeqCmd_PressUnit_SocketCleanContact;
    m_nCmdStage           = 0;
    m_nRetryCount         = 0;
	m_nSetSocketCleanCount= 0;
    StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));

	for(int nY=0; nY < STATION_MAX_Y_SITE; nY++ )
	{
		for(int nX=0; nX < STATION_MAX_Y_SITE; nX++ )
		{
			m_bVisionErrSkip[nX][nY] = FALSE;
		}
	}
	m_nUnitIdx = -1;

	m_nSiteDivX = 0;
	m_nSiteDivY = 0;
	m_nBackVibratorStep = 0;
	m_nVibCurTryCnt = 0;
}


CPressUnitCmd_SocketCleanContact::~CPressUnitCmd_SocketCleanContact(void)
{
}


//==============================================================================
//
//==============================================================================
int CPressUnitCmd_SocketCleanContact::CheckParam()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	m_nUnitIdx = pTaskPressUnit->m_nUnitIdx;

	m_nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	m_nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	m_nSetSocketCleanCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_SockClnContactCnt);

	m_nWorkCmdStage = m_nCmdStage;

	m_nVibCurTryCnt = 0;

    return ERR_NO_ERROR;
}


//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_SocketCleanContact::OnStop()
{
    SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
    return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_SocketCleanContact::OnRetry()
{
    SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
    return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_SocketCleanContact::OnSkip()
{
    ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
    return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_SocketCleanContact::OnPause()
{
    SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
    return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_SocketCleanContact::OnResume()
{
    SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
    return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_SocketCleanContact::State_Setup()
{
    return CBaseSeqCmd::State_Setup();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_SocketCleanContact::State_Executing()
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
			m_nLast_Executing_Step = m_nStep;
			if( g_DMCont.m_dmHandlerOpt.GB(BDM11_PRS_DEVICE_ARRIVE_ENABLE) == DEF_ON )
			{
				//pTaskPressUnit->SendEvent(CEID_PRESS_UNIT_SOC_CLEAN_CONTACT_START);
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Arrived Sensor Check](STR)",m_szCmdName ,m_nStep);

				CString strErr = _T("");

				std::vector<CPoint> vErrPocket;
				vErrPocket.clear();

				std::vector<CString> vErrHistory;
				int nDeviceType = 0;
				int nErrArrivedSen = pTaskPressUnit->ChkSocketArrivedSen_All(strErr,vErrPocket, vErrHistory, nDeviceType);

				int nRetPos = pTaskPressUnit->ChkMotorPos(LOC_IDX_PRESS_UNIT_CONTACT, CTaskPressUnit::eAxisZ);
				if( nErrArrivedSen != ERR_NO_ERROR && nRetPos != ERR_NO_ERROR)
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
					
					int nErr = nDeviceType == eDeviceType::_eDeviceType_Normal ? ERR_PRESS_UNIT_1_ARRIVED_SENSOR : ERR_PRESS_UNIT_1_CLEAN_DEVICE_ARRIVED_SENSOR;

					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Station=%d, Site=%s, Action=Cleaning", m_nUnitIdx+1, strErr);
					ReportAlarm(nErr + m_nUnitIdx, AlarmData,vErrPocket);
					break;
				}
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Arrived Sensor Check](END)",m_szCmdName ,m_nStep);	
			}

            m_nStep = 500;
        }break;
	case 500:	// device exist check vision
		{
			m_nLast_Executing_Step = m_nStep;
			int nDeviceMode     = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
			BOOL bUseArtRule    = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_RuleAAB_VISION);	
			BOOL bVisionUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_VisionUse);

			int nExist = pTaskPressUnit->ChkSiteInDvc();

			m_nStep = 1000;	
		}break;
	case 1000: // check count
		{
			m_nLast_Executing_Step = m_nStep;
			m_nSetSocketCleanCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_SockClnContactCnt);
			if( m_nSetSocketCleanCount > m_nRetryCount )
			{
				m_nStep = 2000;
			}
			else
			{
				m_nStep = 5000;
			}
		}break;

	case 2000:	// move contact-5mm
		{
			int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

			double dContactOffSet = 0.0;
			char szCompany[16] = { 0, };
			g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
			BOOL nSocketCleanFunctionOption = g_DMCont.m_dmEQP.GB(BDM0_SOCKET_CLEAN_FUNCTION_OPTION);

			//dContactOffSet = PRESS_CLEAN_CONTACT_DIST_OFFSET;
			dContactOffSet = g_DMCont.m_dmHandlerOpt.GD(DDM11_PRESS_CLEANING_2ND_Z_OFFSET);

			if (dContactOffSet < PRESS_CLEAN_CONTACT_DIST_MIN_OFFSET)
				dContactOffSet = PRESS_CLEAN_CONTACT_DIST_MIN_OFFSET;

			if (dContactOffSet > PRESS_CLEAN_CONTACT_DIST_MAX_OFFSET)
				dContactOffSet = PRESS_CLEAN_CONTACT_DIST_MIN_OFFSET;
// 
// 			if (5000 > dContactOffSet && dContactOffSet > 20000) //5mm이하, 20mm 초과
// 			{
// 				dContactOffSet = 5000; //기존 5mm로 고정
// 			}
			

			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Move to Contact - %.3fmm Pos](STR)",m_szCmdName ,m_nStep, dContactOffSet/1000.0);
			int nErr = pTaskPressUnit->do_MovePos( LOC_IDX_PRESS_UNIT_CONTACT, 70, -dContactOffSet);
			if( nErr != 0 ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d move Contact - 5mm position cmd Error", m_nUnitIdx+1);
				OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}

			m_tcTimeout.SetCheckTime( (MOTOR_TIME_OUT_MIDDLE/3.0) );
			m_tcTimeout.StartTimer();
			m_nStep = 2100;
		}break;
	case 2100: // check done
		{
			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			int nAxisDone = pMotor->AxisDone();
			double dCurPos = 0;
			pMotor->GetCurrentPos(dCurPos);
			if (nAxisDone == ERR_NO_ERROR)
			{
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Move to Contact Pos] Cur Pos = [%.2f] (END)", m_szCmdName, m_nStep, dCurPos / 1000);
				m_nStep = 2200;
			}
			else
			{
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move z contact -5mm position time over!", m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + m_nUnitIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move z contact pos axis done Error", m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + m_nUnitIdx, AlarmData);
				}
			}
		}break;
	case 2200: // check arrived sensor
		{
			if(g_DMCont.m_dmHandlerOpt.GB(BDM11_PRS_DEVICE_ARRIVE_ENABLE) == DEF_ON){
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Arrived Sensor Check](STR)",m_szCmdName ,m_nStep);
				
				CString strErr = _T("");

				std::vector<CPoint> vErrPocket;
				vErrPocket.clear();

				std::vector<CString> vErrHistory;
				int nDeviceType = 0;
				int nErrArrivedSen = pTaskPressUnit->ChkSocketArrivedSen_All(strErr,vErrPocket, vErrHistory, nDeviceType);

				int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
				char szCompany[16] = { 0, };
				g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
			
				int nRetPos = pTaskPressUnit->ChkMotorPos(LOC_IDX_PRESS_UNIT_CONTACT, CTaskPressUnit::eAxisZ);
				if( nErrArrivedSen != ERR_NO_ERROR && nRetPos != ERR_NO_ERROR)
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

					int nErr = nDeviceType == eDeviceType::_eDeviceType_Normal ? ERR_PRESS_UNIT_1_ARRIVED_SENSOR : ERR_PRESS_UNIT_1_CLEAN_DEVICE_ARRIVED_SENSOR;

					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Station=%d, Site=%s, Action=Cleaning", m_nUnitIdx+1, strErr);
					ReportAlarm(nErr + m_nUnitIdx, AlarmData,vErrPocket);
					break;
				}

				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Arrived Sensor Check](END)",m_szCmdName ,m_nStep);
			}
			m_nStep = 2300;
		}break;
	case 2300:	// move contact
		{
			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Move to Contact Pos](STR)",m_szCmdName ,m_nStep);
			int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
			int nSpeed = 0;
			BOOL nSocketCleanFunctionOption = g_DMCont.m_dmEQP.GB(BDM0_SOCKET_CLEAN_FUNCTION_OPTION);

			nSpeed = PRESS_SOCKET_CLEAN_CONTACT_SPEED;

			double dCleanOffset = 0.0;
			if (g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_CLEAN_OFFSET_OPTION) == TRUE)
			{
				dCleanOffset =  g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS1_CLEAN_OFFSET + m_nUnitIdx) * 1000.0;
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Move to Contact - %.3fmm Clean Offset]", m_szCmdName, m_nStep, dCleanOffset / 1000.0);
			}

			int nErr = pTaskPressUnit->do_MovePos( LOC_IDX_PRESS_UNIT_CONTACT, nSpeed, dCleanOffset);
			if( nErr != 0 ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d move contact position cmd Error", m_nUnitIdx+1);
				OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}

			m_tcTimeout.SetCheckTime( (MOTOR_TIME_OUT_MIDDLE/3.0) );
			m_tcTimeout.StartTimer();
			m_nStep = 2400;
		}break;
	case 2400: // check done
		{
			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			int nAxisDone = pMotor->AxisDone();
			double dCurPos = 0;
			pMotor->GetCurrentPos(dCurPos);
			if (nAxisDone == ERR_NO_ERROR)
			{
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Move to Contact position ] Cur Pos = [%.2f] (END)", m_szCmdName, m_nStep, dCurPos / 1000);
				m_nStep = 2500;
			}
			else
			{
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move z contact position time over!", m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + m_nUnitIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move z contact pos axis done Error", m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + m_nUnitIdx, AlarmData);
				}
			}
		}break;
	case 2500:
		{
			char szCompany[16] = { 0, };
			g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
						
			//blow on
			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Blow On](STR)", m_szCmdName, m_nStep);
			int nErrVac = pTaskPressUnit->do_VacBlow(eVacBlwSt_BlwOn);
			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Blow On](END)", m_szCmdName, m_nStep);

			int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

			double dContactOffSet = 0.0;
			dContactOffSet = g_DMCont.m_dmHandlerOpt.GD(DDM11_PRESS_CLEANING_2ND_Z_OFFSET);

			// Min, Max값 초과하면 5mm로 고정
			if (dContactOffSet < PRESS_CLEAN_CONTACT_DIST_MIN_OFFSET || dContactOffSet > PRESS_CLEAN_CONTACT_DIST_MAX_OFFSET) //5mm이하, 20mm 초과
			{
				dContactOffSet = PRESS_CLEAN_CONTACT_DIST_MIN_OFFSET; //기존 5mm로 고정
			}

			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Move to Contact - %.3fmm Pos](STR)",m_szCmdName ,m_nStep, dContactOffSet/1000.0);
			int nErr = pTaskPressUnit->do_MovePos( LOC_IDX_PRESS_UNIT_CONTACT, 70, -dContactOffSet);
			if( nErr != 0 ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d move Contact - 5mm position cmd Error", m_nUnitIdx+1);
				OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}

			m_tcTimeout.SetCheckTime( (MOTOR_TIME_OUT_MIDDLE/3.0) );
			m_tcTimeout.StartTimer();
			m_nStep = 2600;
		}break;
	case 2600:
		{
			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			int nAxisDone = pMotor->AxisDone();
			double dCurPos = 0;
			pMotor->GetCurrentPos(dCurPos);
			if (nAxisDone == ERR_NO_ERROR)
			{
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Move to Contact Pos, act=%d ] Cur Pos = [%.2f] (END)", m_szCmdName, m_nStep, m_nRetryCount + 1, dCurPos / 1000);
				m_nRetryCount++;

				//blow on
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Blow Off](STR)", m_szCmdName, m_nStep);
				int nErrVac = pTaskPressUnit->do_VacBlow(eVacBlwSt_Off);
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Blow Off](END)", m_szCmdName, m_nStep);

				m_nStep = 1000;
			}
			else
			{
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move z contact -5mm position time over!", m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + m_nUnitIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move z contact pos axis done Error", m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + m_nUnitIdx, AlarmData);
				}
			}
		}break;
	case 5000: // Contact Pos Blow Pos Move
	{
		BOOL nSocketCleanFunctionOption = g_DMCont.m_dmEQP.GB(BDM0_SOCKET_CLEAN_FUNCTION_OPTION);
		if (nSocketCleanFunctionOption == DEF_ON)
		{
			m_nStep++; //Blow Pos, Blow Time
		}
		else
		{
			m_nStep = 6000; //Safety
		}		
	}break;
	case 5001: // Contact Pos Blow Pos Move
	{
		pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Move to Socket Clean Contact Blow Pos](STR)", m_szCmdName, m_nStep);
		int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

		double dSocketCleanBlowPos = g_DMCont.m_dmHandlerOpt.GD(DDM11_PRESS_SOCKET_CLEAN_BLOW_POS) * -1000;

		int nErr = pTaskPressUnit->do_MovePos(LOC_IDX_PRESS_UNIT_CONTACT, 100, dSocketCleanBlowPos);
		if (nErr != 0) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Press Unit %d move contact position cmd Error", m_nUnitIdx + 1);
			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
			ReportAlarm(nErr, AlarmData);
			break;
		}

		m_tcTimeout.SetCheckTime((MOTOR_TIME_OUT_MIDDLE / 3.0));
		m_tcTimeout.StartTimer();
		m_nStep++;
	}break;
	case 5002: // check done
	{
		OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
		int nAxisDone = pMotor->AxisDone();
		double dCurPos = 0;
		pMotor->GetCurrentPos(dCurPos);
		if (nAxisDone == ERR_NO_ERROR)
		{
			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Move to Socket Clean Contact Blow Pos] Cur Pos=[%.2f] (END)", m_szCmdName, m_nStep, dCurPos / 1000.0);
			tmr.StartTmChk();
			m_nStep = 5100;
		}
		else
		{
			if (m_tcTimeout.CheckOverTime() == TRUE)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d move z contact position time over!", m_nUnitIdx + 1);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + m_nUnitIdx, AlarmData);
				break;
			}

			if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d move z contact pos axis done Error", m_nUnitIdx + 1);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + m_nUnitIdx, AlarmData);
			}
		}
	}break;
	case 5100: // Blow 
	{
		int nErrVac = pTaskPressUnit->do_VacBlow(eVacBlwSt_BlwOn);
		if (nErrVac != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Press Unit %d  blow off command error", m_nUnitIdx + 1);
			ReportAlarm(nErrVac, AlarmData);
			break;
		}		
		tmr.StartTmChk();
		pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Socket Clean Blow Press Wait][STR]", m_szCmdName, m_nStep);
		m_nStep++;
	}break;
	case 5101: // Blow Wait Time
	{
		double dSocketCleanBlowTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_PRESS_SOCKET_CLEAN_BLOW_TIME);

		tmr.EndTmChk();
		if (tmr.GetTmDistanceMS() < dSocketCleanBlowTime * 1000)
			break;

		int nErrVac = pTaskPressUnit->do_VacBlow(eVacBlwSt_Off);
		if (nErrVac != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Press Unit %d  blow off command error", m_nUnitIdx + 1);
			ReportAlarm(nErrVac, AlarmData);
			break;
		}

		pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Socket Clean Blow Press Wait][END]", m_szCmdName, m_nStep);
		m_nStep = 6000;

	}break;

	case 6000: // move safety
		{
			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Move to Safety Pos](STR)",m_szCmdName ,m_nStep);
			int nErr = pTaskPressUnit->do_MovePos( LOC_IDX_PRESS_UNIT_SAFETY );
			if( nErr != 0 ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d move Safety pos cmd Error", m_nUnitIdx+1);
				OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime( (MOTOR_TIME_OUT_MIDDLE/3.0) );
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
	case 6001: // axis done & Normal comp
		{
			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR)
			{
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Move to Safety Pos](END)", m_szCmdName, m_nStep);
				ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
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
					AlarmData.m_fnAddItemFormat("Press Unit %d move Safety pos axis done Error", m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + m_nUnitIdx, AlarmData);
				}
			}
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
    }
    return CBaseSeqCmd::State_Executing();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_SocketCleanContact::State_Pause()
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
			//blow on
			pTaskPressUnit->MakeLog("[%s] [PAUSE=%8d] [Blow Off](STR)",m_szCmdName ,m_nStep);
			int nErrVac = pTaskPressUnit->do_VacBlow(eVacBlwSt_Off);
			pTaskPressUnit->MakeLog("[%s] [PAUSE=%8d] [Blow Off](END)",m_szCmdName ,m_nStep);

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
BOOL CPressUnitCmd_SocketCleanContact::State_Recovery()
{
    CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
    if( m_nStep_New != -1 ){
        m_nStep = m_nStep_New;
        m_nStep_New = -1;
    }
    if( m_nStep_Pre != m_nStep ){
        //pTaskPressUnit->MakeLog("[CMD: %s] [RECOVERY] [Step=%8d]", m_szCmdName, m_nStep);
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
    case 0: // 현재 위치와 마지막 위치를 비교 한다.
        {
			//blow on
			pTaskPressUnit->MakeLog("[%s] [RECOVERY=%8d] [LastStep=%d] [Blow On] (STR)",m_szCmdName ,m_nStep, m_nLast_Executing_Step);
			pTaskPressUnit->do_VacBlow(eVacBlwSt_BlwOn);
			pTaskPressUnit->MakeLog("[%s] [RECOVERY=%8d] [LastStep=%d] [Blow On](END)",m_szCmdName ,m_nStep, m_nLast_Executing_Step);
			m_nStep = 1000;
        }break;
	case 1000: // move safety
		{
			pTaskPressUnit->MakeLog("[%s] [RECOVERY=%8d] [LastStep=%d] [Move to Safety Pos](STR)",m_szCmdName ,m_nStep, m_nLast_Executing_Step);
			int nErr = pTaskPressUnit->do_MovePos( LOC_IDX_PRESS_UNIT_SAFETY );
			if( nErr != 0 ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d move Safety pos cmd Error", m_nUnitIdx+1);
				OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime( (MOTOR_TIME_OUT_MIDDLE/3.0) );
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
	case 1001: // axis done & Normal comp
		{
			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR)
			{
				pTaskPressUnit->MakeLog("[%s] [RECOVERY=%8d] [LastStep=%d] [Move to Safety Pos](END)", m_szCmdName, m_nStep, m_nLast_Executing_Step);
				pTaskPressUnit->do_VacBlow(eVacBlwSt_Off);
				m_nStep = 2000;
			}
			else
			{
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move Safety pos Error", m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + m_nUnitIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move Safety pos axis done Error", m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + m_nUnitIdx, AlarmData);
				}
			}
		}break;
	case 2000:
		{			
			pTaskPressUnit->MakeLog("[%s] [RECOVERY=%8d] [LastStep=%d]",m_szCmdName ,m_nStep, m_nLast_Executing_Step);
			if( m_nLast_Executing_Step >= 1000){
				SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing, 1000);
			}else{
				SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing);
			}
			
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
BOOL CPressUnitCmd_SocketCleanContact::State_NormalComplete()
{
    CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
    //pTaskPressUnit->SendEvent(CEID_PRESS_UNIT_SOC_CLEAN_CONTACT_END);

	if( CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto ){
		ST_DD_DEVICE stDeviceData;
		for(int nY=0; nY < m_nSiteDivY; nY++ )
		{
			for(int nX=0; nX < m_nSiteDivX; nX++ )
			{
				stDeviceData.clear();
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1+m_nUnitIdx, nX, nY, &stDeviceData);

				if( stDeviceData.sExist == eDeviceData_Exist )
				{
					stDeviceData.nTestingInfo = eTestSite_CleanEnd;
					stDeviceData.nRealTestingCnt += m_nSetSocketCleanCount;
					g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_SITE_1+m_nUnitIdx, nX, nY, &stDeviceData);
				}
			}
		}
	}

	//blow off
	pTaskPressUnit->do_VacBlow(eVacBlwSt_Off);

    return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_SocketCleanContact::State_Alarm()
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
			//blow off
			int nErrVac = pTaskPressUnit->do_VacBlow(eVacBlwSt_Off);
			if(nErrVac != ERR_NO_ERROR){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d blow off command error", m_nUnitIdx+1 );
				ReportAlarm(nErrVac, AlarmData);
				break;
			}

            OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
            //pMotor->SetStop();
			bool bCloseLimit = pMotor->CheckLimitClose();
			double dCurPos = 0;
			pMotor->GetCurrentPos(dCurPos);
			if (bCloseLimit == false)
			{
				pTaskPressUnit->MakeLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
				pMotor->SetSlowStop();
			}
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
BOOL CPressUnitCmd_SocketCleanContact::State_AlarmComplete()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	
	ST_DD_DEVICE stEmptyDevice; stEmptyDevice.clear();
	CString strErrSkip = _T("");
	for(int nY=0; nY < m_nSiteDivY; nY++ )
	{
		for(int nX=0; nX < m_nSiteDivX; nX++ )
		{
			if(m_bVisionErrSkip[nX][nY] == TRUE){				//Vision NG Error				
				g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_SITE_1+m_nUnitIdx, nX, nY, &stEmptyDevice);
				m_bVisionErrSkip[nX][nY] = FALSE;
				CString strSkipSite = _T("");

				strSkipSite.Format("[%d-%d]", nX+1, nY+1);
				strErrSkip += strSkipSite;
			}
		}
	}

	if( strErrSkip.IsEmpty() != TRUE )
	pTaskPressUnit->MakeLog("[CMD: %s] [ALARM COMP] [Vision NG Error Skip Site = %s]", m_szCmdName, strErrSkip);

    return CBaseSeqCmd::State_AlarmComplete(); 
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_SocketCleanContact::State_Aborting()
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
BOOL CPressUnitCmd_SocketCleanContact::State_AbortComplete()
{
    return CBaseSeqCmd::State_AbortComplete(); 
}
