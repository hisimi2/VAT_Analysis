#include "stdafx.h"
#include "VAT_TaskAutoSystem.h"


CVAT_TaskAutoSystem::CVAT_TaskAutoSystem()
{
	//m_hVatMainGui = NULL;
	m_nCurModeType = VAT_MODE_TYPE_NONE;
	m_nDataSaveCase = eVat_Normal;
}


CVAT_TaskAutoSystem::~CVAT_TaskAutoSystem()
{
}

int CVAT_TaskAutoSystem::Initialize(/*HWND hOwner*/)
{
	//GUI handle hOwner 필요할때 사용.

	m_nAutoRunMainStep = eAutoLotState_InitNeed;
	m_nAutoRunSubStep = 0;
	m_nAutoRunBackUpMainStep = eAutoLotState_InitNeed;
	m_nAutoRunBackUpSubStep = 0;

	//All Auto 상태 관리.
	m_vVatAllAutoTasks.clear();
	m_vVatAllAutoTasks.push_back(&g_VatTaskAutoTrayLoadPP);
	m_vVatAllAutoTasks.push_back(&g_VatTaskAutoTrayUnloadPP);
	m_vVatAllAutoTasks.push_back(&g_VatTaskAutoLoadTestPP1);
	m_vVatAllAutoTasks.push_back(&g_VatTaskAutoUnloadTestPP1);
	m_vVatAllAutoTasks.push_back(&g_VatTaskAutoLoadTestPP2);
	m_vVatAllAutoTasks.push_back(&g_VatTaskAutoUnloadTestPP2);


	m_nDataSaveCase = eVat_Normal; 
	//m_hVatMainGui = hOwner;

	return CBaseTaskManager::Initialize();
}

void CVAT_TaskAutoSystem::Finalize()
{
	CBaseTaskManager::Finalize();
}

int CVAT_TaskAutoSystem::OnEvent(const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList/*=NULL*/)
{
	return ERR_NO_ERROR;
}

int CVAT_TaskAutoSystem::OnCommandRsp(CBaseSeqCmd * pRspCmd, int nEventId, WPARAM wParam, LPARAM lParam, std::vector<CPoint>& vPocket_Picker)
{
	if (nEventId == CMD_EVT_ALARM)
	{
		if (pRspCmd->m_pSender == this)
			ChangeMainStateByRspAlarm();
	}

	return CBaseTaskManager::OnCommandRsp(pRspCmd, nEventId, wParam, lParam, vPocket_Picker);
}

void CVAT_TaskAutoSystem::OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	
 	//Alarm OK/Cancel
 	int nAlid = wParam;
 	if (nOperatorCommand == TWMB_ID_OK)
 	{	
 		switch (nAlid)
 		{
 			case ERR_VAT_DATA_SAVE:
 			{
				g_VATCommonData.MeasureDataSave();
 			}break;
 		}
 	}
	else {
		//저장 하지 않으면, 상태 Ready로 변경.
		for (auto _auto_task : g_VatTaskSystemCtrl.m_vAllTaskAuto)
		{
			_auto_task->_Base_SetMeasure_Status(VAT_STATUS_MEASURE_READY);
		}

// 		for (auto _auto_task : g_VatTaskSystemCtrl.m_vAllTaskAuto)
// 		{
// 			int nCurModeType = g_VatTaskSystemCtrl.GetSeqCurModeType();
// 			if (nCurModeType == VAT_MODE_TYPE_HAND)
// 			{
// 				_auto_task->_Base_SetMeasureHand_Status(VAT_STATUS_MEASURE_READY);
// 			}
// 			else
// 			{
// 				int nHand = _auto_task->m_nHand;
// 				int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
// 				for (int nPart = 0; nPart < nMaxPart; nPart++) {
// 					_auto_task->_Base_SetMeasurePart_Status(nPart, VAT_STATUS_MEASURE_READY);
// 				}
// 			}
// 		}
	}
 
 	//g_VATCommonData.SetDataSaveComplete(TRUE);
}

void CVAT_TaskAutoSystem::ThreadProc1()
{
	SetThreadInterval(0, 50);
	while (GetThreadAliveFlag(0)) {
		if (GetPauseFlag(0)) {
			NEXT;
			continue;
		}
		AutoRunning();
		NEXT;
	}
}

void CVAT_TaskAutoSystem::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = { 0, };
	if (fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf_s(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_VAT_Auto_System), Debug, "", __LINE__, NULL, tmpMsg);
}

void CVAT_TaskAutoSystem::AutoState_InitNeed()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 1000:
	{
		//측정 전, 초기화
		//g_VATCommonData.SetDataSaveComplete(FALSE);

		m_nDataSaveCase = eVat_Normal;
		g_DMCont.m_dmVAT.SB(BDM14_VAT_TEACHING_END, false);

		// 불필요 [9/9/2025 dohyeong.kim]
		//g_VatTaskSystemCtrl.SetLotCommand(eAutoLotCmd_AlarmPause);
		ChangeSubState(1001);
	}break;
	case 1001:
	{

	}break;
	default:
		ChangeSubState(1000);
		break;
	}
}

void CVAT_TaskAutoSystem::AutoState_Initialize()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 2000: //초기 확인 필요한 Interlock
		{
			ChangeSubState(2001);
		}break;
	case 2001:
		{
			//g_VatBaseTask.SetCurLotState(eAutoLotState_Initialize);

			auto not_idle_auto_task = std::find_if(m_vVatAllAutoTasks.begin(), m_vVatAllAutoTasks.end(),
				[](CBaseAutoSeqManager* _auto_task) -> bool
			{
				eAutoLotState test = _auto_task->GetCurrentLotState();
				if (_auto_task->GetCurrentLotState() != eAutoLotState_Idle)
					return true;

				return false;
			});

			if (not_idle_auto_task != m_vVatAllAutoTasks.end())
				break;

			ChangeSubState(2002);
		}break;
	case 2002:
		{
		//g_VatTaskSystemCtrl.ClearMainGuiMeasureStatus();

		//Test PP1 LD/UD reserve
		g_VatTaskSystemCtrl.ReserveTestPpWorkIfSelected(eTestPP_1/*, g_VatTaskAutoLoadTestPP1, g_VatTaskAutoUnloadTestPP1*/);

		//Test PP2 LD/UD reserve
		g_VatTaskSystemCtrl.ReserveTestPpWorkIfSelected(eTestPP_2/*, g_VatTaskAutoLoadTestPP2, g_VatTaskAutoUnloadTestPP2*/);

			ChangeSubState(2100);
		}break;

	case 2100:
		{
			ChangeMainState(eAutoLotState_Idle);
		}break;
	default:
		ChangeSubState(2000);
		break;
	}
}

void CVAT_TaskAutoSystem::AutoState_Idle()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 3000:
		{
			//g_VatBaseTask.SetCurLotState(eAutoLotState_Idle);

			auto not_idle_auto_task = std::find_if(m_vVatAllAutoTasks.begin(), m_vVatAllAutoTasks.end(),
				[](CBaseAutoSeqManager* _auto_task) -> bool
			{
				if (_auto_task->GetCurrentLotState() == eAutoLotState_InitNeed)
					return true;

				return false;
			});

			if (not_idle_auto_task != m_vVatAllAutoTasks.end())
				break;
			
			ChangeSubState(3001);
		}break;
	case 3001:
		{
			ChangeMainState(eAutoLotState_Executing);
		}break;
	default:
		ChangeSubState(3000);
		break;
	}
}


void CVAT_TaskAutoSystem::AutoState_Execute()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%s]"), GetLotStateToString(m_nAutoRunMainStep), GetExcuteStateStr());
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	if (m_nAutoRunSubStep > eExcutingStep_Vat_End) {
		m_nAutoRunSubStep = m_nLast_Excuting_Step;
	}
	m_nLast_Excuting_Step = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
		case eExcutingStep_Vat_Start:
		{
			g_VatTaskSystemCtrl.SetLotCommand(eAutoLotCmd_Resume);
			ChangeSubState(eExcutingStep_Vat_Measure_Wait);
		}break;
		case eExcutingStep_Vat_Measure_Wait:
		{
			// Auto Excuting 상태 확인.
// 			auto not_executing_auto_task = std::find_if(m_vVatAllAutoTasks.begin(), m_vVatAllAutoTasks.end(),
// 				[](CBaseAutoSeqManager* _auto_task) -> bool
// 			{
// 				if (_auto_task->GetCurrentLotState() != eAutoLotState_Executing ) //excuting
// 					return true;
// 
// 				return false;
// 			});
// 			
// 			if (not_executing_auto_task != m_vVatAllAutoTasks.end())
// 				break;
			
			//전체 Auto 측정 완료 확인.(Teaching 진행 중 확인.)
 			auto not_executing_auto_task = std::find_if(m_vVatAllAutoTasks.begin(), m_vVatAllAutoTasks.end(),
 				[](CBaseAutoSeqManager* _auto_task) -> bool
 			{
 				if (_auto_task->GetCurrentLotState() != eAutoLotState_NormalComplete) //Normal Complete
 					return true;
 
 				return false;
 			});
 			
 			if (not_executing_auto_task != m_vVatAllAutoTasks.end())
 				break;


			ChangeSubState(eExcutingStep_Vat_Data_Save);
		}break;
		case eExcutingStep_Vat_Data_Save:
		{
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_VAT_DATA_SAVE, AlarmData, &g_VatTaskAutoSystem);
			ChangeSubState(eExcutingStep_Vat_Data_Save_Wait);
			break;

// 			int nResult = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_COK, ERR_VAT_TYPE_YESNO);
// 			//int nRes =	  g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_COK, ERR_VAT_TYPE_YESNO);
// 			if (nResult == IDYES) {
// 				g_VATCommonData.MeasureDataSave();
// 
// 				ChangeSubState(eExcutingStep_Vat_Data_Save_Wait);
// 			}
// 			else {
// 				ChangeMainState(eAutoLotState_CleanOut);
// 			}
			
			

// 			//Teaching 진행 중인지 확인 필요.
// 			if (g_VatTaskAutoTrayLoadPP.GetFinishFlag() != TRUE || g_VatTaskAutoTrayUnloadPP.GetFinishFlag() != TRUE ||
// 				g_VatTaskAutoTestPP[eTestPP_1].GetFinishFlag() != TRUE || g_VatTaskAutoTestPP[eTestPP_2].GetFinishFlag() != TRUE) {//normal complete 상태 확인으로 변경 해도 괜찮음.
// 				break;
// 			}
// 			else {
// 				//OnOperatorCommand에서 Data Save
// 				CParamList AlarmData;
// 				m_nDataSaveCase = eVat_Normal;
// 				g_Error.AlarmReport(ERR_VAT_DATA_SAVE, AlarmData, &g_VatTaskAutoSystem/*&g_TaskSystemCtrl*/);
// 			}
		}break;

		case eExcutingStep_Vat_Data_Save_Wait:
		{
			//모든 Auto Hand Data Save 진행 하지 않음. -> READY 상태로 변경 됨.
			auto vat_status_data_ready = std::find_if(g_VatTaskSystemCtrl.m_vAllTaskAuto.begin(), g_VatTaskSystemCtrl.m_vAllTaskAuto.end(),
				[](CVatBaseTask* _auto_task) -> bool
			{
				BOOL bIsStatusReady = _auto_task->_Base_IsAllMeasureSameStatus(VAT_STATUS_MEASURE_READY);

				if (bIsStatusReady != TRUE)
					return true;

				return false;
			});

			if (vat_status_data_ready == g_VatTaskSystemCtrl.m_vAllTaskAuto.end()) {//모든 Part Ready 상태
				ChangeMainState(eAutoLotState_CleanOut);
				break;
			}
				


			//모든 Auto Hand Data Save 진행 완료.
			auto vat_status_data_save_complete = std::find_if(g_VatTaskSystemCtrl.m_vAllTaskAuto.begin(), g_VatTaskSystemCtrl.m_vAllTaskAuto.end(),
				[](CVatBaseTask* _auto_task) -> bool
			{
				BOOL bIsStatusDataSaveComplete = _auto_task->_Base_IsAllMeasureSameStatus(VAT_STATUS_DATA_SAVE_COMPLETE);;

// 				if (g_VatTaskSystemCtrl.GetSeqCurModeType() == VAT_MODE_TYPE_HAND) {
// 					bIsStatusDataSaveComplete = _auto_task->_Base_IsHandSameStatus(VAT_STATUS_DATA_SAVE_COMPLETE);
// 				}
// 				else {
// 					bIsStatusDataSaveComplete = _auto_task->_Base_IsAllPartSameStatus(VAT_STATUS_DATA_SAVE_COMPLETE);
// 				}
				
				//Data Save Complete 아니면 return
				if (bIsStatusDataSaveComplete != TRUE)
					return true;

				return false;
			});

			if (vat_status_data_save_complete != g_VatTaskSystemCtrl.m_vAllTaskAuto.end())
				break;

			//Data Save 완료.
			ChangeMainState(eAutoLotState_CleanOut);
		}break;
		default:
			ChangeSubState(eExcutingStep_Vat_Start);
			break;
	}
}

void CVAT_TaskAutoSystem::AutoState_Pause()
{
	//4000;
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 4000:
	{
		g_VatTaskSystemCtrl.SetLotCommand(eAutoLotCmd_Pause);
		//g_TaskSystemCtrl.SetCurrentLotState(eAutoLotState_AlarmStop);
		ChangeSubState(4001);
	}break;
	case 4001:
	{
		auto error_auto_task = std::find_if(m_vVatAllAutoTasks.begin(), m_vVatAllAutoTasks.end(),
			[](CBaseAutoSeqManager* _auto_task) -> bool
		{
			if (_auto_task->GetCurrentLotState() == eAutoLotState_InitNeed)
				return true;

			return false;
		});

		if (error_auto_task != m_vVatAllAutoTasks.end()) {
			ChangeMainState(eAutoLotState_InitNeed);
			return;
		}
	}break;
	default:
		ChangeSubState(4000);
		break;
	}
}

void CVAT_TaskAutoSystem::AutoState_NormalComplete()
{
	//5000
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 5000:
	{
		//Auto 상태 확인.
		auto not_normal_comp_auto_task = std::find_if(m_vVatAllAutoTasks.begin(), m_vVatAllAutoTasks.end(),
			[](CBaseAutoSeqManager* _auto_task) -> bool
		{
			if (_auto_task->GetCurrentLotState() != eAutoLotState_NormalComplete)
				return true;

			return false;
		});

		if (not_normal_comp_auto_task != m_vVatAllAutoTasks.end())
			break;

		//Cmd 상태 확인.
		int TaskCount = GetObserverCount();
		for (int i = 0; i < TaskCount; i++) {
			CBaseTaskManager* pTask = GetTaskByIndex(i);
			if (pTask->GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
				return;
		}

		ChangeSubState(5001);
	}break;
	case 5001:
	{		
		//Table reserve 해제
		//이거 각 Auto 에서 진행
		for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++) {
			for (int nTable = VAT_TABLE_1_OWNER; nTable < VAT_TABLE_OWNER_MAX; nTable++) {
				g_VatTaskSystemCtrl.release_tbl_work_hand(nTable, nHand);
			}
		}

		std::for_each(m_vVatAllAutoTasks.begin(), m_vVatAllAutoTasks.end()
			, [](CBaseAutoSeqManager* _auto_seq)
		{
			_auto_seq->ChangeMainState(eAutoLotState_InitNeed);
		});

		ChangeSubState(5002);
		//ChangeMainState(eAutoLotState_InitNeed);
	}break;
	case 5002:
	{
		HWND hOwner = g_VatTaskSystemCtrl.GetVatMainPageHandle();
		if (hOwner != NULL) {
			if (::IsWindow(hOwner))
				::SendMessage(hOwner, UM_VAT_SEQ_MSG_MEASURE_COMPLETE, (WPARAM)NULL, (LPARAM)NULL);
		}
		
		ChangeSubState(5003);	
	}break;
	case 5003:
	{
		ChangeMainState(eAutoLotState_InitNeed);
	}break;
	default:
		ChangeSubState(5000);
		break;
	}
}

void CVAT_TaskAutoSystem::AutoState_AlarmStop()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 6000:
	{
		ChangeSubState(6001);
	}break;
	case 6001:
	{

	}break;
	default:
		ChangeSubState(6000);
		break;
	}
}

void CVAT_TaskAutoSystem::AutoState_Resume()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 7000:
	{
		//g_TaskSystemCtrl.SetCurrentLotState(eAutoLotState_Resume);
		ChangeSubState(7001);
	}break;
	case 7001:
	{

		ChangeSubState(7003);
	}break;
	case 7003:
	{
// 		int nEndFeed = g_TaskAllAutoStacker.GetTrayEndFeed();
// 		switch (nEndFeed)
// 		{
// 		case DEF_LOTEND_TRAYEND:
// 		case DEF_LOTEND_TRAYFEED:
// 		{
// 			ChangeMainState(eAutoLotState_CleanOut);
// 		}break;
// 		default: ChangeMainState(eAutoLotState_Executing); break;
// 		}
		bool bEnd = g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_END);
		if (bEnd) {	
			ChangeMainState(eAutoLotState_CleanOut);
		}
		else {
			ChangeMainState(eAutoLotState_Executing);
		}		
	}break;
	default:
		ChangeSubState(7000);
		break;
	}

}

void CVAT_TaskAutoSystem::AutoState_CleanOut()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 8000:
	{
		g_DMCont.m_dmVAT.SB(BDM14_VAT_TEACHING_END, true);

		//비정상 종료 일 때, All Auto Clean Out으로 변경.
		if (m_nDataSaveCase == eVat_Abort) {
			//g_VatTaskSystemCtrl.SetLotCommand(eAutoLotCmd_CleanOut);

			for (auto _auto_task : m_vVatAllAutoTasks) {
				if (_auto_task->GetCurrentLotState() != eAutoLotState_NormalComplete) {
					_auto_task->ChangeMainState(eAutoLotState_CleanOut);
				}
			}
			//g_VatTaskAutoSystem.ChangeMainState(eAutoLotState_CleanOut);
		}

		ChangeSubState(8001);
	}break;
	case 8001:
	{
		//비정상 종료, 정상 종료 모두 Status NormalComplete 확인.
		auto not_executing_auto_task = std::find_if(m_vVatAllAutoTasks.begin(), m_vVatAllAutoTasks.end(),
			[](CBaseAutoSeqManager* _auto_task) -> bool
		{
			if (_auto_task->GetCurrentLotState() != eAutoLotState_NormalComplete) //Normal Complete	
				return true;
			
			return false;
		});

		if (not_executing_auto_task != m_vVatAllAutoTasks.end())
			break;

		ChangeSubState(8002);
	}break;
	case 8002:
	{
//  		if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
//  			Sleep(2000.0);

		//시퀀스 비정상 종료 될 때, Data 저장
		if (m_nDataSaveCase == eVat_Abort) {
			auto vat_status_measure_complete = std::find_if(g_VatTaskSystemCtrl.m_vAllTaskAuto.begin(), g_VatTaskSystemCtrl.m_vAllTaskAuto.end(),
				[](CVatBaseTask* _auto_task) -> bool
			{
				BOOL btest = _auto_task->_Base_IsAnyPartSelected();
				BOOL bstatus = _auto_task->_Base_GetMeasurePart_Selected(VAT_STATUS_MEASURE_COMPLETE);

				if (_auto_task->_Base_GetMeasurePart_Selected(VAT_STATUS_MEASURE_COMPLETE) != -1)
					return true;

				return false;
			});

			//Part중 측정 완료 된 놈 있으면 저장.
			if (vat_status_measure_complete != g_VatTaskSystemCtrl.m_vAllTaskAuto.end()) {
				CParamList AlarmData;
				g_Error.AlarmReport(ERR_VAT_DATA_SAVE, AlarmData, &g_VatTaskAutoSystem);
				ChangeSubState(8003);
				break;
			}
		}

		ChangeSubState(8010);
	}break;
	case 8003:
	{
		//여기 추가해야함.
		//Abort Case

		//모든 Auto Hand Data Save 진행 하지 않음. -> READY 상태로 변경 됨.
		auto vat_status_data_ready = std::find_if(g_VatTaskSystemCtrl.m_vAllTaskAuto.begin(), g_VatTaskSystemCtrl.m_vAllTaskAuto.end(),
			[](CVatBaseTask* _auto_task) -> bool
		{
			//Data Save안하면, 선택된 Part는 Ready상태만 존재
			if (_auto_task->_Base_IsAllPartSameStatus(VAT_STATUS_MEASURE_READY) != TRUE)
				return true;

			return false;
		});

		if (vat_status_data_ready == g_VatTaskSystemCtrl.m_vAllTaskAuto.end()) {//모든 Part Ready 상태
			ChangeSubState(8010);
			break;
		}


		//Data Save Wait
		auto vat_status_data_save_complete = std::find_if(g_VatTaskSystemCtrl.m_vAllTaskAuto.begin(), g_VatTaskSystemCtrl.m_vAllTaskAuto.end(),
			[](CVatBaseTask* _auto_task) -> bool
		{
			//Data Save진행 시, 선택된 Part는 2가지 상태만 존재
			//ERR_VAT_ERROR는 part 선택 안된 것.
			int nHand = _auto_task->m_nHand;
			int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
			for (int nPart = 0; nPart < nMaxPart; nPart++) {
				int nStatus = _auto_task->_Base_GetMeasurePart_Status(nPart);				
				if (nStatus != VAT_STATUS_DATA_SAVE_COMPLETE && nStatus != VAT_STATUS_MEASURE_READY && nStatus != ERR_VAT_ERROR) {
					return true;
				}
			}
			

			return false;
		});

		if (vat_status_data_save_complete != g_VatTaskSystemCtrl.m_vAllTaskAuto.end())
			break;

		ChangeSubState(8010);
	}break;
	case 8010:
	{
//		g_DMCont.m_dmVAT.SB(BDM14_VAT_TEACHING_END, true); //Normal Complete로 이동.

//   		auto chk_alarm_stop = std::find_if(m_vVatAllAutoTasks.begin(), m_vVatAllAutoTasks.end(),
//   			[](CBaseAutoSeqManager* _auto_task) -> bool
//   		{
//   			if (_auto_task->GetCurrentLotState() == eAutoLotState_AlarmStop)
//   				return true;
//   
//   			return false;
//   		});
//   
//   		if (chk_alarm_stop != m_vVatAllAutoTasks.end()) {
//   			g_VatTaskSystemCtrl.SetLotCommand(eAutoLotCmd_Pause);
//   			break;
//   		}

// 		auto not_clean_auto_task = std::find_if(m_vVatAllAutoTasks.begin(), m_vVatAllAutoTasks.end(),
// 			[](CBaseAutoSeqManager* _auto_task) -> bool
// 		{
// 			if (_auto_task->GetCurrentLotState() != eAutoLotState_CleanOut
// 				&& _auto_task->GetCurrentLotState() != eAutoLotState_Executing
// 				&& _auto_task->GetCurrentLotState() != eAutoLotState_NormalComplete)
// 				return true;
// 
// 			return false;
// 		});
// 		if (not_clean_auto_task != m_vVatAllAutoTasks.end())
// 			break;

		//g_VatTaskSystemCtrl.SetLotCommand(eAutoLotCmd_CleanOut);
		if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
			Sleep(2000.0);

		ChangeSubState(8011);
	}break;
	case 8011:
	{
		ChangeMainState(eAutoLotState_NormalComplete);
	}break;
	default:
		ChangeSubState(8000);
		break;
	}
}