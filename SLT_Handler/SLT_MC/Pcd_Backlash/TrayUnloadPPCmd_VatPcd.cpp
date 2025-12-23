#include "StdAfx.h"
#include "TrayLoadPPCmd_VatPcd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//CRamTeaching  m_Teaching_Data_;   // decrease by PAGESIZE Error 전역을 써야함.

/* simulation code */
#define is_simulation() ((g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) ? TRUE : FALSE)
#define TRAY_UNLOAD_PP_PCD_SPEED (15)
/* simulation code */
//==============================================================================
//
//==============================================================================

CTrayUnloadPPCmd_VatPcd_Param::CTrayUnloadPPCmd_VatPcd_Param(
	CPoint              _Pocket,
	std::vector<CPoint> _vPicker)
{
	Pocket = _Pocket;
	vPicker = _vPicker;
}
CTrayUnloadPPCmd_VatPcd_Param::CTrayUnloadPPCmd_VatPcd_Param()
{
	//Pocket = _Pocket;
	vPicker.clear();
}



CTrayUnloadPPCmd_VatPcd_Param::CTrayUnloadPPCmd_VatPcd_Param(const CTrayUnloadPPCmd_VatPcd_Param& src)
{
	Pocket = src.Pocket;
	vPicker = src.vPicker;
}

CTrayUnloadPPCmd_VatPcd_Param& CTrayUnloadPPCmd_VatPcd_Param::operator=(const CTrayUnloadPPCmd_VatPcd_Param& src)
{
	if (this == &src) return *this;

	Pocket = src.Pocket;
	vPicker = src.vPicker;

	return *this;
}


CTrayUnloadPPCmd_VatPcd_Param::~CTrayUnloadPPCmd_VatPcd_Param()
{

}

CTrayUnloadPPCmd_VatPcd::CTrayUnloadPPCmd_VatPcd(CTrayUnloadPPCmd_VatPcd_Param _Prm)
	: m_Cmd(_Prm)
{
	m_nCmdId = eSeqCmd_TrayUnloadPP_VatPcd;
	m_nRetryCount = 0;
	sprintf_s(m_szCmdName, sizeof(m_szCmdName), "TrayUnloadPPCmd_VatPcd");

}


CTrayUnloadPPCmd_VatPcd::CTrayUnloadPPCmd_VatPcd(void)
{
	m_nCmdId = eSeqCmd_TrayUnloadPP_VatPcd;
	m_nRetryCount = 0;
	sprintf_s(m_szCmdName, sizeof(m_szCmdName), "CTrayUnloadPPCmd_VatPcd");

	char szDrvFile[512] = { 0, };
	char szCokName[512] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));

	char szCurDir[512] = { 0, };
	GetCurrentDirectory(sizeof(szCurDir), szCurDir);
	sprintf_s(szDrvFile, sizeof(szDrvFile), "%s\\sys_data\\%s", szCurDir, SZ_DRIVER_FILE_NAME);

// 	g_DMCont.m_Teach.Load();
// 	m_Teaching_Data_.Initialize(szDrvFile, szCokName);
// 	m_Teaching_Data_ = g_DMCont.m_Teach;
// 
// 	m_dTrayPP_Z_Safety = 0;
// 	m_dTrayPP_Z_Safety = m_Teaching_Data_.GetTeachPos(40, 9, 4);
	
	/*param default*/
	m_nChkCount =0;
	m_nAxisNum = 0;
	m_nDistance = 0;
}


//==============================================================================
//
//==============================================================================
CTrayUnloadPPCmd_VatPcd::~CTrayUnloadPPCmd_VatPcd(void){}

//==============================================================================
//
//==============================================================================
int CTrayUnloadPPCmd_VatPcd::CheckParam()
{
	double dTimeout = 60.0;
	m_tcTimeout.SetCheckTime(dTimeout);
	m_tcTimeout.StartTimer();
	m_nLast_Executing_Step = exe_start;
	m_nChkCount = 0;

	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;

	if (is_simulation()) {
		m_Cmd_PassRangeUm.x = 200.0;
		m_Cmd_PassRangeUm.y = 200.0;
	}
	else {
		OneAxis* pMotorX = pTaskTrayUnloadPP->GetAxis(CTaskTrayUnloadPP::eAxisX);
		OneAxis* pMotorY = pTaskTrayUnloadPP->GetAxis(CTaskTrayUnloadPP::eAxisY);
		m_Cmd_PassRangeUm.x = pMotorX->GetScaleFactor();
		m_Cmd_PassRangeUm.y = pMotorY->GetScaleFactor();
	}

	return ERR_NO_ERROR;
}


//==============================================================================
//
//==============================================================================
int  CTrayUnloadPPCmd_VatPcd::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTrayUnloadPPCmd_VatPcd::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTrayUnloadPPCmd_VatPcd::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTrayUnloadPPCmd_VatPcd::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTrayUnloadPPCmd_VatPcd::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CTrayUnloadPPCmd_VatPcd::State_Setup()
{	
	return CBaseSeqCmd::State_Setup();
}

//==============================================================================
//
//==============================================================================
BOOL CTrayUnloadPPCmd_VatPcd::State_Executing()
{
	int nErr = 0;

	// check alarm
	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}

	m_nStep_Pre = m_nStep;

	for(int i=0; i< CTaskTrayBasePP::eAxisMax; i++){
		OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)i);
		if( pMotor->ChkMotorUsable() != ERR_NO_ERROR ){
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray PP check motor usable Cmd Error");
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(ERR_TRAY_PP_2_MOTOR_ALARMED, AlarmData);
			return FALSE;
		}
	}

	OneAxis* pMotorX = pTaskTrayUnloadPP->GetAxis(CTaskTrayUnloadPP::eAxisX);
	OneAxis* pMotorY = pTaskTrayUnloadPP->GetAxis(CTaskTrayUnloadPP::eAxisY);
	OneAxis* pMotorZ  = pTaskTrayUnloadPP->GetAxis(CTaskTrayUnloadPP::eAxisZ);

	switch( m_nStep )
	{		
	case exe_start:
		{
			if(m_nAxisNum != eAxis_X && m_nAxisNum !=eAxis_Y) nErr++;

			if(m_nAxisNum == eAxis_X){
				m_StartPos = g_VatPcd.trayunloadpp.m_PcdStartXPos_XYZ;
			}else{
				m_StartPos = g_VatPcd.trayunloadpp.m_PcdStartYPos_XYZ;
			}

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
			pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [move safety and picker all up] [SRT]", m_szCmdName, m_nStep);
			int nErr = pTaskTrayUnloadPP->do_MovePos(eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY, m_Cmd.Pocket, m_vSafetyAllPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}

			m_tcTimeout.SetCheckTime(TIME_OUT_SHORT);
			m_tcTimeout.StartTimer();

			NextStep( exe_hand_z_safety_move_check);
		}break;
	case exe_hand_z_safety_move_check:
		{
			std::vector<CPoint> vPkr;
			pTaskTrayUnloadPP->MakeAllPicker(vPkr);
			int nErrSen = pTaskTrayUnloadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);
			m_vErrPicker.clear();
			int nAxisDone = pMotorZ->AxisDone(vPkr, m_vErrPicker);
			if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR /*&& nErrPicker == ERR_NO_ERROR */) {
				pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [move safety and picker all up] [END]", m_szCmdName, m_nStep);
				NextStep(exe_hand_xy_start_position_move_100mm);
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
	case exe_hand_xy_start_position_move_100mm:
		{
			int nAxisNo[2] = {CTaskTrayUnloadPP::eAxisX, CTaskTrayUnloadPP::eAxisY};

			double dTPos[2] = {0.0,};
			dTPos[0] = m_StartPos.x;
			dTPos[1] = m_StartPos.y;

			dTPos[0] += m_StartPos_Offset.x;
			dTPos[1] += m_StartPos_Offset.y;

			pTaskTrayUnloadPP->MakeVatPreparePos(dTPos[CTaskTrayUnloadPP::eAxisX], dTPos[CTaskTrayUnloadPP::eAxisY], Move_100mm_Offset);

			pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [x, y move_100mm start position] [SRT]", m_szCmdName, m_nStep);
			for(int i=0; i<_countof(nAxisNo); i++){
				int nErr = pTaskTrayUnloadPP->do_MovePos_Pcd_backlash(m_Cmd.vPicker, dTPos[i], (CTaskTrayBasePP::eMotor)nAxisNo[i], TRAY_UNLOAD_PP_PCD_SPEED);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}
			NextStepTimer( exe_hand_xy_start_position_move_100mm_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_xy_start_position_move_100mm_check:
		{


			int nChkComp  = 0;
			int nAxisNo[2] = {CTaskTrayUnloadPP::eAxisX, CTaskTrayUnloadPP::eAxisY};
			for(int i=0; i<_countof(nAxisNo); i++){
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
				int nAxisDone = pMotor->AxisDone();
				if( nAxisDone == ERR_NO_ERROR){
					nChkComp++;
				}
				else {
					if (m_tcTimeout.CheckOverTime() == TRUE) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(ERR_TRAY_PP_2_MOTOR_MOVE_TIMEOUT, AlarmData);
						break;
					}
				}
			}
			if( nChkComp == _countof(nAxisNo)){
				pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [x, y move_100mm start position] [END]", m_szCmdName, m_nStep);

				m_nRetryCount = 0;
				NextStep( exe_hand_xy_start_position_move );
			}
		}break;
	case exe_hand_xy_start_position_move:
		{
			int nAxisNo[2] = {CTaskTrayUnloadPP::eAxisX, CTaskTrayUnloadPP::eAxisY};

			double dTPos[2] = {0.0,};
			dTPos[0] = m_StartPos.x;
			dTPos[1] = m_StartPos.y;

			dTPos[0] += m_StartPos_Offset.x;
			dTPos[1] += m_StartPos_Offset.y;

			pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [x, y move start position] [SRT]", m_szCmdName, m_nStep);
			for(int i=0; i<2; i++){
				int nErr = pTaskTrayUnloadPP->do_MovePos_Pcd_backlash(m_Cmd.vPicker, dTPos[i], (CTaskTrayBasePP::eMotor)nAxisNo[i], TRAY_UNLOAD_PP_PCD_SPEED);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}
			NextStepTimer( exe_hand_xy_start_position_move_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_xy_start_position_move_check:
		{	
			int nChkComp  = 0;
			int nAxisNo[2] = {CTaskTrayUnloadPP::eAxisX, CTaskTrayUnloadPP::eAxisY};
			for(int i=0; i<2; i++){
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
				int nAxisDone = pMotor->AxisDone();
				if( nAxisDone == ERR_NO_ERROR ){
					nChkComp++;
				}
				else {
					if (m_tcTimeout.CheckOverTime() == TRUE) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(ERR_TRAY_PP_2_MOTOR_MOVE_TIMEOUT, AlarmData);
						break;
					}
				}
			}
			if( nChkComp == 2 ){
				pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [x, y move start position] [END]", m_szCmdName, m_nStep);

				m_nRetryCount = 0;
				NextStep( exe_hand_z_start_position_move );
			}
		}break;

	case exe_hand_z_start_position_move:
		{
			double dTPos = m_StartPos.z;
			pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [z move to start position] [SRT]", m_szCmdName, m_nStep);

			nErr = pTaskTrayUnloadPP->do_MovePos_Pcd_backlash(m_Cmd.vPicker, dTPos, CTaskTrayUnloadPP::eAxisZ, TRAY_UNLOAD_PP_PCD_SPEED);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
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
				ReportAlarm(ERR_TRAY_PP_2_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData);
				break;
			}
			int nAxisDone = pMotorZ->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [z move to start position] [END]", m_szCmdName, m_nStep);
				NextStep(exe_visn_recon_start_position);
			}
		}break;
	case exe_visn_recon_start_position:
		{
			pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [PCD start position measurement] [SRT]", m_szCmdName, m_nStep);
			if( is_simulation() ){
				NextStepTimer( exe_visn_recon_start_position_check, VISION_TIME_OUT );
				break;
			}

			Sleep(1000); // Measure Delay.
			g_pVisionTestHand_1->SendMeasure_PcdBacklash(VISION_MEASURE_VAT_ULD_UPPER, VAT_HAND_UNLOAD_TRAYPP);
			NextStepTimer( exe_visn_recon_start_position_check, VISION_TIME_OUT );
		}break;
	case exe_visn_recon_start_position_check:
		{
			ST_PCD_BACKLASH_RCV_DATA vRcvData;
			/* CheckMeasureResult() 에서 alarm check 처리 */
			if( CheckMeasureResult(vRcvData) == TRUE ){
				pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [Start Position measurement dx=%.3f um, dy=%.3f um] [END]", m_szCmdName, m_nStep, vRcvData.dX,vRcvData.dY);

				m_StartPos_Offset.x += vRcvData.dX;
				m_StartPos_Offset.y += vRcvData.dY;
				
				if(fabs(vRcvData.dX) < m_Cmd_PassRangeUm.x && fabs(vRcvData.dY) < m_Cmd_PassRangeUm.y){
					NextStep( exe_hand_z_start_position_safety_move );	
					break;
				}
				NextStep( exe_pcd_loop_entry );	
			}
		}break;
	case exe_hand_z_start_position_safety_move:
		{
			pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [move safety and picker all up] [SRT]", m_szCmdName, m_nStep );
			int nErr = pTaskTrayUnloadPP->do_MovePos(eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY, m_Cmd.Pocket, m_vSafetyAllPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			NextStepTimer( exe_hand_z_start_position_safety_move_check, TIME_OUT_SHORT);
		}break;
	case exe_hand_z_start_position_safety_move_check:
		{
			std::vector<CPoint> vPkr;
			pTaskTrayUnloadPP->MakeAllPicker(vPkr);
			int nErrSen = pTaskTrayUnloadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);
			m_vErrPicker.clear();
			int nAxisDone = pMotorZ->AxisDone(vPkr, m_vErrPicker);
			if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR /*&& nErrPicker == ERR_NO_ERROR */) {
				pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [move safety and picker all up] [END]", m_szCmdName, m_nStep);
				NextStep(exe_hand_xy_target_position_move_100mm);
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

	case exe_hand_xy_target_position_move_100mm:
		{
			int nAxisNo[2] = {CTaskTrayUnloadPP::eAxisX, CTaskTrayUnloadPP::eAxisY};

			double dTPos[2] = {0.0,};
			dTPos[0] = m_TargetPos.x;
			dTPos[1] = m_TargetPos.y;

			dTPos[0] += m_TargetPos_Offset.x;
			dTPos[1] += m_TargetPos_Offset.y;

			pTaskTrayUnloadPP->MakeVatPreparePos(dTPos[CTaskTrayUnloadPP::eAxisX], dTPos[CTaskTrayUnloadPP::eAxisY], Move_100mm_Offset);

			pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [x, y move_100mm target position] [SRT]", m_szCmdName, m_nStep);
			for(int i=0; i<2; i++){
				int nErr = pTaskTrayUnloadPP->do_MovePos_Pcd_backlash(m_Cmd.vPicker, dTPos[i], (CTaskTrayBasePP::eMotor)nAxisNo[i], TRAY_UNLOAD_PP_PCD_SPEED);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}
			NextStepTimer( exe_hand_xy_target_position_move_100mm_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_xy_target_position_move_100mm_check:
		{


			int nChkComp  = 0;
			int nAxisNo[2] = {CTaskTrayUnloadPP::eAxisX, CTaskTrayUnloadPP::eAxisY};
			for(int i=0; i<2; i++){
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
				int nAxisDone = pMotor->AxisDone();
				if (nAxisDone == ERR_NO_ERROR) {
					nChkComp++;
				}
				else {
					if (m_tcTimeout.CheckOverTime() == TRUE) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(ERR_TRAY_PP_2_MOTOR_MOVE_TIMEOUT, AlarmData);
						break;
					}
				}
			}
			if( nChkComp == 2 ){
				pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [x, y move_100mm target position] [END]", m_szCmdName, m_nStep);

				m_nRetryCount = 0;
				NextStep( exe_hand_xy_target_position_move );
			}
		}break;
	case exe_hand_xy_target_position_move:
		{
			int nAxisNo[2] = {CTaskTrayUnloadPP::eAxisX, CTaskTrayUnloadPP::eAxisY};
			
			double dTPos[2] = {0.0,};
			dTPos[0] = m_TargetPos.x;
			dTPos[1] = m_TargetPos.y;

			dTPos[0] += m_TargetPos_Offset.x;
			dTPos[1] += m_TargetPos_Offset.y;

// 			if(m_nAxisNum == eAxis_X){
// 				dTPos[0] += m_nDistance;
// 			}else if(m_nAxisNum == eAxis_Y){
// 				dTPos[1] += m_nDistance;
// 			}

			pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [x, y move target position] [SRT]", m_szCmdName, m_nStep);
			for(int i=0; i<2; i++){
				int nErr = pTaskTrayUnloadPP->do_MovePos_Pcd_backlash(m_Cmd.vPicker, dTPos[i], (CTaskTrayBasePP::eMotor)nAxisNo[i], TRAY_UNLOAD_PP_PCD_SPEED);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}
			NextStepTimer( exe_hand_xy_target_position_move_check, MOTOR_TIME_OUT_LONG );
		}break;

	case exe_hand_xy_target_position_move_check:
		{
			int nChkComp  = 0;
			int nAxisNo[2] = {CTaskTrayUnloadPP::eAxisX, CTaskTrayUnloadPP::eAxisY};
			for(int i=0; i<2; i++){
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
				int nAxisDone = pMotor->AxisDone();
				if( nAxisDone == ERR_NO_ERROR ){
					nChkComp++;
				}
				else {
					if (m_tcTimeout.CheckOverTime() == TRUE) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(ERR_TRAY_PP_2_MOTOR_MOVE_TIMEOUT, AlarmData);
						break;
					}
				}
			}
			if( nChkComp == 2 ){
				pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [x, y move target position] [END]", m_szCmdName, m_nStep);

				m_nRetryCount = 0;
				NextStep( exe_hand_z_target_position_move );
			}
		}break;
	case exe_hand_z_target_position_move:
		{
			double dTPos = m_StartPos.z;
			pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [z move to target position] [SRT]", m_szCmdName, m_nStep);
			int nErr = pTaskTrayUnloadPP->do_MovePos_Pcd_backlash(m_Cmd.vPicker, dTPos, CTaskTrayBasePP::eAxisZ, TRAY_UNLOAD_PP_PCD_SPEED);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			NextStepTimer( exe_hand_z_target_position_move_check, 30.0 );
		}break;
	case exe_hand_z_target_position_move_check:
		{
			if( m_tcTimeout.CheckOverTime() == TRUE ){
				SimpleAlarm(ERR_TRAY_PP_2_Z_PICKER_MOTOR_MOVE_TIMEOUT);
				break;
			}
			int nAxisDone = pMotorZ->AxisDone();
			if( nAxisDone == ERR_NO_ERROR ){
				pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [z move to target position] [END]", m_szCmdName, m_nStep);
				NextStep( exe_visn_recon_target_position );
			}
		}break;
	case exe_visn_recon_target_position:
		{
			pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [PCD target position measurement] [SRT]", m_szCmdName, m_nStep);
			if( is_simulation() ){
				NextStepTimer( exe_visn_recon_target_position_check, VISION_TIME_OUT );
				break;
			}

			//g_TaskTrayPP.VAT_VisionLED_ONOFF(DEF_ON, VAT_UPPER_CAM); 

			Sleep(1000); // Measure Delay.
			g_pVisionTestHand_1->SendMeasure_PcdBacklash(VISION_MEASURE_VAT_ULD_UPPER, VAT_HAND_UNLOAD_TRAYPP);
			NextStepTimer( exe_visn_recon_target_position_check, VISION_TIME_OUT );
		}break;
	case exe_visn_recon_target_position_check:
		{
			ST_PCD_BACKLASH_RCV_DATA vRcvData;
			/* CheckMeasureResult() 에서 alarm check 처리 */
			if( CheckMeasureResult(vRcvData) == TRUE ){
				pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [Start Position measurement dx=%.3f um, dy=%.3f um] [END]", m_szCmdName, m_nStep, vRcvData.dX,vRcvData.dY);

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
			pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [move safety and picker all up] [SRT]", m_szCmdName, m_nStep);
			int nErr = pTaskTrayUnloadPP->do_MovePos(eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY, m_Cmd.Pocket, m_vSafetyAllPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			NextStepTimer( exe_final_z_safety_move_check, TIME_OUT_SHORT);
		}break;
	case exe_final_z_safety_move_check:
		{
			std::vector<CPoint> vPkr;
			pTaskTrayUnloadPP->MakeAllPicker(vPkr);
			int nErrSen = pTaskTrayUnloadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);
			m_vErrPicker.clear();
			int nAxisDone = pMotorZ->AxisDone(vPkr, m_vErrPicker);
			if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR /*&& nErrPicker == ERR_NO_ERROR */) {
				pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [move safety and picker all up] [END]", m_szCmdName, m_nStep);
				NextStep(exe_complete);
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
	case exe_complete:
		{
			pTaskTrayUnloadPP->MakeLog("[%s_%d] PCD complete", m_szCmdName, m_nStep);
			//double dStartOffset = 0.0, dTargetOffset = 0.0;

			OneAxis* pMotor = NULL;
			
			double dStartPos = 0.0, dTargetPos = 0.0;

			if(m_nAxisNum == eAxis_X){
				dStartPos = m_StartPos.x + m_StartPos_Offset.x;
				dTargetPos = m_TargetPos.x + m_TargetPos_Offset.x;

				//dStartOffset = m_StartPos_Offset.x;
				//dTargetOffset = m_TargetPos_Offset.x;
				pMotor = pTaskTrayUnloadPP->GetAxis(CTaskTrayUnloadPP::eAxisX);
			}else if(m_nAxisNum == eAxis_Y){
				dStartPos = m_StartPos.y + m_StartPos_Offset.y;
				dTargetPos = m_TargetPos.y + m_TargetPos_Offset.y;

				//dStartOffset = m_StartPos_Offset.y;
				//dTargetOffset = m_TargetPos_Offset.y;
				pMotor = pTaskTrayUnloadPP->GetAxis(CTaskTrayUnloadPP::eAxisY);
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


			pTaskTrayUnloadPP->MakeLog( "[V.A.T result] PCD AxisNum = %d, Offset = %f ", m_nAxisNum, dMeasureScaleFactor);

// 			if(m_nAxisNum == eAxis_X){
// 				g_VatPcd.traypp.m_PcdOffset_XY.x = dMeasureScaleFactor - pMotor->GetScaleFactor();
// 			}else if(m_nAxisNum == eAxis_Y){
// 				g_VatPcd.traypp.m_PcdOffset_XY.y = dMeasureScaleFactor - pMotor->GetScaleFactor();
// 			}
			double dCurSF = pMotor->GetScaleFactor(); //단위 : um
			double dSubDistance = fabs((dMeasureDistance - (double)m_nDistance)); //실제 이동거리 - 이론치 거리(단위 : um)
			double dOffsetSF =  dSubDistance > dCurSF ? dMeasureScaleFactor - pMotor->GetScaleFactor() : 0;
			if(m_nAxisNum == eAxis_X){
				g_VatPcd.trayunloadpp.m_PcdOffset_XY.x = dOffsetSF;
			}else if(m_nAxisNum == eAxis_Y){
				g_VatPcd.trayunloadpp.m_PcdOffset_XY.y = dOffsetSF;
			}
			g_VatPcd.trayunloadpp.Save();

			/* notify complete */
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray Unload PP PCD complete");
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
BOOL CTrayUnloadPPCmd_VatPcd::State_Pause()
{
	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskTrayUnloadPP->MakeLog("[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			for(int i=0; i<CTaskTrayBasePP::eAxisMax; i++)
			{
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)i);
				if (pMotor == nullptr) {
					continue;
				}

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
BOOL CTrayUnloadPPCmd_VatPcd::State_Recovery()
{
	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskTrayUnloadPP->MakeLog("[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	OneAxis* pMotorZ  = pTaskTrayUnloadPP->GetAxis(CTaskTrayUnloadPP::eAxisZ);

	switch( m_nStep )
	{
	case 0: // 현재 위치와 마지막 위치를 비교 한다.
		{
			{
				pTaskTrayUnloadPP->MakeLog("[%s] [RECOVERY=%8d]",m_szCmdName ,m_nStep);
				m_nStep = 1000;
			}
		}break;
	case 1000:
		{
			pTaskTrayUnloadPP->MakeLog("[%s RECOVERY=%8d] [move safety and picker all up] [SRT]", m_szCmdName, m_nStep);
			int nErr = pTaskTrayUnloadPP->do_MovePos(eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY, m_Cmd.Pocket, m_vSafetyAllPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}

			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
	case 1001:
		{
			std::vector<CPoint> vPkr;
			pTaskTrayUnloadPP->MakeAllPicker(vPkr);
			int nErrSen = pTaskTrayUnloadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);
			m_vErrPicker.clear();
			int nAxisDone = pMotorZ->AxisDone(vPkr, m_vErrPicker);
			if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR /*&& nErrPicker == ERR_NO_ERROR */) {
				pTaskTrayUnloadPP->MakeLog("[%s RECOVERY=%8d] [move safety and picker all up] [END]", m_szCmdName, m_nStep);
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
BOOL CTrayUnloadPPCmd_VatPcd::State_NormalComplete()
{
	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CTrayUnloadPPCmd_VatPcd::State_Alarm()
{
	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskTrayUnloadPP->MakeLog("[CMD: %s] [ALARM] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0: // alarm 발생 했을 때 처리해 줘야 할꺼 처리 해줌
		{
			for (int i = 0; i < CTaskTrayBasePP::eAxisMax; i++)
			{
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)i);
				if (pMotor == nullptr) {
					continue;
				}

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
BOOL CTrayUnloadPPCmd_VatPcd::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete(); 
}

//==============================================================================
//
//==============================================================================
BOOL CTrayUnloadPPCmd_VatPcd::State_Aborting()
{
	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskTrayUnloadPP->MakeLog("[CMD: %s] [ABORT] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			for (int i = 0; i < CTaskTrayBasePP::eAxisMax; i++)
			{
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)i);
				if (pMotor == nullptr) {
					continue;
				}

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
BOOL CTrayUnloadPPCmd_VatPcd::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete(); 
}


//==============================================================================
// local implementation
//==============================================================================
void CTrayUnloadPPCmd_VatPcd::NextStep( execute_step_define next )
{
	m_nStep = next;
}

void CTrayUnloadPPCmd_VatPcd::NextStepTimer( execute_step_define next , double tm )
{
	m_tcTimeout.SetCheckTime( tm );
	m_tcTimeout.StartTimer();
	NextStep( next );
}

void CTrayUnloadPPCmd_VatPcd::SimpleAlarm( int alarm_id )
{
	CParamList AlarmData;
	ReportAlarm( alarm_id, AlarmData );
}

BOOL CTrayUnloadPPCmd_VatPcd::CheckMeasureResult( ST_PCD_BACKLASH_RCV_DATA& result )
{
	if( m_tcTimeout.CheckOverTime() == TRUE ){
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Tray Unload PP WebCam receive data time over!!");
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

	g_pVisionTestHand_1->GetReceiveData((VAT_HAND_UNLOAD_TRAYPP), result);

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
