#include "stdafx.h"
#include "StackerCmd_TrayOut.h"


CStackerCmd_TrayOut::CStackerCmd_TrayOut()
{
}


CStackerCmd_TrayOut::~CStackerCmd_TrayOut()
{
}

int CStackerCmd_TrayOut::CheckParam()
{
	/*m_nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);*/

	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_TrayOut::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_TrayOut::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_TrayOut::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_TrayOut::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_TrayOut::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_TrayOut::State_Setup()
{
	m_swTimeout.SetCheckTime(TIME_OUT_MIDDLE);
	m_swTimeout.StartTimer();

	return CBaseSeqCmd::State_Setup();
}

BOOL CStackerCmd_TrayOut::State_Executing()
{
	// check alarm
	CTaskStacker* pTask = (CTaskStacker*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTask->MakeLog( _T( "[CMD: %s] [EXECUTE] [Step=%8d]" ), m_szCmdName, m_nStep );
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
		case eExecuteStep_Chk_Tray_Sensor:
		{
			if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
			{
				m_nStep = eExecuteStep_End;
				break;
			}


			m_swTimeout.SetCheckTime(TIME_OUT_SHORT*2.0);
			m_swTimeout.StartTimer();

			//순서 변경 하지 말것
			//1. Stacker Front에 C-Tray 있는지 확인.
			//int nErr = g_TaskStacker[eSTACKER_03].ChkExistTray_FrontPos_Sen(eExist);
			int front_sensor = g_TaskStacker[eSTACKER_03].ChkDI(CTaskStacker::xC_TRAY_CV_FRONT_EXIST_SEN, true);
			int slow_sensor = g_TaskStacker[eSTACKER_03].ChkDI(CTaskStacker::xC_TRAY_CV_FRONT_SLOW_SEN, true);	
			if(front_sensor == ERR_NO_ERROR || slow_sensor == ERR_NO_ERROR)
			{
				m_nStep = eExecuteStep_Conveyor_Move;
				break;
			}

			//2. Feeder Place Position에 C-Tray 있는지 확인.
			int nErr = g_TaskTrayFeeder.ChkTrayExist_TransferArea_Sen(eExist);
			if (nErr == ERR_NO_ERROR/*TRUE*/) 
			{
				m_nStep = eExecuteStep_Stcker_Align_Cylinder_Forward;
				break;
			}

			//3. Stacker Rear에 C-Tray 있는지 확인.
			nErr = g_TaskStacker[eSTACKER_03].ChkExistTray_RearPos_Sen(eExist);
			if (nErr == ERR_NO_ERROR) {
				m_nStep = eExecuteStep_Stcker_Align_Cylinder_Backward;
				break;
			}

			//4. Feeder에 C-Tray 있는지 확인.
			nErr = g_TaskTrayFeeder.GetTrayExistSensor(); //B접
			if (nErr == TRUE) {
				m_nStep = eExecuteStep_Feeder_Move_Y;
				break;
			}

			//5. Tray 아무것도 없으면 End.
			m_nStep = eExecuteStep_End;
		}break;
		case eExecuteStep_Stcker_Align_Cylinder_Forward: //Transfer Area에 C-Tray 있을 때, Rear C-Tray 고정.
		{
			if (m_swTimeout.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				ReportAlarm(ERR_STACKER_3_TRAY_ALIGN_FWD_TIMEOUT, AlarmData);
				break;
			}

			int nErr = g_TaskStacker[eSTACKER_03].ChkExistTray_RearPos_Sen(eExist);
			if (nErr == ERR_NO_ERROR) {
				g_TaskStacker[eSTACKER_03].do_cyl_TrayAlign(DEF_FORW);
				Sleep(200);
				int chk_cyl_for = g_TaskStacker[eSTACKER_03].chk_cyl_TrayAlign(DEF_FORW);
				if (chk_cyl_for != ERR_NO_ERROR) {
					break;
				}
			}
		
			m_nStep = eExecuteStep_Conveyor_Move;
		}break;
		case eExecuteStep_Stcker_Align_Cylinder_Backward:
		{
			if (m_swTimeout.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				ReportAlarm(ERR_STACKER_3_TRAY_ALIGN_BKWD_TIMEOUT, AlarmData);
				break;
			}

			g_TaskStacker[eSTACKER_03].do_cyl_TrayAlign(DEF_BACK);
			Sleep(200);
			int chk_cyl_for = g_TaskStacker[eSTACKER_03].chk_cyl_TrayAlign(DEF_BACK);
			if (chk_cyl_for != ERR_NO_ERROR) {
				break;
			}

			m_nStep = eExecuteStep_Conveyor_Move;
		}break;
		case eExecuteStep_Feeder_Move_Y:
		{
			pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Feeder Move Place Pos (STR)", m_szCmdName, m_nStep);

			//Trasnfer영역에 C-Tray 있는지 확인.
			int chk_transfer_area = g_TaskTrayFeeder.ChkTrayExist_TransferArea_Sen(eExist);
			if (chk_transfer_area == ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat(_T("Stacker : 3"));
				ReportAlarm(ERR_FEEDER_TRANSFER_AREA_TRAY_DETECTED, AlarmData);
				break;
			}

			OneAxis* pMotor = (OneAxis*)g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eMotor::eAxisY);
			int nErr = g_TaskTrayFeeder.do_MovePos(CTaskTrayFeeder::eAxisY, eLocIdxFeeder::LOC_IDX_FEEDER_TRAY_PLACE_POS, 0, 50);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Feeder Move Fail.");
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}

			m_swTimeout.SetCheckTime(TIME_OUT_MIDDLE);
			m_swTimeout.StartTimer();
			m_nStep++;

		}break;
		case eExecuteStep_Chk_Feeder_Move_Y:
		{
			OneAxis* pMotor = (OneAxis*)g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eMotor::eAxisY);
			if (m_swTimeout.CheckOverTime()) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Feeder Motor Move Timeout.");
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(ERR_FEEDER_MOTOR_MOVE_TIMEOUT, AlarmData);
				break;
			}

			int nErr = g_TaskTrayFeeder.ChkMotorPos(CTaskTrayFeeder::eAxisY, LOC_IDX_FEEDER_TRAY_PLACE_POS);
			if (nErr == ERR_NO_ERROR) {
				m_swTimeout.SetCheckTime(TIME_OUT_SHORT);
				m_swTimeout.StartTimer();
				m_nStep++;
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Feeder Move Place Pos (END)", m_szCmdName, m_nStep);
				break;
			}
		}break;
		case eExecuteStep_Feeder_Align_Cyl_Back:
		{
			pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Feeder Align Cylinder Back (STR)", m_szCmdName, m_nStep);
			int nErr = g_TaskTrayFeeder.do_cyl_align_ForBack(eBack);
			
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Feeder Align Cylinder Fail.");
				ReportAlarm(ERR_FEEDER_ALIGN_CYLINDER_BACK_FAIL, AlarmData);
				break;
			}

			int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_ALIGN_WAITTIME);
			if (0 < nSleep) {
				Sleep(nSleep);
			}

			m_swTimeout.SetCheckTime(TIME_OUT_SHORT*2.0);
			m_swTimeout.StartTimer();
			m_nStep++;
		}break;
		case eExecuteStep_Chk_Feeder_Align_Cyl_Back:
		{
			if (m_swTimeout.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				g_Error.AlarmReport(ERR_FEEDER_ALIGN_CYLINDER_TIMEOUT, AlarmData, NULL);
				break;
			}

			auto nErr = g_TaskTrayFeeder.chk_cyl_align_ForBack(eBack);
			if (nErr == ERR_NO_ERROR) {
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Feeder Align Cylinder Back (END)", m_szCmdName, m_nStep);
				m_nStep++;
			}

		}break;
		case eExecuteStep_Feeder_Cyl_Down:
		{
			pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Feeder Cylinder Down (STR)", m_szCmdName, m_nStep);
			int nErr = g_TaskTrayFeeder.do_cyl_UpDown(eDown);

			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Feeder Cylinder Down Fail.");
				ReportAlarm(ERR_FEEDER_DOWN_CYLINDER_FAIL, AlarmData);
				break;
			}

			int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_UPDN_WAITTIME);
			if (0 < nSleep) {
				Sleep(nSleep);
			}

			m_swTimeout.SetCheckTime(TIME_OUT_SHORT*2.0);
			m_swTimeout.StartTimer();
			m_nStep++;
		}break;
		case eExecuteStep_Chk_Feeder_Cyl_Down:
		{
			if (m_swTimeout.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				g_Error.AlarmReport(ERR_FEEDER_UP_DOWN_CYLINDER_TIMEOUT, AlarmData, NULL);
				break;
			}

			auto nErr = g_TaskTrayFeeder.chk_cyl_UpDown(eDown);
			if (nErr == ERR_NO_ERROR) {
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Feeder Cylinder Down (END)", m_szCmdName, m_nStep);
				m_nStep++;
			}
		}break;
		case eExecuteStep_Feeder_Unclamp:
		{
			pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Feeder Cylinder Unclamp (STR)", m_szCmdName, m_nStep);
			int nErr = g_TaskTrayFeeder.do_cyl_ClampUnclamp(eUnclamp);

			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Feeder Cylinder Unclamp Fail.");
				ReportAlarm(ERR_FEEDER_UNCLAMP_CYLINDER_FAIL, AlarmData);
				break;
			}

			int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_CLAMP_WAITTIME);
			if (0 < nSleep) {
				Sleep(nSleep);
			}

			m_swTimeout.SetCheckTime(TIME_OUT_SHORT*2.0);
			m_swTimeout.StartTimer();
			m_nStep++;
		}break;
		case eExecuteStep_Chk_Feeder_Unclamp:
		{
			if (m_swTimeout.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				g_Error.AlarmReport(ERR_FEEDER_CLAMP_CYLINDER_TIMEOUT, AlarmData, NULL);
				break;
			}

			auto nErr = g_TaskTrayFeeder.chk_cyl_ClampUnclamp(eUnclamp);
			int transfer_area_tray_exist = g_TaskTrayFeeder.ChkTrayExist_TransferArea_Sen(eExist);
			if (nErr == ERR_NO_ERROR && transfer_area_tray_exist == ERR_NO_ERROR) {
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Feeder Cylinder Unclamp (END)", m_szCmdName, m_nStep);
				m_nStep++;
			}
		}break;
		case eExecuteStep_Feeder_Up:
		{
			pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Feeder Cylinder Up (STR)", m_szCmdName, m_nStep);
			int nErr = g_TaskTrayFeeder.do_cyl_UpDown(eUp);

			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Feeder Cylinder Up Fail.");
				ReportAlarm(ERR_FEEDER_DOWN_CYLINDER_FAIL, AlarmData);
				break;
			}

			int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_UPDN_WAITTIME);
			if (0 < nSleep) {
				Sleep(nSleep);
			}

			m_swTimeout.SetCheckTime(TIME_OUT_SHORT*2.0);
			m_swTimeout.StartTimer();
			m_nStep++;
		}break;
		case eExecuteStep_Chk_Feeder_Up:
		{
			if (m_swTimeout.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				g_Error.AlarmReport(ERR_FEEDER_UP_DOWN_CYLINDER_TIMEOUT, AlarmData, NULL);
				break;
			}

			auto nErr = g_TaskTrayFeeder.chk_cyl_UpDown(eUp);
			if (nErr == ERR_NO_ERROR) {
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Feeder Cylinder Up (END)", m_szCmdName, m_nStep);
				m_nStep = eExecuteStep_Conveyor_Move;
			}
		}break;
		case eExecuteStep_Conveyor_Move:
		{
			int nErr = g_TaskStacker[eSTACKER_03].ConveyorMoving(eConveyorMove_BKWD, false);
			if (nErr == ERR_NO_ERROR) {
				m_nStep++;
				m_swTimeout.SetCheckTime(TIME_OUT_MIDDLE*2.0);
				m_swTimeout.StartTimer();
			}
			else
			{
				CParamList AlarmData;
				int nStackerIndex = pTask->GetIndex();
				AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), nStackerIndex + 1);
				ReportAlarm(ERR_STACKER_1_INTERLOCK_CONVEYOR_DRIVER_NOT_WORK + nStackerIndex, AlarmData);
				break;
			}
		}break;
		case eExecuteStep_Chk_Front_Sen:
		{
			if (m_swTimeout.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				ReportAlarm(ERR_STACKER_3_REAR_TRAY_EXIST_SENSOR_DETECT_TIMEOUT, AlarmData);
				break;
			}

			int nErr = g_TaskStacker[eSTACKER_03].ChkDI(CTaskStacker::xC_TRAY_CV_FRONT_EXIST_SEN, true);
			if (nErr == ERR_NO_ERROR) {
				m_nStep++;
			}
		}break;
		case eExecuteStep_Main_Pusher_Up:
		{
			pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] main pusher Up (STR)", m_szCmdName, m_nStep);
			int nErr = g_TaskStacker[eSTACKER_03].do_cyl_MainPusher(DEF_UP);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
				ReportAlarm(nErr, AlarmData);
				break;
			}

			int nSleep = g_DMCont.m_dmStacker.GN(NDM7_STK_MAIN_UPDN_WAITTIME);
			if (0 < nSleep) {
				Sleep(nSleep);
			}
			m_nStep++;
			m_swTimeout.ResetStartTime();

		}break;
		case eExecuteStep_Chk_Main_Pusher_Up:
		{
			if (m_swTimeout.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				ReportAlarm(ERR_STACKER_3_MAIN_PUSHER_UP_TIMEOUT, AlarmData);
				break;
			}

			int nErr = g_TaskStacker[eSTACKER_03].chk_cyl_MainPusher(DEF_UP);
			if (nErr == ERR_NO_ERROR) {
				m_nStep++;
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] main pusher Up (END)", m_szCmdName, m_nStep);
			}
		}break;
		case eExecuteStep_Conveyor_Move_Stop:
		{
			pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] conveyor stop (STR)", m_szCmdName, m_nStep);
			pTask->ConveyorStop();
			pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] conveyor stop (END)", m_szCmdName, m_nStep);

			m_nStep++;
		}break;
		case eExecuteStep_Stacker_Unclamp:
		{	
			pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] stacker cylinder Unclamp (STR)", m_szCmdName, m_nStep);
			int nErr = g_TaskStacker[eSTACKER_03].do_cyl_LockUnlock(DEF_UNCLAMP);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
				ReportAlarm(nErr, AlarmData);
				break;
			}

			int nSleep = g_DMCont.m_dmStacker.GN(NDM7_STK_CLAMP_WAITTIME);
			if (0 < nSleep) {
				Sleep(nSleep);
			}

			m_nStep++;
			m_swTimeout.SetCheckTime(TIME_OUT_SHORT*2);
			m_swTimeout.StartTimer();
		}break;
		case eExecuteStep_Chk_Stacker_Unclamp:
		{
			if (m_swTimeout.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
				ReportAlarm(ERR_STACKER_3_UNCLAMP_TIMEOUT, AlarmData);
				break;
			}

			int nErr = pTask->chk_cyl_LockUnlock(DEF_UNCLAMP);
			if (nErr == ERR_NO_ERROR) {
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d]  stacker cylinder Unclamp (END)", m_szCmdName, m_nStep);
				m_nStep++;
			}
		}break;
		case eExecuteStep_Sub_Pusher_Up:
		{
			pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] sub pusher Up (STR)", m_szCmdName, m_nStep);
			int nErr = g_TaskStacker[eSTACKER_03].do_cyl_SubPusher(DEF_UP);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
				ReportAlarm(nErr, AlarmData);
				break;
			}

			int nSleep = g_DMCont.m_dmStacker.GN(NDM7_STK_MAIN_UPDN_WAITTIME);
			if (0 < nSleep) {
				Sleep(nSleep);
			}
			m_nStep++;
			m_swTimeout.ResetStartTime();

		}break;
		case eExecuteStep_Chk_Sub_Pusher_Up:
		{
			if (m_swTimeout.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				ReportAlarm(ERR_STACKER_3_SUB_PUSHER_UP_TIMEOUT, AlarmData);
				break;
			}

			int nErr = g_TaskStacker[eSTACKER_03].chk_cyl_SubPusher(DEF_UP);
			if (nErr == ERR_NO_ERROR) {
				m_nStep++;
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] sub pusher Up (END)", m_szCmdName, m_nStep);
			}
		}break;
		case eExecuteStep_Stacker_Clamp:
		{
			pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] stacker cylinder Clamp (STR)", m_szCmdName, m_nStep);
			int nErr = g_TaskStacker[eSTACKER_03].do_cyl_LockUnlock(DEF_CLAMP);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
				ReportAlarm(nErr, AlarmData);
				break;
			}

			int nSleep = g_DMCont.m_dmStacker.GN(NDM7_STK_CLAMP_WAITTIME);
			if (0 < nSleep) {
				Sleep(nSleep);
			}

			m_nStep++;
			m_swTimeout.SetCheckTime(TIME_OUT_SHORT);
			m_swTimeout.StartTimer();
		}break;
		case eExecuteStep_Chk_Stacker_Clamp:
		{
			if (m_swTimeout.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
				ReportAlarm(ERR_STACKER_3_CLAMP_TIMEOUT, AlarmData);
				break;
			}

			int nErr = pTask->chk_cyl_LockUnlock(DEF_CLAMP);
			if (nErr == ERR_NO_ERROR) {
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d]  stacker cylinder Clamp (END)", m_szCmdName, m_nStep);
				m_nStep++;
			}
		}break;
		case eExecuteStep_Sub_Pusher_Down:
		{
			pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] sub pusher Down (STR)", m_szCmdName, m_nStep);
			int nErr = g_TaskStacker[eSTACKER_03].do_cyl_SubPusher(DEF_DOWN);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
				ReportAlarm(nErr, AlarmData);
				break;
			}

			int nSleep = g_DMCont.m_dmStacker.GN(NDM7_STK_MAIN_UPDN_WAITTIME);
			if (0 < nSleep) {
				Sleep(nSleep);
			}
			m_nStep++;
			m_swTimeout.ResetStartTime();
		}break;
		case eExecuteStep_Chk_Sub_Pusher_Down:
		{
			if (m_swTimeout.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				ReportAlarm(ERR_STACKER_3_SUB_PUSHER_DN_TIMEOUT, AlarmData);
				break;
			}

			int nErr = g_TaskStacker[eSTACKER_03].chk_cyl_SubPusher(DEF_DOWN);
			if (nErr == ERR_NO_ERROR) {
				m_nStep++;
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] sub pusher Down (END)", m_szCmdName, m_nStep);
			}
		}break;
		case eExecuteStep_Main_Pusher_Down:
		{
			pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] main pusher Down (STR)", m_szCmdName, m_nStep);
			int nErr = g_TaskStacker[eSTACKER_03].do_cyl_MainPusher(DEF_DOWN);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
				ReportAlarm(nErr, AlarmData);
				break;
			}

			int nSleep = g_DMCont.m_dmStacker.GN(NDM7_STK_MAIN_UPDN_WAITTIME);
			if (0 < nSleep) {
				Sleep(nSleep);
			}
			m_nStep++;
			m_swTimeout.ResetStartTime();
		}break;
		case eExecuteStep_Chk_Main_Pusher_Down:
		{
			if (m_swTimeout.CheckOverTime() == TRUE) {
				CParamList AlarmData;
				ReportAlarm(ERR_STACKER_3_MAIN_PUSHER_UP_TIMEOUT, AlarmData);
				break;
			}

			int nErr = g_TaskStacker[eSTACKER_03].chk_cyl_MainPusher(DEF_DOWN);
			if (nErr == ERR_NO_ERROR) {
				m_nStep = eExecuteStep_Chk_Tray_Sensor;
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] main pusher Down (END)", m_szCmdName, m_nStep);
			}
		}break;
		case eExecuteStep_End:
		{
			ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
		}break;
	}

	return CBaseSeqCmd::State_Executing();
}

BOOL CStackerCmd_TrayOut::State_Pause()
{
	CTaskStacker* pTask = (CTaskStacker*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTask->MakeLog(_T("[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]"), m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0:
	{
		pTask->ConveyorSlowStop();
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
BOOL CStackerCmd_TrayOut::State_Recovery()
{
	CTaskStacker* pTask = (CTaskStacker*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTask->MakeLog(_T("[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%d]"), m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0:
	{
		m_swTimeout.ResetStartTime();
		SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing, 0/*m_nLast_Do_Step*/);
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
BOOL CStackerCmd_TrayOut::State_NormalComplete()
{
	CTaskStacker* pTask = (CTaskStacker*)m_pReceive;
	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_TrayOut::State_Alarm()
{
	CTaskStacker* pTask = (CTaskStacker*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTask->MakeLog(_T("[CMD: %s] [ALARM] [Step=%8d] [LastStep=%d]"), m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0: // alarm 발생 했을 때 처리해 줘야 할꺼 처리 해줌
	{
		pTask->ConveyorSlowStop();
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
	return CBaseSeqCmd::State_Alarm();
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_TrayOut::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_TrayOut::State_Aborting()
{
	CTaskStacker* pTask = (CTaskStacker*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTask->MakeLog(_T("[CMD: %s] [ABORT] [Step=%8d]"), m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0:
	{
		pTask->ConveyorSlowStop();
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
BOOL CStackerCmd_TrayOut::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}
