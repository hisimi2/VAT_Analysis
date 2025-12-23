#include "StdAfx.h"
#include "TaskTrayFeeder.h"

CTaskTrayFeeder::CTaskTrayFeeder()
{
	// DI
	m_mapDI.SetAt( xTRAY_FEEDER_GRIP_FOR_LEFT_SEN,		"TRAY_FEEDER_GRIP_FOR_LEFT_SEN" );
	m_mapDI.SetAt( xTRAY_FEEDER_GRIP_BACK_LEFT_SEN,		"TRAY_FEEDER_GRIP_BACK_LEFT_SEN" );
	m_mapDI.SetAt( xTRAY_FEEDER_UP_LEFT_SEN,			"TRAY_FEEDER_UP_LEFT_SEN" );
	m_mapDI.SetAt( xTRAY_FEEDER_DOWN_LEFT_SEN,			"TRAY_FEEDER_DOWN_LEFT_SEN" );
	m_mapDI.SetAt( xTRAY_FEEDER_GRIP_FOR_RIGHT_SEN,		"TRAY_FEEDER_GRIP_FOR_RIGHT_SEN" );
	m_mapDI.SetAt( xTRAY_FEEDER_GRIP_BACK_RIGHT_SEN,	"TRAY_FEEDER_GRIP_BACK_RIGHT_SEN" );
	m_mapDI.SetAt( xTRAY_FEEDER_UP_RIGHT_SEN,			"TRAY_FEEDER_UP_RIGHT_SEN" );
	m_mapDI.SetAt( xTRAY_FEEDER_DOWN_RIGHT_SEN,			"TRAY_FEEDER_DOWN_RIGHT_SEN" );
	m_mapDI.SetAt( xTRAY_FEEDER_TRANSFER_AREA_TRAY_DETECT_SEN,		"TRAY_FEEDER_TRANSFER_AREA_TRAY_DETECT_SEN" );
	m_mapDI.SetAt( xTRAY_FEEDER_SAFETY_SEN,				"TRAY_FEEDER_SAFETY_SEN" );
	m_mapDI.SetAt( xTRAY_FEEDER_EXIST_SEN,				"TRAY_FEEDER_EXIST_SEN");

	// DO
	m_mapDO.SetAt( yTRAY_FEEDER_GRIP_FOR,				"TRAY_FEEDER_GRIP_FOR" );
	m_mapDO.SetAt( yTRAY_FEEDER_GRIP_BACK,				"TRAY_FEEDER_GRIP_BACK" );
	m_mapDO.SetAt( yTRAY_FEEDER_UP,						"TRAY_FEEDER_UP" );
	m_mapDO.SetAt( yTRAY_FEEDER_DOWN,					"TRAY_FEEDER_DOWN" );
	m_mapDO.SetAt( yTRAY_FEEDER_ALIGN_X_FOR,			"TRAY_FEEDER_ALIGN_X_FOR" );
	m_mapDO.SetAt( yTRAY_FEEDER_ALIGN_X_BACK,			"TRAY_FEEDER_ALIGN_X_BACK" );

	// Motor
	m_mapMotor.SetAt( eAxisY, "Axis Y" );

	m_bInitialized = FALSE;
	m_nTrayDeviceExistRowStep = 0;
	m_bTrayLastRow = FALSE;
	m_nPitchType = 0;
}

int CTaskTrayFeeder::Initialize( char* szDriverFileName )
{
	CConfigData ConfigData( szDriverFileName );
	char szSection[ 128 ] = { 0, };

	// LOG File Name
	char szLogFileName[ 512 ] = { 0, };
	char szBuff[ 512 ] = { 0, };
	char szDefaultValue[ 128 ] = { 0, };

	memset( szSection, 0x00, sizeof( szSection ) );
	sprintf_s( szSection, sizeof( szSection ), "TASK TRAY FEEDER CONFIG" );

	memset( szDefaultValue, 0x00, sizeof( szDefaultValue ) );
	sprintf_s( szDefaultValue, sizeof( szDefaultValue ), "TaskTrayFeeder.log" );

	ConfigData.GetString( szSection, "Log File Name", szDefaultValue, szBuff );
	sprintf_s( szLogFileName, sizeof( szLogFileName ), "%s%s", SZ_LOG_FILE_PATH, szBuff );
	m_Logger.SetFileName( szLogFileName );

	// Digital Input
	memset( szSection, 0x00, sizeof( szSection ) );
	sprintf_s( szSection, sizeof( szSection ), "TASK TRAY FEEDER CONFIG - DI" );
	int nDiCount = m_mapDI.GetCount();
	for( int i = 0; i < nDiCount; i++ ) {
		char szKey[ 128 ] = { 0, };
		CString strName;
		m_mapDI.Lookup( i, strName );
		sprintf_s( szKey, sizeof( szKey ), "%s", strName.GetString() );
		int nIdx = ConfigData.GetInt( szSection, szKey, "0" );
		m_vX.push_back( nIdx );
	}

	// Digital Output
	memset( szSection, 0x00, sizeof( szSection ) );
	sprintf_s( szSection, sizeof( szSection ), "TASK TRAY FEEDER CONFIG - DO" );
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
	sprintf_s( szSection, sizeof( szSection ), "TASK TRAY FEEDER CONFIG - MOTOR" );
	int nMotorCount = m_mapMotor.GetCount();
	for( int i = 0; i < nMotorCount; i++ ) {
		char szKey[ 128 ] = { 0, };
		CString strName;
		m_mapMotor.Lookup( i, strName );
		sprintf_s( szKey, sizeof( szKey ), "%s", strName.GetString() );
		int nIdx = ConfigData.GetInt( szSection, szKey, "-1" );
		if( nIdx != -1 ) {
			OneAxis* pMotor = g_Motor[ nIdx ];
			m_vMotor.push_back( pMotor );
		} else {
			m_vMotor.push_back( nullptr );
		}
	}

	return CBaseTaskManager::Initialize();
}

void CTaskTrayFeeder::Finalize()
{
	CBaseTaskManager::Finalize();
}

void CTaskTrayFeeder::OnOperatorCommand( int nOperatorCommand, WPARAM wParam, LPARAM lParam )
{
	switch( nOperatorCommand )
	{
		case TWMB_ID_SKIP:
		case TWMB_ID_P_SKIP:
		{
			SetProcessCommand( base_seq_cmd::eProcCmd_Skip );
		}break;
	}
}

int CTaskTrayFeeder::do_MovePos( eMotor motor_idx, eLocIdxFeeder nLocStage, int nStep, uint32_t nSeqSpdRate )
{
	ASSERT( 0 <= nLocStage && nLocStage < eLocIdxFeeder::MAX_TRAY_FEEDER_LOC_COUNT );
	ASSERT( 0 <= nSeqSpdRate && nSeqSpdRate <= 100 );

	OneAxis* pMotor = GetAxis( motor_idx );
	if( pMotor == nullptr ) {
		return ERR_MOTOR_DATA_IS_NULL_POINTER;
	}

	double dTPos = GetTeachPos( motor_idx, nLocStage );
	int dRateSpd = g_DMCont.m_dmFeeder.GN(NDM10_SPD_Feeder_Y);
	int nRateAcc = g_DMCont.m_dmFeeder.GN(NDM10_ACC_Feeder_Y);
	int nRateDec = g_DMCont.m_dmFeeder.GN(NDM10_DCC_Feeder_Y);

	double dMtSpd = pMotor->GetSpeed();
	double dMtAcc = pMotor->GetAcc();
	double dMtDec = pMotor->GetDcc();

	double dRetSpd = ( dMtSpd * ( dRateSpd / 100.0 ) ) * ( nSeqSpdRate / 100.0 );
	int nRetAcc = ( int )( dMtAcc - ( dMtAcc * 0.75 ) * ( nRateAcc / 100.0 ) );
	int nRetDec = ( int )( dMtDec - ( dMtDec * 0.75 ) * ( nRateDec / 100.0 ) );

	if( nRetAcc < 100 ) nRetAcc = 100;
	if( nRetDec < 100 ) nRetDec = 100;

	double dCurPos = 0;
	pMotor->GetCurrentPos(dCurPos);
	double dGapPos = fabs( dTPos - dCurPos );
	if( dGapPos < 100000.0 ) {
		dRetSpd = dRetSpd * 0.75;
	}

	double tray_pitch_x_um = g_DMCont.m_dmTrayPP.GN(NDM5_Tray_Pitch_X);
	double tray_pitch_y_um = g_DMCont.m_dmTrayPP.GN( NDM5_Tray_Pitch_Y );

	double dPosOffset = g_DMCont.m_dmFeeder.GD(DDM10_FEEDER_STEP_OFFSET);
// 	if( nStep > 0 ) {
// 		dPosOffset = g_DMCont.m_dmFeeder.GD(DDM10_FEEDER_STEP_OFFSET);
// 	}

	// 1호기 Feeder 개선으로 Top Left로 탐색 하도록 변경
	//  [4/23/2025 donghyun.shin]
	//int nEQPNo = g_DMCont.m_dmEQP.GN(NDM11_Handler_EQP_No);
	//int nWorkDir = (nEQPNo > 1) ? eFeederWorkDirTopLeft : eFeederWorkDirBottomLeft;

	BOOL bApplyBacklash = FALSE;

	int nWorkDir = eFeederWorkDirTopLeft;
	double dFTPos = 0.0;
	if (motor_idx == eAxisY)
	{
		if (nWorkDir == eFeederWorkDirTopLeft)
		{
			dFTPos = dTPos + (tray_pitch_y_um * nStep) + (nStep * dPosOffset);
		}
		else
		{
			dFTPos = dTPos - (tray_pitch_y_um * nStep) + (nStep * dPosOffset);
		}

		bApplyBacklash = TRUE;
	}
	else
	{
		dFTPos = dTPos;
	}

	//double dFTPos = dTPos - (tray_pitch_y_um * nStep) - (nStep * dPosOffset);

//	double dFTPos = dTPos - (tray_pitch_y_um * nPitchGap * nStep) + (tray_pitch_y_um * nFinalDiv)  + dPosOffset;

	MakeLog( "%s : Final Target Pos : %.3fmm, Org Target Pos : %.3fmm, Pos Offset : %.3fmm", pMotor->GetAxisName(), dFTPos / 1000.0, dTPos / 1000.0, dPosOffset / 1000.0 );

	int nErr = pMotor->Move( dFTPos, dRetSpd, nRetAcc, nRetDec, bApplyBacklash);

	return nErr;

	return 0;
}

int CTaskTrayFeeder::do_MovePos_Pcd_backlash(double dAbsPosition, eMotor nAxisIdx, int nSeqSpdRate /*= 5*/, double dPosOffset /*= 0.0*/, BOOL bIsApplyBcklsh)
{
	ASSERT(nAxisIdx >= 0 && nAxisIdx < eAxisMax);
	ASSERT(nSeqSpdRate >= 0 && nSeqSpdRate <= 100);

	OneAxis* pMotor = GetAxis(nAxisIdx);
	if (pMotor == nullptr) {
		return ERR_MOTOR_DATA_IS_NULL_POINTER;
	}

	int dRateSpd = g_DMCont.m_dmFeeder.GN(NDM10_SPD_Feeder_Y);
	int nRateAcc = g_DMCont.m_dmFeeder.GN(NDM10_ACC_Feeder_Y);
	int nRateDec = g_DMCont.m_dmFeeder.GN(NDM10_DCC_Feeder_Y);

	double dMtSpd = pMotor->GetSpeed();
	double dMtAcc = pMotor->GetAcc();
	double dMtDec = pMotor->GetDcc();

	double dRetSpd = (dMtSpd * (dRateSpd / 100.0)) * (nSeqSpdRate / 100.0);
	int nRetAcc = (int)(dMtAcc - (dMtAcc * 0.75) * (nRateAcc / 100.0));
	int nRetDec = (int)(dMtDec - (dMtDec * 0.75) * (nRateDec / 100.0));

	if (nRetAcc < 100) nRetAcc = 100;
	if (nRetDec < 100) nRetDec = 100;

	double dCurPos = 0;
	pMotor->GetCurrentPos(dCurPos);

	double tray_pitch_x_um = g_DMCont.m_dmTrayPP.GN(NDM5_Tray_Pitch_X);
	double tray_pitch_y_um = g_DMCont.m_dmTrayPP.GN(NDM5_Tray_Pitch_Y);


	if (nRetAcc < 200) nRetAcc = 200;
	if (nRetDec < 200) nRetDec = 200;
	

	int nErr = pMotor->Move(dAbsPosition + dPosOffset, dRetSpd, nRetAcc, nRetDec);

	return nErr;

	return 0;
}

int CTaskTrayFeeder::ChkMotorPos( eMotor motor_idx, eLocIdxFeeder nLocStage, int nStep )
{
	ASSERT( 0 <= nLocStage && nLocStage < eLocIdxFeeder::MAX_TRAY_FEEDER_LOC_COUNT );

	double dTeachPos_um = GetTeachPos( motor_idx, nLocStage );

	int tray_pitch_y_um = g_DMCont.m_dmTrayPP.GN( NDM5_Tray_Pitch_Y );
	//double dPosOffset_um = dPosOffset_um = g_DMCont.m_dmFeeder.GD(DDM10_FEEDER_STEP_OFFSET);
	double dPosOffset_um = g_DMCont.m_dmFeeder.GD(DDM10_FEEDER_STEP_OFFSET);
	
	// 	if( nStep > 0 ) {
// 		dPosOffset_um = g_DMCont.m_dmFeeder.GD(DDM10_FEEDER_STEP_OFFSET);
// 	}


//	double dFTPos = dTeachPos_um - ( tray_pitch_y_um * nPitchGap * nStep ) + (tray_pitch_y_um * nFinalDiv) + dPosOffset_um;

	// 1호기 Feeder 개선으로 Top Left로 탐색 하도록 변경
	//  [4/23/2025 donghyun.shin]
	//장비 No
	//int nEQPNo = g_DMCont.m_dmEQP.GN(NDM11_Handler_EQP_No);
	//int nWorkDir = (nEQPNo > 1) ? eFeederWorkDirTopLeft : eFeederWorkDirBottomLeft;
	int nWorkDir =  eFeederWorkDirTopLeft;
	double dFTPos = 0.0;
	if (motor_idx == eAxisY)
	{
		if (nWorkDir == eFeederWorkDirTopLeft)
		{
			dFTPos = dTeachPos_um + (tray_pitch_y_um * nStep) + (nStep * dPosOffset_um);
		}
		else
		{
			dFTPos = dTeachPos_um - (tray_pitch_y_um * nStep) + (nStep * dPosOffset_um);
		}
	}
	else
	{
		dFTPos = dTeachPos_um;
	}

	//Pitch Type 통합.
	//double dFTPos = dTeachPos_um - (tray_pitch_y_um * nStep) - (nStep * dPosOffset_um);

	auto pMotor = GetAxis( motor_idx );
	if (pMotor == nullptr) {
		return -3;
	}

	double dCurPos = 0;
	pMotor->GetCurrentPos(dCurPos);

// 	if( pMotor == nullptr ) {
// 		return -3;
// 	}

	if( pMotor->AxisDone() != ERR_NO_ERROR ) {
		return -1;
	}

	if( fabs( dCurPos - dFTPos ) > MOTOR_INPOSITION ) {
		return -2;
	}

	return ERR_NO_ERROR;
}

int CTaskTrayFeeder::WhereisMotorPos()
{
	double dTeachPos_Start = GetTeachPos(eMotor::eAxisY, LOC_IDX_FEEDER_LOAD_PICK_START_POS);
	double dTeachPos_Load = GetTeachPos(eMotor::eAxisY, LOC_IDX_FEEDER_TRAY_PICK_POS);
	double dTeachPos_Unload = GetTeachPos(eMotor::eAxisY, LOC_IDX_FEEDER_TRAY_PLACE_POS);

	auto pMotor = GetAxis(eMotor::eAxisY);
	double dCurPos = 0;
	pMotor->GetCurrentPos(dCurPos);

	if (dTeachPos_Start - 500.0 <= dCurPos && dCurPos <= dTeachPos_Start + 500.0) {
		return ePosY_StartPos;
 	}
	else if (dTeachPos_Load - 500.0 <= dCurPos && dCurPos <= dTeachPos_Load + 500.0) {
		return ePosY_LoadPos;
	}
	else if (dTeachPos_Unload - 500.0 <= dCurPos && dCurPos <= dTeachPos_Unload + 500.0) {
		return ePosY_UnloadPos;
	}

	return ePosY_None;
}


int CTaskTrayFeeder::do_cyl_ClampUnclamp( eClampCmd nClampUnclamp )
{
	eOnOffState clamp = nClampUnclamp == eClamp ? eOn : eOff;
	eOnOffState unclamp = nClampUnclamp == eUnclamp ? eOn : eOff;

	return SetDO( { { eDO::yTRAY_FEEDER_GRIP_FOR, clamp },
				  { eDO::yTRAY_FEEDER_GRIP_BACK, unclamp } } );
}

stErrCode CTaskTrayFeeder::chk_cyl_ClampUnclamp( eClampCmd nClampUnclamp )
{
	eOnOffState clamp = nClampUnclamp == eClamp ? eOn : eOff;
	eOnOffState unclamp = nClampUnclamp == eUnclamp ? eOn : eOff;

	return ChkDI( { { eDI::xTRAY_FEEDER_GRIP_FOR_LEFT_SEN, clamp },
				  { eDI::xTRAY_FEEDER_GRIP_FOR_RIGHT_SEN, clamp },
				  { eDI::xTRAY_FEEDER_GRIP_BACK_LEFT_SEN, unclamp },
				  { eDI::xTRAY_FEEDER_GRIP_BACK_RIGHT_SEN, unclamp } } );
}

stErrCode CTaskTrayFeeder::get_cyl_ClampUnclamp( eClampCmd nClampUnclamp )
{
	eOnOffState clamp = nClampUnclamp == eClamp ? eOn : eOff;
	eOnOffState unclamp = nClampUnclamp == eUnclamp ? eOn : eOff;

	return GetDI( { { eDI::xTRAY_FEEDER_GRIP_FOR_LEFT_SEN, clamp },
				  { eDI::xTRAY_FEEDER_GRIP_FOR_RIGHT_SEN, clamp },
				  { eDI::xTRAY_FEEDER_GRIP_BACK_LEFT_SEN, unclamp },
				  { eDI::xTRAY_FEEDER_GRIP_BACK_RIGHT_SEN, unclamp } } );
}

int CTaskTrayFeeder::do_cyl_UpDown( eUpDownCmd nUpDown )
{
	//복동에서 단동으로 변경.
	//단동
// 	eOnOffState up = eOn; //상시 Up
// 	eOnOffState down = nUpDown == eDown ? eOn : eOff;
// 
// 	return SetDO({ { eDO::yTRAY_FEEDER_DOWN, down } });


	//복동
	eOnOffState up = nUpDown == eUp ? eOn : eOff;
	eOnOffState down = nUpDown == eDown ? eOn : eOff;

	return SetDO({ { eDO::yTRAY_FEEDER_UP, up },
 				  { eDO::yTRAY_FEEDER_DOWN, down } } );
}

stErrCode CTaskTrayFeeder::chk_cyl_UpDown( eUpDownCmd nUpDown )
{
	eOnOffState up = nUpDown == eUp ? eOn : eOff;
	eOnOffState down = nUpDown == eDown ? eOn : eOff;

	return ChkDI( { { eDI::xTRAY_FEEDER_UP_LEFT_SEN, up },
				  { eDI::xTRAY_FEEDER_UP_RIGHT_SEN, up },
				  { eDI::xTRAY_FEEDER_DOWN_LEFT_SEN, down },
				  { eDI::xTRAY_FEEDER_DOWN_RIGHT_SEN, down } } );
}

stErrCode CTaskTrayFeeder::get_cyl_UpDown( eUpDownCmd nUpDown )
{
	eOnOffState up = nUpDown == eUp ? eOn : eOff;
	eOnOffState down = nUpDown == eDown ? eOn : eOff;

	return GetDI( { { eDI::xTRAY_FEEDER_UP_LEFT_SEN, up },
				  { eDI::xTRAY_FEEDER_UP_RIGHT_SEN, up },
				  { eDI::xTRAY_FEEDER_DOWN_LEFT_SEN, down },
				  { eDI::xTRAY_FEEDER_DOWN_RIGHT_SEN, down } } );
}

int CTaskTrayFeeder::do_cyl_align_ForBack( eForwBackCmd nForBack )
{
	eOnOffState forw = nForBack == eForw ? eOn : eOff;
	eOnOffState back = nForBack == eBack ? eOn : eOff;

	int ret = SetDO({ { eDO::yTRAY_FEEDER_ALIGN_X_FOR, forw },
				  { eDO::yTRAY_FEEDER_ALIGN_X_BACK, back } });

	Sleep( 100 ); // Input Sensor 없어서, 일정 시간 대기 후 함수 빠져나가도록 작성.

	return ret;
}

stErrCode CTaskTrayFeeder::chk_cyl_align_ForBack( eForwBackCmd nForBack )
{
	eOnOffState forw = nForBack == eForw ? eOn : eOff;
	eOnOffState back = nForBack == eBack ? eOn : eOff;

	return GetDO( { { eDO::yTRAY_FEEDER_ALIGN_X_FOR, forw },
				  { eDO::yTRAY_FEEDER_ALIGN_X_BACK, back } } );
}

stErrCode CTaskTrayFeeder::get_cyl_align_ForBack( eForwBackCmd nForBack )
{
	eOnOffState forw = nForBack == eForw ? eOn : eOff;
	eOnOffState back = nForBack == eBack ? eOn : eOff;

	return GetDO( {  { eDO::yTRAY_FEEDER_ALIGN_X_FOR, forw },
				  { eDO::yTRAY_FEEDER_ALIGN_X_BACK, back } } );
}


bool CTaskTrayFeeder::GetTrayExist_TransferArea_Data()
{
	return g_DMCont.m_dmDevice.GetTrayExist(eDeviceStage_STACKER_CONV_3_TRANSFER_AREA) == TRUE;
}

bool CTaskTrayFeeder::GetTrayExist_Data() 
{
	return g_DMCont.m_dmDevice.GetTrayExist( eDeviceStage_TRAY_FEEDER ) == TRUE;
}

bool CTaskTrayFeeder::IsEmptyDevice()
{
	return g_DMCont.m_dmDevice.GetDeviceCount( eDeviceStage_TRAY_FEEDER ) == 0;
}

void CTaskTrayFeeder::SetTrayDeviceExistRowStep(int nTrayDeviceExistRow, int nHandYOffset, int nWorkDir)
{
	if (nWorkDir == eFeederWorkDirTopLeft)
	{
		int nBaseRow = 0;

		if (nBaseRow + nTrayDeviceExistRow == -30 || nBaseRow + nTrayDeviceExistRow == -29)
			return;

		m_nTrayDeviceExistRowStep = nBaseRow + nTrayDeviceExistRow;
		//첫번째 줄
	}
	else // nWorkDir == eFeederWorkDirBottomLeft
	{
		int TrayDiv_Y = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);

		int nY_PitchType = 1;
		if (g_DMCont.m_dmTrayPP.GN(NDM5_Tray_Pitch_Y) < DEF_TRAY_PP_MAX_Y_PITCH_NARROW) {
			nY_PitchType = 2;
		}

		int nBaseRow = TrayDiv_Y - nY_PitchType;

		//C-Tray Step -> 해당 열 Save.
		//Pitch Type 통합.
		m_nTrayDeviceExistRowStep = nBaseRow - nTrayDeviceExistRow;

		// 1호기 Feeder 개선으로 주석 처리
		//  [4/23/2025 donghyun.shin]
		//int nEQPNo = g_DMCont.m_dmEQP.GN(NDM11_Handler_EQP_No);
// 		if (nEQPNo == DEF_EQUIPMENT_UNIT1)
// 		{
// 			if (m_nTrayDeviceExistRowStep == eFeederStepLast_First) // step == -1
// 			{
// 				m_nTrayDeviceExistRowStep = 1;
// 				return;
// 			}
// 			else if (m_nTrayDeviceExistRowStep == eFeederStepLast_Seconde) //step == -2
// 			{
// 				m_nTrayDeviceExistRowStep = 0;
// 				return;
// 			}
// 		}


		//영역 확장 되면 -1 * m_nTrayDevice ExistRowStep 그대로 전달 해도 괜찮다.
		if (m_nTrayDeviceExistRowStep < -1)
		{
			m_nTrayDeviceExistRowStep = 0;
			return;
		}
	}
}

int CTaskTrayFeeder::GetTrayDeviceExistRowStep()
{
	return m_nTrayDeviceExistRowStep;
}

void CTaskTrayFeeder::SetIsTrayLastRow(BOOL bSet)
{
	m_bTrayLastRow = bSet;
}

BOOL CTaskTrayFeeder::GetIsTrayLastRow()
{
	return m_bTrayLastRow;
}

void CTaskTrayFeeder::SetClearStep()
{
	m_nTrayDeviceExistRowStep = 0;
}

int CTaskTrayFeeder::ChkTrayExist_TransferArea_Sen( eExistState nExist )
{
	//임시

	eOnOffState on_off = nExist == eExist ? eOn : eOff;

	int nErr = ChkDI(eDI::xTRAY_FEEDER_TRANSFER_AREA_TRAY_DETECT_SEN, on_off);

	int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);

	BOOL vat_in_process = g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_USE);
	if (nDeviceMode == eNoDeviceNoTray && vat_in_process != TRUE)
	{
		return ERR_NO_ERROR;
	}

	return nErr;
}

int CTaskTrayFeeder::GetTrayExist_TransferArea_DvcMode(int nExist)
{
	int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
	int nErr = ERR_NO_ERROR;

	//if( nDeviceMode == eNoDeviceNoTray )
	//{
	int nExistTray = g_DMCont.m_dmDevice.GetTrayExist(eDeviceDataStage::eDeviceStage_STACKER_CONV_3_TRANSFER_AREA);
	if (nExistTray != nExist)
		return ERR_UNKNOWN;
	//}
	//else

	if (nDeviceMode != eNoDeviceNoTray)
	{
		nErr = GetTrayExist_TransferArea_Sen(eExistState(nExist));
		if (nErr != ERR_NO_ERROR)
			return nErr;
	}


	return nErr;
}

int CTaskTrayFeeder::GetTrayExist_TransferArea_Sen( eExistState nExist )
{
	//임시

	eOnOffState on_off = nExist == eExist ? eOn : eOff;

	return GetDI( { { eDI::xTRAY_FEEDER_TRANSFER_AREA_TRAY_DETECT_SEN, on_off } } );

	
}

int CTaskTrayFeeder::GetSafetySensor()
{
	return GetDI(eDI::xTRAY_FEEDER_SAFETY_SEN);
}

int CTaskTrayFeeder::ChkSafetySensor(eExistState nExist)
{
	eOnOffState on_off = nExist == eExist ? eOn : eOff;

	int nErr = ChkDI(eDI::xTRAY_FEEDER_SAFETY_SEN, on_off);

	return nErr;
}

int CTaskTrayFeeder::GetTrayExistSensor_DvcMode(int nExist)
{
	int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
	int nErr = FALSE;


// 	if (CBaseTaskManager::m_bEqpControlMode != eEqpCtrlMode_Manual)
// 	{
// 

		if (g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK) == eNoDeviceNoTray)
		{
			int nExistTray = g_DMCont.m_dmDevice.GetTrayExist(eDeviceDataStage::eDeviceStage_TRAY_FEEDER);
			//return nExistTray;
			if (nExistTray != nExist)
				return ERR_UNKNOWN;
			else
				return nExistTray;
		}


		if (nDeviceMode != eNoDeviceNoTray)
		{
			nErr = GetTrayExistSensor();
			return nErr;
			// 		if (nErr != ERR_NO_ERROR)
			// 			return nErr;
		}
// 	}
// 	else
// 	{
// 		nErr = GetTrayExistSensor();
// 		return nErr;
// 	}
// 	if (CBaseTaskManager::m_bEqpControlMode != eEqpCtrlMode_Manual)
// 	{
// 
// 	}
// 	else
// 	{
// 		nErr = GetTrayExistSensor();
// 		return nErr;
// 	}

	return nErr;
}

int CTaskTrayFeeder::GetTrayExistSensor()
{
	//  [7/18/2024 dohyeong.kim]2호기 임시
	//  Feeder Sensor 안보고 Data 보도록 변경.
// 	int nEQPNo = g_DMCont.m_dmEQP.GN(NDM11_Handler_EQP_No);
// 	if (nEQPNo > DEF_EQUIPMENT_UNIT1)
// 	{
// 		int nExistTray = g_DMCont.m_dmDevice.GetTrayExist(eDeviceDataStage::eDeviceStage_TRAY_FEEDER);
// 		return nExistTray; //eExist = 1, eNot_Exist = 0
// 	}


	int nRet = GetDI(eDI::xTRAY_FEEDER_EXIST_SEN);
	return !nRet;
}

int CTaskTrayFeeder::ChkTrayExistSensor(eExistState nExist)
{
	eOnOffState on_off = nExist == eExist ? eOff : eOn;
	int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
	BOOL vat_in_process = g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_USE);
	//TRUE : Tray Empty
	//FALSE : Tray Exist
	if (nDeviceMode == eNoDeviceNoTray && vat_in_process != TRUE)
	{
		int nExistTray = g_DMCont.m_dmDevice.GetTrayExist(eDeviceDataStage::eDeviceStage_TRAY_FEEDER);
		if (nExistTray != nExist)
			return ERR_UNKNOWN;
		else
			return ERR_NO_ERROR;
	}

	//  [7/18/2024 dohyeong.kim]2호기 임시
	//  Feeder Sensor 안보고 Data 보도록 변경.
// 	int nEQPNo = g_DMCont.m_dmEQP.GN(NDM11_Handler_EQP_No);
// 	if (nEQPNo > DEF_EQUIPMENT_UNIT1)
// 	{
// 		int nExistTray = g_DMCont.m_dmDevice.GetTrayExist(eDeviceDataStage::eDeviceStage_TRAY_FEEDER);
// 		if (nExistTray != nExist)
// 			return ERR_UNKNOWN;
// 		else
// 			return ERR_NO_ERROR;
// 	}




	int nErr = ChkDI(eDI::xTRAY_FEEDER_EXIST_SEN, on_off);

	return nErr;
}

CTaskTrayFeeder::eWorkSite CTaskTrayFeeder::WhereIsWorkSite()
{
	bool bConvWorkEnable = false;

	auto pMotor = GetAxis( eMotor::eAxisY );

	double dCurrentPos_um = 0;
	pMotor->GetCurrentPos(dCurrentPos_um);
	double dCommandPos_um = 0; 
	pMotor->GetCommandPos( dCommandPos_um );

	double dLoadPos_um = GetTeachPos( eMotor::eAxisY, eLocIdxFeeder::LOC_IDX_FEEDER_LOAD_PICK_START_POS );
	double dLoadPPWorkSafetyDistance_um = 420.0/*mm*/ * 1000.0;

	if( dCurrentPos_um < ( dLoadPos_um - dLoadPPWorkSafetyDistance_um ) ) {
		// feeder가 Load PP 작업 영역 보다 아래 있으면, Load PP는 Conveyor에서 작업 가능.
		if( g_TaskStacker[ eSTACKER_03 ].IsConveyorMoving() == false &&
			g_TaskStacker[ eSTACKER_03 ].get_cyl_TrayAlign( DEF_FORW ) == ERR_NO_ERROR &&
			g_TaskStacker[ eSTACKER_03 ].GetExistTray_RearPos_Sen( DEF_EXIST ) == ERR_NO_ERROR )
		{
			//bConvWorkEnable = true;
		}
	}

	if( pMotor->MotionDone() != ERR_NO_ERROR)
	{
// 		if( dCommandPos_um <= dCurrentPos_um ) {
// 			// stacker 방향으로 이동 중.
// 			if( bConvWorkEnable ) {
// 				return eWorkSite_Conveyor;
// 			}
// 		}

		return eWorkSite_NotAvaliable;
	} 
	else if( pMotor->MotionDone() == ERR_NO_ERROR &&
			  fabs( dCurrentPos_um - dCommandPos_um ) < MOTOR_INPOSITION )
	{
// 		if( bConvWorkEnable ) {
// 			return eWorkSite_Conveyor;
// 		}
		auto nData  = GetTrayExist_Data();
		auto nData1 = get_cyl_UpDown(eUp);
		auto nData2 = get_cyl_ClampUnclamp(eClamp);
		auto nData3	= get_cyl_align_ForBack(eForw);


		if (GetTrayExist_Data() == true &&
			get_cyl_UpDown(eUp) == ERR_NO_ERROR &&
			get_cyl_ClampUnclamp(eClamp) == ERR_NO_ERROR &&
			get_cyl_align_ForBack(eForw) == ERR_NO_ERROR)
		{
			return eWorkSite_Feeder;
		}

//		double dFeederAreaY_um = 400.0/*mm*/ * 1000.0; //314
// 		if( ( dLoadPos_um - dFeederAreaY_um ) < dCurrentPos_um && dCurrentPos_um <= dLoadPos_um ) {
// 			if( GetTrayExist_Data() == true &&
// 				get_cyl_UpDown( eUp ) == ERR_NO_ERROR &&
// 				get_cyl_ClampUnclamp( eClamp ) == ERR_NO_ERROR &&
// 				get_cyl_align_ForBack( eForw ) == ERR_NO_ERROR )
// 			{
// 				return eWorkSite_Feeder;
// 			}
// 		}
	}

	return eWorkSite_NotAvaliable;
}

double CTaskTrayFeeder::GetTeachPos( eMotor motor_idx, eLocIdxFeeder nLocStage )
{
	ASSERT( 0 <= nLocStage && nLocStage < eLocIdxFeeder::MAX_TRAY_FEEDER_LOC_COUNT );

	double dTPos = 0;
	dTPos = g_DMCont.m_Teach.GetTeachPos(RID_FEEDER_(RIDX_FEEDER), nLocStage, ePOS_IDX_FEEDER::POS_IDX_FEEDER_Y);

	return dTPos;
}

void CTaskTrayFeeder::ThreadProc1()
{
	SetThreadInterval( eTHREAD_1, 50 );
	while( GetThreadAliveFlag( eTHREAD_1 ) )
	{
		if( GetPauseFlag( eTHREAD_1 ) )
		{
			NEXT;
			continue;
		}

		for( int i = 0; i < MAX_THREAD_SCAN_COUNT; i++ )
		{
			CBaseTaskManager::SeqCmdRunning();
		}
		NEXT;
	}
}

void CTaskTrayFeeder::MakeLog( LPCTSTR fmt, ... )
{
	TCHAR tmpMsg[ 1024 ] = { 0, };
	if( fmt )
	{
		va_list argList;
		va_start( argList, fmt );
		_vstprintf_s( tmpMsg, fmt, argList );
		va_end( argList );
	}
	TWLOG.MakeLog( static_cast< int >( eLogID_TrayFeeder ), Debug, "", __LINE__, NULL, tmpMsg );
}

OneAxis* CTaskTrayFeeder::GetAxis( eMotor nAxisIdx )
{
	ASSERT( m_vMotor.empty() == false );
	ASSERT( ( nAxisIdx >= 0 ) && ( nAxisIdx < m_vMotor.size() ) );

	return m_vMotor[ nAxisIdx ];
}

stErrCode CTaskTrayFeeder::ChkDI( eDI nIndex, eOnOffState bValue )
{
	ASSERT( nIndex >= ( eDI )0 && nIndex < ( eDI )m_vX.size() );

	stErrCode err;

	if( g_IO.GetDriverType() == _DIO_TYPE_SIMUL_ ) {
		g_IO.SetSimulDi( m_vX[ ( int )nIndex ], bValue );
		Sleep( SIMUL_SLEEP_TIME );
	}

	auto in_state = g_IO.in( m_vX[ ( int )nIndex ] );
	if( in_state != bValue ) {
		err.ErrCode = -1;
		char msg[ MAX_PATH ] = { 0, };
		std::snprintf( msg, sizeof( msg ), "%s %s", m_mapDI[ ( int )nIndex ].GetString(), in_state == eOff ? "Off" : "On" );
		err.msg.append( msg );
	}

	return err;
}

stErrCode CTaskTrayFeeder::ChkDI( std::initializer_list< std::pair< eDI, eOnOffState > > nIndexList )
{
	stErrCode err;

	for( auto p : nIndexList ) {
		stErrCode chk = ChkDI( p.first, p.second );
		if( chk != ERR_NO_ERROR ) {
			err.ErrCode = -1;
			err.msg.append( chk.msg );
			err.msg.append( "\n" );
		}
	}

	return err;
}

stOnOffState CTaskTrayFeeder::GetDI( eDI nIndex )
{
	ASSERT( nIndex >= ( eDI )0 && nIndex < ( eDI )m_vX.size() );
	stOnOffState st;
	st.state = ( eOnOffState )g_IO.in( m_vX[ ( int )nIndex ] );
	std::snprintf( st.msg, sizeof( st.msg ), "%s %s", m_mapDI[ ( int )nIndex ].GetString(), st.state == eOff ? "Off" : "On" );

	return st;
}

stErrCode CTaskTrayFeeder::GetDI( std::initializer_list< std::pair< eDI, eOnOffState > > nIndexList )
{
	stErrCode err;

	for( auto p : nIndexList ) {
		stOnOffState state = GetDI( p.first );
		if( state != p.second ) {
			err.ErrCode = -1;
			err.msg.append( state.msg );
			err.msg.append( "\n" );
		}
	}

	return err;
}

int CTaskTrayFeeder::SetDO( eDO nIndex, eOnOffState eOnOff )
{
	ASSERT( nIndex >= ( eDO )0 && nIndex < ( eDO )m_vY.size() );

	g_IO.out( m_vY[ ( int )nIndex ], eOnOff );

	return ERR_NO_ERROR;
}

int CTaskTrayFeeder::SetDO( std::initializer_list< std::pair< eDO, eOnOffState > > nIndexList )
{
	for( auto p : nIndexList ) {
		int ret = SetDO( p.first, p.second );
		if( ret != ERR_NO_ERROR ) {
			return ret;
		}
	}

	return ERR_NO_ERROR;
}

stOnOffState CTaskTrayFeeder::GetDO( eDO nIndex )
{
	ASSERT( nIndex >= ( eDO )0 && nIndex < ( eDO )m_vY.size() );

	stOnOffState st;
	
	st.state = g_IO.out( m_vY[ ( int )nIndex ] ) == 0 ? eOff : eOn;
	std::snprintf( st.msg, sizeof( st.msg ), "%s %s", m_mapDO[ ( int )nIndex ].GetString(), st.state == eOff ? "Off" : "On" );
	
	return st;
}

stErrCode CTaskTrayFeeder::GetDO( std::initializer_list< std::pair< eDO, eOnOffState > > nIndexList )
{
	stErrCode err;

	for( auto p : nIndexList ) {
		stOnOffState state = GetDO( p.first );
		if( state != p.second ) {
			err.ErrCode = -1;
			err.msg.append( state.msg );
			err.msg.append( "\n" );
		}
	}

	return err;
}

BOOL CTaskTrayFeeder::ChkFeederFinalRow(int nCurStep)
{
	int TrayDiv_Y = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);
	BOOL bChkFinalRow = TrayDiv_Y % 2;
	int nFinalRow = TrayDiv_Y / 2;
	int nFinalDiv = 0;
	if (bChkFinalRow == TRUE && nFinalRow == nCurStep) {
		return TRUE;
	}

	return FALSE;
}


bool CTaskTrayFeeder::is_safety_state()
{
	CBaseSeqCmd::eSeqCmdStatus cmd_status = (CBaseSeqCmd::eSeqCmdStatus)this->GetSeqCmdStatus();

	int motion_done = GetAxis(eAxisY)->MotionDone();
	bool bSafetyPos = false;
	bool bSafetySensor = this->GetSafetySensor();

	OneAxis* pMotorfeeder = g_TaskTrayFeeder.GetAxis(eAxisY);
	double dCurPosY = 0.0;
	pMotorfeeder->GetCurrentPos(dCurPosY);

	if (dCurPosY > INTERLOCK_FEEDER_SAFETY_POS_ENC)
		bSafetyPos = true;


	if (cmd_status == CBaseSeqCmd::eSeqCmdStatus_Ready
		&& motion_done == NO_ERROR
		&& bSafetySensor == true
		&& bSafetyPos == true)
	{
		return true;
	}

	return false;
}

bool CTaskTrayFeeder::reserve_is_safety_state()
{
	bool bSafetyPos = false;
	bool bSafetySensor = this->GetSafetySensor();

	OneAxis* pMotorfeeder = g_TaskTrayFeeder.GetAxis(eAxisY);
	double dCurPosY = 0.0;
	pMotorfeeder->GetCurrentPos(dCurPosY);

	if (dCurPosY > INTERLOCK_FEEDER_SAFETY_POS_ENC)
		bSafetyPos = true;


	if (bSafetySensor == true
		&& bSafetyPos == true)
	{
		return true;
	}

	return false;
}


int CTaskTrayFeeder::GetFeederStatus()
{
	int nRetStatus = -1;
	int feeder_up = get_cyl_UpDown(eUp);
	auto feeder_clamp = chk_cyl_ClampUnclamp(eClamp);
	
	if (feeder_up == ERR_NO_ERROR)
	{
		if (feeder_clamp == ERR_NO_ERROR)	nRetStatus = eFeederStatus_Up_Clamp;
		else								nRetStatus = eFeederStatus_Up_Unclamp;
	}
	else
	{
		if (feeder_clamp == ERR_NO_ERROR)	nRetStatus = eFeederStatus_Down_Clamp;
		else								nRetStatus = eFeederStatus_Down_Unclamp;
	}

	return nRetStatus;
}


int CTaskTrayFeeder::do_VATMovePos(double dTargetPos, double dOffset/* == 0.0*/)
{
	OneAxis* pMotor = GetAxis(eAxisY);
	if (pMotor == nullptr) {
		return ERR_MOTOR_DATA_IS_NULL_POINTER;
	}

	double dRateSpd = DEF_VAT_MOTOR_SPEED_RATE * 2; // g_DMCont.m_dmFeeder.GN(NDM10_SPD_Feeder_Y);
	int nRateAcc = DEF_VAT_MOTOR_ACC_RATE * 2;//g_DMCont.m_dmFeeder.GN(NDM10_ACC_Feeder_Y);
	int nRateDec = DEF_VAT_MOTOR_DCC_RATE * 2;//g_DMCont.m_dmFeeder.GN(NDM10_DCC_Feeder_Y);

	double dMtSpd = pMotor->GetSpeed();
	double dMtAcc = pMotor->GetAcc();
	double dMtDec = pMotor->GetDcc();

	double dRetSpd = (dMtSpd * (dRateSpd / 100.0)); /** (nSeqSpdRate / 100.0);*/
	int nRetAcc = (int)(dMtAcc - (dMtAcc * 0.75) * (nRateAcc / 100.0));
	int nRetDec = (int)(dMtDec - (dMtDec * 0.75) * (nRateDec / 100.0));

	if (nRetAcc < 100) nRetAcc = 100;
	if (nRetDec < 100) nRetDec = 100;


	double dFTPos = dTargetPos + dOffset;

	MakeLog("%s : Final Target Pos : %.3fmm, Org Target Pos : %.3fmm, Pos Offset : %.3fmm", pMotor->GetAxisName(), dFTPos / 1000.0, dTargetPos / 1000.0, dOffset / 1000.0);

	int nErr = pMotor->Move(dFTPos, dRetSpd, nRetAcc, nRetDec, TRUE);

	return nErr;
}


int CTaskTrayFeeder::ChkVATMotorPos(double dTPos)
{
	OneAxis* pMotor = (OneAxis*)GetAxis(eAxisY);
	if (pMotor == NULL)
		return -3;

	double dCurPosY = 0.0;
	pMotor->GetCurrentPos(dCurPosY);

	if (pMotor->MotionDone() != ERR_NO_ERROR) {
		return -1;
	}

	if (fabs(dCurPosY - dTPos) > MOTOR_INPOSITION) {
		return -2;
	}

	return ERR_NO_ERROR;
}