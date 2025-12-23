
#include "stdafx.h"
#include "TaskAutoSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace base_seq_cmd;
using namespace std;

CTaskAutoSystem::CTaskAutoSystem( void )
{
	m_nAutoRunMainStep = eAutoLotState_InitNeed;
	m_nAutoRunSubStep = 0;
	m_nAutoRunBackUpMainStep = eAutoLotState_InitNeed;
	m_nAutoRunBackUpSubStep = 0;

}


CTaskAutoSystem::~CTaskAutoSystem( void )
{
}

int CTaskAutoSystem::Initialize( TCHAR* szDriverFileName )
{
	m_vAllAutoTasks.clear();

	for( int i = 0; i < eMaxStackerCount; i++ ) {
		m_vAllAutoTasks.push_back( &g_TaskAutoStacker[ i ] );
	}

	m_vAllAutoTasks.push_back(&g_TaskAutoTrayUnloadPP);

	m_vAllAutoTasks.push_back(&g_TaskAutoTrayLoadPP);

	m_vAllAutoTasks.push_back( &g_TaskAutoTransfer );

	m_vAllAutoTasks.push_back(&g_TaskAutoTrayFeeder);

	for( int i = 0; i < eMaxLoadTblCount; i++ ) {
		m_vAllAutoTasks.push_back( &g_TaskAutoLdTable[ i ] );
	}

	for( int i = 0; i < eMaxTestPPCount; i++ ) {
		m_vAllAutoTasks.push_back( &g_TaskAutoTestPp[ i ] );
	}

	for( int i = 0; i < eMaxPressUnitCount; i++ ) {
		m_vAllAutoTasks.push_back( &g_TaskAutoPress[ i ] );
	}

	int nSec = 0;

	SYSTEMTIME sysTimeInit;
	::GetLocalTime(&sysTimeInit);
	for(int i = 0; i < eMaxPressUnitCount; i++){
		g_SOT[i] = sysTimeInit;
		g_EOT[i] = sysTimeInit;
	}


	g_load_stackers.clear();
	g_empty_unload_stackers.clear();
	g_empty_load_stackers.clear();

	for (int i = 0; i < eMaxStackerCount; i++) {
		eStacker_Type eType = g_TaskAutoStacker[i].GetStackerType();
		switch (eType)
		{
		case eStackerType_Load: g_load_stackers.push_back(&g_TaskAutoStacker[i]); break;
		case eStackerType_Empty_Unload: g_empty_unload_stackers.push_back(&g_TaskAutoStacker[i]); break;
		case eStackerType_Empty_Load:g_empty_load_stackers.push_back(&g_TaskAutoStacker[i]); break;
		case eStackerType_Unload: break;
		default: ASSERT(false);
		}
	}
	

	return CBaseTaskManager::Initialize();
}

void CTaskAutoSystem::Finalize()
{
	CBaseTaskManager::Finalize();
}

int CTaskAutoSystem::OnEvent( const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList/*=NULL*/ )
{
	return ERR_NO_ERROR;
}

int CTaskAutoSystem::OnCommandRsp( CBaseSeqCmd * pRspCmd, int nEventId, WPARAM wParam, LPARAM lParam, std::vector<CPoint>& vPocket_Picker )
{
	if( nEventId == CMD_EVT_ALARM )
	{
		if( pRspCmd->m_pSender == this )
			ChangeMainStateByRspAlarm();
	}

	return CBaseTaskManager::OnCommandRsp( pRspCmd, nEventId, wParam, lParam, vPocket_Picker );
}

void CTaskAutoSystem::OnOperatorCommand( int nOperatorCommand, WPARAM wParam, LPARAM lParam )
{

}

void CTaskAutoSystem::ThreadProc1()
{
	SetThreadInterval( 0, 50 );
	while( GetThreadAliveFlag( 0 ) ) {
		if( GetPauseFlag( 0 ) ) {
			NEXT;
			continue;
		}
		AutoRunning();
		NEXT;
	}
}

void CTaskAutoSystem::MakeLog( LPCTSTR fmt, ... )
{
	TCHAR tmpMsg[ 1024 ] = { 0, };
	if( fmt )
	{
		va_list argList;
		va_start( argList, fmt );
		_vstprintf_s( tmpMsg, fmt, argList );
		va_end( argList );
	}
	TWLOG.MakeLog( static_cast<int>(eLogID_Auto_SYSTEM), Debug, "", __LINE__, NULL, tmpMsg );
}

void CTaskAutoSystem::AutoState_Idle()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 1000:
		{
			if (g_pDlgHomeCheckStatus != NULL) {
				if (g_pDlgHomeCheckStatus->IsWindowVisible() == TRUE) {
					g_pDlgHomeCheckStatus->ShowWindow(SW_HIDE);
				}
			}
			g_TaskTransfer.m_bInitialized = FALSE;

			CTaskTrayBasePP* pTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
			for (int i = 0; i < eMaxTrayPPCount; i++)
				pTrayPP[i]->m_bInitialized = FALSE;	

			g_TaskTrayFeeder.m_bInitialized = FALSE;

			for (int i = 0; i < eMaxLoadTblCount; i++)
				g_TaskLoadTbl[i].m_bInitialized = FALSE;
			for (int i = 0; i < eMaxTestPPCount; i++)
				g_TaskTestPP[i].m_bInitialized = FALSE;
			for (int i = 0; i < eMaxPressUnitCount; i++)
				g_TaskPressUnit[i].m_bInitialized = FALSE;

			g_TaskSystemCtrl.SetCurrentLotState(eAutoLotState_Idle);
			ChangeSubState( 1001 );
		}break;
	case 1001:
		{
			auto error_auto_task = std::find_if( m_vAllAutoTasks.begin(), m_vAllAutoTasks.end(),
													[] ( CBaseAutoSeqManager* _auto_task ) -> bool
			{
				if( _auto_task->GetCurrentLotState() == eAutoLotState_InitNeed )
					return true;

				return false;
			} );
			if( error_auto_task != m_vAllAutoTasks.end() ) {
				ChangeMainState( eAutoLotState_InitNeed );
				break;
			}
				

			// auto task 들 상태를 점검해서 excute로 변환
			auto not_idle_auto_task = std::find_if( m_vAllAutoTasks.begin(), m_vAllAutoTasks.end(),
						  [] ( CBaseAutoSeqManager* _auto_task ) -> bool
			{
				if( _auto_task->GetCurrentLotState() != eAutoLotState_Idle )
					return true;

				return false;
			} );

			if( not_idle_auto_task != m_vAllAutoTasks.end() )
				break;

			for (int i = 0; i<eMaxLoadTblCount; i++) {
				g_TaskLoadTbl[i].SetWorkReserve(RID_INIT_IDLE_RESERVE);
			}

			// LoadTbl Arrived time
			CTime time = CTime::GetCurrentTime();
			g_TaskLoadTbl[eLoadTbl_1].SetMoveSetPlateTm(time + 100);
			g_TaskLoadTbl[eLoadTbl_4].SetMoveSetPlateTm(time + 1);
			g_TaskLoadTbl[eLoadTbl_2].SetMoveSetPlateTm(time + 100);
			g_TaskLoadTbl[eLoadTbl_3].SetMoveSetPlateTm(time + 1);
			CTaskTrayBasePP* pTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
			for (int i = 0; i < eMaxTrayPPCount; i++)
			{
				pTrayPP[i]->ResetLoadTrayPickFailCnt();
			}


			// lot load time 1회 기록 하지 않는다.
			for (int i = 0; i < eMaxPressUnitCount; i++)
			{
				g_DMCont.m_dmContac_Dn.SN(NDM1_INDEX_RECORD_IGNORE_STATION1 + i, 0);
				g_DMCont.m_dmContac_Dn.SD(DDM1_STOP_INDEX_STATION1 + i, 0);
			}
				

			if (g_TaskSystemCtrl.GetAutoGrrUse() == FALSE)
			{
				// index time ( SOT )
				for (int i = 0; i < eMaxPressUnitCount; i++) {
					g_TaskSystemCtrl.m_tIndexTime[i].ResetStartTime();
					g_TaskSystemCtrl.m_tIndexTime[i].StartTimer();
				}

				g_TaskSystemCtrl.ChangeSiteOffData();
				g_TaskSystemCtrl.LoadSiteOnOffData();
				
				// function violation // By SJI
				//int nAutoRetestKind = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoRetest);
				//g_ART_Rule.ChangeRuleType(nAutoRetestKind);
			}
				
			// jira 313 
			BOOL bRetOneCycle = g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY);
			if (bRetOneCycle) {
				g_DMCont.m_dmEQP.SB(BDM0_ONECYCLE_HISTORY, FALSE);
			}


			g_DMCont.m_dmEQP.SB(BDM0_SYS_LOAD_SUPPLY, TRUE);

			g_TaskAllAutoStacker.SetOneCycleCleanOut(false);

			ChangeMainState(eAutoLotState_Executing);
		}break;
	default :
		ChangeSubState( 1000 );
		break;
	}
}

void CTaskAutoSystem::AutoState_InitNeed()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 2000:
		{
			if (g_pDlgHomeCheckStatus != NULL) {
				if (g_pDlgHomeCheckStatus->IsWindowVisible() == TRUE) {
					g_pDlgHomeCheckStatus->ShowWindow(SW_HIDE);
				}
			}

			g_TaskSystemCtrl.SetCurrentLotState(eAutoLotState_InitNeed);

			g_TaskSystemCtrl.SetAutoGrrUse(FALSE);
			g_TaskSystemCtrl.SetAutoGrrCancel(FALSE);
			g_TaskSystemCtrl.SetAutoLoopCancel(FALSE);
			g_TaskSystemCtrl.SetAutoQAUse(FALSE);
			g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_AutoGrrUse, FALSE);
	
			g_TaskSystemCtrl.OnLotCommand( eAutoLotCmd_AlarmPause, NULL, NULL );
			ChangeSubState( 2001 );
		}break;
	case 2001:
		{

		}break;
	default :
		ChangeSubState( 2000 );
		break;
	}
}

void CTaskAutoSystem::AutoState_Initialize()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 3000 :
		{
			if (g_pDlgHomeCheckStatus != NULL) {
				if (g_pDlgHomeCheckStatus->IsWindowVisible() != TRUE) {
					g_pDlgHomeCheckStatus->ShowWindow(SW_SHOW);
				}
			}
	
			g_TaskSystemCtrl.SetCurrentLotState(eAutoLotState_Initialize);

			// auto task 들 상태를 점검해서 excute로 변환
			auto not_idle_auto_task = std::find_if(m_vAllAutoTasks.begin(), m_vAllAutoTasks.end(),
				[](CBaseAutoSeqManager* _auto_task) -> bool
			{
				if (_auto_task->GetCurrentLotState() != eAutoLotState_Idle)
					return true;

				return false;
			});

			if (not_idle_auto_task != m_vAllAutoTasks.end())
				break;


// 			BOOL bUseNoUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_VisionUse);
// 			if (bUseNoUse == eOPTION_USE)
// 			{
// 				g_pVisionTestHand_1->Cmd_VisionReady_HandCam(eVisionLotStateStart);
// 				//g_pVisionTestHand_2->Cmd_VisionReady_HandCam(eVisionLotStateStart);
// 			}
			
			ChangeSubState(3300);

			// check need recovery loop
			int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN( NDM11_MD_FTRTQCContinue );
			switch( nLotStartMode )
			{
				case eStartMD_Initial:
				case eStartMD_Retest:
				case eStartMD_AutoGRR:
					{
						g_DMCont.m_dmTestPP.SN( NDM04_RECOVERY_SEQ_ENABLE, FALSE ); // init mode no need
						
						for (int i = 0; i < eMaxTestPPCount; i++)
						{
							g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX + i, -1);
							g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_ALREADY_PICK_WORKING_IDX + i, -1);
							g_DMCont.m_dmTestPP.SB(NDM04_TESTHAND1_ALREADY_PICK_TABLE_CALL + i,FALSE);
							g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_ALREADY_PLACE_WORKING_IDX + i, -1);
							g_DMCont.m_dmTestPP.SB(NDM04_TESTHAND1_ALREADY_PICK_UNLOAD_TABLE_CALL + i, FALSE);
							
						}

						int nPocketCount_X = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
						int nPocketCount_Y = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);
						for (int nY = 0; nY < nPocketCount_Y; nY++)
						{
							for (int nX = 0; nX < nPocketCount_X; nX++)
							{
								for (int i = 0; i < eMaxLoadTblCount; i++)
								{
									g_TaskLoadTbl[i].Set_Table_DM_Vacuum_Check(nX, nY, FALSE);
								}
								
							}
						}	
						g_DMCont.m_dmShuttle.SB(BDM3_TABLE_LOAD_VACUUM_CHECK, FALSE);
						g_DMCont.m_dmShuttle.SB(BDM3_TABLE_UNLOAD_VACUUM_CHECK, FALSE);
					}break;
				default:
					{
						g_DMCont.m_dmTestPP.SN( NDM04_RECOVERY_SEQ_ENABLE, TRUE ); // continue mode need
					}break;
			}
		}break;

		//-------------------------------------------------------
		// init complete
		//-------------------------------------------------------
	case 3300:
		{
			int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN( NDM11_MD_FTRTQCContinue );

			//Summary를 만들고 최초 시작일때 
			if (g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt) == 0) {
				// product csv set lot name
				char szLotId[512] = { 0, };
				char szScheduleID[512] = { 0, };
				g_DMCont.m_dmHandlerOpt.GS(SDM11_LotNo, szLotId, sizeof(szLotId));
				g_DMCont.m_dmHandlerOpt.GS(SDM11_ScheduleNo, szScheduleID, sizeof(szScheduleID));
				TWLOG.SetLotName(eLogID_PRODUCTCSV, szLotId);
				TWLOG.SetScheduleNo(eLogID_PRODUCTCSV, szScheduleID);

// 				char sz2didCsvPath[512] = { 0, };
// 				g_DMCont.m_dmHandlerOpt.GS(SDM11_2did_Csv_File_Path, sz2didCsvPath, sizeof(sz2didCsvPath));
// 				TWLOG.SetLotName(eLogID_2DIDCSV, szLotId);
// 				TWLOG.SetScheduleNo(eLogID_2DIDCSV, szScheduleID);
// 
// 				CString strTemp = sz2didCsvPath;
// 				strTemp += "\\Temporary";
// 
// 				TWLOG.SetRootPath(eLogID_2DIDCSV, strTemp);

				int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
				if (nTesterOption != eTester_EachSocket)
				{
					// TWSL421 설비는 2DID CSV File에 Title이 삭제 되어야 함 [6/12/2025 donghyun.shin]
// 					CString strTitle = _T( "" );
// 					strTitle.Format("Handler Send Barcode,Tester Send Barcode,SLT ID,Station ID,Site ID,Tester ID,Socket ID,Test Start Time,Test End Time,Test ErrorCode, Test Result,Test Error String,HW bin");
// 					TWLOG.MakeLog( static_cast<int>( eLogID_PRODUCTCSV ), Debug, "", __LINE__, NULL, strTitle );
//					TWLOG.MakeLog(static_cast<int>(eLogID_2DIDCSV), Debug, "", __LINE__, NULL, strTitle);
				}

				g_DMCont.m_dmEQP.SB(BDM0_CYCLE_UPH_CHECK, FALSE);
				g_DMCont.m_dmEQP.SN(NDM0_CycleCurLotSortingCnt, 0);
				g_DMCont.m_dmEQP.SN(NDM0_CycleLotStartTime, 0);
			}

			BOOL bAutoGrrUse = FALSE;
			if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
			{
				bAutoGrrUse = TRUE;
				g_TaskSystemCtrl.SetAutoGrrCancel(FALSE);
			}

			g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_AutoGrrUse, bAutoGrrUse);
			g_TaskSystemCtrl.SetAutoGrrUse(bAutoGrrUse);

			BOOL bAutoLoopUse = FALSE;
			if (nLotStartMode == eStartMD_AutoLoop || nLotStartMode == eStartMD_AutoLoop_Continue)
			{
				bAutoLoopUse = TRUE;
				g_TaskSystemCtrl.SetAutoLoopCancel(FALSE);
			}

			g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_AutoLoopUse, bAutoLoopUse);
			g_TaskSystemCtrl.SetAutoLoopUse(bAutoLoopUse);

			BOOL bAutoQAUse = FALSE;
			if (nLotStartMode == eStartMD_QA || nLotStartMode == eStartMD_QA_Continue)
			{
				bAutoQAUse = TRUE;
				if (nLotStartMode == eStartMD_QA) {
// 					char szLotSize[64] = { 0, };
// 					CString m_strQty;
// 					m_strQty.Format("%d", 200);
// 					sprintf_s(szLotSize, sizeof(szLotSize), m_strQty);
// 					g_DMCont.m_dmHandlerOpt.SS(SDM11_QTY, szLotSize, sizeof(szLotSize));

					g_DMCont.m_dmEQP.SN(NDM0_CurLotQAEmptyLoadCnt, 0);
					g_DMCont.m_dmEQP.SN(NDM0_QaSampleAddSumLoadCnt, 0);
					g_TaskSystemCtrl.SetQAStatus(eQAStatus::eQAStatus_Load);		
					g_DMCont.m_dmFeeder.SB(BDM10_FEEDER_TRAY_END, FALSE);
				}				
			}

			g_TaskSystemCtrl.SetAutoQAUse(bAutoQAUse);

			BOOL bAutoAuditUse = FALSE;

			char szData[ MAX_PATH ] = { 0, };
			g_DMCont.m_dmHandlerOpt.GS( SDM11_CategoryName, szData, sizeof( szData ) );
			g_CokMgr.LoadCategoryInfo( szData, g_BinCategory );
			//HW Bin 설정 되어있지 않는 Setplate Tray Load 금지
			g_TaskSystemCtrl.SetUseUnloadSetplate( g_BinCategory );

			std::for_each( g_unload_stackers.begin(), g_unload_stackers.end(),
						   [] ( CTaskAutoStacker* _unload_stacker )
			{
				_unload_stacker->SetPassFailConveyor( false );
			} );

			// setting setplate pass/fail bin data
			for( int i = 0; i < g_BinCategory.nIndexCount; i++ )
			{
				if( strcmp( g_BinCategory.aCateInfo[ i ].szHWBin, "1" ) == 0 ) {
					bool bRet = atoi( g_BinCategory.aCateInfo[ i ].szPassFail ) == 1;
					for (int nIdx = 0; nIdx < (int)g_unload_stackers.size(); nIdx++) {
						if (g_unload_stackers[nIdx]->GetStackerIndex() == eSTACKER_04) {
							g_unload_stackers[nIdx]->SetPassFailConveyor(bRet);
							break;
						}
					}
					//g_unload_stackers[ 0 ]->SetPassFailConveyor( bRet );
				}
				if( strcmp( g_BinCategory.aCateInfo[ i ].szHWBin, "2" ) == 0 ) {
					bool bRet = atoi( g_BinCategory.aCateInfo[ i ].szPassFail ) == 1;
					for (int nIdx = 0; nIdx < (int)g_unload_stackers.size(); nIdx++) {
						if (g_unload_stackers[nIdx]->GetStackerIndex() == eSTACKER_05) {
							g_unload_stackers[nIdx]->SetPassFailConveyor(bRet);
							break;
						}
					}
					//g_unload_stackers[ 1 ]->SetPassFailConveyor( bRet );
				}
				if( strcmp( g_BinCategory.aCateInfo[ i ].szHWBin, "3" ) == 0 ) {
					bool bRet = atoi( g_BinCategory.aCateInfo[ i ].szPassFail ) == 1;
					for (int nIdx = 0; nIdx < (int)g_unload_stackers.size(); nIdx++) {
						if (g_unload_stackers[nIdx]->GetStackerIndex() == eSTACKER_06) {
							g_unload_stackers[nIdx]->SetPassFailConveyor(bRet);
							break;
						}
					}
					//g_unload_stackers[ 2 ]->SetPassFailConveyor( bRet );
				}
				if( strcmp( g_BinCategory.aCateInfo[ i ].szHWBin, "4" ) == 0 ) {
					bool bRet = atoi( g_BinCategory.aCateInfo[ i ].szPassFail ) == 1;
					for (int nIdx = 0; nIdx < (int)g_unload_stackers.size(); nIdx++) {
						if (g_unload_stackers[nIdx]->GetStackerIndex() == eSTACKER_07) {
							g_unload_stackers[nIdx]->SetPassFailConveyor(bRet);
							break;
						}
					}
					//g_unload_stackers[ 3 ]->SetPassFailConveyor( bRet );
				}
			}

			if( nLotStartMode == eStartMD_InitContinue || nLotStartMode == eStartMD_RetestContinue || nLotStartMode == eStartMD_AutoGRR_Continue || nLotStartMode == eStartMD_QA_Continue || nLotStartMode == eStartMD_AutoLoop_Continue) {
				g_TaskSystemCtrl.ResetUPHtm();
				g_TaskSystemCtrl.m_dTotalRunTm = g_DMCont.m_dmEQP.GN( NDM0_LotRunTime );
				g_TaskSystemCtrl.m_dTotalStopTm = g_DMCont.m_dmEQP.GN( NDM0_LotStopTime );
				g_TaskSystemCtrl.m_tcUPH.StartTimer();
				g_TaskSystemCtrl.m_tcCycleUPH.StartTimer();

				if( nLotStartMode == eStartMD_RetestContinue ) {
					g_DMCont.m_dmHandlerOpt.SN( NDM11_MD_LOT_MODE, eStartMD_RetestContinue ); // [Make Summary]
				}
			}
			else // only initial mode :: eStartMD_Initial / eStartMD_AutoGRR / eStartMD_QA / eStartMD_Retest
			{
				int nSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
				int nSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
				for (int i = 0; i < eMaxPressUnitCount; i++)
				{
					for (int nY = 0; nY < nSite_Div_Y; nY++)
					{
						for (int nX = 0; nX < nSite_Div_X; nX++)
						{
							char szData[MAX_PATH] = { 0, };
							g_DMCont.m_dmContac_Dn.SS(SDM1_SiteGrrPF_History1_1 + i*STATION_MAX_PARA + nX + nY*STATION_MAX_X_SITE, szData, sizeof(szData));
						}
					}
				}


				g_DMCont.m_dmEQP.SN(NDM0_QaSampleLoadCnt, 0);
				
				g_TaskSystemCtrl.ResetUPHtm();
				if (g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt) == 0) {
					// record lot start time
					SYSTEMTIME time;
					::GetLocalTime(&time);
					CString strLOTstartTm = _T("");
					// sample 2016-01-05 21:10:14.343
					strLOTstartTm.Format("%04d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
					g_DMCont.m_dmEQP.SS(SDM0_CurLotStartTm, (LPSTR)(LPCTSTR)strLOTstartTm, strLOTstartTm.GetLength() + 1);

				}
				else {
					g_TaskSystemCtrl.m_dTotalRunTm = g_DMCont.m_dmEQP.GN(NDM0_LotRunTime);
					g_TaskSystemCtrl.m_dTotalStopTm = g_DMCont.m_dmEQP.GN(NDM0_LotStopTime);
				}
				g_TaskSystemCtrl.m_tcUPH.StartTimer();
				g_TaskSystemCtrl.m_tcCycleUPH.StartTimer();

				BOOL bSocketClean = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_SocketClean);
				if (bSocketClean == TRUE && g_TaskSystemCtrl.GetAutoGrrUse() != TRUE)
				{
					// socket cleaning device data setting
					int nPocketX = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_X);
					int nPocketY = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_X);
					int nDvcCnt = 0;
					//ST_DD_DEVICE *pDvc = new ST_DD_DEVICE;
					//pDvc->sExist = eDvcExist;
					//sprintf_s(pDvc->szBinResult, sizeof(pDvc->szBinResult), DEF_SOCKET_CLEAN_DEVICE);
					ST_DD_DEVICE Dvc;

					for (int nY = 0; nY < nPocketY; nY++) {
						for (int nX = 0; nX < nPocketX; nX++) {
							Dvc.clear();
							g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1_CLEAN_BUFF, nX, nY, &Dvc);
							Dvc.sExist = eDvcExist;
							sprintf_s(Dvc.szBinResult, sizeof(Dvc.szBinResult), DEF_SOCKET_CLEAN_DEVICE);
							g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_PP_1_CLEAN_BUFF, nX, nY, &Dvc);

							Dvc.clear();
							g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_2_CLEAN_BUFF, nX, nY, &Dvc);
							Dvc.sExist = eDvcExist;
							sprintf_s(Dvc.szBinResult, sizeof(Dvc.szBinResult), DEF_SOCKET_CLEAN_DEVICE);
							g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_PP_2_CLEAN_BUFF, nX, nY, &Dvc);
							nDvcCnt++;
						}
					}
					//SAFE_DELETE(pDvc);
				}			

				// only initial mode use
				if (nLotStartMode == eStartMD_Initial || nLotStartMode == eStartMD_QA)
				{
					BOOL bSocketClean = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_SocketClean);
					BOOL bInitiStart = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_CleanInitStart);

					// socket clean initial set
					if (bSocketClean == TRUE && bInitiStart == TRUE && bAutoGrrUse == FALSE)
					{
						for (int i = 0; i < eMaxPressUnitCount; i++) {
							int nSiteOnOff = g_TaskPressUnit[i].GetSocketOnCnt();
							if (nSiteOnOff > 0) {
								g_DMCont.m_dmEQP.SB((BDM0_SOCKETCLEAN_SITE1_RESERVE + i), TRUE);
								g_DMCont.m_dmHandlerOpt.SB(BDM11_MANU_CLN_REV_SITE1 + i, FALSE);
							}
							else {
								g_DMCont.m_dmEQP.SB((BDM0_SOCKETCLEAN_SITE1_RESERVE + i), FALSE);
								g_DMCont.m_dmHandlerOpt.SB(BDM11_MANU_CLN_REV_SITE1 + i, FALSE);
							}
						}
					}				
					if (nLotStartMode == eStartMD_Initial) {
						g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_FTRTQCContinue, eStartMD_InitContinue);
						g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_LOT_MODE, eStartMD_InitContinue); // [Make Summary]
					}
					
					else {
						g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_FTRTQCContinue, eStartMD_QA_Continue);
						g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_LOT_MODE, eStartMD_QA_Continue); // [Make Summary]
					}

				}
				// only grr mode use
				if (nLotStartMode == eStartMD_AutoGRR) {
					int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
					for (int i = 0; i < MAX_TEST_PP_CNT; i++)
						g_TaskAutoTestPp[i].SetSiteGroupReserve(false);

					g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_AutoGrrSupplyEnd, FALSE);
					g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_FTRTQCContinue, eStartMD_AutoGRR_Continue);
					g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_LOT_MODE, eStartMD_AutoGRR_Continue);
					g_DMCont.m_dmEQP.SN(NDM0_CurLotLoadingCnt, 0);

					BOOL bSocketClean = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_SocketClean);
					//BOOL bInitiStart = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_CleanInitStart);

					//Socket Cleaning Initial일 때만 적용 되도록 작업.
					if (bSocketClean == TRUE && bAutoGrrUse == TRUE)
					{
						g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_CleanInitStart, TRUE);
						for (int i = 0; i < eMaxPressUnitCount; i++)
						{
							int nSiteOnOff = g_TaskPressUnit[i].GetSocketOnCntGrr();
							if (nSiteOnOff > 0) {
								g_DMCont.m_dmEQP.SB((BDM0_SOCKETCLEAN_SITE1_RESERVE + i), TRUE);
								g_DMCont.m_dmHandlerOpt.SB(BDM11_MANU_CLN_REV_SITE1 + i, FALSE);
							}
							else {
								g_DMCont.m_dmEQP.SB((BDM0_SOCKETCLEAN_SITE1_RESERVE + i), FALSE);
								g_DMCont.m_dmHandlerOpt.SB(BDM11_MANU_CLN_REV_SITE1 + i, FALSE);
							}
						}
					}

// 					if (bSocketClean == TRUE && bInitiStart == TRUE && bAutoGrrUse == TRUE && nProjectOption == DEF_PROJECT_TPUT)
// 					{
// 						for (int i = 0; i < eMaxPressUnitCount; i++) {
// 							int nSiteOnOff = g_TaskPressUnit[i].GetSocketOnCntGrr();
// 							if (nSiteOnOff > 0) {
// 								g_DMCont.m_dmEQP.SB((BDM0_SOCKETCLEAN_SITE1_RESERVE + i), TRUE);
// 								g_DMCont.m_dmHandlerOpt.SB(BDM11_MANU_CLN_REV_SITE1 + i, FALSE);
// 							}
// 							else {
// 								g_DMCont.m_dmEQP.SB((BDM0_SOCKETCLEAN_SITE1_RESERVE + i), FALSE);
// 								g_DMCont.m_dmHandlerOpt.SB(BDM11_MANU_CLN_REV_SITE1 + i, FALSE);
// 							}
// 						}
// 					}
				}

				// Lot Count Clear [ RT LOT ]
				if( nLotStartMode == eStartMD_Retest )
				{
					g_DMCont.m_dmHandlerOpt.SN( NDM11_MD_FTRTQCContinue, eStartMD_RetestContinue );
					g_DMCont.m_dmHandlerOpt.SN( NDM11_MD_LOT_MODE, eStartMD_RetestContinue );

					for( int i = 0; i<eMaxPressUnitCount; i++ ) {
						g_DMCont.m_dmEQP.SB( ( BDM0_SOCKETCLEAN_SITE1_RESERVE + i ), FALSE );
					}
				}
			}

			g_JamRate.SetBinCateItemList();


			char szCokName[ 512 ] = { 0, };
			ST_BASIC_INFO   stBasicData;
			g_DMCont.m_dmHandlerOpt.GS( SDM11_TrayFileName, szCokName, sizeof( szCokName ) );
			g_CokMgr.LoadBasicInfo( szCokName, stBasicData );

			stBasicData.nFTRTQC = ( int )g_DMCont.m_dmHandlerOpt.GN( NDM11_MD_FTRTQCContinue );
			g_CokMgr.SaveBasicInfo( szCokName, &stBasicData );

			g_TaskSystemCtrl.CalcUPH();	// uph

										// FTP
			int nFtpUse = g_DMCont.m_dmEQP.GN( NDM0_FTP_USE );
			int nRunMode = g_DMCont.m_dmHandlerOpt.GN( NDM11_MD_Run );
			if( nFtpUse == TRUE && nRunMode == eRUN_MODE_ONLINE ) {
				//g_Ftp.DataClear();
				//char szLotID[64] = {0,}; 
				//g_DMCont.m_dmHandlerOpt.GS(SDM11_LotNo,szLotID,sizeof(szLotID));
				//g_Ftp.SetLotNo(szLotID);
				//
				//g_Ftp.CallBackConnect(CTaskSystemCtrl::FtpErrorCallBack);
				//
				//int nFtpType = g_DMCont.m_dmEQP.GN( NDM0_FTP_SERVER_USE );
				//char szIP[128] = {0,} , szID[128] ={0,}, szPW[128]= {0,};
				//
				//g_DMCont.m_dmEQP.GS(SDM0_FTP_ID, szID, sizeof(szID));
				//g_DMCont.m_dmEQP.GS(SDM0_FTP_PW, szPW, sizeof(szPW));
				//g_DMCont.m_dmEQP.GS(SDM0_FTP_IP, szIP, sizeof(szIP));
				//
				//g_Ftp.FtpDataSet(nFtpType,szIP,szID,szPW);
				//
				//for(int i =0; i<eMaxPressUnitCount; i++){
				//	int nSiteOnOff = g_TaskPressUnit[i].GetSocketOnCnt();//g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1+i);		
				//	if( nSiteOnOff > 0 ){
				//		int  nTesterID= g_AppleTestIF.GetStationID(i+1);
				//		if (nTesterID != -1)
				//		{
				//			char szMacPath[256] = { 0, };
				//			g_DMCont.m_dmEQP.GS(SDM0_FTP_MAC_SITE1 + i, (LPSTR)(LPCTSTR)szMacPath, sizeof(szMacPath));
				//			char szNetPath[256] = { 0, };
				//			g_DMCont.m_dmEQP.GS(SDM0_FTP_SERVER_SITE1 + i, (LPSTR)(LPCTSTR)szNetPath, sizeof(szNetPath));
				//			g_Ftp.SetFilePath(nTesterID, szMacPath, szNetPath);
				//		}
				//	}
				//}
			}

			g_2DBarcodeChk.LoadData();

			g_DMCont.m_dmTrayPP.SN(NDM5_TM_LDTrayPP_Remind_Pick_Count, 0);

			ChangeMainState( eAutoLotState_Idle );
		}break;

	default :
		ChangeSubState( 3000 );
		break;
	}
}

void CTaskAutoSystem::AutoState_Pause()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 4000:
		{
			g_TaskSystemCtrl.SetCurrentLotState(eAutoLotState_Pause);
			g_TaskSystemCtrl.OnLotCommand( eAutoLotCmd_Pause, NULL, NULL );
			ChangeSubState( 4001 );
		}break;
	case 4001:
		{
			auto error_auto_task = std::find_if(m_vAllAutoTasks.begin(), m_vAllAutoTasks.end(),
				[](CBaseAutoSeqManager* _auto_task) -> bool
			{
				if (_auto_task->GetCurrentLotState() == eAutoLotState_InitNeed)
					return true;

				return false;
			});
			if (error_auto_task != m_vAllAutoTasks.end()) {
				ChangeMainState(eAutoLotState_InitNeed);
				return;
			}
		}break;
	default :
		ChangeSubState( 4000 );
		break;
	}
}

void CTaskAutoSystem::AutoState_Execute()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%s]" ), GetLotStateToString( m_nAutoRunMainStep ), GetExcuteStateStr() );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	if( m_nAutoRunSubStep > eExcutingStep_End ) {
		m_nAutoRunSubStep = m_nLast_Excuting_Step;
	}
	m_nLast_Excuting_Step = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
		case eExcutingStep_Start:
			{
				g_TaskSystemCtrl.SetCurrentLotState(eAutoLotState_Executing);
				ChangeSubState( eExcutingStep_Command );
			}break;
		case eExcutingStep_Command :
			{
				g_TaskSystemCtrl.OnLotCommand( eAutoLotCmd_Resume, NULL, NULL );
				ChangeSubState( eExcutingStep_Wait );
			}break;
		case eExcutingStep_Wait :
			{
				// load Stacker 상태 확인.
				g_TaskAllAutoStacker.ChkLoadTraySupplyEnd();
				



				// 전체 auto seq들은 excuting  상태 이고,
				// 단위동작들은 전부 ready 상태이고,
				// 전체 device 비어야 하고, load stacker에 아무것도 없다.

// 				if (g_TaskTrayPP[m_eTrayIdx].is_safety_state() != TRUE || g_TaskTransfer.is_safety_state() != TRUE)
// 					break;

				auto not_executing_auto_task = std::find_if( m_vAllAutoTasks.begin(), m_vAllAutoTasks.end(),
														[] ( CBaseAutoSeqManager* _auto_task ) -> bool
				{
					if( _auto_task->GetCurrentLotState() != eAutoLotState_Executing )
						return true;

					return false;
				} );
				if( not_executing_auto_task != m_vAllAutoTasks.end() )
					break;
			

				int TaskCount =  GetObserverCount();
				for( int i = 0; i < TaskCount; i++ ) {
					CBaseTaskManager* pTask = GetTaskByIndex( i );
					if( pTask->GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready )
						return;
				}


				for( int idx = 0; idx < eMaxPressUnitCount; idx++ ) {
					int SiteInDvc = g_TaskPressUnit[ idx ].GetSiteInDvc();
					if( SiteInDvc > 0 ) {
						return;
					}
				}

				for( int idx = 0; idx < eMaxTestPPCount; idx++ ) {
					int loadPickerHaveDvcCnt = g_TaskTestPP[idx].GetLoadPickerDeviceCnt();
					int UnloadPickerHaveDvcCnt = g_TaskTestPP[ idx ].GetUnloadPickerHaveDvcCnt();
					int BufferTableCnt = g_TaskTestPP[ idx ].GetBufferTableCnt();
					if( UnloadPickerHaveDvcCnt > 0 || BufferTableCnt > 0 || loadPickerHaveDvcCnt > 0 ) {
						return;
					}
				}

				for( int idx = 0; idx < eMaxLoadTblCount; idx++ ) {
					int UnloadDeviceCnt = g_TaskLoadTbl[ idx ].GetUnloadDeviceCnt();
					int LoadDeviceCnt = g_TaskLoadTbl[ idx ].GetLoadDeviceCnt();
					if( UnloadDeviceCnt > 0 || LoadDeviceCnt > 0 ) {
						return;
					}
				}

				int TrayPpUnloadDvcCnt = g_TaskTrayUnloadPP.GetTrayPpUnloadDvcCnt();
				int TrayPpLoadDvcCnt = g_TaskTrayLoadPP.GetTrayPpDvcCnt();
				if (TrayPpUnloadDvcCnt > 0 || TrayPpLoadDvcCnt > 0)
					return;
				
				bool bTrayExist_TransferArea_Sen = g_TaskTrayFeeder.GetTrayExist_TransferArea_DvcMode(eExist) == ERR_NO_ERROR;
				int  nTrayExistConv3_Transfer_Area_Data = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_CONV_3_TRANSFER_AREA);

				int  nTrayExistLoadEmpty_Data = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_CONV_1);
				int  nTrayExistBtmLoadEmpty_Data = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_BTM_1);

				if (g_TaskSystemCtrl.IsOneCycleModeOn() == false)
				{
					if (bTrayExist_TransferArea_Sen == true && nTrayExistConv3_Transfer_Area_Data == true)
						return;
				}

				BOOL bAutoQAUse = g_TaskSystemCtrl.GetAutoQAUse();

				if (bAutoQAUse == TRUE && g_TaskSystemCtrl.IsOneCycleModeOn() == false)
				{				
					int nQaSampleCnt = g_TaskSystemCtrl.GetQASampleCount();
			
					int nQAStatus = g_TaskSystemCtrl.GetQAStatus();

					switch (nQAStatus)
					{
						case eQAStatus::eQAStatus_Load:
						{					
							g_TaskSystemCtrl.QaProcessLoad(nQaSampleCnt);
							return;
						}break;
						case eQAStatus::eQAStatus_LoadEnd:
						{
							g_TaskSystemCtrl.QaProcessLoadEnd(nQaSampleCnt);
							return;
						}break;
						case eQAStatus::eQAStatus_EmptyLoad:
						{
							g_TaskSystemCtrl.QaProcessLoadEmpty();
							return;					
						}break;
						case eQAStatus::eQAStatus_End:{}break;
						default:
							break;
					}					
				}



				BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
				BOOL bAutoLoopUse = g_TaskSystemCtrl.GetAutoLoopUse();
			

				BOOL bAutoGrrUseCancel = g_TaskSystemCtrl.GetAutoGrrCancel();
				BOOL bAutoLoopUseCancel = g_TaskSystemCtrl.GetAutoLoopCancel();

				if( g_TaskSystemCtrl.IsOneCycleModeOn() == true
					&& g_TaskAllAutoStacker.GetOneCycleCleanOut() == false )
				{
					bool bNotReadyStacker = FALSE;
					for (std::vector<CTaskAutoStacker*>::size_type i = 0; i < g_unload_stackers.size(); i++)
					{
						int nIdx = g_unload_stackers[i]->GetStackerIndex();
						if (g_DMCont.m_dmDevice.GetTrayExist( nIdx+ eDeviceStage_STACKER_CONV_1 ) != eDeviceData_Exist)
							bNotReadyStacker = TRUE;
					}

					if (bNotReadyStacker == TRUE)
						break;
					

					// Clean Skip One Cycle 변수가 활성화 되면, 자동으로 알람 메세지 없이 Clean Out 한다.
					BOOL bCleanSkipOneCycle = g_DMCont.m_dmEQP.GB(BDM0_CLEAN_DEVICE_SKIP_SYS_ONE_CYCLE);
					if (bCleanSkipOneCycle == TRUE)
					{
						g_TaskAllAutoStacker.SetTrayEndFeed(DEF_LOTEND_TRAYFEED);
						ChangeMainState(eAutoLotState_CleanOut);
						break;
					}

					if( g_Error.IsSetAlarm( ERR_LOT_MSG_ONECYCLE_COMPLETE ) == FALSE ) {
						CParamList AlarmList;
						if (g_TaskSystemCtrl.IsCleanDeviceSkipOneCycleModeOn() == true)
						{
							AlarmList.m_fnAddItemFormat("Clean Device Skip 후, One Cycle이 진행되었습니다.");
						}
						g_Error.AlarmReport( ERR_LOT_MSG_ONECYCLE_COMPLETE, AlarmList, &g_TaskSystemCtrl );
						break;
					}
				}


				int pick_enable_stacker_count = g_TaskAllAutoStacker.GetDevicePickEnableStackerCount();
				int load_enable_stacker_count = g_TaskAllAutoStacker.GetLoadEnableStackerCount();
				if( g_TaskSystemCtrl.IsOneCycleModeOn() != true 
					&& load_enable_stacker_count != 0 
					&& pick_enable_stacker_count != 0 )
				{
					if( bAutoGrrUseCancel == TRUE || bAutoLoopUseCancel == TRUE) {
						// tray 모두 배출하기 위해 TRAY FEED 모드로 배출 한다.
						g_TaskAllAutoStacker.SetTrayEndFeed( DEF_LOTEND_TRAYFEED );
						ChangeMainState( eAutoLotState_CleanOut );
						break;
					}
					//break;
				}

				bool suppyend = g_TaskAllAutoStacker.GetLoadTraySupplyEnd();
				bool onecyclecleanout = g_TaskAllAutoStacker.GetOneCycleCleanOut();
			

				// tray end, feed, sikp 선택 알람 띄우기.
				if( (bAutoGrrUse == FALSE && bAutoLoopUse == FALSE) && ( g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == true || g_TaskAllAutoStacker.GetOneCycleCleanOut() == true )
					&& g_Error.IsSetAlarm( ERR_LOT_MSG_ONECYCLE_COMPLETE ) == FALSE )
				{
					if( g_Error.IsSetAlarm( ERR_LOT_MSG_TRAY_END_FEED ) == FALSE )
					{
						CParamList AlarmList;
						g_Error.AlarmReport( ERR_LOT_MSG_TRAY_END_FEED, AlarmList, &g_TaskSystemCtrl );

						ChangeSubState( eExcutingStep_Wait_User_LOTEND_Confirm );
						break;
					}
				}
				
				if( (bAutoGrrUse == TRUE || bAutoLoopUse == TRUE || bAutoQAUse == TRUE) && pick_enable_stacker_count == 0 )
				{
					int nStackerTray = std::count_if(g_load_stackers.begin(), g_load_stackers.end()
						, [](CTaskAutoStacker* _stacker) -> bool
					{
						eStacker_Idx eIdx = _stacker->GetStackerIndex();
// 						int nTrayExist = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_CONV_1 + eIdx);
// 						int nTrayExistBtm = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_BTM_1 + eIdx);

						int nTrayFeederExist = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_TRAY_FEEDER);
						int nTrayExistConveyor = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_CONV_1 + eIdx);
						int nTrayExistBtm = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_BTM_1 + eIdx);


						int nTrayExistEmpty = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_CONV_1);

						int nTransferTrayExist = g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_Transfer);

						int nStackerRearExist = g_TaskStacker[eIdx].ChkDI(CTaskStacker::xC_TRAY_STACKER_EXIST_SEN, true);

						int nStackerRearLoadEmptyExist = g_TaskStacker[eStacker_Idx::eSTACKER_01].ChkDI(CTaskStacker::xC_TRAY_CV_REAR_EXIST_SEN, true);

						if (nTrayFeederExist == DEF_EXIST || nTrayExistConveyor  == DEF_EXIST 
							|| nTrayExistBtm == DEF_EXIST 
							|| nTrayExistEmpty == DEF_EXIST || nTransferTrayExist  == DEF_EXIST
							|| (nStackerRearLoadEmptyExist == ERR_NO_ERROR && g_TaskSystemCtrl.GetAutoQAUse() == TRUE))
							return true;


						return false;
					});
					if (nStackerTray == 0 || bAutoGrrUseCancel == TRUE)
					{
						// tray 모두 배출하기 위해 TRAY FEED 모드로 배출 한다.
						g_TaskAllAutoStacker.SetTrayEndFeed(DEF_LOTEND_TRAYFEED);
						ChangeMainState(eAutoLotState_CleanOut);
						break;
					}
				}
			}break;
		case eExcutingStep_Wait_User_LOTEND_Confirm :
			{
				int nEndFeed = g_TaskAllAutoStacker.GetTrayEndFeed();

				switch( nEndFeed )
				{
					case DEF_LOTEND_TRAYEND:
					case DEF_LOTEND_TRAYFEED :
						{
							// end, feed면 cleanOut으로,
							ChangeMainState( eAutoLotState_CleanOut );
						}break;
					case DEF_LOTEND_TRAYSKIP :
						{
							// skip이면 pause, supplyEnd 초기화 해야 함.
							g_TaskAllAutoStacker.SetTrayEndFeed( DEF_LOTEND_UNK );
							g_TaskAllAutoStacker.SetLoadTraySupplyEnd( false );
							g_DMCont.m_dmEQP.SB(BDM0_SYS_LOAD_SUPPLY, TRUE);
							g_TaskAllAutoStacker.SetOneCycleCleanOut( false );
							g_TaskSystemCtrl.SetOneCycleMode( DEF_OFF );
							g_TaskSystemCtrl.SetCleanDeviceSkipOneCycleMode(DEF_OFF);

							// Skip 눌렀을땐 다시 재시작 개념으로, Table을 Load Site로 초기화 해준다.
							for (int i = 0; i < eMaxLoadTblCount; i++) {
								g_TaskLoadTbl[i].SetWorkReserve(RID_INIT_IDLE_RESERVE);
							}

							// Press SOT,EOT Time 초기화.
							SYSTEMTIME sysTimeInit;
							::GetLocalTime(&sysTimeInit);
							for (int i = 0; i < eMaxPressUnitCount; i++) {
								g_SOT[i] = sysTimeInit;
								g_EOT[i] = sysTimeInit;
							}


							ChangeMainState( eAutoLotState_Pause );
						}break;
					default :
						return;
				}

			}break;

		case eExcutingStep_End:
			{
			}break;
		default:
			ChangeSubState( eExcutingStep_Start );
			break;
	}
}

void CTaskAutoSystem::AutoState_NormalComplete()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 6000:
		{
			ChangeSubState(6001);
		}break;
	case 6001:
		{
			auto not_normal_comp_auto_task = std::find_if( m_vAllAutoTasks.begin(), m_vAllAutoTasks.end(),
														   [] ( CBaseAutoSeqManager* _auto_task ) -> bool
			{
				if( _auto_task->GetCurrentLotState() != eAutoLotState_NormalComplete )
					return true;

				return false;
			} );

			if( not_normal_comp_auto_task != m_vAllAutoTasks.end() )
				break;

			int TaskCount = GetObserverCount();
			for (int i = 0; i < TaskCount; i++) {
				CBaseTaskManager* pTask = GetTaskByIndex(i);
				if (pTask->GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
					return;
			}


			// 모든 auto task 확인 후 lot state 변경 한다.
			SendEvent( CEID_CHANGE_NORMAL_COMPLETE );
			g_TaskSystemCtrl.SetCurrentLotState( eAutoLotState_NormalComplete );				

			ChangeSubState( 6999 );
		}break;
	case 6999 :
		{
			char szCompany[ 256 ] = { 0, };
			g_DMCont.m_dmHandlerOpt.GS( SDM11_Company, szCompany, sizeof( szCompany ) );


			char szCokName[ 512 ] = { 0, };
			ST_BASIC_INFO   stBasicData;
			g_DMCont.m_dmHandlerOpt.GS( SDM11_TrayFileName, szCokName, sizeof( szCokName ) );
			g_CokMgr.LoadBasicInfo( szCokName, stBasicData );

			int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN( NDM11_MD_FTRTQCContinue );

			int nTrayEndFeed = g_TaskAllAutoStacker.GetTrayEndFeed();
			switch( nTrayEndFeed )
			{
				case DEF_LOTEND_TRAYEND :
					{
						//g_JamRate.SaveSummaryTwType();

						
						g_JamRate.SaveLotJamRate_Summary();
						g_TaskSystemCtrl.LotInfoDataClear();
						
						// record lot start time
						SYSTEMTIME time;
						::GetLocalTime( &time );
						CString strLOTstartTm = _T("");
						// sample 2016-01-05 21:10:14.343
						strLOTstartTm.Format("%04d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
						g_DMCont.m_dmEQP.SS(SDM0_CurLotStartTm, (LPSTR)(LPCTSTR)strLOTstartTm, strLOTstartTm.GetLength()+1);
						
					}break;
				case DEF_LOTEND_TRAYFEED :
					{
						int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN( NDM11_MD_FTRTQCContinue );
						
						switch( nLotStartMode )
						{
							case eStartMD_Initial:
							case eStartMD_InitContinue:
								{
									g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_FTRTQCContinue, eStartMD_Initial);
								}break;
							case eStartMD_AutoGRR:
							case eStartMD_AutoGRR_Continue :
								{
									g_DMCont.m_dmHandlerOpt.SN( NDM11_MD_FTRTQCContinue, eStartMD_AutoGRR);
								}break;
							case eStartMD_Retest:
							case eStartMD_RetestContinue:
								{
									g_DMCont.m_dmHandlerOpt.SN( NDM11_MD_FTRTQCContinue, eStartMD_Retest );
								}
						}
						
						stBasicData.nFTRTQC = eStartMD_Initial; 
						g_CokMgr.SaveBasicInfo( szCokName, &stBasicData );

					}break;
				default:
					ASSERT( false );
					break;
			}
			
			


			// onecycle after check vision reset
			BOOL bUseNoUse = g_DMCont.m_dmHandlerOpt.GB( BDM11_MD_VisionUse );
			if( bUseNoUse == DEF_ON )
			{
				for( int i = 0; i < eMaxPressUnitCount; i++ )
				{
					for( int nY = 0; nY < STATION_MAX_Y_SITE; nY++ )
					{
						for( int nX = 0; nX < STATION_MAX_X_SITE; nX++ )
						{
							g_TaskPressUnit[ i ].SetSiteDvcTestingInfo( nX, nY, eTestSite_Empty );
						}
					}
				}
			}

			g_TaskSystemCtrl.m_dBestTestTime = 0.0;
			

			g_conveyor_work_area.release_work_area( &g_TaskAutoTrayUnloadPP);
			g_conveyor_work_area.release_work_area( &g_TaskAutoTransfer );
			g_conveyor_work_area.release_work_area(&g_TaskAutoTrayFeeder);


			g_Transfer_Feeder_work_area.release_work_area(&g_TaskAutoTransfer);
			g_Transfer_Feeder_work_area.release_work_area(&g_TaskAutoTrayFeeder);

			g_TestPP_Table_work_area[eTestPP_1].release_work_area(&g_TaskAutoTestPp[eTestPP_1]);
			g_TestPP_Table_work_area[eTestPP_1].release_work_area(&g_TaskAutoLdTable[eLoadTbl_2]);
			g_TestPP_Table_work_area[eTestPP_2].release_work_area(&g_TaskAutoTestPp[eTestPP_2]);
			g_TestPP_Table_work_area[eTestPP_2].release_work_area(&g_TaskAutoLdTable[eLoadTbl_3]);

		

			BOOL bSocketClean = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_SocketClean);
			BOOL bInitiStart = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_CleanInitStart);

			// socket clean initial set
			if (bSocketClean == TRUE )
			{
				for (int i = 0; i<eMaxPressUnitCount; i++) {
					g_DMCont.m_dmEQP.SB((BDM0_SOCKETCLEAN_SITE1_RESERVE + i), FALSE);
					g_DMCont.m_dmHandlerOpt.SB(BDM11_MANU_CLN_REV_SITE1 + i, FALSE);
				}
			}

		
			int nLotEndErr = g_TaskSystemCtrl.GetAutoGrrUse() == TRUE ? ERR_GRR_MSG_AUTOGRR_COMPLETE : ERR_LOT_MSG_LOT_NORMAL_COMPLETE;
			if (g_TaskSystemCtrl.GetAutoLoopUse() == TRUE)
			{
				nLotEndErr = ERR_LOOP_MSG_AUTOLOOP_COMPLETE;
			}
			else if (g_TaskSystemCtrl.GetAutoQAUse() == TRUE) {
				nLotEndErr = ERR_LOT_MSG_LOT_QA_NORMAL_COMPLETE;
			}
			

			CParamList AlarmList;
			BOOL bCleanSkipOneCycle = g_DMCont.m_dmEQP.GB(BDM0_CLEAN_DEVICE_SKIP_SYS_ONE_CYCLE);
			if (bCleanSkipOneCycle == TRUE)
			{
				nLotEndErr = ERR_LOT_MSG_LOT_CLEAN_DEVICE_SKIP_NORMAL_COMPLETE;
			}

			g_Error.AlarmReport( nLotEndErr, AlarmList, &g_TaskSystemCtrl );

			g_TaskSystemCtrl.DeActionTowerAndSound( eSIGNAL_REQUEST );
			g_TaskSystemCtrl.ActionTowerAndSound( eSIGNAL_FINISH );

			g_TaskAllAutoStacker.SetLoadTraySupplyEnd(false);
			g_DMCont.m_dmEQP.SB(BDM0_SYS_LOAD_SUPPLY, TRUE);
			g_TaskAllAutoStacker.SetTrayEndFeed(DEF_LOTEND_UNK);
			g_DMCont.m_dmEQP.SB(BDM0_SYS_ONE_CYCLE, FALSE);
			g_DMCont.m_dmEQP.SB(BDM0_ONECYCLE_HISTORY, TRUE); // Lot End 시 Socket off 가능
			g_DMCont.m_dmEQP.SB(BDM0_CLEAN_DEVICE_SKIP_SYS_ONE_CYCLE, FALSE);

			int nPocketCount_X = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
			int nPocketCount_Y = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);
			for (int nY = 0; nY < nPocketCount_Y; nY++)
			{
				for (int nX = 0; nX < nPocketCount_X; nX++)
				{
					for (int i = 0; i < eMaxLoadTblCount; i++)
					{
						g_TaskLoadTbl[i].Set_Table_DM_Vacuum_Check(nX, nY, FALSE);
					}

				}
			}
			g_DMCont.m_dmShuttle.SB(BDM3_TABLE_UNLOAD_VACUUM_CHECK, FALSE);
			g_DMCont.m_dmShuttle.SB(BDM3_TABLE_LOAD_VACUUM_CHECK, FALSE);

// 			if (bUseNoUse == eOPTION_USE)
// 			{
// 				g_pVisionTestHand_1->Cmd_VisionReady_HandCam(eVisionLotStateEnd);
// 				//g_pVisionTestHand_2->Cmd_VisionReady_HandCam(eVisionLotStateEnd);
// 			}

			


			std::for_each( m_vAllAutoTasks.begin(), m_vAllAutoTasks.end()
						   , [] ( CBaseAutoSeqManager* _auto_seq )
			{
				_auto_seq->ChangeMainState( eAutoLotState_InitNeed );
			} );


			g_START_SW.SetStatus( FALSE );
			g_STOP_SW.SetStatus( TRUE );
			ChangeMainState( eAutoLotState_InitNeed );
		}break;
	default :
		ChangeSubState( 6000 );
		break;
	}
}

void CTaskAutoSystem::AutoState_AlarmStop()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 7000:
		{
			g_TaskSystemCtrl.SetCurrentLotState(eAutoLotState_AlarmStop);
			ChangeSubState(7001);
		}break;
	case 7001:
		{

		}break;
	default :
		ChangeSubState( 7000 );
		break;
	}
}

void CTaskAutoSystem::AutoState_Resume()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 8000:
		{
			g_TaskSystemCtrl.SetCurrentLotState(eAutoLotState_Resume);
			ChangeSubState( 8001 );
		}break;
	case 8001:
		{
			ChangeSubState( 8003 );
		}break;
	case 8003:
		{
			int nEndFeed = g_TaskAllAutoStacker.GetTrayEndFeed();
			switch( nEndFeed )
			{
				case DEF_LOTEND_TRAYEND:
				case DEF_LOTEND_TRAYFEED:
					{
						ChangeMainState( eAutoLotState_CleanOut );
					}break;
				default: ChangeMainState( eAutoLotState_Executing ); break;
			}
		}break;
	default :
		ChangeSubState( 8000 );
		break;
	}
}

void CTaskAutoSystem::AutoState_CleanOut()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
		case 9000:
			{
				g_TaskSystemCtrl.SetCurrentLotState(eAutoLotState_CleanOut);


				auto not_clean_auto_task = std::find_if( m_vAllAutoTasks.begin(), m_vAllAutoTasks.end(),
															 [] ( CBaseAutoSeqManager* _auto_task ) -> bool
				{
					if( _auto_task->GetCurrentLotState() != eAutoLotState_CleanOut
						&& _auto_task->GetCurrentLotState() != eAutoLotState_Executing
						&& _auto_task->GetCurrentLotState() != eAutoLotState_NormalComplete )
						return true;

					return false;
				} );
				if( not_clean_auto_task != m_vAllAutoTasks.end() )
					break;

				g_TaskSystemCtrl.OnLotCommand( eAutoLotCmd_CleanOut, NULL, NULL );


				ChangeSubState( 9001 );
			}break;
		case 9001 :
			{
				// transfer, tray pp, stacker 단위 동작들 전부 ready상태
				//int state = g_TaskAllAutoStacker.GetCurrentLotState();
				//if( state == eAutoLotState_NormalComplete ) {
				//	ChangeSubState( 9002 );
				//}

				auto not_clean_auto_task = std::find_if(m_vAllAutoTasks.begin(), m_vAllAutoTasks.end(),
					[](CBaseAutoSeqManager* _auto_task) -> bool
				{
					if (_auto_task->GetCurrentLotState() != eAutoLotState_NormalComplete)
						return true;

					return false;
				});
				if (not_clean_auto_task != m_vAllAutoTasks.end())
					break;

				ChangeSubState(9002);
			}break;
		case 9002 :
			{
				//g_TaskAllAutoStacker.SetOneCycleCleanOut( false );
				//g_TaskSystemCtrl.SetOneCycleMode( DEF_OFF );
				ChangeMainState( eAutoLotState_NormalComplete );
			}break;
		default:
			ChangeSubState( 9000 );
			break;
	}
}

