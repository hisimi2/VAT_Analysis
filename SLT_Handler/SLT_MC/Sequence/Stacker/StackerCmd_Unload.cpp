#include "stdafx.h"
#include "StackerCmd_Unload.h"


CStackerCmd_Unload::CStackerCmd_Unload( bool bConveyorStop /*= true*/, bool bManual /*= false*/ )
	: m_bConveyorStop( bConveyorStop ), m_bManual( bManual )
{
	m_FromStage = eDeviceStage_Undefine;
	m_nCmdId = eSeqCmd_Stacker_Unload;
	StringCchCopy( m_szCmdName, _countof( m_szCmdName ), CString( __func__ ) );

	m_nDeviceMode = eDeviceMode;
	m_nLast_Do_Step = 0;
}

CStackerCmd_Unload::~CStackerCmd_Unload()
{
}

int CStackerCmd_Unload::CheckParam()
{
	m_nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);

	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_Unload::OnStop()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Aborting );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_Unload::OnRetry()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_Unload::OnSkip()
{
	ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_AlarmComplete );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_Unload::OnPause()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Pause );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_Unload::OnResume()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_Unload::State_Setup()
{
	m_swTimeout.SetCheckTime( TIME_OUT_MIDDLE );
	m_swTimeout.StartTimer();

	return CBaseSeqCmd::State_Setup();
}


//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_Unload::State_Executing()
{
	// check alarm
	CTaskStacker* pTask = ( CTaskStacker* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		//pTask->MakeLog( _T( "[CMD: %s] [EXECUTE] [Step=%8d]" ), m_szCmdName, m_nStep );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
		case 0: // check interlock
			{
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] start data check (STR)", m_szCmdName, m_nStep);

				bool bRearExistSen = pTask->GetExistTray_RearPos_DvcMode( DEF_EXIST ) == ERR_NO_ERROR;
				bool bTransferArea_TrayExistSen = g_TaskTrayFeeder.GetTrayExist_TransferArea_DvcMode(eExist) == ERR_NO_ERROR;

// 				if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
// 				{
// 					bRearExistSen = pTask->ChkExistTray_RearPos_Sen(DEF_EXIST) == ERR_NO_ERROR;
// 				}

				if (pTask->GetStackerType() == eStackerType_Load)
				{
					int nErrState = 0;

					// load stacker의 경우 rear와 transfer area 2곳 중 1곳에만 tray가 있어야 한다.

					// 2곳 모두 없으면 error
					if (bRearExistSen != true && bTransferArea_TrayExistSen != true) {
						nErrState++;
					}
					// 2곳 모두 있으면 error
					if(bRearExistSen == true && bTransferArea_TrayExistSen == true)	{ 
						nErrState++;
					}

					if(nErrState != 0 )
					{
						int nAlid = ERR_STACKER_1_TRAY_NOT_EXIST_REAR_POS + pTask->GetIndex();
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
						ReportAlarm(nAlid, AlarmData);
						break;
					}
				}
				else
				{
					if (bRearExistSen != true ) {
						int nAlid = ERR_STACKER_1_TRAY_NOT_EXIST_REAR_POS + pTask->GetIndex();
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
						ReportAlarm(nAlid, AlarmData);
						break;
					}
				}

				if (bRearExistSen != true && bTransferArea_TrayExistSen == true && pTask->GetStackerType() == eStackerType_Load) 
				{
					m_FromStage = eDeviceStage_STACKER_CONV_3_TRANSFER_AREA;
				}
				else 
				{
					m_FromStage = (eDeviceDataStage)(eDeviceStage_STACKER_CONV_1 + pTask->GetIndex());
				}

				bool bFrontNotExistSen = pTask->ChkExistTray_FrontPos_Sen( DEF_NOT_EXIST ) == ERR_NO_ERROR;
				if( bFrontNotExistSen != true ) {
					int nAlid = ERR_STACKER_1_TRAY_EXIST_FRONT_POS + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( _T( "Stacker : %d" ), pTask->GetIndex() + 1 );
					ReportAlarm( nAlid, AlarmData );
					break;
				}
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] start data check (END)", m_szCmdName, m_nStep);

				m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep = 100;

				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] c-tray align cylinder backward (STR)", m_szCmdName, m_nStep);
			}break;

		case 100:
			{
				if( m_swTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_STACKER_1_TRAY_ALIGN_BKWD_TIMEOUT + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				pTask->do_cyl_TrayAlign( DEF_BACK );
				int nErr = pTask->chk_cyl_TrayAlign( DEF_BACK );
				if( nErr != ERR_NO_ERROR ) {
					break;
				}

				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] c-tray align cylinder backward (END)", m_szCmdName, m_nStep);
				m_nLast_Do_Step = m_nStep;
				m_nStep = 200;
			}break;

		case 200:
			{
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] conveyor move fast backward (STR)", m_szCmdName, m_nStep);
				int nErr = pTask->ConveyorMoving( eConveyorMove_BKWD, true );
				if( nErr != ERR_NO_ERROR ) {
					CParamList AlarmData;
					int nStackerIndex = pTask->GetIndex();
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), nStackerIndex + 1);
					ReportAlarm(ERR_STACKER_1_INTERLOCK_CONVEYOR_DRIVER_NOT_WORK + nStackerIndex, AlarmData );
					break;
				}

				m_swTimeout.SetCheckTime(TIME_OUT_SHORT*2.0);
				m_swTimeout.StartTimer();
				m_nStep = 300;
			}break;

		case 300:
			{
				if( m_swTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_STACKER_1_REAR_SLOW_SENSOR_DETECT_TIMEOUT + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				if( m_nDeviceMode != eNoDeviceNoTray || g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
				{
					int nErr = pTask->ChkDI( CTaskStacker::xC_TRAY_CV_FRONT_SLOW_SEN, true );
					if( nErr != ERR_NO_ERROR ) {
						break;
					}
				}
				else {
					Sleep( 500 );
				}

				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] conveyor move fast backward (END)", m_szCmdName, m_nStep);
				m_nStep = 400;
			}break;

		case 400:
			{
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] conveyor move slow backward (STR)", m_szCmdName, m_nStep);
				int nErr = pTask->ConveyorMoving( eConveyorMove_BKWD, false );
				if( nErr != ERR_NO_ERROR ) {
					CParamList AlarmData;
					int nStackerIndex = pTask->GetIndex();
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), nStackerIndex + 1);
					ReportAlarm(ERR_STACKER_1_INTERLOCK_CONVEYOR_DRIVER_NOT_WORK + nStackerIndex, AlarmData );
					break;
				}

				m_swTimeout.SetCheckTime(TIME_OUT_SHORT*2.0);
				m_swTimeout.StartTimer();
				m_nStep = 500;
			}break;

		case 500:
			{
				if( m_swTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_STACKER_1_TRAY_ALIGN_BKWD_TIMEOUT + pTask->GetIndex();
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				if (m_nDeviceMode != eNoDeviceNoTray || g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
				{
					if (m_FromStage == eDeviceStage_STACKER_CONV_3_TRANSFER_AREA)
					{
						int nRearErr = g_TaskTrayFeeder.ChkTrayExist_TransferArea_Sen( eNot_Exist );
						if (nRearErr != ERR_NO_ERROR)
							break;
					}
					else
					{
						int nRearErr = pTask->ChkExistTray_RearPos_Sen(DEF_NOT_EXIST);
						if (nRearErr != ERR_NO_ERROR)
							break;
					}
				}

				if (m_nDeviceMode != eNoDeviceNoTray || g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
				{
					int nErr = pTask->ChkDI(CTaskStacker::xC_TRAY_CV_FRONT_EXIST_SEN, true);
					if (nErr != ERR_NO_ERROR) {
						break;
					}
				}

				if (m_nDeviceMode == eNoDeviceNoTray)
					Sleep( 500 );

				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] conveyor move slow backward (END)", m_szCmdName, m_nStep);

				m_nStep = 1000;
			}break;

		case 1000:
			{
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] conveyor stop (STR)", m_szCmdName, m_nStep);
				pTask->ConveyorSlowStop();
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] conveyor stop (END)", m_szCmdName, m_nStep);

				if( CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto )
				{
					int stacker_idx = pTask->GetIndex();
					pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] swap rear %d ->  front data %d (STR)", m_szCmdName, m_nStep, m_FromStage, eDeviceStage_STACKER_BTM_1 + stacker_idx);					
					g_DMCont.m_dmDevice.SwapStageData(m_FromStage, eDeviceStage_STACKER_BTM_1 + stacker_idx );

					if (m_nDeviceMode != eNoDeviceNoTray || g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
						Sleep(200);

					pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] swap rear %d -> front data  %d (END)", m_szCmdName, m_nStep, m_FromStage, eDeviceStage_STACKER_BTM_1 + stacker_idx);

					int nTrayDataEmptyBtm = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_BTM_1 + stacker_idx);
					int nTrayDataFromStage = g_DMCont.m_dmDevice.GetTrayExist(m_FromStage);

					int nTrayDataEmpty = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_BTM_1 + stacker_idx);

					if(nTrayDataEmptyBtm == false && nTrayDataFromStage == true)
					{									
						pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] [case:1] swap rear %d ->  front data  %d  data = %d"), m_szCmdName, m_nStep, m_FromStage, eDeviceStage_STACKER_BTM_1 + stacker_idx, nTrayDataEmpty);
						pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] [case:1] nTrayDataFromStage = %d -> nTrayDataEmptyBtm = %d"), m_szCmdName, m_nStep, nTrayDataFromStage,nTrayDataEmptyBtm);
						m_nStep = 1000;
						break;
					}
					else if (nTrayDataEmptyBtm == false && nTrayDataFromStage == false)
					{
						int tray_x_cnt = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
						int tray_y_cnt = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);
						ST_DD_TRAY* stTrayData = new ST_DD_TRAY;
						stTrayData->bTrayExist = TRUE;
						stTrayData->nRowMax = tray_y_cnt;
						stTrayData->nColMax = tray_x_cnt;
						g_DMCont.m_dmDevice.SetTrayData(eDeviceStage_STACKER_BTM_1 + stacker_idx, *stTrayData);

						int nTray_Data = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_BTM_1 + stacker_idx);
						pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] [case:2] swap rear %d ->  front data  %d  data = %d"), m_szCmdName, m_nStep, m_FromStage, eDeviceStage_STACKER_BTM_1 + stacker_idx, nTrayDataEmpty);
						pTask->MakeLog(_T("[CMD: %s] [EXECUTE] [Step=%8d] [case:2] nTrayDataFromStage = %d -> nTrayDataEmptyBtm = %d data = %d"), m_szCmdName, m_nStep, nTrayDataFromStage, nTrayDataEmptyBtm , nTray_Data);
					}
				}

				m_nLast_Do_Step = m_nStep = 99999;
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
BOOL CStackerCmd_Unload::State_Pause()
{
	CTaskStacker* pTask = ( CTaskStacker* )m_pReceive;
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
BOOL CStackerCmd_Unload::State_Recovery()
{
	CTaskStacker* pTask = ( CTaskStacker* )m_pReceive;
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
BOOL CStackerCmd_Unload::State_NormalComplete()
{
	CTaskStacker* pTask = ( CTaskStacker* )m_pReceive;
	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_Unload::State_Alarm()
{
	CTaskStacker* pTask = ( CTaskStacker* )m_pReceive;
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
BOOL CStackerCmd_Unload::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_Unload::State_Aborting()
{
	CTaskStacker* pTask = ( CTaskStacker* )m_pReceive;
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
				if (g_DMCont.m_dmHandlerOpt.GB(BDM11_VAT_MODE) == TRUE)
				{
					g_DMCont.m_dmHandlerOpt.SB(BDM11_VAT_ALARM, TRUE);
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
BOOL CStackerCmd_Unload::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}






