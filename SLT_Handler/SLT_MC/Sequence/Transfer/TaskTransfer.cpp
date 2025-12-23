#include "stdafx.h"
#include "TaskTransfer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CTaskTransfer::CTaskTransfer()
{
	m_bInitialized = false;

	// DI
	m_mapDI.SetAt( xTRANSFER_LEFT_CLAMP_SEN    , "TRANSFER_LEFT_CLAMP_SEN"    );
	m_mapDI.SetAt( xTRANSFER_LEFT_UNCLAMP_SEN  , "TRANSFER_LEFT_UNCLAMP_SEN"  );
	m_mapDI.SetAt( xTRANSFER_RIGHT_CLAMP_SEN   , "TRANSFER_RIGHT_CLAMP_SEN"   );
	m_mapDI.SetAt( xTRANSFER_RIGHT_UNCLAMP_SEN , "TRANSFER_RIGHT_UNCLAMP_SEN" );
	m_mapDI.SetAt( xTRANSFER_TRAY_EXIST_SEN    , "TRANSFER_TRAY_EXIST_SEN"    );
	m_mapDI.SetAt( xTRANSFER_UP_SEN            , "TRANSFER_UP_SEN"            );
	m_mapDI.SetAt( xTRANSFER_DOWN_SEN          , "TRANSFER_DOWN_SEN"          );

	// DO
	m_mapDO.SetAt( yTRANSFER_CLAMP_SOL         , "TRANSFER_CLAMP_SOL"         );
	m_mapDO.SetAt( yTRANSFER_UNCLAMP_SOL       , "TRANSFER_UNCLAMP_SOL"       );
	m_mapDO.SetAt( yTRANSFER_UP_SOL            , "TRANSFER_UP_SOL"            );
	m_mapDO.SetAt( yTRANSFER_DOWN_SOL          , "TRANSFER_DOWN_SOL"          );


	// Motor
	m_mapMotor.SetAt( AXIS_TRANSFER_X          , "AXIS_TRANSFER_X"            );
}


CTaskTransfer::~CTaskTransfer()
{
}

int CTaskTransfer::Initialize( TCHAR * szDriverFileName )
{
	CConfigData ConfigData( szDriverFileName );
	char szSection[ 128 ] = { 0, };

	// LOG File Name
	char szLogFileName[ 512 ] = { 0, };
	char szBuff[ 512 ] = { 0, };
	char szDefaultValue[ 128 ] = { 0, };

	sprintf_s( szSection, sizeof( szSection ), "TASK TRANSFER CONFIG" );
	sprintf_s( szDefaultValue, sizeof( szDefaultValue ), "TaskTransfer.log" );

	memset( szSection, 0x00, sizeof( szSection ) );
	ConfigData.GetString( szSection, "Log File Name", szDefaultValue, szBuff );
	sprintf_s( szLogFileName, sizeof( szLogFileName ), "%s%s", SZ_LOG_FILE_PATH, szBuff );
	m_Logger.SetFileName( szLogFileName );

	// Digital Input
	memset( szSection, 0x00, sizeof( szSection ) );
	sprintf_s( szSection, sizeof( szSection ), "TASK TRANSFER CONFIG - DI" );
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
	sprintf_s( szSection, sizeof( szSection ), "TASK TRANSFER CONFIG - DO" );
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
	sprintf_s( szSection, sizeof( szSection ), "TASK TRANSFER CONFIG - MOTOR" );
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

	MakeLog( _T( "CTaskTransfer Init Complete" ) );

	return CBaseTaskManager::Initialize();
}

void CTaskTransfer::Finalize()
{
	CBaseTaskManager::Finalize();
}

void CTaskTransfer::ThreadProc1()
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

int CTaskTransfer::OnEvent( const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList /*= NULL*/ )
{
	return 0;
}

void CTaskTransfer::OnOperatorCommand( int nOperatorCommand, WPARAM wParam, LPARAM lParam )
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

void CTaskTransfer::MakeLog( LPCTSTR fmt, ... )
{
	TCHAR tmpMsg[ 1024 ] = { 0, };
	if( fmt )
	{
		va_list argList;
		va_start( argList, fmt );
		_vstprintf_s( tmpMsg, fmt, argList );
		va_end( argList );
	}
	TWLOG.MakeLog( static_cast< int >( eLogID_Transfer ), Debug, "", __LINE__, NULL, tmpMsg );
}

double CTaskTransfer::GetTeachPos( eLocIdxTransfer nLocStage, CTaskTransfer::eAxis _axis )
{
	ASSERT( 0 <= nLocStage && nLocStage < MAX_TRANSFER_LOC_COUNT );

	ST_LOCATION st_location;
	g_DMCont.m_Teach.GetTeachPos( RID_TRANSFER_( RIDX_TRANSFER ), nLocStage, &st_location );
	double dPos = st_location.dPos[ _axis ];

	return dPos;
}

int CTaskTransfer::do_MovePos( eAxis eAxisNo, eLocIdxTransfer nLocation, double dOffset_um /*= 0.0*/, int nSeqSpdRate /*= 100*/ )
{
	ASSERT( nSeqSpdRate >= 0 && nSeqSpdRate <= 100 );
	ASSERT( 0 <= eAxisNo && eAxisNo < eMaxAxisCount );

	double dTPos_um = GetTeachPos( nLocation, eAxisNo );

	OneAxis* pMotor = ( OneAxis* )m_vMotor[ eAxisNo ];

	double dRateSpd = ( double )g_DMCont.m_dmTransfer.GN( NDM9_SPD_Transfer_X );
	int    nRateAcc = g_DMCont.m_dmTransfer.GN( NDM9_ACC_Transfer_X );
	int    nRateDcc = g_DMCont.m_dmTransfer.GN( NDM9_DCC_Transfer_X );

	double dMtSpd = pMotor->GetSpeed();
	double dMtAcc = pMotor->GetAcc();
	double dMtDcc = pMotor->GetDcc();

	double dRetSpd = 0;
	int    nRetAcc = 0, nRetDcc = 0;

	dRetSpd = ( dMtSpd * ( dRateSpd / 100.0 ) ) * ( ( double )nSeqSpdRate / 100.0 );
	nRetAcc = ( int )( dMtAcc - ( dMtAcc*0.75 )*( ( double )nRateAcc / 100.0 ) ); // 설정되어 있는 값에서 spd rate가 높을 수록 acc/dcc를 감소 시켜준다. 가장 빠를 때가 설정 속도의 95%뺀 acc/dcc로 함.
	nRetDcc = ( int )( dMtDcc - ( dMtDcc*0.75 )*( ( double )nRateDcc / 100.0 ) );

	if( nRetAcc < 200 ) nRetAcc = 200;
	if( nRetDcc < 200 ) nRetDcc = 200;

	int nErr = pMotor->Move( ( dTPos_um + dOffset_um ), dRetSpd, nRetAcc, nRetDcc );

	return nErr;
}

int CTaskTransfer::ChkAxisDone( eAxis eAxisNo )
{
	OneAxis* pMotor = GetAxis( eAxisNo );
	return pMotor->AxisDone();
}

OneAxis* CTaskTransfer::GetAxis( int nAxisIdx )
{
	ASSERT( ( nAxisIdx >= 0 ) && ( nAxisIdx < CTaskTransfer::eMaxAxisCount ) );

	OneAxis* pMotor = NULL;
	pMotor = ( OneAxis* )m_vMotor[ nAxisIdx ];
	return pMotor;
}

int CTaskTransfer::GetAxisCount()
{
	return m_vMotor.size();
}

int CTaskTransfer::ChkDI( int nIndex, BOOL bValue /*= TRUE*/ )
{
	ASSERT( nIndex >= 0 && nIndex < ( int )m_vX.size() );

	if( g_IO.GetDriverType() == _DIO_TYPE_SIMUL_ ) {
		g_IO.SetSimulDi( m_vX[ nIndex ], bValue );
		Sleep(SIMUL_SLEEP_TIME);
		while( g_IO.in[ m_vX[ nIndex ] ] != bValue ) {
			g_IO.SetSimulDi( m_vX[ nIndex ], bValue );
			Sleep(SIMUL_SLEEP_TIME);
		}
	}

	if( g_IO.in[ m_vX[ nIndex ] ] != bValue ) {
		return -1;
	}

	return ERR_NO_ERROR;
}

BOOL CTaskTransfer::GetDI( int nIndex )
{
	ASSERT( nIndex >= 0 && nIndex < ( int )m_vX.size() );

	return ( BOOL )g_IO.in[ m_vX[ nIndex ] ];
}

int CTaskTransfer::GetDI( int nIndex, BOOL bValue )
{
	ASSERT( nIndex >= 0 && nIndex < ( int )m_vX.size() );

	if( ( ( BOOL )g_IO.in[ m_vX[ nIndex ] ] ) == bValue )
		return ERR_NO_ERROR;

	return -1;
}

int CTaskTransfer::do_cyl_ClampUnclamp( int nClampUnclamp )
{
	int nClampOper = ( nClampUnclamp == DEF_CLAMP ) ? DEF_ON : DEF_OFF;
	int nUnclampOper = ( nClampUnclamp == DEF_UNCLAMP ) ? DEF_ON : DEF_OFF;

	g_IO.out( m_vY[ yTRANSFER_CLAMP_SOL ], nClampOper );
	g_IO.out( m_vY[ yTRANSFER_UNCLAMP_SOL ], nUnclampOper );

	return ERR_NO_ERROR;
}

int CTaskTransfer::chk_cyl_ClampUnclamp( int nClampUnclamp )
{
	int nClampOper = ( nClampUnclamp == DEF_CLAMP ) ? DEF_ON : DEF_OFF;
	int nUnclampOper = ( nClampUnclamp == DEF_UNCLAMP ) ? DEF_ON : DEF_OFF;

	int nErr = ERR_NO_ERROR;
	nErr |= ChkDI( xTRANSFER_LEFT_CLAMP_SEN, nClampOper );
	nErr |= ChkDI( xTRANSFER_LEFT_UNCLAMP_SEN, nUnclampOper );
	nErr |= ChkDI( xTRANSFER_RIGHT_CLAMP_SEN, nClampOper );
	nErr |= ChkDI( xTRANSFER_RIGHT_UNCLAMP_SEN, nUnclampOper );

	return nErr;
}

int CTaskTransfer::get_cyl_ClampUnclamp( int nClampUnclamp )
{
	int nClampOper = ( nClampUnclamp == DEF_CLAMP ) ? DEF_ON : DEF_OFF;
	int nUnclampOper = ( nClampUnclamp == DEF_UNCLAMP ) ? DEF_ON : DEF_OFF;

	int nErr = ERR_NO_ERROR;
	nErr |= GetDI( xTRANSFER_LEFT_CLAMP_SEN, nClampOper );
	nErr |= GetDI( xTRANSFER_LEFT_UNCLAMP_SEN, nUnclampOper );
	nErr |= GetDI( xTRANSFER_RIGHT_CLAMP_SEN, nClampOper );
	nErr |= GetDI( xTRANSFER_RIGHT_UNCLAMP_SEN, nUnclampOper );

	return nErr;
}

int CTaskTransfer::do_cyl_UpDown( int nUpDown )
{
	int nUpOper = ( nUpDown == DEF_UP ) ? DEF_ON : DEF_OFF;
	int nDownOper = ( nUpDown == DEF_DOWN ) ? DEF_ON : DEF_OFF;

	for each( auto pMotor in m_vMotor ) {
		if( pMotor->MotionDone() != ERR_NO_ERROR) {
			return ERR_TRNASFER_UP_DOWN_FAIL_BECAUSE_MOTOR_IS_NOT_MOTION_DONE;
		}
	}

	if( nUpDown == DEF_DOWN ) {
		bool bCorrectPos = false;
		double dCurrentPos = 0;
		GetAxis(AXIS_TRANSFER_X)->GetCurrentPos(dCurrentPos);
		for( int pos = LOC_IDX_TRANSFER_CONV1; pos <= LOC_IDX_TRANSFER_CONV7; pos++ ) {
			double dTeachPos = GetTeachPos( ( eLocIdxTransfer )pos, AXIS_TRANSFER_X );
			if( fabs( dCurrentPos - dTeachPos ) < 1000.0 ) {
				bCorrectPos = true;
				break;
			}
		}
		if( bCorrectPos == false ) {
			return ERR_TRNASFER_UP_DOWN_FAIL_BECAUSE_CURRENT_POS_IS_NOT_TEACH_POS;
		}
	}

	g_IO.out( m_vY[ yTRANSFER_UP_SOL ], nUpOper );
	g_IO.out( m_vY[ yTRANSFER_DOWN_SOL ], nDownOper );

	return ERR_NO_ERROR;
}

int CTaskTransfer::chk_cyl_UpDown( int nUpDown )
{
	int nUpOper = ( nUpDown == DEF_UP ) ? DEF_ON : DEF_OFF;
	int nDownOper = ( nUpDown == DEF_DOWN ) ? DEF_ON : DEF_OFF;

	int nErr = ERR_NO_ERROR;
	nErr |= ChkDI( xTRANSFER_UP_SEN, nUpOper );
	nErr |= ChkDI( xTRANSFER_DOWN_SEN, nDownOper );

	return nErr;
}

int CTaskTransfer::get_cyl_UpDown( int nUpDown )
{
	int nUpOper = ( nUpDown == DEF_UP ) ? DEF_ON : DEF_OFF;
	int nDownOper = ( nUpDown == DEF_DOWN ) ? DEF_ON : DEF_OFF;

	int nErr = ERR_NO_ERROR;
	nErr |= GetDI( xTRANSFER_UP_SEN, nUpOper );
	nErr |= GetDI( xTRANSFER_DOWN_SEN, nDownOper );

	return nErr;
}

int CTaskTransfer::ChkTrayExist( int nExist )
{
	int nErr = ChkDI( xTRANSFER_TRAY_EXIST_SEN, ( nExist == DEF_EXIST ) ? DEF_OFF : DEF_ON );

	int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN( NDM11_SYS_MD_DEVICE_CHK );
	if( nDeviceMode == eNoDeviceNoTray )
	{
		nErr = ERR_NO_ERROR;
	}

	return nErr;
}

int CTaskTransfer::GetTrayExist( int nExist )
{
	int nRet = (GetDI( xTRANSFER_TRAY_EXIST_SEN, DEF_OFF) == ERR_NO_ERROR) ? DEF_EXIST : DEF_NOT_EXIST;
	return nRet;
}

int CTaskTransfer::GetTrayExist_DvcMode( int nExist )
{
	int nErr = ERR_UNKNOWN;
	if( g_DMCont.m_dmHandlerOpt.GN( NDM11_SYS_MD_DEVICE_CHK ) != eNoDeviceNoTray )
		nErr = GetTrayExist( nExist );
	else
		nErr = ( g_DMCont.m_dmDevice.GetTrayExist( eDeviceStage_Transfer ) == nExist ? ERR_NO_ERROR : ERR_UNKNOWN );

	return nErr;
}

bool CTaskTransfer::is_safety_state()
{
	CBaseSeqCmd::eSeqCmdStatus cmd_status = ( CBaseSeqCmd::eSeqCmdStatus )this->GetSeqCmdStatus();
	bool bHomeSensor = GetAxis( AXIS_TRANSFER_X )->IsHomeSensorOn() == TRUE;
	int motion_done = GetAxis( AXIS_TRANSFER_X )->MotionDone();

	if( cmd_status == CBaseSeqCmd::eSeqCmdStatus_Ready
		&& bHomeSensor == true
		&& motion_done == NO_ERROR )
	{
		return true;
	}

	return false;
}

