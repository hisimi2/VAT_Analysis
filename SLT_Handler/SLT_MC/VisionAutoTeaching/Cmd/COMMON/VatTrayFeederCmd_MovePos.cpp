#include "stdafx.h"
#include "VatTrayFeederCmd_MovePos.h"


CVatTrayFeederCmd_MovePos::CVatTrayFeederCmd_MovePos(int nMode) : m_nMode(nMode)
{
	m_dCmdpos = 0.0;
	//m_dCmdOffset = 0.0;
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));
}


CVatTrayFeederCmd_MovePos::~CVatTrayFeederCmd_MovePos()
{
}


int CVatTrayFeederCmd_MovePos::CheckParam()
{
	return 0;
}


int  CVatTrayFeederCmd_MovePos::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}


int  CVatTrayFeederCmd_MovePos::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}


int  CVatTrayFeederCmd_MovePos::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}


int  CVatTrayFeederCmd_MovePos::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}


int  CVatTrayFeederCmd_MovePos::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}


BOOL CVatTrayFeederCmd_MovePos::State_Setup()
{
	m_swTimeout.SetCheckTime(TIME_OUT_MIDDLE);
	m_swTimeout.StartTimer();

	return CBaseSeqCmd::State_Setup();
}


BOOL  CVatTrayFeederCmd_MovePos::State_Executing()
{
	CTaskTrayFeeder* pTask = (CTaskTrayFeeder*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%s]"), m_szCmdName, act_string(m_nStep));
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case move_start:
	{
		pTask->MakeVatLog("[%s EXEC=%8d] [MODE=%d] [Target Position move] [START]", m_szCmdName, m_nStep, m_nMode);

		auto align_for_state = pTask->get_cyl_align_ForBack(eForw);
		auto align_back_state = pTask->get_cyl_align_ForBack(eBack);
		if (align_for_state != ERR_NO_ERROR && align_back_state != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Align cylinder is abnormal state.");
			AlarmData.m_fnAddItemFormat(align_for_state.msg.c_str());
			AlarmData.m_fnAddItemFormat(align_back_state.msg.c_str());
			ReportAlarm(ERR_FEEDER_ALIGN_CYLINDER_ABNORMAL, AlarmData);
			break;
		}

		auto clamp_state = pTask->get_cyl_ClampUnclamp(eClamp);
		auto unclamp_state = pTask->get_cyl_ClampUnclamp(eUnclamp);
		if (clamp_state != ERR_NO_ERROR && unclamp_state != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Clamper is abnormal state.");
			AlarmData.m_fnAddItemFormat(clamp_state.msg.c_str());
			AlarmData.m_fnAddItemFormat(unclamp_state.msg.c_str());
			ReportAlarm(ERR_FEEDER_CLAMP_CYLINDER_ABNORMAL, AlarmData);
			break;
		}

		m_swTimeout.SetCheckTime(TIME_OUT_SHORT);
		m_swTimeout.StartTimer();
		m_nStep = feeder_cyl_up_chk;
	}break;

	case feeder_cyl_up_chk:
	{
		if (m_swTimeout.CheckOverTime()) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Feeder Up Timeout.");
			ReportAlarm(ERR_FEEDER_UP_DOWN_CYLINDER_TIMEOUT, AlarmData);
			break;
		}

		auto chk = pTask->chk_cyl_UpDown(eUp);
		if (chk != ERR_NO_ERROR) {
			pTask->do_cyl_UpDown(eUp);

			int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_UPDN_WAITTIME);
			if (0 < nSleep) {
				Sleep(nSleep);
			}
		}

		chk = pTask->chk_cyl_UpDown(eUp);
		if (chk == ERR_NO_ERROR) {
			m_nStep = move_feeder_y;
			break;
		}
	}break;

	case move_feeder_y:
	{
		int nErr = pTask->do_VATMovePos(m_dCmdpos);
		//int err = pTask->do_MovePos(CTaskTrayFeeder::eAxisY, m_nCmdStage, m_nTrayRowStep);
		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Feeder Move Fail.");
			OneAxis* pMotor = pTask->GetAxis(CTaskTrayFeeder::eAxisY);
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(nErr, AlarmData);
			break;
		}

		m_swTimeout.SetCheckTime(TIME_OUT_LONG);
		m_swTimeout.StartTimer();
		m_nStep = move_feeder_y_chk;
	}break;
	case move_feeder_y_chk:
	{
		if (m_swTimeout.CheckOverTime()) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Feeder Motor Move Timeout.");
			ReportAlarm(ERR_FEEDER_MOTOR_MOVE_TIMEOUT, AlarmData);
			break;
		}

		OneAxis* pMotorY = pTask->GetAxis(CTaskTrayFeeder::eAxisY);
		int nAxisDone = pMotorY->AxisDone();

		int nErr = pTask->ChkVATMotorPos(m_dCmdpos);
		//int err = pTask->ChkMotorPos(CTaskTrayFeeder::eAxisY, m_nCmdStage, m_nTrayRowStep);
		if (nErr == ERR_NO_ERROR && nAxisDone == ERR_NO_ERROR) {
			m_swTimeout.SetCheckTime(TIME_OUT_SHORT);
			m_swTimeout.StartTimer();
			m_nStep = move_end;
			break;
		}
		else
		{
			if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Feeder Motor Move Timeout.");
				AlarmData.m_fnAddItemFormat("Axis=%s ", pMotorY->GetAxisName());
				ReportAlarm(ERR_FEEDER_MOTOR_MOVE_FAIL, AlarmData);
				break;
			}
		}
	}break;

	case move_end:
	{
		pTask->MakeVatLog("[%s EXEC=%8d] [MODE=%d] [Target Position move] [END]", m_szCmdName, m_nStep, m_nMode);
		ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
	}break;
	}

	return CBaseSeqCmd::State_Executing();
}


BOOL  CVatTrayFeederCmd_MovePos::State_Pause()
{
	CTaskTrayFeeder* pTask = (CTaskTrayFeeder*)m_pReceive;
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
	case 1000:
	{
		int err = ERR_NO_ERROR;
		for (int i = 0; i < CTaskTrayFeeder::eAxisMax; i++) {
			OneAxis* pMotor = pTask->GetAxis((CTaskTrayFeeder::eMotor)i);
			if (pMotor != nullptr)
			{
				int stop_result = pMotor->SetSlowStop();
				if (stop_result != ERR_NO_ERROR) {
					err = stop_result;
				}
			}
		}

		// 			if( err != ERR_NO_ERROR ) {
		// 				CParamList AlarmData;
		// 				AlarmData.m_fnAddItemFormat( "Feeder Motor Stop Fail." );
		// 				ReportAlarm( ERR_FEEDER_MOTOR_STOP_FAIL, AlarmData );
		// 				break;
		// 			}

		m_swTimeout.SetCheckTime(TIME_OUT_SHORT);
		m_swTimeout.StartTimer();
		m_nStep++;
	}break;
	case 1001:
	{
		if (m_swTimeout.CheckOverTime()) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Feeder Motor Stop Timeout.");
			ReportAlarm(ERR_FEEDER_MOTOR_STOP_TIMEOUT, AlarmData);
			break;
		}

		int motion_done_cnt = 0;
		for (int i = 0; i < CTaskTrayFeeder::eAxisMax; i++) {
			OneAxis* pMotor = pTask->GetAxis((CTaskTrayFeeder::eMotor)i);
			if (pMotor == nullptr)
			{
				motion_done_cnt++;
				continue;
			}
			motion_done_cnt += pMotor->MotionDone() == ERR_NO_ERROR ? 1 : 0;
		}

		if (motion_done_cnt == CTaskTrayFeeder::eAxisMax) {
			m_nStep++;
			break;
		}
	}break;
	case 1002:
	{

	}break;
	default:
	{
		m_nStep = 1000;
	}break;
	}

	return CBaseSeqCmd::State_Pause();
}


BOOL  CVatTrayFeederCmd_MovePos::State_Recovery()
{
	CTaskTrayFeeder* pTask = (CTaskTrayFeeder*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTask->MakeLog("[CMD: %s] [RECOVERY] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 2000:
	{
		auto align_for_state = pTask->get_cyl_align_ForBack(eForw);
		auto align_back_state = pTask->get_cyl_align_ForBack(eBack);
		if (align_for_state != ERR_NO_ERROR && align_back_state != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Align cylinder is abnormal state.");
			AlarmData.m_fnAddItemFormat(align_for_state.msg.c_str());
			AlarmData.m_fnAddItemFormat(align_back_state.msg.c_str());
			ReportAlarm(ERR_FEEDER_ALIGN_CYLINDER_ABNORMAL, AlarmData);
			break;
		}

		auto clamp_state = pTask->get_cyl_ClampUnclamp(eClamp);
		auto unclamp_state = pTask->get_cyl_ClampUnclamp(eUnclamp);
		if (clamp_state != ERR_NO_ERROR && unclamp_state != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Clamper is abnormal state.");
			AlarmData.m_fnAddItemFormat(clamp_state.msg.c_str());
			AlarmData.m_fnAddItemFormat(unclamp_state.msg.c_str());
			ReportAlarm(ERR_FEEDER_CLAMP_CYLINDER_ABNORMAL, AlarmData);
			break;
		}

		auto up_state = pTask->get_cyl_UpDown(eUp);
		auto down_state = pTask->get_cyl_UpDown(eDown);
		if (up_state != ERR_NO_ERROR && down_state != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Up Down is abnormal state.");
			AlarmData.m_fnAddItemFormat(up_state.msg.c_str());
			AlarmData.m_fnAddItemFormat(down_state.msg.c_str());
			ReportAlarm(ERR_FEEDER_UP_DOWN_CYLINDER_ABNORMAL, AlarmData);
			break;
		}

		m_swTimeout.SetCheckTime(TIME_OUT_SHORT);
		m_swTimeout.StartTimer();
		m_nStep++;
	}break;
	case 2001:
	{
		pTask->do_cyl_UpDown(eUp);

		int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_UPDN_WAITTIME);
		if (0 < nSleep) {
			Sleep(nSleep);
		}

		auto chk = pTask->chk_cyl_UpDown(eUp);
		if (m_swTimeout.CheckOverTime()) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Feeder Up Timeout.");
			AlarmData.m_fnAddItemFormat(chk.msg.c_str());
			ReportAlarm(ERR_FEEDER_UP_DOWN_CYLINDER_TIMEOUT, AlarmData);
			break;
		}

		if (chk == ERR_NO_ERROR) {
			m_nStep++;
			m_swTimeout.SetCheckTime(TIME_OUT_MIDDLE);
			m_swTimeout.StartTimer();
			break;
		}
	}break;
	case 2002:
	{
		if (m_swTimeout.CheckOverTime()) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Feeder Move Timeout.");
			ReportAlarm(ERR_FEEDER_MOTOR_MOVE_TIMEOUT, AlarmData);
			break;
		}

		auto ret = pTask->do_MovePos(CTaskTrayFeeder::eAxisY, eLocIdxFeeder::LOC_IDX_FEEDER_SAFETY, 0, 50);
		if (ret != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Feeder Move Fail.");
			OneAxis* pMotor = pTask->GetAxis(CTaskTrayFeeder::eAxisY);
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(ret, AlarmData);
			break;
		}

		m_swTimeout.SetCheckTime(TIME_OUT_LONG);
		m_swTimeout.StartTimer();
		m_nStep++;
	}break;
	case 2003:
	{
		auto ret = pTask->ChkMotorPos(CTaskTrayFeeder::eAxisY, eLocIdxFeeder::LOC_IDX_FEEDER_SAFETY);
		if (m_swTimeout.CheckOverTime()) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Feeder Move Timeout.");
			ReportAlarm(ERR_FEEDER_MOTOR_MOVE_TIMEOUT, AlarmData);
			break;
		}

		if (ret == ERR_NO_ERROR) {
			ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_Executing);
			m_nStep = 2010;
			break;
		}
	}break;
	case 2010:
	{

	}break;
	default:
	{
		m_nStep = 2000;
	}break;
	}

	return CBaseSeqCmd::State_Recovery();
}


BOOL  CVatTrayFeederCmd_MovePos::State_NormalComplete()
{
	return CBaseSeqCmd::State_NormalComplete();
}


BOOL  CVatTrayFeederCmd_MovePos::State_Alarm()
{
	CTaskTrayFeeder* pTask = (CTaskTrayFeeder*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTask->MakeLog("[CMD: %s] [ALARM] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 4000:
	{
		int err = ERR_NO_ERROR;
		for (int i = 0; i < CTaskTrayFeeder::eAxisMax; i++) {
			OneAxis* pMotor = pTask->GetAxis((CTaskTrayFeeder::eMotor)i);
			if (pMotor != nullptr)
			{
				int stop_result = pMotor->SetSlowStop();
				if (stop_result != ERR_NO_ERROR) {
					err = stop_result;
				}
			}
		}

		// 			if( err != ERR_NO_ERROR ) {
		// 				CParamList AlarmData;
		// 				AlarmData.m_fnAddItemFormat( "Feeder Motor Stop Fail." );
		// 				ReportAlarm( ERR_FEEDER_MOTOR_STOP_FAIL, AlarmData );
		// 				break;
		// 			}

		m_swTimeout.SetCheckTime(TIME_OUT_SHORT);
		m_swTimeout.StartTimer();
		m_nStep++;
	}break;
	case 4001:
	{
		if (m_swTimeout.CheckOverTime()) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Feeder Motor Stop Timeout.");
			ReportAlarm(ERR_FEEDER_MOTOR_STOP_TIMEOUT, AlarmData);
			break;
		}

		int motion_done_cnt = 0;
		for (int i = 0; i < CTaskTrayFeeder::eAxisMax; i++) {
			OneAxis* pMotor = pTask->GetAxis((CTaskTrayFeeder::eMotor)i);
			if (pMotor == nullptr)
			{
				motion_done_cnt++;
				continue;
			}
			motion_done_cnt += pMotor->MotionDone() == ERR_NO_ERROR ? 1 : 0;
		}

		if (motion_done_cnt == CTaskTrayFeeder::eAxisMax) {
			m_nStep++;
			break;
		}
	}break;
	case 4002:
	{
		ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete); // 명령 진행 중 STOP 명령을 받아 명령이 중단 됨.
	}break;
	default:
	{
		m_nStep = 4000;
	}break;
	}

	return CBaseSeqCmd::State_Alarm();
}

//5000
BOOL  CVatTrayFeederCmd_MovePos::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete();
}

//6000
BOOL  CVatTrayFeederCmd_MovePos::State_Aborting()
{
	CTaskTrayFeeder* pTask = (CTaskTrayFeeder*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTask->MakeLog("[CMD: %s] [ABORT] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 6000:
	{
		int err = ERR_NO_ERROR;
		for (int i = 0; i < CTaskTrayFeeder::eAxisMax; i++) {
			OneAxis* pMotor = pTask->GetAxis((CTaskTrayFeeder::eMotor)i);
			if (pMotor != nullptr)
			{
				int stop_result = pMotor->SetSlowStop();
				if (stop_result != ERR_NO_ERROR) {
					err = stop_result;
				}
			}
		}

		// 			if( err != ERR_NO_ERROR ) {
		// 				CParamList AlarmData;
		// 				AlarmData.m_fnAddItemFormat( "Feeder Motor Stop Fail." );
		// 				ReportAlarm( ERR_FEEDER_MOTOR_STOP_FAIL, AlarmData );
		// 				break;
		// 			}

		m_swTimeout.SetCheckTime(TIME_OUT_SHORT);
		m_swTimeout.StartTimer();
		m_nStep++;
	}break;
	case 6001:
	{
		if (m_swTimeout.CheckOverTime()) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Feeder Motor Stop Timeout.");
			ReportAlarm(ERR_FEEDER_MOTOR_STOP_TIMEOUT, AlarmData);
			break;
		}

		int motion_done_cnt = 0;
		for (int i = 0; i < CTaskTrayFeeder::eAxisMax; i++) {
			OneAxis* pMotor = pTask->GetAxis((CTaskTrayFeeder::eMotor)i);
			if (pMotor == nullptr)
			{
				motion_done_cnt++;
				continue;
			}
			motion_done_cnt += pMotor->MotionDone() == ERR_NO_ERROR ? 1 : 0;

		}

		if (motion_done_cnt == CTaskTrayFeeder::eAxisMax) {
			m_nStep++;
			break;
		}
	}break;
	case 6002:
	{
		ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AbortComplete); // 명령 진행 중 STOP 명령을 받아 명령이 중단 됨.
	}break;
	default:
	{
		m_nStep = 6000;
	}break;
	}

	return CBaseSeqCmd::State_Aborting();
}


BOOL  CVatTrayFeederCmd_MovePos::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}

CString CVatTrayFeederCmd_MovePos::act_string(int act)
{
	BEGIN_CASE_TO_STR(act);
	CASE_TO_STR(move_start);
	CASE_TO_STR(feeder_cyl_up_chk);
	CASE_TO_STR(move_feeder_y);
	CASE_TO_STR(move_feeder_y_chk);
	CASE_TO_STR(move_end);
	ELSE_CASE(_T("unkown_step"));
	END_CASE_TO_STR();
}