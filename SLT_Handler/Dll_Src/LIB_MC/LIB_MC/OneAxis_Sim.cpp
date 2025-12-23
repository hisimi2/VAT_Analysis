#include "StdAfx.h"
#include "OneAxis_Sim.h"
#include "afxmt.h"
#include "DEF_ERROR_CODE.h"

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
OneAxis_Sim::OneAxis_Sim(void)
{
	m_dWork_Velocity = 1000.0;
	m_dWork_Velocity_Ori = 1000.0;
	m_nWork_Acc = 0;
	m_nWork_Acc_Ori=0;
	m_nWork_Dcc=0;
	m_dMove_Func_Call_Time_msec = (double)::GetTickCount64();
	m_dOrg_Func_Call_Time_msec = (double)::GetTickCount64();
	m_nMotionSimulState = 0;
	m_nMotorSimulStep = 0;
	m_bSopThread = FALSE;
	m_nOri_Flag  = eMotorOrgFlag_ING;
	AfxBeginThread(OneAxis_Sim::ThreadProc, this);

	// LOG File Name
	char szLogFileName[ 512 ] = { 0, };
	sprintf_s(szLogFileName, sizeof(szLogFileName), _T("%sMotor.log"), SZ_LOG_FILE_PATH);
	m_MtLog.SetFileName(szLogFileName);
	m_bApplyBacklash = false;
	m_dBackLashOffset = 0.0;
	m_dPrevMovePos = 0.0;
	m_dPrevBackLashOffset = 0.0;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
OneAxis_Sim::~OneAxis_Sim(void)
{
	m_bSopThread = TRUE;
	::WaitForSingleObject(m_evtTerminatedThread, 100);
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
ERR_CODE OneAxis_Sim::InitBoard()
{
	return ERR_SUCCESS;
}

ERR_CODE OneAxis_Sim::SplineMove(std::vector<_stSpline> SplineMoveCmd, int nAxisNumPhy1, int nAxisNumPhy2)
{
	return ERR_SUCCESS;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : move_pos
             speed
			 accel
			 dccel
  RETURN   : 
  COMMENTS : 
==============================================================================*/



ERR_CODE OneAxis_Sim::Move(double move_pos, double speed/*=0*/, int accel/*=0*/, int dccel/* = 0*/, BOOL bApplyBacklash, _eMotorIntertlock nInterlockType /*= _eMotorIntertlock::eMotorInterlock_Operation*/)
{
	if( m_cbInterLock_Move != NULL && nInterlockType != _eMotorIntertlock::eMotorInterlock_All_Ignore) {
		ERR_CODE nInterLockErr = m_cbInterLock_Move( move_pos, speed, accel, dccel);
		if( nInterLockErr != ERR_SUCCESS ) {
			return nInterLockErr;
		}
	}

	return NoInterlock_Move( move_pos, speed, accel, dccel, bApplyBacklash);	// 20190405 ihc. backlash test 여부 전달
}

ERR_CODE OneAxis_Sim::NoInterlock_Move( double _pos_um, double _spd_umps, int _acc, int _dec, BOOL bApplyBacklash,BOOL bAutoRunning/* = FALSE*/, _eApplyBacklashCondition eApplyBacklashCondition/* =_eApplyBacklashCondition::eApplyBacklashCondition_Normal*/, _eMotorIntertlock nInterlockType /*= _eMotorIntertlock::eMotorInterlock_Operation*/)
{
	//ERR_CODE nErr = CheckMotorUsable();
	//if( nErr != ERR_SUCCESS ) {
	//	return nErr;
	//}

	// --- 20190405 ihc. Copy from ms100 + backlash test여부 조건검사 추가
	// 20190405 ihc. backlash 적용 
	if ( eApplyBacklashCondition == _eApplyBacklashCondition::eApplyBacklashCondition_Normal)
	{
		if (bApplyBacklash == true)
		{
			double dBacklashOffset = 0.0;
			if (m_nAxisNumPhy == eAXIS_TEST_PP_1_X || m_nAxisNumPhy == eAXIS_TEST_PP_1_X2)
				dBacklashOffset = GetBacklashOffset_TestPP_X1(_pos_um, _spd_umps);
			else
				dBacklashOffset = GetBacklashOffset(_pos_um, _spd_umps);

			CString strBacklashLog;	
			strBacklashLog.Format(_T("%s[PhyNo %d] [BACKLASH APPLY] MovePos: %.3fmm, Offset: %.3fmm"), m_szAxisName, m_nAxisNumPhy, _pos_um / 1000, dBacklashOffset / 1000);
			m_MtLog.LOG(strBacklashLog);
			_pos_um += dBacklashOffset;
		}
	}
	else if (eApplyBacklashCondition == _eApplyBacklashCondition::eApplyBacklashCondition_BacklashTest)
	{
		double dBacklashOffset = 0.0;
		if (m_nAxisNumPhy == eAXIS_TEST_PP_1_X || m_nAxisNumPhy == eAXIS_TEST_PP_1_X2)
			dBacklashOffset = GetBacklashOffset_TestPP_X1(_pos_um, _spd_umps);
		else
			dBacklashOffset = GetBacklashOffset(_pos_um, _spd_umps);

		CString strBacklashLog;
		strBacklashLog.Format(_T("%s[PhyNo %d] [BACKLASH APPLY] MovePos: %.3fmm, Offset: %.3fmm"), m_szAxisName, m_nAxisNumPhy, _pos_um / 1000, dBacklashOffset / 1000);
		m_MtLog.LOG(strBacklashLog);
		_pos_um += dBacklashOffset;
	}

	double dTargetPos_pulse = (double)m_nDir_Move * _pos_um / (m_dScaleFactor + m_dScaleFactorOffset);	//pulse
	double dMovePos = _pos_um / 1000;
	// --- 20190405 ihc
	m_SimMotorState.dCurPos = _pos_um;	// 20191120 ihc. Sim GetCurrentPos()에서 m_SimMotorState.dCurPos가져옴.
	m_dTarget_pos_um = _pos_um;
	m_dWork_Velocity = _spd_umps;
	m_nWork_Acc = _acc;
	m_nWork_Dcc = _dec;

	if (bAutoRunning == TRUE)
	{
		m_dLast_pos = m_dTarget_pos_um;
	}

	m_SimMotorState.bInpos = FALSE;
	m_SimMotorState.bDone = FALSE;
	m_SimMotorState.bSequence = TRUE;
	m_SimMotorState.bMotion = TRUE;



	ChangeSimulStatus( eMoSimSt_Move );

	return ERR_SUCCESS;
}

int OneAxis_Sim::Move(std::vector<CPoint> vPicker, double move_pos, double speed, int accel, int dccel, int nBoardType, BOOL bNoUseOffset)
{
	if (m_cbInterLock_Move != NULL) {
		ERR_CODE nInterLockErr = m_cbInterLock_Move(move_pos, speed, accel, dccel);
		if (nInterLockErr != ERR_SUCCESS) {
			return nInterLockErr;
		}
	}

	return NoInterlock_Move(move_pos, speed, accel, dccel);	// 20190405 ihc. backlash test 여부 전달
}

int	OneAxis_Sim::Move(_eBbxmnpAxis eAxis, double move_pos, double speed, int accel, int dccel, int nBoardType, BOOL bNoUseOffset/* = FALSE*/)
{
	if (m_cbInterLock_Move != NULL) {
		ERR_CODE nInterLockErr = m_cbInterLock_Move(move_pos, speed, accel, dccel);
		if (nInterLockErr != ERR_SUCCESS) {
			return nInterLockErr;
		}
	}

	return NoInterlock_Move(move_pos, speed, accel, dccel);	// 20190405 ihc. backlash test 여부 전달
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 속도에 의해 방향이 결정 됨.
==============================================================================*/
ERR_CODE OneAxis_Sim::V_Move(double a_dSpeed, int a_nAccel)
{
	ERR_CODE nErr = CheckMotorUsableForVMove();
	if( nErr != ERR_SUCCESS ){
		return nErr;
	}

	if(a_nAccel==0){
		a_nAccel = m_nCmdAcc;
	}

	// 20190405 ihc. copy from ms100
	double dSpeed_pps = 0.0;
	dSpeed_pps = (double)m_nDir_Move * a_dSpeed / (m_dScaleFactor + m_dScaleFactorOffset);
	SaveMotorMoveLog(dSpeed_pps, 0.0, 2);
	// --- 20190405 ihc.
	
	m_dWork_Velocity = a_dSpeed;
	m_nWork_Acc = a_nAccel;
	m_nWork_Dcc = a_nAccel;

	ChangeSimulStatus( eMoSimSt_VMove );

	return ERR_SUCCESS;
}


ERR_CODE OneAxis_Sim::JogMove( int nDir, double dSpeed_umps, int nAccDec, BOOL bPcdBacklashMode/*=FALSE*/ )
{
	if( !g_pMotion_Cont_Imp->GetInitBoardStatus() ){
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}
	if( IsAmpFault() != 0 ){
		return ERR_MOTOR_CMN_AMP_FAULT;
	}
	if( m_bMotorDisable ){
		return ERR_MOTOR_CMN_DISABLED;
	}

	double move_pos = 0.0;

	if( nDir > 0 ) move_pos = m_dSwLimit_Posi_um;
	else move_pos = m_dSwLimit_Nega_um;

	if( m_cbInterLock_Move != NULL ){
		ERR_CODE nInterLockErr = m_cbInterLock_Move(move_pos, dSpeed_umps, nAccDec, nAccDec);
		if( nInterLockErr != ERR_SUCCESS ){
			return nInterLockErr;
		}
	}
	// --- 20190405 ihc. copy from ms100
	double dMovePos_um = 0.0;
	if (nDir > 0) dMovePos_um = m_dSwLimit_Posi_um;
	else dMovePos_um = m_dSwLimit_Nega_um;
	double dTargetPos_pulse = (dMovePos_um / (m_dScaleFactor + m_dScaleFactorOffset));
	// --- 20190405 ihc. 

	m_dTarget_pos_um = move_pos;
	m_dWork_Velocity = dSpeed_umps;
	m_nWork_Acc = nAccDec;
	m_nWork_Dcc = nAccDec;

	m_SimMotorState.bInpos = FALSE;
	m_SimMotorState.bDone  = FALSE;
	m_SimMotorState.bSequence = TRUE;
	m_SimMotorState.bMotion = TRUE;

	ChangeSimulStatus( eMoSimSt_Move );

	return ERR_SUCCESS;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
ERR_CODE OneAxis_Sim::V_MoveStop()
{
	ChangeSimulStatus( eMoSimSt_Stop );
	return ERR_SUCCESS;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
ERR_CODE OneAxis_Sim::R_Move(double a_dMove_pos, double a_dSpeed/*=0*/, BOOL bPcdBacklashMode/*=FALSE*/)
{
	ERR_CODE nErr = CheckMotorUsable();
	if( nErr != ERR_SUCCESS ){
		return nErr;
	}

	double dTargetPos = 0.0;
	nErr = GetCurrentPos(dTargetPos);
	if (nErr != ERR_SUCCESS)
	{
		return nErr;
	}

	if( m_cbInterLock_Move != NULL ){
		ERR_CODE nInterLockErr = m_cbInterLock_Move(dTargetPos, a_dSpeed, m_nCmdAcc, m_nCmdAcc);
		if( nInterLockErr != ERR_SUCCESS ){
			return nInterLockErr;
		}
	}

	nErr = GetCurrentPos(m_dTarget_pos_um);
	if (nErr != ERR_SUCCESS)
	{
		return nErr;
	}

	m_dTarget_pos_um += a_dMove_pos;
	m_dWork_Velocity = a_dSpeed;
	// --- 20190405 ihc. copy from ms100
	double dTargetPos_pulse = (double)m_nDir_Move * a_dMove_pos / (m_dScaleFactor + m_dScaleFactorOffset);
	double dMovePos_mm = a_dMove_pos / 1000;
	SaveMotorMoveLog(dTargetPos_pulse, dMovePos_mm, 1);

	m_SimMotorState.bInpos = FALSE;
	m_SimMotorState.bDone = FALSE;
	m_SimMotorState.bSequence = TRUE;
	m_SimMotorState.bMotion = TRUE;
	// --- 20190405 ihc
	
	ChangeSimulStatus( eMoSimSt_Move );

	return ERR_SUCCESS;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 0   : org ing
             -1  : org complete
			 0>  : org fail
  COMMENTS : org가 될 때 까지 함수를 호출 한다.
==============================================================================*/
ERR_CODE	OneAxis_Sim::Origin()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return ERR_FAILURE;
	}

	if(m_nOri_Flag == eMotorOrgFlag_OK) {
		return ERR_SUCCESS;
	}
	
	if(m_SimMotorState.bAmpFault){
		return ERR_MOTOR_CMN_AMP_FAULT;
	}

	if(!GetServoStat() && m_nOri_Step==0){
		SetStop();	
		return ERR_MOTOR_CMN_SERVO_OFF;
	}

	if( m_cbInterLock_Org != NULL ){
		ERR_CODE nInterLockErr = m_cbInterLock_Org();
		if( nInterLockErr != ERR_SUCCESS ){
			return nInterLockErr;
		}
	}

	switch(m_nOri_Step)
	{
	case 0:
		{
			m_dOrg_Func_Call_Time_msec = (double)::GetTickCount64();
			ClearStatus();
			FramesClear();	
			m_nOri_Step = 80;
			SetStopRate(1);		

			m_nOri_Flag = eMotorOrgFlag_ING;				// soft servo ethercat 방식으로 인해 수정
			m_SimMotorState.bSen_Org = FALSE;
		}break;
	case 10:								// actuator initial
		{
			m_SimMotorState.bDone = FALSE;
			m_SimMotorState.bMotion = TRUE;
			m_SimMotorState.bPowerOn = TRUE;
			m_SimMotorState.bInpos = FALSE;
			m_SimMotorState.bSequence = TRUE;
			m_nOri_Step = 20;
		}break;
	case 20:
		{
			if( g_pMotion_Cont_Imp->m_nSimulResponseMode == CMotion_Cont_Imp::eZeroTimeResp ){
				m_nOri_Step = 80;
				break;
			}
			double dOrg_Func_Call_Time_usec = (double)::GetTickCount64() - m_dOrg_Func_Call_Time_msec;
			if( m_SimMotorState.dCurPos > 0 ){
				m_SimMotorState.dCurPos -= m_dCmdVelocity_Ori * dOrg_Func_Call_Time_usec / 1000.0;
			}else{
				m_SimMotorState.dCurPos += m_dCmdVelocity_Ori * dOrg_Func_Call_Time_usec / 1000.0;
			}

			if( fabs(m_SimMotorState.dCurPos - m_dCmdVelocity_Ori) < m_dCmdVelocity_Ori )
			{
				m_nOri_Step = 80;
			}
		}break;
	case 80:
		{
			m_SimMotorState.bDone = TRUE;
			m_SimMotorState.bMotion = FALSE;
			m_SimMotorState.bInpos = TRUE;
			m_SimMotorState.bSequence = FALSE;

			m_nOri_Step = 0;
			m_nOri_Flag = eMotorOrgFlag_OK;
			m_SimMotorState.bSen_Org = TRUE;
			SetCommandPos(0.0);
			SetCurrentPos(0.0);

			return ERR_SUCCESS;
		}break;
	default:
		break;
	}	

	// origin 진행 중.
	m_dOrg_Func_Call_Time_msec = (double)::GetTickCount64();

	return ERR_SUCCESS;
}


/*=============================================================================
  AUTHOR   : 나명진
  NAME     : OriginWhile
  PARAMS   : None
  RETURN   : 0   = 해당 축 사용 안함 or Board open fail or 원점 체크 완료.
			 0>  = 원점 잡기 실패
  COMMENTS : 해당 축의 원점 잡기 ( motion done 확인 )
==============================================================================*/
ERR_CODE OneAxis_Sim::OriginWhile()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return ERR_SUCCESS;
	}

	SetSWLimit(m_dSwLimit_Posi_um, m_dSwLimit_Nega_um, TRUE);

	for(int nOriginRetry = 0; nOriginRetry < 2; nOriginRetry++)
	{
		m_nOri_Flag   = eMotorOrgFlag_ING;	//다시 원점을 잡을 경우는 무조건 셋팅 한다.
		m_nOri_Step   = 0;
		m_dTarget_pos_um = 0;

		do
		{
			MSG message;
			if(::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&message);
				::DispatchMessage(&message);
			}
			NEXT;

			int nOriResult = Origin();
			if(nOriResult > 0 ){
				SetStop();	//stop
				Sleep(100);
				AlarmReset();	//Servo reset
				Sleep(100);
				ServoOn();	//Servo on
				Sleep(100);

				if(nOriginRetry == 1){		// 2번째 잡을 때까지도 문제가 되면 return
					return nOriResult;
				}else{
					break;			// 1번 잡고 문제가 될 경우 다음번 잡을 수 있도록 한다.
				}
			}
			else if(m_nOri_Flag == eMotorOrgFlag_OK){	// 원점이 잡히면 빠져 나감.
				break;
			}
		}while(m_nOri_Flag==eMotorOrgFlag_ING);
	}

	SetSWLimit(m_dSwLimit_Posi_um, m_dSwLimit_Nega_um, FALSE);

	return ERR_SUCCESS;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-24]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
BOOL OneAxis_Sim::IsOrgCompleted(ERR_CODE *pError)
{
	ERR_CODE Error = ERR_SUCCESS;
	BOOL bResult = FALSE;

	if (m_nOri_Flag == eMotorOrgFlag_OK)
	{
		Error = ERR_SUCCESS;
		bResult = TRUE;
	}
	else if (m_nOri_Flag == eMotorOrgFlag_ING)
	{
		Error = ERR_MOTOR_CMN_IN_MOVE_STATE;
	}
	else
	{
		Error = ERR_MOTOR_CMN_NOT_ORG_CHECKED;
	}

	if (pError != NULL)
		*pError = Error;

	return bResult;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   :  1 = done ok
             -1 = error
  COMMENTS : 
==============================================================================*/
ERR_CODE	OneAxis_Sim::MotionDone()
{
	//m_SimMotorState.bDone = TRUE;
	//m_SimMotorState.bSequence = FALSE;		// 20191120 IHC. Done처리
	//m_SimMotorState.bMotion = FALSE;		// 20191120 IHC. Done처리
	return m_SimMotorState.bDone ? ERR_SUCCESS : ERR_MOTOR_CMN_IN_MOVE_STATE;
}

int	OneAxis_Sim::AxisDone(std::vector<CPoint>& vPicker, std::vector<CPoint>& vErrPicker)
{
	if (!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable) {
		return ERR_SUCCESS;
	}

	ERR_CODE Error = MotionDone();

	if (Error == ERR_SUCCESS) {
		return Error;
	}
	
	return Error;	
}
/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   :  0        = 움직이는 상태
              1        = 해당 축 동작 완료 or 해당 축 사용 안함 or Board open fail
             -1        = 해당 축 Timeover or Amp fault
  COMMENTS : 해당 축의 동작이 완료 되었거나, AmpFault 상태 일 경우 (memory에서 Door 열기 위한 체크 인듯..)
==============================================================================*/
ERR_CODE OneAxis_Sim::AxisDone()
{
 	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
 		return ERR_SUCCESS;
 	}

	ERR_CODE Error = MotionDone();

	if (Error == ERR_SUCCESS) {
		return Error;

		//double encoder = GetCurrentPos();
		//double dInposValue = m_SimMotorState.dInposRange; 
		//double dErrPosAbs = fabs(encoder - m_dTarget_pos);

		//if (dErrPosAbs < dInposValue) {
		//	return 1;
		//}
		//else
		//{
		//	Move(m_dTarget_pos, m_dWork_Velocity, m_nWork_Acc, m_nWork_Dcc);
		//	TRACE("=== AxisDone Fail & Excute Move [Error Pos = %.6f, InposValue = %.6f ===\n", dErrPosAbs, dInposValue);
		//}

// 		if( (encoder > m_dTarget_pos - err_pos) && (encoder < m_dTarget_pos + err_pos))
// 		{
// 			return 1;
// 		}
	}

	return Error;
}

ERR_CODE OneAxis_Sim::ChkPremovePos()
{
	double dEncoder_um = 0.0;
	ERR_CODE Error = GetCurrentPos(dEncoder_um);
	if (Error != ERR_SUCCESS)
	{
		return Error;
	}

	double dPosErrAbs_um = fabs(dEncoder_um - m_dTarget_pos_um);

	if (dPosErrAbs_um <= PREMOVE_POSITION_GAB)
	{
		return ERR_SUCCESS;
	}

	return ERR_FAILURE;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 0       = OK or 이미 servo on 상태
             1       = 해당 축 사용 안함 or Board open fail
  COMMENTS : Amp를 Enable 시키고 현재 위치를 다시 넣어준다.
==============================================================================*/
ERR_CODE OneAxis_Sim::ServoOn()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}

	if(GetServoStat() == TRUE){	// Servo Status On is return 0
		return ERR_SUCCESS; 
	}

	m_SimMotorState.bServoOn = TRUE;

	//현재의 위치를 다시 넣는다.
	double dPos = 0.0;
	ERR_CODE Error = GetCurrentPos(dPos);
	if (Error != ERR_SUCCESS)
		return Error;

	SetCommandPos(dPos);

	return Error;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : None
  RETURN   : 0       = OK 
             1       = 해당 축 사용 안함 or Board open fail
  COMMENTS : Amp를 disable 시킨다.
==============================================================================*/
ERR_CODE OneAxis_Sim::ServoOff()
{
	if (!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable) {
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}

	m_SimMotorState.bServoOn = FALSE;

	return ERR_SUCCESS;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  RETURN   : FALSE : OK
             TRUE  : Amp Fault
  COMMENTS : servo 상태를 확인 and Servo의 전원이 꺼진것을 체크하여 org 하는 용도
==============================================================================*/
BOOL OneAxis_Sim::CheckPowerOffStat()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return FALSE;
	}


	if( m_SimMotorState.bAmpFault )
	{
		m_nOri_Flag=eMotorOrgFlag_NG;
		return TRUE;
	}
	return FALSE;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void OneAxis_Sim::SetCommandPos(double posit)
{
	m_dTarget_pos_um = posit;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
ERR_CODE OneAxis_Sim::GetCommandPos(double &dPos)
{
	dPos = m_dTarget_pos_um;
	return ERR_SUCCESS;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void OneAxis_Sim::SetCurrentPos(double pos)
{
	m_SimMotorState.dCurPos = pos;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
ERR_CODE OneAxis_Sim::GetCurrentPos(double &dPos)
{
	double curr_pos_um = 0.0;
	switch( m_eMotorType )
	{
	case eMotorType_Servo: { curr_pos_um = m_SimMotorState.dCurPos; }break;
	case eMotorType_Step: { curr_pos_um = m_dTarget_pos_um; } break;
	default:
		break;
	}

	dPos = curr_pos_um;

	return ERR_SUCCESS;
}

double OneAxis_Sim::GetCurrentPos(double &dPos, _eBbxmnpAxis eAxis)
{
	double curr_pos_um = 0.0;
	switch (m_eMotorType)
	{
	case eMotorType_Servo: { curr_pos_um = m_SimMotorState.dCurPos; }break;
	case eMotorType_Step: { curr_pos_um = m_dTarget_pos_um; } break;
	default:
		break;
	}

	dPos = curr_pos_um;

	return ERR_SUCCESS;
}

int	 OneAxis_Sim::GetCurrenPosAvg(double &dPos)
{
	double curr_pos_um = 0.0;
	switch (m_eMotorType)
	{
	case eMotorType_Servo: { curr_pos_um = m_SimMotorState.dCurPos; }break;
	case eMotorType_Step: { curr_pos_um = m_dTarget_pos_um; } break;
	default:
		break;
	}

	dPos = curr_pos_um;

	return ERR_SUCCESS;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void OneAxis_Sim::Set_disable(BOOL bDisable)
{
	m_bMotorDisable = bDisable;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
bool OneAxis_Sim::InSequence()
{ 
	return m_SimMotorState.bSequence; 
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
bool OneAxis_Sim::InMotion()
{ 
	return m_SimMotorState.bMotion; 
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
bool OneAxis_Sim::InPosition()
{ 
	//m_SimMotorState.bInpos = TRUE;
	return m_SimMotorState.bInpos;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
bool OneAxis_Sim::IsAxisDone()
{ 
	return m_SimMotorState.bDone; 
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   :  0        = 움직이는 상태
              1        = 동작 완료 or 해당 축 Ampfault
  COMMENTS : 해당 축의 동작이 완료 되었거나, AmpFault 상태 일 경우 (memory에서 Door 열기 위한 체크 인듯..)
==============================================================================*/
bool OneAxis_Sim::IsAxisDoneOrFree()
{
	if(GetServoStat())
	{
		if(MotionDone() == ERR_SUCCESS)
			return true;
		else
			return false;
	}

	return true;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 0       = OK 
             1       = 해당 축 사용 안함 or Board open fail
            -1       = 해당 축이 Origin이 잡혀 있지 않을 경우
  COMMENTS : 현재 amp가 on or off 인지를 리턴..
==============================================================================*/
BOOL OneAxis_Sim::GetServoStat()
{
	return m_SimMotorState.bServoOn;
}

bool OneAxis_Sim::IsServoOn()
{
	return m_SimMotorState.bServoOn;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
bool OneAxis_Sim::IsHomeSensorOn()
{
	return m_SimMotorState.bSen_Org;
}

bool OneAxis_Sim::IsHomeSensorOn(_eBbxmnpAxis eAxis)
{
	return m_SimMotorState.bSen_Org;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
bool OneAxis_Sim::IsPositiveSensorOn()
{
	return m_SimMotorState.bSen_PosiLimit;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
bool OneAxis_Sim::IsNegativeSensorOn()
{
	return m_SimMotorState.bSen_NegaLimit;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
double OneAxis_Sim::GetVelocity()
{
	return m_SimMotorState.dCurSpeed;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int OneAxis_Sim::GetAcc()
{
	return m_SimMotorState.nCurAcc;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int OneAxis_Sim::GetDcc()
{
	return m_SimMotorState.nCurDcc;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : command speed를 반환 함.
==============================================================================*/
double OneAxis_Sim::GetSpeed()
{
	return m_dCmdVelocity;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
bool OneAxis_Sim::IsAmpFault()
{
	return m_SimMotorState.bAmpFault;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 현재 위치와 목표위치의 차이를 리턴.
==============================================================================*/
double OneAxis_Sim::GetErrorValue()
{
	double dErr = m_dTarget_pos_um - m_SimMotorState.dCurPos;
	return dErr; 
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
ERR_CODE OneAxis_Sim::SetStop()
{
	ChangeSimulStatus( eMoSimSt_Stop );
	return ERR_SUCCESS; 
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
ERR_CODE	OneAxis_Sim::SetSlowStop()
{
	ChangeSimulStatus( eMoSimSt_Stop );
	return ERR_SUCCESS;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void OneAxis_Sim::SetStopRate(double stop_dcc)
{
	m_SimMotorState.dStop_Acc = stop_dcc;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
bool OneAxis_Sim::SetSWLimit(double dPosLimit, double dNegLimit, BOOL bDisable/* = FALSE*/)
{
	m_dSwLimit_Posi_um = dPosLimit;
	m_dSwLimit_Nega_um = dNegLimit;
	m_SimMotorState.bSwLimitStopEnable = !bDisable;
	return true;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void OneAxis_Sim::EnableSWLimit(BOOL bEnable)
{
	m_SimMotorState.bSwLimitStopEnable = bEnable;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
ERR_CODE OneAxis_Sim::AlarmReset()
{
	m_SimMotorState.bAmpFault = FALSE;
	ChangeSimulStatus( eMoSimSt_Idle );
	return ERR_SUCCESS;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void OneAxis_Sim::MotorStrageCheck(ST_MOTORSTATUS* lpstMotorData)
{

}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : None
  RETURN   : 0 : mmc board init fail or 해당 축 disable or Amp Fault
             1 : OK
  COMMENTS : Origin flag clear
==============================================================================*/
BOOL OneAxis_Sim::AmpFaultCheckFunction()
{
	BOOL bChk = TRUE;

	if( m_SimMotorState.bAmpFault ) bChk = FALSE;

	return bChk;
}




/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
bool OneAxis_Sim::IsPositionDone()
{
	if( InMotion() && InPosition() )
	{
		double dCmdPos = 0.0, dCurPos = 0.0;

		ERR_CODE Error = GetCommandPos(dCmdPos);
		if (Error != ERR_SUCCESS)
			return false;

		Error = GetCurrentPos(dCurPos);
		if (Error != ERR_SUCCESS)
			return false;

		double dDifPos = fabs(dCmdPos-dCurPos);
		double dInPos = m_SimMotorState.dInposRange;
		if( dDifPos<= dInPos )
			return true;
	}

	return false;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
bool OneAxis_Sim::IsMotorStop()		//모터 동작 체크용
{
	if( InSequence() || InMotion() )
		return false;

	return true;
}


/*=============================================================================
  RETURN   : 상태정보
             1  : ST_NONE                :  0x0000    :   정상동작
             2  : ST_HOME_SWITCH         :  0x0001    :   원점(home) 센서 감지
             3  : ST_POS_LIMIT           :  0x0002    :   (+) 방향 Limit 센서 감지
             4  : ST_NEG_LIMIT           :  0x0004    :   (-) 방향 Limit 센서 감지
             5  : ST_AMP_FAULT           :  0x0008    :   AMP Fault 상태 발생
             6  : ST_A_LIMIT             :  0x0010    :   가,감속시간이 limit 보다 클때 발생
             7  : ST_V_LIMIT             :  0x0020    :   현재속도가 속도 limit보다 클때 발생
             8  : ST_X_NEG_LIMIT         :  0x0040    :   (-) Position limit를 벗어날때 발생
             9  : ST_X_POS_LIMIT         :  0x0080    :   (+) Position limit를 벗어날때 발생
             10 : ST_ERROR_LIMIT         :  0x0100    :   위치오차가 limit값보다 클때 발생
             11 : ST_PC_COMMAND          :  0x0200    :   EVENT가 발생된 상태일때 발생  set_stop,set_e_stop,set_linear_all_stop
             12 : ST_OUT_OF_FRAMES       :  0x0400    :   Frame Buffer가 Full 상태일때 발생
             13 : ST_AMP_POWER_ONOFF     :  0x0800    :   AMP Disable 상태일때 발생
             14 : ST_ABS_COMM_ERROR      :  0x1000    :   ABS Encoder 통신 error시 발생
             15 : ST_INPOSITION_STATUS   :  0x2000    :   In Position Signal 입력 여부
             16 : ST_RUN_STOP_COMMAND    :  0x4000    :   RUN STOP Error
             17 : ST_COLLISION_STATE     :  0x8000    :   충돌 방지 Error
==============================================================================*/
int OneAxis_Sim::GetAxisStatus()
{
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
ERR_CODE OneAxis_Sim::OriginStart()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}
	m_nOri_Flag = eMotorOrgFlag_ING;
	return ERR_SUCCESS;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int OneAxis_Sim::GetNoControlAxis()
{
	int nCount = 0;
	if( g_pMotion_Cont_Imp ){
		nCount = g_pMotion_Cont_Imp->GetAxisCount();
	}
	return nCount;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
double OneAxis_Sim::GetTargetPos(bool bMM/*=false*/)
{
	return m_dTarget_pos_um;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
ERR_CODE OneAxis_Sim::SetEmergencyStop()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable)
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE ;
	
	ChangeSimulStatus( eMoSimSt_EStop );

//	if( nRet== 0 )
	{
//		if( IsOrgCompleted()==FALSE )
		m_nOri_Flag = eMotorOrgFlag_NG;
	}

	AlarmReset();

	return ERR_SUCCESS;
}


// --- 20190405 ihc. copy from ms100
// void OneAxis_Sim::SetApplyBacklash(bool bApplyBacklash)
// {
// 	m_bApplyBacklash = bApplyBacklash;
// }
// 
// bool OneAxis_Sim::GetApplyBacklash()
// {
// 	return m_bApplyBacklash;
// }
// 
// void OneAxis_Sim::SetBackLashOffset(double dBackLashOffset)
// {
// 	m_dBackLashOffset = dBackLashOffset;
// }

// double OneAxis_Sim::GetBacklashOffset(double dMovePos)
// {
// 	double dOffset = 0.0;
// 
// 	if (m_bApplyBacklash == false)
// 		return dOffset;
// 	else
// 	{
// 		if (m_dPrevMovePos != dMovePos && m_dTarget_pos_um != dMovePos)
// 		{
// 			//150mm를 기준으로 150mm 보다 작은 이동 거리는 Off-Set을 Liner하게 구하여 누적 시켜준다.
// 			if (fabs(m_dPrevMovePos - dMovePos) < m_dBacklashMoveDist)
// 			{				
// 				if (m_dBacklashMoveDist == 0.0)	// 20190415 ihc. 0나누기 방지
// 				{
// 					//150mm보다 작은 경우 150mm를 기준으로 %를 계산 하여 적용
// 					dOffset = m_dBackLashOffset * (fabs(m_dPrevMovePos - dMovePos) / 1.0);
// 				}
// 				else
// 				{
// 					//150mm보다 작은 경우 150mm를 기준으로 %를 계산 하여 적용
// 					dOffset = m_dBackLashOffset * (fabs(m_dPrevMovePos - dMovePos) / m_dBacklashMoveDist);
// 				}
// 			}
// 			else
// 			{
// 				dOffset = m_dBackLashOffset;
// 			}
// 			// 현재 위치에서 플러스 방향으로 가는 경우
// 			if (m_dPrevMovePos < dMovePos)
// 			{
// 				//off_set = -off_set;
// 				//플러스 방향인 경우 무조건 Off-Set을 "0"해서 Test 진행
// 				//해당 내용 관련하여 확정된 내용 없음
// 				dOffset = 0.0;
// 				m_dPrevBackLashOffset = 0.0;
// 			}
// 
// 			if (fabs(m_dPrevBackLashOffset + dOffset) > fabs(m_dBackLashOffset))
// 			{
// 				m_dPrevBackLashOffset = m_dBackLashOffset;
// 			}
// 			else
// 			{
// 				m_dPrevBackLashOffset = dOffset;
// 			}
// 
// 			m_dPrevMovePos = dMovePos;
// 		}
// 		else
// 		{
// 			m_dPrevBackLashOffset = 0.0;
// 		}
// 	}
// 
// 	return m_dPrevBackLashOffset;
// }
// // --- 20190405 ihc.
/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
UINT OneAxis_Sim::ThreadProc(LPVOID lpVoid)
{
	OneAxis_Sim* pSelf = (OneAxis_Sim*)lpVoid;

	while( !pSelf->m_bSopThread ){
		pSelf->MotionSimulProc();
		Sleep(1);
	}

	pSelf->m_evtTerminatedThread.SetEvent();
	return 0;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void OneAxis_Sim::MotionSimulProc()
{
	switch( m_nMotionSimulState )
	{
	case eMoSimSt_Idle: Run_Sim_Idle(); break;
	case eMoSimSt_Move: Run_Sim_Move(); break;
	case eMoSimSt_VMove: Run_Sim_VMove(); break;
	case eMoSimSt_Stop: Run_Sim_Stop(); break;
	case eMoSimSt_EStop: Run_Sim_EStop(); break;
	case eMoSimSt_Alarm: Run_Sim_Alarm(); break;
	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void OneAxis_Sim::Run_Sim_Idle()
{
	switch( m_nMotorSimulStep )
	{
	case 0:
		{
			m_SimMotorState.bInpos = TRUE;
			m_SimMotorState.bDone  = TRUE;
			m_SimMotorState.bSequence = FALSE;
			m_SimMotorState.bMotion = FALSE;
			m_nMotorSimulStep++;
		}break;
	case 1:
		{

		}break;
	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void OneAxis_Sim::Run_Sim_Move()
{
	switch( m_nMotorSimulStep )
	{
	case 0:
		{
			m_SimMotorState.bInpos = FALSE;
			m_SimMotorState.bDone  = FALSE;
			m_SimMotorState.bSequence = TRUE;
			m_SimMotorState.bMotion = TRUE;

			m_SimMotorState.dCurSpeed = m_dWork_Velocity;
			m_SimMotorState.nCurAcc = m_nWork_Acc;
			m_SimMotorState.nCurDcc = m_nWork_Dcc;
			m_dMove_Func_Call_Time_msec = (double)::GetTickCount64();

			if (m_bDegeeCtrl == TRUE)	// (각)도 단위로
			{
				m_dTarget_pos_um /= 1000.0;
			}

			m_nMotorSimulStep++;
		}break;
	case 1:
		{
			if( g_pMotion_Cont_Imp->m_nSimulResponseMode == CMotion_Cont_Imp::eZeroTimeResp ){
				m_SimMotorState.dCurPos = m_dTarget_pos_um;
				m_nMotorSimulStep++;
				break;
			}

// 			if( fabs(m_SimMotorState.dCurPos - m_dTarget_pos) < m_dWork_Velocity )
// 			{
// 				m_SimMotorState.dCurPos = m_dTarget_pos;
// 				m_nMotorSimulStep++;
// 				break;
// 			}



			if( fabs(m_SimMotorState.dCurPos - m_dTarget_pos_um) < m_SimMotorState.dInposRange )
			{
				m_SimMotorState.dCurPos = m_dTarget_pos_um;
				m_nMotorSimulStep++;
			}
			else
			{
				double dTime_usec = (double)::GetTickCount64() - m_dMove_Func_Call_Time_msec;
				if( m_SimMotorState.dCurPos > m_dTarget_pos_um ){
					m_SimMotorState.dCurPos -= (m_dWork_Velocity * dTime_usec / 1000.0);
					
					// 계산된 이동량 때문에 곧바로 반대로 m_SimMotorState.dCurPos 값의 크기가
					// 바뀔 경우에 이동이 끝난 것으로 본다.
					if (m_SimMotorState.dCurPos <= m_dTarget_pos_um) {
						m_SimMotorState.dCurPos = m_dTarget_pos_um;
						m_nMotorSimulStep++;
					}
				}else{
					m_SimMotorState.dCurPos += (m_dWork_Velocity * dTime_usec / 1000.0);

					// 계산된 이동량 때문에 곧바로 반대로 m_SimMotorState.dCurPos 값의 크기가
					// 바뀔 경우에 이동이 끝난 것으로 본다.
					if (m_SimMotorState.dCurPos > m_dTarget_pos_um) {
						m_SimMotorState.dCurPos = m_dTarget_pos_um;
						m_nMotorSimulStep++;
					}
				}

				m_dMove_Func_Call_Time_msec = (double)::GetTickCount64();

				if( m_SimMotorState.dCurPos > m_dSwLimit_Posi_um || m_SimMotorState.dCurPos < m_dSwLimit_Nega_um )
				{
					ChangeSimulStatus( eMoSimSt_Alarm );
				}
			}
		}break;
	case 2:
		{
			ChangeSimulStatus( eMoSimSt_Idle );
		}break;
	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void OneAxis_Sim::Run_Sim_VMove()
{
	switch( m_nMotorSimulStep )
	{
	case 0:
		{
			m_SimMotorState.bInpos = FALSE;
			m_SimMotorState.bDone  = FALSE;
			m_SimMotorState.bSequence = TRUE;
			m_SimMotorState.bMotion = TRUE;

			m_SimMotorState.dCurSpeed = m_dWork_Velocity;
			m_SimMotorState.nCurAcc = m_nCmdAcc;
			m_SimMotorState.nCurDcc = m_nCmdDcc;
			m_dMove_Func_Call_Time_msec = (double)::GetTickCount64();
			m_nMotorSimulStep++;
		}break;
	case 1:
		{
			// 속도에 의해 방향이 결정 됨.
			double dTime_usec = (double)::GetTickCount64() - m_dMove_Func_Call_Time_msec;
			m_SimMotorState.dCurPos += (m_dWork_Velocity * dTime_usec / 1000.0);
			m_dMove_Func_Call_Time_msec = (double)::GetTickCount64();

			if( m_SimMotorState.dCurPos > m_dSwLimit_Posi_um || m_SimMotorState.dCurPos < m_dSwLimit_Nega_um )
			{
				ChangeSimulStatus( eMoSimSt_Alarm );
			}
		}break;
	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void OneAxis_Sim::Run_Sim_Stop()
{
	switch( m_nMotorSimulStep )
	{
	case 0:
		{
			m_SimMotorState.bInpos = TRUE;
			m_SimMotorState.bDone  = TRUE;
			m_SimMotorState.bSequence = FALSE;
			m_SimMotorState.bMotion = FALSE;
			m_nMotorSimulStep++;
		}break;
	case 1:
		{
			ChangeSimulStatus( eMoSimSt_Idle );
		}break;
	}
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void OneAxis_Sim::Run_Sim_EStop()
{
	switch( m_nMotorSimulStep )
	{
	case 0:
		{
			m_SimMotorState.bInpos = TRUE;
			m_SimMotorState.bDone  = TRUE;
			m_SimMotorState.bSequence = FALSE;
			m_SimMotorState.bMotion = FALSE;
			m_nMotorSimulStep++;
		}break;
	case 1:
		{
			ChangeSimulStatus( eMoSimSt_Idle );
		}break;
	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void OneAxis_Sim::Run_Sim_Alarm()
{
	switch( m_nMotorSimulStep )
	{
	case 0:
		{
			m_SimMotorState.bInpos = TRUE;
			m_SimMotorState.bDone  = TRUE;
			m_SimMotorState.bSequence = FALSE;
			m_SimMotorState.bMotion = FALSE;
			m_SimMotorState.bAmpFault = TRUE;
			m_SimMotorState.bServoOn = FALSE;
			m_nMotorSimulStep++;
		}break;
	case 1:
		{
			ChangeSimulStatus( eMoSimSt_Idle );
		}break;
	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void OneAxis_Sim::ChangeSimulStatus(int nNewState)
{
	m_nMotionSimulState = nNewState;
	m_nMotorSimulStep = 0;
}

void OneAxis_Sim::SaveMotorMoveLog( double dTargetPos, double dMovePos, int nLogMode )
{
	// Motor Log 남기지 않도록 주석처리 20190110 김현진
	//// Move
	//if(nLogMode == 0){
	//	m_MtLog.LOG(_T("%s[PhyNo %d] Target Position[MOVE]: [Target Pos: %0.3f] [Move Pos: %0.3fmm]"),m_szAxisName, m_nAxisNumPhy, dTargetPos, dMovePos);
	//}
	//// R Move
	//else if(nLogMode == 1){
	//	m_MtLog.LOG(_T("%s[PhyNo %d] Target Position[R_MOVE]: [Target Pos: %0.3f] [Move Pos: %0.3fmm]"),m_szAxisName, m_nAxisNumPhy, dTargetPos, dMovePos);
	//}
	//// V Move
	//else if(nLogMode == 2){
	//	m_MtLog.LOG(_T("%s[%d] Target Position[V_MOVE]: [Target Pos: %0.3f]"),m_szAxisName, m_nAxisNumPhy, dTargetPos);
	//}	
}

void OneAxis_Sim::LogMtStrage( int nAxisNumPhy, double dPos/*=0.0*/, double dSpeed/*=0.0*/ )
{
	// Motor Log 남기지 않도록 주석처리 20190110 김현진
	//ST_MOTORSTATUS stMotorData;
	//stMotorData.dAcc            = GetAcc();
	//stMotorData.dDcc			= GetDcc();
	//stMotorData.dCommand		= GetCommandPos();
	//stMotorData.dEncoder		= GetCurrentPos();
	//stMotorData.bServoStatus	= GetServoStat();
	//stMotorData.iInsequence		= -1;//in_sequence(nAxisNumPhy);
	//stMotorData.iInmotion		= -1;//in_motion(nAxisNumPhy);
	//stMotorData.iInposition		= -1;//in_position(nAxisNumPhy);
	//stMotorData.iFramesleft		= -1;//frames_left(nAxisNumPhy);
	//stMotorData.iMotiondone		= -1;//motion_done(nAxisNumPhy);
	//stMotorData.iAxisdone		= -1;//axis_done(nAxisNumPhy);
	//stMotorData.iAxisNum		= nAxisNumPhy;
	//stMotorData.dTarget			= dPos;
	//stMotorData.dMotorSpeed		= dSpeed;
	//stMotorData.sEncoderRPM = -1;
	////get_command_rpm(m_nAxisNumPhy, &stMotorData.sEncoderRPM);
	//
	//CString strData = _T("");
	//strData.Format(_T("motor[%s][T Pos %.2fmm][E Pos %.2fmm][cmd Pos %.2fmm][Spd: %.2f pps][Acc: %d][Dcc: %d][AmpFault: %d]"),
	//	m_szAxisName,
	//	stMotorData.dTarget/1000.0,
	//	stMotorData.dEncoder/1000.0,
	//	stMotorData.dCommand/1000.0,
	//	stMotorData.dMotorSpeed, 
	//	stMotorData.dAcc, 
	//	stMotorData.dDcc, 
	//	stMotorData.bServoStatus
	//	);
	//m_MtLog.LOG(strData);
	//
	//strData.Format(_T("[in_Sequence: %d][in_motion: %d] [in_position : %d] [frames_left : %d] [motion_done : %d] [axis_done : %d] [Cmd RPM : %d]"),
	//	stMotorData.iInsequence,
	//	stMotorData.iInmotion, 
	//	stMotorData.iInposition, 
	//	stMotorData.iFramesleft, 
	//	stMotorData.iMotiondone, 
	//	stMotorData.iAxisdone, 
	//	stMotorData.sEncoderRPM);
	//m_MtLog.LOG(strData);
}

// --- 20190405 ihc. copy from ms100
// void OneAxis_Sim::SetScaleFactorOffset(double dOffset)
// {
// 	m_dScaleFactorOffset = dOffset;
// }
// 
// double OneAxis_Sim::GetScaleFactorOffset()
// {
// 	return m_dScaleFactorOffset;
// }
// --- 20190405 ihc.

double OneAxis_Sim::GetBacklashOffset_TestPpX(double dMovePos, double dPPS)
{
	// 현재 속도에 맞는 Backlash Offset Data Load.
	double dTargetDistance_ApplyBefore = fabs(m_dBefore_BacklashPos - dMovePos);
	double dTargetDistance_ApplyAfter = fabs(m_dAfter_BacklashPos - dMovePos);
	double dCheckYMove = dTargetDistance_ApplyAfter - fabs(m_dMem_BacklashOffset);
	double dMotorSacale = m_dScaleFactor * m_dServo_Pulse;
	double dNewDistanceRate_Pulley = dTargetDistance_ApplyBefore / dMotorSacale;
	double dBacklashOffset = 0.0;

	if (m_dAfter_BacklashPos > dMovePos && dCheckYMove > FLT_EPSILON) // - 방향으로 이동할 때, Backlash 감소. 
	{
		m_dDistanceRate -= dNewDistanceRate_Pulley;

		if (m_dDistanceRate < 0.0)
		{
			m_dDistanceRate = 0.0;
		}

		if (dNewDistanceRate_Pulley > 1.0)
		{
			dNewDistanceRate_Pulley = 1.0;
		}

		dTargetDistance_ApplyBefore = dNewDistanceRate_Pulley * 150000.0;

		dBacklashOffset = m_dMem_BacklashOffset - GetBacklashParameterData(dTargetDistance_ApplyBefore, dPPS);

		if (dBacklashOffset < 0.0)
		{
			dBacklashOffset = 0.0;
		}

		m_dMem_BacklashOffset = dBacklashOffset;
	}
	else if (dTargetDistance_ApplyAfter == 0.0) // Backlash Offset 적용 후 추가 이동하지 않도록 함.
	{
		dBacklashOffset = 0.0;
	}
	else // + 방향으로 이동할 때, Backlash 증가. 
	{
		m_dDistanceRate += dNewDistanceRate_Pulley;

		if (m_dDistanceRate > 1.0)
		{
			m_dDistanceRate = 1.0;
		}

		dTargetDistance_ApplyBefore = m_dDistanceRate * 150000.0;

		dBacklashOffset = GetBacklashParameterData(dTargetDistance_ApplyBefore, dPPS);

		m_dMem_BacklashOffset = dBacklashOffset;
	}

	m_dBefore_BacklashPos = dMovePos; // 처음 Target Pos 저장
	m_dAfter_BacklashPos = dMovePos + dBacklashOffset;
	return dBacklashOffset;
}

double OneAxis_Sim::GetBacklashOffset(double dMovePos, double dPPS)
{
	// 현재 속도에 맞는 Backlash Offset Data Load.
	double dBacklashOffset = GetBacklashDataAboutSpeed(dPPS);

	double dTargetDistance_ApplyBefore = fabs(m_dBefore_BacklashPos - dMovePos);
	double dTargetDistance_ApplyAfter = fabs(m_dAfter_BacklashPos - dMovePos);
	double dCheckYMove = dTargetDistance_ApplyAfter - fabs(m_dMem_BacklashOffset);
	double dMotorSacale = m_dScaleFactor * m_dServo_Pulse;
	double dNewDistanceRate = dTargetDistance_ApplyBefore / dMotorSacale;

	if (m_dAfter_BacklashPos < dMovePos && dCheckYMove > FLT_EPSILON)
	{
		m_dDistanceRate -= dNewDistanceRate;
		if (m_dDistanceRate < 0.0)
		{
			m_dDistanceRate = 0.0;
		}

		dBacklashOffset *= m_dDistanceRate; // 100mm를 기준으로 Backlash Offset 비율로 적용.

		m_dMem_BacklashOffset = dBacklashOffset;
	}
	else if (dTargetDistance_ApplyAfter == 0.0) // Backlash Offset 적용 후 추가 이동하지 않도록 함.
	{
		dBacklashOffset = 0.0;
	}
	else
	{
		m_dDistanceRate += dNewDistanceRate;
		if (m_dDistanceRate > 1)
		{
			m_dDistanceRate = 1;
		}

		dBacklashOffset *= m_dDistanceRate; // 100mm를 기준으로 Backlash Offset 비율로 적용.

		m_dMem_BacklashOffset = dBacklashOffset;
	}

	m_dBefore_BacklashPos = dMovePos; // 처음 Target Pos 저장
	m_dAfter_BacklashPos = dMovePos + dBacklashOffset;
	return dBacklashOffset;
}


// TWSL421 Test PP1 번 X1,X2는 정방향이 '-' 방향, 역방향이 + 방향
double OneAxis_Sim::GetBacklashOffset_TestPP_X1(double dMovePos, double dPPS)
{
	// 현재 속도에 맞는 Backlash Offset Data Load.
	double dBacklashOffset = GetBacklashDataAboutSpeed(dPPS);

	double dTargetDistance_ApplyBefore = fabs(m_dBefore_BacklashPos - dMovePos);
	double dTargetDistance_ApplyAfter = fabs(m_dAfter_BacklashPos - dMovePos);
	double dCheckYMove = dTargetDistance_ApplyAfter - fabs(m_dMem_BacklashOffset);
	double dMotorSacale = m_dScaleFactor * m_dServo_Pulse;
	double dNewDistanceRate = dTargetDistance_ApplyBefore / dMotorSacale;

	if (m_dAfter_BacklashPos > dMovePos && dCheckYMove > FLT_EPSILON)
	{
		m_dDistanceRate -= dNewDistanceRate;
		if (m_dDistanceRate < 0.0)
		{
			m_dDistanceRate = 0.0;
		}

		dBacklashOffset *= m_dDistanceRate; // 100mm를 기준으로 Backlash Offset 비율로 적용.

		m_dMem_BacklashOffset = dBacklashOffset;
	}
	else if (dTargetDistance_ApplyAfter == 0.0) // Backlash Offset 적용 후 추가 이동하지 않도록 함.
	{
		dBacklashOffset = 0.0;
	}
	else
	{
		m_dDistanceRate += dNewDistanceRate;
		if (m_dDistanceRate > 1)
		{
			m_dDistanceRate = 1;
		}

		dBacklashOffset *= m_dDistanceRate; // 100mm를 기준으로 Backlash Offset 비율로 적용.

		m_dMem_BacklashOffset = dBacklashOffset;
	}

	m_dBefore_BacklashPos = dMovePos; // 처음 Target Pos 저장
	m_dAfter_BacklashPos = dMovePos + dBacklashOffset;
	return dBacklashOffset;
}

double OneAxis_Sim::GetBacklashDataAboutSpeed(double speed_umps)
{
	int nSpeedRate = 0;
	// 현재 속도를 백분율로 계산. (0~100%)
	double dMax_speed_umps = GetSpeed();
	nSpeedRate = (int)((speed_umps / dMax_speed_umps) * 100.0);

	const int nSpdMinIndex = 0;
	const int nSpdMaxIndex = 10;
	int nSpeedIdx = 0;

	// Spped Rate가 계산되어 넘어 오기 때문에 딱 맞지 않음
	if (nSpeedRate < 10)
	{
		if (nSpeedRate % 10 > 7)
			nSpeedRate = 10;
		else
			nSpeedRate = 5;
	}
	else {
		if (nSpeedRate % 10 > 7)
		{
			nSpeedRate = nSpeedRate + (10 - (nSpeedRate % 10));
		}
		else
		{
			nSpeedRate = nSpeedRate - (nSpeedRate % 10);
		}
	}

	if (nSpeedRate == 5) {
		nSpeedIdx = nSpdMinIndex;
	}
	else {
		nSpeedIdx = nSpeedRate / 10;
	}

	// 	if( nSpeedIdx > nSpdMaxIndex && nSpeedIdx <= 0 ) {
	// 		nSpeedIdx = nSpdMinIndex;
	// 	}

	double dBacklashOffset = m_vBacklashOffset_XY[nSpeedIdx];

	//if( fabs( dBacklashOffset ) > 1000.0 )
	//{
	//	// 과도한 Offset 방지. 1 mm 초과하는 경우 0 리턴.
	//	return 0.0;
	//}

	return dBacklashOffset;
}

double OneAxis_Sim::GetBacklashParameterData(double dMoveDistance, double dspeed_umps)
{
	int nSpeedRate = 0;
	double dDistanceRate = 0.0;
	int nDistanceIdx = 0;
	// 현재 속도를 백분율로 계산. (0~100%)
	double dMax_speed_umps = GetSpeed();
	nSpeedRate = (int)((dspeed_umps / dMax_speed_umps) * 100.0);

	const int nSpdMinIndex = 0;
	const int nSpdMaxIndex = 10;
	int nSpeedIdx = 0;

	// Spped Rate가 계산되어 넘어 오기 때문에 딱 맞지 않음
	if (nSpeedRate < 10)
	{
		if (nSpeedRate % 10 > 7)
			nSpeedRate = 10;
		else
			nSpeedRate = 5;
	}
	else {
		if (nSpeedRate % 10 > 7)
		{
			nSpeedRate = nSpeedRate + (10 - (nSpeedRate % 10));
		}
		else
		{
			nSpeedRate = nSpeedRate - (nSpeedRate % 10);
		}
	}

	if (nSpeedRate == 5) {
		nSpeedIdx = nSpdMinIndex;
	}
	else {
		nSpeedIdx = nSpeedRate / 10;
	}

	if (nSpeedIdx > nSpdMaxIndex || nSpeedIdx <= 0) {
		nSpeedIdx = nSpdMinIndex;
	}

	nDistanceIdx = (int)(dMoveDistance / 10000.0);
	dDistanceRate = (dMoveDistance / 10000.0) - (double)nDistanceIdx;

	if (nDistanceIdx > 14)
	{
		nDistanceIdx = 14;
		dDistanceRate = 1.0;
	}

	double dSectionMaxBacklashOffset = m_dBacklashParameterOffset_XY[nDistanceIdx][nSpeedIdx];
	double dSectionMinBacklashOffset = 0.0;

	if (nDistanceIdx != 0)
	{
		dSectionMinBacklashOffset = m_dBacklashParameterOffset_XY[nDistanceIdx - 1][nSpeedIdx];
	}

	double dBacklashOffset = dSectionMinBacklashOffset + ((dSectionMaxBacklashOffset - dSectionMinBacklashOffset) * dDistanceRate);

	return dBacklashOffset;
}

void OneAxis_Sim::SetSpeedBacklash(int nSpdIdx, double dValue)
{
	m_vBacklashOffset_XY[nSpdIdx] = dValue;
}

void OneAxis_Sim::SetParameterBacklash(int nDistanceIdx, int nSpdIdx, double dValue)
{
	m_dBacklashParameterOffset_XY[nDistanceIdx][nSpdIdx] = dValue;
}


double OneAxis_Sim::GetScaleFactor()
{
	return m_dScaleFactor;
}
