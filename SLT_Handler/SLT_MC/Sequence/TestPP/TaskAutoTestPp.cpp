#include "StdAfx.h"
#include "TaskAutoTestPp.h"


CTaskAutoTestPp::CTaskAutoTestPp(void)
{
	m_nAutoRunMainStep = eAutoLotState_InitNeed;
	m_nAutoRunSubStep = 0;
	m_nAutoRunBackUpMainStep = eAutoLotState_InitNeed;
	m_nAutoRunBackUpSubStep = 0;
	m_eTestIdx = eTestPP_1;

	m_nCleanMode = eOPTION_NOT_USE;

	m_vModuleSetPressIdx.clear();
	m_vModuleSetLoadTableIdx.clear();

	m_nJobAtsIdx = -1;

	m_vJobPicker_Pick.clear();
	m_vJobPicker_Place.clear();

	m_mapExcuteState.SetAt( eExcutingStep_Start                 , _T( "Start" ) );
	m_mapExcuteState.SetAt( eExcutingStep_End                   , _T( "End" ) );

	m_mapExcuteState.SetAt( eExcutingStep_Move_PickLoadTable    , _T( "Move Pick LdTbl" ) );
	m_mapExcuteState.SetAt( eExcutingStep_WaitEnd_PickLoadTable , _T( "Wait Pick LdTbl" ) );

// 		// ART buffer table pick/place
	m_mapExcuteState.SetAt( eExcutingStep_Move_PickArtBuffTable    , _T( "Move Pick ArtBuff" ) );
	m_mapExcuteState.SetAt( eExcutingStep_WaitEnd_PickArtBuffTable , _T( "Wait Pick ArtBuff" ) );
	m_mapExcuteState.SetAt( eExcutingStep_Move_PlaceArtBuffTable   , _T( "Move Place ArtBuff" ) );
	m_mapExcuteState.SetAt( eExcutingStep_WaitEnd_PlaceArtBuffTable, _T( "Wait Place ArtBuff" ) );
		// Clean buffer table pick/place
	m_mapExcuteState.SetAt(eExcutingStep_Move_PickCleanTable    , _T( "Move Pick CleanBuff" ) );
	m_mapExcuteState.SetAt(eExcutingStep_WaitEnd_PickCleanTable , _T( "Wait Pick CleanBuff" ) );
	m_mapExcuteState.SetAt(eExcutingStep_Move_PlaceCleanTable   , _T( "Move Place CleanBuff" ) );
	m_mapExcuteState.SetAt(eExcutingStep_WaitEnd_PlaceCleanTable, _T( "Wait Place CleanBuff" ) );
		// Station pick/place/Exchange
	m_mapExcuteState.SetAt(eExcutingStep_Move_PickStation     , _T( "Move Pick Station" ) );
	m_mapExcuteState.SetAt(eExcutingStep_WaitEnd_PickStation  , _T( "Wait Pick Station" ) );
	m_mapExcuteState.SetAt(eExcutingStep_Move_PlaceStation    , _T( "Move Place Station" ) );
	m_mapExcuteState.SetAt(eExcutingStep_WaitEnd_PlaceStation , _T( "Wait Place Station" ) );
// 	m_mapExcuteState.SetAt(eExcutingStep_Move_ExchStation     , _T( "Move Exchange Station" ) );
// 	m_mapExcuteState.SetAt(eExcutingStep_WaitEnd_ExchStation  , _T( "Wait Exchange Station" ) );
// 
// 	m_mapExcuteState.SetAt(eExcutingStep_Move_PlaceLoadTable_2dErrDvc, _T("Move Place 2dErrDvc"));
// 	m_mapExcuteState.SetAt(eExcutingStep_WaitEnd_PlaceLoadTable_2dErrDvc, _T("Wait Place 2dErrDvc"));

	m_mapExcuteState.SetAt(eExcutingStep_Move_Already_Pick_Dvc_PlaceArtBuffTable, _T("Move Already Pick Dvc place ArtBuff"));
	m_mapExcuteState.SetAt(eExcutingStep_WaitEnd_Already_Pick_Dvc__PlaceArtBuffTable, _T("Wait Already Pick Dvc place ArtBuff"));
// 	m_mapExcuteState.SetAt(eExcutingStep_Move_SamePickPlace_LoadTable, _T("Move Same Pick Place LdTbl"));
// 	m_mapExcuteState.SetAt(eExcutingStep_WaitEnd_SamePickPlace_LoadTable, _T("Wait Same Pick Place LdTbl"));

	m_vLossPkr[eTestPPLoadType].clear();
	m_vLossPkr[eTestPPUnLoadType].clear();
	m_bAlreadyPick = FALSE;

	m_nJobSiteRow = 0;

	m_bNotChkPreContactPos = FALSE;

	m_nLoadingTableInx = -1;
	m_nUnLoadingTableInx = -1;

}


CTaskAutoTestPp::~CTaskAutoTestPp(void)
{
}

int CTaskAutoTestPp::Initialize( TCHAR * szDriverFileName, _eTestPPIdx eTestIdx )
{
	m_eTestIdx = eTestIdx;

	// ** Index 순차적으로 들어가야하니 바꾸지마세요
	if(m_eTestIdx == eTestPP_1)
	{
		m_vModuleSetPressIdx.push_back(ePressUnit_Down_1);
		m_vModuleSetPressIdx.push_back(ePressUnit_Down_2);
		m_vModuleSetPressIdx.push_back(ePressUnit_Down_3);
		m_vModuleSetPressIdx.push_back(ePressUnit_Down_4);

		m_vModuleSetLoadTableIdx.push_back(eLoadTbl_1);
		m_vModuleSetLoadTableIdx.push_back(eLoadTbl_2);
		
		m_nLoadingTableInx = eLoadTbl_2;
		m_nUnLoadingTableInx = eLoadTbl_1;
	}
	else
	{
		m_vModuleSetPressIdx.push_back(ePressUnit_Down_5);
		m_vModuleSetPressIdx.push_back(ePressUnit_Down_6);
		m_vModuleSetPressIdx.push_back(ePressUnit_Down_7);
		m_vModuleSetPressIdx.push_back(ePressUnit_Down_8);

		//임시
		m_vModuleSetLoadTableIdx.push_back(eLoadTbl_4);
		m_vModuleSetLoadTableIdx.push_back(eLoadTbl_3);

		m_nLoadingTableInx = eLoadTbl_3;
		m_nUnLoadingTableInx = eLoadTbl_4;
	}
	
	g_testpp_work_area[eTestIdx].SetTestPpIdx(eTestIdx);
	g_testpp_work_area[eTestIdx].SetLoadTableIdx(eTestIdx);

	if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TESTPP_STATION_FR_PNP) == FALSE)
		m_nJobSiteRow = 0;
	else
		m_nJobSiteRow = 1;

	return CBaseTaskManager::Initialize();
}

void CTaskAutoTestPp::Finalize()
{
	CBaseTaskManager::Finalize();
}


int CTaskAutoTestPp::OnEvent( const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList/*=NULL*/ )
{
	return ERR_NO_ERROR;
}

int CTaskAutoTestPp::OnCommandRsp( CBaseSeqCmd * pRspCmd, int nEventId, WPARAM wParam, LPARAM lParam, std::vector<CPoint>& vPocket_Picker )
{
	if( nEventId == CMD_EVT_ALARM )
	{
		if( pRspCmd->m_pSender == this )
			ChangeMainStateByRspAlarm();
	}

	return CBaseTaskManager::OnCommandRsp( pRspCmd, nEventId, wParam, lParam, vPocket_Picker );;
}

void CTaskAutoTestPp::OnOperatorCommand( int nOperatorCommand, WPARAM wParam, LPARAM lParam )
{
}

void CTaskAutoTestPp::ThreadProc1()
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

void CTaskAutoTestPp::MakeLog( LPCTSTR fmt, ... )
{
	TCHAR tmpMsg[ 1024 ] = { 0, };
	if( fmt )
	{
		va_list argList;
		va_start( argList, fmt );
		_vstprintf_s( tmpMsg, fmt, argList );
		va_end( argList );
	}
	TWLOG.MakeLog( static_cast<int>( eLogID_Auto_Test_1 + m_eTestIdx ), Debug, "", __LINE__, NULL, tmpMsg );
}

void CTaskAutoTestPp::AutoState_Idle()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 1000:
		{
			ChangeSubState(1000);
		}break;
	case 1001:
		{
// 			//2D Barcode Light On
// 			if (m_eTestIdx == eTestPP_1) {
// 				g_pVisionTestHand_1->Cmd_ReqMeasure_BarcodeLED(eBarcodeLight_ON, m_eTestIdx);
// 			}
// 			else {
// 				g_pVisionTestHand_2->Cmd_ReqMeasure_BarcodeLED(eBarcodeLight_ON, m_eTestIdx);
// 			}

			ChangeSubState(1002);
		}break;
		case 1002:
		{

		}break;
		default :
			ChangeSubState(1000);
			break;
	}
}

void CTaskAutoTestPp::AutoState_InitNeed()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 2000:
		{
			g_TaskTestPP[ m_eTestIdx ].SetProcessCommand( base_seq_cmd::eProcCmd_Stop );

			for(int i=0; i < eMaxATS_Area; i++)
				g_testpp_work_area[m_eTestIdx].release_work_area(m_vModuleSetPressIdx[i], this);

			ChangeSubState(2001);
		}break;
	case 2001:
		{
			
		}break;
	default :
		ChangeSubState(2000);
		break;
	}
}

void CTaskAutoTestPp::AutoState_Initialize()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 3000:
		{
			char szCompany[16] = { 0, };
			g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

			int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
			BOOL m_bGRR_Audit_loop = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrAuditLoopUse);

			int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
			BOOL bAutoUseGRR = FALSE;
			if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
				bAutoUseGRR = TRUE;

			//if (nProjectOption == DEF_PROJECT_MAUI) {
				if (g_TaskAutoTestPp[m_eTestIdx].GetModuleSocketOnCnt(bAutoUseGRR) == 0 
					&& g_TaskAutoTestPp[m_eTestIdx].GetModuleAuditSocketOnCnt() == 0
					&& strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
					ChangeSubState(3100);
					break;
				}
			//}


			int nInitDoneCnt = 0;
			for(int i=0; i < (int)m_vModuleSetPressIdx.size(); i++ )
			{
				if( g_TaskPressUnit[ m_vModuleSetPressIdx[i] ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready
				&& g_TaskPressUnit[ m_vModuleSetPressIdx[i] ].m_bInitialized == TRUE )
					nInitDoneCnt++;
			}

			if( nInitDoneCnt == eMaxATS_Area )
				ChangeSubState(3001);
		}break;
	case 3001 :
		{
			if( g_TaskTestPP[ m_eTestIdx ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready )
			{
				CTestPPCmd_Initial* pSeqCmd = new CTestPPCmd_Initial;
				if( SendCommand( g_TaskTestPP[ m_eTestIdx ], pSeqCmd ) == eSeqCmdAck_OK )
					ChangeSubState(3002);
			}
		}break; 
	case 3002:
		{
			if( g_TaskTestPP[ m_eTestIdx ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready 
			&& g_TaskTestPP[ m_eTestIdx ].m_bInitialized == TRUE )
				ChangeSubState(3003);
		}break;
	case 3003: // Blow Warming up 기능 추가
	{
		int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
		int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);

		if ((nLotStartMode == eStartMD_Initial || nLotStartMode == eStartMD_Retest || nLotStartMode == eStartMD_QA || nLotStartMode == eStartMD_AutoLoop || nLotStartMode == eStartMD_AutoGRR))
		{
			if (nDeviceMode == eDeviceMode && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
			{
				for (int nHandType = 0; nHandType < eMaxTestPPTypeCount; nHandType++)
				{
					for (int y = 0; y < MAX_TEST_PP_PKR_CNT_Y; y++) {
						for (int x = 0; x < MAX_TEST_PP_PKR_CNT_X; x++) {
							int nVac = g_TaskTestPP[m_eTestIdx].GetVacuum(nHandType,CPoint(x, y));
							if (nVac == DEF_OFF) {
								g_TaskTestPP[m_eTestIdx].do_VacBlow(eVacBlwSt_BlwOn, nHandType, CPoint(x, y));
							}
						}
					}
				}
				
				int nMemBlowTime = m_eTestIdx == eTestPP_1 ? NDM4_TM_TestPP1_Blow : NDM4_TM_TestPP2_Blow;
				int nSleep = g_DMCont.m_dmTestPP.GN(nMemBlowTime);
				if (nSleep > 0) {
					Sleep(nSleep);
				}

				for (int nHandType = 0; nHandType < eMaxTestPPTypeCount; nHandType++)
				{
					for (int y = 0; y < MAX_TEST_PP_PKR_CNT_Y; y++) {
						for (int x = 0; x < MAX_TEST_PP_PKR_CNT_X; x++) {
							int nVac = g_TaskTestPP[m_eTestIdx].GetVacuum(nHandType, CPoint(x, y));
							if (nVac == DEF_OFF) {
								g_TaskTestPP[m_eTestIdx].do_VacBlow(eVacBlwSt_Off, nHandType, CPoint(x, y));
							}
						}
					}
				}
			}
		}
		ChangeSubState(3100);
	}break;
		//-------------------------------------------------------
		// init complete
		//-------------------------------------------------------
	case 3100 :
		{
			ChangeMainState( eAutoLotState_Idle );
		}break;
	default :
		ChangeSubState(3000);
		break;
	}
}

void CTaskAutoTestPp::AutoState_Pause()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		//MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
		MakeLog(_T("[MainStep=%s] [SubStep=%d] [last step=%d] [JobIndex=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nLast_Excuting_Step,m_nJobAtsIdx);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 4000:
		{
			g_TaskTestPP[ m_eTestIdx ].SetProcessCommand( base_seq_cmd::eProcCmd_Pause );
			m_bNotChkPreContactPos = TRUE;
			ChangeSubState(4001);
		}break;
	case 4001:
		{

		}break;
	default :
		ChangeSubState(4000);
		break;
	}
}

void CTaskAutoTestPp::AutoState_Execute()
{
	//if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
	//	MakeLog( _T( "[MainStep=%s] [SubStep=%s]" ), GetLotStateToString( m_nAutoRunMainStep ), GetExcuteStateStr() );
	//}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	if( m_nAutoRunSubStep > eExcutingStep_End ) {
		m_nAutoRunSubStep = m_nLast_Excuting_Step;
		
	}
// 	if(m_eTestIdx == eTestPP_2)
// 		return;

	m_nLast_Excuting_Step = m_nAutoRunSubStep;
	g_DMCont.m_dmTestPP.SN(NDM4_TestPP1_Auto_Excuting_Step + m_eTestIdx, m_nAutoRunSubStep);
	
	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	if (ModuleUseAbleCheck() != TRUE && strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
		return;
	}

	if (m_eTestIdx == eTestPP_1) {
		int a = 0;
	}

	int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	switch (nLotStartMode)
	{
	case eStartMD_Initial:			case eStartMD_Retest:
	case eStartMD_InitContinue:     case eStartMD_RetestContinue:
	case eStartMD_QA:				case eStartMD_QA_Continue:
	//case eStartMD_AutoLoop:     case eStartMD_AutoLoop_Continue:
	{
		AutoState_Execute_Normal();
	}break;
	case eStartMD_AutoGRR:
	case eStartMD_AutoGRR_Continue:
	{
		AutoState_Execute_AutoGrr();
 	}break;
// 	case eStartMD_QA:
// 	case eStartMD_QA_Continue:
// 	{
// 		AutoState_Execute_QaSample();
// 	}break;
// 	case eStartMD_AutoLoop:
// 	case eStartMD_AutoLoop_Continue:
// 	{
// 		AutoState_Execute_AutoLoop();
// 	}break;
// 	case eStartMD_AutoAudit:
// 	case eStartMD_AutoAudit_Continue:
// 	{
// 		AutoState_Execute_AutoAudit();
// 	}break;
	default: ASSERT(false);
		break;
	}
	
	m_nLast_Excuting_Step = m_nAutoRunSubStep;
}

void CTaskAutoTestPp::AutoState_Execute_Normal()
{
	switch (m_nAutoRunSubStep)
	{
	case eExcutingStep_Start:
	{
		m_nJobAtsIdx = g_DMCont.m_dmTestPP.GN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx); // 저장 된 마지막 idx로 복구
		//MakeLog(_T("[eExcutingStep_Check_Job, WorkingStart=%d"), m_nJobAtsIdx);

		int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
		BOOL bAutoUseGRR = FALSE;
		if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
			bAutoUseGRR = TRUE;

		if (GetModuleSocketOnCnt(bAutoUseGRR) == 0)
			break;
		else
		{
			if (m_nJobAtsIdx > eATS_DN_STATION_L4 || m_nJobAtsIdx < eATS_DN_STATION_L1)
			{
				m_nJobAtsIdx = eATS_DN_STATION_L1;
			}

			CTestPPCmd_MoveAlreadyPos* pSeqCmd = new CTestPPCmd_MoveAlreadyPos;
			pSeqCmd->m_nCmdStage = LOC_IDX_TEST_PP_TBL_1_LD + m_nJobAtsIdx;
			pSeqCmd->m_cCmdPocket = CPoint(0, 0);
			pSeqCmd->m_vCmdPicker.push_back(CPoint(0, 0));
			pSeqCmd->m_nHandType = TEST_LD_HAND;
			pSeqCmd->m_bAutoRunXHomePos = TRUE;
			if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) == eSeqCmdAck_OK)
				ChangeSubState(eExcutingStep_Check_Job);
		}
	}break;

	case eExcutingStep_Check_Job:
	{
		int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
		BOOL bAutoUseGRR = FALSE;
		if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
			bAutoUseGRR = TRUE;

		if (GetModuleSocketOnCnt(bAutoUseGRR) == 0)
			break;

		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
			break;

	
		m_nJobAtsIdx = g_DMCont.m_dmTestPP.GN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx);

		
		if (m_nJobAtsIdx > eATS_DN_STATION_L4 || m_nJobAtsIdx < eATS_DN_STATION_L1)
		{
			m_nJobAtsIdx = eATS_DN_STATION_L1;
		}


 		// 모든 Station이 Test 중이고, 할일이 없으면 Load Table에 있는 자재를 Pick한다.
//  		int nAllTesting = 0;
//  		for (auto mdIdx : m_vModuleSetPressIdx)
//  		{
//  			if (g_DMCont.m_dmEQP.GB(BDM0_TEST_STATUS_SITE1 + m_vModuleSetPressIdx[m_nJobAtsIdx]) == TRUE)
//  				nAllTesting++;
//  		}
//  
//  		if (nAllTesting == m_vModuleSetPressIdx.size())
//  		{
//  			ChangeSubState(eExcutingStep_Move_Already_All_Test_PickLoadTable);
//  			break;
//  		}
 		


		int nWorkPossibleCnt = 0;
		if (g_TaskSystemCtrl.IsOneCycleModeOn() == true || g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == true)
		{ /// Reserve Job Station 1~4번 모두 검색 하여 예약 되어 있으면 해제
			for (int i = 0; i < eMaxATS_Area; i++)
			{
				bool bWorkPossible = g_testpp_work_area[m_eTestIdx].get_reserved_task(m_vModuleSetPressIdx[i], this);
				if (bWorkPossible == true)
				{
					g_testpp_work_area[m_eTestIdx].release_work_area(m_vModuleSetPressIdx[i], this);
				}			
			}	
		}

		bool bWorkPossible = g_testpp_work_area[m_eTestIdx].is_work_enable_state(m_vModuleSetPressIdx[m_nJobAtsIdx], this);

		int nChkInterlock = ChkInterlockAts(m_nJobAtsIdx);
		int nStationOnCnt = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetSocketOnCnt();
		int nStationInDvc = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetSiteInDvc();

		bool bLoadBreak = true;
		
		for (auto nTableIdx : m_vModuleSetLoadTableIdx)
		{
			if (g_TaskSystemCtrl.IsOneCycleModeOn() == FALSE || g_TaskTrayLoadPP.GetTrayPpLoadDvcCnt() > 0
				|| g_TaskLoadTbl[nTableIdx].GetLoadDeviceCnt() > 0 || g_TaskLoadTbl[nTableIdx].GetUnloadDeviceCnt() > 0
				|| g_TaskTestPP[m_eTestIdx].GetBufferTableCnt() > 0 || nStationInDvc > 0
				|| g_TaskTestPP[m_eTestIdx].GetLoadPickerDeviceCnt() > 0
				|| g_TaskTestPP[m_eTestIdx].GetUnloadPickerHaveDvcCnt() > 0)
			{
				bLoadBreak = false;
				break;
			}
		}

		int nStationInfo = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetStationInfo();

		m_nCleanMode = ChkCleaningMode(m_nJobAtsIdx);

		m_bAlreadyPick = FALSE;

		int nChkSiteInDvc = 0;

		if (m_nUnLoadingTableInx == eLoadTbl_1 || m_nUnLoadingTableInx == eLoadTbl_4)
		{
			for (auto mdIdx : m_vModuleSetPressIdx)
				nChkSiteInDvc += g_TaskPressUnit[mdIdx].ChkSiteInDvc();
		}

		if (g_TaskAllAutoStacker.GetLoadEnableStackerCount() == 0 && nChkSiteInDvc == 0 && g_TaskLoadTbl[m_nUnLoadingTableInx].GetUnloadDeviceCnt() > 0)
		{    // 투입 될 Device는 없고, Site에 Device가 없을때 예약이 걸려있으면 풀어준다.
			if (g_TaskLoadTbl[m_nUnLoadingTableInx].GetWorkReserve() == RID_TEST_PP_(m_eTestIdx))
			{
				MakeLog("Work Reserve");
				g_TaskLoadTbl[m_nUnLoadingTableInx].SetWorkReserve(RID_INIT_IDLE_RESERVE);
			}

		}

		// Load Table이 도착 하고, Station은 모두 Test 중이라면 미리 Pick 해보기
		{
			BOOL bCleanMode = FALSE;
			for (int i = 0; i < (int)m_vModuleSetPressIdx.size(); i++)
			{
				if (ChkCleaningMode(i) == TRUE)
				{
					bCleanMode = TRUE;
					break;
				}

			}

			BOOL bTesting = TRUE;
			BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
			for (int i = 0; i < (int)m_vModuleSetPressIdx.size(); i++)
			{
				int nGetStationInfo = g_TaskPressUnit[m_vModuleSetPressIdx[i]].GetStationInfo(bAutoGrrUse,TRUE);
				int nStationOnCnt = g_TaskPressUnit[m_vModuleSetPressIdx[i]].GetSocketOnCnt();
				if ((g_DMCont.m_dmEQP.GB(BDM0_TEST_STATUS_SITE1 + m_vModuleSetPressIdx[i]) == FALSE ||
					(nGetStationInfo != eTestSite_TestStart && nGetStationInfo != eTestSite_Contacting && nGetStationInfo != eTestSite_Testing && nGetStationInfo != eTestSite_DvcLoading
						&& nGetStationInfo != eTestSite_ContactStart)) && nStationOnCnt > 0)
				{
					bTesting = FALSE;
					break;
				}
					
			}
			BOOL bAlreadyPickOpt = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TEST_LOAD_HAND_ALREADY_PICK);
			int nStationOnCnt = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetSocketOnCnt();
			int nLoadTableWorkPos = GetReserveAndLoadTblWorkPos(m_nLoadingTableInx);
			if (bCleanMode != TRUE && bTesting == TRUE && nLoadTableWorkPos == TRUE
				&& g_TaskLoadTbl[m_nLoadingTableInx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready
				&& g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].PressInCleanDeviceCheck(FALSE) == FALSE
				&& nStationOnCnt > 0
				&& g_TaskTestPP[m_eTestIdx].GetLoadHandDeviceCnt() == 0
				&& g_TaskSystemCtrl.IsOneCycleModeOn() != true
				&& g_TaskAllAutoStacker.GetLoadTraySupplyEnd() != true
				&& g_TaskTestPP[m_eTestIdx].GetBufferTableCnt() == 0
				&& g_TaskLoadTbl[m_nLoadingTableInx].GetLoadDeviceCnt() > 0
				&& bAlreadyPickOpt == TRUE)

			{
				MakeLog(_T("[==== %s Check Job Load Table Already Pick =====]"), g_TaskTestPP[m_eTestIdx].GetLocName(LOC_IDX_STATION_1 + m_nJobAtsIdx));
				ChangeSubState(eExcutingStep_Move_Already_All_Test_PickLoadTable);
				break;
			}
		}

		int nTblUldDvcCnt = g_TaskLoadTbl[m_nUnLoadingTableInx].GetUnloadDeviceCnt();
		int nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
		int nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
		int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
		int nMotorUnloadPos = g_TaskLoadTbl[m_nUnLoadingTableInx].ChkMotorPos(LOC_IDX_LD_TBL_TEST_SIDE_1 + m_nJobAtsIdx, CTaskLoadTable::eAxisY);
		OneAxis* pMotorY = g_TaskLoadTbl[m_nUnLoadingTableInx].GetAxis(CTaskLoadTable::eAxisY);
		OneAxis* pMotorR = g_TaskLoadTbl[m_nUnLoadingTableInx].GetAxis(CTaskLoadTable::eAxisRotate);


		if ((g_TaskLoadTbl[m_nUnLoadingTableInx].GetWorkReserve() == RID_TEST_PP_(m_eTestIdx) && pMotorY->MotionDone() == ERR_NO_ERROR && pMotorR->MotionDone() == ERR_NO_ERROR/*nMotorUnloadPos == ERR_NO_ERROR*/ && nTblUldDvcCnt > 0 && g_TaskLoadTbl[m_nUnLoadingTableInx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready))
		{
			if ((nTestSite_Div_Y == STATION_MAX_Y_SITE)
				|| (nTestSite_Div_Y == 1 && (nTblUldDvcCnt > nTestSite_Div_X * nTestSite_Div_Y))
				|| (nTestSite_Div_Y == 1 && nBoardType == eBoardDir_Backward /*&& nTblUldDvcCnt != 0*/)) // 해당 조건 항상 참
			{
				MakeLog(_T("[CheckJob SetWorkReserve UnLoad Table job = [%d]]"), m_nJobAtsIdx + 1);
				g_TaskLoadTbl[m_nUnLoadingTableInx].SetWorkReserve(RID_INIT_IDLE_RESERVE/*RID_TRAY_PP_(RIDX_TRAY_PP_1)*/);
			}
		}

		// cleaning mode에 의한 
		if (bWorkPossible == true && bLoadBreak == false && nChkInterlock == ERR_NO_ERROR && (nStationOnCnt > 0 || nStationInDvc > 0) && nStationInfo != eTestSite_DvcLoading && nStationInfo != eTestSite_CleanLoading)
		{
			// cleaning mode인데 retest buffer에 갈 수량 + buffer수량이 오버가 되면 일단 skip하도록 한다.
			BOOL bAutoRetetsMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);

			int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
			int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
			int nCntSite = 0;
			ST_DD_DEVICE stSiteDvc;

			for (int nY = 0; nY < nSiteDivY; nY++)
			{
				for (int nX = 0; nX < nSiteDivX; nX++)
				{
					stSiteDvc.clear();
					int nDvc = g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1 + m_vModuleSetPressIdx[m_nJobAtsIdx], nX, nY, &stSiteDvc);
					if (stSiteDvc.sExist == eDeviceData_Exist)
					{
						BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stSiteDvc);

						if (strcmp(stSiteDvc.szErrString, DEF_TEST_RESULT_TP101) == 0 || bIsDefaultBin == TRUE
							|| (stSiteDvc.nTestingCnt == eARB_2stTestEnd && stSiteDvc.nBinColor == eFailBin))
							nCntSite++;
					}
				}
			}

			if (m_nCleanMode == TRUE && bAutoRetetsMode == TRUE)
			{
				int nBufferDvcCnt = g_TaskTestPP[m_eTestIdx].GetBufferTableCnt();
				int nSiteArtDvcCnt = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetSiteInDvc(TRUE);
				int nBufferPocketCnt = g_TaskTestPP[m_eTestIdx].GetBufferTablePocketCnt();


				if (nSiteArtDvcCnt + nBufferDvcCnt > nBufferPocketCnt
					|| (g_TaskTestPP[m_eTestIdx].GetLoadPickerLoadDeviceCnt() > nStationOnCnt && (nCntSite + nBufferDvcCnt + (g_TaskTestPP[m_eTestIdx].GetLoadPickerLoadDeviceCnt() - nStationOnCnt) > nBufferPocketCnt / 2)))
				{

					g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx, ++m_nJobAtsIdx);
					MakeLog(_T("[==== %s job 1 =====]"), g_TaskTestPP[m_eTestIdx].GetLocName(LOC_IDX_STATION_1 + m_nJobAtsIdx));
					break;
				}
			}
			// cleaning mode 인데 Load Picker에 Device 가 있을 경우
			if (m_nCleanMode == TRUE && g_TaskTestPP[m_eTestIdx].GetLoadPickerLoadDeviceCnt() > 0) {
				ChangeSubState(eExcutingStep_Move_Already_Pick_Dvc_PlaceArtBuffTable);
				MakeLog("Test2");
				break;
			}

			// auto retest 시 buffer 수량에 의한 station 동작이 불가능할 경우
			if (bAutoRetetsMode == TRUE)
			{
				int nCntBuffSameStation = 0;
				int nBufferDvcCnt = g_TaskTestPP[m_eTestIdx].GetBufferTableCnt();
				int nBufferPocketCnt = g_TaskTestPP[m_eTestIdx].GetBufferTablePocketCnt();
				int nBuffRemainDvcCnt = nBufferDvcCnt;
				int nSameNotStation = 0;

				int nLoadTblPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
				int nLoadTblPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

				for (int nY = 0; nY < nLoadTblPocketCntY; nY++) {
					for (int nX = 0; nX < nLoadTblPocketCntX; nX++) {
						for (int nTableIdx : m_vModuleSetLoadTableIdx)
						{
							stSiteDvc.clear();
							int nDvc = g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1 + m_eTestIdx, nX, nY, &stSiteDvc);
							char setting = g_TaskLoadTbl[nTableIdx].m_LoadTbl_pocket_setting[nY][nX];
							if (stSiteDvc.sExist == eDeviceData_Exist && stSiteDvc.nTestingCnt <= eARB_2stTestEnd && stSiteDvc.nTestingCnt >= eARB_1stTestEnd &&
								setting == 'B')
							{
								if (atoi(stSiteDvc.szTestSiteNum) == m_vModuleSetPressIdx[m_nJobAtsIdx] && strcmp(stSiteDvc.szTestSiteNum, "") != 0)
								{
									BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stSiteDvc);
									if (stSiteDvc.nTestingCnt == eARB_2stTestEnd || strcmp(stSiteDvc.szErrString, DEF_TEST_RESULT_TP101) == 0 || bIsDefaultBin == TRUE)
										nCntBuffSameStation++;
								}
								else {
									nSameNotStation++;
								}
							}
						}
					}
				}
				BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();

				nBuffRemainDvcCnt -= (nStationOnCnt >= nSameNotStation ? nSameNotStation : nStationOnCnt);

				if (nCntSite + nCntBuffSameStation > nBufferPocketCnt /2
					|| nCntSite + nBuffRemainDvcCnt >nBufferPocketCnt
					|| (nStationOnCnt == 0 && ((nCntSite + nBufferDvcCnt > nBufferPocketCnt / 2) 
					|| g_TaskTestPP[m_eTestIdx].GetLoadPickerLoadDeviceCnt() > 0))
					|| (g_TaskTestPP[m_eTestIdx].GetLoadPickerLoadDeviceCnt() > nStationOnCnt && (nCntSite + nBufferDvcCnt + (g_TaskTestPP[m_eTestIdx].GetLoadPickerLoadDeviceCnt() - nStationOnCnt) > nBufferPocketCnt / 2)))
				{				
					if (g_TaskPressUnit[m_nJobAtsIdx].PressInCleanDeviceCheck(bAutoGrrUse) == false)
					{
						g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx, ++m_nJobAtsIdx);
						MakeLog(_T("[==== %s job 2 =====]"), g_TaskTestPP[m_eTestIdx].GetLocName(LOC_IDX_STATION_1 + m_nJobAtsIdx));
						break;
					}
				
				}			

			
				// Retest AAB 인대 Buffer Table에 있는 Device가 B Station으로 가야 될 경우 Job 증가
				int b2stEndMatchCnt = 0;
				int b1stEndMatchCnt = 0;
				for (int nY = 0; nY < nLoadTblPocketCntY; nY++) {
					for (int nX = 0; nX < nLoadTblPocketCntX; nX++) {
						for (auto nTableIdx : m_vModuleSetLoadTableIdx)
						{
							stSiteDvc.clear();
							int nDvc = g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1 + nTableIdx, nX, nY, &stSiteDvc);

							if (stSiteDvc.sExist == eNot_Exist)
								continue;

							if ((atoi(stSiteDvc.szTestSiteNum) == m_vModuleSetPressIdx[m_nJobAtsIdx] && strcmp(stSiteDvc.szTestSiteNum, "") != 0/*|| strcmp(stSiteDvc.szErrString, DEF_HANDLER_ERR_DEVICE) == 0*/)
								&& g_TaskLoadTbl[nTableIdx].GetBufferPocketCheck(nX, nY) == TRUE
								&& stSiteDvc.nTestingCnt == eARB_2stTestEnd)
							{
								b2stEndMatchCnt++;
							}
							if ((atoi(stSiteDvc.szTestSiteNum) != m_vModuleSetPressIdx[m_nJobAtsIdx] && strcmp(stSiteDvc.szTestSiteNum, "") != 0/*|| strcmp(stSiteDvc.szErrString, DEF_HANDLER_ERR_DEVICE) == 0*/)
								&& g_TaskLoadTbl[nTableIdx].GetBufferPocketCheck(nX, nY) == TRUE
								&& stSiteDvc.nTestingCnt == eARB_1stTestEnd)
							{
								b1stEndMatchCnt++;
							}
						}
					}
				}

				int nTestPPLoadDevCnt = g_TaskTestPP[m_eTestIdx].GetLoadHandDeviceCnt();

				ST_DD_DEVICE stDvcHand;
				int nRetest_BTest_Count = 0;
				BOOL bRetestBTestHandPick = FALSE;
				// unload picker 정보
				for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
				{
					for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
					{
						stDvcHand.clear();
						int nDvc = g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY, &stSiteDvc);
						if (stSiteDvc.sExist == eDeviceData_Exist && (atoi(stSiteDvc.szTestSiteNum) == m_vModuleSetPressIdx[m_nJobAtsIdx]) && strcmp(stSiteDvc.szTestSiteNum,"")!=0 )
						{
							nRetest_BTest_Count++;
						}
					}
				}
				// Test Load Hand에 Device가 1개 이상 이고, Hand에 현재 가지고 있는 Device에 B Test 수량이 같으면, Job Index를 변경 한다.  Hang Up 방지
				if (nTestPPLoadDevCnt > 0 && nTestPPLoadDevCnt == nRetest_BTest_Count)
				{
					bRetestBTestHandPick = TRUE;
				}
		
			



				BOOL nSiteCleanDvcCnt = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].PressInCleanDeviceCheck(FALSE);

				if ((g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == true || g_TaskSystemCtrl.IsOneCycleModeOn() == true) && (/*m_nCleanMode == FALSE ||*/ nSiteCleanDvcCnt == FALSE)
					&& (nBufferDvcCnt > 0 && nBufferDvcCnt == b2stEndMatchCnt && nTestPPLoadDevCnt == 0))
				{
					g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx, ++m_nJobAtsIdx);
					MakeLog(_T("[==== %s job 3 =====]"), g_TaskTestPP[m_eTestIdx].GetLocName(LOC_IDX_STATION_1 + m_nJobAtsIdx));
					break;
				}
				if ((g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == true || g_TaskSystemCtrl.IsOneCycleModeOn() == true) && (/*m_nCleanMode == FALSE ||*/ nSiteCleanDvcCnt == FALSE)
					&& (nBufferDvcCnt > 0 && nBufferDvcCnt == b1stEndMatchCnt) || bRetestBTestHandPick)
				{
					g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx, ++m_nJobAtsIdx);
					MakeLog(_T("[==== %s job 4 =====]"), g_TaskTestPP[m_eTestIdx].GetLocName(LOC_IDX_STATION_1 + m_nJobAtsIdx));
					break;
				}

			}




			bool bWork = g_testpp_work_area[m_eTestIdx].reserve_work_area(m_vModuleSetPressIdx[m_nJobAtsIdx], this);

			int nLoadTableWorkPos = GetReserveAndLoadTblWorkPos(m_nLoadingTableInx);
			int nUnLoadTableWorkPos = GetReserveAndLoadTblWorkPos(m_nUnLoadingTableInx);


			//현재 Test PP가 작업 가능한 Job이고, Load Table이 투입 될 Device가 없으면,Test PP Job위치와 같다면 다시 채워 오게 예약 해제 한다.
			if (nLoadTableWorkPos == TRUE && m_nCleanMode != TRUE 
				&& g_TaskTrayLoadPP.GetTrayPpLoadDvcCnt() > 0
				&& g_TaskLoadTbl[m_nLoadingTableInx].GetLoadDeviceCnt() == 0
				&& g_TaskTestPP[m_eTestIdx].GetLoadHandDeviceCnt() == 0)
			{
 				MakeLog(_T("[==== %s job 5 =====]"), g_TaskTestPP[m_eTestIdx].GetLocName(LOC_IDX_STATION_1 + m_nJobAtsIdx));
 				g_TaskLoadTbl[m_nLoadingTableInx].SetWorkReserve(RID_INIT_IDLE_RESERVE/*RID_TRAY_PP_(RIDX_TRAY_PP_1)*/);
				g_DMCont.m_dmShuttle.SB(BDM3_LOAD_TABLE_1_WORK_RESERVE_IDLE + m_eTestIdx, TRUE);
				break;
			}

			// 아산사업장에서 문제 발생 [4/17/2025 donghyun.shin]
			// 현재 Test PP가 작업 가능한 Job이고, Load Table 투입 될 Device가 없고, Test PP Job 위치와 다르면 다시 채워 오게 예약 해제 한다.
			if (g_TaskLoadTbl[m_nLoadingTableInx].GetWorkReserve() == RID_TEST_PP_(m_eTestIdx)
				&& g_TaskLoadTbl[m_nLoadingTableInx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready
				&& m_nCleanMode != TRUE
				&& g_TaskTrayLoadPP.GetTrayPpLoadDvcCnt() > 0
				&& g_TaskLoadTbl[m_nLoadingTableInx].GetLoadDeviceCnt() == 0
				&& g_TaskTestPP[m_eTestIdx].GetLoadHandDeviceCnt() == 0)
			{
				MakeLog(_T("[==== %s job 6 =====]"), g_TaskTestPP[m_eTestIdx].GetLocName(LOC_IDX_STATION_1 + m_nJobAtsIdx));
				g_TaskLoadTbl[m_nLoadingTableInx].SetWorkReserve(RID_INIT_IDLE_RESERVE/*RID_TRAY_PP_(RIDX_TRAY_PP_1)*/);
				g_DMCont.m_dmShuttle.SB(BDM3_LOAD_TABLE_1_WORK_RESERVE_IDLE + m_eTestIdx, TRUE);
				break;
			}

			int nTestPPLoadDevCnt = g_TaskTestPP[m_eTestIdx].GetLoadHandDeviceCnt();

			if (bWork == true && (nLoadTableWorkPos == TRUE  || g_TaskTestPP[m_eTestIdx].GetLoadPickerDeviceCnt() > 0)||
				(bWork == true && (nUnLoadTableWorkPos == TRUE && (g_TaskSystemCtrl.IsOneCycleModeOn() == true || g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == true))))
			{
				g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx, m_nJobAtsIdx);
				ChangeSubState(eExcutingStep_Move_PickCleanTable);
				MakeLog(_T("[==== FindJob reserve_work_area=%s =====]"), g_TaskTestPP[m_eTestIdx].GetLocName(LOC_IDX_STATION_1 + m_nJobAtsIdx));
				break;
			}
			int nLoadTblDevCnt = g_TaskLoadTbl[m_nLoadingTableInx].GetLoadDeviceCnt();
			

			if (bWork == true && (g_TaskSystemCtrl.IsOneCycleModeOn() == true || g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == true) && nStationInDvc == 0 && nLoadTblDevCnt > 0 && nTestPPLoadDevCnt == 0)
			{
				g_testpp_work_area[m_eTestIdx].release_work_area(m_vModuleSetPressIdx[m_nJobAtsIdx], this);
				g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx, ++m_nJobAtsIdx);
				break;
			}
			if (bWork == true && (g_TaskSystemCtrl.IsOneCycleModeOn() == true || g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == true) && nStationInDvc == 0 && nLoadTblDevCnt == 0 && nTestPPLoadDevCnt == 0)
			{
				g_testpp_work_area[m_eTestIdx].release_work_area(m_vModuleSetPressIdx[m_nJobAtsIdx], this);
				g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx, ++m_nJobAtsIdx);
				break;
			}
		}
		else {
			if (m_nCleanMode != TRUE && (nStationInfo != eTestSite_CleanEnd || g_DMCont.m_dmEQP.GB(BDM0_TEST_STATUS_SITE1 + m_vModuleSetPressIdx[m_nJobAtsIdx]) == TRUE))
			{
				g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx, ++m_nJobAtsIdx);

			}
			else if (m_nCleanMode == TRUE && g_DMCont.m_dmEQP.GB(BDM0_TEST_STATUS_SITE1 + m_vModuleSetPressIdx[m_nJobAtsIdx]) == TRUE)
			{
				g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx, ++m_nJobAtsIdx);
			}
		}
	}break;	
	// pick clean buffer - Load Hand
	case eExcutingStep_Move_PickCleanTable:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			int nJob = CheckJob_CleanBuff(m_nJobAtsIdx, DEF_PICK, m_cJobPocket, m_vJobPicker_Pick);
			if (nJob != eExcutingStep_ErrorJob)
			{
				CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
				pSeqCmd->m_nCmdStage = LOC_IDX_CLEAN_TBL;
				pSeqCmd->m_cCmdPocket = m_cJobPocket;
				pSeqCmd->m_vCmdPicker = m_vJobPicker_Pick;
				pSeqCmd->m_nCmdPick = DEF_PICK;
				pSeqCmd->m_nHandType = TEST_LD_HAND;
				MakeLog(_T("[MainStep=%s][CLEAN_BUFFER PICK][x=%d,y=%d][%s][%s]"), GetLotStateToString(m_nAutoRunMainStep), m_cJobPocket.x + 1, m_cJobPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Pick), g_TaskTestPP[m_eTestIdx].GetHandTypeName(pSeqCmd->m_nHandType));

				if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) == eSeqCmdAck_OK) {
				}
			}
			ChangeSubState(eExcutingStep_WaitEnd_PickCleanTable);
		}
	}break;
	case eExcutingStep_WaitEnd_PickCleanTable:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			int nJob = CheckJob_CleanBuff(m_nJobAtsIdx, DEF_PICK, m_cJobPocket, m_vJobPicker_Pick);
			if (nJob != eExcutingStep_ErrorJob)
				ChangeSubState(eExcutingStep_Move_PickCleanTable);
			else
				ChangeSubState(eExcutingStep_Move_PickArtBuffTable);			
		}
	}break;
	// pick buffer - Load Hand
	case eExcutingStep_Move_PickArtBuffTable:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			int nLocIdx = -1;
			int nJob = CheckJob_ArtBuff(m_nJobAtsIdx, DEF_PICK, m_cJobPocket, m_vJobPicker_Pick, nLocIdx);
			if (nJob == eExcutingStep_FindJob && nLocIdx != -1)
			{
				CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;			
				if (nLocIdx == LOC_IDX_TEST_PP_LOAD_TBL_BUFF) {
					//Loading Tbl Reserve보다 우선 이동 Flag Set
					g_TaskLoadTbl[m_nLoadingTableInx].SetFlagLoadTblBuff(TRUE);
					pSeqCmd->m_nCmdStage = LOC_IDX_TEST_PP_TBL_3_LD;
				}
				else {
					pSeqCmd->m_nCmdStage = nLocIdx + m_nJobAtsIdx;
				}

				//Station과 충돌 영역이면, 해당 Job 수행 안함.
				std::vector<int> vLogStationNo;
				vLogStationNo.clear();

				double dTPosX = g_TaskTestPP[m_eTestIdx].GetTeachPos(pSeqCmd->m_nCmdStage, m_cJobPocket, m_vJobPicker_Pick, CTaskTestPP::eZPOS_PICK, CTaskTestPP::eAxisX1, TEST_LD_HAND);
				double dTPosY = g_TaskTestPP[m_eTestIdx].GetTeachPos(pSeqCmd->m_nCmdStage, m_cJobPocket, m_vJobPicker_Pick, CTaskTestPP::eZPOS_PICK, CTaskTestPP::eAxisY, TEST_LD_HAND);
				MakeLog(_T("[dTargetPos=%.3f]"), dTPosY);
				int nWork = g_TaskTestPP[m_eTestIdx].IsYRangeRelatedPress_Z_Safety(dTPosX, dTPosY, TEST_LD_HAND, vLogStationNo);
				if (nWork != ERR_NO_ERROR) {
					MakeLog(_T("[Skip load Hand Pick Table]"));
					for (int i = 0; i < (int)vLogStationNo.size(); i++) {
						MakeLog(_T("[Impact Station Idx = %d]"), vLogStationNo[i]);
					}
					//ChangeSubState(eExcutingStep_Move_PickLoadTable);
					
					// Load 자재 확인 case 추가 [5/7/2025 dohyeong.kim]
					ChangeSubState(eExcutingStep_Check_Load_Table_Release);
					break;
				}

				pSeqCmd->m_cCmdPocket = m_cJobPocket;
				pSeqCmd->m_vCmdPicker = m_vJobPicker_Pick;
				pSeqCmd->m_nCmdPick = DEF_PICK;
				pSeqCmd->m_nHandType = TEST_LD_HAND;
				MakeLog(_T("[ART_BUFFER PICK][x=%d,y=%d][%s][%s]"), m_cJobPocket.x + 1, m_cJobPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Pick), g_TaskTestPP[m_eTestIdx].GetHandTypeName(pSeqCmd->m_nHandType));

				if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) == eSeqCmdAck_OK) {
				}
			}
			if (nJob == eExcutingStep_WaitOtherJob)
				break;

			ChangeSubState(eExcutingStep_WaitEnd_PickArtBuffTable);
		}
	}break;
	case eExcutingStep_WaitEnd_PickArtBuffTable:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			g_TaskLoadTbl[m_nLoadingTableInx].SetFlagLoadTblBuff(FALSE);

			int nLocIdx = -1;
			int nJob = CheckJob_ArtBuff(m_nJobAtsIdx, DEF_PICK, m_cJobPocket, m_vJobPicker_Pick, nLocIdx);
			if (nJob == eExcutingStep_FindJob && nLocIdx != -1)
			{
				ChangeSubState(eExcutingStep_Move_PickArtBuffTable);
			}
			else if (nJob == eExcutingStep_WaitOtherJob)
				ChangeSubState(eExcutingStep_Move_PickArtBuffTable);
			else {

				// Load 자재 확인 case 추가 [5/7/2025 dohyeong.kim]
				//ChangeSubState(eExcutingStep_Move_PickLoadTable); 				
				ChangeSubState(eExcutingStep_Check_Load_Table_Release);
			}
		}
	}break;
	case eExcutingStep_Check_Load_Table_Release:
	{
		//이전 job에서 Load Table 이동 중이라 해제가 안됐을 경우, 확인 코드.
		//Load 투입 자재 확인 후, Table Release
		BOOL bIsOneCycle = g_TaskSystemCtrl.IsOneCycleModeOn();
		BOOL bLoadingBreak = g_TaskAllAutoStacker.GetLoadTraySupplyEnd();
		int nTblRes = g_TaskLoadTbl[m_nLoadingTableInx].GetWorkReserve();

		//Test Site Reserve 확인 && OneCycle 확인 && Load 공급 확인.
		if (nTblRes == RID_TEST_PP_(m_eTestIdx) && bIsOneCycle != TRUE && bLoadingBreak != TRUE && m_nCleanMode != TRUE)
		{
			//투입 가능 자재 개수 확인.(Load Hand + Load Talbe)
			int nTestPpLoadDeviceCnt = g_TaskTestPP[m_eTestIdx].GetLoadHandDeviceCnt();
			int nLoadTblDeviceCnt = g_TaskLoadTbl[m_nLoadingTableInx].GetLoadDeviceCnt();
			int nCntLoadPossible = nTestPpLoadDeviceCnt + nLoadTblDeviceCnt;

			//Socket 투입 가능 자재 개수 확인.
			int n1stFailSiteInDvcCnt = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetSite1stFailInDvcCnt();
			int nSiteOnOffCnt = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetSocketOnCnt();
			int nCntSocketLoadPossible = nSiteOnOffCnt - n1stFailSiteInDvcCnt;

			MakeLog(_T("[Before Load Table Pick] [Possible Cnt] [Test PP = %d, Tbl = %d, 1stFail = %d, SiteOn = %d]"), nTestPpLoadDeviceCnt, nLoadTblDeviceCnt, n1stFailSiteInDvcCnt, nSiteOnOffCnt);
		
			if (nCntLoadPossible < nCntSocketLoadPossible) {
				MakeLog(_T("[Before Load Table Pick] [SetWorkReserve Load Table job = [%d]]"), m_nJobAtsIdx + 1);
				g_TaskLoadTbl[m_nLoadingTableInx].SetWorkReserve(RID_INIT_IDLE_RESERVE/*RID_TRAY_PP_(RIDX_TRAY_PP_1)*/);
				g_DMCont.m_dmShuttle.SB(BDM3_LOAD_TABLE_1_WORK_RESERVE_IDLE + m_eTestIdx, TRUE);
				ChangeSubState(eExcutingStep_WaitEnd_Check_Load_Table_Release);
				break;
			}

		}

		ChangeSubState(eExcutingStep_Move_PickLoadTable);
	}break;
	case eExcutingStep_WaitEnd_Check_Load_Table_Release:
	{
		//해당 case일 경우, Table Load 자재 채워 올 때까지 대기.
		//Table 현재 Position && RID_TEST_PP 확인
		int nLoadTableWorkPos = GetReserveAndLoadTblWorkPos(m_nLoadingTableInx);

		if (nLoadTableWorkPos == TRUE && g_TaskLoadTbl[m_nLoadingTableInx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			MakeLog(_T("[Before Load Table Pick] [END]"));
			ChangeSubState(eExcutingStep_Move_PickLoadTable);
		}
	}break;
	// pick load table - Load Hand
	case eExcutingStep_Move_PickLoadTable:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			int nJob = CheckJob_LoadTbl(m_nJobAtsIdx, DEF_PICK, m_cJobPocket, m_vJobPicker_Pick);
			
			int nLoadDeviceCnt = g_TaskTestPP[m_eTestIdx].GetLoadHandDeviceCnt();
			int nChkInterlock = ChkInterlockAts(m_nJobAtsIdx);

			int nChk1stFailSiteInDvcCnt = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetSite1stFailInDvcCnt();
			int nChkSiteOnOffCnt = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetSocketOnCnt();

			int nSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
			int nSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

			// Station Socket과 매칭 하여 실제 LoadHand가 Device Pick한 가용 가능한 Count를 추려 낸다.
			int nLoadDeviceCount = 0;
 			for (int nY = 0; nY < nSiteY; nY++)
			{
				for (int nX = 0; nX < nSiteX; nX++)
				{
					int nDeviceExistData = g_TaskTestPP[m_eTestIdx].GetHandPkrDvcExist(CPoint(nX, nY), eTestPPLoadType);
					int nSiteOn = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetSocketOnOff(nX, nY);
					if (nSiteOn == DEF_ON && nDeviceExistData == eDvcExist)
					{
						nLoadDeviceCount++;
					}
				}
			}


			// Socket On Count - A Fail Count를 계산 하여 실제로 Press Site에 투입 가능 한 Device Count를 뽑아낸다.
			int nSocketLoadPossibleCnt = nChkSiteOnOffCnt - nChk1stFailSiteInDvcCnt;

			if (nJob == eExcutingStep_WaitOtherJob ) {
				if (nLoadDeviceCount == nSocketLoadPossibleCnt && nChkInterlock == ERR_NO_ERROR) {
					// Hand에 Load Device가 있고 Press가 Safety일 경우 Job 수행 안함
					ChangeSubState(eExcutingStep_Move_PlaceLoadTable_2dErrDvc);
					break;
				}				
				break;
			}

			if (nJob != eExcutingStep_ErrorJob)
			{
				int nLoadTableIdx = GetLoadTableInx(DEF_PICK);
				int ntest = g_TaskLoadTbl[nLoadTableIdx].ChkMotorPos(LOC_IDX_LD_TBL_TEST_SIDE_1 + m_nJobAtsIdx, CTaskLoadTable::eAxisAll);
				if (ntest != ERR_NO_ERROR) {
					int a = 0;
				}
				CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
				pSeqCmd->m_nCmdStage = LOC_IDX_TEST_PP_TBL_1_LD + m_nJobAtsIdx;
				pSeqCmd->m_cCmdPocket = m_cJobPocket;
				pSeqCmd->m_nCmdPick = DEF_PICK;
				pSeqCmd->m_vCmdPicker = m_vJobPicker_Pick;
				pSeqCmd->m_nHandType = TEST_LD_HAND;
				MakeLog(_T("[LD_TBL PICK][x=%d,y=%d][%s][%s]"), m_cJobPocket.x + 1, m_cJobPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Pick), g_TaskTestPP[m_eTestIdx].GetHandTypeName(pSeqCmd->m_nHandType));

				if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) == eSeqCmdAck_OK) {
				}
			}

			ChangeSubState(eExcutingStep_WaitEnd_PickLoadTable);
		}
	}break;
	case eExcutingStep_WaitEnd_PickLoadTable:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			int nJob = CheckJob_LoadTbl(m_nJobAtsIdx, DEF_PICK, m_cJobPocket, m_vJobPicker_Pick);
			if (nJob == eExcutingStep_WaitOtherJob)
				break;
			if (nJob == eExcutingStep_FindJob)
				ChangeSubState(eExcutingStep_Move_PickLoadTable);
			else {

				if (nJob != eExcutingStep_WaitOtherJob)
					ChangeSubState(eExcutingStep_Move_PlaceLoadTable_2dErrDvc);
			}
		}
	}break;
	// place unload table - load hand
	case eExcutingStep_Move_PlaceLoadTable_2dErrDvc:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			int nJob = CheckJob_2dErrLoadTbl(m_cJobPocket, m_vJobPicker_Place,DEF_PLACE);
			if (nJob == eExcutingStep_WaitOtherJob)
				break;

			if (nJob != eExcutingStep_ErrorJob)
			{
				CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
				pSeqCmd->m_nCmdStage = LOC_IDX_TEST_PP_TBL_1_UD + m_nJobAtsIdx;
				pSeqCmd->m_cCmdPocket = m_cJobPocket;
				pSeqCmd->m_nCmdPick = DEF_PLACE;
				pSeqCmd->m_vCmdPicker = m_vJobPicker_Place;
				pSeqCmd->m_nHandType = TEST_LD_HAND;
				MakeLog(_T("[LD_TBL 2D ERR PLACE][x=%d,y=%d][%s]"), m_cJobPocket.x + 1, m_cJobPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Place));

				if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) == eSeqCmdAck_OK) {
				}
			}
			ChangeSubState(eExcutingStep_WaitEnd_PlaceLoadTable_2dErrDvc);
		}
	}break;
	case eExcutingStep_WaitEnd_PlaceLoadTable_2dErrDvc:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			int nJob = CheckJob_2dErrLoadTbl(m_cJobPocket, m_vJobPicker_Place, DEF_PLACE);
			if (nJob == eExcutingStep_WaitOtherJob)
				break;

			if (nJob == eExcutingStep_FindJob)
				ChangeSubState(eExcutingStep_Move_PlaceLoadTable_2dErrDvc);
			else {
				BOOL b2dBarcodeUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use);
				if (b2dBarcodeUse == eOPTION_USE && g_TaskLoadTbl[m_nUnLoadingTableInx].GetUnload2dBarcodeErrDvcCnt() > 0)
				{
					MakeLog(_T("[2D ERROR SetWorkReserve Load Table job = [%d]]"), m_nJobAtsIdx + 1);
					g_TaskLoadTbl[m_nUnLoadingTableInx].SetWorkReserve(RID_INIT_IDLE_RESERVE/*RID_TRAY_PP_(RIDX_TRAY_PP_1)*/);
				}
				

				ChangeSubState(eExcutingStep_Move_load_Table_Release);
			}				
		}
	}break;
	// load table release
  	case eExcutingStep_Move_load_Table_Release:
  	{
  		int nTblLdDvcCnt = g_TaskLoadTbl[m_nLoadingTableInx].GetLoadDeviceCnt();
  		int nMotorLoadPos = g_TaskLoadTbl[m_nLoadingTableInx].ChkMotorPos(LOC_IDX_TBL_LD_SIDE, CTaskLoadTable::eAxisY);
  		int nSiteArtDvcCnt = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetSiteInDvc(TRUE);
		int nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
		int nCleanMode = ChkCleaningMode(m_nJobAtsIdx);
  
		int nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

		MakeLog(_T("[Load Table %d Release[STR]]"), m_nLoadingTableInx + 1);
		if (nMotorLoadPos != ERR_NO_ERROR  && g_TaskLoadTbl[m_nLoadingTableInx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready /*&& m_nBeforeStation > 0*/)
		{
			if ((nTestSite_Div_Y == STATION_MAX_Y_SITE && nTblLdDvcCnt != 8 /*&& nSiteArtDvcCnt == 0*/ && nCleanMode != TRUE)||
				nTestSite_Div_Y == 1 && nTblLdDvcCnt < 4 /*&& nSiteArtDvcCnt == 0*/ && nCleanMode != TRUE)
			{
				int nLoadTblPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
				int nLoadTblPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);
		

				int nBufferDvcCnt = g_TaskLoadTbl[m_nUnLoadingTableInx].GetBufferDeviceCnt();
				/* Unload Table Buffer Device Full 상태거나, Unload Test PP에 B Device가 한개라도 있다면, 
				     Table 예약 해제 하지 않는다.*/
				if (!(nBufferDvcCnt == (nLoadTblPocketCntX*nLoadTblPocketCntY / 2) && g_TaskTestPP[m_eTestIdx].GetUnLoadHandRetestBDeviceCnt() > 0))
				{
					MakeLog(_T("[ SetWorkReserve Load Table job = [%d]]"), m_nJobAtsIdx + 1);
					g_TaskLoadTbl[m_nLoadingTableInx].SetWorkReserve(RID_INIT_IDLE_RESERVE/*RID_TRAY_PP_(RIDX_TRAY_PP_1)*/);
					g_DMCont.m_dmShuttle.SB(BDM3_LOAD_TABLE_1_WORK_RESERVE_IDLE + m_eTestIdx, TRUE);
				}				
				
				//Loading Table 미리 대기를 위한 Contact PrePare 상태 변경.
				//기능에 따라 상태 변경 시점이 다름.
				BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
				if (bAutoRetestUseMode != TRUE) {
					int nIdx = m_nJobAtsIdx + (4 * m_eTestIdx);
					if (!m_bAlreadyPick) {
						m_bAlreadyPick = FALSE;

						if (m_bNotChkPreContactPos != TRUE) {
							//g_DMCont.m_dmContac_Up.SN(NDM2_PRESS_1_PRE_CONTACT_STATUS + nIdx, TRUE);
						}
						else {
							m_bNotChkPreContactPos = FALSE;
						}
					}
				}				
			}		
		}
		MakeLog(_T("[Load Table %d Release[END]]"), m_nLoadingTableInx + 1);

		ChangeSubState(eExcutingStep_Move_PickStation);
   	}break;
	// pick Station - UnLoad Hand
	case eExcutingStep_Move_PickStation:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			if (m_nJobAtsIdx == 0) {
				int a = 0;
			}

			CPoint vPocket;
			int nJob = CheckJob_Station(m_nJobAtsIdx, DEF_PICK, vPocket, m_vJobPicker_Pick);
			if (nJob != eExcutingStep_ErrorJob)
			{
				CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
				pSeqCmd->m_nCmdStage = LOC_IDX_STATION_1 + m_nJobAtsIdx;
				pSeqCmd->m_cCmdPocket = vPocket;
				pSeqCmd->m_vCmdPicker = m_vJobPicker_Pick;
				pSeqCmd->m_nCmdPick = DEF_PICK;
				BOOL bChkCleanDev = GetDvcStationCleanDevAssign(DEF_PICK, m_nJobAtsIdx);
				if (bChkCleanDev == TRUE)
					pSeqCmd->m_nHandType = TEST_LD_HAND;
				else
					pSeqCmd->m_nHandType = TEST_UD_HAND;

				MakeLog(_T("[%s PICK][x=%d,y=%d][%s][%s]"), g_TaskTestPP[m_eTestIdx].GetLocName(LOC_IDX_STATION_1 + m_nJobAtsIdx), vPocket.x + 1, vPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Pick), g_TaskTestPP[m_eTestIdx].GetHandTypeName(pSeqCmd->m_nHandType));

				if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) == eSeqCmdAck_OK) {
					ChangeSubState(eExcutingStep_WaitEnd_PickStation);
				}

			}
			ChangeSubState(eExcutingStep_WaitEnd_PickStation);
		}
	}break;
	case eExcutingStep_WaitEnd_PickStation:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			CPoint vPocket;
			int nJob = CheckJob_Station(m_nJobAtsIdx, DEF_PICK, vPocket, m_vJobPicker_Pick);
			if (nJob != eExcutingStep_ErrorJob)
				ChangeSubState(eExcutingStep_Move_PickStation);
			else {
				ChangeSubState(eExcutingStep_Move_PlaceStation_AARetest);
			}
		}
	}break;
	// Station Place AA Retest - UnLoad Hand
	case eExcutingStep_Move_PlaceStation_AARetest:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			BOOL bArtBinCheck = GetHandAutoRetestBinCheck();
			CPoint vPocket;
			int nJob = CheckJob_Station(m_nJobAtsIdx, DEF_PLACE, vPocket, m_vJobPicker_Place,TRUE);
			if (nJob != eExcutingStep_ErrorJob && bArtBinCheck == TRUE && m_nCleanMode != TRUE)
			{
				CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
				pSeqCmd->m_nCmdStage = LOC_IDX_STATION_1 + m_nJobAtsIdx;
				pSeqCmd->m_cCmdPocket = vPocket;
				pSeqCmd->m_vCmdPicker = m_vJobPicker_Place;
				pSeqCmd->m_nCmdPick = DEF_PLACE;
				BOOL bChkCleanDev = GetDvcStationCleanDevAssign(DEF_PLACE, m_nJobAtsIdx);
				if (bChkCleanDev == TRUE)
					pSeqCmd->m_nHandType = TEST_LD_HAND;
				else
					pSeqCmd->m_nHandType = TEST_UD_HAND;

				MakeLog(_T("[%s PICK][x=%d,y=%d][%s][%s]"), g_TaskTestPP[m_eTestIdx].GetLocName(LOC_IDX_STATION_1 + m_nJobAtsIdx), vPocket.x + 1, vPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Pick), g_TaskTestPP[m_eTestIdx].GetHandTypeName(pSeqCmd->m_nHandType));

				if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) == eSeqCmdAck_OK) {
					ChangeSubState(eExcutingStep_WaitEnd_PlaceStation_AARetest);
				}

			}
			ChangeSubState(eExcutingStep_WaitEnd_PlaceStation_AARetest);
		}
	}break;
	case eExcutingStep_WaitEnd_PlaceStation_AARetest:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			CPoint vPocket;
			BOOL bArtBinCheck = GetHandAutoRetestBinCheck();
			int nJob = CheckJob_Station(m_nJobAtsIdx, DEF_PLACE, vPocket, m_vJobPicker_Place, TRUE);
			if (nJob != eExcutingStep_ErrorJob &&  bArtBinCheck == TRUE && m_nCleanMode != TRUE)
				ChangeSubState(eExcutingStep_Move_PlaceStation_AARetest);
			else {
				ChangeSubState(eExcutingStep_Check_PlaceArtBuffAndLoadTable/*eExcutingStep_Move_Unload_Table_Wait*/);
			}
		}
	}break;

	//Unload Test PP가 Press와 충돌 위치 이면 Table Place먼저 진행.
	case eExcutingStep_Check_PlaceArtBuffAndLoadTable:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{

			int nLocIdx = -1;
			int nJob = CheckJob_ArtBuff(m_nJobAtsIdx, DEF_PLACE, m_cJobPocket, m_vJobPicker_Place, nLocIdx);
			if (nJob == eExcutingStep_FindJob && nLocIdx != -1)
			{
				int nCmdStage = -1;
				if (nLocIdx == LOC_IDX_TEST_PP_LOAD_TBL_BUFF) {
					nCmdStage = LOC_IDX_TEST_PP_TBL_3_LD;
				}
				else {
					nCmdStage = nLocIdx + m_nJobAtsIdx;
				}

				double dTPos = g_TaskTestPP[m_eTestIdx].GetTeachPos(nCmdStage, m_cJobPocket, m_vJobPicker_Place, CTaskTestPP::eZPOS_PLACE, CTaskTestPP::eAxisX2, TEST_UD_HAND);
				BOOL bStationArea = g_TaskTestPP[m_eTestIdx].IsHandStationArea(dTPos);

				if (bStationArea == TRUE) {
					MakeLog(_T("[SKIP Station Already Place][Place Art Buff]"));
					ChangeSubState(eExcutingStep_Move_Unload_Table_Wait);
					break;
				}
			}


			nJob = CheckJob_LoadTbl(m_nJobAtsIdx, DEF_PLACE, m_cJobPocket, m_vJobPicker_Place);
			if (nJob != eExcutingStep_ErrorJob && nJob != eExcutingStep_WaitOtherJob)
			{
				int nCmdStage = LOC_IDX_TEST_PP_TBL_1_UD + m_nJobAtsIdx;
				double dTPos = g_TaskTestPP[m_eTestIdx].GetTeachPos(nCmdStage, m_cJobPocket, m_vJobPicker_Place, CTaskTestPP::eZPOS_PLACE, CTaskTestPP::eAxisX2, TEST_UD_HAND);
				BOOL bStationArea = g_TaskTestPP[m_eTestIdx].IsHandStationArea(dTPos);

				if (bStationArea == TRUE) {
					MakeLog(_T("[SKIP Station Already Place][Place Load Table]"));
					ChangeSubState(eExcutingStep_Move_Unload_Table_Wait);
					break;
				}
			}

			//충돌 영역 아니면 Load자재 투입 먼저 진행.
			ChangeSubState(eExcutingStep_Move_Already_PlaceStation);
		}
	}break;
	// Place station site - Load Hand
	case eExcutingStep_Move_Already_PlaceStation:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			CPoint vPocket;
			int nJob = CheckJob_Station(m_nJobAtsIdx, DEF_PLACE, vPocket, m_vJobPicker_Place);

			if (nJob == eExcutingStep_FindJob)
			{
				CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
				pSeqCmd->m_nCmdStage = LOC_IDX_STATION_1 + m_nJobAtsIdx;
				pSeqCmd->m_cCmdPocket = vPocket;
				pSeqCmd->m_vCmdPicker = m_vJobPicker_Place;
				pSeqCmd->m_nCmdPick = DEF_PLACE;
				pSeqCmd->m_nHandType = TEST_LD_HAND;
				MakeLog(_T("[%s PLACE][x=%d,y=%d][%s][%s]"), g_TaskTestPP[m_eTestIdx].GetLocName(LOC_IDX_STATION_1 + m_nJobAtsIdx), vPocket.x + 1, vPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Place), g_TaskTestPP[m_eTestIdx].GetHandTypeName(pSeqCmd->m_nHandType));

				if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) != eSeqCmdAck_OK) {
				}

			}
			else if (nJob == eExcutingStep_WaitOtherJob)
			{
				ChangeSubState(eExcutingStep_Move_PickStation);
				break;
			}

			ChangeSubState(eExcutingStep_WaitEnd_Already_PlaceStation);
		}
	}break;
	case eExcutingStep_WaitEnd_Already_PlaceStation:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			CPoint vPocket;
			int nJob = CheckJob_Station(m_nJobAtsIdx, DEF_PLACE, vPocket, m_vJobPicker_Place);
			if (nJob == eExcutingStep_FindJob) {
				ChangeSubState(eExcutingStep_Move_Already_PlaceStation);
				break;
			}
			else if (nJob == eExcutingStep_WaitOtherJob) {
				ChangeSubState(eExcutingStep_Move_PickStation);
				break;
			}
			else
			{
				//ChangeSubState(eExcutingStep_Move_PlaceCleanTable);	
				g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_ALREADY_PLACE_WORKING_IDX + m_eTestIdx, m_nJobAtsIdx);
				ChangeSubState(eExcutingStep_Move_Unload_Table_Wait);
			}
		}
	}break;
	case eExcutingStep_Move_Unload_Table_Wait:
	{
		int nUnLoadTableWorkPos = GetReserveAndLoadTblWorkPos(m_nUnLoadingTableInx);	

		if (nUnLoadTableWorkPos == TRUE && g_TaskLoadTbl[m_nUnLoadingTableInx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			ChangeSubState(eExcutingStep_Move_PlaceAllSiteOffDevice/*eExcutingStep_Move_PlaceArtBuffTable*/);
		}
	}break;

	// Station Socket All Off로 인해, 해당 Picker Device Vacuum일 경우, Place 진행.
	case eExcutingStep_Move_PlaceAllSiteOffDevice:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			std::vector<CPoint> vCmdPicker;
			vCmdPicker.clear();

			//Station Site Off <-> Picker Vacuum 비교.
			int  nMaxSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
			int  nMaxSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
			for (int nY = 0; nY < nMaxSiteY; nY++)
			{
				for (int nX = 0; nX < nMaxSiteX; nX++)
				{
					int nDeviceExistData = g_TaskTestPP[m_eTestIdx].GetHandPkrDvcExist(CPoint(nX, nY), TEST_LD_HAND);
					int nExist = g_TaskTestPP[m_eTestIdx].ChkVacuum(DEF_ON, TEST_LD_HAND, CPoint(nX, nY));
					if (nExist == ERR_NO_ERROR && nDeviceExistData == eDvcExist) { //Device Vacuum

						int nCntSocketOff = 0;

						for (int nStation = 0; nStation < (int)m_vModuleSetPressIdx.size(); nStation++)
						{
							int nSiteOnOff = g_TaskPressUnit[m_vModuleSetPressIdx[nStation]].GetSocketOnOff(nX, nY);
							if (nSiteOnOff == DEF_SITE_OFF) {
								nCntSocketOff++;
							}
						}

						if (nCntSocketOff == m_vModuleSetPressIdx.size()) {
							vCmdPicker.push_back(CPoint(nX, nY));
						}
					}
				}
			}


			if (vCmdPicker.size() > 0) {
				
				g_DMCont.m_dmTestPP.SN(BDM4_TESTHAND1_ALL_SOCKET_OFF_PLACE_ART_BUFFER + m_eTestIdx, true);

				int nLocIdx = -1;
				int nJob = CheckJob_ArtBuff(m_nJobAtsIdx, DEF_PLACE, m_cJobPocket, m_vJobPicker_Place, nLocIdx);
				if (nJob == eExcutingStep_FindJob && nLocIdx != -1)
				{
					CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
					if (nLocIdx == LOC_IDX_TEST_PP_LOAD_TBL_BUFF) {
						//Loading Tbl Reserve보다 우선 이동 Flag Set
						g_TaskLoadTbl[m_nLoadingTableInx].SetFlagLoadTblBuff(TRUE);
						pSeqCmd->m_nCmdStage = LOC_IDX_TEST_PP_TBL_3_LD;
					}
					else {
						pSeqCmd->m_nCmdStage = nLocIdx + m_nJobAtsIdx;
					}

					//pSeqCmd->m_nCmdStage = nLocIdx + m_nJobAtsIdx;
					pSeqCmd->m_cCmdPocket = m_cJobPocket;
					pSeqCmd->m_vCmdPicker = vCmdPicker/*m_vJobPicker_Place*/;
					pSeqCmd->m_nCmdPick = DEF_PLACE;
					pSeqCmd->m_nHandType = TEST_LD_HAND;
					MakeLog(_T("[ART_BUFFER PLACE][x=%d,y=%d][%s][%s]"), m_cJobPocket.x + 1, m_cJobPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Place), g_TaskTestPP[m_eTestIdx].GetHandTypeName(pSeqCmd->m_nHandType));

					if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) == eSeqCmdAck_OK) {
					}
				}				
			}

			ChangeSubState(eExcutingStep_WaitEnd_PlaceAllSiteOffDevice);

		}
	}break;
	case eExcutingStep_WaitEnd_PlaceAllSiteOffDevice:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			g_DMCont.m_dmTestPP.SN(BDM4_TESTHAND1_ALL_SOCKET_OFF_PLACE_ART_BUFFER + m_eTestIdx, false);

			g_TaskLoadTbl[m_nLoadingTableInx].SetFlagLoadTblBuff(FALSE);

			ChangeSubState(eExcutingStep_Move_PlaceArtBuffTable);
		}
	}break;

	// place auto retest buffer - Unload Hand
	case eExcutingStep_Move_PlaceArtBuffTable:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			int nLocIdx = -1;
			int nJob = CheckJob_ArtBuff(m_nJobAtsIdx, DEF_PLACE, m_cJobPocket, m_vJobPicker_Place, nLocIdx);
			if (nJob == eExcutingStep_FindJob && nLocIdx != -1)
			{
				CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
				if (nLocIdx == LOC_IDX_TEST_PP_LOAD_TBL_BUFF) {
					//Loading Tbl Reserve보다 우선 이동 Flag Set
					g_TaskLoadTbl[m_nLoadingTableInx].SetFlagLoadTblBuff(TRUE);
					pSeqCmd->m_nCmdStage = LOC_IDX_TEST_PP_TBL_3_LD;
				}
				else {
					pSeqCmd->m_nCmdStage = nLocIdx + m_nJobAtsIdx;
				}

				//pSeqCmd->m_nCmdStage = nLocIdx + m_nJobAtsIdx;
				pSeqCmd->m_cCmdPocket = m_cJobPocket;
				pSeqCmd->m_vCmdPicker = m_vJobPicker_Place;
				pSeqCmd->m_nCmdPick = DEF_PLACE;
				pSeqCmd->m_nHandType = TEST_UD_HAND;
				MakeLog(_T("[ART_BUFFER PLACE][x=%d,y=%d][%s][%s]"), m_cJobPocket.x + 1, m_cJobPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Place), g_TaskTestPP[m_eTestIdx].GetHandTypeName(pSeqCmd->m_nHandType));

				if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) == eSeqCmdAck_OK) {
				}
			}

			ChangeSubState(eExcutingStep_WaitEnd_PlaceArtBuffTable);
		}
	}break;
	case eExcutingStep_WaitEnd_PlaceArtBuffTable:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			g_TaskLoadTbl[m_nLoadingTableInx].SetFlagLoadTblBuff(FALSE);

			int nLocIdx = -1;
			int nJob = CheckJob_ArtBuff(m_nJobAtsIdx, DEF_PLACE, m_cJobPocket, m_vJobPicker_Place, nLocIdx);
			if (nJob == eExcutingStep_FindJob && nLocIdx != -1)
				ChangeSubState(eExcutingStep_Move_PlaceArtBuffTable);
			else {
				ChangeSubState(eExcutingStep_Move_PlaceLoadTable);
			}
		}		
	}break;

	// place load table - UnLoad Hand
	case eExcutingStep_Move_PlaceLoadTable:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			int nJob = CheckJob_LoadTbl(m_nJobAtsIdx, DEF_PLACE, m_cJobPocket, m_vJobPicker_Place);
			if (nJob != eExcutingStep_ErrorJob && nJob != eExcutingStep_WaitOtherJob)
			{
				//Station과 충돌 영역이면, 해당 Job 수행 안함.
				std::vector<int> vLogStationNo;
				vLogStationNo.clear();

				double dTPosX = g_TaskTestPP[m_eTestIdx].GetTeachPos(LOC_IDX_TEST_PP_TBL_1_UD + m_nJobAtsIdx, m_cJobPocket, m_vJobPicker_Place, CTaskTestPP::eZPOS_PLACE, CTaskTestPP::eAxisX2, TEST_UD_HAND);
				double dTPosY = g_TaskTestPP[m_eTestIdx].GetTeachPos(LOC_IDX_TEST_PP_TBL_1_UD + m_nJobAtsIdx, m_cJobPocket, m_vJobPicker_Place, CTaskTestPP::eZPOS_PLACE, CTaskTestPP::eAxisY, TEST_UD_HAND);
				int nWork = g_TaskTestPP[m_eTestIdx].IsYRangeRelatedPress_Z_Safety(dTPosX, dTPosY, TEST_UD_HAND, vLogStationNo);
				if (nWork != ERR_NO_ERROR) {
					MakeLog(_T("[Skip Unload Hand Place Table]"));
					for (int i = 0; i < (int)vLogStationNo.size(); i++) {
						MakeLog(_T("[Impact Station Idx = %d]"), vLogStationNo[i]);
					}
					ChangeSubState(eExcutingStep_Move_Unload_Table_Release);
					break;
				}

				CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
				pSeqCmd->m_nCmdStage = LOC_IDX_TEST_PP_TBL_1_UD + m_nJobAtsIdx;
				pSeqCmd->m_cCmdPocket = m_cJobPocket;
				pSeqCmd->m_vCmdPicker = m_vJobPicker_Place;
				pSeqCmd->m_nCmdPick = DEF_PLACE;
				pSeqCmd->m_nHandType = TEST_UD_HAND;
				MakeLog(_T("[UD_TBL PLACE][x=%d,y=%d][%s][%s]"), m_cJobPocket.x + 1, m_cJobPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Place), g_TaskTestPP[m_eTestIdx].GetHandTypeName(pSeqCmd->m_nHandType));

				if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) == eSeqCmdAck_OK) {
				}
			}
			ChangeSubState(eExcutingStep_WaitEnd_PlaceLoadTable);
		}
	}break;
	case eExcutingStep_WaitEnd_PlaceLoadTable:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			int nJob = CheckJob_LoadTbl(m_nJobAtsIdx, DEF_PLACE, m_cJobPocket, m_vJobPicker_Place);
			if (nJob != eExcutingStep_ErrorJob)
				ChangeSubState(eExcutingStep_Move_PlaceLoadTable);
			else {
				if (nJob != eExcutingStep_WaitOtherJob)
					ChangeSubState(eExcutingStep_Move_Unload_Table_Release);
			}
		}
	}break;
	// unload table release
	case eExcutingStep_Move_Unload_Table_Release:
	{
		int nChkSiteInDvc = 0;
		if (m_nUnLoadingTableInx == eLoadTbl_1 || m_nUnLoadingTableInx == eLoadTbl_4)
		{
			for (auto mdIdx : m_vModuleSetPressIdx)
				nChkSiteInDvc += g_TaskPressUnit[mdIdx].ChkSiteInDvc();
		}

		int nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
		int nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
		int nTblUldDvcCnt = g_TaskLoadTbl[m_nUnLoadingTableInx].GetUnloadDeviceCnt();
		int nMotorUnloadPos = g_TaskLoadTbl[m_nUnLoadingTableInx].ChkMotorPos(LOC_IDX_TBL_LD_SIDE, CTaskLoadTable::eAxisY);
		int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);

		if (nMotorUnloadPos != ERR_NO_ERROR && nTblUldDvcCnt > 0 && g_TaskLoadTbl[m_nUnLoadingTableInx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready
			|| g_TaskAllAutoStacker.GetLoadEnableStackerCount() == 0)
		{					
			if ((nTestSite_Div_Y == STATION_MAX_Y_SITE && nTblUldDvcCnt != 0)
				|| (nTestSite_Div_Y == 1 && (nTblUldDvcCnt > nTestSite_Div_X * nTestSite_Div_Y))
				|| (nTestSite_Div_Y == 1 && nBoardType == eBoardDir_Backward &&  nTblUldDvcCnt != 0))
			{
				MakeLog(_T("[ SetWorkReserve UnLoad Table job = [%d]]"), m_nJobAtsIdx + 1);
				g_TaskLoadTbl[m_nUnLoadingTableInx].SetWorkReserve(RID_INIT_IDLE_RESERVE/*RID_TRAY_PP_(RIDX_TRAY_PP_1)*/);
			}
				
		}
		ChangeSubState(eExcutingStep_Move_PlaceStation);
		//ChangeSubState(eExcutingStep_Move_PlaceCleanTable);
	}break;

 	// Place station site - Load Hand
 	case eExcutingStep_Move_PlaceStation:
 	{
 		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
 		{
 			CPoint vPocket;
 			int nJob = CheckJob_Station(m_nJobAtsIdx, DEF_PLACE, vPocket, m_vJobPicker_Place);
 
 			if (nJob != eExcutingStep_ErrorJob)
 			{
 				CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
 				pSeqCmd->m_nCmdStage = LOC_IDX_STATION_1 + m_nJobAtsIdx;
 				pSeqCmd->m_cCmdPocket = vPocket;
 				pSeqCmd->m_vCmdPicker = m_vJobPicker_Place;
 				pSeqCmd->m_nCmdPick = DEF_PLACE;
 				pSeqCmd->m_nHandType = TEST_LD_HAND;
 				MakeLog(_T("[%s PLACE][x=%d,y=%d][%s][%s]"), g_TaskTestPP[m_eTestIdx].GetLocName(LOC_IDX_STATION_1 + m_nJobAtsIdx), vPocket.x + 1, vPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Place), g_TaskTestPP[m_eTestIdx].GetHandTypeName(pSeqCmd->m_nHandType));
 
 				if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) != eSeqCmdAck_OK) {
 				}
 
 			}
 			else if(nJob == eExcutingStep_WaitOtherJob)
 			{
 				ChangeSubState(eExcutingStep_Move_PickStation);			
 				break;
 			}
 
 			ChangeSubState(eExcutingStep_WaitEnd_PlaceStation);
 		}
 	}break;
 	case eExcutingStep_WaitEnd_PlaceStation:
 	{
 		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
 		{
 			CPoint vPocket;
 			int nJob = CheckJob_Station(m_nJobAtsIdx, DEF_PLACE, vPocket, m_vJobPicker_Place);
 			if (nJob != eExcutingStep_ErrorJob)
 				ChangeSubState(eExcutingStep_Move_PlaceStation);
 			else if (nJob == eExcutingStep_WaitOtherJob)
 				ChangeSubState(eExcutingStep_Move_PickStation);
 			else
 			{
 				ChangeSubState(eExcutingStep_Move_PlaceCleanTable);
 			}
 		}
 	}break;
	//place clean table - load hand
	case eExcutingStep_Move_PlaceCleanTable:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{			
			int nJob = CheckJob_CleanBuff(m_nJobAtsIdx, DEF_PLACE, m_cJobPocket, m_vJobPicker_Place);
			if (nJob != eExcutingStep_ErrorJob)
			{
				g_testpp_work_area[m_eTestIdx].release_work_area(m_vModuleSetPressIdx[m_nJobAtsIdx], this);

				CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
				pSeqCmd->m_nCmdStage = LOC_IDX_CLEAN_TBL;
				pSeqCmd->m_cCmdPocket = m_cJobPocket;
				pSeqCmd->m_vCmdPicker = m_vJobPicker_Place;
				pSeqCmd->m_nCmdPick = DEF_PLACE;
				pSeqCmd->m_nHandType = TEST_LD_HAND;
				MakeLog(_T("[CLEAN_BUFFER PLACE][x=%d,y=%d][%s][%s]"), m_cJobPocket.x + 1, m_cJobPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Place), g_TaskTestPP[m_eTestIdx].GetHandTypeName(pSeqCmd->m_nHandType));

				if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) == eSeqCmdAck_OK) {
				}			
			}
			ChangeSubState(eExcutingStep_WaitEnd_PlaceCleanTable);
		}
	}break;
	case eExcutingStep_WaitEnd_PlaceCleanTable:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			int nJob = CheckJob_CleanBuff(m_nJobAtsIdx, DEF_PLACE, m_cJobPocket, m_vJobPicker_Place);
			if (nJob != eExcutingStep_ErrorJob)
				ChangeSubState(eExcutingStep_Move_PlaceCleanTable);
			else
			{		
				ChangeSubState(eExcutingStep_Check_More_Job);			
			}
		}
	}break;
	case eExcutingStep_Check_More_Job:
	{
		if (g_testpp_work_area[m_eTestIdx].get_reserved_task(m_vModuleSetPressIdx[m_nJobAtsIdx], this))
			MakeLog(_T("[==== FindJob release_work_area=%s =====]"), g_TaskTestPP[m_eTestIdx].GetLocName(LOC_IDX_STATION_1 + m_nJobAtsIdx));
		
			g_testpp_work_area[m_eTestIdx].release_work_area(m_vModuleSetPressIdx[m_nJobAtsIdx], this);
		
			int nTblLdDvcCnt = g_TaskLoadTbl[m_nLoadingTableInx].GetLoadDeviceCnt();
			int nTblUldDvcCnt = g_TaskLoadTbl[m_nUnLoadingTableInx].GetUnloadDeviceCnt();

			int nAARetestDeviceCnt = 0;
			for (auto nLoadTblIdx : m_vModuleSetLoadTableIdx)
			{
				nAARetestDeviceCnt += g_TaskLoadTbl[nLoadTblIdx].GetBufferDeviceAARetestCnt();
			}
							
			int station_device_cnt = g_DMCont.m_dmDevice.GetDeviceCount(eDeviceDataStage::eDeviceStage_TEST_SITE_1 + m_vModuleSetPressIdx[m_nJobAtsIdx]);

			int nChkModuleSiteInDvc = 0;

			if (m_nUnLoadingTableInx == eLoadTbl_1 || m_nUnLoadingTableInx == eLoadTbl_4)
			{
				for (auto mdIdx : m_vModuleSetPressIdx)
					nChkModuleSiteInDvc += g_TaskPressUnit[mdIdx].ChkSiteInDvc();
			}
					
			if (g_TaskAllAutoStacker.GetLoadEnableStackerCount() == 0)
			{
				int a = 1;
			}
			int nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
			int nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
			int nMotorLoadPos = g_TaskLoadTbl[m_nLoadingTableInx].ChkMotorPos(LOC_IDX_LD_TBL_TEST_SIDE_1 + m_nJobAtsIdx, CTaskLoadTable::eAxisY);
			if ((g_TaskLoadTbl[m_nLoadingTableInx].GetWorkReserve() == RID_TEST_PP_(m_eTestIdx) && nMotorLoadPos == ERR_NO_ERROR &&nTestSite_Div_Y == STATION_MAX_Y_SITE && nTblLdDvcCnt != 8 && g_TaskLoadTbl[m_nLoadingTableInx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready) ||
				(g_TaskLoadTbl[m_nLoadingTableInx].GetWorkReserve() == RID_TEST_PP_(m_eTestIdx) && nMotorLoadPos == ERR_NO_ERROR &&nTestSite_Div_Y == 1 && nTblLdDvcCnt < 4 && g_TaskLoadTbl[m_nLoadingTableInx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)) {
				g_TaskLoadTbl[m_nLoadingTableInx].SetWorkReserve(RID_INIT_IDLE_RESERVE/*RID_TRAY_PP_(RIDX_TRAY_PP_1)*/);
				g_DMCont.m_dmShuttle.SB(BDM3_LOAD_TABLE_1_WORK_RESERVE_IDLE + m_eTestIdx, TRUE);
			}
						

			int nMotorUnloadPos = g_TaskLoadTbl[m_nUnLoadingTableInx].ChkMotorPos(LOC_IDX_LD_TBL_TEST_SIDE_1+ m_nJobAtsIdx, CTaskLoadTable::eAxisY);
			if (g_TaskLoadTbl[m_nUnLoadingTableInx].GetWorkReserve() == RID_TEST_PP_(m_eTestIdx) && nMotorUnloadPos == ERR_NO_ERROR && nTblUldDvcCnt > 0 && g_TaskLoadTbl[m_nUnLoadingTableInx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready
				|| g_TaskAllAutoStacker.GetLoadEnableStackerCount() == 0 && nChkModuleSiteInDvc > 0)
			{
				if (g_TaskLoadTbl[m_nUnLoadingTableInx].GetUnloadDeviceCnt() != 0)
					g_TaskLoadTbl[m_nUnLoadingTableInx].SetWorkReserve(RID_INIT_IDLE_RESERVE/*RID_TRAY_PP_(RIDX_TRAY_PP_1)*/);
			}

			int nChkSiteInDvc = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].ChkSiteInDvc();

			// 더 이상 해당 Station에 해야 할 작업이 없다면 Job Index 증가
			//*********************************************************//////////////////////////
			// 1. One Cycle 상태가 On이고, Load, Unload Table 합산 Device Count 0개면 JobIndex 증가
			// 2. Load Tray 공급이 끝났고, Load, Unload Table 합산 Device Count 0이고, 현재 JobIndex Station 내부에 Device가 없으면 JobIndex 증가
			if ((g_TaskSystemCtrl.IsOneCycleModeOn() == true && nTblLdDvcCnt + nTblUldDvcCnt == 0) ||
				(g_TaskAllAutoStacker.GetLoadEnableStackerCount() == 0 && nTblLdDvcCnt + nTblUldDvcCnt == 0 && nChkSiteInDvc == 0))
			{
				g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx, ++m_nJobAtsIdx);
			}

			if (m_nJobAtsIdx > eATS_DN_STATION_L4 || m_nJobAtsIdx < eATS_DN_STATION_L1)
			{
				m_nJobAtsIdx = eATS_DN_STATION_L1;
			}

			int nStationInfo = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetStationInfo();
			nChkSiteInDvc = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].ChkSiteInDvc();
			int nChkSiteOnOffCnt = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetSocketOnCnt();

			m_nCleanMode = ChkCleaningMode(m_nJobAtsIdx);

			if (m_nCleanMode != TRUE && (nStationInfo != eTestSite_CleanEnd /*&& g_DMCont.m_dmEQP.GB(BDM0_TEST_STATUS_SITE1 + m_vModuleSetPressIdx[m_nJobAtsIdx]) == FALSE*/)
				&& (nChkSiteInDvc == TRUE && nChkSiteOnOffCnt > 0))
			{
				m_bAlreadyPick = FALSE;

				int nNextJobIdx = 0;
				int nJobAtsIdx = -1;

				nJobAtsIdx = m_nJobAtsIdx;

				if (PossibleAlreadyPickLoadTable(m_eTestIdx, nJobAtsIdx, nNextJobIdx) == TRUE) {
					m_bAlreadyPick = TRUE;
					m_nJobAtsIdx = nNextJobIdx;
					g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx, m_nJobAtsIdx);
					MakeLog(_T("[PossibleAlreadyPickLoadTable Next Job Idx = %d]"), m_nJobAtsIdx + 1);
					ChangeSubState(eExcutingStep_Move_Already_Table);
					break;
				}
			}

			if (m_nCleanMode == TRUE && g_TaskTestPP[m_eTestIdx].GetLoadPickerLoadDeviceCnt() > 0 && g_DMCont.m_dmEQP.GB(BDM0_TEST_STATUS_SITE1 + m_vModuleSetPressIdx[m_nJobAtsIdx]) != TRUE) {
				ChangeSubState(eExcutingStep_Move_Already_Pick_Dvc_PlaceArtBuffTable);
				MakeLog("Test1");
				break;
			}

			ChangeSubState(eExcutingStep_Check_Job);
		//}
	}break;
	// socket clean job on, buffer table place - load hand
	case eExcutingStep_Move_Already_Pick_Dvc_PlaceArtBuffTable:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready) {

			int nLocIdx = -1;
			int nJob = CheckJobAlreadyPickDvcPlacetoArtBuff(m_nJobAtsIdx, m_cJobPocket, m_vJobPicker_Place, nLocIdx);

			if (nJob != eExcutingStep_ErrorJob && nLocIdx != -1)
			{			
				CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
				pSeqCmd->m_nCmdStage = nLocIdx + m_nJobAtsIdx;
				pSeqCmd->m_cCmdPocket = m_cJobPocket;
				pSeqCmd->m_vCmdPicker = m_vJobPicker_Place;
				pSeqCmd->m_nCmdPick = DEF_PLACE;
				pSeqCmd->m_nHandType = TEST_LD_HAND;
				MakeLog(_T("[ART_BUFFER PLACE(Load Picker)][x=%d,y=%d][%s][%s]"), m_cJobPocket.x + 1, m_cJobPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Place), g_TaskTestPP[m_eTestIdx].GetHandTypeName(pSeqCmd->m_nHandType));

				if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) == eSeqCmdAck_OK) {
					ChangeSubState(eExcutingStep_WaitEnd_Already_Pick_Dvc__PlaceArtBuffTable);
				}
			}
			else {// if (nJob == eExcutingStep_ErrorJob) {
				MakeLog(_T("[ART_BUFFER PLACE(Load Picker) Error!! Next Job Idx]"));
				g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx, ++m_nJobAtsIdx);
				ChangeSubState(eExcutingStep_Check_Job);
			}
		}
	}break;
	case eExcutingStep_WaitEnd_Already_Pick_Dvc__PlaceArtBuffTable:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			ChangeSubState(eExcutingStep_Check_Job);
		}
	}break;
 	// index time 증가를 위한 Load Table 미리 대기
	case eExcutingStep_Move_Already_Table:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			MakeLog(_T("[TABLE %d ALREADY START]"), m_nJobAtsIdx+1);
			CTestPPCmd_MoveAlreadyPos* pSeqCmd = new CTestPPCmd_MoveAlreadyPos;		
			pSeqCmd->m_nCmdStage = LOC_IDX_TEST_PP_TBL_1_LD + m_nJobAtsIdx;
			pSeqCmd->m_cCmdPocket = CPoint(0, 0);
			pSeqCmd->m_vCmdPicker.push_back(CPoint(0, 0));
			pSeqCmd->m_nHandType = eTestPPLoadType;
			//pSeqCmd->m_bXmoveHomeTarget = TRUE;

			if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) == eSeqCmdAck_OK)
				ChangeSubState(eExcutingStep_WaitEnd_Already_Table);
		}
	}break;
	case eExcutingStep_WaitEnd_Already_Table:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			MakeLog(_T("[TABLE %d ALREADY END]"), m_nJobAtsIdx+1);
			ChangeSubState(eExcutingStep_Check_Job);		
		}
	}break;
	// all test load table already pick - load hand
	case eExcutingStep_Move_Already_All_Test_PickLoadTable:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready) {
			m_bAlreadyPick = TRUE;
  			int nJob = CheckJob_LoadTbl(m_nJobAtsIdx, DEF_PICK, m_cJobPocket, m_vJobPicker_Pick);
  			if (nJob == eExcutingStep_WaitOtherJob)
  				break;

			int nLoadTableWorkPos = GetReserveAndLoadTblWorkPos(m_nLoadingTableInx);
			if (g_TaskSystemCtrl.IsOneCycleModeOn() == TRUE || nLoadTableWorkPos == FALSE)
			{
				ChangeSubState(eExcutingStep_Check_Job);
				break;
			}

  			if (nJob == eExcutingStep_FindJob)
  			{
				bool bWork = g_TestPP_Table_work_area[m_eTestIdx].reserve_work_area(&g_TaskAutoTestPp[m_eTestIdx]);
				if (bWork != true)
				{
					break;
				}				
				
				g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_ALREADY_PICK_WORKING_IDX + m_eTestIdx, m_nJobAtsIdx +1);

  				CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
  				pSeqCmd->m_nCmdStage = LOC_IDX_TEST_PP_TBL_1_LD + m_nJobAtsIdx;
  				pSeqCmd->m_cCmdPocket = m_cJobPocket;
  				pSeqCmd->m_nCmdPick = DEF_PICK;
  				pSeqCmd->m_vCmdPicker = m_vJobPicker_Pick;
  				pSeqCmd->m_nHandType = TEST_LD_HAND;
				pSeqCmd->m_bAlreadyPick = TRUE;
			
				MakeLog(_T("[ALL TEST TABLE %d ALREADY PICK START][x=%d,y=%d][%s][%s]"), m_nJobAtsIdx + 1,m_cJobPocket.x + 1, m_cJobPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Pick), g_TaskTestPP[m_eTestIdx].GetHandTypeName(pSeqCmd->m_nHandType));

  				if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) == eSeqCmdAck_OK) {
  				}
  			}

			ChangeSubState(eExcutingStep_WaitEnd_Already_All_Test_PickLoadTable);
		}
	}break;
	case eExcutingStep_WaitEnd_Already_All_Test_PickLoadTable:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			g_TestPP_Table_work_area[m_eTestIdx].release_work_area(&g_TaskAutoTestPp[m_eTestIdx]);
			g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_ALREADY_PICK_WORKING_IDX + m_eTestIdx, -1);
			MakeLog(_T("[ALL TEST Table %d ALREADY PICK END]"), m_nJobAtsIdx + 1);
			g_DMCont.m_dmTestPP.SB(NDM04_TESTHAND1_ALREADY_PICK_TABLE_CALL + m_eTestIdx, FALSE);

			int nTblLdDvcCnt = g_TaskLoadTbl[m_nLoadingTableInx].GetLoadDeviceCnt();
			int nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
			int nLoadDeviceCnt = g_TaskTestPP[m_eTestIdx].GetLoadHandDeviceCnt();
			int nMotorLoadPos = g_TaskLoadTbl[m_nLoadingTableInx].ChkMotorPos(LOC_IDX_LD_TBL_TEST_SIDE_1 + m_nJobAtsIdx, CTaskLoadTable::eAxisY);
			if ((g_TaskLoadTbl[m_nLoadingTableInx].GetWorkReserve() == RID_TEST_PP_(m_eTestIdx) && nTestSite_Div_Y == STATION_MAX_Y_SITE && nTblLdDvcCnt != 8) ||
				(g_TaskLoadTbl[m_nLoadingTableInx].GetWorkReserve() == RID_TEST_PP_(m_eTestIdx) && nTestSite_Div_Y == 1 && nTblLdDvcCnt < 4)) {
				MakeLog(_T("[ALL TEST Table ALREADY PICK SetWorkReserve Load Table job = [%d]]"), m_nJobAtsIdx + 1);
				g_TaskLoadTbl[m_nLoadingTableInx].SetWorkReserve(RID_INIT_IDLE_RESERVE/*RID_TRAY_PP_(RIDX_TRAY_PP_1)*/);
				g_DMCont.m_dmShuttle.SB(BDM3_LOAD_TABLE_1_WORK_RESERVE_IDLE + m_eTestIdx, TRUE);
			}

			ChangeSubState(eExcutingStep_Move_Already_Station);
		}
	}break;
	// station already move pos - unload hand
	case eExcutingStep_Move_Already_Station:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			MakeLog(_T("[Station %d ALREADY START]"), m_nJobAtsIdx + 1);
			CTestPPCmd_MoveAlreadyPos* pSeqCmd = new CTestPPCmd_MoveAlreadyPos;
			pSeqCmd->m_nCmdStage = LOC_IDX_STATION_1 + m_nJobAtsIdx;
			pSeqCmd->m_cCmdPocket = CPoint(0, 0);
			pSeqCmd->m_vCmdPicker.push_back(CPoint(0, 0));
			pSeqCmd->m_nHandType = TEST_UD_HAND;

			if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) == eSeqCmdAck_OK)
				ChangeSubState(eExcutingStep_WaitEnd_Already_Station);
		}
	}break;
	case eExcutingStep_WaitEnd_Already_Station:
	{
		if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			MakeLog(_T("[Station %d ALREADY END]"), m_nJobAtsIdx + 1);

			ChangeSubState(eExcutingStep_Check_Job);
		}
	}break;
	}
	
}

void CTaskAutoTestPp::AutoState_Execute_AutoGrr()
{
	 switch (m_nAutoRunSubStep)
	 {
		case eExcutingStepGrr_Start:
		{
// 			//m_nJobAtsIdx = g_DMCont.m_dmTestPP.GN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx); // 저장 된 마지막 idx로 복구
 			g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx, eATS_DN_STATION_L1);
// 			MakeLog(_T("[eExcutingStep_Check_Job Grr, WorkingStart=%d"), m_nJobAtsIdx);

			m_nJobAtsIdx = g_DMCont.m_dmTestPP.GN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx); // 저장 된 마지막 idx로 복구
																							 //MakeLog(_T("[eExcutingStep_Check_Job, WorkingStart=%d"), m_nJobAtsIdx);

			int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
			BOOL bAutoUseGRR = FALSE;
			if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
				bAutoUseGRR = TRUE;

			if (GetModuleSocketOnCnt(bAutoUseGRR) == 0)
				break;
			else
			{
				if (m_nJobAtsIdx > eATS_DN_STATION_L4 || m_nJobAtsIdx < eATS_DN_STATION_L1)
				{
					m_nJobAtsIdx = eATS_DN_STATION_L1;
				}

				CTestPPCmd_MoveAlreadyPos* pSeqCmd = new CTestPPCmd_MoveAlreadyPos;
				pSeqCmd->m_nCmdStage = LOC_IDX_TEST_PP_TBL_1_LD + m_nJobAtsIdx;
				pSeqCmd->m_cCmdPocket = CPoint(0, 0);
				pSeqCmd->m_vCmdPicker.push_back(CPoint(0, 0));
				pSeqCmd->m_nHandType = TEST_LD_HAND;
				if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) == eSeqCmdAck_OK)
					ChangeSubState(eExcutingStepGrr_WaitLoadTable);
			}

			/*ChangeSubState(eExcutingStepGrr_WaitLoadTable);*/
		}break;
		case eExcutingStepGrr_WaitLoadTable:
		{
			if (ChkStationOnOffDvcCnt(TRUE) == 0)
				break;

			int nIdx = GetTableIdx(eLoadType);
			if (g_TaskLoadTbl[nIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				m_nJobAtsIdx = g_DMCont.m_dmTestPP.GN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx);
				int nTableIdx = g_TaskLoadTbl[nIdx].GrrSetTableIdx(LOC_IDX_LD_TBL_TEST_SIDE_1 + m_nJobAtsIdx);
				int nChkMotor = g_TaskLoadTbl[nIdx].ChkMotorPos(/*LOC_IDX_LD_TBL_TEST_SIDE_1 + m_nJobAtsIdx*/nTableIdx, CTaskLoadTable::eAxisAll);
				if (nChkMotor == ERR_NO_ERROR && g_TaskLoadTbl[nIdx].GetWorkReserve() == RID_TEST_PP_(m_eTestIdx))
				{
					ChangeSubState(eExcutingStepGrr_Check_Job);
				}
				else
				{
					g_TaskLoadTbl[nIdx].SetTestppPrePos(nTableIdx);
				}
			}

 		}break;
		case eExcutingStepGrr_Check_Job:
		{
			if (m_nJobAtsIdx > eATS_DN_STATION_L4 || m_nJobAtsIdx < eATS_DN_STATION_L1)
				m_nJobAtsIdx = eATS_DN_STATION_L1;

			MakeLog(_T("[Check Job][Start Job Index = %d]"), m_nJobAtsIdx);
			bool bWorkPossible = g_testpp_work_area[m_eTestIdx].is_work_enable_state(m_vModuleSetPressIdx[m_nJobAtsIdx], this);

			if (m_nJobAtsIdx == 0 && bWorkPossible != true) {
				int a = 0;
			}

			int nChkInterlock = ChkInterlockAts(m_nJobAtsIdx); //Press Unit Safety 확인.
			int nStationOnCnt = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetSocketOnCntGrr();
			int nStationInfo = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetStationInfo(TRUE);

			m_nCleanMode = ChkCleaningMode(m_nJobAtsIdx);

			if (nStationOnCnt > 0)
			{
				if (bWorkPossible == true && nChkInterlock == ERR_NO_ERROR /*&& nStationInfo != eTestSite_CleanLoading*/)
				{
					bool bWork = g_testpp_work_area[m_eTestIdx].reserve_work_area(m_vModuleSetPressIdx[m_nJobAtsIdx], this);
					if (bWork == true) {
						g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx, m_nJobAtsIdx);
						//m_nJobSiteRow = 0;
						ChangeSubState(eExcutingStepGrr_Move_PickCleanTable);
						break;
					}
				}

				//reserve 안되어 있으면
				BOOL m_bGRR_Audit_loop = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrAuditLoopUse);
				int nSetInDvc = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrUnitCount);
				int nModule1On = g_TaskAutoTestPp[eTestPP_1].GetModuleSocketOnCnt(TRUE);
				int nModule2On = g_TaskAutoTestPp[eTestPP_2].GetModuleSocketOnCnt(TRUE);
				if (nModule1On > 0 && nModule2On > 0)
					nSetInDvc /= 2;

				int nReservedDvcCnt = g_DMCont.m_dmTestPP.GN(NDM04_SITEGROUP1_INSERTED_DEVICE_CNT + m_eTestIdx);
				int nShtPocketX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
				int nShtPocketY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

				if (nReservedDvcCnt == nSetInDvc && g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetSiteInDvc() == 0 && m_bGRR_Audit_loop == FALSE)
				{
					++m_nJobAtsIdx;
					if (m_nJobAtsIdx > eATS_DN_STATION_L4 || m_nJobAtsIdx < eATS_DN_STATION_L1)
						m_nJobAtsIdx = eATS_DN_STATION_L1;
					g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx, m_nJobAtsIdx);
					MakeLog(_T("[Check Job][Station On][CHANGE JOB INDEX = %d][1]"), m_nJobAtsIdx);
				}
				else if (m_bGRR_Audit_loop == TRUE && g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetSiteInDvc() == 0)
				{
					++m_nJobAtsIdx;
					if (m_nJobAtsIdx > eATS_DN_STATION_L4 || m_nJobAtsIdx < eATS_DN_STATION_L1)
						m_nJobAtsIdx = eATS_DN_STATION_L1;
					g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx, m_nJobAtsIdx);
					MakeLog(_T("[Check Job][Station On][CHANGE JOB INDEX = %d][2]"), m_nJobAtsIdx);
				}
				else if (g_TaskSystemCtrl.GetAutoGrrCancel() == TRUE)
				{
					int nInSiteDvc = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetSiteInDvc();
					if (nInSiteDvc == 0)
					{
						++m_nJobAtsIdx;
						if (m_nJobAtsIdx > eATS_DN_STATION_L4 || m_nJobAtsIdx < eATS_DN_STATION_L1)
							m_nJobAtsIdx = eATS_DN_STATION_L1;
						g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx, m_nJobAtsIdx);
						MakeLog(_T("[Check Job][Station On][CHANGE JOB INDEX = %d][3]"), m_nJobAtsIdx);
					}
				}
				else
				{
					//++m_nJobAtsIdx;
					if (m_nJobAtsIdx > eATS_DN_STATION_L4 || m_nJobAtsIdx < eATS_DN_STATION_L1)
						m_nJobAtsIdx = eATS_DN_STATION_L1;
					g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx, m_nJobAtsIdx);
					MakeLog(_T("[Check Job][Station On][CHANGE JOB INDEX = %d][4]"), m_nJobAtsIdx);
				}
				//MakeLog(_T("[Check Job][Station On][CHANGE JOB INDEX = %d]"), m_nJobAtsIdx);
			}
			else {
				++m_nJobAtsIdx;
				if (m_nJobAtsIdx > eATS_DN_STATION_L4 || m_nJobAtsIdx < eATS_DN_STATION_L1)
					m_nJobAtsIdx = eATS_DN_STATION_L1;
				g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx, m_nJobAtsIdx);
				MakeLog(_T("[Check Job][Station Off][CHANGE JOB INDEX = %d]"), m_nJobAtsIdx);
			}

			//예약 안되어 있으면 Loading Table reserve 해제
			//g_TaskLoadTbl[m_nLoadingTableInx].SetWorkReserve(RID_INIT_IDLE_RESERVE/*RID_TRAY_PP_(RIDX_TRAY_PP_1)*/);
		}break;
// 		case eExcutingStepGrr_WaitLoadTable:
// 		{
// 			if (ChkStationOnOffDvcCnt(TRUE) == 0)
// 				break;
// 
// 			int nIdx = GetTableIdx(eLoadType);
// 			if (g_TaskLoadTbl[nIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
// 			{
// 				if (nIdx == 2) {
// 					int a = 0;
// 				}
// 				m_nJobAtsIdx = g_DMCont.m_dmTestPP.GN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx);
// 				int nTableIdx = g_TaskLoadTbl[nIdx].GrrSetTableIdx(LOC_IDX_LD_TBL_TEST_SIDE_1 + m_nJobAtsIdx);
// 				int nChkMotor = g_TaskLoadTbl[nIdx].ChkMotorPos(/*LOC_IDX_LD_TBL_TEST_SIDE_1 + m_nJobAtsIdx*/nTableIdx, CTaskLoadTable::eAxisAll);
// 				if (nChkMotor == ERR_NO_ERROR && g_TaskLoadTbl[nIdx].GetWorkReserve() == RID_TEST_PP_(m_eTestIdx))
// 				{
// 					ChangeSubState(eExcutingStepGrr_Move_PickCleanTable);
// 				}
// 				else
// 				{
// 					g_TaskLoadTbl[nIdx].SetTestppPrePos(nTableIdx);
// 				}
// 			}
// 
// 		}break;


		//pick clean buffer
		case eExcutingStepGrr_Move_PickCleanTable:
		{
			if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				//clean 만들기
 				int nJob = ChkJobGrr_CleanBuff(m_nJobAtsIdx, DEF_PICK, m_cJobPocket, m_vJobPicker_Pick);
 				if (nJob != eExcutingStep_ErrorJob)
 				{
 					CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
					pSeqCmd->m_nCmdStage = LOC_IDX_CLEAN_TBL;
 					pSeqCmd->m_cCmdPocket = m_cJobPocket;
 					pSeqCmd->m_vCmdPicker = m_vJobPicker_Pick;
 					pSeqCmd->m_nCmdPick = DEF_PICK;
					pSeqCmd->m_nHandType = TEST_LD_HAND;
 					MakeLog(_T("[MainStep=%s][CLEAN_BUFFER PICK][x=%d,y=%d][%s]"), GetLotStateToString(m_nAutoRunMainStep), m_cJobPocket.x + 1, m_cJobPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Pick));
 
 					if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) == eSeqCmdAck_OK) {
 					}
 				}
				ChangeSubState(eExcutingStepGrr_WaitEnd_PickCleanTable);
			}
		}break;
		case eExcutingStepGrr_WaitEnd_PickCleanTable:
		{
			if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
 				int nJob = ChkJobGrr_CleanBuff(m_nJobAtsIdx, DEF_PICK, m_cJobPocket, m_vJobPicker_Pick);
 				if (nJob != eExcutingStep_ErrorJob)
 					ChangeSubState(eExcutingStepGrr_Move_PickCleanTable);
 				else
					ChangeSubState(eExcutingStepGrr_Move_PickArtBuffTable_Swap);
			}
		}break;
		case eExcutingStepGrr_Move_PickArtBuffTable_Swap:
		{
			if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				BOOL bSwapMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrDeviceSwapUse);
				if (!bSwapMode) {
					ChangeSubState(eExcutingStepGrr_Move_PickArtBuffTable);
					break;
				}

				if (m_nJobAtsIdx == 0) {
					int a = 0;
				}

				if (m_nJobAtsIdx == 1) {
					int a = 0;
				}

				if (m_nJobAtsIdx == 2) {
					int a = 0;
				}

				if (m_nJobAtsIdx == 3) {
					int a = 0;
				}

				int nLocIdx = -1;
				int nJob = ChkJobGrr_SwapArtBuff(m_nJobAtsIdx, DEF_PICK, m_cJobPocket, m_vJobPicker_Pick, nLocIdx);

				if (nJob == eExcutingStep_FindJob && nLocIdx != -1)
				{
					//int nTableIdx = g_TaskLoadTbl[m_nUnLoadingTableInx].GrrSetTableIdx(nLocIdx + m_nJobAtsIdx);
					int nLoc = GrrTableChangeLoc(nLocIdx + m_nJobAtsIdx);
					CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
					pSeqCmd->m_nCmdStage = /*nLocIdx + m_nJobAtsIdx*/nLoc;
					pSeqCmd->m_cCmdPocket = m_cJobPocket;
					pSeqCmd->m_vCmdPicker = m_vJobPicker_Pick;
					pSeqCmd->m_nCmdPick = DEF_PICK;
					pSeqCmd->m_nHandType = TEST_LD_HAND;
					MakeLog(_T("[GRR SWAP ART BUFF PICK][JobIndex = %d][x=%d,y=%d][%s]"), m_nJobAtsIdx, m_cJobPocket.x + 1, m_cJobPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Pick));

					if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) != eSeqCmdAck_OK) {
					}
				}
				ChangeSubState(eExcutingStepGrr_Wait_PickArtBuffTable_Swap);
			}
		}break;
		case eExcutingStepGrr_Wait_PickArtBuffTable_Swap:
		{
			if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				int nLocIdx = -1;
				int nJob = ChkJobGrr_SwapArtBuff(m_nJobAtsIdx, DEF_PICK, m_cJobPocket, m_vJobPicker_Pick, nLocIdx);
				if (nJob != eExcutingStepGrr_ErrorJob)
					ChangeSubState(eExcutingStepGrr_Move_PickArtBuffTable_Swap);
				else
					ChangeSubState(eExcutingStepGrr_Move_PickArtBuffTable);
			}
		}break;
		case eExcutingStepGrr_Move_PickArtBuffTable:
		{

			if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				if (m_nJobAtsIdx == 2) {
					int a = 0;
				}

				int nLocIdx = -1;
				int nJob = ChkJobGrr_ArtBuff(m_nJobAtsIdx,DEF_PICK, m_cJobPocket, m_vJobPicker_Pick, nLocIdx);

				//Loading Table Wait
				if (nJob == eExcutingStepGrr_WaitOtherJob)
					break;

				//
				if (nJob == eExcutingStep_FindJob && nLocIdx != -1)
				{
					//int nTableIdx = g_TaskLoadTbl[m_nUnLoadingTableInx].GrrSetTableIdx(nLocIdx + m_nJobAtsIdx);
					int nLoc = GrrTableChangeLoc(nLocIdx + m_nJobAtsIdx);
					CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
					pSeqCmd->m_nCmdStage = /*nLocIdx + m_nJobAtsIdx*/nLoc;
					pSeqCmd->m_cCmdPocket = m_cJobPocket;
					pSeqCmd->m_vCmdPicker = m_vJobPicker_Pick;
					pSeqCmd->m_nCmdPick = DEF_PICK;
					pSeqCmd->m_nHandType = TEST_LD_HAND;
					MakeLog(_T("[GRR ART BUFF PICK][JobIndex = %d][x=%d,y=%d][%s]"),m_nJobAtsIdx, m_cJobPocket.x + 1, m_cJobPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Pick));

					if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) != eSeqCmdAck_OK) {
					}
				}
				else
				{
					int nBufferDvcCnt = g_TaskLoadTbl[0].GetBufferDeviceCnt();
					if (nBufferDvcCnt > 0) {
						int a = 0;
					}
					int nBufferDvcCnt2 = g_TaskLoadTbl[1].GetBufferDeviceCnt();
					if (nBufferDvcCnt2 > 0) {
						int a = 0;
					}
				}


				ChangeSubState(eExcutingStepGrr_Wait_PickArtBuffTable);
			}
		}break;
		case eExcutingStepGrr_Wait_PickArtBuffTable:
		{
			if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				int nLocIdx = -1;
				int nJob = ChkJobGrr_ArtBuff(m_nJobAtsIdx, DEF_PICK, m_cJobPocket, m_vJobPicker_Pick, nLocIdx);
				if (nJob != eExcutingStepGrr_ErrorJob)
					ChangeSubState(eExcutingStepGrr_Move_PickArtBuffTable);
				else
					ChangeSubState(eExcutingStepGrr_Move_PickLoadTable);
			}
		}break;
		case eExcutingStepGrr_Move_PickLoadTable:
		{
			if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				int nJob = ChkJobGrr_LoadTable(m_nJobAtsIdx, DEF_PICK, m_cJobPocket, m_vJobPicker_Pick);
				if (nJob == eExcutingStepGrr_WaitOtherJob)
					break;

				if (nJob == eExcutingStepGrr_FindJob)
				{
					int nLoc = GrrTableChangeLoc(LOC_IDX_TEST_PP_TBL_1_LD + m_nJobAtsIdx);
					CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
					pSeqCmd->m_nCmdStage = /*LOC_IDX_TEST_PP_TBL_1_LD + m_nJobAtsIdx*/nLoc;
					pSeqCmd->m_cCmdPocket = m_cJobPocket;
					pSeqCmd->m_nCmdPick = DEF_PICK;
					pSeqCmd->m_vCmdPicker = m_vJobPicker_Pick;
					pSeqCmd->m_nHandType = TEST_LD_HAND;
					MakeLog(_T("[GRR LD_TBL PICK][x=%d,y=%d][%s]"), m_cJobPocket.x + 1, m_cJobPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Pick));

					if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) != eSeqCmdAck_OK) {
					}
				}
				ChangeSubState(eExcutingStepGrr_Wait_PickLoadTable);
			}
		}break;
		case eExcutingStepGrr_Wait_PickLoadTable:
		{
			if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				int nJob = ChkJobGrr_LoadTable(m_nJobAtsIdx, DEF_PICK, m_cJobPocket, m_vJobPicker_Pick);
				// 				if (nJob != eExcutingStepGrr_ErrorJob)
				// 					ChangeSubState(eExcutingStepGrr_Move_PickLoadTable);
				// 				else
				// 					ChangeSubState(eExcutingStepGrr_Move_load_Table_Release);
				if (nJob == eExcutingStep_WaitOtherJob)
					break;
				if (nJob == eExcutingStep_FindJob)
					ChangeSubState(eExcutingStepGrr_Move_PickLoadTable);
				else {

					if (nJob != eExcutingStep_WaitOtherJob)
						ChangeSubState(eExcutingStepGrr_Move_load_Table_Release);
				}
			}
		}break;
		case eExcutingStepGrr_Move_load_Table_Release:
		{
			int nTblLdDvcCnt = g_TaskLoadTbl[m_nLoadingTableInx].GetLoadDeviceCnt();
			int nMotorLoadPos = g_TaskLoadTbl[m_nLoadingTableInx].ChkMotorPos(LOC_IDX_TBL_LD_SIDE, CTaskLoadTable::eAxisY);
			int nSiteArtDvcCnt = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetSiteInDvc(TRUE);

			int nSetInDvc = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrUnitCount);
			int nReservedDvcCnt = g_DMCont.m_dmTestPP.GN(NDM04_SITEGROUP1_INSERTED_DEVICE_CNT + m_eTestIdx);

			//Test End 확인.




			if (nMotorLoadPos != ERR_NO_ERROR  && g_TaskLoadTbl[m_nLoadingTableInx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready /*&& m_nBeforeStation > 0*/)
			{
				if (nSetInDvc == nReservedDvcCnt)
				{
					ChangeSubState(eExcutingStepGrr_Move_PickStation);
					break;
				}

				if (nTblLdDvcCnt != 8 && nSiteArtDvcCnt == 0)
					g_TaskLoadTbl[m_nLoadingTableInx].SetWorkReserve(RID_INIT_IDLE_RESERVE/*RID_TRAY_PP_(RIDX_TRAY_PP_1)*/);


				//Loading Table 미리 대기를 위한 Contact PrePare 상태 변경.
				//기능에 따라 상태 변경 시점이 다름.
				BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
				if (bAutoRetestUseMode != TRUE) {
					int nIdx = m_nJobAtsIdx + (4 * m_eTestIdx);

					if (m_bNotChkPreContactPos != TRUE) {
						//g_DMCont.m_dmContac_Up.SN(NDM2_PRESS_1_PRE_CONTACT_STATUS + nIdx, TRUE);
					}
					else {
						m_bNotChkPreContactPos = FALSE;
					}
				}

				ChangeSubState(eExcutingStepGrr_Move_PickStation);
			}
			// 		else {
			// 			if (g_TaskSystemCtrl.IsOneCycleModeOn() == TRUE)
			// 			{
			// 				ChangeSubState(eExcutingStep_Move_PickArtBuffTable);
			// 				break;
			// 			}
			// 		}
		}break;
		case eExcutingStepGrr_Move_PickStation:
		{
			if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				if (m_nJobAtsIdx == 0) {
					int a = 0;
				}

				int nJob = ChkJobGrr_Station(m_nJobAtsIdx, DEF_PICK, m_cJobPocket, m_vJobPicker_Pick);
				if (nJob != eExcutingStepGrr_ErrorJob)
				{
					CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
					pSeqCmd->m_nCmdStage = LOC_IDX_STATION_1 + m_nJobAtsIdx;
					pSeqCmd->m_cCmdPocket = m_cJobPocket;
					pSeqCmd->m_nCmdPick = DEF_PICK;
					pSeqCmd->m_vCmdPicker = m_vJobPicker_Pick;
					BOOL bChkCleanDev = GetDvcStationCleanDevAssign(DEF_PICK, m_nJobAtsIdx);
					if (bChkCleanDev == TRUE)
						pSeqCmd->m_nHandType = TEST_LD_HAND;
					else
						pSeqCmd->m_nHandType = TEST_UD_HAND;
					MakeLog(_T("[GRR %s PICK][x=%d,y=%d][%s]"), g_TaskTestPP[m_eTestIdx].GetLocName(LOC_IDX_STATION_1 + m_nJobAtsIdx), m_cJobPocket.x + 1, m_cJobPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Pick));

					if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) != eSeqCmdAck_OK) {
					}
				}
				else
				{
					int nCount = g_TaskTestPP[m_eTestIdx].GetLoadHandDeviceCnt();
					if (nCount > 0) {
						int a = 0;
					}
				}

				ChangeSubState(eExcutingStepGrr_Wait_PickStation);
			}
		}break;
		case eExcutingStepGrr_Wait_PickStation:
		{
			if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				int nJob = ChkJobGrr_Station(m_nJobAtsIdx, DEF_PICK, m_cJobPocket, m_vJobPicker_Pick);
				if (nJob != eExcutingStepGrr_ErrorJob)
					ChangeSubState(eExcutingStepGrr_Move_PickStation);
				else
					ChangeSubState(eExcutingStepGrr_Move_SamePlaceStation);
			}
		}break;

		case eExcutingStepGrr_Move_SamePlaceStation:
		{
			if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				if (m_nJobAtsIdx == 0) {
					int a = 0;
				}
				int nJob = ChkJobGrr_SamePlaceStation(m_cJobPocket, m_vJobPicker_Pick);
				if (nJob != eExcutingStepGrr_ErrorJob)
				{
					//cmd 변경
				   /*CTestPPCmd_MoveSamePlaceStation* pSeqCmd = new CTestPPCmd_MoveSamePlaceStation;*/
					CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
					pSeqCmd->m_nCmdStage = LOC_IDX_STATION_1 + m_nJobAtsIdx;
					pSeqCmd->m_cCmdPocket = m_cJobPocket;
					pSeqCmd->m_vCmdPicker = m_vJobPicker_Pick;
					pSeqCmd->m_nCmdPick = DEF_PLACE;
					pSeqCmd->m_nHandType = TEST_UD_HAND;
// 					BOOL bChkCleanDev = GetDvcStationCleanDevAssign(DEF_PLACE, m_nJobAtsIdx);
// 					if (bChkCleanDev == TRUE)
// 						pSeqCmd->m_nHandType = TEST_LD_HAND;
// 					else
// 						pSeqCmd->m_nHandType = TEST_UD_HAND;
					MakeLog(_T("[GRR %s Same PICK/PLACE][x=%d,y=%d][%s]"), g_TaskTestPP[m_eTestIdx].GetLocName(LOC_IDX_STATION_1 + m_nJobAtsIdx), m_cJobPocket.x + 1, m_cJobPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Pick));

					if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) != eSeqCmdAck_OK) {
 					}
				}
				ChangeSubState(eExcutingStepGrr_Wait_SamePlaceStation);
			}
		}break;
		case eExcutingStepGrr_Wait_SamePlaceStation:
		{
			if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				int nJob = ChkJobGrr_SamePlaceStation(m_cJobPocket, m_vJobPicker_Pick);
				if (nJob != eExcutingStepGrr_ErrorJob)
					ChangeSubState(eExcutingStepGrr_Move_SamePlaceStation);
				else
					ChangeSubState(eExcutingStepGrr_Move_Unload_Table_Wait);
			}
		}break;

		//Unload PP가  Unload Table Place 하느거 넣어야 함.
		//1. ArtBuffer
		//2. Unload Table
		case eExcutingStepGrr_Move_Unload_Table_Wait:
		{
/*			int nUnLoadTableWorkPos = GetReserveAndLoadTblWorkPos(m_nUnLoadingTableInx);*/

//			int nLoc = GrrTableChangeLoc(LOC_IDX_LD_TBL_TEST_SIDE_1 + m_nJobAtsIdx);
			int nTableIdx = g_TaskLoadTbl[m_nUnLoadingTableInx].GrrSetTableIdx(LOC_IDX_LD_TBL_TEST_SIDE_1 + m_nJobAtsIdx);
			int nUnLoadTableWorkPos = g_TaskLoadTbl[m_nUnLoadingTableInx].ChkMotorPos(nTableIdx, CTaskLoadTable::eAxisY);


			if (nUnLoadTableWorkPos == ERR_NO_ERROR && g_TaskLoadTbl[m_nUnLoadingTableInx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				ChangeSubState(eExcutingStepGrr_Move_PlaceArtBuffTable);
			}
			else 
			{
				g_TaskLoadTbl[m_nUnLoadingTableInx].SetTestppPrePos(nTableIdx);
			}
		}break;

		// place auto retest buffer - Unload Hand
		case eExcutingStepGrr_Move_PlaceArtBuffTable:
		{
			if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				int nLocIdx = -1;
				int nJob = ChkJobGrr_ArtBuff(m_nJobAtsIdx, DEF_PLACE, m_cJobPocket, m_vJobPicker_Place, nLocIdx);
				if (nJob == eExcutingStep_FindJob && nLocIdx != -1)
				{
					//test
					//////////////////////////////////


					int nLoc = GrrTableChangeLoc(nLocIdx + m_nJobAtsIdx);

					CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
					//loc_buffer_table
					pSeqCmd->m_nCmdStage = /*nLocIdx + m_nJobAtsIdx*/nLoc;
					pSeqCmd->m_cCmdPocket = m_cJobPocket;
					pSeqCmd->m_vCmdPicker = m_vJobPicker_Place;
					pSeqCmd->m_nCmdPick = DEF_PLACE;
					pSeqCmd->m_nHandType = TEST_UD_HAND;
					if (m_vJobPicker_Place.size() == 0) {
						int a = 0;
					}
					if (g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Place) == _T("")) {
						int a = 0;
					}

					MakeLog(_T("[GRR ART BUFF PLACE][JobIndex = %d][x=%d,y=%d][%s]"),m_nJobAtsIdx, m_cJobPocket.x + 1, m_cJobPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Place));

					if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) != eSeqCmdAck_OK) {
					}
				}
				ChangeSubState(eExcutingStepGrr_WaitEnd_PlaceArtBuffTable);
			}
		}break;
		case eExcutingStepGrr_WaitEnd_PlaceArtBuffTable:
		{
			if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				
				int nLocIdx = -1;
				int nJob = ChkJobGrr_ArtBuff(m_nJobAtsIdx, DEF_PLACE, m_cJobPocket, m_vJobPicker_Place, nLocIdx);
				if (nJob == eExcutingStep_FindJob && nLocIdx != -1)
					ChangeSubState(eExcutingStepGrr_Move_PlaceArtBuffTable);
				else {
					MakeLog(_T("[GRR ART BUFF PLACE][x=%d,y=%d][%s][End]"), m_cJobPocket.x + 1, m_cJobPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Place));
					ChangeSubState(eExcutingStepGrr_Move_PlaceLoadTable);
				}
			}
		}break;

		case eExcutingStepGrr_Move_PlaceLoadTable:
		{
			if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				int nJob = ChkJobGrr_LoadTable(m_nJobAtsIdx, DEF_PLACE, m_cJobPocket, m_vJobPicker_Place);
				if (nJob == eExcutingStepGrr_WaitOtherJob)
					break;

				if (nJob == eExcutingStepGrr_FindJob)
				{
					int nLoc = GrrTableChangeLoc(LOC_IDX_TEST_PP_TBL_1_UD + m_nJobAtsIdx);
					CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
					pSeqCmd->m_nCmdStage = /*LOC_IDX_STATION_1 + m_nJobAtsIdx*/nLoc;
					pSeqCmd->m_cCmdPocket = m_cJobPocket;
					pSeqCmd->m_nCmdPick = DEF_PLACE;
					pSeqCmd->m_vCmdPicker = m_vJobPicker_Place;
					pSeqCmd->m_nHandType = TEST_UD_HAND;
					MakeLog(_T("[GRR LD_TBL PLACE][x=%d,y=%d][%s]"), m_cJobPocket.x + 1, m_cJobPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Place));

					if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) != eSeqCmdAck_OK) {
					}
				}
				ChangeSubState(eExcutingStepGrr_Wait_PlaceLoadTable);
			}
		}break;
		case eExcutingStepGrr_Wait_PlaceLoadTable:
		{
			if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				int nJob = ChkJobGrr_LoadTable(m_nJobAtsIdx, DEF_PLACE, m_cJobPocket, m_vJobPicker_Place);
				if (nJob != eExcutingStepGrr_ErrorJob)
					ChangeSubState(eExcutingStepGrr_Move_PlaceLoadTable);
				else
					ChangeSubState(eExcutingStepGrr_Move_Unload_Table_Release);
			}
		}break;
		//Unload Table Release
		case eExcutingStepGrr_Move_Unload_Table_Release:
		{
			int nBufferDvcCnt = 0;

			int nChkSiteInDvc = 0;
			if (m_nUnLoadingTableInx == eLoadTbl_1 || m_nUnLoadingTableInx == eLoadTbl_4)
			{
				for (auto mdIdx : m_vModuleSetPressIdx)
					nChkSiteInDvc += g_TaskPressUnit[mdIdx].ChkSiteInDvc();

				//Unload Table Buffer Count
				nBufferDvcCnt = g_TaskLoadTbl[m_nUnLoadingTableInx].GetBufferDeviceCnt();
			}

			int nTblUldDvcCnt = g_TaskLoadTbl[m_nUnLoadingTableInx].GetUnloadDeviceCnt();
			int nMotorUnloadPos = g_TaskLoadTbl[m_nUnLoadingTableInx].ChkMotorPos(LOC_IDX_TBL_LD_SIDE, CTaskLoadTable::eAxisY);
			if (nMotorUnloadPos != ERR_NO_ERROR && nTblUldDvcCnt > 0 && g_TaskLoadTbl[m_nUnLoadingTableInx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready
				|| g_TaskAllAutoStacker.GetLoadEnableStackerCount() == 0 && nChkSiteInDvc > 0)
			{
				if (g_TaskLoadTbl[m_nUnLoadingTableInx].GetUnloadDeviceCnt() != 0 && nBufferDvcCnt == 0)
					g_TaskLoadTbl[m_nUnLoadingTableInx].SetWorkReserve(RID_INIT_IDLE_RESERVE/*RID_TRAY_PP_(RIDX_TRAY_PP_1)*/);
			}

			ChangeSubState(eExcutingStepGrr_Move_PlaceStation);
		}break;

		//Station Place - Load Hand
		case eExcutingStepGrr_Move_PlaceStation:
		{
			if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				int nJob = ChkJobGrr_Station(m_nJobAtsIdx, DEF_PLACE, m_cJobPocket, m_vJobPicker_Place);

				if (nJob != eExcutingStepGrr_ErrorJob)
				{
					CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
					pSeqCmd->m_nCmdStage = LOC_IDX_STATION_1 + m_nJobAtsIdx;
					pSeqCmd->m_cCmdPocket = m_cJobPocket;
					pSeqCmd->m_nCmdPick = DEF_PLACE;
					pSeqCmd->m_nHandType = TEST_LD_HAND;
					pSeqCmd->m_vCmdPicker = m_vJobPicker_Place;
					MakeLog(_T("[GRR %s PLACE][x=%d,y=%d][%s]"), g_TaskTestPP[m_eTestIdx].GetLocName(LOC_IDX_STATION_1 + m_nJobAtsIdx), m_cJobPocket.x + 1, m_cJobPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Place));

					if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) != eSeqCmdAck_OK) {
					}
				}
				ChangeSubState(eExcutingStepGrr_Wait_PlaceStation);
			}
		}break;
		case eExcutingStepGrr_Wait_PlaceStation:
		{
			if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				int nJob = ChkJobGrr_Station(m_nJobAtsIdx, DEF_PLACE, m_cJobPocket, m_vJobPicker_Place);
				if (nJob != eExcutingStepGrr_ErrorJob)
					ChangeSubState(eExcutingStepGrr_Move_PlaceStation);
				else
					ChangeSubState(eExcutingStepGrr_Move_PlaceCleanTable);
			}
		}break;

		//place clean buffer
		case eExcutingStepGrr_Move_PlaceCleanTable:
		{
			if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				int nJob = ChkJobGrr_CleanBuff(m_nJobAtsIdx, DEF_PLACE, m_cJobPocket, m_vJobPicker_Place);
			//	int nJob = CheckJob_CleanBuff(m_nJobAtsIdx, DEF_PLACE, m_cJobPocket, m_vJobPicker_Place);
				if (nJob != eExcutingStep_ErrorJob)
				{
					CTestPPCmd_MovePickPlace* pSeqCmd = new CTestPPCmd_MovePickPlace;
					//LOC_IDX_CLEAN_BUFFER
					pSeqCmd->m_nCmdStage = LOC_IDX_CLEAN_TBL;
					pSeqCmd->m_cCmdPocket = m_cJobPocket;
					pSeqCmd->m_vCmdPicker = m_vJobPicker_Place;
					pSeqCmd->m_nCmdPick = DEF_PLACE;
					pSeqCmd->m_nHandType = TEST_LD_HAND;
					MakeLog(_T("[CLEAN_BUFFER PLACE][x=%d,y=%d][%s]"), m_cJobPocket.x + 1, m_cJobPocket.y + 1, g_TaskTestPP[m_eTestIdx].GetPickerName(m_vJobPicker_Place));

					if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) == eSeqCmdAck_OK) {
					}
				}
				ChangeSubState(eExcutingStepGrr_WaitEnd_PlaceCleanTable);
			}
		}break;
		case eExcutingStepGrr_WaitEnd_PlaceCleanTable:
		{
			if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				int nJob = ChkJobGrr_CleanBuff(m_nJobAtsIdx, DEF_PLACE, m_cJobPocket, m_vJobPicker_Place);
				if (nJob != eExcutingStep_ErrorJob)
					ChangeSubState(eExcutingStepGrr_Move_PlaceCleanTable);
				else
					ChangeSubState(eExcutingStepGrr_ChkMoreJob);
			}
		}break;
	

		case eExcutingStepGrr_ChkMoreJob:
		{
// 			int nPocketCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
// 			m_nJobSiteRow++;
			if (m_nJobAtsIdx == 0) {
				int a = 0;
			}

			if (m_nJobAtsIdx == 1) {
				int a = 0;
			}

			if (m_nJobAtsIdx == 2) {
				int a = 0;
			}

			if (m_nJobAtsIdx == 3) {
				int a = 0;
			}


 			if (g_testpp_work_area[m_eTestIdx].reserve_work_area(m_vModuleSetPressIdx[m_nJobAtsIdx], this) == true)
 			{
				//Buffer Table에 Device가 있으면

				//device stage 함수 내부에서 확인.
// 				int nTestEnd = g_TaskLoadTbl[m_nUnLoadingTableInx].ChkGrrTestEnd(m_vModuleSetPressIdx[m_nJobAtsIdx]);
// 				int ntest_pp_load_hand_exist = g_TaskTestPP[m_eTestIdx].GetLoadHandDeviceCnt();
// 				int nbuffer_exist = g_TaskTestPP[m_eTestIdx].GetBufferTableCnt();
// 
// 
// 				if (ntest_pp_load_hand_exist == 0 && nbuffer_exist > 0 && nTestEnd != TRUE) { //device exist 갯수로 변경 필요
// 					ChangeSubState(eExcutingStepGrr_Check_Job);
// 					break;
// 				}

				//Swap 진행 중이면 Swap Wait에서 처리 됨.
				//Swap 완료 후, 다음 m_nJobAtsIdx에서 Buffer에 Place까지 완료 된 후, Swap 시작 할게 있으면 check job이동.
				int nLocIdx = -1;
				int nJob = ChkJobGrr_SwapArtBuff(m_nJobAtsIdx, DEF_PICK, m_cJobPocket, m_vJobPicker_Pick, nLocIdx);
				if (nJob == eExcutingStep_FindJob) {
					ChangeSubState(eExcutingStepGrr_Check_Job);
					break;
				}

				MakeLog(_T("[==== FindJob Grr reserve_work_area=%s =====]"), g_TaskTestPP[m_eTestIdx].GetLocName(LOC_IDX_STATION_1 + m_nJobAtsIdx));




				//ChangeSubState(eExcutingStepGrr_Move_PickCleanTable);
			}

			g_testpp_work_area[m_eTestIdx].release_work_area(m_vModuleSetPressIdx[m_nJobAtsIdx], this);
			MakeLog(_T("[==== FindJob Grr release_work_area=%s =====]"), g_TaskTestPP[m_eTestIdx].GetLocName(LOC_IDX_STATION_1 + m_nJobAtsIdx));

			//Cleaning 완료 후, Load 자재 투입
			m_nCleanMode = ChkCleaningMode(m_nJobAtsIdx);

			//Cleaning Initial에만 적용.			
			//초기 site In Dvc Check
			//Station에 Device없고 Table에도 없어야 함.
			int nSiteInDvcCnt = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].ChkSiteInDvc();
			int nTableBufferInDvcCnt = g_TaskLoadTbl[m_nUnLoadingTableInx].GetBufferDeviceCnt() + g_TaskLoadTbl[m_nLoadingTableInx].GetBufferDeviceCnt();
//			int nTableLoadInDvcCnt = g_TaskLoadTbl[m_nLoadingTableInx].GetLoadDeviceCnt();
//  			BOOL nLDTblInDvcCnt = g_TaskLoadTbl[m_nLoadingTableInx].GetTableDeviceInCnt();
//  			BOOL nUDTblInDvcCnt = g_TaskLoadTbl[m_nUnLoadingTableInx].GetTableDeviceInCnt();
// // 
//  
//  			BOOL bTblInDvcCnt = FALSE;
//  			if (nLDTblInDvcCnt == TRUE || nUDTblInDvcCnt == TRUE)
//  				bTblInDvcCnt = TRUE;

			int nCleanDvcCnt = 0;
			int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
			int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
			ST_DD_DEVICE stDvc;
			for (int nX = 0; nX < nSiteDivX; nX++)
			{
				for (int nY = 0; nY < nSiteDivY; nY++)
				{
					stDvc.clear();
					g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetSiteDeviceData(&stDvc, nX, nY);
					if (stDvc.sExist == DEF_EXIST && strcmp(stDvc.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0)
						nCleanDvcCnt++;
				}
			}




			//1. Station에 Socket Clean Dvc 존재 확인.
			//2. socket이 On 되어 있고, site dvc가 0이면 JobIdx 유지
			//BOOL bErr = stData.stGrrTestInfo[m_vModuleSetPressIdx[m_nJobAtsIdx]].chkTestEnd(/*nSearchHandX, m_nJobSiteRow*/);
			if (nCleanDvcCnt > 0 || (nSiteInDvcCnt == 0 && nTableBufferInDvcCnt == 0 && g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetSocketOnCntGrr() > 0))
			{
				ChangeSubState(eExcutingStepGrr_Check_Job);
				break;
 			}


			////////////////////////////////////////////////////
			//Job 변경

			++m_nJobAtsIdx;

			int nFindJobIdx = m_nJobAtsIdx;
			for (; nFindJobIdx < (int)m_vModuleSetPressIdx.size(); nFindJobIdx++) 
			{
				if (g_TaskPressUnit[m_vModuleSetPressIdx[nFindJobIdx]].GetSocketOnCntGrr() > 0)
				{				
					break;
				}
			}
			m_nJobAtsIdx = nFindJobIdx;

			if (m_nJobAtsIdx > eATS_DN_STATION_L4 || m_nJobAtsIdx < eATS_DN_STATION_L1)
				m_nJobAtsIdx = eATS_DN_STATION_L1;
			g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx, m_nJobAtsIdx);

			MakeLog(_T("[Check More Job][CHANGE JOB INDEX = %d]"), m_nJobAtsIdx);

			int nUnLoadDeviceCnt = g_TaskLoadTbl[m_nUnLoadingTableInx].GetUnloadDeviceCnt();

			int nSetInDvc = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrUnitCount);
			int nModule1On = g_TaskAutoTestPp[eTestPP_1].GetModuleSocketOnCnt(TRUE);
			int nModule2On = g_TaskAutoTestPp[eTestPP_2].GetModuleSocketOnCnt(TRUE);
			if (nModule1On > 0 && nModule2On > 0)
				nSetInDvc /= 2;

 			int nReservedDvcCnt = g_DMCont.m_dmTestPP.GN(NDM04_SITEGROUP1_INSERTED_DEVICE_CNT + m_eTestIdx);
			int nTblPocketX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
			int nTblPocketY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

 			int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

			BOOL bLoadDeviceIf = FALSE;
			//Table Load Device 8개 아니면 True
			bLoadDeviceIf = (g_TaskLoadTbl[m_nLoadingTableInx].GetLoadDeviceCnt() != (nSiteDivX*nSiteDivY));
// 
// 			if (nReservedDvcCnt == nSetInDvc) {
// 				ChangeSubState(eExcutingStepGrr_WaitLoadTable);
// 				break;
// 			}

			if ((nUnLoadDeviceCnt > 0 || nUnLoadDeviceCnt == ((nTblPocketX*nTblPocketY) / 2)))
			{
				// 예약을 해제한다
				//SetSiteGroupReserve(false);
				g_TaskLoadTbl[m_nUnLoadingTableInx].SetWorkReserve(RID_INIT_IDLE_RESERVE);
				ChangeSubState(eExcutingStepGrr_WaitLoadTable);
				break;
			}
			else if ((nReservedDvcCnt < nSetInDvc) && (g_TaskLoadTbl[m_nLoadingTableInx].GetLoadDeviceCnt() == 0))
			{
				g_TaskLoadTbl[m_nLoadingTableInx].SetWorkReserve(RID_INIT_IDLE_RESERVE);
				ChangeSubState(eExcutingStepGrr_WaitLoadTable);
				break;
			}
			else if (nReservedDvcCnt < nSetInDvc && bLoadDeviceIf/*g_TaskLoadTbl[m_eTestIdx].GetLoadDeviceCnt() != ((nShtPocketX*nShtPocketY)/2)*/)
			{
				g_TaskLoadTbl[m_eTestIdx].SetWorkReserve(RID_INIT_IDLE_RESERVE);
				ChangeSubState(eExcutingStepGrr_WaitLoadTable);
				break;
			}
			int nTblLdDvcCnt = g_TaskLoadTbl[m_nLoadingTableInx].GetLoadDeviceCnt();
			int nTblUldDvcCnt = g_TaskLoadTbl[m_nUnLoadingTableInx].GetUnloadDeviceCnt();

			int station_device_cnt = g_DMCont.m_dmDevice.GetDeviceCount(eDeviceDataStage::eDeviceStage_TEST_SITE_1 + m_vModuleSetPressIdx[m_nJobAtsIdx]);

			int nChkSiteInDvc = 0;

			if (m_nUnLoadingTableInx == eLoadTbl_1 || m_nUnLoadingTableInx == eLoadTbl_4)
			{
				for (auto mdIdx : m_vModuleSetPressIdx)
					nChkSiteInDvc += g_TaskPressUnit[mdIdx].ChkSiteInDvc();
			}

			int nMotorLoadPos = g_TaskLoadTbl[m_nLoadingTableInx].ChkMotorPos(LOC_IDX_LD_TBL_TEST_SIDE_1 + m_nJobAtsIdx, CTaskLoadTable::eAxisY);
			if (g_TaskLoadTbl[m_nLoadingTableInx].GetWorkReserve() == RID_TEST_PP_(m_eTestIdx) && nMotorLoadPos == ERR_NO_ERROR &&nTblLdDvcCnt != 8 && g_TaskLoadTbl[m_nLoadingTableInx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready /*&& m_nBeforeStation > 0*/) {
				if (nReservedDvcCnt != nSetInDvc) {
					g_TaskLoadTbl[m_nLoadingTableInx].SetWorkReserve(RID_INIT_IDLE_RESERVE);
				}
			}

			int nMotorUnloadPos = g_TaskLoadTbl[m_nUnLoadingTableInx].ChkMotorPos(LOC_IDX_LD_TBL_TEST_SIDE_1 + m_nJobAtsIdx, CTaskLoadTable::eAxisY);
			if (g_TaskLoadTbl[m_nUnLoadingTableInx].GetWorkReserve() == RID_TEST_PP_(m_eTestIdx) && nMotorUnloadPos == ERR_NO_ERROR && nTblUldDvcCnt > 0 && g_TaskLoadTbl[m_nUnLoadingTableInx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready
				|| g_TaskAllAutoStacker.GetLoadEnableStackerCount() == 0 && nChkSiteInDvc > 0)
			{
				if (g_TaskLoadTbl[m_nUnLoadingTableInx].GetUnloadDeviceCnt() != 0)
					g_TaskLoadTbl[m_nUnLoadingTableInx].SetWorkReserve(RID_INIT_IDLE_RESERVE);
			}



			ChangeSubState(/*eExcutingStepGrr_Check_Job*/eExcutingStepGrr_WaitLoadTable);
			//}
			
		}break;
		case eExcutingStep_End:
		{
			MakeLog(_T("[GRR eAutoLotState_NormalComplete]"));
			ChangeMainState(eAutoLotState_NormalComplete);
		}break;

	 }
}

void CTaskAutoTestPp::AutoState_Execute_AutoLoop()
{

}

void CTaskAutoTestPp::AutoState_Execute_AutoAudit()
{

}

void CTaskAutoTestPp::AutoState_Execute_QaSample()
{

}

void CTaskAutoTestPp::AutoState_NormalComplete()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 6000:
		{
			g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_WORKING_IDX+m_eTestIdx, -1);	
			ChangeSubState(6001);
		}break;
	case 6001:
		{
			//ChangeMainState( eAutoLotState_Idle );
		}break;
	default :
		ChangeSubState(6000);
		break;
	}
}

void CTaskAutoTestPp::AutoState_AlarmStop()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		//MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
		MakeLog(_T("[MainStep=%s] [SubStep=%d] [last step=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nLast_Excuting_Step);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 7000:
		{
			g_TaskTestPP[ m_eTestIdx ].SetProcessCommand( base_seq_cmd::eProcCmd_Pause );
			ChangeSubState(7001);
		}break;
	case 7001:
		{
		}break;
	default :
		ChangeSubState(7000);
		break;
	}
}

void CTaskAutoTestPp::AutoState_Resume()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d] [last step=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep, m_nLast_Excuting_Step);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 8000:
		{
			g_TaskTestPP[ m_eTestIdx ].SetProcessCommand( base_seq_cmd::eProcCmd_Resume );
			ChangeSubState(8001);
		}break;
	case 8001:
		{
			if( g_TaskTestPP[ m_eTestIdx ].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready )
				break;
			else
			{
				BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
				if (GetModuleSocketOnCnt(bAutoGrrUse) > 0)
				{
					std::vector<CPoint> vTempPkr;
					vTempPkr.push_back(CPoint(0, 0));
					OneAxis* pMotorX1 = g_TaskTestPP[m_eTestIdx].GetAxis(CTaskTestPP::eAxisX1);
					OneAxis* pMotorX2 = g_TaskTestPP[m_eTestIdx].GetAxis(CTaskTestPP::eAxisX2);
					double dCurPosX1 = 0;
					pMotorX1->GetCurrentPos(dCurPosX1);
					double dCurPosX2 = 0;
					pMotorX2->GetCurrentPos(dCurPosX2);

					double dTPosHome1[CTaskTestPP::eAxisMax] = { 0, };
					g_TaskTestPP[m_eTestIdx].GetTeachPos(LOC_IDX_TEST_PP_HOME, CPoint(0, 0), vTempPkr, CTaskTestPP::eZPOS_SAFETY, dTPosHome1, eTestPPLoadType);

					double dTPosHome2[CTaskTestPP::eAxisMax] = { 0, };
					g_TaskTestPP[m_eTestIdx].GetTeachPos(LOC_IDX_TEST_PP_HOME, CPoint(0, 0), vTempPkr, CTaskTestPP::eZPOS_SAFETY, dTPosHome2, eTestPPUnLoadType);

					int nSafetyX1Sen = g_TaskTestPP[m_eTestIdx].ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON);
					int nSafetyX2Sen = g_TaskTestPP[m_eTestIdx].ChkDI(CTaskTestPP::xX2AXIS_POSITION_SEN, DEF_ON);
					// home 위치보다 크거나 안전 센서가 감지 안되면 Y축 Home
					if (/*dTPosHome1[CTaskTestPP::eAxisX1] + 1000.0 < dCurPosX1 ||*/ nSafetyX1Sen != ERR_NO_ERROR ||
						/*dTPosHome2[CTaskTestPP::eAxisX2] + 1000.0 < dCurPosX2 ||*/  nSafetyX2Sen != ERR_NO_ERROR)
					{
						std::vector<CPoint> vPkr; vPkr.push_back(CPoint(0, 0));
						CTestPPCmd_MoveAlreadyPos* pSeqCmd = new CTestPPCmd_MoveAlreadyPos;
						pSeqCmd->m_nCmdStage = LOC_IDX_TEST_PP_HOME;
						pSeqCmd->m_cCmdPocket = CPoint(0, 0);
						pSeqCmd->m_vCmdPicker = vPkr;
						MakeLog(_T("[ Resume = Move Already Load Table position"));

						if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) == eSeqCmdAck_OK)
						{
							ChangeSubState(8002);
							break;
						}							
					}
				}
			}

			ChangeSubState(8003);
		}break;
	case 8002:
		{
			if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				std::vector<CPoint> vTempPkr;
				vTempPkr.push_back(CPoint(0, 0));
				OneAxis* pMotorX1 = g_TaskTestPP[m_eTestIdx].GetAxis(CTaskTestPP::eAxisX1);
				OneAxis* pMotorX2 = g_TaskTestPP[m_eTestIdx].GetAxis(CTaskTestPP::eAxisX2);
				double dCurPosX1 = 0;
				pMotorX1->GetCurrentPos(dCurPosX1);
				double dCurPosX2 = 0;
				pMotorX2->GetCurrentPos(dCurPosX2);

				double dTPosHome1[CTaskTestPP::eAxisMax] = { 0, };
				g_TaskTestPP[m_eTestIdx].GetTeachPos(LOC_IDX_TEST_PP_HOME, CPoint(0, 0), vTempPkr, CTaskTestPP::eZPOS_SAFETY, dTPosHome1, eTestPPLoadType);
				double dTPosHome2[CTaskTestPP::eAxisMax] = { 0, };
				g_TaskTestPP[m_eTestIdx].GetTeachPos(LOC_IDX_TEST_PP_HOME, CPoint(0, 0), vTempPkr, CTaskTestPP::eZPOS_SAFETY, dTPosHome2, eTestPPUnLoadType);
				int nSafetyX1Sen = g_TaskTestPP[m_eTestIdx].ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON);
				int nSafetyX2Sen = g_TaskTestPP[m_eTestIdx].ChkDI(CTaskTestPP::xX2AXIS_POSITION_SEN, DEF_ON);

				if (dTPosHome1[CTaskTestPP::eAxisX1] + 1000.0 < dCurPosX1 || nSafetyX1Sen != ERR_NO_ERROR ||
					dTPosHome2[CTaskTestPP::eAxisX2] + 1000.0 < dCurPosX2 || nSafetyX2Sen != ERR_NO_ERROR )
					ChangeSubState(8001);
				else
					ChangeSubState(8003);
			}
				
		}break;
	case 8003:
		{		
			BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
			if (bAutoGrrUse == DEF_OFF)
			{
				// 처음부터 다시 시작하도록 해야한다. (onecycle -skip || cleanout - skip || endfeed - skip )
				if (g_DMCont.m_dmTestPP.GN(NDM04_TESTHAND1_WORKING_IDX + m_eTestIdx) == -1) // 저장 된 마지막 idx가 -1이면 모든 step
				{
					for (int i = 0; i < eMaxATS_Area; i++)
						g_testpp_work_area[m_eTestIdx].release_work_area(m_vModuleSetPressIdx[i], this);
					MakeLog(_T("[==== Resume FindJob release_work_area =====]"));
					m_nJobAtsIdx = 0;
					m_nLast_Excuting_Step = 0;
				}
			}
			

			//ChangeMainState(eAutoLotState_Executing, m_nLast_Excuting_Step);
			int nEndFeed = g_TaskAllAutoStacker.GetTrayEndFeed();
			switch( nEndFeed )
			{
				case DEF_LOTEND_TRAYEND:
				case DEF_LOTEND_TRAYFEED:
					{
						ChangeMainState( eAutoLotState_CleanOut );
					}break;
				default: ChangeMainState( eAutoLotState_Executing, m_nLast_Excuting_Step ); break;
			}
		}break;
	default :
		ChangeSubState(8000);
		break;
	}
}

void CTaskAutoTestPp::AutoState_CleanOut()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
		case 9000:
			{
				char szCompany[16] = { 0, };
				g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

				if (ModuleUseAbleCheck() != TRUE && strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
					ChangeSubState(9999);
					break;
				}

				CTestPPCmd_MoveAlreadyPos* pSeqCmd = new CTestPPCmd_MoveAlreadyPos;
				pSeqCmd->m_nCmdStage = LOC_IDX_STATION_4;
				pSeqCmd->m_cCmdPocket = CPoint(0, 0);
				pSeqCmd->m_vCmdPicker.push_back(CPoint(0, 0));
				pSeqCmd->m_nHandType = TEST_LD_HAND;
				if (SendCommand(g_TaskTestPP[m_eTestIdx], pSeqCmd) == eSeqCmdAck_OK)
					ChangeSubState( 9999 );
			}break;
		case 9999:
			{
				if (g_TaskTestPP[m_eTestIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
					break;

				// LotEnd시 Blow를 꺼준다. [9/16/2020 donghyun.shin]
				g_TaskTestPP[m_eTestIdx].do_VacBlow_All(eVacBlwSt_Off, eTestPPLoadType);
				g_TaskTestPP[m_eTestIdx].do_VacBlow_All(eVacBlwSt_Off, eTestPPUnLoadType);


				//Barcode LED OFF
// 				if (m_eTestIdx == eTestPP_1) {
// 					g_pVisionTestHand_1->Cmd_ReqMeasure_BarcodeLED(eBarcodeLight_OFF, m_eTestIdx);
// 				}
// 				else {
// 					g_pVisionTestHand_2->Cmd_ReqMeasure_BarcodeLED(eBarcodeLight_OFF, m_eTestIdx);
// 				}


				ChangeMainState( eAutoLotState_NormalComplete );
			}break;
		default:
			ChangeSubState( 9000 );
			break;
	}
}

int CTaskAutoTestPp::ChkInterlockAts( int nAtsIdx )
{
	if( nAtsIdx >= eATS_DN_STATION_L1 && nAtsIdx <= eATS_DN_STATION_L4)
	{
		if( g_TaskPressUnit[ m_vModuleSetPressIdx[nAtsIdx] ].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready )
			return eExcutingStep_ErrorJob;

		//int nChkMotSafety  = g_TaskPressUnit[ m_vModuleSetPressIdx[nAtsIdx] ].ChkMotorPos(LOC_IDX_PRESS_UNIT_SAFETY, CTaskPressUnit::eAxisZ1);
		//if( nChkMotSafety != ERR_NO_ERROR )
		int nSafetySen = g_TaskPressUnit[m_vModuleSetPressIdx[nAtsIdx]].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON);
		if(nSafetySen != ERR_NO_ERROR)		
			return eExcutingStep_ErrorJob;
	}else
	{
		return eExcutingStep_ErrorJob;
	}

	return ERR_NO_ERROR;
}

int CTaskAutoTestPp::GetDvcStationHandAssign(BOOL bPickPlace, int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nWorkingAreaIdx, BOOL bChkUD_RetestBin)
{
	int nPkrPossibleCnt = 0;

	if( bPickPlace == DEF_PICK )
	{
		int nSocketDvc[STATION_MAX_X_SITE][STATION_MAX_Y_SITE];
		memset( nSocketDvc, DEF_NOT_EXIST, sizeof(nSocketDvc) );
	
		BOOL bChkCleanDev = GetDvcStationCleanDevAssign(bPickPlace, nWorkingAreaIdx);
		
		ST_DD_DEVICE stDvcHand;
		// unload picker 정보
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{
				stDvcHand.clear();
				if(bChkCleanDev == TRUE) // Clean Device가 Station에 Pick 할수 있는 상태면, Load Hand로 작업 한다.
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY, &stDvcHand);
				else
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY + TEST_UD_PKR_Y, &stDvcHand);

				if (stDvcHand.sExist == DEF_NOT_EXIST)
				{
					nPickerDvcPattern[nX][nY] = DEF_NOT_EXIST;	// 사용가능
					nPkrPossibleCnt++;
				}
			}
		}
		// AAB 처리를 여기서 해야 됨


	}else{
		BOOL bAutoRetetsMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);

		ST_DD_DEVICE stDvcHand;
		// unload picker 정보
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{
				stDvcHand.clear();
				if (bAutoRetetsMode == eOPTION_USE && bChkUD_RetestBin == TRUE)
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY + TEST_UD_PKR_Y, &stDvcHand);
				else
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY, &stDvcHand);
				// Hand Device 가 있어야 한다.

				if (stDvcHand.sExist == DEF_EXIST)
				{
					if (stDvcHand.nTestingCnt >= eARB_3stTestEnd) {
						MakeLog("Test PP %d [%d-%d] Picker 3st end device", m_eTestIdx, nX + 1, nY + 1);
						continue;
					}
					if (bAutoRetetsMode == eOPTION_USE && atoi(stDvcHand.szTestSiteNum) == m_vModuleSetPressIdx[nWorkingAreaIdx] && stDvcHand.nTestingCnt == eARB_2stTestEnd)
					{
						MakeLog("Test PP %d [%d-%d] Picker have same station retest B device", m_eTestIdx + 1, nX + 1, nY + 1);
						continue;
					}

					if (strcmp(stDvcHand.szErrString, DEF_2D_READ_ERR_DEVICE) == 0 || strcmp(stDvcHand.szErrString, DEF_TEST_RESULT_WRONG_APN) == 0)
					{
						//MakeLog("Test PP %d [%d-%d] Picker have same station retest B device", m_eTestIdx, nX + 1, TEST_LD_PKR_Y + 1);
						continue;
					}

					if (m_nCleanMode == TRUE && strcmp(stDvcHand.szBinResult, DEF_SOCKET_CLEAN_DEVICE) != 0)
						continue;

					if (m_nCleanMode != TRUE && strcmp(stDvcHand.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0)
						continue;

					BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stDvcHand);

					if (bAutoRetetsMode == eOPTION_USE && stDvcHand.nTestingCnt == eARB_1stTestEnd && stDvcHand.nBinColor == eFailBin && strcmp(stDvcHand.szErrString, DEF_TEST_RESULT_TP101) != 0 && bIsDefaultBin != TRUE
						&& (atoi(stDvcHand.szTestSiteNum) != m_vModuleSetPressIdx[nWorkingAreaIdx] || nY != stDvcHand.cSubSiteNum.y))
						continue;

					if (bAutoRetetsMode == eOPTION_USE && stDvcHand.nBinColor == eFailBin && (strcmp(stDvcHand.szErrString, DEF_TEST_RESULT_TP101) == 0 || bIsDefaultBin == TRUE)
						&& (atoi(stDvcHand.szTestSiteNum) == m_vModuleSetPressIdx[nWorkingAreaIdx]))
						continue;

					int nOnOff = g_TaskPressUnit[m_vModuleSetPressIdx[nWorkingAreaIdx]].GetSocketOnOff(nX, nY);
					if (nOnOff == DEF_SITE_ON)
					{
						if (bChkUD_RetestBin == TRUE)
						{
							if (bAutoRetetsMode == eOPTION_USE && stDvcHand.nBinColor == eFailBin && stDvcHand.nTestingCnt == eARB_1stTestEnd)
							{
								nPickerDvcPattern[nX][nY] = DEF_EXIST;	// 사용가능
								nPkrPossibleCnt++;
							}
						}
						else {
							nPickerDvcPattern[nX][nY] = DEF_EXIST;	// 사용가능
							nPkrPossibleCnt++;
						}
					}
				}				
			}
		}
	}

	return nPkrPossibleCnt;
}

/*=============================================================================================
  DESCRIPT : FillDvc_VirtualStation 
  KEY WORD : Station 의 device 정보
  ARGUMENT : nDvcStageIdx       = device stage 정보
             pnBuffer           = station site device 정보
			 nBufSizeX          = Buffer size max X
			 nBufSizeY          = Buffer size max Y
			 bPickPlace         = Pick / Place 동작
  RETURN   : 없음
 ===============================================================================================*/
int CTaskAutoTestPp::FillDvc_VirtualStation(int nDvcStageIdx, int* pnBuffer, int nBufSizeX, int nBufSizeY, int bPickPlace , BOOL bExchFlag)
{
	int nPocketCount_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nPocketCount_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	BOOL bModeAutoRetest = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
	BOOL bAtoAPickPlace = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_ART_TEST_HAND_PICK_PLACE);

	int nCleanMode = ChkCleaningMode(m_nJobAtsIdx);

	if (nCleanMode == TRUE)
	{
		bAtoAPickPlace = FALSE;
	}

	ST_DD_DEVICE stSite;
	for(int nY=0; nY<nPocketCount_Y; nY++)
	{
		for (int nX=0; nX<nPocketCount_X; nX++)
		{
			stSite.clear();
			int nBuffer_Index = (nX+VIRTUAL_POCKET_X_START)*nBufSizeY/*X*/ + (nY+VIRTUAL_POCKET_Y_START)/*Y*/;
			g_DMCont.m_dmDevice.GetDeviceData(nDvcStageIdx, nX, nY, &stSite);

			if ( stSite.sExist == DEF_EXIST )
				pnBuffer[nBuffer_Index] = DEF_EXIST;			
			else
				pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;

			if ( bPickPlace == DEF_PICK && stSite.sExist == DEF_EXIST )
			{
				if (stSite.nTestingInfo == eTestSite_DvcLoading || stSite.nTestingInfo == eTestSite_CleanLoading)
					pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;

				// Manual Socket Cleanning 스케쥴이 걸리면 AutoRetest AA Pick/Place Mode가 아닐 때는 A나 AA 자재는 잠시 빼줘야 된다. [9/6/2021 donghyun.shin]
				if(nCleanMode == TRUE && stSite.nTestingInfo == eTestSite_DvcLoading && bAtoAPickPlace != eOPTION_USE)
					pnBuffer[nBuffer_Index] = DEF_EXIST;

				// aab retest a to a일 경우 pick/place 기능 옵션에 따라 집을지 말지 결정.
				if (g_TaskSystemCtrl.GetAutoGrrUse() == FALSE)
				{
					BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stSite);

					if (bModeAutoRetest == eOPTION_USE && (bAtoAPickPlace != eOPTION_USE && m_nCleanMode != TRUE) && stSite.nBinColor == eFailBin && stSite.nTestingCnt == eARB_1stTestEnd && bIsDefaultBin != TRUE)
					{	// 집어 갈 수 없게 만든다.
						pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
						stSite.nTestingInfo = eTestSite_DvcLoading;
						g_DMCont.m_dmDevice.SetDeviceData(nDvcStageIdx, nX, nY, &stSite);
					}
				}
				
			}
			if (bPickPlace == DEF_PLACE && stSite.sExist == DEF_EXIST)
			{
				// exchange 할 경우에 pocket에 device가 있더라도 없는 것 처럼 인식해야한다.
				if ( bExchFlag == TRUE )
				{
					BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stSite);
					//exchange 시 AA Retest 들어가야 될경우 Place를 못한다
					if (bModeAutoRetest == eOPTION_USE && bAtoAPickPlace == eOPTION_USE && m_nCleanMode != TRUE
						&& stSite.nBinColor == eFailBin && stSite.nTestingCnt == eARB_1stTestEnd && strcmp(stSite.szErrString, DEF_TEST_RESULT_TP101) != 0 && bIsDefaultBin != TRUE)
					{
						pnBuffer[nBuffer_Index] = DEF_EXIST;
					}
					else {
						pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
					}
				}
			}			
		}
	}

	return ERR_NO_ERROR;
}


int CTaskAutoTestPp::GetAreaProblemStation(int nAtsIdx, int nPocketX, int nPocketY, int nArrHandWork[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nPitchType_X, int nPitchType_Y)
{
	//0번 피커부터 할당되지 않은 포켓은 트림한다.
	int nPockOffsetX = 0;
	int nPockOffsetY = 1;

	//X방향 첫 할당 위치가 X옵셋
	for(; nPockOffsetX<MAX_TEST_PP_PKR_CNT_X; nPockOffsetX++)
	{
		if(nArrHandWork[nPockOffsetX][0]!=DEF_WORK_FREE) break;
		if(nArrHandWork[nPockOffsetX][1]!=DEF_WORK_FREE) break;
	}
	//X방향 첫 할당 위치가 1개라도 있으면 Y옵셋은 0
	for(int nX=0; nX<MAX_TEST_PP_PKR_CNT_X; nX++)
	{
		if(nArrHandWork[nX][0]!=DEF_WORK_FREE) {
			nPockOffsetY = 0;
			break;
		}
	}

	int n_Xpos = nPocketX + nPockOffsetX*nPitchType_X;
	int n_YPos = nPocketY + nPockOffsetY*nPitchType_Y;

	CPoint Pocket = CPoint(n_Xpos, n_YPos);
	std::vector<CPoint> vPicker;
	for(int nPkrY=0; nPkrY < MAX_TEST_PP_PKR_CNT_Y; nPkrY++)
	{
		for(int nPkrX=0; nPkrX < MAX_TEST_PP_PKR_CNT_X; nPkrX++)
		{
			if( nArrHandWork[nPkrX][nPkrY] == DEF_WORK_RESERVED )
			vPicker.push_back( CPoint(nPkrX, nPkrY));
		}
	}
	double dTpos[CTaskTestPP::eAxisMax] = {0,};
	g_TaskTestPP[m_eTestIdx].GetTeachPos(LOC_IDX_STATION_1+nAtsIdx, Pocket, vPicker, CTaskTestPP::eZPOS_SAFETY, dTpos, eTestPPLoadType);


	std::vector<CPoint> vPicker_RL1, vPicker_FR4;
	vPicker_RL1.push_back( CPoint(0,0) );
	vPicker_FR4.push_back( CPoint( MAX_TEST_PP_PKR_CNT_X-1, MAX_TEST_PP_PKR_CNT_Y-1 ) );
	int nSitePocketCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSitePocketCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nSitePitchX     = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Pitch_X);
	int nSitePitchY     = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Pitch_Y);
	double dTRect_RL[CTaskTestPP::eAxisMax]   = {0,};
	double dTRect_FR[CTaskTestPP::eAxisMax]   = {0,};
	// Get rear left & front right rect position
	g_TaskTestPP[m_eTestIdx].GetTeachPos(LOC_IDX_STATION_1+nAtsIdx, CPoint(0,0), vPicker_FR4, CTaskTestPP::eZPOS_PICK  , dTRect_RL, eTestPPLoadType);
	g_TaskTestPP[m_eTestIdx].GetTeachPos(LOC_IDX_STATION_1+nAtsIdx, CPoint(nSitePocketCntX-1,nSitePocketCntY-1), vPicker_RL1, CTaskTestPP::eZPOS_PICK  , dTRect_FR, eTestPPLoadType);

	double dStationCenter = (dTRect_FR[CTaskTestPP::eAxisY] + dTRect_RL[CTaskTestPP::eAxisY]) / 2.0;
	//if( dStationCenter + STATION_CENTER_TO_LR_HW_DIST >= dTpos[CTaskTestPP::eAxisY] && dStationCenter - STATION_CENTER_TO_LR_HW_DIST <= dTpos[CTaskTestPP::eAxisY] )
	if(fabs(dStationCenter /*+ STATION_CENTER_TO_LR_HW_DIST*/-dTpos[CTaskTestPP::eAxisY]) <= 2000.0/*500.0 */)
		return FALSE;
	
	return TRUE;

}

/*=============================================================================================
  DESCRIPT : FindPickPlaceFromTable
  KEY WORD : loadtable, ART buffer table, Socket Cleaning Table 의 작업
  ARGUMENT : nLocIdx            = 작업 할 Loc Index
             nTableDvc          = Table의 device 정보
             nPickerDvcPattern  = hand 작업 가능 패턴
			 bPickPlace         = pick / place 동작
			 n_Xpos             = 찾은 작업 X 위치
			 n_Ypos             = 찾은 작업 Y 위치
			 nWorkPkr           = 찾은 작업 picker pattern
  RETURN   : 작업 가능 수
 ===============================================================================================*/
int CTaskAutoTestPp::FindPickPlaceFromStation( int nAtsIdx, int nSiteDvc[VIRTUAL_POCKET_X_MAX][VIRTUAL_POCKET_Y_MAX], int nPickerPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], BOOL bPickPlace, int *n_Xpos, int *n_YPos, int nWorkPkr[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] )
{
	int nSearchY = 0, nSearchX = 0;
	int nSearchHandY = 0, nSearchHandX = 0;

	/*DEF_WORK_FREE or DEF_WORK_RESERVED*/
	int nArrHandWorkSource[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = {DEF_WORK_FREE,};
	int nArrHandWorkCopy[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y]   = {DEF_WORK_FREE,};

	int nPocketCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nPocketCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
		
	int nTotalPossibleCnt = 0;	// 가장 Pick 할 수 있는 수량
	int nCurrentCnt		  = 0;	// 현재 까지 Pick 할수 있는 수량
	int nCXPosition	= 0, nCYPosition = 0;
	// 1Y, 2Y는 계산 한 대로 가져 온다.	

	int nPitchType_X  = g_TaskTestPP[m_eTestIdx].GetHandPitchType(CTaskTestPP::eAxisPitch_HW_X1_SW_Y1, LOC_IDX_STATION_1);	// Hand와 Target간의 간격 관계 (0=1by1, 1=1:1매칭, 2=한칸뛰기,3=두칸뛰기), Hand 기준임. (Hand 1,3으로 Target 1,2 집는것은 처리 필요)
	int nPitchType_Y  = g_TaskTestPP[m_eTestIdx].GetHandPitchType(CTaskTestPP::eAxisPitch_HW_Y1_SW_X1, LOC_IDX_STATION_1);	// Hand와 Target간의 간격 관계 (0=1by1, 1=1:1매칭, 2=한칸뛰기,3=두칸뛰기), Hand 기준임. (Hand 1,3으로 Target 1,2 집는것은 처리 필요)

	int nSiteState   = DEF_NOT_EXIST;
	int nHandState	 = DEF_NOT_EXIST;
	if(	bPickPlace == DEF_PICK ){	// Pick 일 때 조건
		nSiteState = DEF_EXIST;
		nHandState = DEF_NOT_EXIST;
	}else{							// Place 일 때 조건		
		nSiteState = DEF_NOT_EXIST;
		nHandState = DEF_EXIST;
	}

	//for(nSearchY = VIRTUAL_POCKET_Y_START - ( MAX_TEST_PP_PKR_CNT_Y - 1 ) * nPitchType_Y; nSearchY < VIRTUAL_POCKET_Y_START + nPocketCntY; nSearchY++)
	nSearchY = VIRTUAL_POCKET_Y_START;
	{
		nSearchX = VIRTUAL_POCKET_X_START;
		//for(nSearchX = VIRTUAL_POCKET_X_START - ( MAX_TEST_PP_PKR_CNT_X - 1 ) * nPitchType_X; nSearchX < VIRTUAL_POCKET_X_START + nPocketCntX; nSearchX++)
		{
			nCurrentCnt = 0;

			memset(nArrHandWorkSource, 0, sizeof(nArrHandWorkSource));

			for (nSearchHandY = MAX_TEST_PP_PKR_CNT_Y - 1; nSearchHandY >= 0; nSearchHandY--)
			//nSearchHandY = (bPickPlace == DEF_PICK ? TEST_UD_PKR_Y : TEST_LD_PKR_Y );
			{
				for(nSearchHandX = 0; nSearchHandX < MAX_TEST_PP_PKR_CNT_X; nSearchHandX++)
				{
					int nYPocket = 0, nXPocket = 0;
					nXPocket = nSearchX + nSearchHandX * nPitchType_X;
					nYPocket = nSearchY + nSearchHandY * nPitchType_Y;
				

					if(     nSiteDvc[ nXPocket][ nYPocket  ]		==	nSiteState
						&&  nPickerPattern[nSearchHandX][nSearchHandY]	== 	nHandState)
					{
						nArrHandWorkSource[nSearchHandX][nSearchHandY] = DEF_WORK_RESERVED;
						nCurrentCnt++;
					}
				}
			}
			
			//hand의 영역이 가능한 부분인지 점검한다. x축만 보면된다.
			BOOL bLimitCheck = GetAreaProblemStation(nAtsIdx, nSearchX-VIRTUAL_POCKET_X_START, nSearchY-VIRTUAL_POCKET_Y_START, nArrHandWorkSource, nPitchType_X, nPitchType_Y); // todo slt : 영역 점검을 위한 함수 구현 필요.
			if(bLimitCheck == FALSE )
			{
				if( nCurrentCnt > nTotalPossibleCnt )
				{
					//작업해라.
					nTotalPossibleCnt = nCurrentCnt;
					memcpy_s(nArrHandWorkCopy, sizeof(nArrHandWorkCopy), nArrHandWorkSource, sizeof(nArrHandWorkSource));
					nCXPosition = nSearchX;
					nCYPosition = nSearchY;
				}
			}
		}
	}

	memcpy_s(nWorkPkr, sizeof(nArrHandWorkCopy), nArrHandWorkCopy, sizeof(nArrHandWorkCopy));

	//0번 피커부터 할당되지 않은 포켓은 트림한다.
	int nPockOffsetX = 0;
	int nPockOffsetY = 1;

	//X방향 첫 할당 위치가 X옵셋
	for(; nPockOffsetX<MAX_TEST_PP_PKR_CNT_X; nPockOffsetX++)
	{
		if(nWorkPkr[nPockOffsetX][0]!=DEF_WORK_FREE) break;
		if(nWorkPkr[nPockOffsetX][1]!=DEF_WORK_FREE) break;
	}
	//X방향 첫 할당 위치가 1개라도 있으면 Y옵셋은 0
	for(int nX=0; nX<MAX_TEST_PP_PKR_CNT_X; nX++)
	{
		if(nWorkPkr[nX][0]!=DEF_WORK_FREE) {
			nPockOffsetY = 0;
			break;
		}
	}

	nCXPosition -= VIRTUAL_POCKET_X_START;
	nCYPosition -= VIRTUAL_POCKET_Y_START;

	*n_Xpos = nCXPosition + nPockOffsetX*nPitchType_X;
	*n_YPos = nCYPosition + nPockOffsetY*nPitchType_Y;

	return nTotalPossibleCnt;
}

int CTaskAutoTestPp::CheckJob_Station(int nAtsIdx, BOOL bPickPlace, CPoint &vPocket, std::vector<CPoint>&vPicker, BOOL bRetestPlace)
{
	CString strErrMsg = _T("");
	std::vector<CPoint> vErrPicker; 	vErrPicker.clear();
	std::vector<CString> vErrstring; vErrstring.clear();
	int nHandType = 0;
	int nErr = ChkDevice(strErrMsg, nHandType, vErrstring);
	if (nErr != ERR_NO_ERROR) {
		g_TaskSystemCtrl.SetErrTestPpIdx(m_eTestIdx, 1);
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Test PP %d %s", m_eTestIdx + 1, strErrMsg);
		for (int i = 0; i < (int)vErrstring.size(); i++)
		{
			AlarmData.m_fnAddItemFormat("%s", vErrstring[i]);
		}
		g_Error.AlarmReport(nErr, AlarmData, &g_TaskSystemCtrl,g_TaskTestPP[m_eTestIdx].GetFailPicker(nHandType));
		g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
		return eExcutingStep_ErrorJob;
	}

	int nChkInterlock = ChkInterlockAts(nAtsIdx);
	if (nChkInterlock == eExcutingStep_ErrorJob)
		return eExcutingStep_ErrorJob;

	if (LOC_IDX_STATION_1 + nAtsIdx < LOC_IDX_STATION_1 && LOC_IDX_STATION_1 + nAtsIdx > eATS_DN_STATION_L4)
		return eExcutingStep_ErrorJob;

	vPicker.clear();

	int nSiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	ST_DD_DEVICE stDvc;
	int nDevCnt = 0;
	for (int nX = 0; nX < nSiteCntX; nX++)
	{
		for (int nY = 0; nY < nSiteCntY; nY++)
		{
			int nOnOff = g_TaskPressUnit[m_vModuleSetPressIdx[nAtsIdx]].GetSocketOnOff(nX, nY);
			if (nOnOff == DEF_SITE_ON)
			{
				stDvc.clear();
				g_TaskPressUnit[m_vModuleSetPressIdx[nAtsIdx]].GetSiteDeviceData(&stDvc, nX, nY);
				if (stDvc.sExist == DEF_EXIST)
					nDevCnt++;
			}
		}
	}
	//Clean Device Mode인대 일반 Device가 Station에 있으면 내려놓지 말자
	BOOL bChkCleanDev = GetDvcStationCleanDevAssign(DEF_PLACE, nAtsIdx);
	if(bChkCleanDev == TRUE && nDevCnt > 0 && (bRetestPlace == TRUE || bPickPlace == DEF_PLACE)) 
		return eExcutingStep_ErrorJob;

	int nPkrDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = { 0, };
	int nArrVirtualStation[VIRTUAL_POCKET_X_MAX][VIRTUAL_POCKET_Y_MAX] = { 0, };

	int nPocketCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	// find available hand picker
	for (int nPkrY = 0; nPkrY < MAX_TEST_PP_PKR_CNT_Y; nPkrY++)
	{
		for (int nPkrX = 0; nPkrX < MAX_TEST_PP_PKR_CNT_X; nPkrX++)
		{
			nPkrDvcPattern[nPkrX][nPkrY] = (bPickPlace == DEF_PICK ? DEF_EXIST : DEF_NOT_EXIST);
		}
	}

	BOOL bArtBinCheck = FALSE;

	if (bPickPlace == DEF_PLACE && bRetestPlace == TRUE)
	{
		ST_DD_DEVICE stPkrDvc;
		for (int nPkrX = 0; nPkrX < MAX_TEST_PP_PKR_CNT_X && bArtBinCheck == FALSE; nPkrX++)
		{
			for (int nPkrY = 0; nPkrY < MAX_TEST_PP_PKR_CNT_X && bArtBinCheck == FALSE; nPkrY++)
			{
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nPkrX, nPkrY + TEST_UD_PKR_Y, &stPkrDvc);
				BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stPkrDvc);
				if (stPkrDvc.nBinColor == eFailBin && stPkrDvc.nTestingCnt == eARB_1stTestEnd && strcmp(stPkrDvc.szErrString, DEF_TEST_RESULT_TP101) != 0 && m_nCleanMode != TRUE && bIsDefaultBin != TRUE)
					bArtBinCheck = TRUE;
			}
		}
	}

	int nAvailableTableCount = GetDvcStationHandAssign(bPickPlace, nPkrDvcPattern, nAtsIdx, bArtBinCheck);

	memset(nArrVirtualStation, 0xFF, sizeof(nArrVirtualStation));

	int nUseJobPkr[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = { { 0, }, };
	int nPockX = -1, nPockY = -1;
	int nRetCnt = -1;
	if (nAvailableTableCount > 0) {
		FillDvc_VirtualStation(eDeviceStage_TEST_SITE_1 + m_vModuleSetPressIdx[nAtsIdx], (int*)nArrVirtualStation, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, bPickPlace, (bPickPlace == DEF_EXCH ? TRUE : FALSE));
		nRetCnt = FindPickPlaceFromStation(nAtsIdx, nArrVirtualStation, nPkrDvcPattern, bPickPlace, &nPockX, &nPockY, nUseJobPkr);
		if (nRetCnt > 0){
			std::vector<CPoint> vPkr;
			for (int nPickX = 0; nPickX < MAX_TEST_PP_PKR_CNT_X; nPickX++) {
				for (int nPickY = 0; nPickY < MAX_TEST_PP_PKR_CNT_Y; nPickY++) {
					if (nUseJobPkr[nPickX][nPickY] != 0) {
						vPkr.push_back(CPoint(nPickX, nPickY));
					}
				}
			}

			int nHandCleanDevCnt = 0;
			for (int nPickX = 0; nPickX < MAX_TEST_PP_PKR_CNT_X; nPickX++) {
				for (int nPickY = 0; nPickY < MAX_TEST_PP_PKR_CNT_Y; nPickY++) {
					ST_DD_DEVICE stPkrDvc;
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nPickX, nPickY, &stPkrDvc);
					if (stPkrDvc.sExist == DEF_EXIST && strcmp(stPkrDvc.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0)
						nHandCleanDevCnt++;
				}
			}

			int nSocketDevCnt = 0;	
			int nSocketOnCnt = 0;
			int nPocketCount_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
			int nPocketCount_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

			for (int nX = 0; nX < nPocketCount_X; nX++)
			{
				for (int nY = 0; nY < nPocketCount_Y; nY++)
				{
					ST_DD_DEVICE stDvcSocket;
					int nOnOff = g_TaskPressUnit[m_vModuleSetPressIdx[nAtsIdx]].GetSocketOnOff(nX, nY);
					if (nOnOff == DEF_SITE_ON)
					{
						nSocketOnCnt++;
					}
					g_TaskPressUnit[m_vModuleSetPressIdx[nAtsIdx]].GetSiteDeviceData(&stDvcSocket, nX, nY);
					if (stDvcSocket.sExist == DEF_EXIST && strcmp(stDvcSocket.szBinResult, DEF_SOCKET_CLEAN_DEVICE) != 0)
					{
						nSocketDevCnt++;
					}
				}
			}
			// 
			if (bPickPlace == DEF_PLACE && vPkr.size() == nHandCleanDevCnt && nHandCleanDevCnt == nSocketOnCnt && nSocketDevCnt > 0)
			{
				return eExcutingStep_WaitOtherJob;
			}


			if (vPkr.empty() == false) {
				if (bPickPlace == DEF_PLACE) {
					vPocket = CPoint(nPockX, nPockY);
					vPicker = vPkr;
					return eExcutingStep_FindJob;
				}
				if (bPickPlace == DEF_PICK) {
					vPocket = CPoint(nPockX, nPockY);
					vPicker = vPkr;
					return eExcutingStep_FindJob;
				}
			}
		}
	}
	


	return eExcutingStep_ErrorJob;
}

// int CTaskAutoTestPp::CheckJob_Station(int nAtsIdx, BOOL bPickPlace, CPoint &vPocketPick, CPoint &vPocketPlace, std::vector<CPoint>&vPicker_Pick, std::vector<CPoint>&vPicker_Place)
// {
// 	CString strErrMsg = _T("");
// 	int nErr = ChkDevice(strErrMsg);
// 	if (nErr != ERR_NO_ERROR) {
// 		g_TaskSystemCtrl.SetErrTestPpIdx(m_eTestIdx, 1);
// 		CParamList AlarmData;
// 		AlarmData.m_fnAddItemFormat("Test PP %d %s", m_eTestIdx + 1, strErrMsg);
// 		g_Error.AlarmReport(nErr, AlarmData, &g_TaskSystemCtrl);
// 		g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
// 		return eExcutingStep_ErrorJob;
// 	}
// 
// 	int nChkInterlock = ChkInterlockAts(nAtsIdx);
// 	if( nChkInterlock == eExcutingStep_ErrorJob )
// 		return eExcutingStep_ErrorJob;
// 
// 	if(LOC_IDX_STATION_1 + nAtsIdx < LOC_IDX_STATION_1 && LOC_IDX_STATION_1 + nAtsIdx > eATS_DN_STATION_L4)
// 		return eExcutingStep_ErrorJob;
// 
// 	vPicker_Pick.clear();
// 	vPicker_Place.clear();
// 
// 	int nPkrDvcPattern[2][MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = { 0, };
// 	int nArrVirtualStation[2][VIRTUAL_POCKET_X_MAX][VIRTUAL_POCKET_Y_MAX] = { 0, };
// 
// 	int nPocketCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
// 	
// 	//for(int nRow = 0; nRow < nPocketCntY; nRow ++ )
// 	int nRow = m_nJobSiteRow;
// 	{
// 		// find available hand picker
// 		for( int nPkrY=0; nPkrY < MAX_TEST_PP_PKR_CNT_Y; nPkrY++ )
// 		{
// 			for( int nPkrX=0; nPkrX < MAX_TEST_PP_PKR_CNT_X; nPkrX++ )
// 			{
// 				nPkrDvcPattern[DEF_PICK][nPkrX][nPkrY] = DEF_EXIST;
// 				nPkrDvcPattern[DEF_PLACE][nPkrX][nPkrY] = DEF_NOT_EXIST;
// 			}
// 		}
// 		BOOL bArtBinCheck = FALSE;
// 		int nAvailableCountPick = GetDvcStationHandAssign(DEF_PICK, nPkrDvcPattern[DEF_PICK], nAtsIdx, nRow, bArtBinCheck);
// 		
// 		if (bPickPlace == DEF_PLACE)
// 		{
//  			ST_DD_DEVICE stPkrDvc;
// 			for (int nPkrX = 0; nPkrX < MAX_TEST_PP_PKR_CNT_X && bArtBinCheck==FALSE; nPkrX++)
// 			{
// 				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nPkrX, TEST_UD_PKR_Y, &stPkrDvc);
// 				BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stPkrDvc);
// 				if(stPkrDvc.nBinColor == eFailBin && stPkrDvc.nTestingCnt == eARB_1stTestEnd && strcmp(stPkrDvc.szErrString, DEF_TEST_RESULT_TP101) != 0 && m_nCleanMode != TRUE && bIsDefaultBin != TRUE)
// 					bArtBinCheck = TRUE;
// 			}
// 		}
// 		int nAvailableCountPlace = GetDvcStationHandAssign(DEF_PLACE, nPkrDvcPattern[DEF_PLACE], nAtsIdx, nRow, bArtBinCheck);
// 
// 		memset(nArrVirtualStation, 0xFF, sizeof(nArrVirtualStation));		
// 
// 		int nPockX_Pick = -1, nPockY_Pick = -1, nPockX_Place = -1, nPockY_Place = -1;
// 		int nUseJobPkr_Pick[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = { {0,}, };
// 		int nUseJobPkr_Place[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = { {0,}, };
// 
// 		int nRetCnt_Pick = -1;
// 		if (nAvailableCountPick > 0) {
// 			FillDvc_VirtualStation(eDeviceStage_TEST_SITE_1 + m_vModuleSetPressIdx[nAtsIdx], (int*)nArrVirtualStation[DEF_PICK], VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, DEF_PICK, (bPickPlace == DEF_EXCH ? TRUE :FALSE) );
// 			nRetCnt_Pick = FindPickPlaceFromStation(nAtsIdx, nRow, nArrVirtualStation[DEF_PICK], nPkrDvcPattern[DEF_PICK], DEF_PICK, &nPockX_Pick, &nPockY_Pick, nUseJobPkr_Pick);
// 		}
// 			
// 		int nRetCnt_Place = -1;
// 		if (nAvailableCountPlace > 0) {
// 			FillDvc_VirtualStation(eDeviceStage_TEST_SITE_1 + m_vModuleSetPressIdx[nAtsIdx], (int*)nArrVirtualStation[DEF_PLACE], VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, DEF_PLACE, (bPickPlace == DEF_EXCH ? TRUE : FALSE) );
// 			nRetCnt_Place = FindPickPlaceFromStation(nAtsIdx, nRow, nArrVirtualStation[DEF_PLACE], nPkrDvcPattern[DEF_PLACE], DEF_PLACE, &nPockX_Place, &nPockY_Place, nUseJobPkr_Place);
// 		}
// 			
// 		std::vector<CPoint> vPkrPick, vPkrPlace;
// 		if (nRetCnt_Pick > 0)
// 		{
// 			for (int nPickX = 0; nPickX<MAX_TEST_PP_PKR_CNT_X; nPickX++) {
// 				for (int nPickY = 0; nPickY<MAX_TEST_PP_PKR_CNT_Y; nPickY++) {
// 					if (nUseJobPkr_Pick[nPickX][nPickY] != 0) {
// 						vPkrPick.push_back(CPoint(nPickX, nPickY));
// 					}
// 				}
// 			}
// 		}
// 		if (nRetCnt_Place > 0 )
// 		{
// 			for (int nPickX = 0; nPickX<MAX_TEST_PP_PKR_CNT_X; nPickX++) {
// 				for (int nPickY = 0; nPickY<MAX_TEST_PP_PKR_CNT_Y; nPickY++) {
// 					if (nUseJobPkr_Place[nPickX][nPickY] != 0) {
// 						vPkrPlace.push_back(CPoint(nPickX, nPickY));
// 					}
// 				}
// 			}
// 		}
// 		if ( bPickPlace == DEF_EXCH && nRetCnt_Pick > 0 && nRetCnt_Place > 0 )
// 		{
// 			int nCnt = 0;
// 			// pick 1,2했는데 3,4 place일때 device 3,4자리에 있으면 안된다.(interlock필요)
// 			for (int nR = 0; nR < (int)vPkrPick.size(); nR++)
// 			{
// 				for (int nF = 0; nF < (int)vPkrPlace.size(); nF++)
// 				{
// 					if (vPkrPick[nR].x == vPkrPlace[nF].x)
// 						nCnt++;
// 					/*else {
// 						int nDvc = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TEST_SITE_1 + m_vModuleSetPressIdx[nAtsIdx], vPkrPlace[nF].x, vPkrPlace[nF].y);
// 						if (nDvc == eDeviceData_None)
// 							nCnt++;
// 					}*/
// 				}
// 			}
// 			if ( (int)vPkrPlace.size() == nCnt)
// 			{
// 				vPocketPick = CPoint(nPockX_Pick, nPockY_Pick);
// 				vPocketPlace = CPoint(nPockX_Place, nPockY_Place);
// 				vPicker_Pick = vPkrPick;
// 				vPicker_Place = vPkrPlace;
// 				return eExcutingStep_FindJob;
// 			}
// 		}
// 		if (bPickPlace == DEF_PICK && nRetCnt_Pick > 0)
// 		{
// 			vPocketPick = CPoint(nPockX_Pick, nPockY_Pick);
// 			vPicker_Pick = vPkrPick;
// 			return eExcutingStep_FindJob;
// 		}
// 		if (bPickPlace == DEF_PLACE && nRetCnt_Place > 0)
// 		{
// 			vPocketPlace = CPoint(nPockX_Place, nPockY_Place);
// 			vPicker_Place = vPkrPlace;
// 			return eExcutingStep_FindJob;
// 		}
// 	}
// 	
// 
// 	return eExcutingStep_ErrorJob;
// }

/*=============================================================================================
  DESCRIPT : GetDvcCleanHandAssign
  KEY WORD : Socket Cleaning 용 test hand assign
  ARGUMENT : bPickPlace         = Pick / Place 동작
             nPickerDvcPattern  = hand 작업 가능 패턴
			 nWorkingAreaIdx    = 작업 할 Station Area Index
			 nStationWorkRow    = 작업 할 Station Area 의 row			 
  RETURN   : 작업 가능 picker 수
 ===============================================================================================*/
int CTaskAutoTestPp::GetDvcLdTableHandAssign(BOOL bPickPlace, int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nWorkingAreaIdx )
{
	int nPkrPossibleCnt = 0;
	if( bPickPlace == DEF_PICK )
	{
		BOOL bAutoRetetsMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
		int nSiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
		int nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
		int nSocketDvc[STATION_MAX_X_SITE][STATION_MAX_Y_SITE];
		int nPkrDvc[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y];
		memset( nSocketDvc, DEF_NOT_EXIST, sizeof(nSocketDvc) );
		memset( nPkrDvc, DEF_NOT_EXIST, sizeof(nPkrDvc) );

		// site의 socket on/off 정보.
		ST_DD_DEVICE stDvc;
		for (int nX = 0; nX < nSiteCntX; nX++)
		{
			for (int nY = 0; nY < nSiteCntY; nY++)
			{
				int nOnOff = g_TaskPressUnit[m_vModuleSetPressIdx[nWorkingAreaIdx]].GetSocketOnOff(nX, nY);
				if (nOnOff == DEF_SITE_ON)
				{
					stDvc.clear();
					g_TaskPressUnit[m_vModuleSetPressIdx[nWorkingAreaIdx]].GetSiteDeviceData(&stDvc, nX, nY);

					// site에 device가 없거나 또는 있더라도 test end / cleaning end 면 집을 수 있음.
					if (stDvc.sExist == DEF_NOT_EXIST || g_TaskSystemCtrl.checkUphMode() == true || m_bAlreadyPick == TRUE ||
						(stDvc.sExist == DEF_EXIST && (stDvc.nTestingInfo == eTestSite_TestEnd || stDvc.nTestingInfo == eTestSite_CleanEnd)))
					{
						nSocketDvc[nX][nY] = DEF_ON;	// 가능.
					}
					else
					{
						nSocketDvc[nX][nY] = DEF_OFF;	// 불가능
					}

					if (m_bAlreadyPick != TRUE) {
						// Art 1차 fail bin device 가 들어있는 곳은 이미 넣은 곳이므로 집지 않는다.
						if (bAutoRetetsMode == eOPTION_USE && stDvc.sExist == DEF_EXIST && stDvc.nTestingCnt == eARB_1stTestEnd && stDvc.nBinColor != eGoodBin)
							nSocketDvc[nX][nY] = DEF_OFF;	// 불가능
						//else
						//	nSocketDvc[nX][nStationWorkRow] = DEF_ON;	// 가능.
					}
				}
				else {
					//if (m_bAlreadyPick == TRUE) {
					//	nSocketDvc[nX][nStationWorkRow] = DEF_ON;	// 가능.
					//}
					//else {
					nSocketDvc[nX][nY] = DEF_OFF;	// 불가능
				//}
				}
			}			
		}
		// load picker 정보
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{
				stDvc.clear();
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY, &stDvc);
				nPkrDvc[nX][nY] = stDvc.sExist;
			}
		}
		// 사용 가능한 picker 정보		
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{
				if (nSocketDvc[nX][nY] == DEF_ON && nPkrDvc[nX][nY] == DEF_NOT_EXIST) {
					nPickerDvcPattern[nX][nY] = DEF_NOT_EXIST; // 사용가능
					nPkrPossibleCnt++;
				}
			}
		}
	}else{
		ST_DD_DEVICE stDvc;
		// unload picker 정보
		for(int nX=0; nX < MAX_TEST_PP_PKR_CNT_X; nX++ )
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{
				stDvc.clear();
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY + TEST_UD_PKR_Y, &stDvc);
				if (stDvc.sExist == DEF_EXIST
					&& strcmp(stDvc.szBinResult, DEF_SOCKET_CLEAN_DEVICE) != 0
					&& ChkAutoRetestBin(stDvc) == FALSE) {
					nPickerDvcPattern[nX][nY] = DEF_EXIST;	// 사용가능
					nPkrPossibleCnt++;
				}
			}
		}
	}

	return nPkrPossibleCnt;
}

int CTaskAutoTestPp::GetDvcLdTableHandAssignGrr(BOOL bPickPlace, int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nWorkingAreaIdx)
{
	int nPkrPossibleCnt = 0;
	if (bPickPlace == DEF_PICK)
	{
		BOOL bAutoRetetsMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
		int nSiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
		int nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
		int nSocketDvc[STATION_MAX_X_SITE][STATION_MAX_Y_SITE];
		int nPkrDvc[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y];
		memset(nSocketDvc, DEF_NOT_EXIST, sizeof(nSocketDvc));
		memset(nPkrDvc, DEF_NOT_EXIST, sizeof(nPkrDvc));

		// site의 socket on/off 정보.
		ST_DD_DEVICE stDvc;
		for (int nX = 0; nX < nSiteCntX; nX++)
		{
			for (int nY = 0; nY < nSiteCntY; nY++)
			{
				int nOnOff = g_TaskPressUnit[m_vModuleSetPressIdx[nWorkingAreaIdx]].GetSocketOnOffGrr(nX, nY);
				if (nOnOff == DEF_SITE_ON)
				{
					stDvc.clear();
					g_TaskPressUnit[m_vModuleSetPressIdx[nWorkingAreaIdx]].GetSiteDeviceData(&stDvc, nX, nY);

					// site에 device가 없거나 또는 있더라도 test end / cleaning end 면 집을 수 있음.
					if (stDvc.sExist == DEF_NOT_EXIST || g_TaskSystemCtrl.checkUphMode() == true || m_bAlreadyPick == TRUE ||
						(stDvc.sExist == DEF_EXIST && (stDvc.nTestingInfo == eTestSite_TestEnd || stDvc.nTestingInfo == eTestSite_CleanEnd)))
					{
						nSocketDvc[nX][nY] = DEF_ON;	// 가능.
					}
					else
					{
						nSocketDvc[nX][nY] = DEF_OFF;	// 불가능
					}

					if (m_bAlreadyPick != TRUE) {
						// Art 1차 fail bin device 가 들어있는 곳은 이미 넣은 곳이므로 집지 않는다.
						if (bAutoRetetsMode == eOPTION_USE && stDvc.sExist == DEF_EXIST && stDvc.nTestingCnt == eARB_1stTestEnd && stDvc.nBinColor != eGoodBin)
							nSocketDvc[nX][nY] = DEF_OFF;	// 불가능
															//else
															//	nSocketDvc[nX][nStationWorkRow] = DEF_ON;	// 가능.
					}
				}
				else {
					//if (m_bAlreadyPick == TRUE) {
					//	nSocketDvc[nX][nStationWorkRow] = DEF_ON;	// 가능.
					//}
					//else {
					nSocketDvc[nX][nY] = DEF_OFF;	// 불가능
													//}
				}
			}
		}
		// load picker 정보
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{
				stDvc.clear();
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY, &stDvc);
				nPkrDvc[nX][nY] = stDvc.sExist;
			}
		}
		// 사용 가능한 picker 정보		
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{
				if (nSocketDvc[nX][nY] == DEF_ON && nPkrDvc[nX][nY] == DEF_NOT_EXIST) {
					nPickerDvcPattern[nX][nY] = DEF_NOT_EXIST; // 사용가능
					nPkrPossibleCnt++;
				}
			}
		}
	}
	else {
		ST_DD_DEVICE stDvc;
		// unload picker 정보
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{
				stDvc.clear();
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY + TEST_UD_PKR_Y, &stDvc);
				if (stDvc.sExist == DEF_EXIST
					&& strcmp(stDvc.szBinResult, DEF_SOCKET_CLEAN_DEVICE) != 0
					&& ChkAutoRetestBin(stDvc) == FALSE) {
					nPickerDvcPattern[nX][nY] = DEF_EXIST;	// 사용가능
					nPkrPossibleCnt++;
				}
			}
		}
	}

	return nPkrPossibleCnt;
}



int CTaskAutoTestPp::CheckJob_LoadTbl( int nAtsIdx, BOOL bPickPlace, CPoint &vPocket, std::vector<CPoint> &vPicker )
{
	// Pick / Place에 따라서, 탐색 할 Load Table에 Index를 얻어 온다.
	int nLoadTableIdx = GetLoadTableInx(bPickPlace);
	std::vector<CPoint> vErrPicker; vErrPicker.clear();
	std::vector<CString> vErrstring; vErrstring.clear();
	int nHandType = 0;
	CString strErrMsg = _T("");
	int nErr = ChkDevice(strErrMsg, nHandType, vErrstring);
	if (nErr != ERR_NO_ERROR) {
		g_TaskSystemCtrl.SetErrTestPpIdx(m_eTestIdx, 1);
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Test PP %d %s", m_eTestIdx + 1, strErrMsg);
		for (int i = 0; i < (int)vErrstring.size(); i++)
		{
			AlarmData.m_fnAddItemFormat("%s", vErrstring[i]);
		}
		g_Error.AlarmReport(nErr, AlarmData, &g_TaskSystemCtrl, g_TaskTestPP[m_eTestIdx].GetFailPicker(nHandType));
		g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
		return eExcutingStep_ErrorJob;
	}

	if (bPickPlace == DEF_PICK)
	{
		BOOL bChkCleanDevPick = GetDvcStationCleanDevAssign(DEF_PICK, nAtsIdx);
		BOOL bChkCleanDevPlace = GetDvcStationCleanDevAssign(DEF_PLACE, nAtsIdx);
		if (bChkCleanDevPick != TRUE && bChkCleanDevPlace != TRUE) {
		}
		else { return eExcutingStep_ErrorJob; }
	}



	// table이 단 한번도 ld side로 가지 않았기 때문에 job을 실행하면 안된다.
	int nTblRes = g_TaskLoadTbl[nLoadTableIdx].GetWorkReserve();
	if (nTblRes == RID_INIT_IDLE_RESERVE || nTblRes == RID_NOT_RESERVE)
		return eExcutingStep_WaitOtherJob;
	// cleaning 이 예약 되어 있을 경우에는 집지 못한다.
	if ((bPickPlace == DEF_PICK || (bPickPlace == DEF_PLACE && g_TaskTestPP[m_eTestIdx].GetUnloadPickerHaveDvcCnt() == 0)) && m_nCleanMode == TRUE)
		return eExcutingStep_ErrorJob;

	if(bPickPlace == DEF_PLACE &&  g_TaskLoadTbl[nLoadTableIdx].ChkMotorPos(LOC_IDX_LD_TBL_TEST_SIDE_1+ nAtsIdx,CTaskLoadTable::eAxisAll) != ERR_NO_ERROR
		&& g_TaskLoadTbl[nLoadTableIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
		return eExcutingStep_WaitOtherJob;

	// Loading table의 state를 점검한다.
	if (nTblRes == RID_TEST_PP_(m_eTestIdx))
	{
		if (g_TaskLoadTbl[nLoadTableIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
			return eExcutingStep_WaitOtherJob;

		//int nChkMotor = g_TaskLoadTbl[m_eTestIdx].ChkMotorPos(LOC_IDX_LD_TBL_TEST_SIDE, CTaskLoadTable::eAxisAll);
		//if( nChkMotor != ERR_NO_ERROR )
		//	return eExcutingStep_WaitOtherJob;
	}
	if (g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == FALSE
		|| g_TaskPressUnit[m_vModuleSetPressIdx[nAtsIdx]].ChkSiteInDvc() > 0
		|| g_TaskLoadTbl[nLoadTableIdx].GetLoadDeviceCnt() > 0
		/*|| g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_CONV_2) == TRUE
		|| g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_BTM_2) == TRUE*/
		|| g_TaskTrayLoadPP.GetTrayPpLoadDvcCnt() > 0)
	{
		if (nTblRes == RID_TRAY_PP_(RIDX_TRAY_PP_1))
		{
			//if (g_TaskTrayPP[m_eTrayIdx].GetTrayPpLoadDvcCnt() > 0
			//	|| g_TaskTrayPP[m_eTrayIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
			//return eExcutingStep_WaitOtherJob;
			return eExcutingStep_WaitOtherJob;
		}
	}
	
	
	
	//// state 점검을 했음에도 불구하고, load table에 집을 것이 없다면 집을 것이 없다.
	//if (bPickPlace == DEF_PICK && g_TaskLoadTbl[m_eTestIdx].GetLoadDeviceCnt() <= 0)
	//	return eExcutingStep_ErrorJob;

	//int nChkInterlock = ChkInterlockAts(nAtsIdx);
	//if( nChkInterlock == eExcutingStep_ErrorJob )
	//	return eExcutingStep_ErrorJob;

	vPicker.clear();
	int nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	// pick일 때만 site를 보고 확인하면 된다.
	if( bPickPlace == DEF_PLACE )
		nSiteCntY = 1;

	//for(int nY=0; nY < nSiteCntY; nY++ )
	{
		// find available hand picker
		int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y];
		for (int nPkrX = 0; nPkrX < MAX_TEST_PP_PKR_CNT_X; nPkrX++)
		{
			for (int nPkrY = 0; nPkrY < MAX_TEST_PP_PKR_CNT_Y; nPkrY++)
			{			
				nPickerDvcPattern[nPkrX][nPkrY] = (bPickPlace == DEF_PICK ? DEF_EXIST : DEF_NOT_EXIST);		
			}
		}
		int nAvailableCount = GetDvcLdTableHandAssign(bPickPlace, nPickerDvcPattern, nAtsIdx);

		// get clean buffer table
		if( nAvailableCount > 0 )
		{
			int nArrVirtualTbl[VIRTUAL_POCKET_X_MAX][VIRTUAL_POCKET_Y_MAX];
			memset(nArrVirtualTbl, 0xFF, sizeof(nArrVirtualTbl));
			FillDvc_VirtualTable(eDeviceStage_LD_TBL_1 + nLoadTableIdx, (int*)nArrVirtualTbl, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, bPickPlace,nAtsIdx, DEF_TABLE_TYPE_LOAD_UNLOAD);

			int nUseJobPkr[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = {0,};
			int nPockX = -1, nPockY = -1;
			int nRetCode = FindPickPlaceFromTable(LOC_IDX_TEST_PP_TBL_1_LD + nLoadTableIdx, nArrVirtualTbl, nPickerDvcPattern, bPickPlace, &nPockX, &nPockY, nUseJobPkr);
			if(nRetCode > 0){
				std::vector<CPoint> vPkr;
				for (int nPickY = 0; nPickY<MAX_TEST_PP_PKR_CNT_Y; nPickY++) {
					for(int nPickX=0; nPickX<MAX_TEST_PP_PKR_CNT_X; nPickX++){			
						if(nUseJobPkr[nPickX][nPickY]!=0) {
							vPkr.push_back( CPoint(nPickX, nPickY) );
						}
					}
				}

				if (vPkr.empty() == false) {
// 					if (bPickPlace == DEF_PLACE) {
// 						vPocket = CPoint(nPockX, nPockY);
// 						vPicker = vPkr;
// 						return eExcutingStep_FindJob;
// 					}
// 					else
// 					{
// 						vPocket = CPoint(nPockX, nPockY);
// 						vPicker = vPkr;
// 						return eExcutingStep_FindJob;
// 					}

					vPocket = CPoint(nPockX, nPockY);
					vPicker = vPkr;
					return eExcutingStep_FindJob;
					
					/// SJI 현재 작업 하려고 하는 Station 번호에서 나온 Device 가 Buffer Table에 있을 경우 Load Table 에서 Pick 할 수량을 변경 해준다
					if (bPickPlace == DEF_PICK) {
						ST_DD_DEVICE stData;
						int nPocketCount_X = g_DMCont.m_dmTestPP.GN(NDM4_TestBuffTbl_Div_X);
						int nPocketCount_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestBuffTbl_Div_Y);
						int nSameDvcCnt = 0;
						int nTestPpLDPickerDvcCnt = 0;
						int nSiteArtDvcCnt = 0;
						int nSiteOnCnt = 0;
				
						for (int nY = 0; nY < nPocketCount_Y; nY++) {
							for (int nX = 0; nX < nPocketCount_X; nX++) {
								stData.clear();
								g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1_ART_BUFF + m_eTestIdx, nX, nY, &stData);
				
								if (stData.sExist == DEF_EXIST) {
									BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stData);
									if( (strcmp(stData.szErrString, DEF_TEST_RESULT_TP101) == 0 || bIsDefaultBin == TRUE )&& atoi(stData.szTestSiteNum) == m_vModuleSetPressIdx[nAtsIdx] )
										nSameDvcCnt++;
									else if (strcmp(stData.szErrString, DEF_TEST_RESULT_TP101) != 0 && bIsDefaultBin != TRUE && stData.nTestingCnt < eARB_2stTestEnd && atoi(stData.szTestSiteNum) == m_vModuleSetPressIdx[nAtsIdx]) {
										nSameDvcCnt++;
									}
								}
							}
						}
				
						if (nSameDvcCnt > 0) {
							int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
							int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
				
							for (int nY = 0; nY < nSiteDivY; nY++)
							{
								for (int nX = 0; nX < nSiteDivX; nX++)
								{
									stData.clear();
									g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1 + m_vModuleSetPressIdx[nAtsIdx], nX, nY, &stData);
									int nSiteOnOff = g_TaskPressUnit[m_vModuleSetPressIdx[nAtsIdx]].GetSocketOnOff(nX, nY);
									if (nSiteOnOff == DEF_SITE_ON) {
										nSiteOnCnt++;
				
										BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stData);
				
										if(stData.sExist == DEF_EXIST && ((stData.nBinColor == eFailBin && stData.nTestingCnt != eARB_3stTestEnd) || strcmp(stData.szErrString, DEF_TEST_RESULT_TP101) == 0 || bIsDefaultBin == TRUE)) {
											nSiteArtDvcCnt++;
										}
									}
								}
							}
				
							// load picker 정보
							nTestPpLDPickerDvcCnt =	g_TaskTestPP[m_eTestIdx].GetLoadPickerDeviceCnt();
				
							//Site On 인데 Device 가 없는 Site Count  - Hand 들고있는 Count - Buffer Tbl에 있는 Count 를 한 Count 가 Table 에서 새로운 Device 를 Pick 할수 있는 Count
							int nIsPossibleDvcCnt = nSiteOnCnt - nTestPpLDPickerDvcCnt - nSameDvcCnt - nSiteArtDvcCnt;
				
							if (nIsPossibleDvcCnt > 0) {
								int nOutCnt = (int)vPkr.size() - nIsPossibleDvcCnt;
				
								if (nOutCnt > 0) {
									for (int i = 0; i < nOutCnt; i++) {
										if (vPkr.empty() == false) {
											vPkr.pop_back();
										}
									}
								}
								vPocket = CPoint(nPockX, nPockY);
								vPicker = vPkr;
								return eExcutingStep_FindJob;
							}
						}
						else {
							vPocket = CPoint(nPockX, nPockY);
							vPicker = vPkr;
							return eExcutingStep_FindJob;
						}
									
				
					}//if (bPickPlace == DEF_PICK)
				}//if (vPkr.empty() == false)
			}
		}
	}

	return eExcutingStep_ErrorJob;
}

/*=============================================================================================
  DESCRIPT : ChkAutoRetestBin
  KEY WORD : device auto retest 작업 bin 인지 판단
  ARGUMENT : stDeviceData : device data
  RETURN   : TRUE  : retest bin
             FALSE : not retest bin
 ===============================================================================================*/
int CTaskAutoTestPp::ChkAutoRetestBin( ST_DD_DEVICE stDeviceData )
{
	BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stDeviceData);
	if( (stDeviceData.nTestingCnt == eARB_1stTestEnd || stDeviceData.nTestingCnt == eARB_2stTestEnd || strcmp(stDeviceData.szErrString, DEF_TEST_RESULT_TP101) == 0 || bIsDefaultBin == TRUE) && stDeviceData.nBinColor != eGoodBin )
		return TRUE;
	return FALSE;
}

int CTaskAutoTestPp::GetDvcArtBuffHandAssign(BOOL bPickPlace, int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nWorkingAreaIdx)
{
	BOOL bModeAutoRetest = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
	BOOL bAtoAPickPlace = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_ART_TEST_HAND_PICK_PLACE);

	int nCleanMode = ChkCleaningMode(m_nJobAtsIdx);

	if (nCleanMode == TRUE)
	{
		bAtoAPickPlace = FALSE;
	}

	int nProject = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	ST_DD_DEVICE stDvc;
	int nPkrPossibleCnt = 0;
	if (bPickPlace == DEF_PICK)
	{
		int nSiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
		int nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
		int nSocketDvc[STATION_MAX_X_SITE][STATION_MAX_Y_SITE];
		int nPkrDvc[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y];
		memset(nSocketDvc, DEF_NOT_EXIST, sizeof(nSocketDvc));
		memset(nPkrDvc, DEF_NOT_EXIST, sizeof(nPkrDvc));
		
		for (int nX = 0; nX < nSiteCntX; nX++)
		{
			for (int nY = 0; nY < nSiteCntY; nY++)
			{
				int nOnOff = g_TaskPressUnit[m_vModuleSetPressIdx[nWorkingAreaIdx]].GetSocketOnOff(nX, nY);
				if (nOnOff == DEF_SITE_ON)
				{
					stDvc.clear();
					g_TaskPressUnit[m_vModuleSetPressIdx[nWorkingAreaIdx]].GetSiteDeviceData(&stDvc, nX, nY);

					// site에 device가 없거나 또는 있더라도 test end / cleaning end 면 집을 수 있음.
					if (stDvc.sExist == DEF_NOT_EXIST || (stDvc.sExist == DEF_EXIST && (stDvc.nTestingInfo == eTestSite_TestEnd || stDvc.nTestingInfo == eTestSite_CleanEnd)))
						nSocketDvc[nX][nY] = DEF_ON;	// 가능.
					else
						nSocketDvc[nX][nY] = DEF_OFF;	// 불가능

					BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stDvc);

					if (bAtoAPickPlace == eOPTION_USE && stDvc.nBinColor == eFailBin && stDvc.nTestingCnt == eARB_1stTestEnd && strcmp(stDvc.szErrString, DEF_TEST_RESULT_TP101) != 0 && bIsDefaultBin != TRUE)
						nSocketDvc[nX][nY] = DEF_OFF;	// 불가능

					//if (stDvc.nBinColor == eFailBin && strcmp(stDvc.szErrString, DEF_TEST_RESULT_TP101) == 0
					//	&& (atoi(stDvc.szTestSiteNum) == m_vModuleSetPressIdx[nWorkingAreaIdx]))
					//	nSocketDvc[nX][nStationWorkRow] = DEF_OFF;	// 불가능
				}
				else {
					nSocketDvc[nX][nY] = DEF_OFF;	// 불가능
				}
			}
		}
		// load picker 정보
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{
				nPkrDvc[nX][nY] = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY);
			}
		}
		// 사용 가능한 picker 정보		
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{			
				if (nSocketDvc[nX][nY] == DEF_ON && nPkrDvc[nX][nY] == DEF_NOT_EXIST) {
					nPickerDvcPattern[nX][nY] = DEF_NOT_EXIST; // 사용가능
					nPkrPossibleCnt++;
				}
			}
		}
	}
	else {
		// unload picker 정보
		BOOL bAutoRetetsMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);

		bool bIsAllSocketOffPlace =  g_DMCont.m_dmTestPP.GN(BDM4_TESTHAND1_ALL_SOCKET_OFF_PLACE_ART_BUFFER + m_eTestIdx);
		int nTestPpHand = (bIsAllSocketOffPlace == true) ? TEST_LD_PKR_Y : TEST_UD_PKR_Y;

		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{
				stDvc.clear();
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY + nTestPpHand, &stDvc);
				// Hand Device 가 있어야 한다.
				// Test 3st이면 내려 놓을 수 없다.
				if (stDvc.sExist == DEF_EXIST)
				{
					if (bAutoRetetsMode == eOPTION_USE) {
						if (stDvc.nBinColor == eGoodBin)
							continue;
						if (stDvc.nTestingCnt == eARB_3stTestEnd)
							continue;
						if (strcmp(stDvc.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0)
							continue;
						if (strcmp(stDvc.szErrString, DEF_2D_READ_ERR_DEVICE) == 0)
							continue;
						if (strcmp(stDvc.szErrString, DEF_TEST_RESULT_WRONG_APN) == 0)
							continue;

						//if(g_TaskSystemCtrl.IsCompanyProjectIndex() == eUSI_Marlin)
					
						// USI Marlin은 Test Time Out 발생하면, Test Time Out Bin으로 배출 [7/19/2021 donghyun.shin]
// 						if (strcmp(stDvc.szErrString, DEF_TEST_TIME_OUT_ERR_DEVICE) == 0)
// 							continue;
						


						nPickerDvcPattern[nX][nY] = DEF_EXIST;	// 사용가능
						nPkrPossibleCnt++;
					}
					else if (bAutoRetetsMode == eOPTION_NOT_USE) { // SJI
						BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stDvc);
						if (strcmp(stDvc.szErrString, DEF_TEST_RESULT_TP101) == 0 || bIsDefaultBin == TRUE) {
							nPickerDvcPattern[nX][nY] = DEF_EXIST;	// 사용가능
							nPkrPossibleCnt++;
						}
					}
				}
			}
		}
	}

	return nPkrPossibleCnt;
}

int CTaskAutoTestPp::CheckJob_ArtBuff(int nAtsIdx, BOOL bPickPlace, CPoint &vPocket, std::vector<CPoint> &vPicker,int& nLocIdx)
{
	CString strErrMsg = _T("");
	std::vector<CPoint> vErrPicker; vErrPicker.clear();
	std::vector<CString> vErrstring; vErrstring.clear();
	int nHandType = 0;
	int nErr = ChkDevice(strErrMsg, nHandType, vErrstring);
	if (nErr != ERR_NO_ERROR) {
		g_TaskSystemCtrl.SetErrTestPpIdx(m_eTestIdx, 1);
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Test PP %d %s", m_eTestIdx + 1, strErrMsg);
		for (int i = 0; i < (int)vErrstring.size(); i++)
		{
			AlarmData.m_fnAddItemFormat("%s", vErrstring[i]);
		}
		g_Error.AlarmReport(nErr, AlarmData, &g_TaskSystemCtrl, g_TaskTestPP[m_eTestIdx].GetFailPicker(nHandType));
		g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
		return eExcutingStep_ErrorJob;
	}

	if (bPickPlace == DEF_PICK)
	{
		BOOL bChkCleanDevPick = GetDvcStationCleanDevAssign(DEF_PICK, nAtsIdx);
		BOOL bChkCleanDevPlace = GetDvcStationCleanDevAssign(DEF_PLACE, nAtsIdx);
		if (bChkCleanDevPick != TRUE && bChkCleanDevPlace != TRUE) {
		}
		else { return eExcutingStep_ErrorJob; }
	}


	
	int nLoadTblPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	int nLoadTblPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

	int nLoadTableIdx = -1;

	for (auto nLoadTblIdx : m_vModuleSetLoadTableIdx)
	{
		if (bPickPlace == DEF_PICK)
		{
			int nBufferDvcCnt = g_TaskLoadTbl[nLoadTblIdx].GetBufferDeviceCnt();
			if (nBufferDvcCnt > 0)
			{
				nLoadTableIdx = nLoadTblIdx;
				break;
			}
		}
		else
		{
			int nBufferDvcCnt = g_TaskLoadTbl[nLoadTblIdx].GetBufferDeviceCnt();
			if (nBufferDvcCnt < (nLoadTblPocketCntX*nLoadTblPocketCntY / 2))
			{
				nLoadTableIdx = nLoadTblIdx;
				break;
			}		
		}
	}
	
	if(nLoadTableIdx == -1)
		return eExcutingStep_ErrorJob;


	int nTblRes = g_TaskLoadTbl[nLoadTableIdx].GetWorkReserve();
	if (nTblRes == RID_INIT_IDLE_RESERVE || nTblRes == RID_NOT_RESERVE)
		return eExcutingStep_WaitOtherJob;

	//BOOL bAutoRetetsMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
	//if( bAutoRetetsMode == eOPTION_NOT_USE )
		//return eExcutingStep_ErrorJob; //SJI

	if( bPickPlace == DEF_PICK && m_nCleanMode == TRUE )
		return eExcutingStep_ErrorJob;

	vPicker.clear();

	int nMaxBuff_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestBuffTbl_Div_Y);
	int nMaxBuff_X = g_DMCont.m_dmTestPP.GN(NDM4_TestBuffTbl_Div_X);
	
	//for (int nY = 0; nY < nSiteCntY; nY++)
	{
		// art A to A bin이 있는지 점검한다.
		// a to a'를 해야 하는게 있기 때문에 buffer에서 먼저 작업을 한다.
		if (bPickPlace == DEF_PICK)
		{			
			ST_DD_DEVICE stDvc, stSiteDvc;
			// buffer의 retest bin 정보에 따라서 picker를 쓸 수 있다.			
			for (int nLoadBuffY = 0; nLoadBuffY < nLoadTblPocketCntY; nLoadBuffY++)
			{
				for (int nLoadBuffX = 0; nLoadBuffX < nLoadTblPocketCntX; nLoadBuffX++)
				{
					if (g_TaskLoadTbl[nLoadTableIdx].GetBufferPocketCheck(nLoadBuffX, nLoadBuffY) != TRUE)
						continue;

					stDvc.clear();
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1 + nLoadTableIdx, nLoadBuffX, nLoadBuffY, &stDvc);

					if( stDvc.sExist != eDeviceData_Exist )
						continue;

					BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stDvc);

					if (stDvc.nBinColor == eFailBin && (strcmp(stDvc.szErrString, DEF_TEST_RESULT_TP101) == 0 || bIsDefaultBin == TRUE)
						&& (atoi(stDvc.szTestSiteNum) == m_vModuleSetPressIdx[nAtsIdx]) )
							continue;

					int nSiteOn = g_TaskPressUnit[m_vModuleSetPressIdx[nAtsIdx]].GetSocketOnOff(stDvc.cSubSiteNum.x, stDvc.cSubSiteNum.y);

					bool bMatch = ( atoi(stDvc.szTestSiteNum) == m_vModuleSetPressIdx[nAtsIdx]);
					if ((stDvc.nBinColor == eFailBin && stDvc.nTestingCnt == eARB_1stTestEnd && (bMatch == true && strcmp(stDvc.szErrString, DEF_TEST_RESULT_TP101) != 0 && bIsDefaultBin != TRUE))
						&& nSiteOn == DEF_SITE_ON
						/*|| (bMatch != true && (strcmp(stDvc.szErrString, DEF_TEST_RESULT_TP101) == 0 || bIsDefaultBin == TRUE) && nSiteOn == DEF_SITE_ON)*/)
					{
						stSiteDvc.clear();
						g_TaskPressUnit[m_vModuleSetPressIdx[nAtsIdx]].GetSiteDeviceData(&stSiteDvc, stDvc.cSubSiteNum.x, stDvc.cSubSiteNum.y);

						int nExistPkr = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TEST_PP_1 + m_eTestIdx, stDvc.cSubSiteNum.x, stDvc.cSubSiteNum.y);
						// site에 device가 없거나 또는 있더라도 test end / cleaning end 면 집을 수 있음.
						if ( nExistPkr == eDeviceData_None 
							&& ( stSiteDvc.sExist == DEF_NOT_EXIST || (stSiteDvc.sExist == DEF_EXIST && (stSiteDvc.nTestingInfo == eTestSite_TestEnd || stSiteDvc.nTestingInfo == eTestSite_CleanEnd))) )
						{
							vPocket = CPoint(nLoadBuffX, nLoadBuffY); 
							vPicker.push_back(CPoint(stDvc.cSubSiteNum.x, stDvc.cSubSiteNum.y));
							switch (nLoadTableIdx)
							{
							case eLoadTbl_2:
							case eLoadTbl_3:
							{
								//[11/1/2024 dohyeong.kim] 
								//Load Table Buffer사용 시, Hand 영역이 나오는 TBL3으로 고정
								//Teaching Table에는 없으며, LOC_IDX_TEST_PP_TBL_3_LD 위치로 사용.
								nLocIdx = LOC_IDX_TEST_PP_LOAD_TBL_BUFF;

								//nLocIdx = LOC_IDX_TEST_PP_TBL_1_LD;
							}break;
							case eLoadTbl_1:
							case eLoadTbl_4:
							{
								nLocIdx = LOC_IDX_TEST_PP_TBL_1_UD;
							}break;
							}

// 							int nTblRes = g_TaskLoadTbl[nLoadTableIdx].GetWorkReserve();
// 							int nChkPos = g_TaskLoadTbl[nLoadTableIdx].ChkMotorPos(LOC_IDX_LD_TBL_TEST_SIDE_1 + nAtsIdx,CTaskLoadTable::eAxisAll);
// 							if (nChkPos != ERR_NO_ERROR ||(nTblRes == RID_INIT_IDLE_RESERVE || nTblRes == RID_NOT_RESERVE))
							if(g_TaskLoadTbl[nLoadTableIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
 								return eExcutingStep_WaitOtherJob;

							return eExcutingStep_FindJob;
						}
					}
				}
			}
		}

		// == art a' to b bin에 대한 작업 검사		
		// find available hand picker
		int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y];
		for (int nPkrY = 0; nPkrY < MAX_TEST_PP_PKR_CNT_Y; nPkrY++)
		{
			for (int nPkrX = 0; nPkrX < MAX_TEST_PP_PKR_CNT_X; nPkrX++)
			{
				nPickerDvcPattern[nPkrX][nPkrY] = (bPickPlace == DEF_PICK ? DEF_EXIST : DEF_NOT_EXIST);
			}
		}
		int nAvailableCount = GetDvcArtBuffHandAssign(bPickPlace, nPickerDvcPattern, nAtsIdx);
		if (nAvailableCount > 0)
		{
			// get art buffer table
			int nArrVirtualBuff[VIRTUAL_POCKET_X_MAX][VIRTUAL_POCKET_Y_MAX];
			memset(nArrVirtualBuff, 0xFF, sizeof(nArrVirtualBuff));
			FillDvc_VirtualTable(eDeviceStage_LD_TBL_1 + nLoadTableIdx, (int*)nArrVirtualBuff, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, bPickPlace, nAtsIdx, DEF_TABLE_TYPE_BUFFER);

			int nUseJobPkr[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = { 0, };
			int nPockX = -1, nPockY = -1;
			int nRetCode = FindPickPlaceFromTable(LOC_IDX_TEST_PP_TBL_1_LD + nLoadTableIdx, nArrVirtualBuff, nPickerDvcPattern, bPickPlace, &nPockX, &nPockY, nUseJobPkr, DEF_TABLE_TYPE_BUFFER);
			if (nRetCode > 0) {
				std::vector<CPoint> vPkr;
				for (int nPickX = 0; nPickX<MAX_TEST_PP_PKR_CNT_X; nPickX++) {
					for (int nPickY = 0; nPickY<MAX_TEST_PP_PKR_CNT_Y; nPickY++) {
						if (nUseJobPkr[nPickX][nPickY] != 0) {
							vPkr.push_back(CPoint(nPickX, nPickY));
						}
					}
				}
				switch (nLoadTableIdx)
				{
					case eLoadTbl_2:
					case eLoadTbl_3:
					{
						//[11/1/2024 dohyeong.kim] 
						//Load Table Buffer사용 시, Hand 영역이 나오는 TBL3으로 고정
						//Teaching Table에는 없으며, LOC_IDX_TEST_PP_TBL_3_LD 위치로 사용.
						nLocIdx = LOC_IDX_TEST_PP_LOAD_TBL_BUFF;

						//기존
						//nLocIdx = LOC_IDX_TEST_PP_TBL_1_LD;
					}break;
					case eLoadTbl_1:
					case eLoadTbl_4:
					{
						nLocIdx = LOC_IDX_TEST_PP_TBL_1_UD;
					}break;
				}

// 				int nTblRes = g_TaskLoadTbl[nLoadTableIdx].GetWorkReserve();
// 				int nChkPos = g_TaskLoadTbl[nLoadTableIdx].ChkMotorPos(LOC_IDX_LD_TBL_TEST_SIDE_1 + nAtsIdx, CTaskLoadTable::eAxisAll);
// 				if (nChkPos != ERR_NO_ERROR || (nTblRes == RID_INIT_IDLE_RESERVE || nTblRes == RID_NOT_RESERVE))
// 					return eExcutingStep_WaitOtherJob;
				if (g_TaskLoadTbl[nLoadTableIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
					return eExcutingStep_WaitOtherJob;
									 
				vPocket = CPoint(nPockX, nPockY);
				vPicker = vPkr;
				return eExcutingStep_FindJob;
			}
		}
	}

	return eExcutingStep_ErrorJob;
}

/*=============================================================================================
  DESCRIPT : FindPickPlaceFromTable
  KEY WORD : loadtable, ART buffer table, Socket Cleaning Table 의 작업
  ARGUMENT : nLocIdx            = 작업 할 Loc Index
             nTableDvc          = Table의 device 정보
             nPickerDvcPattern  = hand 작업 가능 패턴
			 bPickPlace         = pick / place 동작
			 n_Xpos             = 찾은 작업 X 위치
			 n_Ypos             = 찾은 작업 Y 위치
			 nWorkPkr           = 찾은 작업 picker pattern
  RETURN   : 작업 가능 수
 ===============================================================================================*/
int CTaskAutoTestPp::FindPickPlaceFromTable( int nLocIdx, int nTableDvc[VIRTUAL_POCKET_X_MAX][VIRTUAL_POCKET_Y_MAX], int nPickerPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], BOOL bPickPlace, int *n_Xpos, int *n_YPos, int nWorkPkr[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nTableLoc)
{
	//ASSERT( nLocIdx == LOC_IDX_CLEAN_BUFFER || nLocIdx == LOC_IDX_ART_BUFFER || nLocIdx == LOC_IDX_TEST_PP_LD_TBL );

	int nSearchY = 0, nSearchX = 0;
	int nSearchHandY = 0, nSearchHandX = 0;

	/*DEF_WORK_FREE or DEF_WORK_RESERVED*/
	int nArrHandWorkSource[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = {DEF_WORK_FREE,};
	int nArrHandWorkCopy[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y]   = {DEF_WORK_FREE,};

	int nPocketCntX = 0, nPocketCntY = 0;
	int nDeviceState = 0;
	int nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);


	switch( nLocIdx )
	{
	case LOC_IDX_CLEAN_TBL:
		{
			nPocketCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_X);
			nPocketCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_Y);
			// Test PP Clean Table 1:1 대응 으로 Pocket Count 강제로 0으로 변경 (충돌 영역 발생 문제)
			nPocketCntX = 1;
			nPocketCntY = 1;
		}break;
	case LOC_IDX_TEST_PP_TBL_1_LD:
	case LOC_IDX_TEST_PP_TBL_2_LD:
	case LOC_IDX_TEST_PP_TBL_3_LD:
	case LOC_IDX_TEST_PP_TBL_4_LD:
		{
			nPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
			nPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

			nDeviceState = nLocIdx - LOC_IDX_TEST_PP_TBL_1_LD;

		}break;
	}

	int nTotalPossibleCnt = 0;	// 가장 Pick 할 수 있는 수량
	int nCurrentCnt		  = 0;	// 현재 까지 Pick 할수 있는 수량
	int nCXPosition	= 0, nCYPosition = 0;
	// 1Y, 2Y는 계산 한 대로 가져 온다.	

	int nPitchType_X  = g_TaskTestPP[m_eTestIdx].GetHandPitchType(CTaskTestPP::eAxisPitch_HW_X1_SW_Y1, nLocIdx);	// Hand와 Target간의 간격 관계 (0=1by1, 1=1:1매칭, 2=한칸뛰기,3=두칸뛰기), Hand 기준임. (Hand 1,3으로 Target 1,2 집는것은 처리 필요)
	int nPitchType_Y  = g_TaskTestPP[m_eTestIdx].GetHandPitchType(CTaskTestPP::eAxisPitch_HW_Y1_SW_X1, nLocIdx);	// Hand와 Target간의 간격 관계 (0=1by1, 1=1:1매칭, 2=한칸뛰기,3=두칸뛰기), Hand 기준임. (Hand 1,3으로 Target 1,2 집는것은 처리 필요)

	int nChkCleanBuffer   = DEF_NOT_EXIST;
	int nHadData		  = DEF_NOT_EXIST;
	if(	bPickPlace == DEF_PICK ){	// Pick 일 때 조건
		nChkCleanBuffer = DEF_EXIST;
		nHadData		= DEF_NOT_EXIST;
	}else{							// Place 일 때 조건		
		nChkCleanBuffer = DEF_NOT_EXIST;
		nHadData		= DEF_EXIST;
	}
	
	
	ST_DD_DEVICE stData;


	nSearchY = GetFindLoadTableSearchY(static_cast<eDeviceDataStage>(eDeviceStage_LD_TBL_1 + nDeviceState), nTableLoc, nLocIdx, nPitchType_Y, bPickPlace);
	
	for(; nSearchY < (VIRTUAL_POCKET_Y_START + nPocketCntY); nSearchY++)
	{
		nSearchX = GetFindLoadTableSearchX(static_cast<eDeviceDataStage>(eDeviceStage_LD_TBL_1 + nDeviceState), nTableLoc, nLocIdx, nPitchType_X, bPickPlace);
		for(; nSearchX < (VIRTUAL_POCKET_X_START + nPocketCntX); nSearchX++)
		{
			nCurrentCnt = 0;

			memset(nArrHandWorkSource, 0, sizeof(nArrHandWorkSource));

			for (nSearchHandX = 0; nSearchHandX < MAX_TEST_PP_PKR_CNT_X; nSearchHandX++)
			{
				for(nSearchHandY = 0; nSearchHandY < MAX_TEST_PP_PKR_CNT_Y; nSearchHandY++)
				{		
					int nYPocket = 0, nXPocket = 0;
					nYPocket = nSearchY + nSearchHandY * nPitchType_Y;
					nXPocket = nSearchX + nSearchHandX * nPitchType_X;

					if(     nTableDvc[ nXPocket ][ nYPocket ]		==	nChkCleanBuffer
						&&  nPickerPattern[nSearchHandX][nSearchHandY]	== 	nHadData)
					{
						if ( bPickPlace == DEF_PICK && g_TaskSystemCtrl.GetAutoGrrUse() == TRUE)
						{
							stData.clear();

							//수정 필요
							switch( nLocIdx )
							{

							case LOC_IDX_TEST_PP_TBL_1_LD:
							case LOC_IDX_TEST_PP_TBL_2_LD:
							case LOC_IDX_TEST_PP_TBL_3_LD:
							case LOC_IDX_TEST_PP_TBL_4_LD:
							{
								nDeviceState = nLocIdx - LOC_IDX_TEST_PP_TBL_1_LD;

								g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1+ nDeviceState, nXPocket - VIRTUAL_POCKET_X_START, nYPocket - VIRTUAL_POCKET_Y_START, &stData);
								
							}break;
							case LOC_IDX_CLEAN_TBL:
								g_DMCont.m_dmDevice.GetDeviceData( eDeviceStage_TEST_PP_1_CLEAN_BUFF + m_eTestIdx, nXPocket - VIRTUAL_POCKET_X_START, nYPocket - VIRTUAL_POCKET_Y_START, &stData );
								break;
							default: 
								break;
							}

							int nSiteInDvcCnt = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].ChkSiteInDvc();
							BOOL bErr = stData.stGrrTestInfo[m_vModuleSetPressIdx[m_nJobAtsIdx]].chkTestEnd(/*nSearchHandX, m_nJobSiteRow*/);
							if (bErr == TRUE && /*nSiteInDvcCnt > 0 &&*/ m_nCleanMode == FALSE)
 								continue;


						}

						switch (nLocIdx)
						{
							case LOC_IDX_TEST_PP_TBL_1_LD:
							case LOC_IDX_TEST_PP_TBL_4_LD:
							{
								// Unload Table Station과 제일 가까운 Pocket 1줄은 Test PP 뒷열 Picker로 Pick하지 못하도록 한다. 영역 관련 문제
								if (nSearchHandY == 0 && nSearchY == (VIRTUAL_POCKET_Y_START + (nPocketCntY - 1)))
								{
									continue;
								}
							}
						}

						nArrHandWorkSource[nSearchHandX][nSearchHandY] = DEF_WORK_RESERVED;
						nCurrentCnt++;
					}
				}
			}
			

			if( nCurrentCnt > nTotalPossibleCnt )
			{
				//작업해라.
				nTotalPossibleCnt = nCurrentCnt;
				memcpy_s(nArrHandWorkCopy, sizeof(nArrHandWorkCopy), nArrHandWorkSource, sizeof(nArrHandWorkSource));
				nCXPosition = nSearchX;
				nCYPosition = nSearchY;
			}
		}
	}
	

	memcpy_s(nWorkPkr, sizeof(nArrHandWorkCopy), nArrHandWorkCopy, sizeof(nArrHandWorkCopy));

	//0번 피커부터 할당되지 않은 포켓은 트림한다.
	int nPockOffsetX = 0;
	int nPockOffsetY = 1;

	//X방향 첫 할당 위치가 X옵셋
	for(; nPockOffsetX<MAX_TEST_PP_PKR_CNT_X; nPockOffsetX++)
	{
		if(nWorkPkr[nPockOffsetX][0]!=DEF_WORK_FREE) break;
		if(nWorkPkr[nPockOffsetX][1]!=DEF_WORK_FREE) break;
	}
	//X방향 첫 할당 위치가 1개라도 있으면 Y옵셋은 0
	for(int nX=0; nX<MAX_TEST_PP_PKR_CNT_X; nX++)
	{
		if(nWorkPkr[nX][0]!=DEF_WORK_FREE) {
			nPockOffsetY = 0;
			break;
		}
	}

	if (nPockOffsetY == 1) {
		int a = 0;
	}

	nCXPosition -= VIRTUAL_POCKET_X_START;
	nCYPosition -= VIRTUAL_POCKET_Y_START;

	*n_Xpos = nCXPosition + nPockOffsetX*nPitchType_X;
	*n_YPos = nCYPosition + nPockOffsetY*nPitchType_Y;

	return nTotalPossibleCnt;
}

int CTaskAutoTestPp::FindPickPlaceFromTableSwap(int nLocIdx, int nTableDvc[VIRTUAL_POCKET_X_MAX][VIRTUAL_POCKET_Y_MAX], int nPickerPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], BOOL bPickPlace, int *n_Xpos, int *n_YPos, int nWorkPkr[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], CPoint* cSamePP_Pos, int nPocketOffset)
{
	//ASSERT( nLocIdx == LOC_IDX_CLEAN_BUFFER || nLocIdx == LOC_IDX_ART_BUFFER || nLocIdx == LOC_IDX_TEST_PP_LD_TBL );

	int nSearchY = 0, nSearchX = 0;
	int nSearchHandY = 0, nSearchHandX = 0;

	/*DEF_WORK_FREE or DEF_WORK_RESERVED*/
	int nArrHandWorkSource[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = { DEF_WORK_FREE, };
	int nArrHandWorkCopy[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = { DEF_WORK_FREE, };

	int nPocketCntX = 0, nPocketCntY = 0;
	int nDeviceState = 0;

	switch (nLocIdx)
	{
	case LOC_IDX_CLEAN_TBL:
	{
		nPocketCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_X);
		nPocketCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_Y);
	}break;
	case LOC_IDX_TEST_PP_TBL_1_LD:
	case LOC_IDX_TEST_PP_TBL_2_LD:
	case LOC_IDX_TEST_PP_TBL_3_LD:
	case LOC_IDX_TEST_PP_TBL_4_LD:
	{
		nPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
		nPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

		nDeviceState = nLocIdx - LOC_IDX_TEST_PP_TBL_1_LD;
	}break;
	}


	int nTotalPossibleCnt = 0;	// 가장 Pick 할 수 있는 수량
	int nCurrentCnt = 0;	// 현재 까지 Pick 할수 있는 수량
	int nCXPosition = 0, nCYPosition = 0;
	// 1Y, 2Y는 계산 한 대로 가져 온다.	

	int nPitchType_X = g_TaskTestPP[m_eTestIdx].GetHandPitchType(CTaskTestPP::eAxisPitch_HW_X1_SW_Y1, nLocIdx);	// Hand와 Target간의 간격 관계 (0=1by1, 1=1:1매칭, 2=한칸뛰기,3=두칸뛰기), Hand 기준임. (Hand 1,3으로 Target 1,2 집는것은 처리 필요)
	int nPitchType_Y = g_TaskTestPP[m_eTestIdx].GetHandPitchType(CTaskTestPP::eAxisPitch_HW_Y1_SW_X1, nLocIdx);	// Hand와 Target간의 간격 관계 (0=1by1, 1=1:1매칭, 2=한칸뛰기,3=두칸뛰기), Hand 기준임. (Hand 1,3으로 Target 1,2 집는것은 처리 필요)

	int nTableData = DEF_NOT_EXIST;
	int nHadData = DEF_NOT_EXIST;
	if (bPickPlace == DEF_PICK) {	// Pick 일 때 조건
		nTableData = DEF_EXIST;
		nHadData = DEF_NOT_EXIST;
	}
	else {							// Place 일 때 조건		
		nTableData = DEF_NOT_EXIST;
		nHadData = DEF_EXIST;
	}


	ST_DD_DEVICE stData;

	if (cSamePP_Pos == NULL)
	{
		//nSearchY = VIRTUAL_POCKET_Y_START - (MAX_TEST_PP_PKR_CNT_Y * nPitchType_Y);
		nSearchY = VIRTUAL_POCKET_Y_START - (MAX_TEST_PP_PKR_CNT_Y - 1) * nPitchType_Y;
	}
	else {
		nSearchY = VIRTUAL_POCKET_Y_START - cSamePP_Pos->y - nPocketOffset;
	}

	for (/*nSearchY = VIRTUAL_POCKET_Y_START - ( MAX_TEST_PP_PKR_CNT_Y - 1 ) * nPitchType_Y*/; nSearchY < VIRTUAL_POCKET_Y_START + nPocketCntY; nSearchY++)
	{
		if (cSamePP_Pos == NULL)
		{
			nSearchX = VIRTUAL_POCKET_X_START - (MAX_TEST_PP_PKR_CNT_X - 1) * nPitchType_X;
		}
		else {
			nSearchX = VIRTUAL_POCKET_X_START - cSamePP_Pos->x * nPitchType_X;
		}
		//if(nSearchY > nSearchY_Limit) {break;} // CTray에서 첫 디바이스가 발견되는 Y+1 줄까지만 검색 하도록!

		for (/*nSearchX = VIRTUAL_POCKET_X_START - ( MAX_TEST_PP_PKR_CNT_X - 1 ) * nPitchType_X*/; nSearchX < VIRTUAL_POCKET_X_START + nPocketCntX; nSearchX++)
		{
			nCurrentCnt = 0;

			memset(nArrHandWorkSource, 0, sizeof(nArrHandWorkSource));

			for (nSearchHandX = 0; nSearchHandX < MAX_TEST_PP_PKR_CNT_X; nSearchHandX++)
			{
				for (nSearchHandY = 0; nSearchHandY < MAX_TEST_PP_PKR_CNT_Y; nSearchHandY++)
				{
					int nYPocket = 0, nXPocket = 0;
					nYPocket = nSearchY + nSearchHandY * nPitchType_Y;
					nXPocket = nSearchX + nSearchHandX * nPitchType_X;

					if (nTableDvc[nXPocket][nYPocket] == nTableData
						&&  nPickerPattern[nSearchHandX][nSearchHandY] == nHadData)
					{
						if (bPickPlace == DEF_PICK && g_TaskSystemCtrl.GetAutoGrrUse() == TRUE)
						{
							stData.clear();

							//수정 필요
							switch (nLocIdx)
							{

							case LOC_IDX_TEST_PP_TBL_1_LD:
							case LOC_IDX_TEST_PP_TBL_2_LD:
							case LOC_IDX_TEST_PP_TBL_3_LD:
							case LOC_IDX_TEST_PP_TBL_4_LD:
							{
								nDeviceState = nLocIdx - LOC_IDX_TEST_PP_TBL_1_LD;

								g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1 + nDeviceState, nXPocket - VIRTUAL_POCKET_X_START, nYPocket - VIRTUAL_POCKET_Y_START, &stData);

							}break;
							case LOC_IDX_CLEAN_TBL:
								g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1_CLEAN_BUFF + m_eTestIdx, nXPocket - VIRTUAL_POCKET_X_START, nYPocket - VIRTUAL_POCKET_Y_START, &stData);
								break;
							default:
								break;
							}

							BOOL bErr = stData.stGrrTestInfo[m_vModuleSetPressIdx[m_nJobAtsIdx]].chkTestEnd(/*nSearchHandX, m_nJobSiteRow*/);
							if (bErr == TRUE && m_nCleanMode == FALSE)
								continue;


						}

						nArrHandWorkSource[nSearchHandX][nSearchHandY] = DEF_WORK_RESERVED;
						nCurrentCnt++;
					}
				}
			}


			if (nCurrentCnt > nTotalPossibleCnt)
			{
				//작업해라.
				nTotalPossibleCnt = nCurrentCnt;
				memcpy_s(nArrHandWorkCopy, sizeof(nArrHandWorkCopy), nArrHandWorkSource, sizeof(nArrHandWorkSource));
				nCXPosition = nSearchX;
				nCYPosition = nSearchY;
			}
		}
	}


	memcpy_s(nWorkPkr, sizeof(nArrHandWorkCopy), nArrHandWorkCopy, sizeof(nArrHandWorkCopy));

	//0번 피커부터 할당되지 않은 포켓은 트림한다.
	int nPockOffsetX = 0;
	int nPockOffsetY = 1;

	//X방향 첫 할당 위치가 X옵셋
	for (; nPockOffsetX < MAX_TEST_PP_PKR_CNT_X; nPockOffsetX++)
	{
		if (nWorkPkr[nPockOffsetX][0] != DEF_WORK_FREE) break;
		if (nWorkPkr[nPockOffsetX][1] != DEF_WORK_FREE) break;
	}
	//X방향 첫 할당 위치가 1개라도 있으면 Y옵셋은 0
	for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
	{
		if (nWorkPkr[nX][0] != DEF_WORK_FREE) {
			nPockOffsetY = 0;
			break;
		}
	}

	if (nPockOffsetY == 1) {
		int a = 0;
	}

	nCXPosition -= VIRTUAL_POCKET_X_START;
	nCYPosition -= VIRTUAL_POCKET_Y_START;

	*n_Xpos = nCXPosition + nPockOffsetX*nPitchType_X;
	*n_YPos = nCYPosition + nPockOffsetY*nPitchType_Y;

	return nTotalPossibleCnt;
}

/*=============================================================================================
DESCRIPT : Clean Table 1:1 Pick 알고리즘
KEY WORD : 1Station당 8Para/4Para
ARGUMENT : 
RETURN   : 
===============================================================================================*/
int CTaskAutoTestPp::FindPickPlaceFromCleanTable(int nTableDvc[VIRTUAL_POCKET_X_MAX][VIRTUAL_POCKET_Y_MAX], int nPickerPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], BOOL bPickPlace, int *n_Xpos, int *n_YPos, int nWorkPkr[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y])
{
	//ASSERT( nLocIdx == LOC_IDX_CLEAN_BUFFER || nLocIdx == LOC_IDX_ART_BUFFER || nLocIdx == LOC_IDX_TEST_PP_LD_TBL );

	int nSearchY = 0, nSearchX = 0;
	int nSearchHandY = 0, nSearchHandX = 0;

	/*DEF_WORK_FREE or DEF_WORK_RESERVED*/
	int nArrHandWorkSource[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = { DEF_WORK_FREE, };
	int nArrHandWorkCopy[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = { DEF_WORK_FREE, };

	int nPocketCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_X); 
	int nPocketCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_Y);

	int nTotalPossibleCnt = 0;	// 가장 Pick 할 수 있는 수량
	int nCurrentCnt = 0;	// 현재 까지 Pick 할수 있는 수량
	int nCXPosition = 0, nCYPosition = 0;
	// 1Y, 2Y는 계산 한 대로 가져 온다.	

	int nPitchType_X = g_TaskTestPP[m_eTestIdx].GetHandPitchType(CTaskTestPP::eAxisPitch_HW_X1_SW_Y1, LOC_IDX_CLEAN_TBL);	// Hand와 Target간의 간격 관계 (0=1by1, 1=1:1매칭, 2=한칸뛰기,3=두칸뛰기), Hand 기준임. (Hand 1,3으로 Target 1,2 집는것은 처리 필요)
	int nPitchType_Y = g_TaskTestPP[m_eTestIdx].GetHandPitchType(CTaskTestPP::eAxisPitch_HW_Y1_SW_X1, LOC_IDX_CLEAN_TBL);	// Hand와 Target간의 간격 관계 (0=1by1, 1=1:1매칭, 2=한칸뛰기,3=두칸뛰기), Hand 기준임. (Hand 1,3으로 Target 1,2 집는것은 처리 필요)

	int nChkCleanBuffer = DEF_NOT_EXIST;
	int nHadData = DEF_NOT_EXIST;
	if (bPickPlace == DEF_PICK) {	// Pick 일 때 조건
		nChkCleanBuffer = DEF_EXIST;
		nHadData = DEF_NOT_EXIST;
	}
	else {							// Place 일 때 조건		
		nChkCleanBuffer = DEF_NOT_EXIST;
		nHadData = DEF_EXIST;
	}


	nSearchY = VIRTUAL_POCKET_Y_START;
	nSearchX = VIRTUAL_POCKET_X_START;

	ST_DD_DEVICE stData;
	for (; nSearchY <= VIRTUAL_POCKET_Y_START; nSearchY++)
	{
		for (; nSearchX <= VIRTUAL_POCKET_X_START; nSearchX++)
		{
			nCurrentCnt = 0;

			memset(nArrHandWorkSource, 0, sizeof(nArrHandWorkSource));

			for (nSearchHandX = 0; nSearchHandX < MAX_TEST_PP_PKR_CNT_X; nSearchHandX++)
			{
				for (nSearchHandY = 0; nSearchHandY < MAX_TEST_PP_PKR_CNT_Y; nSearchHandY++)
				{
					int nYPocket = 0, nXPocket = 0;
					nYPocket = nSearchY + nSearchHandY * nPitchType_Y;
					nXPocket = nSearchX + nSearchHandX * nPitchType_X;

					if (nTableDvc[nXPocket][nYPocket] == nChkCleanBuffer
						&&  nPickerPattern[nSearchHandX][nSearchHandY] == nHadData)
					{
						nArrHandWorkSource[nSearchHandX][nSearchHandY] = DEF_WORK_RESERVED;
						nCurrentCnt++;



// 						if (bPickPlace == DEF_PICK && g_TaskSystemCtrl.GetAutoGrrUse() == TRUE)
// 						{
// 							stData.clear();
// 
// 							//수정 필요
// 							switch (nLocIdx)
// 							{
// 
// 							case LOC_IDX_TEST_PP_TBL_1_LD:
// 							case LOC_IDX_TEST_PP_TBL_2_LD:
// 							case LOC_IDX_TEST_PP_TBL_3_LD:
// 							case LOC_IDX_TEST_PP_TBL_4_LD:
// 							{
// 								int nDeviceState = nLocIdx - LOC_IDX_TEST_PP_TBL_1_LD;
// 
// 								g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1 + nDeviceState, nXPocket - VIRTUAL_POCKET_X_START, nYPocket - VIRTUAL_POCKET_Y_START, &stData);
// 
// 							}break;
// 							case LOC_IDX_CLEAN_TBL:
// 								g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1_CLEAN_BUFF + m_eTestIdx, nXPocket - VIRTUAL_POCKET_X_START, nYPocket - VIRTUAL_POCKET_Y_START, &stData);
// 								break;
// 							default:
// 								break;
// 							}
// 
// 							BOOL bErr = stData.stGrrTestInfo[m_vModuleSetPressIdx[m_nJobAtsIdx]].chkTestEnd(/*nSearchHandX, m_nJobSiteRow*/);
// 							if (bErr == TRUE && m_nCleanMode == FALSE)
// 								continue;
// 
// 
// 						}

// 						nArrHandWorkSource[nSearchHandX][nSearchHandY] = DEF_WORK_RESERVED;
// 						nCurrentCnt++;
					}
				}
			}


			if (nCurrentCnt > nTotalPossibleCnt)
			{
				//작업해라.
				nTotalPossibleCnt = nCurrentCnt;
				memcpy_s(nArrHandWorkCopy, sizeof(nArrHandWorkCopy), nArrHandWorkSource, sizeof(nArrHandWorkSource));
				nCXPosition = nSearchX;
				nCYPosition = nSearchY;
			}
		}
	}


	memcpy_s(nWorkPkr, sizeof(nArrHandWorkCopy), nArrHandWorkCopy, sizeof(nArrHandWorkCopy));

	//0번 피커부터 할당되지 않은 포켓은 트림한다.
	int nPockOffsetX = 0;
	int nPockOffsetY = 1;

	//X방향 첫 할당 위치가 X옵셋
	for (; nPockOffsetX < MAX_TEST_PP_PKR_CNT_X; nPockOffsetX++)
	{
		if (nWorkPkr[nPockOffsetX][0] != DEF_WORK_FREE) break;
		if (nWorkPkr[nPockOffsetX][1] != DEF_WORK_FREE) break;
	}
	//X방향 첫 할당 위치가 1개라도 있으면 Y옵셋은 0
	for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
	{
		if (nWorkPkr[nX][0] != DEF_WORK_FREE) {
			nPockOffsetY = 0;
			break;
		}
	}

	nCXPosition -= VIRTUAL_POCKET_X_START;
	nCYPosition -= VIRTUAL_POCKET_Y_START;

	*n_Xpos = nCXPosition + nPockOffsetX*nPitchType_X;
	*n_YPos = nCYPosition + nPockOffsetY*nPitchType_Y;

	return nTotalPossibleCnt;



}

/*=============================================================================================
  DESCRIPT : FillDvc_VirtualTable 
  KEY WORD : loadtable, ART buffer table, Socket Cleaning Table 의 device 정보
  ARGUMENT : nDvcStageIdx       = device stage 정보
             pnBuffer           = buffer table device 정보
			 nBufSizeX          = Buffer size max X
			 nBufSizeY          = Buffer size max Y
			 bPickPlace         = Pick / Place 동작
  RETURN   : 없음
 ===============================================================================================*/
int CTaskAutoTestPp::FillDvc_VirtualTable(int nDvcStageIdx, int* pnBuffer, int nBufSizeX, int nBufSizeY, int bPickPlace, int nAtsIdx, int nTableType)
{
	int nPocketCount_X = 0;//g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_X);
	int nPocketCount_Y = 0;//g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_Y);

	switch( nDvcStageIdx )
	{
	case eDeviceStage_TEST_PP_1_CLEAN_BUFF:
	case eDeviceStage_TEST_PP_2_CLEAN_BUFF:
		{
			nPocketCount_X = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_X);
			nPocketCount_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_Y);
		}break;
	case eDeviceStage_LD_TBL_1:
	case eDeviceStage_LD_TBL_2:
	case eDeviceStage_LD_TBL_3:
	case eDeviceStage_LD_TBL_4:
		{
			nPocketCount_X = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
			nPocketCount_Y = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);
		}break;
	}

	ST_DD_DEVICE stDevice;
	for (int nX = 0; nX < nPocketCount_X; nX++)
	{
		for(int nY=0; nY<nPocketCount_Y; nY++)
		{		
			stDevice.clear();
			int nBuffer_Index = (nX+VIRTUAL_POCKET_X_START)*nBufSizeY/*X*/ + (nY+VIRTUAL_POCKET_Y_START)/*Y*/;			

			g_DMCont.m_dmDevice.GetDeviceData(nDvcStageIdx, nX, nY, &stDevice);
			if (stDevice.sExist == DEF_EXIST) {
				pnBuffer[nBuffer_Index] = DEF_EXIST;
			}
			else {
				pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
			}

			// Load table의 Y열이 1,2열은 UD, 3,4열이 LD이므로 해당 부분은 작업 못하도록 해야한다.
			if (nTableType == DEF_TABLE_TYPE_LOAD_UNLOAD)
			{
				char setting = g_TaskLoadTbl[nDvcStageIdx - eDeviceStage_LD_TBL_1].m_LoadTbl_pocket_setting[nY][nX];

				if (nDvcStageIdx == eDeviceStage_LD_TBL_1 || nDvcStageIdx == eDeviceStage_LD_TBL_4)
				{			
					if (bPickPlace == DEF_PICK && setting == 'B')
						pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
					if (bPickPlace == DEF_PLACE && setting == 'B')
						pnBuffer[nBuffer_Index] = DEF_EXIST;
				}
				else if (nDvcStageIdx == eDeviceStage_LD_TBL_2 || nDvcStageIdx == eDeviceStage_LD_TBL_3)
				{
					if (bPickPlace == DEF_PICK && setting == 'B')
						pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
					if (bPickPlace == DEF_PLACE && setting == 'B')
						pnBuffer[nBuffer_Index] = DEF_EXIST;
				}


 				if (nDvcStageIdx == eDeviceStage_LD_TBL_1 || nDvcStageIdx == eDeviceStage_LD_TBL_4)
 				{
					if (bPickPlace == DEF_PICK && (nY == 0 || nY == 2))
 						pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
					
//					if(bPickPlace == DEF_PLACE && (nY == 0 || nY == 2))
//						pnBuffer[nBuffer_Index] = DEF_EXIST;

 					// 				if (bPickPlace == DEF_PLACE && nY >= (nPocketCount_Y / 2))
 					// 					pnBuffer[nBuffer_Index] = DEF_EXIST;
 					// 				if (bPickPlace == DEF_PICK && nY < (nPocketCount_Y / 2))
 					// 					pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
 				}
 				else if (nDvcStageIdx == eDeviceStage_LD_TBL_2 || nDvcStageIdx == eDeviceStage_LD_TBL_3)
 				{
 					if (bPickPlace == DEF_PLACE && (nY == 0 || nY == 2))
 						pnBuffer[nBuffer_Index] = DEF_EXIST;

//					if(bPickPlace == DEF_PICK && (nY == 0 || nY == 2))
//						pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
 				}
			}
			else {
				// auto retest buffer의 경우 work pos가 아니면 안된다.
				if (nDvcStageIdx >= eDeviceStage_LD_TBL_1 && nDvcStageIdx <= eDeviceStage_LD_TBL_4)
				{
					if (bPickPlace == DEF_PLACE && g_TaskLoadTbl[nDvcStageIdx - eDeviceStage_LD_TBL_1].GetBufferPocketCheck(nX, nY) != TRUE)
					{
						pnBuffer[nBuffer_Index] = DEF_EXIST;
						continue;
					}
					if (bPickPlace == DEF_PICK && g_TaskLoadTbl[nDvcStageIdx - eDeviceStage_LD_TBL_1].GetBufferPocketCheck(nX, nY) != TRUE)
					{
						pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
						continue;
					}
					

					BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
					if (bAutoGrrUse == DEF_OFF)
					{
						if (bPickPlace == DEF_PICK && stDevice.sExist == DEF_EXIST)
						{
							if (((atoi(stDevice.szTestSiteNum) == m_vModuleSetPressIdx[nAtsIdx] && strcmp(stDevice.szBinResult, DEF_NEW_LOAD_DEVICE) != 0) || stDevice.nTestingCnt == eARB_1stTestEnd)) {
								pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
							}

							BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stDevice);

							if (stDevice.nBinColor == eFailBin && (strcmp(stDevice.szErrString, DEF_TEST_RESULT_TP101) == 0 || bIsDefaultBin == TRUE)
								&& (atoi(stDevice.szTestSiteNum) == m_vModuleSetPressIdx[nAtsIdx])) {
								pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
							}

							if (stDevice.nBinColor == eFailBin && (strcmp(stDevice.szErrString, DEF_TEST_RESULT_TP101) == 0 || bIsDefaultBin == TRUE)
								&& (atoi(stDevice.szTestSiteNum) != m_vModuleSetPressIdx[nAtsIdx])) {
								pnBuffer[nBuffer_Index] = DEF_EXIST;
							}

						}
					}
					else {
						if (bAutoGrrUse == DEF_ON) {
							//[12/4/2023 dohyeong.kim] Art Buffer 1ea 사용하기 위해 Art Buffer(Unload Table) 무조건 Pick 
							
							//하나만 넣고 이전꺼 동일하면 다 Exist로
							//BOOL bTestEnd = stDevice.stGrrTestInfo[m_vModuleSetPressIdx[nAtsIdx]].chkTestEnd();
							
							if (bPickPlace == DEF_PICK) {
								int nTestDoneCnt = stDevice.stGrrTestInfo[m_vModuleSetPressIdx[nAtsIdx]].chkTestDone();
								if (nTestDoneCnt == 0)
									pnBuffer[nBuffer_Index] = DEF_EXIST;
								else
									pnBuffer[nBuffer_Index] = DEF_NOT_EXIST; // Test 완료 Cnt 0 아니면, Swap 진행 중.		
							}
						}
					}
				}
			}
		}
	}

	return ERR_NO_ERROR;
}

int CTaskAutoTestPp::FillDvcGrr_VirtualTable(int nDvcStageIdx, int* pnBuffer, int nBufSizeX, int nBufSizeY, int bPickPlace, int nAtsIdx/* = -1*/, int nTableType/* = -1*/)
{
	int nPocketCount_X = 0;//g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_X);
	int nPocketCount_Y = 0;//g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_Y);

	switch (nDvcStageIdx)
	{
	case eDeviceStage_TEST_PP_1_CLEAN_BUFF:
	case eDeviceStage_TEST_PP_2_CLEAN_BUFF:
	{
		nPocketCount_X = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_X);
		nPocketCount_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_Y);
	}break;
	case eDeviceStage_LD_TBL_1:
	case eDeviceStage_LD_TBL_2:
	case eDeviceStage_LD_TBL_3:
	case eDeviceStage_LD_TBL_4:
	{
		nPocketCount_X = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
		nPocketCount_Y = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);
	}break;
	}


	ST_DD_DEVICE stDevice;
	for (int nX = 0; nX < nPocketCount_X; nX++)
	{
		for (int nY = 0; nY < nPocketCount_Y; nY++)
		{
			stDevice.clear();
			int nBuffer_Index = (nX + VIRTUAL_POCKET_X_START)*nBufSizeY/*X*/ + (nY + VIRTUAL_POCKET_Y_START)/*Y*/;

			g_DMCont.m_dmDevice.GetDeviceData(nDvcStageIdx, nX, nY, &stDevice);
			if (stDevice.sExist == DEF_EXIST) {
				pnBuffer[nBuffer_Index] = DEF_EXIST;
			}
			else {
				pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
			}

			// Load table의 Y열이 1,2열은 UD, 3,4열이 LD이므로 해당 부분은 작업 못하도록 해야한다.
			if (nTableType == DEF_TABLE_TYPE_LOAD_UNLOAD)
			{
				char setting = g_TaskLoadTbl[nDvcStageIdx - eDeviceStage_LD_TBL_1].m_LoadTbl_pocket_setting[nY][nX];

				if (nDvcStageIdx == eDeviceStage_LD_TBL_1 || nDvcStageIdx == eDeviceStage_LD_TBL_4)
				{
					if (bPickPlace == DEF_PICK && setting == 'B')
						pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
					if (bPickPlace == DEF_PLACE && setting == 'B')
						pnBuffer[nBuffer_Index] = DEF_EXIST;
				}
				else if (nDvcStageIdx == eDeviceStage_LD_TBL_2 || nDvcStageIdx == eDeviceStage_LD_TBL_3)
				{
					if (bPickPlace == DEF_PICK && setting == 'B')
						pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
					if (bPickPlace == DEF_PLACE && setting == 'B')
						pnBuffer[nBuffer_Index] = DEF_EXIST;
				}

				//밑에 필요 없음
				if (nDvcStageIdx == eDeviceStage_LD_TBL_1 || nDvcStageIdx == eDeviceStage_LD_TBL_4)
				{
					if (bPickPlace == DEF_PICK && (nY == 0 || nY == 2))
						pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
				}
				else if (nDvcStageIdx == eDeviceStage_LD_TBL_2 || nDvcStageIdx == eDeviceStage_LD_TBL_3)
				{
					if (bPickPlace == DEF_PLACE && (nY == 0 || nY == 2))
						pnBuffer[nBuffer_Index] = DEF_EXIST;
				}
			}
			else {
				// auto retest buffer의 경우 work pos가 아니면 안된다.
				if (nDvcStageIdx >= eDeviceStage_LD_TBL_1 && nDvcStageIdx <= eDeviceStage_LD_TBL_4)
				{
					if (bPickPlace == DEF_PLACE && g_TaskLoadTbl[nDvcStageIdx - eDeviceStage_LD_TBL_1].GetBufferPocketCheck(nX, nY) != TRUE)
					{
						pnBuffer[nBuffer_Index] = DEF_EXIST;
						continue;
					}
					if (bPickPlace == DEF_PICK && g_TaskLoadTbl[nDvcStageIdx - eDeviceStage_LD_TBL_1].GetBufferPocketCheck(nX, nY) != TRUE)
					{
						pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
						continue;
					}


						//하나만 넣고 이전꺼 동일하면 다 Exist로
						//BOOL bTestEnd = stDevice.stGrrTestInfo[m_vModuleSetPressIdx[nAtsIdx]].chkTestEnd();

					if (bPickPlace == DEF_PICK) {
						int nTestDoneCnt = stDevice.stGrrTestInfo[m_vModuleSetPressIdx[nAtsIdx]].chkTestDone();
						if (nTestDoneCnt == 0) {
							pnBuffer[nBuffer_Index] = DEF_EXIST;
						}
						else {
							if (nTestDoneCnt == STATION_MAX_PARA) {
								pnBuffer[nBuffer_Index] = DEF_EXIST;
							}
							else									pnBuffer[nBuffer_Index] = DEF_NOT_EXIST; // Test 완료 Cnt 0 아니면, Swap 진행 중.		
						}

 // 						if (bTestEnd == TRUE && g_TaskSystemCtrl.GetAutoGrrCancel() == FALSE)
 // 						{
 // 							pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
 // 						}
 // 						if (g_TaskSystemCtrl.GetAutoGrrCancel() == FALSE)
 // 						{
 // 							if (bTestEnd == FALSE) {
 // 								if (nTestDoneCnt == 0)
 // 									pnBuffer[nBuffer_Index] = DEF_EXIST;
 // 								else
 // 									pnBuffer[nBuffer_Index] = DEF_NOT_EXIST; // Test 완료 Cnt 0 아니면, Swap 진행 중.				
 // 							}
 // 							else {
 // 								pnBuffer[nBuffer_Index] = DEF_EXIST;
 // 							}
 // 
 // 						}
					}				
				}
			}
		}
	}

	return ERR_NO_ERROR;
}

int CTaskAutoTestPp::FillDvcGrr_VirtualStation(int nDvcStageIdx, int* pnBuffer, int nBufSizeX, int nBufSizeY, int bPickPlace)
{
	int nPocketCount_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nPocketCount_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

//	int nCleanMode = ChkCleaningMode(m_nJobAtsIdx);

	ST_DD_DEVICE stSite;
	for (int nY = 0; nY < nPocketCount_Y; nY++)
	{
		for (int nX = 0; nX < nPocketCount_X; nX++)
		{
			stSite.clear();
			int nBuffer_Index = (nX + VIRTUAL_POCKET_X_START)*nBufSizeY/*X*/ + (nY + VIRTUAL_POCKET_Y_START)/*Y*/;
			g_DMCont.m_dmDevice.GetDeviceData(nDvcStageIdx, nX, nY, &stSite);

			if (stSite.sExist == DEF_EXIST)
				pnBuffer[nBuffer_Index] = DEF_EXIST;
			else
				pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;

			if (bPickPlace == DEF_PICK && stSite.sExist == DEF_EXIST)
			{
				if (stSite.nTestingInfo == eTestSite_DvcLoading || stSite.nTestingInfo == eTestSite_CleanLoading)
 					pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
// 
// 				// Manual Socket Cleanning 스케쥴이 걸리면 AutoRetest AA Pick/Place Mode가 아닐 때는 A나 AA 자재는 잠시 빼줘야 된다. [9/6/2021 donghyun.shin]
// 				if (nCleanMode == TRUE && stSite.nTestingInfo == eTestSite_DvcLoading && bAtoAPickPlace != eOPTION_USE)
// 					pnBuffer[nBuffer_Index] = DEF_EXIST;
// 
 			}
		}
	}

	return ERR_NO_ERROR;
}

int CTaskAutoTestPp::FillDvcGrr_VirtualTable_Swap(int nDvcStageIdx, int* pnBuffer, int nBufSizeX, int nBufSizeY, int bPickPlace, int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nAtsIdx)
{
	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nMAX_STATION_PARA = STATION_MAX_PARA;
	int nMAX_TEST_PP_PKR_CNT_X = MAX_TEST_PP_PKR_CNT_X;
	int nMAX_TEST_PP_PKR_CNT_Y = MAX_TEST_PP_PKR_CNT_Y;
	if (nSiteDivY != 2) {
		nMAX_STATION_PARA = STATION_MAX_PARA / 2;
		nMAX_TEST_PP_PKR_CNT_Y = MAX_TEST_PP_PKR_CNT_Y / 2;
	}


	int nPocketCount_X = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	int nPocketCount_Y = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

	ST_DD_DEVICE stDevice;
	int nWorkPkrX = 0;
	int nWorkPkrY = 0;


	//Table 초기화
	for (int nY = 0; nY < nPocketCount_Y; nY++)
	{
		for (int nX = 0; nX < nPocketCount_X; nX++)
		{
			int nBuffer_Index = (nX + VIRTUAL_POCKET_X_START)*nBufSizeY/*X*/ + (nY + VIRTUAL_POCKET_Y_START)/*Y*/;
			pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;

// 			g_DMCont.m_dmDevice.GetDeviceData(nDvcStageIdx, nX, nY, &stDevice);
// 			if (stDevice.sExist == DEF_EXIST) pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
// 			else                               pnBuffer[nBuffer_Index] = DEF_EXIST;
		}
	}

	//Find Picker
	int nCntPkr = 0;
	for (int nY = 0; nY < nMAX_TEST_PP_PKR_CNT_Y; nY++)
	{
		for(int nX = 0; nX < nMAX_TEST_PP_PKR_CNT_X; nX++)
		{		
			if (nPickerDvcPattern[nX][nY] == DEF_NOT_EXIST) //Work Picker
			{
				nCntPkr++;
				nWorkPkrX = nX;
				nWorkPkrY = nY;
			}
		}
	}

	if (nCntPkr > 1) 
	{
		return eExcutingStepGrr_ErrorJob;
	}

	//nWorkPkrX, nWorkPkrY는 실제 Pick 진행 하기 위한 Picker Index
	int nWorkTblPocketX = 0;
	int nWorkTblPocketY = 0;

	if (nDvcStageIdx >= eDeviceStage_LD_TBL_1 && nDvcStageIdx <= eDeviceStage_LD_TBL_4)
	{
		for (int i = 0; i < MAX_TEST_PP_PKR_CNT; i++)
		{

			nWorkPkrX++;

			if (nWorkPkrX >= nMAX_TEST_PP_PKR_CNT_X) {
				nWorkPkrX = 0;

				//4Para일 경우, nWorkPkrY는 0만 존재.
				if (nSiteDivY == 2) {
					if (nWorkPkrY == 1)		nWorkPkrY = 0;
					else					nWorkPkrY = 1;
				}
			}

			if (nWorkPkrX >= nMAX_TEST_PP_PKR_CNT_X || nWorkPkrY >= nMAX_TEST_PP_PKR_CNT_Y)
				return eExcutingStepGrr_ErrorJob;

			nWorkTblPocketX = nWorkPkrX; //0,1,2,3
			nWorkTblPocketY = nWorkPkrY; //0,1

			g_TaskLoadTbl[nDvcStageIdx - eDeviceStage_LD_TBL_1].GrrConvertWorkPkrToPocket(nWorkTblPocketX, nWorkTblPocketY);

			g_DMCont.m_dmDevice.GetDeviceData(nDvcStageIdx, nWorkTblPocketX, nWorkTblPocketY, &stDevice);


			//Pick 하려고 하는 Device Station Test End 확인.
			if (stDevice.sExist == DEF_EXIST)
			{
				BOOL bTestEnd = stDevice.stGrrTestInfo[m_vModuleSetPressIdx[nAtsIdx]].chkTestEnd(nWorkPkrX, nWorkPkrY);
				int nTestDoneCnt = stDevice.stGrrTestInfo[m_vModuleSetPressIdx[nAtsIdx]].chkTestDone();

				//이전 test 진행 완료 & 전체 Test Count 8개 아님.
				if (bTestEnd == TRUE && nTestDoneCnt != nMAX_STATION_PARA) {
					int nBuffer_Index = (nWorkTblPocketX + VIRTUAL_POCKET_X_START)*nBufSizeY/*X*/ + (nWorkTblPocketY + VIRTUAL_POCKET_Y_START)/*Y*/;
					pnBuffer[nBuffer_Index] = DEF_EXIST;
					return ERR_NO_ERROR;
				}

				if (nTestDoneCnt == nMAX_STATION_PARA) {
					return eExcutingStepGrr_ErrorJob;
				}
			}

		}
	}

	return eExcutingStepGrr_ErrorJob;
}

/*=============================================================================================
  DESCRIPT : GetDvcCleanHandAssign
  KEY WORD : Socket Cleaning 용 test hand assign
  ARGUMENT : bPickPlace         = Pick / Place 동작
             nPickerDvcPattern  = hand 작업 가능 패턴
			 nWorkingAreaIdx    = 작업 할 Station Area Index
			 nStationWorkRow    = 작업 할 Station Area 의 row			 
  RETURN   : 작업 가능 picker 수
 ===============================================================================================*/
int CTaskAutoTestPp::GetDvcCleanHandAssign(BOOL bPickPlace, int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nWorkingAreaIdx )
{
	int nPkrPossibleCnt = 0;
	int nSiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	if( bPickPlace == DEF_PICK )
	{
		int nSocketDvc[STATION_MAX_X_SITE][STATION_MAX_Y_SITE] = { 0, };
		int nPkrDvc[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = { 0, };

		// site의 socket on/off 정보.
		ST_DD_DEVICE stDvc;
		for(int nX=0; nX < nSiteCntX; nX++ )
		{
			for (int nY = 0; nY < nSiteCntY; nY++)
			{
				int nOnOff = g_TaskPressUnit[m_vModuleSetPressIdx[nWorkingAreaIdx]].GetSocketOnOff(nX, nY);
				if (nOnOff == DEF_SITE_ON)
				{
					stDvc.clear();
					g_TaskPressUnit[m_vModuleSetPressIdx[nWorkingAreaIdx]].GetSiteDeviceData(&stDvc, nX, nY);

					// cleaning device 가 들어있는 곳은 이미 넣은 곳이므로 집지 않는다.
					if (stDvc.sExist == DEF_EXIST && strcmp(stDvc.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0)
						nSocketDvc[nX][nY] = DEF_OFF;	// 불가능
					else
						nSocketDvc[nX][nY] = DEF_ON;	// 가능.
				}
				else {
					nSocketDvc[nX][nY] = DEF_OFF;	// 불가능
				}
			}
		}
		// load picker 정보
		for(int nX=0; nX < MAX_TEST_PP_PKR_CNT_X; nX++ )
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{
				stDvc.clear();
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY, &stDvc);
				nPkrDvc[nX][nY] = stDvc.sExist;
			}
		}
		// 사용 가능한 picker 정보		
		for(int nX=0; nX < MAX_TEST_PP_PKR_CNT_X; nX++ )
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{
				if (nSocketDvc[nX][nY] == DEF_ON && nPkrDvc[nX][nY] == DEF_NOT_EXIST) {
					nPickerDvcPattern[nX][nY] = DEF_NOT_EXIST; // 사용가능
					nPkrPossibleCnt++;
				}
			}
		}
	}else{
		ST_DD_DEVICE stDvc;
		// load picker 정보
		for(int nX=0; nX < MAX_TEST_PP_PKR_CNT_X; nX++ )
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{
				stDvc.clear();
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY, &stDvc);

				if (stDvc.sExist == DEF_EXIST && strcmp(stDvc.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0) {
					nPickerDvcPattern[nX][nY] = DEF_EXIST;	// 사용가능
					nPkrPossibleCnt++;
				}
			}
		}
	}

	return nPkrPossibleCnt;
}

int CTaskAutoTestPp::CheckJob_CleanBuff( int nAtsIdx, BOOL bPickPlace, CPoint &vPocket, std::vector<CPoint> &vPicker )
{
	CString strErrMsg = _T("");
	std::vector<CPoint> vErrPicker; vErrPicker.clear();
	std::vector<CString> vErrstring; vErrstring.clear();
	int nHandType = 0;
	int nErr = ChkDevice(strErrMsg, nHandType, vErrstring);
	if (nErr != ERR_NO_ERROR) {
		g_TaskSystemCtrl.SetErrTestPpIdx(m_eTestIdx, 1);
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Test PP %d %s", m_eTestIdx + 1, strErrMsg);
		for (int i = 0; i < (int)vErrstring.size(); i++)
		{
			AlarmData.m_fnAddItemFormat("%s", vErrstring[i]);
		}
		g_Error.AlarmReport(nErr, AlarmData, &g_TaskSystemCtrl, g_TaskTestPP[m_eTestIdx].GetFailPicker(nHandType));
		g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
		return eExcutingStep_ErrorJob;
	}

	//int nChkInterlock = ChkInterlockAts(nAtsIdx);
	//if( nChkInterlock == eExcutingStep_ErrorJob )
	//	return eExcutingStep_ErrorJob;

	if(g_TaskSystemCtrl.IsOneCycleModeOn() == TRUE && bPickPlace == DEF_PICK)
		return eExcutingStep_ErrorJob;

	if( bPickPlace == DEF_PICK && m_nCleanMode == FALSE )
		return eExcutingStep_ErrorJob;
	else
	{
		vPicker.clear();
		
		int nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

		// pick일 때만 site를 보고 확인하면 된다.
		if( bPickPlace == DEF_PLACE )
			nSiteCntY = 1;

		//for(int nY=0; nY < nSiteCntY; nY++ )
		{
			// find available hand picker
			int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y];

			for( int nPkrY=0; nPkrY < MAX_TEST_PP_PKR_CNT_Y; nPkrY++ )
			{
				for( int nPkrX=0; nPkrX < MAX_TEST_PP_PKR_CNT_X; nPkrX++ )
				{
					nPickerDvcPattern[nPkrX][nPkrY] = (bPickPlace == DEF_PICK ? DEF_EXIST : DEF_NOT_EXIST);
				}
			}
			int nAvailableCount = GetDvcCleanHandAssign(bPickPlace, nPickerDvcPattern, nAtsIdx);

			if( nAvailableCount > 0 )
			{
				// get clean buffer table
				int nArrVirtualCleanBuff[VIRTUAL_POCKET_X_MAX][VIRTUAL_POCKET_Y_MAX];
				memset(nArrVirtualCleanBuff, 0xFF, sizeof(nArrVirtualCleanBuff));
				FillDvc_VirtualTable(eDeviceStage_TEST_PP_1_CLEAN_BUFF + m_eTestIdx, (int*)nArrVirtualCleanBuff, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, bPickPlace, nAtsIdx);

				int nUseJobPkr[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = {0,};
				int nPockX = -1, nPockY = -1;
				int nRetCode = FindPickPlaceFromTable(LOC_IDX_CLEAN_TBL, nArrVirtualCleanBuff, nPickerDvcPattern, bPickPlace, &nPockX, &nPockY, nUseJobPkr);
				if(nRetCode > 0){
					std::vector<CPoint> vPkr;
					for(int nPickX=0; nPickX<MAX_TEST_PP_PKR_CNT_X; nPickX++){
						for(int nPickY=0; nPickY<MAX_TEST_PP_PKR_CNT_Y; nPickY++){
							if(nUseJobPkr[nPickX][nPickY]!=0) {
								vPkr.push_back( CPoint(nPickX, nPickY) );
							}
						}
					}

					vPocket = CPoint(nPockX, nPockY);
					vPicker = vPkr;
					return eExcutingStep_FindJob;
				}
				else {
					if (bPickPlace == DEF_PICK) // Socket Clean Device가 Table에 없으면 Cleaning 예약 해제
					{
						BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
						BOOL bSiteCleanDvcIn = FALSE;
						for (auto Idx : m_vModuleSetPressIdx)
						{
							bSiteCleanDvcIn = g_TaskPressUnit[Idx].PressInCleanDeviceCheck(bAutoGrrUse);
							if (bSiteCleanDvcIn == TRUE)
								break;
						}
						
						BOOL bTestPPCleanDev =g_TaskTestPP[m_eTestIdx].GetTestPPCleanDevice();
						if (bTestPPCleanDev == FALSE && bSiteCleanDvcIn == FALSE)
						{
							BOOL bCleanOption = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_SocketClean);
							if (bCleanOption == eOPTION_USE)
							{						
								g_DMCont.m_dmEQP.SB(BDM0_SOCKETCLEAN_SITE1_RESERVE + m_vModuleSetPressIdx[nAtsIdx],FALSE);
								g_DMCont.m_dmHandlerOpt.SB(BDM11_MANU_CLN_REV_SITE1 + m_vModuleSetPressIdx[nAtsIdx],FALSE);
							}
						}
					}
				}
			}
		}
	}

	return eExcutingStep_ErrorJob;
}

int CTaskAutoTestPp::GetModuleSocketOnCnt(BOOL bGrrMode)
{
	int nOnCnt = 0;
	for(int i=0; i < (int)m_vModuleSetPressIdx.size(); i++ )
		nOnCnt += g_TaskPressUnit[ m_vModuleSetPressIdx[i] ].GetSocketOnCnt(bGrrMode);

	return nOnCnt;
}

int CTaskAutoTestPp::GetModuleAuditSocketOnCnt()
{
	int nOnCnt = 0;
	for (int i = 0; i < (int)m_vModuleSetPressIdx.size(); i++)
		nOnCnt += g_TaskPressUnit[m_vModuleSetPressIdx[i]].GetSocketAuditOnCnt();

	return nOnCnt;
}


int CTaskAutoTestPp::GetModuleGrrSocketOnCnt(BOOL bGrrMode)
{
	int nOnArrCnt[eMaxPressUnitCount];
	int nOnCnt = eMaxPressUnitCount;
	for (int i = 0; i < (int)m_vModuleSetPressIdx.size(); i++) {
		nOnArrCnt[i] = 0;
		nOnArrCnt[m_vModuleSetPressIdx[i]] = g_TaskPressUnit[m_vModuleSetPressIdx[i]].GetSocketOnCnt(bGrrMode);
		if (nOnArrCnt[m_vModuleSetPressIdx[i]] < nOnCnt && nOnArrCnt[m_vModuleSetPressIdx[i]] != 0)
			nOnCnt = nOnArrCnt[m_vModuleSetPressIdx[i]];
	}
		

	return nOnCnt;
}

BOOL CTaskAutoTestPp::ChkCleaningMode(int nChkAtsIdx)
{
	//임시
 	if (g_TaskSystemCtrl.GetAutoGrrUse() == TRUE)
 	{ 
 		int nOnCnt = g_TaskPressUnit[m_vModuleSetPressIdx[nChkAtsIdx]].GetSocketOnCntGrr();
 		if (nOnCnt <= 0)
 			return FALSE;
 
 		if (g_TaskSystemCtrl.GetAutoGrrCancel() == TRUE)
 			return FALSE;
 
//  		if (g_TaskSystemCtrl.checkUphMode() == true)
//  			return FALSE;
 
 		BOOL bCleanOption = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_SocketClean);
		BOOL bInitiStart = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_CleanInitStart);

 		if (bCleanOption == eOPTION_USE /*&& bInitiStart == TRUE*/)
 		{
			BOOL btest1 = g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE1_RESERVE);
			BOOL btest2 = g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE2_RESERVE);

			//[8/1/2024 dohyeong.kim] Socket Cleaning Initial만 진행.
			BOOL bCleaningRev = g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE1_RESERVE + m_vModuleSetPressIdx[nChkAtsIdx]);
			if (bCleaningRev == TRUE)
				return TRUE;

			BOOL bCleaningManuRev = g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE1 + m_vModuleSetPressIdx[nChkAtsIdx]);
			if (bCleaningManuRev == TRUE)
				return TRUE;
 		}
 	}
	else
	{
		int nOnCnt = g_TaskPressUnit[m_vModuleSetPressIdx[nChkAtsIdx]].GetSocketOnCnt();
		if (nOnCnt <= 0)
			return FALSE;

		if (g_TaskSystemCtrl.IsOneCycleModeOn() == TRUE)
			return FALSE;

		if (g_TaskSystemCtrl.checkUphMode() == true)
			return FALSE;

		BOOL bCleanOption = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_SocketClean);
		if (bCleanOption == eOPTION_USE)
		{
			BOOL bCleaningRev = g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE1_RESERVE + m_vModuleSetPressIdx[nChkAtsIdx]);

			if (bCleaningRev == TRUE)
				return TRUE;

			BOOL bCleaningManuRev = g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE1 + m_vModuleSetPressIdx[nChkAtsIdx]);
			if (bCleaningManuRev == TRUE)
				return TRUE;

		}
	}



	return FALSE;
}


int CTaskAutoTestPp::CheckJob_2dErrLoadTbl(CPoint & vPocket, std::vector<CPoint>& vPicker, BOOL bPickPlace)
{
	// Pick / Place에 따라서, 탐색 할 Load Table에 Index를 얻어 온다.
	int nLoadTableIdx = GetLoadTableInx(bPickPlace);
	std::vector<CPoint> vErrPicker; vErrPicker.clear();
	std::vector<CString> vErrstring; vErrstring.clear();
	CString strErrMsg = _T("");
	int nHandType = 0;
	int nErr = ChkDevice(strErrMsg, nHandType, vErrstring);
	if (nErr != ERR_NO_ERROR) {
		g_TaskSystemCtrl.SetErrTestPpIdx(m_eTestIdx, 1);
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Test PP %d %s", m_eTestIdx + 1, strErrMsg);
		for (int i = 0; i < (int)vErrstring.size(); i++)
		{
			AlarmData.m_fnAddItemFormat("%s", vErrstring[i]);
		}
		g_Error.AlarmReport(nErr, AlarmData, &g_TaskSystemCtrl, g_TaskTestPP[m_eTestIdx].GetFailPicker(nHandType));
		g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
		return eExcutingStep_ErrorJob;
	}

	BOOL b2dBarcodeUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use);
	if( b2dBarcodeUse == eOPTION_NOT_USE )
		return eExcutingStep_ErrorJob;

	// Loading table의 state를 점검한다.

	vPicker.clear();

	int nPkrPossibleCnt = 0;
	ST_DD_DEVICE stDvc;
	// find available hand picker
	int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y];

	for (int nPkrY = 0; nPkrY < MAX_TEST_PP_PKR_CNT_Y; nPkrY++)
	{
		for (int nPkrX = 0; nPkrX < MAX_TEST_PP_PKR_CNT_X; nPkrX++)
		{
			nPickerDvcPattern[nPkrX][nPkrY] = DEF_NOT_EXIST;

			stDvc.clear();
			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nPkrX, nPkrY, &stDvc);
			if (stDvc.sExist == DEF_EXIST && (strcmp(stDvc.szErrString, DEF_2D_READ_ERR_DEVICE) == 0 || strcmp(stDvc.szErrString, DEF_TEST_RESULT_WRONG_APN) == 0))
			{
				nPickerDvcPattern[nPkrX][nPkrY] = DEF_EXIST;	// 사용가능
				nPkrPossibleCnt++;
			}		
		}
	}

	int nArrVirtualTbl[VIRTUAL_POCKET_X_MAX][VIRTUAL_POCKET_Y_MAX];
	memset(nArrVirtualTbl, 0xFF, sizeof(nArrVirtualTbl));
	FillDvc_VirtualTable(eDeviceStage_LD_TBL_1 + nLoadTableIdx, (int*)nArrVirtualTbl, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, DEF_PLACE, -1, DEF_TABLE_TYPE_LOAD_UNLOAD);

	// get clean buffer table
	if (nPkrPossibleCnt > 0)
	{
		int nUseJobPkr[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = { 0, };
		int nPockX = -1, nPockY = -1;
		int nRetCode = FindPickPlaceFromTable(LOC_IDX_TEST_PP_TBL_1_LD+ nLoadTableIdx, nArrVirtualTbl, nPickerDvcPattern, DEF_PLACE, &nPockX, &nPockY, nUseJobPkr);
		if (nRetCode > 0) {
			std::vector<CPoint> vPkr;
			for (int nPickX = 0; nPickX<MAX_TEST_PP_PKR_CNT_X; nPickX++) {
				for (int nPickY = 0; nPickY<MAX_TEST_PP_PKR_CNT_Y; nPickY++) {
					if (nUseJobPkr[nPickX][nPickY] != 0) {
						vPkr.push_back(CPoint(nPickX, nPickY));
					}
				}
			}

			int nTblRes = g_TaskLoadTbl[nLoadTableIdx].GetWorkReserve();

			if (nTblRes == RID_INIT_IDLE_RESERVE || nTblRes == RID_NOT_RESERVE)
				return eExcutingStep_WaitOtherJob;
		
			if (g_TaskLoadTbl[nLoadTableIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
				return eExcutingStep_WaitOtherJob;
			

			vPocket = CPoint(nPockX, nPockY);
			vPicker = vPkr;
			return eExcutingStep_FindJob;
		}
	}

	return eExcutingStep_ErrorJob;
}



int CTaskAutoTestPp::ConvertPressIdx(int nIdx)
{
	return m_vModuleSetPressIdx[nIdx];
}

int CTaskAutoTestPp::ConvertLoadTableIdx(int nIdx)
{
	return m_vModuleSetLoadTableIdx[nIdx];
}
int CTaskAutoTestPp::ChkDevice(CString &strErrorMsg,int &rHandType, std::vector<CString>& vErrString)
{	
	int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
	if (nDeviceMode != eDeviceMode || g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
	{
		return ERR_NO_ERROR;
	}

	strErrorMsg.Empty();

	CString strErrorLossMsg = _T(""),strErrorMissMsg = _T("");

	std::vector<CPoint> vMisMatchPkr[eMaxTestPPTypeCount];
	for (int nHandType = 0; nHandType < eMaxTestPPTypeCount; nHandType++)
	{
		m_vLossPkr[nHandType].clear();
		vMisMatchPkr[nHandType].clear();
		g_TaskTestPP[m_eTestIdx].m_vFailPicker[nHandType].clear();
	}

	// Test PP는 Load , Unload Type 2개가 존재 하기 때문에, Load Type 우선 순위로 알람을 발생 시킨다.
	BOOL nHandTypeErrorComplete = FALSE;

	for (int nHandType = 0; nHandType < eMaxTestPPTypeCount && nHandTypeErrorComplete == FALSE; nHandType++)
	{
		for (int nPkrY = 0; nPkrY < MAX_TEST_PP_PKR_CNT_Y; nPkrY++)
		{
			for (int nPkrX = 0; nPkrX < MAX_TEST_PP_PKR_CNT_X; nPkrX++)
			{
				CString strPkr = _T("");
				ST_DD_DEVICE Dvc;
				int nDeviceExistData = g_TaskTestPP[m_eTestIdx].GetHandPkrDvcExist(CPoint(nPkrX, nPkrY), nHandType);
				int nExist = g_TaskTestPP[m_eTestIdx].ChkVacuum(DEF_ON, nHandType, CPoint(nPkrX, nPkrY));
				if (nExist != ERR_NO_ERROR && nDeviceExistData == eDvcExist) {
					//Device 있어야 되는데 없는 경우
					Dvc.clear();
					int nUnloadPkrY = nHandType == eTestPPUnLoadType ? TEST_UD_PKR_Y : TEST_LD_PKR_Y;
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nPkrX,  nPkrY + nUnloadPkrY, &Dvc);

					strPkr.Format("[%s][Barcode=%s]", (LPCTSTR)(g_TaskTestPP[m_eTestIdx].GetPickerName(CPoint(nPkrX, nPkrY))), Dvc.sz2DBarcode);
					vErrString.push_back(strPkr);
					m_vLossPkr[nHandType].push_back(CPoint(nPkrX, nPkrY));
					
					nHandTypeErrorComplete = TRUE;
				}
				else if (nExist == ERR_NO_ERROR && nDeviceExistData != eDvcExist) {
					//Device 없어야 되는데 있는 경우	
					strPkr.Format("%s", (LPCTSTR)(g_TaskTestPP[m_eTestIdx].GetPickerName(CPoint(nPkrX, nPkrY))));
					strErrorMissMsg += strPkr;
					vMisMatchPkr[nHandType].push_back(CPoint(nPkrX, nPkrY));
					nHandTypeErrorComplete = TRUE;
				}
			}
		}
	}

	for (int nHandType = 0; nHandType < eMaxTestPPTypeCount; nHandType++)
	{
		CString strPkr = _T("");
		if (m_vLossPkr[nHandType].size() > 0) {
			strPkr.Format("Picker Device Data is Exist, Device Vacuum Sensor is Not Exist");
		
			rHandType = nHandType;
			for (int i = 0; i < (int)m_vLossPkr[nHandType].size(); i++)
			{
				g_TaskTestPP[m_eTestIdx].SetFailPicker(CPoint(m_vLossPkr[nHandType][i].x, m_vLossPkr[nHandType][i].y), nHandType);
			}
			strErrorMsg += strPkr;
			return ERR_TEST_PP_1_Z1_DEVICE_MISS + g_TaskTestPP[m_eTestIdx].GetHandTypeIndex(nHandType);
		}		

		if (vMisMatchPkr[nHandType].size() > 0) {
			strPkr.Format("Picker Device Data is Not Exist, Device Vacuum Sensor is Exist");
			strErrorMissMsg += strPkr;
			rHandType = nHandType;
			for (int i = 0; i < (int)vMisMatchPkr[nHandType].size(); i++)
			{
				g_TaskTestPP[m_eTestIdx].SetFailPicker(CPoint(vMisMatchPkr[nHandType][i].x, vMisMatchPkr[nHandType][i].y), nHandType);
			}
			strErrorMsg = strErrorMissMsg;
			return ERR_TEST_PP_1_Z1_DEVICE_INFO_MISSMATCH + g_TaskTestPP[m_eTestIdx].GetHandTypeIndex(nHandType);
		}			
	}

 	return ERR_NO_ERROR;
}

void CTaskAutoTestPp::LossDevice(int nHandType)
{
	if (m_vLossPkr[nHandType].size() > 0) //m_nLossPicker != -1)
	{
		ST_DD_DEVICE stEmptyDevice; stEmptyDevice.clear();
		ST_DD_DEVICE stHandDevice; stHandDevice.clear();
		eDeviceType _eDeviceType = eDeviceType::_eDeviceType_Normal;
		for (int i = 0; i < (int)m_vLossPkr[nHandType].size(); i++)
		{
			//g_TaskTestPP[m_eTestIdx].SetDeviceData(m_nLossPicker, &stEmptyDevice);
			g_TaskTestPP[m_eTestIdx].GetHandDvcData(m_vLossPkr[nHandType][i], nHandType, &stHandDevice);
			if (stHandDevice.sExist == DEF_EXIST && strcmp(stHandDevice.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0)
			{
				_eDeviceType = eDeviceType::_eDeviceType_Cleaning;
			}


			g_TaskTestPP[m_eTestIdx].SetHandDvcData(m_vLossPkr[nHandType][i], nHandType, &stEmptyDevice);
		}

		//m_nLossPicker = -1;
		m_vLossPkr[nHandType].clear();

		//Clean Device Skip일 경우에는 OneCycle 진행 한다.
		if (_eDeviceType == eDeviceType::_eDeviceType_Cleaning)
		{
			g_TaskSystemCtrl.SetOneCycleMode(DEF_ON);
			g_TaskSystemCtrl.SetCleanDeviceSkipOneCycleMode(DEF_ON);
			MakeLog("[Clean Device Skip Set One Cycle]");
		}
	}
}

BOOL CTaskAutoTestPp::ModuleUseAbleCheck()
{
	//사용 유무에 따른 origin 확인
	if (m_eTestIdx == eTestPP_1) {
		int a = 0;
	}

	int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	BOOL bAutoUseGRR = FALSE;
	if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
		bAutoUseGRR = TRUE;

	BOOL bAutoUseAudit = FALSE;
	if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
		bAutoUseAudit = TRUE;

	if (g_TaskAutoTestPp[m_eTestIdx].GetModuleSocketOnCnt(bAutoUseGRR) > 0 || g_TaskAutoTestPp[m_eTestIdx].GetModuleAuditSocketOnCnt() > 0) {

		//Module에 사용 Site가 있으면 Test PP, Press, Load Table 확인
		if (g_TaskTestPP[m_eTestIdx].IsInitNeed() == TRUE) {
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_TEST_PP_1_MOTOR_ALARMED + m_eTestIdx, AlarmData, &g_TaskSystemCtrl);
			g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
			return FALSE;
		}


		for (int i = 0; i < eMaxATS_Area; i++) {
			int nPressIdx = g_TaskAutoTestPp[m_eTestIdx].ConvertPressIdx(i);
			if (g_TaskPressUnit[nPressIdx].IsInitNeed() == TRUE) {
				CParamList AlarmData;
				g_Error.AlarmReport(ERR_PRESS_UNIT_1_MOTOR_ALARM + i, AlarmData, &g_TaskSystemCtrl);
				g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
				return FALSE;
			}
		}

		
		for (auto nLoadTblIdx : m_vModuleSetLoadTableIdx)
		{
			if (g_TaskLoadTbl[nLoadTblIdx].IsInitNeed() == TRUE) {
				CParamList AlarmData;
				g_Error.AlarmReport(ERR_MOT_INTERLOCK_LOAD_TABLE_1_Y_AXIS_IS_INVALID + nLoadTblIdx, AlarmData, &g_TaskSystemCtrl);
				g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
				return FALSE;
			}
		}
		//사용할 Site 가 있고 Hand도 사용할수 있으면
		return TRUE;

	}
	else {
		//사용할 Site 가 없으면
		return FALSE;
	}
}

int CTaskAutoTestPp::GetLoadTableInx(BOOL bPickPlace)
{
	int nLoadTableIdx = -1;

	switch (m_eTestIdx)
	{
	case eTestPP_1: { (bPickPlace == DEF_PICK ? nLoadTableIdx = eLoadTbl_2 : nLoadTableIdx = eLoadTbl_1); }break;
	case eTestPP_2: { (bPickPlace == DEF_PICK ? nLoadTableIdx = eLoadTbl_3 : nLoadTableIdx = eLoadTbl_4); }break;
	default:
		break;
	}

	return nLoadTableIdx;
}

BOOL CTaskAutoTestPp::GetDvcStationCleanDevAssign(BOOL bPickPlace,int nWorkingAreaIdx)
{
	
	if (bPickPlace == DEF_PICK)
	{
		ST_DD_DEVICE stDvcSocket;
		int nPocketCount_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
		int nPocketCount_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

		for (int nX = 0; nX < nPocketCount_X; nX++)
		{
			for (int nY = 0; nY < nPocketCount_Y; nY++)
			{
				g_TaskPressUnit[m_vModuleSetPressIdx[nWorkingAreaIdx]].GetSiteDeviceData(&stDvcSocket, nX, nY);
				if (stDvcSocket.sExist == DEF_EXIST && strcmp(stDvcSocket.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0)
				{
					return TRUE;
				}
			}
		}
	}
	else
	{
		ST_DD_DEVICE stPkrDvc;
		for (int nPkrX = 0; nPkrX < MAX_TEST_PP_PKR_CNT_X; nPkrX++)
		{
			for (int nPkrY = 0; nPkrY < MAX_TEST_PP_PKR_CNT_X; nPkrY++)
			{
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nPkrX, nPkrY, &stPkrDvc);
				BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stPkrDvc);
				if (stPkrDvc.sExist == DEF_EXIST && strcmp(stPkrDvc.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0)
					return TRUE;
			}
		}
	}


	return FALSE;
}


BOOL CTaskAutoTestPp::PossibleAlreadyPickLoadTable(int nTestHandidx, int nJobIdx, int &nNextJobIdx)
{
	int nStationOnCnt = 0;
	int nTmpJobIdx = -1;
	double dMaxTestTime = 0.0;
	int nBufferDvcCnt = g_TaskTestPP[m_eTestIdx].GetBufferTableCnt();

	bool bLoadBreak = true;

	if (nBufferDvcCnt == 0) {
		for (int i = 0; i < eMaxATS_Area - 1; i++) 
		{ // 현재 작업한 Station 제외 검색 
			nJobIdx++;
			if (nJobIdx > eATS_DN_STATION_L4 || nJobIdx < eATS_DN_STATION_L1)
				nJobIdx = eATS_DN_STATION_L1;

			int nSocketOnCnt = g_TaskPressUnit[m_vModuleSetPressIdx[nJobIdx]].GetSocketOnCnt();
			int nStationInDvc = g_TaskPressUnit[m_vModuleSetPressIdx[nJobIdx]].GetSiteInDvc();

			if (nSocketOnCnt == 0 && nStationInDvc > 0) {
				break;
			}

			if (nSocketOnCnt > 0) {
				nStationOnCnt++;

				bLoadBreak = true;

				if (g_TaskTrayLoadPP.GetTrayPpLoadDvcCnt() > 0 || g_TaskLoadTbl[m_nLoadingTableInx].GetLoadDeviceCnt() > 0 || nStationInDvc > 0  /*|| nStationOnCnt> 0*/)
				{
					bLoadBreak = false;
				}

				// Test가 끝났거나, 시작 안했을 때, && Device가 투입 가능 할 때
				if (g_DMCont.m_dmEQP.GB(BDM0_TEST_STATUS_SITE1 + m_vModuleSetPressIdx[nJobIdx]) == FALSE && bLoadBreak == false/*&& nStationInDvc > 0*//*&& ChkCleaningMode(nJobIdx) != TRUE*/) 
				{
					//nNextJobIdx = nJobIdx;
					//return TRUE;
// 					if (g_TaskSystemCtrl.IsOneCycleModeOn() == TRUE ||
// 						g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == true && nStationInDvc != 0)
// 					{
// 						continue;
// 					}
						// Station 비교 후, 제일 빠른 시간
						
// 					int nJob = FindStation_CompEOT();
// 					if (nJob != -1) // sysTestTm < sysCompTestTm
// 					{
// 						nTmpJobIdx = nJob;
// 						break;
// 					}
					nTmpJobIdx = nJobIdx;
					break;
								
				} // Test가 끝났거나, 시작 안했을 때, && Device가 투입 불가능 할 때
				else if (g_DMCont.m_dmEQP.GB(BDM0_TEST_STATUS_SITE1 + m_vModuleSetPressIdx[nJobIdx]) == FALSE && bLoadBreak == true /*&& nStationInDvc > 0*//*&& ChkCleaningMode(nJobIdx) != TRUE*/) {
					//nNextJobIdx = nJobIdx;
					//return TRUE;
					if (g_TaskSystemCtrl.IsOneCycleModeOn() == TRUE ||
						g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == true && nStationInDvc == 0)
					{
						continue;
					}

					nTmpJobIdx = nJobIdx;
					break;
				}	// Test 하는 중 이거나, && 자재 투입이 가능 할 때.			
				else if (g_DMCont.m_dmEQP.GB(BDM0_TEST_STATUS_SITE1 + m_vModuleSetPressIdx[nJobIdx]) == TRUE) {
					if (dMaxTestTime < g_DMCont.m_dmEQP.GN(NDM0_Press1_TestTm + m_vModuleSetPressIdx[nJobIdx])) {
						dMaxTestTime = g_DMCont.m_dmEQP.GN(NDM0_Press1_TestTm + m_vModuleSetPressIdx[nJobIdx]);

						nTmpJobIdx = nJobIdx;
					}
				}
			}
		}
		if (nTmpJobIdx != -1) {
			if (ChkCleaningMode(nTmpJobIdx) != TRUE && g_TaskPressUnit[m_vModuleSetPressIdx[nTmpJobIdx]].GetSocketOnCnt() > 0) {
				nNextJobIdx = nTmpJobIdx;
				return TRUE;
			}
		}
	}
	
	
	return FALSE;
}

int CTaskAutoTestPp::CheckJobAlreadyPickDvcPlacetoArtBuff(int nAtsIdx, CPoint &vPocket, std::vector<CPoint> &vPicker,int& nLocIdx)
{
	CString strErrMsg = _T("");
	std::vector<CPoint> vErrPicker; vErrPicker.clear();
	std::vector<CString> vErrstring; vErrstring.clear();
	int nHandType = 0;
	int nErr = ChkDevice(strErrMsg, nHandType, vErrstring);
	if (nErr != ERR_NO_ERROR) {
		g_TaskSystemCtrl.SetErrTestPpIdx(m_eTestIdx, 1);
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Test PP %d %s", m_eTestIdx + 1, strErrMsg);
		for (int i = 0; i < (int)vErrstring.size(); i++)
		{
			AlarmData.m_fnAddItemFormat("%s", vErrstring[i]);
		}
		g_Error.AlarmReport(nErr, AlarmData, &g_TaskSystemCtrl, g_TaskTestPP[m_eTestIdx].GetFailPicker(nHandType));
		g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
		return eExcutingStep_ErrorJob;
	}

	vPicker.clear();

	int nLoadTblPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	int nLoadTblPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

	int nLoadTableIdx = -1;

	for (auto nLoadTblIdx : m_vModuleSetLoadTableIdx)
	{		
		int nBufferDvcCnt = g_TaskLoadTbl[nLoadTblIdx].GetBufferDeviceCnt();
		int nBufferPocketCnt = g_TaskTestPP[m_eTestIdx].GetBufferTablePocketCnt();
		if (nBufferDvcCnt < nBufferPocketCnt/2)
		{
			nLoadTableIdx = nLoadTblIdx;
			break;
		}	
	}

	if (nLoadTableIdx == -1)
		return eExcutingStep_ErrorJob;

	int nMaxBuff_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestBuffTbl_Div_Y);
	int nMaxBuff_X = g_DMCont.m_dmTestPP.GN(NDM4_TestBuffTbl_Div_X);

	// find available hand picker
	int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y];
	for (int nPkrY = 0; nPkrY < MAX_TEST_PP_PKR_CNT_Y; nPkrY++)
	{
		for (int nPkrX = 0; nPkrX < MAX_TEST_PP_PKR_CNT_X; nPkrX++)
		{
			nPickerDvcPattern[nPkrX][nPkrY] = DEF_NOT_EXIST;
		}
	}
	int nAvailableCount = GetAlreadyPickDvcArtBuffHandAssign(nPickerDvcPattern, nAtsIdx);
	if (nAvailableCount > 0)
	{
		// get art buffer table
		int nArrVirtualBuff[VIRTUAL_POCKET_X_MAX][VIRTUAL_POCKET_Y_MAX];
		memset(nArrVirtualBuff, 0xFF, sizeof(nArrVirtualBuff));
		FillDvc_VirtualTable(eDeviceStage_LD_TBL_1 + nLoadTableIdx, (int*)nArrVirtualBuff, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, DEF_PLACE, nAtsIdx, DEF_TABLE_TYPE_BUFFER);

		int nUseJobPkr[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = { 0, };
		int nPockX = -1, nPockY = -1;
		int nRetCode = FindPickPlaceFromTable(LOC_IDX_TEST_PP_TBL_1_LD + nLoadTableIdx, nArrVirtualBuff, nPickerDvcPattern, DEF_PLACE, &nPockX, &nPockY, nUseJobPkr);
		if (nRetCode > 0) {
			std::vector<CPoint> vPkr;
			for (int nPickX = 0; nPickX < MAX_TEST_PP_PKR_CNT_X; nPickX++) {
				for (int nPickY = 0; nPickY < MAX_TEST_PP_PKR_CNT_Y; nPickY++) {
					if (nUseJobPkr[nPickX][nPickY] != 0) {
						vPkr.push_back(CPoint(nPickX, nPickY));
					}
				}
			}

			switch (nLoadTableIdx)
			{
			case eLoadTbl_2:
			case eLoadTbl_3:
			{
				nLocIdx = LOC_IDX_TEST_PP_TBL_1_LD;
			}break;
			case eLoadTbl_1:
			case eLoadTbl_4:
			{
				nLocIdx = LOC_IDX_TEST_PP_TBL_1_UD;
			}break;
			}

			vPocket = CPoint(nPockX, nPockY);
			vPicker = vPkr;
			return eExcutingStep_FindJob;
		}
	}

	return eExcutingStep_ErrorJob;
}
int CTaskAutoTestPp::GetAlreadyPickDvcArtBuffHandAssign(int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nWorkingAreaIdx)
{
	BOOL bModeAutoRetest = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
	BOOL bAtoAPickPlace = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_ART_TEST_HAND_PICK_PLACE);
	int nCleanMode = ChkCleaningMode(m_nJobAtsIdx);

	if (nCleanMode == TRUE)
	{
		bAtoAPickPlace = FALSE;
	}

	ST_DD_DEVICE stDvc;
	int nPkrPossibleCnt = 0;

	if (m_nCleanMode == TRUE) {
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{
				stDvc.clear();
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY, &stDvc);
				// Hand Device 가 있어야 한다.
				if (stDvc.sExist == DEF_EXIST)
				{
					if (strcmp(stDvc.szBinResult, DEF_SOCKET_CLEAN_DEVICE) != 0) {
						nPickerDvcPattern[nX][nY] = DEF_EXIST;	// 사용가능
						nPkrPossibleCnt++;
					}

				}
			}
		}
	}

	return nPkrPossibleCnt;
}

int CTaskAutoTestPp::GetReserveAndLoadTblWorkPos(int nLoadTableIdx)
{
	int ntest = g_TaskLoadTbl[nLoadTableIdx].GetWorkReserve();
	int nLoadTableReserve = g_TaskLoadTbl[nLoadTableIdx].GetWorkReserve() == RID_TEST_PP_(m_eTestIdx);
	int table_col_step = g_TaskLoadTbl[nLoadTableIdx].GetTableColStep();
	int nLoadTablePos = g_TaskLoadTbl[nLoadTableIdx].ChkMotorPos(eLocIdxLoadingTbl::LOC_IDX_LD_TBL_TEST_SIDE_1 + m_nJobAtsIdx, CTaskLoadTable::eAxisY, table_col_step);
	

	if (nLoadTableReserve == TRUE && nLoadTablePos == ERR_NO_ERROR)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CTaskAutoTestPp::GetHandAutoRetestBinCheck()
{
	ST_DD_DEVICE stPkrDvc;
	BOOL bArtBinCheck = FALSE;
	for (int nPkrX = 0; nPkrX < MAX_TEST_PP_PKR_CNT_X && bArtBinCheck == FALSE; nPkrX++)
	{
		for (int nPkrY = 0; nPkrY < MAX_TEST_PP_PKR_CNT_X && bArtBinCheck == FALSE; nPkrY++)
		{
			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nPkrX, nPkrY + TEST_UD_PKR_Y, &stPkrDvc);
			BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stPkrDvc);
			if (stPkrDvc.nBinColor == eFailBin && stPkrDvc.nTestingCnt == eARB_1stTestEnd && strcmp(stPkrDvc.szErrString, DEF_TEST_RESULT_TP101) != 0 && m_nCleanMode != TRUE && bIsDefaultBin != TRUE)
				bArtBinCheck = TRUE;
		}
	}

	return bArtBinCheck;
}


//////	Pick			
//		 Station
//	| o	| o	| o	| o	|		 
//  | x	| x	| x	| x	|
//		Load Table
//	| x	| x	| x	| x	|     
//  | x	| x	| x	| x	|
//  | o | o	| o	| o	|
//  | x	| x	| x	| x	|

//////	Place			
//		 Station
//	| o	| o	| o	| o	|		 
//  | x	| x	| x	| x	|
//		Load Table
//	| x	| x	| x	| x	|
//  | o | o	| o	| o	|
//  | x	| x	| x	| x	|
//  | x	| x	| x	| x	|


int CTaskAutoTestPp::GetTableIdx(int nType)
{
	int nRet = 0;
	if (m_eTestIdx == eTestPP_1) {
		nRet = (nType == eLoadType) ? 1 : 0;
	}
	else {
		nRet = (nType == eLoadType) ? 2 : 3;
	}
	return nRet;
}

int CTaskAutoTestPp::ChkStationOnOffDvcCnt(BOOL bGrrMode)
{
	int nTestSiteDvcCnt = 0;

	ST_DD_DEVICE tSiteData;

	int nSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	for (int i = 0; i < eMaxATS_Area; i++)
	{
		for (int nX = 0; nX < nSiteX; nX++)
		{
			for (int nY = 0; nY < nSiteY; nY++)
			{
				g_TaskPressUnit[m_vModuleSetPressIdx[i]].GetSiteDeviceData(&tSiteData, nX, nY);
				int nTarget = (bGrrMode == TRUE ? NDM11_GRR_SocketOnOff_Site1_1 : NDM11_SocketOnOff_Site1_1) + nX + nY*STATION_MAX_X_SITE + (m_vModuleSetPressIdx[i] * STATION_MAX_PARA);
				int nOnOff = g_DMCont.m_dmHandlerOpt.GN(nTarget);
				if (nOnOff == DEF_SITE_ON || tSiteData.sExist == eDeviceData_Exist)
					nTestSiteDvcCnt++;
			}
		}
	}

	return nTestSiteDvcCnt;
}

int CTaskAutoTestPp::ChkJobGrr_LoadTable(int nAtsIdx, BOOL bPickPlace, CPoint &vPocket, std::vector<CPoint> &vPicker)
{
	// Pick / Place에 따라서, 탐색 할 Load Table에 Index를 얻어 온다.
// 	int nLoadTableIdx = GetLoadTableInx(bPickPlace);
// 
// 	if (bPickPlace == DEF_PICK)
// 	{
// 		int nLoadTblCnt = g_TaskLoadTbl[nLoadTableIdx].GetLoadDeviceCnt();
// 		if (nLoadTblCnt <= 0)
// 			return eExcutingStepGrr_ErrorJob;
// 	}
// 
// 	int nTblRes = g_TaskLoadTbl[nLoadTableIdx].GetWorkReserve();
// 	if (nTblRes == RID_TEST_PP_(m_eTestIdx))
// 	{
// 		if (g_TaskLoadTbl[nLoadTableIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
// 			return eExcutingStepGrr_WaitOtherJob;
// 
// 		int nChkMotor = g_TaskLoadTbl[nLoadTableIdx].ChkMotorPos(LOC_IDX_LD_TBL_TEST_SIDE_1 + m_nJobAtsIdx, CTaskLoadTable::eAxisAll);
// 		if (nChkMotor != ERR_NO_ERROR)
// 			return eExcutingStepGrr_WaitOtherJob;
// 	}
	///////////////////////////////////////////////////
 	int nLoadTableIdx = GetLoadTableInx(bPickPlace);
 
 	CString strErrMsg = _T("");
	std::vector<CPoint> vErrPicker; vErrPicker.clear();
	std::vector<CString> vErrstring; vErrstring.clear();
	int nHandType = 0;
 	int nErr = ChkDevice(strErrMsg, nHandType, vErrstring);
 	if (nErr != ERR_NO_ERROR) {
 		g_TaskSystemCtrl.SetErrTestPpIdx(m_eTestIdx, 1);
 		CParamList AlarmData;
 		AlarmData.m_fnAddItemFormat("Test PP %d %s", m_eTestIdx + 1, strErrMsg);
		for (int i = 0; i < (int)vErrstring.size(); i++)
		{
			AlarmData.m_fnAddItemFormat("%s", vErrstring[i]);
		}
 		g_Error.AlarmReport(nErr, AlarmData, &g_TaskSystemCtrl, g_TaskTestPP[m_eTestIdx].GetFailPicker(nHandType));
 		g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
 		return eExcutingStep_ErrorJob;
 	}
 
 	if (bPickPlace == DEF_PICK)
 	{
 		BOOL bChkCleanDevPick = GetDvcStationCleanDevAssign(DEF_PICK, nAtsIdx);
 		BOOL bChkCleanDevPlace = GetDvcStationCleanDevAssign(DEF_PLACE, nAtsIdx);
 		if (bChkCleanDevPick != TRUE && bChkCleanDevPlace != TRUE) {
 		}
 		else { 
			return eExcutingStep_ErrorJob; 
		}


		CPoint JobPocket;
		std::vector<CPoint> vJobPicker_Pick;
		int nLocIdx = -1;

		//Station On Cnt 보다 투입 자재가 많을 경우 처리.
		//Station Pick 할게 남아 있으면 Load 자재 Pick 안함.
//  		int nJobSamePick = ChkJobGrr_Station(m_nJobAtsIdx, DEF_PICK, JobPocket, vJobPicker_Pick);
//  		if (nJobSamePick != eExcutingStepGrr_ErrorJob)
//  			return eExcutingStepGrr_ErrorJob;

		int all_site_test_done = ChkGrrAllSiteTestDone(eDeviceStage_TEST_SITE_1 + m_vModuleSetPressIdx[m_nJobAtsIdx]);
		if (all_site_test_done != TRUE) {
			return eExcutingStepGrr_ErrorJob;
		}


		for (auto nLoadTblIdx : m_vModuleSetLoadTableIdx)
		{
			int nBufferDvcCnt = g_TaskLoadTbl[nLoadTblIdx].GetBufferDeviceCnt();
			if (nBufferDvcCnt > 0)
			{
				return eExcutingStepGrr_ErrorJob;
			}		
		}


 	}
	else /*(bPickPlace == DEF_PLACE)*/ //place일 경우, Unload PP test end 확인 필요.
	{
		int nAllSiteTestDone = ChkGrrAllSiteTestDone(eDeviceStage_TEST_PP_1 + m_eTestIdx);
		if (nAllSiteTestDone != TRUE)
			return eExcutingStep_ErrorJob;
	}



 	// table이 단 한번도 ld side로 가지 않았기 때문에 job을 실행하면 안된다.
 	int nTblRes = g_TaskLoadTbl[nLoadTableIdx].GetWorkReserve();
 	if (nTblRes == RID_INIT_IDLE_RESERVE || nTblRes == RID_NOT_RESERVE)
 		return eExcutingStep_ErrorJob;
 	// cleaning 이 예약 되어 있을 경우에는 집지 못한다.
 	if ((bPickPlace == DEF_PICK || (bPickPlace == DEF_PLACE && g_TaskTestPP[m_eTestIdx].GetUnloadPickerHaveDvcCnt() == 0)) && m_nCleanMode == TRUE)
 		return eExcutingStep_ErrorJob;
 
 	if (bPickPlace == DEF_PLACE && g_TaskTestPP[m_eTestIdx].GetUnloadPickerHaveDvcCnt() != 0 && g_TaskLoadTbl[nLoadTableIdx].ChkMotorPos(LOC_IDX_LD_TBL_TEST_SIDE_1 + nAtsIdx, CTaskLoadTable::eAxisAll) != ERR_NO_ERROR
 		&& g_TaskLoadTbl[nLoadTableIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
 		return eExcutingStep_WaitOtherJob;
 
 	// Loading table의 state를 점검한다.
 	if (nTblRes == RID_TEST_PP_(m_eTestIdx))
 	{
 		if (g_TaskLoadTbl[nLoadTableIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
 			return eExcutingStep_WaitOtherJob;
 
 		//int nChkMotor = g_TaskLoadTbl[m_eTestIdx].ChkMotorPos(LOC_IDX_LD_TBL_TEST_SIDE, CTaskLoadTable::eAxisAll);
 		//if( nChkMotor != ERR_NO_ERROR )
 		//	return eExcutingStep_WaitOtherJob;
 	}







// 	if ((bPickPlace == DEF_PICK || (bPickPlace == DEF_PLACE && g_TaskTestPP[m_eTestIdx].GetUnloadPickerHaveDvcCnt() == 0)) && m_nCleanMode == TRUE)
// 		return eExcutingStepGrr_ErrorJob;

// 	CPoint JobPocket;
// 	std::vector<CPoint> vJobPicker_Pick;
// 	int nJob = ChkJobGrr_SamePlaceStation(JobPocket, vJobPicker_Pick);
// 	if (nJob != eExcutingStepGrr_ErrorJob) //동일 Station Place 할게 있는 경우
// 		return eExcutingStepGrr_ErrorJob; //Load Table Job은 없음.
// 
// 	int nLocIdx = -1;
// 	nJob = ChkJobGrr_ArtBuff(m_nJobAtsIdx, DEF_PICK, JobPocket, vJobPicker_Pick, nLocIdx);
// 	if (nJob != eExcutingStepGrr_ErrorJob)
// 		return eExcutingStepGrr_ErrorJob;


	if (g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == FALSE
		|| g_TaskPressUnit[m_vModuleSetPressIdx[nAtsIdx]].ChkSiteInDvc() > 0
		|| g_TaskLoadTbl[nLoadTableIdx].GetLoadDeviceCnt() > 0
		/*|| g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_CONV_2) == TRUE
		|| g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_BTM_2) == TRUE*/
		|| g_TaskTrayLoadPP.GetTrayPpLoadDvcCnt() > 0)
	{
		if (nTblRes == RID_TRAY_PP_(RIDX_TRAY_PP_1))
		{
			//if (g_TaskTrayPP[m_eTrayIdx].GetTrayPpLoadDvcCnt() > 0
			//	|| g_TaskTrayPP[m_eTrayIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
			//return eExcutingStep_WaitOtherJob;
			return eExcutingStep_ErrorJob;
		}
	}

	vPicker.clear();
	int nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	// pick일 때만 site를 보고 확인하면 된다.
	if (bPickPlace == DEF_PLACE)
		nSiteCntY = 1;

	//for(int nY=0; nY < nSiteCntY; nY++ )
	{
		// find available hand picker
		int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y];
		for (int nPkrX = 0; nPkrX < MAX_TEST_PP_PKR_CNT_X; nPkrX++)
		{
			for (int nPkrY = 0; nPkrY < MAX_TEST_PP_PKR_CNT_Y; nPkrY++)
			{
				nPickerDvcPattern[nPkrX][nPkrY] = (bPickPlace == DEF_PICK ? DEF_EXIST : DEF_NOT_EXIST);
			}
		}
		int nAvailableCount = GetDvcLdTableHandAssignGrr(bPickPlace, nPickerDvcPattern, nAtsIdx);

		// get clean buffer table
		if (nAvailableCount > 0)
		{
			if (bPickPlace == DEF_PLACE) {
				int a = 0;
			}
			int nArrVirtualTbl[VIRTUAL_POCKET_X_MAX][VIRTUAL_POCKET_Y_MAX];
			memset(nArrVirtualTbl, 0xFF, sizeof(nArrVirtualTbl));
			FillDvcGrr_VirtualTable(eDeviceStage_LD_TBL_1 + nLoadTableIdx, (int*)nArrVirtualTbl, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, bPickPlace, nAtsIdx, DEF_TABLE_TYPE_LOAD_UNLOAD);

			int nUseJobPkr[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = { 0, };
			int nPockX = -1, nPockY = -1;
			int nRetCode = FindPickPlaceFromTable(LOC_IDX_TEST_PP_TBL_1_LD + nLoadTableIdx, nArrVirtualTbl, nPickerDvcPattern, bPickPlace, &nPockX, &nPockY, nUseJobPkr);
			if (nRetCode > 0) {
				std::vector<CPoint> vPkr;
				for (int nPickY = 0; nPickY < MAX_TEST_PP_PKR_CNT_Y; nPickY++) {
					for (int nPickX = 0; nPickX < MAX_TEST_PP_PKR_CNT_X; nPickX++) {
						if (nUseJobPkr[nPickX][nPickY] != 0) {
							vPkr.push_back(CPoint(nPickX, nPickY));
						}
					}
				}

				if (vPkr.empty() == false) {
// 					if (bPickPlace == DEF_PLACE) {
// 						vPocket = CPoint(nPockX, nPockY);
// 						vPicker = vPkr;
// 						return eExcutingStepGrr_FindJob;
// 					}
// 					else
// 					{
// 						vPocket = CPoint(nPockX, nPockY);
// 						vPicker = vPkr;
// 						return eExcutingStepGrr_FindJob;
// 					}

					vPocket = CPoint(nPockX, nPockY);
					vPicker = vPkr;
					return eExcutingStepGrr_FindJob;

				}
			}
		}
	}

	return eExcutingStepGrr_ErrorJob;
}

int CTaskAutoTestPp::ChkJobGrr_SamePlaceStation(CPoint &cPocket, std::vector<CPoint> &vPicker)
{
	int nChkInterlock = ChkInterlockAts(m_nJobAtsIdx);
	if (nChkInterlock == eExcutingStep_ErrorJob)
		return eExcutingStep_ErrorJob;

	//if ( g_TaskTestPP[m_eTestIdx].GetLoadPickerDeviceCnt() > 0 )
	//	return eExcutingStepGrr_ErrorJob;

	if (g_TaskSystemCtrl.GetAutoGrrCancel() == TRUE)
		return eExcutingStepGrr_ErrorJob;

	if (m_nCleanMode == TRUE)
		return eExcutingStep_ErrorJob;

	vPicker.clear();

	int nPkrDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = { 0, };
	int nPocketCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	// find available hand picker
	for (int nPkrY = 0; nPkrY < MAX_TEST_PP_PKR_CNT_Y; nPkrY++)
	{
		for (int nPkrX = 0; nPkrX < MAX_TEST_PP_PKR_CNT_X; nPkrX++)
		{
			nPkrDvcPattern[nPkrX][nPkrY] = DEF_NOT_EXIST;
		}
	}
	int nAvailableCnt = GetDvcStationHandAssignGrr(/*DEF_PICK*/DEF_PLACE, nPkrDvcPattern, TRUE);

	if (nAvailableCnt > 0)
	{
		int nArrVirtualStation[VIRTUAL_POCKET_X_MAX][VIRTUAL_POCKET_Y_MAX] = { 0, };
		memset(nArrVirtualStation, 0xFF, sizeof(nArrVirtualStation));
		FillDvcGrr_SamePlaceVirtualStation(eDeviceStage_TEST_SITE_1 + m_vModuleSetPressIdx[m_nJobAtsIdx], (int*)nArrVirtualStation, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX);

		int nPockX = -1, nPockY = -1;
		int nUseJobPkr[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = { 0, };
		int nRetCnt = FindPickPlaceFromStation(m_nJobAtsIdx, nArrVirtualStation, nPkrDvcPattern, /*DEF_PICK*/DEF_PLACE, &nPockX, &nPockY, nUseJobPkr);

		if (nRetCnt > 0)
		{
			std::vector<CPoint> vPkr;
			for (int nPickX = 0; nPickX < MAX_TEST_PP_PKR_CNT_X; nPickX++) {
				for (int nPickY = 0; nPickY < MAX_TEST_PP_PKR_CNT_Y; nPickY++) {
					if (nUseJobPkr[nPickX][nPickY] != 0) {
						vPkr.push_back(CPoint(nPickX, nPickY));
					}
				}
			}

			cPocket = CPoint(nPockX, nPockY);
			vPicker = vPkr;
			return eExcutingStep_FindJob;
		}
	}

	return eExcutingStepGrr_ErrorJob;
}

int CTaskAutoTestPp::GetDvcStationHandAssignGrr(BOOL bPickPlace, int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], BOOL bChkUD_Grr /*= FALSE*/)
{
	ST_DD_DEVICE stDvcHand;

	int nPkrPossibleCnt = 0;
	if (bPickPlace == DEF_PICK)
	{
		int nSocketDvc[STATION_MAX_X_SITE][STATION_MAX_Y_SITE];
		memset(nSocketDvc, DEF_NOT_EXIST, sizeof(nSocketDvc));

		BOOL bChkCleanDev = GetDvcStationCleanDevAssign(bPickPlace, m_nJobAtsIdx);

		// unload picker 정보
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{
				stDvcHand.clear();
				if (bChkCleanDev == TRUE) // Clean Device가 Station에 Pick 할수 있는 상태면, Load Hand로 작업 한다.
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY, &stDvcHand);
				else
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY + TEST_UD_PKR_Y, &stDvcHand);

				if (stDvcHand.sExist == DEF_NOT_EXIST)
				{
					nPickerDvcPattern[nX][nY] = DEF_NOT_EXIST;	// 사용가능
					nPkrPossibleCnt++;
				}
			}
		}

		//GRR retest추가
	}
	else {

		BOOL bFindNotEnd = FALSE;
		int nUloadPckDvcCnt = g_TaskTestPP[m_eTestIdx].GetUnloadPickerHaveDvcCnt();
		if (g_TaskTestPP[m_eTestIdx].GetUnloadPickerHaveDvcCnt() > 0)
		{
			// unload picker 정보
			for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
			{
				stDvcHand.clear();
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, TEST_UD_PKR_Y, &stDvcHand);

				if (stDvcHand.sExist == DEF_EXIST)
				{
					if (stDvcHand.stGrrTestInfo[m_vModuleSetPressIdx[m_nJobAtsIdx]].chkTestEnd(/*nX, nStationWorkRow*/) != TRUE)
					{
						bFindNotEnd = TRUE;
					}
				}
			}
		}

		// unload picker 정보
		//int nUsePkr = (bFindNotEnd == TRUE ? TEST_UD_PKR_Y : TEST_LD_PKR_Y);
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{
				stDvcHand.clear();
				//Grr Test 안끝났으면 Unload Check
				if (bFindNotEnd == TRUE) {
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY + TEST_UD_PKR_Y, &stDvcHand);
				}
				//Grr Test 끝났는데 Same Place Check 할경우
				else if (bFindNotEnd != TRUE && bChkUD_Grr == TRUE)
				{
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY + TEST_UD_PKR_Y, &stDvcHand);
				}
				//else if cleaning 이면 load pp 보도록 해야 겠다..
				else {
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY, &stDvcHand);
				}

				if (stDvcHand.sExist == DEF_EXIST)
				{
					int nSiteInDvcCnt = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].ChkSiteInDvc();
					if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrDeviceSwapUse) == TRUE) {
						if (stDvcHand.stGrrTestInfo[m_vModuleSetPressIdx[m_nJobAtsIdx]].chkTestEnd(nX, nY) == TRUE /*&& nSiteInDvcCnt > 0*/  && m_nCleanMode == FALSE)
							continue;
					}
					else {
						if (stDvcHand.stGrrTestInfo[m_vModuleSetPressIdx[m_nJobAtsIdx]].chkTestEnd() == TRUE /*&& nSiteInDvcCnt > 0*/ && m_nCleanMode == FALSE)
							continue;
					}


					int nOnOff = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetSocketOnOffGrr(nX, nY);
					if (nOnOff == DEF_SITE_ON)
					{
						nPickerDvcPattern[nX][nY] = DEF_EXIST;	// 사용가능
						nPkrPossibleCnt++;
					}
				}
			}
		}

	}

	return nPkrPossibleCnt;
}

int CTaskAutoTestPp::FillDvcGrr_SamePlaceVirtualStation(int nDvcStageIdx, int* pnBuffer, int nBufSizeX, int nBufSizeY)
{
	int nPocketCount_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nPocketCount_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	//BOOL bModeAutoRetest = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
// 	BOOL bAtoAPickPlace = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_ART_TEST_HAND_PICK_PLACE);
// 
// 	int nCleanMode = ChkCleaningMode(m_nJobAtsIdx);
// 
// 	if (nCleanMode == TRUE)
// 	{
// 		bAtoAPickPlace = FALSE;
// 	}

	ST_DD_DEVICE stSite;
	for (int nY = 0; nY < nPocketCount_Y; nY++)
	{
		for (int nX = 0; nX < nPocketCount_X; nX++)
		{
			stSite.clear();
			int nBuffer_Index = (nX + VIRTUAL_POCKET_X_START)*nBufSizeY/*X*/ + (nY + VIRTUAL_POCKET_Y_START)/*Y*/;
			g_DMCont.m_dmDevice.GetDeviceData(nDvcStageIdx, nX, nY, &stSite);

			if (stSite.sExist == DEF_EXIST)
			{
				if (stSite.nTestingInfo == eTestSite_DvcLoading || stSite.nTestingInfo == eTestSite_CleanLoading || stSite.nTestingInfo == eTestSite_TestTimeOut)
				{
					pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
					continue;
				}


				BOOL bChkEnd = stSite.stGrrTestInfo[nDvcStageIdx - eDeviceStage_TEST_SITE_1].chkTestEnd(/*nX, nY*/);
				if (bChkEnd == FALSE)
					pnBuffer[nBuffer_Index] = DEF_EXIST;
				else
					pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
			}
			else
				pnBuffer[nBuffer_Index] = DEF_NOT_EXIST;
		}
	}

	return ERR_NO_ERROR;
}

int CTaskAutoTestPp::ChkJobGrr_CleanBuff(int nAtsIdx, BOOL bPickPlace, CPoint &vPocket, std::vector<CPoint> &vPicker)
{
	CString strErrMsg = _T("");
	std::vector<CPoint> vErrPicker; vErrPicker.clear();
	std::vector<CString> vErrstring; vErrstring.clear();
	int nHandType = 0;
	int nErr = ChkDevice(strErrMsg, nHandType, vErrstring);
	if (nErr != ERR_NO_ERROR) {
		g_TaskSystemCtrl.SetErrTestPpIdx(m_eTestIdx, 1);
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Test PP %d %s", m_eTestIdx + 1, strErrMsg);
		for (int i = 0; i < (int)vErrstring.size(); i++)
		{
			AlarmData.m_fnAddItemFormat("%s", vErrstring[i]);
		}
		g_Error.AlarmReport(nErr, AlarmData, &g_TaskSystemCtrl, g_TaskTestPP[m_eTestIdx].GetFailPicker(nHandType));
		g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
		return eExcutingStep_ErrorJob;
	}

	
	//Initial일 때만, Socket Cleaning 진행.

	if (g_TaskSystemCtrl.GetAutoGrrCancel() == TRUE && bPickPlace == DEF_PICK)
		return eExcutingStep_ErrorJob;

	if (bPickPlace == DEF_PICK && m_nCleanMode == FALSE)
		return eExcutingStep_ErrorJob;
	else
	{
		vPicker.clear();

		int nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

		// pick일 때만 site를 보고 확인하면 된다.
		if (bPickPlace == DEF_PLACE)
			nSiteCntY = 1;

		//for(int nY=0; nY < nSiteCntY; nY++ )
		{
			// find available hand picker
			int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y];

			for (int nPkrY = 0; nPkrY < MAX_TEST_PP_PKR_CNT_Y; nPkrY++)
			{
				for (int nPkrX = 0; nPkrX < MAX_TEST_PP_PKR_CNT_X; nPkrX++)
				{
					nPickerDvcPattern[nPkrX][nPkrY] = (bPickPlace == DEF_PICK ? DEF_EXIST : DEF_NOT_EXIST);
				}
			}
			int nAvailableCount = GetDvcCleanGrrHandAssign(bPickPlace, nPickerDvcPattern, nAtsIdx);

			if (nAvailableCount > 0)
			{
 				// get clean buffer table
 				int nArrVirtualCleanBuff[VIRTUAL_POCKET_X_MAX][VIRTUAL_POCKET_Y_MAX];
 				memset(nArrVirtualCleanBuff, 0xFF, sizeof(nArrVirtualCleanBuff));
				FillDvcGrr_VirtualTable(eDeviceStage_TEST_PP_1_CLEAN_BUFF + m_eTestIdx, (int*)nArrVirtualCleanBuff, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, bPickPlace, nAtsIdx);
 
 				int nUseJobPkr[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = { 0, };
 				int nPockX = -1, nPockY = -1;
 				int nRetCode = FindPickPlaceFromCleanTable(nArrVirtualCleanBuff, nPickerDvcPattern, bPickPlace, &nPockX, &nPockY, nUseJobPkr);
 				if (nRetCode > 0) {
 					std::vector<CPoint> vPkr;
 					for (int nPickX = 0; nPickX < MAX_TEST_PP_PKR_CNT_X; nPickX++) {
 						for (int nPickY = 0; nPickY < MAX_TEST_PP_PKR_CNT_Y; nPickY++) {
 							if (nUseJobPkr[nPickX][nPickY] != 0) {
 								vPkr.push_back(CPoint(nPickX, nPickY));
 							}
 						}
 					}
 
 					vPocket = CPoint(nPockX, nPockY);
 					vPicker = vPkr;
 					return eExcutingStep_FindJob;
 				}
				else {
		
				}
			}
		}
	}

	return eExcutingStep_ErrorJob;
}

int CTaskAutoTestPp::ChkJobGrr_ArtBuff(int nAtsIdx, BOOL bPickPlace, CPoint &vPocket, std::vector<CPoint> &vPicker, int& nLocIdx)
{
	if (bPickPlace == DEF_PLACE && g_TaskSystemCtrl.GetAutoGrrCancel() == TRUE) {
		return eExcutingStep_ErrorJob;
	}

	if (bPickPlace == DEF_PLACE && m_nCleanMode == TRUE)
		return eExcutingStep_ErrorJob;

	if (bPickPlace == DEF_PICK)
	{
		CPoint JobPocket;
		std::vector<CPoint> vJobPicker_Pick;
		int nJob = ChkJobGrr_SamePlaceStation(JobPocket, vJobPicker_Pick);
		if (nJob != eExcutingStepGrr_ErrorJob)
			return eExcutingStepGrr_ErrorJob;

		//int nCntStation = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].ChkSiteInDvc();
		//int nCntStationOn = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetSocketOnCntGrr();
		//if (nCntStation > 0 && nCntStationOn > 0)
		//{
		//	nJob = ChkJobGrr_Station(DEF_PICK, JobPocket, vJobPicker_Pick);
		//	if (nJob == eExcutingStepGrr_ErrorJob)
		//		return eExcutingStepGrr_ErrorJob;
		//}
	}

	int nLoadTblPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	int nLoadTblPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

	int nLoadTableIdx = -1;

	for (auto nLoadTblIdx : m_vModuleSetLoadTableIdx)
	{
		if (bPickPlace == DEF_PICK)
		{
			int nBufferDvcCnt = g_TaskLoadTbl[nLoadTblIdx].GetBufferDeviceCnt();
			if (nBufferDvcCnt > 0)
			{
				nLoadTableIdx = nLoadTblIdx;
				break;
			}
		}
		else
		{
			if (m_nJobAtsIdx == 0) {
				int a = 0;
			}
			int nBufferDvcCnt = g_TaskLoadTbl[nLoadTblIdx].GetBufferDeviceCnt();
			if (nBufferDvcCnt < (nLoadTblPocketCntX*nLoadTblPocketCntY / 2))
			{
				nLoadTableIdx = nLoadTblIdx;
				break;
			}
		}
	}

	if (nLoadTableIdx == -1)
		return eExcutingStepGrr_ErrorJob;


	int nTblRes = g_TaskLoadTbl[nLoadTableIdx].GetWorkReserve();
	if (nTblRes == RID_INIT_IDLE_RESERVE || nTblRes == RID_NOT_RESERVE)
		return eExcutingStepGrr_ErrorJob;

	//BOOL bAutoRetetsMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
	//if( bAutoRetetsMode == eOPTION_NOT_USE )
	//return eExcutingStep_ErrorJob; //SJI

	if (bPickPlace == DEF_PICK && m_nCleanMode == TRUE)
		return eExcutingStepGrr_ErrorJob;

	//place일 경우, Unload PP test end 확인 필요.
	if (bPickPlace == DEF_PLACE)
	{
		ST_DD_DEVICE stDevice;
		stDevice.clear();
		BOOL all_device_test_end = TRUE;
		int all_station_count = 0;
		for (int nPkrY = 0; nPkrY < MAX_TEST_PP_PKR_CNT_Y; nPkrY++)
		{
			for (int nPkrX = 0; nPkrX < MAX_TEST_PP_PKR_CNT_X; nPkrX++)
			{
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nPkrX, nPkrY + TEST_UD_PKR_Y, &stDevice);
				if(stDevice.sExist != DEF_EXIST) continue;

				int nEndCnt = 0;
				for (auto Idx : m_vModuleSetPressIdx) { //testend로만 check할 수 있음.
					BOOL bTestEnd = stDevice.stGrrTestInfo[Idx].chkTestEnd();
					int nTestDoneCnt = stDevice.stGrrTestInfo[Idx].chkTestDone();
					int nSocketOnCnt = g_TaskPressUnit[Idx].GetSocketOnCntGrr();
					if (bTestEnd == TRUE && nTestDoneCnt == nSocketOnCnt)
						nEndCnt++;
				}

				if (nEndCnt == m_vModuleSetPressIdx.size()) {
					all_device_test_end++;
				}
			}
		}

		if (all_device_test_end == STATION_MAX_PARA) {
			return eExcutingStep_ErrorJob;//Test 전체 Station End 되면 Buffer에 Place 못함.
		}
	}




	vPicker.clear();

	// find available hand picker
	int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y];
	for (int nPkrY = 0; nPkrY < MAX_TEST_PP_PKR_CNT_Y; nPkrY++)
	{
		for (int nPkrX = 0; nPkrX < MAX_TEST_PP_PKR_CNT_X; nPkrX++)
		{
			nPickerDvcPattern[nPkrX][nPkrY] = (bPickPlace == DEF_PICK ? DEF_EXIST : DEF_NOT_EXIST);
		}
	}
	int nAvailableCount = GetDvcArtBuffGrrHandAssign(bPickPlace, nPickerDvcPattern, m_nJobAtsIdx, m_nJobSiteRow);
	if (nAvailableCount > 0)
	{
		// get art buffer table
		int nArrVirtualBuff[VIRTUAL_POCKET_X_MAX][VIRTUAL_POCKET_Y_MAX];
		memset(nArrVirtualBuff, 0xFF, sizeof(nArrVirtualBuff));
//		FillDvc_VirtualTable(eDeviceStage_TEST_PP_1_ART_BUFF + m_eTestIdx, (int*)nArrVirtualBuff, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, bPickPlace, m_nJobAtsIdx);
		FillDvcGrr_VirtualTable(eDeviceStage_LD_TBL_1 + nLoadTableIdx, (int*)nArrVirtualBuff, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, bPickPlace, nAtsIdx, DEF_TABLE_TYPE_BUFFER);
//		int nLoadTableIdx = GetLoadTableInx(bPickPlace);
		int nUseJobPkr[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = { 0, };
		int nPockX = -1, nPockY = -1;
		int nRetCode = FindPickPlaceFromTable(LOC_IDX_TEST_PP_TBL_1_LD + nLoadTableIdx, nArrVirtualBuff, nPickerDvcPattern, bPickPlace, &nPockX, &nPockY, nUseJobPkr);
		if (nRetCode > 0) {
			std::vector<CPoint> vPkr;
			for (int nPickX = 0; nPickX < MAX_TEST_PP_PKR_CNT_X; nPickX++) {
				for (int nPickY = 0; nPickY < MAX_TEST_PP_PKR_CNT_Y; nPickY++) {
					if (nUseJobPkr[nPickX][nPickY] != 0) {
						vPkr.push_back(CPoint(nPickX, nPickY));
					}
				}
			}
			switch (nLoadTableIdx)
			{
			case eLoadTbl_2:
			case eLoadTbl_3:
			{
				nLocIdx = LOC_IDX_TEST_PP_TBL_1_LD;
			}break;
			case eLoadTbl_1:
			case eLoadTbl_4:
			{
				nLocIdx = LOC_IDX_TEST_PP_TBL_1_UD;
			}break;
			default:
			{
				int a = 0;
			}break;
			}
				

			if (g_TaskLoadTbl[nLoadTableIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
				return eExcutingStepGrr_WaitOtherJob;


			vPocket = CPoint(nPockX, nPockY);
			vPicker = vPkr;
			return eExcutingStepGrr_FindJob;
		}
	}

	return eExcutingStepGrr_ErrorJob;
}

int CTaskAutoTestPp::ChkJobGrr_SwapArtBuff(int nAtsIdx, BOOL bPickPlace, CPoint &vPocket, std::vector<CPoint> &vPicker, int& nLocIdx)
{
	if (bPickPlace == DEF_PLACE && g_TaskSystemCtrl.GetAutoGrrCancel() == TRUE) {
		return eExcutingStep_ErrorJob;
	}

	if (bPickPlace == DEF_PICK)
	{
		CPoint JobPocket;
		std::vector<CPoint> vJobPicker_Pick;
		int nJob = ChkJobGrr_SamePlaceStation(JobPocket, vJobPicker_Pick);
		if (nJob != eExcutingStepGrr_ErrorJob)
			return eExcutingStepGrr_ErrorJob;
	}

	int nLoadTblPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	int nLoadTblPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

	int nLoadTableIdx = -1;

	for (auto nLoadTblIdx : m_vModuleSetLoadTableIdx)
	{
		if (bPickPlace == DEF_PICK)
		{
			int nBufferDvcCnt = g_TaskLoadTbl[nLoadTblIdx].GetBufferDeviceCnt();
			if (nBufferDvcCnt > 0)
			{
				nLoadTableIdx = nLoadTblIdx;
				break;
			}
		}
		else
		{
			if (m_nJobAtsIdx == 0) {
				int a = 0;
			}
			int nBufferDvcCnt = g_TaskLoadTbl[nLoadTblIdx].GetBufferDeviceCnt();
			if (nBufferDvcCnt < (nLoadTblPocketCntX*nLoadTblPocketCntY / 2))
			{
				nLoadTableIdx = nLoadTblIdx;
				break;
			}
		}
	}

	if (nLoadTableIdx == -1)
		return eExcutingStep_ErrorJob;


	int nTblRes = g_TaskLoadTbl[nLoadTableIdx].GetWorkReserve();
	if (nTblRes == RID_INIT_IDLE_RESERVE || nTblRes == RID_NOT_RESERVE)
		return eExcutingStep_WaitOtherJob;

	if (bPickPlace == DEF_PICK && m_nCleanMode == TRUE)
		return eExcutingStep_ErrorJob;




	vPicker.clear();

	// find available hand picker
	int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y];
	for (int nPkrY = 0; nPkrY < MAX_TEST_PP_PKR_CNT_Y; nPkrY++)
	{
		for (int nPkrX = 0; nPkrX < MAX_TEST_PP_PKR_CNT_X; nPkrX++)
		{
			//필요한 Picker 1ea 담기 위함.
			nPickerDvcPattern[nPkrX][nPkrY] = DEF_EXIST;/*(bPickPlace == DEF_PICK ? DEF_EXIST : DEF_NOT_EXIST);*/
		}
	}

	//picker 한개 
	int nAvailableCount = GetDvcSwapArtBuffGrrHandAssign(DEF_PICK, nPickerDvcPattern, m_nJobAtsIdx);



	//Pocket 한개 만들기
	if (nAvailableCount > 0)
	{
		// get art buffer table
		int nArrVirtualBuff[VIRTUAL_POCKET_X_MAX][VIRTUAL_POCKET_Y_MAX];
		memset(nArrVirtualBuff, 0xFF, sizeof(nArrVirtualBuff));

		int nRetFillDvc = FillDvcGrr_VirtualTable_Swap(eDeviceStage_LD_TBL_1 + nLoadTableIdx, (int*)nArrVirtualBuff, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, bPickPlace, nPickerDvcPattern, nAtsIdx);
		if (nRetFillDvc != ERR_NO_ERROR)
			return eExcutingStep_ErrorJob;	


		int nUseJobPkr[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = { 0, };
		int nPockX = -1, nPockY = -1;
		int nRetCode = FindPickPlaceFromTableSwap(LOC_IDX_TEST_PP_TBL_1_LD + nLoadTableIdx, nArrVirtualBuff, nPickerDvcPattern, bPickPlace, &nPockX, &nPockY, nUseJobPkr);
		if (nRetCode > 0) {
			std::vector<CPoint> vPkr;
			for (int nPickX = 0; nPickX < MAX_TEST_PP_PKR_CNT_X; nPickX++) {
				for (int nPickY = 0; nPickY < MAX_TEST_PP_PKR_CNT_Y; nPickY++) {
					if (nUseJobPkr[nPickX][nPickY] != 0) {
						vPkr.push_back(CPoint(nPickX, nPickY));
					}
				}
			}
			switch (nLoadTableIdx)
			{
			case eLoadTbl_2:
			case eLoadTbl_3:
			{
				nLocIdx = LOC_IDX_TEST_PP_TBL_1_LD;
			}break;
			case eLoadTbl_1:
			case eLoadTbl_4:
			{
				nLocIdx = LOC_IDX_TEST_PP_TBL_1_UD;
			}break;
			default:
			{
				int a = 0;
			}break;
			}


			if (g_TaskLoadTbl[nLoadTableIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
				return eExcutingStep_WaitOtherJob;


			vPocket = CPoint(nPockX, nPockY);
			vPicker = vPkr;
			return eExcutingStep_FindJob;
		}
	}

	return eExcutingStep_ErrorJob;
}

int CTaskAutoTestPp::GetDvcCleanGrrHandAssign(BOOL bPickPlace, int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nWorkingAreaIdx)
{
	int nPkrPossibleCnt = 0;
	int nSiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	if (bPickPlace == DEF_PICK)
	{
		int nSocketDvc[STATION_MAX_X_SITE][STATION_MAX_Y_SITE] = { 0, };
		int nPkrDvc[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = { 0, };

		// site의 socket on/off 정보.
		ST_DD_DEVICE stDvc;
		for (int nX = 0; nX < nSiteCntX; nX++)
		{
			for (int nY = 0; nY < nSiteCntY; nY++)
			{
				//int nOnOff = g_TaskPressUnit[m_vModuleSetPressIdx[nWorkingAreaIdx]].GetSocketOnOff(nX, nY);
				int nOnOff = g_TaskPressUnit[m_vModuleSetPressIdx[nWorkingAreaIdx]].GetSocketOnOffGrr(nX, nY);
				if (nOnOff == DEF_SITE_ON)
				{
					stDvc.clear();
					g_TaskPressUnit[m_vModuleSetPressIdx[nWorkingAreaIdx]].GetSiteDeviceData(&stDvc, nX, nY);

					// cleaning device 가 들어있는 곳은 이미 넣은 곳이므로 집지 않는다.
					if (stDvc.sExist == DEF_EXIST && strcmp(stDvc.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0)
						nSocketDvc[nX][nY] = DEF_OFF;	// 불가능
					else
						nSocketDvc[nX][nY] = DEF_ON;	// 가능.
				}
				else {
					nSocketDvc[nX][nY] = DEF_OFF;	// 불가능
				}
			}
		}
		// load picker 정보
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{
				stDvc.clear();
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY, &stDvc);
				nPkrDvc[nX][nY] = stDvc.sExist;
			}
		}


		// 사용 가능한 picker 정보		
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{
				if (nSocketDvc[nX][nY] == DEF_ON && nPkrDvc[nX][nY] == DEF_NOT_EXIST) {
					nPickerDvcPattern[nX][nY] = DEF_NOT_EXIST; // 사용가능
					nPkrPossibleCnt++;
				}
			}
		}
	}
	else {
		ST_DD_DEVICE stDvc;
		// load picker 정보
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{
				stDvc.clear();
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY, &stDvc);

				if (stDvc.sExist == DEF_EXIST && strcmp(stDvc.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0) {
					nPickerDvcPattern[nX][nY] = DEF_EXIST;	// 사용가능
					nPkrPossibleCnt++;
				}
			}
		}
	}

	return nPkrPossibleCnt;



}

int CTaskAutoTestPp::GetDvcArtBuffGrrHandAssign(BOOL bPickPlace, int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nWorkingAreaIdx, int nStationWorkRow)
{
	ST_DD_DEVICE stDvc;
	int nPkrPossibleCnt = 0;
	if (nWorkingAreaIdx < 0) {
		return nPkrPossibleCnt;
	}

	if (bPickPlace == DEF_PICK)
	{
		int nSiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
		int nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
		int nSocketDvc[STATION_MAX_X_SITE][STATION_MAX_Y_SITE];
		int nPkrDvc[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y];
		memset(nSocketDvc, DEF_NOT_EXIST, sizeof(nSocketDvc));
		memset(nPkrDvc, DEF_NOT_EXIST, sizeof(nPkrDvc));

		for (int nX = 0; nX < nSiteCntX; nX++)
		{
			for (int nY = 0; nY < nSiteCntY; nY++)
			{
				int nOnOff = g_TaskPressUnit[m_vModuleSetPressIdx[nWorkingAreaIdx]].GetSocketOnOffGrr(nX, nY);
				if (nOnOff == DEF_SITE_ON)
				{
					stDvc.clear();
					g_TaskPressUnit[m_vModuleSetPressIdx[nWorkingAreaIdx]].GetSiteDeviceData(&stDvc, nX, nY);

					int ntest = stDvc.nTestingInfo;

					// site에 device가 없거나 또는 있더라도 test end / cleaning end 면 집을 수 있음.
					if (stDvc.sExist == DEF_NOT_EXIST || (stDvc.sExist == DEF_EXIST && (stDvc.nTestingInfo == eTestSite_TestEnd) && stDvc.stGrrTestInfo[m_vModuleSetPressIdx[nWorkingAreaIdx]].chkTestEnd(/*nX, nStationWorkRow*/) == TRUE))
					{
						nSocketDvc[nX][nY] = DEF_ON;	// 가능.
					}
					else
						nSocketDvc[nX][nY] = DEF_OFF;	// 불가능
				}
				else {
					nSocketDvc[nX][nY] = DEF_OFF;	// 불가능
				}
			}
		}
		// load picker 정보
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{
				nPkrDvc[nX][nY] = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY);
			}
		}
		// 사용 가능한 picker 정보		
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{
				if (nSocketDvc[nX][nY] == DEF_ON && nPkrDvc[nX][nY] == DEF_NOT_EXIST) {
					nPickerDvcPattern[nX][nY] = DEF_NOT_EXIST; // 사용가능
					nPkrPossibleCnt++;
				}
			}
		}
	}
	else {  //확인 필요
		// unload picker 정보
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
		{
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
			{
				stDvc.clear();
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY + TEST_UD_PKR_Y, &stDvc);
				// Hand Device 가 있어야 한다.
				// Test 3st이면 내려 놓을 수 없다.
				if (stDvc.sExist == DEF_EXIST)
				{
					/* test pp picker dvc가 아직 작업해야 할 station이 있는지 검사. */
					{
						int nJobAtsIdx = 0;
						//각각의 Device가 Station에서 몇번 Grr진행 했는지 확인.
						for (; nJobAtsIdx < (int)m_vModuleSetPressIdx.size(); nJobAtsIdx++)
						{
							//buffer에 place는 grr 해야하는 dvc 뿐..
// 							char sStnName[4];
// 							sprintf_s(sStnName, sizeof(sStnName), "%d", m_vModuleSetPressIdx[nJobAtsIdx]);
// 							int nFindIdx = stDvc.findIndexTestSite(sStnName);
// 							ASSERT(nFindIdx >= 0);
// 
// 							auto pTestInfo = &stDvc.stGrrTestInfo[nFindIdx];

							auto pTestInfo = &stDvc.stGrrTestInfo[m_vModuleSetPressIdx[nJobAtsIdx]];
							ASSERT(pTestInfo);
							if (pTestInfo->chkTestEnd() == FALSE)
								break; //아직 완료된 station이 아닌 것.
						}
						/* 모두 검사 통과했다면, place 할 수 없다. */
						if (nJobAtsIdx == (int)m_vModuleSetPressIdx.size())
							continue;
					}

					nPickerDvcPattern[nX][nY] = DEF_EXIST;	// 사용가능
					nPkrPossibleCnt++;
				}
			}
		}
	}

	return nPkrPossibleCnt;
}

int CTaskAutoTestPp::GetDvcSwapArtBuffGrrHandAssign(BOOL bPickPlace, int nPickerDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y], int nWorkingAreaIdx)
{
	ST_DD_DEVICE stDvc;
	int nPkrPossibleCnt = 0;
	if (nWorkingAreaIdx < 0) {
		return nPkrPossibleCnt;
	}

	int nSiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nSocketDvc[STATION_MAX_X_SITE][STATION_MAX_Y_SITE];
	int nPkrDvc[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y];
	memset(nSocketDvc, DEF_NOT_EXIST, sizeof(nSocketDvc));
	memset(nPkrDvc, DEF_NOT_EXIST, sizeof(nPkrDvc));

	for (int nY = 0; nY < nSiteCntY; nY++)
	{
		for (int nX = 0; nX < nSiteCntX; nX++)
		{
			int nOnOff = g_TaskPressUnit[m_vModuleSetPressIdx[nWorkingAreaIdx]].GetSocketOnOffGrr(nX, nY);
			if (nOnOff == DEF_SITE_ON)
			{
				stDvc.clear();
				g_TaskPressUnit[m_vModuleSetPressIdx[nWorkingAreaIdx]].GetSiteDeviceData(&stDvc, nX, nY);

				// site에 device가 없거나 또는 있더라도 test end / cleaning end 면 집을 수 있음.
				if (stDvc.sExist == DEF_NOT_EXIST || (stDvc.sExist == DEF_EXIST && (stDvc.nTestingInfo == eTestSite_TestEnd) && stDvc.stGrrTestInfo[m_vModuleSetPressIdx[nWorkingAreaIdx]].chkTestEnd(/*nX, nStationWorkRow*/) == TRUE))
				{
					nSocketDvc[nX][nY] = DEF_ON;	// 가능.
				}
				else
					nSocketDvc[nX][nY] = DEF_OFF;	// 불가능
			}
			else {
				nSocketDvc[nX][nY] = DEF_OFF;	// 불가능
			}
		}
	}
	// load picker 정보
	for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
	{
		for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
		{
			nPkrDvc[nX][nY] = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TEST_PP_1 + m_eTestIdx, nX, nY);
		}
	}
	// 사용 가능한 picker 정보		
	for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++)
	{
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++)
		{
			if (nSocketDvc[nX][nY] == DEF_ON && nPkrDvc[nX][nY] == DEF_NOT_EXIST) {
				nPickerDvcPattern[nX][nY] = DEF_NOT_EXIST; // 사용가능
				nPkrPossibleCnt++;
				return nPkrPossibleCnt;
			}
		}
	}


	return nPkrPossibleCnt;
}

int CTaskAutoTestPp::ChkJobGrr_Station(int nAtsIdx, BOOL bPickPlace, CPoint &cPocket, std::vector<CPoint> &vPicker)
{
	//Grr Cancel시, Station In Device 확인하도록 강화.
	BOOL bHasTest = g_TaskTestPP[m_eTestIdx].TestSdHasDvc();

 	if (bPickPlace == DEF_PLACE && bHasTest != TRUE && g_TaskSystemCtrl.GetAutoGrrCancel() == TRUE) {
 		return eExcutingStep_ErrorJob;
 	}

	CString strErrMsg = _T("");
	std::vector<CPoint> vErrPicker; vErrPicker.clear();
	std::vector<CString> vErrstring; vErrstring.clear();
	int nHandType = 0;
	int nErr = ChkDevice(strErrMsg, nHandType, vErrstring);
	if (nErr != ERR_NO_ERROR) {
		g_TaskSystemCtrl.SetErrTestPpIdx(m_eTestIdx, 1);
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Test PP %d %s", m_eTestIdx + 1, strErrMsg);
		for (int i = 0; i < (int)vErrstring.size(); i++)
		{
			AlarmData.m_fnAddItemFormat("%s", vErrstring[i]);
		}
		g_Error.AlarmReport(nErr, AlarmData, &g_TaskSystemCtrl, g_TaskTestPP[m_eTestIdx].GetFailPicker(nHandType));
		g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
		return eExcutingStep_ErrorJob;
	}

	int nChkInterlock = ChkInterlockAts(m_nJobAtsIdx);
	if (nChkInterlock == eExcutingStep_ErrorJob)
		return eExcutingStep_ErrorJob;

	if (LOC_IDX_STATION_1 + nAtsIdx < LOC_IDX_STATION_1 && LOC_IDX_STATION_1 + nAtsIdx > eATS_DN_STATION_L4)
		return eExcutingStep_ErrorJob;

// 	int nLocIdx = -1;
// 	int nJobSamePlace = ChkJobGrr_SamePlaceStation(cPocket, vPicker);
// 	int nJobArtBuffPlace = ChkJobGrr_ArtBuff(m_nJobAtsIdx, DEF_PLACE, cPocket, vPicker, nLocIdx);
// 	if (nJobSamePlace == eExcutingStepGrr_ErrorJob) //same place job 없음 & ArtBuffer
// 	{
// 	
// 	}

// 	//Swap Mode 진행 중이면 완료 후, Station Place
// 	BOOL bSwapMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrDeviceSwapUse);
// 	if (bSwapMode == TRUE && bPickPlace == DEF_PLACE)
// 	{
// 		int ntest_pp_load_hand_exist = g_TaskTestPP[m_eTestIdx].GetLoadHandDeviceCnt();
// 		int nbuffer_exist = g_TaskTestPP[m_eTestIdx].GetBufferTableCnt();
// 		if (ntest_pp_load_hand_exist == 0 && nbuffer_exist > 0) { //device exist 갯수로 변경 필요
// 			ChangeSubState(eExcutingStepGrr_WaitLoadTable);
// 			break;
// 		}
// 	}

	//if(bPickPlace == DEF_PLACE && g_TaskTestPP[m_eTestIdx].GetLoadPickerDeviceCnt() <= 0)
	//	return eExcutingStepGrr_ErrorJob;

	//if (bPickPlace == DEF_PICK && g_TaskTestPP[m_eTestIdx].GetLoadPickerDeviceCnt() > 0)
	//	return eExcutingStepGrr_ErrorJob;

// 	int nMaxBuff_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestBuffTbl_Div_Y);
// 	int nMaxBuff_X = g_DMCont.m_dmTestPP.GN(NDM4_TestBuffTbl_Div_X);
// 	if (g_TaskSystemCtrl.GetAutoGrrCancel() == FALSE)
// 	{
// 		int nStationOnCnt = g_TaskPressUnit[m_vModuleSetPressIdx[m_nJobAtsIdx]].GetSocketOnCntGrr();
// 		if (bPickPlace == DEF_PICK && g_TaskTestPP[m_eTestIdx].GetLoadPickerDeviceCnt() <= 0 && g_TaskTestPP[m_eTestIdx].GetBufferTableCnt() >= nStationOnCnt)//(nMaxBuff_Y*nMaxBuff_X) / 2)
// 			return eExcutingStepGrr_ErrorJob;
// 	}

	vPicker.clear();

	int nSiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	ST_DD_DEVICE stDvc;
	int nDevCnt = 0;
	for (int nX = 0; nX < nSiteCntX; nX++)
	{
		for (int nY = 0; nY < nSiteCntY; nY++)
		{
			int nOnOff = g_TaskPressUnit[m_vModuleSetPressIdx[nAtsIdx]].GetSocketOnOff(nX, nY);
			if (nOnOff == DEF_SITE_ON)
			{
				stDvc.clear();
				g_TaskPressUnit[m_vModuleSetPressIdx[nAtsIdx]].GetSiteDeviceData(&stDvc, nX, nY);
				if (stDvc.sExist == DEF_EXIST)
					nDevCnt++;
			}
		}
	}
	//Clean Device Mode인대 일반 Device가 Station에 있으면 내려놓지 말자
	BOOL bChkCleanDev = GetDvcStationCleanDevAssign(DEF_PLACE, nAtsIdx);
	if (bChkCleanDev == TRUE && nDevCnt > 0 && (/*bRetestPlace == TRUE ||*/ bPickPlace == DEF_PLACE))
		return eExcutingStep_ErrorJob;

	int nPkrDvcPattern[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = { 0, };
	int nPocketCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	// find available hand picker
	for (int nPkrY = 0; nPkrY < MAX_TEST_PP_PKR_CNT_Y; nPkrY++)
	{
		for (int nPkrX = 0; nPkrX < MAX_TEST_PP_PKR_CNT_X; nPkrX++)
		{
			nPkrDvcPattern[nPkrX][nPkrY] = (bPickPlace == DEF_PICK ? DEF_EXIST : DEF_NOT_EXIST);
		}
	}
	int nAvailableCnt = GetDvcStationHandAssignGrr(bPickPlace, nPkrDvcPattern);

	if (nAvailableCnt > 0)
	{
		int nArrVirtualStation[VIRTUAL_POCKET_X_MAX][VIRTUAL_POCKET_Y_MAX] = { 0, };
		memset(nArrVirtualStation, 0xFF, sizeof(nArrVirtualStation));
		//FillDvc_VirtualStation(eDeviceStage_TEST_SITE_1 + m_vModuleSetPressIdx[m_nJobAtsIdx], (int*)nArrVirtualStation, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, bPickPlace, (bPickPlace == DEF_EXCH ? TRUE : FALSE));
		FillDvcGrr_VirtualStation(eDeviceStage_TEST_SITE_1 + m_vModuleSetPressIdx[m_nJobAtsIdx], (int*)nArrVirtualStation, VIRTUAL_POCKET_X_MAX, VIRTUAL_POCKET_Y_MAX, bPickPlace);
		int nPockX = -1, nPockY = -1;
		int nUseJobPkr[MAX_TEST_PP_PKR_CNT_X][MAX_TEST_PP_PKR_CNT_Y] = { 0, };
		int nRetCnt = FindPickPlaceFromStation(m_nJobAtsIdx, nArrVirtualStation, nPkrDvcPattern, bPickPlace, &nPockX, &nPockY, nUseJobPkr);

		if (nRetCnt > 0)
		{
			std::vector<CPoint> vPkr;
			for (int nPickX = 0; nPickX < MAX_TEST_PP_PKR_CNT_X; nPickX++) {
				for (int nPickY = 0; nPickY < MAX_TEST_PP_PKR_CNT_Y; nPickY++) {
					if (nUseJobPkr[nPickX][nPickY] != 0) {
						vPkr.push_back(CPoint(nPickX, nPickY));
					}
				}
			}

			cPocket = CPoint(nPockX, nPockY);
			vPicker = vPkr;
			return eExcutingStep_FindJob;
		}
	}

	return eExcutingStepGrr_ErrorJob;
}

int CTaskAutoTestPp::GrrTableChangeLoc(int nLoc)
{
	int nRetIdx = -1;
	switch (nLoc)
	{
	case LOC_IDX_TEST_PP_TBL_1_LD: case LOC_IDX_TEST_PP_TBL_2_LD:
	{
		nRetIdx = LOC_IDX_TEST_PP_TBL_2_LD;
	}break;
	case LOC_IDX_TEST_PP_TBL_1_UD: case LOC_IDX_TEST_PP_TBL_2_UD:
	{
		nRetIdx = LOC_IDX_TEST_PP_TBL_2_UD;
	}break;
	case LOC_IDX_TEST_PP_TBL_3_LD: case LOC_IDX_TEST_PP_TBL_4_LD:
	{
		nRetIdx = LOC_IDX_TEST_PP_TBL_3_LD;
	}break;
	case LOC_IDX_TEST_PP_TBL_3_UD: case LOC_IDX_TEST_PP_TBL_4_UD:
	{
		nRetIdx = LOC_IDX_TEST_PP_TBL_3_UD;
	}break;
	default:
		break;
	}

	return nRetIdx;
}

// AutoGRR Site Group Reserve
void CTaskAutoTestPp::SetSiteGroupReserve(bool bReserve)
{
	if (bReserve == true)
	{
		// Table이 Test Side로 도착했으므로 AutoGRR에 대한 예약 설정을 해준다.
		int nLoadDeviceCnt = g_TaskLoadTbl[m_eTestIdx].GetLoadDeviceCnt();
		g_DMCont.m_dmTestPP.SN(NDM04_SITEGROUP1_RESERVED + m_eTestIdx, DEF_WORK_RESERVED);
		g_DMCont.m_dmTestPP.SN(NDM04_SITEGROUP1_RESERVED_DEVICE_CNT + m_eTestIdx, nLoadDeviceCnt);
		g_DMCont.m_dmTestPP.SN(NDM04_SITEGROUP1_INSERTED_DEVICE_CNT + m_eTestIdx, 0);
	}
	else
	{
		// Table을 Tray 보낼려고 하므로 예약 설정을 해제시킨다.
		g_DMCont.m_dmTestPP.SN(NDM04_SITEGROUP1_RESERVED + m_eTestIdx, DEF_WORK_FREE);
		g_DMCont.m_dmTestPP.SN(NDM04_SITEGROUP1_RESERVED_DEVICE_CNT + m_eTestIdx, 0);
		g_DMCont.m_dmTestPP.SN(NDM04_SITEGROUP1_INSERTED_DEVICE_CNT + m_eTestIdx, 0);
	}
}

BOOL CTaskAutoTestPp::ChkGrrAllSiteTestDone(int nDvcStage)
{
	ST_DD_DEVICE stDevice;
	stDevice.clear();

	int nPocketX = 0, nPocketY = 0;
	int nUdPicker = 0;
	switch (nDvcStage)
	{
		case eDeviceStage_TEST_PP_1:
		case eDeviceStage_TEST_PP_2:
		{
			nPocketX = MAX_TEST_PP_PKR_CNT_X;
			nPocketY = MAX_TEST_PP_PKR_CNT_Y;
			nUdPicker = TEST_UD_PKR_Y;
		}break;
		case eDeviceStage_TEST_SITE_1: case eDeviceStage_TEST_SITE_5:
		case eDeviceStage_TEST_SITE_2: case eDeviceStage_TEST_SITE_6:
		case eDeviceStage_TEST_SITE_3: case eDeviceStage_TEST_SITE_7:
		case eDeviceStage_TEST_SITE_4: case eDeviceStage_TEST_SITE_8:
		{
			nPocketX = STATION_MAX_X_SITE;
			nPocketY = STATION_MAX_Y_SITE;		
		}break;
		default:
			break;
	}


	int all_device_test_end = 0;
	for (int nY = 0; nY < nPocketY; nY++)
	{
		for (int nX = 0; nX < nPocketX; nX++)
		{
			g_DMCont.m_dmDevice.GetDeviceData(nDvcStage, nX, nY + nUdPicker, &stDevice);
			if (stDevice.sExist != DEF_EXIST) {
				all_device_test_end++;
				continue;
			}

			int nEndCnt = 0;
			/////Device 1ea 모든 Station Test End 확인
			for (auto Idx : m_vModuleSetPressIdx) {
				BOOL bTestEnd = stDevice.stGrrTestInfo[Idx].chkTestEnd();
				int nTestDoneCnt = stDevice.stGrrTestInfo[Idx].chkTestDone();
				int nSocketOnCnt = g_TaskPressUnit[Idx].GetSocketOnCntGrr();
				if (bTestEnd == TRUE && nTestDoneCnt == nSocketOnCnt)
					nEndCnt++;
			}

			if (nEndCnt == m_vModuleSetPressIdx.size()) {
				all_device_test_end++;
			}
		}
	}

	//해당 Stage에 있는 모든 Device Test 완료 확인.
	if (all_device_test_end != STATION_MAX_PARA && g_TaskSystemCtrl.GetAutoGrrCancel() != TRUE) {
		return FALSE;
	}

	//Test 완료
	return TRUE;
}

std::vector<CPoint> CTaskAutoTestPp::GetLossDevice()
{
	std::vector<CPoint> vLossPkr;
	vLossPkr.clear();
	for (int i = 0; i < eMaxTestPPTypeCount; i++)
	{
		if (m_vLossPkr[i].size() > 0)
		{
			vLossPkr = m_vLossPkr[i];
		}
	}
		
	return vLossPkr;
}

int CTaskAutoTestPp::CompSystemTime(SYSTEMTIME SysTime1, SYSTEMTIME SysTime2) //기존, 신규
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
		if (SysTime1.wMilliseconds > SysTime2.wMilliseconds) {
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

int CTaskAutoTestPp::FindStation_CompEOT() //EOT 제일 빠른거 찾는것
{
	int nRetStationIdx = -1;
	BOOL bFindFstStation = FALSE;

	SYSTEMTIME sysMaxTestTm;
	::GetLocalTime(&sysMaxTestTm);

	BOOL bAutoGrrUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);
	for (int i = 0; i < eMaxATS_Area; i++) {
		int nIdx = i + (eMaxATS_Area * m_eTestIdx);

		//Station On만 확인.
		int nRetSocket = g_TaskPressUnit[nIdx].GetSocketOnCnt(bAutoGrrUse);
		if (nRetSocket == 0)
			continue;

		/*double dTestTm = g_DMCont.m_dmEQP.GN(NDM0_Press1_TestTm + nIdx);*/
		int nCompResult = CompSystemTime(sysMaxTestTm, g_EOT[nIdx]);

		if (bFindFstStation == FALSE || nCompResult == 1) // g_EOT[nIdx] < sysMaxTestTm
		{
			sysMaxTestTm = g_EOT[nIdx];
			nRetStationIdx = i;

			if (bFindFstStation == FALSE)
				bFindFstStation = TRUE;
		}
	}

	return nRetStationIdx;
}

int CTaskAutoTestPp::GetFindLoadTableSearchX(eDeviceDataStage nDevLoc, int nTableLoc, int nLocIdx, int nPitchType_X, BOOL bPickPlace)
{
	int nSearchX = VIRTUAL_POCKET_X_START;

	int nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	eDeviceFullEmptyCheckCode tblDeviceFullEmpty = g_TaskAutoTrayLoadPP.LoadTableFullEmptyCheck(nDevLoc);

	/* X가변 하지 않을 때의 조건
		1. Clean Table일 경우,
		2. Unload Table에  Place 진행 할 때,
		3. Load Table Device Full 상황 일때
	*/
	if (nLocIdx == LOC_IDX_CLEAN_TBL || 
		(nTableLoc == DEF_TABLE_TYPE_LOAD_UNLOAD && bPickPlace == DEF_PLACE && nTestSite_Div_Y == 1 && g_TaskSystemCtrl.GetAutoGrrUse() == FALSE) || 
		(nTableLoc == DEF_TABLE_TYPE_LOAD_UNLOAD && bPickPlace == DEF_PICK && (nLocIdx == LOC_IDX_TEST_PP_TBL_2_LD || nLocIdx == LOC_IDX_TEST_PP_TBL_3_LD)  && tblDeviceFullEmpty == eFull && g_TaskSystemCtrl.GetAutoGrrUse() == FALSE)  )
	{
		nSearchX = VIRTUAL_POCKET_X_START;
	}
	else {
		nSearchX = VIRTUAL_POCKET_X_START - (MAX_TEST_PP_PKR_CNT_X - 1) * nPitchType_X;
	}

	return nSearchX;
}

int CTaskAutoTestPp::GetFindLoadTableSearchY(eDeviceDataStage nDevLoc, int nTableLoc, int nLocIdx, int nPitchType_Y, BOOL bPickPlace)
{
	int nSearchY = VIRTUAL_POCKET_Y_START;

	if (nTableLoc == DEF_TABLE_TYPE_BUFFER) {
		int nEQPNo = g_DMCont.m_dmEQP.GN(NDM0_Handler_EQP_No);
		if (nEQPNo == DEF_EQUIPMENT_UNIT1 && (nLocIdx == LOC_IDX_TEST_PP_TBL_2_LD || nLocIdx == LOC_IDX_TEST_PP_TBL_3_LD))
		{
			nSearchY = VIRTUAL_POCKET_Y_START;
		}
		else {
			nSearchY = VIRTUAL_POCKET_Y_START - (MAX_TEST_PP_PKR_CNT_Y - 1) * nPitchType_Y;
		}		
	}

	return nSearchY;
}

