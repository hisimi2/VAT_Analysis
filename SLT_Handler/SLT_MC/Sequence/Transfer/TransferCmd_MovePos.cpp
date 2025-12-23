#include "stdafx.h"
#include "TransferCmd_MovePos.h"

CTransferCmd_MovePos::CTransferCmd_MovePos( eLocIdxTransfer eTeachPos, int nSpeedRate/* = 100*/ )
	: m_eTeachPos( eTeachPos ), m_nSpdRate( nSpeedRate )
{
	m_nLast_Do_Step = 0;
	StringCchCopy( m_szCmdName, _countof( m_szCmdName ), CString( __func__ ) );
}

CTransferCmd_MovePos::~CTransferCmd_MovePos()
{
}

int CTransferCmd_MovePos::CheckParam()
{
	m_TrayInterlock.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
	m_TrayInterlock.SetTimerCheckEnd();

	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTransferCmd_MovePos::OnStop()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Aborting );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTransferCmd_MovePos::OnRetry()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTransferCmd_MovePos::OnSkip()
{
	ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_AlarmComplete );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTransferCmd_MovePos::OnPause()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Pause );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTransferCmd_MovePos::OnResume()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CTransferCmd_MovePos::State_Setup()
{
	m_swTimeout.SetCheckTime( TIME_OUT_MIDDLE );
	m_swTimeout.StartTimer();	

	return CBaseSeqCmd::State_Setup();
}


//==============================================================================
//
//==============================================================================
BOOL CTransferCmd_MovePos::State_Executing()
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

// 	for (int nIdx = 0; nIdx < eMaxTrayPPCount; nIdx++)
// 	{
// 		CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
// 
// 		if (pTaskTrayPP[nIdx]->is_safety_state() == false) {
// 
// 			if (m_TrayInterlock.IsChecking() == FALSE)
// 			{
// 				m_TrayInterlock.StartTimer();
// 			}
// 
// 			if (m_TrayInterlock.CheckOverTime() == TRUE)
// 			{
// 				m_TrayInterlock.SetTimerCheckEnd();
// 				CParamList AlarmData;
// 				ReportAlarm(ERR_TRNASFER_UNIT_OPER_FAIL_BECAUSE_TRAY_PP_IS_NOT_SAFETY, AlarmData);
// 			}
// 			return FALSE;
// 		}
// 		else {
// 			m_TrayInterlock.SetTimerCheckEnd();
// 		}
// 	}

	switch( m_nStep )
	{
		case 0: // check interlock
			{
				pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] z cylinder up (STR)"), m_szCmdName, m_nStep);
				m_nLast_Do_Step = m_nStep;
				m_nStep = 500;
			}break;
			// 이동하기 전에 항상 Z축을 Safety 위치로 이동시킨다
		case 500:
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

				pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] z cylinder up (END)"), m_szCmdName, m_nStep);
				m_nLast_Do_Step = m_nStep;
				m_nStep = 1000;
			}break;

			// Move X Axis
		case 1000:
			{
				pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] x axis move %d converyor (STR)"), m_szCmdName, m_nStep, m_eTeachPos+1);
				int nErr = ERR_NO_ERROR;
				nErr = pTask->do_MovePos( CTaskTransfer::AXIS_TRANSFER_X, m_eTeachPos, 0.0, m_nSpdRate );
				if( nErr != ERR_NO_ERROR )
				{
					CParamList AlarmData;
					OneAxis* pMotor = pTask->GetAxis(CTaskTransfer::AXIS_TRANSFER_X);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm( nErr, AlarmData );
					break;
				}

				m_swTimeout.SetCheckTime( TIME_OUT_MIDDLE);
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep++;
			}break;
		case 1001:
			{
				int nAxisDone = pTask->ChkAxisDone( CTaskTransfer::AXIS_TRANSFER_X );
				OneAxis* pMotor = pTask->GetAxis(CTaskTransfer::AXIS_TRANSFER_X);
				if( nAxisDone == ERR_NO_ERROR) {
					pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] x axis move %d converyor (END)"), m_szCmdName, m_nStep, m_eTeachPos + 1);
					m_nStep = 99999;
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

		case 99999:
			{
				pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] "), m_szCmdName, m_nStep);
				ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_NormalComplete );
			}break;
	}
	return CBaseSeqCmd::State_Executing();
}


//==============================================================================
//
//==============================================================================
BOOL CTransferCmd_MovePos::State_Pause()
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
						double dCurPos = 0;
						pMotor->GetCurrentPos(dCurPos);
						bool bCloseLimit = pMotor->CheckLimitClose();
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
BOOL CTransferCmd_MovePos::State_Recovery()
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

	switch( m_nStep )
	{
		case 0:
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
BOOL CTransferCmd_MovePos::State_NormalComplete()
{
	CTaskTransfer* pTask = ( CTaskTransfer* )m_pReceive;

	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CTransferCmd_MovePos::State_Alarm()
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
		case 0: // alarm 발생 했을 때 처리해 줘야 할꺼 처리 해줌
			{
				int nAxisCount = CTaskTransfer::eMaxAxisCount;
				for( int i = 0; i < nAxisCount; i++ ) {
					OneAxis* pMotor = ( OneAxis* )pTask->GetAxis( i );
					if( pMotor != NULL )
					{
						//pMotor->SetSlowStop();
						double dCurPos = 0;
						pMotor->GetCurrentPos(dCurPos);
						bool bCloseLimit = pMotor->CheckLimitClose();
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
	return CBaseSeqCmd::State_Alarm();
}

//==============================================================================
//
//==============================================================================
BOOL CTransferCmd_MovePos::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CTransferCmd_MovePos::State_Aborting()
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
BOOL CTransferCmd_MovePos::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}