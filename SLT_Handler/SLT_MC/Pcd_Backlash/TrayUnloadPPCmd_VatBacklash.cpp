#include "StdAfx.h"
#include "TrayUnloadPPCmd_VatBacklash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



/* simulation code */
#define is_simulation() ((g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) ? TRUE : FALSE)
/* simulation code */
#define TRAY_UNLOAD_PP_PCD_SPEED (15)
//CRamTeaching  m_Teaching_Data;   // decrease by PAGESIZE Error 전역을 써야함.

//==============================================================================
//
//==============================================================================
CTrayUnloadPPCmd_VatBacklash_Param::CTrayUnloadPPCmd_VatBacklash_Param(
	CPoint              _Pocket,
	std::vector<CPoint> _vPicker)
{
	Pocket = _Pocket;
	vPicker = _vPicker;
}
CTrayUnloadPPCmd_VatBacklash_Param::CTrayUnloadPPCmd_VatBacklash_Param()
{
	//Pocket = _Pocket;
	vPicker.clear();
}

CTrayUnloadPPCmd_VatBacklash_Param::CTrayUnloadPPCmd_VatBacklash_Param(const CTrayUnloadPPCmd_VatBacklash_Param& src)
{
	Pocket = src.Pocket;
	vPicker = src.vPicker;
}


CTrayUnloadPPCmd_VatBacklash_Param& CTrayUnloadPPCmd_VatBacklash_Param::operator=(const CTrayUnloadPPCmd_VatBacklash_Param& src)
{
	if (this == &src) return *this;

	Pocket = src.Pocket;
	vPicker = src.vPicker;

	return *this;
}

CTrayUnloadPPCmd_VatBacklash_Param::~CTrayUnloadPPCmd_VatBacklash_Param()
{

}


CTrayUnloadPPCmd_VatBacklash::CTrayUnloadPPCmd_VatBacklash(CTrayUnloadPPCmd_VatBacklash_Param _Prm)
	: m_Cmd(_Prm)
{
	m_nCmdId = eSeqCmd_TrayUnloadPP_VatPcd;
	m_nRetryCount = 0;
	sprintf_s(m_szCmdName, sizeof(m_szCmdName), "TrayUnloadPPCmd_VatPcd");

	m_nSpeed = 0;
	m_nLoopCnt = 0;
	m_nAxisNum = eAxis_X;
	m_bBacklashTest = FALSE;
	m_bRepeat = FALSE;
	m_nRepeatCnt = 1;
	m_nRepeatTempCnt = 1;
	m_pParentWnd = nullptr;
	m_nChkLoopCnt = 0;
}

CTrayUnloadPPCmd_VatBacklash::~CTrayUnloadPPCmd_VatBacklash()
{

}

CTrayUnloadPPCmd_VatBacklash::CTrayUnloadPPCmd_VatBacklash(CDlgPcdBacklash *pParent)
{
	char szDrvFile[512] = { 0, };
	char szCokName[512] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));

	char szCurDir[512] = { 0, };
	GetCurrentDirectory(sizeof(szCurDir), szCurDir);
	sprintf_s(szDrvFile, sizeof(szDrvFile), "%s\\sys_data\\%s", szCurDir, SZ_DRIVER_FILE_NAME);

// 	g_DMCont.m_Teach.Load();
// 	m_Teaching_Data.Initialize(szDrvFile, szCokName);
// 	m_Teaching_Data = g_DMCont.m_Teach;
// 
// 	m_dTrayPP_Z_Safety = 0;
// 	m_dTrayPP_Z_Safety = m_Teaching_Data.GetTeachPos(40, 9, 4);

	m_pParentWnd = pParent;
	m_nCmdId = eSeqCmd_TrayPP_VatAtWCamTch;
	m_nRetryCount = 0;
	sprintf_s(m_szCmdName, sizeof(m_szCmdName), "CTrayUnloadPPCmd_VatBacklash");
	
	m_bBacklashTest = FALSE;
	/*param default*/
	m_nAxisNum = eAxis_X;
	m_qSpeed.clear();
	m_nLoopCnt = 0;
	m_nSpeed = 0;
	m_bRepeat = FALSE;
	m_nRepeatCnt = 1;
	m_nRepeatTempCnt = 1;
	m_cTime = CTime::GetCurrentTime(); // 현재 시스템으로부터 날짜 및 시간을 얻어 온다.

	m_vEndToTargetOffset.clear();
	m_vStartToTargetOffset.clear();

	m_BacklashXTestLog.SetFileName("D:\\techwing\\LOG\\BacklashXTest.log");
	m_BacklashYTestLog.SetFileName("D:\\techwing\\LOG\\BacklashYTest.log");

	m_nChkLoopCnt = 0;
}


//
//==============================================================================
int CTrayUnloadPPCmd_VatBacklash::CheckParam()
{
	double dTimeout = 60.0;
	m_tcTimeout.SetCheckTime(dTimeout);
	m_tcTimeout.StartTimer();
	m_nLast_Executing_Step = exe_start;
	m_nChkLoopCnt = 0;

	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;

	OneAxis* pMotorX = pTaskTrayUnloadPP->GetAxis(CTaskTrayUnloadPP::eAxisX);
	OneAxis* pMotorY = pTaskTrayUnloadPP->GetAxis(CTaskTrayUnloadPP::eAxisY);

	m_vEndToTargetOffset.clear();
	m_vStartToTargetOffset.clear();

	return ERR_NO_ERROR;
}


//==============================================================================
//
//==============================================================================
int  CTrayUnloadPPCmd_VatBacklash::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTrayUnloadPPCmd_VatBacklash::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTrayUnloadPPCmd_VatBacklash::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTrayUnloadPPCmd_VatBacklash::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTrayUnloadPPCmd_VatBacklash::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CTrayUnloadPPCmd_VatBacklash::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}

//==============================================================================
//
//==============================================================================
BOOL CTrayUnloadPPCmd_VatBacklash::State_Executing()
{
	int nErr = 0;

	// check alarm
	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
// 	if( m_nStep_Pre != m_nStep ){
// 		pTaskTrayUnloadPP->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d]", m_szCmdName, m_nStep);
// 	}
	m_nStep_Pre = m_nStep;

	int i=0;
	for( i=0; i< CTaskTrayBasePP::eAxisMax; i++){
		OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)i);
		if( pMotor->ChkMotorUsable() != ERR_NO_ERROR ){
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray Unload PP check motor usable Cmd Error");
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(ERR_TRAY_PP_2_MOTOR_ALARMED, AlarmData);
			return FALSE;
		}
	}

 	OneAxis* pMotorX  = pTaskTrayUnloadPP->GetAxis(CTaskTrayUnloadPP::eAxisX);
 	OneAxis* pMotorY  = pTaskTrayUnloadPP->GetAxis(CTaskTrayUnloadPP::eAxisY);
	OneAxis* pMotorZ  = pTaskTrayUnloadPP->GetAxis(CTaskTrayUnloadPP::eAxisZ);
 	OneAxis* pMotorPX = pTaskTrayUnloadPP->GetAxis(CTaskTrayUnloadPP::eAxisPitchX);
 	OneAxis* pMotorPY = pTaskTrayUnloadPP->GetAxis(CTaskTrayUnloadPP::eAxisPitchY);

	switch( m_nStep )
	{		
	case exe_start:
		{
			if( m_qSpeed.size() == 0) nErr++;

			if(m_nLoopCnt < 0 )nErr++;

			if( nErr ){
				SimpleAlarm( ERR_LOT_CMD_PARAMETER_IS_INVALID );
				break;
			} else {
				m_nRetryCount = 0;

				m_vEndToTargetOffset.clear();
				m_vStartToTargetOffset.clear();
				m_vBacklashAdjust.clear();
				m_nChkLoopCnt = 0;

				m_nSpeed = m_qSpeed.front(); //by kmk [20181226] 값 처음 값을 꺼내 초기 스피드로 할당.
				m_qSpeed.pop_front();
				m_pParentWnd->ReportBacklashResult(m_nAxisNum, m_nSpeed);
				NextStep( exe_backlash_loop_entry );
			}
		}break;

	case exe_backlash_loop_entry:
		{
			
			if(m_nLoopCnt <= m_nChkLoopCnt){

				CalcBacklashOffset(m_nSpeed, m_bBacklashTest);
				WriteRepeatOffsetXY(m_vEndToTargetOffset, m_vStartToTargetOffset, m_nSpeed); //Backlash 반복 Log 남기기
				m_vEndToTargetOffset.clear();
				m_vStartToTargetOffset.clear();
				m_nChkLoopCnt = 0;

				if(m_qSpeed.size() == 0 )
				{
					if(m_bBacklashTest == TRUE)
					{
						NextStep(exe_complete);
						break;
					}
					else
					{
						if(m_bRepeat == FALSE)
						{
							NextStep(exe_complete);
						}
						else
						{
							InitLoopStatus();
							NextStep(exe_start);
						}
						break;
					}
				}else{
					//계산 

					m_nSpeed = m_qSpeed.front();
					m_qSpeed.pop_front();
					m_pParentWnd->ReportBacklashResult(m_nAxisNum, m_nSpeed);
				}
			}
			m_nChkLoopCnt++;
			
			pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d][Backlash Start Count : %d, Speed : %d ]",m_szCmdName ,m_nStep,m_nChkLoopCnt, m_nSpeed);
			NextStep( exe_hand_z_safety_move );
		}break;
		// picker up and move z to satety
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
			
			NextStepTimer( exe_hand_z_safety_move_check, MOTOR_TIME_OUT_LONG );
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
				NextStep(exe_hand_xy_move_target_position);
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
	case exe_hand_xy_move_target_position:
		{
			int nAxisNo[2] = {CTaskTrayUnloadPP::eAxisX, CTaskTrayUnloadPP::eAxisY};

			double dTPos[2] = {0.0,};
			dTPos[0] = g_VatBacklash.trayunloadpp.m_BacklashTargetPos_XYZ.x;
			dTPos[1] = g_VatBacklash.trayunloadpp.m_BacklashTargetPos_XYZ.y;

			pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [x, y, Xpitch, Ypitch Start Position move] [SRT]", m_szCmdName, m_nStep);
			for(int i=0; i<2; i++){
				int nErr = pTaskTrayUnloadPP->do_MovePos_Pcd_backlash(m_Cmd.vPicker, dTPos[i], (CTaskTrayBasePP::eMotor)nAxisNo[i], m_nSpeed, 0, m_bBacklashTest);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}
			NextStepTimer( exe_hand_xy_move_target_position_check, MOTOR_TIME_OUT_LONG );
		}break;

	case exe_hand_xy_move_target_position_check:
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
				pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [x, y, Xpitch, Ypitch Start Position move] [END]", m_szCmdName, m_nStep);

				m_nRetryCount = 0;
				NextStep( exe_hand_z_move_target_safety_position );
			}
		}break;
	case exe_hand_z_move_target_safety_position:
		{
			pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [move safety and picker all up] [SRT]", m_szCmdName, m_nStep);
			int nErr = pTaskTrayUnloadPP->do_MovePos(eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY, m_Cmd.Pocket, m_Cmd.vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}

			m_tcTimeout.SetCheckTime(TIME_OUT_SHORT);
			m_tcTimeout.StartTimer();

			NextStepTimer( exe_hand_z_move_target_safety_position_check, 30.0 );
		}break;
	case exe_hand_z_move_target_safety_position_check:
		{
			std::vector<CPoint> vPkr;
			pTaskTrayUnloadPP->MakeAllPicker(vPkr);
			int nErrSen = pTaskTrayUnloadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);
			m_vErrPicker.clear();
			int nAxisDone = pMotorZ->AxisDone(vPkr, m_vErrPicker);
			if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR /*&& nErrPicker == ERR_NO_ERROR */) {
				pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [move safety and picker all up] [END]", m_szCmdName, m_nStep);
				NextStep(exe_hand_xy_move_target_to_start_position);
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
	case exe_hand_xy_move_target_to_start_position:
		{
			int nAxisNo[2] = {CTaskTrayUnloadPP::eAxisX, CTaskTrayUnloadPP::eAxisY};

			double dTPos[2] = {0.0,};
			dTPos[0] = g_VatBacklash.trayunloadpp.m_BacklashStartPos_XY.x;
			dTPos[1] = g_VatBacklash.trayunloadpp.m_BacklashStartPos_XY.y;

			pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [x, y, Xpitch, Ypitch start Position move] [SRT]", m_szCmdName, m_nStep);
			for(int i=0; i<2; i++){
				int nErr = pTaskTrayUnloadPP->do_MovePos_Pcd_backlash(m_Cmd.vPicker, dTPos[i], (CTaskTrayBasePP::eMotor)nAxisNo[i], m_nSpeed, 0, m_bBacklashTest);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}
			NextStepTimer( exe_hand_xy_move_target_to_start_position_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_xy_move_target_to_start_position_check:
		{
			if( m_tcTimeout.CheckOverTime() == TRUE ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				ReportAlarm(ERR_TRAY_PP_2_MOTOR_MOVE_TIMEOUT, AlarmData);
				break;
			}

			int nChkComp  = 0;
			int nAxisNo[2] = {CTaskTrayUnloadPP::eAxisX, CTaskTrayUnloadPP::eAxisY};
			for(int i=0; i<2; i++){
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
				int nAxisDone = pMotor->AxisDone();
				if( nAxisDone == ERR_NO_ERROR ){
					nChkComp++;
				}
			}
			if( nChkComp == 2 ){
				pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [x, y, Xpitch, Ypitch start Position move] [END]", m_szCmdName, m_nStep);

				m_nRetryCount = 0;
				NextStep( exe_hand_xy_move_start_to_target_position );
			}
		}break;
	case exe_hand_xy_move_start_to_target_position:
		{
			int nAxisNo[2] = {CTaskTrayUnloadPP::eAxisX, CTaskTrayUnloadPP::eAxisY};

			double dTPos[2] = {0.0,};
			dTPos[0] = g_VatBacklash.trayunloadpp.m_BacklashTargetPos_XYZ.x;
			dTPos[1] = g_VatBacklash.trayunloadpp.m_BacklashTargetPos_XYZ.y;

			pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [x, y, Xpitch, Ypitch Target Position move] [SRT]", m_szCmdName, m_nStep);
			for(int i=0; i<2; i++){
				int nErr = pTaskTrayUnloadPP->do_MovePos_Pcd_backlash(m_Cmd.vPicker, dTPos[i], (CTaskTrayBasePP::eMotor)nAxisNo[i], m_nSpeed, 0, m_bBacklashTest);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}
			NextStepTimer( exe_hand_xy_move_start_to_target_position_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_xy_move_start_to_target_position_check:
		{


			int nChkComp  = 0;
			int nAxisNo[2] = {CTaskTrayUnloadPP::eAxisX, CTaskTrayUnloadPP::eAxisY};
			for(int i=0; i<2; i++){
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
			if( nChkComp == 2 ){
				pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [x, y, Xpitch, Ypitch Target Position move] [END]", m_szCmdName, m_nStep);

				m_nRetryCount = 0;
				NextStep( exe_hand_z_move_start_to_target_position );
			}
		}break;

	case exe_hand_z_move_start_to_target_position:
		{
			double dTPos = g_VatBacklash.trayunloadpp.m_BacklashTargetPos_XYZ.z;
			pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [z move to Target Position] [SRT]", m_szCmdName, m_nStep);

			//int nPickerAxisNum = m_nStandardPicker;
			int nErr = pTaskTrayUnloadPP->do_MovePos_Pcd_backlash(m_Cmd.vPicker, dTPos, CTaskTrayBasePP::eAxisZ, TRAY_UNLOAD_PP_PCD_SPEED);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			NextStepTimer( exe_hand_z_move_start_to_target_position_check, 30.0 );
		}break;
	case exe_hand_z_move_start_to_target_position_check:
		{
			if( m_tcTimeout.CheckOverTime() == TRUE ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				ReportAlarm(ERR_TRAY_PP_2_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData);
				break;
			}
			int nAxisDone = pMotorZ->AxisDone();
			if( nAxisDone == ERR_NO_ERROR ){
				pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [z move to Target Position] [END]", m_szCmdName, m_nStep);

				/*vision align total time check*/
				m_tcTimeoutVsn.SetCheckTime( VISION_TIME_OUT_LONG );
				m_tcTimeoutVsn.StartTimer();
				NextStep( exe_visn_recon_start_to_target_pos );
			}
		}break;
	case exe_visn_recon_start_to_target_pos:
		{
			pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [Backlash Start to Target measurement] [SRT]", m_szCmdName, m_nStep);
			if( is_simulation() ){
				NextStepTimer( exe_visn_recon_start_to_target_pos_check, VISION_TIME_OUT );
				break;
			}
			Sleep(1000);
			g_pVisionTestHand_1->SendMeasure_PcdBacklash(VISION_MEASURE_VAT_ULD_UPPER, VAT_HAND_UNLOAD_TRAYPP );
			NextStepTimer( exe_visn_recon_start_to_target_pos_check, VISION_TIME_OUT );
		}break;
	case exe_visn_recon_start_to_target_pos_check:
		{
			ST_PCD_BACKLASH_RCV_DATA vRcvData;
			/* CheckMeasureResult() 에서 alarm check 처리 */
			if( CheckMeasureResult(vRcvData) == TRUE ){
				pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [Backlash Speed:%d Start To Target measurement dx=%.3f um, dy=%.3f um] [END]", m_szCmdName, m_nStep,m_nSpeed,vRcvData.dX,vRcvData.dY);

				CPointF temp;
				temp.x = vRcvData.dX;
				temp.y = vRcvData.dY;
				m_vStartToTargetOffset.push_back(temp);
				NextStep( exe_hand_z_start_to_target_safety_move );	
			}
		}break;
	case exe_hand_z_start_to_target_safety_move:
		{
			pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [move safety and picker all up] [SRT]", m_szCmdName, m_nStep);
			int nErr = pTaskTrayUnloadPP->do_MovePos(eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY, m_Cmd.Pocket, m_Cmd.vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}

			NextStepTimer( exe_hand_z_start_to_target_safety_move_check, 10.0 );
		}break;
	case exe_hand_z_start_to_target_safety_move_check:
		{
			std::vector<CPoint> vPkr;
			pTaskTrayUnloadPP->MakeAllPicker(vPkr);
			int nErrSen = pTaskTrayUnloadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);
			m_vErrPicker.clear();
			int nAxisDone = pMotorZ->AxisDone(vPkr, m_vErrPicker);
			if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR /*&& nErrPicker == ERR_NO_ERROR */) {
				pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [move safety and picker all up] [END]", m_szCmdName, m_nStep);
				NextStep(exe_hand_xy_move_target_to_end_position);
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
	case exe_hand_xy_move_target_to_end_position:
		{
			int nAxisNo[2] = {CTaskTrayUnloadPP::eAxisX, CTaskTrayUnloadPP::eAxisY};

			double dTPos[2] = {0.0,};
			dTPos[0] = g_VatBacklash.trayunloadpp.m_BacklashEndPos_XY.x;
			dTPos[1] = g_VatBacklash.trayunloadpp.m_BacklashEndPos_XY.y;



			pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [x, y, Xpitch, Ypitch End Position move] [SRT]", m_szCmdName, m_nStep);
			for(int i=0; i<2; i++){
				int nErr = pTaskTrayUnloadPP->do_MovePos_Pcd_backlash(m_Cmd.vPicker, dTPos[i], (CTaskTrayBasePP::eMotor)nAxisNo[i], m_nSpeed, 0, m_bBacklashTest);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}
			NextStepTimer( exe_hand_xy_move_target_to_end_position_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_xy_move_target_to_end_position_check:
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
				pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [x, y, Xpitch, Ypitch End Position move] [END]", m_szCmdName, m_nStep);

				m_nRetryCount = 0;
				NextStep( exe_hand_xy_move_end_to_target_position );
			}
		}break;
	case exe_hand_xy_move_end_to_target_position:
		{
			int nAxisNo[2] = {CTaskTrayUnloadPP::eAxisX, CTaskTrayUnloadPP::eAxisY};

			double dTPos[2] = {0.0,};
			dTPos[0] = g_VatBacklash.trayunloadpp.m_BacklashTargetPos_XYZ.x;
			dTPos[1] = g_VatBacklash.trayunloadpp.m_BacklashTargetPos_XYZ.y;

			pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [x, y, Xpitch, Ypitch Target Position move] [SRT]", m_szCmdName, m_nStep);
			for(int i=0; i<2; i++){
				int nErr = pTaskTrayUnloadPP->do_MovePos_Pcd_backlash(m_Cmd.vPicker,dTPos[i], (CTaskTrayBasePP::eMotor)nAxisNo[i], m_nSpeed, 0, m_bBacklashTest);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}
			NextStepTimer( exe_hand_xy_move_end_to_target_position_check, MOTOR_TIME_OUT_LONG );
		}break;
	case exe_hand_xy_move_end_to_target_position_check:
		{
			int nChkComp  = 0;
			int nAxisNo[2] = {CTaskTrayUnloadPP::eAxisX, CTaskTrayUnloadPP::eAxisY};
			for(int i=0; i<2; i++){
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
				int nAxisDone = pMotor->AxisDone();
				if( nAxisDone == ERR_NO_ERROR ){
					nChkComp++;
				}else if( nAxisDone == -1){
					CParamList AlarmData;
					OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TRAY_PP_2_MOTOR_MOVE_FAIL, AlarmData);
					break;
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
				pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [x, y, Xpitch, Ypitch Target Position move] [END]", m_szCmdName, m_nStep);

				m_nRetryCount = 0;
				NextStep( exe_hand_z_move_end_to_target_position );
			}
		}break;
	case exe_hand_z_move_end_to_target_position:
		{
			double dTPos = g_VatBacklash.trayunloadpp.m_BacklashTargetPos_XYZ.z;
			pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [z move to Target Position] [SRT]", m_szCmdName, m_nStep);
			//int nPickerAxisNum = m_nStandardPicker;
			int nErr = pTaskTrayUnloadPP->do_MovePos_Pcd_backlash(m_Cmd.vPicker, dTPos, CTaskTrayBasePP::eAxisZ, 5);
			if( nErr != ERR_NO_ERROR ){
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			NextStepTimer( exe_hand_z_move_end_to_target_position_check, 30.0 );
		}break;
	case exe_hand_z_move_end_to_target_position_check:
		{
			if( m_tcTimeout.CheckOverTime() == TRUE ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				ReportAlarm(ERR_TRAY_PP_2_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData);
				break;
			}
			int nAxisDone = pMotorZ->AxisDone();
			if( nAxisDone == ERR_NO_ERROR ){
				pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [z move to Target Position] [END]", m_szCmdName, m_nStep);

				/*vision align total time check*/
				m_tcTimeoutVsn.SetCheckTime( VISION_TIME_OUT_LONG );
				m_tcTimeoutVsn.StartTimer();
				NextStep( exe_visn_recon_end_to_target_pos );
			}
		}break;
	case exe_visn_recon_end_to_target_pos:
		{
			pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [Backlash End to Target measurement] [SRT]", m_szCmdName, m_nStep);
			if( is_simulation() ){
				NextStepTimer( exe_visn_recon_end_to_target_pos_check, VISION_TIME_OUT );
				break;
			}
			Sleep(1000);
			g_pVisionTestHand_1->SendMeasure_PcdBacklash(VISION_MEASURE_VAT_ULD_UPPER, VAT_HAND_UNLOAD_TRAYPP);
			NextStepTimer( exe_visn_recon_end_to_target_pos_check, VISION_TIME_OUT );
		}break;

	case exe_visn_recon_end_to_target_pos_check:
		{
			ST_PCD_BACKLASH_RCV_DATA vRcvData;
			/* CheckMeasureResult() 에서 alarm check 처리 */
			if( CheckMeasureResult(vRcvData) == TRUE ){
				pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [Backlash Speed:%d Start To Target measurement dx=%.3f um, dy=%.3f um] [END]", m_szCmdName, m_nStep,m_nSpeed, vRcvData.dX,vRcvData.dY);

				CPointF temp;
				temp.x = vRcvData.dX;
				temp.y = vRcvData.dY;
				m_vEndToTargetOffset.push_back(temp);
				NextStep( exe_hand_z_end_to_target_safety_move );	
			}
		}break;
	case exe_hand_z_end_to_target_safety_move:
		{
			pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [z move to safety position] [SRT]", m_szCmdName, m_nStep);

			int nErr = pTaskTrayUnloadPP->do_MovePos(eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY, m_Cmd.Pocket, m_Cmd.vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			NextStepTimer( exe_hand_z_end_to_target_safety_move_check, 30.0 );
		}break;
	case exe_hand_z_end_to_target_safety_move_check:
		{
			std::vector<CPoint> vPkr;
			pTaskTrayUnloadPP->MakeAllPicker(vPkr);
			int nErrSen = pTaskTrayUnloadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);
			m_vErrPicker.clear();
			int nAxisDone = pMotorZ->AxisDone(vPkr, m_vErrPicker);
			if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR /*&& nErrPicker == ERR_NO_ERROR */) {
				pTaskTrayUnloadPP->MakeLog("[%s] [EXEC=%8d] [move safety and picker all up] [END]", m_szCmdName, m_nStep);
				NextStep(exe_backlash_loop_entry);
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
			/* notify complete */

			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray Unload PP Backlash complete");

			if(m_bBacklashTest)
			{
				int nLength = m_vBacklashAdjust.size();
				for(int i=0; i<nLength; i++)
				{
					double dOffset = 0.0;
					CString strBacklashOfs = _T("");
					strBacklashOfs = GetBacklashAxisInfo(m_nAxisNum, i, m_nAxisNum == eAxis_X? m_vBacklashAdjust[i].x:m_vBacklashAdjust[i].y);
					AlarmData.m_fnAddItemFormat("%s", strBacklashOfs);
				}
				
			}
			m_pParentWnd->ReportBacklashDone();
			g_Error.AlarmReport(ERR_VISION_AUTOTEACHING_DONE, AlarmData, &g_TaskSystemCtrl);
			ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
		}break;;
	}
	return CBaseSeqCmd::State_Executing();
}

//==============================================================================
//
//==============================================================================
BOOL CTrayUnloadPPCmd_VatBacklash::State_Pause()
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
BOOL CTrayUnloadPPCmd_VatBacklash::State_Recovery()
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
			int nErr = pTaskTrayUnloadPP->do_MovePos(eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY, m_Cmd.Pocket, m_Cmd.vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
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
					ReportAlarm(ERR_TRAY_PP_2_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData);
					break;
				}
			}
		}break;
	case 1002:
		{
			if (m_tcTimeoutPicker.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				ReportAlarm(ERR_TRAY_PP_2_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData);
				break;
			}

			int nChk = pTaskTrayUnloadPP->Chk_Z_Safety_Sen_All();
			if (nChk == ERR_NO_ERROR) {
				pTaskTrayUnloadPP->MakeLog("[%s RECOVERY=%8d] [move safety and picker all up] [END]", m_szCmdName, m_nStep);
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
BOOL CTrayUnloadPPCmd_VatBacklash::State_NormalComplete()
{
	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CTrayUnloadPPCmd_VatBacklash::State_Alarm()
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
BOOL CTrayUnloadPPCmd_VatBacklash::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete(); 
}

//==============================================================================
//
//==============================================================================
BOOL CTrayUnloadPPCmd_VatBacklash::State_Aborting()
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
BOOL CTrayUnloadPPCmd_VatBacklash::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete(); 
}


//==============================================================================
// local implementation
//==============================================================================
void CTrayUnloadPPCmd_VatBacklash::NextStep( execute_step_define next )
{
	m_nStep = next;
}

void CTrayUnloadPPCmd_VatBacklash::NextStepTimer( execute_step_define next , double tm )
{
	m_tcTimeout.SetCheckTime( tm );
	m_tcTimeout.StartTimer();
	NextStep( next );
}

void CTrayUnloadPPCmd_VatBacklash::SimpleAlarm( int alarm_id )
{
	CParamList AlarmData;
	ReportAlarm( alarm_id, AlarmData );
}

BOOL CTrayUnloadPPCmd_VatBacklash::CheckMeasureResult( ST_PCD_BACKLASH_RCV_DATA& result )
{
	if( m_tcTimeout.CheckOverTime() == TRUE ){
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Tray PP WebCam receive data time over!!");
		ReportAlarm(ERR_TRAY_PP_VISION_CMD_RESP_TIME_OUT, AlarmData);
		return FALSE;
	}

	/*vision align total time check*/
	//if( m_tcTimeoutVsn.CheckOverTime() == TRUE ){
	//	CParamList AlarmData;
	//	AlarmData.m_fnAddItemFormat("Tray PP WebCam retry time over!!");
	//	ReportAlarm(ERR_VISION_CMD_RETRY_COUNT_OVER, AlarmData);
	//	return FALSE;
	//}
	
	g_pVisionTestHand_1->GetReceiveData((VAT_HAND_UNLOAD_TRAYPP), result);
		
	if( is_simulation() && m_tcTimeout.MoreThan( 100 /*ms*/ ) ) {
		int rand_range = 500;
		result.dX = ((rand()%rand_range*2)-rand_range);
		result.dY = ((rand()%rand_range*2)-rand_range);
		result.nState = CAM_STATUS_SUCCESS;
	}

	if( result.nState != CAM_STATUS_IDLE){
		int nErrCnt = 0; 
		CParamList AlarmData;
		switch(result.nState)
		{
		case CAM_STATUS_DISCONNECT	 : { AlarmData.m_fnAddItemFormat("Tray PP WebCam disconnect!!"); nErrCnt++; } break;
		case CAM_STATUS_TIMEOUT		 : { AlarmData.m_fnAddItemFormat("Tray PP WebCam time out!!"  ); nErrCnt++; } break;
		case CAM_STATUS_FAILED		 : { AlarmData.m_fnAddItemFormat("Tray PP WebCam failed!!"    ); nErrCnt++; } break;
		case CAM_STATUS_UNKNOWN		 : { AlarmData.m_fnAddItemFormat("Tray PP WebCam unknown!!"   ); nErrCnt++; } break;
		case CAM_STATUS_LIGHT_FAILED : { AlarmData.m_fnAddItemFormat("Tray PP WebCam light fail!!"); nErrCnt++; } break;	
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

void CTrayUnloadPPCmd_VatBacklash::CalcBacklashOffset(int nSpeed, BOOL bIsBacklashTest)
{
	CPointF Temp;
	//by kmk [20190222] 기존 소스는 End - Start로 Backlash를 구함(Vision 프로그램)
	//by kmk [20190222] 추후 Start - End로 Backlash 계산을 통일할 예정
	for(int i=0; i<(int)m_vStartToTargetOffset.size(); i++){
// 				Temp.x += ( m_vStartToTargetOffset[i].x - m_vEndToTargetOffset[i].x );
// 				Temp.y += ( m_vStartToTargetOffset[i].y - m_vEndToTargetOffset[i].y );

		Temp.x += ( m_vEndToTargetOffset[i].x - m_vStartToTargetOffset[i].x  );
		Temp.y += ( m_vEndToTargetOffset[i].y - m_vStartToTargetOffset[i].y  );
	}

	Temp.x = Temp.x / (int)m_vStartToTargetOffset.size();
	Temp.y = Temp.y / (int)m_vStartToTargetOffset.size();

	if(bIsBacklashTest == false)
	{
		int nIdx = 0;

		if(nSpeed > 5){
			nIdx = nSpeed / 10;
		}

		if(nIdx < eSpeed_MAX){
			// by jhRyu [20181029] Hand 보정치는 발생한 Backlash 값 * -1. (가변열에 작착할 때) -> Tray PP Front Left 장착
			//by kmk [20190104] Hand 보정치는 발생한 Backlash 값 그대로(고정열 장착) -> Tray PP Rear Right
			if(m_nAxisNum == eAxis_X){
				g_VatBacklash.trayunloadpp.m_BacklashOffset_XY[nIdx].x = Temp.x /** -1*/;
				m_BacklashXTestLog.LOG(" SPD -> %3d / OFFSET -> %.5f", nSpeed, g_VatBacklash.trayunloadpp.m_BacklashOffset_XY[nIdx].x);
			}else if(m_nAxisNum == eAxis_Y){
				g_VatBacklash.trayunloadpp.m_BacklashOffset_XY[nIdx].y = Temp.y /** -1*/;
				m_BacklashYTestLog.LOG(" SPD -> %3d / OFFSET -> %.5f", nSpeed, g_VatBacklash.trayunloadpp.m_BacklashOffset_XY[nIdx].y);
			}

		}
	}
	else
	{
		m_vBacklashAdjust.push_back(Temp);
	}
	
}

CString CTrayUnloadPPCmd_VatBacklash::GetBacklashAxisInfo(int nAxis, int nSpdIndex, double dOffset)
{
	CString str = _T("");
	int nSpd = 0;
	if(nSpdIndex == 0)
	{
		nSpd = 5;
	}
	else
	{
		nSpd = nSpdIndex * 10;
	}
	str.Format(_T("Axis : %s, Speed : %d, Offset : %.3fmm"), nAxis== eAxis_X? _T("X") : _T("Y"), nSpd, dOffset/1000.0);
	return str;
}

void CTrayUnloadPPCmd_VatBacklash::InitLoopStatus()
{
	for(int i =eSpeed_5; i < eSpeed_MAX; i++ )
	{
		if(i == eSpeed_5)
		{
			m_qSpeed.push_back(5);
		}
		else
		{
			m_qSpeed.push_back(i*10);
		}
	}

	m_nRepeatTempCnt--;
	if(m_nRepeatTempCnt <= 0 )
	{
		m_nAxisNum = m_nAxisNum == eAxis_X ? eAxis_Y : eAxis_X;
		m_nRepeatTempCnt = m_nRepeatCnt;
	}
}

void CTrayUnloadPPCmd_VatBacklash::WriteRepeatOffsetXY(std::vector<CPointF> End2TargetOfs, std::vector<CPointF> Start2TargetOfs, int spd)
{
	if(m_bRepeat == FALSE)
	{
		return;
	}
	
	double dOffset = 0.0;
	
	CString strFilePath;
	CString strFileName;
	CString strAxis = _T("X");
	if (m_nAxisNum == eAxis_Y)
		strAxis = _T("Y");

	strFileName.Format(_T("%04d%02d%02d_BacklashData%s_%d.csv"), m_cTime.GetYear(),
													m_cTime.GetMonth(), m_cTime.GetDay(), (LPCTSTR)strAxis, spd);

	strFilePath = BACKLASH_LOG_PATH + strFileName;

	CFile BacklashFile;
	BacklashFile.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	//by kmk [20190222] 기존 소스는 End - Start로 Backlash를 구함(Vision 프로그램)
	//by kmk [20190222] 추후 Start - End로 Backlash 계산을 통일할 예정

	int nLength = End2TargetOfs.size();
	for(int i=0; i<nLength; i++)
	{

		char szData2[256] = { 0, };
		if(m_nAxisNum == eAxis_X)
		{
			dOffset = (End2TargetOfs[i].x - Start2TargetOfs[i].x);
			/*dOffset = (Start2TargetOfs[i].x - End2TargetOfs[i].x);*/
			sprintf_s(szData2, sizeof(szData2), "%.5f,%.5f,%.5f\r\n",Start2TargetOfs[i].x, End2TargetOfs[i].x, dOffset);
		}
		else
		{
			/*dOffset = (Start2TargetOfs[i].y - End2TargetOfs[i].y);*/
			dOffset = (End2TargetOfs[i].y - Start2TargetOfs[i].y);
			sprintf_s(szData2, sizeof(szData2), "%.5f,%.5f,%.5f\r\n",Start2TargetOfs[i].y, End2TargetOfs[i].y, dOffset);
		}
		BacklashFile.SeekToEnd();
		BacklashFile.Write(szData2, strlen(szData2));

	}
	BacklashFile.Close();
	
}

void CTrayUnloadPPCmd_VatBacklash::WriteMotorEncoder(int speed)
{
	OneAxis* pMotorX  = g_TaskTrayUnloadPP.GetAxis(CTaskTrayUnloadPP::eAxisX);
	OneAxis* pMotorY  = g_TaskTrayUnloadPP.GetAxis(CTaskTrayUnloadPP::eAxisY);

	CTime cTime = CTime::GetCurrentTime(); // 현재 시스템으로부터 날짜 및 시간을 얻어 온다.
	char szData2[256] = { 0, };
	CString strFileName;
	CString strFilePath;
	strFileName.Format(_T("%04d%02d%02d_Motor_Enc.csv"), cTime.GetYear(),
														 cTime.GetMonth(), cTime.GetDay());
	strFilePath = BACKLASH_LOG_PATH + strFileName;

	CFile MotorEnc;
	MotorEnc.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	double dCurPosX=0.0, dCurPosY = 0.0;
	pMotorX->GetCurrentPos(dCurPosX);
	pMotorY->GetCurrentPos(dCurPosY);
	sprintf_s(szData2, sizeof(szData2), "%.5f,%.5f\r\n", dCurPosX, dCurPosY);
	MotorEnc.SeekToEnd();
	MotorEnc.Write(szData2, strlen(szData2));
	MotorEnc.Close();

}
