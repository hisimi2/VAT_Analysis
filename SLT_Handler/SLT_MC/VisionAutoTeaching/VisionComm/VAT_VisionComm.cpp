#include "stdafx.h"
#include "VAT_VisionComm.h"


const int c_nAxisNo[VAT_HAND_MAX][VAT_MAX_AXIS_XY] = {
	{ eAXIS_TRAY_PP_1_X, eAXIS_TRAY_FEEDER_Y },				//LD Tray PP
	{ eAXIS_TRAY_PP_2_X, eAXIS_TRAY_PP_2_Y },				//UD Tray PP
	{ eAXIS_TEST_PP_1_X, eAXIS_TEST_PP_1_Y },				//LD Test PP#1
	{ eAXIS_TEST_PP_1_X2, eAXIS_TEST_PP_1_Y },				//UD Test PP#1
	{ eAXIS_TEST_PP_2_X, eAXIS_TEST_PP_2_Y },				//LD Test PP#2
	{ eAXIS_TEST_PP_2_X2, eAXIS_TEST_PP_2_Y },				//UD Test PP#2
};

CVAT_VisionComm::CVAT_VisionComm()
{
}


CVAT_VisionComm::~CVAT_VisionComm()
{
}


void CVAT_VisionComm::SaveMeasureData(int nHand, int nDataId, int nStatus)
{
	//Manual일경우, Data 저장하지 않기 위해 nHand = -1
	if (!(VAT_HAND_LOAD_TRAYPP <= nHand && nHand <= VAT_HAND_UNLOAD_TESTPP2)){
		return;
	}

	int nCamNo = ConvertDataIdToVatCam(nDataId);

	//Receive Data
	ST_VAT_RCV_DATA stRcvData;
	g_VatVisionMgr[nCamNo].GetReceiveData(stRcvData);

	switch (nStatus)
	{
		case VISION_STATUS_PLATE_JIG:	SaveMeasureData_PlateJig(nHand, nDataId, stRcvData);	break;
		case VISION_STATUS_PICKER:		SaveMeasureData_Picker(nHand, stRcvData);				break;
		case VISION_STATUS_C_TRAY:		SaveMeasureData_C_Tray();  break;
		case VISION_STATUS_TABLE:		SaveMeasureData_Table(nHand, stRcvData);  break;
		case VISION_STATUS_SOCKET:		SaveMeasureData_Sockets();  break;
		case VISION_STATUS_FIND_Z_MEASURE_PICKER: SaveMeasureData_PickerZPos(nHand); break;
		case VISION_STATUS_FIND_Z_RESULT_PICKER:  SaveMeasureData_FocusedPickerZIdx(nHand, stRcvData); break;
		default: {
			//Data Save 필요 없는 Status
			//VISION_STATUS_AUTO_FOCUS
			//VISION_STATUS_LED_ON
			//VISION_STATUS_LED_OFF
		}break;
	}

}


void CVAT_VisionComm::SaveMeasureData_PlateJig(int nHand, int nDataId, ST_VAT_RCV_DATA stRcvData)
{
	int nCamType = ChkCamType(nDataId);

	//[Lower]
	if (nCamType == _eCamType_Lower)
	{
		std::vector<int> vHand;
		vHand.clear();

		//Test PP의 경우 LD/UD Hand가 1개의 하단 Target 공유.
		if (nDataId == VISION_REQ_DATAID_VAT_TEST_1_LOWER) {
			vHand.push_back(VAT_HAND_LOAD_TESTPP1);
			vHand.push_back(VAT_HAND_UNLOAD_TESTPP1);
		}
		else if (nDataId == VISION_REQ_DATAID_VAT_TEST_2_LOWER) {
			vHand.push_back(VAT_HAND_LOAD_TESTPP2);
			vHand.push_back(VAT_HAND_UNLOAD_TESTPP2);
		}
		else {
			vHand.push_back(nHand);
		}

		for (auto _Hand : vHand) {
			g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_LowerOffset[_Hand][VAT_X] = stRcvData.dX;
			//g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_LowerOffset[_Hand][VAT_Y] = stRcvData.dY;

			if (_Hand == VAT_HAND_LOAD_TRAYPP) {
				g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_LowerOffset[_Hand][VAT_Y] = (-1)*stRcvData.dY;
			}
			else {
				g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_LowerOffset[_Hand][VAT_Y] = stRcvData.dY;
 			}		
		}
	}
	//[Upper]
	else
	{
		//In Position 안되면, Target 위치 조정.
		if (IsOffsetOKRange(nHand, stRcvData.dX, stRcvData.dY) != TRUE) {
			g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_TargetPos[nHand][VAT_X] -= stRcvData.dX;  //Vision Offset 부호 그대로 사용.
			//g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_TargetPos[nHand][VAT_Y] -= stRcvData.dY;

			if (nHand == VAT_HAND_LOAD_TRAYPP) {
				g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_TargetPos[nHand][VAT_Y] += stRcvData.dY;
			}
			else {
				g_VATCommonData.m_stVATZFocusInfoSeq.dPlateJIG_TargetPos[nHand][VAT_Y] -= stRcvData.dY;  //Vision Offset 부호 그대로 사용.
 			}
		}
	}
}


void CVAT_VisionComm::SaveMeasureData_Picker(int nHand, ST_VAT_RCV_DATA stRcvData)
{
	int nMode = g_VatTaskSystemCtrl.GetSeqCurMode();

	//[Lower]
	if (IsOffsetOKRange(nHand, stRcvData.dX, stRcvData.dY) != TRUE) {
		switch (nMode)
		{
			case VAT_SEQ_MODE_CAM_CAL:
			{
				g_VATCommonData.m_stVATCamCenterCalInfoSeq.dBotCameraPosition[nHand][VAT_X] -= stRcvData.dX; //Vision Offset 부호 그대로 사용.
				//g_VATCommonData.m_stVATCamCenterCalInfoSeq.dBotCameraPosition[nHand][VAT_Y] -= stRcvData.dY; 

				if (nHand == VAT_HAND_LOAD_TRAYPP) {
					g_VATCommonData.m_stVATCamCenterCalInfoSeq.dBotCameraPosition[nHand][VAT_Y] += stRcvData.dY; //Vision Offset 부호 그대로 사용.
				}
				else {
					g_VATCommonData.m_stVATCamCenterCalInfoSeq.dBotCameraPosition[nHand][VAT_Y] -= stRcvData.dY; //Vision Offset 부호 그대로 사용.
 				}
				
			}break;
			case VAT_SEQ_MODE_PICKER_ALIGN:
			{
				int nPitchType = g_VatTaskSystemCtrl.m_vAllTaskAuto[nHand]->m_stCurPicker.nPitchType;
				int nPickerIdx = g_VatTaskSystemCtrl.m_vAllTaskAuto[nHand]->m_stCurPicker.nPickerIdx;

				g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerPos[nHand][nPitchType][nPickerIdx].first  -= stRcvData.dX; //Vision Offset 부호 그대로 사용.
				

				if (nHand == VAT_HAND_LOAD_TRAYPP) {
					g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerPos[nHand][nPitchType][nPickerIdx].second += stRcvData.dY; //Vision Offset 부호 그대로 사용.
				}
				else {
					g_VATCommonData.m_stVATPickerAlignInfoSeq.pEachPickerPos[nHand][nPitchType][nPickerIdx].second -= stRcvData.dY; //Vision Offset 부호 그대로 사용.
				}
			}break;
			default:
				break;
		}
	}
}


void CVAT_VisionComm::SaveMeasureData_C_Tray()
{

}


void CVAT_VisionComm::SaveMeasureData_Table(int nHand, ST_VAT_RCV_DATA stRcvData)
{
	if (!(VAT_HAND_LOAD_TRAYPP <= nHand && nHand <= VAT_HAND_UNLOAD_TESTPP2))
		return;

	int nCurLoc = g_VatTaskSystemCtrl.m_vAllTaskAuto[nHand]->m_nCurLoc;

	//X Position 보정.
	double dPosX = g_VATCommonData.m_stVATInfoSeq.stHandBasePos.getBasePos(nHand, nCurLoc, VAT_X) - stRcvData.dX;
	g_VATCommonData.m_stVATInfoSeq.stHandBasePos.setBasePos(nHand, nCurLoc, VAT_X, dPosX);

	//Y Position 보정.
	if (nHand == VAT_HAND_LOAD_TRAYPP)
	{
		int nVATPart = (nCurLoc == VAT_IDX_TRAY_PP_FEEDER) ? VAT_PART_FEEDER : (nCurLoc == VAT_IDX_TRAY_PP_TBL_2) ? VAT_PART_TABLE_2 : VAT_PART_TABLE_3;
		double dPosY = g_VATCommonData.m_stVATInfoSeq.stPartBasePos.getBasePos(nVATPart, VAT_TBL_LOAD_SIDE, VAT_PART_Y) - stRcvData.dY;
		g_VATCommonData.m_stVATInfoSeq.stPartBasePos.setBasePos(nVATPart, VAT_TBL_LOAD_SIDE, VAT_PART_Y, dPosX);
	}
	else 
	{
// 		int nVATPart = -1;
// 		int nPartBasePos = -1;
// 
// 		switch (nHand)
// 		{
// 			case VAT_HAND_UNLOAD_TRAYPP:	
// 			{
// 				nVATPart = (nCurLoc == VAT_IDX_TRAY_PP_TBL_1) ? VAT_PART_TABLE_1 : VAT_PART_TABLE_4;
// 				nPartBasePos = VAT_TBL_LOAD_SIDE;
// 			}break;
// 			case VAT_HAND_LOAD_TESTPP1:		
// 			{
// 				nVATPart = VAT_PART_TABLE_2;
// 				nPartBasePos = VAT_TBL_TEST_SIDE_1 + (VAT_IDX_TEST_PP_TBL_1_LD - nCurLoc);
// 			}break;
// 			case VAT_HAND_UNLOAD_TESTPP1:	
// 			{
// 				nVATPart = VAT_PART_TABLE_1;
// 			}break;
// 			case VAT_HAND_LOAD_TESTPP2:		
// 			{
// 				nVATPart = VAT_PART_TABLE_3;
// 			}break;
// 			case VAT_HAND_UNLOAD_TESTPP2:	
// 			{
// 				nVATPart = VAT_PART_TABLE_4;
// 			}break;
// 			default:	
// 				ASSERT(0);  
// 				break;
// 		}

		double dPosY = g_VATCommonData.m_stVATInfoSeq.stHandBasePos.getBasePos(nHand, nCurLoc, VAT_Y) - stRcvData.dY;
		g_VATCommonData.m_stVATInfoSeq.stHandBasePos.setBasePos(nHand, nCurLoc, VAT_Y, dPosY);
	}
}


void CVAT_VisionComm::SaveMeasureData_Sockets()
{

}


void CVAT_VisionComm::GetBbxInfo(int nHand, CBbxmnpClient::_ePICK_MOTOR &eMotor, int &nBbxStartIdx)
{
	switch (nHand)
	{
	case VAT_HAND_LOAD_TRAYPP:
	{
		eMotor = CBbxmnpClient::PICK_MOTOR_LOAD_TRAY;
		nBbxStartIdx = eBMAxis_LOAD_TRAY_Picker01;
	}break;
	case VAT_HAND_UNLOAD_TRAYPP:
	{
		eMotor = CBbxmnpClient::PICK_MOTOR_UNLOAD_TRAY;
		nBbxStartIdx = eBMAxis_UNLOAD_TRAY_Picker01;
	}break;
	case VAT_HAND_LOAD_TESTPP1:
	{
		eMotor = CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1;
		nBbxStartIdx = eBMAxis_LOAD_TESTPP_1_Picker01;
	}break;
	case VAT_HAND_UNLOAD_TESTPP1:
	{
		eMotor = CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1;
		nBbxStartIdx = eBMAxis_UNLOAD_TESTPP_1_Picker01;
	}break;
	case VAT_HAND_LOAD_TESTPP2:
	{
		eMotor = CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2;
		nBbxStartIdx = eBMAxis_LOAD_TESTPP_2_Picker01;
	}break;
	case VAT_HAND_UNLOAD_TESTPP2:
	{
		eMotor = CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2;
		nBbxStartIdx = eBMAxis_UNLOAD_TESTPP_2_Picker01;
	}break;
	default:
		break;
	}
}


void CVAT_VisionComm::SaveMeasureData_PickerZPos(int nHand)
{
	CBbxmnpClient::_ePICK_MOTOR eMotor = (CBbxmnpClient::_ePICK_MOTOR)(-1);
	int nBbxStartIdx = -1;

	GetBbxInfo(nHand, eMotor, nBbxStartIdx);

	int nPkrX = VAT_PICKER_X(m_nMeasurePkrIdx);
	int nPkrY = VAT_PICKER_Y(m_nMeasurePkrIdx);

	if ((g_DMCont.m_dmEQP.GN(NDM0_BoardDirection) == eBoardDir_Backward) &&
		!(nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP)) {
		nPkrX = (MAX_TEST_PP_PKR_CNT_X - 1) - nPkrX;
		nPkrY = (MAX_TEST_PP_PKR_CNT_Y - 1) - nPkrY;
	}

	int nBbxAxis = nBbxStartIdx + (nPkrX + MAX_TRAY_PP_CNT_X * nPkrY);

	double dCurZPos = g_pBbxmnpClient->GetActualPos(eMotor, (_eBbxmnpAxis)nBbxAxis);
	g_VATCommonData.m_stVATPickerAlignInfoSeq.vMeasuredPickerPos[nHand].push_back(dCurZPos);
// 
// 	OneAxis* pMotorZ = NULL;
// 	switch (nHand)
// 	{
// 		case VAT_HAND_LOAD_TRAYPP:		pMotorZ = g_TaskTrayLoadPP.GetAxis(CTaskTrayLoadPP::eAxisZ); break;
// 		case VAT_HAND_UNLOAD_TRAYPP:	pMotorZ = g_TaskTrayUnloadPP.GetAxis(CTaskTrayUnloadPP::eAxisZ); break;
// 		case VAT_HAND_LOAD_TESTPP1:		pMotorZ = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisZ1); break;
// 		case VAT_HAND_UNLOAD_TESTPP1:	pMotorZ = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisZ2); break;
// 		case VAT_HAND_LOAD_TESTPP2:		pMotorZ = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisZ1); break;
// 		case VAT_HAND_UNLOAD_TESTPP2:	pMotorZ = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisZ2); break;
// 		default:
// 			break;
// 	}
// 
// 	if (pMotorZ != NULL) {
// 		//현재 Encoder값 저장.
// 		double dCurZPos = 0.0;
// 		pMotorZ->GetCurrentPos(dCurZPos);
// 
// 		g_VATCommonData.m_stVATPickerAlignInfoSeq.vMeasuredPickerPos[nHand].push_back(dCurZPos);
// 	}
}


void CVAT_VisionComm::SaveMeasureData_FocusedPickerZIdx(int nHand, ST_VAT_RCV_DATA stRcvData)
{
	if (stRcvData.nFocusedZIdx >= 0 && g_VATCommonData.m_stVATPickerAlignInfoSeq.vMeasuredPickerPos[nHand].size() > 0) {
		double dFocusedZPos = g_VATCommonData.m_stVATPickerAlignInfoSeq.vMeasuredPickerPos[nHand][stRcvData.nFocusedZIdx]; //해당 Hand의 N번째 값 Find

		g_VATCommonData.m_stVATPickerAlignInfoSeq.dPickerZFocusPos[nHand][m_nMeasurePkrIdx] = dFocusedZPos;
	}

	//높이 측정 값 초기화
	g_VATCommonData.m_stVATPickerAlignInfoSeq.vMeasuredPickerPos[nHand].clear();
}






int CVAT_VisionComm::ChkCamType(int nDataId)
{
	int nType = -1;

	if (nDataId == VISION_REQ_DATAID_VAT_LD_LOWER || nDataId == VISION_REQ_DATAID_VAT_ULD_LOWER
		|| nDataId == VISION_REQ_DATAID_VAT_TEST_1_LOWER || nDataId == VISION_REQ_DATAID_VAT_TEST_2_LOWER) {
		nType = _eCamType_Lower;
	}
	else {
		nType = _eCamType_Upper;
	}

	return nType;
}


BOOL CVAT_VisionComm::IsOffsetOKRange(int nHand, double dOffsetX, double dOffsetY)
{
	//Offset Ok Range 확인.
	double dOKRangeX = g_Motor[c_nAxisNo[nHand][VAT_X]]->GetScaleFactor()* 5;
	double dOKRangeY = g_Motor[c_nAxisNo[nHand][VAT_Y]]->GetScaleFactor()* 5;

	double dVisionOffsetX = fabs(dOffsetX); //um
	double dVisionOffsetY = fabs(dOffsetY); //um

	if ((dOKRangeX > dVisionOffsetX) && (dOKRangeY > dVisionOffsetY)) {
		return TRUE;
	}
	else {
		return FALSE;
	}

}

int CVAT_VisionComm::GetVisionPcNo(int nDataId)
{
	int nPC = -1;

	if (VISION_REQ_DATAID_VAT_LD_UPPER <= nDataId && nDataId <= VISION_REQ_DATAID_VAT_TEST_1_LOWER) {
		nPC = VAT_VISION_PC_1;
	}
	else {
		nPC = VAT_VISION_PC_2;
	}

	return nPC;
}


CString CVAT_VisionComm::ConvertVATModeToString(int nMode)
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

//각각의 Auto에 해당 하는 DataId -> Cam No로 변환.
int CVAT_VisionComm::ConvertDataIdToVatCam(int nDataId)
{
	int nCam = -1;
	switch (nDataId)
	{
	case VISION_REQ_DATAID_VAT_LD_UPPER:			nCam = VAT_CAM_LD_TRAY_PP_UPPER; break;
	case VISION_REQ_DATAID_VAT_LD_LOWER:			nCam = VAT_CAM_LD_TRAY_PP_LOWER; break;
	case VISION_REQ_DATAID_VAT_ULD_UPPER:			nCam = VAT_CAM_UD_TRAY_PP_UPPER; break;
	case VISION_REQ_DATAID_VAT_ULD_LOWER:			nCam = VAT_CAM_UD_TRAY_PP_LOWER; break;
	case VISION_REQ_DATAID_VAT_TEST_1_LD_UPPER:		nCam = VAT_CAM_LD_TEST_PP_1_UPPER; break;
	case VISION_REQ_DATAID_VAT_TEST_1_ULD_UPPER:	nCam = VAT_CAM_UD_TEST_PP_1_UPPER; break;
	case VISION_REQ_DATAID_VAT_TEST_1_LOWER: 		nCam = VAT_CAM_TEST_PP_1_LOWER; break;
	case VISION_REQ_DATAID_VAT_TEST_2_LD_UPPER:		nCam = VAT_CAM_LD_TEST_PP_2_UPPER; break;
	case VISION_REQ_DATAID_VAT_TEST_2_ULD_UPPER:	nCam = VAT_CAM_UD_TEST_PP_2_UPPER; break;
	case VISION_REQ_DATAID_VAT_TEST_2_LOWER: 		nCam = VAT_CAM_TEST_PP_2_LOWER; break;
	default:
		ASSERT("nWorkPos invalid" == 0);
		return -1;
	}

	return nCam;
}


//Vision Manual Dlg에서 해당 Cam에 해당 하는 Data ID 필요함.
int CVAT_VisionComm::ConvertVatCamToDataId(int nCam)
{
	int nDataId = -1;
	switch (nCam)
	{
	case VAT_CAM_LD_TRAY_PP_UPPER:			nDataId = VISION_REQ_DATAID_VAT_LD_UPPER; break;
	case VAT_CAM_LD_TRAY_PP_LOWER:			nDataId = VISION_REQ_DATAID_VAT_LD_LOWER; break;
	case VAT_CAM_UD_TRAY_PP_UPPER:			nDataId = VISION_REQ_DATAID_VAT_ULD_UPPER; break;
	case VAT_CAM_UD_TRAY_PP_LOWER:			nDataId = VISION_REQ_DATAID_VAT_ULD_LOWER; break;
	case VAT_CAM_LD_TEST_PP_1_UPPER:		nDataId = VISION_REQ_DATAID_VAT_TEST_1_LD_UPPER; break;
	case VAT_CAM_UD_TEST_PP_1_UPPER:		nDataId = VISION_REQ_DATAID_VAT_TEST_1_ULD_UPPER; break;
	case VAT_CAM_TEST_PP_1_LOWER: 			nDataId = VISION_REQ_DATAID_VAT_TEST_1_LOWER; break;
	case VAT_CAM_LD_TEST_PP_2_UPPER:		nDataId = VISION_REQ_DATAID_VAT_TEST_2_LD_UPPER; break;
	case VAT_CAM_UD_TEST_PP_2_UPPER:		nDataId = VISION_REQ_DATAID_VAT_TEST_2_ULD_UPPER; break;
	case VAT_CAM_TEST_PP_2_LOWER: 			nDataId = VISION_REQ_DATAID_VAT_TEST_2_LOWER; break;
	default:
		ASSERT("nWorkPos invalid" == 0);
		return -1;
	}

	return nDataId;
}


CString CVAT_VisionComm::ConvertCamIdToString(int nCam)
{
	CString strCam = _T("");

	switch (nCam)
	{
	case VAT_CAM_LD_TRAY_PP_UPPER:			strCam = _T("[UPPER CAM] LD TRAY PP"); break;
	case VAT_CAM_LD_TRAY_PP_LOWER:			strCam = _T("[LOWER CAM] LD TRAY PP"); break;
	case VAT_CAM_UD_TRAY_PP_UPPER:			strCam = _T("[UPPER CAM] UD TRAY PP"); break;
	case VAT_CAM_UD_TRAY_PP_LOWER:			strCam = _T("[LOWER CAM] UD TRAY PP"); break;
	case VAT_CAM_LD_TEST_PP_1_UPPER:		strCam = _T("[UPPER CAM] LD TEST PP #1"); break;
	case VAT_CAM_UD_TEST_PP_1_UPPER:		strCam = _T("[UPPER CAM] UD TEST PP #1"); break;
	case VAT_CAM_TEST_PP_1_LOWER: 			strCam = _T("[LOWER CAM] TEST PP #1"); break;
	case VAT_CAM_LD_TEST_PP_2_UPPER:		strCam = _T("[UPPER CAM] LD TEST PP #2"); break;
	case VAT_CAM_UD_TEST_PP_2_UPPER:		strCam = _T("[UPPER CAM] UD TEST PP #2"); break;
	case VAT_CAM_TEST_PP_2_LOWER: 			strCam = _T("[LOWER CAM] TEST PP #2"); break;
	default:
		ASSERT("nWorkPos invalid" == 0);
		return _T("");
	}

	return strCam;
}


CString CVAT_VisionComm::ConvertStatusToString(int nStatus)
{
	CString strStatus = _T("");

	switch (nStatus)
	{
	case VISION_STATUS_PLATE_JIG:		strStatus = _T("PLATE JIG");	break;
	case VISION_STATUS_PICKER:			strStatus = _T("PICKER");	break;
	case VISION_STATUS_C_TRAY:			strStatus = _T("C-TRAY");	break;
	case VISION_STATUS_TABLE:			strStatus = _T("TABLE");	break;
	case VISION_STATUS_SOCKET:			strStatus = _T("STATION");	break;
	case VISION_STATUS_AUTO_FOCUS:		strStatus = _T("CAM AUTO FOCUS");	break;
	case VISION_STATUS_LED_ON:			strStatus = _T("LED ON");	break;
	case VISION_STATUS_LED_OFF:			strStatus = _T("LED OFF");	break;
	default:
		break;
	}

	return strStatus;
}


int CVAT_VisionComm::ConvertVisionErrIdxToAlarmIdx(int nErr)
{
	int nAlarmIdx = -1;

	switch (nErr)
	{
	case VISION_ERROR_INSPECTION_SEQ_FINISH_WAIT:		nAlarmIdx = ERR_VAT_INSPECTION_SEQ_FINISH_WAIT; break;
	case VISION_ERROR_MEASURE_TYPE:						nAlarmIdx = ERR_VAT_MEASURE_TYPE;				break;
	case VISION_ERROR_INSPECTION_SEQ_START:				nAlarmIdx = ERR_VAT_INSPECTION_SEQ_START;		break;
	case VISION_ERROR_CAMERA_RESET_SEQ_START:			nAlarmIdx = ERR_VAT_CAMERA_RESET_SEQ_START;		break;
	case VISION_ERROR_CAMERA_RESER_RESULT_WAIT:			nAlarmIdx = ERR_VAT_CAMERA_RESER_RESULT_WAIT;	break;
	case VISION_ERROR_PRE_GRAB_SET:						nAlarmIdx = ERR_VAT_PRE_GRAB_SET;				break;
	case VISION_ERROR_GRAB_FAIL:						nAlarmIdx = ERR_VAT_GRAB_FAIL;					break;
	case VISION_ERROR_GRAB_WAIT:						nAlarmIdx = ERR_VAT_GRAB_WAIT;					break;
	case VISION_ERROR_INSPECTION:						nAlarmIdx = ERR_VAT_INSPECTION;					break;
	case VISION_ERROR_INSPECTION_TIMEOUT:				nAlarmIdx = ERR_VAT_INSPECTION_TIMEOUT;			break;
	case VISION_ERROR_RESULT_DATA_ERROR:				nAlarmIdx = ERR_VAT_RESULT_DATA_ERROR;			break;
	default:
		break;
	}

	return nAlarmIdx;
}