#include "StdAfx.h"
#include "TrayUnloadPPCmd_MovePickPlace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTrayUnloadPPCmd_MovePickPlace_Param::CTrayUnloadPPCmd_MovePickPlace_Param()
{
	eStage = LOC_IDX_TRAY_PP_Undefined;
	//Pocket;
	vPicker.clear();
	vSpecOutPicker.clear();
	vFullCmdPkr.clear();
	ePickPlace = ePick;
	bNormalPkrEnd = FALSE;
	bSpeckOutPkrEnd = FALSE;
	nCntSpecOut = 0;
	bSpecOutPkrUse = FALSE;
	bVacuumCheckFuction = FALSE;
}

CTrayUnloadPPCmd_MovePickPlace_Param::CTrayUnloadPPCmd_MovePickPlace_Param(
	eLocIdxTrayPP       _eStage,
	CPoint              _Pocket,
	std::vector<CPoint> _vPicker,
	std::vector<CPoint> _vSpecOutPicker,
	std::vector<CPoint> _vFullCmdPkr,
	ePPCmd              _ePick,	    // 0=Pick, 1=Place
	BOOL				_bNormalPkrEnd,
	BOOL				_bSpeckOutPkrEnd,
	int				    _nCntSpecOut,
	BOOL				_bSpecOutPkrUse,
	BOOL				_bVacuumCheckFuction
)
{
	eStage          = _eStage;
	Pocket          = _Pocket;
	vPicker         = _vPicker;
	vSpecOutPicker  = _vSpecOutPicker;
	vFullCmdPkr     = _vFullCmdPkr;
	ePickPlace	    = _ePick;
	bNormalPkrEnd   = _bNormalPkrEnd;
	bSpeckOutPkrEnd = _bSpeckOutPkrEnd;
	nCntSpecOut     = _nCntSpecOut;
	bSpecOutPkrUse  = _bSpecOutPkrUse;
	bVacuumCheckFuction = _bVacuumCheckFuction;
}

CTrayUnloadPPCmd_MovePickPlace_Param::CTrayUnloadPPCmd_MovePickPlace_Param(const CTrayUnloadPPCmd_MovePickPlace_Param& src)
{
	eStage = src.eStage;
	Pocket = src.Pocket;
	vPicker = src.vPicker;
	vSpecOutPicker = src.vSpecOutPicker;
	vFullCmdPkr = src.vFullCmdPkr;
	ePickPlace = src.ePickPlace;
	bNormalPkrEnd = src.bNormalPkrEnd;
	bSpeckOutPkrEnd = src.bSpeckOutPkrEnd;
	nCntSpecOut = src.nCntSpecOut;
	bSpecOutPkrUse = src.bSpecOutPkrUse;
	bVacuumCheckFuction = src.bVacuumCheckFuction;
}

CTrayUnloadPPCmd_MovePickPlace_Param& CTrayUnloadPPCmd_MovePickPlace_Param::operator=(const CTrayUnloadPPCmd_MovePickPlace_Param& src)
{
	if (this == &src) return *this;

	eStage = src.eStage;
	Pocket = src.Pocket;
	vPicker = src.vPicker;
	vSpecOutPicker = src.vSpecOutPicker;
	vFullCmdPkr = src.vFullCmdPkr;
	ePickPlace = src.ePickPlace;
	bNormalPkrEnd = src.bNormalPkrEnd;
	bSpeckOutPkrEnd = src.bSpeckOutPkrEnd;
	nCntSpecOut = src.nCntSpecOut;
	bSpecOutPkrUse = src.bSpecOutPkrUse;
	bVacuumCheckFuction = src.bVacuumCheckFuction;

	return *this;
}

CTrayUnloadPPCmd_MovePickPlace_Param::~CTrayUnloadPPCmd_MovePickPlace_Param()
{

}

CTrayUnloadPPCmd_MovePickPlace::CTrayUnloadPPCmd_MovePickPlace()
{
	m_nCmdId = eSeqCmd_TrayUnloadPP_MovePickPlace;
	m_nRetryCount = 0;
	m_nSetRetryCount = 0;
	m_dWaitTime = 0.2;
	m_bDownCheckEnable = FALSE;
	m_dDownCheckTimeout = 5.0;
	sprintf_s(m_szCmdName, sizeof(m_szCmdName), "TrayUnloadPPCmd_MovePickPlace");

	for (int i = 0; i < CTaskTrayBasePP::eAxisMax; i++) {
		m_dInPosRange[i] = MOTOR_INPOSITION;      // [um]
	}

	m_tcTimeout = 0.0;
	m_tcTimeoutPicker = 0.0;
}

CTrayUnloadPPCmd_MovePickPlace::CTrayUnloadPPCmd_MovePickPlace( CTrayUnloadPPCmd_MovePickPlace_Param _Prm  )
	: m_Cmd(_Prm)
{
	m_nCmdId         = eSeqCmd_TrayUnloadPP_MovePickPlace;
	m_nRetryCount    = 0;
	m_nSetRetryCount = 0;
	m_dWaitTime          = 0.2;
	m_bDownCheckEnable   = FALSE;
	m_dDownCheckTimeout  = 5.0;
	sprintf_s(m_szCmdName, sizeof(m_szCmdName), "TrayUnloadPPCmd_MovePickPlace");

	for (int i = 0; i < CTaskTrayBasePP::eAxisMax; i++) {
		m_dInPosRange[i] = MOTOR_INPOSITION;      // [um]
	}

	m_tcTimeout       = 0.0;
	m_tcTimeoutPicker = 0.0;
}

CTrayUnloadPPCmd_MovePickPlace::~CTrayUnloadPPCmd_MovePickPlace( void )
{
}

int CTrayUnloadPPCmd_MovePickPlace::CheckParam()
{
	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;

	m_nSetRetryCount = g_DMCont.m_dmTrayPP.GN(NDM5_LDTrayPP_RetryCnt);

	m_nWorkCmdStage  = m_Cmd.eStage;

	m_TransferInterlock.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
	m_TransferInterlock.SetTimerCheckEnd();

	if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
	{
		if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto && m_nWorkCmdStage >= LOC_IDX_TRAY_PP_CONV_1 && m_nWorkCmdStage <= LOC_IDX_TRAY_PP_CONV_7) {
			g_TaskTrayUnloadPP.chk_TrayUnload_Y_Safety_sen(DEF_OFF);
		}
		else {
			g_TaskTrayUnloadPP.chk_TrayUnload_Y_Safety_sen(DEF_ON);
		}
	}

	return ERR_NO_ERROR;
}

int CTrayUnloadPPCmd_MovePickPlace::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

int CTrayUnloadPPCmd_MovePickPlace::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

int CTrayUnloadPPCmd_MovePickPlace::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

int CTrayUnloadPPCmd_MovePickPlace::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

int CTrayUnloadPPCmd_MovePickPlace::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

BOOL CTrayUnloadPPCmd_MovePickPlace::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}

BOOL CTrayUnloadPPCmd_MovePickPlace::State_Executing()
{
	// check alarm
	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}

	m_nStep_Pre = m_nStep;

	for (int i = 0; i < CTaskTrayBasePP::eAxisMax; i++) {
		OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis(static_cast<CTaskTrayBasePP::eMotor>(i));
		if (pMotor != nullptr){
			if (pMotor->ChkMotorUsable() != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Tray Unload PP check motor usable command error");
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(ERR_TRAY_PP_2_MOTOR_ALARMED, AlarmData);
				return FALSE;
			}
		}
	}

	if (LOC_IDX_TRAY_PP_CONV_1 <= m_Cmd.eStage && m_Cmd.eStage <= LOC_IDX_TRAY_PP_CONV_7) {
		if (g_TaskTransfer.is_safety_state() == false) {

			if (m_TransferInterlock.IsChecking() == FALSE){
				m_TransferInterlock.StartTimer();
			}

			if (m_TransferInterlock.CheckOverTime() == TRUE){
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

	int nCntSpecOut = 0;
	switch (m_nStep)
	{
	case 0: // move z axis safety & picker all up
	{
		std::vector<CPoint> vPkr;
		pTaskTrayUnloadPP->MakeAllPicker(vPkr);
		pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [z move safety & picker all up] [SRT]", m_szCmdName, m_nStep);
		int nErr = pTaskTrayUnloadPP->do_MovePos(m_Cmd.eStage, m_Cmd.Pocket, vPkr, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray Unload PP z axis move safety position command error");
			OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(nErr, AlarmData);
			break;
		}
		m_tcTimeout = (double)GetTickCount64();//check time picker up
		m_nStep = 3000;
	}break;

	case 3000:	// check move z safety
	{
		std::vector<CPoint> vPkr;
		pTaskTrayUnloadPP->MakeAllPicker(vPkr);
		int nErrSen = pTaskTrayUnloadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);
		m_vErrPicker.clear();
		int nAxisDone = pMotorZ->AxisDone(vPkr, m_vErrPicker);
		if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR /*&& nErrPicker == ERR_NO_ERROR */) {
			pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [z move safety & picker all up] [END]", m_szCmdName, m_nStep);
			m_nRetryCount = 0;
			m_nStep = 4000;
			break;
		}
		else
		{
			std::vector<CPoint> vErrPkr; vErrPkr.clear();
			CString sAlarmData = _T("");
			int nErr = pTaskTrayUnloadPP->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, pTaskTrayUnloadPP->GetSafetyFailPicker(ePickerSafetySensorCheckType::eSafetySensorCheck), vErrPkr, sAlarmData);

			double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
			if (dSpanTime > (TIME_OUT_SHORT*1000.0)) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("%s", sAlarmData);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				ReportAlarm(nErr, AlarmData, vErrPkr);
				break;
			}
		}
	}break;

	case 4000:	// move x,y,xp,yp to target pos
	{
		double dTPos[4] = { 0.0, };
		dTPos[0] = pTaskTrayUnloadPP->GetTeachPos(m_Cmd.eStage, m_Cmd.Pocket, m_Cmd.vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX);
		dTPos[1] = pTaskTrayUnloadPP->GetTeachPos(m_Cmd.eStage, m_Cmd.Pocket, m_Cmd.vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisY);
		dTPos[2] = pTaskTrayUnloadPP->GetTeachPos(m_Cmd.eStage, m_Cmd.Pocket, m_Cmd.vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisPitchX);
		dTPos[3] = pTaskTrayUnloadPP->GetTeachPos(m_Cmd.eStage, m_Cmd.Pocket, m_Cmd.vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisPitchY);
		//m_Cmd.Picker를 m_vFullPicker

		int nAxisNo[4] = { CTaskTrayBasePP::eAxisX, CTaskTrayBasePP::eAxisY, CTaskTrayBasePP::eAxisPitchX, CTaskTrayBasePP::eAxisPitchY };
		pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [x, y, Xpitch, Ypitch move %s x:%d / y:%d] [SRT]", m_szCmdName, m_nStep, pTaskTrayUnloadPP->GetLocName(m_Cmd.eStage), m_Cmd.Pocket.x + 1, m_Cmd.Pocket.y + 1);
		int nErr = -1;
		for (int i = 0; i < 4; i++) {
			if (m_Cmd.bSpecOutPkrUse == TRUE)
			{
				if (m_Cmd.bNormalPkrEnd == FALSE)
				{
					if (m_Cmd.ePickPlace == DEF_PICK) {
						nErr = pTaskTrayUnloadPP->do_MovePos(m_Cmd.eStage, m_Cmd.Pocket, m_Cmd.vPicker, CTaskTrayBasePP::eZPOS_SAFETY, (CTaskTrayBasePP::eMotor)nAxisNo[i]);
					}
					else {
						nErr = pTaskTrayUnloadPP->do_MovePos(m_Cmd.eStage, m_Cmd.Pocket, pTaskTrayUnloadPP->m_vFullPkr, CTaskTrayBasePP::eZPOS_SAFETY, (CTaskTrayBasePP::eMotor)nAxisNo[i]);
					}
					//Loading Tbl, C-Tray 위치로 이동 할 때는 Specout된 Picker(m_Cmd.Picker, m_vSpecOutPicker 분리)가 있더라도 전체 m_vFullPkr경로로 줘야 Pocket 제 위치로 이동 
				}
				else  //Spec Out Picker Exist
				{
					if (pTaskTrayUnloadPP->GetPickerAlignStatus(m_Cmd.vSpecOutPicker[m_Cmd.nCntSpecOut - 1].x, m_Cmd.vSpecOutPicker[m_Cmd.nCntSpecOut - 1].y) == TRUE)
					{
						double dOffsetX = 0.0, dOffsetY = 0.0;
						pTaskTrayUnloadPP->GetPickerOffset(dOffsetX, dOffsetY, m_Cmd.vSpecOutPicker[m_Cmd.nCntSpecOut - 1].x, m_Cmd.vSpecOutPicker[m_Cmd.nCntSpecOut - 1].y);
						if (CTaskTrayBasePP::eAxisX == 0)		// x offset 추가
						{
							nErr = pTaskTrayUnloadPP->do_MovePos(m_Cmd.eStage, m_Cmd.Pocket, pTaskTrayUnloadPP->m_vFullPkr, CTaskTrayBasePP::eZPOS_SAFETY, (CTaskTrayBasePP::eMotor)nAxisNo[i], 100, dOffsetX);
						}
						else if (CTaskTrayBasePP::eAxisY == 1) // y offset 추가
						{
							nErr = pTaskTrayUnloadPP->do_MovePos(m_Cmd.eStage, m_Cmd.Pocket, pTaskTrayUnloadPP->m_vFullPkr, CTaskTrayBasePP::eZPOS_SAFETY, (CTaskTrayBasePP::eMotor)nAxisNo[i], 100, dOffsetY);
						}
						else
						{
							nErr = pTaskTrayUnloadPP->do_MovePos(m_Cmd.eStage, m_Cmd.Pocket, pTaskTrayUnloadPP->m_vFullPkr, CTaskTrayBasePP::eZPOS_SAFETY, (CTaskTrayBasePP::eMotor)nAxisNo[i]);
						}
					}
				}
			}
			else   //SpecOut Picker Option Not Use
			{
				nErr = pTaskTrayUnloadPP->do_MovePos(m_Cmd.eStage, m_Cmd.Pocket, m_Cmd.vPicker, CTaskTrayBasePP::eZPOS_SAFETY, (CTaskTrayBasePP::eMotor)nAxisNo[i]);
			}
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Tray PP 4axis(x,y,x-pitch,y-pitch) move command error");
				AlarmData.m_fnAddItemFormat("Problem Axis = %d, Error=%d, CmdStage=%d, CmdPocket=(X:%d,Y:%d)", nAxisNo[i], nErr, m_Cmd.eStage, m_Cmd.Pocket.x + 1, m_Cmd.Pocket.y + 1);
				OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
		}
		m_tcTimeout = (double)GetTickCount64();
		m_nStep = 6000;
	}break;
	case 6000: // check x,y,xp,yp axis done
	{
		int nAxisNo[4] = { CTaskTrayBasePP::eAxisX, CTaskTrayBasePP::eAxisY, CTaskTrayBasePP::eAxisPitchX, CTaskTrayBasePP::eAxisPitchY };


		int nChkComp = 0;
		for (int i = 0; i < 4; i++) {
			OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eMotor(nAxisNo[i]));
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				nChkComp++;
			}
			else
			{
				double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
				if (dSpanTime > (MOTOR_TIME_OUT_MIDDLE*1000.0)) {
					CString strErrAxis = _T("");
					for (int i = 0; i < 4; i++) {
						OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eMotor(nAxisNo[i]));
						int nAxisDone = pMotor->AxisDone();
						if (nAxisDone != NO_ERROR) {
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
					AlarmData.m_fnAddItemFormat("Tray PP 4axis operation is not complete");
					AlarmData.m_fnAddItemFormat("%s motor time over!", strErrAxis);
					AlarmData.m_fnAddItemFormat("[current position] x=%.2f, y=%.2f, x-pitch=%.2f, y-pitch=%.2f", dPosX / 1000.0, dPosY / 1000.0, dPosPX / 1000.0, dPosPY / 1000.0);
					ReportAlarm(ERR_TRAY_PP_2_MOTOR_MOVE_TIMEOUT, AlarmData);
					break;
				}
				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Tray PP 4axis operation is not complete");
					AlarmData.m_fnAddItemFormat("[ check axis done ] x=%d, y=%d, x-pitch=%d, y-pitch=%d", pMotorX->AxisDone(), pMotorY->AxisDone(), pMotorPX->AxisDone(), pMotorPY->AxisDone());
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TRAY_PP_2_MOTOR_MOVE_FAIL, AlarmData);
					break;
				}
			}
		}

		if (nChkComp == 4) {
			pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [x, y, Xpitch, Ypitch move %s x:%d / y:%d] [END]", m_szCmdName, m_nStep, pTaskTrayUnloadPP->GetLocName(m_Cmd.eStage), m_Cmd.Pocket.x + 1, m_Cmd.Pocket.y + 1);

			// waiting for loadtable
			if (LOC_IDX_TRAY_PP_TBL_1 == m_Cmd.eStage || m_Cmd.eStage == LOC_IDX_TRAY_PP_TBL_4 && CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto) {
				m_nStep = 7000; // checking loop
				m_tcTimeout = (double)GetTickCount64();
				pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [Wait %s] [SRT]", m_szCmdName, m_nStep, pTaskTrayUnloadPP->GetLocName(m_Cmd.eStage));
				break;
			}
			// command stage is not loadtable
			m_nStep = 10000;
		}
	}break;

	//------------------------------------------------------------------------------------------
	case 7000: // waiting for load table
	{
		int nLoadTblIndex = m_Cmd.eStage - LOC_IDX_TRAY_PP_TBL_1;

		//check time over
		double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
		if (dSpanTime > (MOTOR_TIME_OUT_MIDDLE*1000.0)) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray PP waiting for  %s time over", pTaskTrayUnloadPP->GetLocName(m_Cmd.eStage));
			ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + nLoadTblIndex, AlarmData);
			break;
		}

		// checking load tbl position
		int nRetLoadTblPos = g_TaskLoadTbl[nLoadTblIndex].ChkMotorPos(LOC_IDX_TBL_LD_SIDE, CTaskLoadTable::eAxisAll);
		if (nRetLoadTblPos == ERR_NO_ERROR && g_TaskLoadTbl[nLoadTblIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready) {
			pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [Wait %s] [END]", m_szCmdName, m_nStep, pTaskTrayUnloadPP->GetLocName(m_Cmd.eStage));
			m_nStep = 10000;
			break;
		}
	}break;

	//------------------------------------------------------------------------------------------
	case  10000: // move z target pos
	{
		int nCylinderWaitTm = g_DMCont.m_dmTrayPP.GN(NDM5_TM_LDTrayPP_CylDownWait);
		double dSpanTime = (double)GetTickCount64() - m_tcTimeoutPicker;
		int nRemainTime = nCylinderWaitTm - (int)dSpanTime;

		if (m_Cmd.ePickPlace == DEF_PLACE && CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto)
		{
			pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [Before blow check device] [STR]", m_szCmdName, m_nStep);
			CString strMsg = _T("");
			std::vector<CString> vErrString; vErrString.clear();
			int nChkErr = pTaskTrayUnloadPP->CheckDevice(strMsg, m_Cmd.vPicker, vErrString);
			if (nChkErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				if (strMsg.IsEmpty() != TRUE) { AlarmData.m_fnAddItemFormat(strMsg); }
				for (int i = 0; i < (int)vErrString.size(); i++)
				{
					AlarmData.m_fnAddItemFormat("%s", vErrString[i]);
				}
				ReportAlarm(nChkErr, AlarmData, pTaskTrayUnloadPP->GetFailPicker());
				break;
			}
			pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [Before blow check device] [END]", m_szCmdName, m_nStep);
		}

		// move z-axis
		CTaskTrayBasePP::eTeachPosZ eZPos = (m_Cmd.ePickPlace == DEF_PICK) ? (CTaskTrayBasePP::eZPOS_PICK) : (CTaskTrayBasePP::eZPOS_PLACE);
		int nErr = -1;
// 		if (m_Cmd.ePickPlace == DEF_PICK) {
// 			double dTrayPP_Pick_1Stage_Offset = (double)g_DMCont.m_dmTrayPP.GN(NDM5_TM_LDTrayPP_Pick_1Stage_Offset);
// 			double dTPos = pTaskTrayUnloadPP->GetTeachPos(m_Cmd.eStage, m_Cmd.Pocket, m_Cmd.vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
// 			nErr = pTaskTrayUnloadPP->do_MovePos(m_Cmd.eStage, m_Cmd.Pocket, m_Cmd.vPicker, eZPos, CTaskTrayBasePP::eAxisZ, 100, dTPos - dTrayPP_Pick_1Stage_Offset);
// 		}
// 		else
		if (m_Cmd.ePickPlace == DEF_PICK)
		{
			std::vector<CPoint> vTargetPicker;
			vTargetPicker.clear();

			vTargetPicker = m_Cmd.vPicker;

			if (pTaskTrayUnloadPP->m_vFailPicker.size() != 0) {
				vTargetPicker = pTaskTrayUnloadPP->m_vFailPicker;
			}
						
			nErr = pTaskTrayUnloadPP->do_MovePos(m_Cmd.eStage, m_Cmd.Pocket, vTargetPicker, eZPos, CTaskTrayBasePP::eAxisZ);
		}
		else {
			nErr = pTaskTrayUnloadPP->do_MovePos(m_Cmd.eStage, m_Cmd.Pocket, m_Cmd.vPicker, eZPos, CTaskTrayBasePP::eAxisZ);
		}
		

		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray PP move z down %s pos cmd Error", (m_Cmd.ePickPlace == DEF_PICK) ? (_T("Pick")) : (_T("Place")));
			OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(nErr, AlarmData);
			break;
		}

		pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [z move target %s pos] [SRT]", m_szCmdName, m_nStep, (m_Cmd.ePickPlace == DEF_PICK) ? (_T("Pick")) : (_T("Place")));
		BOOL bPreVacuum = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TRAY_HAND_PRE_VACUUM);

		if (bPreVacuum == eOPTION_USE && m_Cmd.ePickPlace == DEF_PICK) {
			pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [Pre Vacuum On] [SRT]", m_szCmdName, m_nStep);
			int nErr = pTaskTrayUnloadPP->do_VacBlow(eVacBlwSt_VacOn, m_Cmd.vPicker);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Tray PP vacuum cmd Error");
				ReportAlarm(nErr, AlarmData);
				break;
			}
			pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [Pre Vacuum On] [END]", m_szCmdName, m_nStep);
		}

		m_tcTimeout = (double)GetTickCount64();
		m_nStep = 11000;
	}break;
	case 11000: // checking z axis done
	{
		int nAxisDone = pMotorZ->AxisDone();
		if (nAxisDone == ERR_NO_ERROR) {
			BOOL bPreVacuum = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TRAY_HAND_PRE_VACUUM);

			pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [z move target %s pos] [END]", m_szCmdName, m_nStep, (m_Cmd.ePickPlace == DEF_PICK) ? (_T("Pick")) : (_T("Place")));
			if (g_StepTrayPP_SW[eTrayPP_2_UD].GetStatus() == DEF_ON) {

				if (bPreVacuum == eOPTION_USE && m_Cmd.ePickPlace == DEF_PICK) {
					pTaskTrayUnloadPP->do_VacBlow(eVacBlwSt_BlwOn, m_Cmd.vPicker);
				}

				pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [STEP Working Stop, target %s pos]", m_szCmdName, m_nStep, (m_Cmd.ePickPlace == DEF_PICK) ? (_T("Pick")) : (_T("Place")));
				g_StepTrayPP_SW[eTrayPP_2_UD].SetChangeStatusOff();
				g_StepTrayPP_SW[eTrayPP_2_UD].SetChangeStatusOnOff(DEF_ON);
				SetCmdStatus(eSeqCmdStatus_Alarm);
				g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
			}
			else {
				m_nStep = (m_Cmd.ePickPlace == DEF_PLACE ? eExcuteStep_Do_Blow : 13000);
				break;
			}
		}
		else
		{
			double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
			double dCurPos = 0;
			pMotorZ->GetCurrentPos(dCurPos);
			if (dSpanTime > (MOTOR_TIME_OUT_MIDDLE*1000.0)) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Tray PP z-axis %s pos is not complete", (m_Cmd.ePickPlace == DEF_PICK) ? (_T("Pick")) : (_T("Place")));
				AlarmData.m_fnAddItemFormat("time over! (overtime=%.2f, z-axis current pos=%.2f)", dSpanTime / 1000.0, dCurPos);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				ReportAlarm(ERR_TRAY_PP_2_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData);
				break;
			}
		}
	}break;

	//------------------------------------------------------------------------------------------
	// place blow loop
	case eExcuteStep_Do_Blow:	
	{
		//Place Pos 인대, Z축 Safety Sensor 감지 되면 내려가지 않은 것으로 판단하고 알람 발생 한다.
		int nErrSen = pTaskTrayUnloadPP->Chk_Z_Safety_Sen_Check_Type(m_Cmd.vPicker, ePickerSafetySensorCheckType::ePlaceSafetySensorCheck);
		if (!pTaskTrayUnloadPP->m_vSafetyFailPicker[ePickerSafetySensorCheckType::ePlaceSafetySensorCheck].empty())
		{
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray Unload PP Place Pos Safety Sensing Error %s", pTaskTrayUnloadPP->m_sSaftyFailPicker[ePickerSafetySensorCheckType::ePlaceSafetySensorCheck]);
			ReportAlarm(ERR_TRAY_PP_2_Z_PICKER_PLACE_POS_SAFETY_SENSING, AlarmData, pTaskTrayUnloadPP->GetSafetyFailPicker(ePickerSafetySensorCheckType::ePlaceSafetySensorCheck));
			break;
		}


		pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [Blow On] [SRT]", m_szCmdName, m_nStep);
		int nErr = pTaskTrayUnloadPP->do_VacBlow(eVacBlwSt_BlwOn, m_Cmd.vPicker);
		pTaskTrayUnloadPP->m_bCyclePause = TRUE;
		pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [Blow On] [END]", m_szCmdName, m_nStep);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray PP Blow cmd Error");
			ReportAlarm(nErr, AlarmData);
			break;
		}


		//Sleep(200);
//		pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [Blow Wait] [STR]", m_szCmdName, m_nStep);
// 		int nSleep = g_DMCont.m_dmTrayPP.GN(NDM5_TM_UDTrayPP_Blow);
// 		if (nSleep > 0) {
// 			Sleep(nSleep);
// 		}
//		pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [Blow Wait] [END]", m_szCmdName, m_nStep);


		// write device data
		if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto) {	
			CString strPicker = _T(""), strTmp = _T("");
			pTaskTrayUnloadPP->m_vFailPicker.clear();
			for (std::vector<CPoint>::size_type i = 0; i < (int)m_Cmd.vPicker.size(); i++) {
				strTmp.Format("[X:%d, Y:%d]", m_Cmd.vPicker[i].x + 1, m_Cmd.vPicker[i].y + 1);
				if (pTaskTrayUnloadPP->ChkVacuum(eOnOffState::eOff, m_Cmd.vPicker[i]) != ERR_NO_ERROR) 
				{
					strPicker += strTmp;
					pTaskTrayUnloadPP->m_vFailPicker.push_back(m_Cmd.vPicker[i]);			
				}
			}

			if (pTaskTrayUnloadPP->m_vFailPicker.size() > 0) {
				if (m_nRetryCount < m_nSetRetryCount)
				{
					Sleep(500.0);
					m_nRetryCount++;
					pTaskTrayUnloadPP->MakeLog("[Blow Retry] CmdStage=%s, CmdTargetX=%d, CmdTargetY=%d, %s", pTaskTrayUnloadPP->GetLocName(m_Cmd.eStage), m_Cmd.Pocket.x + 1, m_Cmd.Pocket.y + 1, strPicker);
					m_nStep = eExcuteStep_Do_Blow;
					break;
				}
			}
			std::vector<CString> vErrString; vErrString.clear();
			int nRet = pTaskTrayUnloadPP->ProcDeviceData(m_Cmd.eStage, m_Cmd.Pocket.x, m_Cmd.Pocket.y, m_Cmd.vPicker, m_Cmd.ePickPlace, vErrString);

			pTaskTrayUnloadPP->m_bCyclePause = FALSE;

			strPicker = _T(""), strTmp = _T("");
			for (std::vector<CPoint>::size_type i = 0; i < (int)m_Cmd.vPicker.size(); i++) {
				strTmp.Format("[X:%d, Y:%d]", m_Cmd.vPicker[i].x + 1, m_Cmd.vPicker[i].y + 1);
				strPicker += strTmp;
			}
			pTaskTrayUnloadPP->MakeLog("[ProcDevice Data Place] CmdStage=%s, CmdTargetX=%d, CmdTargetY=%d, %s", pTaskTrayUnloadPP->GetLocName(m_Cmd.eStage), m_Cmd.Pocket.x + 1, m_Cmd.Pocket.y + 1, strPicker);

			if (nRet == ERR_TRAY_PP_2_DEVICE_MISS)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("place fail, Pocket[x:%d,y:%d]", m_Cmd.Pocket.x + 1, m_Cmd.Pocket.y + 1);
				for (int i = 0; i < (int)vErrString.size(); i++)
				{
					AlarmData.m_fnAddItemFormat("%s", vErrString[i]);
				}
				ReportAlarm(nRet, AlarmData, pTaskTrayUnloadPP->GetFailPicker());
				break;
			}

			if (nRet != ERR_NO_ERROR)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("place fail, Pocket[x:%d,y:%d]", m_Cmd.Pocket.x + 1, m_Cmd.Pocket.y + 1);
				ReportAlarm(nRet, AlarmData, pTaskTrayUnloadPP->GetFailPicker());
				break;
			}
		}
		m_nRetryCount = 0;
		m_nLast_Executing_Step = m_nStep;
		m_nStep = 15000;
	}break;

	//------------------------------------------------------------------------------------------
	// pick vac loop
	case 13000: 
	{
		BOOL bPreVacuum = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TRAY_HAND_PRE_VACUUM);

		if (bPreVacuum != eOPTION_USE) {
			pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [Vacuum On] [SRT]", m_szCmdName, m_nStep);
			int nErr = pTaskTrayUnloadPP->do_VacBlow(eVacBlwSt_VacOn, m_Cmd.vPicker);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Tray PP vacuum cmd Error");
				ReportAlarm(nErr, AlarmData);
				break;
			}
			pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [Vacuum On] [END]", m_szCmdName, m_nStep);
		}
		pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [Vacuum Wait] [STR]", m_szCmdName, m_nStep);
		int nSleep = g_DMCont.m_dmTrayPP.GN(NDM5_TM_UDTrayPP_Vacuum);
		if (nSleep > 0) {
			Sleep(nSleep);
		}
		pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [Vacuum Wait] [END]", m_szCmdName, m_nStep);

		m_nStep = 15000;
	}break;

	//------------------------------------------------------------------------------------------
	case 15000: // move z safety & picker Up
	{
		pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [Move Safety and Picker up] [SRT]", m_szCmdName, m_nStep);


		std::vector<CPoint> vPkr;
		pTaskTrayUnloadPP->MakeAllPicker(vPkr);
		int nErr = pTaskTrayUnloadPP->do_MovePos(m_Cmd.eStage, m_Cmd.Pocket, vPkr, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray PP z move safety cmd Error");
			OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(nErr, AlarmData);
			break;
		}
		m_tcTimeout = (double)GetTickCount64();
		m_nStep = 17000;
	}break;
	case 17000: //check z axis done
	{
		std::vector<CPoint> vPkr;
		pTaskTrayUnloadPP->MakeAllPicker(vPkr);
		m_vErrPicker.clear();
		int nErrSen = pTaskTrayUnloadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);
		int nAxisDone = pMotorZ->AxisDone(vPkr, m_vErrPicker);
		if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR) 
		{
			pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [Move Safety and Picker up] [END]", m_szCmdName, m_nStep);
			if (m_Cmd.bVacuumCheckFuction == TRUE)
			{
				m_nStep = eExcuteStep_Table_Vacuum_Check;
			}
			else {
				m_nStep = 18000;
			}
					
			m_tcTimeoutPicker = (double)GetTickCount64();
			break;		
		}
		else {
			std::vector<CPoint> vErrPkr; vErrPkr.clear();
			CString sAlarmData = _T("");
			int nErr = pTaskTrayUnloadPP->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, pTaskTrayUnloadPP->GetSafetyFailPicker(ePickerSafetySensorCheckType::eSafetySensorCheck), vErrPkr, sAlarmData);
			double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
			if (dSpanTime > (TIME_OUT_SHORT*1000.0)) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("%s", sAlarmData);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				ReportAlarm(nErr, AlarmData, vErrPkr);
				break;
			}
		}
	}break;

	//------------------------------------------------------------------------------------------
	case 18000: // picker up check and write device data
	{
		double dSpanTime = (double)GetTickCount64() - m_tcTimeoutPicker;
		if (dSpanTime > TIME_OUT_SHORT*1000.0) {	
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray PP picker down operation is not complete, retry count(%d) time over!", m_nSetRetryCount);
			ReportAlarm(ERR_TRAY_PP_1_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData, pTaskTrayUnloadPP->m_vFailPicker);
			break;		
		}

		int nErr = pTaskTrayUnloadPP->Chk_Z_Safety_Sen_All();
		if (nErr == ERR_NO_ERROR) {
			pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [Move Safety and Picker up] [END]", m_szCmdName, m_nStep);
			if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto && m_Cmd.ePickPlace == DEF_PICK)
			{
				CString strPicker = _T(""), strTmp = _T(""), strPocket = _T("");
				int nPocketX = 0, nPocketY = 0;
				int nNotVacCount = 0;
				pTaskTrayUnloadPP->m_vFailPicker.clear();
				for (std::vector<CPoint>::size_type i = 0; i < (int)m_Cmd.vPicker.size(); i++) {
					strTmp.Format("[X:%d, Y:%d]", m_Cmd.vPicker[i].x + 1, m_Cmd.vPicker[i].y + 1);
					if (pTaskTrayUnloadPP->ChkVacuum(eOnOffState::eOn, m_Cmd.vPicker[i]) != ERR_NO_ERROR) {
						strPicker += strTmp;
						nNotVacCount++;
						pTaskTrayUnloadPP->m_vFailPicker.push_back(m_Cmd.vPicker[i]);
					}
				}

				// Pick Retry
				if (pTaskTrayUnloadPP->m_vFailPicker.size() > 0)
				{
					if (m_nRetryCount < m_nSetRetryCount) {
						pTaskTrayUnloadPP->MakeLog("[Vacuum Retry] CmdStage=%s, CmdTargetX=%d, CmdTargetY=%d, %s", pTaskTrayUnloadPP->GetLocName(m_Cmd.eStage), m_Cmd.Pocket.x + 1, m_Cmd.Pocket.y + 1, strPicker);
						m_nRetryCount++;
						m_nStep = 10000;
						break;
					}
				}
				std::vector<CString> vErrString; vErrString.clear();
				int nRet = pTaskTrayUnloadPP->ProcDeviceData(m_Cmd.eStage, m_Cmd.Pocket.x, m_Cmd.Pocket.y, m_Cmd.vPicker, m_Cmd.ePickPlace, vErrString);
				for (std::vector<CPoint>::size_type i = 0; i < (int)m_Cmd.vPicker.size(); i++) {
					strTmp.Format("[X:%d, Y:%d]", m_Cmd.vPicker[i].x + 1, m_Cmd.vPicker[i].y + 1);
					strPicker += strTmp;
				}

				if ((int)pTaskTrayUnloadPP->m_vFailPicker.size() > 0) {
					typedef std::vector<CPoint>::const_iterator vIterPoint;
					std::pair<vIterPoint, vIterPoint> minX1 = std::minmax_element(m_Cmd.vPicker.begin(), m_Cmd.vPicker.end(), MinX);
					std::pair<vIterPoint, vIterPoint> minY1 = std::minmax_element(m_Cmd.vPicker.begin(), m_Cmd.vPicker.end(), MinY);

					for (std::vector<CPoint>::size_type i = 0; i < (int)m_Cmd.vPicker.size(); i++) {
						for (std::vector<CPoint>::size_type j = 0; j < (int)pTaskTrayUnloadPP->m_vFailPicker.size(); j++) {
							if (m_Cmd.vPicker[i].x == pTaskTrayUnloadPP->m_vFailPicker[j].x && m_Cmd.vPicker[i].y == pTaskTrayUnloadPP->m_vFailPicker[j].y) {
								nPocketX = m_Cmd.Pocket.x + (pTaskTrayUnloadPP->m_vFailPicker[j].x - minX1.first->x) + 1;
								nPocketY = m_Cmd.Pocket.y + (pTaskTrayUnloadPP->m_vFailPicker[j].y - minY1.first->y) + 1;
								strTmp.Format("[X:%d, Y:%d]", nPocketX, nPocketY);
								strPocket += strTmp;
							}
						}
					}
				}

				pTaskTrayUnloadPP->MakeLog("[ProcDevice Data Pick] CmdStage=%s, CmdTargetX=%d, CmdTargetY=%d, %s", pTaskTrayUnloadPP->GetLocName(m_Cmd.eStage), m_Cmd.Pocket.x + 1, m_Cmd.Pocket.y + 1, strPicker);

				if (nRet == ERR_TRAY_PP_2_DEVICE_MISS)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("%s Picker Pick ERROR Pocket %s", strPicker, strPocket);
					for (std::vector<CString>::size_type i = 0; i < (int)vErrString.size(); i++)
					{
						AlarmData.m_fnAddItemFormat("%s", vErrString[i]);
					}
					ReportAlarm(nRet, AlarmData, pTaskTrayUnloadPP->GetFailPicker());
					break;
				}

				if (nRet != ERR_NO_ERROR)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("%s Picker Pick ERROR Pocket %s", strPicker, strPocket);
					ReportAlarm(nRet, AlarmData, pTaskTrayUnloadPP->GetFailPicker());
					break;
				}
			}
			
			//m_nStep = 25000;

			if (m_Cmd.vSpecOutPicker.size() > 0 && m_Cmd.bSpeckOutPkrEnd == FALSE && m_Cmd.ePickPlace == DEF_PLACE && m_Cmd.bSpecOutPkrUse == TRUE)
			{
				if (m_Cmd.nCntSpecOut == m_Cmd.vSpecOutPicker.size()) {
					m_Cmd.bSpeckOutPkrEnd = TRUE;
				}

				if (m_Cmd.bSpeckOutPkrEnd == FALSE)
				{
					m_Cmd.nCntSpecOut++;
					m_Cmd.bNormalPkrEnd = TRUE;
					//m_nStep = 10000;   //offset 넣을려면 더 뒤로 이동.

					m_nStep = 3001;		//Xpitch, Ypitch offset 추가 step
					break;
				}
			}
			m_nStep = 50000;
		}
	}break;
	case eExcuteStep_Table_Vacuum_Check:
	{
		double dSpanTime = (double)GetTickCount64() - m_tcTimeoutPicker;
		if (dSpanTime > TIME_OUT_SHORT*1000.0) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray PP picker down operation is not complete, retry count(%d) time over!", m_nSetRetryCount);
			ReportAlarm(ERR_TRAY_PP_1_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData, pTaskTrayUnloadPP->m_vFailPicker);
			break;
		}

		int nErr = pTaskTrayUnloadPP->Chk_Z_Safety_Sen_All();
		if (nErr == ERR_NO_ERROR) {
			pTaskTrayUnloadPP->MakeLog("[%s EXEC=%8d] [Move Safety and Picker up] [END]", m_szCmdName, m_nStep);
			if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto && m_Cmd.ePickPlace == DEF_PICK)
			{
				CString strPicker = _T(""), strTmp = _T(""), strPocket = _T("");
				int nPocketX = 0, nPocketY = 0;
				int nNotVacCount = 0;
				pTaskTrayUnloadPP->m_vFailPicker.clear();
				for (std::vector<CPoint>::size_type i = 0; i < (int)m_Cmd.vPicker.size(); i++) {
					strTmp.Format("[X:%d, Y:%d]", m_Cmd.vPicker[i].x + 1, m_Cmd.vPicker[i].y + 1);
					if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_ ? pTaskTrayUnloadPP->GetVacuum(m_Cmd.vPicker[i]) == ERR_NO_ERROR :pTaskTrayUnloadPP->ChkVacuum(eOnOffState::eOn, m_Cmd.vPicker[i]) == ERR_NO_ERROR) {
						strPicker += strTmp;
						nNotVacCount++;
						pTaskTrayUnloadPP->m_vFailPicker.push_back(m_Cmd.vPicker[i]);
					}
				}

				int nRet = pTaskTrayUnloadPP->ProcVacuumCheckMem(m_Cmd.eStage, m_Cmd.Pocket.x, m_Cmd.Pocket.y, m_Cmd.vPicker, m_Cmd.ePickPlace);
				

				for (std::vector<CPoint>::size_type i = 0; i < (int)pTaskTrayUnloadPP->m_vFailPicker.size(); i++) {
					strTmp.Format("[X:%d, Y:%d]", pTaskTrayUnloadPP->m_vFailPicker[i].x + 1, pTaskTrayUnloadPP->m_vFailPicker[i].y + 1);
					strPicker += strTmp;
				}

// 				for (std::vector<CPoint>::size_type i = 0; i < (int)m_Cmd.vPicker.size(); i++) {
// 					strTmp.Format("[X:%d, Y:%d]", m_Cmd.vPicker[i].x + 1, m_Cmd.vPicker[i].y + 1);
// 					strPicker += strTmp;
// 				}

				if ((int)pTaskTrayUnloadPP->m_vFailPicker.size() > 0) {
					typedef std::vector<CPoint>::const_iterator vIterPoint;
					std::pair<vIterPoint, vIterPoint> minX1 = std::minmax_element(m_Cmd.vPicker.begin(), m_Cmd.vPicker.end(), MinX);
					std::pair<vIterPoint, vIterPoint> minY1 = std::minmax_element(m_Cmd.vPicker.begin(), m_Cmd.vPicker.end(), MinY);

					for (std::vector<CPoint>::size_type i = 0; i < (int)m_Cmd.vPicker.size(); i++) {
						for (std::vector<CPoint>::size_type j = 0; j < (int)pTaskTrayUnloadPP->m_vFailPicker.size(); j++) {
							if (m_Cmd.vPicker[i].x == pTaskTrayUnloadPP->m_vFailPicker[j].x && m_Cmd.vPicker[i].y == pTaskTrayUnloadPP->m_vFailPicker[j].y) {
								nPocketX = m_Cmd.Pocket.x + (pTaskTrayUnloadPP->m_vFailPicker[j].x - minX1.first->x) + 1;
								nPocketY = m_Cmd.Pocket.y + (pTaskTrayUnloadPP->m_vFailPicker[j].y - minY1.first->y) + 1;
								strTmp.Format("[X:%d, Y:%d]", nPocketX, nPocketY);
								strPocket += strTmp;
							}
						}
					}
				}

				pTaskTrayUnloadPP->MakeLog("[ProcVacuumCheckMem] CmdStage=%s, CmdTargetX=%d, CmdTargetY=%d, %s", pTaskTrayUnloadPP->GetLocName(m_Cmd.eStage), m_Cmd.Pocket.x + 1, m_Cmd.Pocket.y + 1, strPicker);
				if (nRet != ERR_NO_ERROR)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Picker Pick Sensing %s", strPicker);
					ReportAlarm(nRet, AlarmData, pTaskTrayUnloadPP->GetFailPicker());
					break;
				}
			}

			//m_nStep = 25000;

			if (m_Cmd.vSpecOutPicker.size() > 0 && m_Cmd.bSpeckOutPkrEnd == FALSE && m_Cmd.ePickPlace == DEF_PLACE && m_Cmd.bSpecOutPkrUse == TRUE)
			{
				if (m_Cmd.nCntSpecOut == m_Cmd.vSpecOutPicker.size()) {
					m_Cmd.bSpeckOutPkrEnd = TRUE;
				}

				if (m_Cmd.bSpeckOutPkrEnd == FALSE)
				{
					m_Cmd.nCntSpecOut++;
					m_Cmd.bNormalPkrEnd = TRUE;
					//m_nStep = 10000;   //offset 넣을려면 더 뒤로 이동.

					m_nStep = 3001;		//Xpitch, Ypitch offset 추가 step
					break;
				}
			}
			m_nStep = 50000;
		}
	}break;
	case 50000:
	{
		ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
	}break;
	}
	return CBaseSeqCmd::State_Executing();
}

BOOL CTrayUnloadPPCmd_MovePickPlace::State_Pause()
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
			OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis(static_cast<CTaskTrayBasePP::eMotor>(i));
			bool bCloseLimit = pMotor->CheckLimitClose();
			double dCurPos = 0;
			pMotor->GetCurrentPos(dCurPos);
			if (bCloseLimit == false)
			{
				pTaskTrayUnloadPP->MakeLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
				pMotor->SetSlowStop();
			}
		}

		//SetSlowStop 전, Device Data 처리.
		if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto && pTaskTrayUnloadPP->m_bCyclePause == TRUE)
		{
			int nBlowDoneCnt = 0;
			int nHandData = 0;
			for (int i = 0; i < (int)m_Cmd.vPicker.size(); i++) {
				//Blow Done Count Check.
				if (pTaskTrayUnloadPP->ChkVacuum(eOnOffState::eOff, m_Cmd.vPicker[i]) == ERR_NO_ERROR) { //Blow Retry						
					nBlowDoneCnt++;
				}

				//Hand Device Data Check.
				//ST_DD_DEVICE stHandData;
				eExistState eHandData;
				pTaskTrayUnloadPP->GetDeviceDataExist(m_Cmd.vPicker[i], &eHandData);
				if (eHandData == eExist) {
					nHandData++;
				}
			}

			//1. Place일 경우.
			//2. Blow 진행 했을 경우.
			//3. Hand Device Data 존재 할 경우.
			if (m_Cmd.ePickPlace == DEF_PLACE && nBlowDoneCnt == m_Cmd.vPicker.size() && nHandData == m_Cmd.vPicker.size())
			{
				std::vector<CString> vErrString; vErrString.clear();
				int nRet = pTaskTrayUnloadPP->ProcDeviceData(m_Cmd.eStage, m_Cmd.Pocket.x, m_Cmd.Pocket.y, m_Cmd.vPicker, m_Cmd.ePickPlace, vErrString);
				pTaskTrayUnloadPP->m_bCyclePause = FALSE;
				CString strPicker = _T("");
				CString strTmp = _T("");
				for (std::vector<CPoint>::size_type i = 0; i < (int)m_Cmd.vPicker.size(); i++) {
					strTmp.Format("[X:%d, Y:%d]", m_Cmd.vPicker[i].x + 1, m_Cmd.vPicker[i].y + 1);
					strPicker += strTmp;
				}
				pTaskTrayUnloadPP->MakeLog("[Before Stop][ProcDevice Data Place] CmdStage=%s, CmdTargetX=%d, CmdTargetY=%d, %s", pTaskTrayUnloadPP->GetLocName(m_Cmd.eStage), m_Cmd.Pocket.x + 1, m_Cmd.Pocket.y + 1, strPicker);

				if (nRet == ERR_TRAY_PP_2_DEVICE_MISS)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("place fail, Pocket[x:%d,y:%d]", m_Cmd.Pocket.x + 1, m_Cmd.Pocket.y + 1);
					for (int i = 0; i < (int)vErrString.size(); i++)
					{
						AlarmData.m_fnAddItemFormat("%s", vErrString[i]);
					}
					ReportAlarm(nRet, AlarmData, pTaskTrayUnloadPP->GetFailPicker());
					break;
				}

				if (nRet != ERR_NO_ERROR)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("place fail, Pocket[x:%d,y:%d]", m_Cmd.Pocket.x + 1, m_Cmd.Pocket.y + 1);
					ReportAlarm(nRet, AlarmData, pTaskTrayUnloadPP->GetFailPicker());
					break;
				}

				m_nLast_Executing_Step = 15000;
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

BOOL CTrayUnloadPPCmd_MovePickPlace::State_Recovery()
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
	case 0: // move z axis safety & picker all up <start>
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

		m_tcTimeout = (double)GetTickCount64();	//check time picker up
		m_nStep = 1100;
	}break;

	case 1100:	// move z axis safety & picker all up <end>
	{
	
		std::vector<CPoint> vPkr;
		pTaskTrayUnloadPP->MakeAllPicker(vPkr);
		OneAxis* pMotorZ = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisZ);
		m_vErrPicker.clear();
		int nErrSen = pTaskTrayUnloadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);
		int nAxisDone = pMotorZ->AxisDone(vPkr, m_vErrPicker);
		if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR) {
			pTaskTrayUnloadPP->MakeLog("[%s RECOVERY=%8d] [z move safety & picker all up] [END]", m_szCmdName, m_nStep);
			m_nStep = 1200;
			break;
		}
		else 
		{
			std::vector<CPoint> vErrPkr; vErrPkr.clear();
			CString sAlarmData = _T("");
			int nErr = pTaskTrayUnloadPP->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, pTaskTrayUnloadPP->GetSafetyFailPicker(ePickerSafetySensorCheckType::eSafetySensorCheck), vErrPkr, sAlarmData);

			double dSpanTime = (double)GetTickCount64() - m_tcTimeout;

			if (dSpanTime > (TIME_OUT_SHORT*1000.0)) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("%s", sAlarmData);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				ReportAlarm(nErr, AlarmData, vErrPkr);
				break;
			}
		}
	}break;
	case 1200:	// y move safety <start>
	{
		OneAxis* pMotorY = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisY);
		std::vector<CPoint> vPkr;
		pTaskTrayUnloadPP->MakeAllPicker(vPkr);

		int nErr = pTaskTrayUnloadPP->do_MovePos(LOC_IDX_TRAY_PP_SAFETY, CPoint(0, 0), vPkr, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisY);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("%s", pMotorY->GetAxisName());
			ReportAlarm(nErr, AlarmData);
			break;
		}
		m_tcTimeout = (double)GetTickCount64();
		m_nStep = 1201;
	}break;
	case 1201:	// y move safety <end>
	{
		OneAxis* pMotorY = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisY);
		int nAxisDone = pMotorY->AxisDone();
		if (nAxisDone == ERR_NO_ERROR) {
			//SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_NormalComplete );
			g_TaskTrayUnloadPP.chk_TrayUnload_Y_Safety_sen(DEF_ON);
			m_nStep = 5000;
			break;
		}
		else
		{
			double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
			if (dSpanTime > (MOTOR_TIME_OUT_MIDDLE*1000.0)) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Tray pp y axis move y safety time over");
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorY->GetAxisName());
				ReportAlarm(ERR_TRAY_PP_2_MOTOR_MOVE_TIMEOUT, AlarmData);
				break;
			}

			if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Tray pp y axis axis done fail");
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorY->GetAxisName());
				ReportAlarm(ERR_TRAY_PP_2_MOTOR_MOVE_FAIL, AlarmData);
				break;
			}
		}
	}break;
	case 5000:
	{
		if (m_Cmd.bVacuumCheckFuction == TRUE)
		{
			m_nStep = 99999;
			break;
		}

		CString error_msg = _T("");
		eCheckAbnormalCase _case_ = check_abnormal_case(m_Cmd.ePickPlace, error_msg);
		switch (_case_)
		{
		case invaild_case:
		{
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("TrayPPCmd_MovePickPlace::State_Recovery invaild case");
			ReportAlarm(ERR_UNKNOWN, AlarmData);
		}break;
		case cmd_end:
		case abnormal:
		{
			// z up 하고 normal complete
			SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
			break;
		}break;
		case alarm_drop:
		{
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Device Drop %s", error_msg);
			ReportAlarm(ERR_TRAY_PP_2_VAC_SEN_ERR, AlarmData);
		}break;
		case alarm_device_exist:
		{
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Device Remaind %s", error_msg);
			ReportAlarm(ERR_TRAY_PP_2_VAC_SEN_ERR, AlarmData);
		}break;
		case situation:
		{
			switch (m_Cmd.ePickPlace)
			{
			case ePPCmd::ePick:
			{
				CString strPicker = _T(""), strTmp = _T("");
				bool bAllVacuumOn = true;
				for each(auto picker in m_Cmd.vPicker) {
					strTmp.Format("[X:%d, Y:%d]", picker.x + 1, picker.y + 1);
					strPicker += strTmp;

					int nVacuumOnOff = pTaskTrayUnloadPP->GetVacuum(picker);
					bAllVacuumOn &= (nVacuumOnOff == DEF_ON);
				}
				if (bAllVacuumOn == false) {
					// 처음부터 다시 시작.
					m_nStep = 99999;
					break;
				}
				else {
					std::vector<CString> vErrString; vErrString.clear();
					int nRet = pTaskTrayUnloadPP->ProcDeviceData(m_Cmd.eStage, m_Cmd.Pocket.x, m_Cmd.Pocket.y, m_Cmd.vPicker, m_Cmd.ePickPlace, vErrString);

					if (nRet == ERR_TRAY_PP_2_DEVICE_MISS)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("ProcDeviceData fail, Pocket[x:%d,y:%d] use hand [%s]", m_Cmd.Pocket.x + 1, m_Cmd.Pocket.y + 1, strPicker);
						for (int i = 0; i < (int)vErrString.size(); i++)
						{
							AlarmData.m_fnAddItemFormat("%s", vErrString[i]);
						}
						ReportAlarm(nRet, AlarmData, pTaskTrayUnloadPP->GetFailPicker());
						break;
					}

					if (nRet != ERR_NO_ERROR)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("ProcDeviceData fail, Pocket[x:%d,y:%d] use hand [%s]", m_Cmd.Pocket.x + 1, m_Cmd.Pocket.y + 1, strPicker);
						ReportAlarm(nRet, AlarmData);
						break;
					}
					SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
					break;
				}
			}break;
			case ePPCmd::ePlace:
			{
				if (m_nLast_Executing_Step > eExcuteStep_Do_Blow) {
					CString strPicker = _T(""), strTmp = _T("");
					for each(auto picker in m_Cmd.vPicker) {
						strTmp.Format("[X:%d, Y:%d]", picker.x + 1, picker.y + 1);
						strPicker += strTmp;
					}
					std::vector<CString> vErrString; vErrString.clear();
					int nRet = pTaskTrayUnloadPP->ProcDeviceData(m_Cmd.eStage, m_Cmd.Pocket.x, m_Cmd.Pocket.y, m_Cmd.vPicker, m_Cmd.ePickPlace, vErrString);

					if (nRet == ERR_TRAY_PP_2_DEVICE_MISS)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("ProcDeviceData fail, Pocket[x:%d,y:%d] use hand [%s]", m_Cmd.Pocket.x + 1, m_Cmd.Pocket.y + 1, strPicker);
						for (int i = 0; i < (int)vErrString.size(); i++)
						{
							AlarmData.m_fnAddItemFormat("%s", vErrString[i]);
						}
						ReportAlarm(nRet, AlarmData, pTaskTrayUnloadPP->GetFailPicker());
						break;
					}

					if (nRet != ERR_NO_ERROR)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("ProcDeviceData fail, Pocket[x:%d,y:%d] use hand [%s]", m_Cmd.Pocket.x + 1, m_Cmd.Pocket.y + 1, strPicker);
						ReportAlarm(nRet, AlarmData);
						break;
					}
					SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
					break;
				}
				else {

					CString strMsg = _T("");
					std::vector<CString> vErrString; vErrString.clear();
					int nChkErr = pTaskTrayUnloadPP->CheckDevice(strMsg, m_Cmd.vPicker, vErrString);
					if (nChkErr != ERR_NO_ERROR) {
						CParamList AlarmData;
						if (strMsg.IsEmpty() != TRUE) { AlarmData.m_fnAddItemFormat(strMsg); }
						for (int i = 0; i < (int)vErrString.size(); i++)
						{
							AlarmData.m_fnAddItemFormat("%s", vErrString[i]);
						}
						ReportAlarm(nChkErr, AlarmData, pTaskTrayUnloadPP->GetFailPicker());
						break;
					}

					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Device Drop %s", error_msg);
					ReportAlarm(ERR_TRAY_PP_2_VAC_SEN_ERR, AlarmData);
					break;
				}
			}break;
			default:
				ASSERT(false);
				break;
			}
		}break;
		case normal_start:
		{
			// 처음부터 다시 시작.
			m_nStep = 99999;
		}break;
		default:
			ASSERT(false);
			break;
		}
	}break;

	case 10000:	// z move safety & picker up <start>
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

		m_tcTimeout = (double)GetTickCount64();//check time picker up
		m_nStep = 11000;
	}break;
	case 11000: // z move safety & picker up <end>
	{
		std::vector<CPoint> vPkr;
		pTaskTrayUnloadPP->MakeAllPicker(vPkr);
		int nErrSen = pTaskTrayUnloadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);
		m_vErrPicker.clear();
		OneAxis* pMotorZ = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisZ);
		int nAxisDone = pMotorZ->AxisDone(vPkr, m_vErrPicker);
		if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR ) {
			pTaskTrayUnloadPP->MakeLog("[%s RECOVERY=%8d] [z move safety & picker all up] [END]", m_szCmdName, m_nStep);
			m_nStep = 12000;
			break;
		}
		else
		{
			std::vector<CPoint> vErrPkr; vErrPkr.clear();
			CString sAlarmData = _T("");
			int nErr = pTaskTrayUnloadPP->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, pTaskTrayUnloadPP->GetSafetyFailPicker(ePickerSafetySensorCheckType::eSafetySensorCheck), vErrPkr, sAlarmData);

			double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
			if (dSpanTime > (TIME_OUT_SHORT*1000.0)) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("%s", sAlarmData);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				ReportAlarm(nErr, AlarmData, vErrPkr);
				break;
			}
		}
	}break;
	case 12000: // y move safety <start>
	{
		OneAxis* pMotorY = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisY);
		std::vector<CPoint> vPicker;
		vPicker.push_back(CPoint(0, 0));

		int nErr = pTaskTrayUnloadPP->do_MovePos(LOC_IDX_TRAY_PP_SAFETY, CPoint(0, 0), vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisY);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Axis=%s ", pMotorY->GetAxisName());
			ReportAlarm(nErr, AlarmData);
			break;
		}
		m_tcTimeout = (double)GetTickCount64();
		m_nStep = 12001;
	}break;
	case 12001: // y move safety <end>
	{
		OneAxis* pMotorY = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisY);

		int nAxisDone = pMotorY->AxisDone();
		if (nAxisDone == ERR_NO_ERROR) {
			m_nStep = 99999;
			break;
		}
		else 
		{
			double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
			if (dSpanTime > (MOTOR_TIME_OUT_MIDDLE*1000.0)) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Unload Tray PP Y axis move y safety time over");
				AlarmData.m_fnAddItemFormat("Axis=%s ", pMotorY->GetAxisName());
				ReportAlarm(ERR_TRAY_PP_2_MOTOR_MOVE_TIMEOUT, AlarmData);
				break;
			}

			if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Unload Tray PP Y axis axis done fail");
				AlarmData.m_fnAddItemFormat("Axis=%s ", pMotorY->GetAxisName());
				ReportAlarm(ERR_TRAY_PP_2_MOTOR_MOVE_FAIL, AlarmData);
				break;
			}
		}
	}break;
	case 99999:
	{
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
		SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing);
	}break;

	default:
	{
		m_nStep = 0;
	}break;
	}
	

	return CBaseSeqCmd::State_Recovery();
}

BOOL CTrayUnloadPPCmd_MovePickPlace::State_NormalComplete()
{
	return CBaseSeqCmd::State_NormalComplete();
}

BOOL CTrayUnloadPPCmd_MovePickPlace::State_Alarm()
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
			OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis(static_cast<CTaskTrayBasePP::eMotor>(i));
			bool bCloseLimit = pMotor->CheckLimitClose();
			double dCurPos = 0;
			pMotor->GetCurrentPos(dCurPos);
			if (bCloseLimit == false)
			{
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

BOOL CTrayUnloadPPCmd_MovePickPlace::State_AlarmComplete()
{
	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTrayUnloadPP->MakeLog("[CMD: %s] [ALARM COMP] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	OneAxis* pMotorZ = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisZ);

	switch (m_nStep)
	{
	case 0:
	{
		//pTaskTrayPP->SendEvent(CEID_TRAY_PP_ALARMCOMPLETE_START);
		m_nStep = 1000;
	}break;
	case 1000: // move z axis safety & picker all up
	{
		std::vector<CPoint> vPkr;
		pTaskTrayUnloadPP->MakeAllPicker(vPkr);
		int nErr = pTaskTrayUnloadPP->do_MovePos(m_Cmd.eStage, m_Cmd.Pocket, vPkr, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray PP Z Axis safety move command Error");
			OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(nErr, AlarmData);
			break;
		}

		m_tcTimeout = (double)GetTickCount64();
		m_nStep = 3000;
	}break;
	case 3000:	// check move z safety
	{
		int nAxisDone = pMotorZ->AxisDone();
		if (nAxisDone == ERR_NO_ERROR) {
			m_nStep = 4000;
			m_nRetryCount = 0;
			break;
		}
		else 
		{
			double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
			double dCurPos = 0;
			pMotorZ->GetCurrentPos(dCurPos);
			if (dSpanTime > (MOTOR_TIME_OUT_MIDDLE*1000.0)) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Unload Tray PP z-axis operation is not complete, SpanTime=%.2f, z-axis current pos=%.2f", dSpanTime*1000.0, dCurPos);
				AlarmData.m_fnAddItemFormat("Axis=%s ", pMotorZ->GetAxisName());
				ReportAlarm(ERR_TRAY_PP_2_MOTOR_MOVE_TIMEOUT, AlarmData);
				break;
			}

			if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Unload Tray PP z-axis operation is not complete");
				AlarmData.m_fnAddItemFormat("Axis=%s ", pMotorZ->GetAxisName());
				ReportAlarm(ERR_TRAY_PP_2_MOTOR_MOVE_FAIL, AlarmData);
			}
		}
	}break;
	case 4000:
	{
		// Alarm Comp finish
		eStacker_Type stacker_type = (eStacker_Type)-1;
		if (LOC_IDX_TRAY_PP_CONV_1 <= m_Cmd.eStage && m_Cmd.eStage <= LOC_IDX_TRAY_PP_CONV_7)
			stacker_type = g_TaskAutoStacker[m_Cmd.eStage - LOC_IDX_TRAY_PP_CONV_1].GetStackerType();


		if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto) {

			int nSkipType = eDeviceData_Erase;
			if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoSkipUse) == TRUE && g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse) == FALSE
				&& m_Cmd.ePickPlace == DEF_PICK && (stacker_type == eStackerType_Load))
				nSkipType = eDeviceData_EraseAndAutoSkip;

			std::vector<CString> vErrString; vErrString.clear();
			pTaskTrayUnloadPP->ProcDeviceData(m_Cmd.eStage, m_Cmd.Pocket.x, m_Cmd.Pocket.y, m_Cmd.vPicker, m_Cmd.ePickPlace, vErrString, nSkipType);

			CString strPicker = _T(""), strTmp = _T("");
			for (std::vector<CPoint>::size_type i = 0; i < (int)m_Cmd.vPicker.size(); i++) {
				strTmp.Format("[X:%d, Y:%d]", m_Cmd.vPicker[i].x + 1, m_Cmd.vPicker[i].y + 1);
				strPicker += strTmp;
			}

			pTaskTrayUnloadPP->MakeLog("[ProcDevice Data] CmdStage=%d, CmdTargetX=%d, CmdTargetY=%d, Picker=%s", m_Cmd.eStage, m_Cmd.Pocket.x + 1, m_Cmd.Pocket.y + 1, strPicker);

			// set in/out jam count
			int nCount = pTaskTrayUnloadPP->GetLostDvcCount(m_Cmd.vPicker, m_Cmd.ePickPlace);
			switch (m_Cmd.eStage)
			{
			case LOC_IDX_TRAY_PP_TBL_1:
			case LOC_IDX_TRAY_PP_TBL_2:
			{
				if (m_Cmd.ePickPlace == DEF_PICK) {
					g_JamRate.PushInOutJam(eJamType_Output, nCount);
				}
				else {
					g_JamRate.PushInOutJam(eJamType_Input, nCount);
				}
			}break;
			}

			if (stacker_type == eStackerType_Unload) {
				if (m_Cmd.ePickPlace != DEF_PICK) {
					g_JamRate.PushInOutJam(eJamType_Output, nCount);
				}
			}
		}

		if (pTaskTrayUnloadPP->m_bSkipToAssign == TRUE) {
			if (stacker_type == eStackerType_Load) {
				g_GuiComm.SendDataAssignShow(m_Cmd.eStage);
			}
			pTaskTrayUnloadPP->m_bSkipToAssign = FALSE;
		}

		pTaskTrayUnloadPP->MakeLog("[CMD: %s] [ALARM COMP] [Step=%8d] AlarmComplete End", m_szCmdName, m_nStep);
		return CBaseSeqCmd::State_AlarmComplete();
	}break;
	
	default: 
	{
		m_nStep = 0;
	}break;
	}

	return CBaseSeqCmd::State_AlarmComplete();
}

BOOL CTrayUnloadPPCmd_MovePickPlace::State_Aborting()
{
	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskTrayUnloadPP->MakeLog("[CMD: %s] [ABORT] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	OneAxis* pMotorZ = pTaskTrayUnloadPP->GetAxis(CTaskTrayBasePP::eAxisZ);

	switch (m_nStep)
	{
	case 0:
	{
		int nAxisCount = pTaskTrayUnloadPP->GetMotorCount();
		for (int i = 0; i < nAxisCount; i++) {
			OneAxis* pMotor = pTaskTrayUnloadPP->GetAxis(static_cast<CTaskTrayBasePP::eMotor>(i));
			if (pMotor != nullptr) {
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

BOOL CTrayUnloadPPCmd_MovePickPlace::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}




eCheckAbnormalCase CTrayUnloadPPCmd_MovePickPlace::check_abnormal_case( int _PickPlace, CString& error_msg )
{
	/*
			데이터 유무	백큠 유무	pocket의 데이터 유무
	pick		O			O				X				Up하고 명령 끝낸다. ( cmd_end )
				O			O				O				로그 후 명령 종료 ( abnormal )
				O			X				X				알람. drop 또는 사용자 임의 제거 ( alarm_drop )
				O			X				O				로그 후 명령 종료 ( abnormal )
				X			O				X				알람. 비정상, 디바이스가 딸려서 옴 ( alarm_device_exist )
				X			X				X				로그 후 명령 종료 ( abnormal )
				X			X				O				정상. 다시 처음부터 진행. ( normal_start )
				X			O				O				상황 별 처리. ( situation )
															ㄴ 일부 만 백큠 들어오면, 처음
															ㄴ 다 들어오면 데이터 처리만 진행.

	place		O			O				X				정상. 다시 처음부터 진행. ( normal_start )
				O			X				X				상황 별 처리. ( situation )
															ㄴ 블로우 했을 경우, 데이터 처리 만 진행.
															ㄴ 블로우 안했을 경우, 알람 drop 또는 사용자 제거.
				O			O				O				로그 후 명령 종료 ( abnormal )
				O			X				O				로그 후 명령 종료 ( abnormal )
				X			O				X				알람. 비정상, 디바이스가 딸려서 옴 ( alarm_device_exist )
				X			O				O				알람. 비정상, 디바이스가 딸려서 옴 ( alarm_device_exist )
				X			X				X				로그 후 명령 종료 ( abnormal )
				X			X				O				Up하고 명령 끝낸다. ( cmd_end )
	*/

	/*
			데이터 유무	백큠 유무	pocket의 데이터 유무
	pick		O			O				O				로그 후 명령 종료 ( abnormal )
	place		O			O				O				로그 후 명령 종료 ( abnormal )

	pick		O			O				X				Up하고 명령 끝낸다. ( cmd_end )
	place		O			O				X				정상. 다시 처음부터 진행. ( normal_start )

	pick		X			O				O				상황 별 처리. ( situation )
	place		X			O				O				알람. 비정상, 디바이스가 딸려서 옴 ( alarm_device_exist )

	pick		O			X				O				로그 후 명령 종료 ( abnormal )
	place		O			X				O				로그 후 명령 종료 ( abnormal )

	pick		O			X				X				알람. drop 또는 사용자 임의 제거 ( alarm_drop )
	place		O			X				X				상황 별 처리. ( situation )

	pick		X			O				X				알람. 비정상, 디바이스가 딸려서 옴 ( alarm_device_exist )
	place		X			O				X				알람. 비정상, 디바이스가 딸려서 옴 ( alarm_device_exist )

	pick		X			X				O				정상. 다시 처음부터 진행. ( normal_start )
	place		X			X				O				Up하고 명령 끝낸다. ( cmd_end )

	pick		X			X				X				로그 후 명령 종료 ( abnormal )
	place		X			X				X				로그 후 명령 종료 ( abnormal )

	*/

	CTaskTrayUnloadPP* pTaskTrayUnloadPP = (CTaskTrayUnloadPP*)m_pReceive;

	std::vector< std::pair<CPoint, CPoint> > vPickerToPocket = pTaskTrayUnloadPP->change_picker_to_pocket_point( m_Cmd.eStage, m_Cmd.Pocket.x, m_Cmd.Pocket.y, m_Cmd.vPicker );

	eCheckAbnormalCase last_ret = invaild_case;
	CString last_ret_str = _T( "" );

	for each( auto picker in m_Cmd.vPicker ) {
		ST_DD_DEVICE stDevice;
		pTaskTrayUnloadPP->GetTargetCmdStageDvcData(picker.x, picker.y, &stDevice );

		auto iter_current_picker_pocket = std::find_if( vPickerToPocket.begin(), vPickerToPocket.end()
														, [ & ] ( std::pair< CPoint, CPoint > _picker_to_pocket ) -> bool
		{
			if( _picker_to_pocket.first == picker )
				return true;

			return false;
		} );

		CPoint target_pocket = iter_current_picker_pocket->second;
		eDeviceDataStage data_stage_idx = eDeviceDataStage::eDeviceStage_Undefine;
		pTaskTrayUnloadPP->ConvertCmdIdxToStageIdx( m_Cmd.eStage, &data_stage_idx );
		int nPocketDataExist = g_DMCont.m_dmDevice.GetDeviceExist( data_stage_idx, target_pocket.x, target_pocket.y );

		bool bPickerDataExist = stDevice.sExist == DEF_EXIST;
		bool nPickerVacuumOn = pTaskTrayUnloadPP->GetVacuum( picker ) == DEF_ON;
		bool bPocketDataExist = nPocketDataExist == DEF_EXIST;

		int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN( NDM11_SYS_MD_DEVICE_CHK );

		error_msg.Format( "[ Picker( %d, %d ) Device Data : %s ][ Picker Vacuum : %s ][ Pocker( %d, %d ) Device Data : %s ]"
						  , picker.x + 1, picker.y + 1, bPickerDataExist == true ? _T( "Exist" ) : _T( "Not Exist" )
						  , nPickerVacuumOn == true ? _T( "On" ) : _T( "Off" )
						  , target_pocket.x + 1, target_pocket.y + 1, bPocketDataExist == true ? _T( "Exist" ) : _T( "Not Exist" ) );

		pTaskTrayUnloadPP->MakeLog( "[%s] [ Check Abnormal Case ] %s", m_szCmdName, error_msg );

		if( ( bPickerDataExist == true && nPickerVacuumOn == true && bPocketDataExist == true )
			|| ( bPickerDataExist == true && nPickerVacuumOn == false && bPocketDataExist == true )
			|| ( bPickerDataExist == false && nPickerVacuumOn == false && bPocketDataExist == false ) )
		{
			last_ret = abnormal;
			last_ret_str = _T( "abnormal" );
			break;
		}
		else if( bPickerDataExist == true && ( nPickerVacuumOn == true || nDeviceMode != eDeviceMode ) && bPocketDataExist == false ) {
			if( m_Cmd.ePickPlace == DEF_PICK ) {
				last_ret = cmd_end;
				last_ret_str = _T( "cmd_end" );
			}
			else if( m_Cmd.ePickPlace == DEF_PLACE ) {
				last_ret = normal_start;
				last_ret_str = _T( "normal_start" );
			}
		}
		else if( bPickerDataExist == false && ( nPickerVacuumOn == false || nDeviceMode != eDeviceMode ) && bPocketDataExist == true ) {
			if( m_Cmd.ePickPlace == DEF_PICK ) {
				last_ret = normal_start;
				last_ret_str = _T( "normal_start" );
			}
			else if( m_Cmd.ePickPlace == DEF_PLACE ) {
				last_ret = cmd_end;
				last_ret_str = _T( "cmd_end" );
			}
		}
		else if( bPickerDataExist == false && nPickerVacuumOn == true && bPocketDataExist == true ) {
			if( m_Cmd.ePickPlace == DEF_PICK ) {
				last_ret = situation;
				last_ret_str = _T( "situation" );
				break;
			}
			else if( m_Cmd.ePickPlace == DEF_PLACE ) {
				last_ret = alarm_device_exist;
				last_ret_str = _T( "alarm_device_exist" );
				break;
			}
		}
		else if( bPickerDataExist == true && nPickerVacuumOn == false && bPocketDataExist == false ) {
			if( m_Cmd.ePickPlace == DEF_PICK ) {
				last_ret = alarm_drop;
				last_ret_str = _T( "alarm_drop" );
				break;
			}
			else if( m_Cmd.ePickPlace == DEF_PLACE ) {
				last_ret = situation;
				last_ret_str = _T( "situation" );
				break;
			}
		}
		else if( bPickerDataExist == false && nPickerVacuumOn == true && bPocketDataExist == false ) {
			last_ret = alarm_device_exist;
			last_ret_str = _T( "alarm_device_exist" );
			break;
		}
	}

	pTaskTrayUnloadPP->MakeLog( "[%s] [ Check Abnormal Case is %s ]", m_szCmdName, last_ret_str );
	return last_ret;
}

