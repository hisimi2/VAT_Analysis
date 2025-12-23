#include "StdAfx.h"
#include "TaskTestPP.h"
#include "ConfigData.h"
#include <algorithm>
#include "DEF_COMMON.h"
#include "DEF_DEVICE_DATA.h"
#include "ThreadBase.h"
#include "MotorInterlock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define LD_TABLE_DESIGN_Y_GAP (21000)	//21mm
#define BF_TABLE_DESIGN_Y_GAP (21000)   //21mm
#define CL_TABLE_DESIGN_Y_GAP (26000)	//26mm
#define STATION_DESIGN_Y_GAP  (151500)   //80mm

#define STATION_Y_GAP (7000) //7mm 
#define STATION_X_GAP (100000) //100mm

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-07]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CTaskTestPP::CTaskTestPP(void)
{
	m_nTestPPIdx = 0;
	m_bInitialized = FALSE;
	m_bCyclePause = FALSE;
	m_bCalBasePoint = FALSE;

	m_nTestPPType = eTestPPLoadType;
	for(int i=0; i<eAxisMax; i++){
		m_dMotorErrRange[i] = MOTOR_INPOSITION;
	}

	::ZeroMemory( m_nHand_i_VacSen, sizeof( m_nHand_i_VacSen ) );
	::ZeroMemory( m_nHand_o_Vac, sizeof( m_nHand_o_Vac ) );
	::ZeroMemory( m_nHand_o_Blw, sizeof( m_nHand_o_Blw ) );
	::ZeroMemory( m_nHand_o_Reset, sizeof( m_nHand_o_Reset ) );
	// DI
// 	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM11_Handler_Project_Name);
// 	if (nProjectOption == DEF_PROJECT_LOWLAND)
// 	{
		m_mapDI.SetAt(xVAC_SEN_F4_X1, "VAC_SEN_R1_X1");
		m_mapDI.SetAt(xVAC_SEN_F3_X1, "VAC_SEN_R2_X1");
		m_mapDI.SetAt(xVAC_SEN_F2_X1, "VAC_SEN_R3_X1");
		m_mapDI.SetAt(xVAC_SEN_F1_X1, "VAC_SEN_R4_X1");
		m_mapDI.SetAt(xVAC_SEN_R4_X1, "VAC_SEN_F1_X1");
		m_mapDI.SetAt(xVAC_SEN_R3_X1, "VAC_SEN_F2_X1");
		m_mapDI.SetAt(xVAC_SEN_R2_X1, "VAC_SEN_F3_X1");
		m_mapDI.SetAt(xVAC_SEN_R1_X1, "VAC_SEN_F4_X1");

		m_mapDI.SetAt(xVAC_SEN_F4_X2, "VAC_SEN_R1_X2");
		m_mapDI.SetAt(xVAC_SEN_F3_X2, "VAC_SEN_R2_X2");
		m_mapDI.SetAt(xVAC_SEN_F2_X2, "VAC_SEN_R3_X2");
		m_mapDI.SetAt(xVAC_SEN_F1_X2, "VAC_SEN_R4_X2");
		m_mapDI.SetAt(xVAC_SEN_R4_X2, "VAC_SEN_F1_X2");
		m_mapDI.SetAt(xVAC_SEN_R3_X2, "VAC_SEN_F2_X2");
		m_mapDI.SetAt(xVAC_SEN_R2_X2, "VAC_SEN_F3_X2");
		m_mapDI.SetAt(xVAC_SEN_R1_X2, "VAC_SEN_F4_X2");
// 	}
// 	else
// 	{
// 		m_mapDI.SetAt(xVAC_SEN_R1_X1, "VAC_SEN_R1_X1");
// 		m_mapDI.SetAt(xVAC_SEN_R2_X1, "VAC_SEN_R2_X1");
// 		m_mapDI.SetAt(xVAC_SEN_R3_X1, "VAC_SEN_R3_X1");
// 		m_mapDI.SetAt(xVAC_SEN_R4_X1, "VAC_SEN_R4_X1");
// 		m_mapDI.SetAt(xVAC_SEN_F1_X1, "VAC_SEN_F1_X1");
// 		m_mapDI.SetAt(xVAC_SEN_F2_X1, "VAC_SEN_F2_X1");
// 		m_mapDI.SetAt(xVAC_SEN_F3_X1, "VAC_SEN_F3_X1");
// 		m_mapDI.SetAt(xVAC_SEN_F4_X1, "VAC_SEN_F4_X1");
// 
// 		m_mapDI.SetAt(xVAC_SEN_R1_X2, "VAC_SEN_R1_X2");
// 		m_mapDI.SetAt(xVAC_SEN_R2_X2, "VAC_SEN_R2_X2");
// 		m_mapDI.SetAt(xVAC_SEN_R3_X2, "VAC_SEN_R3_X2");
// 		m_mapDI.SetAt(xVAC_SEN_R4_X2, "VAC_SEN_R4_X2");
// 		m_mapDI.SetAt(xVAC_SEN_F1_X2, "VAC_SEN_F1_X2");
// 		m_mapDI.SetAt(xVAC_SEN_F2_X2, "VAC_SEN_F2_X2");
// 		m_mapDI.SetAt(xVAC_SEN_F3_X2, "VAC_SEN_F3_X2");
// 		m_mapDI.SetAt(xVAC_SEN_F4_X2, "VAC_SEN_F4_X2");
// 	}
	m_mapDI.SetAt(xX1AXIS_POSITION_SEN, "X1AXIS_POSITION_SEN");
	m_mapDI.SetAt(xX2AXIS_POSITION_SEN, "X2AXIS_POSITION_SEN");


	// DO
// 	if (nProjectOption == DEF_PROJECT_LOWLAND)
// 	{
		m_mapDO.SetAt(yHAND_VAC_F4_X1, "HAND_VAC_R1_X1");
		m_mapDO.SetAt(yHAND_VAC_F3_X1, "HAND_VAC_R2_X1");
		m_mapDO.SetAt(yHAND_VAC_F2_X1, "HAND_VAC_R3_X1");
		m_mapDO.SetAt(yHAND_VAC_F1_X1, "HAND_VAC_R4_X1");
		m_mapDO.SetAt(yHAND_VAC_R4_X1, "HAND_VAC_F1_X1");
		m_mapDO.SetAt(yHAND_VAC_R3_X1, "HAND_VAC_F2_X1");
		m_mapDO.SetAt(yHAND_VAC_R2_X1, "HAND_VAC_F3_X1");
		m_mapDO.SetAt(yHAND_VAC_R1_X1, "HAND_VAC_F4_X1");


		m_mapDO.SetAt(yHAND_VAC_F4_X2, "HAND_VAC_R1_X2");
		m_mapDO.SetAt(yHAND_VAC_F3_X2, "HAND_VAC_R2_X2");
		m_mapDO.SetAt(yHAND_VAC_F2_X2, "HAND_VAC_R3_X2");
		m_mapDO.SetAt(yHAND_VAC_F1_X2, "HAND_VAC_R4_X2");
		m_mapDO.SetAt(yHAND_VAC_R4_X2, "HAND_VAC_F1_X2");
		m_mapDO.SetAt(yHAND_VAC_R3_X2, "HAND_VAC_F2_X2");
		m_mapDO.SetAt(yHAND_VAC_R2_X2, "HAND_VAC_F3_X2");
		m_mapDO.SetAt(yHAND_VAC_R1_X2, "HAND_VAC_F4_X2");


		m_mapDO.SetAt(yHAND_BLW_F4_X1, "HAND_BLW_R1_X1");
		m_mapDO.SetAt(yHAND_BLW_F3_X1, "HAND_BLW_R2_X1");
		m_mapDO.SetAt(yHAND_BLW_F2_X1, "HAND_BLW_R3_X1");
		m_mapDO.SetAt(yHAND_BLW_F1_X1, "HAND_BLW_R4_X1");
		m_mapDO.SetAt(yHAND_BLW_R4_X1, "HAND_BLW_F1_X1");
		m_mapDO.SetAt(yHAND_BLW_R3_X1, "HAND_BLW_F2_X1");
		m_mapDO.SetAt(yHAND_BLW_R2_X1, "HAND_BLW_F3_X1");
		m_mapDO.SetAt(yHAND_BLW_R1_X1, "HAND_BLW_F4_X1");

		m_mapDO.SetAt(yHAND_BLW_F4_X2, "HAND_BLW_R1_X2");
		m_mapDO.SetAt(yHAND_BLW_F3_X2, "HAND_BLW_R2_X2");
		m_mapDO.SetAt(yHAND_BLW_F2_X2, "HAND_BLW_R3_X2");
		m_mapDO.SetAt(yHAND_BLW_F1_X2, "HAND_BLW_R4_X2");
		m_mapDO.SetAt(yHAND_BLW_R4_X2, "HAND_BLW_F1_X2");
		m_mapDO.SetAt(yHAND_BLW_R3_X2, "HAND_BLW_F2_X2");
		m_mapDO.SetAt(yHAND_BLW_R2_X2, "HAND_BLW_F3_X2");
		m_mapDO.SetAt(yHAND_BLW_R1_X2, "HAND_BLW_F4_X2");

		m_mapDO.SetAt(yHAND_RESET_F4_X1, "HAND_RESET_R1_X1");
		m_mapDO.SetAt(yHAND_RESET_F3_X1, "HAND_RESET_R2_X1");
		m_mapDO.SetAt(yHAND_RESET_F2_X1, "HAND_RESET_R3_X1");
		m_mapDO.SetAt(yHAND_RESET_F1_X1, "HAND_RESET_R4_X1");
		m_mapDO.SetAt(yHAND_RESET_R4_X1, "HAND_RESET_F1_X1");
		m_mapDO.SetAt(yHAND_RESET_R3_X1, "HAND_RESET_F2_X1");
		m_mapDO.SetAt(yHAND_RESET_R2_X1, "HAND_RESET_F3_X1");
		m_mapDO.SetAt(yHAND_RESET_R1_X1, "HAND_RESET_F4_X1");

		m_mapDO.SetAt(yHAND_RESET_F4_X2, "HAND_RESET_R1_X2");
		m_mapDO.SetAt(yHAND_RESET_F3_X2, "HAND_RESET_R2_X2");
		m_mapDO.SetAt(yHAND_RESET_F2_X2, "HAND_RESET_R3_X2");
		m_mapDO.SetAt(yHAND_RESET_F1_X2, "HAND_RESET_R4_X2");
		m_mapDO.SetAt(yHAND_RESET_R4_X2, "HAND_RESET_F1_X2");
		m_mapDO.SetAt(yHAND_RESET_R3_X2, "HAND_RESET_F2_X2");
		m_mapDO.SetAt(yHAND_RESET_R2_X2, "HAND_RESET_F3_X2");
		m_mapDO.SetAt(yHAND_RESET_R1_X2, "HAND_RESET_F4_X2");
// 	}
// 	else
// 	{
// 		m_mapDO.SetAt(yHAND_VAC_R1_X1, "HAND_VAC_R1_X1");
// 		m_mapDO.SetAt(yHAND_VAC_R2_X1, "HAND_VAC_R2_X1");
// 		m_mapDO.SetAt(yHAND_VAC_R3_X1, "HAND_VAC_R3_X1");
// 		m_mapDO.SetAt(yHAND_VAC_R4_X1, "HAND_VAC_R4_X1");
// 		m_mapDO.SetAt(yHAND_VAC_F1_X1, "HAND_VAC_F1_X1");
// 		m_mapDO.SetAt(yHAND_VAC_F2_X1, "HAND_VAC_F2_X1");
// 		m_mapDO.SetAt(yHAND_VAC_F3_X1, "HAND_VAC_F3_X1");
// 		m_mapDO.SetAt(yHAND_VAC_F4_X1, "HAND_VAC_F4_X1");
// 
// 		m_mapDO.SetAt(yHAND_VAC_R1_X2, "HAND_VAC_R1_X2");
// 		m_mapDO.SetAt(yHAND_VAC_R2_X2, "HAND_VAC_R2_X2");
// 		m_mapDO.SetAt(yHAND_VAC_R3_X2, "HAND_VAC_R3_X2");
// 		m_mapDO.SetAt(yHAND_VAC_R4_X2, "HAND_VAC_R4_X2");
// 		m_mapDO.SetAt(yHAND_VAC_F1_X2, "HAND_VAC_F1_X2");
// 		m_mapDO.SetAt(yHAND_VAC_F2_X2, "HAND_VAC_F2_X2");
// 		m_mapDO.SetAt(yHAND_VAC_F3_X2, "HAND_VAC_F3_X2");
// 		m_mapDO.SetAt(yHAND_VAC_F4_X2, "HAND_VAC_F4_X2");
// 
// 		m_mapDO.SetAt(yHAND_BLW_R1_X1, "HAND_BLW_R1_X1");
// 		m_mapDO.SetAt(yHAND_BLW_R2_X1, "HAND_BLW_R2_X1");
// 		m_mapDO.SetAt(yHAND_BLW_R3_X1, "HAND_BLW_R3_X1");
// 		m_mapDO.SetAt(yHAND_BLW_R4_X1, "HAND_BLW_R4_X1");
// 		m_mapDO.SetAt(yHAND_BLW_F1_X1, "HAND_BLW_F1_X1");
// 		m_mapDO.SetAt(yHAND_BLW_F2_X1, "HAND_BLW_F2_X1");
// 		m_mapDO.SetAt(yHAND_BLW_F3_X1, "HAND_BLW_F3_X1");
// 		m_mapDO.SetAt(yHAND_BLW_F4_X1, "HAND_BLW_F4_X1");
// 
// 		m_mapDO.SetAt(yHAND_BLW_R1_X2, "HAND_BLW_R1_X2");
// 		m_mapDO.SetAt(yHAND_BLW_R2_X2, "HAND_BLW_R2_X2");
// 		m_mapDO.SetAt(yHAND_BLW_R3_X2, "HAND_BLW_R3_X2");
// 		m_mapDO.SetAt(yHAND_BLW_R4_X2, "HAND_BLW_R4_X2");
// 		m_mapDO.SetAt(yHAND_BLW_F1_X2, "HAND_BLW_F1_X2");
// 		m_mapDO.SetAt(yHAND_BLW_F2_X2, "HAND_BLW_F2_X2");
// 		m_mapDO.SetAt(yHAND_BLW_F3_X2, "HAND_BLW_F3_X2");
// 		m_mapDO.SetAt(yHAND_BLW_F4_X2, "HAND_BLW_F4_X2");
// 
// 		m_mapDO.SetAt(yHAND_RESET_R1_X1, "HAND_RESET_R1_X1");
// 		m_mapDO.SetAt(yHAND_RESET_R2_X1, "HAND_RESET_R2_X1");
// 		m_mapDO.SetAt(yHAND_RESET_R3_X1, "HAND_RESET_R3_X1");
// 		m_mapDO.SetAt(yHAND_RESET_R4_X1, "HAND_RESET_R4_X1");
// 		m_mapDO.SetAt(yHAND_RESET_F1_X1, "HAND_RESET_F1_X1");
// 		m_mapDO.SetAt(yHAND_RESET_F2_X1, "HAND_RESET_F2_X1");
// 		m_mapDO.SetAt(yHAND_RESET_F3_X1, "HAND_RESET_F3_X1");
// 		m_mapDO.SetAt(yHAND_RESET_F4_X1, "HAND_RESET_F4_X1");
// 
// 		m_mapDO.SetAt(yHAND_RESET_R1_X2, "HAND_RESET_R1_X2");
// 		m_mapDO.SetAt(yHAND_RESET_R2_X2, "HAND_RESET_R2_X2");
// 		m_mapDO.SetAt(yHAND_RESET_R3_X2, "HAND_RESET_R3_X2");
// 		m_mapDO.SetAt(yHAND_RESET_R4_X2, "HAND_RESET_R4_X2");
// 		m_mapDO.SetAt(yHAND_RESET_F1_X2, "HAND_RESET_F1_X2");
// 		m_mapDO.SetAt(yHAND_RESET_F2_X2, "HAND_RESET_F2_X2");
// 		m_mapDO.SetAt(yHAND_RESET_F3_X2, "HAND_RESET_F3_X2");
// 		m_mapDO.SetAt(yHAND_RESET_F4_X2, "HAND_RESET_F4_X2");
// 	}
// Motor
	m_mapMotor.SetAt(eAxisX1, "Axis X1");
	m_mapMotor.SetAt(eAxisX2, "Axis X2");
	m_mapMotor.SetAt(eAxisY, "Axis Y");
	m_mapMotor.SetAt(eAxisZ1, "Axis Z1");
	m_mapMotor.SetAt(eAxisZ2, "Axis Z2");
	m_mapMotor.SetAt(eAxisPitch_HW_Y1_SW_X1, "Axis Pitch X1");
	m_mapMotor.SetAt(eAxisPitch_HW_Y2_SW_X2, "Axis Pitch X2");
	m_mapMotor.SetAt(eAxisPitch_HW_X1_SW_Y1, "Axis Pitch Y1");
	m_mapMotor.SetAt(eAxisPitch_HW_X2_SW_Y2, "Axis Pitch Y2");

	m_vModuleSetLoadTableIdx.clear();
	
	
	//VAT
	m_hLog = NULL;

	::ZeroMemory(m_nBbxmnpAxis, sizeof(m_nBbxmnpAxis));
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-07]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CTaskTestPP::~CTaskTestPP(void)
{
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-07]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskTestPP::Initialize(int nTestPPIdx, char* szDriverFileName )
{
	m_nTestPPIdx = nTestPPIdx;

	CConfigData ConfigData(szDriverFileName);
	char szSection[128] = {0,};

	// LOG File Name
	char szLogFileName[ 512 ] = { 0, };
	char szBuff[512] = {0, };
	char szDefaultValue[128] = {0,};

	memset(szSection, 0x00, sizeof(szSection));
	sprintf_s(szSection, sizeof(szSection), "TASK TESTPP %d CONFIG", nTestPPIdx+1);

	memset(szDefaultValue, 0x00, sizeof(szDefaultValue));
	sprintf_s(szDefaultValue, sizeof(szDefaultValue), "TaskTestPP_%d.log", nTestPPIdx+1);

	ConfigData.GetString( szSection, "Log File Name", szDefaultValue, szBuff);
	sprintf_s(szLogFileName, sizeof(szLogFileName), "%s%s", SZ_LOG_FILE_PATH, szBuff);
	m_Logger.SetFileName(szLogFileName);


	// Digital Input
	memset(szSection, 0x00, sizeof(szSection));
	sprintf_s(szSection, sizeof(szSection), "TASK TESTPP %d CONFIG - DI", nTestPPIdx+1);
	int nDiCount = m_mapDI.GetCount();
	for(int i=0; i<nDiCount; i++){
		char szKey[128] = {0,};
		CString strName;
		m_mapDI.Lookup(i, strName);
		sprintf_s(szKey, sizeof(szKey), "%s", strName.GetString() );
		int nIdx = ConfigData.GetInt(szSection, szKey, "0");
		m_vX.push_back(nIdx);
	}

	// Digital Output
	memset(szSection, 0x00, sizeof(szSection));
	sprintf_s(szSection, sizeof(szSection), "TASK TESTPP %d CONFIG - DO", nTestPPIdx+1);
	int nDoCount = m_mapDO.GetCount();
	for(int i=0; i<nDoCount; i++){
		char szKey[128] = {0,};
		CString strName;
		m_mapDO.Lookup(i, strName);
		sprintf_s(szKey, sizeof(szKey), "%s", strName.GetString() );
		int nIdx = ConfigData.GetInt(szSection, szKey, "0");
		m_vY.push_back(nIdx);
	}

	// Motor
	memset(szSection, 0x00, sizeof(szSection));
	sprintf_s(szSection, sizeof(szSection), "TASK TESTPP %d CONFIG - MOTOR", nTestPPIdx+1);
	int nMotorCount = m_mapMotor.GetCount();
	for(int i=0; i<nMotorCount; i++){
		char szKey[128] = {0,};
		CString strName;
		m_mapMotor.Lookup(i, strName);
		sprintf_s(szKey, sizeof(szKey), "%s", strName.GetString() );
		int nIdx = ConfigData.GetInt(szSection, szKey, "0");
		OneAxis* pMotor = g_Motor[nIdx];
		m_vMotor.push_back(pMotor);
	}


	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);

	if (nBoardType == eBoardDir_Backward)
	{
		// Load PP
		for (int y = 0; y < MAX_TEST_PP_PKR_CNT_Y; y++) {
			for (int x = 0; x < MAX_TEST_PP_PKR_CNT_X; x++) {
				m_nHand_i_VacSen[eTestPPLoadType][y][x] = m_vX[(xVAC_SEN_R1_X1 + (y*MAX_TEST_PP_PKR_CNT_X) + x)];

				m_nHand_o_Vac[eTestPPLoadType][y][x] = m_vY[yHAND_RESET_R1_X1 + (y*MAX_TEST_PP_PKR_CNT_X) + x];
				m_nHand_o_Blw[eTestPPLoadType][y][x] = m_vY[yHAND_BLW_R1_X1 + (y*MAX_TEST_PP_PKR_CNT_X) + x];
				m_nHand_o_Reset[eTestPPLoadType][y][x] = m_vY[yHAND_VAC_R1_X1 + (y*MAX_TEST_PP_PKR_CNT_X) + x];
			}
		}

		// UnLoad PP
		for (int y = 0; y < MAX_TEST_PP_PKR_CNT_Y; y++) {
			for (int x = 0; x < MAX_TEST_PP_PKR_CNT_X; x++) {
				m_nHand_i_VacSen[eTestPPUnLoadType][y][x] = m_vX[(xVAC_SEN_R1_X2 + (y*MAX_TEST_PP_PKR_CNT_X) + x)];

				m_nHand_o_Vac[eTestPPUnLoadType][y][x] = m_vY[yHAND_RESET_R1_X2 + (y*MAX_TEST_PP_PKR_CNT_X) + x];
				m_nHand_o_Blw[eTestPPUnLoadType][y][x] = m_vY[yHAND_BLW_R1_X2 + (y*MAX_TEST_PP_PKR_CNT_X) + x];
				m_nHand_o_Reset[eTestPPUnLoadType][y][x] = m_vY[yHAND_VAC_R1_X2 + (y*MAX_TEST_PP_PKR_CNT_X) + x];
			}
		}
	}
	else {
		// Load PP
		int nCount = MAX_TEST_PP_PKR_CNT_Y*MAX_TEST_PP_PKR_CNT_X - 1;
		for (int y = 0; y < MAX_TEST_PP_PKR_CNT_Y; y++) {
			for (int x = 0; x < MAX_TEST_PP_PKR_CNT_X; x++) {
				m_nHand_i_VacSen[eTestPPLoadType][y][x] = m_vX[(xVAC_SEN_R1_X1 + abs((y*MAX_TEST_PP_PKR_CNT_X) + x - nCount))];

				m_nHand_o_Vac[eTestPPLoadType][y][x] = m_vY[(yHAND_RESET_R1_X1 + abs((y*MAX_TEST_PP_PKR_CNT_X) + x - nCount))];
				m_nHand_o_Blw[eTestPPLoadType][y][x] = m_vY[(yHAND_BLW_R1_X1 + abs((y*MAX_TEST_PP_PKR_CNT_X) + x - nCount))];
				m_nHand_o_Reset[eTestPPLoadType][y][x] = m_vY[(yHAND_VAC_R1_X1 + abs((y*MAX_TEST_PP_PKR_CNT_X) + x - nCount))];
			}
		}

		// UnLoad PP
		for (int y = 0; y < MAX_TEST_PP_PKR_CNT_Y; y++) {
			for (int x = 0; x < MAX_TEST_PP_PKR_CNT_X; x++) {
				m_nHand_i_VacSen[eTestPPUnLoadType][y][x] = m_vX[(xVAC_SEN_R1_X2 + abs((y*MAX_TEST_PP_PKR_CNT_X) + x - nCount))];

				m_nHand_o_Vac[eTestPPUnLoadType][y][x] = m_vY[(yHAND_RESET_R1_X2 + abs((y*MAX_TEST_PP_PKR_CNT_X) + x - nCount))];
				m_nHand_o_Blw[eTestPPUnLoadType][y][x] = m_vY[(yHAND_BLW_R1_X2 + abs((y*MAX_TEST_PP_PKR_CNT_X) + x - nCount))];
				m_nHand_o_Reset[eTestPPUnLoadType][y][x] = m_vY[(yHAND_VAC_R1_X2 + abs((y*MAX_TEST_PP_PKR_CNT_X) + x - nCount))];
			}
		}

	}
	

	GetUserOffsetData();
	GetTeachingPos();

	
	if (m_nTestPPIdx == eTestPP_1)
	{
		m_vModuleSetLoadTableIdx.push_back(eLoadTbl_1);
		m_vModuleSetLoadTableIdx.push_back(eLoadTbl_2);
	}
	else
	{
		m_vModuleSetLoadTableIdx.push_back(eLoadTbl_3);
		m_vModuleSetLoadTableIdx.push_back(eLoadTbl_4);
	}

	//VAT
	for (int hand_type = 0; hand_type < VAT_TESTPP_HAND_TYPE; hand_type++) {
		int nStartPkr = -1;
		if (m_nTestPPIdx == eTestPP_1) {
			nStartPkr = (hand_type == VAT_TESTPP_LOAD) ? eBMAxis_LOAD_TESTPP_1_Picker01 : eBMAxis_UNLOAD_TESTPP_1_Picker01;
		}
		else {
			nStartPkr = (hand_type == VAT_TESTPP_LOAD) ? eBMAxis_LOAD_TESTPP_2_Picker01 : eBMAxis_UNLOAD_TESTPP_2_Picker01;
		}

		//Picker 8ea 저장.
		for (int pkr = VAT_PICKER_01; pkr < VAT_PICKER_MAX; pkr++) {
			m_nBbxmnpAxis[hand_type][pkr] = nStartPkr + pkr;
		}
	}

	return CBaseTaskManager::Initialize();
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-07]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CTaskTestPP::Finalize()
{
	CBaseTaskManager::Finalize();
}

//==============================================================================
// 도착한 명령을 큐에서 꺼내어 실행 시킨다.
//==============================================================================
void CTaskTestPP::ThreadProc1()
{
	SetThreadInterval(0, 50);
	while( GetThreadAliveFlag(0) ){
		if( GetPauseFlag(0) ) { 
			NEXT;
			continue;
		}
		for(int i=0; i<MAX_THREAD_SCAN_COUNT; i++){ 
			CBaseTaskManager::SeqCmdRunning(); 
		}
		NEXT;
	}
}

void CTaskTestPP::OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{
	switch(nOperatorCommand)
	{
	case TWMB_ID_SKIP:
	case TWMB_ID_P_SKIP:
		{
			SetProcessCommand(base_seq_cmd::eProcCmd_Skip);
		}break;
	}
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-07]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskTestPP::OnEvent(const CBaseTaskManager* pSender,int nEventId, CParamList* pDataList/*=NULL*/)
{
	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-07]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
OneAxis* CTaskTestPP::GetAxis(int nAxisIdx)
{
	ASSERT( (nAxisIdx >=0 ) && (nAxisIdx < CTaskTestPP::eAxisMax) );
	return m_vMotor[nAxisIdx];
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-07]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskTestPP::GetPickerOffset(int nLocStage, std::vector<CPoint> vPicker, double* pdOffsetX, double* pdOffsetY)
{
	double dPickerDist[2] = { 0, };
	int nHandMatrix[MAX_TEST_PP_PKR_CNT_Y][MAX_TEST_PP_PKR_CNT_X];
	// Matrix 0로 초기화
	memset(nHandMatrix, 0x00, sizeof(nHandMatrix));
	
	dPickerDist[0] = g_DMCont.m_dmTestPP.GN(NDM4_TestPP_Pitch_X);
	dPickerDist[1] = g_DMCont.m_dmTestPP.GN(NDM4_TestPP_Pitch_Y);
		
	if( !vPicker.empty() )
	{
		for(std::vector<CPoint>::size_type i=0; i<vPicker.size(); i++){
			int x = vPicker[i].x;
			int y = vPicker[i].y;
			nHandMatrix[y][x] = 1; 
		}
		// hand의 Rear Picker 전체가 비었으면 X축 pitch 만큼 더 이동 시킨다.
		int nSum = 0;
		for(int i=0; i<MAX_TEST_PP_PKR_CNT_X; i++){
			nSum += nHandMatrix[0][i];
		}
		if( nSum == 0 ){
			/**pdOffsetY += dPickerDist[1];*/
			if (nLocStage >= LOC_IDX_TEST_PP_TBL_1_LD && nLocStage <= LOC_IDX_TEST_PP_TBL_4_UD)
			{
				if (g_TaskTestPP[m_nTestPPIdx].GetEqpCtrlMode() == eEqpCtrlMode_Manual)
				{
					*pdOffsetX += dPickerDist[1] / 2; // Load Table 영억이면 Y Picker Pitch를 2로 나눠준다. 실제 Motor X축으로 변환					
				}
				else
				{
					*pdOffsetX += dPickerDist[1]; // Y Picker Pitch를 실제 Motor X축으로 변환
				}
				
			}			
			else
				*pdOffsetX += dPickerDist[1]; // Y Picker Pitch를 실제 Motor X축으로 변환
		
		}

		int nOffSetXPos = 0;
		// hand의 1열~8열까지 비었으면 X축 pitch 만큼 더 이동 시킨다.
		for(int x=0; x<MAX_TEST_PP_PKR_CNT_X; x++){
			nSum = 0;
			BOOL bStopCalc = FALSE;
			for(int y=0; y<MAX_TEST_PP_PKR_CNT_Y; y++){
				nSum += nHandMatrix[y][x];

				if(nHandMatrix[y][x]){ // Left 부터 확인하여 둘다 없는 경우가 아니라면 빠져 나가야 한다.
					bStopCalc = TRUE;
				}
			}

			if(bStopCalc == TRUE){
				nOffSetXPos = x;
				break;
			}

			if( nSum == 0 ){
				*pdOffsetY -= dPickerDist[0]; // X Picker Pitch를 실제 Motor Y축으로 변환
			}
		}
	}

	return ERR_NO_ERROR;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-07]
  NAME     : 
  PARAMS   : nTargetX : 0 ~ max x count
             nTargetY : 0 ~ max y count
             nUpDown  : 0 = Place, 1 = Pick, 2 = Safety
  RETURN   : 
  COMMENTS : Test PP의 Picker Rear Left 기준.
==============================================================================*/
int CTaskTestPP::GetTeachPos(int nLocStage, CPoint Pocket, std::vector<CPoint> vPicker, int nLocZ,
	double* pdPosX1, double* pdPosY1, double* pdPosZ1, double* pdPitchX1, double* pdPitchY1,
	double* pdPosX2, double* pdPosY2, double* pdPosZ2, double* pdPitchX2, double* pdPitchY2, int nTestHandType, BOOL bCleanOffsetUse)
{
	ASSERT(nLocStage >= 0 && nLocStage < MAX_TEST_PP_LOC_COUNT);
	ASSERT(nLocZ >= 0 && nLocZ < CTaskTestPP::eMAX_ZPOS_COUNT);

	double dTPos[CTaskTestPP::ePos_Max] = { 0.0, };

	dTPos[CTaskTestPP::ePos_AxisX1] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_X1);
	dTPos[CTaskTestPP::ePos_AxisX2] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_X2);

	dTPos[CTaskTestPP::ePos_AxisY1] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_Y1);
	dTPos[CTaskTestPP::ePos_AxisY2] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_Y2);

	if (nLocZ == eZPOS_PICK) {
		dTPos[CTaskTestPP::ePos_AxisZ1] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_Z1_PICK);
		dTPos[CTaskTestPP::ePos_AxisZ2] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_Z2_PICK);
	}
	else if (nLocZ == eZPOS_PLACE) {
		dTPos[CTaskTestPP::ePos_AxisZ1] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_Z1_PLACE);
		dTPos[CTaskTestPP::ePos_AxisZ2] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_Z2_PLACE);
	}
	else {
		dTPos[CTaskTestPP::ePos_AxisZ1] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_Z1_SAFETY);
		dTPos[CTaskTestPP::ePos_AxisZ2] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_Z2_SAFETY);
	}

	//[작업 필요] teaching pos 나오면 확인.
	dTPos[CTaskTestPP::ePos_AxisPitchX1] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_X1_PITCH); //POS_IDX_TEST_PP_X_PITCH
	dTPos[CTaskTestPP::ePos_AxisPitchY1] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_Y1_PITCH); //POS_IDX_TEST_PP_X_PITCH

	dTPos[CTaskTestPP::ePos_AxisPitchX2] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_X2_PITCH); //POS_IDX_TEST_PP_X_PITCH
	dTPos[CTaskTestPP::ePos_AxisPitchY2] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_Y2_PITCH); //POS_IDX_TEST_PP_X_PITCH


	// 1. Picker Rear Left( 좌상)을 기준으로 거리 계산
	double dPocketDist[2] = { 0, };
	double dTargetPosPitch[2] = { 0, };


	if (nLocStage >= LOC_IDX_TEST_PP_TBL_1_LD && nLocStage <= LOC_IDX_TEST_PP_TBL_4_UD)
	{
		dTargetPosPitch[CTaskTestPP::ePos_AxisX1] = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_X);
		dTargetPosPitch[CTaskTestPP::ePos_AxisY1] = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_Y);
	}
	else if (nLocStage == LOC_IDX_CLEAN_TBL)
	{
		// 		dTargetPosPitch[CTaskTestPP::ePos_AxisX1] = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Pitch_X);
		// 		dTargetPosPitch[CTaskTestPP::ePos_AxisY1] = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Pitch_Y);
	}
	else if (nLocStage == LOC_IDX_TEST_PP_HOME) {
	}
	else //LOC_IDX_STATION_1 ~ LOC_IDX_STATION_4
	{
		dTargetPosPitch[CTaskTestPP::ePos_AxisX1] = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Pitch_X);
		dTargetPosPitch[CTaskTestPP::ePos_AxisY1] = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Pitch_Y);
	}


	//Pocket 계산
	//int nProjectOption = g_DMCont.m_dmEQP.GN(NDM11_Handler_Project_Name);
	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
	if (nBoardType == eBoardDir_Forward)
	{
		if (m_nTestPPIdx == eTestPP_1)
		{
			dPocketDist[CTaskTestPP::ePos_AxisY1] += (dTargetPosPitch[CTaskTestPP::ePos_AxisX1] * Pocket.x);
			dPocketDist[CTaskTestPP::ePos_AxisX1] += (dTargetPosPitch[CTaskTestPP::ePos_AxisY1] * Pocket.y);
		}
		else
		{
			dPocketDist[CTaskTestPP::ePos_AxisY1] -= (dTargetPosPitch[CTaskTestPP::ePos_AxisX1] * Pocket.x);
			dPocketDist[CTaskTestPP::ePos_AxisX1] -= (dTargetPosPitch[CTaskTestPP::ePos_AxisY1] * Pocket.y);
		}
	}
	else
	{
		if (m_nTestPPIdx == eTestPP_1)
		{
			dPocketDist[CTaskTestPP::ePos_AxisY1] -= (dTargetPosPitch[CTaskTestPP::ePos_AxisX1] * Pocket.x);
			dPocketDist[CTaskTestPP::ePos_AxisX1] -= (dTargetPosPitch[CTaskTestPP::ePos_AxisY1] * Pocket.y);
		}
		else
		{
			dPocketDist[CTaskTestPP::ePos_AxisY1] += (dTargetPosPitch[CTaskTestPP::ePos_AxisX1] * Pocket.x);
			dPocketDist[CTaskTestPP::ePos_AxisX1] += (dTargetPosPitch[CTaskTestPP::ePos_AxisY1] * Pocket.y);
		}
	}

	if (nLocStage >= LOC_IDX_STATION_1 && nLocStage <= LOC_IDX_STATION_4) {
	}
	else
	{
		// Picker 감안하여 Offset 계산.
		dTPos[CTaskTestPP::ePos_AxisX1] += dPocketDist[CTaskTestPP::ePos_AxisX1];
		dTPos[CTaskTestPP::ePos_AxisY1] += dPocketDist[CTaskTestPP::ePos_AxisY1];

		dTPos[CTaskTestPP::ePos_AxisX2] += dPocketDist[CTaskTestPP::ePos_AxisX1];
		dTPos[CTaskTestPP::ePos_AxisY2] += dPocketDist[CTaskTestPP::ePos_AxisY1];
	}


	// 2. 집어야 할 Picker 감안하여 Offset 계산.
	double dPickerOffset[2] = { 0, };
	if ((LOC_IDX_TEST_PP_TBL_1_LD <= nLocStage && nLocStage <= LOC_IDX_TEST_PP_TBL_4_UD/*&& nLocStage <= LOC_IDX_CLEAN_TBL*/))
	{
		GetPickerOffset(nLocStage, vPicker, &dPickerOffset[0], &dPickerOffset[1]);
	}

	if (nLocStage == LOC_IDX_TEST_PP_HOME) { // Safety 위치는 Pocket 계산 제거
	}
	else
	{
		//int nProjectOption = g_DMCont.m_dmEQP.GN(NDM11_Handler_Project_Name);
		int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
		if (nBoardType == eBoardDir_Forward)
		{
			if (m_nTestPPIdx == eTestPP_1)
			{
				dTPos[CTaskTestPP::ePos_AxisX1] -= dPickerOffset[CTaskTestPP::ePos_AxisX1];
				dTPos[CTaskTestPP::ePos_AxisX2] -= dPickerOffset[CTaskTestPP::ePos_AxisX1];

				dTPos[CTaskTestPP::ePos_AxisY1] += dPickerOffset[CTaskTestPP::ePos_AxisY1];
				dTPos[CTaskTestPP::ePos_AxisY2] += dPickerOffset[CTaskTestPP::ePos_AxisY1];
			}
			else
			{
				dTPos[CTaskTestPP::ePos_AxisX1] += dPickerOffset[CTaskTestPP::ePos_AxisX1];
				dTPos[CTaskTestPP::ePos_AxisX2] += dPickerOffset[CTaskTestPP::ePos_AxisX1];

				dTPos[CTaskTestPP::ePos_AxisY1] -= dPickerOffset[CTaskTestPP::ePos_AxisY1];
				dTPos[CTaskTestPP::ePos_AxisY2] -= dPickerOffset[CTaskTestPP::ePos_AxisY1];
			}
		}
		else
		{
			if (m_nTestPPIdx == eTestPP_1)
			{
				dTPos[CTaskTestPP::ePos_AxisX1] += dPickerOffset[CTaskTestPP::ePos_AxisX1];
				dTPos[CTaskTestPP::ePos_AxisX2] += dPickerOffset[CTaskTestPP::ePos_AxisX1];

				dTPos[CTaskTestPP::ePos_AxisY1] -= dPickerOffset[CTaskTestPP::ePos_AxisY1];
				dTPos[CTaskTestPP::ePos_AxisY2] -= dPickerOffset[CTaskTestPP::ePos_AxisY1];
			}
			else
			{
				dTPos[CTaskTestPP::ePos_AxisX1] -= dPickerOffset[CTaskTestPP::ePos_AxisX1];
				dTPos[CTaskTestPP::ePos_AxisX2] -= dPickerOffset[CTaskTestPP::ePos_AxisX1];

				dTPos[CTaskTestPP::ePos_AxisY1] += dPickerOffset[CTaskTestPP::ePos_AxisY1];
				dTPos[CTaskTestPP::ePos_AxisY2] += dPickerOffset[CTaskTestPP::ePos_AxisY1];
			}
		}
	}
	

	// user off set ( only test site )


	//Clean Offset 추가
	if (nLocStage >= LOC_IDX_STATION_1 && nLocStage <= LOC_IDX_STATION_4)
	{
		if (bCleanOffsetUse == eCleanOffsetUse)
		{
			double dUserOffSet[CTaskTestPP::ePos_Max] = { 0.0, };
			dUserOffSet[CTaskTestPP::ePos_AxisX1] = (double)m_stTeachPp.stTestSite[nLocStage - LOC_IDX_STATION_1].nOffSet[eTestPpOffSet_X];
			dUserOffSet[CTaskTestPP::ePos_AxisY1] = (double)m_stTeachPp.stTestSite[nLocStage - LOC_IDX_STATION_1].nOffSet[eTestPpOffSet_Y];

			if (nLocZ == eZPOS_PICK) {
				dUserOffSet[CTaskTestPP::ePos_AxisZ1] = (double)m_stTeachPp.stTestSite[nLocStage - LOC_IDX_STATION_1].nOffSet[eTestPpOffSet_Z_Pick];
			}
			else if (nLocZ == eZPOS_PLACE) {
				dUserOffSet[CTaskTestPP::ePos_AxisZ1] = (double)m_stTeachPp.stTestSite[nLocStage - LOC_IDX_STATION_1].nOffSet[eTestPpOffSet_Z_Place];
			}

			dTPos[CTaskTestPP::ePos_AxisX1] += dUserOffSet[CTaskTestPP::ePos_AxisX1];
			dTPos[CTaskTestPP::ePos_AxisY1] += dUserOffSet[CTaskTestPP::ePos_AxisY1];
			dTPos[CTaskTestPP::ePos_AxisZ1] += dUserOffSet[CTaskTestPP::ePos_AxisZ1];

		}
	}
// 	if (nLocStage >= LOC_IDX_STATION_1 && nLocStage <= LOC_IDX_STATION_4) {
// 		// Front Site Offset
// 		//TWSL421 8Picker로 1Pos Offset만 있으면 됨.
// 		//Load Hand, Unload Hand Offset 필요
// 		//[작업 필요]offset gui 작업 필요.
// 		if (Pocket.y == 0) { //Pocket.y == 1
// 			double dUserOffSet[CTaskTestPP::ePos_Max] = { 0.0, };
// 
// 			dUserOffSet[CTaskTestPP::ePos_AxisX1] = (double)m_stTeachPp.stTestSite[nLocStage - LOC_IDX_STATION_1].nOffSet[eTestPpOffSet_X];
// 			dUserOffSet[CTaskTestPP::ePos_AxisY1] = (double)m_stTeachPp.stTestSite[nLocStage - LOC_IDX_STATION_1].nOffSet[eTestPpOffSet_Y];
// 			dUserOffSet[CTaskTestPP::ePos_AxisX2] = (double)m_stTeachPp.stTestSite[nLocStage - LOC_IDX_STATION_1].nOffSet[eTestPpOffSet_X];
// 			dUserOffSet[CTaskTestPP::ePos_AxisY2] = (double)m_stTeachPp.stTestSite[nLocStage - LOC_IDX_STATION_1].nOffSet[eTestPpOffSet_Y];
// 
// 			if (nLocZ == CTaskTestPP::eZPOS_PLACE) {
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ1] = (double)m_stTeachPp.stTestSite[nLocStage - LOC_IDX_STATION_1].nOffSet[eTestPpOffSet_Z_Place];
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ2] = (double)m_stTeachPp.stTestSite[nLocStage - LOC_IDX_STATION_1].nOffSet[eTestPpOffSet_Z_Place];
// 			}
// 			else if (nLocZ == CTaskTestPP::eZPOS_PICK) {
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ1] = (double)m_stTeachPp.stTestSite[nLocStage - LOC_IDX_STATION_1].nOffSet[eTestPpOffSet_Z_Pick];
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ2] = (double)m_stTeachPp.stTestSite[nLocStage - LOC_IDX_STATION_1].nOffSet[eTestPpOffSet_Z_Pick];
// 			}
// 			else {
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ1] = 0.0;
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ2] = 0.0;
// 			}
// 
// 			dTPos[CTaskTestPP::ePos_AxisX1] += dUserOffSet[CTaskTestPP::ePos_AxisX1];
// 			dTPos[CTaskTestPP::ePos_AxisX2] += dUserOffSet[CTaskTestPP::ePos_AxisX2];
// 			dTPos[CTaskTestPP::ePos_AxisY1] += dUserOffSet[CTaskTestPP::ePos_AxisY1];
// 			dTPos[CTaskTestPP::ePos_AxisZ1] += dUserOffSet[CTaskTestPP::ePos_AxisZ1];
// 			dTPos[CTaskTestPP::ePos_AxisZ2] += dUserOffSet[CTaskTestPP::ePos_AxisZ2];
// 			dTPos[CTaskTestPP::ePos_AxisY2] += dUserOffSet[CTaskTestPP::ePos_AxisY2];


// 			//Test PP Site Exchange 시 Front picker off set 
// 
// 			if ((int)vPicker.size() > 0) {
// 				if (vPicker[0].y == 1) {
// 					double dUserOffSet[CTaskTestPP::ePos_Max] = { 0.0, };
// 
// 					dUserOffSet[CTaskTestPP::ePos_AxisX] = (double)m_stTeachPp.stTestSite[nLocStage - 1].nFrontSiteFrontPickerOffset[eTestPpOffSet_X];
// 					dUserOffSet[CTaskTestPP::ePos_AxisY] = (double)m_stTeachPp.stTestSite[nLocStage - 1].nFrontSiteFrontPickerOffset[eTestPpOffSet_Y];
// 
// 					if (nLocZ == CTaskTestPP::eZPOS_PLACE) {
// 						dUserOffSet[CTaskTestPP::eAxisZ] = (double)m_stTeachPp.stTestSite[nLocStage - 1].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Place];
// 					}
// 					else if (nLocZ == CTaskTestPP::eZPOS_PICK) {
// 						dUserOffSet[CTaskTestPP::eAxisZ] = (double)m_stTeachPp.stTestSite[nLocStage - 1].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Pick];
// 					}
// 					else {
// 						dUserOffSet[CTaskTestPP::eAxisZ] = 0.0;
// 					}
// 
// 					dTPos[CTaskTestPP::eAxisX] += dUserOffSet[CTaskTestPP::eAxisX];
// 					dTPos[CTaskTestPP::eAxisY] += dUserOffSet[CTaskTestPP::eAxisY];
// 					dTPos[CTaskTestPP::eAxisZ] += dUserOffSet[CTaskTestPP::eAxisZ];
// 
// 				}
// 			}
//
//		}
//		//Rear Site Offset
// 		else if (Pocket.y == 0) {
// 			//Test PP Site Exchange 시 Front picker off set 
// 			if ((int)vPicker.size() > 0) {
// 				if (vPicker[0].y == 1) {
// 					double dUserOffSet[CTaskTestPP::eAxisMax] = { 0.0, };
// 
// 					dUserOffSet[CTaskTestPP::eAxisX] = (double)m_stTeachPp.stTestSite[nLocStage - 1].nRearSiteFrontPickerOffset[eTestPpOffSet_X];
// 					dUserOffSet[CTaskTestPP::eAxisY] = (double)m_stTeachPp.stTestSite[nLocStage - 1].nRearSiteFrontPickerOffset[eTestPpOffSet_Y];
// 
// 					if (nLocZ == CTaskTestPP::eZPOS_PLACE) {
// 						dUserOffSet[CTaskTestPP::eAxisZ] = (double)m_stTeachPp.stTestSite[nLocStage - 1].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Place];
// 					}
// 					else if (nLocZ == CTaskTestPP::eZPOS_PICK) {
// 						dUserOffSet[CTaskTestPP::eAxisZ] = (double)m_stTeachPp.stTestSite[nLocStage - 1].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Pick];
// 					}
// 					else {
// 						dUserOffSet[CTaskTestPP::eAxisZ] = 0.0;
// 					}
// 
// 					dTPos[CTaskTestPP::eAxisX] += dUserOffSet[CTaskTestPP::eAxisX];
// 					dTPos[CTaskTestPP::eAxisY] += dUserOffSet[CTaskTestPP::eAxisY];
// 					dTPos[CTaskTestPP::eAxisZ] += dUserOffSet[CTaskTestPP::eAxisZ];
// 				}
// 			}
// 		}
// 	}
// 	else if (nLocStage >= LOC_IDX_TEST_PP_TBL_1_LD && nLocStage <= LOC_IDX_TEST_PP_TBL_4_UD) {
// 		//Load Table 2Y,Y4 Pocket offset
// 		if (Pocket.y == 1 || Pocket.y == 3) {
// 			double dUserOffSet[CTaskTestPP::ePos_Max] = { 0.0, };
// 
// 			dUserOffSet[CTaskTestPP::ePos_AxisX1] = (double)m_stTeachPp.nLoadTable2YRowOffset[eTestPpOffSet_X];
// 			dUserOffSet[CTaskTestPP::ePos_AxisY1] = (double)m_stTeachPp.nLoadTable2YRowOffset[eTestPpOffSet_Y];
// 			dUserOffSet[CTaskTestPP::ePos_AxisX2] = (double)m_stTeachPp.nLoadTable2YRowOffset[eTestPpOffSet_X];
// 			dUserOffSet[CTaskTestPP::ePos_AxisY2] = (double)m_stTeachPp.nLoadTable2YRowOffset[eTestPpOffSet_Y];
// 
// 			if (nLocZ == CTaskTestPP::eZPOS_PLACE) {
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ1] = (double)m_stTeachPp.nLoadTable2YRowOffset[eTestPpOffSet_Z_Place];
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ2] = (double)m_stTeachPp.nLoadTable2YRowOffset[eTestPpOffSet_Z_Place];
// 			}
// 			else if (nLocZ == CTaskTestPP::eZPOS_PICK) {
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ1] = (double)m_stTeachPp.nLoadTable2YRowOffset[eTestPpOffSet_Z_Pick];
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ2] = (double)m_stTeachPp.nLoadTable2YRowOffset[eTestPpOffSet_Z_Pick];
// 			}
// 			else {
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ1] = 0.0;
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ2] = 0.0;
// 			}
// 
// 			dTPos[CTaskTestPP::ePos_AxisX1] += dUserOffSet[CTaskTestPP::ePos_AxisX1];
// 			dTPos[CTaskTestPP::ePos_AxisX2] += dUserOffSet[CTaskTestPP::ePos_AxisX2];
// 			dTPos[CTaskTestPP::ePos_AxisY1] += dUserOffSet[CTaskTestPP::ePos_AxisY1];
// 			dTPos[CTaskTestPP::ePos_AxisZ1] += dUserOffSet[CTaskTestPP::ePos_AxisZ1];
// 			dTPos[CTaskTestPP::ePos_AxisZ2] += dUserOffSet[CTaskTestPP::ePos_AxisZ2];
// 			dTPos[CTaskTestPP::ePos_AxisY2] += dUserOffSet[CTaskTestPP::ePos_AxisY2];
// 		}
// 	}

	//Base Point Teaching값 사용안할 경우, Pitch 계산.
	if (m_bCalBasePoint == TRUE)
	{
		dTPos[CTaskTestPP::ePos_AxisY1] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_Y1);
		dTPos[CTaskTestPP::ePos_AxisY2] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_Y2);
	}

	*pdPosX1 = dTPos[CTaskTestPP::ePos_AxisX1];
	*pdPosY1 = dTPos[CTaskTestPP::ePos_AxisY1];
	*pdPosY2 = dTPos[CTaskTestPP::ePos_AxisY2];
	*pdPosZ1 = dTPos[CTaskTestPP::ePos_AxisZ1];
	*pdPitchX1 = dTPos[CTaskTestPP::ePos_AxisPitchX1];
	*pdPitchY1 = dTPos[CTaskTestPP::ePos_AxisPitchY1];
	*pdPosX2 = dTPos[CTaskTestPP::ePos_AxisX2];
	*pdPosZ2 = dTPos[CTaskTestPP::ePos_AxisZ2];
	*pdPitchX2 = dTPos[CTaskTestPP::ePos_AxisPitchX2];
	*pdPitchY2 = dTPos[CTaskTestPP::ePos_AxisPitchY2];

	return ERR_NO_ERROR;
}

int CTaskTestPP::GetTeachPos(int nLocStage, CPoint Pocket, std::vector<CPoint> vPicker, int nLocZ, double *dTPos, int nTestHandType, BOOL bCleanOffsetUse)
{
	ASSERT(nLocStage >= 0 && nLocStage < MAX_TEST_PP_LOC_COUNT);
	ASSERT(nLocZ >= 0 && nLocZ < CTaskTestPP::eMAX_ZPOS_COUNT);

	double dBufferTPos[CTaskTestPP::ePos_Max] = { 0, };

	dBufferTPos[CTaskTestPP::ePos_AxisX1] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_X1);
	dBufferTPos[CTaskTestPP::ePos_AxisX2] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_X2);

	dBufferTPos[CTaskTestPP::ePos_AxisY1] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_Y1);
	dBufferTPos[CTaskTestPP::ePos_AxisY2] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_Y2);

	if (nLocZ == eZPOS_PICK) {
		dBufferTPos[CTaskTestPP::ePos_AxisZ1] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_Z1_PICK);
		dBufferTPos[CTaskTestPP::ePos_AxisZ2] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_Z2_PICK);
	}
	else if (nLocZ == eZPOS_PLACE) {
		dBufferTPos[CTaskTestPP::ePos_AxisZ1] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_Z1_PLACE);
		dBufferTPos[CTaskTestPP::ePos_AxisZ2] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_Z2_PLACE);
	}
	else {
		dBufferTPos[CTaskTestPP::ePos_AxisZ1] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_Z1_SAFETY);
		dBufferTPos[CTaskTestPP::ePos_AxisZ2] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_Z2_SAFETY);
	}

	//[작업 필요] teaching pos 나오면 확인.
	dBufferTPos[CTaskTestPP::ePos_AxisPitchX1] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_X1_PITCH); //POS_IDX_TEST_PP_X_PITCH
	dBufferTPos[CTaskTestPP::ePos_AxisPitchY1] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_Y1_PITCH); //POS_IDX_TEST_PP_X_PITCH

	dBufferTPos[CTaskTestPP::ePos_AxisPitchX2] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_X2_PITCH); //POS_IDX_TEST_PP_X_PITCH
	dBufferTPos[CTaskTestPP::ePos_AxisPitchY2] = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nLocStage, POS_IDX_TEST_PP_Y2_PITCH); //POS_IDX_TEST_PP_X_PITCH

																																		  // 1. Picker Rear Left( 좌상)을 기준으로 거리 계산
	double dPocketDist[2] = { 0, };
	double dTargetPosPitch[2] = { 0, };


	if (nLocStage >= LOC_IDX_TEST_PP_TBL_1_LD && nLocStage <= LOC_IDX_TEST_PP_TBL_4_UD)
	{
		dTargetPosPitch[CTaskTestPP::ePos_AxisX1] = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_X);
		dTargetPosPitch[CTaskTestPP::ePos_AxisY1] = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_Y);
	}
	else if (nLocStage == LOC_IDX_TEST_PP_HOME) // Home Position Buffer에 들어가는 거 확인.
	{
	}
	else if (nLocStage == LOC_IDX_CLEAN_TBL)
	{
// 		dTargetPosPitch[CTaskTestPP::ePos_AxisX1] = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Pitch_X);
// 		dTargetPosPitch[CTaskTestPP::ePos_AxisY1] = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Pitch_Y);
	}
	else //LOC_IDX_STATION_1 ~ LOC_IDX_STATION_4_UL
	{
		dTargetPosPitch[CTaskTestPP::ePos_AxisX1] = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Pitch_X);
		dTargetPosPitch[CTaskTestPP::ePos_AxisY1] = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Pitch_Y);
	}


	//Pocket 계산
	if (m_nTestPPIdx == eTestPP_1)
	{
		dPocketDist[CTaskTestPP::ePos_AxisY1] += (dTargetPosPitch[CTaskTestPP::ePos_AxisX1] * Pocket.x);
		dPocketDist[CTaskTestPP::ePos_AxisX1] += (dTargetPosPitch[CTaskTestPP::ePos_AxisY1] * Pocket.y);
	}
	else
	{
		dPocketDist[CTaskTestPP::ePos_AxisY1] -= (dTargetPosPitch[CTaskTestPP::ePos_AxisX1] * Pocket.x);
		dPocketDist[CTaskTestPP::ePos_AxisX1] -= (dTargetPosPitch[CTaskTestPP::ePos_AxisY1] * Pocket.y);
	}

	// Picker 감안하여 Offset 계산.
	dBufferTPos[CTaskTestPP::ePos_AxisX1] += dPocketDist[CTaskTestPP::ePos_AxisX1];
	dBufferTPos[CTaskTestPP::ePos_AxisY1] += dPocketDist[CTaskTestPP::ePos_AxisY1];
	dBufferTPos[CTaskTestPP::ePos_AxisX2] += dPocketDist[CTaskTestPP::ePos_AxisX1];
	dBufferTPos[CTaskTestPP::ePos_AxisY2] += dPocketDist[CTaskTestPP::ePos_AxisY1];

	// 2. 집어야 할 Picker 감안하여 Offset 계산.
	double dPickerOffset[2] = { 0, };

// 	if ((nLocStage >= LOC_IDX_TEST_PP_TBL_1_LD && nLocStage <= LOC_IDX_CLEAN_TBL))
// 	{
// 		GetPickerOffset(nLocStage, vPicker, &dPickerOffset[0], &dPickerOffset[1]);
// 	}

	if (nLocStage == LOC_IDX_TEST_PP_HOME) { // Safety 위치는 Pocket 계산 제거
	}
	else
	{
		GetPickerOffset(nLocStage, vPicker, &dPickerOffset[0], &dPickerOffset[1]);
		if (m_nTestPPIdx == eTestPP_1)
		{
			dBufferTPos[CTaskTestPP::ePos_AxisX1] -= dPickerOffset[CTaskTestPP::ePos_AxisX1];
			dBufferTPos[CTaskTestPP::ePos_AxisX2] -= dPickerOffset[CTaskTestPP::ePos_AxisX1];

			dBufferTPos[CTaskTestPP::ePos_AxisY1] += dPickerOffset[CTaskTestPP::ePos_AxisY1];
			dBufferTPos[CTaskTestPP::ePos_AxisY2] += dPickerOffset[CTaskTestPP::ePos_AxisY1];
		}
		else
		{
			dBufferTPos[CTaskTestPP::ePos_AxisX1] += dPickerOffset[CTaskTestPP::ePos_AxisX1];
			dBufferTPos[CTaskTestPP::ePos_AxisX2] += dPickerOffset[CTaskTestPP::ePos_AxisX1];

			dBufferTPos[CTaskTestPP::ePos_AxisY1] -= dPickerOffset[CTaskTestPP::ePos_AxisY1];
			dBufferTPos[CTaskTestPP::ePos_AxisY2] -= dPickerOffset[CTaskTestPP::ePos_AxisY1];
		}
	}

	// user off set ( only test site )

// 	if (nLocStage >= LOC_IDX_STATION_1 && nLocStage <= LOC_IDX_STATION_4) {
// 		// Front Site Offset
// 		//TWSL421 8Picker로 1Pos Offset만 있으면 됨.
// 		//Load Hand, Unload Hand Offset 필요
// 		//[작업 필요]offset gui 작업 필요.
// 		if (Pocket.y == 0) { //Pocket.y == 1
// 			double dUserOffSet[CTaskTestPP::ePos_Max] = { 0.0, };
// 
// 			dUserOffSet[CTaskTestPP::ePos_AxisX1] = (double)m_stTeachPp.stTestSite[nLocStage - LOC_IDX_STATION_1].nOffSet[eTestPpOffSet_X];
// 			dUserOffSet[CTaskTestPP::ePos_AxisY1] = (double)m_stTeachPp.stTestSite[nLocStage - LOC_IDX_STATION_1].nOffSet[eTestPpOffSet_Y];
// 			dUserOffSet[CTaskTestPP::ePos_AxisX2] = (double)m_stTeachPp.stTestSite[nLocStage - LOC_IDX_STATION_1].nOffSet[eTestPpOffSet_X];
// 			dUserOffSet[CTaskTestPP::ePos_AxisY2] = (double)m_stTeachPp.stTestSite[nLocStage - LOC_IDX_STATION_1].nOffSet[eTestPpOffSet_Y];
// 
// 			if (nLocZ == CTaskTestPP::eZPOS_PLACE) {
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ1] = (double)m_stTeachPp.stTestSite[nLocStage - LOC_IDX_STATION_1].nOffSet[eTestPpOffSet_Z_Place];
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ2] = (double)m_stTeachPp.stTestSite[nLocStage - LOC_IDX_STATION_1].nOffSet[eTestPpOffSet_Z_Place];
// 			}
// 			else if (nLocZ == CTaskTestPP::eZPOS_PICK) {
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ1] = (double)m_stTeachPp.stTestSite[nLocStage - LOC_IDX_STATION_1].nOffSet[eTestPpOffSet_Z_Pick];
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ2] = (double)m_stTeachPp.stTestSite[nLocStage - LOC_IDX_STATION_1].nOffSet[eTestPpOffSet_Z_Pick];
// 			}
// 			else {
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ1] = 0.0;
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ2] = 0.0;
// 			}
// 
// 			dBufferTPos[CTaskTestPP::ePos_AxisX1] += dUserOffSet[CTaskTestPP::ePos_AxisX1];
// 			dBufferTPos[CTaskTestPP::ePos_AxisX2] += dUserOffSet[CTaskTestPP::ePos_AxisX2];
// 			dBufferTPos[CTaskTestPP::ePos_AxisY1] += dUserOffSet[CTaskTestPP::ePos_AxisY1];
// 			dBufferTPos[CTaskTestPP::ePos_AxisZ1] += dUserOffSet[CTaskTestPP::ePos_AxisZ1];
// 			dBufferTPos[CTaskTestPP::ePos_AxisZ2] += dUserOffSet[CTaskTestPP::ePos_AxisZ2];
// 			dBufferTPos[CTaskTestPP::ePos_AxisY2] += dUserOffSet[CTaskTestPP::ePos_AxisY2];
// 
// 
// 			// 			//Test PP Site Exchange 시 Front picker off set 
// 			// 
// 			// 			if ((int)vPicker.size() > 0) {
// 			// 				if (vPicker[0].y == 1) {
// 			// 					double dUserOffSet[CTaskTestPP::ePos_Max] = { 0.0, };
// 			// 
// 			// 					dUserOffSet[CTaskTestPP::ePos_AxisX] = (double)m_stTeachPp.stTestSite[nLocStage - 1].nFrontSiteFrontPickerOffset[eTestPpOffSet_X];
// 			// 					dUserOffSet[CTaskTestPP::ePos_AxisY] = (double)m_stTeachPp.stTestSite[nLocStage - 1].nFrontSiteFrontPickerOffset[eTestPpOffSet_Y];
// 			// 
// 			// 					if (nLocZ == CTaskTestPP::eZPOS_PLACE) {
// 			// 						dUserOffSet[CTaskTestPP::eAxisZ] = (double)m_stTeachPp.stTestSite[nLocStage - 1].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Place];
// 			// 					}
// 			// 					else if (nLocZ == CTaskTestPP::eZPOS_PICK) {
// 			// 						dUserOffSet[CTaskTestPP::eAxisZ] = (double)m_stTeachPp.stTestSite[nLocStage - 1].nFrontSiteFrontPickerOffset[eTestPpOffSet_Z_Pick];
// 			// 					}
// 			// 					else {
// 			// 						dUserOffSet[CTaskTestPP::eAxisZ] = 0.0;
// 			// 					}
// 			// 
// 			// 					dBufferTPos[CTaskTestPP::eAxisX] += dUserOffSet[CTaskTestPP::eAxisX];
// 			// 					dBufferTPos[CTaskTestPP::eAxisY] += dUserOffSet[CTaskTestPP::eAxisY];
// 			// 					dBufferTPos[CTaskTestPP::eAxisZ] += dUserOffSet[CTaskTestPP::eAxisZ];
// 			// 
// 			// 				}
// 			// 			}
// 
// 		}
// 		//Rear Site Offset
// 		// 		else if (Pocket.y == 0) {
// 		// 			//Test PP Site Exchange 시 Front picker off set 
// 		// 			if ((int)vPicker.size() > 0) {
// 		// 				if (vPicker[0].y == 1) {
// 		// 					double dUserOffSet[CTaskTestPP::eAxisMax] = { 0.0, };
// 		// 
// 		// 					dUserOffSet[CTaskTestPP::eAxisX] = (double)m_stTeachPp.stTestSite[nLocStage - 1].nRearSiteFrontPickerOffset[eTestPpOffSet_X];
// 		// 					dUserOffSet[CTaskTestPP::eAxisY] = (double)m_stTeachPp.stTestSite[nLocStage - 1].nRearSiteFrontPickerOffset[eTestPpOffSet_Y];
// 		// 
// 		// 					if (nLocZ == CTaskTestPP::eZPOS_PLACE) {
// 		// 						dUserOffSet[CTaskTestPP::eAxisZ] = (double)m_stTeachPp.stTestSite[nLocStage - 1].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Place];
// 		// 					}
// 		// 					else if (nLocZ == CTaskTestPP::eZPOS_PICK) {
// 		// 						dUserOffSet[CTaskTestPP::eAxisZ] = (double)m_stTeachPp.stTestSite[nLocStage - 1].nRearSiteFrontPickerOffset[eTestPpOffSet_Z_Pick];
// 		// 					}
// 		// 					else {
// 		// 						dUserOffSet[CTaskTestPP::eAxisZ] = 0.0;
// 		// 					}
// 		// 
// 		// 					dBufferTPos[CTaskTestPP::eAxisX] += dUserOffSet[CTaskTestPP::eAxisX];
// 		// 					dBufferTPos[CTaskTestPP::eAxisY] += dUserOffSet[CTaskTestPP::eAxisY];
// 		// 					dBufferTPos[CTaskTestPP::eAxisZ] += dUserOffSet[CTaskTestPP::eAxisZ];
// 		// 				}
// 		// 			}
// 		// 		}
// 
// 	}
// 	else if (nLocStage >= LOC_IDX_TEST_PP_TBL_1_LD && nLocStage <= LOC_IDX_TEST_PP_TBL_4_UD) {
// 		//Load Table 2Y,Y4 Pocket offset
// 		if (Pocket.y == 1 || Pocket.y == 3) {
// 			double dUserOffSet[CTaskTestPP::ePos_Max] = { 0.0, };
// 
// 			dUserOffSet[CTaskTestPP::ePos_AxisX1] = (double)m_stTeachPp.nLoadTable2YRowOffset[eTestPpOffSet_X];
// 			dUserOffSet[CTaskTestPP::ePos_AxisY1] = (double)m_stTeachPp.nLoadTable2YRowOffset[eTestPpOffSet_Y];
// 			dUserOffSet[CTaskTestPP::ePos_AxisX1] = (double)m_stTeachPp.nLoadTable2YRowOffset[eTestPpOffSet_X];
// 			dUserOffSet[CTaskTestPP::ePos_AxisY2] = (double)m_stTeachPp.nLoadTable2YRowOffset[eTestPpOffSet_Y];
// 
// 			if (nLocZ == CTaskTestPP::eZPOS_PLACE) {
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ1] = (double)m_stTeachPp.nLoadTable2YRowOffset[eTestPpOffSet_Z_Place];
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ2] = (double)m_stTeachPp.nLoadTable2YRowOffset[eTestPpOffSet_Z_Place];
// 			}
// 			else if (nLocZ == CTaskTestPP::eZPOS_PICK) {
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ1] = (double)m_stTeachPp.nLoadTable2YRowOffset[eTestPpOffSet_Z_Pick];
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ2] = (double)m_stTeachPp.nLoadTable2YRowOffset[eTestPpOffSet_Z_Pick];
// 			}
// 			else {
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ1] = 0.0;
// 				dUserOffSet[CTaskTestPP::ePos_AxisZ2] = 0.0;
// 			}
// 
// 			dBufferTPos[CTaskTestPP::ePos_AxisX1] += dUserOffSet[CTaskTestPP::ePos_AxisX1];
// 			dBufferTPos[CTaskTestPP::ePos_AxisX2] += dUserOffSet[CTaskTestPP::ePos_AxisX2];
// 			dBufferTPos[CTaskTestPP::ePos_AxisY1] += dUserOffSet[CTaskTestPP::ePos_AxisY1];
// 			dBufferTPos[CTaskTestPP::ePos_AxisZ1] += dUserOffSet[CTaskTestPP::ePos_AxisZ1];
// 			dBufferTPos[CTaskTestPP::ePos_AxisZ2] += dUserOffSet[CTaskTestPP::ePos_AxisZ2];
// 			dBufferTPos[CTaskTestPP::ePos_AxisY2] += dUserOffSet[CTaskTestPP::ePos_AxisY2];
// 		}
// 	}


	dTPos[CTaskTestPP::eAxisX1] = dBufferTPos[CTaskTestPP::ePos_AxisX1];
	if(nTestHandType == eTestPPLoadType)
		dTPos[CTaskTestPP::eAxisY] = dBufferTPos[CTaskTestPP::ePos_AxisY1];
	else
		dTPos[CTaskTestPP::eAxisY] = dBufferTPos[CTaskTestPP::ePos_AxisY2];

	dTPos[CTaskTestPP::eAxisZ1] = dBufferTPos[CTaskTestPP::ePos_AxisZ1];
	dTPos[CTaskTestPP::eAxisPitch_HW_X1_SW_Y1] = dBufferTPos[CTaskTestPP::ePos_AxisPitchX1];
	dTPos[CTaskTestPP::eAxisPitch_HW_Y1_SW_X1] = dBufferTPos[CTaskTestPP::ePos_AxisPitchY1];
	dTPos[CTaskTestPP::eAxisX2]		 = dBufferTPos[CTaskTestPP::ePos_AxisX2];
	dTPos[CTaskTestPP::eAxisZ2]		= dBufferTPos[CTaskTestPP::ePos_AxisZ2];
	dTPos[CTaskTestPP::eAxisPitch_HW_X2_SW_Y2] = dBufferTPos[CTaskTestPP::ePos_AxisPitchX2];
	dTPos[CTaskTestPP::eAxisPitch_HW_Y2_SW_X2] = dBufferTPos[CTaskTestPP::ePos_AxisPitchY2];

	return ERR_NO_ERROR;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-07]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
double CTaskTestPP::GetTeachPos(int nLocStage, CPoint Pocket, std::vector<CPoint> vPicker, int nUpDown, int nAxisIdx,int nTestHandType, BOOL bCleanOffsetUse)
{
	ASSERT( nAxisIdx >= 0 && nAxisIdx < CTaskTestPP::eAxisMax);
	
	double dTPos[CTaskTestPP::ePos_Max] = {0.0,};
	GetTeachPos(nLocStage, Pocket, vPicker, nUpDown, &dTPos[0], &dTPos[1], &dTPos[2], &dTPos[3], &dTPos[4],
		&dTPos[5], &dTPos[6], &dTPos[7], &dTPos[8], &dTPos[9], nTestHandType, bCleanOffsetUse);


	int nAxisConIdx = TeachPosIdxConvertor(nAxisIdx, nTestHandType);

	return dTPos[nAxisConIdx];
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-08-17]
  NAME     : 
  PARAMS   : eTeachPos : 
             eAxis     : -1 = 모든 축의 위치가 맞는지 점검 한다.
  RETURN   :  0 = OK
             -1 = Motion 동작 중 임.
			 -2 = 동작은 멈췄으나 Position Range 안에 들어오지 않음.
  COMMENTS : Motor의 현재 위치가 지정한 위치에 있는지 점검 한다.
==============================================================================*/
int CTaskTestPP::ChkMotorPos(int nLocStage, int nTestHandType, CPoint Pocket, std::vector<CPoint> vPicker, int nUpDown, int nAxisIdx/*=eAxisAll*/)
{
	ASSERT( nLocStage >= 0 && nLocStage < MAX_TEST_PP_LOC_COUNT        );
	ASSERT( nAxisIdx  >= eAxisAll && nAxisIdx  < CTaskTestPP::eAxisMax );

	int i=0;
	double dTeachPos[CTaskTestPP::ePos_Max] = {0.0, };
	double dCurPos  [CTaskTestPP::eAxisMax] = {0.0, };

	GetTeachPos(nLocStage, Pocket, vPicker, nUpDown, &dTeachPos[0], &dTeachPos[1], &dTeachPos[2], &dTeachPos[3], &dTeachPos[4],
		&dTeachPos[5], &dTeachPos[6], &dTeachPos[7], &dTeachPos[8], &dTeachPos[9], nTestHandType);

	for(i=0; i<CTaskTestPP::eAxisMax; i++){
		OneAxis* pMotor = (OneAxis*)m_vMotor[i];
			pMotor->GetCurrentPos(dCurPos[i]);
	}

	if (nAxisIdx == CTaskTestPP::eAxisAll) {
		for (i = 0; i < CTaskTestPP::eAxisMax; i++) {
			OneAxis* pMotor = (OneAxis*)m_vMotor[i];

			//Hand 전체 축에 대해 MotionDone Check.
			//X1, X2, Y, Z1, Z2, XPitch1, XPitch2, YPitch1, YPitch2
			if (pMotor->MotionDone() != ERR_NO_ERROR) {
				return -1;
			}

			int nIdx = TransIdxMotorAxisToTeachPos(i);

			//dcurPos : 해당 축의 실제 Pos. (X1, X2, Y, Z1, Z2, XPitch1, XPitch2, YPitch1, YPitch2)
			//dTeachPos : 해당 Loc의 Teaching Pos. (X, Y, Z, XPitch, YPitch)
			if (fabs(dCurPos[i] - dTeachPos[nIdx]) > m_dMotorErrRange[i]) {
				return -2;
			}
		}
	}
	else {
		OneAxis* pMotor = (OneAxis*)m_vMotor[nAxisIdx];
		if( pMotor->MotionDone() != ERR_NO_ERROR){
			return -1;
		}
		
		int nIdx = TransIdxMotorAxisToTeachPos(nAxisIdx);
		if( fabs(dCurPos[nAxisIdx] - dTeachPos[nIdx]) > m_dMotorErrRange[nAxisIdx] ){
			return -2;			
		}
	}



// 	if( nAxisIdx == CTaskTestPP::eAxisAll ){
// 		for(i=0; i<CTaskTestPP::eAxisMax; i++){
// 			OneAxis* pMotor = (OneAxis*)m_vMotor[i];
// 			if( pMotor->MotionDone() != 1 ){
// 				return -1;
// 			}
// 
// 			if( fabs(dCurPos[i] - dTeachPos[i]) > m_dMotorErrRange[i] ){
// 				return -2;			
// 			}
// 		}
// 	}else{
// 		OneAxis* pMotor = (OneAxis*)m_vMotor[nAxisIdx];
// 		if( pMotor->MotionDone() != 1 ){
// 			return -1;
// 		}
// 
// 		if( fabs(dCurPos[nAxisIdx] - dTeachPos[nAxisIdx]) > m_dMotorErrRange[nAxisIdx] ){
// 			return -2;			
// 		}
// 	}

	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-08-30]
  NAME     : 
  PARAMS   : 
  RETURN   : 0 = OK
            -1 = Motion 동작 중 임.
            -2 = 동작은 멈췄으나 Position Range 안에 들어오지 않음.
  COMMENTS : 각 축의 위치가 지정한 위치 + offset 에 있는지 점검 한다.
==============================================================================*/
int CTaskTestPP::ChkMotorPos(int nLocStage, int nTestHandType, CPoint Pocket, std::vector<CPoint> vPicker, int nUpDown, double dOffset, int nAxisIdx/*=eAxisAll*/)
{
	ASSERT( nLocStage >= 0 && nLocStage < MAX_TEST_PP_LOC_COUNT      );
	ASSERT( nAxisIdx  >= 0 && nAxisIdx  < eAxisMax);

	double dTeachPos[CTaskTestPP::ePos_Max] = {0.0, };
	double dCurPos  [CTaskTestPP::eAxisMax] = {0.0, };

	GetTeachPos(nLocStage, Pocket, vPicker, nUpDown, &dTeachPos[0], &dTeachPos[1], &dTeachPos[2], &dTeachPos[3], &dTeachPos[4],
		&dTeachPos[5], &dTeachPos[6], &dTeachPos[7], &dTeachPos[8], &dTeachPos[9], nTestHandType);

	for(int i=0; i<CTaskTestPP::eAxisMax; i++){
		OneAxis* pMotor = (OneAxis*)m_vMotor[i];
		pMotor->GetCurrentPos(dCurPos[i]);
	}

	if( nAxisIdx == CTaskTestPP::eAxisAll ){
		for(int i=0; i<CTaskTestPP::eAxisMax; i++){
			OneAxis* pMotor = (OneAxis*)m_vMotor[i];
			if( pMotor->MotionDone() != ERR_NO_ERROR){
				return -1;
			}

			int nIdx = TransIdxMotorAxisToTeachPos(i);

			if( fabs(dCurPos[i] - (dTeachPos[nIdx] + dOffset)) > m_dMotorErrRange[i] ){
				return -2;			
			}
		}
	}else{
		OneAxis* pMotor = (OneAxis*)m_vMotor[nAxisIdx];
		if( pMotor->MotionDone() != ERR_NO_ERROR){
			return -1;
		}

		int nIdx = TransIdxMotorAxisToTeachPos(nAxisIdx);

		if( fabs(dCurPos[nAxisIdx] - (dTeachPos[nIdx] + dOffset) ) > m_dMotorErrRange[nAxisIdx] ){
			return -2;			
		}
	}

	return ERR_NO_ERROR;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-07]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskTestPP::do_MovePos(int nLocStage,int nTestHandType, CPoint Pocket, std::vector<CPoint> vPicker, int nUpDown, int nAxisIdx, BOOL bCleanOffsetUse, int nSeqSpdRate/*=100*/, double dPosOffset)
{
	ASSERT( nLocStage >= 0 && nLocStage < MAX_TEST_PP_LOC_COUNT      );
	ASSERT( nAxisIdx  >= 0 && nAxisIdx  < eAxisMax);
	ASSERT( nSeqSpdRate >=0 && nSeqSpdRate <= 100);
	
	double dTPos = GetTeachPos(nLocStage, Pocket, vPicker, nUpDown, nAxisIdx, nTestHandType, bCleanOffsetUse);

	int nErr = Interlock_MovePos(nAxisIdx, nLocStage);
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}

	OneAxis* pMotor = (OneAxis*)m_vMotor[nAxisIdx];

	/*User Spd, Acc, Dcc & Seq Spd, Acc, Dcc 를 가지고 계산*/

	int nIndexSpd[eMaxTestPPCount][CTaskTestPP::eAxisMax] = { { NDM4_SPD_TestPP1_X1, NDM4_SPD_TestPP1_X2, NDM4_SPD_TestPP1_Y, NDM4_SPD_TestPP1_Z1, NDM4_SPD_TestPP1_Z2, NDM5_SPD_TestPP_1_Pitch_X1, NDM5_SPD_TestPP_1_Pitch_X2, NDM5_SPD_TestPP_1_Pitch_Y1, NDM5_SPD_TestPP_1_Pitch_Y2 },
															  {NDM4_SPD_TestPP2_X1, NDM4_SPD_TestPP2_X2, NDM4_SPD_TestPP2_Y, NDM4_SPD_TestPP2_Z1, NDM4_SPD_TestPP2_Z2, NDM5_SPD_TestPP_2_Pitch_X1, NDM5_SPD_TestPP_2_Pitch_X2, NDM5_SPD_TestPP_2_Pitch_Y1, NDM5_SPD_TestPP_2_Pitch_Y2 } };

	int nIndexAcc[eMaxTestPPCount][CTaskTestPP::eAxisMax] = { {NDM4_ACC_TestPP1_X1, NDM4_ACC_TestPP1_X2, NDM4_ACC_TestPP1_Y, NDM4_ACC_TestPP1_Z1, NDM4_ACC_TestPP1_Z2, NDM5_ACC_TestPP_1_Pitch_X1, NDM5_ACC_TestPP_1_Pitch_X2, NDM5_ACC_TestPP_1_Pitch_Y1, NDM5_ACC_TestPP_1_Pitch_Y2 },
															  {NDM4_ACC_TestPP2_X1, NDM4_ACC_TestPP2_X2, NDM4_ACC_TestPP2_Y, NDM4_ACC_TestPP2_Z1, NDM4_ACC_TestPP2_Z2, NDM5_ACC_TestPP_2_Pitch_X1, NDM5_ACC_TestPP_2_Pitch_X2, NDM5_ACC_TestPP_2_Pitch_Y1, NDM5_ACC_TestPP_2_Pitch_Y2 } };

	int nIndexDcc[eMaxTestPPCount][CTaskTestPP::eAxisMax] = {{NDM4_DCC_TestPP1_X1, NDM4_DCC_TestPP1_X2, NDM4_DCC_TestPP1_Y, NDM4_DCC_TestPP1_Z1, NDM4_DCC_TestPP1_Z2, NDM5_DCC_TestPP_1_Pitch_X1, NDM5_DCC_TestPP_1_Pitch_X2, NDM5_DCC_TestPP_1_Pitch_Y1, NDM5_DCC_TestPP_1_Pitch_Y2 },
															 { NDM4_DCC_TestPP2_X1, NDM4_DCC_TestPP2_X2, NDM4_DCC_TestPP2_Y, NDM4_DCC_TestPP2_Z1, NDM4_DCC_TestPP2_Z2, NDM5_DCC_TestPP_2_Pitch_X1, NDM5_DCC_TestPP_2_Pitch_X2, NDM5_DCC_TestPP_2_Pitch_Y1, NDM5_DCC_TestPP_2_Pitch_Y2 } };


	double dRateSpd = (double)g_DMCont.m_dmTestPP.GN(nIndexSpd[m_nTestPPIdx][nAxisIdx]);
	int    nRateAcc = g_DMCont.m_dmTestPP.GN(nIndexAcc[m_nTestPPIdx][nAxisIdx]);
	int    nRateDcc = g_DMCont.m_dmTestPP.GN(nIndexDcc[m_nTestPPIdx][nAxisIdx]);

	double dMtSpd = pMotor->GetSpeed();
	double dMtAcc = pMotor->GetAcc();
	double dMtDcc = pMotor->GetDcc();

	double dRetSpd=0;
	int    nRetAcc=0, nRetDcc=0;

	dRetSpd = (dMtSpd * (dRateSpd/100.0)) * (nSeqSpdRate/100.0);
	nRetAcc = (int)(dMtAcc - (dMtAcc*0.75)*(nRateAcc/100.0)); // 설정되어 있는 값에서 spd rate가 높을 수록 acc/dcc를 감소 시켜준다. 가장 빠를 때가 설정 속도의 95%뺀 acc/dcc로 함.
	nRetDcc = (int)(dMtDcc - (dMtDcc*0.75)*(nRateDcc/100.0));
	// Picker Z Axis Motor
	if (nAxisIdx == eAxisZ1 || nAxisIdx == eAxisZ2) { 
	}
	else{
		if(nRetAcc < 200) nRetAcc = 200;
		if(nRetDcc < 200) nRetDcc = 200;
	}

	double dTargetPos = ( dTPos + dPosOffset );
	if( nAxisIdx == eAxisZ1 ) {
		if( m_nTestPPIdx == eTestPP_1 ) {
			MotorMoveInterLock_Test_Hand1_z1( dTargetPos, dRetSpd, nRetAcc, nRetDcc );
		}
		else if( m_nTestPPIdx == eTestPP_2 ) {
			MotorMoveInterLock_Test_Hand2_z1( dTargetPos, dRetSpd, nRetAcc, nRetDcc );
		}
		else {
			ASSERT( false );
		}
	}
	
	//거리에 따른 속도 조절 [ X Axis, Y Axis ]
	double dCurPos = 0;
	pMotor->GetCurrentPos(dCurPos);
	double dGapPos = fabs(dTPos - dCurPos);
	if (nAxisIdx == eAxisX1  && dGapPos < 100000.0)//(TRAY_PP_MOTOR_SPD_STD_GAP_DIST - 50000.0) && dRetSpd >(dMtSpd*0.7))
	{
		// (X Axis or Y Axis) && Move Speed over the 70% >> original speed half speed
		//if (|| nAxisIdx == eAxisY)
		//{
		dRetSpd = dRetSpd * 0.75;
		//}
	}
	if (nAxisIdx == eAxisY  && dGapPos < 100000.0)//(TRAY_PP_MOTOR_SPD_STD_GAP_DIST - 50000.0) && dRetSpd >(dMtSpd*0.7))
	{
		// (X Axis or Y Axis) && Move Speed over the 70% >> original speed half speed
		//if (|| nAxisIdx == eAxisY)
		//{
		dRetSpd = dRetSpd * 0.75;
		//}
	}
	if (nAxisIdx == eAxisZ1  && dGapPos < 50000.0)//(TRAY_PP_MOTOR_SPD_STD_GAP_DIST - 50000.0) && dRetSpd >(dMtSpd*0.7))
	{
		// (X Axis or Y Axis) && Move Speed over the 70% >> original speed half speed
		//if (|| nAxisIdx == eAxisY)
		//{
		dRetSpd = dRetSpd * 0.75;
		//}
	}

	//TWSL421 현재 사용 안함.
	double dOffset = 0.0;

	//[작업 필요] Interlock_Move_X로 변경 필요.
	//nErr = Interlock_Move_X(dTargetPos, nAxisIdx, nTestHandType);

	if (nErr != ERR_NO_ERROR)
		return nErr;
	
	
	CString strPosLog = _T( "" );
	strPosLog.Format( "CTaskTestPP::do_MovePos Axis : %d , Target Pos : %.3f um , Location : %s ", nAxisIdx, dTargetPos, GetLocName(nLocStage));
	MakeLog( strPosLog );

	BOOL bZpickerNouseOffset = FALSE;
	if (nLocStage == LOC_IDX_TEST_PP_HOME || nUpDown == eZPOS_SAFETY)
	{
		bZpickerNouseOffset = TRUE;
	}

	BOOL bApplyBacklash = FALSE;
	if (nAxisIdx == eAxisX1 || nAxisIdx == eAxisX2 || nAxisIdx == eAxisY)
	{
		bApplyBacklash = TRUE;
	}
	//VAT 측정 시, 속도 조절
	//VAT에서 얘 do_Move 사용 안함.
// 	if (g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_USE) == TRUE) {
// 		dRetSpd = dMtSpd * (VAT_MOTOR_SPEED / 100.0);
// 		bZpickerNouseOffset = TRUE;
// 	}
// 	else { //Running시, Picker Gap 저장
// 		if ((nAxisIdx == eAxisZ1 || nAxisIdx == eAxisZ2) && nLocStage != LOC_IDX_TEST_PP_HOME)
// 		{
// 			if (m_nTestPPIdx == eTestPP_1) {
// 				g_pBbxmnpClient->SetZAutoTeachingData(g_VATCommonData.m_stVATZTeachInfo_temp.dTestPp1_Loc_PickerGap[nTestHandType][nLocStage]);
// 			}
// 			else {
// 				g_pBbxmnpClient->SetZAutoTeachingData(g_VATCommonData.m_stVATZTeachInfo_temp.dTestPp2_Loc_PickerGap[nTestHandType][nLocStage]);
// 			}
// 			//g_pBbxmnpClient->SetZAutoTeachingData((nAxisIdx == eAxisZ1) ? g_VATCommonData.m_stVATZTeachInfo.dTestPp1_Loc_PickerGap[nTestHandType][nLocStage] : g_VATCommonData.m_stVATZTeachInfo.dTestPp2_Loc_PickerGap[nTestHandType][nLocStage]);
// 		}
// 	}


	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
	if (nAxisIdx == eAxisZ1 || nAxisIdx == eAxisZ2) { // Picker Z Axis Motor
		if (nLocStage != LOC_IDX_TEST_PP_HOME) {
			_SetFlatnessOffset(nTestHandType, nLocStage);
		}
		
		nErr = pMotor->Move(vPicker, dTargetPos, dRetSpd, nRetAcc, nRetDcc, nBoardType, bZpickerNouseOffset/*bVatOffsetUse*/);
	}
	else {
		nErr = pMotor->Move(dTargetPos, dRetSpd, nRetAcc, nRetDcc, bApplyBacklash);
	}
	
	return nErr;
}

	 	


int CTaskTestPP::do_MovePos_Pcd_backlash( double dAbsPosition, std::vector<CPoint> vPicker, int nAxisIdx, int nSeqSpdRate, double dPosOffset, BOOL bIsApplyBcklsh , int nTestHandType )
{
	int nErr = NO_ERROR;
	ASSERT(nAxisIdx >= 0 && nAxisIdx < eAxisMax);
	ASSERT(nSeqSpdRate >= 0 && nSeqSpdRate <= 100);

// 	int nErr = Interlock_MovePos(nAxisIdx, dAbsPosition);
// 	if (nErr != ERR_NO_ERROR) {
// 		return nErr;
// 	}

	OneAxis* pMotor = (OneAxis*)m_vMotor[nAxisIdx];

	/*User Spd, Acc, Dcc & Seq Spd, Acc, Dcc 를 가지고 계산*/

	int nIndexSpd[eMaxTestPPCount][CTaskTestPP::eAxisMax] = { { NDM4_SPD_TestPP1_X1, NDM4_SPD_TestPP1_X2, NDM4_SPD_TestPP1_Y, NDM4_SPD_TestPP1_Z1, NDM4_SPD_TestPP1_Z2, NDM5_SPD_TestPP_1_Pitch_X1, NDM5_SPD_TestPP_1_Pitch_X2, NDM5_SPD_TestPP_1_Pitch_Y1, NDM5_SPD_TestPP_1_Pitch_Y2 },
															  { NDM4_SPD_TestPP2_X1, NDM4_SPD_TestPP2_X2, NDM4_SPD_TestPP2_Y, NDM4_SPD_TestPP2_Z1, NDM4_SPD_TestPP2_Z2, NDM5_SPD_TestPP_2_Pitch_X1, NDM5_SPD_TestPP_2_Pitch_X2, NDM5_SPD_TestPP_2_Pitch_Y1, NDM5_SPD_TestPP_2_Pitch_Y2 } };

	int nIndexAcc[eMaxTestPPCount][CTaskTestPP::eAxisMax] = { { NDM4_ACC_TestPP1_X1, NDM4_ACC_TestPP1_X2, NDM4_ACC_TestPP1_Y, NDM4_ACC_TestPP1_Z1, NDM4_ACC_TestPP1_Z2, NDM5_ACC_TestPP_1_Pitch_X1, NDM5_ACC_TestPP_1_Pitch_X2, NDM5_ACC_TestPP_1_Pitch_Y1, NDM5_ACC_TestPP_1_Pitch_Y2 },
															  { NDM4_ACC_TestPP2_X1, NDM4_ACC_TestPP2_X2, NDM4_ACC_TestPP2_Y, NDM4_ACC_TestPP2_Z1, NDM4_ACC_TestPP2_Z2, NDM5_ACC_TestPP_2_Pitch_X1, NDM5_ACC_TestPP_2_Pitch_X2, NDM5_ACC_TestPP_2_Pitch_Y1, NDM5_ACC_TestPP_2_Pitch_Y2 } };

	int nIndexDcc[eMaxTestPPCount][CTaskTestPP::eAxisMax] = { { NDM4_DCC_TestPP1_X1, NDM4_DCC_TestPP1_X2, NDM4_DCC_TestPP1_Y, NDM4_DCC_TestPP1_Z1, NDM4_DCC_TestPP1_Z2, NDM5_DCC_TestPP_1_Pitch_X1, NDM5_DCC_TestPP_1_Pitch_X2, NDM5_DCC_TestPP_1_Pitch_Y1, NDM5_DCC_TestPP_1_Pitch_Y2 },
															  { NDM4_DCC_TestPP2_X1, NDM4_DCC_TestPP2_X2, NDM4_DCC_TestPP2_Y, NDM4_DCC_TestPP2_Z1, NDM4_DCC_TestPP2_Z2, NDM5_DCC_TestPP_2_Pitch_X1, NDM5_DCC_TestPP_2_Pitch_X2, NDM5_DCC_TestPP_2_Pitch_Y1, NDM5_DCC_TestPP_2_Pitch_Y2 } };



	double dRateSpd = (double)g_DMCont.m_dmTestPP.GN(nIndexSpd[m_nTestPPIdx][nAxisIdx]);
	int    nRateAcc = g_DMCont.m_dmTestPP.GN(nIndexAcc[m_nTestPPIdx][nAxisIdx]);
	int    nRateDcc = g_DMCont.m_dmTestPP.GN(nIndexDcc[m_nTestPPIdx][nAxisIdx]);

	double dMtSpd = pMotor->GetSpeed();
	double dMtAcc = pMotor->GetAcc();
	double dMtDcc = pMotor->GetDcc();

	double dRetSpd = 0;
	int    nRetAcc = 0, nRetDcc = 0;

	dRetSpd = (dMtSpd * (dRateSpd / 100.0)) * (nSeqSpdRate / 100.0);
	nRetAcc = (int)(dMtAcc - (dMtAcc*0.75)*(nRateAcc / 100.0)); // 설정되어 있는 값에서 spd rate가 높을 수록 acc/dcc를 감소 시켜준다. 가장 빠를 때가 설정 속도의 95%뺀 acc/dcc로 함.
	nRetDcc = (int)(dMtDcc - (dMtDcc*0.75)*(nRateDcc / 100.0));

	if (nRetAcc < 200) nRetAcc = 200;
	if (nRetDcc < 200) nRetDcc = 200;

/////////////////////////////////////////////////////////////////////////////////
	//거리에 따른 속도 조절 [ X Axis, Y Axis ]   추가했음. 
	double dCurPos = 0;
	pMotor->GetCurrentPos(dCurPos);
	double dGapPos = fabs(dAbsPosition - dCurPos);
	if (nAxisIdx == eAxisX1  && dGapPos < 100000.0)//(TRAY_PP_MOTOR_SPD_STD_GAP_DIST - 50000.0) && dRetSpd >(dMtSpd*0.7))
	{
		// (X Axis or Y Axis) && Move Speed over the 70% >> original speed half speed
		//if (|| nAxisIdx == eAxisY)
		//{
		dRetSpd = dRetSpd * 0.75;
		//}
	}
	if (nAxisIdx == eAxisY  && dGapPos < 100000.0)//(TRAY_PP_MOTOR_SPD_STD_GAP_DIST - 50000.0) && dRetSpd >(dMtSpd*0.7))
	{
		// (X Axis or Y Axis) && Move Speed over the 70% >> original speed half speed
		//if (|| nAxisIdx == eAxisY)
		//{
		dRetSpd = dRetSpd * 0.75;
		//}
	}
	if (nAxisIdx == eAxisZ1  && dGapPos < 50000.0)//(TRAY_PP_MOTOR_SPD_STD_GAP_DIST - 50000.0) && dRetSpd >(dMtSpd*0.7))
	{
		// (X Axis or Y Axis) && Move Speed over the 70% >> original speed half speed
		//if (|| nAxisIdx == eAxisY)
		//{
		dRetSpd = dRetSpd * 0.75;
		//}
	}

// 	nErr = Interlock_Move_X(dAbsPosition + dPosOffset, nAxisIdx);
// 
// 	if (nErr != ERR_NO_ERROR)
// 		return nErr;
	
	////////////////////////////////////////////////////////////////////////////////////////////////


															//  [9/11/2020 jaehoon.jung] 테스트를 위한 속도 최저

//	nErr = pMotor->Move((dAbsPosition + dPosOffset), /*TEACH_MOVE_SPD_LOW*/dRetSpd, nRetAcc, nRetDcc, bIsApplyBcklsh);


	if (nAxisIdx == eAxisZ1 || nAxisIdx == eAxisZ2) // Picker Z Axis Motor
		nErr = pMotor->Move(vPicker, dAbsPosition + dPosOffset, dRetSpd, nRetAcc, nRetDcc);
	else
		nErr = pMotor->Move((dAbsPosition + dPosOffset), /*TEACH_MOVE_SPD_LOW*/dRetSpd, nRetAcc, nRetDcc);

	return nErr;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-07]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskTestPP::Interlock_MovePos(int nAxisIdx, int nLocStage) //[작업 필요] Alarm 수정 필요.
{
	ASSERT(LOC_IDX_STATION_1 <= nLocStage && nLocStage < MAX_TEST_PP_LOC_COUNT );

	std::vector<int> vLocList;
	double dTeachSafety[2][eAxisMax] = {0,};

	switch( nAxisIdx )
	{
	case CTaskTestPP::eAxisX1:
		{
// 			BOOL bSafetyZ = ChkDI(CTaskTestPP::xZ1AXIS_SAFETY_SEN_BC, DEF_ON);
// 			if(bSafetyZ != ERR_NO_ERROR){
// 				return ERR_MOT_INTERLOCK_TEST_PP_1_Z_IS_NOT_SAFETY + m_nTestPPIdx;
// 			}
		}break;
	case CTaskTestPP::eAxisX2:
	{
		OneAxis* pMotor = (OneAxis*)m_vMotor[eAxisZ2];
		if (pMotor->MotionDone() != ERR_NO_ERROR) {
			return ERR_TEST_PP_1_Y_MOVE_NOT_MOTION_DONE_Z2 + m_nTestPPIdx;
		}

// 		BOOL bSafetyZ = ChkDI(CTaskTestPP::xZ1AXIS_SAFETY_SEN_BC, DEF_ON);
// 		if (bSafetyZ != ERR_NO_ERROR) {
// 			return ERR_MOT_INTERLOCK_TEST_PP_1_Z_IS_NOT_SAFETY + m_nTestPPIdx;
// 		}

		switch (nLocStage)
		{
			//[작업 필요] station 1 UL 추가
			case LOC_IDX_STATION_1:
			case LOC_IDX_STATION_2:
			case LOC_IDX_STATION_3:
			case LOC_IDX_STATION_4:
			{
				//			int nPressIdx = ePressUnit_Down_1 + nLocStage - LOC_IDX_STATION_DN_1 + m_nTestPPIdx*(eMaxPressUnitCount / 4);
				int nPressIdx = ePressUnit_Down_1 + nLocStage - LOC_IDX_STATION_1 + m_nTestPPIdx*(eMaxPressUnitCount / 2);
				g_TaskPressUnit[nPressIdx].WhereIsMotorPos(vLocList, CTaskPressUnit::eAxisZ);
	
				if (CTaskPressUnit::eMotPos_Safety < vLocList[CTaskPressUnit::eAxisZ]) {
					return ERR_TEST_PP_1_MOVE_TEST_SITE_INTERLOCK + m_nTestPPIdx;
				}
				if (g_TaskPressUnit[nPressIdx].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON) != ERR_NO_ERROR) {
					return ERR_PRESS_UNIT_1_NOT_DETECT_SAFETY_SEN + nPressIdx;
				}
				if (CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto
					&& g_TaskPressUnit[nPressIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready)
				{
					return ERR_TEST_PP_1_MOVE_TEST_SITE_INTERLOCK + m_nTestPPIdx;
				}
			}break;
		}

	}break;
	case CTaskTestPP::eAxisY:
		{
			OneAxis* pMotor = (OneAxis*)m_vMotor[eAxisZ1];
			if( pMotor->MotionDone() != ERR_NO_ERROR){
				return ERR_TEST_PP_1_Y_MOVE_NOT_MOTION_DONE_Z1 + m_nTestPPIdx;
			}

// 			BOOL bSafetyZ = ChkDI(CTaskTestPP::xZ1AXIS_SAFETY_SEN_BC, DEF_ON);
// 			if(bSafetyZ != ERR_NO_ERROR){
// 				return ERR_MOT_INTERLOCK_TEST_PP_1_Z_IS_NOT_SAFETY + m_nTestPPIdx;
// 			}

			switch(nLocStage)
			{
			case LOC_IDX_STATION_1:
			case LOC_IDX_STATION_2:
			case LOC_IDX_STATION_3:
			case LOC_IDX_STATION_4:
				{
					int nPressIdx = ePressUnit_Down_1 + nLocStage- LOC_IDX_STATION_1 + m_nTestPPIdx*(eMaxPressUnitCount/2);
					g_TaskPressUnit[nPressIdx].WhereIsMotorPos(vLocList, CTaskPressUnit::eAxisZ);

// 					if(CTaskPressUnit::eMotPos_Safety < vLocList[CTaskPressUnit::eAxisZ]){
// 						return ERR_TEST_PP_1_MOVE_TEST_SITE_INTERLOCK + m_nTestPPIdx;
// 					}
// 					if( g_TaskPressUnit[nPressIdx].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON) != ERR_NO_ERROR){
// 						return ERR_PRESS_UNIT_1_NOT_DETECT_SAFETY_SEN + nPressIdx;
// 					}
// 					if( CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto
// 						&& g_TaskPressUnit[nPressIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready )
// 					{
// 						return ERR_TEST_PP_1_MOVE_TEST_SITE_INTERLOCK + m_nTestPPIdx;
// 					}
				}break;
			}			
		}break;
	case CTaskTestPP::eAxisZ1:
		{
			OneAxis* pMotor = (OneAxis*)m_vMotor[eAxisY];
			if( pMotor->MotionDone() != ERR_SUCCESS){
				return ERR_TEST_PP_1_Z1_MOVE_NOT_MOTION_DONE_Y + m_nTestPPIdx;
			}
		}break;
	case CTaskTestPP::eAxisZ2:
		{
			OneAxis* pMotor = (OneAxis*)m_vMotor[eAxisY];
			if (pMotor->MotionDone() != ERR_SUCCESS) {
				return ERR_TEST_PP_1_Z2_MOVE_NOT_MOTION_DONE_Y + m_nTestPPIdx;
			}
		}break;
	}



	return ERR_NO_ERROR;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-07]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskTestPP::WhereIsMotorPos(int nHandType, std::vector<int>& vLocList, int nAxisIdx/*=eAxisAll*/)
{
	int nPosition_x1 = -1;
	int nPosition_x2 = -1;
	int nPosition_y = -1;
	int nPosition_z1 = -1;
	int nPosition_z2 = -1;

// 	double dCurPosX = m_vMotor[CTaskTestPP::eAxisX]->GetCurrentPos();
// 	double dCurPosY = m_vMotor[CTaskTestPP::eAxisY]->GetCurrentPos();
// 	double dCurPosZ = m_vMotor[CTaskTestPP::eAxisZ]->GetCurrentPos();
	double dCurPosX1 = 0.0,dCurPosX2 = 0.0, dCurPosY = 0.0, dCurPosZ1 = 0.0, dCurPosZ2 = 0.0;
	m_vMotor[CTaskTestPP::eAxisX1]->GetCurrentPos(dCurPosX1);
	m_vMotor[CTaskTestPP::eAxisX2]->GetCurrentPos(dCurPosX2);
	m_vMotor[CTaskTestPP::eAxisY]->GetCurrentPos(dCurPosY);
	m_vMotor[CTaskTestPP::eAxisZ1]->GetCurrentPos(dCurPosZ1);
	m_vMotor[CTaskTestPP::eAxisZ2]->GetCurrentPos(dCurPosZ2);

	std::vector<CPoint> vPicker_RL1, vPicker_FR4;
	vPicker_RL1.push_back( CPoint(0,0) );
	vPicker_FR4.push_back( CPoint( MAX_TEST_PP_PKR_CNT_X-1, MAX_TEST_PP_PKR_CNT_Y-1 ) );

	double dTPosSafe[MAX_TEST_PP_LOC_COUNT][CTaskTestPP::ePos_Max] = {{0.0,},}; // 최대 Rear  방향 & Safety Z Teaching
	double dTPosPick[MAX_TEST_PP_LOC_COUNT][CTaskTestPP::ePos_Max] = {{0.0,},}; // 최대 Rear  방향 & Safety Z Teaching

	double dTRect_RL[MAX_TEST_PP_LOC_COUNT][CTaskTestPP::ePos_Max]   = {{0.0,},}; // 최대 Rear  방향 & Pick Z Teaching
	double dTRect_FR[MAX_TEST_PP_LOC_COUNT][CTaskTestPP::ePos_Max]   = {{0.0,},}; // 최대 Front 방향 & Pick Z Teaching

	int nSitePocketCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSitePocketCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nSitePitchX     = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Pitch_X);
	int nSitePitchY     = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Pitch_Y);

	// 계산법 =======================================
	// Teaching position으로 영역을 구분한다
	// -. 모든것은 전면에서 보는 기준!!
	// Ex) Test Site Rear Left pocket에서 hand의 front right picker가 활동하는 끝 영역이 된다.
	//     Test Site Front Right pocket에서 hand의 rear left picker가 활동하는 끝 영역이 된다.
	// teaching 기준으로 좌상점부터 우하점까지로 영역을 나누어 본다.

	std::vector<double> vCalcPos1, vCalcPos2;
	std::vector<double>::iterator vItCalsPos1, vItCalsPos2;

	for(int nStage=0; nStage<MAX_TEST_PP_LOC_COUNT; nStage++){
		// Get safety & pick position
		GetTeachPos(nStage, CPoint(0,0), vPicker_RL1, eZPOS_PICK  , dTPosPick[nStage], nHandType);

		GetTeachPos(nStage, CPoint(0,0), vPicker_RL1, eZPOS_SAFETY, dTPosSafe[nStage], nHandType);

		// Get rear left & front right rect position
		GetTeachPos(nStage, CPoint(0,0), vPicker_FR4, eZPOS_PICK  , dTRect_RL[nStage], nHandType);

		GetTeachPos(nStage, CPoint(nSitePocketCntX-1,nSitePocketCntY-1), vPicker_RL1, eZPOS_PICK  , dTRect_FR[nStage], nHandType);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Z1축 위치에 대한 판정. //Load
	vCalcPos1.clear();	// down site
	vCalcPos1.push_back(dTPosSafe[LOC_IDX_STATION_1][CTaskTestPP::eAxisZ1]);
	vCalcPos1.push_back(dTPosSafe[LOC_IDX_STATION_2][CTaskTestPP::eAxisZ1]);
	vCalcPos1.push_back(dTPosSafe[LOC_IDX_STATION_3][CTaskTestPP::eAxisZ1]);
	vCalcPos1.push_back(dTPosSafe[LOC_IDX_STATION_4][CTaskTestPP::eAxisZ1]);

	vCalcPos1.push_back(dTPosSafe[LOC_IDX_TEST_PP_TBL_1_LD][CTaskTestPP::eAxisZ1]);
	vCalcPos1.push_back(dTPosSafe[LOC_IDX_TEST_PP_TBL_2_LD][CTaskTestPP::eAxisZ1]);
	vCalcPos1.push_back(dTPosSafe[LOC_IDX_TEST_PP_TBL_3_LD][CTaskTestPP::eAxisZ1]);
	vCalcPos1.push_back(dTPosSafe[LOC_IDX_TEST_PP_TBL_4_LD][CTaskTestPP::eAxisZ1]);

	vCalcPos1.push_back(dTPosSafe[LOC_IDX_TEST_PP_TBL_1_UD][CTaskTestPP::eAxisZ1]);
	vCalcPos1.push_back(dTPosSafe[LOC_IDX_TEST_PP_TBL_2_UD][CTaskTestPP::eAxisZ1]);
	vCalcPos1.push_back(dTPosSafe[LOC_IDX_TEST_PP_TBL_3_UD][CTaskTestPP::eAxisZ1]);
	vCalcPos1.push_back(dTPosSafe[LOC_IDX_TEST_PP_TBL_4_UD][CTaskTestPP::eAxisZ1]);

	//vCalcPos1.push_back(dTPosSafe[LOC_IDX_CLEAN_TBL][CTaskTestPP::ePos_AxisZ1]);

	
	// down safety아래 ~ down pick/place
	vItCalsPos1 = std::max_element(std::begin(vCalcPos1), std::end(vCalcPos1));
	if( *vItCalsPos1 + 500.0 <= dCurPosZ1 )
		nPosition_z1 = ePosZ_DownSafetyToPosiLimit;

	// down safety
	vItCalsPos2 = std::min_element(std::begin(vCalcPos1), std::end(vCalcPos1));
	if (*vItCalsPos1 + 500.0 > dCurPosZ1 && *vItCalsPos2 - 2000.0 <= dCurPosZ1)
		nPosition_z1 = ePosZ_DownSafety;

	vItCalsPos2 = std::min_element(std::begin(vCalcPos1), std::end(vCalcPos1));
	//down Safety ~ down Limit
	if (*vItCalsPos2 - 500.0 > dCurPosZ1)
		nPosition_z1 = ePosZ_DownSafetyToNegLimit;
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Z2축 위치에 대한 판정. //Unload
	vCalcPos1.clear();	// down site

	vCalcPos1.push_back(dTPosSafe[LOC_IDX_STATION_1][CTaskTestPP::eAxisZ2]);
	vCalcPos1.push_back(dTPosSafe[LOC_IDX_STATION_2][CTaskTestPP::eAxisZ2]);
	vCalcPos1.push_back(dTPosSafe[LOC_IDX_STATION_3][CTaskTestPP::eAxisZ2]);
	vCalcPos1.push_back(dTPosSafe[LOC_IDX_STATION_4][CTaskTestPP::eAxisZ2]);

	vCalcPos1.push_back(dTPosSafe[LOC_IDX_TEST_PP_TBL_1_LD][CTaskTestPP::eAxisZ2]);
	vCalcPos1.push_back(dTPosSafe[LOC_IDX_TEST_PP_TBL_2_LD][CTaskTestPP::eAxisZ2]);
	vCalcPos1.push_back(dTPosSafe[LOC_IDX_TEST_PP_TBL_3_LD][CTaskTestPP::eAxisZ2]);
	vCalcPos1.push_back(dTPosSafe[LOC_IDX_TEST_PP_TBL_4_LD][CTaskTestPP::eAxisZ2]);

	vCalcPos1.push_back(dTPosSafe[LOC_IDX_TEST_PP_TBL_1_UD][CTaskTestPP::eAxisZ2]);
	vCalcPos1.push_back(dTPosSafe[LOC_IDX_TEST_PP_TBL_2_UD][CTaskTestPP::eAxisZ2]);
	vCalcPos1.push_back(dTPosSafe[LOC_IDX_TEST_PP_TBL_3_UD][CTaskTestPP::eAxisZ2]);
	vCalcPos1.push_back(dTPosSafe[LOC_IDX_TEST_PP_TBL_4_UD][CTaskTestPP::eAxisZ2]);

	vCalcPos1.push_back(dTPosSafe[LOC_IDX_CLEAN_TBL][CTaskTestPP::ePos_AxisZ2]);


	// down safety아래 ~ down pick/place
	vItCalsPos1 = std::max_element(std::begin(vCalcPos1), std::end(vCalcPos1));
	if (*vItCalsPos1 + 500.0 <= dCurPosZ2)
		nPosition_z2 = ePosZ_DownSafetyToPosiLimit;

	// down safety
	vItCalsPos2 = std::min_element(std::begin(vCalcPos1), std::end(vCalcPos1));
	if (*vItCalsPos1 + 500.0 > dCurPosZ2 && *vItCalsPos2 - 2000.0 <= dCurPosZ2)
		nPosition_z2 = ePosZ_DownSafety;

	vItCalsPos2 = std::max_element(std::begin(vCalcPos1), std::end(vCalcPos1));
	//down Safety ~ down Limit
	if (*vItCalsPos2 - 2000.0 > dCurPosZ2)
		nPosition_z2 = ePosZ_DownSafetyToNegLimit;
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// X1축 위치에 대한 판정.
	vCalcPos1.clear();	// test site
	vCalcPos1.push_back(dTRect_FR[LOC_IDX_STATION_1][CTaskTestPP::eAxisX1]); //[작업필요] teaching pos 확정시 변경
	vCalcPos1.push_back(dTRect_FR[LOC_IDX_STATION_2][CTaskTestPP::eAxisX1]);
	vCalcPos1.push_back(dTRect_FR[LOC_IDX_STATION_3][CTaskTestPP::eAxisX1]);
	vCalcPos1.push_back(dTRect_FR[LOC_IDX_STATION_4][CTaskTestPP::eAxisX1]);

	vCalcPos2.clear();	// table( buffer, clean, load )
// 	vCalcPos2.push_back(dTRect_RL[LOC_IDX_TEST_PP_TBL_1_LD][CTaskTestPP::ePos_AxisX1]);
// 	vCalcPos2.push_back(dTRect_RL[LOC_IDX_TEST_PP_TBL_2_LD][CTaskTestPP::ePos_AxisX1]);
// 	vCalcPos2.push_back(dTRect_RL[LOC_IDX_TEST_PP_TBL_3_LD][CTaskTestPP::ePos_AxisX1]);
// 	vCalcPos2.push_back(dTRect_RL[LOC_IDX_TEST_PP_TBL_4_LD][CTaskTestPP::ePos_AxisX1]);

	//X축은 Unload Table Teaching값이 제일 앞단.
	vCalcPos2.push_back(dTRect_RL[LOC_IDX_TEST_PP_TBL_1_UD][CTaskTestPP::eAxisX1]);
	vCalcPos2.push_back(dTRect_RL[LOC_IDX_TEST_PP_TBL_2_UD][CTaskTestPP::eAxisX1]);
	vCalcPos2.push_back(dTRect_RL[LOC_IDX_TEST_PP_TBL_3_UD][CTaskTestPP::eAxisX1]);
	vCalcPos2.push_back(dTRect_RL[LOC_IDX_TEST_PP_TBL_4_UD][CTaskTestPP::eAxisX1]);

// 		vCalcPos2.push_back(dTRect_RL[LOC_IDX_CLEAN_TBL][CTaskTestPP::ePos_AxisX1]);
// 		vCalcPos2.push_back(dTRect_RL[LOC_IDX_CLEAN_BUFFER][CTaskTestPP::ePos_AxisX]);

	if (m_nTestPPIdx == eTestPP_1) {
		if (dCurPosX1 <= INTERLOCK_TESTPP_1_X_STATION_START_POS) //station Pos
		{
			nPosition_x1 = ePosX_StationAndPosiLimit;
		}
		else {
			vItCalsPos1 = std::max_element(std::begin(vCalcPos1), std::end(vCalcPos1)); //X1 기준 작은 값.
			vItCalsPos2 = std::min_element(std::begin(vCalcPos2), std::end(vCalcPos2)); //X1 기준 큰 값.
			if (*vItCalsPos1 + 1000.0 < dCurPosX1 && dCurPosX1 < *vItCalsPos2 - 1000.0)
				nPosition_x1 = ePosX_StationToTable;

			if (*vItCalsPos2 - 1000.0 <= dCurPosX1)
				nPosition_x1 = ePosX_TableToNegaLimit;
		}
	}
	else {
		if (dCurPosX1 >= INTERLOCK_TESTPP_2_X_STATION_START_POS)
		{
			nPosition_x1 = ePosX_StationAndPosiLimit;
		}
		else {
			vItCalsPos1 = std::min_element(std::begin(vCalcPos1), std::end(vCalcPos1));
			vItCalsPos2 = std::max_element(std::begin(vCalcPos2), std::end(vCalcPos2));
			if (*vItCalsPos1 - 1000.0 > dCurPosX1 && *vItCalsPos2 + 1000.0 < dCurPosX1)
				nPosition_x1 = ePosX_StationToTable;

			if (*vItCalsPos2 + 1000.0 >= dCurPosX1)
				nPosition_x1 = ePosX_TableToNegaLimit;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// X2축 위치에 대한 판정.

	vCalcPos1.clear();	// test site
	vCalcPos1.push_back(dTRect_FR[LOC_IDX_STATION_1][CTaskTestPP::eAxisX2]); //[작업필요] teaching pos 확정시 변경
	vCalcPos1.push_back(dTRect_FR[LOC_IDX_STATION_2][CTaskTestPP::eAxisX2]);
	vCalcPos1.push_back(dTRect_FR[LOC_IDX_STATION_3][CTaskTestPP::eAxisX2]);
	vCalcPos1.push_back(dTRect_FR[LOC_IDX_STATION_4][CTaskTestPP::eAxisX2]);

	vCalcPos2.clear();	// table( buffer, clean, load )
// 	vCalcPos2.push_back(dTRect_RL[LOC_IDX_TEST_PP_TBL_1_LD][CTaskTestPP::ePos_AxisX2]);
// 	vCalcPos2.push_back(dTRect_RL[LOC_IDX_TEST_PP_TBL_2_LD][CTaskTestPP::ePos_AxisX2]);
// 	vCalcPos2.push_back(dTRect_RL[LOC_IDX_TEST_PP_TBL_3_LD][CTaskTestPP::ePos_AxisX2]);
// 	vCalcPos2.push_back(dTRect_RL[LOC_IDX_TEST_PP_TBL_4_LD][CTaskTestPP::ePos_AxisX2]);

	//X축은 Unload Table Teaching값이 제일 앞단.
	vCalcPos2.push_back(dTRect_RL[LOC_IDX_TEST_PP_TBL_1_UD][CTaskTestPP::eAxisX2]);
	vCalcPos2.push_back(dTRect_RL[LOC_IDX_TEST_PP_TBL_2_UD][CTaskTestPP::eAxisX2]);
	vCalcPos2.push_back(dTRect_RL[LOC_IDX_TEST_PP_TBL_3_UD][CTaskTestPP::eAxisX2]);
	vCalcPos2.push_back(dTRect_RL[LOC_IDX_TEST_PP_TBL_4_UD][CTaskTestPP::eAxisX2]);

	if (m_nTestPPIdx == eTestPP_1) {
		if (dCurPosX2 <= INTERLOCK_TESTPP_1_X_STATION_START_POS) //station Pos
		{
			nPosition_x2 = ePosX_StationAndPosiLimit;
		}
		else {
			vItCalsPos1 = std::max_element(std::begin(vCalcPos1), std::end(vCalcPos1)); //X1 기준 작은 값.
			vItCalsPos2 = std::min_element(std::begin(vCalcPos2), std::end(vCalcPos2)); //X1 기준 큰 값.
			if (*vItCalsPos1 + 1000.0 < dCurPosX2 && dCurPosX2 < *vItCalsPos2 - 1000.0)
				nPosition_x2 = ePosX_StationToTable;

			if (*vItCalsPos2 - 1000.0 <= dCurPosX2)
				nPosition_x2 = ePosX_TableToNegaLimit;
		}
	}
	else {
		if (dCurPosX2 >= INTERLOCK_TESTPP_2_X_STATION_START_POS)
		{
			nPosition_x2 = ePosX_StationAndPosiLimit;
		}
		else {
			vItCalsPos1 = std::min_element(std::begin(vCalcPos1), std::end(vCalcPos1));
			vItCalsPos2 = std::max_element(std::begin(vCalcPos2), std::end(vCalcPos2));
			if (*vItCalsPos1 - 1000.0 > dCurPosX2 && *vItCalsPos2 + 1000.0 < dCurPosX2)
				nPosition_x2 = ePosX_StationToTable;

			if (*vItCalsPos2 + 1000.0 >= dCurPosX2)
				nPosition_x2 = ePosX_TableToNegaLimit;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Y축 위치에 대한 판정. [작업 필요] Loading tbl, clean, buffer tbl teaching pos 확정 시, 재작업 필요.
	{
		// Y1
		if (nHandType == eTestPPLoadType)
		{
			if (nPosition_x1 == ePosX_TableToNegaLimit) {
				if (dCurPosY <= dTRect_FR[LOC_IDX_TEST_PP_TBL_1_LD][CTaskTestPP::eAxisY]
					&& dTRect_RL[LOC_IDX_TEST_PP_TBL_1_LD][CTaskTestPP::eAxisY] <= dCurPosY) {

					nPosition_y = ePosY_LoadTable1;
				}
				else if (dCurPosY <= dTRect_FR[LOC_IDX_TEST_PP_TBL_2_LD][CTaskTestPP::eAxisY]
					&& dTRect_RL[LOC_IDX_TEST_PP_TBL_2_LD][CTaskTestPP::eAxisY] <= dCurPosY) {
					nPosition_y = ePosY_LoadTable2;
				}
				else if (dCurPosY <= dTRect_FR[LOC_IDX_TEST_PP_TBL_3_LD][CTaskTestPP::eAxisY]
					&& dTRect_RL[LOC_IDX_TEST_PP_TBL_3_LD][CTaskTestPP::eAxisY] <= dCurPosY) {
					nPosition_y = ePosY_LoadTable3;
				}
				else if (dCurPosY <= dTRect_FR[LOC_IDX_TEST_PP_TBL_4_LD][CTaskTestPP::eAxisY]
					&& dTRect_RL[LOC_IDX_TEST_PP_TBL_4_LD][CTaskTestPP::eAxisY] <= dCurPosY) {
					nPosition_y = ePosY_LoadTable4;
				}

				else if (dTRect_RL[LOC_IDX_CLEAN_TBL][CTaskTestPP::eAxisY] <= dCurPosY
					&& dTRect_FR[LOC_IDX_CLEAN_TBL][CTaskTestPP::eAxisY] >= dCurPosY) {

					nPosition_y = ePosY_ArtOrCleanBuffer;
				}
				else {
					nPosition_y = ePosY_Etc;
				}
			}

			// station & table사이 일 경우에는 station에 맞게
			else {
				nPosition_y = ePosY_NoInStation;
				for (int i = 0; i < (eMaxPressUnitCount / 2) && (ePosY_NoInStation == nPosition_y); i++)
				{
// 					double dStationCenter = 0.0;
// 					if (m_nTestPPIdx == eTestPP_1)
// 						dStationCenter = (dTRect_FR[LOC_IDX_STATION_1 + i][CTaskTestPP::ePos_AxisY1] + dTRect_RL[LOC_IDX_STATION_1 + i][CTaskTestPP::ePos_AxisY1]) / 2.0;
// 					else
// 						dStationCenter = (dTRect_FR[LOC_IDX_STATION_1 + i][CTaskTestPP::ePos_AxisY1] + dTRect_RL[LOC_IDX_STATION_1 + i][CTaskTestPP::ePos_AxisY1]) / 2.0;

					double dStationCenter = (dTRect_FR[LOC_IDX_STATION_1 + i][CTaskTestPP::eAxisY] + dTRect_RL[LOC_IDX_STATION_1 + i][CTaskTestPP::eAxisY]) / 2.0;
					if (dStationCenter + STATION_CENTER_TO_LR_HW_DIST >= dCurPosY && dStationCenter - STATION_CENTER_TO_LR_HW_DIST <= dCurPosY)
						nPosition_y = ePosY_StationL1 + i;
				}
			}
		}
		else
		{
			if (nPosition_x1 == ePosX_TableToNegaLimit) {
				if (dCurPosY <= dTRect_FR[LOC_IDX_TEST_PP_TBL_1_UD][CTaskTestPP::eAxisY]
					&& dTRect_RL[LOC_IDX_TEST_PP_TBL_1_UD][CTaskTestPP::eAxisY] <= dCurPosY) {

					nPosition_y = ePosY_LoadTable1;
				}
				else if (dCurPosY <= dTRect_FR[LOC_IDX_TEST_PP_TBL_2_UD][CTaskTestPP::eAxisY]
					&& dTRect_RL[LOC_IDX_TEST_PP_TBL_2_UD][CTaskTestPP::eAxisY] <= dCurPosY) {
					nPosition_y = ePosY_LoadTable2;
				}
				else if (dCurPosY <= dTRect_FR[LOC_IDX_TEST_PP_TBL_3_UD][CTaskTestPP::eAxisY]
					&& dTRect_RL[LOC_IDX_TEST_PP_TBL_3_UD][CTaskTestPP::eAxisY] <= dCurPosY) {
					nPosition_y = ePosY_LoadTable3;
				}
				else if (dCurPosY <= dTRect_FR[LOC_IDX_TEST_PP_TBL_4_UD][CTaskTestPP::eAxisY]
					&& dTRect_RL[LOC_IDX_TEST_PP_TBL_4_UD][CTaskTestPP::eAxisY] <= dCurPosY) {
					nPosition_y = ePosY_LoadTable4;
				}

				else if (dTRect_RL[LOC_IDX_CLEAN_TBL][CTaskTestPP::eAxisY] <= dCurPosY
					&& dTRect_FR[LOC_IDX_CLEAN_TBL][CTaskTestPP::eAxisY] >= dCurPosY) {

					nPosition_y = ePosY_ArtOrCleanBuffer;
				}
				else {
					nPosition_y = ePosY_Etc;
				}
			}

			// station & table사이 일 경우에는 station에 맞게
			else {
				nPosition_y = ePosY_NoInStation;
				for (int i = 0; i < (eMaxPressUnitCount / 2) && (ePosY_NoInStation == nPosition_y); i++)
				{
					// 					double dStationCenter = 0.0;
					// 					if (m_nTestPPIdx == eTestPP_1)
					// 						dStationCenter = (dTRect_FR[LOC_IDX_STATION_1 + i][CTaskTestPP::ePos_AxisY1] + dTRect_RL[LOC_IDX_STATION_1 + i][CTaskTestPP::ePos_AxisY1]) / 2.0;
					// 					else
					// 						dStationCenter = (dTRect_FR[LOC_IDX_STATION_1 + i][CTaskTestPP::ePos_AxisY1] + dTRect_RL[LOC_IDX_STATION_1 + i][CTaskTestPP::ePos_AxisY1]) / 2.0;

					double dStationCenter = (dTRect_FR[LOC_IDX_STATION_1 + i][CTaskTestPP::eAxisY] + dTRect_RL[LOC_IDX_STATION_1 + i][CTaskTestPP::eAxisY]) / 2.0;
					if (dStationCenter + STATION_CENTER_TO_LR_HW_DIST >= dCurPosY && dStationCenter - STATION_CENTER_TO_LR_HW_DIST <= dCurPosY)
						nPosition_y = ePosY_StationL1 + i;
				}
			}
		}
	}

	ASSERT(nPosition_z1 != -1 || nPosition_z2 != -1 || nPosition_y != -1 || nPosition_x1 != -1 || nPosition_x2 != -1);
	if( nAxisIdx == eAxisX1 )
	{
		vLocList.push_back(nPosition_x1);
	}
	else if (nAxisIdx == eAxisX2)
	{
		vLocList.push_back(nPosition_x2);
	}
	else if( nAxisIdx == eAxisY )
	{
		vLocList.push_back(nPosition_y);
	}
	else if( nAxisIdx == eAxisZ1 )
	{
		vLocList.push_back(nPosition_z1);
	}
	else if (nAxisIdx == eAxisZ2)
	{
		vLocList.push_back(nPosition_z1);
	}
	else
	{
		vLocList.push_back(nPosition_x1);
		vLocList.push_back(nPosition_x2);
		vLocList.push_back(nPosition_y);
		vLocList.push_back(nPosition_z1);
		vLocList.push_back(nPosition_z2);
	}

	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 김선진  [2015-11-19]
  NAME     : 
  PARAMS   : 
  RETURN   : enum eLocIdxTestPP  :: 해당 위치에서는 Z축을 먼저 잡아야 한다.
             -1 : Y축을 먼저 잡아도 되는 경우
  COMMENTS : TestPP가 Teaching Position을 비교하여 Safety 영역의 Teaching Target Stage 가 어디인지 찾도록 한다.
             동작 이후 사용자가 움직여 놓았을 경우 Pp가 안전 위치로 도피하여 Y축을 이동하여야 하는지 판단하도록 한다.
==============================================================================*/


int CTaskTestPP::do_VacBlow(int nVacBlow,int nHandType ,CPoint Picker)
{
	ASSERT( Picker.x >= 0 && Picker.x < MAX_TEST_PP_PKR_CNT_X );
	ASSERT( Picker.y >= 0 && Picker.y < MAX_TEST_PP_PKR_CNT_Y );


	int nErr = Interlock_VacBlow(nVacBlow, Picker);
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}

	if (g_IO.GetDriverType() != _DIO_TYPE_SIMUL_ )
	{
		BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
		int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
		if (nDeviceMode != eDeviceMode && g_START_SW.GetStatus() == TRUE)
		{
			return ERR_NO_ERROR;
		}
	}

	if( nVacBlow == eVacBlwSt_Off ){
		g_IO.out(m_nHand_o_Vac[nHandType][Picker.y][Picker.x], DEF_OFF);
		g_IO.out(m_nHand_o_Blw[nHandType][Picker.y][Picker.x], DEF_OFF);
		g_IO.out(m_nHand_o_Reset[nHandType][Picker.y][Picker.x], DEF_OFF);
	}else if( nVacBlow == eVacBlwSt_VacOn ){
		g_IO.out(m_nHand_o_Blw[nHandType][Picker.y][Picker.x], DEF_OFF);
		g_IO.out(m_nHand_o_Reset[nHandType][Picker.y][Picker.x], DEF_OFF);
		g_IO.out(m_nHand_o_Vac[nHandType][Picker.y][Picker.x], DEF_ON);
	}else if( nVacBlow == eVacBlwSt_BlwOn ){
		g_IO.out(m_nHand_o_Vac[nHandType][Picker.y][Picker.x], DEF_OFF);
		g_IO.out(m_nHand_o_Reset[nHandType][Picker.y][Picker.x], DEF_ON);
		g_IO.out(m_nHand_o_Blw[nHandType][Picker.y][Picker.x], DEF_ON);
	}

	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-07]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskTestPP::do_VacBlow(int nVacBlow,int nHandType ,std::vector<CPoint> vPicker)
{
	int nErr = Interlock_VacBlow(nVacBlow, vPicker);
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}

	Sleep(EJECTOR_CHANGE_MIN_TIME);
	for( std::vector<CPoint>::size_type i=0; i<vPicker.size(); i++){
		do_VacBlow(nVacBlow, nHandType, vPicker[i]);
	}

	if(nVacBlow == eVacBlwSt_BlwOn){
		int nSleep = g_DMCont.m_dmTestPP.GN(NDM4_TM_TestPP1_Blow + 2*m_nTestPPIdx);
		if(nSleep <= 0){ nSleep = MIN_VAC_BLOW_TIME;	}
		Sleep(nSleep);
		for( std::vector<CPoint>::size_type i=0; i<vPicker.size(); i++){
			do_VacBlow(eVacBlwSt_Off, nHandType, vPicker[i]);
		}
	}

	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-07]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskTestPP::Interlock_VacBlow(int nVacBlow, CPoint Picker)
{
	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-07]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskTestPP::Interlock_VacBlow(int nVacBlow, std::vector<CPoint> vPicker)
{
	return ERR_NO_ERROR;
}
 
int CTaskTestPP::do_VacBlow_All(int nVacBlow, int nHandType)
{
	std::vector<CPoint> vPicker;
	MakeAllPicker( vPicker );
	return do_VacBlow(nVacBlow, nHandType, vPicker);
}

int CTaskTestPP::do_NoDeviceResetAll()
{
	std::vector<CPoint> vPicker;
	std::vector<CPoint> vPicker_Act;
	MakeAllPicker(vPicker);
	for (int nHandType = 0; nHandType < NAX_TEST_PP_HEAD_COUNT; nHandType++)
	{
		for (std::vector<CPoint>::size_type i = 0; i < vPicker.size(); i++) {
			int nVac = GetVacuum(nHandType,vPicker[i]);
			if (nVac != DEF_ON) {
				do_VacBlow(eVacBlwSt_BlwOn, nHandType, vPicker[i]);
				vPicker_Act.push_back(vPicker[i]);
			}
		}
	}
	Sleep(50);
	for (int nHandType = 0; nHandType < NAX_TEST_PP_HEAD_COUNT; nHandType++)
	{
		for (std::vector<CPoint>::size_type i = 0; i < vPicker_Act.size(); i++)
		{
			do_VacBlow(eVacBlwSt_Off, nHandType, vPicker_Act[i]);
		}
	}

	return ERR_NO_ERROR;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-03]
  NAME     : 
  PARAMS   : bOnOff : 0 = Off되었는지 점검함. 1 = On 되었는지 점검함.
             nPicker: 점검 할 Picker를 지정 함.
  RETURN   : 0 = OK
            -1 = 지정한 상태가 아님.
  COMMENTS : Vacuum 이 ON 상태인지 OFF 상태인지 점검 함.
==============================================================================*/
int CTaskTestPP::GetVacuum(int nHandType, CPoint Picker)
{
	ASSERT( Picker.x >= 0 && Picker.x < MAX_TEST_PP_PKR_CNT_X );
	ASSERT( Picker.y >= 0 && Picker.y < MAX_TEST_PP_PKR_CNT_Y );

	return g_IO.in( m_nHand_i_VacSen[nHandType][Picker.y][Picker.x] );
}

int CTaskTestPP::GetVacuum_All(int nHandType, BOOL bOnOff)
{
	std::vector<CPoint> vPicker;
	MakeAllPicker(vPicker);

	for (std::vector<CPoint>::size_type i = 0; i < vPicker.size(); i++) {
		int nVac = GetVacuum(nHandType, vPicker[i]);
		if (nVac != bOnOff) {
			if (nHandType == eTestPPLoadType)
				return ERR_TEST_PP_1_X1_VAC_SEN_ERR + nHandType;
			else
				return ERR_TEST_PP_2_X1_VAC_SEN_ERR + nHandType;
		}
	}
	
	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-03]
  NAME     : 
  PARAMS   : bOnOff : 0 = Off되었는지 점검함. 1 = On 되었는지 점검함.
             nPicker: 점검 할 Picker를 지정 함.
  RETURN   : 0 = OK
            -1 = 지정한 상태가 아님.
  COMMENTS : Vacuum 이 ON 상태인지 OFF 상태인지 점검 함.
==============================================================================*/
int CTaskTestPP::ChkVacuum(BOOL bOnOff,int nHandType, CPoint Picker)
{
	ASSERT( Picker.x >= 0 && Picker.x < MAX_TEST_PP_PKR_CNT_X );
	ASSERT( Picker.y >= 0 && Picker.y < MAX_TEST_PP_PKR_CNT_Y );

	int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
	BOOL vat_in_process = g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_USE);

	if( nDeviceMode != eDeviceMode && vat_in_process != TRUE)
	{
		return ERR_NO_ERROR;
	}

	if( g_IO.GetDriverType() == _DIO_TYPE_SIMUL_ ){
		//NEXT;
		g_IO.SetSimulDi(m_nHand_i_VacSen[nHandType][Picker.y][Picker.x], bOnOff);
		Sleep(SIMUL_SLEEP_TIME);
		while (g_IO.in(m_nHand_i_VacSen[nHandType][Picker.y][Picker.x]) != bOnOff) {
			g_IO.SetSimulDi(m_nHand_i_VacSen[nHandType][Picker.y][Picker.x], bOnOff);
			Sleep(SIMUL_SLEEP_TIME);
		}
	}
	

	if( g_IO.in( m_nHand_i_VacSen[nHandType][Picker.y][Picker.x]) != bOnOff ){
		return ERR_TEST_PP_1_X1_VAC_SEN_ERR + m_nTestPPIdx;
	}

	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-07]
  NAME     : 
  PARAMS   : bOnOff : 0 = Off되었는지 점검함. 1 = On 되었는지 점검함.
             vPicker: 점검 할 Picker를 지정 함.
  RETURN   : 0 = OK
            -1 = 지정한 상태가 아님.
  COMMENTS : Vacuum 이 ON 상태인지 OFF 상태인지 점검 함.
==============================================================================*/
int CTaskTestPP::ChkVacuum(BOOL bOnOff, int nHandType, std::vector<CPoint> vPicker)
{
	for( std::vector<CPoint>::size_type i = 0; i<vPicker.size(); i++){
		int nErr = ChkVacuum(bOnOff, nHandType, vPicker[i]);
		if( nErr != ERR_NO_ERROR ){
			m_vFailPicker[nHandType].push_back(vPicker[i]);
			return nErr;
		}
	}

	return ERR_NO_ERROR;
}

int CTaskTestPP::ChkVacuum_All(BOOL bOnOff)
{
// 	std::vector<CPoint> vPicker;
// 	MakeAllPicker( vPicker );
// 	return ChkVacuum(bOnOff, vPicker);
	return NO_ERROR;
}

void CTaskTestPP::MakeAllPicker(std::vector<CPoint>& vPicker)
{
	for(int y=0; y<MAX_TEST_PP_PKR_CNT_Y; y++){
		for(int x=0; x<MAX_TEST_PP_PKR_CNT_X; x++){
			vPicker.push_back(CPoint(x,y));
		}
	}
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-08-25]
  NAME     : 
  PARAMS   : 
  RETURN   : 0 = OK, -1 = 지정한 상태가 아니다.
  COMMENTS : DI의 상태를 점검 한다. 시퀀스에서 이 함수를 사용하라.
             bValue로 정상인 상태의 값을 넘겨 주면 시뮬레이션 모드에서 강제로 센서 값을 바꾼다.
			 Display와 같은 단순 조회만 할꺼라면 GetDI() 함수를 사용 하라.
==============================================================================*/
int CTaskTestPP::ChkDI(int nIndex, BOOL bValue/*=TRUE*/)
{
	ASSERT( nIndex >=0 && nIndex < (int)m_vX.size() );

	if( g_IO.GetDriverType() == _DIO_TYPE_SIMUL_ ){
		g_IO.SetSimulDi(m_vX[nIndex], bValue);
		Sleep(SIMUL_SLEEP_TIME);
	}

	if( g_IO.in(m_vX[nIndex]) != bValue ){
		return -1;
	}

	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-30]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 센서 상태를 반환 한다. 단순 조회만 한다면 이 함수를 사용 할 것.            
==============================================================================*/
BOOL CTaskTestPP::GetDI(int nIndex)
{
	ASSERT( nIndex >=0 && nIndex < (int)m_vX.size() );

	return (BOOL)g_IO.in(m_vX[nIndex]);
}

/*=============================================================================
  AUTHOR   : 김선진  [2019-01-16]
  NAME     : 
  PARAMS   :  nStageCmd = LOC Cmd ID
  RETURN   :  DeviceStage ID
  COMMENTS : Cmd Stage를 통해서 device 저장 위치를 가져 온다.
==============================================================================*/
int CTaskTestPP::ConvertStageIdx(int nStageCmd) //[작업 필요]
{
	ASSERT( nStageCmd >= LOC_IDX_STATION_1 && nStageCmd <= LOC_IDX_CLEAN_TBL);

 	int nStageIdx = -1;

	switch (nStageCmd)
	{
	case LOC_IDX_TEST_PP_TBL_1_LD:	
	case LOC_IDX_TEST_PP_TBL_2_LD: 
	case LOC_IDX_TEST_PP_TBL_3_LD: 
	case LOC_IDX_TEST_PP_TBL_4_LD:
	{
		if (m_nTestPPIdx == eTestPP_1)
			nStageIdx = eDeviceStage_LD_TBL_2;
		else
			nStageIdx = eDeviceStage_LD_TBL_3;
	}break;
	case LOC_IDX_TEST_PP_TBL_1_UD:
	case LOC_IDX_TEST_PP_TBL_2_UD: 
	case LOC_IDX_TEST_PP_TBL_3_UD: 
	case LOC_IDX_TEST_PP_TBL_4_UD: 
	{
		if (m_nTestPPIdx == eTestPP_1)
			nStageIdx = eDeviceStage_LD_TBL_1;
		else
			nStageIdx = eDeviceStage_LD_TBL_4;
	}break;
	case LOC_IDX_STATION_1: { nStageIdx = (eDeviceStage_TEST_SITE_1 + ((eMaxPressUnitCount / 2) * m_nTestPPIdx)); } break;
	case LOC_IDX_STATION_2: { nStageIdx = (eDeviceStage_TEST_SITE_2 + ((eMaxPressUnitCount / 2) * m_nTestPPIdx)); } break;
	case LOC_IDX_STATION_3: { nStageIdx = (eDeviceStage_TEST_SITE_3 + ((eMaxPressUnitCount / 2) * m_nTestPPIdx)); } break;
	case LOC_IDX_STATION_4: { nStageIdx = (eDeviceStage_TEST_SITE_4 + ((eMaxPressUnitCount / 2) * m_nTestPPIdx)); } break;
	case LOC_IDX_CLEAN_TBL:
	{
		nStageIdx = eDeviceStage_TEST_PP_1_CLEAN_BUFF + m_nTestPPIdx;
	}break;
  	default: { ASSERT(0); } break;
  	}

	return nStageIdx;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-07]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskTestPP::GetHandPkrDvcExist(CPoint Picker, int nHandType)
{
	ASSERT( Picker.x >= 0 && Picker.x < MAX_TEST_PP_PKR_CNT_X );
	ASSERT( Picker.y >= 0 && Picker.y < MAX_TEST_PP_PKR_CNT_Y );

	if(nHandType == eTestPPLoadType)
		return g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TEST_PP_1+m_nTestPPIdx, Picker.x, Picker.y);
	else
		return g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TEST_PP_1 + m_nTestPPIdx, Picker.x, MAX_TEST_PP_PKR_CNT_Y + Picker.y);
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-07]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskTestPP::SetHandPkrDvcExist(CPoint Picker, int nExist)
{
	ASSERT( Picker.x >= 0 && Picker.x < MAX_TEST_PP_PKR_CNT_X );
	ASSERT( Picker.y >= 0 && Picker.y < MAX_TEST_PP_PKR_CNT_Y );

	g_DMCont.m_dmDevice.SetDeviceExist(eDeviceStage_TEST_PP_1+m_nTestPPIdx, Picker.x, Picker.y, nExist);
	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : Check Motor Initialize status
  NAME     : 김선진
  PARAMS   : None
  RETURN   : TRUE  : Need Initial,  FALSE : No need initialzie
  COMMENTS : Initialize 작업이 필요한 Motor 가 존재 하는지 확인.
==============================================================================*/
BOOL CTaskTestPP::IsInitNeed()
{
	for(std::vector<OneAxis*>::size_type i=0; i < m_vMotor.size(); i++){
		OneAxis* pMotor = (OneAxis*)m_vMotor[i];

		if(pMotor->ChkMotorUsable() != ERR_NO_ERROR){
			return TRUE;
		}
	}
	return FALSE;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : None
  RETURN   : Load Empty Picker Count
  COMMENTS : Test PP의 Load용 Picker에 Empty Picker가 몇개 있는지 확인.
==============================================================================*/
int CTaskTestPP::GetLoadPickerDeviceCnt()
{
	int nCnt = 0;
	for(int nX=0; nX<MAX_TEST_PP_PKR_CNT_X; nX++){
		for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++) {
			nCnt += g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TEST_PP_1 + m_nTestPPIdx, nX, nY);
		}
	}
	return nCnt;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : 
  RETURN   : Unload Empty Picker Count
  COMMENTS : Test PP의 Unload용 Picker가 가지고 있는 Device Count
==============================================================================*/
int CTaskTestPP::GetUnloadPickerHaveDvcCnt()
{
	int nCnt = 0;
	for(int nX=0; nX<MAX_TEST_PP_PKR_CNT_X; nX++){
		for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++) {
			nCnt += g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TEST_PP_1 + m_nTestPPIdx, nX, nY+ TEST_UD_PKR_Y);
		}
	}
	return nCnt;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : Device에 대한 모든 정보를 가져 온다
==============================================================================*/
int CTaskTestPP::GetHandDvcData(CPoint Picker, int nHandType, ST_DD_DEVICE* lpstBuff)
{
	ASSERT( Picker.x >= 0 && Picker.x < MAX_TEST_PP_PKR_CNT_X );
	ASSERT( Picker.y >= 0 && Picker.y < MAX_TEST_PP_PKR_CNT_Y );

	int nErr;

	if(nHandType == eTestPPLoadType)
		nErr = g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1+ m_nTestPPIdx, Picker.x, Picker.y, lpstBuff);
	else
		nErr = g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_nTestPPIdx, Picker.x, Picker.y + TEST_UD_PKR_Y, lpstBuff);

	return nErr;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : Device에 대한 모든 정보를 셋팅 한다.
==============================================================================*/
int CTaskTestPP::SetHandDvcData(CPoint Picker, int nHandType, ST_DD_DEVICE* lpstBuff)
{
	ASSERT( Picker.x >= 0 && Picker.x < MAX_TEST_PP_PKR_CNT_X );
	ASSERT( Picker.y >= 0 && Picker.y < MAX_TEST_PP_PKR_CNT_Y );

	int nErr;

	if (nHandType == eTestPPLoadType)
		nErr = g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_PP_1+m_nTestPPIdx, Picker.x, Picker.y, lpstBuff);
	else 
		nErr = g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_PP_1 + m_nTestPPIdx, Picker.x, TEST_UD_PKR_Y + Picker.y, lpstBuff);

	return nErr;
}

int CTaskTestPP::GetCmdStageDvcData(int nCmdStage, CPoint Target, int nHandType, ST_DD_DEVICE* lpstBuff)
{
	int nDeviceStage = ConvertStageIdx(nCmdStage);
	int nErr = g_DMCont.m_dmDevice.GetDeviceData(nDeviceStage, Target.x, Target.y, lpstBuff);
	return nErr;
}

int CTaskTestPP::SetCmdStageDvcData(int nCmdStage, CPoint Target, int nHandType, ST_DD_DEVICE* lpstBuff)
{
	int nDeviceStage = ConvertStageIdx(nCmdStage);
	int nErr = g_DMCont.m_dmDevice.SetDeviceData(nDeviceStage, Target.x, Target.y, lpstBuff);
	return nErr;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : Device Data를 Target Cmd Stage에 따라서 데이터 처리 Load Tbl->Test PP or Test PP->Test Site
==============================================================================*/
int CTaskTestPP::ProcDeviceData(int nCmdStage, int nHandType, int nTargetX, int nTargetY, std::vector<CPoint> vCmdPicker, int nPickPlace, std::vector<CString>& vErrstring, int& nRetDeviceType, BOOL bMisMatchErase)
{
	ASSERT( nCmdStage >= 0 );
	ASSERT( (nTargetX >=0) && (nTargetX <DEVICE_DATA_MAX_X) );
	ASSERT( (nTargetY >=0) && (nTargetY <DEVICE_DATA_MAX_Y) );
	ASSERT( vCmdPicker.size() != 0 );

	ST_DD_DEVICE stSourceData;
	ST_DD_DEVICE stTargetData;
	ST_DD_DEVICE stEmptyDevice; stEmptyDevice.clear();
	m_vFailPicker[nHandType].clear();

	//[작업 필요]
	// hand pitch type check <srt> ==============================================
	int nX_PitchType = GetHandPitchType(CTaskTestPP::eAxisPitch_HW_X1_SW_Y1, nCmdStage);	// Hand와 Target간의 간격 관계 (0=1by1, 1=1:1매칭, 2=한칸뛰기,3=두칸뛰기), Hand 기준임. (Hand 1,3으로 Target 1,2 집는것은 처리 필요)
	int nY_PitchType = GetHandPitchType(CTaskTestPP::eAxisPitch_HW_Y1_SW_X1, nCmdStage);	// Hand와 Target간의 간격 관계 (0=1by1, 1=1:1매칭, 2=한칸뛰기,3=두칸뛰기), Hand 기준임. (Hand 1,3으로 Target 1,2 집는것은 처리 필요)
	// hand pitch type check <end> ==============================================

	int nX=0, nY=0, nRetError = ERR_NO_ERROR;
	switch(nCmdStage)
	{
	case LOC_IDX_STATION_1:  
	case LOC_IDX_STATION_2:  
	case LOC_IDX_STATION_3:  
	case LOC_IDX_STATION_4:  
	case LOC_IDX_TEST_PP_TBL_1_LD: 
	case LOC_IDX_TEST_PP_TBL_2_LD: 
	case LOC_IDX_TEST_PP_TBL_3_LD: 
	case LOC_IDX_TEST_PP_TBL_4_LD: 
	case LOC_IDX_TEST_PP_TBL_1_UD:
	case LOC_IDX_TEST_PP_TBL_2_UD:
	case LOC_IDX_TEST_PP_TBL_3_UD:
	case LOC_IDX_TEST_PP_TBL_4_UD:
	case LOC_IDX_CLEAN_TBL:  
		{
			typedef std::vector<CPoint>::const_iterator vIterPoint;
			std::pair<vIterPoint, vIterPoint> minX = std::minmax_element(vCmdPicker.begin(), vCmdPicker.end(), LessByMinX);
			std::pair<vIterPoint, vIterPoint> minY = std::minmax_element(vCmdPicker.begin(), vCmdPicker.end(), LessByMinY);
		
			if(nPickPlace == DEF_PICK)
			{
				for(int i=0; i < (int)vCmdPicker.size(); i++)
				{
					// change picker position
					int nStartPickerX = vCmdPicker[i].x -minX.first->x;
					int nStartPickerY = vCmdPicker[i].y -minY.first->y;

					if( nX_PitchType == 2 && vCmdPicker[i].x != minX.first->x ){
						nStartPickerX *=nX_PitchType;
					}
					if( nY_PitchType == 2 && vCmdPicker[i].y != minY.first->y ){
						nStartPickerY *=nY_PitchType;
					}

					int nPosX = nTargetX + nStartPickerX;
					int nPosY = nTargetY + nStartPickerY;

					stSourceData.clear();
					GetCmdStageDvcData(nCmdStage, CPoint(nPosX, nPosY), nHandType, &stSourceData);

					// Get Target data ( Target = Hand )
					stTargetData.clear();
					GetHandDvcData(vCmdPicker[i], nHandType, &stTargetData);

					// checking hand picker vacuum sensor 
					int nChkSen = ChkVacuum(DEF_ON, nHandType, vCmdPicker[i]);

				
					// ============================================================================================================
					// case 1 :: Source device exist, hand device sensing
					// ============================================================================================================
					if( stSourceData.sExist == eDeviceData_Exist && stTargetData.sExist != eDeviceData_Exist && nChkSen == ERR_NO_ERROR){
						//Device Pick History
						stSourceData.stHistory.nStage = CmdStageHistoryStageConvertor(nCmdStage);
						stSourceData.stHistory.cPocketNum = CPoint(nPosX, nPosY);
						stSourceData.stHistory.cPicker = vCmdPicker[i];
						//
						SetHandDvcData( vCmdPicker[i], nHandType, &stSourceData);
						SetCmdStageDvcData(nCmdStage, CPoint(nPosX, nPosY), nHandType, &stEmptyDevice);
					}
					// ============================================================================================================
					// case 2 :: Source device exist, hand device not sensing [ pick fail ]
					// ============================================================================================================
					else if( stSourceData.sExist == eDeviceData_Exist && nChkSen != ERR_NO_ERROR ){
						int nDeviceType = DeviceType(nCmdStage, stSourceData, stTargetData);
						if (nDeviceType == eDeviceType::_eDeviceType_Cleaning)
							nRetDeviceType = eDeviceType::_eDeviceType_Cleaning;

						if( bMisMatchErase == TRUE ){
							SetHandDvcData( vCmdPicker[i] , nHandType, &stEmptyDevice);
							SetCmdStageDvcData(nCmdStage, CPoint(nPosX, nPosY), nHandType, &stEmptyDevice);


						}else{
							SetFailPicker(vCmdPicker[i], nHandType);

							nRetError |= nDeviceType == eDeviceType::_eDeviceType_Normal ? ERR_TEST_PP_1_Z1_DEVICE_PICK_FAIL + GetHandTypeIndex(nHandType) : ERR_TEST_PP_1_Z1_CLEAN_DEVICE_PICK_FAIL + GetHandTypeIndex(nHandType);
						}
					}
					// ============================================================================================================
					// case 3 :: Source device no-exist, hand device exist, hand device not sensing [ device lost ]
					// ============================================================================================================
					else if( stSourceData.sExist != eDeviceData_Exist && stTargetData.sExist == eDeviceData_Exist && nChkSen != ERR_NO_ERROR ){
						if( bMisMatchErase == TRUE ){
							int nDeviceType = DeviceType(nCmdStage, stSourceData, stTargetData);
							if (nDeviceType == eDeviceType::_eDeviceType_Cleaning)
								nRetDeviceType = eDeviceType::_eDeviceType_Cleaning;

							SetHandDvcData(vCmdPicker[i], nHandType, &stEmptyDevice);
							SetCmdStageDvcData(nCmdStage, CPoint(nPosX, nPosY), nHandType, &stEmptyDevice);
	
						} else {							
							nRetError |= ERR_TEST_PP_1_Z1_DEVICE_MISS + GetHandTypeIndex(nHandType);
							SetFailPicker(vCmdPicker[i], nHandType);
							CString strPkr;
							strPkr.Format("[%s][Barcode=%s]", (LPCTSTR)(g_TaskTestPP[m_nTestPPIdx].GetPickerName(CPoint(vCmdPicker[i].x, vCmdPicker[i].y))), stTargetData.sz2DBarcode);
							vErrstring.push_back(strPkr);

						}
					}
					// ============================================================================================================
					// case 4 :: Source device no-exist, hand device exist, hand device sensing
					// ============================================================================================================
					else if( stSourceData.sExist != eDeviceData_Exist && stTargetData.sExist == eDeviceData_Exist && nChkSen == ERR_NO_ERROR ){
						continue;
					}
					// ============================================================================================================
					// case 5 :: ??
					// ============================================================================================================
					else{
						ASSERT(0);
						SetFailPicker(vCmdPicker[i], nHandType);
						nRetError |= ERR_TEST_PP_1_Z1_DEVICE_MISS + GetHandTypeIndex(nHandType);
					}
				}

// 				//Loading Table 미리 대기를 위한 Contact PrePare 상태 변경.
// 				//기능에 따라 상태 변경 시점이 다름.
// 				BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
// 				if (bAutoRetestUseMode != TRUE) {
// 					if (LOC_IDX_TEST_PP_TBL_1_LD <= nCmdStage && nCmdStage <= LOC_IDX_TEST_PP_TBL_4_LD) {
// 						int nStrIdx = 0;
// 						switch (nCmdStage)
// 						{
// 							/*case LOC_IDX_STATION_1:*/case LOC_IDX_TEST_PP_TBL_1_LD: /*case LOC_IDX_TEST_PP_TBL_1_UD:*/ nStrIdx = 0; break;
// 							/*case LOC_IDX_STATION_2:*/case LOC_IDX_TEST_PP_TBL_2_LD: /*case LOC_IDX_TEST_PP_TBL_2_UD:*/ nStrIdx = 1; break;
// 							/*case LOC_IDX_STATION_3:*/case LOC_IDX_TEST_PP_TBL_3_LD: /*case LOC_IDX_TEST_PP_TBL_3_UD:*/ nStrIdx = 2; break;
// 							/*case LOC_IDX_STATION_4:*/case LOC_IDX_TEST_PP_TBL_4_LD: /*case LOC_IDX_TEST_PP_TBL_4_UD:*/ nStrIdx = 3; break;
// 							default:
// 								break;
// 						}
// 
// 						int nIdx = nStrIdx + (4 * m_nTestPPIdx);
// 						g_DMCont.m_dmContac_Up.SN(NDM2_PRESS_1_PRE_CONTACT_STATUS + nIdx, TRUE);
// 					}
// 				}
// 				else {
// 
//     				}



			}else{
				for(int i=0; i < (int)vCmdPicker.size(); i++)
				{
					int nStartPickerX = vCmdPicker[i].x -minX.first->x;
					int nStartPickerY = vCmdPicker[i].y -minY.first->y;

					if( nX_PitchType == 2 && vCmdPicker[i].x != minX.first->x ){
						nStartPickerX *=nX_PitchType;
					}
					if( nY_PitchType == 2 && vCmdPicker[i].y != minY.first->y ){
						nStartPickerY *=nY_PitchType;
					}

					int nPosX = nTargetX + nStartPickerX;
					int nPosY = nTargetY + nStartPickerY;

					// Get Source data ( Source = Hand )
					stSourceData.clear();
					GetHandDvcData( vCmdPicker[i], nHandType, &stSourceData);

					// Get Target data ( Target = Table or Buff )
					stTargetData.clear();
					GetCmdStageDvcData(nCmdStage, CPoint(nPosX, nPosY), nHandType, &stTargetData);

					// checking hand picker vacuum sensor
					int nChkSen = ChkVacuum(DEF_OFF, nHandType, vCmdPicker[i]);

					// ============================================================================================================
					// case 1 :: hand device data exist, Target device data empty, hand not sensing device
					// ============================================================================================================
					if( stSourceData.sExist == eDeviceData_Exist && stTargetData.sExist != eDeviceData_Exist && nChkSen == ERR_NO_ERROR && bMisMatchErase != TRUE){

						if (nCmdStage >= LOC_IDX_STATION_1 && nCmdStage <= LOC_IDX_STATION_4)
						{
							if( strcmp(stSourceData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0 )
								stSourceData.nTestingInfo = eTestSite_CleanLoading;
							else
								stSourceData.nTestingInfo = eTestSite_DvcLoading;
						}						

						SetCmdStageDvcData(nCmdStage, CPoint(nPosX, nPosY), nHandType, &stSourceData);
						SetHandDvcData( vCmdPicker[i], nHandType, &stEmptyDevice);
					}
					// ============================================================================================================
					// case 2 :: hand device data exist, but hand don't have device [device lost] 
					//           [Alarm Device Skip시 Device Data 삭제]
					// ============================================================================================================
					else if( stSourceData.sExist == eDeviceData_Exist && nChkSen == ERR_NO_ERROR && stTargetData.sExist != eDeviceData_Exist && bMisMatchErase == TRUE){
						nRetError |= ERR_TEST_PP_1_Z1_DEVICE_MISS + GetHandTypeIndex(nHandType);
						
						int nDeviceType = DeviceType(nCmdStage, stSourceData, stTargetData);
						if (nDeviceType == eDeviceType::_eDeviceType_Cleaning)
							nRetDeviceType = eDeviceType::_eDeviceType_Cleaning;

						SetHandDvcData(vCmdPicker[i], nHandType, &stEmptyDevice);

						CString strPkr;
						strPkr.Format("[%s][Barcode=%s]", (LPCTSTR)(g_TaskTestPP[m_nTestPPIdx].GetPickerName(CPoint(vCmdPicker[i].x, vCmdPicker[i].y))), stSourceData.sz2DBarcode);
						vErrstring.push_back(strPkr);
						
						//SetFailPicker(vCmdPicker[i]);
					}
					// ============================================================================================================
					// case 3 :: hand device data no-exist, Target pocket device data exist, hand sensing device
					// ============================================================================================================
					else if( stSourceData.sExist != eDeviceData_Exist && stTargetData.sExist == eDeviceData_Exist && nChkSen != ERR_NO_ERROR ){
						int nDeviceType = DeviceType(nCmdStage, stSourceData, stTargetData);

						nRetError |= nDeviceType == eDeviceType::_eDeviceType_Normal ? ERR_TEST_PP_1_Z1_DEVICE_PLACE_FAIL + GetHandTypeIndex(nHandType) : ERR_TEST_PP_1_Z1_CLEAN_DEVICE_PLACE_FAIL + GetHandTypeIndex(nHandType);

/*						nRetError |= ERR_TEST_PP_1_Z1_DEVICE_PLACE_FAIL + GetHandTypeIndex(nHandType);*/
						SetFailPicker(vCmdPicker[i], nHandType);
					}
					// ============================================================================================================
					// case 4 :: hand device data no-exist, Target pocket device data exist, hand don't have device 
					//           [ normal complete ]
					// ============================================================================================================
					else if( stSourceData.sExist != eDeviceData_Exist && stTargetData.sExist == eDeviceData_Exist && nChkSen == ERR_NO_ERROR){
						continue;
					}

					// ============================================================================================================
					// case 5 :: hand device data exist, Target pocket device data no-exist, hand have device 
					//           [ Alarm Device Skip시 정상적인 경우]
					//           [ Alarm Skip이 아닐 경우 Place Fail, Place를 했는데 Device가 hand에 감지된다.]
					// ============================================================================================================
					else if (stSourceData.sExist == eDeviceData_Exist && nChkSen != ERR_NO_ERROR && stTargetData.sExist != eDeviceData_Exist) {
						if (bMisMatchErase == TRUE) {
							continue;
						}
						else {
							nRetError |= ERR_TEST_PP_1_Z1_DEVICE_PLACE_FAIL + GetHandTypeIndex(nHandType);
						}
					}
					// ============================================================================================================
					// case 6 :: ??
					// ============================================================================================================
					else{
						ASSERT(0);
						SetFailPicker(vCmdPicker[i], nHandType);
						nRetError |= ERR_TEST_PP_1_Z1_DEVICE_MISS + GetHandTypeIndex(nHandType);
					}
				}

				//Loading Table 미리 대기를 위한 Contact PrePare 상태 변경.
				//Unload Table에 Place
				//기능에 따라 상태 변경 시점이 다름.
				BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
				if (bAutoRetestUseMode != TRUE) {
					if (LOC_IDX_TEST_PP_TBL_1_UD <= nCmdStage && nCmdStage <= LOC_IDX_TEST_PP_TBL_4_UD && nHandType == eTestPPLoadType) {
						int nStrIdx = 0;
						switch (nCmdStage)
						{
							/*case LOC_IDX_STATION_1:*//*case LOC_IDX_TEST_PP_TBL_1_LD:*/ case LOC_IDX_TEST_PP_TBL_1_UD: nStrIdx = 0; break;
								/*case LOC_IDX_STATION_2:*//*case LOC_IDX_TEST_PP_TBL_2_LD:*/ case LOC_IDX_TEST_PP_TBL_2_UD: nStrIdx = 1; break;
									/*case LOC_IDX_STATION_3:*//*case LOC_IDX_TEST_PP_TBL_3_LD:*/ case LOC_IDX_TEST_PP_TBL_3_UD: nStrIdx = 2; break;
										/*case LOC_IDX_STATION_4:*//*case LOC_IDX_TEST_PP_TBL_4_LD:*/ case LOC_IDX_TEST_PP_TBL_4_UD: nStrIdx = 3; break;
										default:
											break;
						}

						int nIdx = nStrIdx + (4 * m_nTestPPIdx);
						g_DMCont.m_dmContac_Up.SN(NDM2_PRESS_1_PRE_CONTACT_STATUS + nIdx, FALSE);
					}
				}
				else {

  				}
			}
		}break;
	}

	return nRetError;
}

void CTaskTestPP::MakeLog(LPCTSTR fmt, ...)
{
    TCHAR tmpMsg[1024] = {0,};
    if(fmt)
    {
        va_list argList;
        va_start(argList, fmt);
        _vstprintf_s(tmpMsg, fmt, argList);
        va_end(argList);
    }
    TWLOG.MakeLog(static_cast<int>(eLogID_TestPP_1+m_nTestPPIdx), Debug, "", __LINE__, NULL, tmpMsg);
}


int CTaskTestPP::ChkLostDvcCnt( std::vector<CPoint> vCmdPicker,int nHandtype, int nPickPlace )
{
	int nLostDvcCnt=0;
	for(int i=0; i < (int)vCmdPicker.size(); i++){
		int nExsitDvc = GetHandPkrDvcExist(vCmdPicker[i], nHandtype);
		int nRetErr = ChkVacuum(DEF_ON, nHandtype, vCmdPicker[i]);
		
		// device data exist but not sensing device
		if( nPickPlace == DEF_PICK ){
			if(nExsitDvc == eDvcExist && nRetErr != ERR_NO_ERROR )
				nLostDvcCnt++;
		}
		if( nPickPlace == DEF_PLACE )
		{
			if(nExsitDvc != eDvcExist && nRetErr == ERR_NO_ERROR )
				nLostDvcCnt++;
		}
	}
	return nLostDvcCnt;
}

CString CTaskTestPP::GetLocName(int nCmdStage) //[작업 필요]
{
	ASSERT( nCmdStage >= LOC_IDX_STATION_1 && nCmdStage <= LOC_IDX_TEST_PP_HOME );

	CString strLoc = _T("");
	switch (nCmdStage)
	{
	case LOC_IDX_STATION_1:
	case LOC_IDX_STATION_2:
	case LOC_IDX_STATION_3:
	case LOC_IDX_STATION_4:
	{
		//		int nPressIdx = (nCmdStage - LOC_IDX_STATION_1) + (ePressUnit_Down_1 + 1) + (eMaxPressUnitCount / 2)*m_nTestPPIdx;
		int nPressIdx = (nCmdStage + 1) + (eMaxPressUnitCount / 2)*m_nTestPPIdx;
		strLoc.Format("STATION (#%d)", nPressIdx);
	}break;
	case LOC_IDX_TEST_PP_TBL_1_LD:
	case LOC_IDX_TEST_PP_TBL_2_LD:
	case LOC_IDX_TEST_PP_TBL_3_LD:
	case LOC_IDX_TEST_PP_TBL_4_LD:
	{
		int nTableIdx = 0;
		if(m_nTestPPIdx == eTestPP_1)
			nTableIdx = eLoadTbl_2;
		else
			nTableIdx = eLoadTbl_3;

		strLoc.Format("TABLE (#%d) [Loc =%d]", nTableIdx + 1, (nCmdStage - LOC_IDX_TEST_PP_TBL_1_LD) + 1);
	}break;
	case LOC_IDX_TEST_PP_TBL_1_UD:
	case LOC_IDX_TEST_PP_TBL_2_UD:
	case LOC_IDX_TEST_PP_TBL_3_UD:
	case LOC_IDX_TEST_PP_TBL_4_UD:
	{
		int nTableIdx = 0;
		if (m_nTestPPIdx == eTestPP_1)
			nTableIdx = eLoadTbl_1;
		else
			nTableIdx = eLoadTbl_4;

		strLoc.Format("TABLE (#%d) [LOC =%d]", nTableIdx + 1, (nCmdStage - LOC_IDX_TEST_PP_TBL_1_UD) + 1);
	}break;
// 	case LOC_IDX_ART_BUFFER_1:
// 	case LOC_IDX_ART_BUFFER_2:
// 	case LOC_IDX_ART_BUFFER_3:
// 	case LOC_IDX_ART_BUFFER_4:
// 	{
// 		int nBufferTblIdx = nCmdStage + 1;
// 		strLoc.Format("ART BUFFER (#%d)", nBufferTblIdx);
//	}break;
	case LOC_IDX_CLEAN_TBL:
	{
		strLoc.Format("CLEAN BUFFER");
	}break;
	case LOC_IDX_TEST_PP_HOME:
	{
		strLoc.Format("HOME");
	}break;
	default: { strLoc.Format("NONE"); }break;
	}
	return strLoc;
}

CString CTaskTestPP::GetPickerName(CPoint Picker)
{
	CString strPicker = _T("");
	if( Picker.y == 0 ){
		strPicker += _T("[R");
	}else{
		strPicker += _T("[F");
	}

	CString strPickerX = _T("");
	strPickerX.Format("L%d]", Picker.x+1 );
	strPicker += strPickerX;

	return strPicker;
}

CString CTaskTestPP::GetPickerName( std::vector<CPoint> vPicker )
{
	CString strPicker =_T("");

	for( std::vector<CPoint>::size_type i=0; i<vPicker.size(); i++){
		strPicker += GetPickerName(vPicker[i]);
	}

	return strPicker;
}

CString CTaskTestPP::GetHandTypeName(int nHandType)
{
	CString strHandTypeName = _T("");

	switch (nHandType)
	{
	case eTestPPLoadType: {strHandTypeName = "LoadHand"; }; break;
	case eTestPPUnLoadType: {strHandTypeName = "UNLoadHand"; }; break;
	default:
		break;
	}

	return strHandTypeName;
}


int CTaskTestPP::GetPressNum(int nCmdStage) //[작업 필요]
{
	ASSERT( nCmdStage >= LOC_IDX_STATION_1 && nCmdStage <= LOC_IDX_TEST_PP_HOME );

	int nPressNum = 0;
	switch(nCmdStage)
	{
	case LOC_IDX_STATION_1: 
	case LOC_IDX_STATION_2: 
	case LOC_IDX_STATION_3: 
	case LOC_IDX_STATION_4: 
		{
			nPressNum = ePressUnit_Down_1 + (nCmdStage- LOC_IDX_STATION_1) + (m_nTestPPIdx*(eMaxPressUnitCount/2));
		}break;
	}

	return nPressNum;
}

int CTaskTestPP::GetHandTypeIndex(int nHandType)
{
	int nIdx = -1;
	switch (m_nTestPPIdx)
	{
		case eTestPP_1:
		{
			if (nHandType == eTestPPLoadType)
				nIdx = eTestPP_1_LoadType;
			else
				nIdx = eTestPP_2_LoadType;
		}break;
		case eTestPP_2:
		{
			if (nHandType == eTestPPLoadType)
				nIdx = eTestPP_1_UnloadType;
			else
				nIdx = eTestPP_2_UnloadType;
		}break;
		default:
			break;
	}
	
	return nIdx;
}


void CTaskTestPP::SetFailPicker(CPoint Picker,int nHandType)
{
	m_vFailPicker[nHandType].push_back( CPoint(Picker.x,Picker.y) );
}

std::vector<CPoint> CTaskTestPP::GetFailPicker(int nHandType)
{	
	return m_vFailPicker[nHandType];
}

int CTaskTestPP::GetLoadHandDeviceCnt()
{
	int cnt = 0;
	for(int nY=0; nY<MAX_TEST_PP_PKR_CNT_Y; nY++){
		for(int nX=0; nX<MAX_TEST_PP_PKR_CNT_X; nX++){
			cnt += g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TEST_PP_1+m_nTestPPIdx, nX, nY);
		}
	}

	return cnt;
}

int CTaskTestPP::GetUnLoadHandRetestBDeviceCnt()
{
	ST_DD_DEVICE stSiteDvc;
	int nCnt = 0;
	for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++) {
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++) {
			stSiteDvc.clear();
			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_nTestPPIdx, nX, nY + TEST_UD_PKR_Y, &stSiteDvc);
			if (stSiteDvc.sExist == eDeviceData_Exist && stSiteDvc.nTestingCnt == eARB_2stTestEnd)
			{
				nCnt++;
			}
		}
	}

	return nCnt;
}

int CTaskTestPP::GetUnLoadHandDeviceCnt()
{
	int cnt = 0;
	for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++) {
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++) {
			cnt += g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TEST_PP_1 + m_nTestPPIdx, nX, nY + TEST_UD_PKR_Y);
		}
	}

	return cnt;
}

int CTaskTestPP::GetBufferTableCnt()
{
	int nLoadTblPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	int nLoadTblPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

	// Load Table 1  Load Table 2	 // Load Table 3  Load Table 4
	// 4 * 4		// 4 * 4		// 4 * 4		// 4 * 4			
	// 좌상단이 0,0	 // 좌상단이 0,0  // 좌상단이 0,0	 // 좌상단이 0,0
	//     0 1 2 3	//     0 1 2 3	 //     0 1 2 3	//     0 1 2 3
	//    ㅡㅡㅡㅡㅡ	//    ㅡㅡㅡㅡㅡ	 //    ㅡㅡㅡㅡㅡ	//    ㅡㅡㅡㅡㅡ
	// 0 | U U U U	// 0 | L L L L	 // 0 | U U U U	// 0 | L L L L
	// 1 | B B B B	// 1 | B B B B	 // 1 | B B B B	// 1 | B B B B
	// 2 | U U U U	// 2 | L L L L	 // 2 | U U U U	// 2 | L L L L
	// 3 | B B B B	// 3 | B B B B	 // 3 | B B B B	// 3 | B B B B

	int nBufferloadDvcCnt = 0;
	for (int nY = 0; nY < nLoadTblPocketCntY; nY++) {
		for (int nX = 0; nX < nLoadTblPocketCntX; nX++) {
			for (int nTableIdx : m_vModuleSetLoadTableIdx)
			{
				char setting = g_TaskLoadTbl[nTableIdx].m_LoadTbl_pocket_setting[nY][nX];
				bool device_exist = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_LD_TBL_1+ nTableIdx, nX, nY) == DEF_EXIST;
				if (setting == 'B' && device_exist == true)
					nBufferloadDvcCnt++;
			}
		}
	}

	return nBufferloadDvcCnt;
}

int CTaskTestPP::GetBufferTablePocketCnt()
{
	int nLoadTblPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	int nLoadTblPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

	// Load Table 1  Load Table 2	 // Load Table 3  Load Table 4
	// 4 * 4		// 4 * 4		// 4 * 4		// 4 * 4			
	// 좌상단이 0,0	 // 좌상단이 0,0  // 좌상단이 0,0	 // 좌상단이 0,0
	//     0 1 2 3	//     0 1 2 3	 //     0 1 2 3	//     0 1 2 3
	//    ㅡㅡㅡㅡㅡ	//    ㅡㅡㅡㅡㅡ	 //    ㅡㅡㅡㅡㅡ	//    ㅡㅡㅡㅡㅡ
	// 0 | U U U U	// 0 | L L L L	 // 0 | U U U U	// 0 | L L L L
	// 1 | B B B B	// 1 | B B B B	 // 1 | B B B B	// 1 | B B B B
	// 2 | U U U U	// 2 | L L L L	 // 2 | U U U U	// 2 | L L L L
	// 3 | B B B B	// 3 | B B B B	 // 3 | B B B B	// 3 | B B B B

	int nBufferPocketCnt = 0;
	for (int nY = 0; nY < nLoadTblPocketCntY; nY++) {
		for (int nX = 0; nX < nLoadTblPocketCntX; nX++) {
			for (int nTableIdx : m_vModuleSetLoadTableIdx)
			{
				char setting = g_TaskLoadTbl[nTableIdx].m_LoadTbl_pocket_setting[nY][nX];
				if (setting == 'B')
					nBufferPocketCnt++;
			}
		}
	}

	return nBufferPocketCnt;
}

int CTaskTestPP::GetHandPitchType( int nAxis, int nLocIdx )
{
	ASSERT( nAxis == CTaskTestPP::eAxisPitch_HW_X1_SW_Y1 || nAxis == CTaskTestPP::eAxisPitch_HW_Y1_SW_X1 ||
			nAxis == CTaskTestPP::eAxisPitch_HW_X2_SW_Y2 || nAxis == CTaskTestPP::eAxisPitch_HW_Y2_SW_X2); //[작업 필요]
	
	int nPocketPitchX = 0, nPocketPitchY = 0;
	int nPitchType_X = 0, nPitchType_Y = 0;
	int nHanPitchX = DEF_TEST_PP_MAX_X_PITCH_WIDE, nHanPitchY = DEF_TEST_PP_MAX_Y_PITCH_WIDE;
	
	switch (nLocIdx)
	{
		case LOC_IDX_CLEAN_TBL:
		{
			nPocketPitchX = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Pitch_X);
			nPocketPitchY = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Pitch_Y);

			nHanPitchX = DEF_TEST_PP_CLEAN_TABLE_PITCH_X;//g_DMCont.m_dmTestPP.GN(NDM4_TestPP_Pitch_X);
			nHanPitchY = DEF_TEST_PP_CLEAN_TABLE_PITCH_Y;// g_DMCont.m_dmTestPP.GN(NDM4_TestPP_Pitch_Y);
		}break;
		// 	case LOC_IDX_ART_BUFFER_1: 
		// 	case LOC_IDX_ART_BUFFER_2: 
		// 	case LOC_IDX_ART_BUFFER_3: 
		// 	case LOC_IDX_ART_BUFFER_4: 
		// 		{
		// 			nPocketPitchX = g_DMCont.m_dmTestPP.GN(NDM4_TestBuffTbl_Pitch_X);
		// 			nPocketPitchY = g_DMCont.m_dmTestPP.GN(NDM4_TestBuffTbl_Pitch_Y);
		// 		}break;
		case LOC_IDX_TEST_PP_TBL_1_LD:
		case LOC_IDX_TEST_PP_TBL_2_LD:
		case LOC_IDX_TEST_PP_TBL_3_LD:
		case LOC_IDX_TEST_PP_TBL_4_LD:
		case LOC_IDX_TEST_PP_TBL_1_UD:
		case LOC_IDX_TEST_PP_TBL_2_UD:
		case LOC_IDX_TEST_PP_TBL_3_UD:
		case LOC_IDX_TEST_PP_TBL_4_UD:
		{
			nPocketPitchX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_X);
			nPocketPitchY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_Y);

			nHanPitchX = g_DMCont.m_dmTestPP.GN(NDM4_TestPP_Pitch_X);
			nHanPitchY = g_DMCont.m_dmTestPP.GN(NDM4_TestPP_Pitch_Y);

		}break;
		default:	// station
		{
			nPocketPitchX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Pitch_X);
			nPocketPitchY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Pitch_Y);

			nHanPitchX = g_DMCont.m_dmTestPP.GN(NDM4_TestPP_ST_Pitch_X);
			nHanPitchY = g_DMCont.m_dmTestPP.GN(NDM4_TestPP_ST_Pitch_Y);
		}break;
	}

		// == calc pitch x type
	if (nPocketPitchX >= DEF_TEST_PP_MAX_X_PITCH_NARROW && nPocketPitchX <= DEF_TEST_PP_MAX_X_PITCH_WIDE)
	{
		double dContainerPitch_X_Cnt = (double)nHanPitchX / (double)nPocketPitchX;
		double Remainder_X = dContainerPitch_X_Cnt - (int)dContainerPitch_X_Cnt;	// one by one 인지 확인 한다.

		nPitchType_X = (int)dContainerPitch_X_Cnt;
		if (Remainder_X != 0) {				// 나머지가 있어서 one by one 진행 하여야한다. 한번에 내려놓을수 없는 조건
			nPitchType_X = 0;
		}
	}
	else if (nPocketPitchX > DEF_TEST_PP_MAX_X_PITCH_WIDE)
	{
		double dContainerPitch_X_Cnt = (double)nPocketPitchX / (double)nHanPitchX;
		double Remainder_X = dContainerPitch_X_Cnt - (int)dContainerPitch_X_Cnt;	// one by one 인지 확인 한다.

		nPitchType_X = (int)dContainerPitch_X_Cnt * -1;
		if (Remainder_X != 0) {				// 나머지가 있어서 one by one 진행 하여야한다. 한번에 내려놓을수 없는 조건
			nPitchType_X = 0;
		}
	}
	// == calc pitch y type
	if (nPocketPitchY >= DEF_TEST_PP_MAX_Y_PITCH_NARROW && nPocketPitchY <= DEF_TEST_PP_MAX_Y_PITCH_WIDE)
	{
		double dContainerPitch_Y_Cnt = (double)nHanPitchY / (double)nPocketPitchY;
		double Remainder_Y = dContainerPitch_Y_Cnt - (int)dContainerPitch_Y_Cnt;	// one by one 인지 확인 한다.

		nPitchType_Y = (int)dContainerPitch_Y_Cnt;
		if (Remainder_Y != 0) {				// 나머지가 있어서 one by one 진행 하여야한다. 한번에 내려놓을수 없는 조건
			nPitchType_Y = 0;
		}
	}
	else if (nPocketPitchY > DEF_TEST_PP_MAX_Y_PITCH_WIDE)
	{
		//double dContainerPitch_Y_Cnt = (double)nPocketPitchY / (double)nHanPitchY;
		//double Remainder_Y = dContainerPitch_Y_Cnt - (int)dContainerPitch_Y_Cnt;	// one by one 인지 확인 한다.

		//nPitchType_Y = (int)dContainerPitch_Y_Cnt * -1;
		//if (Remainder_Y != 0) {				// 나머지가 있어서 one by one 진행 하여야한다. 한번에 내려놓을수 없는 조건
		//	nPitchType_Y = 0;
		//}
		nPitchType_Y = 0;
	}
	if (nAxis == CTaskTestPP::eAxisPitch_HW_X1_SW_Y1)
		return nPitchType_X;
	if (nAxis == CTaskTestPP::eAxisPitch_HW_Y1_SW_X1)
		return nPitchType_Y;

	return NO_ERROR;
}

int CTaskTestPP::ChkBuffArrivedSen_All(CString &strErrMsg, std::vector<CPoint> &vErrPocket)
{
	if( g_DMCont.m_dmHandlerOpt.GB( BDM11_MD_DevExist_Sen_Check ) == TRUE )
		return ERR_NO_ERROR;

	BOOL bErrArrivedSen = FALSE;

	CString strTmp = _T("");
	strErrMsg.Empty();

	int nPocketCount_X = g_DMCont.m_dmTestPP.GN(NDM4_TestBuffTbl_Div_X);

	for (int nY = 0; nY <= xART_BUFF_ARRIVED_SEN2-xART_BUFF_ARRIVED_SEN1; nY++)
	{
		int nErr = ChkDI(CTaskTestPP::xART_BUFF_ARRIVED_SEN1 + nY, DEF_ON);
		if (nErr != ERR_NO_ERROR) {
			bErrArrivedSen = TRUE;
			strTmp.Format("%d,", nY + 1);
			strErrMsg += strTmp;
			for (int nX = 0; nX < nPocketCount_X; nX++) {
				vErrPocket.push_back(CPoint(nX, nY));
			}
		}
	}

	if (bErrArrivedSen == TRUE)
		return (ERR_TEST_PP_1_ARRIVED_SEN_FAIL + m_nTestPPIdx);

	return ERR_NO_ERROR;
}

int CTaskTestPP::ChkBuffArrivedSen(int nY, CString &strErrMsg, std::vector<CPoint> &vErrPocket, std::vector<CString> &vErrHistory)
{
	if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_DevExist_Sen_Check) == TRUE)
		return ERR_NO_ERROR;

	BOOL bErrArrivedSen = FALSE;

	CString strTmp = _T("");
	strErrMsg.Empty();

	int nPocketCount_X = g_DMCont.m_dmTestPP.GN(NDM4_TestBuffTbl_Div_X);

	int nErr = ChkDI(CTaskTestPP::xART_BUFF_ARRIVED_SEN1 + nY, DEF_ON);
	if (nErr != ERR_NO_ERROR) {
		bErrArrivedSen = TRUE;
		strTmp.Format("%d,", nY + 1);
		strErrMsg += strTmp;
		for (int nX = 0; nX < nPocketCount_X; nX++) {
			vErrPocket.push_back(CPoint(nX, nY));
			
			ST_DD_DEVICE stDvc;
			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1_ART_BUFF + m_nTestPPIdx, nX, nY, &stDvc);
			
			if (stDvc.sExist == DEF_EXIST) {
				strTmp.Format(_T("[Pocket X=%d Y=%d] Device Pick History Stage: %s, Pocket: X=%d Y=%d, Picker: X=%d Y=%d"),
					nX + 1, nY + 1, g_TaskSystemCtrl.GetHistoryStageName(stDvc.stHistory.nStage), stDvc.stHistory.cPocketNum.x + 1, stDvc.stHistory.cPocketNum.y + 1, stDvc.stHistory.cPicker.x + 1, stDvc.stHistory.cPicker.y + 1);

				vErrHistory.push_back(strTmp);
			}
		}
	}

// 	if (bErrArrivedSen == TRUE)
// 		return (ERR_TEST_PP_1_X1_ARRIVED_SEN_FAIL + m_nTestPPIdx);

	return ERR_NO_ERROR;
}

void CTaskTestPP::ApplyBacklash(int nHandType)
{
	int nAxisNumX = m_nTestPPIdx == eTestPP_1 ? eAXIS_TEST_PP_1_X: eAXIS_TEST_PP_2_X;
	int nAxisNumY = m_nTestPPIdx == eTestPP_1 ? eAXIS_TEST_PP_1_Y : eAXIS_TEST_PP_2_Y;
	for (int nSpeed = eSpeed_5; nSpeed < eSpeed_MAX; nSpeed++)
	{
		g_Motor[nAxisNumX + nHandType]->SetSpeedBacklash(nSpeed, g_VatBacklash.testpp[m_nTestPPIdx][nHandType].m_BacklashOffset_XY[nSpeed].x);
		g_Motor[nAxisNumY]->SetSpeedBacklash(nSpeed, g_VatBacklash.testpp[m_nTestPPIdx][eTestPPLoadType].m_BacklashOffset_XY[nSpeed].y);
	}
}
BOOL CTaskTestPP::ChkPkrArt1StBin(std::vector<CPoint> vPicker)
{
	BOOL bAutoRetetsMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
	if (bAutoRetetsMode == eOPTION_NOT_USE)
		return FALSE;

	ST_DD_DEVICE stPkr;
	int n1stEndDvcCnt = 0;
	for (int i = 0; i < (int)vPicker.size(); i++)
	{
		g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_nTestPPIdx, vPicker[i].x, vPicker[i].y, &stPkr);

		BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stPkr);

		if ( stPkr.sExist == eDeviceData_Exist && stPkr.nTestingCnt == eARB_1stTestEnd && stPkr.nBinColor == eFailBin && strcmp(stPkr.szErrString, DEF_TEST_RESULT_TP101) != 0 && bIsDefaultBin != TRUE )
		{
			int nSiteOnOff = g_TaskPressUnit[atoi(stPkr.szTestSiteNum)].GetSocketOnOff(stPkr.cSubSiteNum.x, stPkr.cSubSiteNum.y);
			if( nSiteOnOff == DEF_ON )
				n1stEndDvcCnt++;
		}	
	}

	if (n1stEndDvcCnt == 0)
		return  FALSE;
	
	return TRUE;
}



CString CTaskTestPP::GetErrPickerName(int nUpDown, int nHandtype, std::vector<CPoint> vPicker)
{
// 	CString strErrPkr = _T("");
// 
// 	for (int i = 0; i < (int)vPicker.size(); i++)
// 	{
// 		if (GetPicker(nUpDown, nHandtype, vPicker[i]) != ERR_NO_ERROR)
// 		{
// 			strErrPkr += GetPickerName(vPicker[i]);
// 		}
// 	}
// 
// 	return strErrPkr;
	return NO_ERROR;
}

void CTaskTestPP::GetUserOffsetData()
{
	char szCokName[128] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
	g_CokMgr.LoadUserOffSetDataTestPp_MP(szCokName, m_nTestPPIdx, m_stTeachPp);
}

void CTaskTestPP::GetTeachingPos() 
{
	char szDrvFile[512] = { 0, };
	char szCokName[512] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));

	char szCurDir[512] = { 0, };
	GetCurrentDirectory(sizeof(szCurDir), szCurDir);
	sprintf_s(szDrvFile, sizeof(szDrvFile), "%s\\sys_data\\%s", szCurDir, SZ_DRIVER_FILE_NAME);

	g_DMCont.m_Teach.Load();
	m_CTeaching_Data.Initialize(szDrvFile, szCokName);
	m_CTeaching_Data = g_DMCont.m_Teach;

}

int CTaskTestPP::GetLoadPickerLoadDeviceCnt()
{
	std::vector<CPoint> vPicker;
	int nCnt = 0;
	ST_DD_DEVICE stDvc;

	for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++) {
		for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++) {
			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_nTestPPIdx, nX, nY, &stDvc);
			if (stDvc.sExist == DEF_EXIST && strcmp(stDvc.szBinResult, DEF_SOCKET_CLEAN_DEVICE) != 0) {
				nCnt++;
			}
		}
	}
	return nCnt;
}

BOOL CTaskTestPP::GetTestPPCleanDevice()
{
	std::vector<CPoint> vPicker;
	BOOL bGetCleanDev = FALSE;
	ST_DD_DEVICE stDvc;

	for (int nHandType = 0; nHandType < eMaxTestPPTypeCount; nHandType++)
	{
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++) {
			for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++) {
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_nTestPPIdx, nX, nY + (nHandType == eTestPPLoadType ? TEST_LD_PKR_Y : TEST_UD_PKR_Y), &stDvc);
				if (stDvc.sExist == DEF_EXIST && strcmp(stDvc.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0) {
					bGetCleanDev = TRUE;
					break;
				}
			}
		}
	}
	
	return bGetCleanDev;
}

int CTaskTestPP::ChkBeforeBlowDevice(std::vector<CPoint> vCmdPicker, int nHandType, CString& strErrorMsg, std::vector<CString>& vErrString)
{
	int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
	if (nDeviceMode != eDeviceMode || g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
	{
		return ERR_NO_ERROR;
	}

	m_vLostBlowDvc[nHandType].clear();
	strErrorMsg.Empty();
	ST_DD_DEVICE Dvc;
	CString strPkr = _T("");
	CString strTmp;
	for (int i = 0; i < (int)vCmdPicker.size(); i++)
	{
		int nDeviceExistData = g_TaskTestPP[m_nTestPPIdx].GetHandPkrDvcExist(vCmdPicker[i], nHandType);
		int nExist = g_TaskTestPP[m_nTestPPIdx].ChkVacuum(DEF_ON, nHandType, vCmdPicker[i]);
		if (nExist != ERR_NO_ERROR && nDeviceExistData == eDvcExist) {
			Dvc.clear();
			int nUnloadPkrY = nHandType == eTestPPUnLoadType ? TEST_UD_PKR_Y : TEST_LD_PKR_Y;
			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_PP_1 + m_nTestPPIdx, vCmdPicker[i].x, vCmdPicker[i].y + nUnloadPkrY, &Dvc);

			strTmp.Format("[%s]", (LPCTSTR)(g_TaskTestPP[m_nTestPPIdx].GetPickerName(vCmdPicker[i])));
			strErrorMsg += strTmp;
			m_vLostBlowDvc[nHandType].push_back(vCmdPicker[i]);
			strPkr.Format("[%s][Barcode=%s]", (LPCTSTR)(g_TaskTestPP[m_nTestPPIdx].GetPickerName(CPoint(vCmdPicker[i].x, vCmdPicker[i].y))), Dvc.sz2DBarcode);
			vErrString.push_back(strPkr);
		}
	}

	if (strErrorMsg.IsEmpty() == false)
	{
		strErrorMsg += "Picker Device Data is Exist, Device Vacuum Sensor is Not Exist";
		return ERR_TEST_PP_1_Z1_DEVICE_MISS + g_TaskTestPP[m_nTestPPIdx].GetHandTypeIndex(nHandType);
	}

	return ERR_NO_ERROR;
}

std::vector<CPoint> CTaskTestPP::GetLostPicker(int nHandType)
{
	return m_vLostBlowDvc[nHandType];
}

//[작업 필요]
int CTaskTestPP::Interlock_Move_X(double dTargetX , int nAxis, int nHandType) // Y위치를 보고 움직임 (Station입구에 들어오는 범위인지 본다)
{
	if(nAxis != eAxisX1 && nAxis != eAxisX2 && nAxis  != eAXIS_TEST_PP_1_X && nAxis != eAXIS_TEST_PP_1_X2 && nAxis != eAXIS_TEST_PP_2_X && nAxis != eAXIS_TEST_PP_2_X2) //X축을 뻗을 때만 해당되는 인터락.
		return ERR_NO_ERROR;

	OneAxis* pMotorX[2] = {m_vMotor[eAxisX1],m_vMotor[eAxisX2] };
	double dStationYLimit = 0.0;
	double dStationXLimit = 0.0;
	//clean Table Y Pos. (Station 가기전의 최상단 Y 티칭 값)

	if(m_nTestPPIdx == eTestPP_1)
		dStationXLimit = m_CTeaching_Data.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), LOC_IDX_LD_TBL_TEST_SIDE_1, eAxisX1); 
	else
		dStationXLimit = m_CTeaching_Data.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), LOC_IDX_LD_TBL_TEST_SIDE_1, eAxisX2);

	
	double dCurPosX = 0;
	pMotorX[nHandType]->GetCurrentPos(dCurPosX);

	if ((dStationXLimit + STATION_X_GAP) > dTargetX)
	{
		// 2. X를 Station 까지 뻗는것이 아니면 상관 없음?
		return ERR_NO_ERROR;
	}
	else
	{
		// 1. X를 Station 까지 뻗으려 한다면
		// 2. Y의 위치가 Station 입구에 들어오는 영역인가?
		for (int nStationNum = LOC_IDX_STATION_1; nStationNum <= LOC_IDX_STATION_4; nStationNum++)
		{
			dStationYLimit = m_CTeaching_Data.GetTeachPos(RID_TEST_PP_(m_nTestPPIdx), nStationNum, eAxisY);

			if ((dStationYLimit - STATION_Y_GAP) < dCurPosX && dCurPosX < (dStationYLimit + STATION_Y_GAP))
			{
				//x 위치가 특정 station 진입 위치 확인 OK
				return ERR_NO_ERROR;
			}
		}
		//for(nStationNum) 에서 걸러지지 안았다면 충돌 상황.
		if (g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
			return ERR_TEST_PP_1_X1_MOVE_POS_NOT_SAFETY + GetHandTypeIndex(nHandType); //에러
		else
			return ERR_NO_ERROR;
	}
	
	return ERR_NO_ERROR;
}



int CTaskTestPP::TransIdxMotorAxisToTeachPos(int nMotorAxis)
{
	int nIdx = 0;

	switch (nMotorAxis)
	{
	case eAxisX1:		nIdx = ePos_AxisX1; break;
	case eAxisX2:		nIdx = ePos_AxisX2; break;
	case eAxisY:		nIdx = ePos_AxisY1; break;
	case eAxisZ1:		nIdx = ePos_AxisZ1; break;
	case eAxisZ2:		nIdx = ePos_AxisZ2; break;
	case eAxisPitch_HW_X1_SW_Y1:	nIdx = ePos_AxisPitchX1; break;
	case eAxisPitch_HW_X2_SW_Y2:	nIdx = ePos_AxisPitchX2; break;
	case eAxisPitch_HW_Y1_SW_X1:	nIdx = ePos_AxisPitchY1; break;
	case eAxisPitch_HW_Y2_SW_X2:	nIdx = ePos_AxisPitchY2; break;
	}

	return nIdx;
}


int CTaskTestPP::CmdStageHistoryStageConvertor(int nCmdStage)
{
	int nHistoryStage = -1;
	switch (nCmdStage)
	{
	case LOC_IDX_STATION_1: { 	nHistoryStage = (m_nTestPPIdx == eTestPP_1) ? HISTORY_LOC_IDX_TEST_PP1_STATION_DN_1 : HISTORY_LOC_IDX_TEST_PP2_STATION_DN_1;}break;
	case LOC_IDX_STATION_2: { 	nHistoryStage = (m_nTestPPIdx == eTestPP_1) ? HISTORY_LOC_IDX_TEST_PP1_STATION_DN_2 : HISTORY_LOC_IDX_TEST_PP2_STATION_DN_2; }break;
	case LOC_IDX_STATION_3: { 	nHistoryStage = (m_nTestPPIdx == eTestPP_1) ? HISTORY_LOC_IDX_TEST_PP1_STATION_DN_3 : HISTORY_LOC_IDX_TEST_PP2_STATION_DN_3; }break;
	case LOC_IDX_STATION_4: { 	nHistoryStage = (m_nTestPPIdx == eTestPP_1) ? HISTORY_LOC_IDX_TEST_PP1_STATION_DN_4 : HISTORY_LOC_IDX_TEST_PP2_STATION_DN_4; }break;
	case LOC_IDX_TEST_PP_TBL_1_LD: { 	nHistoryStage = (m_nTestPPIdx == eTestPP_1) ? HISTORY_LOC_IDX_TEST_PP1_LD_TBL_1 : HISTORY_LOC_IDX_TEST_PP2_LD_TBL_1;}break;
	case LOC_IDX_TEST_PP_TBL_2_LD: { 	nHistoryStage = (m_nTestPPIdx == eTestPP_1) ? HISTORY_LOC_IDX_TEST_PP1_LD_TBL_2 : HISTORY_LOC_IDX_TEST_PP2_LD_TBL_2;}break;
	case LOC_IDX_TEST_PP_TBL_3_LD: { 	nHistoryStage = (m_nTestPPIdx == eTestPP_1) ? HISTORY_LOC_IDX_TEST_PP1_LD_TBL_3 : HISTORY_LOC_IDX_TEST_PP2_LD_TBL_3;}break;
	case LOC_IDX_TEST_PP_TBL_4_LD: { 	nHistoryStage = (m_nTestPPIdx == eTestPP_1) ? HISTORY_LOC_IDX_TEST_PP1_LD_TBL_4 : HISTORY_LOC_IDX_TEST_PP2_LD_TBL_4;}break;
	case LOC_IDX_TEST_PP_TBL_1_UD: { 	nHistoryStage = (m_nTestPPIdx == eTestPP_1) ? HISTORY_LOC_IDX_TEST_PP1_UD_TBL_1 : HISTORY_LOC_IDX_TEST_PP2_UD_TBL_1;}break;
	case LOC_IDX_TEST_PP_TBL_2_UD: { 	nHistoryStage = (m_nTestPPIdx == eTestPP_1) ? HISTORY_LOC_IDX_TEST_PP1_UD_TBL_2 : HISTORY_LOC_IDX_TEST_PP2_UD_TBL_2;}break;
	case LOC_IDX_TEST_PP_TBL_3_UD: { 	nHistoryStage = (m_nTestPPIdx == eTestPP_1) ? HISTORY_LOC_IDX_TEST_PP1_UD_TBL_3 : HISTORY_LOC_IDX_TEST_PP2_UD_TBL_3;}break;
	case LOC_IDX_TEST_PP_TBL_4_UD: { 	nHistoryStage = (m_nTestPPIdx == eTestPP_1) ? HISTORY_LOC_IDX_TEST_PP1_UD_TBL_4 : HISTORY_LOC_IDX_TEST_PP2_UD_TBL_4;}break;
	case LOC_IDX_CLEAN_TBL: { 	nHistoryStage = (m_nTestPPIdx == eTestPP_1) ? HISTORY_LOC_IDX_TEST_PP1_CLEAN_BUFFER : HISTORY_LOC_IDX_TEST_PP2_CLEAN_BUFFER; }break;
	default:
		break;
	}

	return nHistoryStage;
}

int CTaskTestPP::FromTestPPToLoadTableIdx(int nCmdStage)
{
	int nLoadTbleIdx = -1;
// 	if (eTestPP_1 == m_nTestPPIdx)
// 	{
// 		if (nHandType == eTestPPLoadType)
// 			nLoadTbleIdx = eLoadTbl_2;
// 		else
// 			nLoadTbleIdx = eLoadTbl_1;
// 	}
// 	if (eTestPP_2 == m_nTestPPIdx)
// 	{
// 		if (nHandType == eTestPPLoadType)
// 			nLoadTbleIdx = eLoadTbl_3;
// 		else
// 			nLoadTbleIdx = eLoadTbl_4;
// 	}		 
	switch (nCmdStage)
	{
	case LOC_IDX_TEST_PP_TBL_1_LD:
	case LOC_IDX_TEST_PP_TBL_2_LD:
	case LOC_IDX_TEST_PP_TBL_3_LD:
	case LOC_IDX_TEST_PP_TBL_4_LD:
	{
		if(m_nTestPPIdx == eTestPP_1)
			nLoadTbleIdx = eLoadTbl_2;
		else
			nLoadTbleIdx = eLoadTbl_3;
	}break;
	case LOC_IDX_TEST_PP_TBL_1_UD:
	case LOC_IDX_TEST_PP_TBL_2_UD:
	case LOC_IDX_TEST_PP_TBL_3_UD:
	case LOC_IDX_TEST_PP_TBL_4_UD:
	{
		if (m_nTestPPIdx == eTestPP_1)
			nLoadTbleIdx = eLoadTbl_1;
		else
			nLoadTbleIdx = eLoadTbl_4;
	}break;
	}

	return nLoadTbleIdx;
}

int CTaskTestPP::FromTestPPToLoadTableCmdStageConvetor(int nCmdStage)
{
	int nCStage = -1;
	switch (nCmdStage)
	{
		case LOC_IDX_TEST_PP_TBL_1_LD:
		case LOC_IDX_TEST_PP_TBL_2_LD:
		case LOC_IDX_TEST_PP_TBL_3_LD:
		case LOC_IDX_TEST_PP_TBL_4_LD:	
		{
			nCStage = LOC_IDX_LD_TBL_TEST_SIDE_1 + (nCmdStage - LOC_IDX_TEST_PP_TBL_1_LD);
		}break;
		case LOC_IDX_TEST_PP_TBL_1_UD:
		case LOC_IDX_TEST_PP_TBL_2_UD:
		case LOC_IDX_TEST_PP_TBL_3_UD:
		case LOC_IDX_TEST_PP_TBL_4_UD:
		{
			nCStage = LOC_IDX_LD_TBL_TEST_SIDE_1 + (nCmdStage - LOC_IDX_TEST_PP_TBL_1_UD);
		}break;
	}

	return nCStage;
}

int CTaskTestPP::Chk_Z_Safety_Sen_All(int nHandType)
{
	std::vector<CPoint> vPicker;
	for (int y = 0; y < MAX_TRAY_PP_CNT_Y; y++) {
		for (int x = 0; x < MAX_TRAY_PP_CNT_X; x++) {
			vPicker.push_back(CPoint(x, y));
		}
	}
	return Chk_Z_Safety_Sen(vPicker, nHandType);
}

int CTaskTestPP::Chk_Z_Safety_Sen(std::vector<CPoint> vPicker,int nHandType)
{
	bool bSafety = false;

	CString strPlaceErrPkr = _T("");

	_eBbxmnpAxis picker_base;
	eMotor eMotorZ;


	if (nHandType == eTestPPLoadType){eMotorZ = eMotor::eAxisZ1;}
	else { eMotorZ = eMotor::eAxisZ2; }


	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
	
	if (m_nTestPPIdx == eTestPP_1) {
		if (nHandType == eTestPPLoadType)
			picker_base = eBMAxis_LOAD_TESTPP_1_Picker01;
		else
			picker_base = eBMAxis_UNLOAD_TESTPP_1_Picker01;
	}
	else {
		if (nHandType == eTestPPLoadType)
			picker_base = eBMAxis_LOAD_TESTPP_2_Picker01;
		else
			picker_base = eBMAxis_UNLOAD_TESTPP_2_Picker01;
	}

	// picker를 지정하지 않았으면 모든 picker를 확인하는 것으로 본다.
	if (vPicker.empty())
	{
		for (int y = 0; y < MAX_TEST_PP_PKR_CNT_Y; y++) {
			for (int x = 0; x < MAX_TEST_PP_PKR_CNT_X; x++) {
				vPicker.push_back(CPoint(x, y));
			}
		}
	}

	int nChk = 0;
	int nCount = MAX_TEST_PP_PKR_CNT_Y*MAX_TEST_PP_PKR_CNT_X - 1;
	for (auto it = vPicker.begin(); it != vPicker.end(); it++) {
			_eBbxmnpAxis picker;
			if (nBoardType == eBoardDir_Backward)
			{
				picker = (_eBbxmnpAxis)((int)picker_base + abs(((*it).y*MAX_TEST_PP_PKR_CNT_X) + (*it).x - nCount));
			}
			else {
				picker = (_eBbxmnpAxis)((int)picker_base + (*it).x + ((*it).y*MAX_TEST_PP_PKR_CNT_X));
			}
		if (m_vMotor[eMotorZ]->IsHomeSensorOn(picker) == TRUE) {
			nChk++;
		}
	}

	// home sensor가 모두 감지되어 있으면 safety
	if (nChk == vPicker.size())
	{
		bSafety = true;
	}

	return (bSafety == true) ? ERR_NO_ERROR : -1;
}

int CTaskTestPP::Chk_Z_Safety_Sen_Check_Type(std::vector<CPoint> vPicker, int nHandType, ePickerSafetySensorCheckType _ePickerHomeSensorCheckType)
{
	bool bSafety = false;
	for (int i = 0; i < eMaxTestPPTypeCount; i++)
	{
		m_vSafetyFailPicker[i][_ePickerHomeSensorCheckType].clear();
		m_sSaftyFailPicker[i][_ePickerHomeSensorCheckType] = _T("");
	}

	CString strPlaceErrPkr = _T("");

	_eBbxmnpAxis picker_base;
	eMotor eMotorZ;


	if (nHandType == eTestPPLoadType) { eMotorZ = eMotor::eAxisZ1; }
	else { eMotorZ = eMotor::eAxisZ2; }


	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);

	if (m_nTestPPIdx == eTestPP_1) {
		if (nHandType == eTestPPLoadType)
			picker_base = eBMAxis_LOAD_TESTPP_1_Picker01;
		else
			picker_base = eBMAxis_UNLOAD_TESTPP_1_Picker01;
	}
	else {
		if (nHandType == eTestPPLoadType)
			picker_base = eBMAxis_LOAD_TESTPP_2_Picker01;
		else
			picker_base = eBMAxis_UNLOAD_TESTPP_2_Picker01;
	}

	// picker를 지정하지 않았으면 모든 picker를 확인하는 것으로 본다.
	if (vPicker.empty())
	{
		for (int y = 0; y < MAX_TEST_PP_PKR_CNT_Y; y++) {
			for (int x = 0; x < MAX_TEST_PP_PKR_CNT_X; x++) {
				vPicker.push_back(CPoint(x, y));
			}
		}
	}

	int nChk = 0;
	int nCount = MAX_TEST_PP_PKR_CNT_Y*MAX_TEST_PP_PKR_CNT_X - 1;
	for (auto it = vPicker.begin(); it != vPicker.end(); it++) {
		_eBbxmnpAxis picker;
		if (nBoardType == eBoardDir_Backward)
		{
			picker = (_eBbxmnpAxis)((int)picker_base + abs(((*it).y*MAX_TEST_PP_PKR_CNT_X) + (*it).x - nCount));
		}
		else {
			picker = (_eBbxmnpAxis)((int)picker_base + (*it).x + ((*it).y*MAX_TEST_PP_PKR_CNT_X));
		}
		if (m_vMotor[eMotorZ]->IsHomeSensorOn(picker) == TRUE) {
			nChk++;
			if (g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
			{
				if (_ePickerHomeSensorCheckType == ePlaceSafetySensorCheck) //Place Check
				{
					// Place 위치로 이동 후, Home Sensor 감지 되면 알람 처리
					strPlaceErrPkr.Format("[X:%d, Y:%d]", (*it).x + 1, (*it).y + 1);
					m_sSaftyFailPicker[nHandType][_ePickerHomeSensorCheckType] += strPlaceErrPkr;
					m_vSafetyFailPicker[nHandType][_ePickerHomeSensorCheckType].push_back(CPoint((*it).x, (*it).y));
				}
			}
		}
		else{
			if (_ePickerHomeSensorCheckType == eSafetySensorCheck) //Safety Sensor Check
			{
				strPlaceErrPkr.Format("[X:%d, Y:%d]", (*it).x + 1, (*it).y + 1);
				m_sSaftyFailPicker[nHandType][_ePickerHomeSensorCheckType] += strPlaceErrPkr;
				m_vSafetyFailPicker[nHandType][_ePickerHomeSensorCheckType].push_back(CPoint((*it).x, (*it).y));
			}
		}
	}

	// home sensor가 모두 감지되어 있으면 safety
	if (nChk == vPicker.size())
	{
		bSafety = true;
	}

	return (bSafety == true) ? ERR_NO_ERROR : -1;
}

int CTaskTestPP::TeachPosIdxConvertor(int nAxisIdx, int nTestHandType)
{
	int nIdx = -1;
	switch (nAxisIdx)
	{
	case eAxisX1: {nIdx = ePos_AxisX1; }break;
	case eAxisX2: {nIdx = ePos_AxisX2; }break;
	case eAxisY:
	{
		if (nTestHandType == eTestPPLoadType)
			nIdx = ePos_AxisY1;
		else
			nIdx = ePos_AxisY2;
	}break;

	case eAxisZ1: {nIdx = ePos_AxisZ1; }break;
	case eAxisZ2: {nIdx = ePos_AxisZ2; }break;
	case eAxisPitch_HW_Y1_SW_X1: {nIdx = ePos_AxisPitchY1; }break; //ePos_AxisPitchX1
	case eAxisPitch_HW_Y2_SW_X2: {nIdx = ePos_AxisPitchY2; }break; //ePos_AxisPitchX2
	case eAxisPitch_HW_X1_SW_Y1: {nIdx = ePos_AxisPitchX1; }break; //ePos_AxisPitchY1
	case eAxisPitch_HW_X2_SW_Y2: {nIdx = ePos_AxisPitchX2; }break; //ePos_AxisPitchY1
	default:
		break;
	}

	return nIdx;
}

BOOL CTaskTestPP::CompareXData(double dData1, double dData2)
{
	//dData1 : Target 위치, 현재 위치
	//dData2 : 절대 위치
	// True면 dData2내에 dData1 존재.
	if (m_nTestPPIdx == eTestPP_1) {
		return (dData1 > dData2) ? TRUE : FALSE;
	}
	else {
		return (dData1 < dData2) ? TRUE : FALSE;
	}

}


BOOL CTaskTestPP::TestSdHasDvc()
{
	BOOL bHasTestSdDvc = FALSE;

	int nPressIdxStr = (m_nTestPPIdx == eTestPP_1) ? ePressUnit_Down_1 : ePressUnit_Down_5;
	int device_cnt_in_all_dut = 0;

	for(int i = 0; i < eMaxPressUnitCount / 2; i++) {
		device_cnt_in_all_dut += g_TaskPressUnit[nPressIdxStr + i].ChkSiteInDvc();
	}

	int load_device_cnt = (m_nTestPPIdx == eTestPP_1) ? g_TaskLoadTbl[eLoadTbl_2].GetLoadDeviceCnt() : g_TaskLoadTbl[eLoadTbl_3].GetLoadDeviceCnt();

	if (device_cnt_in_all_dut > 0 || g_TaskTestPP[m_nTestPPIdx].GetUnloadPickerHaveDvcCnt() > 0 || g_TaskTestPP[m_nTestPPIdx].GetLoadPickerDeviceCnt()
		|| load_device_cnt > 0 || g_TaskTrayLoadPP.GetTrayPpLoadDvcCnt() > 0)
	{
		bHasTestSdDvc = TRUE;
	}

	return bHasTestSdDvc;
}

/*========================================================================
COMMENTS : Test PP CmdPicker, CmdPocket에 맞는 Table Move Step 확인 함수.
RETURN : TRUE -> 테이블 이동 필요.
		 FALSE -> 테이블 이동 불필요.
==========================================================================*/
/*
현재 해당 concept 사용 불가.
UPH Teaching 위치에 Loading Table Teaching시, 영역 문제로 Unload Hand가 Load Table Teaching 불가.

*/
BOOL CTaskTestPP::ChkTableMove(int nHandType, int nLocStage, CPoint Pocket, std::vector<CPoint> vPicker, int &nStep)
{
	nStep = 0;

	//nLocStage #1, #4만 진행
	if ( nLocStage != LOC_IDX_TEST_PP_TBL_1_LD && nLocStage != LOC_IDX_TEST_PP_TBL_4_LD &&
		nLocStage != LOC_IDX_TEST_PP_TBL_1_UD && nLocStage != LOC_IDX_TEST_PP_TBL_4_UD)
	{
		return FALSE;
	}

	//Pocket X == 0 과 Picker X == 0 존재할 경우, Step 계산 필요 없음.
	// -> Pitch 이동이 필요 없는 상황
	for (int i = 0; i < vPicker.size(); i++)
	{
		if (vPicker[i].x == 0 && Pocket.x == 0)
		{
			return FALSE;
		}
	}
	
	//첫번째 Picker 찾기.
	int nFindFirstPkrX = vPicker[0].x;
	for (int i = 0; i < vPicker.size(); i++)
	{
		int nMinIdx = nFindFirstPkrX;
		if (nMinIdx > vPicker[i].x)
			nFindFirstPkrX = vPicker[i].x;
	}


	//Picker idx와 Pocket idx 차이 만큼 Pocket 이동 진행.
	//Test PP#1 & Test PP#2가 대칭 구조

	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
	if (nBoardType == eBoardDir_Forward)
	{
		nStep = (m_nTestPPIdx == eTestPP_1) ? (nFindFirstPkrX - Pocket.x) : (Pocket.x - nFindFirstPkrX);
	}
	else
	{
		nStep = (m_nTestPPIdx == eTestPP_1) ? (Pocket.x - nFindFirstPkrX) : (nFindFirstPkrX - Pocket.x);
	}


	if (nLocStage == LOC_IDX_TEST_PP_TBL_1_LD || nLocStage == LOC_IDX_TEST_PP_TBL_1_UD)
	{
		//Hand (-)방향 이동하려면 Table(+)로 이동
		//Hand (+)방향 Limit없이 자유로움, Table Step (-)는 필요 없음.
		if (nStep < 0) {
			nStep = 0;
		}
	}
	else //nLocStage == LOC_IDX_TEST_PP_TBL_4_LD || nLocStage == LOC_IDX_TEST_PP_TBL_4_UD
	{
		//Hand (+)방향 이동하려면 Table(-)로 이동
		//Hand (-)방향 Limit없이 자유로움, Table Step (+)는 필요 없음.
		if (nStep > 0) {
			nStep = 0;
		}
	}

	if (nStep != 0) {
		return TRUE;
	}

	return FALSE;
}

BOOL CTaskTestPP::IsHandStationArea(double dTPos)
{
	//X축 Station 영역 확인.
	BOOL bChkStationArea = FALSE;
	if (m_nTestPPIdx == eTestPP_1)
	{
		if (dTPos < INTERLOCK_TESTPP_1_X_STATION_START_POS) { bChkStationArea = TRUE; }
	}
	else
	{
		if (dTPos > INTERLOCK_TESTPP_2_X_STATION_START_POS) { bChkStationArea = TRUE; }
	}

	return bChkStationArea;
}

int CTaskTestPP::IsYRangeRelatedPress_Z_Safety(double dTPosX, double dTPosY, int nHandType, std::vector<int> &nRelatedStationNo)
{
	BOOL bChkStationArea = IsHandStationArea(dTPosX);

	if (!bChkStationArea) { return ERR_NO_ERROR; }

// 	int nRetStationNum = -1;
// 	double dCurYPos = 0.0;
// 	OneAxis* pMotorY = g_TaskTestPP[m_nTestPPIdx].GetAxis(CTaskTestPP::eAxisY);
// 	pMotorY->GetCurrentPos(dCurYPos);

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

// 	std::vector<int> vStationNo;
// 	vStationNo.clear();

	if (dStartPos1 <= dTPosY && dTPosY <= dEndPos1) {
		nRelatedStationNo.push_back(LOC_IDX_STATION_1);
		//nRetStationNum = LOC_IDX_STATION_1;
	}
	if (dStartPos2 <= dTPosY && dTPosY <= dEndPos2) {
		nRelatedStationNo.push_back(LOC_IDX_STATION_2);
		//nRetStationNum = LOC_IDX_STATION_2;
	}
	if (dStartPos3 <= dTPosY && dTPosY <= dEndPos3) {
		nRelatedStationNo.push_back(LOC_IDX_STATION_3);
		//nRetStationNum = LOC_IDX_STATION_3;
	}
	if (dStartPos4 <= dTPosY && dTPosY <= dEndPos4) {
		nRelatedStationNo.push_back(LOC_IDX_STATION_4);
		//nRetStationNum = LOC_IDX_STATION_4;
	}


	std::vector<int> vLocList;
	vLocList.clear();
	g_TaskTestPP[m_nTestPPIdx].WhereIsMotorPos(nHandType, vLocList, CTaskTestPP::eAxisAll);

	if (nRelatedStationNo.size() != 0) {
		for (int i = 0; i < (int)nRelatedStationNo.size(); i++) {
			int nZidx = (nHandType == eTestPPLoadType) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
			if (vLocList[nZidx] <= CTaskTestPP::ePosZ_DownSafety)
			{
				int nPressIdx = g_TaskTestPP[m_nTestPPIdx].GetPressNum(LOC_IDX_STATION_1 + nRelatedStationNo[i]);
				if (g_TaskPressUnit[nPressIdx].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON) != ERR_NO_ERROR) {
					return -1;
				}
					//return ERR_PRESS_UNIT_1_NOT_DETECT_SAFETY_SEN + nPressIdx;
				if (g_TaskPressUnit[nPressIdx].ChkMotorPos(LOC_IDX_PRESS_UNIT_SAFETY, CTaskPressUnit::eAxisAll) != ERR_NO_ERROR) {
					return -1;
				}
					//return ERR_MOT_INTERLOCK_PRESS_UNIT_1_NOT_DETECT_SAFETY_POS + nPressIdx;
			}
		}
	}

	return ERR_NO_ERROR;

}

int	CTaskTestPP::GetPickerErrorCase(int nAxisDone, int nErrSen, std::vector<CPoint> vAxisDonePkr, std::vector<CPoint> vErrSen, int nHandType, std::vector<CPoint>& vErrPicker, CString& sAlarmData)
{
	int nErr = -1;
	CString strErrPkr = _T(""), strErrTempPkr = _T("");

	std::vector<CPoint> vFailPkr;
	vFailPkr.clear();
	
	// AxisDone ERR, Safety Sensor NO_ERR
	if ((nAxisDone != ERR_NO_ERROR && nErrSen == ERR_NO_ERROR) || (nAxisDone != ERR_NO_ERROR && nErrSen != ERR_NO_ERROR))
	{
		vFailPkr = AxisDoneFailPickerIdxConverter(vAxisDonePkr);
		for (auto it = vFailPkr.begin(); it != vFailPkr.end(); it++) {
			strErrTempPkr.Format("[X:%d, Y:%d]", (*it).x + 1, (*it).y + 1);
			strErrPkr += strErrTempPkr;
		}

		sAlarmData.Format("test pp %d %s hand z axis move safety position move time over!! [%s]", m_nTestPPIdx+1, nHandType == 0 ? "load " : "unload", strErrPkr);
		nErr = ERR_TEST_PP_1_Z1_PICKER_MOVE_TIMEOUT + GetHandTypeIndex(nHandType);
		vErrPicker = vFailPkr;

	}// AxisDone NO_ERR, Safety Sensor ERR
	else if (nAxisDone == ERR_NO_ERROR && nErrSen != ERR_NO_ERROR)
	{
		vFailPkr = AxisDoneFailPickerIdxConverter(vErrSen);
		for (auto it = vFailPkr.begin(); it != vFailPkr.end(); it++) {
			strErrTempPkr.Format("[X:%d, Y:%d]", (*it).x + 1, (*it).y + 1);
			strErrPkr += strErrTempPkr;
		}

		sAlarmData.Format("test pp %d %s hand z axis safety sensor not detected time over!! [%s]", m_nTestPPIdx + 1, nHandType == 0 ? "load " : "unload", strErrPkr);
		nErr = ERR_TEST_PP_1_Z1_Z_PICKER_SAFETY_SENSOR_NOT_DETECTED + GetHandTypeIndex(nHandType);
		vErrPicker = vFailPkr;

	}
	else { // 정상
		nErr = ERR_NO_ERROR;
	}

	return nErr;
}

void CTaskTestPP::SetVatLogHandle(HWND hOwner)
{
	m_hLog = hOwner;
}

void CTaskTestPP::WriteDisplayLog(const char * fmt, ...)
{
	char szBuff[MAX_BUFF] = { 0, };

	va_list va;
	va_start(va, fmt);
	vsprintf(szBuff, fmt, va);
	va_end(va);

	//Display Log는 LD/UD 같은 Hand에 표기.
	int nHand = (m_nTestPPIdx == eTestPP_1) ? VAT_HAND_TESTPP1 : VAT_HAND_TESTPP2;

	::SendMessage(m_hLog, UM_VAT_SEQ_MSG_PRINT_LOG, (WPARAM)nHand, (LPARAM)szBuff);
}

void CTaskTestPP::MakeVatLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = { 0, };
	if (fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf_s(tmpMsg, fmt, argList);
		va_end(argList);
	}

	int nLogID = (m_nTestPPIdx == eTestPP_1) ? eLogID_VAT_Test_1 : eLogID_VAT_Test_1;

	TWLOG.MakeLog(static_cast<int>(nLogID), Debug, "", __LINE__, NULL, tmpMsg);
}


int	CTaskTestPP::do_VATMovePos(int nAxisIdx, double dTargetPos, double dPosOffset/* = 0.0*/)
{
	ASSERT(0 <= nAxisIdx && nAxisIdx < eAxisMax);
	if (eAxisZ1 <= nAxisIdx && nAxisIdx < eAxisZ2)
		return ERR_CMD_PARAM_INVALID;

	OneAxis* pMotor = GetAxis(nAxisIdx);
	if (pMotor == nullptr) {
		return ERR_MOTOR_DATA_IS_NULL_POINTER;
	}

	int	   nRateSpd = DEF_VAT_MOTOR_SPEED_RATE;/*(double)g_DMCont.m_dmTrayPP.GN(nIndexSpd[m_nRobotId_Idx][nAxisIdx]);*/
	int    nRateAcc = DEF_VAT_MOTOR_ACC_RATE;/*g_DMCont.m_dmTrayPP.GN(nIndexAcc[m_nRobotId_Idx][nAxisIdx]);*/
	int    nRateDcc = DEF_VAT_MOTOR_DCC_RATE;/*g_DMCont.m_dmTrayPP.GN(nIndexDcc[m_nRobotId_Idx][nAxisIdx]);*/

	double dRetSpd = 0;
	int    nRetAcc = 0, nRetDcc = 0;

	double dMtSpd = pMotor->GetSpeed();
	double dMtAcc = pMotor->GetAcc();
	double dMtDcc = pMotor->GetDcc();

	dRetSpd = (dMtSpd * (nRateSpd / 100.0)); /** (nSeqSpdRate / 100.0);*/
	nRetAcc = (int)(dMtAcc - (dMtAcc * 0.75) *(nRateAcc / 100.0)); // 설정되어 있는 값에서 spd rate가 높을 수록 acc/dcc를 감소 시켜준다. 가장 빠를 때가 설정 속도의 95%뺀 acc/dcc로 함.
	nRetDcc = (int)(dMtDcc - (dMtDcc * 0.75) *(nRateDcc / 100.0));

	if (nRetAcc < 100) nRetAcc = 100;
	if (nRetDcc < 100) nRetDcc = 100;

	double dFTPos = dTargetPos + dPosOffset;
	MakeVatLog("%s : Final Target Pos : %.3fmm, Org Target Pos : %.3fmm, Pos Offset : %.3fmm", pMotor->GetAxisName(), dFTPos / 1000.0, dTargetPos / 1000.0, dPosOffset / 1000.0);

	int nErr = -1;

	nErr = pMotor->Move(dFTPos, dRetSpd, nRetAcc, nRetDcc, TRUE); // ApplyBacklash = TRUE

	return nErr;
}


int	CTaskTestPP::do_VATMovePos_Z(int nAxisIdx, std::vector<CPoint> vPicker, double dTargetPos, double dPosOffset/* = 0.0*/)
{
	ASSERT(eAxisZ1 <= nAxisIdx && nAxisIdx <= eAxisZ2);

	OneAxis* pMotor = GetAxis(nAxisIdx);
	//(nTestHandType == VAT_TESTPP_LOAD) ? GetAxis(CTaskTestPP::eAxisZ1) : GetAxis(CTaskTestPP::eAxisZ2);

	if (pMotor == nullptr) {
		return ERR_MOTOR_DATA_IS_NULL_POINTER;
	}

	int	   nRateSpd = DEF_VAT_MOTOR_SPEED_RATE;/*(double)g_DMCont.m_dmTrayPP.GN(nIndexSpd[m_nRobotId_Idx][nAxisIdx]);*/
	int    nRateAcc = DEF_VAT_MOTOR_ACC_RATE;/*g_DMCont.m_dmTrayPP.GN(nIndexAcc[m_nRobotId_Idx][nAxisIdx]);*/
	int    nRateDcc = DEF_VAT_MOTOR_DCC_RATE;/*g_DMCont.m_dmTrayPP.GN(nIndexDcc[m_nRobotId_Idx][nAxisIdx]);*/

	double dRetSpd = 0;
	int    nRetAcc = 0, nRetDcc = 0;

	double dMtSpd = pMotor->GetSpeed();
	double dMtAcc = pMotor->GetAcc();
	double dMtDcc = pMotor->GetDcc();


	dRetSpd = (dMtSpd * (nRateSpd / 100.0)); /** (nSeqSpdRate / 100.0);*/
	nRetAcc = (int)(dMtAcc - (dMtAcc * 0.75) *(nRateAcc / 100.0)); // 설정되어 있는 값에서 spd rate가 높을 수록 acc/dcc를 감소 시켜준다. 가장 빠를 때가 설정 속도의 95%뺀 acc/dcc로 함.
	nRetDcc = (int)(dMtDcc - (dMtDcc * 0.75) *(nRateDcc / 100.0));

	if (nRetAcc < 100) nRetAcc = 100;
	if (nRetDcc < 100) nRetDcc = 100;

	double dFTPos = dTargetPos + dPosOffset;
	MakeVatLog("%s : Final Target Pos : %.3fmm, Org Target Pos : %.3fmm, Pos Offset : %.3fmm", pMotor->GetAxisName(), dFTPos / 1000.0, dTargetPos / 1000.0, dPosOffset / 1000.0);

	int nErr = -1;

	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
	if (vPicker.size() == 1) {
		//Picker 1개 제어
		int nPickerNum = vPicker[0].x + 4 * (vPicker[0].y);
		if (nBoardType == eBoardDir_Backward) {
			nPickerNum = abs(nPickerNum - 7);
		}

		int nTestHandType = (nAxisIdx == eAxisZ1) ? VAT_TESTPP_LOAD : VAT_TESTPP_UNLOAD;

		nErr = pMotor->Move((_eBbxmnpAxis)m_nBbxmnpAxis[nTestHandType][nPickerNum], dFTPos, dRetSpd, nRetAcc, nRetDcc, nBoardType, TRUE); //bZpickerNouseOffset == TRUE
	}
	else {
		//Picker 여러개 제어
		nErr = pMotor->Move(vPicker, dFTPos, dRetSpd, nRetAcc, nRetDcc, nBoardType, TRUE);//bZpickerNouseOffset == TRUE
	}

	return nErr;
}


void CTaskTestPP::_SetFlatnessOffset(int nHandType, int nStage)
{
	ASSERT(VAT_IDX_STATION_1 <= nStage && nStage <= VAT_IDX_CLEAN_TBL);

	double dPickerGap[VAT_PICKER_MAX] = { 0, };

	int nHand = -1;
	if (m_nTestPPIdx == eTestPP_1) {
		nHand = (nHandType == VAT_TESTPP_LOAD) ? VAT_HAND_LOAD_TESTPP1 : VAT_HAND_UNLOAD_TESTPP1;
	}
	else {
		nHand = (nHandType == VAT_TESTPP_LOAD) ? VAT_HAND_LOAD_TESTPP2 : VAT_HAND_UNLOAD_TESTPP2;
	}

	for (int i = 0; i < VAT_PICKER_MAX; i++) {
		dPickerGap[i] = g_VATCommonData.m_stVATZTeachInfo.stPickerGap.getPickerGap(nHand, nStage, i);
	}

	g_pBbxmnpClient->SetFlatnessOffset(nHand, dPickerGap);
}

int CTaskTestPP::Interlock_X_Related_Clean_Table_Range()
{
	int nSafetyX1Sen = g_TaskTestPP[m_nTestPPIdx].ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON);

	//Safety Pos
	double dCmdPos[CTaskTestPP::eAxisMax] = { 0, };
	std::vector<CPoint> vTempPkr;
	vTempPkr.push_back(CPoint(0, 0));
	g_TaskTestPP[m_nTestPPIdx].GetTeachPos(LOC_IDX_CLEAN_TBL, CPoint(0, 0), vTempPkr, CTaskTestPP::eZPOS_SAFETY, dCmdPos, eTestPPLoadType);

	OneAxis* pMotorX1 = g_TaskTestPP[m_nTestPPIdx].GetAxis(CTaskTestPP::eAxisX1);
	double dCurPosX1 = 0.0;
	pMotorX1->GetCurrentPos(dCurPosX1);

	//BOOL bIsX1SafetyPos = (m_nTestPPIdx == eTestPP_1) ? dCurPosX1 > dCmdPos[CTaskTestPP::eAxisX1] : dCurPosX1 < dCmdPos[CTaskTestPP::eAxisX1];
	BOOL bIsX1SafetyPos = (m_nTestPPIdx == eTestPP_1) ? dCurPosX1 > INTERLOCK_TESTPP1_X_CLEAN_TABLE_LIMIT : dCurPosX1 < INTERLOCK_TESTPP2_X_CLEAN_TABLE_LIMIT;
	double dCurPosY = 0.0;
	OneAxis* pMotorY = g_TaskTestPP[m_nTestPPIdx].GetAxis(CTaskTestPP::eAxisY);
	pMotorY->GetCurrentPos(dCurPosY);

	int nPressZSafetySen = g_TaskPressUnit[ePressUnit_Down_1 + (eMaxATS_Area * m_nTestPPIdx)].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON);

	if (dCurPosY <= INTERLOCK_TESTPP_Y_CLEAN_TABLE_LIMIT && bIsX1SafetyPos != TRUE)
	{
// 		if (nSafetyX1Sen != ERR_NO_ERROR) 
// 		{
// 			return ERR_TEST_PP_1_X1_IS_NOT_SAFETY_IN_CLEANTBL_POS + m_nTestPPIdx;
// 		}

		if (nPressZSafetySen != ERR_NO_ERROR)
		{
			return ERR_TEST_PP_1_X1_IS_NOT_SAFETY_IN_CLEANTBL_POS + m_nTestPPIdx;
		}
	}

	return ERR_NO_ERROR;
}

void CTaskTestPP::Interlock_Y_Range_Related_Press_Z_Safety(double dCurPos, int nHandType, std::vector<int> &nRelatedStationNo)
{
	BOOL bChkStationArea = FALSE;
	if (m_nTestPPIdx == eTestPP_1)
	{
		if (dCurPos < INTERLOCK_TESTPP_1_X_STATION_START_POS) { bChkStationArea = TRUE; }
	}
	else
	{
		if (dCurPos > INTERLOCK_TESTPP_2_X_STATION_START_POS) { bChkStationArea = TRUE; }
	}

	if (!bChkStationArea) { return; }

	int nRetStationNum = -1;
	double dCurYPos = 0.0;
	OneAxis* pMotorY = g_TaskTestPP[m_nTestPPIdx].GetAxis(CTaskTestPP::eAxisY);
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
}


int CTaskTestPP::Check_X_Related_Press_Z_Safety()
{
	//Test PP Recovery & Press Safety 충돌 영역 확인.
	for (int nTestPpType = eTestPPLoadType; nTestPpType < eMaxTestPPTypeCount; nTestPpType++)
	{
		int nAxis = (nTestPpType == eTestPPLoadType) ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2;
		OneAxis* pMotorX = GetAxis(nAxis);

		//X축 CurPos
		double dCurPosX = 0.0;
		pMotorX->GetCurrentPos(dCurPosX);

		//X축 Station 영역 확인.
		BOOL bChkXStationArea = FALSE;
		if (m_nTestPPIdx == eTestPP_1) {
			if (dCurPosX < INTERLOCK_TESTPP_1_X_STATION_START_POS) { bChkXStationArea = TRUE; }
		}
		else {
			if (dCurPosX > INTERLOCK_TESTPP_2_X_STATION_START_POS) { bChkXStationArea = TRUE; }
		}

		if (bChkXStationArea)
		{
			std::vector<int> vStationNo;
			vStationNo.clear();

			g_TaskTestPP[m_nTestPPIdx].Interlock_Y_Range_Related_Press_Z_Safety(dCurPosX, nTestPpType, vStationNo);
			if (vStationNo.size() != 0) {
				for (int i = 0; i < (int)vStationNo.size(); i++) {
					int nPressIdx = GetPressNum(LOC_IDX_STATION_1 + vStationNo[i]);
					if (g_TaskPressUnit[nPressIdx].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON) != ERR_NO_ERROR) {
						int nErr = (nTestPpType == eTestPPLoadType) ? ERR_TEST_PP_LOAD_X_NOT_DETECT_SAFETY_SENSOR_CHECK_Y_NOT_IN_POS_PRESS_1 : ERR_TEST_PP_UNLOAD_X_NOT_DETECT_SAFETY_POS_CHECK_Y_NOT_IN_POS_PRESS_1;
						return nErr + nPressIdx;
					}
					else if (g_TaskPressUnit[nPressIdx].ChkMotorPos(LOC_IDX_PRESS_UNIT_SAFETY, CTaskPressUnit::eAxisAll) != ERR_NO_ERROR) {
						int nErr = (nTestPpType == eTestPPLoadType) ? ERR_TEST_PP_LOAD_X_NOT_DETECT_SAFETY_POS_CHECK_Y_NOT_IN_POS_PRESS_1 : ERR_TEST_PP_UNLOAD_X_NOT_DETECT_SAFETY_POS_CHECK_Y_NOT_IN_POS_PRESS_1;
						return nErr + nPressIdx;
					}
				}
			}
		}
	}
	

	return ERR_NO_ERROR;
}

int CTaskTestPP::CheckDeviceType(int nCmdStage, int nHandType, int nTargetX, int nTargetY, std::vector<CPoint> vCmdPicker, int nPickPlace)
{
	ST_DD_DEVICE stSourceData;
	ST_DD_DEVICE stTargetData;
	ST_DD_DEVICE stEmptyDevice; stEmptyDevice.clear();

	//[작업 필요]
	// hand pitch type check <srt> ==============================================
	int nX_PitchType = GetHandPitchType(CTaskTestPP::eAxisPitch_HW_X1_SW_Y1, nCmdStage);	// Hand와 Target간의 간격 관계 (0=1by1, 1=1:1매칭, 2=한칸뛰기,3=두칸뛰기), Hand 기준임. (Hand 1,3으로 Target 1,2 집는것은 처리 필요)
	int nY_PitchType = GetHandPitchType(CTaskTestPP::eAxisPitch_HW_Y1_SW_X1, nCmdStage);	// Hand와 Target간의 간격 관계 (0=1by1, 1=1:1매칭, 2=한칸뛰기,3=두칸뛰기), Hand 기준임. (Hand 1,3으로 Target 1,2 집는것은 처리 필요)

	typedef std::vector<CPoint>::const_iterator vIterPoint;
	std::pair<vIterPoint, vIterPoint> minX = std::minmax_element(vCmdPicker.begin(), vCmdPicker.end(), LessByMinX);
	std::pair<vIterPoint, vIterPoint> minY = std::minmax_element(vCmdPicker.begin(), vCmdPicker.end(), LessByMinY);

	int nDeviceType = eDeviceType::_eDeviceType_Normal;

	switch (nCmdStage)
	{
		//Hand 일 경우
		case eDeviceStage_TEST_PP_1:
		case eDeviceStage_TEST_PP_2:
		{
			for (int i = 0; i < (int)vCmdPicker.size(); i++)
			{
				GetHandDvcData(vCmdPicker[i], nHandType, &stTargetData);
				if (stTargetData.sExist == eDeviceData_Exist && strcmp(stTargetData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0)
					nDeviceType = eDeviceType::_eDeviceType_Cleaning;
			}
		}break;
	default:
		{   //Pocket 일 경우
			for (int i = 0; i < (int)vCmdPicker.size(); i++)
			{
				// change picker position
				int nStartPickerX = vCmdPicker[i].x - minX.first->x;
				int nStartPickerY = vCmdPicker[i].y - minY.first->y;

				if (nX_PitchType == 2 && vCmdPicker[i].x != minX.first->x) {
					nStartPickerX *= nX_PitchType;
				}
				if (nY_PitchType == 2 && vCmdPicker[i].y != minY.first->y) {
					nStartPickerY *= nY_PitchType;
				}

				int nPosX = nTargetX + nStartPickerX;
				int nPosY = nTargetY + nStartPickerY;

				stSourceData.clear();
				GetCmdStageDvcData(nCmdStage, CPoint(nPosX, nPosY), nHandType, &stSourceData);

				// Get Target data ( Target = Hand )
				stTargetData.clear();
				GetHandDvcData(vCmdPicker[i], nHandType, &stTargetData);

				// checking hand picker vacuum sensor 
				int nChkSen = ChkVacuum(DEF_ON, nHandType, vCmdPicker[i]);

				nDeviceType = DeviceType(nCmdStage, stSourceData, stTargetData);
				if (nDeviceType == eDeviceType::_eDeviceType_Cleaning)
					return nDeviceType;
			}
		}break;
	}

	return nDeviceType;
}

// Clean Device를 Pick/Place 하는 상황인지 체크 한다.
// Clean Pick/Place 1
// Normal Pick/Place 0
int CTaskTestPP::DeviceType(int nCmdStage, ST_DD_DEVICE stSourceData, ST_DD_DEVICE stTargetDevice)
{
	int nDeviceType = eDeviceType::_eDeviceType_Normal;
	switch (nCmdStage)
	{
		case LOC_IDX_STATION_1:
		case LOC_IDX_STATION_2:
		case LOC_IDX_STATION_3:
		case LOC_IDX_STATION_4:
		case LOC_IDX_CLEAN_TBL:
		{
			if ((stSourceData.sExist == eDeviceData_Exist && strcmp(stSourceData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0)
				|| (stTargetDevice.sExist == eDeviceData_Exist && strcmp(stTargetDevice.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0))
			{
				nDeviceType = eDeviceType::_eDeviceType_Cleaning;
			}
		}

	}

	return nDeviceType;
}

std::vector<CPoint> CTaskTestPP::AxisDoneFailPickerIdxConverter(std::vector<CPoint> vFailPkr)
{
	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);

	//정방향은 변경할 필요 없음
	if (nBoardType == eBoardDir_Forward) {
		return vFailPkr;
	}
	else { //역방향은 Picker 번호 역순으로 계산
		std::vector<CPoint> vAxisDoneFailPkr;
		vAxisDoneFailPkr.clear();
		int nPickerIdx = 0, nReverseIdx = 0;
		for (auto it = vFailPkr.begin(); it != vFailPkr.end(); it++)
		{
			nPickerIdx = (*it).x + MAX_PICKER_X_CNT * (*it).y;
			nReverseIdx = abs(MAX_PICKER_Z_AXIS_CNT - nPickerIdx - 1);
			vAxisDoneFailPkr.push_back(CPoint(nReverseIdx % MAX_PICKER_X_CNT, nReverseIdx / MAX_PICKER_X_CNT));
		}
		return vAxisDoneFailPkr;
	}
}
