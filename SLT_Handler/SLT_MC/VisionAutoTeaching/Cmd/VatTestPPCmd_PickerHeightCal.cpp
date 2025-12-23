#include "stdafx.h"
#include "VatTestPPCmd_PickerHeightCal.h"


CVatTestPPCmd_PickerHeightCal::CVatTestPPCmd_PickerHeightCal()
{
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));

	m_nHand = -1;
	m_nSpeed = eZSpeed_None;
	move_offset_um = 0.0;
	m_nStage = -1;

	m_vCmdPicker.clear();
	m_dTargetPosZ = 0.0;

	m_nChkAllVacuum = 0;
	m_vAllPicker.clear();
	m_vBMAxis.clear();

	m_vErrPicker.clear();

	for (int i = 0; i < VAT_PICKER_MAX; i++) {
		m_bFindPos[i] = FALSE;
		m_dPickPos[i] = 0.0;
	}

	
	m_nTestPpIdx = eTestPP_1;
	m_nTestPpType = VAT_TESTPP_LOAD;	
}


CVatTestPPCmd_PickerHeightCal::~CVatTestPPCmd_PickerHeightCal()
{
}

int CVatTestPPCmd_PickerHeightCal::CheckParam()
{
	CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;

	//[Time Init]
	m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
	m_tcTimeout.StartTimer();

	//[Test Pp Index]
	m_nTestPpIdx = pTaskTestPP->m_nTestPPIdx;

	//[Cmd Picker] -> 8Para, 4Para에 따라 변경.
	// Non Part는 사용하고자 하는 Picker 외부에서 입력.
	if (m_vCmdPicker.size() == 0)
	{
		int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
		int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

		for (int nPkrY = 0; nPkrY < nSiteDivY/*MAX_TRAY_PP_CNT_Y*/; nPkrY++) {
			for (int nPkrX = 0; nPkrX < nSiteDivX/*MAX_TRAY_PP_CNT_X*/; nPkrX++) {
				m_vCmdPicker.push_back(CPoint(nPkrX, nPkrY));
			}
		}
	}

	//[Cmd All Picker]
	for (int nPkrY = 0; nPkrY < MAX_TEST_PP_PKR_CNT_Y; nPkrY++){
		for (int nPkrX = 0; nPkrX < MAX_TEST_PP_PKR_CNT_X; nPkrX++){
			m_vAllPicker.push_back(CPoint(nPkrX, nPkrY));
		}
	}

	//[Cmd Picker Convert to BMAxis]
	int nBMAxisStartPkr = -1;
	if (m_nTestPpIdx == eTestPP_1) {
		nBMAxisStartPkr = (m_nTestPpType == VAT_TESTPP_LOAD) ? eBMAxis_LOAD_TESTPP_1_Picker01 : eBMAxis_UNLOAD_TESTPP_1_Picker01;
	}
	else {
		nBMAxisStartPkr = (m_nTestPpType == VAT_TESTPP_LOAD) ? eBMAxis_LOAD_TESTPP_2_Picker01 : eBMAxis_UNLOAD_TESTPP_2_Picker01;
	}

	std::vector<int> vTemp;
	vTemp.clear();
	for (int i = 0; i < m_vCmdPicker.size(); i++) {
		int nPkrX = m_vCmdPicker[i].x;
		int nPkrY = m_vCmdPicker[i].y;

		if (g_DMCont.m_dmEQP.GN(NDM0_BoardDirection) == eBoardDir_Backward) {
			nPkrX = (MAX_TEST_PP_PKR_CNT_X - 1) - nPkrX;
			nPkrY = (MAX_TEST_PP_PKR_CNT_Y - 1) - nPkrY;
		}

		m_vBMAxis.push_back(nBMAxisStartPkr + (nPkrX + MAX_TRAY_PP_CNT_X * nPkrY));
	}

// 	for (int i = 0; i < m_vCmdPicker.size(); i++) {
// 		if (g_DMCont.m_dmEQP.GN(NDM0_BoardDirection) == eBoardDir_Backward) {
// 			m_vBMAxis.push_back(vTemp[(vTemp.size() - 1) - i]);
// 		}
// 		else {
// 			m_vBMAxis.push_back(vTemp[i]);
// 		}
// 	}



	//==========================================================================
	// 이전 Z Auto Teaching 8 or 4 Picker 사용할 경우 [9/3/2025 dohyeong.kim]
// 	std::vector<int> vTemp; 
// 	vTemp.clear();
// 	for (int nPkrY = 0; nPkrY < MAX_TEST_PP_PKR_CNT_Y; nPkrY++) {
// 		for (int nPkrX = 0; nPkrX < MAX_TEST_PP_PKR_CNT_X; nPkrX++) {
// 			vTemp.push_back(nBMAxisStartPkr + (nPkrX + MAX_TRAY_PP_CNT_X * nPkrY));
// 		}
// 	}
// 
// 	//BmAxis Board Direction Conversion
// 	for (int i = 0; i < nSiteDivX*nSiteDivY; i++) {
// 		if (g_DMCont.m_dmEQP.GN(NDM0_BoardDirection) == eBoardDir_Backward) {			
// 			m_vBMAxis.push_back(vTemp[(vTemp.size() - 1) - i]);	
// 		}
// 		else {
// 			m_vBMAxis.push_back(vTemp[i]);
// 		}
// 	}
	//==========================================================================

	//BMAxis Log
	CString strBMAxis = _T("");
	for (int i = 0; i < m_vBMAxis.size(); i++) {
		CString strTemp = _T("");
		strTemp.Format("%d", m_vBMAxis[i]);
		strBMAxis += strTemp;
		strBMAxis += ", ";
	}
	pTaskTestPP->MakeVatLog(_T("BMAxis=%s"), strBMAxis);
	
	return ERR_NO_ERROR;
}

int CVatTestPPCmd_PickerHeightCal::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

int CVatTestPPCmd_PickerHeightCal::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

int CVatTestPPCmd_PickerHeightCal::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

int CVatTestPPCmd_PickerHeightCal::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

int CVatTestPPCmd_PickerHeightCal::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

BOOL CVatTestPPCmd_PickerHeightCal::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}

BOOL CVatTestPPCmd_PickerHeightCal::State_Executing()
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
				AlarmData.m_fnAddItemFormat("Teset PP %d check motor usable command error", m_nTestPpIdx + 1);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(ERR_TEST_PP_1_MOTOR_ALARMED + m_nTestPpIdx, AlarmData);
				return FALSE;
			}
		}
	}

	switch (m_nStep)
	{
	case height_cal_start:
	{
		m_nStep = height_cal_all_vacuum_on;
// 		if (m_nSpeed == eZSpeed_Fast)
// 			m_nStep = height_cal_move_z_safety;
// 		else //m_nSpeed == eZSpeed_Slow
// 			m_nStep = height_cal_all_vacuum_on;


/*		m_nStep = height_cal_move_z_safety;*/
	}break;
	case height_cal_all_vacuum_on:
	{
		pTaskTestPP->WriteDisplayLog("[EXE] Vacuum Test PP %s All On", pTaskTestPP->GetHandTypeName(m_nTestPpType));
		pTaskTestPP->MakeVatLog("[%s EXEC=%8d] [Test PP %s Vacuum All On] [SRT]", m_szCmdName, m_nStep, pTaskTestPP->GetHandTypeName(m_nTestPpType));

		m_simulCtrl._WaitSeqControl(VAT_SIM_CONTROL_TIME_SHORT);

		//int nErr = pTaskTestPP->do_VacBlow_All(eVacBlwSt_VacOn, m_nTestPpType);
		int nErr = pTaskTestPP->do_VacBlow(eVacBlwSt_VacOn, m_nTestPpType, m_vCmdPicker);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Test PP %s Vacuum FAIL", pTaskTestPP->GetHandTypeName(m_nTestPpType));
			ReportAlarm(((m_nTestPpType == VAT_TESTPP_LOAD) ? ERR_TEST_PP_1_X1_VAC_SEN_ERR : ERR_TEST_PP_1_X2_VAC_SEN_ERR) + m_nTestPpIdx, AlarmData);
			break;
		}

		int nSleep = g_DMCont.m_dmTestPP.GN(NDM4_TM_TestPP1_Vacuum + (m_nTestPpIdx*2));
		if (nSleep > 0) {
			Sleep(nSleep);
		}

		m_nStep = height_cal_vacuum_chk;
	}break;
	//처음 위치가 Vacuum 위치일 경우를 위해, 상단에서 Vacuum Check 후, 진행 하도록 변경.  [6/10/2025 dohyeong.kim]
	case height_cal_vacuum_chk:
	{
		double dCurPos = 0.0;
		int nAxisNo = (m_nTestPpType == VAT_TESTPP_LOAD) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
		pTaskTestPP->GetAxis(nAxisNo)->GetCurrentPos(dCurPos);

		if (m_nSpeed == eZSpeed_Fast)
		{
			//GetCurrentPos 기준 Picker로 변경 생각해 보기
			//Picker Offset 이동 후, Log Write
			pTaskTestPP->WriteDisplayLog("[EXE] [Down Fast] Current Pos : %.3f", dCurPos / 1000.0);

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
				if (pTaskTestPP->ChkVacuum(eOn, m_nTestPpType, m_vCmdPicker[i]) == ERR_NO_ERROR || m_simulCtrl._IsSimul())
				{
					pTaskTestPP->WriteDisplayLog("[EXE] [Down Fast] Find Fast Pos");
					pTaskTestPP->MakeVatLog("[%s EXEC=%8d] [Z move Done] [Fast Pos] [END]", m_szCmdName, m_nStep);
					m_nStep = height_cal_save_fast_height_pos;
					break;
				}
			}

			if (m_nStep == height_cal_save_fast_height_pos)
				break;

			m_nStep = height_cal_picker_down_fast;
		}
		else
		{
			//Picker Offset 이동 후, Log Write
			pTaskTestPP->WriteDisplayLog("[EXE] [Down Slow] Current Pos : %.3f", dCurPos / 1000.0);

			//Simulation
			if (m_simulCtrl._IsRemainSeqCycle())
			{
				m_nStep = height_cal_picker_down_slow;
				break;
			}
			m_simulCtrl._ClearParam();

			int nAxisNo = (m_nTestPpType == VAT_TESTPP_LOAD) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
			OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo);

	
			CBbxmnpClient::_ePICK_MOTOR eMotor = CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1;
			if (m_nTestPpIdx == eTestPP_1) {
				eMotor = (m_nTestPpType == VAT_TESTPP_LOAD) ? CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1 : CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1;
			}
			else { //m_nTestPpIdx == eTestPP_2
				eMotor = (m_nTestPpType == VAT_TESTPP_LOAD) ? CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2 : CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2;
			}

			m_nChkAllVacuum = 0;

			for (int i = 0; i < m_vCmdPicker.size()/*VAT_PICKER_MAX*/; i++)
			{
				if (m_bFindPos[i] != TRUE) {
					if (pTaskTestPP->ChkVacuum(eOn, m_nTestPpType, m_vCmdPicker[i]) == ERR_NO_ERROR || m_simulCtrl._IsSimul())
					{
						m_nChkAllVacuum++;
						m_bFindPos[i] = TRUE;
						m_dPickPos[i] = g_pBbxmnpClient->GetActualPos(eMotor, (_eBbxmnpAxis)(m_vBMAxis[i]));

						//Vacuum 감지 된 Picker 멈춤 & 탈조 현상 방지.
						//if (g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
						//	pMotor->SetSlowStopEach((_eBbxmnpAxis)(m_vBMAxis[i]));
					}
				}
				else {
					//이미 찾은 Picker
					m_nChkAllVacuum++;
				}
			}

			//if (pTaskTestPP->GetVacuum_All(m_nTestPpType, eOn) == ERR_NO_ERROR)
			if (m_nChkAllVacuum == m_vCmdPicker.size())
			{
				pTaskTestPP->WriteDisplayLog("[EXE] [Down Slow] Find Slow Pos");
				pTaskTestPP->MakeVatLog("[%s EXEC=%8d] [Z move Done] [Slow Pos][End]", m_szCmdName, m_nStep);
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

		pTaskTestPP->MakeVatLog("[%s EXEC=%8d] [Z%d %s move] [Picker Down Fast] [%.3f um] [STR]", m_szCmdName, m_nStep, m_nTestPpType + 1, pTaskTestPP->GetLocName(m_nStage), (m_dTargetPosZ + move_offset_um));

		int nAxisNo = (m_nTestPpType == VAT_TESTPP_LOAD) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;

		int nErr = pTaskTestPP->do_VATMovePos_Z(nAxisNo, m_vCmdPicker, m_dTargetPosZ, move_offset_um);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Test PP %s Move Fast Z Down Fail", pTaskTestPP->GetHandTypeName(m_nTestPpType));
			OneAxis* pMotor = pTaskTestPP->GetAxis(static_cast<CTaskTestPP::eMotor>(nAxisNo));
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
		pTaskTestPP->MakeVatLog("[%s EXEC=%8d] [Z%d %s move] [Picker Down Slow] [%.3f um] [STR]", m_szCmdName, m_nStep, m_nTestPpType + 1, pTaskTestPP->GetLocName(m_nStage), (m_dTargetPosZ + move_offset_um));

		int nAxisNo = (m_nTestPpType == VAT_TESTPP_LOAD) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;

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

			nErr = pTaskTestPP->do_VATMovePos_Z(nAxisNo, vCmdEachPicker, m_dTargetPosZ, move_offset_um);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Tray PP %s Move Slow Z Down Fail", pTaskTestPP->GetHandTypeName(m_nTestPpType));
				OneAxis* pMotor = pTaskTestPP->GetAxis(static_cast<CTaskTestPP::eMotor>(nAxisNo));
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
		}

		if (nErr != ERR_NO_ERROR)
			break;

		m_simulCtrl._WaitSeqControl(VAT_SIM_CONTROL_TIME_SHORT);

		m_tcTimeout.SetCheckTime(TIME_OUT_SHORT);
		m_tcTimeout.ResetStartTime();
		m_nStep = height_cal_picker_down_chk;
	}break;
	case height_cal_picker_down_chk:
	{
		int nAxisNo = (m_nTestPpType == VAT_TESTPP_LOAD) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
		OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo);

		if (m_tcTimeout.CheckOverTime()) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
			break;
		}

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
		g_VATCommonData.SetFirstPickOffset(m_nHand, dFirstPos);

		pTaskTestPP->MakeVatLog("[%s EXEC=%8d] [Z%d Save Height ] [Fast Pos] [%.3f um]", m_szCmdName, m_nStep, m_nTestPpType + 1, dFirstPos);

		m_nStep = height_cal_all_picker_1mm_up;
	}break;
	case height_cal_save_slow_height_pos_z_auto_focus:
	{
		pTaskTestPP->WriteDisplayLog("===============<STD VACUUM POS>===================");

		if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
			double dEncoder = 100.0;
			for (int i = 0; i < m_vCmdPicker.size(); i++) {
				m_dPickPos[i] = dEncoder;
				dEncoder += 100.0;
			}
		}

		double dStdPickerPos = m_dPickPos[0]; //Std Picker Vacuum Pos

		//Save
		g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_VacuumPos[m_nHand][VAT_Z] = dStdPickerPos;
		pTaskTestPP->WriteDisplayLog("[EXE] [Std Picker] Vacuum Pos : %.3f um", dStdPickerPos);
		pTaskTestPP->MakeVatLog("[%s EXEC=%8d] [Z Save Height] [Slow Pos] [Std Picker] [Vacuum Pos : %.3f um]", m_szCmdName, m_nStep, dStdPickerPos);

		m_nStep = height_cal_all_picker_1mm_up;

	}break;
	case height_cal_save_slow_height_pos_z_auto_teaching:
	{
		pTaskTestPP->WriteDisplayLog("===============<VACUUM POS>===================");

		if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
			double dEncoder = 100.0;
			for (int i = 0; i < m_vCmdPicker.size(); i++) {
				m_dPickPos[i] = dEncoder;
				dEncoder += 100.0;
			}
		}

		//1. Picker Vacuum Pos 저장.
		for (int nPicker = VAT_PICKER_01; nPicker < m_vCmdPicker.size(); nPicker++) {
			g_VATCommonData.m_stVATZTeachInfoSeq.stPickerVacPos.setPickerVacPos(m_nHand, m_nStage, nPicker, m_dPickPos[nPicker]);

			pTaskTestPP->WriteDisplayLog("[EXE] [Picker %d] Vacuum Pos : %.3f um", nPicker + 1, m_dPickPos[nPicker]);
			pTaskTestPP->MakeVatLog("[%s EXEC=%8d] [Z Save Height] [Slow Pos] [Picker %d] [Vacuum Pos : %.3f um]", m_szCmdName, m_nStep, nPicker + 1, m_dPickPos[nPicker]);
		}

		pTaskTestPP->WriteDisplayLog("=================<GAP>=====================");

		//2. Picker 간 Gap 저장.
		double pick_base_pos = m_dPickPos[VAT_PICKER_01];
		for (int nPicker = VAT_PICKER_01; nPicker < m_vCmdPicker.size(); nPicker++) {
			double dPickerGap = m_dPickPos[nPicker] - pick_base_pos;
			g_VATCommonData.m_stVATZTeachInfoSeq.stPickerGap.setPickerGap(m_nHand, m_nStage, nPicker, dPickerGap);

			pTaskTestPP->WriteDisplayLog("[EXE] [Picker %d] Gap : %.3f um", nPicker + 1, dPickerGap);
			pTaskTestPP->MakeVatLog("[%s EXEC=%8d] [Z Save Height] [Slow Pos] [Picker %d] [Gap : %.3f um]", m_szCmdName, m_nStep, nPicker + 1, dPickerGap);
		}

		m_nStep = height_cal_all_picker_1mm_up;
	}break;
	case height_cal_all_picker_1mm_up:
	{
		//1mm 높은 곳에서 blow
		move_offset_um -= 1000.0; //1mm 

		pTaskTestPP->MakeVatLog("[%s EXEC=%8d] [Z%d move 1mm Up] [%.3f um] [SRT]", m_szCmdName, m_nStep, m_nTestPpType + 1, pTaskTestPP->GetLocName(m_nStage), move_offset_um);
		int nAxisNo = (m_nTestPpType == VAT_TESTPP_LOAD) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
		int nErr = pTaskTestPP->do_VATMovePos_Z(nAxisNo, m_vCmdPicker, m_dTargetPosZ, move_offset_um);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Test PP%d Move Z 1mm Up Fail", m_nTestPpIdx + 1);
			OneAxis* pMotor = pTaskTestPP->GetAxis(static_cast<CTaskTestPP::eMotor>(nAxisNo));
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
		int nAxisNo = (m_nTestPpType == VAT_TESTPP_LOAD) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
		OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo);

		if (m_tcTimeout.CheckOverTime() == TRUE) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
			break;
		}

	
		int nAxisDone = pMotor->AxisDone();
		if (nAxisDone == ERR_NO_ERROR)
		{
			m_nStep = height_cal_vacuum_off;
		}	
	}break;
	case height_cal_vacuum_off:
	{
		int nErr = pTaskTestPP->do_VacBlow_All(eVacBlwSt_BlwOn, m_nTestPpType); //[test 필요]eVacBlwSt_BlwOn, eVacBlwSt_Off
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Test PP Blow FAIL");
			ReportAlarm(ERR_TRAY_PP_2_VAC_SEN_ERR, AlarmData);
			break;
		}

		int nSleep = g_DMCont.m_dmTestPP.GN(NDM4_TM_TestPP1_Blow + (m_nTestPpIdx * 2));
		if (nSleep > 0) {
			Sleep(nSleep);
		}

		nErr = pTaskTestPP->do_VacBlow_All(eVacBlwSt_Off, m_nTestPpType);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Test PP Vac/Blow Off FAIL");
			ReportAlarm(ERR_TRAY_PP_2_VAC_SEN_ERR, AlarmData);
			break;
		}

		m_nStep = (m_nSpeed == eZSpeed_Fast) ? height_cal_end : height_cal_all_picker_up;
	}break;
	case height_cal_all_picker_up:
	{
		pTaskTestPP->MakeVatLog("[%s EXEC=%8d] [Move Safety and Picker All Up] [SRT]", m_szCmdName, m_nStep);

		int nAxisNo = (m_nTestPpType == VAT_TESTPP_LOAD) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
		int nLocIdx = (VAT_IDX_STATION_1 <= m_nStage && m_nStage <= VAT_IDX_CLEAN_TBL) ? m_nStage : LOC_IDX_TEST_PP_HOME;
		double dSafetyPosZ = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPpIdx), nLocIdx, (m_nTestPpType == VAT_TESTPP_LOAD) ? POS_IDX_TEST_PP_Z1_SAFETY : POS_IDX_TEST_PP_Z2_SAFETY);
		int nErr = pTaskTestPP->do_VATMovePos_Z(nAxisNo, m_vCmdPicker, dSafetyPosZ);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Test PP%d Move Z Up Fail", m_nTestPpIdx);
			OneAxis* pMotor = pTaskTestPP->GetAxis(static_cast<CTaskTestPP::eMotor>(nAxisNo));
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
		int nAxisNo = (m_nTestPpType == VAT_TESTPP_LOAD) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
		OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo);

		m_vErrPicker.clear();
		int nAxisDone = pMotor->AxisDone(m_vCmdPicker, m_vErrPicker);
		int nErrSen = pTaskTestPP->Chk_Z_Safety_Sen_Check_Type(m_vAllPicker, m_nTestPpType);
		if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR)
		{
			pTaskTestPP->MakeVatLog("[%s EXEC=%8d] [Move Safety and Picker All Up] [END]", m_szCmdName, m_nStep);
			m_nStep = height_cal_end;
		}
		else {
			std::vector<CPoint> vErrPkr; vErrPkr.clear();
			CString sAlarmData = _T("");
			int nErr = pTaskTestPP->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, pTaskTestPP->GetSafetyFailPicker(m_nTestPpType, ePickerSafetySensorCheckType::eSafetySensorCheck), m_nTestPpType, vErrPkr, sAlarmData);

			if (m_tcTimeout.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData, m_vErrPicker);
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

BOOL CVatTestPPCmd_PickerHeightCal::State_Pause()
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
	case 0:
	{
		pTaskTestPP->WriteDisplayLog("[PAUSE]==============================");

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

BOOL CVatTestPPCmd_PickerHeightCal::State_Recovery()
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

BOOL CVatTestPPCmd_PickerHeightCal::State_NormalComplete()
{
	//VAT LED 끄기
	return CBaseSeqCmd::State_NormalComplete();
}

BOOL CVatTestPPCmd_PickerHeightCal::State_Alarm()
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
	case 0:
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

BOOL CVatTestPPCmd_PickerHeightCal::State_AlarmComplete()
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

BOOL CVatTestPPCmd_PickerHeightCal::State_Aborting()
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
	case 0:
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

BOOL CVatTestPPCmd_PickerHeightCal::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}

CString CVatTestPPCmd_PickerHeightCal::act_string(int act)
{
	BEGIN_CASE_TO_STR(act);
	CASE_TO_STR(height_cal_start);
// 	CASE_TO_STR(height_cal_move_z_safety);
// 	CASE_TO_STR(height_cal_move_z_safety_chk);
// 	CASE_TO_STR(height_cal_move_x1_x2_safety);
// 	CASE_TO_STR(height_cal_move_x1_x2_safety_chk);
// 	CASE_TO_STR(height_cal_move_y_px_py_target_pos);
// 	CASE_TO_STR(height_cal_move_y_px_py_target_pos_chk);
// 	CASE_TO_STR(height_cal_move_x_target_pos);
// 	CASE_TO_STR(height_cal_move_x_target_pos_chk);
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