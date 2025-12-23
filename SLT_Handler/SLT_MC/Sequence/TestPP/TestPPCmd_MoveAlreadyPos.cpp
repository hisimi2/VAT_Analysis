#include "StdAfx.h"
#include "TestPPCmd_MoveAlreadyPos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

CTestPPCmd_MoveAlreadyPos::CTestPPCmd_MoveAlreadyPos(void)
{
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));
    m_nCmdId = eSeqCmd_TestPP_MoveAlreadyPos;
    
	m_nCmdStage    = 0;
	m_nTestPpIdx   = 0;
	m_nMaxRetryCnt = 0;	
	m_nRetryCount  = 0;
	m_nHandType = 0;
	for(int i=0; i<CTaskTestPP::eAxisMax; i++){
		m_dLastPosErrRange[i] = MOTOR_INPOSITION; // [um]
		m_dInPosRange[i]      = MOTOR_INPOSITION; // [um]
		m_pMotor[i]           = NULL;
		m_dLastPos[i] = NULL;
	}

	m_vCmdPicker.clear();

	m_strStageName.Empty();
	m_bXmoveTarget = FALSE;

	m_tcTimeout = 0.0;
	m_tcTimeoutPicker =0.0;
	m_bAutoRunXHomePos = FALSE;
}


CTestPPCmd_MoveAlreadyPos::~CTestPPCmd_MoveAlreadyPos(void)
{
}


//==============================================================================
//
//==============================================================================
int CTestPPCmd_MoveAlreadyPos::CheckParam()
{
	CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
	m_nTestPpIdx = pTaskTestPP->m_nTestPPIdx;

	m_nMaxRetryCnt = g_DMCont.m_dmTestPP.GN(NDM4_TestPP1_RetryCnt + m_nTestPpIdx);

	m_pMotor[CTaskTestPP::eAxisX1] = pTaskTestPP->GetAxis(CTaskTestPP::eAxisX1);
	m_pMotor[CTaskTestPP::eAxisY] = pTaskTestPP->GetAxis(CTaskTestPP::eAxisY);
	m_pMotor[CTaskTestPP::eAxisX2] = pTaskTestPP->GetAxis(CTaskTestPP::eAxisX2);
	m_pMotor[CTaskTestPP::eAxisZ1] = pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ1);
	m_pMotor[CTaskTestPP::eAxisZ2] = pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ2);
	m_pMotor[CTaskTestPP::eAxisPitch_HW_X1_SW_Y1] = pTaskTestPP->GetAxis(CTaskTestPP::eAxisPitch_HW_X1_SW_Y1);
	m_pMotor[CTaskTestPP::eAxisPitch_HW_Y1_SW_X1] = pTaskTestPP->GetAxis(CTaskTestPP::eAxisPitch_HW_Y1_SW_X1);
	m_pMotor[CTaskTestPP::eAxisPitch_HW_X2_SW_Y2] = pTaskTestPP->GetAxis(CTaskTestPP::eAxisPitch_HW_X2_SW_Y2);
	m_pMotor[CTaskTestPP::eAxisPitch_HW_Y2_SW_X2] = pTaskTestPP->GetAxis(CTaskTestPP::eAxisPitch_HW_Y2_SW_X2);

	for (int i = 0; i < CTaskTestPP::eAxisMax; i++)
	{
		m_pMotor[i]->GetCurrentPos(m_dLastPos[i]);
	}

	m_strStageName = pTaskTestPP->GetLocName(m_nCmdStage);

	m_nWorkCmdStage = m_nCmdStage;

    return ERR_NO_ERROR;
}


//==============================================================================
//
//==============================================================================
int  CTestPPCmd_MoveAlreadyPos::OnStop()
{
    SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
    return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTestPPCmd_MoveAlreadyPos::OnRetry()
{
    SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
    return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTestPPCmd_MoveAlreadyPos::OnSkip()
{
    ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
    return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTestPPCmd_MoveAlreadyPos::OnPause()
{
    SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
    return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTestPPCmd_MoveAlreadyPos::OnResume()
{
    SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
    return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_MoveAlreadyPos::State_Setup()
{
	//m_cCmdPocket = CPoint(0,0);
	//m_vCmdPicker.clear();
	//m_vCmdPicker.push_back(CPoint(0,0));

    return CBaseSeqCmd::State_Setup();
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_MoveAlreadyPos::State_Executing()
{
    // check alarm
    CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
    if( m_nStep_New != -1 ){
        m_nStep = m_nStep_New;
        m_nStep_New = -1;
    }
    if( m_nStep_Pre != m_nStep ){
    //    pTaskTestPP->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d]", m_szCmdName, m_nStep);
    }
    m_nStep_Pre = m_nStep;

    int i=0;
    for( i=0; i<CTaskTestPP::eAxisMax; i++){
        OneAxis* pMotor = pTaskTestPP->GetAxis(i);
        if( pMotor->ChkMotorUsable() != ERR_NO_ERROR ){
            CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
            ReportAlarm(ERR_TEST_PP_1_MOTOR_ALARMED + m_nTestPpIdx, AlarmData);
            return FALSE;
        }
    }

    switch( m_nStep )
    {		
	case 0: // check interlock
		{
			double dErr[CTaskTestPP::eAxisMax]  = { 0, };
			double dTPos[CTaskTestPP::eAxisMax] = { 0, };
			pTaskTestPP->GetTeachPos(m_nCmdStage, m_cCmdPocket, m_vCmdPicker, CTaskTestPP::eZPOS_SAFETY, dTPos, m_nHandType);
			
			double dCurPos[CTaskTestPP::eAxisMax] = { 0, };
			for (int i = 0; i < CTaskTestPP::eAxisMax; i++)
			{
				m_pMotor[CTaskTestPP::eAxisX1]->GetCurrentPos(dCurPos[i]);
			}

			// Teaching 위치와 현재 위치가 일치 하는지 확인.
			dErr[CTaskTestPP::eAxisX1] = fabs(dCurPos[CTaskTestPP::eAxisX1] - dTPos[CTaskTestPP::eAxisX1]);
			dErr[CTaskTestPP::eAxisZ1] = fabs(dCurPos[CTaskTestPP::eAxisZ1] - dTPos[CTaskTestPP::eAxisZ1]);
			dErr[CTaskTestPP::eAxisPitch_HW_X1_SW_Y1] = fabs(dCurPos[CTaskTestPP::eAxisPitch_HW_X1_SW_Y1] - dTPos[CTaskTestPP::eAxisPitch_HW_X1_SW_Y1]);
			dErr[CTaskTestPP::eAxisPitch_HW_Y1_SW_X1] = fabs(dCurPos[CTaskTestPP::eAxisPitch_HW_Y1_SW_X1] - dTPos[CTaskTestPP::eAxisPitch_HW_Y1_SW_X1]);

			dErr[CTaskTestPP::eAxisY] = fabs(dCurPos[CTaskTestPP::eAxisY] - dTPos[CTaskTestPP::eAxisY]);

			dErr[CTaskTestPP::eAxisX2] = fabs(dCurPos[CTaskTestPP::eAxisX2] - dTPos[CTaskTestPP::eAxisX2]);
			dErr[CTaskTestPP::eAxisZ2] = fabs(dCurPos[CTaskTestPP::eAxisZ2] - dTPos[CTaskTestPP::eAxisZ2]);
			dErr[CTaskTestPP::eAxisPitch_HW_X2_SW_Y2] = fabs(dCurPos[CTaskTestPP::eAxisPitch_HW_X2_SW_Y2] - dTPos[CTaskTestPP::eAxisPitch_HW_X2_SW_Y2]);
			dErr[CTaskTestPP::eAxisPitch_HW_Y2_SW_X2] = fabs(dCurPos[CTaskTestPP::eAxisPitch_HW_Y2_SW_X2] - dTPos[CTaskTestPP::eAxisPitch_HW_Y2_SW_X2]);

			int nChkTeachSame = 0;
			for( int i=0; i < CTaskTestPP::eAxisMax; i++ )
			{
				if( dErr[i] <= m_dInPosRange[i] )
					nChkTeachSame++;
			}

			// 축 이동이 완료 되어진 상태 이므로 motor 동작이 필요 없음.
			if( nChkTeachSame == CTaskTestPP::eAxisMax ){
				m_nStep = 10000;	// going to pick z
			}else{
				m_nStep = 1000;		// move aleady pos
				m_nRetryCount= 0;
			}
		}break;
	case 1000: // all picker up
		{
			pTaskTestPP->MakeLog("[%s] [EXEC] [Step=%8d] [Picker All Up](STR)", m_szCmdName ,m_nStep);
			int nAxisNo[2] = { CTaskTestPP::eAxisZ1 ,CTaskTestPP::eAxisZ2 };

			std::vector<CPoint> vPkr;
			pTaskTestPP->MakeAllPicker(vPkr);

			pTaskTestPP->MakeLog("[%s] [EXEC] [Step=%8d] [Z1,Z2 move %s safety pos](STR)", m_szCmdName, m_nStep, m_strStageName);
			for (int i = 0; i < _countof(nAxisNo); i++)
			{
				int nErr = pTaskTestPP->do_MovePos(LOC_IDX_TEST_PP_HOME, m_nHandType, m_cCmdPocket, vPkr, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i]);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d all picker up command, but this was wrong ", m_nTestPpIdx + 1);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}
			m_tcTimeout = (double)GetTickCount64();
			m_nStep++;
		}break;
	case 1001:  // check picker all up
		{
			OneAxis* pMotorZ1 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ1);
			OneAxis* pMotorZ2 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ2);

			OneAxis* pMotor[2] = { pMotorZ1 ,pMotorZ2 };
			int nComp = 0;
			for (int i = 0; i < _countof(pMotor); i++)
			{
				std::vector<CPoint> vPkr;
				pTaskTestPP->MakeAllPicker(vPkr);
				m_vErrPicker.clear();
				int nErrSen = pTaskTestPP->Chk_Z_Safety_Sen_Check_Type(vPkr,i);
				int nAxisDone = pMotor[i]->AxisDone(vPkr, m_vErrPicker);
				if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR) {

					nComp++;
				}
				else 
				{
					std::vector<CPoint> vErrPkr; vErrPkr.clear();
					CString sAlarmData = _T("");
					int nErr = pTaskTestPP->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, pTaskTestPP->GetSafetyFailPicker(i, ePickerSafetySensorCheckType::eSafetySensorCheck), i, vErrPkr, sAlarmData);

					double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
					if (dSpanTime > TIME_OUT_SHORT*1000.0)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("%s", sAlarmData);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
						ReportAlarm(nErr, AlarmData, vErrPkr);
						break;
					}
				}
			}

			if (nComp == _countof(pMotor))
			{
				m_nStep = 2000;
				pTaskTestPP->MakeLog("[%s] [EXEC] [Step=%8d] [Z%d Axis Move to Safety Pos](END)", m_szCmdName, m_nStep, m_nHandType + 1);
			}
		}break;
	case 2000: //  X1,X2 home safety
		{
			pTaskTestPP->MakeLog("[%s] [EXEC] [Step=%8d] [X%d Axis Move to Safety Pos Check]", m_szCmdName, m_nStep, m_nHandType + 1);
			std::vector<CPoint> vTempPkr;
			vTempPkr.push_back(CPoint(0, 0));
			double dCurPosX1 = 0;
			m_pMotor[CTaskTestPP::eAxisX1]->GetCurrentPos(dCurPosX1);
			double dCurPosX2 = 0;
			m_pMotor[CTaskTestPP::eAxisX2]->GetCurrentPos(dCurPosX2);

			double dTPosHome[CTaskTestPP::eAxisMax] = { 0, };
			pTaskTestPP->GetTeachPos(LOC_IDX_TEST_PP_HOME, CPoint(0, 0), vTempPkr, CTaskTestPP::eZPOS_SAFETY, dTPosHome, m_nHandType);

			int nSafetySenX1 = pTaskTestPP->ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON);
			int nSafetySenX2 = pTaskTestPP->ChkDI(CTaskTestPP::xX2AXIS_POSITION_SEN, DEF_ON);
			// home 위치보다 크거나 안전 센서가 감지 안되면	 X1,X2축 Home
			if (/*dTPosHome[CTaskTestPP::eAxisX1] + 1000.0 < dCurPosX1 ||*/ nSafetySenX1 != ERR_NO_ERROR /*|| 0 > dCurPosX1 ||*/
				/*dTPosHome[CTaskTestPP::eAxisX2] + 1000.0 < dCurPosX2 ||*/ || nSafetySenX2 != ERR_NO_ERROR /*|| 0 > dCurPosX2*/ || g_IO.GetDriverType() == _DIO_TYPE_SIMUL_ 
				|| m_bAutoRunXHomePos == TRUE)
			{
				m_tcTimeout = (double)GetTickCount64();
				m_nStep = 3000;
			}
			// 그 외에 경우에는 그냥 움직이면 된다.
			else {
				m_tcTimeout = (double)GetTickCount64();
				m_nStep = 4000;
			}
		}break;
	case 3000: //X1, X2 move Home
		{
			if (m_nCmdStage >= LOC_IDX_STATION_1 && m_nCmdStage <= LOC_IDX_STATION_4)
			{
				int nPressIdx = pTaskTestPP->GetPressNum(m_nCmdStage);

				double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
				if (dSpanTime > MOTOR_TIME_OUT_SHORT*1000.0) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d Y Axis can not move station [WorkStage=%s not safety]", m_nTestPpIdx + 1, m_strStageName);
					ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + nPressIdx, AlarmData);
					break;
				}

 				if (g_TaskPressUnit[nPressIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready)
 					break;
			}

			int nAxisNo[2] = { CTaskTestPP::eAxisX1 ,CTaskTestPP::eAxisX2 };
			std::vector<CPoint> vPicker;
			vPicker.push_back(CPoint(0, 0));
			pTaskTestPP->MakeLog("[%s] [EXEC] [Step=%8d] [X1,X2 Axis Move to Home Pos](STR)", m_szCmdName, m_nStep);
			for (int i = 0; i < _countof(nAxisNo); i++)
			{
				int nErr = pTaskTestPP->do_MovePos(LOC_IDX_TEST_PP_HOME, m_nHandType, m_cCmdPocket, m_vCmdPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i]);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d y axis move command,but this was wrong", m_nTestPpIdx + 1);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}
			m_tcTimeout = (double)GetTickCount64();
			m_nStep++;
		}break;
	case 3001: // check done X1, X2 safety
		{
			OneAxis* pMotorX1 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisX1);
			OneAxis* pMotorX2 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisX2);

			OneAxis* pMotor[2] = { pMotorX1 ,pMotorX2 };
			int nComp = 0;
			for (int i = 0; i < _countof(pMotor); i++)
			{
				int nAxisDone = pMotor[i]->AxisDone();
				if (nAxisDone == ERR_NO_ERROR) {
					nComp++;
				}
				else 
				{
					double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
					if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d Y Home Position move time over", m_nTestPpIdx + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
						ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
						break;
					}

					if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d Y axis done fail, move home position", m_nTestPpIdx + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
						ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPpIdx, AlarmData);
					}
				}
			}

			if (nComp == _countof(pMotor))
			{
				m_nStep = 4000;
				pTaskTestPP->MakeLog("[%s] [EXEC] [Step=%8d] [X1,X2 Move Home](END)", m_szCmdName, m_nStep);
			}
		}break;
		case 4000:	// move X1,Y1 Pitch or X2,Y2 Pitch, Y target position 
		{
			int nAxisNo[3] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_X1_SW_Y1 : CTaskTestPP::eAxisPitch_HW_X2_SW_Y2,
				m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_Y1_SW_X1 : CTaskTestPP::eAxisPitch_HW_Y2_SW_X2,
				CTaskTestPP::eAxisY };

			pTaskTestPP->MakeLog("[%s] [EXEC] [Step=%8d] [X%d Pitch, Y%d Pitch , Y move %s safety pos](STR)", m_szCmdName, m_nStep, m_nHandType + 1, m_nHandType + 1, m_strStageName);
			for (int i = 0; i < _countof(nAxisNo); i++) {
				int nErr = pTaskTestPP->do_MovePos(m_nCmdStage, m_nHandType, m_cCmdPocket, m_vCmdPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i]);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d X%d Pitch, Y%d Pitch, Y target position command,but this was wrong [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_nHandType + 1, m_strStageName);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}

			m_tcTimeout = (double)GetTickCount64();
			m_nStep++;
		}break;
		case 4001:	// check done move X1,Y1 or X2,Y2 Pitch target position
		{
			int nChkComp = 0;
			int nAxisNo[3] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_X1_SW_Y1 : CTaskTestPP::eAxisPitch_HW_X2_SW_Y2,
				m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_Y1_SW_X1 : CTaskTestPP::eAxisPitch_HW_Y2_SW_X2,
				CTaskTestPP::eAxisY };

			for (int i = 0; i < _countof(nAxisNo); i++) {
				OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo[i]);
				int nAxisDone = pMotor->AxisDone();
				if (nAxisDone == ERR_NO_ERROR) {
					nChkComp++;
				}
				else 
				{
					double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
					if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d, X%dPitch, Y%dPitch Axis move time over [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_nHandType + 1, m_strStageName);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
						break;
					}

					if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d axis done fail, X%dPitch, Y%dPitch move target position [Axis=%s] [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_nHandType + 1, pMotor->GetAxisName(), m_strStageName);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPpIdx, AlarmData);
						break;
					}
				}
			}
			if (nChkComp == _countof(nAxisNo)) {
				m_nStep = 10000;
			}
		}break;
		case 10000:	// move X1 or X2 target position 
		{
			if (m_bXmoveTarget == TRUE)
			{
				int nAxisNo[1] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2 };
				pTaskTestPP->MakeLog("[%s] [EXEC] [Step=%8d] [X%d move %s safety pos](STR)", m_szCmdName, m_nStep, m_nHandType + 1, m_strStageName);
				for (int i = 0; i < _countof(nAxisNo); i++) {
					int nErr = pTaskTestPP->do_MovePos(m_nCmdStage, m_nHandType, m_cCmdPocket, m_vCmdPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i]);
					if (nErr != ERR_NO_ERROR) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d X%d target position command,but this was wrong [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_strStageName);
						OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(nErr, AlarmData);
						break;
					}
				}

				m_tcTimeout = (double)GetTickCount64();
				m_nStep++;
			}
			else
				m_nStep = 20000;

		}break;
		case 10001:	// check done move X1 or X2 target position
		{
			int nChkComp = 0;
			int nAxisNo[1] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2 };
			for (int i = 0; i<_countof(nAxisNo); i++) {
				OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo[i]);
				int nAxisDone = pMotor->AxisDone();
				if (nAxisDone == ERR_NO_ERROR) {
					nChkComp++;
				}
				else 
				{
					double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
					if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d X%d Axis move time over [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_strStageName);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
						break;
					}

					if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d axis done fail, X%d move target position [Axis=%s] [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, pMotor->GetAxisName(), m_strStageName);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPpIdx, AlarmData);
						break;
					}
				}
			}
			if (nChkComp == _countof(nAxisNo)) {
				m_nStep = 20000;
			}
		}break;
    case 20000:
        {
			ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
        }break;
    }
    return CBaseSeqCmd::State_Executing();
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_MoveAlreadyPos::State_Pause()
{
    CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
    if( m_nStep_New != -1 ){
        m_nStep = m_nStep_New;
        m_nStep_New = -1;
    }
    if( m_nStep_Pre != m_nStep ){
        pTaskTestPP->MakeLog("[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
    }
    m_nStep_Pre = m_nStep;

    switch( m_nStep )
    {
    case 0:
        {
            int nAxisCount = pTaskTestPP->m_vMotor.size();
            for(int i=0; i<nAxisCount; i++){
                OneAxis* pMotor = pTaskTestPP->GetAxis(i);
                //pMotor->SetSlowStop();
				bool bCloseLimit = pMotor->CheckLimitClose();
				double dCurPos = 0;
				pMotor->GetCurrentPos(dCurPos);
				if (bCloseLimit == false)
				{
					pTaskTestPP->MakeLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
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

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_MoveAlreadyPos::State_Recovery()
{
    CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
    if( m_nStep_New != -1 ){
        m_nStep = m_nStep_New;
        m_nStep_New = -1;
    }
    if( m_nStep_Pre != m_nStep ){
        pTaskTestPP->MakeLog("[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
    }
    m_nStep_Pre = m_nStep;

    switch( m_nStep )
    {
    case 0: // 현재 위치와 마지막 위치를 비교 한다.
        {
            SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing);
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
BOOL CTestPPCmd_MoveAlreadyPos::State_NormalComplete()
{
    CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
    //pTaskTestPP->SendEvent(CEID_TEST_PP_MOVE_ALREADY_END);
    return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_MoveAlreadyPos::State_Alarm()
{
    CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
    if( m_nStep_New != -1 ){
        m_nStep = m_nStep_New;
        m_nStep_New = -1;
    }
    if( m_nStep_Pre != m_nStep ){
        pTaskTestPP->MakeLog("[CMD: %s] [ALARM] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
    }
    m_nStep_Pre = m_nStep;

    switch( m_nStep )
    {
    case 0: // alarm 발생 했을 때 처리해 줘야 할꺼 처리 해줌
        {
            int nAxisCount = pTaskTestPP->m_vMotor.size();
            for(int i=0; i<nAxisCount; i++){
                OneAxis* pMotor = pTaskTestPP->GetAxis(i);
                //pMotor->SetSlowStop();
				bool bCloseLimit = pMotor->CheckLimitClose();
				double dCurPos = 0;
				pMotor->GetCurrentPos(dCurPos);
				if (bCloseLimit == false)
				{
					pTaskTestPP->MakeLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos);
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

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_MoveAlreadyPos::State_AlarmComplete()
{
    CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
    if( m_nStep_New != -1 ){
        m_nStep = m_nStep_New;
        m_nStep_New = -1;
    }
    if( m_nStep_Pre != m_nStep ){
       // pTaskTestPP->MakeLog("[CMD: %s] [ALARM COMP] [Step=%8d]", m_szCmdName, m_nStep);
    }
    m_nStep_Pre = m_nStep;

	int i=0;
	for( i=0; i<CTaskTestPP::eAxisMax; i++){
		OneAxis* pMotor = pTaskTestPP->GetAxis(i);
		if( pMotor->ChkMotorUsable() != ERR_NO_ERROR ){
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("%s is not usable, please check", pMotor->GetAxisName());
			ReportAlarm(ERR_TEST_PP_1_MOTOR_ALARMED + m_nTestPpIdx, AlarmData);
			return FALSE;
		}
	}

    switch(m_nStep)
    {
	case 0:       // all picker up
		{
			double dErr[CTaskTestPP::eAxisMax] = { 0, };
			double dTPos[CTaskTestPP::eAxisMax] = { 0, };
			pTaskTestPP->GetTeachPos(m_nCmdStage, m_cCmdPocket, m_vCmdPicker, CTaskTestPP::eZPOS_SAFETY, dTPos, m_nHandType);

			dTPos[1] = 0.0;	//원점 위치 기준
			double dCurPos[CTaskTestPP::eAxisMax];

			for (int i = 0; i < CTaskTestPP::eAxisMax; i++)
			{
				m_pMotor[CTaskTestPP::eAxisX1]->GetCurrentPos(dCurPos[i]);				
			}
							// Teaching 위치와 현재 위치가 일치 하는지 확인.
			dErr[CTaskTestPP::eAxisX1] = fabs(dCurPos[CTaskTestPP::eAxisX1] - dTPos[CTaskTestPP::eAxisX1]);
			dErr[CTaskTestPP::eAxisZ1] = fabs(dCurPos[CTaskTestPP::eAxisZ1] - dTPos[CTaskTestPP::eAxisZ1]);
			dErr[CTaskTestPP::eAxisPitch_HW_X1_SW_Y1] = fabs(dCurPos[CTaskTestPP::eAxisPitch_HW_X1_SW_Y1] - dTPos[CTaskTestPP::eAxisPitch_HW_X1_SW_Y1]);
			dErr[CTaskTestPP::eAxisPitch_HW_Y1_SW_X1] = fabs(dCurPos[CTaskTestPP::eAxisPitch_HW_Y1_SW_X1] - dTPos[CTaskTestPP::eAxisPitch_HW_Y1_SW_X1]);

			dErr[CTaskTestPP::eAxisY] = fabs(dCurPos[CTaskTestPP::eAxisY] - dTPos[CTaskTestPP::eAxisY]);

			dErr[CTaskTestPP::eAxisX2] = fabs(dCurPos[CTaskTestPP::eAxisX2] - dTPos[CTaskTestPP::eAxisX2]);
			dErr[CTaskTestPP::eAxisZ2] = fabs(dCurPos[CTaskTestPP::eAxisZ2] - dTPos[CTaskTestPP::eAxisZ2]);
			dErr[CTaskTestPP::eAxisPitch_HW_X2_SW_Y2] = fabs(dCurPos[CTaskTestPP::eAxisPitch_HW_X2_SW_Y2] - dTPos[CTaskTestPP::eAxisPitch_HW_X2_SW_Y2]);
			dErr[CTaskTestPP::eAxisPitch_HW_Y2_SW_X2] = fabs(dCurPos[CTaskTestPP::eAxisPitch_HW_Y2_SW_X2] - dTPos[CTaskTestPP::eAxisPitch_HW_Y2_SW_X2]);

			int nChkTeachSame = 0;
			for (int i = 0; i < CTaskTestPP::eAxisMax; i++)
			{
				if (dErr[i] <= m_dInPosRange[i])
					nChkTeachSame++;
			}

			// 축 이동이 완료 되어진 상태 이므로 motor 동작이 필요 없음.
			if (nChkTeachSame == CTaskTestPP::eAxisMax) {
				m_nStep = 10000;
			}
			else {
				m_nStep = 3000;
				m_nRetryCount = 0;
			}
		}break;
	case 1000: // all picker up
		{
			pTaskTestPP->MakeLog("[%s] [ALARM COMP] [Step=%8d] [Picker All Up](STR)", m_szCmdName, m_nStep);

			std::vector<CPoint> vPkr;
			pTaskTestPP->MakeAllPicker(vPkr);

			int nAxisNo[2] = { CTaskTestPP::eAxisZ1 ,CTaskTestPP::eAxisZ2 };
			pTaskTestPP->MakeLog("[%s] [ALARM COMP] [Step=%8d] [Z1,Z2 move %s safety pos](STR)", m_szCmdName, m_nStep, m_strStageName);
			for (int i = 0; i < _countof(nAxisNo); i++)
			{
				int nErr = pTaskTestPP->do_MovePos(LOC_IDX_TEST_PP_HOME, m_nHandType, m_cCmdPocket, vPkr, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i]);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d all picker up command, but this was wrong ", m_nTestPpIdx + 1);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_Z1_PICKER_CMD_INTERLOCK + pTaskTestPP->GetHandTypeIndex(m_nHandType), AlarmData);
					break;
				}
			}
			m_tcTimeout = (double)GetTickCount64();
			m_nStep++;
		}break;
	case 1001:  // check picker all up
		{
			OneAxis* pMotorZ1 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ1);
			OneAxis* pMotorZ2 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisZ2);

			OneAxis* pMotor[2] = { pMotorZ1 ,pMotorZ2 };
			int nComp = 0;
			for (int i = 0; i < _countof(pMotor); i++)
			{
				std::vector<CPoint> vPkr;
				pTaskTestPP->MakeAllPicker(vPkr);
				int nErrSen = pTaskTestPP->Chk_Z_Safety_Sen_Check_Type(vPkr,i);
				m_vErrPicker.clear();
				int nAxisDone = pMotor[i]->AxisDone(vPkr, m_vErrPicker);
				if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR) {

					nComp++;
				}
				else 
				{
					std::vector<CPoint> vErrPkr; vErrPkr.clear();
					CString sAlarmData = _T("");
					int nErr = pTaskTestPP->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, pTaskTestPP->GetSafetyFailPicker(i, ePickerSafetySensorCheckType::eSafetySensorCheck), i, vErrPkr, sAlarmData);

					double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
					if (dSpanTime > TIME_OUT_SHORT*1000.0)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("%s", sAlarmData);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
						ReportAlarm(nErr, AlarmData, vErrPkr);
						break;
					}
				}
			}

			if (nComp == _countof(pMotor))
			{
				m_nStep = 3000;
				pTaskTestPP->MakeLog("[%s] [ALARM COMP] [Step=%8d] [Z%d Axis Move to Safety Pos](END)", m_szCmdName, m_nStep, m_nHandType + 1);
			}
		}break;
	case 3000:
		{
			pTaskTestPP->MakeLog("[%s] [ALARM COMP] [Step=%8d] [X%d Axis Move to Safety Pos Check]", m_szCmdName, m_nStep, m_nHandType + 1);
			std::vector<CPoint> vTempPkr;
			vTempPkr.push_back(CPoint(0, 0));
			double dCurPosX1 = 0;
			m_pMotor[CTaskTestPP::eAxisX1]->GetCurrentPos(dCurPosX1);
			double dCurPosX2 = 0;
			m_pMotor[CTaskTestPP::eAxisX2]->GetCurrentPos(dCurPosX2);

			double dTPosHome[CTaskTestPP::eAxisMax] = { 0, };
			pTaskTestPP->GetTeachPos(LOC_IDX_TEST_PP_HOME, CPoint(0, 0), vTempPkr, CTaskTestPP::eZPOS_SAFETY, dTPosHome, m_nHandType);

			int nSafetySenX1 = pTaskTestPP->ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON);
			int nSafetySenX2 = pTaskTestPP->ChkDI(CTaskTestPP::xX2AXIS_POSITION_SEN, DEF_ON);
			// home 위치보다 크거나 안전 센서가 감지 안되면	 X1,X2축 Home
			if (dTPosHome[CTaskTestPP::eAxisX1] + 1000.0 < dCurPosX1 || nSafetySenX1 != ERR_NO_ERROR || 0 > dCurPosX1 ||
				dTPosHome[CTaskTestPP::eAxisX2] + 1000.0 < dCurPosX2 || nSafetySenX2 != ERR_NO_ERROR || 0 > dCurPosX2)
			{
				m_tcTimeout = (double)GetTickCount64();
				m_nStep = 3001;
			}
			// 그 외에 경우에는 그냥 움직이면 된다.
			else {
				m_tcTimeout = (double)GetTickCount64();
				m_nStep = 4000;
			}
		}break;
	case 3001: // check done X1, X2 safety
		{
			OneAxis* pMotorX1 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisX1);
			OneAxis* pMotorX2 = pTaskTestPP->GetAxis(CTaskTestPP::eAxisX2);

			OneAxis* pMotor[2] = { pMotorX1 ,pMotorX2 };
			int nComp = 0;
			for (int i = 0; i < _countof(pMotor); i++)
			{
				int nAxisDone = pMotor[i]->AxisDone();
				if (nAxisDone == ERR_NO_ERROR) {
					nComp++;
				}
				else 
				{
					double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
					if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d Y Home Position move time over", m_nTestPpIdx + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
						ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
						break;
					}

					if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Test PP %d Y axis done fail, move home position", m_nTestPpIdx + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[i]->GetAxisName());
						ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPpIdx, AlarmData);
					}
				}
			}

			if (nComp == _countof(pMotor))
			{
				m_nStep = 4000;
				pTaskTestPP->MakeLog("[%s] [ALARM COMP] [Step=%8d] [X1,X2 Move Home](END)", m_szCmdName, m_nStep);
			}
		}break;
	case 4000:	// move X1,Y1 Pitch or X2,Y2 Pitch, Y target position 
	{
		int nAxisNo[3] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_X1_SW_Y1 : CTaskTestPP::eAxisPitch_HW_X2_SW_Y2,
			m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_Y1_SW_X1 : CTaskTestPP::eAxisPitch_HW_Y2_SW_X2,
			CTaskTestPP::eAxisY };

		pTaskTestPP->MakeLog("[%s] [ALARM COMP] [Step=%8d] [X%d Pitch, Y%d Pitch , Y move %s safety pos](STR)", m_szCmdName, m_nStep, m_nHandType + 1, m_nHandType + 1, m_strStageName);
		for (int i = 0; i < _countof(nAxisNo); i++) {
			int nErr = pTaskTestPP->do_MovePos(m_nCmdStage, m_nHandType, m_cCmdPocket, m_vCmdPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i]);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Test PP %d X%d Pitch, Y%d Pitch, Y target position command,but this was wrong [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_strStageName);
				OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
		}

		m_tcTimeout = (double)GetTickCount64();
		m_nStep++;
	}break;
	case 4001:	// check done move X1,Y1 or X2,Y2 Pitch target position
	{
		int nChkComp = 0;
		int nAxisNo[2] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_X1_SW_Y1 : CTaskTestPP::eAxisPitch_HW_X2_SW_Y2,
			m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisPitch_HW_Y1_SW_X1 : CTaskTestPP::eAxisPitch_HW_Y2_SW_X2 };

		for (int i = 0; i < _countof(nAxisNo); i++) {
			OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo[i]);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				nChkComp++;
			}
			else 
			{
				double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
				if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d, X%dPitch, Y%dPitch Axis move time over [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_nHandType + 1, m_strStageName);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d axis done fail, X%dPitch, Y%dPitch move target position [Axis=%s] [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_nHandType + 1, pMotor->GetAxisName(), m_strStageName);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPpIdx, AlarmData);
					break;
				}
			}
		}
		if (nChkComp == _countof(nAxisNo)) {
			m_nStep = 10000;
		}
	}break;
	case 10000:	// move X1 or X2 target position 
	{
		if (m_bXmoveTarget == TRUE)
		{
			int nAxisNo[1] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2 };
			pTaskTestPP->MakeLog("[%s] [ALARM COMP] [Step=%8d] [X%d move %s safety pos](STR)", m_szCmdName, m_nStep, m_nHandType + 1, m_strStageName);
			for (int i = 0; i < _countof(nAxisNo); i++) {
				int nErr = pTaskTestPP->do_MovePos(m_nCmdStage, m_nHandType, m_cCmdPocket, m_vCmdPicker, CTaskTestPP::eZPOS_SAFETY, nAxisNo[i]);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d X%d target position command,but this was wrong [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_strStageName);
					OneAxis* pMotor = pTaskTestPP->GetAxis((CTaskTestPP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}

			m_tcTimeout = (double)GetTickCount64();
			m_nStep++;
		}
		else
			m_nStep = 20000;

	}break;
	case 10001:	// check done move X1 or X2 target position
	{
		int nChkComp = 0;
		int nAxisNo[1] = { m_nHandType == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2 };
		for (int i = 0; i < _countof(nAxisNo); i++) {
			OneAxis* pMotor = pTaskTestPP->GetAxis(nAxisNo[i]);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				nChkComp++;
			}
			else 
			{
				double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
				if (dSpanTime > MOTOR_TIME_OUT_MIDDLE*1000.0) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d X%d Axis move time over [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, m_strStageName);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + m_nTestPpIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Test PP %d axis done fail, X%d move target position [Axis=%s] [CmdStage=%s]", m_nTestPpIdx + 1, m_nHandType + 1, pMotor->GetAxisName(), m_strStageName);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TEST_PP_1_MOTOR_MOVE_FAIL + m_nTestPpIdx, AlarmData);
					break;
				}
			}
		}
		if (nChkComp == _countof(nAxisNo)) {
			m_nStep = 20000;
		}
	}break;
	case 20000:
		{
			return CBaseSeqCmd::State_AlarmComplete(); 
		}break;
	default: {
		m_nStep = 0;
	}break;
    }

    return CBaseSeqCmd::State_AlarmComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CTestPPCmd_MoveAlreadyPos::State_Aborting()
{
    CTaskTestPP* pTaskTestPP = (CTaskTestPP*)m_pReceive;
    if( m_nStep_New != -1 ){
        m_nStep = m_nStep_New;
        m_nStep_New = -1;
    }
    if( m_nStep_Pre != m_nStep ){
        pTaskTestPP->MakeLog("[CMD: %s] [ABORT] [Step=%8d]", m_szCmdName, m_nStep);
    }
    m_nStep_Pre = m_nStep;

    switch( m_nStep )
    {
    case 0:
        {
            int nAxisCount = pTaskTestPP->m_vMotor.size();
            for(int i=0; i<nAxisCount; i++){
                OneAxis* pMotor = pTaskTestPP->GetAxis(i);
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
BOOL CTestPPCmd_MoveAlreadyPos::State_AbortComplete()
{
    return CBaseSeqCmd::State_AbortComplete(); 
}
