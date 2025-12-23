#include "stdafx.h"
#include "TaskAutoTrayLoadPP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace base_seq_cmd;
using namespace std;


void CTaskAutoTrayUnloadPP::AutoState_Execute_Normal()
{
	int nErr = 0;

	switch (m_nAutoRunSubStep)
	{
		//========================================================
		// START
		//========================================================
	case eAutoTrayStep_Run_Start:
	{
		m_nChkSafetyPosCnt = 0;
		int nReadyUnload = 0;

		for (std::vector<CTaskAutoStacker*>::size_type i = 0; i < g_unload_stackers.size(); i++)
		{
			int nIdx = g_unload_stackers[i]->GetStackerIndex();
			auto conv_state = g_TaskAutoStacker[nIdx].GetConveyorState();
			if (conv_state == PlaceEnable_Device)
				nReadyUnload++;
		}
		// Unload Setplate에 C-Tray가 모두 준비되어 있는지 확인.
		//if (nReadyUnload == (int)g_unload_stackers.size() || bStackerUnloadTransHolding == FALSE)
		{
			MakeLog(_T("[MainStep=%s] [SubStep=%s]"), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr());
			//ChangeSubState(eAutoTrayStep_Run_Table_Vacuum_Check_job);
			ChangeSubState(eAutoTrayStep_Run_ResetSchedule);
			
		}
	}break;	
	//========================================================
	// prepare pick & place
	//========================================================
	case eAutoTrayStep_Run_ResetSchedule:
	{
		MakeLog(_T("[MainStep=%s] [SubStep=%s]"), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr());
		FreeAllSchedule();
		ChangeSubState(eAutoTrayStep_Run_Table_Vacuum_Check_job);
	}break;
	// Table Vacuum Check Start
	case eAutoTrayStep_Run_Table_Vacuum_Check_job:
	{
		int nJob = SetVacuumCheck_TrayPP_Schedule(DEF_PICK, _eUsePicker_Unload);
		if (nJob == eExcutingStep_FindJob) {
		
			MakeLog(_T("[MainStep=%s] [SubStep=%s] eExcutingStep_FindJob"), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr());
			ChangeSubState(eAutoTrayStep_Run_Table_Vacuum_Check_Start);
		}	
		else if (nJob == eExcutingStep_ErrorJob) {
			MakeLog(_T("[MainStep=%s] [SubStep=%s] eExcutingStep_ErrorJob "), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr());
			ChangeSubState(eAutoTrayStep_Run_Table_Vacuum_Check_Safety_move);
		}
		
	}break;
	case eAutoTrayStep_Run_Table_Vacuum_Check_Start:
	{
		if (m_pTaskTrayPP->GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			CTrayUnloadPPCmd_MovePickPlace* pPpCmd = (CTrayUnloadPPCmd_MovePickPlace*)GetNextSchedule();
			switch (pPpCmd->m_Cmd.eStage)
			{
			case LOC_IDX_TRAY_PP_TBL_1:
			case LOC_IDX_TRAY_PP_TBL_2:
			case LOC_IDX_TRAY_PP_TBL_3:
			case LOC_IDX_TRAY_PP_TBL_4:
			{
				g_TaskAutoTrayUnloadPP.MakeLog("[work=%s check area", m_pTaskTrayPP->GetLocName(pPpCmd->m_Cmd.eStage));

				// teaching 영역 벗어날 경우에는 release하지 않아야 함.
				double dTargetPos = m_pTaskTrayPP->GetTeachPos(pPpCmd->m_Cmd.eStage, pPpCmd->m_Cmd.Pocket, pPpCmd->m_Cmd.vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisY);
				double dTargetPosPitchY = m_pTaskTrayPP->GetTeachPos(pPpCmd->m_Cmd.eStage, pPpCmd->m_Cmd.Pocket, pPpCmd->m_Cmd.vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisPitchY);

				// 안전영역에 대한 Y축 계산.
				double dLoadTablePitchY = (double)g_DMCont.m_dmTrayPP.GN(NDM5_TrayPP_LdTbl_Pitch_Y);
				//double dWorkEnableMin_um = MIN_TRAY_PP_WORK_ENABLE_POS_um - (DEF_TRAY_PP_MAX_Y_PITCH_WIDE - dLoadTablePitchY - 10000.0);

				// Y축 offset 만큼 빠지기 때문에 hand도 계산 되어야 한다.
				OneAxis* pMotorPitchY = m_pTaskTrayPP->GetAxis(CTaskTrayBasePP::eAxisPitchY);
				double dPosiLimit = pMotorPitchY->GetSwLimitPosi();
				double dWorkEnablePitchY = dPosiLimit - (60000.0 - dLoadTablePitchY);


				double dWorkEnableMin_um = INTERLOCK_UD_TRAYPP_Y_WORK_ENABLE_POS_TRANSFER*(-1);
				if (dTargetPos < dWorkEnableMin_um) {//MIN_TRAY_PP_WORK_ENABLE_POS_um) {
					g_TaskAutoTrayUnloadPP.MakeLog("[Converyor=%s release_work_area 1", m_pTaskTrayPP->GetLocName(pPpCmd->m_Cmd.eStage));
					bool bReserveWork = g_conveyor_work_area.reserve_work_area(this);
					if (bReserveWork != true) {
						int home_sensor = g_TaskTransfer.GetAxis(CTaskTransfer::AXIS_TRANSFER_X)->IsHomeSensorOn() == TRUE;
						int motion_done = g_TaskTransfer.GetAxis(CTaskTransfer::AXIS_TRANSFER_X)->MotionDone();
						g_TaskAutoTrayUnloadPP.MakeLog("[Transfer Status [HomeSensor=%d] [motion_done=%d] [GetSeqCmdStatus=%d]", home_sensor, motion_done, g_TaskTransfer.GetSeqCmdStatus());
						SAFE_DELETE(pPpCmd);
						ChangeSubState(eAutoTrayStep_Run_Table_Vacuum_Check_job);
						return;
					}
					// 					bool bTrayPP_Safety2 = g_TaskTrayUnloadPP.is_safety_state();
					// 					if(bTrayPP_Safety2 == true)
					// 						g_conveyor_work_area.release_work_area(this);
					//g_conveyor_work_area.release_work_area(this);
				}
				else {
					bool bReserveWork = g_conveyor_work_area.reserve_work_area(this);
					if (bReserveWork != true) {
						int home_sensor = g_TaskTransfer.GetAxis(CTaskTransfer::AXIS_TRANSFER_X)->IsHomeSensorOn() == TRUE;
						int motion_done = g_TaskTransfer.GetAxis(CTaskTransfer::AXIS_TRANSFER_X)->MotionDone();
						g_TaskAutoTrayUnloadPP.MakeLog("[Transfer Status [HomeSensor=%d] [motion_done=%d] [GetSeqCmdStatus=%d]", home_sensor, motion_done, g_TaskTransfer.GetSeqCmdStatus());
						SAFE_DELETE(pPpCmd);
						ChangeSubState(eAutoTrayStep_Run_Table_Vacuum_Check_job);
						return;
					}
					g_TaskAutoTrayUnloadPP.MakeLog("[Converyor=%s reserve_work_area 2", m_pTaskTrayPP->GetLocName(pPpCmd->m_Cmd.eStage));
				}
			}
			}
		
		

		
			MakeLog(_T("[MainStep=%s] [SubStep=%s]"), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr());
			if (pPpCmd != nullptr)
			{
				pPpCmd->m_Cmd.bVacuumCheckFuction = TRUE;
				if (SendCommand(*m_pTaskTrayPP, pPpCmd) == eSeqCmdAck_OK) {

				}
			}
			ChangeSubState(eAutoTrayStep_Run_Table_Vacuum_Check_Wait);
		}
	}break;
	case eAutoTrayStep_Run_Table_Vacuum_Check_Wait:
	{
		int nHandStatus = m_pTaskTrayPP->GetSeqCmdStatus();
		if (nHandStatus == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			MakeLog(_T("[MainStep=%s] [SubStep=%s]"), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr());
			ChangeSubState(eAutoTrayStep_Run_Table_Vacuum_Check_job);
		}

	}break;

	case eAutoTrayStep_Run_Table_Vacuum_Check_Safety_move:
	{
		MakeLog(_T("[MainStep=%s] [SubStep=%s]"), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr());
		CTrayUnloadPPCmd_MovePos* pCmd = new CTrayUnloadPPCmd_MovePos;
		pCmd->m_Cmd.eStage = (eLocIdxTrayPP)(LOC_IDX_TRAY_PP_TBL_1);
		pCmd->m_Cmd.Pocket = CPoint(0, 0);
		std::vector<CPoint> vPicker = { CPoint(0, 0) };
		pCmd->m_Cmd.vPicker = vPicker;
		if (SendCommand(*m_pTaskTrayPP, pCmd) == eSeqCmdAck_OK) {
			g_conveyor_work_area.release_work_area(this);
			ChangeSubState(eAutoTrayStep_Run_Table_Vacuum_Check_Safety_end);
		}
	}break;
	case eAutoTrayStep_Run_Table_Vacuum_Check_Safety_end:
	{
		int nHandStatus = m_pTaskTrayPP->GetSeqCmdStatus();
		if (nHandStatus == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{		
			g_DMCont.m_dmShuttle.SB(BDM3_TABLE_UNLOAD_VACUUM_CHECK, FALSE);
			if (g_DMCont.m_dmShuttle.GB(BDM3_TABLE_LOAD_VACUUM_CHECK) == FALSE && g_DMCont.m_dmShuttle.GB(BDM3_TABLE_UNLOAD_VACUUM_CHECK) == FALSE)
			{
				ChangeSubState(eAutoTrayStep_Run_MakeSchedule);
				MakeLog(_T("[MainStep=%s] [SubStep=%s]"), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr());
				break;
			}
			
		}
	}break;
	// Table Vacuum Check End



	//========================================================
	// find job
	//========================================================
	case eAutoTrayStep_Run_MakeSchedule:
	{
		if (m_pTaskTrayPP->GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready) {
			// job check
			LoadingTblSetReserve();

			if (m_listTrayPpSchedule.empty() == true)
			{
			

				int  ScheduleCount = 0;
				BOOL bAutoGrrUse   = g_TaskSystemCtrl.GetAutoGrrUse();
				BOOL bAutoLoopUse  = g_TaskSystemCtrl.GetAutoLoopUse();

				if (bAutoGrrUse == TRUE) {
					ScheduleCount = MakeTrayPpScheduleGrr();
				}
				else if (bAutoLoopUse == TRUE) { //기능 구현 안되있음.
				}
				else {
					ScheduleCount = MakeTrayPpSchedule();
				}
				

				// have jop
				if (ScheduleCount > 0) {
					ChangeSubState(eAutoTrayStep_Run_TrayPpPickPlaceStart);
					break;
				}
				// no more Job
				else
				{
					if (m_nPickPlaceStep == eTrayPpPickPlaceSubCmd_Idle && m_nChkSafetyPosCnt > (MAX_LD_TBL_POCKET_CNT_X*MAX_LD_TBL_POCKET_CNT_Y))
					{
						m_nChkSafetyPosCnt = 0;
						if (g_conveyor_work_area.get_reserved_task() == this &&  m_listTrayPpSchedule.size() == 0)
						{
							if (static_cast<CTaskTrayUnloadPP*>(m_pTaskTrayPP)->is_safety_state() == true) {
								g_conveyor_work_area.release_work_area(this);
								break;
							}
							else {
								// tray pp 작업이 없는데 예약했으면, 안전 위치로 회피해서 예약 풀어버린다.
								ChangeSubState(eAutoTrayStep_Run_TrayPP_MoveSafetyPos);
								break;
							}
						}
						else
						{
							int nHandStatus = m_pTaskTrayPP->GetSeqCmdStatus();
							if (nHandStatus == CBaseSeqCmd::eSeqCmdStatus_Ready
								&& static_cast<CTaskTrayUnloadPP*>(m_pTaskTrayPP)->is_safety_state() == true
								&& g_TaskTrayUnloadPP.GetTrayPpDvcCnt() == 0)
							{
								int nWorkReserveTbl1 = g_TaskLoadTbl[eLoadTbl_1].GetWorkReserve();
								int nWorkReserveTbl4 = g_TaskLoadTbl[eLoadTbl_4].GetWorkReserve();
								int nJobTbl = -1;

								CTime cTime = CTime::GetCurrentTime();
								if (RID_INIT_IDLE_RESERVE == nWorkReserveTbl1 || RID_INIT_IDLE_RESERVE == nWorkReserveTbl4) {
									int nWorkReserveTbl[2] = { nWorkReserveTbl1 ,nWorkReserveTbl4 };
									int nUnloadTableIdx[2] = { eLoadTbl_1 ,eLoadTbl_4 };
									int nTestPPIdx = 0;

									if (RID_INIT_IDLE_RESERVE == nWorkReserveTbl1 && RID_INIT_IDLE_RESERVE == nWorkReserveTbl4)
									{
										ChangeSubState(eAutoTrayStep_Run_TrayPpPickPlaceWait);
										break;
									}

									if (RID_TRAY_PP_(eTrayPP_2_UD) == nWorkReserveTbl1 || RID_TRAY_PP_(eTrayPP_2_UD) == nWorkReserveTbl4)
									{
										ChangeSubState(eAutoTrayStep_Run_TrayPpPickPlaceWait);
										break;
									}

									for (int i = 0; i < _countof(nUnloadTableIdx); i++) {
										if (g_TaskAutoTestPp[nTestPPIdx].GetModuleSocketOnCnt() > 0 && nWorkReserveTbl[i] == RID_INIT_IDLE_RESERVE
											&&  g_TaskLoadTbl[nUnloadTableIdx[i]].GetUnloadDeviceCnt() > 0) {
											//cTime = g_TaskLoadTbl[nUnloadTableIdx[i]].GetMoveSetplateTm();
											nJobTbl = nUnloadTableIdx[i];
										}
										nTestPPIdx++;
									}

									CPoint pocket(0, 0);

									int nEqpOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_EQP_No);
									if (nEqpOption > DEF_EQUIPMENT_UNIT1) // 2호기 부터는 Pocket 0,1 위치가 Safety 위치로 변경 됨
									{
										pocket = CPoint(0, 1);
									}
									std::vector< CPoint > vPicker = { CPoint(0, 0) };

									if (nJobTbl >= 0)
									{
										int nChkTbl_X = m_pTaskTrayPP->ChkMotorPos(eLocIdxTrayPP(LOC_IDX_TRAY_PP_TBL_1 + nJobTbl), pocket, vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX);
										int nChkTbl_Y = m_pTaskTrayPP->ChkMotorPos(eLocIdxTrayPP(LOC_IDX_TRAY_PP_TBL_1 + nJobTbl), pocket, vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisY);
										if (nChkTbl_X != ERR_NO_ERROR || nChkTbl_Y != ERR_NO_ERROR /*nJobTbl != m_nJobTbl*/)
										{
											MakeLog(_T("eAutoTrayStep_Run_TrayPP_MoveTableWaitPos [MainStep=%s] [SubStep=%s] Before=%d, After=%d"), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr(), m_nJobTbl, m_nJobTbl);
											m_nJobTbl = nJobTbl;
											ChangeSubState(eAutoTrayStep_Run_TrayPP_MoveTableWaitPos);
											break;
										}
									}
								}
							}
						}
					}
					else {
						m_nChkSafetyPosCnt++;
					}
					ChangeSubState(eAutoTrayStep_Run_TrayPpPickPlaceWait);
					break;
				}
			}
			// remain job ( JobCount > 0 )
			else
			{
				ChangeSubState(eAutoTrayStep_Run_TrayPpPickPlaceStart);
				break;
			}
		}
		// ready 상태가 아니면 무조건 할일을 끝내고 난뒤에 생각하여야 한다.
		ChangeSubState(eAutoTrayStep_Run_TrayPpPickPlaceWait);
	}break;

	//========================================================
	//loop entry point - auto scheduling start
	//========================================================
	case eAutoTrayStep_Run_TrayPreMoveLoadTblStart:
	{
		MakeLog(_T("[MainStep=%s] [SubStep=%s]"), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr());

		CTrayUnloadPPCmd_MovePos* pPpCmd = (CTrayUnloadPPCmd_MovePos*)GetNextSchedule();

		if (pPpCmd != nullptr) {
			if (SendCommand(*m_pTaskTrayPP, pPpCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eAutoTrayStep_Run_TrayPpPickPlaceWait);
				break;
			}
		}
		else {
			/*no more job*/
			ChangeSubState(eAutoTrayStep_Run_MakeSchedule);
			break;
		}
	}break;

	//========================================================
	// Pick Place Start
	//========================================================
	case eAutoTrayStep_Run_TrayPpPickPlaceStart:
	{
		MakeLog(_T("[MainStep=%s] [SubStep=%s]"), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr());

		CTrayUnloadPPCmd_MovePickPlace* pPpCmd = (CTrayUnloadPPCmd_MovePickPlace*)GetNextSchedule();
		if (pPpCmd != nullptr) {

			switch (pPpCmd->m_Cmd.eStage)
			{
			case LOC_IDX_TRAY_PP_CONV_2:
			case LOC_IDX_TRAY_PP_CONV_3:
			case LOC_IDX_TRAY_PP_CONV_4:
			case LOC_IDX_TRAY_PP_CONV_5:
			case LOC_IDX_TRAY_PP_CONV_6:
			case LOC_IDX_TRAY_PP_CONV_7:
			{
				g_TaskAutoTrayUnloadPP.MakeLog("[Converyor=%s reserve_work_area", m_pTaskTrayPP->GetLocName(pPpCmd->m_Cmd.eStage));
				bool bReserveWork = g_conveyor_work_area.reserve_work_area(this);
				if (bReserveWork != true) {
					g_TaskAutoTrayUnloadPP.MakeLog("[Converyor=%s reserve_work_area escape", m_pTaskTrayPP->GetLocName(pPpCmd->m_Cmd.eStage));
					SAFE_DELETE(pPpCmd);
					ChangeSubState(eAutoTrayStep_Run_TrayPpPickPlaceWait);
					return;
				}
			}break;

			case LOC_IDX_TRAY_PP_TBL_1:
			case LOC_IDX_TRAY_PP_TBL_2:
			case LOC_IDX_TRAY_PP_TBL_3:
			case LOC_IDX_TRAY_PP_TBL_4:
			{
				g_TaskAutoTrayUnloadPP.MakeLog("[work=%s check area", m_pTaskTrayPP->GetLocName(pPpCmd->m_Cmd.eStage));

				// teaching 영역 벗어날 경우에는 release하지 않아야 함.
				double dTargetPos = m_pTaskTrayPP->GetTeachPos(pPpCmd->m_Cmd.eStage, pPpCmd->m_Cmd.Pocket, pPpCmd->m_Cmd.vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisY);
				double dTargetPosPitchY = m_pTaskTrayPP->GetTeachPos(pPpCmd->m_Cmd.eStage, pPpCmd->m_Cmd.Pocket, pPpCmd->m_Cmd.vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisPitchY);

				// 안전영역에 대한 Y축 계산.
				double dLoadTablePitchY  = (double)g_DMCont.m_dmTrayPP.GN(NDM5_TrayPP_LdTbl_Pitch_Y);
				//double dWorkEnableMin_um = MIN_TRAY_PP_WORK_ENABLE_POS_um - (DEF_TRAY_PP_MAX_Y_PITCH_WIDE - dLoadTablePitchY - 10000.0);

				// Y축 offset 만큼 빠지기 때문에 hand도 계산 되어야 한다.
				OneAxis* pMotorPitchY    = m_pTaskTrayPP->GetAxis(CTaskTrayBasePP::eAxisPitchY);
				double dPosiLimit        = pMotorPitchY->GetSwLimitPosi();
				double dWorkEnablePitchY = dPosiLimit - (60000.0 - dLoadTablePitchY);


				double dWorkEnableMin_um = INTERLOCK_UD_TRAYPP_Y_WORK_ENABLE_POS_TRANSFER*(-1);
				if (dTargetPos < dWorkEnableMin_um) {//MIN_TRAY_PP_WORK_ENABLE_POS_um) {
					g_TaskAutoTrayUnloadPP.MakeLog("[Converyor=%s release_work_area 1", m_pTaskTrayPP->GetLocName(pPpCmd->m_Cmd.eStage));				
					bool bReserveWork = g_conveyor_work_area.reserve_work_area(this);
					if (bReserveWork != true) {				
						int home_sensor = g_TaskTransfer.GetAxis(CTaskTransfer::AXIS_TRANSFER_X)->IsHomeSensorOn() == TRUE;
						int motion_done = g_TaskTransfer.GetAxis(CTaskTransfer::AXIS_TRANSFER_X)->MotionDone();
						g_TaskAutoTrayUnloadPP.MakeLog("[Transfer Status [HomeSensor=%d] [motion_done=%d] [GetSeqCmdStatus=%d]", home_sensor, motion_done, g_TaskTransfer.GetSeqCmdStatus());
						SAFE_DELETE(pPpCmd);
						ChangeSubState(eAutoTrayStep_Run_TrayPpPickPlaceWait);
						return;
					}
					// 					bool bTrayPP_Safety2 = g_TaskTrayUnloadPP.is_safety_state();
					// 					if(bTrayPP_Safety2 == true)
					// 						g_conveyor_work_area.release_work_area(this);
					//g_conveyor_work_area.release_work_area(this);
				}
				else {
					bool bReserveWork = g_conveyor_work_area.reserve_work_area(this);
					if (bReserveWork != true) {
						int home_sensor = g_TaskTransfer.GetAxis(CTaskTransfer::AXIS_TRANSFER_X)->IsHomeSensorOn() == TRUE;
						int motion_done = g_TaskTransfer.GetAxis(CTaskTransfer::AXIS_TRANSFER_X)->MotionDone();
						g_TaskAutoTrayUnloadPP.MakeLog("[Transfer Status [HomeSensor=%d] [motion_done=%d] [GetSeqCmdStatus=%d]", home_sensor, motion_done, g_TaskTransfer.GetSeqCmdStatus());
						SAFE_DELETE(pPpCmd);
						ChangeSubState(eAutoTrayStep_Run_TrayPpPickPlaceWait);
						return;
					}
					g_TaskAutoTrayUnloadPP.MakeLog("[Converyor=%s reserve_work_area 2", m_pTaskTrayPP->GetLocName(pPpCmd->m_Cmd.eStage));
				}


// 				if (dTargetPos > MIN_TRAY_PP_WORK_ENABLE_POS_um || (dWorkEnableMin_um < dTargetPos && dTargetPosPitchY < dWorkEnablePitchY)) {
// 					g_TaskAutoTrayUnloadPP.MakeLog("[Converyor=%s release_work_area 1", m_pTaskTrayPP->GetLocName(pPpCmd->m_Cmd.eStage));
// 					bool bTrayPP_Safety2 = g_TaskTrayUnloadPP.is_safety_state();
// 					if(bTrayPP_Safety2 == true)
// 						g_conveyor_work_area.release_work_area(this);
// 				}
// 				else {
// 					bool bReserveWork = g_conveyor_work_area.reserve_work_area(this);
// 					if (bReserveWork != true) {
// 						SAFE_DELETE(pPpCmd);
// 						ChangeSubState(eAutoTrayStep_Run_TrayPpPickPlaceWait);
// 						return;
// 					}
// 					g_TaskAutoTrayUnloadPP.MakeLog("[Converyor=%s reserve_work_area 2", m_pTaskTrayPP->GetLocName(pPpCmd->m_Cmd.eStage));
// 				}

			}break;
			default:
				ASSERT(false);
				break;
			}

			g_TaskAutoTrayUnloadPP.MakeLog("[EXEC] [Step=%8d] [Work :: CmdStage=%s, Xpos:%d, Ypos:%d, PickerCnt=%d, Pick/Place=%s]"
				, m_nAutoRunSubStep, m_pTaskTrayPP->GetLocName(pPpCmd->m_Cmd.eStage)
				, pPpCmd->m_Cmd.Pocket.x + 1, pPpCmd->m_Cmd.Pocket.y + 1
				, (int)pPpCmd->m_Cmd.vPicker.size(), (pPpCmd->m_Cmd.ePickPlace == ePick ? _T("Pick") : _T("Place")));

			if (SendCommand(*m_pTaskTrayPP, pPpCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eAutoTrayStep_Run_TrayPpPickPlaceWait);
				break;
			}
		}
		else {
			/*no more job*/
			ChangeSubState(eAutoTrayStep_Run_MakeSchedule);
			break;
		}
	}break;

	//========================================================
	// Move Safety Pos
	//========================================================
	case eAutoTrayStep_Run_TrayPP_MoveSafetyPos:
	{
		int nHandStatus = m_pTaskTrayPP->GetSeqCmdStatus();
		if (nHandStatus == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{

			//Tray PP 에 Load Device 가 있으면 Table 위치로 이동 
			CTrayUnloadPPCmd_MovePos* pCmd = NULL;

			CTime cTime = CTime::GetCurrentTime();
			int nJobTbl = -1;

			if (m_pTaskTrayPP->GetTrayPpUnloadDvcCnt() == 0) {
				if ((g_TaskSystemCtrl.IsOneCycleModeOn() == DEF_OFF)
					|| g_TaskSystemCtrl.IsOneCycleModeOn() == DEF_ON || g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == true)
				{
					int nUnloadTableIdx[2] = { eLoadTbl_1 ,eLoadTbl_4};
					int nTestPPIdx = 0;
					for (int i = 0; i < _countof(nUnloadTableIdx); i++) {
						if (g_TaskLoadTbl[nUnloadTableIdx[i]].GetMoveSetplateTm() < cTime && g_TaskAutoTestPp[nTestPPIdx].GetModuleSocketOnCnt() > 0) {
							cTime = g_TaskLoadTbl[nUnloadTableIdx[i]].GetMoveSetplateTm();
							nJobTbl = nUnloadTableIdx[i];					
						}
						nTestPPIdx++;
					}
				}
			}

			pCmd = new CTrayUnloadPPCmd_MovePos;
			if (nJobTbl != -1) {
				pCmd->m_Cmd.eStage  = (eLocIdxTrayPP)(LOC_IDX_TRAY_PP_TBL_1+ nJobTbl);
			}
			else {
				pCmd->m_Cmd.eStage = (eLocIdxTrayPP)(LOC_IDX_TRAY_PP_SAFETY);
			}
				

			CPoint pocket(0, 0);

			int nEqpOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_EQP_No);
			if (nEqpOption > DEF_EQUIPMENT_UNIT1) // 2호기 부터는 Pocket 0,1 위치가 Safety 위치로 변경 됨
			{
// 				int nProjectOption = g_DMCont.m_dmEQP.GN(NDM11_Handler_Project_Name);
// 				if (nProjectOption == DEF_PROJECT_WATT) {
// 					pocket = CPoint(0, 1);
// 				}
// 				else if (nProjectOption == DEF_PROJECT_CARPENTERS) {
// 					pocket = CPoint(0, 0);
// 				}
				pocket = CPoint(0, 1);
			}
			std::vector< CPoint > vPicker = { CPoint(0, 0) };
			pCmd->m_Cmd.Pocket  = pocket;
			pCmd->m_Cmd.vPicker = vPicker;

			if (SendCommand(*m_pTaskTrayPP, pCmd) == eSeqCmdAck_OK) {
				g_conveyor_work_area.release_work_area(this);
				CString strCmdName = _T("");
				if (nJobTbl != -1) {
					strCmdName.Format(_T("Alreay Table %d"), nJobTbl + 1);
				}
				else {
					strCmdName.Format(_T("Safety"));
				}
				MakeLog(_T("[MainStep=%s] [SubStep=%s] [Move %s Pos]"), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr(), strCmdName);
				ChangeSubState(eAutoTrayStep_Run_TrayPpPickPlaceWait);
				break;
			}			
		}
	}break;
	case eAutoTrayStep_Run_TrayPP_MoveTableWaitPos:
	{
		int nHandStatus = m_pTaskTrayPP->GetSeqCmdStatus();
		if (nHandStatus == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{

			//Tray PP 에 Load Device 가 있으면 Table 위치로 이동 
			CTrayUnloadPPCmd_MovePos* pCmd = NULL;

			pCmd = new CTrayUnloadPPCmd_MovePos;

			if (m_nJobTbl != -1) {
				pCmd->m_Cmd.eStage = (eLocIdxTrayPP)(LOC_IDX_TRAY_PP_TBL_1 + m_nJobTbl);
			}
			else {
				SAFE_DELETE(pCmd);
				ChangeSubState(eAutoTrayStep_Run_TrayPpPickPlaceWait);
				break;
			}

			CPoint pocket(0, 0);

			int nEqpOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_EQP_No);
			if (nEqpOption > DEF_EQUIPMENT_UNIT1) // 2호기 부터는 Pocket 0,1 위치가 Safety 위치로 변경 됨
			{
				pocket = CPoint(0, 1);
			}
			std::vector< CPoint > vPicker = { CPoint(0, 0) };
			pCmd->m_Cmd.Pocket = pocket;
			pCmd->m_Cmd.vPicker = vPicker;

			if (SendCommand(*m_pTaskTrayPP, pCmd) == eSeqCmdAck_OK) {
				g_conveyor_work_area.release_work_area(this);
				CString strCmdName = _T("");
				if (m_nJobTbl != -1) {
					strCmdName.Format(_T("Alreay Table %d"), m_nJobTbl + 1);
				}
				else {
					strCmdName.Format(_T("Safety"));
				}
				MakeLog(_T("[MainStep=%s] [SubStep=%s] [Move %s Pos]"), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr(), strCmdName);
				ChangeSubState(eAutoTrayStep_Run_TrayPpPickPlaceWait);
				break;
			}
		}
	}
	//========================================================
	// Pick Place Wait
	//========================================================
	case eAutoTrayStep_Run_TrayPpPickPlaceWait:
	{
		int nHandStatus = m_pTaskTrayPP->GetSeqCmdStatus();
		if (nHandStatus == CBaseSeqCmd::eSeqCmdStatus_Ready) {
			ChangeSubState(eAutoTrayStep_Run_TrayPpComptCheck);
			break;
		}
	}break;

	//========================================================
	// Comp Check
	//========================================================
	case eAutoTrayStep_Run_TrayPpComptCheck:
	{	
//		LoadingTblSetReserve();
		ChangeSubState(eAutoTrayStep_Run_MakeSchedule);
	}break;
	}
}

