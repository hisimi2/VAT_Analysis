
#include "StdAfx.h"
#include "TestPPCmd_VatBacklash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/* simulation code */
#define is_simulation() ((g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) ? TRUE : FALSE)
/* simulation code */
#define PICK_PLACE_Z_OFFSET 1500
#define PCD_TEST_PP_SPD				(15)
//==============================================================================
//
//==============================================================================
CTestPPCmd_VatBacklash::CTestPPCmd_VatBacklash(int nTestPpIdx, CDlgPcdBacklash *pParent)
	: m_nTestPpIdx(nTestPpIdx)
{
	m_pParentWnd = pParent;
	//m_nCmdId = eSeqCmd_TestPP_AutoVacZ;
	m_nRetryCount = 0;
	sprintf_s(m_szCmdName, sizeof(m_szCmdName), "CTestPPCmd_VatBacklash");
	m_bBacklashTest = FALSE;
	/*param default*/
	m_nLoopCnt = 0;
	m_nDistanceCnt = 0;
	m_nSpeed = 0;
	m_qSpeed.clear();

	m_dUpperSafety = 0.0;
	m_dLowerSafety = 0.0;
	m_nAxisNum = eAxis_X;

	m_bIsContinue = FALSE;
	m_bRepeat = FALSE;
	m_nRepeatCnt = 1;
	m_nRepeatTempCnt = 1;

	m_cTime = CTime::GetCurrentTime();

	m_BacklashXTestLog.SetFileName("D:\\techwing\\LOG\\BacklashXTest.log");
	m_BacklashYTestLog.SetFileName("D:\\techwing\\LOG\\BacklashYTest.log");

	m_nLast_Executing_Step = exe_start;

	m_nChkLoopCnt = 0;
	m_nChkDistanceCnt = 0;

	m_nHandType = eTestPPLoadType;

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
CTestPPCmd_VatBacklash::~CTestPPCmd_VatBacklash(void){}

//==============================================================================
//
//==============================================================================
int CTestPPCmd_VatBacklash::CheckParam(){return ERR_NO_ERROR;}


//==============================================================================
//
//==============================================================================
int  CTestPPCmd_VatBacklash::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTestPPCmd_VatBacklash::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTestPPCmd_VatBacklash::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTestPPCmd_VatBacklash::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTestPPCmd_VatBacklash::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_VatBacklash::State_Setup()
{
	double dTimeout = 60.0;
	m_tcTimeout.SetCheckTime( dTimeout );
	m_tcTimeout.StartTimer();

	return CBaseSeqCmd::State_Setup();
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_VatBacklash::State_Executing()
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

	OneAxis* pMotorX = m_nHandType == eTestPPLoadType ? pTaskTestPP->GetAxis(CTaskTestPP::eAxisX1) : pTaskTestPP->GetAxis(CTaskTestPP::eAxisX2);
	OneAxis* pMotorY = pTaskTestPP->GetAxis(CTaskTestPP::eAxisY);
	OneAxis* pMotorZ = m_nHandType == eTestPPLoadType ? pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ1) : pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ2);

	switch( m_nStep )
	{		
	case exe_start:
		{
			if( m_qSpeed.size() == 0) nErr++;

			if(m_nLoopCnt < 0 || m_nDistanceCnt < 0)nErr++;

			if( nErr ){
				SimpleAlarm( ERR_LOT_CMD_PARAMETER_IS_INVALID );
				break;
			} 

			m_StartPos  = g_VatBacklash.testpp[m_nTestPpIdx][m_nHandType].m_BacklashStartPos_XY;
			m_TargetPos = g_VatBacklash.testpp[m_nTestPpIdx][m_nHandType].m_BacklashTargetPos_XYZ;
			m_EndPos    = g_VatBacklash.testpp[m_nTestPpIdx][m_nHandType].m_BacklashEndPos_XY;

			m_dUpperSafety = g_VatPcd.testpp[m_nTestPpIdx][m_nHandType].m_dUpperSafetyPos * 1000;
			m_dLowerSafety = g_VatPcd.testpp[m_nTestPpIdx][m_nHandType].m_dLowerSafetyPos * 1000;

			double dNegaLimitX = pMotorX->GetSwLimitNega();
			double dPosiLimitX = pMotorX->GetSwLimitPosi();
			int nLimitError = ERR_NO_ERROR;

			if (m_nTestPpIdx == eTestPP_1)
			{
				if (m_StartPos.x < dNegaLimitX)
				{
					nLimitError = ERR_MOTOR_CMN_SOFTWARE_POSITIVE_LIMIT_HIT;
				}
				else if(m_EndPos.x > dPosiLimitX)
				{
					nLimitError = ERR_MOTOR_CMN_SOFTWARE_NEGATIVE_LIMIT_HIT;
				}			
			}
			else
			{
				if (m_StartPos.x < dNegaLimitX)
				{
					nLimitError = ERR_MOTOR_CMN_SOFTWARE_NEGATIVE_LIMIT_HIT;
				}
				else if (m_EndPos.x > dPosiLimitX)
				{
					nLimitError = ERR_MOTOR_CMN_SOFTWARE_POSITIVE_LIMIT_HIT;
				}
			}

			if (nLimitError)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d X%d", m_nTestPpIdx + 1, m_nHandType+1);
				ReportAlarm(nLimitError, AlarmData);
				break;
			}
			
		
			OneAxis* pMotorX = m_nHandType == eTestPPLoadType ? pTaskTestPP->GetAxis(CTaskTestPP::eAxisX1) : pTaskTestPP->GetAxis(CTaskTestPP::eAxisX2);

			m_vStartToTargetOffset.clear();
			m_vEndToTargetOffset.clear();
			m_vBacklashAdjust.clear();

			m_nChkLoopCnt = 0;
			m_nChkDistanceCnt = 0;
			
			m_nSpeed = m_qSpeed.front();
			m_qSpeed.pop_front();
			m_pParentWnd->ReportBacklashResult(m_nAxisNum, m_nSpeed);

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
				m_nStep = exe_backlash_loop_entry;
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z%d Axis Move to Safety Pos](END)", m_szCmdName, m_nStep, m_nHandType + 1);
			}
		}break;

	case exe_backlash_loop_entry:
		{
// 			if (m_nAxisNum == eAxis_X)
// 			{
// 				if (m_nDistanceCnt > m_nChkDistanceCnt)
// 				{
// 					NextStep(exe_hand_confict_check);
// 					break;
// 				}
// 				//  [12/17/2020 donghyun.shin]
// 				m_nChkLoopCnt++;
// 			}

			if(m_nLoopCnt <= m_nChkLoopCnt){
				CalcBacklashOffset(m_nSpeed, m_bBacklashTest,m_nHandType);
				WriteRepeatOffsetXY(m_vEndToTargetOffset, m_vStartToTargetOffset, m_nSpeed); //Backlash 반복 Log 남기기
				m_vEndToTargetOffset.clear();
				m_vStartToTargetOffset.clear();
				m_nChkLoopCnt = 0;
				m_nChkDistanceCnt = 0;

				if(m_qSpeed.size() == 0 )
				{
					if(m_bBacklashTest == TRUE)
					{
						NextStep(exe_final_x_safe_move);
						break;
					}
					else
					{
						if(m_bRepeat == FALSE)
						{
							NextStep(exe_final_x_safe_move);
							break;
						}
						else
						{
							InitLoopStatus();
							NextStep(exe_start);
						}
						break;
					}
				}else{
					m_nSpeed = m_qSpeed.front();
					m_qSpeed.pop_front();
					m_pParentWnd->ReportBacklashResult(m_nAxisNum, m_nSpeed);
				}
			}

			m_nChkLoopCnt++;
			m_nChkDistanceCnt = 0;
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d][Backlash Start Count : %d, Speed : %d ]",m_szCmdName ,m_nStep, m_nChkLoopCnt, m_nSpeed);
			NextStep( exe_hand_confict_check );
		}break;

		// x,y,z confilct check
	case exe_hand_confict_check:
		{
			m_nLast_Executing_Step = exe_hand_confict_check;

			// 축별 도피에 대한 점검
			 // Z축 안전 위치
			int nSafetySenX1 = pTaskTestPP->ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON);
			int nSafetySenX2 = pTaskTestPP->ChkDI(CTaskTestPP::xX2AXIS_POSITION_SEN, DEF_ON);
			int nErrSenZ1 = pTaskTestPP->Chk_Z_Safety_Sen_All(eTestPPLoadType);
			int nErrSenZ2 = pTaskTestPP->Chk_Z_Safety_Sen_All(eTestPPUnLoadType);
			if (nErrSenZ1 == ERR_NO_ERROR && nErrSenZ2 == ERR_NO_ERROR/*|| pMotorZ->GetHomeSensor() == DEF_ON*/) {
				// X1,X2 Home Or Z축 상하단 이동 가능

				if (nSafetySenX1 == ERR_NO_ERROR && nSafetySenX2 == ERR_NO_ERROR)
				{
					NextStep(exe_hand_z_move_safety_down_position);
				}
				else {
					NextStep(exe_hand_anti_conflict_x_home);
				}
			}
			// Z축 안전 위치 이동
			else 
			{
				//if (/*pMotorY->GetHomeSensor() == DEF_ON*/nSafetySen == ERR_NO_ERROR)
				//{
				NextStep(exe_hand_anti_conflict_z_move);
			}
		}break;

		// anti-confict Z move
	case exe_hand_anti_conflict_z_move:
		{
			int nAxisNo[2] = { CTaskTestPP::eAxisZ1 ,CTaskTestPP::eAxisZ2 };
			std::vector<CPoint> vPicker;
			vPicker.push_back(CPoint(0, 0));
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z1,Z2 Axis Move to Home Pos](STR)", m_szCmdName, m_nStep);
			for (int i = 0; i < _countof(nAxisNo); i++)
			{
				int nErr = pTaskTestPP->do_MovePos(LOC_IDX_TEST_PP_HOME, m_nHandType, m_cCmdPocket, m_vSafetyAllPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i], FALSE, PCD_TEST_PP_SPD);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d Z axis move command,but this was wrong", m_nTestPpIdx + 1);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}
			m_tcTimeout.SetCheckTime(TIME_OUT_MIDDLE);
			m_tcTimeout.StartTimer();
			
			NextStepTimer( exe_hand_anti_conflict_z_move_check, (MOTOR_TIME_OUT_MIDDLE/2.0) );
		}break;
	case exe_hand_anti_conflict_z_move_check:
		{
			OneAxis* pMotorZ1 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ1);
			OneAxis* pMotorZ2 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ2);

			OneAxis* pMotor[2] = { pMotorZ1 ,pMotorZ2 };
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
						AlarmData.m_fnAddItemFormat("TestPP %d z axis move avoid safety position", pTaskTestPP->m_nTestPPIdx + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
						CBaseSeqCmd::ReportAlarm(ERR_TEST_PP_1_Z1_PICKER_MOVE_TIMEOUT + pTaskTestPP->GetHandTypeIndex(i), AlarmData);
						break;
					}

					if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d z axis done fail, move home position", m_nTestPpIdx + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
						ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPpIdx, AlarmData);
					}
				}
			}

			if (nComp == _countof(pMotor))
			{
				NextStep(exe_hand_anti_conflict_x_home);
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X1,X2 Move Home](END)", m_szCmdName, m_nStep);
			}
		}break;

		// move y to home -> move z to target pos
	case exe_hand_anti_conflict_x_home:
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
					AlarmData.m_fnAddItemFormat("Test PP %d x axis move command,but this was wrong", m_nTestPpIdx + 1);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}
			m_tcTimeout.SetCheckTime(TIME_OUT_MIDDLE);
			m_tcTimeout.StartTimer();

			NextStepTimer(exe_hand_anti_conflict_x_home_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_anti_conflict_x_home_check:
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
						CBaseSeqCmd::ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + pTaskTestPP->m_nTestPPIdx, AlarmData);
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
				NextStep(exe_hand_z_move_safety_down_position_check);
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X1,X2 Move Home](END)", m_szCmdName, m_nStep);
			}
		}break;

	case exe_hand_z_move_safety_down_position:
		{
			m_nLast_Executing_Step = exe_hand_z_move_safety_down_position;

			int nAxisNo[2] = { CTaskTestPP::eAxisZ1 ,CTaskTestPP::eAxisZ2 };
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z1,Z2 move safety pos](STR)", m_szCmdName, m_nStep);
			for (int i = 0; i < _countof(nAxisNo); i++)
			{
				int nErr = pTaskTestPP->do_MovePos(LOC_IDX_TEST_PP_HOME, m_nHandType, m_cCmdPocket, m_vSafetyAllPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i], FALSE, PCD_TEST_PP_SPD);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d z all picker up command, but this was wrong ", m_nTestPpIdx + 1);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_Z1_PICKER_CMD_INTERLOCK + pTaskTestPP->GetHandTypeIndex(m_nHandType), AlarmData);
					break;
				}
			}

			NextStepTimer( exe_hand_z_move_safety_down_position_check, MOTOR_TIME_OUT_LONG );
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
		int nAxisNo[1] = {CTaskTestPP::eAxisY };
		double TPos[1] = { m_StartPos.y };

		pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Y Axis Move to Start Pos](SRT)", m_szCmdName, m_nStep);
		for (int i = 0; i < _countof(nAxisNo); i++)
		{
			int nErr = pTaskTestPP->do_MovePos_Pcd_backlash(TPos[i], m_vCmdPicker, nAxisNo[i], m_nSpeed, 0, m_bBacklashTest);
			if (nErr != ERR_NO_ERROR)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d Y Axis move to Start pos Cmd Error", m_nTestPpIdx + 1);
				OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				return FALSE;
			}
		}

		NextStepTimer(exe_hand_y_move_start_position_check, MOTOR_TIME_OUT_LONG);
	}break;
	case exe_hand_y_move_start_position_check:
	{
		if (m_tcTimeout.CheckOverTime() == TRUE) {
			CParamList AlarmData;


			int nAxisDoneY = pMotorY->AxisDone();
			double dCurPosY = 0.0;
			pMotorY->GetCurrentPos(dCurPosY);


			AlarmData.m_fnAddItemFormat("Test PP %d X,Y Axis move to Start pos Error", m_nTestPpIdx + 1);
			AlarmData.m_fnAddItemFormat("AxisDoneY = %d, SetTime = %.2f", nAxisDoneY, m_tcTimeout.GetCheckTime());
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
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X,Y Axis Move to Start Pos](END)", m_szCmdName, m_nStep);
			NextStep(exe_hand_xy_move_start_position);
		}
		m_bIsContinue = TRUE;
	}break;

	case exe_hand_xy_move_start_position:
		{
			int nAxisNo[2] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2, CTaskTestPP::eAxisY };
			double TPos[2] = {m_StartPos.x, m_StartPos.y};

			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X,Y Axis Move to Start Pos](SRT)",m_szCmdName ,m_nStep);
			for(int i=0; i<2; i++)
			{
				int nErr = pTaskTestPP->do_MovePos_Pcd_backlash(TPos[i],m_vCmdPicker, nAxisNo[i], m_nSpeed , 0, m_bBacklashTest);
				if( nErr != ERR_NO_ERROR )
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d X,Y Axis move to Start pos Cmd Error", m_nTestPpIdx+1);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					return FALSE;
				}
			}

			NextStepTimer( exe_hand_xy_move_start_position_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_xy_move_start_position_check:
		{
			if( m_tcTimeout.CheckOverTime() == TRUE ){
				CParamList AlarmData;

				int nAxisDoneX = pMotorX->AxisDone();
				int nAxisDoneY = pMotorY->AxisDone();
				double dCurPosX = 0.0, dCurPosY = 0.0;
				pMotorX->GetCurrentPos(dCurPosX);
				pMotorY->GetCurrentPos(dCurPosY);


				AlarmData.m_fnAddItemFormat("Test PP %d X,Y Axis move to Start pos Error", m_nTestPpIdx+1);
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
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X,Y Axis Move to Start Pos](END)",m_szCmdName ,m_nStep);
				NextStep( exe_hand_xy_move_start_to_target_position );
			}
			m_bIsContinue = TRUE;
		}break;

	case exe_hand_xy_move_start_to_target_position:
		{
			int nAxisNo[2] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2, CTaskTestPP::eAxisY };
			double TPos[2] = {m_TargetPos.x, m_TargetPos.y};

			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X,Y Axis Move to Target Pos](SRT)",m_szCmdName ,m_nStep);
			for(int i=0; i<2; i++){
				int nErr = pTaskTestPP->do_MovePos_Pcd_backlash(TPos[i], m_vCmdPicker, nAxisNo[i], m_nSpeed , 0, m_bBacklashTest);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d X,Y Axis move to Target pos Cmd Error", m_nTestPpIdx+1);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					return FALSE;
				}
			}

			NextStepTimer( exe_hand_xy_move_start_to_target_position_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_xy_move_start_to_target_position_check:
		{
			if( m_tcTimeout.CheckOverTime() == TRUE ){
				CParamList AlarmData;

				int nAxisDoneX = pMotorX->AxisDone();
				int nAxisDoneY = pMotorY->AxisDone();
				double dCurPosX = 0.0, dCurPosY = 0.0;
				pMotorX->GetCurrentPos(dCurPosX);
				pMotorY->GetCurrentPos(dCurPosY);
				AlarmData.m_fnAddItemFormat("Test PP %d X,Y Axis move to Target pos Error", m_nTestPpIdx+1);
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
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X,Y Axis Move to Target Pos](END)",m_szCmdName ,m_nStep);
				NextStep( exe_hand_z_move_start_to_target_position );
			}
		}break;
	case exe_hand_z_move_start_to_target_position:
		{
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [z move target position ] [SRT]", m_szCmdName, m_nStep);

			int nAxisNo = m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
			nErr = pTaskTestPP->do_MovePos_Pcd_backlash( m_TargetPos.z, m_vCmdPicker, nAxisNo, PCD_TEST_PP_SPD);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}

			NextStepTimer( exe_hand_z_move_start_to_target_position_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_z_move_start_to_target_position_check:
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
				Sleep(1000);
				NextStep(exe_visn_recon_start_to_target_pos);
			}
		}break;
	case exe_visn_recon_start_to_target_pos:
		{
			m_nLast_Executing_Step = exe_visn_recon_start_to_target_pos;

			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Pcd target position measurement] [SRT]", m_szCmdName, m_nStep);
			if( is_simulation() ){
				NextStepTimer( exe_visn_recon_start_to_target_pos_check, VISION_TIME_OUT );
				break;
			}

			int nVATIdx = g_pVisionTestHand_1->GetHandTypeIdx((_eTestPPIdx)m_nTestPpIdx, (eTestPPType)m_nHandType);
			if(m_nTestPpIdx == eTestPP_1)
				g_pVisionTestHand_1->SendMeasure_PcdBacklash(m_nHandType == eTestPPLoadType ? VISION_MEASURE_VAT_TEST_1_LD_UPPER: VISION_MEASURE_VAT_TEST_1_ULD_UPPER, nVATIdx);
			else
				g_pVisionTestHand_2->SendMeasure_PcdBacklash(m_nHandType == eTestPPLoadType ? VISION_MEASURE_VAT_TEST_2_LD_UPPER : VISION_MEASURE_VAT_TEST_2_ULD_UPPER, nVATIdx);

			NextStepTimer( exe_visn_recon_start_to_target_pos_check, VISION_TIME_OUT );
		}break;

	case exe_visn_recon_start_to_target_pos_check:
		{
			ST_PCD_BACKLASH_RCV_DATA vRcvData;
			/* CheckMeasureResult() 에서 alarm check 처리 */
			if( CheckMeasureResult(vRcvData) == TRUE ){
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Pcd target position measurement dx=%.3f um, dy=%.3f um] [END]", m_szCmdName, m_nStep,vRcvData.dX,vRcvData.dY);

				CPointF Temp;
				Temp.x = vRcvData.dX;
				Temp.y = vRcvData.dY;

				m_vStartToTargetOffset.push_back(Temp);

				m_StartOffset = Temp;

				NextStep( exe_hand_z_start_to_target_safety_move );
			}
		}break;
	case exe_hand_z_start_to_target_safety_move:
		{
			m_nLast_Executing_Step = exe_hand_z_start_to_target_safety_move;

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

			NextStepTimer( exe_hand_z_start_to_target_safety_move_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_z_start_to_target_safety_move_check:
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
				m_nStep = exe_hand_xy_move_end_position;
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z%d Axis Move to Safety Pos](END)", m_szCmdName, m_nStep, m_nHandType + 1);
			}
		}break;
	case exe_hand_xy_move_end_position:
		{
			int nAxisNo[2] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2, CTaskTestPP::eAxisY };
			double TPos[2] = { m_EndPos.x, 0.0 };

// 			if (m_nAxisNum == eAxis_X)
// 			{
// 				TPos[1] = { m_EndPos.y };
// 			}
// 			else 
// 			{
// 				TPos[1] = { m_EndPos.y };
// 			}

			TPos[1] = { m_EndPos.y };

			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X,Y Axis Move to End Pos](SRT)",m_szCmdName ,m_nStep);
			for(int i=0; i<2; i++){
				int nErr = pTaskTestPP->do_MovePos_Pcd_backlash(TPos[i], m_vSafetyAllPicker, nAxisNo[i], m_nSpeed, 0, m_bBacklashTest);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d X,Y Axis move to End pos Cmd Error", m_nTestPpIdx+1);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					return FALSE;
				}
			}

			NextStepTimer( exe_hand_xy_move_end_position_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_xy_move_end_position_check:
		{
			if( m_tcTimeout.CheckOverTime() == TRUE ){
				CParamList AlarmData;

				int nAxisDoneX = pMotorX->AxisDone();
				int nAxisDoneY = pMotorY->AxisDone();
				double dCurPosX = 0.0, dCurPosY = 0.0;
				pMotorX->GetCurrentPos(dCurPosX);
				pMotorY->GetCurrentPos(dCurPosY);
				AlarmData.m_fnAddItemFormat("Test PP %d X,Y Axis move to End pos Error", m_nTestPpIdx+1);
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
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X,Y Axis Move to End Pos](END)",m_szCmdName ,m_nStep);
				NextStep( exe_hand_z_end_safety_move );
			}
		}break;
	case exe_hand_z_end_safety_move:
		{
			m_nLast_Executing_Step = exe_hand_z_end_safety_move;

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
			NextStepTimer( exe_hand_z_end_safety_move_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_z_end_safety_move_check:
		{
			if( m_tcTimeout.CheckOverTime() == TRUE ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				ReportAlarm(ERR_TEST_PP_1_Z1_PICKER_MOVE_TIMEOUT + pTaskTestPP->GetHandTypeIndex(m_nHandType), AlarmData);
				break;
			}
			int nAxisDone = pMotorZ->AxisDone();
			if( nAxisDone == ERR_NO_ERROR ){
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [z safety move] [STR]", m_szCmdName, m_nStep);
				NextStepTimer( exe_hand_z_end_safety_picker_check, TIME_OUT_SHORT );
			}
		}break;
	case exe_hand_z_end_safety_picker_check:
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
				m_nStep = exe_hand_xy_move_end_to_target_position;
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Z%d Axis Move to Safety Pos](END)", m_szCmdName, m_nStep, m_nHandType + 1);
			}
		}break;
	case exe_hand_xy_move_end_to_target_position:
		{
			int nAxisNo[2] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2, CTaskTestPP::eAxisY };
			double TPos[2] = {m_TargetPos.x, m_TargetPos.y};

			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X,Y Axis Move to Target Pos](SRT)",m_szCmdName ,m_nStep);
			for(int i=0; i<2; i++){
				int nErr = pTaskTestPP->do_MovePos_Pcd_backlash(TPos[i],m_vCmdPicker, nAxisNo[i], m_nSpeed, 0, m_bBacklashTest);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d X,Y Axis move to Target pos Cmd Error", m_nTestPpIdx+1);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					return FALSE;
				}
			}

			NextStepTimer( exe_hand_xy_move_end_to_target_position_check, MOTOR_TIME_OUT_LONG );
		}break;

	case exe_hand_xy_move_end_to_target_position_check:
		{
			if( m_tcTimeout.CheckOverTime() == TRUE ){
				CParamList AlarmData;

				int nAxisDoneX = pMotorX->AxisDone();
				int nAxisDoneY = pMotorY->AxisDone();
				double dCurPosX = 0.0, dCurPosY = 0.0;
				pMotorX->GetCurrentPos(dCurPosX);
				pMotorY->GetCurrentPos(dCurPosY);
				AlarmData.m_fnAddItemFormat("Test PP %d X,Y Axis move to Target pos Error", m_nTestPpIdx+1);
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
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [X,Y Axis Move to Target Pos](END)",m_szCmdName ,m_nStep);
				NextStep( exe_hand_z_move_end_to_target_position );
			}
		}break;
	case exe_hand_z_move_end_to_target_position:
		{
			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [z safety move] [STR]", m_szCmdName, m_nStep);
			int nAxisNo = m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;

			nErr = pTaskTestPP->do_MovePos_Pcd_backlash( m_TargetPos.z, m_vCmdPicker, nAxisNo, PCD_TEST_PP_SPD);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			NextStepTimer( exe_hand_z_move_end_to_target_position_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_z_move_end_to_target_position_check:
		{
			if( m_tcTimeout.CheckOverTime() == TRUE ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				ReportAlarm(ERR_TEST_PP_1_Z1_PICKER_MOVE_TIMEOUT + pTaskTestPP->GetHandTypeIndex(m_nHandType), AlarmData);
				break;
			}
			int nAxisDone = pMotorZ->AxisDone();
			if( nAxisDone == ERR_NO_ERROR ){
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [z safety move] [STR]", m_szCmdName, m_nStep);
				Sleep(1000);
				NextStepTimer( exe_visn_recon_end_to_target_pos, TIME_OUT_SHORT );
			}
		}break;
	case exe_visn_recon_end_to_target_pos:
		{
			m_nLast_Executing_Step = exe_visn_recon_end_to_target_pos;

			pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Pcd target position measurement] [SRT]", m_szCmdName, m_nStep);
			if( is_simulation() ){
				NextStepTimer( exe_visn_recon_end_to_target_pos_check, VISION_TIME_OUT );
				break;
			}
			int nVATIdx = g_pVisionTestHand_1->GetHandTypeIdx((_eTestPPIdx)m_nTestPpIdx, (eTestPPType)m_nHandType);
			if (m_nTestPpIdx == eTestPP_1)
				g_pVisionTestHand_1->SendMeasure_PcdBacklash(m_nHandType == eTestPPLoadType ? VISION_MEASURE_VAT_TEST_1_LD_UPPER : VISION_MEASURE_VAT_TEST_1_ULD_UPPER, nVATIdx);
			else
				g_pVisionTestHand_2->SendMeasure_PcdBacklash(m_nHandType == eTestPPLoadType ? VISION_MEASURE_VAT_TEST_2_LD_UPPER : VISION_MEASURE_VAT_TEST_2_ULD_UPPER, nVATIdx);

			NextStepTimer( exe_visn_recon_end_to_target_pos_check, VISION_TIME_OUT );
		}break;
	case exe_visn_recon_end_to_target_pos_check:
		{
			ST_PCD_BACKLASH_RCV_DATA vRcvData;
			/* CheckMeasureResult() 에서 alarm check 처리 */
			if( CheckMeasureResult(vRcvData) == TRUE ){
				pTaskTestPP->MakeLog("[%s] [EXEC=%8d] [Pcd target position measurement dx=%.3f um, dy=%.3f um] [END]", m_szCmdName, m_nStep,vRcvData.dX,vRcvData.dY);

				CPointF Temp;

				Temp.x = vRcvData.dX;
				Temp.y = vRcvData.dY;

				m_vEndToTargetOffset.push_back(Temp);

				double dx = Temp.x - m_StartOffset.x;
				double dy = Temp.y - m_StartOffset.y;
				CString straa = _T("");
				straa.Format("Speed %d, Backslash Off set, X : %.3f, Y : %.3f", m_nSpeed, dx / 1000.0 ,dy / 1000.0);
				g_TaskSystemCtrl.MakeLog(straa);

				m_StartOffset.x = 0.0;
				m_StartOffset.y = 0.0;

				NextStep( exe_hand_z_end_to_target_safety_move );
			}
		}break;
	case exe_hand_z_end_to_target_safety_move:
		{
			m_nLast_Executing_Step = exe_hand_z_end_to_target_safety_move;

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

			NextStepTimer( exe_hand_z_end_to_target_safety_move_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_z_end_to_target_safety_move_check:
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
				m_nStep = exe_backlash_loop_entry;
				m_nChkDistanceCnt++;
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
			m_tcTimeout.SetCheckTime(TIME_OUT_MIDDLE);
			m_tcTimeout.StartTimer();
			m_nStep = exe_final_x_safe_move_check;
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
						CBaseSeqCmd::ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + pTaskTestPP->m_nTestPPIdx, AlarmData);
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
			/* notify complete */

			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Test PP%d Axis %s Backlash complete", m_nTestPpIdx+1, (m_nAxisNum == CTaskTestPP::eAxisX1 ) ? ("X") : ("Y"));

			if(m_bBacklashTest)
			{
				int nLength = m_vBacklashAdjust.size();
				for(int i=0; i<nLength; i++)
				{
					double dOffset = 0.0;
					CString strBacklashOfs = _T("");
					strBacklashOfs = GetBacklashAxisInfo(m_nAxisNum, i, ( m_nAxisNum == eAxis_X) ? ( m_vBacklashAdjust[i].x ) : ( m_vBacklashAdjust[i].y ));
					AlarmData.m_fnAddItemFormat("%s", strBacklashOfs);
				}

			}
			m_pParentWnd->ReportBacklashDone();
			g_Error.AlarmReport(ERR_VISION_AUTOTEACHING_DONE, AlarmData, &g_TaskSystemCtrl);
			ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
		}break;
	}
	return CBaseSeqCmd::State_Executing();
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_VatBacklash::State_Pause()
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
BOOL CTestPPCmd_VatBacklash::State_Recovery()
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
BOOL CTestPPCmd_VatBacklash::State_NormalComplete()
{
	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_VatBacklash::State_Alarm()
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
BOOL CTestPPCmd_VatBacklash::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete(); 
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_VatBacklash::State_Aborting()
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
BOOL CTestPPCmd_VatBacklash::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete(); 
}


//==============================================================================
// local implementation
//==============================================================================
void CTestPPCmd_VatBacklash::NextStep( execute_step_define next )
{
	m_nStep = next;
}

void CTestPPCmd_VatBacklash::NextStepTimer( execute_step_define next , double tm )
{
	m_tcTimeout.SetCheckTime( tm );
	m_tcTimeout.StartTimer();
	NextStep( next );
}

void CTestPPCmd_VatBacklash::SimpleAlarm( int alarm_id )
{
	CParamList AlarmData;
	ReportAlarm( alarm_id, AlarmData );
}





BOOL CTestPPCmd_VatBacklash::CheckMeasureResult( ST_PCD_BACKLASH_RCV_DATA& result )
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
		result.dX = ((rand()%rand_range)-rand_range);
		result.dY = ((rand()%rand_range)-rand_range);
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

void CTestPPCmd_VatBacklash::WriteRepeatOffsetXY(std::vector<CPointF> End2TargetOfs, std::vector<CPointF> Start2TargetOfs, int spd)
{
	if(m_bRepeat == FALSE)
	{
		return;
	}

	double dOffset = 0.0;

	CString strFilePath;
	CString strFileName;
	CString strAxis = _T("X");
	if (m_nAxisNum == eAxis_Y)
		strAxis = _T("Y");

	strFileName.Format(_T("%04d%02d%02d_TEST_PP%d_BacklashData%s_%d.csv"), m_cTime.GetYear(),
		m_cTime.GetMonth(), m_cTime.GetDay(), m_nTestPpIdx+1, (LPCTSTR)strAxis, spd);

	strFilePath = BACKLASH_LOG_PATH + strFileName;

	CFile BacklashFile;
	BacklashFile.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);

	//by kmk [20190222] 기존 소스는 End - Start로 Backlash를 구함(Vision 프로그램)
	//by kmk [20190222] 추후 Start - End로 Backlash 계산을 통일할 예정

	int nLength = End2TargetOfs.size();
	for(int i=0; i<nLength; i++)
	{

		char szData2[256] = { 0, };
		if(m_nAxisNum == eAxis_X)
		{
			dOffset = (End2TargetOfs[i].x - Start2TargetOfs[i].x);
			/*dOffset = (Start2TargetOfs[i].x - End2TargetOfs[i].x);*/
			sprintf_s(szData2, sizeof(szData2), "%.5f,%.5f,%.5f\r\n",Start2TargetOfs[i].x, End2TargetOfs[i].x, dOffset);
		}
		else
		{
			/*dOffset = (Start2TargetOfs[i].y - End2TargetOfs[i].y);*/
			dOffset = (End2TargetOfs[i].y - Start2TargetOfs[i].y);
			sprintf_s(szData2, sizeof(szData2), "%.5f,%.5f,%.5f\r\n",Start2TargetOfs[i].y, End2TargetOfs[i].y, dOffset);
		}
		BacklashFile.SeekToEnd();
		BacklashFile.Write(szData2, strlen(szData2));

	}
	BacklashFile.Close();
}

void CTestPPCmd_VatBacklash::CalcBacklashOffset(int nSpeed, BOOL bIsBacklashTest,int nHandType)
{
	CPointF Temp;

	if (bIsBacklashTest == FALSE)
	{
		int nIdx = 0;

		if (nSpeed > 5)
		{
			nIdx = nSpeed / 10;
		}

		if (m_nAxisNum == eAxis_X)
		{
			//by kmk [20190222] 기존 소스는 End - Start로 Backlash를 구함(Vision 프로그램)
			//by kmk [20190222] 추후 Start - End로 Backlash 계산을 통일할 예정
			for (int i = 0; i < (int)m_vStartToTargetOffset.size(); i++)
			{
				Temp.x += (m_vEndToTargetOffset[i].x - m_vStartToTargetOffset[i].x);
				/*Temp.y += (m_vEndToTargetOffset[i].y - m_vStartToTargetOffset[i].y);*/
			}

			Temp.x = Temp.x / (int)m_vStartToTargetOffset.size();
			/*Temp.y = Temp.y / (int)m_vStartToTargetOffset.size();*/

			g_VatBacklash.testpp[m_nTestPpIdx][nHandType].m_BacklashOffset_XY[nIdx].x = Temp.x;
			m_BacklashXTestLog.LOG(" SPD -> %3d / OFFSET -> %.5f", nSpeed, g_VatBacklash.testpp[m_nTestPpIdx][nHandType].m_BacklashOffset_XY[nIdx].x);
		}
		else // m_nAxisNum == eAxis_Y
		{
			for (int i = 0; i < (int)m_vStartToTargetOffset.size(); i++)
			{
				/*Temp.x = (m_vEndToTargetOffset[i].x - m_vStartToTargetOffset[i].x);*/
				Temp.y = (m_vEndToTargetOffset[i].y - m_vStartToTargetOffset[i].y);

				g_VatBacklash.testpp[m_nTestPpIdx][nHandType].m_BacklashOffset_XY[nIdx].y = Temp.y;
				m_BacklashXTestLog.LOG(" Distance -> %3d / SPD -> %3d / OFFSET -> %.5f", (i + 1) * 10, nSpeed, g_VatBacklash.testpp[m_nTestPpIdx][nHandType].m_BacklashOffset_XY[nIdx].y);
			}
		}
	}
	else
	{
		m_vBacklashAdjust.push_back(Temp);
	}
}

CString CTestPPCmd_VatBacklash::GetBacklashAxisInfo(int nAxis, int nSpdIndex, double dOffset)
{
	CString str = _T("");
	int nSpd = 0;
	if(nSpdIndex == 0)
	{
		nSpd = 5;
	}
	else
	{
		nSpd = nSpdIndex * 10;
	}
	str.Format(_T("Axis : %s, Speed : %d, Offset : %.3fmm"), nAxis== eAxis_X? _T("X") : _T("Y"), nSpd, dOffset/1000.0);
	return str;
}

void CTestPPCmd_VatBacklash::InitLoopStatus()
{
	for(int i =eSpeed_5; i < eSpeed_MAX; i++ )
	{
		if(i == eSpeed_5)
		{
			m_qSpeed.push_back(5);
		}
		else
		{
			m_qSpeed.push_back(i*10);
		}
	}

	m_nRepeatTempCnt--;
	if(m_nRepeatTempCnt <= 0 )
	{
		m_nAxisNum = m_nAxisNum == eAxis_X ? eAxis_Y : eAxis_X;
		m_nRepeatTempCnt = m_nRepeatCnt;
	}
}