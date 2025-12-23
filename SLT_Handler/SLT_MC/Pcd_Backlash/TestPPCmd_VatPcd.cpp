
#include "StdAfx.h"
#include "TestPPCmd_VatPcd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* simulation code */
#define is_simulation() ((g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) ? TRUE : FALSE)
#define PCD_TEST_PP_SPD				(15)
/* simulation code */
//==============================================================================
//
//==============================================================================
CTestPPCmd_VatPcd::CTestPPCmd_VatPcd(int nTestPpIdx)
	: m_nTestPpIdx(nTestPpIdx)
{
	m_nCmdId = eSeqCmd_TestPP_AutoVacZ;
	m_nRetryCount = 0;
	sprintf_s(m_szCmdName, sizeof(m_szCmdName), "CTestPPCmd_VatPcd");
	m_dLowerSafety =0.0;
	m_dUpperSafety =0.0;

	m_nDistance = 0;
	m_nAxisNum = 0;

	m_nHandType = eTestPPLoadType;

	for (int i = 0; i < CTaskTestPP::eAxisMax; i++)
	{
		 m_dLastPos[i] =0.0;
		 m_dLastPosErrRange[i] =0.0;
		 m_dInPosRange[i] = 0.0;
	}

	vStationNo.clear();
	if (m_nTestPpIdx == eTestPP_1)
	{
		vStationNo.push_back(ePressUnit_Down_1);
		vStationNo.push_back(ePressUnit_Down_2);
		vStationNo.push_back(ePressUnit_Down_3);
		vStationNo.push_back(ePressUnit_Down_4);
	}
	else {
		vStationNo.push_back(ePressUnit_Down_5);
		vStationNo.push_back(ePressUnit_Down_6);
		vStationNo.push_back(ePressUnit_Down_7);
		vStationNo.push_back(ePressUnit_Down_8);
	}
		
}


//==============================================================================
//
//==============================================================================
CTestPPCmd_VatPcd::~CTestPPCmd_VatPcd(void){}

//==============================================================================
//
//==============================================================================
int CTestPPCmd_VatPcd::CheckParam()
{
	double dTimeout = 60.0;
	m_tcTimeout.SetCheckTime(dTimeout);
	m_tcTimeout.StartTimer();
	m_nLast_Executing_Step = exe_start;

	CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;

	if (is_simulation()) {
		m_Cmd_PassRangeUm.x = 200.0;
		m_Cmd_PassRangeUm.y = 200.0;
	}
	else {
		OneAxis* pMotorX = pTaskTestPP->GetAxis(CTaskTestPP::eAxisX1);
		OneAxis* pMotorY = pTaskTestPP->GetAxis(CTaskTestPP::eAxisY);
		m_Cmd_PassRangeUm.x = pMotorX->GetScaleFactor();
		m_Cmd_PassRangeUm.y = pMotorY->GetScaleFactor();
	}

	return ERR_NO_ERROR;
}


//==============================================================================
//
//==============================================================================
int  CTestPPCmd_VatPcd::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTestPPCmd_VatPcd::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTestPPCmd_VatPcd::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTestPPCmd_VatPcd::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTestPPCmd_VatPcd::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_VatPcd::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_VatPcd::State_Executing()
{
	int nErr = 0;

	// check alarm
	CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;

	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	m_nStep_Pre = m_nStep;

	int i=0;
	for( i=0; i<CTaskTestPP::eAxisMax; i++){
		OneAxis* pMotor = pTaskTestPP->GetAxis(i);
		if( pMotor->ChkMotorUsable() != ERR_NO_ERROR ){
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Test PP %d check motor usable Cmd Error", m_nTestPpIdx+1);
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(ERR_TEST_PP_1_MOTOR_ALARMED + m_nTestPpIdx, AlarmData);
			return FALSE;
		}
	}

	for (int i = 0; i < vStationNo.size();i++)
	{
		int nPressIdx = vStationNo[i];
		if (g_TaskPressUnit[nPressIdx].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON) != ERR_NO_ERROR)
		{
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Test PP %d check motor usable Cmd Error", m_nTestPpIdx + 1);
			ReportAlarm(ERR_PRESS_UNIT_1_NOT_DETECT_SAFETY_SEN + nPressIdx, AlarmData);
		}

		if (g_TaskPressUnit[nPressIdx].ChkMotorPos(LOC_IDX_PRESS_UNIT_SAFETY, CTaskPressUnit::eAxisAll) != ERR_NO_ERROR)
		{
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Test PP %d check motor usable Cmd Error", m_nTestPpIdx + 1);
			ReportAlarm(ERR_MOT_INTERLOCK_PRESS_UNIT_1_NOT_DETECT_SAFETY_POS + nPressIdx, AlarmData);
		}
	}
	


	OneAxis* pMotorX  = m_nHandType == eTestPPLoadType? pTaskTestPP->GetAxis(CTaskTestPP::eAxisX1) : pTaskTestPP->GetAxis(CTaskTestPP::eAxisX2);
	OneAxis* pMotorY  = pTaskTestPP->GetAxis(CTaskTestPP::eAxisY);
	OneAxis* pMotorZ  = m_nHandType == eTestPPLoadType ? pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ1) : pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ2);

	switch( m_nStep )
	{		
	case exe_start:
		{
			if(m_nAxisNum != eAxis_X && m_nAxisNum !=eAxis_Y) nErr++;

			if( nErr ){
				SimpleAlarm( ERR_LOT_CMD_PARAMETER_IS_INVALID );
				break;
			} 
			m_StartPos_Offset = CPointF(0,0,0);
			m_TargetPos_Offset = CPointF(0,0,0);

			if(m_nAxisNum == eAxis_X){
				m_PcdStart_Pos = g_VatPcd.testpp[m_nTestPpIdx][m_nHandType].m_PcdStartXPos_XYZ;
			}else{
				m_PcdStart_Pos = g_VatPcd.testpp[m_nTestPpIdx][m_nHandType].m_PcdStartYPos_XYZ;
			}
		
			m_dLowerSafety = g_VatPcd.testpp[m_nTestPpIdx][m_nHandType].m_dLowerSafetyPos * 1000;
			m_dUpperSafety = g_VatPcd.testpp[m_nTestPpIdx][m_nHandType].m_dUpperSafetyPos * 1000;

			m_nLast_Executing_Step = exe_hand_all_pkr_up;
			NextStep( exe_hand_all_pkr_up );
		}break;

		// picker up
	case exe_hand_all_pkr_up:
		{
			int nAxisNo[2] = { CTaskTestPP::eAxisZ1 ,CTaskTestPP::eAxisZ2 };
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z1,Z2 move safety pos](STR)", m_szCmdName, m_nStep);
			for (int i = 0; i < _countof(nAxisNo); i++)
			{
				int nErr = pTaskTestPP->do_MovePos(LOC_IDX_TEST_PP_HOME, m_nHandType, m_cCmdPocket, m_vSafetyAllPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i], FALSE, PCD_TEST_PP_SPD);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d all picker up command, but this was wrong ", m_nTestPpIdx + 1);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_Z1_PICKER_CMD_INTERLOCK + pTaskTestPP->GetHandTypeIndex(m_nHandType), AlarmData);
					break;
				}
			}
			
			NextStepTimer( exe_hand_all_pkr_up_check, TIME_OUT_SHORT );
		}break;
	case exe_hand_all_pkr_up_check:
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
				int nAxisDone = pMotor[i]->AxisDone(vPkr, m_vErrPicker);
				int nErrSen = pTaskTestPP->Chk_Z_Safety_Sen_Check_Type(vPkr,i);
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
				m_nStep = exe_hand_safety_x_check;
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z%d Axis Move to Safety Pos](END)", m_szCmdName, m_nStep, m_nHandType + 1);
			}

		}break;

	case exe_hand_safety_x_check:
		{
			// 축별 도피에 대한 점검
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X%d Axis Move to Safety Pos Check]", m_szCmdName, m_nStep, m_nHandType + 1);
			std::vector<CPoint> vTempPkr;
			vTempPkr.push_back(CPoint(0, 0));
			double dCurPosX1 = 0;
			double dCurPosX2 = 0;
			OneAxis* pMotorX1 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisX1);
			OneAxis* pMotorX2 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisX2);
			pMotorX1->GetCurrentPos(dCurPosX1);
			pMotorX2->GetCurrentPos(dCurPosX2);

			double dTPosHome[CTaskTestPP::eAxisMax] = { 0, };
			pTaskTestPP->GetTeachPos(LOC_IDX_TEST_PP_HOME, CPoint(0, 0), vTempPkr, CTaskTestPP::eZPOS_SAFETY, dTPosHome, m_nHandType);

			int nSafetySenX1 = pTaskTestPP->ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON);
			int nSafetySenX2 = pTaskTestPP->ChkDI(CTaskTestPP::xX2AXIS_POSITION_SEN, DEF_ON);
			// home 위치보다 크거나 안전 센서가 감지 안되면	 X1,X2축 Home
// 			if (dTPosHome[CTaskTestPP::eAxisX1] + 1000.0 < dCurPosX1 || nSafetySenX1 != ERR_NO_ERROR || 0 > dCurPosX1 ||
// 				dTPosHome[CTaskTestPP::eAxisX2] + 1000.0 < dCurPosX2 || nSafetySenX2 != ERR_NO_ERROR || 0 > dCurPosX2)
// 			{
				NextStep(exe_hand_safety_x_move);
// 			}
// 			else {
// 
// 			}
		}break;
		// anti-confict Z move
	case exe_hand_safety_x_move:
		{
			int nAxisNo[2] = { CTaskTestPP::eAxisX1 ,CTaskTestPP::eAxisX2 };
			std::vector<CPoint> vPicker;
			vPicker.push_back(CPoint(0, 0));
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X1,X2 Axis Move to Home Pos](STR)", m_szCmdName, m_nStep);
			for (int i = 0; i < _countof(nAxisNo); i++)
			{
				int nErr = pTaskTestPP->do_MovePos_Pcd_backlash(0, m_vSafetyAllPicker, nAxisNo[i], PCD_TEST_PP_SPD,0,FALSE, m_nHandType);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d y axis move command,but this was wrong", m_nTestPpIdx + 1);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}
			m_tcTimeout.SetCheckTime(TIME_OUT_MIDDLE);
			m_tcTimeout.StartTimer();

			NextStep(exe_hand_safety_x_move_check);

		}break;
	case exe_hand_safety_x_move_check:
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
					if (m_tcTimeout.CheckOverTime() == TRUE)
					{
						//	pMotorZ->V_MoveStop();
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("TestPP %d x axis move avoid safety position", pTaskTestPP->m_nTestPPIdx + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
						CBaseSeqCmd::ReportAlarm(ERR_TEST_PP_1_MOTOR_ORG_TIMEOUT + pTaskTestPP->m_nTestPPIdx, AlarmData);
						break;
					}

					if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d x axis done fail, move home position", m_nTestPpIdx + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
						ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPpIdx, AlarmData);
					}
				}
			}

			if (nComp == _countof(pMotor))
			{
				NextStep(exe_hand_z_move_safety_down_position);
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X1,X2 Move Home](END)", m_szCmdName, m_nStep);
			}
		}break;
		// picker up
	case exe_hand_z_move_safety_down_position:
	{
		int nAxisNo[2] = { CTaskTestPP::eAxisZ1 ,CTaskTestPP::eAxisZ2 };
		pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z1,Z2 move safety pos](STR)", m_szCmdName, m_nStep);
		for (int i = 0; i < _countof(nAxisNo); i++)
		{
			int nErr = pTaskTestPP->do_MovePos(LOC_IDX_TEST_PP_HOME, m_nHandType, m_cCmdPocket, m_vSafetyAllPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i], FALSE, PCD_TEST_PP_SPD);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d all picker up command, but this was wrong ", m_nTestPpIdx + 1);
				OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(ERR_TEST_PP_1_Z1_PICKER_CMD_INTERLOCK + pTaskTestPP->GetHandTypeIndex(m_nHandType), AlarmData);
				break;
			}
		}

		NextStepTimer(exe_hand_z_move_safety_down_position_check, TIME_OUT_SHORT);
	}break;
	case exe_hand_z_move_safety_down_position_check:
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
			int nAxisDone = pMotor[i]->AxisDone(vPkr, m_vErrPicker);
			int nErrSen = pTaskTestPP->Chk_Z_Safety_Sen_Check_Type(vPkr,i);
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
			m_nStep = exe_hand_xy_pitch_move_safety_down_position;
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z%d Axis Move to Safety Pos](END)", m_szCmdName, m_nStep, m_nHandType + 1);
		}

	}break;
	case exe_hand_xy_pitch_move_safety_down_position:
	{
		int nAxisNo[4] = { CTaskTestPP::eAxisPitch_HW_Y1_SW_X1 ,CTaskTestPP::eAxisPitch_HW_Y2_SW_X2,
			CTaskTestPP::eAxisPitch_HW_X1_SW_Y1,CTaskTestPP::eAxisPitch_HW_X2_SW_Y2 };
		pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Pitch X1,X2,Y1,Y2 move safety pos](STR)", m_szCmdName, m_nStep);
		for (int i = 0; i < _countof(nAxisNo); i++)
		{
			int nErr = pTaskTestPP->do_MovePos(LOC_IDX_TEST_PP_HOME, m_nHandType, m_cCmdPocket, m_vSafetyAllPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i], FALSE, PCD_TEST_PP_SPD);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d Pitch X1,X2,Y1,Y2 command, but this was wrong ", m_nTestPpIdx + 1);
				OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + pTaskTestPP->GetHandTypeIndex(m_nHandType), AlarmData);
				break;
			}
		}

		NextStepTimer(exe_hand_xy_pitch_move_safety_down_position_check, TIME_OUT_SHORT);
	}break;
	case exe_hand_xy_pitch_move_safety_down_position_check:
	{
		OneAxis* pMotorX1Pitch = pTaskTestPP->GetAxis(CTaskTestPP::eAxisPitch_HW_Y1_SW_X1);
		OneAxis* pMotorX2Pitch = pTaskTestPP->GetAxis(CTaskTestPP::eAxisPitch_HW_Y2_SW_X2);
		OneAxis* pMotorY1Pitch = pTaskTestPP->GetAxis(CTaskTestPP::eAxisPitch_HW_X1_SW_Y1);
		OneAxis* pMotorY2Pitch = pTaskTestPP->GetAxis(CTaskTestPP::eAxisPitch_HW_X2_SW_Y2);

		OneAxis* pMotor[4] = { pMotorX1Pitch ,pMotorX2Pitch, pMotorY1Pitch ,pMotorY2Pitch };
		int nComp = 0;
		CString strErr = _T("");
		for (int i = 0; i < _countof(pMotor); i++)
		{
			std::vector<CPoint> vPkr;
			pTaskTestPP->MakeAllPicker(vPkr);
			m_vErrPicker.clear();
			int nAxisDone = pMotor[i]->AxisDone(vPkr, m_vErrPicker);
			if (nAxisDone == ERR_NO_ERROR) {
				nComp++;
			}
			else
			{
				strErr += pMotor[i]->GetAxisName();
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					
					AlarmData.m_fnAddItemFormat("Test PP %d Pitch X1,X2,Y1,Y2 Move Fail ", m_nTestPpIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s", strErr);
					ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + pTaskTestPP->GetHandTypeIndex(i), AlarmData, m_vErrPicker);
					break;
				}
			}
		}
		if (nComp == _countof(pMotor))
		{
			m_nStep = exe_hand_y_move_start_position;
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Test PP %d Pitch X1,X2,Y1,Y2 Safety Pos](END)", m_szCmdName, m_nStep, m_nTestPpIdx + 1);
		}

	}break;
	case exe_hand_y_move_start_position:
	{
		m_nLast_Executing_Step = exe_hand_y_move_start_position;
		pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Y Axis Start Move ](SRT)", m_szCmdName, m_nStep);

		int nAxisNo[1] = { CTaskTestPP::eAxisY };
		double dPos[1] = { m_PcdStart_Pos.y + m_StartPos_Offset.y };


		for (int i = 0; i < _countof(nAxisNo); i++)
		{
			int nErr = pTaskTestPP->do_MovePos_Pcd_backlash(dPos[i], m_vCmdPicker, nAxisNo[i], PCD_TEST_PP_SPD, 0, FALSE, m_nHandType);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d move y to target pos Cmd Error", m_nTestPpIdx + 1);
				OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				return FALSE;
			}
		}

		NextStepTimer(exe_hand_y_move_check_start_position, MOTOR_TIME_OUT_LONG);
	}break;
	case exe_hand_y_move_check_start_position:
	{
		if (m_tcTimeout.CheckOverTime() == TRUE) {
			CParamList AlarmData;
			int nAxisDoneY = pMotorZ->AxisDone();
			double dCurPosY = 0.0;
			pMotorY->GetCurrentPos(dCurPosY);

			AlarmData.m_fnAddItemFormat("Test PP %d Y Axis move  Error", m_nTestPpIdx + 1);
			AlarmData.m_fnAddItemFormat("AxisDoneY = %d, SetTime = %.2f",nAxisDoneY, m_tcTimeout.GetCheckTime());
			AlarmData.m_fnAddItemFormat("Y Cur Pos = %.3f", dCurPosY);
			ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
			break;
		}

		int nChkComp = 0;
		int nAxisNo[1] = { CTaskTestPP::eAxisY };
		for (int i = 0; i < _countof(nAxisNo); i++) {
			OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo[i]);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				nChkComp++;
			}
		}
		if (nChkComp == _countof(nAxisNo)) {
			pTaskTestPP->MakeLog("[%s] [Y Axis Start Move](END)", m_szCmdName, m_nStep);
			//return to xyz confilct check step
			NextStep(exe_hand_xy_move_start_position_before_100mm);
		}
	}
	break;
	case exe_hand_xy_move_start_position_before_100mm:
	{
		m_nLast_Executing_Step = exe_hand_xy_move_start_position_before_100mm;
		pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X,Y Axis Start Move Before 100mm](SRT)",m_szCmdName ,m_nStep);

		int nAxisNo[2] = {m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2, CTaskTestPP::eAxisY};
		double dPos[2] = {m_PcdStart_Pos.x + m_StartPos_Offset.x, m_PcdStart_Pos.y + m_StartPos_Offset.y} ;

		MakeVatPreparePos(m_nTestPpIdx, dPos[0], dPos[1], Move_100mm_Offset);

		for(int i=0; i<_countof(nAxisNo); i++)
		{
			int nErr = pTaskTestPP->do_MovePos_Pcd_backlash(dPos[i], m_vCmdPicker, nAxisNo[i], PCD_TEST_PP_SPD, 0, FALSE, m_nHandType);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d axis %s to target pos Cmd Error", m_nTestPpIdx+1, i==0? "X":"Y");
				OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				return FALSE;
			}
		}

		NextStepTimer( exe_hand_xy_move_check_start_position_before_100mm, MOTOR_TIME_OUT_LONG );
	}break;
	case exe_hand_xy_move_check_start_position_before_100mm:
		{
			if( m_tcTimeout.CheckOverTime() == TRUE ){
				CParamList AlarmData;

				int nAxisDoneX = pMotorX->AxisDone();
				int nAxisDoneY = pMotorZ->AxisDone();
				double dCurPosX=0.0, dCurPosY = 0.0;
				pMotorX->GetCurrentPos(dCurPosX);
				pMotorY->GetCurrentPos(dCurPosY);

				AlarmData.m_fnAddItemFormat("Test PP %d X,Y Axis move  Error", m_nTestPpIdx+1);
				AlarmData.m_fnAddItemFormat("AxisDoneX = %d, AxisDoneY = %d, SetTime = %.2f", nAxisDoneX, nAxisDoneY, m_tcTimeout.GetCheckTime());
				AlarmData.m_fnAddItemFormat("X Cur Pos = %.3f, Y Cur Pos = %.3f", dCurPosX, dCurPosY);
				ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
				break;
			}

			int nChkComp  = 0;
			int nAxisNo[2] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2, CTaskTestPP::eAxisY };
			for(int i=0; i<_countof(nAxisNo); i++){
				OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo[i]);
				int nAxisDone = pMotor->AxisDone();
				if( nAxisDone == ERR_NO_ERROR ){
					nChkComp++;
				}
			}
			if( nChkComp == _countof(nAxisNo)){
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X,Y Axis Start Move Before 100mm](END)", m_szCmdName, m_nStep);
				//return to xyz confilct check step
				NextStep( exe_hand_xy_move_start_position );
			}
		}break;
		case exe_hand_xy_move_start_position:
			{
				m_nLast_Executing_Step = exe_hand_xy_move_start_position;
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X,Y Axis Start Move ](SRT)",m_szCmdName ,m_nStep);

				int nAxisNo[2] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2, CTaskTestPP::eAxisY };
				double dPos[2] = {m_PcdStart_Pos.x + m_StartPos_Offset.x, m_PcdStart_Pos.y + m_StartPos_Offset.y} ;


				for(int i=0; i<_countof(nAxisNo); i++)
				{
					int nErr = pTaskTestPP->do_MovePos_Pcd_backlash(dPos[i], m_vCmdPicker,nAxisNo[i], PCD_TEST_PP_SPD, 0, FALSE, m_nHandType);
					if( nErr != ERR_NO_ERROR ){
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d move x, y to target pos Cmd Error", m_nTestPpIdx+1);
						OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(nErr, AlarmData);
						return FALSE;
					}
				}

				NextStepTimer( exe_hand_xy_move_check_start_position, MOTOR_TIME_OUT_LONG );
			}break;
		case exe_hand_xy_move_check_start_position:
			{
				if( m_tcTimeout.CheckOverTime() == TRUE ){
					CParamList AlarmData;

					int nAxisDoneX = pMotorX->AxisDone();
					int nAxisDoneY = pMotorZ->AxisDone();
					double dCurPosX = 0.0, dCurPosY = 0.0;
					pMotorX->GetCurrentPos(dCurPosX);
					pMotorY->GetCurrentPos(dCurPosY);

					AlarmData.m_fnAddItemFormat("Test PP %d X,Y Axis move  Error", m_nTestPpIdx+1);
					AlarmData.m_fnAddItemFormat("AxisDoneX = %d, AxisDoneY = %d, SetTime = %.2f", nAxisDoneX, nAxisDoneY, m_tcTimeout.GetCheckTime());
					AlarmData.m_fnAddItemFormat("X Cur Pos = %.3f, Y Cur Pos = %.3f", dCurPosX, dCurPosY);
					ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
					break;
				}

				int nChkComp  = 0;
				int nAxisNo[2] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2, CTaskTestPP::eAxisY };
				for(int i=0; i<2; i++){
					OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo[i]);
					int nAxisDone = pMotor->AxisDone();
					if( nAxisDone == ERR_NO_ERROR ){
						nChkComp++;
					}
				}
				if( nChkComp == 2 ){
					pTaskTestPP->MakeLog("[%s] [X,Y Axis Start Move](END)",m_szCmdName ,m_nStep);
					//return to xyz confilct check step
					NextStep( exe_hand_z_start_position_move );
				}
			}
			break;
	case exe_hand_z_start_position_move:
		{
			m_nLast_Executing_Step = exe_hand_z_start_position_move;

			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [z move start position ] [SRT]", m_szCmdName, m_nStep);

			int nAxisNo =  m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;

			nErr = pTaskTestPP->do_MovePos_Pcd_backlash( m_PcdStart_Pos.z, m_vCmdPicker, nAxisNo, PCD_TEST_PP_SPD);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}

			NextStepTimer( exe_hand_z_start_position_move_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_z_start_position_move_check:
		{
			if( m_tcTimeout.CheckOverTime() == TRUE ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				ReportAlarm(ERR_TEST_PP_1_Z1_PICKER_MOVE_TIMEOUT + pTaskTestPP->GetHandTypeIndex(m_nHandType), AlarmData);
				break;
			}
			int nAxisDone = pMotorZ->AxisDone();
			if( nAxisDone == ERR_NO_ERROR ){
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [z move start position ] [END]", m_szCmdName, m_nStep);

				NextStep(exe_visn_recon_start_position);
			}
		}break;
	case exe_visn_recon_start_position:
		{
			m_nLast_Executing_Step =exe_visn_recon_start_position;

			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Pcd start position measurement] [SRT]", m_szCmdName, m_nStep);
			if( is_simulation() ){
				NextStepTimer( exe_visn_recon_start_position_check, VISION_TIME_OUT );
				break;
			}
			Sleep(1000);
			int nVATIdx = g_pVisionTestHand_1->GetHandTypeIdx((_eTestPPIdx)m_nTestPpIdx, (eTestPPType)m_nHandType);
			if (m_nTestPpIdx == eTestPP_1)
				g_pVisionTestHand_1->SendMeasure_PcdBacklash(m_nHandType == eTestPPLoadType ? VISION_MEASURE_VAT_TEST_1_LD_UPPER : VISION_MEASURE_VAT_TEST_1_ULD_UPPER, nVATIdx);
			else
				g_pVisionTestHand_2->SendMeasure_PcdBacklash(m_nHandType == eTestPPLoadType ? VISION_MEASURE_VAT_TEST_2_LD_UPPER : VISION_MEASURE_VAT_TEST_2_ULD_UPPER, nVATIdx);

			NextStepTimer( exe_visn_recon_start_position_check, VISION_TIME_OUT );
		}break;
	case exe_visn_recon_start_position_check:
		{
			ST_PCD_BACKLASH_RCV_DATA vRcvData;
			/* CheckMeasureResult() 에서 alarm check 처리 */
			if( CheckMeasureResult(vRcvData) == TRUE ){
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Pcd Start position measurement dx=%.3f um, dy=%.3f um] [END]", m_szCmdName, m_nStep,vRcvData.dX,vRcvData.dY);

				m_StartPos_Offset.x += vRcvData.dX;
				m_StartPos_Offset.y += vRcvData.dY;

				if(fabs(vRcvData.dX) < m_Cmd_PassRangeUm.x && fabs(vRcvData.dY) < m_Cmd_PassRangeUm.y){
					NextStep( exe_hand_z_start_position_safety_move );
					break;
				}
				else
				{
					NextStep(exe_hand_z_move_safety_down_position);
				}
				

			}
		}break;
	case exe_hand_z_start_position_safety_move:
		{
			m_nLast_Executing_Step = exe_hand_z_start_position_safety_move;

			int nAxisNo[2] = { CTaskTestPP::eAxisZ1 ,CTaskTestPP::eAxisZ2 };
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z1,Z2 move safety pos](STR)", m_szCmdName, m_nStep);
			for (int i = 0; i < _countof(nAxisNo); i++)
			{
				int nErr = pTaskTestPP->do_MovePos(LOC_IDX_TEST_PP_HOME, m_nHandType, m_cCmdPocket, m_vSafetyAllPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i], FALSE, PCD_TEST_PP_SPD);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d all picker up command, but this was wrong ", m_nTestPpIdx + 1);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_Z1_PICKER_CMD_INTERLOCK + pTaskTestPP->GetHandTypeIndex(m_nHandType), AlarmData);
					break;
				}
			}

			NextStepTimer( exe_hand_z_start_position_safety_move_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_z_start_position_safety_move_check:
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
				int nAxisDone = pMotor[i]->AxisDone(vPkr, m_vErrPicker);
				int nErrSen = pTaskTestPP->Chk_Z_Safety_Sen_Check_Type(vPkr,i);
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
				m_nStep = exe_hand_xy_move_target_position_before_100mm;
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z%d Axis Move to Safety Pos](END)", m_szCmdName, m_nStep, m_nHandType + 1);
			}
		}break;

	case exe_hand_xy_move_target_position_before_100mm:
		{
			m_nLast_Executing_Step = exe_hand_xy_move_target_position_before_100mm;

			double dTargetPos_X = 0.0 , dTargetPos_Y = 0.0;
			if(m_nAxisNum == eAxis_X){
				if(m_nTestPpIdx == eTestPP_1)
					dTargetPos_X = m_PcdStart_Pos.x - m_nDistance;
				else
					dTargetPos_X = m_PcdStart_Pos.x + m_nDistance;

				dTargetPos_Y = m_PcdStart_Pos.y;
			}else if( m_nAxisNum == eAxis_Y){
				dTargetPos_X = m_PcdStart_Pos.x;
				dTargetPos_Y = m_PcdStart_Pos.y - m_nDistance;
			}

			dTargetPos_X += m_TargetPos_Offset.x;
			dTargetPos_Y += m_TargetPos_Offset.y;

			MakeVatPreparePos(m_nTestPpIdx, dTargetPos_X, dTargetPos_Y, Move_100mm_Offset);

			int nAxisNo[2] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2, CTaskTestPP::eAxisY };
			double TPos[2] = {dTargetPos_X, dTargetPos_Y};

			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X,Y Axis Move to Target Pos](SRT)",m_szCmdName ,m_nStep);
			for(int i=0; i<2; i++)
			{
				int nErr = pTaskTestPP->do_MovePos_Pcd_backlash(TPos[i], m_vCmdPicker, nAxisNo[i], PCD_TEST_PP_SPD, 0, FALSE, m_nHandType);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d X,Y Axis move to Target pos Cmd Error", m_nTestPpIdx+1);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					return FALSE;
				}

				OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo[i]);
				int nAxisDone = pMotor->AxisDone();
			}

			NextStepTimer( exe_hand_xy_move_check_target_position_before_100mm, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_xy_move_check_target_position_before_100mm:
		{
			if( m_tcTimeout.CheckOverTime() == TRUE ){
				CParamList AlarmData;

				int nAxisDoneX = pMotorX->AxisDone();
				int nAxisDoneY = pMotorZ->AxisDone();
				double dCurPosX = 0.0, dCurPosY = 0.0;
				pMotorX->GetCurrentPos(dCurPosX);
				pMotorY->GetCurrentPos(dCurPosY);

				AlarmData.m_fnAddItemFormat("Test PP %d X,Y Axis move  Error", m_nTestPpIdx+1);
				AlarmData.m_fnAddItemFormat("AxisDoneX = %d, AxisDoneY = %d, SetTime = %.2f", nAxisDoneX, nAxisDoneY, m_tcTimeout.GetCheckTime());
				AlarmData.m_fnAddItemFormat("X Cur Pos = %.3f, Y Cur Pos = %.3f", dCurPosX, dCurPosY);
				ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
				break;
			}

			int nChkComp  = 0;
			int nAxisNo[2] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2, CTaskTestPP::eAxisY };
			for(int i=0; i<2; i++){
				OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo[i]);
				int nAxisDone = pMotor->AxisDone();
				if( nAxisDone == ERR_NO_ERROR ){
					nChkComp++;
				}
			}
			if( nChkComp == 2 ){
				pTaskTestPP->MakeLog("[%s] [X,Y Axis target Move Before 100mm](END)",m_szCmdName ,m_nStep);
				NextStep( exe_hand_xy_move_target_position );
			}
		}break;
	case exe_hand_xy_move_target_position:
		{
			m_nLast_Executing_Step = exe_hand_xy_move_target_position;

			double dTargetPos_X = 0.0 , dTargetPos_Y = 0.0;
			if(m_nAxisNum == eAxis_X){
				//dTargetPos_X = m_PcdStart_Pos.x + m_nDistance;
				if (m_nTestPpIdx == eTestPP_1)
					dTargetPos_X = m_PcdStart_Pos.x - m_nDistance;
				else
					dTargetPos_X = m_PcdStart_Pos.x + m_nDistance;

				dTargetPos_Y = m_PcdStart_Pos.y;
			}else if( m_nAxisNum == eAxis_Y){
				dTargetPos_X = m_PcdStart_Pos.x;
				dTargetPos_Y = m_PcdStart_Pos.y - m_nDistance;
			}

			dTargetPos_X += m_TargetPos_Offset.x;
			dTargetPos_Y += m_TargetPos_Offset.y;


			int nAxisNo[2] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2, CTaskTestPP::eAxisY };
			double TPos[2] = {dTargetPos_X, dTargetPos_Y};

			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X,Y Axis Move to Target Pos](SRT)",m_szCmdName ,m_nStep);
			for(int i=0; i<2; i++){
				int nErr = pTaskTestPP->do_MovePos_Pcd_backlash(TPos[i],m_vCmdPicker, nAxisNo[i], PCD_TEST_PP_SPD, 0, FALSE, m_nHandType);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d X,Y Axis move to Target pos Cmd Error", m_nTestPpIdx+1);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					return FALSE;
				}
			}

			NextStepTimer( exe_hand_xy_move_check_target_position, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_xy_move_check_target_position:
		{
			if( m_tcTimeout.CheckOverTime() == TRUE ){
				CParamList AlarmData;

				int nAxisDoneX = pMotorX->AxisDone();
				int nAxisDoneY = pMotorZ->AxisDone();
				double dCurPosX = 0.0, dCurPosY = 0.0;
				pMotorX->GetCurrentPos(dCurPosX);
				pMotorY->GetCurrentPos(dCurPosY);

				AlarmData.m_fnAddItemFormat("Test PP %d X,Y Axis move  Error", m_nTestPpIdx+1);
				AlarmData.m_fnAddItemFormat("AxisDoneX = %d, AxisDoneY = %d, SetTime = %.2f", nAxisDoneX, nAxisDoneY, m_tcTimeout.GetCheckTime());
				AlarmData.m_fnAddItemFormat("X Cur Pos = %.3f, Y Cur Pos = %.3f", dCurPosX, dCurPosY);
				ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
				break;
			}

			int nChkComp  = 0;
			int nAxisNo[2] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2, CTaskTestPP::eAxisY };
			for(int i=0; i<2; i++){
				OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo[i]);
				int nAxisDone = pMotor->AxisDone();
				if( nAxisDone == ERR_NO_ERROR ){
					nChkComp++;
				}
			}
			if( nChkComp == 2 ){
				pTaskTestPP->MakeLog("[%s] [X,Y Axis Start Move Before 100mm](END)",m_szCmdName ,m_nStep);
				//return to xyz confilct check step
				NextStep( exe_hand_z_target_position_move );
			}
		}break;

	case exe_hand_z_target_position_move:
		{
			m_nLast_Executing_Step = exe_hand_z_target_position_move;

			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [z move target position ] [SRT]", m_szCmdName, m_nStep);
			int nAxisNo = m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;

			nErr = pTaskTestPP->do_MovePos_Pcd_backlash( m_PcdStart_Pos.z, m_vCmdPicker, nAxisNo, PCD_TEST_PP_SPD);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}

			NextStepTimer( exe_hand_z_target_position_move_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_z_target_position_move_check:
		{
			if( m_tcTimeout.CheckOverTime() == TRUE ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				ReportAlarm(ERR_TEST_PP_1_Z1_PICKER_MOVE_TIMEOUT + pTaskTestPP->GetHandTypeIndex(m_nHandType), AlarmData);
				break;
			}
			int nAxisDone = pMotorZ->AxisDone();
			if( nAxisDone == ERR_NO_ERROR ){
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [z move target position ] [END]", m_szCmdName, m_nStep);

				NextStep(exe_visn_recon_target_position);
			}
		}break;
	case exe_visn_recon_target_position:
		{
			m_nLast_Executing_Step = exe_visn_recon_target_position;

			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [backlash target position measurement] [SRT]", m_szCmdName, m_nStep);
			if( is_simulation() ){
				NextStepTimer( exe_visn_recon_target_position_check, VISION_TIME_OUT );
				break;
			}
			int nVATIdx = g_pVisionTestHand_1->GetHandTypeIdx((_eTestPPIdx)m_nTestPpIdx, (eTestPPType)m_nHandType);
			if (m_nTestPpIdx == eTestPP_1)
				g_pVisionTestHand_1->SendMeasure_PcdBacklash(m_nHandType == eTestPPLoadType ? VISION_MEASURE_VAT_TEST_1_LD_UPPER : VISION_MEASURE_VAT_TEST_1_ULD_UPPER, nVATIdx);
			else
				g_pVisionTestHand_2->SendMeasure_PcdBacklash(m_nHandType == eTestPPLoadType ? VISION_MEASURE_VAT_TEST_2_LD_UPPER : VISION_MEASURE_VAT_TEST_2_ULD_UPPER, nVATIdx);

			NextStepTimer( exe_visn_recon_target_position_check, VISION_TIME_OUT );
		}break;

	case exe_visn_recon_target_position_check:
		{
			ST_PCD_BACKLASH_RCV_DATA vRcvData;
			/* CheckMeasureResult() 에서 alarm check 처리 */
			if( CheckMeasureResult(vRcvData) == TRUE ){
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Pcd target position measurement dx=%.3f um, dy=%.3f um] [END]", m_szCmdName, m_nStep,vRcvData.dX,vRcvData.dY);

				m_TargetPos_Offset.x += vRcvData.dX;
				m_TargetPos_Offset.y += vRcvData.dY;

				if(fabs(vRcvData.dX) < m_Cmd_PassRangeUm.x && fabs(vRcvData.dY) < m_Cmd_PassRangeUm.y){
					NextStep( exe_final_z_safety_move );	
					break;
				}

				NextStep(exe_hand_z_start_position_safety_move);
			}
		}break;
	case exe_final_z_safety_move:
		{
			m_nLast_Executing_Step = exe_final_z_safety_move;

			int nAxisNo[2] = { CTaskTestPP::eAxisZ1 ,CTaskTestPP::eAxisZ2 };
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z1,Z2 move safety pos](STR)", m_szCmdName, m_nStep);
			for (int i = 0; i < _countof(nAxisNo); i++)
			{
				int nErr = pTaskTestPP->do_MovePos(LOC_IDX_TEST_PP_HOME, m_nHandType, m_cCmdPocket, m_vSafetyAllPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i], FALSE, PCD_TEST_PP_SPD);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d all picker up command, but this was wrong ", m_nTestPpIdx + 1);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_Z1_PICKER_CMD_INTERLOCK + pTaskTestPP->GetHandTypeIndex(m_nHandType), AlarmData);
					break;
				}
			}

			NextStepTimer( exe_final_z_safety_move_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_final_z_safety_move_check:
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
				int nAxisDone = pMotor[i]->AxisDone(vPkr, m_vErrPicker);
				int nErrSen = pTaskTestPP->Chk_Z_Safety_Sen_Check_Type(vPkr,i);
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
				m_nStep = exe_final_x_safe_move;
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z%d Axis Move to Safety Pos](END)", m_szCmdName, m_nStep, m_nHandType + 1);
			}
		}break;
	case exe_final_x_safe_move:
		{
			int nAxisNo[2] = { CTaskTestPP::eAxisX1 ,CTaskTestPP::eAxisX2 };
			std::vector<CPoint> vPicker;
			vPicker.push_back(CPoint(0, 0));
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X1,X2 Axis Move to Home Pos](STR)", m_szCmdName, m_nStep);
			for (int i = 0; i < _countof(nAxisNo); i++)
			{
				int nErr = pTaskTestPP->do_MovePos_Pcd_backlash(0, m_vSafetyAllPicker, nAxisNo[i], PCD_TEST_PP_SPD, 0, FALSE, m_nHandType);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d y axis move command,but this was wrong", m_nTestPpIdx + 1);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}
			NextStep(exe_final_x_safe_move_check);
		}break;
	case exe_final_x_safe_move_check:
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
					if (m_tcTimeout.CheckOverTime() == TRUE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("TestPP %d x axis move avoid safety position", pTaskTestPP->m_nTestPPIdx + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
						CBaseSeqCmd::ReportAlarm(ERR_TEST_PP_1_MOTOR_ORG_TIMEOUT + pTaskTestPP->m_nTestPPIdx, AlarmData);
						break;
					}

					if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d x axis done fail, move home position", m_nTestPpIdx + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
						ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPpIdx, AlarmData);
					}
				}
			}

			if (nComp == _countof(pMotor))
			{
				NextStep(exe_complete);
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X1,X2 Move Home](END)", m_szCmdName, m_nStep);
			}
		}break;
	case exe_complete:
		{	
			m_nLast_Executing_Step = exe_complete;

			//double dStartOffset = 0.0, dTargetOffset = 0.0;

			double dStartPos = 0.0, dTargetPos = 0.0;

			OneAxis* pMotor = NULL;

			if(m_nAxisNum == eAxis_X){
				dStartPos = m_PcdStart_Pos.x + m_StartPos_Offset.x;

				if (m_nTestPpIdx == eTestPP_1)
					dTargetPos = m_PcdStart_Pos.x - m_nDistance + m_TargetPos_Offset.x;
				else
					dTargetPos = m_PcdStart_Pos.x + m_nDistance + m_TargetPos_Offset.x;
			

				pMotor = m_nHandType == eTestPPLoadType ? pTaskTestPP->GetAxis(CTaskTestPP::eAxisX1) : pTaskTestPP->GetAxis(CTaskTestPP::eAxisX2);
			}else if(m_nAxisNum == eAxis_Y){
				dStartPos = m_PcdStart_Pos.y + m_StartPos_Offset.y;
				dTargetPos = m_PcdStart_Pos.y - m_nDistance + m_TargetPos_Offset.y;
				
				pMotor = pTaskTestPP->GetAxis(CTaskTestPP::eAxisY);
			}

			if( pMotor == NULL){
				SimpleAlarm( ERR_LOT_CMD_PARAMETER_IS_INVALID ); 
				break;
			}

			double dMeasureDistance = fabs(dTargetPos - dStartPos); // Encoder 상으로 측정 된 Vision Target 간 거리.
			double dDesignDistance  = fabs(static_cast<double>(m_nDistance)); // 설계상의 Vision Target 간 거리.

			double dDesignScaleFactor = pMotor->GetScaleFactor(); // Scale Factor 설계치.
			ASSERT( dDesignScaleFactor > 0.01 );
			double dMeasurePulse = dMeasureDistance / dDesignScaleFactor;
			double dMeasureScaleFactor = dDesignDistance / dMeasurePulse;


			pTaskTestPP->MakeLog( "[V.A.T result] PCD AxisNum = %d, Offset = %f ", m_nAxisNum, dMeasureScaleFactor);

			//by kmk [20190221] 실제 이동거리와 이론치랑 뺀 값이 현재 스케일 팩터보다 크면 Offset 값은 구한 값으로 저장하고 다시 스케일 팩터를 구하도록 한다.
			//by kmk [20190221] 실제 이동거리와 이론치랑 뺀 값이 현재 스케일 팩터보다 작다면, 더 이상 정교하게 동작 시킬 수 없으므로 offset 값은 0으로 저장한다.

			double dCurSF = pMotor->GetScaleFactor(); //단위 : um
			double dSubDistance = fabs((dMeasureDistance - (double)m_nDistance)); //실제 이동거리 - 이론치 거리(단위 : um)
			double dOffsetSF =  dSubDistance > dCurSF ? dMeasureScaleFactor - pMotor->GetScaleFactor() : 0;
			if(m_nAxisNum == eAxis_X){
				g_VatPcd.testpp[m_nTestPpIdx][m_nHandType].m_PcdOffset_XY.x = dOffsetSF;
			}else if(m_nAxisNum == eAxis_Y){
				g_VatPcd.testpp[m_nTestPpIdx][m_nHandType].m_PcdOffset_XY.y = dOffsetSF;
			}
			g_VatPcd.testpp[m_nTestPpIdx][m_nHandType].Save(m_nHandType);

			/* notify complete */
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray PP PCD complete");
			AlarmData.m_fnAddItemFormat("%20s : %.5fum/p", _T("Before Scale Factor"), pMotor->GetScaleFactor());
			AlarmData.m_fnAddItemFormat("%20s : %.5fum/p", _T("Current Scale Factor"), dMeasureScaleFactor);
			AlarmData.m_fnAddItemFormat("%15s : %.5fmm", _T("Move Distance"), (double)m_nDistance / 1000);
			AlarmData.m_fnAddItemFormat("%15s : %.5fmm", _T("Motor Encoder"), dMeasureDistance / 1000);
			AlarmData.m_fnAddItemFormat("%s : %.5fmm", _T("GAB"), (dMeasureDistance - (double)m_nDistance) / 1000);
			AlarmData.m_fnAddItemFormat("%s : %.5fum", _T("GAB"), (dMeasureDistance - (double)m_nDistance));
			g_Error.AlarmReport(ERR_VISION_AUTOTEACHING_DONE, AlarmData, &g_TaskSystemCtrl);
				
			ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
		}break;
	}
	return CBaseSeqCmd::State_Executing();
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_VatPcd::State_Pause()
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
BOOL CTestPPCmd_VatPcd::State_Recovery()
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

	OneAxis* pMotorX = m_nHandType == eTestPPLoadType ? pTaskTestPP->GetAxis(CTaskTestPP::eAxisX1) : pTaskTestPP->GetAxis(CTaskTestPP::eAxisX2);
	OneAxis* pMotorY = pTaskTestPP->GetAxis(CTaskTestPP::eAxisY);
	OneAxis* pMotorZ = m_nHandType == eTestPPLoadType ? pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ1) : pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ2);

	switch( m_nStep )
	{
	case 0: // 현재 위치와 마지막 위치를 비교 한다.
		{
			pTaskTestPP->MakeLog("[%s] [RECOVERY=%8d]",m_szCmdName ,m_nStep);
			m_nStep = 1000;
			m_nRetryCount = 0;
			//SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing);
		}break;
	case 1000:
		{
			int nAxisNo[2] = { CTaskTestPP::eAxisZ1 ,CTaskTestPP::eAxisZ2 };
			pTaskTestPP->MakeLog("[%s] [RECOVERY=%8d] [Z1,Z2 move %s safety pos](STR)", m_szCmdName, m_nStep);
			for (int i = 0; i < _countof(nAxisNo); i++)
			{
				int nErr = pTaskTestPP->do_MovePos(LOC_IDX_TEST_PP_HOME, m_nHandType, m_cCmdPocket, m_vSafetyAllPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i],FALSE, PCD_TEST_PP_SPD);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d all picker up command, but this was wrong ", m_nTestPpIdx + 1);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_Z1_PICKER_CMD_INTERLOCK + pTaskTestPP->GetHandTypeIndex(m_nHandType), AlarmData);
					break;
				}
			}
			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_SHORT);
			m_tcTimeout.StartTimer();
			m_nStep = 1001;
		}break;
	case 1001:
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
				int nAxisDone = pMotor[i]->AxisDone(vPkr, m_vErrPicker);
				int nErrSen = pTaskTestPP->Chk_Z_Safety_Sen_Check_Type(vPkr,i);
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
				pTaskTestPP->MakeLog("[%s] [RECOVERY=%8d] [Z%d Axis Move to Safety Pos](END)", m_szCmdName, m_nStep, m_nHandType + 1);
			}
		}break;
	case 2000:
		{
			int nAxisNo[2] = { CTaskTestPP::eAxisX1 ,CTaskTestPP::eAxisX2 };
			std::vector<CPoint> vPicker;
			vPicker.push_back(CPoint(0, 0));
			pTaskTestPP->MakeLog("[%s] [RECOVERY=%8d] [X1,X2 Axis Move to Home Pos](STR)", m_szCmdName, m_nStep);
			for (int i = 0; i < _countof(nAxisNo); i++)
			{
				int nErr = pTaskTestPP->do_MovePos(LOC_IDX_TEST_PP_HOME, m_nHandType, m_cCmdPocket, m_vSafetyAllPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i], FALSE, PCD_TEST_PP_SPD);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d y axis move command,but this was wrong", m_nTestPpIdx + 1);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}
			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_SHORT);
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
	case 2001:
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
					if (m_tcTimeout.CheckOverTime() == TRUE)
					{			
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
					}
				}
			}

			if (nComp == _countof(pMotor))
			{
				m_nStep = 3000;
				pTaskTestPP->MakeLog("[%s] [RECOVERY=%8d] [X1,X2 Move Home](END)", m_szCmdName, m_nStep);
			}
		}break;
	case 3000:
		{
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
BOOL CTestPPCmd_VatPcd::State_NormalComplete()
{
	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_VatPcd::State_Alarm()
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
BOOL CTestPPCmd_VatPcd::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete(); 
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_VatPcd::State_Aborting()
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
BOOL CTestPPCmd_VatPcd::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete(); 
}


//==============================================================================
// local implementation
//==============================================================================
void CTestPPCmd_VatPcd::NextStep( execute_step_define next )
{
	m_nStep = next;
}

void CTestPPCmd_VatPcd::NextStepTimer( execute_step_define next , double tm )
{
	m_tcTimeout.SetCheckTime( tm );
	m_tcTimeout.StartTimer();
	NextStep( next );
}

void CTestPPCmd_VatPcd::SimpleAlarm( int alarm_id )
{
	CParamList AlarmData;
	ReportAlarm( alarm_id, AlarmData );
}

BOOL CTestPPCmd_VatPcd::CheckMeasureResult( ST_PCD_BACKLASH_RCV_DATA& result )
{
	if( m_tcTimeout.CheckOverTime() == TRUE ){
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Test PP %d WebCam receive data time over!!", m_nTestPpIdx+1);
		ReportAlarm(ERR_TEST_PP1_VISION_CMD_RESP_TIME_OUT + m_nTestPpIdx, AlarmData);
		return FALSE;
	}

	/*vision align total time check*/
	//if( m_tcFineAdjTimeout.CheckOverTime() == TRUE ){
	//	CParamList AlarmData;
	//	AlarmData.m_fnAddItemFormat("Test PP %d WebCam retry time over!!", m_nTestPpIdx+1);
	//	ReportAlarm(ERR_VISION_CMD_RETRY_COUNT_OVER, AlarmData);
	//	return FALSE;
	//}
	
		
	if (m_nTestPpIdx == eTestPP_1) {
		int nVATIdx = g_pVisionTestHand_1->GetHandTypeIdx((_eTestPPIdx)m_nTestPpIdx, (eTestPPType)m_nHandType);
		g_pVisionTestHand_1->GetReceiveData(nVATIdx, result);
	}	
	else
	{
		int nVATIdx = g_pVisionTestHand_2->GetHandTypeIdx((_eTestPPIdx)m_nTestPpIdx, (eTestPPType)m_nHandType);
		g_pVisionTestHand_2->GetReceiveData(nVATIdx, result);
	}
	

	if( is_simulation() && m_tcTimeout.MoreThan( 100 /*ms*/ ) ) {
		int rand_range = 500;
		result.dX = ((rand()%rand_range*2)-rand_range);
		result.dY = ((rand()%rand_range*2)-rand_range);
		result.nState = CAM_STATUS_SUCCESS;
	}

	if( result.nState != CAM_STATUS_IDLE){
		int nErrCnt = 0; 
		CParamList AlarmData;
		switch(result.nState)
		{
		case CAM_STATUS_DISCONNECT	 : { AlarmData.m_fnAddItemFormat("Test PP %d WebCam disconnect!!", m_nTestPpIdx+1); nErrCnt++; } break;
		case CAM_STATUS_TIMEOUT		 : { AlarmData.m_fnAddItemFormat("Test PP %d WebCam time out!!"  , m_nTestPpIdx+1); nErrCnt++; } break;
		case CAM_STATUS_FAILED		 : { AlarmData.m_fnAddItemFormat("Test PP %d WebCam failed!!"    , m_nTestPpIdx+1); nErrCnt++; } break;
		case CAM_STATUS_UNKNOWN		 : { AlarmData.m_fnAddItemFormat("Test PP %d WebCam unknown!!"   , m_nTestPpIdx+1); nErrCnt++; } break;
		case CAM_STATUS_LIGHT_FAILED : { AlarmData.m_fnAddItemFormat("Test PP %d WebCam light fail!!", m_nTestPpIdx+1); nErrCnt++; } break;	
		}
		if( nErrCnt != 0 ){
			ReportAlarm(ERR_TEST_PP1_VISION_RESULT_NG + m_nTestPpIdx, AlarmData);
			return FALSE;
		}

		if( result.nState == CAM_STATUS_SUCCESS ){
			return TRUE;
		}
	}
	return FALSE;
}

void CTestPPCmd_VatPcd::MakeVatPreparePos(int nIndex, double &dX, double &dY, double dPrepareOffset)
{
	double dNegaLimitX = 0.0;
	double dPosiLimitY = 0.0;
	int nAxisNumber;
	if (nIndex == 0) 
	{
		if (m_nHandType == eTestPPLoadType)
		{
			nAxisNumber = eAXIS_TEST_PP_1_X;
		}
		else {
			nAxisNumber = eAXIS_TEST_PP_1_X2;
		}
		dNegaLimitX = g_Motor[nAxisNumber]->GetSwLimitNega();
		dPosiLimitY = g_Motor[eAXIS_TEST_PP_1_Y]->GetSwLimitPosi();
	}
	else
	{
		if (m_nHandType == eTestPPLoadType)
		{
			nAxisNumber = eAXIS_TEST_PP_2_X;
		}
		else {
			nAxisNumber = eAXIS_TEST_PP_2_X2;
		}

		dNegaLimitX = g_Motor[nAxisNumber]->GetSwLimitNega();
		dPosiLimitY = g_Motor[eAXIS_TEST_PP_2_Y]->GetSwLimitPosi();
	}

	if (m_nAxisNum == eAxis_X) 
	{
		double dPreparePosX = 0; //dX - dPrepareOffset;
		if(m_nTestPpIdx == eTestPP_1)
			dPreparePosX = dX + dPrepareOffset;
		else
			dPreparePosX = dX - dPrepareOffset;

		dX = (dPreparePosX <= dNegaLimitX) ? (dNegaLimitX) : (dPreparePosX);
	}
	else
	{
		double dPreparePosY = dY - dPrepareOffset;
		dY = (dPreparePosY >= dPosiLimitY) ? (dPosiLimitY) : (dPreparePosY);
	}
}