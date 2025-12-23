#include "stdafx.h"
#include "VatTrayLoadPPCmd_MovePos.h"


CVatTrayLoadPPCmd_MovePos::CVatTrayLoadPPCmd_MovePos(int nMode) : m_nMode(nMode)
{
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));

	m_strCmdStage = _T("");

	for (int nAxis = 0; nAxis < VAT_MAX_AXIS; nAxis++) {
		m_dCmdPos[nAxis] = -1;
	}

	m_bZOnlyMove = FALSE; // Z축만 이동.

	m_vCmdPicker.clear();

	m_cBasePocket = CPoint(0, 0);

	m_vBasePicker.clear();
	for (int nPkrY = 0; nPkrY < MAX_TRAY_PP_CNT_Y; nPkrY++) {
		for (int nPkrX = 0; nPkrX < MAX_TRAY_PP_CNT_X; nPkrX++) {
			m_vBasePicker.push_back(CPoint(nPkrX, nPkrY));
		}
	}

	m_vErrPicker.clear();



// 	for (int i = 0; i < eAxisPos_Max; i++) {
// 		m_dTargetPos[i] = 0.0;
// 	}
}


CVatTrayLoadPPCmd_MovePos::~CVatTrayLoadPPCmd_MovePos()
{
}


int CVatTrayLoadPPCmd_MovePos::CheckParam()
{
	m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
	m_tcTimeout.StartTimer();

	//m_dZSafetyPos = g_TaskTrayLoadPP.GetTeachPos(eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY, m_cBasePocket, m_vBasePicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
		
	return ERR_NO_ERROR;
}


int  CVatTrayLoadPPCmd_MovePos::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}


int  CVatTrayLoadPPCmd_MovePos::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}


int  CVatTrayLoadPPCmd_MovePos::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}


int  CVatTrayLoadPPCmd_MovePos::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}


int  CVatTrayLoadPPCmd_MovePos::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

BOOL CVatTrayLoadPPCmd_MovePos::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}


BOOL CVatTrayLoadPPCmd_MovePos::State_Executing()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = (CTaskTrayLoadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTrayLoadPP->MakeVatLog(_T("[CMD: %s] [EXECUTE] [Step=%s]"), m_szCmdName, act_string(m_nStep));
	}
	m_nStep_Pre = m_nStep;

	for (int i = 0; i < CTaskTrayBasePP::eAxisMax; i++) {
		OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)i);
		if (pMotor == nullptr)
			continue;

		if (pMotor->ChkMotorUsable() != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray PP check motor usable Cmd Error");
			ReportAlarm(ERR_TRAY_PP_1_MOTOR_ALARMED, AlarmData);
			return FALSE;
		}
	}

	OneAxis* pMotorX = pTaskTrayLoadPP->GetAxis(CTaskTrayBasePP::eAxisX);
	OneAxis* pMotorZ = pTaskTrayLoadPP->GetAxis(CTaskTrayBasePP::eAxisZ);
	OneAxis* pMotorPX = pTaskTrayLoadPP->GetAxis(CTaskTrayBasePP::eAxisPitchX);
	OneAxis* pMotorPY = pTaskTrayLoadPP->GetAxis(CTaskTrayBasePP::eAxisPitchY);
	
	switch (m_nStep)
	{
		case move_start:
		{
			pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [MODE=%d] [%s move] [START]", m_szCmdName, m_nStep, m_nMode, m_strCmdStage);

			if (m_bZOnlyMove) { //Z축만 이동.
				m_nStep = z_move_target_pos;
			}
			else {
				m_nStep = z_move_safety;
			}		
		}break;
		case z_move_safety:
		{
			pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [Move Z Safety] [SRT]", m_szCmdName, m_nStep);

			//int nErr = pTaskTrayLoadPP->do_MovePos(eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY, m_cBasePocket, m_vBasePicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
			double dZSafetyPos = g_TaskTrayLoadPP.GetTeachPos(eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY, m_cBasePocket, m_vBasePicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
			int nErr = pTaskTrayLoadPP->do_VATMovePos_Z(m_vBasePicker, dZSafetyPos);
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
				ReportAlarm(ERR_TRAY_PP_1_MOTOR_MOVE_TIMEOUT, AlarmData, m_vErrPicker);
				break;
			}

			m_vErrPicker.clear();
			int nAxisDone = pMotorZ->AxisDone(m_vBasePicker, m_vErrPicker);
			int nErrSen = pTaskTrayLoadPP->Chk_Z_Safety_Sen_All();
			if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR) {
				pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [Move Z Safety] [END]", m_szCmdName, m_nStep);
				m_tcTimeout.ResetStartTime();
				m_nStep = x_px_py_move_target_pos;
				break;
			}
		}break;
		case x_px_py_move_target_pos:
		{
			pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [%s] [x, Xpitch, Ypitch move] [STR]", m_szCmdName, m_nStep, m_strCmdStage);

			//double dTPos[2] = {m_dTargetPosX, }
			double dCmdPos[3] = { m_dCmdPos[VAT_X],  m_dCmdPos[VAT_PITCH_X],  m_dCmdPos[VAT_PITCH_Y] };
			int nAxisNo[3] = { CTaskTrayBasePP::eAxisX, CTaskTrayBasePP::eAxisPitchX, CTaskTrayBasePP::eAxisPitchY };

			for (int i = 0; i < _countof(nAxisNo); i++) {
				int nErr = pTaskTrayLoadPP->do_VATMovePos((CTaskTrayBasePP::eMotor)nAxisNo[i], dCmdPos[i]);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}
			
			m_tcTimeout.ResetStartTime();
			m_nStep = x_px_py_move_target_pos_chk;
		}break;
		case x_px_py_move_target_pos_chk:
		{
			int nAxisNo[3] = { CTaskTrayBasePP::eAxisX, CTaskTrayBasePP::eAxisPitchX, CTaskTrayBasePP::eAxisPitchY };

			if (m_tcTimeout.CheckOverTime() == TRUE) {
				CString strErrAxis = _T("");
				for (int i = 0; i < _countof(nAxisNo); i++) {
					OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
					int nAxisDone = pMotor->AxisDone();
					if (nAxisDone != ERR_NO_ERROR) {
						if (strErrAxis.Compare("") != 0) {
							strErrAxis += ", ";
						}
						strErrAxis += pMotor->GetAxisName();
					}
				}

				double dPosX = 0, dPosPX = 0, dPosPY = 0;
				pMotorX->GetCurrentPos(dPosX);
				pMotorPX->GetCurrentPos(dPosPX);
				pMotorPY->GetCurrentPos(dPosPY);

				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Tray PP axis operation is not complete");
				AlarmData.m_fnAddItemFormat("%s motor time over!", strErrAxis);
				AlarmData.m_fnAddItemFormat("[current position] x=%.2f, x-pitch=%.2f, y-pitch=%.2f", dPosX / 1000.0, dPosPX / 1000.0, dPosPY / 1000.0);
				ReportAlarm(ERR_TRAY_PP_1_MOTOR_MOVE_TIMEOUT, AlarmData);
				break;
			}

			int nChkComp = 0;
			for (int i = 0; i < _countof(nAxisNo); i++) {
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
				int nAxisDone = pMotor->AxisDone();
				if (nAxisDone == ERR_NO_ERROR) {
					nChkComp++;
				}
				else if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Tray PP axis operation is not complete");
					AlarmData.m_fnAddItemFormat("[ check axis done ] x=%d, x-pitch=%d, y-pitch=%d", pMotorX->AxisDone(), pMotorPX->AxisDone(), pMotorPY->AxisDone());
					ReportAlarm(ERR_TRAY_PP_1_MOTOR_MOVE_FAIL, AlarmData);
					break;
				}
			}

			if (nChkComp == _countof(nAxisNo)) {
				pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [%s] [x, Xpitch, Ypitch move] [END]", m_szCmdName, m_nStep, m_strCmdStage);
				m_nStep = z_move_target_pos;
				break;
			}

		}break;
		case z_move_target_pos:
		{
			pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [%s] [Z move] [SRT]", m_szCmdName, m_nStep, m_strCmdStage);
			
			int nErr = pTaskTrayLoadPP->do_VATMovePos_Z(m_vCmdPicker, m_dCmdPos[VAT_Z]);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Load Tray PP Move Target Z Move Fail");
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
				ReportAlarm(ERR_TRAY_PP_1_MOTOR_MOVE_TIMEOUT, AlarmData, m_vErrPicker);
				break;
			}

			OneAxis* pMotor = pTaskTrayLoadPP->GetAxis(CTaskTrayBasePP::eAxisZ);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				m_nStep = move_end;
				break;
			}
		}break;
		case move_end:
		{
			pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [MODE=%d] [%s move] [END]", m_szCmdName, m_nStep, m_nMode, m_strCmdStage);
			ReportCmdEnd(eSeqCmdStatus_NormalComplete);
		}break;
		default:
			break;
	}

	return CBaseSeqCmd::State_Executing();
}


BOOL CVatTrayLoadPPCmd_MovePos::State_Pause()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = (CTaskTrayLoadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTrayLoadPP->MakeVatLog("[CMD:%s] [PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
		case 1000:
		{
			pTaskTrayLoadPP->WriteDisplayLog("[PAUSE]==============================");

			int nAxisCount = pTaskTrayLoadPP->GetMotorCount();
			for (int i = 0; i < nAxisCount; i++) {
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis(static_cast<CTaskTrayBasePP::eMotor>(i));
				if (pMotor == nullptr) {
					continue;
				}

				bool bCloseLimit = pMotor->CheckLimitClose();
				double dCurPos = 0.0;
				pMotor->GetCurrentPos(dCurPos);
				if (bCloseLimit == false)
				{
					pTaskTrayLoadPP->MakeVatLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
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


BOOL CVatTrayLoadPPCmd_MovePos::State_Recovery()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = (CTaskTrayLoadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTrayLoadPP->MakeVatLog("[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
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

//3000
BOOL CVatTrayLoadPPCmd_MovePos::State_NormalComplete()
{
	return CBaseSeqCmd::State_NormalComplete();
}


//4000
BOOL CVatTrayLoadPPCmd_MovePos::State_Alarm()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = (CTaskTrayLoadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTrayLoadPP->MakeVatLog("[CMD: %s] [ALARM PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
		case 4000:
		{
			for (int i = 0; i < CTaskTrayLoadPP::eAxisMax; i++) {
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayLoadPP::eMotor)i);
				if (pMotor == nullptr) {
					continue;
				}

				double dCurPos = 0.0;
				pMotor->GetCurrentPos(dCurPos);

				bool bCloseLimit = pMotor->CheckLimitClose();
				if (bCloseLimit == false)
				{
					pTaskTrayLoadPP->MakeVatLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
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
BOOL CVatTrayLoadPPCmd_MovePos::State_AlarmComplete()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = (CTaskTrayLoadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTrayLoadPP->MakeVatLog("[CMD: %s] [ALARM COMP] [Step=%8d]", m_szCmdName, m_nStep);
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
BOOL CVatTrayLoadPPCmd_MovePos::State_Aborting()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = (CTaskTrayLoadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTrayLoadPP->MakeVatLog("[CMD: %s] [ABORT] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 6000:
	{
		for (int i = 0; i < CTaskTrayLoadPP::eAxisMax; i++) {
			OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayLoadPP::eMotor)i);
			if (pMotor == nullptr) {
				continue;
			}

			double dCurPos = 0.0;
			pMotor->GetCurrentPos(dCurPos);

			bool bCloseLimit = pMotor->CheckLimitClose();
			if (bCloseLimit == false)
			{
				pTaskTrayLoadPP->MakeVatLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
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


BOOL CVatTrayLoadPPCmd_MovePos::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}


//==================================================================================
// COMMENT : Parameter 넣은 후, Cmd 생성 전 Check 필요.
//			
//==================================================================================
BOOL CVatTrayLoadPPCmd_MovePos::IsParamExist()
{
	//Target 위치 및 CmdPicker Check.
// 	if (m_eCmdStage == LOC_IDX_TRAY_PP_Undefined || m_vCmdPicker.size() == 0) {
// 		return FALSE;
// 	}

	//Limit Check는 Position입력에서 진행 중.


	return TRUE;
}


CString CVatTrayLoadPPCmd_MovePos::act_string(int act)
{
	BEGIN_CASE_TO_STR(act);
	CASE_TO_STR(move_start);
	CASE_TO_STR(z_move_safety);
	CASE_TO_STR(z_move_safety_chk);
	CASE_TO_STR(x_px_py_move_target_pos);
	CASE_TO_STR(x_px_py_move_target_pos_chk);
	CASE_TO_STR(z_move_target_pos);
	CASE_TO_STR(z_move_target_pos_chk);
	CASE_TO_STR(move_end);
	ELSE_CASE(_T("unkown_step"));
	END_CASE_TO_STR();
}