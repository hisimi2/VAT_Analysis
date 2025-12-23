#include "stdafx.h"
#include "VatTestPPCmd_MovePos.h"


CVatTestPPCmd_MovePos::CVatTestPPCmd_MovePos(int nMode) : m_nMode(nMode)
{
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));

	m_nTestPPIdx = -1;
	m_nTestPPType = VAT_TESTPP_LOAD;

	for (int nAxis = 0; nAxis < VAT_MAX_AXIS; nAxis++) {
		m_dCmdPos[nAxis] = -1;
	}

	m_vCmdPicker.clear();

	m_bZOnlyMove = FALSE;

	m_strCmdStage = _T("");

	m_cBasePocket = CPoint(0, 0);

	m_vBasePicker.clear();
	for (int nPkrY = 0; nPkrY < MAX_TRAY_PP_CNT_Y; nPkrY++) {
		for (int nPkrX = 0; nPkrX < MAX_TRAY_PP_CNT_X; nPkrX++) {
			m_vBasePicker.push_back(CPoint(nPkrX, nPkrY));
		}
	}

	m_vErrPicker.clear();


}


CVatTestPPCmd_MovePos::~CVatTestPPCmd_MovePos()
{
}



int CVatTestPPCmd_MovePos::CheckParam()
{
	CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
	m_nTestPPIdx = pTaskTestPP->m_nTestPPIdx;

	m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
	m_tcTimeout.StartTimer();

	return ERR_NO_ERROR;
}


int CVatTestPPCmd_MovePos::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}


int CVatTestPPCmd_MovePos::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}


int CVatTestPPCmd_MovePos::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}


int CVatTestPPCmd_MovePos::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}


int CVatTestPPCmd_MovePos::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}


BOOL CVatTestPPCmd_MovePos::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}


BOOL CVatTestPPCmd_MovePos::State_Executing()
{
	CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}

	if (m_nStep_Pre != m_nStep) {
		pTaskTestPP->MakeVatLog(_T("[CMD: %s] [EXECUTE] [Step=%s]"), m_szCmdName, act_string(m_nStep));
	}

	m_nStep_Pre = m_nStep;

	for (int i = 0; i < CTaskTestPP::eAxisMax; i++) {
		OneAxis* pMotor = pTaskTestPP->GetAxis(static_cast<CTaskTestPP::eMotor>(i));
		if (pMotor != nullptr) {
			if (pMotor->ChkMotorUsable() != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d check motor usable command error", m_nTestPPIdx + 1);
				ReportAlarm(ERR_TEST_PP_1_MOTOR_ALARMED + m_nTestPPIdx, AlarmData);
				return FALSE;
			}
		}
	}

	switch (m_nStep)
	{
		case move_start:
		{
			pTaskTestPP->MakeVatLog("[%s EXEC=%8d] [MODE=%d] [%s move] [START]", m_szCmdName, m_nStep, m_nMode, m_strCmdStage);

			if (m_bZOnlyMove) { //Z축만 이동.
				m_nStep = z_move_target_pos;
			}
			else {
				m_nStep = z_move_safety;
			}	
		}break;
		case z_move_safety:
		{
			pTaskTestPP->MakeVatLog("[%s EXEC=%8d] [Move Z Safety] [SRT]", m_szCmdName, m_nStep);

			int nAxisNo[2] = { CTaskTestPP::eAxisZ1 ,CTaskTestPP::eAxisZ2 };
			for (int i = 0; i < _countof(nAxisNo); i++)
			{
				double dZSafetyPos = pTaskTestPP->GetTeachPos(eLocIdxTestPP::LOC_IDX_TEST_PP_HOME, m_cBasePocket, m_vBasePicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i], m_nTestPPType);
				int nErr = pTaskTestPP->do_VATMovePos_Z(nAxisNo[i], m_vBasePicker, dZSafetyPos);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d all picker up command, but this was wrong ", m_nTestPPIdx + 1);
					ReportAlarm(ERR_TEST_PP_1_Z1_PICKER_CMD_INTERLOCK + pTaskTestPP->GetHandTypeIndex(m_nTestPPType), AlarmData);
					break;
				}
			}

			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_SHORT);
			m_tcTimeout.StartTimer();
			m_nStep = z_move_safety_chk;
		}break;
		case z_move_safety_chk:
		{
			int nAxisNo[2] = { CTaskTestPP::eAxisZ1 ,CTaskTestPP::eAxisZ2 };

			if (m_tcTimeout.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPPIdx, AlarmData, m_vErrPicker);
				break;
			}

			int nComp = 0;
			for (int i = 0; i < _countof(nAxisNo); i++)
			{
				OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo[i]);
				m_vErrPicker.clear();
				int nAxisDone = pMotor->AxisDone(m_vBasePicker, m_vErrPicker);
				int nErrSen = pTaskTestPP->Chk_Z_Safety_Sen_All(VAT_TESTPP_LOAD + i);
				if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR)
				{
					nComp++;
				}
			}

			if (nComp == _countof(nAxisNo))
			{
				m_tcTimeout.ResetStartTime();

				m_nStep = x1_x2_move_safety;

				break;
			}
		}break;
		case x1_x2_move_safety:
		{
// 			int nSafetyX1Sen = g_TaskTestPP[m_nTestPPIdx].ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON);
// 			int nSafetyX2Sen = g_TaskTestPP[m_nTestPPIdx].ChkDI(CTaskTestPP::xX2AXIS_POSITION_SEN, DEF_ON);
// 			if (nSafetyX1Sen == ERR_NO_ERROR && nSafetyX2Sen == ERR_NO_ERROR) {
// 				m_nStep = y_px_py_move_target_pos;
// 				break;
// 			}

			pTaskTestPP->MakeVatLog("[%s EXEC=%8d] [X1, X2 Axis Move to Safety Pos] [SRT]", m_szCmdName, m_nStep);

			int nAxisNo[2] = { CTaskTestPP::eAxisX1 ,CTaskTestPP::eAxisX2 };
			//double dCmdPos[2] = { m_dCmdPos[VAT_HAND_AXIS_X] };
			
			for (int i = 0; i < _countof(nAxisNo); i++)
			{
				double dXSafetyPos = pTaskTestPP->GetTeachPos(eLocIdxTestPP::LOC_IDX_TEST_PP_HOME, m_cBasePocket, m_vBasePicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i], m_nTestPPType);
				int nErr = pTaskTestPP->do_VATMovePos(nAxisNo[i], dXSafetyPos);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d x1, x2 axis move command,but this was wrong", m_nTestPPIdx + 1);
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}

			m_tcTimeout.ResetStartTime();
			m_nStep = x1_x2_move_safety_chk;
		}break;
		case x1_x2_move_safety_chk:
		{
			int nAxisNo[2] = { CTaskTestPP::eAxisX1 ,CTaskTestPP::eAxisX2 };

			if (m_tcTimeout.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP%d, X1, X2 Axis move time over [CmdStage=%s]", m_nTestPPIdx + 1, m_strCmdStage);
				ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPPIdx, AlarmData);
				break;
			}

			int nComp = 0;
			for (int i = 0; i < _countof(nAxisNo); i++)
			{
				OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo[i]);
				int nAxisDone = pMotor->AxisDone();
				if (nAxisDone == ERR_NO_ERROR) {
					nComp++;
				}
				else if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP%d X1, X2 axis operation is not complete", m_nTestPPIdx + 1);
					ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPPIdx, AlarmData);
					break;
				}
			}

			if (nComp == _countof(nAxisNo))
			{
				pTaskTestPP->MakeVatLog("[%s EXEC=%8d] [X1, X2 Axis Move to Safety Pos] [END]", m_szCmdName, m_nStep);
				m_nStep = y_px_py_move_target_pos;
			}
		}break;
		case y_px_py_move_target_pos:
		{
			pTaskTestPP->MakeVatLog("[%s EXEC=%8d] [Y, X%dPitch, Y%dPitch Axis Move to Target Pos] [CmdStage=%s] [SRT]", m_szCmdName, m_nStep, m_nTestPPType + 1, m_nTestPPType + 1, m_strCmdStage);

			double dCmdPos[3] = { m_dCmdPos[VAT_Y], m_dCmdPos[VAT_PITCH_X], m_dCmdPos[VAT_PITCH_Y] };
			int nAxisNo[3] = { CTaskTestPP::eAxisY ,
				(m_nTestPPType == VAT_TESTPP_LOAD) ? CTaskTestPP::eAxisPitch_HW_X1_SW_Y1 : CTaskTestPP::eAxisPitch_HW_X2_SW_Y2,
				(m_nTestPPType == VAT_TESTPP_LOAD) ? CTaskTestPP::eAxisPitch_HW_Y1_SW_X1 : CTaskTestPP::eAxisPitch_HW_Y2_SW_X2 };


			for (int i = 0; i < _countof(nAxisNo); i++)
			{
				int nErr = pTaskTestPP->do_VATMovePos(nAxisNo[i], dCmdPos[i]);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}
			 
			m_tcTimeout.ResetStartTime();
			m_nStep = y_px_py_move_target_pos_chk;
		}break;
		case y_px_py_move_target_pos_chk:
		{
			int nAxisNo[3] = { CTaskTestPP::eAxisY ,
				(m_nTestPPType == VAT_TESTPP_LOAD) ? CTaskTestPP::eAxisPitch_HW_X1_SW_Y1 : CTaskTestPP::eAxisPitch_HW_X2_SW_Y2,
				(m_nTestPPType == VAT_TESTPP_LOAD) ? CTaskTestPP::eAxisPitch_HW_Y1_SW_X1 : CTaskTestPP::eAxisPitch_HW_Y2_SW_X2 };

			if (m_tcTimeout.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP%d, Y, X%dPitch, Y%dPitch Axis move time over [CmdStage=%s]", m_nTestPPIdx + 1, m_nTestPPType + 1, m_nTestPPType + 1, m_strCmdStage);
				ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPPIdx, AlarmData);
				break;
			}

			int nComp = 0;
			for (int i = 0; i < _countof(nAxisNo); i++)
			{
				OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo[i]);
				int nAxisDone = pMotor->AxisDone();
				if (nAxisDone == ERR_NO_ERROR) {
					nComp++;
				}
				else if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP%d, Y, X%dPitch, Y%dPitch axis operation is not complete", m_nTestPPIdx + 1, m_nTestPPType + 1, m_nTestPPType + 1);
					ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPPIdx, AlarmData);
					break;
				}
			}

			if (nComp == _countof(nAxisNo))
			{
				pTaskTestPP->MakeVatLog("[%s EXEC=%8d] [Y, X%dPitch, Y%dPitch Axis Move to Target Pos] [CmdStage=%s] [END]", m_szCmdName, m_nStep, m_nTestPPType + 1, m_nTestPPType + 1, m_strCmdStage);
				m_nStep = x_move_target_pos;
			}
		}break;
		case x_move_target_pos:
		{
			pTaskTestPP->MakeVatLog("[%s EXEC=%8d] [X Axis Move to Target Pos] [SRT]", m_szCmdName, m_nStep);

			int nAxisNo = (m_nTestPPType == VAT_TESTPP_LOAD) ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2;

			int nErr = pTaskTestPP->do_VATMovePos(nAxisNo, m_dCmdPos[VAT_X]);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP%d x axis move command,but this was wrong", m_nTestPPIdx + 1);
				ReportAlarm(nErr, AlarmData);
				break;
			}

			m_tcTimeout.ResetStartTime();
			m_nStep = x_move_target_pos_chk;
		}break;
		case x_move_target_pos_chk:
		{
			int nAxisNo = (m_nTestPPType == VAT_TESTPP_LOAD) ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2;

			if (m_tcTimeout.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP%d, X Axis move time over [CmdStage=%s]", m_nTestPPIdx + 1, m_strCmdStage);
				ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPPIdx, AlarmData);
				break;
			}

			OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				pTaskTestPP->MakeVatLog("[%s EXEC=%8d] [X Axis Move to Target Pos] [END]", m_szCmdName, m_nStep);
				m_nStep = z_move_target_pos;
			}
			else if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP%d, X axis operation is not complete", m_nTestPPIdx + 1, m_nTestPPType + 1, m_nTestPPType + 1);
				ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPPIdx, AlarmData);
				break;
			}
		}break;
		case z_move_target_pos:
		{
			pTaskTestPP->MakeVatLog("[%s EXEC=%8d] [Z %s move] [SRT]", m_szCmdName, m_nStep, m_strCmdStage);


			int nAxisNo = (m_nTestPPType == VAT_TESTPP_LOAD) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
			int nErr = pTaskTestPP->do_VATMovePos_Z(nAxisNo, m_vCmdPicker, m_dCmdPos[VAT_Z]);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP%d Move Target Z Move Fail", m_nTestPPIdx + 1);
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime(TIME_OUT_SHORT);
			m_tcTimeout.ResetStartTime();
			m_nStep = z_move_target_pos_chk;
		}break;
		case z_move_target_pos_chk:
		{
			if (m_tcTimeout.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP%d, Z%d Axis move time over [CmdStage=%s]", m_nTestPPIdx + 1, m_nTestPPType + 1, m_strCmdStage);
				ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPPIdx, AlarmData);
				break;
			}
			int nAxisNo = (m_nTestPPType == VAT_TESTPP_LOAD) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
			OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				m_nStep = move_end;
				break;
			}
		}break;
		case move_end:
		{
			pTaskTestPP->MakeVatLog("[%s EXEC=%8d] [MODE=%d] [%s move] [END]", m_szCmdName, m_nStep, m_nMode, m_strCmdStage);
			ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
		}break;
		default:
			break;
	}

	return CBaseSeqCmd::State_Executing();
}


//case 1000
BOOL CVatTestPPCmd_MovePos::State_Pause()
{
	CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTestPP->MakeVatLog("[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 1000:
	{
		//pTaskTestPP->WriteDisplayLog("[PAUSE]==============================");

		int nAxisCount = pTaskTestPP->m_vMotor.size();
		for (int i = 0; i < nAxisCount; i++) {
			OneAxis* pMotor = pTaskTestPP->GetAxis(i);
			if (pMotor == nullptr) {
				continue;
			}

			bool bCloseLimit = pMotor->CheckLimitClose();
			double dCurPos = 0.0;
			pMotor->GetCurrentPos(dCurPos);
			if (bCloseLimit == false)
			{
				pTaskTestPP->MakeVatLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
				pMotor->SetSlowStop();
			}
		}
		m_nStep++;

	}break;
	case 1001:
	{

	}break;
	default:
	{
		m_nStep = 1000;
	}break;
	}

	return CBaseSeqCmd::State_Pause();
}


//case 2000
BOOL CVatTestPPCmd_MovePos::State_Recovery()
{
	CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTestPP->MakeVatLog("[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 2000:
	{
		SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing);
	}break;
	default:
	{
		m_nStep = 2000;
	}break;
	}

	return CBaseSeqCmd::State_Recovery();
}


//case 3000
BOOL CVatTestPPCmd_MovePos::State_NormalComplete()
{
	return CBaseSeqCmd::State_NormalComplete();
}


//case 4000
BOOL CVatTestPPCmd_MovePos::State_Alarm()
{
	CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTestPP->MakeVatLog("[CMD: %s] [ALARM PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 4000:
	{
		int nAxisCount = pTaskTestPP->m_vMotor.size();
		for (int i = 0; i < nAxisCount; i++) {
			OneAxis* pMotor = pTaskTestPP->GetAxis(i);
			if (pMotor == nullptr) {
				continue;
			}

			bool bCloseLimit = pMotor->CheckLimitClose();
			double dCurPos = 0.0;
			pMotor->GetCurrentPos(dCurPos);
			if (bCloseLimit == false)
			{
				pTaskTestPP->MakeVatLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
				pMotor->SetSlowStop();
			}
		}
		m_nStep++;
	}break;
	case 4001: //Retry, Skip을 기다림.
	{

	}break;
	default:
	{
		m_nStep = 4000;
	}break;
	}

	return CBaseSeqCmd::State_Alarm();
}


//case 5000
BOOL CVatTestPPCmd_MovePos::State_AlarmComplete()
{
	CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTestPP->MakeVatLog("[CMD: %s] [ALARM COMP] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 5000:
	{
		return CBaseSeqCmd::State_AlarmComplete();
	}break;
	default: {
		m_nStep = 5000;
	}break;
	}

	return CBaseSeqCmd::State_AlarmComplete();
}


//case 6000
BOOL CVatTestPPCmd_MovePos::State_Aborting()
{
	CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTestPP->MakeVatLog("[CMD: %s] [ABORT] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 6000:
	{
		for (int i = 0; i < CTaskTestPP::eAxisMax; i++) {
			OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)i);
			if (pMotor == nullptr) {
				continue;
			}

			double dCurPos = 0.0;
			pMotor->GetCurrentPos(dCurPos);

			bool bCloseLimit = pMotor->CheckLimitClose();
			if (bCloseLimit == false)
			{
				pTaskTestPP->MakeVatLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
				pMotor->SetSlowStop();
			}
		}
		m_nStep++;
	}break;
	case 6001:
	{
		ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AbortComplete); // 명령 진행 중 STOP 명령을 받아 명령이 중단 됨.
	}break;
	default:
	{
		m_nStep = 6000;
	}break;
	}

	return CBaseSeqCmd::State_Aborting();
}


//case 7000
BOOL CVatTestPPCmd_MovePos::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}


//==================================================================================
// COMMENT : Parameter 넣은 후, Cmd 생성 전 Check 필요.
//			
//==================================================================================
BOOL CVatTestPPCmd_MovePos::IsParamExist()
{
	//Target 위치 및 CmdPicker Check.
// 	if (m_eCmdStage == LOC_IDX_TEST_PP_Undefined || m_vCmdPicker.size() == 0) {
// 		return FALSE;
// 	}

	//Limit Check는 Position입력에서 진행 중.


	return TRUE;
}


CString CVatTestPPCmd_MovePos::act_string(int act)
{
	BEGIN_CASE_TO_STR(act);
	CASE_TO_STR(move_start);
	CASE_TO_STR(z_move_safety);
	CASE_TO_STR(z_move_safety_chk);
	CASE_TO_STR(x1_x2_move_safety);
	CASE_TO_STR(x1_x2_move_safety_chk);
	CASE_TO_STR(y_px_py_move_target_pos);
	CASE_TO_STR(y_px_py_move_target_pos_chk);
	CASE_TO_STR(x_move_target_pos);
	CASE_TO_STR(x_move_target_pos_chk);
	CASE_TO_STR(z_move_target_pos);
	CASE_TO_STR(z_move_target_pos_chk);
	CASE_TO_STR(move_end);
	ELSE_CASE(_T("unkown_step"));
	END_CASE_TO_STR();
}