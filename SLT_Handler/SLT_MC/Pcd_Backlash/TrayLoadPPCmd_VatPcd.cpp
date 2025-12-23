#include "StdAfx.h"
#include "TrayLoadPPCmd_VatPcd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//CRamTeaching  m_Teaching_Data_;   // decrease by PAGESIZE Error 전역을 써야함.

/* simulation code */
#define is_simulation() ((g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) ? TRUE : FALSE)
#define TRAY_LOADPP_PCD_SPEED (15)
/* simulation code */
//==============================================================================
//
//==============================================================================
CTrayLoadPPCmd_VatPcd::CTrayLoadPPCmd_VatPcd(void)
{
	m_nCmdId = eSeqCmd_TrayPP_VatAtWCamTch;
	m_nRetryCount = 0;
	sprintf_s(m_szCmdName, sizeof(m_szCmdName), "CTrayLoadPPCmd_VatPcd");

	char szDrvFile[512] = { 0, };
	char szCokName[512] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));

	char szCurDir[512] = { 0, };
	GetCurrentDirectory(sizeof(szCurDir), szCurDir);
	sprintf_s(szDrvFile, sizeof(szDrvFile), "%s\\sys_data\\%s", szCurDir, SZ_DRIVER_FILE_NAME);
// 
// 	g_DMCont.m_Teach.Load();
// 	m_Teaching_Data_.Initialize(szDrvFile, szCokName);
// 	m_Teaching_Data_ = g_DMCont.m_Teach;
// 
 	m_dTrayPP_Z_Safety = 0;
// 	m_dTrayPP_Z_Safety = m_Teaching_Data_.GetTeachPos(40, 9, 4);
	
	/*param default*/
	m_nChkCount =0;
	m_nAxisNum = 0;
	m_nDistance = 0;
}


//==============================================================================
//
//==============================================================================
CTrayLoadPPCmd_VatPcd::~CTrayLoadPPCmd_VatPcd(void){}

//==============================================================================
//
//==============================================================================
int CTrayLoadPPCmd_VatPcd::CheckParam()
{
	double dTimeout = 60.0;
	m_tcTimeout.SetCheckTime(dTimeout);
	m_tcTimeout.StartTimer();
	m_nLast_Executing_Step = exe_start;
	m_nChkCount = 0;

	CTaskTrayLoadPP* pTaskTrayLoadPP = (CTaskTrayLoadPP*)m_pReceive;

	if (is_simulation()) {
		m_Cmd_PassRangeUm.x = 200.0;
		m_Cmd_PassRangeUm.y = 200.0;
	}
	else {
		OneAxis* pMotorX = pTaskTrayLoadPP->GetAxis(CTaskTrayLoadPP::eAxisX);
		m_Cmd_PassRangeUm.x = pMotorX->GetScaleFactor();
	}

	return ERR_NO_ERROR;
}


//==============================================================================
//
//==============================================================================
int  CTrayLoadPPCmd_VatPcd::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTrayLoadPPCmd_VatPcd::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTrayLoadPPCmd_VatPcd::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTrayLoadPPCmd_VatPcd::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTrayLoadPPCmd_VatPcd::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CTrayLoadPPCmd_VatPcd::State_Setup()
{	
	return CBaseSeqCmd::State_Setup();
}

//==============================================================================
//
//==============================================================================
BOOL CTrayLoadPPCmd_VatPcd::State_Executing()
{
	int nErr = 0;

	// check alarm
	CTaskTrayLoadPP* pTaskTrayLoadPP = (CTaskTrayLoadPP*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}

	m_nStep_Pre = m_nStep;

	//by kmk [20190228] Pitch는 PCD & Backlash에서 사용하지 않음.
	int i=0;
	for( i=0; i< CTaskTrayBasePP::eAxisMax; i++){
		OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)i);
		if (pMotor == nullptr)
			continue;

		if( pMotor->ChkMotorUsable() != ERR_NO_ERROR ){
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray Load PP check motor usable Cmd Error");
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(ERR_TRAY_PP_1_MOTOR_ALARMED, AlarmData);
			return FALSE;
		}
	}

	//feeder
	OneAxis* pMotorY = g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eMotor::eAxisY);
	if (pMotorY->ChkMotorUsable() != ERR_NO_ERROR) {
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Tray feeder check motor usable Cmd Error");
		AlarmData.m_fnAddItemFormat("Axis=%s", pMotorY->GetAxisName());
		ReportAlarm(ERR_FEEDER_MOTOR_MOVE_FAIL, AlarmData);
		return FALSE;
	}


	OneAxis* pMotorX = pTaskTrayLoadPP->GetAxis(CTaskTrayLoadPP::eAxisX);
	OneAxis* pMotorZ  = pTaskTrayLoadPP->GetAxis(CTaskTrayLoadPP::eAxisZ);
	OneAxis* pMotorPX = pTaskTrayLoadPP->GetAxis(CTaskTrayLoadPP::eAxisPitchX);
	OneAxis* pMotorPY = pTaskTrayLoadPP->GetAxis(CTaskTrayLoadPP::eAxisPitchY);

	switch( m_nStep )
	{		
	case exe_start:
		{
			if(m_nAxisNum != eAxis_X && m_nAxisNum !=eAxis_Y) nErr++;

			if(m_nAxisNum == eAxis_X){
				m_StartPos = g_VatPcd.trayloadpp.m_PcdStartXPos_XYZ;
			}
// 			else{
// 				m_StartPos = g_VatPcd.traypp.m_PcdStartYPos_XYZ;
// 			}

			m_TargetPos = m_StartPos;

			if(m_nAxisNum == eAxis_X){
				m_TargetPos.x += m_nDistance;
			}else if(m_nAxisNum == eAxis_Y){
				m_TargetPos.y += m_nDistance;
			}

			if( nErr ){
				SimpleAlarm( ERR_LOT_CMD_PARAMETER_IS_INVALID );
				break;
			} else {
				m_nRetryCount = 0;
				m_StartPos_Offset  = CPointF(0,0,0);
				m_TargetPos_Offset = CPointF(0,0,0);
				NextStep( exe_pcd_loop_entry );
			}
		}break;

	case exe_pcd_loop_entry:
		{
			NextStep(exe_hand_z_safety_move);
		}break;
	case exe_hand_z_safety_move:
		{
			pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [move safety and picker all up] [SRT]", m_szCmdName, m_nStep );
			int nErr = pTaskTrayLoadPP->do_MovePos(eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY, Pocket, m_vSafetyAllPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}

			m_tcTimeout.SetCheckTime(TIME_OUT_SHORT);
			m_tcTimeout.StartTimer();
			NextStepTimer( exe_hand_z_safety_move_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_z_safety_move_check:
		{
			std::vector<CPoint> vPkr;
			pTaskTrayLoadPP->MakeAllPicker(vPkr);
			int nErrSen = pTaskTrayLoadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);
			m_vErrPicker.clear();
			int nAxisDone = pMotorZ->AxisDone(vPkr, m_vErrPicker);
			if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR)
			{
				pTaskTrayLoadPP->MakeLog("[%s EXEC=%8d] [move safety and picker all up] [END]", m_szCmdName, m_nStep);
				m_nStep = exe_hand_xy_pitch_safety_move;
				break;
			}
			else {
				std::vector<CPoint> vErrPkr; vErrPkr.clear();
				CString sAlarmData = _T("");
				int nErr = pTaskTrayLoadPP->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, pTaskTrayLoadPP->GetSafetyFailPicker(ePickerSafetySensorCheckType::eSafetySensorCheck), vErrPkr, sAlarmData);

				if (m_tcTimeout.CheckOverTime() == TRUE) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("%s", sAlarmData);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
					ReportAlarm(nErr, AlarmData, vErrPkr);
					break;
				}
			}
		}break;
	case exe_hand_xy_pitch_safety_move:
	{
		pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [move pitch x,y safety pos] [SRT]", m_szCmdName, m_nStep);
		int nAxisNo[2] = { CTaskTrayLoadPP::eAxisPitchX ,CTaskTrayLoadPP::eAxisPitchY};
		for (int i = 0; i < _countof(nAxisNo); i++)
		{
			int nErr = pTaskTrayLoadPP->do_MovePos(eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY, Pocket, m_vSafetyAllPicker, CTaskTrayBasePP::eZPOS_SAFETY, (CTaskTrayBasePP::eMotor)nAxisNo[i], TRAY_LOADPP_PCD_SPEED);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
		}
	
		m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
		m_tcTimeout.StartTimer();
		NextStepTimer(exe_hand_xy_pitch_safety_move_check, MOTOR_TIME_OUT_LONG);
	}break;
	case exe_hand_xy_pitch_safety_move_check:
	{
		int nChkComp = 0;
		int nAxisNo[2] = { CTaskTrayLoadPP::eAxisPitchX ,CTaskTrayLoadPP::eAxisPitchY };
	
		for (int i = 0; i < _countof(nAxisNo); i++) {
			OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				nChkComp++;
			}
			else {
				if (m_tcTimeout.CheckOverTime() == TRUE) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TRAY_PP_1_MOTOR_MOVE_TIMEOUT, AlarmData);
					break;
				}
			}
		}



		if (nChkComp == _countof(nAxisNo)) {
			pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [move pitch x,y safety pos] [END]", m_szCmdName, m_nStep);
			m_tcTimeoutPicker.SetCheckTime(TIME_OUT_SHORT);
			m_tcTimeoutPicker.StartTimer();
			m_nStep = exe_hand_xy_start_position_move_100mm;
			break;
		}
	}break;
	case exe_hand_xy_start_position_move_100mm:
		{
			int nAxisX[1] = {CTaskTrayLoadPP::eAxisX};
			int nAxisY[1] = { CTaskTrayFeeder::eAxisY};
			double dTPosX[1] = {0.0,}, dTPosY[1] = { 0.0, };

			dTPosX[0] = m_StartPos.x;
			dTPosX[0] += m_StartPos_Offset.x;

			dTPosY[0] = m_StartPos.y;
			dTPosY[0] += m_StartPos_Offset.y;

			double dTemp = m_StartPos.y;

			pTaskTrayLoadPP->MakeVatPreparePos(dTPosX[CTaskTrayLoadPP::eAxisX], dTPosY[CTaskTrayFeeder::eAxisY], Move_100mm_Offset);

			pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [x move_100mm start position] [SRT]", m_szCmdName, m_nStep);
			// Hand X
			for(int i=0; i<_countof(nAxisX); i++){
				int nErr = pTaskTrayLoadPP->do_MovePos_Pcd_backlash(vPicker, dTPosX[i], (CTaskTrayBasePP::eMotor)nAxisX[i], TRAY_LOADPP_PCD_SPEED);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisX[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}
			// Feeder Y
			pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [feeder y move_100mm start position] [SRT]", m_szCmdName, m_nStep);
			for (int i = 0; i < _countof(nAxisY); i++) {
				int nErr = g_TaskTrayFeeder.do_MovePos_Pcd_backlash(dTPosY[i], (CTaskTrayFeeder::eMotor)nAxisY[i], TRAY_LOADPP_PCD_SPEED, 0);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					OneAxis* pMotor = g_TaskTrayFeeder.GetAxis((CTaskTrayFeeder::eMotor)nAxisY[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}

			NextStepTimer(exe_hand_xy_start_position_move_100mm_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_xy_start_position_move_100mm_check:
		{
			OneAxis* pMotor[2] = { pMotorX,pMotorY};

			int nChkComp  = 0;
			for(int i=0; i<_countof(pMotor); i++){
				int nAxisDone = pMotor[i]->AxisDone();
				if( nAxisDone == ERR_NO_ERROR ){
					nChkComp++;
				}
				else {
					if (m_tcTimeout.CheckOverTime() == TRUE) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
						ReportAlarm(ERR_TRAY_PP_1_MOTOR_MOVE_TIMEOUT, AlarmData);
						break;
					}
				}
			}
			if( nChkComp == _countof(pMotor)){
				pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [x,y move_100mm start position] [END]", m_szCmdName, m_nStep);

				m_nRetryCount = 0;
				NextStep(exe_hand_xy_start_position_move);
			}
		}break;
	case exe_hand_xy_start_position_move:
		{
			int nAxisX[1] = {CTaskTrayLoadPP::eAxisX};

			double dTPosX[1] = {0.0,};
			dTPosX[0] = m_StartPos.x;
			dTPosX[0] += m_StartPos_Offset.x;

			pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [x move start position] [SRT]", m_szCmdName, m_nStep);
			for(int i=0; i<_countof(nAxisX); i++){
				int nErr = pTaskTrayLoadPP->do_MovePos_Pcd_backlash(vPicker, dTPosX[i], (CTaskTrayBasePP::eMotor)nAxisX[i], TRAY_LOADPP_PCD_SPEED);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisX[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}

			int nAxisY[1] = { CTaskTrayFeeder::eAxisY };

			double dTPosY[1] = { 0.0, };
			dTPosY[0] = m_StartPos.y;
			dTPosY[0] += m_StartPos_Offset.y;

			pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [feeder y Start Position move] [SRT]", m_szCmdName, m_nStep);
			for (int i = 0; i < _countof(nAxisY); i++) {
				int nErr = g_TaskTrayFeeder.do_MovePos_Pcd_backlash(dTPosY[i], (CTaskTrayFeeder::eMotor)nAxisY[i], TRAY_LOADPP_PCD_SPEED, 0);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					OneAxis* pMotor = g_TaskTrayFeeder.GetAxis((CTaskTrayFeeder::eMotor)nAxisY[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}
			NextStepTimer(exe_hand_xy_start_position_move_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_xy_start_position_move_check:
		{
			OneAxis* pMotor[2] = { pMotorX,pMotorY };

			int nChkComp = 0;
			for (int i = 0; i < _countof(pMotor); i++) {
				int nAxisDone = pMotor[i]->AxisDone();
				if (nAxisDone == ERR_NO_ERROR) {
					nChkComp++;
				}
				else {
					if (m_tcTimeout.CheckOverTime() == TRUE) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
						ReportAlarm(ERR_TRAY_PP_1_MOTOR_MOVE_TIMEOUT, AlarmData);
						break;
					}
				}
			}
			if (nChkComp == _countof(pMotor)) {
				pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [x,y move start position] [END]", m_szCmdName, m_nStep);

				m_nRetryCount = 0;
				NextStep(exe_hand_z_start_position_move);
			}
		}break;
	case exe_hand_z_start_position_move:
		{
			double dTPos = m_StartPos.z;
			pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [z move to start position] [SRT]", m_szCmdName, m_nStep);

			int nErr = pTaskTrayLoadPP->do_MovePos_Pcd_backlash(vPicker, dTPos, CTaskTrayBasePP::eAxisZ, TRAY_LOADPP_PCD_SPEED);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}

			NextStepTimer( exe_hand_z_start_position_move_check, 30.0 );
		}break;

	case exe_hand_z_start_position_move_check:
		{
			if( m_tcTimeout.CheckOverTime() == TRUE ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				ReportAlarm(ERR_TRAY_PP_1_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData);
				break;
			}
			int nAxisDone = pMotorZ->AxisDone();
			if( nAxisDone == ERR_NO_ERROR ){
				pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [z move to start position] [END]", m_szCmdName, m_nStep);
				NextStep( exe_visn_recon_start_position );
			}
		}break;
	case exe_visn_recon_start_position:
		{
			pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [PCD start position measurement] [SRT]", m_szCmdName, m_nStep);
			if( is_simulation() ){
				NextStepTimer( exe_visn_recon_start_position_check, VISION_TIME_OUT );
				break;
			}

			Sleep(1000); // Measure Delay.
			g_pVisionTestHand_1->SendMeasure_PcdBacklash(VISION_MEASURE_VAT_LD_UPPER, VAT_HAND_LOAD_TRAYPP );
			NextStepTimer( exe_visn_recon_start_position_check, VISION_TIME_OUT );
		}break;
	case exe_visn_recon_start_position_check:
		{
			ST_PCD_BACKLASH_RCV_DATA vRcvData;
			/* CheckMeasureResult() 에서 alarm check 처리 */
			if( CheckMeasureResult(vRcvData) == TRUE ){
				pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [Start Position measurement dx=%.3f um, dy=%.3f um] [END]", m_szCmdName, m_nStep, vRcvData.dX,vRcvData.dY);

				m_StartPos_Offset.x += vRcvData.dX;
				m_StartPos_Offset.y += vRcvData.dY;
				
				if (fabs(vRcvData.dX) < m_Cmd_PassRangeUm.x && fabs(vRcvData.dY) < m_Cmd_PassRangeUm.y) {
					NextStep( exe_hand_z_start_position_safety_move );	
					break;
				}
				NextStep( exe_pcd_loop_entry );	
			}
		}break;
	case exe_hand_z_start_position_safety_move:
		{
			pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [move safety and picker all up] [SRT]", m_szCmdName, m_nStep );
			int nErr = pTaskTrayLoadPP->do_MovePos(eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY, Pocket, m_vSafetyAllPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}

			NextStepTimer( exe_hand_z_start_position_safety_move_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_z_start_position_safety_move_check:
		{
			if( m_tcTimeout.CheckOverTime() == TRUE ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				ReportAlarm(ERR_TRAY_PP_1_MOTOR_MOVE_TIMEOUT, AlarmData);
				break;
			}
			int nAxisDone = pMotorZ->AxisDone();
			if( nAxisDone == ERR_NO_ERROR){
				NextStepTimer( exe_hand_z_start_position_safety_picker_check, TIME_OUT_SHORT );
			}
		}break;
	case exe_hand_z_start_position_safety_picker_check:
		{
			int nMaxRetry = MAX_CYLINDER_RETRY_COUNT;
			if( m_tcTimeout.CheckOverTime() == TRUE ){
				if( m_nRetryCount > nMaxRetry ){
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Tray PP Picker Up Error");
					AlarmData.m_fnAddItemFormat("Setting time = %.2f, RetryCnt = %d, ", m_tcTimeout.GetCheckTime(), nMaxRetry);
					ReportAlarm(ERR_TRAY_PP_1_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData);
					break;
				}else{
					m_nRetryCount++;
					NextStep( exe_hand_z_start_position_safety_move );
				}
			}
			int nChk = pTaskTrayLoadPP->Chk_Z_Safety_Sen_All();
			if( nChk == ERR_NO_ERROR ){
				pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [move safety and picker all up] [END]", m_szCmdName, m_nStep );
				NextStep(exe_hand_xy_target_position_move_100mm);
			}
		}break;
	case exe_hand_xy_target_position_move_100mm:
		{
			int nAxisX[1] = {CTaskTrayLoadPP::eAxisX};
			int nAxisY[1] = { CTaskTrayFeeder::eAxisY };

			double dTPosX[1] = {0.0,};
			dTPosX[0] = m_TargetPos.x;
			dTPosX[0] += m_TargetPos_Offset.x;

			double dTPosY[1] = { 0.0, };
			dTPosY[0] = m_TargetPos.y;
			dTPosY[0] += m_TargetPos_Offset.y;

			pTaskTrayLoadPP->MakeVatPreparePos(dTPosX[CTaskTrayLoadPP::eAxisX], dTPosY[CTaskTrayFeeder::eAxisY], Move_100mm_Offset);

			// Load Tray Hand x
			pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [x move_100mm target position] [SRT]", m_szCmdName, m_nStep);
			for(int i=0; i<_countof(nAxisX); i++){
				int nErr = pTaskTrayLoadPP->do_MovePos_Pcd_backlash(vPicker, dTPosX[i], (CTaskTrayBasePP::eMotor)nAxisX[i], TRAY_LOADPP_PCD_SPEED);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisX[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}
			//feeder y
			pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [feeder y move_100mm target Position move] [SRT]", m_szCmdName, m_nStep);
			for (int i = 0; i < _countof(nAxisY); i++) {
				int nErr = g_TaskTrayFeeder.do_MovePos_Pcd_backlash(dTPosY[i], (CTaskTrayFeeder::eMotor)nAxisY[i], TRAY_LOADPP_PCD_SPEED, 0);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					OneAxis* pMotor = g_TaskTrayFeeder.GetAxis((CTaskTrayFeeder::eMotor)nAxisY[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}

			NextStepTimer( exe_hand_xy_target_position_move_100mm_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_xy_target_position_move_100mm_check:
		{
			OneAxis* pMotor[2] = { pMotorX,pMotorY };

			int nChkComp = 0;
			for (int i = 0; i < _countof(pMotor); i++) {
				int nAxisDone = pMotor[i]->AxisDone();
				if (nAxisDone == ERR_NO_ERROR) {
					nChkComp++;
				}
				else {
					if (m_tcTimeout.CheckOverTime() == TRUE) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
						ReportAlarm(ERR_TRAY_PP_1_MOTOR_MOVE_TIMEOUT, AlarmData);
						break;
					}
				}
			}
			if (nChkComp == _countof(pMotor)) {
				pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [x,y move_100mm target position] [END]", m_szCmdName, m_nStep);

				m_nRetryCount = 0;
				NextStep(exe_hand_xy_target_position_move);
			}
		}break;
	case exe_hand_xy_target_position_move:
		{
			int nAxisX[1] = {CTaskTrayLoadPP::eAxisX};
			int nAxisY[1] = { CTaskTrayFeeder::eAxisY };

			double dTPosX[1] = {0.0,};
			dTPosX[0] = m_TargetPos.x;
			dTPosX[0] += m_TargetPos_Offset.x;

			double dTPosY[1] = { 0.0, };
			dTPosY[0] = m_TargetPos.y;
			dTPosY[0] += m_TargetPos_Offset.y;


			pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [x move target position] [SRT]", m_szCmdName, m_nStep);
			for(int i=0; i<_countof(nAxisX); i++){
				int nErr = pTaskTrayLoadPP->do_MovePos_Pcd_backlash(vPicker, dTPosX[i], (CTaskTrayBasePP::eMotor)nAxisX[i], TRAY_LOADPP_PCD_SPEED);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisX[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}

			pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [feeder y target Position move] [SRT]", m_szCmdName, m_nStep);
			for (int i = 0; i < _countof(nAxisY); i++) {
				int nErr = g_TaskTrayFeeder.do_MovePos_Pcd_backlash(dTPosY[i], (CTaskTrayFeeder::eMotor)nAxisY[i], TRAY_LOADPP_PCD_SPEED, 0);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					OneAxis* pMotor = g_TaskTrayFeeder.GetAxis((CTaskTrayFeeder::eMotor)nAxisY[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}

			NextStepTimer( exe_hand_xy_target_position_move_check, MOTOR_TIME_OUT_LONG );
		}break;

	case exe_hand_xy_target_position_move_check:
		{
			OneAxis* pMotor[2] = { pMotorX,pMotorY };

			int nChkComp = 0;
			for (int i = 0; i < _countof(pMotor); i++) {
				int nAxisDone = pMotor[i]->AxisDone();
				if (nAxisDone == ERR_NO_ERROR) {
					nChkComp++;
				}
				else {
					if (m_tcTimeout.CheckOverTime() == TRUE) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
						ReportAlarm(ERR_TRAY_PP_1_MOTOR_MOVE_TIMEOUT, AlarmData);
						break;
					}
				}
			}
			if (nChkComp == _countof(pMotor)) {
				pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [x,y move target position] [END]", m_szCmdName, m_nStep);

				m_nRetryCount = 0;
				NextStep(exe_hand_z_target_position_move);
			}
		}break;
	case exe_hand_z_target_position_move:
		{
			double dTPos = m_StartPos.z;
			pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [z move to target position] [SRT]", m_szCmdName, m_nStep);

			int nErr = pTaskTrayLoadPP->do_MovePos_Pcd_backlash(vPicker, dTPos, CTaskTrayBasePP::eAxisZ, TRAY_LOADPP_PCD_SPEED);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			NextStepTimer( exe_hand_z_target_position_move_check, 30.0 );
		}break;
	case exe_hand_z_target_position_move_check:
		{
			if( m_tcTimeout.CheckOverTime() == TRUE ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				ReportAlarm(ERR_TRAY_PP_1_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData);
				break;
			}
			int nAxisDone = pMotorZ->AxisDone();
			if( nAxisDone == ERR_NO_ERROR ){
				pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [z move to target position] [END]", m_szCmdName, m_nStep);
				NextStep( exe_visn_recon_target_position );
			}
		}break;
	case exe_visn_recon_target_position:
		{
			pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [PCD target position measurement] [SRT]", m_szCmdName, m_nStep);
			if( is_simulation() ){
				NextStepTimer( exe_visn_recon_target_position_check, VISION_TIME_OUT );
				break;
			}

			//g_TaskTrayPP.VAT_VisionLED_ONOFF(DEF_ON, VAT_UPPER_CAM); 

			Sleep(1000); // Measure Delay.
			g_pVisionTestHand_1->SendMeasure_PcdBacklash(VISION_MEASURE_VAT_LD_UPPER, VAT_HAND_LOAD_TRAYPP);
			NextStepTimer( exe_visn_recon_target_position_check, VISION_TIME_OUT );
		}break;
	case exe_visn_recon_target_position_check:
		{
			ST_PCD_BACKLASH_RCV_DATA vRcvData;
			/* CheckMeasureResult() 에서 alarm check 처리 */
			if( CheckMeasureResult(vRcvData) == TRUE ){
				pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [Start Position measurement dx=%.3f um, dy=%.3f um] [END]", m_szCmdName, m_nStep, vRcvData.dX,vRcvData.dY);

				m_TargetPos_Offset.x += vRcvData.dX;
				m_TargetPos_Offset.y += vRcvData.dY;

				if(fabs(vRcvData.dX) < m_Cmd_PassRangeUm.x && fabs(vRcvData.dY) < m_Cmd_PassRangeUm.y)
				{
					NextStep( exe_final_z_safety_move );	
					break;
				}

				NextStep(exe_hand_z_start_position_safety_move);
			}
		}break;
	case exe_final_z_safety_move:
		{
			pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [move safety and picker all up] [SRT]", m_szCmdName, m_nStep );
			int nErr = pTaskTrayLoadPP->do_MovePos(eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY, Pocket, m_vSafetyAllPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}

			NextStepTimer( exe_final_z_safety_move_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_final_z_safety_move_check:
		{
			std::vector<CPoint> vPkr;
			pTaskTrayLoadPP->MakeAllPicker(vPkr);
			int nErrSen = pTaskTrayLoadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);
			m_vErrPicker.clear();
			int nAxisDone = pMotorZ->AxisDone(vPkr, m_vErrPicker);
			if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR)
			{
				pTaskTrayLoadPP->MakeLog("[%s] [EXEC=%8d] [move safety and picker all up] [END]", m_szCmdName, m_nStep);
				NextStep(exe_complete);
				break;
			}
			else {
				std::vector<CPoint> vErrPkr; vErrPkr.clear();
				CString sAlarmData = _T("");
				int nErr = pTaskTrayLoadPP->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, pTaskTrayLoadPP->GetSafetyFailPicker(ePickerSafetySensorCheckType::eSafetySensorCheck), vErrPkr, sAlarmData);

				if (m_tcTimeout.CheckOverTime() == TRUE) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("%s", sAlarmData);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
					ReportAlarm(nErr, AlarmData, vErrPkr);
					break;
				}
			}
		}break;
	case exe_complete:
		{
			pTaskTrayLoadPP->MakeLog("[%s_%d] PCD complete", m_szCmdName, m_nStep);
			//double dStartOffset = 0.0, dTargetOffset = 0.0;

			OneAxis* pMotor = NULL;
			
			double dStartPos = 0.0, dTargetPos = 0.0;

			if(m_nAxisNum == eAxis_X){
				dStartPos = m_StartPos.x + m_StartPos_Offset.x;
				dTargetPos = m_TargetPos.x + m_TargetPos_Offset.x;

				//dStartOffset = m_StartPos_Offset.x;
				//dTargetOffset = m_TargetPos_Offset.x;
				pMotor = pTaskTrayLoadPP->GetAxis(CTaskTrayLoadPP::eAxisX);
			}
 			else if(m_nAxisNum == eAxis_Y){
				dStartPos = m_StartPos.y + m_StartPos_Offset.y;
				dTargetPos = m_TargetPos.y + m_TargetPos_Offset.y;
// 
// 				//dStartOffset = m_StartPos_Offset.y;
// 				//dTargetOffset = m_TargetPos_Offset.y;
 				pMotor = g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eAxisY);
 			}

			if( pMotor == NULL){
				SimpleAlarm( ERR_LOT_CMD_PARAMETER_IS_INVALID ); 
				break;
			}

			// jhRyu [181214] Scale Factor 계산식 수정.
			double dMeasureDistance = fabs(dTargetPos - dStartPos); // Encoder 상으로 측정 된 Vision Target 간 거리.
			double dDesignDistance  = fabs(static_cast<double>(m_nDistance)); // 설계상의 Vision Target 간 거리.

			double dDesignScaleFactor = pMotor->GetScaleFactor(); // Scale Factor 설계치.
			ASSERT( dDesignScaleFactor > 0.01 );
			double dMeasurePulse = dMeasureDistance / dDesignScaleFactor;
			double dMeasureScaleFactor = dDesignDistance / dMeasurePulse;


			pTaskTrayLoadPP->MakeLog( "[V.A.T result] PCD AxisNum = %d, Offset = %f ", m_nAxisNum, dMeasureScaleFactor);

// 			if(m_nAxisNum == eAxis_X){
// 				g_VatPcd.traypp.m_PcdOffset_XY.x = dMeasureScaleFactor - pMotor->GetScaleFactor();
// 			}else if(m_nAxisNum == eAxis_Y){
// 				g_VatPcd.traypp.m_PcdOffset_XY.y = dMeasureScaleFactor - pMotor->GetScaleFactor();
// 			}
			double dCurSF = pMotor->GetScaleFactor(); //단위 : um
			double dSubDistance = fabs((dMeasureDistance - (double)m_nDistance)); //실제 이동거리 - 이론치 거리(단위 : um)
			double dOffsetSF =  dSubDistance > dCurSF ? dMeasureScaleFactor - pMotor->GetScaleFactor() : 0;
			if(m_nAxisNum == eAxis_X){
				g_VatPcd.trayloadpp.m_PcdOffset_XY.x = dOffsetSF;
			}
			else if(m_nAxisNum == eAxis_Y){
				g_VatPcd.trayloadpp.m_PcdOffset_XY.y = dOffsetSF;
			}
			g_VatPcd.trayloadpp.Save();

			/* notify complete */
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray Load PP PCD complete");
			AlarmData.m_fnAddItemFormat("%20s : %.5fum/p", _T("Before Scale Factor"), pMotor->GetScaleFactor());
			AlarmData.m_fnAddItemFormat("%20s : %.5fum/p", _T("Current Scale Factor"), dMeasureScaleFactor);
			AlarmData.m_fnAddItemFormat("%15s : %.5fmm", _T("Move Distance"), (double)m_nDistance / 1000);
			AlarmData.m_fnAddItemFormat("%15s : %.5fmm", _T("Motor Encoder"), dMeasureDistance / 1000);
			AlarmData.m_fnAddItemFormat("%s : %.5fmm", _T("GAB"), (dMeasureDistance - (double)m_nDistance) / 1000);
			AlarmData.m_fnAddItemFormat("%s : %.5fum", _T("GAB"), (dMeasureDistance - (double)m_nDistance) );
			g_Error.AlarmReport(ERR_VISION_AUTOTEACHING_DONE, AlarmData, &g_TaskSystemCtrl);

			ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);

		}break;
	}
	return CBaseSeqCmd::State_Executing();
}

//==============================================================================
//
//==============================================================================
BOOL CTrayLoadPPCmd_VatPcd::State_Pause()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = (CTaskTrayLoadPP*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskTrayLoadPP->MakeLog("[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			for (int i = 0; i < CTaskTrayBasePP::eAxisMax; i++)
			{
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)i);
				if (pMotor == nullptr) {
					continue;
				}

				pMotor->SetSlowStop();
			}

			OneAxis* pMotor = g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eMotor::eAxisY);
			pMotor->SetSlowStop();

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
BOOL CTrayLoadPPCmd_VatPcd::State_Recovery()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = (CTaskTrayLoadPP*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskTrayLoadPP->MakeLog("[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	OneAxis* pMotorZ  = pTaskTrayLoadPP->GetAxis(CTaskTrayLoadPP::eAxisZ);

	switch( m_nStep )
	{
	case 0: // 현재 위치와 마지막 위치를 비교 한다.
		{
			{
				pTaskTrayLoadPP->MakeLog("[%s] [RECOVERY=%8d]",m_szCmdName ,m_nStep);
				m_nStep = 1000;
			}
		}break;
	case 1000:
		{
			pTaskTrayLoadPP->MakeLog("[%s RECOVERY=%8d] [move safety and picker all up] [SRT]", m_szCmdName, m_nStep);
			int nErr = pTaskTrayLoadPP->do_MovePos(eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY, Pocket, m_vSafetyAllPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_LONG);
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
	case 1001:
		{
			int nAxisDone = pMotorZ->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				m_tcTimeoutPicker.SetCheckTime(TIME_OUT_SHORT);
				m_tcTimeoutPicker.StartTimer();
				m_nStep++;
				break;
			}
			else
			{
				if (m_tcTimeout.CheckOverTime() == TRUE) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
					ReportAlarm(ERR_TRAY_PP_1_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData);
					break;
				}
			}
		}break;
	case 1002:
		{
			if (m_tcTimeoutPicker.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				ReportAlarm(ERR_TRAY_PP_1_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData);
				break;
			}

			int nChk = pTaskTrayLoadPP->Chk_Z_Safety_Sen_All();
			if (nChk == ERR_NO_ERROR) {
				pTaskTrayLoadPP->MakeLog("[%s RECOVERY=%8d] [move safety and picker all up] [END]", m_szCmdName, m_nStep);
				m_nStep = 2000;
				break;
			}
		}break;
	case 2000:
		{
			SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing,m_nLast_Executing_Step);
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
BOOL CTrayLoadPPCmd_VatPcd::State_NormalComplete()
{
	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CTrayLoadPPCmd_VatPcd::State_Alarm()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = (CTaskTrayLoadPP*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskTrayLoadPP->MakeLog("[CMD: %s] [ALARM] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0: // alarm 발생 했을 때 처리해 줘야 할꺼 처리 해줌
		{
			for (int i = 0; i < CTaskTrayBasePP::eAxisMax; i++)
			{
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)i);
				if (pMotor == nullptr) {
					continue;
				}

				pMotor->SetSlowStop();
			}

			OneAxis* pMotor = g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eMotor::eAxisY);
			pMotor->SetSlowStop();

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
BOOL CTrayLoadPPCmd_VatPcd::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete(); 
}

//==============================================================================
//
//==============================================================================
BOOL CTrayLoadPPCmd_VatPcd::State_Aborting()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = (CTaskTrayLoadPP*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskTrayLoadPP->MakeLog("[CMD: %s] [ABORT] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			for (int i = 0; i < CTaskTrayBasePP::eAxisMax; i++)
			{
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)i);
				if (pMotor == nullptr) {
					continue;
				}

				pMotor->SetSlowStop();
			}

			OneAxis* pMotor = g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eMotor::eAxisY);
			pMotor->SetSlowStop();

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
BOOL CTrayLoadPPCmd_VatPcd::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete(); 
}


//==============================================================================
// local implementation
//==============================================================================
void CTrayLoadPPCmd_VatPcd::NextStep( execute_step_define next )
{
	m_nStep = next;
}

void CTrayLoadPPCmd_VatPcd::NextStepTimer( execute_step_define next , double tm )
{
	m_tcTimeout.SetCheckTime( tm );
	m_tcTimeout.StartTimer();
	NextStep( next );
}

void CTrayLoadPPCmd_VatPcd::SimpleAlarm( int alarm_id )
{
	CParamList AlarmData;
	ReportAlarm( alarm_id, AlarmData );
}

BOOL CTrayLoadPPCmd_VatPcd::CheckMeasureResult( ST_PCD_BACKLASH_RCV_DATA& result )
{
	if( m_tcTimeout.CheckOverTime() == TRUE ){
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Tray Load PP WebCam receive data time over!!");
		ReportAlarm(ERR_TRAY_PP_VISION_CMD_RESP_TIME_OUT, AlarmData);
		return FALSE;
	}

	/*vision align total time check*/
	//if( m_tcTimeoutVsn.CheckOverTime() == TRUE ){
	//	CParamList AlarmData;
	//	AlarmData.m_fnAddItemFormat("Tray PP WebCam retry time over!!");
	//	ReportAfalarm(ERR_VISION_CMD_RETRY_COUNT_OVER, AlarmData);
	//	return FALSE;
	//}

	g_pVisionTestHand_1->GetReceiveData((VAT_HAND_LOAD_TRAYPP), result);

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
		case CAM_STATUS_DISCONNECT	 : { AlarmData.m_fnAddItemFormat("Tray Unload PP WebCam disconnect!!"); nErrCnt++; } break;
		case CAM_STATUS_TIMEOUT		 : { AlarmData.m_fnAddItemFormat("Tray Unload PP WebCam time out!!"  ); nErrCnt++; } break;
		case CAM_STATUS_FAILED		 : { AlarmData.m_fnAddItemFormat("Tray Unload PP WebCam failed!!"    ); nErrCnt++; } break;
		case CAM_STATUS_UNKNOWN		 : { AlarmData.m_fnAddItemFormat("Tray Unload PP WebCam unknown!!"   ); nErrCnt++; } break;
		case CAM_STATUS_LIGHT_FAILED : { AlarmData.m_fnAddItemFormat("Tray Unload PP WebCam light fail!!"); nErrCnt++; } break;	
		}
		if( nErrCnt != 0 ){
			ReportAlarm(ERR_VISION_RESULT_NG, AlarmData);
			return FALSE;
		}

		if( result.nState == CAM_STATUS_SUCCESS ){
			return TRUE;
		}
	}
	return FALSE;
}
