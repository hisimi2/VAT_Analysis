#include "StdAfx.h"
#include "TaskPressUnit.h"
#include "ConfigData.h"
#include "DEF_COMMON.h"
#include "DEF_ERROR_CODE.h"
#include "OneAxis.h"
#include "DEF_DM_11.h"
#include "..\stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

/*=============================================================================
  AUTHOR   : 나명진  [2015-08-27]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CTaskPressUnit::CTaskPressUnit(void)
{
	m_nUnitIdx = 0;
	for(int i=0; i<eMaxAxisCount; i++){
		m_dMotorErrRange[i] = MOTOR_INPOSITION;
	}

	// DO

	m_mapDO.SetAt(yPRESS_BLOW_ON, "BLOW_ON");
	m_mapDO.SetAt(yVIBRATOR_ON_OFF, "VIBRATOR_ON");
	m_mapDO.SetAt(yVIBRATOR_FOR, "VIBRATOR_FOR");
	m_mapDO.SetAt(yVIBRATOR_BACK, "VIBRATOR_BACK");

	// DI
	m_mapDI.SetAt(xZAXIS_SAFETY_SEN, "ZAXIS_SAFETY_SEN");
	m_mapDI.SetAt(xZAXIS_ARRIVED_SAFETY_SEN_1, "ARRIVED_SAFETY_SEN_1");
	m_mapDI.SetAt(xZAXIS_ARRIVED_SAFETY_SEN_2, "ARRIVED_SAFETY_SEN_2");
	m_mapDI.SetAt(xZAXIS_ARRIVED_SAFETY_SEN_3, "ARRIVED_SAFETY_SEN_3");
	m_mapDI.SetAt(xZAXIS_ARRIVED_SAFETY_SEN_4, "ARRIVED_SAFETY_SEN_4");
	m_mapDI.SetAt(xZAXIS_ARRIVED_SAFETY_SEN_5, "ARRIVED_SAFETY_SEN_5");
	m_mapDI.SetAt(xZAXIS_ARRIVED_SAFETY_SEN_6, "ARRIVED_SAFETY_SEN_6");
	m_mapDI.SetAt(xZAXIS_ARRIVED_SAFETY_SEN_7, "ARRIVED_SAFETY_SEN_7");
	m_mapDI.SetAt(xZAXIS_ARRIVED_SAFETY_SEN_8, "ARRIVED_SAFETY_SEN_8");

	m_mapDI.SetAt(xZAXIS_ARRIVED_SAFETY_FIBER_SEN_R1, "ARRIVED_SAFETY_FIBER_SEN_R1");
	m_mapDI.SetAt(xZAXIS_ARRIVED_SAFETY_FIBER_SEN_F1, "ARRIVED_SAFETY_FIBER_SEN_F1");

	// Motor
	m_mapMotor.SetAt(eAxisZ, "Axis Z");

	m_bInitialized = FALSE;
	m_AllStationFailCnt = 0;

	for (int i = 0; i < eMaxTblCount / 2; i++) {
		m_bPressReqState[i] = FALSE;
	}

}



/*=============================================================================
  AUTHOR   : 나명진  [2015-08-27]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CTaskPressUnit::~CTaskPressUnit(void)
{
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-08-27]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskPressUnit::Initialize(int nUnitIdx, char* szDriverFileName)
{
	// 0,1,2,3,4,5,6,7 <- Down
	// 8,9,10,11,12,13,14,15 <- Up
	m_nUnitIdx = nUnitIdx;
	CString strUpDown;
//	if( nUnitIdx < ePressUnit_Up_1){
		strUpDown = _T("DOWN");
// 	}else{
// 		strUpDown = _T("UP");
// 	}

	CConfigData ConfigData(szDriverFileName);
	char szSection[128] = {0,};

	// LOG File Name
	char szLogFileName[ 512 ] = { 0, };
	char szBuff[512] = {0, };
	char szDefaultValue[128] = {0,};

	memset(szSection, 0x00, sizeof(szSection));
	sprintf_s(szSection, sizeof(szSection), "TASK PRESS UNIT %s %d CONFIG", strUpDown.GetString(), (nUnitIdx%8)+1);

	memset(szDefaultValue, 0x00, sizeof(szDefaultValue));
	sprintf_s(szDefaultValue, sizeof(szDefaultValue), "TaskPressUnit_%s_%d.log", strUpDown.GetString(), (nUnitIdx%8)+1);

	ConfigData.GetString( szSection, "Log File Name", szDefaultValue, szBuff);
	sprintf_s(szLogFileName, sizeof(szLogFileName), "%s%s", SZ_LOG_FILE_PATH, szBuff);
	m_Logger.SetFileName(szLogFileName);


	// Digital Input
	memset(szSection, 0x00, sizeof(szSection));
	sprintf_s(szSection, sizeof(szSection), "TASK PRESS UNIT %s %d CONFIG - DI", strUpDown.GetString(), (nUnitIdx%8)+1);
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
	sprintf_s(szSection, sizeof(szSection), "TASK PRESS UNIT %s %d CONFIG - DO",  strUpDown.GetString(), (nUnitIdx%8)+1);
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
	sprintf_s(szSection, sizeof(szSection), "TASK PRESS UNIT %s %d CONFIG - MOTOR",  strUpDown.GetString(), (nUnitIdx%8)+1);
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

	return CBaseTaskManager::Initialize();
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-08-27]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CTaskPressUnit::Finalize()
{
	CBaseTaskManager::Finalize();
}

//==============================================================================
// 도착한 명령을 큐에서 꺼내어 실행 시킨다.
//==============================================================================
void CTaskPressUnit::ThreadProc1()
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

void CTaskPressUnit::OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
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
  AUTHOR   : 나명진  [2015-08-27]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskPressUnit::OnEvent(const CBaseTaskManager* pSender,int nEventId, CParamList* pDataList/*=NULL*/)
{
	return 0;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-08-27]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
OneAxis* CTaskPressUnit::GetAxis(int nAxisIdx)
{
	ASSERT( (nAxisIdx >=0 ) && (nAxisIdx < CTaskPressUnit::eMaxAxisCount) );

	return m_vMotor[nAxisIdx];
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-08-27]
  NAME     : 
  PARAMS   : nLocStage   = Loc Stage //enum eLocIdxPressUnit{
             nSeqSpdRate = Setting 속도에서, Seq에서 쓰려는 속도 %를 재 설정 함.
			 dPosOffset  = Target 위치 대비하여 이동 위치의 Offset 조정 [ um ]
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskPressUnit::do_MovePos(int nLocStage, int nSeqSpdRate/*=100*/,  double dPosOffset/*=0.0*/)
{
	ASSERT(nLocStage >= LOC_IDX_PRESS_UNIT_SAFETY && nLocStage < MAX_PRESS_UNIT_LOC_COUNT);
	ASSERT( nSeqSpdRate >=0 && nSeqSpdRate <= 100);

	double dTPos = 0.0;
	GetTeachPos(nLocStage, &dTPos);
	int nErr = Interlock_MovePos(nLocStage);
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}

	// safety 위치보다 작을 수는 없다.
	double dTPosSafety = 0.0;
	GetTeachPos(LOC_IDX_PRESS_UNIT_SAFETY, &dTPosSafety);
	dTPos += dPosOffset;

	if( dTPos < dTPosSafety )
		dTPos = dTPosSafety;

	OneAxis* pMotor = (OneAxis*)m_vMotor[CTaskPressUnit::eAxisZ];
	/*User Spd, Acc, Dcc & Seq Spd, Acc, Dcc 를 가지고 계산*/
	int nIndexSpd[eMaxPressUnitCount] = { NDM11_SPD_PressUnit1, NDM11_SPD_PressUnit2, NDM11_SPD_PressUnit3, NDM11_SPD_PressUnit4,
										  NDM11_SPD_PressUnit5, NDM11_SPD_PressUnit6, NDM11_SPD_PressUnit7, NDM11_SPD_PressUnit8 };

	int nIndexAcc[eMaxPressUnitCount] = { NDM11_ACC_PressUnit1, NDM11_ACC_PressUnit2, NDM11_ACC_PressUnit3, NDM11_ACC_PressUnit4,
								   		  NDM11_ACC_PressUnit5, NDM11_ACC_PressUnit6, NDM11_ACC_PressUnit7, NDM11_ACC_PressUnit8 };


	int nIndexDcc[eMaxPressUnitCount] = { NDM11_DCC_PressUnit1, NDM11_DCC_PressUnit2, NDM11_DCC_PressUnit3, NDM11_DCC_PressUnit4,
										  NDM11_DCC_PressUnit5, NDM11_DCC_PressUnit6, NDM11_DCC_PressUnit7, NDM11_DCC_PressUnit8 };

	double dRateSpd = (double)g_DMCont.m_dmHandlerOpt.GN(nIndexSpd[m_nUnitIdx]);
	int    nRateAcc = g_DMCont.m_dmHandlerOpt.GN(nIndexAcc[m_nUnitIdx]);
	int    nRateDcc = g_DMCont.m_dmHandlerOpt.GN(nIndexDcc[m_nUnitIdx]);

	double dMtSpd = pMotor->GetSpeed();
	double dMtAcc = pMotor->GetAcc();
	double dMtDcc = pMotor->GetDcc();

	double dRetSpd=0;
	int    nRetAcc=0, nRetDcc=0;

	dRetSpd = (dMtSpd * (dRateSpd/100.0)) * ((double)nSeqSpdRate/100.0); // um -> mm로 환산
	nRetAcc = (int)(dMtAcc - (dMtAcc*0.75)*((double)nRateAcc/100.0)); // 설정되어 있는 값에서 spd rate가 높을 수록 acc/dcc를 감소 시켜준다. 가장 빠를 때가 설정 속도의 95%뺀 acc/dcc로 함.
	nRetDcc = (int)(dMtDcc - (dMtDcc*0.75)*((double)nRateDcc/100.0));

	if(nRetAcc < 200) nRetAcc = 200;
	if(nRetDcc < 200) nRetDcc = 200;

	nErr = pMotor->Move(dTPos, dRetSpd, nRetAcc, nRetDcc);

	return nErr;
}


// int CTaskPressUnit::do_MoveTargetPos(double dTargetPos, int nSpdRate/* = 100*/, double dPosOffset/* = 0.0*/)
// {
// 	ASSERT(nSpdRate >= 0 && nSpdRate <= 100);
// 
// 
// 	// safety 위치보다 작을 수는 없다.
// 	double dTPosSafety = 0.0;
// 	GetTeachPos(LOC_IDX_PRESS_UNIT_SAFETY, &dTPosSafety);
// 	dTPos += dPosOffset;
// 
// 	if (dTPos < dTPosSafety)
// 		dTPos = dTPosSafety;
// 
// 	OneAxis* pMotor = (OneAxis*)m_vMotor[CTaskPressUnit::eAxisZ];
// 	/*User Spd, Acc, Dcc & Seq Spd, Acc, Dcc 를 가지고 계산*/
// 	int nIndexSpd[eMaxPressUnitCount] = { NDM11_SPD_PressUnit1, NDM11_SPD_PressUnit2, NDM11_SPD_PressUnit3, NDM11_SPD_PressUnit4,
// 		NDM11_SPD_PressUnit5, NDM11_SPD_PressUnit6, NDM11_SPD_PressUnit7, NDM11_SPD_PressUnit8 };
// 
// 	int nIndexAcc[eMaxPressUnitCount] = { NDM11_ACC_PressUnit1, NDM11_ACC_PressUnit2, NDM11_ACC_PressUnit3, NDM11_ACC_PressUnit4,
// 		NDM11_ACC_PressUnit5, NDM11_ACC_PressUnit6, NDM11_ACC_PressUnit7, NDM11_ACC_PressUnit8 };
// 
// 
// 	int nIndexDcc[eMaxPressUnitCount] = { NDM11_DCC_PressUnit1, NDM11_DCC_PressUnit2, NDM11_DCC_PressUnit3, NDM11_DCC_PressUnit4,
// 		NDM11_DCC_PressUnit5, NDM11_DCC_PressUnit6, NDM11_DCC_PressUnit7, NDM11_DCC_PressUnit8 };
// 
// 	double dRateSpd = (double)g_DMCont.m_dmHandlerOpt.GN(nIndexSpd[m_nUnitIdx]);
// 	int    nRateAcc = g_DMCont.m_dmHandlerOpt.GN(nIndexAcc[m_nUnitIdx]);
// 	int    nRateDcc = g_DMCont.m_dmHandlerOpt.GN(nIndexDcc[m_nUnitIdx]);
// 
// 	double dMtSpd = pMotor->GetSpeed();
// 	double dMtAcc = pMotor->GetAcc();
// 	double dMtDcc = pMotor->GetDcc();
// 
// 	double dRetSpd = 0;
// 	int    nRetAcc = 0, nRetDcc = 0;
// 
// 	dRetSpd = (dMtSpd * (dRateSpd / 100.0)) * ((double)nSeqSpdRate / 100.0); // um -> mm로 환산
// 	nRetAcc = (int)(dMtAcc - (dMtAcc*0.75)*((double)nRateAcc / 100.0)); // 설정되어 있는 값에서 spd rate가 높을 수록 acc/dcc를 감소 시켜준다. 가장 빠를 때가 설정 속도의 95%뺀 acc/dcc로 함.
// 	nRetDcc = (int)(dMtDcc - (dMtDcc*0.75)*((double)nRateDcc / 100.0));
// 
// 	if (nRetAcc < 200) nRetAcc = 200;
// 	if (nRetDcc < 200) nRetDcc = 200;
// 
// 	nErr = pMotor->Move(dTPos, dRetSpd, nRetAcc, nRetDcc);
// 
// 	return nErr;
// 
// }

/*=============================================================================
  AUTHOR   : 나명진  [2015-08-27]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskPressUnit::Interlock_MovePos(int nLoc)
{
/* [작업 필요] test pp Teach Pos 확인 후, 작업
	if(LOC_IDX_PRESS_UNIT_CONTACT != nLoc) return ERR_NO_ERROR;

	std::vector<int> vLocList;
	
	int nTestPpIdx  = -1, nPressAreaIdx = -1;
	switch(m_nUnitIdx)
	{
	case ePressUnit_Down_1:	case ePressUnit_Down_2://	case ePressUnit_Up_1: case ePressUnit_Up_2:
	case ePressUnit_Down_3:	case ePressUnit_Down_4://	case ePressUnit_Up_3: case ePressUnit_Up_4:
		{
			nTestPpIdx = eTestPP_1;
		}break;
	case ePressUnit_Down_5:	case ePressUnit_Down_6://	case ePressUnit_Up_5: case ePressUnit_Up_6:
	case ePressUnit_Down_7:	case ePressUnit_Down_8://	case ePressUnit_Up_7:	case ePressUnit_Up_8:
		{
			nTestPpIdx = eTestPP_2;
		}break;
	}

	switch(m_nUnitIdx)
	{
		case ePressUnit_Down_1:	case ePressUnit_Down_5:	{ nPressAreaIdx = eATS_DN_STATION_L1; } break;
		case ePressUnit_Down_2: case ePressUnit_Down_6:	{ nPressAreaIdx = eATS_DN_STATION_L2; } break;
		case ePressUnit_Down_3: case ePressUnit_Down_7:	{ nPressAreaIdx = eATS_DN_STATION_L3; } break;
		case ePressUnit_Down_4: case ePressUnit_Down_8:	{ nPressAreaIdx = eATS_DN_STATION_L4; } break;
	}
	

	//TODO KSJ :: Y축 Sensor 점검하여 Press 이동을 할 수 있다.
	// Y가 Test Site 부에 있을 경우는 X, Z를 통해서 위치 파악을 하여야한다.
	vLocList.clear();
	int nSafetyX1Sen = g_TaskTestPP[nTestPpIdx].ChkDI(CTaskTestPP::xX1AXIS_POSITION_SEN, DEF_ON);
	int nSafetyX2Sen = g_TaskTestPP[nTestPpIdx].ChkDI(CTaskTestPP::xX2AXIS_POSITION_SEN, DEF_ON);

	g_TaskTestPP[nTestPpIdx].WhereIsMotorPos(vLocList,CTaskTestPP::eAxisAll);
	if( (vLocList[CTaskTestPP::eAxisX1] < CTaskTestPP::ePosX_TableToNegaLimit) || nSafetyX1Sen != ERR_NO_ERROR &&
	(vLocList[CTaskTestPP::eAxisX2] < CTaskTestPP::ePosX_TableToNegaLimit) || nSafetyX1Sen != ERR_NO_ERROR)
	{
		switch(nPressAreaIdx) // X의 위치를 확인하여야 한다.
		{
		case eATS_DN_STATION_L1:
		case eATS_DN_STATION_L2:
		case eATS_DN_STATION_L3:
		case eATS_DN_STATION_L4:
			{
				if(vLocList[CTaskTestPP::eAxisX1] == (CTaskTestPP::ePosY_StationL1 +(nPressAreaIdx-eATS_DN_STATION_L1))
					&& vLocList[CTaskTestPP::eAxisZ1] >= CTaskTestPP::ePosZ_DownSafety)
						return ERR_TEST_PP_1_POSITION_NOT_SAFETY + nTestPpIdx;
			}break;
		}
	}
*/
	return ERR_NO_ERROR;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-08-27]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskPressUnit::GetTeachPos(int nLocStage, double* pdPosZ)
{
	double dTPos = 0.0;
	dTPos = g_DMCont.m_Teach.GetTeachPos(RID_PRESS_UNIT_(m_nUnitIdx), nLocStage, POS_IDX_PRESS_UNIT_Z);

	*pdPosZ = dTPos;

	return ERR_NO_ERROR;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-08-27]
  NAME     : 
  PARAMS   : eTeachPos : 
  eAxis     : -1 = 모든 축의 위치가 맞는지 점검 한다.
  RETURN   : 0 = OK
            -1 = Motion 동작 중 임.
            -2 = 동작은 멈췄으나 Position Range 안에 들어오지 않음
  COMMENTS : Motor의 현재 위치가 지정한 위치에 있는지 점검 한다. 
==============================================================================*/
int CTaskPressUnit::ChkMotorPos(int nLoc, int nAxisIdx/*=eAxisAll*/)
{
	ASSERT(nLoc >= LOC_IDX_PRESS_UNIT_SAFETY && nLoc < MAX_PRESS_UNIT_LOC_COUNT);
	ASSERT( nAxisIdx  >= CTaskPressUnit::eAxisAll && nAxisIdx  < CTaskPressUnit::eMaxAxisCount);

	double dTeachPos[CTaskPressUnit::eMaxAxisCount] = {0.0, };
	double dCurPos  [CTaskPressUnit::eMaxAxisCount] = {0.0, };

	for(int i=0; i<CTaskPressUnit::eMaxAxisCount; i++)
	{
		OneAxis* pMotor = (OneAxis*)m_vMotor[i];
		pMotor->GetCurrentPos(dCurPos[i]);
		GetTeachPos(nLoc, &dTeachPos[i]);
	}
	
	int nAxisCount = m_vMotor.size();

	if(nAxisIdx == CTaskPressUnit::eAxisAll){
		for(int i=0; i<nAxisCount; i++){
			OneAxis* pMotor = (OneAxis*)m_vMotor[i];
			if( pMotor->MotionDone() != ERR_NO_ERROR){
				return -1;
			}

			if( fabs(dCurPos[i] - dTeachPos[i]) > m_dMotorErrRange[i] ){
				return -2;			
			}
		}
	}else{
		OneAxis* pMotor = (OneAxis*)m_vMotor[nAxisIdx];
		if( pMotor->MotionDone() != ERR_NO_ERROR){
			return -1;
		}

		if( fabs(dCurPos[nAxisIdx] - dTeachPos[nAxisIdx]) > m_dMotorErrRange[nAxisIdx] ){
			return -2;			
		}
	}

	return ERR_NO_ERROR;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-08-27]
  NAME     : 
  PARAMS   : 
  RETURN   : 0 = OK
            -1 = Motion 동작 중 임.
            -2 = 동작은 멈췄으나 Position Range 안에 들어오지 않음.
  COMMENTS : 각 축의 위치가 지정한 위치 + offset 에 있는지 점검 한다.
==============================================================================*/
int CTaskPressUnit::ChkMotorPos(int nLoc, double dOffset, int nAxisIdx/*=eAxisAll*/)
{
	ASSERT(nLoc >= LOC_IDX_PRESS_UNIT_SAFETY && nLoc < MAX_PRESS_UNIT_LOC_COUNT);
	ASSERT( nAxisIdx  >= 0 && nAxisIdx  < CTaskPressUnit::eMaxAxisCount);

	double dTeachPos[CTaskPressUnit::eMaxAxisCount] = {0.0, };
	double dCurPos  [CTaskPressUnit::eMaxAxisCount] = {0.0, };

	int nAxisCount = m_vMotor.size();

	for(int i=0; i<nAxisCount; i++)
	{
		OneAxis* pMotor = (OneAxis*)m_vMotor[i];
		pMotor->GetCurrentPos(dCurPos[i]);
		GetTeachPos(nLoc, &dTeachPos[i]);
	}

	if(nAxisIdx == CTaskPressUnit::eAxisAll){
		for(int i=0; i<nAxisCount; i++){
			OneAxis* pMotor = (OneAxis*)m_vMotor[i];
			if( pMotor->MotionDone() != ERR_NO_ERROR){
				return -1;
			}

			if( fabs(dCurPos[i] - (dTeachPos[i] + dOffset)) > m_dMotorErrRange[i] ){
				return -2;			
			}
		}
	}else{
		OneAxis* pMotor = (OneAxis*)m_vMotor[nAxisIdx];
		if( pMotor->MotionDone() != ERR_NO_ERROR){
			return -1;
		}

		if( fabs(dCurPos[nAxisIdx] - (dTeachPos[nAxisIdx] + dOffset) ) > m_dMotorErrRange[nAxisIdx] ){
			return -2;			
		}
	}

	return ERR_NO_ERROR;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-08-27]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTaskPressUnit::WhereIsMotorPos(std::vector<int>& vLocList, int nAxisIdx/*=eAxisAll*/)
{
	ASSERT(nAxisIdx >= eAxisAll && nAxisIdx < eMaxAxisCount);

	double dCurrentPos = 0.0;
	double dTPos[MAX_PRESS_UNIT_LOC_COUNT] = {0.0,};

	// Get Current Pos & Teaching Pos
	OneAxis* pMotor = (OneAxis*)m_vMotor[eAxisZ];
	pMotor->GetCurrentPos(dCurrentPos);

	GetTeachPos(LOC_IDX_PRESS_UNIT_SAFETY , &dTPos[LOC_IDX_PRESS_UNIT_SAFETY]);
	GetTeachPos(LOC_IDX_PRESS_UNIT_CONTACT, &dTPos[LOC_IDX_PRESS_UNIT_CONTACT]);

	if( fabs(dCurrentPos - dTPos[LOC_IDX_PRESS_UNIT_SAFETY]) <= MOTOR_INPOSITION){
		vLocList.push_back(eMotPos_Safety);                  // Safety pos
	}else if(fabs(dCurrentPos - dTPos[LOC_IDX_PRESS_UNIT_CONTACT]) <= MOTOR_INPOSITION){
		vLocList.push_back(eMotPos_Contact);                 // Contact pos
	}else if(dCurrentPos < (dTPos[LOC_IDX_PRESS_UNIT_SAFETY]-MOTOR_INPOSITION)){
		vLocList.push_back(eMotPos_Between_NegaToSafety);    // Between safety & nega
	}else if(dCurrentPos > (dTPos[LOC_IDX_PRESS_UNIT_CONTACT]+MOTOR_INPOSITION)){
		vLocList.push_back(eMotPos_Between_ContactToPosi);   // Between Contact & Posi
	}else{
		vLocList.push_back(eMotPos_Between_SafetyToContact); // Between Contact & safety
	}
	
	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : Check Motor Initialize status
  NAME     : 김선진
  PARAMS   : None
  RETURN   : TRUE  : Need Initial,  FALSE : No need initialzie
  COMMENTS : Initialize 작업이 필요한 Motor 가 존재 하는지 확인.
==============================================================================*/
BOOL CTaskPressUnit::IsInitNeed()
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
  PARAMS   : 
  RETURN   : 
  COMMENTS :  Test Site의 Device의 Test 진행 상태에 대한 정보 // enum eTestSiteInfo
==============================================================================*/
int  CTaskPressUnit::GetSiteDvcTestingInfo(int nX, int nY)
{
	//*nStatus = g_DMCont.m_dmDevice.GetSiteDvcTestingInfo((eDeviceStage_TEST_SITE_1+m_nUnitIdx), nX, nY);
	return g_DMCont.m_dmDevice.GetSiteDvcTestingInfo((eDeviceStage_TEST_SITE_1 + m_nUnitIdx), nX, nY);//g_DMCont.m_dmContac_Up.GN(NDM2_StationStatus_1+m_nUnitIdx);
}
/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : Test Site의 Device의 Test 진행 상태에 대한 정보 // enum eTestSiteInfo
==============================================================================*/
int  CTaskPressUnit::SetSiteDvcTestingInfo(int nX, int nY, int nStatus)
{
	g_DMCont.m_dmDevice.SetSiteDvcTestingInfo((eDeviceStage_TEST_SITE_1 + m_nUnitIdx), nStatus, nX, nY);

	////g_DMCont.m_dmDevice.SetSiteDvcTestingInfo((eDeviceStage_TEST_SITE_1+m_nUnitIdx), nStatus, nX, nY);
	//int nCnt = 0;
	//int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	//int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	//ST_DD_DEVICE stDvc;
	//for( int nY=0; nY < nSiteDivY; nY++ )
	//{
	//	for( int nX=0; nX < nSiteDivX; nX++ )
	//	{
	//		int nExist = g_DMCont.m_dmDevice.GetDeviceData( eDeviceStage_TEST_SITE_1+m_nUnitIdx, nX, nY, &stDvc);
	//		if (stDvc.sExist == eDeviceData_Exist)
	//		{
	//			if( nStatus == eTestSite_CleanLoading || nStatus == eTestSite_CleanEnd )
	//			{ 
	//				if( strcmp(stDvc.szErrString, DEF_SOCKET_CLEAN_DEVICE) == 0)
	//				{
	//					
	//				}
	//			}
	//			else {
	//				g_DMCont.m_dmDevice.SetSiteDvcTestingInfo( (eDeviceStage_TEST_SITE_1 + m_nUnitIdx), nStatus, nX, nY );
	//			}
	//			nCnt++;
	//		}else
	//			g_DMCont.m_dmDevice.SetSiteDvcTestingInfo((eDeviceStage_TEST_SITE_1 + m_nUnitIdx), eTestSite_Empty, nX, nY);
	//	}
	//}

	//if (nCnt > 0)
	//{
	//	g_DMCont.m_dmContac_Up.SN(NDM2_StationStatus_1 + m_nUnitIdx, nStatus);
	//}
	//else {
	//	g_DMCont.m_dmContac_Up.SN(NDM2_StationStatus_1 + m_nUnitIdx, eTestSite_Empty);
	//}
	
	return ERR_NO_ERROR;
}

int CTaskPressUnit::GetStationInfo(BOOL bGrr,BOOL bTesting)
{
	int nSiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nInformation[eMaxTestSiteTestCount] = { 0, };

	ST_DD_DEVICE tSiteData;
	for (int nY = 0; nY < nSiteCntY; nY++)
	{
		for (int nX = 0; nX < nSiteCntX; nX++)
		{
			tSiteData.clear();
			GetSiteDeviceData(&tSiteData, nX, nY);
			int nOnOff = (bGrr == FALSE ? GetSocketOnOff(nX, nY) : GetSocketOnOffGrr(nX, nY) );

			if (tSiteData.sExist == eDeviceData_Exist/* && nOnOff == DEF_SITE_ON*/)
			{
				nInformation[tSiteData.nTestingInfo]++;
			}
			if (tSiteData.sExist == eDeviceData_None)
				nInformation[eTestSite_Empty]++;
		}
	}

	if ( nInformation[eTestSite_Empty] == nSiteCntX*nSiteCntY )
		return eTestSite_Empty;
	if( (nInformation[eTestSite_TestEnd] + nInformation[eTestSite_Empty]) == nSiteCntX*nSiteCntY )
		return eTestSite_TestEnd;
	if( (nInformation[eTestSite_CleanEnd] + nInformation[eTestSite_Empty]) == nSiteCntX*nSiteCntY )
		return eTestSite_CleanEnd;
	if( (nInformation[eTestSite_DvcLoading] + nInformation[eTestSite_Empty]) == nSiteCntX*nSiteCntY )
		return eTestSite_DvcLoading;
	if ( (nInformation[eTestSite_CleanLoading] + nInformation[eTestSite_Empty]) == nSiteCntX*nSiteCntY )
		return eTestSite_CleanLoading;
	if ( (nInformation[eTestSite_Contacting] + nInformation[eTestSite_Empty]) == nSiteCntX*nSiteCntY)
		return eTestSite_Contacting;
	if ((nInformation[eTestSite_TestTimeOut] + nInformation[eTestSite_Empty]) == nSiteCntX*nSiteCntY)
		return eTestSite_TestTimeOut;
	if ((nInformation[eTestSite_ContactStart] + nInformation[eTestSite_Empty]) == nSiteCntX*nSiteCntY)
		return eTestSite_ContactStart;

	if (bTesting == TRUE)
	{
		if ((nInformation[eTestSite_TestStart] + nInformation[eTestSite_Empty]) == nSiteCntX*nSiteCntY)
			return eTestSite_TestStart;
		if ((nInformation[eTestSite_Testing] + nInformation[eTestSite_Empty]) == nSiteCntX*nSiteCntY)
			return eTestSite_Testing;
	}

	return -1;
}

int CTaskPressUnit::GetAuditStationInfo()
{
	int nSiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nInformation[eMaxTestSiteTestCount] = { 0, };

	ST_DD_DEVICE tSiteData;
	for (int nY = 0; nY < nSiteCntY; nY++)
	{
		for (int nX = 0; nX < nSiteCntX; nX++)
		{
			tSiteData.clear();
			GetSiteDeviceData(&tSiteData, nX, nY);
			int nOnOff = GetSocketOnOffAudit(nX, nY);

			if (tSiteData.sExist == eDeviceData_Exist/* && nOnOff == DEF_SITE_ON*/)
			{
				nInformation[tSiteData.nTestingInfo]++;
			}
			if (tSiteData.sExist == eDeviceData_None)
				nInformation[eTestSite_Empty]++;
		}
	}

	if (nInformation[eTestSite_Empty] == nSiteCntX*nSiteCntY)
		return eTestSite_Empty;
	if ((nInformation[eTestSite_TestEnd] + nInformation[eTestSite_Empty]) == nSiteCntX*nSiteCntY)
		return eTestSite_TestEnd;
	if ((nInformation[eTestSite_CleanEnd] + nInformation[eTestSite_Empty]) == nSiteCntX*nSiteCntY)
		return eTestSite_CleanEnd;
	if ((nInformation[eTestSite_DvcLoading] + nInformation[eTestSite_Empty]) == nSiteCntX*nSiteCntY)
		return eTestSite_DvcLoading;
	if ((nInformation[eTestSite_CleanLoading] + nInformation[eTestSite_Empty]) == nSiteCntX*nSiteCntY)
		return eTestSite_CleanLoading;
	if ((nInformation[eTestSite_Contacting] + nInformation[eTestSite_Empty]) == nSiteCntX*nSiteCntY)
		return eTestSite_Contacting;
	if ((nInformation[eTestSite_TestTimeOut] + nInformation[eTestSite_Empty]) == nSiteCntX*nSiteCntY)
		return eTestSite_TestTimeOut;

	return -1;
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : //enum eATS_SiteLoc
  PARAMS   : SitePos => Down Left, Down Right, Up Left, Up Right
  RETURN   : pnStageIdx = StageIdx
  COMMENTS : Area당 4개의 TestSite 의 Number를 Stage Index로 변환
==============================================================================*/
int CTaskPressUnit::ConvertStageIdx(int nAutoAreaIdx, int* pnStageIdx)
{
	ASSERT( nAutoAreaIdx >= eATS_DN_STATION_L1 && nAutoAreaIdx <= eATS_DN_STATION_L4);

	*pnStageIdx = (eDeviceStage_TEST_SITE_1+m_nUnitIdx);

	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-30]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 센서 상태를 반환 한다. 단순 조회만 한다면 이 함수를 사용 할 것.            
==============================================================================*/
BOOL CTaskPressUnit::GetDI(int nIndex)
{
	ASSERT( nIndex < xMAX_COUNT );
	return (BOOL)g_IO.in(m_vX[nIndex]);
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
int CTaskPressUnit::ChkDI(int nIndex, BOOL bValue/*=TRUE*/)
{
	ASSERT( nIndex < xMAX_COUNT );

	if( g_IO.GetDriverType() == _DIO_TYPE_SIMUL_ ){
		g_IO.SetSimulDi(m_vX[nIndex], bValue);
		Sleep(SIMUL_SLEEP_TIME);
	}

// 	if (nIndex == xZAXIS_VIBRATION_CYL_UP_SEN || nIndex == xZAXIS_VIBRATION_CYL_DOWN_SEN) {
// 		if (g_IO.GetDriverType() != _DIO_TYPE_SIMUL_) {
// 			if ((g_IO.out(m_vY[yPRESS_UNIT_VIBRATION_CYL_UP]) == DEF_ON && g_IO.out(m_vY[yPRESS_UNIT_VIBRATION_CYL_DOWN]) == DEF_ON)
// 				|| (g_IO.in(m_vX[xZAXIS_VIBRATION_CYL_UP_SEN]) == DEF_ON && g_IO.in(m_vX[xZAXIS_VIBRATION_CYL_DOWN_SEN]) == DEF_ON))
// 			{
// 				CParamList AlarmData;
// 				AlarmData.m_fnAddItemFormat("Station %d번 Vibration  CYL UP,DOWN detected simultously", m_nUnitIdx + 1);
// 				g_Error.AlarmReport(ERR_PRESS_UNIT_1_VIBRATOR_CYLINDER_UP_DOWN_SIMULTOUSLY + m_nUnitIdx, AlarmData, NULL);
// 				return -2;
// 			}
// 		}
// 		
// 	}

	if(g_IO.in(m_vX[nIndex]) != bValue ){
		return -1;
	}


	return ERR_NO_ERROR;
}

int  CTaskPressUnit::SetSiteDeviceData(ST_DD_DEVICE* lpstBuff, int nX, int nY)
{
	g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_SITE_1+m_nUnitIdx, nX, nY, lpstBuff);

	return ERR_NO_ERROR;
}

int CTaskPressUnit::GetSiteDeviceData(ST_DD_DEVICE* lpstBuff, int nX, int nY)
{
	g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1+m_nUnitIdx, nX, nY, lpstBuff);
	return ERR_NO_ERROR;
}


void CTaskPressUnit::MakeLog(LPCTSTR fmt, ...)
{
    TCHAR tmpMsg[1024] = {0,};
    if(fmt)
    {
        va_list argList;
        va_start(argList, fmt);
        _vstprintf_s(tmpMsg, fmt, argList);
        va_end(argList);
    }
    TWLOG.MakeLog(static_cast<int>(eLogID_PressUnit_1+m_nUnitIdx), Debug, "", __LINE__, NULL, tmpMsg);
}

int CTaskPressUnit::Interlock_VacBlow(int nVacBlow)
{
	return ERR_NO_ERROR;
}

int CTaskPressUnit::do_VacBlow(int nVacBlow)
{
	int nErr = Interlock_VacBlow(nVacBlow);
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}

	if( nVacBlow == eVacBlwSt_Off ){
		g_IO.out(m_vY[yPRESS_BLOW_ON], DEF_OFF);
	}else if( nVacBlow == eVacBlwSt_BlwOn ){
		g_IO.out( m_vY[yPRESS_BLOW_ON], DEF_ON);
	}

	return ERR_NO_ERROR;
}

int CTaskPressUnit::do_SideVibratorOnOff(int nOnOff)
{
	int nPressVibratorNew = g_DMCont.m_dmEQP.GN(NDM0_PRESS_VIBRATOR_NEW);
	//단동은 Vibrator압이 약함.
	if (nPressVibratorNew == 0) {
		g_IO.out(m_vY[yVIBRATOR_ON_OFF], nOnOff);
	}
	else { //복동 변경.
		int nForOper = (nOnOff == DEF_ON) ? eOn : eOff;
		int nBackOper = (nOnOff == DEF_ON) ? eOff : eOn;

		g_IO.out(m_vY[yVIBRATOR_FOR], nForOper);
		g_IO.out(m_vY[yVIBRATOR_BACK], nBackOper);
	}

	if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
		Sleep(SIMUL_SLEEP_TIME);
	}

	return ERR_NO_ERROR;
}


int CTaskPressUnit::GetSocketOnCnt(BOOL bGrrMode)
{
	int nOnCnt = 0;

	int nSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	for(int nX=0; nX < nSiteX; nX++)
	{
		for(int nY=0; nY < nSiteY; nY++)
		{
			int nTarget = (bGrrMode == TRUE ? NDM11_GRR_SocketOnOff_Site1_1 : NDM11_SocketOnOff_Site1_1 )+nX+nY*STATION_MAX_X_SITE +(m_nUnitIdx *STATION_MAX_PARA);
			if (g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_ON || g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_OFF_RESERVATION || g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_OFF_RESERVATION_YIELD)
				nOnCnt++;
		}
	}
	
	return nOnCnt;
}

int CTaskPressUnit::GetSocketOnCntGrr()
{
	int nOnCnt = 0;

	int nSiteX = g_DMCont.m_dmTestPP.GN( NDM4_TestSite_Div_X );
	int nSiteY = g_DMCont.m_dmTestPP.GN( NDM4_TestSite_Div_Y );

	for( int nX = 0; nX < nSiteX; nX++ )
	{
		for( int nY = 0; nY < nSiteY; nY++ )
		{
			int nTarget = NDM11_GRR_SocketOnOff_Site1_1 + nX + nY*STATION_MAX_X_SITE + ( m_nUnitIdx *STATION_MAX_PARA );
			if (g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_ON)
				nOnCnt++;
		}
	}

	return nOnCnt;
}

int CTaskPressUnit::GetSocketAuditOnCnt()
{
	int nOnCnt = 0;

	int nSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	for (int nX = 0; nX < nSiteX; nX++)
	{
		for (int nY = 0; nY < nSiteY; nY++)
		{
			int nTarget = NDM11_Audit_SocketOnOff_Site1_1  + nX + nY*STATION_MAX_X_SITE + (m_nUnitIdx *STATION_MAX_PARA);
			if (g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_ON || g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_OFF_RESERVATION || g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_OFF_RESERVATION_YIELD)
				nOnCnt++;
		}
	}

	return nOnCnt;
}
int CTaskPressUnit::GetSocketAuditOnOffCnt(BOOL bReserve)
{
	int nOnCnt = 0;

	int nSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	for (int nX = 0; nX < nSiteX; nX++)
	{
		for (int nY = 0; nY < nSiteY; nY++)
		{
			int nTarget = NDM11_Audit_SocketOnOff_Site1_1 + nX + nY*STATION_MAX_X_SITE + (m_nUnitIdx *STATION_MAX_PARA);
			if (bReserve == TRUE)
			{
				if (g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_ON || g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_OFF_RESERVATION || g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_OFF_RESERVATION_YIELD)
					nOnCnt++;
			}
			else {
				if (g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_ON)
					nOnCnt++;
			}
			
		}
	}

	return nOnCnt;
}


int CTaskPressUnit::ChkSocketArrivedSen_All( CString &strErrMsg, std::vector<CPoint> &vErrPocket, std::vector<CString> &vErrHistor, int &nDeviceType, BOOL isSafety)
{
	BOOL bErrArrivedSen = FALSE, bChkSite = FALSE;
	int nOpt_ArriveDvcSen = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRS_DEVICE_ARRIVE_OPTION);
	CString strTmp =_T("");
	strErrMsg.Empty();

	int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	BOOL bAUtoGrrUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);
	BOOL bAUtoAuditUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);
	int nSiteAddr = NDM11_SocketOnOff_Site1_1;
	if (bAUtoGrrUse) {
		nSiteAddr = NDM11_GRR_SocketOnOff_Site1_1;
	}
	else if (bAUtoAuditUse) {
		nSiteAddr = NDM11_Audit_SocketOnOff_Site1_1;
	}

	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	ST_DD_DEVICE stDvc;

	for(int nY=0; nY < nSiteDivY; nY++ )
	{
		for(int nX=0; nX < nSiteDivX; nX++ )
		{
			bChkSite = FALSE;
			if( nOpt_ArriveDvcSen == CTaskPressUnit::eOnly_On_Site )
			{
				int nAdrr = nX + nY*STATION_MAX_X_SITE + m_nUnitIdx*STATION_MAX_PARA;
				if( g_DMCont.m_dmHandlerOpt.GN(nSiteAddr + nAdrr ) == DEF_SITE_ON || g_DMCont.m_dmHandlerOpt.GN(nSiteAddr + nAdrr) == DEF_SITE_OFF_RESERVATION || g_DMCont.m_dmHandlerOpt.GN(nSiteAddr + nAdrr) == DEF_SITE_OFF_RESERVATION_YIELD)
					bChkSite = TRUE;
			}else{	//CTaskPressUnit::eAllSite
				bChkSite = TRUE;
			}


			if(bChkSite == TRUE)
			{
				int nErr =0;
				
				int nPressSensorType = g_DMCont.m_dmEQP.GN(NDM0_PRESS_ARRIVED_SENSOR_TYPE);
				if (nPressSensorType == eArriveSensorType::eEach){
					nErr = ChkDI(CTaskPressUnit::xZAXIS_ARRIVED_SAFETY_SEN_1 + nX + nY*STATION_MAX_X_SITE, DEF_ON);
				}
				else{
					nErr = ChkDI(CTaskPressUnit::xZAXIS_ARRIVED_SAFETY_FIBER_SEN_R1 + nY, DEF_ON);
				}


				if (nErr != ERR_NO_ERROR)
				{
					bErrArrivedSen = TRUE;
					stDvc.clear();
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &stDvc);
					if (strcmp(stDvc.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0) {
						strTmp.Format("[%d (Clean Device)]", nX + 1 + (nY*nSiteDivX));
						nDeviceType = eDeviceType::_eDeviceType_Cleaning;
					}
					else {
						strTmp.Format("[%d]%s", nX + 1 + (nY*nSiteDivX) , isSafety == FALSE ? GetAABString(stDvc.nTestingCnt) : _T(""));
						nDeviceType = eDeviceType::_eDeviceType_Normal;
					}
					strErrMsg += strTmp;
					vErrPocket.push_back(CPoint(nX, nY));

					strTmp.Format(_T("[Site %d] Device Pick History Stage: %s, Pocket: X=%d Y=%d, Picker: X=%d Y=%d"),
						nX + 1 + (nY*nSiteDivX), g_TaskSystemCtrl.GetHistoryStageName(stDvc.stHistory.nStage), stDvc.stHistory.cPocketNum.x + 1, stDvc.stHistory.cPocketNum.y + 1, stDvc.stHistory.cPicker.x + 1, stDvc.stHistory.cPicker.y + 1);

					vErrHistor.push_back(strTmp);

					
				}
			}
		}
	}

	if( bErrArrivedSen == TRUE )
		return nDeviceType == eDeviceType::_eDeviceType_Normal ? ERR_PRESS_UNIT_1_ARRIVED_SENSOR + m_nUnitIdx : ERR_PRESS_UNIT_1_CLEAN_DEVICE_ARRIVED_SENSOR + m_nUnitIdx;

	return ERR_NO_ERROR;
}

int CTaskPressUnit::ChkSocketArrivedSen(CString &strErrMsg, std::vector<CPoint> &vErrPocket, std::vector<CString> &vErrHistor, int &nDeviceType, BOOL isSafety)
{
	BOOL bErrArrivedSen = FALSE, bChkSite = FALSE;
	int nOpt_ArriveDvcSen = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRS_DEVICE_ARRIVE_OPTION);
	CString strTmp = _T("");
	strErrMsg.Empty();

	int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	BOOL bAUtoGrrUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);
	BOOL bAUtoAuditUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoAuditUse);
	int nSiteAddr = NDM11_SocketOnOff_Site1_1;
	if (bAUtoGrrUse) {
		nSiteAddr = NDM11_GRR_SocketOnOff_Site1_1;
	}
	else if(bAUtoAuditUse)
		nSiteAddr = NDM11_Audit_SocketOnOff_Site1_1;

	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	ST_DD_DEVICE stDvc;

	for (int nX = 0; nX < nSiteDivX; nX++)
	{
		for (int nY = 0; nY < nSiteDivY; nY++)
		{
			bChkSite = FALSE;
			if (nOpt_ArriveDvcSen == CTaskPressUnit::eOnly_On_Site)
			{
				int nAdrr = nX + nY*STATION_MAX_X_SITE + m_nUnitIdx*STATION_MAX_PARA;
				if (g_DMCont.m_dmHandlerOpt.GN(nSiteAddr + nAdrr) == DEF_SITE_ON || g_DMCont.m_dmHandlerOpt.GN(nSiteAddr + nAdrr) == DEF_SITE_OFF_RESERVATION || g_DMCont.m_dmHandlerOpt.GN(nSiteAddr + nAdrr) == DEF_SITE_OFF_RESERVATION_YIELD)
					bChkSite = TRUE;
			}
			else {	//CTaskPressUnit::eAllSite
				bChkSite = TRUE;
			}
			if (bChkSite == TRUE)
			{
				int nErr = 0;

				int nPressSensorType = g_DMCont.m_dmEQP.GN(NDM0_PRESS_ARRIVED_SENSOR_TYPE);
				if (nPressSensorType == eArriveSensorType::eEach){
					nErr = ChkDI(CTaskPressUnit::xZAXIS_ARRIVED_SAFETY_SEN_1 + nX + nY*STATION_MAX_X_SITE, DEF_ON);
				}
				else {
					nErr = ChkDI(CTaskPressUnit::xZAXIS_ARRIVED_SAFETY_FIBER_SEN_R1 + nY, DEF_ON);
				}

				if (nErr != ERR_NO_ERROR) {
					bErrArrivedSen = TRUE;
					stDvc.clear();
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &stDvc);

					if (strcmp(stDvc.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0) {
						strTmp.Format("[%d (Clean Device)]", nX + 1 + (nY*nSiteDivX));
						nDeviceType = eDeviceType::_eDeviceType_Cleaning;
					}
					else {
						strTmp.Format("[%d] %s", nX + 1 + (nY*nSiteDivX), isSafety == FALSE ? GetAABString(stDvc.nTestingCnt) : _T(""));
						nDeviceType = eDeviceType::_eDeviceType_Normal;
					}

					strErrMsg += strTmp;
					vErrPocket.push_back(CPoint(nX, nY));

					strTmp.Format(_T("[Site %d] Device Pick History Stage: %s, Pocket: X=%d Y=%d, Picker: X=%d Y=%d"),
						nX + 1 + (nY*nSiteDivX), g_TaskSystemCtrl.GetHistoryStageName(stDvc.stHistory.nStage), stDvc.stHistory.cPocketNum.x + 1, stDvc.stHistory.cPocketNum.y + 1, stDvc.stHistory.cPicker.x + 1, stDvc.stHistory.cPicker.y + 1);

					vErrHistor.push_back(strTmp);
				}
			}
		}
	}
	

	if (bErrArrivedSen == TRUE)
		return nDeviceType == eDeviceType::_eDeviceType_Normal ? ERR_PRESS_UNIT_1_ARRIVED_SENSOR + m_nUnitIdx : ERR_PRESS_UNIT_1_CLEAN_DEVICE_ARRIVED_SENSOR + m_nUnitIdx;

	return ERR_NO_ERROR;
}

//======================================
// RETURN : TRUE = Exist Device
//          FALSE= Not Exist Device
//======================================
int CTaskPressUnit::ChkSiteInDvc()
{
	int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	for(int nY=0; nY < nSiteDivY; nY++ )
	{
		for(int nX=0; nX < nSiteDivX; nX++ )
		{
			int nDvc = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TEST_SITE_1+m_nUnitIdx, nX, nY);
			if( nDvc == eDeviceData_Exist )
				return TRUE;
		}
	}

	return FALSE;
}

int CTaskPressUnit::GetSiteInDvc(BOOL bRetestBin)
{
	int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nCnt = 0;
	ST_DD_DEVICE stSiteDvc;

	for(int nY=0; nY < nSiteDivY; nY++ )
	{
		for(int nX=0; nX < nSiteDivX; nX++ )
		{
			stSiteDvc.clear();
			int nDvc = g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1+m_nUnitIdx, nX, nY, &stSiteDvc);
			if (stSiteDvc.sExist == eDeviceData_Exist)
			{
				// retest buffer에 가야만 하는 bin의 갯수
				if (bRetestBin == TRUE)
				{
					BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stSiteDvc);

					if( strcmp(stSiteDvc.szErrString, DEF_TEST_RESULT_TP101) == 0 || bIsDefaultBin == TRUE
					|| ( stSiteDvc.nTestingCnt >= eARB_1stTestEnd && stSiteDvc.nTestingCnt <= eARB_2stTestEnd && stSiteDvc.nBinColor == eFailBin ) )
						nCnt++;
				}
				else
				{
					nCnt++;
				}
			}
		}
	}

	return nCnt;
}

int CTaskPressUnit::GetSiteInDvcTestPossible()
{
	int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nCnt = 0;
	ST_DD_DEVICE stSiteDvc;

	for (int nY = 0; nY < nSiteDivY; nY++)
	{
		for (int nX = 0; nX < nSiteDivX; nX++)
		{
			stSiteDvc.clear();
			int nDvc = g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &stSiteDvc);
			if (stSiteDvc.sExist == eDeviceData_Exist)
			{
				// retest buffer에 가야만 하는 bin의 갯수
				BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stSiteDvc);
				
				if(bIsDefaultBin == TRUE)
					continue;
				if(stSiteDvc.nBinColor == eGoodBin)
					continue;
				if(stSiteDvc.nTestingCnt >= eARB_2stTestEnd)
					continue;
				
				nCnt++;
			}
		}
	}

	return nCnt;
}

int CTaskPressUnit::GetSite1stFailInDvcCnt()
{
	int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nCnt = 0;
	ST_DD_DEVICE stSiteDvc;

	for (int nY = 0; nY < nSiteDivY; nY++)
	{
		for (int nX = 0; nX < nSiteDivX; nX++)
		{
			stSiteDvc.clear();
			int nDvc = g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &stSiteDvc);
			if (stSiteDvc.sExist == eDeviceData_Exist)
			{
				if (stSiteDvc.nTestingCnt == eARB_1stTestEnd && stSiteDvc.nBinColor == eFailBin)
					nCnt++;			
			}
		}
	}

	return nCnt;
}

int CTaskPressUnit::GetSocketOnOff( int nX, int nY )
{
	int nOnOff = g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + nX + nY*STATION_MAX_X_SITE + (m_nUnitIdx * STATION_MAX_PARA));
	int nNum = NDM11_SocketOnOff_Site1_1 + nX + nY*STATION_MAX_X_SITE + (m_nUnitIdx * STATION_MAX_PARA);
	if( nOnOff == DEF_SITE_ON || nOnOff == DEF_SITE_OFF_RESERVATION || nOnOff == DEF_SITE_OFF_RESERVATION_YIELD)
		return DEF_SITE_ON;

	return DEF_SITE_OFF;
}

int  CTaskPressUnit::GetSocketOnOffGrr(int nX, int nY)
{
	int nOnOff = g_DMCont.m_dmHandlerOpt.GN(NDM11_GRR_SocketOnOff_Site1_1 + nX + nY*STATION_MAX_X_SITE + (m_nUnitIdx * STATION_MAX_PARA));

	if (nOnOff == DEF_SITE_ON)
		return DEF_SITE_ON;

	return DEF_SITE_OFF;
}

int  CTaskPressUnit::GetSocketOnOffAudit(int nX, int nY)
{
	int nOnOff = g_DMCont.m_dmHandlerOpt.GN(NDM11_Audit_SocketOnOff_Site1_1 + nX + nY*STATION_MAX_X_SITE + (m_nUnitIdx * STATION_MAX_PARA));

	if (nOnOff == DEF_SITE_ON)
		return DEF_SITE_ON;
	if (nOnOff == DEF_SITE_OFF_RESERVATION)
		return DEF_SITE_OFF_RESERVATION;

	return DEF_SITE_OFF;
}

void CTaskPressUnit::SetFailHistoryAndPFcnt( int nX, int nY, int nPassFail, int nTestCnt)
{
	int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);

	int nPos = nX + nY*STATION_MAX_X_SITE + m_nUnitIdx*(STATION_MAX_X_SITE*STATION_MAX_Y_SITE);
	int nRetSiteHis = g_DMCont.m_dmContac_Dn.GN(NDM1_SiteHistory1_1 + nPos );
	int nChangeData = 0;
	int nSetBitTrue = 0x01, nSetBitFalse = 0x00;

	nRetSiteHis <<= 2;
	(nPassFail != eFailBin)? SETBIT(nRetSiteHis, 1) : CLRBIT(nRetSiteHis, 1);
	SETBIT(nRetSiteHis, 0);
	g_DMCont.m_dmContac_Dn.SN(NDM1_SiteHistory1_1 + nPos, nRetSiteHis);

	//Tester Status History Clear
	int nRetSiteStatusHis = g_DMCont.m_dmContac_Dn.GN(NDM1_SiteStatusHistory1_1 + nPos);
	CLRBIT(nRetSiteStatusHis, 0);
	CLRBIT(nRetSiteStatusHis, 1);
	g_DMCont.m_dmContac_Dn.SN(NDM1_SiteStatusHistory1_1 + nPos, nRetSiteStatusHis);

	// Product Count
	if(nPassFail == eFailBin){
		int nFailBinCnt    = g_DMCont.m_dmContac_Dn.GN(NDM1_FailBinCnt_Press1_1 + nPos );
		g_DMCont.m_dmContac_Dn.SN(NDM1_FailBinCnt_Press1_1+ nPos , ++nFailBinCnt);

		int nClsFailBinCnt = g_DMCont.m_dmContac_Dn.GN(NDM1_ClsAfterFailBinCntPress1_1 + nPos );
		g_DMCont.m_dmContac_Dn.SN(NDM1_ClsAfterFailBinCntPress1_1+ nPos, ++nClsFailBinCnt);

		if (bAutoRetestUseMode == TRUE) {
			if (nTestCnt == eARB_1stTestEnd)
			{
				int n1stFailBinCnt = g_DMCont.m_dmContac_Up.GN(NDM2_1STFailBinCnt_Press1_1 + nPos);
				g_DMCont.m_dmContac_Up.SN(NDM2_1STFailBinCnt_Press1_1 + nPos, ++n1stFailBinCnt);
			}
		}
		else
		{
			int n1stFailBinCnt = g_DMCont.m_dmContac_Up.GN(NDM2_1STFailBinCnt_Press1_1 + nPos);
			g_DMCont.m_dmContac_Up.SN(NDM2_1STFailBinCnt_Press1_1 + nPos, ++n1stFailBinCnt);
		}

	}else{
		int nGoodBinCnt    = g_DMCont.m_dmContac_Dn.GN(NDM1_GdBinCnt_Press1_1 + nPos);
		g_DMCont.m_dmContac_Dn.SN(NDM1_GdBinCnt_Press1_1 + nPos, ++nGoodBinCnt);

		int nClsGoodBinCnt = g_DMCont.m_dmContac_Dn.GN(NDM1_ClrAfterGdBinCntPress1_1+ nPos );
		g_DMCont.m_dmContac_Dn.SN(NDM1_ClrAfterGdBinCntPress1_1+ nPos, ++nClsGoodBinCnt);
	}
}

void CTaskPressUnit::SetNotTestHistory(int nX, int nY)
{
	int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	int nPos = nX + nY*STATION_MAX_X_SITE + m_nUnitIdx*(STATION_MAX_X_SITE*STATION_MAX_Y_SITE);
	int nRetSiteHis = g_DMCont.m_dmContac_Dn.GN(NDM1_SiteHistory1_1 + nPos);

	nRetSiteHis <<= 2;
	SETBIT(nRetSiteHis, 1);
	CLRBIT(nRetSiteHis, 0);
	g_DMCont.m_dmContac_Dn.SN(NDM1_SiteHistory1_1 + nPos, nRetSiteHis);
	SetHwBinHistory(nX, nY, 0);
}

void CTaskPressUnit::SetHwBinHistory(int nX, int nY, int nHwBinNo)
{
	int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	int nPos = nX + nY*STATION_MAX_X_SITE + m_nUnitIdx*(STATION_MAX_PARA);
	int nOldHis = g_DMCont.m_dmContac_Up.GN(NDM2_SiteHistoryHW1_1 + nPos);

	int nCalNo[5] = { 0, };

	for (int i = 1; i <= 5; i++)
	{
		int nData = nOldHis % 10;
		nCalNo[5 - i] = nData;
		nOldHis /= 10;
	}

	int nNewNo = nHwBinNo * 10000 + nCalNo[0] * 1000 + nCalNo[1] * 100 + nCalNo[2] * 10 + nCalNo[3];//nCalNo[1]*10000 + nCalNo[2] * 1000 + nCalNo[3] * 100 + nCalNo[4] * 10 + nHwBinNo;
	g_DMCont.m_dmContac_Up.SN(NDM2_SiteHistoryHW1_1 + nPos, nNewNo);
}

//[1/26/2024 dohyeong.kim]Tester 상태 미리 표기를 위함.   
//0bit -> 상태 미리 준비(eSetPrePare)
//1bit -> Test 진행 상태(eSetStatus)
void CTaskPressUnit::SetTestStatusHistory(int nStatus)
{
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	if (nTesterOption != eTester_EachSocket) {
		return;
	}


	int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	for (int nY = 0; nY < nSiteDivY; nY++)
	{
		for (int nX = 0; nX < nSiteDivX; nX++)
		{
			int nPos = nX + nY*STATION_MAX_X_SITE + m_nUnitIdx*(STATION_MAX_PARA);
			int nRetSiteHis = g_DMCont.m_dmContac_Dn.GN(NDM1_SiteStatusHistory1_1 + nPos);

			if (nStatus == eSetPrepare) {
				int nSiteOnOff = GetSocketOnOff(nX, nY);
				ST_DD_DEVICE stDvc;
				GetSiteDeviceData(&stDvc, nX, nY);
				if (nSiteOnOff == DEF_SITE_ON && stDvc.sExist == DEF_EXIST && strcmp(stDvc.szBinResult, DEF_SOCKET_CLEAN_DEVICE) != 0)
				{
					SETBIT(nRetSiteHis, 0);
				}
				else
				{
					CLRBIT(nRetSiteHis, 0);
				}
				g_DMCont.m_dmContac_Dn.SN(NDM1_SiteStatusHistory1_1 + nPos, nRetSiteHis);
			}
			else {
				if (GETBIT(nRetSiteHis, 0)) {
					if (g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run) == eRUN_MODE_ONLINE)
					{
						CTester* testerOctal = NULL;
						int nSocketIdx = nX + (nY * 4);
						int nPhysicalSocketNo = g_AppleTestIF.m_TesterMgr.EachTesterIndex(m_nUnitIdx+1, nSocketIdx);
						
						testerOctal = g_AppleTestIF.m_TesterMgr.GetTester(nPhysicalSocketNo, TRUE);
						if (testerOctal != NULL)
						{
							if (testerOctal->GetStatus() == STATUS_TESTING) {
								SETBIT(nRetSiteHis, 1);
							}
						}					
					}
					else //eRun_MODE_OFFLINE
					{
						SETBIT(nRetSiteHis, 1);
					}
					g_DMCont.m_dmContac_Dn.SN(NDM1_SiteStatusHistory1_1 + nPos, nRetSiteHis);
				}
			}
		}
	}
}


int CTaskPressUnit::GetCalcTestingCnt( int nCurTestCnt , ST_DD_DEVICE & stDvcData)
{
	BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
	int nAutoRetestKind = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoRetest);
	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	BOOL bRetestSikpFuncOn = g_DMCont.m_dmHandlerOpt.GB(BDM11_SltPgm_Sort_as_DefaultBin);
	BOOL bRetestSkip = FALSE;

	int nTestCnt = eARB_3stTestEnd;
	if(bAutoRetestUseMode)
	{
		//Auto Retest Skip
		//if (bRetestSikpFuncOn == DEF_ON) {
		//	for (int i = 0; i<(int)g_DefaultBinCategory.vDefaultBin.size(); i++) {
		//		if (g_DefaultBinCategory.vDefaultBin[i].Compare(stDvcData.szErrString) == 0) {
		//			bRetestSkip = TRUE;
		//		}
		//	}
		//}
		// USI는 DEF_TEST_ALARM_DEVICE 알람 발생 하면 Retest 하지 않음. [12/28/2021 donghyun.shin]
		if (strcmp(stDvcData.szErrString, DEF_TEST_ALARM_DEVICE) == 0
			/*|| strcmp(stDvcData.szErrString, DEF_TEST_TIME_OUT_ERR_DEVICE) == 0*/)
			bRetestSkip = TRUE;

		// Amkor 박보경 주임 요청 사항 으로 WRONG_APN Fail Bin은 Retest 하지 않고, Sorting [5/30/2025 donghyun.shin]
		if (nProjectOption == DEF_PROJECT_WATT && strcmp(stDvcData.szErrString, DEF_TEST_RESULT_WRONG_APN) == 0)
		{
			bRetestSkip = TRUE;
		}
	

// 		if(nProjectOption == DEF_PROJECT_MAUI)
// 		{
// 			if (strcmp(stDvcData.szErrString, DEF_TEST_RESULT_TP100) == 0 || strcmp(stDvcData.szErrString, DEF_TEST_RESULT_TP103) == 0) {
// 				bRetestSkip = TRUE;
// 			}
// 		}
	
		////Auto Retest Skip
		//BOOL bRetestSkip[4] = { g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Retest_Skip_HW1), g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Retest_Skip_HW2),
		// 	g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Retest_Skip_HW3), g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Retest_Skip_HW4)};
		// 
		//int nDvcTestCnt = nTestCnt;
		//if(nAutoRetestKind == eAutoRetest_AB && nTestCnt > eARB_NotTest){
		// 	nDvcTestCnt -= 1; //AB의 경우는 1stEnd 후에 바로 2stEnd로 바꾸어주기 때문에 빼야함.
		//}
		//// 
		//BOOL bRetestSkipOn = FALSE;
		//for(int i =0; i<4; i++){
		// 	if(bRetestSkip[i] == TRUE){
		// 		if(stDvcData.stRecInfo.stDvcInfo[nCurTestCnt].nHwBin == i){
		// 			bRetestSkipOn = TRUE;
		//			//nTestCnt =  eARB_3stTestEnd;
		// 		}
		// 	}
		//}

		if (bRetestSkip != TRUE)//if (bRetestSkipOn !=TRUE)
		{
			switch(nAutoRetestKind)
			{
			case eAutoRetest_AB: // 0 to 1 (XAB)
				{
					if(nCurTestCnt == eARB_NotTest){
						nTestCnt =  eARB_2stTestEnd;
					}else{
						nTestCnt = ++nCurTestCnt;
					}
				}break;
			case eAutoRetest_AAB:// normal
				{
					nTestCnt = ++nCurTestCnt;
				}break;
			case eAutoRetest_AA: // 2 to 3 ( AAX )
				{
					if(nCurTestCnt != eARB_1stTestEnd)
						nTestCnt = ++nCurTestCnt;
				}break;
			}
		}
	}

	return nTestCnt;
}

void CTaskPressUnit::PushYieldData( int nX, int nY, int nPassFail,int nTestCnt)
{
	BOOL bMD_YieldMonitor = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_YieldCtrl);

	if(bMD_YieldMonitor == DEF_ON)
	{
		//g_TaskSystemCtrl.AddYieldVariance(m_nUnitIdx, nPassFail, nX, nY);
		g_TaskSystemCtrl.AddYieldSample(m_nUnitIdx, nPassFail, nX, nY);
		g_TaskSystemCtrl.AddYieldContiFail(m_nUnitIdx, nPassFail, nX, nY);
		g_TaskSystemCtrl.AddYield1stFailSample(m_nUnitIdx, nPassFail, nX, nY, nTestCnt);

		int nYieldCnt = g_DMCont.m_dmContac_Dn.GN( NDM1_Yield_Count_Press1_1 + nX + nY*STATION_MAX_X_SITE + m_nUnitIdx * STATION_MAX_PARA );
		g_DMCont.m_dmContac_Dn.SN( NDM1_Yield_Count_Press1_1 + nX + nY*STATION_MAX_X_SITE + m_nUnitIdx * STATION_MAX_PARA , ++nYieldCnt);

		if (nTestCnt == 0 && nPassFail == eFailBin)
		{
			int nYieldCnt = g_DMCont.m_dmContac_Dn.GN(NDM1_Yield_1st_Count_Press1_1 + nX + nY*STATION_MAX_X_SITE + m_nUnitIdx * STATION_MAX_PARA);
			g_DMCont.m_dmContac_Dn.SN(NDM1_Yield_1st_Count_Press1_1 + nX + nY*STATION_MAX_X_SITE + m_nUnitIdx * STATION_MAX_PARA, ++nYieldCnt);
		}

	}else{
		//g_TaskSystemCtrl.YieldVarianceClear(m_nUnitIdx, nX, nY);
		g_TaskSystemCtrl.YieldSampleClear(m_nUnitIdx, nX, nY);
		g_TaskSystemCtrl.YieldContiFailClear(m_nUnitIdx, nX, nY);
		g_TaskSystemCtrl.Yield1stFailClear(m_nUnitIdx, nX, nY);
	}
}

void CTaskPressUnit::SetTestTimeOutHandlerErrorBin(int nCmdStage, SYSTEMTIME sysTmTestStart)
{
	int nProject = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	// TODO KSJ : Skip 했을 경우에는 어떻게 Bin 처리를 해줘야 하는가?
	//꺼내온 Data가지고 처리한다. // Data 처리
	ST_DD_DEVICE pDvcData;
	BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
	BOOL bGrrCancel = g_TaskSystemCtrl.GetAutoGrrCancel();
	BOOL bLoopCancel = g_TaskSystemCtrl.GetAutoLoopCancel();

	// Get local time
	SYSTEMTIME timeEnd;
	::GetLocalTime(&timeEnd);
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	CTesterStatus cTesterStatus;
	//CAgentStatus cAgentStatus;
	int nSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

// #if	DEF_TESTER_COMM_SPEC_ONE_SOCKET
// 	int nDirSiteOnOff[eEachTesterSocket::eMaxTesterForYanmade] = { 0, };
// #else
// 	int nDirSiteOnOff[eMaxTester] = { 0, };
// #endif

	//Single, Dual, Each 동일하게 사용
	int nDirSiteOnOff[eEachTesterSocket::eMaxTesterForYanmade] = { 0, };

	TesterSiteOnOffCheck(nDirSiteOnOff);

	ResultData rData;
	
	if (nTesterOption != eTester_Agent)
	{
		cTesterStatus = g_AppleTestIF.GetTesterStatus(m_nUnitIdx + 1, nTesterOption);
		g_AppleTestIF.GetResultDataFromTester(m_nUnitIdx + 1, &rData, nDirSiteOnOff, nTesterOption);
	}	
	else
	{
// 		cAgentStatus = g_AppleAgentIF.GetAgentStatus(m_nUnitIdx + 1);
// 		g_AppleAgentIF.GetResultDataFromAgent(m_nUnitIdx + 1, &rData);
	}
		

	char szEqpName[128] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_UDP_HOST_NAME, szEqpName, sizeof(szEqpName));

	for (int nY = 0; nY < nSiteY; nY++)
	{
		for (int nX = 0; nX < nSiteX; nX++)
		{
			CString strResult = _T("FAIL");
			pDvcData.clear();
			// get device data
			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &pDvcData);
			BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
			int nSiteOnOff = (bAutoGrrUse == TRUE ? GetSocketOnOffGrr(nX, nY) : GetSocketOnOff(nX, nY));


			if (nSiteOnOff == DEF_SITE_ON && pDvcData.sExist == DEF_EXIST)
			{
				//Error code compare result data
				CString strTRL = _T(""), strStartTm = _T(""), strEndTm = _T("");
				char szHWbin[16] = { 0, };

				int  nBinColor = eFailBin;
 
				int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

				int nTargetPos = nX + nY*STATION_MAX_X_SITE;

				if (rData.m_nTestResult[(nX + STATION_MAX_X_SITE*nY)] == -1)
				{
					strcpy_s(pDvcData.szErrString, sizeof(pDvcData.szErrString), DEF_TEST_TIME_OUT_ERR_DEVICE);
					g_TaskPressUnit[m_nUnitIdx].MakeLog("Skip to online test(Test Time out)");
					strcpy_s(pDvcData.szBinResult, sizeof(pDvcData.szBinResult), "97");
					strcpy_s(pDvcData.szErrorCode, sizeof(pDvcData.szErrorCode), "97");
					strcpy_s(pDvcData.szTesterHWBin, sizeof(pDvcData.szTesterHWBin), "3");
					sprintf_s(szHWbin, sizeof(szHWbin), "4");
				}
				else
				{
					BOOL bChkRegCateInfo = FALSE;
					// case 1 :: category에 등록이 되어있는 경우
					for (int i = 0; i < g_BinCategory.nIndexCount; i++) {
						if (strcmp(g_BinCategory.aCateInfo[i].szDesc, rData.m_cErrorStr[nTargetPos]) == 0)
						{
							strcpy_s(pDvcData.szBinResult, sizeof(pDvcData.szBinResult), g_BinCategory.aCateInfo[i].szSWBin);
							sprintf_s(szHWbin, sizeof(szHWbin), g_BinCategory.aCateInfo[i].szHWBin);
							pDvcData.nBinColor = rData.m_nTestResult[nTargetPos]; /*Pass/Fail 받은 것*/
							strcpy_s(pDvcData.szErrString, sizeof(pDvcData.szErrString), rData.m_cErrorStr[nTargetPos]);
							strcpy_s(pDvcData.szErrorCode, sizeof(pDvcData.szErrorCode), rData.m_cErrorCode[nTargetPos]);
							if(strcmp(pDvcData.szErrorCode,"") == 0)
								strcpy_s(pDvcData.szErrorCode, sizeof(pDvcData.szErrorCode), g_BinCategory.aCateInfo[i].szCode);
							if (strcmp(pDvcData.szTesterHWBin, "") == 0)
								strcpy_s(pDvcData.szTesterHWBin, sizeof(pDvcData.szTesterHWBin), g_BinCategory.aCateInfo[i].szHWBin_Tester);

							nBinColor = pDvcData.nBinColor;
							bChkRegCateInfo = TRUE;
						}
					}
					// case 2 :: category에 등록이 안되어 있는 경우
					if (bChkRegCateInfo == FALSE) {
						for (int i = 0; i < g_BinCategory.nIndexCount; i++) {
							if (atoi(g_BinCategory.aCateInfo[i].szSWBin) == rData.m_nTestResult[nTargetPos]) {
								strcpy_s(pDvcData.szBinResult, sizeof(pDvcData.szBinResult), g_BinCategory.aCateInfo[i].szSWBin);
								sprintf_s(szHWbin, sizeof(szHWbin), g_BinCategory.aCateInfo[i].szHWBin);
								// Bin Color = Msg Data Pass/Fail
								nBinColor = pDvcData.nBinColor = rData.m_nTestResult[nTargetPos]; /*Pass/Fail 받은 것*/
								strcpy_s(pDvcData.szErrString, sizeof(pDvcData.szErrString), rData.m_cErrorStr[nTargetPos]);
								strcpy_s(pDvcData.szErrorCode, sizeof(pDvcData.szErrorCode), rData.m_cErrorCode[nTargetPos]);
								if (strcmp(pDvcData.szErrorCode, "") == 0)
									strcpy_s(pDvcData.szErrorCode, sizeof(pDvcData.szErrorCode), g_BinCategory.aCateInfo[i].szCode);
								if (strcmp(pDvcData.szTesterHWBin, "") == 0)
									strcpy_s(pDvcData.szTesterHWBin, sizeof(pDvcData.szTesterHWBin), g_BinCategory.aCateInfo[i].szHWBin_Tester);
							}
						}
					}
					// case 3 :: case 2 에서도 판별이 안나온 경우
					if (bChkRegCateInfo == FALSE && (strcmp(szHWbin, "0") == 0 || strcmp(szHWbin, "") == 0)) {
						strcpy_s(pDvcData.szErrString, sizeof(pDvcData.szErrString), DEF_TEST_TIME_OUT_ERR_DEVICE);
						for (int i = 0; i < g_BinCategory.nIndexCount; i++) {
							if (strcmp(g_BinCategory.aCateInfo[i].szDesc, pDvcData.szErrString) == 0)
							{
								strcpy_s(pDvcData.szBinResult, sizeof(pDvcData.szBinResult), g_BinCategory.aCateInfo[i].szSWBin);
								sprintf_s(szHWbin, sizeof(szHWbin), g_BinCategory.aCateInfo[i].szHWBin);
								strcpy_s(pDvcData.szErrorCode, sizeof(pDvcData.szErrorCode), g_BinCategory.aCateInfo[i].szCode);
								strcpy_s(pDvcData.szTesterHWBin, sizeof(pDvcData.szTesterHWBin), g_BinCategory.aCateInfo[i].szHWBin_Tester);
								nBinColor = pDvcData.nBinColor;
							}
						}
					}
				}				
				
// 				else {
// 					strcpy_s(pDvcData.szErrString, sizeof(pDvcData.szErrString), "FAIL");
// 					g_TaskPressUnit[m_nUnitIdx].MakeLog("Skip to online test(Test Time out)");
// 
// 					strcpy_s(pDvcData.szBinResult, sizeof(pDvcData.szBinResult), "2");
// 					sprintf_s(szHWbin, sizeof(szHWbin), "2");
// 
// 					pDvcData.nBinColor = nBinColor;
// 				}			

				//for (int i = 0; i < g_BinCategory.nIndexCount; i++)
				//{
				//	if (strcmp(g_BinCategory.aCateInfo[i].szDesc, pDvcData.szErrString) == 0) {
				//		strcpy_s(pDvcData.szBinResult, sizeof(pDvcData.szBinResult), g_BinCategory.aCateInfo[i].szSWBin);
				//		sprintf_s(szHWbin, sizeof(szHWbin), g_BinCategory.aCateInfo[i].szHWBin);
				//		// Bin Color = Msg Data Pass/Fail
				//		pDvcData.nBinColor = atoi(g_BinCategory.aCateInfo[i].szPassFail); /*Pass/Fail 받은 것*/
				//		break;
				//	}
				//}			

				// Record Test Result log file
				
				strStartTm.Format("%04d%02d%02d%02d%02d%02d%03d"
					, sysTmTestStart.wYear, sysTmTestStart.wMonth, sysTmTestStart.wDay
					, sysTmTestStart.wHour, sysTmTestStart.wMinute, sysTmTestStart.wSecond, sysTmTestStart.wMilliseconds);
				strEndTm.Format("%04d%02d%02d%02d%02d%02d%03d"
					, timeEnd.wYear, timeEnd.wMonth, timeEnd.wDay
					, timeEnd.wHour, timeEnd.wMinute, timeEnd.wSecond, timeEnd.wMilliseconds);
				
				//송도 Amkor 요청사항. 대문자 -> 소문자 변환
				CString strBinResult = (rData.m_nTestResult[nTargetPos] == eGoodBin) ? _T("PASS") : _T("FAIL");
				strBinResult.MakeLower();

//				if (nTesterOption == eTester_EachSocket)
				{
					int nStationID = 0;
					CString strStationID = _T("");
				
					nStationID = (((nSiteX*nSiteY)*m_nUnitIdx) + nX + nY*nSiteX) + 1;
					CTesterStatus cTesterStatus = g_AppleTestIF.GetTesterStatus(nStationID);
					strStationID.Format("%d", cTesterStatus.m_nStationID);

					strTRL.Format(_T("%s,%s,%s,%s,%s,%s,%s")
						, pDvcData.sz2DBarcode
						, strStationID
						, (LPCTSTR)strStartTm, (LPCTSTR)strEndTm
						, strBinResult/*(rData.m_nTestResult[nTargetPos] == eGoodBin ? _T("PASS") : _T("FAIL"))*/
						, rData.m_cErrorStr[nTargetPos], szHWbin);
				}
// 				else
// 				{
// 					strTRL.Format("%s,%s,%s,%d,%d,%s,%s,%s,%s,%s,%s,%s"
// 						, pDvcData.sz2DBarcode, rData.m_cDutSn[nTargetPos], szEqpName
// 						, m_nUnitIdx + 1, nX + nY*STATION_MAX_X_SITE + 1
// 						, (LPCTSTR)(cTesterStatus.m_sTesterID_Orginal)
// 						, (LPCTSTR)(cTesterStatus.m_sSocketID_Orginal[(nX + nY*STATION_MAX_X_SITE)])
// 						, (LPCTSTR)strStartTm, (LPCTSTR)strEndTm
// 						, strBinResult/*(rData.m_nTestResult[nTargetPos] == eGoodBin ? _T("PASS") : _T("FAIL"))*//*_T("FAIL")*/
// 						, pDvcData.szErrString, szHWbin);
// 				}
			

				TWLOG.MakeLog(static_cast<int>(eLogID_PRODUCTCSV), Debug, "", __LINE__, NULL, strTRL);
//				TWLOG.MakeLog(static_cast<int>(eLogID_2DIDCSV), Debug, "", __LINE__, NULL, strTRL);

				// Set Site number
				sprintf_s(pDvcData.szTestSiteNum, sizeof(pDvcData.szTestSiteNum), "%d", m_nUnitIdx);
				pDvcData.cSubSiteNum = CPoint(nX, nY);

				// Set Testing Count
				int nTestCnt = pDvcData.nTestingCnt;

				BOOL bAutoGrrUseMode = g_TaskSystemCtrl.GetAutoGrrUse();
				BOOL bAutoLoopUseMode = g_TaskSystemCtrl.GetAutoLoopUse();
				if (bAutoGrrUseMode != TRUE)
				{	// JAM RATE DATA RECORD
					if (bAutoLoopUseMode != TRUE){
						ASSERT(nTestCnt >= eARB_NotTest && nTestCnt <= eARB_3stTestEnd);
						int nDvcTestCnt = nTestCnt;
						if (bAutoRetestUseMode == TRUE && g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoRetest) == eAutoRetest_AB && nTestCnt > eARB_NotTest) {
							nDvcTestCnt -= 1; //AB의 경우는 1stEnd 후에 바로 2stEnd로 바꾸어주기 때문에 빼야함.
						}

						pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].nHwBin = atoi(szHWbin) - 1;
						pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].nPF = nBinColor;
						pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].nSiteNo = atoi(pDvcData.szTestSiteNum);
						pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].cSubSiteNo = pDvcData.cSubSiteNum;

						sprintf_s(pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrStr, sizeof(pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrStr), pDvcData.szErrString);
						sprintf_s(pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrCode, sizeof(pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrCode), pDvcData.szErrorCode);
						sprintf_s(pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szTesterHwBin, sizeof(pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szTesterHwBin), pDvcData.szTesterHWBin);

						int nArtCnt = g_DMCont.m_dmEQP.GN(NDM0_ART_FT_TOTAL + (nTestCnt * 2));
						g_DMCont.m_dmEQP.SN(NDM0_ART_FT_TOTAL + (nTestCnt * 2), ++nArtCnt);

						// push yileld monitoring data
						PushYieldData(nX, nY, nBinColor, nTestCnt);
					}
					else {
						if (bAutoLoopUseMode == TRUE) {
							if (bLoopCancel == FALSE)
								pDvcData.stLoopTestInfo[m_nUnitIdx].testDoneAll();
							else {
								int nMaxTestCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoLoopTestCount);
								for (int nTestCnt = pDvcData.stLoopTestInfo[m_nUnitIdx].nTestCnt[nX][nY]; nTestCnt < nMaxTestCount; nTestCnt++)
								{
									pDvcData.stLoopTestInfo[m_nUnitIdx].testDoneAll();
								}
							}
						}						
					}
					
				}
				else {
					//if (strcmp(pReadData.szErrString, DEF_TEST_RESULT_TP101) != 0)
					//{
					int nFindIdx = 0;
					nFindIdx = pDvcData.findIndexTestSite(pDvcData.szTestSiteNum);
					ASSERT(nFindIdx >= 0);

					//BOOL bSwapOption = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrDeviceSwapUse);
					//if (bSwapOption == TRUE)
					//	pDvcData.stGrrTestInfo[nFindIdx].testDone(nX, nY);	// 해당 Site의 Test 횟수를 증가시켜줌
					//else
						//pDvcData.stGrrTestInfo[nFindIdx].testDoneAll();	// 해당 Site의 Test 횟수를 증가시켜줌
					if (bGrrCancel == FALSE)
						pDvcData.stGrrTestInfo[nFindIdx].testDoneAll();	// 해당 Site의 Test 횟수를 증가시켜줌
					else
					{
						int nMaxTestCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrTestCount);
						for (int nTestCnt = pDvcData.stGrrTestInfo[nFindIdx].nTestCnt[nX][nY]; nTestCnt < nMaxTestCount; nTestCnt++)
						{
							pDvcData.stGrrTestInfo[nFindIdx].testDoneAll();
						}
					}

																			// Create DB Data
					_tGRR_DATA stGrrData;
					sprintf(stGrrData.szUnique, "%s", pDvcData.szUniqueID);
					sprintf(stGrrData.sz2DID, "%s", pDvcData.sz2DBarcode);
					sprintf(stGrrData.szSiteNum, "%d", atoi(pDvcData.szTestSiteNum) + 1);
					sprintf(stGrrData.szSubSiteNo, "%d_%d", nX + 1, nY + 1);
					stGrrData.nTestCntNo = pDvcData.stGrrTestInfo[nFindIdx].nTestCnt[nX][nY];
					sprintf(stGrrData.szTestResult, (atoi(pDvcData.szBinResult) == eGoodBin ? "PASS" : "FAIL"));
					sprintf(stGrrData.szResultDescription, "%s", pDvcData.szErrString);

					g_DB.ASYNC_INSERT_GRRDATA_TBL(stGrrData);

					char szGrrSiteHis[MAX_PATH] = { 0, };
					g_DMCont.m_dmContac_Dn.GS(SDM1_SiteGrrPF_History1_1 + m_nUnitIdx*STATION_MAX_PARA + nX + nY * STATION_MAX_X_SITE, szGrrSiteHis, sizeof(szGrrSiteHis));
					CString strGrrSiteHis = _T("");
					strGrrSiteHis.Format("%s%s", szGrrSiteHis, (atoi(pDvcData.szBinResult) == eGoodBin ? "P" : "F"));
					int nlength = strGrrSiteHis.GetLength();
					if (nlength >= MAX_STRING_LENGTH)
						g_DMCont.m_dmContac_Dn.SS(SDM1_SiteGrrPF_History1_1 + m_nUnitIdx*STATION_MAX_PARA + nX + nY * STATION_MAX_X_SITE, (LPSTR)(LPCTSTR)strGrrSiteHis, strGrrSiteHis.GetLength() + 1);
					//}								
				}

				//Test time over 발생시 Auto Retest Skip 안함
				//pDvcData.nTestingCnt = nTestCnt = eARB_3stTestEnd;
				// AutoRetest AAB Timeout관련 Count 추가 [2/4/2021 donghyun.shin]
// 				if (nProjectOption == DEF_PROJECT_MARLIN) {
// 					int nCaclTestCnt = g_TaskPressUnit[m_nUnitIdx].GetCalcTestingCnt(nTestCnt, pDvcData);
// 					pDvcData.nTestingCnt = nTestCnt = nCaclTestCnt;
// 				}
				
// 				if ( strcmp(pDvcData.szErrString, DEF_TEST_TIME_OUT_ERR_DEVICE) == 0 )
// 				{
// 					//pDvcData.nTestingCnt = nTestCnt = eARB_3stTestEnd;
// 					int nCaclTestCnt = g_TaskPressUnit[m_nUnitIdx].GetCalcTestingCnt(nTestCnt, pDvcData);
// 					pDvcData.nTestingCnt = nTestCnt = nCaclTestCnt;
// 				}
// 				else if(strcmp(pDvcData.szErrString, DEF_TEST_TIME_OUT_ERR_DEVICE) != 0)
// 				{
					int nCaclTestCnt = g_TaskPressUnit[m_nUnitIdx].GetCalcTestingCnt(nTestCnt, pDvcData);
					pDvcData.nTestingCnt = nTestCnt = nCaclTestCnt;
//				}


				// change status

				//pDvcData.nTestingInfo = eTestSite_TestTimeOut;

				// site pass fail history
				int nDataPos = nX + nY*STATION_MAX_X_SITE + (m_nUnitIdx*STATION_MAX_PARA);

				// set fail history
				SetFailHistoryAndPFcnt(nX, nY, nBinColor, nTestCnt);

				// set hw bin history
				SetHwBinHistory(nX, nY, atoi(szHWbin));

				// set last error string
				g_DMCont.m_dmContac_Dn.SS(SDM1_SiteLastDesc1_1 + nDataPos, pDvcData.szErrString, sizeof(pDvcData.szErrString));
				
				// setting device data
				g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &pDvcData);

				g_TaskPressUnit[m_nUnitIdx].SetSiteDvcTestingInfo(nX, nY, eTestSite_TestEnd);

			}
			else if (GetSocketOnOff(nX, nY) == DEF_SITE_ON) {
				SetNotTestHistory(nX, nY);
			}
		}
	}

	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
	if (nRunMode == eRUN_MODE_ONLINE)
	{
		// Amkor BinSolution
		CString strBinSolutionData = _T("");
		g_TaskPressUnit->SetTextBinSolution(strBinSolutionData, m_nUnitIdx);

		int resultCode = ::sendto(g_UDPSockBinSolution, strBinSolutionData, strBinSolutionData.GetLength(), 0,
			(SOCKADDR*)&g_udpserverAddr, sizeof(g_udpserverAddr));
		g_TaskPressUnit[m_nUnitIdx].MakeLog("[SetTestTimeOutHandlerErrorBin] [BinSolution=%s]", strBinSolutionData);
	}
}



int CTaskPressUnit::CovertAutoTestSiteIdx(int nStationID)
{
	int nIdx = 0;

	switch (nStationID)
	{
	case ePressUnit_Down_1: case ePressUnit_Down_2:
	case ePressUnit_Down_3: case ePressUnit_Down_4:
// 	case ePressUnit_Up_1:   case ePressUnit_Up_2:
// 	case ePressUnit_Up_3:   case ePressUnit_Up_4:
	{
		nIdx = eTestPP_1;
	}break;
	
	case ePressUnit_Down_5: case ePressUnit_Down_6:
	case ePressUnit_Down_7: case ePressUnit_Down_8:
// 	case ePressUnit_Up_5:   case ePressUnit_Up_6:
// 	case ePressUnit_Up_7:   case ePressUnit_Up_8:
	{
		nIdx = eTestPP_2;
	}break;
	}
	return nIdx;
}

int CTaskPressUnit::CovertPressAreaIdx(int nStationID)
{
	int nIdx = 0;

	switch (nStationID)
	{
		case ePressUnit_Down_1:	case ePressUnit_Down_5:
		{
			nIdx = eATS_DN_STATION_L1;
		}break;
		case ePressUnit_Down_2: case ePressUnit_Down_6:
		{
			nIdx = eATS_DN_STATION_L2;
		}break;
		case ePressUnit_Down_3: case ePressUnit_Down_7:
		{
			nIdx = eATS_DN_STATION_L3;
		}break;
		case ePressUnit_Down_4: case ePressUnit_Down_8:
		{
			nIdx = eATS_DN_STATION_L4;
		}break;

// 		case ePressUnit_Up_1: case ePressUnit_Up_5:
// 		{
// 			nIdx = eATS_UP_STATION_L1;
// 		}break;
// 		case ePressUnit_Up_2: case ePressUnit_Up_6:
// 		{
// 			nIdx = eATS_UP_STATION_L2;
// 		}break;
// 		case ePressUnit_Up_3: case ePressUnit_Up_7:
// 		{
// 			nIdx = eATS_UP_STATION_L3;
// 		}break;
// 		case ePressUnit_Up_4: case ePressUnit_Up_8:
// 		{
// 			nIdx = eATS_UP_STATION_L4;
// 		}break;
	}

	return nIdx;
}

int CTaskPressUnit::GetAllStationFailCnt()
{
	return m_AllStationFailCnt;
}

void CTaskPressUnit::AddAllStationFailCnt()
{
	m_AllStationFailCnt++;
}
void CTaskPressUnit::ClearAllStationFailCnt()
{
	m_AllStationFailCnt = 0;
}

BOOL CTaskPressUnit::GetStationOn(BOOL bGrrMode)
{
	int nOnCnt = 0;

	int nSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	for (int nX = 0; nX < nSiteX; nX++)
	{
		for (int nY = 0; nY < nSiteY; nY++)
		{
			int nTarget = (bGrrMode == TRUE ? NDM11_GRR_SocketOnOff_Site1_1 : NDM11_SocketOnOff_Site1_1) + nX + nY*STATION_MAX_X_SITE + (m_nUnitIdx *STATION_MAX_PARA);
			if (g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_ON || g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_OFF_RESERVATION)
				nOnCnt++;
		}
	}

	if (nOnCnt > 0) return TRUE;

	return FALSE;
}

int CTaskPressUnit::GetPressUnitIndex(int nPressIndex)
{
	//  Test Code [11/15/2020 jaehoon.jung]
	//함수화 해야함

	int nPressIndexForMotor = -1;
	switch (nPressIndex)
	{
	case ePressUnit_Down_1: nPressIndexForMotor = eAXIS_PRESS_UNIT_DN1; break;
	case ePressUnit_Down_2: nPressIndexForMotor = eAXIS_PRESS_UNIT_DN2; break;
	case ePressUnit_Down_3: nPressIndexForMotor = eAXIS_PRESS_UNIT_DN3; break;
	case ePressUnit_Down_4: nPressIndexForMotor = eAXIS_PRESS_UNIT_DN4; break;
	case ePressUnit_Down_5: nPressIndexForMotor = eAXIS_PRESS_UNIT_DN5; break;
	case ePressUnit_Down_6: nPressIndexForMotor = eAXIS_PRESS_UNIT_DN6; break;
	case ePressUnit_Down_7: nPressIndexForMotor = eAXIS_PRESS_UNIT_DN7; break;
	case ePressUnit_Down_8: nPressIndexForMotor = eAXIS_PRESS_UNIT_DN8; break;

// 	case ePressUnit_Up_1: nPressIndexForMotor = eAXIS_PRESS_UNIT_UP1; break;
// 	case ePressUnit_Up_2: nPressIndexForMotor = eAXIS_PRESS_UNIT_UP2; break;
// 	case ePressUnit_Up_3: nPressIndexForMotor = eAXIS_PRESS_UNIT_UP3; break;
// 	case ePressUnit_Up_4: nPressIndexForMotor = eAXIS_PRESS_UNIT_UP4; break;
// 	case ePressUnit_Up_5: nPressIndexForMotor = eAXIS_PRESS_UNIT_UP5; break;
// 	case ePressUnit_Up_6: nPressIndexForMotor = eAXIS_PRESS_UNIT_UP6; break;
// 	case ePressUnit_Up_7: nPressIndexForMotor = eAXIS_PRESS_UNIT_UP7; break;
// 	case ePressUnit_Up_8: nPressIndexForMotor = eAXIS_PRESS_UNIT_UP8; break;

	default: ASSERT(FALSE);
		break;
	}


	return nPressIndexForMotor;

}

void CTaskPressUnit::SetTest2DIDHandlerErrorBin(int nCmdStage, SYSTEMTIME sysTmTestStart,int* n2DIDSiteErrorNumber)
{
	ST_DD_DEVICE pDvcData;
// #if	DEF_TESTER_COMM_SPEC_ONE_SOCKET
// 	int nDirSiteOnOff[eEachTesterSocket::eMaxTesterForYanmade] = { 0, };
// #else
// 	int nDirSiteOnOff[eMaxTester] = { 0, };
// #endif

	//Single, Dual, Each 동일하게 사용
	int nDirSiteOnOff[eEachTesterSocket::eMaxTesterForYanmade] = { 0, };
	TesterSiteOnOffCheck(nDirSiteOnOff);

	ResultData rData;
	BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
	BOOL bGrrCancel = g_TaskSystemCtrl.GetAutoGrrCancel();
	BOOL bLoopCancel = g_TaskSystemCtrl.GetAutoLoopCancel();
	
	// Get local time
	SYSTEMTIME timeEnd;
	::GetLocalTime(&timeEnd);
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	CTesterStatus cTesterStatus;
	//CAgentStatus cAgentStatus;
	if (nTesterOption != eTester_Agent)
	{
		cTesterStatus = g_AppleTestIF.GetTesterStatus(m_nUnitIdx + 1, nTesterOption);
		g_AppleTestIF.GetResultDataFromTester(m_nUnitIdx + 1, &rData, nDirSiteOnOff, nTesterOption);
	}
	else
	{
// 		cAgentStatus = g_AppleAgentIF.GetAgentStatus(m_nUnitIdx + 1);
// 		g_AppleAgentIF.GetResultDataFromAgent(m_nUnitIdx + 1, &rData);
	}
		

	int nSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	char szEqpName[128] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_UDP_HOST_NAME, szEqpName, sizeof(szEqpName));

	for (int nY = 0; nY < nSiteY; nY++)
	{
		for (int nX = 0; nX < nSiteX; nX++)
		{
			CString strResult = _T("FAIL");
			pDvcData.clear();
			// get device data
			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &pDvcData);
			BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
			int nSiteOnOff = (bAutoGrrUse == TRUE ? GetSocketOnOffGrr(nX, nY) : GetSocketOnOff(nX, nY));

			if (nSiteOnOff == DEF_SITE_ON && pDvcData.sExist == DEF_EXIST)
			{
				//Error code compare result data
				CString strTRL = _T(""), strStartTm = _T(""), strEndTm = _T("");
				char szHWbin[16] = { 0, };
				BOOL bChkRegCateInfo = FALSE;
				int  nBinColor = eFailBin;

				int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
				BOOL bAutoLoopUseMode = g_TaskSystemCtrl.GetAutoLoopUse();
				//				if (nProjectOption == DEF_PROJECT_MAUI) {
				// sot error?
				int nTargetPos = nX + nY*STATION_MAX_X_SITE;

				if (n2DIDSiteErrorNumber[nTargetPos] == TRUE)
				{				
					char szCompany[16] = { 0, };
					g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
					
					strcpy_s(rData.m_cErrorStr[nTargetPos], sizeof(rData.m_cErrorStr[nTargetPos]), nProjectOption == DEF_PROJECT_WATT ? DEF_TEST_RESULT_WRONG_APN : DEF_2D_READ_ERR_DEVICE);
					strcpy_s(pDvcData.szErrString, sizeof(pDvcData.szErrString), nProjectOption == DEF_PROJECT_WATT ? DEF_TEST_RESULT_WRONG_APN : DEF_2D_READ_ERR_DEVICE);
					for (int i = 0; i < g_BinCategory.nIndexCount; i++) {
						if (strcmp(g_BinCategory.aCateInfo[i].szDesc, rData.m_cErrorStr[nTargetPos]) == 0)
						{
							strcpy_s(pDvcData.szErrorCode, sizeof(pDvcData.szErrorCode), g_BinCategory.aCateInfo[i].szCode);
							strcpy_s(pDvcData.szTesterHWBin, sizeof(pDvcData.szTesterHWBin), g_BinCategory.aCateInfo[i].szHWBin_Tester);
						}
					}
					MakeLog("Skip to 2DID Fail DEF_2D_READ_ERR_DEVICE test");
					
					pDvcData.nBinColor = nBinColor;
					pDvcData.n2DIDErrorCnt++;
				}
				
				if (strcmp(pDvcData.szErrString ,DEF_HANDLER_ERR_DEVICE) != 0)
				{
					// case 1 :: category에 등록이 되어있는 경우
					for (int i = 0; i < g_BinCategory.nIndexCount; i++) {
						if (strcmp(g_BinCategory.aCateInfo[i].szDesc, rData.m_cErrorStr[nTargetPos]) == 0)
						{
							strcpy_s(pDvcData.szBinResult, sizeof(pDvcData.szBinResult), g_BinCategory.aCateInfo[i].szSWBin);
							sprintf_s(szHWbin, sizeof(szHWbin), g_BinCategory.aCateInfo[i].szHWBin);
							if (strcmp(pDvcData.szErrString, nProjectOption == DEF_PROJECT_WATT ? DEF_TEST_RESULT_WRONG_APN : DEF_2D_READ_ERR_DEVICE) != 0) {
								pDvcData.nBinColor = rData.m_nTestResult[nTargetPos]; /*Pass/Fail 받은 것*/							
							}

							strcpy_s(pDvcData.szErrString, sizeof(pDvcData.szErrString), rData.m_cErrorStr[nTargetPos]);
							nBinColor = pDvcData.nBinColor;				
							strcpy_s(pDvcData.szErrorCode, sizeof(pDvcData.szErrorCode), rData.m_cErrorCode[nTargetPos]);
							if(strcmp(pDvcData.szErrorCode,"") ==0)
								strcpy_s(pDvcData.szErrorCode, sizeof(pDvcData.szErrorCode), g_BinCategory.aCateInfo[i].szCode);
							if (strcmp(pDvcData.szTesterHWBin, "") == 0)
								strcpy_s(pDvcData.szTesterHWBin, sizeof(pDvcData.szTesterHWBin), g_BinCategory.aCateInfo[i].szHWBin_Tester);


							bChkRegCateInfo = TRUE;
						}
					}
					// case 2 :: category에 등록이 안되어 있는 경우
					if (bChkRegCateInfo == FALSE) {
						for (int i = 0; i < g_BinCategory.nIndexCount; i++) {
							if (atoi(g_BinCategory.aCateInfo[i].szSWBin) == rData.m_nTestResult[nTargetPos]) {
								strcpy_s(pDvcData.szBinResult, sizeof(pDvcData.szBinResult), g_BinCategory.aCateInfo[i].szSWBin);
								sprintf_s(szHWbin, sizeof(szHWbin), g_BinCategory.aCateInfo[i].szHWBin);
								// Bin Color = Msg Data Pass/Fail
								nBinColor = pDvcData.nBinColor = rData.m_nTestResult[nTargetPos]; /*Pass/Fail 받은 것*/
								strcpy_s(pDvcData.szErrString, sizeof(pDvcData.szErrString), rData.m_cErrorStr[nTargetPos]);
								strcpy_s(pDvcData.szErrorCode, sizeof(pDvcData.szErrorCode), rData.m_cErrorCode[nTargetPos]);
								if (strcmp(pDvcData.szErrorCode, "") == 0)
									strcpy_s(pDvcData.szErrorCode, sizeof(pDvcData.szErrorCode), g_BinCategory.aCateInfo[i].szCode);
								if (strcmp(pDvcData.szTesterHWBin, "") == 0)
									strcpy_s(pDvcData.szTesterHWBin, sizeof(pDvcData.szTesterHWBin), g_BinCategory.aCateInfo[i].szHWBin_Tester);
							}
						}
					}
					// case 3 :: case 2 에서도 판별이 안나온 경우
					if (bChkRegCateInfo == FALSE && (strcmp(szHWbin, "0") == 0 || strcmp(szHWbin, "") == 0)) {
						strcpy_s(pDvcData.szErrString, sizeof(pDvcData.szErrString), nProjectOption == DEF_PROJECT_WATT ? DEF_TEST_RESULT_WRONG_APN : DEF_2D_READ_ERR_DEVICE);
						for (int i = 0; i < g_BinCategory.nIndexCount; i++) {
							if (strcmp(g_BinCategory.aCateInfo[i].szDesc, pDvcData.szErrString) == 0)
							{
								strcpy_s(pDvcData.szBinResult, sizeof(pDvcData.szBinResult), g_BinCategory.aCateInfo[i].szSWBin);
								sprintf_s(szHWbin, sizeof(szHWbin), g_BinCategory.aCateInfo[i].szHWBin);
								strcpy_s(pDvcData.szErrorCode, sizeof(pDvcData.szErrorCode), g_BinCategory.aCateInfo[i].szCode);
								strcpy_s(pDvcData.szTesterHWBin, sizeof(pDvcData.szTesterHWBin), g_BinCategory.aCateInfo[i].szHWBin_Tester);
								nBinColor = pDvcData.nBinColor;
							}
						}
					}
				}

				// Record Test Result log file

				strStartTm.Format("%04d%02d%02d%02d%02d%02d%03d"
					, sysTmTestStart.wYear, sysTmTestStart.wMonth, sysTmTestStart.wDay
					, sysTmTestStart.wHour, sysTmTestStart.wMinute, sysTmTestStart.wSecond, sysTmTestStart.wMilliseconds);
				strEndTm.Format("%04d%02d%02d%02d%02d%02d%03d"
					, timeEnd.wYear, timeEnd.wMonth, timeEnd.wDay
					, timeEnd.wHour, timeEnd.wMinute, timeEnd.wSecond, timeEnd.wMilliseconds);

				//송도 Amkor 요청사항. 대문자 -> 소문자 변환
				CString strBinResult = (rData.m_nTestResult[nTargetPos] == eGoodBin) ? _T("PASS") : _T("FAIL");
				strBinResult.MakeLower();

//				if (nTesterOption == eTester_EachSocket)
				{
					int nStationID = 0;
					CString strStationID = _T("");
					nStationID = (((nSiteX*nSiteY)*m_nUnitIdx) + nX + nY*nSiteX) + 1;
					CTesterStatus cTesterStatus = g_AppleTestIF.GetTesterStatus(nStationID);
					strStationID.Format("%d", cTesterStatus.m_nStationID);

					strTRL.Format(_T("%s,%s,%s,%s,%s,%s,%s")
						, pDvcData.sz2DBarcode
						, strStationID
						, (LPCTSTR)strStartTm, (LPCTSTR)strEndTm
						, strBinResult/*(rData.m_nTestResult[nTargetPos] == eGoodBin ? _T("PASS") : _T("FAIL"))*/
						, rData.m_cErrorStr[nTargetPos], szHWbin);
				}
//				else
 //				{
// 					strTRL.Format("%s,%s,%s,%d,%d,%s,%s,%s,%s,%s,%s,%s"
// 						, pDvcData.sz2DBarcode, rData.m_cDutSn[nTargetPos], szEqpName
// 						, m_nUnitIdx + 1, nX + nY*STATION_MAX_X_SITE + 1
// 						, (LPCTSTR)(cAgentStatus.m_sTesterID)
// 						, (LPCTSTR)(cAgentStatus.m_sCarrierId[(nX + nY*STATION_MAX_X_SITE)])
// 						, (LPCTSTR)strStartTm, (LPCTSTR)strEndTm
// 						, strBinResult/*(rData.m_nTestResult[nTargetPos] == eGoodBin ? _T("PASS") : _T("FAIL"))*/
// 						, pDvcData.szErrString, szHWbin);
//				}

				TWLOG.MakeLog(static_cast<int>(eLogID_PRODUCTCSV), Debug, "", __LINE__, NULL, strTRL);
//				TWLOG.MakeLog(static_cast<int>(eLogID_2DIDCSV), Debug, "", __LINE__, NULL, strTRL);

				// Set Site number
				sprintf_s(pDvcData.szTestSiteNum, sizeof(pDvcData.szTestSiteNum), "%d", m_nUnitIdx);
				pDvcData.cSubSiteNum = CPoint(nX, nY);

				// Set Testing Count
				int nTestCnt = pDvcData.nTestingCnt;

				BOOL bAutoGrrUseMode = g_TaskSystemCtrl.GetAutoGrrUse();

				if (bAutoGrrUseMode != TRUE)
				{	// JAM RATE DATA RECORD
					if (bAutoLoopUseMode != TRUE ) {
						ASSERT(nTestCnt >= eARB_NotTest && nTestCnt <= eARB_3stTestEnd);
						int nDvcTestCnt = nTestCnt;
						if (bAutoRetestUseMode == TRUE && g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoRetest) == eAutoRetest_AB && nTestCnt > eARB_NotTest) {
							nDvcTestCnt -= 1; //AB의 경우는 1stEnd 후에 바로 2stEnd로 바꾸어주기 때문에 빼야함.
						}

						pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].nHwBin = atoi(szHWbin) - 1;
						pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].nPF = nBinColor;
						pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].nSiteNo = atoi(pDvcData.szTestSiteNum);
						pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].cSubSiteNo = pDvcData.cSubSiteNum;

						sprintf_s(pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrStr, sizeof(pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrStr), pDvcData.szErrString);
						sprintf_s(pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrCode, sizeof(pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrCode), pDvcData.szErrorCode);
						sprintf_s(pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szTesterHwBin, sizeof(pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szTesterHwBin), pDvcData.szTesterHWBin);

						int nArtCnt = g_DMCont.m_dmEQP.GN(NDM0_ART_FT_TOTAL + (nTestCnt * 2));
						g_DMCont.m_dmEQP.SN(NDM0_ART_FT_TOTAL + (nTestCnt * 2), ++nArtCnt);

						// push yileld monitoring data
						PushYieldData(nX, nY, nBinColor, nTestCnt);
					}
					else {
						if (bAutoLoopUseMode == TRUE) {
							if (bLoopCancel == FALSE)
								pDvcData.stLoopTestInfo[m_nUnitIdx].testDoneAll();
							else {
								int nMaxTestCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoLoopTestCount);
								for (int nTestCnt = pDvcData.stLoopTestInfo[m_nUnitIdx].nTestCnt[nX][nY]; nTestCnt < nMaxTestCount; nTestCnt++)
								{
									pDvcData.stLoopTestInfo[m_nUnitIdx].testDoneAll();
								}
							}
						}
					}

				}
				else {
					//if (strcmp(pReadData.szErrString, DEF_TEST_RESULT_TP101) != 0)
					//{
					int nFindIdx = 0;
					nFindIdx = pDvcData.findIndexTestSite(pDvcData.szTestSiteNum);
					ASSERT(nFindIdx >= 0);

					//BOOL bSwapOption = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrDeviceSwapUse);
					//if (bSwapOption == TRUE)
					//	pDvcData.stGrrTestInfo[nFindIdx].testDone(nX, nY);	// 해당 Site의 Test 횟수를 증가시켜줌
					//else
					//pDvcData.stGrrTestInfo[nFindIdx].testDoneAll();	// 해당 Site의 Test 횟수를 증가시켜줌
					if (bGrrCancel == FALSE)
						pDvcData.stGrrTestInfo[nFindIdx].testDoneAll();	// 해당 Site의 Test 횟수를 증가시켜줌
					else
					{
						int nMaxTestCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrTestCount);
						for (int nTestCnt = pDvcData.stGrrTestInfo[nFindIdx].nTestCnt[nX][nY]; nTestCnt < nMaxTestCount; nTestCnt++)
						{
							pDvcData.stGrrTestInfo[nFindIdx].testDoneAll();
						}
					}

					// Create DB Data
					_tGRR_DATA stGrrData;
					sprintf(stGrrData.szUnique, "%s", pDvcData.szUniqueID);
					sprintf(stGrrData.sz2DID, "%s", pDvcData.sz2DBarcode);
					sprintf(stGrrData.szSiteNum, "%d", atoi(pDvcData.szTestSiteNum) + 1);
					sprintf(stGrrData.szSubSiteNo, "%d_%d", nX + 1, nY + 1);
					stGrrData.nTestCntNo = pDvcData.stGrrTestInfo[nFindIdx].nTestCnt[nX][nY];
					sprintf(stGrrData.szTestResult, (atoi(pDvcData.szBinResult) == eGoodBin ? "PASS" : "FAIL"));
					sprintf(stGrrData.szResultDescription, "%s", pDvcData.szErrString);

					g_DB.ASYNC_INSERT_GRRDATA_TBL(stGrrData);

					char szGrrSiteHis[MAX_PATH] = { 0, };
					g_DMCont.m_dmContac_Dn.GS(SDM1_SiteGrrPF_History1_1 + m_nUnitIdx*STATION_MAX_PARA + nX + nY * STATION_MAX_X_SITE, szGrrSiteHis, sizeof(szGrrSiteHis));
					CString strGrrSiteHis = _T("");
					strGrrSiteHis.Format("%s%s", szGrrSiteHis, (atoi(pDvcData.szBinResult) == eGoodBin ? "P" : "F"));
					int nlength = strGrrSiteHis.GetLength();
					if (nlength >= MAX_STRING_LENGTH)
						g_DMCont.m_dmContac_Dn.SS(SDM1_SiteGrrPF_History1_1 + m_nUnitIdx*STATION_MAX_PARA + nX + nY * STATION_MAX_X_SITE, (LPSTR)(LPCTSTR)strGrrSiteHis, strGrrSiteHis.GetLength() + 1);
					//}								
				}

				//Test time over 발생시 Auto Retest Skip 안함
				//pDvcData.nTestingCnt = nTestCnt = eARB_3stTestEnd;
				// AutoRetest AAB Timeout관련 Count 추가 [2/4/2021 donghyun.shin]
				
				if (strcmp(pDvcData.szErrString, nProjectOption == DEF_PROJECT_WATT ? DEF_TEST_RESULT_WRONG_APN : DEF_2D_READ_ERR_DEVICE) == 0)
				{
					SetFail2DIDcnt(nX, nY);
					pDvcData.nTestingCnt = nTestCnt = eARB_3stTestEnd;

				}				
				else if (strcmp(pDvcData.szErrString , DEF_HANDLER_ERR_DEVICE) !=0)
				{
					int nCaclTestCnt = g_TaskPressUnit[m_nUnitIdx].GetCalcTestingCnt(nTestCnt, pDvcData);
					pDvcData.nTestingCnt = nTestCnt = nCaclTestCnt;
				}
				// change status

				//pDvcData.nTestingInfo = eTestSite_TestTimeOut;

				// site pass fail history
				int nDataPos = nX + nY*STATION_MAX_X_SITE + (m_nUnitIdx*STATION_MAX_PARA);

				// set fail history
				SetFailHistoryAndPFcnt(nX, nY, nBinColor, nTestCnt);

				// set hw bin history
				SetHwBinHistory(nX, nY, atoi(szHWbin));

				// set last error string
				g_DMCont.m_dmContac_Dn.SS(SDM1_SiteLastDesc1_1 + nDataPos, pDvcData.szErrString, sizeof(pDvcData.szErrString));

				// setting device data
				g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &pDvcData);

				g_TaskPressUnit[m_nUnitIdx].SetSiteDvcTestingInfo(nX, nY, eTestSite_TestEnd);
			}
			else if (GetSocketOnOff(nX, nY) == DEF_SITE_ON) {
				SetNotTestHistory(nX, nY);
			}
		}
	}

	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
	if (nRunMode == eRUN_MODE_ONLINE)
	{
		// Amkor BinSolution
		CString strBinSolutionData = _T("");
		g_TaskPressUnit->SetTextBinSolution(strBinSolutionData, m_nUnitIdx);

		int resultCode = ::sendto(g_UDPSockBinSolution, strBinSolutionData, strBinSolutionData.GetLength(), 0,
			(SOCKADDR*)&g_udpserverAddr, sizeof(g_udpserverAddr));
		g_TaskPressUnit[m_nUnitIdx].MakeLog("[SetTest2DIDHandlerErrorBin] [BinSolution=%s]", strBinSolutionData);
	}
}

void CTaskPressUnit::TesterSiteOnOffCheck(int* pDirSiteOnOff)
{
	ST_DD_DEVICE pDvcData;
	BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
	int nSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nSitecnt = 0;
	for (int nY = 0; nY < nSiteY; nY++)
	{
		for (int nX = 0; nX < nSiteX; nX++)
		{
			pDvcData.clear();
			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &pDvcData);

			//int Addr = nX + nY*STATION_MAX_X_SITE + m_nUnitIdx*STATION_MAX_PARA;
			//int nSiteOnOff = g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + Addr);

			int nSiteOnOff = (bAutoGrrUse == TRUE ? GetSocketOnOffGrr(nX, nY) : GetSocketOnOff(nX, nY));

			
			int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
			if (nTesterOption == eTester_EachSocket)
			{
				if (nSiteOnOff == DEF_SITE_ON && pDvcData.sExist == DEF_EXIST) {	// 제품이 있고 소켓도 on상태여야 테스트 시작 가능 조건이다.
					pDirSiteOnOff[nSitecnt] = DEF_SITE_ON;
				}

				nSitecnt++;
			}
			else
			{
				if (nSiteOnOff == DEF_SITE_ON && pDvcData.sExist == DEF_EXIST) {
					if (nY == eRearTester)
						pDirSiteOnOff[eRearTester]++;
					else
						pDirSiteOnOff[eFrontTester]++;
				}
			}


//   		#if DEF_TESTER_COMM_SPEC_ONE_SOCKET
//   			if (nSiteOnOff == DEF_SITE_ON && pDvcData.sExist == DEF_EXIST) {	// 제품이 있고 소켓도 on상태여야 테스트 시작 가능 조건이다.
//   				pDirSiteOnOff[nSitecnt] = DEF_SITE_ON;
//   		    }
//   
//   			nSitecnt++;
//   		#else
//   			if (nSiteOnOff == DEF_SITE_ON && pDvcData.sExist == DEF_EXIST) {
//   				if (nY == eRearTester)
//   					pDirSiteOnOff[eRearTester]++;
//   				else
//   					pDirSiteOnOff[eFrontTester]++;
//   			}
//   		#endif
		}
	}
}

void CTaskPressUnit::GetSocketStsForEachSocket(BOOL bGrrMode, BOOL *pSocketSts)
{
	int nSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	int nSocketIdx = 0;
	//  0 1 2 3
	//  4 5 6 7
	for (int nY = 0; nY < nSiteY; nY++)
	{
		for (int nX = 0; nX < nSiteX; nX++)
		{
			int nTarget = (bGrrMode == TRUE ? NDM11_GRR_SocketOnOff_Site1_1 : NDM11_SocketOnOff_Site1_1) + nX + nY*STATION_MAX_X_SITE + (m_nUnitIdx *STATION_MAX_PARA);
			if (g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_ON || g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_OFF_RESERVATION)
			{
				pSocketSts[nSocketIdx] = DEF_ON;
			}

			nSocketIdx++;
		}
	}
}

void CTaskPressUnit::GetSocketAuditOnOffForEachSocket(BOOL *pSocketSts)
{
	int nSocketIdx = 0;
	int nSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	for (int nY = 0; nY < nSiteY; nY++)
	{
		for (int nX = 0; nX < nSiteX; nX++)
		{
			int nTarget = NDM11_Audit_SocketOnOff_Site1_1 + nX + nY*STATION_MAX_X_SITE + (m_nUnitIdx *STATION_MAX_PARA);
			if (g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_ON || g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_OFF_RESERVATION)
			{
				pSocketSts[nSocketIdx] = DEF_ON;
			}
		}
	}
}

int CTaskPressUnit::GetDirDualSocketOnCnt(BOOL bGrrMode,int nDirTester)
{
	int nOnCnt = 0;

	int nSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	if (nTesterOption == eTester_EachSocket)
	{
		int nDutCnt = 0;
		for (int nY = 0; nY < nSiteY; nY++)
		{
			for (int nX = 0; nX < nSiteX; nX++)
			{
				int nTarget = (bGrrMode == TRUE ? NDM11_GRR_SocketOnOff_Site1_1 : NDM11_SocketOnOff_Site1_1) + nX + nY*STATION_MAX_X_SITE + (m_nUnitIdx *STATION_MAX_PARA);
				if (g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_ON || g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_OFF_RESERVATION || g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_OFF_RESERVATION_YIELD)
				{
					//if (nDirTester == nDutCnt)
					{
						nOnCnt++;
					}
				}
				//	nDutCnt++;
			}
		}
	}
	else
	{
		for (int nX = 0; nX < nSiteX; nX++)
		{
			for (int nY = 0; nY < nSiteY; nY++)
			{
				int nTarget = (bGrrMode == TRUE ? NDM11_GRR_SocketOnOff_Site1_1 : NDM11_SocketOnOff_Site1_1) + nX + nY*STATION_MAX_X_SITE + (m_nUnitIdx *STATION_MAX_PARA);
				if (g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_ON || g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_OFF_RESERVATION)
					if (nDirTester == nY)
					{
						nOnCnt++;
					}
			}
		}
	}

	return nOnCnt;

// #if DEF_TESTER_COMM_SPEC_ONE_SOCKET
// 
// 	int nDutCnt = 0;
// 	for (int nY = 0; nY < nSiteY; nY++)
// 	{
// 		for (int nX = 0; nX < nSiteX; nX++)
// 		{
// 			int nTarget = (bGrrMode == TRUE ? NDM11_GRR_SocketOnOff_Site1_1 : NDM11_SocketOnOff_Site1_1) + nX + nY*STATION_MAX_X_SITE + (m_nUnitIdx *STATION_MAX_PARA);
// 			if (g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_ON || g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_OFF_RESERVATION)
// 			{
// 				//if (nDirTester == nDutCnt)
// 				{
// 					nOnCnt++;
// 				}
// 			}
// 
// 		//	nDutCnt++;
// 		}
// 	}
// #else
// 	for (int nX = 0; nX < nSiteX; nX++)
// 	{
// 		for (int nY = 0; nY < nSiteY; nY++)
// 		{
// 			int nTarget = (bGrrMode == TRUE ? NDM11_GRR_SocketOnOff_Site1_1 : NDM11_SocketOnOff_Site1_1) + nX + nY*STATION_MAX_X_SITE + (m_nUnitIdx *STATION_MAX_PARA);
// 			if (g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_ON || g_DMCont.m_dmHandlerOpt.GN(nTarget) == DEF_SITE_OFF_RESERVATION)
// 				if (nDirTester == nY)
// 				{
// 					nOnCnt++;
// 				}
// 		}
// 	}
// #endif

//	return nOnCnt;
}

BOOL CTaskPressUnit::PressInCleanDeviceCheck(BOOL bGrr)
{
	int nSiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nInformation[eMaxTestSiteTestCount] = { 0, };

	ST_DD_DEVICE tSiteData;
	for (int nY = 0; nY < nSiteCntY; nY++)
	{
		for (int nX = 0; nX < nSiteCntX; nX++)
		{
			tSiteData.clear();
			GetSiteDeviceData(&tSiteData, nX, nY);
			int nOnOff = (bGrr == FALSE ? GetSocketOnOff(nX, nY) : GetSocketOnOffGrr(nX, nY));
	
			if (tSiteData.sExist == eDeviceData_Exist && strcmp(tSiteData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0)/* && nOnOff == DEF_SITE_ON*/
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

void CTaskPressUnit::SetTestHandlerErrorBin(ST_DD_DEVICE& pDvcData, ResultData& rData, int& nTargetPos, char* szHWbin,int& nBinColor,CString& strResult)
{
	BOOL bChkRegCateInfo = FALSE;
	BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);

	strcpy_s(pDvcData.szErrString, sizeof(pDvcData.szErrString), rData.m_cErrorStr[nTargetPos]);

	if (strcmp(pDvcData.szErrString, "") == 0 || strcmp(rData.m_cErrorStr[nTargetPos], "") == 0) {
		strcpy_s(pDvcData.szErrString, sizeof(pDvcData.szErrString), DEF_TEST_TIME_OUT_ERR_DEVICE);
		strcpy_s(pDvcData.szErrorCode, sizeof(pDvcData.szErrorCode), "98");
		strcpy_s(pDvcData.szTesterHWBin, sizeof(pDvcData.szTesterHWBin), "98");
	}
	


	// case 1 :: category에 등록이 되어있는 경우
	for (int i = 0; i < g_BinCategory.nIndexCount; i++) {
		if (strcmp(g_BinCategory.aCateInfo[i].szDesc, pDvcData.szErrString) == 0)
		{
			strcpy_s(pDvcData.szBinResult, sizeof(pDvcData.szBinResult), g_BinCategory.aCateInfo[i].szSWBin);
			sprintf_s(szHWbin, sizeof(szHWbin), g_BinCategory.aCateInfo[i].szHWBin);
			strcpy_s(pDvcData.szErrorCode, sizeof(pDvcData.szErrorCode), g_BinCategory.aCateInfo[i].szCode);
			strcpy_s(pDvcData.szTesterHWBin, sizeof(pDvcData.szTesterHWBin), g_BinCategory.aCateInfo[i].szHWBin_Tester);
			if (strcmp(pDvcData.szErrString, DEF_PASS_DEVICE) == 0)
			{
				pDvcData.nBinColor = eGoodBin;
				strResult = DEF_PASS_DEVICE;
			}
				
			nBinColor = pDvcData.nBinColor;
			bChkRegCateInfo = TRUE;
		}
	}
	// case 2 :: category에 등록이 안되어 있는 경우
	if (bChkRegCateInfo == FALSE) {
		for (int i = 0; i < g_BinCategory.nIndexCount; i++) {
			if (atoi(g_BinCategory.aCateInfo[i].szSWBin) == rData.m_nTestResult[nTargetPos]) {
				strcpy_s(pDvcData.szBinResult, sizeof(pDvcData.szBinResult), g_BinCategory.aCateInfo[i].szSWBin);
				sprintf_s(szHWbin, sizeof(szHWbin), g_BinCategory.aCateInfo[i].szHWBin);
				strcpy_s(pDvcData.szErrorCode, sizeof(pDvcData.szErrorCode), rData.m_cErrorCode[nTargetPos]);
				if (strcmp(pDvcData.szErrorCode, "") == 0)
					strcpy_s(pDvcData.szErrorCode, sizeof(pDvcData.szErrorCode), g_BinCategory.aCateInfo[i].szCode);
				if (strcmp(pDvcData.szTesterHWBin, "") == 0)
					strcpy_s(pDvcData.szTesterHWBin, sizeof(pDvcData.szTesterHWBin), g_BinCategory.aCateInfo[i].szHWBin_Tester);
				// Bin Color = Msg Data Pass/Fail
				nBinColor = pDvcData.nBinColor = rData.m_nTestResult[nTargetPos]; /*Pass/Fail 받은 것*/
				strcpy_s(pDvcData.szErrString, sizeof(pDvcData.szErrString), rData.m_cErrorStr[nTargetPos]);
			}
		}
	}
	// case 3 :: case 2 에서도 판별이 안나온 경우
	if (bChkRegCateInfo == FALSE && (strcmp(szHWbin, "0") == 0 || strcmp(szHWbin, "") == 0 )) {
		strcpy_s(pDvcData.szErrString, sizeof(pDvcData.szErrString), DEF_TEST_TIME_OUT_ERR_DEVICE);
		for (int i = 0; i < g_BinCategory.nIndexCount; i++) {
			if (strcmp(g_BinCategory.aCateInfo[i].szDesc, pDvcData.szErrString) == 0)
			{
				strcpy_s(pDvcData.szBinResult, sizeof(pDvcData.szBinResult), g_BinCategory.aCateInfo[i].szSWBin);
				strcpy_s(pDvcData.szErrorCode, sizeof(pDvcData.szErrorCode), g_BinCategory.aCateInfo[i].szCode);
				strcpy_s(pDvcData.szTesterHWBin, sizeof(pDvcData.szTesterHWBin), g_BinCategory.aCateInfo[i].szHWBin_Tester);
				sprintf_s(szHWbin, sizeof(szHWbin), g_BinCategory.aCateInfo[i].szHWBin);
				nBinColor = pDvcData.nBinColor;
			}
		}
	}
}

void CTaskPressUnit::SetFail2DIDcnt(int nX, int nY)
{
	int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	int nPos = nX + nY*STATION_MAX_X_SITE + m_nUnitIdx*(nSiteDivX*nSiteDivY);
	// Product Count
	int n2DIDCnt = g_DMCont.m_dmContac_Up.GN(NDM2_2DIDFailBinCnt_Press1_1 + nPos);
	g_DMCont.m_dmContac_Up.SN(NDM2_2DIDFailBinCnt_Press1_1 + nPos, ++n2DIDCnt);
		
}

void CTaskPressUnit::SetPressReqTable(BOOL bState, int nTblType)
{
	if (nTblType == CTaskLoadTable::eMaxTblType) {
		for (int i = 0; i < CTaskLoadTable::eMaxTblType; i++) {
			m_bPressReqState[i] = bState;
		}
	}
	else {
		m_bPressReqState[nTblType] = bState;
	}	
}

BOOL CTaskPressUnit::GetPressReqTable(int nTblType)
{
	return m_bPressReqState[nTblType];
}

void CTaskPressUnit::SetTextBinSolution(CString& strBin, int nStationNum)
{
	int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	char szEqpName[128] = { 0, };
	g_DMCont.m_dmEQP.GS(SDM0_HANDER_ID, szEqpName, sizeof(szEqpName));
	// Host Name
	strBin += szEqpName;
	strBin += ",";
	ST_DD_DEVICE pDvcData;
	int nCount = 0;
	for(int nStation = 0; nStation < eMaxPressUnitCount; nStation++)
	{	
		for (int nY = 0; nY < nSiteDivY; nY++)
		{
			for (int nX = 0; nX < nSiteDivX; nX++)
			{
				pDvcData.clear();
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1 + nStationNum, nX, nY, &pDvcData);
				if (nStation == nStationNum)
				{
					int nSiteOnOff = g_TaskPressUnit[nStationNum].GetSocketOnOff(nX, nY);

					if (nSiteOnOff == DEF_SITE_ON && pDvcData.sExist == DEF_EXIST)// Device가 있고, 소켓도 on상태여야 테스트 시작 가능 조건이다.
					{	
						// Pass Bin Test Count 관계 없이 항상 보고
						// Fail Bin은 Final Test에만 보고 
						int nBinColor = GetBinColor(pDvcData);
						switch (nBinColor)
						{
						case eFailBin:
							{
								if (pDvcData.nTestingCnt == eARB_3stTestEnd) 
								{
									strBin += DEF_BIN_SOLUTION_FAIL_SW_BIN;
								}
							}
							break;
						case eGoodBin:
							{
								strBin += pDvcData.szBinResult;
							}
							break;
						case eOtherFailBin: //일반 FAIL이 아닌 Category에 등록 되어 있는 여러 종류에 FAIL임. SW Result 번호로 보낸다.
							{
								if (pDvcData.nTestingCnt == eARB_3stTestEnd)
								{
									strBin += pDvcData.szBinResult;
								}					
							}break;
						}						
					}
				}
				nCount++;
				if (nCount != (eMaxPressUnitCount) * (nSiteDivX * nSiteDivY))
				{
					strBin += ",";
				}				
			}
		}
	}
	//마침표
	strBin += _T("*\0");
}

int CTaskPressUnit::GetBinColor(ST_DD_DEVICE pDvcData)
{
	int nBinColor =0;
	if (pDvcData.nBinColor == eFailBin)
	{
		if (strcmp(pDvcData.szBinResult, "0") == 0 || // "FAIL" SW Bin은 0,-1,공백을 제외한 나머지 숫자만 가능
			strcmp(pDvcData.szBinResult, "-1") == 0 ||
			strcmp(pDvcData.szBinResult, "") == 0)
		{
			nBinColor = eFailBin;
		}
		else {
			nBinColor = eOtherFailBin;
		}
	}	
	else {
		nBinColor = eGoodBin;
	}

	return nBinColor;
}

int CTaskPressUnit::CalYieldMonitoring()
{
	g_DMCont.m_dmEQP.SB(BDM0_TEST_STATUS_SITE1 + m_nUnitIdx, FALSE);    // Set testing status off
	g_DMCont.m_dmEQP.SB(BDM0_PRESS_CONTACT_STATUS_SITE1 + m_nUnitIdx, FALSE); // Set Contact status off
	BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
	BOOL bAutoLoopUse = g_TaskSystemCtrl.GetAutoLoopUse();
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	if (bAutoGrrUse == FALSE && bAutoLoopUse == FALSE)
	{
		std::vector<CPoint> vAlarmSet;
		g_TaskSystemCtrl.CalcYieldContiFailMonitoring(m_nUnitIdx, vAlarmSet);
		g_TaskSystemCtrl.CalcYieldSampleMonitoring(m_nUnitIdx, vAlarmSet);
		g_TaskSystemCtrl.CalcYield1stFailMonitoring(m_nUnitIdx, vAlarmSet);
		g_TaskSystemCtrl.CalcYieldContiFailMonitoring_Station(m_nUnitIdx, vAlarmSet);

		//g_TaskSystemCtrl.CalcYieldVarianceMonitoring();
	}
	return ERR_NO_ERROR;
}


void CTaskPressUnit::SaveFileHeightCalData(double dMeasureData)
{
	char szCokName[512] = { 0, };
	ST_COK_INFO     stCokData, stBeforeCokData;
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
	g_CokMgr.LoadCokInfo(szCokName, stCokData);

	stBeforeCokData = stCokData;

	stCokData.dPressHeightCalData[m_nUnitIdx] = dMeasureData / 1000.0;

	g_CokMgr.SaveCokInfo(szCokName, &stCokData);

	std::vector<std::pair<CString, std::pair<double, double>>> vData;
	stBeforeCokData.IsSamePressHeightCalData(stCokData, m_nUnitIdx, vData);

	if (!(vData.empty()))
	{
		for (int i = 0; i < (int)vData.size(); i++)
		{
			MakeLog("[%s Auto Height Cal Data Changed][BEFORE = %.2f, AFTER = %.2f]", vData[i].first, vData[i].second.first, vData[i].second.second); //ex) Press#1 Auto Height Cal Data Changed
		}
	}
}

CString CTaskPressUnit::GetAABString(int nTestCount)
{
	CString strAABString = _T("");
	switch (nTestCount)
	{
		case eAutoRetestBin::eARB_NotTest: {;}break;
		case eAutoRetestBin::eARB_1stTestEnd: {;}break;
		case eAutoRetestBin::eARB_2stTestEnd: 
		case eAutoRetestBin::eARB_3stTestEnd: 
		{
			strAABString = "<AAB> ";
		}break;

	default:
		break;
	}
	return strAABString;
}