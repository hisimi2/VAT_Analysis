#include "StdAfx.h"
#include "TrayLoadPPCmd_MovePickPlace.h"

#define PICKER_FAIL_RETRY_CNT	(1)

CTrayLoadPPCmd_MovePickPlace::CTrayLoadPPCmd_MovePickPlace( void )
{
	eStage = eLocIdxTrayPP::LOC_IDX_TRAY_PP_Undefined;
	Pocket = CPoint( 0, 0 );
	vPicker.clear();
	vSpecOutPicker.clear();
	vFullCmdPkr.clear();
	ePickPlace = ePPCmd_Unknown;
	bNormalPkrEnd = FALSE;
	bSpeckOutPkrEnd = FALSE;
	nCntSpecOut = 0;
	bSpecOutPkrUse = FALSE;

	m_nCmdId = eSeqCmd_TrayLoadPP_MovePickPlace;
	m_nRetryCount = 0;
	m_nSetRetryCount = 0;
	m_dWaitTime = 0.2;
	m_bDownCheckEnable = FALSE;
	m_dDownCheckTimeout = 5.0;
	sprintf_s( m_szCmdName, sizeof( m_szCmdName ), "TrayLoadPPCmd_MovePickPlace" );

	for( int i = 0; i < CTaskTrayBasePP::eAxisMax; i++ ) {
		m_dInPosRange[ i ] = MOTOR_INPOSITION;      // [um]
	}

	m_tcTimeout = 0.0;
	m_tcTimeoutPicker = 0.0;
	bVacuumCheckFuction = FALSE;
}

CTrayLoadPPCmd_MovePickPlace::~CTrayLoadPPCmd_MovePickPlace( void )
{
}

int CTrayLoadPPCmd_MovePickPlace::CheckParam()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = ( CTaskTrayLoadPP* )m_pReceive;

	m_nSetRetryCount = g_DMCont.m_dmTrayPP.GN( NDM5_LDTrayPP_RetryCnt );
	switch( eStage )
	{
	case eLocIdxTrayPP::LOC_IDX_TRAY_PP_CONV_3:
	case eLocIdxTrayPP::LOC_IDX_TRAY_PP_FEEDER:
	case eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY:
	// case eLocIdxTrayPP::LOC_IDX_TRAY_PP_TBL_1: Unload Table
	case eLocIdxTrayPP::LOC_IDX_TRAY_PP_TBL_2: // Load Table
	case eLocIdxTrayPP::LOC_IDX_TRAY_PP_TBL_3: // Load Table
	// case eLocIdxTrayPP::LOC_IDX_TRAY_PP_TBL_4: Unload Table
		{
			m_nWorkCmdStage = eStage;
		}break;
	default:
		{
			return ERR_CMD_PARAM_INVALID;
		}break;
	}

	m_TransferInterlock.SetCheckTime( MOTOR_TIME_OUT_MIDDLE );
	m_TransferInterlock.SetTimerCheckEnd();

	return ERR_NO_ERROR;
}

int CTrayLoadPPCmd_MovePickPlace::OnStop()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Aborting );
	return ERR_NO_ERROR;
}

int CTrayLoadPPCmd_MovePickPlace::OnRetry()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

int CTrayLoadPPCmd_MovePickPlace::OnSkip()
{
	ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_AlarmComplete );
	return ERR_NO_ERROR;
}

int CTrayLoadPPCmd_MovePickPlace::OnPause()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Pause );
	return ERR_NO_ERROR;
}

int CTrayLoadPPCmd_MovePickPlace::OnResume()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

BOOL CTrayLoadPPCmd_MovePickPlace::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}

BOOL CTrayLoadPPCmd_MovePickPlace::State_Executing()
{
	// check alarm
	CTaskTrayLoadPP* pTaskTrayLoadPP = ( CTaskTrayLoadPP* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTaskTrayLoadPP->MakeLog( _T( "[CMD: %s] [EXECUTE] [Step=%s]" ), m_szCmdName, act_string( m_nStep ) );
	}
	m_nStep_Pre = m_nStep;

	for( int i = 0; i < CTaskTrayBasePP::eAxisMax; i++ ) {
		OneAxis* pMotor = pTaskTrayLoadPP->GetAxis( static_cast< CTaskTrayBasePP::eMotor >( i ) );
		if( pMotor != nullptr ) {
			if( pMotor->ChkMotorUsable() != ERR_NO_ERROR ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Tray Load PP check motor usable command error" );
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm( ERR_TRAY_PP_1_MOTOR_ALARMED, AlarmData );
				return FALSE;
			}
		}
	}

	OneAxis* pMotorX = pTaskTrayLoadPP->GetAxis( CTaskTrayBasePP::eAxisX );
	OneAxis* pMotorZ = pTaskTrayLoadPP->GetAxis( CTaskTrayBasePP::eAxisZ );
	OneAxis* pMotorPX = pTaskTrayLoadPP->GetAxis( CTaskTrayBasePP::eAxisPitchX );
	OneAxis* pMotorPY = pTaskTrayLoadPP->GetAxis( CTaskTrayBasePP::eAxisPitchY );

	int nCntSpecOut = 0;
	switch( m_nStep )
	{
	case start: 
		{
			m_nStep = z_move_safety;
		}break;

	case z_move_safety: // move z axis safety & picker all up
		{
			std::vector<CPoint> vPkr;
			pTaskTrayLoadPP->MakeAllPicker(vPkr);
			pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [z move safety & picker all up] [SRT]", m_szCmdName, m_nStep );
			int nErr = pTaskTrayLoadPP->do_MovePos( eStage, Pocket, vPkr, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ );
			if( nErr != ERR_NO_ERROR ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Tray Load PP z axis move safety position command error" );
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm( nErr, AlarmData );
				break;
			}
			m_tcTimeout = ( double )GetTickCount64();//check time picker up
			m_nStep = z_move_safety_chk;
		}break;
	case z_move_safety_chk: // check move z safety
		{	
			std::vector<CPoint> vPkr;
			pTaskTrayLoadPP->MakeAllPicker(vPkr);
			int nErrSen = pTaskTrayLoadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);
			m_vErrPicker.clear();
			int nAxisDone = pMotorZ->AxisDone(vPkr, m_vErrPicker);
			if( nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR) 
			{
				pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [z move safety & picker all up] [END]", m_szCmdName, m_nStep );
				m_nRetryCount = 0;
				m_nStep = x_px_py_move;
				break;
			}
			else {
				std::vector<CPoint> vErrPkr; vErrPkr.clear();
				CString sAlarmData = _T("");
				int nErr = pTaskTrayLoadPP->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, pTaskTrayLoadPP->GetSafetyFailPicker(ePickerSafetySensorCheckType::eSafetySensorCheck), vErrPkr, sAlarmData);

				double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
				if (dSpanTime > (TIME_OUT_SHORT * 1000.0)) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("%s", sAlarmData);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
					ReportAlarm(nErr, AlarmData, vErrPkr);
					break;
				}
			}
		}break;

	case x_px_py_move: // move x,y,xp,yp to target pos
		{
			double dTPos[ 3 ] = { 0.0, };
			dTPos[ 0 ] = pTaskTrayLoadPP->GetTeachPos( eStage, Pocket, vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX );
			dTPos[ 1 ] = pTaskTrayLoadPP->GetTeachPos( eStage, Pocket, vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisPitchX );
			dTPos[ 2 ] = pTaskTrayLoadPP->GetTeachPos( eStage, Pocket, vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisPitchY );
			//m_Cmd.Picker를 m_vFullPicker

			int nAxisNo[ 3 ] = { CTaskTrayBasePP::eAxisX, CTaskTrayBasePP::eAxisPitchX, CTaskTrayBasePP::eAxisPitchY };
			pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [x, y, Xpitch, Ypitch move %s x:%d / y:%d] [SRT]", m_szCmdName, m_nStep, pTaskTrayLoadPP->GetLocName( eStage ), Pocket.x + 1, Pocket.y + 1 );
			int nErr = -1;
			for( int i = 0; i < 3; i++ ) {
				if( bSpecOutPkrUse == TRUE )
				{
					if( bNormalPkrEnd == FALSE )
					{
						if( ePickPlace == DEF_PICK ) {
							nErr = pTaskTrayLoadPP->do_MovePos( eStage, Pocket, vPicker, CTaskTrayBasePP::eZPOS_SAFETY, ( CTaskTrayBasePP::eMotor )nAxisNo[ i ] );
						}
						else {
							nErr = pTaskTrayLoadPP->do_MovePos( eStage, Pocket, pTaskTrayLoadPP->m_vFullPkr, CTaskTrayBasePP::eZPOS_SAFETY, ( CTaskTrayBasePP::eMotor )nAxisNo[ i ] );
						}
						//Loading Tbl, C-Tray 위치로 이동 할 때는 Specout된 Picker(m_Cmd.Picker, m_vSpecOutPicker 분리)가 있더라도 전체 m_vFullPkr경로로 줘야 Pocket 제 위치로 이동 
					}
					else  //Spec Out Picker Exist
					{
						if( pTaskTrayLoadPP->GetPickerAlignStatus( vSpecOutPicker[ nCntSpecOut - 1 ].x, vSpecOutPicker[ nCntSpecOut - 1 ].y ) == TRUE )
						{
							double dOffsetX = 0.0, dOffsetY = 0.0;
							pTaskTrayLoadPP->GetPickerOffset( dOffsetX, dOffsetY, vSpecOutPicker[ nCntSpecOut - 1 ].x, vSpecOutPicker[ nCntSpecOut - 1 ].y );
							if( CTaskTrayBasePP::eAxisX == 0 )		// x offset 추가
							{
								nErr = pTaskTrayLoadPP->do_MovePos( eStage, Pocket, pTaskTrayLoadPP->m_vFullPkr, CTaskTrayBasePP::eZPOS_SAFETY, ( CTaskTrayBasePP::eMotor )nAxisNo[ i ], 100, dOffsetX );
							}
							else
							{
								nErr = pTaskTrayLoadPP->do_MovePos( eStage, Pocket, pTaskTrayLoadPP->m_vFullPkr, CTaskTrayBasePP::eZPOS_SAFETY, ( CTaskTrayBasePP::eMotor )nAxisNo[ i ] );
							}
						}
					}
				}
				else   //SpecOut Picker Option Not Use
				{
					nErr = pTaskTrayLoadPP->do_MovePos( eStage, Pocket, vPicker, CTaskTrayBasePP::eZPOS_SAFETY, ( CTaskTrayBasePP::eMotor )nAxisNo[ i ] );
				}
				if( nErr != ERR_NO_ERROR ) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( "Tray PP 4axis(x,x-pitch,y-pitch) move command error" );
					AlarmData.m_fnAddItemFormat( "Problem Axis = %d, Error=%d, CmdStage=%d, CmdPocket=(X:%d,Y:%d)", nAxisNo[ i ], nErr, eStage, Pocket.x + 1, Pocket.y + 1 );
					OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm( nErr, AlarmData );
					break;
				}
			}
			m_tcTimeout = ( double )GetTickCount64();
			m_nStep = x_px_py_move_chk;
		}break;

	case x_px_py_move_chk: // check x,y,xp,yp axis done
		{
			int nAxisNo[ 3 ] = { CTaskTrayBasePP::eAxisX, CTaskTrayBasePP::eAxisPitchX, CTaskTrayBasePP::eAxisPitchY };

			double dSpanTime = ( double )GetTickCount64() - m_tcTimeout;
			if( dSpanTime > ( MOTOR_TIME_OUT_MIDDLE * 1000.0 ) ) {
				CString strErrAxis = _T( "" );
				for( int i = 0; i < 3; i++ ) {
					OneAxis* pMotor = pTaskTrayLoadPP->GetAxis( CTaskTrayBasePP::eMotor( nAxisNo[ i ] ) );
					int nAxisDone = pMotor->AxisDone();
					if( nAxisDone != ERR_NO_ERROR ) {
						if( strErrAxis.Compare( "" ) != 0 ) {
							strErrAxis += ", ";
						}
						strErrAxis += pMotor->GetAxisName();
					}
				}
				double dPosX = 0, dPosPX = 0, dPosPY = 0;
				pMotorX->GetCurrentPos(dPosX);
				pMotorPX->GetCurrentPos(dPosPX);
				pMotorPY->GetCurrentPos(dPosPY);

				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Load Tray PP 3axis operation is not complete" );
				AlarmData.m_fnAddItemFormat( "%s motor time over!", strErrAxis );
				AlarmData.m_fnAddItemFormat( "[current position] x=%.2f, x-pitch=%.2f, y-pitch=%.2f", dPosX / 1000.0, dPosPX / 1000.0, dPosPY / 1000.0 );
				ReportAlarm( ERR_TRAY_PP_1_MOTOR_MOVE_TIMEOUT, AlarmData );
				break;
			}
			int nChkComp = 0;
			for( int i = 0; i < 3; i++ ) {
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis( CTaskTrayBasePP::eMotor( nAxisNo[ i ] ) );
				int nAxisDone = pMotor->AxisDone();
				if( nAxisDone == ERR_NO_ERROR ) {
					nChkComp++;
				}
				else if( nAxisDone == -1 ) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( "Tray PP 3axis operation is not complete" );
					AlarmData.m_fnAddItemFormat( "[ check axis done ] x=%d, x-pitch=%d, y-pitch=%d", pMotorX->AxisDone(), pMotorPX->AxisDone(), pMotorPY->AxisDone() );
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm( ERR_TRAY_PP_1_MOTOR_MOVE_FAIL, AlarmData );
					break;
				}
			}

			if( nChkComp == 3 ) {
				pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [x, y, Xpitch, Ypitch move %s x:%d / y:%d] [END]", m_szCmdName, m_nStep, pTaskTrayLoadPP->GetLocName( eStage ), Pocket.x + 1, Pocket.y + 1 );

				// waiting for loadtable
				if( LOC_IDX_TRAY_PP_TBL_2 <= eStage && eStage <= LOC_IDX_TRAY_PP_TBL_3 && CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto ) {
					m_nStep = waiting_for_load_table; // checking loop
					m_tcTimeout = ( double )GetTickCount64();
					pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [Wait %s] [SRT]", m_szCmdName, m_nStep, pTaskTrayLoadPP->GetLocName( eStage ) );
					break;
				}
				else if (eStage == LOC_IDX_TRAY_PP_FEEDER && CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto)
				{
					m_nStep = waiting_for_feeder;
					m_tcTimeout = (double)GetTickCount64();
					pTaskTrayLoadPP->MakeLog("[%s EXEC=%8d] [Wait %s] [SRT]", m_szCmdName, m_nStep, pTaskTrayLoadPP->GetLocName(eStage));
					break;
				}
				else
				{
					m_nStep = move_z_target_pos;
					break;
				}
				// command stage is not loadtable

			}
		}break;

		//------------------------------------------------------------------------------------------
	case waiting_for_load_table : // waiting for load table
		{
			int nLoadTblIndex = eStage - LOC_IDX_TRAY_PP_TBL_1;

			//check time over
			double dSpanTime = ( double )GetTickCount64() - m_tcTimeout;
			if( dSpanTime > ( MOTOR_TIME_OUT_MIDDLE * 1000.0 ) ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Tray PP waiting for  %s time over", pTaskTrayLoadPP->GetLocName( eStage ) );
				ReportAlarm( ERR_LOAD_TBL_1_MOVE_TIMEOUT + nLoadTblIndex, AlarmData );
				break;
			}

			int table_row_step = g_TaskLoadTbl[nLoadTblIndex].GetTableRowStep();

			// checking load tbl position
			int nRetLoadTblPos = g_TaskLoadTbl[ nLoadTblIndex ].ChkMotorPos( LOC_IDX_TBL_LD_SIDE, CTaskLoadTable::eAxisAll, table_row_step);
			if( nRetLoadTblPos == ERR_NO_ERROR && g_TaskLoadTbl[ nLoadTblIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready ) {
				pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [Wait %s] [END]", m_szCmdName, m_nStep, pTaskTrayLoadPP->GetLocName( eStage ) );
				m_nStep = move_z_target_pos;
				break;
			}
		}break;

		//------------------------------------------------------------------------------------------
	case waiting_for_feeder: // waiting for feeder
	{
		int nLoadTblIndex = LOC_IDX_TRAY_PP_FEEDER;

		//check time over
		OneAxis* pMotor = (OneAxis*)g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eMotor::eAxisY);
		double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
		if (dSpanTime > (MOTOR_TIME_OUT_MIDDLE * 1000.0)) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray PP waiting for  %s time over", pTaskTrayLoadPP->GetLocName(eStage));
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(ERR_FEEDER_MOTOR_MOVE_TIMEOUT + nLoadTblIndex, AlarmData);
			break;
		}

		// checking feeder position
		int nFeederStep = g_TaskTrayFeeder.GetTrayDeviceExistRowStep();
		int nRetFeederPos = g_TaskTrayFeeder.ChkMotorPos(CTaskTrayFeeder::eAxisY, LOC_IDX_FEEDER_LOAD_PICK_START_POS, nFeederStep);
		if (nRetFeederPos == ERR_NO_ERROR && g_TaskTrayFeeder.GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready) {
			pTaskTrayLoadPP->MakeLog("[%s EXEC=%8d] [Wait %s] [END]", m_szCmdName, m_nStep, pTaskTrayLoadPP->GetLocName(eStage));
			m_nStep = move_z_target_pos;
			break;
		}
	}break;

		//------------------------------------------------------------------------------------------
	case  move_z_target_pos : // move z target pos
		{
			int nCylinderWaitTm = g_DMCont.m_dmTrayPP.GN( NDM5_TM_LDTrayPP_CylDownWait );
			double dSpanTime = ( double )GetTickCount64() - m_tcTimeoutPicker;
			int nRemainTime = nCylinderWaitTm - ( int )dSpanTime;

			if( ePickPlace == DEF_PLACE && CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto )
			{
				pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [Before blow check device] [STR]", m_szCmdName, m_nStep );
				CString strMsg = _T( "" );
				std::vector<CString> vErrString; vErrString.clear();
				int nChkErr = pTaskTrayLoadPP->CheckDevice( strMsg, vPicker, vErrString);
				if( nChkErr != ERR_NO_ERROR ) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( strMsg );
					ReportAlarm( nChkErr, AlarmData, pTaskTrayLoadPP->GetFailPicker());
					break;
				}
				pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [Before blow check device] [END]", m_szCmdName, m_nStep );
			}

			// move z-axis
			CTaskTrayBasePP::eTeachPosZ eZPos = ( ePickPlace == DEF_PICK ) ? ( CTaskTrayBasePP::eZPOS_PICK ) : ( CTaskTrayBasePP::eZPOS_PLACE );
			int nErr = -1;
			if (ePickPlace == DEF_PICK) {
				double dTrayPP_Pick_1Stage_Offset = (double)g_DMCont.m_dmTrayPP.GN(NDM5_TM_LDTrayPP_Pick_1Stage_Offset);
				double dTPos = pTaskTrayLoadPP->GetTeachPos(eStage, Pocket, vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);

				std::vector<CPoint> vTargetPicker;
				vTargetPicker.clear();

				vTargetPicker = vPicker;

				if (pTaskTrayLoadPP->m_vFailPicker.size() != 0 && bVacuumCheckFuction == FALSE) {
					vTargetPicker = pTaskTrayLoadPP->m_vFailPicker;
				}
		
				nErr = pTaskTrayLoadPP->do_MovePos(eStage, Pocket, vTargetPicker, eZPos, CTaskTrayBasePP::eAxisZ, 100, dTPos - dTrayPP_Pick_1Stage_Offset);
//				nErr = pTaskTrayLoadPP->do_MovePos(eStage, Pocket, vPicker, eZPos, CTaskTrayBasePP::eAxisZ, 100, dTPos - dTrayPP_Pick_1Stage_Offset);
			}				
			else
				nErr = pTaskTrayLoadPP->do_MovePos(eStage, Pocket, vPicker, eZPos, CTaskTrayBasePP::eAxisZ);

			if( nErr != ERR_NO_ERROR ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Tray PP move z down %s pos cmd Error", ( ePickPlace == DEF_PICK ) ? ( _T( "Pick" ) ) : ( _T( "Place" ) ) );
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm( nErr, AlarmData );
				break;
			}

			pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [z move target %s pos] [SRT]", m_szCmdName, m_nStep, ( ePickPlace == DEF_PICK ) ? ( _T( "Pick" ) ) : ( _T( "Place" ) ) );
			BOOL bPreVacuum = g_DMCont.m_dmHandlerOpt.GB( BDM11_MD_TRAY_HAND_PRE_VACUUM );

			if( bPreVacuum == eOPTION_USE && ePickPlace == DEF_PICK ) {
				pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [Pre Vacuum On] [SRT]", m_szCmdName, m_nStep );
				int nErr = pTaskTrayLoadPP->do_VacBlow( eVacBlwSt_VacOn, vPicker );
				if( nErr != ERR_NO_ERROR ) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( "Tray PP vacuum cmd Error" );
					ReportAlarm( nErr, AlarmData );
					break;
				}
				pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [Pre Vacuum On] [END]", m_szCmdName, m_nStep );
			}

			m_tcTimeout = ( double )GetTickCount64();
			if (ePickPlace == DEF_PICK)
				m_nStep = move_z_target_soft_Pick_1stage_done;
			else
				m_nStep = checking_z_axis_done;
		}break;
	case move_z_target_soft_Pick_1stage_done:
	{
		double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
		double dCurPos = 0.0;
		pMotorZ->GetCurrentPos(dCurPos);
		if (dSpanTime > (MOTOR_TIME_OUT_MIDDLE * 1000.0)) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Load Tray PP z-axis %s pos is not complete", (ePickPlace == DEF_PICK) ? (_T("Pick")) : (_T("Place")));
			AlarmData.m_fnAddItemFormat("time over! (overtime=%.2f, z-axis current pos=%.2f)", dSpanTime / 1000.0, dCurPos);
			ReportAlarm(ERR_TRAY_PP_1_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData);
			break;
		}
		int nAxisDone = pMotorZ->AxisDone();
		if (nAxisDone == ERR_NO_ERROR) {
			m_nStep = move_z_target_soft_Pick_2stage;
			break;
		}
		else if (nAxisDone == -1) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Load Tray PP move z down done Error");
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
			ReportAlarm(ERR_TRAY_PP_1_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData);
		}
	}break;
	case move_z_target_soft_Pick_2stage:
	{
		int nCylinderWaitTm = g_DMCont.m_dmTrayPP.GN(NDM5_TM_LDTrayPP_CylDownWait);
		double dSpanTime = (double)GetTickCount64() - m_tcTimeoutPicker;
		int nRemainTime = nCylinderWaitTm - (int)dSpanTime;

		if (ePickPlace == DEF_PLACE && CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto)
		{
			pTaskTrayLoadPP->MakeLog("[%s EXEC=%8d] [Before blow check device] [STR]", m_szCmdName, m_nStep);
			CString strMsg = _T("");
			std::vector<CString> vErrString; vErrString.clear();
			int nChkErr = pTaskTrayLoadPP->CheckDevice(strMsg, vPicker, vErrString);
			if (nChkErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat(strMsg);
				ReportAlarm(nChkErr, AlarmData, pTaskTrayLoadPP->GetFailPicker());
				break;
			}
			pTaskTrayLoadPP->MakeLog("[%s EXEC=%8d] [Before blow check device] [END]", m_szCmdName, m_nStep);
		}

		// move z-axis
		CTaskTrayBasePP::eTeachPosZ eZPos = (ePickPlace == DEF_PICK) ? (CTaskTrayBasePP::eZPOS_PICK) : (CTaskTrayBasePP::eZPOS_PLACE);
		int nErr = -1;
		if (ePickPlace == DEF_PICK) {
			std::vector<CPoint> vTargetPicker;
			vTargetPicker.clear();
			if (pTaskTrayLoadPP->GetEqpCtrlMode() == eEqpCtrlMode_Manual) {
				vTargetPicker = vPicker;
			}
			else {
				if (pTaskTrayLoadPP->m_vFailPicker.size() != 0 && bVacuumCheckFuction == FALSE) {
					vTargetPicker = pTaskTrayLoadPP->m_vFailPicker;
				}
				else {
					vTargetPicker = vPicker;
				}
			}



			double dTrayPP_Pick_1Stage_Speed = (double)g_DMCont.m_dmTrayPP.GN(NDM5_TM_LDTrayPP_Pick_1Stage_Speed)/1000;
			nErr = pTaskTrayLoadPP->do_MovePos(eStage, Pocket, vTargetPicker, eZPos, CTaskTrayBasePP::eAxisZ, dTrayPP_Pick_1Stage_Speed);
//			nErr = pTaskTrayLoadPP->do_MovePos(eStage, Pocket, vPicker, eZPos, CTaskTrayBasePP::eAxisZ, dTrayPP_Pick_1Stage_Speed);
		}
		else
			nErr = pTaskTrayLoadPP->do_MovePos(eStage, Pocket, vPicker, eZPos, CTaskTrayBasePP::eAxisZ);

		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tray PP move z down %s pos cmd Error", (ePickPlace == DEF_PICK) ? (_T("Pick")) : (_T("Place")));
			OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
			AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
			ReportAlarm(nErr, AlarmData);
			break;
		}

		pTaskTrayLoadPP->MakeLog("[%s EXEC=%8d] [z move target %s pos] [SRT]", m_szCmdName, m_nStep, (ePickPlace == DEF_PICK) ? (_T("Pick")) : (_T("Place")));
		BOOL bPreVacuum = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TRAY_HAND_PRE_VACUUM);

		if (bPreVacuum == eOPTION_USE && ePickPlace == DEF_PICK) {
			pTaskTrayLoadPP->MakeLog("[%s EXEC=%8d] [Pre Vacuum On] [SRT]", m_szCmdName, m_nStep);
			int nErr = pTaskTrayLoadPP->do_VacBlow(eVacBlwSt_VacOn, vPicker);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Tray PP vacuum cmd Error");
				ReportAlarm(nErr, AlarmData);
				break;
			}
			pTaskTrayLoadPP->MakeLog("[%s EXEC=%8d] [Pre Vacuum On] [END]", m_szCmdName, m_nStep);
		}

		m_tcTimeout = (double)GetTickCount64();
		m_nStep = checking_z_axis_done;
	}break;
	case checking_z_axis_done : // checking z axis done
		{
			double dSpanTime = ( double )GetTickCount64() - m_tcTimeout;
			double dCurPos = 0.0;
			pMotorZ->GetCurrentPos(dCurPos);
			if( dSpanTime > ( MOTOR_TIME_OUT_MIDDLE * 1000.0 ) ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Load Tray PP z-axis %s pos is not complete", ( ePickPlace == DEF_PICK ) ? ( _T( "Pick" ) ) : ( _T( "Place" ) ) );
				AlarmData.m_fnAddItemFormat( "time over! (overtime=%.2f, z-axis current pos=%.2f)", dSpanTime / 1000.0, dCurPos);
				ReportAlarm(ERR_TRAY_PP_1_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData );
				break;
			}
			int nAxisDone = pMotorZ->AxisDone();
			if( nAxisDone == ERR_NO_ERROR ) {
				BOOL bPreVacuum = g_DMCont.m_dmHandlerOpt.GB( BDM11_MD_TRAY_HAND_PRE_VACUUM );

				pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [z move target %s pos] [END]", m_szCmdName, m_nStep, ( ePickPlace == DEF_PICK ) ? ( _T( "Pick" ) ) : ( _T( "Place" ) ) );
				if( g_StepTrayPP_SW[eTrayPP_1_LD].GetStatus() == DEF_ON ) {

					if( bPreVacuum == eOPTION_USE && ePickPlace == DEF_PICK ) {
						pTaskTrayLoadPP->do_VacBlow( eVacBlwSt_BlwOn, vPicker );
					}

					pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [STEP Working Stop, target %s pos]", m_szCmdName, m_nStep, ( ePickPlace == DEF_PICK ) ? ( _T( "Pick" ) ) : ( _T( "Place" ) ) );
					g_StepTrayPP_SW[eTrayPP_1_LD].SetChangeStatusOff();
					g_StepTrayPP_SW[eTrayPP_1_LD].SetChangeStatusOnOff( DEF_ON );
					SetCmdStatus( eSeqCmdStatus_Alarm );
					g_TaskSystemCtrl.OnLotCommand( eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto );
				}
				else {
					m_nStep = ( ePickPlace == DEF_PLACE ? do_blow : pick_vac );
					break;
				}
			}
			else if( nAxisDone == -1 ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Tray PP move z down done Error" );
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				ReportAlarm( ERR_TRAY_PP_1_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData );
			}
		}break;

		//------------------------------------------------------------------------------------------
		// place blow loop
	case do_blow:
		{
			//Place Pos 인대, Z축 Safety Sensor 감지 되면 내려가지 않은 것으로 판단하고 알람 발생 한다.
			int nErrSen = pTaskTrayLoadPP->Chk_Z_Safety_Sen_Check_Type(vPicker, ePickerSafetySensorCheckType::ePlaceSafetySensorCheck);
			if (!pTaskTrayLoadPP->m_vSafetyFailPicker[ePickerSafetySensorCheckType::ePlaceSafetySensorCheck].empty())
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Tray Load PP Place Pos Safety Sensing Error %s", pTaskTrayLoadPP->m_sSaftyFailPicker[ePickerSafetySensorCheckType::ePlaceSafetySensorCheck]);
				ReportAlarm(ERR_TRAY_PP_1_Z_PICKER_PLACE_POS_SAFETY_SENSING, AlarmData, pTaskTrayLoadPP->GetSafetyFailPicker(ePickerSafetySensorCheckType::ePlaceSafetySensorCheck));
				break;
			}

			pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [Blow On] [SRT]", m_szCmdName, m_nStep );
			int nErr = pTaskTrayLoadPP->do_VacBlow( eVacBlwSt_BlwOn, vPicker );
			pTaskTrayLoadPP->m_bCyclePause = TRUE;
			pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [Blow On] [END]", m_szCmdName, m_nStep );
			if( nErr != ERR_NO_ERROR ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Tray PP Blow cmd Error" );
				ReportAlarm( nErr, AlarmData );
				break;
			}

//			pTaskTrayLoadPP->MakeLog("[%s EXEC=%8d] [Blow Wait] [STR]", m_szCmdName, m_nStep);
// 			int nSleep = g_DMCont.m_dmTrayPP.GN(NDM5_TM_LDTrayPP_Blow);
// 			if (nSleep > 0) {
// 				Sleep(nSleep);
// 			}
//			pTaskTrayLoadPP->MakeLog("[%s EXEC=%8d] [Blow Wait] [END]", m_szCmdName, m_nStep);

			// write device data
			if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto) {
				CString strPicker = _T(""), strTmp = _T("");
				pTaskTrayLoadPP->m_vFailPicker.clear();
				for (std::vector<CPoint>::size_type i = 0; i < (int)vPicker.size(); i++) {
					strTmp.Format("[X:%d, Y:%d]", vPicker[i].x + 1, vPicker[i].y + 1);
					if (pTaskTrayLoadPP->ChkVacuum(eOnOffState::eOff, vPicker[i]) != ERR_NO_ERROR) {
						strPicker += strTmp;
						pTaskTrayLoadPP->m_vFailPicker.push_back(vPicker[i]);
					}
				}

				if (pTaskTrayLoadPP->m_vFailPicker.size() > 0) {
					if (m_nRetryCount < m_nSetRetryCount)
					{
						Sleep(500.0);
						m_nRetryCount++;
						pTaskTrayLoadPP->MakeLog("[Blow Retry] CmdStage=%s, CmdTargetX=%d, CmdTargetY=%d, %s", pTaskTrayLoadPP->GetLocName(eStage), Pocket.x + 1, Pocket.y + 1, strPicker);
						m_nStep = do_blow;
						break;
					}
				}
				std::vector<CString> vErrString; vErrString.clear();
				int nRet = pTaskTrayLoadPP->ProcDeviceData(eStage, Pocket.x, Pocket.y, vPicker, ePickPlace, vErrString);

				pTaskTrayLoadPP->m_bCyclePause = FALSE;

				strPicker = _T( "" ), strTmp = _T( "" );
				for( std::vector<CPoint>::size_type i = 0; i < ( int )vPicker.size(); i++ ) {
					strTmp.Format( "[X:%d, Y:%d]", vPicker[ i ].x + 1, vPicker[ i ].y + 1 );
					strPicker += strTmp;
				}
				pTaskTrayLoadPP->MakeLog( "[ProcDevice Data Place] CmdStage=%s, CmdTargetX=%d, CmdTargetY=%d, %s", pTaskTrayLoadPP->GetLocName( eStage ), Pocket.x + 1, Pocket.y + 1, strPicker );

				if( nRet != ERR_NO_ERROR )
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( "place fail, Pocket[x:%d,y:%d]", Pocket.x + 1, Pocket.y + 1 );
					ReportAlarm( nRet, AlarmData, pTaskTrayLoadPP->GetFailPicker() );
					break;
				}
			}
			m_nRetryCount = 0;
			m_nLast_Executing_Step = m_nStep;
			m_nStep = move_z_safety;
		}break;

		//------------------------------------------------------------------------------------------
		// pick vac loop
	case pick_vac :
		{
			BOOL bPreVacuum = g_DMCont.m_dmHandlerOpt.GB( BDM11_MD_TRAY_HAND_PRE_VACUUM );

			if( bPreVacuum != eOPTION_USE ) {
				pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [Vacuum On] [SRT]", m_szCmdName, m_nStep );
				int nErr = pTaskTrayLoadPP->do_VacBlow( eVacBlwSt_VacOn, vPicker );
				if( nErr != ERR_NO_ERROR ) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( "Tray PP vacuum cmd Error" );
					ReportAlarm( nErr, AlarmData );
					break;
				}
				pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [Vacuum On] [END]", m_szCmdName, m_nStep );
			}
			pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [Vacuum Wait] [STR]", m_szCmdName, m_nStep );
			int nSleep = g_DMCont.m_dmTrayPP.GN( NDM5_TM_LDTrayPP_Vacuum );
			if( nSleep > 0 ) {
				Sleep( nSleep );
			}
			pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [Vacuum Wait] [END]", m_szCmdName, m_nStep );

			m_nStep = move_z_safety;
		}break;

		//------------------------------------------------------------------------------------------
	case move_z_safety : // move z safety & picker Up
		{
			pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [Move Safety and Picker up] [SRT]", m_szCmdName, m_nStep );
			std::vector<CPoint> vPkr;
			pTaskTrayLoadPP->MakeAllPicker(vPkr);
			int nErr = pTaskTrayLoadPP->do_MovePos( eStage, Pocket, vPkr, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ );
			if( nErr != ERR_NO_ERROR ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Tray PP z move safety cmd Error" );
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm( nErr, AlarmData );
				break;
			}
			m_tcTimeout = ( double )GetTickCount64();
			m_nStep = check_z_axis_done;
		}break;
	case check_z_axis_done : //check z axis done
		{

			std::vector<CPoint> vPkr;
			pTaskTrayLoadPP->MakeAllPicker(vPkr);
			m_vErrPicker.clear();
			int nErrSen = pTaskTrayLoadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);

			int nAxisDone = pMotorZ->AxisDone(vPkr, m_vErrPicker);
			if( nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR)							
			{
				m_nStep = check_vacuum_fail;
				m_tcTimeoutPicker = ( double )GetTickCount64();
				break;
			}
			
			std::vector<CPoint> vErrPkr; vErrPkr.clear();
			CString sAlarmData = _T("");
			int nErr = pTaskTrayLoadPP->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, pTaskTrayLoadPP->GetSafetyFailPicker(ePickerSafetySensorCheckType::eSafetySensorCheck), vErrPkr, sAlarmData);

			double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
			if (dSpanTime > (TIME_OUT_SHORT * 1000.0)) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("%s", sAlarmData);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				ReportAlarm(nErr, AlarmData, vErrPkr);
				break;
			}
		}break;
	case check_vacuum_fail:
	{
		if (eStage == LOC_IDX_TRAY_PP_FEEDER && ePickPlace == DEF_PICK)
		{
			CString strPicker = _T(""), strTmp = _T("");
			pTaskTrayLoadPP->m_vFailPicker.clear();
			for (std::vector<CPoint>::size_type i = 0; i < (int)vPicker.size(); i++) {
				strTmp.Format("[X:%d, Y:%d]", vPicker[i].x + 1, vPicker[i].y + 1);
				if (pTaskTrayLoadPP->ChkVacuum(eOnOffState::eOn, vPicker[i]) != ERR_NO_ERROR) {
					strPicker += strTmp;
					pTaskTrayLoadPP->m_vFailPicker.push_back(vPicker[i]);
				}
			}

			if (pTaskTrayLoadPP->m_vFailPicker.size() > 0) {
				if (m_nRetryCount >= PICKER_FAIL_RETRY_CNT) {
					m_nRetryCount = 0;
					m_nStep = picker_up_check_and_write_device_data;
					break;
				}

				m_nRetryCount++;
				m_nStep = move_z_target_pos;
				pTaskTrayLoadPP->MakeLog("[Vacuum Fail Move Again] CmdStage=%s, CmdTargetX=%d, CmdTargetY=%d, %s", pTaskTrayLoadPP->GetLocName(eStage), Pocket.x + 1, Pocket.y + 1, strPicker);
				break;
			}
			else {
				m_nStep = picker_up_check_and_write_device_data;
				break;
			}
		}
		else {
			if(bVacuumCheckFuction == TRUE)
				m_nStep = picker_up_check_and_write_device_data_table_vacuum_check;
			else
				m_nStep = picker_up_check_and_write_device_data;
			break;
		}
	}break;
		//------------------------------------------------------------------------------------------
	case picker_up_check_and_write_device_data : // picker up check and write device data
		{
			double dSpanTime = ( double )GetTickCount64() - m_tcTimeoutPicker;
			if( dSpanTime > TIME_OUT_SHORT * 1000.0 ) {
				if( m_nRetryCount >= m_nSetRetryCount ) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( "Tray PP picker down operation is not complete, retry count(%d) time over!", m_nSetRetryCount );
					ReportAlarm( ERR_TRAY_PP_1_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData, pTaskTrayLoadPP->m_vFailPicker);
					break;
				}
				else {
					m_nRetryCount++;
					m_nStep = move_z_safety;
					break;
				}
			}

			eStacker_Type stacker_type = ( eStacker_Type )-1;
			if( LOC_IDX_TRAY_PP_CONV_3 == eStage )
				stacker_type = g_TaskAutoStacker[ eStage - LOC_IDX_TRAY_PP_CONV_1 ].GetStackerType();
			if (LOC_IDX_TRAY_PP_FEEDER == eStage)
				stacker_type = eStackerType_Load;

			int nErr = pTaskTrayLoadPP->Chk_Z_Safety_Sen_All();
			if( nErr == ERR_NO_ERROR ) {
				pTaskTrayLoadPP->MakeLog( "[%s EXEC=%8d] [Move Safety and Picker up] [END]", m_szCmdName, m_nStep );
				// =================================================================================================
				// Auto Skip Use Mode & Target Stage Loading Setplate
				// Need Auto Skip Erase pick fail pocket data
				// =================================================================================================
				if( g_DMCont.m_dmHandlerOpt.GB( BDM11_MD_AutoSkipUse ) == TRUE && g_DMCont.m_dmHandlerOpt.GB( BDM11_MD_AutoGrrUse ) == FALSE
					&& ePickPlace == DEF_PICK && CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto
					&& ( stacker_type == eStackerType_Load ) ) {

					CString strPicker = _T( "" ), strTmp = _T( "" );
					int nNotVacCount = 0;
					pTaskTrayLoadPP->m_vFailPicker.clear();
					for( std::vector<CPoint>::size_type i = 0; i < ( int )vPicker.size(); i++ ) {
						strTmp.Format( "[X:%d, Y:%d]", vPicker[ i ].x + 1, vPicker[ i ].y + 1 );
						if( pTaskTrayLoadPP->ChkVacuum( eOnOffState::eOn, vPicker[ i ] ) != ERR_NO_ERROR ) {
							strPicker += strTmp;
							nNotVacCount++;
							pTaskTrayLoadPP->m_vFailPicker.push_back( vPicker[ i ] );
						}
					}
					//TRACE("Pick Pocket : X: %d  Y: %d ", Pocket.x, Pocket.y);
					pTaskTrayLoadPP->AddLoadTrayPickFailCnt( nNotVacCount );

					BOOL bChkIsOverFail = pTaskTrayLoadPP->ChkLoadTrayPickFailCntIsOver();
					if( bChkIsOverFail == FALSE || ( Pocket.x == 0 && Pocket.y == 0 ) ) { // auto skip device count 이하 or 0
						if( nNotVacCount != ( int )vPicker.size() || ( Pocket.x == 0 && Pocket.y == 0 ) ) {
							pTaskTrayLoadPP->ResetLoadTrayPickFailCnt();
						}

						std::vector<CString> vErrString; vErrString.clear();
						pTaskTrayLoadPP->MakeLog( "[ProcDevice Data AutoSkip Pick] CmdStage=%s, CmdTargetX=%d, CmdTargetY=%d, %s", pTaskTrayLoadPP->GetLocName( eStage ), Pocket.x + 1, Pocket.y + 1, strPicker );
						int nRet = pTaskTrayLoadPP->ProcDeviceData( eStage, Pocket.x, Pocket.y, vPicker, ePickPlace, vErrString, eDeviceData_EraseAndAutoSkip );
						
						if( nRet != ERR_NO_ERROR )
						{
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat( "auto skip fail, Pocket[x:%d,y:%d] use hand [%s]", Pocket.x + 1, Pocket.y + 1, strPicker );
							ReportAlarm( nRet, AlarmData, pTaskTrayLoadPP->GetFailPicker() );
							break;
						}
					}
					else { // auto skip device count over
						pTaskTrayLoadPP->ResetLoadTrayPickFailCnt();
						CParamList AlarmData;
						//AlarmData.m_fnAddItemFormat( "auto skip fail count over!! please check it!" );
						AlarmData.m_fnAddItemFormat( "auto skip fail, count over!! please check it! CmdStage=%s, Pocket[x:%d,y:%d] use hand [%s]", pTaskTrayLoadPP->GetLocName( eStage ), Pocket.x + 1, Pocket.y + 1, strPicker );
						ReportAlarm( ERR_TRAY_PP_1_DEVICE_AUTO_SKIP_PICK_FAIL_OVER, AlarmData, pTaskTrayLoadPP->GetFailPicker() );
						break;
					}
				}
				// =================================================================================================
				// of normal case
				// =================================================================================================
				else {
					if( CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto && ePickPlace == DEF_PICK )
					{
						///////					
//    						CParamList AlarmData;
//    						ReportAlarm(ERR_TRAY_PP_1_DEVICE_AUTO_SKIP_PICK_FAIL_OVER, AlarmData, pTaskTrayLoadPP->GetFailPicker());
//    						break;
						///////


						CString strPicker = _T( "" ), strTmp = _T( "" ), strPocket = _T( "" );
						int nPocketX = 0, nPocketY = 0;
						//pTaskTrayPP->m_vFailPicker.clear();
						std::vector<CString> vErrString; vErrString.clear();
						int nRet = pTaskTrayLoadPP->ProcDeviceData( eStage, Pocket.x, Pocket.y, vPicker, ePickPlace, vErrString);
						for( std::vector<CPoint>::size_type i = 0; i < vPicker.size(); i++ ) {
							strTmp.Format( "[X:%d, Y:%d]", vPicker[ i ].x + 1, vPicker[ i ].y + 1 );
							strPicker += strTmp;
						}

						if( ( int )pTaskTrayLoadPP->m_vFailPicker.size() > 0 ) {
							typedef std::vector<CPoint>::const_iterator vIterPoint;
							std::pair<vIterPoint, vIterPoint> minX1 = std::minmax_element( vPicker.begin(), vPicker.end(), MinX );
							std::pair<vIterPoint, vIterPoint> minY1 = std::minmax_element( vPicker.begin(), vPicker.end(), MinY );

							for( std::vector<CPoint>::size_type i = 0; i < vPicker.size(); i++ ) {
								for( std::vector<CPoint>::size_type j = 0; j < ( int )pTaskTrayLoadPP->m_vFailPicker.size(); j++ ) {
									if( vPicker[ i ].x == pTaskTrayLoadPP->m_vFailPicker[ j ].x && vPicker[ i ].y == pTaskTrayLoadPP->m_vFailPicker[ j ].y ) {
										nPocketX = Pocket.x + ( pTaskTrayLoadPP->m_vFailPicker[ j ].x - minX1.first->x ) + 1;
										nPocketY = Pocket.y + ( pTaskTrayLoadPP->m_vFailPicker[ j ].y - minY1.first->y ) + 1;
										strTmp.Format( "[X:%d, Y:%d]", nPocketX, nPocketY );
										strPocket += strTmp;
									}
								}
							}
						}

						pTaskTrayLoadPP->MakeLog( "[ProcDevice Data Pick] CmdStage=%s, CmdTargetX=%d, CmdTargetY=%d, %s", pTaskTrayLoadPP->GetLocName( eStage ), Pocket.x + 1, Pocket.y + 1, strPicker );
						if( nRet != ERR_NO_ERROR )
						{
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat( "%s Picker Pick ERROR Pocket %s", strPicker, strPocket );
							if( stacker_type == eStackerType_Load ) {
								nRet = ERR_TRAY_PP_1_DEVICE_PICK_FAIL_NO_JAM;
							}

							ReportAlarm( nRet, AlarmData, pTaskTrayLoadPP->GetFailPicker() );
							break;
						}
					}
				}
				//m_nStep = 25000;

				if( vSpecOutPicker.size() > 0 && bSpeckOutPkrEnd == FALSE && ePickPlace == DEF_PLACE && bSpecOutPkrUse == TRUE )
				{
					if( nCntSpecOut == vSpecOutPicker.size() ) {
						bSpeckOutPkrEnd = TRUE;
					}

					if( bSpeckOutPkrEnd == FALSE )
					{
						nCntSpecOut++;
						bNormalPkrEnd = TRUE;
						//m_nStep = 10000;   //offset 넣을려면 더 뒤로 이동.

						//m_nStep = 3001;		//Xpitch, Ypitch offset 추가 step
						//break;
					}
				}

				ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_NormalComplete );
			}
		}break;
		case picker_up_check_and_write_device_data_table_vacuum_check: // picker up check and write device data table vacuum check
		{
			double dSpanTime = (double)GetTickCount64() - m_tcTimeoutPicker;
			if (dSpanTime > TIME_OUT_SHORT * 1000.0) {
				if (m_nRetryCount >= m_nSetRetryCount) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Tray PP picker down operation is not complete, retry count(%d) time over!", m_nSetRetryCount);
					ReportAlarm(ERR_TRAY_PP_1_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData, pTaskTrayLoadPP->m_vFailPicker);
					break;
				}
				else {
					m_nRetryCount++;
					m_nStep = move_z_safety;
					break;
				}
			}

			int nErr = pTaskTrayLoadPP->Chk_Z_Safety_Sen_All();
			if (nErr == ERR_NO_ERROR) {
				pTaskTrayLoadPP->MakeLog("[%s EXEC=%8d] [Move Safety and Picker up] [END]", m_szCmdName, m_nStep);
				if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto && ePickPlace == DEF_PICK)
				{
					///////					
					//    						CParamList AlarmData;
					//    						ReportAlarm(ERR_TRAY_PP_1_DEVICE_AUTO_SKIP_PICK_FAIL_OVER, AlarmData, pTaskTrayLoadPP->GetFailPicker());
					//    						break;
					///////


					CString strPicker = _T(""), strTmp = _T(""), strPocket = _T("");
					int nPocketX = 0, nPocketY = 0;
					//pTaskTrayPP->m_vFailPicker.clear();

					int nRet = pTaskTrayLoadPP->ProcVacuumCheckMem(eStage, Pocket.x, Pocket.y, vPicker, ePickPlace);
// 					for (std::vector<CPoint>::size_type i = 0; i < vPicker.size(); i++) {
// 						strTmp.Format("[X:%d, Y:%d]", vPicker[i].x + 1, vPicker[i].y + 1);
// 						strPicker += strTmp;
// 					}
					for (std::vector<CPoint>::size_type i = 0; i < (int)pTaskTrayLoadPP->m_vFailPicker.size(); i++) {
						strTmp.Format("[X:%d, Y:%d]", pTaskTrayLoadPP->m_vFailPicker[i].x + 1, pTaskTrayLoadPP->m_vFailPicker[i].y + 1);
						strPicker += strTmp;
					}

					if ((int)pTaskTrayLoadPP->m_vFailPicker.size() > 0) {
						typedef std::vector<CPoint>::const_iterator vIterPoint;
						std::pair<vIterPoint, vIterPoint> minX1 = std::minmax_element(vPicker.begin(), vPicker.end(), MinX);
						std::pair<vIterPoint, vIterPoint> minY1 = std::minmax_element(vPicker.begin(), vPicker.end(), MinY);

						for (std::vector<CPoint>::size_type i = 0; i < vPicker.size(); i++) {
							for (std::vector<CPoint>::size_type j = 0; j < (int)pTaskTrayLoadPP->m_vFailPicker.size(); j++) {
								if (vPicker[i].x == pTaskTrayLoadPP->m_vFailPicker[j].x && vPicker[i].y == pTaskTrayLoadPP->m_vFailPicker[j].y) {
									nPocketX = Pocket.x + (pTaskTrayLoadPP->m_vFailPicker[j].x - minX1.first->x) + 1;
									nPocketY = Pocket.y + (pTaskTrayLoadPP->m_vFailPicker[j].y - minY1.first->y) + 1;
									strTmp.Format("[X:%d, Y:%d]", nPocketX, nPocketY);
									strPocket += strTmp;
								}
							}
						}
					}

					pTaskTrayLoadPP->MakeLog("[ProcVacuumCheckMem] CmdStage=%s, CmdTargetX=%d, CmdTargetY=%d, %s", pTaskTrayLoadPP->GetLocName(eStage), Pocket.x + 1, Pocket.y + 1, strPicker);
					if (nRet != ERR_NO_ERROR)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Picker Pick Sensing %s", strPicker);
						ReportAlarm(nRet, AlarmData, pTaskTrayLoadPP->GetFailPicker());
						break;
					}
				}
				
				//m_nStep = 25000;

				if (vSpecOutPicker.size() > 0 && bSpeckOutPkrEnd == FALSE && ePickPlace == DEF_PLACE && bSpecOutPkrUse == TRUE)
				{
					if (nCntSpecOut == vSpecOutPicker.size()) {
						bSpeckOutPkrEnd = TRUE;
					}

					if (bSpeckOutPkrEnd == FALSE)
					{
						nCntSpecOut++;
						bNormalPkrEnd = TRUE;
						//m_nStep = 10000;   //offset 넣을려면 더 뒤로 이동.

						//m_nStep = 3001;		//Xpitch, Ypitch offset 추가 step
						//break;
					}
				}

				ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
			}
		}break;
	}
	return CBaseSeqCmd::State_Executing();
}

BOOL CTrayLoadPPCmd_MovePickPlace::State_Pause()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = ( CTaskTrayLoadPP* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTaskTrayLoadPP->MakeLog( "[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			int nAxisCount = pTaskTrayLoadPP->GetMotorCount();
			for( int i = 0; i < nAxisCount; i++ ) {
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis( static_cast< CTaskTrayBasePP::eMotor >( i ) );
				if( pMotor == nullptr ) {
					continue;
				}

				bool bCloseLimit = pMotor->CheckLimitClose();
				double dCurPos = 0.0;
				pMotor->GetCurrentPos(dCurPos);
				if( bCloseLimit == false )
				{
					pTaskTrayLoadPP->MakeLog( "[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0 );
					pMotor->SetSlowStop();
				}
			}

			//SetSlowStop 전, Device Data 처리.
			if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto && pTaskTrayLoadPP->m_bCyclePause == TRUE)
			{
				int nBlowDoneCnt = 0;
				int nHandData = 0;
				for (int i = 0; i < (int)vPicker.size(); i++) {
					//Blow Done Count Check.
					if (pTaskTrayLoadPP->ChkVacuum(eOnOffState::eOff, vPicker[i]) == ERR_NO_ERROR) { //Blow Retry						
						nBlowDoneCnt++;
					}

					//Hand Device Data Check.
					//ST_DD_DEVICE stHandData;
					eExistState eHandData;
					pTaskTrayLoadPP->GetDeviceDataExist(vPicker[i], &eHandData);
					if (eHandData == eExist) {
						nHandData++;
					}
				}

				//1. Place일 경우.
				//2. Blow 진행 했을 경우.
				//3. Hand Device Data 존재 할 경우.
				if (ePickPlace == DEF_PLACE && nBlowDoneCnt == vPicker.size() && nHandData == vPicker.size())
				{
					std::vector<CString> vErrString; vErrString.clear();
					int nRet = pTaskTrayLoadPP->ProcDeviceData(eStage, Pocket.x, Pocket.y, vPicker, ePickPlace, vErrString);
					pTaskTrayLoadPP->m_bCyclePause = FALSE;
					CString strPicker = _T("");
					CString strTmp = _T("");
					for (std::vector<CPoint>::size_type i = 0; i < (int)vPicker.size(); i++) {
						strTmp.Format("[X:%d, Y:%d]", vPicker[i].x + 1, vPicker[i].y + 1);
						strPicker += strTmp;
					}
					pTaskTrayLoadPP->MakeLog("[Before Stop][ProcDevice Data Place] CmdStage=%s, CmdTargetX=%d, CmdTargetY=%d, %s", pTaskTrayLoadPP->GetLocName(eStage), Pocket.x + 1, Pocket.y + 1, strPicker);

					if (nRet != ERR_NO_ERROR)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("place fail, Pocket[x:%d,y:%d]", Pocket.x + 1, Pocket.y + 1);
						ReportAlarm(nRet, AlarmData, pTaskTrayLoadPP->GetFailPicker());
						break;
					}

					m_nLast_Executing_Step = move_z_safety;
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

BOOL CTrayLoadPPCmd_MovePickPlace::State_Recovery()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = ( CTaskTrayLoadPP* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTaskTrayLoadPP->MakeLog( "[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step );
	}
	m_nStep_Pre = m_nStep;


	switch( m_nStep )
	{
	case 0: // move z axis safety & picker all up <start>
		{
			std::vector<CPoint> vPkr;
			pTaskTrayLoadPP->MakeAllPicker(vPkr);
			pTaskTrayLoadPP->MakeLog( "[%s RECOVERY=%8d] [z move safety & picker all up] [SRT]", m_szCmdName, m_nStep );
			int nErr = pTaskTrayLoadPP->do_MovePos( eStage, Pocket, vPkr, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ );
			if( nErr != ERR_NO_ERROR ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Tray PP z axis move safety position command error" );
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm( nErr, AlarmData );
				break;
			}

			m_tcTimeout = ( double )GetTickCount64();	//check time picker up
			m_nStep = 1100;
		}break;

	case 1100:	// move z axis safety & picker all up <end>
		{
	
			std::vector<CPoint> vPkr;
			pTaskTrayLoadPP->MakeAllPicker(vPkr);
			int nErrSen = pTaskTrayLoadPP->Chk_Z_Safety_Sen_Check_Type(vPkr, ePickerSafetySensorCheckType::eSafetySensorCheck);
			OneAxis* pMotorZ = pTaskTrayLoadPP->GetAxis( CTaskTrayBasePP::eAxisZ );
			m_vErrPicker.clear();
			int nAxisDone = pMotorZ->AxisDone(vPkr, m_vErrPicker);
			if( nAxisDone == ERR_NO_ERROR && nErrSen == ERR_NO_ERROR ) {
				pTaskTrayLoadPP->MakeLog( "[%s RECOVERY=%8d] [z move safety & picker all up] [END]", m_szCmdName, m_nStep );
				m_nStep = 1200;
				break;
			}

			std::vector<CPoint> vErrPkr; vErrPkr.clear();
			CString sAlarmData = _T("");
			int nErr = pTaskTrayLoadPP->GetPickerErrorCase(nAxisDone, nErrSen, m_vErrPicker, pTaskTrayLoadPP->GetSafetyFailPicker(ePickerSafetySensorCheckType::eSafetySensorCheck), vErrPkr, sAlarmData);

			double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
			if (dSpanTime > (TIME_OUT_SHORT * 1000.0)) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("%s", sAlarmData);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				ReportAlarm(nErr, AlarmData, vErrPkr);
				break;
			}

		}break;
	case 1200:	// x move safety <start>
		{
			std::vector<CPoint> vPicker;
			vPicker.push_back( CPoint( 0, 0 ) );

			int nErr = pTaskTrayLoadPP->do_MovePos( LOC_IDX_TRAY_PP_SAFETY, CPoint( 0, 0 ), vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX );
			if( nErr != ERR_NO_ERROR ) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisX);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm( nErr, AlarmData );
				break;
			}
			m_tcTimeout = ( double )GetTickCount64();
			m_nStep = 1201;
		}break;
	case 1201:	// y move safety <end>
		{
			OneAxis* pMotorX = pTaskTrayLoadPP->GetAxis( CTaskTrayBasePP::eAxisX );

			double dSpanTime = ( double )GetTickCount64() - m_tcTimeout;
			if( dSpanTime > ( MOTOR_TIME_OUT_MIDDLE * 1000.0 ) ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Load Tray PP X axis move x safety time over" );
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorX->GetAxisName());
				ReportAlarm( ERR_TRAY_PP_1_MOTOR_MOVE_TIMEOUT, AlarmData );
				break;
			}

			int nAxisDone = pMotorX->AxisDone();
			if( nAxisDone == ERR_NO_ERROR ) {
				//SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_NormalComplete );
				m_nStep = 5000;
				break;
			}
			else if( nAxisDone == -1 ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Tray PP x axis axis done fail" );
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorX->GetAxisName());
				ReportAlarm( ERR_TRAY_PP_1_MOTOR_MOVE_FAIL, AlarmData );
				break;
			}
		}break;
	case 5000:
		{
			if (eStage == LOC_IDX_TRAY_PP_FEEDER &&  ePickPlace == DEF_PICK)
			{
				CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();			
				if (feeder_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready)
				{
					break;
				}
			}
			m_nStep = 6000;

		}break;
	case 6000:
		{
			if (bVacuumCheckFuction == TRUE)
			{
				m_nStep = 99999;
				break;
			}

			CString error_msg = _T( "" );
			eCheckAbnormalCase _case_ = check_abnormal_case( ePickPlace, error_msg );
			switch( _case_ )
			{
			case invaild_case:
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( "TrayPPCmd_MovePickPlace::State_Recovery invaild case" );
					ReportAlarm( ERR_UNKNOWN, AlarmData);
				}break;
			case cmd_end:
			case abnormal:
				{
					// z up 하고 normal complete
					SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_NormalComplete );
					break;
				}break;
			case alarm_drop:
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( "Device Drop %s", error_msg );
					ReportAlarm( ERR_TRAY_PP_1_VAC_SEN_ERR, AlarmData);
				}break;
			case alarm_device_exist:
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( "Device Remaind %s", error_msg );
					ReportAlarm( ERR_TRAY_PP_1_VAC_SEN_ERR, AlarmData);
				}break;
			case situation:
				{
					switch( ePickPlace )
					{
					case ePPCmd::ePick:
						{
							CString strPicker = _T( "" ), strTmp = _T( "" );
							bool bAllVacuumOn = true;
							for each( auto picker in vPicker ) {
								strTmp.Format( "[X:%d, Y:%d]", picker.x + 1, picker.y + 1 );
								strPicker += strTmp;

								int nVacuumOnOff = pTaskTrayLoadPP->GetVacuum( picker );
								bAllVacuumOn &= ( nVacuumOnOff == DEF_ON );
							}
							if( bAllVacuumOn == false ) {
								// 처음부터 다시 시작.
								m_nStep = 99999;
								break;
							}
							else {
								std::vector<CString> vErrString; vErrString.clear();
								int nRet = pTaskTrayLoadPP->ProcDeviceData( eStage, Pocket.x, Pocket.y, vPicker, ePickPlace, vErrString);
								if( nRet != ERR_NO_ERROR )
								{
									CParamList AlarmData;
									AlarmData.m_fnAddItemFormat( "ProcDeviceData fail, Pocket[x:%d,y:%d] use hand [%s]", Pocket.x + 1, Pocket.y + 1, strPicker );
									ReportAlarm( nRet, AlarmData);
									break;
								}
								SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_NormalComplete );
								break;
							}
						}break;
					case ePPCmd::ePlace:
						{
							if( m_nLast_Executing_Step > do_blow ) {
								CString strPicker = _T( "" ), strTmp = _T( "" );
								for each( auto picker in vPicker ) {
									strTmp.Format( "[X:%d, Y:%d]", picker.x + 1, picker.y + 1 );
									strPicker += strTmp;
								}
								std::vector<CString> vErrString; vErrString.clear();
								int nRet = pTaskTrayLoadPP->ProcDeviceData( eStage, Pocket.x, Pocket.y, vPicker, ePickPlace, vErrString);
								if( nRet != ERR_NO_ERROR )
								{
									CParamList AlarmData;
									AlarmData.m_fnAddItemFormat( "ProcDeviceData fail, Pocket[x:%d,y:%d] use hand [%s]", Pocket.x + 1, Pocket.y + 1, strPicker );
									ReportAlarm( nRet, AlarmData );
									break;
								}
								SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_NormalComplete );
								break;
							}
							else {
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat( "Device Drop %s", error_msg );
								ReportAlarm( ERR_TRAY_PP_1_VAC_SEN_ERR, AlarmData);
								break;
							}
						}break;
					default:
						ASSERT( false );
						break;
					}
				}break;
			case normal_start:
				{
					// 처음부터 다시 시작.
					m_nStep = 99999;
				}break;
			default:
				ASSERT( false );
				break;
			}
		}break;

	case 99999:
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

BOOL CTrayLoadPPCmd_MovePickPlace::State_NormalComplete()
{
// 	if (ePickPlace == DEF_PLACE) {
// 		int nTblIdx = (eStage == LOC_IDX_TRAY_PP_TBL_2) ? eLoadTbl_2 : eLoadTbl_3;
// 		CTime time = CTime::GetCurrentTime();
// 		g_TaskLoadTbl[nTblIdx].SetMoveSetPlateTm(time);
// 	}
	return CBaseSeqCmd::State_NormalComplete();
}

BOOL CTrayLoadPPCmd_MovePickPlace::State_Alarm()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = ( CTaskTrayLoadPP* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTaskTrayLoadPP->MakeLog( "[CMD: %s] [ALARM] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0: // alarm 발생 했을 때 처리해 줘야 할꺼 처리 해줌
		{
			int nAxisCount = pTaskTrayLoadPP->GetMotorCount();
			for( int i = 0; i < nAxisCount; i++ ) {
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis( static_cast< CTaskTrayBasePP::eMotor >( i ) );
				if( pMotor == nullptr ) {
					continue;
				}

				bool bCloseLimit = pMotor->CheckLimitClose();
				double dCurPos = 0.0;
				pMotor->GetCurrentPos(dCurPos);
				if( bCloseLimit == false )
				{
					pTaskTrayLoadPP->MakeLog( "[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0 );
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

BOOL CTrayLoadPPCmd_MovePickPlace::State_AlarmComplete()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = ( CTaskTrayLoadPP* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTaskTrayLoadPP->MakeLog( "[CMD: %s] [ALARM COMP] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step );
	}
	m_nStep_Pre = m_nStep;

	OneAxis* pMotorZ = pTaskTrayLoadPP->GetAxis( CTaskTrayBasePP::eAxisZ );

	switch( m_nStep )
	{
	case 0:
		{
			//pTaskTrayPP->SendEvent(CEID_TRAY_PP_ALARMCOMPLETE_START);
			m_nStep = 1000;
		}break;
	case 1000: // move z axis safety & picker all up
		{
			std::vector<CPoint> vPkr;
			pTaskTrayLoadPP->MakeAllPicker(vPkr);
			int nErr = pTaskTrayLoadPP->do_MovePos( eStage, Pocket, vPkr, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ );
			if( nErr != ERR_NO_ERROR ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Tray PP Z Axis safety move command Error" );
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis((CTaskTrayBasePP::eMotor)eAxisZ);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm( nErr, AlarmData );
				break;
			}

			m_tcTimeout = ( double )GetTickCount64();
			m_nStep = 3000;
		}break;
	case 3000:	// check move z safety
		{
			double dSpanTime = ( double )GetTickCount64() - m_tcTimeout;
			double dCurPos = 0.0;
			pMotorZ->GetCurrentPos(dCurPos);
			if( dSpanTime > ( MOTOR_TIME_OUT_MIDDLE * 1000.0 ) ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Load Tray PP z-axis operation is not complete, SpanTime=%.2f, z-axis current pos=%.2f", dSpanTime * 1000.0, dCurPos);
				ReportAlarm(ERR_TRAY_PP_1_Z_PICKER_MOTOR_MOVE_TIMEOUT, AlarmData );
				break;
			}

			int nAxisDone = pMotorZ->AxisDone();
			if( nAxisDone == ERR_NO_ERROR ) {
				m_nStep = 4000;
				m_nRetryCount = 0;
				break;
			}
			else if( nAxisDone == -1 ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Tray PP z-axis operation is not complete" );
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotorZ->GetAxisName());
				ReportAlarm( ERR_TRAY_PP_1_MOTOR_MOVE_FAIL, AlarmData );
			}
		}break;
	case 4000:
		{
			// Alarm Comp finish
			eStacker_Type stacker_type = ( eStacker_Type )-1;
			if (LOC_IDX_TRAY_PP_CONV_1 <= eStage && eStage <= LOC_IDX_TRAY_PP_CONV_7)
				stacker_type = g_TaskAutoStacker[eStage - LOC_IDX_TRAY_PP_CONV_1].GetStackerType();
			else if (LOC_IDX_TRAY_PP_FEEDER == eStage)
				stacker_type = eStackerType_Load;

			if( CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto ) {

				int nSkipType = eDeviceData_Erase;
				if( g_DMCont.m_dmHandlerOpt.GB( BDM11_MD_AutoSkipUse ) == TRUE && g_DMCont.m_dmHandlerOpt.GB( BDM11_MD_AutoGrrUse ) == FALSE
					&& ePickPlace == DEF_PICK && ( stacker_type == eStackerType_Load ) )
					nSkipType = eDeviceData_EraseAndAutoSkip;

				std::vector<CString> vErrString; vErrString.clear();
				pTaskTrayLoadPP->ProcDeviceData( eStage, Pocket.x, Pocket.y, vPicker, ePickPlace, vErrString, nSkipType );

				CString strPicker = _T( "" ), strTmp = _T( "" );
				for( std::vector<CPoint>::size_type i = 0; i < ( int )vPicker.size(); i++ ) {
					strTmp.Format( "[X:%d, Y:%d]", vPicker[ i ].x + 1, vPicker[ i ].y + 1 );
					strPicker += strTmp;
				}

				pTaskTrayLoadPP->MakeLog( "[ProcDevice Data] CmdStage=%d, CmdTargetX=%d, CmdTargetY=%d, Picker=%s", eStage, Pocket.x + 1, Pocket.y + 1, strPicker );

				// set in/out jam count
				int nCount = pTaskTrayLoadPP->GetLostDvcCount( vPicker, ePickPlace );
				switch( eStage )
				{
				case LOC_IDX_TRAY_PP_TBL_1:
				case LOC_IDX_TRAY_PP_TBL_2:
					{
						if( ePickPlace == DEF_PICK ) {
							g_JamRate.PushInOutJam( eJamType_Output, nCount );
						}
						else {
							g_JamRate.PushInOutJam( eJamType_Input, nCount );
						}
					}break;
				}

				if( stacker_type == eStackerType_Unload ) {
					if( ePickPlace != DEF_PICK ) {
						g_JamRate.PushInOutJam( eJamType_Output, nCount );
					}
				}
			}

			if( pTaskTrayLoadPP->m_bSkipToAssign == TRUE ) {
				if( stacker_type == eStackerType_Load ) {
					g_GuiComm.SendDataAssignShow( eStage );
				}
				pTaskTrayLoadPP->m_bSkipToAssign = FALSE;
			}

			pTaskTrayLoadPP->MakeLog( "[CMD: %s] [ALARM COMP] [Step=%8d] AlarmComplete End", m_szCmdName, m_nStep );
			return CBaseSeqCmd::State_AlarmComplete();
		}break;

	default:
		{
			m_nStep = 0;
		}break;
	}

	return CBaseSeqCmd::State_AlarmComplete();
}

BOOL CTrayLoadPPCmd_MovePickPlace::State_Aborting()
{
	CTaskTrayLoadPP* pTaskTrayLoadPP = ( CTaskTrayLoadPP* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTaskTrayLoadPP->MakeLog( "[CMD: %s] [ABORT] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step );
	}
	m_nStep_Pre = m_nStep;

	OneAxis* pMotorZ = pTaskTrayLoadPP->GetAxis( CTaskTrayBasePP::eAxisZ );

	switch( m_nStep )
	{
	case 0:
		{
			int nAxisCount = pTaskTrayLoadPP->GetMotorCount();
			for( int i = 0; i < nAxisCount; i++ ) {
				OneAxis* pMotor = pTaskTrayLoadPP->GetAxis( static_cast< CTaskTrayBasePP::eMotor >( i ) );
				if( pMotor != nullptr ) {
					pMotor->SetSlowStop();
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

BOOL CTrayLoadPPCmd_MovePickPlace::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}


eCheckAbnormalCase CTrayLoadPPCmd_MovePickPlace::check_abnormal_case( int _PickPlace, CString& error_msg )
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

	CTaskTrayLoadPP* pTaskTrayLoadPP = ( CTaskTrayLoadPP* )m_pReceive;

	std::vector< std::pair<CPoint, CPoint> > vPickerToPocket = pTaskTrayLoadPP->change_picker_to_pocket_point( eStage, Pocket.x, Pocket.y, vPicker );

	eCheckAbnormalCase last_ret = invaild_case;
	CString last_ret_str = _T( "" );

	for each( auto picker in vPicker ) {
		ST_DD_DEVICE stDevice;
		pTaskTrayLoadPP->GetTargetCmdStageDvcData( picker.x, picker.y, &stDevice );

		auto iter_current_picker_pocket = std::find_if( vPickerToPocket.begin(), vPickerToPocket.end()
														, [ & ]( std::pair< CPoint, CPoint > _picker_to_pocket ) -> bool
		{
			if( _picker_to_pocket.first == picker )
				return true;

			return false;
		} );

		CPoint target_pocket = iter_current_picker_pocket->second;
		eDeviceDataStage data_stage_idx = eDeviceDataStage::eDeviceStage_Undefine;
		pTaskTrayLoadPP->ConvertCmdIdxToStageIdx( eStage, &data_stage_idx );
		int nPocketDataExist = g_DMCont.m_dmDevice.GetDeviceExist( data_stage_idx, target_pocket.x, target_pocket.y );

		bool bPickerDataExist = stDevice.sExist == DEF_EXIST;
		bool nPickerVacuumOn = pTaskTrayLoadPP->GetVacuum( picker ) == DEF_ON;
		bool bPocketDataExist = nPocketDataExist == DEF_EXIST;

		int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN( NDM11_SYS_MD_DEVICE_CHK );

		error_msg.Format( "[ Picker( %d, %d ) Device Data : %s ][ Picker Vacuum : %s ][ Pocker( %d, %d ) Device Data : %s ]"
						  , picker.x + 1, picker.y + 1, bPickerDataExist == true ? _T( "Exist" ) : _T( "Not Exist" )
						  , nPickerVacuumOn == true ? _T( "On" ) : _T( "Off" )
						  , target_pocket.x + 1, target_pocket.y + 1, bPocketDataExist == true ? _T( "Exist" ) : _T( "Not Exist" ) );

		pTaskTrayLoadPP->MakeLog( "[%s] [ Check Abnormal Case ] %s", m_szCmdName, error_msg );

		if( ( bPickerDataExist == true && nPickerVacuumOn == true && bPocketDataExist == true )
			|| ( bPickerDataExist == true && nPickerVacuumOn == false && bPocketDataExist == true )
			|| ( bPickerDataExist == false && nPickerVacuumOn == false && bPocketDataExist == false ) )
		{
			last_ret = abnormal;
			last_ret_str = _T( "abnormal" );
			break;
		}
		else if( bPickerDataExist == true && ( nPickerVacuumOn == true || nDeviceMode != eDeviceMode ) && bPocketDataExist == false ) {
			if( ePickPlace == DEF_PICK ) {
				last_ret = cmd_end;
				last_ret_str = _T( "cmd_end" );
			}
			else if( ePickPlace == DEF_PLACE ) {
				last_ret = normal_start;
				last_ret_str = _T( "normal_start" );
			}
		}
		else if( bPickerDataExist == false && ( nPickerVacuumOn == false || nDeviceMode != eDeviceMode ) && bPocketDataExist == true ) {
			if( ePickPlace == DEF_PICK ) {
				last_ret = normal_start;
				last_ret_str = _T( "normal_start" );
			}
			else if( ePickPlace == DEF_PLACE ) {
				last_ret = cmd_end;
				last_ret_str = _T( "cmd_end" );
			}
		}
		else if( bPickerDataExist == false && nPickerVacuumOn == true && bPocketDataExist == true ) {
			if( ePickPlace == DEF_PICK ) {
				last_ret = situation;
				last_ret_str = _T( "situation" );
				break;
			}
			else if( ePickPlace == DEF_PLACE ) {
				last_ret = alarm_device_exist;
				last_ret_str = _T( "alarm_device_exist" );
				break;
			}
		}
		else if( bPickerDataExist == true && nPickerVacuumOn == false && bPocketDataExist == false ) {
			if( ePickPlace == DEF_PICK ) {
				last_ret = alarm_drop;
				last_ret_str = _T( "alarm_drop" );
				break;
			}
			else if( ePickPlace == DEF_PLACE ) {
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

	pTaskTrayLoadPP->MakeLog( "[%s] [ Check Abnormal Case is %s ]", m_szCmdName, last_ret_str );
	return last_ret;
}

CString CTrayLoadPPCmd_MovePickPlace::act_string( int act )
{
	BEGIN_CASE_TO_STR( act );
	CASE_TO_STR( start );
	CASE_TO_STR( do_blow );
	CASE_TO_STR( z_move_safety );
	CASE_TO_STR( z_move_safety_chk );
	CASE_TO_STR( x_px_py_move );
	CASE_TO_STR( x_px_py_move_chk );
	CASE_TO_STR( waiting_for_load_table );
	CASE_TO_STR( waiting_for_feeder);
	CASE_TO_STR( move_z_target_pos );
	CASE_TO_STR(move_z_target_soft_Pick_1stage_done);
	CASE_TO_STR(move_z_target_soft_Pick_2stage);
	CASE_TO_STR( checking_z_axis_done );
	CASE_TO_STR( pick_vac );
	CASE_TO_STR( move_z_safety );
	CASE_TO_STR( check_z_axis_done );
	CASE_TO_STR(check_vacuum_fail);
	CASE_TO_STR( picker_up_check_and_write_device_data );
	CASE_TO_STR( end );
	ELSE_CASE( _T( "unkown_step" ) );
	END_CASE_TO_STR();
}
