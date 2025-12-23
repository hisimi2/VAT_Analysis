#include "stdafx.h"
#include "VAT_TaskAutoTrayLoadPP.h"



CVAT_TaskAutoTrayLoadPP::CVAT_TaskAutoTrayLoadPP()
{
	m_nHand = VAT_HAND_LOAD_TRAYPP;
}


CVAT_TaskAutoTrayLoadPP::~CVAT_TaskAutoTrayLoadPP()
{
}

int CVAT_TaskAutoTrayLoadPP::Initialize()
{
	m_nAutoRunMainStep = eAutoLotState_InitNeed;
	m_nAutoRunSubStep = 0;
	m_nAutoRunBackUpMainStep = eAutoLotState_InitNeed;
	m_nAutoRunBackUpSubStep = 0;

// 	m_eTrayIdx = eTrayPP_1_LD;
// 	m_pTaskTrayPP = &g_TaskTrayLoadPP;

	//SetVatMainGuiHandle(hOwner); //다른 곳에서 진행.

	//m_nHand = VAT_HAND_LOAD_TRAYPP;

	return CBaseTaskManager::Initialize();
}

void CVAT_TaskAutoTrayLoadPP::Finalize()
{
	CBaseTaskManager::Finalize();
}

int CVAT_TaskAutoTrayLoadPP::OnEvent(const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList/*=NULL*/)
{
	return ERR_NO_ERROR;
}

int CVAT_TaskAutoTrayLoadPP::OnCommandRsp(CBaseSeqCmd* pRspCmd, int nEventId, WPARAM wParam, LPARAM lParam, std::vector<CPoint>& vPocket_Picker)
{
	if (nEventId == CMD_EVT_ALARM)
	{
		if (pRspCmd->m_pSender == this)
			ChangeMainStateByRspAlarm();
	}

	return CBaseTaskManager::OnCommandRsp(pRspCmd, nEventId, wParam, lParam, vPocket_Picker);
}

void CVAT_TaskAutoTrayLoadPP::OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
}

void CVAT_TaskAutoTrayLoadPP::ThreadProc1()
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

void CVAT_TaskAutoTrayLoadPP::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = { 0, };
	if (fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf_s(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_VAT_Auto_Tray_1), Debug, "", __LINE__, NULL, tmpMsg);
}


void CVAT_TaskAutoTrayLoadPP::AutoState_InitNeed()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 1000:
	{
		//g_TaskTrayLoadPP.SetProcessCommand(base_seq_cmd::eProcCmd_Stop );
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


void CVAT_TaskAutoTrayLoadPP::AutoState_Initialize()
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
		_Base_ClearParam();

		ChangeSubState(2001);
	}break;

	case 2001:
	{
		CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
		if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
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
void CVAT_TaskAutoTrayLoadPP::AutoState_Idle()
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

void CVAT_TaskAutoTrayLoadPP::AutoState_Execute()
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
		AutoState_Execute_Vision_Auto_Teaching();
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


void CVAT_TaskAutoTrayLoadPP::AutoState_Pause()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d] [last step=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nLast_Excuting_Step);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 4000:
	{
		//g_TaskTrayLoadPP.SetProcessCommand(base_seq_cmd::eProcCmd_Pause);
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


void CVAT_TaskAutoTrayLoadPP::AutoState_NormalComplete()
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

void CVAT_TaskAutoTrayLoadPP::AutoState_AlarmStop()
{
	if (m_nAutoRunBackUpSubStep != m_nAutoRunSubStep) {
		MakeLog(_T("[MainStep=%s] [SubStep=%d] [last step=%d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nLast_Excuting_Step);
	}
	m_nAutoRunBackUpSubStep = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
	case 6000:
	{
		//g_TaskTrayLoadPP.SetProcessCommand(base_seq_cmd::eProcCmd_Pause);
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


void CVAT_TaskAutoTrayLoadPP::AutoState_Resume()
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
		//g_TaskTrayLoadPP.SetProcessCommand(base_seq_cmd::eProcCmd_Resume);
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
				g_VATCommonData.m_stVATPickerAlignInfoSeq.vMeasuredPickerPos[m_nHand].clear();
			}

			ChangeMainState(eAutoLotState_Executing, m_nLast_Excuting_Step);
		}
		else if(nModeType == VAT_MODE_TYPE_PART) {
			ChangeMainState(eAutoLotState_Executing, m_nLast_Excuting_Step);
		}

		//Retry Count 초기화
		m_nVisionRetryCnt = 0;
		
	}break;

	default:
		ChangeSubState(7000);
		break;
	}
}

void CVAT_TaskAutoTrayLoadPP::AutoState_CleanOut()
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
// 		else if (nModeType == VAT_MODE_TYPE_PICKER) {
// 
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
		int nHandStatus = g_TaskTrayLoadPP.GetSeqCmdStatus();
		if (nHandStatus == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
// 			CTrayLoadPPCmd_MovePos* pSeqCmd = new CTrayLoadPPCmd_MovePos();
// 			pSeqCmd->eStage = eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY;/*LOC_IDX_TRAY_PP_CONV_3;*/
// 			pSeqCmd->Pocket = CPoint(0, 0);
// 
// 			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
// 				ChangeSubState(8888);
// 			}

			CVatTrayLoadPPFeederCmd_MovePos* pSeqCmd = new CVatTrayLoadPPFeederCmd_MovePos(m_nSeqMode);
			pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker, 모든 Picker Safety
			pSeqCmd->m_strCmdStage = _T("All Picker Safety Position");
			_Base_GetSafetyPos(pSeqCmd->m_dCmdPos); //[Cmd Position] - X,Y,Z,XP,YP

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Safety Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(8888);
			}
		}
	}break;
	case 8888:
	{
		if (g_TaskTrayLoadPP.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
			break;

		ChangeMainState(eAutoLotState_NormalComplete);

	}break;
	default:
		ChangeSubState(8000);
		break;
	}
}


int CVAT_TaskAutoTrayLoadPP::IsAllCmdReady()
{
	//VAT Tray Load Hand 관련된 cmd 확인.
	if (g_TaskTrayLoadPP.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready ||
		g_TaskTrayFeeder.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready ||
		g_TaskLoadTbl[eLoadTbl_2].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready ||
		g_TaskLoadTbl[eLoadTbl_3].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
		return ERR_VAT_ERROR;

	return ERR_NO_ERROR;
}


BOOL CVAT_TaskAutoTrayLoadPP::IsCurSeqCmdReady()
{
	CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
	
	if (VAT_IDX_TRAY_PP_TBL_2 <= m_nCurLoc && m_nCurLoc <= VAT_IDX_TRAY_PP_TBL_3) {
		int nTblIdx = (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_2) ? eLoadTbl_2 : eLoadTbl_3;
		CBaseSeqCmd::eSeqCmdStatus table_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskLoadTbl[nTblIdx].GetSeqCmdStatus();
		if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready || table_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			return FALSE;
		}
	}
	else if(m_nCurLoc == VAT_IDX_TRAY_PP_FEEDER){
		CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
		if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready || feeder_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			return FALSE;
		}
	}

	return TRUE;
}


void CVAT_TaskAutoTrayLoadPP::SetProcessCmd(base_seq_cmd::eProcessCommand eCommand)
{
	g_TaskTrayLoadPP.SetProcessCommand(eCommand);

	switch (m_nCurLoc)
	{
	case VAT_IDX_TRAY_PP_TBL_2:
	case VAT_IDX_TRAY_PP_TBL_3:
	{
		int nTblIndex = (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_2) ? eLoadTbl_2 : eLoadTbl_3;

		bool bIsReserved = g_VatTaskSystemCtrl.get_tbl_reserved_hand(nTblIndex, m_nHand);
		if (bIsReserved) {
			g_TaskLoadTbl[nTblIndex].SetProcessCommand(eCommand);
		}
	}break;
	case VAT_IDX_TRAY_PP_FEEDER:
	{
		g_TaskStacker[eSTACKER_03].SetProcessCommand(eCommand);
		g_TaskTrayFeeder.SetProcessCommand(eCommand);
	}break;
	default:
	{
		g_TaskLoadTbl[eLoadTbl_2].SetProcessCommand(eCommand);
		g_TaskLoadTbl[eLoadTbl_3].SetProcessCommand(eCommand);
		g_TaskStacker[eSTACKER_03].SetProcessCommand(eCommand);
		g_TaskTrayFeeder.SetProcessCommand(eCommand);
	}break;
	}
}


int CVAT_TaskAutoTrayLoadPP::GET_STATUS_ID_UPPER()
{
	int nRet = -1;
	switch (m_nCurLoc)
	{
		case VAT_IDX_TRAY_PP_TBL_1:
		case VAT_IDX_TRAY_PP_TBL_2:
		case VAT_IDX_TRAY_PP_TBL_3:
		case VAT_IDX_TRAY_PP_TBL_4:
		{
			nRet = VISION_STATUS_TABLE;
		}break;
		case VAT_IDX_TRAY_PP_FEEDER:
		{
			nRet = VISION_STATUS_C_TRAY;
		}break;
		default:
			break;
	}

	return nRet;
}


int CVAT_TaskAutoTrayLoadPP::GET_VAT_PART_IDX()
{
	return (m_nCurLoc == VAT_IDX_TRAY_PP_FEEDER) ? VAT_PART_FEEDER : (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_2) ? VAT_PART_TABLE_2 : VAT_PART_TABLE_3;
}


int CVAT_TaskAutoTrayLoadPP::GET_VAT_PART_POS()
{
	return (m_nCurLoc == VAT_IDX_TRAY_PP_FEEDER) ? VAT_FEEDER_C_TRAY_JIG : VAT_TBL_LOAD_SIDE;
}


//===================================================================================================[Z Auto Focus]
void CVAT_TaskAutoTrayLoadPP::AutoState_Execute_Z_Auto_Focus()
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
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//READY -> ONGOING
			_Base_SetMeasureHand_Status(VAT_STATUS_MEASURE_ONGOING);

			CVatTrayLoadPPFeederCmd_MovePos* pSeqCmd = new CVatTrayLoadPPFeederCmd_MovePos(m_nSeqMode);
			CPoint cStdPicker = _Base_GetStdPicker();
			pSeqCmd->m_vCmdPicker.push_back(cStdPicker);
			pSeqCmd->m_strCmdStage = _T("Plate Jig");
			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos, eZFocus_PlateJigTargetPos); //[Cmd Position] - X,Y,Z,XP,YP

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Start Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_ZFocus_LowerCam_AF_and_UpperCam_Pre_AF);
			}

		}break;
		case eExcutingStep_ZFocus_LowerCam_AF_and_UpperCam_Pre_AF:
		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//AF 동시 진행.
			//Lower Cam Auto Focus
			//Upper CAm Pre Auto Focus 
			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			pSeqCmd->m_vDataId.push_back(VISION_REQ_DATAID_VAT_LD_LOWER);
			pSeqCmd->m_vDataId.push_back(VISION_REQ_DATAID_VAT_LD_UPPER);
			pSeqCmd->m_nStatus = VISION_STATUS_AUTO_FOCUS;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Lower Cam AF & Upper Cam AF]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				//GUI Notify(CAM AUTO FOCUS)
				CVAT_MeasureStatus& GuiNotifier = CVAT_MeasureStatus::GetInstance();	
				GuiNotifier.UpdateVisionMeasureCnt(VAT_CAM_LD_TRAY_PP_LOWER, VAT_VISION_MEASURE_AUTO_FOCUS);
				GuiNotifier.UpdateVisionMeasureCnt(VAT_CAM_LD_TRAY_PP_UPPER, VAT_VISION_MEASURE_AUTO_FOCUS);

				//해당 Cmd Tray Load PP에 Log 생성 되는지 확인
				ChangeSubState(eExcutingStep_ZFocus_PlateJig_LowerTarget_Measure);
			}
		}break;
		case eExcutingStep_ZFocus_PlateJig_LowerTarget_Measure: //Plate Jig Lower Target 측정.
		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			pSeqCmd->m_vDataId.push_back(VISION_REQ_DATAID_VAT_LD_LOWER);
			pSeqCmd->m_nStatus = VISION_STATUS_PLATE_JIG;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Plate Jig Lower Target Find]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				//GUI Notify
				CVAT_MeasureStatus::GetInstance().UpdateVisionMeasureCnt(VAT_CAM_LD_TRAY_PP_LOWER, VAT_VISION_MEASURE_SINGLE);

				ChangeSubState(eExcutingStep_ZFocus_PlateJig_UpperTarget_Move_Pre_Pos);
			}
		}break;
		case eExcutingStep_ZFocus_PlateJig_UpperTarget_Move_Pre_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
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

			CVatTrayLoadPPFeederCmd_MovePos* pSeqCmd = new CVatTrayLoadPPFeederCmd_MovePos(m_nSeqMode);
			pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker, Pre Position은 모든 Picker Safety
			pSeqCmd->m_strCmdStage = _T("Plate Jig Pre Position");
			memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPos, sizeof(dCmdPos));

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Pre Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_ZFocus_PlateJig_UpperTarget_Move_Target_Pos);
			}

		}break;
 		case eExcutingStep_ZFocus_PlateJig_UpperTarget_Move_Target_Pos:
 		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CVatTrayLoadPPFeederCmd_MovePos* pSeqCmd = new CVatTrayLoadPPFeederCmd_MovePos(m_nSeqMode);
			CPoint cStdPicker = _Base_GetStdPicker();
			pSeqCmd->m_vCmdPicker.push_back(cStdPicker); //Standard Picker
			pSeqCmd->m_strCmdStage = _T("Plate Jig Target Position");
			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos, eZFocus_PlateJigTargetPos); //[Cmd Position] - X,Y,Z,XP,YP

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Target Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_ZFocus_PlateJig_UpperTarget_Measure);
			}
 		}break;
		case eExcutingStep_ZFocus_PlateJig_UpperTarget_Measure: //Plate Jig Upper Target 측정.
		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//이전 Position 저장.
			m_dPrevPosX = g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_TargetPos[m_nHand][VAT_X];
			m_dPrevPosY = g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_TargetPos[m_nHand][VAT_Y];

			//Plate Jig 상단 Target 측정.
			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			pSeqCmd->m_vDataId.push_back(VISION_REQ_DATAID_VAT_LD_UPPER);
			pSeqCmd->m_nStatus = VISION_STATUS_PLATE_JIG;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Measure Plate Jig Upper Target]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				//GUI Notify
				CVAT_MeasureStatus::GetInstance().UpdateVisionMeasureCnt(VAT_CAM_LD_TRAY_PP_UPPER, m_nVisionRetryCnt + 1);

				ChangeSubState(eExcutingStep_ZFocus_PlateJig_UpperTarget_Measure_Chk);
			}
		}break;
		case eExcutingStep_ZFocus_PlateJig_UpperTarget_Measure_Chk:
		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}
		
			if (m_dPrevPosX != g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_TargetPos[m_nHand][VAT_X] ||
				m_dPrevPosY != g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_TargetPos[m_nHand][VAT_Y])
			{
				m_nVisionRetryCnt++;

				if (m_nVisionRetryCnt >= DEF_VAT_VISION_RETRY_CNT_MAX) {
					_Base_AlarmReport_VAT(ERR_VAT_VISION_RETRY_CNT_OVER);
				}

				MakeLog(_T("[MainStep=%s] [SubStep=%d] [Plate Jig Upper Target Measure Retry][Cnt : %d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nVisionRetryCnt);

				

				//Center Find까지 재측정.
				ChangeSubState(eExcutingStep_ZFocus_PlateJig_UpperTarget_Move_Pre_Pos);
			}
			else
			{
				//Center Find.
				MakeLog(_T("[MainStep=%s] [SubStep=%d] [Plate Jig Upper Target Center Find]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

				ChangeSubState(eExcutingStep_ZFocus_UpperCam_AF);
			}
		}break;
		case eExcutingStep_ZFocus_UpperCam_AF:
		{
			//Upper Cam Auto Focus
			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			pSeqCmd->m_vDataId.push_back(VISION_REQ_DATAID_VAT_LD_UPPER);
			pSeqCmd->m_nStatus = VISION_STATUS_AUTO_FOCUS;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Upper Cam Final Auto Focus]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				CVAT_MeasureStatus::GetInstance().UpdateVisionMeasureCnt(VAT_CAM_LD_TRAY_PP_UPPER, VAT_VISION_MEASURE_AUTO_FOCUS);

				//해당 Cmd Tray Load PP에 Log 생성 되는지 확인
				ChangeSubState(eExcutingStep_ZFocus_Std_Picker_Move_Target_Pos);
			}
		}break;
		case eExcutingStep_ZFocus_Std_Picker_Move_Target_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CVatTrayLoadPPFeederCmd_MovePos* pSeqCmd = new CVatTrayLoadPPFeederCmd_MovePos(m_nSeqMode);
			CPoint cStdPicker = _Base_GetStdPicker();
			pSeqCmd->m_vCmdPicker.push_back(cStdPicker); //Standard Picker
			pSeqCmd->m_strCmdStage = _T("Plate Jig Vacuum Position");
			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos, eZFocus_PlateJigVacPos); //[Cmd Position] - X,Y,Z,XP,YP

			//Z축 과도하게 누르는 것 방지.
			//_Base_CheckandCreateZHeightCalPrePos(pSeqCmd->m_dCmdPos[VAT_Z]);

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Vacuum Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_ZFocus_Find_PlateJig_Z_Fast);
			}
		}break;
		case eExcutingStep_ZFocus_Find_PlateJig_Z_Fast: //기준 Picker Find
		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			g_VATCommonData.ClearFirstPickOffset(VAT_HAND_LOAD_TRAYPP);

			CVatTrayLoadPPCmd_PickerHeightCal* pSeqCmd = new CVatTrayLoadPPCmd_PickerHeightCal();
			pSeqCmd->m_nSpeed = eZSpeed_Fast;
			pSeqCmd->move_offset_um = 0.0;
			pSeqCmd->m_nStage = VAT_NON_PART_PLATAE_JIG;//m_nCurLoc;
			pSeqCmd->m_dTargetPosZ = g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_VacuumPos[VAT_HAND_LOAD_TRAYPP][VAT_Z];

			//기준 Picker
			CPoint cStdPicker = _Base_GetStdPicker();
			pSeqCmd->m_vCmdPicker.push_back(cStdPicker);

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Height Cal] [Speed=Fast, move_offset=0.0, Loc=plate jig]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			_Base_SetZAutoTeachingSpd(pSeqCmd->m_nSpeed);
			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_ZFocus_Find_PlateJig_Z_Slow);
			}
		}break;
		case eExcutingStep_ZFocus_Find_PlateJig_Z_Slow:
		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CVatTrayLoadPPCmd_PickerHeightCal* pSeqCmd = new CVatTrayLoadPPCmd_PickerHeightCal();
			pSeqCmd->m_nSpeed = eZSpeed_Slow;
			pSeqCmd->move_offset_um = g_VATCommonData.GetFirstPickOffset(VAT_HAND_LOAD_TRAYPP);
			pSeqCmd->m_nStage = VAT_NON_PART_PLATAE_JIG;
			pSeqCmd->m_dTargetPosZ = g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_VacuumPos[VAT_HAND_LOAD_TRAYPP][VAT_Z];

			//기준 Picker
			CPoint cStdPicker = _Base_GetStdPicker();
			pSeqCmd->m_vCmdPicker.push_back(cStdPicker);

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Height Cal] [Speed=Slow, move_offset=0.0, Loc=plate jig]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
			_Base_SetZAutoTeachingSpd(pSeqCmd->m_nSpeed);
			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_ZFocus_Find_PlateJig_Z_End);
			}
		}break;
		case eExcutingStep_ZFocus_Find_PlateJig_Z_End:
		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
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
void CVAT_TaskAutoTrayLoadPP::AutoState_Execute_Cam_Center_Calibration()
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
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
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

			CVatTrayLoadPPFeederCmd_MovePos* pSeqCmd = new CVatTrayLoadPPFeederCmd_MovePos(m_nSeqMode);
			pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker, Pre Position은 모든 Picker Safety
			pSeqCmd->m_strCmdStage = _T("STD Picker Pre Position");
			memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPos, sizeof(dCmdPos));

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Pre Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_CamCenterCal_Move_Target_Pos);
			}
		}break;
		case eExcutingStep_CamCenterCal_Move_Target_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CVatTrayLoadPPFeederCmd_MovePos* pSeqCmd = new CVatTrayLoadPPFeederCmd_MovePos(m_nSeqMode);
			CPoint cStdPicker = _Base_GetStdPicker();
			pSeqCmd->m_vCmdPicker.push_back(cStdPicker);
			pSeqCmd->m_strCmdStage = _T("STD Picker Target Position");
			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos, eCamCenCal_BotCamCenterPos); //[Cmd Position] - X,Y,Z,XP,YP

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Target Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_CamCenterCal_Std_Picker_Measure);
			}
		}break;
		case eExcutingStep_CamCenterCal_Std_Picker_Measure:
		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//이전 Position 저장.
			m_dPrevPosX = g_VATCommonData.m_stVATCamCenterCalInfoSeq.dBotCameraPosition[m_nHand][VAT_X];
			m_dPrevPosY = g_VATCommonData.m_stVATCamCenterCalInfoSeq.dBotCameraPosition[m_nHand][VAT_Y];

			//STD Picker Hole 측정.
			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			pSeqCmd->m_vDataId.push_back(VISION_REQ_DATAID_VAT_LD_LOWER);
			pSeqCmd->m_nStatus = VISION_STATUS_PICKER;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Measure STD Picker]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				//GUI Notify
				CVAT_MeasureStatus::GetInstance().UpdateVisionMeasureCnt(VAT_CAM_LD_TRAY_PP_LOWER, m_nVisionRetryCnt + 1);

				ChangeSubState(eExcutingStep_CamCenterCal_Std_Picker_Measure_Chk);
			}
		}break;
		case eExcutingStep_CamCenterCal_Std_Picker_Measure_Chk:
		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
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
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CVatTrayLoadPPCmd_MovePos* pSeqCmd = new CVatTrayLoadPPCmd_MovePos(m_nSeqMode);
			pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker, 모든 Picker Safety
			pSeqCmd->m_strCmdStage = _T("All Picker Safety Position");
			pSeqCmd->m_bZOnlyMove = TRUE;
			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos); //[Cmd Position] - X,Y,Z,XP,YP
			
 			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Safety Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
 
 			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
 				ChangeSubState(eExcutingStep_CamCenterCal_End);
 			}
		}break;
		case eExcutingStep_CamCenterCal_End:
		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
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
void CVAT_TaskAutoTrayLoadPP::AutoState_Execute_Picker_Align()
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
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
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
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CString strCmdStage = _T("");
			strCmdStage.Format( "[Picker Idx=%d] Setting Position", m_stCurPicker.nPickerIdx);

			CVatTrayLoadPPFeederCmd_MovePos* pSeqCmd = new CVatTrayLoadPPFeederCmd_MovePos(m_nSeqMode);
			CPoint cCurPicker = _Base_ConvertPickerIdxToPoint(m_stCurPicker.nPickerIdx);
			pSeqCmd->m_vCmdPicker.push_back(cCurPicker);
			pSeqCmd->m_strCmdStage = strCmdStage;
			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos, ePickerAlign_BeforePickerSetPos);

			//Setting Positoin Z축은 -1mm ~ +1mm(2mm)구간에서 찾음.
			pSeqCmd->m_dCmdPos[VAT_Z] -= 1000.0;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] %s Move"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, strCmdStage);

			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_PickerAlign_Picker_Setting_Pos_Measure);
			}
		}break;
		case eExcutingStep_PickerAlign_Picker_Setting_Pos_Measure:
		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//Current Picker Hole 측정.
			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			pSeqCmd->m_vDataId.push_back(VISION_REQ_DATAID_VAT_LD_LOWER);
			pSeqCmd->m_nStatus = VISION_STATUS_FIND_Z_MEASURE_PICKER;
			pSeqCmd->m_nMeasurePkrIdx = m_stCurPicker.nPickerIdx;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Picker Z Focus Pos] [Picker Idx = %d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_stCurPicker.nPickerIdx);

			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				//GUI Notify
				//CVAT_MeasureStatus::GetInstance().UpdateVisionMeasureCnt(VAT_CAM_LD_TRAY_PP_LOWER, m_nVisionRetryCnt + 1);

				//GUI Notify
				CVAT_MeasureStatus::GetInstance().UpdateVisionMeasureCnt(VAT_CAM_LD_TRAY_PP_LOWER, VAT_VISION_MEASURE_AUTO_FOCUS);

				ChangeSubState(eExcutingStep_PickerAlign_Picker_Setting_Pos_Measure_Chk);
			}
		}break;
		case eExcutingStep_PickerAlign_Picker_Setting_Pos_Measure_Chk:
		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
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
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
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

			CVatTrayLoadPPCmd_MovePos* pSeqCmd = new CVatTrayLoadPPCmd_MovePos(m_nSeqMode);
			CPoint cCurPicker = _Base_ConvertPickerIdxToPoint(m_stCurPicker.nPickerIdx);
			pSeqCmd->m_vCmdPicker.push_back(cCurPicker);
			pSeqCmd->m_strCmdStage = strCmdStage;
			pSeqCmd->m_dCmdPos[VAT_Z] = dLastPos + VAT_PICKER_ALIGN_Z_FIND_MOVE_OFFSET; //0.1mm
			pSeqCmd->m_bZOnlyMove = TRUE;
		
			MakeLog(_T("[MainStep=%s] [SubStep=%d] %s Move"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, strCmdStage);

			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_PickerAlign_Picker_Setting_Pos_Measure);
			}
		}break;
		case eExcutingStep_PickerAlign_Picker_Z_Focus_Pos_Find:
		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//Current Picker Z Find Position.
			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			pSeqCmd->m_vDataId.push_back(VISION_REQ_DATAID_VAT_LD_LOWER);
			pSeqCmd->m_nStatus = VISION_STATUS_FIND_Z_RESULT_PICKER;
			pSeqCmd->m_nMeasurePkrIdx = m_stCurPicker.nPickerIdx;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Picker Z Find Focus Pos] [Picker Idx = %d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_stCurPicker.nPickerIdx);

			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_PickerAlign_Move_Pre_Pos);
			}
		}break;
		case eExcutingStep_PickerAlign_Move_Pre_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
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

			CVatTrayLoadPPFeederCmd_MovePos* pSeqCmd = new CVatTrayLoadPPFeederCmd_MovePos(m_nSeqMode);
			pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker, Pre Position은 모든 Picker Safety
			pSeqCmd->m_strCmdStage = strCmdStage;
			memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPos, sizeof(dCmdPos));

			MakeLog(_T("[MainStep=%s] [SubStep=%d] %s Move"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, strCmdStage);

			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_PickerAlign_Move_Target_Pos);
			}
		}break;
		case eExcutingStep_PickerAlign_Move_Target_Pos:
		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CString strCmdStage = _T("");
			strCmdStage.Format(
				"[Picker Type=%s][Picker Idx=%d] Target Position",
				(m_stCurPicker.nPitchType == VAT_PITCH_TYPE_WIDE) ? _T("WIDE") : _T("NARROW"),
				m_stCurPicker.nPickerIdx
			);

			CVatTrayLoadPPFeederCmd_MovePos* pSeqCmd = new CVatTrayLoadPPFeederCmd_MovePos(m_nSeqMode);
			CPoint cCurPicker = _Base_ConvertPickerIdxToPoint(m_stCurPicker.nPickerIdx);
			pSeqCmd->m_vCmdPicker.push_back(cCurPicker);
			pSeqCmd->m_strCmdStage = strCmdStage;
			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos, ePickerAlign_AfterPickerSetPos);

			MakeLog(_T("[MainStep=%s] [SubStep=%d] %s Move"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, strCmdStage);

			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_PickerAlign_Move_Cur_Picker_Measure);
			}
		}break;
		case eExcutingStep_PickerAlign_Move_Cur_Picker_Measure:
		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			//이전 Position 저장.
			m_dPrevPosX = g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerPos[m_nHand][m_stCurPicker.nPitchType][m_stCurPicker.nPickerIdx].first;
			m_dPrevPosY = g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerPos[m_nHand][m_stCurPicker.nPitchType][m_stCurPicker.nPickerIdx].second;

			//Current Picker Hole 측정.
			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			pSeqCmd->m_vDataId.push_back(VISION_REQ_DATAID_VAT_LD_LOWER);
			pSeqCmd->m_nStatus = VISION_STATUS_PICKER;
			pSeqCmd->m_nMeasurePkrIdx = m_stCurPicker.nPickerIdx;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Picker Hole] [Picker Type = %d] [Picker Idx = %d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_stCurPicker.nPitchType, m_stCurPicker.nPickerIdx);

			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				//GUI Notify
				CVAT_MeasureStatus::GetInstance().UpdateVisionMeasureCnt(VAT_CAM_LD_TRAY_PP_LOWER, m_nVisionRetryCnt + 1);

				ChangeSubState(eExcutingStep_PickerAlign_Move_Cur_Picker_Measure_Chk);
			}
		}break;
		case eExcutingStep_PickerAlign_Move_Cur_Picker_Measure_Chk:
		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
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
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CVatTrayLoadPPFeederCmd_MovePos* pSeqCmd = new CVatTrayLoadPPFeederCmd_MovePos(m_nSeqMode);
			pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker, 모든 Picker Safety
			pSeqCmd->m_strCmdStage = _T("All Picker Safety Position");
			_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos); //[Cmd Position] - X,Y,Z,XP,YP

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Safety Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_PickerAlign_End);
			}
		}break;
		case eExcutingStep_PickerAlign_End:
		{
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
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
// 	int nVirtual_CurPicker = 0;
// 	int nVirtual_LastPicker = 0;
// 
// 	//2. Picker 측정 완료 시, Case Step Change처럼 다음 Picker로 변경하여 이전 Percentage 100%로 변경.
// 	if (m_stCurPicker.nPickerIdx != -1)
// 	{
// 		int nCurPickerStatus = _Base_GetMeasurePicker_Status(m_stCurPicker.nPitchType, m_stCurPicker.nPickerIdx);
// 		if (nCurPickerStatus == VAT_STATUS_MEASURE_COMPLETE) {
// 			ST_VAT_PICKER stCurPicker = _Base_GetMeasurePicker_Each(VAT_STATUS_MEASURE_READY);
// 			nVirtual_CurPicker = stCurPicker.nPickerIdx;
// 		}
// 		else if(nCurPickerStatus == VAT_STATUS_MEASURE_ONGOING) {
// 			nVirtual_LastPicker = m_stCurPicker.nPickerIdx;
// 		}
// 	}
// 
//  	_Base_UpdateProgressRate(nVirtual_CurPicker, nVirtual_LastPicker, nVirtual_MaxPickerType);
}

//===================================================================================================[Vision Auto Teaching]
void CVAT_TaskAutoTrayLoadPP::AutoState_Execute_Vision_Auto_Teaching()
{
	if (m_nAutoRunSubStep > eExcutingStep_VAT_End) {
		m_nAutoRunSubStep = m_nLast_Excuting_Step;
	}

//  Progress Bar 없음.
// 	if (m_nLast_Excuting_Step != m_nAutoRunSubStep) {
// 		m_tcAutoSchedule.StartTimer();
// 	}

	m_nLast_Excuting_Step = m_nAutoRunSubStep;

	switch (m_nAutoRunSubStep)
	{
		case eExcutingStep_VAT_Start:
		{
			//선택 Part 없을 경우, CLEAN OUT 진행.
			if (_Base_IsAnyPartSelected() != TRUE) {
				ChangeMainState(eAutoLotState_CleanOut);
				break;
			}

			MakeLog(_T("[Mode=%s] [MainStep=%s] [SubStep=%d]  [Start]"), GetExcuteModeStr(m_nSeqMode), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
			ChangeSubState(eExcutingStep_ZTeaching_CheckCondition);
		}break;
		case eExcutingStep_VAT_Check_PlateJig:
		{
			

			ChangeSubState(eExcutingStep_VAT_Check_C_Tray_Jig);
		}break;
		case eExcutingStep_VAT_Check_C_Tray_Jig:
		{
			//Feeder Select 되었다면,
			//Feeder, Conveyor Pick, Conveyor Place, Stacker 하단에 있는지 확인 진행.

			ChangeSubState(eExcutingStep_VAT_Select_Measure_Part);
		}break;
		case eExcutingStep_VAT_Select_Measure_Part:
		{
			//해당 Case 넘어오면, 선택된 Part가 있는 것.
			CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
			if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			m_nCurLoc = _Base_GetMeasurePart_Selected(VAT_STATUS_MEASURE_READY);
			if (m_nCurLoc != ERR_VAT_ERROR) 
			{
				MakeLog(_T("[MainStep=%s] [SubStep=%d][Select Part=%s]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, g_TaskTrayLoadPP.GetLocName((eLocIdxTrayPP)m_nCurLoc));
				if (m_nCurLoc == VAT_IDX_TRAY_PP_FEEDER) {
					ChangeSubState(eExcutingStep_VAT_Feeder_Measure_Start);
				}
				else { //Table#2, Table#3
					int nTableIdx = (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_2) ? eLoadTbl_2 : eLoadTbl_3;
					bool bIsPossible = g_VatTaskSystemCtrl.reserve_tbl_work_hand(nTableIdx, m_nHand);
					if (!bIsPossible) {
						//[Status] READY -> WAIT
						_Base_SetMeasurePart_Status(m_nCurLoc, VAT_STATUS_MEASURE_WAIT);
						break;
					}
					
					ChangeSubState(eExcutingStep_VAT_Table_Measure_Start);
				}

				//[Status] READY -> ONGOING
				_Base_SetMeasurePart_Status(m_nCurLoc, VAT_STATUS_MEASURE_ONGOING);
			}
			else
			{
				//대기 상태 Part 확인.
				//Part Measure All Complete 확인.
			}

		}break;
		//========================================================= Feeder Measure Start
		case eExcutingStep_VAT_Feeder_Measure_Start:
		{
			MakeLog(_T("[MainStep=%s] [SubStep=%d][Feeder Measure Start]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			ChangeSubState(eExcutingStep_VAT_Move_Stacker_Down);
		}break;
// 		case eExcutingStep_VAT_Move_Feeder_Pick_Pre_Pos:
// 		{
// 			if (IsCurSeqCmdReady())
// 				break;
// 			
// 			//[Target Position] - X, Y, Z, XP, YP
// 			double dCmdPos[VAT_MAX_AXIS] = { 0, };
// 			//_Base_GetSeqTargetPos_Unit(dCmdPos, eVAT_Feeder_PickPos);
// 			
// 			//[Make Pre Position] - X, Y
// 			dCmdPos[VAT_X] -= VAT_PRE_POS_100mm_OFFSET;
// 			dCmdPos[VAT_Y] -= VAT_PRE_POS_100mm_OFFSET;
// 
// 			//[Target Limit Pos Check]
// 			_Base_CheckandCreateLimitPos(dCmdPos);
// 
// 			CVatTrayLoadPPFeederCmd_MovePos* pSeqCmd = new CVatTrayLoadPPFeederCmd_MovePos(m_nSeqMode);
// 			pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker, Pre Position은 모든 Picker Safety
// 			pSeqCmd->m_strCmdStage = _T("Feeder Pick Pre Position");
// 			memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPos, sizeof(dCmdPos));
// 
// 			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Pre Move][Feeder Pick Position]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
// 
// 			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
// 				ChangeSubState(eExcutingStep_VAT_Move_Feeder_Pick_Target_Pos);
// 			}
// 		}break;
// 		case eExcutingStep_VAT_Move_Feeder_Pick_Target_Pos:
// 		{
// 			if (IsCurSeqCmdReady())
// 				break;
// 
// 			CVatTrayLoadPPFeederCmd_MovePos* pSeqCmd = new CVatTrayLoadPPFeederCmd_MovePos(m_nSeqMode);
// 			CPoint cStdPicker = _Base_GetStdPicker();
// 			pSeqCmd->m_vCmdPicker.push_back(cStdPicker); //Standard Picker
// 			pSeqCmd->m_strCmdStage = _T("Feeder Pick Position");
// 			//_Base_GetSeqTargetPos_Unit(pSeqCmd->m_dCmdPos, eVAT_Feeder_PickPos); //[Cmd Position] - X,Y,Z,XP,YP
// 
// 			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Move][Feeder Pick Position]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
// 
// 			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
// 				ChangeSubState(eExcutingStep_VAT_Feeder_Pick_Target_Measure);
// 			}
// 		}break;
// 		case eExcutingStep_VAT_Feeder_Pick_Target_Measure:
// 		{
// 			if (IsCurSeqCmdReady())
// 				break;
// 
// 			//이전 Position 저장.
// 			m_dPrevPosX = g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_TargetPos[m_nHand][VAT_X];
// 			m_dPrevPosY = g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_TargetPos[m_nHand][VAT_Y];
// 
// 			//Plate Jig 상단 Feeder Target 측정.
// 			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
// 			pSeqCmd->m_vDataId.push_back(VISION_REQ_DATAID_VAT_LD_UPPER);
// 			pSeqCmd->m_nStatus = VISION_STATUS_PLATE_JIG;
// 
// 			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure] [Plate Jig Feeder Target]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
// 
// 			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
// 				//GUI Notify
// 				//CVAT_MeasureStatus::GetInstance().UpdateVisionMeasureCnt(VAT_CAM_LD_TRAY_PP_UPPER, m_nVisionRetryCnt + 1);
// 
// 				ChangeSubState(eExcutingStep_VAT_Feeder_Pick_Target_Measure_Chk);
// 			}
// 		}break;
// 		case eExcutingStep_VAT_Feeder_Pick_Target_Measure_Chk:
// 		{
// 			if (IsCurSeqCmdReady())
// 				break;
// 
// 			if (m_dPrevPosX != g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_TargetPos[m_nHand][VAT_X] ||
// 				m_dPrevPosY != g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_TargetPos[m_nHand][VAT_Y])
// 			{
// 				m_nVisionRetryCnt++;
// 
// 				if (m_nVisionRetryCnt >= DEF_VAT_VISION_RETRY_CNT_MAX) {
// 					_Base_AlarmReport_VAT(ERR_VAT_VISION_RETRY_CNT_OVER);
// 				}
// 
// 				MakeLog(_T("[MainStep=%s] [SubStep=%d] [Plate Jig Feeder Target Measure Retry][Cnt : %d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nVisionRetryCnt);
// 
// 				//Target Center Find까지 재측정.
// 				ChangeSubState(eExcutingStep_VAT_Move_Feeder_Pick_Pre_Pos);
// 			}
// 			else
// 			{
// 				//Center Find
// 				MakeLog(_T("[MainStep=%s] [SubStep=%d] [Plate Jig Feeder Target Center Find]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
// 
// 				//Hand Only는 해당 Case로 들어오지 않음.
// 				int nMeasureType = _Base_GetVATMeasureType(VAT_HAND_LOAD_TRAYPP);
// 				if (nMeasureType == VAT_MEASURE_TYPE_BOTH) {
// 					ChangeSubState(eExcutingStep_VAT_Move_Stacker_Down);
// 				}
// 				else { //(VAT_MEASURE_TYPE_UNIT_ONLY) 
// 					ChangeSubState(eExcutingStep_VAT_End);
// 				}
// 			}
// 		}break;
		case eExcutingStep_VAT_Move_Stacker_Down:
		{
			CBaseSeqCmd::eSeqCmdStatus stacker3_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskStacker[eSTACKER_03].GetSeqCmdStatus();
			if (stacker3_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Stacker #3 Down]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			int stacker_front_jig_not_exist = g_TaskStacker[eSTACKER_03].ChkExistTray_FrontPos_Sen(DEF_NOT_EXIST);
			if (stacker_front_jig_not_exist == ERR_NO_ERROR) {
				CStackerCmd_TrayDn* pSeqCmd = new CStackerCmd_TrayDn();
				if (SendCommand(g_TaskStacker[eSTACKER_03], pSeqCmd) == eSeqCmdAck_OK) {
					ChangeSubState(eExcutingStep_VAT_Move_Stacker_Load);
					break;
				}
			}
			else {
				_Base_AlarmReport_VAT(ERR_VAT_TRAY_PP_STACKER_FRONT_POS_JIG_EXIST, _T("Stacker 3"));
			}
		}break;
		case eExcutingStep_VAT_Move_Stacker_Load:
		{
			CBaseSeqCmd::eSeqCmdStatus stacker3_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskStacker[eSTACKER_03].GetSeqCmdStatus();
			if (stacker3_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			int stacker_rear_jig_not_exist = g_TaskStacker[eSTACKER_03].ChkExistTray_RearPos_Sen(DEF_NOT_EXIST);
			int stacker_front_jig_exist = g_TaskStacker[eSTACKER_03].ChkExistTray_FrontPos_Sen(DEF_EXIST);
			if (stacker_rear_jig_not_exist == ERR_NO_ERROR && stacker_front_jig_exist == ERR_NO_ERROR) {
				MakeLog(_T("[MainStep=%s] [SubStep=%d] [Stacker #3 Load]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
				CStackerCmd_Load* pSeqCmd = new CStackerCmd_Load(true);
				if (SendCommand(g_TaskStacker[eSTACKER_03], pSeqCmd) == eSeqCmdAck_OK) {
					ChangeSubState(eExcutingStep_VAT_Move_Feeder_Pick_C_Tray_Jig);
				}
			}
			else {
				//Rear, Front Position에 대한 알람.
				_Base_AlarmReport_VAT(((stacker_rear_jig_not_exist != ERR_NO_ERROR) ? ERR_VAT_TRAY_PP_STACKER_REAR_POS_JIG_EXIST : ERR_VAT_TRAY_PP_STACKER_FRONT_POS_JIG_NOT_EXIST), _T("Stacker 3"));
			}
		}break;
		case eExcutingStep_VAT_Move_Feeder_Pick_C_Tray_Jig:
		{
			if (g_TaskStacker[eSTACKER_03].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready ||
				g_TaskTrayFeeder.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			int stacker_rear_jig_exist = g_TaskStacker[eSTACKER_03].ChkExistTray_RearPos_Sen(DEF_EXIST);
			int feeder_jig_no_exist = g_TaskTrayFeeder.ChkTrayExistSensor(eNot_Exist);
			bool conveyor_moving = g_TaskStacker[eSTACKER_03].IsConveyorMoving();
			if (stacker_rear_jig_exist == ERR_NO_ERROR && feeder_jig_no_exist == ERR_NO_ERROR && conveyor_moving == false) {
				CTrayFeederCmd_MovePickPlace* pSeqCmd = new CTrayFeederCmd_MovePickPlace();
				pSeqCmd->m_nCmdStage = eLocIdxFeeder::LOC_IDX_FEEDER_TRAY_PICK_POS;
				pSeqCmd->m_nCmdPickPlace = ePick;

				MakeLog(_T("[MainStep=%s] [SubStep=%d] [Feeder Pick]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

				if (SendCommand(g_TaskTrayFeeder, pSeqCmd) == eSeqCmdAck_OK) {	
					ChangeSubState(eExcutingStep_VAT_Std_Picker_Move_Target_Pos);
					break;
				}
			}
			else {
				int nErr = -1;
				if (stacker_rear_jig_exist != ERR_NO_ERROR)		nErr = ERR_VAT_TRAY_PP_STACKER_REAR_POS_JIG_NOT_EXIST;
				else if (feeder_jig_no_exist != ERR_NO_ERROR)	nErr = ERR_VAT_TRAY_PP_FEEDER_JIG_EXIST;
				else                                            nErr = ERR_VAT_TRAY_PP_CONVEYOR_IS_MOVING_BEFORE_FEEDER_PICK;

				_Base_AlarmReport_VAT(nErr, _T("Stacker 3"));
				break;
			}
		}break;
		//C-Tray Jig Place Seq
		case eExcutingStep_VAT_Feeder_Place_C_Tray_Jig:
		{
			CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
			if (feeder_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready)
				break;

			CBaseSeqCmd::eSeqCmdStatus transfer_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTransfer.GetSeqCmdStatus();
			if (transfer_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready)
				break;

			//Transfer Safety 위치 확인.
			bool bSafetyTransfer = g_TaskTransfer.is_safety_state();
			if (bSafetyTransfer != TRUE) {
				_Base_AlarmReport_VAT(ERR_TRNASFER_IS_NOT_SAFETY_STATE);
				break;
			}

			int stacker_rear_jig_not_exist = g_TaskStacker[eSTACKER_03].ChkExistTray_RearPos_Sen(DEF_NOT_EXIST);
			int stacker_front_jig_not_exist = g_TaskStacker[eSTACKER_03].ChkExistTray_FrontPos_Sen(DEF_NOT_EXIST);
			int transfer_area_jig_not_exist = g_TaskTrayFeeder.ChkTrayExist_TransferArea_Sen(eNot_Exist);
			bool conveyor_moving = g_TaskStacker[eSTACKER_03].IsConveyorMoving();

			if (stacker_rear_jig_not_exist == ERR_NO_ERROR &&
				stacker_front_jig_not_exist == ERR_NO_ERROR &&
				transfer_area_jig_not_exist == ERR_NO_ERROR &&
				conveyor_moving == false) {
				CTrayFeederCmd_MovePickPlace* pSeqCmd = new CTrayFeederCmd_MovePickPlace();
				pSeqCmd->m_nCmdStage = eLocIdxFeeder::LOC_IDX_FEEDER_TRAY_PLACE_POS;
				pSeqCmd->m_nCmdPickPlace = ePlace;

				if (SendCommand(g_TaskTrayFeeder, pSeqCmd) == eSeqCmdAck_OK) {
					MakeLog(_T("[MainStep=%s] [SubStep=%d] [Feeder Place]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
					ChangeSubState(eExcutingStep_VAT_Feeder_Move_Safety);
					break;
				}
			}
		}break;
		case eExcutingStep_VAT_Feeder_Move_Safety:
		{
			CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
			if (feeder_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready)
				break;

			CTrayFeederCmd_MovePos* pSeqCmd = new CTrayFeederCmd_MovePos();
			pSeqCmd->m_nCmdStage = eLocIdxFeeder::LOC_IDX_FEEDER_SAFETY;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Feeder Move Safety POS]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayFeeder, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_VAT_Check_C_Tray_Jig_Measure_Remain);
				break;
			}
		}break;
		case eExcutingStep_VAT_Check_C_Tray_Jig_Measure_Remain:
		{
			CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
			if (feeder_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready)
				break;

			//Unload Tray PP 측정 남았는지 확인.
			BOOL bIsRemain = FALSE;
			for (int nPart = VAT_IDX_TRAY_PP_CONV_4; nPart < VAT_IDX_TRAY_PP_CONV_7; nPart++) {
				UNITCOMPONENT cStackerUnit = g_VatTaskAutoTrayUnloadPP._Base_GetMeasurePart_Each(nPart);
				if (cStackerUnit.first == nPart) { //Selected 된 것.
					bIsRemain = TRUE;
					MakeLog(_T("[MainStep=%s] [SubStep=%d] [Unload Tray Pp Use C-Tray Jig]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
					ChangeSubState(eExcutingStep_VAT_Check_All_Part_Complete);
					break;
				}
			}

			if(bIsRemain)
				break;

			ChangeSubState(eExcutingStep_VAT_Move_Stacker_Unload);
		}break;
		case eExcutingStep_VAT_Move_Stacker_Unload:
		{
			CBaseSeqCmd::eSeqCmdStatus stacker3_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskStacker[eSTACKER_03].GetSeqCmdStatus();
			if (stacker3_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			CStackerCmd_Unload* pSeqCmd = new CStackerCmd_Unload(true);
			if (SendCommand(g_TaskStacker[eSTACKER_03], pSeqCmd) == eSeqCmdAck_OK) {
				MakeLog(_T("[MainStep=%s] [SubStep=%d] [Stacker #3 Unload]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
				ChangeSubState(eExcutingStep_VAT_Move_Stacker_Up);
			}

		}break;
		case eExcutingStep_VAT_Move_Stacker_Up:
		{
			CBaseSeqCmd::eSeqCmdStatus stacker3_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskStacker[eSTACKER_03].GetSeqCmdStatus();
			if (stacker3_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			int nErr = g_TaskStacker[eSTACKER_03].ChkExistTray_FrontPos_Sen(DEF_EXIST);
			if (nErr == ERR_NO_ERROR) {
				CStackerCmd_TrayUp* pSeqCmd = new CStackerCmd_TrayUp;
				if (SendCommand(g_TaskStacker[eSTACKER_03], pSeqCmd) == eSeqCmdAck_OK) {
					MakeLog(_T("[MainStep=%s] [SubStep=%d] [Stacker #3 Up]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
					ChangeSubState(eExcutingStep_VAT_Check_All_Part_Complete);
				}
			}
			else {
				_Base_AlarmReport_VAT(ERR_VAT_TRAY_PP_STACKER_FRONT_POS_JIG_NOT_EXIST, _T("Stacker 3"));
			}

		}break;

		//========================================================= Table Measure Start
		case eExcutingStep_VAT_Table_Measure_Start:
		{
			MakeLog(_T("[MainStep=%s] [SubStep=%d][Table Measure Start]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			int nMeasureType = _Base_GetVATMeasureType(VAT_HAND_LOAD_TRAYPP);
			if (nMeasureType == VAT_MEASURE_TYPE_HAND_ONLY) {
				m_nTableTargetIdx = eTableTarget_VAT;
			}
			else{ //(VAT_MEASURE_TYPE_UNIT_ONLY || VAT_MEASURE_TYPE_BOTH) 				
				m_nTableTargetIdx = eTableTarget_1;
			}

			ChangeSubState(eExcutingStep_VAT_Std_Picker_Move_Target_Pos);
		}break;
		case eExcutingStep_VAT_Calculate_Table_Rotate_Pos:
		{
			if (IsCurSeqCmdReady())
				break;

			//Picker Align 1,4번 기울기 계산.
			std::pair<double, double> pPickerPos1 = g_VATCommonData.m_stVATPickerAlignInfo.pEachPickerPos[m_nHand][VAT_PITCH_TYPE_WIDE][VAT_PICKER_01];
			std::pair<double, double> pPickerPos2 = g_VATCommonData.m_stVATPickerAlignInfo.pEachPickerPos[m_nHand][VAT_PITCH_TYPE_WIDE][VAT_PICKER_04];

			double dX = pPickerPos2.first - pPickerPos1.first;
			double dY = pPickerPos2.second - pPickerPos1.second;
			double dPkrDegree = _Base_CalSlopeDegree(dX, dY);

			//Table 1,2 Target 기울기 계산.
			dX = m_dTblTargetPos[eTableTarget_2][VAT_X] - m_dTblTargetPos[eTableTarget_1][VAT_X];
			dY = m_dTblTargetPos[eTableTarget_2][VAT_Y] - m_dTblTargetPos[eTableTarget_1][VAT_Y];
			double dTblDegree = _Base_CalSlopeDegree(dX, dY);

			double dDegreeGap = dTblDegree - dPkrDegree; //보정 해야할 각도.

			//Table Rotate 보정 진행.
			//[12/22/2025 dohyeong.kim]
			//※Pulse Per Round 현재 맞지 않으면 각도 변경 하여 비율 계산 진행 필요.[실장비 검증 필요함.]
			//현재 Pulley비 1:1.5 정확한 값은 1:1.57
			double dCurRotatePos = g_VATCommonData.m_stVATInfoSeq.stPartBasePos.getBasePos(GET_VAT_PART_IDX(), GET_VAT_PART_POS(), VAT_PART_ROTATE);
			dCurRotatePos -= dDegreeGap;

			g_VATCommonData.m_stVATInfoSeq.stPartBasePos.setBasePos(GET_VAT_PART_IDX(), GET_VAT_PART_POS(), VAT_PART_ROTATE, dCurRotatePos);


			//Hand Only Type은 해당 Case에 들어오지 않음.
			int nMeasureType = _Base_GetVATMeasureType(VAT_HAND_LOAD_TRAYPP);
			ASSERT(nMeasureType == VAT_MEASURE_TYPE_BOTH || nMeasureType == VAT_MEASURE_TYPE_UNIT_ONLY);
			if(nMeasureType == VAT_MEASURE_TYPE_BOTH){
				//Setting 된 Rotate로 이동 하여, VAT 측정 진행.
				m_nTableTargetIdx = eTableTarget_VAT;
				ChangeSubState(eExcutingStep_VAT_Move_Hand_Pre_Pos_1);
			}
			else { //VAT_MEASURE_TYPE_UNIT_ONLY
				m_nTableTargetIdx = eTableTarget_None;
				ChangeSubState(eExcutingStep_VAT_Table_Release);
			}
		}break;
		//========================================================= Measure Common
		case eExcutingStep_VAT_Std_Picker_Move_Target_Pos:
		{
			if (IsCurSeqCmdReady())
				break;

			//[Hand Cmd Position]
			double dCmdPosHand[VAT_MAX_AXIS] = { 0, };
			_Base_GetSeqTargetPos(dCmdPosHand, eVisionAutoTeaching_HandBasePos);

			//[Part Cmd Position]
			double dCmdPosUnit[VAT_MAX_AXIS_YROTATE] = { 0, };
			_Base_GetSeqTargetPos_Unit(dCmdPosUnit, GET_VAT_PART_IDX(), (_eVAT_PART_ONLY_POS)GET_VAT_PART_POS());

			//[Safety Position] 
			double dSafetyPos[VAT_MAX_AXIS] = { 0, };
			_Base_GetSafetyPos(dSafetyPos);

			//[Make Pick Position] - Target 위치에 기준 Picker 이동.
			dCmdPosHand[VAT_X] += g_VATCommonData.m_stVATCamCenterCalInfo.dTopCamToPkrDistance[m_nHand][VAT_X];
			dCmdPosUnit[VAT_PART_Y] += g_VATCommonData.m_stVATCamCenterCalInfo.dTopCamToPkrDistance[m_nHand][VAT_Y];
			dCmdPosHand[VAT_Z] = dSafetyPos[VAT_Z];

			//[Target Limit Pos Check]
			_Base_CheckandCreateLimitPos(dCmdPosHand, VAT_X);
			_Base_CheckandCreateLimitPos_Unit(dCmdPosUnit, GET_VAT_PART_IDX());

			if (m_nCurLoc == VAT_IDX_TRAY_PP_FEEDER) {
				dCmdPosHand[VAT_Y] = dCmdPosUnit[VAT_PART_Y]; //Feeder Cmd는 Hand Y가 Feeder Y로 운용.
				CVatTrayLoadPPFeederCmd_MovePos* pSeqCmd = new CVatTrayLoadPPFeederCmd_MovePos(m_nSeqMode);
				pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker, Pre Position은 모든 Picker Safety
				pSeqCmd->m_strCmdStage = _T("Feeder Pick Pos");
				memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPosHand, sizeof(dCmdPosHand));

				if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
					MakeLog(_T("[MainStep=%s] [SubStep=%d] [VAT][Move Feeder Pick Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
				}
			}
			else { //Table
				CVatTrayLoadPPTableCmd_MovePos* pSeqCmd = new CVatTrayLoadPPTableCmd_MovePos(m_nSeqMode);
				pSeqCmd->m_nTableIdx = (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_2) ? eLoadTbl_2 : eLoadTbl_3;
				pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker, Pre Position은 모든 Picker Safety
				pSeqCmd->m_strCmdStage = _T("Table Pick Pos");
				memcpy_s(pSeqCmd->m_dCmdPos_Hand, sizeof(pSeqCmd->m_dCmdPos_Hand), dCmdPosHand, sizeof(dCmdPosHand));
				memcpy_s(pSeqCmd->m_dCmdPos_Table, sizeof(pSeqCmd->m_dCmdPos_Table), dCmdPosUnit, sizeof(dCmdPosUnit));

				if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
					MakeLog(_T("[MainStep=%s] [SubStep=%d] [VAT][Move Table Pick Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
				}
			}

			ChangeSubState(eExcutingStep_VAT_Find_Unit_Target_Z_Fast);

		}break;
		case eExcutingStep_VAT_Find_Unit_Target_Z_Fast:
		{
			if (IsCurSeqCmdReady())
				break;

			g_VATCommonData.ClearFirstPickOffset(VAT_HAND_LOAD_TRAYPP);

			CVatTrayLoadPPCmd_PickerHeightCal* pSeqCmd = new CVatTrayLoadPPCmd_PickerHeightCal();
			pSeqCmd->m_nSpeed = eZSpeed_Fast;
			pSeqCmd->move_offset_um = 0.0;
			pSeqCmd->m_nStage = m_nCurLoc;
			pSeqCmd->m_dTargetPosZ = g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_VacuumPos[VAT_HAND_LOAD_TRAYPP][VAT_Z]; //Plate Jig 높이가 다른 Target보다 높음.

			//기준 Picker
			CPoint cStdPicker = _Base_GetStdPicker();
			pSeqCmd->m_vCmdPicker.push_back(cStdPicker);

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Height Cal] [Speed=Fast, move_offset=0.0, Loc=plate jig]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			//_Base_SetZAutoTeachingSpd(pSeqCmd->m_nSpeed);
			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_VAT_Find_Unit_Target_Z_Slow);
			}
		}break;
		case eExcutingStep_VAT_Find_Unit_Target_Z_Slow:
		{
			if (IsCurSeqCmdReady())
				break;

			CVatTrayLoadPPCmd_PickerHeightCal* pSeqCmd = new CVatTrayLoadPPCmd_PickerHeightCal();
			pSeqCmd->m_nSpeed = eZSpeed_Slow;
			pSeqCmd->move_offset_um = g_VATCommonData.GetFirstPickOffset(VAT_HAND_LOAD_TRAYPP);
			pSeqCmd->m_nStage = m_nCurLoc;
			pSeqCmd->m_dTargetPosZ = g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_VacuumPos[VAT_HAND_LOAD_TRAYPP][VAT_Z];

			//기준 Picker
			CPoint cStdPicker = _Base_GetStdPicker();
			pSeqCmd->m_vCmdPicker.push_back(cStdPicker);

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Height Cal] [Speed=Slow, move_offset=0.0, Loc=plate jig]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
			_Base_SetZAutoTeachingSpd(pSeqCmd->m_nSpeed);
			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_VAT_Move_Hand_Pre_Pos_1);
			}
		}break;
		case eExcutingStep_VAT_Move_Hand_Pre_Pos_1:
		{
			if (IsCurSeqCmdReady())
				break;

			//===============Make Pre Position===============
			//[Hand Cmd Position] - X, Y, Z, XP, YP
			double dCmdPosHand[VAT_MAX_AXIS] = { 0, };
			_Base_GetSeqTargetPos(dCmdPosHand, eVisionAutoTeaching_HandBasePos);

			//[Part Cmd Position]
			double dCmdPosUnit[VAT_MAX_AXIS_YROTATE] = { 0, };
			_Base_GetSeqTargetPos_Unit(dCmdPosUnit, GET_VAT_PART_IDX(), (_eVAT_PART_ONLY_POS)GET_VAT_PART_POS());

			//[Safety Position] 
			double dSafetyPos[VAT_MAX_AXIS] = { 0, };
			_Base_GetSafetyPos(dSafetyPos);

			//[Make Pre Pos]
			dCmdPosHand[VAT_X] -= VAT_PRE_POS_100mm_OFFSET;
			dCmdPosUnit[VAT_PART_Y] -= VAT_PRE_POS_100mm_OFFSET;
			dCmdPosHand[VAT_Z] = dSafetyPos[VAT_Z];
			//==================================================

			//[Target Limit Pos Check]
			_Base_CheckandCreateLimitPos(dCmdPosHand, VAT_X);
			_Base_CheckandCreateLimitPos_Unit(dCmdPosUnit, GET_VAT_PART_IDX());

			if (m_nCurLoc == VAT_IDX_TRAY_PP_FEEDER) {
				dCmdPosHand[VAT_Y] = dCmdPosUnit[VAT_PART_Y]; //Feeder Cmd는 Hand Y가 Feeder Y로 운용.
				CVatTrayLoadPPFeederCmd_MovePos* pSeqCmd = new CVatTrayLoadPPFeederCmd_MovePos(m_nSeqMode);
				pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker, Pre Position은 모든 Picker Safety
				pSeqCmd->m_strCmdStage = _T("Feeder Pre Pos");
				memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPosHand, sizeof(dCmdPosHand));

				if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
					MakeLog(_T("[MainStep=%s] [SubStep=%d] [VAT][Move Feeder Pre Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
				}
			}
			else { //Table
				CVatTrayLoadPPTableCmd_MovePos* pSeqCmd = new CVatTrayLoadPPTableCmd_MovePos(m_nSeqMode);
				pSeqCmd->m_nTableIdx = (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_2) ? eLoadTbl_2 : eLoadTbl_3;
				pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker, Pre Position은 모든 Picker Safety
				pSeqCmd->m_strCmdStage = _T("Table Pre Pos1");
				memcpy_s(pSeqCmd->m_dCmdPos_Hand, sizeof(pSeqCmd->m_dCmdPos_Hand), dCmdPosHand, sizeof(dCmdPosHand));
				memcpy_s(pSeqCmd->m_dCmdPos_Table, sizeof(pSeqCmd->m_dCmdPos_Table), dCmdPosUnit, sizeof(dCmdPosUnit));
				
				if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {		
					MakeLog(_T("[MainStep=%s] [SubStep=%d] [VAT][Move Table Pre Pos1]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
				}
			}
		
			ChangeSubState(eExcutingStep_VAT_Move_Hand_Target_Pos_1);
		}break;
		case eExcutingStep_VAT_Move_Hand_Target_Pos_1:
		{
			if (IsCurSeqCmdReady())
				break;

			if (m_nCurLoc == VAT_IDX_TRAY_PP_FEEDER) {
				CVatTrayLoadPPFeederCmd_MovePos* pSeqCmd = new CVatTrayLoadPPFeederCmd_MovePos(m_nSeqMode);
				CPoint cStdPicker = _Base_GetStdPicker();
				pSeqCmd->m_vCmdPicker.push_back(cStdPicker); //Standard Picker
				pSeqCmd->m_strCmdStage = _T("Feeder Target Pos");
				_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos, eVisionAutoTeaching_HandBasePos); //[Cmd Position] - X,Z,XP,YP
				
				//Feeder Cmd는 Hand Y가 Feeder Y로 운용.
				double dCmdPosUnit[VAT_MAX_AXIS_YROTATE] = { 0, };
				_Base_GetSeqTargetPos_Unit(dCmdPosUnit, GET_VAT_PART_IDX(), (_eVAT_PART_ONLY_POS)GET_VAT_PART_POS());
				pSeqCmd->m_dCmdPos[VAT_Y] = dCmdPosUnit[VAT_PART_Y];
								
				if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
					MakeLog(_T("[MainStep=%s] [SubStep=%d] [VAT][Move Feeder Target Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
				}
			}
			else {
				CVatTrayLoadPPTableCmd_MovePos* pSeqCmd = new CVatTrayLoadPPTableCmd_MovePos(m_nSeqMode);
				pSeqCmd->m_nTableIdx = (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_2) ? eLoadTbl_2 : eLoadTbl_3;
				CPoint cStdPicker = _Base_GetStdPicker();
				pSeqCmd->m_vCmdPicker.push_back(cStdPicker); //Standard Picker
				pSeqCmd->m_strCmdStage = _T("Table Target Pos1");
				_Base_GetSeqTargetPos(pSeqCmd->m_dCmdPos_Hand, eVisionAutoTeaching_HandBasePos);
				_Base_GetSeqTargetPos_Unit(pSeqCmd->m_dCmdPos_Table, GET_VAT_PART_IDX(), (_eVAT_PART_ONLY_POS)GET_VAT_PART_POS());
			
				if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {				
					MakeLog(_T("[MainStep=%s] [SubStep=%d] [VAT][Move Table Target Pos1]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
				}
			}

			ChangeSubState(eExcutingStep_VAT_Target_Measure);
		}break;

		//Table Target 2 측정 Case.	
		case eExcutingStep_VAT_Move_Hand_Pre_Pos_2:
		{
			if (IsCurSeqCmdReady())
				break;

			//===============Make Pre Position===============
			//[Hand Cmd Position] - X, Y, Z, XP, YP
			double dCmdPosHand[VAT_MAX_AXIS] = { 0, };
			_Base_GetSeqTargetPos(dCmdPosHand, eVisionAutoTeaching_HandBasePos);

			//[Part Cmd Position]
			double dCmdPosUnit[VAT_MAX_AXIS_YROTATE] = { 0, };
			_Base_GetSeqTargetPos_Unit(dCmdPosUnit, GET_VAT_PART_IDX(), (_eVAT_PART_ONLY_POS)GET_VAT_PART_POS());

			//[Safety Position] 
			double dSafetyPos[VAT_MAX_AXIS] = { 0, };
			_Base_GetSafetyPos(dSafetyPos);

			//[Make Target 2 Pos]
			double dTargetDistance = g_DMCont.m_dmTrayPP.GN(NDM5_TrayPP_LdTbl_Pitch_X) * 3;
			dCmdPosHand[VAT_X] += dTargetDistance;

			//[Make Pre Pos]
			dCmdPosHand[VAT_X] -= VAT_PRE_POS_100mm_OFFSET;
			dCmdPosUnit[VAT_PART_Y] -= VAT_PRE_POS_100mm_OFFSET;
			dCmdPosHand[VAT_Z] = dSafetyPos[VAT_Z];
			//==================================================

			//[Target Limit Pos Check]
			_Base_CheckandCreateLimitPos(dCmdPosHand, VAT_X);
			_Base_CheckandCreateLimitPos_Unit(dCmdPosUnit, GET_VAT_PART_IDX());

			CVatTrayLoadPPTableCmd_MovePos* pSeqCmd = new CVatTrayLoadPPTableCmd_MovePos(m_nSeqMode);
			pSeqCmd->m_nTableIdx = (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_2) ? eLoadTbl_2 : eLoadTbl_3;
			pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker, Pre Position은 모든 Picker Safety
			pSeqCmd->m_strCmdStage = _T("Table Pre Pos2");
			memcpy_s(pSeqCmd->m_dCmdPos_Hand, sizeof(pSeqCmd->m_dCmdPos_Hand), dCmdPosHand, sizeof(dCmdPosHand));
			memcpy_s(pSeqCmd->m_dCmdPos_Table, sizeof(pSeqCmd->m_dCmdPos_Table), dCmdPosUnit, sizeof(dCmdPosUnit));

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [VAT][Move Table Pre Pos2]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_VAT_Move_Hand_Target_Pos_2);
			}	
		}break;
		case eExcutingStep_VAT_Move_Hand_Target_Pos_2:
		{
			if (IsCurSeqCmdReady())
				break;

			//[Make Target 2 Pos]
			double dCmdPosHand[VAT_MAX_AXIS] = { 0, };
			_Base_GetSeqTargetPos(dCmdPosHand, eVisionAutoTeaching_HandBasePos);

			double dTargetDistance = g_DMCont.m_dmTrayPP.GN(NDM5_TrayPP_LdTbl_Pitch_X) * 3;
			dCmdPosHand[VAT_X] += dTargetDistance;

			CVatTrayLoadPPTableCmd_MovePos* pSeqCmd = new CVatTrayLoadPPTableCmd_MovePos(m_nSeqMode);
			pSeqCmd->m_nTableIdx = (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_2) ? eLoadTbl_2 : eLoadTbl_3;
			CPoint cStdPicker = _Base_GetStdPicker();
			pSeqCmd->m_vCmdPicker.push_back(cStdPicker); //Standard Picker
			pSeqCmd->m_strCmdStage = _T("Table Target Pos1");
			memcpy_s(pSeqCmd->m_dCmdPos_Hand, sizeof(pSeqCmd->m_dCmdPos_Hand), dCmdPosHand, sizeof(dCmdPosHand));
			_Base_GetSeqTargetPos_Unit(pSeqCmd->m_dCmdPos_Table, GET_VAT_PART_IDX(), (_eVAT_PART_ONLY_POS)GET_VAT_PART_POS());

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [VAT][Move Table Target Pos2]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_VAT_Target_Measure);
			}
		}break;
		case eExcutingStep_VAT_Target_Measure: //Feeder, Table 측정 공통 사항.
		{
			if (IsCurSeqCmdReady())
				break;

			//이전 Position 저장.
			m_dPrevPosX = g_VATCommonData.m_stVATInfoSeq.stHandBasePos.getBasePos(m_nHand, m_nCurLoc, VAT_X);
			m_dPrevPosY = g_VATCommonData.m_stVATInfoSeq.stPartBasePos.getBasePos(GET_VAT_PART_IDX(), GET_VAT_PART_POS(), VAT_PART_Y);

			//C-Tray Jig 상단 Target 측정.
			CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(m_nHand);
			pSeqCmd->m_vDataId.push_back(VISION_REQ_DATAID_VAT_LD_UPPER);
			pSeqCmd->m_nStatus = GET_STATUS_ID_UPPER();

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Send Measure]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
				//GUI Notify
				//CVAT_MeasureStatus::GetInstance().UpdateVisionMeasureCnt(VAT_CAM_LD_TRAY_PP_UPPER, m_nVisionRetryCnt + 1);

				ChangeSubState(eExcutingStep_VAT_Target_Measure_Chk);
			}
		}break;
		case eExcutingStep_VAT_Target_Measure_Chk:
		{
			if (IsCurSeqCmdReady())
				break;

			if (m_dPrevPosX != g_VATCommonData.m_stVATInfoSeq.stHandBasePos.getBasePos(m_nHand, m_nCurLoc, VAT_X) ||
				m_dPrevPosY != g_VATCommonData.m_stVATInfoSeq.stPartBasePos.getBasePos(GET_VAT_PART_IDX(), GET_VAT_PART_POS(), VAT_PART_Y))
			{
				m_nVisionRetryCnt++;

				if (m_nVisionRetryCnt >= DEF_VAT_VISION_RETRY_CNT_MAX) {
					_Base_AlarmReport_VAT(ERR_VAT_VISION_RETRY_CNT_OVER);
				}

				MakeLog(_T("[MainStep=%s] [SubStep=%d] [Target Measure Retry][Cnt : %d]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, m_nVisionRetryCnt);

				//Center Find까지 재측정.
				if (m_nCurLoc == VAT_IDX_TRAY_PP_FEEDER) { //Feeder
					ChangeSubState(eExcutingStep_VAT_Move_Hand_Pre_Pos_1);
				}
				else { //Table
					int sub_step = (m_nTableTargetIdx == eTableTarget_1) ? eExcutingStep_VAT_Move_Hand_Pre_Pos_1 : eExcutingStep_VAT_Move_Hand_Pre_Pos_2;
					ChangeSubState(sub_step);
				}			
			}
			else
			{
				//Center Find.
				MakeLog(_T("[MainStep=%s] [SubStep=%d] [Target Center Find]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

				ChangeSubState(eExcutingStep_VAT_Target_Measure_Done);
			}
		}break;

		case eExcutingStep_VAT_Target_Measure_Done:
		{
			if (IsCurSeqCmdReady())
				break;

			if (m_nCurLoc == LOC_IDX_TRAY_PP_FEEDER) {
				ChangeSubState(eExcutingStep_VAT_Feeder_Place_C_Tray_Jig);
			}
			else { //Table
				if (m_nTableTargetIdx == eTableTarget_VAT) { //Target1만 측정
					ChangeSubState(eExcutingStep_VAT_Table_Release);
				}
				else { //(VAT_MEASURE_TYPE_UNIT_ONLY || VAT_MEASURE_TYPE_BOTH) //Target2 측정.				
					m_dTblTargetPos[m_nTableTargetIdx][VAT_X] = g_VATCommonData.m_stVATInfoSeq.stHandBasePos.getBasePos(m_nHand, m_nCurLoc, VAT_X);
					m_dTblTargetPos[m_nTableTargetIdx][VAT_Y] = g_VATCommonData.m_stVATInfoSeq.stPartBasePos.getBasePos(GET_VAT_PART_IDX(), GET_VAT_PART_POS(), VAT_X);

					if (m_nTableTargetIdx == eTableTarget_1) {
						//Target2 측정 필요.
						m_nTableTargetIdx = eTableTarget_2;
						ChangeSubState(eExcutingStep_VAT_Move_Hand_Pre_Pos_2);
					}
					else {//eTableTarget_2
						//Target 1, 2 측정 완료 후, Rotate Calculate.
						ChangeSubState(eExcutingStep_VAT_Calculate_Table_Rotate_Pos);
					}			
				}
			}
		}break;
		case eExcutingStep_VAT_Table_Release:
		{
			//Table 측정 완료 후 case
			if (IsCurSeqCmdReady())
				break;

			bool bIsRelease = g_VatTaskSystemCtrl.release_tbl_work_hand(m_nCurLoc, m_nHand);
			if (bIsRelease) {
				MakeLog(_T("[MainStep=%s] [SubStep=%d][Release Table Success][%s]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, g_TaskTrayLoadPP.GetLocName((eLocIdxTrayPP)m_nCurLoc));
			}

			ChangeSubState(eExcutingStep_VAT_Check_All_Part_Complete);
		}break;
		case eExcutingStep_VAT_Check_All_Part_Complete:
		{
			//m_nCurLoc 측정 관련 모든 동작 완료 후, Check 하는 Case

			//[Status] ONGOING -> COMPLETE
			_Base_SetMeasurePart_Status(m_nCurLoc, VAT_STATUS_MEASURE_COMPLETE);

			BOOL bIsAllComplete = _Base_IsAllPartSameStatus(VAT_STATUS_MEASURE_COMPLETE);
			if (bIsAllComplete){
				MakeLog(_T("[MainStep=%s] [SubStep=%d][All Part Measure Complete]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

				ChangeSubState(eExcutingStep_VAT_End);
			}
			else {
				ChangeSubState(eExcutingStep_VAT_Select_Measure_Part);			
			}
		}break;
		case eExcutingStep_VAT_End:
		{
			MakeLog(_T("[Mode=%s] [MainStep=%s] [SubStep=%d] [End]"), GetExcuteModeStr(m_nSeqMode), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			ChangeMainState(eAutoLotState_CleanOut);
		}break;
		default:
			break;
	}

}

//===================================================================================================[Z Auto Teaching]
void CVAT_TaskAutoTrayLoadPP::AutoState_Execute_Z_Auto_Teaching()
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

		MakeLog(_T("[Mode=%s] [MainStep=%s] [SubStep=%d] [Start]"), GetExcuteModeStr(m_nSeqMode), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
		ChangeSubState(eExcutingStep_ZTeaching_CheckCondition);
	}break;
	case eExcutingStep_ZTeaching_CheckCondition:
	{
		//RECOVERY에서 모든 Mode 공통으로 사용하는 것 고려해 보기.
		if (m_nCurLoc != -1)
		{
			// [6/11/2025 dohyeong.kim] 순서 변경 하지 말것 

			// 1. VAT Tray Load PP 관련 Cmd 진행 확인.
			int cmd_ready = IsAllCmdReady();
			if (cmd_ready != ERR_NO_ERROR)
				break;

			// 2. 전체 Part 측정 완료 확인.
			BOOL bIsAllComplete = _Base_IsAllPartSameStatus(VAT_STATUS_MEASURE_COMPLETE);
			if (bIsAllComplete) {
				//Feeder에 있는 C-Tray 배출
				int feeder_jig_exist = g_TaskTrayFeeder.ChkTrayExistSensor(eExist);
				if (feeder_jig_exist == ERR_NO_ERROR) {
					ChangeSubState(eExcutingStep_ZTeaching_Feeder_Place_Stacker);
				}
				else {
					//Feeder에 Jig 없으면 Auto Seq End.
					ChangeSubState(eExcutingStep_ZTeaching_End);
				}				
				break;
			}

			// 3. 현재 Part 측정 완료 확인.
			int nCurPartStatus = _Base_GetMeasurePart_Status(m_nCurLoc);
			if (nCurPartStatus == VAT_STATUS_MEASURE_COMPLETE) {
				ChangeSubState(eExcutingStep_ZTeaching_Table_Release);
				break;
			}

			// 4. Part 측정 완료되지 않았을 경우.
			if (m_nCurLoc == LOC_IDX_TRAY_PP_FEEDER) {
				int nChkFeederPos = g_TaskTrayFeeder.ChkMotorPos(CTaskTrayFeeder::eAxisY, eLocIdxFeeder::LOC_IDX_FEEDER_LOAD_PICK_START_POS);
				if (nChkFeederPos != ERR_NO_ERROR) //Feeder 측정 위치가 아님.
				{
					int feeder_jig_exist = g_TaskTrayFeeder.ChkTrayExistSensor(eExist);
					int stacker_rear_jig_exist = g_TaskStacker[eSTACKER_03].ChkExistTray_RearPos_Sen(DEF_EXIST);
					int stacker_front_jig_exist = g_TaskStacker[eSTACKER_03].ChkExistTray_FrontPos_Sen(DEF_EXIST);

					if (feeder_jig_exist == ERR_NO_ERROR) {
						ChangeSubState(eExcutingStep_ZTeaching_Move_Feeder);
						break;
					}
					else { //Feeder에 C-Tray 없을 경우
						if (stacker_rear_jig_exist == ERR_NO_ERROR && stacker_front_jig_exist != ERR_NO_ERROR) //Rear에 존재 할 경우.
						{ 
							ChangeSubState(eExcutingStep_ZTeaching_Feeder_Pick_Stacker);
						}
						else if (stacker_rear_jig_exist != ERR_NO_ERROR && stacker_front_jig_exist == ERR_NO_ERROR) //Front에 존재 할 경우.
						{
							ChangeSubState(eExcutingStep_ZTeaching_Move_Stacker_Load);
						}
						else if (stacker_rear_jig_exist != ERR_NO_ERROR && stacker_front_jig_exist != ERR_NO_ERROR) //둘다 없을 경우.
						{
							/*C-Tray Jig Stacker#3 Front에 안착 시, 높이로 인해 Exist Sensor 감지 안될 수 도 있음(1호기)*/
							//case 1 : Rear/Front에 없고 Conveyor 중간에 있을 경우. -> Stacker Down후, Load하기 때문에 괜찮음.
							//case 2 : Stacker#3 Front에 안착 되어 있을 경우.

							ChangeSubState(eExcutingStep_ZTeaching_Move_Stacker_Down);
						}
						else // Rear와 Front에 C-Tray가 있음, Sensor Error
						{
							if (g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
							{
								_Base_AlarmReport_VAT(ERR_VAT_TRAY_PP_STACKER_FRONT_AND_REAR_BOTH_POS_JIG_EXIST);
							}
						}
						break;					
					}

// 					ChangeSubState(eExcutingStep_ZTeaching_Move_Feeder);
// 					break;
				}
			}
			else { //Table
				int nTblIndex = (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_2) ? eLoadTbl_2 : eLoadTbl_3;	

				//현재 Table 소유권이 없거나, 현재 Hand가 소유권 가지고 있으면 bIsPossible == true
				bool bIsPossible = g_VatTaskSystemCtrl.reserve_tbl_work_hand(nTblIndex, m_nHand);
				if (!bIsPossible) {
					//Part 상태 변경[Init->Wait]
					_Base_SetMeasurePart_Status(m_nCurLoc, VAT_STATUS_MEASURE_WAIT);
					ChangeSubState(eExcutingStep_ZTeaching_Table_Release);
					break;
				}
				////////////////////////////////////////////////////////////////////////////////////////////////////////////part 상태 ongoing으로 변경 해줘야함.
				int nInPos = 0;
				double dCmdPos[CTaskLoadTable::eMaxAxisCount] = { 0, };

				for (int i = 0; i < CTaskLoadTable::eMaxAxisCount; i++) {
					//[Cmd Position]
					dCmdPos[i] = g_TaskLoadTbl[nTblIndex].GetTeachPos(LOC_IDX_TBL_LD_SIDE, i);

					//Tray Load PP의 경우, Y축이 없기 때문에 Table이 Y축 역할.
					if (i == CTaskLoadTable::eAxisY) {
						double tbl_y_half_pitch = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_Y) / 2.0;
						dCmdPos[i] += tbl_y_half_pitch;
					}

					//[Motor Position Check]
					int nChkTablePos = g_TaskLoadTbl[nTblIndex].ChkVATMotorPos(i, dCmdPos[i]);
					if (nChkTablePos == ERR_NO_ERROR) 
						nInPos++;
				}

				if (nInPos != CTaskLoadTable::eMaxAxisCount) {
					ChangeSubState(eExcutingStep_ZTeaching_Move_Loading_Tbl);
					break;
				}

				//이전
// 				int nChkTablePos = g_TaskLoadTbl[nTblIndex].ChkMotorPos(LOC_IDX_TBL_LD_SIDE, CTaskLoadTable::eAxisAll);
// 				if (nChkTablePos != ERR_NO_ERROR) {
// 					ChangeSubState(eExcutingStep_ZTeaching_Move_Loading_Tbl);
// 					break;
// 				}
			}

			//m_nCurLoc이 Feeder or Table.
			//Feeder Jig있으며 측정 위치에 있음.
			//Load Table이 측정 위치에 있음.

			//Slow, Fast 확인.
			int nCurSpeed = _Base_GetZAutoTeachingSpd();
			if (nCurSpeed == eZSpeed_Fast) {
				//Fast Cmd 실행 상태.
				ChangeSubState(eExcutingStep_ZTeaching_Find_Target_Z_Slow);
			}
			else if (nCurSpeed == eZSpeed_Slow) {
				ChangeSubState(eExcutingStep_ZTeaching_Find_Target_Z_End);
			}
			else { //nCurSpeed == eZSpeed_None
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
		if (m_nCurLoc != -1 && m_nCurLoc != LOC_IDX_TRAY_PP_FEEDER)
		{
			bool bIsRelease = g_VatTaskSystemCtrl.release_tbl_work_hand(m_nCurLoc, m_nHand);
			if (bIsRelease) {
				MakeLog(_T("[MainStep=%s] [SubStep=%d][Release Table Success][%s]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, g_TaskTrayLoadPP.GetLocName((eLocIdxTrayPP)m_nCurLoc));
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

			if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_ && (m_nCurLoc != LOC_IDX_TRAY_PP_FEEDER)) {
				ChangeSubState(eExcutingStep_ZTeaching_End);
				break;
			}


			//Feeder에 있는 C-Tray Jig 배출
			int feeder_jig_exist = g_TaskTrayFeeder.ChkTrayExistSensor(eExist);
			if (feeder_jig_exist == ERR_NO_ERROR) {
				ChangeSubState(eExcutingStep_ZTeaching_Feeder_Place_Stacker);
			}
			else {
				//Feeder에 Jig 없으면 Auto Seq End.
				ChangeSubState(eExcutingStep_ZTeaching_End);
			}

			break;
		}

		ChangeSubState(eExcutingStep_ZTeaching_Select_Part);
		
	}break;
	case eExcutingStep_ZTeaching_Select_Part:
	{
		CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
		if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		m_nCurLoc = _Base_GetMeasurePart_Selected(VAT_STATUS_MEASURE_READY);

		if (m_nCurLoc != ERR_VAT_ERROR)
		{
			MakeLog(_T("[MainStep=%s] [SubStep=%d][Select Part=%s]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, g_TaskTrayLoadPP.GetLocName((eLocIdxTrayPP)m_nCurLoc));
			
			if (m_nCurLoc == LOC_IDX_TRAY_PP_FEEDER) {
				ChangeSubState(eExcutingStep_ZTeaching_Move_Stacker_Down);
			}
			else { //Table

				int nTblIndex = (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_2) ? eLoadTbl_2 : eLoadTbl_3;

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

			//Part 상태 변경[Init -> Ongoing]
			_Base_SetMeasurePart_Status(m_nCurLoc, VAT_STATUS_MEASURE_ONGOING);
		}
		else
		{
			//측정 대기 Part 탐색.(Loading Table)
			m_nCurLoc = _Base_GetMeasurePart_Selected(VAT_STATUS_MEASURE_WAIT);
			if (m_nCurLoc != ERR_VAT_ERROR) {
				int nTblIndex = (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_2) ? eLoadTbl_2 : eLoadTbl_3;

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
	case eExcutingStep_ZTeaching_Move_Stacker_Down:
	{
		MakeLog(_T("[MainStep=%s] [SubStep=%d] [Stacker #3 Down]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

		int stacker_front_jig_not_exist = g_TaskStacker[eSTACKER_03].ChkExistTray_FrontPos_Sen(DEF_NOT_EXIST);
		if (stacker_front_jig_not_exist == ERR_NO_ERROR) {
			CStackerCmd_TrayDn* pSeqCmd = new CStackerCmd_TrayDn();
			if (SendCommand(g_TaskStacker[eSTACKER_03], pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_ZTeaching_Move_Stacker_Load);
				break;
			}
		}
		else {
			_Base_AlarmReport_VAT(ERR_VAT_TRAY_PP_STACKER_FRONT_POS_JIG_EXIST, _T("Stacker 3"));
		}
	}break;
	case eExcutingStep_ZTeaching_Move_Stacker_Load:
	{
		if (g_TaskStacker[eSTACKER_03].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}
		
		int stacker_rear_jig_not_exist = g_TaskStacker[eSTACKER_03].ChkExistTray_RearPos_Sen(DEF_NOT_EXIST);
		int stacker_front_jig_exist = g_TaskStacker[eSTACKER_03].ChkExistTray_FrontPos_Sen(DEF_EXIST);
		if (stacker_rear_jig_not_exist == ERR_NO_ERROR && stacker_front_jig_exist == ERR_NO_ERROR) {
			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Stacker #3 Load]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
			CStackerCmd_Load* pSeqCmd = new CStackerCmd_Load(true);
			if (SendCommand(g_TaskStacker[eSTACKER_03], pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_ZTeaching_Feeder_Pick_Stacker);
			}
		}
		else{
			//Rear, Front Position에 대한 알람.
			_Base_AlarmReport_VAT(((stacker_rear_jig_not_exist != ERR_NO_ERROR) ? ERR_VAT_TRAY_PP_STACKER_REAR_POS_JIG_EXIST : ERR_VAT_TRAY_PP_STACKER_FRONT_POS_JIG_NOT_EXIST), _T("Stacker 3"));
		}	
	}break;
	case eExcutingStep_ZTeaching_Feeder_Pick_Stacker:
	{
		if (g_TaskStacker[eSTACKER_03].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready ||
			g_TaskTrayFeeder.GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		int stacker_rear_jig_exist = g_TaskStacker[eSTACKER_03].ChkExistTray_RearPos_Sen(DEF_EXIST);
		int feeder_jig_no_exist = g_TaskTrayFeeder.ChkTrayExistSensor(eNot_Exist);
		bool conveyor_moving = g_TaskStacker[eSTACKER_03].IsConveyorMoving();
		if (stacker_rear_jig_exist == ERR_NO_ERROR && feeder_jig_no_exist == ERR_NO_ERROR && conveyor_moving == false) {
			CTrayFeederCmd_MovePickPlace* pSeqCmd = new CTrayFeederCmd_MovePickPlace();
			pSeqCmd->m_nCmdStage = eLocIdxFeeder::LOC_IDX_FEEDER_TRAY_PICK_POS;
			pSeqCmd->m_nCmdPickPlace = ePick;

			if (SendCommand(g_TaskTrayFeeder, pSeqCmd) == eSeqCmdAck_OK) {
				MakeLog(_T("[MainStep=%s] [SubStep=%d] [Feeder Pick]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
				ChangeSubState(eExcutingStep_ZTeaching_Move_Feeder);
				break;
			}
		}
		else {
			int nErr = -1;
			if (stacker_rear_jig_exist != ERR_NO_ERROR)		nErr = ERR_VAT_TRAY_PP_STACKER_REAR_POS_JIG_NOT_EXIST;
			else if (feeder_jig_no_exist != ERR_NO_ERROR)	nErr = ERR_VAT_TRAY_PP_FEEDER_JIG_EXIST;
			else                                            nErr = ERR_VAT_TRAY_PP_CONVEYOR_IS_MOVING_BEFORE_FEEDER_PICK;

			_Base_AlarmReport_VAT(nErr, _T("Stacker 3"));
			break;
		}
	}break;
	case eExcutingStep_ZTeaching_Move_Feeder:
		{
			CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
			if (feeder_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready)
				break;

			//Feeder Tray 감지 확인.	
			int nErr = g_TaskTrayFeeder.ChkTrayExistSensor(eExist);
			if (nErr != ERR_NO_ERROR) {
				_Base_AlarmReport_VAT(ERR_VAT_TRAY_PP_FEEDER_JIG_NOT_EXIST);
				break;
			}

			//Feeder Move
			double dCmdPos = g_TaskTrayFeeder.GetTeachPos(CTaskTrayFeeder::eAxisY, LOC_IDX_FEEDER_LOAD_PICK_START_POS);
			dCmdPos += 0.0; //Feeder Z Teaching 가능한 위치 offset.

			int nChkFeederPos = g_TaskTrayFeeder.ChkVATMotorPos(dCmdPos);
			//int nChkFeederPos = g_TaskTrayFeeder.ChkMotorPos(CTaskTrayFeeder::eAxisY, eLocIdxFeeder::LOC_IDX_FEEDER_LOAD_PICK_START_POS);
			if (nChkFeederPos != ERR_NO_ERROR) 
			{
				if (g_TaskTrayFeeder.GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
				{
					
// 					CTrayFeederCmd_MovePos* pSeqCmd = new CTrayFeederCmd_MovePos();
// 					pSeqCmd->m_nCmdStage = eLocIdxFeeder::LOC_IDX_FEEDER_LOAD_PICK_START_POS;

					CVatTrayFeederCmd_MovePos* pSeqCmd = new CVatTrayFeederCmd_MovePos(m_nSeqMode);
					pSeqCmd->m_dCmdpos = dCmdPos;

					MakeLog(_T("[MainStep=%s] [SubStep=%d] [Feeder Move Pick Start POS]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

					if (SendCommand(g_TaskTrayFeeder, pSeqCmd) == eSeqCmdAck_OK) {
// 						ChangeSubState(eExcutingStep_ZTeaching_Wait_Parts_Move);
// 						break;
					}
				}
			}

			ChangeSubState(eExcutingStep_ZTeaching_Wait_Parts_Move);
		}break;
	case eExcutingStep_ZTeaching_Move_Loading_Tbl:
		{
			int nTblIndex = (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_2) ? eLoadTbl_2 : eLoadTbl_3;
			CBaseSeqCmd::eSeqCmdStatus table_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskLoadTbl[nTblIndex].GetSeqCmdStatus();
			if (table_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}

			int nInPos = 0;
			double dCmdPos[CTaskLoadTable::eMaxAxisCount] = { 0, };

			for (int i = 0; i < CTaskLoadTable::eMaxAxisCount; i++) {
				//[Cmd Position]
				dCmdPos[i] = g_TaskLoadTbl[nTblIndex].GetTeachPos(LOC_IDX_TBL_LD_SIDE, i);

				//Tray Load PP의 경우, Y축이 없기 때문에 Table이 Y축 역할.
				if (i == CTaskLoadTable::eAxisY) {
					double tbl_y_half_pitch = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_Y) / 2.0;
					dCmdPos[i] -= tbl_y_half_pitch;
				}

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

				MakeLog(_T("[MainStep=%s] [SubStep=%d] [Table %d Move Load Site]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, nTblIndex + 1);

				if (SendCommand(g_TaskLoadTbl[nTblIndex], pSeqCmd) == eSeqCmdAck_OK) {
					//ChangeSubState(eExcutingStep_ZTeaching_Wait_Parts_Move);
				}				
			}

			ChangeSubState(eExcutingStep_ZTeaching_Wait_Parts_Move);		
		}break;
	//Feeder or Table Move wait
	case eExcutingStep_ZTeaching_Wait_Parts_Move:
		{
			if (m_nCurLoc == LOC_IDX_TRAY_PP_FEEDER) {
				CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
				if (feeder_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready)
					break;
			}
			else {
				int nTblIndex = (m_nCurLoc == VAT_IDX_TRAY_PP_TBL_2) ? eLoadTbl_2 : eLoadTbl_3;
				CBaseSeqCmd::eSeqCmdStatus table_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskLoadTbl[nTblIndex].GetSeqCmdStatus();
				if (table_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
					break;
				}
			}

			ChangeSubState(eExcutingStep_ZTeaching_Move_Target_Pos);
		}break;
	case eExcutingStep_ZTeaching_Move_Target_Pos:
	{
		CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
		if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}
		
		//[Cmd Position] - X,Y,Z,XP,YP
		double dCmdPos[VAT_MAX_AXIS] = { 0, };
		for (int i = 0; i < VAT_MAX_AXIS_XYZ; i++) {
			dCmdPos[i] = g_VATCommonData.m_stVATZTeachInfoSeq.stZTeachBasePos.getPartBasePos(VAT_HAND_LOAD_TRAYPP, m_nCurLoc, i);
		}
		int nLocStageConv = g_TaskTrayLoadPP.TrayHandLocIdxConvertor(RIDX_TRAY_PP_1, m_nCurLoc);
		dCmdPos[VAT_PITCH_X] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_1), nLocStageConv, POS_IDX_TRAY_LOAD_PP_X_PITCH);
		dCmdPos[VAT_PITCH_Y] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_1), nLocStageConv, POS_IDX_TRAY_LOAD_PP_Y_PITCH);


		CVatTrayLoadPPCmd_MovePos* pSeqCmd = new CVatTrayLoadPPCmd_MovePos(m_nSeqMode);
		pSeqCmd->m_vCmdPicker = pSeqCmd->m_vBasePicker; //8ea Picker
		pSeqCmd->m_strCmdStage = g_TaskTrayLoadPP.GetLocName((eLocIdxTrayPP)m_nCurLoc);
		memcpy_s(pSeqCmd->m_dCmdPos, sizeof(pSeqCmd->m_dCmdPos), dCmdPos, sizeof(dCmdPos));

		MakeLog(_T("[MainStep=%s] [SubStep=%d] [Hand Move Target Pos]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

		if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
			ChangeSubState(eExcutingStep_ZTeaching_Find_Target_Z_Fast);
		}

	}break;
	/*
	여기 까지 Target Unit 및 Hand가 측정 위치에 있어야 함.

	Z Auto Teaching 시작.
	*/
	case eExcutingStep_ZTeaching_Find_Target_Z_Fast:
	{
		CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
		if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		g_VATCommonData.ClearFirstPickOffset(VAT_HAND_LOAD_TRAYPP);

		CVatTrayLoadPPCmd_PickerHeightCal* pSeqCmd = new CVatTrayLoadPPCmd_PickerHeightCal();
		pSeqCmd->m_nSpeed = eZSpeed_Fast;
		pSeqCmd->move_offset_um = 0.0;
		pSeqCmd->m_nStage = m_nCurLoc;
		pSeqCmd->m_dTargetPosZ = g_VATCommonData.m_stVATZTeachInfoSeq.stZTeachBasePos.getPartBasePos(VAT_HAND_LOAD_TRAYPP, m_nCurLoc, VAT_Z);
		//MakeCmdPicker(pSeqCmd->m_vCmdPicker);

		MakeLog(_T("[MainStep=%s] [SubStep=%d] [Height Cal] [Speed=Fast, move_offset=0.0, Loc=%s]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, g_TaskTrayLoadPP.GetLocName((eLocIdxTrayPP)m_nCurLoc));

		_Base_SetZAutoTeachingSpd(pSeqCmd->m_nSpeed);
		if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
			ChangeSubState(eExcutingStep_ZTeaching_Find_Target_Z_Slow);
		}

	}break;
	case eExcutingStep_ZTeaching_Find_Target_Z_Slow:
	{
		CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
		if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		CVatTrayLoadPPCmd_PickerHeightCal* pSeqCmd = new CVatTrayLoadPPCmd_PickerHeightCal();
		pSeqCmd->m_nSpeed = eZSpeed_Slow;
		pSeqCmd->move_offset_um = g_VATCommonData.GetFirstPickOffset(VAT_HAND_LOAD_TRAYPP);
		pSeqCmd->m_nStage = m_nCurLoc;
		pSeqCmd->m_dTargetPosZ = g_VATCommonData.m_stVATZTeachInfoSeq.stZTeachBasePos.getPartBasePos(VAT_HAND_LOAD_TRAYPP, m_nCurLoc, VAT_Z);
		//MakeCmdPicker(pSeqCmd->m_vCmdPicker);
		//MakePickerBMAxis(pSeqCmd->m_vBMAxis, CBbxmnpClient::PICK_MOTOR_LOAD_TRAY);

		MakeLog(_T("[MainStep=%s] [SubStep=%d] [Height Cal] [Speed=Slow, move_offset=%.3f, Loc=%s]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep, pSeqCmd->move_offset_um, g_TaskTrayLoadPP.GetLocName((eLocIdxTrayPP)m_nCurLoc));

		_Base_SetZAutoTeachingSpd(pSeqCmd->m_nSpeed);
		if (SendCommand(g_TaskTrayLoadPP, pSeqCmd) == eSeqCmdAck_OK) {
			ChangeSubState(eExcutingStep_ZTeaching_Find_Target_Z_End);
		}

	}break;
	case eExcutingStep_ZTeaching_Find_Target_Z_End:
	{
		CBaseSeqCmd::eSeqCmdStatus load_pp_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayLoadPP.GetSeqCmdStatus();
		if (load_pp_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		_Base_SetMeasurePart_Status(m_nCurLoc, VAT_STATUS_MEASURE_COMPLETE);

//		SetPartStatus(m_nCurLoc, VAT_STATUS_MEASURE_COMPLETE);
// 		HWND hOwner = GetVatMainGuiHandle();
// 		if(hOwner != NULL)
// 			::SendMessage(hOwner, UM_VAT_SEQ_MSG_CHANGE_PART_COLOR, (WPARAM)m_nHand, (LPARAM)m_nCurLoc);

		_Base_SetZAutoTeachingSpd(eZSpeed_None);
		ChangeSubState(eExcutingStep_ZTeaching_Table_Release);
	}break;
	//Feeder에 Jig Exist 상태.
	case eExcutingStep_ZTeaching_Feeder_Place_Stacker:
	{
		CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
		if (feeder_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready)
			break;

		CBaseSeqCmd::eSeqCmdStatus transfer_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTransfer.GetSeqCmdStatus();
		if (transfer_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready)
			break;

		//Transfer Safety 위치 확인.
		bool bSafetyTransfer = g_TaskTransfer.is_safety_state();
		if (bSafetyTransfer != TRUE) {
			break;
		}

		//Conveyor 위에 감지 되는 것이 있는지 확인.
		int stacker_rear_jig_not_exist = g_TaskStacker[eSTACKER_03].ChkExistTray_RearPos_Sen(DEF_NOT_EXIST);
		int stacker_front_jig_not_exist = g_TaskStacker[eSTACKER_03].ChkExistTray_FrontPos_Sen(DEF_NOT_EXIST);
		int transfer_area_jig_not_exist = g_TaskTrayFeeder.ChkTrayExist_TransferArea_Sen(eNot_Exist);
		bool conveyor_moving = g_TaskStacker[eSTACKER_03].IsConveyorMoving();

		if (stacker_rear_jig_not_exist == ERR_NO_ERROR && 
			stacker_front_jig_not_exist == ERR_NO_ERROR && 
			transfer_area_jig_not_exist == ERR_NO_ERROR &&
			conveyor_moving == false) {
			CTrayFeederCmd_MovePickPlace* pSeqCmd = new CTrayFeederCmd_MovePickPlace();
			pSeqCmd->m_nCmdStage = eLocIdxFeeder::LOC_IDX_FEEDER_TRAY_PLACE_POS;
			pSeqCmd->m_nCmdPickPlace = ePlace;

			if (SendCommand(g_TaskTrayFeeder, pSeqCmd) == eSeqCmdAck_OK) {
				MakeLog(_T("[MainStep=%s] [SubStep=%d] [Feeder Place]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
				ChangeSubState(eExcutingStep_ZTeaching_Feeder_Move_Safety);
				break;
			}
		}
	}break;
	case eExcutingStep_ZTeaching_Feeder_Move_Safety:
	{
		CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
		if (feeder_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready)
			break;

		if (g_TaskTrayFeeder.GetSeqCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Ready)
		{
			CTrayFeederCmd_MovePos* pSeqCmd = new CTrayFeederCmd_MovePos();
			pSeqCmd->m_nCmdStage = eLocIdxFeeder::LOC_IDX_FEEDER_SAFETY;

			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Feeder Move Safety POS]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

			if (SendCommand(g_TaskTrayFeeder, pSeqCmd) == eSeqCmdAck_OK) {
				ChangeSubState(eExcutingStep_ZTeaching_Check_Feeder_Measure_Remain);
				break;
			}
		}
	}break;
	case eExcutingStep_ZTeaching_Check_Feeder_Measure_Remain:
	{
		CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
		if (feeder_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready)
			break;

		//Unload Tray PP 측정 남았는지 확인.
		BOOL bIsRemain = FALSE;
		for (int nPart = VAT_IDX_TRAY_PP_CONV_4; nPart < VAT_IDX_TRAY_PP_CONV_7; nPart++) {
			UNITCOMPONENT cStackerUnit = g_VatTaskAutoTrayUnloadPP._Base_GetMeasurePart_Each(nPart);
			if (cStackerUnit.first == nPart) { //Selected 된 것.
				bIsRemain = TRUE;
				MakeLog(_T("[MainStep=%s] [SubStep=%d] [Unload Tray Pp Use C-Trya Jig]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
				ChangeSubState(eExcutingStep_ZTeaching_End);
				break;
			}
		}

		if(bIsRemain)
			break;

		ChangeSubState(eExcutingStep_ZTeaching_Move_Stacker_Unload);
	}break;
	case eExcutingStep_ZTeaching_Move_Stacker_Unload:
	{
		int ntest = g_TaskStacker[eSTACKER_03].GetSeqCmdStatus();
		if (g_TaskStacker[eSTACKER_03].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}
		
		CBaseSeqCmd::eSeqCmdStatus feeder_cmd_status = (CBaseSeqCmd::eSeqCmdStatus)g_TaskTrayFeeder.GetSeqCmdStatus();
		if (feeder_cmd_status != CBaseSeqCmd::eSeqCmdStatus_Ready)
			break;

		CStackerCmd_Unload* pSeqCmd = new CStackerCmd_Unload(true);
		if (SendCommand(g_TaskStacker[eSTACKER_03], pSeqCmd) == eSeqCmdAck_OK) {
			MakeLog(_T("[MainStep=%s] [SubStep=%d] [Stacker #3 Unload]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
			ChangeSubState(eExcutingStep_ZTeaching_Move_Stacker_Up);
		}
	}break;
	case eExcutingStep_ZTeaching_Move_Stacker_Up:
	{
		if (g_TaskStacker[eSTACKER_03].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			break;
		}

		int nErr = g_TaskStacker[eSTACKER_03].ChkExistTray_FrontPos_Sen(DEF_EXIST);
		if (nErr == ERR_NO_ERROR) {
			CStackerCmd_TrayUp* pSeqCmd = new CStackerCmd_TrayUp;
			if (SendCommand(g_TaskStacker[eSTACKER_03], pSeqCmd) == eSeqCmdAck_OK) {
				MakeLog(_T("[MainStep=%s] [SubStep=%d] [Stacker #3 Up]"), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);
				ChangeSubState(eExcutingStep_ZTeaching_End);
			}
		}
		else {
			_Base_AlarmReport_VAT(ERR_VAT_TRAY_PP_STACKER_FRONT_POS_JIG_NOT_EXIST, _T("Stacker 3"));
		}
	}break;
	case eExcutingStep_ZTeaching_End:
	{		
		//SetFinishFlag(TRUE);
		if (m_nCurLoc == VAT_IDX_TRAY_PP_FEEDER) {
			if (g_TaskStacker[eSTACKER_03].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				break;
			}
		}



		MakeLog(_T("[Mode=%s] [MainStep=%s] [SubStep=%d] [End]"), GetExcuteModeStr(m_nSeqMode), GetLotStateToString(m_nAutoRunMainStep), m_nAutoRunSubStep);

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
