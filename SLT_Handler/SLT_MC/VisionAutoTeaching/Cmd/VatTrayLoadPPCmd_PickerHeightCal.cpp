#include "stdafx.h"
#include "VatTrayLoadPPCmd_PickerHeightCal.h"


CVatTrayLoadPPCmd_PickerHeightCal::CVatTrayLoadPPCmd_PickerHeightCal()
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

	//8para, 4para 상관없이 변수 초기화.
	for (int i = 0; i < VAT_PICKER_MAX; i++) {
		m_bFindPos[i] = FALSE;
		m_dPickPos[i] = 0.0;
	}
}


CVatTrayLoadPPCmd_PickerHeightCal::~CVatTrayLoadPPCmd_PickerHeightCal()
{
}

int CVatTrayLoadPPCmd_PickerHeightCal::CheckParam()
{
	//[Time Init]
	m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
	m_tcTimeout.StartTimer();

	//[Cmd Picker]
	// Non Part는 사용하고자 하는 Picker 외부에서 입력.
	if (m_vCmdPicker.size() == 0) {
		//Tray Load PP는 8para / 4para 상관 없이 8picker 측정.
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

		m_vBMAxis.push_back(eBMAxis_LOAD_TRAY_Picker01 + (nPkrX + MAX_TRAY_PP_CNT_X * nPkrY));
	}



	// 이전 Z Auto Teaching 8 Picker 사용할 경우 [8/25/2025 dohyeong.kim]
// 	for (int nPkrY = 0; nPkrY < MAX_TRAY_PP_CNT_Y; nPkrY++) {
// 		for (int nPkrX = 0; nPkrX < MAX_TRAY_PP_CNT_X; nPkrX++) {
// 			m_vBMAxis.push_back(eBMAxis_LOAD_TRAY_Picker01 + (nPkrX + MAX_TRAY_PP_CNT_X * nPkrY));
// 		}
// 	}

	return ERR_NO_ERROR;
}


int CVatTrayLoadPPCmd_PickerHeightCal::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

int CVatTrayLoadPPCmd_PickerHeightCal::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

int CVatTrayLoadPPCmd_PickerHeightCal::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

int CVatTrayLoadPPCmd_PickerHeightCal::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

int CVatTrayLoadPPCmd_PickerHeightCal::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

BOOL CVatTrayLoadPPCmd_PickerHeightCal::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}

BOOL CVatTrayLoadPPCmd_PickerHeightCal::State_Executing()
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
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
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
	case height_cal_start:
	{
		m_nStep = height_cal_all_vacuum_on;
// 	if(m_nSpeed == eZSpeed_Fast)
// 		m_nStep = height_cal_move_z_safety;
// 	else //m_nSpeed == eZSpeed_Slow
// 		m_nStep = height_cal_all_vacuum_on;

	}break;
	case height_cal_all_vacuum_on:
	{
		pTaskTrayLoadPP->WriteDisplayLog("[EXE] Vacuum All On");
		pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [Tray PP Vacuum All On] [STR]", m_szCmdName, m_nStep);

		m_simulCtrl._WaitSeqControl(VAT_SIM_CONTROL_TIME_SHORT);

		//int nErr = pTaskTrayLoadPP->do_VacBlow_All(eVacBlwSt_VacOn);
		int nErr = pTaskTrayLoadPP->do_VacBlow(eVacBlwSt_VacOn, m_vCmdPicker);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray Load PP Vacuum FAIL");
			ReportAlarm(ERR_TRAY_PP_1_VAC_SEN_ERR, AlarmData);
			break;
		}

		int nSleep = g_DMCont.m_dmTrayPP.GN(NDM5_TM_LDTrayPP_Vacuum);
		if (nSleep > 0) {
			Sleep(nSleep);
		}

		m_nStep = height_cal_vacuum_chk;
	}break;

	//처음 위치가 Vacuum 위치일 경우를 위해, 상단에서 Vacuum Check 후, 진행 하도록 변경.  [6/10/2025 dohyeong.kim]
	case height_cal_vacuum_chk:
	{
		double dCurPos = 0.0;
		pTaskTrayLoadPP->GetAxis(CTaskTrayBasePP::eAxisZ)->GetCurrentPos(dCurPos);

		if (m_nSpeed == eZSpeed_Fast)
		{
			pTaskTrayLoadPP->WriteDisplayLog("[EXE] [Down Fast] Current Pos : %.3f", dCurPos / 1000.0);

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
				if (pTaskTrayLoadPP->ChkVacuum(eOn, m_vCmdPicker[i]) == ERR_NO_ERROR || m_simulCtrl._IsSimul())
				{
					pTaskTrayLoadPP->WriteDisplayLog("[EXE] [Down Fast] Find Fast Pos");
					pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [Z move Done] [Fast Pos] [END]", m_szCmdName, m_nStep);
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
			pTaskTrayLoadPP->WriteDisplayLog("[EXE] [Down Slow] Current Pos : %.3f", dCurPos / 1000.0);

			//Simulation
			if (m_simulCtrl._IsRemainSeqCycle())
			{
				m_nStep = height_cal_picker_down_slow;
				break;
			}
			m_simulCtrl._ClearParam();

			OneAxis* pMotor = pTaskTrayLoadPP->GetAxis(CTaskTrayBasePP::eAxisZ);
			
			m_nChkAllVacuum = 0;
			for (int i = 0; i < m_vCmdPicker.size(); i++)
			{
				if (m_bFindPos[i] != TRUE) {
					if (pTaskTrayLoadPP->ChkVacuum(eOn, m_vCmdPicker[i]) == ERR_NO_ERROR || m_simulCtrl._IsSimul())
					{
						m_nChkAllVacuum++;
						m_bFindPos[i] = TRUE;
						m_dPickPos[i] = g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TRAY, (_eBbxmnpAxis)(m_vBMAxis[i]));
					}
				}
				else {
					//이미 찾은 Picker
					m_nChkAllVacuum++;
				}
			}


			//if (pTaskTrayLoadPP->ChkVacuum_All(eOn) == ERR_NO_ERROR)
			if (m_nChkAllVacuum == m_vCmdPicker.size())
			{
				m_nChkAllVacuum = 0;
				pTaskTrayLoadPP->WriteDisplayLog("[EXE] [Down Slow] Find Slow Pos");
				pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [Z move Done] [Slow Pos][End]", m_szCmdName, m_nStep);
				if (m_nStage == VAT_NON_PART_PLATAE_JIG) {
					m_nStep = height_cal_save_slow_height_pos_z_auto_focus;
				}
				else {
					int nMode = g_DMCont.m_dmVAT.GN(NDM14_VAT_CURRENT_MODE);
					if (nMode == VAT_SEQ_MODE_VAT) {
						m_nStep = height_cal_save_slow_height_pos_vision_auto_teaching;
					}
					else { //Z Auto Teaching
						m_nStep = height_cal_save_slow_height_pos_z_auto_teaching;
					}					
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

		pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [Z %s move] [Picker Down Fast] [%.3f um] [STR]", m_szCmdName, m_nStep, pTaskTrayLoadPP->GetLocName((eLocIdxTrayPP)m_nStage), (m_dTargetPosZ + move_offset_um));

		int nErr = pTaskTrayLoadPP->do_VATMovePos_Z(m_vCmdPicker, m_dTargetPosZ, move_offset_um);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;						
			AlarmData.m_fnAddItemFormat("Load Tray PP Move Fast Z Down Fail");	
			OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
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
		pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [Z %s move] [Picker Down Slow] [%.3f um] [STR]", m_szCmdName, m_nStep, pTaskTrayLoadPP->GetLocName((eLocIdxTrayPP)m_nStage), (m_dTargetPosZ + move_offset_um));

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

			nErr = pTaskTrayLoadPP->do_VATMovePos_Z(vCmdEachPicker, m_dTargetPosZ, move_offset_um);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Load Tray PP Move Slow Z Down Fail");
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
		}

		if(nErr != ERR_NO_ERROR)
			break;

		//Picker 하나씩 Down
//  		double z_safety_pos_um = pTaskTrayLoadPP->GetTeachPos(m_eStage, m_cCmdPocket, m_vCmdPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
//  		move_offset_um += VAT_Z_TEACHING_MOVE_OFFSET_SLOW; //50um
//  		double dTPos = z_safety_pos_um + move_offset_um;
//  		for (int i = 0; i < m_vBMAxis.size(); i++)
//  		{
//  			int nErr = pTaskTrayLoadPP->do_MovePos_ZAxisPicker((_eBbxmnpAxis)m_vBMAxis[i], dTPos, CTaskTrayBasePP::eAxisZ, VAT_MOTOR_SPEED/*10*/);
//  			if (nErr != ERR_NO_ERROR) {
//  				CParamList AlarmData;
//  				AlarmData.m_fnAddItemFormat("Load Tray PP Move Slow Z Down Fail");
//  				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
//  				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
//  				ReportAlarm(nErr, AlarmData);
//  				break;
//  			}
//  		}		



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
			ReportAlarm(ERR_TRAY_PP_1_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData);
			break;
		}

		OneAxis* pMotor = pTaskTrayLoadPP->GetAxis(CTaskTrayBasePP::eAxisZ);
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
		g_VATCommonData.SetFirstPickOffset(VAT_HAND_LOAD_TRAYPP, dFirstPos);

		pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [Z Save Height] [Fast Pos] [%.3f um]", m_szCmdName, m_nStep, m_dTargetPosZ + dFirstPos);

		m_nStep = height_cal_all_picker_1mm_up;
	}break;
	case height_cal_save_slow_height_pos_z_auto_focus: //[Z Auto Focus]
	{
		pTaskTrayLoadPP->WriteDisplayLog("===============<STD VACUUM POS>===================");

		if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
			double dEncoder = 100.0;
			for (int i = 0; i < m_vCmdPicker.size(); i++) {
				m_dPickPos[i] = dEncoder;
				dEncoder += 100.0;
			}
		}

		double dStdPickerPos = m_dPickPos[0]; //Std Picker Vacuum Pos

		//Save
		g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_VacuumPos[VAT_HAND_LOAD_TRAYPP][VAT_Z] = dStdPickerPos;
		pTaskTrayLoadPP->WriteDisplayLog("[EXE] [Std Picker] Vacuum Pos : %.3f um", dStdPickerPos);
		pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [Z Auto Focus] [Save Slow Pos] [Std Picker] [Vacuum Pos : %.3f um]", m_szCmdName, m_nStep, dStdPickerPos);

		m_nStep = height_cal_all_picker_1mm_up;

	}break;
	case height_cal_save_slow_height_pos_vision_auto_teaching: //[Vision Auto Teaching]
	{
		pTaskTrayLoadPP->WriteDisplayLog("===============<STD VACUUM POS>===================");

		if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
			double dEncoder = 100.0;
			for (int i = 0; i < m_vCmdPicker.size(); i++) {
				m_dPickPos[i] = dEncoder;
				dEncoder += 100.0;
			}
		}

		double dStdPickerPos = m_dPickPos[0]; //Std Picker Vacuum Pos

		//Save 
		double dTargetFocusPosZ = dStdPickerPos - g_VATCommonData.m_stVATCamCenterCalInfo.dTopCamToPkrDistance[VAT_HAND_LOAD_TRAYPP][VAT_Z];
		g_VATCommonData.m_stVATInfoSeq.stHandBasePos.setBasePos(VAT_HAND_LOAD_TRAYPP, m_nStage, VAT_Z, dTargetFocusPosZ);
		pTaskTrayLoadPP->WriteDisplayLog("[EXE] [Std Picker] Vacuum Pos : %.3f um", dStdPickerPos);
		pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [Vision Auto Teaching] [Save Slow Pos] [Target Focus Pos : %.3f um]", m_szCmdName, m_nStep, dTargetFocusPosZ);

		m_nStep = height_cal_all_picker_1mm_up;
	}break;
	case height_cal_save_slow_height_pos_z_auto_teaching: //[Z Auto Teaching]
	{
		pTaskTrayLoadPP->WriteDisplayLog("===============<VACUUM POS>===================");

 		if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
 			double dEncoder = 100.0;
 			for (int i = 0; i < m_vCmdPicker.size(); i++) {
 				m_dPickPos[i] = dEncoder;
 				dEncoder += 100.0;
 			}
 		}

		//1. Picker Vacuum Pos 저장.
		for (int nPicker = VAT_PICKER_01; nPicker < VAT_PICKER_MAX; nPicker++) {
			g_VATCommonData.m_stVATZTeachInfoSeq.stPickerVacPos.setPickerVacPos(VAT_HAND_LOAD_TRAYPP, m_nStage, nPicker, m_dPickPos[nPicker]);

			pTaskTrayLoadPP->WriteDisplayLog("[EXE] [Picker %d] Vacuum Pos : %.3f um", nPicker + 1, m_dPickPos[nPicker]);
			pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [Z Auto Teaching] [Save Slow Pos] [Picker %d] [Vacuum Pos : %.3f um]", m_szCmdName, m_nStep, nPicker + 1, m_dPickPos[nPicker]);
		}
 
		pTaskTrayLoadPP->WriteDisplayLog("=================<GAP>=====================");
 
 		//2. Picker 간 Gap 저장.
		double pick_base_pos = m_dPickPos[VAT_PICKER_01];
		for (int nPicker = VAT_PICKER_01; nPicker < VAT_PICKER_MAX; nPicker++) {
			double dPickerGap = m_dPickPos[nPicker] - pick_base_pos;
			g_VATCommonData.m_stVATZTeachInfoSeq.stPickerGap.setPickerGap(VAT_HAND_LOAD_TRAYPP, m_nStage, nPicker, dPickerGap);

			pTaskTrayLoadPP->WriteDisplayLog("[EXE] [Picker %d] Gap : %.3f um", nPicker + 1, dPickerGap);
			pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [Z Save Height] [Slow Pos] [Picker %d] [Gap : %.3f um]", m_szCmdName, m_nStep, nPicker + 1, dPickerGap);
		}

		m_nStep = height_cal_all_picker_1mm_up;

	}break;
	case height_cal_all_picker_1mm_up:
	{
		//1mm 높은 곳에서 blow
		move_offset_um -= 1000.0; //1mm 

		pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [Z move 1mm Up] [%.3f um] [STR]", m_szCmdName, m_nStep, pTaskTrayLoadPP->GetLocName((eLocIdxTrayPP)m_nStage), m_dTargetPosZ + move_offset_um);
		int nErr = pTaskTrayLoadPP->do_VATMovePos_Z(m_vCmdPicker, m_dTargetPosZ, move_offset_um);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Load Tray PP Move Z 1mm Up Fail");
			OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
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
			ReportAlarm(ERR_TRAY_PP_1_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData);
			break;
		}

		OneAxis* pMotor = pTaskTrayLoadPP->GetAxis(CTaskTrayBasePP::eAxisZ);
		int nAxisDone = pMotor->AxisDone();

		if (nAxisDone == ERR_NO_ERROR) {
			m_nStep = height_cal_vacuum_off;
			break;
		}
	}break;
	case height_cal_vacuum_off:
	{
		int nErr = pTaskTrayLoadPP->do_VacBlow_All(eVacBlwSt_BlwOn); //[test 필요]eVacBlwSt_BlwOn, eVacBlwSt_Off
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray Load PP Blow FAIL");
			ReportAlarm(ERR_TRAY_PP_1_VAC_SEN_ERR, AlarmData);
			break;
		}

		int nSleep = g_DMCont.m_dmTrayPP.GN(NDM5_TM_LDTrayPP_Blow);
		if (nSleep > 0) {
			Sleep(nSleep);
		}

		nErr = pTaskTrayLoadPP->do_VacBlow_All(eVacBlwSt_Off);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray Load PP Vac/Blow Off FAIL");
			ReportAlarm(ERR_TRAY_PP_1_VAC_SEN_ERR, AlarmData);
			break;
		}

		m_nStep = (m_nSpeed == eZSpeed_Fast) ? height_cal_end : height_cal_all_picker_up;

	}break;
	case height_cal_all_picker_up:
	{
		//Safety Position Move
		pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [Move Safety and Picker All Up] [STR]", m_szCmdName, m_nStep);

		int nLocStageConv = pTaskTrayLoadPP->TrayHandLocIdxConvertor(RIDX_TRAY_PP_1, m_nStage);
		double dSafetyPosZ = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_1), nLocStageConv, POS_IDX_TRAY_LOAD_PP_Z_SAFETY);
		int nErr = pTaskTrayLoadPP->do_VATMovePos_Z(m_vCmdPicker, dSafetyPosZ);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Load Tray PP Move Z Up Fail");
			OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
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
		pTaskTrayLoadPP->MakeAllPicker(vPkr);
		m_vErrPicker.clear();
		OneAxis* pMotor = pTaskTrayLoadPP->GetAxis(CTaskTrayBasePP::eAxisZ);
		int nAxisDone = pMotor->AxisDone(m_vCmdPicker, m_vErrPicker);
		int nErrSen = pTaskTrayLoadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);
		if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR) {
			pTaskTrayLoadPP->MakeVatLog("[%s EXEC=%8d] [Move Safety and Picker All Up] [END]", m_szCmdName, m_nStep);
			m_nStep = height_cal_end;
			break;
		}
		else {
			std::vector<CPoint> vErrPkr; vErrPkr.clear();
			CString sAlarmData = _T("");
			int nErr = pTaskTrayLoadPP->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, pTaskTrayLoadPP->GetSafetyFailPicker(ePickerSafetySensorCheckType::eSafetySensorCheck), vErrPkr, sAlarmData);

			if (m_tcTimeout.CheckOverTime()) {
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

BOOL CVatTrayLoadPPCmd_PickerHeightCal::State_Pause()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = (CTaskTrayLoadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTrayLoadPP->MakeVatLog("[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0:
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

BOOL CVatTrayLoadPPCmd_PickerHeightCal::State_Recovery()
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
	case 0:
	{
		//Simul 정상 실행을 위함.
// 		if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
// 			for (int i = 0; i < VAT_PICKER_MAX; i++) {
// 				m_bFindPos[i] = FALSE;
// 			}
// 		}

		SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing);
	}break;
	default:
	{
		m_nStep = 0;
	}break;
	}

	return CBaseSeqCmd::State_Recovery();
}

BOOL CVatTrayLoadPPCmd_PickerHeightCal::State_NormalComplete()
{
	return CBaseSeqCmd::State_NormalComplete();
}

BOOL CVatTrayLoadPPCmd_PickerHeightCal::State_Alarm()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = (CTaskTrayLoadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTrayLoadPP->MakeVatLog("[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0:
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

BOOL CVatTrayLoadPPCmd_PickerHeightCal::State_AlarmComplete()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = (CTaskTrayLoadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		//pTaskTestPP->MakeVatLog("[CMD: %s] [ALARM COMP] [Step=%8d]", m_szCmdName, m_nStep);
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

BOOL CVatTrayLoadPPCmd_PickerHeightCal::State_Aborting()
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
	case 0:
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

BOOL CVatTrayLoadPPCmd_PickerHeightCal::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}

CString CVatTrayLoadPPCmd_PickerHeightCal::act_string(int act)
{
	BEGIN_CASE_TO_STR(act);
	CASE_TO_STR(height_cal_start);
	//CASE_TO_STR(height_cal_move_z_safety);
	//CASE_TO_STR(height_cal_move_z_safety_chk);
	//CASE_TO_STR(height_cal_move_x_px_py_target_pos);
	//CASE_TO_STR(height_cal_move_x_px_py_target_pos_chk);
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

