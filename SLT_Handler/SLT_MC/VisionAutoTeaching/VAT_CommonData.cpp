#include "stdafx.h"
#include "VAT_CommonData.h"

CVAT_CommonData::CVAT_CommonData()
{
	InitializeCriticalSection(&m_cs);

	//Z Auto Teaching
	memset(first_move_offset_um, 0.0, sizeof(first_move_offset_um));
	data_save_complete = FALSE;
}

CVAT_CommonData::~CVAT_CommonData()
{
	DeleteCriticalSection(&m_cs);
}


//<Z Auto Teaching>
void CVAT_CommonData::SetFirstPickOffset(int nHand, double dPos)
{
	//EnterCriticalSection(&m_cs);
	first_move_offset_um[nHand] = dPos;
	//LeaveCriticalSection(&m_cs);
}


double CVAT_CommonData::GetFirstPickOffset(int nHand)
{
/*	EnterCriticalSection(&m_cs);*/
	return first_move_offset_um[nHand];
/*	LeaveCriticalSection(&m_cs);*/
}


void CVAT_CommonData::ClearFirstPickOffset(int nHand)
{
	first_move_offset_um[nHand] = 0.0;
	/*memset(first_move_offset_um, 0.0, sizeof(first_move_offset_um));*/
}

//==========================================================================================
//COMMENT : 선택 된 Hand 또는 Part의 측정 상태 확인.
//==========================================================================================
BOOL CVAT_CommonData::IsSelectUnitInState(int nHand, _eVAT_MeasureStatus nMeasureStatus)
{
	BOOL bRet = FALSE;

	int nModeType = g_VatTaskSystemCtrl.GetSeqCurModeType();

	if (nModeType == VAT_MODE_TYPE_HAND)
	{
		BOOL bIsHandSelected = g_VatTaskSystemCtrl.m_vAllTaskAuto[nHand]->_Base_IsHandSelected();
		BOOL bIsHandMeasureComplete = g_VatTaskSystemCtrl.m_vAllTaskAuto[nHand]->_Base_IsHandSameStatus(nMeasureStatus);
		if (bIsHandSelected && bIsHandMeasureComplete)
			bRet = TRUE;
	}
	else if (nModeType == VAT_MODE_TYPE_PICKER)
	{
		//Hand에 해당 하는 전체 Picker 상태 확인.
		BOOL bIsAnyPickerSelected = g_VatTaskSystemCtrl.m_vAllTaskAuto[nHand]->_Base_IsAnyPickerSelected();
		ST_VAT_PICKER stPicker = g_VatTaskSystemCtrl.m_vAllTaskAuto[nHand]->_Base_GetMeasurePicker_Each(nMeasureStatus);
		if (bIsAnyPickerSelected && stPicker.nPitchType != -1 && stPicker.nPickerIdx != -1)
			bRet = TRUE;
	}
	else
	{
		//Abort일 경우, 모든 Part가 Data Save가 아닐 수 있음.
		//'VAT_STATUS_DATA_SAVE_COMPLETE' 상태 1ea라도 있으면 저장.
		//  1) Part Unselecet 되면 return -1
		//  2) Part 상태 'VAT_STATUS_DATA_SAVE_COMPLETE' 없으면 return -1
		int nPart = g_VatTaskSystemCtrl.m_vAllTaskAuto[nHand]->_Base_GetMeasurePart_Selected(nMeasureStatus);
		if (nPart != -1) {
			bRet = TRUE;
		}
	}

	return bRet;
}



//==========================================================================================
//COMMENT : VAT Data 흐름 방식.
//			1) Sequence 구조체 Data로 모든 측정 진행.
//			2) Real 구조체 Data -> Buffer 구조체 Data.(이전 Data 비교를 위해 Buffer에 저장) 
//			3) Sequence 구조체 Data -> Real 구조체 Data.
//==========================================================================================
void CVAT_CommonData::MeasureDataSave()
{
	int nSeqMode = g_VatTaskSystemCtrl.GetSeqCurMode();

	ASSERT(VAT_SEQ_MODE_Z_FOCUS <= nSeqMode && nSeqMode <= VAT_SEQ_MODE_Z_TEACHING);

	// 하기 저장 순서 변경 금지 [9/23/2025 dohyeong.kim]

	//측정 완료 된 Data로(Seq) Calculate 진행
	switch (nSeqMode)
	{
	case VAT_SEQ_MODE_Z_FOCUS:				DataSaveZAutoFocus();			break;
	case VAT_SEQ_MODE_CAM_CAL:				DataSaveCamCenterCalibration();	break;
	case VAT_SEQ_MODE_PICKER_ALIGN:			DataSavePickerAlign();			break;
	case VAT_SEQ_MODE_VAT:					DataSaveVisionAutoTeaching();	break;
	case VAT_SEQ_MODE_Z_TEACHING:			DataSaveZAutoTeaching();		break;
	default:
		break;
	}

	//이전 측정 Data 저장.
	DataCopyRealToBuffer(nSeqMode);

	//신규 측정 Data 저장.
	DataCopySequenceToReal(nSeqMode);

	//File Data 저장.
	MeasureDataSaveToFile(nSeqMode);

	//Gui Update
	CVAT_MeasureStatus::GetInstance().UpdateSaveData();
}

//==========================================================================================
//COMMENT : 해당 Mode Data만 저장.
//			필요 Data만 저장 하고자 함.
//			 -. Measure 후, 변경 된 Data.
//==========================================================================================
void CVAT_CommonData::MeasureDataSaveToFile(int nMode)
{
/*	int nModeType = g_VatTaskSystemCtrl.GetSeqCurModeType();*/

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {	
		if (!IsSelectUnitInState(nHand, VAT_STATUS_DATA_SAVE_COMPLETE)) {
			continue;
		}

		//Buffer Data 가 존재 하는 구조체들은 파일에도 Buffer Data 저장해줘야 함.
		switch (nMode)
		{
		case VAT_SEQ_MODE_Z_FOCUS:
		{
			//측정 후, 변경 되는 Data 저장
			//eZFocusSaveEach_PlateJigTargetPos		-> X, Y
			//eZFocusSaveEach_PlateJigVacPos		-> Z
			//eZFocusSaveEach_PlateJigLowerOffset	-> X, Y
			SaveZAutoFocusInformation();
//  			for (int nType = 0; nType < eZFocusType_Max; nType++) {
//  				SaveEachZAutoFocusInformation((_eVAT_MODE_EACH_DATA_TYPE)nType, nHand);
//  			}
			
			SaveEachCamCenterCalInformation(eCamCenCal_BotCamCenterPos, nHand, VAT_Z); //기준 Picker 높이
			SaveEachCamCenterCalInformation(eCamCenCal_TopCamToPkrDis, nHand, VAT_Z);  //기준 Picker ~ Cam 높이

			SaveEachVisionAutoTeachingInformation(eVisionAutoTeaching_HandBasePos, nHand, VAT_Z); //Target 측정 높이

		}break;
		case VAT_SEQ_MODE_CAM_CAL:
		{
			SaveCamCenterCalInformation();
		}break;
		case VAT_SEQ_MODE_PICKER_ALIGN:
		{
			SavePickerAlignInformation();
		}break;
		case VAT_SEQ_MODE_VAT:
		{
			
		}break;
		case VAT_SEQ_MODE_Z_TEACHING:
		{
			SaveEachZAutoTeachingInformation(eZAutoTeaching_PickerFlatnessOffset, nHand);
			SaveEachZAutoTeachingInformation(eZAutoTeaching_PickerVacPos, nHand);
		}break;
		default:
			break;
		}
	}
}


void CVAT_CommonData::DataSaveZAutoFocus()
{
	//1. 하단 Target X,Y Offset						-> Seq에서 측정 완료.
	//2. 상단 Target 초점 맞는 위치 X, Y				-> Seq에서 측정 완료.

	//3. 기준 Picker 하단 Cam 측정 높이 저장.			-> Cal 필요.
	//4. All Part 초점 맞는 Base Z축.				-> Cal 필요.
	//   -> 설계치가 맞지 않는다면, 기준 Picker ~ Cam간의 Z 저장 하며 Z 높이 실측.			
	  	
	//5. 기준 Picker(Z Pick 높이) ~ Cam 높이

	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++)
	{
		if(!IsSelectUnitInState(nHand, VAT_STATUS_MEASURE_COMPLETE))
			continue;

		//3. 기준 Picker 하단 Cam 측정 높이.
		double dPlateJigVacPos = m_stVATZFocusInfoSeq.dPlateJIG_VacuumPos[nHand][VAT_Z];
		double dPlateJigUpperHeight = g_VATCommonStaticData.m_stVATHandCommonData.dTopPlateThickness[nHand];
		m_stVATCamCenterCalInfoBuff.dBotCameraPosition[nHand][VAT_Z] = m_stVATCamCenterCalInfo.dBotCameraPosition[nHand][VAT_Z]; //이전 Data 저장(Real -> Buff)
		m_stVATCamCenterCalInfo.dBotCameraPosition[nHand][VAT_Z] = (dPlateJigVacPos + dPlateJigUpperHeight); //Real에 저장.

		//4. All Part Base Z 계산.
		double dPlateJigTargetPos = m_stVATZFocusInfoSeq.dPlateJIG_TargetPos[nHand][VAT_Z];
		double dBaseToPlateJigHeight = g_VATCommonStaticData.m_stVATHandCommonData.dBaseToPlateJigHeight[nHand];
		double dCamBasePos = dPlateJigTargetPos + dBaseToPlateJigHeight;


		int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
		for (int nPart = 0; nPart < nMaxPart; nPart++)
		{
			if (m_stVATFileSection._IsNecessaryPart(nHand, nPart) != TRUE) {
				continue;
			}

			double dZFocusHeight = VAT_CalZFocusHeight(nHand, nPart, dCamBasePos);

			double dLastData = m_stVATInfo.stHandBasePos.getBasePos(nHand, nPart, VAT_Z);
			m_stVATInfoBuff.stHandBasePos.setBasePos(nHand, nPart, VAT_Z, dLastData); //이전 Data 저장(Real -> Buff)
			m_stVATInfo.stHandBasePos.setBasePos(nHand, nPart, VAT_Z, dZFocusHeight); //Real에 저장.		
		}	

		//5. 기준 Picker(Z Pick 높이) ~ Cam 높이
		m_stVATCamCenterCalInfoBuff.dTopCamToPkrDistance[nHand][VAT_Z] = m_stVATCamCenterCalInfo.dTopCamToPkrDistance[nHand][VAT_Z]; //이전 Data 저장(Real -> Buff)
		m_stVATCamCenterCalInfo.dTopCamToPkrDistance[nHand][VAT_Z] = dPlateJigVacPos - dPlateJigTargetPos;


		//Hand 상태 변경 [MEASURE COMPLETE -> DATA SAVE COMPLETE]
		g_VatTaskSystemCtrl.m_vAllTaskAuto[nHand]->_Base_SetMeasureHand_Status(VAT_STATUS_DATA_SAVE_COMPLETE);
	}

}


double CVAT_CommonData::VAT_GetBaseToUnitHeight(int nHand, int nPart)
{
	//Base To Unit Height
	double dBaseToUnitHeight = 0.0;
	int nBaseToUnitIdx = -1;
	switch (nHand)
	{
	case VAT_HAND_LOAD_TRAYPP:
	case VAT_HAND_UNLOAD_TRAYPP:
	{
		if (VAT_IDX_TRAY_PP_TBL_1 <= nPart && nPart <= VAT_IDX_TRAY_PP_TBL_4) {
			nBaseToUnitIdx = VAT_TRAY_PP_BASE_TO_TABLE;
		}
		else {
			nBaseToUnitIdx = VAT_TRAY_PP_BASE_TO_FEEDER_OR_STACKER;
		}
	}break;
	case VAT_HAND_LOAD_TESTPP1:
	case VAT_HAND_UNLOAD_TESTPP1:
	case VAT_HAND_LOAD_TESTPP2:
	case VAT_HAND_UNLOAD_TESTPP2:
	{
		if (VAT_IDX_STATION_1 <= nPart && nPart <= VAT_IDX_STATION_4) {
			nBaseToUnitIdx = VAT_TEST_PP_BASE_TO_STATION;
		}
		else if(nPart == VAT_IDX_CLEAN_TBL) {
			nBaseToUnitIdx = VAT_TEST_PP_BASE_TO_CLEAN_TABLE;
		}
		else {
			nBaseToUnitIdx = VAT_TEST_PP_BASE_TO_TABLE;
		}
	}break;
	default:
		break;
	}

	if (nBaseToUnitIdx != -1) {
		dBaseToUnitHeight = g_VATCommonStaticData.m_stVATHandCommonData.dBaseToUnitHeight[nHand][nBaseToUnitIdx];
	}

	return dBaseToUnitHeight;
}

double CVAT_CommonData::VAT_CalZFocusHeight(int nHand, int nPart, double dCamBasePos)
{
	double dRet = 0.0;

	double dBaseToUnitHeight = VAT_GetBaseToUnitHeight(nHand, nPart);
	double dBaseHeight = dCamBasePos - dBaseToUnitHeight; //PKG C.O.K 상관 없는 Base 높이
	double dCOKHeight = 0.0;

	//PKG별 C.O.K 높이 적용
	switch (nHand)
	{
	case VAT_HAND_LOAD_TRAYPP:
	case VAT_HAND_UNLOAD_TRAYPP:
	{
		if (VAT_IDX_TRAY_PP_TBL_1 <= nPart && nPart <= VAT_IDX_TRAY_PP_TBL_4) {
			dCOKHeight = g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[VAT_TABLE_HEIGHT];
		}
		else { // Feeder or Stacker
			if (nHand == VAT_HAND_LOAD_TRAYPP) {
				//C-Tray Jig 홈파여 있는 높이 (4mm)
				dCOKHeight = g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[VAT_C_TRAY_JIG_HEIGHT] - 2000.0; // 4mm
			}
			else {
				dCOKHeight = g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[VAT_C_TRAY_JIG_HEIGHT]; //6mm
			}			
		}
	}break;
	case VAT_HAND_LOAD_TESTPP1:
	case VAT_HAND_UNLOAD_TESTPP1:
	case VAT_HAND_LOAD_TESTPP2:
	case VAT_HAND_UNLOAD_TESTPP2:
	{
		if (VAT_IDX_STATION_1 <= nPart && nPart <= VAT_IDX_STATION_4) {
			dCOKHeight = 40000.0; //Station Jig 높이(1호기) //2호기도 높이 맞출수 있으면 동일하게 해서 base to Height 높이에 더하자.
		}
		else if(VAT_IDX_TEST_PP_TBL_1_LD <= nPart && nPart <= VAT_IDX_TEST_PP_TBL_4_UD) { // Feeder or Stacker
			dCOKHeight = g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[VAT_TABLE_HEIGHT];
		}
		else { //clean table
			dCOKHeight = g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[VAT_CLEAN_TABLE_HEIGHT];
		}
	}break;
	default:
		break;
	}

	dRet = dBaseHeight - dCOKHeight;

	return dRet;
}


void CVAT_CommonData::DataSaveCamCenterCalibration()
{
	//1. 기준 Picker 하단 Cam과 Align 되는 Position			-> Seq에서 측정 완료.

	//2. 기준 Picker ~ 상단 Cam 거리.							-> Cal 필요.
	

	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++)
	{
		if (!IsSelectUnitInState(nHand, VAT_STATUS_MEASURE_COMPLETE))
			continue;

		//기준 Picker ~ 상단 Cam 거리. (X, Y)
		for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XY; nAxis++) {
			double dStdPicker = m_stVATCamCenterCalInfoSeq.dBotCameraPosition[nHand][nAxis];
			double dUpperCamPos = m_stVATZFocusInfo.dPlateJIG_TargetPos[nHand][nAxis];
			double dLowerCamOffset = m_stVATZFocusInfo.dPlateJIG_LowerOffset[nHand][nAxis];

			m_stVATCamCenterCalInfoSeq.dTopCamToPkrDistance[nHand][nAxis] = (dStdPicker + dLowerCamOffset) - dUpperCamPos;
		}

		//Hand 상태 변경 [MEASURE COMPLETE -> DATA SAVE COMPLETE]
		g_VatTaskSystemCtrl.m_vAllTaskAuto[nHand]->_Base_SetMeasureHand_Status(VAT_STATUS_DATA_SAVE_COMPLETE);
	}
}

void CVAT_CommonData::DataSavePickerAlign()
{
	//1. Wide/Narrow Picker Center Pos				-> Seq에서 측정 완료.

	//2. Wide/Narrow Picker별 Offset 계산.			-> Cal 필요.

	//3. Wide/Narrow 평균 Offset

	//4. Pitch Teaching값.				-> Cal 필요.

	//순서 변경 하지 말것.
	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++)
	{
		if (!IsSelectUnitInState(nHand, VAT_STATUS_MEASURE_COMPLETE))
			continue;

		//[2. Wide/Narrow/Wide-Narrow Picker별 Offset 계산]
		VAT_CalWideNarrowOffset(nHand);

		//[3. Pitch Teaching값 계산]
		//Offset Spec Out이면 계산 진행 하며, 계산 진행 시 Return값 True.
		VAT_CalXYPitchTeachingPos(nHand);

		//[4. Wide/Narrow 평균 Offset]
		VAT_CalWideNarrowAvgOffset(nHand);

		//Hand 상태 변경 [MEASURE COMPLETE -> DATA SAVE COMPLETE]
		g_VatTaskSystemCtrl.m_vAllTaskAuto[nHand]->_Base_SetMeasureAllPicker_Status(VAT_STATUS_DATA_SAVE_COMPLETE);
	}

	g_DMCont.m_Teach.Save(); //Save 처리하는 속도가 느리면, Hand만 접근 해서 저장 할 수 있도록 함수 추가 필요.
}


void CVAT_CommonData::VAT_CalWideNarrowOffset(int nHand)
{
	// Wide or Narrow의 8ea Picker 측정완료 상태만 Data 계산 진행.
	// Picker Offset은 8ea Picker Data가 있어야 계산이 가능.
	for (int nPitchType = 0; nPitchType < VAT_PITCH_TYPE_MAX; nPitchType++)
	{
		//Selected Picker 전체 측정 완료 Check.
		BOOL bMeasureDone = g_VatTaskSystemCtrl.m_vAllTaskAuto[nHand]->_Base_IsAllPickerSameStatus(nPitchType, VAT_STATUS_MEASURE_COMPLETE);
		if (bMeasureDone != TRUE)
			continue;

		std::pair<double, double> pCalOffset[VAT_PICKER_MAX];

		//[1. Picker Offset 계산] - STD Picker 기준으로 각 Picker의 절대 좌표.
		for (int nPickerIdx = 0; nPickerIdx < VAT_PICKER_MAX; nPickerIdx++) {
			std::pair<double, double> pDesignPos = g_VatTaskSystemCtrl.m_vAllTaskAuto[nHand]->_Base_CalPitchTargetPosXY(nPitchType, nPickerIdx);
			std::pair<double, double> pMeasurePos = g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerPos[nHand][nPitchType][nPickerIdx];

			pCalOffset[nPickerIdx].first = pMeasurePos.first - pDesignPos.first;
			pCalOffset[nPickerIdx].second = pMeasurePos.second - pDesignPos.second;
		}

		//[2. 기준 Picker Offset을 Zero Base로 재 계산]
		CPoint cStdPicker = g_VatTaskSystemCtrl.m_vAllTaskAuto[nHand]->_Base_GetStdPicker();
		int nStdPickerIdx = cStdPicker.x + (4 * cStdPicker.y);
		std::pair<double, double> pStdPickerOffset = pCalOffset[nStdPickerIdx];

		for (int nPickerIdx = 0; nPickerIdx < VAT_PICKER_MAX; nPickerIdx++) {
			pCalOffset[nPickerIdx].first -= pStdPickerOffset.first;
			pCalOffset[nPickerIdx].second -= pStdPickerOffset.second;

			//(-1)을 곱해야 실제 Picker가 어디 방향에 위치 하는지 확인 가능.
			//ex) 설계치로 이동하면 Encoeer값 15, Center 측정 위치는 15.2라면,
			//    0.2만큼 Pitch가 Narrow되어 있는것 -> -0.2
			
			//if(nPickerIdx == VAT_PICKER_01)
// 			if (nPickerIdx == VAT_PICKER_01 || nPickerIdx == VAT_PICKER_02 || nPickerIdx == VAT_PICKER_05 || nPickerIdx == VAT_PICKER_06) {
// 				g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerOffset[nHand][nPitchType][nPickerIdx].first = (-1)*pCalOffset[nPickerIdx].first;
// 			}
// 			else {
// 				g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerOffset[nHand][nPitchType][nPickerIdx].first = /*(-1)**/pCalOffset[nPickerIdx].first;
// 			}

			g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerOffset[nHand][nPitchType][nPickerIdx].first = (-1)*pCalOffset[nPickerIdx].first;
			g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerOffset[nHand][nPitchType][nPickerIdx].second = /*(-1)**/pCalOffset[nPickerIdx].second;
		}
	}
}


void CVAT_CommonData::VAT_CalXYPitchTeachingPos(int nHand)
{

	//Wide/Narrow Offset 평균 값 보정해서 Teaching값 산정.
	for (int nPitchType = 0; nPitchType < VAT_PITCH_TYPE_MAX; nPitchType++)
	{
		//Spec Out Exist확인.
		BOOL bIsSpecOutX = VAT_IsSpecOutOffsetExist(nHand, nPitchType, VAT_PITCH_X);
		BOOL bIsSpecOutY = VAT_IsSpecOutOffsetExist(nHand, nPitchType, VAT_PITCH_Y);


		std::vector<double> vOffsetX;
		std::vector<double> vOffsetY;

		double dSumOffsetX = 0.0;
		double dSumOffsetY = 0.0;

		CPoint cStdPicker = g_VatTaskSystemCtrl.m_vAllTaskAuto[nHand]->_Base_GetStdPicker();
		int nStdPkrX1 = cStdPicker.x;
		int nStdPkrX2 = cStdPicker.x + 4;

		for (int nPickerIdx = 0; nPickerIdx < VAT_PICKER_MAX; nPickerIdx++) {	
			//==============[X값 저장]
			if (!(nStdPkrX1 == nPickerIdx || nStdPkrX2 == nPickerIdx)) {
				//6개 저장.
				dSumOffsetX += fabs(g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerOffset[nHand][nPitchType][nPickerIdx].first);
			}

			//===============[Y값 저장]
			if (nPickerIdx <= VAT_PICKER_04) {
				dSumOffsetY += fabs(g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerOffset[nHand][nPitchType][nPickerIdx].second);
			}		
		}

		//기준 Picker 제외 7ea picker에 대한 평균값 계산.
		double dAvgOffsetX = dSumOffsetX / 6.0; //기준 열 제외 6ea Offset
		double dAvgOffsetY = dSumOffsetY / 4.0; //기준 행 제외 4ea Offset

		//측정 진행 한 Pitch Teaching Position Get.
 		std::pair<double, double> pPitchTeachPos = g_VatTaskSystemCtrl.m_vAllTaskAuto[nHand]->_Base_GetPitchXYTeachPos(nHand, nPitchType);
 		double dTeachPosX = pPitchTeachPos.first;
 		double dTeachPosY = pPitchTeachPos.second;
		

		//최종 저장 Data
		std::pair<int, int> pAdjustOffsetDir = VAT_GetPitchOffsetDir(nHand, nPitchType);
		int nDirX = (pAdjustOffsetDir.first == VAT_DIRECTION_PLUSE) ? 1 : -1; //실제 Pitch 움직여야 하는 방향
		int nDirY = (pAdjustOffsetDir.second == VAT_DIRECTION_PLUSE) ? 1 : -1; //실제 Pitch 움직여야 하는 방향

		double dSaveDataX = 0.0;
 		double dSaveDataY = 0.0;

		if (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) {
			dSaveDataX = dTeachPosX + (nDirX * dAvgOffsetX);
			dSaveDataY = dTeachPosY + (nDirY * dAvgOffsetY);
		}
		else {
			dSaveDataX = dTeachPosX + (nDirY * dAvgOffsetY);
			dSaveDataY = dTeachPosY + (nDirX * dAvgOffsetX);
		}


		//Hand Pitch 정보 가져옴.
		ST_VAT_HAND_PITCH_INFO HandPitchInfo;

		auto it = cHandPitchParamMap.find(nHand);
		if (it != cHandPitchParamMap.end()) {
			HandPitchInfo = it->second;
		}
		else {
			ASSERT(0);
		}

		int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
		for (int nPart = 0; nPart < nMaxPart; nPart++) {
			if (m_stVATFileSection._IsNecessaryPart(nHand, nPart) != TRUE)
				continue;

			if(VAT_GetPartPitchType(nHand, nPart) != nPitchType)
				continue;

			//Test Pp Load Hand의 Clean Table Teaching값 계산 필요.
			if ((nHand == VAT_HAND_LOAD_TESTPP1 || nHand == VAT_HAND_LOAD_TESTPP2) && nPart == VAT_IDX_CLEAN_TBL)
			{
				//현재 Clean Table은 Narrow Pitch로 계산되어, Table Pitch로 계산되어 있음.
				double dTblPitchX = g_DMCont.m_dmTestPP.GN(NDM4_TestPP_Pitch_X); //46mm[Loading Table]
				double dTblPitchY = g_DMCont.m_dmTestPP.GN(NDM4_TestPP_Pitch_Y); //42mm[Loading Table]

				double dCleanTblPitchX = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Pitch_X); //26mm[Loading Table]
				double dCleanTblPitchY = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Pitch_Y); //23mm[Loading Table]

				double dGapXTbltoCleanTbl = dTblPitchX - dCleanTblPitchX;
				double dGapYTbltoCleanTbl = dTblPitchY - dCleanTblPitchY;

				//※※※Test PP의 경우 계산되는 Pitch와 실제 장비 정면에서 Pitch가 반대.※※※
				dSaveDataX -= dGapYTbltoCleanTbl;
				dSaveDataY -= dGapXTbltoCleanTbl;
			}


			int teach_loc_idx = -1;
			switch (nHand)
			{
				case VAT_HAND_LOAD_TRAYPP:
				{
					teach_loc_idx = g_TaskTrayLoadPP.TrayHandLocIdxConvertor(RIDX_TRAY_PP_1, nPart); //Part에 따라 Teach Loc Convert
				}break;
				case VAT_HAND_UNLOAD_TRAYPP:
				{
					teach_loc_idx = g_TaskTrayUnloadPP.TrayHandLocIdxConvertor(RIDX_TRAY_PP_1, nPart); //Part에 따라 Teach Loc Convert
				}break;
				default: //Test PP
				{
					teach_loc_idx = nPart;
				}break;
			}
	
			//Spec In의 경우, Pitch 계산 필요 없음.
			if (bIsSpecOutX) {
				g_DMCont.m_Teach.SetTeachPos(HandPitchInfo.robot_id, teach_loc_idx, HandPitchInfo.xp_pos_idx, dSaveDataX);
			}
			
			if (bIsSpecOutY) {
				g_DMCont.m_Teach.SetTeachPos(HandPitchInfo.robot_id, teach_loc_idx, HandPitchInfo.yp_pos_idx, dSaveDataY);
			}	
		}

		//Spec Out의 경우, Teaching Position을 변경해 줬기 때문에,
		//해당 Teaching값으로 재 측정 했을 때의 Offset도 반영해 줘야 함.
		if (bIsSpecOutX) {
			CPoint cStdPicker = g_VatTaskSystemCtrl.m_vAllTaskAuto[nHand]->_Base_GetStdPicker();
			int nStdPkrX1 = cStdPicker.x;
			int nStdPkrX2 = cStdPicker.x + 4;

			for (int nPickerIdx = 0; nPickerIdx < VAT_PICKER_MAX; nPickerIdx++) {
				if(nPickerIdx == nStdPkrX1 || nPickerIdx == nStdPkrX2)
					continue; //움직이지 않는 기준 열 제외 6ea Offset 계산.

				BOOL bIsFirstRow = (nPickerIdx <= VAT_PICKER_04) ? TRUE : FALSE;
				BOOL bIsLeftPicker = bIsFirstRow ? (nPickerIdx < nStdPkrX1) : (nPickerIdx < nStdPkrX2);

				//Offset방향 설정
				int nCalDir = 1;

				if (bIsLeftPicker) {
					if (pAdjustOffsetDir.first == VAT_DIRECTION_PLUSE)		nCalDir = -1;
					else											    nCalDir = 1;				
				}
				else {
					if (pAdjustOffsetDir.first == VAT_DIRECTION_PLUSE)		nCalDir = 1;
					else												nCalDir = -1;
				}

				auto& pickerOffset = g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerOffset[nHand][nPitchType][nPickerIdx].first;
				pickerOffset += nCalDir*dAvgOffsetX;
			}
		}

		if (bIsSpecOutY) {
			for (int nPickerIdx = 0; nPickerIdx < VAT_PICKER_MAX; nPickerIdx++) {
				//기준 행 제외 4ea Offset 계산.
				if (nPickerIdx <= VAT_PICKER_04) { 

					int nCalDir = 1;

					if (pAdjustOffsetDir.second == VAT_DIRECTION_PLUSE)  nCalDir = 1;
					else                                           nCalDir = -1;

					auto& pickerOffset = g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerOffset[nHand][nPitchType][nPickerIdx].second;
					pickerOffset += nCalDir*dAvgOffsetY;
				}
			}
		}
	}
}


void CVAT_CommonData::VAT_CalWideNarrowAvgOffset(int nHand)
{
	for (int nPitchType = 0; nPitchType < VAT_PITCH_TYPE_MAX; nPitchType++)
	{
		std::vector<double> vEachPickerOffsetX; vEachPickerOffsetX.clear();
		std::vector<double> vEachPickerOffsetY; vEachPickerOffsetY.clear();

		for (int nPickerIdx = 0; nPickerIdx < VAT_PICKER_MAX; nPickerIdx++)
		{
			vEachPickerOffsetX.push_back(g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerOffset[nHand][nPitchType][nPickerIdx].first);
			vEachPickerOffsetY.push_back(g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerOffset[nHand][nPitchType][nPickerIdx].second);
		}

		double dMaxOffsetX = *max_element(vEachPickerOffsetX.begin(), vEachPickerOffsetX.end());
		double dMinOffsetX = *min_element(vEachPickerOffsetX.begin(), vEachPickerOffsetX.end());

		double dMaxOffsetY = *max_element(vEachPickerOffsetY.begin(), vEachPickerOffsetY.end());
		double dMinOffsetY = *min_element(vEachPickerOffsetY.begin(), vEachPickerOffsetY.end());


		g_VATCommonData.m_stVATPickerAlignInfoSeq.dPkrWideNarrowAvgOffset[nHand][nPitchType].first = (dMaxOffsetX + dMinOffsetX) / 2;
		g_VATCommonData.m_stVATPickerAlignInfoSeq.dPkrWideNarrowAvgOffset[nHand][nPitchType].second = (dMaxOffsetY + dMinOffsetY) / 2;
	}
}



BOOL CVAT_CommonData::VAT_IsSpecOutOffsetExist(int nHand, int nPitchType, int nPitchAxis)
{
	ASSERT(nPitchAxis == VAT_PITCH_X || nPitchAxis == VAT_PITCH_Y);

	double dOffset = 0.0;

	for (int nPickerIdx = 0; nPickerIdx < VAT_PICKER_MAX; nPickerIdx++) {
		if (nPitchAxis == VAT_PITCH_X) {
			dOffset = fabs(g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerOffset[nHand][nPitchType][nPickerIdx].first);
			if (dOffset > VAT_PICKER_X_OFFSET_SPEC) {
				return TRUE;
			}
		}
		else {
			dOffset = g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerOffset[nHand][nPitchType][nPickerIdx].second;
			if (dOffset > VAT_PICKER_Y_OFFSET_SPEC) {
				return TRUE;
			}
		}
	}

	return FALSE;
}


std::pair<int, int> CVAT_CommonData::VAT_GetPitchOffsetDir(int nHand, int nPitchType)
{
	std::pair<int, int> pReturnDir;
	
	auto Get_MAX_ABS_VALUE = [](const std::vector<double>& vec) -> double {
		return *std::max_element(vec.begin(), vec.end(),
			[](double dOffset1, double dOffset2) { return std::fabs(dOffset1) < std::fabs(dOffset2); });
	};

	//=======================<계산 시작>==========================
	std::vector<double> vLeftPkrOffset;  vLeftPkrOffset.clear(); //기준 Picker 좌측 Offset
	std::vector<double> vRightPkrOffset;  vRightPkrOffset.clear();//기준 PIcker 우측 Offset
	
	//[X Offset Direction Find]
	CPoint cStdPicker = g_VatTaskSystemCtrl.m_vAllTaskAuto[nHand]->_Base_GetStdPicker();
	int nStdPkrX1 = cStdPicker.x;
	int nStdPkrX2 = cStdPicker.x + 4;
	
	for (int nPickerIdx = 0; nPickerIdx < VAT_PICKER_MAX; nPickerIdx++)
	{
		if(nStdPkrX1 == nPickerIdx || nStdPkrX2 == nPickerIdx)
			continue;

		const auto& picker_offset = g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerOffset[nHand][nPitchType][nPickerIdx];

		BOOL bIsFirstRow = (nPickerIdx <= VAT_PICKER_04) ? TRUE : FALSE;
		BOOL bIsLeftPicker = bIsFirstRow ? (nPickerIdx < nStdPkrX1) : (nPickerIdx < nStdPkrX2);

		if (bIsLeftPicker) {
			vLeftPkrOffset.push_back(picker_offset.first);
		}
		else {
			vRightPkrOffset.push_back(picker_offset.first);
		}	
	}

	double dMaxValLeft = Get_MAX_ABS_VALUE(vLeftPkrOffset);
	double dMaxValRight = Get_MAX_ABS_VALUE(vRightPkrOffset);
	BOOL bIsLeftLarge = fabs(dMaxValLeft) > fabs(dMaxValRight);

	if (bIsLeftLarge) {
		pReturnDir.first = (dMaxValLeft > 0) ? VAT_DIRECTION_PLUSE : VAT_DIRECTION_MINUS;
	}
	else {
		pReturnDir.first = (dMaxValRight > 0) ? VAT_DIRECTION_MINUS : VAT_DIRECTION_PLUSE;
	}

	//[Y Offset Direction Find]
	std::vector<double> vPkrOffset;  vPkrOffset.clear(); //기준 Picker열 제외, 1~4번 Picker열은 기준 열이 아님.
	for (int nPickerIdx = 0; nPickerIdx <= VAT_PICKER_04; nPickerIdx++)
	{
		const auto& picker_offset = g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerOffset[nHand][nPitchType][nPickerIdx];

		vPkrOffset.push_back(picker_offset.second);
	}

	pReturnDir.second = (Get_MAX_ABS_VALUE(vPkrOffset) > 0) ? VAT_DIRECTION_MINUS : VAT_DIRECTION_PLUSE;	

	return pReturnDir; //실제 Pitch 움직여야 하는 방향 return
}

int CVAT_CommonData::VAT_GetPartPitchType(int nHand, int nPart)
{
	if (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP)
	{
		if (VAT_IDX_TRAY_PP_TBL_1 <= nPart && nPart <= VAT_IDX_TRAY_PP_TBL_4) {
			return VAT_PITCH_TYPE_WIDE;
		}
		else {
			return VAT_PITCH_TYPE_NARROW;
		}
	}
	else //Test Pp
	{
		if (VAT_IDX_STATION_1 <= nPart && nPart <= VAT_IDX_STATION_4) {
			return VAT_PITCH_TYPE_WIDE;
		}
		else {
			return VAT_PITCH_TYPE_NARROW;
		}
	}
}

void CVAT_CommonData::DataSaveVisionAutoTeaching()
{

}


void CVAT_CommonData::DataSaveZAutoTeaching()
{
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++){
		//Make_AutoTeaching_ZPos(nHand);
		switch (nHand)
		{
		case VAT_HAND_LOAD_TRAYPP:
		{
			VAT_Load_TraySiteCal_ZPos();
		}break;
		case VAT_HAND_UNLOAD_TRAYPP:	
		{
			VAT_Unload_TraySiteCal_ZPos();
		}break;
		case VAT_HAND_LOAD_TESTPP1:		
		case VAT_HAND_UNLOAD_TESTPP1:	
		case VAT_HAND_LOAD_TESTPP2:		
		case VAT_HAND_UNLOAD_TESTPP2:	
		{
			VAT_TestSite_Cal_ZPos(nHand);
		}break;
		default:
			break;
		}
	}

	g_DMCont.m_Teach.Save(); //Save 처리하는 속도가 느리면, Hand만 접근 해서 저장 할 수 있도록 함수 추가 필요.
}

void CVAT_CommonData::VAT_Load_TraySiteCal_ZPos()
{
	//선택 된 Part가 있는지 확인.
	BOOL IsAnyPartSelected = g_VatTaskAutoTrayLoadPP._Base_IsAnyPartSelected();
	if (!IsAnyPartSelected)
		return;


	while (1)
	{
		//[Part Status 'Complete'만 저장]
		int nCurPart = g_VatTaskAutoTrayLoadPP._Base_GetMeasurePart_Selected(VAT_STATUS_MEASURE_COMPLETE);
		if(nCurPart == ERR_VAT_ERROR)
			break; //삭제 하지 말 것.

		//[COK Height] [Pocket Z] [Device Height] - 설계 치수
		double dDeviceHeight = g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[VAT_DEVICE_HEIGHT];

		double dCOKHeight = 0.0;
		double dPocketZ = 0.0;
		if (VAT_IDX_TRAY_PP_TBL_1 <= nCurPart && nCurPart <= VAT_IDX_TRAY_PP_TBL_4) {
			dCOKHeight = g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[VAT_TABLE_HEIGHT];
			dPocketZ = g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[VAT_TABLE_POCKET_Z];
		}
		else { //Feeder
			dCOKHeight = g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[VAT_C_TRAY_JIG_HEIGHT]; //Feeder는 Jig에서 측정.
			dPocketZ = g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[VAT_C_TRAY_POCKET_Z];
		}

		//[Pick/Place Offset]
		int nUnitType = g_VATCommonData.m_stVATZTeachInfo.stUnitZOffset.getUnitType(VAT_HAND_LOAD_TRAYPP, nCurPart);
		double dPickOffset = g_VATCommonData.m_stVATZTeachInfo.stUnitZOffset.getUnitZOffset(VAT_HAND_LOAD_TRAYPP, VAT_Z_PICK_OFFSET, nUnitType);
		double dPlaceOffset = g_VATCommonData.m_stVATZTeachInfo.stUnitZOffset.getUnitZOffset(VAT_HAND_LOAD_TRAYPP, VAT_Z_PLACE_OFFSET, nUnitType);

		//[Vacuum Position]			
		double dRealVacuumPos = g_VATCommonData.m_stVATZTeachInfo.stPickerVacPos.getPickerVacPos(VAT_HAND_LOAD_TRAYPP, nCurPart, VAT_PICKER_01);

		//[Calculate Vacuum Position]	
		double dCalVauumPos = dRealVacuumPos + dCOKHeight - dPocketZ - dDeviceHeight;

		//[DATA SAVE]
		double set_pick_pos = dCalVauumPos + dPickOffset;
		double set_place_pos = set_pick_pos - dPlaceOffset;
		
		int teach_loc_idx = g_TaskTrayLoadPP.TrayHandLocIdxConvertor(RIDX_TRAY_PP_1, nCurPart); //Part에 따라 Teach Loc Convert

		g_DMCont.m_Teach.SetTeachPos(tRobotId_TRAYPP_1, teach_loc_idx, POS_IDX_TRAY_LOAD_PP_Z_PICK, set_pick_pos);
		g_DMCont.m_Teach.SetTeachPos(tRobotId_TRAYPP_1, teach_loc_idx, POS_IDX_TRAY_LOAD_PP_Z_PLACE, set_place_pos);

		//Part 상태 변경 [MEASURE COMPLETE -> DATA SAVE COMPLETE]
		g_VatTaskAutoTrayLoadPP._Base_SetMeasurePart_Status(nCurPart, VAT_STATUS_DATA_SAVE_COMPLETE);

		Sleep(1);
	}
}

void CVAT_CommonData::VAT_Unload_TraySiteCal_ZPos()
{
	//선택 된 Part가 있는지 확인.
	BOOL IsAnyPartSelected = g_VatTaskAutoTrayUnloadPP._Base_IsAnyPartSelected();
	if (!IsAnyPartSelected)
		return;

	while (1)
	{
		//[Part Status 'Complete'만 저장]
		int nCurPart = g_VatTaskAutoTrayUnloadPP._Base_GetMeasurePart_Selected(VAT_STATUS_MEASURE_COMPLETE);
		if (nCurPart == ERR_VAT_ERROR)
			break; //삭제 하지 말 것.

		//[COK Height] [Pocket Z] [Device Height] - 설계 치수
		double dDeviceHeight = g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[VAT_DEVICE_HEIGHT];

		double dCOKHeight = 0.0;
		double dPocketZ = 0.0;
		if (VAT_IDX_TRAY_PP_TBL_1 <= nCurPart && nCurPart <= VAT_IDX_TRAY_PP_TBL_4) {
			dCOKHeight = g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[VAT_TABLE_HEIGHT];
			dPocketZ = g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[VAT_TABLE_POCKET_Z];
		}
		else {
			dCOKHeight = g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[VAT_C_TRAY_JIG_HEIGHT]; //Stacker는 Jig에서 측정.
			dPocketZ = g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[VAT_C_TRAY_POCKET_Z];	
		}

		//[Pick/Place Offset]
		int nUnitType = g_VATCommonData.m_stVATZTeachInfo.stUnitZOffset.getUnitType(VAT_HAND_UNLOAD_TRAYPP, nCurPart);
		double dPickOffset = g_VATCommonData.m_stVATZTeachInfo.stUnitZOffset.getUnitZOffset(VAT_HAND_UNLOAD_TRAYPP, VAT_Z_PICK_OFFSET, nUnitType);
		double dPlaceOffset = g_VATCommonData.m_stVATZTeachInfo.stUnitZOffset.getUnitZOffset(VAT_HAND_UNLOAD_TRAYPP, VAT_Z_PLACE_OFFSET, nUnitType);

		//[Vacuum Position]			
		double dRealVacuumPos = g_VATCommonData.m_stVATZTeachInfo.stPickerVacPos.getPickerVacPos(VAT_HAND_UNLOAD_TRAYPP, nCurPart, VAT_PICKER_01);

		//[Calculate Vacuum Position]	
		double dCalVauumPos = dRealVacuumPos + dCOKHeight - dPocketZ - dDeviceHeight;

		//[DATA SAVE]
		double set_pick_pos = dCalVauumPos + dPickOffset;
		double set_place_pos = set_pick_pos - dPlaceOffset;
	
		int teach_loc_idx = g_TaskTrayLoadPP.TrayHandLocIdxConvertor(RIDX_TRAY_PP_2, nCurPart); //Part에 따라 Teach Loc Convert

		g_DMCont.m_Teach.SetTeachPos(tRobotId_TRAYPP_2, teach_loc_idx, POS_IDX_TRAY_PP_Z_PICK, set_pick_pos);
		g_DMCont.m_Teach.SetTeachPos(tRobotId_TRAYPP_2, teach_loc_idx, POS_IDX_TRAY_PP_Z_PLACE, set_place_pos);

		//Part 상태 변경 [MEASURE COMPLETE -> DATA SAVE COMPLETE]
		g_VatTaskAutoTrayUnloadPP._Base_SetMeasurePart_Status(nCurPart, VAT_STATUS_DATA_SAVE_COMPLETE);

		Sleep(1);
	}
}

void CVAT_CommonData::VAT_TestSite_Cal_ZPos(int nHand)
{

	CVatBaseTask* _BaseTaskAuto = NULL;
	switch (nHand)
	{
	case VAT_HAND_LOAD_TESTPP1:		_BaseTaskAuto = &g_VatTaskAutoLoadTestPP1;		break;
	case VAT_HAND_UNLOAD_TESTPP1:	_BaseTaskAuto = &g_VatTaskAutoUnloadTestPP1;		break;
	case VAT_HAND_LOAD_TESTPP2:		_BaseTaskAuto = &g_VatTaskAutoLoadTestPP2;		break;
	case VAT_HAND_UNLOAD_TESTPP2:	_BaseTaskAuto = &g_VatTaskAutoUnloadTestPP2;		break;
	default:
		break;
	}

	//선택 된 Part가 있는지 확인.
	BOOL IsAnyPartSelected = _BaseTaskAuto->_Base_IsAnyPartSelected();
	if (!IsAnyPartSelected)
		return;

	while (1)
	{
		//[Part Status 'Complete'만 저장]
		int nCurPart = _BaseTaskAuto->_Base_GetMeasurePart_Selected(VAT_STATUS_MEASURE_COMPLETE);
		if (nCurPart == ERR_VAT_ERROR)
			break; //삭제 하지 말 것.

		//[COK Height] [Pocket Z] [Device Height] - 설계 치수
		double dDeviceHeight = g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[VAT_DEVICE_HEIGHT];

		double dCOKHeight = 0.0;
		double dPocketZ = 0.0;
		if (VAT_IDX_STATION_1 <= nCurPart && nCurPart <= VAT_IDX_STATION_4) {
			dCOKHeight = g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[VAT_STATION_HEIGHT];
			dPocketZ = g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[VAT_STATION_SOCKET_Z];
		}
		else if(VAT_IDX_TEST_PP_TBL_1_LD <= nCurPart && nCurPart <= VAT_IDX_TEST_PP_TBL_4_UD) {
			dCOKHeight = g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[VAT_TABLE_HEIGHT];
			dPocketZ = g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[VAT_TABLE_POCKET_Z];
		}
		else { //Clean Table
			dCOKHeight = g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[VAT_CLEAN_TABLE_HEIGHT];
			dPocketZ = g_VATCommonData.m_stVATDesignValueConstInfo.dZAutoTeaching[VAT_CLEAN_TABLE_POCKET_Z];
		}


		//[Pick/Place Offset]
		int nUnitType = g_VATCommonData.m_stVATZTeachInfo.stUnitZOffset.getUnitType(nHand, nCurPart);
		double dPickOffset = g_VATCommonData.m_stVATZTeachInfo.stUnitZOffset.getUnitZOffset(nHand, VAT_Z_PICK_OFFSET, nUnitType);
		double dPlaceOffset = g_VATCommonData.m_stVATZTeachInfo.stUnitZOffset.getUnitZOffset(nHand, VAT_Z_PLACE_OFFSET, nUnitType);

		//[Vacuum Position]			
		double dRealVacuumPos = g_VATCommonData.m_stVATZTeachInfo.stPickerVacPos.getPickerVacPos(nHand, nCurPart, VAT_PICKER_01);

		//[Calculate Vacuum Position]	
		double dCalVauumPos = dRealVacuumPos + dCOKHeight - dPocketZ - dDeviceHeight;

		//[DATA SAVE]
		double set_pick_pos = dCalVauumPos + dPickOffset;
		double set_place_pos = set_pick_pos - dPlaceOffset;

		int nRobotId = (nHand == VAT_HAND_LOAD_TESTPP1 || nHand == VAT_HAND_UNLOAD_TESTPP1) ? tRobotId_TESTPP_1 : tRobotId_TESTPP_2;// TestPp1/TestPp2
		int nPosPickIdx = (nHand == VAT_HAND_LOAD_TESTPP1 || nHand == VAT_HAND_LOAD_TESTPP2) ? POS_IDX_TEST_PP_Z1_PICK : POS_IDX_TEST_PP_Z2_PICK; // LD/UD
		int nPosPlaceIdx = (nHand == VAT_HAND_LOAD_TESTPP1 || nHand == VAT_HAND_LOAD_TESTPP2) ? POS_IDX_TEST_PP_Z1_PLACE : POS_IDX_TEST_PP_Z2_PLACE; // LD/UD

		g_DMCont.m_Teach.SetTeachPos(nRobotId, nCurPart, nPosPickIdx, set_pick_pos);
		g_DMCont.m_Teach.SetTeachPos(nRobotId, nCurPart, nPosPlaceIdx, set_place_pos);

		//Part 상태 변경 [MEASURE COMPLETE -> DATA SAVE COMPLETE]
		_BaseTaskAuto->_Base_SetMeasurePart_Status(nCurPart, VAT_STATUS_DATA_SAVE_COMPLETE);

		Sleep(1);
	}
}

void CVAT_CommonData::LoadVATData()
{
	//File 유무 Check
	IsExistVATDirectory();

	LoadZAutoFocusInformation();
	LoadCamCenterCalInformation();
	LoadPickerAlignInformation();
	LoadZAutoTeachingInformation(); 
	LoadVisionAutoTeachingInformation();

	//LoadPartBasePosition(); //필요 없음.

	LoadDesignValue();

	LoadVATOption();
}

void CVAT_CommonData::SaveVATData()
{		
	SaveZAutoFocusInformation();
	SaveCamCenterCalInformation();			
	SavePickerAlignInformation();			
	SaveZAutoTeachingInformation();			
	SaveVisionAutoTeachingInformation();		

	//SavePartBasePosition();
}


//=================================================================================
// COMMENT : Data 공통으로 저장.(Hand 별로 분기 X)
//=================================================================================
CString CVAT_CommonData::GetFilePathCom(int nMode)
{
	char szCokName[128] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));

	CString strFilePath = _T("");

	switch (nMode)
	{
	case VAT_DATA_PATH_DESIGN_VALUE:
		strFilePath.Format("sys_data\\HandlerRecipe\\%s\\%s\\VAT\\VAT_DesignValue.dat", SZ_TRAY_FOLDER, szCokName /*strHandName*/);
		break;
	case VAT_DATA_PATH_OPTION:
		strFilePath.Format("sys_data\\HandlerRecipe\\%s\\%s\\VAT\\VAT_Option.dat", SZ_TRAY_FOLDER, szCokName /*strHandName*/);
		break;
	default:
		break;
	}

	return strFilePath;
}


//=================================================================================
// COMMENT : nHand의 경우 Hand별로 Data를 분기해야 할때 사용.
//=================================================================================
 CString CVAT_CommonData::GetFilePathHand(int nMode, int nHand)
 {
	 //ASSERT(VAT_HAND_LOAD_TRAYPP <= nHand && nHand <= VAT_HAND_UNLOAD_TESTPP2);

	 char szCokName[128] = { 0, };
	 g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));

	 CString strHandName = GetHandString(nHand);


	 CString strFilePath = _T("");

	 switch (nMode)
	 {
	 case VAT_DATA_PATH_Z_FOCUS:
		 strFilePath.Format("sys_data\\HandlerRecipe\\%s\\%s\\VAT\\%s\\1.VAT_ZAutoFocusInfo.dat", SZ_TRAY_FOLDER, szCokName, strHandName);
		 break;
	 case VAT_DATA_PATH_CAM_CENTER_CAL:
		 strFilePath.Format("sys_data\\HandlerRecipe\\%s\\%s\\VAT\\%s\\2.VAT_CamCenterCalInfo.dat", SZ_TRAY_FOLDER, szCokName, strHandName);
		 break;
	 case VAT_DATA_PATH_PICKER_ALIGN:
		 strFilePath.Format("sys_data\\HandlerRecipe\\%s\\%s\\VAT\\%s\\3.VAT_PickerAlignInfo.dat", SZ_TRAY_FOLDER, szCokName, strHandName);
		 break;
	 case VAT_DATA_PATH_VISION_AUTO_TEACHING:
		 strFilePath.Format("sys_data\\HandlerRecipe\\%s\\%s\\VAT\\%s\\4.VAT_VisionAutoTeachingInfo.dat", SZ_TRAY_FOLDER, szCokName, strHandName);
		 break;
	 case VAT_DATA_PATH_Z_AUTO_TEACHING:
		 strFilePath.Format("sys_data\\HandlerRecipe\\%s\\%s\\VAT\\%s\\5.VAT_ZTeachingInfo.dat", SZ_TRAY_FOLDER, szCokName, strHandName);
		 break;
// 	 case VAT_DATA_PATH_PART_BASE_POSITION:
// 		 strFilePath.Format("sys_data\\HandlerRecipe\\%s\\%s\\VAT\\%s\\VAT_PartBasePos.dat", SZ_TRAY_FOLDER, szCokName, strHandName);
//  		 break;
	 default:
		 break;
	 }

	 return strFilePath;
 }


 CString CVAT_CommonData::GetHandString(int nHand)
 {
	 CString strHand = _T("");
	 switch (nHand)
	 {
	 case VAT_HAND_LOAD_TRAYPP:		strHand = _T("1.LD TRAY PP");		break;
	 case VAT_HAND_UNLOAD_TRAYPP:	strHand = _T("2.UD TRAY PP");		break;
	 case VAT_HAND_LOAD_TESTPP1:	strHand = _T("3.LD TEST PP #1");	break;
	 case VAT_HAND_UNLOAD_TESTPP1:	strHand = _T("4.UD TEST PP #1");	break;
	 case VAT_HAND_LOAD_TESTPP2:	strHand = _T("5.LD TEST PP #2");	break;
	 case VAT_HAND_UNLOAD_TESTPP2:	strHand = _T("6.UD TEST PP #2");	break;
	 default:
		 break;
	 }

	 return strHand;
 }


 CString CVAT_CommonData::GetFilePathPart(int nVATPart)
 {
	 char szCokName[128] = { 0, };
	 g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));

	 CString strPartName = GetPartString(nVATPart);

	 CString strFilePath = _T("");
	 strFilePath.Format("sys_data\\HandlerRecipe\\%s\\%s\\VAT\\%s\\PartBasePos.dat", SZ_TRAY_FOLDER, szCokName, strPartName);

	 return strFilePath;
 }


CString CVAT_CommonData::GetPartString(int nVATPart)
{
	CString strPart = _T("");

	switch (nVATPart)
	{
	case VAT_PART_FEEDER:		strPart = _T("7.FEEDER");	break;
	case VAT_PART_TABLE_1:		strPart = _T("8.TABLE_1");	break;
	case VAT_PART_TABLE_2:		strPart = _T("9.TABLE_2");	break;
	case VAT_PART_TABLE_3:		strPart = _T("10.TABLE_3");	break;
	case VAT_PART_TABLE_4:		strPart = _T("11.TABLE_4");	break;
	default:
		break;
	}

	return strPart;
}


 void CVAT_CommonData::IsExistVATDirectory()
 {
 	char szCokName[128] = { 0, };
 	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
 
	//VAT Log 폴더 확인.
	CString strLogFilePath = _T("");
	strLogFilePath.Format("sys_data\\HandlerRecipe\\%s\\%s\\VAT\\", SZ_TRAY_FOLDER, szCokName);

	if (GetFileAttributes(strLogFilePath) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strLogFilePath, NULL);
	}

	//Hand 폴더 확인.
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{
		CString strHandName = GetHandString(nHand);

		CString strHandFilePath = _T("");
		strHandFilePath.Format("sys_data\\HandlerRecipe\\%s\\%s\\VAT\\%s\\", SZ_TRAY_FOLDER, szCokName, strHandName);

		if (GetFileAttributes(strHandFilePath) == INVALID_FILE_ATTRIBUTES) {
			CreateDirectory(strHandFilePath, NULL);
		}
	}

	//Part 폴더 확인.
	for (int nVATPart = 0; nVATPart < VAT_PART_MAX; nVATPart++)
	{
		CString strPartName = GetPartString(nVATPart);

		CString strPartFilePath = _T("");
		strPartFilePath.Format("sys_data\\HandlerRecipe\\%s\\%s\\VAT\\%s\\", SZ_TRAY_FOLDER, szCokName, strPartName);

		if (GetFileAttributes(strPartFilePath) == INVALID_FILE_ATTRIBUTES) {
			CreateDirectory(strPartFilePath, NULL);
		}
	}
 }

void CVAT_CommonData::LoadZAutoFocusInformation()
{
	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++) {

		CConfigData cfData((LPSTR)(LPCTSTR)GetFilePathHand(VAT_DATA_PATH_Z_FOCUS, nHand));

		for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XYZ; nAxis++) {
			//Real
			m_stVATZFocusInfo.dPlateJIG_TargetPos[nHand][nAxis] = cfData.GetDouble(m_stVATFileSection.pZFocusSec_PlateJigTargetPos, m_stVATFileSection.pKeyAxisXYZ[nAxis], "0.0");
			m_stVATZFocusInfo.dPlateJIG_VacuumPos[nHand][nAxis] = cfData.GetDouble(m_stVATFileSection.pZFocusSec_PlateJigVacPos, m_stVATFileSection.pKeyAxisXYZ[nAxis], "0.0");
			
			//Buffer
			m_stVATZFocusInfoBuff.dPlateJIG_TargetPos[nHand][nAxis] = cfData.GetDouble(m_stVATFileSection.pZFocusSecBuff_PlateJigTargetPos, m_stVATFileSection.pKeyAxisXYZ[nAxis], "0.0");
			m_stVATZFocusInfoBuff.dPlateJIG_VacuumPos[nHand][nAxis] = cfData.GetDouble(m_stVATFileSection.pZFocusSecBuff_PlateJigVacPos, m_stVATFileSection.pKeyAxisXYZ[nAxis], "0.0");
		}


		//측정 완료 Data
		for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XY; nAxis++) {
			//Real
			m_stVATZFocusInfo.dPlateJIG_LowerOffset[nHand][nAxis] = cfData.GetDouble(m_stVATFileSection.pZFocusSec_PlateJigLowerOffset, m_stVATFileSection.pKeyAxisXY[nAxis], "0.0");

			//Buffer
			m_stVATZFocusInfoBuff.dPlateJIG_LowerOffset[nHand][nAxis] = cfData.GetDouble(m_stVATFileSection.pZFocusSecBuff_PlateJigLowerOffset, m_stVATFileSection.pKeyAxisXY[nAxis], "0.0");
		}
	}
}


void CVAT_CommonData::LoadCamCenterCalInformation()
{	
	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++) {
		CConfigData cfData((LPSTR)(LPCTSTR)GetFilePathHand(VAT_DATA_PATH_CAM_CENTER_CAL, nHand));
		for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XYZ; nAxis++) {
			//Real
			m_stVATCamCenterCalInfo.dBotCameraPosition[nHand][nAxis] = cfData.GetDouble(m_stVATFileSection.pCamCenCalSec_BotCamCenterPos, m_stVATFileSection.pKeyAxisXYZ[nAxis], "0.0");
			m_stVATCamCenterCalInfo.dTopCamToPkrDistance[nHand][nAxis] = cfData.GetDouble(m_stVATFileSection.pCamCenCalSec_TopCamToPkrDis, m_stVATFileSection.pKeyAxisXYZ[nAxis], "0.0");

			//Buff
			m_stVATCamCenterCalInfoBuff.dBotCameraPosition[nHand][nAxis] = cfData.GetDouble(m_stVATFileSection.pCamCenCalSecBuff_BotCamCenterPos, m_stVATFileSection.pKeyAxisXYZ[nAxis], "0.0");
			m_stVATCamCenterCalInfoBuff.dTopCamToPkrDistance[nHand][nAxis] = cfData.GetDouble(m_stVATFileSection.pCamCenCalSecBuff_TopCamToPkrDis, m_stVATFileSection.pKeyAxisXYZ[nAxis], "0.0");
		}
	}
}


//buffer는 따로 없음.
void CVAT_CommonData::LoadPickerAlignInformation()
{
 	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++)
 	{
 		CConfigData cfData((LPSTR)(LPCTSTR)GetFilePathHand(VAT_DATA_PATH_PICKER_ALIGN, nHand));
		
		/*초기 File Data load 순서 저렬을 위해 For문 따로 배치*/

		//[1. Selected Picker]
		for (int nPickerIdx = VAT_PICKER_01; nPickerIdx < VAT_PICKER_MAX; nPickerIdx++) {
			m_stVATPickerAlignInfo.nSelectedPicker[nHand][nPickerIdx] = cfData.GetInt(m_stVATFileSection.pPickerAlignSec_SelectedPkr, m_stVATFileSection.pKeyPicker[nPickerIdx], "1");
		}
		
		//[2. Picker Offset] - Wide/Narrow
		for (int nPitchType = 0; nPitchType < VAT_PITCH_TYPE_MAX; nPitchType++) {
			for (int nPickerIdx = VAT_PICKER_01; nPickerIdx < VAT_PICKER_MAX; nPickerIdx++) {
				m_stVATPickerAlignInfo.pEachPickerOffset[nHand][nPitchType][nPickerIdx].first = cfData.GetDouble(m_stVATFileSection.pPickerAlignSec_PickerOffset[nPitchType], m_stVATFileSection.pKeyPickerX[nPickerIdx], "0.0");
				m_stVATPickerAlignInfo.pEachPickerOffset[nHand][nPitchType][nPickerIdx].second = cfData.GetDouble(m_stVATFileSection.pPickerAlignSec_PickerOffset[nPitchType], m_stVATFileSection.pKeyPickerY[nPickerIdx], "0.0");
			}
		}

		//[3. Picker Position] - Wide/Narrow
		for (int nPitchType = 0; nPitchType < VAT_PITCH_TYPE_MAX; nPitchType++) {
			for (int nPickerIdx = VAT_PICKER_01; nPickerIdx < VAT_PICKER_MAX; nPickerIdx++) {
				m_stVATPickerAlignInfo.pEachPickerPos[nHand][nPitchType][nPickerIdx].first = cfData.GetDouble(m_stVATFileSection.pPickerAlignSec_PickerPos[nPitchType], m_stVATFileSection.pKeyPickerX[nPickerIdx], "0.0");
				m_stVATPickerAlignInfo.pEachPickerPos[nHand][nPitchType][nPickerIdx].second = cfData.GetDouble(m_stVATFileSection.pPickerAlignSec_PickerPos[nPitchType], m_stVATFileSection.pKeyPickerY[nPickerIdx], "0.0");
			}
		}

		//[4. Picker Wide/Narrow Avg Offset]
		for (int nPitchType = 0; nPitchType < VAT_PITCH_TYPE_MAX; nPitchType++) {
			m_stVATPickerAlignInfo.dPkrWideNarrowAvgOffset[nHand][nPitchType].first = cfData.GetDouble(m_stVATFileSection.pPickerAlignSec_WideNarrowAvgOffset[nPitchType], m_stVATFileSection.pKeyX, "0.0");
			m_stVATPickerAlignInfo.dPkrWideNarrowAvgOffset[nHand][nPitchType].second = cfData.GetDouble(m_stVATFileSection.pPickerAlignSec_WideNarrowAvgOffset[nPitchType], m_stVATFileSection.pKeyY, "0.0");
		}

		//[5. Picker간 Z Focus Position]
		for (int nPickerIdx = VAT_PICKER_01; nPickerIdx < VAT_PICKER_MAX; nPickerIdx++) {
			m_stVATPickerAlignInfo.dPickerZFocusPos[nHand][nPickerIdx] = cfData.GetDouble(m_stVATFileSection.pPickerAlignSec_ZFocusPos, m_stVATFileSection.pKeyPicker[nPickerIdx], "0.0");
		}


		//Picker Sub Info 작업 완료 후, 추가 필요 함.
 		for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XY; nAxis++)
 		{
 			m_stVAT_PickerAlignSubInfo.dPreSettingPos[nHand][nAxis] = cfData.GetDouble(m_stVATFileSection.pPickerAlignSecSub_PrePos, m_stVATFileSection.pKeyAxisXY[nAxis], "0.0");
 		}
 	}
}

void CVAT_CommonData::LoadZAutoTeachingInformation()
{	
	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++)
	{
		CConfigData cfData((LPSTR)(LPCTSTR)GetFilePathHand(VAT_DATA_PATH_Z_AUTO_TEACHING, nHand));

		int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;

		//============Real
		//m_stVATZTeachConstInfo.dZPickOffset[nHand] = cfData.GetDouble(m_stVATFileSection.pZAutoTeachingSec_SettingParam, m_stVATFileSection.pKeyPickOffset, "2000.0");
		//m_stVATZTeachConstInfo.dZPlaceOffset[nHand] = cfData.GetDouble(m_stVATFileSection.pZAutoTeachingSec_SettingParam, m_stVATFileSection.pKeyPlaceOffset, "2000.0");
		
		//Z Pick/Place Offset
		int nMAX_UNIT = (nHand == VAT_HAND_LOAD_TESTPP1 || nHand == VAT_HAND_LOAD_TESTPP2) ? VAT_Z_OFFSET_UNIT_MAX : VAT_Z_OFFSET_UNIT_MIN;
		for (int nUnit = 0; nUnit < nMAX_UNIT; nUnit++) {
			//Z Pick Offset
			double dZPickOffset = cfData.GetDouble(m_stVATFileSection.pZAutoTeachingSec_ZPickPlaceOffset[VAT_Z_PICK_OFFSET], m_stVATFileSection.pKeyZOffset[nHand][nUnit], "500");
			m_stVATZTeachInfo.stUnitZOffset.setUnitZOffset(nHand, VAT_Z_PICK_OFFSET, nUnit, dZPickOffset);

			//Z Place Offset
			double dZPlaceOffset = cfData.GetDouble(m_stVATFileSection.pZAutoTeachingSec_ZPickPlaceOffset[VAT_Z_PLACE_OFFSET], m_stVATFileSection.pKeyZOffset[nHand][nUnit], "2000");
			m_stVATZTeachInfo.stUnitZOffset.setUnitZOffset(nHand, VAT_Z_PLACE_OFFSET, nUnit, dZPlaceOffset);
		}
		

		for (int nPart = 0; nPart < nMaxPart; nPart++) {
			if (m_stVATFileSection._IsNecessaryPart(nHand, nPart) != TRUE)
				continue;
		
			char *pSecZTeachGapPos = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? m_stVATFileSection.pZAutoTeachingSec_PickerGap_TrayPP[nPart] : m_stVATFileSection.pZAutoTeachingSec_PickerGap_TesstPP[nPart];
			char *pSecZTeachVacuumPos = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? m_stVATFileSection.pZAutoTeachingSec_PickerVacPos_TrayPP[nPart] : m_stVATFileSection.pZAutoTeachingSec_PickerVacPos_TestPP[nPart];
			for (int nPicker = 0; nPicker < VAT_PICKER_MAX; nPicker++) {
				//Picker Gap
				double dGap = cfData.GetDouble(pSecZTeachGapPos, m_stVATFileSection.pKeyHandPickerNum[nPicker], "0.0");
				m_stVATZTeachInfo.stPickerGap.setPickerGap(nHand, nPart, nPicker, dGap);

				//Picker Vac Pos
				double dVacPos = cfData.GetDouble(pSecZTeachVacuumPos, m_stVATFileSection.pKeyHandPickerNum[nPicker], "0.0");
				m_stVATZTeachInfo.stPickerVacPos.setPickerVacPos(nHand, nPart, nPicker, dVacPos);
			}

			//Base Position
			char *pSecZTeachBasePos = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? m_stVATFileSection.pVATSec_TrayPpBasePos[nPart] : m_stVATFileSection.pVATSec_TestPpBasePos[nPart];
			for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XYZ; nAxis++) {	
				double dBasePosReal = cfData.GetDouble(pSecZTeachBasePos, m_stVATFileSection.pKeyAxisXYZ[nAxis], "0.0");
				m_stVATZTeachInfo.stZTeachBasePos.setPartBasePos(nHand, nPart, nAxis, dBasePosReal);
			}
		}		
	}

}


//초기 File에 Data 생성될 때, 보기 편한 구조로 정렬.
void CVAT_CommonData::LoadVisionAutoTeachingInformation()
{
	//[1. Hand Base Position]
	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++)
	{
		CConfigData cfData((LPSTR)(LPCTSTR)GetFilePathHand(VAT_DATA_PATH_VISION_AUTO_TEACHING, nHand));

		int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
		
		//===============Real
		for (int nPart = 0; nPart < nMaxPart; nPart++)
		{
			char *pSecHandBasePos = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? m_stVATFileSection.pVATSec_TrayPpBasePos[nPart] : m_stVATFileSection.pVATSec_TestPpBasePos[nPart];
			for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XYZ; nAxis++) {			
				double dBasePosReal = cfData.GetDouble(pSecHandBasePos, m_stVATFileSection.pKeyAxisXYZ[nAxis], "0.0");
				m_stVATInfo.stHandBasePos.setBasePos(nHand, nPart, nAxis, dBasePosReal);
			}
		}
		
		//===============Buff
		for (int nPart = 0; nPart < nMaxPart; nPart++)
		{
			char *pSecHandBasePosBuff = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? m_stVATFileSection.pVATSecBuff_TrayPpBasePos[nPart] : m_stVATFileSection.pVATSecBuff_TestPpBasePos[nPart];
			for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XYZ; nAxis++) {
				double dBasePosBuff = cfData.GetDouble(pSecHandBasePosBuff, m_stVATFileSection.pKeyAxisXYZ[nAxis], "0.0");
				m_stVATInfoBuff.stHandBasePos.setBasePos(nHand, nPart, nAxis, dBasePosBuff);
			}
		}
	}

	//[2. Part Base Position]
	for (int nVATPart = 0; nVATPart < VAT_PART_MAX; nVATPart++)
	{
		CConfigData cfData((LPSTR)(LPCTSTR)GetFilePathPart(nVATPart));
	
		int nMAX_PART_POS = (nVATPart == VAT_PART_FEEDER) ? VAT_MAX_FEEDER_LOC_COUNT : VAT_MAX_TBL_LOC_COUNT;
		int nMAX_AXIS = (nVATPart == VAT_PART_FEEDER) ? VAT_MAX_AXIS_Y : VAT_MAX_AXIS_YROTATE;

		//===============Real
		for (int nPartPos = 0; nPartPos < nMAX_PART_POS; nPartPos++)
		{
			char *pSecPartBasePos = (nVATPart == VAT_PART_FEEDER) ? m_stVATFileSection.pVATSec_PartFeederBasePos[nPartPos] : m_stVATFileSection.pVATSec_PartTableBasePos[nPartPos];
			for (int nAxis = 0; nAxis < nMAX_AXIS; nAxis++) {
				double dBasePosReal = cfData.GetDouble(pSecPartBasePos, m_stVATFileSection.pKeyAxisYRotate[nAxis], "0.0");
				m_stVATInfo.stPartBasePos.setBasePos(nVATPart, nPartPos, nAxis, dBasePosReal);
			}
		}

		//===============Buff
		for (int nPartPos = 0; nPartPos < nMAX_PART_POS; nPartPos++)
		{
			char *pSecPartBasePos = (nVATPart == VAT_PART_FEEDER) ? m_stVATFileSection.pVATSecBuff_PartFeederBasePos[nPartPos] : m_stVATFileSection.pVATSecBuff_PartTableBasePos[nPartPos];
			for (int nAxis = 0; nAxis < nMAX_AXIS; nAxis++) {
				double dBasePosReal = cfData.GetDouble(pSecPartBasePos, m_stVATFileSection.pKeyAxisYRotate[nAxis], "0.0");
				m_stVATInfoBuff.stPartBasePos.setBasePos(nVATPart, nPartPos, nAxis, dBasePosReal);
			}
		}
	}

}


// void CVAT_CommonData::LoadPartBasePosition()
// {
// 	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++)
// 	{
// 		CConfigData cfData((LPSTR)(LPCTSTR)GetFilePath(VAT_DATA_PATH_PART_BASE_POSITION, nHand));
// 
// 		int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
// 
// 		//===============Real
// 		for (int nPart = 0; nPart < nMaxPart; nPart++)
// 		{
// 			for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XYZ; nAxis++) {
// 				
// 				char *pSecPartBasePos = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? m_stVATFileSection.pPartBasePos_TrayPP[nPart] : m_stVATFileSection.pPartBasePos_TestPP[nPart];
// 				double dBasePosReal = cfData.GetDouble(pSecPartBasePos, m_stVATFileSection.pKeyAxisXYZ[nAxis], "0.0");
// 				m_stVATPartBasePos.setPartBasePos(nHand, nPart, nAxis, dBasePosReal);
// 			}
// 		}
// 
// 		//===============Buff
// 		for (int nPart = 0; nPart < nMaxPart; nPart++)
// 		{
// 			for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XYZ; nAxis++) {
// 				char *pSecPartBasePosBuff = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? m_stVATFileSection.pPartBasePosBuff_TrayPP[nPart] : m_stVATFileSection.pPartBasePosBuff_TestPP[nPart];
// 				double dBasePosBuff = cfData.GetDouble(pSecPartBasePosBuff, m_stVATFileSection.pKeyAxisXYZ[nAxis], "0.0");
// 				m_stVATPartBasePosBuff.setPartBasePos(nHand, nPart, nAxis, dBasePosBuff);
// 			}
// 		}
// 	}
// }


void CVAT_CommonData::LoadDesignValue()
{
	CConfigData cfData((LPSTR)(LPCTSTR)GetFilePathCom(VAT_DATA_PATH_DESIGN_VALUE));

	for (int nIdx = 0; nIdx < VAT_MAX_DESIGN_VALUE_Z_AUTO_TEACHING; nIdx++) {
		m_stVATDesignValueConstInfo.dZAutoTeaching[nIdx] = cfData.GetDouble(m_stVATFileSection.pDesignValue_ZAutoTeaching[nIdx], m_stVATFileSection.pKeyValue, "0.0");
	}

}


void CVAT_CommonData::LoadVATOption()
{
	CConfigData cfData((LPSTR)(LPCTSTR)GetFilePathCom(VAT_DATA_PATH_OPTION));

	m_stVATOption.nPickerAutoFocus = cfData.GetInt(m_stVATFileSection.pOption_PkrAutoFocus, m_stVATFileSection.pkeyOption, "1");
}



void CVAT_CommonData::SaveZAutoFocusInformation()
{
	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++)
	{
		CConfigData cfData((LPSTR)(LPCTSTR)GetFilePathHand(VAT_DATA_PATH_Z_FOCUS, nHand));

		for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XYZ; nAxis++) {
			//Real
			cfData.Set(m_stVATFileSection.pZFocusSec_PlateJigTargetPos,		 m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATZFocusInfo.dPlateJIG_TargetPos[nHand][nAxis]);
			cfData.Set(m_stVATFileSection.pZFocusSec_PlateJigVacPos,		 m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATZFocusInfo.dPlateJIG_VacuumPos[nHand][nAxis]);
		
			//Buffer
			cfData.Set(m_stVATFileSection.pZFocusSecBuff_PlateJigTargetPos,  m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATZFocusInfoBuff.dPlateJIG_TargetPos[nHand][nAxis]);
			cfData.Set(m_stVATFileSection.pZFocusSecBuff_PlateJigVacPos,     m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATZFocusInfoBuff.dPlateJIG_VacuumPos[nHand][nAxis]);
		}

		for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XY; nAxis++) {
			//Real
			cfData.Set(m_stVATFileSection.pZFocusSec_PlateJigLowerOffset, m_stVATFileSection.pKeyAxisXY[nAxis], m_stVATZFocusInfo.dPlateJIG_LowerOffset[nHand][nAxis]);	

			//Buff
			cfData.Set(m_stVATFileSection.pZFocusSecBuff_PlateJigLowerOffset, m_stVATFileSection.pKeyAxisXY[nAxis], m_stVATZFocusInfoBuff.dPlateJIG_LowerOffset[nHand][nAxis]);
			
		}			
	}
}


void CVAT_CommonData::SaveCamCenterCalInformation()
{
	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++) {

		CConfigData cfData((LPSTR)(LPCTSTR)GetFilePathHand(VAT_DATA_PATH_CAM_CENTER_CAL, nHand));

		for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XYZ; nAxis++) {
			//Real
			cfData.Set(m_stVATFileSection.pCamCenCalSec_BotCamCenterPos, m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATCamCenterCalInfo.dBotCameraPosition[nHand][nAxis]);
			cfData.Set(m_stVATFileSection.pCamCenCalSec_TopCamToPkrDis, m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATCamCenterCalInfo.dTopCamToPkrDistance[nHand][nAxis]);

			//Buffer
			cfData.Set(m_stVATFileSection.pCamCenCalSecBuff_BotCamCenterPos, m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATCamCenterCalInfoBuff.dBotCameraPosition[nHand][nAxis]);
			cfData.Set(m_stVATFileSection.pCamCenCalSecBuff_TopCamToPkrDis, m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATCamCenterCalInfoBuff.dTopCamToPkrDistance[nHand][nAxis]);
		}
	}
}


void CVAT_CommonData::SavePickerAlignInformation()
{
  	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++)
  	{
  		CConfigData cfData((LPSTR)(LPCTSTR)GetFilePathHand(VAT_DATA_PATH_PICKER_ALIGN, nHand));
  
		for (int nPickerIdx = VAT_PICKER_01; nPickerIdx < VAT_PICKER_MAX; nPickerIdx++) {
			//[1. Selected Picker]
			cfData.Set(m_stVATFileSection.pPickerAlignSec_SelectedPkr, m_stVATFileSection.pKeyPicker[nPickerIdx], m_stVATPickerAlignInfo.nSelectedPicker[nHand][nPickerIdx]);

			for (int nPitchType = 0; nPitchType < VAT_PITCH_TYPE_MAX; nPitchType++) {
				//[2. Picker Offset] - Wide/Narrow
				cfData.Set(m_stVATFileSection.pPickerAlignSec_PickerOffset[nPitchType], m_stVATFileSection.pKeyPickerX[nPickerIdx], m_stVATPickerAlignInfo.pEachPickerOffset[nHand][nPitchType][nPickerIdx].first);
				cfData.Set(m_stVATFileSection.pPickerAlignSec_PickerOffset[nPitchType], m_stVATFileSection.pKeyPickerY[nPickerIdx], m_stVATPickerAlignInfo.pEachPickerOffset[nHand][nPitchType][nPickerIdx].second);

				//[3. Picker Position] - Wide/Narrow
				cfData.Set(m_stVATFileSection.pPickerAlignSec_PickerPos[nPitchType], m_stVATFileSection.pKeyPickerX[nPickerIdx], m_stVATPickerAlignInfo.pEachPickerPos[nHand][nPitchType][nPickerIdx].first);
				cfData.Set(m_stVATFileSection.pPickerAlignSec_PickerPos[nPitchType], m_stVATFileSection.pKeyPickerY[nPickerIdx], m_stVATPickerAlignInfo.pEachPickerPos[nHand][nPitchType][nPickerIdx].second);

				//[4. Picker Wide/Narrow Avg Offset]
				cfData.Set(m_stVATFileSection.pPickerAlignSec_WideNarrowAvgOffset[nPitchType], m_stVATFileSection.pKeyX, m_stVATPickerAlignInfo.dPkrWideNarrowAvgOffset[nHand][nPitchType].first);
				cfData.Set(m_stVATFileSection.pPickerAlignSec_WideNarrowAvgOffset[nPitchType], m_stVATFileSection.pKeyY, m_stVATPickerAlignInfo.dPkrWideNarrowAvgOffset[nHand][nPitchType].second);
			}	

			//[5. Picker간 Z Focus Position]
			cfData.Set(m_stVATFileSection.pPickerAlignSec_ZFocusPos, m_stVATFileSection.pKeyPicker[nPickerIdx], m_stVATPickerAlignInfo.dPickerZFocusPos[nHand][nPickerIdx]);
		}

	

		//Picker Sub Info 작업 완료 후, 추가 필요 함.

// 		for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XY; nAxis++)
// 		{
//			cfData.Set(m_stVATFileSection.pPickerAlignSecSub_PrePos, m_stVATFileSection.pKeyAxisXY[nAxis], m_stVAT_PickerAlignSubInfo.dPreSettingPos[nHand][nAxis]);
// 		}

  	}
}


void CVAT_CommonData::SaveZAutoTeachingInformation()
{
	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++)
	{
		CConfigData cfData((LPSTR)(LPCTSTR)GetFilePathHand(VAT_DATA_PATH_Z_AUTO_TEACHING, nHand));

		int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;

		//============Const
		//cfData.Set(m_stVATFileSection.pZAutoTeachingSec_SettingParam, m_stVATFileSection.pKeyPickOffset, m_stVATZTeachConstInfo.dZPickOffset[nHand]);
		//cfData.Set(m_stVATFileSection.pZAutoTeachingSec_SettingParam, m_stVATFileSection.pKeyPlaceOffset, m_stVATZTeachConstInfo.dZPlaceOffset[nHand]);
		//Z Pick/Place Offset
		for (int nType = 0; nType < VAT_Z_OFFSET_TYPE_MAX; nType++) {
			int nMAX_UNIT = (nHand == VAT_HAND_LOAD_TESTPP1 || nHand == VAT_HAND_LOAD_TESTPP2) ? VAT_Z_OFFSET_UNIT_MAX : VAT_Z_OFFSET_UNIT_MIN;
			for (int nUnit = 0; nUnit < nMAX_UNIT; nUnit++) {
				double dOffset = m_stVATZTeachInfo.stUnitZOffset.getUnitZOffset(nHand, nType, nUnit);
				cfData.Set(m_stVATFileSection.pZAutoTeachingSec_ZPickPlaceOffset[nType], m_stVATFileSection.pKeyZOffset[nHand][nUnit], dOffset);
			}
		}


		for (int nPart = 0; nPart < nMaxPart; nPart++) {
			if (m_stVATFileSection._IsNecessaryPart(nHand, nPart) != TRUE)
				continue;

			char *pSecZTeachGapPos = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? m_stVATFileSection.pZAutoTeachingSec_PickerGap_TrayPP[nPart] : m_stVATFileSection.pZAutoTeachingSec_PickerGap_TesstPP[nPart];
			char *pSecZTeachVacuumPos = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? m_stVATFileSection.pZAutoTeachingSec_PickerVacPos_TrayPP[nPart] : m_stVATFileSection.pZAutoTeachingSec_PickerVacPos_TestPP[nPart];
			for (int nPicker = 0; nPicker < VAT_PICKER_MAX; nPicker++) {
				//Picker Gap
				double dGap = m_stVATZTeachInfo.stPickerGap.getPickerGap(nHand, nPart, nPicker);
				cfData.Set(pSecZTeachGapPos, m_stVATFileSection.pKeyHandPickerNum[nPicker], dGap);

				//Picker Vac Pos
				double dVacPos = m_stVATZTeachInfo.stPickerVacPos.getPickerVacPos(nHand, nPart, nPicker);
				cfData.Set(pSecZTeachVacuumPos, m_stVATFileSection.pKeyHandPickerNum[nPicker], dVacPos);
			}

			//Z Teach Base Pos
			char *pSecZTeachBasePos = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? m_stVATFileSection.pVATSec_TrayPpBasePos[nPart] : m_stVATFileSection.pVATSec_TestPpBasePos[nPart];
			for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XYZ; nAxis++) {
				cfData.Set(pSecZTeachBasePos, m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATZTeachInfo.stZTeachBasePos.getPartBasePos(nHand, nPart, nAxis));
			}
		}
	}
}

void CVAT_CommonData::SaveVisionAutoTeachingInformation()
{
	//[1. Hand Base Position]
	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++)
	{
		CConfigData cfData((LPSTR)(LPCTSTR)GetFilePathHand(VAT_DATA_PATH_VISION_AUTO_TEACHING, nHand));

		int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
		
		for (int nPart = 0; nPart < nMaxPart; nPart++)
		{
			for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XYZ; nAxis++) {
				//=============Real
				char *pSecHandBasePosReal = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? m_stVATFileSection.pVATSec_TrayPpBasePos[nPart] : m_stVATFileSection.pVATSec_TestPpBasePos[nPart];	
				cfData.Set(pSecHandBasePosReal, m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATInfo.stHandBasePos.getBasePos(nHand, nPart, nAxis));
		
				//=============Buff
				char *pSecHandBasePosBuff = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? m_stVATFileSection.pVATSecBuff_TrayPpBasePos[nPart] : m_stVATFileSection.pVATSecBuff_TestPpBasePos[nPart];
				cfData.Set(pSecHandBasePosBuff, m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATInfoBuff.stHandBasePos.getBasePos(nHand, nPart, nAxis));
			}
		}
	}


	//[2. Part Base Position]
	for (int nVATPart = 0; nVATPart < VAT_PART_MAX; nVATPart++)
	{
		CConfigData cfData((LPSTR)(LPCTSTR)GetFilePathPart(nVATPart));

		int nMAX_PART_POS = (nVATPart == VAT_PART_FEEDER) ? VAT_MAX_FEEDER_LOC_COUNT : VAT_MAX_TBL_LOC_COUNT;
		int nMAX_AXIS = (nVATPart == VAT_PART_FEEDER) ? VAT_MAX_AXIS_Y : VAT_MAX_AXIS_YROTATE;

		
		for (int nPartPos = 0; nPartPos < nMAX_PART_POS; nPartPos++)
		{
			for (int nAxis = 0; nAxis < nMAX_AXIS; nAxis++) {
				//===============Real
				char *pSecPartBasePosReal = (nVATPart == VAT_PART_FEEDER) ? m_stVATFileSection.pVATSec_PartFeederBasePos[nPartPos] : m_stVATFileSection.pVATSec_PartTableBasePos[nPartPos];
				cfData.Set(pSecPartBasePosReal, m_stVATFileSection.pKeyAxisYRotate[nAxis], m_stVATInfo.stPartBasePos.getBasePos(nVATPart, nPartPos, nAxis));

				//===============Buff
				char *pSecPartBasePosBuff = (nVATPart == VAT_PART_FEEDER) ? m_stVATFileSection.pVATSecBuff_PartFeederBasePos[nPartPos] : m_stVATFileSection.pVATSecBuff_PartTableBasePos[nPartPos];
				cfData.Set(pSecPartBasePosBuff, m_stVATFileSection.pKeyAxisYRotate[nAxis], m_stVATInfoBuff.stPartBasePos.getBasePos(nVATPart, nPartPos, nAxis));
			}
		}
	}
}


void CVAT_CommonData::SaveVATOption(int nOptionType)
{
	CConfigData cfData((LPSTR)(LPCTSTR)GetFilePathCom(VAT_DATA_PATH_OPTION));
	
	switch (nOptionType)
	{
		case eVatOption_PkrAutoFocus:
		{
			//Picker Align Auto Focus
			cfData.Set(m_stVATFileSection.pOption_PkrAutoFocus, m_stVATFileSection.pkeyOption, m_stVATOption.nPickerAutoFocus);
		}break;
		default:
			break;
	}	
}

//Save Part Type을 XY, Z, All로 나누어야 겠다.
// void CVAT_CommonData::SavePartBasePosition()
// {
// 	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++)
// 	{
// 		CConfigData cfData((LPSTR)(LPCTSTR)GetFilePath(VAT_DATA_PATH_PART_BASE_POSITION, nHand));
// 
// 		int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
// 
// 		for (int nPart = 0; nPart < nMaxPart; nPart++)
// 		{
// 			for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XYZ; nAxis++) {
// 				//=============Real
// 				char *pSecPartBasePosReal = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? m_stVATFileSection.pPartBasePos_TrayPP[nPart] : m_stVATFileSection.pPartBasePos_TestPP[nPart];	
// 				cfData.Set(pSecPartBasePosReal, m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATPartBasePos.getPartBasePos(nHand, nPart, nAxis));
// 
// 				//=============Buff
// 				char *pSecPartBasePosBuff = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? m_stVATFileSection.pPartBasePosBuff_TrayPP[nPart] : m_stVATFileSection.pPartBasePosBuff_TestPP[nPart];
// 				cfData.Set(pSecPartBasePosBuff, m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATPartBasePosBuff.getPartBasePos(nHand, nPart, nAxis));
// 			}
// 		}
// 	}
// }


void CVAT_CommonData::SaveEachZAutoFocusInformation(_eVAT_MODE_EACH_DATA_TYPE _eType, int nHand, int nTargetAxis/* = -1*/)
{
	CConfigData cfData((LPSTR)(LPCTSTR)GetFilePathHand(VAT_DATA_PATH_Z_FOCUS, nHand));

	int nSTART_AXIS = VAT_X;
	int nMAX_AXIS = (_eType == eZFocus_PlateJigLowerOffset) ? VAT_MAX_AXIS_XY : VAT_MAX_AXIS_XYZ;

	//Target Axis있을 경우, 해당 축만 File 저장.
	if (nTargetAxis != -1) {
		ASSERT(VAT_X <= nTargetAxis && nTargetAxis <= VAT_Z);

		nSTART_AXIS = nTargetAxis;
		nMAX_AXIS = nTargetAxis + 1;
	}

	for (int nAxis = nSTART_AXIS; nAxis < nMAX_AXIS; nAxis++) {
		switch (_eType)
		{
		case eZFocus_PlateJigTargetPos:
		{
			//Real
			cfData.Set(m_stVATFileSection.pZFocusSec_PlateJigTargetPos, m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATZFocusInfo.dPlateJIG_TargetPos[nHand][nAxis]);

			//Buff
			cfData.Set(m_stVATFileSection.pZFocusSecBuff_PlateJigTargetPos, m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATZFocusInfoBuff.dPlateJIG_TargetPos[nHand][nAxis]);
		}break;
		case eZFocus_PlateJigVacPos:
		{
			//Real
			cfData.Set(m_stVATFileSection.pZFocusSec_PlateJigVacPos, m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATZFocusInfo.dPlateJIG_VacuumPos[nHand][nAxis]);

			//Buff
			cfData.Set(m_stVATFileSection.pZFocusSecBuff_PlateJigVacPos, m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATZFocusInfoBuff.dPlateJIG_VacuumPos[nHand][nAxis]);
		}break;
		case eZFocus_PlateJigLowerOffset:
		{
			//Real
			cfData.Set(m_stVATFileSection.pZFocusSec_PlateJigLowerOffset, m_stVATFileSection.pKeyAxisXY[nAxis], m_stVATZFocusInfo.dPlateJIG_LowerOffset[nHand][nAxis]);

			//Buff
			cfData.Set(m_stVATFileSection.pZFocusSecBuff_PlateJigLowerOffset, m_stVATFileSection.pKeyAxisXY[nAxis], m_stVATZFocusInfoBuff.dPlateJIG_LowerOffset[nHand][nAxis]);
		}break;
		default:
			break;
		}
	}
}


void CVAT_CommonData::SaveEachCamCenterCalInformation(_eVAT_MODE_EACH_DATA_TYPE _eType, int nHand, int nTargetAxis/* = -1*/)
{
	CConfigData cfData((LPSTR)(LPCTSTR)GetFilePathHand(VAT_DATA_PATH_CAM_CENTER_CAL, nHand));

	int nSTART_AXIS = VAT_X;
	int nMAX_AXIS = VAT_MAX_AXIS_XYZ;

	//Target Axis있을 경우, 해당 축만 File 저장.
	if (nTargetAxis != -1) {
		ASSERT(VAT_X <= nTargetAxis && nTargetAxis <= VAT_Z);

		nSTART_AXIS = nTargetAxis;
		nMAX_AXIS = nTargetAxis + 1;
	}

	for (int nAxis = nSTART_AXIS; nAxis < nMAX_AXIS; nAxis++) {
		switch (_eType)
		{
		case eCamCenCal_BotCamCenterPos:
		{
			cfData.Set(m_stVATFileSection.pCamCenCalSec_BotCamCenterPos, m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATCamCenterCalInfo.dBotCameraPosition[nHand][nAxis]);
			cfData.Set(m_stVATFileSection.pCamCenCalSecBuff_BotCamCenterPos, m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATCamCenterCalInfoBuff.dBotCameraPosition[nHand][nAxis]);
		}break;
		case eCamCenCal_TopCamToPkrDis:
		{
			cfData.Set(m_stVATFileSection.pCamCenCalSec_TopCamToPkrDis, m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATCamCenterCalInfo.dTopCamToPkrDistance[nHand][nAxis]);
			cfData.Set(m_stVATFileSection.pCamCenCalSecBuff_TopCamToPkrDis, m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATCamCenterCalInfoBuff.dTopCamToPkrDistance[nHand][nAxis]);
		}break;
		default:
			break;
		}
	}
}


void CVAT_CommonData::SaveEachPickerAlignInformation(_eVAT_MODE_EACH_DATA_TYPE _eType, int nHand)
{
	CConfigData cfData((LPSTR)(LPCTSTR)GetFilePathHand(VAT_DATA_PATH_PICKER_ALIGN, nHand));

	switch (_eType)
	{
		case ePickerAlign_SelectedPkr:
		{
			//[1. Selected Picker]
			for (int nPickerIdx = VAT_PICKER_01; nPickerIdx < VAT_PICKER_MAX; nPickerIdx++) {
				cfData.Set(m_stVATFileSection.pPickerAlignSec_SelectedPkr, m_stVATFileSection.pKeyPicker[nPickerIdx], m_stVATPickerAlignInfo.nSelectedPicker[nHand][nPickerIdx]);
			}
		}break;
		case ePickerAlign_EachPickerOffset:
		{
			//[2. Picker Offset] - Wide/Narrow
			for (int nPitchType = 0; nPitchType < VAT_PITCH_TYPE_MAX; nPitchType++) {
				for (int nPickerIdx = VAT_PICKER_01; nPickerIdx < VAT_PICKER_MAX; nPickerIdx++) {
					cfData.Set(m_stVATFileSection.pPickerAlignSec_PickerOffset[nPitchType], m_stVATFileSection.pKeyPickerX[nPickerIdx], m_stVATPickerAlignInfo.pEachPickerOffset[nHand][nPitchType][nPickerIdx].first);
					cfData.Set(m_stVATFileSection.pPickerAlignSec_PickerOffset[nPitchType], m_stVATFileSection.pKeyPickerY[nPickerIdx], m_stVATPickerAlignInfo.pEachPickerOffset[nHand][nPitchType][nPickerIdx].second);
				}
			}
		}break;
		case ePickerAlign_EachPickerPos:
		{
			//[3. Picker Position] - Wide/Narrow
			for (int nPitchType = 0; nPitchType < VAT_PITCH_TYPE_MAX; nPitchType++) {
				for (int nPickerIdx = VAT_PICKER_01; nPickerIdx < VAT_PICKER_MAX; nPickerIdx++) {
					cfData.Set(m_stVATFileSection.pPickerAlignSec_PickerPos[nPitchType], m_stVATFileSection.pKeyPickerX[nPickerIdx], m_stVATPickerAlignInfo.pEachPickerPos[nHand][nPitchType][nPickerIdx].first);
					cfData.Set(m_stVATFileSection.pPickerAlignSec_PickerPos[nPitchType], m_stVATFileSection.pKeyPickerY[nPickerIdx], m_stVATPickerAlignInfo.pEachPickerPos[nHand][nPitchType][nPickerIdx].second);
				}
			}
		}break;
		case ePickerAlign_AvgOffset:
		{
			//[4. Picker Wide/Narrow Avg Offset]
			for (int nPitchType = 0; nPitchType < VAT_PITCH_TYPE_MAX; nPitchType++) {
				for (int nPickerIdx = VAT_PICKER_01; nPickerIdx < VAT_PICKER_MAX; nPickerIdx++) {
					cfData.Set(m_stVATFileSection.pPickerAlignSec_WideNarrowAvgOffset[nPitchType], m_stVATFileSection.pKeyX, m_stVATPickerAlignInfo.dPkrWideNarrowAvgOffset[nHand][nPitchType].first);
					cfData.Set(m_stVATFileSection.pPickerAlignSec_WideNarrowAvgOffset[nPitchType], m_stVATFileSection.pKeyY, m_stVATPickerAlignInfo.dPkrWideNarrowAvgOffset[nHand][nPitchType].second);
				}
			}
		}break;
		case ePickerAlign_ZFocusPos:
		{
			//[5. Picker간 Z Focus Position]
			for (int nPickerIdx = VAT_PICKER_01; nPickerIdx < VAT_PICKER_MAX; nPickerIdx++) {
				cfData.Set(m_stVATFileSection.pPickerAlignSec_ZFocusPos, m_stVATFileSection.pKeyPicker[nPickerIdx], m_stVATPickerAlignInfo.dPickerZFocusPos[nHand][nPickerIdx]);
			}
		}break;
		case ePickerAlign_PreSettingPos:
		{
			//[6. Picker Pre Setting Position][Sub]
			for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XY; nAxis++)
			{
				cfData.Set(m_stVATFileSection.pPickerAlignSecSub_PrePos, m_stVATFileSection.pKeyAxisXY[nAxis], m_stVAT_PickerAlignSubInfo.dPreSettingPos[nHand][nAxis]);
			}
		}break;
		default:
			break;
	}



	//========================================================================================
	//추후, sub info 개발 완료 후 작업 필요.
// 	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++)
// 	{
// 		CConfigData cfData((LPSTR)(LPCTSTR)GetFilePath(VAT_DATA_PATH_PICKER_ALIGN, nHand));
// 
// 		//Picker Align Sub Data 저장.
// 		//Picker Setting Pre Position
// 		for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XY; nAxis++) {
// 			cfData.Set(m_stVATFileSection.pPickerAlignSecSub_PrePos, m_stVATFileSection.pKeyAxisXY[nAxis], m_stVAT_PickerAlignSubInfo.dPreSettingPos[nHand][nAxis]);
// 		}
// 		
// 	}

	//========================================================================================
}


void CVAT_CommonData::SaveEachZAutoTeachingInformation(_eVAT_MODE_EACH_DATA_TYPE _eType, int nHand)
{
	CConfigData cfData((LPSTR)(LPCTSTR)GetFilePathHand(VAT_DATA_PATH_Z_AUTO_TEACHING, nHand));

	switch (_eType)
	{
		case eZAutoTeaching_ZTeachBasePos:
		{
			int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
			for (int nPart = 0; nPart < nMaxPart; nPart++) {
				if(m_stVATFileSection._IsNecessaryPart(nHand, nPart) != TRUE)
					continue;

				//Z Teach Base Pos
				for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XYZ; nAxis++) {
					char *pSecZTeachBasePos = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? m_stVATFileSection.pVATSec_TrayPpBasePos[nPart] : m_stVATFileSection.pVATSec_TestPpBasePos[nPart];
					cfData.Set(pSecZTeachBasePos, m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATZTeachInfo.stZTeachBasePos.getPartBasePos(nHand, nPart, nAxis));
				}
			}			
		}break;
		case eZAutoTeaching_PickPlaceOffset:
		{
			for (int nType = 0; nType < VAT_Z_OFFSET_TYPE_MAX; nType++) {
				int nMAX_UNIT = (nHand == VAT_HAND_LOAD_TESTPP1 || nHand == VAT_HAND_LOAD_TESTPP2) ? VAT_Z_OFFSET_UNIT_MAX : VAT_Z_OFFSET_UNIT_MIN;
				for (int nUnit = 0; nUnit < nMAX_UNIT; nUnit++) {
					double dOffset = m_stVATZTeachInfo.stUnitZOffset.getUnitZOffset(nHand, nType, nUnit);
					cfData.Set(m_stVATFileSection.pZAutoTeachingSec_ZPickPlaceOffset[nType], m_stVATFileSection.pKeyZOffset[nHand][nUnit], dOffset);
				}
			}


			//============Const
			//cfData.Set(m_stVATFileSection.pZAutoTeachingSec_SettingParam, m_stVATFileSection.pKeyPickOffset, m_stVATZTeachConstInfo.dZPickOffset[nHand]);
			//cfData.Set(m_stVATFileSection.pZAutoTeachingSec_SettingParam, m_stVATFileSection.pKeyPlaceOffset, m_stVATZTeachConstInfo.dZPlaceOffset[nHand]);
		}break;
		case eZAutoTeaching_PickerFlatnessOffset:
		{
			int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
			for (int nPart = 0; nPart < nMaxPart; nPart++) {
				if (m_stVATFileSection._IsNecessaryPart(nHand, nPart) != TRUE)
					continue;

				char *pSecZTeachGapPos = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? m_stVATFileSection.pZAutoTeachingSec_PickerGap_TrayPP[nPart] : m_stVATFileSection.pZAutoTeachingSec_PickerGap_TesstPP[nPart];
				for (int nPicker = 0; nPicker < VAT_PICKER_MAX; nPicker++) {
					double dGap = m_stVATZTeachInfo.stPickerGap.getPickerGap(nHand, nPart, nPicker);
					cfData.Set(pSecZTeachGapPos, m_stVATFileSection.pKeyHandPickerNum[nPicker], dGap);
				}
			}
		}break;
		case eZAutoTeaching_PickerVacPos:
		{
			int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
			for (int nPart = 0; nPart < nMaxPart; nPart++) {
				if (m_stVATFileSection._IsNecessaryPart(nHand, nPart) != TRUE)
					continue;

				char *pSecZTeachVacuumPos = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? m_stVATFileSection.pZAutoTeachingSec_PickerVacPos_TrayPP[nPart] : m_stVATFileSection.pZAutoTeachingSec_PickerVacPos_TestPP[nPart];
				for (int nPicker = 0; nPicker < VAT_PICKER_MAX; nPicker++) {
					double dVacPos = m_stVATZTeachInfo.stPickerVacPos.getPickerVacPos(nHand, nPart, nPicker);
					cfData.Set(pSecZTeachVacuumPos, m_stVATFileSection.pKeyHandPickerNum[nPicker], dVacPos);
				}
			}
		}break;
		default:
			break;
	}


}


//============================================================================================
//COMMENT : 
//Param2  : nUnit의 경우 _eType에 따라 Hand가 될 수도 있고 VATPart가 될 수도 있음.
//			nUnit 혼동을 방지 하고자, 초기 변수이름 변경 하여 사용.
//Param3  : nSection의 경우 _eType에 따라 PartIdx가 될 수도 있고 PartPos이 될 수도 있음.
//			nSection 혼동을 방지 하고자, 초기 변수이름 변경 하여 사용.
//=============================================================================================
void CVAT_CommonData::SaveEachVisionAutoTeachingInformation(_eVAT_MODE_EACH_DATA_TYPE _eType, int nUnit, int nSection, int nTargetAxis /*= -1*/)
{
	switch (_eType)
	{
	case eVisionAutoTeaching_HandBasePos:
	{
		int nHand = nUnit;		//변수 이름 변경.
		int nPart = nSection;	//변수 이름 변경.

		CConfigData cfData((LPSTR)(LPCTSTR)GetFilePathHand(VAT_DATA_PATH_VISION_AUTO_TEACHING, nHand));

		int nSTART_AXIS = VAT_X;
		int nMAX_AXIS = VAT_MAX_AXIS_XYZ;

		//Target Axis있을 경우, 해당 축만 File 저장.
		if (nTargetAxis != -1) {
			ASSERT(VAT_X <= nTargetAxis && nTargetAxis <= VAT_Z);

			nSTART_AXIS = nTargetAxis;
			nMAX_AXIS = nTargetAxis + 1;
		}

		for (int nAxis = nSTART_AXIS; nAxis < nMAX_AXIS; nAxis++) {
			//=============Real
			char *pSecHandBasePosReal = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? m_stVATFileSection.pVATSec_TrayPpBasePos[nPart] : m_stVATFileSection.pVATSec_TestPpBasePos[nPart];
			cfData.Set(pSecHandBasePosReal, m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATInfo.stHandBasePos.getBasePos(nHand, nPart, nAxis));

			//=============Buff
			char *pSecHandBasePosBuff = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? m_stVATFileSection.pVATSecBuff_TrayPpBasePos[nPart] : m_stVATFileSection.pVATSecBuff_TestPpBasePos[nPart];
			cfData.Set(pSecHandBasePosBuff, m_stVATFileSection.pKeyAxisXYZ[nAxis], m_stVATInfoBuff.stHandBasePos.getBasePos(nHand, nPart, nAxis));
		}		
	}break;
	case eVisionAutoTeaching_PartBasePos:
	{
		int nVATPart = nUnit;		//변수 이름 변경.
		int nPartPos = nSection;	//변수 이름 변경.
		CConfigData cfData((LPSTR)(LPCTSTR)GetFilePathPart(nVATPart));

		int nSTART_AXIS = VAT_PART_Y;
		int nMAX_AXIS = (nVATPart == VAT_PART_FEEDER) ? VAT_MAX_AXIS_Y : VAT_MAX_AXIS_YROTATE;

		//Target Axis 있을 경우, 해당 축만 File 저장.
		if (nTargetAxis != -1) {
			ASSERT(VAT_PART_Y <= nTargetAxis && nTargetAxis <= VAT_PART_ROTATE);

			nSTART_AXIS = nTargetAxis;
			nMAX_AXIS = nTargetAxis + 1;
		}

		for (int nAxis = nSTART_AXIS; nAxis < nMAX_AXIS; nAxis++) {
			//===============Real
			char *pSecPartBasePosReal = (nVATPart == VAT_PART_FEEDER) ? m_stVATFileSection.pVATSec_PartFeederBasePos[nPartPos] : m_stVATFileSection.pVATSec_PartTableBasePos[nPartPos];
			cfData.Set(pSecPartBasePosReal, m_stVATFileSection.pKeyAxisYRotate[nAxis], m_stVATInfo.stPartBasePos.getBasePos(nVATPart, nPartPos, nAxis));

			//===============Buff
			char *pSecPartBasePosBuff = (nVATPart == VAT_PART_FEEDER) ? m_stVATFileSection.pVATSecBuff_PartFeederBasePos[nPartPos] : m_stVATFileSection.pVATSecBuff_PartTableBasePos[nPartPos];
			cfData.Set(pSecPartBasePosBuff, m_stVATFileSection.pKeyAxisYRotate[nAxis], m_stVATInfoBuff.stPartBasePos.getBasePos(nVATPart, nPartPos, nAxis));
		}
	}break;
	default:
		break;
	}	
}


// void CVAT_CommonData::SaveEachHandPartBasePosition(int nHand)
// {
// 	CConfigData cfData((LPSTR)(LPCTSTR)GetFilePath(VAT_DATA_PATH_PART_BASE_POSITION, nHand));
// 
// 	int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
// 
// 	for (int nPart = 0; nPart < nMaxPart; nPart++)
// 	{
// 		for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XYZ; nAxis++) {
// 			char *pSecPartBasePos = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? m_stVATFileSection.pPartBasePos_TrayPP[nPart] : m_stVATFileSection.pPartBasePos_TestPP[nPart];
// 
// 			cfData.Set(pSecPartBasePos, m_stVATFileSection.pKeyAxisXYZ[nAxis], g_VATCommonData.m_stVATPartBasePos.getPartBasePos(nHand, nPart, nAxis));
// 		}
// 	}
// }

void CVAT_CommonData::SaveEachDesignValue(_eVAT_MODE_EACH_DATA_TYPE _eType)
{
	CConfigData cfData((LPSTR)(LPCTSTR)GetFilePathCom(VAT_DATA_PATH_DESIGN_VALUE));

	switch (_eType)
	{
	case eDesignValue_ZAutoFocus:
	{
	}break;
	case eDesignValue_CamCenterCalibration:
	{

	}break;
	case eDesignValue_PickerAlign:
	{

	}break;
	case eDesignValue_ZAutoTeaching:
	{
		for (int nIdx = 0; nIdx < VAT_MAX_DESIGN_VALUE_Z_AUTO_TEACHING; nIdx++) {
			cfData.Set(m_stVATFileSection.pDesignValue_ZAutoTeaching[nIdx], m_stVATFileSection.pKeyValue, m_stVATDesignValueConstInfo.dZAutoTeaching[nIdx]);
		}
	}break;
	case eDesignValue_VisionAutoTeaching:
	{

	}break;
	default:
		break;
	}
}


//=========================================================<Data 관리 운용>

void CVAT_CommonData::DataCopyBufferToSource()
{
	//g_VATCommonData.m_stVATZTeachInfo = g_VATCommonData.m_stVATZTeachInfoBuff;
}

void CVAT_CommonData::DataCopySourceToBuffer()
{
	//g_VATCommonData.m_stVATZTeachInfoBuff = g_VATCommonData.m_stVATZTeachInfo;
}


/*                  
                     [측정 시작 시]
		  ∧  ------------------------------------>  ∨
		  ∧									        ∨
		  ∧	  [측정 종료 시]							∨
		Real ----------------> Buffer            Sequence
		  ∧       					                ∨
		  ∧									        ∨	
		  ∧  <-----------------------------------   ∨
						[측정 종료 시]	

		Real Data	: GUI에서 수정 된 Data/측정 완료된 Data.(가장 최신 DATA)
		Buffer Data : 이전 측정 Data.
		Seq Data    : 측정 시에만 사용 되는 Data.


		Real -> Sequence : 측정 시작 시, Real Data를 Sequence Data로 복사.
		Real -> Buffer   : 측정 종료 시, Real Data를 Buffer Data로 복사.
		Seq  -> Real     : 측정 종료 시, Seq Data를 Real로 복사.

*/



//============================================================================================
//COMMENT : Sequence 시작 시, Real Data를 Sequence Data로 copy
//          측정 시에는 Sequence Data만 사용하기 위함.
//          -. 측정 중, 취소 또는 Data 변경 되어도 Buffer/Real Data 변경 점 없음.
//			-. 측정 후, 해당 Data 사용 안함.
//          -. Z Auto Teaching Data는 Const이거나 이전 Data와 차이를 확인 하지 않아 Buff가 없음.
//=============================================================================================
void CVAT_CommonData::DataCopyRealToSequence(int nMode)
{
	switch (nMode)
	{
		case VAT_SEQ_MODE_Z_FOCUS:			m_stVATZFocusInfoSeq = m_stVATZFocusInfo;							break;
		case VAT_SEQ_MODE_CAM_CAL:			m_stVATCamCenterCalInfoSeq = m_stVATCamCenterCalInfo;				break;
		case VAT_SEQ_MODE_PICKER_ALIGN:		m_stVATPickerAlignInfoSeq = m_stVATPickerAlignInfo;					break;
		case VAT_SEQ_MODE_VAT:				m_stVATInfoSeq = m_stVATInfo;	break;
		case VAT_SEQ_MODE_Z_TEACHING:		m_stVATZTeachInfoSeq = m_stVATZTeachInfo;							break;
		default:
			break;
	}
}

//==========================================================================
//COMMENT : Sequence 측정 후, Data Save전 기존 Real Data를 Buffer에 copy
//			이전 측정 Data와 차이를 확인 하기 위함.
//			Real - Buffer로 이전 Data 차이 확인 가능.
//          Z Auto Teaching Data는 Const이거나 이전 Data와 차이를 확인 하지 않아 Buff가 없음.
//==========================================================================
void CVAT_CommonData::DataCopyRealToBuffer(int nMode)
{
	//'Z Auto Teaching' Data Buff 없음.
	if (nMode == VAT_SEQ_MODE_Z_TEACHING || nMode == VAT_SEQ_MODE_PICKER_ALIGN) {
		return;
	}

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		if (!IsSelectUnitInState(nHand, VAT_STATUS_DATA_SAVE_COMPLETE)) {
			continue;
		}

		switch (nMode)
		{
			case VAT_SEQ_MODE_Z_FOCUS:
			{
				COPY_VAT_ARRAY(m_stVATZFocusInfoBuff.dPlateJIG_TargetPos[nHand], m_stVATZFocusInfo.dPlateJIG_TargetPos[nHand]);
				COPY_VAT_ARRAY(m_stVATZFocusInfoBuff.dPlateJIG_VacuumPos[nHand], m_stVATZFocusInfo.dPlateJIG_VacuumPos[nHand]);
				COPY_VAT_ARRAY(m_stVATZFocusInfoBuff.dPlateJIG_LowerOffset[nHand], m_stVATZFocusInfo.dPlateJIG_LowerOffset[nHand]);
			}break;
			case VAT_SEQ_MODE_CAM_CAL:
			{
				COPY_VAT_ARRAY(m_stVATCamCenterCalInfoBuff.dBotCameraPosition[nHand], m_stVATCamCenterCalInfo.dBotCameraPosition[nHand]);
				COPY_VAT_ARRAY(m_stVATCamCenterCalInfoBuff.dTopCamToPkrDistance[nHand], m_stVATCamCenterCalInfo.dTopCamToPkrDistance[nHand]);
			}break;
			case VAT_SEQ_MODE_VAT:
			{

			}break;
			default:
				break;
		}
	}

	// 이전 방식은 전체 복사 [9/15/2025 dohyeong.kim]
// 	switch (nMode)
// 	{
// 	case VAT_SEQ_MODE_Z_FOCUS:			m_stVATZFocusInfoBuff = m_stVATZFocusInfo;							break;
// 	case VAT_SEQ_MODE_CAM_CAL:			m_stVATCamCenterCalInfoBuff = m_stVATCamCenterCalInfo;				break;
// 	case VAT_SEQ_MODE_PICKER_ALIGN:		m_stVATPickerAlignInfoBuff = m_stVATPickerAlignInfo;				break;
// 	case VAT_SEQ_MODE_VAT:				m_stVATVisionAutoTeachingInfoBuff = m_stVATVisionAutoTeachingInfo;	break;
// 	case VAT_SEQ_MODE_Z_TEACHING:		/*Data Buff 없음.*/													break;
// 	default:
// 		break;
// 	}
}


//==========================================================================
//COMMENT : Sequence 측정 후, Data Save시 Sequence -> Real로 변경.
//			이전 측정 Data는 Buffer로 복사된 상태.
//			Sequence에서 Offset으로 정밀 Teaching된 최종값이 Real로 copy됨.	
//==========================================================================
void CVAT_CommonData::DataCopySequenceToReal(int nMode)
{
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		if (!IsSelectUnitInState(nHand, VAT_STATUS_DATA_SAVE_COMPLETE)) {
			continue;
		}

		switch (nMode)
		{
		case VAT_SEQ_MODE_Z_FOCUS:
		{
			COPY_VAT_ARRAY(m_stVATZFocusInfo.dPlateJIG_TargetPos[nHand], m_stVATZFocusInfoSeq.dPlateJIG_TargetPos[nHand]);
			COPY_VAT_ARRAY(m_stVATZFocusInfo.dPlateJIG_VacuumPos[nHand], m_stVATZFocusInfoSeq.dPlateJIG_VacuumPos[nHand]);
			COPY_VAT_ARRAY(m_stVATZFocusInfo.dPlateJIG_LowerOffset[nHand], m_stVATZFocusInfoSeq.dPlateJIG_LowerOffset[nHand]);
		}break;
		case VAT_SEQ_MODE_CAM_CAL:
		{
			COPY_VAT_ARRAY(m_stVATCamCenterCalInfo.dBotCameraPosition[nHand], m_stVATCamCenterCalInfoSeq.dBotCameraPosition[nHand]);
			COPY_VAT_ARRAY(m_stVATCamCenterCalInfo.dTopCamToPkrDistance[nHand], m_stVATCamCenterCalInfoSeq.dTopCamToPkrDistance[nHand]);
		}break;
		case VAT_SEQ_MODE_PICKER_ALIGN:
		{
			m_stVATPickerAlignInfo = m_stVATPickerAlignInfoSeq;
		}break;
		case VAT_SEQ_MODE_VAT:
		{

		}break;
		default:
			break;
		}
	}


// 이전 방식 전체 복사 [9/15/2025 dohyeong.kim]
// 	switch (nMode)
// 	{
// 	case VAT_SEQ_MODE_Z_FOCUS:			m_stVATZFocusInfo = m_stVATZFocusInfoSeq;							break;
// 	case VAT_SEQ_MODE_CAM_CAL:			m_stVATCamCenterCalInfo = m_stVATCamCenterCalInfoSeq;				break;
// 	case VAT_SEQ_MODE_PICKER_ALIGN:		m_stVATPickerAlignInfo = m_stVATPickerAlignInfoSeq;					break;
// 	case VAT_SEQ_MODE_VAT:				m_stVATVisionAutoTeachingInfo = m_stVATVisionAutoTeachingInfoSeq;	break;
// 	case VAT_SEQ_MODE_Z_TEACHING:		m_stVATZTeachInfo = m_stVATZTeachInfoSeq;							break;
// 	default:
// 		break;
// 	}
}
