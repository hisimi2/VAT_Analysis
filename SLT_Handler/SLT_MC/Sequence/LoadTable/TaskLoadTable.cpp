#include "StdAfx.h"
#include "TaskLoadTable.h"
#include "ConfigData.h"
#include "DlgAxisActionMove.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*=============================================================================
  AUTHOR   : 나명진  [2015-08-25]
  NAME     :
  PARAMS   :
  RETURN   :
  COMMENTS :
==============================================================================*/
CTaskLoadTable::CTaskLoadTable( void )
{
	m_vRec2d.clear();
	m_vRec2d.reserve( 60000 );

	m_nTableIdx = 0;

	m_nWorkReserve = RID_INIT_IDLE_RESERVE;
	m_bInitialized = FALSE;

// 	for( i = 0; i < eMaxAxisCount; i++ ) {
// 		m_dMotorErrRange[ i ] = MOTOR_INPOSITION;
// 	}
	m_dMotorErrRange[eAxisY] = MOTOR_INPOSITION;
	m_dMotorErrRange[eAxisRotate] = 0.5;

	for (int i = 0; i < _countof(m_nLoadUnloadTablePocket); i++)
	{
		m_nLoadUnloadTablePocket[i] = 0;
	}
		
	// DI
	m_mapDI.SetAt( xLOAD_TABLE_DEVICE1_SEN, "LOAD_TABLE_DEVICE1_SEN" );
	m_mapDI.SetAt( xLOAD_TABLE_DEVICE2_SEN, "LOAD_TABLE_DEVICE2_SEN" );
	m_mapDI.SetAt( xLOAD_TABLE_DEVICE3_SEN, "LOAD_TABLE_DEVICE3_SEN" );
	m_mapDI.SetAt( xLOAD_TABLE_DEVICE4_SEN, "LOAD_TABLE_DEVICE4_SEN" );
	m_mapDI.SetAt( x2D_BARCODE_LEFT_SEN,	"2D_BARCODE_LEFT_SEN" );
	m_mapDI.SetAt( x2D_BARCODE_RIGHT_SEN,	"2D_BARCODE_RIGHT_SEN" );
	m_mapDI.SetAt(xLOAD_TABLE_ROTATE1_SEN,"LOAD_TABLE_ROTATOR_1_SEN");
	m_mapDI.SetAt(xLOAD_TABLE_ROTATE2_SEN,"LOAD_TABLE_ROTATOR_2_SEN");
	m_mapDI.SetAt(xLOAD_TABLE_ROTATE_STOP_SEN, "LOAD_TABLE_ROTATOR_STOP_SEN");
	m_mapDI.SetAt(xLOAD_TABLE_VIB_FOR_SEN, "LOAD_TABLE_VIB_FOR_SEN");
	m_mapDI.SetAt(xLOAD_TABLE_VIB_BACK_SEN, "LOAD_TABLE_VIB_BACK_SEN");

	// DO

	// DO
	m_mapDO.SetAt( y2D_BARCODE_LEFT, "2D_BARCODE_LEFT" );
	m_mapDO.SetAt( y2D_BARCODE_RIGHT, "2D_BARCODE_RIGHT" );
	m_mapDO.SetAt(yVIBRATOR_FOR, "VIBRATOR_FOR");
	m_mapDO.SetAt(yVIBRATOR_BACK, "VIBRATOR_BACK");

	// motor
	m_mapMotor.SetAt( eAxisY, "Axis Y" );
	m_mapMotor.SetAt(eAxisRotate, "Axis Rotate");

	m_nTableRowStep = 0;
	m_nTableColStep = 0;
	m_nTestPPPrePos = -1;
	::InitializeCriticalSection( m_cs );
	m_bLoadTableSkip = FALSE;
	m_bLoadTableAlarmMove = FALSE;
	m_nTblCurPos = -1;
	m_bLoadTblBuffUse = FALSE;

	m_bTriggerInitial = FALSE;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-08-25]
  NAME     :
  PARAMS   :
  RETURN   :
  COMMENTS :
==============================================================================*/
CTaskLoadTable::~CTaskLoadTable( void )
{
	::DeleteCriticalSection( m_cs );
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-08-25]
  NAME     :
  PARAMS   :
  RETURN   :
  COMMENTS :
==============================================================================*/
int CTaskLoadTable::Initialize( int nTableIdx, char* szDriverFileName )
{
	m_nTableIdx = nTableIdx;

	CConfigData ConfigData( szDriverFileName );
	char szSection[128] = { 0, };
	
	//int nProjectOption = g_DMCont.m_dmEQP.GN(NDM11_Handler_Project_Name);
	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);

	if (nBoardType == eBoardDir_Forward)
	{
		if (m_nTableIdx == eLoadTbl_1 || m_nTableIdx == eLoadTbl_4)
		{
			char row1[4] = { 'U', 'U', 'U', 'U' };
			char row2[4] = { 'B', 'B', 'B', 'B' };
			char row3[4] = { 'U', 'U', 'U', 'U' };
			char row4[4] = { 'B', 'B', 'B', 'B' };
			m_LoadTbl_pocket_setting.add_row(row1, _countof(row1));
			m_LoadTbl_pocket_setting.add_row(row2, _countof(row2));
			m_LoadTbl_pocket_setting.add_row(row3, _countof(row3));
			m_LoadTbl_pocket_setting.add_row(row4, _countof(row4));



		}
		else
		{
			char row1[4] = { 'L', 'L', 'L', 'L' };
			char row2[4] = { 'B', 'B', 'B', 'B' };
			char row3[4] = { 'L', 'L', 'L', 'L' };
			char row4[4] = { 'B', 'B', 'B', 'B' };
			m_LoadTbl_pocket_setting.add_row(row1, _countof(row1));
			m_LoadTbl_pocket_setting.add_row(row2, _countof(row2));
			m_LoadTbl_pocket_setting.add_row(row3, _countof(row3));
			m_LoadTbl_pocket_setting.add_row(row4, _countof(row4));

			if (m_nTableIdx == eLoadTbl_2)
				SetPocketLocNum(_eCCW);
			else if (m_nTableIdx == eLoadTbl_3)
				SetPocketLocNum(_eCW);

		}

		m_nLoadUnloadTablePocket[DEF_REAR] = eLoadTablePocketY1;
		m_nLoadUnloadTablePocket[DEF_FRONT] = eLoadTablePocketY3;
	}
	else
	{
		if (m_nTableIdx == eLoadTbl_1 || m_nTableIdx == eLoadTbl_4)
		{
			char row1[4] = { 'B', 'B', 'B', 'B' };
			char row2[4] = { 'U', 'U', 'U', 'U' };
			char row3[4] = { 'B', 'B', 'B', 'B' };
			char row4[4] = { 'U', 'U', 'U', 'U' };
			m_LoadTbl_pocket_setting.add_row(row1, _countof(row1));
			m_LoadTbl_pocket_setting.add_row(row2, _countof(row2));
			m_LoadTbl_pocket_setting.add_row(row3, _countof(row3));
			m_LoadTbl_pocket_setting.add_row(row4, _countof(row4));
		}
		else
		{
			char row1[4] = { 'B', 'B', 'B', 'B' };
			char row2[4] = { 'L', 'L', 'L', 'L' };
			char row3[4] = { 'B', 'B', 'B', 'B' };
			char row4[4] = { 'L', 'L', 'L', 'L' };
			m_LoadTbl_pocket_setting.add_row(row1, _countof(row1));
			m_LoadTbl_pocket_setting.add_row(row2, _countof(row2));
			m_LoadTbl_pocket_setting.add_row(row3, _countof(row3));
			m_LoadTbl_pocket_setting.add_row(row4, _countof(row4));


			if (m_nTableIdx == eLoadTbl_2)
				SetPocketLocNum(_eCW);
			else if(m_nTableIdx == eLoadTbl_3)
				SetPocketLocNum(_eCCW);
			

		}
		m_nLoadUnloadTablePocket[DEF_REAR] = eLoadTablePocketY2;
		m_nLoadUnloadTablePocket[DEF_FRONT] = eLoadTablePocketY4;
	}
	
	

	// LOG File Name
	char szLogFileName[ 512 ] = { 0, };
	char szBuff[ 512 ] = { 0, };
	char szDefaultValue[ 128 ] = { 0, };

	memset( szSection, 0x00, sizeof( szSection ) );
	sprintf_s( szSection, sizeof( szSection ), "TASK LOADING TABLE %d CONFIG", nTableIdx + 1 );

	memset( szDefaultValue, 0x00, sizeof( szDefaultValue ) );
	sprintf_s( szDefaultValue, sizeof( szDefaultValue ), "TaskLoadTable_%d.log", nTableIdx + 1 );

	ConfigData.GetString( szSection, "Log File Name", szDefaultValue, szBuff );
	sprintf_s( szLogFileName, sizeof( szLogFileName ), "%s%s", SZ_LOG_FILE_PATH, szBuff );
	m_Logger.SetFileName( szLogFileName );

	// Digital Input
	memset( szSection, 0x00, sizeof( szSection ) );
	sprintf_s( szSection, sizeof( szSection ), "TASK LOADING TABLE %d CONFIG - DI", nTableIdx + 1 );
	int nDiCount = m_mapDI.GetCount();
	for( int i = 0; i < nDiCount; i++ ) {
		char szKey[ 128 ] = { 0, };
		CString strName;
		m_mapDI.Lookup( i, strName );
		sprintf_s( szKey, sizeof( szKey ), "%s", strName.GetString() );
		int nIdx = ConfigData.GetInt( szSection, szKey, "0" );
		m_vX.push_back( nIdx );
	}

	memset( szSection, 0x00, sizeof( szSection ) );
	sprintf_s( szSection, sizeof( szSection ), "TASK LOADING TABLE %d CONFIG - DO", nTableIdx + 1 );
	int nDoCount = m_mapDO.GetCount();
	for( int i = 0; i < nDoCount; i++ ) {
		char szKey[ 128 ] = { 0, };
		CString strName;
		m_mapDO.Lookup( i, strName );
		sprintf_s( szKey, sizeof( szKey ), "%s", strName.GetString() );
		int nIdx = ConfigData.GetInt( szSection, szKey, "0" );
		m_vY.push_back( nIdx );
	}

	// Motor
	memset( szSection, 0x00, sizeof( szSection ) );
	sprintf_s( szSection, sizeof( szSection ), "TASK LOADING TABLE %d CONFIG - MOTOR", nTableIdx + 1 );
	int nMotorCount = m_mapMotor.GetCount();
	for( int i = 0; i < nMotorCount; i++ ) {
		char szKey[ 128 ] = { 0, };
		CString strName;
		m_mapMotor.Lookup( i, strName );
		sprintf_s( szKey, sizeof( szKey ), "%s", strName.GetString() );
		int nIdx = ConfigData.GetInt( szSection, szKey, "0" );
		OneAxis* pMotor = g_Motor[ nIdx ];
		m_vMotor.push_back( pMotor );
	}

	return CBaseTaskManager::Initialize();
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-08-25]
  NAME     :
  PARAMS   :
  RETURN   :
  COMMENTS :
==============================================================================*/
void CTaskLoadTable::Finalize()
{
	CBaseTaskManager::Finalize();
}

//==============================================================================
// 도착한 명령을 큐에서 꺼내어 실행 시킨다.
//==============================================================================
void CTaskLoadTable::ThreadProc1()
{
	SetThreadInterval( 0, 50 );
	while( GetThreadAliveFlag( 0 ) ) {
		if( GetPauseFlag( 0 ) ) {
			NEXT;
			continue;
		}
		for( int i = 0; i < MAX_THREAD_SCAN_COUNT; i++ ) {
			CBaseTaskManager::SeqCmdRunning();
		}
		NEXT;
	}
}

void CTaskLoadTable::OnOperatorCommand( int nOperatorCommand, WPARAM wParam, LPARAM lParam )
{
	switch( nOperatorCommand )
	{
		case TWMB_ID_SKIP:
		case TWMB_ID_P_SKIP:
		{
			m_bLoadTableSkip = TRUE;
			SetProcessCommand( base_seq_cmd::eProcCmd_Skip );
		}break;
		case TWMB_ID_AXIS_MOVE:
		{
			int nInterlockErr = g_TaskSystemCtrl.CheckManualable();
			if (nInterlockErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
				return;
			}

			m_bLoadTableAlarmMove = TRUE;
			CDlgAxisActionMove dlg;
			dlg.m_nStartPoint = eMANU_OPER_LD_TBL1 + m_nTableIdx;
			dlg.DoModal();
		}break;
	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-08-25]
  NAME     :
  PARAMS   :
  RETURN   :
  COMMENTS :
==============================================================================*/
int CTaskLoadTable::OnEvent( const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList/*=NULL*/ )
{
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-08-25]
  NAME     :
  PARAMS   :
  RETURN   :
  COMMENTS :
			 Load Table #1,#4 - x,y
			 Load Table #2,#3 - y
==============================================================================*/
int CTaskLoadTable::do_MovePos( int nLoc, int nAxisIdx, int nSeqSpdRate/*=100*/, double dOffset/*=0.0*/, int nStep /*= 0*/)
{
	ASSERT( nSeqSpdRate >= 0 && nSeqSpdRate <= 100 );
	ASSERT(nAxisIdx >= 0 && nAxisIdx  < eMaxAxisCount);

	int nErr = 0;
	int nAxisCount = m_vMotor.size();

	// Y, Rotate
	OneAxis* pMotor = ( OneAxis* )m_vMotor[nAxisIdx];
	double dTPos = GetTeachPos( nLoc, nAxisIdx);
	double dRetSpd = 0;
	int    nRetAcc = 0, nRetDcc = 0;
	/*User Spd, Acc, Dcc & Seq Spd, Acc, Dcc 를 가지고 계산*/
	//int nIndexY[eMaxLoadTblCount] = {NDM11_SPD_LoadTbl1_Y, NDM11_SPD_LoadTbl2_Y, NDM11_SPD_LoadTbl3_Y, NDM11_SPD_LoadTbl4_Y};
// 	int nIndexYSpd[ eMaxLoadTblCount ] = { NDM3_SPD_LoadTbl1_Y, NDM3_SPD_LoadTbl2_Y };
// 	int nIndexYAcc[ eMaxLoadTblCount ] = { NDM3_ACC_LoadTbl1_Y, NDM3_ACC_LoadTbl2_Y };
// 	int nIndexYDcc[ eMaxLoadTblCount ] = { NDM3_DCC_LoadTbl1_Y, NDM3_DCC_LoadTbl2_Y };

	//rotator spd,acc,dcc 추가
	int nIndexSpd[eMaxTblCount][eMaxAxisCount] = { { NDM3_SPD_LoadTbl1_Y, NDM3_SPD_LoadTbl1_Rotate },
												   { NDM3_SPD_LoadTbl2_Y, NDM3_SPD_LoadTbl2_Rotate },
												   { NDM3_SPD_LoadTbl3_Y, NDM3_SPD_LoadTbl3_Rotate },
												   { NDM3_SPD_LoadTbl4_Y, NDM3_SPD_LoadTbl4_Rotate }};
	int nIndexAcc[eMaxTblCount][eMaxAxisCount] = { { NDM3_ACC_LoadTbl1_Y, NDM3_ACC_LoadTbl1_Rotate },
												   { NDM3_ACC_LoadTbl2_Y, NDM3_ACC_LoadTbl2_Rotate },
												   { NDM3_ACC_LoadTbl3_Y, NDM3_ACC_LoadTbl3_Rotate },
												   { NDM3_ACC_LoadTbl4_Y, NDM3_ACC_LoadTbl4_Rotate }};
	int nIndexDcc[eMaxTblCount][eMaxAxisCount] = { { NDM3_DCC_LoadTbl1_Y, NDM3_DCC_LoadTbl1_Rotate },
												   { NDM3_DCC_LoadTbl2_Y, NDM3_DCC_LoadTbl2_Rotate },
												   { NDM3_DCC_LoadTbl3_Y, NDM3_DCC_LoadTbl3_Rotate },
												   { NDM3_DCC_LoadTbl4_Y, NDM3_DCC_LoadTbl4_Rotate }};

//	int nIndexAcc[eMaxAxisCount] = { NDM5_ACC_TrayPP_X, NDM5_ACC_TrayPP_Y, NDM5_ACC_TrayPP_Z,NDM5_ACC_TrayPP_ROTATOR };
//	int nIndexDcc[eMaxAxisCount] = { NDM5_DCC_TrayPP_X, NDM5_DCC_TrayPP_Y, NDM5_DCC_TrayPP_Z,NDM5_DCC_TrayPP_ROTATOR };





	double dRateSpd = ( double )g_DMCont.m_dmShuttle.GN( nIndexSpd[m_nTableIdx][nAxisIdx] );
	int    nRateAcc = g_DMCont.m_dmShuttle.GN(nIndexAcc[ m_nTableIdx ][nAxisIdx] );
	int    nRateDcc = g_DMCont.m_dmShuttle.GN(nIndexDcc[ m_nTableIdx ][nAxisIdx]);

	double dMtSpd = pMotor->GetSpeed();
	double dMtAcc = pMotor->GetAcc();
	double dMtDcc = pMotor->GetDcc();

	if (nAxisIdx == eAxisY) {
		if (dMtSpd > 1600000.0 && dOffset != 0.0 && nLoc != LOC_IDX_TBL_LD_SIDE) //2D Barcode 속도 조절
		{
			dMtSpd = 1600000.0;
		}
	}

	//2d speed 고정.
// 	if (GetTableUsage() == eLoadType && g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use) == TRUE && nAxisIdx == CTaskLoadTable::eAxisY) {
// 		std::vector<int> vLocList;
// 		WhereIsMotorPos(vLocList, CTaskLoadTable::eAxisAll);
// 		if()
// 		//double dRateSpd = (double)g_DMCont.m_dmShuttle.GN(NDM3_SPD_LoadTbl1_Y);
// 		double d2dSpd = nIndexSpd[m_nTableIdx][nAxisIdx];
// 		if (d2dSpd > 50) {
// 			dRateSpd = 50;
// 		}
// 	}

	dRetSpd = ( dMtSpd * ( dRateSpd / 100.0 ) ) * ( nSeqSpdRate / 100.0 );
	nRetAcc = ( int )( dMtAcc - ( dMtAcc*0.75 )*( nRateAcc / 100.0 ) ); // 설정되어 있는 값에서 spd rate가 높을 수록 acc/dcc를 감소 시켜준다. 가장 빠를 때가 설정 속도의 95%뺀 acc/dcc로 함.
	nRetDcc = ( int )( dMtDcc - ( dMtDcc*0.75 )*( nRateDcc / 100.0 ) );

	if( nRetAcc < 200 ) nRetAcc = 200;
	if( nRetDcc < 200 ) nRetDcc = 200;

	//Manual 일 경우, Step 0으로 고정.
	if (g_TaskLoadTbl[m_nTableIdx].GetEqpCtrlMode() == eEqpCtrlMode_Manual){
		nStep = 0;
	}


	double dMovePos = 0.0;
	if (nAxisIdx == eAxisY)
	{
		//dMovePos = dTPos + dOffset + (nRowStep* nTbl_2Pitch_Y);
		BOOL bTableVaccumChk = TableVacuumCheckMem();
		
		if (bTableVaccumChk == FALSE)
		{
			if (nLoc == LOC_IDX_TBL_LD_SIDE)
			{
				int nTbl_2Pitch_Y = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_Y) * 2;	//현재 Load Table Y는 2Pitch
				dMovePos = dTPos + dOffset + (nStep* nTbl_2Pitch_Y);
			}
			else
			{
				int nTbl_Pitch_X = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_X);
				dMovePos = dTPos + dOffset + (nStep* nTbl_Pitch_X);
			}
		}
		else {
			if (nLoc == LOC_IDX_TBL_LD_SIDE)
			{// vacuum check step
				int nTbl_Pitch_Y = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_Y);
				dMovePos = dTPos + dOffset + (nStep* nTbl_Pitch_Y);
			}
		}

	}
	else //nAxisIdx == eAxisRotate
	{
		dMovePos = dTPos + dOffset;
	}

	nErr = pMotor->Move(dMovePos, dRetSpd, nRetAcc, nRetDcc );

	return nErr;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-08-25]
  NAME     :
  PARAMS   :
  RETURN   :
  COMMENTS :
==============================================================================*/
int CTaskLoadTable::Interlock_MovePos( int nLoc, int a_nAxisIdx )
{
	int nErr = 0;

	return nErr;
}


// Stacker Elevator

/*=============================================================================
  AUTHOR   : 나명진  [2015-08-25]
  NAME     :
  PARAMS   :
  RETURN   :
  COMMENTS :
==============================================================================*/
OneAxis* CTaskLoadTable::GetAxis( int nAxisIdx )
{
	ASSERT( ( nAxisIdx >= 0 ) && ( nAxisIdx < CTaskLoadTable::eMaxAxisCount ) );

	OneAxis* pMotor = NULL;

	if( m_vMotor.size() == 2 )
	{
		pMotor = ( OneAxis* )m_vMotor[ nAxisIdx ];
	}
	else if( m_vMotor.size() == 1 )
	{
		// Y축 하나밖에 없는데 X축 요청하면 NULL을 반환 함.
		if( nAxisIdx == eAxisY )
		{
			pMotor = ( OneAxis* )m_vMotor[ 0 ];
		}
	}

	return pMotor;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-08-26]
  NAME     :
  PARAMS   :
  RETURN   :
  COMMENTS :
==============================================================================*/
int CTaskLoadTable::GetAxisCount()
{
	return m_vMotor.size();
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-08-25]
  NAME     :
  PARAMS   :
  RETURN   :
  COMMENTS :
==============================================================================*/
int CTaskLoadTable::GetTeachPos( int nLocStage, double* pdPosY, double* pdRotator)
{
	ASSERT( nLocStage >= 0 && nLocStage < MAX_LD_TBL_LOC_COUNT );

	double dTPos[ CTaskLoadTable::eMaxAxisCount ] = { 0.0, };
// 	if(m_nTableIdx == eLoadTbl_1 || m_nTableIdx == eLoadTbl_4){
// 		dTPos[0] = g_DMCont.m_Teach.GetTeachPos(RID_LD_TBL_(m_nTableIdx), nLocStage, POS_IDX_LD_TBL_X);
// 	}

	dTPos[ 0 ] = g_DMCont.m_Teach.GetTeachPos( RID_LD_TBL_( m_nTableIdx ), nLocStage, POS_IDX_LD_TBL_Y );
	dTPos[ 1 ] = g_DMCont.m_Teach.GetTeachPos( RID_LD_TBL_( m_nTableIdx ), nLocStage, POS_IDX_LD_TBL_ROTATE);


	*pdPosY = dTPos[CTaskLoadTable::eAxisY];
	*pdRotator = dTPos[CTaskLoadTable::eAxisRotate];

	return ERR_NO_ERROR;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-08-25]
  NAME     :
  PARAMS   :
  RETURN   :
  COMMENTS :
==============================================================================*/
double CTaskLoadTable::GetTeachPos( int nLocStage, int nAxisIdx)
{
	ASSERT( nLocStage >= 0 && nLocStage < MAX_LD_TBL_LOC_COUNT );
	ASSERT( nAxisIdx >= 0 && nAxisIdx < CTaskLoadTable::eMaxAxisCount );

	double dTPos[ CTaskLoadTable::eMaxAxisCount ] = { 0.0, };
	GetTeachPos( nLocStage, &dTPos[ 0 ], &dTPos[1] );

	return dTPos[ nAxisIdx ];
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-08-25]
  NAME     :
  PARAMS   : eTeachPos :
			 eAxis     : -1 = 모든 축의 위치가 맞는지 점검 한다.
  RETURN   : 0 = OK
			-1 = Motion 동작 중 임.
			-2 = 동작은 멈췄으나 Position Range 안에 들어오지 않음
  COMMENTS : Motor의 현재 위치가 지정한 위치에 있는지 점검 한다.
==============================================================================*/
int CTaskLoadTable::ChkMotorPos( int nLocStage, int nAxisIdx/*=eAxisAll*/, int nStep/* = 0*/)
{
	ASSERT( nLocStage >= 0 && nLocStage < MAX_LD_TBL_LOC_COUNT );
	ASSERT( nAxisIdx >= CTaskLoadTable::eAxisAll && nAxisIdx < CTaskLoadTable::eMaxAxisCount );

	double dTeachPos[ CTaskLoadTable::eMaxAxisCount ] = { 0.0, };
	double dCurPos[ CTaskLoadTable::eMaxAxisCount ] = { 0.0, };

	for (int i = 0; i < CTaskLoadTable::eMaxAxisCount; i++) {
		dTeachPos[i] = GetTeachPos(nLocStage, i);

		if (i == eAxisY)
		{
			BOOL bTableVaccumChk = TableVacuumCheckMem();


			if (bTableVaccumChk == FALSE)
			{
				if (nLocStage == LOC_IDX_TBL_LD_SIDE)
				{
					// LocStage에 따라 Row Step Pitch가 달라짐. [9/27/2024 dohyeong.kim]
					int nTbl_2Pitch_Y = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_Y) * 2;
					dTeachPos[i] += nStep*nTbl_2Pitch_Y;
					//dTeachPos[i] += nRowStep*nTbl_2Pitch_Y;
				}
				else
				{
					//Test Site Table Step Move는 Loading Table X Pitch만큼 움직임.
					int nTbl_Pitch_X = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_X);
					dTeachPos[i] += nStep*nTbl_Pitch_X;
				}
			}
			else {
				if (nLocStage == LOC_IDX_TBL_LD_SIDE)
				{
					int nTbl_2Pitch_Y = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_Y);
					dTeachPos[i] += nStep*nTbl_2Pitch_Y;
				}
			}

		}
		else //i == eAxisRotate
		{
			dTeachPos[i] = dTeachPos[i] / 1000.0;
		}

// 		if (i == eAxisRotate)
// 			dTeachPos[i] = dTeachPos[i] / 1000.0;
	}

	for(int i = 0; i < CTaskLoadTable::eMaxAxisCount; i++ ) {
		OneAxis* pMotor = ( OneAxis* )GetAxis( i );
		if( pMotor == NULL ) { continue; }
			pMotor->GetCurrentPos(dCurPos[i]);
//		dCurPos[ i ] = pMotor->GetCurrentPos();
	}

	if( nAxisIdx == CTaskLoadTable::eAxisAll ) {
		for(int i = 0; i < CTaskLoadTable::eMaxAxisCount; i++ ) {
			OneAxis* pMotor = ( OneAxis* )GetAxis( i ); 
			if( pMotor == NULL ) { continue; }
			if( pMotor->MotionDone() != ERR_NO_ERROR) {
				return -1;
			}
			double dpos = fabs(dCurPos[i] - dTeachPos[i]);
			if( fabs( dCurPos[ i ] - dTeachPos[ i ] ) > m_dMotorErrRange[ i ] ) {
				return -2;
			}
		}
	}
	else {
		OneAxis* pMotor = ( OneAxis* )GetAxis( nAxisIdx );
		if( pMotor != NULL ) {
			if( pMotor->MotionDone() != ERR_NO_ERROR) {
				return -1;
			}

			if( fabs( dCurPos[ nAxisIdx ] - dTeachPos[ nAxisIdx ] ) > m_dMotorErrRange[ nAxisIdx ] ) {
				return -2;
			}
		}
	}

	return ERR_NO_ERROR;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-08-25]
  NAME     :
  PARAMS   :
  RETURN   : 0 = OK
			-1 = Motion 동작 중 임.
			-2 = 동작은 멈췄으나 Position Range 안에 들어오지 않음.
  COMMENTS : 각 축의 위치가 지정한 위치 + offset 에 있는지 점검 한다.
  ==============================================================================*/
int CTaskLoadTable::ChkMotorPos( int nLocStage, double dOffset, int nAxisIdx/*=eAxisAll*/ )
{
	ASSERT( nLocStage >= 0 && nLocStage < MAX_LD_TBL_LOC_COUNT );
	ASSERT( nAxisIdx >= CTaskLoadTable::eAxisAll && nAxisIdx < CTaskLoadTable::eMaxAxisCount );

	int i = 0;
	double dTeachPos[ CTaskLoadTable::eMaxAxisCount ] = { 0.0, };
	double dCurPos[ CTaskLoadTable::eMaxAxisCount ] = { 0.0, };

//	GetTeachPos( nLocStage, &dTeachPos[ 0 ]/*, &dTeachPos[1]*/ );

	for( i = 0; i < CTaskLoadTable::eMaxAxisCount; i++ ) {
		OneAxis* pMotor = ( OneAxis* )GetAxis( i );
		if( pMotor == NULL ) continue;
			pMotor->GetCurrentPos(dCurPos[i]);
	}

	if (nAxisIdx == CTaskLoadTable::eAxisAll) {
		for (i = 0; i < CTaskLoadTable::eMaxAxisCount; i++) {
			OneAxis* pMotor = (OneAxis*)GetAxis(i);
			if (pMotor == NULL) { continue; }
			if (pMotor->MotionDone() != ERR_NO_ERROR) {
				return -1;
			}

			if (fabs(dCurPos[i] - (dTeachPos[i] + dOffset)) > m_dMotorErrRange[i]) {
				return -2;
			}
		}
	}
 	else{
 		OneAxis* pMotor = (OneAxis*)GetAxis(nAxisIdx);
 
 		if(pMotor != NULL){
 			if( pMotor->MotionDone() != ERR_NO_ERROR){
 				return -1;
 			}
 
 			if( fabs(dCurPos[nAxisIdx] - (dTeachPos[nAxisIdx] + dOffset) ) > m_dMotorErrRange[nAxisIdx] ){
 				return -2;			
 			}
 		}		
 	}

	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-08-25]
  NAME     :
  PARAMS   :
  RETURN   :
  COMMENTS :
==============================================================================*/
int CTaskLoadTable::WhereIsMotorPos( std::vector<int>& vLocList, int nAxisIdx/*=eAxisAll*/ )
{
	int nPosition_y = -1;
	int nPosition_Rotate = -1;

	int nAxY_Index = eAxisY;
	double dCurPos[ eMaxAxisCount ] = { 0.0, };

	for( int i = 0; i < eMaxAxisCount; i++ ) {
		OneAxis* pMotor = GetAxis( i );
		if( pMotor )
		{
			 pMotor->GetCurrentPos(dCurPos[i]);
		}
	}

	double dTeachPos[MAX_LD_TBL_LOC_COUNT][eMaxAxisCount] = { 0.0, };
	double dUDTeachPos[MAX_LD_TBL_LOC_COUNT][eMaxAxisCount] = { 0.0, };
	
	for (int nStage = 0; nStage < MAX_LD_TBL_LOC_COUNT; nStage++)
	{
		GetTeachPos(nStage, &dTeachPos[nStage][eAxisY], &dTeachPos[nStage][eAxisRotate]);
	}

	//Y를 먼저 구한다.
	//순차 적용.
	if (dCurPos[CTaskLoadTable::eAxisY] < dTeachPos[LOC_IDX_TBL_LD_SIDE][eAxisY] - 2000.0)
		nPosition_y = eYPOS_NegaToLoadSide;
	else if (dCurPos[CTaskLoadTable::eAxisY] <= dTeachPos[LOC_IDX_TBL_LD_SIDE][eAxisY] + 2000.0)
		nPosition_y = eYPOS_LoadSide;
	else if (dCurPos[CTaskLoadTable::eAxisY] < dTeachPos[LOC_IDX_LD_TBL_TEST_SIDE_1][eAxisY] - 2000.0)
		nPosition_y = eYPOS_LoadSideToSitePos1;
	else if (dCurPos[CTaskLoadTable::eAxisY] <= dTeachPos[LOC_IDX_LD_TBL_TEST_SIDE_1][eAxisY] + 2000.0)
		nPosition_y = eYPOS_SitePos1;
	else if (dCurPos[CTaskLoadTable::eAxisY] < dTeachPos[LOC_IDX_LD_TBL_TEST_SIDE_2][eAxisY] - 2000.0)
		nPosition_y = eYPOS_SitePos1ToSitePos2;
	else if (dCurPos[CTaskLoadTable::eAxisY] <= dTeachPos[LOC_IDX_LD_TBL_TEST_SIDE_2][eAxisY] + 2000.0)
		nPosition_y = eYPOS_SitePos2;
	else if (dCurPos[CTaskLoadTable::eAxisY] < dTeachPos[LOC_IDX_LD_TBL_TEST_SIDE_3][eAxisY] - 2000.0)
		nPosition_y = eYPOS_SitePos2ToSitePos3;
	else if (dCurPos[CTaskLoadTable::eAxisY] <= dTeachPos[LOC_IDX_LD_TBL_TEST_SIDE_3][eAxisY] + 2000.0)
		nPosition_y = eYPOS_SitePos3;
	else if (dCurPos[CTaskLoadTable::eAxisY] < dTeachPos[LOC_IDX_LD_TBL_TEST_SIDE_4][eAxisY] - 2000.0)
		nPosition_y = eYPOS_SitePos3ToSitePos4;
	else if (dCurPos[CTaskLoadTable::eAxisY] <= dTeachPos[LOC_IDX_LD_TBL_TEST_SIDE_4][eAxisY] + 2000.0)
		nPosition_y = eYPOS_SitePos4;
	else
		nPosition_y = eYPOS_SitePos4ToPosiLimit;



	//Rotate Position
	dCurPos[CTaskLoadTable::eAxisRotate] = dCurPos[CTaskLoadTable::eAxisRotate] / 1000.0;
	if (dCurPos[CTaskLoadTable::eAxisRotate] <= dTeachPos[LOC_IDX_TBL_LD_SIDE][eAxisRotate] - MOTOR_INPOSITION)
		nPosition_Rotate = eYPOS_Rotate_AngleNegTo0;
	else if(dCurPos[CTaskLoadTable::eAxisRotate] < dTeachPos[LOC_IDX_TBL_LD_SIDE][eAxisRotate] + MOTOR_INPOSITION)
		nPosition_Rotate = eYPOS_Rotate_Angle0;
	else if(dCurPos[CTaskLoadTable::eAxisRotate] <= dTeachPos[LOC_IDX_LD_TBL_TEST_SIDE_1][eAxisY] - MOTOR_INPOSITION) // Test Site는 Table 각도 90도로 동일
		nPosition_Rotate = eYPOS_Rotate_Angle0To90;
	else if (dCurPos[CTaskLoadTable::eAxisRotate] < dTeachPos[LOC_IDX_LD_TBL_TEST_SIDE_1][eAxisY] + MOTOR_INPOSITION)
		nPosition_Rotate = eYPOS_Rotate_Angle90;
	else
		nPosition_Rotate = eYPOS_Rotate_Angle90ToPos;



	if (nAxisIdx == eAxisY)
	{
		vLocList.push_back(nPosition_y);
	}
	else if (nAxisIdx == eAxisRotate)
	{
		vLocList.push_back(nPosition_Rotate);
	}
	else
	{
		vLocList.push_back(nPosition_y);
		vLocList.push_back(nPosition_Rotate);
	}




	return ERR_NO_ERROR;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-08-25]
  NAME     :
  PARAMS   :
  RETURN   :
  COMMENTS :
==============================================================================*/
int CTaskLoadTable::ChkMoveConflictX( double a_dTPos )
{
	return 0;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-08-25]
  NAME     :
  PARAMS   :
  RETURN   :
  COMMENTS :
==============================================================================*/
int CTaskLoadTable::ChkMoveConflictY( double a_dTPos )
{
	return 0;
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     :
  PARAMS   : nRobotID = Oner Robot ID
  RETURN   : None
  COMMENTS : LoadTable Work Oner Set reserve
==============================================================================*/
void CTaskLoadTable::SetWorkReserve( int nRobotID )
{
	ASSERT( m_nWorkReserve == RID_NOT_RESERVE
			|| ( RID_TEST_PP_( RIDX_TEST_PP_1 ) <= m_nWorkReserve && m_nWorkReserve <= RID_TEST_PP_( RIDX_TEST_PP_2 ) )
			|| ( RID_TRAY_PP_( RIDX_TRAY_PP_1 ) )
	);
// 	if(m_nWorkReserve != RID_NOT_RESERVE)
// 	{
// 		::AfxMessageBox(_T("work reserve is reserved!! check LoadTable SetWorkReserve!!"));
// 		return;
// 	}
	m_cs.Lock();
	m_nWorkReserve = nRobotID;
	m_cs.Unlock();
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     :
  PARAMS   : none
  RETURN   : Robot ID
			 RID_NOT_RESERVE = not reserved
  COMMENTS : LoadTable Work Oner reserve check
==============================================================================*/
int  CTaskLoadTable::GetWorkReserve()
{
	return m_nWorkReserve;
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     :
  PARAMS   : none
  RETURN   : none
  COMMENTS : Clear Work Reserve
==============================================================================*/
// void CTaskLoadTable::ClearWorkReserve()
// {
// 	m_nWorkReserve = RID_NOT_RESERVE;
// }


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : none
  PARAMS   : nTableIdx : Load Table Index Number
  RETURN   : Exist Count
  COMMENTS : Load table had Device Exist Count
==============================================================================*/
int  CTaskLoadTable::GetLoadDeviceCnt()
{
	int nStageIdx = -1;
	switch( m_nTableIdx )
	{
		case eLoadTbl_1: { nStageIdx = eDeviceStage_LD_TBL_1; } break;
		case eLoadTbl_2: { nStageIdx = eDeviceStage_LD_TBL_2; } break;
		case eLoadTbl_3: { nStageIdx = eDeviceStage_LD_TBL_3; } break;
		case eLoadTbl_4: { nStageIdx = eDeviceStage_LD_TBL_4; } break;
		default: ASSERT( false );
	}

	ASSERT( nStageIdx != -1 );
	int nLoadTblPocketCntX = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Div_X );
	int nLoadTblPocketCntY = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Div_Y );

	// 4 * 4
	// 좌상단이 0,0
	//     0 1 2 3
	//    ㅡㅡㅡㅡㅡ
	// 0 | U U U U
	// 1 | U U U U
	// 2 | L L L L
	// 3 | L L L L

	int nLoadDvcCnt = 0;
	for( int nY = 0; nY < nLoadTblPocketCntY; nY++ ) {
		for( int nX = 0; nX < ( nLoadTblPocketCntX ); nX++ ) {
			char setting = m_LoadTbl_pocket_setting[ nY ][ nX ];
			bool device_exist = g_DMCont.m_dmDevice.GetDeviceExist( nStageIdx, nX, nY ) == DEF_EXIST;
			if( setting == 'L' && device_exist == true )
				nLoadDvcCnt++;
		}
	}

	return nLoadDvcCnt;
}

int CTaskLoadTable::GetLoadAreaEmptyPocketCnt()
{
	int nLoadTblPocketCntX = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Div_X );
	int nLoadTblPocketCntY = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Div_Y );

	int nLoadPocketCnt = 0;
	for( int nY = 0; nY < nLoadTblPocketCntY; nY++ ) {
		for( int nX = 0; nX < nLoadTblPocketCntX; nX++ ) {
			char setting = m_LoadTbl_pocket_setting[ nY ][ nX ];
			if( setting == 'L'  )
				nLoadPocketCnt++;
		}
	}

	int LoadDeviceCnt = GetLoadDeviceCnt();

	return nLoadPocketCnt - LoadDeviceCnt;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     :
  PARAMS   :
  RETURN   : Unload Device Count
  COMMENTS : Load table had Unloading Device Count
==============================================================================*/
int  CTaskLoadTable::GetUnloadDeviceCnt()
{
	int nStageIdx = -1;
	switch( m_nTableIdx )
	{
		case eLoadTbl_1: { nStageIdx = eDeviceStage_LD_TBL_1; } break;
		case eLoadTbl_2: { nStageIdx = eDeviceStage_LD_TBL_2; } break;
		case eLoadTbl_3: { nStageIdx = eDeviceStage_LD_TBL_3; } break;
		case eLoadTbl_4: { nStageIdx = eDeviceStage_LD_TBL_4; } break;
		default: ASSERT( false );
	}

	ASSERT( nStageIdx != -1 );
	int nLoadTblPocketCntX = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Div_X );
	int nLoadTblPocketCntY = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Div_Y );

	// 4 * 4
	// 좌상단이 0,0
	//     0 1 2 3
	//    ㅡㅡㅡㅡㅡ
	// 0 | U U U U
	// 1 | U U U U
	// 2 | L L L L
	// 3 | L L L L

	int nUnloadDvcCnt = 0;
	for( int nY = 0; nY < nLoadTblPocketCntY; nY++ ) {
		for( int nX = 0; nX < nLoadTblPocketCntX; nX++ ) {

			char setting = m_LoadTbl_pocket_setting[ nY ][ nX ];
			bool device_exist = g_DMCont.m_dmDevice.GetDeviceExist( nStageIdx, nX, nY ) == DEF_EXIST;
			if( setting == 'U' && device_exist == true )
				nUnloadDvcCnt++;
		}
	}

	return nUnloadDvcCnt;
}


int  CTaskLoadTable::GetBufferDeviceCnt()
{
	int nStageIdx = -1;
	switch (m_nTableIdx)
	{
	case eLoadTbl_1: { nStageIdx = eDeviceStage_LD_TBL_1; } break;
	case eLoadTbl_2: { nStageIdx = eDeviceStage_LD_TBL_2; } break;
	case eLoadTbl_3: { nStageIdx = eDeviceStage_LD_TBL_3; } break;
	case eLoadTbl_4: { nStageIdx = eDeviceStage_LD_TBL_4; } break;
	default: ASSERT(false);
	}

	ASSERT(nStageIdx != -1);
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

	int nUnloadDvcCnt = 0;
	for (int nY = 0; nY < nLoadTblPocketCntY; nY++) {
		for (int nX = 0; nX < nLoadTblPocketCntX; nX++) {

			char setting = m_LoadTbl_pocket_setting[nY][nX];
			bool device_exist = g_DMCont.m_dmDevice.GetDeviceExist(nStageIdx, nX, nY) == DEF_EXIST;
			if (setting == 'B' && device_exist == true)
				nUnloadDvcCnt++;
		}
	}

	return nUnloadDvcCnt;
}

int  CTaskLoadTable::GetBufferDeviceAARetestCnt()
{
	int nStageIdx = -1;
	switch (m_nTableIdx)
	{
	case eLoadTbl_1: { nStageIdx = eDeviceStage_LD_TBL_1; } break;
	case eLoadTbl_2: { nStageIdx = eDeviceStage_LD_TBL_2; } break;
	case eLoadTbl_3: { nStageIdx = eDeviceStage_LD_TBL_3; } break;
	case eLoadTbl_4: { nStageIdx = eDeviceStage_LD_TBL_4; } break;
	default: ASSERT(false);
	}

	ASSERT(nStageIdx != -1);
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

	int nAABloadDvcCnt = 0;
	for (int nY = 0; nY < nLoadTblPocketCntY; nY++) {
		for (int nX = 0; nX < nLoadTblPocketCntX; nX++) {

			char setting = m_LoadTbl_pocket_setting[nY][nX];
			bool device_exist = g_DMCont.m_dmDevice.GetDeviceExist(nStageIdx, nX, nY) == DEF_EXIST;
			ST_DD_DEVICE stDvc;
			g_DMCont.m_dmDevice.GetDeviceData(nStageIdx, nX, nY, &stDvc);
			if (setting == 'B' && device_exist == true && stDvc.nTestingCnt == eARB_1stTestEnd)
				nAABloadDvcCnt++;
		}
	}

	return nAABloadDvcCnt;
}

BOOL  CTaskLoadTable::GetLoadDeviceRearFrontExist(BOOL bRear)
{
	int nStageIdx = -1;
	switch (m_nTableIdx)
	{
	case eLoadTbl_1: { nStageIdx = eDeviceStage_LD_TBL_1; } break;
	case eLoadTbl_2: { nStageIdx = eDeviceStage_LD_TBL_2; } break;
	case eLoadTbl_3: { nStageIdx = eDeviceStage_LD_TBL_3; } break;
	case eLoadTbl_4: { nStageIdx = eDeviceStage_LD_TBL_4; } break;
	default: ASSERT(false);
	}

	ASSERT(nStageIdx != -1);
	int nLoadTblPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	int nLoadTblPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);


	int nY = m_nLoadUnloadTablePocket[bRear];

	for (int nX = 0; nX < nLoadTblPocketCntX; nX++) 
	{
		char setting = m_LoadTbl_pocket_setting[nY][nX];
		bool device_exist = g_DMCont.m_dmDevice.GetDeviceExist(nStageIdx, nX, nY) == DEF_EXIST;
		if (setting == 'L' && device_exist == true)
			return TRUE;
	}

	return FALSE;
}

// Load Table에서 Pocket L/U / Buffer 영역인지 확인 하는 함수 
BOOL  CTaskLoadTable::GetBufferPocketCheck(int nPocketX, int nPocketY)
{
	int nStageIdx = -1;
	switch (m_nTableIdx)
	{
	case eLoadTbl_1: { nStageIdx = eDeviceStage_LD_TBL_1; } break;
	case eLoadTbl_2: { nStageIdx = eDeviceStage_LD_TBL_2; } break;
	case eLoadTbl_3: { nStageIdx = eDeviceStage_LD_TBL_3; } break;
	case eLoadTbl_4: { nStageIdx = eDeviceStage_LD_TBL_4; } break;
	default: ASSERT(false);
	}

	ASSERT(nStageIdx != -1);

	// Load Table 1  Load Table 2	 // Load Table 3  Load Table 4
	// 4 * 4		// 4 * 4		// 4 * 4		// 4 * 4			
	// 좌상단이 0,0	 // 좌상단이 0,0  // 좌상단이 0,0	 // 좌상단이 0,0
	//     0 1 2 3	//     0 1 2 3	 //     0 1 2 3	//     0 1 2 3
	//    ㅡㅡㅡㅡㅡ	//    ㅡㅡㅡㅡㅡ	 //    ㅡㅡㅡㅡㅡ	//    ㅡㅡㅡㅡㅡ
	// 0 | U U U U	// 0 | L L L L	 // 0 | U U U U	// 0 | L L L L
	// 1 | B B B B	// 1 | B B B B	 // 1 | B B B B	// 1 | B B B B
	// 2 | U U U U	// 2 | L L L L	 // 2 | U U U U	// 2 | L L L L
	// 3 | B B B B	// 3 | B B B B	 // 3 | B B B B	// 3 | B B B B

	int nUnloadDvcCnt = 0;
	char setting = m_LoadTbl_pocket_setting[nPocketY][nPocketX];

	if (setting == 'B')
		return TRUE;
	

	return FALSE;
}

BOOL CTaskLoadTable::GetTableDeviceInCnt()
{
	int nStageIdx = -1;
	switch (m_nTableIdx)
	{
	case eLoadTbl_1: { nStageIdx = eDeviceStage_LD_TBL_1; } break;
	case eLoadTbl_2: { nStageIdx = eDeviceStage_LD_TBL_2; } break;
	case eLoadTbl_3: { nStageIdx = eDeviceStage_LD_TBL_3; } break;
	case eLoadTbl_4: { nStageIdx = eDeviceStage_LD_TBL_4; } break;
	default: ASSERT(false);
	}

	ASSERT(nStageIdx != -1);

	int nLoadTblPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	int nLoadTblPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

	for (int nY = 0; nY < nLoadTblPocketCntY; nY++) {
		for (int nX = 0; nX < nLoadTblPocketCntX; nX++) {
			//char setting = m_LoadTbl_pocket_setting[nY][nX];
			bool device_exist = g_DMCont.m_dmDevice.GetDeviceExist(nStageIdx, nX, nY);
			if (device_exist == true)
			{
				return TRUE;
			}

		}
	}

	return FALSE;
}

int CTaskLoadTable::GetUnload2dBarcodeErrDvcCnt()
{
	int nStageIdx = -1;
	switch (m_nTableIdx)
	{
	case eLoadTbl_1: { nStageIdx = eDeviceStage_LD_TBL_1; } break;
	case eLoadTbl_2: { nStageIdx = eDeviceStage_LD_TBL_2; } break;
	case eLoadTbl_3: { nStageIdx = eDeviceStage_LD_TBL_3; } break;
	case eLoadTbl_4: { nStageIdx = eDeviceStage_LD_TBL_4; } break;
	default: ASSERT( false );
	}

	ASSERT(nStageIdx != -1);
	int nLoadTblPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	int nLoadTblPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

	// 4 * 4
	// 좌상단이 0,0
	//     0 1 2 3
	//    ㅡㅡㅡㅡㅡ
	// 0 | U U U U
	// 1 | U U U U
	// 2 | L L L L
	// 3 | L L L L

	int nUnloadDvcCnt = 0;
	ST_DD_DEVICE stDvc;
	for (int nY = 0; nY < nLoadTblPocketCntY; nY++) 
	{
		for (int nX = 0; nX < nLoadTblPocketCntX; nX++) 
		{
			stDvc.clear();
			char setting = m_LoadTbl_pocket_setting[nY][nX];
			g_DMCont.m_dmDevice.GetDeviceData(nStageIdx, nX, nY, &stDvc);
			if (setting == 'U' && stDvc.sExist == eDeviceData_Exist && (strcmp(DEF_2D_READ_ERR_DEVICE, stDvc.szErrString) == 0  || strcmp(DEF_TEST_RESULT_WRONG_APN, stDvc.szErrString) == 0))
				nUnloadDvcCnt++;
		}
	}

	return nUnloadDvcCnt;
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     :
  PARAMS   :
  RETURN   :
  COMMENTS :
==============================================================================*/
// void CTaskLoadTable::SetUnLoadDevice(int nX, int nY, int nStatus)
// {
// 	// load table pocket 변경 (place to loadtable in loading pocket )
// 	// table1,3 = U L U L
// 	// table2,4 = L U L U
// 	int nDvcDataX = (nX%2)*2 + ( m_nTableIdx%2 ? 0 : 1 );
// 
// 	g_DMCont.m_dmDevice.SetDeviceExist(eDeviceStage_LD_TBL_1+m_nTableIdx, nDvcDataX, nY, nStatus);
// }


/*=============================================================================
  AUTHOR   : Check Motor Initialize status
  NAME     : 김선진
  PARAMS   : None
  RETURN   : TRUE  : Need Initial,  FALSE : No need initialzie
  COMMENTS : Initialize 작업이 필요한 Motor 가 존재 하는지 확인.
==============================================================================*/
BOOL CTaskLoadTable::IsInitNeed()
{
	for( std::vector<OneAxis*>::size_type i = 0; i < m_vMotor.size(); i++ ) {
		OneAxis* pMotor = ( OneAxis* )m_vMotor[ i ];

		if( pMotor->ChkMotorUsable() != ERR_NO_ERROR ) {
			return TRUE;
		}
	}
	return FALSE;
}

int CTaskLoadTable::GetDeviceDataExist( int nX, int nY, int* pnExist )
{
	*pnExist = g_DMCont.m_dmDevice.GetDeviceExist( eDeviceStage_LD_TBL_1 + m_nTableIdx, nX, nY );
	return ERR_NO_ERROR;
}

int CTaskLoadTable::SetDeviceDataExist( int nX, int nY, int nExist )
{
	int nErr = g_DMCont.m_dmDevice.SetDeviceExist( eDeviceStage_LD_TBL_1 + m_nTableIdx, nX, nY, nExist );
	return nErr;
}

void  CTaskLoadTable::SetMoveSetPlateTm( CTime cTm )
{
	m_cTmMoveTime = cTm;
}

CTime CTaskLoadTable::GetMoveSetplateTm()
{
	return m_cTmMoveTime;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-30]
  NAME     :
  PARAMS   :
  RETURN   :
  COMMENTS : 센서 상태를 반환 한다. 단순 조회만 한다면 이 함수를 사용 할 것.
==============================================================================*/
BOOL CTaskLoadTable::GetDI( int nIndex )
{
	ASSERT( nIndex >= 0 && nIndex < ( int )m_vX.size() );

	return ( BOOL )g_IO.in( m_vX[ nIndex ] );
}



int CTaskLoadTable::GetDI( int nIndex, BOOL bValue )
{
	ASSERT( nIndex >= 0 && nIndex < ( int )m_vX.size() );

	if( ( ( BOOL )g_IO.in[ m_vX[ nIndex ] ] ) == bValue )
		return ERR_NO_ERROR;

	return -1;
}

int CTaskLoadTable::do_cyl_2DBarcode_LeftRight( int nLeftRight )
{
	int nLeftOper = ( nLeftRight == DEF_LEFT ) ? DEF_ON : DEF_OFF;
	int nRightOper = ( nLeftRight == DEF_RIGHT ) ? DEF_ON : DEF_OFF;

	g_IO.out( m_vY[ y2D_BARCODE_LEFT ], nLeftOper );
	g_IO.out( m_vY[ y2D_BARCODE_RIGHT ], nRightOper );

	return 0;
}

int CTaskLoadTable::chk_cyl_2DBarcode_LeftRight( int nLeftRight )
{
	int nLeftOper = ( nLeftRight == DEF_LEFT ) ? DEF_ON : DEF_OFF;
	int nRightOper = ( nLeftRight == DEF_RIGHT ) ? DEF_ON : DEF_OFF;

	int nErr = ERR_NO_ERROR;
	nErr |= ChkDI( x2D_BARCODE_LEFT_SEN, nLeftOper );
	nErr |= ChkDI( x2D_BARCODE_RIGHT_SEN, nRightOper );

	return nErr;
}


int	CTaskLoadTable::chk_rotate_direction(int nDir)
{
// 	//Sensor 감지 되면 Test Site 방향.
// 	int nTableOper = (nDir == eDir_TestSite) ? DEF_ON : DEF_OFF;
// 
// 	int nErr = ERR_NO_ERROR;
// 
// 
// 
// 
// 	nErr |= ChkDI(xLOAD_TABLE_ROTATE1_SEN, nTableOper);
// 	nErr |= ChkDI(xLOAD_TABLE_ROTATE2_SEN, nTableOper);
// 
// 	return nErr;
	////////////////////////////////////////////////////////////////////
	int nErr = -1;
	if (nDir == eDir_LoadSite)
	{
		OneAxis* pMotor = (OneAxis*)m_vMotor[eAxisRotate];
		if (pMotor->IsHomeSensorOn() == true)
			nErr = ERR_NO_ERROR;
	}
	else
	{
		double dTPos = GetTeachPos(LOC_IDX_LD_TBL_TEST_SIDE_1 + m_nTableIdx, eMotor::eAxisRotate);
		//Package 상관 없이 Teaching값으로 Rotate 센서 확인.
		if (m_nTableIdx == eLoadTbl_1 || m_nTableIdx == eLoadTbl_2)
		{
			if (dTPos < 0) //Table 반시계 방향 teaching
			{
				nErr = ChkDI(xLOAD_TABLE_ROTATE2_SEN, DEF_ON);
			}
			else
			{
				nErr = ChkDI(xLOAD_TABLE_ROTATE1_SEN, DEF_ON);
			}
		}
		else
		{
			if (dTPos > 0) //Table 반시계 방향 teaching
			{
				nErr = ChkDI(xLOAD_TABLE_ROTATE2_SEN, DEF_ON);
			}
			else
			{
				nErr = ChkDI(xLOAD_TABLE_ROTATE1_SEN, DEF_ON);
			}
		}

	}

	return nErr;
}



int CTaskLoadTable::get_cyl_2DBarcode_LeftRight( int nLeftRight )
{
	int nLeftOper = ( nLeftRight == DEF_LEFT ) ? DEF_ON : DEF_OFF;
	int nRightOper = ( nLeftRight == DEF_RIGHT ) ? DEF_ON : DEF_OFF;

	int nErr = ERR_NO_ERROR;
	nErr |= GetDI( x2D_BARCODE_LEFT_SEN, nLeftOper );
	nErr |= GetDI( x2D_BARCODE_RIGHT_SEN, nRightOper );

	return nErr;
}

// BOOL CTaskLoadTable::do_IonizerClean(BOOL bOnOff)
// {
// 	g_IO.out(m_vY[yION_CLEAN_ON_OFF], bOnOff);
// 	if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
// 		Sleep(SIMUL_SLEEP_TIME);
// 	}
// 
// 	return ERR_NO_ERROR;
// }
// 
// BOOL CTaskLoadTable::do_IonizerOnOff(BOOL bOnOff)
// {
// 	g_IO.out(m_vY[yION_PLC_ON_OFF], !bOnOff);
// 	if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
// 		Sleep(SIMUL_SLEEP_TIME);
// 	}
// 
// 	return ERR_NO_ERROR;
// }

/*=============================================================================
  AUTHOR   : 나명진  [2015-08-25]
  NAME     :
  PARAMS   :
  RETURN   : 0 = OK, -1 = 지정한 상태가 아니다.
  COMMENTS : DI의 상태를 점검 한다. 시퀀스에서 이 함수를 사용하라.
			 bValue로 정상인 상태의 값을 넘겨 주면 시뮬레이션 모드에서 강제로 센서 값을 바꾼다.
			 Display와 같은 단순 조회만 할꺼라면 GetDI() 함수를 사용 하라.
==============================================================================*/
int CTaskLoadTable::ChkDI( int nIndex, BOOL bValue/*=TRUE*/ )
{
	ASSERT( nIndex >= 0 && nIndex < ( int )m_vX.size() );

	if( g_IO.GetDriverType() == _DIO_TYPE_SIMUL_ ) {
		g_IO.SetSimulDi( m_vX[ nIndex ], bValue );
		Sleep(SIMUL_SLEEP_TIME);
	}

	if( g_IO.in( m_vX[ nIndex ] ) != bValue ) {
		return -1;
	}

	return ERR_NO_ERROR;
}

CString CTaskLoadTable::GetLocName( int nLoc )
{
	CString strLoc = _T( "" );
	switch( nLoc )
	{
		case LOC_IDX_TBL_LD_SIDE: { strLoc.Format( "Load Side" ); }break;
		case LOC_IDX_LD_TBL_TEST_SIDE_1: { strLoc.Format("Test Side #1"); }break;
		case LOC_IDX_LD_TBL_TEST_SIDE_2: { strLoc.Format("Test Side #2"); }break;
		case LOC_IDX_LD_TBL_TEST_SIDE_3: { strLoc.Format("Test Side #3"); }break;
		case LOC_IDX_LD_TBL_TEST_SIDE_4: { strLoc.Format("Test Side #4"); }break;
		case LOC_IDX_LD_TBL_TEST_SIDE_2D: { strLoc.Format( "2D Barcode Side" ); }break;
		default: { strLoc.Format( "NONE" ); }break;
	}
	return strLoc;
}

CString CTaskLoadTable::GetDirName(int nLoc)
{
	CString strDir = _T("");
	switch (nLoc)
	{
	case LOC_IDX_TBL_LD_SIDE:
	{
		strDir.Format("Load Side"); 
	} break;
	case LOC_IDX_LD_TBL_TEST_SIDE_1:
	case LOC_IDX_LD_TBL_TEST_SIDE_2:
	case LOC_IDX_LD_TBL_TEST_SIDE_3:
	case LOC_IDX_LD_TBL_TEST_SIDE_4:
	case LOC_IDX_LD_TBL_TEST_SIDE_2D:
	{
		strDir.Format("Test Side");
	}break;
	default: {strDir.Format("NONE"); } break;
	}

	return strDir;
}

void CTaskLoadTable::MakeLog( LPCTSTR fmt, ... )
{
	TCHAR tmpMsg[ 1024 ] = { 0, };
	if( fmt )
	{
		va_list argList;
		va_start( argList, fmt );
		_vstprintf_s( tmpMsg, fmt, argList );
		va_end( argList );
	}
	TWLOG.MakeLog( static_cast< int >( eLogID_LoadTable_1 + m_nTableIdx ), Debug, "", __LINE__, NULL, tmpMsg );
}

int CTaskLoadTable::GetTableUsage()
{
	return(m_nTableIdx == eLoadTbl_2 || m_nTableIdx == eLoadTbl_3) ? eLoadType : eUnloadType;
}

void CTaskLoadTable::InitialTrigger()
{
	BOOL is_use = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use);
	if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_ || is_use == FALSE)
		return;


	g_trigger_generator.SetTriggerPos(m_nTableIdx);
	g_trigger_generator.SetStartTrigger(m_nTableIdx);

	m_bTriggerInitial = TRUE;
}




int CTaskLoadTable::LoadTblFromTestPPToIdx(int nLoadTblIdx)
{
	int nTestPPIdx = 1;
	switch (nLoadTblIdx)
	{
	case eLoadTbl_1:
	case eLoadTbl_2:
	{
		nTestPPIdx = eTestPP_1;
	}break;
	case eLoadTbl_3:
	case eLoadTbl_4:
	{
		nTestPPIdx = eTestPP_2;
	}break;
	default:
		ASSERT(nLoadTblIdx);
		break;
	}
	return nTestPPIdx;
}

void CTaskLoadTable::SetTableVacuumCheckRowStep(int nStep)
{
	if (nStep == 0)
		nStep = -1;
	else 
		nStep = 0;

	m_nTableRowStep = nStep;
}

void CTaskLoadTable::SetTableRowStep(int nStep)
{
	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
	if (nBoardType == eBoardDir_Forward)
	{
		if (nStep == -2)			nStep = -1;
		else if (nStep == 2)		nStep = 1;
		else						nStep = 0;
	}
	else
	{
		//현재 row_step 0 or -1 만 사용
		//-1 : Rear Picker가 Table 2번열 위치
		//3 : Front Picker가 Table 4번열 위치
		if (nStep == -1)			nStep = -1;
		else if (nStep == 3)		nStep = 1;
		else						nStep = 0;
	}

// 기존 방식
// 	if (nStep == -1)			nStep = -1;
// 	else if (nStep == 3)		nStep = 1;
// 	else						nStep = 0;


	m_nTableRowStep = nStep;
}

int CTaskLoadTable::GetTableRowStep()
{
	return m_nTableRowStep;
}


void CTaskLoadTable::SetTableColStep(int nStep)
{
	m_nTableColStep = nStep;
}


int CTaskLoadTable::GetTableColStep()
{
	return m_nTableColStep;
}

// void CTaskLoadTable::SetTrayDeviceExistRowStep(int nTrayDeviceExistRow)
// {
// 	//현재 row_step 0 or -1 만 사용
// 	//-1 : Rear Picker가 Table 2번열 위치
// 	//3 : Front Picker가 Table 4번열 위치
// //  	if (nTrayDeviceExistRow != 0 && nTrayDeviceExistRow != -1/* && nTrayDeviceExistRow != 3*/) {
// //  		nTrayDeviceExistRow = 0;
// //  	}
//  	if (nTrayDeviceExistRow == -1)			nTrayDeviceExistRow = -1;
//  	else if (nTrayDeviceExistRow == 3)		nTrayDeviceExistRow = 1;	
//  	else									nTrayDeviceExistRow = 0;
// 	
// 
// 	m_nTableRowStep = nTrayDeviceExistRow;
// }

// int CTaskLoadTable::GetTrayDeviceExistRowStep()
// {
// 	return m_nTableRowStep;
// }

void CTaskLoadTable::SetTestppPrePos(int nJogIdx)
{
	m_nTestPPPrePos = nJogIdx;
}

int CTaskLoadTable::GetTestppPrePos()
{
	return m_nTestPPPrePos;
}

void CTaskLoadTable::SetFlagLoadTblBuff(BOOL TR_FL)
{
	m_bLoadTblBuffUse = TR_FL;
}

BOOL CTaskLoadTable::GetFlagLoadTblBuff()
{
	return m_bLoadTblBuffUse;
}


void CTaskLoadTable::SetTableCurPos(int nCurPos)
{
	m_nTblCurPos = nCurPos;
}

int CTaskLoadTable::GetTableCurPos()
{
	return m_nTblCurPos;
}

int CTaskLoadTable::GrrSetTableIdx(int nJobIdx)
{
	int nRetIdx = -1;
	//LOC_IDX_LD_TBL_TEST_SIDE_1
	if (nJobIdx == LOC_IDX_LD_TBL_TEST_SIDE_1 || nJobIdx == LOC_IDX_LD_TBL_TEST_SIDE_2) {
		nRetIdx = LOC_IDX_LD_TBL_TEST_SIDE_2;
	}
	else {
		nRetIdx = LOC_IDX_LD_TBL_TEST_SIDE_3;
	}
	return nRetIdx;
}


void CTaskLoadTable::GrrConvertWorkPkrToPocket(int& nPocketX, int& nPocketY)
{
	int nLoadTblPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	int nLoadTblPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);

	//Temp 변수
	int nFindX = nPocketX;
	int nFindY = nPocketY;

	for (int nY = 0; nY < nLoadTblPocketCntY; nY++) {
		for (int nX = 0; nX < nLoadTblPocketCntX; nX++) {
			char setting = m_LoadTbl_pocket_setting[nY][nX];
			if (setting == 'B') {
				//Y Pocket
				if (nBoardType == eBoardDir_Forward)
				{
					if (nPocketY == (nY / 3)) {
						nFindY = nY;
					}
				}
				else
				{			
					if (nPocketY == (nY / 2)) {
						nFindY = nY;
					}
				}

				//X Pocket
				if (nPocketX == nX) {
					nFindX = nX;
				}
			}
		}
	}

	
	nPocketX = nFindX;
	nPocketY = nFindY;

}

BOOL CTaskLoadTable::ChkGrrTestEnd(int nStationIdx)
{
	//한 Station에 대한 Test End 확인.

 	BOOL bRet = TRUE;

	int nPocketCount_X = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	int nPocketCount_Y = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

	int nDvcStage = eDeviceStage_LD_TBL_1 + m_nTableIdx;

 	ST_DD_DEVICE stDevice;
	int nChkTestEndCnt = 0;
 	for (int nY = 0; nY < nPocketCount_Y; nY++) {
 		for (int nX = 0; nX < nPocketCount_X; nX++) {
 			g_DMCont.m_dmDevice.GetDeviceData(nDvcStage, nX, nY, &stDevice);
			char setting = m_LoadTbl_pocket_setting[nY][nX];
 			if (stDevice.sExist == DEF_EXIST && setting == 'B') {
				BOOL bTestEnd = stDevice.stGrrTestInfo[nStationIdx].chkTestEnd();
				if (bTestEnd != TRUE)
					bRet = FALSE;
 			}
 		}
 	}

	return bRet;
}

BOOL CTaskLoadTable::IsTableStackerImpactArea()
{
	//센서 A접
	BOOL bRet = FALSE;

	int nSen = ChkDI(CTaskLoadTable::xLOAD_TABLE_ROTATE_STOP_SEN, DEF_ON);
	if (nSen == ERR_NO_ERROR)
	{
		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}

	return bRet;
}

int CTaskLoadTable::DM_StartAddress()
{
	int nMemAdressStart = 0;
	switch (m_nTableIdx)
	{
	case eLoadTbl_1: {nMemAdressStart = BDM3_TABLE1_POCKET_VACUUM_CHECK_COMPLETE_X1_Y1;}break;
	case eLoadTbl_2: {nMemAdressStart = BDM3_TABLE2_POCKET_VACUUM_CHECK_COMPLETE_X1_Y1;}break;
	case eLoadTbl_3: {nMemAdressStart = BDM3_TABLE3_POCKET_VACUUM_CHECK_COMPLETE_X1_Y1;}break;
	case eLoadTbl_4: {nMemAdressStart = BDM3_TABLE4_POCKET_VACUUM_CHECK_COMPLETE_X1_Y1;}break;
	}
	return nMemAdressStart;
}

void CTaskLoadTable::Set_Table_DM_Vacuum_Check(int nX, int nY, BOOL bOnOff)
{
	int nMemAdressStart = DM_StartAddress();
	int nAddress = nMemAdressStart + (nX + (nY*MAX_LD_TBL_POCKET_CNT_X));

	g_DMCont.m_dmShuttle.SB(nAddress, bOnOff);
}

BOOL CTaskLoadTable::Get_Table_DM_Vacuum_Check(int nX,int nY)
{
	int nMemAdressStart = DM_StartAddress();
	int nAddress = nMemAdressStart + (nX + (nY*MAX_LD_TBL_POCKET_CNT_X));

	return g_DMCont.m_dmShuttle.GB(nAddress);
}

BOOL CTaskLoadTable::Get_Table_DM_Vacuum_All_Check()
{
	int nPocketCount_X = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	int nPocketCount_Y = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

	for (int nY = 0; nY < nPocketCount_Y; nY++)
	{
		for (int nX = 0; nX < nPocketCount_X; nX++)
		{
			int nMemAdressStart = DM_StartAddress();
			int nAddress = nMemAdressStart + (nX + (nY*MAX_LD_TBL_POCKET_CNT_X));
			if (g_DMCont.m_dmShuttle.GB(nAddress) == FALSE)
				return FALSE;
		}
	}

	return TRUE;
}

BOOL CTaskLoadTable::TableVacuumCheckMem()
{
	BOOL bTableVaccumChk = FALSE;
	switch (m_nTableIdx)
	{
	case eLoadTbl_1:
	case eLoadTbl_4:
	{
		bTableVaccumChk = g_DMCont.m_dmShuttle.GB(BDM3_TABLE_UNLOAD_VACUUM_CHECK);
	}break;
	case eLoadTbl_2:
	case eLoadTbl_3:
	{
		bTableVaccumChk = g_DMCont.m_dmShuttle.GB(BDM3_TABLE_LOAD_VACUUM_CHECK);
	}break;
	default:
		break;
	}

	return bTableVaccumChk;
}


void CTaskLoadTable::SetPocketLocNum(_eCw_CCw eCW)
{
	// first = Load Table 가로 방향 Pocket 번호
	// Second = Load Table 세로 방향 Pocket 번호
	if (_eCW == eCW)
	{
		m_vPocketLocChange.push_back(std::make_pair(CPoint(0, 1), CPoint(2, 0)));
		m_vPocketLocChange.push_back(std::make_pair(CPoint(1, 1), CPoint(2, 1)));
		m_vPocketLocChange.push_back(std::make_pair(CPoint(2, 1), CPoint(2, 2)));
		m_vPocketLocChange.push_back(std::make_pair(CPoint(3, 1), CPoint(2, 3)));

		m_vPocketLocChange.push_back(std::make_pair(CPoint(0, 3), CPoint(0, 0)));
		m_vPocketLocChange.push_back(std::make_pair(CPoint(1, 3), CPoint(0, 1)));
		m_vPocketLocChange.push_back(std::make_pair(CPoint(2, 3), CPoint(0, 2)));
		m_vPocketLocChange.push_back(std::make_pair(CPoint(3, 3), CPoint(0, 3)));
	}
	else {
		m_vPocketLocChange.push_back(std::make_pair(CPoint(0, 1), CPoint(1, 3)));
		m_vPocketLocChange.push_back(std::make_pair(CPoint(1, 1), CPoint(1, 2)));
		m_vPocketLocChange.push_back(std::make_pair(CPoint(2, 1), CPoint(1, 1)));
		m_vPocketLocChange.push_back(std::make_pair(CPoint(3, 1), CPoint(1, 0)));

		m_vPocketLocChange.push_back(std::make_pair(CPoint(0, 3), CPoint(3, 3)));
		m_vPocketLocChange.push_back(std::make_pair(CPoint(1, 3), CPoint(3, 2)));
		m_vPocketLocChange.push_back(std::make_pair(CPoint(2, 3), CPoint(3, 1)));
		m_vPocketLocChange.push_back(std::make_pair(CPoint(3, 3), CPoint(3, 0)));
	}	
}


std::vector<std::pair<CPoint, CPoint>> CTaskLoadTable::GetPocketLocNum()
{
	return m_vPocketLocChange;
}


int CTaskLoadTable::do_SideVibratorOnOff(int nOnOff)
{
	int nForOper = (nOnOff == DEF_ON) ? eOn : eOff;
	int nBackOper = (nOnOff == DEF_ON) ? eOff : eOn;

	g_IO.out(m_vY[yVIBRATOR_FOR], nForOper);
	g_IO.out(m_vY[yVIBRATOR_BACK], nBackOper);
	
	if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
		Sleep(SIMUL_SLEEP_TIME);
	}

	return ERR_NO_ERROR;
}

int CTaskLoadTable::chk_cyl_vibrator_onoff(int nOnOff)
{
	int nForOper = (nOnOff == DEF_ON) ? eOn : eOff;
	int nBackOper = (nOnOff == DEF_ON) ? eOff : eOn;

	int nErr = ERR_NO_ERROR;
	nErr |= ChkDI(xLOAD_TABLE_VIB_FOR_SEN, nForOper);
	nErr |= ChkDI(xLOAD_TABLE_VIB_BACK_SEN, nBackOper);

	return nErr;
}

int CTaskLoadTable::do_VATMovePos(int nAxisIdx, double dTargetPos, double dOffset/* = 0.0*/)
{
	ASSERT(0 <= nAxisIdx && nAxisIdx < eMaxAxisCount);

	int nErr = 0;
	int nAxisCount = m_vMotor.size();

	// Y, Rotate
	OneAxis* pMotor = (OneAxis*)m_vMotor[nAxisIdx];

	double dRateSpd = DEF_VAT_MOTOR_SPEED_RATE;/*(double)g_DMCont.m_dmShuttle.GN(nIndexSpd[m_nTableIdx][nAxisIdx]);*/
	int    nRateAcc = DEF_VAT_MOTOR_ACC_RATE;/*g_DMCont.m_dmShuttle.GN(nIndexAcc[m_nTableIdx][nAxisIdx]);*/
	int    nRateDcc = DEF_VAT_MOTOR_DCC_RATE;/*g_DMCont.m_dmShuttle.GN(nIndexDcc[m_nTableIdx][nAxisIdx]);*/

	double dRetSpd = 0;
	int    nRetAcc = 0, nRetDcc = 0;

	double dMtSpd = pMotor->GetSpeed();
	double dMtAcc = pMotor->GetAcc();
	double dMtDcc = pMotor->GetDcc();


	dRetSpd = (dMtSpd * (dRateSpd / 100.0)); /** (nSeqSpdRate / 100.0);*/
	nRetAcc = (int)(dMtAcc - (dMtAcc*0.75)*(nRateAcc / 100.0)); // 설정되어 있는 값에서 spd rate가 높을 수록 acc/dcc를 감소 시켜준다. 가장 빠를 때가 설정 속도의 95%뺀 acc/dcc로 함.
	nRetDcc = (int)(dMtDcc - (dMtDcc*0.75)*(nRateDcc / 100.0));

	if (nRetAcc < 200) nRetAcc = 200;
	if (nRetDcc < 200) nRetDcc = 200;


	double dMovePos = dTargetPos + dOffset;
	
	nErr = pMotor->Move(dMovePos, dRetSpd, nRetAcc, nRetDcc);

	return nErr;
}

int CTaskLoadTable::ChkVATMotorPos(int nAxisIdx, double dTPos)
{
	ASSERT(nAxisIdx >= CTaskLoadTable::eAxisAll && nAxisIdx < CTaskLoadTable::eMaxAxisCount);

	if (nAxisIdx == eAxisRotate) {
		dTPos = dTPos / 1000.0;
	}

	double dCurPos = 0.0;
	OneAxis* pMotor = (OneAxis*)GetAxis(nAxisIdx);
	if (pMotor == NULL) 
		return -3;

	pMotor->GetCurrentPos(dCurPos);

	if (pMotor->MotionDone() != ERR_NO_ERROR) {
		return -1;
	}

	if (fabs(dCurPos - dTPos) > m_dMotorErrRange[nAxisIdx]) {
		return -2;
	}
	
	return ERR_NO_ERROR;
}

void CTaskLoadTable::MakeVatLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = { 0, };
	if (fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf_s(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_LoadTable_1 + m_nTableIdx), Debug, "", __LINE__, NULL, tmpMsg);
}