#include "StdAfx.h"
#include "MotorInterLock_LoadTbl.h"


CMotorInterLock_LoadTbl::CMotorInterLock_LoadTbl(int nLoadTblIdx, int nTestPpIdx)
	: m_nLoadTblIdx(nLoadTblIdx),m_nTestPpIdx(nTestPpIdx)
{

}


CMotorInterLock_LoadTbl::~CMotorInterLock_LoadTbl(void)
{
}

// int CMotorInterLock_LoadTbl::Interlock_x_org()
// {
// 	int nErr = CBaseMotorInterlock::CommonInterlock();
// 	if( nErr != ERR_NO_ERROR ){
// 		return nErr;
// 	}
// 
// 	nErr = Interlock_common();
// 	if( nErr != ERR_NO_ERROR ){
// 		return nErr;
// 	}
// 
// 	return ERR_NO_ERROR;
// }
// int CMotorInterLock_LoadTbl::Interlock_x_mov(double dTPos, double dSpd, int nAcc, int nDcc)
// {
// 	int nErr = CBaseMotorInterlock::CommonInterlock();
// 	if( nErr != ERR_NO_ERROR ){
// 		return nErr;
// 	}
// 
// 	nErr = Interlock_common();
// 	if( nErr != ERR_NO_ERROR ){
// 		return nErr;
// 	}
// 
// 	std::vector<int> vLocList;
// 	g_TaskTestPP[m_nTestPpIdx].WhereIsMotorPos(eTestPPLoadType, vLocList, CTaskTestPP::eAxisAll);
// 
// 	OneAxis* pMotorZ = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ1);
// 	double dCurPos_TestPP_Z = pMotorZ->GetCurrentPos();
// 
// 	double dPos[ CTaskTestPP::eAxisMax ] = {0.0,};
// 
// 	std::vector<CPoint> vPicker;
// 	vPicker.push_back( CPoint(0,0) );
// 
// 	g_TaskTestPP[m_nTestPpIdx].GetTeachPos(LOC_IDX_TEST_PP_TBL_1, CPoint(0,0), vPicker, CTaskTestPP::eZPOS_SAFETY, dPos, eAxis_TestPP_X1);
// 
// 	// Load Table or Home 영역에 x가 존재할 경우
// 	if(vLocList[CTaskTestPP::eAxisX1] == CTaskTestPP::ePosX_TableToNegaLimit || vLocList[CTaskTestPP::eAxisX2] == CTaskTestPP::ePosX_TableToNegaLimit)
// 	{
// 		if(dCurPos_TestPP_Z > dPos[CTaskTestPP::eAxisZ1] + 1000.0 || dCurPos_TestPP_Z > dPos[CTaskTestPP::eAxisZ2] + 1000.0)
// 		{
// 			if(g_TaskTestPP[m_nTestPpIdx].ChkPicker_All(DEF_UP) != ERR_NO_ERROR){
// 				return ERR_TEST_PP_1_PICKER_SEN + m_nLoadTblIdx;
// 			}			
// 			return ERR_MOT_INTERLOCK_TEST_PP_1_Z_IS_NOT_SAFETY + m_nLoadTblIdx;
// 		}
// 	}
// 
// 	return ERR_NO_ERROR;
// }

int CMotorInterLock_LoadTbl::Interlock_y_org()
{
	int nErr = ERR_NO_ERROR;

	nErr = Interlock_common();
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}

	//Tray pp Z Home Sensor 확인.
	bool bSafetySenZ_tray_load = g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisZ)->IsHomeSensorOn();
	bool bSafetySenZ_tray_unload = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisZ)->IsHomeSensorOn();
	
	if (bSafetySenZ_tray_load != true || bSafetySenZ_tray_unload != true) {
		return (bSafetySenZ_tray_load != true) ? ERR_MOT_INTERLOCK_TRAY_PP_1_Z_AXIS_IS_NOT_DETECTED_HOME_SENSOR : ERR_MOT_INTERLOCK_TRAY_PP_2_Z_AXIS_IS_NOT_DETECTED_HOME_SENSOR;
	}

	//Tray pp Z Safety Pos 확인.
	bool bSafetyPosZ_tray_load = g_TaskTrayLoadPP.WhereIsMotorPosZ();
	bool bSafetyPosZ_tray_unload = g_TaskTrayUnloadPP.WhereIsMotorPosZ();

	if (bSafetyPosZ_tray_load != true || bSafetyPosZ_tray_unload != true) {
		return (bSafetyPosZ_tray_load != true) ? ERR_TRAY_PP_1_Z_IS_NOT_SAFETY_POS : ERR_TRAY_PP_2_Z_IS_NOT_SAFETY_POS;
	}

	//Test pp Z Home Sensor 확인.
	bool bSafetySenZ1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ1)->IsHomeSensorOn();
	bool bSafetySenZ2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ2)->IsHomeSensorOn();

	if (bSafetySenZ1 != true) {
		return ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx;
	}

	if (bSafetySenZ2 != true) {
		return ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx;
	}

	//Test pp Z Safety Sensor 확인.
	std::vector<int> vLocList;
	vLocList.clear();
	g_TaskTestPP[m_nTestPpIdx].WhereIsMotorPos(m_nTestPpIdx, vLocList, CTaskTestPP::eAxisAll); //Test pp Idx로 type 고정

	if (vLocList[CTaskTestPP::eAxisZ1] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit || vLocList[CTaskTestPP::eAxisZ2] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit)
		return (vLocList[CTaskTestPP::eAxisZ1] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit) ? ERR_TEST_PP_1_Z1_MOVE_POS_NOT_SAFETY + m_nTestPpIdx : ERR_TEST_PP_1_Z2_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;

	return ERR_NO_ERROR;
}
int CMotorInterLock_LoadTbl::Interlock_y_mov(double dTPos, double dSpd, int nAcc, int nDcc)
{
	int nErr = ERR_NO_ERROR;

	nErr = Interlock_common();
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}

	double dCurPosY = 0.0;
	OneAxis* pMotorYTbl = g_TaskLoadTbl[m_nLoadTblIdx].GetAxis(CTaskLoadTable::eAxisY);
	pMotorYTbl->GetCurrentPos(dCurPosY);

	if (dTPos < INTERLOCK_LOADING_TBL_POS_BOUNDARY || dCurPosY < INTERLOCK_LOADING_TBL_POS_BOUNDARY)  
	{
//		nErr = Interlock_trayPP_z_position_check();
		nErr = g_TaskAutoLdTable[m_nLoadTblIdx].ChkMoveLoadSite();
		if (nErr != ERR_NO_ERROR) {
			return nErr;
		}
	}

	if (dTPos > INTERLOCK_LOADING_TBL_POS_BOUNDARY || dCurPosY > INTERLOCK_LOADING_TBL_POS_BOUNDARY) 
	{
		int nRet = g_TaskAutoLdTable[m_nLoadTblIdx].ChkMoveTestSite();
		if (nRet != ERR_NO_ERROR)
			return nRet;
	}

	return ERR_NO_ERROR;
}

int CMotorInterLock_LoadTbl::Interlock_y_jog( double dSpd, int nAcc, int nDcc )
{
	int nErr = ERR_NO_ERROR;

	nErr = Interlock_common();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	double dCurPosY = 0.0;
	OneAxis* pMotorYTbl = g_TaskLoadTbl[m_nLoadTblIdx].GetAxis(CTaskLoadTable::eAxisY);
	pMotorYTbl->GetCurrentPos(dCurPosY);

	if (dCurPosY < INTERLOCK_LOADING_TBL_POS_BOUNDARY) 
	{
		nErr = g_TaskAutoLdTable[m_nLoadTblIdx].ChkMoveLoadSite();
		if (nErr != ERR_NO_ERROR) {
			return nErr;
		}
	}

	if (dCurPosY > INTERLOCK_LOADING_TBL_POS_BOUNDARY)
	{
		int nErr = g_TaskAutoLdTable[m_nLoadTblIdx].ChkMoveTestSite();
		if (nErr != ERR_NO_ERROR)
			return nErr;
	}

	return 0;
}

int CMotorInterLock_LoadTbl::Interlock_Rotate_org()
{
	int nErr = ERR_NO_ERROR;

	nErr = Interlock_common();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}


	//Tray pp Z Home Sensor 확인.
	bool bSafetySenZ_tray_load = g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisZ)->IsHomeSensorOn();
	bool bSafetySenZ_tray_unload = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisZ)->IsHomeSensorOn();

	if (bSafetySenZ_tray_load != true || bSafetySenZ_tray_unload != true) {
		return (bSafetySenZ_tray_load != true) ? ERR_MOT_INTERLOCK_TRAY_PP_1_Z_AXIS_IS_NOT_DETECTED_HOME_SENSOR : ERR_MOT_INTERLOCK_TRAY_PP_2_Z_AXIS_IS_NOT_DETECTED_HOME_SENSOR;
	}

	//Tray pp Z Safety Pos 확인.
	bool bSafetyPosZ_tray_load = g_TaskTrayLoadPP.WhereIsMotorPosZ();
	bool bSafetyPosZ_tray_unload = g_TaskTrayUnloadPP.WhereIsMotorPosZ();

	if (bSafetyPosZ_tray_load != true || bSafetyPosZ_tray_unload != true) {
		return (bSafetyPosZ_tray_load != true) ? ERR_TRAY_PP_1_Z_IS_NOT_SAFETY_POS : ERR_TRAY_PP_2_Z_IS_NOT_SAFETY_POS;
	}

	//Test pp Z Home Sensor 확인.
	bool bSafetySenZ1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ1)->IsHomeSensorOn();
	bool bSafetySenZ2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ2)->IsHomeSensorOn();

	if (bSafetySenZ1 != true) {
		return ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx;
	}

	if (bSafetySenZ2 != true) {
		return ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTestPpIdx;
	}

	//Test pp Z Safety Sensor 확인.
	std::vector<int> vLocList;
	vLocList.clear();
	g_TaskTestPP[m_nTestPpIdx].WhereIsMotorPos(m_nTestPpIdx, vLocList, CTaskTestPP::eAxisAll); //Test pp Idx로 type 고정

	if (vLocList[CTaskTestPP::eAxisZ1] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit || vLocList[CTaskTestPP::eAxisZ2] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit)
		return (vLocList[CTaskTestPP::eAxisZ1] == CTaskTestPP::ePosZ_DownSafetyToPosiLimit) ? ERR_TEST_PP_1_Z1_MOVE_POS_NOT_SAFETY + m_nTestPpIdx : ERR_TEST_PP_1_Z2_MOVE_POS_NOT_SAFETY + m_nTestPpIdx;

	//Vibrator FWD 상태에선 회전 하지 못하도록 한다.

	
	if (g_DMCont.m_dmEQP.GN(NDM0_TABLE_VIBRATOR_OPTION) == DEF_ON)
	{
		int nErr = g_TaskLoadTbl[m_nLoadTblIdx].chk_cyl_vibrator_onoff(DEF_OFF);
		if (nErr != ERR_NO_ERROR)
		{
			return ERR_LOAD_TBL_1_VIBRATOR_CYLINDER_BWD_FAIL + m_nLoadTblIdx;
		}
	}


// 	nErr = Interlock_trayPP_z_position_check();
// 	if (nErr != ERR_NO_ERROR) {
// 		return nErr;
// 	}
// 
// 	nErr = g_TaskAutoLdTable[m_nLoadTblIdx].ChkMoveTestSite();
// 	if (nErr != ERR_NO_ERROR)
// 		return nErr;

	return ERR_NO_ERROR;
}
int CMotorInterLock_LoadTbl::Interlock_Rotate_mov(double dTPos, double dSpd, int nAcc, int nDcc)
{
	int nErr = ERR_NO_ERROR;

	nErr = Interlock_common();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	

	OneAxis* pMotorTblY = g_TaskLoadTbl[m_nLoadTblIdx].GetAxis(CTaskLoadTable::eAxisY);
	if (pMotorTblY->IsOrgCompleted(nullptr) != TRUE) {
		return ERR_MOT_INTERLOCK_LOADING_TBL_1_Y_IS_NOT_ORIGIN + m_nLoadTblIdx;
	}

	double nCurPosY = 0.0;
	pMotorTblY->GetCurrentPos(nCurPosY);

	if (nCurPosY < INTERLOCK_LOADING_TBL_POS_BOUNDARY)
	{
		int nRet = g_TaskAutoLdTable[m_nLoadTblIdx].ChkMoveLoadSite();
		if (nRet != ERR_NO_ERROR)
			return nRet;
	}

	if (nCurPosY > INTERLOCK_LOADING_TBL_POS_BOUNDARY)
	{
		int nRet = g_TaskAutoLdTable[m_nLoadTblIdx].ChkMoveTestSite();
		if (nRet != ERR_NO_ERROR)
			return nRet;
	}

	//Vibrator FWD 상태에선 회전 하지 못하도록 한다.
	if (g_DMCont.m_dmEQP.GN(NDM0_TABLE_VIBRATOR_OPTION) == DEF_ON)
	{
		int nErr = g_TaskLoadTbl[m_nLoadTblIdx].chk_cyl_vibrator_onoff(DEF_OFF);
		if (nErr != ERR_NO_ERROR)
		{
			return ERR_LOAD_TBL_1_VIBRATOR_CYLINDER_BWD_FAIL + m_nLoadTblIdx;
		}
	}


	return ERR_NO_ERROR;
}

int CMotorInterLock_LoadTbl::Interlock_Rotate_jog(double dSpd, int nAcc, int nDcc)
{
	int nErr = ERR_NO_ERROR;

	nErr = Interlock_common();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	//Y축 Origin Check
	OneAxis* pMotorTblY = g_TaskLoadTbl[m_nLoadTblIdx].GetAxis(CTaskLoadTable::eAxisY);
	if (pMotorTblY->IsOrgCompleted(nullptr) != TRUE) {
		return ERR_MOT_INTERLOCK_LOADING_TBL_1_Y_IS_NOT_ORIGIN + m_nLoadTblIdx;
	}

	double nCurPosY = 0.0;
	pMotorTblY->GetCurrentPos(nCurPosY);

	//Table Y축 test site 확인.
	if (nCurPosY < INTERLOCK_LOADING_TBL_POS_BOUNDARY)
	{
		int nRet = g_TaskAutoLdTable[m_nLoadTblIdx].ChkMoveLoadSite();
		if (nRet != ERR_NO_ERROR)
			return nRet;
	}

	if (nCurPosY > INTERLOCK_LOADING_TBL_POS_BOUNDARY)
	{
		int nRet = g_TaskAutoLdTable[m_nLoadTblIdx].ChkMoveTestSite();
		if (nRet != ERR_NO_ERROR)
			return nRet;
	}


	//Vibrator FWD 상태에선 회전 하지 못하도록 한다.
	if (g_DMCont.m_dmEQP.GN(NDM0_TABLE_VIBRATOR_OPTION) == DEF_ON)
	{
		int nErr = g_TaskLoadTbl[m_nLoadTblIdx].chk_cyl_vibrator_onoff(DEF_OFF);
		if (nErr != ERR_NO_ERROR)
		{
			return ERR_LOAD_TBL_1_VIBRATOR_CYLINDER_BWD_FAIL + m_nLoadTblIdx;
		}
	}

	return 0;
}

int CMotorInterLock_LoadTbl::Interlock_common()
{
// 	OneAxis* pMotorTblY = g_TaskLoadTbl[m_nLoadTblIdx].GetAxis(CTaskLoadTable::eAxisY);
// 	if (pMotorTblY->ChkMotorUsable() != ERR_NO_ERROR) {
// 		return ERR_MOT_INTERLOCK_LOAD_TABLE_1_Y_AXIS_IS_INVALID + m_nLoadTblIdx;
// 	}
	int nErr = ERR_NO_ERROR;

	OneAxis* pMotorX1 = g_TaskTestPP[m_nTestPpIdx].GetAxis( CTaskTestPP::eAxisX1 );
	OneAxis* pMotorX2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisX2);
	OneAxis* pMotorY = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisY);
	OneAxis* pMotorZ1 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ1);
	OneAxis* pMotorZ2 = g_TaskTestPP[m_nTestPpIdx].GetAxis(CTaskTestPP::eAxisZ2);

	int nIdx = m_nLoadTblIdx/2;

	if (pMotorZ1->IsMotorUsable(nErr) != TRUE) {
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z1_IS_AMP_FAULT + nIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z1_AXIS_IS_NOT_ORIGIN + nIdx;
	}
	if (pMotorZ2->IsMotorUsable(nErr) != TRUE) {
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TEST_PP_1_Z2_IS_AMP_FAULT + nIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Z2_AXIS_IS_NOT_ORIGIN + nIdx;
	}
	if( pMotorY->IsMotorUsable(nErr) != TRUE){
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TEST_PP_1_Y_IS_AMP_FAULT + nIdx : ERR_MOT_INTERLOCK_TEST_PP_1_Y_AXIS_IS_NOT_ORIGIN + nIdx;
	}
	if(pMotorX1->IsMotorUsable(nErr) != TRUE){
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TEST_PP_1_X1_IS_AMP_FAULT + nIdx : ERR_MOT_INTERLOCK_TEST_PP_1_X1_AXIS_IS_NOT_ORIGIN + nIdx;
	}
	if (pMotorX2->IsMotorUsable(nErr) != TRUE) {
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TEST_PP_1_X2_IS_AMP_FAULT + nIdx : ERR_MOT_INTERLOCK_TEST_PP_1_X2_AXIS_IS_NOT_ORIGIN + nIdx;
	}

	OneAxis* pMotorTrayPp1X = g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisX);
	OneAxis* pMotorTrayPp2X = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisX);

	OneAxis* pMotorTrayPp1Z = g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisZ);
	OneAxis* pMotorTrayPp2Z = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisZ);


	if (pMotorTrayPp1X->IsMotorUsable(nErr) != TRUE) {
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TRAY_PP_1_X_IS_AMP_FAULT : ERR_MOT_INTERLOCK_TRAY_PP_1_X_AXIS_IS_NOT_ORIGIN;
	}

	if (pMotorTrayPp2X->IsMotorUsable(nErr) != TRUE) {
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TRAY_PP_2_X_IS_AMP_FAULT : ERR_MOT_INTERLOCK_TRAY_PP_2_X_AXIS_IS_NOT_ORIGIN;
	}

	if(pMotorTrayPp1Z->IsMotorUsable(nErr) != TRUE){
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TRAY_PP_1_Z_IS_AMP_FAULT : ERR_MOT_INTERLOCK_TRAY_PP_1_Z_AXIS_IS_NOT_ORIGIN;
	}

	if (pMotorTrayPp2Z->IsMotorUsable(nErr) != TRUE) {
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TRAY_PP_2_Z_IS_AMP_FAULT : ERR_MOT_INTERLOCK_TRAY_PP_2_Z_AXIS_IS_NOT_ORIGIN;
	}

	return ERR_NO_ERROR;
}

int CMotorInterLock_LoadTbl::Interlock_trayPP_z_position_check()
{
	CPoint pocket_0_0( 0, 0 );
	CPoint pocket_3_2( MAX_LD_TBL_POCKET_CNT_X-1, MAX_LD_TBL_POCKET_CNT_Y - 1 );

	std::vector<CPoint> vPicker_7_1;
	vPicker_7_1.push_back( CPoint( MAX_TRAY_PP_CNT_X-1, MAX_TRAY_PP_CNT_Y - 1) );
	std::vector<CPoint> vPicker_0_0;
	vPicker_0_0.push_back( CPoint( 0, 0 ) );


	/*
	table1 - U
	Load Hand
	1) Motion Done이 아니면 Z 확인.
	2) Motion Done이면 table Load 위치, feeder 위치 확인.(table2,4, feeder확인.) -> 해당 위치 아니면 Z 확인 필요함.
	Unload Hand - Teaching position 확인 -> Z Safety 위치 확인.


	table2 - L
	Load Hand - Teaching Position 확인 -> Z Safety 위치 확인.
	Unload Hand - Check할 필요 없음.





	table3 - U
	table4 - L

	*/


	for (int nTrayPpInx = 0; nTrayPpInx < eMaxTrayPPCount; nTrayPpInx++)
	{
		for (int nLoc = 0; nLoc < LOC_IDX_TRAY_PP_TBL_MAX_COUNT; nLoc++)
		{
			//tray PP teaching Position 맞춰서 작업.
			CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
			double dLeft   = pTaskTrayPP[nTrayPpInx]->GetTeachPos((eLocIdxTrayPP)(LOC_IDX_TRAY_PP_TBL_1 + nLoc), pocket_0_0, vPicker_7_1, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX);
			double dTop    = pTaskTrayPP[nTrayPpInx]->GetTeachPos((eLocIdxTrayPP)(LOC_IDX_TRAY_PP_TBL_1 + nLoc), pocket_0_0, vPicker_7_1, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisY);
			double dRight  = pTaskTrayPP[nTrayPpInx]->GetTeachPos((eLocIdxTrayPP)(LOC_IDX_TRAY_PP_TBL_1 + nLoc), pocket_3_2, vPicker_0_0, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX);
			double dBottom = pTaskTrayPP[nTrayPpInx]->GetTeachPos((eLocIdxTrayPP)(LOC_IDX_TRAY_PP_TBL_1 + nLoc), pocket_3_2, vPicker_0_0, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisY);

			double dTrayPP_Cur_Pos_X=0, dTrayPP_Cur_Pos_Y=0;

			pTaskTrayPP[nTrayPpInx]->GetAxis(CTaskTrayBasePP::eAxisX)->GetCurrentPos(dTrayPP_Cur_Pos_X);
			pTaskTrayPP[nTrayPpInx]->GetAxis(CTaskTrayBasePP::eAxisY)->GetCurrentPos(dTrayPP_Cur_Pos_Y);

			if (((dTrayPP_Cur_Pos_X > dLeft) && (dTrayPP_Cur_Pos_X < dRight)) &&
				(dTrayPP_Cur_Pos_Y > dBottom) && (dTrayPP_Cur_Pos_Y < dTop))
			{
				std::vector<int> vLocList;
				bool bSafetyZ = pTaskTrayPP[nTrayPpInx]->WhereIsMotorPosZ();
				
				if ( bSafetyZ != true ) {
					return ERR_TRAY_PP_1_Z_IS_NOT_SAFETY_POS + nTrayPpInx;
				}
			}
		}		
	}


	return ERR_NO_ERROR;
}
