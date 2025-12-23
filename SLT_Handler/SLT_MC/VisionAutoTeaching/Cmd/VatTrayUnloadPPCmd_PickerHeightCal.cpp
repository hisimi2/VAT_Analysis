#include "stdafx.h"
#include "VatTrayUnloadPPCmd_PickerHeightCal.h"


CVatTrayUnloadPPCmd_PickerHeightCal::CVatTrayUnloadPPCmd_PickerHeightCal()
{
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));

	m_nSpeed = eZSpeed_None;
	move_offset_um = 0.0;
	m_nStage = -1;

	m_vCmdPicker.clear();
	m_dTargetPosZ = 0.0;

	m_nChkAllVacuum = 0;
	m_vBMAxis.clear();
	
	m_vErrPicker.clear();

	for (int i = 0; i < VAT_PICKER_MAX; i++) {
		m_bFindPos[i] = FALSE;
		m_dPickPos[i] = 0.0;
	}
}


CVatTrayUnloadPPCmd_PickerHeightCal::~CVatTrayUnloadPPCmd_PickerHeightCal()
{
}

int CVatTrayUnloadPPCmd_PickerHeightCal::CheckParam()
{
	//[Time Init]
	m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
	m_tcTimeout.StartTimer();

	
	m_TransferInterlock.SetCheckTime(MOTOR_TIME_OUT_MIDDLE); //Transfer Safety 상태 Check용
	m_TransferInterlock.SetTimerCheckEnd();
	

	//[Cmd Picker]
	// Non Part는 사용하고자 하는 Picker 외부에서 입력.
	if (m_vCmdPicker.size() == 0) {
		//Tray Load PP는 8para/4para 상관 없이 8picker 측정.
		for (int nPkrY = 0; nPkrY < MAX_TRAY_PP_CNT_Y; nPkrY++) {
			for (int nPkrX = 0; nPkrX < MAX_TRAY_PP_CNT_X; nPkrX++) {
				m_vCmdPicker.push_back(CPoint(nPkrX, nPkrY));
			}
		}
	}

	//[Cmd Picker Convert to BMAxis]
	for (int i = 0; i < m_vCmdPicker.size(); i++) {
		int nPkrX = m_vCmdPicker[i].x;
		int nPkrY = m_vCmdPicker[i].y;

		m_vBMAxis.push_back(eBMAxis_UNLOAD_TRAY_Picker01 + (nPkrX + MAX_TRAY_PP_CNT_X * nPkrY));
	}


	// 이전 Z Auto Teaching 8 Picker 사용할 경우 [8/28/2025 dohyeong.kim]
// 	for (int nPkrY = 0; nPkrY < MAX_TRAY_PP_CNT_Y; nPkrY++) {
// 		for (int nPkrX = 0; nPkrX < MAX_TRAY_PP_CNT_X; nPkrX++) {
// 			m_vBMAxis.push_back(eBMAxis_UNLOAD_TRAY_Picker01 + (nPkrX + MAX_TRAY_PP_CNT_X * nPkrY));
// 		}
// 	}

	return ERR_NO_ERROR;
}

int CVatTrayUnloadPPCmd_PickerHeightCal::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

int CVatTrayUnloadPPCmd_PickerHeightCal::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

int CVatTrayUnloadPPCmd_PickerHeightCal::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

int CVatTrayUnloadPPCmd_PickerHeightCal::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

int CVatTrayUnloadPPCmd_PickerHeightCal::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

BOOL CVatTrayUnloadPPCmd_PickerHeightCal::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}

BOOL CVatTrayUnloadPPCmd_PickerHeightCal::State_Executing()
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
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(ERR_TRAY_PP_2_MOTOR_ALARMED, AlarmData);
				return FALSE;
			}
		}
	}

	if (VAT_IDX_TRAY_PP_CONV_1 <= m_nStage && m_nStage <= VAT_IDX_TRAY_PP_CONV_7) {
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
	case height_cal_start:
	{
		m_nStep = height_cal_all_vacuum_on;
// 		if (m_nSpeed == eZSpeed_Fast)
// 			m_nStep = height_cal_move_z_safety;
// 		else //m_nSpeed == eZSpeed_Slow
// 			m_nStep = height_cal_all_vacuum_on;

	}break;
	case height_cal_all_vacuum_on:
	{
		pTaskTrayUnloadPP->WriteDisplayLog("[EXE] Vacuum All On");
		pTaskTrayUnloadPP->MakeVatLog("[%s EXEC=%8d] [Tray PP Vacuum All On] [SRT]", m_szCmdName, m_nStep);

		m_simulCtrl._WaitSeqControl(VAT_SIM_CONTROL_TIME_SHORT);

		//int nErr = pTaskTrayUnloadPP->do_VacBlow_All(eVacBlwSt_VacOn);
		int nErr = pTaskTrayUnloadPP->do_VacBlow(eVacBlwSt_VacOn, m_vCmdPicker);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray Unload PP Vacuum FAIL");
			ReportAlarm(ERR_TRAY_PP_2_VAC_SEN_ERR, AlarmData);
			break;
		}

		int nSleep = g_DMCont.m_dmTrayPP.GN(NDM5_TM_UDTrayPP_Vacuum);
		if (nSleep > 0) {
			Sleep(nSleep);
		}

		m_nStep = height_cal_vacuum_chk;
	}break;

	//처음 위치가 Vacuum 위치일 경우를 위해, 상단에서 Vacuum Check 후, 진행 하도록 변경.  [6/10/2025 dohyeong.kim]
	case height_cal_vacuum_chk:
	{
		double dCurPos = 0.0;
		pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisZ)->GetCurrentPos(dCurPos);

		if (m_nSpeed == eZSpeed_Fast)
		{
			pTaskTrayUnloadPP->WriteDisplayLog("[EXE] [Down Fast] Current Pos : %.3f", dCurPos / 1000.0);

			//Simulation
			if (m_simulCtrl._IsRemainSeqCycle())
			{
				m_nStep = height_cal_picker_down_fast;
				break;
			}
			m_simulCtrl._ClearParam();

			//Picker 한개라도 찾으면 Slow로 전환.
			for (int i = 0; i < m_vCmdPicker.size(); i++)
			{
				if (pTaskTrayUnloadPP->ChkVacuum(eOn, m_vCmdPicker[i]) == ERR_NO_ERROR || m_simulCtrl._IsSimul())
				{
					pTaskTrayUnloadPP->WriteDisplayLog("[EXE] [Down Fast] Find Fast Pos");
					pTaskTrayUnloadPP->MakeVatLog("[%s EXEC=%8d] [Z move Done] [Fast Pos] [END]", m_szCmdName, m_nStep);
					m_nStep = height_cal_save_fast_height_pos;
					break;
				}
			}

			if (m_nStep == height_cal_save_fast_height_pos)
				break;

			m_nStep = height_cal_picker_down_fast;
		}
		else //Speed Slow
		{
			pTaskTrayUnloadPP->WriteDisplayLog("[EXE] [Down Slow] Current Pos : %.3f", dCurPos / 1000.0);

			//Simulation
			if (m_simulCtrl._IsRemainSeqCycle())
			{
				m_nStep = height_cal_picker_down_slow;
				break;
			}
			m_simulCtrl._ClearParam();

			OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisZ);
			
			m_nChkAllVacuum = 0;
			for (int i = 0; i < m_vCmdPicker.size(); i++)
			{
				if (m_bFindPos[i] != TRUE) {
					if (pTaskTrayUnloadPP->ChkVacuum(eOn, m_vCmdPicker[i]) == ERR_NO_ERROR || m_simulCtrl._IsSimul())
					{
						m_nChkAllVacuum++;
						m_bFindPos[i] = TRUE;
						m_dPickPos[i] = g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TRAY, (_eBbxmnpAxis)(m_vBMAxis[i]));
					}
				}
				else {
					//이미 찾은 Picker
					m_nChkAllVacuum++;
				}
			}


			//if (pTaskTrayUnloadPP->ChkVacuum_All(eOn) == ERR_NO_ERROR)
			if (m_nChkAllVacuum == m_vCmdPicker.size())
			{
				m_nChkAllVacuum = 0;
				pTaskTrayUnloadPP->WriteDisplayLog("[EXE] [Down Slow] Find Slow Pos");
				pTaskTrayUnloadPP->MakeVatLog("[%s EXEC=%8d] [Z move Done] [Slow Pos][End]", m_szCmdName, m_nStep);
				if (m_nStage == VAT_NON_PART_PLATAE_JIG) {
					m_nStep = height_cal_save_slow_height_pos_z_auto_focus;
				}
				else {
					m_nStep = height_cal_save_slow_height_pos_z_auto_teaching;
				}
		
				break;
			}

			m_nStep = height_cal_picker_down_slow;
		}
	}break;

	case height_cal_picker_down_fast:
	{
		move_offset_um += VAT_Z_TEACHING_MOVE_OFFSET_FAST; //70um

		// COK 설계치수 확인 및 Z Auto Focus 측정 후, Z 축 Pos limit값 확인 추가 필요 [3/14/2024 dohyeong.kim]  

		pTaskTrayUnloadPP->MakeVatLog("[%s EXEC=%8d] [Z %s move] [Picker Down Fast] [%.3f um] [STR]", m_szCmdName, m_nStep, pTaskTrayUnloadPP->GetLocName((eLocIdxTrayPP)m_nStage), (m_dTargetPosZ + move_offset_um));

		int nErr = pTaskTrayUnloadPP->do_VATMovePos_Z(m_vCmdPicker, m_dTargetPosZ, move_offset_um);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Unload Tray PP Move Fast Z Down Fail");
			OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis(static_cast<CTaskTrayBasePP::eMotor>(eAxisZ));
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(nErr, AlarmData);
			break;
		}

		m_simulCtrl._WaitSeqControl(VAT_SIM_CONTROL_TIME_SHORT);

		m_tcTimeout.SetCheckTime(TIME_OUT_SHORT);
		m_tcTimeout.ResetStartTime();
		m_nStep = height_cal_picker_down_chk;
	}break;
	case height_cal_picker_down_slow:
	{
		move_offset_um += VAT_Z_TEACHING_MOVE_OFFSET_SLOW; //20um

		// COK 설계치수 확인 및 Z Auto Focus 측정 후, Z 축 Pos limit값 확인 추가 필요 [3/14/2024 dohyeong.kim]
		pTaskTrayUnloadPP->MakeVatLog("[%s EXEC=%8d] [Z %s move] [Picker Down Slow] [%.3f um] [STR]", m_szCmdName, m_nStep, pTaskTrayUnloadPP->GetLocName((eLocIdxTrayPP)m_nStage), (m_dTargetPosZ + move_offset_um));

		int nErr = ERR_NO_ERROR;
		std::vector<CPoint> vCmdEachPicker;

		//Picker 하나씩 Down
		for (int i = 0; i < m_vCmdPicker.size(); i++)
		{
			//Vacuum 찾은 Picker 동작 Stop
			if(m_bFindPos[i] == TRUE)
				continue;

			vCmdEachPicker.clear();
			vCmdEachPicker.push_back(m_vCmdPicker[i]);

			nErr = pTaskTrayUnloadPP->do_VATMovePos_Z(vCmdEachPicker, m_dTargetPosZ, move_offset_um);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Unload Tray PP Move Slow Z Down Fail");
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis(static_cast<CTaskTrayBasePP::eMotor>(eAxisZ));
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
		}

		if(nErr != ERR_NO_ERROR)
			break;


// 		for (int i = 0; i < m_vBMAxis.size(); i++)
// 		{
// 			int nErr = pTaskTrayUnloadPP->do_MovePos_ZAxisPicker((_eBbxmnpAxis)m_vBMAxis[i], dTPos, CTaskTrayBasePP::eAxisZ, VAT_MOTOR_SPEED/*10*/);
// 			if (nErr != ERR_NO_ERROR) {
// 				CParamList AlarmData;
// 				AlarmData.m_fnAddItemFormat("Unload Tray PP Move Slow Z Down Fail");
// 				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis(static_cast<CTaskTrayBasePP::eMotor>(eAxisZ));
// 				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
// 				ReportAlarm(nErr, AlarmData);
// 				break;
// 			}
// 		}

		m_simulCtrl._WaitSeqControl(VAT_SIM_CONTROL_TIME_SHORT);

		m_tcTimeout.SetCheckTime(TIME_OUT_SHORT);
		m_tcTimeout.ResetStartTime();
		m_nStep = height_cal_picker_down_chk;
	}break;
	case height_cal_picker_down_chk:
	{
		if (m_tcTimeout.CheckOverTime()) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
			ReportAlarm(ERR_TRAY_PP_2_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData);
			break;
		}

		OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisZ);
		int nAxisDone = pMotor->AxisDone();

		if (nAxisDone == ERR_NO_ERROR) {
			m_nStep = height_cal_vacuum_chk;
			break;
		}
	}break;
	case height_cal_save_fast_height_pos:
	{
		//move_offset_um 저장
		//1mm Up 하여 저장.
		double dFirstPos = move_offset_um - 1000.0;
		if (m_simulCtrl._IsSimul()) {
			dFirstPos = move_offset_um;
		}
		g_VATCommonData.SetFirstPickOffset(VAT_HAND_UNLOAD_TRAYPP, dFirstPos);

		pTaskTrayUnloadPP->MakeVatLog("[%s EXEC=%8d] [Z Save Height ] [Fast Pos] [%.3f um]", m_szCmdName, m_nStep, dFirstPos);

		m_nStep = height_cal_all_picker_1mm_up;
	}break;
	case height_cal_save_slow_height_pos_z_auto_focus:
	{
		pTaskTrayUnloadPP->WriteDisplayLog("===============<STD VACUUM POS>===================");

		if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
			double dEncoder = 100.0;
			for (int i = 0; i < m_vCmdPicker.size(); i++) {
				m_dPickPos[i] = dEncoder;
				dEncoder += 100.0;
			}
		}

		double dStdPickerPos = m_dPickPos[0]; //Std Picker Vacuum Pos

		 //Save
		g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_VacuumPos[VAT_HAND_UNLOAD_TRAYPP][VAT_Z] = dStdPickerPos;
		pTaskTrayUnloadPP->WriteDisplayLog("[EXE] [Std Picker] Vacuum Pos : %.3f um", dStdPickerPos);
		pTaskTrayUnloadPP->MakeVatLog("[%s EXEC=%8d] [Z Save Height] [Slow Pos] [Std Picker] [Vacuum Pos : %.3f um]", m_szCmdName, m_nStep, dStdPickerPos);

		m_nStep = height_cal_all_picker_1mm_up;

	}break;
	case height_cal_save_slow_height_pos_z_auto_teaching:
	{
		pTaskTrayUnloadPP->WriteDisplayLog("===============<VACUUM POS>===================");
		
		if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
			double dEncoder = 100.0;
			for (int i = 0; i < m_vCmdPicker.size(); i++) {
				m_dPickPos[i] = dEncoder;
				dEncoder += 100.0;
			}
		}

		//1. Picker Vacuum Pos 저장.
		for (int nPicker = VAT_PICKER_01; nPicker < VAT_PICKER_MAX; nPicker++) {
			g_VATCommonData.m_stVATZTeachInfoSeq.stPickerVacPos.setPickerVacPos(VAT_HAND_UNLOAD_TRAYPP, m_nStage, nPicker, m_dPickPos[nPicker]);

			pTaskTrayUnloadPP->WriteDisplayLog("[EXE] [Picker %d] Vacuum Pos : %.3f um", nPicker + 1, m_dPickPos[nPicker]);
			pTaskTrayUnloadPP->MakeVatLog("[%s EXEC=%8d] [Z Save Height] [Slow Pos] [Picker %d] [Vacuum Pos : %.3f um]", m_szCmdName, m_nStep, nPicker + 1, m_dPickPos[nPicker]);
		}

		pTaskTrayUnloadPP->WriteDisplayLog("=================<GAP>=====================");

		//2. Picker 간 Gap 저장.
		double pick_base_pos = m_dPickPos[VAT_PICKER_01];
		for (int nPicker = VAT_PICKER_01; nPicker < VAT_PICKER_MAX; nPicker++) {
			double dPickerGap = m_dPickPos[nPicker] - pick_base_pos;
			g_VATCommonData.m_stVATZTeachInfoSeq.stPickerGap.setPickerGap(VAT_HAND_UNLOAD_TRAYPP, m_nStage, nPicker, dPickerGap);

			pTaskTrayUnloadPP->WriteDisplayLog("[EXE] [Picker %d] Gap : %.3f um", nPicker + 1, dPickerGap);
			pTaskTrayUnloadPP->MakeVatLog("[%s EXEC=%8d] [Z Save Height] [Slow Pos] [Picker %d] [Gap : %.3f um]", m_szCmdName, m_nStep, nPicker + 1, dPickerGap);
		}

		m_nStep = height_cal_all_picker_1mm_up;

	}break;
	case height_cal_all_picker_1mm_up:
	{
		//1mm 높은 곳에서 blow
		move_offset_um -= 1000.0; //1mm 

		pTaskTrayUnloadPP->MakeVatLog("[%s EXEC=%8d] [Z move 1mm Up] [%.3f um] [SRT]", m_szCmdName, m_nStep, pTaskTrayUnloadPP->GetLocName((eLocIdxTrayPP)m_nStage), (m_dTargetPosZ + move_offset_um));
		int nErr = pTaskTrayUnloadPP->do_VATMovePos_Z(m_vCmdPicker, m_dTargetPosZ, move_offset_um);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Unload Tray PP Move Z 1mm Up Fail");
			OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis(static_cast<CTaskTrayBasePP::eMotor>(eAxisZ));
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(nErr, AlarmData);
			break;
		}

		m_simulCtrl._WaitSeqControl(VAT_SIM_CONTROL_TIME_SHORT);
		m_tcTimeout.SetCheckTime(TIME_OUT_SHORT);
		m_tcTimeout.ResetStartTime();
		m_nStep = height_cal_all_picker_1mm_up_check;
	}break;
	case height_cal_all_picker_1mm_up_check:
	{
		if (m_tcTimeout.CheckOverTime()) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
			ReportAlarm(ERR_TRAY_PP_2_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData);
			break;
		}

		OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisZ);
		int nAxisDone = pMotor->AxisDone();

		if (nAxisDone == ERR_NO_ERROR) {
			m_nStep = height_cal_vacuum_off;
			break;
		}
	}break;
	case height_cal_vacuum_off:
	{
		int nErr = pTaskTrayUnloadPP->do_VacBlow_All(eVacBlwSt_BlwOn); //[test 필요]eVacBlwSt_BlwOn, eVacBlwSt_Off
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray Unload PP Blow FAIL");
			ReportAlarm(ERR_TRAY_PP_2_VAC_SEN_ERR, AlarmData);
			break;
		}

		int nSleep = g_DMCont.m_dmTrayPP.GN(NDM5_TM_UDTrayPP_Blow);
		if (nSleep > 0) {
			Sleep(nSleep);
		}

		nErr = pTaskTrayUnloadPP->do_VacBlow_All(eVacBlwSt_Off);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray Unload PP Vac/Blow Off FAIL");
			ReportAlarm(ERR_TRAY_PP_2_VAC_SEN_ERR, AlarmData);
			break;
		}

		m_nStep = (m_nSpeed == eZSpeed_Fast) ? height_cal_end : height_cal_all_picker_up;

	}break;
	case height_cal_all_picker_up:
	{
		//Safety Position Move
		pTaskTrayUnloadPP->MakeVatLog("[%s EXEC=%8d] [Move Safety and Picker All Up] [SRT]", m_szCmdName, m_nStep);

		int nLocStageConv = pTaskTrayUnloadPP->TrayHandLocIdxConvertor(RIDX_TRAY_PP_2, m_nStage);
		double dSafetyPosZ = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_2), nLocStageConv, POS_IDX_TRAY_PP_Z_SAFETY);
		int nErr = pTaskTrayUnloadPP->do_VATMovePos_Z(m_vCmdPicker, dSafetyPosZ);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Unload Tray PP Move Z Up Fail");
			OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis(static_cast<CTaskTrayBasePP::eMotor>(eAxisZ));
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(nErr, AlarmData);
			break;
		}

		m_simulCtrl._WaitSeqControl(VAT_SIM_CONTROL_TIME_SHORT);
		m_tcTimeout.SetCheckTime(TIME_OUT_MIDDLE);
		m_tcTimeout.ResetStartTime();
		m_nStep = height_cal_all_picker_up_check;
	}break;
	case height_cal_all_picker_up_check:
	{
		std::vector<CPoint> vPkr;
		pTaskTrayUnloadPP->MakeAllPicker(vPkr);
		m_vErrPicker.clear();
		OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisZ);
		int nAxisDone = pMotor->AxisDone(m_vCmdPicker, m_vErrPicker);
		int nErrSen = pTaskTrayUnloadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);

		if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR) {
			pTaskTrayUnloadPP->MakeVatLog("[%s EXEC=%8d] [Move Safety and Picker All Up] [END]", m_szCmdName, m_nStep);
			m_nStep = height_cal_end;
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
	case height_cal_end:
	{
		ReportCmdEnd(eSeqCmdStatus_NormalComplete);
	}break;
	default:
		break;
	}

	return CBaseSeqCmd::State_Executing();
}

BOOL CVatTrayUnloadPPCmd_PickerHeightCal::State_Pause()
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
	case 0:
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

BOOL CVatTrayUnloadPPCmd_PickerHeightCal::State_Recovery()
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
	case 0:
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

BOOL CVatTrayUnloadPPCmd_PickerHeightCal::State_NormalComplete()
{
	return CBaseSeqCmd::State_NormalComplete();
}

BOOL CVatTrayUnloadPPCmd_PickerHeightCal::State_Alarm()
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
	case 0:
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
	case 1: //Retry, Skip을 기다림.
	{

	}break;
	default:
	{
		m_nStep = 0;
	}break;
	}

	return CBaseSeqCmd::State_Alarm();
}

BOOL CVatTrayUnloadPPCmd_PickerHeightCal::State_AlarmComplete()
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
	case 0:
	{
		return CBaseSeqCmd::State_AlarmComplete();
	}break;
	default: {
		m_nStep = 0;
	}break;
	}

	return CBaseSeqCmd::State_AlarmComplete();
}

BOOL CVatTrayUnloadPPCmd_PickerHeightCal::State_Aborting()
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
	case 0:
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

BOOL CVatTrayUnloadPPCmd_PickerHeightCal::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}

CString CVatTrayUnloadPPCmd_PickerHeightCal::act_string(int act)
{
	BEGIN_CASE_TO_STR(act);
	CASE_TO_STR(height_cal_start);
	//CASE_TO_STR(height_cal_move_z_safety);
	//CASE_TO_STR(height_cal_move_z_safety_chk);
	//CASE_TO_STR(height_cal_move_x_y_px_py_target_pos);
	//CASE_TO_STR(height_cal_move_x_y_px_py_target_pos_chk);
	CASE_TO_STR(height_cal_all_vacuum_on);
	CASE_TO_STR(height_cal_picker_down_fast);
	CASE_TO_STR(height_cal_picker_down_slow);
	CASE_TO_STR(height_cal_picker_down_chk);
	CASE_TO_STR(height_cal_vacuum_chk);
	CASE_TO_STR(height_cal_save_fast_height_pos);
	CASE_TO_STR(height_cal_save_slow_height_pos_z_auto_focus);
	CASE_TO_STR(height_cal_save_slow_height_pos_z_auto_teaching);
	CASE_TO_STR(height_cal_all_picker_1mm_up);
	CASE_TO_STR(height_cal_all_picker_1mm_up_check);
	CASE_TO_STR(height_cal_vacuum_off);
	CASE_TO_STR(height_cal_all_picker_up);
	CASE_TO_STR(height_cal_all_picker_up_check);
	CASE_TO_STR(height_cal_end);
	ELSE_CASE(_T("unknown_step"));
	END_CASE_TO_STR();
}