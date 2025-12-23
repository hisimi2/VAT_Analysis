#include "stdafx.h"
#include "TransferCmd_Initial.h"


CTransferCmd_Initial::CTransferCmd_Initial()
{
	m_nOrgRetryCount = 0;
	StringCchCopy( m_szCmdName, _countof( m_szCmdName ), CString( __func__ ) );
}

CTransferCmd_Initial::~CTransferCmd_Initial()
{
}

int CTransferCmd_Initial::CheckParam()
{
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTransferCmd_Initial::OnStop()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Aborting );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTransferCmd_Initial::OnRetry()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTransferCmd_Initial::OnSkip()
{
	ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_AlarmComplete );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTransferCmd_Initial::OnPause()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Pause );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CTransferCmd_Initial::OnResume()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CTransferCmd_Initial::State_Setup()
{
	m_swTimeout.SetCheckTime( TIME_OUT_MIDDLE );
	m_swTimeout.StartTimer();

	return CBaseSeqCmd::State_Setup();
}


//==============================================================================
//
//==============================================================================
BOOL CTransferCmd_Initial::State_Executing()
{
	// check alarm
	CTaskTransfer* pTaskTransfer = ( CTaskTransfer* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTaskTransfer->MakeLog( _T( "[CMD: %s] [EXECUTE] [Step=%8d]" ), m_szCmdName, m_nStep );
	}
	m_nStep_Pre = m_nStep;

	bool bTrayPPSafety = g_TaskTrayUnloadPP.is_safety_state();
	if (bTrayPPSafety != true) {
		int nAlid = ERR_TRNASFER_UNIT_OPER_FAIL_BECAUSE_TRAY_PP_IS_NOT_SAFETY;
		CParamList AlarmData;
		ReportAlarm(nAlid, AlarmData);
		return CBaseSeqCmd::State_Executing();
	}
	




	OneAxis* pMotorX = ( OneAxis* )pTaskTransfer->GetAxis( CTaskTransfer::AXIS_TRANSFER_X );

	switch( m_nStep )
	{
		case 0: // check interlock
			{
				m_nOrgRetryCount = 0;
				pTaskTransfer->m_bInitialized = FALSE;

				m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
				m_swTimeout.StartTimer();
				m_nStep = 1000;
			}break;

		case 1000:
			{
				if( m_swTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_TRNASFER_UP_TIMEOUT;
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotorX->GetAxisName());
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				pTaskTransfer->do_cyl_UpDown( DEF_UP );
				int nErr = pTaskTransfer->chk_cyl_UpDown( DEF_UP );
				if( nErr != ERR_NO_ERROR ) {
					break;
				}

				m_nStep = 1100;
			}break;

		case 1100:
			{
				//int nErr = pTask->GetTrayExist( DEF_NOT_EXIST );
				//if( nErr != ERR_NO_ERROR ) {
				//	int nAlid = ERR_TRNASFER_TRAY_EXIST;
				//	CParamList AlarmData;
				//	ReportAlarm( nAlid, AlarmData );
				//	break;
				//}

				m_nStep = 2000;
			}break;

		case 2000:
			{
				pMotorX->SetStop();
				Sleep( 100 );
				pMotorX->AlarmReset();
				Sleep( 100 );
				pMotorX->ServoOn();
				Sleep( 100 );
				pMotorX->EnableSWLimit( DEF_DISABLE );
				pMotorX->ClearOrgFlag();
				m_swTimeout.ResetStartTime();
				m_swTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
				m_nStep++;
			}break;
		case 2001:
			{
				int nErrOrg = pMotorX->Origin();
				if (nErrOrg != ERR_NO_ERROR)
				{					
					m_nOrgRetryCount++;
					if( m_nOrgRetryCount >= 2 ) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat( _T( "%s Axis" ), pMotorX->GetAxisName() );
						ReportAlarm(nErrOrg, AlarmData );
						break;
					}
					m_nStep = 2000;
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
							m_nStep = 3000;
							break;
						}
					}
					else {
						if (nErrOrg == ERR_MOTOR_CMN_IN_MOVE_STATE)
						{
							if (m_swTimeout.CheckOverTime() == TRUE)
							{
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat(_T("%s Axis"), pMotorX->GetAxisName());
								ReportAlarm(ERR_TRANSFER_MOTOR_ORG_FAIL, AlarmData);
								break;
							}
						}
						else 
						{
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat(_T("%s Axis"), pMotorX->GetAxisName());
							ReportAlarm(nErrOrg, AlarmData);
							break;
						}					
					}
				}
			}break;
		case 3000:
		{
			double dTPosSafety = 0.0;
			dTPosSafety = pTaskTransfer->GetTeachPos(eLocIdxTransfer::LOC_IDX_TRANSFER_SAFETY, CTaskTransfer::eAxis::AXIS_TRANSFER_X);
			OneAxis* pMotor = (OneAxis*)pTaskTransfer->GetAxis(CTaskTransfer::eAxis::AXIS_TRANSFER_X);
			if (dTPosSafety <= 0.0) {
				pTaskTransfer->MakeLog("[%s] [EXEC=%8d] [Move to Safety Pos](STR)", m_szCmdName, m_nStep);

				pMotor->Move(dTPosSafety, TEACH_MOVE_SPD_LOW, 200, 200);

				m_swTimeout.SetCheckTime((MOTOR_TIME_OUT_SHORT));
				m_swTimeout.StartTimer();
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
		case 3001:
		{

			OneAxis* pMotor = pTaskTransfer->GetAxis(CTaskTransfer::eAxis::AXIS_TRANSFER_X);
			double dCurPos = 0;
			pMotor->GetCurrentPos(dCurPos);

			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				pTaskTransfer->MakeLog("[%s] [EXEC=%8d] [Move to Safety Pos](END)", m_szCmdName, m_nStep);
				m_nStep = 99999;
			}
			else
			{
				if (m_swTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Transfer move Safety pos Error");
					AlarmData.m_fnAddItemFormat("SetTime=%.2f, z cur pos=%.2f", m_swTimeout.GetCheckTime(), dCurPos);
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


		case 99999:
			{
				ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_NormalComplete );
			}break;
	}
	return CBaseSeqCmd::State_Executing();
}


//==============================================================================
//
//==============================================================================
BOOL CTransferCmd_Initial::State_Pause()
{
	CTaskTransfer* pTaskTransfer = ( CTaskTransfer* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTaskTransfer->MakeLog( _T( "[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]" ), m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
		case 0:
			{
				int nAxisCount = CTaskTransfer::eMaxAxisCount;
				for( int i = 0; i < nAxisCount; i++ ) {
					OneAxis* pMotor = ( OneAxis* )pTaskTransfer->GetAxis( i );
					if( pMotor != NULL )
					{
						pMotor->SetStop();
						pMotor->OriginStop();
					}
				}
				m_nStep++;
			}break;
		case 1:
			{
				// Initialize 명령의 경우 Pause가 들어 오면 명령을 완료 시키고 삭제한다.
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
BOOL CTransferCmd_Initial::State_Recovery()
{
	CTaskTransfer* pTaskTransfer = ( CTaskTransfer* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTaskTransfer->MakeLog( _T( "[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%d] " ), m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
		case 0:
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
BOOL CTransferCmd_Initial::State_NormalComplete()
{
	CTaskTransfer* pTaskTransfer = ( CTaskTransfer* )m_pReceive;
	pTaskTransfer->m_bInitialized = TRUE;
	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CTransferCmd_Initial::State_Alarm()
{
	CTaskTransfer* pTaskTransfer = ( CTaskTransfer* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTaskTransfer->MakeLog( _T( "[CMD: %s] [ALARM] [Step=%8d] [LastStep=%d]" ), m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
		case 0: // alarm 발생 했을 때 처리해 줘야 할꺼 처리 해줌
			{
				int nAxisCount = CTaskTransfer::eMaxAxisCount;
				for( int i = 0; i < nAxisCount; i++ ) {
					OneAxis* pMotor = ( OneAxis* )pTaskTransfer->GetAxis( i );
					if( pMotor != NULL )
					{
						pMotor->SetStop();
						pMotor->OriginStop();
					}
				}
				m_nStep++;
			}break;
		case 1:
			{
				// Initialize 명령의 경우 Alarm이 발생되면 명령을 완료 시키고 삭제한다.
				SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Aborting );
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
BOOL CTransferCmd_Initial::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CTransferCmd_Initial::State_Aborting()
{
	CTaskTransfer* pTaskTransfer = ( CTaskTransfer* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTaskTransfer->MakeLog( _T( "[CMD: %s] [ABORT] [Step=%8d]" ), m_szCmdName, m_nStep );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
		case 0:
			{
				int nAxisCount = CTaskTransfer::eMaxAxisCount;
				for( int i = 0; i < nAxisCount; i++ ) {
					OneAxis* pMotor = ( OneAxis* )pTaskTransfer->GetAxis( i );
					if( pMotor != NULL )
					{
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
BOOL CTransferCmd_Initial::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}
