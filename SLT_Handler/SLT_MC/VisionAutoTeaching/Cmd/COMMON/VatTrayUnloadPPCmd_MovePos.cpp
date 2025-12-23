#include "stdafx.h"
#include "VatTrayUnloadPPCmd_MovePos.h"


CVatTrayUnloadPPCmd_MovePos::CVatTrayUnloadPPCmd_MovePos(int nMode) : m_nMode(nMode)
{
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));

	m_strCmdStage = _T("");

	for (int nAxis = 0; nAxis < VAT_MAX_AXIS; nAxis++) {
		m_dCmdPos[nAxis] = -1;
	}

	m_bZOnlyMove = FALSE; //Z축만 이동.

 	m_vCmdPicker.clear();

	m_cBasePocket = CPoint(0, 0);

	m_vBasePicker.clear();
	for (int nPkrY = 0; nPkrY < MAX_TRAY_PP_CNT_Y; nPkrY++) {
		for (int nPkrX = 0; nPkrX < MAX_TRAY_PP_CNT_X; nPkrX++) {
			m_vBasePicker.push_back(CPoint(nPkrX, nPkrY));
		}
	}

	m_vErrPicker.clear();


}


CVatTrayUnloadPPCmd_MovePos::~CVatTrayUnloadPPCmd_MovePos()
{
}


int CVatTrayUnloadPPCmd_MovePos::CheckParam()
{
	m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
	m_tcTimeout.StartTimer();

	//Transfer Safety 상태 Check용
	m_TransferInterlock.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
	m_TransferInterlock.SetTimerCheckEnd();

	return ERR_NO_ERROR;
}


int CVatTrayUnloadPPCmd_MovePos::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}


int CVatTrayUnloadPPCmd_MovePos::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}


int CVatTrayUnloadPPCmd_MovePos::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}


int CVatTrayUnloadPPCmd_MovePos::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}


int CVatTrayUnloadPPCmd_MovePos::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}


BOOL CVatTrayUnloadPPCmd_MovePos::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}


BOOL CVatTrayUnloadPPCmd_MovePos::State_Executing()
{
	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}

	if (m_nStep_Pre != m_nStep) {
		pTaskTrayUnloadPP->MakeVatLog(_T("[CMD: %s] [EXECUTE] [Step=%s]"), m_szCmdName, act_string(m_nStep));
	}

	m_nStep_Pre = m_nStep;

	for (int i = 0; i < CTaskTrayBasePP::eAxisMax; i++) {
		OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis(static_cast<CTaskTrayBasePP::eMotor>(i));
		if (pMotor != nullptr) {
			if (pMotor->ChkMotorUsable() != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Tray Unload PP check motor usable command error");
				ReportAlarm(ERR_TRAY_PP_2_MOTOR_ALARMED, AlarmData);
				return FALSE;
			}
		}
	}

	double dWorkEnableMin_um = INTERLOCK_UD_TRAYPP_Y_WORK_ENABLE_POS_TRANSFER*(-1);
	if (m_dCmdPos[VAT_Y] < dWorkEnableMin_um) {
		if (g_TaskTransfer.is_safety_state() == false) {

			if (m_TransferInterlock.IsChecking() == FALSE) {
				m_TransferInterlock.StartTimer();
			}

			if (m_TransferInterlock.CheckOverTime() == TRUE) {
				m_TransferInterlock.SetTimerCheckEnd();
				CParamList AlarmData;
				ReportAlarm(ERR_TRNASFER_IS_NOT_SAFETY_STATE, AlarmData);
			}
			return FALSE;
		}
		else {
			m_TransferInterlock.SetTimerCheckEnd();
		}
	}

	OneAxis* pMotorX = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisX);
	OneAxis* pMotorY = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisY);
	OneAxis* pMotorZ = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisZ);
	OneAxis* pMotorPX = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisPitchX);
	OneAxis* pMotorPY = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisPitchY);

	switch (m_nStep)
	{
		case move_start:
		{
			pTaskTrayUnloadPP->MakeVatLog("[%s EXEC=%8d] [MODE=%d] [%s move] [START]", m_szCmdName, m_nStep, m_nMode, m_strCmdStage);

			if (m_bZOnlyMove) { //Z축만 이동.
				m_nStep = z_move_target_pos;
			}
			else {
				m_nStep = z_move_safety;
			}

			//m_nStep = z_move_safety;
		}break;
		case z_move_safety:
		{
			pTaskTrayUnloadPP->MakeVatLog("[%s EXEC=%8d] [Move Z Safety] [SRT]", m_szCmdName, m_nStep);

			double dZSafetyPos = pTaskTrayUnloadPP->GetTeachPos(eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY, m_cBasePocket, m_vBasePicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
			int nErr = pTaskTrayUnloadPP->do_VATMovePos_Z(m_vBasePicker, dZSafetyPos);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				ReportAlarm(nErr, AlarmData);
				break;
			}

			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_SHORT);
			m_tcTimeout.StartTimer();
			m_nStep = z_move_safety_chk;
		}break;
		case z_move_safety_chk:
		{
			if (m_tcTimeout.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				ReportAlarm(ERR_TRAY_PP_2_MOTOR_MOVE_TIMEOUT, AlarmData, m_vErrPicker);
				break;
			}

			m_vErrPicker.clear();
			int nAxisDone = pMotorZ->AxisDone(m_vBasePicker, m_vErrPicker);
			int nErrSen = pTaskTrayUnloadPP->Chk_Z_Safety_Sen_All();
			if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR) {
				m_tcTimeout.ResetStartTime();
				m_nStep = x_y_px_py_move_target_pos;
				break;
			}
		}break;
		case x_y_px_py_move_target_pos:
		{
			//pTaskTrayUnloadPP->WriteDisplayLog("[EXE] Hand Target Pos Move");
			pTaskTrayUnloadPP->MakeVatLog("[%s EXEC=%8d] [x, y, Xpitch, Ypitch move %s] [SRT]", m_szCmdName, m_nStep, m_strCmdStage);
			double dCmdPos[4] = { m_dCmdPos[VAT_X], m_dCmdPos[VAT_Y], m_dCmdPos[VAT_PITCH_X], m_dCmdPos[VAT_PITCH_Y] };
			int nAxisNo[4] = { CTaskTrayBasePP::eAxisX, CTaskTrayBasePP::eAxisY, CTaskTrayBasePP::eAxisPitchX, CTaskTrayBasePP::eAxisPitchY };

			for (int i = 0; i < _countof(nAxisNo); i++) {
				int nErr = pTaskTrayUnloadPP->do_VATMovePos((CTaskTrayBasePP::eMotor)nAxisNo[i], dCmdPos[i]);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}

			m_tcTimeout.ResetStartTime();
			m_nStep = x_y_px_py_move_target_pos_chk;
		}break;
		case x_y_px_py_move_target_pos_chk:
		{
			int nAxisNo[4] = { CTaskTrayBasePP::eAxisX, CTaskTrayBasePP::eAxisY, CTaskTrayBasePP::eAxisPitchX, CTaskTrayBasePP::eAxisPitchY };

			if (m_tcTimeout.CheckOverTime() == TRUE) {
				CString strErrAxis = _T("");
				for (int i = 0; i < _countof(nAxisNo); i++) {
					OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
					int nAxisDone = pMotor->AxisDone();
					if (nAxisDone != ERR_NO_ERROR) {
						if (strErrAxis.Compare("") != 0) {
							strErrAxis += ", ";
						}
						strErrAxis += pMotor->GetAxisName();
					}
				}

				double dPosX = 0, dPosY = 0, dPosPX = 0, dPosPY = 0;
				pMotorX->GetCurrentPos(dPosX);
				pMotorY->GetCurrentPos(dPosY);
				pMotorPX->GetCurrentPos(dPosPX);
				pMotorPY->GetCurrentPos(dPosPY);

				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Tray PP axis operation is not complete");
				AlarmData.m_fnAddItemFormat("%s motor time over!", strErrAxis);
				AlarmData.m_fnAddItemFormat("[current position] x=%.2f, y=%.2f, x-pitch=%.2f, y-pitch=%.2f", dPosX / 1000.0, dPosY / 1000.0, dPosPX / 1000.0, dPosPY / 1000.0);
				ReportAlarm(ERR_TRAY_PP_2_MOTOR_MOVE_TIMEOUT, AlarmData);
				break;
			}

			int nChkComp = 0;
			for (int i = 0; i < _countof(nAxisNo); i++) {
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
				int nAxisDone = pMotor->AxisDone();
				if (nAxisDone == ERR_NO_ERROR) {
					nChkComp++;
				}
				else if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Tray PP axis operation is not complete");
					AlarmData.m_fnAddItemFormat("[ check axis done ] x=%d, y=%d, x-pitch=%d, y-pitch=%d", pMotorX->AxisDone(), pMotorPX->AxisDone(), pMotorPY->AxisDone());
					ReportAlarm(ERR_TRAY_PP_2_MOTOR_MOVE_FAIL, AlarmData);
					break;
				}
			}

			if (nChkComp == _countof(nAxisNo)) {
				pTaskTrayUnloadPP->MakeVatLog("[%s EXEC=%8d] [x, y, Xpitch, Ypitch move %s] [END]", m_szCmdName, m_nStep, m_strCmdStage);
				m_nStep = z_move_target_pos;
				break;
			}
		}break;
		case z_move_target_pos:
		{
			pTaskTrayUnloadPP->MakeVatLog("[%s EXEC=%8d] [Z %s move] [SRT]", m_szCmdName, m_nStep, m_strCmdStage);

			int nErr = pTaskTrayUnloadPP->do_VATMovePos_Z(m_vCmdPicker, m_dCmdPos[VAT_Z]);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Unload Tray PP Move Target Z Move Fail");
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
				ReportAlarm(ERR_TRAY_PP_2_MOTOR_MOVE_TIMEOUT, AlarmData, m_vErrPicker);
				break;
			}

			OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisZ);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				m_nStep = move_end;
				break;
			}
		}break;
		case move_end:
		{
			pTaskTrayUnloadPP->MakeVatLog("[%s EXEC=%8d] [MODE=%d] [%s move] [END]", m_szCmdName, m_nStep, m_nMode, m_strCmdStage);
			ReportCmdEnd(eSeqCmdStatus_NormalComplete);
		}break;
		default:
			break;
	}

	return CBaseSeqCmd::State_Executing();
}

//case 1000
BOOL CVatTrayUnloadPPCmd_MovePos::State_Pause()
{
	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTrayUnloadPP->MakeVatLog("[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 1000:
	{
		pTaskTrayUnloadPP->WriteDisplayLog("[PAUSE]==============================");

		int nAxisCount = pTaskTrayUnloadPP->GetMotorCount();
		for (int i = 0; i < nAxisCount; i++) {
			OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis(static_cast<CTaskTrayBasePP::eMotor>(i));
			if (pMotor == nullptr) {
				continue;
			}

			bool bCloseLimit = pMotor->CheckLimitClose();
			double dCurPos = 0.0;
			pMotor->GetCurrentPos(dCurPos);
			if (bCloseLimit == false)
			{
				pTaskTrayUnloadPP->MakeVatLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
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
BOOL CVatTrayUnloadPPCmd_MovePos::State_Recovery()
{
	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTrayUnloadPP->MakeVatLog("[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
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
BOOL CVatTrayUnloadPPCmd_MovePos::State_NormalComplete()
{
	return CBaseSeqCmd::State_NormalComplete();
}


//case 4000
BOOL CVatTrayUnloadPPCmd_MovePos::State_Alarm()
{
	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTrayUnloadPP->MakeVatLog("[CMD: %s] [ALARM PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 4000:
	{
		for (int i = 0; i < CTaskTrayLoadPP::eAxisMax; i++) {
			OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayLoadPP::eMotor)i);
			if (pMotor == nullptr) {
				continue;
			}

			double dCurPos = 0.0;
			pMotor->GetCurrentPos(dCurPos);

			bool bCloseLimit = pMotor->CheckLimitClose();
			if (bCloseLimit == false)
			{
				pTaskTrayUnloadPP->MakeVatLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
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
BOOL CVatTrayUnloadPPCmd_MovePos::State_AlarmComplete()
{
	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTrayUnloadPP->MakeVatLog("[CMD: %s] [ALARM COMP] [Step=%8d]", m_szCmdName, m_nStep);
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

//6000
BOOL CVatTrayUnloadPPCmd_MovePos::State_Aborting()
{
	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTrayUnloadPP->MakeVatLog("[CMD: %s] [ABORT] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 6000:
	{
		for (int i = 0; i < CTaskTrayLoadPP::eAxisMax; i++) {
			OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayLoadPP::eMotor)i);
			if (pMotor == nullptr) {
				continue;
			}

			double dCurPos = 0.0;
			pMotor->GetCurrentPos(dCurPos);

			bool bCloseLimit = pMotor->CheckLimitClose();
			if (bCloseLimit == false)
			{
				pTaskTrayUnloadPP->MakeVatLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
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


BOOL CVatTrayUnloadPPCmd_MovePos::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}


//==================================================================================
// COMMENT : Parameter 넣은 후, Cmd 생성 전 Check 필요.
//			
//==================================================================================
BOOL CVatTrayUnloadPPCmd_MovePos::IsParamExist()
{
	//Target 위치 및 CmdPicker Check.
// 	if (m_eCmdStage == LOC_IDX_TRAY_PP_Undefined || m_vCmdPicker.size() == 0) {
// 		return FALSE;
// 	}

	//Limit Check는 Position입력에서 진행 중.


	return TRUE;
}

CString CVatTrayUnloadPPCmd_MovePos::act_string(int act)
{
	BEGIN_CASE_TO_STR(act);
	CASE_TO_STR(move_start);
	CASE_TO_STR(z_move_safety);
	CASE_TO_STR(z_move_safety_chk);
	CASE_TO_STR(x_y_px_py_move_target_pos);
	CASE_TO_STR(x_y_px_py_move_target_pos_chk);
	CASE_TO_STR(z_move_target_pos);
	CASE_TO_STR(z_move_target_pos_chk);
	CASE_TO_STR(move_end);
	ELSE_CASE(_T("unkown_step"));
	END_CASE_TO_STR();
}