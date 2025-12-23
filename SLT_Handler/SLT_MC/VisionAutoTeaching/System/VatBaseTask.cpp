#include "stdafx.h"
#include "VatBaseTask.h"

CVatBaseTask::CVatBaseTask()
{
// 	m_vLoadTrayPPParts.clear();	
// 	m_vLoadTrayPPParts.reserve(VAT_LOAD_TRAYPP_LOC_MAX);
// 	m_vUnloadTrayPPParts.clear();	
// 	m_vUnloadTrayPPParts.reserve(VAT_UNLOAD_TRAYPP_LOC_MAX);
// 
// 	for (int i = 0; i < eMaxTestPPCount; i++)
// 	{
// 		m_vTestPPParts[i].clear();
// 		m_vTestPPParts[i].reserve(VAT_TEST_PP_LOC_MAX);
// 	}

	m_hVatMainGui = NULL;

	m_nSeqMode = -1;
	m_nHand = -1;
	m_nCurLoc = -1;
	

	//m_vSelectedParts.clear();
	m_nZAutoTeachingSpeed = eZSpeed_None;


//	m_cSelectedHand.clear();
	m_cSelectedPart.clear();

	m_dPrevPosX = 0.0;
	m_dPrevPosY = 0.0;
	m_nVisionRetryCnt = 0;
//	m_nPkrAutoFocusCnt = 0;

	m_dEachCaseRate = 0.0;
	m_dThresholdProgressRate = 0.0;

	m_nLastPickerForProgress = -1;

	m_nTableTargetIdx = eTableTarget_1;
	memset(m_dTblTargetPos, 0.0, sizeof(m_dTblTargetPos));
}


CVatBaseTask::~CVatBaseTask()
{
}



/*=============================================================================
AUTHOR   : [4/16/2024 dohyeong.kim]
NAME     :
PARAMS   :
RETURN   : 
COMMENTS : Part 상태 변경, 
           측정 완료 or 측정 미완료 or 측정 대기
==============================================================================*/
//void CVatBaseTask::SetPartStatus(int nCurLoc, int part_status)
//{	
	/* 잠시 주석
	for (auto nPart : m_vSelectedParts)
	{
		if (nPart.first == nCurLoc) {
			std::replace(m_vSelectedParts.begin(), m_vSelectedParts.end(), nPart, std::make_pair(nCurLoc, part_status));
			return;
		}
  	}
	*/

// 	for (int nPart = 0; nPart < m_vSelectedParts.size(); nPart++)
// 	{
// 		if (m_vSelectedParts[nPart].first == nCurLoc) {
// 			m_vSelectedParts[nPart].second = part_status;
// 			return;
// 		}
// 	}

//}

//int CVatBaseTask::GetPartStatus(int nCurLoc)
//{
//	int nRet = ERR_VAT_ERROR;

	/* 잠시 주석
	for (auto nPart : m_vSelectedParts)
	{
		if (nPart.first == nCurLoc) {
			nRet = nPart.second;
			break;
		}
	}
	*/ 

//	return nRet;
//}




// int CVatBaseTask::SetLotCommand(eAutoLotCmd eLotCmd)
// {
// 	//나머지 Auto Sequence 추가.
// 	BOOL bChkErr = FALSE;
// 	int nErr = g_VatTaskAutoSystem.OnLotCommand(eLotCmd, NULL, NULL);
// 	if (nErr != ERR_NO_ERROR) {
// 		bChkErr = TRUE;
// 	}
// 
// 	nErr = g_VatTaskAutoTrayLoadPP.OnLotCommand(eLotCmd, NULL, NULL);
// 	if (nErr != ERR_NO_ERROR) {
// 		bChkErr = TRUE;
// 	}
// 
// 	if (bChkErr == TRUE) {
// 		g_START_SW.SetStatus(FALSE);
// 		g_STOP_SW.SetStatus(TRUE);
// 		return ERR_LOT_CMD_CANNOT_PERFORM_NOW;
// 	}
// 
// 	return ERR_NO_ERROR;
// }

void CVatBaseTask::SetVatMainGuiHandle(HWND hOwner)
{
	m_hVatMainGui = hOwner;
}

HWND CVatBaseTask::GetVatMainGuiHandle()
{
	return m_hVatMainGui;
}


void CVatBaseTask::_Base_SetZAutoTeachingSpd(int eSpd)
{
	m_nZAutoTeachingSpeed = eSpd;
}

int CVatBaseTask::_Base_GetZAutoTeachingSpd()
{
	return m_nZAutoTeachingSpeed;
}

CString CVatBaseTask::GetExcuteModeStr(int nMode)
{
	CString strRet = _T("");
	switch (nMode)
	{
	case VAT_SEQ_MODE_Z_FOCUS:			strRet = _T("Z AUTO FOCUS");			break;
	case VAT_SEQ_MODE_CAM_CAL:			strRet = _T("CAM CENTER CALIBRATION");	break;
	case VAT_SEQ_MODE_PICKER_ALIGN:		strRet = _T("PICKER ALIGN");			break;
	case VAT_SEQ_MODE_VAT:				strRet = _T("VISION AUTO TEACHING");	break;
	case VAT_SEQ_MODE_Z_TEACHING:		strRet = _T("Z AUTO TEACHING");			break;
	default:							strRet = _T("NONE");					break;
	}

	return strRet;
}





//Part Index로 Hand Index return
// int CVatBaseTask::GetSelectedHand(int nPart)
// {
// 	int nRet = 0;
// 	//VAT_LOC_MAX
// 	if (VAT_LOAD_TRAYPP_FEEDER <= nPart && nPart <= VAT_LOAD_TRAYPP_TABLE2)						nRet = VAT_HAND_LOAD_TRAYPP;
// 	else if (VAT_UNLOAD_TRAYPP_TRAY2 <= nPart && nPart <= VAT_UNLOAD_TRAYPP_TABLE2)				nRet = VAT_HAND_UNLOAD_TRAYPP;
// 	else if (VAT_LOAD_TESTPP1_STATION1 <= nPart && nPart <= VAT_UNLOAD_TESTPP1_CLEAN_TABLE)		nRet = VAT_HAND_TESTPP1;
// 	else																						nRet = VAT_HAND_TESTPP2;
// 
// 	return nRet;
// }




CPoint CVatBaseTask::_Base_GetStdPicker()
{
	ASSERT(VAT_HAND_LOAD_TRAYPP <= m_nHand && m_nHand <= VAT_HAND_UNLOAD_TESTPP2);
	
	CPoint cStdPicker;
	cStdPicker.x = g_VATCommonStaticData.m_stVATHandCommonData.pairStdPicker[m_nHand].first;
	cStdPicker.y = g_VATCommonStaticData.m_stVATHandCommonData.pairStdPicker[m_nHand].second;

	return cStdPicker;
}


/*
	(0, 0) (1, 0) (2, 0) (3, 0)
	(0, 1) (1, 1) (2, 1) (3, 1)
*/
CPoint CVatBaseTask::_Base_ConvertPickerIdxToPoint(int nPkrIdx)
{
	ASSERT(VAT_PICKER_01 <= nPkrIdx && nPkrIdx <= VAT_PICKER_08);

	CPoint cPicker;

	cPicker.x = VAT_PICKER_X(nPkrIdx);
	cPicker.y = VAT_PICKER_Y(nPkrIdx);

	return cPicker;
}


std::pair<double, double> CVatBaseTask::_Base_CalPitchTargetPosXY(int nPitchType, int nPickerIdx)
{
	ASSERT(nPitchType != -1 && nPickerIdx != -1); //-1이 될 수 없음.

	//1. 기준 Picker
	CPoint cStdPicker = _Base_GetStdPicker();

	//2. Current Picker
	CPoint cCurPicker = _Base_ConvertPickerIdxToPoint(nPickerIdx);

	//3. Move Step
	//Hand Motor 동작 방향에 따라 move_step 방향 정의.
	int move_x_step = 0;
	int move_y_step = 0;
	switch (m_nHand)
	{
		case VAT_HAND_LOAD_TRAYPP:
		{
			move_x_step = cStdPicker.x - cCurPicker.x;
			move_y_step = cStdPicker.y - cCurPicker.y;
		}break;
		case VAT_HAND_UNLOAD_TRAYPP:
		{
			move_x_step = cStdPicker.x - cCurPicker.x;
			move_y_step = cCurPicker.y - cStdPicker.y;
		}break;
		case VAT_HAND_LOAD_TESTPP1:
		case VAT_HAND_UNLOAD_TESTPP1:
		{
			move_x_step = cCurPicker.x - cStdPicker.x;
			move_y_step = cCurPicker.y - cStdPicker.y;
		}break;
		case VAT_HAND_LOAD_TESTPP2:
		case VAT_HAND_UNLOAD_TESTPP2:
		{
			move_x_step = cStdPicker.x - cCurPicker.x;
			move_y_step = cStdPicker.y - cCurPicker.y;
		}break;
		default:
			break;
	}

	//4. Hand Pitch
	double dHandPitchX = 0.0;
	double dHandPitchY = 0.0;

	if (m_nHand == VAT_HAND_LOAD_TRAYPP || m_nHand == VAT_HAND_UNLOAD_TRAYPP) {
		if (nPitchType == VAT_PITCH_TYPE_WIDE) {
			dHandPitchX = g_DMCont.m_dmTrayPP.GN(NDM5_TrayPP_LdTbl_Pitch_X);
			dHandPitchY = g_DMCont.m_dmTrayPP.GN(NDM5_TrayPP_LdTbl_Pitch_Y);
		}
		else { //nPitchType == VAT_PITCH_TYPE_NARROW
			dHandPitchX = g_DMCont.m_dmTrayPP.GN(NDM5_Tray_Pitch_X);
			dHandPitchY = g_DMCont.m_dmTrayPP.GN(NDM5_Tray_Pitch_Y);

			if (dHandPitchX < VAT_HAND_MINUMUM_X_PITCH){
				dHandPitchX *= 2;
			}
			if (dHandPitchY < VAT_HAND_MINUMUM_Y_PITCH) {
				dHandPitchY *= 2;
			}
		}
	}
	else { //Test PP
		if (nPitchType == VAT_PITCH_TYPE_WIDE) {
			dHandPitchX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Pitch_X); //46mm[Station]
			dHandPitchY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Pitch_Y); //60mm[Station]
		}
		else { //nPitchType == VAT_PITCH_TYPE_NARROW
			dHandPitchX = g_DMCont.m_dmTestPP.GN(NDM4_TestPP_Pitch_X); //46mm[Loading Table]
			dHandPitchY = g_DMCont.m_dmTestPP.GN(NDM4_TestPP_Pitch_Y); //42mm[Loading Table]
		}
	}

	//5. Calculate Target Position
	double dStdPkrPosX = g_VATCommonData.m_stVATCamCenterCalInfo.dBotCameraPosition[m_nHand][VAT_X];
	double dStdPkrPosY = g_VATCommonData.m_stVATCamCenterCalInfo.dBotCameraPosition[m_nHand][VAT_Y];

	double dTargetPosX = 0.0;
	double dTargetPosY = 0.0;

	if (m_nHand == VAT_HAND_LOAD_TRAYPP || m_nHand == VAT_HAND_UNLOAD_TRAYPP) {
		dTargetPosX = dStdPkrPosX + (move_x_step*dHandPitchX);
		dTargetPosY = dStdPkrPosY + (move_y_step*dHandPitchY);
	}
	else {
		//※※※Test PP의 경우 계산되는 Pitch와 실제 장비 정면에서 Pitch가 반대.※※※
		dTargetPosX = dStdPkrPosX + (move_y_step*dHandPitchY);
		dTargetPosY = dStdPkrPosY + (move_x_step*dHandPitchX);
	}
	
	std::pair<double, double> p;
	p.first = dTargetPosX;
 	p.second = dTargetPosY;

	return p;
}


std::pair<double, double> CVatBaseTask::_Base_GetPitchXYTeachPos(int nHand, int nPitchType)
{
	//Hand Pitch 정보 가져옴.
	ST_VAT_HAND_PITCH_INFO HandPitchInfo;

	auto it = cHandPitchParamMap.find(m_nHand);
	if (it != cHandPitchParamMap.end()) {
		HandPitchInfo = it->second;
	}
	else {
		ASSERT(0);
	}

	//[XP, YP] - Teaching Table에서 가져옴.
	int nLoc = -1;
	if (m_nHand == VAT_HAND_LOAD_TRAYPP || m_nHand == VAT_HAND_UNLOAD_TRAYPP) {
		if (nPitchType == VAT_PITCH_TYPE_WIDE) {
			nLoc = (m_nHand == VAT_HAND_LOAD_TRAYPP) ? LOC_IDX_TRAY_LOAD_PP_TBL_2 : LOC_IDX_TRAY_UNLOAD_PP_TBL_1;
		}
		else { //nPitchType == VAT_PITCH_TYPE_NARROW
			nLoc = (m_nHand == VAT_HAND_LOAD_TRAYPP) ? LOC_IDX_TRAY_LOAD_PP_FEEDER : LOC_IDX_TRAY_UNLOAD_PP_CONV_4;
		}
	}
	else { //Test PP
		if (nPitchType == VAT_PITCH_TYPE_WIDE) {
			nLoc = LOC_IDX_STATION_1;
		}
		else { //nPitchType == VAT_PITCH_TYPE_NARROW
			nLoc = LOC_IDX_TEST_PP_TBL_1_LD;
		}
	}

	std::pair<double, double> p;
	p.first = g_DMCont.m_Teach.GetTeachPos(HandPitchInfo.robot_id, nLoc, HandPitchInfo.xp_pos_idx);
	p.second = g_DMCont.m_Teach.GetTeachPos(HandPitchInfo.robot_id, nLoc, HandPitchInfo.yp_pos_idx);

	return p;
}

//=====================================================================================
// COMMENT : 각각 Mode에 해당 하는 Base Target Position 생성 함수.
//			 해당 함수에서 모든 Base Position 관리 하며, Seq에 해당하는 구조체만 관리.( ex. m_stVATZFocusInfoSeq )
// PARAM1 : 이동 하고자 하는 Target Location 위치. (대상 위치)
// PARAM2 : 이동 하고자 하는 Target Position 위치값 (대상 위치의 좌표 값)
//=====================================================================================
void CVatBaseTask::_Base_GetSeqTargetPos(double dTPos[], _eVAT_MODE_EACH_DATA_TYPE _eType /*= eModeEachDataType_None*/)
{
	//나중에 필요한 곳만 사용.
	//Hand Pitch 정보 가져옴.
	ST_VAT_HAND_PITCH_INFO HandPitchInfo;

	auto it = cHandPitchParamMap.find(m_nHand);
	if (it != cHandPitchParamMap.end()) {	
		HandPitchInfo = it->second;
	}
	else {
		ASSERT(0);
	}

	switch (m_nSeqMode)
	{
		case VAT_SEQ_MODE_Z_FOCUS:
		{
			//X, Y, Z
			for (int i = 0; i < VAT_MAX_AXIS_XYZ; i++) {
				if (_eType == eZFocus_PlateJigTargetPos)
				{
					dTPos[i] = g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_TargetPos[m_nHand][i];				
				}
				else if (_eType == eZFocus_PlateJigVacPos)
				{
					dTPos[i] = g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_VacuumPos[m_nHand][i];
				}
			}

			//X-Pitch, Y-Pitch
			dTPos[VAT_PITCH_X] = g_Motor[HandPitchInfo.xp_axis_no]->GetSwLimitPosi() - 1000.0;
			dTPos[VAT_PITCH_Y] = g_Motor[HandPitchInfo.yp_axis_no]->GetSwLimitPosi() - 1000.0;
		}break;
 		case VAT_SEQ_MODE_CAM_CAL:
 		{
			//X, Y, Z
			for (int i = 0; i < VAT_MAX_AXIS_XYZ; i++) {
				dTPos[i] = g_VATCommonData.m_stVATCamCenterCalInfoSeq.dBotCameraPosition[m_nHand][i];
			}

			//※Z축 위치 조정.
			//eCamCenCal_BotCamCenterPos아니면 Safety Position
			if (_eType == eModeEachDataType_None) {
				dTPos[VAT_Z] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_1), LOC_IDX_TRAY_LOAD_PP_SAFETY, POS_IDX_TRAY_LOAD_PP_Z_SAFETY);
			}

			//X-Pitch, Y-Pitch
			dTPos[VAT_PITCH_X] = g_Motor[HandPitchInfo.xp_axis_no]->GetSwLimitPosi() - 1000.0;
			dTPos[VAT_PITCH_Y] = g_Motor[HandPitchInfo.yp_axis_no]->GetSwLimitPosi() - 1000.0;
 		}break;
 		case VAT_SEQ_MODE_PICKER_ALIGN:
 		{
			//[X, Y, Z]
			int nPitchType = m_stCurPicker.nPitchType;
			int nPickerIdx = m_stCurPicker.nPickerIdx;

			dTPos[VAT_X] = g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerPos[m_nHand][nPitchType][nPickerIdx].first;
			dTPos[VAT_Y] = g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerPos[m_nHand][nPitchType][nPickerIdx].second;

			if (_eType == ePickerAlign_BeforePickerSetPos) {
				dTPos[VAT_Z] = g_VATCommonData.m_stVATCamCenterCalInfo.dBotCameraPosition[m_nHand][VAT_Z]; //기준 Picker Cam Center Position 초점 맞는 높이.
			}
			else if(_eType == ePickerAlign_AfterPickerSetPos){
				dTPos[VAT_Z] = g_VATCommonData.m_stVATPickerAlignInfoSeq.dPickerZFocusPos[m_nHand][nPickerIdx];
			}
			else {//_eType == eModeEachDataType_None
				dTPos[VAT_Z] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_1), LOC_IDX_TRAY_LOAD_PP_SAFETY, POS_IDX_TRAY_LOAD_PP_Z_SAFETY);
			}
			
			std::pair<double, double> pPitchTeachPos = _Base_GetPitchXYTeachPos(m_nHand, nPitchType);

			//X-Pitch, Y-Pitch
			dTPos[VAT_PITCH_X] = pPitchTeachPos.first;
			dTPos[VAT_PITCH_Y] = pPitchTeachPos.second;
 		}break;
 		case VAT_SEQ_MODE_VAT:
 		{
			//다른 Mode와 동일하게 Hand관련 Base Position만 관리.
			for (int i = 0; i < VAT_MAX_AXIS_XYZ; i++) {
				dTPos[i] = g_VATCommonData.m_stVATInfoSeq.stHandBasePos.getBasePos(m_nHand, m_nCurLoc, i);
			}

			//X-Pitch, Y-Pitch
			dTPos[VAT_PITCH_X] = g_Motor[HandPitchInfo.xp_axis_no]->GetSwLimitPosi() - 1000.0;
			dTPos[VAT_PITCH_Y] = g_Motor[HandPitchInfo.yp_axis_no]->GetSwLimitPosi() - 1000.0; 		
 		}break;
		case VAT_SEQ_MODE_Z_TEACHING:
		{
			//얘는 Z Auto Focus Move 검증 후에 코드 변경 하자
			//아직 사용 안함.

			//X, Y, Z
			double dCmdPos[VAT_MAX_AXIS] = { 0, };
			for (int i = 0; i < VAT_MAX_AXIS_XYZ; i++) {
				dCmdPos[i] = g_VATCommonData.m_stVATZTeachInfoSeq.stZTeachBasePos.getPartBasePos(m_nHand, m_nCurLoc, i);
			}

			// LD/UD Tray PP만 m_nCurLoc Convert 필요함.
			CTaskTrayBasePP* pTaskTray = nullptr;
			if (m_nHand == VAT_HAND_LOAD_TRAYPP) {
				pTaskTray = &g_TaskTrayLoadPP;
			}
			else if (m_nHand == VAT_HAND_UNLOAD_TRAYPP) {
				pTaskTray = &g_TaskTrayUnloadPP;
			}

			if (pTaskTray != nullptr) {
				int nLocStageConv = pTaskTray->TrayHandLocIdxConvertor((m_nHand == VAT_HAND_LOAD_TRAYPP) ? RIDX_TRAY_PP_1: RIDX_TRAY_PP_2, m_nCurLoc);
				m_nCurLoc = nLocStageConv;
			}

			//X-Pitch, Y-Pitch
			dCmdPos[VAT_PITCH_X] = g_DMCont.m_Teach.GetTeachPos(HandPitchInfo.robot_id, m_nCurLoc, HandPitchInfo.xp_pos_idx);
			dCmdPos[VAT_PITCH_Y] = g_DMCont.m_Teach.GetTeachPos(HandPitchInfo.robot_id, m_nCurLoc, HandPitchInfo.yp_pos_idx);
		}break;
		default:
		{
			ASSERT(0);			
		}break;
	}
}


//=====================================================================================
// COMMENT : Vision Auto Teaching Unit-Only 측정 시, 사용 되는 Position 정의.
//			 현재 Table Position만 사용하고 있음.
//=====================================================================================
void CVatBaseTask::_Base_GetSeqTargetPos_Unit(double dTPos[], int nVATPart, _eVAT_PART_ONLY_POS _ePos)
{
	int nMAX_AXIS = (nVATPart == VAT_PART_FEEDER) ? VAT_MAX_AXIS_Y : VAT_MAX_AXIS_YROTATE;

	for (int nAxis = 0; nAxis < nMAX_AXIS; nAxis++) {
		dTPos[nAxis] = g_VATCommonData.m_stVATInfoSeq.stPartBasePos.getBasePos(nVATPart, (int)_ePos, nAxis);
	}
}


//=====================================================================================
// COMMENT : 각각 Hand에 해당 하는 Safety Position 생성 함수.
//			 해당 함수에서 모든 Hand Safety Position 관리 하며, Teaching Data Safety Pos를 기준으로 함
// PARAM1 : 선택 핸드 Safety 위치.
//=====================================================================================
void CVatBaseTask::_Base_GetSafetyPos(double dTPos[])
{
	switch (m_nHand)
	{
	case VAT_HAND_LOAD_TRAYPP:
	{
		dTPos[VAT_X] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_1), LOC_IDX_TRAY_LOAD_PP_SAFETY, ePOS_IDX_TRAY_LOAD_PP::POS_IDX_TRAY_LOAD_PP_X);
		dTPos[VAT_Y] = g_DMCont.m_Teach.GetTeachPos(RID_FEEDER_(RIDX_FEEDER), LOC_IDX_FEEDER_SAFETY, ePOS_IDX_FEEDER::POS_IDX_FEEDER_Y);
		dTPos[VAT_Z] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_1), LOC_IDX_TRAY_LOAD_PP_SAFETY, ePOS_IDX_TRAY_LOAD_PP::POS_IDX_TRAY_LOAD_PP_Z_SAFETY);
		dTPos[VAT_PITCH_X] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_1), LOC_IDX_TRAY_LOAD_PP_SAFETY, ePOS_IDX_TRAY_LOAD_PP::POS_IDX_TRAY_LOAD_PP_X_PITCH);
		dTPos[VAT_PITCH_Y] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_1), LOC_IDX_TRAY_LOAD_PP_SAFETY, ePOS_IDX_TRAY_LOAD_PP::POS_IDX_TRAY_LOAD_PP_Y_PITCH);
	}break;
	case VAT_HAND_UNLOAD_TRAYPP:
	{
		dTPos[VAT_X] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_2), LOC_IDX_TRAY_UNLOAD_PP_SAFETY, ePOS_IDX_TRAY_PP::POS_IDX_TRAY_PP_X);
		dTPos[VAT_Y] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_2), LOC_IDX_TRAY_UNLOAD_PP_SAFETY, ePOS_IDX_TRAY_PP::POS_IDX_TRAY_PP_Y);
		dTPos[VAT_Z] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_2), LOC_IDX_TRAY_UNLOAD_PP_SAFETY, ePOS_IDX_TRAY_PP::POS_IDX_TRAY_PP_Z_SAFETY);
		dTPos[VAT_PITCH_X] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_2), LOC_IDX_TRAY_UNLOAD_PP_SAFETY, ePOS_IDX_TRAY_PP::POS_IDX_TRAY_PP_X_PITCH);
		dTPos[VAT_PITCH_Y] = g_DMCont.m_Teach.GetTeachPos(RID_TRAY_PP_(RIDX_TRAY_PP_2), LOC_IDX_TRAY_UNLOAD_PP_SAFETY, ePOS_IDX_TRAY_PP::POS_IDX_TRAY_PP_Y_PITCH);
	}break;
	case VAT_HAND_LOAD_TESTPP1:
	{
		dTPos[VAT_X] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(RIDX_TEST_PP_1), LOC_IDX_TEST_PP_HOME, ePOS_IDX_TEST_PP::POS_IDX_TEST_PP_X1);
		dTPos[VAT_Y] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(RIDX_TEST_PP_1), LOC_IDX_TEST_PP_HOME, ePOS_IDX_TEST_PP::POS_IDX_TEST_PP_Y1);
		dTPos[VAT_Z] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(RIDX_TEST_PP_1), LOC_IDX_TEST_PP_HOME, ePOS_IDX_TEST_PP::POS_IDX_TEST_PP_Z1_SAFETY);
		dTPos[VAT_PITCH_X] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(RIDX_TEST_PP_1), LOC_IDX_TEST_PP_HOME, ePOS_IDX_TEST_PP::POS_IDX_TEST_PP_X1_PITCH);
		dTPos[VAT_PITCH_Y] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(RIDX_TEST_PP_1), LOC_IDX_TEST_PP_HOME, ePOS_IDX_TEST_PP::POS_IDX_TEST_PP_Y1_PITCH);
	}break;
	case VAT_HAND_UNLOAD_TESTPP1:
	{
		dTPos[VAT_X] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(RIDX_TEST_PP_1), LOC_IDX_TEST_PP_HOME, ePOS_IDX_TEST_PP::POS_IDX_TEST_PP_X2);
		dTPos[VAT_Y] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(RIDX_TEST_PP_1), LOC_IDX_TEST_PP_HOME, ePOS_IDX_TEST_PP::POS_IDX_TEST_PP_Y2);
		dTPos[VAT_Z] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(RIDX_TEST_PP_1), LOC_IDX_TEST_PP_HOME, ePOS_IDX_TEST_PP::POS_IDX_TEST_PP_Z2_SAFETY);
		dTPos[VAT_PITCH_X] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(RIDX_TEST_PP_1), LOC_IDX_TEST_PP_HOME, ePOS_IDX_TEST_PP::POS_IDX_TEST_PP_X2_PITCH);
		dTPos[VAT_PITCH_Y] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(RIDX_TEST_PP_1), LOC_IDX_TEST_PP_HOME, ePOS_IDX_TEST_PP::POS_IDX_TEST_PP_Y2_PITCH);
	}break;
	case VAT_HAND_LOAD_TESTPP2:
	{
		dTPos[VAT_X] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(RIDX_TEST_PP_2), LOC_IDX_TEST_PP_HOME, ePOS_IDX_TEST_PP::POS_IDX_TEST_PP_X1);
		dTPos[VAT_Y] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(RIDX_TEST_PP_2), LOC_IDX_TEST_PP_HOME, ePOS_IDX_TEST_PP::POS_IDX_TEST_PP_Y1);
		dTPos[VAT_Z] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(RIDX_TEST_PP_2), LOC_IDX_TEST_PP_HOME, ePOS_IDX_TEST_PP::POS_IDX_TEST_PP_Z1_SAFETY);
		dTPos[VAT_PITCH_X] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(RIDX_TEST_PP_2), LOC_IDX_TEST_PP_HOME, ePOS_IDX_TEST_PP::POS_IDX_TEST_PP_X1_PITCH);
		dTPos[VAT_PITCH_Y] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(RIDX_TEST_PP_2), LOC_IDX_TEST_PP_HOME, ePOS_IDX_TEST_PP::POS_IDX_TEST_PP_Y1_PITCH);
	}break;
	case VAT_HAND_UNLOAD_TESTPP2:
	{
		dTPos[VAT_X] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(RIDX_TEST_PP_2), LOC_IDX_TEST_PP_HOME, ePOS_IDX_TEST_PP::POS_IDX_TEST_PP_X2);
		dTPos[VAT_Y] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(RIDX_TEST_PP_2), LOC_IDX_TEST_PP_HOME, ePOS_IDX_TEST_PP::POS_IDX_TEST_PP_Y2);
		dTPos[VAT_Z] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(RIDX_TEST_PP_2), LOC_IDX_TEST_PP_HOME, ePOS_IDX_TEST_PP::POS_IDX_TEST_PP_Z2_SAFETY);
		dTPos[VAT_PITCH_X] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(RIDX_TEST_PP_2), LOC_IDX_TEST_PP_HOME, ePOS_IDX_TEST_PP::POS_IDX_TEST_PP_X2_PITCH);
		dTPos[VAT_PITCH_Y] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(RIDX_TEST_PP_2), LOC_IDX_TEST_PP_HOME, ePOS_IDX_TEST_PP::POS_IDX_TEST_PP_Y2_PITCH);
	}break;
	default:
		break;
	}

}


void CVatBaseTask::_Base_MakePrePos(double dTPos[])
{
	double dSafetyPos[VAT_MAX_AXIS] = { 0, };
	_Base_GetSafetyPos(dSafetyPos);

	//[Make Pre Position] - X, Y, Z
	dTPos[VAT_X] -= VAT_PRE_POS_100mm_OFFSET;
	dTPos[VAT_Y] -= VAT_PRE_POS_100mm_OFFSET;
	dTPos[VAT_Z] = dSafetyPos[VAT_Z];
}


//=====================================================================================
// COMMENT : Hand Move시, Limit일 경우 Alarm 발생 하지 않고 Target Position 재생성 하는 함수.
//           ex) Move Pre Position에서 사용.
//=====================================================================================
void CVatBaseTask::_Base_CheckandCreateLimitPos(double dTPos[], int nAxis /*= VAT_MAX_AXIS_XY*/)
{
	//Hand XYZ 정보 가져옴.
	ST_VAT_HAND_XYZ_INFO HandXYZInfo;

	auto it = cHandXYZParamMap.find(m_nHand);
	if (it != cHandXYZParamMap.end()) {
		HandXYZInfo = it->second;
	}
	else {
		ASSERT(0);
	}

	auto _MakeLimitPos = [](double& dTPos, double dPosLimit, double dNegLimit) {
		if (dTPos > dPosLimit) dTPos = dPosLimit - 1000.0;
		else if (dTPos < dNegLimit) dTPos = dNegLimit + 1000.0;
	};

	if (nAxis == VAT_X || nAxis == VAT_MAX_AXIS_XY) 
	{
		double X_POSITIVE_LIMIT = g_Motor[HandXYZInfo.x_axis_no]->GetSwLimitPosi();
		double X_NEGATIVE_LIMIT = g_Motor[HandXYZInfo.x_axis_no]->GetSwLimitNega();
		_MakeLimitPos(dTPos[VAT_X], X_POSITIVE_LIMIT, X_NEGATIVE_LIMIT);
	}

	if (nAxis == VAT_Y || nAxis == VAT_MAX_AXIS_XY)
	{
		double Y_POSITIVE_LIMIT = g_Motor[HandXYZInfo.y_axis_no]->GetSwLimitPosi();
		double Y_NEGATIVE_LIMIT = g_Motor[HandXYZInfo.y_axis_no]->GetSwLimitNega();
		_MakeLimitPos(dTPos[VAT_Y], Y_POSITIVE_LIMIT, Y_NEGATIVE_LIMIT);
	}
}


void CVatBaseTask::_Base_CheckandCreateLimitPos_Unit(double dTPos[], int nVATPart)
{
	ST_VAT_PART_AXIS_INFO stPartAxisInfo;

	auto it = cPartAxisParamMap.find(nVATPart);
	if (it != cPartAxisParamMap.end()) {
		stPartAxisInfo = it->second;
	}
	else {
		ASSERT(0);
	}

	auto _MakeLimitPos = [](double& dTPos, double dPosLimit, double dNegLimit) {
		if (dTPos > dPosLimit) dTPos = dPosLimit - 1000.0;
		else if (dTPos < dNegLimit) dTPos = dNegLimit + 1000.0;
	};

	double Y_POSITIVE_LIMIT = g_Motor[stPartAxisInfo.axis_no_1]->GetSwLimitPosi();
	double Y_NEGATIVE_LIMIT = g_Motor[stPartAxisInfo.axis_no_1]->GetSwLimitNega();
	_MakeLimitPos(dTPos[VAT_PART_Y], Y_POSITIVE_LIMIT, Y_NEGATIVE_LIMIT);

	if (!(nVATPart == VAT_PART_FEEDER))
	{
		double ROTATE_POSITIVE_LIMIT = g_Motor[stPartAxisInfo.axis_no_2]->GetSwLimitPosi();
		double ROTATE_NEGATIVE_LIMIT = g_Motor[stPartAxisInfo.axis_no_2]->GetSwLimitNega();
		_MakeLimitPos(dTPos[VAT_PART_ROTATE], ROTATE_POSITIVE_LIMIT, ROTATE_NEGATIVE_LIMIT);
	}
}


//====================================================================
//COMMENT : Z Height Cal 진행 시, Z축 Position이 Jig상에 과도하게 누름 방지를 위함.
//Param   : Z Height Cal Position
//====================================================================

void CVatBaseTask::_Base_CheckandCreateZHeightCalPrePos(double &dZPos)
{
	double dZPrePos = dZPos - VAt_Z_HEIGHT_CAL_PRE_POS_OFFSET;

	//Hand XYZ 정보 가져옴.
	ST_VAT_HAND_XYZ_INFO HandXYZInfo;

	auto it = cHandXYZParamMap.find(m_nHand);
	if (it != cHandXYZParamMap.end()) {
		HandXYZInfo = it->second;
	}
	else {
		ASSERT(0);
	}

	double Z_NEGATIVE_LIMIT = g_Motor[HandXYZInfo.z_axis_no]->GetSwLimitNega();
	if (dZPrePos > Z_NEGATIVE_LIMIT) {
		dZPos = dZPrePos;
	}
}

//====================================================================
//[10/16/2025 dohyeong.kim]
//COMMENT : Hand/Picker/Part Type 통합 상태 변경 함수.
//Param1  : 변경 하고자 하는 상태
//Param2  : vector에 담겨 있는 상태와 동일한지 전체 검사 후, 상태 변경.
//          vector.size() == 0 일 경우, 무조건 상태 변경.
//====================================================================
void CVatBaseTask::_Base_SetMeasure_Status(_eVAT_MeasureStatus nUpdateStatus, std::vector<int> vStatus /*= {}*/)
{
	auto IsStatusInclude = [](const std::vector<int> &vStatus, int nStatus) -> bool {
		return std::find(vStatus.begin(), vStatus.end(), nStatus) != vStatus.end(); //값이 있으면 true Return
	};

	int nModeType = g_VatTaskSystemCtrl.GetSeqCurModeType();
	//Hand Type
	if (nModeType == VAT_MODE_TYPE_HAND) {
		int nStatus = _Base_GetMeasureHand_Status();

		if(vStatus.empty() || IsStatusInclude(vStatus, nStatus)){
			_Base_SetMeasureHand_Status(nUpdateStatus);
		}
	}
	else if (nModeType == VAT_MODE_TYPE_PICKER) {
		for (int nPitchType = 0; nPitchType < VAT_PITCH_TYPE_MAX; nPitchType++) {
			for (int nPickerIdx = 0; nPickerIdx < VAT_PICKER_MAX; nPickerIdx++) {
				int nStatus = _Base_GetMeasurePicker_Status(nPitchType, nPickerIdx);
				if (vStatus.empty() || IsStatusInclude(vStatus, nStatus)) {
					_Base_SetMeasurePicker_Status(nPitchType, nPickerIdx, nUpdateStatus);
				}
			}
		}
	}
	//Part Type
	else {
		//Part중, OnGoing or Wait있을 경우 상태 Ready로 변경.(Abort일 경우)
		int nSTART_PART_IDX = (m_nHand == VAT_HAND_LOAD_TRAYPP || m_nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_IDX_TRAY_PP_TBL_1 : VAT_IDX_STATION_1;
		int nMAX_PART_IDX = (m_nHand == VAT_HAND_LOAD_TRAYPP || m_nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;

		for (int nPart = nSTART_PART_IDX; nPart < nMAX_PART_IDX; nPart++) {
			int nStatus = _Base_GetMeasurePart_Status(nPart);
			if (vStatus.empty() || IsStatusInclude(vStatus, nStatus)) {
				_Base_SetMeasurePart_Status(nPart, nUpdateStatus);
			}
// 			if (nStatus == VAT_STATUS_MEASURE_ONGOING || nStatus == VAT_STATUS_MEASURE_WAIT) {
// 				_Base_SetMeasurePart_Status(nPart, VAT_STATUS_MEASURE_READY);
// 			}
		}
	}
}


BOOL CVatBaseTask::_Base_IsAllMeasureSameStatus(_eVAT_MeasureStatus status)
{
	BOOL bRet = FALSE;

	int nModeType = g_VatTaskSystemCtrl.GetSeqCurModeType();

	if (nModeType == VAT_MODE_TYPE_HAND) 
	{
		bRet = _Base_IsHandSameStatus(status);
	}
	else if (nModeType == VAT_MODE_TYPE_PICKER) 
	{
		bRet = (_Base_IsAllPickerSameStatus(VAT_PITCH_TYPE_WIDE, status) == TRUE && _Base_IsAllPickerSameStatus(VAT_PITCH_TYPE_NARROW, status) == TRUE);
	}
	else
	{
		bRet = _Base_IsAllPartSameStatus(status);
	}

	return bRet;
}


//Auto Schedule 관련 parameter 초기화.
void CVatBaseTask::_Base_ClearParam()
{
	m_nCurLoc = -1;
	m_nSeqMode = g_VatTaskSystemCtrl.GetSeqCurMode();
	m_nVisionRetryCnt = 0; //Vision Retry Count 초기화.
	m_dPrevPosX = 0.0;
	m_dPrevPosY = 0.0;
	//GUI status initial로 변경 하자
	m_stCurPicker.clear();

	m_nTableTargetIdx = eTableTarget_None;
	memset(m_dTblTargetPos, 0.0, sizeof(m_dTblTargetPos));
}


void CVatBaseTask::_Base_SetMeasureHand(BOOL bSelect)
{
	//선택 안된 Hand 제외
	if (bSelect != TRUE)
		return;

	m_cSelectedHand.first = m_nHand;
	m_cSelectedHand.second = VAT_STATUS_MEASURE_READY;

	CVAT_MeasureStatus::GetInstance().UpdateHandStatus(m_cSelectedHand.first, m_cSelectedHand.second); //Hand, Status

// 	std::pair<int, _eVAT_MeasureStatus> p;
// 	p.first = m_nHand;			//Selected Hand.
// 	p.second = VAT_STATUS_MEASURE_READY;	//Status 측정 준비.
// 
// 	m_cSelectedHand.push_back(p);



}


void CVatBaseTask::_Base_SetMeasureHand_Status(_eVAT_MeasureStatus nStatus)
{
	if (m_cSelectedHand.first != m_nHand) 
		return;

	m_cSelectedHand.second = nStatus;


	CVAT_MeasureStatus::GetInstance().UpdateHandStatus(m_cSelectedHand.first, m_cSelectedHand.second); //Hand, Status
}


int CVatBaseTask::_Base_GetMeasureHand_Status()
{
	return m_cSelectedHand.second;
}


//Hand 선택 되지 않으면 어떤 상태 Check에서든 TRUE를 Return
//
BOOL CVatBaseTask::_Base_IsHandSameStatus(_eVAT_MeasureStatus hand_status)
{
	if (m_cSelectedHand.first != -1 && m_cSelectedHand.second != hand_status) {
		return FALSE;
	}
	
	return TRUE;
}

//*	PICKER TYPE *//

void CVatBaseTask::_Base_SetMeasurePicker_Selected(int nPicker, BOOL bSelect)
{
	if (bSelect != TRUE)
		return;

	std::pair<int, _eVAT_MeasureStatus> p;
	p.first = nPicker;						//Picker Number.
	p.second = VAT_STATUS_MEASURE_READY;	//Status 준비.

	m_cSelectedPicker[VAT_PITCH_TYPE_WIDE].push_back(p);
	CVAT_MeasureStatus::GetInstance().UpdatePickerStatus(m_nHand, VAT_PITCH_TYPE_WIDE, p.first, p.second);

	m_cSelectedPicker[VAT_PITCH_TYPE_NARROW].push_back(p);
	CVAT_MeasureStatus::GetInstance().UpdatePickerStatus(m_nHand, VAT_PITCH_TYPE_NARROW, p.first, p.second);
}

//=====================================================================================
// COMMENT : 해당 상태에 대한 Picker 1개 return
// PARAM1 : 측정 상태.
// RETURN1 : Picker Index
// RETURN2 : Pitch Type(WIDE/NARROW)
//=====================================================================================
ST_VAT_PICKER CVatBaseTask::_Base_GetMeasurePicker_Each(_eVAT_MeasureStatus nStatus)
{	
	ST_VAT_PICKER stPicker;
	stPicker.nPickerIdx = -1;
	stPicker.nPitchType = -1;

	for (int nType = 0; nType < VAT_PITCH_TYPE_MAX; nType++)
	{
		for (auto aSelectedPkr : m_cSelectedPicker[nType])
		{
			if (aSelectedPkr.second == nStatus) {
				stPicker.nPickerIdx = aSelectedPkr.first;	//Picker Index
				stPicker.nPitchType = nType;				//Pitch Type			
				return stPicker;
			}
		}
	}

	return stPicker;
}


void CVatBaseTask::_Base_SetMeasureAllPicker_Status(_eVAT_MeasureStatus nStatus)
{
	for (int nPitchType = 0; nPitchType < VAT_PITCH_TYPE_MAX; nPitchType++) {
		for (int nPickerIdx = 0; nPickerIdx < VAT_PICKER_MAX; nPickerIdx++) {
			_Base_SetMeasurePicker_Status(nPitchType, nPickerIdx, nStatus);
		}
	}
}


void CVatBaseTask::_Base_SetMeasurePicker_Status(int nPitchType, int nPicker, _eVAT_MeasureStatus nStatus)
{
	for (auto aSelectedPkr : m_cSelectedPicker[nPitchType]) {
		if (aSelectedPkr.first == nPicker) {
			std::replace(m_cSelectedPicker[nPitchType].begin(), m_cSelectedPicker[nPitchType].end(), aSelectedPkr, std::make_pair(nPicker, nStatus));

			CVAT_MeasureStatus::GetInstance().UpdatePickerStatus(m_nHand, nPitchType, m_cSelectedPicker[nPitchType][nPicker].first, m_cSelectedPicker[nPitchType][nPicker].second);

			return;
		}
	}
}


int CVatBaseTask::_Base_GetMeasurePicker_Status(int nPitchType, int nPicker)
{
	ASSERT(VAT_PITCH_TYPE_WIDE <= nPitchType && nPitchType <= VAT_PITCH_TYPE_NARROW);
	ASSERT(VAT_PICKER_01 <= nPicker && nPicker <= VAT_PICKER_08);

	int nRet = ERR_VAT_ERROR;

	for (auto aSelectedPkr : m_cSelectedPicker[nPitchType])
	{
		if (aSelectedPkr.first == nPicker) {
			nRet = aSelectedPkr.second;
			break;
		}
	}

	return nRet;
}

//Pitch Type에 해당 하는 All Picker 동일 상태 확인.
BOOL CVatBaseTask::_Base_IsAllPickerSameStatus(int nPitchType, int part_status)
{
	for (auto aSelectedPkr : m_cSelectedPicker[nPitchType])
	{
		if (aSelectedPkr.second != part_status) {
			return FALSE;
		}
	}

	return TRUE;
}


BOOL CVatBaseTask::_Base_IsAnyPickerSelected()
{
	return (m_cSelectedPicker[VAT_PITCH_TYPE_WIDE].size() > 0 && m_cSelectedPicker[VAT_PITCH_TYPE_NARROW].size() > 0);
}


//*	PART TYPE *//

void CVatBaseTask::_Base_SetMeasurePart_Selected(int nPart, BOOL bSelect)
{
	//선택 안된 Part 제외
	if (bSelect != TRUE)
		return;

	std::pair<int, _eVAT_MeasureStatus> p;
	p.first = nPart;					//Part Number.
	p.second = VAT_STATUS_MEASURE_READY; //Status 준비.

	m_cSelectedPart.push_back(p);

	//DM 상태 관리.
	_SetMeasurePart_Status_DM(nPart, VAT_STATUS_MEASURE_READY);
}


int CVatBaseTask::_Base_GetMeasurePart_Selected(_eVAT_MeasureStatus nStatus)
{
	int part_idx_ret = -1;

	for (auto aSelectedPart : m_cSelectedPart)
	{
		if (aSelectedPart.second == nStatus)
		{
			part_idx_ret = aSelectedPart.first;
			break;
		}
	}

	return part_idx_ret;
}


UNITCOMPONENT CVatBaseTask::_Base_GetMeasurePart_Each(int nPart)
{
	//초기화.
	UNITCOMPONENT cRetPart;
	cRetPart.first = ERR_VAT_ERROR; //(-1)
	cRetPart.second = VAT_STATUS_MEASURE_INIT;

	for (auto aSelectedPart : m_cSelectedPart)
	{
		if (aSelectedPart.first == nPart)
		{
			cRetPart = aSelectedPart;
			break;
		}
	}

	return cRetPart;
}


//==========================================================================================
//COMMENT : 선택 된 Part 측정 상태 변경.
//==========================================================================================
void CVatBaseTask::_Base_SetMeasurePart_Status(int nPart, _eVAT_MeasureStatus nStatus)
{
	for (auto aSelectedPart : m_cSelectedPart)
	{
		if (aSelectedPart.first == nPart) {
			std::replace(m_cSelectedPart.begin(), m_cSelectedPart.end(), aSelectedPart, std::make_pair(nPart, nStatus));

			//DM 상태 관리.
			_SetMeasurePart_Status_DM(nPart, nStatus);

			return;
		}
	}
}


//==========================================================================================
//COMMENT : 선택 된 Part 현재 상태 확인 함수.
//==========================================================================================
int CVatBaseTask::_Base_GetMeasurePart_Status(int nPart)
{
	int nRet = ERR_VAT_ERROR;

	for (auto aSelectedPart : m_cSelectedPart)
	{
		if (aSelectedPart.first == nPart) {
			nRet = aSelectedPart.second;
			break;
		}
	}
	
	return nRet;
}


//==========================================================================================
//COMMENT : Part Type 측정 에서 사용 - Z Auto Teaching/Vision Auto Teaching
//			DM에 저장 후, 측정 Main Dlg에서 상태 표기를 위함.
//==========================================================================================
void CVatBaseTask::_SetMeasurePart_Status_DM(int nPart, _eVAT_MeasureStatus nStatus)
{
	ASSERT(VAT_STATUS_MEASURE_INIT <= nStatus && nStatus <= VAT_STATUS_MAX);

	int nMode = g_DMCont.m_dmVAT.GN(NDM14_VAT_CURRENT_MODE);
	if (!(nMode == VAT_SEQ_MODE_VAT || nMode == VAT_SEQ_MODE_Z_TEACHING))
		return;

	int nDMStartIdx = -1;

	switch (m_nHand)
	{
	case VAT_HAND_LOAD_TRAYPP:		nDMStartIdx = (nMode == VAT_SEQ_MODE_VAT) ? NDM14_State_Vat_LD_Tray_Pp_Part1 : NDM14_State_ZTeach_LD_Tray_Pp_Part1;		break;
	case VAT_HAND_UNLOAD_TRAYPP:	nDMStartIdx = (nMode == VAT_SEQ_MODE_VAT) ? NDM14_State_Vat_UD_Tray_Pp_Part1 : NDM14_State_ZTeach_UD_Tray_Pp_Part1;		break;
	case VAT_HAND_LOAD_TESTPP1:		nDMStartIdx = (nMode == VAT_SEQ_MODE_VAT) ? NDM14_State_Vat_LD_Test_Pp_1_Part1 : NDM14_State_ZTeach_LD_Test_Pp_1_Part1;	break;
	case VAT_HAND_UNLOAD_TESTPP1:	nDMStartIdx = (nMode == VAT_SEQ_MODE_VAT) ? NDM14_State_Vat_UD_Test_Pp_1_Part1 : NDM14_State_ZTeach_UD_Test_Pp_1_Part1;	break;
	case VAT_HAND_LOAD_TESTPP2:		nDMStartIdx = (nMode == VAT_SEQ_MODE_VAT) ? NDM14_State_Vat_LD_Test_Pp_2_Part1 : NDM14_State_ZTeach_LD_Test_Pp_2_Part1;	break;
	case VAT_HAND_UNLOAD_TESTPP2:	nDMStartIdx = (nMode == VAT_SEQ_MODE_VAT) ? NDM14_State_Vat_UD_Test_Pp_2_Part1 : NDM14_State_ZTeach_UD_Test_Pp_2_Part1;	break;
	default:
		break;
	}

	if (nDMStartIdx != -1) {
		g_DMCont.m_dmVAT.SN(nDMStartIdx + nPart, nStatus);
	}
}

//Part 선택 되지 않으면 어떤 상태 Check에서든 TRUE를 Return 
//Loop문에서 모든 Auto 상태 check시, part selected안된 hand들 continue 역할.
BOOL CVatBaseTask::_Base_IsAllPartSameStatus(int part_status)
{
	for (auto nPart : m_cSelectedPart){
		if (nPart.second != part_status) {
			return FALSE;
		}
	}

	return TRUE;
}


int CVatBaseTask::_Base_GetVATMeasureType(int nHand)
{
	int nRet = VAT_MEASURE_TYPE_NONE;

	BOOL bIsHandOnlyOptUse = g_DMCont.m_dmVAT.GN(NMD14_VAT_MEASURE_HAND_ONLY_1 + nHand);
	BOOL bIsUnitOnlyOptUse = g_DMCont.m_dmVAT.GN(NMD14_VAT_MEASURE_UNIT_ONLY_1 + nHand);
	
	if (bIsHandOnlyOptUse && !bIsUnitOnlyOptUse) {
		nRet = VAT_MEASURE_TYPE_HAND_ONLY;
	}
	else if (!bIsHandOnlyOptUse && bIsUnitOnlyOptUse) {
		nRet = VAT_MEASURE_TYPE_UNIT_ONLY;
	}
	else if (!bIsHandOnlyOptUse && !bIsUnitOnlyOptUse) {
		nRet = VAT_MEASURE_TYPE_BOTH;
	}

	return nRet;
}


//==========================================================================================
//COMMENT : 두 점 변화량에 대한 기울기 각도 계산.
//==========================================================================================
double CVatBaseTask::_Base_CalSlopeDegree(double dX, double dY)
{
	double dTheta = 0.0;
	if (dX == 0.0)
	{
		if (dY > 0)			dTheta = VAT_PI / 2;		//rad(90°)
		else if (dY < 0)	dTheta = (-1)*VAT_PI / 2;	//rad(-90°)
		else				dTheta = 0.0;				//rad(0°)		
	}
	else
	{
		dTheta = atan(dY / dX); //radian
	}

	double dDegree = dTheta * 180.0 / VAT_PI;

	return dDegree;
}


void CVatBaseTask::_Base_AlarmReport_VAT(int nErr, CString strAlarmData /*= _T("")*/)
{
	CParamList AlarmData;
	AlarmData.m_fnAddItemFormat("%s", strAlarmData);
	g_Error.AlarmReport(nErr, AlarmData, NULL);
	g_VatTaskSystemCtrl.SetLotCommand(eAutoLotCmd_AlarmPause);
}


//==============================================================================
// [9/5/2025 dohyeong.kim]
// COMMENT : 각각의 Auto에서 Hand 진행률 Update를 하기 위함.
//           Total case에 비례하여 각각의 case 진행률 Update.
//
//			[Update Progress 2가지 case]
//			1. 현재 case 측정 진행 중인 상황.(nCompleteProgressRate < nCurProgressRateMax)
//			
//			2. 이전 case 돌아가서 측정 진행 중인 상황.(nCompleteProgressRate < m_dThresholdProgressRate)
//			※단, ThresholdRate까지만 지속 증가.
//
// PARAM1 : nCurMainStep -> ChangeStep 진행 한, 현재 Step
// PARAM2 : nLastMainStep -> ChangeStep 하기 전, 최근 Step
// PARAM3 : nTotalCase -> 각 Mode의 전체 total case 개수
//==============================================================================
void CVatBaseTask::_Base_UpdateProgressRate(int nCurMainStep, int nLastMainStep, int nTotalCase)
{
	if (nCurMainStep < 0 || nLastMainStep < 0)
		return;	

	nCurMainStep++;		//case step 1base로 변경.
	nLastMainStep++;	//case step 1base로 변경.
	nTotalCase++;		//case step 1base로 변경.

	CVAT_MeasureStatus& GuiNotifier = CVAT_MeasureStatus::GetInstance();

	if (nCurMainStep == nTotalCase) {
		GuiNotifier.UpdateProgressPercent(m_nHand, 100.0);
	}

	//각 단계가 차지하는 비율
	double dStepWeight = 1.0 / nTotalCase;

	double dCompleteProgressRate = GuiNotifier.GetProgressPercent(m_nHand); //현재 까지 완료 된 진행률
	double dCurStepProgressRate = (nCurMainStep * dStepWeight * 100.0);
	double dLastStepProgressRate = (nLastMainStep * dStepWeight * 100.0);

	if (nCurMainStep != nLastMainStep)
	{
		if (dCompleteProgressRate < dLastStepProgressRate) {
			//[1. 다음 case 넘어 가는 경우]
			if (nCurMainStep > nLastMainStep) {
				//이전 Step 진행률 100% Update
				GuiNotifier.UpdateProgressPercent(m_nHand, dLastStepProgressRate);

				m_dEachCaseRate = 0.0; //case progress 초기화.
				m_dThresholdProgressRate = 0.0;
				return;
			}
			//[2. 이전 case 넘어 가는 경우]
			else {
				m_dThresholdProgressRate = static_cast<int>(nLastMainStep * dStepWeight * 100.0);
			}
		}
	}

	double dSpanTime = m_tcAutoSchedule.CompareTime(FALSE); //sec //	
	//m_dEachCaseRate += dSpanTime * 0.000001; //1초당 X% 증가.
	m_dEachCaseRate = dSpanTime * 0.000001; //1초당 X% 증가.

	if (m_dEachCaseRate >= 100.0) {
		m_dEachCaseRate = 100.0;
	}
	
	//Cur Step 및 Threshold 최대 지점 까지만 Update 진행.
	dCompleteProgressRate = GuiNotifier.GetProgressPercent(m_nHand);
	double dTotalProgressRate = (dCompleteProgressRate)+(m_dEachCaseRate * dStepWeight);

	if (dTotalProgressRate < dCurStepProgressRate || dTotalProgressRate < m_dThresholdProgressRate) {
		GuiNotifier.UpdateProgressPercent(m_nHand, dTotalProgressRate);
	}
}


void CVatBaseTask::_Base_UpdateProgressRate_Picker()
{
	if (m_stCurPicker.nPitchType == -1 || m_stCurPicker.nPickerIdx == -1)
		return;

	CVAT_MeasureStatus& GuiNotifier = CVAT_MeasureStatus::GetInstance();

	int nMaxPicker = m_cSelectedPicker[VAT_PITCH_TYPE_WIDE].size() + m_cSelectedPicker[VAT_PITCH_TYPE_NARROW].size();
	
	//1 ~ 16개 Picker Index 계산
	int nCurPickerIdx = (m_stCurPicker.nPickerIdx + 1) + (m_stCurPicker.nPitchType * 8); 
	
	double dStepWeight = 1.0 / nMaxPicker;
	double dCurProgressRate = GuiNotifier.GetProgressPercent(m_nHand);  //현재 진행 중인 Progress Rate
	double dCurMinProgressRate = (nCurPickerIdx - 1) * dStepWeight * 100.0;
	double dCurMaxProgressRate = nCurPickerIdx * dStepWeight * 100.0; //현재 진행 중인 Max Progress Rate
	
	double dTotalProgressRate = dCurProgressRate;

	//이전 구간 보정.
	if (dCurProgressRate < dCurMinProgressRate) {
		dTotalProgressRate = dCurMinProgressRate;
	}
	else {
		//현재 Picker 구간 진행률 증가.
		if (dCurProgressRate < dCurMaxProgressRate)
		{
			double dSpanTime = m_tcAutoSchedule.CompareTime(FALSE); //sec
			m_dEachCaseRate = dSpanTime * 0.001;
			if (m_dEachCaseRate >= 100.0) {
				m_dEachCaseRate = 100.0;
			}

			dTotalProgressRate = dCurProgressRate + (m_dEachCaseRate * dStepWeight);
		}
	}



	//계산 값이 현재 Max Progress보다 넘지 않도록 제한.
	if (dTotalProgressRate >= dCurMaxProgressRate)
		dTotalProgressRate = dCurMaxProgressRate;

	//계산 값이 이전 측정 Progress보다 다시 줄어들지 않도록 제한.
	if (dTotalProgressRate < dCurMinProgressRate)
		dTotalProgressRate = dCurMinProgressRate;

	//마지막 Picker 측정 완료 확인.
	if (nCurPickerIdx == nMaxPicker) {
		//마지막 Picker 측정 중, 100% 넘으면 99% 유지.
		if (dTotalProgressRate >= 100.0) {
			dTotalProgressRate = 99.0;
		}

		//전체 Picker 측정 완료 시, 100%로 세팅
		BOOL bWideDone = _Base_IsAllPickerSameStatus(VAT_PITCH_TYPE_WIDE, VAT_STATUS_MEASURE_COMPLETE);
		BOOL bNarrowDone = _Base_IsAllPickerSameStatus(VAT_PITCH_TYPE_NARROW, VAT_STATUS_MEASURE_COMPLETE);
		if (bWideDone && bNarrowDone) {
			dTotalProgressRate = 100.0;
		}
	}

	GuiNotifier.UpdateProgressPercent(m_nHand, dTotalProgressRate);
// 	if (dCurProgressRate < dCurMaxProgressRate) {
// 		double dSpanTime = m_tcAutoSchedule.CompareTime(FALSE); //sec
// 		m_dEachCaseRate = dSpanTime * 0.0001;
// 		if (m_dEachCaseRate >= 100.0) {
// 			m_dEachCaseRate = 100.0;
// 		}
// 		dTotalProgressRate = dCurProgressRate + (m_dEachCaseRate * dStepWeight);
// 	}
// 	else {
// 		//현재 Picker Progress Rate 도달 했을 경우
// 		dTotalProgressRate = dCurMaxProgressRate;
//  	}

//	GuiNotifier.UpdateProgressPercent(m_nHand, dTotalProgressRate);
}