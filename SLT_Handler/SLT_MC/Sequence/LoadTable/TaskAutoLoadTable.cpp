#include "StdAfx.h"
#include "TaskAutoLoadTable.h"

#include <vector>
#include <algorithm>

using namespace std;

CTaskAutoLoadTable::CTaskAutoLoadTable( void )
{
	m_nAutoRunMainStep = eAutoLotState_InitNeed;
	m_nAutoRunSubStep = 0;
	m_nAutoRunBackUpMainStep = eAutoLotState_InitNeed;
	m_nAutoRunBackUpSubStep = 0;

	m_eTblIndex = eLoadTbl_1;


	m_nLocJobIdx = eJobIndex_None;
	for (int i = 0; i < eMaxLoadTblCount; i++) {
		m_nTablePos[i] = -1;
	}

	m_mapExcuteState.SetAt( eExcutingStep_Start, _T( "Start" ) );

	m_mapExcuteState.SetAt( eExcutingStep_Check_Job, _T( "Check_Job" ) );

	m_mapExcuteState.SetAt( eExcutingStep_Check_Work_Enable_LdSide, _T( "Check_Work_Enable_LdSide" ) );
	m_mapExcuteState.SetAt( eExcutingStep_Move_LdSide, _T( "Move LD Side" ) );
	m_mapExcuteState.SetAt( eExcutingStep_WaitEnd_LdSide, _T( "WaitEnd LD Side" ) );

	m_mapExcuteState.SetAt( eExcutingStep_Check_Work_Enable_TestSide, _T( "Check_Work_Enable_TestSide" ) );
	m_mapExcuteState.SetAt( eExcutingStep_Move_TestSide, _T( "Move TEST Side" ) );
	m_mapExcuteState.SetAt( eExcutingStep_WaitEnd_TestSide, _T( "WaitEnd TEST Side" ) );

	m_mapExcuteState.SetAt( eExcutingStep_End, _T( "End" ) );

	m_mapExcuteState.SetAt( eExcutingStep_ErrorJob, _T( "_ErrorJob" ) );
	m_bChkTime = FALSE;
	m_nPreContactIdx = -1;

}


CTaskAutoLoadTable::~CTaskAutoLoadTable( void )
{
}

int CTaskAutoLoadTable::Initialize( TCHAR * szDriverFileName, _eLoadTblIdx eTblIndex )
{
	m_eTblIndex = eTblIndex;
	m_nLocJobIdx = eJobIndex_None;

	if (m_eTblIndex == eLoadTbl_2)
		m_eTestPPIdx = eTestPP_1;
	else if (m_eTblIndex == eLoadTbl_3)
		m_eTestPPIdx = eTestPP_2;

	return CBaseTaskManager::Initialize();
}

void CTaskAutoLoadTable::Finalize()
{
	CBaseTaskManager::Finalize();
}


int CTaskAutoLoadTable::OnEvent( const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList/*=NULL*/ )
{
	return ERR_NO_ERROR;
}

int CTaskAutoLoadTable::OnCommandRsp( CBaseSeqCmd * pRspCmd, int nEventId, WPARAM wParam, LPARAM lParam, std::vector<CPoint>& vPocket_Picker )
{
	if( nEventId == CMD_EVT_ALARM )
	{
		if( pRspCmd->m_pSender == this )
			ChangeMainStateByRspAlarm();
	}

	if( nEventId == CMD_EVT_CMD_END )
	{
		//// table 동작 완료 시 reserve 변경 한다.
		//if( pRspCmd->m_pReceive == &g_TaskLoadTbl[ m_eTblIndex ] )
		//{
		//	if( CBaseTaskManager::m_bEqpControlMode == eSeqMode_Auto )
		//	{
		//		if( pRspCmd->m_nCmdId == eSeqCmd_LoadTbl_MovePos )
		//		{
		//			CLoadTblCmd_MovePos* pSeqCmd = ( CLoadTblCmd_MovePos* )pRspCmd;
		//			if( pSeqCmd->m_nCmdStage == LOC_IDX_LD_TBL_LD_SIDE ) {
		//				g_TaskLoadTbl[ m_eTblIndex ].SetWorkReserve( RID_TRAY_PP_( RIDX_TRAY_PP_1 ) );
		//			}
		//			else {
		//				g_TaskLoadTbl[ m_eTblIndex ].SetWorkReserve( RID_TEST_PP_( m_eTblIndex ) );
		//			}
		//		}
		//	}
		//}
	}

	return CBaseTaskManager::OnCommandRsp( pRspCmd, nEventId, wParam, lParam, vPocket_Picker );
}

void CTaskAutoLoadTable::OnOperatorCommand( int nOperatorCommand, WPARAM wParam, LPARAM lParam )
{
}

void CTaskAutoLoadTable::ThreadProc1()
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

void CTaskAutoLoadTable::MakeLog( LPCTSTR fmt, ... )
{
	TCHAR tmpMsg[ 1024 ] = { 0, };
	if( fmt )
	{
		va_list argList;
		va_start( argList, fmt );
		_vstprintf_s( tmpMsg, fmt, argList );
		va_end( argList );
	}
	TWLOG.MakeLog( static_cast< int >( eLogID_Auto_LoadTbl_1 + m_eTblIndex ), Debug, "", __LINE__, NULL, tmpMsg );
}

void CTaskAutoLoadTable::AutoState_Idle()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
		case 1000:
			{
				ChangeSubState( 1001 );
			}break;
		case 1001:
			{
				for (int i = 0; i < eMaxPressUnitCount; i++) {
					g_DMCont.m_dmContac_Up.SN(NDM2_PRESS_1_PRE_CONTACT_STATUS + i, FALSE);
				}

				SYSTEMTIME sysTimeInit;
				::GetLocalTime(&sysTimeInit);
				for (int i = 0; i < eMaxPressUnitCount; i++) {
					g_SOT[i] = sysTimeInit;
					g_EOT[i] = sysTimeInit;
				}



				g_DMCont.m_dmShuttle.SN(NDM3_LDTBL_TESTPP_PREPOS, -1);
				ChangeSubState(1002);
			}break;
		case 1002:
			{

			}break;
		default:
			ChangeSubState( 1000 );
			break;
	}
}

void CTaskAutoLoadTable::AutoState_InitNeed()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
		case 2000:
			{
				g_TaskLoadTbl[ m_eTblIndex ].SetProcessCommand( base_seq_cmd::eProcCmd_Stop );
				ChangeSubState( 2001 );
			}break;
		case 2001:
			{

			}break;
		default:
			ChangeSubState( 2000 );
			break;
	}
}

void CTaskAutoLoadTable::AutoState_Initialize()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
		case 3000:
			{
				int nTestPPIdx = g_TaskLoadTbl[m_eTblIndex].LoadTblFromTestPPToIdx(m_eTblIndex);

				char szCompany[16] = { 0, };
				g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
				int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
				BOOL m_bGRR_Audit_loop = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrAuditLoopUse);

				int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
				BOOL bAutoUseGRR = FALSE;
				if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
					bAutoUseGRR = TRUE;

				//if (nProjectOption == DEF_PROJECT_MAUI) {
				if (g_TaskAutoTestPp[m_eTblIndex/2].GetModuleSocketOnCnt(bAutoUseGRR) == 0 
					&& g_TaskAutoTestPp[m_eTblIndex/2].GetModuleAuditSocketOnCnt() == 0
					&& strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
					ChangeSubState(3100);
					break;
				}
				//}

				if( g_TaskTestPP[nTestPPIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready
					&& g_TaskTestPP[nTestPPIdx].m_bInitialized == TRUE
					&& g_TaskTrayLoadPP.GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready
					&& g_TaskTrayLoadPP.m_bInitialized == TRUE 
					&& g_TaskTrayUnloadPP.GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready
					&& g_TaskTrayUnloadPP.m_bInitialized == TRUE)
				{
					ChangeSubState( 3001 );
				}
			}break;
		case 3001:
			{
				if( g_TaskLoadTbl[ m_eTblIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
				{
					CLoadTblCmd_Initial* pSeqCmd = new CLoadTblCmd_Initial;
					if( SendCommand( g_TaskLoadTbl[ m_eTblIndex ], pSeqCmd ) == eSeqCmdAck_OK )
						ChangeSubState( 3002 );
				}

			}break;
		case 3002:
			{
				if( g_TaskLoadTbl[ m_eTblIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready
					&& g_TaskLoadTbl[ m_eTblIndex ].m_bInitialized == TRUE )
					ChangeSubState( 3100 );
			}break;
			//-------------------------------------------------------
			// init complete
			//-------------------------------------------------------
		case 3100:
			{
				ChangeMainState( eAutoLotState_Idle );
			}break;
		default:
			ChangeSubState( 3000 );
			break;
	}
}

void CTaskAutoLoadTable::AutoState_Pause()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d] [last step=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nLast_Excuting_Step);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
		case 4000:
			{
				g_TaskLoadTbl[ m_eTblIndex ].SetProcessCommand( base_seq_cmd::eProcCmd_Pause );

				for (int i = 0; i < eMaxPressUnitCount; i++) {
					g_DMCont.m_dmContac_Up.SN(NDM2_PRESS_1_PRE_CONTACT_STATUS + i, FALSE);
				}

				ChangeSubState( 4001 );
			}break;
		case 4001:
			{

			}break;
		default:
			ChangeSubState( 4000 );
			break;
	}
}

void CTaskAutoLoadTable::AutoState_Execute()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%s]" ), GetLotStateToString( m_nAutoRunMainStep ), GetExcuteStateStr() );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	if( m_nAutoRunSubStep > eExcutingStep_End ) {
		m_nAutoRunSubStep = m_nLast_Excuting_Step;
	}
	m_nLast_Excuting_Step = m_nAutoRunSubStep; // Recovery 용.

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	//분석 필요.
	if (UseAbleCheck() != TRUE && strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
		Sleep(1);
		return;
	}



	switch( m_nAutoRunSubStep )
	{
		case eExcutingStep_Start:
			{
				ChangeSubState( eExcutingStep_Check_Job );
			}break;
		case eExcutingStep_Check_Job:
			{
				int nGrrModeUse = g_TaskSystemCtrl.GetAutoGrrUse();
				int nJob = (nGrrModeUse == FALSE) ? FindJobState(m_nLocJobIdx) : GrrFindJobState(m_nLocJobIdx);
				if( nJob != eExcutingStep_ErrorJob /*&& m_nLocJobIdx != eJobIndex_None*/)
					ChangeSubState( nJob );
			}break;
		case eExcutingStep_Check_Work_Enable_LdSide:
			{
				int nChkMoveLoadSite = ChkMoveLoadSite();
				int nChkMoveTestSite = ChkMoveTestSite();
				if(nChkMoveLoadSite == ERR_NO_ERROR && nChkMoveTestSite == ERR_NO_ERROR )
					ChangeSubState( eExcutingStep_Move_LdSide );
			}break;
		case eExcutingStep_Move_LdSide:
			{
				if( g_TaskLoadTbl[ m_eTblIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready )
				{
					CLoadTblCmd_MovePos* pSeqCmd = new CLoadTblCmd_MovePos;
					pSeqCmd->m_nCmdStage = LOC_IDX_TBL_LD_SIDE;
					pSeqCmd->m_nMoveStep = g_TaskLoadTbl[m_eTblIndex].GetTableRowStep();

					MakeLog(_T("[MainStep=%s] [SubStep=%s] [LoadSite] [Step=%d]"), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr(), g_TaskLoadTbl[m_eTblIndex].GetTableRowStep());
					if( SendCommand( g_TaskLoadTbl[ m_eTblIndex ], pSeqCmd ) == eSeqCmdAck_OK ) {
						ChangeSubState( eExcutingStep_WaitEnd_LdSide );

					}
				}
			}break;
		case eExcutingStep_WaitEnd_LdSide:
			{
				if (g_TaskLoadTbl[m_eTblIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready) {
					ChangeSubState(eExcutingStep_Check_Job);

					//Load Table일 경우, Reserve Idle 해제.
					if (m_eTblIndex == eLoadTbl_2 || m_eTblIndex == eLoadTbl_3)
					{
						int nHandIdx = m_eTblIndex / 2;
						g_DMCont.m_dmShuttle.SB(BDM3_LOAD_TABLE_1_WORK_RESERVE_IDLE + nHandIdx, FALSE);
					}
				}
			}break;

		case eExcutingStep_Check_Work_Enable_TestSide:
			{

				auto nCheckTestSite = ChkMoveTestSite();
				auto nCheckLoadSite = ChkMoveLoadSite();
				if(nCheckLoadSite == ERR_NO_ERROR && nCheckTestSite == ERR_NO_ERROR )
					ChangeSubState( eExcutingStep_Move_TestSide );
			}break;
		case eExcutingStep_Move_TestSide:
			{
				if( g_TaskLoadTbl[ m_eTblIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready )
				{
					int nTestPPIdx = (m_eTblIndex / 2);
					int nJobAtsIdx = g_DMCont.m_dmTestPP.GN(NDM04_TESTHAND1_ALREADY_PICK_WORKING_IDX + nTestPPIdx);
					BOOL bAlreadyPickTableCall = g_DMCont.m_dmTestPP.GB(NDM04_TESTHAND1_ALREADY_PICK_TABLE_CALL + nTestPPIdx);

					if (g_TaskLoadTbl[m_eTblIndex].GetTableUsage() == eLoadType && g_TaskLoadTbl[m_eTblIndex].GetFlagLoadTblBuff() != TRUE && nJobAtsIdx != -1)
					{
						bool bWork = g_TestPP_Table_work_area[m_eTestPPIdx].reserve_work_area(&g_TaskAutoLdTable[m_eTblIndex]);
						int table_col_step = g_TaskLoadTbl[m_eTblIndex].GetTableColStep();
						int nRetLoadTblPos = g_TaskLoadTbl[m_eTblIndex].ChkMotorPos(m_nLocJobIdx, CTaskLoadTable::eAxisAll, table_col_step);
						int nchkTestSite = g_TaskAutoLdTable[m_eTblIndex].ChkMoveTestSite();
						
						if ((bWork != true && nJobAtsIdx != m_nLocJobIdx) && bAlreadyPickTableCall == FALSE )
							break;

						// Test PP Already Pick 하려고 하는대, Table 현재 위치랑 다르면 Test PP에서 요청하는 jobIndex로 넘겨 준다.
						if (bAlreadyPickTableCall == TRUE && nJobAtsIdx != m_nLocJobIdx)
						{
							m_nLocJobIdx = nJobAtsIdx;
						}

					}
					
					int nTestPpCmdWorkIdx = g_TaskTestPP[nTestPPIdx].GetSeqWorkCmdStage();
					
					int convert_table_idx = -1;
					

					//g_TaskAutoTestPp[nTestPPIdx].GetLoadTableInx()
					if (LOC_IDX_TEST_PP_TBL_1_LD <= nTestPpCmdWorkIdx && nTestPpCmdWorkIdx <= LOC_IDX_TEST_PP_TBL_4_LD)
					{
						convert_table_idx = nTestPpCmdWorkIdx - LOC_IDX_TEST_PP_TBL_1_LD;
						/*nTestPPType = eTestPPLoadType;*/
					}
					else if (LOC_IDX_TEST_PP_TBL_1_UD <= nTestPpCmdWorkIdx && nTestPpCmdWorkIdx <= LOC_IDX_TEST_PP_TBL_4_UD)
					{
						convert_table_idx = nTestPpCmdWorkIdx - LOC_IDX_TEST_PP_TBL_1_UD;
						/*nTestPPType = eTestPPUnLoadType;*/
					}

					//Test PP Reserve Check
					int test_pp_reserve_idx = -2;
					for (int i = 0; i < eMaxPressUnitCount / 2; i++)
					{
						int nIdx = i + eMaxATS_Area * (m_eTblIndex / 2);
						if (g_testpp_work_area[nTestPPIdx].get_reserved_task(nIdx, &g_TaskAutoTestPp[nTestPPIdx]) == true)
						{
							test_pp_reserve_idx = i; //*eJobIndex_TestSite1*/ + i;
							break;
						}
					}			

					if (g_TaskLoadTbl[m_eTblIndex].GetTableUsage() == eUnloadType)
					{
						int nTestPPIdx = (m_eTblIndex / 2);
						if (g_TaskTestPP[nTestPPIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready /* && vLocList[CTaskLoadTable::eAxisY]> CTaskLoadTable::eYPOS_LoadSide*/)
						{
							BOOL bUnloadTableCall = g_DMCont.m_dmTestPP.GB(NDM04_TESTHAND1_ALREADY_PICK_UNLOAD_TABLE_CALL + nTestPPIdx);
							if (test_pp_reserve_idx == convert_table_idx && bUnloadTableCall == FALSE)
								break;
// 							int nTestPPType = g_TaskTestPP[nTestPPIdx].m_nTestPPType;
// 
// 							std::vector<int>vLocTestPpList;
// 							vLocTestPpList.clear();
// 							g_TaskTestPP[nTestPPIdx].WhereIsMotorPos(nTestPPType, vLocTestPpList, CTaskTestPP::eAxisAll);
// 
// 							int nErrSenZ = g_TaskTestPP[nTestPPIdx].Chk_Z_Safety_Sen_All(nTestPPType);
// 							int nAxisZ = (nTestPPType == eTestPPLoadType) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
// 							if (nErrSenZ != ERR_NO_ERROR || vLocTestPpList[nAxisZ] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit) {
// 								ChangeSubState(eExcutingStep_Check_Job);
// 								break;
// 							}
						}
					}

					CLoadTblCmd_MovePos* pSeqCmd = new CLoadTblCmd_MovePos;
					pSeqCmd->m_nCmdStage = m_nLocJobIdx;
 					if (m_nLocJobIdx == eJobIndex_TestSite1 || m_nLocJobIdx == eJobIndex_TestSite4){
 						pSeqCmd->m_nMoveStep = g_TaskLoadTbl[m_eTblIndex].GetTableColStep();
 					}
 					else{
 						pSeqCmd->m_nMoveStep = 0;
 					}

//					pSeqCmd->m_nMoveStep = g_TaskLoadTbl[m_eTblIndex].GetTableColStep();					

					MakeLog(_T("[MainStep=%s] [SubStep=%s] [TestSite=%d] [Step=%d]"), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr(), m_nLocJobIdx, g_TaskLoadTbl[m_eTblIndex].GetTableColStep());
					if( SendCommand( g_TaskLoadTbl[ m_eTblIndex ], pSeqCmd ) == eSeqCmdAck_OK ) {
						ChangeSubState( eExcutingStep_WaitEnd_TestSide );
					}
				}
			}break;
		case eExcutingStep_WaitEnd_TestSide:
		{
//Origin
// 			if (g_TaskLoadTbl[m_eTblIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
// 				ChangeSubState(eExcutingStep_Check_Job);
			if (g_TaskLoadTbl[m_eTblIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready) {
				if (g_TaskLoadTbl[m_eTblIndex].GetTableUsage() == eLoadType)
				{
					g_TestPP_Table_work_area[m_eTestPPIdx].release_work_area(&g_TaskAutoLdTable[m_eTblIndex]);
				}
				ChangeSubState(eExcutingStep_Check_Job);
			}

		}break;

		case eExcutingStep_End:
			{
				ChangeMainState( eAutoLotState_NormalComplete );
			}break;

	}
	m_nLast_Excuting_Step = m_nAutoRunSubStep;
}

void CTaskAutoLoadTable::AutoState_NormalComplete()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
		case 6000:
			{
				ChangeSubState( 6001 );
			}break;
		case 6001:
			{
				//ChangeMainState( eAutoLotState_Idle );
			}break;
		default:
			ChangeSubState( 6000 );
			break;
	}
}

void CTaskAutoLoadTable::AutoState_AlarmStop()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d] [last step=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nLast_Excuting_Step);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
		case 7000:
			{
				g_TaskLoadTbl[ m_eTblIndex ].SetProcessCommand( base_seq_cmd::eProcCmd_Pause );
				ChangeSubState( 7001 );
			}break;
		case 7001:
			{
			}break;
		default:
			ChangeSubState( 7000 );
			break;
	}
}

void CTaskAutoLoadTable::AutoState_Resume()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		//MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
		MakeLog(_T("[MainStep=%s] [SubStep=%d] [last step=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nLast_Excuting_Step);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
		case 8000:
			{
				g_TaskLoadTbl[ m_eTblIndex ].SetProcessCommand( base_seq_cmd::eProcCmd_Resume );
				ChangeSubState( 8001 );
			}break;
		case 8001:
			{
				if( g_TaskLoadTbl[ m_eTblIndex ].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready )
					break;

				ChangeSubState( 8002 );
			}break;
		case 8002:
			{
				int nEndFeed = g_TaskAllAutoStacker.GetTrayEndFeed();
				switch( nEndFeed )
				{
					case DEF_LOTEND_TRAYEND:
					case DEF_LOTEND_TRAYFEED:
						{
							ChangeMainState( eAutoLotState_CleanOut );
						}break;
					default: {
						m_nTablePos[m_eTblIndex] = -1;
						ChangeMainState(eAutoLotState_Executing, m_nLast_Excuting_Step); 
					}break;
				}

				//ChangeMainState( eAutoLotState_Executing, m_nLast_Excuting_Step );
			}break;
		default:
			ChangeSubState( 8000 );
			break;
	}
}

void CTaskAutoLoadTable::AutoState_CleanOut()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
		case 9000:
		{
			if (g_TaskLoadTbl[m_eTblIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				char szCompany[16] = { 0, };
				g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
				if (UseAbleCheck() != TRUE && strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
					ChangeSubState(9999);
					break;
				}
				else
				{
					ChangeSubState(9001);
				}
			}
		}break;
		case 9001:
			{
				if (g_TaskLoadTbl[m_eTblIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
				{
					CLoadTblCmd_MovePos* pSeqCmd = new CLoadTblCmd_MovePos;
					pSeqCmd->m_nCmdStage = LOC_IDX_LD_TBL_TEST_SIDE_4;
					if (SendCommand(g_TaskLoadTbl[m_eTblIndex], pSeqCmd) == eSeqCmdAck_OK) {
						ChangeSubState(9999);
					}
				}		
			}break;
		case 9999:
			{
				if (g_TaskLoadTbl[m_eTblIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
				{
					ChangeMainState(eAutoLotState_NormalComplete);
				}
			}break;
		default:
			ChangeSubState( 9000 );
			break;
	}
}

int CTaskAutoLoadTable::ChkMoveLoadSite()
{
	double dCurPosX_LD = 0.0, dCurPosX_UD = 0.0, dCurposY_UD = 0.0;
	g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisX)->GetCurrentPos(dCurPosX_LD);
	g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisX)->GetCurrentPos(dCurPosX_UD);
	g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisY)->GetCurrentPos(dCurposY_UD);

	BOOL bChkInPos = FALSE;
	bool bSafetyPosZ = false;
	bool bSafetySenZ = false;

	//Tray Hand 1,2와 Loading Table #1~4  1:1 매칭
	switch (m_eTblIndex)
	{
	case eLoadTbl_1:
	{

		int nEQPNo = g_DMCont.m_dmEQP.GN(NDM0_Handler_EQP_No);
		double dUnloadPpYTablePos = (nEQPNo > DEF_EQUIPMENT_UNIT1) ? -79000.0 : -100000.0;

		//Unload
		if (dCurPosX_UD < INTERLOCK_UD_TRAYPP_X_WORK_ENABLE_FEEDER_START_POS && dCurposY_UD >= dUnloadPpYTablePos/*-100000.0*/)
		{
			bChkInPos = TRUE;
			bSafetySenZ = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisZ)->IsHomeSensorOn();
			bSafetyPosZ = g_TaskTrayUnloadPP.WhereIsMotorPosZ();
		}
	}break;
	case eLoadTbl_2:
	{
		//Load
		if (dCurPosX_LD < INTERLOCK_LD_TRAYPP_X_WORK_ENABLE_FEEDER_START_POS)
		{
			bChkInPos = TRUE;
			bSafetySenZ = g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisZ)->IsHomeSensorOn();
			bSafetyPosZ = g_TaskTrayLoadPP.WhereIsMotorPosZ();
		}
	}break;
	case eLoadTbl_3:
	{
		//Load
		if (dCurPosX_LD > INTERLOCK_LD_TRAYPP_X_WORK_ENABLE_FEEDER_END_POS)
		{
			bChkInPos = TRUE;
			bSafetySenZ = g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisZ)->IsHomeSensorOn();
			bSafetyPosZ = g_TaskTrayLoadPP.WhereIsMotorPosZ();
		}
	}break;
	case eLoadTbl_4:
	{
		int nEQPNo = g_DMCont.m_dmEQP.GN(NDM0_Handler_EQP_No);
		double dUnloadPpYTablePos = (nEQPNo > DEF_EQUIPMENT_UNIT1) ? -69000.0 : -90000.0;
		//Unload
		if (dCurPosX_UD > INTERLOCK_UD_TRAYPP_X_WORK_ENABLE_FEEDER_END_POS && dCurposY_UD >= dUnloadPpYTablePos/*-90000.0*/)
		{
			bChkInPos = TRUE;
			bSafetySenZ = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisZ)->IsHomeSensorOn();
			bSafetyPosZ = g_TaskTrayUnloadPP.WhereIsMotorPosZ();
		}
	}break;
	default:
		break;
	}

	if (bChkInPos == TRUE) {
		if (bSafetyPosZ != true)
		{
			return(m_eTblIndex == eLoadTbl_2 || m_eTblIndex == eLoadTbl_3) ? ERR_MOT_INTERLOCK_TRAY_PP_1_Z_AXIS_IS_NOT_DETECTED_HOME_SENSOR : ERR_MOT_INTERLOCK_TRAY_PP_2_Z_AXIS_IS_NOT_DETECTED_HOME_SENSOR;
		}

		if (bSafetySenZ != true)
		{
			return(m_eTblIndex == eLoadTbl_2 || m_eTblIndex == eLoadTbl_3) ? ERR_TRAY_PP_1_Z_IS_NOT_SAFETY_POS : ERR_TRAY_PP_2_Z_IS_NOT_SAFETY_POS;
		}

	}
	return ERR_NO_ERROR;


	//////////////////////////////////////////////////////////////////////////
// 	CPoint pocket_LeftRear( 0, 0 );
// 	CPoint pocket_FrontRight(MAX_LD_TBL_POCKET_CNT_X - 1, MAX_LD_TBL_POCKET_CNT_Y - 1);
// 
// 	vector<CPoint> vPicker_FrontRight;
// 	vPicker_FrontRight.push_back( CPoint( MAX_TRAY_PP_CNT_X - 1, MAX_TRAY_PP_CNT_Y - 1 ) );
// 	vector<CPoint> vPicker_0_0;
// 	vPicker_0_0.push_back( CPoint( 0, 0 ) );
// 
// 
// 	//2차때 확인 하자.
// 	if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto)
// 	{
// 		double dLeft = 0.0, dTop = 0.0, dRight = 0.0, dBottom = 0.0;
// 		double dTrayPP_Cur_Pos_X = 0.0, dTrayPP_Cur_Pos_Y = 0.0;
// 		bool bSafetyPosZ = false;
// 		bool bSafetySenZ = false;
// 
// 		if (m_eTblIndex == eLoadTbl_2 || m_eTblIndex == eLoadTbl_3) // Chk Load Tray PP
// 		{
// 			dLeft = g_TaskTrayLoadPP.GetTeachPos(static_cast<eLocIdxTrayPP>(LOC_IDX_TRAY_PP_TBL_1 + m_eTblIndex), pocket_LeftRear, vPicker_FrontRight, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX);
// 			dTop = g_TaskTrayLoadPP.GetTeachPos(static_cast<eLocIdxTrayPP>(LOC_IDX_TRAY_PP_TBL_1 + m_eTblIndex), pocket_LeftRear, vPicker_FrontRight, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisY);
// 			dRight = g_TaskTrayLoadPP.GetTeachPos(static_cast<eLocIdxTrayPP>(LOC_IDX_TRAY_PP_TBL_1 + m_eTblIndex), pocket_FrontRight, vPicker_0_0, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX);
// 			dBottom = g_TaskTrayLoadPP.GetTeachPos(static_cast<eLocIdxTrayPP>(LOC_IDX_TRAY_PP_TBL_1 + m_eTblIndex), pocket_FrontRight, vPicker_0_0, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisY);
// 
// 			g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisX)->GetCurrentPos(dTrayPP_Cur_Pos_X);
// 			/*dTrayPP_Cur_Pos_Y = g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisY)->GetCurrentPos();*/
// 
// 			bSafetyPosZ = g_TaskTrayLoadPP.WhereIsMotorPosZ();
// 			bSafetySenZ = g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisZ)->IsHomeSensorOn();
// 		}
// 		else //Chk Unload Tray PP
// 		{
// 			dLeft = g_TaskTrayUnloadPP.GetTeachPos(static_cast<eLocIdxTrayPP>(LOC_IDX_TRAY_PP_TBL_1 + m_eTblIndex), pocket_LeftRear, vPicker_FrontRight, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX);
// 			dTop = g_TaskTrayUnloadPP.GetTeachPos(static_cast<eLocIdxTrayPP>(LOC_IDX_TRAY_PP_TBL_1 + m_eTblIndex), pocket_LeftRear, vPicker_FrontRight, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisY);
// 			dRight = g_TaskTrayUnloadPP.GetTeachPos(static_cast<eLocIdxTrayPP>(LOC_IDX_TRAY_PP_TBL_1 + m_eTblIndex), pocket_FrontRight, vPicker_0_0, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX);
// 			dBottom = g_TaskTrayUnloadPP.GetTeachPos(static_cast<eLocIdxTrayPP>(LOC_IDX_TRAY_PP_TBL_1 + m_eTblIndex), pocket_FrontRight, vPicker_0_0, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisY);
// 
// 			g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisX)->GetCurrentPos(dTrayPP_Cur_Pos_X);
// 			g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisY)->GetCurrentPos(dTrayPP_Cur_Pos_Y);
// 
// 			bSafetyPosZ = g_TaskTrayUnloadPP.WhereIsMotorPosZ();
// 			bSafetySenZ = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisZ)->IsHomeSensorOn();
// 		}
// 
// 		if (((dLeft < dTrayPP_Cur_Pos_X) && (dTrayPP_Cur_Pos_X < dRight)) && (dBottom < dTrayPP_Cur_Pos_Y) && (dTrayPP_Cur_Pos_Y < dTop))
// 		{
// 			if (bSafetyPosZ != true)
// 			{
// 				return(m_eTblIndex == eLoadTbl_2 || m_eTblIndex == eLoadTbl_3) ? ERR_MOT_INTERLOCK_TRAY_PP_1_Z_AXIS_IS_NOT_DETECTED_HOME_SENSOR : ERR_MOT_INTERLOCK_TRAY_PP_2_Z_AXIS_IS_NOT_DETECTED_HOME_SENSOR;
// 			}
// 
// 			if (bSafetySenZ != true)
// 			{
// 				return(m_eTblIndex == eLoadTbl_2 || m_eTblIndex == eLoadTbl_3) ? ERR_TRAY_PP_1_Z_IS_NOT_SAFETY : ERR_TRAY_PP_2_Z_IS_NOT_SAFETY;
// 			}
// 
// 
// 		}
// 	}
// 	else
// 	{
// 		bool bSafetyZ1 = g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisZ)->IsHomeSensorOn();
// 		bool bSafetyZ2 = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisZ)->IsHomeSensorOn();
// 
// 		if (bSafetyZ1 != TRUE || bSafetyZ2 != TRUE)
// 			return (bSafetyZ1 != TRUE) ? ERR_MOT_INTERLOCK_TRAY_PP_1_Z_AXIS_IS_NOT_DETECTED_HOME_SENSOR : ERR_MOT_INTERLOCK_TRAY_PP_2_Z_AXIS_IS_NOT_DETECTED_HOME_SENSOR;
// 
// 
// 		bool bSafetyPosZ1 = g_TaskTrayLoadPP.WhereIsMotorPosZ();
// 		bool bSafetyPosZ2 =  g_TaskTrayUnloadPP.WhereIsMotorPosZ();
// 
// 		if (bSafetyPosZ1 != true || bSafetyPosZ2 != true)
// 			return (bSafetyPosZ1 != true) ? ERR_MOT_INTERLOCK_TRAY_PP_1_Z_AXIS_IS_NOT_SAFETY_POS : ERR_MOT_INTERLOCK_TRAY_PP_2_Z_AXIS_IS_NOT_SAFETY_POS;
// 	}
// 	return ERR_NO_ERROR;
}

int CTaskAutoLoadTable::ChkMoveTestSite()
{
	// 수정 필요 [5/22/2023 dohyeong.kim]
// 	int nHandIdx = m_eTblIndex / 2;
// 
// 	std::vector<int> vLocList;
// 	vLocList.clear();
// 	
// 	g_TaskTestPP[nHandIdx].WhereIsMotorPos(eTestPPLoadType, vLocList, CTaskTestPP::eAxisAll);
// 
// 	
// 
// 	if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto)
// 	{
// 		double dCurX1Pos = 0.0, dCurX2Pos = 0.0;
// 		g_TaskTestPP[nHandIdx].GetAxis(CTaskTestPP::eAxisX1)->GetCurrentPos(dCurX1Pos);
// 		g_TaskTestPP[nHandIdx].GetAxis(CTaskTestPP::eAxisX2)->GetCurrentPos(dCurX2Pos);
// 
// 		BOOL bChkStationArea = FALSE;
// 		if (nHandIdx == eTestPP_1){
// 			if (dCurX1Pos <= INTERLOCK_TESTPP_1_X_STATION_START_POS + 1000.0 ) { bChkStationArea = TRUE; }
// 		}
// 		else{
// 			if (dCurX1Pos >= INTERLOCK_TESTPP_2_X_STATION_START_POS - 1000.0){ bChkStationArea = TRUE; }
//  		}
// 
// 
// 		if (vLocList[CTaskTestPP::eAxisX1] != CTaskTestPP::ePosX_StationAndPosiLimit || bChkStationArea != TRUE)
// 		{
// 			bool bSafetyZ = g_TaskTestPP[nHandIdx].GetAxis(CTaskTestPP::eAxisZ1)->IsHomeSensorOn();
// 			if (bSafetyZ != true)
// 			{
// 				return ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_DETECTED_HOME_SENSOR + nHandIdx;
// 			}
// 
// 
// 			// Z축이 안전 위치가 아니다.
// 			if (vLocList[CTaskTestPP::eAxisZ1] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit)
// 				return ERR_TEST_PP_1_Z1_MOVE_POS_NOT_SAFETY + nHandIdx;
// 		}
// 
// 
// 		bChkStationArea = FALSE;
// 		if (nHandIdx == eTestPP_1){
// 			if (dCurX2Pos <= INTERLOCK_TESTPP_1_X_STATION_START_POS + 1000.0) { bChkStationArea = TRUE; }
// 		}
// 		else{
// 			if (dCurX2Pos >= INTERLOCK_TESTPP_2_X_STATION_START_POS - 1000.0) { bChkStationArea = TRUE; }
// 		}
// 
// 		if (vLocList[CTaskTestPP::eAxisX2] != CTaskTestPP::ePosX_StationAndPosiLimit || bChkStationArea != TRUE)
// 		{
// 			bool bSafetyZ = g_TaskTestPP[nHandIdx].GetAxis(CTaskTestPP::eAxisZ2)->IsHomeSensorOn();
// 			if (bSafetyZ != true)
// 			{
// 				return  ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_DETECTED_HOME_SENSOR + nHandIdx;
// 			}
// 
// 
// 			// Z축이 안전 위치가 아니다.
// 			if (vLocList[CTaskTestPP::eAxisZ2] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit)
// 				return ERR_TEST_PP_1_Z2_MOVE_POS_NOT_SAFETY + nHandIdx;
// 		}
// 	}
// 	else 
// 	{
// 		bool bSafetyZ1 = g_TaskTestPP[nHandIdx].GetAxis(CTaskTestPP::eAxisZ1)->IsHomeSensorOn();
// 		bool bSafetyZ2 = g_TaskTestPP[nHandIdx].GetAxis(CTaskTestPP::eAxisZ2)->IsHomeSensorOn();
// 
// 		if (bSafetyZ1 != TRUE || bSafetyZ2 != TRUE)
// 			return (bSafetyZ1 != TRUE) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_DETECTED_HOME_SENSOR + nHandIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_DETECTED_HOME_SENSOR + nHandIdx;
// 
// 		if (vLocList[CTaskTestPP::eAxisZ1] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit || vLocList[CTaskTestPP::eAxisZ2] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit)
// 			return (vLocList[CTaskTestPP::eAxisZ1] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit) ? ERR_TEST_PP_1_Z1_MOVE_POS_NOT_SAFETY + nHandIdx : ERR_TEST_PP_1_Z2_MOVE_POS_NOT_SAFETY + nHandIdx;
// 
// 	}

	////////////////////////////////////////////////////////////////////////
	int nHandIdx = m_eTblIndex / 2;
	std::vector<int> vLocList;
	vLocList.clear();
	g_TaskTestPP[nHandIdx].WhereIsMotorPos(eTestPPLoadType, vLocList, CTaskTestPP::eAxisAll);

	double dCurPosX_LD = 0.0, dCurPosX_UD = 0.0;
	g_TaskTestPP[nHandIdx].GetAxis(CTaskTestPP::eAxisX1)->GetCurrentPos(dCurPosX_LD);
	g_TaskTestPP[nHandIdx].GetAxis(CTaskTestPP::eAxisX2)->GetCurrentPos(dCurPosX_UD);

	//X축 영역 확인.
	double dLImitX1Pos = (nHandIdx == eTestPP_1) ? INTERLOCK_TESTPP_X_POS_WITH_TESTPP_Z_AND_TBL_FRONT*(-1) : INTERLOCK_TESTPP_X_POS_WITH_TESTPP_Z_AND_TBL_FRONT;
	double dLimitX2Pos = (nHandIdx == eTestPP_1) ? INTERLOCK_TESTPP_X_POS_WITH_TESTPP_Z_AND_TBL_REAR*(-1) : INTERLOCK_TESTPP_X_POS_WITH_TESTPP_Z_AND_TBL_REAR;
	//double dLimitX2Pos = (m_nTestPpIdx == eTestPP_1) ? 81350.0*(-1) : 81350.0;


	/*
	Tbl 1은 fornt ~ rear 확인
	Tbl 2는 Rear 확인
	Tbl 3은 Rear 확인.
	Tbl 4는 Front ~ Rear 확인.
	
	*/

	if (m_eTblIndex == eLoadTbl_1 || m_eTblIndex == eLoadTbl_4)
	{
		//Load Hand
		if (g_TaskTestPP[nHandIdx].CompareXData(dCurPosX_LD, dLImitX1Pos) == TRUE && g_TaskTestPP[nHandIdx].CompareXData(dCurPosX_LD, dLimitX2Pos) != TRUE) // Front내에 존재, Rear내에 존재 안함(Front ~ Rear)
		{
			bool bSafetyZ = g_TaskTestPP[nHandIdx].GetAxis(CTaskTestPP::eAxisZ1)->IsHomeSensorOn();
			if (bSafetyZ != true)
			{
				return ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_DETECTED_HOME_SENSOR + nHandIdx;
			}

			// Z축이 안전 위치가 아니다.
			if (vLocList[CTaskTestPP::eAxisZ1] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit)
				return ERR_TEST_PP_1_Z1_MOVE_POS_NOT_SAFETY + nHandIdx;
		}

		//Unload Hand
		if (g_TaskTestPP[nHandIdx].CompareXData(dCurPosX_UD, dLImitX1Pos) == TRUE && g_TaskTestPP[nHandIdx].CompareXData(dCurPosX_UD, dLimitX2Pos) != TRUE) // Front내에 존재, Rear내에 존재 안함(Front ~ Rear)
		{
			bool bSafetyZ = g_TaskTestPP[nHandIdx].GetAxis(CTaskTestPP::eAxisZ2)->IsHomeSensorOn();
			if (bSafetyZ != true)
			{
				return ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_DETECTED_HOME_SENSOR + nHandIdx;
			}

			// Z축이 안전 위치가 아니다.
			if (vLocList[CTaskTestPP::eAxisZ2] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit)
				return ERR_TEST_PP_1_Z2_MOVE_POS_NOT_SAFETY + nHandIdx;
		}
	}
	else
	{
		//Load Hand
		if (g_TaskTestPP[nHandIdx].CompareXData(dCurPosX_LD, dLimitX2Pos) == TRUE) { //Limit내에 존재하면(In position)
			bool bSafetyZ = g_TaskTestPP[nHandIdx].GetAxis(CTaskTestPP::eAxisZ1)->IsHomeSensorOn();
			if (bSafetyZ != true)
			{
				return ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_DETECTED_HOME_SENSOR + nHandIdx;
			}

			// Z축이 안전 위치가 아니다.
			if (vLocList[CTaskTestPP::eAxisZ1] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit)
				return ERR_TEST_PP_1_Z1_MOVE_POS_NOT_SAFETY + nHandIdx;
		}

		//Unload Hand
		if (g_TaskTestPP[nHandIdx].CompareXData(dCurPosX_UD, dLimitX2Pos) == TRUE) { //Limit내에 존재하면(In position)
			bool bSafetyZ = g_TaskTestPP[nHandIdx].GetAxis(CTaskTestPP::eAxisZ2)->IsHomeSensorOn();
			if (bSafetyZ != true)
			{
				return ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_DETECTED_HOME_SENSOR + nHandIdx;
			}

			// Z축이 안전 위치가 아니다.
// 			if (vLocList[CTaskTestPP::eAxisZ1] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit)
// 				return ERR_TEST_PP_1_Z2_MOVE_POS_NOT_SAFETY + nHandIdx;
		}
	}

	return ERR_NO_ERROR;
}

int CTaskAutoLoadTable::FindJobState(int &nLocJobIdx)
{
	int nHandIdx = m_eTblIndex / 2;
	//interlock move
	if( g_TaskLoadTbl[ m_eTblIndex ].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready )
		return eExcutingStep_ErrorJob;


	//Test PP가 RID_INIT_IDLE_RESERVE 변경 상태 확인.(Load Table만 확인)
	if (m_eTblIndex == eLoadTbl_2 || m_eTblIndex == eLoadTbl_3)
	{
		BOOL bIsWorkReserveIdle = g_DMCont.m_dmShuttle.GB(BDM3_LOAD_TABLE_1_WORK_RESERVE_IDLE + nHandIdx);
		if (bIsWorkReserveIdle)
		{
			nLocJobIdx = -1;
			return eExcutingStep_Check_Work_Enable_LdSide;
		}
	}


	int nTblRes = g_TaskLoadTbl[ m_eTblIndex ].GetWorkReserve();
	// lot init start 거나 tray hand 일 경우


	// System 초기, Test PP가 일을 다하고 놓아준 상황
	if (nTblRes == RID_INIT_IDLE_RESERVE) {
		nLocJobIdx = -1;
		return eExcutingStep_Check_Work_Enable_LdSide;
	}

	if( nTblRes == RID_TRAY_PP_( RIDX_TRAY_PP_1 ) || nTblRes == RID_TRAY_PP_(RIDX_TRAY_PP_2) )
	{		
		nLocJobIdx = -1;

		int table_row_step = g_TaskLoadTbl[m_eTblIndex].GetTableRowStep();

		int nMotorPos = g_TaskLoadTbl[m_eTblIndex].ChkMotorPos(LOC_IDX_TBL_LD_SIDE, CTaskLoadTable::eAxisAll, table_row_step);
		if (nMotorPos != ERR_NO_ERROR) {
			return eExcutingStep_Check_Work_Enable_LdSide;
		}
	}
	// Test hand거나 Tray hand가 일을 다하고 놓아준 상황
	else if( (nTblRes == RID_TEST_PP_(nHandIdx) || nTblRes == RID_TEST_PP_(nHandIdx) ||nTblRes == RID_NOT_RESERVE))
	{
		int nMotorPos = ERR_NO_ERROR;

		// 1. Test 진행 or Test 완료 Check.
		BOOL bAutoGrrUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);
		BOOL bChkTestComplete = FALSE;
		int nLoadTblDevCnt = g_TaskLoadTbl[m_eTblIndex].GetLoadDeviceCnt();
 		
		//Station중 Test 완료된 것 Check.
		for (int i = 0; i < eMaxATS_Area; i++)
		{
			int nIdx = i + 4*(m_eTblIndex/2);

			ASSERT(ePressUnit_Down_1 <= nIdx && nIdx <= ePressUnit_Down_8);

			int nRetSocket = g_TaskPressUnit[nIdx].GetSocketOnCnt(bAutoGrrUse);
			BOOL bTestStatus = g_DMCont.m_dmEQP.GB(BDM0_TEST_STATUS_SITE1 + nIdx);
			BOOL bContactStatus = g_DMCont.m_dmEQP.GB(BDM0_PRESS_CONTACT_STATUS_SITE1 + nIdx);

			if (nRetSocket > 0 && bTestStatus == FALSE && bContactStatus == TRUE)  //TRUE : test 진행 중, FALSE : test 완료.)
			{
				int a = 1;
			}

			if (nRetSocket > 0 && bTestStatus == FALSE && bContactStatus == FALSE) { //TRUE : test 진행 중, FALSE : test 완료.
				bChkTestComplete = TRUE;
				break;
			}
		}

		if (m_eTblIndex == 0)
			int TEST = 0;

		// 2,3번 Load Table은 비어있다면 Test Site에서 움직일 필요 없음
		BOOL bChkTestSite = FALSE;
		std::vector<int> vLocList;
		vLocList.clear();
		g_TaskLoadTbl[m_eTblIndex].WhereIsMotorPos(vLocList, CTaskLoadTable::eAxisAll);
		if ((vLocList[CTaskLoadTable::eAxisY] > CTaskLoadTable::eYPOS_LoadSideToSitePos1))
		{
			bChkTestSite = TRUE;
		}
	
		//[11/1/2024 dohyeong.kim]
		//Load Table Buffer 사용 시, Reserve & Time보다 먼저 Check 
		//Load Table Buffer 사용은 Test Site 3 위치로 고정
		if (g_TaskLoadTbl[m_eTblIndex].GetTableUsage() == eLoadType && g_TaskLoadTbl[m_eTblIndex].GetFlagLoadTblBuff() == TRUE)
		{
			nLocJobIdx = eJobIndex_TestSite3;
			nMotorPos = g_TaskLoadTbl[m_eTblIndex].ChkMotorPos(nLocJobIdx, CTaskLoadTable::eAxisAll/*, table_col_step*/);
			if (nMotorPos != ERR_NO_ERROR)
				return eExcutingStep_Check_Work_Enable_TestSide;
			else
				return eExcutingStep_ErrorJob;		
		}


		//1. EOT로 Index check [ bChkTestComplete == TRUE Test 완료] [ bChkTestComplete == FALSE Test 중]
		int loc_job_index_chk_time = (bChkTestComplete == FALSE) ? FindStation_CompTime(eTime_SOT) : FindStation_CompTime(eTime_EOT);

		//time 무조건 찾도록 해야 겠다
		if (loc_job_index_chk_time == -1 && m_nPreContactIdx != -1) {
			//-1이 나온건 continue제외 하고 모두 Test진행 중.
			//m_nPreContactIdx 제외 후, 다시 찾기.
			loc_job_index_chk_time = FindStation_CompTime(eTime_SOT);

 		}

		//2. Test Hand Reseve Check
		int loc_job_index_chk_reserve = eExcutingStep_ErrorJob;

		for (int i = 0; i < eMaxPressUnitCount / 2; i++)
		{
			int nIdx = i + eMaxATS_Area * (m_eTblIndex / 2);
			if (g_testpp_work_area[nHandIdx].get_reserved_task(nIdx, &g_TaskAutoTestPp[nHandIdx]) == true) 
			{
				loc_job_index_chk_reserve = eJobIndex_TestSite1 + i;
				break;
			}
		}		

		if (loc_job_index_chk_reserve == loc_job_index_chk_time)
		{
			nLocJobIdx = loc_job_index_chk_reserve;
		}
		else
		{
			if (loc_job_index_chk_time == eExcutingStep_ErrorJob && loc_job_index_chk_reserve != eExcutingStep_ErrorJob)
			{
				// 초기 자재 넣을 때, Unload Table Load side에 둠.
				// Test Hand reserve 된 곳은 없는데 Table 미리 이동 시켜 둘 때.
				nLocJobIdx = loc_job_index_chk_reserve;
			}
			else if (loc_job_index_chk_reserve == eExcutingStep_ErrorJob && loc_job_index_chk_time != eExcutingStep_ErrorJob)
			{
				// 초기 자재 넣을 때, Loading table reserve 된 곳으로 호출.
				// 그 외, Test Hand reserve와 Time check Index 다를 경우 호출.
				if (g_TaskSystemCtrl.IsOneCycleModeOn() != TRUE) {
					int nTestPpCmdPos = g_TaskLoadTbl[m_eTblIndex].GetTestppPrePos();
					if (nTestPpCmdPos != -1) {
						nLocJobIdx = nTestPpCmdPos;
						g_TaskLoadTbl[m_eTblIndex].SetTestppPrePos(-1);
					}
					else {
						nLocJobIdx = loc_job_index_chk_time;
					}
				}
				else {
					nLocJobIdx = loc_job_index_chk_time;
				}
			}
			else
			{
				//기본값은 Reserve
				nLocJobIdx = loc_job_index_chk_reserve;

				if (g_TaskSystemCtrl.IsOneCycleModeOn() == false && g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == false)
				{
					if (g_TaskLoadTbl[m_eTblIndex].GetTableUsage() == eLoadType)
					{
						int nHandIdx = m_eTblIndex / 2;
						int nResPressIdx = (loc_job_index_chk_reserve - 1) + (eMaxATS_Area * nHandIdx);
						int nTimePressIdx = (loc_job_index_chk_time - 1) + (eMaxATS_Area * nHandIdx);

						BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
						int nStationInfo = g_TaskPressUnit[nResPressIdx].GetStationInfo(bAutoGrrUse, TRUE);
						int nStationTestPossibleDvcCnt = g_TaskPressUnit[nResPressIdx].GetSiteInDvcTestPossible();
						int nStationOnOffCnt = g_TaskPressUnit[nResPressIdx].GetSocketOnCnt();

						BOOL bTestStatus = g_DMCont.m_dmEQP.GB(BDM0_TEST_STATUS_SITE1 + nResPressIdx);
						BOOL bContactStatus = g_DMCont.m_dmEQP.GB(BDM0_PRESS_CONTACT_STATUS_SITE1 + nResPressIdx);

						if (bTestStatus == TRUE)
						{
							nStationInfo = eTestSite_Testing;
						}
						else if (bContactStatus == TRUE)
						{
							nStationInfo = eTestSite_ContactStart;
						}

						if (bChkTestComplete == FALSE) // Moudle All Test중인 조건
						{
							nStationInfo = eTestSite_All_Station_Test;
						}
							
						switch (nStationInfo)
						{
							case eTestSite_DvcLoading:
							{
								// Station Device Load 상태에서 Test 할 조건이 완벽하게 갖춰 지면 해당 Station으로 Table 이동하지 않는다.
								int nPressCurExecuitngStep = g_TaskAutoPress[nResPressIdx].GetCurExecutingStep();
								if (nStationTestPossibleDvcCnt == nStationOnOffCnt)
									nLocJobIdx = loc_job_index_chk_time;
								else {
									if (nPressCurExecuitngStep > 5 && nPressCurExecuitngStep <= 8) // Press Contact ~ Test End 까지
									{
										nLocJobIdx = loc_job_index_chk_time;
									}
								}
							

							}break;
							//Test를 진행하고 있는 조건임으로 Reserve로 갈 필요가 없고, SOT or EOT로 이동 한다.
							case eTestSite_ContactStart:
							case eTestSite_Contacting:
							case eTestSite_TestStart:
							case eTestSite_Testing:
							case eTestSite_All_Station_Test:
							{
								int nPressCurExecuitngStep = g_TaskAutoPress[nResPressIdx].GetCurExecutingStep();
								if (nPressCurExecuitngStep <= 1) //Press Job을 찾지 못했으면 Reserve로 보낸다.
									nLocJobIdx = loc_job_index_chk_reserve;
								else
									nLocJobIdx = loc_job_index_chk_time;
							}break;
							default: {
								nLocJobIdx = loc_job_index_chk_reserve;
							}break;
						}					
					}
				}
			}
		}
		if ((m_eTblIndex == eLoadTbl_2 || m_eTblIndex == eLoadTbl_3))
		{
			if (g_TaskSystemCtrl.IsOneCycleModeOn() == false && g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == false)
			{
				if (nLoadTblDevCnt == 0) 
				{
					int nJobIdx = g_DMCont.m_dmTestPP.GN(NDM04_TESTHAND1_WORKING_IDX + (m_eTblIndex / 2));
					if (nJobIdx > 3) {
						return eExcutingStep_ErrorJob;
					}
					else {
						int nUnloadTblIdx = (m_eTblIndex == eLoadTbl_2) ? eLoadTbl_1 : eLoadTbl_4;
						int nUnloadTblCurPos = g_TaskLoadTbl[nUnloadTblIdx].GetTableCurPos();
						/*
							1. Unlaod 현재 Pos와 실제 찾은 위치 안맞으면 Unlaod 현재 위치로 이동

							pick 이미 하고 Unload Table 안왔을 때
							 -. nJobIdx 잡혀 있기 때문에 Unload도 해당 Pos로 이동, Load Table도 그냥 그자리 있음.											
						*/
						if (nUnloadTblCurPos != nLocJobIdx) {
							nLocJobIdx = nJobIdx + 1;
						}
					}

				}
			}
		}
		else
		{
			if (g_TaskLoadTbl[m_eTblIndex].GetUnloadDeviceCnt() == 8)
				return eExcutingStep_ErrorJob;
		}
		if (g_TaskSystemCtrl.IsOneCycleModeOn() == TRUE) {
			for (int nIdx = 0; nIdx < eMaxPressUnitCount; nIdx++) {
				g_DMCont.m_dmContac_Up.SN(NDM2_PRESS_1_PRE_CONTACT_STATUS + nIdx, FALSE);
			}
			
			int nTestPpCmdPos = g_TaskLoadTbl[m_eTblIndex].GetTestppPrePos();
			if (nTestPpCmdPos != -1) {
				nLocJobIdx = nTestPpCmdPos;
				g_TaskLoadTbl[m_eTblIndex].SetTestppPrePos(-1);
			}
		}
			
		if (nLocJobIdx == -1) {
			return eExcutingStep_ErrorJob;
		}
		else {
			g_TaskLoadTbl[m_eTblIndex].SetTableCurPos(nLocJobIdx);
		}
		
		
		//nMotorPos = g_TaskLoadTbl[m_eTblIndex].ChkMotorPos(nLocJobIdx, CTaskLoadTable::eAxisAll);

		//Table Step 확인 추가.
		int table_col_step = g_TaskLoadTbl[m_eTblIndex].GetTableColStep();

		if (m_eTblIndex == 1)
			int TEST = 0;

		nMotorPos = g_TaskLoadTbl[m_eTblIndex].ChkMotorPos(nLocJobIdx, CTaskLoadTable::eAxisAll, table_col_step);
		if (nMotorPos != ERR_NO_ERROR)
			return eExcutingStep_Check_Work_Enable_TestSide;
	}
	return eExcutingStep_ErrorJob;
}

int CTaskAutoLoadTable::GrrFindJobState(int &nLocJobIdx)
{
	int nHandIdx = m_eTblIndex / 2;
	//interlock move
	if (g_TaskLoadTbl[m_eTblIndex].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
		return eExcutingStep_ErrorJob;

	int nTblRes = g_TaskLoadTbl[m_eTblIndex].GetWorkReserve();
	// lot init start 거나 tray hand 일 경우


	// System 초기, Test PP가 일을 다하고 놓아준 상황
	if (nTblRes == RID_INIT_IDLE_RESERVE) {
		nLocJobIdx = -1;
		return eExcutingStep_Check_Work_Enable_LdSide;
	}

	if (nTblRes == RID_TRAY_PP_(RIDX_TRAY_PP_1) || nTblRes == RID_TRAY_PP_(RIDX_TRAY_PP_2))
	{
		nLocJobIdx = -1;

		int table_row_step = g_TaskLoadTbl[m_eTblIndex].GetTableRowStep();

		int nMotorPos = g_TaskLoadTbl[m_eTblIndex].ChkMotorPos(LOC_IDX_TBL_LD_SIDE, CTaskLoadTable::eAxisAll, table_row_step);
		if (nMotorPos != ERR_NO_ERROR) {
			return eExcutingStep_Check_Work_Enable_LdSide;
		}
	}
	// Test hand거나 Tray hand가 일을 다하고 놓아준 상황
	else if ((nTblRes == RID_TEST_PP_(nHandIdx) || nTblRes == RID_TEST_PP_(nHandIdx) || nTblRes == RID_NOT_RESERVE))
	{
		int nMotorPos = ERR_NO_ERROR;

		// 1. Test 진행 or Test 완료 Check.
		BOOL bAutoGrrUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);
		BOOL bChkTestComplete = FALSE;
		int nLoadTblDevCnt = g_TaskLoadTbl[m_eTblIndex].GetLoadDeviceCnt();

		//Station중 Test 완료된 것 Check.
		for (int i = 0; i < eMaxATS_Area; i++)
		{
			int nIdx = i + 4 * (m_eTblIndex / 2);

			ASSERT(ePressUnit_Down_1 <= nIdx && nIdx <= ePressUnit_Down_8);

			int nRetSocket = g_TaskPressUnit[nIdx].GetSocketOnCnt(bAutoGrrUse);
			BOOL bTestStatus = g_DMCont.m_dmEQP.GB(BDM0_TEST_STATUS_SITE1 + nIdx);

			if (nRetSocket > 0 && bTestStatus == FALSE) { //TRUE : test 진행 중, FALSE : test 완료.
				bChkTestComplete = TRUE;
				break;
			}
		}

		// 2,3번 Load Table은 비어있다면 Test Site에서 움직일 필요 없음
		BOOL bChkTestSite = FALSE;
		std::vector<int> vLocList;
		vLocList.clear();
		g_TaskLoadTbl[m_eTblIndex].WhereIsMotorPos(vLocList, CTaskLoadTable::eAxisAll);
		if ((vLocList[CTaskLoadTable::eAxisY] > CTaskLoadTable::eYPOS_LoadSideToSitePos1))
		{
			bChkTestSite = TRUE;
		}

		//1. EOT로 Index check
		int loc_job_index_chk_time = (bChkTestComplete == FALSE) ? FindStation_CompTime(eTime_SOT) : FindStation_CompTime(eTime_EOT);

		//time 무조건 찾도록 해야 겠다
		if (loc_job_index_chk_time == -1 && m_nPreContactIdx != -1) {
			//-1이 나온건 continue제외 하고 모두 Test진행 중.
			//m_nPreContactIdx 제외 후, 다시 찾기.
			loc_job_index_chk_time = FindStation_CompTime(eTime_SOT);

		}

		//2. Test Hand Reseve Check
		int loc_job_index_chk_reserve = eExcutingStep_ErrorJob;

		for (int i = 0; i < eMaxPressUnitCount / 2; i++)
		{
			int nIdx = i + 4 * (m_eTblIndex / 2);

			if (g_testpp_work_area[nHandIdx].get_reserved_task(nIdx, &g_TaskAutoTestPp[nHandIdx]) == true) {

				loc_job_index_chk_reserve = eJobIndex_TestSite1 + i;
				break;
			}
		}

		if (loc_job_index_chk_reserve == loc_job_index_chk_time)
		{
			if (loc_job_index_chk_reserve == -1) {
				int nTestPpCmdPos = g_TaskLoadTbl[m_eTblIndex].GetTestppPrePos();
				if (nTestPpCmdPos != -1) {
					nLocJobIdx = nTestPpCmdPos;
					g_TaskLoadTbl[m_eTblIndex].SetTestppPrePos(-1);
				}
			}
			else {
				nLocJobIdx = loc_job_index_chk_reserve;
			}
		}
		else
		{

			if (loc_job_index_chk_time == eExcutingStep_ErrorJob && loc_job_index_chk_reserve != eExcutingStep_ErrorJob)
			{
				// 초기 자재 넣을 때, Unload Table Load side에 둠.
				// Test Hand reserve 된 곳은 없는데 Table 미리 이동 시켜 둘 때.
				nLocJobIdx = loc_job_index_chk_reserve;
			}
			else if (loc_job_index_chk_reserve == eExcutingStep_ErrorJob && loc_job_index_chk_time != eExcutingStep_ErrorJob)
			{
				// 초기 자재 넣을 때, Loading table reserve 된 곳으로 호출.
				// 그 외, Test Hand reserve와 Time check Index 다를 경우 호출.
				if (g_TaskSystemCtrl.IsOneCycleModeOn() != TRUE) {
					int nTestPpCmdPos = g_TaskLoadTbl[m_eTblIndex].GetTestppPrePos();
					if (nTestPpCmdPos != -1) {
						nLocJobIdx = nTestPpCmdPos;
						g_TaskLoadTbl[m_eTblIndex].SetTestppPrePos(-1);
					}
					else {
						nLocJobIdx = loc_job_index_chk_time;
					}
				}
				else {
					nLocJobIdx = loc_job_index_chk_time;
				}
			}
			else
			{
				int nTestPpCmdPos = g_TaskLoadTbl[m_eTblIndex].GetTestppPrePos();
				if (nTestPpCmdPos != -1 && nTestPpCmdPos != loc_job_index_chk_reserve) {
					nLocJobIdx = nTestPpCmdPos;
				}
				else {
					nLocJobIdx = loc_job_index_chk_reserve;
				}
			}
		}

		if ((m_eTblIndex == eLoadTbl_2 || m_eTblIndex == eLoadTbl_3))
		{
			int test = g_TaskAllAutoStacker.GetLoadTraySupplyEnd();
			if (g_TaskSystemCtrl.GetAutoGrrCancel() == false /*&& g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == false*/)
			{
 				if (nLoadTblDevCnt == 0 /*&& m_bChkPause != TRUE*/) {
 					int nJobIdx = g_DMCont.m_dmTestPP.GN(NDM04_TESTHAND1_WORKING_IDX + (m_eTblIndex / 2));
 					if (nJobIdx > 3) {
 						return eExcutingStep_ErrorJob;
 					}
 					else {
 						int nUnloadTblIdx = (m_eTblIndex == eLoadTbl_2) ? eLoadTbl_1 : eLoadTbl_4;
 						int nUnloadTblCurPos = g_TaskLoadTbl[nUnloadTblIdx].GetTableCurPos();
 						/*
 						1. Unlaod 현재 Pos와 실제 찾은 위치 안맞으면 Unlaod 현재 위치로 이동
 
 						pick 이미 하고 Unload Table 안왔을 때
 						-. nJobIdx 잡혀 있기 때문에 Unload도 해당 Pos로 이동, Load Table도 그냥 그자리 있음.
 						*/
 						if (nUnloadTblCurPos != nLocJobIdx) {
 							nLocJobIdx = nJobIdx + 1;
 						}
 					}
 				}
			}
		}
		else
		{
			if (g_TaskLoadTbl[m_eTblIndex].GetUnloadDeviceCnt() == 8)
				return eExcutingStep_ErrorJob;
		}

		if (g_TaskSystemCtrl.GetAutoGrrCancel() == TRUE) {
			for (int i = 0; i < eMaxPressUnitCount; i++) {
				g_DMCont.m_dmContac_Up.SN(NDM2_PRESS_1_PRE_CONTACT_STATUS + i, FALSE);
			}

			int nTestPpCmdPos = g_TaskLoadTbl[m_eTblIndex].GetTestppPrePos();
			if (nTestPpCmdPos != -1) {
				nLocJobIdx = nTestPpCmdPos;
				g_TaskLoadTbl[m_eTblIndex].SetTestppPrePos(-1);
			}
		}

		if (nLocJobIdx == -1) {
			return eExcutingStep_ErrorJob;
		}
		else {
			g_TaskLoadTbl[m_eTblIndex].SetTableCurPos(nLocJobIdx);
		}

		//Table Idx Grr Mode 변환
		int nGrrTblIdx = g_TaskLoadTbl[m_eTblIndex].GrrSetTableIdx(nLocJobIdx);
		nLocJobIdx = nGrrTblIdx;

		nMotorPos = g_TaskLoadTbl[m_eTblIndex].ChkMotorPos(/*nLocJobIdx*/nGrrTblIdx, CTaskLoadTable::eAxisAll);
		if (nMotorPos != ERR_NO_ERROR)
			return eExcutingStep_Check_Work_Enable_TestSide;
	}



	return eExcutingStep_ErrorJob;
}


BOOL CTaskAutoLoadTable::UseAbleCheck()
{
	//사용 유무에 따른 origin 확인
	int nTestPPIdx = g_TaskLoadTbl[m_eTblIndex].LoadTblFromTestPPToIdx(m_eTblIndex);
	int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	BOOL bAutoUseGRR = FALSE;
	if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
		bAutoUseGRR = TRUE;

	
	if (g_TaskAutoTestPp[nTestPPIdx].GetModuleSocketOnCnt(bAutoUseGRR) > 0) {
		//Module에 사용 Site가 있으면 Load Table 확인
		if (g_TaskLoadTbl[m_eTblIndex].IsInitNeed() == TRUE) {
			return FALSE;
		}
		//사용할 Site 가 있고 Hand도 사용할수 있으면
		return TRUE;
	}
	else {
		//사용할 Site 가 없으면
		return FALSE;
	}
		
}

//전체 Station Test 중 일때, SOT 비교
int CTaskAutoLoadTable::FindStation_CompSOT()
{
	BOOL bAutoGrrUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);

	int nRetStationIdx = eJobIndex_None;

	double dMinSOT = 0.0;
	SYSTEMTIME sysMinTestTm;
	::GetLocalTime(&sysMinTestTm); //Initial

	BOOL bFindFstStation = FALSE;
	for (int i = 0; i < eMaxATS_Area; i++) {
		int nIdx = i + 4 * (m_eTblIndex / 2);

		//Station On만 확인.
		int nRetSocket = g_TaskPressUnit[nIdx].GetSocketOnCnt(bAutoGrrUse);
		if(nRetSocket == 0)
			continue;

		////////////////////////////////////////////////////////////////////////////
/*		double dTestTm = g_DMCont.m_dmEQP.GN(NDM0_Press1_TestTm + nIdx);*/

		int nCompResult = CompSystemTime(sysMinTestTm, g_SOT[nIdx]);

		if (bFindFstStation == FALSE || nCompResult == 1) //g_SOT[nIdx] < sysMinTestTm
		{
			sysMinTestTm = g_SOT[nIdx];
			nRetStationIdx = eJobIndex_TestSite1 + i;

			if (bFindFstStation == FALSE) 
				bFindFstStation = TRUE;
		}

		///////////////////////////////////////////////////////////////////////////
	}
	
	return nRetStationIdx;
}

int CTaskAutoLoadTable::FindStation_CompEOT() //EOT 제일 빠른거 찾는것
{
	

	int nRetStationIdx = eJobIndex_None;
	BOOL bFindFstStation = FALSE;

	SYSTEMTIME sysMaxTestTm;
	::GetLocalTime(&sysMaxTestTm);

	BOOL bAutoGrrUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);
	for (int i = 0; i < eMaxATS_Area; i++) {
		int nIdx = i + 4 * (m_eTblIndex / 2);

		//Station On만 확인.
		int nRetSocket = g_TaskPressUnit[nIdx].GetSocketOnCnt(bAutoGrrUse);
		if (nRetSocket == 0)
			continue;

		/*double dTestTm = g_DMCont.m_dmEQP.GN(NDM0_Press1_TestTm + nIdx);*/
		int nCompResult = CompSystemTime(sysMaxTestTm, g_EOT[nIdx]);

		if (bFindFstStation == FALSE || nCompResult == 1) // g_EOT[nIdx] < sysMaxTestTm
		{
			sysMaxTestTm = g_EOT[nIdx];
			nRetStationIdx = eJobIndex_TestSite1 + i;

			if (bFindFstStation == FALSE)
				bFindFstStation = TRUE;
		}
	}

	return nRetStationIdx;
}


/////////////////////////////////////////////////////////////
// SOT, EOT 중 빠른 시간 찾음.
// EOT는 Test 완료 된 Station만 비교.
// RETURN : 해당 Station Index 
////////////////////////////////////////////////////////////
int CTaskAutoLoadTable::FindStation_CompTime(int nTimeType)
{
	int nRetJobIdx = eJobIndex_None;
	BOOL bFindFstStation = FALSE;

	SYSTEMTIME sysCompTestTm;
	::GetLocalTime(&sysCompTestTm);

	BOOL bAutoGrrUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);
	for (int i = 0; i < eMaxATS_Area; i++) {
		int nIdx = i + 4 * (m_eTblIndex / 2);

		if (m_nPreContactIdx == nIdx) {
			continue;
		}

		//Station On 확인.
		int nRetSocket = g_TaskPressUnit[nIdx].GetSocketOnCnt(bAutoGrrUse);
		if (nRetSocket == 0)
			continue;

		int nStationInfo = g_TaskPressUnit[nIdx].GetStationInfo(bAutoGrrUse);
		if (nStationInfo >= eTestSite_DvcLoading && nStationInfo <= eTestSite_Testing) {
			m_nPreContactIdx = nIdx;
			continue;
		}

 		int nContactStat = g_DMCont.m_dmContac_Up.GN(NDM2_PRESS_1_PRE_CONTACT_STATUS + nIdx);
 		if (nContactStat == TRUE /*&& g_TaskLoadTbl[m_eTblIndex].GetTableUsage() == eLoadType*/ && g_TaskSystemCtrl.IsOneCycleModeOn() != TRUE) {
 			m_nPreContactIdx = nIdx;
 			continue;
 		}

		//EOT Test End 확인.
		BOOL bTestStatus = g_DMCont.m_dmEQP.GB(BDM0_TEST_STATUS_SITE1 + nIdx);
		BOOL bContactStatus = g_DMCont.m_dmEQP.GB(BDM0_PRESS_CONTACT_STATUS_SITE1 + nIdx);

		if (nTimeType == eTime_EOT && (bTestStatus == TRUE || bContactStatus == TRUE)) { //test 진행, Contact 진행중.
			continue;
  		}

		SYSTEMTIME sysTestTm = (nTimeType == eTime_SOT) ? g_SOT[nIdx] : g_EOT[nIdx];
		int nCompResult = CompSystemTime(sysCompTestTm, sysTestTm);

		// Station 비교 후, 제일 빠른 시간
		if ( nCompResult == 1) // sysTestTm < sysCompTestTm
		{
			sysCompTestTm = sysTestTm;
			nRetJobIdx = eJobIndex_TestSite1 + i;
		}	
	}

	if (nRetJobIdx != -1) {
		m_nPreContactIdx = -1;
	}


	return nRetJobIdx;
}

////////////////////////////////////////////////////////////////////////////////////
// Return : SysTime1 > SysTime2    1
//			SysTime1 < SysTime2    -1
//			SysTime1 == SysTime2    0
////////////////////////////////////////////////////////////////////////////////////
int CTaskAutoLoadTable::CompSystemTime(SYSTEMTIME SysTime1, SYSTEMTIME SysTime2) //기존, 신규
{
	//FileTime으로 변환 후, 시간 비교.
	FILETIME FileTime1, FileTime2;
	SystemTimeToFileTime(&SysTime1, &FileTime1); //Time1 변환.
	SystemTimeToFileTime(&SysTime2, &FileTime2); //Time2 변환.

	int nRet = CompareFileTime(&FileTime1, &FileTime2);
	
	//millisecond 비교
	//Station이 동시에 끝날 경우 초단위 동일.
	if (nRet == 0) 
	{
		if (SysTime1.wMilliseconds > SysTime2.wMilliseconds){
			nRet = 1;
		}
		else if (SysTime1.wMilliseconds < SysTime2.wMilliseconds) {
			nRet = -1;
		}
		else {
			nRet = 0;
		}
	}
	return nRet;
}


BOOL CTaskAutoLoadTable::EnableMoveTimeIdx()
{
	BOOL nRet = FALSE;
	//device 들고 있거나
	//device 안들고 있으면 움직이고 있거나
	int nType = g_TaskLoadTbl[m_eTblIndex].GetTableUsage();
	int nDeviceCnt = 0;

	vector<CPoint> vPkr;
	g_TaskTestPP[m_eTblIndex/2].MakeAllPicker(vPkr);
	int nChkVac = g_TaskTestPP[m_eTblIndex / 2].ChkVacuum(DEF_ON, nType, vPkr);

	if (nType == eLoadType) {
		nDeviceCnt = g_TaskTestPP[m_eTblIndex / 2].GetLoadHandDeviceCnt();
		
		if (nDeviceCnt > 0 || (nDeviceCnt == 0 && nChkVac != ERR_NO_ERROR)/*g_TaskTestPP[m_eTblIndex / 2].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Executing)*/)
		{
			nRet = TRUE;
		}
	}
	else {
		//device 안들고 있는 경우
		// 1) 대기 중 2)Unload Device load 상태
		nDeviceCnt = g_TaskTestPP[m_eTblIndex / 2].GetUnLoadHandDeviceCnt();
		if (nDeviceCnt == 0)
			nRet = TRUE;
	}
	return nRet;
}