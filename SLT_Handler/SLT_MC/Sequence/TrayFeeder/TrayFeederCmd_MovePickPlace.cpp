#include "StdAfx.h"
#include "TrayFeederCmd_MovePickPlace.h"

CTrayFeederCmd_MovePickPlace::CTrayFeederCmd_MovePickPlace( void )
{
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));
	m_nLast_Do_Step = 0;
	m_nAlignRetry = 0;
	m_bDo_FeederUp = FALSE;
}

CTrayFeederCmd_MovePickPlace::~CTrayFeederCmd_MovePickPlace( void )
{
}

int CTrayFeederCmd_MovePickPlace::CheckParam()
{
	if( m_nCmdStage != eLocIdxFeeder::LOC_IDX_FEEDER_TRAY_PICK_POS &&
		m_nCmdStage != eLocIdxFeeder::LOC_IDX_FEEDER_TRAY_PLACE_POS)
	{
		return ERR_CMD_PARAM_INVALID;
	}

	if( m_nCmdPickPlace != ePPCmd::ePick &&
		m_nCmdPickPlace != ePPCmd::ePlace )
	{
		return ERR_CMD_PARAM_INVALID;
	}

	return ERR_NO_ERROR;
}

int CTrayFeederCmd_MovePickPlace::OnStop()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Aborting );
	return ERR_NO_ERROR;
}

int CTrayFeederCmd_MovePickPlace::OnRetry()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

int CTrayFeederCmd_MovePickPlace::OnSkip()
{
	ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_AlarmComplete );
	return ERR_NO_ERROR;
}

int CTrayFeederCmd_MovePickPlace::OnPause()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Pause );
	return ERR_NO_ERROR;
}

int CTrayFeederCmd_MovePickPlace::OnResume()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

BOOL CTrayFeederCmd_MovePickPlace::State_Setup()
{
	m_swTimeout.SetCheckTime( TIME_OUT_MIDDLE );
	m_swTimeout.StartTimer();

	return CBaseSeqCmd::State_Setup();
}

BOOL CTrayFeederCmd_MovePickPlace::State_Executing()
{
	CTaskTrayFeeder* pTask = ( CTaskTrayFeeder* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTask->MakeLog( _T( "[CMD: %s] [EXECUTE] [Step=%s]" ), m_szCmdName, act_string( m_nStep ) );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case start:
		{		
			auto align_for_state = pTask->get_cyl_align_ForBack( eForw );
			auto align_back_state = pTask->get_cyl_align_ForBack( eBack );
			if( align_for_state != ERR_NO_ERROR && align_back_state != ERR_NO_ERROR ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Align cylinder is abnormal state." );
				AlarmData.m_fnAddItemFormat( align_for_state.msg.c_str() );
				AlarmData.m_fnAddItemFormat( align_back_state.msg.c_str() );
				ReportAlarm(ERR_FEEDER_ALIGN_CYLINDER_ABNORMAL, AlarmData );
				break;
			}

			auto clamp_state = pTask->get_cyl_ClampUnclamp( eClamp );
			auto unclamp_state = pTask->get_cyl_ClampUnclamp( eUnclamp );
			if( clamp_state != ERR_NO_ERROR && unclamp_state != ERR_NO_ERROR ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Clamper is abnormal state." );
				AlarmData.m_fnAddItemFormat( clamp_state.msg.c_str() );
				AlarmData.m_fnAddItemFormat( unclamp_state.msg.c_str() );
				ReportAlarm(ERR_FEEDER_CLAMP_CYLINDER_ABNORMAL, AlarmData );
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

			m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
			m_swTimeout.StartTimer();
			m_nLast_Do_Step = m_nStep;
			m_nStep = do_chk_up_cyl_start;
		}break;

	case do_chk_up_cyl_start:
		{
			if( m_swTimeout.CheckOverTime() ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Feeder Up Timeout." );
				ReportAlarm(ERR_FEEDER_UP_DOWN_CYLINDER_TIMEOUT, AlarmData );
				break;
			}

			pTask->do_cyl_UpDown( eUp );

			int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_UPDN_WAITTIME);
			if (0 < nSleep) {
				Sleep(nSleep);
			}

			auto chk = pTask->chk_cyl_UpDown( eUp );
			if( chk == ERR_NO_ERROR ) {
//				m_nStep = do_move;
				m_nLast_Do_Step = m_nStep;
				m_nStep = do_chk_Interlock_before_move;
				break;
			}
		}break;
	case do_chk_Interlock_before_move:
		{
			int nErr = -1;
			switch (m_nCmdPickPlace)
			{
				case ePPCmd::ePick: {
					//1. Stacker에 C-Tray 있는지 확인.
					nErr = g_TaskStacker[eSTACKER_03].ChkExistTray_RearPos_Sen(eExist);
					if (nErr != ERR_NO_ERROR) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat(_T("Stacker : 3"));
						ReportAlarm(ERR_STACKER_3_TRAY_NOT_EXIST_REAR_POS, AlarmData);
						break;
					}

					//2. Feeder에 Tray 있는지 확인.
					nErr = pTask->GetTrayExistSensor_DvcMode(eNot_Exist); //B접
					if (nErr == DEF_EXIST) {
						CParamList AlarmData;
//						AlarmData.m_fnAddItemFormat(_T("Feeder Move Tray Site"));
						ReportAlarm(ERR_FEEDER_TRAY_DETECTED, AlarmData);
						break;
					}
					pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] PICK", m_szCmdName, m_nStep);
				} break;
				case ePPCmd::ePlace: { 
					//1. Feeder에 Tray가 있는지 확인.
					nErr = pTask->ChkTrayExistSensor(eExist);
					if (nErr != ERR_NO_ERROR) {
						CParamList AlarmData;
//						AlarmData.m_fnAddItemFormat(_T("Feeder Move Tray Site"));
						ReportAlarm(ERR_FEEDER_TRAY_NOT_DETECTED, AlarmData);
						break;
					}

					//2. Trasnfer영역에 C-Tray 있는지 확인.
					nErr = g_TaskTrayFeeder.ChkTrayExist_TransferArea_Sen(eExist);
					if (nErr == TRUE) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat(_T("Stacker : 3"));
						ReportAlarm(ERR_FEEDER_TRANSFER_AREA_TRAY_DETECTED, AlarmData);
						break;
					}
					pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] PLACE", m_szCmdName, m_nStep);
				}break;
			}
			m_nLast_Do_Step = m_nStep;
			m_nStep = do_move;
	}break;

	case do_move:
		{
			int err = pTask->do_MovePos( CTaskTrayFeeder::eAxisY, m_nCmdStage );
			if( err != ERR_NO_ERROR ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Feeder Move Fail." );
				OneAxis* pMotor = pTask->GetAxis(CTaskTrayFeeder::eAxisY);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(err, AlarmData);
				//ReportAlarm( ERR_FEEDER_MOTOR_MOVE_FAIL, AlarmData );
				break;
			}

			m_swTimeout.SetCheckTime( TIME_OUT_LONG );
			m_swTimeout.StartTimer();
			m_nLast_Do_Step = m_nStep;
			m_nStep = chk_move;
		}break;
	case chk_move:
		{
			OneAxis* pMotor = pTask->GetAxis(CTaskTrayFeeder::eAxisY);
			if( m_swTimeout.CheckOverTime() ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Feeder Motor Move Timeout." );
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm( ERR_FEEDER_MOTOR_MOVE_TIMEOUT, AlarmData );
				break;
			}

			int err = pTask->ChkMotorPos( CTaskTrayFeeder::eAxisY, m_nCmdStage );
			if( err == ERR_NO_ERROR ) {
				m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep = do_chk_down_cyl;
				break;
			}
		}break;
	case do_chk_down_cyl:
		{
			m_swTimeout.SetCheckTime(TIME_OUT_SHORT);
			m_swTimeout.StartTimer();
			switch (m_nCmdPickPlace)
			{
				case ePPCmd::ePick: {
					//vat 측정 시에는 Device Data와 비교 안하도록.
					BOOL bVatTeaching = g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_USE);
					if (pTask->GetEqpCtrlMode() == eEqpCtrlMode_Auto && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_ && bVatTeaching != TRUE)
					{
						BOOL bConveyor3Sen = g_TaskStacker[eSTACKER_03].ChkExistTray_RearPos_Sen(DEF_EXIST) == ERR_NO_ERROR;
						BOOL bFeederExistSen = g_TaskTrayFeeder.GetTrayExistSensor(); //Sensor Check(B접);


						BOOL bConveyor3Data = g_DMCont.m_dmDevice.GetTrayExist(eDeviceDataStage::eDeviceStage_STACKER_CONV_3);;
						BOOL bFeederExistData = g_TaskTrayFeeder.GetTrayExist_Data();						

						if (bFeederExistSen != bFeederExistData)
						{
							CParamList AlarmData;
							ReportAlarm(ERR_FEEDER_DEVICE_DATA_NOT_MISS_MATCH, AlarmData);
							break;
						}

						if (bConveyor3Sen != bConveyor3Data)
						{
							CParamList AlarmData;
							ReportAlarm(ERR_LOT_CMD_CONVEYOR_3_SENSOR_DATA_MISMATCH, AlarmData);
							break;
						}
					}

					pTask->do_cyl_UpDown(eDown);
					int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_UPDN_WAITTIME);
					if (0 < nSleep) {
						Sleep(nSleep);
					}

					auto chk = pTask->chk_cyl_UpDown(eDown);
					if (chk == ERR_NO_ERROR)
					{
						m_nLast_Do_Step = m_nStep;
						m_nStep = pick_unclamp;
						break;
					}
						
					if (m_swTimeout.CheckOverTime()) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Feeder Down Timeout.");
						AlarmData.m_fnAddItemFormat(chk.msg.c_str());
						ReportAlarm(ERR_FEEDER_UP_DOWN_CYLINDER_TIMEOUT, AlarmData);
						break;
					}
					
				} break;
				case ePPCmd::ePlace: { 
					m_nLast_Do_Step = m_nStep;
					m_nStep = place_dealign;
				}break;
				
			}
			break;		
		}break;
		// pick
	case pick_unclamp:
	{
		int nChkUp = g_TaskTrayFeeder.get_cyl_UpDown(eUp);
		BOOL bFeederExistSen = g_TaskTrayFeeder.GetTrayExistSensor(); //Sensor Check(B접);
		if (pTask->GetEqpCtrlMode() == eEqpCtrlMode_Auto)
		{			
			BOOL bFeederExistData = g_TaskTrayFeeder.GetTrayExist_Data();

			if (nChkUp == ERR_NO_ERROR && bFeederExistSen != bFeederExistData)
			{
				CParamList AlarmData;
				ReportAlarm(ERR_FEEDER_GRIPPER_UNCLAMP_FIAL_BECAUSE_FEEDER_POS_UP_AND_FEEDER_HAS_C_TRAY, AlarmData);
				break;
			}
		}
		else
		{
			if (nChkUp == ERR_NO_ERROR && bFeederExistSen == TRUE)
			{
				CParamList AlarmData;
				ReportAlarm(ERR_FEEDER_GRIPPER_UNCLAMP_FIAL_BECAUSE_FEEDER_POS_UP_AND_FEEDER_HAS_C_TRAY, AlarmData);
				break;
			}
		}

		pTask->do_cyl_ClampUnclamp(eUnclamp);
		int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_CLAMP_WAITTIME);
		if (0 < nSleep) {
			Sleep(nSleep);
		}

		auto chk_clamp = pTask->chk_cyl_ClampUnclamp(eUnclamp);

		if (m_swTimeout.CheckOverTime()) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Clamp Timeout.");
			AlarmData.m_fnAddItemFormat(chk_clamp.msg.c_str());
			ReportAlarm(ERR_FEEDER_CLAMP_CYLINDER_TIMEOUT, AlarmData);
			break;
		}

		if (chk_clamp == ERR_NO_ERROR) {
			m_swTimeout.SetCheckTime(TIME_OUT_SHORT);
			m_swTimeout.StartTimer();
			m_nLast_Do_Step = m_nStep;
			m_nStep = pick_before_conv_align_back;
			break;
		}
	}break;
	case pick_before_conv_align_back:
	{
		g_TaskStacker[eSTACKER_03].do_cyl_TrayAlign(eBack);
		int nErr = g_TaskStacker[eSTACKER_03].chk_cyl_TrayAlign(eBack);
		if (nErr == ERR_NO_ERROR) {
			m_swTimeout.SetCheckTime(TIME_OUT_SHORT);
			m_swTimeout.StartTimer();
			m_nLast_Do_Step = m_nStep;		
			m_nStep = pick_conv_align;
			break;
		}

		if (m_swTimeout.CheckOverTime()) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Stacker 03 Before Align backword Timeout.");
			ReportAlarm(ERR_STACKER_3_TRAY_ALIGN_BKWD_TIMEOUT, AlarmData);
			break;
		}
	}break;
	case pick_conv_align:
	{
		g_TaskStacker[eSTACKER_03].do_cyl_TrayAlign(eForw);
		int nErr = g_TaskStacker[eSTACKER_03].chk_cyl_TrayAlign(eForw);
		if (nErr == ERR_NO_ERROR) {
			m_swTimeout.SetCheckTime(TIME_OUT_SHORT);
			m_swTimeout.StartTimer();
			m_nLast_Do_Step = m_nStep;
			m_nStep = pick_clamp;
			break;
		}

		if (m_swTimeout.CheckOverTime()) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Stacker 03 Align Forword Timeout.");
			ReportAlarm(ERR_STACKER_3_TRAY_ALIGN_BKWD_TIMEOUT, AlarmData);
			break;
		}
	}break;
	case pick_clamp:
		{
			pTask->do_cyl_ClampUnclamp( eClamp );
			int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_CLAMP_WAITTIME);
			if (0 < nSleep) {
				Sleep(nSleep);
			}

			auto chk_clamp = pTask->chk_cyl_ClampUnclamp( eClamp );
			 
			if( m_swTimeout.CheckOverTime() ) {
				CParamList AlarmData; 
				AlarmData.m_fnAddItemFormat( "Clamp Timeout." );
				AlarmData.m_fnAddItemFormat( chk_clamp.msg.c_str() );
				ReportAlarm(ERR_FEEDER_CLAMP_CYLINDER_TIMEOUT, AlarmData );
				break;
			} 
			

			if( chk_clamp == ERR_NO_ERROR ) {
				m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep = pick_after_conv_align_back;
				break; 
			}
		}break;
	case pick_after_conv_align_back:
	{
		g_TaskStacker[eSTACKER_03].do_cyl_TrayAlign(eBack);
		int nErr = g_TaskStacker[eSTACKER_03].chk_cyl_TrayAlign(eBack);
		if (nErr == ERR_NO_ERROR) {
			m_swTimeout.SetCheckTime(TIME_OUT_SHORT);
			m_swTimeout.StartTimer();
			m_nLast_Do_Step = m_nStep;
			m_nStep = pick_do_chk_up_cyl_start;
			break;
		}

		if (m_swTimeout.CheckOverTime()) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Stacker 03 After Align backword Timeout.");
			ReportAlarm(ERR_STACKER_3_TRAY_ALIGN_BKWD_TIMEOUT, AlarmData);
			break;
		}
	}break;
	case pick_do_chk_up_cyl_start:
	{
		if (m_swTimeout.CheckOverTime()) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Feeder Up Timeout.");
			ReportAlarm(ERR_FEEDER_UP_DOWN_CYLINDER_TIMEOUT, AlarmData);
			break;
		}

		pTask->do_cyl_UpDown(eUp);
		m_bDo_FeederUp = TRUE;

		int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_UPDN_WAITTIME);
		if (0 < nSleep) {
			Sleep(nSleep);
		}

		auto chk = pTask->chk_cyl_UpDown(eUp);
		if (chk == ERR_NO_ERROR) {
			m_swTimeout.SetCheckTime(TIME_OUT_SHORT);
			m_swTimeout.StartTimer();
			m_nLast_Do_Step = m_nStep;
			m_nStep = pick_align;
			break;
		}
	}break;
	case pick_align:
		{
			pTask->do_cyl_align_ForBack( eForw );
			auto chk_align = pTask->chk_cyl_align_ForBack( eForw );

			if( m_swTimeout.CheckOverTime() ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Feeder Align Forward Timeout." );
				AlarmData.m_fnAddItemFormat( chk_align.msg.c_str() );
				ReportAlarm(ERR_FEEDER_ALIGN_CYLINDER_TIMEOUT, AlarmData );
				break;
			}

			if( chk_align == ERR_NO_ERROR  ) {
				m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
				m_swTimeout.StartTimer();
// 				if (m_nAlignRetry >= 1)
// 				{
					m_nLast_Do_Step = m_nStep;
					m_nStep = do_chk_up_cyl_end;
// 				}
// 				else
// 				{
// 					m_nLast_Do_Step = m_nStep;
// 					m_nStep = pick_align_back;
// 				}
				
			}
		}break;
	case pick_align_back:
	{
		m_nAlignRetry++;
		pTask->do_cyl_align_ForBack(eBack);
		int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_ALIGN_WAITTIME);
		if (0 < nSleep) {
			Sleep(nSleep);
		}
		auto chk_align = pTask->chk_cyl_align_ForBack(eBack);

		if (m_swTimeout.CheckOverTime()) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Feeder Align Backward Timeout.");
			AlarmData.m_fnAddItemFormat(chk_align.msg.c_str());
			ReportAlarm(ERR_FEEDER_ALIGN_CYLINDER_TIMEOUT, AlarmData);
			break;
		}

		if (chk_align == ERR_NO_ERROR) {
			m_swTimeout.SetCheckTime(TIME_OUT_SHORT);
			m_swTimeout.StartTimer();
			m_nLast_Do_Step = m_nStep;
			m_nStep = pick_align;
			break;
		}
	}break;

		// place
	case place_dealign:
		{
			pTask->do_cyl_align_ForBack( eBack );
			int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_ALIGN_WAITTIME);
			if (0 < nSleep) {
				Sleep(nSleep);
			}
			auto chk_align = pTask->chk_cyl_align_ForBack( eBack );

			if( m_swTimeout.CheckOverTime() ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Feeder Align Backward Timeout." );
				AlarmData.m_fnAddItemFormat( chk_align.msg.c_str() );
				ReportAlarm(ERR_FEEDER_ALIGN_CYLINDER_TIMEOUT, AlarmData );
				break;
			}

			if( chk_align == ERR_NO_ERROR ) {
				m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep = place_down_cyl;
				break;
			}
		}break;
	case place_down_cyl:
	{
		//[8/28/2024 dohyeong.kim] Conveyor 동작 중, Down 안하도록
		//Full Tray 투입 중, Feeder C-Tray Down 하여 Trasnfer Area Sensor 감지 못하는 현상 발생.
		bool conveyor_moving = g_TaskStacker[eSTACKER_03].IsConveyorMoving();
		if (conveyor_moving == true)
			break;

		pTask->do_cyl_UpDown(eDown);
		int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_UPDN_WAITTIME);
		if (0 < nSleep) {
			Sleep(nSleep);
		}

		auto chk = pTask->chk_cyl_UpDown(eDown);
		if (chk == ERR_NO_ERROR)
		{
			m_swTimeout.SetCheckTime(TIME_OUT_SHORT*2);
			m_swTimeout.StartTimer();
			m_nLast_Do_Step = m_nStep;
			m_nStep = place_unclamp;
			break;
		}

		if (m_swTimeout.CheckOverTime()) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Feeder Down Timeout.");
			AlarmData.m_fnAddItemFormat(chk.msg.c_str());
			ReportAlarm(ERR_FEEDER_UP_DOWN_CYLINDER_TIMEOUT, AlarmData);
			break;
		}

	}break;
	case place_unclamp:
		{

			pTask->do_cyl_ClampUnclamp(eUnclamp);
			int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_CLAMP_WAITTIME);
			if (0 < nSleep) {
				Sleep(nSleep);
			}

			ST_DD_TRAY tray;


			
			auto chk_clamp = pTask->chk_cyl_ClampUnclamp(eUnclamp);
			if( chk_clamp == ERR_NO_ERROR ) {
				m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep = do_chk_up_cyl_end;
				break;
			}

			if (m_swTimeout.CheckOverTime()) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Clamp Timeout.");
				AlarmData.m_fnAddItemFormat(chk_clamp.msg.c_str());
				ReportAlarm(ERR_FEEDER_CLAMP_CYLINDER_TIMEOUT, AlarmData);
				break;
			}

		}break;

	case do_chk_up_cyl_end:
		{
			pTask->do_cyl_UpDown( eUp );
			int nSleep = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_UPDN_WAITTIME);
			if (0 < nSleep) {
				Sleep(nSleep);
			}

			auto chk = pTask->chk_cyl_UpDown( eUp );
			if( m_swTimeout.CheckOverTime() ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Feeder Up Timeout." );
				AlarmData.m_fnAddItemFormat( chk.msg.c_str() );
				ReportAlarm(ERR_FEEDER_UP_DOWN_CYLINDER_TIMEOUT, AlarmData );
				break;
			}

			if( chk == ERR_NO_ERROR )
			{
				m_swTimeout.SetCheckTime(TIME_OUT_SHORT*2);
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep = transfer_area_sensor_chk;
				break;
			}
		}break;
	// Transfer Area Place 하고 나서 Data 넘기기 전에 Transfer Area Sensor 감지 되고 있는지 확인 해준다.
	case transfer_area_sensor_chk:
	{
		if (m_nCmdPickPlace == ePlace)
		{
			if (m_swTimeout.CheckOverTime()) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat(_T("Stacker : 3 Transfer Area Sensor Not Detected"));
				ReportAlarm(ERR_FEEDER_TRANSFER_AREA_TRAY_NOT_DETECTED, AlarmData);
				break;
			}

			int transfer_area_tray_exist = g_TaskTrayFeeder.ChkTrayExist_TransferArea_Sen(eExist);
			if (transfer_area_tray_exist != ERR_NO_ERROR)
				break;
		}

		m_swTimeout.SetCheckTime(TIME_OUT_SHORT);
		m_swTimeout.StartTimer();
		m_nStep = check_tray;
	}break;

	case check_tray :
		{
			if (m_swTimeout.CheckOverTime()) {
				CParamList AlarmData;
				ReportAlarm(ERR_FEEDER_TRAY_CHECK_TIMEOUT_ERROR, AlarmData);
				break;
			}

			if( m_nCmdPickPlace == ePick ) {
				if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto || g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
				{
					pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%s] Pick Data Swap %s -> %s"), m_szCmdName, act_string(m_nStep), "eSTACKER_03", "eDeviceStage_TRAY_FEEDER");
					g_TaskStacker[eSTACKER_03].ChkExistTray_RearPos_Sen(eNot_Exist);
					g_DMCont.m_dmDevice.SwapStageData(eDeviceStage_STACKER_CONV_3, eDeviceStage_TRAY_FEEDER);
					Sleep(300);
					int nTrayFeeder_Data = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_TRAY_FEEDER);
					int nTrayConv3_Data = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_CONV_3);
					if (nTrayFeeder_Data == false)
					{
						pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%s] Pick Data Swap nTrayFeeder_Data = false = %s [nTrayConv3_Data = %d]"), m_szCmdName, act_string(m_nStep), "eDeviceStage_TRAY_FEEDER", nTrayConv3_Data);
						if (nTrayConv3_Data == false)
						{
							int tray_x_cnt = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
							int tray_y_cnt = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);
							ST_DD_TRAY* stTrayData = new ST_DD_TRAY;
							stTrayData->bTrayExist = TRUE;
							stTrayData->nRowMax = tray_y_cnt;
							stTrayData->nColMax = tray_x_cnt;
							g_DMCont.m_dmDevice.SetTrayData(eDeviceStage_TRAY_FEEDER, *stTrayData);
							ST_DD_DEVICE* stDvc = new ST_DD_DEVICE;
							stDvc->clear();
							for (int y = 0; y < tray_y_cnt; y++) {
								for (int x = 0; x < tray_x_cnt; x++) {
									stDvc->sExist = DEF_EXIST;

									if (stDvc->sExist == DEF_EXIST) {
										StringCchPrintf(stDvc->szBinResult, _countof(stDvc->szBinResult), DEF_NEW_LOAD_DEVICE);
									}
									g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TRAY_FEEDER, x, y, stDvc);
								}
							}				
						}
						nTrayFeeder_Data = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_TRAY_FEEDER);
						pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%s] Pick Data Swap nTrayFeeder_Data = false = %s [nTrayFeeder_Data = %d]"), m_szCmdName, act_string(m_nStep), "eDeviceStage_TRAY_FEEDER", nTrayFeeder_Data);
					}
					g_DMCont.m_dmTrayPP.SN(NDM5_TM_LDTrayPP_Remind_Pick_Count, 0);
				}	

				g_TaskTrayFeeder.SetClearStep();
			}
			else {
// 				nErr = g_TaskTrayFeeder.ChkTrayExist_TransferArea_Sen( eExist );
// 				if (nErr == ERR_NO_ERROR)
// 				{
					if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto || g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
					{
						pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%s] Place Data Swap %s -> %s"), m_szCmdName, act_string(m_nStep), "eDeviceStage_TRAY_FEEDER", "eDeviceStage_STACKER_CONV_3_TRANSFER_AREA");
						g_DMCont.m_dmDevice.SwapStageData(eDeviceStage_TRAY_FEEDER, eDeviceStage_STACKER_CONV_3_TRANSFER_AREA);
					}
	//			}
				
			}			
		
			m_nStep = end;
			break;
			
		}break;

	case end:
		{
			ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_NormalComplete );
		}break;
	}

	return CBaseSeqCmd::State_Executing();
}

BOOL CTrayFeederCmd_MovePickPlace::State_Pause()
{
	CTaskTrayFeeder* pTask = ( CTaskTrayFeeder* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTask->MakeLog( _T( "[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]" ), m_szCmdName, m_nStep, m_nLast_Do_Step );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			int err = ERR_NO_ERROR;
			for( int i = 0; i < CTaskTrayFeeder::eAxisMax; i++ ) {
				OneAxis* pMotor = pTask->GetAxis( ( CTaskTrayFeeder::eMotor )i );
				if (pMotor != nullptr) {
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

			m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
			m_swTimeout.StartTimer();
			m_nStep++;
		}break;
	case 1:
		{
			if( m_swTimeout.CheckOverTime() ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Feeder Motor Stop Timeout." );
				ReportAlarm( ERR_FEEDER_MOTOR_STOP_TIMEOUT, AlarmData );
				break;
			}

			int motion_done_cnt = 0;
			for( int i = 0; i < CTaskTrayFeeder::eAxisMax; i++ ) {
				OneAxis* pMotor = pTask->GetAxis( ( CTaskTrayFeeder::eMotor )i );
				if (pMotor == nullptr) {
					motion_done_cnt++;
					continue;
				}
				motion_done_cnt += pMotor->MotionDone() == ERR_NO_ERROR ? 1 : 0;
			}

			if( motion_done_cnt == CTaskTrayFeeder::eAxisMax ) {
				m_nStep++;
				break;
			}
		}break;
	case 2:
		{
			
		}break;
	default:
		{
			m_nStep = 0;
		}break;
	}

	return CBaseSeqCmd::State_Pause();
}

BOOL CTrayFeederCmd_MovePickPlace::State_Recovery()
{
	CTaskTrayFeeder* pTask = ( CTaskTrayFeeder* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTask->MakeLog( "[CMD: %s] [RECOVERY] [Step=%8d] [Last Step=%d]", m_szCmdName, m_nStep, m_nLast_Do_Step );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			auto align_for_state = pTask->get_cyl_align_ForBack( eForw );
			auto align_back_state = pTask->get_cyl_align_ForBack( eBack );
			if( align_for_state != ERR_NO_ERROR && align_back_state != ERR_NO_ERROR ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Align cylinder is abnormal state." );
				AlarmData.m_fnAddItemFormat( align_for_state.msg.c_str() );
				AlarmData.m_fnAddItemFormat( align_back_state.msg.c_str() );
				ReportAlarm(ERR_FEEDER_ALIGN_CYLINDER_ABNORMAL, AlarmData );
				break;
			}

			auto clamp_state = pTask->get_cyl_ClampUnclamp( eClamp );
			auto unclamp_state = pTask->get_cyl_ClampUnclamp( eUnclamp );
			if( clamp_state != ERR_NO_ERROR && unclamp_state != ERR_NO_ERROR ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Clamper is abnormal state." );
				AlarmData.m_fnAddItemFormat( clamp_state.msg.c_str() );
				AlarmData.m_fnAddItemFormat( unclamp_state.msg.c_str() );
				ReportAlarm(ERR_FEEDER_CLAMP_CYLINDER_ABNORMAL, AlarmData );
				break;
			}

			auto up_state = pTask->get_cyl_UpDown( eUp );
			auto down_state = pTask->get_cyl_UpDown( eDown );
			if( up_state != ERR_NO_ERROR && down_state != ERR_NO_ERROR ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Up Down is abnormal state." );
				AlarmData.m_fnAddItemFormat( up_state.msg.c_str() );
				AlarmData.m_fnAddItemFormat( down_state.msg.c_str() );
				ReportAlarm(ERR_FEEDER_UP_DOWN_CYLINDER_ABNORMAL, AlarmData );
				break;
			}

			m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
			m_swTimeout.StartTimer();
			m_nStep++;
		}break;
	case 1:
		{
			//[7/20/2024 dohyeong.kim] 
		    //Status 상태 Check 후, m_nLasStep 설정 되도록 개선.
			int nStatus = pTask->GetFeederStatus();

			BOOL bFeederExistSen = g_TaskTrayFeeder.GetTrayExistSensor(); //Sensor Check(B접);
			BOOL bFeederExistData = g_TaskTrayFeeder.GetTrayExist_Data();

			pTask->MakeLog("[Feeder Before Last Step:%d]", m_nLast_Do_Step);

			if (m_nCmdPickPlace == ePPCmd::ePick)
			{
				switch (nStatus)
				{
					case CTaskTrayFeeder::eFeederStatus_Up_Clamp:
					{
						if (bFeederExistSen == TRUE && bFeederExistData == TRUE)
						{
							m_nLast_Do_Step = end;//pick_align;
						}
						else if (bFeederExistSen != TRUE && bFeederExistData != TRUE)
						{
							m_nLast_Do_Step = do_chk_Interlock_before_move;
						}
						else
						{
							//bFeederExistSen == TRUE && bFeederExistData != TRUE
							//bFeederExistSen != TRUE && bFeederExistData == TRUE
							if (m_bDo_FeederUp != TRUE) //m_bDo_FeederUp가 FALSE이면 C-Tray 들어올리기 전이라 Feeder가 C-Tray 들고 있으면 안됨.
							{
								pTask->MakeLog("[Feeder Status:%d][Feeder Sen:%d][Feeder Data:%d]", nStatus, bFeederExistSen, bFeederExistData);

								CParamList AlarmData;
								ReportAlarm(ERR_FEEDER_DEVICE_DATA_NOT_MISS_MATCH, AlarmData);
								break;
							}
							else
							{
								//m_nLast_Do_Step으로 진행.
							}
						}
					}break;
					case CTaskTrayFeeder::eFeederStatus_Up_Unclamp:
					{
						m_nLast_Do_Step = do_chk_Interlock_before_move;
					}break;
					case CTaskTrayFeeder::eFeederStatus_Down_Clamp:
					case CTaskTrayFeeder::eFeederStatus_Down_Unclamp:
					{
						m_nLast_Do_Step = do_chk_down_cyl;
					}break;
				default:
					break;
				}

				pTask->MakeLog("[Feeder After Last Step:%d]", m_nLast_Do_Step);
				pTask->MakeLog("[Feeder Status:%d][Feeder Sen:%d][Feeder Data:%d]", nStatus, bFeederExistSen, bFeederExistData);

				SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing, m_nLast_Do_Step);
				m_nStep = 99999;
				break;

			}
			else //Place는 상태 상관 없이 Recovery 진행
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
					break;
				}
			}
		}break;
	case 2:
		{
			eLocIdxFeeder	eLoc;
			switch (m_nCmdPickPlace)
			{
				case ePPCmd::ePick: {eLoc = LOC_IDX_FEEDER_TRAY_PICK_POS;}break;
				case ePPCmd::ePlace: { eLoc = LOC_IDX_FEEDER_TRAY_PLACE_POS;}break;
			}
			auto ret = pTask->do_MovePos(CTaskTrayFeeder::eAxisY, eLoc, 0, 50);
			if (ret != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Feeder Move Fail.");
				OneAxis* pMotor = pTask->GetAxis(CTaskTrayFeeder::eAxisY);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(ret, AlarmData);
				break;
			}

			m_swTimeout.SetCheckTime( TIME_OUT_LONG );
			m_swTimeout.StartTimer();
			m_nStep++;
		}break;
	case 3:
		{
			eLocIdxFeeder	eLoc;
			switch (m_nCmdPickPlace)
			{
				case ePPCmd::ePick: {eLoc = LOC_IDX_FEEDER_TRAY_PICK_POS;}break;
				case ePPCmd::ePlace: { eLoc = LOC_IDX_FEEDER_TRAY_PLACE_POS;}break;
			}

			auto ret = pTask->ChkMotorPos( CTaskTrayFeeder::eAxisY, eLoc);
			if( m_swTimeout.CheckOverTime() ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Feeder Move Timeout." );
				OneAxis* pMotor = pTask->GetAxis(CTaskTrayFeeder::eAxisY);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm( ERR_FEEDER_MOTOR_MOVE_TIMEOUT, AlarmData );
				break;
			}

			if( ret == ERR_NO_ERROR ) {
				SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing, m_nLast_Do_Step);
				m_nStep = 99999;
				break;
			}
		}break;
	case 99999:
		{

		}break;
	}

	return CBaseSeqCmd::State_Recovery();
}

BOOL CTrayFeederCmd_MovePickPlace::State_NormalComplete()
{
	return CBaseSeqCmd::State_NormalComplete();
}

BOOL CTrayFeederCmd_MovePickPlace::State_Alarm()
{
	CTaskTrayFeeder* pTask = ( CTaskTrayFeeder* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTask->MakeLog( "[CMD: %s] [ALARM] [Step=%8d]", m_szCmdName, m_nStep );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			int err = ERR_NO_ERROR;
			for( int i = 0; i < CTaskTrayFeeder::eAxisMax; i++ ) {
				OneAxis* pMotor = pTask->GetAxis( ( CTaskTrayFeeder::eMotor )i );
				if (pMotor != nullptr) {
					int stop_result = pMotor->SetSlowStop();
					if (stop_result != ERR_NO_ERROR) {
						err = stop_result;
					}
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

	return CBaseSeqCmd::State_Alarm();
}

BOOL CTrayFeederCmd_MovePickPlace::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete();
}

BOOL CTrayFeederCmd_MovePickPlace::State_Aborting()
{
	CTaskTrayFeeder* pTask = ( CTaskTrayFeeder* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTask->MakeLog( "[CMD: %s] [ABORT] [Step=%8d]", m_szCmdName, m_nStep );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
			int err = ERR_NO_ERROR;
			for( int i = 0; i < CTaskTrayFeeder::eAxisMax; i++ ) {
				OneAxis* pMotor = pTask->GetAxis( ( CTaskTrayFeeder::eMotor )i );
				if (pMotor != nullptr) {
					int stop_result = pMotor->SetSlowStop();
					if (stop_result != ERR_NO_ERROR) {
						err = stop_result;
					}
				}
				
			}

			m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
			m_swTimeout.StartTimer();
			m_nStep++;
		}break;
	case 1:
		{
			if( m_swTimeout.CheckOverTime() ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Feeder Motor Stop Timeout." );
				ReportAlarm( ERR_FEEDER_MOTOR_STOP_TIMEOUT, AlarmData );
				break;
			}

			int motion_done_cnt = 0;
			for( int i = 0; i < CTaskTrayFeeder::eAxisMax; i++ ) {
				OneAxis* pMotor = pTask->GetAxis( ( CTaskTrayFeeder::eMotor )i );
				if (pMotor == nullptr) {
					motion_done_cnt++;
					continue;
				}
				motion_done_cnt += pMotor->MotionDone() == ERR_NO_ERROR ? 1 : 0;
			}

			if( motion_done_cnt == CTaskTrayFeeder::eAxisMax ) {
				m_nStep++;
				break;
			}
		}break;
	case 2:
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

BOOL CTrayFeederCmd_MovePickPlace::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}

CString CTrayFeederCmd_MovePickPlace::act_string( int act )
{
	BEGIN_CASE_TO_STR( act );
	CASE_TO_STR( start );
	CASE_TO_STR(do_chk_up_cyl_start);
	CASE_TO_STR(do_chk_Interlock_before_move);
	CASE_TO_STR( do_move );
	CASE_TO_STR( chk_move );
	CASE_TO_STR( do_chk_down_cyl );

	CASE_TO_STR(pick_unclamp);
	CASE_TO_STR(pick_before_conv_align_back);
	CASE_TO_STR(pick_conv_align);
	CASE_TO_STR(pick_clamp);
	CASE_TO_STR(pick_after_conv_align_back);
	CASE_TO_STR(pick_do_chk_up_cyl_start);
	CASE_TO_STR(pick_align);
	CASE_TO_STR(pick_align_back);

	CASE_TO_STR( place_dealign );
	CASE_TO_STR(place_down_cyl);
	CASE_TO_STR( place_unclamp );
	CASE_TO_STR( do_chk_up_cyl_end );

	CASE_TO_STR(transfer_area_sensor_chk);

	CASE_TO_STR(check_tray);

	CASE_TO_STR( end );
	ELSE_CASE( _T( "unkown_step" ) );
	END_CASE_TO_STR();
}
