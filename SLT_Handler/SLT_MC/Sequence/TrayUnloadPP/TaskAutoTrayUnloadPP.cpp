#include "stdafx.h"
#include "TaskAutoTrayLoadPP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace base_seq_cmd;
using namespace std;

CTaskAutoTrayUnloadPP::CTaskAutoTrayUnloadPP( void )
{
	m_nAutoRunMainStep = eAutoLotState_InitNeed;
	m_nAutoRunSubStep = 0;
	m_nAutoRunBackUpMainStep = eAutoLotState_InitNeed;
	m_nAutoRunBackUpSubStep = 0;

	m_mapExcuteState.SetAt(eAutoTrayStep_Run_Start                   , _T("Unload Tray Start"));

	m_mapExcuteState.SetAt(eAutoTrayStep_Run_Table_Vacuum_Check_job, _T("Unload Tray Table Vacuum Check Job"));
	m_mapExcuteState.SetAt(eAutoTrayStep_Run_Table_Vacuum_Check_Start, _T("Unload Tray Table Vacuum Check Start"));
	m_mapExcuteState.SetAt(eAutoTrayStep_Run_Table_Vacuum_Check_Wait, _T("Unload Tray Table Vacuum Check Wait"));
	m_mapExcuteState.SetAt(eAutoTrayStep_Run_Table_Vacuum_Check_Safety_move, _T("Unload Tray Table Vacuum Check Safety Move"));
	m_mapExcuteState.SetAt(eAutoTrayStep_Run_Table_Vacuum_Check_Safety_end, _T("Unload Tray Table Vacuum Check Safety end"));

	m_mapExcuteState.SetAt(eAutoTrayStep_Run_ResetSchedule			 , _T("Unload Tray Reset Schedule"));
	m_mapExcuteState.SetAt(eAutoTrayStep_Run_MakeSchedule			 , _T("Unload Tray Make Schedule"));
	m_mapExcuteState.SetAt(eAutoTrayStep_Run_TrayPpPickPlaceStart	 , _T("Unload Tray Pick Place Start"));
	m_mapExcuteState.SetAt(eAutoTrayStep_Run_TrayPpPickPlaceWait	 , _T("Unload Tray Pick Place Wait"));
	m_mapExcuteState.SetAt(eAutoTrayStep_Run_TrayPreMoveLoadTblStart , _T("Unload Tray PreMove LoadTbl Start"));
	m_mapExcuteState.SetAt(eAutoTrayStep_Run_TrayPP_MoveSafetyPos	 , _T("Unload Tray Move Safety Pos"));
	m_mapExcuteState.SetAt(eAutoTrayStep_Run_TrayPpComptCheck		 , _T("Unload Tray Compt Check"));
	m_mapExcuteState.SetAt(eAutoTrayStep_Run_End					 , _T("Unload Tray End"));

	std::vector< _ePressUnit > press_group_left_module;
	press_group_left_module.push_back(ePressUnit_Down_1);
	press_group_left_module.push_back(ePressUnit_Down_2);
	press_group_left_module.push_back(ePressUnit_Down_3);
	press_group_left_module.push_back(ePressUnit_Down_4);

	std::vector< _ePressUnit > press_group_right_module;
	press_group_right_module.push_back(ePressUnit_Down_5);
	press_group_right_module.push_back(ePressUnit_Down_6);
	press_group_right_module.push_back(ePressUnit_Down_7);
	press_group_right_module.push_back(ePressUnit_Down_8);

	m_test_modules_press_idx.push_back(press_group_left_module);
	m_test_modules_press_idx.push_back(press_group_right_module);

	m_nChkSafetyPosCnt = 0;
	m_nPickPlaceStep = 0;
	m_nPickPlaceBackUpStep = 0;
	m_nWorkRecLoadTbl = 0;
	m_nLoadTrayAutoSkipCount = 0;
	m_nFindWorkingShuttle = -1;
	m_nTblReworkNo = -1;

	m_cJobPocket = CPoint(0, 0);
	m_vJobPicker_Pick.clear();
	m_vJobPicker_Place.clear();
}


CTaskAutoTrayUnloadPP::~CTaskAutoTrayUnloadPP( void )
{
}

int CTaskAutoTrayUnloadPP::Initialize()
{
	m_eTrayIdx    = eTrayPP_2_UD;
	m_pTaskTrayPP = &g_TaskTrayUnloadPP;
	m_nJobTbl = -1;

	return CBaseTaskManager::Initialize();
}

void CTaskAutoTrayUnloadPP::Finalize()
{
	//FreeAllSchedule();
	CBaseTaskManager::Finalize();
}

int CTaskAutoTrayUnloadPP::OnEvent( const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList/*=NULL*/ )
{
	return ERR_NO_ERROR;
}

int CTaskAutoTrayUnloadPP::OnCommandRsp( CBaseSeqCmd* pRspCmd, int nEventId, WPARAM wParam, LPARAM lParam, std::vector<CPoint>& vPocket_Picker )
{
	if( nEventId == CMD_EVT_ALARM )
	{
		if( pRspCmd->m_pSender == this )
			ChangeMainStateByRspAlarm();
	}

	return CBaseTaskManager::OnCommandRsp( pRspCmd, nEventId, wParam, lParam, vPocket_Picker );
}

void CTaskAutoTrayUnloadPP::OnOperatorCommand( int nOperatorCommand, WPARAM wParam, LPARAM lParam )
{
}

void CTaskAutoTrayUnloadPP::ThreadProc1()
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

void CTaskAutoTrayUnloadPP::MakeLog( LPCTSTR fmt, ... )
{
	TCHAR tmpMsg[ 1024 ] = { 0, };
	if( fmt )
	{
		va_list argList;
		va_start( argList, fmt );
		_vstprintf_s( tmpMsg, fmt, argList );
		va_end( argList );
	}
	TWLOG.MakeLog( static_cast< int >( eLogID_Auto_Tray_2 ), Debug, "", __LINE__, NULL, tmpMsg );
}

void CTaskAutoTrayUnloadPP::AutoState_Idle()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 1000:
		{
			m_nLast_Excuting_Step = 0;
			ChangeSubState( 1001 );
		}break;
	case 1001:
		{
		}break;
	default:
		{
			ChangeSubState(1000);
		}break;
	}
}

void CTaskAutoTrayUnloadPP::AutoState_InitNeed()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 2000:
		{
			g_TaskTrayUnloadPP.SetProcessCommand(eProcCmd_Stop);
			ChangeSubState(2001);
		}break;
	case 2001:
		{
		}break;
	default:
		{
			ChangeSubState(2000);
		}break;
	}
}

void CTaskAutoTrayUnloadPP::AutoState_Initialize()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
		case 3000:
		{
			CTrayUnloadPPCmd_Initial* pSeqCmd = new CTrayUnloadPPCmd_Initial();
			if (SendCommand(*m_pTaskTrayPP, pSeqCmd) == ERR_NO_ERROR) {
				ChangeSubState(3001);
			}
		}break;
		
		case 3001:
		{
			if (m_pTaskTrayPP->GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready
				&& m_pTaskTrayPP->m_bInitialized == TRUE)
				ChangeSubState(3002);
		}break;
		
		case 3002: // Blow Warming up 기능 추가
		{
			int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
			int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);

			if ((nLotStartMode == eStartMD_Initial || nLotStartMode == eStartMD_Retest || nLotStartMode == eStartMD_QA || nLotStartMode == eStartMD_AutoLoop || nLotStartMode == eStartMD_AutoGRR))
			{
				if (nDeviceMode == eDeviceMode && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
				{
					for (int y = 0; y < MAX_TRAY_PP_CNT_Y; y++) {
						for (int x = 0; x < MAX_TRAY_PP_CNT_X; x++) {
							eOnOffState state = g_TaskTrayUnloadPP.GetVacuum(CPoint(x, y));
							if (state == eOff) {
								g_TaskTrayUnloadPP.do_VacBlow(eVacBlwSt_BlwOn, CPoint(x, y));
							}
						}
					}
					int nSleep = g_DMCont.m_dmTrayPP.GN(NDM5_TM_UDTrayPP_Blow);
					if (nSleep > 0) {
						Sleep(nSleep);
					}
					for (int y = 0; y < MAX_TRAY_PP_CNT_Y; y++) {
						for (int x = 0; x < MAX_TRAY_PP_CNT_X; x++) {
							eOnOffState state = g_TaskTrayUnloadPP.GetVacuum(CPoint(x, y));
							if (state == eOff) {
								g_TaskTrayUnloadPP.do_VacBlow(eVacBlwSt_Off, CPoint(x, y));
							}
						}
					}
				}
			}
			ChangeSubState(3100);
		}break;
		
		case 3100:
		{
			ChangeMainState(eAutoLotState_Idle);
		}break;
		
		default:
		{
			ChangeSubState(3000);
		}break;
	}
}

void CTaskAutoTrayUnloadPP::AutoState_Pause()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d] [last step=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep, m_nLast_Excuting_Step );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 4000:
		{
			g_TaskTrayUnloadPP.SetProcessCommand( eProcCmd_Pause );
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

void CTaskAutoTrayUnloadPP::AutoState_Execute()
{
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	m_nLast_Excuting_Step = m_nAutoRunSubStep;

	AutoState_Execute_Normal();
}

void CTaskAutoTrayUnloadPP::AutoState_NormalComplete()
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

		}break;
	default:
		ChangeSubState( 6000 );
		break;
	}
}

void CTaskAutoTrayUnloadPP::AutoState_AlarmStop()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d] [last step=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep, m_nLast_Excuting_Step );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 7000:
		{
			g_TaskTrayUnloadPP.SetProcessCommand( base_seq_cmd::eProcCmd_Pause );
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

void CTaskAutoTrayUnloadPP::AutoState_Resume()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d] [last step=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nLast_Excuting_Step);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 8000:
	{
		g_TaskTrayUnloadPP.SetProcessCommand(eProcCmd_Resume);

		ChangeSubState(8005);
		break;

		// 작업 중이냐? 작업 중이지 않느냐에 따라서 recovery가 달라져야함.
		if (m_pTaskTrayPP->GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			ChangeSubState(8002);
		}
		else { // 작업 중이면 일하도록 시킨다.
			ChangeSubState(8001);
		}
	}break;
	case 8001:
	{
		if (m_pTaskTrayPP->GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
			break;

		ChangeSubState(8002);
	}break;
	case 8002:
	{
		// tray pp y 축 안전 영역 이동 시켜야 함.
		CPoint pocket(0, 0);
		std::vector<CPoint> vPicker;
		vPicker.push_back(CPoint(0, 0));
		CTrayUnloadPPCmd_MovePos_Param prm(LOC_IDX_TRAY_PP_SAFETY, pocket, vPicker);
		CTrayUnloadPPCmd_MovePos* pSeqCmd = new CTrayUnloadPPCmd_MovePos(prm);
		if (SendCommand(*m_pTaskTrayPP, pSeqCmd) == eSeqCmdAck_OK) {
			ChangeSubState(8003);
			break;
		}

	}break;
	case 8003:
	{
		// tray pp y 축 안전 영역 이동 확인.
		if (m_pTaskTrayPP->GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
			break;

		ChangeSubState(8004);
	}break;
	case 8004:
	{
		if (static_cast<CTaskTrayUnloadPP*>(m_pTaskTrayPP)->is_safety_state() == true)
			g_conveyor_work_area.release_work_area(this);

		ChangeSubState(8005);
	}break;
	case 8005:
	{
		if (g_TaskTransfer.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
			break;

		MakeLog(_T("[MainStep=%s] [SubStep=%d] [last step=%d] [g_TaskTransfer.GetSeqCmdStatus()]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nLast_Excuting_Step);

		int nEndFeed = g_TaskAllAutoStacker.GetTrayEndFeed();
		switch (nEndFeed)
		{
		case DEF_LOTEND_TRAYEND:
		case DEF_LOTEND_TRAYFEED:
		{
			MakeLog(_T("[MainStep=%s] [SubStep=%d] [last step=%d] [nEndFeed = %d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nLast_Excuting_Step, nEndFeed);
			ChangeMainState(eAutoLotState_CleanOut);
		}break;
		}

		MakeLog(_T("[MainStep=%s] [SubStep=%d] [last step=%d] [ChangeMainState(eAutoLotState_Executing)]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nLast_Excuting_Step);
		ChangeMainState(eAutoLotState_Executing, m_nLast_Excuting_Step);
	}break;
	default:
		ChangeSubState(8000);
		break;
	}
}

void CTaskAutoTrayUnloadPP::AutoState_CleanOut()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 9000:
	{
		CPoint pocket(0, 0);
		std::vector< CPoint > vPicker = { CPoint(1, 0) };
		CTrayUnloadPPCmd_MovePos_Param prm(LOC_IDX_TRAY_PP_TBL_1, pocket, vPicker);

		CTrayUnloadPPCmd_MovePos* pCmd = new CTrayUnloadPPCmd_MovePos(prm);

		if (SendCommand(*m_pTaskTrayPP, pCmd) == eSeqCmdAck_OK) {
			ChangeSubState(9999);
		}
	}break;
	case 9999:
	{
		if (m_pTaskTrayPP->GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
			break;

		// Lot가 진행 되면 Blow를 꺼준다 [9/16/2020 donghyun.shin]
		m_pTaskTrayPP->do_VacBlow_All(eVacBlwSt_Off);

		ChangeMainState(eAutoLotState_NormalComplete);

	}break;
	default:
		ChangeSubState(9000);
		break;
	}
}


int CTaskAutoTrayUnloadPP::_MakeJobPlaceToBuffer()
{
	return 0;
}

int CTaskAutoTrayUnloadPP::_MakeJobPickFromBuffer()
{
	return 0;
}

