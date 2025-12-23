#include "stdafx.h"
#include "MotorInterlock.h"

ST_MOT_INTERLOCK_CALLBACK_MAP g_aMotorInterlockMap[eMAX_AXIS_COUNT] = {
  	{ eAXIS_PRESS_UNIT_DN1      , MotorMoveInterLock_PressUnit_Dn1_z		,	MotorOrgInterLock_PressUnit_Dn1_z		, MotorJogInterLock_PressUnit_Dn1_z },
  	{ eAXIS_PRESS_UNIT_DN2      , MotorMoveInterLock_PressUnit_Dn2_z		,	MotorOrgInterLock_PressUnit_Dn2_z		, MotorJogInterLock_PressUnit_Dn2_z },
  	{ eAXIS_PRESS_UNIT_DN3      , MotorMoveInterLock_PressUnit_Dn3_z		,	MotorOrgInterLock_PressUnit_Dn3_z		, MotorJogInterLock_PressUnit_Dn3_z },
  	{ eAXIS_PRESS_UNIT_DN4      , MotorMoveInterLock_PressUnit_Dn4_z		,	MotorOrgInterLock_PressUnit_Dn4_z		, MotorJogInterLock_PressUnit_Dn4_z },
  	{ eAXIS_PRESS_UNIT_DN5      , MotorMoveInterLock_PressUnit_Dn5_z		,	MotorOrgInterLock_PressUnit_Dn5_z		, MotorJogInterLock_PressUnit_Dn5_z },
  	{ eAXIS_PRESS_UNIT_DN6      , MotorMoveInterLock_PressUnit_Dn6_z		,	MotorOrgInterLock_PressUnit_Dn6_z		, MotorJogInterLock_PressUnit_Dn6_z },
  	{ eAXIS_PRESS_UNIT_DN7      , MotorMoveInterLock_PressUnit_Dn7_z		,	MotorOrgInterLock_PressUnit_Dn7_z		, MotorJogInterLock_PressUnit_Dn7_z },
  	{ eAXIS_PRESS_UNIT_DN8      , MotorMoveInterLock_PressUnit_Dn8_z		,	MotorOrgInterLock_PressUnit_Dn8_z		, MotorJogInterLock_PressUnit_Dn8_z },
  
  	{ eAXIS_TEST_PP_1_X         , MotorMoveInterLock_Test_Hand1_x1			,	MotorOrgInterLock_Test_Hand1_x1			, MotorJogInterLock_Test_Hand1_x1 },
  	{ eAXIS_TEST_PP_1_X2        , MotorMoveInterLock_Test_Hand1_x2			,	MotorOrgInterLock_Test_Hand1_x2			, MotorJogInterLock_Test_Hand1_x2 },
  	{ eAXIS_TEST_PP_1_Y         , MotorMoveInterLock_Test_Hand1_y			,	MotorOrgInterLock_Test_Hand1_y			, MotorJogInterLock_Test_Hand1_y },
  	{ eAXIS_TEST_PP_1_Z         , MotorMoveInterLock_Test_Hand1_z1			,	MotorOrgInterLock_Test_Hand1_z1			, MotorJogInterLock_Test_Hand1_z1 },
  	{ eAXIS_TEST_PP_1_Z2        , MotorMoveInterLock_Test_Hand1_z2			,	MotorOrgInterLock_Test_Hand1_z2			, MotorJogInterLock_Test_Hand1_z2 },
  	{ eAXIS_TEST_PP_1_X_PITCH	, MotorMoveInterLock_Test_Hand1_Pitch_HW_X1_SW_Y1	,	MotorOrgInterLock_Test_Hand1_Pitch_HW_X1_SW_Y1	, MotorJogInterLock_Test_Hand1_Pitch_HW_X1_SW_Y1},
  	{ eAXIS_TEST_PP_1_X2_PITCH	, MotorMoveInterLock_Test_Hand1_Pitch_HW_X2_SW_Y2	,	MotorOrgInterLock_Test_Hand1_Pitch_HW_X2_SW_Y2	, MotorJogInterLock_Test_Hand1_Pitch_HW_X2_SW_Y2},
  	{ eAXIS_TEST_PP_1_Y_PITCH	, MotorMoveInterLock_Test_Hand1_Pitch_HW_Y1_SW_X1	,	MotorOrgInterLock_Test_Hand1_Pitch_HW_Y1_SW_X1	, MotorJogInterLock_Test_Hand1_Pitch_HW_Y1_SW_X1 },
  	{ eAXIS_TEST_PP_1_Y2_PITCH	, MotorMoveInterLock_Test_Hand1_Pitch_HW_Y2_SW_X2	,	MotorOrgInterLock_Test_Hand1_Pitch_HW_Y2_SW_X2	, MotorJogInterLock_Test_Hand1_Pitch_HW_Y2_SW_X2 },
  
  
  	{ eAXIS_TEST_PP_2_X         , MotorMoveInterLock_Test_Hand2_x1			,	MotorOrgInterLock_Test_Hand2_x1			, MotorJogInterLock_Test_Hand2_x1 },
  	{ eAXIS_TEST_PP_2_X2        , MotorMoveInterLock_Test_Hand2_x2			,	MotorOrgInterLock_Test_Hand2_x2			, MotorJogInterLock_Test_Hand2_x2 },
  	{ eAXIS_TEST_PP_2_Y         , MotorMoveInterLock_Test_Hand2_y			,	MotorOrgInterLock_Test_Hand2_y			, MotorJogInterLock_Test_Hand2_y },
  	{ eAXIS_TEST_PP_2_Z         , MotorMoveInterLock_Test_Hand2_z1			,	MotorOrgInterLock_Test_Hand2_z1			, MotorJogInterLock_Test_Hand2_z1 },
  	{ eAXIS_TEST_PP_2_Z2        , MotorMoveInterLock_Test_Hand2_z2			,	MotorOrgInterLock_Test_Hand2_z2			, MotorJogInterLock_Test_Hand2_z2 },
  	{ eAXIS_TEST_PP_2_X_PITCH	, MotorMoveInterLock_Test_Hand2_Pitch_HW_X1_SW_Y1	,	MotorOrgInterLock_Test_Hand2_Pitch_HW_X1_SW_Y1	, MotorJogInterLock_Test_Hand2_Pitch_HW_X1_SW_Y1 },
  	{ eAXIS_TEST_PP_2_X2_PITCH	, MotorMoveInterLock_Test_Hand2_Pitch_HW_X2_SW_Y2	,	MotorOrgInterLock_Test_Hand2_Pitch_HW_X2_SW_Y2	, MotorJogInterLock_Test_Hand2_Pitch_HW_X2_SW_Y2 },
  	{ eAXIS_TEST_PP_2_Y_PITCH	, MotorMoveInterLock_Test_Hand2_Pitch_HW_Y1_SW_X1	,	MotorOrgInterLock_Test_Hand2_Pitch_HW_Y1_SW_X1	, MotorJogInterLock_Test_Hand2_Pitch_HW_Y1_SW_X1 },
  	{ eAXIS_TEST_PP_2_Y2_PITCH	, MotorMoveInterLock_Test_Hand2_Pitch_HW_Y2_SW_X2	,	MotorOrgInterLock_Test_Hand2_Pitch_HW_Y2_SW_X2	, MotorJogInterLock_Test_Hand2_Pitch_HW_Y2_SW_X2 },
  
  
  	//Task TrayPP 작업 완료 시, IO 및 g_TaskTrayPP 변경.
  	{ eAXIS_LOAD_TABLE_1_Y      , MotorMoveInterLock_LoadTbl1_y				,	MotorOrgInterLock_LoadTbl1_y			, MotorJogInterLock_LoadTbl1_y },
  	{ eAXIS_LOAD_TABLE_2_Y      , MotorMoveInterLock_LoadTbl2_y				,	MotorOrgInterLock_LoadTbl2_y			, MotorJogInterLock_LoadTbl2_y },
  	{ eAXIS_LOAD_TABLE_3_Y      , MotorMoveInterLock_LoadTbl3_y				,	MotorOrgInterLock_LoadTbl3_y			, MotorJogInterLock_LoadTbl3_y },
  	{ eAXIS_LOAD_TABLE_4_Y      , MotorMoveInterLock_LoadTbl4_y				,	MotorOrgInterLock_LoadTbl4_y			, MotorJogInterLock_LoadTbl4_y },
  
  	{ eAXIS_LOAD_TABLE_1_ROTATE     , MotorMoveInterLock_LoadTbl1_Rotate				,	MotorOrgInterLock_LoadTbl1_Rotate			, MotorJogInterLock_LoadTbl1_Rotate },
  	{ eAXIS_LOAD_TABLE_2_ROTATE     , MotorMoveInterLock_LoadTbl2_Rotate				,	MotorOrgInterLock_LoadTbl2_Rotate			, MotorJogInterLock_LoadTbl2_Rotate },
  	{ eAXIS_LOAD_TABLE_3_ROTATE     , MotorMoveInterLock_LoadTbl3_Rotate				,	MotorOrgInterLock_LoadTbl3_Rotate			, MotorJogInterLock_LoadTbl3_Rotate },
  	{ eAXIS_LOAD_TABLE_4_ROTATE     , MotorMoveInterLock_LoadTbl4_Rotate				,	MotorOrgInterLock_LoadTbl4_Rotate			, MotorJogInterLock_LoadTbl4_Rotate },
  
  	//Tray PP, Feeder  Task작업 완료 시, Interlock 마무리 예정.
  	//TaskTrayLoadPP, TaskTrayUnloadPP
  	//
  	{ eAXIS_TRAY_PP_1_X_PITCH     , MotorMoveInterLock_CTray_Hand1_Pitch_x	,	MotorOrgInterLock_CTray_Hand1_Pitch_x	, MotorJogInterLock_CTray_Hand1_Pitch_x },
  	{ eAXIS_TRAY_PP_1_Y_PITCH     , MotorMoveInterLock_CTray_Hand1_Pitch_y	,	MotorOrgInterLock_CTray_Hand1_Pitch_y	, MotorJogInterLock_CTray_Hand1_Pitch_y },
  	{ eAXIS_TRAY_PP_1_X           , MotorMoveInterLock_CTray_Hand1_x      	,	MotorOrgInterLock_CTray_Hand1_x			, MotorJogInterLock_CTray_Hand1_x },
  	{ eAXIS_TRAY_PP_1_Z           , MotorMoveInterLock_CTray_Hand1_z		,	MotorOrgInterLock_CTray_Hand1_z			, MotorJogInterLock_CTray_Hand1_z },
  
  	{ eAXIS_TRAY_PP_2_X_PITCH     , MotorMoveInterLock_CTray_Hand2_Pitch_x	,	MotorOrgInterLock_CTray_Hand2_Pitch_x	, MotorJogInterLock_CTray_Hand2_Pitch_x },
  	{ eAXIS_TRAY_PP_2_Y_PITCH     , MotorMoveInterLock_CTray_Hand2_Pitch_y	,	MotorOrgInterLock_CTray_Hand2_Pitch_y	, MotorJogInterLock_CTray_Hand2_Pitch_y },
  	{ eAXIS_TRAY_PP_2_X           , MotorMoveInterLock_CTray_Hand2_x      	,	MotorOrgInterLock_CTray_Hand2_x			, MotorJogInterLock_CTray_Hand2_x },
  	{ eAXIS_TRAY_PP_2_Y           , MotorMoveInterLock_CTray_Hand2_y       	,	MotorOrgInterLock_CTray_Hand2_y			, MotorJogInterLock_CTray_Hand2_y },
  	{ eAXIS_TRAY_PP_2_Z           , MotorMoveInterLock_CTray_Hand2_z		,	MotorOrgInterLock_CTray_Hand2_z			, MotorJogInterLock_CTray_Hand2_z },
  
  	{ eAXIS_TRANSFER_X          , MotorMoveInterLock_Transfer_X				,	MotorOrgInterLock_Transfer_X			, MotorJogInterLock_Transfer_X },
  
  	{ eAXIS_TRAY_FEEDER_Y           , MotorMoveInterLock_CTray_Feeder       	,	MotorOrgInterLock_CTray_Feeder			, MotorJogInterLock_CTray_Feeder },
  

};


//==============================================================================
//
//==============================================================================
void AfxGetMotorInterlockData( int nLogicalIdx, ST_MOT_INTERLOCK_CALLBACK_MAP& rData )
{
	for( int i = 0; i < eMAX_AXIS_COUNT; i++ ) {
		if( g_aMotorInterlockMap[ i ].nLogicalIdx == nLogicalIdx ) {
			rData = g_aMotorInterlockMap[ i ];
			break;
		}
	}
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-24]
  NAME     :
  PARAMS   :
  RETURN   : 0 = No Error
			 음수 = Motor Interlock Error Code (DEF_ERROR_CODE.h에 정의하여 사용)
  COMMENTS : [InterLock Move] Press Unit Down 1
==============================================================================*/
int MotorMoveInterLock_PressUnit_Dn1_z( double dTPos, double dSpd, int nAcc, int nDcc )
{

	CMotorInterlock_PressUnit cMotorInterlock( ePressUnit_Down_1, eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_1].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_z_mov( dTPos, dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}

	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-24]
  NAME     :
  PARAMS   :
  RETURN   : 0 = No Error
			 음수 = Motor Interlock Error Code (DEF_ERROR_CODE.h에 정의하여 사용)
  COMMENTS : [InterLock Move] Press Unit Down 2
==============================================================================*/
int MotorMoveInterLock_PressUnit_Dn2_z( double dTPos, double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_PressUnit cMotorInterlock( ePressUnit_Down_2, eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_2].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_z_mov( dTPos, dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-24]
  NAME     :
  PARAMS   :
  RETURN   : 0 = No Error
			 음수 = Motor Interlock Error Code (DEF_ERROR_CODE.h에 정의하여 사용)
  COMMENTS : [InterLock Move] Press Unit Down 3
==============================================================================*/
int MotorMoveInterLock_PressUnit_Dn3_z( double dTPos, double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_PressUnit cMotorInterlock( ePressUnit_Down_3, eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_3].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_z_mov( dTPos, dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-24]
  NAME     :
  PARAMS   :
  RETURN   : 0 = No Error
			 음수 = Motor Interlock Error Code (DEF_ERROR_CODE.h에 정의하여 사용)
  COMMENTS : [InterLock Move] Press Unit Down 4
==============================================================================*/
int MotorMoveInterLock_PressUnit_Dn4_z( double dTPos, double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_PressUnit cMotorInterlock( ePressUnit_Down_4, eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_4].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_z_mov( dTPos, dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-24]
  NAME     :
  PARAMS   :
  RETURN   : 0 = No Error
			음수 = Motor Interlock Error Code (DEF_ERROR_CODE.h에 정의하여 사용)
  COMMENTS : [InterLock Move] Press Unit Down 5
==============================================================================*/
int MotorMoveInterLock_PressUnit_Dn5_z( double dTPos, double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_PressUnit cMotorInterlock( ePressUnit_Down_5, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_5].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_z_mov( dTPos, dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-24]
  NAME     :
  PARAMS   :
  RETURN   : 0 = No Error
			 음수 = Motor Interlock Error Code (DEF_ERROR_CODE.h에 정의하여 사용)
  COMMENTS : [InterLock Move] Press Unit Down 6
==============================================================================*/
int MotorMoveInterLock_PressUnit_Dn6_z( double dTPos, double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_PressUnit cMotorInterlock( ePressUnit_Down_6, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_6].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_z_mov( dTPos, dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-24]
  NAME     :
  PARAMS   :
  RETURN   : 0 = No Error
			 음수 = Motor Interlock Error Code (DEF_ERROR_CODE.h에 정의하여 사용)
  COMMENTS : [InterLock Move] Press Unit Down 7
==============================================================================*/
int MotorMoveInterLock_PressUnit_Dn7_z( double dTPos, double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_PressUnit cMotorInterlock( ePressUnit_Down_7, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_7].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_z_mov( dTPos, dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-24]
  NAME     :
  PARAMS   :
  RETURN   : 0 = No Error
			 음수 = Motor Interlock Error Code (DEF_ERROR_CODE.h에 정의하여 사용)
  COMMENTS : [InterLock Move] Press Unit Down 8
==============================================================================*/
int MotorMoveInterLock_PressUnit_Dn8_z( double dTPos, double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_PressUnit cMotorInterlock( ePressUnit_Down_8, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_8].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_z_mov( dTPos, dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-24]
  NAME     :
  PARAMS   :
  RETURN   :
  COMMENTS :
==============================================================================*/
int MotorMoveInterLock_Test_Hand1_x1( double dTPos, double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_TestPP cMotorInterlock( eTestPP_1 , eTestPPLoadType );

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisX1);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_x_mov( dTPos, dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_Test_Hand1_x2(double dTPos, double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_1, eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisX2);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_x_mov(dTPos, dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-24]
  NAME     :
  PARAMS   :
  RETURN   :
  COMMENTS :
==============================================================================*/
int MotorMoveInterLock_Test_Hand1_y( double dTPos, double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_TestPP cMotorInterlock( eTestPP_1 );

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_y_mov( dTPos, dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-24]
  NAME     :
  PARAMS   :
  RETURN   :
  COMMENTS :
==============================================================================*/
int MotorMoveInterLock_Test_Hand1_z1( double dTPos, double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_TestPP cMotorInterlock( eTestPP_1, eTestPPLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisZ1);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	//Z Up(safety pos) 일 경우 No Interlock
// 	CPoint cPocket = CPoint(0,0);
// 	std::vector<CPoint> vPicker; vPicker.clear;
// 	vPicker.push_back(CPoint(0, 0));
// 	g_TaskTestPP[eTestPP_1].GetTeachPos(LOC_IDX_TEST_PP_HOME, cPocket, vPicker, CTaskTestPP::eZPOS_SAFETY, dTPos, m_nHandType);
// 	double dSafetyPos = g_DMCont.m_Teach.GetTeachPos(eTestPP_1, LOC_IDX_TEST_PP_HOME, POS_IDX_TEST_PP_Z1_SAFETY);
// 	if (dTPos == dSafetyPos)
// 		return ERR_NO_ERROR;
//Z Up(safety pos) 일 경우 No Interlock
	double dSafetyPos = g_DMCont.m_Teach.GetTeachPos(eTestPP_1, LOC_IDX_TEST_PP_HOME, POS_IDX_TEST_PP_Z1_SAFETY);
	if (dTPos <= dSafetyPos)
		return ERR_NO_ERROR;

	double dCurPos = 0.0;
	pMotor->GetCurrentPos(dCurPos);
	if (dCurPos > dTPos) {
		return ERR_NO_ERROR;
	}


	nErr = cMotorInterlock.Interlock_z_mov( dTPos, dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_Test_Hand1_z2(double dTPos, double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_1, eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisZ2);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	//Z Up(safety pos) 일 경우 No Interlock
// 	double dSafetyPos = g_DMCont.m_Teach.GetTeachPos(eTestPP_1, LOC_IDX_TEST_PP_HOME, POS_IDX_TEST_PP_Z2_SAFETY);
// 	if (dTPos == dSafetyPos)
// 		return ERR_NO_ERROR;

	//Z Up(safety pos) 일 경우 No Interlock
	double dSafetyPos = g_DMCont.m_Teach.GetTeachPos(eTestPP_1, LOC_IDX_TEST_PP_HOME, POS_IDX_TEST_PP_Z2_SAFETY);
	if (dTPos <= dSafetyPos)
		return ERR_NO_ERROR;


	double dCurPos = 0.0;
	pMotor->GetCurrentPos(dCurPos);
	if (dCurPos > dTPos) {
		return ERR_NO_ERROR;
	}

	nErr = cMotorInterlock.Interlock_z_mov(dTPos, dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_Test_Hand1_Pitch_HW_X1_SW_Y1(double dTPos, double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_1,eTestPPLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisPitch_HW_X1_SW_Y1);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_xpitch_mov(dTPos, dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_Test_Hand1_Pitch_HW_X2_SW_Y2(double dTPos, double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_1, eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisPitch_HW_X2_SW_Y2);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_xpitch_mov(dTPos, dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_Test_Hand1_Pitch_HW_Y1_SW_X1(double dTPos, double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_1, eTestPPLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisPitch_HW_Y1_SW_X1);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_ypitch_mov(dTPos, dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_Test_Hand1_Pitch_HW_Y2_SW_X2(double dTPos, double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_1, eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisPitch_HW_Y2_SW_X2);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_ypitch_mov(dTPos, dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}


int MotorMoveInterLock_Test_Hand2_x1( double dTPos, double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_TestPP cMotorInterlock( eTestPP_2, eTestPPLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisX1);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_x_mov( dTPos, dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_Test_Hand2_x2(double dTPos, double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_2, eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisX2);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_x_mov(dTPos, dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}


int MotorMoveInterLock_Test_Hand2_y( double dTPos, double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_TestPP cMotorInterlock( eTestPP_2 );

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_y_mov( dTPos, dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-24]
  NAME     :
  PARAMS   :
  RETURN   :
  COMMENTS :
==============================================================================*/
int MotorMoveInterLock_Test_Hand2_z1( double dTPos, double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_TestPP cMotorInterlock( eTestPP_2, eTestPPLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisZ1);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	//Z Up(safety pos) 일 경우 No Interlock
	double dSafetyPos = g_DMCont.m_Teach.GetTeachPos(eTestPP_2, LOC_IDX_TEST_PP_HOME, POS_IDX_TEST_PP_Z1_SAFETY);
	if (dTPos <= dSafetyPos)
		return ERR_NO_ERROR;

	double dCurPos = 0.0;
	pMotor->GetCurrentPos(dCurPos);
	if (dCurPos > dTPos) {
		return ERR_NO_ERROR;
	}

	nErr = cMotorInterlock.Interlock_z_mov( dTPos, dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}
int MotorMoveInterLock_Test_Hand2_z2(double dTPos, double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_2, eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisZ2);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	//Z Up(safety pos) 일 경우 No Interlock
	double dSafetyPos = g_DMCont.m_Teach.GetTeachPos(eTestPP_2, LOC_IDX_TEST_PP_HOME, POS_IDX_TEST_PP_Z1_SAFETY);
	if (dTPos <= dSafetyPos)
		return ERR_NO_ERROR;

	double dCurPos = 0.0;
	pMotor->GetCurrentPos(dCurPos);
	if (dCurPos > dTPos) {
		return ERR_NO_ERROR;
	}

	nErr = cMotorInterlock.Interlock_z_mov(dTPos, dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_Test_Hand2_Pitch_HW_X1_SW_Y1(double dTPos, double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_2,eTestPPLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisPitch_HW_X1_SW_Y1);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_xpitch_mov(dTPos, dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_Test_Hand2_Pitch_HW_X2_SW_Y2(double dTPos, double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_2, eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisPitch_HW_X2_SW_Y2);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_xpitch_mov(dTPos, dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_Test_Hand2_Pitch_HW_Y1_SW_X1(double dTPos, double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_2, eTestPPLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisPitch_HW_Y1_SW_X1);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_ypitch_mov(dTPos, dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_Test_Hand2_Pitch_HW_Y2_SW_X2( double dTPos, double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_2, eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisPitch_HW_Y2_SW_X2);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_ypitch_mov( dTPos, dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_Transfer_X(double dTPos, double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_Transfer cMotorInterlock(eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTransfer.GetAxis(CTaskTransfer::AXIS_TRANSFER_X);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_x_mov(dTPos, dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

// int MotorMoveInterLock_Tray_Feeder_End_Stopper(double dTPos, double dSpd, int nAcc, int nDcc)
// {
// 	CMotorInterlock_TrayPP cMotorInterlock;
// 	int nErr = cMotorInterlock.CommonInterlock();
// 	if (nErr != ERR_NO_ERROR) {
// 		return nErr;
// 	}
// 
// 	OneAxis* pMotor = g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eAxisEndStopper);
// 	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
// 	if (nErr != ERR_NO_ERROR) {
// 		return nErr;
// 	}
// 	
// 	//Stacker 3 Align Cylinder Back 확인.
// 	nErr = g_TaskStacker[eSTACKER_03].chk_cyl_TrayAlign(DEF_BACK);
// 	if (nErr != ERR_NO_ERROR) {
// 		return ERR_MOT_INTERLOCK_END_STOPPER_MOVE_NOT_STACKER_3_ALIGN_CYLINDER_FORWARD;
// 	}
// 
// 	return ERR_NO_ERROR;
// }

int MotorMoveInterLock_CTray_Hand1_x( double dTPos, double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_1_LD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayLoadPP.GetAxis(CTaskTrayLoadPP::eAxisX);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_x_mov( dTPos, dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_CTray_Feeder( double dTPos, double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_TrayPP cMotorInterlock;

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_Feeder_mov( dTPos, dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_CTray_Hand1_z( double dTPos, double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_1_LD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayLoadPP.GetAxis(CTaskTrayLoadPP::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	//Z Up(safety pos) 일 경우 No Interlock
	std::vector<CPoint> vCmdPicker;
	vCmdPicker.push_back(CPoint(0, 0));
	double dSafetyPos = g_TaskTrayLoadPP.GetTeachPos(static_cast< eLocIdxTrayPP >(LOC_IDX_TRAY_PP_SAFETY), CPoint(0, 0), vCmdPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);

	if (dTPos == dSafetyPos)
		return ERR_NO_ERROR;

// 	double dCurPos = 0.0;
// 	pMotor->GetCurrentPos(dCurPos);
// 	if (dCurPos > dTPos) {
// 		return ERR_NO_ERROR;
// 	}

	//Z Down Interlock Check.
	nErr = cMotorInterlock.Interlock_z_mov( dTPos, dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_CTray_Hand1_Pitch_x( double dTPos, double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_1_LD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayLoadPP.GetAxis(CTaskTrayLoadPP::eAxisPitchX);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_xpitch_mov( dTPos, dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_CTray_Hand1_Pitch_y( double dTPos, double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_1_LD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayLoadPP.GetAxis(CTaskTrayLoadPP::eAxisPitchY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_ypitch_mov( dTPos, dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}


int MotorMoveInterLock_CTray_Hand2_x(double dTPos, double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_2_UD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayUnloadPP.GetAxis(CTaskTrayUnloadPP::eAxisX);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_x_mov(dTPos, dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_CTray_Hand2_y(double dTPos, double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_2_UD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayUnloadPP.GetAxis(CTaskTrayUnloadPP::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_y_mov(dTPos, dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_CTray_Hand2_z(double dTPos, double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_2_UD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayUnloadPP.GetAxis(CTaskTrayUnloadPP::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	//Z Up(safety pos) 일 경우 No Interlock
	std::vector<CPoint> vCmdPicker;
	vCmdPicker.push_back(CPoint(0, 0));
	double dSafetyPos = g_TaskTrayUnloadPP.GetTeachPos(static_cast<eLocIdxTrayPP>(LOC_IDX_TRAY_PP_SAFETY), CPoint(0, 0), vCmdPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);

	if (dTPos == dSafetyPos)
		return ERR_NO_ERROR;

	double dCurPos = 0.0;
	pMotor->GetCurrentPos(dCurPos);
	if (dCurPos > dTPos - 200) {
		return ERR_NO_ERROR;
 	}

	// Z Down일 경우 Interlock Check.
	nErr = cMotorInterlock.Interlock_z_mov(dTPos, dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_CTray_Hand2_Pitch_x(double dTPos, double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_2_UD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayUnloadPP.GetAxis(CTaskTrayUnloadPP::eAxisPitchX);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_xpitch_mov(dTPos, dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_CTray_Hand2_Pitch_y(double dTPos, double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_2_UD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayUnloadPP.GetAxis(CTaskTrayUnloadPP::eAxisPitchY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_ypitch_mov(dTPos, dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}


int MotorMoveInterLock_LoadTbl1_y( double dTPos, double dSpd, int nAcc, int nDcc )
{
	CMotorInterLock_LoadTbl cMotorInterlock( eLoadTbl_1, eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_1].GetAxis(CTaskLoadTable::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_y_mov( dTPos, dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}


int MotorMoveInterLock_LoadTbl2_y( double dTPos, double dSpd, int nAcc, int nDcc )
{
	CMotorInterLock_LoadTbl cMotorInterlock( eLoadTbl_2, eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_2].GetAxis(CTaskLoadTable::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_y_mov( dTPos, dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_LoadTbl3_y(double dTPos, double dSpd, int nAcc, int nDcc)
{
	CMotorInterLock_LoadTbl cMotorInterlock(eLoadTbl_3, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_3].GetAxis(CTaskLoadTable::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_y_mov(dTPos, dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_LoadTbl4_y(double dTPos, double dSpd, int nAcc, int nDcc)
{
	CMotorInterLock_LoadTbl cMotorInterlock(eLoadTbl_4, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_4].GetAxis(CTaskLoadTable::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_y_mov(dTPos, dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_LoadTbl1_Rotate(double dTPos, double dSpd, int nAcc, int nDcc)
{
	CMotorInterLock_LoadTbl cMotorInterlock(eLoadTbl_1, eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_1].GetAxis(CTaskLoadTable::eAxisRotate);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_Rotate_mov(dTPos, dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}


int MotorMoveInterLock_LoadTbl2_Rotate(double dTPos, double dSpd, int nAcc, int nDcc)
{
	CMotorInterLock_LoadTbl cMotorInterlock(eLoadTbl_2, eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_2].GetAxis(CTaskLoadTable::eAxisRotate);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_Rotate_mov(dTPos, dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_LoadTbl3_Rotate(double dTPos, double dSpd, int nAcc, int nDcc)
{
	CMotorInterLock_LoadTbl cMotorInterlock(eLoadTbl_3, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_3].GetAxis(CTaskLoadTable::eAxisRotate);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_Rotate_mov(dTPos, dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorMoveInterLock_LoadTbl4_Rotate(double dTPos, double dSpd, int nAcc, int nDcc)
{
	CMotorInterLock_LoadTbl cMotorInterlock(eLoadTbl_4, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_4].GetAxis(CTaskLoadTable::eAxisRotate);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_Rotate_mov(dTPos, dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}


int MotorOrgInterLock_PressUnit_Dn1_z()
{
	CMotorInterlock_PressUnit cMotorInterlock( ePressUnit_Down_1 ,eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_1].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	
	return ERR_NO_ERROR;
}


int MotorOrgInterLock_PressUnit_Dn2_z()
{
	CMotorInterlock_PressUnit cMotorInterlock( ePressUnit_Down_2, eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_2].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	return ERR_NO_ERROR;
}

int MotorOrgInterLock_PressUnit_Dn3_z()
{
	CMotorInterlock_PressUnit cMotorInterlock( ePressUnit_Down_3, eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_3].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	return ERR_NO_ERROR;
}

int MotorOrgInterLock_PressUnit_Dn4_z()
{
	CMotorInterlock_PressUnit cMotorInterlock( ePressUnit_Down_4, eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_4].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	return ERR_NO_ERROR;
}


int MotorOrgInterLock_PressUnit_Dn5_z()
{
	CMotorInterlock_PressUnit cMotorInterlock( ePressUnit_Down_5, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_5].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	return ERR_NO_ERROR;
}

int MotorOrgInterLock_PressUnit_Dn6_z()
{
	CMotorInterlock_PressUnit cMotorInterlock( ePressUnit_Down_6, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_6].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	return ERR_NO_ERROR;
}

int MotorOrgInterLock_PressUnit_Dn7_z()
{
	CMotorInterlock_PressUnit cMotorInterlock(ePressUnit_Down_7, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_7].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	return ERR_NO_ERROR;
}

int MotorOrgInterLock_PressUnit_Dn8_z()
{
	CMotorInterlock_PressUnit cMotorInterlock( ePressUnit_Down_8, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_8].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	return ERR_NO_ERROR;
}

/*=============================================================================
/////////////////////////////Origin Test PP 1/////////////////////////////////
==============================================================================*/

int MotorOrgInterLock_Test_Hand1_x1()
{
	CMotorInterlock_TestPP cMotorInterlock( eTestPP_1, eTestPPLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisX1);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_x_org();
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorOrgInterLock_Test_Hand1_x2()
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_1, eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisX2);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_x_org();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorOrgInterLock_Test_Hand1_y()
{
	CMotorInterlock_TestPP cMotorInterlock( eTestPP_1 );

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_y_org();
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}


int MotorOrgInterLock_Test_Hand1_z1()
{
 	CMotorInterlock_TestPP cMotorInterlock( eTestPP_1, eTestPPLoadType);
 
 	int nErr = cMotorInterlock.CommonInterlock();
 	if (nErr != ERR_NO_ERROR) {
 		return nErr;
 	}
 
 	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisZ1);
 	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
 	if (nErr != ERR_NO_ERROR) {
 		return nErr;
 	}
 
//  	nErr = cMotorInterlock.Interlock_z_org();
//  	if( nErr != ERR_NO_ERROR ) {
//  		return nErr;
//  	}
	return ERR_NO_ERROR;
}

int MotorOrgInterLock_Test_Hand1_z2()
{
 	CMotorInterlock_TestPP cMotorInterlock(eTestPP_1, eTestPPUnLoadType);
 
 	int nErr = cMotorInterlock.CommonInterlock();
 	if (nErr != ERR_NO_ERROR) {
 		return nErr;
 	}
 
 	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisZ2);
 	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
 	if (nErr != ERR_NO_ERROR) {
 		return nErr;
 	}
 
//  	nErr = cMotorInterlock.Interlock_z_org();
//  	if (nErr != ERR_NO_ERROR) {
//  		return nErr;
//  	}
	return ERR_NO_ERROR;
}

int MotorOrgInterLock_Test_Hand1_Pitch_HW_X1_SW_Y1()
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_1, eTestPPLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisPitch_HW_X1_SW_Y1);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_xpitch_org();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorOrgInterLock_Test_Hand1_Pitch_HW_X2_SW_Y2()
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_1, eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisPitch_HW_X2_SW_Y2);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_xpitch_org();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorOrgInterLock_Test_Hand1_Pitch_HW_Y1_SW_X1()
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_1, eTestPPLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisPitch_HW_Y1_SW_X1);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_ypitch_org();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorOrgInterLock_Test_Hand1_Pitch_HW_Y2_SW_X2()
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_1, eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	//OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisPitch_HW_Y2_SW_X2);
	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisPitch_HW_Y2_SW_X2);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_ypitch_org();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

/*=============================================================================
/////////////////////////////Origin Test PP 2/////////////////////////////////
==============================================================================*/

int MotorOrgInterLock_Test_Hand2_x1()
{
	CMotorInterlock_TestPP cMotorInterlock( eTestPP_2, eTestPPLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisX1);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_x_org();
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorOrgInterLock_Test_Hand2_x2()
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_2, eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisX2);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_x_org();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorOrgInterLock_Test_Hand2_y()
{
	CMotorInterlock_TestPP cMotorInterlock( eTestPP_2 );

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_y_org();
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}



int MotorOrgInterLock_Test_Hand2_z1()
{
	CMotorInterlock_TestPP cMotorInterlock( eTestPP_2, eTestPPLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisZ1);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

// 	nErr = cMotorInterlock.Interlock_z_org();
// 	if( nErr != ERR_NO_ERROR ) {
// 		return nErr;
// 	}
	return ERR_NO_ERROR;
}

int MotorOrgInterLock_Test_Hand2_z2()
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_2, eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisZ2);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

// 	nErr = cMotorInterlock.Interlock_z_org();
// 	if (nErr != ERR_NO_ERROR) {
// 		return nErr;
// 	}
	return ERR_NO_ERROR;
}

int MotorOrgInterLock_Test_Hand2_Pitch_HW_X1_SW_Y1()
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_2, eTestPPLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisPitch_HW_X1_SW_Y1);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_xpitch_org();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorOrgInterLock_Test_Hand2_Pitch_HW_X2_SW_Y2()
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_2, eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisPitch_HW_X2_SW_Y2);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_xpitch_org();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorOrgInterLock_Test_Hand2_Pitch_HW_Y1_SW_X1()
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_2, eTestPPLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisPitch_HW_Y1_SW_X1);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_ypitch_org();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorOrgInterLock_Test_Hand2_Pitch_HW_Y2_SW_X2()
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_2, eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisPitch_HW_Y2_SW_X2);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_ypitch_org();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

/*=============================================================================
/////////////////////////////Origin Transfer/////////////////////////////////
==============================================================================*/

int MotorOrgInterLock_Transfer_X()
{

	CMotorInterlock_Transfer cMotorInterlock(eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTransfer.GetAxis(CTaskTransfer::AXIS_TRANSFER_X);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_x_org();
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}


/*=============================================================================
/////////////////////////////Origin Tray PP 1/////////////////////////////////
==============================================================================*/

int MotorOrgInterLock_CTray_Hand1_x()
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_1_LD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayLoadPP.GetAxis(CTaskTrayLoadPP::eAxisX);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_x_org();
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorOrgInterLock_CTray_Feeder()
{
	CMotorInterlock_TrayPP cMotorInterlock;

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_Feeder_org();
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorOrgInterLock_CTray_Hand1_z()
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_1_LD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayLoadPP.GetAxis(CTaskTrayLoadPP::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

// 	nErr = cMotorInterlock.Interlock_z_org();
// 	if( nErr != ERR_NO_ERROR ) {
// 		return nErr;
// 	}
	return ERR_NO_ERROR;
}

int MotorOrgInterLock_CTray_Hand1_Pitch_x()
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_1_LD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayLoadPP.GetAxis(CTaskTrayLoadPP::eAxisPitchX);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_xpitch_org();
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorOrgInterLock_CTray_Hand1_Pitch_y()
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_1_LD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayLoadPP.GetAxis(CTaskTrayLoadPP::eAxisPitchY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_ypitch_org();
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

/*=============================================================================
/////////////////////////////Origin Tray PP 2/////////////////////////////////
==============================================================================*/

int MotorOrgInterLock_CTray_Hand2_x()
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_2_UD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayUnloadPP.GetAxis(CTaskTrayUnloadPP::eAxisX);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_x_org();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorOrgInterLock_CTray_Hand2_y()
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_2_UD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayUnloadPP.GetAxis(CTaskTrayUnloadPP::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_y_org();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorOrgInterLock_CTray_Hand2_z()
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_2_UD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayUnloadPP.GetAxis(CTaskTrayUnloadPP::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

// 	nErr = cMotorInterlock.Interlock_z_org();
// 	if (nErr != ERR_NO_ERROR) {
// 		return nErr;
// 	}
	return ERR_NO_ERROR;
}

int MotorOrgInterLock_CTray_Hand2_Pitch_x()
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_2_UD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayUnloadPP.GetAxis(CTaskTrayUnloadPP::eAxisPitchX);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_xpitch_org();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorOrgInterLock_CTray_Hand2_Pitch_y()
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_2_UD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayUnloadPP.GetAxis(CTaskTrayUnloadPP::eAxisPitchY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_ypitch_org();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

/*=============================================================================
/////////////////////////////Origin Load Table/////////////////////////////////
==============================================================================*/
int MotorOrgInterLock_LoadTbl1_y()
{
	CMotorInterLock_LoadTbl cMotorInterlock( eLoadTbl_1, eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_1].GetAxis(CTaskLoadTable::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_y_org();
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}

	return ERR_NO_ERROR;
}

int MotorOrgInterLock_LoadTbl2_y()
{
	CMotorInterLock_LoadTbl cMotorInterlock( eLoadTbl_2, eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_2].GetAxis(CTaskLoadTable::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_y_org();
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}

	return ERR_NO_ERROR;
}
int MotorOrgInterLock_LoadTbl3_y()
{
	CMotorInterLock_LoadTbl cMotorInterlock(eLoadTbl_3, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_3].GetAxis(CTaskLoadTable::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_y_org();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	return ERR_NO_ERROR;
}

int MotorOrgInterLock_LoadTbl4_y()
{
	CMotorInterLock_LoadTbl cMotorInterlock(eLoadTbl_4, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_4].GetAxis(CTaskLoadTable::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_y_org();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	return ERR_NO_ERROR;
}

int MotorOrgInterLock_LoadTbl1_Rotate()
{
	CMotorInterLock_LoadTbl cMotorInterlock(eLoadTbl_1, eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_1].GetAxis(CTaskLoadTable::eAxisRotate);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_Rotate_org();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}



	return ERR_NO_ERROR;
}

int MotorOrgInterLock_LoadTbl2_Rotate()
{
	CMotorInterLock_LoadTbl cMotorInterlock(eLoadTbl_2, eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_2].GetAxis(CTaskLoadTable::eAxisRotate);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_Rotate_org();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	return ERR_NO_ERROR;
}
int MotorOrgInterLock_LoadTbl3_Rotate()
{
	CMotorInterLock_LoadTbl cMotorInterlock(eLoadTbl_3, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_3].GetAxis(CTaskLoadTable::eAxisRotate);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_Rotate_org();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	return ERR_NO_ERROR;
}

int MotorOrgInterLock_LoadTbl4_Rotate()
{
	CMotorInterLock_LoadTbl cMotorInterlock(eLoadTbl_4, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_4].GetAxis(CTaskLoadTable::eAxisRotate);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor, FALSE);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_Rotate_org();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	return ERR_NO_ERROR;
}


/*=============================================================================
/////////////////////////////Jog Press////////////////////////////////////////
==============================================================================*/

int MotorJogInterLock_PressUnit_Dn1_z( double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_PressUnit cMotorInterlock( ePressUnit_Down_1, eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_1].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_z_jog( dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}

	return ERR_NO_ERROR;
}
int MotorJogInterLock_PressUnit_Dn2_z( double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_PressUnit cMotorInterlock( ePressUnit_Down_2, eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_2].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_z_jog( dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}

	return ERR_NO_ERROR;
}
int MotorJogInterLock_PressUnit_Dn3_z( double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_PressUnit cMotorInterlock( ePressUnit_Down_3, eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_3].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_z_jog( dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}

	return ERR_NO_ERROR;
}
int MotorJogInterLock_PressUnit_Dn4_z( double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_PressUnit cMotorInterlock( ePressUnit_Down_4, eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_4].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_z_jog( dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}

	return ERR_NO_ERROR;
}
int MotorJogInterLock_PressUnit_Dn5_z( double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_PressUnit cMotorInterlock( ePressUnit_Down_5, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_5].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_z_jog( dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}

	return ERR_NO_ERROR;
}
int MotorJogInterLock_PressUnit_Dn6_z( double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_PressUnit cMotorInterlock( ePressUnit_Down_6, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_6].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_z_jog( dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}

	return ERR_NO_ERROR;
}
int MotorJogInterLock_PressUnit_Dn7_z( double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_PressUnit cMotorInterlock( ePressUnit_Down_7, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_7].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_z_jog( dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}

	return ERR_NO_ERROR;
}
int MotorJogInterLock_PressUnit_Dn8_z( double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_PressUnit cMotorInterlock( ePressUnit_Down_8, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskPressUnit[ePressUnit_Down_8].GetAxis(CTaskPressUnit::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_z_jog( dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}

	return ERR_NO_ERROR;
}
/*=============================================================================
/////////////////////////////Jog Test PP1//////////////////////////////////////
==============================================================================*/

int MotorJogInterLock_Test_Hand1_x1( double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_TestPP cMotorInterlock( eTestPP_1, eTestPPLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisX1);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_x_jog( dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorJogInterLock_Test_Hand1_x2(double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_1, eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisX2);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_x_jog(dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorJogInterLock_Test_Hand1_y( double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_TestPP cMotorInterlock( eTestPP_1 );

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_y_jog( dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}
int MotorJogInterLock_Test_Hand1_z1( double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_TestPP cMotorInterlock( eTestPP_1, eTestPPLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisZ1);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_z_jog( dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorJogInterLock_Test_Hand1_z2(double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_1, eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisZ2);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_z_jog(dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorJogInterLock_Test_Hand1_Pitch_HW_X1_SW_Y1(double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_1, eTestPPLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisPitch_HW_X1_SW_Y1);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_xpitch_jog(dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorJogInterLock_Test_Hand1_Pitch_HW_X2_SW_Y2(double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_1, eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisPitch_HW_X2_SW_Y2);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_xpitch_jog(dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorJogInterLock_Test_Hand1_Pitch_HW_Y1_SW_X1(double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_1, eTestPPLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisPitch_HW_Y1_SW_X1);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_ypitch_jog(dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorJogInterLock_Test_Hand1_Pitch_HW_Y2_SW_X2(double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_1, eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_1].GetAxis(CTaskTestPP::eAxisPitch_HW_Y2_SW_X2);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_ypitch_jog(dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}




/*=============================================================================
/////////////////////////////Jog Test PP2//////////////////////////////////////
==============================================================================*/


int MotorJogInterLock_Test_Hand2_x1( double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_TestPP cMotorInterlock( eTestPP_2, eTestPPLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisX1);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_x_jog( dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}
int MotorJogInterLock_Test_Hand2_x2(double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_2, eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisX2);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_x_jog(dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorJogInterLock_Test_Hand2_y( double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_TestPP cMotorInterlock( eTestPP_2 );

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_y_jog( dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}
int MotorJogInterLock_Test_Hand2_z1( double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_TestPP cMotorInterlock( eTestPP_2, eTestPPLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisZ1);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_z_jog( dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}
int MotorJogInterLock_Test_Hand2_z2(double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_2, eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisZ2);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_z_jog(dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}


int MotorJogInterLock_Test_Hand2_Pitch_HW_X1_SW_Y1(double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_2, eTestPPLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisPitch_HW_X1_SW_Y1);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_xpitch_jog(dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}
int MotorJogInterLock_Test_Hand2_Pitch_HW_X2_SW_Y2(double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_2, eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisPitch_HW_X2_SW_Y2);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_xpitch_jog(dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}
int MotorJogInterLock_Test_Hand2_Pitch_HW_Y1_SW_X1(double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_2, eTestPPLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisPitch_HW_Y1_SW_X1);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_ypitch_jog(dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}
int MotorJogInterLock_Test_Hand2_Pitch_HW_Y2_SW_X2(double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TestPP cMotorInterlock(eTestPP_2, eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTestPP[eTestPP_2].GetAxis(CTaskTestPP::eAxisPitch_HW_Y2_SW_X2);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_ypitch_jog(dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

/*=============================================================================
/////////////////////////////Jog Transfer/////////////////////////////////////
==============================================================================*/

int MotorJogInterLock_Transfer_X( double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_Transfer cMotorInterlock(eTestPPUnLoadType);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTransfer.GetAxis(CTaskTransfer::AXIS_TRANSFER_X);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_x_jog( dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}


// int MotorJogInterLock_Tray_Feeder_End_Stopper(double dSpd, int nAcc, int nDcc)
// {
// 	CMotorInterlock_TrayPP cMotorInterlock;
// 	int nErr = cMotorInterlock.CommonInterlock();
// 	if (nErr != ERR_NO_ERROR) {
// 		return nErr;
// 	}
// 
// 	OneAxis* pMotor = g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eAxisEndStopper);
// 	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
// 	if (nErr != ERR_NO_ERROR) {
// 		return nErr;
// 	}
// 
// 	//Stacker 3 Align Cylinder Back 확인.
// 	nErr = g_TaskStacker[eSTACKER_03].chk_cyl_TrayAlign(DEF_BACK);
// 	if (nErr != ERR_NO_ERROR) {
// 		return ERR_MOT_INTERLOCK_END_STOPPER_MOVE_NOT_STACKER_3_ALIGN_CYLINDER_FORWARD;
// 	}
// 
// 	return ERR_NO_ERROR;
// }

/*=============================================================================
/////////////////////////////Jog Tray PP 1/////////////////////////////////////
==============================================================================*/
int MotorJogInterLock_CTray_Hand1_x( double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_1_LD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayLoadPP.GetAxis(CTaskTrayLoadPP::eAxisX);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_x_jog( dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}
int MotorJogInterLock_CTray_Feeder( double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_1_LD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_Feeder_jog( dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}
int MotorJogInterLock_CTray_Hand1_z( double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_1_LD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayLoadPP.GetAxis(CTaskTrayLoadPP::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

// 	nErr = cMotorInterlock.Interlock_z_jog( dSpd, nAcc, nDcc );
// 	if( nErr != ERR_NO_ERROR ) {
// 		return nErr;
// 	}
	return ERR_NO_ERROR;
}
int MotorJogInterLock_CTray_Hand1_Pitch_x( double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_1_LD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayLoadPP.GetAxis(CTaskTrayLoadPP::eAxisPitchX);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_xpitch_jog( dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}
int MotorJogInterLock_CTray_Hand1_Pitch_y( double dSpd, int nAcc, int nDcc )
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_1_LD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayLoadPP.GetAxis(CTaskTrayLoadPP::eAxisPitchY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_ypitch_jog( dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

/*=============================================================================
/////////////////////////////Jog Tray PP 2/////////////////////////////////////
==============================================================================*/


int MotorJogInterLock_CTray_Hand2_x(double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_2_UD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayUnloadPP.GetAxis(CTaskTrayUnloadPP::eAxisX);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_x_jog(dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}
int MotorJogInterLock_CTray_Hand2_y(double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_2_UD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayUnloadPP.GetAxis(CTaskTrayUnloadPP::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_y_jog(dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}
int MotorJogInterLock_CTray_Hand2_z(double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_2_UD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayUnloadPP.GetAxis(CTaskTrayUnloadPP::eAxisZ);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

// 	nErr = cMotorInterlock.Interlock_z_jog(dSpd, nAcc, nDcc);
// 	if (nErr != ERR_NO_ERROR) {
// 		return nErr;
// 	}
	return ERR_NO_ERROR;
}
int MotorJogInterLock_CTray_Hand2_Pitch_x(double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_2_UD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayUnloadPP.GetAxis(CTaskTrayUnloadPP::eAxisPitchX);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_xpitch_jog(dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}
int MotorJogInterLock_CTray_Hand2_Pitch_y(double dSpd, int nAcc, int nDcc)
{
	CMotorInterlock_TrayPP cMotorInterlock(eTrayPP_2_UD);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskTrayUnloadPP.GetAxis(CTaskTrayUnloadPP::eAxisPitchY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_ypitch_jog(dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

/*=============================================================================
/////////////////////////////Jog LoadTable/////////////////////////////////////
==============================================================================*/

int MotorJogInterLock_LoadTbl1_y( double dSpd, int nAcc, int nDcc )
{
	CMotorInterLock_LoadTbl cMotorInterlock( eLoadTbl_1, eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_1].GetAxis(CTaskLoadTable::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_y_jog( dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}
int MotorJogInterLock_LoadTbl2_y( double dSpd, int nAcc, int nDcc )
{
	CMotorInterLock_LoadTbl cMotorInterlock( eLoadTbl_2, eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_2].GetAxis(CTaskLoadTable::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_y_jog( dSpd, nAcc, nDcc );
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
	return ERR_NO_ERROR;
}
int MotorJogInterLock_LoadTbl3_y(double dSpd, int nAcc, int nDcc)
{
	CMotorInterLock_LoadTbl cMotorInterlock(eLoadTbl_3, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_3].GetAxis(CTaskLoadTable::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_y_jog(dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}
int MotorJogInterLock_LoadTbl4_y(double dSpd, int nAcc, int nDcc)
{
	CMotorInterLock_LoadTbl cMotorInterlock(eLoadTbl_4, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_4].GetAxis(CTaskLoadTable::eAxisY);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_y_jog(dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}

int MotorJogInterLock_LoadTbl1_Rotate(double dSpd, int nAcc, int nDcc)
{
	CMotorInterLock_LoadTbl cMotorInterlock(eLoadTbl_1, eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_1].GetAxis(CTaskLoadTable::eAxisRotate);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_Rotate_jog(dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}
int MotorJogInterLock_LoadTbl2_Rotate(double dSpd, int nAcc, int nDcc)
{
	CMotorInterLock_LoadTbl cMotorInterlock(eLoadTbl_2, eTestPP_1);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_2].GetAxis(CTaskLoadTable::eAxisRotate);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_Rotate_jog(dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}
int MotorJogInterLock_LoadTbl3_Rotate(double dSpd, int nAcc, int nDcc)
{
	CMotorInterLock_LoadTbl cMotorInterlock(eLoadTbl_3, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_3].GetAxis(CTaskLoadTable::eAxisRotate);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_Rotate_jog(dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}
int MotorJogInterLock_LoadTbl4_Rotate(double dSpd, int nAcc, int nDcc)
{
	CMotorInterLock_LoadTbl cMotorInterlock(eLoadTbl_4, eTestPP_2);

	int nErr = cMotorInterlock.CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	OneAxis* pMotor = g_TaskLoadTbl[eLoadTbl_4].GetAxis(CTaskLoadTable::eAxisRotate);
	nErr = cMotorInterlock.CommonMotorInterlock(pMotor);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = cMotorInterlock.Interlock_Rotate_jog(dSpd, nAcc, nDcc);
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}
	return ERR_NO_ERROR;
}