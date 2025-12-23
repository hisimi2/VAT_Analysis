#include "StdAfx.h"
#include "OneAxis_MMC.h"
#include "Motion_Cont_Imp.h"
#include "DEF_ERROR_CODE.h"

//==============================================================================
//
//==============================================================================
#define MMC_BOARD_HALF

#ifdef MMC_BOARD_FULL
	#include "..\..\..\Include\MMC\mmcwfp.h"
	#pragma comment( lib, "..\\..\\..\\LibFiles\\MMCWFP50.lib" )
#else
	#include "../../../Include/MMC/MMMCWHP220.h"
	#pragma comment( lib, "..\\..\\..\\LibFiles\\MMCWHP220.lib")
#endif



//==============================================================================
//
//==============================================================================
int OneAxis_MMC::InitBoard(int nBoardCount, long* plBoardAddr)
{
	int nRet = mmc_initx(nBoardCount,plBoardAddr);
	return nRet;
}



//==============================================================================
//
//==============================================================================
OneAxis_MMC::OneAxis_MMC()
{
	m_nOri_Flag = eMotorOrgFlag_NG;
	m_nOri_Step = 0;

	// LOG File Name
	char szLogFileName[ 512 ] = { 0, };
	sprintf_s(szLogFileName, sizeof(szLogFileName), "%sMotor.log", SZ_LOG_FILE_PATH);
	m_MtLog.SetFileName(szLogFileName);

	char szLogFileName2[512] = { 0, };
	sprintf_s(szLogFileName2, sizeof(szLogFileName2), "%sOriginEncoder.log", SZ_LOG_FILE_PATH);
	m_EncoderLog.SetFileName(szLogFileName2);

	char szLogFileName3[512] = { 0, };
	sprintf_s(szLogFileName3, sizeof(szLogFileName3), "%sTestYEncoder.log", SZ_LOG_FILE_PATH);
	m_TestYEncoder.SetFileName(szLogFileName3);
}

//==============================================================================
//
//==============================================================================
OneAxis_MMC::~OneAxis_MMC()
{
	ServoOff();
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : Move
  PARAMS   : move_pos    = 움직이고자 하는 위치		[um    ] (미크론 단위로 scale_factor로 나누어야 실제 pulse값이 됨)
             speed       = 움직이려는 축의 속도      [um/sec]
			 accel       = 움직이려는 축의 가속도		[msec  ]
			 dccel       = 움직이려는 축의 감속도		[msec  ]
  RETURN   :  0          = OK
              1          = Board Init Fail or 해당 축 disable
             -1          = Origin이 안 잡힌 상태
			 -2          = Servo가 On이 안되어 있는 상태
			 -3          = AMP FAULT 상태
			 -4          = Limit를 벗어나는 상태
			 -5          = Limit Pulse를 넘는 상태 ( 모터 RPM Over )
  COMMENTS : 해당 축을 움직이려는 함수 ( Motion Done을 확인해야 함 )
==============================================================================*/
int OneAxis_MMC::Move(double a_dPos_um, double a_dSpeed_umps, int a_nAccel, int a_nDccel, BOOL bApplyBacklash /*= TRUE*/ , BOOL bNewBacklashMode /*= FALSE*/)
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() ){
		return ERR_MOT_INTERLOCK_BD_INIT_FAIL;
	}	
	if( m_bMotorDisable ){
		return ERR_MOT_INTERLOCK_IS_NOT_ENABLE;
	}
	if(m_nOri_Flag != eMotorOrgFlag_OK){	// 원점이 안되어 있으면 리턴한다.
		return ERR_MOT_INTERLOCK_IS_NOT_ORG;
	}
	if(!GetServoStat()){
		return ERR_MOT_INTERLOCK_IS_NOT_SERVO_ON;
	}

	if (bApplyBacklash == TRUE)
	{
		if (m_nAxisNumPhy != eAXIS_TEST_PP_1_Y && m_nAxisNumPhy != eAXIS_TEST_PP_2_Y)
		{
			a_dPos_um += GetBacklashOffset(a_dPos_um, a_dSpeed_umps);
		}
		else
		{
			if( ! bNewBacklashMode ) //Test PP Y축 Backlash 보정 시 , 기존 Offset 사용하겠다.
			a_dPos_um += GetBacklashOffset_TestPpY(a_dPos_um, a_dSpeed_umps);
		}
	}

	if( m_cbInterLock_Move != NULL ){
		int nInterLockErr = m_cbInterLock_Move( a_dPos_um, a_dSpeed_umps, a_nAccel, a_nDccel);
		if( nInterLockErr != 0 ){
			return nInterLockErr;
		}
	}

	ClearStatus();
	int r_val = axis_source(m_nAxisNumPhy);	// MMC보드로 부터 축의 현재 상태를 읽어들인다.

	if((r_val & ST_AMP_FAULT))	// amp fault
	{
		LogMtStrage(m_nAxisNumPhy, a_dPos_um, a_dSpeed_umps );
		m_nOri_Flag = eMotorOrgFlag_NG;

		return ERR_MOT_INTERLOCK_IS_AMP_FAULT;
	}

	// 움직이고자 하는 위치가 s/w limit를 벗어나는 위치인지를 확인
	if( a_dPos_um > m_dSwLimit_Posi ){
		return ERR_MOT_INTERLOCK_IS_POSI_SW_LMT;
	}
	if( a_dPos_um < m_dSwLimit_Nega){
		return ERR_MOT_INTERLOCK_IS_NEGA_SW_LMT;
	}

	if( a_dSpeed_umps <=0){	// 속도 설정 안했을 시
		a_dSpeed_umps = m_dCmdVelocity;
	}
	if(a_nAccel<=0){	// 가속 설정 안했을 시
		a_nAccel = m_nCmdAcc;
	}
	if(a_nDccel<=0){	// 감속 설정 안했을 시
		a_nDccel = m_nCmdDcc;
	}
	if( a_dSpeed_umps ==1){
		a_dSpeed_umps = m_dCmdVelocity;
		a_nAccel = 100;
		a_nDccel = 100;
	}
	// 최악의 경우 방지
	if (m_nAxisNumPhy != 24 && m_nAxisNumPhy != 25 && m_nAxisNumPhy != 28)
	{
		if (a_nAccel <= 190) {	// 가속 들어가지 않는거에 대비 하여
			a_nAccel = 190;
		}
		if (a_nDccel <= 190) {	// 감속 들어가지 않는거에 대비 하여
			a_nDccel = 190;
		}
	}
	else {
		if (a_nAccel <= 100) {	// 가속 들어가지 않는거에 대비 하여
			a_nAccel = 100;
		}
		if (a_nDccel <= 100) {	// 감속 들어가지 않는거에 대비 하여
			a_nDccel = 100;
		}
	}
	

	double dTargetPos_mmc = (double)m_nDir_Move * a_dPos_um / m_dScaleFactor;	//pulse
	double dSpeed_mmc = a_dSpeed_umps / m_dScaleFactor;
	int    nAccel_mmc = a_nAccel / MMC_ACC_TIME_UNIT;
	int    nDccel_mmc = a_nDccel / MMC_ACC_TIME_UNIT;

	if(dTargetPos_mmc > m_dPosLimitPulse )
	{
		return ERR_MOT_INTERLOCK_IS_POSI_SW_LMT;
	}
	if(dTargetPos_mmc < m_dNegLimitPulse)
	{
		return ERR_MOT_INTERLOCK_IS_NEGA_SW_LMT;
	}
		
	double dEncoder = GetCurrentPos();
	double	dMovePos_mm = a_dPos_um / 1000.0;
	SaveMotorMoveLog(dTargetPos_mmc, dMovePos_mm, 0, dEncoder);

	m_dTarget_pos = a_dPos_um;	//이동할 위치
	m_dLast_pos = a_dPos_um;



	// test hand z축 target pos 0 입력으로 인한 충돌 발생, 재발 장비를 위한 인터락 코드 추가. 
	if( m_nAxisNumPhy == 18 || m_nAxisNumPhy == 21 ) {
		if( dMovePos_mm == 0.0 ) {
			return ERR_MOT_INTERLOCK_MOT_BD_API_FAIL;
		}
	}

	if( m_cbInterLock_Move != NULL ) {
		int nInterLockErr = m_cbInterLock_Move( a_dPos_um, a_dSpeed_umps, a_nAccel, a_nDccel );
		if( nInterLockErr != 0 ) {
			return nInterLockErr;
		}
	}



	//TRACE(("Ax:%d , Target:%.03f , Spd : %.3f , Acc : %d , Dec %d\n"), m_nAxisNumPhy, dTargetPos_mmc, dSpeed_mmc, nAccel_mmc, nDccel_mmc);
	int nApiErr = start_ts_move(m_nAxisNumPhy, dTargetPos_mmc, dSpeed_mmc, nAccel_mmc, nDccel_mmc);
	if( nApiErr != MMC_OK ){
		return ERR_MOT_INTERLOCK_MOT_BD_API_FAIL;
	}

	return 0;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : R_Move
  PARAMS   : move_pos = 움직이고자 하는 위치		[um    ] (미크론 단위로 scale_factor로 나누어야 실제 pulse값이 됨)
             speed    = 움직이려는 축의 속도		[um/sec]			 
  RETURN   :  0       = OK
              1       = Board Init Fail or 해당 축 disable
             -1       = Origin이 안 잡힌 상태
			 -2       = Servo가 On이 안되어 있는 상태
			 -3       = AMP FAULT 상태
			 -4       = Limit를 벗어나는 상태
			 -5       = Limit Pulse를 넘는 상태 ( 모터 RPM Over )
  COMMENTS : 현재 위치부터 설정 값 만큼(move_pos) 이동 한다.
==============================================================================*/
int OneAxis_MMC::R_Move(double a_dPos, double a_dSpeed)
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1;
	}
	if(m_nOri_Flag != eMotorOrgFlag_OK){
		return -1; 
	}
	if(!GetServoStat()){
		return -2;
	}
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1;
	}

	double dTargetPos = a_dPos + GetCurrentPos();
	if(dTargetPos > m_dSwLimit_Posi || dTargetPos < m_dSwLimit_Nega){ // 움직이고자 하는 위치가 s/w limit를 벗어나는 위치인지를 확인 
		return -4;
	}

	ClearStatus();
	int r_val = axis_source(m_nAxisNumPhy);

	if((r_val & ST_AMP_FAULT)){
		LogMtStrage(m_nAxisNumPhy, a_dPos, a_dSpeed);
		m_nOri_Flag = eMotorOrgFlag_NG;
		return -3;
	}

	if(a_dSpeed<=0){
		a_dSpeed = m_dCmdVelocity;
	}

	if( m_cbInterLock_Move != NULL ){
		int nInterLockErr = m_cbInterLock_Move(dTargetPos, a_dSpeed, m_nCmdAcc, m_nCmdAcc);
		if( nInterLockErr != 0 ){
			return nInterLockErr;
		}
	}

	Sleep(5);

	if(g_pMotion_Cont_Imp->GetInitBoardStatus()){
		double dTargetPos_mmc	= 0.0;
		double dMovePos			= a_dPos / 1000.0;
		double dSpeed_mmc       = a_dSpeed / m_dScaleFactor;
		int    nAccel_mmc       = m_nCmdAcc / MMC_ACC_TIME_UNIT;

// 		if( m_nEncoderType == eEncoderType_4Ch_NoPulseDir ){
// 			dTargetPos_mmc = (double)m_nDir_Move * a_dPos / (double)m_dScaleFactor;
// 		}else{			
			dTargetPos_mmc = (double)m_nDir_Move *(double)m_nDir_Pulse * a_dPos / (double)m_dScaleFactor;
//		}
					

		if(dTargetPos_mmc > m_dPosLimitPulse || dTargetPos_mmc < m_dNegLimitPulse){
			return -5;
		}


		double dCur_pos = GetCurrentPos();
		SaveMotorMoveLog(dTargetPos_mmc, dMovePos, 1, dCur_pos);
		
		m_dTarget_pos = dCur_pos + a_dPos;		

		m_dLast_pos = m_dTarget_pos;

		start_r_move(m_nAxisNumPhy, dTargetPos_mmc, dSpeed_mmc, nAccel_mmc);
		//TRACE("start_rs_move(%d, %f, %f, %d)\n",m_nAxisNumPhy, dTargetPos_mmc, dSpeed_mmc, nAccel_mmc);
	}
	return 0;
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : Origin
  PARAMS   : None
  RETURN   :  0       = Origin 진행 중.
             -1       = Board Init Fail or 해당 축 disable or Origin이 잡혀 있는 상태
			 0>       = Error
                        Origin 시 문제가 있는 상태
			            Servo가 On이 안되어 있는 상태
			            AMP FAULT 상태
			            Limit를 벗어나는 상태
			            Limit Pulse를 넘는 상태 ( 모터 RPM Over )
  COMMENTS : 현재 위치부터 설정 값 만큼(move_pos) 이동 한다.
==============================================================================*/
int OneAxis_MMC::Origin(int nRound)
{
	CString strLog = _T("");
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		strLog.Format(_T("Axis = %d Board Status || Motor Disable Error"), m_nAxisNumPhy);
		m_MtLog.LOG(strLog);
		return -1;
	}

	if(m_nOri_Flag == eMotorOrgFlag_OK  || m_nOri_Flag == eMotorOrgFlag_NG){
		strLog.Format(_T("Axis = %d m_nOri_Flag Error"), m_nAxisNumPhy);
		m_MtLog.LOG(strLog);
		return -1;
	}

	int r_val = axis_source(m_nAxisNumPhy);
	if((r_val & ST_AMP_FAULT)){
		LogMtStrage(m_nAxisNumPhy);
		m_nOri_Flag = eMotorOrgFlag_NG;
		return ERR_MOT_INTERLOCK_IS_AMP_FAULT;
	}

	if(!GetServoStat() && m_nOri_Step==0){
		strLog.Format(_T("Axis = %d GetServoStat && m_nOri_Flag ==0"), m_nAxisNumPhy);
		m_MtLog.LOG(strLog);
		SetStop();	
		return ERR_MOT_INTERLOCK_IS_NOT_SERVO_ON;
	}

	if( m_cbInterLock_Org != NULL ){
		int nInterLockErr = m_cbInterLock_Org();
		if( nInterLockErr != 0 ){
			strLog.Format(_T("Axis = %d orgin interlock return = %d"), m_nAxisNumPhy, nInterLockErr);
			m_MtLog.LOG(strLog);
			return nInterLockErr;
		}
	}
	if (m_nLastOri_Step != m_nOri_Step) {
		m_nLastOri_Step = m_nOri_Step;
		strLog.Format(_T("Axis = %d step = %d"), m_nAxisNumPhy, m_nOri_Step);
		m_MtLog.LOG(strLog);
	}
	
	CString strInformation = _T("");

	double dPos = 0.0;
	int nMotorMoveRes = 0;

	//TRACE("MMC ORG [step:%d]\n", m_nOri_Step);
	switch(m_nOri_Step)
	{
	case 0:
		{
			strInformation.Format(_T("Axis = %s ,Origin Start Encoder :  %f"), m_szAxisName, GetCurrentPos());
			m_EncoderLog.LOG(strInformation);

			m_nLastOri_Step = m_nOri_Step;
			ClearStatus();
			FramesClear();	
			m_nOri_Step = 10;
			SetStopRate(1);		
		}break;
	case 10:								// actuator initial
		{
			m_nOri_Step = 20;
		}break;
	case 20:
		{
			if(GetHomeSensor() != TRUE){	// 처음 위치가 home sensor가 감지되지 않은경우 
				FramesClear();
				m_clockStart = clock();
				m_nOri_Step = 22;
			}
			else{							// 처음 위치가 home sensor가 감지된 경우 
				m_nOri_Step = 40;
			}
		}break;
	case 22:
		{
			if(TimeCheck(500)==1)
			{	// frames_clear 수행후 delay가 필요함. // 본 명령은 반드시 frames_clear후에 사용.
				// 외부에서 지정한 원점복귀 속도로 home sensor방향으로 이동 
				if(ERR_NO_ERROR != V_Move(m_nDir_Ori * m_dCmdVelocity_Ori, m_nCmdAcc_Ori))
				{
					strLog.Format(_T("Axis = %d  step = %d v_move Error"), m_nAxisNumPhy, m_nOri_Step);
					m_MtLog.LOG(strLog);

					m_nOri_Flag = eMotorOrgFlag_NG;
					m_nOri_Step = 0;
					return ERR_MOT_INTERLOCK_IS_AMP_FAULT;
				}
				SetHome(STOP_EVENT);
				m_nOri_Step = 30;
			}
		}break;
	case 30:
		{
			if(MotionDone()==1)
			{
				if(GetHomeSensor() == TRUE)
				{
					SetHome(NO_EVENT);
					ClearStatus();
					m_nOri_Step = 40;
				}
				else
				{
					m_nOri_Step = 0;
					m_nOri_Flag = eMotorOrgFlag_NG;
					return ERR_MOT_INTERLOCK_IS_AMP_FAULT;
				}
			}
			else if(MotionDone()==-1)
			{
				m_nOri_Step = 0;
				m_nOri_Flag = eMotorOrgFlag_NG;
				return ERR_MOT_INTERLOCK_IS_AMP_FAULT;
			}
		}break;
	case 40:
		{
			if(GetHomeSensor() == TRUE){
				FramesClear();
				m_clockStart = clock();
				m_nOri_Step = 45;
			}
		}break;	
	case 45:
		{
			if(TimeCheck(500)==1)
			{
				if(ERR_NO_ERROR != V_Move(-m_nDir_Ori * 125, 10))//if(-1 == V_Move((-dir * ori_vel)/4, 20))
				{
					strLog.Format(_T("Axis = %d  step = %d v_move Error"), m_nAxisNumPhy, m_nOri_Step);
					m_MtLog.LOG(strLog);
					m_nOri_Flag = eMotorOrgFlag_NG;
					m_nOri_Step = 0;
					return ERR_MOT_INTERLOCK_IS_AMP_FAULT;
				}
				m_nOri_Step = 50;
			}
		}break;
	case 50:
		{
			if(GetHomeSensor() != TRUE){
				m_clockStart = clock();
				m_clockFinish = clock();
				m_nOri_Step = 55;
			}
		}break;
	case 55:
		{
			if(TimeCheck(30)==1){ //hyb 시간 줄임 500 -> 50 -> 30
				SetStop();
				m_clockStart = clock();
				m_nOri_Step = 57;
			}
		}break;
	case 57:
		{
			if(TimeCheck(500)==1){
				ClearStatus();
				FramesClear();
				m_clockStart = clock();
				m_nOri_Step = 60;
			}
		}break;
	case 60:
		{
			if(TimeCheck(500)==1){
				if(ERR_NO_ERROR != V_Move(m_nDir_Ori * 125, 10))//if(-1 == V_Move((dir * ori_vel)/8, 10))
				{
					strLog.Format(_T("Axis = %d  step = %d v_move Error"), m_nAxisNumPhy, m_nOri_Step);
					m_MtLog.LOG(strLog);
					m_nOri_Flag = eMotorOrgFlag_NG;
					m_nOri_Step = 0;
					return ERR_MOT_INTERLOCK_IS_AMP_FAULT;
				}
				SetHome(STOP_EVENT);
				m_nOri_Step = 70;
			}
		}break;
	case 70:
		{
			if(MotionDone()==1)
			{
				if(GetHomeSensor() == TRUE)
				{
					SetHome(NO_EVENT);
					ClearStatus();
					FramesClear();
					m_clockStart = clock();
					m_nOri_Step = 80;

				strInformation.Format(_T("Axis = %s ,Origin Done Encoder :  %f"), m_szAxisName, GetCurrentPos());
				m_EncoderLog.LOG(strInformation);

				}
				else
				{
					m_nOri_Flag = eMotorOrgFlag_NG;
					m_nOri_Step = 0;
					return ERR_MOT_INTERLOCK_IS_AMP_FAULT;
				}
			}
			else if(MotionDone()==-1)
			{
				m_nOri_Step = 0;
				m_nOri_Flag = eMotorOrgFlag_NG;
				return ERR_MOT_INTERLOCK_IS_AMP_FAULT;
			}
		}break;
	case 80:
		{
			if(TimeCheck(1000)==1)
			{
				strInformation.Format(_T("Axis = %s ,Origin End Encoder :  %f"), m_szAxisName, GetCurrentPos());
				m_EncoderLog.LOG(strInformation);

				m_nOri_Step = 0;
				m_nOri_Flag = eMotorOrgFlag_OK;
				SetCommandPos(0.0);
				SetCurrentPos(0.0);

				m_dDistanceRate = 0.0;

				m_dBefore_BacklashPos = 0.0;
				m_dAfter_BacklashPos = 0.0;

				if (nRound == 0 && NeedDualOriginChecking() == TRUE)
				{
					dPos = m_dServo_Pulse * m_dScaleFactor;
					nMotorMoveRes = MoveWhile(dPos, TEACH_MOVE_SPD_LOW, TEACH_MOVE_ACC_LOW, TEACH_MOVE_DEC_LOW);

					if (nMotorMoveRes != -1)
					{
						return 0;
					}
				}				

				return -1;
			}
		}break;
	default:
		break;
	}	
	
	return 0;
}
/*=============================================================================
  AUTHOR   : 김선진
  NAME     : OriginWhile
  PARAMS   : None
  RETURN   : 0   = 해당 축 사용 안함 or Board open fail or 원점 체크 완료.
			 0>  = 원점 잡기 실패
  COMMENTS : 해당 축의 원점 잡기 ( motion done 확인 )
==============================================================================*/
int OneAxis_MMC::OriginWhile()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return ERR_NO_ERROR;
	}

	SetSWLimit(m_dSwLimit_Posi, m_dSwLimit_Nega, TRUE);

	m_bOri_Stop = FALSE;

	for(int nOriginRetry = 0; nOriginRetry < 2; nOriginRetry++)
	{
		m_nOri_Flag   = eMotorOrgFlag_ING;	//다시 원점을 잡을 경우는 무조건 셋팅 한다.
		m_nOri_Step   = 0;
		m_dTarget_pos = 0;

		do
		{
			MSG message;
			if(::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&message);
				::DispatchMessage(&message);
			}
			NEXT;

			if( m_bOri_Stop == TRUE ){
				return 0;
			}

			int nOriResult = Origin(nOriginRetry);
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
		}while(m_nOri_Flag == eMotorOrgFlag_ING);
	}
	SetSWLimit(m_dSwLimit_Posi, m_dSwLimit_Nega, FALSE);

	return ERR_NO_ERROR;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-24]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
BOOL OneAxis_MMC::IsOrgCompleted()
{
	if( m_nOri_Flag == eMotorOrgFlag_OK )
		return TRUE;
	return FALSE;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : Origin
  PARAMS   : None
  RETURN   :  dCalcPos= 목표 위치에 대한 position 값 [um]
              1       = Board Init Fail or 해당 축 disable or Origin이 잡혀 있는 상태
             
  COMMENTS : 목표 위치에 대한 position 값 확인.
==============================================================================*/
double OneAxis_MMC::GetCommandPos()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1;
	}

	double dPos = 0.0;
	if(g_pMotion_Cont_Imp->GetInitBoardStatus()){
		get_command(m_nAxisNumPhy, &dPos);	//축의 목표위치를 읽어 들인다.
	}

	double dCalcPos = 0.0;

//	if( eEncoderType_4Ch_NoPulseDir == m_nEncoderType){
		//dPos /= MMC_MULTIPLICATION;
		dCalcPos = (double)m_nDir_Move * dPos * m_dScaleFactor;
// 	}else{
// 		dPos /= MMC_MULTIPLICATION;
// 		dCalcPos = (double)m_nDir_Move * (double)m_nDir_Pulse * dPos * m_dScaleFactor;
// 	}

	return dCalcPos;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : MotionDone
  PARAMS   : None
  RETURN   :  dCalcPos = 목표 위치에 대한 position 값
              1        = 해당 동작 완료 or 해당 축 사용 안함 or Board open fail
             -1        = AMP Fault 인 경우
  COMMENTS : 해당 축이 목표하는 위치에 도착 완료 확인.
==============================================================================*/
int OneAxis_MMC::MotionDone()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1;
	}

	//int r_val = motion_done( m_nAxisNumPhy );
	int r_val  = axis_done(m_nAxisNumPhy); // axis_done이 motor driver의 in position 신호까지 확인하므로 motion_done -> axis_done 으로 변경.
	int rr_val = axis_source(m_nAxisNumPhy);	

	if((rr_val & ST_AMP_FAULT))
	{
		LogMtStrage(m_nAxisNumPhy);
		m_nOri_Flag = eMotorOrgFlag_NG;

		return -1;
	}

	if(r_val==1)
		m_dActTime = m_tm.CompareTime();

	return r_val;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : GetaxisdoneORfree
  PARAMS   : None
  RETURN   :  0        = 움직이는 상태
              1        = 동작 완료 or 해당 축 Ampfault
  COMMENTS : 해당 축의 동작이 완료 되었거나, AmpFault 상태 일 경우 (memory에서 Door 열기 위한 체크 인듯..)
==============================================================================*/
int OneAxis_MMC::GetaxisdoneORfree()
{
	if(GetServoStat())
	{
		if(MotionDone())
			return 1;
		else
			return 0;
	}
	return 1;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : AxisDone
  PARAMS   : None
  RETURN   :  0        = 움직이는 상태
              1        = 해당 축 동작 완료 or 해당 축 사용 안함 or Board open fail
			 -1        = 해당 축 Timeover or Amp fault
  COMMENTS : 해당 축의 동작이 완료 되었거나, AmpFault 상태 일 경우 (memory에서 Door 열기 위한 체크 인듯..)
==============================================================================*/
int OneAxis_MMC::AxisDone()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1;
	}

	double encoder = 0.0, st_encoder = 0.0, ed_encoder = 0.0;
	double dInpositionValue = (MOTOR_INPOSITION);// / 2.0);
	int    cd_encoder = 0;
	int    encoder_retry = 0;

	//CTimerCheck tt(30,TRUE);

	int r_val  = MotionDone();
	//st_encoder = GetCurrentPos();

	int rr_val = axis_source(m_nAxisNumPhy);
	if((rr_val & ST_AMP_FAULT))
	{
		SetStop();		
		ClearStatus();
		m_nOri_Flag = eMotorOrgFlag_NG;

		LogMtStrage(m_nAxisNumPhy);
		return -1;

	}

	//if( (encoder > m_dTarget_pos - err_pos) && (encoder < m_dTarget_pos + err_pos))
	if( r_val == 1 )
	{
		FramesClear();

		encoder    = GetCurrentPos();
		double dPosErrAbs = fabs(encoder - m_dTarget_pos);
		if( (dPosErrAbs < dInpositionValue) )// && (r_val==1) )
		{
			//r_val = 1;
			return 1;
		}
		else
		{
			Sleep(10);
			ClearStatus();
			Sleep(10);
			Move(m_dTarget_pos, m_dCmdVelocity*0.5, m_nCmdAcc, m_nCmdDcc);
			Sleep(10);
		}
	}

	return 0;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : MoveWhile
  PARAMS   : move_pos = 움직이고자 하는 위치 (미크론 단위로 scale_factor로 나누어야 실제 pulse값이 됨)
             speed    = 움직이려는 축의 속도      (PPS 단위)
             accel    = 움직이려는 축의 가속도
             dccel    = 움직이려는 축의 감속도
  RETURN   :  1       = Board Init Fail or 해당 축 disable or OK
             -1       = AMP FAULT 상태 or Move Time Over
  COMMENTS : 해당 축을 움직이려는 함수 ( Motion Done 확인 불필요 )
==============================================================================*/
int OneAxis_MMC::MoveWhile(double a_dPos, double a_dSpeed, int a_nAccel, int a_nDccel)
{
	double encoder    = 0.0;
	double err_pos    = 0.0;
	double st_encoder = 0.0, ed_encoder = 0.0;
	int cd_encoder    = 0;
	int encoder_retry = 0;
	int err_retry     = 0;

	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1;
	}

	int r_val = 0;
	CTimerCheck tt(30,TRUE);

	st_encoder = GetCurrentPos();

	for(int nRetryCnt = 0; nRetryCnt < 10; nRetryCnt++)
	{
		int nMoveResult = Move(a_dPos, a_dSpeed, a_nAccel, a_nDccel);

		while(nMoveResult == 0)
		{
			r_val = MotionDone();

			if(r_val == -1)	// AMP FAULT
			{
				LogMtStrage(m_nAxisNumPhy, a_dPos, a_dSpeed);
				SetStop();
				ClearStatus();

				return -1;
			}

			if(r_val == 1)	         // Motion Done
			{
				break;
			}

			if(tt.CheckOverTime())	// Time Over Check
			{
				LogMtStrage(m_nAxisNumPhy, a_dPos, a_dSpeed);
				SetStop();
				ClearStatus();

				return -1;
			}

			NEXT;
		}		

		err_pos = (MOTOR_INPOSITION);// / 2.0); // by jhjo[150424] Inposition 누락된 부분 수정		
		encoder = GetCurrentPos();

		if( (encoder > a_dPos - err_pos) && (encoder < a_dPos + err_pos))
		{
			return 1;
		}		

		ClearStatus();
	}	

	SetStop();
	ClearStatus();

	return -1;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : R_MoveWhile
  PARAMS   : pos      = 움직이고자 하는 거리
             speed    = 움직이려는 축의 속도      (PPS 단위)
  RETURN   : nResult(-1) = Ampfault 및 이동 불가 사항
             nResult( 1) = 정상 동작 완료 or 해당 축 사용 안함 or Board open fail
  COMMENTS : 현재 위치에서 pos 입력치 만큼 이동 시키는 함수.
==============================================================================*/
int OneAxis_MMC::R_MoveWhile(double a_dPos, double a_dSpeed)
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1;
	}

	double Goal = a_dPos + GetCurrentPos();	// 거리 + 현재위치
	int nResult = MoveWhile(Goal,a_dSpeed);

	return nResult;
}

int OneAxis_MMC::V_MoveCheckInterlock(double speed, int accel)
{
	if (m_cbInterLock_Jog != NULL) {
		int nInterLockErr = m_cbInterLock_Jog(speed, accel, accel);
		if (nInterLockErr != ERR_NO_ERROR) {
			V_MoveStop();
			return nInterLockErr;
		}
	}

	return 0;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : V_Move
  PARAMS   : pos      = 움직이고자 하는 거리
             speed    = 움직이려는 축의 속도      (PPS 단위)
  RETURN   : -1       = Amp fault or Servo Off
              0       = OK
			  1       = 해당 축 사용 안함 or Board open fail
  COMMENTS : 현재 위치에서 pos 입력치 만큼 이동 시키는 함수.
==============================================================================*/
int OneAxis_MMC::V_Move(double a_Speed, int a_nAccel)
{
	CString strLog = _T("");

	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		strLog.Format(_T("Axis = %d Board Status || Motor Disable Error"), m_nAxisNumPhy);
		m_MtLog.LOG(strLog);
		return 1;
	}
	
	if(!GetServoStat()){
		strLog.Format(_T("Axis = %d GetServostat Error"), m_nAxisNumPhy);
		m_MtLog.LOG(strLog);
		return -1;
	}

	int r_val = axis_source(m_nAxisNumPhy);
	if((r_val & ST_AMP_FAULT)){
		m_nOri_Flag = eMotorOrgFlag_NG;
		LogMtStrage(m_nAxisNumPhy, 0, a_Speed);
		return ERR_MOT_INTERLOCK_IS_AMP_FAULT;
	}

	if (m_nOri_Flag == eMotorOrgFlag_OK)
	{
		if (m_cbInterLock_Jog != NULL) {
			int nInterLockErr = m_cbInterLock_Jog(a_Speed, a_nAccel, a_nAccel);
			if (nInterLockErr != ERR_NO_ERROR) {
				V_MoveStop();
				strLog.Format(_T("Axis = %d Interlock Error"), m_nAxisNumPhy);
				m_MtLog.LOG(strLog);
				return nInterLockErr;
			}
		}

		double dCurPos = GetCurrentPos();
		if ((dCurPos > GetSwLimitPosi() && a_Speed > 0) || (dCurPos < GetSwLimitNega() && a_Speed < 0)) {
			strLog.Format(_T("Axis = %d Limit pos error"), m_nAxisNumPhy);
			m_MtLog.LOG(strLog);
			return ERR_MOT_INTERLOCK_IS_POSI_SW_LMT;
		}
	}
	
	if(a_nAccel==0){
		a_nAccel = m_nCmdAcc;
	}

	ClearStatus();

	double dTargetPos = 0.0;

//	if( eEncoderType_4Ch_NoPulseDir == m_nEncoderType){
		dTargetPos = (double)m_nDir_Move * a_Speed;
// 	}else{
// 		dTargetPos = (double)m_nDir_Move * (double)m_nDir_Pulse * a_Speed;
// 	}
	
	double dEncoder = GetCurrentPos();
	SaveMotorMoveLog(dTargetPos, dTargetPos, 2, dEncoder);
	v_move(m_nAxisNumPhy, dTargetPos, a_nAccel);
	
	return 0;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : V_MoveStop
  PARAMS   : None
  RETURN   : 0       = OK
			 1       = 해당 축 사용 안함 or Board open fail
  COMMENTS : v_move를 Stop
==============================================================================*/
int OneAxis_MMC::V_MoveStop()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable)	
		return 1;
	
	double dEncoder = GetCurrentPos();
	SaveMotorMoveLog(0, 0, 2, dEncoder);
	v_move(m_nAxisNumPhy, 0, 0);
	
	//Sleep(10);
	//ClearStatus();
	//Sleep(10);
	//FramesClear();
	FramesClear();
	ClearStatus();
	Sleep(200);

	return 0;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : ServoOn
  PARAMS   : None
  RETURN   : 0       = OK or 이미 servo on 상태
			 1       = 해당 축 사용 안함 or Board open fail
  COMMENTS : Amp를 Enable 시키고 현재 위치를 다시 넣어준다.
==============================================================================*/
int OneAxis_MMC::ServoOn()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1;
	}

	if(GetServoStat() == TRUE){	// Servo Status On is return 0
		return 0; 
	}

	set_amp_enable(m_nAxisNumPhy, TRUE);
	//m_MotorLogger.LOG("%s[%d] Amp Eanble True", m_szAxisName, m_nAxisNumPhy);

	//현재의 위치를 다시 넣는다.
	SetCommandPos(GetCurrentPos());
	
	return 0;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : ServoOff
  PARAMS   : None
  RETURN   : 0       = OK 
			 1       = 해당 축 사용 안함 or Board open fail
  COMMENTS : Amp를 disable 시킨다.
==============================================================================*/
int OneAxis_MMC::ServoOff()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1;
	}
	
	set_amp_enable(m_nAxisNumPhy, FALSE);
	//m_MotorLogger.LOG("%s[%d] Amp Enable False", m_szAxisName, m_nAxisNumPhy);

	return 0;
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : Reset
  PARAMS   : None
  RETURN   : 0       = OK 
			 1       = 해당 축 사용 안함 or Board open fail
  COMMENTS : Mmc Board 의 한축을 리셋한다.
==============================================================================*/
int OneAxis_MMC::AlarmReset()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1;
	}

	//정지 명령....
	if(g_pMotion_Cont_Imp->GetInitBoardStatus())
	{
		// amp setting's
		set_amp_fault(m_nAxisNumPhy, STOP_EVENT);
		set_amp_fault_level(m_nAxisNumPhy, HIGH);
		set_amp_reset_level(m_nAxisNumPhy, m_bResetLevel);
		SetHome(NO_EVENT);

		// amp clear
		amp_fault_reset(m_nAxisNumPhy);
		Sleep(10);
		amp_fault_set(m_nAxisNumPhy);

		// 본 output을 reset해야 motor가 동작함.
		// 각보드의 IO Base Address 가 32 씩 증가 하므로 아래의 연산을 해야 함.
		int portion = m_nAxisNumPhy / 8;	    // 몫
		int remainder = m_nAxisNumPhy % 8;	// 나머지
		int result = (portion*32) + remainder;
		reset_bit(result);

		ClearStatus();

		//m_MotorLogger.LOG("%s[%d] Axis Motor Reset", m_szAxisName, m_nAxisNumPhy);
	}

	return 0;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : Reset
  PARAMS   : None
  RETURN   : 0       = OK 
			 1       = 해당 축 사용 안함 or Board open fail
			-1       = 해당 축이 Origin이 잡혀 있지 않을 경우
  COMMENTS : 현재 amp가 on or off 인지를 리턴..
==============================================================================*/
BOOL OneAxis_MMC::GetServoStat()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable)	return 1;
	
	short res=0;
	get_amp_enable(m_nAxisNumPhy, &res);

	//서보가 on된경우에는 AmpFault를 확인한다.
	if(res)
	{//만일 AmpFault 이면 서보를 off 한다.
		int rr_val = axis_source(m_nAxisNumPhy);

		if((rr_val & ST_AMP_FAULT))
		{
			set_amp_enable(m_nAxisNumPhy, FALSE);
			m_nOri_Flag = eMotorOrgFlag_NG;
			//m_MotorLogger.LOG("%s[%d] Amp Enable False", m_szAxisName, m_nAxisNumPhy);
			res = 0;
		}
	}

	return (BOOL)res;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : GetHomeSensor
  PARAMS   : None
  RETURN   : TRUE   = 해당 축 사용 안함 or Board open fail or Home sensor가 감지 됨.    
			 FALSE  = Home sensor 감지 안됨.
  COMMENTS : Home Sensor 가 감지 되는지 확인.
==============================================================================*/
BOOL OneAxis_MMC::GetHomeSensor()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1;
	}
	BOOL bHomeSwitch = (BOOL)home_switch(m_nAxisNumPhy);

	return bHomeSwitch;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : GetPositiveSensor
  PARAMS   : None
  RETURN   : TRUE   = 해당 축 사용 안함 or Board open fail or +Limit가 감지 됨
			 FALSE  = +Limit가 감지 안됨.
  COMMENTS : + 방향의 Limit Sensor의 상태를 읽어온다.
==============================================================================*/
BOOL OneAxis_MMC::GetPositiveSensor()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1;
	}
	BOOL bPosSwitch = (BOOL)pos_switch(m_nAxisNumPhy);

	return bPosSwitch;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : GetNegativeSensor
  PARAMS   : None
  RETURN   : TRUE   = 해당 축 사용 안함 or Board open fail or -Limit가 감지 됨
			 FALSE  = -Limit가 감지 안됨.
  COMMENTS : - 방향의 Limit Sensor의 상태를 읽어온다.
==============================================================================*/
BOOL OneAxis_MMC::GetNegativeSensor()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1;
	}
	BOOL bPosSwitch = (BOOL)neg_switch(m_nAxisNumPhy);

	return bPosSwitch;	
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : ClearStatus
  PARAMS   : None
  RETURN   : TRUE   = 해당 축 사용 안함 or Board open fail or 해당 축 EVENT 해제 완료.
			 FALSE  = 해당 축 EVENT 해제 실패
  COMMENTS : 해당 축의 발생된 EVENT를 해제 하고, 다음명령부터 실행 한다
==============================================================================*/
BOOL OneAxis_MMC::ClearStatus()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return TRUE;
	}

	int recheck_cnt = 0;
	int nStatus = 0;
	
	for( int nRetryCnt = 0; nRetryCnt < 10; nRetryCnt++ )
	{
		nStatus = axis_state( m_nAxisNumPhy );
		if( nStatus != NO_EVENT )	// 축의 현재 EVENT 발생상태를 확인 ( 0 == NO_EVENT )
		{
			int ret = clear_status( m_nAxisNumPhy );
			//TRACE("Event Clear : %d Axis , Status is : %d\n", m_nAxisNumPhy , nStatus);

			//m_MotorLogger.LOG( "%s[%d] Axis Clear Status", m_szAxisName, m_nAxisNumPhy );

			if( ret != MMC_TIMEOUT_ERR )
			{
				return TRUE;
			}
			else
			{
				Sleep( 100 );
			}
		}
		else
		{
			return TRUE;
		}
	}

	return FALSE;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : SetStop
  PARAMS   : None
  RETURN   :	( 0 ) 정상 작업					///< lwy 20150819
				( 1 ) init 실패 or 축 사용 안함. ///< lwy 20150819
  COMMENTS : 지정된 축의 STOP_EVENT를 생성, 축의 이동을 멈춘다.
==============================================================================*/
int OneAxis_MMC::SetStop()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1 ;
	}
	int nRet = set_stop(m_nAxisNumPhy);
	
	//m_MotorLogger.LOG("%s[%d] Axis Set Stop", m_szAxisName, m_nAxisNumPhy);
	//AlarmReset();

	return nRet;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : SetStop
  PARAMS   : None
  RETURN   :	( 0 ) 정상 작업
				( 1 ) init 실패 or 축 사용 안함.
  COMMENTS : 지정된 축의 STOP_EVENT를 생성, 축의 이동을 멈춘다.
==============================================================================*/
int	OneAxis_MMC::SetSlowStop()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1 ;
	}

	//int nCmdDcc = 100;
	//if( m_nCmdDcc > 0 ){
	//	if( m_nCmdDcc > 500){
	//		nCmdDcc = 10;
	//	}else{
	//		nCmdDcc = 20;
	//	}
	//}

	SetStopRate(10/*nCmdDcc*/);
	int nRet = set_stop(m_nAxisNumPhy);

	//m_MotorLogger.LOG("%s[%d] Axis Set Slow Stop", m_szAxisName, m_nAxisNumPhy);
	//AlarmReset();

	return nRet;
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : SetStop
  PARAMS   : stop_acc = 감속 시간 (stop_acc * 10msec)
  RETURN   : 
  COMMENTS : 지정된 축의 STOP_EVENT 수행시 감속시간을 지정
==============================================================================*/
void OneAxis_MMC::SetStopRate(double stop_dcc)
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return;
	}

	set_stop_rate(m_nAxisNumPhy, (int)stop_dcc); //(stop_acc * 10msec)
	//m_MotorLogger.LOG("%s[%d] Axis Set Stop Rate", m_szAxisName, m_nAxisNumPhy);	
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : SetPos
  PARAMS   : posit = 목표 위치
  RETURN   : 
  COMMENTS : 축의 목표 위치를 지정
==============================================================================*/
void OneAxis_MMC::SetCommandPos(double a_dPos)
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){	
		return;
	}

	if(a_dPos != 0.0)
	{
//		if( m_nEncoderType == eEncoderType_4Ch_NoPulseDir ){
			a_dPos = (double)m_nDir_Move * a_dPos / m_dScaleFactor;
// 		}else{
// 			a_dPos = (double)m_nDir_Move * (double)m_nDir_Pulse * a_dPos / m_dScaleFactor;
// 		}
		
	}
	set_command(m_nAxisNumPhy, a_dPos);
	//m_MotorLogger.LOG("%s[%d] Set Command [%0.2f Pulse]", m_szAxisName, m_nAxisNumPhy, a_dPos);
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : FramesClear
  PARAMS   : 
  RETURN   : 
  COMMENTS :  Handler 에서 MMC보드의 Frame Buffer를 Clear
==============================================================================*/
void OneAxis_MMC::FramesClear()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return;
	}

	frames_clear(m_nAxisNumPhy);
	//m_MotorLogger.LOG("%s[%d] Axis Frame Clear", m_szAxisName, m_nAxisNumPhy);
}

/*=============================================================================
AUTHOR   : 류재훈 [2018-11-16]
NAME     :
PARAMS   : dspeed_umps = 이동 속도(umps)
RETURN   : 현재 속도에 해당하는 Backlash Offset 반환.
COMMENTS : 핸드/축/속도에 맞는 Offset 반환.
==============================================================================*/
double OneAxis_MMC::GetBacklashDataAboutSpeed( double dspeed_umps )
{
	int nSpeedRate = 0;
	// 현재 속도를 백분율로 계산. (0~100%)
	double dMax_speed_umps = GetSpeed();
	nSpeedRate = ( int )( ( dspeed_umps / dMax_speed_umps ) * 100.0 );

	const int nSpdMinIndex = 0;
	const int nSpdMaxIndex = 10;
	int nSpeedIdx = 0;

	// Spped Rate가 계산되어 넘어 오기 때문에 딱 맞지 않음
	if( nSpeedRate < 10 )
	{
		if( nSpeedRate % 10 > 7 )
			nSpeedRate = 10;
		else
			nSpeedRate = 5;
	}
	else {
		if( nSpeedRate % 10 > 7 )
		{
			nSpeedRate = nSpeedRate + ( 10 - ( nSpeedRate % 10 ) );
		}
		else
		{
			nSpeedRate = nSpeedRate - ( nSpeedRate % 10 );
		}
	}

	if( nSpeedRate == 5 ) {
		nSpeedIdx = nSpdMinIndex;
	}
	else {
		nSpeedIdx = nSpeedRate / 10;
	}

	if( nSpeedIdx > nSpdMaxIndex || nSpeedIdx <= 0 ) {
		nSpeedIdx = nSpdMinIndex;
	}

	double dBacklashOffset = m_vBacklashOffset_XY[ nSpeedIdx ];

	//if( fabs( dBacklashOffset ) > 1000.0 )
	//{
	//	// 과도한 Offset 방지. 1 mm 초과하는 경우 0 리턴.
	//	return 0.0;
	//}

	return dBacklashOffset;
}

double OneAxis_MMC::GetBacklashParameterData(double dMoveDistance, double dspeed_umps)
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

	nDistanceIdx = (int)( dMoveDistance / 10000.0 );
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

void OneAxis_MMC::SetSpeedBacklash( int nSpdIdx, double dValue )
{
	m_vBacklashOffset_XY[ nSpdIdx ] = dValue;
}

void OneAxis_MMC::SetParameterBacklash(int nDistanceIdx, int nSpdIdx, double dValue)
{
	m_dBacklashParameterOffset_XY[nDistanceIdx][nSpdIdx] = dValue;
}

/*=============================================================================
AUTHOR   : 류재훈 [2018-11-16]
NAME     :
PARAMS   : dMovePos = 목표 위치 / dPPS = 이동 속도(pps)
RETURN   : 상황에 맞는 Backlash Offset 반환.
COMMENTS : '-' 방향으로 이동하는 경우에만 Backlash Offset 적용함.
이동거리가 100 mm 이하인 경우 백분율 적용 함.
==============================================================================*/
double OneAxis_MMC::GetBacklashOffset_TestPpY(double dMovePos, double dPPS)
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

double OneAxis_MMC::GetBacklashOffset(double dMovePos, double dPPS)
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

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : SetHome
  PARAMS   : active
			  -> NO_EVENT    (0) = ignore a condition
              -> STOP_EVENT  (1) = generate a stop event
              -> E_STOP_EVENT(2) = generate an e_stop event
              -> ABORT_EVENT (3) = disable PID control, and disable the amplifier
  RETURN   : 
  COMMENTS :  Home Sensor를 만났을때 동작할 EVENT를 지정
==============================================================================*/
void OneAxis_MMC::SetHome(int active)
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return;
	}

	set_home(m_nAxisNumPhy, active);
	//m_MotorLogger.LOG("%s[%d] Axis Set Home [Active = %d]", m_szAxisName, m_nAxisNumPhy, active);
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : GetEncoder
  PARAMS   : None
  RETURN   : 1         = 해당 축 사용 안함 or Board open fail
             dCalcPos  = 위치의 Encoder 값	[um]
  COMMENTS :  축의 실제위치(현재위치)를 읽어 들인다
==============================================================================*/
double OneAxis_MMC::GetCurrentPos()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1;
	}

	double dPos = 0.0;
	if(g_pMotion_Cont_Imp->GetInitBoardStatus())
		get_position(m_nAxisNumPhy, &dPos);

	double dCalcPos = 0.0;

//	if( m_nEncoderType == eEncoderType_4Ch_NoPulseDir ){
		dPos /= MMC_MULTIPLICATION;
		dCalcPos = (double)m_nDir_Pulse * dPos * m_dScaleFactor;
// 	}else{
// 		dPos /= MMC_MULTIPLICATION;
// 		dCalcPos = (double)m_nDir_Move * (double)m_nDir_Pulse * dPos * m_dScaleFactor;
// 	}

	m_dBackDisplay_MtEncoder = dCalcPos;

	return dCalcPos;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : SetEncoder
  PARAMS   : pos : 위치
  RETURN   : 
  COMMENTS : 축의 실제위치 및 목표위치를 지정

  ** set_position(ax, 0) 할 경우 command와 position이 모두 '0'으로 되나, 
     '0'이 아닌 값으로 하게 될 경우 command는 지정한 값이 되지만, 
	 position은 지정값의 4체배된 값이 표시된다. 
	 서보가 입력대비 출력되는 엔코더 값이 4체배의 연산관계를 가지지 않는 경우는 문제의 소지가 있으므로 "주의" 해야 합니다.
==============================================================================*/
void OneAxis_MMC::SetCurrentPos(double pos)
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return;
	}

	if(pos != 0.0)
	{
// 		if( m_nEncoderType == eEncoderType_4Ch_NoPulseDir ){
// 			pos = (double)m_nDir_Pulse  * pos / m_dScaleFactor;
// 			pos *= MMC_MULTIPLICATION;
// 		}else{
			pos = (double)m_nDir_Move * (double)m_nDir_Pulse * pos / m_dScaleFactor;
			pos *= MMC_MULTIPLICATION;
//		}
	}
	
	set_position(m_nAxisNumPhy, pos);
	//m_MotorLogger.LOG("%s[%d] Axis Set Encoder [%0.2f Pulse]", m_szAxisName, m_nAxisNumPhy, pos);
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : SetEncoder
  PARAMS   : pos : 위치
  RETURN   : 1 : AMP Fault 
             0 : OK or 해당 축 사용 안함 or Board open fail
  COMMENTS : AMP Fault 상태 확인
==============================================================================*/
BOOL OneAxis_MMC::GetAmpFaultStat()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return FALSE;
	}

	int r_val = axis_source(m_nAxisNumPhy);

	if((r_val & ST_AMP_FAULT))
	{
		m_nOri_Flag = eMotorOrgFlag_NG;
		LogMtStrage(m_nAxisNumPhy);
		return TRUE;
	}
	return FALSE;
}

/*=============================================================================
  AUTHOR   : 이원용
  NAME     : GetErrorValue
  PARAMS   : None
  RETURN   : double
  COMMENTS : 현재 위치와 목표위치의 차이를 리턴.
==============================================================================*/
double		OneAxis_MMC::GetErrorValue()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1;
	}

	double dErrorValue;

	get_error( m_nAxisNumPhy, &dErrorValue );

	return dErrorValue;
}

// OneAxis virtual

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : GetVelocity
  PARAMS   : 
  RETURN   : vel = 해당 축의 motor parameter 설정 치 (pps)
  COMMENTS : Motor Parameter에 설정 된 speed
==============================================================================*/
double OneAxis_MMC::GetVelocity()
{
	return get_velocity( m_nAxisNumPhy );
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : GetAcc
  PARAMS   : 
  RETURN   : acc = 해당 축의 motor parameter 설정 치 (pps)
  COMMENTS : Motor Parameter에 설정 된 Accel
==============================================================================*/
int OneAxis_MMC::GetAcc()
{
	return m_nCmdAcc;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : GetDcc
  PARAMS   : 
  RETURN   : dcc = 해당 축의 motor parameter 설정 치 (pps)
  COMMENTS : Motor Parameter에 설정 된 Decel
==============================================================================*/
int OneAxis_MMC::GetDcc()
{
	return m_nCmdDcc;
}

// virtual OneAxis
void OneAxis_MMC::SetPulseDirection(int zdir, int pdir, int mdir, BOOL bEnType)
{
	m_nDir_Ori		= zdir;
	m_nDir_Pulse	= pdir;
	m_nDir_Move		= mdir;

	this->Set_Reset_Level(bEnType);
	//m_MotorLogger.LOG("%s[%d] Axis Reset Level [Encode Type = %d]", m_szAxisName, m_nAxisNumPhy, bEnType);
	/*
	m_EncoderType=1;
	*/
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : GetDcc
  PARAMS   : bType : 1 = HIGH, 0 = LOW
  RETURN   : dcc = 해당 축의 motor parameter 설정 치 (pps)
  COMMENTS : Motor Parameter에 설정 된 Decel
==============================================================================*/
void OneAxis_MMC::Set_Reset_Level(BOOL bType)
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return;
	}
	
	m_bResetLevel=bType;

	set_amp_reset_level(m_nAxisNumPhy, bType);
	//m_MotorLogger.LOG("%s[%d] Axis Amp Reset Level [Level = %d]", m_szAxisName, m_nAxisNumPhy, bType);
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : RepeatMove
  PARAMS   : move_pos = 움직일 거리
             rspeed   = 속도
			 racc     = 가속
			 rdcc     = 감속
  RETURN   : 1 : 해당 축 사용 안함 or Board open fail
            -1 : 원점 안잡힌 상태 or 움직일 수 없는 상황 or AMP fault or Limit 위치를 벗어난 상황
  COMMENTS : 설정 거리 만큼의 반복 동작을 위한 함수
==============================================================================*/
int OneAxis_MMC::RepeatMove(double move_pos, double rspeed,int racc,int rdcc)
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1;
	}

	if(m_nOri_Flag != eMotorOrgFlag_OK){
		return -1;
	}

	if(!GetServoStat())
	{
		return -1;
	}

	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1;
	}

	ClearStatus();

	int r_val = _source();

	if((r_val & ST_AMP_FAULT)){
		m_nOri_Flag = eMotorOrgFlag_NG;
		return -1;
	}

	// 움직이고자 하는 위치가 s/w limit를 벗어나는 위치인지를 확인
	if(move_pos > m_dSwLimit_Posi || move_pos < m_dSwLimit_Nega)
	{
		return -1;
	}

	if(rspeed<=0)
		rspeed = m_dCmdVelocity;

// 	if(racc<=0 && racc>500)
// 		racc=m_nCmdAcc;
// 
// 	if(rdcc<=0 && rdcc>500)
// 		rdcc= m_nCmdDcc;

	m_tm.StartTimer();	// Repeat Motion Done check time

	ASSERT(m_dScaleFactor != 0.0);

	double dTargetPos	= (double)m_nDir_Move * move_pos / m_dScaleFactor;
	double dMovePos	= move_pos / 1000.0;

	double dEncoder = GetCurrentPos();

	SaveMotorMoveLog(dTargetPos, dMovePos, 0, dEncoder);
	start_ts_move(m_nAxisNumPhy, dTargetPos, rspeed, racc ,rdcc);	

	return 0;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : _source
  PARAMS   : 
  RETURN   : 1 : 해당 축 사용 안함 or Board open fail
            ret: 1이면 정상
  COMMENTS : MMC보드로 부터 축의 현재상태를 읽어들인다, amp on/off 인지
==============================================================================*/
int OneAxis_MMC::_source()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1;
	}
	
	INT ret = axis_source(m_nAxisNumPhy);
	return ret;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : sethomelevel
  PARAMS   : 
  RETURN   : 1 : OK
  COMMENTS : Home Sensor의 Active Level 상태를 HIGH(1) 또는 LOW(0)로 지정한다
==============================================================================*/
int OneAxis_MMC::sethomelevel(BOOL bStatus)
{
	set_home_level(m_nAxisNumPhy, bStatus);
	//m_MotorLogger.LOG("%s[%d] Axis Home Level [Level = %d]", m_szAxisName, m_nAxisNumPhy, bStatus);
	return 1;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : CheckPowerOffStat
  PARAMS   : 
  RETURN   : FALSE : OK
             TRUE  : Amp Fault
  COMMENTS : servo 상태를 확인 and Servo의 전원이 꺼진것을 체크하여 org 하는 용도
==============================================================================*/
BOOL OneAxis_MMC::CheckPowerOffStat()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return FALSE;
	}

	int r_val = axis_source(m_nAxisNumPhy);//_source();	

	if((r_val & (ST_AMP_FAULT+ST_INPOSITION_STATUS))==(ST_AMP_FAULT+ST_INPOSITION_STATUS))
	{
		m_nOri_Flag = eMotorOrgFlag_NG;
		return TRUE;
	}
	return FALSE;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : GetSpeed
  PARAMS   : 
  RETURN   : 1 : mmc board init fail or 해당 축 disable
             speed : 저장 된 속도 값(pps)
  COMMENTS : parameter에 설정 된 속도값을 불러온다. (pps)
==============================================================================*/
double OneAxis_MMC::GetSpeed()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1;
	}

	double dSpeed = m_dCmdVelocity;
	return dSpeed;
}

/************************************************************************/
/* 프로그램 시작시에 한번만 호출 한다.                                  */
/************************************************************************/
// virtual OneAxis
BOOL OneAxis_MMC::InitBoard()
{
	return FALSE;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : Set_disable
  PARAMS   : bDisable = Motor disable 상태 인가에 대한 parameter
  RETURN   : 
  COMMENTS : 해당 축의 사용 유/무 setting
==============================================================================*/
void OneAxis_MMC::Set_disable(BOOL bDisable)
{
	m_bMotorDisable = bDisable;
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : IsDisable
  PARAMS   : 
  RETURN   : bDisable = Motor disable 상태 인가에 대한 parameter
  COMMENTS : 해당 축의 사용 유/무 setting
==============================================================================*/
BOOL OneAxis_MMC::IsDisable()
{
	return m_bMotorDisable;
}



/*=============================================================================
  AUTHOR   : 김선진
  NAME     : MotorStrageCheck
  PARAMS   : ST_MOTORSTATUS = 축에 대한 정보
  RETURN   : None
  COMMENTS : 서보모터 로그 and 충돌한 축에 정보 먼저(Target, Speed, 감속, 가속)  기록 나머지 축 정보도 기록
==============================================================================*/
void OneAxis_MMC::MotorStrageCheck(ST_MOTORSTATUS* lpstMotorData) // 
{
	/*
	CString strData;

	strData.Format("------------motor Jam Log start %s--------------", _MotorNameStr[lpstMotorData->iAxisNum]);
	g_CDailyProduct.SaveMotorErrLogFileString(strData);

	// by hyb [141011]: 서보모터 로그 표시 수정 <start>
	strData.Format("[T Pos %.2fmm] [E Pos %.2fmm] [cmd Pos %.2fmm] [Spd %.2f pps] [Acc %d] [Dcc %d] [Amp Fault %d]",
		lpstMotorData->dTarget/1000,lpstMotorData->dEncoder/1000,lpstMotorData->dCommand/1000,lpstMotorData->dMotorSpeed, lpstMotorData->dAcc, lpstMotorData->dDcc, lpstMotorData->bServoStatus);
	g_CDailyProduct.SaveMotorErrLogFileString(strData);

	// By jhjang [150227] Add Motor Amp Fault Log <start>
	strData.Format("Motor Status [in_Sequence : %d] [in_motion : %d] [in_position : %d] [frames_left : %d] [motion_done : %d] [axis_done : %d] [Command RPM : %d]"
		,lpstMotorData->iInsequence, lpstMotorData->iInmotion, lpstMotorData->iInposition, lpstMotorData->iFramesleft, lpstMotorData->iMotiondone, lpstMotorData->iAxisdone, lpstMotorData->sEncoderRPM);
	// By jhjang [150227] Add Motor Amp Fault Log <end>

	g_CDailyProduct.SaveMotorErrLogFileString(strData);

	// by hyb [141011]: 서보모터 로그 표시 수정 <end>

	strData.Format("------------motor Jam Log end %s--------------", _MotorNameStr[lpstMotorData->iAxisNum]);
	g_CDailyProduct.SaveMotorErrLogFileString(strData);
	*/
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : AmpFaultCheckFunction
  PARAMS   : None
  RETURN   : 0 : mmc board init fail or 해당 축 disable or Amp Fault
             1 : OK
  COMMENTS : Origin flag clear
==============================================================================*/
BOOL OneAxis_MMC::AmpFaultCheckFunction()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable)
	{
		return FALSE;
	}

	ClearStatus();
	int r_val = axis_source(m_nAxisNumPhy);

	if((r_val & ST_AMP_FAULT))	// amp fault
	{
		LogMtStrage(m_nAxisNumPhy);

		m_nOri_Flag = eMotorOrgFlag_NG;

		return FALSE;
	}

	return TRUE;
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : AmpFaultCheckFunction
  PARAMS   : dTargetPos : target posisition
             dMovePos   : Move   positioin
  RETURN   : None
  COMMENTS : Motor 이동 시 Log를 기록한다.
==============================================================================*/
void OneAxis_MMC::SaveMotorMoveLog(double dTargetPos, double dMovePos, int nLogMode, double dEncoder)
{
	// Move
	if(nLogMode == 0){
		m_MtLog.LOG("%s[PhyNo %d] Target Position[MOVE]: [Target Pos: %0.3f] [Move Pos: %0.3fmm] [Encoder: %0.3fmm]",m_szAxisName, m_nAxisNumPhy, dTargetPos, dMovePos, dEncoder);
		//m_EncoderLog.LOG("%s[PhyNo %d] Target Position[MOVE]: [Target Pos: %0.3f] [Move Pos: %0.3fmm]", m_szAxisName, m_nAxisNumPhy, dTargetPos, dMovePos);
	}
	// R Move
	else if(nLogMode == 1){
		m_MtLog.LOG("%s[PhyNo %d] Target Position[R_MOVE]: [Target Pos: %0.3f] [Move Pos: %0.3fmm] [Encoder: %0.3fmm]",m_szAxisName, m_nAxisNumPhy, dTargetPos, dMovePos, dEncoder);
	//	m_EncoderLog.LOG("%s[PhyNo %d] Target Position[R_MOVE]: [Target Pos: %0.3f] [Move Pos: %0.3fmm]", m_szAxisName, m_nAxisNumPhy, dTargetPos, dMovePos);
	}
	// V Move
	else if(nLogMode == 2){
		m_MtLog.LOG("%s[%d] Target Position[V_MOVE]: [Target Pos: %0.3f] [Encoder: %0.3fmm]",m_szAxisName, m_nAxisNumPhy, dTargetPos, dEncoder);
	//	m_EncoderLog.LOG("%s[PhyNo %d] Target Position[V_MOVE]: [Target Pos: %0.3f] [Move Pos: %0.3fmm]", m_szAxisName, m_nAxisNumPhy, dTargetPos, dMovePos);
	}	
}

/*==============================================================================
DESCRIPT : Load 및 Parameter 저장하는 부분에서 호출
KEY WORD : // By jhjang [150225] Save Motor Move Log
ARGUMENT : dPosLimit : [um]
           dNegLimit : [um]
		   bDisable  : 0 = SW Limit이 유효함.
                       1 = Orgin 진행 시에 sw limit을 무시 한다.
RETURN   : None
==============================================================================*/
void OneAxis_MMC::SetSWLimit(double dPosLimit, double dNegLimit, BOOL bDisable)
{
	double dPosLimitPulse	= 0.0;
	double dNegLimitPulse	= 0.0;

	m_dSwLimit_Posi = dPosLimit;
	m_dSwLimit_Nega = dNegLimit;

	if( m_nDir_Move == 1)
	{
		dPosLimitPulse = dPosLimit / m_dScaleFactor;
		dNegLimitPulse = dNegLimit / m_dScaleFactor;
	}
	else
	{
		dPosLimitPulse = (double)m_nDir_Move * dNegLimit / m_dScaleFactor;
		dNegLimitPulse = (double)m_nDir_Move * dPosLimit / m_dScaleFactor;
	}

	m_dPosLimitPulse = dPosLimitPulse;
	m_dNegLimitPulse = dNegLimitPulse;

	// 일반 Running
	if(bDisable == FALSE)
	{
		set_positive_sw_limit(m_nAxisNumPhy, dPosLimitPulse, E_STOP_EVENT);
		set_negative_sw_limit(m_nAxisNumPhy, dNegLimitPulse, E_STOP_EVENT);
	}
	// 원점체크 시작 시
	else if(bDisable == TRUE)
	{
		set_positive_sw_limit(m_nAxisNumPhy, dPosLimitPulse, NO_EVENT);
		set_negative_sw_limit(m_nAxisNumPhy, dNegLimitPulse, NO_EVENT);
	}
		
	//m_MotorLogger.LOG("Set %s[%d] Axis Software Limit : [Pos %0.2f Pulse] [Neg %0.2f Pulse]", m_szAxisName, m_nAxisNumPhy, dPosLimitPulse, dNegLimitPulse);
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-08-26]
  NAME     : 
  PARAMS   : 0=disable, 1=enable
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void OneAxis_MMC::EnableSWLimit(BOOL bEnable)
{
	SetSWLimit(m_dSwLimit_Posi, m_dSwLimit_Nega, !bEnable);
}



/*=============================================================================
  AUTHOR   : 이원용
  NAME     : InSequence
  PARAMS   : None
  RETURN   : bool
  COMMENTS : 축이 명령을 수행중 일때 TRUE(1)을 돌려줌
==============================================================================*/
int	OneAxis_MMC::InSequence()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return true;
	}

	int bResult = in_sequence( m_nAxisNumPhy );

	return bResult;
}

/*=============================================================================
  AUTHOR   : 이원용
  NAME     : InMotion
  PARAMS   : None
  RETURN   : bool 
  COMMENTS : 축이 동작중일때 TRUE(1)을 돌려줌
==============================================================================*/
int	OneAxis_MMC::InMotion()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return true;
	}

	int bResult = in_motion( m_nAxisNumPhy );

	return bResult;
}

/*=============================================================================
  AUTHOR   : 이원용
  NAME     : InPosition
  PARAMS   : None
  RETURN   : bool
  COMMENTS : 위치오차가 In_Position범위 내일때 TRUE(1)을 돌려줌
==============================================================================*/
int	OneAxis_MMC::InPosition()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return true;
	}

	int bResult = in_position( m_nAxisNumPhy );

	return bResult;
}

/*=============================================================================
  AUTHOR   : 이원용
  NAME     : IsAxisDone
  PARAMS   : None
  RETURN   : bool
  COMMENTS : motion_done && in_position 일때 TRUE(1)을 돌려줌
==============================================================================*/
int	OneAxis_MMC::IsAxisDone()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return true;
	}

	int bResult = axis_done( m_nAxisNumPhy );

	return bResult;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
bool OneAxis_MMC::IsMotorStop()		//모터 동작 체크용
{
	if( InSequence() || InMotion() )
		return true;
	return false;
}


//동작 상태
bool OneAxis_MMC::IsPositionDone()	//위치 체크용
{
	if( InMotion() && InPosition() )
	{
		double dCmdPos = GetCommandPos();
		double dCurPos = GetCurrentPos();
		double dDifPos = abs(dCmdPos-dCurPos);
		double dInPos =  0;
		get_in_position(m_nAxisNumPhy, &dInPos);
		if( dDifPos<= dInPos )
			return true;
	}

	return false;
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
int	OneAxis_MMC::GetAxisStatus()
{
	int nRet = axis_source( m_nAxisNumPhy );

	if( !(nRet^0x0008) || !(nRet^0x0800) )
		m_nOri_Flag = eMotorOrgFlag_NG;	//서보OFF시 다시 오리진 하도록 함.

	return nRet;
}
int	OneAxis_MMC::OriginStart()		//Switch 형식의 오리진 시작
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable){
		return 1;
	}
	m_bOri_Stop = FALSE;
	m_nOri_Flag = eMotorOrgFlag_ING;
	return m_nOri_Flag;
}

int	OneAxis_MMC::GetNoControlAxis()	//제어하고 있는 축수
{
	int nRet=0;
	nRet = mmc_all_axes();
	return nRet;
}
double	OneAxis_MMC::GetTargetPos(bool bMM)
{
	return m_dTarget_pos;
}
int OneAxis_MMC::SetEmgStop()
{
	if(!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable)
		return 1 ;

	int nRet = set_e_stop(m_nAxisNumPhy);
	if( nRet==MMC_OK )
	{
		if( IsOrgCompleted()==FALSE )
			m_nOri_Flag = eMotorOrgFlag_NG;
	}

	AlarmReset();

	return nRet;
}

void OneAxis_MMC::LogMtStrage( int nAxisNumPhy, double dPos, double dSpeed )
{
	ST_MOTORSTATUS stMotorData;
	stMotorData.dAcc            = GetAcc();
	stMotorData.dDcc			= GetDcc();
	stMotorData.dCommand		= GetCommandPos();
	stMotorData.dEncoder		= GetCurrentPos();
	stMotorData.bServoStatus	= GetServoStat();
	stMotorData.iInsequence		= in_sequence(nAxisNumPhy);
	stMotorData.iInmotion		= in_motion(nAxisNumPhy);
	stMotorData.iInposition		= in_position(nAxisNumPhy);
	stMotorData.iFramesleft		= frames_left(nAxisNumPhy);
	stMotorData.iMotiondone		= motion_done(nAxisNumPhy);
	stMotorData.iAxisdone		= axis_done(nAxisNumPhy);
	stMotorData.iAxisNum		= nAxisNumPhy;
	stMotorData.dTarget			= dPos;
	stMotorData.dMotorSpeed		= dSpeed;
	get_command_rpm(m_nAxisNumPhy, &stMotorData.sEncoderRPM);

	CString strData = _T("");
	strData.Format("motor[%s][T Pos %.2fmm][E Pos %.2fmm][cmd Pos %.2fmm][Spd: %.2f pps][Acc: %d][Dcc: %d][AmpFault: %d]",
		m_szAxisName,
		stMotorData.dTarget/1000.0,
		stMotorData.dEncoder/1000.0,
		stMotorData.dCommand/1000.0,
		stMotorData.dMotorSpeed, 
		stMotorData.dAcc, 
		stMotorData.dDcc, 
		stMotorData.bServoStatus
	);
	m_MtLog.LOG(strData);
	
	strData.Format("[in_Sequence: %d][in_motion: %d] [in_position : %d] [frames_left : %d] [motion_done : %d] [axis_done : %d] [Cmd RPM : %d]",
		stMotorData.iInsequence,
		stMotorData.iInmotion, 
		stMotorData.iInposition, 
		stMotorData.iFramesleft, 
		stMotorData.iMotiondone, 
		stMotorData.iAxisdone, 
		stMotorData.sEncoderRPM);
	m_MtLog.LOG(strData);
}

double OneAxis_MMC::GetScaleFactor()
{
	return m_dScaleFactor;
}

double OneAxis_MMC::GetServoPulse()
{
	return m_dServo_Pulse;
}

/*=============================================================================
AUTHOR   : 김선진
NAME     :
PARAMS   :
RETURN   :
COMMENTS : 현재의 위치가 limit에 가까운지 확인한다.
           가까우면 true
		   아니면   false
==============================================================================*/
bool OneAxis_MMC::CheckLimitClose()
{
	double dCurPos = GetCurrentPos();

	// 움직이고자 하는 위치가 s/w limit를 벗어나는 위치인지를 확인
	if (dCurPos > m_dSwLimit_Nega + 100000.0 && dCurPos < m_dSwLimit_Posi - 100000.0) {
		return false;
	}

	return true;
}

/*=============================================================================
AUTHOR   : 김선진
NAME     :
PARAMS   :
RETURN   :
COMMENTS : Motor Display를 위한 위치 값을 가져온다.
==============================================================================*/
double OneAxis_MMC::GetMtMonCurPos()
{
	if (!g_pMotion_Cont_Imp->GetInitBoardStatus() || m_bMotorDisable) {
		return 0;
	}

	return m_dBackDisplay_MtEncoder;
}

BOOL OneAxis_MMC::NeedDualOriginChecking()
{
	std::vector<int> vMotorNo;

	vMotorNo.push_back(eAXIS_TEST_PP_1_X);
	vMotorNo.push_back(eAXIS_TEST_PP_2_X);
	vMotorNo.push_back(eAXIS_TEST_PP_1_Y);
	vMotorNo.push_back(eAXIS_TEST_PP_2_Y);
	//vMotorNo.push_back(eAXIS_TRAY_PP_X);
	//vMotorNo.push_back(eAXIS_TRAY_PP_Y);

	for (int i = 0; i < (int)vMotorNo.size(); i++) {
		if (vMotorNo[i] == m_nAxisNumPhy) {
			return TRUE;
		}
	}

	return FALSE;
}

double OneAxis_MMC::GetEncoderPulse()
{
	ASSERT(m_dScaleFactor > 0.0);
	return (GetCurrentPos() / m_dScaleFactor);
}