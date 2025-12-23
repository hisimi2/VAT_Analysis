#include "stdafx.h"
#include "VAT_TaskAutoTrayUnloadPP.h"


CVAT_TaskAutoTrayUnloadPP::CVAT_TaskAutoTrayUnloadPP()
{
	m_nHand = VAT_HAND_UNLOAD_TRAYPP;
}


CVAT_TaskAutoTrayUnloadPP::~CVAT_TaskAutoTrayUnloadPP()
{
}


int CVAT_TaskAutoTrayUnloadPP::Initialize()
{
	m_nAutoRunMainStep = eAutoLotState_InitNeed;
	m_nAutoRunSubStep = 0;
	m_nAutoRunBackUpMainStep = eAutoLotState_InitNeed;
	m_nAutoRunBackUpSubStep = 0;

	// 	m_eTrayIdx = eTrayPP_1_LD;
	// 	m_pTaskTrayPP = &g_TaskTrayLoadPP;

	//SetVatMainGuiHandle(hOwner); // Mode 시작 시, 다른 곳에서 진행.

	//m_nHand = VAT_HAND_UNLOAD_TRAYPP;

	return CBaseTaskManager::Initialize();
}

void CVAT_TaskAutoTrayUnloadPP::Finalize()
{
	CBaseTaskManager::Finalize();
}

int CVAT_TaskAutoTrayUnloadPP::OnEvent(const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList/*=NULL*/)
{
	return ERR_NO_ERROR;
}

int CVAT_TaskAutoTrayUnloadPP::OnCommandRsp(CBaseSeqCmd* pRspCmd, int nEventId, WPARAM wParam, LPARAM lParam, std::vector<CPoint>& vPocket_Picker)
{
	if (nEventId == CMD_EVT_ALARM)
	{
		if (pRspCmd->m_pSender == this)
			ChangeMainStateByRspAlarm();
	}

	return CBaseTaskManager::OnCommandRsp(pRspCmd, nEventId, wParam, lParam, vPocket_Picker);
}

void CVAT_TaskAutoTrayUnloadPP::OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
}

void CVAT_TaskAutoTrayUnloadPP::ThreadProc1()
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

void CVAT_TaskAutoTrayUnloadPP::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = { 0, };
	if (fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf_s(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_VAT_Auto_Tray_2), Debug, "", __LINE__, NULL, tmpMsg);
}

void CVAT_TaskAutoTrayUnloadPP::AutoState_InitNeed()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 1000:
	{
		//g_TaskTrayUnloadPP.SetProcessCommand(base_seq_cmd::eProcCmd_Stop);
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


void CVAT_TaskAutoTrayUnloadPP::AutoState_Initialize()
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
		g_DMCont.m_dmVAT.SN(NDM14_VAT_TRANSFER_SOURCE_INDEX, DEF_UNK);
		g_DMCont.m_dmVAT.SN(NDM14_VAT_TRANSFER_DESTINATION_INDEX, DEF_UNK);

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
		CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
		if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
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

//다른 Hand Safety Pos 이동 동작 대기
void CVAT_TaskAutoTrayUnloadPP::AutoState_Idle()
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

void CVAT_TaskAutoTrayUnloadPP::AutoState_Execute()
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

void CVAT_TaskAutoTrayUnloadPP::AutoState_Pause()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d] [last step=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nLast_Excuting_Step);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 4000:
	{
		//g_TaskTrayUnloadPP.SetProcessCommand(base_seq_cmd::eProcCmd_Pause);
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

void CVAT_TaskAutoTrayUnloadPP::AutoState_NormalComplete()
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

void CVAT_TaskAutoTrayUnloadPP::AutoState_AlarmStop()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d] [last step=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nLast_Excuting_Step);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 6000:
	{
		//g_TaskTrayUnloadPP.SetProcessCommand(base_seq_cmd::eProcCmd_Pause);
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

void CVAT_TaskAutoTrayUnloadPP::AutoState_Resume()
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
		//g_TaskTrayUnloadPP.SetProcessCommand(base_seq_cmd::eProcCmd_Resume);
		SetProcessCmd(base_seq_cmd::eProcCmd_Resume);
		ChangeSubState(7001);
	}break;
	case 7001:
	{
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

		//Retry Count 초기화
		m_nVisionRetryCnt = 0;

	}break;

	default:
		ChangeSubState(7000);
		break;
	}
}

void CVAT_TaskAutoTrayUnloadPP::AutoState_CleanOut()
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
// 		else {
// 			//Part중, OnGoing or Wait있을 경우 상태 Ready로 변경.(Abort일 경우)
// 			for (int nPart = VAT_IDX_TRAY_PP_TBL_1; nPart < VAT_MAX_TRAY_PP_LOC_COUNT; nPart++) {
// 				int nStatus = _Base_GetMeasurePart_Status(nPart);
// 				if (nStatus == VAT_STATUS_MEASURE_ONGOING || nStatus == VAT_STATUS_MEASURE_WAIT) {
// 					_Base_SetMeasurePart_Status(nPart, VAT_STATUS_MEASURE_READY);
// 				}
// 			}
// 		}

		//OnGoing or Wait있을 경우 상태 Ready로 변경
		std::vector<int> vChkStatus = { VAT_STATUS_MEASURE_ONGOING , VAT_STATUS_MEASURE_WAIT };
		_Base_SetMeasure_Status(VAT_STATUS_MEASURE_READY, vChkStatus);

		SetProcessCmd(base_seq_cmd::eProcCmd_Stop);
		ChangeSubState(8001);
	}break;
	case 8001:
	{
		int nHandStatus = g_TaskTrayUnloadPP.GetSeqCmdStatus();
		if (nHandStatus == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
// 			CTrayUnloadPPCmd_MovePos* pSeqCmd = new CTrayUnloadPPCmd_MovePos();
// 			pSeqCmd->m_Cmd.eStage = eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY;/*LOC_IDX_TRAY_PP_CONV_3;*/
// 			pSeqCmd->m_Cmd.Pocket = CPoint(0, 0);
// 
// 			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
// 				ChangeSubState(8888);
// 			}
			CVatTrayUnloadPPCmd_MovePos* pSeqCmd = new CVatTrayUnloadPPCmd_MovePos(m_nSeqMode);
			pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker, Safety Position은 모든 Picker Safety
			pSeqCmd->m_strCmdStage = _T("STD Picker Safety Position");
			_Base_GetSafetyPos(pSeqCmd->m_dCmdPos); //[Cmd Position] - X,Y,Z,XP,YP

			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(8888);
			}
		}
	}break;
	case 8888:
	{
		if (g_TaskTrayUnloadPP.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
			break;

		ChangeMainState(eAutoLotState_NormalComplete);

	}break;
	default:
		ChangeSubState(8000);
		break;
	}
}


//===================================================================================================[Z Auto Focus]
void CVAT_TaskAutoTrayUnloadPP::AutoState_Execute_Z_Auto_Focus()
{
	if (m_nAutoRunSubStep > eExcutingStep_ZFocus_End) {
		m_nAutoRunSubStep = m_nLast_Excuting_Step;
	}

	if (m_nLast_Excuting_Step != m_nAutoRunSubStep) {
		m_tcAutoSchedule.StartTimer();
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
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//READY -> ONGOING
			_Base_SetMeasureHand_Status(VAT_STATUS_MEASURE_ONGOING);

			CVatTrayUnloadPPCmd_MovePos* pSeqCmd = new CVatTrayUnloadPPCmd_MovePos(m_nSeqMode);

			//기준 Picker
			CPoint cStdPicker = _Base_GetStdPicker();
			pSeqCmd->m_vCmdPicker.push_back(cStdPicker);
			pSeqCmd->m_strCmdStage = _T("Plate Jig");
			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos, eZFocus_PlateJigTargetPos); //[Cmd Position] - X,Y,Z,XP,YP

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Start Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_ZFocus_LowerCam_AF_and_UpperCam_Pre_AF);
			}

		}break;
		case eExcutingStep_ZFocus_LowerCam_AF_and_UpperCam_Pre_AF:
		{
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//AF 동시 진행.
			//Lower Cam Auto Focus
			//Upper CAm Pre Auto Focus 
			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			pSeqCmd->m_vDataId.push_back(VISION_REQ_DATAID_VAT_ULD_LOWER);
			pSeqCmd->m_vDataId.push_back(VISION_REQ_DATAID_VAT_ULD_UPPER);
			pSeqCmd->m_nStatus = VISION_STATUS_AUTO_FOCUS;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Lower Cam AF & Upper Cam AF]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
				//GUI Notify(CAM AUTO FOCUS)
				CVAT_MeasureStatus& GuiNotifier = CVAT_MeasureStatus::GetInstance();
				GuiNotifier.UpdateVisionMeasureCnt(VAT_CAM_UD_TRAY_PP_LOWER, VAT_VISION_MEASURE_AUTO_FOCUS);
				GuiNotifier.UpdateVisionMeasureCnt(VAT_CAM_UD_TRAY_PP_UPPER, VAT_VISION_MEASURE_AUTO_FOCUS);

				//해당 Cmd Tray Load PP에 Log 생성 되는지 확인
				ChangeSubState(eExcutingStep_ZFocus_PlateJig_LowerTarget_Measure);
			}
		}break;
		case eExcutingStep_ZFocus_PlateJig_LowerTarget_Measure:
		{
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			pSeqCmd->m_vDataId.push_back(VISION_REQ_DATAID_VAT_ULD_LOWER);
			pSeqCmd->m_nStatus = VISION_STATUS_PLATE_JIG;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Plate Jig Lower Target Find]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
				//GUI Notify
				CVAT_MeasureStatus::GetInstance().UpdateVisionMeasureCnt(VAT_CAM_UD_TRAY_PP_LOWER, VAT_VISION_MEASURE_SINGLE);

				ChangeSubState(eExcutingStep_ZFocus_PlateJig_UpperTarget_Move_Pre_Pos);
			}
		}break;
		case eExcutingStep_ZFocus_PlateJig_UpperTarget_Move_Pre_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//[Target Position] - X, Y, Z, XP, YP
			double dCmdPos[VAT_MAX_AXIS] = { 0, };
			_Base_GetSeqTargetPos(dCmdPos, eZFocus_PlateJigTargetPos); 

			 //[Make Pre Position] - X, Y
			dCmdPos[VAT_X] -= VAT_PRE_POS_100mm_OFFSET;
			dCmdPos[VAT_Y] -= VAT_PRE_POS_100mm_OFFSET;

			//[Target Limit Pos Check]
			_Base_CheckandCreateLimitPos(dCmdPos);

			CVatTrayUnloadPPCmd_MovePos* pSeqCmd = new CVatTrayUnloadPPCmd_MovePos(m_nSeqMode);
			pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker, Pre Position은 모든 Picker Safety
			pSeqCmd->m_strCmdStage = _T("Plate Jig Pre Position");
			memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPos, sizeof(dCmdPos));

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Pre Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_ZFocus_PlateJig_UpperTarget_Move_Target_Pos);
			}

		}break;
		case eExcutingStep_ZFocus_PlateJig_UpperTarget_Move_Target_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CVatTrayUnloadPPCmd_MovePos* pSeqCmd = new CVatTrayUnloadPPCmd_MovePos(m_nSeqMode);
			CPoint cStdPicker = _Base_GetStdPicker();
			pSeqCmd->m_vCmdPicker.push_back(cStdPicker); //Standard Picker
			pSeqCmd->m_strCmdStage = _T("Plate Jig Target Position");
			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos, eZFocus_PlateJigTargetPos); //[Cmd Position] - X,Y,Z,XP,YP

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Target Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_ZFocus_PlateJig_UpperTarget_Measure);
			}
		}break;
		case eExcutingStep_ZFocus_PlateJig_UpperTarget_Measure:
		{
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//이전 Position 저장.
			m_dPrevPosX = g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_TargetPos[m_nHand][VAT_X];
			m_dPrevPosY = g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_TargetPos[m_nHand][VAT_Y];

			//Plate Jig 상단 Target 측정.
			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			pSeqCmd->m_vDataId.push_back(VISION_REQ_DATAID_VAT_ULD_UPPER);
			pSeqCmd->m_nStatus = VISION_STATUS_PLATE_JIG;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Measure Plate Jig Upper Target]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
				//GUI Notify
				CVAT_MeasureStatus::GetInstance().UpdateVisionMeasureCnt(VAT_CAM_UD_TRAY_PP_UPPER, m_nVisionRetryCnt + 1);

				ChangeSubState(eExcutingStep_ZFocus_PlateJig_UpperTarget_Measure_Chk);
			}
		}break;
		case eExcutingStep_ZFocus_PlateJig_UpperTarget_Measure_Chk:
		{
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
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
			pSeqCmd->m_vDataId.push_back(VISION_REQ_DATAID_VAT_ULD_UPPER);
			pSeqCmd->m_nStatus = VISION_STATUS_AUTO_FOCUS;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Upper Cam Final Auto Focus]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
				CVAT_MeasureStatus::GetInstance().UpdateVisionMeasureCnt(VAT_CAM_UD_TRAY_PP_UPPER, VAT_VISION_MEASURE_AUTO_FOCUS);
				//해당 Cmd Tray Load PP에 Log 생성 되는지 확인
				ChangeSubState(eExcutingStep_ZFocus_Std_Picker_Move_Target_Pos);
			}
		}break;
		case eExcutingStep_ZFocus_Std_Picker_Move_Target_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CVatTrayUnloadPPCmd_MovePos* pSeqCmd = new CVatTrayUnloadPPCmd_MovePos(m_nSeqMode);
			CPoint cStdPicker = _Base_GetStdPicker();
			pSeqCmd->m_vCmdPicker.push_back(cStdPicker); //Standard Picker
			pSeqCmd->m_strCmdStage = _T("Plate Jig Vacuum Position");
			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos, eZFocus_PlateJigVacPos); //[Cmd Position] - X,Y,Z,XP,YP
			
			//Z축 과도하게 누르는 것 방지.
			//_Base_CheckandCreateZHeightCalPrePos(pSeqCmd->m_dCmdPos[VAT_Z]);

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Vacuum Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_ZFocus_Find_PlateJig_Z_Fast);
			}
		}break;
		case eExcutingStep_ZFocus_Find_PlateJig_Z_Fast: //기준 Picker Find
		{
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			g_VATCommonData.ClearFirstPickOffset(VAT_HAND_UNLOAD_TRAYPP);

			CVatTrayUnloadPPCmd_PickerHeightCal* pSeqCmd = new CVatTrayUnloadPPCmd_PickerHeightCal();
			pSeqCmd->m_nSpeed = eZSpeed_Fast;
			pSeqCmd->move_offset_um = 0.0;
			pSeqCmd->m_nStage = VAT_NON_PART_PLATAE_JIG;
			pSeqCmd->m_dTargetPosZ = g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_VacuumPos[VAT_HAND_UNLOAD_TRAYPP][VAT_Z];
			
			//기준 Picker
			CPoint cStdPicker = _Base_GetStdPicker();
			pSeqCmd->m_vCmdPicker.push_back(cStdPicker);

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Height Cal] [Speed=Fast, move_offset=0.0, Loc=plate jig]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			_Base_SetZAutoTeachingSpd(pSeqCmd->m_nSpeed);
			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_ZFocus_Find_PlateJig_Z_Slow);
			}
		}break;
		case eExcutingStep_ZFocus_Find_PlateJig_Z_Slow:
		{
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CVatTrayUnloadPPCmd_PickerHeightCal* pSeqCmd = new CVatTrayUnloadPPCmd_PickerHeightCal();
			pSeqCmd->m_nSpeed = eZSpeed_Slow;
			pSeqCmd->move_offset_um = g_VATCommonData.GetFirstPickOffset(VAT_HAND_UNLOAD_TRAYPP);
			pSeqCmd->m_nStage = VAT_NON_PART_PLATAE_JIG;
			pSeqCmd->m_dTargetPosZ = g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_VacuumPos[VAT_HAND_UNLOAD_TRAYPP][VAT_Z];

			//기준 Picker
			CPoint cStdPicker = _Base_GetStdPicker();
			pSeqCmd->m_vCmdPicker.push_back(cStdPicker);

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Height Cal] [Speed=Slow, move_offset=0.0, Loc=plate jig]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
			_Base_SetZAutoTeachingSpd(pSeqCmd->m_nSpeed);
			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_ZFocus_Find_PlateJig_Z_End);
			}
		}break;
		case eExcutingStep_ZFocus_Find_PlateJig_Z_End:
		{
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//ONGOING -> COMPLETE
			_Base_SetMeasureHand_Status(VAT_STATUS_MEASURE_COMPLETE);

			_Base_SetZAutoTeachingSpd(eZSpeed_None);
			ChangeSubState(eExcutingStep_ZFocus_End);

		}break;
		case eExcutingStep_ZFocus_End:
		{
			MakeLog(_T("[Mode=%s] [MainStep=%s] [SubStep=%d] [End]"), GetExcuteModeStr(m_nSeqMode), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			ChangeMainState(eAutoLotState_CleanOut);
		}break;

		default:
			break;
	}

	_Base_UpdateProgressRate(m_nAutoRunSubStep, m_nLast_Excuting_Step, eExcutingStep_ZFocus_End);
}



//===================================================================================================[Cam Center Calibration]
void CVAT_TaskAutoTrayUnloadPP::AutoState_Execute_Cam_Center_Calibration()
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
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//READY -> ONGOING
			_Base_SetMeasureHand_Status(VAT_STATUS_MEASURE_ONGOING);

			//[Target Position] - X, Y, Z, XP, YP
			double dCmdPos[VAT_MAX_AXIS] = { 0, };
			_Base_GetSeqTargetPos(dCmdPos);

			//[Make Pre Position] - X, Y
			dCmdPos[VAT_X] -= VAT_PRE_POS_100mm_OFFSET;
			dCmdPos[VAT_Y] -= VAT_PRE_POS_100mm_OFFSET;

			//[Target Limit Pos Check]
			_Base_CheckandCreateLimitPos(dCmdPos);

			CVatTrayUnloadPPCmd_MovePos* pSeqCmd = new CVatTrayUnloadPPCmd_MovePos(m_nSeqMode);
			pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker, Pre Position은 모든 Picker Safety
			pSeqCmd->m_strCmdStage = _T("STD Picker Pre Position");
			memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPos, sizeof(dCmdPos));

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Pre Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_CamCenterCal_Move_Target_Pos);
			}
		}break;
		case eExcutingStep_CamCenterCal_Move_Target_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CVatTrayUnloadPPCmd_MovePos* pSeqCmd = new CVatTrayUnloadPPCmd_MovePos(m_nSeqMode);
			CPoint cStdPicker = _Base_GetStdPicker();
			pSeqCmd->m_vCmdPicker.push_back(cStdPicker); //Standard Picker
			pSeqCmd->m_strCmdStage = _T("STD Picker Target Position");
			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos, eCamCenCal_BotCamCenterPos); //[Cmd Position] - X,Y,Z,XP,YP

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Target Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_CamCenterCal_Std_Picker_Measure);
			}
		}break;
		case eExcutingStep_CamCenterCal_Std_Picker_Measure:
		{
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//이전 Position 저장.
			m_dPrevPosX = g_VATCommonData.m_stVATCamCenterCalInfoSeq.dBotCameraPosition[m_nHand][VAT_X];
			m_dPrevPosY = g_VATCommonData.m_stVATCamCenterCalInfoSeq.dBotCameraPosition[m_nHand][VAT_Y];

			//STD Picker Hole 측정.
			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			pSeqCmd->m_vDataId.push_back(VISION_REQ_DATAID_VAT_ULD_LOWER);
			pSeqCmd->m_nStatus = VISION_STATUS_PICKER;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Measure STD Picker]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
				//GUI Notify
				CVAT_MeasureStatus::GetInstance().UpdateVisionMeasureCnt(VAT_CAM_UD_TRAY_PP_LOWER, m_nVisionRetryCnt + 1);

				ChangeSubState(eExcutingStep_CamCenterCal_Std_Picker_Measure_Chk);
			}

		}break;
		case eExcutingStep_CamCenterCal_Std_Picker_Measure_Chk:
		{
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
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
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CVatTrayUnloadPPCmd_MovePos* pSeqCmd = new CVatTrayUnloadPPCmd_MovePos(m_nSeqMode);
			pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker, Safety Position은 모든 Picker Safety
			pSeqCmd->m_strCmdStage = _T("STD Picker Safety Position");
			pSeqCmd->m_bZOnlyMove = TRUE;
			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos); //[Cmd Position] - X,Y,Z,XP,YP
			
			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Safety Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_CamCenterCal_End);
			}
		}break;
		case eExcutingStep_CamCenterCal_End:
		{
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			MakeLog(_T("[Mode=%s] [MainStep=%s] [SubStep=%d] [End]"), GetExcuteModeStr(m_nSeqMode), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			//ONGOING -> COMPLETE
			_Base_SetMeasureHand_Status(VAT_STATUS_MEASURE_COMPLETE);

			ChangeMainState(eAutoLotState_CleanOut);
		}break;
		default:
			break;
	}

	_Base_UpdateProgressRate(m_nAutoRunSubStep, m_nLast_Excuting_Step, eExcutingStep_CamCenterCal_End);
}


//===================================================================================================[Picker Align]
void CVAT_TaskAutoTrayUnloadPP::AutoState_Execute_Picker_Align()
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
			//선택 Picker 없을 경우, CLEAN OUT 진행.
			if (_Base_IsAnyPickerSelected() != TRUE) {
				ChangeMainState(eAutoLotState_CleanOut);
				return;
			}

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
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
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
		/*Z축 Move Setting Position -1mm Up*/
		case eExcutingStep_PickerAlign_Move_Picker_Setting_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CString strCmdStage = _T("");
			strCmdStage.Format("[Picker Idx=%d] Setting Position", m_stCurPicker.nPickerIdx);

			CVatTrayUnloadPPCmd_MovePos* pSeqCmd = new CVatTrayUnloadPPCmd_MovePos(m_nSeqMode);
			CPoint cCurPicker = _Base_ConvertPickerIdxToPoint(m_stCurPicker.nPickerIdx);
			pSeqCmd->m_vCmdPicker.push_back(cCurPicker);
			pSeqCmd->m_strCmdStage = strCmdStage;
			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos, ePickerAlign_BeforePickerSetPos);

			//Setting Positoin Z축은 -1mm ~ +1mm(2mm)구간에서 찾음.
			pSeqCmd->m_dCmdPos[VAT_Z] -= 1000.0;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] %s Move"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, strCmdStage);

			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_PickerAlign_Picker_Setting_Pos_Measure);
			}
		}break;
		case eExcutingStep_PickerAlign_Picker_Setting_Pos_Measure:
		{
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//Current Picker Hole 측정.
			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			pSeqCmd->m_vDataId.push_back(VISION_REQ_DATAID_VAT_ULD_LOWER);
			pSeqCmd->m_nStatus = VISION_STATUS_FIND_Z_MEASURE_PICKER;
			pSeqCmd->m_nMeasurePkrIdx = m_stCurPicker.nPickerIdx;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Picker Z Focus Pos] [Picker Idx = %d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_stCurPicker.nPickerIdx);

			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
				CVAT_MeasureStatus::GetInstance().UpdateVisionMeasureCnt(VAT_CAM_UD_TRAY_PP_LOWER, VAT_VISION_MEASURE_AUTO_FOCUS);

				ChangeSubState(eExcutingStep_PickerAlign_Picker_Setting_Pos_Measure_Chk);
			}
		}break;
		case eExcutingStep_PickerAlign_Picker_Setting_Pos_Measure_Chk:
		{
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
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
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
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

			CVatTrayUnloadPPCmd_MovePos* pSeqCmd = new CVatTrayUnloadPPCmd_MovePos(m_nSeqMode);
			CPoint cCurPicker = _Base_ConvertPickerIdxToPoint(m_stCurPicker.nPickerIdx);
			pSeqCmd->m_vCmdPicker.push_back(cCurPicker);
			pSeqCmd->m_strCmdStage = strCmdStage;
			pSeqCmd->m_dCmdPos[VAT_Z] = dLastPos + VAT_PICKER_ALIGN_Z_FIND_MOVE_OFFSET; //0.1mm
			pSeqCmd->m_bZOnlyMove = TRUE;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] %s Move"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, strCmdStage);

			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_PickerAlign_Picker_Setting_Pos_Measure);
			}
		}break;
		case eExcutingStep_PickerAlign_Picker_Z_Focus_Pos_Find:
		{
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//Current Picker Z Find Position.
			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			pSeqCmd->m_vDataId.push_back(VISION_REQ_DATAID_VAT_ULD_LOWER);
			pSeqCmd->m_nStatus = VISION_STATUS_FIND_Z_RESULT_PICKER;
			pSeqCmd->m_nMeasurePkrIdx = m_stCurPicker.nPickerIdx;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Picker Z Find Focus Pos] [Picker Idx = %d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_stCurPicker.nPickerIdx);

			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_PickerAlign_Move_Pre_Pos);
			}
		}break;
		case eExcutingStep_PickerAlign_Move_Pre_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//[Picker Position] - X, Y, Z, XP, YP
			double dCmdPos[VAT_MAX_AXIS] = { 0, };
			_Base_GetSeqTargetPos(dCmdPos);

			//[Make Pre Position] - X, Y
			dCmdPos[VAT_X] -= VAT_PRE_POS_100mm_OFFSET;
			dCmdPos[VAT_Y] -= VAT_PRE_POS_100mm_OFFSET;

			//[Target Limit Pos Check]
			_Base_CheckandCreateLimitPos(dCmdPos);

			CString strCmdStage = _T("");
			strCmdStage.Format(
				"[Picker Type=%s][Picker Idx=%d] Pre Position",
				(m_stCurPicker.nPitchType == VAT_PITCH_TYPE_WIDE) ? _T("WIDE") : _T("NARROW"),
				m_stCurPicker.nPickerIdx
			);

			CVatTrayUnloadPPCmd_MovePos* pSeqCmd = new CVatTrayUnloadPPCmd_MovePos(m_nSeqMode);
			pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker, Pre Position은 모든 Picker Safety
			pSeqCmd->m_strCmdStage = strCmdStage;
			memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPos, sizeof(dCmdPos));

			MakeLog(_T("[MainStep=%s] [SubStep=%d] %s Move"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, strCmdStage);

			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_PickerAlign_Move_Target_Pos);
			}
		}break;
		case eExcutingStep_PickerAlign_Move_Target_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CString strCmdStage = _T("");
			strCmdStage.Format(
				"[Picker Type=%s][Picker Idx=%d] Target Position",
				(m_stCurPicker.nPitchType == VAT_PITCH_TYPE_WIDE) ? _T("WIDE") : _T("NARROW"),
				m_stCurPicker.nPickerIdx
			);

			CVatTrayUnloadPPCmd_MovePos* pSeqCmd = new CVatTrayUnloadPPCmd_MovePos(m_nSeqMode);
			CPoint cCurPicker = _Base_ConvertPickerIdxToPoint(m_stCurPicker.nPickerIdx);
			pSeqCmd->m_vCmdPicker.push_back(cCurPicker);
			pSeqCmd->m_strCmdStage = strCmdStage;
			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos, ePickerAlign_AfterPickerSetPos);

			MakeLog(_T("[MainStep=%s] [SubStep=%d] %s Move"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, strCmdStage);

			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_PickerAlign_Move_Cur_Picker_Measure);
			}
		}break;
		case eExcutingStep_PickerAlign_Move_Cur_Picker_Measure:
		{
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//이전 Position 저장.
			m_dPrevPosX = g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerPos[m_nHand][m_stCurPicker.nPitchType][m_stCurPicker.nPickerIdx].first;
			m_dPrevPosY = g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerPos[m_nHand][m_stCurPicker.nPitchType][m_stCurPicker.nPickerIdx].second;

			//Current Picker Hole 측정.
			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			pSeqCmd->m_vDataId.push_back(VISION_REQ_DATAID_VAT_ULD_LOWER);
			pSeqCmd->m_nStatus = VISION_STATUS_PICKER;
			pSeqCmd->m_nMeasurePkrIdx = m_stCurPicker.nPickerIdx;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Picker Hole] [Picker Type = %d] [Picker Idx = %d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_stCurPicker.nPitchType, m_stCurPicker.nPickerIdx);

			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
				//GUI Notify
				CVAT_MeasureStatus::GetInstance().UpdateVisionMeasureCnt(VAT_CAM_UD_TRAY_PP_LOWER, m_nVisionRetryCnt + 1);

				ChangeSubState(eExcutingStep_PickerAlign_Move_Cur_Picker_Measure_Chk);
			}
		}break;
		case eExcutingStep_PickerAlign_Move_Cur_Picker_Measure_Chk:
		{
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
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
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CVatTrayUnloadPPCmd_MovePos* pSeqCmd = new CVatTrayUnloadPPCmd_MovePos(m_nSeqMode);
			pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker, 모든 Picker Safety
			pSeqCmd->m_strCmdStage = _T("All Picker Safety Position");
			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos); //[Cmd Position] - X,Y,Z,XP,YP
			
			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Safety Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_PickerAlign_End);
			}
		}break;
		case eExcutingStep_PickerAlign_End:
		{
			CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
			if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

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
void CVAT_TaskAutoTrayUnloadPP::AutoState_Execute_Z_Auto_Teaching()
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

		MakeLog(_T("[Mode=%s] [MainStep=%s] [SubStep=%d]  [Start]"), GetExcuteModeStr(m_nSeqMode), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
		ChangeSubState(eExcutingStep_ZTeaching_CheckCondition);
	}break;
	case eExcutingStep_ZTeaching_CheckCondition:
	{
		//RECOVERY에서 모든 Mode 공통으로 사용하는 것 고려해 보기.
		if (m_nCurLoc != -1)
		{
			// [6/11/2025 dohyeong.kim] 순서 변경 하지 말것 

			// 1. VAT Tray Unload PP 관련 Cmd 진행 확인.
			int cmd_ready = IsAllCmdReady();
			if (cmd_ready != ERR_NO_ERROR)
				break;

			// 2. 전체 Part 측정 완료 확인.
			BOOL bIsAllComplete = _Base_IsAllPartSameStatus(VAT_STATUS_MEASURE_COMPLETE);
			if (bIsAllComplete) {
				//종료 전, Jig가 있으면 배출.
				int dest_stacker_idx = g_DMCont.m_dmVAT.GN(NDM14_VAT_TRANSFER_DESTINATION_INDEX);
				if (dest_stacker_idx != DEF_UNK){
					int stacker_rear_jig_exist = g_TaskStacker[dest_stacker_idx].ChkExistTray_RearPos_Sen(DEF_EXIST);
					if (stacker_rear_jig_exist == ERR_NO_ERROR) {
						ChangeSubState(eExcutingStep_ZTeaching_Move_Stacker_Unload);
					}
				}
						

				//Stacker에 Jig 없으면 Auto Seq End;
				ChangeSubState(eExcutingStep_ZTeaching_End);				
				break;
			}
			
			// 3. 현재 Part 측정 완료 확인.
			int nCurPartStatus = _Base_GetMeasurePart_Status(m_nCurLoc);
			if (nCurPartStatus == VAT_STATUS_MEASURE_COMPLETE) {
				ChangeSubState(eExcutingStep_ZTeaching_Table_Release);
				break;
			}

			// 4. Part 측정 완료되지 않았을 경우.
			if (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_1 || m_nCurLoc == VAT_IDX_TRAY_PP_TBL_4)
			{
				int nTblIndex = (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_1) ? eLoadTbl_1 : eLoadTbl_4;

				//현재 Table 소유권이 없거나, 현재 Hand가 소유권 가지고 있으면 bIsPossible == true
				bool bIsPossible = g_VatTaskSystemCtrl.reserve_tbl_work_hand(nTblIndex, m_nHand); 
				if (!bIsPossible) {
					//Part 상태 변경[Init->Wait]
					_Base_SetMeasurePart_Status(m_nCurLoc, VAT_STATUS_MEASURE_WAIT);
					ChangeSubState(eExcutingStep_ZTeaching_Table_Release);
					break;
				}

				int nInPos = 0;
				double dCmdPos[CTaskLoadTable::eMaxAxisCount] = { 0, };

				for (int i = 0; i < CTaskLoadTable::eMaxAxisCount; i++) {
					//[Cmd Position]
					dCmdPos[i] = g_TaskLoadTbl[nTblIndex].GetTeachPos(LOC_IDX_TBL_LD_SIDE, i);

					//[Motor Position Check]
					int nChkTablePos = g_TaskLoadTbl[nTblIndex].ChkVATMotorPos(i, dCmdPos[i]);
					if (nChkTablePos == ERR_NO_ERROR)
						nInPos++;
				}

				if (nInPos != CTaskLoadTable::eMaxAxisCount) {
					ChangeSubState(eExcutingStep_ZTeaching_Move_Loading_Tbl);
					break;
				}
			}
			else { // Stacker
				int dest_stacker_idx = g_DMCont.m_dmVAT.GN(NDM14_VAT_TRANSFER_DESTINATION_INDEX);
				if (dest_stacker_idx == DEF_UNK) {
					//Part 선택 중 Destination Setting이 안된 것.
					ChangeSubState(eExcutingStep_ZTeaching_Select_Part);
					break;
				}

				int stacker_rear_jig_exist = g_TaskStacker[dest_stacker_idx].ChkExistTray_RearPos_Sen(DEF_EXIST);
				int stacker_front_jig_exist = g_TaskStacker[dest_stacker_idx].ChkExistTray_FrontPos_Sen(DEF_EXIST);
				if (stacker_rear_jig_exist == ERR_NO_ERROR && stacker_front_jig_exist != ERR_NO_ERROR) { //Rear에 C-Tray 있을 경우
					if (g_TaskTransfer.is_safety_state() != true) {
						ChangeSubState(eExcutingStep_ZTeaching_Move_Transfer_Move_Safety);
						break;
					}
																										 
					//Transfer Safety & Rear에 Jig 존재 하면 Fast/Slow 확인.
				}
				else {
					//[Rear와 Front에 Jig가 없음]
					if (stacker_rear_jig_exist != ERR_NO_ERROR && stacker_front_jig_exist != ERR_NO_ERROR) { 

						int transfer_tray_exist = g_TaskTransfer.GetTrayExist(DEF_EXIST);
						int transfer_area_jig_exist = g_TaskTrayFeeder.ChkTrayExist_TransferArea_Sen(eExist);

						// 1) Transfer에 Pick 되어 있는 경우.
						if (transfer_tray_exist == DEF_EXIST) {
							ChangeSubState(eExcutingStep_ZTeaching_Move_Transfer_Place); //Place로 변경 필요
						}

						// 2) Transfer Area에 있는 경우.
						else if (transfer_area_jig_exist == ERR_NO_ERROR) {
							g_DMCont.m_dmVAT.SN(NDM14_VAT_TRANSFER_SOURCE_INDEX, eSTACKER_03); //Source는 Stacker#3
							g_DMCont.m_dmVAT.SN(NDM14_VAT_TRANSFER_DESTINATION_INDEX, m_nCurLoc - VAT_IDX_TRAY_PP_CONV_1); //측정할 idx 저장.	

							ChangeSubState(eExcutingStep_ZTeaching_Move_Transfer_Pick);
						}

						// 3) Stacker에 안착 되어 있는 경우.
						else {
							ChangeSubState(eExcutingStep_ZTeaching_Move_Stacker_Down);
						}
					}
					//[Front에 C-Tray 있을 경우]
					else if (stacker_rear_jig_exist != ERR_NO_ERROR && stacker_front_jig_exist == ERR_NO_ERROR) { 
						ChangeSubState(eExcutingStep_ZTeaching_Move_Stacker_Load);
					}
					//[ Rear와 Front에 Jig가 있음, Sensor Error]
					else { 
						if (g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
						{
							_Base_AlarmReport_VAT(ERR_VAT_TRAY_PP_STACKER_FRONT_AND_REAR_BOTH_POS_JIG_EXIST);
						}
						else {
							//Simulation은 Transfer가 다시 Pick하는 곳으로 보냄.
							ChangeSubState(eExcutingStep_ZTeaching_Move_Transfer_Pick);
						}
					}

					break;
				}
			}

			//m_nCurLoc이 Table or Stacker.
			//Stacker Rear에 jig가 준비 되어 있음.
			//Load Table이 측정 위치에 있음.

			//Slow, Fast 확인.
			int nCurSpeed = _Base_GetZAutoTeachingSpd();
			if (nCurSpeed == eZSpeed_Fast) {
				ChangeSubState(eExcutingStep_ZTeaching_Find_Target_Z_Slow);
			}
			else if (nCurSpeed == eZSpeed_Slow) {
				if (VAT_IDX_TRAY_PP_CONV_1 <= m_nCurLoc && m_nCurLoc <= VAT_IDX_TRAY_PP_CONV_7) {
					ChangeSubState(eExcutingStep_ZTeaching_Move_Y_Safety_Pos);
				}
				else {
					ChangeSubState(eExcutingStep_ZTeaching_Find_Target_Z_End);
				}
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
		if (m_nCurLoc != -1 && (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_1 || m_nCurLoc == VAT_IDX_TRAY_PP_TBL_4))
		{
			bool bIsRelease = g_VatTaskSystemCtrl.release_tbl_work_hand(m_nCurLoc, m_nHand);
			if (bIsRelease) {
				MakeLog(_T("[MainStep=%s] [SubStep=%d][Release Table Success][%s]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, g_TaskTrayUnloadPP.GetLocName((eLocIdxTrayPP)m_nCurLoc));
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
			MakeLog(_T("[MainStep=%s] [SubStep=%d][All Part Measure Complete]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			//종료 전, Jig가 있으면 배출.
			int dest_stacker_idx = g_DMCont.m_dmVAT.GN(NDM14_VAT_TRANSFER_DESTINATION_INDEX);
			if (dest_stacker_idx != DEF_UNK) {
				int stacker_rear_jig_exist = g_TaskStacker[dest_stacker_idx].ChkExistTray_RearPos_Sen(DEF_EXIST);
				if (stacker_rear_jig_exist == ERR_NO_ERROR) {
					ChangeSubState(eExcutingStep_ZTeaching_Move_Stacker_Unload);
					break;
				}
			}

			//Stacker에 Jig 없으면 Auto Seq End;
			ChangeSubState(eExcutingStep_ZTeaching_End);
			break;
		}

		ChangeSubState(eExcutingStep_ZTeaching_Select_Part);
	}break;
	case eExcutingStep_ZTeaching_Select_Part:
	{
		CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
		if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		m_nCurLoc = _Base_GetMeasurePart_Selected(VAT_STATUS_MEASURE_READY);

		if (m_nCurLoc != ERR_VAT_ERROR)
		{
			MakeLog(_T("[MainStep=%s] [SubStep=%d][Select Part=%s]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, g_TaskTrayUnloadPP.GetLocName((eLocIdxTrayPP)m_nCurLoc));
			if (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_1 || m_nCurLoc == VAT_IDX_TRAY_PP_TBL_4) {

				int nTblIndex = (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_1) ? eLoadTbl_1 : eLoadTbl_4;

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
			else { //Stacker#4, Stacker#5, Stacker#6, Stacker#7
				int last_dest_stacker_idx = g_DMCont.m_dmVAT.GN(NDM14_VAT_TRANSFER_DESTINATION_INDEX);
				//Tray Load PP가 Feeder 측정이 있으면, Jig 소유권 후 순위.
				//Transfer로 이동하기 위함.
				UNITCOMPONENT cFeederUnit = g_VatTaskAutoTrayLoadPP._Base_GetMeasurePart_Each(VAT_IDX_TRAY_PP_FEEDER);
				//Tray Load PP Selected
				if (cFeederUnit.first == VAT_IDX_TRAY_PP_FEEDER) {
					if (cFeederUnit.second == VAT_STATUS_MEASURE_COMPLETE) {
						if (last_dest_stacker_idx == DEF_UNK) {
							g_DMCont.m_dmVAT.SN(NDM14_VAT_TRANSFER_SOURCE_INDEX, eSTACKER_03); //Source는 Stacker#3
						}
						else {
							g_DMCont.m_dmVAT.SN(NDM14_VAT_TRANSFER_SOURCE_INDEX, last_dest_stacker_idx);
						}
						ChangeSubState(eExcutingStep_ZTeaching_Move_Transfer_Pick);
					}
					else {
						//Part 상태 변경[Init->Wait]
						_Base_SetMeasurePart_Status(m_nCurLoc, VAT_STATUS_MEASURE_WAIT);
						break;
					}
				}
				//Tray Load PP UnSelected
				else {
					if (last_dest_stacker_idx == DEF_UNK) {
						//초기 측정 일 경우.
						ChangeSubState(eExcutingStep_ZTeaching_Move_Stacker_Down); //Load C-Tray Jig			
					}
					else {
						ChangeSubState(eExcutingStep_ZTeaching_Move_Transfer_Pick);
					}
					g_DMCont.m_dmVAT.SN(NDM14_VAT_TRANSFER_SOURCE_INDEX, last_dest_stacker_idx); //이전 idx 저장.
				}


				g_DMCont.m_dmVAT.SN(NDM14_VAT_TRANSFER_DESTINATION_INDEX, m_nCurLoc - VAT_IDX_TRAY_PP_CONV_1); //측정할 idx 저장.		
			}

			//Part 상태 변경[reay -> Ongoing]
			_Base_SetMeasurePart_Status(m_nCurLoc, VAT_STATUS_MEASURE_ONGOING);
		}
		else
		{
			//측정 대기 Part 탐색.(Loading Table, Stacker)
			m_nCurLoc = _Base_GetMeasurePart_Selected(VAT_STATUS_MEASURE_WAIT);
			if(m_nCurLoc != ERR_VAT_ERROR){
				if (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_1 || m_nCurLoc == VAT_IDX_TRAY_PP_TBL_4) {
					int nTblIndex = (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_1) ? eLoadTbl_1 : eLoadTbl_4;

					bool bIsPossible = g_VatTaskSystemCtrl.reserve_tbl_work_hand(nTblIndex, m_nHand);
					if (bIsPossible == true) {
						//Part 상태 변경[Wait -> Ongoing]
						_Base_SetMeasurePart_Status(m_nCurLoc, VAT_STATUS_MEASURE_ONGOING);

						ChangeSubState(eExcutingStep_ZTeaching_Move_Loading_Tbl);
					}
				}
				else { //Feeder Measure Wait 상태는 Tray LD Pp 사용 때문임.
					UNITCOMPONENT cFeederUnit = g_VatTaskAutoTrayLoadPP._Base_GetMeasurePart_Each(VAT_IDX_TRAY_PP_FEEDER);
					if (cFeederUnit.first == VAT_IDX_TRAY_PP_FEEDER && cFeederUnit.second == VAT_STATUS_MEASURE_COMPLETE) {
						//Part 상태 변경[Wait -> Ongoing]
						_Base_SetMeasurePart_Status(m_nCurLoc, VAT_STATUS_MEASURE_ONGOING);

						int last_dest_stacker_idx = g_DMCont.m_dmVAT.GN(NDM14_VAT_TRANSFER_DESTINATION_INDEX);
						if (last_dest_stacker_idx == DEF_UNK) {
							g_DMCont.m_dmVAT.SN(NDM14_VAT_TRANSFER_SOURCE_INDEX, eSTACKER_03); //Source는 Stacker#3
						}
						else {
							g_DMCont.m_dmVAT.SN(NDM14_VAT_TRANSFER_SOURCE_INDEX, last_dest_stacker_idx);
						}
						g_DMCont.m_dmVAT.SN(NDM14_VAT_TRANSFER_DESTINATION_INDEX, m_nCurLoc - VAT_IDX_TRAY_PP_CONV_1); //측정할 idx 저장.	

						ChangeSubState(eExcutingStep_ZTeaching_Move_Transfer_Pick);
					}
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
		int nTblIndex = (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_1) ? eLoadTbl_1 : eLoadTbl_4;

		CBaseSeqCmd::eSeqCmdStatus table_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskLoadTbl[nTblIndex].GetSeqCmdStatus();
		if (table_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		int nInPos = 0;
		double dCmdPos[CTaskLoadTable::eMaxAxisCount] = { 0, };

		for (int i = 0; i < CTaskLoadTable::eMaxAxisCount; i++) {
			//[Cmd Position]
			dCmdPos[i] = g_TaskLoadTbl[nTblIndex].GetTeachPos(LOC_IDX_TBL_LD_SIDE, i);

			//[Motor Position Check]
			int nChkTablePos = g_TaskLoadTbl[nTblIndex].ChkVATMotorPos(i, dCmdPos[i]);
			if (nChkTablePos == ERR_NO_ERROR)
				nInPos++;
		}

		if (nInPos != CTaskLoadTable::eMaxAxisCount)
		{
			CVatLoadTblCmd_MovePos* pSeqCmd = new CVatLoadTblCmd_MovePos(m_nSeqMode);
			pSeqCmd->m_nTableIdx = nTblIndex;
			memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPos, sizeof(dCmdPos));

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Table %d Move Load Site]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, nTblIndex);

			if (SendCommand(g_TaskLoadTbl[nTblIndex], pSeqCmd) == eSeqCmdAck_OK) {
				//ChangeSubState(eExcutingStep_ZTeaching_Wait_Parts_Move);
			}

		}

		ChangeSubState(eExcutingStep_ZTeaching_Wait_Parts_Move);
		
	}break;
	case eExcutingStep_ZTeaching_Move_Stacker_Down:
	{
		int dest_stacker_idx = g_DMCont.m_dmVAT.GN(NDM14_VAT_TRANSFER_DESTINATION_INDEX);

		MakeLog(_T("[MainStep=%s] [SubStep=%d] [Stacker %d Down]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, dest_stacker_idx);

		int stacker_front_jig_not_exist = g_TaskStacker[dest_stacker_idx].ChkExistTray_FrontPos_Sen(DEF_NOT_EXIST);
		if (stacker_front_jig_not_exist == ERR_NO_ERROR) {
			CStackerCmd_TrayDn* pSeqCmd = new CStackerCmd_TrayDn();
			if (SendCommand(g_TaskStacker[dest_stacker_idx], pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_ZTeaching_Move_Stacker_Load);
				break;
			}
		}
		else {
			CString strAlarmData = _T("");
			strAlarmData.Format("Stacker %d", dest_stacker_idx + 1);
			
			_Base_AlarmReport_VAT(ERR_VAT_TRAY_PP_STACKER_FRONT_POS_JIG_EXIST, strAlarmData);
		}
	}break;
	case eExcutingStep_ZTeaching_Move_Stacker_Load:
	{
		int dest_stacker_idx = g_DMCont.m_dmVAT.GN(NDM14_VAT_TRANSFER_DESTINATION_INDEX);
		if (g_TaskStacker[dest_stacker_idx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		int stacker_rear_jig_not_exist = g_TaskStacker[dest_stacker_idx].ChkExistTray_RearPos_Sen(DEF_NOT_EXIST);
		int stacker_front_jig_exist = g_TaskStacker[dest_stacker_idx].ChkExistTray_FrontPos_Sen(DEF_EXIST);
		if (stacker_rear_jig_not_exist == ERR_NO_ERROR && stacker_front_jig_exist == ERR_NO_ERROR) {
			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Stacker %d Load]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, dest_stacker_idx);
			CStackerCmd_Load* pSeqCmd = new CStackerCmd_Load(true);
			if (SendCommand(g_TaskStacker[dest_stacker_idx], pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_ZTeaching_Wait_Parts_Move);
			}
		}
		else {
			//Rear, Front Position에 대한 알람.
			CString strAlarmData = _T("");
			strAlarmData.Format("Stacker %d", dest_stacker_idx + 1);

			_Base_AlarmReport_VAT(((stacker_rear_jig_not_exist != ERR_NO_ERROR) ? ERR_VAT_TRAY_PP_STACKER_REAR_POS_JIG_EXIST : ERR_VAT_TRAY_PP_STACKER_FRONT_POS_JIG_NOT_EXIST), strAlarmData);
		}
	}break;
	case eExcutingStep_ZTeaching_Move_Transfer_Pick:
	{
		if (g_TaskTransfer.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready ||
			g_TaskTrayFeeder.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		//Feeder Safety 위치 확인
		if(g_TaskTrayFeeder.GetSafetySensor() != true)
			break;

		//source index가 3번이면, Transfer Area
		int src_stacker_idx = g_DMCont.m_dmVAT.GN(NDM14_VAT_TRANSFER_SOURCE_INDEX);
		if (src_stacker_idx == eSTACKER_03) {
			//Feeder에 jig없고, Transfer Area에 있어야 함.
			int feeder_jig_not_exist = g_TaskTrayFeeder.ChkTrayExistSensor(eNot_Exist);
			int feeder_safety_pos = g_TaskTrayFeeder.ChkMotorPos(CTaskTrayFeeder::eAxisY, LOC_IDX_FEEDER_SAFETY);
			int transfer_area_jig_exist = g_TaskTrayFeeder.ChkTrayExist_TransferArea_Sen(eExist);
			
			if(!(feeder_safety_pos == ERR_NO_ERROR && feeder_jig_not_exist == ERR_NO_ERROR && transfer_area_jig_exist == ERR_NO_ERROR))
				break;
		}


		MakeLog(_T("[MainStep=%s] [SubStep=%d] [Transfer Move Pick] [Src Stacker %d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, src_stacker_idx);
		CTransferCmd_MovePickPlace* pSeqCmd = new CTransferCmd_MovePickPlace(src_stacker_idx, DEF_PICK);
		if (SendCommand(g_TaskTransfer, pSeqCmd) == eSeqCmdAck_OK) {
			ChangeSubState(eExcutingStep_ZTeaching_Move_Transfer_Place);
		}
	}break;
	case eExcutingStep_ZTeaching_Move_Transfer_Place:
	{
		if (g_TaskTransfer.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		//Feeder Safety 위치 확인
		if (g_TaskTrayFeeder.GetSafetySensor() != true)
			break;

		int dest_stacker_idx = g_DMCont.m_dmVAT.GN(NDM14_VAT_TRANSFER_DESTINATION_INDEX);
		MakeLog(_T("[MainStep=%s] [SubStep=%d] [Transfer Move Place] [Dest Stacker %d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, dest_stacker_idx);
		CTransferCmd_MovePickPlace* pSeqCmd = new CTransferCmd_MovePickPlace(dest_stacker_idx, DEF_PLACE);
		if (SendCommand(g_TaskTransfer, pSeqCmd) == eSeqCmdAck_OK) {
			ChangeSubState(eExcutingStep_ZTeaching_Move_Transfer_Move_Safety);
		}
	}break;
	case eExcutingStep_ZTeaching_Move_Transfer_Move_Safety:
	{
		if (g_TaskTransfer.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		//Feeder Safety 위치 확인
		if (g_TaskTrayFeeder.GetSafetySensor() != true)
			break;

		CTransferCmd_MovePos* pSeqCmd = new CTransferCmd_MovePos(LOC_IDX_TRANSFER_SAFETY);
		if (SendCommand(g_TaskTransfer, pSeqCmd) == eSeqCmdAck_OK) {
			ChangeSubState(eExcutingStep_ZTeaching_Wait_Parts_Move);
		}
	}break;
	case eExcutingStep_ZTeaching_Wait_Parts_Move:
	{
		if (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_1 || m_nCurLoc == VAT_IDX_TRAY_PP_TBL_4)
		{
			int nTblIndex = (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_1) ? eLoadTbl_1 : eLoadTbl_4;
			CBaseSeqCmd::eSeqCmdStatus table_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskLoadTbl[nTblIndex].GetSeqCmdStatus();
			if (table_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}
		}
		else 
		{
			//Transfer Cmd Ready 확인.
			CBaseSeqCmd::eSeqCmdStatus transfer_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTransfer.GetSeqCmdStatus();
			if(transfer_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready)
				break;

			//Transfer Safety check
			if (g_TaskTransfer.is_safety_state() != true) {
				_Base_AlarmReport_VAT(ERR_TRNASFER_IS_NOT_SAFETY_STATE); 
				break;
			}

			//Stacker Cmd Ready 확인.
			int dest_stacker_idx = g_DMCont.m_dmVAT.GN(NDM14_VAT_TRANSFER_DESTINATION_INDEX);
			if (g_TaskStacker[dest_stacker_idx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//m_nCurLoc 위치에 따른 Stacker Rear Sensor 확인.
			int nErr = g_TaskStacker[dest_stacker_idx].ChkExistTray_RearPos_Sen(DEF_EXIST);
			if (nErr != ERR_NO_ERROR) // rear에 존재 해야 함.
			{
				CString strAlarmData = _T("");
				strAlarmData.Format("Stacker %d", dest_stacker_idx + 1);

				_Base_AlarmReport_VAT(ERR_VAT_TRAY_PP_STACKER_REAR_POS_JIG_NOT_EXIST, strAlarmData);
				break;
			}
		}

		ChangeSubState(eExcutingStep_ZTeaching_Move_Target_Pos);
	}break;
	case eExcutingStep_ZTeaching_Move_Target_Pos:
	{
		CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
		if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		//[Cmd Position] - X,Y,Z,XP,YP
		double dCmdPos[VAT_MAX_AXIS] = { 0, };
		for (int i = 0; i < VAT_MAX_AXIS_XYZ; i++) {
			dCmdPos[i] = g_VATCommonData.m_stVATZTeachInfoSeq.stZTeachBasePos.getPartBasePos(VAT_HAND_UNLOAD_TRAYPP, m_nCurLoc, i);
		}
		int nLocStageConv = g_TaskTrayUnloadPP.TrayHandLocIdxConvertor(RIDX_TRAY_PP_2, m_nCurLoc);
		dCmdPos[VAT_PITCH_X] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_2), nLocStageConv, POS_IDX_TRAY_PP_X_PITCH);
		dCmdPos[VAT_PITCH_Y] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_2), nLocStageConv, POS_IDX_TRAY_PP_Y_PITCH);

		CVatTrayUnloadPPCmd_MovePos* pSeqCmd = new CVatTrayUnloadPPCmd_MovePos(m_nSeqMode);
		pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker;
		pSeqCmd->m_strCmdStage = g_TaskTrayUnloadPP.GetLocName((eLocIdxTrayPP)m_nCurLoc);
		memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPos, sizeof(dCmdPos));

		MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Target Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

		if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
			ChangeSubState(eExcutingStep_ZTeaching_Find_Target_Z_Fast);
		}
	}break;
	/*
	여기 까지 Target Unit 및 Hand가 측정 위치에 있어야 함.

	Z Auto Teaching 시작.
	*/

	case eExcutingStep_ZTeaching_Find_Target_Z_Fast:
	{
		CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
		if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		g_VATCommonData.ClearFirstPickOffset(VAT_HAND_UNLOAD_TRAYPP);

		CVatTrayUnloadPPCmd_PickerHeightCal* pSeqCmd = new CVatTrayUnloadPPCmd_PickerHeightCal();
		pSeqCmd->m_nSpeed = eZSpeed_Fast;
		pSeqCmd->move_offset_um = 0.0;
		pSeqCmd->m_nStage = m_nCurLoc;
		pSeqCmd->m_dTargetPosZ = g_VATCommonData.m_stVATZTeachInfoSeq.stZTeachBasePos.getPartBasePos(VAT_HAND_UNLOAD_TRAYPP, m_nCurLoc, VAT_Z);	
		//MakeCmdPicker(pSeqCmd->m_vCmdPicker);

		MakeLog(_T("[MainStep=%s] [SubStep=%d] [Height Cal] [Speed=Fast, move_offset=0.0, Loc=%s]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, g_TaskTrayUnloadPP.GetLocName((eLocIdxTrayPP)m_nCurLoc));

		_Base_SetZAutoTeachingSpd(pSeqCmd->m_nSpeed);
		if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
			ChangeSubState(eExcutingStep_ZTeaching_Find_Target_Z_Slow);
		}

	}break;
	case eExcutingStep_ZTeaching_Find_Target_Z_Slow:
	{
		CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
		if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		CVatTrayUnloadPPCmd_PickerHeightCal* pSeqCmd = new CVatTrayUnloadPPCmd_PickerHeightCal();
		pSeqCmd->m_nSpeed = eZSpeed_Slow;
		pSeqCmd->move_offset_um = g_VATCommonData.GetFirstPickOffset(VAT_HAND_UNLOAD_TRAYPP);
		pSeqCmd->m_nStage = m_nCurLoc;
		pSeqCmd->m_dTargetPosZ = g_VATCommonData.m_stVATZTeachInfoSeq.stZTeachBasePos.getPartBasePos(VAT_HAND_UNLOAD_TRAYPP, m_nCurLoc, VAT_Z);
		//MakeCmdPicker(pSeqCmd->m_vCmdPicker);
		//MakePickerBMAxis(pSeqCmd->m_vBMAxis, CBbxmnpClient::PICK_MOTOR_UNLOAD_TRAY);

		MakeLog(_T("[MainStep=%s] [SubStep=%d] [Height Cal] [Speed=Slow, move_offset=%.3f, Loc=%s]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, pSeqCmd->move_offset_um, g_TaskTrayUnloadPP.GetLocName((eLocIdxTrayPP)m_nCurLoc));

		_Base_SetZAutoTeachingSpd(pSeqCmd->m_nSpeed);
		if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
			if (VAT_IDX_TRAY_PP_CONV_1 <= m_nCurLoc && m_nCurLoc <= VAT_IDX_TRAY_PP_CONV_7) {
				ChangeSubState(eExcutingStep_ZTeaching_Move_Y_Safety_Pos);
			}
			else {
				ChangeSubState(eExcutingStep_ZTeaching_Find_Target_Z_End);
			}			
		}
	}break;
	case eExcutingStep_ZTeaching_Move_Y_Safety_Pos: //Transfer Move시, Interlock 회피.
	{
		CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
		if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		//Hand Safety로 보내는 것은 이동 시간 Loss가 큼.
		//측정 위치에서 Y만 Safety로 보내는 것.

		//[Cmd Positoin 생성] 
		// X : VAT Base Pos
		// Y : Teaching Pos(Safety)
		// Z : Teaching Pos(Safety)
		//XP : Teaching Pos
		//YP : Teaching Pos
		double dCmdPos[VAT_MAX_AXIS] = { 0, };
		int nLocStageConv = g_TaskTrayUnloadPP.TrayHandLocIdxConvertor(RIDX_TRAY_PP_2, m_nCurLoc);

		dCmdPos[VAT_X] = g_VATCommonData.m_stVATZTeachInfoSeq.stZTeachBasePos.getPartBasePos(VAT_HAND_UNLOAD_TRAYPP, m_nCurLoc, VAT_X);
		dCmdPos[VAT_Y] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_2), LOC_IDX_TRAY_UNLOAD_PP_SAFETY, POS_IDX_TRAY_PP_Y);
		dCmdPos[VAT_Z] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_2), LOC_IDX_TRAY_UNLOAD_PP_SAFETY, POS_IDX_TRAY_PP_Z_SAFETY);
// 		for (int i = 0; i < VAT_MAX_AXIS_XYZ; i++) {
// 			if (i == VAT_Y) {
// 				dCmdPos[VAT_Y] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_2), LOC_IDX_TRAY_UNLOAD_PP_SAFETY, POS_IDX_TRAY_PP_Y);
// 				continue;
// 			}
// 
// 			dCmdPos[i] = g_VATCommonData.m_stVATZTeachInfoSeq.stZTeachBasePos.getPartBasePos(VAT_HAND_UNLOAD_TRAYPP, m_nCurLoc, i);
// 		}		
		dCmdPos[VAT_PITCH_X] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_2), nLocStageConv, POS_IDX_TRAY_LOAD_PP_X_PITCH);
		dCmdPos[VAT_PITCH_Y] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_2), nLocStageConv, POS_IDX_TRAY_LOAD_PP_Y_PITCH);

		CVatTrayUnloadPPCmd_MovePos* pSeqCmd = new CVatTrayUnloadPPCmd_MovePos(m_nSeqMode);
		pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker
		pSeqCmd->m_strCmdStage = g_TaskTrayUnloadPP.GetLocName((eLocIdxTrayPP)m_nCurLoc);
		memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPos, sizeof(dCmdPos));

		MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Target Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

		if (SendCommand(g_TaskTrayUnloadPP, pSeqCmd) == eSeqCmdAck_OK) {
			ChangeSubState(eExcutingStep_ZTeaching_Find_Target_Z_End);
		}
	}break;
	case eExcutingStep_ZTeaching_Find_Target_Z_End:
	{
		CBaseSeqCmd::eSeqCmdStatus unload_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayUnloadPP.GetSeqCmdStatus();
		if (unload_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		//CurLoc이 Stacker 위치 이면 Safety 확인.
		if (VAT_IDX_TRAY_PP_CONV_1 <= m_nCurLoc && m_nCurLoc <= VAT_IDX_TRAY_PP_CONV_7)
		{
			//Unload Tray Y Safety Sensor 확인.
			bool bSafetySen = g_TaskTrayUnloadPP.get_TrayUnload_Y_Safety_sen(DEF_ON);
			if (bSafetySen != true) {
				_Base_AlarmReport_VAT(ERR_TRAY_PP_2_Y_IS_NOT_DETECT_SAFETY_SENSOR);
				break;
			}
		}

		MakeLog(_T("[MainStep=%s] [SubStep=%d] [Check More Part]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

		_Base_SetMeasurePart_Status(m_nCurLoc, VAT_STATUS_MEASURE_COMPLETE);

//		SetPartStatus(m_nCurLoc, VAT_STATUS_MEASURE_COMPLETE);
// 		HWND hOwner = GetVatMainGuiHandle();
// 		if (hOwner != NULL)
// 			::SendMessage(hOwner, UM_VAT_SEQ_MSG_CHANGE_PART_COLOR, (WPARAM)m_nHand, (LPARAM)m_nCurLoc);

		_Base_SetZAutoTeachingSpd(eZSpeed_None);
		ChangeSubState(eExcutingStep_ZTeaching_Table_Release);
	}break;
	case eExcutingStep_ZTeaching_Move_Stacker_Unload:
	{
		int dest_stacker_idx = g_DMCont.m_dmVAT.GN(NDM14_VAT_TRANSFER_DESTINATION_INDEX);
		if (g_TaskStacker[dest_stacker_idx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}
	
		CStackerCmd_Unload* pSeqCmd = new CStackerCmd_Unload(true);
		if (SendCommand(g_TaskStacker[dest_stacker_idx], pSeqCmd) == eSeqCmdAck_OK) {
			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Stacker %d Load]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, dest_stacker_idx);
			ChangeSubState(eExcutingStep_ZTeaching_Move_Stacker_Up);
		}
	}break;
	case eExcutingStep_ZTeaching_Move_Stacker_Up:
	{
		int dest_stacker_idx = g_DMCont.m_dmVAT.GN(NDM14_VAT_TRANSFER_DESTINATION_INDEX);
		if (g_TaskStacker[dest_stacker_idx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		int nErr = g_TaskStacker[dest_stacker_idx].ChkExistTray_FrontPos_Sen(DEF_EXIST);
		if (nErr == ERR_NO_ERROR) {
			CStackerCmd_TrayUp* pSeqCmd = new CStackerCmd_TrayUp;
			if (SendCommand(g_TaskStacker[dest_stacker_idx], pSeqCmd) == eSeqCmdAck_OK) {
				MakeLog(_T("[MainStep=%s] [SubStep=%d] [Stacker %d Up]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, dest_stacker_idx);
				ChangeSubState(eExcutingStep_ZTeaching_End);
			}
		}
	}break;
	case eExcutingStep_ZTeaching_End:
	{
		if (VAT_IDX_TRAY_PP_CONV_1 <= m_nCurLoc && m_nCurLoc <= VAT_IDX_TRAY_PP_CONV_7) {
			int dest_stacker_idx = g_DMCont.m_dmVAT.GN(NDM14_VAT_TRANSFER_DESTINATION_INDEX);
			if (g_TaskStacker[dest_stacker_idx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}
		}
		//SetFinishFlag(TRUE);

		MakeLog(_T("[Mode=%s] [MainStep=%s] [SubStep=%d] [End]"), GetExcuteModeStr(m_nSeqMode), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep );

		ChangeMainState(eAutoLotState_CleanOut);
		//ChangeSubState(eExcutingStep_ZTeaching_Wait_Other_Hand);
	}break;
// 	case eExcutingStep_ZTeaching_Wait_Other_Hand:
// 	{
// 	}break;
	default:
		break;
	}
}

int CVAT_TaskAutoTrayUnloadPP::IsAllCmdReady()
{
	//VAT Tray Unload Hand 관련된 cmd 확인.
	int nRet = ERR_NO_ERROR;

	int test = g_TaskTrayUnloadPP.GetSeqCmdStatus();
	int test1 = g_TaskLoadTbl[eLoadTbl_1].GetSeqCmdStatus();
	int test2 = g_TaskLoadTbl[eLoadTbl_4].GetSeqCmdStatus();
	int test3 = g_TaskTransfer.GetSeqCmdStatus();


	if (g_TaskTrayUnloadPP.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready ||
		g_TaskLoadTbl[eLoadTbl_1].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready ||
		g_TaskLoadTbl[eLoadTbl_4].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready ||
		g_TaskTransfer.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready) {
		nRet = ERR_VAT_ERROR;
		return nRet;
	}

	for (int i = 0; i < eMaxStackerCount; i++)
	{
		int ntest = g_TaskStacker[i].GetSeqCmdStatus();
		if (g_TaskStacker[i].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			nRet = ERR_VAT_ERROR;
			break;
		}
	}	

	return nRet;
}

void CVAT_TaskAutoTrayUnloadPP::SetProcessCmd(base_seq_cmd::eProcessCommand eCommand)
{
	g_TaskTrayUnloadPP.SetProcessCommand(eCommand);

	switch (m_nCurLoc)
	{
		case VAT_IDX_TRAY_PP_TBL_1:
		case VAT_IDX_TRAY_PP_TBL_4:
		{
			int nTblIndex = (m_nCurLoc == LOC_IDX_TRAY_PP_TBL_1) ? eLoadTbl_1 : eLoadTbl_4;
			bool bIsReserved = g_VatTaskSystemCtrl.get_tbl_reserved_hand(nTblIndex, m_nHand);
			if (bIsReserved) {
				g_TaskLoadTbl[nTblIndex].SetProcessCommand(eCommand);
			}
// 			if (g_TaskLoadTbl[nTblIndex].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready) {
// 				g_TaskLoadTbl[nTblIndex].SetProcessCommand(eCommand);
// 			}
		}break;
		case VAT_IDX_TRAY_PP_CONV_4:
		case VAT_IDX_TRAY_PP_CONV_5:
		case VAT_IDX_TRAY_PP_CONV_6:
		case VAT_IDX_TRAY_PP_CONV_7:
		{
			//Source Stacker Set Cmd
			int src_stacker_idx = g_DMCont.m_dmVAT.GN(NDM14_VAT_TRANSFER_SOURCE_INDEX);
			
			if (eSTACKER_03 <= src_stacker_idx && src_stacker_idx <= eSTACKER_07) { //Transfer Area => VAT_IDX_TRAY_PP_CONV_3
				g_TaskStacker[src_stacker_idx].SetProcessCommand(eCommand);
			}

			//Destination Stacker Set Cmd
			int dest_stacker_idx = g_DMCont.m_dmVAT.GN(NDM14_VAT_TRANSFER_DESTINATION_INDEX);
			if (eSTACKER_03 <= dest_stacker_idx && dest_stacker_idx <= eSTACKER_07) {
				g_TaskStacker[dest_stacker_idx].SetProcessCommand(eCommand);
			}

			//Transfer Set Cmd
			g_TaskTransfer.SetProcessCommand(eCommand);
		}break;
		default:
			break;
	}
}

