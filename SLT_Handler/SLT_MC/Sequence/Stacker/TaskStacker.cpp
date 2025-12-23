#include "stdafx.h"
#include "TaskStacker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CTaskStacker::CTaskStacker()
{
	m_eStackerIdx = eSTACKER_01;
	m_eStackerType = eStackerType_Undefined;
}


CTaskStacker::~CTaskStacker()
{
}

CString StrFmt( const char* _format, ... )
{
	TCHAR tmpMsg[ 1024 ] = { 0, };
	if( _format )
	{
		va_list argList;
		va_start( argList, _format );
		_vstprintf_s( tmpMsg, _format, argList );
		va_end( argList );
	}

	return tmpMsg;
}

int CTaskStacker::Initialize( TCHAR * szDriverFileName, eStacker_Idx stacker_idx, eStacker_Type stacker_type )
{
	m_eStackerIdx = stacker_idx;
	m_eStackerType = stacker_type;

	// DI
	m_mapDI.SetAt(xC_TRAY_HOLD_FOR_SEN         , StrFmt("STCKR_C-TRAY_HOLD_FOR_SEN"        ) );
	m_mapDI.SetAt(xC_TRAY_HOLD_BACK_SEN        , StrFmt("STCKR_C-TRAY_HOLD_BACK_SEN"       ) );
	m_mapDI.SetAt(xC_TRAY_ALIGN_FOR_SEN        , StrFmt("STCKR_C-TRAY_ALIGN_FOR_SEN"       ) );
	m_mapDI.SetAt(xC_TRAY_ALIGN_BACK_SEN       , StrFmt("STCKR_C-TRAY_ALIGN_BACK_SEN"      ) );
	m_mapDI.SetAt( xC_TRAY_CV_REAR_EXIST_SEN   , StrFmt("STCKR_C-TRAY_CV_REAR_EXIST_SEN"   ) );
	m_mapDI.SetAt( xC_TRAY_CV_REAR_SLOW_SEN    , StrFmt("STCKR_C-TRAY_CV_REAR_SLOW_SEN"    ) );	
	m_mapDI.SetAt( xC_TRAY_FRONT_LEFT_LOCK_SEN , StrFmt("STCKR_C-TRAY_FRONT_LEFT_LOCK_SEN" ) );
	m_mapDI.SetAt( xC_TRAY_REAR_LEFT_LOCK_SEN  , StrFmt("STCKR_C-TRAY_REAR_LEFT_LOCK_SEN"  ) );
	m_mapDI.SetAt( xC_TRAY_FRONT_RIGHT_LOCK_SEN, StrFmt("STCKR_C-TRAY_FRONT_RIGHT_LOCK_SEN") );
	m_mapDI.SetAt( xC_TRAY_REAR_RIGHT_LOCK_SEN , StrFmt("STCKR_C-TRAY_REAR_RIGHT_LOCK_SEN" ) );
	m_mapDI.SetAt( xC_TRAY_SUB_PUSHER_UP_SEN   , StrFmt("STCKR_C-TRAY_SUB_PUSHER_UP_SEN"   ) );
	m_mapDI.SetAt( xC_TRAY_SUB_PUSHER_DOWN_SEN , StrFmt("STCKR_C-TRAY_SUB_PUSHER_DOWN_SEN" ) );
	m_mapDI.SetAt( xC_TRAY_MAIN_PUSHER_UP_SEN  , StrFmt("STCKR_C-TRAY_MAIN_PUSHER_UP_SEN"  ) );
	m_mapDI.SetAt( xC_TRAY_MAIN_PUSHER_DOWN_SEN, StrFmt("STCKR_C-TRAY_MAIN_PUSHER_DOWN_SEN") );
	m_mapDI.SetAt( xC_TRAY_STACKER_FULL_SEN    , StrFmt("STCKR_C-TRAY_STACKER_FULL_SEN"    ) );
	m_mapDI.SetAt( xC_TRAY_STACKER_EXIST_SEN   , StrFmt("STCKR_C-TRAY_STACKER_EXIST_SEN"   ) );
	m_mapDI.SetAt( xC_TRAY_CV_FRONT_EXIST_SEN  , StrFmt("STCKR_C-TRAY_CV_FRONT_EXIST_SEN"  ) );
	m_mapDI.SetAt( xC_TRAY_CV_FRONT_SLOW_SEN   , StrFmt("STCKR_C-TRAY_CV_FRONT_SLOW_SEN"   ) );

	m_mapDI.SetAt( xCV_ALARM, StrFmt( "STCKR_CV_ALARM") );

	// DO
	m_mapDO.SetAt( yCV_HIGH_SPEED_CONTROL  , StrFmt( "STCKR_CV_HIGH_SPEED_CONTROL"  ) );
	m_mapDO.SetAt( yCV_FWD_CONTROL         , StrFmt( "STCKR_CV_FWD_CONTROL"         ) );
	m_mapDO.SetAt( yCV_REV_CONTROL         , StrFmt( "STCKR_CV_REV_CONTROL"         ) );
	m_mapDO.SetAt( yCV_C_TRAY_ALIGN_FOR    , StrFmt( "STCKR_CV_C-TRAY_ALIGN_FOR"    ) );
	m_mapDO.SetAt( yCV_C_TRAY_ALIGN_BACK   , StrFmt( "STCKR_CV_C-TRAY_ALIGN_BACK"   ) );	
	m_mapDO.SetAt( yC_TRAY_LOCK            , StrFmt( "STCKR_C-TRAY_LOCK"            ) );
	m_mapDO.SetAt( yC_TRAY_UNLOCK          , StrFmt( "STCKR_C-TRAY_UNLOCK"          ) );
	m_mapDO.SetAt( yC_TRAY_SUB_PUSHER_UP   , StrFmt( "STCKR_C-TRAY_SUB_PUSHER_UP"   ) );
	m_mapDO.SetAt( yC_TRAY_SUB_PUSHER_DOWN , StrFmt( "STCKR_C-TRAY_SUB_PUSHER_DOWN" ) );
	m_mapDO.SetAt( yC_TRAY_MAIN_PUSHER_UP  , StrFmt( "STCKR_C-TRAY_MAIN_PUSHER_UP"  ) );
	m_mapDO.SetAt( yC_TRAY_MAIN_PUSHER_DOWN, StrFmt( "STCKR_C-TRAY_MAIN_PUSHER_DOWN") );


	if( stacker_type == eStackerType_Unload ) {
		m_mapDO.SetAt( yCV_VIBRATOR_FOR, StrFmt( "STCKR_CV_VIBRATOR_FOR") );
		m_mapDO.SetAt( yCV_VIBRATOR_BACK, StrFmt( "STCKR_CV_VIBRATOR_BACK") );
	}


	CConfigData ConfigData( szDriverFileName );
	char szSection[ 128 ] = { 0, };

	// LOG File Name
	char szLogFileName[ 512 ] = { 0, };
	char szBuff[ 512 ] = { 0, };
	char szDefaultValue[ 128 ] = { 0, };

	sprintf_s( szSection, sizeof( szSection ), "TASK STACKER %d CONFIG", m_eStackerIdx + 1 );
	sprintf_s( szDefaultValue, sizeof( szDefaultValue ), "TaskStacker_%d.log", m_eStackerIdx + 1 );

	memset( szSection, 0x00, sizeof( szSection ) );
	ConfigData.GetString( szSection, "Log File Name", szDefaultValue, szBuff );
	sprintf_s( szLogFileName, sizeof( szLogFileName ), "%s%s", SZ_LOG_FILE_PATH, szBuff );
	m_Logger.SetFileName( szLogFileName );

	// Digital Input
	memset( szSection, 0x00, sizeof( szSection ) );
	sprintf_s( szSection, sizeof( szSection ), "TASK STACKER %d CONFIG - DI", m_eStackerIdx + 1 );
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
	sprintf_s( szSection, sizeof( szSection ), "TASK STACKER %d CONFIG - DO", m_eStackerIdx + 1 );
	int nDoCount = m_mapDO.GetCount();
	for( int i = 0; i < nDoCount; i++ ) {
		char szKey[ 128 ] = { 0, };
		CString strName;
		m_mapDO.Lookup( i, strName );
		sprintf_s( szKey, sizeof( szKey ), "%s", strName.GetString() );
		int nIdx = ConfigData.GetInt( szSection, szKey, "0" );
		m_vY.push_back( nIdx );
	}

	m_StackerTrayExistDelaySen.Initialize(m_vX[xC_TRAY_STACKER_EXIST_SEN], &g_IO, 3000.0);

	MakeLog( _T( "CTaskStacker Init Complete" ) );

	return CBaseTaskManager::Initialize();
}


void CTaskStacker::Finalize()
{
	CBaseTaskManager::Finalize();
}

void CTaskStacker::ThreadProc1()
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
			Sub_Application();
		}
		NEXT;
	}
}

void CTaskStacker::Sub_Application()
{
	m_StackerTrayExistDelaySen.do_CheckStable();
}

int CTaskStacker::GetLoadTrayStatus()
{
	int nSenSts = m_StackerTrayExistDelaySen.GetSensorState();

	return nSenSts;
}

int CTaskStacker::OnEvent( const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList /*= NULL*/ )
{
	return 0;
}

void CTaskStacker::OnOperatorCommand( int nOperatorCommand, WPARAM wParam, LPARAM lParam )
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

void CTaskStacker::MakeLog( LPCTSTR fmt, ... )
{
	TCHAR tmpMsg[ 1024 ] = { 0, };
	if( fmt )
	{
		va_list argList;
		va_start( argList, fmt );
		_vstprintf_s( tmpMsg, fmt, argList );
		va_end( argList );
	}
	TWLOG.MakeLog( static_cast< int >( eLogID_Stacker_1 + m_eStackerIdx ), Debug, "", __LINE__, NULL, tmpMsg );
}



int CTaskStacker::ChkDI( int nIndex, BOOL bValue /*= TRUE*/ )
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

	// First Chk
	if( g_IO.in[ m_vX[ nIndex ] ] != bValue ) {
		return -1;
	}

	return ERR_NO_ERROR;
}

BOOL CTaskStacker::GetDI( int nIndex )
{
	ASSERT( nIndex >= 0 && nIndex < ( int )m_vX.size() );

	return ( BOOL )g_IO.in[ m_vX[ nIndex ] ];
}

int CTaskStacker::GetDI( int nIndex, BOOL bValue )
{
	ASSERT( nIndex >= 0 && nIndex < ( int )m_vX.size() );

	if( ( ( BOOL )g_IO.in[ m_vX[ nIndex ] ] ) == bValue )
		return ERR_NO_ERROR;

	return -1;
}

int CTaskStacker::do_cyl_LockUnlock( int nLockUnlock )
{
	int nLockOper = ( nLockUnlock == DEF_LOCK ) ? DEF_ON : DEF_OFF;
	int nUnlockOper = ( nLockUnlock == DEF_UNLOCK ) ? DEF_ON : DEF_OFF;

	//int nFrontExistErr = this->GetExistTray_FrontPos_DvcMode( DEF_EXIST );
	//int nFrontNotExistErr = this->GetExistTray_FrontPos_DvcMode( DEF_NOT_EXIST );
	int nMainUpErr = this->get_cyl_MainPusher( DEF_UP );
	int nSubUpErr = this->get_cyl_SubPusher( DEF_UP );


	if( nMainUpErr != ERR_NO_ERROR ) {
		if( nUnlockOper == DEF_ON ) {
			return ERR_STACKER_1_INTERLOCK_CAN_NOT_UNLOCK + GetIndex();
		}
	}


	g_IO.out( m_vY[ yC_TRAY_LOCK ], nLockOper );
	g_IO.out( m_vY[ yC_TRAY_UNLOCK ], nUnlockOper );

	return ERR_NO_ERROR;
}

int CTaskStacker::chk_cyl_LockUnlock( int nLockUnlock )
{
	int nLockOper = ( nLockUnlock == DEF_LOCK ) ? DEF_ON : DEF_OFF;

	int nErr = ERR_NO_ERROR;
	nErr |= ChkDI( xC_TRAY_FRONT_LEFT_LOCK_SEN, nLockOper );
	nErr |= ChkDI( xC_TRAY_REAR_LEFT_LOCK_SEN, nLockOper );
	nErr |= ChkDI( xC_TRAY_FRONT_RIGHT_LOCK_SEN, nLockOper );
	nErr |= ChkDI( xC_TRAY_REAR_RIGHT_LOCK_SEN, nLockOper );

	return nErr;
}

int CTaskStacker::get_cyl_LockUnlock( int nLockUnlock )
{
	int nLockOper = ( nLockUnlock == DEF_LOCK ) ? DEF_ON : DEF_OFF;

	int nErr = ERR_NO_ERROR;
	nErr |= GetDI( xC_TRAY_FRONT_LEFT_LOCK_SEN, nLockOper );
	nErr |= GetDI( xC_TRAY_REAR_LEFT_LOCK_SEN, nLockOper );
	nErr |= GetDI( xC_TRAY_FRONT_RIGHT_LOCK_SEN, nLockOper );
	nErr |= GetDI( xC_TRAY_REAR_RIGHT_LOCK_SEN, nLockOper );

	return nErr;
}

int CTaskStacker::do_cyl_MainPusher( int nUpDn, BOOL bNoInterlock)
{
	int nUpOper = ( nUpDn == DEF_UP ) ? DEF_ON : DEF_OFF;
	int nDownOper = ( nUpDn == DEF_DOWN ) ? DEF_ON : DEF_OFF;

	int nFrontExistErr = this->ChkDI(xC_TRAY_CV_FRONT_EXIST_SEN, DEF_OFF);
	int nUnlockErr = this->chk_cyl_LockUnlock( DEF_UNLOCK );
	int nSubDownErr = this->chk_cyl_SubPusher( DEF_DOWN );

	BOOL vat_in_process = g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_USE);
	int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
	if ((nDeviceMode != eNoDeviceNoTray || vat_in_process) && nUpDn == DEF_UP)
	{
		int nFrontExistSensor = this->ChkDI(xC_TRAY_CV_FRONT_EXIST_SEN, DEF_ON);
		int nFrontSlowSensor = this->ChkDI(xC_TRAY_CV_FRONT_SLOW_SEN, DEF_ON);
		if(nFrontExistSensor != nFrontSlowSensor && bNoInterlock == eNoInterLock_OFF)
		{
			return ERR_STACKER_1_INTERLOCK_CAN_NOT_MAIN_PUSHER_UP + GetIndex();
		}
	}

	// MAIN PUSHER UP 동작을 수행 하려고 할 때 인터락 조건
	// Front에 Tray가 없어야 하고, Clamp는 Unlock 상태이어야 하고, Sub Pusher는 Down 상태이어야 한다.
	if (nUpOper == DEF_ON) {
		if (nFrontExistErr != ERR_NO_ERROR && nUnlockErr != ERR_NO_ERROR && nSubDownErr != ERR_NO_ERROR) {
			return ERR_STACKER_1_INTERLOCK_CAN_NOT_MAIN_PUSHER_UP + GetIndex();
		}
	}

	g_IO.out( m_vY[ yC_TRAY_MAIN_PUSHER_UP ], nUpOper );
	g_IO.out( m_vY[ yC_TRAY_MAIN_PUSHER_DOWN ], nDownOper );

	return ERR_NO_ERROR;
}

int CTaskStacker::chk_cyl_MainPusher( int nUpDn )
{
	int nUpOper = ( nUpDn == DEF_UP ) ? DEF_ON : DEF_OFF;
	int nDownOper = ( nUpDn == DEF_DOWN ) ? DEF_ON : DEF_OFF;

	int nErr = ERR_NO_ERROR;
	nErr |= ChkDI( xC_TRAY_MAIN_PUSHER_UP_SEN, nUpOper );
	nErr |= ChkDI( xC_TRAY_MAIN_PUSHER_DOWN_SEN, nDownOper );

	return nErr;
}

int CTaskStacker::get_cyl_MainPusher( int nUpDn )
{
	int nUpOper = ( nUpDn == DEF_UP ) ? DEF_ON : DEF_OFF;
	int nDownOper = ( nUpDn == DEF_DOWN ) ? DEF_ON : DEF_OFF;

	int nErr = ERR_NO_ERROR;
	nErr |= GetDI( xC_TRAY_MAIN_PUSHER_UP_SEN, nUpOper );
	nErr |= GetDI( xC_TRAY_MAIN_PUSHER_DOWN_SEN, nDownOper );

	return nErr;
}

int CTaskStacker::do_cyl_SubPusher( int nUpDn )
{
	int nUpOper = ( nUpDn == DEF_UP ) ? DEF_ON : DEF_OFF;
	int nDownOper = ( nUpDn == DEF_DOWN ) ? DEF_ON : DEF_OFF;

	int nFrontNotExistErr = this->GetDI(xC_TRAY_CV_FRONT_EXIST_SEN, DEF_OFF);
	int nUnlockErr = this->get_cyl_LockUnlock( DEF_UNLOCK );


	if( nFrontNotExistErr != ERR_NO_ERROR && nUnlockErr != ERR_NO_ERROR ) {
		if( nUpOper == DEF_ON ) {
			return ERR_STACKER_1_INTERLOCK_CAN_NOT_SUB_PUSHER_UP + GetIndex();
		}
	}

// 	if (g_TaskLoadTbl[nTableIdx].GetEqpCtrlMode() == eEqpCtrlMode_Manual) {
// 
// 	}

	g_IO.out( m_vY[ yC_TRAY_SUB_PUSHER_UP ], nUpOper );
	g_IO.out( m_vY[ yC_TRAY_SUB_PUSHER_DOWN ], nDownOper );

	return ERR_NO_ERROR;
}

int CTaskStacker::chk_cyl_SubPusher( int nUpDn )
{
	int nUpOper = ( nUpDn == DEF_UP ) ? DEF_ON : DEF_OFF;
	int nDownOper = ( nUpDn == DEF_DOWN ) ? DEF_ON : DEF_OFF;

	int nErr = ERR_NO_ERROR;
	nErr |= ChkDI( xC_TRAY_SUB_PUSHER_UP_SEN, nUpOper );
	nErr |= ChkDI( xC_TRAY_SUB_PUSHER_DOWN_SEN, nDownOper );

	return nErr;
}

int CTaskStacker::get_cyl_SubPusher( int nUpDn )
{
	int nUpOper = ( nUpDn == DEF_UP ) ? DEF_ON : DEF_OFF;
	int nDownOper = ( nUpDn == DEF_DOWN ) ? DEF_ON : DEF_OFF;

	int nErr = ERR_NO_ERROR;
	nErr |= GetDI( xC_TRAY_SUB_PUSHER_UP_SEN, nUpOper );
	nErr |= GetDI( xC_TRAY_SUB_PUSHER_DOWN_SEN, nDownOper );

	return nErr;
}

int CTaskStacker::do_cyl_TrayAlign( int nFwdBwd )
{
	int nForOper_For = (nFwdBwd == DEF_FORW) ? DEF_ON : DEF_OFF;
	int nForOper_Back = ( nFwdBwd == DEF_FORW ) ? DEF_OFF : DEF_ON;

	g_IO.out( m_vY[yCV_C_TRAY_ALIGN_FOR], nForOper_For);
	g_IO.out( m_vY[ yCV_C_TRAY_ALIGN_BACK ], nForOper_Back );

	return ERR_NO_ERROR;
}

int CTaskStacker::chk_cyl_TrayAlign( int nFwdBwd )
{
	int nForOper = ( nFwdBwd == DEF_FORW ) ? DEF_ON : DEF_OFF;
	int nBackOper = ( nFwdBwd == DEF_BACK ) ? DEF_ON : DEF_OFF;

	int nErr = ERR_NO_ERROR;
	nErr |= ChkDI( xC_TRAY_ALIGN_FOR_SEN, nForOper );
	nErr |= ChkDI( xC_TRAY_ALIGN_BACK_SEN, nBackOper );

	return nErr;
}

int CTaskStacker::get_cyl_TrayAlign( int nFwdBwd )
{
	int nForOper = ( nFwdBwd == DEF_FORW ) ? DEF_ON : DEF_OFF;
	int nBackOper = ( nFwdBwd == DEF_BACK ) ? DEF_ON : DEF_OFF;

	int nErr = ERR_NO_ERROR;
	nErr |= GetDI( xC_TRAY_ALIGN_FOR_SEN, nForOper );
	nErr |= GetDI( xC_TRAY_ALIGN_BACK_SEN, nBackOper );

	return nErr;
}

int CTaskStacker::do_cyl_Vibrator( int nFwdBwd )
{
	int nFwdOper = ( nFwdBwd == DEF_FORW ) ? DEF_ON : DEF_OFF;
	int nBwdOper = ( nFwdBwd == DEF_BACK ) ? DEF_ON : DEF_OFF;

	g_IO.out( m_vY[ yCV_VIBRATOR_FOR ], nFwdOper );
	g_IO.out( m_vY[ yCV_VIBRATOR_BACK ], nBwdOper );

	return ERR_NO_ERROR;
}

int CTaskStacker::ConveyorStop()
{
	g_IO.out( m_vY[ yCV_HIGH_SPEED_CONTROL ], DEF_OFF );
	Sleep( 50 );
	g_IO.out( m_vY[ yCV_FWD_CONTROL ], DEF_OFF );
	g_IO.out( m_vY[ yCV_REV_CONTROL ], DEF_OFF );
	Sleep( 50 );

	return ERR_NO_ERROR;
}

void CTaskStacker::ConveyorSlowStop()
{
	g_IO.out(m_vY[yCV_HIGH_SPEED_CONTROL], DEF_OFF);
	Sleep(200);
	g_IO.out(m_vY[yCV_FWD_CONTROL], DEF_OFF);
	g_IO.out(m_vY[yCV_REV_CONTROL], DEF_OFF);
	Sleep( 50 );
}

int CTaskStacker::ConveyorMoving( eConveyorMoveDir eMoveDir, bool bHighSpeed )
{
	
	int nErr = ChkConveyorDriveCVAlarm();

	if (nErr != ERR_NO_ERROR && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
	{
		return nErr;
	}

	int nMoveDirIndex = ( eMoveDir == eConveyorMove_FWD ) ? yCV_FWD_CONTROL : yCV_REV_CONTROL;

	g_IO.out( m_vY[ yCV_HIGH_SPEED_CONTROL ], ( bHighSpeed == true ) ? DEF_ON : DEF_OFF );
	//Sleep( 50 );
	g_IO.out( m_vY[ nMoveDirIndex ], DEF_ON );
	Sleep( 50 );
	

	return ERR_NO_ERROR;
}

int CTaskStacker::ChkConveyorDriveCVAlarm()
{
	//B접
	if (g_IO.in(m_vX[xCV_ALARM]) != TRUE)
	{
		return -1;
	}
	return ERR_NO_ERROR;
}
bool CTaskStacker::IsConveyorMoving()
{
	bool bFwd = g_IO.out( m_vY[ yCV_FWD_CONTROL ] ) == DEF_ON;
	bool bBwd = g_IO.out( m_vY[ yCV_REV_CONTROL ] ) == DEF_ON;
	if( bFwd == false && bBwd == false ) {
		return false;
	}

	return true;
}

int CTaskStacker::ChangeConveyorSpeed( bool bHighSpeed )
{
	g_IO.out( m_vY[ yCV_HIGH_SPEED_CONTROL ], ( bHighSpeed == true ) ? DEF_ON : DEF_OFF );

	return ERR_NO_ERROR;
}

bool CTaskStacker::ChkConveyorAlarm()
{
	// B접점
	int nChk = GetDI( xCV_ALARM, DEF_OFF ); //Alarm이므로 굳이 ChkDI로 하지말자..
	bool bRet = ( nChk == ERR_NO_ERROR ) ? true : false;

	return bRet;
}

bool CTaskStacker::GetFullTray()
{
	int nChk = GetDI( xC_TRAY_STACKER_FULL_SEN, DEF_OFF );//Full상태체크이므로 ChkDI로 하지말자..
	bool bRet = ( nChk == ERR_NO_ERROR ) ? true : false;

	return bRet;
}

bool CTaskStacker::ChkFullTray( BOOL bOnOff )
{
	int nChk = ChkDI( xC_TRAY_STACKER_FULL_SEN, bOnOff );//B 접
	bool bRet = ( nChk == ERR_NO_ERROR ) ? true : false;

	return bRet;
}

bool CTaskStacker::ChkEmptyTray()
{
	int nChk = ChkDI( xC_TRAY_STACKER_EXIST_SEN, DEF_ON );
	bool bRet = ( nChk == ERR_NO_ERROR ) ? false : true;

	return bRet;
}

bool CTaskStacker::GetEmptyTray()
{
	int nChk = GetDI( xC_TRAY_STACKER_EXIST_SEN, DEF_ON );
	bool bRet = ( nChk == ERR_NO_ERROR ) ? false : true;

	return bRet;
}

int CTaskStacker::GetConveyorTrayDeviceCnt()
{
	int device_cnt = g_DMCont.m_dmDevice.GetDeviceCount( eDeviceDataStage::eDeviceStage_STACKER_CONV_1 + m_eStackerIdx );

	return device_cnt;
}

int CTaskStacker::ChkExistTray_FrontPos_Sen( int nExist )
{
	int State = ( nExist == DEF_EXIST ) ? DEF_ON : DEF_OFF;

	int nErr = ERR_NO_ERROR;
	nErr |= ChkDI( xC_TRAY_CV_FRONT_SLOW_SEN, State );
	nErr |= ChkDI( xC_TRAY_CV_FRONT_EXIST_SEN, State );

	int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
	BOOL vat_in_process = g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_USE);
	if (nDeviceMode == eNoDeviceNoTray && vat_in_process != TRUE)
	{
		return ERR_NO_ERROR;
	}

	return nErr;
}

int CTaskStacker::ChkExistTray_RearPos_Sen( int nExist )
{
	int State = ( nExist == DEF_EXIST ) ? DEF_ON : DEF_OFF;

	int nErr = ERR_NO_ERROR;
	//if(m_eStackerIdx != eSTACKER_03)
	nErr |= ChkDI( xC_TRAY_CV_REAR_SLOW_SEN, State );
	nErr |= ChkDI( xC_TRAY_CV_REAR_EXIST_SEN, State );

	int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
	BOOL vat_in_process = g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_USE);
	if (nDeviceMode == eNoDeviceNoTray && vat_in_process != TRUE)
	{
		return ERR_NO_ERROR;
	}

	return nErr;
}

int CTaskStacker::GetExistTray_FrontPos_Sen( int nExist )
{
	int State = ( nExist == DEF_EXIST ) ? DEF_ON : DEF_OFF;

	int nErr = ERR_NO_ERROR;
	nErr |= GetDI( xC_TRAY_CV_FRONT_SLOW_SEN, State );
	nErr |= GetDI( xC_TRAY_CV_FRONT_EXIST_SEN, State );

	return nErr;
}

int CTaskStacker::GetExistTray_FrontPos_DvcMode( int nExist )
{
	int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN( NDM11_SYS_MD_DEVICE_CHK );
	int nErr = ERR_NO_ERROR;

	//if( nDeviceMode == eNoDeviceNoTray )
	//{
		int nExistTray = g_DMCont.m_dmDevice.GetTrayExist( eDeviceDataStage::eDeviceStage_STACKER_BTM_1 + m_eStackerIdx );
		if( nExistTray != nExist )
			return ERR_UNKNOWN;
	//}
	//else

	if (nDeviceMode != eNoDeviceNoTray)
	{
		nErr = GetExistTray_FrontPos_Sen(nExist);
		if (nErr != ERR_NO_ERROR)
			return nErr;
	}
		

	return nErr;
}


int CTaskStacker::GetExistTray_RearPos_Sen( int nExist )
{
	int State = ( nExist == DEF_EXIST ) ? DEF_ON : DEF_OFF;

	int nErr = ERR_NO_ERROR;
	//if (m_eStackerIdx != eSTACKER_03)
	nErr |= GetDI( xC_TRAY_CV_REAR_SLOW_SEN, State );

	nErr |= GetDI( xC_TRAY_CV_REAR_EXIST_SEN, State );

	return nErr;
}

int CTaskStacker::GetExistTray_RearPos_DvcMode( int nExist )
{
	int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN( NDM11_SYS_MD_DEVICE_CHK );
	int nErr = ERR_NO_ERROR;

// 	if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
// 		nErr = GetExistTray_RearPos_Sen(nExist);
// 		return nErr;			
// 	}

	if( nDeviceMode == eNoDeviceNoTray )
	{
		int nExistTray = g_DMCont.m_dmDevice.GetTrayExist( eDeviceDataStage::eDeviceStage_STACKER_CONV_1 + m_eStackerIdx );
		if (nExistTray != nExist)
			return ERR_UNKNOWN;
	}
	//else

	if (nDeviceMode != eNoDeviceNoTray)
	{
		nErr = GetExistTray_RearPos_Sen(nExist);
		if (nErr != ERR_NO_ERROR)
			return nErr;
	}
		


	return nErr;
}

eStacker_Idx CTaskStacker::GetIndex()
{
	return m_eStackerIdx;
}

eStacker_Type CTaskStacker::GetStackerType()
{
	return m_eStackerType;
}

