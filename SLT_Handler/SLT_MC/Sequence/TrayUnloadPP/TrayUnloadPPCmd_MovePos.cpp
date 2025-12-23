#include "StdAfx.h"
#include "TrayUnloadPPCmd_MovePos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTrayUnloadPPCmd_MovePos_Param::CTrayUnloadPPCmd_MovePos_Param(
	eLocIdxTrayPP           _eStage,
	CPoint                  _Pocket,
	std::vector<CPoint>     _vPicker
)
{
	eStage  = _eStage;
	Pocket  = _Pocket;
	vPicker = _vPicker;
}

CTrayUnloadPPCmd_MovePos_Param::CTrayUnloadPPCmd_MovePos_Param()
{
	eStage = LOC_IDX_TRAY_PP_Undefined;
	// Pocket;
	vPicker.clear();
}

CTrayUnloadPPCmd_MovePos_Param::CTrayUnloadPPCmd_MovePos_Param(const CTrayUnloadPPCmd_MovePos_Param& src)
{
	eStage = src.eStage;
	Pocket = src.Pocket;
	vPicker = src.vPicker;
}

CTrayUnloadPPCmd_MovePos_Param& CTrayUnloadPPCmd_MovePos_Param::operator=(const CTrayUnloadPPCmd_MovePos_Param& src)
{
	if (this == &src) return *this;
	eStage = src.eStage;
	Pocket = src.Pocket;
	vPicker = src.vPicker;

	return *this;
}

CTrayUnloadPPCmd_MovePos_Param::~CTrayUnloadPPCmd_MovePos_Param()
{
	eStage = eLocIdxTrayPP::LOC_IDX_TRAY_PP_Undefined;
	vPicker.clear();
}


CTrayUnloadPPCmd_MovePos::CTrayUnloadPPCmd_MovePos(CTrayUnloadPPCmd_MovePos_Param _Prm)
	:m_Cmd(_Prm)
{
	m_nCmdId      = eSeqCmd_TrayUnloadPP_MovePos;
	m_nRetryCount = 0;
	m_Cmd.vPicker.clear();
	sprintf_s(m_szCmdName, sizeof(m_szCmdName), "TrayUnloadPPCmd_MovePos");
}

CTrayUnloadPPCmd_MovePos::CTrayUnloadPPCmd_MovePos()
{
	m_nCmdId = eSeqCmd_TrayUnloadPP_MovePos;
	m_nRetryCount = 0;
	m_Cmd.vPicker.clear();
	sprintf_s(m_szCmdName, sizeof(m_szCmdName), "TrayUnloadPPCmd_MovePos");
}

CTrayUnloadPPCmd_MovePos::~CTrayUnloadPPCmd_MovePos( void )
{
}

int CTrayUnloadPPCmd_MovePos::CheckParam()
{
	double dTimeout = MOTOR_TIME_OUT_MIDDLE;
	m_tcTimeout.SetCheckTime(dTimeout);
	m_tcTimeout.StartTimer();

	m_TransferInterlock.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
	m_TransferInterlock.SetTimerCheckEnd();

	return ERR_NO_ERROR;
}

int CTrayUnloadPPCmd_MovePos::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

int CTrayUnloadPPCmd_MovePos::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

int CTrayUnloadPPCmd_MovePos::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

int CTrayUnloadPPCmd_MovePos::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

int CTrayUnloadPPCmd_MovePos::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

BOOL CTrayUnloadPPCmd_MovePos::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}

BOOL CTrayUnloadPPCmd_MovePos::State_Executing()
{
	// check alarm
	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	m_nStep_Pre = m_nStep;

	for (int i = 0; i < CTaskTrayBasePP::eAxisMax; i++) {
		OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)i);
		if (pMotor->ChkMotorUsable() != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray PP check motor usable Cmd Error");
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(ERR_TRAY_PP_2_MOTOR_ALARMED, AlarmData);
			return FALSE;
		}
	}

	if (LOC_IDX_TRAY_PP_CONV_1 <= m_Cmd.eStage && m_Cmd.eStage <= LOC_IDX_TRAY_PP_CONV_7) {
		if (g_TaskTransfer.is_safety_state() == false) {

			if (m_TransferInterlock.IsChecking() == FALSE)
			{
				m_TransferInterlock.StartTimer();
			}

			if (m_TransferInterlock.CheckOverTime() == TRUE)
			{
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

	OneAxis* pMotorX  = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisX);
	OneAxis* pMotorY  = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisY);
	OneAxis* pMotorZ  = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisZ);
	OneAxis* pMotorPX = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisPitchX);
	OneAxis* pMotorPY = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisPitchY);

	switch (m_nStep)
	{	
	case 0: // check interlock
	{
		std::vector<CPoint> vPkr;
		pTaskTrayUnloadPP->MakeAllPicker(vPkr);

		pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [move safety and picker all up] [SRT]", m_szCmdName, m_nStep);
		int nErr = pTaskTrayUnloadPP->do_MovePos(m_Cmd.eStage, m_Cmd.Pocket, vPkr, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(nErr, AlarmData);
			break;
		}

		m_tcTimeout.SetCheckTime(TIME_OUT_SHORT);
		m_tcTimeout.StartTimer();
		m_nStep++;
	}break;

	case 1:
	{
		std::vector<CPoint> vPkr;
		pTaskTrayUnloadPP->MakeAllPicker(vPkr);
		int nErrSen = pTaskTrayUnloadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);
		m_vErrPicker.clear();
		int nAxisDone = pMotorZ->AxisDone(vPkr, m_vErrPicker);
		if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR /*&& nErrPicker == ERR_NO_ERROR */) {
			pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [move safety and picker all up] [END]", m_szCmdName, m_nStep);
			m_nStep = 2000;
			break;
		}
		else
		{
			std::vector<CPoint> vErrPkr; vErrPkr.clear();
			CString sAlarmData = _T("");
			int nErr = pTaskTrayUnloadPP->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, pTaskTrayUnloadPP->GetSafetyFailPicker(ePickerSafetySensorCheckType::eSafetySensorCheck), vErrPkr, sAlarmData);

			if (m_tcTimeout.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("%s", sAlarmData);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				ReportAlarm(nErr, AlarmData, vErrPkr);
				break;
			}
		}
	}break;

	// move x,y,xp,yp to target pos
	case 2000:
	{
		double dTPos[3] = { 0.0, };
		dTPos[0] = pTaskTrayUnloadPP->GetTeachPos(m_Cmd.eStage, m_Cmd.Pocket, m_Cmd.vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisY);
		dTPos[1] = pTaskTrayUnloadPP->GetTeachPos(m_Cmd.eStage, m_Cmd.Pocket, m_Cmd.vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisPitchX);
		dTPos[2] = pTaskTrayUnloadPP->GetTeachPos(m_Cmd.eStage, m_Cmd.Pocket, m_Cmd.vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisPitchY);

		int nAxisNo[3] = { CTaskTrayBasePP::eAxisY, CTaskTrayBasePP::eAxisPitchX, CTaskTrayBasePP::eAxisPitchY };
		pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [y, Xpitch, Ypitch move %s x:%d / y:%d] [SRT]", m_szCmdName, m_nStep, pTaskTrayUnloadPP->GetLocName(m_Cmd.eStage), m_Cmd.Pocket.x + 1, m_Cmd.Pocket.y + 1);
		for (int i = 0; i < _countof(nAxisNo); i++) {
			int nErr = pTaskTrayUnloadPP->do_MovePos(m_Cmd.eStage, m_Cmd.Pocket, m_Cmd.vPicker, CTaskTrayBasePP::eZPOS_SAFETY, (CTaskTrayBasePP::eMotor)nAxisNo[i]);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
		}
		m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
		m_tcTimeout.ResetStartTime();
		m_nStep++;
	}break;

	case 2001:
	{
		int nAxisNo[3] = { CTaskTrayBasePP::eAxisY, CTaskTrayBasePP::eAxisPitchX, CTaskTrayBasePP::eAxisPitchY };



		int nChkComp = 0;
		for (int i = 0; i < _countof(nAxisNo); i++) {
			OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				nChkComp++;
			}
			else
			{
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

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Tray PP axis operation is not complete");
					AlarmData.m_fnAddItemFormat("[ check axis done ] x=%d, y=%d, x-pitch=%d, y-pitch=%d", pMotorX->AxisDone(), pMotorY->AxisDone(), pMotorPX->AxisDone(), pMotorPY->AxisDone());
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_TRAY_PP_2_MOTOR_MOVE_FAIL, AlarmData);
					break;
				}
			}
		}
		if (nChkComp == _countof(nAxisNo)) {
			pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [y, Xpitch, Ypitch move %s x:%d / y:%d] [END]", m_szCmdName, m_nStep, pTaskTrayUnloadPP->GetLocName(m_Cmd.eStage), m_Cmd.Pocket.x + 1, m_Cmd.Pocket.y + 1);
			m_nStep = 3000;
			break;
		}
	}break;

	case 3000:
	{
		double dTPos[1] = { 0.0, };
		dTPos[0] = pTaskTrayUnloadPP->GetTeachPos(m_Cmd.eStage, m_Cmd.Pocket, m_Cmd.vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX);

		int nAxisNo[1] = { CTaskTrayBasePP::eAxisX };
		pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [x %s x:%d / y:%d] [SRT]", m_szCmdName, m_nStep, pTaskTrayUnloadPP->GetLocName(m_Cmd.eStage), m_Cmd.Pocket.x + 1, m_Cmd.Pocket.y + 1);
		for (int i = 0; i < _countof(nAxisNo); i++) {
			int nErr = pTaskTrayUnloadPP->do_MovePos(m_Cmd.eStage, m_Cmd.Pocket, m_Cmd.vPicker, CTaskTrayBasePP::eZPOS_SAFETY, (CTaskTrayBasePP::eMotor)nAxisNo[i]);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
		}
		m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
		m_tcTimeout.ResetStartTime();
		m_nStep++;
	}break;

	case 3001:
	{
		int nAxisNo[1] = { CTaskTrayBasePP::eAxisX };

		int nChkComp = 0;
		for (int i = 0; i < _countof(nAxisNo); i++) {
			OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				nChkComp++;
			}
			else 
			{
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

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Tray PP axis operation is not complete");
					AlarmData.m_fnAddItemFormat("[ check axis done ] x=%d, y=%d, x-pitch=%d, y-pitch=%d", pMotorX->AxisDone(), pMotorY->AxisDone(), pMotorPX->AxisDone(), pMotorPY->AxisDone());
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_TRAY_PP_2_MOTOR_MOVE_FAIL, AlarmData);
					break;
				}
			}
		}
		if (nChkComp == _countof(nAxisNo)) {
			pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [x, y, Xpitch, Ypitch move %s x:%d / y:%d] [END]", m_szCmdName, m_nStep, pTaskTrayUnloadPP->GetLocName(m_Cmd.eStage), m_Cmd.Pocket.x + 1, m_Cmd.Pocket.y + 1);
			m_nStep = 99999;
			break;
		}
	}break;

	case 99999:
	{
		double dTargetPos = pTaskTrayUnloadPP->GetTeachPos(m_Cmd.eStage, m_Cmd.Pocket, m_Cmd.vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisY);
		if (dTargetPos > MIN_TRAY_PP_WORK_ENABLE_POS_um)
		{
			int nErr = pTaskTrayUnloadPP->chk_TrayUnload_Y_Safety_sen(DEF_ON);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Tray PP Y axis is not safety");
				ReportAlarm(ERR_TRAY_PP_2_Y_IS_NOT_SAFETY, AlarmData);
				break;
			}
		}
		// 혹시 안전 센서가 추가되면 마지막 안전 센서 확인 코드를 여기에 넣는다. 안전센서 삭제되어 코드 삭제 함.
		pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [Move Pos : %s, eSeqCmdStatus_NormalComplete]", m_szCmdName, m_nStep, pTaskTrayUnloadPP->GetLocName(m_Cmd.eStage));
		ReportCmdEnd(eSeqCmdStatus_NormalComplete);
	}break;

	}
	return CBaseSeqCmd::State_Executing();
}

BOOL CTrayUnloadPPCmd_MovePos::State_Pause()
{
	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTrayUnloadPP->MakeLog("[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0:
	{
		int nAxisCount = pTaskTrayUnloadPP->GetMotorCount();
		for (int i = 0; i < nAxisCount; i++) {
			double dCurPos = 0;
			OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis(static_cast<CTaskTrayBasePP::eMotor>(i));
			bool bCloseLimit = pMotor->CheckLimitClose();
			if (bCloseLimit == false)
			{
				pMotor->GetCurrentPos(dCurPos);
				pTaskTrayUnloadPP->MakeLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
				pMotor->SetSlowStop();
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

BOOL CTrayUnloadPPCmd_MovePos::State_Recovery()
{
	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTrayUnloadPP->MakeLog("[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0:
	{
		pTaskTrayUnloadPP->MakeLog("[%s RECOVERY=%8d] [z move safety & picker all up] [SRT]", m_szCmdName, m_nStep);
		std::vector<CPoint> vPkr;
		pTaskTrayUnloadPP->MakeAllPicker(vPkr);
		int nErr = pTaskTrayUnloadPP->do_MovePos(m_Cmd.eStage, m_Cmd.Pocket, vPkr, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray PP z axis move safety position command error");
			OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(nErr, AlarmData);
			break;
		}

		m_tcTimeout.SetCheckTime(TIME_OUT_SHORT);
		m_tcTimeout.ResetStartTime();
		m_nStep = 1000;
	}break;
	case 1000:
	{
		std::vector<CPoint> vPkr;
		pTaskTrayUnloadPP->MakeAllPicker(vPkr);
		int nErrSen = pTaskTrayUnloadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);
		OneAxis* pMotorZ = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisZ);
		m_vErrPicker.clear();
		int nAxisDone = pMotorZ->AxisDone(vPkr, m_vErrPicker);
		if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR ) {
			pTaskTrayUnloadPP->MakeLog("[%s RECOVERY=%8d] [z move safety & picker all up] [END]", m_szCmdName, m_nStep);
			m_nStep = 2000;
			break;
		}
		else {
			std::vector<CPoint> vErrPkr; vErrPkr.clear();
			CString sAlarmData = _T("");
			int nErr = pTaskTrayUnloadPP->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, pTaskTrayUnloadPP->GetSafetyFailPicker(ePickerSafetySensorCheckType::eSafetySensorCheck), vErrPkr, sAlarmData);

			if (m_tcTimeout.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("%s", sAlarmData);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				ReportAlarm(nErr, AlarmData, vErrPkr);
				break;
			}
		}	
	}break;
	case 2000:
	{
		OneAxis* pMotorY = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisY);
		std::vector<CPoint> vPicker;
		vPicker.push_back(CPoint(0, 0));

		int nErr = pTaskTrayUnloadPP->do_MovePos(LOC_IDX_TRAY_PP_SAFETY, CPoint(0, 0), vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisY);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("%s", pMotorY->GetAxisName());
			ReportAlarm(nErr, AlarmData);
			break;
		}
		m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
		m_tcTimeout.ResetStartTime();
		m_nStep = 3000;
	}break;
	case 3000:
	{
		OneAxis* pMotorY = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisY);

		int nAxisDone = pMotorY->AxisDone();
		int nErr = pTaskTrayUnloadPP->chk_TrayUnload_Y_Safety_sen(TRUE);
		if (nAxisDone == ERR_NO_ERROR && nErr == ERR_NO_ERROR) {
			m_nStep = 99999;
			break;
		}
		else 
		{
			if (m_tcTimeout.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Tray PP Y axis move y safety time over");
				AlarmData.m_fnAddItemFormat("Axis=%s ", pMotorY->GetAxisName());
				ReportAlarm(ERR_TRAY_PP_2_MOTOR_MOVE_TIMEOUT, AlarmData);
				break;
			}

			if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Tray PP Y axis axis done fail");
				AlarmData.m_fnAddItemFormat("Axis=%s ", pMotorY->GetAxisName());
				ReportAlarm(ERR_TRAY_PP_2_MOTOR_MOVE_FAIL, AlarmData);
				break;
			}
		}
	}break;

	case 99999:
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

BOOL CTrayUnloadPPCmd_MovePos::State_NormalComplete()
{
	return CBaseSeqCmd::State_NormalComplete();
}

BOOL CTrayUnloadPPCmd_MovePos::State_Alarm()
{
	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTrayUnloadPP->MakeLog("[CMD: %s] [ALARM] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0: // alarm 발생 했을 때 처리해 줘야 할꺼 처리 해줌
	{
		int nAxisCount = pTaskTrayUnloadPP->GetMotorCount();
		for (int i = 0; i < nAxisCount; i++) {
			OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)i);
			//pMotor->SetSlowStop();
			double dCurPos = 0;
			bool bCloseLimit = pMotor->CheckLimitClose();
			if (bCloseLimit == false)
			{
				pMotor->GetCurrentPos(dCurPos);
				pTaskTrayUnloadPP->MakeLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
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

BOOL CTrayUnloadPPCmd_MovePos::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete();
}

BOOL CTrayUnloadPPCmd_MovePos::State_Aborting()
{
	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTrayUnloadPP->MakeLog("[CMD: %s] [ABORT] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0:
	{
		int nAxisCount = pTaskTrayUnloadPP->GetMotorCount();
		for (int i = 0; i < nAxisCount; i++) {
			OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)i);
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

BOOL CTrayUnloadPPCmd_MovePos::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}

