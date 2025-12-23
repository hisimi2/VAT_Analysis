#include "StdAfx.h"
#include "MotorInterlock_Transfer.h"


//==============================================================================
//
//==============================================================================
CMotorInterlock_Transfer::CMotorInterlock_Transfer(int nTrayPPInx)
	:m_nTrayPPInx(nTrayPPInx)
{
}



//==============================================================================
//
//==============================================================================
CMotorInterlock_Transfer::~CMotorInterlock_Transfer(void)
{
}



//==============================================================================
//
//==============================================================================
int CMotorInterlock_Transfer::Interlock_Transfer_Common()
{
	int nErr = CBaseMotorInterlock::CommonInterlock();
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int CMotorInterlock_Transfer::Interlock_x_common()
{
// 	OneAxis* pMotor = g_TaskTransfer.GetAxis( CTaskTransfer::AXIS_TRANSFER_X );
// 	BOOL bServoOn = pMotor->GetServoStat();
// 	if( bServoOn != TRUE ) {
// 		return ERR_MOT_INTERLOCK_TRANSFER_X_IS_NOT_SERVO_ON;
// 	}
	int nErr = ERR_NO_ERROR;

	//Transfer Z Up cylinder
	int nErr_SafetyZ = g_TaskTransfer.ChkDI(CTaskTransfer::xTRANSFER_UP_SEN, TRUE);
	if((nErr_SafetyZ != ERR_NO_ERROR))
	{
		return ERR_MOT_INTERLOCK_TRANSFER_Z_IS_NOT_UP;
	}

	OneAxis* pMotorFeederY = g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eAxisY);
	if (pMotorFeederY->IsMotorUsable(nErr) != TRUE)
	{
		return (nErr == -1) ? ERR_MOT_INTERLOCK_FEEDER_Y_IS_AMP_FAULT : ERR_MOT_INTERLOCK_TRAY_FEEDER_Y_AXIS_IS_NOT_ORIGIN;
	}

	OneAxis* pMotorUnloadPPY = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisY);
	if (pMotorUnloadPPY->IsMotorUsable(nErr) != TRUE)
	{
		return (nErr == -1) ? ERR_MOT_INTERLOCK_TRAY_PP_2_Y_IS_AMP_FAULT : ERR_MOT_INTERLOCK_TRAY_PP_2_Y_AXIS_IS_NOT_ORIGIN;
	}

// 	//Unload 안전 영역
// 	bool bUnloadTrayPPSafety = g_TaskTrayUnloadPP.is_safety_state();
// 
// 	if (bUnloadTrayPPSafety != true) {
// 		return ERR_MOT_INTERLOCK_TRANSFER_TRAY_PP_IS_NOT_SAFETY;
// 	}

	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int CMotorInterlock_Transfer::Interlock_z_common(int nHead)
{
	// Load Area 위치에 있는 상태에서 Load Stacker Motor가 유효하지 않으면 이동 불가.
	// Unload Area 위치에 있는 상태에서 Unload Stacker Motor가 유효하지 않으면 이동 불가.

	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int CMotorInterlock_Transfer::Interlock_x_org()
{
// 	int nErr = Interlock_Transfer_Common();
// 	if( nErr != ERR_NO_ERROR ){
// 		return nErr;
// 	}
	int nErr = ERR_NO_ERROR;

	nErr = Interlock_x_common();
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}

	//Feeder 안전 sensor 감지
	bool bSafetyFeeder = g_TaskTrayFeeder.GetSafetySensor();
	if (bSafetyFeeder != true) {
		return ERR_MOT_INTERLOCK_TRANSFER_MOVE_FAIL_BECAUSE_TRAY_FEEDER_IS_NOT_DETECTED_HOME_SENSOR;
	}

	//Feeder 안전 위치 확인.
	double dFeederCurPosY = 0.0;
	g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eAxisY)->GetCurrentPos(dFeederCurPosY);

	if (dFeederCurPosY < INTERLOCK_FEEDER_SAFETY_POS_ENC) {
		return ERR_MOT_INTERLOCK_TRANSFER_MOVE_FAIL_BECAUSE_TRAY_FEEDER_IS_NOT_SAFETY_POS;
	}

	//Unload Tray Y Safety Sensor 확인.
	bool bSafetySen = g_TaskTrayUnloadPP.get_TrayUnload_Y_Safety_sen(DEF_ON);
	if (bSafetySen != true)   //Transfer가 Safety 위치가 아니면 Y축은 Safety 위치에 있어야 X축 Move 가능.
	{
		return ERR_TRAY_PP_2_Y_IS_NOT_DETECT_SAFETY_SENSOR;
	}

	//Unload Tray Y Safety Position 확인.
	double dCurPosY = 0.0;
	OneAxis* pMotorY = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisY);
	pMotorY->GetCurrentPos(dCurPosY);

	bool bSafetyPos = false;

	double dWorkEnablePos = INTERLOCK_UD_TRAYPP_Y_WORK_ENABLE_POS_TRANSFER*(-1);
	if (dCurPosY > dWorkEnablePos) {
		bSafetyPos = true;
	}


	if (bSafetyPos != true) {
		return ERR_TRAY_PP_2_Y_IS_NOT_SAFETY;
	}

	return ERR_NO_ERROR;
}


//==============================================================================
//
//==============================================================================
int CMotorInterlock_Transfer::Interlock_x_mov(double dTPos, double dSpd, int nAcc, int nDcc)
{
// 	int nErr = Interlock_Transfer_Common();
// 	if (nErr != ERR_NO_ERROR) {
// 		return nErr;
// 	}

	int nErr = ERR_NO_ERROR;

	nErr = Interlock_x_common(); // Servo, Up_Sen, TrayUnload Y Safety.
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}

	//Feeder 안전 sensor 감지
	bool bSafetyFeeder = g_TaskTrayFeeder.GetSafetySensor();
	if (bSafetyFeeder != true) {
		return ERR_MOT_INTERLOCK_TRANSFER_MOVE_FAIL_BECAUSE_TRAY_FEEDER_IS_NOT_DETECTED_HOME_SENSOR;
	}

	//Feeder 안전 위치 확인.
	double dFeederCurPosY = 0.0;
	g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eAxisY)->GetCurrentPos(dFeederCurPosY);

	if (dFeederCurPosY < INTERLOCK_FEEDER_SAFETY_POS_ENC) {
		return ERR_MOT_INTERLOCK_TRANSFER_MOVE_FAIL_BECAUSE_TRAY_FEEDER_IS_NOT_SAFETY_POS;
	}

	//Unload Tray Y Safety Sensor 확인.
	bool bSafetySen = g_TaskTrayUnloadPP.get_TrayUnload_Y_Safety_sen(DEF_ON);
	if (bSafetySen != true)   //Transfer가 Safety 위치가 아니면 Y축은 Safety 위치에 있어야 X축 Move 가능.
	{
		return ERR_TRAY_PP_2_Y_IS_NOT_DETECT_SAFETY_SENSOR;
	}

	//Unload Tray Y Safety Position 확인.
	double dCurPosY = 0.0;
	OneAxis* pMotorY = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisY);
	pMotorY->GetCurrentPos(dCurPosY);

	bool bSafetyPos = false;
	double dWorkEnablePos = INTERLOCK_UD_TRAYPP_Y_WORK_ENABLE_POS_TRANSFER*(-1);
	if (dCurPosY > dWorkEnablePos) {
		bSafetyPos = true;
	}

	if (bSafetyPos != true) {
		return ERR_TRAY_PP_2_Y_IS_NOT_SAFETY;
	}


	return ERR_NO_ERROR;
}

int CMotorInterlock_Transfer::Interlock_x_jog( double dSpd, int nAcc, int nDcc )
{
// 	int nErr = Interlock_Transfer_Common();
// 	if( nErr != ERR_NO_ERROR ) {
// 		return nErr;
// 	}
	int nErr = ERR_NO_ERROR;

	nErr = Interlock_x_common();
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}

	//Feeder 안전 sensor 감지
	bool bSafetyFeeder = g_TaskTrayFeeder.GetSafetySensor();

	if (bSafetyFeeder != true) {
		return ERR_MOT_INTERLOCK_TRANSFER_MOVE_FAIL_BECAUSE_TRAY_FEEDER_IS_NOT_DETECTED_HOME_SENSOR;
	}

	//Feeder 안전 위치 확인.
	double dFeederCurPosY = 0.0;
	g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eAxisY)->GetCurrentPos(dFeederCurPosY);

	if (dFeederCurPosY < INTERLOCK_FEEDER_SAFETY_POS_ENC) {
		return ERR_MOT_INTERLOCK_TRANSFER_MOVE_FAIL_BECAUSE_TRAY_FEEDER_IS_NOT_SAFETY_POS;
	}

	//Unload Tray Y Safety Sensor 확인.
	bool bSafetySen = g_TaskTrayUnloadPP.get_TrayUnload_Y_Safety_sen(DEF_ON);
	if (bSafetySen != true)   //Transfer가 Safety 위치가 아니면 Y축은 Safety 위치에 있어야 X축 Move 가능.
	{
		return ERR_TRAY_PP_2_Y_IS_NOT_DETECT_SAFETY_SENSOR;
	}

	//Unload Tray Y Safety Position 확인.
	double dCurPosY = 0.0;
	OneAxis* pMotorY = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisY);
	pMotorY->GetCurrentPos(dCurPosY);

	bool bSafetyPos = false;
	double dWorkEnablePos = INTERLOCK_UD_TRAYPP_Y_WORK_ENABLE_POS_TRANSFER*(-1);
	if (dCurPosY > dWorkEnablePos) {
		bSafetyPos = true;
	}

	if (bSafetyPos != true) {
		return ERR_TRAY_PP_2_Y_IS_NOT_SAFETY;
	}

	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int CMotorInterlock_Transfer::Interlock_z_org(int nHead)
{
	int nErr = Interlock_Transfer_Common();
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}
	nErr = Interlock_z_common(nHead);
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}

	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int CMotorInterlock_Transfer::Interlock_z_mov(int nHead, double dTPos, double dSpd, int nAcc, int nDcc)
{
	int nErr = Interlock_Transfer_Common();
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}
	nErr = Interlock_z_common(nHead);
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}

	return ERR_NO_ERROR;
}

