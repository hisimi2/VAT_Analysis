#include "StdAfx.h"
#include "TaskTrayLoadPP.h"


CTaskTrayLoadPP::CTaskTrayLoadPP()
{
	SetRobotIdIdx(eTrayPP_1_LD);

	NDM_TrayPP_RetryCnt = NDM5_LDTrayPP_RetryCnt;
	NDM_TM_TrayPP_CylDownWait = NDM5_TM_LDTrayPP_CylDownWait;
	NDM_TM_TrayPP_Vacuum = NDM5_TM_LDTrayPP_Vacuum;
	NDM_TM_TrayPP_Blow = NDM5_TM_LDTrayPP_Blow;

	m_eMyDeviceStageNum = eDeviceStage_TRAY_PP_1;

	char row[ 4 ] = { 'L','L','L','L' };
	m_TrayPP_picker_setting.add_row( row, 4 );
	m_TrayPP_picker_setting.add_row( row, 4 );

	ZeroMemory( m_nHand_i_VacSen, sizeof( m_nHand_i_VacSen ) );
	ZeroMemory( m_nHand_o_Vac, sizeof( m_nHand_o_Vac ) );
	ZeroMemory( m_nHand_o_Blw, sizeof( m_nHand_o_Blw ) );
	ZeroMemory( m_nHand_o_Pick, sizeof( m_nHand_o_Pick ) );
	ZeroMemory( m_nHand_o_Reset, sizeof( m_nHand_o_Reset ) );


	// DI
	m_mapDI.SetAt( xLOADER_TRAY_VACUUM_1_SEN, "xLOADER_TRAY_VACUUM_1_SEN" );
	m_mapDI.SetAt( xLOADER_TRAY_VACUUM_2_SEN, "xLOADER_TRAY_VACUUM_2_SEN" );
	m_mapDI.SetAt( xLOADER_TRAY_VACUUM_3_SEN, "xLOADER_TRAY_VACUUM_3_SEN" );
	m_mapDI.SetAt( xLOADER_TRAY_VACUUM_4_SEN, "xLOADER_TRAY_VACUUM_4_SEN" );
	m_mapDI.SetAt( xLOADER_TRAY_VACUUM_5_SEN, "xLOADER_TRAY_VACUUM_5_SEN" );
	m_mapDI.SetAt( xLOADER_TRAY_VACUUM_6_SEN, "xLOADER_TRAY_VACUUM_6_SEN" );
	m_mapDI.SetAt( xLOADER_TRAY_VACUUM_7_SEN, "xLOADER_TRAY_VACUUM_7_SEN" );
	m_mapDI.SetAt( xLOADER_TRAY_VACUUM_8_SEN, "xLOADER_TRAY_VACUUM_8_SEN" );

	// DO
	m_mapDO.SetAt( yLOADER_TRAY_SET1, "yLOADER_TRAY_SET1" );
	m_mapDO.SetAt( yLOADER_TRAY_SET2, "yLOADER_TRAY_SET2" );
	m_mapDO.SetAt( yLOADER_TRAY_SET3, "yLOADER_TRAY_SET3" );
	m_mapDO.SetAt( yLOADER_TRAY_SET4, "yLOADER_TRAY_SET4" );
	m_mapDO.SetAt( yLOADER_TRAY_SET5, "yLOADER_TRAY_SET5" );
	m_mapDO.SetAt( yLOADER_TRAY_SET6, "yLOADER_TRAY_SET6" );
	m_mapDO.SetAt( yLOADER_TRAY_SET7, "yLOADER_TRAY_SET7" );
	m_mapDO.SetAt( yLOADER_TRAY_SET8, "yLOADER_TRAY_SET8" );
	m_mapDO.SetAt( yLOADER_TRAY_RESET1, "yLOADER_TRAY_RESET1" );
	m_mapDO.SetAt( yLOADER_TRAY_RESET2, "yLOADER_TRAY_RESET2" );
	m_mapDO.SetAt( yLOADER_TRAY_RESET3, "yLOADER_TRAY_RESET3" );
	m_mapDO.SetAt( yLOADER_TRAY_RESET4, "yLOADER_TRAY_RESET4" );
	m_mapDO.SetAt( yLOADER_TRAY_RESET5, "yLOADER_TRAY_RESET5" );
	m_mapDO.SetAt( yLOADER_TRAY_RESET6, "yLOADER_TRAY_RESET6" );
	m_mapDO.SetAt( yLOADER_TRAY_RESET7, "yLOADER_TRAY_RESET7" );
	m_mapDO.SetAt( yLOADER_TRAY_RESET8, "yLOADER_TRAY_RESET8" );
	m_mapDO.SetAt( yLOADER_TRAY_BLOW1, "yLOADER_TRAY_BLOW1" );
	m_mapDO.SetAt( yLOADER_TRAY_BLOW2, "yLOADER_TRAY_BLOW2" );
	m_mapDO.SetAt( yLOADER_TRAY_BLOW3, "yLOADER_TRAY_BLOW3" );
	m_mapDO.SetAt( yLOADER_TRAY_BLOW4, "yLOADER_TRAY_BLOW4" );
	m_mapDO.SetAt( yLOADER_TRAY_BLOW5, "yLOADER_TRAY_BLOW5" );
	m_mapDO.SetAt( yLOADER_TRAY_BLOW6, "yLOADER_TRAY_BLOW6" );
	m_mapDO.SetAt( yLOADER_TRAY_BLOW7, "yLOADER_TRAY_BLOW7" );
	m_mapDO.SetAt( yLOADER_TRAY_BLOW8, "yLOADER_TRAY_BLOW8" );

	// Motor
	m_mapMotor.SetAt( eAxisX, "Axis X" );
	m_mapMotor.SetAt( eAxisY, "Axis Y" );
	m_mapMotor.SetAt( eAxisZ, "Axis Z" );
	m_mapMotor.SetAt( eAxisPitchX, "Axis Pitch X" );
	m_mapMotor.SetAt( eAxisPitchY, "Axis Pitch Y" );



	m_bInitialized = FALSE;
	m_bSkipToAssign = FALSE;

	m_vDevLossX.reserve( 30 );
	m_vDevLossY.reserve( 30 );

	m_vDevLossX.clear();
	m_vDevLossY.clear();

	//VAT
	m_hLog = NULL;

}

int CTaskTrayLoadPP::Initialize(int nTrayPPIdx, char* szDriverFileName)
{
	SetRobotIdIdx(nTrayPPIdx);

	CConfigData ConfigData( szDriverFileName );
	char szSection[ 128 ] = { 0, };

	// LOG File Name
	char szLogFileName[ 512 ] = { 0, };
	char szBuff[ 512 ] = { 0, };
	char szDefaultValue[ 128 ] = { 0, };

	memset( szSection, 0x00, sizeof( szSection ) );
	sprintf_s( szSection, sizeof( szSection ), "TASK TRAY LOAD PP CONFIG" );

	memset( szDefaultValue, 0x00, sizeof( szDefaultValue ) );
	sprintf_s( szDefaultValue, sizeof( szDefaultValue ), "TaskTrayLoadPP.log" );

	ConfigData.GetString( szSection, "Log File Name", szDefaultValue, szBuff );
	sprintf_s( szLogFileName, sizeof( szLogFileName ), "%s%s", SZ_LOG_FILE_PATH, szBuff );
	m_Logger.SetFileName( szLogFileName );

	// Digital Input
	memset( szSection, 0x00, sizeof( szSection ) );
	sprintf_s( szSection, sizeof( szSection ), "TASK TRAY LOAD PP CONFIG - DI" );
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
	sprintf_s( szSection, sizeof( szSection ), "TASK TRAY LOAD PP CONFIG - DO" );
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
	sprintf_s( szSection, sizeof( szSection ), "TASK TRAY LOAD PP CONFIG - MOTOR" );
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
		}
		else {
			m_vMotor.push_back( nullptr );
		}
	}

	for( int y = 0; y < MAX_TRAY_PP_CNT_Y; y++ ) {
		for( int x = 0; x < MAX_TRAY_PP_CNT_X; x++ ) {
			m_nHand_i_VacSen[ y ][ x ] = static_cast< CTaskTrayBasePP::eDI >( ( int )eDI::xLOADER_TRAY_VACUUM_1_SEN + ( y * MAX_TRAY_PP_CNT_X ) + x );
			m_nHand_o_Vac[ y ][ x ] = static_cast< CTaskTrayBasePP::eDO >( ( int )eDO::yLOADER_TRAY_RESET1 + ( y * MAX_TRAY_PP_CNT_X ) + x );
			m_nHand_o_Reset[ y ][ x ] = static_cast< CTaskTrayBasePP::eDO >( ( int )eDO::yLOADER_TRAY_SET1 + ( y * MAX_TRAY_PP_CNT_X ) + x );
			m_nHand_o_Blw[ y ][ x ] = static_cast< CTaskTrayBasePP::eDO >( ( int )eDO::yLOADER_TRAY_BLOW1 + ( y * MAX_TRAY_PP_CNT_X ) + x );
		}
	}
	//m_nVATvisionLED_o_ONOFF[ eUpperCam ] = static_cast< CTaskTrayBasePP::eDO >( m_vY[ ( int )eDO::yC_TRAY_HAND_VAT_CAM_UP ] );
	//m_nVATvisionLED_o_ONOFF[ eDownCam ] = static_cast< CTaskTrayBasePP::eDO >( m_vY[ ( int )eDO::yC_TRAY_HAND_VAT_CAM_DN ] );

	//VAT
	for (int i = VAT_PICKER_01; i < VAT_PICKER_MAX; i++) {
		m_nBbxmnpAxis[i] = eBMAxis_LOAD_TRAY_Picker01 + i;
	}

	return CBaseTaskManager::Initialize();
}

void CTaskTrayLoadPP::Finalize()
{
	CBaseTaskManager::Finalize();
}

void CTaskTrayLoadPP::OnOperatorCommand( int nOperatorCommand, WPARAM wParam, LPARAM lParam )
{
	switch( nOperatorCommand )
	{
		case TWMB_ID_SKIP:
		case TWMB_ID_P_SKIP:
		{
			SetProcessCommand( base_seq_cmd::eProcCmd_Skip );
		}break;
		case TWMB_ID_SKIP_TO_ASSIGN:
		{
			m_bSkipToAssign = TRUE;
			SetProcessCommand(base_seq_cmd::eProcCmd_Skip);
		}break;
		case TWMB_ID_PARTIAL_TRAYEND:
		{
			g_DMCont.m_dmTrayPP.SN(NDM5_TM_TrayPP_Tray_End, TRUE);

			int nPocketCntX = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_X);
			int nPocketCntY = g_DMCont.m_dmTrayPP.GN(NDM5_TrayDiv_Y);
			ST_DD_DEVICE stDevice;

			bool bFindAutoSkipDevice = false;

			for (int j = 0; j < nPocketCntY; j++)
			{
				for (int i = 0; i < nPocketCntX; i++)
				{
					stDevice.clear();
					//g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TRAY_FEEDER, i, j, &stDevice);
					/*if (stDevice.sExist == eDeviceData_None && strcmp(stDevice.szBinResult, DEF_TRAY_PICK_ERR_DEVICE) == 0)*/
					{
						StringCchPrintf(stDevice.szBinResult, _countof(stDevice.szBinResult), "");
						stDevice.sExist = eDeviceData_None;
						g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TRAY_FEEDER, i, j, &stDevice);
					}
				}
			}

			BOOL bAutoQAUse = g_TaskSystemCtrl.GetAutoQAUse();
			if (bAutoQAUse == TRUE) //QA Mode �δ� Tray End�� �Ǹ� ���� ���� �� �������� ���� �� �ش�.
			{
				g_TaskSystemCtrl.SetQAStatus(eQAStatus::eQAStatus_End);
			}

			SetProcessCommand(base_seq_cmd::eProcCmd_Stop);
		}break;
	}
}

int CTaskTrayLoadPP::WhereIsMotorPosXY( eLocIdxTrayPP& pos_xy )
{
	pos_xy = LOC_IDX_TRAY_PP_Undefined;

	// get need data
	OneAxis* pMotorX = this->GetAxis( eAxisX );

	bool bXCheckSkip = ( pMotorX == nullptr );

	// get all setplate teaching y pos
	double dTechPos_X = 0.0;
	double dTechPos_Y = 0.0;
	double dMargine = 500.0; // 0.5mm

	double dCurPosX = 0;
	pMotorX->GetCurrentPos(dCurPosX);

	// loading table width x height
	double dLoadingTableDiv[ 2 ] = { 0.0, };
	dLoadingTableDiv[ 0 ] = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Div_X );
	double dLoadingTablePitch[ 2 ] = { 0.0, };
	dLoadingTablePitch[ 0 ] = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Pitch_X );

	double dLdTblWidth = dLoadingTableDiv[ 0 ] * dLoadingTablePitch[ 0 ]; // um

	for( int i = 0; i < eMaxLoadTblCount; i++ )
	{
		std::vector<CPoint> vCmdPicker;
		vCmdPicker.push_back( CPoint( 0, 0 ) );

		dTechPos_X = GetTeachPos( static_cast< eLocIdxTrayPP >( LOC_IDX_TRAY_PP_TBL_1 + i ), CPoint( 0, 0 ), vCmdPicker, eZPOS_SAFETY, eAxisX );

		bool bRangeIn_X = false;

		if( ( dCurPosX >= ( dTechPos_X - dMargine ) ) && ( dCurPosX <= ( dTechPos_X + dLdTblWidth + dMargine ) ) )
		{
			bRangeIn_X = true;
		}

		if( bRangeIn_X )
		{
			pos_xy = static_cast< eLocIdxTrayPP >( LOC_IDX_TRAY_PP_TBL_1 + i );
			return ERR_NO_ERROR;
		}
	}

	// c-tray table width x height
	double dCTrayDiv[ 2 ] = { 0.0, };
	dCTrayDiv[ 0 ] = g_DMCont.m_dmTrayPP.GN( NDM5_TrayDiv_X );
	dCTrayDiv[ 1 ] = g_DMCont.m_dmTrayPP.GN( NDM5_TrayDiv_Y );

	double dCTrayPitch[ 2 ] = { 0.0, };
	dCTrayPitch[ 0 ] = g_DMCont.m_dmTrayPP.GN( NDM5_Tray_Pitch_X );
	dCTrayPitch[ 1 ] = g_DMCont.m_dmTrayPP.GN( NDM5_Tray_Pitch_Y );

	double dTrayWidth = dCTrayDiv[ 0 ] * dCTrayPitch[ 0 ]; // um
	double dTrayHeight = dCTrayDiv[ 1 ] * dCTrayPitch[ 1 ]; // um	



	// 현재 위치가 Margine을 포함한 C-Tray 크기 영역 안에 있는지 확인 한다.
	std::vector<CPoint> vCmdPicker;
	vCmdPicker.push_back( CPoint( 0, 0 ) );

	dTechPos_X = GetTeachPos( static_cast< eLocIdxTrayPP >(LOC_IDX_TRAY_PP_FEEDER), CPoint( 0, 0 ), vCmdPicker, eZPOS_SAFETY, eAxisX );

	bool bRangeIn_X = false;
	bool bRangeIn_Y = false;

	if( ( dCurPosX >= ( dTechPos_X - dMargine ) ) && ( dCurPosX <= ( dTechPos_X + dTrayWidth + dMargine ) ) )
	{
		bRangeIn_X = true;
	}

	if( bRangeIn_X )
	{
		pos_xy = static_cast< eLocIdxTrayPP >(LOC_IDX_TRAY_PP_FEEDER);
		return ERR_NO_ERROR;
	}

	return ERR_NO_ERROR;
}

BOOL CTaskTrayLoadPP::GetPickerAlignStatus(int nX, int nY)
{
	BOOL bAlignStatus = FALSE;

	std::vector<std::vector<int>> v = {
		{ BDM11_PICKER_LD_X1_Y1, BDM11_PICKER_LD_X2_Y1, BDM11_PICKER_LD_X3_Y1, BDM11_PICKER_LD_X4_Y1 },
		{ BDM11_PICKER_LD_X1_Y2, BDM11_PICKER_LD_X2_Y2, BDM11_PICKER_LD_X3_Y2, BDM11_PICKER_LD_X4_Y2 } };

	bAlignStatus = v[nY][nX];

	return bAlignStatus;
}

void CTaskTrayLoadPP::GetPickerOffset(double &dOffsetX, double &dOffsetY, int nX, int nY)
{
	std::vector<std::vector<CPoint>> v = {
	{
		CPoint(DDM11_PKR_LD_X0_Y0_OFFSETX , DDM11_PKR_LD_X0_Y0_OFFSETY),
		CPoint(DDM11_PKR_LD_X1_Y0_OFFSETX , DDM11_PKR_LD_X1_Y0_OFFSETY),
		CPoint(DDM11_PKR_LD_X2_Y0_OFFSETX , DDM11_PKR_LD_X2_Y0_OFFSETY),
		CPoint(DDM11_PKR_LD_X3_Y0_OFFSETX , DDM11_PKR_LD_X3_Y0_OFFSETY) 
	},
	{
		CPoint(DDM11_PKR_LD_X0_Y1_OFFSETX , DDM11_PKR_LD_X0_Y1_OFFSETY),
		CPoint(DDM11_PKR_LD_X1_Y1_OFFSETX , DDM11_PKR_LD_X1_Y1_OFFSETY),
		CPoint(DDM11_PKR_LD_X2_Y1_OFFSETX , DDM11_PKR_LD_X2_Y1_OFFSETY),
		CPoint(DDM11_PKR_LD_X3_Y1_OFFSETX , DDM11_PKR_LD_X3_Y1_OFFSETY) 
	} };

	int x = v[nY][nX].x;
	int y = v[nY][nX].y;

	dOffsetX = g_DMCont.m_dmHandlerOpt.GD(x);
	dOffsetY = g_DMCont.m_dmHandlerOpt.GD(y);
}

void CTaskTrayLoadPP::ApplyVATOffset()
{

}

bool CTaskTrayLoadPP::is_safety_state()
{
	bool bSafetyPos = false;
	OneAxis* pMotorX = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisX);
	BOOL bOrigin = pMotorX->IsOrgCompleted(nullptr);

	OneAxis* pMotorPitchY = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisPitchY);
	BOOL bOriginPitchY = pMotorPitchY->IsOrgCompleted(nullptr);

	if (bOrigin == TRUE && bOriginPitchY == TRUE) {
		double dLoadTablePitchY = (double)g_DMCont.m_dmTrayPP.GN(NDM5_TrayPP_LdTbl_Pitch_Y);

		double dCurPos_um = 0.0, dCurPitchY_um = 0.0, dPosiLimitPitchY = 0.0;

		pMotorX->GetCurrentPos(dCurPos_um);
		pMotorPitchY->GetCurrentPos(dCurPitchY_um);

		dPosiLimitPitchY = pMotorPitchY->GetSwLimitPosi();

		//double dWorkEnableMin_um = MIN_TRAY_PP_WORK_ENABLE_POS_um - (DEF_TRAY_PP_MAX_Y_PITCH_WIDE - dLoadTablePitchY - 10000.0);
		//double dWorkEnablePitchY = dPosiLimitPitchY - (60000.0 - dLoadTablePitchY);
		//if ((dWorkEnableMin_um < dCurPos_um && dCurPitchY_um < dWorkEnablePitchY) || MIN_TRAY_PP_WORK_ENABLE_POS_um < dCurPos_um) {

		double dWorkEnableMin_um = INTERLOCK_UD_TRAYPP_Y_WORK_ENABLE_POS_TRANSFER*(-1);
		if (dCurPos_um < dWorkEnableMin_um) {//MIN_TRAY_PP_WORK_ENABLE_POS_um) {	
			bSafetyPos = true;
		}
	}

	if (bSafetyPos == true) {
		return true;
	}

	return false;
}

void CTaskTrayLoadPP::MakeLog( LPCTSTR fmt, ... )
{
	TCHAR tmpMsg[ 1024 ] = { 0, };
	if( fmt )
	{
		va_list argList;
		va_start( argList, fmt );
		_vstprintf_s( tmpMsg, fmt, argList );
		va_end( argList );
	}
	TWLOG.MakeLog( static_cast< int >( eLogID_TrayPP_1 ), Debug, "", __LINE__, NULL, tmpMsg );
}

void CTaskTrayLoadPP::MakeVatLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = { 0, };
	if (fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf_s(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_VAT_Tray_1), Debug, "", __LINE__, NULL, tmpMsg);
}

void CTaskTrayLoadPP::ThreadProc1()
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

int CTaskTrayLoadPP::ChkDI( eDI nIndex, eOnOffState bValue )
{
	return CTaskTrayBasePP::ChkDI( ( CTaskTrayBasePP::eDI )nIndex, bValue );
}

eOnOffState CTaskTrayLoadPP::GetDI( eDI nIndex )
{
	return CTaskTrayBasePP::GetDI( ( CTaskTrayBasePP::eDI )nIndex );
}

int CTaskTrayLoadPP::SetDO( eDO nIndex, eOnOffState eOnOff )
{
	return CTaskTrayBasePP::SetDO( ( CTaskTrayBasePP::eDO )nIndex, eOnOff );
}

void CTaskTrayLoadPP::ApplyBacklash()
{
	for (int i = eSpeed_5; i < eSpeed_MAX; i++) {
		g_Motor[eAXIS_TRAY_PP_1_X]->SetSpeedBacklash(i, g_VatBacklash.trayloadpp.m_BacklashOffset_XY[i].x);		
		g_Motor[eAXIS_TRAY_FEEDER_Y]->SetSpeedBacklash(i, g_VatBacklash.trayloadpp.m_BacklashOffset_XY[i].y);
	}
}

void CTaskTrayLoadPP::SetVatLogHandle(HWND hOwner)
{
	m_hLog = hOwner;
}

void CTaskTrayLoadPP::MakeVatPreparePos(double & dX, double & dY, double dPrepareOffset)
{
	double dNegaLimitX = m_vMotor[CTaskTrayLoadPP::eAxisX]->GetSwLimitNega();
	double dNegaLimitY = g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eAxisY)->GetSwLimitNega();

	double dPreparePosX = dX - dPrepareOffset;
	double dPreparePosY = dY - dPrepareOffset;

	dX = (dPreparePosX <= dNegaLimitX) ? (dNegaLimitX) : (dPreparePosX);
	dY = (dPreparePosY <= dNegaLimitY) ? (dNegaLimitY) : (dPreparePosY);
}

bool CTaskTrayLoadPP::IsQALoadPickEnd(int nQaSampleCnt, int nLotLoadCnt, bool bAutoQAUse, eQAStatus nQAStatus)
{
	return (nQaSampleCnt == nLotLoadCnt)
		&& bAutoQAUse
		&& (nQAStatus == eQAStatus::eQAStatus_Load);
}

int CTaskTrayLoadPP::GetDeviceFullCount()
{
	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	int nNumberOfLoadTable = g_DMCont.m_dmHandlerOpt.GN(NDM11_NUMBER_OF_LOADTABLE);

	int nMaxCount = MAX_TRAY_PP_CNT_X * MAX_TRAY_PP_CNT_Y;

	if (nSiteDivY == 1 && nNumberOfLoadTable == _eNumberOfLoadTable::eNumberOfLoadTable_4Unit)
		return nMaxCount / 2;

	return nMaxCount;
}

void CTaskTrayLoadPP::WriteDisplayLog(const char * fmt, ...)
{
	char szBuff[MAX_BUFF] = { 0, };

	va_list va;
	va_start(va, fmt);
	vsprintf(szBuff, fmt, va);
	va_end(va);

	::SendMessage(m_hLog, UM_VAT_SEQ_MSG_PRINT_LOG, (WPARAM)VAT_HAND_LOAD_TRAYPP, (LPARAM)szBuff);
}