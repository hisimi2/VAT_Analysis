#include "stdafx.h"
#include "StackerCmd_TrayDn.h"
#define UM_VAT_SEQ_ALARM_REPORT		  (WM_USER + 100)
CStackerCmd_TrayDn::CStackerCmd_TrayDn()
{
	m_nCmdId = eSeqCmd_Stacker_TrayDn;
	StringCchCopy( m_szCmdName, _countof( m_szCmdName ), CString( __func__ ) );

	m_nDeviceMode = eDeviceMode;
	m_nLast_Do_Step = 0;
}

CStackerCmd_TrayDn::~CStackerCmd_TrayDn()
{
}

int CStackerCmd_TrayDn::CheckParam()
{
	m_nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);

	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_TrayDn::OnStop()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Aborting );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_TrayDn::OnRetry()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_TrayDn::OnSkip()
{
	ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_AlarmComplete );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_TrayDn::OnPause()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Pause );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CStackerCmd_TrayDn::OnResume()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_TrayDn::State_Setup()
{
	m_swTimeout.SetCheckTime( TIME_OUT_MIDDLE );
	m_swTimeout.StartTimer();
	
	return CBaseSeqCmd::State_Setup();
}


//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_TrayDn::State_Executing()
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
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] check data (STR)", m_szCmdName, m_nStep);

				if (pTask->ChkFullTray(DEF_ON) != true) {
					int nAlid = ERR_STACKER_1_TRAY_FULL + pTask->GetIndex();
 					if (g_DMCont.m_dmHandlerOpt.GB(BDM11_VAT_MODE) == TRUE) {
 						g_DMCont.m_dmEQP.SN(NDM11_VAT_ALARM_IDX, nAlid);
 						break;
 					}
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d tray full sensor detect!"), pTask->GetIndex() + 1);
					ReportAlarm(nAlid, AlarmData);
					break;
				}

				// Down pos에 Tray가 있으면 파손될 위험이 있음
				if( pTask->ChkExistTray_FrontPos_Sen( DEF_NOT_EXIST ) != ERR_NO_ERROR ) {
					int nAlid = ERR_STACKER_1_TRAY_EXIST_FRONT_POS + pTask->GetIndex();
					if (g_DMCont.m_dmHandlerOpt.GB(BDM11_VAT_MODE) == TRUE) {
						g_DMCont.m_dmEQP.SN(NDM11_VAT_ALARM_IDX, nAlid);
						break;
					}
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				// VAT 측정 Mode일 경우, Stacker Down 하도록 변경. [6/12/2025 dohyeong.kim]
				// 1호기의 경우, C-Tray Jig 감지가 안됨.
				BOOL vat_in_process = g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_USE);

				// Tray Down할 Tray가 없다..
				if( m_nDeviceMode != eNoDeviceNoTray && vat_in_process != TRUE)
				{
					if( pTask->GetEmptyTray() == true ) {
						int nAlid = ERR_STACKER_1_TRAY_EMPTY + pTask->GetIndex();
						if (g_DMCont.m_dmHandlerOpt.GB(BDM11_VAT_MODE) == TRUE) {
							g_DMCont.m_dmEQP.SN(NDM11_VAT_ALARM_IDX, nAlid);
							break;
						}
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
						ReportAlarm( nAlid, AlarmData );
						break;
					}
				}
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] check data (END)", m_szCmdName, m_nStep);

				m_nLast_Do_Step = m_nStep;
				m_nStep = 1000;
			}break;
		// Sub pusher up
		case 1000:
			{
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] sub cylinder up (STR)", m_szCmdName, m_nStep);
				int nErr = pTask->do_cyl_SubPusher( DEF_UP );
				if( nErr != ERR_NO_ERROR ) {
					if (g_DMCont.m_dmHandlerOpt.GB(BDM11_VAT_MODE) == TRUE) {
						g_DMCont.m_dmEQP.SN(NDM11_VAT_ALARM_IDX, nErr);
						break;
					}
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nErr, AlarmData );
					break;
				}

				int nSleep = g_DMCont.m_dmStacker.GN( NDM7_STK_SUB_UPDN_WAITTIME );
				if( 0 < nSleep ) {
					Sleep( nSleep );
				}

				m_swTimeout.SetCheckTime( TIME_OUT_MIDDLE / 3 );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep++;
			}break;
		case 1001:
			{
				if( m_swTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_STACKER_1_SUB_PUSHER_UP_TIMEOUT + pTask->GetIndex();
					if (g_DMCont.m_dmHandlerOpt.GB(BDM11_VAT_MODE) == TRUE) {
						g_DMCont.m_dmEQP.SN(NDM11_VAT_ALARM_IDX, nAlid);
						break;
					}
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				int nErr = pTask->chk_cyl_SubPusher( DEF_UP );
				if( nErr == ERR_NO_ERROR ) {
					pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] sub cylinder up (END)", m_szCmdName, m_nStep);
					m_nStep = 1010;
				}
			}break;
			// Main pusher up
		case 1010:
			{
				if (pTask->ChkFullTray(DEF_ON) != true) {
					int nAlid = ERR_STACKER_1_TRAY_FULL + pTask->GetIndex();
					if (g_DMCont.m_dmHandlerOpt.GB(BDM11_VAT_MODE) == TRUE) {
						g_DMCont.m_dmEQP.SN(NDM11_VAT_ALARM_IDX, nAlid);
						break;
					}
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d tray full sensor detect!"), pTask->GetIndex() + 1);
					ReportAlarm(nAlid, AlarmData);
					break;
				}

				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Main cylinder up (STR)", m_szCmdName, m_nStep);
				int nErr = pTask->do_cyl_MainPusher( DEF_UP );
				if( nErr != ERR_NO_ERROR ) {
					if (g_DMCont.m_dmHandlerOpt.GB(BDM11_VAT_MODE) == TRUE) {
						g_DMCont.m_dmEQP.SN(NDM11_VAT_ALARM_IDX, nErr);
						break;
					}
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nErr, AlarmData );
					break;
				}

				int nSleep = g_DMCont.m_dmStacker.GN( NDM7_STK_MAIN_UPDN_WAITTIME );
				if( 0 < nSleep ) {
					Sleep( nSleep );
				}

				m_swTimeout.SetCheckTime( TIME_OUT_MIDDLE / 3 );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep++;
			}break;
		case 1011:
			{
				if( m_swTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_STACKER_1_MAIN_PUSHER_UP_TIMEOUT + pTask->GetIndex();
					if (g_DMCont.m_dmHandlerOpt.GB(BDM11_VAT_MODE) == TRUE) {
						g_DMCont.m_dmEQP.SN(NDM11_VAT_ALARM_IDX, nAlid);
						break;
					}
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				int nErr = pTask->chk_cyl_MainPusher( DEF_UP );
				if( nErr == ERR_NO_ERROR ) {
					m_nStep = 1020;
					pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Main cylinder up (END)", m_szCmdName, m_nStep);
				}
			}break;
		// Unclamp
		case 1020:
			{
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] lever cylinder unclamp (STR)", m_szCmdName, m_nStep);
				int nErr = pTask->do_cyl_LockUnlock( DEF_UNCLAMP );
				if( nErr != ERR_NO_ERROR ) {
					if (g_DMCont.m_dmHandlerOpt.GB(BDM11_VAT_MODE) == TRUE) {
						g_DMCont.m_dmEQP.SN(NDM11_VAT_ALARM_IDX, nErr);
						break;
					}
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nErr, AlarmData );
					break;
				}

				int nSleep = g_DMCont.m_dmStacker.GN( NDM7_STK_CLAMP_WAITTIME );
				if( 0 < nSleep ) {
					Sleep( nSleep );
				}

				m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep++;
			}break;
		case 1021:
			{
				if( m_swTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_STACKER_1_UNCLAMP_TIMEOUT + pTask->GetIndex();
					if (g_DMCont.m_dmHandlerOpt.GB(BDM11_VAT_MODE) == TRUE) {
						g_DMCont.m_dmEQP.SN(NDM11_VAT_ALARM_IDX, nAlid);
						break;
					}
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				int nErr = pTask->chk_cyl_LockUnlock( DEF_UNCLAMP );
				if( nErr == ERR_NO_ERROR ) {
					m_nStep = 1030;
					pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] lever cylinder unclamp (END)", m_szCmdName, m_nStep);
				}
			}break;
		// Sub pusher down
		case 1030:
			{
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] sub cylinder down (STR)", m_szCmdName, m_nStep);
				int nErr = pTask->do_cyl_SubPusher( DEF_DOWN );
				if( nErr != ERR_NO_ERROR ) {
					if (g_DMCont.m_dmHandlerOpt.GB(BDM11_VAT_MODE) == TRUE) {
						g_DMCont.m_dmEQP.SN(NDM11_VAT_ALARM_IDX, nErr);
						break;
					}
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nErr, AlarmData );
					break;
				}

				int nSleep = g_DMCont.m_dmStacker.GN( NDM7_STK_SUB_UPDN_WAITTIME );
				if( 0 < nSleep ) {
					Sleep( nSleep );
				}

				m_swTimeout.SetCheckTime( TIME_OUT_MIDDLE / 3 );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep++;
			}break;
		case 1031:
			{
				if( m_swTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_STACKER_1_SUB_PUSHER_DN_TIMEOUT + pTask->GetIndex();
					if (g_DMCont.m_dmHandlerOpt.GB(BDM11_VAT_MODE) == TRUE) {
						g_DMCont.m_dmEQP.SN(NDM11_VAT_ALARM_IDX, nAlid);
						break;
					}
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				int nErr = pTask->chk_cyl_SubPusher( DEF_DOWN );
				if( nErr == ERR_NO_ERROR ) {
					m_nStep = 1040;
					pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] sub cylinder down (END)", m_szCmdName, m_nStep);
				}
			}break;
		// Clamp
		case 1040:
			{
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] lever cylinder clamp (STR)", m_szCmdName, m_nStep);
				Sleep( 100 );
				int nErr = pTask->do_cyl_LockUnlock( DEF_CLAMP );
				if( nErr != ERR_NO_ERROR ) {
					if (g_DMCont.m_dmHandlerOpt.GB(BDM11_VAT_MODE) == TRUE) {
						g_DMCont.m_dmEQP.SN(NDM11_VAT_ALARM_IDX, nErr);
						break;
					}
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nErr, AlarmData );
					break;
				}

				int nSleep = g_DMCont.m_dmStacker.GN( NDM7_STK_CLAMP_WAITTIME );
				if( 0 < nSleep ) {
					Sleep( nSleep );
				}

				m_swTimeout.SetCheckTime( TIME_OUT_SHORT );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep++;
			}break;
		case 1041:
			{
				if( m_swTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_STACKER_1_CLAMP_TIMEOUT + pTask->GetIndex();
					if (g_DMCont.m_dmHandlerOpt.GB(BDM11_VAT_MODE) == TRUE) {
						g_DMCont.m_dmEQP.SN(NDM11_VAT_ALARM_IDX, nAlid);
						break;
					}
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				int nErr = pTask->chk_cyl_LockUnlock( DEF_LOCK );
				if( nErr == ERR_NO_ERROR ) {
					m_nStep = 1050;
					pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] lever cylinder clamp (END)", m_szCmdName, m_nStep);
				}
			}break;
		// Main pusher down
		case 1050:
			{
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] main cylinder down (STR)", m_szCmdName, m_nStep);
				int nErr = pTask->do_cyl_MainPusher( DEF_DOWN );
				if( nErr != ERR_NO_ERROR ) {
					if (g_DMCont.m_dmHandlerOpt.GB(BDM11_VAT_MODE) == TRUE) {
						g_DMCont.m_dmEQP.SN(NDM11_VAT_ALARM_IDX, nErr);
						break;
					}
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nErr, AlarmData );
					break;
				}

				int nSleep = g_DMCont.m_dmStacker.GN( NDM7_STK_MAIN_UPDN_WAITTIME );
				if( 0 < nSleep ) {
					Sleep( nSleep );
				}

				m_swTimeout.SetCheckTime( TIME_OUT_MIDDLE / 3 );
				m_swTimeout.StartTimer();
				m_nLast_Do_Step = m_nStep;
				m_nStep++;
			}break;
		case 1051:
			{
				if( m_swTimeout.CheckOverTime() == TRUE ) {
					int nAlid = ERR_STACKER_1_MAIN_PUSHER_DN_TIMEOUT + pTask->GetIndex();
					if (g_DMCont.m_dmHandlerOpt.GB(BDM11_VAT_MODE) == TRUE) {
						g_DMCont.m_dmEQP.SN(NDM11_VAT_ALARM_IDX, nAlid);
						break;
					}
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
					ReportAlarm( nAlid, AlarmData );
					break;
				}

				int nErr = pTask->chk_cyl_MainPusher( DEF_DOWN );
				if( nErr == ERR_NO_ERROR ) {
					m_nStep = 1200;
					pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] main cylinder down (END)", m_szCmdName, m_nStep);
				}
			}break;
		// Check Tray exist (down을 했으므로 tray가 있어야 함)
		case 1200:
			{
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Check tray (STR)", m_szCmdName, m_nStep);
				//if( m_nDeviceMode != eNoDeviceNoTray )
				//{
					if( pTask->ChkExistTray_FrontPos_Sen( DEF_EXIST ) != ERR_NO_ERROR ) {
						int nAlid = ERR_STACKER_1_TRAY_NOT_EXIST_FRONT_POS + pTask->GetIndex();
						if (g_DMCont.m_dmHandlerOpt.GB(BDM11_VAT_MODE) == TRUE) {
							g_DMCont.m_dmEQP.SN(NDM11_VAT_ALARM_IDX, nAlid);
							break;
						}
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
						ReportAlarm( nAlid, AlarmData );
						break;
					}
				//}
				pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Check tray (END)", m_szCmdName, m_nStep);

				
				if( CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto )
				{
					pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Make tray data (STR)", m_szCmdName, m_nStep);
					// tray data 생성.
					eStacker_Idx idx = pTask->GetIndex();
					int stacker_idx = pTask->GetIndex();
					auto stacker_type = g_TaskAutoStacker[ stacker_idx ].GetStackerType();

					int tray_x_cnt = g_DMCont.m_dmTrayPP.GN( NDM5_TrayDiv_X );
					int tray_y_cnt = g_DMCont.m_dmTrayPP.GN( NDM5_TrayDiv_Y );

					ST_DD_DEVICE* stDvc = new ST_DD_DEVICE;
					stDvc->clear();

					ST_DD_TRAY* stTrayData = new ST_DD_TRAY;
					stTrayData->bTrayExist = TRUE;
					stTrayData->nRowMax = tray_y_cnt;
					stTrayData->nColMax = tray_x_cnt;
					g_DMCont.m_dmDevice.SetTrayData( eDeviceStage_STACKER_BTM_1 + idx, *stTrayData );

					int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);

					if( g_TaskSystemCtrl.GetAutoGrrUse() == TRUE )
					{
						int nMaxTestCount = g_DMCont.m_dmHandlerOpt.GN( NDM11_MD_AutoGrrTestCount );
						int nDeviceCnt = 0;
						ST_GRRTEST_INFO stGrrTestInfo[ _countof( stDvc->stGrrTestInfo ) ];

						nDeviceCnt = g_DMCont.m_dmHandlerOpt.GN( NDM11_MD_AutoGrrUnitCount );			// Test할 Device 개수

						int nSiteDivX = g_DMCont.m_dmTestPP.GN( NDM4_TestSite_Div_X );
						int nSiteDivY = g_DMCont.m_dmTestPP.GN( NDM4_TestSite_Div_Y );


						for( int i = 0; i < _countof( stDvc->stGrrTestInfo ); i++ ) {

							int nTotalStieOnCnt = 0;
							for( int nY = 0; nY < nSiteDivY; nY++ )
							{
								for( int nX = 0; nX < nSiteDivX; nX++ )
								{
									int nOnOff = g_TaskPressUnit[i].GetSocketOnOffGrr(nX, nY);
									if(nOnOff == DEF_SITE_ON )
									{
										nTotalStieOnCnt++;
									}
									else {
										stGrrTestInfo[i].nTestCnt[nX][nY] = nMaxTestCount;
									}
								}
							}

							sprintf(stGrrTestInfo[i].szTestSiteNum, "%d", i);

							if( nTotalStieOnCnt > 0 ) {
								
								stGrrTestInfo[i].nMaxTestCnt = nMaxTestCount;
								stGrrTestInfo[i].nSiteCntX = nSiteDivX;
								stGrrTestInfo[i].nSiteCntY = nSiteDivY;
							}
							else {
								stGrrTestInfo[i].nMaxTestCnt = 0;
								stGrrTestInfo[i].nSiteCntX = 0;
								stGrrTestInfo[i].nSiteCntY = 0;
							}
						}

						stDvc->nTotalStationCnt = _countof(stDvc->stGrrTestInfo);

						int nCnt = 0;
						for( int y = 0; y < tray_y_cnt; y++ )
						{
							for( int x = 0; x < tray_x_cnt; x++ )
							{
								if( nCnt < nDeviceCnt )		// Device가 존재하는지 확인하여 데이터 삽입
								{
									stDvc->sExist = stacker_type == eStackerType_Load ? DEF_EXIST : DEF_NOT_EXIST;
									if( stDvc->sExist == DEF_EXIST )
									{
										StringCchPrintf( stDvc->szBinResult, _countof( stDvc->szBinResult ), DEF_NEW_LOAD_DEVICE);
										for( int i = 0; i < _countof( stDvc->stGrrTestInfo ); i++ ) {
											stDvc->stGrrTestInfo[ i ] = stGrrTestInfo[ i ];
										}

										// Make Unique ID
										sprintf( stDvc->szUniqueID, "%d_%d", y + 1, x + 1 );
										stDvc->nRow = y;
										stDvc->nCol = x;
									}
									nCnt++;
									g_DMCont.m_dmDevice.SetDeviceData( eDeviceStage_STACKER_BTM_1 + idx, x, y, stDvc );
								}
							}
						}
					}
					else if (g_TaskSystemCtrl.GetAutoLoopUse() == TRUE)
					{
						int nMaxTestCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoLoopTestCount);
						int nDeviceCnt = 0;
						ST_LOOPTEST_INFO stLoopTestInfo[_countof(stDvc->stLoopTestInfo)];

						nDeviceCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoLoopUnitCount);			// Test할 Device 개수

						int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
						int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);


						for (int i = 0; i < _countof(stDvc->stLoopTestInfo); i++) {

							int nTotalStieOnCnt = 0;
							for (int nY = 0; nY < nSiteDivY; nY++)
							{
								for (int nX = 0; nX < nSiteDivX; nX++)
								{
									int nOnOff = g_TaskPressUnit[i].GetSocketOnOff(nX, nY);
									if (nOnOff == DEF_SITE_ON)
									{
										nTotalStieOnCnt++;
									}
									else {
										stLoopTestInfo[i].nTestCnt[nX][nY] = nMaxTestCount;
									}
								}
							}

							if (nTotalStieOnCnt > 0) {

								stLoopTestInfo[i].nMaxTestCnt = nMaxTestCount;
								stLoopTestInfo[i].nSiteCntX = nSiteDivX;
								stLoopTestInfo[i].nSiteCntY = nSiteDivY;
							}
							else {
								stLoopTestInfo[i].nMaxTestCnt = 0;
								stLoopTestInfo[i].nSiteCntX = 0;
								stLoopTestInfo[i].nSiteCntY = 0;
							}
						}

						stDvc->nTotalStationCnt = _countof(stDvc->stLoopTestInfo);

						int nCnt = 0;
						for (int y = 0; y < tray_y_cnt; y++)
						{
							for (int x = 0; x < tray_x_cnt; x++)
							{
								if (nCnt < nDeviceCnt)		// Device가 존재하는지 확인하여 데이터 삽입
								{
									stDvc->sExist = stacker_type == eStackerType_Load ? DEF_EXIST : DEF_NOT_EXIST;
									if (stDvc->sExist == DEF_EXIST)
									{
										StringCchPrintf(stDvc->szBinResult, _countof(stDvc->szBinResult), DEF_NEW_LOAD_DEVICE);
										for (int i = 0; i < _countof(stDvc->stLoopTestInfo); i++) {
											stDvc->stLoopTestInfo[i] = stLoopTestInfo[i];
										}

										// Make Unique ID
										sprintf(stDvc->szUniqueID, "%d_%d", y + 1, x + 1);
										stDvc->nRow = y;
										stDvc->nCol = x;
									}
									nCnt++;
									g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_STACKER_BTM_1 + idx, x, y, stDvc);
								}
							}
						}
					}				
					else {
// 						if (nLotStartMode == eStartMD_QA || nLotStartMode == eStartMD_QA_Continue)
// 						{
// 							int nCnt = 0;
// 							char szRetBuff[MAX_PATH];
// 							g_DMCont.m_dmHandlerOpt.GS(SDM11_QTY, szRetBuff, MAX_PATH);
// 							int nQaSampleCnt = atoi(szRetBuff);
// 							for (int y = 0; y < tray_y_cnt; y++) {
// 								for (int x = 0; x < tray_x_cnt; x++) {
// 									if (nCnt < nQaSampleCnt)
// 									{
// 										stDvc->sExist = stacker_type == eStackerType_Load ? DEF_EXIST : DEF_NOT_EXIST;
// 
// 										if (stDvc->sExist == DEF_EXIST) {
// 											StringCchPrintf(stDvc->szBinResult, _countof(stDvc->szBinResult), DEF_NEW_LOAD_DEVICE);
// 										}
// 										nCnt++;
// 										g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_STACKER_BTM_1 + idx, x, y, stDvc);
// 									}
// 
// 								}
// 							}
// 						}
// 						else {
							for (int y = 0; y < tray_y_cnt; y++) {
								for (int x = 0; x < tray_x_cnt; x++) {
									stDvc->sExist = stacker_type == eStackerType_Load ? DEF_EXIST : DEF_NOT_EXIST;

									if (stDvc->sExist == DEF_EXIST) {
										StringCchPrintf(stDvc->szBinResult, _countof(stDvc->szBinResult), DEF_NEW_LOAD_DEVICE);
									}
									g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_STACKER_BTM_1 + idx, x, y, stDvc);
								}
							}
//						}
						
					}

					SAFE_DELETE( stDvc );
					SAFE_DELETE( stTrayData );
					pTask->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d] Make tray data (END)", m_szCmdName, m_nStep);
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
BOOL CStackerCmd_TrayDn::State_Pause()
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
BOOL CStackerCmd_TrayDn::State_Recovery()
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
				int nlockErr = pTask->chk_cyl_LockUnlock(DEF_LOCK);
				int nSubUpErr = pTask->chk_cyl_SubPusher(DEF_UP);
				int nMainUpErr = pTask->chk_cyl_MainPusher(DEF_UP);
				/*
				2. Clamp는 Lock 상태고
				3. SubPusher는 Up 상태고
				4. Main Pusher도 Up 상태 일 때만
				Main Pusher를 Down 시켜 준다.
				*/
				if (nlockErr == ERR_NO_ERROR && nSubUpErr == ERR_NO_ERROR && nMainUpErr == ERR_NO_ERROR)
				{
					pTask->MakeLog("[CMD: %s] [RECOVERY] [Step=%8d] Main cylinder down (STR)", m_szCmdName, m_nStep);
					int nErr = pTask->do_cyl_MainPusher(DEF_DOWN, eNoInterLock_ON);
					if (nErr != ERR_NO_ERROR) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
						ReportAlarm(nErr, AlarmData);
						break;
					}

					int nSleep = g_DMCont.m_dmStacker.GN(NDM7_STK_MAIN_UPDN_WAITTIME);
					if (0 < nSleep) {
						Sleep(nSleep);
					}

					m_swTimeout.SetCheckTime(TIME_OUT_MIDDLE / 3);
					m_swTimeout.StartTimer();
					m_nStep++;
					break;
				}
				else {
					m_nStep = 10;
					break;
				}
			}break;
		case 1:
		{
			if (m_swTimeout.CheckOverTime() == TRUE) {
				int nAlid = ERR_STACKER_1_MAIN_PUSHER_DN_TIMEOUT + pTask->GetIndex();
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat(_T("Stacker : %d"), pTask->GetIndex() + 1);
				ReportAlarm(nAlid, AlarmData);
				break;
			}

			int nErr = pTask->chk_cyl_MainPusher(DEF_DOWN);
			if (nErr == ERR_NO_ERROR) {
				m_nStep = 10;
				pTask->MakeLog("[CMD: %s] [RECOVERY] [Step=%8d] Main cylinder down (END)", m_szCmdName, m_nStep);
			}
		}break;
		case 10:
		{
			m_swTimeout.ResetStartTime();
			SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing, m_nLast_Do_Step);
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
BOOL CStackerCmd_TrayDn::State_NormalComplete()
{
	CTaskStacker* pTask = ( CTaskStacker* )m_pReceive;
	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_TrayDn::State_Alarm()
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
BOOL CStackerCmd_TrayDn::State_AlarmComplete()
{
	return CBaseSeqCmd::State_AlarmComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CStackerCmd_TrayDn::State_Aborting()
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
BOOL CStackerCmd_TrayDn::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}






