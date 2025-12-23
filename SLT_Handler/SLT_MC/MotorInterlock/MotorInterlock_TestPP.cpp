#include "StdAfx.h"
#include "MotorInterlock_TestPP.h"

#define STATION_X_GAP (100000) //7mm (7000)
#define STATION_Y_GAP (2000) //100mm (100000)


CMotorInterlock_TestPP::CMotorInterlock_TestPP(int nTestPpIdx, int nTestPpType)
	: m_nTestPpIdx(nTestPpIdx), m_nTestPpType(nTestPpType)
{
}


CMotorInterlock_TestPP::~CMotorInterlock_TestPP(void)
{
}

int CMotorInterlock_TestPP::Interlock_x_org()
{
	int nErr = ERR_NO_ERROR;

	nErr = Interlock_Orgcommon();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	
	int nAxisZ = (m_nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;

	//Home Sensor Check.
	bool bSafetyZ = g_TaskTestPP[m_nTestPpIdx].GetAxis(nAxisZ)->IsHomeSensorOn();
	if (bSafetyZ != true)
	{
		return (m_nTestPpType == eTestPPLoadType) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx;
	}
	

	//Safety Pos Check
	std::vector<int> vLocList;
	vLocList.clear();
	g_TaskTestPP[m_nTestPpIdx].WhereIsMotorPos(m_nTestPpType, vLocList, CTaskTestPP::eAxisAll); 

	if (vLocList[nAxisZ] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit)
	{
		return (m_nTestPpType == eTestPPLoadType) ? ERR_TEST_PP_1_Z1_MOVE_POS_NOT_SAFETY + m_nTestPpIdx : ERR_TEST_PP_1_Z2_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;
	}

	return ERR_NO_ERROR;
}
int CMotorInterlock_TestPP::Interlock_x_mov(double dTPos, double dSpd, int nAcc, int nDcc)
{
	int nErr = ERR_NO_ERROR;

	nErr = Interlock_common();
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}

	nErr = Interlock_PressUnit();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotorZ1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ1);
	OneAxis* pMotorZ2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ2);

	double dCurPosZ1 = 0.0, dCurPosZ2 = 0.0;

	pMotorZ1->GetCurrentPos(dCurPosZ1);
	pMotorZ2->GetCurrentPos(dCurPosZ2);

	std::vector<int> vLocList;
	vLocList.clear();
	g_TaskTestPP[m_nTestPpIdx].WhereIsMotorPos(m_nTestPpType, vLocList, CTaskTestPP::eAxisAll);

	//Z축  Motion Done 확인.
	if (pMotorZ1->MotionDone() != ERR_NO_ERROR)
		return ERR_TEST_PP_1_Y_MOVE_NOT_MOTION_DONE_Z1 + m_nTestPpIdx;
	if (pMotorZ2->MotionDone() != ERR_NO_ERROR)
		return ERR_TEST_PP_1_Y_MOVE_NOT_MOTION_DONE_Z2 + m_nTestPpIdx;

	int nAxisZ = (m_nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;

	//Z Home Sensor Check.
	bool bSafetyZ = g_TaskTestPP[m_nTestPpIdx].GetAxis(nAxisZ)->IsHomeSensorOn();
	if (bSafetyZ != true)
	{
		return (m_nTestPpType == eTestPPLoadType) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx;
	}

	// Z축이 안전 위치가 아니다.
	if (vLocList[nAxisZ] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit)
	{
		return (m_nTestPpType == eTestPPLoadType) ? ERR_TEST_PP_1_Z1_MOVE_POS_NOT_SAFETY + m_nTestPpIdx : ERR_TEST_PP_1_Z2_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;
	}

	//1. X를 Station 까지 뻗으려 한다면
	//2. Y의 위치가 Station 입구에 들어오는 영역인가?(Teaching값으로 판단)
	// Station Gap 2mm
	BOOL vat_in_process = g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_USE);
	nErr = Interlock_X_Related_Station_Range(dTPos);
	if (nErr != ERR_NO_ERROR && vat_in_process != TRUE) {
		return nErr;
	}


	//X축 Station 영역 확인.
	BOOL bChkStationArea = FALSE;
	if (m_nTestPpIdx == eTestPP_1)
	{
		if (dTPos < INTERLOCK_TESTPP_1_X_STATION_START_POS) { bChkStationArea = TRUE; }
	}
	else
	{
		if (dTPos > INTERLOCK_TESTPP_2_X_STATION_START_POS) { bChkStationArea = TRUE; }
	}

	//Cleaning 처리
	double dCurPosY = 0;
	OneAxis* pMotorY = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisY);
	pMotorY->GetCurrentPos(dCurPosY);

	double dTeachPos[CTaskTestPP::eAxisMax] = { 0, };
	std::vector<CPoint> vTempPkr;
	vTempPkr.push_back(CPoint(0, 0));
	g_TaskTestPP[m_nTestPpIdx].GetTeachPos(LOC_IDX_CLEAN_TBL, CPoint(0, 0), vTempPkr, CTaskTestPP::eZPOS_SAFETY, dTeachPos, eTestPPLoadType);
	
	if (dTeachPos[CTaskTestPP::eAxisY] - 500.0 <= dCurPosY && dCurPosY <= dTeachPos[CTaskTestPP::eAxisY] + 500.0) //Y가 해당 Pos내에 있으면 Clean Table 위치
	{
		if (m_nTestPpIdx == eTestPP_1)
		{
			if (dTPos < INTERLOCK_TESTPP1_X_CLEAN_TABLE_LIMIT) 
			{
				bChkStationArea = TRUE; 
			}
			else
			{
				bChkStationArea = FALSE;
			}
		}
		else
		{
			if (dTPos > INTERLOCK_TESTPP2_X_CLEAN_TABLE_LIMIT) 
			{
				bChkStationArea = TRUE; 
			}
			else
			{
				bChkStationArea = FALSE;
			}
		}
	}

	//1. X를 Station 까지 뻗으려 한다면
	//2. Y의 위치가 Press Unit과 간섭이 나지 않는 위치인가?
	if (bChkStationArea)
	{
//		int nFindPressIdx = Interlock_Y_Range_Related_Press_Z_Safety(dTPos, m_nTestPpType);

		std::vector<int> vStationNo;
		vStationNo.clear();

		Interlock_Y_Range_Related_Press_Z_Safety(dTPos, m_nTestPpType, vStationNo);
		if (vStationNo.size() != 0) {
			for (int i = 0; i < (int)vStationNo.size(); i++) {
				int nZidx = (m_nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
				if (vLocList[nZidx] <= CTaskTestPP::ePosZ_DownSafety)
				{
					int nPressIdx = g_TaskTestPP[m_nTestPpIdx].GetPressNum(LOC_IDX_STATION_1 + vStationNo[i]);
					if (g_TaskPressUnit[nPressIdx].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON) != ERR_NO_ERROR)
						return ERR_PRESS_UNIT_1_NOT_DETECT_SAFETY_SEN + nPressIdx;
					if (g_TaskPressUnit[nPressIdx].ChkMotorPos(LOC_IDX_PRESS_UNIT_SAFETY, CTaskPressUnit::eAxisAll) != ERR_NO_ERROR)
						return ERR_MOT_INTERLOCK_PRESS_UNIT_1_NOT_DETECT_SAFETY_POS + nPressIdx;
				}
			}
		}

// 		if (nFindPressIdx != -1) {
// 			int nZidx = (m_nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
// 			if (vLocList[nZidx] <= CTaskTestPP::ePosZ_DownSafety)
// 			{
// 				int nPressIdx = g_TaskTestPP[m_nTestPpIdx].GetPressNum(LOC_IDX_STATION_1 + nFindPressIdx);
// 				if (g_TaskPressUnit[nPressIdx].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON) != ERR_NO_ERROR)
// 					return ERR_PRESS_UNIT_1_NOT_DETECT_SAFETY_SEN + nPressIdx;
// 				if (g_TaskPressUnit[nPressIdx].ChkMotorPos(LOC_IDX_PRESS_UNIT_SAFETY, CTaskPressUnit::eAxisAll) != ERR_NO_ERROR)
// 					return ERR_MOT_INTERLOCK_PRESS_UNIT_1_NOT_DETECT_SAFETY_POS + nPressIdx;
// 			}
// 		}
	}

	return ERR_NO_ERROR;
	/////////////////////////////////////////////////////////////////////////////////////////////////

// 	std::vector<int> vLocList;
// 	vLocList.clear();
// 	g_TaskTestPP[m_nTestPpIdx].WhereIsMotorPos(m_nTestPpType, vLocList, CTaskTestPP::eAxisAll);
// 
// 	OneAxis* pMotorX1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisX1);
// 	OneAxis* pMotorX2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisX2);
// 	OneAxis* pMotorY = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisY);
// 	OneAxis* pMotorZ1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ1);
// 	OneAxis* pMotorZ2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ2);
// 	double dCurPosX1, dCurPosX2, dCurPosY, dCurPosZ1, dCurPosZ2;
// 
// 	pMotorX1->GetCurrentPos(dCurPosX1);
// 	pMotorX2->GetCurrentPos(dCurPosX2);
// 	pMotorY->GetCurrentPos(dCurPosY);
// 	pMotorZ1->GetCurrentPos(dCurPosZ1);
// 	pMotorZ2->GetCurrentPos(dCurPosZ2);
// 
// 	if (pMotorZ1->MotionDone() != ERR_NO_ERROR)
// 		return ERR_TEST_PP_1_Y_MOVE_NOT_MOTION_DONE_Z1 + m_nTestPpIdx;
// 	if (pMotorZ2->MotionDone() != ERR_NO_ERROR)
// 		return ERR_TEST_PP_1_Y_MOVE_NOT_MOTION_DONE_Z2 + m_nTestPpIdx;
// 
// 	double dTPosHome[CTaskTestPP::eAxisMax] = { 0, };
// 	std::vector<CPoint> vPicker;
// 	vPicker.push_back(CPoint(0, 0));
// 
// 	g_TaskTestPP[m_nTestPpIdx].GetTeachPos(LOC_IDX_TEST_PP_HOME, CPoint(0, 0), vPicker, CTaskTestPP::eZPOS_SAFETY, dTPosHome, m_nTestPpType);
// 
// 
// 	int nAxisZ = (m_nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
// 
// 	//Z Home Sensor Check.
// 	bool bSafetyZ = g_TaskTestPP[m_nTestPpIdx].GetAxis(nAxisZ)->IsHomeSensorOn();
// 	if (bSafetyZ != true)
// 	{
// 		return (m_nTestPpType == eTestPPLoadType) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx;
// 	}
// 
// 
// 	// Z축이 안전 위치가 아니다.
// 	if (vLocList[nAxisZ] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit)
// 	{
// 		return (m_nTestPpType == eTestPPLoadType) ? ERR_TEST_PP_1_Z1_MOVE_POS_NOT_SAFETY + m_nTestPpIdx : ERR_TEST_PP_1_Z2_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;
// 	}
// 
// 
// 
// 	// x가 station쪽으로 움직일 경우 범주안에 없으면 기둥에 부딛힌다.
// 	double dXHomePos = 0.0;
// 	if (m_nTestPpType == eTestPPLoadType) { dXHomePos = dTPosHome[CTaskTestPP::eAxisX1]; }
// 	else                                  { dXHomePos = dTPosHome[CTaskTestPP::eAxisX2]; }
// 			
// 
// 	//if (dTPos >= dTPosHome[CTaskTestPP::eAxisX1] + 200000.0)
// 	//Rear Left, Front Right 값으로 Station 범위 확인, 사용자 Offset가미된 Teaching 값.
// 	double dBeforeStation = (m_nTestPpIdx == eTestPP_1) ? INTERLOCK_TESTPP_1_X_STATION_START_POS : INTERLOCK_TESTPP_2_X_STATION_START_POS;
// 
// 	BOOL bChkStationArea = FALSE;
// 	if (m_nTestPpIdx == eTestPP_1)
// 	{
// 		if (dTPos <= dXHomePos + INTERLOCK_TESTPP_1_X_STATION_START_POS) { bChkStationArea = TRUE; }
// 	}
// 	else
// 	{
// 		if (dTPos >= dXHomePos + INTERLOCK_TESTPP_2_X_STATION_START_POS) { bChkStationArea = TRUE; }
// 	}
// 
// 	if(bChkStationArea/*dTPos >= dXHomePos + dBeforeStation*/) //[측정 필요]Home Position에서 Station 진입 영역까지 거리
// 	{
// 		std::vector<CPoint> vPicker_RL1, vPicker_FR4;
// 		vPicker_RL1.push_back(CPoint(0, 0));
// 		vPicker_FR4.push_back(CPoint(MAX_TEST_PP_PKR_CNT_X - 1, MAX_TEST_PP_PKR_CNT_Y - 1));
// 
// 		double dTRect_RL[MAX_TEST_PP_LOC_COUNT][CTaskTestPP::eAxisMax] = { { 0.0, }, }; // 최대 Rear  방향 & Pick Z Teaching
// 		double dTRect_FR[MAX_TEST_PP_LOC_COUNT][CTaskTestPP::eAxisMax] = { { 0.0, }, }; // 최대 Front 방향 & Pick Z Teaching
// 
// 		int nSitePocketCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
// 		int nSitePocketCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
// 
// 		for (int nStage = 0; nStage < MAX_TEST_PP_LOC_COUNT; nStage++) {
// 			// Get rear left & front right rect position
// 			g_TaskTestPP[m_nTestPpIdx].GetTeachPos(nStage, CPoint(0, 0), vPicker_FR4, CTaskTestPP::eZPOS_SAFETY, dTRect_RL[nStage], m_nTestPpType);
// 			g_TaskTestPP[m_nTestPpIdx].GetTeachPos(nStage, CPoint(nSitePocketCntX - 1, nSitePocketCntY - 1), vPicker_RL1, CTaskTestPP::eZPOS_SAFETY, dTRect_FR[nStage], m_nTestPpType);
// 		}
// 
// 		BOOL bFindTeachPos = FALSE;
// 		int nFindPressIdx = -1;
// 
// 		for (int i = 0; i < (eMaxPressUnitCount / 2); i++)
// 		{
// 			double dStationCenter = (dTRect_FR[LOC_IDX_STATION_1 + i][CTaskTestPP::eAxisY] + dTRect_RL[LOC_IDX_STATION_1 + i][CTaskTestPP::eAxisY]) / 2.0;
// 			if (dStationCenter - STATION_CENTER_TO_LR_HW_DIST <= dCurPosY && dCurPosY <= dStationCenter + STATION_CENTER_TO_LR_HW_DIST) //[측정 필요] HW 거리 측정 필요.
// 			{
// 				bFindTeachPos = TRUE;
// 				nFindPressIdx = i;
// 			}
// 		}
// 		if (bFindTeachPos != TRUE)
// 			return ( m_nTestPpType == eTestPPLoadType ) ? ERR_TEST_PP_1_X1_MOVE_NOT_SAFETY_Y_POSITION + m_nTestPpIdx : ERR_TEST_PP_1_X2_MOVE_NOT_SAFETY_Y_POSITION + m_nTestPpIdx;
// 		//Test PP 1번 Load Hand X축을 움직이려 하였으나, Test PP 1번 Y축이 안전영역에 있지 않습니다.
// 
// 		if (vLocList[CTaskTestPP::eAxisZ1] <= CTaskTestPP::ePosZ_DownSafety)
// 		{
// 			int nPressIdx = g_TaskTestPP[m_nTestPpIdx].GetPressNum(LOC_IDX_STATION_1 + nFindPressIdx);
// 			if (g_TaskPressUnit[nPressIdx].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON) != ERR_NO_ERROR)
// 				return ERR_PRESS_UNIT_1_NOT_DETECT_SAFETY_SEN + nPressIdx;
// 			if (g_TaskPressUnit[nPressIdx].ChkMotorPos(LOC_IDX_PRESS_UNIT_SAFETY, CTaskPressUnit::eAxisAll) != ERR_NO_ERROR)
// 				return ERR_PRESS_UNIT_1_NOT_DETECT_SAFETY_SEN + nPressIdx;
// 
// 
// 		}
// 	}

//	return ERR_NO_ERROR;
}

int CMotorInterlock_TestPP::Interlock_x_jog( double dSpd, int nAcc, int nDcc )
{
	int nErr = ERR_NO_ERROR;

	nErr = Interlock_common();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	nErr = Interlock_PressUnit();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

// 	int nAxisZ = (m_nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
// 
// 	//Z Home Sensor Check.
// 	bool bSafetyZ = g_TaskTestPP[m_nTestPpIdx].GetAxis(nAxisZ)->IsHomeSensorOn();
// 	if (bSafetyZ != true)
// 	{
// 		return (m_nTestPpType == eTestPPLoadType) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx;
// 	}
// 
// 
// 	// Z축이 안전 위치가 아니다.
// 	std::vector<int> vLocList;
// 	vLocList.clear();
// 	g_TaskTestPP[m_nTestPpIdx].WhereIsMotorPos(m_nTestPpType, vLocList, CTaskTestPP::eAxisAll);
// 
// 	if (vLocList[nAxisZ] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit)
// 	{
// 		return (m_nTestPpType == eTestPPLoadType) ? ERR_TEST_PP_1_Z1_MOVE_POS_NOT_SAFETY + m_nTestPpIdx : ERR_TEST_PP_1_Z2_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;
// 	}


	OneAxis* pMotorX = g_TaskTestPP[m_nTestPpIdx].GetAxis((m_nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2);
	OneAxis* pMotorZ1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ1);
	OneAxis* pMotorZ2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ2);

	double dCurPosX = 0.0, dCurPosZ1 = 0.0, dCurPosZ2 = 0.0;

	pMotorX->GetCurrentPos(dCurPosX);
	pMotorZ1->GetCurrentPos(dCurPosZ1);
	pMotorZ2->GetCurrentPos(dCurPosZ2);

	std::vector<int> vLocList;
	vLocList.clear();
	g_TaskTestPP[m_nTestPpIdx].WhereIsMotorPos(m_nTestPpType, vLocList, CTaskTestPP::eAxisAll);

	//Z축  Motion Done 확인.
	if (pMotorZ1->MotionDone() != ERR_NO_ERROR)
		return ERR_TEST_PP_1_Y_MOVE_NOT_MOTION_DONE_Z1 + m_nTestPpIdx;
	if (pMotorZ2->MotionDone() != ERR_NO_ERROR)
		return ERR_TEST_PP_1_Y_MOVE_NOT_MOTION_DONE_Z2 + m_nTestPpIdx;

	int nAxisZ = (m_nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;

	//Z Home Sensor Check.
	bool bSafetyZ = g_TaskTestPP[m_nTestPpIdx].GetAxis(nAxisZ)->IsHomeSensorOn();
	if (bSafetyZ != true)
	{
		return (m_nTestPpType == eTestPPLoadType) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx;
	}

	// Z축이 안전 위치가 아니다.
	if (vLocList[nAxisZ] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit)
	{
		return (m_nTestPpType == eTestPPLoadType) ? ERR_TEST_PP_1_Z1_MOVE_POS_NOT_SAFETY + m_nTestPpIdx : ERR_TEST_PP_1_Z2_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;
	}

	//1. X를 Station 까지 뻗으려 한다면
	//2. Y의 위치가 Station 입구에 들어오는 영역인가?(Teaching값으로 판단)
	// Station Gap 2mm
	nErr = Interlock_X_Related_Station_Range(dCurPosX);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}


	//X축 Station 영역 확인.
	BOOL bChkStationArea = FALSE;
	if (m_nTestPpIdx == eTestPP_1)
	{
		if (dCurPosX < INTERLOCK_TESTPP_1_X_STATION_START_POS) { bChkStationArea = TRUE; }
	}
	else
	{
		if (dCurPosX > INTERLOCK_TESTPP_2_X_STATION_START_POS) { bChkStationArea = TRUE; }
	}

	//1. X를 Station 까지 뻗으려 한다면
	//2. Y의 위치가 Press Unit과 간섭이 나지 않는 위치인가?
	if (bChkStationArea)
	{
		std::vector<int> vStationNo;
		vStationNo.clear();

		Interlock_Y_Range_Related_Press_Z_Safety(dCurPosX, m_nTestPpType, vStationNo);

		if (vStationNo.size() != 0) {
			for (int i = 0; i < (int)vStationNo.size(); i++) {
				int nZidx = (m_nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
				if (vLocList[nZidx] <= CTaskTestPP::ePosZ_DownSafety)
				{
					int nPressIdx = g_TaskTestPP[m_nTestPpIdx].GetPressNum(LOC_IDX_STATION_1 + vStationNo[i]);
					if (g_TaskPressUnit[nPressIdx].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON) != ERR_NO_ERROR)
						return ERR_PRESS_UNIT_1_NOT_DETECT_SAFETY_SEN + nPressIdx;
					if (g_TaskPressUnit[nPressIdx].ChkMotorPos(LOC_IDX_PRESS_UNIT_SAFETY, CTaskPressUnit::eAxisAll) != ERR_NO_ERROR)
						return ERR_MOT_INTERLOCK_PRESS_UNIT_1_NOT_DETECT_SAFETY_POS + nPressIdx;
				}
			}
		}


		//int nFindPressIdx = Interlock_Y_Range_Related_Press_Z_Safety(dCurPosX, m_nTestPpType);
// 		if (nFindPressIdx != -1) {
// 			int nZidx = (m_nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
// 			if (vLocList[nZidx] <= CTaskTestPP::ePosZ_DownSafety)
// 			{
// 				int nPressIdx = g_TaskTestPP[m_nTestPpIdx].GetPressNum(LOC_IDX_STATION_1 + nFindPressIdx);
// 				if (g_TaskPressUnit[nPressIdx].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON) != ERR_NO_ERROR)
// 					return ERR_PRESS_UNIT_1_NOT_DETECT_SAFETY_SEN + nPressIdx;
// 				if (g_TaskPressUnit[nPressIdx].ChkMotorPos(LOC_IDX_PRESS_UNIT_SAFETY, CTaskPressUnit::eAxisAll) != ERR_NO_ERROR)
// 					return ERR_MOT_INTERLOCK_PRESS_UNIT_1_NOT_DETECT_SAFETY_POS + nPressIdx;
// 			}
// 		}
	}

	return ERR_NO_ERROR;



/////////////////////////////////////////////////////////////////////////////////////
 	// x가 station쪽으로 움직일 경우 범주안에 없으면 기둥에 부딛힌다.
 // 	OneAxis* pMotorX1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisX1);
 // 	OneAxis* pMotorX2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisX2);
//  	
//  	OneAxis* pMotorX = g_TaskTestPP[m_nTestPpIdx].GetAxis((m_nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2);
//  	OneAxis* pMotorY = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisY);
//  
//  	double dCurPosX= 0,dCurPosY = 0;
//  	pMotorX->GetCurrentPos(dCurPosX);
//  	pMotorY->GetCurrentPos(dCurPosY);
//  
//  	double dTPosHome[CTaskTestPP::eAxisMax] = { 0, };
//  	std::vector<CPoint> vPicker;
//  	vPicker.push_back(CPoint(0, 0));
//  
//  	g_TaskTestPP[m_nTestPpIdx].GetTeachPos(LOC_IDX_TEST_PP_HOME, CPoint(0, 0), vPicker, CTaskTestPP::eZPOS_SAFETY, dTPosHome, m_nTestPpType);
//  
//  	
//  	double dXHomePos = 0.0;
//  	if (m_nTestPpType == eTestPPLoadType) { dXHomePos = dTPosHome[CTaskTestPP::eAxisX1]; }
//  	else { dXHomePos = dTPosHome[CTaskTestPP::eAxisX2]; }
//  
//  	//Rear Left, Front Right 값으로 Station 범위 확인, 사용자 Offset가미된 Teaching 값.
//  //	if (dSpd > 0 && dTPos >= dXHomePos + 200000.0) //[측정 필요]Home Position에서 Station 진입 영역까지 거리
//  	BOOL bStationAreaChk = FALSE;
//  	if (m_nTestPpIdx == eTestPP_1) {
//  		if (dCurPosX < INTERLOCK_TESTPP_1_X_STATION_START_POS) { bStationAreaChk = TRUE; }
//  
//  	}
//  	else {
//  		if(dCurPosX > INTERLOCK_TESTPP_2_X_STATION_START_POS) { bStationAreaChk = TRUE; }
//  	}
//  
//  	if (bStationAreaChk == TRUE) //[측정 필요]Home Position에서 Station 진입 영역까지 거리, Jog는 buffer까지는 움직이도록 측정 값은 Station 진입 전 위치로 한다.
//  	{
//  		std::vector<CPoint> vPicker_RL1, vPicker_FR4;
//  		vPicker_RL1.push_back(CPoint(0, 0));
//  		vPicker_FR4.push_back(CPoint(MAX_TEST_PP_PKR_CNT_X - 1, MAX_TEST_PP_PKR_CNT_Y - 1));
//  
//  		double dTRect_RL[MAX_TEST_PP_LOC_COUNT][CTaskTestPP::eAxisMax] = { { 0.0, }, }; // 최대 Rear  방향 & Pick Z Teaching
//  		double dTRect_FR[MAX_TEST_PP_LOC_COUNT][CTaskTestPP::eAxisMax] = { { 0.0, }, }; // 최대 Front 방향 & Pick Z Teaching
//  
//  		int nSitePocketCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
//  		int nSitePocketCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
//  
//  		for (int nStage = 0; nStage < MAX_TEST_PP_LOC_COUNT; nStage++) {
//  			// Get rear left & front right rect position
//  			g_TaskTestPP[m_nTestPpIdx].GetTeachPos(nStage, CPoint(0, 0), vPicker_FR4, CTaskTestPP::eZPOS_SAFETY, dTRect_RL[nStage], m_nTestPpType);
//  			g_TaskTestPP[m_nTestPpIdx].GetTeachPos(nStage, CPoint(nSitePocketCntX - 1, nSitePocketCntY - 1), vPicker_RL1, CTaskTestPP::eZPOS_SAFETY, dTRect_FR[nStage], m_nTestPpType);
//  		}
//  
//  		BOOL bFindTeachPos = FALSE;
//  		int nFindPressIdx = -1;
//  
//  		for (int i = 0; i < (eMaxPressUnitCount / 2); i++)
//  		{
//  			double dStationCenter = (dTRect_FR[LOC_IDX_STATION_1 + i][CTaskTestPP::eAxisY] + dTRect_RL[LOC_IDX_STATION_1 + i][CTaskTestPP::eAxisY]) / 2.0;
//  			if (dStationCenter - (STATION_CENTER_TO_LR_HW_DIST - 110000.0) <= dCurPosY && dCurPosY <= dStationCenter + (STATION_CENTER_TO_LR_HW_DIST - 110000.0)) //[측정 필요] HW 거리 측정 필요.
//  			{
//  				bFindTeachPos = TRUE;
//  				nFindPressIdx = i;
//  			}
//  		}
//  		if (bFindTeachPos != TRUE)
//  			return (m_nTestPpType == eTestPPLoadType) ? ERR_TEST_PP_1_X1_MOVE_NOT_SAFETY_Y_POSITION + m_nTestPpIdx : ERR_TEST_PP_1_X2_MOVE_NOT_SAFETY_Y_POSITION + m_nTestPpIdx;
//  		//Test PP 1번 Load Hand X축을 움직이려 하였으나, Test PP 1번 Y축이 안전영역에 있지 않습니다.
//  
//  
//  		int nAxisZidx = (m_nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
//  		if (vLocList[nAxisZidx] <= CTaskTestPP::ePosZ_DownSafety)
//  		{
//  			int nPressIdx = g_TaskTestPP[m_nTestPpIdx].GetPressNum(LOC_IDX_STATION_1 + nFindPressIdx);
//  			if (g_TaskPressUnit[nPressIdx].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON) != ERR_NO_ERROR)
//  				return ERR_PRESS_UNIT_1_NOT_DETECT_SAFETY_SEN + nPressIdx;
//  			if (g_TaskPressUnit[nPressIdx].ChkMotorPos(LOC_IDX_PRESS_UNIT_SAFETY, CTaskPressUnit::eAxisAll) != ERR_NO_ERROR)
//  				return ERR_PRESS_UNIT_1_NOT_DETECT_SAFETY_SEN + nPressIdx;
//  		}
//  	}
//  
//  	return 0;
}

int CMotorInterlock_TestPP::Interlock_y_org()
{
	int nErr = ERR_NO_ERROR;

	nErr = Interlock_Orgcommon();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	//Test Hand X축 Origin Check(X1, X2)
	OneAxis* pMotorX1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisX1);
	if (pMotorX1->IsMotorUsable(nErr) != TRUE)
	{
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TEST_PP_1_X1_IS_AMP_FAULT + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_X1_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}
	
	OneAxis* pMotorX2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisX2);
	if (pMotorX2->IsMotorUsable(nErr) != TRUE)
	{
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TEST_PP_1_X2_IS_AMP_FAULT + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_X2_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}

	// Z축 Home 센서가 감지되지 않는다. ( B접점 )
	bool bSafetyZ1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ1)->IsHomeSensorOn();
	bool bSafetyZ2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ2)->IsHomeSensorOn();

	if (bSafetyZ1 != TRUE || bSafetyZ2 != TRUE)
		return (bSafetyZ1 != TRUE) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx;

	//Z축 Safety Pos
	std::vector<int> vLocList;
	vLocList.clear();
	g_TaskTestPP[m_nTestPpIdx].WhereIsMotorPos(m_nTestPpType, vLocList, CTaskTestPP::eAxisAll);

	if (vLocList[CTaskTestPP::eAxisZ1] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit)
		return ERR_TEST_PP_1_Z1_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;
	if (vLocList[CTaskTestPP::eAxisZ2] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit)
		return ERR_TEST_PP_1_Z2_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;

	// X축이 잡혀 있지 않으면 Y축을 잡을 수 없다
	//1. X축 Safety Sensor 확인.
	int nSafetyX1Sen = g_TaskTestPP[m_nTestPpIdx].ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON);
	int nSafetyX2Sen = g_TaskTestPP[m_nTestPpIdx].ChkDI(CTaskTestPP::xX2AXIS_POSITION_SEN, DEF_ON);

	if (nSafetyX1Sen != ERR_NO_ERROR)
		return ERR_MOT_INTERLOCK_TEST_PP_1_X1_POSITION_NOT_DETECTED_SAFETY_SENSOR + m_nTestPpIdx;

	if (nSafetyX2Sen != ERR_NO_ERROR)
		return ERR_MOT_INTERLOCK_TEST_PP_1_X2_POSITION_NOT_DETECTED_SAFETY_SENSOR + m_nTestPpIdx;


	//2. X축 Safety Position 확인.
// 	OneAxis* pMotorX1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisX1);
// 	OneAxis* pMotorX2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisX2);
	double dCurPosX1 = 0, dCurPosX2 = 0;
	pMotorX1->GetCurrentPos(dCurPosX1);
	pMotorX2->GetCurrentPos(dCurPosX2);

	double dTestPP_X_SafetyPos = (m_nTestPpIdx == eTestPP_1) ? INTERLOCK_TESTPP_X_SAFETY_POS*(-1) : INTERLOCK_TESTPP_X_SAFETY_POS;
	//X1
	if (g_TaskTestPP[m_nTestPpIdx].CompareXData(dCurPosX1, dTestPP_X_SafetyPos) != TRUE) { //True면 safety Pos 내, Fals면 Safety Pos 밖.
		return ERR_TEST_PP_1_X1_POSITION_NOT_SAFETY + m_nTestPpIdx;
	}
	//X2
	if (g_TaskTestPP[m_nTestPpIdx].CompareXData(dCurPosX2, dTestPP_X_SafetyPos) != TRUE) {
		return ERR_TEST_PP_1_X2_POSITION_NOT_SAFETY + m_nTestPpIdx;
	}
	

	return ERR_NO_ERROR;
}

/////////////////////

int CMotorInterlock_TestPP::Interlock_X_Related_Station_Range(double dTargetX) //Y위치를 보고 움직임 (Station입구에 들어오는 범위인지 본다)
{

 	OneAxis* pMotorY = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisY);
 	double dStationXLimit = 0.0;
 	double dStationYLimit = 0.0;
 
 	BOOL bChkStationBeforeArea = FALSE;
 	if (m_nTestPpIdx == eTestPP_1) {
 		dStationXLimit = (-1)* INTERLOCK_TESTPP_X_POS_WITH_TESTPP_Z_AND_TBL_FRONT;
 		if ((dStationXLimit /*+ (STATION_X_GAP - 90000)*/) < dTargetX) { bChkStationBeforeArea = TRUE; }
 	}
 	else {
 		dStationXLimit = INTERLOCK_TESTPP_X_POS_WITH_TESTPP_Z_AND_TBL_FRONT;
 		if ((dStationXLimit /*+ (STATION_X_GAP - 90000)*/) > dTargetX) { bChkStationBeforeArea = TRUE; }
 	}
 
 	double dCurPosY = 0;
 	pMotorY->GetCurrentPos(dCurPosY);
 
	//Cleaning 처리
	double dTPos[CTaskTestPP::eAxisMax] = { 0, };
	std::vector<CPoint> vTempPkr;
	vTempPkr.push_back(CPoint(0, 0));
	g_TaskTestPP[m_nTestPpIdx].GetTeachPos(LOC_IDX_CLEAN_TBL, CPoint(0, 0), vTempPkr, CTaskTestPP::eZPOS_SAFETY, dTPos, eTestPPLoadType);
	if (dTPos[CTaskTestPP::eAxisY] - 500.0 <= dCurPosY && dCurPosY <= dTPos[CTaskTestPP::eAxisY] + 500.0) //Y가 해당 Pos내에 있으면 Clean Table 위치
	{
		double dCleanTblLimitX = (m_nTestPpIdx == eTestPP_1) ? INTERLOCK_TESTPP1_X_CLEAN_TABLE_LIMIT : INTERLOCK_TESTPP2_X_CLEAN_TABLE_LIMIT;
		if (m_nTestPpIdx == eTestPP_1) {
			if(dCleanTblLimitX < dTargetX) { bChkStationBeforeArea = TRUE; }
		}
		else {
			if (dCleanTblLimitX > dTargetX) { bChkStationBeforeArea = TRUE; }
		}
	}


 	if (bChkStationBeforeArea)
 	{
 		// 2. X를 Station 까지 뻗는것이 아니면 상관 없음
 		return ERR_NO_ERROR;
 	}
 	else
 	{
 		// 1. X를 Station 까지 뻗으려 한다면
 		// 2. Y의 위치가 Station 입구에 들어오는 영역인가?
 		for (int nStationNum = LOC_IDX_STATION_1; nStationNum <= LOC_IDX_STATION_4; nStationNum++)
 		{
 			int nPosition = POS_IDX_TEST_PP_Y1;
 			if (m_nTestPpType == eTestPPUnLoadType) {
 				nPosition = POS_IDX_TEST_PP_Y2;
 			}
 
 			dStationYLimit = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPpIdx), nStationNum, nPosition);
 
 			if ((dStationYLimit - STATION_Y_GAP) < dCurPosY && dCurPosY < (dStationYLimit + STATION_Y_GAP))
 			{
 				//Y 위치가 특정 station 진입 위치 확인 OK
 				return ERR_NO_ERROR;
 			}
 		}
 		//for(nStationNum) 에서 걸러지지 안았다면 충돌 상황.
 		if(g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
 			return (m_nTestPpType == eTestPPLoadType) ? ERR_TEST_PP_1_X1_MOVE_NOT_SAFETY_Y_POSITION + m_nTestPpIdx : ERR_TEST_PP_1_X2_MOVE_NOT_SAFETY_Y_POSITION + m_nTestPpIdx; //에러
 		else
 			return ERR_NO_ERROR;
 	}

}

////////////////////
int CMotorInterlock_TestPP::Interlock_y_mov(double dTPos, double dSpd, int nAcc, int nDcc)
{
	int nErr = ERR_NO_ERROR;

	nErr = Interlock_common();
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}
	nErr = Interlock_PressUnit();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	std::vector<int> vLocList;
	vLocList.clear();
	g_TaskTestPP[m_nTestPpIdx].WhereIsMotorPos(m_nTestPpType, vLocList, CTaskTestPP::eAxisAll);
	OneAxis* pMotorY = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisY);
	OneAxis* pMotorZ1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ1);
	OneAxis* pMotorX1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisX1);
	OneAxis* pMotorZ2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ2);
	OneAxis* pMotorX2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisX2);

	double dCurPosY = 0, dCurPosZ1 = 0, dCurPosX1 = 0, dCurPosZ2 = 0, dCurPosX2 = 0;
	pMotorY->GetCurrentPos(dCurPosY);
	pMotorZ1->GetCurrentPos(dCurPosZ1);
	pMotorX1->GetCurrentPos(dCurPosX1);
	pMotorZ2->GetCurrentPos(dCurPosZ2);
	pMotorX2->GetCurrentPos(dCurPosX2);

	std::vector<CPoint> vPicker;
	vPicker.push_back(CPoint(0, 0));

	double dTPosHome[CTaskTestPP::eAxisMax] = { 0, };
	g_TaskTestPP[m_nTestPpIdx].GetTeachPos(LOC_IDX_TEST_PP_HOME, CPoint(0, 0), vPicker, CTaskTestPP::eZPOS_SAFETY, dTPosHome, eTestPPLoadType);



	// Station Front Picker X Axis Offset 때문에 이동 필요 (Offset +-1mm + Backlash offset)
	if (abs(dCurPosY - dTPos) > 2000.0) {
		// 1. X축 안전 센서가 감지되지 않는데 움직일 수 없다..
		// Load, Unload X축 Safety 확인 후, Y Move 가능.
		int nSafetyX1Sen = g_TaskTestPP[m_nTestPpIdx].ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON);
		int nSafetyX2Sen = g_TaskTestPP[m_nTestPpIdx].ChkDI(CTaskTestPP::xX2AXIS_POSITION_SEN, DEF_ON);

// 		BOOL bGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
// 		if (bGrrUse == TRUE) 
// 		{
// 			nSafetyX1Sen = ERR_NO_ERROR;
// 			nSafetyX2Sen = ERR_NO_ERROR;
// 		}

		if (nSafetyX1Sen != ERR_NO_ERROR)
			return ERR_MOT_INTERLOCK_TEST_PP_1_X1_POSITION_NOT_DETECTED_SAFETY_SENSOR + m_nTestPpIdx;

		if(nSafetyX2Sen != ERR_NO_ERROR)
			return ERR_MOT_INTERLOCK_TEST_PP_1_X2_POSITION_NOT_DETECTED_SAFETY_SENSOR + m_nTestPpIdx;

		//2. X축 Safety Position 확인.
		double dTestPP_X_SafetyPos = (m_nTestPpIdx == eTestPP_1) ? INTERLOCK_TESTPP_X_SAFETY_POS*(-1) : INTERLOCK_TESTPP_X_SAFETY_POS;

		//X1
		if (g_TaskTestPP[m_nTestPpIdx].CompareXData(dCurPosX1, dTestPP_X_SafetyPos) != TRUE) {
			return ERR_TEST_PP_1_X1_POSITION_NOT_SAFETY + m_nTestPpIdx;
		}

		//X2
		if (g_TaskTestPP[m_nTestPpIdx].CompareXData(dCurPosX2, dTestPP_X_SafetyPos) != TRUE) {
			return ERR_TEST_PP_1_X2_POSITION_NOT_SAFETY + m_nTestPpIdx;
		}
	}

	// Z축 안전 센서가 감지되지 않는다. ( B접점 )
	bool bSafetyZ1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ1)->IsHomeSensorOn();
	bool bSafetyZ2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ2)->IsHomeSensorOn();

	if (bSafetyZ1 != TRUE || bSafetyZ2 != TRUE)
		return (bSafetyZ1 != TRUE) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx;

	// 3. Z축이 down 안전 위치 아래 인 경우는 없다.
	if (vLocList[CTaskTestPP::eAxisZ1] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit)
		return ERR_TEST_PP_1_Z1_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;
	if (vLocList[CTaskTestPP::eAxisZ2] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit)
		return ERR_TEST_PP_1_Z2_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;


	// 3. X축 home 위치를 벗어난 경우.
	//if(fabs(dTPosHome[CTaskTestPP::eAxisY] - dCurPosY) >= 1000.0 /*|| pMotorY->GetHomeSensor() != DEF_ON */) 
	if (dTPosHome[CTaskTestPP::eAxisX1] + 1000.0 > dCurPosX1 || dTPosHome[CTaskTestPP::eAxisX2] + 1000.0 < dCurPosX2)
	{
		// Z축이 안전 위치가 아니다.
		if (vLocList[CTaskTestPP::eAxisZ1] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit || vLocList[CTaskTestPP::eAxisZ2] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit)
			return (vLocList[CTaskTestPP::eAxisZ1] != CTaskTestPP::ePosZ_DownSafetyToPosiLimit) ? ERR_TEST_PP_1_Z1_MOVE_POS_NOT_SAFETY + m_nTestPpIdx : ERR_TEST_PP_1_Z2_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;


		// Z축 안전 센서가 감지되지 않는다. ( B접점 )
		bool bSafetyZ1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ1)->IsHomeSensorOn();
		bool bSafetyZ2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ2)->IsHomeSensorOn();

		if (bSafetyZ1 != TRUE || bSafetyZ2 != TRUE)
			return (bSafetyZ1 != TRUE) ? ERR_TEST_PP_1_Z1_MOVE_POS_NOT_SAFETY + m_nTestPpIdx : ERR_TEST_PP_1_Z2_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;
	}

	return ERR_NO_ERROR;
}
int CMotorInterlock_TestPP::Interlock_y_jog( double dSpd, int nAcc, int nDcc )
{
	int nErr = ERR_NO_ERROR;

	nErr = Interlock_common();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	nErr = Interlock_PressUnit();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	// Z1축, Z2축 Home 센서가 감지되지 않는다.
	bool bSafetyZ1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ1)->IsHomeSensorOn();
	bool bSafetyZ2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ2)->IsHomeSensorOn();

	if (bSafetyZ1 != TRUE)
		return ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx;  

	if(bSafetyZ2 != TRUE)
		return ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx;

	// Z1축, Z2축 안전 위치 체크.
	std::vector<int> vLocList;
	vLocList.clear();
	g_TaskTestPP[m_nTestPpIdx].WhereIsMotorPos(m_nTestPpType, vLocList, CTaskTestPP::eAxisAll);

	if (vLocList[CTaskTestPP::eAxisZ1] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit || vLocList[CTaskTestPP::eAxisZ2] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit)
		return (vLocList[CTaskTestPP::eAxisZ1] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit) ? ERR_TEST_PP_1_Z1_MOVE_POS_NOT_SAFETY + m_nTestPpIdx : ERR_TEST_PP_1_Z2_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;

	// 1. X축 Safety Sensor 확인.(X1, X2)
	int nSafetx1Sen = g_TaskTestPP[m_nTestPpIdx].ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON);
	int nSafetx2Sen = g_TaskTestPP[m_nTestPpIdx].ChkDI(CTaskTestPP::xX2AXIS_POSITION_SEN, DEF_ON);

	if (nSafetx1Sen != ERR_NO_ERROR)
		return ERR_MOT_INTERLOCK_TEST_PP_1_X1_POSITION_NOT_DETECTED_SAFETY_SENSOR + m_nTestPpIdx;

	if (nSafetx2Sen != ERR_NO_ERROR)
		return ERR_MOT_INTERLOCK_TEST_PP_1_X2_POSITION_NOT_DETECTED_SAFETY_SENSOR + m_nTestPpIdx;

	//2. X축 Safety Position 확인.(X1, X2)
	OneAxis* pMotorX1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisX1);
	OneAxis* pMotorX2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisX2);
	double dCurPosX1 = 0.0, dCurPosX2 = 0.0;
	pMotorX1->GetCurrentPos(dCurPosX1);
	pMotorX2->GetCurrentPos(dCurPosX2);
	double dTestPP_X_SafetyPos = (m_nTestPpIdx == eTestPP_1) ? INTERLOCK_TESTPP_X_SAFETY_POS*(-1) : INTERLOCK_TESTPP_X_SAFETY_POS;

	//X1
	if (g_TaskTestPP[m_nTestPpIdx].CompareXData(dCurPosX1, dTestPP_X_SafetyPos) != TRUE) {
		return ERR_TEST_PP_1_X1_POSITION_NOT_SAFETY + m_nTestPpIdx;
	}
	//X2
	if (g_TaskTestPP[m_nTestPpIdx].CompareXData(dCurPosX2, dTestPP_X_SafetyPos) != TRUE) {
		return ERR_TEST_PP_1_X2_POSITION_NOT_SAFETY + m_nTestPpIdx;
	}

	return 0;
}
int CMotorInterlock_TestPP::Interlock_z_org()
{
	int nErr = ERR_NO_ERROR;

	nErr = Interlock_Orgcommon();
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}

	return ERR_NO_ERROR;
}
int CMotorInterlock_TestPP::Interlock_z_mov(double dTPos, double dSpd, int nAcc, int nDcc)
{
	int nTestHandXMotionDoneRetry = 0;
	
MotionDoneRetryGoto:
	int nErr = ERR_NO_ERROR;

	nErr = Interlock_common();
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}
	nErr = Interlock_PressUnit();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	std::vector<int> vLocList;
	vLocList.clear();
	g_TaskTestPP[m_nTestPpIdx].WhereIsMotorPos(m_nTestPpType, vLocList, CTaskTestPP::eAxisAll);


	int nXAxis = (m_nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2;

	OneAxis* pMotorX = g_TaskTestPP[m_nTestPpIdx].GetAxis(nXAxis);
	OneAxis* pMotorY = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisY);

	//Hand X, Y축 MotionDone 확인.
	if( pMotorX->MotionDone() != ERR_NO_ERROR)
	{
		// [4/17/2025 donghyun.shin]
		// Picker Z축 동작 중에, CTest PP X축 MotionDone이 되지 않아 Retry 5회 추가 goto문
		if (nTestHandXMotionDoneRetry < 5) 
		{
			nTestHandXMotionDoneRetry++;
			Sleep(10);
			goto MotionDoneRetryGoto;
		}
		
		//ASSERT(0);
		return (m_nTestPpType == eTestPPLoadType) ? ERR_TEST_PP_1_Z1_MOVE_NOT_MOTION_DONE_X + m_nTestPpIdx : ERR_TEST_PP_1_Z2_MOVE_NOT_MOTION_DONE_X + m_nTestPpIdx;
	}

	if (pMotorY->MotionDone() != ERR_NO_ERROR)
	{
		//ASSERT(0);
		return (m_nTestPpType == eTestPPLoadType) ? ERR_TEST_PP_1_Z1_MOVE_NOT_MOTION_DONE_Y + m_nTestPpIdx : ERR_TEST_PP_1_Z2_MOVE_NOT_MOTION_DONE_Y + m_nTestPpIdx;
	}

	//Test Hand Y축 Loading Table 영역이면, Load Table Y축 Motion Done 확인.

	double dCurPosX = 0.0;
	int nIndX = (m_nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2;
	g_TaskTestPP[m_nTestPpIdx].GetAxis(nIndX)->GetCurrentPos(dCurPosX);

	//변경 필요
	//X축 영역 확인.
	double dLImitX1Pos = (m_nTestPpIdx == eTestPP_1) ? INTERLOCK_TESTPP_X_POS_WITH_TESTPP_Z_AND_TBL_FRONT*(-1) : INTERLOCK_TESTPP_X_POS_WITH_TESTPP_Z_AND_TBL_FRONT;
	double dLimitX2Pos = (m_nTestPpIdx == eTestPP_1) ? INTERLOCK_TESTPP_X_POS_WITH_TESTPP_Z_AND_TBL_REAR*(-1) : INTERLOCK_TESTPP_X_POS_WITH_TESTPP_Z_AND_TBL_REAR;
	//double dLimitX2Pos = (m_nTestPpIdx == eTestPP_1) ? 81350.0*(-1) : 81350.0;
	
	//X축 Table Rear Position(Tbl2, Tbl3)
	BOOL bChkHandRearPos = FALSE;
	if (m_nTestPpIdx == eTestPP_1)
	{
		if (dCurPosX >= dLimitX2Pos) { bChkHandRearPos = TRUE; }
	}
	else
	{
		if (dCurPosX <= dLimitX2Pos) { bChkHandRearPos = TRUE; }
	}

	if (bChkHandRearPos) {
		int nIdxTable = (m_nTestPpIdx == eTestPP_1) ? eLoadTbl_2 : eLoadTbl_3;
		OneAxis* pMotorY = g_TaskLoadTbl[nIdxTable].GetAxis(CTaskLoadTable::eAxisY);
		if (pMotorY->MotionDone() != ERR_NO_ERROR)
		{
			return (m_nTestPpType == eTestPPLoadType) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_MOVE_NOT_MOTION_DONE_LOAD_TABLE_2 + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_MOVE_NOT_MOTION_DONE_LOAD_TABLE_2 + m_nTestPpIdx;
		}
	}

	//X축 Table Front Position(Tbl1, Tbl4)
	BOOL bChkHandFrontPos = FALSE;
	if (m_nTestPpIdx == eTestPP_1)
	{
		if (dLImitX1Pos < dCurPosX && dCurPosX < dLimitX2Pos) { bChkHandFrontPos = TRUE; }
	}
	else
	{
		if (dLimitX2Pos < dCurPosX && dCurPosX < dLImitX1Pos) { bChkHandFrontPos = TRUE; }
	}

	if (bChkHandFrontPos) {
		int nIdxTable = (m_nTestPpIdx == eTestPP_1) ? eLoadTbl_1 : eLoadTbl_4;
		OneAxis* pMotorY = g_TaskLoadTbl[nIdxTable].GetAxis(CTaskLoadTable::eAxisY);

		if (pMotorY->MotionDone() != ERR_NO_ERROR)
		{
			return (m_nTestPpType == eTestPPLoadType) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_MOVE_NOT_MOTION_DONE_LOAD_TABLE_1 + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_MOVE_NOT_MOTION_DONE_LOAD_TABLE_1 + m_nTestPpIdx;
		}
	}
	
	return ERR_NO_ERROR;
}

int CMotorInterlock_TestPP::Interlock_z_jog( double dSpd, int nAcc, int nDcc )
{
	int nErr = ERR_NO_ERROR;

	//nErr = Interlock_common();
	//if (nErr != ERR_NO_ERROR) {
	//	return nErr;
	//}
	nErr = Interlock_PressUnit();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotorX = g_TaskTestPP[m_nTestPpIdx].GetAxis((m_nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2);
	OneAxis* pMotorY = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisY);

	//Jog Z Move는 Origin만 Check 
	if (pMotorX->IsOrgCompleted(nullptr) != TRUE)
	{
		return (m_nTestPpType == eTestPPLoadType) ? ERR_MOT_INTERLOCK_TEST_PP_1_X1_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_X2_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}

	if (pMotorY->IsOrgCompleted(nullptr) != TRUE)
	{
		return ERR_MOT_INTERLOCK_TEST_PP_1_Y_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}

	return 0;
}

int CMotorInterlock_TestPP::Interlock_xpitch_org()
{
// 	int nErr = ERR_NO_ERROR;
// 
// 	nErr = Interlock_Orgcommon();
// 	if (nErr != ERR_NO_ERROR) {
// 		return nErr;
// 	}

	//Test Hand Z Origin Check(Z1 or Z2)
	OneAxis* pMotorZ = g_TaskTestPP[m_nTestPpIdx].GetAxis((m_nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2);

	if (pMotorZ->IsOrgCompleted(nullptr) != TRUE)
	{
		return (m_nTestPpType == eTestPPLoadType) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}

	//Z축 Safety Sensor 확인.
	bool bSafetZSen = 0;

	if (m_nTestPpType == eTestPPLoadType)
		bSafetZSen = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ1)->IsHomeSensorOn();
	else
		bSafetZSen = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ2)->IsHomeSensorOn();

	if (bSafetZSen != TRUE) {
		return (m_nTestPpType == eTestPPLoadType) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx;;
	}

	//Z축 Safety Pos 확인.
	std::vector<int> vLocList;
	vLocList.clear();
	g_TaskTestPP[m_nTestPpIdx].WhereIsMotorPos(m_nTestPpType, vLocList, CTaskTestPP::eAxisAll);

	if (m_nTestPpType == eTestPPLoadType) {
		if (vLocList[CTaskTestPP::eAxisZ1] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit) {
			return ERR_TEST_PP_1_Z1_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;
		}
	}
	else {
		if (vLocList[CTaskTestPP::eAxisZ2] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit) {
			return ERR_TEST_PP_1_Z2_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;
		}
	}

	return ERR_NO_ERROR;
}
int CMotorInterlock_TestPP::Interlock_xpitch_mov(double dTPos, double dSpd, int nAcc, int nDcc)
{
// 	int nErr = ERR_NO_ERROR;
// 
// 	nErr = Interlock_common();
// 	if (nErr != ERR_NO_ERROR) {
// 		return nErr;
// 	}

	//Test Hand Z Origin Check(Z1 or Z2)
	OneAxis* pMotorZ = g_TaskTestPP[m_nTestPpIdx].GetAxis((m_nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2);

	if (pMotorZ->IsOrgCompleted(nullptr) != TRUE)
	{
		return (m_nTestPpType == eTestPPLoadType) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}

	bool bSafetZSen = false;
	bool bSafetZPos = false;

	//Home sensor
	if (m_nTestPpType == eTestPPLoadType) 
		bSafetZSen = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ1)->IsHomeSensorOn();
	else
		bSafetZSen = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ2)->IsHomeSensorOn();

	if (bSafetZSen != true) {
		return (m_nTestPpType == eTestPPLoadType) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx;
	}

	//Safety Pos 확인.
	std::vector<int> vLocList;
	vLocList.clear();
	g_TaskTestPP[m_nTestPpIdx].WhereIsMotorPos(m_nTestPpType, vLocList, CTaskTestPP::eAxisAll);

	if (m_nTestPpType == eTestPPLoadType) {
		if (vLocList[CTaskTestPP::eAxisZ1] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit) {
			return ERR_TEST_PP_1_Z1_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;
		}
	}
	else {
		if (vLocList[CTaskTestPP::eAxisZ2] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit) {
			return ERR_TEST_PP_1_Z2_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;
		}
	}
	
	return ERR_NO_ERROR;
}
int CMotorInterlock_TestPP::Interlock_xpitch_jog(double dSpd, int nAcc, int nDcc)
{
// 	int nErr = ERR_NO_ERROR;
// 
// 	nErr = Interlock_common();
// 	if (nErr != ERR_NO_ERROR) {
// 		return nErr;
// 	}

	//Test Hand Z Origin Check(Z1 or Z2)
	OneAxis* pMotorZ = g_TaskTestPP[m_nTestPpIdx].GetAxis((m_nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2);

	if (pMotorZ->IsOrgCompleted(nullptr) != TRUE)
	{
		return (m_nTestPpType == eTestPPLoadType) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}

	//Z Safety Sensor 확인.
	bool bSafetZSen = 0;

	if (m_nTestPpType == eTestPPLoadType)
		bSafetZSen = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ1)->IsHomeSensorOn();
	else
		bSafetZSen = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ2)->IsHomeSensorOn();

	if (bSafetZSen != TRUE) {
		return (m_nTestPpType == eTestPPLoadType) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx;
	}


	//Safety Pos 확인.
	std::vector<int> vLocList;
	vLocList.clear();
	g_TaskTestPP[m_nTestPpIdx].WhereIsMotorPos(m_nTestPpType, vLocList, CTaskTestPP::eAxisAll);

	if (m_nTestPpType == eTestPPLoadType) {
		if (vLocList[CTaskTestPP::eAxisZ1] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit) {
			return ERR_TEST_PP_1_Z1_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;
		}
	}
	else {
		if (vLocList[CTaskTestPP::eAxisZ2] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit) {
			return ERR_TEST_PP_1_Z2_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;
		}
	}

	return ERR_NO_ERROR;
}
int CMotorInterlock_TestPP::Interlock_ypitch_org()
{
// 	int nErr = ERR_NO_ERROR;
// 
// 	nErr = Interlock_Orgcommon();
// 	if (nErr != ERR_NO_ERROR) {
// 		return nErr;
// 	}

	//Test Hand Z Origin Check(Z1 or Z2)
	OneAxis* pMotorZ = g_TaskTestPP[m_nTestPpIdx].GetAxis((m_nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2);

	if (pMotorZ->IsOrgCompleted(nullptr) != TRUE)
	{
		return (m_nTestPpType == eTestPPLoadType) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}

	//Z Safety Sensor 확인.
	bool bSafetZSen = 0;

	if (m_nTestPpType == eTestPPLoadType)
		bSafetZSen = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ1)->IsHomeSensorOn();
	else
		bSafetZSen = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ2)->IsHomeSensorOn();

	if (bSafetZSen != TRUE) {
		return (m_nTestPpType == eTestPPLoadType) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx;
	}


	//Safety Pos 확인.
	std::vector<int> vLocList;
	vLocList.clear();
	g_TaskTestPP[m_nTestPpIdx].WhereIsMotorPos(m_nTestPpType, vLocList, CTaskTestPP::eAxisAll);

	if (m_nTestPpType == eTestPPLoadType) {
		if (vLocList[CTaskTestPP::eAxisZ1] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit) {
			return ERR_TEST_PP_1_Z1_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;
		}
	}
	else {
		if (vLocList[CTaskTestPP::eAxisZ2] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit) {
			return ERR_TEST_PP_1_Z2_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;
		}
	}

	return ERR_NO_ERROR;
}

int CMotorInterlock_TestPP::Interlock_ypitch_mov(double dTPos, double dSpd, int nAcc, int nDcc)
{
// 	int nErr = ERR_NO_ERROR;
// 
// 	nErr = Interlock_common();
// 	if (nErr != ERR_NO_ERROR) {
// 		return nErr;
// 	}


	//Test Hand Z Origin Check(Z1 or Z2)
	OneAxis* pMotorZ = g_TaskTestPP[m_nTestPpIdx].GetAxis((m_nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2);

	if (pMotorZ->IsOrgCompleted(nullptr) != TRUE)
	{
		return (m_nTestPpType == eTestPPLoadType) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}

	//Z Safety Sensor 확인.
	bool bSafetZSen = 0;

	if (m_nTestPpType == eTestPPLoadType)
		bSafetZSen = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ1)->IsHomeSensorOn();
	else
		bSafetZSen = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ2)->IsHomeSensorOn();

	if (bSafetZSen != TRUE) {
		return (m_nTestPpType == eTestPPLoadType) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx;
	}

	//Safety Pos 확인.
	std::vector<int> vLocList;
	vLocList.clear();
	g_TaskTestPP[m_nTestPpIdx].WhereIsMotorPos(m_nTestPpType, vLocList, CTaskTestPP::eAxisAll);

	if (m_nTestPpType == eTestPPLoadType) {
		if (vLocList[CTaskTestPP::eAxisZ1] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit) {
			return ERR_TEST_PP_1_Z1_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;
		}
	}
	else {
		if (vLocList[CTaskTestPP::eAxisZ2] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit) {
			return ERR_TEST_PP_1_Z2_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;
		}
	}

	return ERR_NO_ERROR;
}

int CMotorInterlock_TestPP::Interlock_ypitch_jog(double dSpd, int nAcc, int nDcc)
{
// 	int nErr = ERR_NO_ERROR;
// 
// 	nErr = Interlock_common();
// 	if (nErr != ERR_NO_ERROR) {
// 		return nErr;
// 	}

	//Test Hand Z Origin Check(Z1 or Z2)
	OneAxis* pMotorZ = g_TaskTestPP[m_nTestPpIdx].GetAxis((m_nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2);

	if (pMotorZ->IsOrgCompleted(nullptr) != TRUE)
	{
		return (m_nTestPpType == eTestPPLoadType) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}

	//Z Safety Sensor 확인.
	bool bSafetZSen = 0;
	
	if (m_nTestPpType == eTestPPLoadType)
		bSafetZSen = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ1)->IsHomeSensorOn();
	else
		bSafetZSen = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ2)->IsHomeSensorOn();

	if (bSafetZSen != TRUE) {
		return (m_nTestPpType == eTestPPLoadType) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx;
	}


	//Safety Pos 확인.
	std::vector<int> vLocList;
	vLocList.clear();
	g_TaskTestPP[m_nTestPpIdx].WhereIsMotorPos(m_nTestPpType, vLocList, CTaskTestPP::eAxisAll);

	if (m_nTestPpType == eTestPPLoadType) {
		if (vLocList[CTaskTestPP::eAxisZ1] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit) {
			return ERR_TEST_PP_1_Z1_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;
		}
	}
	else {
		if (vLocList[CTaskTestPP::eAxisZ2] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit) {
			return ERR_TEST_PP_1_Z2_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;
		}
	}

	return ERR_NO_ERROR;
}



int CMotorInterlock_TestPP::Interlock_x_common()
{
	return ERR_NO_ERROR;
}
int CMotorInterlock_TestPP::Interlock_y_common()
{
	

	return ERR_NO_ERROR;
}
int CMotorInterlock_TestPP::Interlock_z_common()
{

	return ERR_NO_ERROR;
}

int CMotorInterlock_TestPP::Interlock_xpitch_common()
{
	OneAxis* pMotor = g_TaskTestPP[m_nTestPpIdx].GetAxis((m_nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisPitch_HW_X1_SW_Y1 : CTaskTestPP::eAxisPitch_HW_X2_SW_Y2);

	BOOL bServoOn = pMotor->GetServoStat();
	if (bServoOn != TRUE) {
		return (m_nTestPpType == eTestPPLoadType) ? ERR_MOT_INTERLOCK_TEST_PP1_X1_PITCH_SERVO_IS_NOT_ON + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP1_X2_PITCH_SERVO_IS_NOT_ON + m_nTestPpIdx;
	}

	return ERR_NO_ERROR;
}

int CMotorInterlock_TestPP::Interlock_ypitch_common()
{
	OneAxis* pMotor = g_TaskTestPP[m_nTestPpIdx].GetAxis((m_nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisPitch_HW_Y1_SW_X1 : CTaskTestPP::eAxisPitch_HW_Y2_SW_X2);

	BOOL bServoOn = pMotor->GetServoStat();
	if (bServoOn != TRUE) {
		return (m_nTestPpType == eTestPPLoadType) ? ERR_MOT_INTERLOCK_TEST_PP1_Y1_PITCH_SERVO_IS_NOT_ON + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP1_Y2_PITCH_SERVO_IS_NOT_ON + m_nTestPpIdx;
	}

	return ERR_NO_ERROR;
}


int CMotorInterlock_TestPP::Interlock_Orgcommon()
{
	//Press Origin Check
	int nErrRet = Interlock_PressUnit();
	if (nErrRet != ERR_NO_ERROR) {
		return nErrRet;
	}

	//Test Hand Z Origin Check(Z1, Z2)
	OneAxis* pMotorZ1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ1);	
	if (pMotorZ1->IsOrgCompleted(nullptr) != TRUE)
	{
		return ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx ;
	}

	OneAxis* pMotorZ2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ2);
	if (pMotorZ2->IsOrgCompleted(nullptr) != TRUE)
	{
		return ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}


// 	//Test Hand X축 Origin Check(X1, X2)
// 	OneAxis* pMotorX1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisX1);
// 	if (pMotorX1->IsOrgCompleted(nullptr) != TRUE)
// 	{
// 		return ERR_MOT_INTERLOCK_TEST_PP_1_X1_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
// 	}
// 
// 	OneAxis* pMotorX2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisX2);
// 	if (pMotorX2->IsOrgCompleted(nullptr) != TRUE)
// 	{
// 		return ERR_MOT_INTERLOCK_TEST_PP_1_X2_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
// 	}

	return ERR_NO_ERROR;
}

int CMotorInterlock_TestPP::Interlock_PressUnit()
{
	int nErrRet = 0;
	// Press Unit이 원점이 잡혀 있지 않으면 동작 할 수 없다.

	int nModuleIdx = eMaxATS_Area * m_nTestPpIdx;
	OneAxis* pPressMotorZ_F1 = g_TaskPressUnit[ePressUnit_Down_1 + nModuleIdx].GetAxis(CTaskPressUnit::eAxisZ);
	OneAxis* pPressMotorZ_F2 = g_TaskPressUnit[ePressUnit_Down_2 + nModuleIdx].GetAxis(CTaskPressUnit::eAxisZ);
	OneAxis* pPressMotorZ_F3 = g_TaskPressUnit[ePressUnit_Down_3 + nModuleIdx].GetAxis(CTaskPressUnit::eAxisZ);
	OneAxis* pPressMotorZ_F4 = g_TaskPressUnit[ePressUnit_Down_4 + nModuleIdx].GetAxis(CTaskPressUnit::eAxisZ);
	


	nErrRet = pPressMotorZ_F1->IsOrgCompleted(nullptr);
	if (nErrRet != TRUE) {
		return ERR_MOT_INTERLOCK_PRESS_1_Z_IS_NOT_ORIGIN + nModuleIdx;
	}
	nErrRet = pPressMotorZ_F2->IsOrgCompleted(nullptr);
	if (nErrRet != TRUE) {
		return ERR_MOT_INTERLOCK_PRESS_2_Z_IS_NOT_ORIGIN + nModuleIdx;
	}
	nErrRet = pPressMotorZ_F3->IsOrgCompleted(nullptr);
	if (nErrRet != TRUE) {
		return ERR_MOT_INTERLOCK_PRESS_3_Z_IS_NOT_ORIGIN + nModuleIdx;
	}
	nErrRet = pPressMotorZ_F4->IsOrgCompleted(nullptr);
	if (nErrRet != TRUE) {
		return ERR_MOT_INTERLOCK_PRESS_4_Z_IS_NOT_ORIGIN + nModuleIdx;
	}

	return ERR_NO_ERROR;
}

int CMotorInterlock_TestPP::Interlock_common()
{
	OneAxis* pMotorX1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisX1);
	OneAxis* pMotorX2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisX2);
	OneAxis* pMotorY = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisY);

	OneAxis* pMotorZ1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ1);
	OneAxis* pMotorZ2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ2);

	OneAxis* pMotorPitchX1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisPitch_HW_X1_SW_Y1);
	OneAxis* pMotorPitchX2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisPitch_HW_X2_SW_Y2);
	OneAxis* pMotorPitchY1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisPitch_HW_Y1_SW_X1);
	OneAxis* pMotorPitchY2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisPitch_HW_Y2_SW_X2);


	int nErr = ERR_NO_ERROR;

	if( pMotorY->IsMotorUsable(nErr) != TRUE){
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TEST_PP_1_Y_IS_AMP_FAULT + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Y_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}
	if(pMotorX1->IsMotorUsable(nErr) != TRUE){
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TEST_PP_1_X1_IS_AMP_FAULT + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_X1_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}
	if ( pMotorX2->IsMotorUsable(nErr) != TRUE) {
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TEST_PP_1_X2_IS_AMP_FAULT + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_X2_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}
	if(pMotorZ1->IsMotorUsable(nErr) != TRUE){
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_IS_AMP_FAULT + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}
	if (pMotorZ2->IsMotorUsable(nErr) != TRUE) {
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z2_IS_AMP_FAULT + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}

	if (pMotorPitchX1->IsMotorUsable(nErr) != TRUE) {
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TEST_PP_1_X1_PITCH_IS_AMP_FAULT + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_X1_PITCH_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}

	if (pMotorPitchX2->IsMotorUsable(nErr) != TRUE) {
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TEST_PP_1_X2_PITCH_IS_AMP_FAULT + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_X2_PITCH_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}

	if (pMotorPitchY1->IsMotorUsable(nErr) != TRUE) {
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TEST_PP_1_Y1_PITCH_IS_AMP_FAULT + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Y1_PITCH_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}

	if ( pMotorPitchY2->IsMotorUsable(nErr) != TRUE) {
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TEST_PP_1_Y2_PITCH_IS_AMP_FAULT + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Y2_PITCH_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}

	return ERR_NO_ERROR;
}

void CMotorInterlock_TestPP::Interlock_Y_Range_Related_Press_Z_Safety(double dTPos, int nHandType, std::vector<int> &nRelatedStationNo)
{
	BOOL bChkStationArea = FALSE;
	if (m_nTestPpIdx == eTestPP_1)
	{
		if (dTPos < INTERLOCK_TESTPP_1_X_STATION_START_POS) { bChkStationArea = TRUE; }
	}
	else
	{
		if (dTPos > INTERLOCK_TESTPP_2_X_STATION_START_POS) { bChkStationArea = TRUE; }
	}

	if (!bChkStationArea) { return; }

	int nRetStationNum = -1;
	double dCurYPos = 0.0;
	OneAxis* pMotorY = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisY);
	pMotorY->GetCurrentPos(dCurYPos);

	double dStationGap = INTERLOCK_STATION_GAP;
	//double dHandLength = INTERLOCK_HAND_LENGTH;
	double dHandLength = 0.0;

	double dStartPos1 = (nHandType == eTestPPLoadType) ? INTERLOCK_TESTPP_Y_STATION_1_START_POS_LOAD + dHandLength : INTERLOCK_TESTPP_Y_STATION_1_START_POS_UNLOAD + dHandLength;
	double dEndPos1 = (nHandType == eTestPPLoadType) ? INTERLOCK_TESTPP_Y_STATION_1_END_POS_LOAD - dHandLength : INTERLOCK_TESTPP_Y_STATION_1_END_POS_UNLOAD - dHandLength;

	double dStartPos2 = (nHandType == eTestPPLoadType) ? INTERLOCK_TESTPP_Y_STATION_2_START_POS_LOAD + dHandLength : INTERLOCK_TESTPP_Y_STATION_2_START_POS_UNLOAD + dHandLength;
	double dEndPos2 = (nHandType == eTestPPLoadType) ? INTERLOCK_TESTPP_Y_STATION_2_END_POS_LOAD - dHandLength : INTERLOCK_TESTPP_Y_STATION_2_END_POS_UNLOAD - dHandLength;

	double dStartPos3 = (nHandType == eTestPPLoadType) ? INTERLOCK_TESTPP_Y_STATION_3_START_POS_LOAD + dHandLength : INTERLOCK_TESTPP_Y_STATION_3_START_POS_UNLOAD + dHandLength;
	double dEndPos3 = (nHandType == eTestPPLoadType) ? INTERLOCK_TESTPP_Y_STATION_3_END_POS_LOAD - dHandLength : INTERLOCK_TESTPP_Y_STATION_3_END_POS_UNLOAD - dHandLength;

	double dStartPos4 = (nHandType == eTestPPLoadType) ? INTERLOCK_TESTPP_Y_STATION_4_START_POS_LOAD + dHandLength : INTERLOCK_TESTPP_Y_STATION_4_START_POS_UNLOAD + dHandLength;
	double dEndPos4 = (nHandType == eTestPPLoadType) ? INTERLOCK_TESTPP_Y_STATION_4_END_POS_LOAD - dHandLength : INTERLOCK_TESTPP_Y_STATION_4_END_POS_UNLOAD - dHandLength;



	if (dStartPos1 <= dCurYPos && dCurYPos <= dEndPos1) {
		nRelatedStationNo.push_back(LOC_IDX_STATION_1);
		//nRetStationNum = LOC_IDX_STATION_1;
	}
	if (dStartPos2 <= dCurYPos && dCurYPos <= dEndPos2) {
		nRelatedStationNo.push_back(LOC_IDX_STATION_2);
		//nRetStationNum = LOC_IDX_STATION_2;
	}
	if (dStartPos3 <= dCurYPos && dCurYPos <= dEndPos3) {
		nRelatedStationNo.push_back(LOC_IDX_STATION_3);
		//nRetStationNum = LOC_IDX_STATION_3;
	}
	if (dStartPos4 <= dCurYPos && dCurYPos <= dEndPos4) {
		nRelatedStationNo.push_back(LOC_IDX_STATION_4);
		//nRetStationNum = LOC_IDX_STATION_4;
	}

	//return nRetStationNum;
}

/*
BOOL CMotorInterlock_TestPP::CompareXData(int nHandIdx, double dData1, double dData2)
{
//dData1 : Target 위치, 현재 위치
//dData2 : 절대 위치
	if (nHandIdx == eTestPP_1) {
		return (dData1 > dData2) ? TRUE : FALSE;
	}
	else {
		return (dData1 < dData2) ? TRUE : FALSE;
	}
}
*/