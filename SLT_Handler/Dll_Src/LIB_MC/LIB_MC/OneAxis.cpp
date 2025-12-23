#include "StdAfx.h"
#include "OneAxis.h"
#include "DEF_ERROR_CODE.h"


//==============================================================================
//
//==============================================================================
OneAxis::OneAxis(void)
{
	m_dDesignScaleFactor = 0.0;
	m_dTarget_pos      = 0.0;

	m_dSwLimit_Posi    = 0.0;
	m_dSwLimit_Nega    = 0.0;

	m_dDistanceRate = 0.0;
	m_dMem_BacklashOffset = 0.0;
	m_dBackDisplay_MtEncoder = 0.0;

	m_bBacklashOffset_Apply_Flag = FALSE;
	m_dBefore_BacklashPos = 0.0;
	m_dAfter_BacklashPos = 0.0;
	m_cbInterLock_Org = NULL;
	m_cbInterLock_Move = NULL;
	m_cbInterLock_Jog = NULL;
	for( int i = eSpeed_5; i < eSpeed_MAX; i++ )
	{
		m_vBacklashOffset_XY.push_back( 0.0 );
	}
	
	m_dServo_Pulse    = 2500.0;

	m_dBefore_BacklashPos = 0.0;
	m_dAfter_BacklashPos = 0.0;
	m_bBacklashOffset_Apply_Flag = FALSE;

	m_OriMemFlag = FALSE;

	m_nLastOri_Step = 0;

	memset(m_dBacklashParameterOffset_XY, 0x00, sizeof(m_dBacklashParameterOffset_XY));
	

	////////////////////////////////////Softservo////////////////////////////////////
	m_nAxisNumPhy = 0;
	m_nSubZAxisNumPhy = -1;
	m_nAxisNumLogical = 0;
	m_nPartNo = -1;
	m_dSwLimit_Posi_um = 0.0;
	m_dSwLimit_Nega_um = 0.0;
	m_dScaleFactor = 0.0;

	m_dTarget_pos_um = 0.0;
	m_dLast_pos = 0.0;
	m_dCmdVelocity = 0.0;
	m_dCmdVelocity_Ori = 0.0;
	m_nCmdAcc = 0;
	m_nCmdAcc_Ori = 0;
	m_nCmdDcc = 0;
	
	m_nDir_Ori = 0;
	m_nMMC_Encoder_dir = 0;
	m_nMMC_Coordinate_dir = 0;
	m_nDir_Move = 0;
	m_nDir_Pulse = 0;

	m_nDouble_chk_um = 0;
	m_bDoubleCheckFlag = false;

	m_nOri_Step = 0;

	m_clockStart = 0;
	m_clockFinish = 0;
	m_nEncoderType = 0;
	m_eMotorType = eMotorType_Servo;

	memset(m_szAxisName, 0x00, sizeof(m_szAxisName));
	m_dPosLimitPulse = 0.0;
	m_dNegLimitPulse = 0.0;

	m_bMotorDisable = FALSE;
	m_bResetLevel = 0;

	m_dActTime = 0.0;
	m_nOri_Flag = eMotorOrgFlag_NG;
	m_bOri_Stop = TRUE;

	m_eMotorSyncType = eMotorSyncType_None;
	m_nSlaveNo = 0;
	m_bDegeeCtrl = FALSE;
	m_dPulsePerRound = 0.0;		// 20191214 ihc.
	m_dHwInPosWidth = 0.0;

	//Backlash
	m_nMotorSpdRateIdx = 10;	// 100%
	m_bApplyBacklash = false;
	memset(m_arrBackLashOffset, 0, sizeof(m_arrBackLashOffset));
	memset(m_arrPrevBackLashOffset, 0, sizeof(m_arrPrevBackLashOffset));
	m_dPrevMovePos = 0.0;
	m_dBacklashMoveDist = 0.0;

	//pcd
	m_dScaleFactorOffset = 0.0;

	//module trigger state
	m_bPlviModuleServo = false;
}



//==============================================================================
//
//==============================================================================
OneAxis::~OneAxis(void)
{
	m_vBacklashOffset_XY.clear();
}

//==============================================================================
//
//==============================================================================
void OneAxis::SetAxisInitInfo(char* szAxisName,
							  int num,
							  int nLogicalNum,
							  double scale,
							  int Zr_direction/*=1*/,
							  int encoder_dir/*=1*/,
							  int move_dir/*=1*/,
							  int a_nEncoderType/*=0*/,
							  int coordinate_dir,
							  int pulse_dir,
							  int double_check_pulse,
							  MOTOR_TYPE _motor_type,
							  MOTOR_SYNC_TYPE _motor_sync,
							  int nslaveno,
							  BOOL bDegreeCtrl,
							  int nPulsePerRound,
							  double dHwInPosWidth)
{
	strcpy_s(m_szAxisName, sizeof(m_szAxisName), szAxisName);

	m_nAxisNumPhy = num;
	m_nAxisNumLogical = nLogicalNum;
	m_dScaleFactor = scale;
	m_nDir_Ori = Zr_direction;
	m_nMMC_Encoder_dir = encoder_dir;
	m_nDir_Move = move_dir;
	m_nEncoderType = a_nEncoderType;
	m_nMMC_Coordinate_dir = coordinate_dir;
	m_nDir_Pluse = pulse_dir;
	m_nDouble_chk_um = double_check_pulse;
	m_eMotorType = _motor_type;
	m_eMotorSyncType = _motor_sync;
	m_nSlaveNo = nslaveno;
	m_bDegeeCtrl = bDegreeCtrl;

	m_dPulsePerRound = nPulsePerRound;
	m_dHwInPosWidth = dHwInPosWidth;
}

/*==============================================================================
  DESCRIPT :
  KEY WORD : // By jhjang[150000]: Daily 코드 개선 주석
  ARGUMENT : None
  RETURN   : None
==============================================================================*/
void OneAxis::SetVelAcc(double r_vel, int r_acc, double o_vel, int o_acc, double pos, double neg)
{
	m_dCmdVelocity = r_vel; 
	m_nCmdAcc = r_acc; 
	m_dCmdVelocity_Ori = o_vel; 
	m_nCmdAcc_Ori = o_acc;	
	m_dSwLimit_Posi = pos; 
	m_dSwLimit_Nega = neg;

	SetSWLimit(pos, neg);
}

/*==============================================================================
DESCRIPT :
KEY WORD : // By jhjang [150225] Save Motor Move Log
ARGUMENT : None
RETURN   : None
==============================================================================*/
// virtual OneAxis
void OneAxis::SetVelAcc(double r_vel, int r_acc, int r_dcc, double o_vel, int o_acc, double pos, double neg)
{
	m_dCmdVelocity = r_vel; 
	m_nCmdAcc = r_acc; 
	m_nCmdDcc = r_dcc;
	m_dCmdVelocity_Ori = o_vel; 
	m_nCmdAcc_Ori = o_acc;	
	m_dSwLimit_Posi = pos; 
	m_dSwLimit_Nega = neg;	

	SetSWLimit(pos, neg);
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : ClearOrgFlag
  PARAMS   : 
  RETURN   : 
  COMMENTS : Origin flag clear
==============================================================================*/
void OneAxis::ClearOrgFlag()	
{ 
	m_nOri_Step = 0; 
	m_nOri_Flag = eMotorOrgFlag_ING; 
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : LimitValueCheck
  PARAMS   : Posi_Nega = Posi or Nega
  RETURN   : TRUE  -> Positive limit
			 FALSE -> Negative limie
  COMMENTS : Origin flag clear
==============================================================================*/
double OneAxis::LimitValueCheck(BOOL Posi_Nega)
{
	if(Posi_Nega == TRUE)	// Positive 값 리턴 요구
		return m_dSwLimit_Posi;
	else
		return m_dSwLimit_Nega;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : Origin
  PARAMS   : time = 사용자 설정 시간.
  RETURN   :  1   = 설정 시간 보다 크거나 같은 경우
              0   = 설정 시간 보다 움직이는 시간이 작을 경우
  COMMENTS : Origin 시 경과 시간을 구한다.
==============================================================================*/
int OneAxis::TimeCheck(int time)//msec
{
	m_clockFinish = clock();

	if(m_clockFinish < m_clockStart){
		m_clockStart = clock();
		Sleep(1);
		m_clockFinish = clock();
	}

	double duration = (double)(m_clockFinish - m_clockStart) / CLOCKS_PER_SEC;
	double run_time = (double)(duration*1000);

	if(time < (int)run_time){
		return 1;
	}

	return 0;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void OneAxis::SetScaleFactor(double scale)
{
	m_dScaleFactor = scale;
}



// /*=============================================================================
//   AUTHOR   : 나명진  [2015-08-22]
//   NAME     : 
//   PARAMS   : 
//   RETURN   : 0  : 구동 가능.            
// 			 0> : 보드가 초기화 되어 있지 않음.
//                 : 원점이 잡혀 있지 않음.
// 			    : Amp Fault 상태 임.
// 			    : Motor Disable 상태 임.
//   COMMENTS : Motor 구동이 가능 한 것인지 점검 한다.
// ==============================================================================*/
int OneAxis::ChkMotorUsable()
{
	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}
	// 	if( m_nOri_Flag != eMotorOrgFlag_OK ){
	// 		return ERR_MOTOR_CMN_NOT_ORG_CHECKED;
	// 	}

	ERR_CODE eErrCode = 0;

	if (IsAmpFault()) {
		return ERR_MOTOR_CMN_AMP_FAULT;
	}
	if (m_bMotorDisable) {
		return ERR_MOTOR_CMN_DISABLED;
	}

	if (IsOrgCompleted(&eErrCode) == FALSE)
	{
		return eErrCode;
	}

	return 0;
}


BOOL OneAxis::IsMotorUsable(int &nErr)
{
	nErr = 0;

	if (IsAmpFault()) {
		nErr = -1;
		return FALSE;
	}


	if (IsOrgCompleted(nullptr) == FALSE)
	{
		nErr = -2;
		return FALSE;
	}

	return TRUE;
}



int OneAxis::OriginStop()
{
	m_bOri_Stop = TRUE;
	SetSlowStop();
	return 0;
}

void OneAxis::SetServoPulse(double dServoPulse)
{
	m_dServo_Pulse = dServoPulse;
}

ERR_CODE OneAxis::CheckMotorUsableForVMove()
{
	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}

	if (IsAmpFault() != 0) {
		return ERR_MOTOR_CMN_AMP_FAULT;
	}
	if (m_bMotorDisable) {
		return ERR_MOTOR_CMN_DISABLED;
	}

	return ERR_SUCCESS;
}

ERR_CODE OneAxis::CheckMotorUsable()
{
	if (!g_pMotion_Cont_Imp->GetInitBoardStatus()) {
		return ERR_MOTOR_SYS_BOARD_INITIALIZTION_FAILURE;
	}
	// 	if( m_nOri_Flag != eMotorOrgFlag_OK ){
	// 		return ERR_MOTOR_CMN_NOT_ORG_CHECKED;
	// 	}

	ERR_CODE eErrCode = 0;

	if (IsAmpFault()) {
		return ERR_MOTOR_CMN_AMP_FAULT;
	}
	if (m_bMotorDisable) {
		return ERR_MOTOR_CMN_DISABLED;
	}

	if (IsOrgCompleted(&eErrCode) == FALSE)
	{
		return eErrCode;
	}

	return ERR_SUCCESS;
}