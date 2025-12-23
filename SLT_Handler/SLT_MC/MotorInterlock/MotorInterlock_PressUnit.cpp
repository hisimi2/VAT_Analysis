#include "StdAfx.h"
#include "MotorInterlock_PressUnit.h"


CMotorInterlock_PressUnit::CMotorInterlock_PressUnit(int nPressIdx,int nTestPpIdx)
	:m_nPressIdx(nPressIdx) , m_nTestPpIdx(nTestPpIdx)
{

}


CMotorInterlock_PressUnit::~CMotorInterlock_PressUnit(void)
{
}

int CMotorInterlock_PressUnit::Interlock_z_org()
{
	int nErr = CBaseMotorInterlock::CommonInterlock();
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}

	return ERR_NO_ERROR;
}

int CMotorInterlock_PressUnit::Interlock_z_mov(double dTPos, double dSpd, int nAcc, int nDcc)
{
//	int nErr = ERR_NO_ERROR;
	if (dTPos <= INTERLOCK_PRESS_Z_SAFETY_POS)
	{
		if (m_nPressIdx == ePressUnit_Down_1 || m_nPressIdx == ePressUnit_Down_5)
		{
			int nTestPpIdx = m_nPressIdx == ePressUnit_Down_1 ? eTestPP_1 : eTestPP_2;
			int nErr = g_TaskTestPP[nTestPpIdx].Interlock_X_Related_Clean_Table_Range();
			if (nErr != ERR_NO_ERROR) {
				return nErr;
			}
		}

		return ERR_NO_ERROR;
	}
	// TODO KSJ :: 안전 영역에 대한 위치 확인이 필요함.
// 	double dTeachPosZ = 0.0;
// 	g_TaskPressUnit[m_nPressIdx].GetTeachPos(LOC_IDX_PRESS_UNIT_SAFETY, &dTeachPosZ);
// 	if(dTPos > INTERLOCK_PRESS_Z_SAFETY_POS){ //dTeachPosZ + 100.0
// 
// 		nErr = Interlock_Press_common();
// 		if (nErr != ERR_NO_ERROR) {
// 			return nErr;
// 		}

//		int nAreaIdx = ChangePressIdxToAreaIdx();

// 		//Y축 위치 확인을 위함.
// 		std::vector<int> vLocList;		vLocList.clear();
// 		std::vector<int> vLocUnList;	vLocUnList.clear();
// 
// 		g_TaskTestPP[m_nTestPpIdx].WhereIsMotorPos(eTestPPLoadType, vLocList, CTaskTestPP::eAxisAll);
// 		g_TaskTestPP[m_nTestPpIdx].WhereIsMotorPos(eTestPPUnLoadType, vLocUnList, CTaskTestPP::eAxisAll);
// 
// 		//TODO KSJ :: X축 Sensor 점검하여 Press 이동을 할 수 있다.
// 		int nSafetyX1Sen = g_TaskTestPP[m_nTestPpIdx].ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON);
// 		int nSafetyX2Sen = g_TaskTestPP[m_nTestPpIdx].ChkDI(CTaskTestPP::xX2AXIS_POSITION_SEN, DEF_ON);
// 
// 		//X1과 X2가 모두 Safety Senesor가 감지 되어야 Press 동작할 수 있음.
// 		//X축 LocList는 Load, Unload Type 동일.
// 		if ((vLocList[CTaskTestPP::eAxisX1] < CTaskTestPP::ePosX_TableToNegaLimit) || nSafetyX1Sen != ERR_NO_ERROR ||
// 			(vLocUnList[CTaskTestPP::eAxisX2] < CTaskTestPP::ePosX_TableToNegaLimit) || nSafetyX2Sen != ERR_NO_ERROR)
// 		{
// 			if (vLocList[CTaskTestPP::eAxisY] == CTaskTestPP::ePosY_StationL1 + nAreaIdx)
// 			{
// 				return ERR_TEST_PP_1_LOAD_Y_POSITION_NOT_SAFETY + m_nTestPpIdx;
// 			}
// 
// 			if (vLocUnList[CTaskTestPP::eAxisY] == CTaskTestPP::ePosY_StationL1 + nAreaIdx)
// 			{
// 				return ERR_TEST_PP_1_UNLOAD_Y_POSITION_NOT_SAFETY + m_nTestPpIdx;
// 			}
// 		}
		int nErr = ERR_NO_ERROR;

		if (dTPos > INTERLOCK_PRESS_Z_SAFETY_POS) { //dTeachPosZ + 100.0
			nErr = Interlock_Press_common();
			if (nErr != ERR_NO_ERROR) {
				return nErr;
			}
	
		int nAreaIdx = ChangePressIdxToAreaIdx();
		int nSafetyPos = (m_nTestPpIdx == eTestPP_1) ? INTERLOCK_TESTPP_X_SAFETY_POS*(-1) : INTERLOCK_TESTPP_X_SAFETY_POS;

		BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();

		////1. Load Hand 확인.
		int nSafetyX1Sen = g_TaskTestPP[m_nTestPpIdx].ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON);
// 		if (bAutoGrrUse == TRUE) {
// 			nSafetyX1Sen == ERR_NO_ERROR;
// 		}

		double dCurPosX1 = 0.0;
		OneAxis* pMotorX1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisX1);
		pMotorX1->GetCurrentPos(dCurPosX1);
		
		//Y축이 Press 간섭 영역에 있는지 확인.
		BOOL bInPosition_LD = Interlock_Chk_Testpp_Y_In_Press_Area(nAreaIdx, m_nTestPpIdx, eTestPPLoadType);


		////////////////////
		/*
		Clean 처리
		bInPosition_LD이고 
		m_nPressIdx가 0 이면

		X1 Safety Position만 확인 하도록(Def 하나 만들어서) && Teaching Position 보다 안쪽에 있으면 불러 와서 같이 확인.


		*/
		////////////////////

		
		if (m_nPressIdx != ePressUnit_Down_1 && m_nPressIdx != ePressUnit_Down_5)
		{
			//X1 Safety Sensor 확인.
			if (bInPosition_LD == TRUE && nSafetyX1Sen != ERR_NO_ERROR)
			{
				return ERR_TEST_PP_LOAD_X_NOT_DETECT_SAFETY_SENSOR_CHECK_Y_NOT_IN_POS_PRESS_1 + m_nPressIdx;
			}

			//X1 Safety Position 확인.
			if (bInPosition_LD == TRUE && g_TaskTestPP[m_nTestPpIdx].CompareXData(dCurPosX1, nSafetyPos) != TRUE)
			{
				return ERR_TEST_PP_LOAD_X_NOT_DETECT_SAFETY_POS_CHECK_Y_NOT_IN_POS_PRESS_1 + m_nPressIdx;
			}
		}
		else //m_nPressIdx == ePressUnit_Down_1 || m_nPressIdx == ePressUnit_Down_5
		{
			//Clean Table Limit 절댓값 확인.
			nSafetyPos = (m_nTestPpIdx == eTestPP_1) ? INTERLOCK_TESTPP1_X_CLEAN_TABLE_LIMIT : INTERLOCK_TESTPP2_X_CLEAN_TABLE_LIMIT;
			if (bInPosition_LD == TRUE && g_TaskTestPP[m_nTestPpIdx].CompareXData(dCurPosX1, nSafetyPos) != TRUE)
			{
				return ERR_TEST_PP_LOAD_X_NOT_DETECT_SAFETY_POS_CHECK_Y_NOT_IN_POS_PRESS_1 + m_nPressIdx;
			}


			//Teaching 값 비교
			double dTPos[CTaskTestPP::eAxisMax] = { 0, };
			std::vector<CPoint> vTempPkr;
			vTempPkr.push_back(CPoint(0, 0));
			g_TaskTestPP[m_nTestPpIdx].GetTeachPos(LOC_IDX_CLEAN_TBL, CPoint(0, 0), vTempPkr, CTaskTestPP::eZPOS_SAFETY, dTPos, eTestPPLoadType);

			double dLimitPos = (m_nTestPpIdx == eTestPP_1) ? dTPos[CTaskTestPP::eAxisX1] - 1000.0 : dTPos[CTaskTestPP::eAxisX1] + 1000.0;
			if (bInPosition_LD == TRUE && g_TaskTestPP[m_nTestPpIdx].CompareXData(dCurPosX1, dLimitPos) != TRUE)
			{
				return ERR_TEST_PP_LOAD_X_NOT_DETECT_SAFETY_POS_CHECK_Y_NOT_IN_POS_PRESS_1 + m_nPressIdx;
			}
		}
		
		////2. Unload Hand 확인.
		int nSafetyX2Sen = g_TaskTestPP[m_nTestPpIdx].ChkDI(CTaskTestPP::xX2AXIS_POSITION_SEN, DEF_ON);
		// 		if (bAutoGrrUse == TRUE) {
		// 			nSafetyX1Sen == ERR_NO_ERROR;
		// 		}

		double dCurPosX2 = 0.0;
		OneAxis* pMotorX2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisX2);
		pMotorX2->GetCurrentPos(dCurPosX2);

		//Y축이 Press 간섭 영역에 있는지 확인.
		BOOL bInPosition_UD = Interlock_Chk_Testpp_Y_In_Press_Area(nAreaIdx, m_nTestPpIdx, eTestPPUnLoadType);

		//X1 Safety Sensor 확인.
		if (bInPosition_UD == TRUE && nSafetyX2Sen != ERR_NO_ERROR)
		{
			return ERR_TEST_PP_UNLOAD_X_NOT_DETECT_SAFETY_SENSOR_CHECK_Y_NOT_IN_POS_PRESS_1 + m_nPressIdx;
		}

		//X1 Safety Position 확인.
		if (bInPosition_UD == TRUE && g_TaskTestPP[m_nTestPpIdx].CompareXData(dCurPosX2, nSafetyPos) != TRUE)
		{
			return ERR_TEST_PP_UNLOAD_X_NOT_DETECT_SAFETY_POS_CHECK_Y_NOT_IN_POS_PRESS_1 + m_nPressIdx;
		}
		
	}
// 	else {
// 		OneAxis* pPressMotor = g_TaskPressUnit[m_nPressIdx].GetAxis(CTaskPressUnit::eAxisZ);
// 		int nErrRet = pPressMotor->ChkMotorUsable();
// 		if (nErrRet != ERR_NO_ERROR) {
// 			return nErrRet;
// 		}
// 	}
	

	//------------------------------------------------- 이동하고자 하는 축의 상태 점검.
	// 속도가 한계치 이상이어서는 안된다.

	// 장비가 Run 상태일때 점검해야 할 인터락
	if( g_DMCont.m_dmEQP.GB(BDM0_EQP_RUN_STATE) )
	{

	}

	return ERR_NO_ERROR;
}

int CMotorInterlock_PressUnit::Interlock_z_jog( double dSpd, int nAcc, int nDcc )
{
// 	int nErr = ERR_NO_ERROR;
// 
// 	// down 방향에 대한 interlock
// 	if ( dSpd > 0 )
// 	{
// 		nErr = Interlock_Press_common();
// 		if (nErr != ERR_NO_ERROR) {
// 			return nErr;
// 		}
// 
// 		int nAreaIdx = ChangePressIdxToAreaIdx();
// 		std::vector<int> vLocList;		vLocList.clear();
// 		std::vector<int> vLocUnList;	vLocUnList.clear();
// 		
// 		g_TaskTestPP[m_nTestPpIdx].WhereIsMotorPos(eTestPPLoadType,vLocList, CTaskTestPP::eAxisAll);
// 		g_TaskTestPP[m_nTestPpIdx].WhereIsMotorPos(eTestPPUnLoadType, vLocUnList, CTaskTestPP::eAxisAll);
// 
// 		//TODO KSJ :: X축 Sensor 점검하여 Press 이동을 할 수 있다.
// 		int nSafetyX1Sen = g_TaskTestPP[m_nTestPpIdx].ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON);
// 		int nSafetyX2Sen = g_TaskTestPP[m_nTestPpIdx].ChkDI(CTaskTestPP::xX2AXIS_POSITION_SEN, DEF_ON);
// 		if ((vLocList[CTaskTestPP::eAxisX1] < CTaskTestPP::ePosX_TableToNegaLimit) || nSafetyX1Sen != ERR_NO_ERROR ||
// 		(vLocUnList[CTaskTestPP::eAxisX2] < CTaskTestPP::ePosX_TableToNegaLimit) || nSafetyX2Sen != ERR_NO_ERROR)
// 		{
// 			if (vLocList[CTaskTestPP::eAxisY] == CTaskTestPP::ePosY_StationL1 + nAreaIdx)
// 			{
// 				return ERR_TEST_PP_1_LOAD_Y_POSITION_NOT_SAFETY + m_nTestPpIdx;
// 			}
// 
// 			if (vLocUnList[CTaskTestPP::eAxisY] == CTaskTestPP::ePosY_StationL1 + nAreaIdx)
// 			{
// 				return ERR_TEST_PP_1_UNLOAD_Y_POSITION_NOT_SAFETY + m_nTestPpIdx;
// 			}
// 		}
// 		
// 		
// 	}
// 	else {
// 		OneAxis* pPressMotor = g_TaskPressUnit[m_nPressIdx].GetAxis(CTaskPressUnit::eAxisZ);
// 		int nErrRet = pPressMotor->ChkMotorUsable();
// 		if (nErrRet != ERR_NO_ERROR) {
// 			return nErrRet;
// 		}
// 	}
	int nErr = ERR_NO_ERROR;

	if (dSpd > 0) { //dTeachPosZ + 100.0
		nErr = Interlock_Press_common();
		if (nErr != ERR_NO_ERROR) {
			return nErr;
		}

		int nAreaIdx = ChangePressIdxToAreaIdx();
		int nSafetyPos = (m_nTestPpIdx == eTestPP_1) ? INTERLOCK_TESTPP_X_SAFETY_POS*(-1) : INTERLOCK_TESTPP_X_SAFETY_POS;

		////1. Load Hand 확인.
		int nSafetyX1Sen = g_TaskTestPP[m_nTestPpIdx].ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON);
		double dCurPosX1 = 0.0;
		OneAxis* pMotorX1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisX1);
		pMotorX1->GetCurrentPos(dCurPosX1);

		//Y축이 Press 간섭 영역에 있는지 확인.
		BOOL bInPosition_LD = Interlock_Chk_Testpp_Y_In_Press_Area(nAreaIdx, m_nTestPpIdx, eTestPPLoadType);

		//X1 Safety Sensor 확인.
		if (bInPosition_LD == TRUE && nSafetyX1Sen != ERR_NO_ERROR)
		{
			return ERR_TEST_PP_LOAD_X_NOT_DETECT_SAFETY_SENSOR_CHECK_Y_NOT_IN_POS_PRESS_1 + m_nPressIdx;
		}

		//X1 Safety Position 확인.
		if (bInPosition_LD == TRUE && g_TaskTestPP[m_nTestPpIdx].CompareXData(dCurPosX1, nSafetyPos) != TRUE)
		{
			return ERR_TEST_PP_LOAD_X_NOT_DETECT_SAFETY_POS_CHECK_Y_NOT_IN_POS_PRESS_1 + m_nPressIdx;
		}


		////2. Unload Hand 확인.
		int nSafetyX2Sen = g_TaskTestPP[m_nTestPpIdx].ChkDI(CTaskTestPP::xX2AXIS_POSITION_SEN, DEF_ON);
		double dCurPosX2 = 0.0;
		OneAxis* pMotorX2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisX2);
		pMotorX2->GetCurrentPos(dCurPosX2);

		//Y축이 Press 간섭 영역에 있는지 확인.
		BOOL bInPosition_UD = Interlock_Chk_Testpp_Y_In_Press_Area(nAreaIdx, m_nTestPpIdx, eTestPPUnLoadType);

		//X1 Safety Sensor 확인.
		if (bInPosition_UD == TRUE && nSafetyX2Sen != ERR_NO_ERROR)
		{
			return ERR_TEST_PP_UNLOAD_X_NOT_DETECT_SAFETY_SENSOR_CHECK_Y_NOT_IN_POS_PRESS_1 + m_nPressIdx;
		}

		//X1 Safety Position 확인.
		if (bInPosition_UD == TRUE && g_TaskTestPP[m_nTestPpIdx].CompareXData(dCurPosX2, nSafetyPos) != TRUE)
		{
			return ERR_TEST_PP_UNLOAD_X_NOT_DETECT_SAFETY_POS_CHECK_Y_NOT_IN_POS_PRESS_1 + m_nPressIdx;
		}

	}
	return 0;
}

int CMotorInterlock_PressUnit::ChangePressIdxToAreaIdx()	
{
	ASSERT(m_nPressIdx >= ePressUnit_Down_1 && m_nPressIdx <= ePressUnit_Down_8);

	int nAreaIdx = 0;
	switch( m_nPressIdx )	
	{
	case ePressUnit_Down_1:  case ePressUnit_Down_5: { nAreaIdx = eATS_DN_STATION_L1; }break;
	case ePressUnit_Down_2:  case ePressUnit_Down_6: { nAreaIdx = eATS_DN_STATION_L2; }break;
	case ePressUnit_Down_3:  case ePressUnit_Down_7: { nAreaIdx = eATS_DN_STATION_L3; }break;
	case ePressUnit_Down_4:  case ePressUnit_Down_8: { nAreaIdx = eATS_DN_STATION_L4; }break;
		
// 	case ePressUnit_Up_1:    case ePressUnit_Up_5:   { nAreaIdx = eATS_UP_STATION_L1; }break;
// 	case ePressUnit_Up_2:    case ePressUnit_Up_6:   { nAreaIdx = eATS_UP_STATION_L2; }break;
// 	case ePressUnit_Up_3:    case ePressUnit_Up_7:   { nAreaIdx = eATS_UP_STATION_L3; }break;
// 	case ePressUnit_Up_4:    case ePressUnit_Up_8:   { nAreaIdx = eATS_UP_STATION_L4; }break;
	}

	return nAreaIdx;
}


int CMotorInterlock_PressUnit::Interlock_Press_common()
{
	
	OneAxis* pMotorX1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisX1);
	OneAxis* pMotorX2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisX2);
	OneAxis* pMotorY = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisY);
	OneAxis* pMotorZ1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ1);
	OneAxis* pMotorZ2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ2);

	int nErr = ERR_NO_ERROR;

	if (pMotorX1->IsMotorUsable(nErr) != TRUE) {
		return (nErr == -1 ) ? ERR_MOT_INTERLOCK_TEST_PP_1_X1_IS_AMP_FAULT + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_X1_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}

	if (pMotorX2->IsMotorUsable(nErr) != TRUE) {
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TEST_PP_1_X2_IS_AMP_FAULT + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_X2_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}

	if (pMotorY->IsMotorUsable(nErr) != TRUE) {
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TEST_PP_1_Y_IS_AMP_FAULT + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Y_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}

	if (pMotorZ1->IsMotorUsable(nErr) != TRUE) {
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_IS_AMP_FAULT + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}
	
	if (pMotorZ2->IsMotorUsable(nErr) != TRUE) {
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z2_IS_AMP_FAULT + m_nTestPpIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_ORIGIN + m_nTestPpIdx;
	}

// 	OneAxis* pPressMotor = g_TaskPressUnit[m_nPressIdx].GetAxis(CTaskPressUnit::eAxisZ);
// 	int nErrRet = pPressMotor->ChkMotorUsable();
// 	if (nErrRet != ERR_NO_ERROR) {
// 		return nErrRet;
// 	}

	return ERR_NO_ERROR;
}

int CMotorInterlock_PressUnit::ChangeTestPPIdxToAreaIdx(std::vector<int>& vTestPPAreaStartEnd)
{	
	ASSERT(m_nPressIdx >= ePressUnit_Down_1 && m_nPressIdx <= ePressUnit_Down_8);

	int nAreaIdx = 0;
	switch (m_nPressIdx)
	{
		case ePressUnit_Down_1:
		case ePressUnit_Down_2:
		case ePressUnit_Down_3:
		case ePressUnit_Down_4:
		{
			vTestPPAreaStartEnd.push_back(eTestPP_1);
			
		}break;
		case ePressUnit_Down_5:
		case ePressUnit_Down_6:
		case ePressUnit_Down_7:
		case ePressUnit_Down_8:
		{
			vTestPPAreaStartEnd.push_back(eTestPP_2);
		}break;
	}

	return nAreaIdx;
}

BOOL CMotorInterlock_PressUnit::Interlock_Chk_Testpp_Y_In_Press_Area(int nPressIdx, int nTestPPIdx, int nTestPPType)
{
	BOOL bInPosition = FALSE;

	double dCurYPos = 0.0;
	OneAxis* pMotorY = g_TaskTestPP[nTestPPIdx].GetAxis(CTaskTestPP::eAxisY);
	pMotorY->GetCurrentPos(dCurYPos);

	switch (nPressIdx)
	{
	case eATS_DN_STATION_L1:
	{
		//Test Load Hand Y Station 영역 확인.
		if (nTestPPType == eTestPPLoadType) {
			bInPosition = INPOSITION(dCurYPos, INTERLOCK_TESTPP_Y_STATION_1_START_POS_LOAD, INTERLOCK_TESTPP_Y_STATION_1_END_POS_LOAD);
		}
		//Test Unload Hand Y Station 영역 확인.
		else {
			bInPosition = INPOSITION(dCurYPos, INTERLOCK_TESTPP_Y_STATION_1_START_POS_UNLOAD, INTERLOCK_TESTPP_Y_STATION_1_END_POS_UNLOAD);
		}
	}break;
	case eATS_DN_STATION_L2:
	{
		//Test Load Hand Y Station 영역 확인.
		if (nTestPPType == eTestPPLoadType) {
			bInPosition = INPOSITION(dCurYPos, INTERLOCK_TESTPP_Y_STATION_2_START_POS_LOAD, INTERLOCK_TESTPP_Y_STATION_2_END_POS_LOAD);
		}
		//Test Unload Hand Y Station 영역 확인.
		else {
			bInPosition = INPOSITION(dCurYPos, INTERLOCK_TESTPP_Y_STATION_2_START_POS_UNLOAD, INTERLOCK_TESTPP_Y_STATION_2_END_POS_UNLOAD);
		}
	}break;
	case eATS_DN_STATION_L3:
	{
		//Test Load Hand Y Station 영역 확인.
		if (nTestPPType == eTestPPLoadType) {
			bInPosition = INPOSITION(dCurYPos, INTERLOCK_TESTPP_Y_STATION_3_START_POS_LOAD, INTERLOCK_TESTPP_Y_STATION_3_END_POS_LOAD);
		}
		//Test Unload Hand Y Station 영역 확인.
		else {
			bInPosition = INPOSITION(dCurYPos, INTERLOCK_TESTPP_Y_STATION_3_START_POS_UNLOAD, INTERLOCK_TESTPP_Y_STATION_3_END_POS_UNLOAD);
		}
	}break;
	case eATS_DN_STATION_L4:
	{
		//Test Load Hand Y Station 영역 확인.
		if (nTestPPType == eTestPPLoadType) {
			bInPosition = INPOSITION(dCurYPos, INTERLOCK_TESTPP_Y_STATION_4_START_POS_LOAD, INTERLOCK_TESTPP_Y_STATION_4_END_POS_LOAD);
		}
		//Test Unload Hand Y Station 영역 확인.
		else {
			bInPosition = INPOSITION(dCurYPos, INTERLOCK_TESTPP_Y_STATION_4_START_POS_UNLOAD, INTERLOCK_TESTPP_Y_STATION_4_END_POS_UNLOAD);
		}
	}break;
	default:
		break;
	}

	return bInPosition;
}