#include "StdAfx.h"
#include "TestPPCmd_MovePickPlace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTestPPCmd_MovePickPlace::CTestPPCmd_MovePickPlace(void)
{
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));

    m_nCmdId          = eSeqCmd_TestPP_MovePickPlace;
    m_nCmdStage       = 0;
    m_nRetryCount     = 0;
	m_nPickRetry = 0;

	for(int i=0; i<CTaskTestPP::eAxisMax; i++){
		m_dLastPosErrRange[i] = MOTOR_INPOSITION; // [um]
		m_dInPosRange[i]      = MOTOR_INPOSITION; // [um]
		m_pMotor[i]           = NULL;
		m_dLastPos[i] = 0;
	}
	m_nHandType = 0;
	m_nCmdPick     = DEF_PICK;
	m_nMaxRetryCnt = 0;	
	m_nTestPpIdx   = 0;
	m_nPressIdx = 0;

	m_nVacuumWait  = 0;
	m_nBlowWait    = 0;

	m_vCmdPicker.clear();
	m_strWorkPkrName.Empty();
	m_strStageName.Empty();
	m_strPickPlace.Empty();
	m_BackAlarmData.m_fnClear();

	m_nLast_Executing_Step = 0;

	m_nBackVibratorStep = 0;

	ZeroMemory(m_bVisionErrSkip, sizeof(m_bVisionErrSkip));

	m_tcTimeout = 0;
	m_tcTimeoutPicker = 0;

	m_cCmdManualPickPlace = DEF_UNK;

	m_nVibCurTryCnt = 0;
	m_bVib_Retry_Complete = 0;

	m_bCleanOffsetUse = eCleanOffsetNotUse;
	m_bAlreadyPick = FALSE;
	//m_nCmdHandType = eTestPPLoadType;
}


CTestPPCmd_MovePickPlace::~CTestPPCmd_MovePickPlace(void)
{
}

//==============================================================================
//
//==============================================================================
int CTestPPCmd_MovePickPlace::CheckParam()
{
	CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
	m_nTestPpIdx = pTaskTestPP->m_nTestPPIdx;

	m_nPressIdx = pTaskTestPP->GetPressNum(m_nCmdStage);
	m_nMaxRetryCnt = g_DMCont.m_dmTestPP.GN(NDM4_TestPP1_RetryCnt + m_nTestPpIdx);
	m_nVacuumWait = g_DMCont.m_dmTestPP.GN(NDM4_TM_TestPP1_Vacuum + m_nTestPpIdx * 2);
	if (m_nVacuumWait <= 0)
		m_nVacuumWait = EJECTOR_CHANGE_MIN_TIME;
	//m_nBlowWait       = g_DMCont.m_dmTestPP.GN(NDM4_TM_TestPP1_Blow        + m_nTestPpIdx*2);
	//if( m_nBlowWait <= 0 )
	//	m_nBlowWait = EJECTOR_CHANGE_MIN_TIME;

	m_pMotor[CTaskTestPP::eAxisX1] = pTaskTestPP->GetAxis(CTaskTestPP::eAxisX1);
	m_pMotor[CTaskTestPP::eAxisY] = pTaskTestPP->GetAxis(CTaskTestPP::eAxisY);
	m_pMotor[CTaskTestPP::eAxisX2] = pTaskTestPP->GetAxis(CTaskTestPP::eAxisX2);
	m_pMotor[CTaskTestPP::eAxisZ1] = pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ1);
	m_pMotor[CTaskTestPP::eAxisZ2] = pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ2);
	m_pMotor[CTaskTestPP::eAxisPitch_HW_X1_SW_Y1] = pTaskTestPP->GetAxis(CTaskTestPP::eAxisPitch_HW_X1_SW_Y1);
	m_pMotor[CTaskTestPP::eAxisPitch_HW_Y1_SW_X1] = pTaskTestPP->GetAxis(CTaskTestPP::eAxisPitch_HW_Y1_SW_X1);
	m_pMotor[CTaskTestPP::eAxisPitch_HW_X2_SW_Y2] = pTaskTestPP->GetAxis(CTaskTestPP::eAxisPitch_HW_X2_SW_Y2);
	m_pMotor[CTaskTestPP::eAxisPitch_HW_Y2_SW_X2] = pTaskTestPP->GetAxis(CTaskTestPP::eAxisPitch_HW_Y2_SW_X2);

	for (int i = 0; i < CTaskTestPP::eAxisMax; i++)
	{
		m_pMotor[i]->GetCurrentPos(m_dLastPos[i]);
	}

	m_strWorkPkrName = pTaskTestPP->GetPickerName(m_vCmdPicker);
	m_strStageName = pTaskTestPP->GetLocName(m_nCmdStage);
	m_strPickPlace = (m_nCmdPick == DEF_PICK ? _T("Pick") : _T("Place"));

	m_nWorkCmdStage = m_nCmdStage;

	//pTaskTestPP->VisionLED_ONOFF(DEF_ON, m_nTestPpIdx);

	if ((m_nCmdStage >= LOC_IDX_STATION_1 && m_nCmdStage <= LOC_IDX_STATION_4) && m_nHandType == eTestPPLoadType)
	{
		if (m_nCmdPick == DEF_PICK)
		{
			int nPocketCount_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
			int nPocketCount_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
			for (int nY = 0; nY < nPocketCount_Y; nY++)
			{
				for (int nX = 0; nX < nPocketCount_X; nX++)
				{
					ST_DD_DEVICE stDvc;
					g_TaskPressUnit[m_nPressIdx].GetSiteDeviceData(&stDvc, nX, nY);
					if (stDvc.sExist == DEF_EXIST && strcmp(stDvc.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0 && CBaseTaskManager::m_bEqpControlMode != eEqpCtrlMode_Manual)
					{
						m_bCleanOffsetUse = eCleanOffsetUse;
						break;
					}
				}
			}
		}
		else 
		{
			for (int i = 0; i < (int)m_vCmdPicker.size(); i++)
			{
				ST_DD_DEVICE stTargetData;
				pTaskTestPP->GetHandDvcData(m_vCmdPicker[i], eTestPPLoadType, &stTargetData);
				if (stTargetData.sExist == DEF_EXIST && strcmp(stTargetData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0 && CBaseTaskManager::m_bEqpControlMode != eEqpCtrlMode_Manual)
				{
					m_bCleanOffsetUse = eCleanOffsetUse;
					break;
				}
			}	
		}
	}

	pTaskTestPP->m_bCalBasePoint = FALSE;
	int nIdx = pTaskTestPP->FromTestPPToLoadTableIdx(m_nCmdStage);
	g_TaskLoadTbl[nIdx].SetTableColStep(0); // table step 0


	pTaskTestPP->m_nTestPPType = m_nHandType;

	return ERR_NO_ERROR;
}


//==============================================================================
//
//==============================================================================
int  CTestPPCmd_MovePickPlace::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTestPPCmd_MovePickPlace::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTestPPCmd_MovePickPlace::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTestPPCmd_MovePickPlace::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTestPPCmd_MovePickPlace::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_MovePickPlace::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_MovePickPlace::State_Executing()
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

	for(int i=0; i<CTaskTestPP::eAxisMax; i++){
		if(m_pMotor[i]->ChkMotorUsable() != ERR_NO_ERROR ){
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Test PP %d check motor usable Cmd Error", m_nTestPpIdx+1);
			AlarmData.m_fnAddItemFormat("Axis=%s", m_pMotor[i]->GetAxisName());
			ReportAlarm(ERR_TEST_PP_1_MOTOR_ALARMED + m_nTestPpIdx, AlarmData);
			return FALSE;
		}
	}

	switch (m_nStep)
	{
	case 0:
	{
		if (m_nCmdStage == LOC_IDX_CLEAN_TBL && CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto)
		{
			int nClnContact = g_DMCont.m_dmHandlerOpt.GN(NDM11_SockClnContactCnt);
			int nAlramCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_SockClnAlarmCnt);
			BOOL bEventType = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_CleanDev_Event_Type);

			ST_DD_DEVICE stDeviceData;
			CParamList AlarmData;
			BOOL bFindAlarmCountOver = FALSE;
			std::vector<CPoint> vCleanTablePocket;
			vCleanTablePocket.clear();
			CString strErr = _T("");
			std::vector<CString> vAlarmData;
			if (m_nCmdPick == DEF_PICK)
			{
				int nClnBuff_X = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_X);
				int nClnBuff_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_Y);

				for (int nY = 0; nY < nClnBuff_Y; nY++)
				{
					for (int nX = 0; nX < nClnBuff_X; nX++)
					{
						g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1_CLEAN_BUFF + m_nTestPpIdx, nX, nY, &stDeviceData);

						if (stDeviceData.sExist == eDeviceData_Exist && nAlramCnt < stDeviceData.nRealTestingCnt /*+ nClnContact*/) {
							CString strMsg = _T("");
							bFindAlarmCountOver = TRUE;
							vCleanTablePocket.push_back(CPoint(nX, nY));
							strMsg.Format("Current Count: %d, Set Clean Device Life: %d, x=%d, y=%d", stDeviceData.nRealTestingCnt, nAlramCnt, nX + 1, nY + 1);
							vAlarmData.push_back(strMsg);
						}
					}
				}
			}
			//else {
			//	for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
			//	{
			//		g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1_CLEAN_BUFF + m_nTestPpIdx, nX, TEST_UD_PKR_Y, &stDeviceData);
			//
			//		if (stDeviceData.sExist == eDeviceData_Exist && nAlramCnt < stDeviceData.nRealTestingCnt + nClnContact) {
			//			bFindAlarmCountOver = TRUE;
			//			AlarmData.m_fnAddItemFormat("Current Count: %d, Set Clean Device Life: %d, x=%d, y=%d", stDeviceData.nRealTestingCnt, nAlramCnt, nX + 1, TEST_UD_PKR_Y+1);
			//		}
			//	}
			//}

			if (bFindAlarmCountOver == TRUE)
			{
				AlarmData.m_fnAddItemFormat("Clean Table #%d Clean Device Life is over", m_nTestPpIdx + 1);
				for (int i = 0; i < vAlarmData.size(); i++)
				{
					AlarmData.m_fnAddItemFormat("%s", vAlarmData[i]);
				}
				g_Error.AlarmReport(ERR_SOCKET_CLEAN_TABLE_1_DEVICE_LIFE_OVER + m_nTestPpIdx, AlarmData, &g_TaskSystemCtrl, vCleanTablePocket);
				if (bEventType == 0)
				{
					g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
					//ReportAlarm(ERR_SOCKET_CLEAN_DEVICE_LIFE_OVER, AlarmData);
					break;
				}
				else {
					//g_Error.AlarmReport(ERR_SOCKET_CLEAN_DEVICE_LIFE_OVER, AlarmData, &g_TaskSystemCtrl);
				}
			}
		}


		if (m_nCmdStage >= LOC_IDX_STATION_1 && m_nCmdStage <= LOC_IDX_STATION_4 && CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Manual)
		{
			int nPressIdx = pTaskTestPP->GetPressNum(m_nCmdStage);

			if (g_TaskPressUnit[nPressIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d x axis target position command,but this was wrong [CmdStage=%s]", m_nTestPpIdx + 1, m_strStageName);
				ReportAlarm(ERR_TEST_PP_1_MOVE_TEST_SITE_INTERLOCK + m_nTestPpIdx, AlarmData);
				break;
			}
		}


		// Vision Ready On
		BOOL bVisionUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_VisionUse);
		int  nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);

		if (m_nCmdStage >= LOC_IDX_STATION_1 && m_nCmdStage <= LOC_IDX_STATION_4)
		{
			if (bVisionUseMode == DEF_ON /*&& nDeviceMode == eDeviceMode*/)
			{
				if (m_nCmdPick == DEF_PICK && m_nHandType == eTestPPUnLoadType)
				{ // DEF_PLACE //vision Empty						
					if (m_nPressIdx < eMaxPressUnitCount / 2)
						g_pVisionTestHand_1->Cmd_VisionReady_StationCam(eCamStateRun_LightON, m_nPressIdx + 1);
					else
						g_pVisionTestHand_2->Cmd_VisionReady_StationCam(eCamStateRun_LightON, m_nPressIdx + 1);
				}
			}
		}



		double dErr[CTaskTestPP::eAxisMax] = { 0, };
		double dTPos[CTaskTestPP::eAxisMax] = { 0, };
		pTaskTestPP->GetTeachPos(m_nCmdStage, m_cCmdPocket, m_vCmdPicker, CTaskTestPP::eZPOS_SAFETY, dTPos, m_nHandType, m_bCleanOffsetUse);

		// Teaching 위치와 현재 위치가 일치 하는지 확인.
		double dCurPos[CTaskTestPP::eAxisMax] = { 0, };

		for (int i = 0; i < CTaskTestPP::eAxisMax; i++)
		{
			m_pMotor[i]->GetCurrentPos(dCurPos[i]);
		}

		dErr[CTaskTestPP::eAxisX1] = fabs(dCurPos[CTaskTestPP::eAxisX1] - dTPos[CTaskTestPP::eAxisX1]);
		dErr[CTaskTestPP::eAxisZ1] = fabs(dCurPos[CTaskTestPP::eAxisZ1] - dTPos[CTaskTestPP::eAxisZ1]);
		dErr[CTaskTestPP::eAxisPitch_HW_X1_SW_Y1] = fabs(dCurPos[CTaskTestPP::eAxisPitch_HW_X1_SW_Y1] - dTPos[CTaskTestPP::eAxisPitch_HW_X1_SW_Y1]);
		dErr[CTaskTestPP::eAxisPitch_HW_Y1_SW_X1] = fabs(dCurPos[CTaskTestPP::eAxisPitch_HW_Y1_SW_X1] - dTPos[CTaskTestPP::eAxisPitch_HW_Y1_SW_X1]);

		dErr[CTaskTestPP::eAxisY] = fabs(dCurPos[CTaskTestPP::eAxisY] - dTPos[CTaskTestPP::eAxisY]);

		dErr[CTaskTestPP::eAxisX2] = fabs(dCurPos[CTaskTestPP::eAxisX2] - dTPos[CTaskTestPP::eAxisX2]);
		dErr[CTaskTestPP::eAxisZ2] = fabs(dCurPos[CTaskTestPP::eAxisZ2] - dTPos[CTaskTestPP::eAxisZ2]);
		dErr[CTaskTestPP::eAxisPitch_HW_X2_SW_Y2] = fabs(dCurPos[CTaskTestPP::eAxisPitch_HW_X2_SW_Y2] - dTPos[CTaskTestPP::eAxisPitch_HW_X2_SW_Y2]);
		dErr[CTaskTestPP::eAxisPitch_HW_Y2_SW_X2] = fabs(dCurPos[CTaskTestPP::eAxisPitch_HW_Y2_SW_X2] - dTPos[CTaskTestPP::eAxisPitch_HW_Y2_SW_X2]);



		int nChkTeachSame = 0;
		for (int i = 0; i < CTaskTestPP::eAxisMax; i++)
		{
			if (dErr[i] <= m_dInPosRange[i])
				nChkTeachSame++;
		}

		// 축 이동이 완료 되어진 상태 이므로 motor 동작이 필요 없음.
		if (nChkTeachSame == CTaskTestPP::eAxisMax) {
			m_nStep = 10000;
			m_nLast_Executing_Step = m_nStep;
		}
		else {
			m_tcTimeout = (double)GetTickCount64();
			m_nStep++;
			m_nRetryCount = 0;
		}
	}break;
	case 1:
	{
		if (m_nCmdStage >= LOC_IDX_STATION_1 && m_nCmdStage <= LOC_IDX_STATION_4)
		{
			int nPressIdx = pTaskTestPP->GetPressNum(m_nCmdStage);
			double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
			std::vector<int> vLocList;
			vLocList.clear();
			g_TaskPressUnit[nPressIdx].WhereIsMotorPos(vLocList, CTaskPressUnit::eAxisZ);

			if (g_TaskPressUnit[nPressIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready ||
				CTaskPressUnit::eMotPos_Safety < vLocList[CTaskPressUnit::eAxisZ])
			{
				if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d X1,X2 axis target position command, Press Unit Cmd Time Out [CmdStage=%s]", m_nTestPpIdx + 1, m_strStageName);
					ReportAlarm(ERR_TEST_PP_1_MOVE_TEST_SITE_INTERLOCK + m_nTestPpIdx, AlarmData);
					break;
				}
				break;
			}

		}

		m_nStep = 500;
	}break;
	case 500:
	{
		pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z1,Z2 Axis Move to Safety Pos Check]", m_szCmdName, m_nStep);

		OneAxis* pMotorZ1 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ1);
		OneAxis* pMotorZ2 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ2);

		OneAxis* pMotor[2] = { pMotorZ1 ,pMotorZ2 };
		int nComp = 0;
		for (int i = 0; i < _countof(pMotor); i++)
		{
			std::vector<CPoint> vPkr;
			m_vErrPicker.clear();
			pTaskTestPP->MakeAllPicker(vPkr);
			int nErrSen = pTaskTestPP->Chk_Z_Safety_Sen_All(i);
			int nAxisDone = pMotor[i]->AxisDone(vPkr, m_vErrPicker);
			if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR) {
				nComp++;
			}
		}

		if (nComp == _countof(pMotor))
		{
			m_nStep = 2000; // Safety 위치가 맞으면 X축 이동 하러 간다.
			break;
		}
		else {
			m_nStep = 1000; // Safety 위치가 아니면 Safety Move로 이동
			break;
		}

	}break;
	case 1000:  // all Z1,Z2 Picker Safety move
	{
		int nAxisNo[2] = { CTaskTestPP::eAxisZ1 ,CTaskTestPP::eAxisZ2 };
		pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z1,Z2 move %s safety pos](STR)", m_szCmdName, m_nStep, m_strStageName);
		std::vector<CPoint> vPkr;
		pTaskTestPP->MakeAllPicker(vPkr);

		for (int i = 0; i < _countof(nAxisNo); i++)
		{
			int nErr = pTaskTestPP->do_MovePos(eLocIdxTestPP::LOC_IDX_TEST_PP_HOME, i, m_cCmdPocket, vPkr, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i]);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d all picker up command, but this was wrong ", m_nTestPpIdx + 1);
				OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(ERR_TEST_PP_1_Z1_PICKER_CMD_INTERLOCK + pTaskTestPP->GetHandTypeIndex(m_nHandType), AlarmData);
				break;
			}
		}
		m_tcTimeout = (double)GetTickCount64();
		m_nStep++;
		m_nStep = 1001;
	}break;
	case 1001:  // check all Z1,Z2 Picker Safety
	{
		OneAxis* pMotorZ1 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ1);
		OneAxis* pMotorZ2 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ2);

		OneAxis* pMotor[2] = { pMotorZ1 ,pMotorZ2 };
		int nComp = 0;
		for (int i = 0; i < _countof(pMotor); i++)
		{
			std::vector<CPoint> vPkr;
			m_vErrPicker.clear();
			pTaskTestPP->MakeAllPicker(vPkr);
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

				double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
				if (dSpanTime > TIME_OUT_SHORT*1000.0)
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
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z%d Axis Move to Safety Pos](END)", m_szCmdName, m_nStep, m_nHandType + 1);
			m_nStep = 2000;
		}

	}break;
	case 2000: //  X1,X2 home safety
	{
		//  [12/26/2023 dohyeong.kim] 
		//Grr Mode & Picker 1ea이면 Swap 진행 중.
		//Swap 할 경우, X Safety로 안 보낸다.
		BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
		if (bAutoGrrUse == TRUE && m_vCmdPicker.size() == 1/* && m_vCmdPicker[0].x != 0 && m_vCmdPicker[0].y != 1*/)
		{
			if (m_vCmdPicker[0].x == 0 && m_vCmdPicker[0].y == 1) {
			} //해당 Picker만 X Safety Move 하도록
			else {
				m_tcTimeout = (double)GetTickCount64();
				m_nStep = 4000;
				break;
			}
		}

		pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X1,X2 Axis Move to Safety Pos Check]", m_szCmdName, m_nStep);
		std::vector<CPoint> vTempPkr;
		vTempPkr.push_back(CPoint(0, 0));

		// 			m_pMotor[CTaskTestPP::eAxisX1]->GetCurrentPos(dCurPosX1);
		// 			m_pMotor[CTaskTestPP::eAxisX2]->GetCurrentPos(dCurPosX2);



		// 			int nSafetySenX1 = pTaskTestPP->ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON);
		// 			int nSafetySenX2 = pTaskTestPP->ChkDI(CTaskTestPP::xX2AXIS_POSITION_SEN, DEF_ON);

		double dCurPosX1 = 0, dCurPosX2 = 0;
		m_pMotor[CTaskTestPP::eAxisX1]->GetCurrentPos(dCurPosX1);
		m_pMotor[CTaskTestPP::eAxisX2]->GetCurrentPos(dCurPosX2);

		double dTPosHome[CTaskTestPP::eAxisMax] = { 0, };
		pTaskTestPP->GetTeachPos(LOC_IDX_TEST_PP_HOME, CPoint(0, 0), vTempPkr, CTaskTestPP::eZPOS_SAFETY, dTPosHome, m_nHandType, m_bCleanOffsetUse);

		//int nSafetySenX = (m_nHandType == eTestPPLoadType) ? pTaskTestPP->ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON) : pTaskTestPP->ChkDI(CTaskTestPP::xX2AXIS_POSITION_SEN, DEF_ON);
		int dCurPosX = (m_nHandType == eTestPPLoadType) ? dCurPosX1 : dCurPosX2;
		int nAxis = (m_nHandType == eTestPPLoadType) ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2;
		int nSafetySenX1 = pTaskTestPP->ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON);
		int nSafetySenX2 = pTaskTestPP->ChkDI(CTaskTestPP::xX2AXIS_POSITION_SEN, DEF_ON);

		//BOOL bSafetyPos = pTaskTestPP->CompareXData(dCurPosX, dTPosHome[nAxis] + 1000.0);
		BOOL bStationAreaX1 = pTaskTestPP->IsHandStationArea(dCurPosX1);
		BOOL bStationAreaX2 = pTaskTestPP->IsHandStationArea(dCurPosX2);
	

		// home 위치보다 크거나 안전 센서가 감지 안되면	 X1,X2축 Home
		if (bStationAreaX1 == TRUE || bStationAreaX2 == TRUE || nSafetySenX1 != ERR_NO_ERROR || nSafetySenX2 != ERR_NO_ERROR)
			/*dTPosHome[CTaskTestPP::eAxisX2] + 1000.0 < dCurPosX2 || *//*nSafetySenX2 != ERR_NO_ERROR*/
		{
			m_tcTimeout = (double)GetTickCount64();
			m_nStep = 3000;
		}
		// 그 외에 경우에는 그냥 움직이면 된다.
		else {
			m_tcTimeout = (double)GetTickCount64();
			//m_nStep = 4000;
			m_nStep = 3100; //Loading Table Step Move Check.
		}
	}break;
	case 3000:  //X1, X2 move Home
	{
		if (m_nCmdStage >= LOC_IDX_STATION_1 && m_nCmdStage <= LOC_IDX_STATION_4)
		{
			int nPressIdx = pTaskTestPP->GetPressNum(m_nCmdStage);

			double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
			if (dSpanTime > MOTOR_TIME_OUT_SHORT*1000.0) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d Y Axis can not move station [WorkStage=%s not safety]", m_nTestPpIdx + 1, m_strStageName);
				ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + nPressIdx, AlarmData);
				break;
			}

			if (g_TaskPressUnit[nPressIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
				break;
		}

		int nAxisNo[2] = { CTaskTestPP::eAxisX1 ,CTaskTestPP::eAxisX2 };
		std::vector<CPoint> vPicker;
		vPicker.push_back(CPoint(0, 0));
		pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X1,X2 Axis Move to Home Pos](STR)", m_szCmdName, m_nStep);
		for (int i = 0; i < _countof(nAxisNo); i++)
		{
			int nErr = pTaskTestPP->do_MovePos(LOC_IDX_TEST_PP_HOME, m_nHandType, m_cCmdPocket, m_vCmdPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i], m_bCleanOffsetUse);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d y axis move command,but this was wrong", m_nTestPpIdx + 1);
				OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
		}
		m_tcTimeout = (double)GetTickCount64();
		m_nStep++;
	}break;
	case 3001: // check done X1, X2 move Home
	{
		OneAxis* pMotorX1 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisX1);
		OneAxis* pMotorX2 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisX2);

		OneAxis* pMotor[2] = { pMotorX1 ,pMotorX2 };
		int nComp = 0;
		for (int i = 0; i < _countof(pMotor); i++)
		{
			int nAxisDone = pMotor[i]->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				nComp++;
			}
			else
			{
				double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
				if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d x Home Position move time over", m_nTestPpIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
					break;
				}
			}
		}

		if (nComp == _countof(pMotor))
		{
			//m_nStep = 4000;
			m_nStep = 3100;
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X1,X2 Move Home](END)", m_szCmdName, m_nStep);
		}
	}break;
	case 3100: // Loading Table Step Move 이동 확인.[9/30/2024 dohyeong.kim] 
	{
		//Table Step 계산 후, Hand X, Y이동.
		//Seq 최적화를 위해 Step 계산 후 Table Position Check 할 때 까지 대기 안함.
		//Table Position Check는 Hand X, Y이동 후 Check 진행.

		int nUseTableAlignTeaching = g_DMCont.m_dmEQP.GN(NDM0_TABLE_ALIGN_TEACHING);
		if (nUseTableAlignTeaching != TRUE)
		{
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Check Table Move] [STR]", m_szCmdName, m_nStep);
			int table_step = 0;
			BOOL bNeedTblMove = pTaskTestPP->ChkTableMove(m_nHandType, m_nCmdStage, m_cCmdPocket, m_vCmdPicker, table_step);
			if (bNeedTblMove == TRUE) {
				pTaskTestPP->m_bCalBasePoint = TRUE;//GetTeachPos에서 사용.

				int nIdx = pTaskTestPP->FromTestPPToLoadTableIdx(m_nCmdStage);
				g_TaskLoadTbl[nIdx].SetTableColStep(table_step);
			}
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Check Table Move] [END] [Table Step=%d] [Cal Base Point=%s]", m_szCmdName, m_nStep, table_step, (pTaskTestPP->m_bCalBasePoint == TRUE) ? _T("TRUE") : _T("FALSE"));
		}
		m_nStep = 3500;

	}break;
	// case 3500 ~ 3521 Start
	// Unload Hand는 Y Move 후, X, X,Y Pitch 동시에 움직인다.
	case 3500: 
	{
		if (m_nCmdStage >= LOC_IDX_STATION_1 && m_nCmdStage <= LOC_IDX_STATION_4 && m_nHandType == eTestPPUnLoadType)
		{
			m_nStep = 3510;
			break;
		}
		else {
			m_nStep = 4000;
			break;
		}
	}break;
	case 3510:	// move Y target position 
	{
		int nAxisNo[] = { CTaskTestPP::eAxisY };

		pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Y move %s safety pos](STR)", m_szCmdName, m_nStep, m_strStageName);
		for (int i = 0; i < _countof(nAxisNo); i++) {
			int nErr = pTaskTestPP->do_MovePos(m_nCmdStage, m_nHandType, m_cCmdPocket, m_vCmdPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i], m_bCleanOffsetUse);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d Y target position command,but this was wrong [CmdStage=%s]", m_nTestPpIdx + 1, m_strStageName);
				OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
		}

		m_tcTimeout = (double)GetTickCount64();
		m_nStep++;
	}break;
	case 3511:	// check done move Y Pitch target position
	{
		int nChkComp = 0;
		int nAxisNo[] = { CTaskTestPP::eAxisY };


		for (int i = 0; i < _countof(nAxisNo); i++) {
			OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo[i]);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				nChkComp++;
			}
			else
			{
				double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
				if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d Y Axis move time over [CmdStage=%s]", m_nTestPpIdx + 1, m_strStageName);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d axis done fail, Y move target position [Axis=%s] [CmdStage=%s]", m_nTestPpIdx + 1, pMotor->GetAxisName(), m_strStageName);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPpIdx, AlarmData);
					break;
				}
			}
		}
		if (nChkComp == _countof(nAxisNo)) {
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Y move %s safety pos](END)", m_szCmdName, m_nStep, m_strStageName);
			m_nStep = 3520;
		}
	}break;
	case 3520:	// move X1,Y1 or X2,Y2 Pitch, X target position 
	{
		int nAxisNo[3] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2,
			m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_X1_SW_Y1 : CTaskTestPP::eAxisPitch_HW_X2_SW_Y2,
			m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_Y1_SW_X1 : CTaskTestPP::eAxisPitch_HW_Y2_SW_X2 };

		pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X%d Pitch, Y%d Pitch , X%d move %s safety pos](STR)", m_szCmdName, m_nStep, m_nHandType + 1, m_nHandType + 1, m_nHandType + 1, m_strStageName);
		for (int i = 0; i < _countof(nAxisNo); i++) {
			int nErr = pTaskTestPP->do_MovePos(m_nCmdStage, m_nHandType, m_cCmdPocket, m_vCmdPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i], m_bCleanOffsetUse);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d X%d Pitch, Y%d Pitch, X%d target position command,but this was wrong [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_nHandType + 1, m_nHandType + 1, m_strStageName);
				OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
		}

		m_tcTimeout = (double)GetTickCount64();
		m_nStep++;
	}break;
	case 3521:	// check done move X1,Y1 or X2,Y2 Pitch target position
	{
		int nChkComp = 0;
		int nAxisNo[3] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2,
			m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_X1_SW_Y1 : CTaskTestPP::eAxisPitch_HW_X2_SW_Y2,
			m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_Y1_SW_X1 : CTaskTestPP::eAxisPitch_HW_Y2_SW_X2 };

		for (int i = 0; i < _countof(nAxisNo); i++) {
			OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo[i]);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				nChkComp++;
			}
			else
			{
				double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
				if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d, X%d, X%dPitch, Y%dPitch Axis move time over [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1,m_nHandType + 1, m_nHandType + 1, m_strStageName);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d axis done fail, X%d, X%dPitch, Y%dPitch move target position [Axis=%s] [CmdStage=%s]", m_nHandType + 1, m_nTestPpIdx + 1, m_nHandType + 1, m_nHandType + 1, pMotor->GetAxisName(), m_strStageName);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPpIdx, AlarmData);
					break;
				}
			}
		}
		if (nChkComp == _countof(nAxisNo)) {
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X%d Pitch, Y%d Pitch , X%d move %s safety pos](END)", m_szCmdName, m_nStep, m_nHandType + 1, m_nHandType + 1, m_nHandType + 1, m_strStageName);
			m_nStep = 9500;
		}
	}break;
	// case 3500 ~ 3521 End
	// Unload Hand는 Y Move 후, X, X,Y Pitch 동시에 움직인다.

	case 4000:	// move X1,Y1 or X2,Y2 Pitch, Y target position 
	{
		int nAxisNo[3] = { CTaskTestPP::eAxisY,
							m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_X1_SW_Y1 : CTaskTestPP::eAxisPitch_HW_X2_SW_Y2,
							m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_Y1_SW_X1 : CTaskTestPP::eAxisPitch_HW_Y2_SW_X2 };

		pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X%d Pitch, Y%d Pitch , Y move %s safety pos](STR)", m_szCmdName, m_nStep, m_nHandType + 1, m_nHandType + 1, m_strStageName);
		for (int i = 0; i < _countof(nAxisNo); i++) {
			int nErr = pTaskTestPP->do_MovePos(m_nCmdStage, m_nHandType, m_cCmdPocket, m_vCmdPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i], m_bCleanOffsetUse);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d X%d Pitch, Y%d Pitch, Y target position command,but this was wrong [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_nHandType + 1, m_strStageName);
				OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
		}

		m_tcTimeout = (double)GetTickCount64();
		m_nStep++;
	}break;
	case 4001:	// check done move X1,Y1 or X2,Y2 Pitch target position
	{
		int nChkComp = 0;
		int nAxisNo[3] = { CTaskTestPP::eAxisY,
			m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_X1_SW_Y1 : CTaskTestPP::eAxisPitch_HW_X2_SW_Y2,
			m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_Y1_SW_X1 : CTaskTestPP::eAxisPitch_HW_Y2_SW_X2 };

		for (int i = 0; i < _countof(nAxisNo); i++) {
			OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo[i]);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				nChkComp++;
			}
			else
			{
				double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
				if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d, X%dPitch, Y%dPitch Axis move time over [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_nHandType + 1, m_strStageName);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d axis done fail, X%dPitch, Y%dPitch move target position [Axis=%s] [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_nHandType + 1, pMotor->GetAxisName(), m_strStageName);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPpIdx, AlarmData);
					break;
				}
			}
		}
		if (nChkComp == _countof(nAxisNo)) {
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X%d Pitch, Y%d Pitch , Y move %s safety pos](END)", m_szCmdName, m_nStep, m_nHandType + 1, m_nHandType + 1, m_strStageName);
			m_nStep = 5000;
		}
	}break;

	case 5000:	// move X1 or X2 target position 
	{
		int nAxisNo[1] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2 };
		pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X%d move %s target pos](STR)", m_szCmdName, m_nStep, m_nHandType + 1, m_strStageName);
		for (int i = 0; i < _countof(nAxisNo); i++) {
			int nErr = pTaskTestPP->do_MovePos(m_nCmdStage, m_nHandType, m_cCmdPocket, m_vCmdPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i], m_bCleanOffsetUse);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d X%d target position command,but this was wrong [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_strStageName);
				OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
		}

		m_tcTimeout = (double)GetTickCount64();
		m_nStep++;
	}break;
	case 5001:	// check done move X1 or X2 target position
	{
		int nChkComp = 0;
		int nAxisNo[1] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2 };
		for (int i = 0; i < _countof(nAxisNo); i++) {
			OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo[i]);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				nChkComp++;
			}
			else
			{
				double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
				if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d X%d Axis move time over [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_strStageName);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d axis done fail, X%d move target position [Axis=%s] [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, pMotor->GetAxisName(), m_strStageName);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPpIdx, AlarmData);
					break;
				}
			}
		}
		if (nChkComp == _countof(nAxisNo)) {
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X%d move %s target pos](END)", m_szCmdName, m_nStep, m_nHandType + 1, m_strStageName);
			m_nStep = 9500;
		}
	}break;
	// 	case 6000:
	// 		{
	// 			BOOL bDeviceExistCheck = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TestPP_DevExist_Check);
	// 			BOOL bVisionUseMode    = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_VisionUse);
	// 			int  nDeviceMode       = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
	// 
	// 			if( m_nCmdStage >= LOC_IDX_STATION_1 && m_nCmdStage <= LOC_IDX_STATION_4)
	// 			{
	// 				if(bVisionUseMode == DEF_ON && nDeviceMode == eDeviceMode)
	// 				{
	// 					if(bDeviceExistCheck == TRUE && m_nCmdPick == DEF_PICK && pTaskTestPP->ChkVacuum(DEF_ON, m_nHandType, m_vCmdPicker) != ERR_NO_ERROR){
	// 						//m_nStep = 6500;	//vision Exist
	// 						m_tcTimeout = (double)GetTickCount64();
	// 						break;
	// 					}else if(m_nCmdPick == DEF_PLACE && m_nHandType == eTestPPUnLoadType){ // DEF_PLACE
	// 						m_nStep = 7500;	//vision Empty
	// 						m_tcTimeout = (double)GetTickCount64();
	// 						break;
	// 					}
	// 				}
	// 			}
	// 
	// 			m_tcTimeout = (double)GetTickCount64();
	// 			m_nStep =8500;	// not check vision
	// 		}break;
			// === VISION EMPTY CHECK <END>

	// 	case 8500:	// move X1 or X2 target position
	// 		{
	// 			if (m_nCmdStage >= LOC_IDX_STATION_1 && m_nCmdStage <= LOC_IDX_STATION_4)
	// 			{
	// 				int nPressIdx = pTaskTestPP->GetPressNum(m_nCmdStage);
	// 			
	// 				double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
	// 				if (dSpanTime > MOTOR_TIME_OUT_SHORT*1000.0) {
	// 					CParamList AlarmData;
	// 					AlarmData.m_fnAddItemFormat("Test PP %d Y Axis can not move station [WorkStage=%s not safety]", m_nTestPpIdx + 1, m_strStageName);
	// 					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + nPressIdx, AlarmData);
	// 					break;
	// 				}
	// 				
	// 				if (g_TaskPressUnit[nPressIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
	// 					break;
	// 			}
	// 
	// 			int nAxisNo[1] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2 };
	// 			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X%d move %s safety pos](STR)", m_szCmdName, m_nStep, m_nHandType + 1, m_strStageName);
	// 			for (int i = 0; i < _countof(nAxisNo); i++) {
	// 				int nErr = pTaskTestPP->do_MovePos(m_nCmdStage, m_nHandType, m_cCmdPocket, m_vCmdPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i]);
	// 				if (nErr != ERR_NO_ERROR) {
	// 					CParamList AlarmData;
	// 					AlarmData.m_fnAddItemFormat("Test PP %d X%d target position command,but this was wrong [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_strStageName);
	// 					ReportAlarm(nErr, AlarmData);
	// 					break;
	// 				}
	// 			}
	// 
	// 			m_tcTimeout = (double)GetTickCount64();
	// 			m_nStep++;
	// 		}break;
	// 	case 8501:	// check done move X1 or X2 target position
	// 		{
	// 			int nChkComp = 0;
	// 			int nAxisNo[1] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2 };
	// 			for (int i = 0; i < _countof(nAxisNo); i++) {
	// 				OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo[i]);
	// 				int nAxisDone = pMotor->AxisDone();
	// 				if (nAxisDone == ERR_NO_ERROR) {					
	// 					nChkComp++;				
	// 				}
	// 				else 
	// 				{
	// 					double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
	// 					if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
	// 						CParamList AlarmData;
	// 						AlarmData.m_fnAddItemFormat("Test PP %d X%d Axis move time over, move x target position", m_nTestPpIdx + 1, m_nHandType +1);
	// 						AlarmData.m_fnAddItemFormat("[WorkStage=%s]", m_strStageName);
	// 						ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
	// 						break;
	// 					}
	// 
	// 					if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
	// 					{
	// 						CParamList AlarmData;
	// 						AlarmData.m_fnAddItemFormat("Test PP %d X%d axis done fail [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_strStageName);
	// 						ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPpIdx, AlarmData);
	// 					}
	// 				}
	// 			}
	// 			if (nChkComp == _countof(nAxisNo)) {
	// 				m_nStep = 9500;
	// 				m_tcTimeout = (double)GetTickCount64();
	// 				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X%d Axis Move to %s Pos](END)", m_szCmdName, m_nStep, m_nHandType + 1,m_strStageName);
	// 			}
	// 			
	// 		}break;	
	case 9500:	// check arrived sensor
	{
		if ((m_nCmdStage >= LOC_IDX_STATION_1 && m_nCmdStage <= LOC_IDX_STATION_4) && m_nCmdPick == DEF_PICK)
		{
			if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TEST_HAND_PNP_SITE_ARRIVED_SEN) == DEF_ON) {
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Arrived Sensor Check](STR)", m_szCmdName, m_nStep);

				CString strErr = _T("");

				std::vector<CPoint> vErrPocket;
				vErrPocket.clear();

				std::vector<CString> vErrHistory;
				int nDeviceType = 0;
				int nErrArrivedSen = g_TaskPressUnit[m_nPressIdx].ChkSocketArrivedSen(strErr, vErrPocket, vErrHistory, nDeviceType);
				int nRetPos = g_TaskPressUnit[m_nPressIdx].ChkMotorPos(LOC_IDX_PRESS_UNIT_CONTACT, CTaskPressUnit::eAxisZ);
				if (nErrArrivedSen != ERR_NO_ERROR && nRetPos != ERR_NO_ERROR)
				{

					int nVibRetryCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_VIBRATOR_RETRY_CNT);
					if (m_nVibCurTryCnt >= nVibRetryCnt)   // Vibrator를 Retry 횟수만큼 실행한 상태 // Retouch Mode 에 따라 분기
					{
						m_nVibCurTryCnt = 0;
						m_bVib_Retry_Complete = TRUE;

						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d before the %s, Station =%d, Site =%s", m_nTestPpIdx + 1, m_strPickPlace, m_nPressIdx + 1, strErr);

						if ((int)vErrHistory.size() > 0) {
							for (int i = 0; i < (int)vErrHistory.size(); i++) {
								AlarmData.m_fnAddItemFormat(vErrHistory[i]);
							}
						}

						m_BackAlarmData = AlarmData;
						m_nStep = 31000; //Y Move Home

										 //ReportAlarm((m_nCmdPick == DEF_PICK ? ERR_TEST_PP_1_SITE_ARRIVED_SENSOR_PICK : ERR_TEST_PP_1_SITE_ARRIVED_SENSOR_PLACE ) + m_nTestPpIdx, AlarmData,/*vErrPocket*/m_vErrPicker);
						break;
					}
					else  //Vibrator 를 더 진행하거나 완료됐거나..
					{
						if (m_bVib_Retry_Complete)
						{
							m_bVib_Retry_Complete = FALSE;
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("Test PP %d before the %s, Station =%d, Site =%s", m_nTestPpIdx + 1, m_strPickPlace, m_nPressIdx + 1, strErr);

							if ((int)vErrHistory.size() > 0) {
								for (int i = 0; i < (int)vErrHistory.size(); i++) {
									AlarmData.m_fnAddItemFormat(vErrHistory[i]);
								}
							}
							m_BackAlarmData = AlarmData;
							m_nStep = 31000; //Y Move Home

											 //ReportAlarm((m_nCmdPick == DEF_PICK ? ERR_TEST_PP_1_SITE_ARRIVED_SENSOR_PICK : ERR_TEST_PP_1_SITE_ARRIVED_SENSOR_PLACE) + m_nTestPpIdx, AlarmData,/*vErrPocket*/m_vErrPicker);
							break;
						}
						else
						{
							m_nBackVibratorStep = m_nStep;
							m_nVibCurTryCnt++;
							m_nStep = do_Vibrator_On; // Vibrator 동작 Case								
							break;
						}
					}

					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP%d before the %s Station =%d, Site =%s", m_nTestPpIdx + 1, m_strPickPlace, m_nPressIdx + 1, strErr);

					if ((int)vErrHistory.size() > 0) {
						for (int i = 0; i < (int)vErrHistory.size(); i++) {
							AlarmData.m_fnAddItemFormat(vErrHistory[i]);
						}
					}

					//ReportAlarm( (m_nCmdPick == DEF_PICK ? ERR_TEST_PP_1_SITE_ARRIVED_SENSOR_PICK : ERR_TEST_PP_1_SITE_ARRIVED_SENSOR_PLACE) + m_nTestPpIdx, AlarmData,vErrPocket);
					m_BackAlarmData = AlarmData;
					m_nStep = 31000; //Y Move Home
					break;
				}
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Arrived Sensor Check](END)", m_szCmdName, m_nStep);
			}
		}

		m_nLast_Executing_Step = m_nStep = 10000;
		m_nRetryCount = 0;
		m_nPickRetry = 0;
	}break;

	// step 0 ~ 10000 이하 까지 X/Y CmdStage 위치이며, Z는 Safety 위치 이다.
	case 10000:
	{
		m_nRetryCount = 0;
		m_tcTimeout = (double)GetTickCount64();
		if (m_nCmdStage >= LOC_IDX_TEST_PP_TBL_1_LD && m_nCmdStage <= LOC_IDX_TEST_PP_TBL_4_UD && CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto) {
			m_nStep++;
			break;
		}
		else if (m_nCmdStage >= LOC_IDX_STATION_1 && m_nCmdStage <= LOC_IDX_STATION_4)
		{
			m_nStep = 10600;
			break;
		}
		m_nStep = 11500;
	}break;
	case 10001:
	{
		double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
		if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Test PP %d move time over, wait load table", m_nTestPpIdx + 1);
			CBaseSeqCmd::ReportAlarm(ERR_LOAD_TBL_1_NOT_ARRIVED_POS + m_nTestPpIdx, AlarmData);
			break;
		}

		int nLoadTblIdx = pTaskTestPP->FromTestPPToLoadTableIdx(m_nCmdStage);
		int nLoadTblCmdStage = pTaskTestPP->FromTestPPToLoadTableCmdStageConvetor(m_nCmdStage);

		//기존 방식
		//int nRetLoadTblPos = g_TaskLoadTbl[nLoadTblIdx].ChkMotorPos(nLoadTblCmdStage, CTaskLoadTable::eAxisAll);

		//Table Step 포함하여 Position확인.
		int table_col_step = g_TaskLoadTbl[nLoadTblIdx].GetTableColStep();

		int nRetLoadTblPos = g_TaskLoadTbl[nLoadTblIdx].ChkMotorPos(nLoadTblCmdStage, CTaskLoadTable::eAxisAll, table_col_step);
		if (nRetLoadTblPos == ERR_NO_ERROR && g_TaskLoadTbl[nLoadTblIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready) {
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Check Table Pos] [Table Step=%d] [END] ", m_szCmdName, m_nStep, table_col_step);
			if (nLoadTblIdx == eLoadTbl_1 || nLoadTblIdx == eLoadTbl_4)
				g_DMCont.m_dmTestPP.SB(NDM04_TESTHAND1_ALREADY_PICK_UNLOAD_TABLE_CALL + m_nTestPpIdx, FALSE);
			else
				g_DMCont.m_dmTestPP.SB(NDM04_TESTHAND1_ALREADY_PICK_TABLE_CALL + m_nTestPpIdx, FALSE);

			m_nStep = 11500;
		}
		else {
			g_TaskLoadTbl[nLoadTblIdx].SetTestppPrePos(nLoadTblCmdStage);
			// Load Table Already pick 상황만 처리
			if (nRetLoadTblPos != ERR_NO_ERROR && g_TaskLoadTbl[nLoadTblIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				if (nLoadTblIdx == eLoadTbl_1 || nLoadTblIdx == eLoadTbl_4)
				{
					g_DMCont.m_dmTestPP.SB(NDM04_TESTHAND1_ALREADY_PICK_UNLOAD_TABLE_CALL + m_nTestPpIdx, TRUE);
				}
				else {
					g_DMCont.m_dmTestPP.SB(NDM04_TESTHAND1_ALREADY_PICK_TABLE_CALL + m_nTestPpIdx, TRUE);
				}
			}
		}
	}break;
	case 10600:
	{
		double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
		if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Test PP %d move time over, wait press unit safety", m_nTestPpIdx + 1);
			CBaseSeqCmd::ReportAlarm(ERR_PRESS_UNIT_1_NOT_DETECT_SAFETY_SEN + m_nPressIdx, AlarmData);
			break;
		}

		int nErr = g_TaskPressUnit[m_nPressIdx].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON);
		if (nErr == ERR_NO_ERROR && g_TaskPressUnit[m_nPressIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready) {
			m_nStep = 11500;
		}
	}break;
	case 11500:	// move z target pos
	{
		if (m_nCmdPick == DEF_PLACE && CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto)
		{
			pTaskTestPP->MakeLog("[%s EXEC=%8d] [Before blow check device] [STR]", m_szCmdName, m_nStep);
			CString strMsg = _T("");
			std::vector<CString> vErrstring; vErrstring.clear();
			int nChkErr = pTaskTestPP->ChkBeforeBlowDevice(m_vCmdPicker, m_nHandType, strMsg, vErrstring);
			if (nChkErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat(strMsg);
				for (int i = 0; i < (int)vErrstring.size(); i++)
				{
					AlarmData.m_fnAddItemFormat("%s", vErrstring[i]);
				}
				ReportAlarm(nChkErr, AlarmData, pTaskTestPP->GetLostPicker(m_nHandType));
				break;
			}
			pTaskTestPP->MakeLog("[%s EXEC=%8d] [Before blow check device] [END]", m_szCmdName, m_nStep);
		}

		int nLocZ = (m_nCmdPick == DEF_PICK) ? CTaskTestPP::eZPOS_PICK : CTaskTestPP::eZPOS_PLACE;
		int nAxisNo[1] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2 };
		pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z%d Axis Move to %s %s Pos](STR)", m_szCmdName, m_nStep, m_nHandType + 1, m_strStageName, m_strPickPlace);



		for (int i = 0; i < _countof(nAxisNo); i++)
		{
			std::vector<CPoint> vTargetPicker;
			vTargetPicker.clear();

			vTargetPicker = m_vCmdPicker;
			
			double dPickerRetryOffset = 0.0;
			if (pTaskTestPP->m_vFailPicker[m_nHandType].size() != 0)
			{
				vTargetPicker = pTaskTestPP->m_vFailPicker[m_nHandType];
				if (m_nCmdPick == DEF_PICK) // Picker Error Retry 진행 시 Retry Offset 추가 retry 횟수 설정 값 만큼 곱하여 증가
				{
					double dmOffset = g_DMCont.m_dmTestPP.GD(DDM4_TestPP1_Pick_Retry_Offset + m_nTestPpIdx);
					dPickerRetryOffset = dmOffset* m_nPickRetry;

					 if (dPickerRetryOffset > Z_MOTOR_PICK_RETRY_MAX_POS_um)
						 dPickerRetryOffset = Z_MOTOR_PICK_RETRY_MAX_POS_um;

					pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z%d Picker Retry Offset dPickerRetryOffset =%0.2f Axis Move to %s %s Pos](STR)", m_szCmdName, m_nStep, m_nHandType + 1, dPickerRetryOffset, m_strStageName, m_strPickPlace);
				}
					
			}

			int	nErr = pTaskTestPP->do_MovePos(m_nCmdStage, m_nHandType, m_cCmdPocket, vTargetPicker, nLocZ, nAxisNo[i], m_bCleanOffsetUse,100, dPickerRetryOffset);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d Z%d move %s position command,but this was wrong", m_nTestPpIdx + 1, m_nHandType + 1, m_strPickPlace);
				AlarmData.m_fnAddItemFormat("[CmdStage=%s]", m_strStageName);
				OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
		}

		BOOL bPreVacuum = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TEST_HAND_PRE_VACUUM);
		if (m_nCmdPick == DEF_PICK && bPreVacuum == eOPTION_USE)
		{
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [%s](STR)", m_szCmdName, m_nStep, (m_nCmdPick == DEF_PICK ? _T("Vacuum") : _T("Blow")));
			pTaskTestPP->do_VacBlow(eVacBlwSt_VacOn, m_nHandType, m_vCmdPicker);
		}

		m_tcTimeout = (double)GetTickCount64();
		m_nStep = 11510;
	}break;
	case 11510:
	{
		int nAxisNo = m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
		OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo);
		int nAxisDone = pMotor->AxisDone();
		//int nErrSen = pTaskTestPP->Chk_Z_Safety_Sen_All(m_nHandType);

		if (nAxisDone == ERR_NO_ERROR/* && nErrSen == ERR_NO_ERROR*/) {
			// == check step function
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z%d Axis Move to %s %s Pos](END)", m_szCmdName, m_nStep, m_nHandType + 1, m_strStageName, m_strPickPlace);

			//Picker Motor Wait Time - Load Hand Station에 Place 하는 상황에만 적용
			if (m_nCmdPick == DEF_PLACE && (m_nCmdStage >= LOC_IDX_STATION_1 && m_nCmdStage <= LOC_IDX_STATION_4) && m_nHandType == eTestPPLoadType)
			{
				double dmPickerWaitTime = g_DMCont.m_dmTestPP.GD(DDM4_TestPP1_Picker_Wait_Time + m_nTestPpIdx);
				if(dmPickerWaitTime > 0.0)
				{
					pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z%d Axis Move to %s %s Wait Time](STR)", m_szCmdName, m_nStep, m_nHandType + 1, m_strStageName, m_strPickPlace);
					Sleep(dmPickerWaitTime);
					pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z%d Axis Move to %s %s Wait Time](END)", m_szCmdName, m_nStep, m_nHandType + 1, m_strStageName, m_strPickPlace);
				}
			
			}
	
			if (g_StepTestPP_SW[m_nTestPpIdx].GetStatus() == DEF_ON) {

				BOOL bPreVacuum = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TEST_HAND_PRE_VACUUM);
				if (m_nCmdPick == DEF_PICK && bPreVacuum == eOPTION_USE)
				{
					pTaskTestPP->do_VacBlow(eVacBlwSt_BlwOn, m_nHandType, m_vCmdPicker);
				}

				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [STEP Working Stop Loop, Working=%s %s", m_szCmdName, m_nStep, m_strStageName, m_strPickPlace);
				g_StepTestPP_SW[m_nTestPpIdx].SetChangeStatusOff();
				g_StepTestPP_SW[m_nTestPpIdx].SetChangeStatusOnOff(DEF_ON);
				SetCmdStatus(eSeqCmdStatus_Alarm);
				g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
				break;
			}
			else {

				BOOL bPreVacuum = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TEST_HAND_PRE_VACUUM);
				if (m_nCmdPick == DEF_PLACE || (m_nCmdPick == DEF_PICK && bPreVacuum != eOPTION_USE))
				{
					if (m_nCmdPick == DEF_PLACE)
					{
						//Place Pos 인대, Z축 Safety Sensor 감지 되면 내려가지 않은 것으로 판단하고 알람 발생 한다.
						int nErrSen = pTaskTestPP->Chk_Z_Safety_Sen_Check_Type(m_vCmdPicker, m_nHandType, ePlaceSafetySensorCheck);
						if (!pTaskTestPP->m_vSafetyFailPicker[m_nHandType][ePlaceSafetySensorCheck].empty())
						{
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("Test PP %d Z%d Place Pos Safety Sensing Error %s", m_nTestPpIdx + 1, m_nHandType + 1, pTaskTestPP->m_sSaftyFailPicker[m_nHandType][ePlaceSafetySensorCheck]);
							ReportAlarm(ERR_TEST_PP_1_Z1_PLACE_POS_SAFETY_SENSING + pTaskTestPP->GetHandTypeIndex(m_nHandType), AlarmData, pTaskTestPP->GetSafetyFailPicker(m_nHandType, ePlaceSafetySensorCheck));
							break;
						}
					}

					int nVacBlow = m_nCmdPick == DEF_PICK ? eVacBlwSt_VacOn : eVacBlwSt_BlwOn;
					pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [%s](STR)", m_szCmdName, m_nStep, (m_nCmdPick == DEF_PICK ? _T("Vacuum") : _T("Blow")));
					int nErr = pTaskTestPP->do_VacBlow(nVacBlow, m_nHandType, m_vCmdPicker);
					if (nErr != ERR_NO_ERROR) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d [CmdPicker=%s] %s command,but this was wrong", m_nTestPpIdx + 1, m_strWorkPkrName, (m_nCmdPick == DEF_PICK ? _T("Vacuum") : _T("Blow")));
						ReportAlarm(nErr, AlarmData);
						break;
					}
					if (m_nCmdPick == DEF_PLACE) {
						pTaskTestPP->m_bCyclePause = TRUE;

						CString strTmp = _T("");
						// Blow 는 Blow 하고 바로 Device Data 이동
						pTaskTestPP->m_vFailPicker[m_nHandType].clear();
						if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto) {
							for (std::vector<CPoint>::size_type i = 0; i < (int)m_vCmdPicker.size(); i++) {
								strTmp.Format("[X:%d, Y:%d]", m_vCmdPicker[i].x + 1, m_vCmdPicker[i].y + 1);
								if (pTaskTestPP->ChkVacuum(DEF_OFF, m_nHandType, m_vCmdPicker[i]) != ERR_NO_ERROR) { //Blow Retry						
									pTaskTestPP->m_vFailPicker[m_nHandType].push_back(m_vCmdPicker[i]);
								}
							}

							if (pTaskTestPP->m_vFailPicker[m_nHandType].size() > 0) {
								if (m_nRetryCount < m_nMaxRetryCnt)
								{
									Sleep(500.0);
									m_nRetryCount++;
									m_nStep = 15500; // pick/place fail retry, move safety
									pTaskTestPP->MakeLog("[Place Blow Retry] CmdStage=%s, CmdTargetX=%d, CmdTargetY=%d, CmdPickPicker=%s", pTaskTestPP->GetLocName(m_nCmdStage), m_cCmdPocket.x + 1, m_cCmdPocket.y + 1, pTaskTestPP->GetPickerName(pTaskTestPP->GetFailPicker(m_nHandType)));
									break;
								}
							}
							//int nRet = ERR_NO_ERROR;
							std::vector<CString> vErrString; vErrString.clear();
							int nDeviceType = 0;
							int nRet = pTaskTestPP->ProcDeviceData(m_nCmdStage, m_nHandType, m_cCmdPocket.x, m_cCmdPocket.y, m_vCmdPicker, DEF_PLACE, vErrString, nDeviceType);
							pTaskTestPP->m_bCyclePause = FALSE;
							pTaskTestPP->MakeLog("[ProcDevice Data Place] CmdStage=%s, CmdTargetX=%d, CmdTargetY=%d, CmdPickPicker=%s", pTaskTestPP->GetLocName(m_nCmdStage), m_cCmdPocket.x + 1, m_cCmdPocket.y + 1, m_strWorkPkrName);

							CParamList AlarmData;
							if (nRet >= ERR_TEST_PP_1_Z1_DEVICE_MISS && nRet <= ERR_TEST_PP_2_Z2_DEVICE_MISS) {
								AlarmData.m_fnAddItemFormat("Test PP %d %s picker pick after that lost device", m_nTestPpIdx + 1, pTaskTestPP->GetHandTypeName(m_nHandType));
								for (int i = 0; i < (int)vErrString.size(); i++)
								{
									AlarmData.m_fnAddItemFormat("%s", vErrString[i]);
								}
								ReportAlarm(nRet, AlarmData, pTaskTestPP->GetFailPicker(m_nHandType));
								break;
							}

							if (nRet != ERR_NO_ERROR) {
							
								AlarmData.m_fnAddItemFormat("Test PP %d CmdStage=%s", m_nTestPpIdx + 1, m_strStageName);
								ReportAlarm(nRet, AlarmData, pTaskTestPP->GetFailPicker(m_nHandType));
								break;
							}
							pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [%s](END)", m_szCmdName, m_nStep, (m_nCmdPick == DEF_PICK ? _T("Vacuum") : _T("Blow")));
							m_nRetryCount = 0;
							m_nLast_Executing_Step = m_nStep = 20000;
							break;
						}
					}
				}

				m_nRetryCount = 0;
				m_tcTimeout = (double)GetTickCount64();
				m_nStep = 15000;
				break;
			}
		}
		else
		{
			double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
			if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d move time over, z %s position [CmdStage=%s]", m_nTestPpIdx + 1, m_strPickPlace, m_strStageName);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
				break;
			}

		}
	}break;
	case 15000:	// pick/place loop
	{
		int nSleep = (m_nCmdPick == DEF_PICK ? m_nVacuumWait : m_nBlowWait);

		double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
		if (dSpanTime > ((double)nSleep))
		{
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [%s](END)", m_szCmdName, m_nStep, (m_nCmdPick == DEF_PICK ? _T("Vacuum") : _T("Blow")));
			m_nStep = 20000;
			m_nRetryCount = 0;
		}
	}break;
	case 15500: // pick/place fail retry, move safety
	{
		int nAxisNo = m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
		pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z Axis Move to Safety Pos](STR)", m_szCmdName, m_nStep);
		int nErr = pTaskTestPP->do_MovePos(m_nCmdStage, m_nHandType, m_cCmdPocket, m_vCmdPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Test PP %d Z%d move CmdStage=%s %s position retry command,but this was wrong", m_nTestPpIdx + 1, m_nHandType + 1, m_strStageName, m_strPickPlace);
			OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo);
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(nErr, AlarmData);
			break;
		}
		m_tcTimeout = (double)GetTickCount64();
		m_nStep++;
	}break;
	case 15501:	// pick/place fail retry done z safety 
	{
		std::vector<CPoint> vPkr;
		pTaskTestPP->MakeAllPicker(vPkr);
		int nAxisNo = m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
		m_vErrPicker.clear();
		int nAxisDone = m_pMotor[nAxisNo]->AxisDone(vPkr, m_vErrPicker);
		int nErrSen = pTaskTestPP->Chk_Z_Safety_Sen_Check_Type(vPkr,m_nHandType);

		if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR) {
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z Axis Move to Safety Pos](END)", m_szCmdName, m_nStep);
			m_nStep = 10000;
		}
		else
		{
			std::vector<CPoint> vErrPkr; vErrPkr.clear();
			CString sAlarmData = _T("");
			int nErr = pTaskTestPP->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, pTaskTestPP->GetSafetyFailPicker(m_nHandType, ePickerSafetySensorCheckType::eSafetySensorCheck), m_nHandType, vErrPkr, sAlarmData);

			double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
			if (dSpanTime > TIME_OUT_SHORT*1000.0) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("%s", sAlarmData);
				AlarmData.m_fnAddItemFormat("Axis=%s", m_pMotor[nAxisNo]->GetAxisName());
				ReportAlarm(nErr, AlarmData, vErrPkr);
				break;
			}
		}
	}break;
	case 20000:	// move z safety
		{
			int nAxisNo = m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z%d Axis Move to Safety Pos](STR)",m_szCmdName ,m_nStep, m_nHandType+1);
			int nErr = pTaskTestPP->do_MovePos(m_nCmdStage, m_nHandType, m_cCmdPocket, m_vCmdPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d z move pick position command,but this was wrong",m_nTestPpIdx+1);
				AlarmData.m_fnAddItemFormat("[CmdStage=%s]", m_strStageName);
				OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout = (double)GetTickCount64();
			m_nStep++;
		}break;
	case 20001:	// done move z safety
		{
			int nAxisNo = m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
			std::vector<CPoint> vPkr;
			pTaskTestPP->MakeAllPicker(vPkr);
			m_vErrPicker.clear();
			int nAxisDone = m_pMotor[nAxisNo]->AxisDone(vPkr, m_vErrPicker);
			int nErrSen = pTaskTestPP->Chk_Z_Safety_Sen_Check_Type(vPkr,m_nHandType);


			if( nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR){
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z%d Axis Move to Safety Pos](END)",m_szCmdName ,m_nStep, m_nHandType + 1);
				m_nStep = 20500;
			}else 
			{
				std::vector<CPoint> vErrPkr; vErrPkr.clear();
				CString sAlarmData = _T("");
				int nErr = pTaskTestPP->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, pTaskTestPP->GetSafetyFailPicker(m_nHandType, ePickerSafetySensorCheckType::eSafetySensorCheck), m_nHandType, vErrPkr, sAlarmData);
				double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
				if (dSpanTime > TIME_OUT_SHORT*1000.0) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("%s", sAlarmData);
					AlarmData.m_fnAddItemFormat("Axis=%s", m_pMotor[nAxisNo]->GetAxisName());
					ReportAlarm(nErr, AlarmData, vErrPkr);
					break;
				}
			}
		}break;
	case 20500: // check device 낙석
		{
			if(m_nCmdPick == DEF_PICK)
			{
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Pick device Check](STR)", m_szCmdName, m_nStep);
				int nNotVacCount = 0;
				CString strPocketErr = _T("");
				CString strTmp = _T("");
				pTaskTestPP->m_vFailPicker[m_nHandType].clear();

				std::vector<CPoint> vFailPicker;
				vFailPicker.clear();
				for(std::vector<CPoint>::size_type i=0; i < (int)m_vCmdPicker.size(); i++){
					if( pTaskTestPP->ChkVacuum(DEF_ON, m_nHandType, m_vCmdPicker[i]) != ERR_NO_ERROR ){
						vFailPicker.push_back(m_vCmdPicker[i]);
						nNotVacCount++;
						strTmp.Format(_T("[%d]"), (m_vCmdPicker[i].x + (m_vCmdPicker[i].y * 4/*Picker Y 계산*/ * 2/*2 Pitch*/) + (m_cCmdPocket.y * STATION_MAX_X_SITE) + 1));
						strPocketErr += strTmp;
						pTaskTestPP->m_vFailPicker[m_nHandType] = vFailPicker;
					}
				}

				if (nNotVacCount != 0) {
					if (m_nPickRetry < m_nMaxRetryCnt) {
						m_nRetryCount = 0;
						m_nPickRetry++;
						m_nStep = 15500; // pick/place fail retry, move safety
						break;
					}
				}

				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Pick device Check](END)",m_szCmdName ,m_nStep);
				if( nNotVacCount != 0 )
				{				
					int nRet = 0;
					std::vector<CString> vErrString; vErrString.clear();
					if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto)
					{
						//Pick 실패 후 Table Jam 처리 시, Data가 Table에 남아 있어 Barcode Length Error 발생.
						if (LOC_IDX_TEST_PP_TBL_1_LD <= m_nCmdStage  && m_nCmdStage <= LOC_IDX_TEST_PP_TBL_4_UD) {
							int nDeviceType = 0;
							nRet = pTaskTestPP->ProcDeviceData(m_nCmdStage, m_nHandType, m_cCmdPocket.x, m_cCmdPocket.y, m_vCmdPicker, DEF_PICK, vErrString, nDeviceType);
						}
					}
					CParamList AlarmData;
					if (nRet >= ERR_TEST_PP_1_Z1_DEVICE_MISS && nRet <= ERR_TEST_PP_2_Z2_DEVICE_MISS) {
						AlarmData.m_fnAddItemFormat("Test PP %d %s picker pick after that lost device", m_nTestPpIdx + 1, pTaskTestPP->GetHandTypeName(m_nHandType));
						for (int i = 0; i < (int)vErrString.size(); i++)
						{
							AlarmData.m_fnAddItemFormat("%s", vErrString[i]);
						}
						ReportAlarm(nRet, AlarmData, vFailPicker);
						break;
					}
									
					AlarmData.m_fnAddItemFormat("Test PP %d pick after that pick picker device not sensing", m_nTestPpIdx + 1);
					AlarmData.m_fnAddItemFormat("[CmdPicker = %s, Stage = %s, Pocket = %s]", pTaskTestPP->GetPickerName(vFailPicker), m_strStageName, strPocketErr);

					int nDeviceType = pTaskTestPP->CheckDeviceType(m_nCmdStage, m_nHandType,m_cCmdPocket.x, m_cCmdPocket.y, m_vCmdPicker, DEF_PICK);
					int nErr = nDeviceType == eDeviceType::_eDeviceType_Cleaning ? ERR_TEST_PP_1_Z1_CLEAN_DEVICE_PICK_FAIL : ERR_TEST_PP_1_Z1_DEVICE_PICK_FAIL;
				
					ReportAlarm(nErr + pTaskTestPP->GetHandTypeIndex(m_nHandType), AlarmData, vFailPicker);
					break;					
				}
				else
				{
					// save data
					if(CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto)
					{
						std::vector<CString> vErrString; vErrString.clear();
						int nDeviceType = 0;
						int nRet = pTaskTestPP->ProcDeviceData(m_nCmdStage, m_nHandType, m_cCmdPocket.x, m_cCmdPocket.y, m_vCmdPicker, DEF_PICK, vErrString, nDeviceType);
						pTaskTestPP->MakeLog("[ProcDevice Data Pick] CmdStage=%s, CmdTargetX=%d, CmdTargetY=%d, CmdPickPicker=%s", m_strStageName, m_cCmdPocket.x+1, m_cCmdPocket.y+1, m_strWorkPkrName);

						CParamList AlarmData;

						if (nRet >= ERR_TEST_PP_1_Z1_DEVICE_MISS && nRet <= ERR_TEST_PP_2_Z2_DEVICE_MISS) {
							AlarmData.m_fnAddItemFormat("Test PP %d %s picker pick after that lost device", m_nTestPpIdx + 1, pTaskTestPP->GetHandTypeName(m_nHandType));
							for (int i = 0; i < (int)vErrString.size(); i++)
							{
								AlarmData.m_fnAddItemFormat("%s", vErrString[i]);
							}
							ReportAlarm(nRet, AlarmData, pTaskTestPP->GetFailPicker(m_nHandType));
							break;
						}

						if(nRet != ERR_NO_ERROR){						
							AlarmData.m_fnAddItemFormat("Test PP %d [Picker=%s]", m_nTestPpIdx+1, m_strWorkPkrName);
							ReportAlarm( nRet, AlarmData, pTaskTestPP->GetFailPicker(m_nHandType) );
							break;
						}
					}
				}            
			}
			else
			{
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Place device Check](STR)",m_szCmdName ,m_nStep);
				int nNotVacCount = 0;
				CString strPocketErr = _T("");
				CString strTmp = _T("");
				pTaskTestPP->m_vFailPicker[m_nHandType].clear();
				for(std::vector<CPoint>::size_type i=0; i < (int)m_vCmdPicker.size(); i++){
					if( pTaskTestPP->ChkVacuum(DEF_OFF, m_nHandType, m_vCmdPicker[i]) != ERR_NO_ERROR ){
						pTaskTestPP->SetFailPicker(m_vCmdPicker[i], m_nHandType);
						nNotVacCount++;
						strTmp.Format(_T("[%d]"), (m_vCmdPicker[i].x + (m_vCmdPicker[i].y * 4/*Picker Y 계산*/ * 2/*2 Pitch*/) + (m_cCmdPocket.y * STATION_MAX_X_SITE) + 1));
						strPocketErr += strTmp;
					}
				}
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Place device Check](END)",m_szCmdName ,m_nStep);

				int nDeviceType = pTaskTestPP->CheckDeviceType(m_nCmdStage, m_nHandType, m_cCmdPocket.x, m_cCmdPocket.y, m_vCmdPicker, DEF_PLACE);
				int nErr = nDeviceType == eDeviceType::_eDeviceType_Cleaning ? ERR_TEST_PP_1_Z1_CLEAN_DEVICE_PLACE_FAIL : ERR_TEST_PP_1_Z1_DEVICE_PLACE_FAIL;

				if( nNotVacCount != 0 ){
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d place picker is sensing, check device", m_nTestPpIdx+1);
					AlarmData.m_fnAddItemFormat("[CmdPicker = %s, Stage = %s, Pocket = %s]",m_strWorkPkrName ,m_strStageName,strPocketErr);
					ReportAlarm(nErr + pTaskTestPP->GetHandTypeIndex(m_nHandType), AlarmData, pTaskTestPP->GetFailPicker(m_nHandType) );
					break;
				}
			}
			m_nStep = 22000;
		}break;
	case 22000:	// move X home
		{			
			// place일 경우 한번 더 해당 site or buffer에 안착을 한번 더 확인해 준다.
			if (m_nCmdPick == DEF_PLACE)
			{
				if (m_nCmdStage >= LOC_IDX_STATION_1 && m_nCmdStage <= LOC_IDX_STATION_4)
				{
					if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TEST_HAND_PNP_SITE_ARRIVED_SEN) == DEF_ON) 
					{
						pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Arrived Sensor Check](STR)", m_szCmdName, m_nStep);

						CString strErr = _T("");

						m_vErrPicker.clear();

						std::vector<CString> vErrHistory;
						int nDeviceType = 0;
						int nErrArrivedSen = g_TaskPressUnit[m_nPressIdx].ChkSocketArrivedSen(strErr, /*vErrPocket*/m_vErrPicker,vErrHistory, nDeviceType);
						int nRetPos = g_TaskPressUnit[m_nPressIdx].ChkMotorPos(LOC_IDX_PRESS_UNIT_CONTACT, CTaskPressUnit::eAxisZ);

						// Vibrator 강제 동작 확인 Option 추가. [5/14/2025 dohyeong.kim]
						int nIsVibModeUse = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_VIBRATOR_OPTION);
						int nPressSensorType = g_DMCont.m_dmEQP.GN(NDM0_PRESS_ARRIVED_SENSOR_TYPE);
						BOOL bVibratorForceAction = FALSE;
 						if (nPressSensorType == CTaskPressUnit::eArriveSensor_ForceAction && nRetPos != ERR_NO_ERROR && nIsVibModeUse == TRUE)
 						{
 							int nVibRetryCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_VIBRATOR_RETRY_CNT); //얘는 하나로 변경 가능.
							if (m_nVibCurTryCnt < nVibRetryCnt)
							{
								bVibratorForceAction = TRUE;
							}
 						}


						if ((nErrArrivedSen != ERR_NO_ERROR && nRetPos != ERR_NO_ERROR) || bVibratorForceAction == TRUE) {

							int nUseVibMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_VIBRATOR_OPTION);
							if(nUseVibMode == DEF_ON) // Vibrator Mode 가 Use Mode라면
							{
								int nVibRetryCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_VIBRATOR_RETRY_CNT);
								if (m_nVibCurTryCnt >= nVibRetryCnt)   // Vibrator를 Retry 횟수만큼 실행한 상태 // Retouch Mode 에 따라 분기
								{
									m_nVibCurTryCnt = 0;
									m_bVib_Retry_Complete = TRUE;
									CParamList AlarmData;
									AlarmData.m_fnAddItemFormat("Test PP %d before the %s, Station =%d, Site =%s", m_nTestPpIdx + 1, m_strPickPlace, m_nPressIdx + 1, strErr);

									if ((int)vErrHistory.size() > 0) {
										for (int i = 0; i < (int)vErrHistory.size(); i++) {
											AlarmData.m_fnAddItemFormat(vErrHistory[i]);
										}
									}

									m_BackAlarmData = AlarmData;
									m_nStep = 31000;
									//ReportAlarm((m_nCmdPick == DEF_PICK ? ERR_TEST_PP_1_SITE_ARRIVED_SENSOR_PICK : ERR_TEST_PP_1_SITE_ARRIVED_SENSOR_PLACE) + m_nTestPpIdx, AlarmData,/*vErrPocket*/m_vErrPicker);
									break;
								}
								else  //Vibrator 를 더 진행하거나 완료됐거나..
								{
									if (m_bVib_Retry_Complete)
									{
										m_bVib_Retry_Complete = FALSE;
										CParamList AlarmData;
										AlarmData.m_fnAddItemFormat("Test PP %d before the %s, Station =%d, Site =%s", m_nTestPpIdx + 1, m_strPickPlace, m_nPressIdx + 1, strErr);

										if ((int)vErrHistory.size() > 0) {
											for (int i = 0; i < (int)vErrHistory.size(); i++) {
												AlarmData.m_fnAddItemFormat(vErrHistory[i]);
											}
										}
										m_BackAlarmData = AlarmData;
										m_nStep = 31000;
										//ReportAlarm((m_nCmdPick == DEF_PICK ? ERR_TEST_PP_1_SITE_ARRIVED_SENSOR_PICK : ERR_TEST_PP_1_SITE_ARRIVED_SENSOR_PLACE) + m_nTestPpIdx, AlarmData,/*vErrPocket*/m_vErrPicker);
										break;
									}
									else
									{
										m_nBackVibratorStep = m_nStep;
										m_nVibCurTryCnt++;
										m_nStep = do_Vibrator_On; // Vibrator 동작 Case										
										break;
									}
								}
							}
							else {
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat("Test PP %d before the %s, Station =%d, Site =%s", m_nTestPpIdx + 1, m_strPickPlace, m_nPressIdx + 1, strErr);

								if ((int)vErrHistory.size() > 0) {
									for (int i = 0; i < (int)vErrHistory.size(); i++) {
										AlarmData.m_fnAddItemFormat(vErrHistory[i]);
									}
								}
								m_BackAlarmData = AlarmData;
								m_nStep = 31000;
								break;
							}
						}
						
						pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Arrived Sensor Check](END)", m_szCmdName, m_nStep);
					}
				}
			}

			if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Manual)
			{
				if (m_cCmdManualPickPlace == DEF_PICK || m_cCmdManualPickPlace == DEF_PLACE) {				
					m_nLast_Executing_Step = m_nStep = 40000;
					pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X%d Axis Move to Manual m_cCmdManualPickPlace %s Pos](END)", m_szCmdName, m_nStep, m_nHandType + 1, pTaskTestPP->GetLocName(m_nCmdStage));
					break;
				}
			}
			
// 			if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto)
// 			{
// 				if (m_nCmdPick == DEF_PICK && m_nCmdStage >= LOC_IDX_STATION_1 && m_nCmdStage <= LOC_IDX_STATION_4)
// 				{
// 					// Auto Retest mode 사용 중일 경우 1차 retest bin을 들었으면 Y축 빼지 않고 대기
// 					BOOL bAvoid = pTaskTestPP->ChkPkrArt1StBin(m_vCmdPicker);
// 					if (bAvoid == TRUE)
// 					{
// 						m_nLast_Executing_Step = m_nStep = 50000;
// 						break;
// 					}
// 				}
// 			}		

			//Grr Mode & Picker 1ea이면 Swap 진행 중.
			//Swap 할 경우, X Safety로 안 보낸다.
			BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
			if (bAutoGrrUse == TRUE && m_vCmdPicker.size() == 1)
			{
				m_nStep = 50000;
				break;
			}

			double dTPosHome[CTaskTestPP::eAxisMax] = { 0, };
			
			int SiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
			int SiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

			std::vector<CPoint> vPicker;
			vPicker.push_back(CPoint(0, 0));
			CPoint cPocket(0, 0);

			int nLocEndPos = eLocIdxTestPP::LOC_IDX_TEST_PP_HOME;

			//4Para의 경우			
// 			if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto && m_nCmdStage >= LOC_IDX_STATION_1 && m_nCmdStage <= LOC_IDX_STATION_4 && (SiteCntX * SiteCntY) == STATION_MAX_PARA / 2)
// 			{
// 				cPocket = CPoint(0, 1);
// 				if (m_nHandType == eTestPPLoadType) // Load Hand가 Station에 Place 하고 나오는 상황
// 				{
// 					int nLoadTableIdx = m_nTestPpIdx == eTestPP_1 ? eLoadTbl_2 : eLoadTbl_3;
// 					BOOL bRearDeviceExist = g_TaskLoadTbl[nLoadTableIdx].GetLoadDeviceRearFrontExist(DEF_REAR);     // Table 2열에 Device 유/무 체크
// 					BOOL bfrontDeviceExist = g_TaskLoadTbl[nLoadTableIdx].GetLoadDeviceRearFrontExist(DEF_FRONT);	 // Table 4열에 Device 유/무 체크
// 
// 					nLocEndPos = eLocIdxTestPP::LOC_IDX_TEST_PP_TBL_1_LD + (m_nCmdStage - LOC_IDX_STATION_1);
// 
// 					if (bRearDeviceExist == FALSE && bfrontDeviceExist == TRUE) // Table 2열에 Device 없고, 4열에 Device 있으면, 미리 4열로 보낸다.
// 					{						
// 						cPocket = CPoint(0, 3);
// 					}
// 				}
// 				else {								// Unload Hand가 Station에 Pick 하고 나오는 상황
// 					nLocEndPos = eLocIdxTestPP::LOC_IDX_TEST_PP_TBL_1_UD + (m_nCmdStage - LOC_IDX_STATION_1);
// 				}					
// 			}
// 			else if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto && m_nCmdStage >= LOC_IDX_TEST_PP_TBL_1_LD && m_nCmdStage <= LOC_IDX_TEST_PP_TBL_4_UD
// 				&& (SiteCntX * SiteCntY) == STATION_MAX_PARA / 2) //CmdStage가 Table인 상황은 Home 위치로 보내지 않는다. 어차피 현재 Table 위치에 있기 때문에
// 			{
// 				m_nLast_Executing_Step = m_nStep++;
// 				break;
// 			}
					
			int nAxisNo[3] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2,
				m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_X1_SW_Y1 : CTaskTestPP::eAxisPitch_HW_X2_SW_Y2,
				m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_Y1_SW_X1 : CTaskTestPP::eAxisPitch_HW_Y2_SW_X2};

			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X%d Pitch, Y%d Pitch , X%d move %s safety pos](STR)", m_szCmdName, m_nStep, m_nHandType + 1, m_nHandType + 1, m_nHandType + 1, m_strStageName);
				
			for (int i = 0; i < _countof(nAxisNo); i++) {
				int nErr = pTaskTestPP->do_MovePos(nLocEndPos, m_nHandType, cPocket, vPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i]);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d X%d axis home move command,but this was wrong", m_nTestPpIdx + 1, m_nHandType + 1);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}
			m_tcTimeout = (double)GetTickCount64();
			m_nLast_Executing_Step = m_nStep++;		
		}break;
	case 22001:	// done move y home
		{
			int nAxisNo[3] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2,
			m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_X1_SW_Y1 : CTaskTestPP::eAxisPitch_HW_X2_SW_Y2,
			m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_Y1_SW_X1 : CTaskTestPP::eAxisPitch_HW_Y2_SW_X2};

			int nChk = 0;
			for (int i = 0; i < _countof(nAxisNo); i++) {
				int nAxisDone = m_pMotor[i]->AxisDone();
				if (nAxisDone == ERR_NO_ERROR) {
					
					nChk++;
				}
				else
				{
					double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
					if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d X%d Axis Home Position move time over", m_nTestPpIdx + 1, m_nHandType + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s", m_pMotor[i]->GetAxisName());
						ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
						break;
					}

					if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d Y axis done fail, move home position", m_nTestPpIdx + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s", m_pMotor[i]->GetAxisName());
						ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPpIdx, AlarmData);
					}
				}
			}
			if (nChk == _countof(nAxisNo))
			{
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X%d Pitch, Y%d Pitch , X%d move %s safety pos](END)", m_szCmdName, m_nStep, m_nHandType + 1, m_nHandType + 1, m_nHandType + 1, m_strStageName);
				m_nStep = 40000;
				break;
			}
		}break;
	case 40000:
	{
		BOOL bDeviceExistCheck = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TestPP_DevExist_Check);
		BOOL bVisionUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_VisionUse);
		int  nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);

		if (m_nCmdStage >= LOC_IDX_STATION_1 && m_nCmdStage <= LOC_IDX_STATION_4)
		{
			if (bVisionUseMode == DEF_ON /*&& nDeviceMode == eDeviceMode*/)
			{
// 				if (bDeviceExistCheck == TRUE && m_nCmdPick == DEF_PICK && pTaskTestPP->ChkVacuum(DEF_ON, m_nHandType, m_vCmdPicker) != ERR_NO_ERROR) {
// 					//m_nStep = 6500;	//vision Exist
// 					m_tcTimeout = (double)GetTickCount64();
// 					break;
// 				}
				if (m_nCmdPick == DEF_PICK && m_nHandType == eTestPPUnLoadType) { // DEF_PLACE
					m_nStep = 41000;	//vision Empty
					m_tcTimeout = (double)GetTickCount64();
					break;
				}
			}
		}

		m_tcTimeout = (double)GetTickCount64();
		m_nStep = 50000;	// not check vision
	}
	break;
	// === VISION EMPTY CHECK <START>
// 	case  41000:	// move X1 or X2 vision position ( hand 구조 상 좌표에 rear L1 picker가 와야함 )
// 	{
// 		if (m_nCmdStage >= LOC_IDX_STATION_1 && m_nCmdStage <= LOC_IDX_STATION_4)
// 		{
// 			int nPressIdx = pTaskTestPP->GetPressNum(m_nCmdStage);
// 
// 			double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
// 			if (dSpanTime > MOTOR_TIME_OUT_SHORT*1000.0) {
// 				CParamList AlarmData;
// 				AlarmData.m_fnAddItemFormat("Test PP %d Y Axis can not move station [WorkStage=%s not safety]", m_nTestPpIdx + 1, m_strStageName);
// 				ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + nPressIdx, AlarmData);
// 				break;
// 			}
// 
// 			if (g_TaskPressUnit[nPressIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
// 				break;
// 		}
// 
// 		std::vector<CPoint> vCmdPicker;
// 		vCmdPicker.push_back(CPoint(0, 0));
// 		CPoint cCmdPocket;
// 		cCmdPocket.x = 0;
// 		cCmdPocket.y = 0;
// 
// 		pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X%d Axis Move to %s 1,1 Pos](STR)", m_szCmdName, m_nStep, m_nHandType + 1, m_strStageName);
// 
// 		int nAxisNo[1] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2 };
// 		for (int i = 0; i < _countof(nAxisNo); i++) {
// 			int nErr = pTaskTestPP->do_MovePos(m_nCmdStage, m_nHandType, cCmdPocket, vCmdPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i]);
// 			if (nErr != ERR_NO_ERROR) {
// 				CParamList AlarmData;
// 				AlarmData.m_fnAddItemFormat("Test PP %d y axis target position command,but this was wrong [CmdStage=%s]", m_nTestPpIdx + 1, m_strStageName);
// 				ReportAlarm(nErr, AlarmData);
// 				break;
// 			}
// 		}
// 
// 		m_tcTimeout = (double)GetTickCount64();
// 		m_nStep++;
// 	}break;
// 	case 40001:	// check done move X target position
// 	{
// 		int nAxisNo[1] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2 };
// 		for (int i = 0; i < _countof(nAxisNo); i++)
// 		{
// 			OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo[i]);
// 			int nAxisDone = pMotor->AxisDone();
// 			if (nAxisDone == ERR_NO_ERROR) {
// 				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X%d Axis Move to %s Pos](END)", m_szCmdName, m_nStep, m_nHandType + 1, m_strStageName);
// 				m_nStep++;
// 
// 				m_tcTimeout = (double)GetTickCount64();
// 			}
// 			else
// 			{
// 				double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
// 				if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
// 					CParamList AlarmData;
// 					AlarmData.m_fnAddItemFormat("Test PP %d X%d Axis move time over, move y target position", m_nTestPpIdx + 1, m_nHandType + 1);
// 					AlarmData.m_fnAddItemFormat("[WorkStage=%s]", m_strStageName);
// 					ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
// 					break;
// 				}
// 
// 				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
// 				{
// 					CParamList AlarmData;
// 					AlarmData.m_fnAddItemFormat("Test PP %d %d axis done fail [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_strStageName);
// 					ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPpIdx, AlarmData);
// 				}
// 			}
// 		}
// 
// 	}break;
	case 41000:	// vision empty send measure
	{
		m_tVisionMsgMeasure.clear();

		int nErr = -1;
		pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Vision ReqMeasure](STR)", m_szCmdName, m_nStep);

		if (m_nPressIdx < eMaxPressUnitCount / 2)
			nErr = g_pVisionTestHand_1->Cmd_ReqMeasure_StationCam(m_nPressIdx, &m_tVisionMsgMeasure, m_vCmdPicker, m_cCmdPocket, m_nPressIdx);
		else //if (m_nTestPpIdx == eTestPP_2)
			nErr = g_pVisionTestHand_2->Cmd_ReqMeasure_StationCam(m_nPressIdx, &m_tVisionMsgMeasure, m_vCmdPicker, m_cCmdPocket, m_nPressIdx);

		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Test PP %d Vision Empty Measure error, Press Unit=%d", m_nTestPpIdx + 1, m_nPressIdx + 1);
			ReportAlarm(nErr, AlarmData);
			break;
		}
		m_tcTimeout = (double)GetTickCount64();
		m_nStep++;
	}break;
	case 41001:
	{
		double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
		if (dSpanTime > VISION_TIME_OUT*1000.0)
		{
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("vision wait result time over, Test hand = %d", m_nTestPpIdx + 1);
			ReportAlarm(ERR_TEST_PP1_VISION_CMD_RESP_TIME_OUT + m_nTestPpIdx, AlarmData);
			break;
		}
		int nCmdDone = eVisionMsgWaitState_Waiting;
		if (m_nPressIdx < eMaxPressUnitCount / 2)
			nCmdDone = g_pVisionTestHand_1->GetCmdDone(m_nPressIdx);
		else //if (m_nTestPpIdx == eTestPP_2)
			nCmdDone = g_pVisionTestHand_2->GetCmdDone(m_nPressIdx);

		if (nCmdDone == eVisionMsgWaitState_Received)
		{
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Vision Received Result](STR)", m_szCmdName, m_nStep);
			if (m_nRetryCount > MAX_VISION_RETRY_COUNT)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test hand = %d", m_nTestPpIdx + 1);
				ReportAlarm(ERR_TEST_PP1_VISION_CMD_RETRY_COUNT_OVER + m_nTestPpIdx, AlarmData);
				break;
			}

			// 수신된 데이터 수가 맞지 않을 경우 alarm 발생. 2개 이어야 함. (busy, result)
			int nRevData[MAX_VISION_RESULT_DATA_COUNT] = { 0, };

			CString strPosResult = _T("");
			if (m_nPressIdx < eMaxPressUnitCount / 2)
				nCmdDone = g_pVisionTestHand_1->GetCmdResultData(m_nPressIdx, (int*)nRevData, strPosResult);
			else //if (m_nTestPpIdx == eTestPP_2)
				nCmdDone = g_pVisionTestHand_2->GetCmdResultData(m_nPressIdx, (int*)nRevData, strPosResult);

			if (nRevData[eResData_DataCnt] != (MAX_VISION_RESULT_DATA_COUNT - 2))
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test hand = %d, Data Count = %d", m_nTestPpIdx + 1, nRevData[eResData_DataCnt]);
				int nErrorCode = g_Error.CheckVisionError(nRevData[eResData_Error_Discription]);
				ReportAlarm(nErrorCode + m_nPressIdx, AlarmData);
				//ReportAlarm(ERR_TEST_PP1_VISION_CMD_RESP_DATA_IS_WRONG + m_nTestPpIdx, AlarmData);
				break;
			}

			// vision이 아직 처리 중이면 (busy) 다시 데이터 요청 한다. timeout이내에는 처리 해야 함.
			// 				if( nRevData[eResData_BusyStatus] != eCam_Not_Busy )
			// 				{
			// 					m_nRetryCount++;
			// 					Sleep(100);
			// 					m_nStep--;
			// 					break;
			// 				}
			// vision이 처리 하였지만, ok 처리가 되지 않는 경우에 retry 한다.
			CString strVisionResult;
			if (nRevData[eResData_ResultStatus] == eCamResult_NG)
			{
// 				if (nRevData[eResData_ResultStatus] == eCamResult_NG) {
// 					strVisionResult = _T("Device Exist");
// 				}
// 				else {
// 					strVisionResult.Format("%d", nRevData[eResData_ResultStatus]);
// 				}

				strVisionResult = _T("Device Exist");

				CString strErrMsg = _T("");
				std::vector<CPoint> vFailPocket;
				ZeroMemory(m_bVisionErrSkip, sizeof(m_bVisionErrSkip));
				BOOL bRet = g_TaskSystemCtrl.ChkVisionPosResult(strPosResult, strErrMsg, m_vCmdPicker, m_cCmdPocket, vFailPocket);
				if (bRet == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Station = %d, Site = %s, Vision Result = %s vision Empty result NG!!!!", m_nPressIdx + 1, strErrMsg, strVisionResult);
					ReportAlarm(ERR_TEST_PP1_VISION_RESULT_NG + m_nTestPpIdx, AlarmData, vFailPocket);
					break;
				}
				else {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Station = %d Detail Site Data Error", m_nPressIdx + 1);
					int nErrorCode = g_Error.CheckVisionError(nRevData[eResData_Error_Discription]);
					ReportAlarm(nErrorCode + m_nPressIdx, AlarmData);
					//ReportAlarm(ERR_TEST_PP1_VISION_CMD_RESP_DATA_IS_WRONG + m_nTestPpIdx, AlarmData);
					break;
				}
			}
			else if (nRevData[eResData_ResultStatus] == eCamResult_Busy)
			{
				strVisionResult = _T("Cam Result Busy");
			}
			else {
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Vision Received Result](END)", m_szCmdName, m_nStep);
				m_nStep = 50000;
			}
			//pTaskTestPP->VisionLED_ONOFF(DEF_OFF, m_nTestPpIdx);
		}
		else if (nCmdDone == eVisionMsgWaitState_Canceled)
		{
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Test hand = %d", m_nTestPpIdx + 1);
			ReportAlarm(ERR_TEST_PP1_VISION_CMD_RESP_IS_CANCELED + m_nTestPpIdx, AlarmData);
			break;
		}
	}break;
	case do_Vibrator_On:
	{
		g_TaskPressUnit[m_nPressIdx].do_SideVibratorOnOff(DEF_ON);

		tmr.StartTmChk();

		pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [do_Vibrator_On]", m_szCmdName, m_nStep);
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

		pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [do_Vibrator_On_Wait]", m_szCmdName, m_nStep);
		m_nStep = do_Vibrator_Off;

		break;
	}
	case do_Vibrator_Off:
	{
		g_TaskPressUnit[m_nPressIdx].do_SideVibratorOnOff(DEF_OFF);

		tmr.StartTmChk();
		pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [do_Vibrator_Off]", m_szCmdName, m_nStep);
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

		pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [do_Vibrator_Off_Wait]", m_szCmdName, m_nStep);
		m_nStep = Vibrator_complete;
		break;
	}
	case Vibrator_complete:
	{
		m_nStep = m_nBackVibratorStep;
		break;
	}
	case 31000: //Socket Miss Error시, X Move Safety
	{
		if (m_nCmdStage >= LOC_IDX_STATION_1 && m_nCmdStage <= LOC_IDX_STATION_4)
		{
			int nPressIdx = pTaskTestPP->GetPressNum(m_nCmdStage);

			double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
			if (dSpanTime > MOTOR_TIME_OUT_SHORT*1000.0) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d Y Axis can not move station [WorkStage=%s not safety]", m_nTestPpIdx + 1, m_strStageName);
				ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + nPressIdx, AlarmData);
				break;
			}

			if (g_TaskPressUnit[nPressIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
				break;
		}

		int nAxisNo = m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2;

		std::vector<CPoint> vPicker;
		vPicker.push_back(CPoint(0, 0));
		pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X1,X2 Axis Move to Home Pos](STR)", m_szCmdName, m_nStep);
		int nErr = pTaskTestPP->do_MovePos(LOC_IDX_TEST_PP_HOME, m_nHandType, CPoint(0, 0), vPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Test PP %d y axis move command,but this was wrong", m_nTestPpIdx + 1);
			OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo);
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(nErr, AlarmData);
			break;
		}
		m_tcTimeout = (double)GetTickCount64();
		m_nStep++;
	}break;
	case 31001:
	{
		int nAxisNo = m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2;

		int nAxisDone = m_pMotor[nAxisNo]->AxisDone();
		if (nAxisDone == ERR_NO_ERROR) {

	// 			CString strErr = _T("");
	// 			std::vector<CPoint> vErrPocket;
	// 			vErrPocket.clear();
	// 
	// 			std::vector<CString> vErrHistory;
	// 			int nErrArrivedSen = g_TaskPressUnit[m_nPressIdx].ChkSocketArrivedSen(m_cCmdPocket.y, strErr, vErrPocket, vErrHistory);
	// 			int nRetPos = g_TaskPressUnit[m_nPressIdx].ChkMotorPos(LOC_IDX_PRESS_UNIT_CONTACT, CTaskPressUnit::eAxisZ);
	// 			if (nErrArrivedSen != ERR_NO_ERROR && nRetPos != ERR_NO_ERROR)
	// 			{ 
	// 				CParamList AlarmData;
	// 				AlarmData.m_fnAddItemFormat("Test PP %d before the %s, Station =%d, Site =%s", m_nTestPpIdx + 1, m_strPickPlace, m_nPressIdx + 1, strErr);
	// 
	// 				if ((int)vErrHistory.size() > 0) {
	// 					for (int i = 0; i < (int)vErrHistory.size(); i++) {
	// 						AlarmData.m_fnAddItemFormat(vErrHistory[i]);
	// 					}
	// 				}
	// 
	// 				ReportAlarm((m_nCmdPick == DEF_PICK ? ERR_TEST_PP_1_SITE_ARRIVED_SENSOR_PICK : ERR_TEST_PP_1_SITE_ARRIVED_SENSOR_PLACE) + m_nTestPpIdx, AlarmData,/*vErrPocket*/m_vErrPicker);
	// 			}

			int nDeviceType = pTaskTestPP->CheckDeviceType(m_nCmdStage, m_nHandType, m_cCmdPocket.x, m_cCmdPocket.y, m_vCmdPicker, DEF_PICK);
			int nErr = 0;
			if(m_nCmdPick == DEF_PICK)
				nErr = nDeviceType == eDeviceType::_eDeviceType_Normal ? ERR_TEST_PP_1_SITE_ARRIVED_SENSOR_PICK : ERR_TEST_PP_1_SITE_CLEAN_DEVICE_ARRIVED_SENSOR_PICK;
			else
				nErr = nDeviceType == eDeviceType::_eDeviceType_Normal ? ERR_TEST_PP_1_SITE_ARRIVED_SENSOR_PLACE : ERR_TEST_PP_1_SITE_CLEAN_DEVICE_ARRIVED_SENSOR_PLACE;

			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X%d Move Home](END)", m_szCmdName, m_nStep, m_nHandType+1);
			ReportAlarm(nErr + m_nTestPpIdx, m_BackAlarmData,/*vErrPocket*/m_vErrPicker);
		}
		else 
		{
			double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
			if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d Y Home Position move time over", m_nTestPpIdx + 1);
				ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
				OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				break;
			}

			if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d Y axis done fail, move home position", m_nTestPpIdx + 1);
				OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPpIdx, AlarmData);
				break;
			}
		}

	}break;
	case 50000:
		{
 			//Test Hand가 작업 완료 된 상태 
 			if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto) {
 				if (m_nPressIdx != 0) {
 					int a = 0;
 				}
 				if (m_nCmdPick == DEF_PICK && m_nHandType == eTestPPLoadType && (LOC_IDX_TEST_PP_TBL_1_LD <= m_nCmdStage && m_nCmdStage <= LOC_IDX_TEST_PP_TBL_4_LD))
 				{
 					int nIdx = m_nCmdStage - LOC_IDX_TEST_PP_TBL_1_LD + (4 * m_nTestPpIdx);
 					g_TaskPressUnit[nIdx].SetPressReqTable(FALSE, CTaskLoadTable::eLoadType);
 				}
 				else if (m_nCmdPick == DEF_PLACE && m_nHandType == eTestPPUnLoadType && (LOC_IDX_TEST_PP_TBL_1_UD <= m_nCmdStage && m_nCmdStage <= LOC_IDX_TEST_PP_TBL_4_UD))
 				{
 					int nIdx = m_nCmdStage - LOC_IDX_TEST_PP_TBL_1_UD + (4 * m_nTestPpIdx);
 					g_TaskPressUnit[nIdx].SetPressReqTable(FALSE, CTaskLoadTable::eUnloadType);
					if (g_TaskSystemCtrl.IsOneCycleModeOn() == false) {
						g_TaskPressUnit[nIdx].SetPressReqTable(FALSE, CTaskLoadTable::eLoadType);
					}
 				}
 			}			
			ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
		}break;
	}
	return CBaseSeqCmd::State_Executing();
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_MovePickPlace::State_Pause()
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
				//pMotor->SetSlowStop();
				bool bCloseLimit = pMotor->CheckLimitClose();
				double dCurPos = 0;
				pMotor->GetCurrentPos(dCurPos);
				if (bCloseLimit == false)
				{
					pTaskTestPP->MakeLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
					pMotor->SetSlowStop();		
				}
			}

			//SetSlowStop 전, Device Data 처리.
			if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto && pTaskTestPP->m_bCyclePause == TRUE)
			{
				int nBlowDoneCnt = 0;
				int nHandData = 0;
				for (int i = 0; i < (int)m_vCmdPicker.size(); i++) {
					//Blow Done Count Check.
					if (pTaskTestPP->ChkVacuum(DEF_OFF, m_nHandType, m_vCmdPicker[i]) == ERR_NO_ERROR) { //Blow Retry						
						nBlowDoneCnt++;
					}

					//Hand Device Data Check.
					ST_DD_DEVICE stHandData;
					pTaskTestPP->GetHandDvcData(m_vCmdPicker[i], m_nHandType, &stHandData);
					if (stHandData.sExist == DEF_EXIST) {
						nHandData++;
					}
				}

				//1. Place일 경우.
				//2. Blow 진행 했을 경우.
				//3. Hand Device Data 존재 할 경우.

				std::vector<CPoint> vFailPicker;
				vFailPicker.clear();
				if (m_nCmdPick == DEF_PLACE && nBlowDoneCnt == m_vCmdPicker.size() && nHandData == m_vCmdPicker.size())
				{
					vFailPicker = pTaskTestPP->GetFailPicker(m_nHandType);
					std::vector<CString> vErrString; vErrString.clear();
					int nDeviceType = 0;
					int nRet = pTaskTestPP->ProcDeviceData(m_nCmdStage, m_nHandType, m_cCmdPocket.x, m_cCmdPocket.y, m_vCmdPicker, DEF_PLACE, vErrString, nDeviceType);
					pTaskTestPP->m_bCyclePause = FALSE;
					pTaskTestPP->MakeLog("[Before Stop][ProcDevice Data Place] CmdStage=%s, CmdTargetX=%d, CmdTargetY=%d, CmdPickPicker=%s", pTaskTestPP->GetLocName(m_nCmdStage), m_cCmdPocket.x + 1, m_cCmdPocket.y + 1, m_strWorkPkrName);

					if (nRet != ERR_NO_ERROR) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d CmdStage=%s", m_nTestPpIdx + 1, m_strStageName);
						ReportAlarm(nRet, AlarmData, vFailPicker);
						break;
					}
					pTaskTestPP->MakeLog("[%s][Before Stop][EXEC=%8d] [%s](END)", m_szCmdName, m_nStep, (m_nCmdPick == DEF_PICK ? _T("Vacuum") : _T("Blow")));

					m_nLast_Executing_Step = 20000;
				}
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
BOOL CTestPPCmd_MovePickPlace::State_Recovery()
{
	CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		//pTaskTestPP->MakeLog("[CMD: %s] [RECOVERY] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	int i=0;
	for( i=0; i<CTaskTestPP::eAxisMax; i++){
		OneAxis* pMotor = pTaskTestPP->GetAxis(i);
		if( pMotor->ChkMotorUsable() != ERR_NO_ERROR ){
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("%s is not usable, please check", pMotor->GetAxisName());
			ReportAlarm(ERR_TEST_PP_1_MOTOR_ALARMED + m_nTestPpIdx, AlarmData);
			return FALSE;
		}
	}

	switch( m_nStep )
	{
	case 0:
		{
			double dErr[CTaskTestPP::eAxisMax]  = { 0, };
			double dTPos[CTaskTestPP::eAxisMax] = { 0, };
			pTaskTestPP->GetTeachPos(m_nCmdStage, m_cCmdPocket, m_vCmdPicker, CTaskTestPP::eZPOS_SAFETY, dTPos, m_nHandType, m_bCleanOffsetUse);


			double dCurPos[CTaskTestPP::eAxisMax] = { 0, };

			for (int i = 0; i < CTaskTestPP::eAxisMax; i++)
			{
				m_pMotor[i]->GetCurrentPos(dCurPos[i]);
			}
			
			// Teaching 위치와 현재 위치가 일치 하는지 확인.

			dTPos[CTaskTestPP::eAxisY] = 0.0;
		
			dErr[CTaskTestPP::eAxisX1] = fabs(dCurPos[CTaskTestPP::eAxisX1] - dTPos[CTaskTestPP::eAxisX1]);
			dErr[CTaskTestPP::eAxisZ1] = fabs(dCurPos[CTaskTestPP::eAxisZ1] - dTPos[CTaskTestPP::eAxisZ1]);
			dErr[CTaskTestPP::eAxisPitch_HW_X1_SW_Y1] = fabs(dCurPos[CTaskTestPP::eAxisPitch_HW_X1_SW_Y1] - dTPos[CTaskTestPP::eAxisPitch_HW_X1_SW_Y1]);
			dErr[CTaskTestPP::eAxisPitch_HW_Y1_SW_X1] = fabs(dCurPos[CTaskTestPP::eAxisPitch_HW_Y1_SW_X1] - dTPos[CTaskTestPP::eAxisPitch_HW_Y1_SW_X1]);

			dErr[CTaskTestPP::eAxisX2] = fabs(dCurPos[CTaskTestPP::eAxisX2] - dTPos[CTaskTestPP::eAxisX2]);
			dErr[CTaskTestPP::eAxisZ2] = fabs(dCurPos[CTaskTestPP::eAxisZ2] - dTPos[CTaskTestPP::eAxisZ2]);
			dErr[CTaskTestPP::eAxisPitch_HW_X2_SW_Y2] = fabs(dCurPos[CTaskTestPP::eAxisPitch_HW_X2_SW_Y2] - dTPos[CTaskTestPP::eAxisPitch_HW_X2_SW_Y2]);
			dErr[CTaskTestPP::eAxisPitch_HW_Y2_SW_X2] = fabs(dCurPos[CTaskTestPP::eAxisPitch_HW_Y2_SW_X2] - dTPos[CTaskTestPP::eAxisPitch_HW_Y2_SW_X2]);

			dErr[CTaskTestPP::eAxisY] = fabs(dCurPos[CTaskTestPP::eAxisY] - dTPos[CTaskTestPP::eAxisY]);


			int nChkTeachSame = 0;
			for( int i=0; i < CTaskTestPP::eAxisMax; i++ )
			{
				if( dErr[i] <= m_dInPosRange[i] )
					nChkTeachSame++;
			}

			// 축 이동이 완료 되어진 상태 이므로 motor 동작이 필요 없음.
			if( nChkTeachSame == CTaskTestPP::eAxisMax ){
				m_nStep = 10000; 
			}else{
				m_nStep = 1000;
				m_nRetryCount = 0;
			}
		}break;
	case 1000:   // Z Axis All picker up
		{
			pTaskTestPP->MakeLog("[%s] [RECOVERY=%8d] [Z1,Z2 move %s safety pos](STR)", m_szCmdName, m_nStep, m_strStageName);
			int nAxisNo[2] = { CTaskTestPP::eAxisZ1 ,CTaskTestPP::eAxisZ2 };
			std::vector<CPoint> vPkr;
			pTaskTestPP->MakeAllPicker(vPkr);
			for (int i = 0; i < _countof(nAxisNo); i++)
			{
				int nErr = pTaskTestPP->do_MovePos(eLocIdxTestPP::LOC_IDX_TEST_PP_HOME, i, m_cCmdPocket, vPkr, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i]);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d all picker up command, but this was wrong ", m_nTestPpIdx + 1);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}				
			}
			m_tcTimeout = (double)GetTickCount64();
			m_nStep++;
		}break;
	case 1001:  //  Z Axis All check picker all up
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

					double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
					if (dSpanTime > TIME_OUT_SHORT*1000.0)
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
				pTaskTestPP->MakeLog("[%s] [RECOVERY=%8d] [Z Axis Move to Safety Pos](END)", m_szCmdName, m_nStep);
			}
		
		}break;
	case 2000: // check z safety and X1 or x2 home
		{
			pTaskTestPP->MakeLog("[%s] [RECOVERY=%8d] [X%d Axis Move to Safety Pos Check]", m_szCmdName, m_nStep, m_nHandType+1);
			std::vector<CPoint> vTempPkr;
			vTempPkr.push_back(CPoint(0, 0));
			double dCurPosX1 = 0;
			m_pMotor[CTaskTestPP::eAxisX1]->GetCurrentPos(dCurPosX1);
			double dCurPosX2 = 0;
			m_pMotor[CTaskTestPP::eAxisX2]->GetCurrentPos(dCurPosX2);

			double dTPosHome[CTaskTestPP::eAxisMax] = { 0, };
			pTaskTestPP->GetTeachPos(LOC_IDX_TEST_PP_HOME, CPoint(0, 0), vTempPkr, CTaskTestPP::eZPOS_SAFETY, dTPosHome, m_nHandType, m_bCleanOffsetUse);

			int nSafetySenX1 = pTaskTestPP->ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON);
			int nSafetySenX2 = pTaskTestPP->ChkDI(CTaskTestPP::xX2AXIS_POSITION_SEN, DEF_ON);
			// home 위치보다 크거나 안전 센서가 감지 안되면	 X1,X2축 Home
			if (dTPosHome[CTaskTestPP::eAxisX1] + 1000.0 < dCurPosX1 || nSafetySenX1 != ERR_NO_ERROR || 0 > dCurPosX1 ||
				dTPosHome[CTaskTestPP::eAxisX2] + 1000.0 < dCurPosX2 || nSafetySenX2 != ERR_NO_ERROR || 0 > dCurPosX2)
			{
				m_tcTimeout = (double)GetTickCount64();
				m_nStep = 3000;
			}
			// 그 외에 경우에는 그냥 움직이면 된다.
			else {
				m_tcTimeout = (double)GetTickCount64();
				m_nStep = 4000;
			}
		}break;
	case 3000: //X1, X2 move Home
		{
			if (m_nCmdStage >= LOC_IDX_STATION_1 && m_nCmdStage <= LOC_IDX_STATION_4)
			{
				int nPressIdx = pTaskTestPP->GetPressNum(m_nCmdStage);

				double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
				if (dSpanTime > MOTOR_TIME_OUT_SHORT*1000.0) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d Y Axis can not move station [WorkStage=%s not safety]", m_nTestPpIdx + 1, m_strStageName);
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + nPressIdx, AlarmData);
					break;
				}

				if (g_TaskPressUnit[nPressIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
					break;
			}

			int nAxisNo[2] = { CTaskTestPP::eAxisX1 ,CTaskTestPP::eAxisX2 };
			std::vector<CPoint> vPicker;
			vPicker.push_back(CPoint(0, 0));
			pTaskTestPP->MakeLog("[%s] [RECOVERY=%8d] [X1,X2 Axis Move to Home Pos](STR)", m_szCmdName, m_nStep);
			for (int i = 0; i < _countof(nAxisNo); i++)
			{
				int nErr = pTaskTestPP->do_MovePos(LOC_IDX_TEST_PP_HOME, m_nHandType, m_cCmdPocket, m_vCmdPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i],m_bCleanOffsetUse);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d y axis move command,but this was wrong", m_nTestPpIdx + 1);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}
			m_tcTimeout = (double)GetTickCount64();
			m_nStep++;
		}break;
	case 3001: // check done X1, X2 move Home
		{
			OneAxis* pMotorX1 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisX1);
			OneAxis* pMotorX2 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisX2);

			OneAxis* pMotor[2] = { pMotorX1 ,pMotorX2 };
			int nComp = 0;
			for (int i = 0; i < _countof(pMotor); i++)
			{
				int nAxisDone = pMotor[i]->AxisDone();
				if (nAxisDone == ERR_NO_ERROR) {
					nComp++;
				}
				else
				{
					double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
					if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d Y Home Position move time over", m_nTestPpIdx + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
						ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
						break;
					}

					if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d Y axis done fail, move home position", m_nTestPpIdx + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
						ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPpIdx, AlarmData);
					}
				}
			}

			if (nComp == _countof(pMotor))
			{
				m_nStep = 4000;
				pTaskTestPP->MakeLog("[%s] [RECOVERY=%8d] [X1,X2 Move Home](END)", m_szCmdName, m_nStep);
			}
		}break;
	case 4000: // // move X1,Y1 or X2,Y2 Pitch, Y target position 
		{
			if (m_nCmdStage >= LOC_IDX_TEST_PP_TBL_1_LD && m_nCmdStage <= LOC_IDX_TEST_PP_TBL_4_UD)
			{
				int nUseTableAlignTeaching = g_DMCont.m_dmEQP.GN(NDM0_TABLE_ALIGN_TEACHING);
				if (nUseTableAlignTeaching != TRUE)
				{
					pTaskTestPP->MakeLog("[%s] [RECOVERY=%8d] [Check Table Move] [STR]", m_szCmdName, m_nStep);
					int table_step = 0;
					BOOL bNeedTblMove = pTaskTestPP->ChkTableMove(m_nHandType, m_nCmdStage, m_cCmdPocket, m_vCmdPicker, table_step);
					if (bNeedTblMove == TRUE) {
						pTaskTestPP->m_bCalBasePoint = TRUE;//GetTeachPos에서 사용.

						int nIdx = pTaskTestPP->FromTestPPToLoadTableIdx(m_nCmdStage);
						g_TaskLoadTbl[nIdx].SetTableColStep(table_step);
					}
					pTaskTestPP->MakeLog("[%s] [RECOVERY=%8d] [Check Table Move] [END] [Table Step=%d] [Cal Base Point=%s]", m_szCmdName, m_nStep, table_step, (pTaskTestPP->m_bCalBasePoint == TRUE) ? _T("TRUE") : _T("FALSE"));
				}
			}

			int nAxisNo[3] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_X1_SW_Y1 : CTaskTestPP::eAxisPitch_HW_X2_SW_Y2,
				m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_Y1_SW_X1 : CTaskTestPP::eAxisPitch_HW_Y2_SW_X2,
				CTaskTestPP::eAxisY};

			pTaskTestPP->MakeLog("[%s] [RECOVERY=%8d] [X%d Pitch, Y%d Pitch , Y move %s safety pos](STR)", m_szCmdName, m_nStep, m_nHandType + 1, m_nHandType + 1, m_strStageName);
			for (int i = 0; i < _countof(nAxisNo); i++) {
				int nErr = pTaskTestPP->do_MovePos(m_nCmdStage, m_nHandType, m_cCmdPocket, m_vCmdPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i],m_bCleanOffsetUse);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d X%d Pitch, Y%d Pitch, Y target position command,but this was wrong [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_nHandType + 1, m_strStageName);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}

			m_tcTimeout = (double)GetTickCount64();
			m_nStep++;
		}break;
	case 4001: // check done move X1,Y1 or X2,Y2 Pitch Y target position
		{
			int nChkComp = 0;
			int nAxisNo[3] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_X1_SW_Y1 : CTaskTestPP::eAxisPitch_HW_X2_SW_Y2,
				m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_Y1_SW_X1 : CTaskTestPP::eAxisPitch_HW_Y2_SW_X2,
				CTaskTestPP::eAxisY};

			for (int i = 0; i < _countof(nAxisNo); i++) {
				OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo[i]);
				int nAxisDone = pMotor->AxisDone();
				if (nAxisDone == ERR_NO_ERROR) {
					nChkComp++;
				}
				else 
				{
					double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
					if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d, X%dPitch, Y%dPitch Axis move time over [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_nHandType + 1, m_strStageName);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
						break;
					}

					if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d axis done fail, X%dPitch, Y%dPitch, Y move target position [Axis=%s] [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_nHandType + 1, pMotor->GetAxisName(), m_strStageName);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPpIdx, AlarmData);
						break;
					}
				}
			}
			if (nChkComp == _countof(nAxisNo)) {
				m_nStep = 10000;
				pTaskTestPP->MakeLog("[%s] [RECOVERY=%8d] [X%d Pitch, Y%d Pitch, Y move %s target pos](END)", m_szCmdName, m_nStep, m_nHandType + 1, m_nHandType + 1, m_strStageName);
			}
		}break;
	case 10000:
		{
			// 10000번 ~ 20000번  사이일 경우에는 Y축도 제 위치로 보내주어야 한다.
			if( m_nLast_Executing_Step == 10000 )
			{
				m_nStep++;
			}else{
				m_nStep = 20000;
			}
		}break;
	case 10001:
		{
			if (m_nCmdStage >= LOC_IDX_STATION_1 && m_nCmdStage <= LOC_IDX_STATION_4)
			{
				m_tcTimeout = (double)GetTickCount64();
				m_nStep = 10002;
				break;
			}
			else
			{

				if (m_nCmdStage >= LOC_IDX_TEST_PP_TBL_1_LD && m_nCmdStage <= LOC_IDX_TEST_PP_TBL_4_UD)
				{
					int nLoadTblIdx = pTaskTestPP->FromTestPPToLoadTableIdx(m_nCmdStage);
					int nLoadTblCmdStage = pTaskTestPP->FromTestPPToLoadTableCmdStageConvetor(m_nCmdStage);

					int table_col_step = g_TaskLoadTbl[nLoadTblIdx].GetTableColStep();
					int nRetLoadTblPos = g_TaskLoadTbl[nLoadTblIdx].ChkMotorPos(nLoadTblCmdStage, CTaskLoadTable::eAxisAll, table_col_step);
					pTaskTestPP->MakeLog("[Table Col Step : %d] [ChkMotorPos Ret : %d]", table_col_step, nRetLoadTblPos);
					g_TaskLoadTbl[nLoadTblIdx].SetTestppPrePos(nLoadTblCmdStage);

				
					if (nRetLoadTblPos == ERR_NO_ERROR && g_TaskLoadTbl[nLoadTblIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
					{
						if (nLoadTblIdx == eLoadTbl_1 || nLoadTblIdx == eLoadTbl_4)
						{
							g_DMCont.m_dmTestPP.SB(NDM04_TESTHAND1_ALREADY_PICK_UNLOAD_TABLE_CALL + m_nTestPpIdx, FALSE);
						}
						else {
							g_DMCont.m_dmTestPP.SB(NDM04_TESTHAND1_ALREADY_PICK_TABLE_CALL + m_nTestPpIdx, FALSE);
						}
					
						m_nStep = 10003;
						break;
					}
					else{ // Load Table Already pick 상황만 처리
// 						if (m_bAlreadyPick == TRUE && nRetLoadTblPos != ERR_NO_ERROR && g_TaskLoadTbl[nLoadTblIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
// 						{
// 							if (m_nCmdStage >= LOC_IDX_TEST_PP_TBL_1_LD  && m_nCmdStage <= LOC_IDX_TEST_PP_TBL_4_LD)
// 							{
// 								//int nWorkIndex = m_nCmdStage - LOC_IDX_TEST_PP_TBL_1_LD;
// 								g_DMCont.m_dmTestPP.SB(NDM04_TESTHAND1_ALREADY_PICK_TABLE_CALL + m_nTestPpIdx, TRUE);
// 							}						
// 						}
// 						else if (nRetLoadTblPos != ERR_NO_ERROR && g_TaskLoadTbl[nLoadTblIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
// 						{
// 							if (m_nCmdStage >= LOC_IDX_TEST_PP_TBL_1_LD  && m_nCmdStage <= LOC_IDX_TEST_PP_TBL_4_UD)
// 							{
// 								//int nWorkIndex = m_nCmdStage - LOC_IDX_TEST_PP_TBL_1_LD;
// 								g_DMCont.m_dmTestPP.SB(NDM04_TESTHAND1_ALREADY_PICK_UNLOAD_TABLE_CALL + m_nTestPpIdx, TRUE);
// 							}
// 						}
						if (nRetLoadTblPos != ERR_NO_ERROR && g_TaskLoadTbl[nLoadTblIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
						{
							if (nLoadTblIdx == eLoadTbl_1 || nLoadTblIdx == eLoadTbl_4)
							{
								g_DMCont.m_dmTestPP.SB(NDM04_TESTHAND1_ALREADY_PICK_UNLOAD_TABLE_CALL + m_nTestPpIdx, TRUE);
							}	
							else {
								g_DMCont.m_dmTestPP.SB(NDM04_TESTHAND1_ALREADY_PICK_TABLE_CALL + m_nTestPpIdx, TRUE);
							}
						}

					}
				}
				else
				{
					m_nStep = 10003;
					break;
				}				
			}			
		}break;
	case 10002:
		{
			double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
			if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d move time over, wait press unit safety", m_nTestPpIdx + 1);
				CBaseSeqCmd::ReportAlarm(ERR_PRESS_UNIT_1_NOT_DETECT_SAFETY_SEN + m_nPressIdx, AlarmData);
				break;
			}

			int nErr = ERR_NO_ERROR;
			if (g_DMCont.m_dmEQP.GB(BDM0_MD_VIBRATION_MODE) == DEF_ON) {
				nErr = g_TaskPressUnit[m_nPressIdx].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON);
			}
			
			if (nErr == ERR_NO_ERROR && g_TaskPressUnit[m_nPressIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready) {
				m_nStep = 10003;
			}
		}break;
	case 10003: // move X1 or X2 target position
		{
			int nAxisNo[1] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2 };
			pTaskTestPP->MakeLog("[%s] [RECOVERY=%8d] [X%d move %s Target pos](STR)", m_szCmdName, m_nStep, m_nHandType + 1, m_strStageName);
			for (int i = 0; i < _countof(nAxisNo); i++) {
				int nErr = pTaskTestPP->do_MovePos(m_nCmdStage, m_nHandType, m_cCmdPocket, m_vCmdPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i],m_bCleanOffsetUse);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d X%d target position command,but this was wrong [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_strStageName);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}

			m_tcTimeout = (double)GetTickCount64();
			m_nStep++;
		}break;
	case 10004: // check done move X1 or X2 target position
		{
			int nChkComp = 0;
			int nAxisNo[1] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2 };
			for (int i = 0; i < _countof(nAxisNo); i++) {
				OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo[i]);
				int nAxisDone = pMotor->AxisDone();
				if (nAxisDone == ERR_NO_ERROR) {
					nChkComp++;
				}
				else
				{
					double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
					if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d X%d Axis move time over [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_strStageName);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
						break;
					}

					if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d axis done fail, X%d move target position [Axis=%s] [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, pMotor->GetAxisName(), m_strStageName);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPpIdx, AlarmData);
						break;
					}
				}
			}
			if (nChkComp == _countof(nAxisNo)) {
				pTaskTestPP->MakeLog("[%s] [RECOVERY=%8d] [X%d move %s Target pos](END)", m_szCmdName, m_nStep, m_nHandType + 1, m_strStageName);
				m_nStep = 11000;
			}
		}break;
	case 11000:
	{
		m_nRetryCount = 0;
		m_tcTimeout = (double)GetTickCount64();
		if (m_nCmdStage >= LOC_IDX_TEST_PP_TBL_1_LD && m_nCmdStage <= LOC_IDX_TEST_PP_TBL_4_UD && CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto) {
			m_nStep++;
			break;
		}
		m_nStep = 20000;
	}break;
	case 11001:
	{
		double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
		if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Test PP %d move time over, wait load table", m_nTestPpIdx + 1);
			CBaseSeqCmd::ReportAlarm(ERR_LOAD_TBL_1_NOT_ARRIVED_POS + m_nTestPpIdx, AlarmData);
			break;
		}

		int nLoadTblIdx = pTaskTestPP->FromTestPPToLoadTableIdx(m_nCmdStage);
		int nLoadTblCmdStage = pTaskTestPP->FromTestPPToLoadTableCmdStageConvetor(m_nCmdStage);
		int table_col_step = g_TaskLoadTbl[nLoadTblIdx].GetTableColStep();
		int nRetLoadTblPos = g_TaskLoadTbl[nLoadTblIdx].ChkMotorPos(nLoadTblCmdStage, CTaskLoadTable::eAxisAll, table_col_step);
		pTaskTestPP->MakeLog("[Table Col Step : %d] [ChkMotorPos Ret : %d]", table_col_step, nRetLoadTblPos);
		if (nRetLoadTblPos == ERR_NO_ERROR && g_TaskLoadTbl[nLoadTblIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready) {
			m_nStep = 20000;
		}
		else {
			g_TaskLoadTbl[nLoadTblIdx].SetTestppPrePos(nLoadTblCmdStage);
		}
	}break;

	case 20000:
		{
			m_nPickRetry = 0;
			SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing, m_nLast_Executing_Step);
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
BOOL CTestPPCmd_MovePickPlace::State_NormalComplete()
{
	CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
	//pTaskTestPP->SendEvent(CEID_TEST_PP_MOVE_PICK_PLACE_END);

	// Vision Ready On
	BOOL bVisionUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_VisionUse);
	int  nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);

	if (m_nCmdStage >= LOC_IDX_STATION_1 && m_nCmdStage <= LOC_IDX_STATION_4)
	{
		if (bVisionUseMode == DEF_ON /*&& nDeviceMode == eDeviceMode*/)
		{
			if (m_nCmdPick == DEF_PICK && m_nHandType == eTestPPUnLoadType)
			{ // DEF_PLACE //vision Empty						
				if (m_nPressIdx < eMaxPressUnitCount / 2)
					g_pVisionTestHand_1->Cmd_VisionReady_StationCam(eCamStateRun_LightOFF, m_nPressIdx+1);
				else
					g_pVisionTestHand_2->Cmd_VisionReady_StationCam(eCamStateRun_LightOFF, m_nPressIdx+1);
			}
		}
	}
	
	pTaskTestPP->m_bCalBasePoint = FALSE;
	int nIdx = pTaskTestPP->FromTestPPToLoadTableIdx(m_nCmdStage);
	g_TaskLoadTbl[nIdx].SetTableColStep(0); // table step 0

	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_MovePickPlace::State_Alarm()
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
				//pMotor->SetSlowStop();
				bool bCloseLimit = pMotor->CheckLimitClose();
				double dCurPos = 0;
				pMotor->GetCurrentPos(dCurPos);
				if (bCloseLimit == false)
				{
					pTaskTestPP->MakeLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
					pMotor->SetSlowStop();
				}
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
BOOL CTestPPCmd_MovePickPlace::State_AlarmComplete()
{
	CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		//pTaskTestPP->MakeLog("[CMD: %s] [ALARM COMP] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch(m_nStep)
	{
	case 0:    
		{
			double dErr[CTaskTestPP::eAxisMax]  = { 0, };
			double dTPos[CTaskTestPP::eAxisMax] = { 0, };
			pTaskTestPP->GetTeachPos(m_nCmdStage, m_cCmdPocket, m_vCmdPicker, CTaskTestPP::eZPOS_SAFETY, dTPos, m_nHandType,m_bCleanOffsetUse);
			double dCurPos[CTaskTestPP::eAxisMax] = { 0, };
			for (int i = 0; i < CTaskTestPP::eAxisMax; i++)
			{
				m_pMotor[i]->GetCurrentPos(dCurPos[i]);
			}

			dTPos[CTaskTestPP::eAxisY] = 0.0;

			// Teaching 위치와 현재 위치가 일치 하는지 확인.
			dErr[CTaskTestPP::eAxisX1] = fabs(dCurPos[CTaskTestPP::eAxisX1] - dTPos[CTaskTestPP::eAxisX1]);
			dErr[CTaskTestPP::eAxisZ1] = fabs(dCurPos[CTaskTestPP::eAxisZ1] - dTPos[CTaskTestPP::eAxisZ1]);
			dErr[CTaskTestPP::eAxisPitch_HW_X1_SW_Y1] = fabs(dCurPos[CTaskTestPP::eAxisPitch_HW_X1_SW_Y1] - dTPos[CTaskTestPP::eAxisPitch_HW_X1_SW_Y1]);
			dErr[CTaskTestPP::eAxisPitch_HW_Y1_SW_X1] = fabs(dCurPos[CTaskTestPP::eAxisPitch_HW_Y1_SW_X1] - dTPos[CTaskTestPP::eAxisPitch_HW_Y1_SW_X1]);

			dErr[CTaskTestPP::eAxisX2] = fabs(dCurPos[CTaskTestPP::eAxisX2] - dTPos[CTaskTestPP::eAxisX2]);
			dErr[CTaskTestPP::eAxisZ2] = fabs(dCurPos[CTaskTestPP::eAxisZ2] - dTPos[CTaskTestPP::eAxisZ2]);
			dErr[CTaskTestPP::eAxisPitch_HW_X2_SW_Y2] = fabs(dCurPos[CTaskTestPP::eAxisPitch_HW_X2_SW_Y2] - dTPos[CTaskTestPP::eAxisPitch_HW_X2_SW_Y2]);
			dErr[CTaskTestPP::eAxisPitch_HW_Y2_SW_X2] = fabs(dCurPos[CTaskTestPP::eAxisPitch_HW_Y2_SW_X2] - dTPos[CTaskTestPP::eAxisPitch_HW_Y2_SW_X2]);

			dErr[CTaskTestPP::eAxisY] = fabs(dCurPos[CTaskTestPP::eAxisY] - dTPos[CTaskTestPP::eAxisY]);

			int nChkTeachSame = 0;
			for( int i=0; i < CTaskTestPP::eAxisMax; i++ )
			{
				if( dErr[i] <= m_dInPosRange[i] )
					nChkTeachSame++;
			}

			// 축 이동이 완료 되어진 상태 이므로 motor 동작이 필요 없음.
			if( nChkTeachSame == CTaskTestPP::eAxisMax ){
				m_nStep = 10000; 
			}else{
				m_nStep = 1000;
				m_nRetryCount = 0;
			}
		}break;	
	case 1000:   // Z Axis All picker up
	{
		pTaskTestPP->MakeLog("[%s] [ALARM COMP=%8d] [Z1,Z2 move %s safety pos](STR)", m_szCmdName, m_nStep, m_strStageName);
		int nAxisNo[2] = { CTaskTestPP::eAxisZ1 ,CTaskTestPP::eAxisZ2 };
		std::vector<CPoint> vPkr;
		pTaskTestPP->MakeAllPicker(vPkr);
		for (int i = 0; i < _countof(nAxisNo); i++)
		{
			int nErr = pTaskTestPP->do_MovePos(eLocIdxTestPP::LOC_IDX_TEST_PP_HOME, i, m_cCmdPocket, vPkr, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i]);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d all picker up command, but this was wrong ", m_nTestPpIdx + 1);
				OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(ERR_TEST_PP_1_Z1_PICKER_CMD_INTERLOCK + pTaskTestPP->GetHandTypeIndex(m_nHandType), AlarmData);
				break;
			}
		}
		m_tcTimeout = (double)GetTickCount64();
		m_nStep++;
	}break;
	case 1001:  //  Z Axis All check picker all up
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

				double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
				if (dSpanTime > TIME_OUT_SHORT*1000.0)
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
			pTaskTestPP->MakeLog("[%s] [ALARM COMP=%8d] [Z Axis Move to Safety Pos](END)", m_szCmdName, m_nStep);
		}

	}break;
	case 2000: // check z safety and X1 or x2 home
	{
		pTaskTestPP->MakeLog("[%s] [ALARM COMP=%8d] [X%d Axis Move to Safety Pos Check]", m_szCmdName, m_nStep, m_nHandType + 1);
		std::vector<CPoint> vTempPkr;
		vTempPkr.push_back(CPoint(0, 0));
		double dCurPosX1 = 0;
		m_pMotor[CTaskTestPP::eAxisX1]->GetCurrentPos(dCurPosX1);
		double dCurPosX2 = 0;
		m_pMotor[CTaskTestPP::eAxisX2]->GetCurrentPos(dCurPosX2);

		double dTPosHome[CTaskTestPP::eAxisMax] = { 0, };
		pTaskTestPP->GetTeachPos(LOC_IDX_TEST_PP_HOME, CPoint(0, 0), vTempPkr, CTaskTestPP::eZPOS_SAFETY, dTPosHome, m_nHandType,m_bCleanOffsetUse);

		int nSafetySenX1 = pTaskTestPP->ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON);
		int nSafetySenX2 = pTaskTestPP->ChkDI(CTaskTestPP::xX2AXIS_POSITION_SEN, DEF_ON);
		// home 위치보다 크거나 안전 센서가 감지 안되면	 X1,X2축 Home
		if (dTPosHome[CTaskTestPP::eAxisX1] + 1000.0 < dCurPosX1 || nSafetySenX1 != ERR_NO_ERROR || 0 > dCurPosX1 ||
			dTPosHome[CTaskTestPP::eAxisX2] + 1000.0 < dCurPosX2 || nSafetySenX2 != ERR_NO_ERROR || 0 > dCurPosX2)
		{
			m_tcTimeout = (double)GetTickCount64();
			m_nStep = 3000;
		}
		// 그 외에 경우에는 그냥 움직이면 된다.
		else {
			m_tcTimeout = (double)GetTickCount64();
			m_nStep = 4000;
		}
	}break;
	case 3000: //X1, X2 move Home
	{
		if (m_nCmdStage >= LOC_IDX_STATION_1 && m_nCmdStage <= LOC_IDX_STATION_4)
		{
			int nPressIdx = pTaskTestPP->GetPressNum(m_nCmdStage);

			double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
			if (dSpanTime > MOTOR_TIME_OUT_SHORT*1000.0) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d x Axis can not move station [WorkStage=%s not safety]", m_nTestPpIdx + 1, m_strStageName);
				ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + nPressIdx, AlarmData);
				break;
			}

			if (g_TaskPressUnit[nPressIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
				break;
		}

		int nAxisNo[1] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2 };
		std::vector<CPoint> vPicker;
		vPicker.push_back(CPoint(0, 0));
		pTaskTestPP->MakeLog("[%s] [ALARM COMP=%8d] [X1,X2 Axis Move to Home Pos](STR)", m_szCmdName, m_nStep);
		for (int i = 0; i < _countof(nAxisNo); i++)
		{
			int nErr = pTaskTestPP->do_MovePos(m_nCmdStage, m_nHandType, m_cCmdPocket, m_vCmdPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i],m_bCleanOffsetUse);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d x axis move command,but this was wrong", m_nTestPpIdx + 1);
				OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
		}
		m_tcTimeout = (double)GetTickCount64();
		m_nStep++;
	}break;
	case 3001: // check done X1, X2 move Home
	{
		OneAxis* pMotorX1 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisX1);
		OneAxis* pMotorX2 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisX2);

		OneAxis* pMotor[1] = { m_nHandType == eTestPPLoadType ? pMotorX1 : pMotorX2 };
		int nComp = 0;
		for (int i = 0; i < _countof(pMotor); i++)
		{
			int nAxisDone = pMotor[i]->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				nComp++;
			}
			else 
			{
				double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
				if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d x Home Position move time over", m_nTestPpIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d x axis done fail, move home position", m_nTestPpIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPpIdx, AlarmData);
					break;
				}
			}
		}

		if (nComp == _countof(pMotor))
		{
			m_nStep = 4000;
			pTaskTestPP->MakeLog("[%s] [ALARM COMP=%8d] [X1,X2 Move Home](END)", m_szCmdName, m_nStep);
		}
	}break;
	case 4000: // // move X1,Y1 or X2,Y2 Pitch, Y target position 
	{
		int nAxisNo[3] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_X1_SW_Y1 : CTaskTestPP::eAxisPitch_HW_X2_SW_Y2,
			m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_Y1_SW_X1 : CTaskTestPP::eAxisPitch_HW_Y2_SW_X2,
			CTaskTestPP::eAxisY };

		pTaskTestPP->MakeLog("[%s] [ALARM COMP=%8d] [X%d Pitch, Y%d Pitch , Y move %s safety pos](STR)", m_szCmdName, m_nStep, m_nHandType + 1, m_nHandType + 1, m_strStageName);
		for (int i = 0; i < _countof(nAxisNo); i++) {
			int nErr = pTaskTestPP->do_MovePos(m_nCmdStage, m_nHandType, m_cCmdPocket, m_vCmdPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i], m_bCleanOffsetUse);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d X%d Pitch, Y%d Pitch, Y target position command,but this was wrong [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_nHandType + 1, m_strStageName);
				OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
		}

		m_tcTimeout = (double)GetTickCount64();
		m_nStep++;
	}break;
	case 4001: // check done move X1,Y1 or X2,Y2 Pitch Y target position
	{
		int nChkComp = 0;
		int nAxisNo[3] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_X1_SW_Y1 : CTaskTestPP::eAxisPitch_HW_X2_SW_Y2,
			m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_Y1_SW_X1 : CTaskTestPP::eAxisPitch_HW_Y2_SW_X2,
			CTaskTestPP::eAxisY };

		for (int i = 0; i < _countof(nAxisNo); i++) {
			OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo[i]);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				nChkComp++;
			}
			else 
			{
				double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
				if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d, X%dPitch, Y%dPitch Axis move time over [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_nHandType + 1, m_strStageName);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d axis done fail, X%dPitch, Y%dPitch, Y move target position [Axis=%s] [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_nHandType + 1, pMotor->GetAxisName(), m_strStageName);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPpIdx, AlarmData);
					break;
				}
			}
		}
		if (nChkComp == _countof(nAxisNo)) {
			m_nStep = 10000;
			pTaskTestPP->MakeLog("[%s] [ALARM COMP=%8d] [X%d Pitch, Y%d Pitch, Y move %s target pos](END)", m_szCmdName, m_nStep, m_nHandType + 1, m_nHandType + 1, m_strStageName);
		}
	}break;
	case 10000:
		{
			int nSiteCntX = g_DMCont.m_dmTestPP.GN( NDM4_TestSite_Div_X );
			int nSiteCntY = g_DMCont.m_dmTestPP.GN( NDM4_TestSite_Div_Y );

			//pTaskTestPP->SendEvent(CEID_TEST_PP_ALARMCOMPLETE_END);
			int nErrSkipDeviceType = eDeviceType::_eDeviceType_Normal;
			std::vector<CPoint> vVisionErrSkip;
			if( CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto ){
				int nVisionErrCnt = 0;
				for( int nY = 0; nY < nSiteCntY; nY++ )
				{
					for( int nX = 0; nX < nSiteCntX; nX++ )
					{
						if( m_bVisionErrSkip[ nX ][ nY ] == TRUE ) {
							nVisionErrCnt++;
							vVisionErrSkip.push_back(CPoint(nX, nY));
						}
					}
				}

				if (nVisionErrCnt == 0) {

					std::vector<CPoint> vLostPkr;
					vLostPkr = pTaskTestPP->GetLostPicker(m_nHandType);
					if (vLostPkr.size() > 0)
					{
						nErrSkipDeviceType = pTaskTestPP->CheckDeviceType(eDeviceStage_TEST_PP_1 + m_nTestPpIdx, m_nHandType, m_cCmdPocket.x, m_cCmdPocket.y, vLostPkr, DEF_PICK);
						
						ST_DD_DEVICE stEmptyDevice; stEmptyDevice.clear();
						for (int i = 0; i < (int)vLostPkr.size(); i++)
						{
							if(m_nHandType == eTestPPLoadType)
								g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_PP_1 + m_nTestPpIdx, vLostPkr[i].x, vLostPkr[i].y, &stEmptyDevice);
							else
								g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_PP_1 + m_nTestPpIdx, vLostPkr[i].x, vLostPkr[i].y + TEST_UD_PKR_Y, &stEmptyDevice);

							g_JamRate.PushInOutJam(eJamType_Output, 1);
							if (m_nHandType == eTestPPLoadType)
								pTaskTestPP->MakeLog("[%s] [ALARM COMP=%8d] [Before Blow device Skip Picker= %d-%d]", m_szCmdName, m_nStep, vLostPkr[i].x + 1, vLostPkr[i].y + 1);
							else
								pTaskTestPP->MakeLog("[%s] [ALARM COMP=%8d] [Before Blow device Skip Picker= %d-%d]", m_szCmdName, m_nStep, vLostPkr[i].x + 1, vLostPkr[i].y + 1 + 2);
						}

						

						pTaskTestPP->m_vLostBlowDvc[m_nHandType].clear();
					}
					else {
						std::vector<CString> vErrString; vErrString.clear();
						int nRet = pTaskTestPP->ProcDeviceData(m_nCmdStage, m_nHandType, m_cCmdPocket.x, m_cCmdPocket.y, m_vCmdPicker, m_nCmdPick, vErrString, nErrSkipDeviceType, TRUE);

						// set in/out jam count
						int nCount = pTaskTestPP->ChkLostDvcCnt(m_vCmdPicker,m_nHandType, m_nCmdPick);
						switch (m_nCmdStage)
						{
						case LOC_IDX_TEST_PP_TBL_1_LD:
						case LOC_IDX_TEST_PP_TBL_2_LD:
						case LOC_IDX_TEST_PP_TBL_3_LD:
						case LOC_IDX_TEST_PP_TBL_4_LD:
						case LOC_IDX_TEST_PP_TBL_1_UD:
						case LOC_IDX_TEST_PP_TBL_2_UD:
						case LOC_IDX_TEST_PP_TBL_3_UD:
						case LOC_IDX_TEST_PP_TBL_4_UD:
						{
							if (m_nCmdPick == DEF_PICK) {
								g_JamRate.PushInOutJam(eJamType_Input, nCount);
							}
							else {
								g_JamRate.PushInOutJam(eJamType_Output, nCount);
							}
						}break;
						case LOC_IDX_STATION_1: // case LOC_IDX_STATION_UP_1:
						case LOC_IDX_STATION_2: //case LOC_IDX_STATION_UP_2:
						case LOC_IDX_STATION_3: // case LOC_IDX_STATION_UP_3:
						case LOC_IDX_STATION_4: // case LOC_IDX_STATION_UP_4
						{
							if (m_nCmdPick == DEF_PICK) {
								g_JamRate.PushInOutJam(eJamType_Output, nCount);
							}
							else {
								g_JamRate.PushInOutJam(eJamType_Input, nCount);
							}
						}break;
						}
					}					
				}
				else {
					if (m_nCmdStage >= LOC_IDX_STATION_1 && m_nCmdStage <= LOC_IDX_STATION_4)
					{
						ST_DD_DEVICE stEmptyDevice; stEmptyDevice.clear();

						/*if (nErrSkipDeviceType == eDeviceType::_eDeviceType_Normal) {*/
						nErrSkipDeviceType = pTaskTestPP->CheckDeviceType(m_nCmdStage, m_nHandType, m_cCmdPocket.x, m_cCmdPocket.y, vVisionErrSkip, DEF_PICK);
						//}

						for (int nY = 0; nY < nSiteCntY; nY++)
						{
							for (int nX = 0; nX < nSiteCntX; nX++)
							{
								pTaskTestPP->SetCmdStageDvcData(m_nCmdStage, CPoint(nX, nY), m_nHandType, &stEmptyDevice);
								m_bVisionErrSkip[nX][nY] = FALSE;
								g_JamRate.PushInOutJam(eJamType_Output, 1);// Vision Exist 는 Test 끝난 Device 를 Pick 할 경우만 있다
								pTaskTestPP->MakeLog("[%s] [ALARM COMP=%8d] [Vision Error Skip Station =%d, Site = %d-%d]", m_szCmdName, m_nStep, m_nPressIdx + 1, nX + 1, nY + 1);							
							}
						}
					}
				}
			}


			if (nErrSkipDeviceType == eDeviceType::_eDeviceType_Cleaning)
			{
				g_TaskSystemCtrl.SetOneCycleMode(DEF_ON);
				g_TaskSystemCtrl.SetCleanDeviceSkipOneCycleMode(DEF_ON);
				pTaskTestPP->MakeLog("[%s] [ALARM COMP=%8d] [Clean Device Skip Set One Cycle]", m_szCmdName, m_nStep);
			}

			if (m_nCmdStage >= LOC_IDX_TEST_PP_TBL_1_LD && m_nCmdStage <= LOC_IDX_TEST_PP_TBL_4_UD)
			{
				int nLoadTblIdx = pTaskTestPP->FromTestPPToLoadTableIdx(m_nCmdStage);
				if (nLoadTblIdx == eLoadTbl_1 || nLoadTblIdx == eLoadTbl_4)
					g_DMCont.m_dmTestPP.SB(NDM04_TESTHAND1_ALREADY_PICK_UNLOAD_TABLE_CALL + m_nTestPpIdx, FALSE);
				else
					g_DMCont.m_dmTestPP.SB(NDM04_TESTHAND1_ALREADY_PICK_TABLE_CALL + m_nTestPpIdx, FALSE);
			}

			pTaskTestPP->m_bCalBasePoint = FALSE;
			int nIdx = pTaskTestPP->FromTestPPToLoadTableIdx(m_nCmdStage);
			g_TaskLoadTbl[nIdx].SetTableColStep(0); // table step 0

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
BOOL CTestPPCmd_MovePickPlace::State_Aborting()
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
				pMotor->SetSlowStop();
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
BOOL CTestPPCmd_MovePickPlace::State_AbortComplete()
{
// 	CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
// 	if( CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto){
// 		pTaskTestPP->ProcDeviceData(m_nCmdStage, m_nCmdTargetX, m_nCmdTargetY, m_vCmdPicker, m_nCmdPick);
// 	}

	return CBaseSeqCmd::State_AbortComplete(); 
}


