#include "StdAfx.h"
#include "TaskAutoPress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace base_seq_cmd;
using namespace std;

CTaskAutoPress::CTaskAutoPress(void)
{
	m_nAutoRunMainStep = eAutoLotState_InitNeed;
	m_nAutoRunSubStep = 0;
	m_nAutoRunBackUpMainStep = eAutoLotState_InitNeed;
	m_nAutoRunBackUpSubStep = 0;
	m_ePressIndex = ePressUnit_Down_1;
	m_eATSIdx     = eATS_DN_STATION_L1;

	m_eTestPpIndex = eTestPP_1;

	m_nAutoCurExecutingStep = 0;

	m_vModuleSetPressIdx.clear();

	m_mapExcuteState.SetAt( eExcutingStep_Start                     , _T( "Start" ) );
	m_mapExcuteState.SetAt( eExcutingStep_End                       , _T( "End" ) );

	m_mapExcuteState.SetAt(eExcutingStep_Move_SafetyPos             , _T("Move Safety"));
	m_mapExcuteState.SetAt(eExcutingStep_WaitEnd_SafetyPos          , _T("WaitEnd Safety"));
	
	m_mapExcuteState.SetAt(eExcutingStep_Move_Contact               , _T("Move Contact"));
	m_mapExcuteState.SetAt(eExcutingStep_WaitEnd_Contact            , _T("WaitEnd Contact"));
	m_mapExcuteState.SetAt(eExcutingStep_TestStart                  , _T("Test Start"));
	
	m_mapExcuteState.SetAt(eExcutingStep_Move_Cleaning              , _T("Move Cleaning"));
	m_mapExcuteState.SetAt(eExcutingStep_WaitEnd_Cleaning           , _T("WaitEnd Cleaning"));
}


CTaskAutoPress::~CTaskAutoPress(void)
{
}

int CTaskAutoPress::Initialize( TCHAR * szDriverFileName, _ePressUnit ePressIndex )
{
	m_ePressIndex = ePressIndex;

	switch(ePressIndex)
	{
	case ePressUnit_Down_1:  //case ePressUnit_Up_1:
	case ePressUnit_Down_2:  //case ePressUnit_Up_2:
	case ePressUnit_Down_3:  //case ePressUnit_Up_3:
	case ePressUnit_Down_4:  //case ePressUnit_Up_4:
		{
			m_eTestPpIndex = eTestPP_1;
			for(int i=0; i<eMaxATS_Area/2; i++){
				m_vModuleSetPressIdx.push_back(ePressUnit_Down_1+i);
				//m_vModuleSetPressIdx.push_back(ePressUnit_Up_1+i);
			}
		}break;
	case ePressUnit_Down_5:  //case ePressUnit_Up_5:
	case ePressUnit_Down_6: // case ePressUnit_Up_6:
	case ePressUnit_Down_7:  //case ePressUnit_Up_7:
	case ePressUnit_Down_8: // case ePressUnit_Up_8:
		{
			m_eTestPpIndex = eTestPP_2;
			for(int i=0; i<eMaxATS_Area/2; i++){
				m_vModuleSetPressIdx.push_back(ePressUnit_Down_5+i);
				//m_vModuleSetPressIdx.push_back(ePressUnit_Up_5+i);
			}
		}break;
	}

	switch(ePressIndex)
	{
	case ePressUnit_Down_1:  case ePressUnit_Down_5:
		{
			m_eATSIdx     = eATS_DN_STATION_L1;
		}break;
	case ePressUnit_Down_2:  case ePressUnit_Down_6:
		{
			m_eATSIdx     = eATS_DN_STATION_L2;
		}break;
	case ePressUnit_Down_3:  case ePressUnit_Down_7:
		{
			m_eATSIdx     = eATS_DN_STATION_L3;
		}break;
	case ePressUnit_Down_4:  case ePressUnit_Down_8:
		{
			m_eATSIdx     = eATS_DN_STATION_L4;
		}break;
// 	case ePressUnit_Up_5:  case ePressUnit_Up_1:
// 		{
// 			m_eATSIdx     = eATS_UP_STATION_L1;
// 		}break;
// 	case ePressUnit_Up_6:  case ePressUnit_Up_2:
// 		{
// 			m_eATSIdx     = eATS_UP_STATION_L2;
// 		}break;
// 	case ePressUnit_Up_7:  case ePressUnit_Up_3:
// 		{
// 			m_eATSIdx     = eATS_UP_STATION_L3;
// 		}break;
// 	case ePressUnit_Up_8:  case ePressUnit_Up_4:
// 		{
// 			m_eATSIdx     = eATS_UP_STATION_L4;
// 		}break;
	}

	return CBaseTaskManager::Initialize();
}

void CTaskAutoPress::Finalize()
{
	CBaseTaskManager::Finalize();
}


int CTaskAutoPress::OnEvent( const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList/*=NULL*/ )
{
	return ERR_NO_ERROR;
}

int CTaskAutoPress::OnCommandRsp( CBaseSeqCmd * pRspCmd, int nEventId, WPARAM wParam, LPARAM lParam, std::vector<CPoint>& vPocket_Picker )
{
	if (nEventId == CMD_EVT_ALARM)
	{
		if (pRspCmd->m_pSender == this)
			ChangeMainStateByRspAlarm();
	}

	if( pRspCmd->m_pReceive == &g_TaskPressUnit[m_ePressIndex] )
	{
		if(nEventId == CMD_EVT_CMD_END){
			if(CBaseTaskManager::m_bEqpControlMode == eSeqMode_Auto ){
				//checking yield monitoring
				if( pRspCmd->m_nCmdId == eSeqCmd_PressUnit_MovePos )
				{
					MakeLog(_T("2. Contact End Safety Press %d"), m_ePressIndex + 1);
					g_DMCont.m_dmEQP.SB(BDM0_PRESS_CONTACT_STATUS_SITE1 + m_ePressIndex, FALSE);    // Set Contact status off
				
				}
				// index time start [EOT]
				if(pRspCmd->m_nCmdId == eSeqCmd_PressUnit_OfflineTest || pRspCmd->m_nCmdId == eSeqCmd_PressUnit_OnlineTest)
				{
					MakeLog(_T("1. EOT Press %d"), m_ePressIndex+1 );
					g_DMCont.m_dmEQP.SB(BDM0_TEST_STATUS_SITE1+m_ePressIndex, FALSE);    // Set testing status off

					g_DMCont.m_dmEQP.SB(BDM0_PRESS_CONTACT_STATUS_SITE1 + m_ePressIndex, FALSE); // Set Contact status off

// 					BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
// 					if (bAutoGrrUse != TRUE)
// 					{
// 						std::vector<CPoint> vAlarmSet;
// 						g_TaskSystemCtrl.CalcYieldContiFailMonitoring(m_ePressIndex, vAlarmSet);
// 						g_TaskSystemCtrl.CalcYieldSampleMonitoring(m_ePressIndex, vAlarmSet);
// 						g_TaskSystemCtrl.CalcYieldContiFailMonitoring_Station(m_ePressIndex, vAlarmSet);
// 						g_TaskSystemCtrl.CalcYield1stFailMonitoring(m_ePressIndex, vAlarmSet);
// 						//g_TaskSystemCtrl.CalcYieldVarianceMonitoring();
// 					}
					//RecordDataOEE();
					
				}
			}
		}
	}	

	return	CBaseTaskManager::OnCommandRsp( pRspCmd, nEventId, wParam, lParam, vPocket_Picker );
}

void CTaskAutoPress::OnOperatorCommand( int nOperatorCommand, WPARAM wParam, LPARAM lParam )
{
}

void CTaskAutoPress::ThreadProc1()
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

void CTaskAutoPress::MakeLog( LPCTSTR fmt, ... )
{
	TCHAR tmpMsg[ 1024 ] = { 0, };
	if( fmt )
	{
		va_list argList;
		va_start( argList, fmt );
		_vstprintf_s( tmpMsg, fmt, argList );
		va_end( argList );
	}
	TWLOG.MakeLog( static_cast<int>( eLogID_Auto_Press_1 + m_ePressIndex ), Debug, "", __LINE__, NULL, tmpMsg );
}

void CTaskAutoPress::AutoState_Idle()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 1000:
		{
			ChangeSubState(1001);
		}break;
	case 1001:
		{

		}break;
	default :
		ChangeSubState(1000);
		break;
	}
}

void CTaskAutoPress::AutoState_InitNeed()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 2000:
		{
			g_TaskPressUnit[ m_ePressIndex ].SetProcessCommand( eProcCmd_Stop );
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

void CTaskAutoPress::AutoState_Initialize()
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

			double dForcePerPin = g_DMCont.m_dmHandlerOpt.GD(DDM11_DvcContactForce_GF);
			if (dForcePerPin <= 0.0) {
				dForcePerPin = 0.0;
			}

			g_TaskRegulator[m_ePressIndex].do_SetForcePerPin(dForcePerPin);
			Sleep(500);

			int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
			BOOL bAutoUseGRR = FALSE;
			if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
				bAutoUseGRR = TRUE;

			if (g_TaskAutoTestPp[m_eTestPpIndex].GetModuleSocketOnCnt(bAutoUseGRR) == 0 
				&& g_TaskAutoTestPp[m_eTestPpIndex].GetModuleAuditSocketOnCnt() == 0
				&& strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
				ChangeSubState(3100);
				break;
			}

			CPressUnitCmd_Initial* pSeqCmd = new CPressUnitCmd_Initial;
			if( SendCommand( g_TaskPressUnit[ m_ePressIndex ], pSeqCmd ) == eSeqCmdAck_OK)
				ChangeSubState(3001);
		}break;
	case 3001 :
		{
			if(  g_TaskPressUnit[ m_ePressIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready
			&& g_TaskPressUnit[ m_ePressIndex ].m_bInitialized == TRUE )
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

void CTaskAutoPress::AutoState_Pause()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d] [LastStep=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nLast_Excuting_Step);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 4000:
		{
			g_TaskPressUnit[ m_ePressIndex ].SetProcessCommand( eProcCmd_Pause );
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

void CTaskAutoPress::AutoState_Execute()
{
	//if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
	//	MakeLog( _T( "[MainStep=%s] [SubStep=%s]" ), GetLotStateToString( m_nAutoRunMainStep ), GetExcuteStateStr() );
	//}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	if( m_nAutoRunSubStep > eExcutingStep_End ) {
		m_nAutoRunSubStep = m_nLast_Excuting_Step;
	}
	m_nLast_Excuting_Step = m_nAutoRunSubStep;
// 	if(m_ePressIndex >= ePressUnit_Down_5 && m_ePressIndex <= ePressUnit_Down_8)
// 		return;

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	SetCurExecutingStep(m_nAutoRunSubStep);

	if (UseAbleCheck() != TRUE && strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
		return;
	}

	switch( m_nAutoRunSubStep )
	{
	case eExcutingStep_Start:
		{
			ChangeSubState(eExcutingStep_Check_Job);
		}break;
	case eExcutingStep_Check_Job:
		{
			int nJobState = FindJobState();
			if (nJobState != eExcutingStep_ErrorJob) {
				ChangeSubState(nJobState);
				MakeLog(_T("[EXECUTE], find job state = %d"), nJobState);
			}
		}break;

		// safety loop
	case eExcutingStep_Move_SafetyPos:
		{
			if( g_TaskPressUnit[ m_ePressIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready )
			{
				CPressUnitCmd_MovePos* pSeqCmd = new CPressUnitCmd_MovePos;
				pSeqCmd->m_nCmdStage = LOC_IDX_PRESS_UNIT_SAFETY;
				if( SendCommand( g_TaskPressUnit[ m_ePressIndex ], pSeqCmd ) == eSeqCmdAck_OK) {
					MakeLog(_T("[EXECUTE], move safety pos"), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr());
					ChangeSubState(eExcutingStep_WaitEnd_SafetyPos);
				}
			}
		}break;
	case eExcutingStep_WaitEnd_SafetyPos:
		{
			if (g_TaskPressUnit[m_ePressIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				g_testpp_work_area[m_eTestPpIndex].release_work_area(m_ePressIndex, this);
				ChangeSubState(eExcutingStep_Check_Job);
			}				
		}break;

		// contact loop
	case eExcutingStep_Check_Work_Area_Enable_Contact :
		{
			//해당 Idx는 Load Already Place 완료된 Station
			int nChkAleadyPlace = g_DMCont.m_dmTestPP.GN(NDM04_TESTHAND1_ALREADY_PLACE_WORKING_IDX + m_eTestPpIndex);
			int nAreaIdx = g_TaskPressUnit[m_ePressIndex].CovertPressAreaIdx(m_ePressIndex);


			if (nChkAleadyPlace == nAreaIdx)
			{
				int a = 0;
			}

			if (g_testpp_work_area[m_eTestPpIndex].is_work_enable_state(m_ePressIndex, this) == true)
			{
				int a = 0;
			}

			if ((g_testpp_work_area[m_eTestPpIndex].is_work_enable_state(m_ePressIndex, this) == true) || (nChkAleadyPlace == nAreaIdx))
			{
				//1. X축 Safety Sensor 확인.
				int nSafetyX1Sen = g_TaskTestPP[m_eTestPpIndex].ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON);
				int nSafetyX2Sen = g_TaskTestPP[m_eTestPpIndex].ChkDI(CTaskTestPP::xX2AXIS_POSITION_SEN, DEF_ON);
				if ((nSafetyX1Sen != ERR_NO_ERROR || nSafetyX2Sen != ERR_NO_ERROR) && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_) {
					break;
				}


				//2. X축 Safety Position 확인.
 				OneAxis* pMotorX1 = g_TaskTestPP[m_eTestPpIndex].GetAxis(CTaskTestPP::eAxisX1);
 				OneAxis* pMotorX2 = g_TaskTestPP[m_eTestPpIndex].GetAxis(CTaskTestPP::eAxisX2);
 				double dCurPosX1 = 0.0;
 				double dCurPosX2 = 0.0;
 
 				pMotorX1->GetCurrentPos(dCurPosX1);
 				pMotorX2->GetCurrentPos(dCurPosX2);
 
 				double dTestPP_X_SafetyPos = (m_eTestPpIndex == eTestPP_1) ? INTERLOCK_TESTPP_X_SAFETY_POS*(-1) : INTERLOCK_TESTPP_X_SAFETY_POS;
 
 				//X1, X2
 				if (g_TaskTestPP[m_eTestPpIndex].CompareXData(dCurPosX1, dTestPP_X_SafetyPos) != TRUE
 					|| g_TaskTestPP[m_eTestPpIndex].CompareXData(dCurPosX2, dTestPP_X_SafetyPos) != TRUE) {
 					break;
  				}

				bool work_enable = g_testpp_work_area[m_eTestPpIndex].reserve_work_area(m_ePressIndex, this);
				if (work_enable == true || nChkAleadyPlace == nAreaIdx)
				{
					BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
					if (g_TaskPressUnit[m_ePressIndex].PressInCleanDeviceCheck(bAutoGrrUse) == true)
					{	
						MakeLog(_T("[EXECUTE], Clean Device contact release area -> Start"));
						g_testpp_work_area[m_eTestPpIndex].release_work_area(m_ePressIndex, this);
						ChangeSubState(eExcutingStep_Start);					
						break;
					}

					// Contact 준비 중인대, Socket에 Device가 없다면, 다시 Job 생성 으로 Step 변경 [10/28/2021 donghyun.shin]
					int nDvc = g_TaskPressUnit[m_ePressIndex].ChkSiteInDvc();
					if (nDvc == eDeviceData_None)
					{
						MakeLog(_T("[EXECUTE], [eExcutingStep_Move_Contact -> eExcutingStep_Start], DeviceExist = [%d]"), nDvc);
						g_testpp_work_area[m_eTestPpIndex].release_work_area(m_ePressIndex, this);
						ChangeSubState(eExcutingStep_Start);
						break;
					}

					MakeLog(_T("[EXECUTE], contact reserve area"));
					ChangeSubState(eExcutingStep_Move_Contact);									
				}
			}
		}break;
	case eExcutingStep_Move_Contact :
		{
			if( g_TaskPressUnit[ m_ePressIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready )
			{
				int nChkPos = g_TaskPressUnit[m_ePressIndex].ChkMotorPos(LOC_IDX_PRESS_UNIT_CONTACT, CTaskPressUnit::eAxisZ);
				BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
				BOOL bAutoLoopUse = g_TaskSystemCtrl.GetAutoLoopUse();

				if (nChkPos == ERR_NO_ERROR && (bAutoGrrUse || bAutoLoopUse)) {
					ChangeSubState(eExcutingStep_TestStart);
				}
				else {
					CPressUnitCmd_Contact* pSeqCmd = new CPressUnitCmd_Contact;
					pSeqCmd->m_nCmdStage = LOC_IDX_PRESS_UNIT_CONTACT;

					MakeLog(_T("[EXECUTE], contact"));
					if (SendCommand(g_TaskPressUnit[m_ePressIndex], pSeqCmd) == eSeqCmdAck_OK) {
						ChangeSubState(eExcutingStep_WaitEnd_Contact);
					}

					g_DMCont.m_dmTestPP.SN(NDM04_TESTHAND1_ALREADY_PLACE_WORKING_IDX + m_eTestPpIndex, -1);
				}
			}
		}break;
	case eExcutingStep_WaitEnd_Contact :
		{
			if( g_TaskPressUnit[ m_ePressIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready )
				ChangeSubState(eExcutingStep_TestStart);
		}break;
	case eExcutingStep_TestStart :
		{
			if( g_TaskPressUnit[ m_ePressIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready )
			{
				MakeLog(_T("[EXECUTE], test start"));

				int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
				int m_nFTRCQC = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
				BOOL bAutoQAUse = g_TaskSystemCtrl.GetAutoQAUse();
				int nQAStatus = g_TaskSystemCtrl.GetQAStatus();
				
				if (bAutoQAUse == TRUE && nQAStatus != eQAStatus_Load && nQAStatus != eQAStatus_LoadEnd)
				{
					CPressUnitCmd_QAEmptyTest*  pSeqCmd = new CPressUnitCmd_QAEmptyTest;
					pSeqCmd->m_nCmdStage = m_eATSIdx;
					if (SendCommand(g_TaskPressUnit[m_ePressIndex], pSeqCmd) == eSeqCmdAck_OK) {
						ChangeSubState(eExcutingStep_WaitTestEnd);
						break;
					}
				}
				else 
				{
					if (nRunMode == eRUN_MODE_OFFLINE) {
						g_TaskPressUnit[m_ePressIndex].SetPressReqTable(TRUE, CTaskLoadTable::eMaxTblType);

						CPressUnitCmd_OffLineTest*  pSeqCmd = new CPressUnitCmd_OffLineTest;
						pSeqCmd->m_nCmdStage = m_eATSIdx;
						if (SendCommand(g_TaskPressUnit[m_ePressIndex], pSeqCmd) == eSeqCmdAck_OK) {
							ChangeSubState(eExcutingStep_WaitTestEnd);
							break;
						}
					}
					else {
						CPressUnitCmd_OnLineTestApple*  pSeqCmd = new CPressUnitCmd_OnLineTestApple;
						pSeqCmd->m_nCmdStage = m_eATSIdx;
						if (SendCommand(g_TaskPressUnit[m_ePressIndex], pSeqCmd) == eSeqCmdAck_OK) {
							ChangeSubState(eExcutingStep_WaitTestEnd);
							break;
						}

					}
				}				
			}
		}break;
	case eExcutingStep_WaitTestEnd:
		{
			if( g_TaskPressUnit[ m_ePressIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready )
			{
				MakeLog(_T("[EXECUTE], test end"));
				ChangeSubState(eExcutingStep_Move_SafetyPos_TestEnd);			
			}
		}break;
	case eExcutingStep_Move_SafetyPos_TestEnd :
		{
			if (g_TaskPressUnit[m_ePressIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
				int nStationInfo = g_TaskPressUnit[m_ePressIndex].GetStationInfo(bAutoGrrUse);
				BOOL bAutoLoopUse = g_TaskSystemCtrl.GetAutoLoopUse();
			
				if (nStationInfo == eTestSite_TestEnd) {
					CPressUnitCmd_MovePos* pSeqCmd = new CPressUnitCmd_MovePos;
					pSeqCmd->m_nCmdStage = LOC_IDX_PRESS_UNIT_SAFETY;

					MakeLog(_T("[EXECUTE], move safety test end"));
					if (SendCommand(g_TaskPressUnit[m_ePressIndex], pSeqCmd) == eSeqCmdAck_OK) {
						ChangeSubState(eExcutingStep_WaitEnd_SafetyPos_TestEnd);
					}
				}
				else if ((bAutoGrrUse == TRUE || bAutoLoopUse == TRUE ) && nStationInfo == eTestSite_DvcLoading) {
					ChangeSubState(eExcutingStep_Check_Job);
				}
				
			}
		}break;
	case eExcutingStep_WaitEnd_SafetyPos_TestEnd:
		{
			if (g_TaskPressUnit[m_ePressIndex].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
			{
				MakeLog(_T("[EXECUTE], contact release area"));
				g_testpp_work_area[m_eTestPpIndex].release_work_area(m_ePressIndex, this);
				ChangeSubState(eExcutingStep_Check_Job);
			}
		}break;

		// cleaning loop
	case eExcutingStep_Check_Work_Area_Enable_Cleaning :
		{
			if (g_testpp_work_area[m_eTestPpIndex].is_work_enable_state(m_ePressIndex, this) == true)
			{
				MakeLog(_T("[EXECUTE], cleaning release area"));
				bool work_enable = g_testpp_work_area[m_eTestPpIndex].reserve_work_area(m_ePressIndex, this);
				if (work_enable == true)
					ChangeSubState(eExcutingStep_Move_Cleaning);
			}
			
		}break;
	case eExcutingStep_Move_Cleaning:
		{
			if( g_TaskPressUnit[ m_ePressIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready )
			{
				CPressUnitCmd_SocketCleanContact* pSeqCmd = new CPressUnitCmd_SocketCleanContact;
				MakeLog(_T("[EXECUTE], cleaning contact"));
				if( SendCommand( g_TaskPressUnit[ m_ePressIndex ], pSeqCmd ) == eSeqCmdAck_OK) {
					ChangeSubState(eExcutingStep_WaitEnd_Cleaning);
				}
			}
		}break;
	case eExcutingStep_WaitEnd_Cleaning :
		{
			if( g_TaskPressUnit[ m_ePressIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready )
				ChangeSubState(eExcutingStep_Move_SafetyPos_Cleaning);
		}break;
	case eExcutingStep_Move_SafetyPos_Cleaning :
		{
			if( g_TaskPressUnit[ m_ePressIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready )
			{
				CPressUnitCmd_MovePos* pSeqCmd = new CPressUnitCmd_MovePos;
				MakeLog(_T("[EXECUTE], cleaning done move safety"));
				pSeqCmd->m_nCmdStage = LOC_IDX_PRESS_UNIT_SAFETY;
				if( SendCommand( g_TaskPressUnit[ m_ePressIndex ], pSeqCmd ) == eSeqCmdAck_OK) {
					ChangeSubState(eExcutingStep_WaitEnd_SafetyPos_Cleaning);
				}
			}
		}break;
	case eExcutingStep_WaitEnd_SafetyPos_Cleaning :
		{
			if( g_TaskPressUnit[ m_ePressIndex ].GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready ){
				MakeLog(_T("[EXECUTE], cleaning release, flag done"));
				g_testpp_work_area[m_eTestPpIndex].release_work_area(m_ePressIndex, this);
				//cleaning 관련 flag 초기화
				g_DMCont.m_dmEQP.SB(BDM0_SOCKETCLEAN_SITE1_RESERVE + m_ePressIndex, FALSE);
				g_DMCont.m_dmHandlerOpt.SB(BDM11_MANU_CLN_REV_SITE1 + m_ePressIndex, FALSE);

				ChangeSubState(eExcutingStep_Check_Job);
			}
		}break;

	case eExcutingStep_End:
		{
			//int nLotEnd = CheckLotComplete();
			ChangeMainState( eAutoLotState_NormalComplete );
		}break;
	}

	m_nLast_Excuting_Step = m_nAutoRunSubStep;
}

void CTaskAutoPress::AutoState_NormalComplete()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d] [LastStep=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nLast_Excuting_Step);
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
			//ChangeMainState( eAutoLotState_Idle );
		}break;
	default :
		ChangeSubState(6000);
		break;
	}
}

void CTaskAutoPress::AutoState_AlarmStop()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d] [LastStep=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nLast_Excuting_Step);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 7000:
		{
			g_TaskPressUnit[ m_ePressIndex ].SetProcessCommand(base_seq_cmd::eProcCmd_Pause);
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

void CTaskAutoPress::AutoState_Resume()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d] [LastStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep, m_nLast_Excuting_Step);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
	case 8000:
		{
			g_TaskPressUnit[ m_ePressIndex ].SetProcessCommand( eProcCmd_Resume );
			ChangeSubState(8001);
		}break;
	case 8001:
		{

			if( g_TaskPressUnit[ m_ePressIndex ].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready )
				break;

			
			ChangeSubState(8100);
		}break;
	case 8100:
		{
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

			//ChangeMainState( eAutoLotState_Executing, m_nLast_Excuting_Step );
		}break;
	default :
		ChangeSubState(8000);
		break;
	}
}

void CTaskAutoPress::AutoState_CleanOut()
{
	if( m_nAutoRunBackUpSubStep != m_nAutoRunSubStep ) {
		MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch( m_nAutoRunSubStep )
	{
		case 9000:
			{
				ChangeSubState( 9999 );
			}break;
		case 9999:
			{
				ChangeMainState( eAutoLotState_NormalComplete );
			}break;
		default:
			ChangeSubState( 9000 );
			break;
	}
}


int CTaskAutoPress::FindJobState()
{
	// interlock
	if (g_TaskPressUnit[m_ePressIndex].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
		return eExcutingStep_ErrorJob;

	int nRetState = eExcutingStep_ErrorJob;

	BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
	int nStationInfo = g_TaskPressUnit[m_ePressIndex].GetStationInfo(bAutoGrrUse);

	if (nStationInfo == eTestSite_Empty || nStationInfo == eTestSite_TestEnd || nStationInfo == eTestSite_CleanEnd)
	{
		int nChkMotSafety = g_TaskPressUnit[m_ePressIndex].ChkMotorPos(LOC_IDX_PRESS_UNIT_SAFETY, CTaskPressUnit::eAxisZ);
		int nChkSenSafety = g_TaskPressUnit[m_ePressIndex].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON);
		if (nChkMotSafety != ERR_NO_ERROR || nChkSenSafety != ERR_NO_ERROR)
			nRetState = eExcutingStep_Move_SafetyPos;
	}
	else if (nStationInfo == eTestSite_DvcLoading) {
		nRetState = eExcutingStep_Check_Work_Area_Enable_Contact;
	}
	else if (nStationInfo == eTestSite_CleanLoading) {
		nRetState = eExcutingStep_Check_Work_Area_Enable_Cleaning;
	}
	else if (nStationInfo == eTestSite_Contacting || nStationInfo == eTestSite_ContactStart)
	{
		int nChkMotSafety = g_TaskPressUnit[m_ePressIndex].ChkMotorPos(LOC_IDX_PRESS_UNIT_CONTACT, CTaskPressUnit::eAxisZ);
		if (nChkMotSafety != ERR_NO_ERROR)
			nRetState = eExcutingStep_Check_Work_Area_Enable_Contact;
	}

	if (nStationInfo != eTestSite_TestStart && nStationInfo != eTestSite_Testing  && nStationInfo != eTestSite_TestTimeOut && nRetState == eExcutingStep_ErrorJob)
	{
		int nChkMotSafety = g_TaskPressUnit[m_ePressIndex].ChkMotorPos(LOC_IDX_PRESS_UNIT_SAFETY, CTaskPressUnit::eAxisZ);
		int nChkSenSafety = g_TaskPressUnit[m_ePressIndex].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON);
		if (nChkMotSafety != ERR_NO_ERROR || nChkSenSafety != ERR_NO_ERROR)
			nRetState = eExcutingStep_Move_SafetyPos;
	}

	return nRetState;
}

void CTaskAutoPress::RecordDataOEE()
{
	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
	if( nRunMode != eRUN_MODE_ONLINE){
		return;
	}

	CString strData = _T("");
	char szCompany[256] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	int nSiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	ST_DD_DEVICE tDeviceData;

	if(strcmp(szCompany, DEF_COMPANY_AMKOR) == 0)
	{
		char szEqpName[128] = {0,};
		g_DMCont.m_dmHandlerOpt.GS(SDM11_UDP_HOST_NAME, szEqpName, sizeof(szEqpName));
		strData.Format("%s,",szEqpName);

		int nSocketStatus = 0;
		for(int i=0; i < eMaxPressUnitCount; i++)
		{
			for(int nY=0; nY < STATION_MAX_Y_SITE; nY++)
			{
				for(int nX=0; nX < STATION_MAX_X_SITE; nX++)
				{
					if( nX < nSiteCntX && nY < nSiteCntY)
					{
						tDeviceData.clear();
						int nOnOff = g_TaskPressUnit[m_ePressIndex].GetSocketOnOff(nX, nY);
						g_TaskPressUnit[m_ePressIndex].GetSiteDeviceData(&tDeviceData, nX, nY);

						if( nOnOff == DEF_SITE_ON )
						{
							if( tDeviceData.nBinColor == eFailBin )
							{
								strData += _T("F");
							}
							else if( tDeviceData.nBinColor == eGoodBin )
							{
								strData += _T("P");
							}
						}
					}
					// 문자열의 가장 마지막이 아닐 경우.
					if( i != eMaxPressUnitCount-1 && nX != STATION_MAX_X_SITE-1 && nY != STATION_MAX_Y_SITE-1 )
						strData += _T(",");
				}
			}
		}
		strData += _T("*");
//		g_Udp.OnSendUdp(strData);
	}
}

BOOL CTaskAutoPress::UseAbleCheck()
{
	//사용 유무에 따른 origin 확인
	int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	BOOL bAutoUseGRR = FALSE;
	if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
		bAutoUseGRR = TRUE;


	if (g_TaskAutoTestPp[m_eTestPpIndex].GetModuleSocketOnCnt(bAutoUseGRR) > 0 || g_TaskAutoTestPp[m_eTestPpIndex].GetModuleAuditSocketOnCnt() > 0) {
		//Module에 사용 Site가 있으면 Press확인
		if (g_TaskPressUnit[m_ePressIndex].IsInitNeed() == TRUE) {
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

void CTaskAutoPress::SetCurExecutingStep(int nStep)
{
	m_nAutoCurExecutingStep = nStep;
}
int CTaskAutoPress::GetCurExecutingStep()
{
	return m_nAutoCurExecutingStep;
}

