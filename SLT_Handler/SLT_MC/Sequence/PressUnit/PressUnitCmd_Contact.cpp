#include "StdAfx.h"
#include "PressUnitCmd_Contact.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPressUnitCmd_Contact::CPressUnitCmd_Contact(void)
{
	m_nCmdId = eSeqCmd_PressUnit_Contact;
	m_nCmdStage = 0;
	m_nRetryCount = 0;
	m_nChkExist   = DEF_OFF;
	
	::ZeroMemory( m_dLastPos, sizeof( m_dLastPos ) );

	for(int i=0; i<CTaskPressUnit::eMaxAxisCount; i++){
		m_dLastPosErrRange[i] = MOTOR_INPOSITION; // [um]
		m_dInPosRange[i] = MOTOR_INPOSITION;      // [um]
	}

	m_nUnitIdx = 0;
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));

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
	m_nReContactCnt = 0;
	m_bBlowOff = FALSE;
	m_nCmdSafetyStage = LOC_IDX_PRESS_UNIT_SAFETY;
	m_nTorqueAxisNo = -1;
}


CPressUnitCmd_Contact::~CPressUnitCmd_Contact(void)
{
}


//==============================================================================
//
//==============================================================================
int CPressUnitCmd_Contact::CheckParam()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	m_nUnitIdx = pTaskPressUnit->m_nUnitIdx;

	BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();// g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);
	BOOL bSocketClean = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_SocketClean);

	if (bSocketClean == TRUE && bAutoGrrUse == FALSE) {
		BOOL bInterval = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_CleanIntervalCnt);
		int nContactCnt = g_DMCont.m_dmEQP.GN(NDM0_CleaningContact_Press1 + m_nUnitIdx);
		int nSetCleanCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_SockClnIntervalCnt);
		if (bInterval == TRUE && nSetCleanCnt != 0 && ((nContactCnt + 1) % nSetCleanCnt) == 0)
		{
			g_DMCont.m_dmEQP.SB((BDM0_SOCKETCLEAN_SITE1_RESERVE + m_nUnitIdx), TRUE);
		}
	}
// 	if (m_nUnitIdx <= ePressUnit_Down_8) {
// 		m_nTorqueAxisNo = eAXIS_PRESS_UNIT_DN1 + m_nUnitIdx;
// 	}
// // 	else {
// // 		m_nTorqueAxisNo = eAXIS_PRESS_UNIT_UP1 + (m_nUnitIdx - ePressUnit_Up_1);
// // 	}
// 
// 	eAxisNo eContactForceAxisNo = static_cast<eAxisNo>(m_nTorqueAxisNo);
// 
// 	if (g_MotorDriverComIF.IsUsableAxis(static_cast<eAxisNo>(eContactForceAxisNo))) {
// 		g_MotorDriverComIF.start_comm_running(static_cast<eAxisNo>(eContactForceAxisNo));
// 	}

	


	m_nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	m_nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	m_nWorkCmdStage = m_nCmdStage;

	m_nVibCurTryCnt = 0;

	m_nReContactCnt = 0;

	m_nCmdSafetyStage = LOC_IDX_PRESS_UNIT_SAFETY;


	return ERR_NO_ERROR;
}


//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_Contact::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_Contact::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_Contact::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_Contact::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_Contact::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_Contact::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_Contact::State_Executing()
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
		AlarmData.m_fnAddItemFormat("PressUnit %d check motor usable Cmd Error", m_nUnitIdx+1);
		AlarmData.m_fnAddItemFormat("Axis=%s ", pMotorZ->GetAxisName());
		ReportAlarm(ERR_PRESS_UNIT_1_MOTOR_ALARM + m_nUnitIdx, AlarmData);
		return FALSE;
	}

	switch( m_nStep )
	{		
	case 0: // arrived sensor [ 안착 센서 ]
		{
			g_DMCont.m_dmEQP.SB(BDM0_PRESS_CONTACT_STATUS_SITE1 + m_nUnitIdx, TRUE);    // Set Contact status on

			if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto) {
				ST_DD_DEVICE stDvc;
				for (int nY = 0; nY < STATION_MAX_Y_SITE; nY++)
				{
					for (int nX = 0; nX < STATION_MAX_X_SITE; nX++)
					{
						stDvc.clear();
						pTaskPressUnit->GetSiteDeviceData(&stDvc, nX, nY);
						if (stDvc.sExist == eDeviceData_Exist)
							g_TaskPressUnit[m_nUnitIdx].SetSiteDvcTestingInfo(nX, nY, eTestSite_ContactStart);
					}
				}
			}


			if( g_DMCont.m_dmHandlerOpt.GB(BDM11_PRS_DEVICE_ARRIVE_ENABLE) == DEF_ON )
			{
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Arrived Sensor Check](STR)",m_szCmdName ,m_nStep);
				CString strErr = _T("");
				std::vector<CPoint> vErrPocket;
				vErrPocket.clear();

				std::vector<CString> vErrHistory;
				int nDeviceType = 0;
				int nErrArrivedSen = pTaskPressUnit->ChkSocketArrivedSen_All(strErr, vErrPocket, vErrHistory, nDeviceType);
				
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
					AlarmData.m_fnAddItemFormat("Station =%d, Site =%s ", m_nUnitIdx+1, strErr);
					ReportAlarm(nErr + m_nUnitIdx, AlarmData,vErrPocket);
					break;
				}
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Arrived Sensor Check](END)",m_szCmdName ,m_nStep);
			}
			m_nStep = 1000;
		}break;
	case 1000:	// device exist check vision
		{
			int nDeviceMode     = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
 			BOOL bUseArtRule    = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_RuleAAB_VISION);	
			BOOL bVisionUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_VisionUse);

			int nExist = pTaskPressUnit->ChkSiteInDvc();
 			
			m_nStep = 2000;	// contact
		}break;

	case 2000: // move 1step // target pos - 15mm
		{
			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Z Axis Move to Contact Pos - 5mm ](STR)",m_szCmdName ,m_nStep);
			int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
		
			double dContactOffSet = 0.0;

			dContactOffSet = PRESS_MARLIN_CONTACT_DIST_OFFSET;

			int nErr = pTaskPressUnit->do_MovePos( LOC_IDX_PRESS_UNIT_CONTACT , 100, dContactOffSet);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d move z contact pos cmd Error", m_nUnitIdx+1);
				OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime( (MOTOR_TIME_OUT_MIDDLE / 3.0) );
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
	case 2001:
		{
			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Z Axis Move to Contact Pos - 5mm ](END)", m_szCmdName, m_nStep);
				m_nStep = 2500;
			}
			else {
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("PressUnit Down %d move z contact pos Error", m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + m_nUnitIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("PressUnit %d move z contact pos axis done Error [nAxisDone = %d]", m_nUnitIdx + 1, nAxisDone);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + m_nUnitIdx, AlarmData);
				}
			}
		}break;
	case 2500 :	// contact 전 안착 감지 센서 체크
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
			m_nStep = 3000;
		}break;
	case 3000: // move 2Step // Target 위치 대비 5mm 이전부터 Target까지 기존 속도 대비 10%로 움직임.
		{
			int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
			int nSpeed = 0;

			nSpeed = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_CONTACT_2ST_SPEED);
		
			if (nSpeed < PRESS_MIN_CONTACT_SPEED) // 5%보다 낮으면 최소값 5% 고정
				nSpeed = PRESS_MIN_CONTACT_SPEED;
	
			if(nSpeed > PRESS_MAX_CONTACT_SPEED) // 15%보다 높으면 최대값 15% 고정
				nSpeed = PRESS_MAX_CONTACT_SPEED;

			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Z Axis Move to Contact Pos](STR)",m_szCmdName ,m_nStep);
			int nErr = pTaskPressUnit->do_MovePos( LOC_IDX_PRESS_UNIT_CONTACT , nSpeed);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("PressUnit %d move z contact pos cmd Error", m_nUnitIdx+1);
				OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}

			m_tcTimeout.SetCheckTime( (MOTOR_TIME_OUT_MIDDLE / 3.0) );
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
	case 3001:
		{
			int nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
			int	nReContactCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_RECONTACT_CNT);			
			int nReContactOtn = (int)g_DMCont.m_dmHandlerOpt.GB(BDM11_PRS_RECONTACT_ENABLE);

			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				m_nStep = 6000;
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Z Axis Move to Contact Pos](END)", m_szCmdName, m_nStep);
			}
			else {
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("PressUnit %d move z contact pos Error", m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + m_nUnitIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("PressUnit %d move z contact pos axis done Error [nAxisDone = %d]", m_nUnitIdx + 1, nAxisDone);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
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

	}break;
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
	}break;
	case do_Vibrator_Off:
	{
		g_TaskPressUnit[m_nUnitIdx].do_SideVibratorOnOff(DEF_OFF);

		tmr.StartTmChk();
		pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [do_Vibrator_Off]", m_szCmdName, m_nStep);
		m_nStep = do_Vibrator_Off_Wait;
	}break;
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
	}break;
	case Vibrator_complete:
	{
		m_nVibCurTryCnt++;
		m_nStep = m_nBackVibratorStep;
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
BOOL CPressUnitCmd_Contact::State_Pause()
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
BOOL CPressUnitCmd_Contact::State_Recovery()
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

	OneAxis* pMotorZ = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);

	switch( m_nStep )
	{
	case 0: // Move
		{
			int nErr = pTaskPressUnit->do_MovePos( LOC_IDX_PRESS_UNIT_SAFETY );
			if( nErr != 0 ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Press Unit %d move Safety position cmd Error", m_nUnitIdx+1);
				OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
	case 1: // check move complete
		{
			OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing);
			}
			else {
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
					AlarmData.m_fnAddItemFormat("Press Unit %d move Safety position axis done Error [nAxisDone = %d]", m_nUnitIdx + 1,nAxisDone);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + m_nUnitIdx, AlarmData);
				}
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
BOOL CPressUnitCmd_Contact::State_NormalComplete()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;

	if( CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto ){
		int nContactCnt = g_DMCont.m_dmEQP.GN(NDM0_CleaningContact_Press1+m_nUnitIdx);
		g_DMCont.m_dmEQP.SN(NDM0_CleaningContact_Press1+m_nUnitIdx, ++nContactCnt);

		ST_DD_DEVICE stDvc;
		for (int nY = 0; nY < STATION_MAX_Y_SITE; nY++)
		{
			for (int nX = 0; nX < STATION_MAX_X_SITE; nX++)
			{
				stDvc.clear();
				pTaskPressUnit->GetSiteDeviceData(&stDvc, nX, nY);
				if (stDvc.sExist == eDeviceData_Exist)
					g_TaskPressUnit[m_nUnitIdx].SetSiteDvcTestingInfo(nX, nY, eTestSite_Contacting);
			}
		}
	}

	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_Contact::State_Alarm()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskPressUnit->MakeLog("[CMD: %s] [ALARM] [Step=%8d] [LastStep=%d] ", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0: // alarm 발생 했을 때 처리해 줘야 할꺼 처리 해줌
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
BOOL CPressUnitCmd_Contact::State_AlarmComplete()
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
			int nErr = pTaskPressUnit->do_MovePos( LOC_IDX_PRESS_UNIT_SAFETY );
			if( nErr != 0 ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("PressUnit %d move safety pos cmd Error", m_nUnitIdx+1);
				OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
	case 1: // check move complete
		{
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
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

				return CBaseSeqCmd::State_AlarmComplete();
			}
			else {
				double dCurPos = 0;
				pMotor->GetCurrentPos(dCurPos);

				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Press Unit %d move Safety pos Error", m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("SetTime=%.2f, z cur pos=%.2f", m_tcTimeout.GetCheckTime(), dCurPos);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + m_nUnitIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("PressUnit %d move Safety pos axis done Error [nAxisDone = %d]", m_nUnitIdx + 1, nAxisDone);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + m_nUnitIdx, AlarmData);
				}
			}
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
BOOL CPressUnitCmd_Contact::State_Aborting()
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

	OneAxis* pMotor = pTaskPressUnit->GetAxis(CTaskPressUnit::eAxisZ);

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
BOOL CPressUnitCmd_Contact::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete(); 
}
