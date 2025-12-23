#include "stdafx.h"
#include "VatTrayLoadPPFeederCmd_MovePos.h"


CVatTrayLoadPPFeederCmd_MovePos::CVatTrayLoadPPFeederCmd_MovePos(int nMode) : m_nMode(nMode)
{
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));

	m_strCmdStage = _T("");

	for (int nAxis = 0; nAxis < VAT_MAX_AXIS; nAxis++) {
		m_dCmdPos[nAxis] = 0.0;
	}

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


CVatTrayLoadPPFeederCmd_MovePos::~CVatTrayLoadPPFeederCmd_MovePos()
{

}


int CVatTrayLoadPPFeederCmd_MovePos::CheckParam()
{
	m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
	m_tcTimeout.StartTimer();

	return 0;
}


int CVatTrayLoadPPFeederCmd_MovePos::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}


int CVatTrayLoadPPFeederCmd_MovePos::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}


int CVatTrayLoadPPFeederCmd_MovePos::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}


int CVatTrayLoadPPFeederCmd_MovePos::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}


int CVatTrayLoadPPFeederCmd_MovePos::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}


BOOL CVatTrayLoadPPFeederCmd_MovePos::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}


BOOL CVatTrayLoadPPFeederCmd_MovePos::State_Executing()
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


	OneAxis* pMotorX = pTaskTrayLoadPP->GetAxis(CTaskTrayBasePP::eAxisX);		//Tray PP
	OneAxis* pMotorY = g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eAxisY);		//Feeder
	OneAxis* pMotorZ = pTaskTrayLoadPP->GetAxis(CTaskTrayBasePP::eAxisZ);		//Tray PP
	OneAxis* pMotorPX = pTaskTrayLoadPP->GetAxis(CTaskTrayBasePP::eAxisPitchX); //Tray PP
	OneAxis* pMotorPY = pTaskTrayLoadPP->GetAxis(CTaskTrayBasePP::eAxisPitchY); //Tray PP


	//Load Tray PP
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

	//Feeder
	if (pMotorY->ChkMotorUsable() != ERR_NO_ERROR) {
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Tray Feeder check motor usable Cmd Error");
		ReportAlarm(ERR_MOT_INTERLOCK_IS_NOT_ENABLE, AlarmData);
		return FALSE;
	}

	switch (m_nStep)
	{
		case move_start:
		{
			pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [MODE=%d] [%s move] [START]", m_szCmdName, m_nStep, m_nMode, m_strCmdStage);

			m_nStep = z_move_safety;
		}break;
		case z_move_safety:
		{
			pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [Move Z Safety] [SRT]", m_szCmdName, m_nStep);

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
				m_nStep = feeder_cyl_abnormal_chk;
			}
		}break;
		case feeder_cyl_abnormal_chk:
		{
			auto align_for_state = g_TaskTrayFeeder.get_cyl_align_ForBack(eForw);
			auto align_back_state = g_TaskTrayFeeder.get_cyl_align_ForBack(eBack);
			if (align_for_state != ERR_NO_ERROR && align_back_state != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Align cylinder is abnormal state.");
				AlarmData.m_fnAddItemFormat(align_for_state.msg.c_str());
				AlarmData.m_fnAddItemFormat(align_back_state.msg.c_str());
				ReportAlarm(ERR_FEEDER_ALIGN_CYLINDER_ABNORMAL, AlarmData);
				break;
			}

			auto clamp_state = g_TaskTrayFeeder.get_cyl_ClampUnclamp(eClamp);
			auto unclamp_state = g_TaskTrayFeeder.get_cyl_ClampUnclamp(eUnclamp);
			if (clamp_state != ERR_NO_ERROR && unclamp_state != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Clamper is abnormal state.");
				AlarmData.m_fnAddItemFormat(clamp_state.msg.c_str());
				AlarmData.m_fnAddItemFormat(unclamp_state.msg.c_str());
				ReportAlarm(ERR_FEEDER_CLAMP_CYLINDER_ABNORMAL, AlarmData);
				break;
			}

			m_tcTimeout.ResetStartTime();
			m_nStep = feeder_cyl_up_chk;

		}break;
		case feeder_cyl_up_chk:
		{
			if (m_tcTimeout.CheckOverTime()) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Feeder Up Timeout.");
				ReportAlarm(ERR_FEEDER_UP_DOWN_CYLINDER_TIMEOUT, AlarmData);
				break;
			}

			auto chk = g_TaskTrayFeeder.chk_cyl_UpDown(eUp);
			if (chk != ERR_NO_ERROR) {
				pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [Feeder Cylinder Up]", m_szCmdName, m_nStep);

				g_TaskTrayFeeder.do_cyl_UpDown(eUp);

				int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_UPDN_WAITTIME);
				if (0 < nSleep) {
					Sleep(nSleep);
				}

				chk = g_TaskTrayFeeder.chk_cyl_UpDown(eUp);
				if (chk != ERR_NO_ERROR) {
					break;
				}
			}

			m_nStep = x_y_px_py_move_target_pos;

		}break;
		case x_y_px_py_move_target_pos:
		{
			pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [%s] [x, y, Xpitch, Ypitch move ] [STR]", m_szCmdName, m_nStep, m_strCmdStage);

			//Tray Load Pp Move
			double dCmdPos[3] = { m_dCmdPos[VAT_X],  m_dCmdPos[VAT_PITCH_X],  m_dCmdPos[VAT_PITCH_Y] };
			int nAxisNo[3] = { CTaskTrayBasePP::eAxisX, CTaskTrayBasePP::eAxisPitchX, CTaskTrayBasePP::eAxisPitchY };
			int nErr = -1;
			for (int i = 0; i < _countof(nAxisNo); i++) {
				nErr = pTaskTrayLoadPP->do_VATMovePos((CTaskTrayBasePP::eMotor)nAxisNo[i], dCmdPos[i]);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}

			//Tray Feeder Move
			nErr = g_TaskTrayFeeder.do_VATMovePos(m_dCmdPos[VAT_Y]);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				ReportAlarm(nErr, AlarmData);
				break;
			}

			m_tcTimeout.ResetStartTime();
			m_nStep = x_y_px_py_move_target_pos_chk;
		}break;
		case x_y_px_py_move_target_pos_chk:
		{
			int nAxisNo[4] = { CTaskTrayBasePP::eAxisX, CTaskTrayFeeder::eAxisY, CTaskTrayBasePP::eAxisPitchX, CTaskTrayBasePP::eAxisPitchY };
			if (m_tcTimeout.CheckOverTime() == TRUE) {
				CString strErrAxis = _T("");
				for (int i = 0; i < _countof(nAxisNo); i++) {
					OneAxis* pMotor = nullptr;
					if (i == VAT_Y) {
						pMotor = g_TaskTrayFeeder.GetAxis((CTaskTrayFeeder::eMotor)nAxisNo[i]);
					}
					else {
						pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
					}

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
				AlarmData.m_fnAddItemFormat("%s motor time over!", strErrAxis);
				AlarmData.m_fnAddItemFormat("[current position] x=%.2f, y=%.2f, x-pitch=%.2f, y-pitch=%.2f", dPosX / 1000.0, dPosY / 1000.0, dPosPX / 1000.0, dPosPY / 1000.0);
				ReportAlarm(ERR_TRAY_PP_1_MOTOR_MOVE_TIMEOUT, AlarmData);
				break;
			}

			int nChkComp = 0;
			for (int i = 0; i < _countof(nAxisNo); i++) {
				OneAxis* pMotor = nullptr;
				if (i == VAT_Y) {
					pMotor = g_TaskTrayFeeder.GetAxis((CTaskTrayFeeder::eMotor)nAxisNo[i]);
				}
				else {
					pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
				}

				int nAxisDone = pMotor->AxisDone();
				if (nAxisDone == ERR_NO_ERROR) {
					nChkComp++;
				}
				else if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Tray PP axis operation is not complete");
					AlarmData.m_fnAddItemFormat("[ check axis done ] x=%d, y=%d, x-pitch=%d, y-pitch=%d", pMotorX->AxisDone(), pMotorY->AxisDone(), pMotorPX->AxisDone(), pMotorPY->AxisDone());
					ReportAlarm(ERR_TRAY_PP_1_MOTOR_MOVE_FAIL, AlarmData);
					break;
				}
			}

			if (nChkComp == _countof(nAxisNo)) {
				pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [x, y, Xpitch, Ypitch move target pos] [END]", m_szCmdName, m_nStep);
				m_nStep = z_move_target_pos;
				break;
			}
		}break;
		case z_move_target_pos:
		{
			pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [%s] [Z move pos] [STR]", m_szCmdName, m_nStep, m_strCmdStage);

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
		{
			m_nStep = 0;
		}break;
	}

	return CBaseSeqCmd::State_Executing();
}


//1000
BOOL CVatTrayLoadPPFeederCmd_MovePos::State_Pause()
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
			
			//Tray Load PP SetSlowStop
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
			
			//Tray Feeder SetSlowStop
			for (int i = 0; i < CTaskTrayFeeder::eAxisMax; i++) {
				OneAxis* pMotor = g_TaskTrayFeeder.GetAxis((CTaskTrayFeeder::eMotor)i);
				if (pMotor != nullptr)
				{
					double dCurPos = 0.0;
					pMotor->GetCurrentPos(dCurPos);
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


//2000
BOOL CVatTrayLoadPPFeederCmd_MovePos::State_Recovery()
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
BOOL CVatTrayLoadPPFeederCmd_MovePos::State_NormalComplete()
{
	return CBaseSeqCmd::State_NormalComplete();
}


//4000
BOOL CVatTrayLoadPPFeederCmd_MovePos::State_Alarm()
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
		//Tray Load PP SetSlowStop
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

		//Tray Feeder SetSlowStop
		for (int i = 0; i < CTaskTrayFeeder::eAxisMax; i++) {
			OneAxis* pMotor = g_TaskTrayFeeder.GetAxis((CTaskTrayFeeder::eMotor)i);
			if (pMotor != nullptr)
			{
				double dCurPos = 0.0;
				pMotor->GetCurrentPos(dCurPos);
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


//5000
BOOL CVatTrayLoadPPFeederCmd_MovePos::State_AlarmComplete()
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


//6000
BOOL CVatTrayLoadPPFeederCmd_MovePos::State_Aborting()
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
		//Tray Load PP SetSlowStop
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

		//Tray Feeder SetSlowStop
		for (int i = 0; i < CTaskTrayFeeder::eAxisMax; i++) {
			OneAxis* pMotor = g_TaskTrayFeeder.GetAxis((CTaskTrayFeeder::eMotor)i);
			if (pMotor != nullptr)
			{
				double dCurPos = 0.0;
				pMotor->GetCurrentPos(dCurPos);
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


BOOL CVatTrayLoadPPFeederCmd_MovePos::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}


CString CVatTrayLoadPPFeederCmd_MovePos::act_string(int act)
{
	BEGIN_CASE_TO_STR(act);
	CASE_TO_STR(move_start);
	CASE_TO_STR(z_move_safety);
	CASE_TO_STR(z_move_safety_chk);
	CASE_TO_STR(feeder_cyl_abnormal_chk);
	CASE_TO_STR(feeder_cyl_up_chk);
	CASE_TO_STR(x_y_px_py_move_target_pos);
	CASE_TO_STR(x_y_px_py_move_target_pos_chk);
	CASE_TO_STR(z_move_target_pos);
	CASE_TO_STR(z_move_target_pos_chk);
	CASE_TO_STR(move_end);
	ELSE_CASE(_T("unkown_step"));
	END_CASE_TO_STR();
}