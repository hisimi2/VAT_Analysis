#include "stdafx.h"
#include "VAT_TaskAutoTestPP.h"


CVAT_TaskAutoTestPP::CVAT_TaskAutoTestPP()
{
	m_nTestPpIdx = 0;
	m_nTestPpType = VAT_TESTPP_LOAD;

}


CVAT_TaskAutoTestPP::~CVAT_TaskAutoTestPP()
{
}

int CVAT_TaskAutoTestPP::Initialize(int nHand)
{
	m_nAutoRunMainStep = eAutoLotState_InitNeed;
	m_nAutoRunSubStep = 0;
	m_nAutoRunBackUpMainStep = eAutoLotState_InitNeed;
	m_nAutoRunBackUpSubStep = 0;

	//SetVatMainGuiHandle(hOwner); 모드 측정 시작 시, 전달 예정.

	//VAT Hand Idx
	m_nHand = nHand;

	//Test PP#1, #2 Idx
	m_nTestPpIdx = (m_nHand == VAT_HAND_LOAD_TESTPP1 || m_nHand == VAT_HAND_UNLOAD_TESTPP1) ? eTestPP_1 : eTestPP_2;

	//Test PP Type
	m_nTestPpType = (m_nHand == VAT_HAND_LOAD_TESTPP1 || m_nHand == VAT_HAND_LOAD_TESTPP2) ? VAT_TESTPP_LOAD : VAT_TESTPP_UNLOAD;

	return CBaseTaskManager::Initialize();
}

void CVAT_TaskAutoTestPP::Finalize()
{
	CBaseTaskManager::Finalize();
}

int CVAT_TaskAutoTestPP::OnEvent(const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList/*=NULL*/)
{
	return ERR_NO_ERROR;
}

int CVAT_TaskAutoTestPP::OnCommandRsp(CBaseSeqCmd* pRspCmd, int nEventId, WPARAM wParam, LPARAM lParam, std::vector<CPoint>& vPocket_Picker)
{
	if (nEventId == CMD_EVT_ALARM)
	{
		if (pRspCmd->m_pSender == this)
			ChangeMainStateByRspAlarm();
	}

	return CBaseTaskManager::OnCommandRsp(pRspCmd, nEventId, wParam, lParam, vPocket_Picker);
}

void CVAT_TaskAutoTestPP::OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
}

void CVAT_TaskAutoTestPP::ThreadProc1()
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

void CVAT_TaskAutoTestPP::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = { 0, };
	if (fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf_s(tmpMsg, fmt, argList);
		va_end(argList);
	}

	//eLogID LogId = (m_nTestPpIdx == eTestPP_1) ? eLogID_VAT_Auto_LD_Test_1 : eLogID_VAT_Auto_UD_Test_2;
	eLogID LogId = (eLogID)(m_nHand - VAT_HAND_LOAD_TESTPP1 + eLogID_VAT_Auto_LD_Test_1);

	TWLOG.MakeLog(static_cast<int>(LogId), Debug, "", __LINE__, NULL, tmpMsg);
}

void CVAT_TaskAutoTestPP::AutoState_InitNeed()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 1000:
	{		
		SetProcessCmd(base_seq_cmd::eProcCmd_Stop);
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

//원점 확인
//Safety 위치에서 시작
void CVAT_TaskAutoTestPP::AutoState_Initialize()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 2000:
	{
		//초기화 변수 정리 후, 추후 함수화(Init)
		m_nCurLoc = -1;
		m_nSeqMode = g_VatTaskSystemCtrl.GetSeqCurMode();
		m_nVisionRetryCnt = 0; //Vision Retry Count 초기화.
		m_dPrevPosX = 0.0;
		m_dPrevPosY = 0.0;
		m_stCurPicker.clear();

		ChangeSubState(2001);
	}break;

	case 2001:
	{
		CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
		if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		ChangeSubState(2999);
	}break;

	case 2999:
	{
		ChangeMainState(eAutoLotState_Idle);
	}break;
	default:
		ChangeSubState(2000);
		break;
	}
}


void CVAT_TaskAutoTestPP::AutoState_Idle()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 3000:
	{
		m_nLast_Excuting_Step = 0;
		ChangeSubState(3001);
	}break;
	case 3001:
	{

	}break;
	default:
		ChangeSubState(3000);
		break;
	}
}

void CVAT_TaskAutoTestPP::AutoState_Execute()
{
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nSeqMode)
	{
	case VAT_SEQ_MODE_Z_FOCUS:
	{
		AutoState_Execute_Z_Auto_Focus();
	}break;
	case VAT_SEQ_MODE_CAM_CAL:
	{
		AutoState_Execute_Cam_Center_Calibration();
	}break;
	case VAT_SEQ_MODE_PICKER_ALIGN:
	{
		AutoState_Execute_Picker_Align();
	}break;
	case VAT_SEQ_MODE_VAT:
	{
		//AutoState_Execute_Vision_Auto_Teaching();
	}break;
	case VAT_SEQ_MODE_Z_TEACHING:
	{
		AutoState_Execute_Z_Auto_Teaching();
	}break;
	default:
		ASSERT(FALSE);
		break;
	}
}

void CVAT_TaskAutoTestPP::AutoState_Pause()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d] [last step=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nLast_Excuting_Step);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 4000:
	{
		//g_TaskTestPP[m_nTestPpIdx].SetProcessCommand(base_seq_cmd::eProcCmd_Pause);
		SetProcessCmd(base_seq_cmd::eProcCmd_Pause);
		ChangeSubState(4001);
	}break;
	case 4001:
	{
	}break;
	default:
		ChangeSubState(4000);
		break;
	}
}

void CVAT_TaskAutoTestPP::AutoState_NormalComplete()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 5000:
	{

		ChangeSubState(5001);
	}break;
	case 5001:
	{

	}break;
	default:
		ChangeSubState(5000);
		break;
	}
}

void CVAT_TaskAutoTestPP::AutoState_AlarmStop()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d] [last step=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nLast_Excuting_Step);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 6000:
	{
		//g_TaskTestPP[m_nTestPpIdx].SetProcessCommand(base_seq_cmd::eProcCmd_Pause);
		SetProcessCmd(base_seq_cmd::eProcCmd_Pause);
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

void CVAT_TaskAutoTestPP::AutoState_Resume()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		//MakeLog( _T( "[MainStep=%s] [SubStep=%d]" ), GetLotStateToString( m_nAutoRunMainStep ), m_nAutoRunSubStep );
		MakeLog(_T("[MainStep=%s] [SubStep=%d] [last step=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nLast_Excuting_Step);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 7000:
	{
		//g_TaskTestPP[m_nTestPpIdx].SetProcessCommand(base_seq_cmd::eProcCmd_Resume);
		SetProcessCmd(base_seq_cmd::eProcCmd_Resume);
		ChangeSubState(7001);
	}break;
	case 7001:
	{

		//int nHandOwner = g_VatTaskSystemCtrl.get_test_pp_work_hand(m_nTestPpIdx);
		// 필요 없는 코드 주석 처리  [9/16/2025 dohyeong.kim]
// 		if (_Base_IsAnyPartSelected() != TRUE /*|| nHandOwner == -1*/) {
// 			ChangeMainState(eAutoLotState_CleanOut);
// 			break;
// 		}

		int nModeType = g_VatTaskSystemCtrl.GetSeqCurModeType();

		if (nModeType == VAT_MODE_TYPE_HAND) {
			ChangeMainState(eAutoLotState_Executing, m_nLast_Excuting_Step);
		}
		else if (nModeType == VAT_MODE_TYPE_PICKER) {
			//Wide Picker Focus 찾는 도중 Stop or Alarm 발생 시, 다시 측정.
			if (m_nLast_Excuting_Step < eExcutingStep_PickerAlign_Picker_Z_Focus_Pos_Find && m_stCurPicker.nPitchType == VAT_PITCH_TYPE_WIDE) {
				m_nLast_Excuting_Step = eExcutingStep_PickerAlign_Move_Picker_Setting_Pos;
			}

			ChangeMainState(eAutoLotState_Executing, m_nLast_Excuting_Step);
		}
		else {
			ChangeMainState(eAutoLotState_Executing);
		}

		//Vision Retry Count 초기화.
		m_nVisionRetryCnt = 0; 
	}break;

	default:
		ChangeSubState(7000);
		break;
	}
}

void CVAT_TaskAutoTestPP::AutoState_CleanOut()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 8000:
	{
// 		int nModeType = g_VatTaskSystemCtrl.GetSeqCurModeType();
// 		//Hand Type
// 		if (nModeType == VAT_MODE_TYPE_HAND) {
// 			int nStatus = _Base_GetMeasureHand_Status();
// 			if (nStatus == VAT_STATUS_MEASURE_ONGOING || nStatus == VAT_STATUS_MEASURE_WAIT) {
// 				_Base_SetMeasureHand_Status(VAT_STATUS_MEASURE_READY);
// 			}
// 		}
// 		//Part Type
// 		else{
// 			//Part중, OnGoing or Wait있을 경우 상태 Init으로 변경.(Abort일 경우)
// 			for (int nPart = VAT_IDX_STATION_1; nPart < VAT_MAX_TEST_PP_LOC_COUNT; nPart++) {
// 				int nStatus = _Base_GetMeasurePart_Status(nPart);
// 				if (nStatus == VAT_STATUS_MEASURE_ONGOING || nStatus == VAT_STATUS_MEASURE_WAIT) {
// 					_Base_SetMeasurePart_Status(nPart, VAT_STATUS_MEASURE_READY);
// 				}
// 			}
// 		}

		//OnGoing or Wait있을 경우 상태 Ready로 변경
		std::vector<int> vChkStatus = { VAT_STATUS_MEASURE_ONGOING , VAT_STATUS_MEASURE_WAIT };
		_Base_SetMeasure_Status(VAT_STATUS_MEASURE_READY, vChkStatus);



		int nHandOwner = g_VatTaskSystemCtrl.get_test_pp_work_hand(m_nTestPpIdx);

		//Test pp 선택 안하면 nHandOwner은 -1, m_nTestPpType =0 으로 되어 있음.
		//-1 처리도 필요한지 test 필요
// 		if (nHandOwner == -1 && m_nTestPpType == VAT_TESTPP_LOAD) {
// 			//Safety Move 진행
// 			ChangeSubState(8001);
// 			break;
// 		}

		if (nHandOwner != m_nTestPpType) {
			ChangeMainState(eAutoLotState_NormalComplete);
			break;
		}

		
		//Safety Move 진행
		ChangeSubState(8001);
	}break;
	case 8001:
	{
		//g_TaskTestPP[m_nTestPpIdx].SetProcessCommand(base_seq_cmd::eProcCmd_Stop);
		SetProcessCmd(base_seq_cmd::eProcCmd_Stop);
		ChangeSubState(8002);
	}break;
	case 8002:
	{
		int nHandStatus = g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
		if (nHandStatus == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
// 			CTestPPCmd_MoveAlreadyPos* pSeqCmd = new CTestPPCmd_MoveAlreadyPos();
// 			pSeqCmd->m_nCmdStage = VAT_IDX_STATION_4;
// 			pSeqCmd->m_cCmdPocket = CPoint(0, 0);
// 			pSeqCmd->m_vCmdPicker.push_back(CPoint(0, 0));
// 			pSeqCmd->m_nHandType = VAT_TESTPP_LOAD;
// 
// 			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
// 				ChangeSubState(8888);
// 			}

			CVatTestPPCmd_MovePos* pSeqCmd = new CVatTestPPCmd_MovePos(m_nSeqMode);
			pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker, Safety Position은 모든 Picker Safety
			pSeqCmd->m_strCmdStage = _T("STD Picker Safety Position");
			pSeqCmd->m_nTestPPType = m_nTestPpType;
			_Base_GetSafetyPos(pSeqCmd->m_dCmdPos); //[Cmd Position] - X,Y,Z,XP,YP

			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(8888);
			}
		}
	}break;
	case 8888:
	{
		if (g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
			break;

		g_VatTaskSystemCtrl.reserve_test_pp_work_hand(m_nTestPpIdx, -1);

		ChangeMainState(eAutoLotState_NormalComplete);

	}break;
	default:
		ChangeSubState(8000);
		break;
	}
}

//===================================================================================================[Z Auto Focus]
void CVAT_TaskAutoTestPP::AutoState_Execute_Z_Auto_Focus()
{
	if (m_nAutoRunSubStep > eExcutingStep_ZFocus_End) {
		m_nAutoRunSubStep = m_nLast_Excuting_Step;
	}
	m_nLast_Excuting_Step = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
		case eExcutingStep_ZFocus_Start:
		{
			//선택 Hand 없을 경우, CLEAN OUT 진행.
			if (_Base_IsHandSelected() != TRUE) {
				ChangeMainState(eAutoLotState_CleanOut);
				return;
			}

			int nHandOwner = g_VatTaskSystemCtrl.get_test_pp_work_hand(m_nTestPpIdx);
			if(m_nTestPpType == VAT_TESTPP_UNLOAD && nHandOwner == VAT_TESTPP_LOAD)
				return; //시작 하지 않은 Hand Progress 표기 안함.

			MakeLog(_T("[Mode=%s] [MainStep=%s] [SubStep=%d] [Start]"), GetExcuteModeStr(m_nSeqMode), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
			ChangeSubState(eExcutingStep_ZFocus_Check_PlateJig);
		}break;
		case eExcutingStep_ZFocus_Check_PlateJig:
		{
			//Auto Focus 하지 않아도 하단 Target 보이는지 확인 필요.
			//또는 Jig 있는지 없는지에 대해 알 수 있는 방법 확인.


			ChangeSubState(eExcutingStep_ZFocus_Move_Start_Pos);
		}break;
		case eExcutingStep_ZFocus_Move_Start_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//READY -> ONGOING
			_Base_SetMeasureHand_Status(VAT_STATUS_MEASURE_ONGOING);

			CVatTestPPCmd_MovePos* pSeqCmd = new CVatTestPPCmd_MovePos(m_nSeqMode);	
			CPoint cStdPicker = _Base_GetStdPicker(); //기준 Picker
			pSeqCmd->m_vCmdPicker.push_back(cStdPicker);
			pSeqCmd->m_strCmdStage = _T("Plate Jig");
			pSeqCmd->m_nTestPPType = m_nTestPpType;
			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos, eZFocus_PlateJigTargetPos); //[Cmd Position] - X,Y,Z,XP,YP

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Start Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_ZFocus_LowerCam_AF_and_UpperCam_Pre_AF);
			}

		}break;
		case eExcutingStep_ZFocus_LowerCam_AF_and_UpperCam_Pre_AF:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//AF 동시 진행.
			//Lower Cam Auto Focus
			//Upper CAm Pre Auto Focus 
			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			if (!g_VatTaskSystemCtrl.IsLoadTestPpAlreadyDone(m_nTestPpIdx)) {
				pSeqCmd->m_vDataId.push_back(GET_DATA_ID_LOWER());
			}

			pSeqCmd->m_vDataId.push_back(GET_DATA_ID_UPPER());

			pSeqCmd->m_nStatus = VISION_STATUS_AUTO_FOCUS;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Lower Cam AF & Upper Cam AF]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
				//GUI Notify(CAM AUTO FOCUS)
				CVAT_MeasureStatus& GuiNotifier = CVAT_MeasureStatus::GetInstance();
				if (pSeqCmd->m_vDataId.size() > 1) {
					GuiNotifier.UpdateVisionMeasureCnt(GET_LOWER_CAM_NUM(), VAT_VISION_MEASURE_AUTO_FOCUS);
				}
				GuiNotifier.UpdateVisionMeasureCnt(GET_UPPER_CAM_NUM(), VAT_VISION_MEASURE_AUTO_FOCUS);


				ChangeSubState(eExcutingStep_ZFocus_PlateJig_LowerTarget_Measure);
			}
		}break;
		case eExcutingStep_ZFocus_PlateJig_LowerTarget_Measure:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			if (g_VatTaskSystemCtrl.IsLoadTestPpAlreadyDone(m_nTestPpIdx)) {
				MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Plate Jig Lower Target Find Already Done]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
				ChangeSubState(eExcutingStep_ZFocus_PlateJig_UpperTarget_Move_Pre_Pos);
				break;
			}

			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			pSeqCmd->m_vDataId.push_back(GET_DATA_ID_LOWER());
			pSeqCmd->m_nStatus = VISION_STATUS_PLATE_JIG;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Plate Jig Lower Target Find]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
				//GUI Notify
				CVAT_MeasureStatus::GetInstance().UpdateVisionMeasureCnt(GET_LOWER_CAM_NUM(), VAT_VISION_MEASURE_SINGLE);

				ChangeSubState(eExcutingStep_ZFocus_PlateJig_UpperTarget_Move_Pre_Pos);
			}
		}break;
		case eExcutingStep_ZFocus_PlateJig_UpperTarget_Move_Pre_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//[Target Position] - X, Y, Z, XP, YP
			double dCmdPos[VAT_MAX_AXIS] = { 0, };
			_Base_GetSeqTargetPos(dCmdPos, eZFocus_PlateJigTargetPos);

			//[Make Pre Position] - X, Y
			double dPRE_POS_100MM_OFFSET = (m_nTestPpIdx == eTestPP_1) ? VAT_PRE_POS_100mm_OFFSET + 50000 : -(VAT_PRE_POS_100mm_OFFSET + 50000);
			dCmdPos[VAT_X] += dPRE_POS_100MM_OFFSET;
			dCmdPos[VAT_Y] += (VAT_PRE_POS_100mm_OFFSET + 50000);

			//[Target Limit Pos Check]
			_Base_CheckandCreateLimitPos(dCmdPos);

			CVatTestPPCmd_MovePos* pSeqCmd = new CVatTestPPCmd_MovePos(m_nSeqMode);
			pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker;
			pSeqCmd->m_strCmdStage = _T("Plate Jig Pre Position");
			pSeqCmd->m_nTestPPType = m_nTestPpType;
			memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPos, sizeof(dCmdPos));

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Pre Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_ZFocus_PlateJig_UpperTarget_Move_Target_Pos);
			}
		}break;
		case eExcutingStep_ZFocus_PlateJig_UpperTarget_Move_Target_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CVatTestPPCmd_MovePos* pSeqCmd = new CVatTestPPCmd_MovePos(m_nSeqMode);
			CPoint cStdPicker = _Base_GetStdPicker();
			pSeqCmd->m_vCmdPicker.push_back(cStdPicker); //Standard Picker
			pSeqCmd->m_strCmdStage = _T("Plate Jig Target Position");
			pSeqCmd->m_nTestPPType = m_nTestPpType;
			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos, eZFocus_PlateJigTargetPos); //[Cmd Position] - X,Y,Z,XP,YP

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Target Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_ZFocus_PlateJig_UpperTarget_Measure);
			}
		}break;
		case eExcutingStep_ZFocus_PlateJig_UpperTarget_Measure:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//이전 Position 저장.
			m_dPrevPosX = g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_TargetPos[m_nHand][VAT_X];
			m_dPrevPosY = g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_TargetPos[m_nHand][VAT_Y];

			//Plate Jig 상단 Target 측정.
			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			pSeqCmd->m_vDataId.push_back(GET_DATA_ID_UPPER());
			pSeqCmd->m_nStatus = VISION_STATUS_PLATE_JIG;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Measure Plate Jig Upper Target]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
				//GUI Notify
				CVAT_MeasureStatus::GetInstance().UpdateVisionMeasureCnt(GET_UPPER_CAM_NUM(), m_nVisionRetryCnt + 1);

				ChangeSubState(eExcutingStep_ZFocus_PlateJig_UpperTarget_Measure_Chk);
			}
		}break;
		case eExcutingStep_ZFocus_PlateJig_UpperTarget_Measure_Chk:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			if (m_dPrevPosX != g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_TargetPos[m_nHand][VAT_X] ||
				m_dPrevPosY != g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_TargetPos[m_nHand][VAT_Y])
			{
				m_nVisionRetryCnt++;

				if (m_nVisionRetryCnt >= DEF_VAT_VISION_RETRY_CNT_MAX) {
					_Base_AlarmReport_VAT(ERR_VAT_VISION_RETRY_CNT_OVER);
				}

				//Center Find까지 재측정.
				ChangeSubState(eExcutingStep_ZFocus_PlateJig_UpperTarget_Move_Pre_Pos);
			}
			else
			{
				//Center Find.
				ChangeSubState(eExcutingStep_ZFocus_UpperCam_AF);
			}
		}break;
		case eExcutingStep_ZFocus_UpperCam_AF:
		{
			//Upper Cam Auto Focus
			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			pSeqCmd->m_vDataId.push_back(GET_DATA_ID_UPPER());
			pSeqCmd->m_nStatus = VISION_STATUS_AUTO_FOCUS;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Upper Cam Final Auto Focus]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
				CVAT_MeasureStatus::GetInstance().UpdateVisionMeasureCnt(GET_UPPER_CAM_NUM(), VAT_VISION_MEASURE_AUTO_FOCUS);
				//해당 Cmd Tray Load PP에 Log 생성 되는지 확인
				ChangeSubState(eExcutingStep_ZFocus_Std_Picker_Move_Target_Pos);
			}
		}break;
		case eExcutingStep_ZFocus_Std_Picker_Move_Target_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CVatTestPPCmd_MovePos* pSeqCmd = new CVatTestPPCmd_MovePos(m_nSeqMode);
			CPoint cStdPicker = _Base_GetStdPicker();
			pSeqCmd->m_vCmdPicker.push_back(cStdPicker); //Standard Picker
			pSeqCmd->m_strCmdStage = _T("Plate Jig Vacuum Position");
			pSeqCmd->m_nTestPPType = m_nTestPpType;
			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos, eZFocus_PlateJigVacPos); //[Cmd Position] - X,Y,Z,XP,YP

			//Z축 과도하게 누르는 것 방지.
			//_Base_CheckandCreateZHeightCalPrePos(pSeqCmd->m_dCmdPos[VAT_Z]);

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Vacuum Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_ZFocus_Find_PlateJig_Z_Fast);
			}
		}break;
		case eExcutingStep_ZFocus_Find_PlateJig_Z_Fast:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			g_VATCommonData.ClearFirstPickOffset(m_nHand);

			CVatTestPPCmd_PickerHeightCal* pSeqCmd = new CVatTestPPCmd_PickerHeightCal();
			pSeqCmd->m_nHand = m_nHand;
			pSeqCmd->m_nSpeed = eZSpeed_Fast;
			pSeqCmd->move_offset_um = 0.0;
			pSeqCmd->m_nStage = VAT_NON_PART_PLATAE_JIG;
			pSeqCmd->m_dTargetPosZ = g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_VacuumPos[m_nHand][VAT_Z];
			pSeqCmd->m_nTestPpType = m_nTestPpType;

			//기준 Picker
			CPoint cStdPicker = _Base_GetStdPicker();
			pSeqCmd->m_vCmdPicker.push_back(cStdPicker);

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Height Cal] [Speed=Fast, move_offset=0.0, Loc=plate jig]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			_Base_SetZAutoTeachingSpd(pSeqCmd->m_nSpeed);
			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_ZFocus_Find_PlateJig_Z_Slow);
			}
		}break;
		case eExcutingStep_ZFocus_Find_PlateJig_Z_Slow:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CVatTestPPCmd_PickerHeightCal* pSeqCmd = new CVatTestPPCmd_PickerHeightCal();
			pSeqCmd->m_nHand = m_nHand;
			pSeqCmd->m_nSpeed = eZSpeed_Slow;
			pSeqCmd->move_offset_um = g_VATCommonData.GetFirstPickOffset(m_nHand);
			pSeqCmd->m_nStage = VAT_NON_PART_PLATAE_JIG;
			pSeqCmd->m_dTargetPosZ = g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_VacuumPos[m_nHand][VAT_Z];
			pSeqCmd->m_nTestPpType = m_nTestPpType;

			//기준 Picker
			CPoint cStdPicker = _Base_GetStdPicker();
			pSeqCmd->m_vCmdPicker.push_back(cStdPicker);

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Height Cal] [Speed=Slow, move_offset=0.0, Loc=plate jig]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
			_Base_SetZAutoTeachingSpd(pSeqCmd->m_nSpeed);
			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_ZFocus_Find_PlateJig_Z_End);
			}
		}break;
		case eExcutingStep_ZFocus_Find_PlateJig_Z_End:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//ONGOING -> COMPLETE
			_Base_SetMeasureHand_Status(VAT_STATUS_MEASURE_COMPLETE);

			_Base_SetZAutoTeachingSpd(eZSpeed_None);
			ChangeSubState(eExcutingStep_ZFocus_End);

		}break;
		case eExcutingStep_ZFocus_End:
		{
			//LD 측정 완료 시, UD로 소유권 넘김.
			g_VatTaskSystemCtrl.ReserveTestPpWorkIfSelected(m_nTestPpIdx);

			MakeLog(_T("[Mode=%s] [MainStep=%s] [SubStep=%d] [End]"), GetExcuteModeStr(m_nSeqMode), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			ChangeMainState(eAutoLotState_CleanOut);
		}break;
		default:
			break;
	}

	_Base_UpdateProgressRate(m_nAutoRunSubStep, m_nLast_Excuting_Step, eExcutingStep_ZFocus_End);
}


//===================================================================================================[Cam Center Calibration]
void CVAT_TaskAutoTestPP::AutoState_Execute_Cam_Center_Calibration()
{
	if (m_nAutoRunSubStep > eExcutingStep_CamCenterCal_End) {
		m_nAutoRunSubStep = m_nLast_Excuting_Step;
	}

	if (m_nLast_Excuting_Step != m_nAutoRunSubStep) {
		m_tcAutoSchedule.StartTimer();
	}

	m_nLast_Excuting_Step = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
		case eExcutingStep_CamCenterCal_Start:
		{
			//선택 Hand 없을 경우, CLEAN OUT 진행.
			if (_Base_IsHandSelected() != TRUE) {
				ChangeMainState(eAutoLotState_CleanOut);
				return;
			}

			int nHandOwner = g_VatTaskSystemCtrl.get_test_pp_work_hand(m_nTestPpIdx);
			if (m_nTestPpType == VAT_TESTPP_UNLOAD && nHandOwner == VAT_TESTPP_LOAD)
				return; //시작 하지 않은 Hand Progress 표기 안함.

			MakeLog(_T("[Mode=%s] [MainStep=%s] [SubStep=%d] [Start]"), GetExcuteModeStr(m_nSeqMode), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
			ChangeSubState(eExcutingStep_CamCenterCal_Check_PlateJig);
		}break;
		case eExcutingStep_CamCenterCal_Check_PlateJig:
		{
			//하단으로 Plate Jig있는지 확인
			//NG날 경우 Plate Jig 없는 것.


			ChangeSubState(eExcutingStep_CamCenterCal_Move_Pre_Pos);

		}break;
		case eExcutingStep_CamCenterCal_Move_Pre_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//READY -> ONGOING
			_Base_SetMeasureHand_Status(VAT_STATUS_MEASURE_ONGOING);

			//[Target Position] - X, Y, Z, XP, YP
			double dCmdPos[VAT_MAX_AXIS] = { 0, };
			_Base_GetSeqTargetPos(dCmdPos);

			//[Make Pre Position] - X, Y
// 			double dPRE_POS_100MM_OFFSET = (m_nTestPpIdx == eTestPP_1) ? VAT_PRE_POS_100mm_OFFSET : -(VAT_PRE_POS_100mm_OFFSET);
// 			dCmdPos[VAT_X] += dPRE_POS_100MM_OFFSET;
			//dCmdPos[VAT_Y] -= VAT_PRE_POS_100mm_OFFSET;


			//[Make Pre Position] - X, Y
			double dPRE_POS_100MM_OFFSET = (m_nTestPpIdx == eTestPP_1) ? VAT_PRE_POS_100mm_OFFSET + 50000 : -(VAT_PRE_POS_100mm_OFFSET + 50000);
			dCmdPos[VAT_X] += dPRE_POS_100MM_OFFSET;
			dCmdPos[VAT_Y] += (VAT_PRE_POS_100mm_OFFSET + 50000);

			//[Target Limit Pos Check]
			_Base_CheckandCreateLimitPos(dCmdPos);

			CVatTestPPCmd_MovePos* pSeqCmd = new CVatTestPPCmd_MovePos(m_nSeqMode);
			pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker, Pre Position은 모든 Picker Safety
			pSeqCmd->m_strCmdStage = _T("STD Picker Pre Position");
			pSeqCmd->m_nTestPPType = m_nTestPpType;
			
			memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPos, sizeof(dCmdPos));

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Pre Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_CamCenterCal_Move_Target_Pos);
			}
		}break;
		case eExcutingStep_CamCenterCal_Move_Target_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CVatTestPPCmd_MovePos* pSeqCmd = new CVatTestPPCmd_MovePos(m_nSeqMode);
			CPoint cStdPicker = _Base_GetStdPicker(); //기준 Picker
			pSeqCmd->m_vCmdPicker.push_back(cStdPicker);
			pSeqCmd->m_strCmdStage = _T("STD Picker Target Position");
			pSeqCmd->m_nTestPPType = m_nTestPpType;

			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos, eCamCenCal_BotCamCenterPos); //[Cmd Position] - X,Y,Z,XP,YP

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Target Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_CamCenterCal_Std_Picker_Measure);
			}
		}break;
		case eExcutingStep_CamCenterCal_Std_Picker_Measure:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//이전 Position 저장.
			m_dPrevPosX = g_VATCommonData.m_stVATCamCenterCalInfoSeq.dBotCameraPosition[m_nHand][VAT_X];
			m_dPrevPosY = g_VATCommonData.m_stVATCamCenterCalInfoSeq.dBotCameraPosition[m_nHand][VAT_Y];

			//STD Picker Hole 측정.
			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			pSeqCmd->m_vDataId.push_back(GET_DATA_ID_LOWER());
			pSeqCmd->m_nStatus = VISION_STATUS_PICKER;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Measure STD Picker]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
				//GUI Notify
				CVAT_MeasureStatus::GetInstance().UpdateVisionMeasureCnt(GET_LOWER_CAM_NUM(), m_nVisionRetryCnt + 1);

				ChangeSubState(eExcutingStep_CamCenterCal_Std_Picker_Measure_Chk);
			}
		}break;
		case eExcutingStep_CamCenterCal_Std_Picker_Measure_Chk:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			if (m_dPrevPosX != g_VATCommonData.m_stVATCamCenterCalInfoSeq.dBotCameraPosition[m_nHand][VAT_X] ||
				m_dPrevPosY != g_VATCommonData.m_stVATCamCenterCalInfoSeq.dBotCameraPosition[m_nHand][VAT_Y])
			{
				m_nVisionRetryCnt++;

				if (m_nVisionRetryCnt >= DEF_VAT_VISION_RETRY_CNT_MAX) {
					_Base_AlarmReport_VAT(ERR_VAT_VISION_RETRY_CNT_OVER);
				}

				MakeLog(_T("[MainStep=%s] [SubStep=%d] [STD Picker Measure Retry][Cnt : %d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nVisionRetryCnt);

				//Center Find까지 재측정.
				ChangeSubState(eExcutingStep_CamCenterCal_Move_Pre_Pos);
			}
			else
			{
				//Center Find.
				MakeLog(_T("[MainStep=%s] [SubStep=%d] [STD Picker Center Find]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

				ChangeSubState(eExcutingStep_CamCenterCal_Move_Z_Safety_Pos);
			}
		}break;
		case eExcutingStep_CamCenterCal_Move_Z_Safety_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CVatTestPPCmd_MovePos* pSeqCmd = new CVatTestPPCmd_MovePos(m_nSeqMode);
			pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker, Safety Position은 모든 Picker Safety
			pSeqCmd->m_strCmdStage = _T("STD Picker Safety Position");
			pSeqCmd->m_nTestPPType = m_nTestPpType;
			pSeqCmd->m_bZOnlyMove = TRUE;
			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos); //[Cmd Position] - X,Y,Z,XP,YP

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Safety Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_CamCenterCal_End);
			}
		}break;
		case eExcutingStep_CamCenterCal_End:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			MakeLog(_T("[Mode=%s] [MainStep=%s] [SubStep=%d] [End]"), GetExcuteModeStr(m_nSeqMode), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			//ONGOING -> COMPLETE
			_Base_SetMeasureHand_Status(VAT_STATUS_MEASURE_COMPLETE);

			//LD 측정 완료 시, UD로 소유권 넘김.
			g_VatTaskSystemCtrl.ReserveTestPpWorkIfSelected(m_nTestPpIdx);

			ChangeMainState(eAutoLotState_CleanOut);
		}break;
		default:
			break;
	}

	_Base_UpdateProgressRate(m_nAutoRunSubStep, m_nLast_Excuting_Step, eExcutingStep_CamCenterCal_End);

}


//===================================================================================================[Picker Align]
void CVAT_TaskAutoTestPP::AutoState_Execute_Picker_Align()
{
	if (m_nAutoRunSubStep > eExcutingStep_PickerAlign_End) {
		m_nAutoRunSubStep = m_nLast_Excuting_Step;
	}

// 	if (m_nLast_Excuting_Step != m_nAutoRunSubStep) {
// 		m_tcAutoSchedule.StartTimer();
// 	}

	if (m_stCurPicker.nPickerIdx != m_nLastPickerForProgress) {
		m_nLastPickerForProgress = m_stCurPicker.nPickerIdx;
		m_tcAutoSchedule.StartTimer();
	}

	m_nLast_Excuting_Step = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
		case eExcutingStep_PickerAlign_Start:
		{
			if (_Base_IsAnyPickerSelected() != TRUE) {
				ChangeMainState(eAutoLotState_CleanOut);
				return;
			}

			int nHandOwner = g_VatTaskSystemCtrl.get_test_pp_work_hand(m_nTestPpIdx);
			if (m_nTestPpType == VAT_TESTPP_UNLOAD && nHandOwner == VAT_TESTPP_LOAD)
				return; //시작 하지 않은 Hand Progress 표기 안함.

			MakeLog(_T("[Mode=%s] [MainStep=%s] [SubStep=%d] [Start]"), GetExcuteModeStr(m_nSeqMode), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
			ChangeSubState(eExcutingStep_PickerAlign_Check_PlateJig);
		}break;
		case eExcutingStep_PickerAlign_Check_PlateJig:
		{
			//하단으로 Plate Jig있는지 확인
			//NG날 경우 Plate Jig 없는 것.

			ChangeSubState(eExcutingStep_PickerAlign_Select_Measure_Pkr);
		}break;
		case eExcutingStep_PickerAlign_Select_Measure_Pkr:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//Current Picker 저장.
			m_stCurPicker = _Base_GetMeasurePicker_Each(VAT_STATUS_MEASURE_READY);

			int nPitchType = m_stCurPicker.nPitchType;
			int nPickerIdx = m_stCurPicker.nPickerIdx;
			if (nPitchType == -1 && nPickerIdx == -1) {
				ChangeSubState(eExcutingStep_PickerAlign_Move_Safety_Pos);
				break;
			}

			//READY -> ONGOING
			_Base_SetMeasurePicker_Status(nPitchType, nPickerIdx, VAT_STATUS_MEASURE_ONGOING);

			//CalCulate Current Picker Position 
			std::pair<double, double> pCurPkrPos = _Base_CalPitchTargetPosXY(nPitchType, nPickerIdx);
			g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerPos[m_nHand][nPitchType][nPickerIdx].first = pCurPkrPos.first;
			g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerPos[m_nHand][nPitchType][nPickerIdx].second = pCurPkrPos.second;

			//Wide 측정 시에만 Picker Z Setting 진행.
			if (nPitchType == VAT_PITCH_TYPE_WIDE) {
				int nPkrAutoFocusUse = g_DMCont.m_dmVAT.GN(NMD14_VAT_PICKER_ALIGN_AUTO_FOCUS);
				if (nPkrAutoFocusUse) {
					ChangeSubState(eExcutingStep_PickerAlign_Move_Picker_Setting_Pos);
				}
				else {
					ChangeSubState(eExcutingStep_PickerAlign_Move_Pre_Pos);
				}
			}
			else { //nPitchType == VAT_PITCH_TYPE_NARROW
				ChangeSubState(eExcutingStep_PickerAlign_Move_Pre_Pos);
			}

			//다음 Picker 측정 시, Vision Retry Cnt 초기화.
			m_nVisionRetryCnt = 0;
		}break;
		case eExcutingStep_PickerAlign_Move_Picker_Setting_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CString strCmdStage = _T("");
			strCmdStage.Format("[Picker Idx=%d] Setting Position", m_stCurPicker.nPickerIdx);

			CVatTestPPCmd_MovePos* pSeqCmd = new CVatTestPPCmd_MovePos(m_nSeqMode);
			CPoint cCurPicker = _Base_ConvertPickerIdxToPoint(m_stCurPicker.nPickerIdx);
			pSeqCmd->m_vCmdPicker.push_back(cCurPicker);
			pSeqCmd->m_strCmdStage = strCmdStage;
			pSeqCmd->m_nTestPPType = m_nTestPpType;
			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos, ePickerAlign_BeforePickerSetPos);

			//Setting Positoin Z축은 -1mm ~ +1mm(2mm)구간에서 찾음.
			pSeqCmd->m_dCmdPos[VAT_Z] -= 1000.0;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] %s Move"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, strCmdStage);

			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_PickerAlign_Picker_Setting_Pos_Measure);
			}
		}break;
		case eExcutingStep_PickerAlign_Picker_Setting_Pos_Measure:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//Current Picker Hole 측정.
			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			pSeqCmd->m_vDataId.push_back(GET_DATA_ID_LOWER());
			pSeqCmd->m_nStatus = VISION_STATUS_FIND_Z_MEASURE_PICKER;
			pSeqCmd->m_nMeasurePkrIdx = m_stCurPicker.nPickerIdx;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Picker Z Focus Pos] [Picker Idx = %d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_stCurPicker.nPickerIdx);

			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
				//GUI Notify
				CVAT_MeasureStatus::GetInstance().UpdateVisionMeasureCnt(GET_LOWER_CAM_NUM(), VAT_VISION_MEASURE_AUTO_FOCUS);

				ChangeSubState(eExcutingStep_PickerAlign_Picker_Setting_Pos_Measure_Chk);
			}
		}break;
		case eExcutingStep_PickerAlign_Picker_Setting_Pos_Measure_Chk:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			m_nVisionRetryCnt++;

			if (m_nVisionRetryCnt < DEF_VAT_VISION_RETRY_CNT_FIND_Z) {
				ChangeSubState(eExcutingStep_PickerAlign_Move_Picker_Z_Focus_Pos);
			}
			else {
				m_nVisionRetryCnt = 0;
				ChangeSubState(eExcutingStep_PickerAlign_Picker_Z_Focus_Pos_Find);
			}
		}break;
		case eExcutingStep_PickerAlign_Move_Picker_Z_Focus_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//Z축 Down Position
			double dLastPos = 0.0;
			if (!g_VATCommonData.m_stVATPickerAlignInfoSeq.vMeasuredPickerPos[m_nHand].empty()) {
				int nLastIdx = g_VATCommonData.m_stVATPickerAlignInfoSeq.vMeasuredPickerPos[m_nHand].size() - 1;
				dLastPos = g_VATCommonData.m_stVATPickerAlignInfoSeq.vMeasuredPickerPos[m_nHand][nLastIdx];
			}

			CString strCmdStage = _T("");
			strCmdStage.Format("[Picker Idx=%d] Picker Down Focus Pos", m_stCurPicker.nPickerIdx);

			CVatTestPPCmd_MovePos* pSeqCmd = new CVatTestPPCmd_MovePos(m_nSeqMode);
			CPoint cCurPicker = _Base_ConvertPickerIdxToPoint(m_stCurPicker.nPickerIdx);
			pSeqCmd->m_vCmdPicker.push_back(cCurPicker);
			pSeqCmd->m_strCmdStage = strCmdStage;
			pSeqCmd->m_nTestPPType = m_nTestPpType;
			pSeqCmd->m_dCmdPos[VAT_Z] = dLastPos + VAT_PICKER_ALIGN_Z_FIND_MOVE_OFFSET; //0.1mm
			pSeqCmd->m_bZOnlyMove = TRUE;
			
			MakeLog(_T("[MainStep=%s] [SubStep=%d] %s Move"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, strCmdStage);

			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_PickerAlign_Picker_Setting_Pos_Measure);
			}
		}break;
		case eExcutingStep_PickerAlign_Picker_Z_Focus_Pos_Find:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//Current Picker Z Find Position
			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			pSeqCmd->m_vDataId.push_back(GET_DATA_ID_LOWER());
			pSeqCmd->m_nStatus = VISION_STATUS_FIND_Z_RESULT_PICKER;
			pSeqCmd->m_nMeasurePkrIdx = m_stCurPicker.nPickerIdx;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Picker Z Find Focus Pos] [Picker Idx = %d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_stCurPicker.nPickerIdx);

			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_PickerAlign_Move_Pre_Pos);
			}
		}break;
		case eExcutingStep_PickerAlign_Move_Pre_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//[Picker Position] - X, Y, Z, XP, YP
			double dCmdPos[VAT_MAX_AXIS] = { 0, };
			_Base_GetSeqTargetPos(dCmdPos);

			//[Make Pre Position] - X, Y
			double dPRE_POS_100MM_OFFSET = (m_nTestPpIdx == eTestPP_1) ? VAT_PRE_POS_100mm_OFFSET : -(VAT_PRE_POS_100mm_OFFSET);
			dCmdPos[VAT_X] += dPRE_POS_100MM_OFFSET;
			dCmdPos[VAT_Y] += (VAT_PRE_POS_100mm_OFFSET + 50000);

			//[Target Limit Pos Check]
			_Base_CheckandCreateLimitPos(dCmdPos);

			CString strCmdStage = _T("");
			strCmdStage.Format(
				"[Picker Type=%s][Picker Idx=%d] Pre Position",
				(m_stCurPicker.nPitchType == VAT_PITCH_TYPE_WIDE) ? _T("WIDE") : _T("NARROW"),
				m_stCurPicker.nPickerIdx
			);

			CVatTestPPCmd_MovePos* pSeqCmd = new CVatTestPPCmd_MovePos(m_nSeqMode);
			pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker, Pre Position은 모든 Picker Safety
			pSeqCmd->m_strCmdStage = strCmdStage;
			pSeqCmd->m_nTestPPType = m_nTestPpType;
			memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPos, sizeof(dCmdPos));

			MakeLog(_T("[MainStep=%s] [SubStep=%d] %s Move"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, strCmdStage);

			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_PickerAlign_Move_Target_Pos);
			}
		}break;
		case eExcutingStep_PickerAlign_Move_Target_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CString strCmdStage = _T("");
			strCmdStage.Format(
				"[Picker Type=%s][Picker Idx=%d] Target Position",
				(m_stCurPicker.nPitchType == VAT_PITCH_TYPE_WIDE) ? _T("WIDE") : _T("NARROW"),
				m_stCurPicker.nPickerIdx
			);

			CVatTestPPCmd_MovePos* pSeqCmd = new CVatTestPPCmd_MovePos(m_nSeqMode);
			CPoint cCurPicker = _Base_ConvertPickerIdxToPoint(m_stCurPicker.nPickerIdx);
			pSeqCmd->m_vCmdPicker.push_back(cCurPicker);
			pSeqCmd->m_strCmdStage = strCmdStage;
			pSeqCmd->m_nTestPPType = m_nTestPpType;
			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos, ePickerAlign_AfterPickerSetPos); //[Cmd Position] - X,Y,Z,XP,YP

			MakeLog(_T("[MainStep=%s] [SubStep=%d] %s Move"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, strCmdStage);

			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_PickerAlign_Move_Cur_Picker_Measure);
			}
		}break;
		case eExcutingStep_PickerAlign_Move_Cur_Picker_Measure:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//이전 Position 저장.
			m_dPrevPosX = g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerPos[m_nHand][m_stCurPicker.nPitchType][m_stCurPicker.nPickerIdx].first;
			m_dPrevPosY = g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerPos[m_nHand][m_stCurPicker.nPitchType][m_stCurPicker.nPickerIdx].second;

			//Current Picker Hole 측정.
			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			pSeqCmd->m_vDataId.push_back(GET_DATA_ID_LOWER());
			pSeqCmd->m_nStatus = VISION_STATUS_PICKER;
			pSeqCmd->m_nMeasurePkrIdx = m_stCurPicker.nPickerIdx;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Picker Hole] [Picker Type = %d] [Picker Idx = %d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_stCurPicker.nPitchType, m_stCurPicker.nPickerIdx);

			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
				//GUI Notify
				CVAT_MeasureStatus::GetInstance().UpdateVisionMeasureCnt(GET_LOWER_CAM_NUM(), m_nVisionRetryCnt + 1);

				ChangeSubState(eExcutingStep_PickerAlign_Move_Cur_Picker_Measure_Chk);
			}
		}break;
		case eExcutingStep_PickerAlign_Move_Cur_Picker_Measure_Chk:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			if (m_dPrevPosX != g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerPos[m_nHand][m_stCurPicker.nPitchType][m_stCurPicker.nPickerIdx].first ||
				m_dPrevPosY != g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerPos[m_nHand][m_stCurPicker.nPitchType][m_stCurPicker.nPickerIdx].second)
			{
				m_nVisionRetryCnt++;

				if (m_nVisionRetryCnt >= DEF_VAT_VISION_RETRY_CNT_MAX) {
					_Base_AlarmReport_VAT(ERR_VAT_VISION_RETRY_CNT_OVER);
				}

				MakeLog(_T("[MainStep=%s] [SubStep=%d] [Picker Measure Retry]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
				MakeLog(_T("[Picker Type=%s][Picker Idx=%d][Cnt : %d]"),
					(m_stCurPicker.nPitchType == VAT_PITCH_TYPE_WIDE) ? _T("WIDE") : _T("NARROW"),
					m_stCurPicker.nPickerIdx,
					m_nVisionRetryCnt
				);

				//Center Find까지 재측정.
				ChangeSubState(eExcutingStep_PickerAlign_Move_Pre_Pos);
			}
			else
			{
				//Center Find.
				MakeLog(_T("[MainStep=%s] [SubStep=%d] [Picker Type=%s][Picker Idx=%d][Current Picker Center Find]"),
					GetLotStateToString(m_nAutoRunMainStep),
					m_nAutoRunSubStep,
					(m_stCurPicker.nPitchType == VAT_PITCH_TYPE_WIDE) ? _T("WIDE") : _T("NARROW"),
					m_stCurPicker.nPickerIdx
				);

				//READY -> ONGOING
				_Base_SetMeasurePicker_Status(m_stCurPicker.nPitchType, m_stCurPicker.nPickerIdx, VAT_STATUS_MEASURE_COMPLETE);

				ChangeSubState(eExcutingStep_PickerAlign_Select_Measure_Pkr);
			}
		}break;
		case eExcutingStep_PickerAlign_Move_Safety_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CVatTestPPCmd_MovePos* pSeqCmd = new CVatTestPPCmd_MovePos(m_nSeqMode);
			pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker, 모든 Picker Safety
			pSeqCmd->m_strCmdStage = _T("All Picker Safety Position");;
			pSeqCmd->m_nTestPPType = m_nTestPpType;
			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos); //[Cmd Position] - X,Y,Z,XP,YP
			
			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Safety Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_PickerAlign_End);
			}
		}break;
		case eExcutingStep_PickerAlign_End:
		{
			CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
			if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//LD 측정 완료 시, UD로 소유권 넘김.
			g_VatTaskSystemCtrl.ReserveTestPpWorkIfSelected(m_nTestPpIdx);

			MakeLog(_T("[Mode=%s] [MainStep=%s] [SubStep=%d] [End]"), GetExcuteModeStr(m_nSeqMode), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			ChangeMainState(eAutoLotState_CleanOut);
		}break;
		default:
			break;
	}

	_Base_UpdateProgressRate_Picker();

	// 	//※※※Case Step이 아닌 Picker Wide/Narrow 측정 완료 갯수로 ProgressBar Update※※※
	// 	//1. Picker 측정 중엔 Cur/Last Picker 동일.
	// 	int nVirtual_MaxPickerType = VAT_PITCH_TYPE_MAX * VAT_PICKER_MAX;
	// 	int nVirtual_CurPicker = m_stCurPicker.nPickerIdx;
	// 	int nVirtual_LastPicker = m_stCurPicker.nPickerIdx;
	// 
	// 	//2. Picker 측정 완료 시, Case Step Chaing처럼 다음 Picker로 변경하여 이전 Percentage 100%로 변경.
	// 	int nCurPickerStatus = _Base_GetMeasurePicker_Status(m_stCurPicker.nPitchType, m_stCurPicker.nPickerIdx);
	// 	if (nCurPickerStatus == VAT_STATUS_MEASURE_COMPLETE) {
	// 		ST_VAT_PICKER stCurPicker = _Base_GetMeasurePicker_Each(VAT_STATUS_MEASURE_READY);
	// 		nVirtual_CurPicker = stCurPicker.nPickerIdx;
	// 	}
	// 
	// 	_Base_UpdateProgressRate(nVirtual_CurPicker, nVirtual_LastPicker, nVirtual_MaxPickerType);

}

//===================================================================================================[Vision Auto Teaching]


//===================================================================================================[Z Auto Teaching]
void CVAT_TaskAutoTestPP::AutoState_Execute_Z_Auto_Teaching()
{
	switch (m_nAutoRunSubStep)
	{
	case eExcutingStep_ZTeaching_Start:
	{
		//선택 Part 없을 경우, CLEAN OUT 진행.
		if (_Base_IsAnyPartSelected() != TRUE) {
			ChangeMainState(eAutoLotState_CleanOut);
			break;
		}

		int nHandOwner = g_VatTaskSystemCtrl.get_test_pp_work_hand(m_nTestPpIdx);
		if (m_nTestPpType == VAT_TESTPP_UNLOAD && nHandOwner == VAT_TESTPP_LOAD)
			break;

		MakeLog(_T("[Mode=%s] [MainStep=%s] [SubStep=%d] [Start]"), GetExcuteModeStr(m_nSeqMode), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
		ChangeSubState(eExcutingStep_ZTeaching_CheckCondition);
	}break;
	case eExcutingStep_ZTeaching_CheckCondition:
	{
		//RECOVERY에서 모든 Mode 공통으로 사용하는 것 고려해 보기.
		if (m_nCurLoc != -1)
		{
			// [6/11/2025 dohyeong.kim] 순서 변경 하지 말것 
			
			// 1. VAT Test PP 관련 Cmd 진행 확인.
			int cmd_ready = IsAllCmdReady();
			if (cmd_ready != ERR_NO_ERROR)
				break;

			// 2. 전체 Part 측정 완료 확인.
			BOOL bIsAllComplete  = _Base_IsAllPartSameStatus(VAT_STATUS_MEASURE_COMPLETE);
			if (bIsAllComplete)
			{
				ChangeSubState(eExcutingStep_ZTeaching_End);
				break;
			}

			// 3. 현재 Part 측정 완료 확인.
			int nCurPartStatus = _Base_GetMeasurePart_Status(m_nCurLoc);
			if (nCurPartStatus == VAT_STATUS_MEASURE_COMPLETE) {
				ChangeSubState(eExcutingStep_ZTeaching_Table_Release);
				break;
			}

			// 4. Part 측정 완료 되지 않았을 경우.
			if (VAT_IDX_STATION_1 <= m_nCurLoc && m_nCurLoc <= VAT_IDX_STATION_4)
			{
				//Station Up 상태 확인.
				int nPressIdx = g_TaskAutoTestPp[m_nTestPpIdx].ConvertPressIdx(m_nCurLoc);
				if (g_TaskPressUnit[nPressIdx].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON) != ERR_NO_ERROR) {
					_Base_AlarmReport_VAT(ERR_PRESS_UNIT_1_NOT_DETECT_SAFETY_SEN + nPressIdx);
				}
			}
			else if (VAT_IDX_TEST_PP_TBL_1_LD <= m_nCurLoc && m_nCurLoc <= VAT_IDX_TEST_PP_TBL_4_UD)
			{
				int nTblIndex = g_TaskTestPP[m_nTestPpIdx].FromTestPPToLoadTableIdx(m_nCurLoc);

				//Part 소유권 확인
				bool bIsPossible = g_VatTaskSystemCtrl.get_tbl_reserved_hand(nTblIndex, m_nHand);
				if (!bIsPossible) {
					//Part 상태 변경[Init->Wait]
					_Base_SetMeasurePart_Status(m_nCurLoc, VAT_STATUS_MEASURE_WAIT);
					ChangeSubState(eExcutingStep_ZTeaching_Table_Release);
					break;
				}

				int nTblCmdStage = g_TaskTestPP[m_nTestPpIdx].FromTestPPToLoadTableCmdStageConvetor(m_nCurLoc);
				int nChkTablePos = g_TaskLoadTbl[nTblIndex].ChkMotorPos(nTblCmdStage, CTaskLoadTable::eAxisAll);
				if (nChkTablePos != ERR_NO_ERROR) {
					ChangeSubState(eExcutingStep_ZTeaching_Move_Loading_Tbl);
					break;
				}
			}
			else //clean table
			{
				//첫번째, Press Safety 위치 확인.
				int nPressIdx = (m_nTestPpIdx == eTestPP_1) ? ePressUnit_Down_1 : ePressUnit_Down_5;
				if (g_TaskPressUnit[nPressIdx].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON) != ERR_NO_ERROR) {
					_Base_AlarmReport_VAT(ERR_PRESS_UNIT_1_NOT_DETECT_SAFETY_SEN + nPressIdx);
				}
			}

			//m_nCurLoc이 Station or Table.
			//Press Safety 상태.
			//Load Table이 측정 위치에 있음.

			//Slow, Fast 확인.
			int nCurSpeed = _Base_GetZAutoTeachingSpd();
			if (nCurSpeed == eZSpeed_Fast) {
				ChangeSubState(eExcutingStep_ZTeaching_Find_Target_Z_Slow);
			}
			else if (nCurSpeed == eZSpeed_Slow) {
				ChangeSubState(eExcutingStep_ZTeaching_Find_Target_Z_End);
			}
			else {
				ChangeSubState(eExcutingStep_ZTeaching_Wait_Parts_Move);
			}
			break;
		}

		ChangeSubState(eExcutingStep_ZTeaching_Table_Release);
	}break;
	case eExcutingStep_ZTeaching_Table_Release:
	{
		//다음 Part 측정 전, Loading Table Release
		//측정 처음 일 경우 제외, 이전 Part Feeder일 경우 제외.
		if (m_nCurLoc != -1 && (VAT_IDX_TEST_PP_TBL_1_LD <= m_nCurLoc && m_nCurLoc <= VAT_IDX_TEST_PP_TBL_4_UD))
		{
			int nTblIndex = g_TaskTestPP[m_nTestPpIdx].FromTestPPToLoadTableIdx(m_nCurLoc);
			bool bIsRelease = g_VatTaskSystemCtrl.release_tbl_work_hand(nTblIndex, m_nHand);
			if (bIsRelease) {
				MakeLog(_T("[MainStep=%s] [SubStep=%d][Release Table Success][%s]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, g_TaskTestPP[m_nTestPpIdx].GetLocName(m_nCurLoc));
			}
		}

		ChangeSubState(eExcutingStep_ZTeaching_Check_All_Part_Complete);
	}break;
	case eExcutingStep_ZTeaching_Check_All_Part_Complete:
	{
		//모든 Part 측정 완료 확인.
		BOOL bIsAllComplete = _Base_IsAllPartSameStatus(VAT_STATUS_MEASURE_COMPLETE);
		if (bIsAllComplete)
		{
			ChangeSubState(eExcutingStep_ZTeaching_End);
			break;
		}

		ChangeSubState(eExcutingStep_ZTeaching_Select_Part);
	}break;

	case eExcutingStep_ZTeaching_Select_Part:
	{
		CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
		if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		m_nCurLoc = _Base_GetMeasurePart_Selected(VAT_STATUS_MEASURE_READY);

		if (m_nCurLoc != ERR_VAT_ERROR)
		{
			MakeLog(_T("[MainStep=%s] [SubStep=%d][Select Part=%s]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, g_TaskTestPP[m_nTestPpIdx].GetLocName(m_nCurLoc));
			if (VAT_IDX_TEST_PP_TBL_1_LD <= m_nCurLoc && m_nCurLoc <= VAT_IDX_TEST_PP_TBL_4_UD)
			{
				int nTblIndex = g_TaskTestPP[m_nTestPpIdx].FromTestPPToLoadTableIdx(m_nCurLoc);

				bool bIsPossible = g_VatTaskSystemCtrl.reserve_tbl_work_hand(nTblIndex, m_nHand);
				if (bIsPossible == true) {
					ChangeSubState(eExcutingStep_ZTeaching_Move_Loading_Tbl);
				}
				else {
					//Part 상태 변경[Init->Wait]
					_Base_SetMeasurePart_Status(m_nCurLoc, VAT_STATUS_MEASURE_WAIT);
					break;
				}
			}
			else //station & clean table
			{
				ChangeSubState(eExcutingStep_ZTeaching_Wait_Parts_Move);
			}

			//Part 상태 변경[Init -> Ongoing]
			_Base_SetMeasurePart_Status(m_nCurLoc, VAT_STATUS_MEASURE_ONGOING);
		}
		else
		{
			//측정 대기 Part 탐색.(Loading Table)
			m_nCurLoc = _Base_GetMeasurePart_Selected(VAT_STATUS_MEASURE_WAIT);
			if (m_nCurLoc != ERR_VAT_ERROR) {
				int nTblIndex = g_TaskTestPP[m_nTestPpIdx].FromTestPPToLoadTableIdx(m_nCurLoc);

				bool bIsPossible = g_VatTaskSystemCtrl.reserve_tbl_work_hand(nTblIndex, m_nHand);
				if (bIsPossible == true) {
					//Part 상태 변경[Wait -> Ongoing]
					_Base_SetMeasurePart_Status(m_nCurLoc, VAT_STATUS_MEASURE_ONGOING);

					ChangeSubState(eExcutingStep_ZTeaching_Move_Loading_Tbl);
				}
			}
			else {
				//여기서 부터는 Part 측정 '초기'/'대기' 상태 없음.
				ChangeSubState(eExcutingStep_ZTeaching_Check_All_Part_Complete);
			}
		}

	}break;
	case eExcutingStep_ZTeaching_Move_Loading_Tbl:
	{
		int nTblIndex = g_TaskTestPP[m_nTestPpIdx].FromTestPPToLoadTableIdx(m_nCurLoc);
		int nTblCmdStage = g_TaskTestPP[m_nTestPpIdx].FromTestPPToLoadTableCmdStageConvetor(m_nCurLoc);


		CBaseSeqCmd::eSeqCmdStatus table_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskLoadTbl[nTblIndex].GetSeqCmdStatus();
		if (table_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}


		int nChkTablePos = g_TaskLoadTbl[nTblIndex].ChkMotorPos(nTblCmdStage, CTaskLoadTable::eAxisAll);

		if (nChkTablePos != ERR_NO_ERROR)
		{
			//[Cmd Position]
			double dCmdPos[CTaskLoadTable::eMaxAxisCount] = { 0, };
			for (int i = 0; i < CTaskLoadTable::eMaxAxisCount; i++) {
				dCmdPos[i] = g_TaskLoadTbl[nTblIndex].GetTeachPos(nTblCmdStage, i);
			}

			CVatLoadTblCmd_MovePos* pSeqCmd = new CVatLoadTblCmd_MovePos(m_nSeqMode);
			pSeqCmd->m_nTableIdx = nTblIndex;
			memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPos, sizeof(dCmdPos));

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Table %d Move] [Test Site %d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, nTblIndex, nTblCmdStage);

			if (SendCommand(g_TaskLoadTbl[nTblIndex], pSeqCmd) == eSeqCmdAck_OK) {
				//ChangeSubState(eExcutingStep_ZTeaching_Wait_Parts_Move);
			}
		}

		
		ChangeSubState(eExcutingStep_ZTeaching_Wait_Parts_Move);
	
	}break;
	case eExcutingStep_ZTeaching_Wait_Parts_Move:
	{
		if (VAT_IDX_STATION_1 <= m_nCurLoc && m_nCurLoc <= VAT_IDX_STATION_4)
		{
			int nPressIdx = g_TaskAutoTestPp[m_nTestPpIdx].ConvertPressIdx(m_nCurLoc);

			CBaseSeqCmd::eSeqCmdStatus press_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskPressUnit[nPressIdx].GetSeqCmdStatus();
			if (press_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//Station Up 상태 확인.		
			if (g_TaskPressUnit[nPressIdx].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON) != ERR_NO_ERROR) {
				_Base_AlarmReport_VAT(ERR_PRESS_UNIT_1_NOT_DETECT_SAFETY_SEN + nPressIdx);
			}
		}
		else if (VAT_IDX_TEST_PP_TBL_1_LD <= m_nCurLoc && m_nCurLoc <= VAT_IDX_TEST_PP_TBL_4_UD)
		{
			int nTblIndex = g_TaskTestPP[m_nTestPpIdx].FromTestPPToLoadTableIdx(m_nCurLoc);
			int nTblCmdStage = g_TaskTestPP[m_nTestPpIdx].FromTestPPToLoadTableCmdStageConvetor(m_nCurLoc);

			CBaseSeqCmd::eSeqCmdStatus table_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskLoadTbl[nTblIndex].GetSeqCmdStatus();
			if (table_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			int nChkTablePos = g_TaskLoadTbl[nTblIndex].ChkMotorPos(nTblCmdStage, CTaskLoadTable::eAxisAll);
			if (nChkTablePos != ERR_NO_ERROR) {
				break;
			}
		}
		else //clean table
		{
			//첫번째, Press Safety 위치 확인.
			int nPressIdx = (m_nTestPpIdx == eTestPP_1) ? ePressUnit_Down_1 : ePressUnit_Down_5;
			if (g_TaskPressUnit[nPressIdx].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON) != ERR_NO_ERROR) {
				_Base_AlarmReport_VAT(ERR_PRESS_UNIT_1_NOT_DETECT_SAFETY_SEN + nPressIdx);
			}
		}

		ChangeSubState(eExcutingStep_ZTeaching_Move_Target_Pos);
	}break;
	case eExcutingStep_ZTeaching_Move_Target_Pos:
	{
		CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
		if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		//[Cmd Position] - X,Y,Z,XP,YP
		double dCmdPos[VAT_MAX_AXIS] = { 0, };
		for (int i = 0; i < VAT_MAX_AXIS_XYZ; i++) {
			dCmdPos[i] = g_VATCommonData.m_stVATZTeachInfoSeq.stZTeachBasePos.getPartBasePos(m_nHand, m_nCurLoc, i);
		}
		dCmdPos[VAT_PITCH_X] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPpIdx), m_nCurLoc, (m_nTestPpType == VAT_TESTPP_LOAD) ? POS_IDX_TEST_PP_X1_PITCH : POS_IDX_TEST_PP_X2_PITCH);
		dCmdPos[VAT_PITCH_Y] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPpIdx), m_nCurLoc, (m_nTestPpType == VAT_TESTPP_LOAD) ? POS_IDX_TEST_PP_Y1_PITCH : POS_IDX_TEST_PP_Y2_PITCH);

		CVatTestPPCmd_MovePos* pSeqCmd = new CVatTestPPCmd_MovePos(m_nSeqMode);
		pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker;
		pSeqCmd->m_strCmdStage = g_TaskTestPP[m_nTestPpIdx].GetLocName(m_nCurLoc);
		pSeqCmd->m_nTestPPType = m_nTestPpType;
		memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPos, sizeof(dCmdPos));

		MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Target Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

		if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
			ChangeSubState(eExcutingStep_ZTeaching_Find_Target_Z_Fast);
		}
	}break;
	case eExcutingStep_ZTeaching_Find_Target_Z_Fast:
	{
		CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
		if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		CVatTestPPCmd_PickerHeightCal* pSeqCmd = new CVatTestPPCmd_PickerHeightCal();
		pSeqCmd->m_nHand = m_nHand;
		pSeqCmd->m_nSpeed = eZSpeed_Fast;
		pSeqCmd->move_offset_um = 0.0;
		pSeqCmd->m_nStage = m_nCurLoc;
		pSeqCmd->m_dTargetPosZ = g_VATCommonData.m_stVATZTeachInfoSeq.stZTeachBasePos.getPartBasePos(m_nHand, m_nCurLoc, VAT_Z);

		pSeqCmd->m_nTestPpIdx = m_nTestPpIdx;
		pSeqCmd->m_nTestPpType = m_nTestPpType;

		MakeLog(_T("[MainStep=%s] [SubStep=%d] [Height Cal] [Speed=Fast, move_offset=0.0, Loc=%s]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, g_TaskTestPP[m_nTestPpIdx].GetLocName(m_nCurLoc));

		_Base_SetZAutoTeachingSpd(pSeqCmd->m_nSpeed);
		if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
			ChangeSubState(eExcutingStep_ZTeaching_Find_Target_Z_Slow);
		}

	}break;
	case eExcutingStep_ZTeaching_Find_Target_Z_Slow:
	{
		CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
		if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		CVatTestPPCmd_PickerHeightCal* pSeqCmd = new CVatTestPPCmd_PickerHeightCal();
		pSeqCmd->m_nHand = m_nHand;
		pSeqCmd->m_nSpeed = eZSpeed_Slow;
		pSeqCmd->move_offset_um = g_VATCommonData.GetFirstPickOffset(m_nHand);
		pSeqCmd->m_nStage = m_nCurLoc;
		pSeqCmd->m_dTargetPosZ = g_VATCommonData.m_stVATZTeachInfoSeq.stZTeachBasePos.getPartBasePos(m_nHand, m_nCurLoc, VAT_Z);

		pSeqCmd->m_nTestPpIdx = m_nTestPpIdx;
		pSeqCmd->m_nTestPpType = m_nTestPpType;


		MakeLog(_T("[MainStep=%s] [SubStep=%d] [Height Cal] [Speed=Slow, move_offset=%.3f, Loc=%s]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, pSeqCmd->move_offset_um, g_TaskTestPP[m_nTestPpIdx].GetLocName(m_nCurLoc));

		_Base_SetZAutoTeachingSpd(pSeqCmd->m_nSpeed);
		if (SendCommand(g_TaskTestPP[m_nTestPpIdx], pSeqCmd) == eSeqCmdAck_OK) {
			ChangeSubState(eExcutingStep_ZTeaching_Find_Target_Z_End);
		}

	}break;
	case eExcutingStep_ZTeaching_Find_Target_Z_End:
	{
		CBaseSeqCmd::eSeqCmdStatus test_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus();
		if (test_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		MakeLog(_T("[MainStep=%s] [SubStep=%d] [Check More Part]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

		_Base_SetMeasurePart_Status(m_nCurLoc, VAT_STATUS_MEASURE_COMPLETE);


		_Base_SetZAutoTeachingSpd(eZSpeed_None);
		ChangeSubState(eExcutingStep_ZTeaching_Table_Release);
	}break;
	case eExcutingStep_ZTeaching_End:
	{
		//LD 측정 완료 시, UD로 소유권 넘김.
		g_VatTaskSystemCtrl.ReserveTestPpWorkIfSelected(m_nTestPpIdx);

		// Hand 소유권 넘기는 이전 방식 [9/17/2025 dohyeong.kim]
// 		//LD 측정 완료 시, UD Part 선택 유무 상관 없이 소유권 넘김.
// 		if (m_nTestPpType == VAT_TESTPP_LOAD) {
// 			BOOL bIsUDPartSelected = (m_nTestPpIdx == eTestPP_1) ? g_VatTaskAutoUnloadTestPP1._Base_IsAnyPartSelected() : g_VatTaskAutoUnloadTestPP2._Base_IsAnyPartSelected();
// 			if (bIsUDPartSelected) {
// 				g_VatTaskSystemCtrl.reserve_test_pp_work_hand(m_nTestPpIdx, VAT_TESTPP_UNLOAD);
// 			}
// 		}

		MakeLog(_T("[MainStep=%s] [SubStep=%d] [Mode=%s] [End]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, GetExcuteModeStr(m_nSeqMode));
		
		ChangeMainState(eAutoLotState_CleanOut);
	}break;
// 	case eExcutingStep_ZTeaching_Wait_Other_Hand:
// 	{
// 	}break;



	default:
		break;
	}
}

void CVAT_TaskAutoTestPP::SetProcessCmd(base_seq_cmd::eProcessCommand eCommand)
{
	g_TaskTestPP[m_nTestPpIdx].SetProcessCommand(eCommand);
	//Test PP 1개에 LD/UD Auto가 각각 있기 때문에 SetProcessCmd는 한 곳에서만 진행 하면 됨.	
	//현재 reserve된 Hand Type에서 SetProcessCmd 진행.
	int nHandOwner = g_VatTaskSystemCtrl.get_test_pp_work_hand(m_nTestPpIdx);
	if (nHandOwner != m_nTestPpType) {
		return;
	}




	switch (m_nCurLoc)
	{
		case VAT_IDX_STATION_1:
		case VAT_IDX_STATION_2:
		case VAT_IDX_STATION_3:
		case VAT_IDX_STATION_4:
		{
			int nPressIdx = g_TaskAutoTestPp[m_nTestPpIdx].ConvertPressIdx(m_nCurLoc);
			g_TaskPressUnit[nPressIdx].SetProcessCommand(eCommand);
		}break;
		case VAT_IDX_TEST_PP_TBL_1_LD:	case VAT_IDX_TEST_PP_TBL_1_UD:
		case VAT_IDX_TEST_PP_TBL_2_LD:	case VAT_IDX_TEST_PP_TBL_2_UD:
		case VAT_IDX_TEST_PP_TBL_3_LD:	case VAT_IDX_TEST_PP_TBL_3_UD:
		case VAT_IDX_TEST_PP_TBL_4_LD:	case VAT_IDX_TEST_PP_TBL_4_UD:
		{
			int nTblIndex = g_TaskTestPP[m_nTestPpIdx].FromTestPPToLoadTableIdx(m_nCurLoc);

			//Part 소유권 확인
			bool bIsReserved = g_VatTaskSystemCtrl.get_tbl_reserved_hand(nTblIndex, m_nHand);
			if (bIsReserved) {
				g_TaskLoadTbl[nTblIndex].SetProcessCommand(eCommand);
			}
		}break;
	default: //Clean Table Motor 없음.
		break;
	}
}

int CVAT_TaskAutoTestPP::IsAllCmdReady()
{
	//VAT Test PP 관련 Cmd 확인.
	//int nRet = ERR_NO_ERROR;

	int nLoadTblIndex = (m_nTestPpIdx == eTestPP_1) ? eLoadTbl_2 : eLoadTbl_3;
	int nUnloadTblIndex = (m_nTestPpIdx == eTestPP_1) ? eLoadTbl_1 : eLoadTbl_4;

	if (g_TaskTestPP[m_nTestPpIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready ||
		g_TaskLoadTbl[nLoadTblIndex].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready ||
		g_TaskLoadTbl[nUnloadTblIndex].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready) {
		return ERR_VAT_ERROR;
	}

	return ERR_NO_ERROR;
}

// CBbxmnpClient::_ePICK_MOTOR CVAT_TaskAutoTestPP::GetBbxMnpMotorIdx()
// {
	//Test PP#1, #2 / Load, Unload 선택
// 	CBbxmnpClient::_ePICK_MOTOR eHand;
//	if (m_nTestPpIdx == eTestPP_1)
//		eHand = (m_nTestPpType == VAT_TESTPP_LOAD) ? CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1 : CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1;	
//	else //m_nTestPpIdx == eTestPP_2
//		eHand = (m_nTestPpType == VAT_TESTPP_LOAD) ? CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2 : CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2;

//	return eHand;
//}


int CVAT_TaskAutoTestPP::GET_DATA_ID_UPPER()
{	
	ASSERT(m_nTestPpIdx == eTestPP_1 || m_nTestPpIdx == eTestPP_2);
	ASSERT(m_nTestPpType == VAT_TESTPP_LOAD || m_nTestPpType == VAT_TESTPP_UNLOAD);

	if (m_nTestPpIdx == eTestPP_1) {
		return (m_nTestPpType == VAT_TESTPP_LOAD) ? VISION_REQ_DATAID_VAT_TEST_1_LD_UPPER : VISION_REQ_DATAID_VAT_TEST_1_ULD_UPPER;
	}
	else {
		return (m_nTestPpType == VAT_TESTPP_LOAD) ? VISION_REQ_DATAID_VAT_TEST_2_LD_UPPER : VISION_REQ_DATAID_VAT_TEST_2_ULD_UPPER;
	}
}


int CVAT_TaskAutoTestPP::GET_DATA_ID_LOWER()
{
	ASSERT(m_nTestPpIdx == eTestPP_1 || m_nTestPpIdx == eTestPP_2);

	return (m_nTestPpIdx == eTestPP_1) ? VISION_REQ_DATAID_VAT_TEST_1_LOWER : VISION_REQ_DATAID_VAT_TEST_2_LOWER;
}


int CVAT_TaskAutoTestPP::GET_UPPER_CAM_NUM()
{
	ASSERT(m_nTestPpIdx == eTestPP_1 || m_nTestPpIdx == eTestPP_2);
	ASSERT(m_nTestPpType == VAT_TESTPP_LOAD || m_nTestPpType == VAT_TESTPP_UNLOAD);

	if (m_nTestPpIdx == eTestPP_1) {
		return (m_nTestPpType == VAT_TESTPP_LOAD) ? VAT_CAM_LD_TEST_PP_1_UPPER : VAT_CAM_UD_TEST_PP_1_UPPER;
	}
	else {
		return (m_nTestPpType == VAT_TESTPP_LOAD) ? VAT_CAM_LD_TEST_PP_2_UPPER : VAT_CAM_UD_TEST_PP_2_UPPER;
	}
}

int CVAT_TaskAutoTestPP::GET_LOWER_CAM_NUM()
{
	ASSERT(m_nTestPpIdx == eTestPP_1 || m_nTestPpIdx == eTestPP_2);

	return (m_nTestPpIdx == eTestPP_1) ? VAT_CAM_TEST_PP_1_LOWER : VAT_CAM_TEST_PP_2_LOWER;
}