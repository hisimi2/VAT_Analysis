#include "stdafx.h"
#include "VatLoadTblCmd_MovePos.h"


CVatLoadTblCmd_MovePos::CVatLoadTblCmd_MovePos(int nMode) : m_nMode(nMode)
{
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));

	m_nTableIdx = 0;

	for (int nAxis = 0; nAxis < VAT_MAX_AXIS_YROTATE; nAxis++) {
		m_dCmdPos[nAxis] = 0.0;
	}
}


CVatLoadTblCmd_MovePos::~CVatLoadTblCmd_MovePos()
{
}


int CVatLoadTblCmd_MovePos::CheckParam()
{
	m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
	m_tcTimeout.StartTimer();

	return ERR_NO_ERROR;
}


int  CVatLoadTblCmd_MovePos::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}


int  CVatLoadTblCmd_MovePos::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

int  CVatLoadTblCmd_MovePos::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}


int  CVatLoadTblCmd_MovePos::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}


int  CVatLoadTblCmd_MovePos::OnResume()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}


BOOL CVatLoadTblCmd_MovePos::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}


BOOL CVatLoadTblCmd_MovePos::State_Executing()
{
	CTaskLoadTable* pTaskLoadTbl = (CTaskLoadTable*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		//pTaskLoadTbl->MakeVatLog(_T("[CMD: %s] [EXECUTE] [Step=%s]"), m_szCmdName, act_string(m_nStep));
	}
	m_nStep_Pre = m_nStep;

	for (int i = 0; i < CTaskLoadTable::eMaxAxisCount; i++) {
		OneAxis* pMotor = pTaskLoadTbl->GetAxis(i);
		if (pMotor != NULL)
		{
			if (pMotor->ChkMotorUsable() != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Load Table %d check motor usable Cmd Error", m_nTableIdx + 1);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(ERR_LOAD_TBL_1_MOTOR_ALARMED + m_nTableIdx, AlarmData);
				return FALSE;
			}
		}
	}

// 	int nTestPPIdx = (pTaskLoadTbl->m_nTableIdx / 2);
// 	if (m_nCmdStage >= LOC_IDX_LD_TBL_TEST_SIDE_1 && g_TaskTestPP[nTestPPIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready /* && vLocList[CTaskLoadTable::eAxisY]> CTaskLoadTable::eYPOS_LoadSide*/)
// 	{
// 		int nTestPPType = g_TaskTestPP[nTestPPIdx].m_nTestPPType;
// 
// 		std::vector<int>vLocTestPpList;
// 		vLocTestPpList.clear();
// 		g_TaskTestPP[nTestPPIdx].WhereIsMotorPos(nTestPPType, vLocTestPpList, CTaskTestPP::eAxisAll);
// 
// 		int nErrSenZ = g_TaskTestPP[nTestPPIdx].Chk_Z_Safety_Sen_All(nTestPPType);
// 		int nAxisX = (nTestPPType == eTestPPLoadType) ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2;
// 		if (nErrSenZ != ERR_NO_ERROR && vLocTestPpList[nAxisX] != CTaskTestPP::ePosX_StationAndPosiLimit) {
// 			return FALSE;
// 		}
// 	}

	OneAxis* pMotorY = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
	OneAxis* pMotorRotate = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisRotate);

	switch (m_nStep)
	{
		case move_start:
		{
			pTaskLoadTbl->MakeVatLog("[%s EXEC=%8d] [MODE=%d] [%s move] [START]", m_szCmdName, m_nStep, m_nMode, m_strCmdStage);

			m_tcTimeout.SetCheckTime((MOTOR_TIME_OUT_MIDDLE / 3.0));
			m_tcTimeout.StartTimer();
			m_nStep = target_site_chk_before_move;
		}break;
		case target_site_chk_before_move:
		{
			double dTPosY = m_dCmdPos[CTaskLoadTable::eAxisY];

			double dCurPosY = 0.0;
			pMotorY->GetCurrentPos(dCurPosY);

			int nErr = -1;
			if (dTPosY < INTERLOCK_LOADING_TBL_POS_BOUNDARY || dCurPosY < INTERLOCK_LOADING_TBL_POS_BOUNDARY)
			{
				nErr = g_TaskAutoLdTable[m_nTableIdx].ChkMoveLoadSite();
				if (nErr != ERR_NO_ERROR) {
					break;
				}
			}

			if (dTPosY > INTERLOCK_LOADING_TBL_POS_BOUNDARY || dCurPosY > INTERLOCK_LOADING_TBL_POS_BOUNDARY)
			{
				nErr = g_TaskAutoLdTable[m_nTableIdx].ChkMoveTestSite();
				if (nErr != ERR_NO_ERROR)
					break;
			}

			pTaskLoadTbl->MakeVatLog("[%s EXEC=%8d] [Target Site Check]", m_szCmdName, m_nStep);

			m_nStep = move_table_y_prepos;
		}break;
		case move_table_y_prepos:
		{
			int nTblType = pTaskLoadTbl->GetTableUsage();
			if (nTblType == eUnloadType)
			{
				double dTPosY = m_dCmdPos[CTaskLoadTable::eAxisY];

				double dCurPosY = 0.0;
				pMotorY->GetCurrentPos(dCurPosY);

				//현재 위치 or Target 위치.
				if (dCurPosY <= INTERLCOK_LOADING_TABLE_Y_STACKER_IMPACT_LOC || dTPosY <= INTERLCOK_LOADING_TABLE_Y_STACKER_IMPACT_LOC)
				{
					pTaskLoadTbl->MakeVatLog("[%s EXEC=%8d] [Move to %s Pre Position] [STR]", m_szCmdName, m_nStep, m_strCmdStage);

					int nErr = pTaskLoadTbl->do_VATMovePos(CTaskLoadTable::eAxisY, dTPosY, LOAD_TABLE_MOVE_OFFSET_Y);
					if (nErr != ERR_VAT_NO_ERROR)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("[Load Table %d] [Axis Y] pre move %s command error", m_nTableIdx + 1, m_strCmdStage);
						ReportAlarm(nErr, AlarmData);
						break;
					}

					m_tcTimeout.SetCheckTime((MOTOR_TIME_OUT_MIDDLE / 3.0));
					m_tcTimeout.StartTimer();

					m_nStep = move_table_y_prepos_chk;
					break;
				}
			}

			//Load Type일 경우.
			m_nStep = move_table_rotate_target_pos;

		}break;
		case move_table_y_prepos_chk:
		{
			int nAxisDone = pMotorY->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				pTaskLoadTbl->MakeVatLog("[%s EXEC=%8d] [Move to %s Pre Position] [END]", m_szCmdName, m_nStep, m_strCmdStage);
				m_nStep = move_table_rotate_target_pos;
			}
			else {
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotorY->GetAxisName());
					ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotorY->GetAxisName());
					ReportAlarm(ERR_LOAD_TBL_1_MOVE_FAIL + m_nTableIdx, AlarmData);
				}
			}
		}break;
		case move_table_rotate_target_pos:
		{
			pTaskLoadTbl->MakeVatLog("[%s] [EXEC=%8d] [Move Rotate to Target Pos] [STR]", m_szCmdName, m_nStep);

			double dTPosRotate = m_dCmdPos[CTaskLoadTable::eAxisRotate];
			int nErr = pTaskLoadTbl->do_VATMovePos(CTaskLoadTable::eAxisRotate, dTPosRotate);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Load Table %d, move rotate %s command error", m_nTableIdx + 1, m_strCmdStage);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorRotate->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}

			m_tcTimeout.SetCheckTime((MOTOR_TIME_OUT_SHORT));
			m_tcTimeout.StartTimer();
			m_nStep = move_table_rotate_target_pos_chk;
		}break;
		case move_table_rotate_target_pos_chk:
		{
			OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisRotate);

			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				pTaskLoadTbl->MakeVatLog("[%s] [EXEC=%8d] [Move Rotate to Target Pos] [END]", m_szCmdName, m_nStep);
				m_nStep = move_table_y_target_pos;
			}
			else {
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
					AlarmData.m_fnAddItemFormat("%s Axis", pMotor->GetAxisName());
					ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("%s Axis", pMotor->GetAxisName());
					ReportAlarm(nAxisDone, AlarmData);
				}
			}	
		}break;
		case move_table_y_target_pos:
		{
			pTaskLoadTbl->MakeVatLog("[%s] [EXEC=%8d] [Move Y to Target Pos] [STR]", m_szCmdName, m_nStep);

			double dTPosY = m_dCmdPos[CTaskLoadTable::eAxisY];
			int nErr = pTaskLoadTbl->do_VATMovePos(CTaskLoadTable::eAxisY, dTPosY);
			if (nErr != ERR_VAT_NO_ERROR)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("[Load Table %d] [Axis Y] move %s command error", m_nTableIdx + 1, m_strCmdStage);
				ReportAlarm(nErr, AlarmData);
				break;
			}

			m_tcTimeout.SetCheckTime((MOTOR_TIME_OUT_MIDDLE / 3.0));
			m_tcTimeout.StartTimer();

			m_nStep = move_table_y_target_pos_chk;
		}break;
		case move_table_y_target_pos_chk:
		{
			int nAxisDone = pMotorY->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				pTaskLoadTbl->MakeVatLog("[%s] [EXEC=%8d] [Move Y to Target Pos] [END]", m_szCmdName, m_nStep);
				m_nStep = move_end;
			}
			else {
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=Y ");
					ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Axis=Y");
					ReportAlarm(ERR_LOAD_TBL_1_MOVE_FAIL + m_nTableIdx, AlarmData);
				}
			}
		}break;
		case move_end:
		{
			pTaskLoadTbl->MakeVatLog("[%s EXEC=%8d] [MODE=%d] [%s move] [END]", m_szCmdName, m_nStep, m_nMode, m_strCmdStage);
			ReportCmdEnd(eSeqCmdStatus_NormalComplete);
		}break;
		default:
			break;
	}

	return CBaseSeqCmd::State_Executing();
}


//case 1000 ~
BOOL CVatLoadTblCmd_MovePos::State_Pause()
{
	CTaskLoadTable* pTaskLoadTbl = (CTaskLoadTable*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskLoadTbl->MakeLog("[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 1000:
	{
		int nAxisCount = CTaskLoadTable::eMaxAxisCount;
		for (int i = 0; i < nAxisCount; i++) {
			OneAxis* pMotor = pTaskLoadTbl->GetAxis(i);
			if (pMotor != NULL)
			{
				bool bCloseLimit = pMotor->CheckLimitClose();
				double dCurPos = 0;
				pMotor->GetCurrentPos(dCurPos);
				if (bCloseLimit == false)
				{
					pTaskLoadTbl->MakeVatLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
					pMotor->SetSlowStop();
				}
			}
		}
		m_nStep++;
	}break;
	case 1001:
	{

	}break;
	default:
	{
		m_nStep = 1000;
	}break;
	}

	return CBaseSeqCmd::State_Pause();
}


BOOL CVatLoadTblCmd_MovePos::State_Recovery()
{
	CTaskLoadTable* pTaskLoadTbl = (CTaskLoadTable*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskLoadTbl->MakeVatLog("[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 2000:
	{
		SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing);
	}break;
	default:
	{
		m_nStep = 2000;
	}break;
	}

	return CBaseSeqCmd::State_Recovery();
}


BOOL CVatLoadTblCmd_MovePos::State_NormalComplete()
{
	return CBaseSeqCmd::State_NormalComplete();
}


BOOL CVatLoadTblCmd_MovePos::State_Alarm()
{
	CTaskLoadTable* pTaskLoadTbl = (CTaskLoadTable*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskLoadTbl->MakeLog("[CMD: %s] [ALARM PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
		case 4000:
		{
			int nAxisCount = CTaskLoadTable::eMaxAxisCount;
			for (int i = 0; i < nAxisCount; i++) {
				OneAxis* pMotor = pTaskLoadTbl->GetAxis(i);
				if (pMotor != NULL)
				{
					bool bCloseLimit = pMotor->CheckLimitClose();
					double dCurPos = 0;
					pMotor->GetCurrentPos(dCurPos);
					if (bCloseLimit == false)
					{
						pTaskLoadTbl->MakeVatLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
						pMotor->SetSlowStop();
					}
				}
			}
			m_nStep++;
		}break;
		case 4001:
		{

		}break;
		default:
		{
			m_nStep = 4000;
		}break;
	}

	return CBaseSeqCmd::State_Alarm();
}


BOOL CVatLoadTblCmd_MovePos::State_AlarmComplete()
{
	CTaskLoadTable* pTaskLoadTbl = (CTaskLoadTable*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskLoadTbl->MakeVatLog("[CMD: %s] [ALARM COMP] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 5000:
	{
		return CBaseSeqCmd::State_AlarmComplete();
	}break;
	default: {
		m_nStep = 5000;
	}break;
	}

	return CBaseSeqCmd::State_AlarmComplete();
}


BOOL CVatLoadTblCmd_MovePos::State_Aborting()
{
	CTaskLoadTable* pTaskLoadTbl = (CTaskLoadTable*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskLoadTbl->MakeVatLog("[CMD: %s] [ABORT] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 6000:
	{
		int nAxisCount = CTaskLoadTable::eMaxAxisCount;
		for (int i = 0; i < nAxisCount; i++) {
			OneAxis* pMotor = pTaskLoadTbl->GetAxis(i);
			if (pMotor != NULL)
			{
				bool bCloseLimit = pMotor->CheckLimitClose();
				double dCurPos = 0;
				pMotor->GetCurrentPos(dCurPos);
				if (bCloseLimit == false)
				{
					pTaskLoadTbl->MakeVatLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
					pMotor->SetSlowStop();
				}
			}
		}
		m_nStep++;
	}break;
	case 6001:
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


BOOL CVatLoadTblCmd_MovePos::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}



CString CVatLoadTblCmd_MovePos::act_string(int act)
{
	BEGIN_CASE_TO_STR(act);
	CASE_TO_STR(move_start);
	CASE_TO_STR(target_site_chk_before_move);
	CASE_TO_STR(move_table_y_prepos);
	CASE_TO_STR(move_table_y_prepos_chk);
	CASE_TO_STR(move_table_rotate_target_pos);
	CASE_TO_STR(move_table_rotate_target_pos_chk);
	CASE_TO_STR(move_table_y_target_pos);
	CASE_TO_STR(move_table_y_target_pos_chk);
	CASE_TO_STR(move_end);
	ELSE_CASE(_T("unkown_step"));
	END_CASE_TO_STR();
}