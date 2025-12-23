#include "stdafx.h"
#include "BbxmnpClient.h"
#include "ConfigData.h"
#include "BBXMNP\nmiMNApiDefs.h"
#include "TimeCheck.h"
#include <vector>
using namespace std;

#define		HOME_ESCAPE_PULSE		(100.0)		// pulse
#define		MOVE_CHECK_TIME			(15.0)		//second


#define		MAX_PICKER_CNT	MAX_PICKER_X_CNT*MAX_PICKER_Y_CNT
/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
CBbxmnpClient::CBbxmnpClient( void )
{
	memset( m_bOriginFlag, 0, sizeof( m_bOriginFlag ) );
	memset( m_adTargetPos, 0, sizeof( m_adTargetPos ) );
	memset( m_adLastPos, 0, sizeof( m_adLastPos ) );
	
	m_nBoardType = 0;

	for( int i = 0; i < PICK_MOTOR_MAX; i++ )
	{
		m_adScaleFactor[ i ] = 1;
		m_anMoveDir[ i ] = 1;
		m_anPulseDir[ i ] = 1;

		for( int j = 0; j < eBMAxis_Picker_MAX; j++ ) {
			m_SimStatus[ i * eBMAxis_Picker_MAX + j ].mem = 0;
			m_dActualPos[ i * eBMAxis_Picker_MAX + j ] = 0.0;
			m_All_Last_Pos[ i ][ j ] = 0.0;
		}
	}
	m_nSimul = 0;
	m_veBDSlaveNum.clear();

	m_veBDSlaveNum.push_back(LOAD_TRAY_PICKER_SLAVE_NUM1);
	m_veBDSlaveNum.push_back(LOAD_TRAY_PICKER_SLAVE_NUM2);
	m_veBDSlaveNum.push_back(TEST_PP1_LOAD_HAND_PICKER_SLAVE_NUM1);
	m_veBDSlaveNum.push_back(TEST_PP1_LOAD_HAND_PICKER_SLAVE_NUM2);
	m_veBDSlaveNum.push_back(TEST_PP1_UNLOAD_HAND_PICKER_SLAVE_NUM1);
	m_veBDSlaveNum.push_back(TEST_PP1_UNLOAD_HAND_PICKER_SLAVE_NUM2);
	m_veBDSlaveNum.push_back(UNLOAD_TRAY_PICKER_SLAVE_NUM1);
	m_veBDSlaveNum.push_back(UNLOAD_TRAY_PICKER_SLAVE_NUM2);
	m_veBDSlaveNum.push_back(TEST_PP2_LOAD_HAND_PICKER_SLAVE_NUM1);
	m_veBDSlaveNum.push_back(TEST_PP2_LOAD_HAND_PICKER_SLAVE_NUM2);
	m_veBDSlaveNum.push_back(TEST_PP2_UNLOAD_HAND_PICKER_SLAVE_NUM1);
	m_veBDSlaveNum.push_back(TEST_PP2_UNLOAD_HAND_PICKER_SLAVE_NUM2);

	vePickerDirIdx.clear();

	vePickerDirIdx.push_back(eDirPicker_Picker08);
	vePickerDirIdx.push_back(eDirPicker_Picker07);
	vePickerDirIdx.push_back(eDirPicker_Picker06);
	vePickerDirIdx.push_back(eDirPicker_Picker05);
	vePickerDirIdx.push_back(eDirPicker_Picker04);
	vePickerDirIdx.push_back(eDirPicker_Picker03);
	vePickerDirIdx.push_back(eDirPicker_Picker02);
	vePickerDirIdx.push_back(eDirPicker_Picker01);

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < eBMAxis_Picker_MAX; j++) {
			m_dHandGap[i][j] = 0.0;
		}
	}
	
}


/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
CBbxmnpClient::~CBbxmnpClient( void )
{
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
int CBbxmnpClient::Initialize( char* szFilePath )
{
	char szCurDir[ 100 ] = { 0 };
	char szFileName[ 256 ] = { 0 };
	char szDriverFileName[MAX_PATH] = { 0, };
	GetCurrentDirectory( 99, szCurDir );
	char szpath[ 256 ] = { 0, };
	sprintf_s( szpath, sizeof( szpath ), "%s\\sys_data", szCurDir );
	if( szpath[ strlen( szpath ) - 1 ] != '\\' )
	{
		sprintf_s( szFileName, sizeof( szFileName ) - 1, "%s\\SystemOption.ini", szpath );
	}
	else
	{
		sprintf_s( szFileName, sizeof( szFileName ) - 1, "%sSystemOption.ini", szpath );
	}
	char szBuff[ 100 ] = { 0 };
	GetPrivateProfileString( "SIMULATOR", "Motion", "0", szBuff, 99, szFileName );
	WritePrivateProfileString( "SIMULATOR", "Motion", szBuff, szFileName );
	m_nSimul = atoi( szBuff );

	int nRet = 0;
	nRet = m_bbxmnpWnd.Initialize( this, szFilePath, CBbxmnpClient::OnReceiveAlarm );

	if( nRet != 1 )
		return nRet;

	//Set Board Type
	sprintf_s(szDriverFileName, sizeof(szDriverFileName), "%s\\sys_data\\%s", szCurDir, SZ_DRIVER_FILE_NAME);
	CConfigData ConfigData(szDriverFileName);
	m_nBoardType = ConfigData.GetInt("SysOption", "Board_Direction", "0");

	LoadPickOffset();

	char szLogFileName[512] = { 0, };
	sprintf_s(szLogFileName, sizeof(szLogFileName), _T("%sMotor.log"), SZ_LOG_FILE_PATH);
	m_MtLog.SetFileName(szLogFileName);

	// SW Limit Setting
	//SetSwLimitPos(PICK_MOTOR_OUTPUT);

	for (int i = 0; i < PICK_MOTOR_MAX; i++)
	{
		SetSwLimitAction(_ePICK_MOTOR(i), emSOFT_SSTOP);		// 감속정지
		SetSwLimitEnable(_ePICK_MOTOR(i), emTRUE);				// 이벤트사용

		SetOrgLevel(_ePICK_MOTOR(i), 0);
		// Origin Level, Inposition Level
		// Origin Mode 설정 (1 : ORG ON -> Stop -> Go back(Rev Spd) -> ORG OFF -> Goforward(Rev Spd) -> ORG ON -> Stop(Default))
		SetOriginMode(_ePICK_MOTOR(i), 1);
	}


	return nRet;
}



void CBbxmnpClient::Finalize()
{
	m_bbxmnpWnd.Finialize();
}

//통신 재 연결
int CBbxmnpClient::ReConnection()
{
	int nRet = 0;
	nRet = m_bbxmnpWnd.ReConnection();

	if (nRet != 1)
		return nRet;

	for (int i = 0; i < PICK_MOTOR_MAX; i++)
	{
		SetSwLimitAction(_ePICK_MOTOR(i), emSOFT_SSTOP);		// 감속정지
		SetSwLimitEnable(_ePICK_MOTOR(i), emTRUE);				// 이벤트사용

		SetOrgLevel(_ePICK_MOTOR(i), 0);
		// Origin Level, Inposition Level
		// Origin Mode 설정 (1 : ORG ON -> Stop -> Go back(Rev Spd) -> ORG OFF -> Goforward(Rev Spd) -> ORG ON -> Stop(Default))
		SetOriginMode(_ePICK_MOTOR(i), 1);
	}

	return ERR_NO_ERROR;
}


void CBbxmnpClient::LoadPickOffset()
{
	CConfigData cConfigData("SYS_DATA\\PickerHeightCal.ini");
	char szSection[128];
	char szkey[128];
	for (int i = 0; i < PICK_MOTOR_MAX; i++)
	{
		for (int j = 0; j < eBMAxis_Picker_MAX; j++)
		{
			memset(szSection, 0x00, sizeof(szSection));
			memset(szkey, 0x00, sizeof(szkey));
			sprintf_s(szSection, sizeof(szSection), "Picker Offset Unit %d", i + 1);
			sprintf_s(szkey, sizeof(szkey), "%d", j + 1);
			m_stPickOffset[i].dFlatOffset[j] = cConfigData.GetDouble(szSection, szkey, "0.0");
		}
	}
 	//g_RAM_DATA.Load( m_stPickOffset, sizeof( PICK_OFFSET )*PICK_MOTOR_MAX, "sys_data\\St_PickOffset.dat" );
}

void CBbxmnpClient::SavePickOffset()
{
	CConfigData cConfigData("SYS_DATA\\PickerHeightCal.ini");
	char szSection[128];
	char szkey[128];
	for (int i = 0; i < PICK_MOTOR_MAX; i++)
	{
		for (int j = 0; j < eBMAxis_Picker_MAX; j++)
		{
			memset(szSection, 0x00, sizeof(szSection));
			memset(szkey, 0x00, sizeof(szkey));
			sprintf_s(szSection, sizeof(szSection), "Picker Offset Unit %d", i + 1);
			sprintf_s(szkey, sizeof(szkey), "%d", j + 1);
			cConfigData.Set(szSection, szkey, m_stPickOffset[i].dFlatOffset[j]);
		}
	}
	//g_RAM_DATA.Save( m_stPickOffset, sizeof( PICK_OFFSET )*PICK_MOTOR_MAX, "sys_data\\St_PickOffset.dat" );
}

void CBbxmnpClient::SetInitAxisInfo( _ePICK_MOTOR eMotor, double dScaleFactor, int nPulseDir /*= 1*/, int nMoveDir /*= 1*/, int nSubZAxisNum)
{
	m_adScaleFactor[ eMotor ] = dScaleFactor;
	m_anPulseDir[ eMotor ] = nPulseDir;
	m_anMoveDir[ eMotor ] = nMoveDir;
}

void CBbxmnpClient::SetInitParameter( _ePICK_MOTOR eMotor )
{
// 	if(PICK_MOTOR_LOAD_TRAY == eMotor )
// 	{	
// 		m_stMotorParam[ eMotor ].dSpeed = g_DMCont.m_dmTrayPP.GN(NDM5_SPD_TrayPP_1_Z);
// 		m_stMotorParam[ eMotor ].dAccTime = g_DMCont.m_dmTrayPP.GN(NDM5_ACC_TrayPP_1_Z);
// 		m_stMotorParam[ eMotor ].dDecTime = g_DMCont.m_dmTrayPP.GN(NDM5_DCC_TrayPP_1_Z);
// 	}
// 	else if(PICK_MOTOR_UNLOAD_TRAY == eMotor )
// 	{
// 		m_stMotorParam[ eMotor ].dSpeed = g_DMCont.m_dmTrayPP.GN(NDM5_SPD_TrayPP_2_Z);
// 		m_stMotorParam[ eMotor ].dAccTime = g_DMCont.m_dmTrayPP.GN(NDM5_ACC_TrayPP_2_Z);
// 		m_stMotorParam[eMotor].dDecTime = g_DMCont.m_dmTrayPP.GN(NDM5_DCC_TrayPP_2_Z);
// 	}
}

void CBbxmnpClient::SetHandMatrix()
{
	int nPickerAxisNumberStart = 0;
	m_vHandMatrix.clear();
	for (int i = 0; i < PICK_MOTOR_MAX; i++)
		m_vHandAxis[i].clear();

	int nCount = 0;
	int nSum = 0;

	for (int nPickerMotor = 0; nPickerMotor < PICK_MOTOR_MAX; nPickerMotor++)
	{
		for (int nPickerCnt = 0; nPickerCnt < eBMAxis_Picker_MAX; nPickerCnt++)
		{
			m_vHandMatrix.push_back(eBMAxis_LOAD_TRAY_Picker01 + nCount);
			m_vHandAxis[nPickerMotor].push_back(eBMAxis_LOAD_TRAY_Picker01+ nCount);
			nCount++;
		}
	}
}

void CBbxmnpClient::SetInitMotorParam(_ePICK_MOTOR eMotor, double dSpeed, double dAcc, double dDec,
	double dOrgSpeed, double dOrgAcc, double dSwLimitP, double dSwLimitN, double dScaleFactor)
{
	m_stMotorParam[ eMotor ].dSpeed = dSpeed;
	m_stMotorParam[ eMotor ].dAccTime = dAcc;
	m_stMotorParam[ eMotor ].dDecTime = dDec;
	m_stMotorParam[ eMotor ].dOrgSpeed = dOrgSpeed;
	m_stMotorParam[ eMotor ].dOrgRevSpeed = dOrgSpeed;
	m_stMotorParam[ eMotor ].dOrgAccDecTime = dOrgAcc;
	m_stMotorParam[ eMotor ].dSwLimitP = dSwLimitP;
	m_stMotorParam[ eMotor ].dSwLimitN = dSwLimitN;
	m_stMotorParam[eMotor].dScaleFactor = dScaleFactor;

	for (int nIdx : m_vHandAxis[eMotor]) {
		m_bbxmnpWnd.AxSetMaxVel(_eBbxmnpAxis(m_vHandMatrix[nIdx]), DEF_ZAXIS_MAX_SPEED);
	}	

	SetSwLimitPos( eMotor );
}

void CBbxmnpClient::SetPickOffset( _ePICK_MOTOR eMotor, PICK_OFFSET stPickOffset )
{
	m_stPickOffset[ eMotor ] = stPickOffset;
}

PICK_OFFSET CBbxmnpClient::GetPickOffset( _ePICK_MOTOR eMotor )
{
	ASSERT( eMotor < PICK_MOTOR_MAX );

	return m_stPickOffset[ eMotor ];
}

void CBbxmnpClient::SetOriginMode( _ePICK_MOTOR eMotor, int nMode )
{
	for (int nIdx : m_vHandAxis[eMotor]) {
		m_bbxmnpWnd.AxHomeSetType(_eBbxmnpAxis(m_vHandMatrix[nIdx]), nMode);
	}
}

int CBbxmnpClient::SetSwLimitPos( _ePICK_MOTOR eMotor )
{
	int nRet = 0, nTemp = 0;
	double dSwLimitP = 0, dSwLimitN = 0, dScaleFactor = 0;

	dScaleFactor = m_adScaleFactor[ eMotor ];
	dSwLimitP = m_stMotorParam[ eMotor ].dSwLimitP / dScaleFactor;
	dSwLimitN = m_stMotorParam[ eMotor ].dSwLimitN / dScaleFactor;

	for (int nIdx : m_vHandAxis[eMotor])
	{
		nTemp = m_bbxmnpWnd.AxSetSoftLimitPos( _eBbxmnpAxis(m_vHandMatrix[nIdx]), dSwLimitP, dSwLimitN );
		if( nTemp != TMC_RV_OK )
		{
			nRet = nTemp;
		}
	}

	return 0;
}

int CBbxmnpClient::SetSwLimitAction( _ePICK_MOTOR eMotor, int nAction )
{
	int nRet = 0, nTemp = 0;

	for (int nIdx : m_vHandAxis[eMotor]) {
		nTemp = m_bbxmnpWnd.AxSetSoftLimitAction( _eBbxmnpAxis(m_vHandMatrix[nIdx]), nAction );
		if( nTemp != TMC_RV_OK ) {
			nRet = nTemp;
		}
	}

 	return 0;
}

int CBbxmnpClient::SetSwLimitEnable( _ePICK_MOTOR eMotor, int nEnable )
{
	int nRet = 0, nTemp = 0;

	for (int nIdx : m_vHandAxis[eMotor]) {
		nTemp = m_bbxmnpWnd.AxSetSoftLimitEnable(_eBbxmnpAxis(m_vHandMatrix[nIdx]), nEnable );
		if( nTemp != TMC_RV_OK ) {
			nRet = nTemp;
		}
	}

	return 0;
}

BOOL CBbxmnpClient::GetMotorStatus( _eBbxmnpAxis eAxis, _eMOTOR_STS_TYPE eStatusType )
{
	BOOL bRetValue = FALSE;
	udtCardStatus status;

	m_bbxmnpWnd.AxGetMechanical( eAxis, &status.mem );
	if( m_nSimul == 1 ) {
		status = m_SimStatus[ ( int )eAxis ];
	}

	switch( eStatusType )
	{
		case eMotorSts_Org:
			bRetValue = status.mem_bit.ORG;
			break;
		case eMotorSts_EZ:
			bRetValue = status.mem_bit.EZ;
			break;
		case eMotorSts_EMG:
			bRetValue = status.mem_bit.EMG;
			break;
		case eMotorSts_INP:
			bRetValue = status.mem_bit.INP;
			break;
		case eMotorSts_ALM:
			bRetValue = status.mem_bit.ALM;
			break;
		case eMotorSts_LimitP:
			bRetValue = status.mem_bit.P_LMT;
			break;
		case eMotorSts_LimitN:
			bRetValue = status.mem_bit.N_LMT;
			break;
		case eMotorSts_CRC:
			bRetValue = status.mem_bit.CRC;
			break;
		case eMotorSts_ServoOn:
			bRetValue = status.mem_bit.SON;
			break;
		case eMotorSts_Reset:
			bRetValue = status.mem_bit.RST;
			break;
		case eMotorSts_HomeOK:
			bRetValue = status.mem_bit.H_OK;
			break;
	}

	return bRetValue;
}
/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
BOOL CBbxmnpClient::GetMotorStatus( _ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis, _eMOTOR_STS_TYPE eStatusType )
{
	int bRet = TRUE;

	bRet = GetMotorStatus( eAxis, eStatusType );

 	return bRet;
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
void CBbxmnpClient::GetMotorStatus( _ePICK_MOTOR eMotor, _eMOTOR_STS_TYPE eStatusType, BOOL* pStatus )
{
	int nArrIdx = 0;
	for (int nIdx : m_vHandAxis[eMotor])
	{
		pStatus[nArrIdx] = GetMotorStatus( _eBbxmnpAxis(m_vHandMatrix[nIdx]), eStatusType );
		nArrIdx++;
	}
}

BOOL CBbxmnpClient::GetMotionStatus( _eBbxmnpAxis eAxis, _eMOTION_STS_TYPE eType )
{
	BOOL bRetValue = FALSE;
	uMotionStatus status;

	m_bbxmnpWnd.AxGetMotion( eAxis, &status.mem );
	//if (m_nSimul == 1) {
	//	status = m_SimStatus[(int)eAxis];
	//}

	switch( eType )
	{
		case eMotionSts_DRV:
			bRetValue = status.mem_bit.DRV_STOP;
			break;

		case eMotionSts_WAIT_DR:
			bRetValue = status.mem_bit.WAIT_DR;
			break;

		case eMotionSts_WAIT_STA:
			bRetValue = status.mem_bit.WAIT_STA;
			break;

		case eMotionSts_WAIT_INSYNC:
			bRetValue = status.mem_bit.WAIT_INSYNC;
			break;

		case eMotionSts_WAIT_OTHER:
			bRetValue = status.mem_bit.WAIT_OTHER;
			break;

		case eMotionSts_WAIT_ERC:
			bRetValue = status.mem_bit.WAIT_ERC;
			break;

		case eMotionSts_WAIT_DIR:
			bRetValue = status.mem_bit.WAIT_DIR;
			break;

		case eMotionSts_BACKLASH:
			bRetValue = status.mem_bit.BACKLASH;
			break;

		case eMotionSts_WAIT_PE:
			bRetValue = status.mem_bit.WAIT_PE;
			break;

		case eMotionSts_IN_FA:
			bRetValue = status.mem_bit.IN_FA;
			break;

		case eMotionSts_IN_FL:
			bRetValue = status.mem_bit.IN_FL;
			break;

		case eMotionSts_IN_FUR:
			bRetValue = status.mem_bit.IN_FUR;
			break;

		case eMotionSts_IN_FH:
			bRetValue = status.mem_bit.IN_FH;
			break;

		case eMotionSts_IN_FDR:
			bRetValue = status.mem_bit.IN_FDR;
			break;

		case eMotionSts_WAIT_INP:
			bRetValue = status.mem_bit.WAIT_INP;
			break;

		case eMotionSts_IN_CMP:
			bRetValue = status.mem_bit.IN_CMP;
			break;

		case eMotionSts_WAIT_SYNC:
			bRetValue = status.mem_bit.WAIT_SYNC;
			break;

		case eMotionSts_WAIT_GANT:
			bRetValue = status.mem_bit.WAIT_GANT;
			break;
	}

	return bRetValue;
}

void CBbxmnpClient::GetMotionStatus( _ePICK_MOTOR eMotor, _eMOTION_STS_TYPE eType, BOOL * pStatus )
{
	int nArrIdx = 0;
	for (int nIdx : m_vHandAxis[eMotor])
	{
		pStatus[nArrIdx] = GetMotionStatus( _eBbxmnpAxis(m_vHandMatrix[nIdx]), eType );
		nArrIdx++;
	}
}

int CBbxmnpClient::ServoOn( _eBbxmnpAxis eAxis, bool bEnable )
{
	int nRetValue = 0;
	bool bLockFalg = false;

	int nState = ( true == bEnable ) ? TMC_BIT_ON : TMC_BIT_OFF;
	nRetValue = m_bbxmnpWnd.AxSetServoOn( eAxis, nState );
	m_SimStatus[ ( int )eAxis ].mem_bit.SON = nState;


	return nRetValue;
}

int CBbxmnpClient::ServoOn( _ePICK_MOTOR eMotor, bool bEnable )
{
	int nRetValue = 0;
	bool bLockFalg = false;

	// Trans array

	int nState = ( true == bEnable ) ? TMC_BIT_ON : TMC_BIT_OFF;
	for (int nIdx : m_vHandAxis[eMotor]) 
	{
		m_SimStatus[ ( int )m_vHandMatrix[nIdx]].mem_bit.SON = nState;
		nRetValue = m_bbxmnpWnd.AxSetServoOn(_eBbxmnpAxis(m_vHandMatrix[nIdx]), nState );
	}

	return 0;
}

int CBbxmnpClient::Origin( _ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis )
{
	//if( GetOriginFlag( eMotor, eAxis ) == true ) return 1;
	SetOriginFlag(eMotor, eAxis, false);
	// Origin Direction Setting
	m_bbxmnpWnd.AxHomeSetDir(eAxis, emDIR_N);
	// 원점 탈출 펄스량 (이게 설정이 필요한지 모르겠음, 필요하다면 적정량은??)
	m_bbxmnpWnd.AxHomeSetEscapePul( eAxis, HOME_ESCAPE_PULSE );
	// 원점센서가 감지되어 있을 때 이송할 속도
	double dInitSpeed = double( m_stMotorParam[ eMotor ].dOrgSpeed ) / m_stMotorParam[eMotor].dScaleFactor;
	m_bbxmnpWnd.AxHomeSetInitVel( eAxis, dInitSpeed );

	m_bbxmnpWnd.AxHomeSetType(eAxis, 1);


	double dVel = 0, dRevVel = 0, dAccDecTime = 0;
	dVel = m_stMotorParam[ eMotor ].dOrgSpeed;
	dRevVel = m_stMotorParam[ eMotor ].dOrgRevSpeed;
	dAccDecTime = m_stMotorParam[ eMotor ].dOrgAccDecTime;
	// 속도 프로파일 설정
	m_bbxmnpWnd.AxHomeSetVelProf( eAxis, emPROF_S, dVel, dRevVel, dAccDecTime );

	if( m_nSimul == 1 ) {
		return 1;
	}

	// Homming start
	int nRetValue = m_bbxmnpWnd.AxHomeMove( eAxis );
	if( nRetValue != TMC_RV_OK )
	{
		SetOriginFlag( eMotor, eAxis, false );

		return nRetValue;
	}
	SetOriginFlag(eMotor, eAxis, true);
	return TMC_RV_OK;
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
BOOL CBbxmnpClient::OriginWhile( _ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis, BOOL bMsgDisplay /*= FALSE*/ )
{
	bool bLockFlag = false;
	// SW Limit의 Event를 Off한다.
	m_bbxmnpWnd.AxSetSoftLimitEnable( eAxis, emFALSE );

	// Origin Flag setting
	SetOriginFlag( eMotor, eAxis, false );

	m_swTimeout.SetCheckTime(TIME_OUT_SHORT);
	m_swTimeout.StartTimer();

	int nRet = Origin( eMotor, eAxis );
	if( nRet != TMC_RV_OK )
	{
		// SW Limit의 Event를 On한다.
		//m_bbxmnpWnd.AxSetSoftLimitEnable( eAxis, emTRUE );
		return -1;
	}
	else
	{
		// Home Check Done
		int nHomeDone = 0;
		do {
			m_bbxmnpWnd.AxHomeCheckDone( eAxis, &nHomeDone );
			NEXT;
			if (m_swTimeout.CheckOverTime() == TRUE) {
			
				break;
			}
		} while( nHomeDone == emRUNNING );

		if( GetMotorStatus( eAxis, eMotorSts_HomeOK ) == TRUE )
		{
			// Origin Flag setting
			SetOriginFlag( eMotor, eAxis, true );
		}
		else
		{
			// SW Limit의 Event를 On한다.
			//m_bbxmnpWnd.AxSetSoftLimitEnable( eAxis, emTRUE );
			return -1;
		}
	}

	// SW Limit의 Event를 On한다.
	//m_bbxmnpWnd.AxSetSoftLimitEnable( eAxis, emTRUE );

	return ERR_NO_ERROR;
}

int CBbxmnpClient::MultiOrigin( _ePICK_MOTOR eMotor )
{
	//if (GetOriginFlag(eMotor) == true) return TMC_RV_OK;
	{
		int nIdx = 0;
		// Input & Output Picker 수는 동일하므로..
		int nAxes[ePICKER_MAX][eMULTI_OPER_MAX] = { 0 };
		int nRetValue = 0;
		int nPickerStart = ZAxisStartPicker(eMotor);

		// Setting
		for (int nPicPos = 0; nPicPos < ePICKER_MAX; nPicPos++)
		{
			for (int i = nPickerStart; i < (nPickerStart + eMULTI_OPER_MAX); i++)
			{
				m_bbxmnpWnd.AxSetSoftLimitEnable(_eBbxmnpAxis(i), emFALSE);
				// Origin Direction Setting
				m_bbxmnpWnd.AxHomeSetDir(_eBbxmnpAxis(i), emDIR_N);
				// 원점 탈출 펄스량 (이게 설정이 필요한지 모르겠음, 필요하다면 적정량은??)
				m_bbxmnpWnd.AxHomeSetEscapePul(_eBbxmnpAxis(i), HOME_ESCAPE_PULSE);
				// 원점센서가 감지되어 있을 때 이송할 속도
				double dInitSpeed = double(m_stMotorParam[eMotor].dOrgSpeed) / m_stMotorParam[eMotor].dScaleFactor;
				m_bbxmnpWnd.AxHomeSetInitVel(_eBbxmnpAxis(i), dInitSpeed);

				m_bbxmnpWnd.AxHomeSetType(_eBbxmnpAxis(i), 1);

				double dVel = 0, dRevVel = 0, dAccDecTime = 0;
				dVel = m_stMotorParam[eMotor].dOrgSpeed;
				dRevVel = m_stMotorParam[eMotor].dOrgRevSpeed;
				dAccDecTime = m_stMotorParam[eMotor].dOrgAccDecTime;
				// 속도 프로파일 설정
				m_bbxmnpWnd.AxHomeSetVelProf(_eBbxmnpAxis(i), emPROF_S, dVel, dRevVel, dAccDecTime);

				nAxes[nPicPos][nIdx] = i;
				nIdx++;
			}
			// Multi Homming start
			nRetValue = m_bbxmnpWnd.MxHomeMove(nIdx, nAxes[nPicPos]);
			Sleep(50);
			if (nRetValue != TMC_RV_OK && m_nSimul == 0)
			{
				SetOriginFlag(eMotor, false);
				//SetSwLimitEnable(eMotor, emTRUE);
				return nRetValue;
			}

			nIdx = 0;
			nPickerStart = nPickerStart + eMULTI_OPER_MAX;
		}
	}

	return TMC_RV_OK;
}

BOOL CBbxmnpClient::MultiOriginWhile( _ePICK_MOTOR eMotor/*= FALSE*/ )
{
	bool bLockFlag = false;
	CTimerCheck tmOrgChkTimeOver( 5.0, FALSE );

	// SW Limit의 Event를 Off한다.
	SetSwLimitEnable( eMotor, FALSE );

	// Init Origin Flag
	SetOriginFlag( eMotor, false );

	int nRetValue = MultiOrigin( eMotor );
	if( nRetValue != TMC_RV_OK )
	{
		//SetSwLimitEnable( eMotor, TRUE );
		return -1;
	}
	else
	{
		// Trans array
		int nArrIdx = 0;
		int nOperAxesCnt[ 2 ] = { 4, 4 };

		// Home Check Done
		int nHomeDone = 0, nCheckCnt = 0;
		tmOrgChkTimeOver.StartTimer();
		do {
			nCheckCnt = 0;

			if( tmOrgChkTimeOver.CheckOverTime() == TRUE )
			{
				for (int nIdx : m_vHandAxis[eMotor])
				{
					m_bbxmnpWnd.AxStop(_eBbxmnpAxis(m_vHandMatrix[nIdx]));
				}
			}

			for (int nIdx : m_vHandAxis[eMotor])
			{
				m_bbxmnpWnd.AxHomeCheckDone(_eBbxmnpAxis(m_vHandMatrix[nIdx]), &nHomeDone );
				if( m_nSimul == 1 ) {
					m_SimStatus[nIdx].mem_bit.H_OK = 1;
					nHomeDone = emSTAND;
				}

				//				m_bbxmnpWnd.
				// 동작이 완료되었는지 확인
				if( nHomeDone == emSTAND )
				{
					nCheckCnt++;
				}
			}

			NEXT;
		} while( nCheckCnt < eBMAxis_Picker_MAX );// Output이랑 Input이랑 picker 개수가 달라지지는 않을 것임

												 // Check status (Home OK)
		nCheckCnt = 0;
		for (int nIdx : m_vHandAxis[eMotor])
		{
			if( GetMotorStatus(_eBbxmnpAxis(m_vHandMatrix[nIdx]), eMotorSts_HomeOK ) == TRUE )
			{
				nCheckCnt++;
			}
		}

		if( nCheckCnt == eBMAxis_Picker_MAX )
		{
			SetOriginFlag( eMotor, true );

			double dSafetyPos = 0;

			dSafetyPos = 0.0;
			//MoveWhile(eMoveType_ABS, eMotor, dSafetyPos, 100, 0, 0, TRUE);			
		}
		else
		{
			// SW Limit의 Event를 On한다.
			//SetSwLimitEnable( eMotor, TRUE );

			return -1;
		}
	}

	// SW Limit의 Event를 On한다.
	SetOriginFlag(eMotor, true);
	//SetSwLimitEnable( eMotor, TRUE );

	return ERR_NO_ERROR;
}

/*=============================================================================
DESCRIPT : 절대위치 및 상대위치 이동을 하는 함수
KEY WORD :
ARGUMENT : eMoveType => 절대위치 or 상대위치인지 옵션, eAxis => 움직일 축, dMovePos => 이동거리(um), dSpeed => 속도(pps), dAccTime => 가속시간(msec), dDecTime => 감속시간(msec)
RETURN   : 0 => 정상, 그외 => Error
==============================================================================*/
int CBbxmnpClient::Move( _eMoveType eMoveType, _eBbxmnpAxis eAxis, double dMovePos, double dSpeed /*= 0*/, double dAccTime /*= 0*/, double dDecTime /*= 0*/, int nBoardType, BOOL bNoUseOffset/* = FALSE*/ )
{
	int nRetValue = 0;
	int nRobotNo = 0;
	bool bLockFlag = false;
	double dTargetPos = 0;
	CString strAxisName = "", strSecMsg = "";
	_ePICK_MOTOR eMotor;
	AxisTransToName( eAxis, strAxisName );
	AxisTransToPickMotor( eAxis, eMotor );

	// Check Origin Flag
	if( GetOriginFlag( eMotor, eAxis ) == false )
	{
		
	}

	if( bNoUseOffset == FALSE )
	{
		double dFlatOffset = ApplyZMotorFlatOffset( eAxis );

		if( dMovePos > 0.0 )
		{
			dMovePos += dFlatOffset;
		}
	}

	// Check SW Limit
// 	double dSwLimitP = m_stMotorParam[ eMotor ].dSwLimitP;
// 	double dSwLimitN = m_stMotorParam[ eMotor ].dSwLimitN;
// 	if (dMovePos > dSwLimitP || dMovePos < dSwLimitN)
// 	{
// 		CString strText;
// 		strText.Format("[%s // Target : %10.1f // Positive Pos : %10.1f // Negative Pos : %10.1f]",
// 			strAxisName, dMovePos / 1000.0, dSwLimitP / 1000.0, dSwLimitN / 1000.0);
// 	}

	m_All_Last_Pos[ eMotor ][ eAxis ] = dMovePos;
	dTargetPos = dMovePos;
	// 초기속도 (Zero)
	m_bbxmnpWnd.AxSetInitVel( eAxis, DEF_ZAXIS_INIT_SPEED);
	// Setting Velocity profile
	m_bbxmnpWnd.AxSetVelProf( eAxis, emPROF_T, dSpeed, dAccTime, dDecTime );

	nRetValue = m_bbxmnpWnd.AxPosMove( eAxis, eMoveType, dTargetPos );
	m_dActualPos[ ( int )eAxis ] = dTargetPos;

	Sleep(50);
	if( nRetValue == TMC_RV_OK ) {
		nRetValue = 0;
	}

	return nRetValue;
}

BOOL CBbxmnpClient::MoveWhile( _eMoveType eMoveType, _eBbxmnpAxis eAxis, double dMovePos, double dSpeed /*= 0*/, double dAccTime /*= 0*/, double dDecTime /*= 0*/, BOOL bNoUseOffset/* = FALSE*/ )
{
	bool bLockFlag = false;
	CString strAxisName = "", strSecMsg = "";
	AxisTransToName( eAxis, strAxisName );

	int nRet = Move( eMoveType, eAxis, dMovePos, dSpeed, dAccTime, dDecTime, bNoUseOffset );
	if( nRet != 0/*TMC_RV_OK*/ )
	{	
		return FALSE;		
	}

	// Check Time over
	CTimerCheck timeChecker( 15 );
	timeChecker.StartTimer();
	while( CheckMoveEnd( eAxis ) != 1 /*TMC_RV_OK*/ )
	{
		// TIMEOVER !!!
		if( timeChecker.CheckOverTime() )
		{
			Stop( eAxis );

			CStringList strList;
			strSecMsg.Format( "%s // (Move)", strAxisName );


			return FALSE;
		}
		NEXT;
	}

	return TRUE;
}

/*=============================================================================
DESCRIPT : 절대위치 및 상대위치 이동을 하는 함수
KEY WORD :
ARGUMENT : eMoveType=절대위치 or 상대위치인지 옵션, nNumOfAxes=움직일 축의 개수, anAxes=움직일 축의 배열,
		   dMovePos=이동거리(um), dSpeed=속도(pps), dAccTime=가속시간(msec), dDecTime => 감속시간(msec)
RETURN   : 0 => 정상, 그외 => Error
==============================================================================*/
int CBbxmnpClient::Move( _eMoveType eMoveType, _ePICK_MOTOR eMotor, std::vector<CPoint> vPicker, double dMovePos, double dSpeed /*= 0*/, double dAccTime /*= 0*/, double dDecTime /*= 0*/,int nBoardType, BOOL bNoUseOffset/* = FALSE*/ /*BOOL bVatOffsetUse= FALSE*/)
{
	//ASSERT( anAxes != nullptr );
	ASSERT(0 != vPicker.size());

	int nRetValue = 0;
	int nRobotNo = 0;
	bool bLockFlag = false;
	double dTargetPos = 0;
	CString strAxisName = "", strSecMsg = "";

	int nOperAxes[ePICKER_MAX][MAX_PICKER_CNT_EACH_SLAVE] = { 0, };
	int nHandPickerNum[ePICKER_MAX][MAX_PICKER_CNT_EACH_SLAVE] = {0,  };
	int nOperAxesCnt[ ePICKER_MAX ] = { 0, };
	memset( nOperAxes, -1, sizeof( nOperAxes ) );

	int nHandNum = 0;

	for (int i = 0; i < MAX_PICKER_Y_CNT; i++)
	{
		for (int j = 0; j < MAX_PICKER_X_CNT; j++)
		{
			nHandPickerNum[ i ][ j ] = nHandNum;
			nHandNum++;
		}
	}

	// 실제 구동할 Picker의 위치를 Setting

	SetOperAxes( eMotor, vPicker, nOperAxes, nOperAxesCnt );

	_eBbxmnpAxis eOperMotor = ( _eBbxmnpAxis( nOperAxes[ 0 ][ 0 ] ) >= 0 ) ? _eBbxmnpAxis( nOperAxes[ 0 ][ 0 ] ) : _eBbxmnpAxis( nOperAxes[ 1 ][ 0 ] );

	AxisTransToName(eMotor, strAxisName );

	double dScale = m_adScaleFactor[ eMotor ];
	int		nMoveDir = m_anMoveDir[eMotor];

	ASSERT( dScale != 0 );

	int nFlatOffsetPos = 0;
	int nStandardPos = 0;

	SetMaxVel(_ePICK_MOTOR(eMotor));

	CString strFlatnessOffset = _T("");

	// Velocity Setting
	for( int nPicPos = 0; nPicPos < ePICKER_MAX; nPicPos++ ) 
	{
// 		double* adTargetPos = new double[ nOperAxesCnt[ nPicPos ] ];
// 		memset( adTargetPos, 0, sizeof( double )*nOperAxesCnt[ nPicPos ] );
		double adTargetPos[MAX_PICKER_CNT_EACH_SLAVE] = {0,};
		//for( int nPicNum = 0; nPicNum < nOperAxesCnt[ nPicPos ]; nPicNum++ ) {
		for (int nPicNum = 0; nPicNum < MAX_PICKER_CNT_EACH_SLAVE; nPicNum++) {
			if (nOperAxes[nPicPos][nPicNum] >= 0) 
			{
				double dFlatOffset = 0.0;
				// 초기속도 (Zero)
				m_bbxmnpWnd.AxSetInitVel(_eBbxmnpAxis(nOperAxes[nPicPos][nPicNum]), DEF_ZAXIS_INIT_SPEED);
				// Velocity profile
				m_bbxmnpWnd.AxSetVelProf(_eBbxmnpAxis(nOperAxes[nPicPos][nPicNum]), emPROF_T, dSpeed, dAccTime, dDecTime);

				// Height Cal 보정값 적용
				//BOOL bVATUse = g_DMCont.m_dmVAT.GB(BDM14_VAT_MD_USE);			
								
				CString strHandGap = _T("");

				if (bNoUseOffset == FALSE)
				{
// 					nStandardPos = ZAxisStartPicker(eMotor);
// 					nFlatOffsetPos = nOperAxes[nPicPos][nPicNum] - nStandardPos;
// 
// 					dFlatOffset = nMoveDir * (m_dHandGap[/*nOperAxes[nPicPos][nPicNum]*/nFlatOffsetPos] / dScale);
					int nHand = -1;
					AxisTransToVatHand(eMotor, nHand);

					int nPickerIdx = nPicNum + (4 * nPicPos);
					dFlatOffset = nMoveDir * (m_dHandGap[nHand][nPickerIdx] / dScale); //m_dHandGap BoardDirection에 맞게 저장 되어 있음.

					strHandGap.Format("%.3f, ", m_dHandGap[nHand][nPickerIdx]);
				}
				
			
				strFlatnessOffset += strHandGap;


				double dRealOffset = 0.0;
				if (dMovePos > 0.0)
				{
					dRealOffset = dFlatOffset;
				}

				if (nOperAxes[nPicPos][nPicNum] >= 0)
				{
					SetLastPos(eMotor, nHandPickerNum[nPicPos][nPicNum], dMovePos);

					adTargetPos[nPicNum] = dMovePos;
					adTargetPos[nPicNum] += dRealOffset;

					int nIndex = nOperAxes[nPicPos][nPicNum];
					m_dActualPos[nIndex] = dTargetPos;
				}
			}
		}



		//초기화 진행.
// 		for (int i = 0; i < eBMAxis_Picker_MAX; i++) {
// 			m_dHandGap[i] = 0.0;
// 		}

		if( nOperAxesCnt[ nPicPos ] > 0 ) {
			nRetValue = m_bbxmnpWnd.MxPosMove( nOperAxesCnt[ nPicPos ], eMoveType, nOperAxes[ nPicPos ], adTargetPos );
		}

		if( nRetValue == TMC_RV_OK ) {
			nRetValue = 0;
		}

// 		if(nOperAxesCnt[nPicPos] > 0)
// 			delete[] adTargetPos;
	}

	//Z축 Picker Motor
	CString strPicker = _T("");
	for (std::vector<CPoint>::size_type i = 0; i < vPicker.size(); i++) {
		strPicker += GetPickerName(vPicker[i]);
	}

	m_MtLog.LOG("[%s] [Offset Use : %d] [Target Position : %.3f] [%s]  ", strAxisName, !bNoUseOffset, (dMovePos*dScale), strPicker);
	m_MtLog.LOG("[%s]", strFlatnessOffset);

	if( m_nSimul == 1 ) {
		return 0;
	}

return 0;
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
BOOL CBbxmnpClient::MoveWhile( _eMoveType eMoveType, _ePICK_MOTOR eMotor, std::vector<CPoint> vPicker, double dMovePos, double dSpeed /*= 0*/, double dAccTime /*= 0*/, double dDecTime /*= 0*/, BOOL bNoUseOffset/* = FALSE*/ )
{
	ASSERT(vPicker.size() != 0 );

	bool bLockFlag = false;
	CString strAxisName = "", strSecMsg = "";

	int nOperAxes[ 2 ][ 4 ] = { 0, };
	int nOperAxesCnt[ PICK_MOTOR_MAX ] = { 0, };
	memset( nOperAxes, -1, sizeof( nOperAxes ) );

	// 실제 구동할 Picker의 위치를 Setting
	SetOperAxes( eMotor, vPicker, nOperAxes, nOperAxesCnt );

	_eBbxmnpAxis eOperMotor = ( _eBbxmnpAxis( nOperAxes[ 0 ][ 0 ] ) >= 0 ) ? _eBbxmnpAxis( nOperAxes[ 0 ][ 0 ] ) : _eBbxmnpAxis( nOperAxes[ 1 ][ 0 ] );

	AxisTransToName( eOperMotor, strAxisName );


	int nRet = Move( eMoveType, eMotor, vPicker, dMovePos, dSpeed, dAccTime, dDecTime, bNoUseOffset );
	if( nRet != 0 /*TMC_RV_OK*/ )
	{
		
	}

	// 현재 Motor Type Check
	AxisTransToPickMotor( eOperMotor, eMotor );

	// Check Time over
	CTimerCheck timeChecker( 15 );
	timeChecker.StartTimer();
	for( int nPicPos = 0; nPicPos < ePICKER_MAX; nPicPos++ ) {
		while( CheckMoveEnd( nOperAxesCnt[ nPicPos ], nOperAxes[ nPicPos ] ) != TMC_RV_OK )
		{
			// TIMEOVER !!!
			if( timeChecker.CheckOverTime() )
			{
				Stop( nOperAxesCnt[ ePICKER_REAR ], nOperAxes[ ePICKER_REAR ] );
				Stop( nOperAxesCnt[ ePICKER_FRONT ], nOperAxes[ ePICKER_FRONT ] );

				CStringList strList;
				strSecMsg.Format( "%s // (Move)", strAxisName );

				return FALSE;
			}
			NEXT;
		}
	}

	return TRUE;
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
BOOL CBbxmnpClient::MoveWhile( _eMoveType eMoveType, _ePICK_MOTOR eMotor, double dMovePos, double dSpeed /*= 0*/, double dAccTime /*= 0*/, double dDecTime /*= 0*/, BOOL bNoUseOffset/* = FALSE*/ )
{
	int anAxes[ ePICKER_MAX ][ eBMAxis_Picker_MAX ] = { 0, };
	std::vector<CPoint> vPicker;
	for( int nPicPos = 0; nPicPos < ePICKER_MAX; nPicPos++ )
	{
		for( int nPicNum = 0; nPicNum < eMULTI_OPER_MAX; nPicNum++ ) {			
			vPicker.push_back(CPoint(nPicPos, nPicNum));
		}
	}

	return MoveWhile( eMoveType, eMotor, vPicker, dMovePos, dSpeed, dAccTime, dDecTime, bNoUseOffset );
}

BOOL CBbxmnpClient::MoveWhiletoLastPos( _eMoveType eMoveType, _ePICK_MOTOR eMotor, double dSpeed, double dAccTime, double dDecTime )
{
	// Home Check
	MultiOriginWhile( eMotor );
	// MoveWhile로 Last Pos 이동 
	for( int i = 0; i < eBMAxis_Picker_MAX; i++ ) {
		MoveWhile( eMoveType_ABS, eMotor, m_adLastPos[ eMotor ][ i ], 100, 0, 0, FALSE );
		m_MtLog.LOG("Last Position : Picker No : %d, Pos : %lf",i, m_adLastPos[eMotor][i]);
	}
	return 0;
}

int CBbxmnpClient::JogMove( _eJogDir eJogDir, _eBbxmnpAxis eAxis, double dSpeed, double dAccDecTime )
{
	int nRetValue = 0;
	bool bLockFlag = false;

	// Set Init speed
	m_bbxmnpWnd.AxSetInitJogVel( eAxis, 0 );
	// Set Velocity profile
	m_bbxmnpWnd.AxSetJogVelProf( eAxis, emPROF_S, dSpeed, dAccDecTime );

	// JOG MOVE
	nRetValue = m_bbxmnpWnd.AxJogMove( eAxis, eJogDir );

	return nRetValue;
}

int CBbxmnpClient::JogMove( _eJogDir eJogDir, int nNumOfAxes, int* anAxes, double dSpeed, double dAccDecTime)
{
	int nRetValue = 0;
	bool bLockFlag = false;

	int* anDir = new int[ nNumOfAxes ];
	memset( anDir, 0, sizeof( int )*nNumOfAxes );


	// Setting Velocity
	for( int i = 0; i < nNumOfAxes; i++ )
	{
		// Init speed
		m_bbxmnpWnd.AxSetInitJogVel( _eBbxmnpAxis( anAxes[ i ] ), 0 );
		// Velocity profile
		m_bbxmnpWnd.AxSetJogVelProf( _eBbxmnpAxis( anAxes[ i ] ), emPROF_S, dSpeed, dAccDecTime );

		anDir[ i ] = eJogDir;
	}
	
	// JOG Move
	nRetValue = m_bbxmnpWnd.MxJogMove( nNumOfAxes, anAxes, anDir );

	delete[] anDir;
	return nRetValue;
}

double CBbxmnpClient::GetCommandPos( _ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis )
{
	bool bLockFlag = false;
	double dPos = 0;


	int nRetValue = m_bbxmnpWnd.AxGetCmdPos( eAxis, &dPos );
	if( nRetValue != TMC_RV_OK )
	{
		// Alarm
	}


	double dRetPos = m_anPulseDir[ eMotor ] * dPos * m_adScaleFactor[ eMotor ];
	return dRetPos;
}

void CBbxmnpClient::SetCommandPos( _ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis, double dPos )
{
	bool bLockFlag = false;

	if( dPos != 0 )
	{
		dPos = m_anPulseDir[ eMotor ] * dPos / m_adScaleFactor[ eMotor ];
	}

	m_bbxmnpWnd.AxSetCmdPos( eAxis, dPos );
}

double CBbxmnpClient::GetActualPos( _ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis )
{
	bool bLockFlag = false;

	if( m_nSimul == 1 ) {
		double dRetPos = m_dActualPos[ ( int )eAxis ];

		return dRetPos;
	}

	double dPos = 0;
	m_bbxmnpWnd.AxGetActPos( eAxis, &dPos );


	double dRetPos = m_anPulseDir[ eMotor ] * dPos * m_adScaleFactor[ eMotor ];
	return dRetPos;
}

void CBbxmnpClient::SetActiaulPos( _ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis, double dPos )
{
	bool bLockFlag = false;

	if( dPos != 0 )
	{
		dPos = m_anPulseDir[ eMotor ] * dPos / m_adScaleFactor[ eMotor ];
	}

	if( m_nSimul == 1 ) {
		m_dActualPos[ ( int )eAxis ] = dPos;
	}

	m_bbxmnpWnd.AxSetActPos( eAxis, dPos );

}

int CBbxmnpClient::CheckMoveEnd( _eBbxmnpAxis eAxis )
{
	int nRetValue = 0;
	bool bLockFlag = false;

	if( m_nSimul == 1 ) {
		return TMC_RV_OK;
	}

	int nDone = 0;
	int nRet = m_bbxmnpWnd.AxCheckDone( eAxis, &nDone );
	if( nRet == TMC_RV_OK )
	{
		if( nDone == emSTAND )
		{
			nRetValue = TMC_RV_OK;
		}
		else//RUNNING
		{
			nRetValue = 0;	// 이송중..
		}
	}
	else
	{
		nRetValue = nRet;
	}

	return nRetValue;
}

/*=============================================================================
DESCRIPT : IN/OUTPUT 전체에 대하여 이송완료를 확인
KEY WORD :
ARGUMENT :
RETURN   : nRetValue => 1 : 이송완료, 0 : 이송중, 그외의 값 : 에러
==============================================================================*/
int CBbxmnpClient::CheckMoveEnd( _ePICK_MOTOR eMotor )
{
	int nRet = 0;
	int nRetValue = 0;
	bool bLockFlag = false;
	int nAxes[ ePICKER_MAX ][ eBMAxis_Picker_MAX ] = { { 0, },{ 0, } };
	int nDones[ ePICKER_MAX ];
	int nPickerStart = ( eMotor == PICK_MOTOR_LOAD_TRAY ) ? eBMAxis_LOAD_TRAY_Picker01 : eBMAxis_UNLOAD_TRAY_Picker01;

	CString strAxisName = "", strSecMsg = "";
	AxisTransToName( eMotor, strAxisName );

	CTimerCheck timeChecker( 15 );

	int nIdx = 0, nX = 0, nY = 0;
	for (int nIdx : m_vHandAxis[eMotor])
	{
		nAxes[nY][nX] = nIdx;
		
		timeChecker.StartTimer();
		do
		{
			nRet = m_bbxmnpWnd.MxCheckDone( eMULTI_OPER_MAX, nAxes[nY], nDones );
			if( timeChecker.CheckOverTime() )
			{
				CStringList strList;
				strSecMsg.Format( "%s // (Move)", strAxisName );

				return FALSE;
			}
			NEXT;
		} while( nRet != 1 );

		if (nIdx > eMULTI_OPER_MAX)
		{
			nX = 0;
			nY++;
		}
		else
			nX++;
	}

	if( nDones[ ePICKER_REAR ] == emRUNNING || nDones[ ePICKER_FRONT ] == emRUNNING )
	{
		nRetValue = 0;
	}
	else
	{
		nRetValue = TMC_RV_OK;
	}

	return nRetValue;
}

/*=============================================================================
DESCRIPT : Multi Axes에 대하여 이송완료를 확인
KEY WORD :
ARGUMENT :
RETURN   : nRetValue => 1 : 이송완료, 0 : 이송중, 그외의 값 : 에러
==============================================================================*/
int CBbxmnpClient::CheckMoveEnd( int nNumOfAxes, int* anAxes )
{
	if( m_nSimul == 1 ) {
		return TMC_RV_OK;
	}
	if( nNumOfAxes <= 0 ) {
		return TMC_RV_OK;
	}
	int nRetValue = 0;
	bool bLockFlag = false;

	int* anDones = new int[ nNumOfAxes ];
	memset( anDones, 0, sizeof( int )*nNumOfAxes );

	int nRet = m_bbxmnpWnd.MxCheckDone( nNumOfAxes, anAxes, anDones );
	if( TMC_RV_OK == nRet )
	{
		int nCnt = 0;
		for( int i = 0; i < nNumOfAxes; i++ )
		{
			if( emSTAND == anDones[ i ] )
			{
				nCnt++;
			}
		}

		// 모든 축이 정지되었다면 이송이 완료된 것이다.
		if( nNumOfAxes == nCnt )
		{
			nRetValue = TMC_RV_OK;
		}
		else
		{
			nRetValue = 0;	// 이송중
		}
	}
	else
	{
		nRetValue = nRet;
	}

	delete[] anDones;
return 0;
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
int CBbxmnpClient::Stop( _ePICK_MOTOR eMotor )
{
	int nRetValue = 0;
	int nRet[ ePICKER_MAX ] = { 0, };
	int nAxes[ ePICKER_MAX ][ eMULTI_OPER_MAX ] = { { 0, },{ 0, } };
	int nPickerStart = ( eMotor == PICK_MOTOR_LOAD_TRAY ) ? eBMAxis_LOAD_TRAY_Picker01 : eBMAxis_UNLOAD_TRAY_Picker01;
	bool bLockFlag = false;

	int nIdx = 0, nX = 0, nY = 0;
	for (int nIdx : m_vHandAxis[eMotor])
	{
		nAxes[nY][nX] = nIdx;
		nRet[nY] = m_bbxmnpWnd.MxStop(eMULTI_OPER_MAX, nAxes[nY]);
		if (nIdx > eMULTI_OPER_MAX)
		{
			nX = 0;
			nY++;
		}
		else
			nX++;	
	}

// 	for( int nPicPos = 0; nPicPos < ePICKER_MAX; nPicPos++ ) {
// 		for( int i = nPickerStart; i < nPickerStart + eMULTI_OPER_MAX; i++ ) {
// 			nAxes[ nPicPos ][ nIdx ] = i;
// 			nIdx++;
// 		}
// 		nRet[ nPicPos ] = m_bbxmnpWnd.MxStop( eMULTI_OPER_MAX, nAxes[ nPicPos ] );
// 
// 		nIdx = 0;
// 		nPickerStart += eMULTI_OPER_MAX;
// 	}


	nRetValue = nRet[ ePICKER_REAR ] & nRet[ ePICKER_FRONT ];

	return 0;
}

int CBbxmnpClient::Stop( _eBbxmnpAxis eAxis )
{
	int nRetValue = 0;
	bool bLockFlag = false;

	nRetValue = m_bbxmnpWnd.AxStop( eAxis );

	return nRetValue;
}

int CBbxmnpClient::Stop( int nNumOfAxes, int* anAxes )
{
	int nRetValue = 0;
	bool bLockFlag = false;

	nRetValue = m_bbxmnpWnd.MxStop( nNumOfAxes, anAxes );

	return nRetValue;
}

int CBbxmnpClient::EStop( _ePICK_MOTOR eMotor )
{
	int nRetValue = 0;
	int nRet[ ePICKER_MAX ] = { 0, };
	int nAxes[ ePICKER_MAX ][ eMULTI_OPER_MAX ] = { { 0, },{ 0, } };
	int nPickerStart = ( eMotor == PICK_MOTOR_LOAD_TRAY ) ? eBMAxis_LOAD_TRAY_Picker01 : eBMAxis_UNLOAD_TRAY_Picker01;
	bool bLockFlag = false;

	int nIdx = 0;
	for( int nPicPos = 0; nPicPos < ePICKER_MAX; nPicPos++ ) {
		for( int i = nPickerStart; i < ( nPickerStart + eMULTI_OPER_MAX ); i++ ) {
			nAxes[ nPicPos ][ nIdx ] = i;
			nIdx++;
		}
		nRet[ nPicPos ] = m_bbxmnpWnd.MxEStop( eMULTI_OPER_MAX, nAxes[ nPicPos ] );

		nIdx = 0;
		nPickerStart += eMULTI_OPER_MAX;
	}


	nRetValue = nRet[ ePICKER_REAR ] & nRet[ ePICKER_FRONT ];

	return 0;
}

int CBbxmnpClient::EStop( _eBbxmnpAxis eAxis )
{
	int nRetValue = 0;
	bool bLockFlag = false;

	nRetValue = m_bbxmnpWnd.AxEStop( eAxis );

	return nRetValue;
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
int CBbxmnpClient::EStop( int nNumOfAxes, int* anAxes )
{
	int nRetValue = 0;
	bool bLockFlag = false;

	nRetValue = m_bbxmnpWnd.MxEStop( nNumOfAxes, anAxes );

	return nRetValue;
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
void CBbxmnpClient::SetOriginFlag( _ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis, bool bFlag )
{
	m_bOriginFlag[ eMotor ][ eAxis ] = bFlag;
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
void CBbxmnpClient::SetOriginFlag( _ePICK_MOTOR eMotor, bool bFlag )
{
	for( int i = 0; i < eBMAxis_Picker_MAX; i++ ) {
		m_bOriginFlag[ eMotor ][ i ] = bFlag;
	}
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
bool CBbxmnpClient::GetOriginFlag( _ePICK_MOTOR eMotor )
{
	int nCnt = 0;
	bool bRetValue = false;

	for( int i = 0; i < eBMAxis_Picker_MAX; i++ ) {
		if( m_bOriginFlag[ eMotor ][ i ] != true ) {
			nCnt++;
		}
	}

	bRetValue = ( nCnt == 0 ) ? true : false;
	return bRetValue;
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
bool CBbxmnpClient::GetOriginFlag( _ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis )
{
	int nPickerNum = 0;

	switch (eMotor)
	{
	case CBbxmnpClient::PICK_MOTOR_LOAD_TRAY:{nPickerNum = eAxis - eBMAxis_LOAD_TRAY_Picker01;}break;
	case CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1: {nPickerNum = eAxis - eBMAxis_LOAD_TESTPP_1_Picker01; }break;
	case CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1: {nPickerNum = eAxis - eBMAxis_UNLOAD_TESTPP_1_Picker01; }break;
	case CBbxmnpClient::PICK_MOTOR_UNLOAD_TRAY: {nPickerNum = eAxis - eBMAxis_UNLOAD_TRAY_Picker01; }break;
	case CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2: {nPickerNum = eAxis - eBMAxis_LOAD_TESTPP_2_Picker01; }break;
	case CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2: {nPickerNum = eAxis - eBMAxis_UNLOAD_TESTPP_2_Picker01; }break;
	default:
		break;
	}

	return m_bOriginFlag[ eMotor ][ nPickerNum ];
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
void CBbxmnpClient::SetTargetPos( _ePICK_MOTOR eMotor, double dTargetPos )
{
	m_adTargetPos[ eMotor ] = dTargetPos;
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
double CBbxmnpClient::GetTargetPos( _ePICK_MOTOR eMotor )
{
	return m_adTargetPos[ eMotor ];
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
void CBbxmnpClient::SetLastPos( _ePICK_MOTOR eMotor, int nPickerNo, double dLastPos )
{
	m_adLastPos[ eMotor ][ nPickerNo ] = dLastPos;
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
double CBbxmnpClient::GetLastPos( _ePICK_MOTOR eMotor, int nPickerNo )
{
	return m_adLastPos[ eMotor ][ nPickerNo ];
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
void CBbxmnpClient::AxisTransToName( _eBbxmnpAxis eAxis, CString& strText )
{
	if(eBMAxis_UNLOAD_TRAY_Picker01 <= eAxis && eBMAxis_UNLOAD_TRAY_Picker08 >= eAxis ) {
		strText = "[Unload Tray Z-AXIS]";
	}
	else if( eBMAxis_LOAD_TRAY_Picker01 <= eAxis && eBMAxis_LOAD_TRAY_Picker08 >= eAxis ) {
		strText = "[Load Tray Z-AXIS]";
	}
	if (eBMAxis_UNLOAD_TESTPP_1_Picker01 <= eAxis && eBMAxis_UNLOAD_TESTPP_1_Picker08 >= eAxis) {
		strText = "[Test PP 1 Z1-AXIS]";
	}
	else if (eBMAxis_LOAD_TESTPP_1_Picker01 <= eAxis && eBMAxis_LOAD_TESTPP_1_Picker08 >= eAxis) {
		strText = "[Test PP 1 Z2-AXIS]";
	}
	if (eBMAxis_UNLOAD_TESTPP_2_Picker01 <= eAxis && eBMAxis_UNLOAD_TESTPP_2_Picker08 >= eAxis) {
		strText = "[Test PP 2 Z1-AXIS]";
	}
	else if (eBMAxis_LOAD_TESTPP_2_Picker01 <= eAxis && eBMAxis_LOAD_TESTPP_2_Picker08 >= eAxis) {
		strText = "[Test PP 2 Z2-AXIS]";
	}
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
void CBbxmnpClient::AxisTransToName( _ePICK_MOTOR eMotor, CString& strText )
{
	if( PICK_MOTOR_LOAD_TRAY == eMotor ) {
		strText = "[Load Tray Z-AXIS]";
	}
	else if(PICK_MOTOR_UNLOAD_TRAY == eMotor ) {
		strText = "[UnLoad Tray Z-AXIS]";
	}
	if (PICK_MOTOR_LOAD_TEST_1 == eMotor) {
		strText = "[Test PP 1 Z1-AXIS]";
	}
	else if (PICK_MOTOR_UNLOAD_TEST_1 == eMotor) {
		strText = "[Test PP 1 Z2-AXIS]";
	}
	if (PICK_MOTOR_LOAD_TEST_2 == eMotor) {
		strText = "[Test PP 2 Z1-AXIS]";
	}
	else if (PICK_MOTOR_UNLOAD_TEST_2 == eMotor) {
		strText = "[Test PP 2 Z2-AXIS]";
	}
}

int CBbxmnpClient::ZAxisStartPicker(_ePICK_MOTOR eMotor)
{
	int nStartPicker = 0;
	if (PICK_MOTOR_LOAD_TRAY == eMotor) {
		nStartPicker = eBMAxis_LOAD_TRAY_Picker01;
	}
	else if (PICK_MOTOR_UNLOAD_TRAY == eMotor) {
		nStartPicker = eBMAxis_UNLOAD_TRAY_Picker01;
	}
	if (PICK_MOTOR_LOAD_TEST_1 == eMotor) {
		nStartPicker = eBMAxis_LOAD_TESTPP_1_Picker01;	
	}
	else if (PICK_MOTOR_UNLOAD_TEST_1 == eMotor) {
		nStartPicker = eBMAxis_UNLOAD_TESTPP_1_Picker01;
	}
	if (PICK_MOTOR_LOAD_TEST_2 == eMotor) {
		nStartPicker = eBMAxis_LOAD_TESTPP_2_Picker01;
	}
	else if (PICK_MOTOR_UNLOAD_TEST_2 == eMotor) {
		nStartPicker = eBMAxis_UNLOAD_TESTPP_2_Picker01;
	}
	return nStartPicker;
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
void CBbxmnpClient::AxisTransToPickMotor( _eBbxmnpAxis eAxis, _ePICK_MOTOR& eMotor )
{
	if (eBMAxis_LOAD_TRAY_Picker01 <= eAxis && eBMAxis_LOAD_TRAY_Picker08 >= eAxis) {
		eMotor = PICK_MOTOR_LOAD_TRAY;
	}
	else if (eBMAxis_UNLOAD_TRAY_Picker01 <= eAxis && eBMAxis_UNLOAD_TRAY_Picker08 >= eAxis) {
		eMotor = PICK_MOTOR_UNLOAD_TRAY;
	}	
	if (eBMAxis_UNLOAD_TESTPP_1_Picker01 <= eAxis && eBMAxis_UNLOAD_TESTPP_1_Picker08 >= eAxis) {
		eMotor = PICK_MOTOR_UNLOAD_TEST_1;
	}
	else if (eBMAxis_LOAD_TESTPP_1_Picker01 <= eAxis && eBMAxis_LOAD_TESTPP_1_Picker08 >= eAxis) {
		eMotor = PICK_MOTOR_LOAD_TEST_1;
	}
	if (eBMAxis_UNLOAD_TESTPP_2_Picker01 <= eAxis && eBMAxis_UNLOAD_TESTPP_2_Picker08 >= eAxis) {
		eMotor = PICK_MOTOR_UNLOAD_TEST_2;
	}
	else if (eBMAxis_LOAD_TESTPP_2_Picker01 <= eAxis && eBMAxis_LOAD_TESTPP_2_Picker08 >= eAxis) {
		eMotor = PICK_MOTOR_LOAD_TEST_2;
	}
}


void CBbxmnpClient::AxisTransToVatHand(_ePICK_MOTOR eMotor, int &nHand)
{
	if (PICK_MOTOR_LOAD_TRAY == eMotor) {
		nHand = 0;
	}
	else if (PICK_MOTOR_UNLOAD_TRAY == eMotor) {
		nHand = 1;
	}
	else if (PICK_MOTOR_LOAD_TEST_1 == eMotor) {
		nHand = 2;
	}
	else if (PICK_MOTOR_UNLOAD_TEST_1 == eMotor) {
		nHand = 3;
	}
	else if (PICK_MOTOR_LOAD_TEST_2 == eMotor) {
		nHand = 4;
	}
	else if (PICK_MOTOR_UNLOAD_TEST_2 == eMotor) {
		nHand = 5;
	}
}
/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/


/*=============================================================================
DESCRIPT : 기존의 Ramdata의 Input/Output Z축 Parameter를 그대로 활용하기 위한 함수
		   Ramdata <- CBbxmnpClient 파라미터 설정을 덮어 씌운다.
KEY WORD :
ARGUMENT :
RETURN   :
==============================================================================*/
//void CBbxmnpClient::TransToRamdataMotorParam( _ePICK_MOTOR eMotor, St_MotorParameter& ramdataMotorParam )
//{
// 	int nIndex = ( eMotor == PICK_MOTOR_LOAD_TRAY ) ? INPUT_ARM_Z_AXIS : OUTPUT_ARM_Z_AXIS;
// 
// 	ramdataMotorParam.Speed[ nIndex ] = ( int )m_stMotorParam[ eMotor ].dSpeed;
// 	ramdataMotorParam.AccSpeed[ nIndex ] = ( int )m_stMotorParam[ eMotor ].dAccTime;
// 	ramdataMotorParam.DccSpeed[ nIndex ] = ( int )m_stMotorParam[ eMotor ].dDecTime;
// 	ramdataMotorParam.OriginSpeed[ nIndex ] = ( int )m_stMotorParam[ eMotor ].dOrgSpeed;
// 	ramdataMotorParam.OriginAccSpeed[ nIndex ] = ( int )m_stMotorParam[ eMotor ].dOrgAccDecTime;
// 	ramdataMotorParam.Posi[ nIndex ] = ( int )m_stMotorParam[ eMotor ].dSwLimitP;
// 	ramdataMotorParam.Nega[ nIndex ] = ( int )m_stMotorParam[ eMotor ].dSwLimitN;
//}

/*=============================================================================
DESCRIPT : 기존의 Ramdata의 Input/Output Z축 Parameter를 그대로 활용하기 위한 함수
		   Ramdata -> CBbxmnpClient 파라미터 설정을 덮어 씌운다.
KEY WORD :
ARGUMENT :
RETURN   :
==============================================================================*/
//void CBbxmnpClient::TransToBbxmnpClientMotorParam( _ePICK_MOTOR eMotor, const St_MotorParameter& ramdataMotorParam )
//{
// 	int nIndex = ( eMotor == PICK_MOTOR_INPUT ) ? INPUT_ARM_Z_AXIS : OUTPUT_ARM_Z_AXIS;
// 
// 	m_stMotorParam[ eMotor ].dSpeed = ( double )ramdataMotorParam.Speed[ nIndex ];
// 	m_stMotorParam[ eMotor ].dAccTime = ( double )ramdataMotorParam.AccSpeed[ nIndex ];
// 	m_stMotorParam[ eMotor ].dDecTime = ( double )ramdataMotorParam.DccSpeed[ nIndex ];
// 	m_stMotorParam[ eMotor ].dOrgSpeed = ( double )ramdataMotorParam.OriginSpeed[ nIndex ];
// 	m_stMotorParam[ eMotor ].dOrgRevSpeed = ( double )ramdataMotorParam.OriginSpeed[ nIndex ];
// 	m_stMotorParam[ eMotor ].dOrgAccDecTime = ( double )ramdataMotorParam.OriginAccSpeed[ nIndex ];
// 	m_stMotorParam[ eMotor ].dSwLimitP = ( double )ramdataMotorParam.Posi[ nIndex ];
// 	m_stMotorParam[ eMotor ].dSwLimitN = ( double )ramdataMotorParam.Nega[ nIndex ];
//}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
int CBbxmnpClient::SetPulseType( _eBbxmnpAxis eAxis, int nType )
{
	return m_bbxmnpWnd.AxSetPulseType( eAxis, nType );
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
int CBbxmnpClient::SetOrgLevel( _eBbxmnpAxis eAxis, int nLevel )
{
	return m_bbxmnpWnd.AxSetOrgLevel( eAxis, nLevel );
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
int CBbxmnpClient::SetOrgLevel( _ePICK_MOTOR eMotor, int nLevel )
{
	int nRet = 0, nTemp = 0;

	for (int nIdx : m_vHandAxis[eMotor])  {
		nTemp = m_bbxmnpWnd.AxSetOrgLevel( _eBbxmnpAxis(m_vHandMatrix[nIdx]), nLevel );
		if( nTemp != TMC_RV_OK ) {
			nRet = nTemp;
		}
	}

	return 0;
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
void CBbxmnpClient::SetOperAxes( _ePICK_MOTOR eMotor, std::vector<CPoint> vPicker, int nOperAxes[ 2 ][ 4 ], int nOperAxesCnt[ PICK_MOTOR_MAX ] )
{
	int nAxes[2][8] = { 0, };
	memset(nAxes, 0x00, sizeof(nAxes));
	for (int i = 0; i < (int)vPicker.size(); i++)
	{
		nAxes[vPicker[i].y][vPicker[i].x] = 1;
	}

	int nIdxRear = 0;
	int nIdxFront = 0;

	if (m_nBoardType == eBoardDir_Forward)
	{
		for (int nY = 0; nY < MAX_PICKER_Y_CNT; nY++) {
			for (int nX = 0; nX < MAX_PICKER_X_CNT; nX++) {
				if (nAxes[nY][nX] > 0) {
					nOperAxes[nY][nX] = m_vHandAxis[eMotor][nX + (MAX_PICKER_X_CNT * nY)];
					nOperAxesCnt[nY]++;
					// 				if( /*m_nHandMatrix[ eMotor ][ nY ][ nX ] <*/ nFindPicker ) {
					// 					nOperAxes[ ePICKER_REAR ][ nIdxRear++ ] = /*m_nHandMatrix[ eMotor ][ nY ][ nX ]*/; /*_eBbxmnpAxis(nX + (nY * 4))*/;
					// 					nOperAxesCnt[ ePICKER_REAR ]++;
					// 				}
					// 				else {
					// 					nOperAxes[ ePICKER_FRONT ][ nIdxFront++ ] = m_nHandMatrix[ eMotor ][ nY ][ nX ]; /*_eBbxmnpAxis(nX + (nY * 4))*/;
					// 					nOperAxesCnt[ ePICKER_FRONT ]++;
					// 				}
				}
			}
		}
	}
	else //eBoardDir_Backward
	{
		if (eMotor == PICK_MOTOR_LOAD_TEST_1 || eMotor == PICK_MOTOR_UNLOAD_TEST_1 || eMotor == PICK_MOTOR_LOAD_TEST_2 || eMotor == PICK_MOTOR_UNLOAD_TEST_2)
		{
			for (int nY = 0; nY < MAX_PICKER_Y_CNT; nY++) {
				for (int nX = 0; nX < MAX_PICKER_X_CNT; nX++) {
					if (nAxes[nY][nX] > 0) {
						//int nRow = (nY == 0) ? 1 : 0;
						int nIdx = nX + (MAX_PICKER_X_CNT * nY);
						int nGap = abs(7 - nIdx);
						//nOperAxes[nY][nX] = m_vHandAxis[eMotor][nX + (MAX_PICKER_X_CNT * nRow)];
						nOperAxes[nY][nX] = m_vHandAxis[eMotor][nGap];
						nOperAxesCnt[nY]++;
					}
				}
			}
		}
		else
		{
			for (int nY = 0; nY < MAX_PICKER_Y_CNT; nY++) {
				for (int nX = 0; nX < MAX_PICKER_X_CNT; nX++) {
					if (nAxes[nY][nX] > 0) {
						nOperAxes[nY][nX] = m_vHandAxis[eMotor][nX + (MAX_PICKER_X_CNT * nY)];
						nOperAxesCnt[nY]++;
					}
				}
			}
		}
	}
	/*
	eBMAxis_LOAD_TESTPP_1_Picker01,			eBMAxis_LOAD_TESTPP_1_Picker08,
	eBMAxis_LOAD_TESTPP_1_Picker02,			eBMAxis_LOAD_TESTPP_1_Picker07,
	eBMAxis_LOAD_TESTPP_1_Picker03,			eBMAxis_LOAD_TESTPP_1_Picker06,
	eBMAxis_LOAD_TESTPP_1_Picker04,			eBMAxis_LOAD_TESTPP_1_Picker05,
	eBMAxis_LOAD_TESTPP_1_Picker05,			eBMAxis_LOAD_TESTPP_1_Picker04,
	eBMAxis_LOAD_TESTPP_1_Picker06,
	eBMAxis_LOAD_TESTPP_1_Picker07,
	eBMAxis_LOAD_TESTPP_1_Picker08,
	
	*/
	//eBoardDir_Forward


	return;
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
BOOL CBbxmnpClient::GetBoardCommSts( int *pBoardConnectSts )
{
	BOOL bRetVAl = FALSE;	// 반환값 확인		
	bRetVAl = m_bbxmnpWnd.GetBoardCommSts( pBoardConnectSts );

	return bRetVAl;
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
void CBbxmnpClient::OnReceiveAlarm( CWnd* pWnd, LPARAM lParam, WPARAM wParam )
{
	CBbxmnpClient* pSelf = ( CBbxmnpClient* )pWnd;

	pSelf->OnAlarmDisplay();
}

/*=============================================================================
PARAMS   :
RETURN   :
COMMENTS :
==============================================================================*/
void CBbxmnpClient::OnAlarmDisplay()
{
// 	_Start.SetStatus( FALSE );
// 	_Stop.SetStatus( TRUE );
// 	NEXT;
	// 1532 : Hand z축 motion board와 연결에 문제가 있습니다. 게속적으로 해당 메시지가 발생 하면 hand motion board 통신 연결 상태를 확인 해주세요.
	//g_ERRORMSG.ShowMessageBox( g_ERRORMSG.ErrMsg( "1532" ) );	// hand z축 motor 통신 이상 알람
}

BOOL CBbxmnpClient::GetHomeSensorStatus( _ePICK_MOTOR eMotor )
{
	if( m_nSimul == 1 ) {
		return TRUE;
	}

	int nCardNum = 0;
	int nSlaveNum = 0;
	int nOrigin[ MAX_SLAVE_BOARD_CNT ][ MAX_PICKER_CNT_EACH_SLAVE ] = { 0, };
	BOOL bRet = TRUE;
	// slave number (0: Hand Type, 1: rear picker, 2:front picker, )
	// -----------------
	//0:Hand Type  1: |1|2|3|4|
	//			   2: |1|2|3|4|

	int nBDSlaveStartNum = 0;
	int nBDSlaveEndNum = 0;

	int nBDSlaveStartIdx = 0;
	int nBDSlaveEndIdx = 0;

	int nCount = 0;
	//BoardSlaveNumber(eMotor, nBDSlaveStartNum, nBDSlaveEndNum);
	BoardSlaveIndex(eMotor, nBDSlaveStartIdx, nBDSlaveEndIdx);
	nCardNum = BoardNumber(eMotor);
	for( int nSlaveNum = nBDSlaveStartIdx; nSlaveNum <= nBDSlaveEndIdx; nSlaveNum++ )
	{
		int nRet = m_bbxmnpWnd.AxGetHomeSensorStatus(nCardNum, m_veBDSlaveNum[nSlaveNum], nOrigin[nSlaveNum] );

		if( nRet != TMC_RV_OK )
		{
			bRet = FALSE;
			break;
		}

		bRet = CheckHomeSensor( nOrigin[nSlaveNum] );
		if( bRet == FALSE )
			break;

		nCount++;
	}
	
	return bRet;
}

BOOL CBbxmnpClient::CheckHomeSensor( int nOrigin[ MAX_PICKER_CNT_EACH_SLAVE ] )
{
	BOOL bRet = TRUE;

	for( int j = 0; j < 4; j++ )
	{
		if( nOrigin[ j ] == 0 )
		{
			bRet = FALSE;
			break;
		}
	}

	return bRet;
}

BOOL CBbxmnpClient::GetHomeSensorStatusEach( _ePICK_MOTOR eMotor, int nPickerAxis )
{
	int nCardNum = 0;
	int nSlaveNum = 0;
	int nOrigin[ MAX_SLAVE_BOARD_CNT ][ MAX_PICKER_CNT_EACH_SLAVE ] = { 0, };
	BOOL bRet = TRUE;

	int nBDSlaveStartNum= 0;
	int nBDSlaveEndNum = 0;

	BoardSlaveIndex(eMotor, nBDSlaveStartNum, nBDSlaveEndNum);
	nCardNum = BoardNumber(eMotor);
	for (int nSlaveNum = nBDSlaveStartNum; nSlaveNum <= nBDSlaveEndNum; nSlaveNum++)
	{
		int nRet = m_bbxmnpWnd.AxGetHomeSensorStatus(nCardNum, m_veBDSlaveNum[nSlaveNum], nOrigin[nSlaveNum]);

		if (nRet != TMC_RV_OK)
		{
			bRet = FALSE;
			break;
		}

		bRet = CheckHomeSensorEach(nOrigin[nSlaveNum], nPickerAxis);
		if (bRet == FALSE)
			break;
	}
	return bRet;
}

BOOL CBbxmnpClient::GetHomeSensorStatus(_ePICK_MOTOR eMotor, int nPickerAxis)
{
	
	int nOrigin[MAX_SLAVE_BOARD_CNT][MAX_PICKER_CNT_EACH_SLAVE] = { 0, };
	BOOL bRet = TRUE;

	int nBDSlaveStartNum = 0;
	int nBDSlaveEndNum = 0;

	int nSlaveNumber = BoardSlaveNumber(eMotor, nPickerAxis);
	int nSlavePickerNum = BoardSlavePickerNumber(eMotor, nPickerAxis);
	int nCardNum = BoardNumber(eMotor);

	int nRet = m_bbxmnpWnd.AxGetHomeSensorStatus(nCardNum, m_veBDSlaveNum[nSlaveNumber], nOrigin[nSlaveNumber]);

	if (nRet != TMC_RV_OK)
	{
		bRet = FALSE;
		return bRet;
	}

	bRet = nOrigin[nSlaveNumber][nSlavePickerNum];
	
	return bRet;
}


int CBbxmnpClient::Reset( _ePICK_MOTOR eMotor )
{
	for (int nIdx : m_vHandAxis[eMotor]) {
		return m_bbxmnpWnd.AxSetServoReset((_eBbxmnpAxis)(m_vHandMatrix[nIdx]), 0);
	}
	return 0;
}

BOOL CBbxmnpClient::CheckHomeSensorEach( int nOrigin[ MAX_PICKER_CNT_EACH_SLAVE ], int nPickerAxis )
{
	BOOL bRet = TRUE;
	if( nOrigin[ nPickerAxis ] == 0 )
	{
		bRet = FALSE;
	}
	return bRet;
}

double CBbxmnpClient::ApplyZMotorFlatOffset( _eBbxmnpAxis eAxis )
{
	int nY = 0;
	int nX = 0;

	switch( eAxis )
	{
		case eBMAxis_LOAD_TRAY_Picker01:
		case eBMAxis_UNLOAD_TRAY_Picker01:
			{
				nY = 0;
				nX = 0;
			}break;
		case eBMAxis_LOAD_TRAY_Picker02:
		case eBMAxis_UNLOAD_TRAY_Picker02:
			{
				nY = 1;
				nX = 0;
			}break;
		case eBMAxis_LOAD_TRAY_Picker03:
		case eBMAxis_UNLOAD_TRAY_Picker03:
			{
				nY = 0;
				nX = 1;
			}break;
		case eBMAxis_LOAD_TRAY_Picker04:
		case eBMAxis_UNLOAD_TRAY_Picker04:
			{
				nY = 1;
				nX = 1;
			}break;
		case eBMAxis_LOAD_TRAY_Picker05:
		case eBMAxis_UNLOAD_TRAY_Picker05:
			{
				nY = 0;
				nX = 2;
			}break;
		case eBMAxis_LOAD_TRAY_Picker06:
		case eBMAxis_UNLOAD_TRAY_Picker06:
			{
				nY = 1;
				nX = 2;
			}break;
		case eBMAxis_LOAD_TRAY_Picker07:
		case eBMAxis_UNLOAD_TRAY_Picker07:
			{
				nY = 0;
				nX = 3;
			}break;
		case eBMAxis_LOAD_TRAY_Picker08:
		case eBMAxis_UNLOAD_TRAY_Picker08:
			{
				nY = 1;
				nX = 3;
			}break;
	}

	double dFlatOffset = 0;

	if( eBMAxis_LOAD_TRAY_Picker01 <= eAxis && eBMAxis_UNLOAD_TRAY_Picker01 >= eAxis )
	{
		//dFlatOffset = g_RAM_DATA.m_stVATZTeachInfo.dZMotorPickerFlatOffset[ VAT_AREA_INPUT ][ nY ][ nX ];
	}
	else // eBMAxis_Output_Picker01 <= eAxis && eBMAxis_Output_Picker08 >= eAxis
	{
		//dFlatOffset = g_RAM_DATA.m_stVATZTeachInfo.dZMotorPickerFlatOffset[ VAT_AREA_OUTPUT ][ nY ][ nX ];
	}

	if( fabs( dFlatOffset ) > 2000.0 )
	{
		dFlatOffset = 0;
	}

	return 0;

}

int CBbxmnpClient::BoardSlaveNumber(_ePICK_MOTOR eMotor, int nPickerAxis)
{
	int nBDSlaveNum = 0;
	switch (eMotor)
	{
	case PICK_MOTOR_LOAD_TRAY:
		{
			if(nPickerAxis - eBMAxis_LOAD_TRAY_Picker01 < 4)
				nBDSlaveNum = eBMAxis_LOAD_TRAY_PICKER_SLAVE_NUM1;
			else
				nBDSlaveNum = eBMAxis_LOAD_TRAY_PICKER_SLAVE_NUM2;
		}break;
	case PICK_MOTOR_LOAD_TEST_1:
	{
		if (nPickerAxis - eBMAxis_LOAD_TESTPP_1_Picker01< 4)
			nBDSlaveNum = eBMAxis_TEST_PP1_LOAD_HAND_PICKER_SLAVE_NUM1;
		else
			nBDSlaveNum = eBMAxis_TEST_PP1_LOAD_HAND_PICKER_SLAVE_NUM2;
	}break;
	case PICK_MOTOR_UNLOAD_TEST_1:
	{
		if (nPickerAxis - eBMAxis_UNLOAD_TESTPP_1_Picker01< 4)
			nBDSlaveNum = eBMAxis_TEST_PP1_UNLOAD_HAND_PICKER_SLAVE_NUM1;
		else
			nBDSlaveNum = eBMAxis_TEST_PP1_UNLOAD_HAND_PICKER_SLAVE_NUM2;
	}break;
	case PICK_MOTOR_UNLOAD_TRAY:
	{
		if (nPickerAxis - eBMAxis_UNLOAD_TRAY_Picker01< 4)
			nBDSlaveNum = eBMAxis_UNLOAD_TRAY_PICKER_SLAVE_NUM1;
		else
			nBDSlaveNum = eBMAxis_UNLOAD_TRAY_PICKER_SLAVE_NUM2;
	}break;
	case PICK_MOTOR_LOAD_TEST_2:
	{
		if (nPickerAxis - eBMAxis_LOAD_TESTPP_2_Picker01 < 4)
			nBDSlaveNum = eBMAxis_TEST_PP2_LOAD_HAND_PICKER_SLAVE_NUM1;
		else
			nBDSlaveNum = eBMAxis_TEST_PP2_LOAD_HAND_PICKER_SLAVE_NUM2;
	}break;
	case PICK_MOTOR_UNLOAD_TEST_2:
	{
		if (nPickerAxis - eBMAxis_UNLOAD_TESTPP_2_Picker01< 4)
			nBDSlaveNum = eBMAxis_TEST_PP2_UNLOAD_HAND_PICKER_SLAVE_NUM1;
		else
			nBDSlaveNum = eBMAxis_TEST_PP2_UNLOAD_HAND_PICKER_SLAVE_NUM2;
	}break;
	}
	return nBDSlaveNum;
}

int CBbxmnpClient::BoardSlavePickerNumber(_ePICK_MOTOR eMotor, int nPickerAxis)
{
	int nSlavePickerNumber = 0;
	switch (eMotor)
	{
	case PICK_MOTOR_LOAD_TRAY:
	{
		if (nPickerAxis - eBMAxis_LOAD_TRAY_Picker01 < 4)
			nSlavePickerNumber = nPickerAxis - eBMAxis_LOAD_TRAY_Picker01;
		else
			nSlavePickerNumber = nPickerAxis - eBMAxis_LOAD_TRAY_Picker05;
	}break;
	case PICK_MOTOR_LOAD_TEST_1:
	{
		if (nPickerAxis - eBMAxis_LOAD_TESTPP_1_Picker01 < 4)
			nSlavePickerNumber = nPickerAxis - eBMAxis_LOAD_TESTPP_1_Picker01;
		else
			nSlavePickerNumber = nPickerAxis - eBMAxis_LOAD_TESTPP_1_Picker05;
	}break;
	case PICK_MOTOR_UNLOAD_TEST_1:
	{
		if (nPickerAxis - eBMAxis_UNLOAD_TESTPP_1_Picker01< 4)
			nSlavePickerNumber = nPickerAxis - eBMAxis_UNLOAD_TESTPP_1_Picker01;
		else
			nSlavePickerNumber = nPickerAxis - eBMAxis_UNLOAD_TESTPP_1_Picker05;
	}break;
	case PICK_MOTOR_UNLOAD_TRAY:
	{
		if (nPickerAxis - eBMAxis_UNLOAD_TRAY_Picker01< 4)
			nSlavePickerNumber = nPickerAxis - eBMAxis_UNLOAD_TRAY_Picker01;
		else
			nSlavePickerNumber = nPickerAxis - eBMAxis_UNLOAD_TRAY_Picker05;
	}break;
	case PICK_MOTOR_LOAD_TEST_2:
	{
		if (nPickerAxis - eBMAxis_LOAD_TESTPP_2_Picker01 < 4)
			nSlavePickerNumber = nPickerAxis - eBMAxis_LOAD_TESTPP_2_Picker01;
		else
			nSlavePickerNumber = nPickerAxis - eBMAxis_LOAD_TESTPP_2_Picker05;
	}break;
	case PICK_MOTOR_UNLOAD_TEST_2:
	{
		if (nPickerAxis - eBMAxis_UNLOAD_TESTPP_2_Picker01 < 4)
			nSlavePickerNumber = nPickerAxis - eBMAxis_UNLOAD_TESTPP_2_Picker01;
		else
			nSlavePickerNumber = nPickerAxis - eBMAxis_UNLOAD_TESTPP_2_Picker05;
	}break;
	}
	return nSlavePickerNumber;
}

void CBbxmnpClient::BoardSlaveIndex(_ePICK_MOTOR eMotor, int &nBDStartNum, int &nBDEndNum)
{
	switch (eMotor)
	{
	case PICK_MOTOR_LOAD_TRAY:
	{
		nBDStartNum = eBMAxis_LOAD_TRAY_PICKER_SLAVE_NUM1;
		nBDEndNum = eBMAxis_LOAD_TRAY_PICKER_SLAVE_NUM2;
	}break;
	case PICK_MOTOR_LOAD_TEST_1:
	{
		nBDStartNum = eBMAxis_TEST_PP1_LOAD_HAND_PICKER_SLAVE_NUM1;
		nBDEndNum = eBMAxis_TEST_PP1_LOAD_HAND_PICKER_SLAVE_NUM2;
	}break;
	case PICK_MOTOR_UNLOAD_TEST_1:
	{
		nBDStartNum = eBMAxis_TEST_PP1_UNLOAD_HAND_PICKER_SLAVE_NUM1;
		nBDEndNum = eBMAxis_TEST_PP1_UNLOAD_HAND_PICKER_SLAVE_NUM2;
	}break;
	case PICK_MOTOR_UNLOAD_TRAY:
	{
		nBDStartNum = eBMAxis_UNLOAD_TRAY_PICKER_SLAVE_NUM1;
		nBDEndNum = eBMAxis_UNLOAD_TRAY_PICKER_SLAVE_NUM2;
	}break;
	case PICK_MOTOR_LOAD_TEST_2:
	{
		nBDStartNum = eBMAxis_TEST_PP2_LOAD_HAND_PICKER_SLAVE_NUM1;
		nBDEndNum = eBMAxis_TEST_PP2_LOAD_HAND_PICKER_SLAVE_NUM2;
	}break;
	case PICK_MOTOR_UNLOAD_TEST_2:
	{
		nBDStartNum = eBMAxis_TEST_PP2_UNLOAD_HAND_PICKER_SLAVE_NUM1;
		nBDEndNum = eBMAxis_TEST_PP2_UNLOAD_HAND_PICKER_SLAVE_NUM2;
	}break;
	}
}

int CBbxmnpClient::BoardNumber(_ePICK_MOTOR eMotor)
{
	int nBoardNum;
	switch (eMotor)
	{
	case CBbxmnpClient::PICK_MOTOR_LOAD_TRAY:
	case CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1:
	case CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1:
	{
		nBoardNum = HAND_PICKER_MASTER_BRD_NUM1;
		break;
	}
	case CBbxmnpClient::PICK_MOTOR_UNLOAD_TRAY:
	case CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2:
	case CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2:
	{
		nBoardNum = HAND_PICKER_MASTER_BRD_NUM2;
		break;
	}
	default:
		break;
	}
	return nBoardNum;
}

int CBbxmnpClient::GetZPickerMotorNumber(int nAxisNumber)
{
	int eMotorNum;

// 	switch (nAxisNumber)
// 	{
	if (eAXIS_TRAY_PP_1_Z == nAxisNumber)  { eMotorNum = CBbxmnpClient::PICK_MOTOR_LOAD_TRAY; }
	if (eAXIS_TEST_PP_1_Z == nAxisNumber)  { eMotorNum = CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1; }
	if(eAXIS_TEST_PP_1_Z2== nAxisNumber) {eMotorNum = CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1; }
	if(eAXIS_TRAY_PP_2_Z == nAxisNumber){eMotorNum = CBbxmnpClient::PICK_MOTOR_UNLOAD_TRAY; }
	if(eAXIS_TEST_PP_2_Z == nAxisNumber){eMotorNum = CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2; }
	if(eAXIS_TEST_PP_2_Z2== nAxisNumber) {eMotorNum = CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2; }
// 	default:
// 		break;
// 	}

	return eMotorNum;
}

void CBbxmnpClient::SetMaxVel(_ePICK_MOTOR eMotor)
{
	for (int nIdx : m_vHandAxis[eMotor]) {
		m_bbxmnpWnd.AxSetMaxVel(_eBbxmnpAxis(m_vHandMatrix[nIdx]), DEF_ZAXIS_MAX_SPEED);
	}
}

void CBbxmnpClient::MakeBMAxisPicker(std::vector<int>& vPicker, CBbxmnpClient::_ePICK_MOTOR eMotor)
{
	int nPicker_start = ZAxisStartPicker(eMotor);

	for (int y = 0; y < MAX_TRAY_PP_CNT_Y; y++) {
		for (int x = 0; x < MAX_TRAY_PP_CNT_X; x++) {
			vPicker.push_back(nPicker_start + (x + MAX_TRAY_PP_CNT_X * y));
		}
	}
}

double CBbxmnpClient::GetPickerOffset(CBbxmnpClient::_ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis)
{
	//um
	return m_stPickOffset[eMotor].dFlatOffset[eAxis];
}


// void CBbxmnpClient::SetZAutoTeachingData(double dHandGap[])
// {
//  	for (int i = 0; i < eBMAxis_Picker_MAX; i++)
//  		m_dHandGap[i] = dHandGap[i];
// // 	memcpy(m_dHandGap, dHandGap, sizeof(&dHandGap));
// }




/*                                                      
//////////////////////////////////////////////////////////////////////////
Hand별 기준 Picker (정방향 기준 으로 표시 함)   () <----기준 피커
Test PP1 UnLoad PP					Test PP2 UnLoad PP
     4     8							5    1
	 3     7							6    2
	(2)    6							7   (3)
	 1     5							8    4
Test PP1 Load PP					Test PP2 Load PP
	 4     8							5    1		
    (3)    7							6   (2)
	 2     6							7    3
	 1     5							8    4
					Tray Load PP				
					1 | 2 | 3 | 4
					5 | 6 |(7)| 8

					Tray UnLoad PP
					1 | 2 | 3 | 4
					5 | 6 |(7)| 8

					Picker CPoint 배열
					0,0|1,0|2,0|3,0
					0,1|1,1|2,1|3,1

//////////////////////////////////////////////////////////////////////////
*/

//기준 Cam이 장착 되어 있는 Picker 정의
#define DEF_VISION_CAM_STANDARD_PICKER_LOAD_TRAY						CPoint(2,1)
#define DEF_VISION_CAM_STANDARD_PICKER_UNLOAD_TRAY						CPoint(1,1)
//Station Board 정방향 
#define DEF_VISION_CAM_STANDARD_PICKER_TESTPP1_LOAD_TRAY_FWD			CPoint(2,0)
#define DEF_VISION_CAM_STANDARD_PICKER_TESTPP1_UNLOAD_TRAY_FWD			CPoint(1,0)
#define DEF_VISION_CAM_STANDARD_PICKER_TESTPP2_LOAD_TRAY_FWD			CPoint(1,0)
#define DEF_VISION_CAM_STANDARD_PICKER_TESTPP2_UNLOAD_TRAY_FWD			CPoint(2,0)
//Station Board 역방향 
#define DEF_VISION_CAM_STANDARD_PICKER_TESTPP1_LOAD_TRAY_BWD			CPoint(2,1)
#define DEF_VISION_CAM_STANDARD_PICKER_TESTPP1_UNLOAD_TRAY_BWD			CPoint(2,1)
#define DEF_VISION_CAM_STANDARD_PICKER_TESTPP2_LOAD_TRAY_BWD			CPoint(2,1)
#define DEF_VISION_CAM_STANDARD_PICKER_TESTPP2_UNLOAD_TRAY_BWD			CPoint(1,1)

std::vector<CPoint> CBbxmnpClient::VisionCamStandardPicker(CBbxmnpClient::_ePICK_MOTOR eMotor)
{
	std::vector<CPoint> vStdPicker;
	vStdPicker.clear();
	switch (eMotor)
	{
	case PICK_MOTOR_LOAD_TRAY: {vStdPicker.push_back(DEF_VISION_CAM_STANDARD_PICKER_LOAD_TRAY);}break;
	case PICK_MOTOR_UNLOAD_TRAY: {vStdPicker.push_back(DEF_VISION_CAM_STANDARD_PICKER_UNLOAD_TRAY);}break;
	case PICK_MOTOR_LOAD_TEST_1: {vStdPicker.push_back(m_nBoardType == eBoardDir_Forward ? DEF_VISION_CAM_STANDARD_PICKER_TESTPP1_LOAD_TRAY_FWD : DEF_VISION_CAM_STANDARD_PICKER_TESTPP1_LOAD_TRAY_BWD);}break;
	case PICK_MOTOR_UNLOAD_TEST_1: {vStdPicker.push_back(m_nBoardType == eBoardDir_Forward ? DEF_VISION_CAM_STANDARD_PICKER_TESTPP1_UNLOAD_TRAY_FWD : DEF_VISION_CAM_STANDARD_PICKER_TESTPP1_UNLOAD_TRAY_BWD);}break;	
	case PICK_MOTOR_LOAD_TEST_2: {vStdPicker.push_back(m_nBoardType == eBoardDir_Forward ? DEF_VISION_CAM_STANDARD_PICKER_TESTPP2_LOAD_TRAY_FWD : DEF_VISION_CAM_STANDARD_PICKER_TESTPP2_LOAD_TRAY_BWD);}break;
	case PICK_MOTOR_UNLOAD_TEST_2: {vStdPicker.push_back(m_nBoardType == eBoardDir_Forward ? DEF_VISION_CAM_STANDARD_PICKER_TESTPP2_UNLOAD_TRAY_FWD : DEF_VISION_CAM_STANDARD_PICKER_TESTPP2_UNLOAD_TRAY_BWD);}break;
	default:
		break;
	}
	return vStdPicker;
}

void CBbxmnpClient::MakeAllPicker(std::vector<CPoint>& vPicker)
{
	for (int y = 0; y < MAX_TRAY_PP_CNT_Y; y++) {
		for (int x = 0; x < MAX_TRAY_PP_CNT_X; x++) {
			vPicker.push_back(CPoint(x, y));
		}
	}
}


//Z Auto Teaching Flatness Offset
void CBbxmnpClient::SetFlatnessOffset(int nHand, double dHandGap[])
{
	for (int i = 0; i < eBMAxis_Picker_MAX; i++) {
		m_dHandGap[nHand][i] = dHandGap[i];
	}
}


CString CBbxmnpClient::GetPickerName(CPoint Picker)
{
	CString strPicker = _T("");
	if (Picker.y == 0) {
		strPicker += _T("[R");
	}
	else {
		strPicker += _T("[F");
	}

	CString strPickerX = _T("");
	strPickerX.Format("L%d]", Picker.x + 1);
	strPicker += strPickerX;

	return strPicker;
}