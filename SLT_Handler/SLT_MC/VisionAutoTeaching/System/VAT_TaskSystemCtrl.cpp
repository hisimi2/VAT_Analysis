#include "stdafx.h"
#include "VAT_TaskSystemCtrl.h"


CVAT_TaskSystemCtrl::CVAT_TaskSystemCtrl()
{
	::InitializeCriticalSection(m_cs);
	::InitializeCriticalSection(m_csTblOwner);
	::InitializeCriticalSection(m_csCTrayJigOwner);
 	::InitializeCriticalSection(m_csTestPpOwner);
	
	//Loading Table 소유권 초기화.
	for (int table_idx = 0; table_idx < VAT_TABLE_OWNER_MAX; table_idx++)
		m_reserved_table[table_idx] = VAT_TABLE_OWNER_NONE;

	//Feeder 소유권 초기화.
	m_reserved_tray_jig[VAT_HAND_LOAD_TRAYPP] = VAT_TABLE_OWNER_NONE;
	m_reserved_tray_jig[VAT_HAND_LOAD_TRAYPP] = VAT_TABLE_OWNER_NONE;

	//Test Pp LD/UD Hand 소유권
	m_reserved_test_pp[eTestPP_1] = -1;
	m_reserved_test_pp[eTestPP_2] = -1;

	m_hVatMainPage = NULL;
}


CVAT_TaskSystemCtrl::~CVAT_TaskSystemCtrl()
{
 	::DeleteCriticalSection(m_cs);
 	::DeleteCriticalSection(m_csTblOwner);
 	::DeleteCriticalSection(m_csCTrayJigOwner);
 	::DeleteCriticalSection(m_csTestPpOwner);
}

int CVAT_TaskSystemCtrl::Initialize(/*HWND hOwner*/)
{
	//VAT Auto Initial
	g_VatTaskAutoSystem.Initialize();
	g_VatTaskAutoTrayLoadPP.Initialize();
 	g_VatTaskAutoTrayUnloadPP.Initialize();
	g_VatTaskAutoLoadTestPP1.Initialize(VAT_HAND_LOAD_TESTPP1);
	g_VatTaskAutoUnloadTestPP1.Initialize(VAT_HAND_UNLOAD_TESTPP1);
	g_VatTaskAutoLoadTestPP2.Initialize(VAT_HAND_LOAD_TESTPP2);
	g_VatTaskAutoUnloadTestPP2.Initialize(VAT_HAND_UNLOAD_TESTPP2);


// 	g_VatTaskAutoTestPP[eTestPP_1].Initialize(VAT_HAND_TESTPP1);
// 	g_VatTaskAutoTestPP[eTestPP_2].Initialize(VAT_HAND_TESTPP2);

	//Loading Table 소유권 초기화.
	for (int table_idx = 0; table_idx < VAT_TABLE_OWNER_MAX; table_idx++)
		m_reserved_table[table_idx] = VAT_TABLE_OWNER_NONE;

	//All Auto Parameter 관리.
	m_vAllTaskAuto.clear();
	m_vAllTaskAuto.push_back(&g_VatTaskAutoTrayLoadPP);
	m_vAllTaskAuto.push_back(&g_VatTaskAutoTrayUnloadPP);
	m_vAllTaskAuto.push_back(&g_VatTaskAutoLoadTestPP1);
	m_vAllTaskAuto.push_back(&g_VatTaskAutoUnloadTestPP1);
	m_vAllTaskAuto.push_back(&g_VatTaskAutoLoadTestPP2);
	m_vAllTaskAuto.push_back(&g_VatTaskAutoUnloadTestPP2);


	return CBaseTaskManager::Initialize();
}

void CVAT_TaskSystemCtrl::Finalize()
{
	//VAT 관련 Thread 종료
	g_VatTaskAutoSystem.Finalize();
	g_VatTaskAutoTrayLoadPP.Finalize();
	g_VatTaskAutoTrayUnloadPP.Finalize();
	g_VatTaskAutoLoadTestPP1.Finalize();
	g_VatTaskAutoUnloadTestPP1.Finalize();
	g_VatTaskAutoLoadTestPP2.Finalize();
	g_VatTaskAutoUnloadTestPP2.Finalize();

	//g_VatTaskAutoTestPP[eTestPP_1].Finalize();
	//g_VatTaskAutoTestPP[eTestPP_2].Finalize();

	CBaseTaskManager::Finalize();
}


void CVAT_TaskSystemCtrl::ThreadProc1()
{
	SetThreadInterval(0, 50);
	while (GetThreadAliveFlag(0)) {
		if (GetPauseFlag(0)) {
			NEXT;
			continue;
		}

		CBaseTaskManager::SeqCmdRunning();
		
		NEXT_10;
	}
}


void CVAT_TaskSystemCtrl::OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	//Request Alarm 여기서 관리
	//VAT System Ctrol 관련 여기서 관리
	//시작 Alarm등.


// 	int nAlid = wParam;
// 	switch (nAlid)
// 	{
// 	case ERR_2DBARCODE_NO_CHECK_MODE_ON:
// 	{
// 		g_TaskSystemCtrl.DeActionTowerAndSound(eSIGNAL_REQUEST);
// 		OnRcv_Req_2D_Duplicate_Ack(nOperatorCommand, wParam, lParam);
// 	}break;

}


int CVAT_TaskSystemCtrl::SetLotCommand(eAutoLotCmd eLotCmd)
{
	switch (eLotCmd)
	{
	case eAutoLotCmd_Skip:
	case eAutoLotCmd_AlarmPause:
	case eAutoLotCmd_Pause:
	{

	}break;
	case eAutoLotCmd_Initial:
	{
		CBaseTaskManager::SetEqpCtrlMode(eEqpCtrlMode_Auto);
	}break;
	case eAutoLotCmd_Retry:
	case eAutoLotCmd_CleanOut:
	case eAutoLotCmd_Resume:
	{
		CBaseTaskManager::SetEqpCtrlMode(eEqpCtrlMode_Auto);
	}break;
	}

	//나머지 Auto Sequence 추가.
	BOOL bChkErr = FALSE;
	int nErr = g_VatTaskAutoSystem.OnLotCommand(eLotCmd, NULL, NULL);
	if (nErr != ERR_NO_ERROR) {
		bChkErr = TRUE;
	}

	nErr = g_VatTaskAutoTrayLoadPP.OnLotCommand(eLotCmd, NULL, NULL);
	if (nErr != ERR_NO_ERROR) {
		bChkErr = TRUE;
	}

	nErr = g_VatTaskAutoTrayUnloadPP.OnLotCommand(eLotCmd, NULL, NULL);
	if (nErr != ERR_NO_ERROR) {
		bChkErr = TRUE;
	}

	nErr = g_VatTaskAutoLoadTestPP1.OnLotCommand(eLotCmd, NULL, NULL);
	if (nErr != ERR_NO_ERROR) {
		bChkErr = TRUE;
	}

	nErr = g_VatTaskAutoUnloadTestPP1.OnLotCommand(eLotCmd, NULL, NULL);
	if (nErr != ERR_NO_ERROR) {
		bChkErr = TRUE;
	}

	nErr = g_VatTaskAutoLoadTestPP2.OnLotCommand(eLotCmd, NULL, NULL);
	if (nErr != ERR_NO_ERROR) {
		bChkErr = TRUE;
	}

	nErr = g_VatTaskAutoUnloadTestPP2.OnLotCommand(eLotCmd, NULL, NULL);
	if (nErr != ERR_NO_ERROR) {
		bChkErr = TRUE;
	}

// 	nErr = g_VatTaskAutoTestPP[eTestPP_1].OnLotCommand(eLotCmd, NULL, NULL);
// 	if (nErr != ERR_NO_ERROR) {
// 		bChkErr = TRUE;
// 	}
// 
// 	nErr = g_VatTaskAutoTestPP[eTestPP_2].OnLotCommand(eLotCmd, NULL, NULL);
// 	if (nErr != ERR_NO_ERROR) {
// 		bChkErr = TRUE;
// 	}

	if (bChkErr == TRUE) {
		g_START_SW.SetStatus(FALSE);
		g_STOP_SW.SetStatus(TRUE);
		return ERR_LOT_CMD_CANNOT_PERFORM_NOW;
	}

	return ERR_NO_ERROR;
}

void CVAT_TaskSystemCtrl::SetSeqCurMode(int nMode)
{
	ASSERT(VAT_SEQ_MODE_Z_FOCUS <= nMode && nMode <= VAT_SEQ_MODE_Z_TEACHING);

	g_DMCont.m_dmVAT.SN(NDM14_VAT_CURRENT_MODE, nMode);
}

int CVAT_TaskSystemCtrl::GetSeqCurMode()
{
	return g_DMCont.m_dmVAT.GN(NDM14_VAT_CURRENT_MODE);
}


int CVAT_TaskSystemCtrl::GetSeqCurModeType()
{
	m_cs.Lock();

	int nRetType = VAT_MODE_TYPE_NONE;

	int nCurMode = GetSeqCurMode();
	if (nCurMode == VAT_SEQ_MODE_Z_FOCUS || nCurMode == VAT_SEQ_MODE_CAM_CAL ) {
		nRetType = VAT_MODE_TYPE_HAND;
	}
	else if (nCurMode == VAT_SEQ_MODE_PICKER_ALIGN) {
		nRetType = VAT_MODE_TYPE_PICKER;
	}
	else if (nCurMode == VAT_SEQ_MODE_Z_TEACHING || nCurMode == VAT_SEQ_MODE_VAT) {
		nRetType = VAT_MODE_TYPE_PART;
	}

	m_cs.Unlock();

	return nRetType;
}



int CVAT_TaskSystemCtrl::CheckEqpCondition(CString &strErrAxis)
{
	int nErr = ERR_NO_ERROR;
	// cover switch
	if (!g_COVER_SW.GetStatus()) {
		return ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK;
	}

	// emo & power off & check door
	nErr = g_TaskSystemCtrl.ChkComInterlock();
	if (nErr != ERR_NO_ERROR){
		return nErr;
	}

	//origin 확인
	//Motor 어떤 축이 Origin 안되어 있는지 알람 필요.

	int nMotorCount = g_Motor.GetAxisCount();
	for (int i = 0; i < nMotorCount; i++)
	{
		BOOL IsOrigin = g_Motor[i]->IsOrgCompleted(nullptr);
		if (IsOrigin != TRUE) {
			strErrAxis = g_Motor[i]->GetAxisName();
			return ERR_VAT_NOT_ORIGIN_CHECK;
		}
	}

	return nErr;

	//Vision Connect 확인
}


void CVAT_TaskSystemCtrl::SetVatMainPageHandle(HWND hOwner)
{
	m_hVatMainPage = hOwner;
}


HWND CVAT_TaskSystemCtrl::GetVatMainPageHandle()
{
	return m_hVatMainPage;
}

/*=============================================================================
AUTHOR   : [4/17/2024 dohyeong.kim]
NAME     :
PARAMS   :
RETURN   : reserve 완료 일 때, true / reserve 미완료 일 때, false
COMMENTS : Table에 대한 Hand 소유권 부여
==============================================================================*/
bool CVAT_TaskSystemCtrl::reserve_tbl_work_hand(int table_idx, int nHand)
{
	m_csTblOwner.Lock();

	bool bRet = false;

	if (m_reserved_table[table_idx] == VAT_TABLE_OWNER_NONE || m_reserved_table[table_idx] == nHand)
	{
		m_reserved_table[table_idx] = nHand;
		bRet = true;
	}

	m_csTblOwner.Unlock();

	return bRet;	
}

bool CVAT_TaskSystemCtrl::release_tbl_work_hand(int table_idx, int nHand)
{
	m_csTblOwner.Lock();

	bool bRet = false;

	if (m_reserved_table[table_idx] == nHand)
	{
		m_reserved_table[table_idx] = VAT_TABLE_OWNER_NONE;
		bRet = true;
	}

	m_csTblOwner.Unlock();

	return bRet;
}

bool CVAT_TaskSystemCtrl::get_tbl_reserved_hand(int table_idx, int nHand)
{
	m_csTblOwner.Lock();

	bool bRet = false;

	if (m_reserved_table[table_idx] == nHand)
	{
		bRet = true;
	}

	m_csTblOwner.Unlock();

	return bRet;
}


/*=============================================================================
AUTHOR   : [6/12/2025 dohyeong.kim]
NAME     :
PARAMS   :
RETURN   : reserve 완료 일 때, true / reserve 미완료 일 때, false
COMMENTS : C-Tray Jig에 대한 Hand 소유권 부여
==============================================================================*/
bool CVAT_TaskSystemCtrl::reserve_tray_jig_work_hand(int table_idx, int nHand)
{
	ASSERT(nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP);

	m_csCTrayJigOwner.Lock();

	bool bRet = false;

	if (m_reserved_tray_jig[table_idx] == VAT_TABLE_OWNER_NONE || m_reserved_tray_jig[table_idx] == nHand)
	{
		m_reserved_tray_jig[table_idx] = nHand;
		bRet = true;
	}

	m_csCTrayJigOwner.Unlock();

	return bRet;
}


bool CVAT_TaskSystemCtrl::release_tray_jig_work_hand(int table_idx, int nHand)
{
	ASSERT(nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP);

	m_csCTrayJigOwner.Lock();

	bool bRet = false;

	if (m_reserved_tray_jig[table_idx] == nHand)
	{
		m_reserved_tray_jig[table_idx] = VAT_TABLE_OWNER_NONE;
		bRet = true;
	}

	m_csCTrayJigOwner.Unlock();

	return bRet;
}


bool CVAT_TaskSystemCtrl::get_tray_jig_reserved_hand(int table_idx, int nHand)
{
	ASSERT(nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP);

	m_csCTrayJigOwner.Lock();

	bool bRet = false;

	if (m_reserved_tray_jig[table_idx] == nHand)
	{
		bRet = true;
	}

	m_csCTrayJigOwner.Unlock();

	return bRet;
}


void CVAT_TaskSystemCtrl::reserve_test_pp_work_hand(int nTestPpIdx, int eType)
{
	m_csTestPpOwner.Lock();

	m_reserved_test_pp[nTestPpIdx] = eType;

	m_csTestPpOwner.Unlock();
}


int CVAT_TaskSystemCtrl::get_test_pp_work_hand(int nTestPpIdx)
{
	return m_reserved_test_pp[nTestPpIdx];
}


// void CVAT_TaskSystemCtrl::ReserveTestPpWorkIfSelected(int nTestPpIdx, CVatBaseTask& loadTask, CVatBaseTask& unloadTask)
// {
// 	BOOL bIsLoadSelected = FALSE;
// 	BOOL bIsUnloadSelected = FALSE;
// 
// 	int nModeType = g_VatTaskSystemCtrl.GetSeqCurModeType();
// 
// 	if (nModeType == VAT_MODE_TYPE_HAND) {
// 		bIsLoadSelected = loadTask._Base_IsHandSelected();
// 		bIsUnloadSelected = unloadTask._Base_IsHandSelected();
// 	}
// 	else {
// 		bIsLoadSelected = loadTask._Base_IsAnyPartSelected();
// 		bIsUnloadSelected = unloadTask._Base_IsAnyPartSelected();
// 	}
// 
// 
// 	if (bIsLoadSelected) {
// 		reserve_test_pp_work_hand(nTestPpIdx, VAT_TESTPP_LOAD);
// 	}
// 	else if (bIsUnloadSelected) {
// 		reserve_test_pp_work_hand(nTestPpIdx, VAT_TESTPP_UNLOAD);
// 	}
// }

//=============================================================================
// COMMENT : 1. Hand 및 Part Select 확인.
//           2. Measure 상태 확인.   
//           ※ 2가지 사항으로 Test PP Work Reserve 진행.
//=============================================================================
void CVAT_TaskSystemCtrl::ReserveTestPpWorkIfSelected(int nTestPpIdx)
{
	CVatBaseTask& loadTask = (nTestPpIdx == eTestPP_1) ? g_VatTaskAutoLoadTestPP1 : g_VatTaskAutoLoadTestPP2;
	CVatBaseTask& unloadTask = (nTestPpIdx == eTestPP_1) ? g_VatTaskAutoUnloadTestPP1 : g_VatTaskAutoUnloadTestPP2;

	auto getStatus = [](CVatBaseTask& task, BOOL& bIsSelected, BOOL& bIsMeasureComplete) {
		int nModeType = g_VatTaskSystemCtrl.GetSeqCurModeType();
		if (nModeType == VAT_MODE_TYPE_HAND) {
			bIsSelected = task._Base_IsHandSelected();
			bIsMeasureComplete = task._Base_IsHandSameStatus(VAT_STATUS_MEASURE_COMPLETE);
		}
		else if (nModeType == VAT_MODE_TYPE_PICKER) {
			bIsSelected = task._Base_IsAnyPickerSelected();
			BOOL bWideComplete = task._Base_IsAllPickerSameStatus(VAT_PITCH_TYPE_WIDE, VAT_STATUS_MEASURE_COMPLETE);
			BOOL bNarrowComplete = task._Base_IsAllPickerSameStatus(VAT_PITCH_TYPE_NARROW, VAT_STATUS_MEASURE_COMPLETE);
			bIsMeasureComplete = (bWideComplete && bNarrowComplete);			
		}
		else {
			bIsSelected = task._Base_IsAnyPartSelected();
			bIsMeasureComplete = task._Base_IsAllPartSameStatus(VAT_STATUS_MEASURE_COMPLETE);
		}
	};

	BOOL bIsLoadSelected = FALSE, bIsUnloadSelected = FALSE;
	BOOL bIsLoadMeasureComplete = FALSE, bIsUnloadMeasureComplete = FALSE;

	getStatus(loadTask, bIsLoadSelected, bIsLoadMeasureComplete);
	getStatus(unloadTask, bIsUnloadSelected, bIsUnloadMeasureComplete);

	if (bIsLoadSelected && !bIsLoadMeasureComplete) {
		reserve_test_pp_work_hand(nTestPpIdx, VAT_TESTPP_LOAD);
	}
	else if (bIsUnloadSelected && !bIsUnloadMeasureComplete) {
		reserve_test_pp_work_hand(nTestPpIdx, VAT_TESTPP_UNLOAD);
	}
}


void CVAT_TaskSystemCtrl::ClearMainGuiMeasureStatus()
{
	CVAT_MeasureStatus& GuiNotifier = CVAT_MeasureStatus::GetInstance();

	int nMode = GetSeqCurMode();

	switch (nMode)
	{
		case VAT_SEQ_MODE_Z_FOCUS:
		case VAT_SEQ_MODE_CAM_CAL:
		case VAT_SEQ_MODE_PICKER_ALIGN:
		{
			for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
				GuiNotifier.UpdateHandStatus(nHand, VAT_STATUS_MEASURE_INIT);
				GuiNotifier.UpdateProgressPercent(nHand, 0.0);
			}
			for (int nCam = 0; nCam < VAT_CAM_MAX; nCam++) {
				GuiNotifier.UpdateVisionStatus(nCam, VAT_RCV_STATUS_INIT);
				GuiNotifier.UpdateVisionMeasureCnt(nCam, 0);
				GuiNotifier.UpdateVisionOffset(nCam, 0.0, 0.0);
			}

			//Picker Align만 따로 처리.
			if (nMode == VAT_SEQ_MODE_PICKER_ALIGN)
			{
				for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
					for (int nPitchType = 0; nPitchType < VAT_PITCH_TYPE_MAX; nPitchType++) {
						for (int nPickerIdx = 0; nPickerIdx < VAT_PICKER_MAX; nPickerIdx++) {
							GuiNotifier.UpdatePickerStatus(nHand, nPitchType, nPickerIdx, VAT_STATUS_MEASURE_INIT);
						}
					}
				}			
			}

		}break;
		default:
			break;
	}

// 	if (nMode == VAT_SEQ_MODE_Z_FOCUS) {
// 	
// 		for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
// 			GuiNotifier.UpdateProgressPercent(nHand, 0.0);
// 		}
// 		for (int nCam = 0; nCam < VAT_CAM_MAX; nCam++) {
// 			GuiNotifier.UpdateVisionStatus(nCam, VAT_RCV_STATUS_INIT);
// 			GuiNotifier.UpdateVisionMeasureCnt(nCam, 0);
// 		}
// 	}
// 	else if (nMode == VAT_SEQ_MODE_CAM_CAL) {
// 
// 	}
}


//==============================================================================================
// COMMENT : Z Auto Focus 측정 시, Lower Cam 2번 측정 방지를 위해 LD Test PP 측정 완료 확인 함수.
//==============================================================================================
BOOL CVAT_TaskSystemCtrl::IsLoadTestPpAlreadyDone(int nTestPpIdx)
{
	CVatBaseTask& loadTask = (nTestPpIdx == eTestPP_1) ? g_VatTaskAutoLoadTestPP1 : g_VatTaskAutoLoadTestPP2;
	BOOL bIsSelected = loadTask._Base_IsHandSelected();
	BOOL bIsMeasureComplete = loadTask._Base_IsHandSameStatus(VAT_STATUS_MEASURE_COMPLETE);
	
	return (bIsSelected && bIsMeasureComplete) ? TRUE : FALSE;
}


//========================================================================================
//COMMENT : 전체 Auto 선택 Hand 초기화.
//========================================================================================
void CVAT_TaskSystemCtrl::ClearAllMeasureHand()
{
	for (auto EachTaskAuto : m_vAllTaskAuto) {
		EachTaskAuto->_Base_ClearMeasureHand();
	}
}


void CVAT_TaskSystemCtrl::SetMeasureHandToEachAuto(int nHand, BOOL bIsSelected)
{
	switch (nHand)
	{
	case VAT_HAND_LOAD_TRAYPP:			g_VatTaskAutoTrayLoadPP._Base_SetMeasureHand(bIsSelected);	break;
	case VAT_HAND_UNLOAD_TRAYPP:		g_VatTaskAutoTrayUnloadPP._Base_SetMeasureHand(bIsSelected);	 break;
	case VAT_HAND_LOAD_TESTPP1:			g_VatTaskAutoLoadTestPP1._Base_SetMeasureHand(bIsSelected);	 break;
	case VAT_HAND_UNLOAD_TESTPP1:		g_VatTaskAutoUnloadTestPP1._Base_SetMeasureHand(bIsSelected);	 break;
	case VAT_HAND_LOAD_TESTPP2:			g_VatTaskAutoLoadTestPP2._Base_SetMeasureHand(bIsSelected);	 break;
	case VAT_HAND_UNLOAD_TESTPP2:		g_VatTaskAutoUnloadTestPP2._Base_SetMeasureHand(bIsSelected);	 break;
	default:
		break;
	}
}

//========================================================================================
//COMMENT : 전체 Auto 선택 Picker 초기화.
//========================================================================================
void CVAT_TaskSystemCtrl::ClearAllMeasurePicker()
{
	for (auto EachTaskAuto : m_vAllTaskAuto) {
		EachTaskAuto->_Base_ClearMeasurePicker();
	}
}


void CVAT_TaskSystemCtrl::SetMeasurePickerToEachAuto(int nHand, int nPicker, BOOL bIsSelected)
{
	switch (nHand)
	{
	case VAT_HAND_LOAD_TRAYPP:			g_VatTaskAutoTrayLoadPP._Base_SetMeasurePicker_Selected(nPicker, bIsSelected);		break;
	case VAT_HAND_UNLOAD_TRAYPP:		g_VatTaskAutoTrayUnloadPP._Base_SetMeasurePicker_Selected(nPicker, bIsSelected);	break;
	case VAT_HAND_LOAD_TESTPP1:			g_VatTaskAutoLoadTestPP1._Base_SetMeasurePicker_Selected(nPicker, bIsSelected);		break;
	case VAT_HAND_UNLOAD_TESTPP1:		g_VatTaskAutoUnloadTestPP1._Base_SetMeasurePicker_Selected(nPicker, bIsSelected);	break;
	case VAT_HAND_LOAD_TESTPP2:			g_VatTaskAutoLoadTestPP2._Base_SetMeasurePicker_Selected(nPicker, bIsSelected);		break;
	case VAT_HAND_UNLOAD_TESTPP2:		g_VatTaskAutoUnloadTestPP2._Base_SetMeasurePicker_Selected(nPicker, bIsSelected);	break;
	default:
		break;
	}
}


//========================================================================================
//COMMENT : 전체 Auto 선택 Part 초기화.
//========================================================================================
void CVAT_TaskSystemCtrl::ClearAllMeasurePart()
{
	for (auto EachTaskAuto : m_vAllTaskAuto) {
		EachTaskAuto->_Base_ClearMeasurePart();
	}
}


//========================================================================================
//COMMENT : GUI에서 선택 된 Part를 해당 Auto로 전달.
//========================================================================================
void CVAT_TaskSystemCtrl::SetMeasurePartToEachAuto(int nHand, int nPart, BOOL bIsSelected)
{
	switch (nHand)
	{
	case VAT_HAND_LOAD_TRAYPP:			g_VatTaskAutoTrayLoadPP._Base_SetMeasurePart_Selected(nPart, bIsSelected);		break;
	case VAT_HAND_UNLOAD_TRAYPP:		g_VatTaskAutoTrayUnloadPP._Base_SetMeasurePart_Selected(nPart, bIsSelected);	 break;
	case VAT_HAND_LOAD_TESTPP1:			g_VatTaskAutoLoadTestPP1._Base_SetMeasurePart_Selected(nPart, bIsSelected);	 break;
	case VAT_HAND_UNLOAD_TESTPP1:		g_VatTaskAutoUnloadTestPP1._Base_SetMeasurePart_Selected(nPart, bIsSelected);	 break;
	case VAT_HAND_LOAD_TESTPP2:			g_VatTaskAutoLoadTestPP2._Base_SetMeasurePart_Selected(nPart, bIsSelected);	 break;
	case VAT_HAND_UNLOAD_TESTPP2:		g_VatTaskAutoUnloadTestPP2._Base_SetMeasurePart_Selected(nPart, bIsSelected);	 break;
	default:
		break;
	}
}
