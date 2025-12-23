#include "StdAfx.h"
#include "MultiOrgCmd_Axis.h"
#include "MainFrm.h"


CMultiOrgCmd_Axis::CMultiOrgCmd_Axis( void )
{
	m_nCmdId = eSeqCmd_MultiOrg;
	m_nCmdStage = 0;
	m_nRetryCount = 0;
	sprintf_s( m_szCmdName, sizeof( m_szCmdName ), "eSeqCmd_MultiOrg" );

	m_vAutoCmdMultiOrgPress.clear();
	m_vAutoCmdMultiOrgTestPp.clear();
	m_vAutoCmdMultiOrgLoadTable.clear();
	m_vAutoCmdMultiOrgEtc.clear();
	m_vAutoCmdMultiOrgTrayPp.clear();
	m_vAutoCmdMultiOrgTrayFeeder.clear();
	m_vDetectedStopSenTable.clear();
	m_nOrgRetryCount = 0;
	m_nErrUnit = 0;
	m_nBackVibratorStep = 0;
	m_vErrPicker.clear();
}


CMultiOrgCmd_Axis::~CMultiOrgCmd_Axis( void )
{
}


//==============================================================================
//
//==============================================================================
int CMultiOrgCmd_Axis::CheckParam()
{
	if (m_nCmdStage == eMultiOrgType_PressDown || m_nCmdStage == eMultiOrgType_AxisAll || m_nCmdStage == eMultiOrgType_EqpUpAll || m_nCmdStage == eMultiOrgType_ModuleAll) {
		for (int i = 0; i < eMaxPressUnitCount; i++) {
			m_vAutoCmdMultiOrgPress.push_back(&g_TaskPressUnit[ePressUnit_Down_1 + i]);
		}
	}
	if (m_nCmdStage == eMultiOrgType_TestPp || m_nCmdStage == eMultiOrgType_AxisAll || m_nCmdStage == eMultiOrgType_EqpUpAll || m_nCmdStage == eMultiOrgType_ModuleAll) {
		for (int i = 0; i < eMaxTestPPCount; i++) {
			m_vAutoCmdMultiOrgTestPp.push_back(&g_TaskTestPP[eTestPP_1 + i]);
		}
	}
	if (m_nCmdStage == eMultiOrgType_LoadTable || m_nCmdStage == eMultiOrgType_AxisAll || m_nCmdStage == eMultiOrgType_EqpUpAll || m_nCmdStage == eMultiOrgType_ModuleAll) {
		for (int i = 0; i < eMaxLoadTblCount; i++) {
			m_vAutoCmdMultiOrgLoadTable.push_back(&g_TaskLoadTbl[eLoadTbl_1 + i]);
		}
	}
	if (m_nCmdStage == eMultiOrgType_TrayPP || m_nCmdStage == eMultiOrgType_AxisAll || m_nCmdStage == eMultiOrgType_EqpUpAll || m_nCmdStage == eMultiOrgType_ModuleAll) {
		CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
		for (int i = 0; i < eMaxTrayPPCount; i++) {
			m_vAutoCmdMultiOrgTrayPp.push_back(pTaskTrayPP[eTrayPP_1_LD + i]);
		}

		
	}

	if (m_nCmdStage == eMultiOrgType_AxisAll)
	{
		m_vAutoCmdMultiOrgTrayFeeder.push_back(&g_TaskTrayFeeder);
	}
	

	if ((m_nCmdStage >= eMultiOrgType_Module1 && m_nCmdStage <= eMultiOrgType_Module2)) {
		m_vAutoCmdMultiOrgPress.push_back(&g_TaskPressUnit[ePressUnit_Down_1 + ((m_nCmdStage - eMultiOrgType_Module1)*eMaxATS_Area)]);
		m_vAutoCmdMultiOrgPress.push_back(&g_TaskPressUnit[ePressUnit_Down_2 + ((m_nCmdStage - eMultiOrgType_Module1)*eMaxATS_Area)]);
		m_vAutoCmdMultiOrgPress.push_back(&g_TaskPressUnit[ePressUnit_Down_3 + ((m_nCmdStage - eMultiOrgType_Module1)*eMaxATS_Area)]);
		m_vAutoCmdMultiOrgPress.push_back(&g_TaskPressUnit[ePressUnit_Down_4 + ((m_nCmdStage - eMultiOrgType_Module1)*eMaxATS_Area)]);
// 		m_vAutoCmdMultiOrgPress.push_back(&g_TaskPressUnit[ePressUnit_Up_1 + ((m_nCmdStage - eMultiOrgType_Module1)*eMaxATS_Area / 2)]);
// 		m_vAutoCmdMultiOrgPress.push_back(&g_TaskPressUnit[ePressUnit_Up_2 + ((m_nCmdStage - eMultiOrgType_Module1)*eMaxATS_Area / 2)]);
// 		m_vAutoCmdMultiOrgPress.push_back(&g_TaskPressUnit[ePressUnit_Up_3 + ((m_nCmdStage - eMultiOrgType_Module1)*eMaxATS_Area / 2)]);
// 		m_vAutoCmdMultiOrgPress.push_back(&g_TaskPressUnit[ePressUnit_Up_4 + ((m_nCmdStage - eMultiOrgType_Module1)*eMaxATS_Area / 2)]);
		m_vAutoCmdMultiOrgTestPp.push_back(&g_TaskTestPP[eTestPP_1 + (m_nCmdStage - eMultiOrgType_Module1)]);
	}

	return ERR_NO_ERROR;
}


//==============================================================================
//
//==============================================================================
int  CMultiOrgCmd_Axis::OnStop()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Aborting );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CMultiOrgCmd_Axis::OnRetry()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CMultiOrgCmd_Axis::OnSkip()
{
	ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_AlarmComplete );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CMultiOrgCmd_Axis::OnPause()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Pause );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CMultiOrgCmd_Axis::OnResume()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CMultiOrgCmd_Axis::State_Setup()
{
	m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_LONG);
	m_tcTimeout.StartTimer();

	return CBaseSeqCmd::State_Setup();
}

//==============================================================================
//
//==============================================================================
BOOL CMultiOrgCmd_Axis::State_Executing()
{
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		//g_TaskSystemCtrl.MakeLog( "[CMD: %s] [EXECUTE] [Step=%8d]", m_szCmdName, m_nStep );
	}
	m_nStep_Pre = m_nStep;

	if(CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Manual && g_DMCont.m_dmHandlerOpt.GN(NDM11_MULTIORIGIN_HISTORY) == eMulti_Origin_Start)
	{ 
		if (g_pDlgHomeCheckStatus != NULL)
		{
			if (g_pDlgHomeCheckStatus->IsWindowVisible() != TRUE)
			{
				g_pDlgHomeCheckStatus->ShowWindow(SW_SHOW);
			}
		}
	}

 	switch( m_nStep )
	{
		case 0: // push task
			{
				m_nStep = 1000;
			}break;
		case 1000:
			{
				m_nStep = 3000;
			}break;
			// ===== tray pp org
		case 3000:			// check interlock 
			{
				m_nErrUnit = 0;
				if( ( m_nCmdStage == eMultiOrgType_EqpUpAll || m_nCmdStage == eMultiOrgType_AxisAll || m_nCmdStage == eMultiOrgType_ModuleAll) == FALSE ) {
					m_nStep = 5000;
					break;
				}
				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Tray PP Origin [STR]]", m_szCmdName, m_nStep);
				m_nStep = 3200;
			
			}break;
			// z axis org
		case 3200: //2000
			{
				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Tray PP Z Axis Origin [STR]]", m_szCmdName, m_nStep);
				for (int i = 0; i < (int)m_vAutoCmdMultiOrgTrayPp.size(); i++) {
					OneAxis* pMotorZ = m_vAutoCmdMultiOrgTrayPp[i]->GetAxis(CTaskTrayBasePP::eAxisZ);
					pMotorZ->ClearOrgFlag();
					pMotorZ->SetStop();
					Sleep(100);
					pMotorZ->AlarmReset();
					Sleep(100);
					pMotorZ->ServoOn();
					Sleep(100);
					pMotorZ->EnableSWLimit(DEF_DISABLE);
					m_tcTimeout.ResetStartTime();
					m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_SHORT);
					m_vAutoCmdMultiOrgTrayPp[i]->m_bInitialized = FALSE;
				}
				m_nStep++;
				m_nErrUnit = 0;
				
			}break;
		case 3201: // 2001
			{
				int nMaxRetry = MAX_MOTOR_RETRY_COUNT;				
				int nComp = 0;

				for (int i = 0; i < (int)m_vAutoCmdMultiOrgTrayPp.size(); i++) {
					OneAxis* pMotorZ = m_vAutoCmdMultiOrgTrayPp[i]->GetAxis(CTaskTrayBasePP::eAxisZ);

					int nErrOrg = 0;
					nErrOrg = pMotorZ->Origin();
					if (nErrOrg != ERR_NO_ERROR) {
						m_nOrgRetryCount++;
						if (m_nOrgRetryCount >= 2) {
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
							ReportAlarm(nErrOrg, AlarmData);
							break;
						}
						m_nStep--;
					}
					else {
						BOOL bCompleted = pMotorZ->IsOrgCompleted(&nErrOrg);
						if (bCompleted == TRUE) {
							pMotorZ->EnableSWLimit(DEF_ENABLE);
							nComp++;
						}
						else
						{
							m_nErrUnit = m_vAutoCmdMultiOrgTrayPp[i]->GetRobotIdIdx();
						
							if (m_tcTimeout.CheckOverTime() == TRUE) {
								if (m_nRetryCount > nMaxRetry) {
									CParamList AlarmData;
									AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
									CBaseSeqCmd::ReportAlarm(ERR_TRAY_PP_1_MOTOR_MOVE_TIMEOUT + m_nErrUnit, AlarmData);
								}
							}														
						}
					}				
					m_nRetryCount = 0;					
				}
				if (nComp == (int)m_vAutoCmdMultiOrgTrayPp.size())
				{
					m_nStep = 3300;
					g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Tray PP Z Axis Origin [END]]", m_szCmdName, m_nStep);
				}			
			}break;		
		case 3300:// move to safey pos
		{
			g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Tray PP Z Axis Move to Safety Pos [STR]]", m_szCmdName, m_nStep);

			std::vector<CPoint> vPkr;
			g_TaskTrayLoadPP.MakeAllPicker(vPkr);
			vPkr.push_back(CPoint(0, 0));
			for (int i = 0; i < (int)m_vAutoCmdMultiOrgTrayPp.size(); i++) {

				vPkr.clear();
				if (i == 0)
				{
					g_TaskTrayLoadPP.MakeAllPicker(vPkr);
				}
				else
				{
					g_TaskTrayUnloadPP.MakeAllPicker(vPkr);
				}

				OneAxis* pMotorZ = m_vAutoCmdMultiOrgTrayPp[i]->GetAxis(CTaskTrayBasePP::eAxisZ);
				int nErr = m_vAutoCmdMultiOrgTrayPp[i]->do_MovePos(LOC_IDX_TRAY_PP_SAFETY, CPoint(0, 0), vPkr, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}

			m_nErrUnit = 0;
			m_tcTimeout.ResetStartTime();
			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_SHORT);
			m_nStep++;
		}break;
		case 3301:
		{
			int nComp=0;

			for (int i = 0; i < (int)m_vAutoCmdMultiOrgTrayPp.size(); i++) {
				OneAxis* pMotorZ = m_vAutoCmdMultiOrgTrayPp[i]->GetAxis(CTaskTrayBasePP::eAxisZ);
			
				std::vector<CPoint> vPkr;
				m_vAutoCmdMultiOrgTrayPp[i]->MakeAllPicker(vPkr);
				m_vErrPicker.clear();
				int nErrSen = m_vAutoCmdMultiOrgTrayPp[i]->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);

				int nAxisDone = pMotorZ->AxisDone(vPkr,m_vErrPicker);
				if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR) {
					nComp++;
				}
				else 
				{
					std::vector<CPoint> vErrPkr; vErrPkr.clear();
					CString sAlarmData = _T("");
					int nErr = m_vAutoCmdMultiOrgTrayPp[i]->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, m_vAutoCmdMultiOrgTrayPp[i]->GetSafetyFailPicker(ePickerSafetySensorCheckType::eSafetySensorCheck), vErrPkr, sAlarmData);

					if (m_tcTimeout.CheckOverTime() == TRUE) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("%s", sAlarmData);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
						ReportAlarm(nErr, AlarmData, vErrPkr);
						break;
					}
					m_nErrUnit = m_vAutoCmdMultiOrgTrayPp[i]->GetRobotIdIdx();
				}			
			}
		
			if (nComp == (int)m_vAutoCmdMultiOrgTrayPp.size())
			{
				m_nStep = 3500;
				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Tray PP Z Axis Move to Safety Pos [END]]", m_szCmdName, m_nStep);
			}

		}break;
			// Tray pp y axis org
		case 3500: // 2500
			{
				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Tray PP Y, X-Pitch, Y-Pitch Axis Origin [STR]]", m_szCmdName, m_nStep);
				for (int i = 0; i < (int)m_vAutoCmdMultiOrgTrayPp.size(); i++) {
					OneAxis* pMotorY  = m_vAutoCmdMultiOrgTrayPp[i]->GetAxis(CTaskTrayBasePP::eAxisY);
					OneAxis* pMotorPX = m_vAutoCmdMultiOrgTrayPp[i]->GetAxis(CTaskTrayBasePP::eAxisPitchX);
					OneAxis* pMotorPY = m_vAutoCmdMultiOrgTrayPp[i]->GetAxis(CTaskTrayBasePP::eAxisPitchY);
					OneAxis* pMotor[3] = { pMotorY, pMotorPX , pMotorPY };
					for (int i = 0; i < _countof(pMotor); i++) {
						if( pMotor[i] == nullptr ) continue;
						pMotor[i]->ClearOrgFlag();
						pMotor[i]->SetStop();
						Sleep(100);
						pMotor[i]->AlarmReset();
						Sleep(100);
						pMotor[i]->ServoOn();
						Sleep(100);
						pMotor[i]->EnableSWLimit(DEF_DISABLE);
					}
				}
				m_nErrUnit = 0;
				m_tcTimeout.ResetStartTime();
				m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_SHORT);
				m_nStep++;
			}break;
		case 3501://2501
			{
				int nMaxRetry = MAX_MOTOR_RETRY_COUNT;
				int nComp = 0;
			
				int nAxisCount = 0;
				for (int i = 0; i < (int)m_vAutoCmdMultiOrgTrayPp.size(); i++) {
					OneAxis* pMotorY  = m_vAutoCmdMultiOrgTrayPp[i]->GetAxis(CTaskTrayBasePP::eAxisY );
					OneAxis* pMotorPX = m_vAutoCmdMultiOrgTrayPp[i]->GetAxis(CTaskTrayBasePP::eAxisPitchX);
					OneAxis* pMotorPY = m_vAutoCmdMultiOrgTrayPp[i]->GetAxis(CTaskTrayBasePP::eAxisPitchY);
					OneAxis* pMotor[3] = { pMotorY, pMotorPX, pMotorPY };

					int nErrOrg = 0;
				
					for( int j = 0; j < _countof( pMotor ); j++ ) {
						if( pMotor[j] == nullptr ) continue;
						nAxisCount++;
						nErrOrg = pMotor[j]->Origin();
						if( nErrOrg != ERR_NO_ERROR) {
							m_nOrgRetryCount++;
							if( m_nOrgRetryCount >= 2 ) {
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[j]->GetAxisName());
								ReportAlarm( nErrOrg, AlarmData );
								break;
							}
							m_nStep--;
						}
						else
						{
							m_nErrUnit = m_vAutoCmdMultiOrgTrayPp[i]->GetRobotIdIdx();

							BOOL bCompleted = pMotor[j]->IsOrgCompleted(&nErrOrg);
							if (bCompleted)
							{
								pMotor[j]->EnableSWLimit(DEF_ENABLE);
								int nErr = pMotor[j]->ChkMotorUsable();
								if (nErr == ERR_NO_ERROR) {
									nComp++;
								}
							}
							else
							{
								if (nErrOrg == ERR_MOTOR_CMN_IN_MOVE_STATE)
								{
									if (m_tcTimeout.CheckOverTime() == TRUE) {
										CParamList AlarmData;
										AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[j]->GetAxisName());
										CBaseSeqCmd::ReportAlarm(ERR_TRAY_PP_1_MOTOR_ORG_FAIL + m_nErrUnit, AlarmData);
										break;
									}
								}
								else
								{
									CParamList AlarmData;
									AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[j]->GetAxisName());
									ReportAlarm(ERR_TRAY_PP_1_MOTOR_ORG_FAIL + i, AlarmData);
									break;
								}
							}
						}												
					}// for end
				}

				if( nComp == nAxisCount) {
					m_nStep = 3600;
					m_nRetryCount = 0;
					g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Tray PP Y, X-Pitch, Y-Pitch Axis Origin [END]]", m_szCmdName, m_nStep);
					//ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
				}
			}break;
			// x,pitch x, pitch y axis org
		case 3600: //3000
			{
				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Tray PP X Axis Origin [STR]]", m_szCmdName, m_nStep);
				for (int i = 0; i < (int)m_vAutoCmdMultiOrgTrayPp.size(); i++) {
					OneAxis* pMotorX = m_vAutoCmdMultiOrgTrayPp[i]->GetAxis(CTaskTrayBasePP::eAxisX);
					OneAxis* pMotor[1] = { pMotorX };
					for (int i = 0; i < _countof(pMotor); i++) {
						pMotor[i]->ClearOrgFlag();
						pMotor[i]->SetStop();
						Sleep(100);
						pMotor[i]->AlarmReset();
						Sleep(100);
						pMotor[i]->ServoOn();
						Sleep(100);
						pMotor[i]->EnableSWLimit(DEF_DISABLE);
					}
				}
				m_nErrUnit = 0;
				m_tcTimeout.ResetStartTime();
				m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_LONG);
				m_nStep++;
			}break;
		case 3601: // 3001
			{
				int nMaxRetry = MAX_MOTOR_RETRY_COUNT;
			
				int nComp = 0;

				for (int i = 0; i < (int)m_vAutoCmdMultiOrgTrayPp.size(); i++) {
					OneAxis* pMotorX = m_vAutoCmdMultiOrgTrayPp[i]->GetAxis(CTaskTrayBasePP::eAxisX);
					OneAxis* pMotor[1] = { pMotorX };

					int nErrOrg = 0;
					
					for (int j = 0; j < _countof(pMotor); j++) {
						nErrOrg = pMotor[j]->Origin();
						if (nErrOrg != NO_ERROR) {
							m_nOrgRetryCount++;
							if (m_nOrgRetryCount >= 2) {
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[j]->GetAxisName());
								ReportAlarm(nErrOrg, AlarmData);
								break;
							}
							m_nStep--;
						}
						else 
						{
							m_nErrUnit = m_vAutoCmdMultiOrgTrayPp[i]->GetRobotIdIdx();
							BOOL bCompleted = pMotor[j]->IsOrgCompleted(&nErrOrg);

							if (bCompleted)
							{
								pMotor[j]->EnableSWLimit(DEF_ENABLE);
								int nErr = pMotor[j]->ChkMotorUsable();
								if (nErr == ERR_NO_ERROR) {
									nComp++;
								}
							}							
							else {
								if (nErrOrg == ERR_MOTOR_CMN_IN_MOVE_STATE)
								{
									if (m_tcTimeout.CheckOverTime() == TRUE) {
										CParamList AlarmData;
										AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[j]->GetAxisName());
										CBaseSeqCmd::ReportAlarm(ERR_TRAY_PP_1_MOTOR_ORG_FAIL + m_nErrUnit, AlarmData);
										break;
									}
								}
								else
								{
									CParamList AlarmData;
									AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[j]->GetAxisName());
									ReportAlarm(ERR_TRAY_PP_1_MOTOR_ORG_FAIL + i, AlarmData);
									break;
								}						
							}
						}						
					}// for end
				}

				if( nComp == (int)m_vAutoCmdMultiOrgTrayPp.size() ) {
				
					for (int i = 0; i < (int)m_vAutoCmdMultiOrgTrayPp.size(); i++) 
						m_vAutoCmdMultiOrgTrayPp[i]->m_bInitialized = TRUE;

					m_nStep = 4000;
					g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Tray PP X Axis Origin [END]]", m_szCmdName, m_nStep);
					g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Tray PP Origin [END]]", m_szCmdName, m_nStep);
					//ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
				}
			}break;
			// Tray Feeder Y
		case 4000:
		{
			if ((m_nCmdStage == eMultiOrgType_EqpUpAll || m_nCmdStage == eMultiOrgType_AxisAll) == FALSE) {
				m_nStep = 5000;
				break;
			}

			BOOL bFeederExistSen = g_TaskTrayFeeder.ChkTrayExistSensor(eExist);
			if(bFeederExistSen == ERR_NO_ERROR)
			{
				//cyl clamp
				g_TaskTrayFeeder.do_cyl_ClampUnclamp( eClamp );

				int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_CLAMP_WAITTIME);
				if (0 < nSleep) {
					Sleep(nSleep);
				}

				auto clamp_state = g_TaskTrayFeeder.chk_cyl_ClampUnclamp( eClamp );
				if( clamp_state != ERR_NO_ERROR ) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( "Clamper is abnormal state." );
					AlarmData.m_fnAddItemFormat( clamp_state.msg.c_str() );
					ReportAlarm(ERR_FEEDER_CLAMP_CYLINDER_FAIL, AlarmData );
					break;
				}

				//cyl do_cyl Up
				g_TaskTrayFeeder.do_cyl_UpDown(eUp);

				nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_UPDN_WAITTIME);
				if (0 < nSleep) {
					Sleep(nSleep);
				}

				auto cylinder_up_state = g_TaskTrayFeeder.chk_cyl_UpDown(eUp);
				if (cylinder_up_state != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(cylinder_up_state.msg.c_str());
					ReportAlarm(ERR_FEEDER_UP_CYLINDER_FAIL, AlarmData);
					break;
				}

				// cyl align for
				g_TaskTrayFeeder.do_cyl_align_ForBack(eForw);
				nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_ALIGN_WAITTIME);
				if (0 < nSleep) {
					Sleep(nSleep);
				}
				auto align_for_state = g_TaskTrayFeeder.chk_cyl_align_ForBack(eForw);
				if (align_for_state != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(align_for_state.msg.c_str());
					ReportAlarm(ERR_FEEDER_ALIGN_CYLINDER_FOR_FAIL, AlarmData);
					break;
				}
			}
			else {
				// cyl align back
				g_TaskTrayFeeder.do_cyl_align_ForBack( eBack );
				int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_ALIGN_WAITTIME);
				if (0 < nSleep) {
					Sleep(nSleep);
				}

				auto align_back_state = g_TaskTrayFeeder.chk_cyl_align_ForBack( eBack );
				if( align_back_state != ERR_NO_ERROR ) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( align_back_state.msg.c_str() );
					ReportAlarm(ERR_FEEDER_ALIGN_CYLINDER_BACK_FAIL, AlarmData );
					break;
				}

				//cyl Unclamp
				g_TaskTrayFeeder.do_cyl_ClampUnclamp( eUnclamp );

				nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_CLAMP_WAITTIME);
				if (0 < nSleep) {
					Sleep(nSleep);
				}

				auto unclamp_state = g_TaskTrayFeeder.chk_cyl_ClampUnclamp( eUnclamp );
				if( unclamp_state != ERR_NO_ERROR ) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( unclamp_state.msg.c_str() );
					ReportAlarm(ERR_FEEDER_UNCLAMP_CYLINDER_FAIL, AlarmData );
					break;
				}

				//cyl do_cyl Up
				g_TaskTrayFeeder.do_cyl_UpDown(eUp);

				nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_UPDN_WAITTIME);

				// MultiOrg경우, Feeder UpDown Cylinder Check 최소 시간 변경.  [5/14/2025 dohyeong.kim]
				// 동작 시간이 1초 이상일 경우, Feeder Up/Down 속도 조절 필요함.
				if (nSleep < 1000) {
					nSleep = 1000;
				}

				if (0 < nSleep) {
					Sleep(nSleep);
				}

				auto cylinder_up_state = g_TaskTrayFeeder.chk_cyl_UpDown(eUp);
				if (cylinder_up_state != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(cylinder_up_state.msg.c_str());
					ReportAlarm(ERR_FEEDER_UP_CYLINDER_FAIL, AlarmData);
					break;
				}
			}


			m_nStep++;
			g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Tray Feeder Y [STR]]", m_szCmdName, m_nStep);
		}
		case 4001:
		{
			

			for (int i = 0; i < (int)m_vAutoCmdMultiOrgTrayFeeder.size(); i++)
			{
				OneAxis* pMotorY = m_vAutoCmdMultiOrgTrayFeeder[i]->GetAxis(CTaskTrayFeeder::eAxisY);
				OneAxis* pMotor[] = { pMotorY };
				for (int i = 0; i < _countof(pMotor); i++)
				{
					if (pMotor[i] == nullptr)
						continue;

					pMotor[i]->ClearOrgFlag();
					pMotor[i]->SetStop();
					Sleep(100);
					pMotor[i]->AlarmReset();
					Sleep(100);
					pMotor[i]->ServoOn();
					Sleep(100);
					pMotor[i]->EnableSWLimit(DEF_DISABLE);
				}
			}
			m_tcTimeout.ResetStartTime();
			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
			m_nStep++;
		}break;
		case 4002:
		{
			int nMaxRetry = MAX_MOTOR_RETRY_COUNT;

			int nComp = 0;
			int nChk = 0;

			for (int i = 0; i < (int)m_vAutoCmdMultiOrgTrayFeeder.size(); i++)
			{
				OneAxis* pMotorFeederY = m_vAutoCmdMultiOrgTrayFeeder[i]->GetAxis(CTaskTrayFeeder::eAxisY);
				OneAxis* pMotor[1] = { pMotorFeederY };

				int nErrOrg = 0;

				for (int j = 0; j < _countof(pMotor); j++) {
					if (pMotor[j] == nullptr) {
						nComp++;
						continue;
					}
						

					nErrOrg = pMotor[j]->Origin();
					if (nErrOrg != NO_ERROR) {
						m_nOrgRetryCount++;
						if (m_nOrgRetryCount >= 2) {
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[j]->GetAxisName());
							ReportAlarm(nErrOrg, AlarmData);
							break;
						}
						m_nStep--;
					}
					else
					{
						BOOL bCompleted = pMotor[j]->IsOrgCompleted(&nErrOrg);
						if (bCompleted)
						{
							pMotor[j]->EnableSWLimit(DEF_ENABLE);
							int nErr = pMotor[j]->ChkMotorUsable();
							if (nErr == ERR_NO_ERROR) {
								nComp++;
							}
						}
						else {
							if (nErrOrg == ERR_MOTOR_CMN_IN_MOVE_STATE)
							{
								if (m_tcTimeout.CheckOverTime() == TRUE)
								{
									CParamList AlarmData;
									AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[j]->GetAxisName());
									CBaseSeqCmd::ReportAlarm(ERR_FEEDER_MOTOR_ORG_TIMEOUT, AlarmData);
									break;
								}
							}
							else {
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[j]->GetAxisName());
								ReportAlarm(ERR_FEEDER_MOTOR_ORG_FAIL, AlarmData);
								break;
							}
						}
					}
				}// for end
				if (_countof(pMotor) == nComp)
				{
					nChk++;
				}
			}

			if (nChk == (int)m_vAutoCmdMultiOrgTrayFeeder.size()) {
				m_nStep = 4500;
				m_nRetryCount = 0;
				m_nErrUnit = 0; 
				m_vAutoCmdMultiOrgTrayFeeder.clear();
				if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
				{
					g_TaskTrayFeeder.ChkSafetySensor(eExist);
				}
				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Tray Feeder Y [END]]", m_szCmdName, m_nStep);
				//ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
			}
		}break;
			// ========= transfer org
		case 4500: // check interlock
			{
				if( (m_nCmdStage == eMultiOrgType_ModuleAll || m_nCmdStage == eMultiOrgType_EqpUpAll || m_nCmdStage == eMultiOrgType_AxisAll ) == FALSE ) {
					m_nStep = 5000;
					break;
				}

				m_nRetryCount = 0;
				g_TaskTransfer.m_bInitialized = FALSE;
				m_tcTimeout.SetCheckTime( TIME_OUT_SHORT );
				m_tcTimeout.StartTimer();
				m_nStep = 4501;
				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Transfer Origin [STR]]", m_szCmdName, m_nStep);
				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Transfer Z Axis Origin [STR]]", m_szCmdName, m_nStep);
			}break;
		case 4501:
			{
				if( m_tcTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_TRNASFER_UP_TIMEOUT;
					CParamList AlarmData;
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				g_TaskTransfer.do_cyl_UpDown( DEF_UP );
				int nErr = g_TaskTransfer.chk_cyl_UpDown( DEF_UP );
				if( nErr != ERR_NO_ERROR ) {
					break;
				}

				m_nStep++;
			}break;
		case 4502:
			{
				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Transfer Z Axis Origin [END]]", m_szCmdName, m_nStep);
				//int nErr = g_TaskTransfer.GetTrayExist( DEF_NOT_EXIST );
				//if( nErr != ERR_NO_ERROR ) {
				//	int nAlid = ERR_TRNASFER_TRAY_EXIST;
				//	CParamList AlarmData;
				//	ReportAlarm( nAlid, AlarmData );
				//	break;
				//}
				m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_LONG);
				m_tcTimeout.StartTimer();
				m_nStep++;
			}break;
		case 4503:
			{
				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Transfer X Axis Origin [STR]]", m_szCmdName, m_nStep);
				
				OneAxis* pMotorX = ( OneAxis* )g_TaskTransfer.GetAxis( CTaskTransfer::AXIS_TRANSFER_X );
				pMotorX->SetStop();
				Sleep( 100 );
				pMotorX->AlarmReset();
				Sleep( 100 );
				pMotorX->ServoOn();
				Sleep( 100 );
				pMotorX->EnableSWLimit( DEF_DISABLE );
				pMotorX->ClearOrgFlag();

				m_nStep++;
			}break;
		case 4504:
			{
				OneAxis* pMotorX = ( OneAxis* )g_TaskTransfer.GetAxis( CTaskTransfer::AXIS_TRANSFER_X );
				int nErrOrg = pMotorX->Origin();
				if( nErrOrg != NO_ERROR ) {
					m_nRetryCount++;
					if( m_nRetryCount >= 2 ) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotorX->GetAxisName());
						AlarmData.m_fnAddItemFormat( _T( "%s Axis" ), pMotorX->GetAxisName() );
						ReportAlarm(nErrOrg, AlarmData );
						break;
					}
					m_nStep--;
					break;
				}
				else
				{
					BOOL bCompleted = pMotorX->IsOrgCompleted(&nErrOrg);
					if (bCompleted)
					{
						pMotorX->EnableSWLimit(DEF_ENABLE);
						int nErr = pMotorX->ChkMotorUsable();
						if (nErr == ERR_NO_ERROR) {
							g_TaskTransfer.m_bInitialized = TRUE;
							m_nStep++;
							g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Transfer X Axis Origin [END]]", m_szCmdName, m_nStep);
							g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Transfer Origin [END]]", m_szCmdName, m_nStep);
							break;
						}
					}
					else {
						if (nErrOrg == ERR_MOTOR_CMN_IN_MOVE_STATE)
						{
							if (m_tcTimeout.CheckOverTime() == TRUE) {
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat("Axis=%s", pMotorX->GetAxisName());
								ReportAlarm(ERR_TRANSFER_X_AXIS_MOVE_TIME_OUT, AlarmData);
								break;
							}
						}
						else
						{
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat(_T("%s Axis"), pMotorX->GetAxisName());
							ReportAlarm(ERR_TRANSFER_MOTOR_ORG_FAIL, AlarmData);
							break;
						}

					}				
				}
			}break;
		case 4505:
		{
			double dTPosSafety = 0.0;
			dTPosSafety = g_TaskTransfer.GetTeachPos(eLocIdxTransfer::LOC_IDX_TRANSFER_SAFETY, CTaskTransfer::eAxis::AXIS_TRANSFER_X);
			OneAxis* pMotor = (OneAxis*)g_TaskTransfer.GetAxis(CTaskTransfer::eAxis::AXIS_TRANSFER_X);
			if (dTPosSafety <= 0.0) {
				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Transfer Move to Safety Pos[STR]]", m_szCmdName, m_nStep);

				pMotor->Move(dTPosSafety, TEACH_MOVE_SPD_LOW, 200, 200);

				m_tcTimeout.SetCheckTime((MOTOR_TIME_OUT_SHORT));
				m_tcTimeout.StartTimer();
				m_nStep++;
				break;
			}
			else {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Transfer move Safety pos cmd Error");
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(ERR_TRANSFER_X_AXIS_MOVE_FAIL, AlarmData);
				break;
			}
		}break;
		case 4506:
		{
			OneAxis* pMotor = g_TaskTransfer.GetAxis(CTaskTransfer::eAxis::AXIS_TRANSFER_X);
			double dCurPos = 0;
			pMotor->GetCurrentPos(dCurPos);

			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Transfer Move to Safety Pos[END]]", m_szCmdName, m_nStep);
				m_nStep = 5000;
				m_nOrgRetryCount = 0;
			}
			else
			{
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Transfer move Safety pos Error");
					AlarmData.m_fnAddItemFormat("SetTime=%.2f, z cur pos=%.2f", m_tcTimeout.GetCheckTime(), dCurPos);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TRANSFER_X_AXIS_MOVE_FAIL, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Transfer move Safety pos axis done Error [nAxisDone = %d]", nAxisDone);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_TRANSFER_X_AXIS_MOVE_FAIL, AlarmData);
				}
			}
		}break;

			// ========== org press
		case 5000:
			{
				if( m_vAutoCmdMultiOrgPress.size() == 0 ) {
					m_nStep = 6000;
					break;
				}

				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Press Unit Origin [STR]]", m_szCmdName, m_nStep);


				for( int i = 0; i < ( int )m_vAutoCmdMultiOrgPress.size(); i++ ) {
					OneAxis* pMotor = ( OneAxis* )( m_vAutoCmdMultiOrgPress[ i ]->GetAxis( CTaskPressUnit::eAxisZ ) );
					pMotor->ClearOrgFlag();
					pMotor->SetStop();
					Sleep( 100 );
					pMotor->AlarmReset();
					Sleep( 100 );
					pMotor->ServoOn();
					Sleep( 100 );
					pMotor->EnableSWLimit( DEF_DISABLE );
				}

				for (int i = 0; i < (int)m_vAutoCmdMultiOrgPress.size(); i++) {
					CTaskPressUnit* pPressUnit = m_vAutoCmdMultiOrgPress[i];
					pPressUnit->m_bInitialized = FALSE;
				}

				m_nErrUnit = 0;

				m_tcTimeout.SetCheckTime( MOTOR_TIME_OUT_MIDDLE );
				m_tcTimeout.StartTimer();
				m_nStep++;
				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Press Unit Z Axis Origin [STR]]", m_szCmdName, m_nStep);
			}break;
		case 5001:
			{
				int nErrOrg = 0;
				int nComp = 0;
				for( int i = 0; i < ( int )m_vAutoCmdMultiOrgPress.size(); i++ ) {
					OneAxis* pMotor = ( OneAxis* )( m_vAutoCmdMultiOrgPress[ i ]->GetAxis( CTaskPressUnit::eAxisZ ) );
					nErrOrg = pMotor->Origin();
					if( nErrOrg != NO_ERROR ) {
						m_nOrgRetryCount++;
						if( m_nOrgRetryCount >= 2 ) {
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat( "Multi-Origin Press unit fail" );
							AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
							ReportAlarm( nErrOrg, AlarmData );
							break;
						}
						m_nStep = 5000;
						break;
					}
					else 
					{ // org done
						BOOL bCompleted = pMotor->IsOrgCompleted(&nErrOrg);
						if (bCompleted)
						{
							pMotor->EnableSWLimit(DEF_ENABLE);
							int nErr = pMotor->ChkMotorUsable();
							if (nErr == ERR_NO_ERROR) {
								nComp++;
							}
						}
						else 
						{
							m_nErrUnit = m_vAutoCmdMultiOrgPress[i]->m_nUnitIdx;
							if (nErrOrg == ERR_MOTOR_CMN_IN_MOVE_STATE)
							{
								if (m_tcTimeout.CheckOverTime() == TRUE)
								{
									CParamList AlarmData;
									AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
									ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + m_nErrUnit, AlarmData);
									break;
								}
							}
							else
							{
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat("%s Axis", pMotor->GetAxisName());
								ReportAlarm(nErrOrg, AlarmData);
								break;
							}
						
						}
					}
				}

				if( nComp == ( int )m_vAutoCmdMultiOrgPress.size() ) {
					if( g_DMCont.m_dmEQP.GB( BDM0_MD_VIBRATION_MODE ) == DEF_ON ) {
						m_nStep++;
					}
					else {
						m_nStep = 5100;
					}
					g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Press Unit Z Axis Origin [END]]", m_szCmdName, m_nStep);
					m_nOrgRetryCount = 0;
				}
			}break;
		case 5002:
			{
				m_nStep = 5100;
			}break;

		case 5100:
			{
				// safety 위치보다 작을 수는 없다.

				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Press Unit Move to Safety Pos [STR]]", m_szCmdName, m_nStep);
				int nPressCnt = 0;
				double dTPosSafety = 0.0;
				for (int i = 0; i < (int)m_vAutoCmdMultiOrgPress.size(); i++) 
				{				
					m_vAutoCmdMultiOrgPress[i]->GetTeachPos(LOC_IDX_PRESS_UNIT_SAFETY, &dTPosSafety);
					OneAxis* pMotor = (OneAxis*)m_vAutoCmdMultiOrgPress[i]->GetAxis(CTaskPressUnit::eAxisZ);
					if (dTPosSafety <= 0.0) {									
						pMotor->Move(dTPosSafety, TEACH_MOVE_SPD_LOW, 200, 200);
						nPressCnt++;
					}
					else {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Press Unit %d move Safety pos cmd Error", m_vAutoCmdMultiOrgPress[i]->m_nUnitIdx + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + m_vAutoCmdMultiOrgPress[i]->m_nUnitIdx, AlarmData);
						break;
					}
				}

				if (nPressCnt == (int)m_vAutoCmdMultiOrgPress.size())
				{
					m_nErrUnit = 0;
					m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
					m_tcTimeout.StartTimer();
					m_nStep++;
				}
			}break;
		case 5101:
			{
				int nPressCnt = 0;
				for (int i = 0; i < (int)m_vAutoCmdMultiOrgPress.size(); i++)
				{
					OneAxis* pMotor = (OneAxis*)m_vAutoCmdMultiOrgPress[i]->GetAxis(CTaskPressUnit::eAxisZ);
					int nAxisDone = pMotor->AxisDone();
					if (nAxisDone == ERR_NO_ERROR) {
						//m_vAutoCmdMultiOrgPress[i]->MakeLog("[%s] [EXEC=%8d] [Move to Safety Pos](END)", m_szCmdName, m_nStep);
						nPressCnt++;
					}
					else
					{
						m_nErrUnit = m_vAutoCmdMultiOrgPress[i]->m_nUnitIdx;
						if (m_tcTimeout.CheckOverTime() == TRUE)
						{
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("Press Unit move Safety pos Error");
							AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
							ReportAlarm(ERR_PRESS_UNIT_1_MOVE_TIMEOUT + m_nErrUnit, AlarmData);
							break;
						}

						if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
						{
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("Press Unit %d move Safety pos axis done Error", m_vAutoCmdMultiOrgPress[i]->m_nUnitIdx + 1);
							AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
							ReportAlarm(ERR_PRESS_UNIT_1_MOVE_FAIL + m_vAutoCmdMultiOrgPress[i]->m_nUnitIdx, AlarmData);
							break;
						}
					}
				}
			
				if (nPressCnt == (int)m_vAutoCmdMultiOrgPress.size())
				{
					m_nStep = 5300;
					g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Press Unit Move to Safety Pos [END]]", m_szCmdName, m_nStep);
				}
			}break;
		case 5300:
			{
				for( int i = 0; i < ( int )m_vAutoCmdMultiOrgPress.size(); i++ ) {
					CTaskPressUnit* pPressUnit = m_vAutoCmdMultiOrgPress[ i ];
					pPressUnit->m_bInitialized = TRUE;
				}
				m_vAutoCmdMultiOrgPress.clear();
				m_nStep = 6000;
				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Press Unit Origin [END]]", m_szCmdName, m_nStep);
			}
			// ===== org test pp ( 6000 ~ 7000 )
		case 6000: // test pp picker up
		{
			if ((int)m_vAutoCmdMultiOrgTestPp.size() == 0) {
				m_nStep = 9000;
				break;
			}
			m_nStep = 6500;
		}break;
		case 6500: // Test PP Z1,Z2 Safety
			{
				for( int i = 0; i < ( int )m_vAutoCmdMultiOrgTestPp.size(); i++ ) {
					OneAxis* pMotorZ1 = m_vAutoCmdMultiOrgTestPp[i]->GetAxis(CTaskTestPP::eAxisZ1);
					OneAxis* pMotorZ2 = m_vAutoCmdMultiOrgTestPp[i]->GetAxis(CTaskTestPP::eAxisZ2);
					OneAxis* pMotor[2] = { pMotorZ1, pMotorZ2 };
					for (int j = 0; j < 2; j++) {
						pMotor[j]->ClearOrgFlag();
						pMotor[j]->SetStop();
						Sleep(100);
						pMotor[j]->AlarmReset();
						Sleep(100);
						pMotor[j]->ServoOn();
						Sleep(100);
						pMotor[j]->EnableSWLimit(DEF_DISABLE);
					}
				}

				for (int i = 0; i < (int)m_vAutoCmdMultiOrgTestPp.size(); i++) {
					m_vAutoCmdMultiOrgTestPp[i]->m_bInitialized = FALSE;
				}

				m_tcTimeout.SetCheckTime(TIME_OUT_SHORT);
				m_tcTimeout.StartTimer();
				m_nStep++;			
			}break;
			case 6501:
			{
				int nChk = 0;
				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Test PP Z Axis Move to Safety Pos [STR]]", m_szCmdName, m_nStep);
				for (int i = 0; i < (int)m_vAutoCmdMultiOrgTestPp.size(); i++)
				{
					OneAxis* pMotorZ1 = m_vAutoCmdMultiOrgTestPp[i]->GetAxis(CTaskTestPP::eAxisZ1);
					OneAxis* pMotorZ2 = m_vAutoCmdMultiOrgTestPp[i]->GetAxis(CTaskTestPP::eAxisZ2);

					OneAxis* pMotor[2] = { pMotorZ1, pMotorZ2 };
					int nErrOrg = 0;
					int nComp = 0;
					for (int j = 0; j < 2; j++)
					{
						nErrOrg = pMotor[j]->Origin();
						if (nErrOrg != NO_ERROR) {
							m_nOrgRetryCount++;
							if (m_nOrgRetryCount >= MAX_MOTOR_RETRY_COUNT) {
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat("Test Pp Multi origin Z axis error");
								AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[j]->GetAxisName());
								ReportAlarm(ERR_TEST_PP_1_MOTOR_ORG_FAIL + m_vAutoCmdMultiOrgTestPp[i]->m_nTestPPIdx, AlarmData);
								return FALSE;
							}						
							m_nStep--;
							break;
						}
						else 
						{
							m_nErrUnit = m_vAutoCmdMultiOrgTestPp[i]->m_nTestPPIdx;
							BOOL bCompleted = pMotor[j]->IsOrgCompleted(&nErrOrg);
							if (bCompleted)
							{
								pMotor[j]->EnableSWLimit(DEF_ENABLE);
								int nErr = pMotor[j]->ChkMotorUsable();
								if (nErr == ERR_NO_ERROR && pMotor[j]->IsHomeSensorOn() == DEF_ON) {
									nComp++;
								}
							}
							else {						
								if (m_tcTimeout.CheckOverTime())
								{
									CParamList AlarmData;
									AlarmData.m_fnAddItemFormat("%s Axis", pMotor[j]->GetAxisName());
									ReportAlarm(ERR_TEST_PP_1_MOTOR_ORG_TIMEOUT + m_vAutoCmdMultiOrgTestPp[i]->m_nTestPPIdx, AlarmData);
									break;
								}						
							}
						}
					}// for end

					if (nComp == 2) {
						nChk++;
					}
				}

				if (nChk == (int)m_vAutoCmdMultiOrgTestPp.size()) {
					m_nStep++;
				}			
			}break;
			case 6502:
			{				
				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [test PP Z Axis Move to Safety Pos [STR]]", m_szCmdName, m_nStep);

				std::vector<CPoint> vPkr;
				g_TaskTrayLoadPP.MakeAllPicker(vPkr);
				vPkr.push_back(CPoint(0, 0));
				for (int i = 0; i < (int)m_vAutoCmdMultiOrgTestPp.size(); i++)
				{
					int nTestPPIdx = m_vAutoCmdMultiOrgTestPp[i]->m_nTestPPIdx;
					OneAxis* pMotorZ1 = m_vAutoCmdMultiOrgTestPp[i]->GetAxis(CTaskTestPP::eAxisZ1);
					OneAxis* pMotorZ2 = m_vAutoCmdMultiOrgTestPp[i]->GetAxis(CTaskTestPP::eAxisZ2);

					OneAxis* pMotor[eTestPPType::eMaxTestPPTypeCount] = { pMotorZ1, pMotorZ2 };
					vPkr.clear();
					g_TaskTestPP[nTestPPIdx].MakeAllPicker(vPkr);
					int nAxisNum[2] = { CTaskTestPP::eAxisZ1 ,CTaskTestPP::eAxisZ2 };
					for (int nLoadUnload = 0; nLoadUnload < eTestPPType::eMaxTestPPTypeCount; nLoadUnload++)
					{
						int nErr = g_TaskTestPP[nTestPPIdx].do_MovePos(eLocIdxTestPP::LOC_IDX_TEST_PP_HOME, nLoadUnload, CPoint(0, 0), vPkr, CTaskTestPP::eTeachPosZ::eZPOS_SAFETY, nAxisNum[nLoadUnload]);
						if (nErr != ERR_NO_ERROR) {
							CParamList AlarmData;
							ReportAlarm(nErr, AlarmData);
							break;
						}
					}
				}

				m_nErrUnit = 0;
				m_tcTimeout.ResetStartTime();
				m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_SHORT);
				m_nStep++;
				
			}break;
		case 6503:
			{
				int nComp = 0;

				for (int i = 0; i < (int)m_vAutoCmdMultiOrgTestPp.size(); i++) {
					int nTestPPIdx = m_vAutoCmdMultiOrgTestPp[i]->m_nTestPPIdx;
					OneAxis* pMotorZ1 = m_vAutoCmdMultiOrgTestPp[i]->GetAxis(CTaskTestPP::eAxisZ1);
					OneAxis* pMotorZ2 = m_vAutoCmdMultiOrgTestPp[i]->GetAxis(CTaskTestPP::eAxisZ2);

					OneAxis* pMotor[eTestPPType::eMaxTestPPTypeCount] = { pMotorZ1, pMotorZ2 };

					for (int nLoadUnload = 0; nLoadUnload < eTestPPType::eMaxTestPPTypeCount; nLoadUnload++)
					{

						OneAxis* pMotorZ = pMotor[nLoadUnload];
						std::vector<CPoint> vPkr;
						m_vAutoCmdMultiOrgTrayPp[i]->MakeAllPicker(vPkr);
						m_vErrPicker.clear();
						int nErrSen = m_vAutoCmdMultiOrgTestPp[i]->Chk_Z_Safety_Sen_Check_Type(vPkr, nLoadUnload);
						int nAxisDone = pMotorZ->AxisDone(vPkr, m_vErrPicker);
						if (nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR) {
							nComp++;
						}
						else
						{

							std::vector<CPoint> vErrPkr; vErrPkr.clear();
							CString sAlarmData = _T("");
							int nErr = m_vAutoCmdMultiOrgTestPp[i]->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, m_vAutoCmdMultiOrgTestPp[i]->GetSafetyFailPicker(nLoadUnload, ePickerSafetySensorCheckType::eSafetySensorCheck), nLoadUnload, vErrPkr, sAlarmData);

							if (m_tcTimeout.CheckOverTime() == TRUE) {
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat("%s", sAlarmData);
								AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
								ReportAlarm(nErr, AlarmData, vErrPkr);
								break;
							}
						}

					}
				}

				if (nComp == (int)m_vAutoCmdMultiOrgTestPp.size() * eTestPPType::eMaxTestPPTypeCount)
				{
					m_nStep = 7000;
					g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Test PP Z Axis Move to Safety Pos [END]]", m_szCmdName, m_nStep);
				}

			}break;
		case 7000: // test pp x1,x2,x1 pitch,x2 pitch, y1 pitch,y2 pitch, org
			{
				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Test PP Y Axis Origin [STR]]", m_szCmdName, m_nStep);

				for( int i = 0; i < ( int )m_vAutoCmdMultiOrgTestPp.size(); i++ ) 
				{
					OneAxis* pMotorX1 = m_vAutoCmdMultiOrgTestPp[ i ]->GetAxis( CTaskTestPP::eAxisX1);
					OneAxis* pMotorX2 = m_vAutoCmdMultiOrgTestPp[ i ]->GetAxis(CTaskTestPP::eAxisX2);
					OneAxis* pMotorPX1 = m_vAutoCmdMultiOrgTestPp[i]->GetAxis(CTaskTestPP::eAxisPitch_HW_X1_SW_Y1);
					OneAxis* pMotorPX2 = m_vAutoCmdMultiOrgTestPp[i]->GetAxis(CTaskTestPP::eAxisPitch_HW_X2_SW_Y2);
					OneAxis* pMotorPY1 = m_vAutoCmdMultiOrgTestPp[i]->GetAxis(CTaskTestPP::eAxisPitch_HW_Y1_SW_X1);
					OneAxis* pMotorPY2 = m_vAutoCmdMultiOrgTestPp[i]->GetAxis(CTaskTestPP::eAxisPitch_HW_Y2_SW_X2);

					OneAxis* pMotor[6] = { pMotorX1 ,pMotorX2, pMotorPX1,pMotorPX2,pMotorPY1,pMotorPY2, };
					for (int j = 0; j < _countof(pMotor); j++)
					{
						pMotor[j]->ClearOrgFlag();
						pMotor[j]->SetStop();
						Sleep(100);
						pMotor[j]->AlarmReset();
						Sleep(100);
						pMotor[j]->ServoOn();
						Sleep(100);
						pMotor[j]->EnableSWLimit(DEF_DISABLE);
					}
				}

				m_nErrUnit = 0;

				m_tcTimeout.SetCheckTime( MOTOR_TIME_OUT_LONG );
				m_tcTimeout.StartTimer();
				m_nStep++;
			}break;
		case 7001:
			{
				int nChk = 0;

				for (int i = 0; i < (int)m_vAutoCmdMultiOrgTestPp.size(); i++)
				{
					OneAxis* pMotorX1 = m_vAutoCmdMultiOrgTestPp[i]->GetAxis(CTaskTestPP::eAxisX1);
					OneAxis* pMotorX2 = m_vAutoCmdMultiOrgTestPp[i]->GetAxis(CTaskTestPP::eAxisX2);
					OneAxis* pMotorPX1 = m_vAutoCmdMultiOrgTestPp[i]->GetAxis(CTaskTestPP::eAxisPitch_HW_X1_SW_Y1);
					OneAxis* pMotorPX2 = m_vAutoCmdMultiOrgTestPp[i]->GetAxis(CTaskTestPP::eAxisPitch_HW_X2_SW_Y2);
					OneAxis* pMotorPY1 = m_vAutoCmdMultiOrgTestPp[i]->GetAxis(CTaskTestPP::eAxisPitch_HW_Y1_SW_X1);
					OneAxis* pMotorPY2 = m_vAutoCmdMultiOrgTestPp[i]->GetAxis(CTaskTestPP::eAxisPitch_HW_Y2_SW_X2);

					OneAxis* pMotor[6] = { pMotorX1 ,pMotorX2, pMotorPX1,pMotorPX2,pMotorPY1,pMotorPY2, };

					int nComp = 0;
					int nErrOrg = 0;
					for (int j = 0; j < _countof(pMotor); j++)
					{
						nErrOrg = pMotor[j]->Origin();
						if (nErrOrg != NO_ERROR) {
							m_nOrgRetryCount++;
							if (m_nOrgRetryCount >= MAX_MOTOR_RETRY_COUNT) {
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat("Test Pp Multi origin X axis error");
								AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[j]->GetAxisName());
								ReportAlarm(nErrOrg + m_vAutoCmdMultiOrgTestPp[i]->m_nTestPPIdx, AlarmData);
								break;
							}
							m_nStep--;
							break;
						}
						else 
						{
							m_nErrUnit = m_vAutoCmdMultiOrgTestPp[i]->m_nTestPPIdx;
							BOOL bCompleted = pMotor[j]->IsOrgCompleted(&nErrOrg);
							if (bCompleted)
							{
								pMotor[j]->EnableSWLimit(DEF_ENABLE);
								int nErr = pMotor[j]->ChkMotorUsable();
								if (nErr == ERR_NO_ERROR && pMotor[j]->IsHomeSensorOn() == DEF_ON) {
									nComp++;
								}
							}
							else {
								if (nErrOrg == ERR_MOTOR_CMN_IN_MOVE_STATE)
								{
									if (m_tcTimeout.CheckOverTime() == TRUE)
									{
										CParamList AlarmData;
										AlarmData.m_fnAddItemFormat("%s Axis", pMotor[j]->GetAxisName());
										CBaseSeqCmd::ReportAlarm(ERR_TEST_PP_1_MOTOR_ORG_TIMEOUT + m_vAutoCmdMultiOrgTestPp[i]->m_nTestPPIdx, AlarmData);
										break;
									}
								}
								else {
									CParamList AlarmData;
									AlarmData.m_fnAddItemFormat("%s Axis", pMotor[j]->GetAxisName());
									ReportAlarm(ERR_TEST_PP_1_MOTOR_ORG_FAIL + m_vAutoCmdMultiOrgTestPp[i]->m_nTestPPIdx, AlarmData);
									break;
								}							
							}
						}					
					}// for end
					if (nComp == _countof(pMotor)) {
						nChk++;
					}
				}
				if (nChk == (int)m_vAutoCmdMultiOrgTestPp.size()) {
					m_nStep = 7500;
					m_nRetryCount = 0;
					g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Test PP Y Axis Origin [END]]", m_szCmdName, m_nStep);
				}
			}break;
		case 7500: // test pp Y org
			{
				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Test PP X,Z Axis Origin [STR]]", m_szCmdName, m_nStep);
				
				for( int i = 0; i < ( int )m_vAutoCmdMultiOrgTestPp.size(); i++ ) {
					OneAxis* pMotorY = m_vAutoCmdMultiOrgTestPp[ i ]->GetAxis( CTaskTestPP::eAxisY);

					OneAxis* pMotor[ 1 ] = { pMotorY };
					for( int j = 0; j < 1; j++ ) {
						pMotor[ j ]->ClearOrgFlag();
						pMotor[ j ]->SetStop();
						Sleep( 100 );
						pMotor[ j ]->AlarmReset();
						Sleep( 100 );
						pMotor[ j ]->ServoOn();
						Sleep( 100 );
						pMotor[ j ]->EnableSWLimit( DEF_DISABLE );
					}
				}
				m_nErrUnit = 0;

				m_tcTimeout.SetCheckTime( MOTOR_TIME_OUT_LONG );
				m_tcTimeout.StartTimer();
				m_nStep++;
			}break;
		case 7501:
			{				
				int nChk = 0;
				for( int i = 0; i < ( int )m_vAutoCmdMultiOrgTestPp.size(); i++ ) {
					OneAxis* pMotorY = m_vAutoCmdMultiOrgTestPp[ i ]->GetAxis( CTaskTestPP::eAxisY );

					OneAxis* pMotor[ 1 ] = { pMotorY };
					int nErrOrg = 0;
					int nComp = 0;
					for( int j = 0; j < _countof(pMotor); j++ ) {
						nErrOrg = pMotor[ j ]->Origin();
						if( nErrOrg != NO_ERROR ) 
						{
							m_nOrgRetryCount++;
							if (m_nOrgRetryCount >= MAX_MOTOR_RETRY_COUNT) {
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat("Test Pp Multi origin X,Z axis error");
								AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[j]->GetAxisName());
								ReportAlarm(ERR_TEST_PP_1_MOTOR_ORG_FAIL + m_vAutoCmdMultiOrgTestPp[i]->m_nTestPPIdx, AlarmData);
								break;;
							}
							m_nStep--;
							break;
						}
						else 
						{
							m_nErrUnit = m_vAutoCmdMultiOrgTestPp[i]->m_nTestPPIdx;
							BOOL bCompleted = pMotor[j]->IsOrgCompleted(&nErrOrg);
							if (bCompleted)
							{
								pMotor[j]->EnableSWLimit(DEF_ENABLE);
								int nErr = pMotor[j]->ChkMotorUsable();
								int nMotionDone = pMotorY->MotionDone();
								if (nErr == ERR_NO_ERROR && pMotor[j]->IsHomeSensorOn() == DEF_ON && nMotionDone == ERR_NO_ERROR) {
									nComp++;
								}
							}
							else {
								if (nErrOrg == ERR_MOTOR_CMN_IN_MOVE_STATE)
								{
									if (m_tcTimeout.CheckOverTime() == TRUE)
									{
										CParamList AlarmData;
										AlarmData.m_fnAddItemFormat("Multi-Origin Test Pp X,Z Axis action time over!");
										AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[j]->GetAxisName());
										CBaseSeqCmd::ReportAlarm(ERR_TEST_PP_1_MOTOR_ORG_TIMEOUT + m_nErrUnit, AlarmData);
										break;
									}
								}
								else
								{
									CParamList AlarmData;
									AlarmData.m_fnAddItemFormat("Test Pp Multi origin X,Z axis not usable.");
									AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[j]->GetAxisName());
									ReportAlarm(ERR_TEST_PP_1_MOTOR_ORG_FAIL + m_vAutoCmdMultiOrgTestPp[i]->m_nTestPPIdx, AlarmData);
									break;
								}							
							}
						}
					}// for end

					if( nComp == _countof(pMotor)) {
						nChk++;
					}
				}

				if( nChk == ( int )m_vAutoCmdMultiOrgTestPp.size() ) {
					m_nStep = 7600;//8000;
					m_nRetryCount = 0;
					g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Test PP X,Z Axis Origin [END]]", m_szCmdName, m_nStep);
				}
			}break;
		case 7600: //Z1,Z2 Safety Move
			{			
				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Test PP Z1,Z2 Axis Move to Safety Pos[STR]]", m_szCmdName, m_nStep);
				int nAxisIdx[2] = { CTaskTestPP::eAxisZ1 ,CTaskTestPP::eAxisZ2 };
				for (int i = 0; i < (int)m_vAutoCmdMultiOrgTestPp.size(); i++) {
					std::vector<CPoint> vPicker;
					vPicker.push_back(CPoint(0, 0));
					g_TaskTestPP[i].MakeAllPicker(vPicker);
					for (int j = 0; j < _countof(nAxisIdx); j++) {
						int nErr = m_vAutoCmdMultiOrgTestPp[i]->do_MovePos(LOC_IDX_TEST_PP_HOME, eTestPPLoadType, CPoint(0, 0), vPicker, CTaskTestPP::eZPOS_SAFETY, nAxisIdx[j], 10);
						OneAxis* pMotor = m_vAutoCmdMultiOrgTestPp[i]->GetAxis(nAxisIdx[j]);
						if (nErr != ERR_NO_ERROR) {
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("Test PP %d Z1,Z2 axis home move command,but this was wrong", m_vAutoCmdMultiOrgTestPp[i]->m_nTestPPIdx + 1);
							AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
							ReportAlarm(nErr, AlarmData);
							break;
						}
					}
				}

				m_tcTimeout.ResetStartTime();
				m_tcTimeout.SetCheckTime(TIME_OUT_MIDDLE);
				m_nStep++;
			}break;
		case 7601: //Z1,Z2 Safety Done
			{
				int nChk = 0;

				for (int i = 0; i < (int)m_vAutoCmdMultiOrgTestPp.size(); i++) {
					OneAxis* pMotorZ1 = m_vAutoCmdMultiOrgTestPp[i]->GetAxis(CTaskTestPP::eAxisZ1);
					OneAxis* pMotorZ2 = m_vAutoCmdMultiOrgTestPp[i]->GetAxis(CTaskTestPP::eAxisZ2);
					OneAxis* pMotor[2] = { pMotorZ1,pMotorZ2};
					for (int j = 0; j < _countof(pMotor); j++)
					{
						std::vector<CPoint> vPkr;
						m_vAutoCmdMultiOrgTrayPp[i]->MakeAllPicker(vPkr);
						m_vErrPicker.clear();
						int nAxisDone = pMotor[j]->AxisDone(vPkr, m_vErrPicker);
						int nErrSen = m_vAutoCmdMultiOrgTestPp[i]->Chk_Z_Safety_Sen_Check_Type(vPkr, j);
						if (nAxisDone == ERR_NO_ERROR) {
							nChk++;
						}
						else
						{
							std::vector<CPoint> vErrPkr; vErrPkr.clear();
							CString sAlarmData = _T("");
							int nErr = m_vAutoCmdMultiOrgTestPp[i]->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, m_vAutoCmdMultiOrgTestPp[i]->GetSafetyFailPicker(j, ePickerSafetySensorCheckType::eSafetySensorCheck), j, vErrPkr, sAlarmData);
							if (m_tcTimeout.CheckOverTime() == TRUE) {
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat("%s", sAlarmData);
								AlarmData.m_fnAddItemFormat("Axis=%s", pMotor[j]->GetAxisName());
								ReportAlarm(nErr, AlarmData, vErrPkr);
								break;
							}
						}
					}
				}

				if (nChk == (int)m_vAutoCmdMultiOrgTestPp.size() * 2) {
					for (int i = 0; i < (int)m_vAutoCmdMultiOrgTestPp.size(); i++) {
						m_vAutoCmdMultiOrgTestPp[i]->m_bInitialized = TRUE;
					}
					m_nStep = 8000;//9000;
					m_nRetryCount = 0;
					g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Test PP Z Axis Move to Safety Pos[END]]", m_szCmdName, m_nStep);
					g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Test PP Origin [END]]", m_szCmdName, m_nStep);
				}
			}break;		
			// ====== load table org
		case 8000:
		{
			if (m_vAutoCmdMultiOrgLoadTable.size() == 0) {
				m_nStep = 20000;
				break;
			}

			//Table Stacker 충돌 영역 회피.
			g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Motor Y Move Stacker Impact Area [STR]]", m_szCmdName, m_nStep);
//			int nEQPNo = g_DMCont.m_dmEQP.GN(NDM0_Handler_EQP_No);
//			if (nEQPNo > DEF_EQUIPMENT_UNIT1)
//			{
				for (int i = 0; i < m_vAutoCmdMultiOrgLoadTable.size(); i++)
				{	
					if (m_vAutoCmdMultiOrgLoadTable[i]->IsTableStackerImpactArea() == TRUE)
					{
						m_vDetectedStopSenTable.push_back(m_vAutoCmdMultiOrgLoadTable[i]);
					}
				}

				if (m_vDetectedStopSenTable.size() > 0)
				{
					m_nStep++;
				}
				else
				{
					m_nStep = 9000;
				}
// 			}
// 			else //1호기는 Stop Sensor가 없음.
// 			{
// 				m_nStep = 9000;
// 			}
		}break;
		case 8001:
		{
			if (m_vDetectedStopSenTable.size() == 0)
			{
				m_nStep = 9000;
				break;
			}

			for (int i = 0; i < (int)m_vDetectedStopSenTable.size(); i++)
			{
				OneAxis* pMotor = (OneAxis*)(m_vAutoCmdMultiOrgLoadTable[i]->GetAxis(CTaskLoadTable::eAxisY));
				pMotor->ClearOrgFlag();
				pMotor->SetStop();
				Sleep(100);
				pMotor->AlarmReset();
				Sleep(100);
				pMotor->ServoOn();
				Sleep(100);
				pMotor->EnableSWLimit(DEF_DISABLE);
			}

			m_nStep++;
		}break;
		case 8002:
		{
			if (m_vDetectedStopSenTable.size() == 0 || g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
			{
				m_nStep = 9000;
				break;
			}

			//Stop Sensor 확인 & Load Table Y Move.
			for (int i = 0; i < (int)m_vDetectedStopSenTable.size(); i++)
			{
				OneAxis* pMotor = (OneAxis*)(m_vDetectedStopSenTable[i]->GetAxis(CTaskLoadTable::eAxisY));
				pMotor->SetCommandPos(0.0);

				double dMtSpd = pMotor->GetSpeed();
				double dMtSpd5 = dMtSpd * 0.05; // 5%

				int nErr = pMotor->Move(LOADER_TABLE_STACKER_IMPACT_MOVE, dMtSpd5, 100, 100, FALSE, _eMotorIntertlock::eMotorInterlock_All_Ignore);
				if (nErr != ERR_NO_ERROR)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Multi-Origin LoadTable %d fail, Motor Y Move Stacker Impact Area");
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}

			m_nStep++;
		}break;
		case 8003:
		{
			if ((int)m_vDetectedStopSenTable.size() == 0)
			{
				m_nStep = 9000;
				break;
			}

			//Motion Done 확인.
			BOOL bHappenProblem = FALSE;
			int nChkAxisDone = 0;
			for (int i = 0; i < (int)m_vDetectedStopSenTable.size(); i++)
			{
				OneAxis* pMotor = (OneAxis*)(m_vDetectedStopSenTable[i]->GetAxis(CTaskLoadTable::eAxisY));
				int nErr = pMotor->MotionDone();

				BOOL bIsImpactArea = m_vDetectedStopSenTable[i]->IsTableStackerImpactArea();
				if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_){
					bIsImpactArea = FALSE;
				}

				if (nErr == ERR_NO_ERROR && bIsImpactArea != TRUE) //Stop Sensor에 감지 되지 않아 Rotate Origin 가능
				{
					nChkAxisDone++;
				}

				if (nErr != ERR_NO_ERROR && nErr != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					bHappenProblem = TRUE;
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Multi-Origin LoadTable %d fail, Motor Y Move Stacker Impact Area");
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}

				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					bHappenProblem = TRUE;
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Multi-Origin LoadTable fail time over!");
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + i, AlarmData);				
					break;
				}
			}

			if (bHappenProblem == TRUE)
			{
				break;
			}

			if (nChkAxisDone == m_vDetectedStopSenTable.size())
			{
				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Motor Y Move Stacker Impact Area [END]]", m_szCmdName, m_nStep);
				m_nStep = 9000;
			}
		}break;

		case 9000: // Rotate Org
		{
			if ((int)m_vAutoCmdMultiOrgLoadTable.size() == 0) {
				m_nStep = 20000;
				break;
			}
			g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d]", m_szCmdName, m_nStep);
			m_tcTimeout.SetCheckTime(TIME_OUT_SHORT);
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
		case 9001:	// Vibrator BWD Check
		{
			int nComp = 0;
			if (g_DMCont.m_dmEQP.GN(NDM0_TABLE_VIBRATOR_OPTION) == DEF_ON)
			{
				for (int i = 0; i < (int)m_vAutoCmdMultiOrgLoadTable.size(); i++) 
				{
					int nErr = m_vAutoCmdMultiOrgLoadTable[i]->chk_cyl_vibrator_onoff(DEF_OFF);
					if (nErr != ERR_NO_ERROR)
					{
						if (m_tcTimeout.CheckOverTime() == TRUE)
						{
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_vAutoCmdMultiOrgLoadTable[i]->m_nTableIdx + 1);
							ReportAlarm(ERR_LOAD_TBL_1_VIBRATOR_CYLINDER_BWD_TIMEOUT + m_vAutoCmdMultiOrgLoadTable[i]->m_nTableIdx, AlarmData);
							break;
						}
						else 
						{
							g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Load Table = %d] Interlock Vibrator On -> Off", m_szCmdName, m_nStep, m_vAutoCmdMultiOrgLoadTable[i]->m_nTableIdx + 1);
							m_nBackVibratorStep = m_nStep;
							m_nStep = do_Table_Vibrator_Off;
							break;
						}			
					}
					else {
						nComp++;
					}				
				}
				if (nComp == m_vAutoCmdMultiOrgLoadTable.size())
				{
					m_nStep++;
				}
			}
			else {
				m_nStep++;
			}
		}break;
		case 9002: //checking Load Table Vibrator OFF
		{		
			if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
				for (int i = 0; i < (int)m_vAutoCmdMultiOrgLoadTable.size(); i++) {
					m_vAutoCmdMultiOrgLoadTable[i]->ChkDI(CTaskLoadTable::xLOAD_TABLE_ROTATE1_SEN, DEF_OFF);
					m_vAutoCmdMultiOrgLoadTable[i]->ChkDI(CTaskLoadTable::xLOAD_TABLE_ROTATE2_SEN, DEF_OFF);
				}
			}

			g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Load Table Rotate Origin [STR]]", m_szCmdName, m_nStep);

			for (int i = 0; i < (int)m_vAutoCmdMultiOrgLoadTable.size(); i++) {
				OneAxis* pMotor = (OneAxis*)(m_vAutoCmdMultiOrgLoadTable[i]->GetAxis(CTaskLoadTable::eAxisRotate));
				pMotor->ClearOrgFlag();
				pMotor->SetStop();
				Sleep(100);
				pMotor->AlarmReset();
				Sleep(100);
				pMotor->ServoOn();
				Sleep(100);
				pMotor->EnableSWLimit(DEF_DISABLE);
			}

			m_nErrUnit = 0;

			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;

		case 9003:	// home search 반대 방향으로 일단 움직인다. 
		{
			BOOL bHappenProblem = FALSE;
			// 전체 센서 감지 상태 확인 한다. home 또는 반대 방향 위치 감지 
			int nLdStopSenIONo[MAX_LOAD_TABLE] = { DI_LOADER_TABLE_1_ROTATOR_2_SEN,
												   DI_LOADER_TABLE_2_ROTATOR_2_SEN,
												   DI_LOADER_TABLE_3_ROTATOR_1_SEN,
												   DI_LOADER_TABLE_4_ROTATOR_1_SEN };

			for (int i = 0; i < (int)m_vAutoCmdMultiOrgLoadTable.size(); i++) {
				OneAxis* pMotor = (OneAxis*)(m_vAutoCmdMultiOrgLoadTable[i]->GetAxis(CTaskLoadTable::eAxisRotate));
						
				bool bHomeSensorON = pMotor->IsHomeSensorOn();
				if (g_IO.in(nLdStopSenIONo[i]) != DEF_ON && bHomeSensorON != true) // 반대 방향 감지 센서 감지 또는 홈 센서가 감지 되지 않으면 반대 방향으로 회전 시도 한다.
				{
					// 반대 방향으로 180도 회전 시도 한다. 
					pMotor->SetCommandPos(0.0);
					
// 					double dNegativeLimit = pMotor->GetSwLimitNega();
// 					int nErr = pMotor->Move(dNegativeLimit, 20.0, 100, 100, _eMotorIntertlock::eMotorInterlock_All_Ignore);
					int nErr = pMotor->Move(LOADER_TABLE_REVERSE_SPIN_DEGREE, 20.0, 100, 100, FALSE, _eMotorIntertlock::eMotorInterlock_All_Ignore);

					if (nErr != ERR_NO_ERROR)
					{
						bHappenProblem = TRUE;
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Multi-Origin LoadTable %d fail", i + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(nErr, AlarmData);
						break;
					}
				}
			}

			if(bHappenProblem == TRUE )
			{
				break;
			}

			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
			m_tcTimeout.StartTimer();
			m_nStep++;

		}break;

		case 9004: // home sensor 또는 반대 방향 확인 센서 감지를 확인 한 후 모터 정지 시킨다.
		{
			if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
				for (int i = 0; i < (int)m_vAutoCmdMultiOrgLoadTable.size(); i++) {
					m_vAutoCmdMultiOrgLoadTable[i]->ChkDI(CTaskLoadTable::xLOAD_TABLE_ROTATE1_SEN, DEF_ON);
					m_vAutoCmdMultiOrgLoadTable[i]->ChkDI(CTaskLoadTable::xLOAD_TABLE_ROTATE2_SEN, DEF_ON);
				}
			}

			BOOL bHappenProblem = FALSE;
			int nLdStopSenIONo[MAX_LOAD_TABLE] = { DI_LOADER_TABLE_1_ROTATOR_2_SEN,
												   DI_LOADER_TABLE_2_ROTATOR_2_SEN,
												   DI_LOADER_TABLE_3_ROTATOR_1_SEN,
												   DI_LOADER_TABLE_4_ROTATOR_1_SEN };

			int nOKCnt = 0;
			for (int i = 0; i < (int)m_vAutoCmdMultiOrgLoadTable.size(); i++) {
				OneAxis* pMotor = (OneAxis*)(m_vAutoCmdMultiOrgLoadTable[i]->GetAxis(CTaskLoadTable::eAxisRotate));
				
				int nErr		   = pMotor->MotionDone();
				bool bHomeSensorON = pMotor->IsHomeSensorOn();

				if (g_IO.in(nLdStopSenIONo[i]) == DEF_ON || bHomeSensorON == true) // 홈 센서 또는 반대 IO센서 감지 되면 모터 정지 한다..
				{				
					pMotor->SetSlowStop();
					nOKCnt++;
					continue;
				}			
				
				if (nErr != ERR_NO_ERROR && nErr != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Multi-Origin LoadTable fail!");
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					bHappenProblem = TRUE;
					break;
				}

				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Multi-Origin LoadTable fail time over!");
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + i, AlarmData);
					bHappenProblem = TRUE;
					break;
				}
			}

			if (bHappenProblem == TRUE)
			{
				break;
			}

			if (nOKCnt == 4)
			{
				m_nStep++;
				m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
				m_tcTimeout.StartTimer();
			}
		}break;

		case 9005:	// motor done 확인
		{
			BOOL bHappenProblem = FALSE;
			int nOKCnt = 0;
			for (int i = 0; i < (int)m_vAutoCmdMultiOrgLoadTable.size(); i++) {
				OneAxis* pMotor = (OneAxis*)(m_vAutoCmdMultiOrgLoadTable[i]->GetAxis(CTaskLoadTable::eAxisRotate));

				int nErr = pMotor->MotionDone();

				if (nErr == ERR_SUCCESS)
				{
					nOKCnt++;
					continue;
				}
				else if (nErr != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					bHappenProblem = TRUE;
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
				
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					bHappenProblem = TRUE;
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Multi-Origin LoadTable fail time over!");
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + i, AlarmData);
					break;
				}
			}

			if (bHappenProblem == TRUE)
			{
				break;
			}

			if (nOKCnt == (int)m_vAutoCmdMultiOrgLoadTable.size())
			{
				m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
				m_tcTimeout.StartTimer();
				m_nStep++;
			}
		}break;

		case 9006: // Rotate Org Done
		{
			int nErrOrg = 0;
			int nComp = 0;

			for (int i = 0; i < (int)m_vAutoCmdMultiOrgLoadTable.size(); i++) {
				OneAxis* pMotor = (OneAxis*)(m_vAutoCmdMultiOrgLoadTable[i]->GetAxis(CTaskLoadTable::eAxisRotate));
				nErrOrg = pMotor->Origin();
				if (nErrOrg != NO_ERROR) { // error
					m_nOrgRetryCount++;
					if (m_nOrgRetryCount >= MAX_MOTOR_RETRY_COUNT) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Multi-Origin LoadTable fail");
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(nErrOrg, AlarmData);
						break;
					}
					m_nStep--;
					break;
				}
				else // org done
				{
					BOOL bCompleted = pMotor->IsOrgCompleted(&nErrOrg);
					if (bCompleted)
					{
						pMotor->EnableSWLimit(DEF_ENABLE);
						int nErr = pMotor->ChkMotorUsable();
						if (nErr == ERR_NO_ERROR) {
							nComp++;
						}
					}
					else
					{
						if (nErrOrg == ERR_MOTOR_CMN_IN_MOVE_STATE)
						{
							if (m_tcTimeout.CheckOverTime() == TRUE)
							{
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat("Multi-Origin LoadTable fail time over!");
								AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
								ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + i, AlarmData);
								break;
							}
						}
						else {
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("Multi-Origin LoadTable fail");
							AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
							ReportAlarm(nErrOrg, AlarmData);
							break;
						}
					}
				}

			}

			if (nComp == (int)m_vAutoCmdMultiOrgLoadTable.size()) {
				m_nStep++;
				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Load Table Y Origin [END]]", m_szCmdName, m_nStep);
			}
		}break;

		case 9007:  // roator 홈센서 도그 형항 변경으로 추가 시퀀스 (roator를 Station work position 이동)
		{		
			BOOL bFailHappen = FALSE;
			for (int i = 0; i < (int)m_vAutoCmdMultiOrgLoadTable.size(); i++) {
				OneAxis* pMotor = (OneAxis*)(m_vAutoCmdMultiOrgLoadTable[i]->GetAxis(CTaskLoadTable::eAxisRotate));
					

				double dTPos = m_vAutoCmdMultiOrgLoadTable[i]->GetTeachPos(LOC_IDX_LD_TBL_TEST_SIDE_1, CTaskLoadTable::eAxisRotate);
				int nErr = pMotor->Move(dTPos, 20.0, 100, 100, FALSE, _eMotorIntertlock::eMotorInterlock_All_Ignore);

//				int nErr = m_vAutoCmdMultiOrgLoadTable[i]->do_MovePos(LOC_IDX_LD_TBL_TEST_SIDE_1, CTaskLoadTable::eAxisRotate);

				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Load Table %d, move rotate %s command error", m_vAutoCmdMultiOrgLoadTable[i]->m_nTableIdx + 1, _T("LoadTblCmd_Initial"));
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					bFailHappen = TRUE;
					break;
				}
			}

			if (bFailHappen == TRUE)
			{
				break;
			}

			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_SHORT);
			m_tcTimeout.StartTimer();
			m_nStep++;
		}

		case 9008:
		{
			int nComp = 0;

			for (int i = 0; i < (int)m_vAutoCmdMultiOrgLoadTable.size(); i++) {
				OneAxis* pMotor = m_vAutoCmdMultiOrgLoadTable[i]->GetAxis(CTaskLoadTable::eAxisRotate);
				int nAxisDone = pMotor->AxisDone();
				if (nAxisDone == ERR_NO_ERROR) {

					//Rotate Sensor 확인.
					if (m_vAutoCmdMultiOrgLoadTable[i]->chk_rotate_direction(CTaskLoadTable::eDir_TestSite) != ERR_NO_ERROR && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Load Table %d, rotate direction state error, state : %s ", m_vAutoCmdMultiOrgLoadTable[i]->m_nTableIdx + 1, _T("LoadTblCmd_Initial"));
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(ERR_LOAD_TBL_ROTATE_STATE_TEST_SITE_FAIL, AlarmData);
						break;
					}

					nComp++;
				}
				else
				{
					if (m_tcTimeout.CheckOverTime() == TRUE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_vAutoCmdMultiOrgLoadTable[i]->m_nTableIdx + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_vAutoCmdMultiOrgLoadTable[i]->m_nTableIdx, AlarmData);
						break;
					}

					if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("%s Axis", pMotor->GetAxisName());
						ReportAlarm(nAxisDone, AlarmData);
						break;
					}
				}
			}


			if (nComp == m_vAutoCmdMultiOrgLoadTable.size())
			{
				m_nStep++;
			}

		}break;
		

		case 9009: // Table Y Org
			{
				if( m_vAutoCmdMultiOrgLoadTable.size() == 0 ) {
					m_nStep = 20000;
					break;
				}
				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Load Table Y Origin [STR]]", m_szCmdName, m_nStep);

				for( int i = 0; i < ( int )m_vAutoCmdMultiOrgLoadTable.size(); i++ ) {
					OneAxis* pMotor = ( OneAxis* )( m_vAutoCmdMultiOrgLoadTable[ i ]->GetAxis( CTaskLoadTable::eAxisY ) );
					pMotor->ClearOrgFlag();
					pMotor->SetStop();
					Sleep( 100 );
					pMotor->AlarmReset();
					Sleep( 100 );
					pMotor->ServoOn();
					Sleep( 100 );
					pMotor->EnableSWLimit( DEF_DISABLE );
				}

				for (int i = 0; i < (int)m_vAutoCmdMultiOrgLoadTable.size(); i++) {
					m_vAutoCmdMultiOrgLoadTable[i]->m_bInitialized = FALSE;
				}
				m_nErrUnit = 0;

				m_tcTimeout.SetCheckTime( MOTOR_TIME_OUT_MIDDLE );
				m_tcTimeout.StartTimer();
				m_nStep++;
			}break;
		case 9010:
			{
				int nErrOrg = 0;
				int nComp = 0;

				for( int i = 0; i < ( int )m_vAutoCmdMultiOrgLoadTable.size(); i++ ) {
					OneAxis* pMotor = ( OneAxis* )( m_vAutoCmdMultiOrgLoadTable[ i ]->GetAxis( CTaskLoadTable::eAxisY ) );
					nErrOrg = pMotor->Origin();
					if( nErrOrg != NO_ERROR ) { // error
						m_nOrgRetryCount++;
						if (m_nOrgRetryCount >= MAX_MOTOR_RETRY_COUNT) {
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("Multi-Origin LoadTable fail");
							AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
							ReportAlarm(nErrOrg, AlarmData);
							break;
						}
						m_nStep--;
						break;
					}
					else 
					{ // org done
						BOOL bCompleted = pMotor->IsOrgCompleted(&nErrOrg);
						if (bCompleted)
						{
							pMotor->EnableSWLimit(DEF_ENABLE);
							int nErr = pMotor->ChkMotorUsable();
							if (nErr == ERR_NO_ERROR) {
								nComp++;
							}
						}
						else 
						{
							if (nErrOrg == ERR_MOTOR_CMN_IN_MOVE_STATE)
							{
								if (m_tcTimeout.CheckOverTime() == TRUE)
								{
									CParamList AlarmData;
									AlarmData.m_fnAddItemFormat("Multi-Origin LoadTable fail time over!");
									AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
									ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nErrUnit, AlarmData);
									break;
								}
							}
							else {
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat("Multi-Origin LoadTable fail");
								AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
								ReportAlarm(nErrOrg, AlarmData);
								break;
							}
							
						}
					}				
				}
				if( nComp == ( int )m_vAutoCmdMultiOrgLoadTable.size() ) {	
					for (int i = 0; i < (int)m_vAutoCmdMultiOrgLoadTable.size(); i++) {
						m_vAutoCmdMultiOrgLoadTable[i]->m_bInitialized = TRUE;
					}
					m_nStep = 20000;		

					//loading table initial
				
 					g_TaskLoadTbl[eLoadTbl_2].InitialTrigger();
 					g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Load Table2 Trigger Initial]", m_szCmdName, m_nStep);
 					
 					
 					
 					g_TaskLoadTbl[eLoadTbl_3].InitialTrigger();
 					g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Load Table3 Trigger Initial]", m_szCmdName, m_nStep);
					

					m_vAutoCmdMultiOrgLoadTable.clear();
					g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [Load Table Y Origin [END]]", m_szCmdName, m_nStep);
				}
			}break;
		case do_Table_Vibrator_Off:
		{
			for (int i = 0; i < (int)m_vAutoCmdMultiOrgLoadTable.size(); i++) {
				m_vAutoCmdMultiOrgLoadTable[i]->do_SideVibratorOnOff(DEF_OFF);
				g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [do_Vibrator_Off]", m_szCmdName, m_nStep);
			}
			tmr.StartTmChk();

			m_nStep = do_Table_Vibrator_Off_Wait;

			break;
		}
		case do_Table_Vibrator_Off_Wait:
		{
			// Vibrator Delay Time 추가 [0보다 작거나 '-' 값이 들어가면 최소값 0.1로 만듬] [8/4/2020 donghyun.shin]
			double dVibDelayTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_LOADING_TABLE_VIBRATOR_DELAY_TIME);

			if (dVibDelayTime <= 0)
				dVibDelayTime = 0.05;

			tmr.EndTmChk();
			if (tmr.GetTmDistanceMS() < dVibDelayTime*1000.0)
				break;

			g_TaskSystemCtrl.MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] [do_Vibrator_Off_Wait]", m_szCmdName, m_nStep);
			m_nStep = Table_Vibrator_complete;
			break;
		}
		case Table_Vibrator_complete:
		{
			m_nStep = m_nBackVibratorStep;
			break;
		}
		case 20000: // check complete
			{
				ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_NormalComplete );
			}break;
	}
	return CBaseSeqCmd::State_Executing();
}

//==============================================================================
//
//==============================================================================
BOOL CMultiOrgCmd_Axis::State_Pause()
{
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		g_TaskSystemCtrl.MakeLog( "[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
		case 0:
			{
				if( ( int )m_vAutoCmdMultiOrgPress.size() != 0 ) {
					for( int i = 0; i < ( int )m_vAutoCmdMultiOrgPress.size(); i++ ) {
						OneAxis* pMotor = ( OneAxis* )( m_vAutoCmdMultiOrgPress[ i ]->GetAxis( CTaskPressUnit::eAxisZ ) );
						pMotor->SetStop();
						pMotor->OriginStop();
					}
				}

				if( ( int )m_vAutoCmdMultiOrgTestPp.size() != 0 ) {
					for( int i = 0; i < ( int )m_vAutoCmdMultiOrgTestPp.size(); i++ ) {
						for( int j = CTaskTestPP::eAxisX1; j < CTaskTestPP::eAxisMax; j++ ) {
							OneAxis* pMotor = m_vAutoCmdMultiOrgTestPp[ i ]->GetAxis( j );
							pMotor->SetStop();
							pMotor->OriginStop();
						}
					}
				}

				if( ( int )m_vAutoCmdMultiOrgLoadTable.size() != 0 ) {
					for( int i = 0; i < ( int )m_vAutoCmdMultiOrgLoadTable.size(); i++ ) {
						OneAxis* pMotor = ( OneAxis* )( m_vAutoCmdMultiOrgLoadTable[ i ]->GetAxis( CTaskLoadTable::eAxisY ) );
						pMotor->SetStop();
						pMotor->OriginStop();
					}
				}

				if ((int)m_vAutoCmdMultiOrgTrayPp.size() != 0) {
					for (int i = 0; i < (int)m_vAutoCmdMultiOrgTrayPp.size(); i++) {
						for (int j = 0; j < CTaskTrayBasePP::eAxisMax; j++) {

							OneAxis* pMotor = m_vAutoCmdMultiOrgTrayPp[i]->GetAxis((CTaskTrayBasePP::eMotor)(j));
							if( pMotor == nullptr ) continue;
							pMotor->SetStop();
							pMotor->OriginStop();
						}
					}
				}

				for (int i = 0; i < CTaskTrayFeeder::eAxisMax; i++)
				{
					OneAxis* pMotor = g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eMotor(i));
					if (pMotor == nullptr) continue;
					pMotor->SetStop();
					pMotor->OriginStop();

				}

				if ((m_nCmdStage == eMultiOrgType_EqpUpAll || m_nCmdStage == eMultiOrgType_AxisAll))
				{
					for (int i = 0; i < CTaskTransfer::eMaxAxisCount; i++) {
						OneAxis* pMotor = g_TaskTransfer.GetAxis(i);
						pMotor->SetStop();
						pMotor->OriginStop();
					}

	
				}

				m_nStep++;
			}break;
		case 1:
			{
				SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Aborting );
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
BOOL CMultiOrgCmd_Axis::State_Recovery()
{
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		g_TaskSystemCtrl.MakeLog( "[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
		case 0:	// 현재 위치와 마지막 위치를 비교 한다.
			{
				SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Executing );
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
BOOL CMultiOrgCmd_Axis::State_NormalComplete()
{
	//g_TaskSystemCtrl.SendEvent(CEID_MULTI_ORG_END);

	if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Manual && m_nCmdStage == eMultiOrgType_AxisAll)
	{
		g_DMCont.m_dmHandlerOpt.SN(NDM11_MULTIORIGIN_HISTORY, eMulti_Origin_End);

		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Multi-Origin Completed!");
		ReportAlarm(ERR_MULTI_ORIGIN_COMPLETED, AlarmData);
	}
	
	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CMultiOrgCmd_Axis::State_Alarm()
{
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		g_TaskSystemCtrl.MakeLog( "[CMD: %s] [ALARM] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
		case 0: // alarm 발생 했을 때 처리해5 줘야 할꺼 처리 해줌
			{
				if( ( int )m_vAutoCmdMultiOrgPress.size() != 0 ) {
					for( int i = 0; i < ( int )m_vAutoCmdMultiOrgPress.size(); i++ ) {
						OneAxis* pMotor = ( OneAxis* )( m_vAutoCmdMultiOrgPress[ i ]->GetAxis( CTaskPressUnit::eAxisZ ) );
						pMotor->SetStop();
						pMotor->OriginStop();
					}
				}

				if( ( int )m_vAutoCmdMultiOrgTestPp.size() != 0 ) {
					for( int i = 0; i < ( int )m_vAutoCmdMultiOrgTestPp.size(); i++ ) {
						for( int j = CTaskTestPP::eAxisX1; j < CTaskTestPP::eAxisMax; j++ ) {
							OneAxis* pMotor = m_vAutoCmdMultiOrgTestPp[ i ]->GetAxis( j );
							pMotor->SetStop();
							pMotor->OriginStop();
						}
					}
				}

				if( ( int )m_vAutoCmdMultiOrgLoadTable.size() != 0 ) {
					for( int i = 0; i < ( int )m_vAutoCmdMultiOrgLoadTable.size(); i++ ) {
						OneAxis* pMotor = ( OneAxis* )( m_vAutoCmdMultiOrgLoadTable[ i ]->GetAxis( CTaskLoadTable::eAxisY ) );
						if( pMotor == nullptr ) continue;
						pMotor->SetStop();
						pMotor->OriginStop();
					}
				}
				if ((int)m_vAutoCmdMultiOrgTrayPp.size() != 0) {
					for (int i = 0; i < (int)m_vAutoCmdMultiOrgTrayPp.size(); i++) {
						for (int j = 0; j < CTaskTrayBasePP::eAxisMax; j++) {
							OneAxis* pMotor = m_vAutoCmdMultiOrgTrayPp[i]->GetAxis((CTaskTrayBasePP::eMotor)(j));
							if( pMotor == nullptr ) continue;
							pMotor->SetStop();
							pMotor->OriginStop();
						}
					}
				}

				for (int i = 0; i < CTaskTrayFeeder::eAxisMax; i++) 
				{
					OneAxis* pMotor = g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eMotor(i));
					if (pMotor == nullptr) continue;
						pMotor->SetStop();
						pMotor->OriginStop();
					
				}

				//if( ( int )m_vAutoCmdMultiOrgEtc.size() != 0 ) {
				if ((m_nCmdStage == eMultiOrgType_EqpUpAll || m_nCmdStage == eMultiOrgType_AxisAll) )
				{
					for( int i = 0; i < CTaskTransfer::eMaxAxisCount; i++ ) {
						OneAxis* pMotor = g_TaskTransfer.GetAxis( i );
						pMotor->SetStop();
						pMotor->OriginStop();
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
BOOL CMultiOrgCmd_Axis::State_AlarmComplete()
{
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		g_TaskSystemCtrl.MakeLog( "[CMD: %s] [ALARM COMP] [Step=%8d]", m_szCmdName, m_nStep );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
		case 0:
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
BOOL CMultiOrgCmd_Axis::State_Aborting()
{
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		g_TaskSystemCtrl.MakeLog( "[CMD: %s] [ABORT] [Step=%8d]", m_szCmdName, m_nStep );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
		case 0:
			{
				if( ( int )m_vAutoCmdMultiOrgPress.size() != 0 ) {
					for( int i = 0; i < ( int )m_vAutoCmdMultiOrgPress.size(); i++ ) {
						OneAxis* pMotor = ( OneAxis* )( m_vAutoCmdMultiOrgPress[ i ]->GetAxis( CTaskPressUnit::eAxisZ ) );
						pMotor->SetStop();
						pMotor->OriginStop();
						pMotor->EnableSWLimit(DEF_ENABLE);
					}
				}

				if( ( int )m_vAutoCmdMultiOrgTestPp.size() != 0 ) {
					for( int i = 0; i < ( int )m_vAutoCmdMultiOrgTestPp.size(); i++ ) {
						for( int j = CTaskTestPP::eAxisX1; j < CTaskTestPP::eAxisMax; j++ ) {
							OneAxis* pMotor = m_vAutoCmdMultiOrgTestPp[ i ]->GetAxis( j );
							pMotor->SetStop();
							pMotor->OriginStop();
							pMotor->EnableSWLimit(DEF_ENABLE);
						}
					}
				}

				if( ( int )m_vAutoCmdMultiOrgLoadTable.size() != 0 ) {
					for( int i = 0; i < ( int )m_vAutoCmdMultiOrgLoadTable.size(); i++ ) {
						OneAxis* pMotor = ( OneAxis* )( m_vAutoCmdMultiOrgLoadTable[ i ]->GetAxis( CTaskLoadTable::eAxisY ) );
						pMotor->SetStop();
						pMotor->OriginStop();
						pMotor->EnableSWLimit(DEF_ENABLE);
					}
				}

				if ((int)m_vAutoCmdMultiOrgTrayPp.size() != 0) {
					for (int i = 0; i < (int)m_vAutoCmdMultiOrgTrayPp.size(); i++) {
						for (int j = 0; j < CTaskTrayBasePP::eAxisMax; j++) {
							OneAxis* pMotor = m_vAutoCmdMultiOrgTrayPp[i]->GetAxis((CTaskTrayBasePP::eMotor)(j));
							if(pMotor == nullptr ) continue;
							pMotor->SetStop();
							pMotor->OriginStop();
							pMotor->EnableSWLimit(DEF_ENABLE);
						}
					}
				}

				for (int i = 0; i < CTaskTrayFeeder::eAxisMax; i++)
				{
					OneAxis* pMotor = g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eMotor(i));
					if (pMotor == nullptr) continue;
					pMotor->SetStop();
					pMotor->OriginStop();
					pMotor->EnableSWLimit(DEF_ENABLE);
				}

				if ((m_nCmdStage == eMultiOrgType_EqpUpAll || m_nCmdStage == eMultiOrgType_AxisAll))
				{
					for (int i = 0; i < CTaskTransfer::eMaxAxisCount; i++) {
						OneAxis* pMotor = g_TaskTransfer.GetAxis(i);
						pMotor->SetStop();
						pMotor->OriginStop();
						pMotor->EnableSWLimit(DEF_ENABLE);
					}
				}

				m_nStep++;
			}break;
		case 1:
			{
				ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_AbortComplete ); // 명령 진행 중 STOP 명령을 받아 명령이 중단 됨.
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
BOOL CMultiOrgCmd_Axis::State_AbortComplete()
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pMainFrm->PostMessage(UM_GUI_MULTI_ORIGIN, (WPARAM)eMultiOriginStop, NULL);

	return CBaseSeqCmd::State_AbortComplete();
}