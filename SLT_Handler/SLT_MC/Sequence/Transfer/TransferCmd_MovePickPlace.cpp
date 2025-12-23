#include "stdafx.h"
#include "TransferCmd_MovePickPlace.h"


CTransferCmd_MovePickPlace::CTransferCmd_MovePickPlace( int nTargetConveyorIndex, int nPickPlace )
	: m_nTargetConveyorIndex( nTargetConveyorIndex ), m_nPickPlace( nPickPlace )
{
	m_nLast_Do_Step = 0;
	StringCchCopy( m_szCmdName, _countof( m_szCmdName ), CString( __func__ ) );
}

CTransferCmd_MovePickPlace::~CTransferCmd_MovePickPlace()
{
}

int CTransferCmd_MovePickPlace::CheckParam()
{
	m_swTimeout.SetCheckTime(TIME_OUT_MIDDLE);
	m_swTimeout.StartTimer();

	m_TrayInterlock.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
	m_TrayInterlock.SetTimerCheckEnd();

	m_nWorkCmdStage = m_nTargetConveyorIndex;

	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTransferCmd_MovePickPlace::OnStop()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Aborting );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTransferCmd_MovePickPlace::OnRetry()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTransferCmd_MovePickPlace::OnSkip()
{
	ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_AlarmComplete );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTransferCmd_MovePickPlace::OnPause()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Pause );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTransferCmd_MovePickPlace::OnResume()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CTransferCmd_MovePickPlace::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}


//==============================================================================
//
//==============================================================================
BOOL CTransferCmd_MovePickPlace::State_Executing()
{
	// check alarm
	CTaskTransfer* pTask = ( CTaskTransfer* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		//pTask->MakeLog( _T( "[CMD: %s] [EXECUTE] [Step=%8d]" ), m_szCmdName, m_nStep );
	}
	m_nStep_Pre = m_nStep;


	if (g_TaskTrayUnloadPP.is_safety_state() == false) {

		if (m_TrayInterlock.IsChecking() == FALSE)
		{
			m_TrayInterlock.StartTimer();
		}

		if (m_TrayInterlock.CheckOverTime() == TRUE)
		{
			m_TrayInterlock.SetTimerCheckEnd();
			CParamList AlarmData;
			ReportAlarm(ERR_TRNASFER_UNIT_OPER_FAIL_BECAUSE_TRAY_PP_IS_NOT_SAFETY, AlarmData);
		}
		return FALSE;
	}
	else {
		m_TrayInterlock.SetTimerCheckEnd();
	}
	
	

	//bool bTrayPPSafety = g_TaskTrayPP[m_eTrayIdx].is_safety_state();
	//if( bTrayPPSafety != true ) {
	//	int nAlid = ERR_TRNASFER_UNIT_OPER_FAIL_BECAUSE_TRAY_PP_IS_NOT_SAFETY;
	//	CParamList AlarmData;
	//	ReportAlarm( nAlid, AlarmData );
	//	return CBaseSeqCmd::State_Executing();
	//}



	switch( m_nStep )
	{
		case eExcutingStep_Start: // check interlock
			{
				// Pick일 때는 Tray가 감지되지 않아야 함
				pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] start check data (STR)"), m_szCmdName, m_nStep);
				if( m_nPickPlace == DEF_PICK ) {
					if( pTask->ChkTrayExist( DEF_NOT_EXIST ) != ERR_NO_ERROR ) {
						CParamList AlarmData;
						ReportAlarm( ERR_TRANSFER_CANNOT_PICK_TRAY_CUZ_TRAY_IS_EXIST, AlarmData );
						break;
					}
				}
				// Place일 때는 Tray가 있어야 함 (이건 없어도 무방할 듯..)
				else {
					if( pTask->ChkTrayExist( DEF_EXIST ) != ERR_NO_ERROR ) {
						CParamList AlarmData;
						ReportAlarm( ERR_TRANSFER_CANNOT_PLACE_TRAY_CUZ_TRAY_IS_NOT_EXIST, AlarmData );
						break;
					}
				}
				pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] start check data (END)"), m_szCmdName, m_nStep);

				m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep = eExcutingStep_Do_Pre_ClampUnClamp;

				pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] cylinder clamp/unlcamp = %s (STR)"), m_szCmdName, m_nStep, (m_nPickPlace == DEF_PICK?_T("CLAMP") : _T("UNCLAMP")) );
			}break;

		case eExcutingStep_Do_Pre_ClampUnClamp: // 이동 전에 clamp 상태 확인 한다.
			{
				// pick 동작이면 unclamp되어 있여야하고, place 동작이면 clamp 시킴.
				int nClampUnclamp = m_nPickPlace == DEF_PICK ? DEF_UNCLAMP : DEF_CLAMP;

				if( m_swTimeout.CheckOverTime() == TRUE )
				{
					int nAlid = nClampUnclamp == DEF_CLAMP ? ERR_TRNASFER_CLAMP_TIMEOUT : ERR_TRNASFER_UNCLAMP_TIMEOUT;
					CParamList AlarmData;
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				pTask->do_cyl_ClampUnclamp( nClampUnclamp );
				int nErr = pTask->chk_cyl_ClampUnclamp( nClampUnclamp );
				if( nErr != ERR_NO_ERROR ) {
					break;
				}

				pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] cylinder clamp/unlcamp = %s (END)"), m_szCmdName, m_nStep, (m_nPickPlace == DEF_PICK ? _T("CLAMP") : _T("UNCLAMP")));
				m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep = eExcutingStep_Do_Pre_Z_Up;
				pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] z cylnider up (STR)"), m_szCmdName, m_nStep);
			}break;

		// 이동하기 전에 항상 Z cylinder Safety 위치로 이동시킨다
		case eExcutingStep_Do_Pre_Z_Up:
			{
				if( m_swTimeout.CheckOverTime() == TRUE )
				{
					CParamList AlarmData;
					ReportAlarm( ERR_TRNASFER_UP_TIMEOUT, AlarmData );
					break;
				}

				pTask->do_cyl_UpDown( DEF_UP );
				int nErr = pTask->chk_cyl_UpDown( DEF_UP );
				if( nErr != ERR_NO_ERROR ) {
					break;
				}
				pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] z cylnider up (END)"), m_szCmdName, m_nStep);
				m_nLast_Do_Step = m_nStep;
				m_nStep = eExcutingStep_Move_X;
			}break;
		// Move X Axis
		case eExcutingStep_Move_X:
			{
				pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] %d conveyor move x axis  (STR)"), m_szCmdName, m_nStep, m_nTargetConveyorIndex+1);
				int nErr = ERR_NO_ERROR;
				nErr = pTask->do_MovePos( CTaskTransfer::AXIS_TRANSFER_X, ( eLocIdxTransfer )m_nTargetConveyorIndex );
				if( nErr != ERR_NO_ERROR )
				{
					CParamList AlarmData;
					OneAxis* pMotor = pTask->GetAxis(CTaskTransfer::AXIS_TRANSFER_X);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm( nErr, AlarmData );
					break;
				}

				m_swTimeout.SetCheckTime( TIME_OUT_MIDDLE * 2 );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep = eExcutingStep_WaitEnd_Move_X;
			}break;
		case eExcutingStep_WaitEnd_Move_X:
			{
				int nAxisDone = pTask->ChkAxisDone( CTaskTransfer::AXIS_TRANSFER_X );
				OneAxis* pMotor = pTask->GetAxis(CTaskTransfer::AXIS_TRANSFER_X);
				if( nAxisDone == ERR_NO_ERROR) {
					pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] %d conveyor move x axis  (END)"), m_szCmdName, m_nStep, m_nTargetConveyorIndex + 1);

					// Place check tray after move
					pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] place tray check(STR)"), m_szCmdName, m_nStep);
					if (m_nPickPlace == DEF_PLACE) {
						if (pTask->ChkTrayExist(DEF_EXIST) != ERR_NO_ERROR) {
							CParamList AlarmData;
							ReportAlarm(ERR_TRANSFER_CANNOT_PLACE_TRAY_CUZ_TRAY_IS_NOT_EXIST, AlarmData);
							break;
						}
					}
					pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] place tray check(END)"), m_szCmdName, m_nStep);

					m_swTimeout.SetCheckTime( TIME_OUT_MIDDLE / 2 );
					m_swTimeout.StartTimer();
					m_nStep = eExcutingStep_Check_Conveyor_State;

					pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] %d conveyor check stat  (STR)"), m_szCmdName, m_nStep, m_nTargetConveyorIndex + 1);
					break;
				}
				else
				{
					if (m_swTimeout.CheckOverTime() == TRUE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(ERR_TRANSFER_X_AXIS_MOVE_TIME_OUT, AlarmData);
						break;
					}


					if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(ERR_TRANSFER_X_AXIS_MOVE_FAIL, AlarmData);
						break;
					}
				}
			}break;

		case eExcutingStep_Check_Conveyor_State:
			{
				if( m_swTimeout.CheckOverTime() == TRUE )
				{
					CParamList AlarmData;
					ReportAlarm( ERR_TRNASFER_TARGET_CONV_NOT_READY, AlarmData );
					break;
				}

				BOOL vat_in_process = g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_USE);
				if (vat_in_process && g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
					m_swTimeout.SetCheckTime(TIME_OUT_SHORT);
					m_swTimeout.StartTimer();
					m_nStep = eExcutingStep_Do_Pre_Tray_Align_ForBack;
					break;
				}

				eStacker_Conveyor_State state = g_TaskAutoStacker[ m_nTargetConveyorIndex ].GetConveyorState();
				if( state == ReadyToLoad_Tray || state == ReadyToUnload_Tray || CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Manual ) {
					pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] %d conveyor check stat  (END)"), m_szCmdName, m_nStep, m_nTargetConveyorIndex + 1);

					m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
					m_swTimeout.StartTimer();
					m_nStep = eExcutingStep_Do_Pre_Tray_Align_ForBack;

					pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] %d conveyor c-tray align cylinder for/back=%s  (STR)"), m_szCmdName, m_nStep, m_nTargetConveyorIndex + 1, (m_nPickPlace == DEF_PICK ? "FORW" : "BACK") );
					break;
				}

			}break;

		case eExcutingStep_Do_Pre_Tray_Align_ForBack:
			{
				int nForBack = m_nPickPlace == DEF_PICK ? DEF_FORW : DEF_BACK;

				if( g_DMCont.m_dmHandlerOpt.GN( NDM11_SYS_MD_DEVICE_CHK ) == eNoDeviceNoTray && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_ )
					nForBack = DEF_BACK;

				if( m_swTimeout.CheckOverTime() == TRUE )
				{
					CParamList AlarmData;
					ReportAlarm( nForBack == DEF_FORW ? ERR_STACKER_1_TRAY_ALIGN_FWD_TIMEOUT + m_nTargetConveyorIndex : ERR_STACKER_1_TRAY_ALIGN_BKWD_TIMEOUT + m_nTargetConveyorIndex, AlarmData );
					break;
				}

				if (m_nTargetConveyorIndex != eSTACKER_03)
				{
					g_TaskStacker[m_nTargetConveyorIndex].do_cyl_TrayAlign(nForBack);
					int nErr = g_TaskStacker[m_nTargetConveyorIndex].chk_cyl_TrayAlign(nForBack);
					if (nErr != ERR_NO_ERROR)
						break;
				}

				Sleep( 100 );//imsi

				pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] %d conveyor c-tray align cylinder for/back=%s  (END)"), m_szCmdName, m_nStep, m_nTargetConveyorIndex + 1, (m_nPickPlace == DEF_PICK ? "FORW" : "BACK"));

				m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);				
				if (nDeviceMode != eNoDeviceNoTray)
				{
					m_nStep = eExcutingStep_Check_Conveyor_State_Retry;
					pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] %d conveyor check stat  (STR)"), m_szCmdName, m_nStep, m_nTargetConveyorIndex + 1);
					break;
				}
				else
				{
					m_nStep = eExcutingStep_Do_Z_Down;
					pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] z cylnider down (STR)"), m_szCmdName, m_nStep);
					break;
				}
					
			}break;

		case eExcutingStep_Check_Conveyor_State_Retry:
		{
			if (m_swTimeout.CheckOverTime() == TRUE)
			{
				CParamList AlarmData;
				ReportAlarm(ERR_TRNASFER_TARGET_CONV_NOT_READY, AlarmData);
				break;
			}

			BOOL vat_in_process = g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_USE);
			if (vat_in_process && g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
				m_swTimeout.SetCheckTime(TIME_OUT_SHORT);
				m_swTimeout.StartTimer();
				m_nStep = eExcutingStep_Do_Z_Down;
				break;
			}


			eStacker_Conveyor_State state = g_TaskAutoStacker[m_nTargetConveyorIndex].GetConveyorState();
			if (state == ReadyToLoad_Tray || state == ReadyToUnload_Tray || CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Manual) {
				pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] %d conveyor check stat  (END)"), m_szCmdName, m_nStep, m_nTargetConveyorIndex + 1);

				m_swTimeout.SetCheckTime(TIME_OUT_SHORT);
				m_swTimeout.StartTimer();
				m_nStep = eExcutingStep_Do_Z_Down;
				pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] z cylnider down (STR)"), m_szCmdName, m_nStep);
				break;
			}

		}break;

		// Move Z down
		case eExcutingStep_Do_Z_Down:
			{
				if( m_swTimeout.CheckOverTime() == TRUE )
				{
					CParamList AlarmData;
					ReportAlarm( ERR_TRNASFER_DOWN_TIMEOUT, AlarmData );
					break;
				}

				pTask->do_cyl_UpDown( DEF_DOWN );
				int nErr = pTask->chk_cyl_UpDown( DEF_DOWN );
				if( nErr != ERR_NO_ERROR ) {
					break;
				}

				Sleep( 100 );//imsi

				m_nLast_Do_Step = m_nStep;
				m_nStep = eExcutingStep_Do_ClampUnclamp;
				m_swTimeout.SetCheckTime(TIME_OUT_MIDDLE / 2);
				m_swTimeout.StartTimer();
				pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] z cylnider down (END)"), m_szCmdName, m_nStep);

				pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] cylinder clamp/uncalmp=%s (STR)"), m_szCmdName, m_nStep, (m_nPickPlace == DEF_PICK ? "CLAMP" : "UNCLAMP"));
			}break;
		// Tray Clamp or Unclamp

		case eExcutingStep_Do_ClampUnclamp:
			{
				int nClampUnclamp = m_nPickPlace == DEF_PICK ? DEF_CLAMP : DEF_UNCLAMP;

				if( m_swTimeout.CheckOverTime() == TRUE )
				{
					int nAlid = nClampUnclamp == DEF_CLAMP ? ERR_TRNASFER_CLAMP_TIMEOUT : ERR_TRNASFER_UNCLAMP_TIMEOUT;
					CParamList AlarmData;
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				pTask->do_cyl_ClampUnclamp( nClampUnclamp );
				int nErr = pTask->chk_cyl_ClampUnclamp( nClampUnclamp );
				if( nErr != ERR_NO_ERROR ) {
					break;
				}

				pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] cylinder clamp/uncalmp=%s (END)"), m_szCmdName, m_nStep, (m_nPickPlace == DEF_PICK ? "CLAMP" : "UNCLAMP"));
				m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;

				if(m_nTargetConveyorIndex == eSTACKER_03)
					m_nStep = eExcutingStep_Do_Z_Up;
				else {
					BOOL vat_in_process = g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_USE);

					m_nStep =(vat_in_process == TRUE) ? eExcutingStep_Move_Conveyor_for_Jig : eExcutingStep_Do_Tray_Align_ForBack;
				}

				pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] c-tray align cylinder for/back=%s (STR)"), m_szCmdName, m_nStep, (m_nPickPlace == DEF_PICK ? "BACK" : "FORW"));
			}break;
		case eExcutingStep_Move_Conveyor_for_Jig:
			{
				if (m_nPickPlace == DEF_PICK) {
					m_nStep = eExcutingStep_Do_Tray_Align_ForBack;
					break;
				}

				int dest_stacker_idx = g_DMCont.m_dmVAT.GN(NDM14_VAT_TRANSFER_DESTINATION_INDEX);
				int nErr = g_TaskStacker[dest_stacker_idx].ConveyorMoving(eConveyorMove_FWD, false);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), dest_stacker_idx + 1);
					ReportAlarm(ERR_STACKER_1_INTERLOCK_CONVEYOR_DRIVER_NOT_WORK + dest_stacker_idx, AlarmData);
					break;
				}

				m_nStep = eExcutingStep_Do_Tray_Align_ForBack;
			}break;

		case eExcutingStep_Do_Tray_Align_ForBack:
			{
				int nForBack = m_nPickPlace == DEF_PICK ? DEF_BACK : DEF_FORW;


				//if( g_DMCont.m_dmHandlerOpt.GN( NDM11_SYS_MD_DEVICE_CHK ) == eNoDeviceNoTray && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_ )
				//{
				//	// tray가 없을 경우 align left랑 transfer tray cover 간섭
				//	int nErrConveyorTrayExist = g_TaskStacker[ m_nTargetConveyorIndex ].ChkExistTray_RearPos( DEF_NOT_EXIST );
				//	if( nErrConveyorTrayExist == ERR_NO_ERROR )
				//		nForBack = DEF_BACK;
				//}

				if( m_swTimeout.CheckOverTime() == TRUE )
				{
					CParamList AlarmData;
					ReportAlarm( nForBack == DEF_FORW ? ERR_STACKER_1_TRAY_ALIGN_FWD_TIMEOUT + m_nTargetConveyorIndex : ERR_STACKER_1_TRAY_ALIGN_BKWD_TIMEOUT + m_nTargetConveyorIndex, AlarmData );
					break;
				}

				if (m_nTargetConveyorIndex != eSTACKER_03)
				{
					g_TaskStacker[m_nTargetConveyorIndex].do_cyl_TrayAlign(nForBack);
					int nErr = g_TaskStacker[m_nTargetConveyorIndex].chk_cyl_TrayAlign(nForBack);
					if (nErr != ERR_NO_ERROR)
						break;
				}

				BOOL vat_in_process = g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_USE);
				if (vat_in_process) {
					int dest_stacker_idx = g_DMCont.m_dmVAT.GN(NDM14_VAT_TRANSFER_DESTINATION_INDEX);
					g_TaskStacker[dest_stacker_idx].ConveyorStop();
				}

				pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] c-tray align cylinder for/back=%s (END)"), m_szCmdName, m_nStep, (m_nPickPlace == DEF_PICK ? "BACK" : "FORW"));
				m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep = eExcutingStep_Do_Z_Up;
				pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] z cylnider up (STR)"), m_szCmdName, m_nStep);
			}break;

		case eExcutingStep_Do_Z_Up:
			{
				if( m_swTimeout.CheckOverTime() == TRUE )
				{
					CParamList AlarmData;
					ReportAlarm( ERR_TRNASFER_UP_TIMEOUT, AlarmData );
					break;
				}

				pTask->do_cyl_UpDown( DEF_UP );
				int nErr = pTask->chk_cyl_UpDown( DEF_UP );
				if( nErr != ERR_NO_ERROR ) {
					break;
				}

				Sleep( 100 );//imsi
				pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] z cylnider up (END)"), m_szCmdName, m_nStep);

				m_nLast_Do_Step = m_nStep;
				m_nStep = eExcutingStep_Check_Data;
			}break;

		case eExcutingStep_Check_Data:
			{
				BOOL bTrayDataExist = g_DMCont.m_dmDevice.GetTrayExist( eDeviceStage_Transfer );
				int  nTrayChk       = g_TaskTransfer.ChkTrayExist( (m_nPickPlace == DEF_PICK ? DEF_EXIST : DEF_NOT_EXIST) );

				if( ( m_nPickPlace == DEF_PICK && bTrayDataExist == TRUE && nTrayChk == ERR_NO_ERROR)
					|| ( m_nPickPlace == DEF_PLACE && bTrayDataExist == FALSE && nTrayChk == ERR_NO_ERROR) ) {
					m_nStep = eExcutingStep_End;
					break;
				}

				m_nStep = eExcutingStep_Check_Sensor_And_Process_Data;
			}break;

		case eExcutingStep_Check_Sensor_And_Process_Data:
			{
				if( pTask->GetEqpCtrlMode() == eEqpCtrlMode_Auto )
				{
					pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] check data (STR)"), m_szCmdName, m_nStep);
					// Pick일 때는 Tray가 감지되어야 함.
					if( m_nPickPlace == DEF_PICK ) {
						if (pTask->ChkTrayExist(DEF_EXIST) != ERR_NO_ERROR) {
							CParamList AlarmData;
							ReportAlarm(ERR_TRNASFER_AFTER_PICKING_TRAY_IS_NOT_EXIST_AT_TRANSFER_HEAD, AlarmData);
							break;
						}
						int nErrConveyorTrayExist = -1;
						if (m_nTargetConveyorIndex == eSTACKER_03) {
							nErrConveyorTrayExist = g_TaskTrayFeeder.ChkTrayExist_TransferArea_Sen(eNot_Exist);			
						}
						else {
							nErrConveyorTrayExist = g_TaskStacker[m_nTargetConveyorIndex].ChkExistTray_RearPos_Sen(DEF_NOT_EXIST);
						}
						
						if( nErrConveyorTrayExist != ERR_NO_ERROR ) {
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("[Conveyor %d]", m_nTargetConveyorIndex + 1);
							ReportAlarm( ERR_TRNASFER_AFTER_PICKING_TRAY_IS_EXIST_ON_CONVEYOR, AlarmData );
							break;
						}

						if( pTask->GetEqpCtrlMode() == eEqpCtrlMode_Auto )
						{
							if (g_TaskAutoStacker[m_nTargetConveyorIndex].GetStackerType() == eStackerType_Load )
							{
								g_TaskTrayFeeder.ChkTrayExist_TransferArea_Sen(eNot_Exist);
								g_DMCont.m_dmDevice.SwapStageData(eDeviceStage_STACKER_CONV_3_TRANSFER_AREA, eDeviceStage_Transfer);
							}
							else
							{
								g_DMCont.m_dmDevice.SwapStageData(eDeviceStage_STACKER_CONV_1 + m_nTargetConveyorIndex, eDeviceStage_Transfer);
							}
						}
					}
					else if( m_nPickPlace == DEF_PLACE ) {
						// Place일 때는 Tray가 conveyor 위에서 감지되어야 함.
						if (pTask->ChkTrayExist(DEF_NOT_EXIST) != ERR_NO_ERROR) {
							CParamList AlarmData;
							ReportAlarm(ERR_TRNASFER_AFTER_PLACING_TRAY_IS_EXIST_AT_TRANSFER_HEAD, AlarmData);
							break;
						}
						int nErrConveyorTrayExist = g_TaskStacker[ m_nTargetConveyorIndex ].ChkExistTray_RearPos_Sen( DEF_EXIST );
						if( nErrConveyorTrayExist != ERR_NO_ERROR ) {
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("[Conveyor %d]", m_nTargetConveyorIndex+1);
							ReportAlarm( ERR_TRNASFER_AFTER_PLACING_TRAY_IS_NOT_EXIST_ON_CONVEYOR, AlarmData );
							break;
						}

						if( pTask->GetEqpCtrlMode() == eEqpCtrlMode_Auto )
							g_DMCont.m_dmDevice.SwapStageData( eDeviceStage_Transfer, eDeviceStage_STACKER_CONV_1 + m_nTargetConveyorIndex );
					}
					else {
						ASSERT( false );
					}
					pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] check data (END)"), m_szCmdName, m_nStep);
				}

				m_nStep = eExcutingStep_End;
			}break;

		case eExcutingStep_End:
			{
				ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_NormalComplete );
			}break;
	}
	return CBaseSeqCmd::State_Executing();
}


//==============================================================================
//
//==============================================================================
BOOL CTransferCmd_MovePickPlace::State_Pause()
{
	CTaskTransfer* pTask = ( CTaskTransfer* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTask->MakeLog( _T( "[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]" ), m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
		case 0:
			{
				int nAxisCount = CTaskTransfer::eMaxAxisCount;
				for( int i = 0; i < nAxisCount; i++ ) {
					OneAxis* pMotor = ( OneAxis* )pTask->GetAxis( i );
					if( pMotor != NULL )
					{
						//pMotor->SetSlowStop();
						bool bCloseLimit = pMotor->CheckLimitClose();
						double dCurPos = 0;
						pMotor->GetCurrentPos(dCurPos);
						if (bCloseLimit == false)
						{
							pTask->MakeLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
							pMotor->SetSlowStop();
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

	return CBaseSeqCmd::State_Pause();
}

//==============================================================================
//
//==============================================================================
BOOL CTransferCmd_MovePickPlace::State_Recovery()
{
	CTaskTransfer* pTask = ( CTaskTransfer* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTask->MakeLog( _T( "[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%d]" ), m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;


	if (g_TaskTrayUnloadPP.is_safety_state() == false) {

		if (m_TrayInterlock.IsChecking() == FALSE)
		{
			m_TrayInterlock.StartTimer();
		}

		if (m_TrayInterlock.CheckOverTime() == TRUE)
		{
			m_TrayInterlock.SetTimerCheckEnd();
			CParamList AlarmData;
			ReportAlarm(ERR_TRNASFER_UNIT_OPER_FAIL_BECAUSE_TRAY_PP_IS_NOT_SAFETY, AlarmData);
		}
		return FALSE;
	}
	else {
		m_TrayInterlock.SetTimerCheckEnd();
	}
	

	switch( m_nStep )
	{
		case 0:
			{
				pTask->do_cyl_UpDown( DEF_UP );
				m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
				m_swTimeout.StartTimer();
				m_nStep = 1;
			}break;
		case 1:
			{
				if( m_swTimeout.CheckOverTime() == TRUE )
				{
					CParamList AlarmData;
					ReportAlarm( ERR_TRNASFER_UP_TIMEOUT, AlarmData );
					break;
				}
				int nErr = pTask->chk_cyl_UpDown( DEF_UP );
				if( nErr != ERR_NO_ERROR ) {
					break;
				}
				m_nStep = 500;
			}break;
		case 500:
			{
				m_swTimeout.SetCheckTime(TIME_OUT_MIDDLE);
				m_swTimeout.StartTimer();
				m_nStep = 501;
			}break;
		case 501:  // Stacker Cmd 상태가 Ready 인지 확인 한다. Stacker Vibrator 동작 중 이면, Cmd 종료 되지 않는 문제로 Tray Unload 되지 않음.
			{
				if (m_swTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Transfer Cmd:%s, Conveyor%d")
						, m_nPickPlace == DEF_PICK ? _T("Pick") : _T("Place")
						, m_nTargetConveyorIndex + 1);
					ReportAlarm(ERR_TRNASFER_STACKER_CMD_NOT_COMPLETE_TIMEOUT, AlarmData);
					break;
				}

				if (g_TaskStacker[m_nTargetConveyorIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
				{
					m_nStep = 1000;
				}
			}break;
		case 1000:
			{
				int nErrTransferTray = pTask->ChkTrayExist( (m_nPickPlace == DEF_PICK ? DEF_EXIST : DEF_NOT_EXIST) );
				int nErrConveyorTray = g_TaskStacker[m_nTargetConveyorIndex].ChkExistTray_RearPos_Sen( (m_nPickPlace == DEF_PICK ? DEF_NOT_EXIST : DEF_EXIST ) );

				//Stacker#3 Conveyor Rear영역이 아니라 Transfer Area영역으로 처리.
				if (m_nTargetConveyorIndex == eSTACKER_03)
				{
					nErrConveyorTray = g_TaskTrayFeeder.GetTrayExist_TransferArea_Sen(eExist);
					if (pTask->GetEqpCtrlMode() == eEqpCtrlMode_Auto)
					{
						bool bData_Exist_TransferArea = g_TaskTrayFeeder.GetTrayExist_TransferArea_Data();
						if (bData_Exist_TransferArea != true)
						{
							nErrConveyorTray = -1;
						}
					}
// 					bool bSensor_Exist_TransferArea = g_TaskTrayFeeder.GetTrayExist_TransferArea_Sen(eExist) == ERR_NO_ERROR;
// 					if (bData_Exist_TransferArea == true && bSensor_Exist_TransferArea == true)
// 					{
// 						nErrConveyorTray = ERR_NO_ERROR;
// 					}
// 					else
// 					{
// 						nErrConveyorTray = -1;
// 					}
				}

				bool bDeviceFullTray = g_TaskAutoStacker[m_nTargetConveyorIndex].ConveyorIsFullTray();
			
				if( ( m_nPickPlace == DEF_PICK && nErrTransferTray == ERR_NO_ERROR && nErrConveyorTray == ERR_NO_ERROR )
					|| ( m_nPickPlace == DEF_PLACE && nErrTransferTray == ERR_NO_ERROR && nErrConveyorTray == ERR_NO_ERROR && bDeviceFullTray != true) )
				{
					// Place 조건 추가  - TargetConveyorIndex가 Device Full Tray면 eExcutingStep_Do_ClampUnclamp Step으로 이동 하지 못하도록 변경
					// Bin Double 방지 
					m_nLast_Do_Step = eExcutingStep_Do_ClampUnclamp;
					m_nStep = 99999;
					break;
				}
				else if( ( m_nPickPlace == DEF_PICK && nErrTransferTray != ERR_NO_ERROR && nErrConveyorTray != ERR_NO_ERROR )
						 || ( m_nPickPlace == DEF_PLACE && nErrTransferTray != ERR_NO_ERROR && nErrConveyorTray != ERR_NO_ERROR ) )
				{
					m_nLast_Do_Step = eExcutingStep_Start;
					m_nStep = 99999;
					break;
				}
				else if (m_nTargetConveyorIndex == eSTACKER_03 && m_nPickPlace == DEF_PICK && nErrTransferTray != ERR_NO_ERROR && nErrConveyorTray == ERR_NO_ERROR)
				{
					m_nLast_Do_Step = eExcutingStep_Start;
					m_nStep = 99999;
					break;
				}
				else if (m_nTargetConveyorIndex == eSTACKER_03 && m_nPickPlace == DEF_PICK && nErrTransferTray == ERR_NO_ERROR && nErrConveyorTray != ERR_NO_ERROR)
				{
					m_nLast_Do_Step = eExcutingStep_Do_Z_Up;
					m_nStep = 99999;
				}

				//int nErrNotExist = pTask->GetTrayExist_DvcMode(DEF_NOT_EXIST);
				//int nErrExist = pTask->GetTrayExist_DvcMode(DEF_EXIST);
				//int nErrConveyorTrayExist = g_TaskStacker[ m_nTargetConveyorIndex ].GetExistTray_RearPos_DvcMode( DEF_EXIST );
				//int nErrConveyorTrayNotExist = g_TaskStacker[ m_nTargetConveyorIndex ].GetExistTray_RearPos_DvcMode( DEF_NOT_EXIST );

				//if( ( m_nPickPlace == DEF_PICK && nErrExist == ERR_NO_ERROR && nErrConveyorTrayNotExist == ERR_NO_ERROR )
				//	|| ( m_nPickPlace == DEF_PLACE && nErrNotExist == ERR_NO_ERROR && nErrConveyorTrayExist == ERR_NO_ERROR ) )
				//{
				//	m_nLast_Do_Step = eExcutingStep_Do_ClampUnclamp;
				//	m_nStep = 99999;
				//	break;
				//}
				//else if( ( m_nPickPlace == DEF_PICK && nErrNotExist == ERR_NO_ERROR && nErrConveyorTrayExist == ERR_NO_ERROR )
				//		 || ( m_nPickPlace == DEF_PLACE && nErrExist == ERR_NO_ERROR && nErrConveyorTrayNotExist == ERR_NO_ERROR ) )
				//{
				//	m_nLast_Do_Step = eExcutingStep_Start;
				//	m_nStep = 99999;
				//	break;
				//}
				else {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( _T( "Transfer Cmd:%s, HeadTray:%s, Conveyor%d:%s" )
												 , m_nPickPlace == DEF_PICK ? _T( "Pick" ) : _T( "Place" )
												 //, nErrTransferTray == ERR_NO_ERROR ? _T( "Exist" ) : _T( "NotExist" )
												 , m_nPickPlace == DEF_PICK ? (nErrTransferTray == ERR_NO_ERROR ? _T("Exist") : _T("NotExist")) : (nErrTransferTray == ERR_NO_ERROR ? _T("NotExist") : _T("Exist"))
												 , m_nTargetConveyorIndex + 1
												 , m_nPickPlace == DEF_PICK ? (nErrConveyorTray==ERR_NO_ERROR ? _T("NotExist") : _T("Exist")) : (nErrConveyorTray == ERR_NO_ERROR ? _T("Exist") : _T("NotExist")) );
												 //, nErrConveyorTrayNotExist == ERR_NO_ERROR ? _T( "NotExist" ) : _T( "Exist" ) );

					ReportAlarm( ERR_LOT_CMD_TRANSFER_SENSOR_DATA_MISMATCH, AlarmData );
					break;
				}
			}break;

		case 99999:
			{
				m_swTimeout.ResetStartTime();
				SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Executing, m_nLast_Do_Step );
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
BOOL CTransferCmd_MovePickPlace::State_NormalComplete()
{
	CTaskTransfer* pTask = ( CTaskTransfer* )m_pReceive;
	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CTransferCmd_MovePickPlace::State_Alarm()
{
	CTaskTransfer* pTask = ( CTaskTransfer* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTask->MakeLog( _T( "[CMD: %s] [ALARM] [Step=%8d] [LastStep=%d]" ), m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
		case 0: // alarm 발생 했을 때 처리해 줘야 할꺼 처리 해줌
			{
				int nAxisCount = CTaskTransfer::eMaxAxisCount;
				for( int i = 0; i < nAxisCount; i++ ) {
					OneAxis* pMotor = ( OneAxis* )pTask->GetAxis( i );
					if( pMotor != NULL )
					{
						//pMotor->SetSlowStop();
						bool bCloseLimit = pMotor->CheckLimitClose();
						double dCurPos = 0;
						pMotor->GetCurrentPos(dCurPos);
						if (bCloseLimit == false)
						{
							pTask->MakeLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
							pMotor->SetSlowStop();
						}
					}
				}
				m_nStep++;
			}break;
		case 1:	// Retry, Skip을 기다림
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
BOOL CTransferCmd_MovePickPlace::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CTransferCmd_MovePickPlace::State_Aborting()
{
	CTaskTransfer* pTask = ( CTaskTransfer* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTask->MakeLog( _T( "[CMD: %s] [ABORT] [Step=%8d]" ), m_szCmdName, m_nStep );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
		case 0:
			{
				int nAxisCount = CTaskTransfer::eMaxAxisCount;
				for( int i = 0; i < nAxisCount; i++ ) {
					OneAxis* pMotor = ( OneAxis* )pTask->GetAxis( i );
					if( pMotor != NULL )
					{
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

//==============================================================================
//
//==============================================================================
BOOL CTransferCmd_MovePickPlace::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}


