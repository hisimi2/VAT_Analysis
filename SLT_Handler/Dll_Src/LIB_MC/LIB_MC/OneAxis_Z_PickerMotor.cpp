#include "stdafx.h"
#include "OneAxis_Z_PickerMotor.h"


OneAxis_Z_PickerMotor::OneAxis_Z_PickerMotor(void)
{
	m_vOutPicker.clear();
	char szLogFileName[512] = { 0, };
	sprintf_s(szLogFileName, sizeof(szLogFileName), _T("%sMotor.log"), SZ_LOG_FILE_PATH);
	m_MtLog.SetFileName(szLogFileName);
}


OneAxis_Z_PickerMotor::~OneAxis_Z_PickerMotor(void)
{
}

void OneAxis_Z_PickerMotor::bbwmnpAxisInfo(int nSubZAxisNum, double scale, int Zr_direction/*=1*/, int pulse_dir/*=1*/, int move_dir/*=1*/)
{
	m_nSubZAxisNumPhy = nSubZAxisNum;
	g_bbxmnpClient.SetInitAxisInfo(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy),scale, pulse_dir, move_dir, m_nSubZAxisNumPhy);
}

BOOL OneAxis_Z_PickerMotor::GetAmpFaultStat()
{
	BOOL bRet = FALSE;

	const int nAxes = eBMAxis_Picker_MAX;
	CBbxmnpClient::_eMOTOR_STS_TYPE eMotorStsType = CBbxmnpClient::eMotorSts_ALM;
	BOOL bAmpFault[nAxes] = { 0 };
	
	g_bbxmnpClient.GetMotorStatus(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), eMotorStsType, bAmpFault);
	
	for (int i = 0; i < nAxes; i++) {
		if (bAmpFault[i] == TRUE) {
			bRet = TRUE;
			break;
		}
	}

	return bRet;
}

BOOL OneAxis_Z_PickerMotor::Origin()
{
	CString strLog = _T("");
	BOOL bRet = FALSE;

// 	if (m_nOri_Flag == eMotorOrgFlag_OK || m_nOri_Flag == eMotorOrgFlag_NG) {
// 		strLog.Format(_T("Axis = %d m_nOri_Flag Error"), m_nAxisNumPhy);
// 		m_MtLog.LOG(strLog);
// 		return -1;
// 	}

	if (m_cbInterLock_Org != NULL) {
		int nInterLockErr = m_cbInterLock_Org();
		if (nInterLockErr != 0) {
			strLog.Format(_T("Axis = %d orgin interlock return = %d"), m_nAxisNumPhy, nInterLockErr);
			m_MtLog.LOG(strLog);
			return nInterLockErr;
		}
	}

	bRet = g_bbxmnpClient.MultiOriginWhile(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy));

	if (bRet == TRUE)
	{
		m_nOri_Flag = 1;
		SetOriginFlag(true);

		int nPickStartNum = GetPickerStartAndEndNumber();
		int nPickEndNum = nPickStartNum + eBMAxis_Picker_MAX;

		for (int i = nPickStartNum; i < nPickEndNum; i++)
		{
			g_bbxmnpClient.SetActiaulPos(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), (_eBbxmnpAxis)i, 0.0);
			g_bbxmnpClient.SetCommandPos(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), (_eBbxmnpAxis)i, 0.0);
		}

		double dSafetyPos = 0;

		dSafetyPos = 0.0;

		m_vOutPicker.clear();
	}

	return bRet;
}

BOOL OneAxis_Z_PickerMotor::OriginWhile(int mode/*=1*/, BOOL MsgDisplay /*= FALSE*/)
{
	BOOL bRet = FALSE;
	
	bRet = g_bbxmnpClient.MultiOriginWhile(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy));

	if (bRet == TRUE)
	{
		m_nOri_Flag = 1;
		SetOriginFlag(true);
	}
	
	return bRet;
}

bool OneAxis_Z_PickerMotor::IsHomeSensorOn()
{
	BOOL bRet = FALSE;

	bRet = g_bbxmnpClient.GetHomeSensorStatus(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy));

	return bRet;

	int nChkOrg = 0;

	int nPickStartNum = GetPickerStartAndEndNumber();
	int nPickEndNum = nPickStartNum + eBMAxis_Picker_MAX;
	
	for (int i = nPickStartNum; i < nPickEndNum; i++)
	{
		if (g_bbxmnpClient.GetMotorStatus(_eBbxmnpAxis(i), CBbxmnpClient::eMotorSts_Org))
		{
			nChkOrg++;
		}
	}

	if (nChkOrg == eBMAxis_Picker_MAX)
	{
		bRet = TRUE;
	}

	return bRet;
}

bool OneAxis_Z_PickerMotor::IsHomeSensorOn(_eBbxmnpAxis eAxis)
{
	BOOL bRet = FALSE;
	bool bLockFlag = false;

	// 	if( eAxis >= eBMAxis_Input_Picker01 && eAxis <= eBMAxis_Input_Picker08)
	// 	{
	// 		bRet = g_bbxmnpClient.GetHomeSensorStatus(CBbxmnpClient::PICK_MOTOR_INPUT);
	// 	}
	// 	else
	// 	{
	// 		bRet = g_bbxmnpClient.GetHomeSensorStatus(CBbxmnpClient::PICK_MOTOR_OUTPUT);
	// 	}

	bRet = g_bbxmnpClient.GetMotorStatus(eAxis, CBbxmnpClient::eMotorSts_Org);

	return bRet;
}

BOOL OneAxis_Z_PickerMotor::GetServoStat()
{
	int nChkServo = 0;
	BOOL bRes = FALSE;
	bool bLockFlag = false;

	int nPickStartNum = GetPickerStartAndEndNumber();
	int nPickEndNum = nPickStartNum + eBMAxis_Picker_MAX;

	for (int i = nPickStartNum; i < nPickEndNum; i++) {
		if (g_bbxmnpClient.GetMotorStatus(_eBbxmnpAxis(i), CBbxmnpClient::eMotorSts_ServoOn)) {
			nChkServo++;
		}
	}

	if (nChkServo == eBMAxis_Picker_MAX) {
		bRes = TRUE;
	}


	return bRes;
}

BOOL OneAxis_Z_PickerMotor::GetServoState(_eBbxmnpAxis eAxis)
{
	BOOL bRes = FALSE;
	bool bLockFlag = false;

	bRes = g_bbxmnpClient.GetMotorStatus(eAxis, CBbxmnpClient::eMotorSts_ServoOn);

	return bRes;
}

int OneAxis_Z_PickerMotor::ServoOn()
{
	if (GetServoStat() == TRUE) {
		return 0;
	}

	g_bbxmnpClient.ServoOn(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), true);
	
	return 0;
}

int OneAxis_Z_PickerMotor::ServoOff()
{
	if (GetServoStat() == FALSE) {
		return 0;
	}

	g_bbxmnpClient.ServoOn(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), false);

	return 0;
}
int OneAxis_Z_PickerMotor::ServoOn(_eBbxmnpAxis eAxis)
{
// 	if (GetServoStat() == TRUE) {
// 		return 0;
// 	}

	g_bbxmnpClient.ServoOn(eAxis, true);

	return 0;
}

int OneAxis_Z_PickerMotor::ServoOff(_eBbxmnpAxis eAxis)
{
// 	if (GetServoStat() == FALSE) {
// 		return 0;
// 	}

	g_bbxmnpClient.ServoOn(eAxis, false);

	return 0;
}


int OneAxis_Z_PickerMotor::SetSlowStop()
{
	int nStartAxis = GetPickerStartAndEndNumber();
	int nEndAxis = nStartAxis + eBMAxis_Picker_MAX;

	for (int i = nStartAxis; i < nEndAxis; i++) {
		g_bbxmnpClient.Stop(_eBbxmnpAxis(i));
	}

	return 0;
}

int OneAxis_Z_PickerMotor::SetSlowStopEach(_eBbxmnpAxis eAxis)
{
	g_bbxmnpClient.Stop(_eBbxmnpAxis(eAxis));
	return 0;
}

int OneAxis_Z_PickerMotor::V_MoveStop()
{
	int nStartAxis = GetPickerStartAndEndNumber();
	int nEndAxis = nStartAxis + eBMAxis_Picker_MAX;

	for (int i = nStartAxis; i < nEndAxis; i++) {
		g_bbxmnpClient.Stop(_eBbxmnpAxis(i));
	}

	return 0;
}

int OneAxis_Z_PickerMotor::V_MoveStop(_eBbxmnpAxis eAxis)
{
	g_bbxmnpClient.Stop(_eBbxmnpAxis(eAxis));
	return 0;
}

int OneAxis_Z_PickerMotor::V_Move(_eJogDir eJogDir, int nAxisCnt, int* anAxes, double speed, int accel/*=0*/)
{
	CString strLog = _T("");

	if (!GetServoStat()) {
		strLog.Format(_T("Axis = %d GetServostat Error"), m_nAxisNumPhy);
		m_MtLog.LOG(strLog);
		return -1;
	}

	if (m_cbInterLock_Jog != NULL) {
		int nInterLockErr = m_cbInterLock_Jog(speed, accel, accel);
		if (nInterLockErr != ERR_NO_ERROR) {
			V_MoveStop();
			strLog.Format(_T("Axis = %d Interlock Error"), m_nAxisNumPhy);
			m_MtLog.LOG(strLog);
			return nInterLockErr;
		}
 	}

	g_bbxmnpClient.JogMove(eJogDir, nAxisCnt, anAxes, speed, accel);

	return 0;
}

int OneAxis_Z_PickerMotor::V_Move(_eJogDir eJogDir, _eBbxmnpAxis eAxis, double speed, int accel/*=0*/)
{
	CString strLog = _T("");

	if (!GetServoStat()) {
		strLog.Format(_T("Axis = %d GetServostat Error"), m_nAxisNumPhy);
		m_MtLog.LOG(strLog);
		return -1;
	}

	if (m_cbInterLock_Jog != NULL) {
		int nInterLockErr = m_cbInterLock_Jog(speed, accel, accel);
		if (nInterLockErr != ERR_NO_ERROR) {
			V_MoveStop();
			strLog.Format(_T("Axis = %d Interlock Error"), m_nAxisNumPhy);
			m_MtLog.LOG(strLog);
			return nInterLockErr;
		}
	}

	g_bbxmnpClient.JogMove(eJogDir, eAxis, speed, accel);

	return 0;
}

/**=================================================================================================================
*
* RETURN : 0 : 정상
*                    0 이외 : Error

Picker를 한번에 제어 할 수 있는 함수 임
================================================================================================================= */
int OneAxis_Z_PickerMotor::Move(std::vector<CPoint> vPicker, double a_dPos_um, double a_dSpeed_umps, int a_nAccel, int a_nDccel, int nBoardType, BOOL bNoUseOffset /*= FALSE*/ /*BOOL bVatOffsetUse = FALSE*/)
{
	int nRet = 0;

// 	if (m_nOri_Flag != eMotorOrgFlag_OK) {	// 원점이 안되어 있으면 리턴한다.
// 		return ERR_MOT_INTERLOCK_IS_NOT_ORG;
// 	}
// 	if (!GetServoStat()) {
// 		return ERR_MOT_INTERLOCK_IS_NOT_SERVO_ON;
// 	}
// 
 	if (m_cbInterLock_Move != NULL) {
 		int nInterLockErr = m_cbInterLock_Move(a_dPos_um, a_dSpeed_umps, a_nAccel, a_nDccel);
 		if (nInterLockErr != 0) {
 			return nInterLockErr;
 		}
 	}
 
 	if (a_dPos_um > m_dSwLimit_Posi) {
 		return ERR_MOT_INTERLOCK_IS_POSI_SW_LMT;
 	}
 
 	if (a_dPos_um < m_dSwLimit_Nega) {
 		return ERR_MOT_INTERLOCK_IS_NEGA_SW_LMT;
 	}
 
 
 	if (a_dSpeed_umps <= 0)
 		a_dSpeed_umps = m_dCmdVelocity;
 
 	if (a_nAccel <= 0)
 		a_nAccel = m_nCmdAcc;
 
 	if (a_nDccel <= 0)
 		a_nDccel = m_nCmdDcc;
 
 	if (a_dSpeed_umps == 1)
 	{
 		a_dSpeed_umps = m_dCmdVelocity;
 		a_nAccel = 100;
 		a_nDccel = 100;
 	}
 
 	if (a_nAccel < 10)
 		a_nAccel = 50;
 
 	if (a_nDccel < 10)
 		a_nDccel = 50;

	m_dTarget_pos_um = a_dPos_um;

	double dTargetPos = (double)m_nDir_Move * a_dPos_um / m_dScaleFactor;

//  	if (dTargetPos > m_dPosLimitPulse)
//  	{
//  		return ERR_MOT_INTERLOCK_IS_POSI_SW_LMT;
//  	}
//  	if (dTargetPos < m_dNegLimitPulse)
//  	{
//  		return ERR_MOT_INTERLOCK_IS_NEGA_SW_LMT;
//  	}

	nRet = g_bbxmnpClient.Move(eMoveType_ABS, static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), vPicker, dTargetPos, a_dSpeed_umps, a_nAccel, a_nDccel, nBoardType, bNoUseOffset/*bVatOffsetUse*/);

	return nRet;
}

int OneAxis_Z_PickerMotor::Move(_eBbxmnpAxis eAxis, double a_dPos_um, double a_dSpeed_umps, int a_nAccel, int a_nDccel, int nBoardType, BOOL bNoUseOffset /*= FALSE*/)
{
	int nRet = 0;

	// 	if (m_nOri_Flag != eMotorOrgFlag_OK) {	// 원점이 안되어 있으면 리턴한다.
	// 		return ERR_MOT_INTERLOCK_IS_NOT_ORG;
	// 	}
	// 	if (!GetServoStat()) {
	// 		return ERR_MOT_INTERLOCK_IS_NOT_SERVO_ON;
	// 	}
	// 
	if (m_cbInterLock_Move != NULL) {
		int nInterLockErr = m_cbInterLock_Move(a_dPos_um, a_dSpeed_umps, a_nAccel, a_nDccel);
		if (nInterLockErr != 0) {
			return nInterLockErr;
		}
	}

	if (a_dPos_um > m_dSwLimit_Posi) {
		return ERR_MOT_INTERLOCK_IS_POSI_SW_LMT;
	}

	if (a_dPos_um < m_dSwLimit_Nega) {
		return ERR_MOT_INTERLOCK_IS_NEGA_SW_LMT;
	}


	if (a_dSpeed_umps <= 0)
		a_dSpeed_umps = m_dCmdVelocity;

	if (a_nAccel <= 0)
		a_nAccel = m_nCmdAcc;

	if (a_nDccel <= 0)
		a_nDccel = m_nCmdDcc;

	if (a_dSpeed_umps == 1)
	{
		a_dSpeed_umps = m_dCmdVelocity;
		a_nAccel = 100;
		a_nDccel = 100;
	}

	if (a_nAccel < 10)
		a_nAccel = 50;

	if (a_nDccel < 10)
		a_nDccel = 50;

	m_dTarget_pos_um = a_dPos_um;

	double dTargetPos = (double)m_nDir_Move * a_dPos_um / m_dScaleFactor;

	//  	if (dTargetPos > m_dPosLimitPulse)
	//  	{
	//  		return ERR_MOT_INTERLOCK_IS_POSI_SW_LMT;
	//  	}
	//  	if (dTargetPos < m_dNegLimitPulse)
	//  	{
	//  		return ERR_MOT_INTERLOCK_IS_NEGA_SW_LMT;
	//  	}

	nRet = g_bbxmnpClient.Move(eMoveType_ABS, eAxis, dTargetPos, a_dSpeed_umps, a_nAccel, a_nDccel, nBoardType, bNoUseOffset);
	return nRet;
}


int OneAxis_Z_PickerMotor::MoveWhile(double a_dPos_um, double a_dSpeed_umps, int a_nAccel, int a_nDccel, BOOL No_Check_safety /*= FALSE*/)
{
	int nRet = 0;

	if (m_nOri_Flag != eMotorOrgFlag_OK) {	// 원점이 안되어 있으면 리턴한다.
		return ERR_MOT_INTERLOCK_IS_NOT_ORG;
	}
	if (!GetServoStat()) {
		return ERR_MOT_INTERLOCK_IS_NOT_SERVO_ON;
	}

	if (m_cbInterLock_Move != NULL) {
		int nInterLockErr = m_cbInterLock_Move(a_dPos_um, a_dSpeed_umps, a_nAccel, a_nDccel);
		if (nInterLockErr != 0) {
			return nInterLockErr;
		}
	}

	if (a_dPos_um > m_dSwLimit_Posi) {
		return ERR_MOT_INTERLOCK_IS_POSI_SW_LMT;
	}
	if (a_dPos_um < m_dSwLimit_Nega) {
		return ERR_MOT_INTERLOCK_IS_NEGA_SW_LMT;
	}


	if (a_dSpeed_umps <= 0)
		a_dSpeed_umps = m_dCmdVelocity;

	if (a_nAccel <= 0)
		a_nAccel = m_nCmdAcc;

	if (a_nDccel <= 0)
		a_nDccel = m_nCmdDcc;

	if (a_dSpeed_umps == 1)
	{
		a_dSpeed_umps = m_dCmdVelocity;
		a_nAccel = 100;
		a_nDccel = 100;
	}

	if (a_nAccel < 10)
		a_nAccel = 50;

	if (a_nDccel < 10)
		a_nDccel = 50;

	double dTargetPos = (double)m_nDir_Move * a_dPos_um / m_dScaleFactor;

	if (dTargetPos > m_dPosLimitPulse)
	{
		return ERR_MOT_INTERLOCK_IS_POSI_SW_LMT;
	}
	if (dTargetPos < m_dNegLimitPulse)
	{
		return ERR_MOT_INTERLOCK_IS_NEGA_SW_LMT;
	}


	nRet = g_bbxmnpClient.MoveWhile(eMoveType_ABS, static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), dTargetPos, a_dSpeed_umps, a_nAccel, a_nDccel);
	
	return nRet;
}

int OneAxis_Z_PickerMotor::MoveWhile(std::vector<CPoint> vPicker, double a_dPos_um, double a_dSpeed_umps, int a_nAccel, int a_nDccel,   BOOL bNoUseOffset /*= FALSE*/)
{
	int nRet = 0;

	if (m_nOri_Flag != eMotorOrgFlag_OK) {	// 원점이 안되어 있으면 리턴한다.
		return ERR_MOT_INTERLOCK_IS_NOT_ORG;
	}
	if (!GetServoStat()) {
		return ERR_MOT_INTERLOCK_IS_NOT_SERVO_ON;
	}

	if (m_cbInterLock_Move != NULL) {
		int nInterLockErr = m_cbInterLock_Move(a_dPos_um, a_dSpeed_umps, a_nAccel, a_nDccel);
		if (nInterLockErr != 0) {
			return nInterLockErr;
		}
	}

	if (a_dPos_um > m_dSwLimit_Posi) {
		return ERR_MOT_INTERLOCK_IS_POSI_SW_LMT;
	}
	if (a_dPos_um < m_dSwLimit_Nega) {
		return ERR_MOT_INTERLOCK_IS_NEGA_SW_LMT;
	}


	if (a_dSpeed_umps <= 0)
		a_dSpeed_umps = m_dCmdVelocity;

	if (a_nAccel <= 0)
		a_nAccel = m_nCmdAcc;

	if (a_nDccel <= 0)
		a_nDccel = m_nCmdDcc;

	if (a_dSpeed_umps == 1)
	{
		a_dSpeed_umps = m_dCmdVelocity;
		a_nAccel = 100;
		a_nDccel = 100;
	}

	if (a_nAccel < 10)
		a_nAccel = 50;

	if (a_nDccel < 10)
		a_nDccel = 50;

	double dTargetPos = (double)m_nDir_Move * a_dPos_um / m_dScaleFactor;

	if (dTargetPos > m_dPosLimitPulse)
	{
		return ERR_MOT_INTERLOCK_IS_POSI_SW_LMT;
	}
	if (dTargetPos < m_dNegLimitPulse)
	{
		return ERR_MOT_INTERLOCK_IS_NEGA_SW_LMT;
	}
	
	nRet = g_bbxmnpClient.MoveWhile(eMoveType_ABS, static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), vPicker, dTargetPos, a_dSpeed_umps, a_nAccel, a_nDccel, bNoUseOffset);

	return nRet;
}

int OneAxis_Z_PickerMotor::MoveWhile(_eBbxmnpAxis eAxis, double a_dPos_um, double a_dSpeed_umps, int a_nAccel, int a_nDccel, BOOL No_Check_safety /*= FALSE*/)
{
	int nRet = 0;

	if (m_nOri_Flag != eMotorOrgFlag_OK) {	// 원점이 안되어 있으면 리턴한다.
		return ERR_MOT_INTERLOCK_IS_NOT_ORG;
	}
	if (!GetServoStat()) {
		return ERR_MOT_INTERLOCK_IS_NOT_SERVO_ON;
	}

	if (m_cbInterLock_Move != NULL) {
		int nInterLockErr = m_cbInterLock_Move(a_dPos_um, a_dSpeed_umps, a_nAccel, a_nDccel);
		if (nInterLockErr != 0) {
			return nInterLockErr;
		}
	}

	if (a_dPos_um > m_dSwLimit_Posi) {
		return ERR_MOT_INTERLOCK_IS_POSI_SW_LMT;
	}
	if (a_dPos_um < m_dSwLimit_Nega) {
		return ERR_MOT_INTERLOCK_IS_NEGA_SW_LMT;
	}


	if (a_dSpeed_umps <= 0)
		a_dSpeed_umps = m_dCmdVelocity;

	if (a_nAccel <= 0)
		a_nAccel = m_nCmdAcc;

	if (a_nDccel <= 0)
		a_nDccel = m_nCmdDcc;

	if (a_dSpeed_umps == 1)
	{
		a_dSpeed_umps = m_dCmdVelocity;
		a_nAccel = 100;
		a_nDccel = 100;
	}

	if (a_nAccel < 10)
		a_nAccel = 50;

	if (a_nDccel < 10)
		a_nDccel = 50;

	double dTargetPos = (double)m_nDir_Move * a_dPos_um / m_dScaleFactor;

	if (dTargetPos > m_dPosLimitPulse)
	{
		return ERR_MOT_INTERLOCK_IS_POSI_SW_LMT;
	}
	if (dTargetPos < m_dNegLimitPulse)
	{
		return ERR_MOT_INTERLOCK_IS_NEGA_SW_LMT;
	}

	nRet = g_bbxmnpClient.MoveWhile(eMoveType_ABS, eAxis, dTargetPos, a_dSpeed_umps, a_nAccel, a_nDccel);

	return nRet;
}

int OneAxis_Z_PickerMotor::R_MoveWhileForChkOrign(double pos, double speed/*=0*/, int accel/*=0*/)
{
	int nRet = 0;

	nRet = g_bbxmnpClient.MoveWhile(eMoveType_INC, static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), pos, speed, accel, accel, TRUE);

	return nRet;

}

double OneAxis_Z_PickerMotor::LimitValueCheck(BOOL bPosi)
{
	if (bPosi == TRUE)
	{								// Positive 값 리턴 요구
		return m_dSwLimit_Posi;
	}
	else
	{
		return m_dSwLimit_Nega;
	}
}

int OneAxis_Z_PickerMotor::MotionDone()
{
	int nRet = FALSE;
	bool bLockFlag = false;

	const int nAxes = eBMAxis_Picker_MAX;
	CBbxmnpClient::_eMOTION_STS_TYPE eType = CBbxmnpClient::eMotionSts_DRV;
	BOOL bDrvSts[nAxes] = { 0 };
	
	g_bbxmnpClient.GetMotionStatus(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), eType, bDrvSts);

	for (int i = 0; i < nAxes; i++) {
		if (bDrvSts[i] == TRUE) {
			nRet = TRUE;
			break;
		}
	}

	return nRet;
}

int OneAxis_Z_PickerMotor::R_MoveWhileForChkOrign(std::vector<CPoint> vPicker, double pos, double speed/*=0*/, int accel/*=0*/)
{
	int nRet = 0;

	nRet = g_bbxmnpClient.MoveWhile(eMoveType_INC, static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), vPicker, pos, speed, accel, accel, TRUE);

	return nRet;
}
int OneAxis_Z_PickerMotor::R_MoveWhile(double pos, double speed/*=0*/, int accel/*=0*/)
{
	int nRet = 0;

	nRet = g_bbxmnpClient.MoveWhile(eMoveType_INC, static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), pos, speed, accel, accel);

	return nRet;
}

int OneAxis_Z_PickerMotor::R_MoveWhile(std::vector<CPoint> vPicker, double pos, double speed/*=0*/, int accel/*=0*/)
{
	int nRet = 0;

	nRet = g_bbxmnpClient.MoveWhile(eMoveType_INC, static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), vPicker, pos, speed, accel, accel);

	return nRet;
}

void OneAxis_Z_PickerMotor::SetInitMotorParam(double r_vel, int r_acc, int r_dcc, double o_vel, int o_acc, double pos, double neg, double scalefactor)
{
	g_bbxmnpClient.SetInitMotorParam(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), r_vel, r_acc, r_dcc, o_vel, o_acc, pos, neg, scalefactor);

}

BOOL OneAxis_Z_PickerMotor::GetOriginFlag()
{
	BOOL bRet = FALSE;

	bRet = g_bbxmnpClient.GetOriginFlag(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy));

	return bRet;
}

void OneAxis_Z_PickerMotor::SetOriginFlag(bool bFlag)
{
	g_bbxmnpClient.SetOriginFlag(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), bFlag);
}

BOOL OneAxis_Z_PickerMotor::IsOrgCompleted(ERR_CODE *pError)
{
 	BOOL bRet = FALSE;
 	
 	bRet = g_bbxmnpClient.GetOriginFlag(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy));
	if (pError != nullptr) {
		*pError = !bRet;
	}
	return bRet;
}

double OneAxis_Z_PickerMotor::GetCommand(CBbxmnpClient::_ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis)
{
	return g_bbxmnpClient.GetCommandPos(eMotor, eAxis);
}

void OneAxis_Z_PickerMotor::SetPickerLastPosition(int nPickerNo, double dLastPos)
{	
	g_bbxmnpClient.SetLastPos(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), (_eBbxmnpAxis)nPickerNo, dLastPos);
}

double OneAxis_Z_PickerMotor::GetPickerLastPosition(int nPickerNo)
{
	double dLastPos = 0.0;
	PICK_OFFSET stOffset;

	dLastPos = g_bbxmnpClient.GetLastPos(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), (_eBbxmnpAxis)nPickerNo);
	stOffset = g_bbxmnpClient.GetPickOffset(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy));

	dLastPos += stOffset.dFlatOffset[nPickerNo];
	return dLastPos;
}

double OneAxis_Z_PickerMotor::GetCurrentPos(double &dPos,_eBbxmnpAxis eAxis)
{
	dPos = GetActual(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), _eBbxmnpAxis(eAxis));
	return dPos;
}

int OneAxis_Z_PickerMotor::GetCurrentPos(double &dPos)
{
	int nStartAxis = GetPickerStartAndEndNumber();
	dPos = GetActual(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), _eBbxmnpAxis(nStartAxis/*+1*/));
	return dPos;
}

int OneAxis_Z_PickerMotor::GetCurrenPosAvg(double &dPos)
{
// 	int nStartAxis = GetPickerStartAndEndNumber();
// 
// 	double dAddPos = 0.0;
// 	for (int i = 0; i < eBMAxis_Picker_MAX; i++) {
// 		dAddPos += GetActual(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), _eBbxmnpAxis(nStartAxis + 1));
// 	}
// 	//Average Pos
// 	dPos = dAddPos / eBMAxis_Picker_MAX;
// 	return dPos;


	//Offset 0 Picker GetCurPos
	int nStartAxis = GetPickerStartAndEndNumber();
	int nZeroOffsetPkr = nStartAxis + 1;
	double dFlatOffset = 0.0;
	
	for (int i = 0; i < eBMAxis_Picker_MAX; i++) {
		dFlatOffset = g_bbxmnpClient.GetPickerOffset(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), _eBbxmnpAxis(nStartAxis));
		if (dFlatOffset == 0.0) {
			nZeroOffsetPkr += i;
			break;
		}
	}

 	dPos = GetActual(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), _eBbxmnpAxis(nZeroOffsetPkr));

	return ERR_NO_ERROR;
}

double OneAxis_Z_PickerMotor::GetActual(CBbxmnpClient::_ePICK_MOTOR eMotor, _eBbxmnpAxis eAxis)
{
	return g_bbxmnpClient.GetActualPos(eMotor, eAxis);
}

int OneAxis_Z_PickerMotor::AlarmReset()
{	
	return g_bbxmnpClient.Reset(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy));
}

int OneAxis_Z_PickerMotor::AxisDone()
{
	BOOL bRet = FALSE;
	bool bLockFlag = false;

	const int nAxes = eBMAxis_Picker_MAX;
	CBbxmnpClient::_eMOTION_STS_TYPE eType = CBbxmnpClient::eMotionSts_DRV;
	BOOL bDrvSts[eBMAxis_Picker_MAX] = { 0, };
	memset(bDrvSts, 0, sizeof(bDrvSts));
	g_bbxmnpClient.GetMotionStatus(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), eType, bDrvSts);

	for (int i = 0; i < eBMAxis_Picker_MAX; i++) {
		if (bDrvSts[i] == TRUE) {
			bRet = TRUE;
			break;
		}
	}
	return bRet;
}

int OneAxis_Z_PickerMotor::AxisDone(std::vector<CPoint>& vPicker, std::vector<CPoint>& vErrPicker)
{
	BOOL bRet = FALSE;
	bool bLockFlag = false;

	const int nAxes = eBMAxis_Picker_MAX;
	CBbxmnpClient::_eMOTION_STS_TYPE eType = CBbxmnpClient::eMotionSts_DRV;
	BOOL bDrvSts[eBMAxis_Picker_MAX] = { 0, };
	memset(bDrvSts, 0, sizeof(bDrvSts));
	g_bbxmnpClient.GetMotionStatus(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), eType, bDrvSts);
	vErrPicker.clear();

	for (int i = 0; i < eBMAxis_Picker_MAX; i++) {
		if (bDrvSts[i] == TRUE) {
			bRet = TRUE;
			break;
		}
	}

	if (bRet == FALSE) {
		std::vector<int> vPickerAxisNum;
		BOOL bMoveTargetPos = FALSE;
		g_bbxmnpClient.MakeBMAxisPicker(vPickerAxisNum, static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy));
		int nPickerIdx = 0;

	
		for (auto i : vPickerAxisNum)
		{			
			double dEncoder_um = 0.0;
			GetCurrentPos(dEncoder_um, _eBbxmnpAxis(i));
			if (MOTOR_INPOSITION <= abs(m_dTarget_pos_um - dEncoder_um)) {
				//임시 
//				bMoveTargetPos = TRUE;

				CString strLog = _T("");
				strLog.Format(_T("[Axis No =%d]Z Axis Error "), m_nAxisNumPhy);
				m_MtLog.LOG(strLog);
				vErrPicker.push_back(CPoint(nPickerIdx % MAX_PICKER_X_CNT, nPickerIdx / MAX_PICKER_X_CNT));

				//비교 후, index 동일하면 vector에 넣지 않음.
				BOOL bChk = FALSE;
				if (m_vOutPicker.size() != 0) {
					for (int nIdx = 0; nIdx < m_vOutPicker.size(); nIdx++) {
						if (m_vOutPicker[nIdx] == i) {
							bChk = TRUE;
						}
					}
				}

				if (bChk != TRUE) {
					m_vOutPicker.push_back(i);
				}

				//[9/25/2023 dohyeong.kim]
				//Set Command Position을 Target Position으로 Reset
				//Set Command 하지 않을 시, 보정 값이 Command Pos로 증가되는 현상이 발생
				g_bbxmnpClient.SetCommandPos(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), _eBbxmnpAxis(i), m_dTarget_pos_um);

				double dCommandPos = g_bbxmnpClient.GetCommandPos(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), _eBbxmnpAxis(i));
				double dActualPos = g_bbxmnpClient.GetActualPos(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), _eBbxmnpAxis(i));
				double dGap = dCommandPos - dActualPos;
				strLog.Format(_T("[Axis No =%d] Target Pos = %.3f "), m_nAxisNumPhy, m_dTarget_pos_um);
				m_MtLog.LOG(strLog);
				strLog.Format(_T("[Axis No =%d][CommandPos = %.3f][ActualPos = %.3f] Gap =%.3f, Bbxaxis : %d"), m_nAxisNumPhy, dCommandPos, dActualPos, dGap, i);
				m_MtLog.LOG(strLog);

				double dTargetPos = dGap / m_dScaleFactor;
				if (dCommandPos != dActualPos) {
					g_bbxmnpClient.Move(eMoveType_INC, _eBbxmnpAxis(i), dTargetPos, 1000, 50, 50, eBoardDir_Backward, TRUE);
					bRet = TRUE;
 				}
			}
			nPickerIdx++;
		}

		std::vector<CPoint> vOutPicker;
		vOutPicker.clear();
 		if (vErrPicker.size() == 0 && m_vOutPicker.size() != 0)
 		{
 			for (int i = 0; i < m_vOutPicker.size(); i++) {
 				double dActualPos = g_bbxmnpClient.GetActualPos(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), _eBbxmnpAxis(m_vOutPicker[i]));

 				g_bbxmnpClient.SetCommandPos(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), _eBbxmnpAxis(m_vOutPicker[i]), dActualPos);

				CString strLog = _T("");			
				strLog.Format(_T("[Axis No =%d][Bbxaxis : %d][ActualPos = %.3f]"), m_nAxisNumPhy, m_vOutPicker[i], dActualPos);
				m_MtLog.LOG(strLog);
				int nPicker_start = g_bbxmnpClient.ZAxisStartPicker(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy));
				
				int nOutPicker = abs(nPicker_start - m_vOutPicker[i]);

				vOutPicker.push_back(CPoint(nOutPicker % MAX_PICKER_X_CNT, nOutPicker / MAX_PICKER_X_CNT));
// 				g_bbxmnpClient.SetActiaulPos(static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), _eBbxmnpAxis(i), dActualPos);
 			}
			vErrPicker = vOutPicker;
 			m_vOutPicker.clear();
 		}


// 		if (bMoveTargetPos == TRUE)
// 		{
// 			double dTarget_pos_um = (double)m_nDir_Move * m_dTarget_pos_um / m_dScaleFactor;
// 			g_bbxmnpClient.Move(eMoveType_ABS, static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy), vPicker, dTarget_pos_um, 100, 30, 30, TRUE);
// 			bRet = TRUE;
// 		}
	}

	return bRet;
}


double OneAxis_Z_PickerMotor::GetAllLastPos(int nPickerNo)
{
	return g_bbxmnpClient.m_All_Last_Pos[static_cast<CBbxmnpClient::_ePICK_MOTOR>(m_nSubZAxisNumPhy)][nPickerNo];
}

int OneAxis_Z_PickerMotor::GetPickerStartAndEndNumber()
{
	int nStartPicNum = 0;
// 	switch (m_nAxisNumPhy)
// 	{
		 if(eAXIS_TRAY_PP_1_Z == m_nAxisNumPhy)
		 {
			 nStartPicNum = eBMAxis_LOAD_TRAY_Picker01;
		 }
		 else if(eAXIS_TRAY_PP_2_Z == m_nAxisNumPhy)
		 {
			 nStartPicNum = eBMAxis_UNLOAD_TRAY_Picker01;
		 }
		 else if(eAXIS_TEST_PP_1_Z == m_nAxisNumPhy)
		 {
			 nStartPicNum = eBMAxis_LOAD_TESTPP_1_Picker01;
		 }//break;
		 else if (eAXIS_TEST_PP_1_Z2 == m_nAxisNumPhy)
		 {
			 nStartPicNum = eBMAxis_UNLOAD_TESTPP_1_Picker01;
		 }//break;
		 else if (eAXIS_TEST_PP_2_Z == m_nAxisNumPhy)
		 {
			 nStartPicNum = eBMAxis_LOAD_TESTPP_2_Picker01;
		 }
		 else if (eAXIS_TEST_PP_2_Z2 == m_nAxisNumPhy)
		 {
			 nStartPicNum = eBMAxis_UNLOAD_TESTPP_2_Picker01;
		 }
//	}

	return nStartPicNum;
}

void OneAxis_Z_PickerMotor::ClearOrgFlag()
{
	SetOriginFlag(false);
}

double OneAxis_Z_PickerMotor::GetSpeed()
{
	if (!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable) {
		return 1;
	}

	double dSpeed = m_dCmdVelocity;

	if (dSpeed < 10.0)
		dSpeed = 10.0;

	return dSpeed;
}

int OneAxis_Z_PickerMotor::GetAcc()
{
	return m_nCmdAcc;
}

int OneAxis_Z_PickerMotor::GetDcc()
{
	return m_nCmdDcc;
}