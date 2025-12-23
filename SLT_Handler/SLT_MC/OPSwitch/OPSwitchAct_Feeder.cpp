#include "stdafx.h"
#include "OPSwitchAct_Feeder.h"


COPSwitchAct_Feeder::COPSwitchAct_Feeder()
{
	m_bFeederSwitchOnOff = FALSE;
}


COPSwitchAct_Feeder::~COPSwitchAct_Feeder()
{
}

void COPSwitchAct_Feeder::OPSwitchSet(CString strName, int nSw_IO_Addr, int nLed_IO_Addr, eOP_OUT_ATTRIB eOut_Attrib, BOOL bBlink/*=FALSE*/, int nLedType/*=0*/)
{
	COPSwitchAct::OPSwitchSet(strName, nSw_IO_Addr, nLed_IO_Addr, eOut_Attrib, bBlink, nLedType);
}

void COPSwitchAct_Feeder::Switch_Operation()
{
	switch (m_nOperationStep)
	{
	case 0:
	{
		if (!m_bLockMode)	//Lock되어 있으면 아무것도 하지 않는다.
		{
			int nChkOpSwitch = 0;

			if (g_Feeder_SW.GetStatus() == DEF_ON) {
				nChkOpSwitch++;
			}


			if (GetSWStatus())
			{
				if (m_bTemp_key == FALSE)
				{
					m_bTemp_key = TRUE;

					if (!GetStatus() && nChkOpSwitch == 0)
					{
						if (m_tmChkPush.IsChecking() == FALSE) {
							m_tmChkPush.SetCheckTime(0.05);
							m_tmChkPush.StartTimer();
						}
					}
					else
					{
						m_tmChkPush.SetTimerCheckEnd();
						m_bTemp_Memo_Flag = FALSE;
						m_nOperationStep = 2000;
					}
				}
				else {
					if (m_tmChkPush.IsChecking() == TRUE && m_tmChkPush.CheckOverTime() == TRUE && GetStatus() != DEF_ON && nChkOpSwitch == 0) {
						m_tmChkPush.SetTimerCheckEnd();
						m_bTemp_Memo_Flag = TRUE;
						m_nOperationStep = 1000;
					}
				}
			}
			else
			{
				m_tmChkPush.SetTimerCheckEnd();
				m_bTemp_key = FALSE;
			}
		}
	}break;

	case 1000: // Feeder STEP FUNCTION ON 
	{
		BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_EQP_RUN_STATE);

		if (bLotState == TRUE) {
			m_nOperationStep = 0;
			break;
		}
		if (!g_COVER_SW.GetStatus()) {
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK, AlarmData, NULL);
			m_nOperationStep = 0;
			return;
		}

		SetChangeStatusOnOff(DEF_ON);
		Sleep(100);
		SetStatus(DEF_ON);
		g_TaskSystemCtrl.MakeLog("Push Feeder [ON]");

		g_TaskAllAutoStacker.SetSwitchTrayFeederIgnore(DEF_REQ_UNK);
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("%s", GetStatus() == TRUE ? _T("Feeder Pick -> Unload Place Move") : _T("Unload Place -> Feeder Pick Move"));
		g_Error.AlarmReport(ERR_SWITCH_TRAY_FEEDER, AlarmData, &g_TaskSystemCtrl);
		m_nOperationStep++;
	}break;
	case 1001:
	{
		int nReqAck = g_TaskAllAutoStacker.GetSwitchTrayFeederIgnore();
		if (nReqAck != DEF_REQ_UNK) {
			if (nReqAck == DEF_REQ_OK) {
				m_nOperationStep = 1501;
				break;
			}
			else if(nReqAck == DEF_REQ_CANCEL){
				m_nOperationStep = 0;
				break;
			}
		}
	}break;
	case 1501: // Tray Load, Unload Z Safery move
	{
		if (g_DMCont.m_dmEQP.GB(BDM0_EQP_RUN_STATE) == TRUE)
		{
			m_nOperationStep = 0;
			break;
		}
			

		CBaseTaskManager::SetEqpCtrlMode(eEqpCtrlMode_Manual);

		std::vector<CPoint> vPkr;
		CPoint Pocket(0, 0);
		CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
		for (int i = 0; i < _countof(pTaskTrayPP); i++)
		{
			pTaskTrayPP[i]->MakeAllPicker(vPkr);
			int nErr = pTaskTrayPP[i]->do_MovePos(eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY, Pocket, vPkr, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nOperationStep = 0;
				break;
			}
		}
	

		m_tmChkPush.StartTimer();
		m_tmChkPush.SetCheckTime(TIME_OUT_SHORT);
		m_nOperationStep++;	
		
		g_TaskSystemCtrl.MakeLog("Push Feeder [%d] move safety and picker all up Step = %d", GetStatus() == TRUE ? "ON" : "OFF", m_nOperationStep);
	}break;
	case 1502: // Tray Load, Unload Z Safery & Motion Done Check
	{


		std::vector<CPoint> vPkr;
		g_TaskTrayLoadPP.MakeAllPicker(vPkr);
		std::vector<CPoint> vErrPocket;
		vErrPocket.clear();
		int nComp = 0;
		CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
		for (int i = 0; i < _countof(pTaskTrayPP); i++)
		{
			OneAxis* pMotorZ = pTaskTrayPP[i]->GetAxis(CTaskTrayBasePP::eAxisZ);
			int nAxisDone = pMotorZ->AxisDone(vPkr, vErrPocket);
			int nChk = pTaskTrayPP[i]->Chk_Z_Safety_Sen_All();
			if (nAxisDone == ERR_NO_ERROR && nChk == ERR_NO_ERROR) {
				nComp++;
			}

			if (m_tmChkPush.CheckOverTime() == TRUE)
			{
				CParamList AlarmData;
				g_Error.AlarmReport(ERR_TRAY_PP_1_Z_PICKER_MOTOR_MOVE_TIMEOUT + i, AlarmData, &g_TaskSystemCtrl, vErrPocket);
				m_nOperationStep = 0;
				break;
			}

		}

		if (nComp == _countof(pTaskTrayPP))
		{
			m_nOperationStep++;
			break;
		}

	}break;
	case 1503:  // Feeder Move 분기 (Place Pos or Pick Pos)
	{
		int nErr = -1;
		if (g_TaskTrayLoadPP.GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Ready
			&& g_TaskTrayFeeder.GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{		
			int nErr = g_TaskTrayLoadPP.Chk_Z_Safety_Sen_All();
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nOperationStep = 0;
				break;
			}

			// [Switch On] Feeder Current Pos -> Feeder Place Pos
			if (GetStatus() == TRUE) { 
				CTrayFeederCmd_MovePos* pSeqCmd = new CTrayFeederCmd_MovePos();
				pSeqCmd->m_nCmdStage = eLocIdxFeeder::LOC_IDX_FEEDER_TRAY_PLACE_POS;
				nErr = g_TaskSystemCtrl.SendCommand(g_TaskTrayFeeder, pSeqCmd, eEqpCtrlMode_Manual);
			}				
			else// [Switch OFF] Feeder Current Pos -> Feeder Pick Pos
			{	
				//Safety로 이동 하려 할 땐, Transfer Interlock 체크하지 않고 움직인다.

				g_TaskTrayFeeder.do_cyl_UpDown(eUp);

				int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_UPDN_WAITTIME);
				if (0 < nSleep) {
					Sleep(nSleep);
				}

				auto cylinder_up_state = g_TaskTrayFeeder.chk_cyl_UpDown(eUp);
				if (cylinder_up_state != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(cylinder_up_state.msg.c_str());
					g_Error.AlarmReport(ERR_FEEDER_UP_CYLINDER_FAIL, AlarmData, NULL);
					m_nOperationStep = 0;
					break;
				}

				OneAxis* pMotor = (OneAxis*)g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eMotor::eAxisY);
				double dTeachPos_Safety = g_TaskTrayFeeder.GetTeachPos(CTaskTrayFeeder::eMotor::eAxisY, LOC_IDX_FEEDER_LOAD_PICK_START_POS);
				int tray_row_step = g_TaskTrayFeeder.GetTrayDeviceExistRowStep();

				double dPosOffset = 0.0;
				if (tray_row_step > 0) {
					dPosOffset = g_DMCont.m_dmFeeder.GD(DDM10_FEEDER_STEP_OFFSET);
				}

				double tray_pitch_y_um = g_DMCont.m_dmTrayPP.GN(NDM5_Tray_Pitch_Y);
				int nY_PitchType = 1;
				if (tray_pitch_y_um < DEF_TRAY_PP_MAX_Y_PITCH_NARROW)
				{
					nY_PitchType = 2;
				}

				if (tray_row_step == -1) {
					tray_row_step = abs(tray_row_step);
				}

				//int nEQPNo = g_DMCont.m_dmEQP.GN(NDM11_Handler_EQP_No);
				double dTargetPos = 0;
				// 1호기 Feeder 개선으로 주석 처리
				//  [4/23/2025 donghyun.shin]
//				if (nEQPNo > DEF_EQUIPMENT_UNIT1)
					dTargetPos = dTeachPos_Safety + (tray_pitch_y_um * tray_row_step) - (tray_row_step * dPosOffset);
// 				else
// 					dTargetPos = dTeachPos_Safety - (tray_pitch_y_um * tray_row_step) - (tray_row_step * dPosOffset);

				nErr = pMotor->Move(dTargetPos, 200000.0, 100, 100, FALSE, _eMotorIntertlock::eMotorInterlock_All_Ignore);
			}

			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
				m_nOperationStep = 0;
				break;
			}			

			m_tmChkPush.StartTimer();
			m_tmChkPush.SetCheckTime(TIME_OUT_MIDDLE / 2);
			m_nOperationStep++;
			CString strLog;
			strLog.Format("%s", GetStatus() == TRUE ? "LOC_IDX_FEEDER_TRAY_PLACE_POS" : "LOC_IDX_FEEDER_LOAD_PICK_START_POS");
			g_TaskSystemCtrl.MakeLog("Push Feeder [%d] LOC_IDX_TRAY_PP_CONV_3 Feeder %s Move Step = %d", GetStatus() == TRUE ? "ON" : "OFF", strLog, m_nOperationStep);
		}
	}break;
	case 1504: // Feeder Motion Done Check
	{
		if (m_tmChkPush.CheckOverTime() == TRUE)
		{
			CParamList AlarmData;
			m_nOperationStep = 0;
			OneAxis* pMotor = (OneAxis*)g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eMotor::eAxisY);
			pMotor->SetSlowStop();
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			g_Error.AlarmReport(ERR_FEEDER_MOTOR_MOVE_TIMEOUT, AlarmData, &g_TaskSystemCtrl);
			break;
		}

		if (GetStatus() == TRUE) {
			if (g_TaskTrayFeeder.GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Ready) {
				m_nOperationStep = 0;
				g_TaskSystemCtrl.MakeLog("Push Feeder [%d] Complete = %d", GetStatus() == TRUE ? "ON" : "OFF", m_nOperationStep);
				break;
			}
		}
		else {
			OneAxis* pMotor = (OneAxis*)g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eMotor::eAxisY);
			if (pMotor->MotionDone() == ERR_NO_ERROR)
			{
				m_nOperationStep = 0;
				g_TaskSystemCtrl.MakeLog("Push Feeder [%d] Complete = %d", GetStatus() == TRUE ? "ON" : "OFF", m_nOperationStep);
				break;
			}
		}
	}break;
	case 2000: // Feeder STEP FUNCTION OFF
	{
		BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_EQP_RUN_STATE);

		if (bLotState == TRUE) {
			m_nOperationStep = 0;
			break;
		}

		if (!g_COVER_SW.GetStatus()) {
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK, AlarmData, NULL);
			m_nOperationStep = 0;
			return;
		}

		SetChangeStatusOnOff(DEF_OFF);
		Sleep(100);
		SetStatus(DEF_OFF);
		g_TaskSystemCtrl.MakeLog("Push Feeder [Off]");
		g_TaskAllAutoStacker.SetSwitchTrayFeederIgnore(DEF_REQ_UNK);
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("%s", GetStatus() == TRUE ? _T("Feeder Load Pos -> Feeder Unload Pos Move") : _T("Feeder Unload Pos -> Feeder Load Pos Move-> "));
		g_Error.AlarmReport(ERR_SWITCH_TRAY_FEEDER, AlarmData, &g_TaskSystemCtrl);
		m_nOperationStep = 1001;
	}break;
	};
}

void COPSwitchAct_Feeder::SetChangeStatusOff()
{
	SetStatus(DEF_OFF);
	m_tmChkPush.SetTimerCheckEnd();
}

void COPSwitchAct_Feeder::SetChangeStatusOnOff(BOOL bOnOff)
{
	if (bOnOff == DEF_OFF)
	{
		SwitchLEDOnOff(DEF_OFF);
	}
}