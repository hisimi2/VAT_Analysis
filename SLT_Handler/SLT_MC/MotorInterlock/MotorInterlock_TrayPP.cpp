#include "StdAfx.h"
#include "MotorInterlock_TrayPP.h"


CMotorInterlock_TrayPP::CMotorInterlock_TrayPP(int nTrayPpIdx)
	: m_nTrayPpIdx(nTrayPpIdx)
{
}


CMotorInterlock_TrayPP::~CMotorInterlock_TrayPP(void)
{
}

int CMotorInterlock_TrayPP::Interlock_TrayPP_Common()
{
	//Z축 원점 Check.
	OneAxis* pMotorZ = (m_nTrayPpIdx == eTrayPP_1_LD) ? g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisZ) : g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisZ);
	BOOL bOriginZ = pMotorZ->IsOrgCompleted(nullptr);
	if (bOriginZ != TRUE) {
		return ERR_MOT_INTERLOCK_TRAY_PP_1_Z_AXIS_IS_NOT_ORIGIN + m_nTrayPpIdx;
	}

	//Z축 SafetyPos, SafetySen
	bool bSafetyPosZ = (m_nTrayPpIdx == eTrayPP_1_LD) ? g_TaskTrayLoadPP.WhereIsMotorPosZ() : g_TaskTrayUnloadPP.WhereIsMotorPosZ();
	bool bSafetySenZ = (m_nTrayPpIdx == eTrayPP_1_LD) ? g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisZ)->IsHomeSensorOn() : g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisZ)->IsHomeSensorOn();

	if (bSafetySenZ != true) {
		return ERR_MOT_INTERLOCK_TRAY_PP_1_Z_AXIS_IS_NOT_DETECTED_HOME_SENSOR + m_nTrayPpIdx;
	}

	if (bSafetyPosZ != true)
		return ERR_MOT_INTERLOCK_TRAY_PP_1_Z_AXIS_IS_NOT_SAFETY_POS + m_nTrayPpIdx;

	return ERR_NO_ERROR;
}

int CMotorInterlock_TrayPP::Interlock_x_common()
{
	OneAxis* pMotorX = (m_nTrayPpIdx == eTrayPP_1_LD) ? g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisX) : g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisX);
	BOOL bServoOn = pMotorX->GetServoStat();
	if( bServoOn != TRUE ) {
		return ERR_MOT_INTERLOCK_TRAY_PP_1_X_AXIS_SERVO_IS_NOT_ON + m_nTrayPpIdx;
	}

	return ERR_NO_ERROR;
}
int CMotorInterlock_TrayPP::Interlock_y_common()
{
	//Y축은 Unload Tray만 존재
	OneAxis* pMotorY = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisY);
	BOOL bServoOn = pMotorY->GetServoStat();
	if( bServoOn != TRUE ) {
		return ERR_MOT_INTERLOCK_TRAY_PP_2_Y_AXIS_SERVO_IS_NOT_ON;
	}

	return ERR_NO_ERROR;
}
int CMotorInterlock_TrayPP::Interlock_z_common()
{	
	//X축 Origin Check, Motion Done 확인.
	OneAxis* pMotorX = (m_nTrayPpIdx == eTrayPP_1_LD) ? g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisX) : g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisX);
	if (pMotorX->IsOrgCompleted(nullptr) != TRUE) {
		return ERR_MOT_INTERLOCK_TRAY_PP_1_X_AXIS_IS_NOT_ORIGIN + m_nTrayPpIdx;
	}
	if (pMotorX->MotionDone() != ERR_NO_ERROR) {
		return ERR_MOT_INTERLOCK_TRAY_PP_1_X_NOT_MOTION_DONE + m_nTrayPpIdx;
	}

	//Unload Tray PP만 Y축 Origin Check
	if (m_nTrayPpIdx == eTrayPP_2_UD)
	{
		OneAxis* pMotorY = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisY);
		if (pMotorY->IsOrgCompleted(nullptr) != TRUE) {
			return ERR_MOT_INTERLOCK_TRAY_PP_2_Y_AXIS_IS_NOT_ORIGIN;
		}
		if (pMotorY->MotionDone() != ERR_NO_ERROR) {
			return ERR_MOT_INTERLOCK_TRAY_PP_2_Y_NOT_MOTION_DONE;
		}
	}


	//X Pitch Origin Check, Motion Done확인.
	OneAxis* pMotorXPitch = (m_nTrayPpIdx == eTrayPP_1_LD) ? g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisPitchX) : g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisPitchX);
	if (pMotorXPitch->IsOrgCompleted(nullptr) != TRUE) {
		return ERR_MOT_INTERLOCK_TRAY_PP_1_X_PITCH_AXIS_IS_NOT_ORIGIN + m_nTrayPpIdx;
	}
	if (pMotorXPitch->MotionDone() != ERR_NO_ERROR) {
		return ERR_MOT_INTERLOCK_TRAY_PP_1_X_PITCH_NOT_MOTION_DONE + m_nTrayPpIdx;
	}

	//Y Pitch Origin Check, Motion Done확인.
	OneAxis* pMotorYPitch = (m_nTrayPpIdx == eTrayPP_1_LD) ? g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisPitchY) : g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisPitchY);
	if (pMotorYPitch->IsOrgCompleted(nullptr) != TRUE) {
		return ERR_MOT_INTERLOCK_TRAY_PP_1_Y_PITCH_AXIS_IS_NOT_ORIGIN + m_nTrayPpIdx;
	}
	if (pMotorYPitch->MotionDone() != ERR_NO_ERROR) {
		return ERR_MOT_INTERLOCK_TRAY_PP_1_Y_PITCH_NOT_MOTION_DONE + m_nTrayPpIdx;
	}

// 	//End Stopper Origin Check, Motion Done확인.
// 	OneAxis* pMotorEndStopperY = g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eAxisEndStopper);
// 	if (pMotorEndStopperY != nullptr)
// 	{
// 		if (pMotorEndStopperY->IsOrgCompleted(nullptr) != TRUE) {
// 			return ERR_MOT_INTERLOCK_END_STOPPER_Y_AXIS_IS_NOT_ORIGIN + m_nTrayPpIdx;
// 		}
// 
// 		int nMotionDoneErr = pMotorEndStopperY->MotionDone();
// 		if (nMotionDoneErr != ERR_NO_ERROR) {
// 			CString strLog = _T("");
// // 			strLog.Format(_T("End Stopper Motion Done Fail. [Err Code = %d]", nMotionDoneErr));
// // 			m_MtLog.LOG(strLog);
// 			return ERR_MOT_INTERLOCK_END_STOPPER_NOT_MOTION_DONE;
// 		}
// // 		if (pMotorEndStopperY->MotionDone() != ERR_NO_ERROR) {
// // 			return ERR_MOT_INTERLOCK_END_STOPPER_NOT_MOTION_DONE;
// // 		}
// 	}

	return ERR_NO_ERROR;
}
int CMotorInterlock_TrayPP::Interlock_xpitch_common()
{
	OneAxis* pMotor = (m_nTrayPpIdx == eTrayPP_1_LD) ? g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisPitchX) : g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisPitchX);
	BOOL bServoOn = pMotor->GetServoStat();
	if( bServoOn != TRUE ) {
		return ERR_MOT_INTERLOCK_TRAY_PP_1_X_PITCH_SERVO_IS_NOT_ON + m_nTrayPpIdx;
	}

	return ERR_NO_ERROR;
}
int CMotorInterlock_TrayPP::Interlock_ypitch_common()
{
	OneAxis* pMotor = (m_nTrayPpIdx == eTrayPP_1_LD) ? g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisPitchY) : g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisPitchY);
	BOOL bServoOn = pMotor->GetServoStat();
	if( bServoOn != TRUE ) {
		return ERR_MOT_INTERLOCK_TRAY_PP_1_Y_PITCH_SERVO_IS_NOT_ON + m_nTrayPpIdx;
	}

	return ERR_NO_ERROR;
}

int CMotorInterlock_TrayPP::Interlock_Tray_Feeder_Common()
{
	int nErr = ERR_NO_ERROR;

	BOOL bChkCylinderUp = g_TaskTrayFeeder.chk_cyl_UpDown(eUp);
	if (bChkCylinderUp != ERR_NO_ERROR)
		return ERR_MOT_INTERLOCK_FEEDER_Z_IS_NOT_UP;

	OneAxis* pMotorZLoad = g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisZ);
	BOOL bOriginZLoad = pMotorZLoad->IsOrgCompleted(nullptr);
	if (bOriginZLoad != TRUE) {
		return ERR_MOT_INTERLOCK_TRAY_PP_1_Z_AXIS_IS_NOT_ORIGIN;
	}

	OneAxis* pMotorZUnload = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisZ);
	BOOL bOriginZUnload = pMotorZUnload->IsOrgCompleted(nullptr);
	if (bOriginZUnload != TRUE) {
		return ERR_MOT_INTERLOCK_TRAY_PP_2_Z_AXIS_IS_NOT_ORIGIN;
	}

	return ERR_NO_ERROR;
}


int CMotorInterlock_TrayPP::Interlock_x_org()
{
	int nErr = Interlock_TrayPP_Common();
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}
// 	nErr = Interlock_x_common();
// 	if( nErr != ERR_NO_ERROR ){
// 		return nErr;
// 	}

	if (m_nTrayPpIdx == eTrayPP_2_UD) {
		//Unload Y축 Origin Check
		OneAxis* pMotorY = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisY);
		if (pMotorY->IsOrgCompleted(nullptr) != TRUE) {
			return ERR_MOT_INTERLOCK_TRAY_PP_2_Y_AXIS_IS_NOT_ORIGIN;
		}

		bool bSafetySen = g_TaskTrayUnloadPP.get_TrayUnload_Y_Safety_sen(DEF_ON);
		if (bSafetySen != true) {
			return ERR_TRAY_PP_2_Y_IS_NOT_DETECT_SAFETY_SENSOR;
		}

		bool bSafetyY = g_TaskTrayUnloadPP.is_safety_state();
		if (bSafetyY != true) {
			return ERR_MOT_INTERLOCK_TRAY_PP_Y_AXIS_IS_NOT_SAFETY_POS;
		}
	}

	return ERR_NO_ERROR;
}
int CMotorInterlock_TrayPP::Interlock_x_mov(double dTPos, double dSpd, int nAcc, int nDcc)
{
	int nErr = Interlock_TrayPP_Common(); //Common Interlock, Z Origin, Z Safety Pos
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}

	if (m_nTrayPpIdx == eTrayPP_2_UD) //Unload Hand는 Transfer와 충돌 위험.
	{
		//Unload Tray PP일 경우 Y축 원점 Check.
		OneAxis* pMotorY = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisY);
		if (pMotorY->IsOrgCompleted(nullptr) != TRUE) {
			return ERR_MOT_INTERLOCK_TRAY_PP_2_Y_AXIS_IS_NOT_ORIGIN;
		}

		//Transfer X 원점 Check.
		OneAxis* pMotorTransferX = g_TaskTransfer.GetAxis(CTaskTrayBasePP::eAxisX);
		if (pMotorTransferX->IsOrgCompleted(nullptr) != TRUE) {
			return ERR_MOT_INTERLOCK_TRANSFER_X_IS_NOT_INIT;
		}
		

		bool bSafetyTransfer = g_TaskTransfer.is_safety_state();
		//bool bChkYSafetySen = g_TaskTrayUnloadPP.is_safety_state();
		
		//Unload Tray Y Safety Sensor 확인.
		bool bSafetySen = g_TaskTrayUnloadPP.get_TrayUnload_Y_Safety_sen(DEF_ON);
		if (bSafetyTransfer != true && bSafetySen != true)   //Transfer가 Safety 위치가 아니면 Y축은 Safety 위치에 있어야 X축 Move 가능.
		{
			return ERR_MOT_INTERLOCK_TRAY_PP_Y_AXIS_AND_TRANSFER_IS_NOT_SAFETY_SENSOR;
		}

		//Unload Tray Y Safety Position 확인.
		double dCurPosY = 0.0;
		pMotorY->GetCurrentPos(dCurPosY);

		bool bSafetyPos = false;
		double dWorkEnablePos = INTERLOCK_UD_TRAYPP_Y_WORK_ENABLE_POS_TRANSFER*(-1);
		if (dCurPosY > dWorkEnablePos) {
			bSafetyPos = true;
		}

		if (bSafetyTransfer != true && bSafetyPos != true) {
			return ERR_MOT_INTERLOCK_TRAY_PP_Y_AXIS_AND_TRANSFER_IS_NOT_SAFETY_POS;
		}


	}

	return ERR_NO_ERROR;
}
int CMotorInterlock_TrayPP::Interlock_x_jog( double dSpd, int nAcc, int nDcc )
{
	int nErr = Interlock_TrayPP_Common();
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
// 	nErr = Interlock_x_common();
// 	if( nErr != ERR_NO_ERROR ) {
// 		return nErr;
// 	}

	if (m_nTrayPpIdx == eTrayPP_2_UD) //Unload Hand는 Transfer와 충돌 위험.
	{
		//Unload Tray PP일 경우 Y축 원점 Check.
		OneAxis* pMotorY = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisY);
		if (pMotorY->IsOrgCompleted(nullptr) != TRUE) {
			return ERR_MOT_INTERLOCK_TRAY_PP_2_Y_AXIS_IS_NOT_ORIGIN;
		}

		//Transfer X 원점 Check.
		OneAxis* pMotorTransferX = g_TaskTransfer.GetAxis(CTaskTrayBasePP::eAxisX);
		if (pMotorTransferX->IsOrgCompleted(nullptr) != TRUE) {
			return ERR_MOT_INTERLOCK_TRANSFER_X_IS_NOT_INIT;
		}


		bool bSafetyTransfer = g_TaskTransfer.is_safety_state();
		//bool bChkYSafetySen = g_TaskTrayUnloadPP.is_safety_state();

		//Unload Tray Y Safety Sensor 확인.
		bool bSafetySen = g_TaskTrayUnloadPP.get_TrayUnload_Y_Safety_sen(DEF_ON);
		if (bSafetyTransfer != true && bSafetySen != true)   //Transfer가 Safety 위치가 아니면 Y축은 Safety 위치에 있어야 X축 Move 가능.
		{
			return ERR_MOT_INTERLOCK_TRAY_PP_Y_AXIS_AND_TRANSFER_IS_NOT_SAFETY_SENSOR;
		}

		//Unload Tray Y Safety Position 확인.
		double dCurPosY = 0.0;
		pMotorY->GetCurrentPos(dCurPosY);

		bool bSafetyPos = false;
		double dWorkEnablePos = INTERLOCK_UD_TRAYPP_Y_WORK_ENABLE_POS_TRANSFER*(-1);
		if (dCurPosY > dWorkEnablePos) {
			bSafetyPos = true;
		}

		if (bSafetyTransfer != true && bSafetyPos != true) {
			return ERR_MOT_INTERLOCK_TRAY_PP_Y_AXIS_AND_TRANSFER_IS_NOT_SAFETY_POS;
		}
	}

	return ERR_NO_ERROR;
}
int CMotorInterlock_TrayPP::Interlock_y_org()
{
	int nErr = Interlock_TrayPP_Common();
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}
// 	nErr = Interlock_y_common();
// 	if( nErr != ERR_NO_ERROR ){
// 		return nErr;
// 	}

	return ERR_NO_ERROR;
}
int CMotorInterlock_TrayPP::Interlock_y_mov(double dTPos, double dSpd, int nAcc, int nDcc)
{
	int nErr = Interlock_TrayPP_Common();
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}
// 	nErr = Interlock_y_common();
// 	if( nErr != ERR_NO_ERROR ){
// 		return nErr;
// 	}

	//Transfer X 원점 Check.
	OneAxis* pMotorTransferX = g_TaskTransfer.GetAxis(CTaskTrayBasePP::eAxisX);
	if (pMotorTransferX->IsOrgCompleted(nullptr) != TRUE) {
		return ERR_MOT_INTERLOCK_TRANSFER_X_IS_NOT_INIT;
	}

	// Transfer가 안전 위치에 있지 않으면, Position 이동 영역을 확인하여 범위 초과 시 동작 되지 않고 알람 발생.
	bool bSafetyTransfer = g_TaskTransfer.is_safety_state();
	if( bSafetyTransfer != true ) {
		double dWorkEnableMin_um = INTERLOCK_UD_TRAYPP_Y_WORK_ENABLE_POS_TRANSFER*(-1);
		if( dTPos < dWorkEnableMin_um){//MIN_TRAY_PP_WORK_ENABLE_POS_um) {
			return ERR_MOT_INTERLOCK_TRAY_PP_Y_AXIS_MOVE_FAIL_BECAUSE_TRANSFER_IS_NOT_SAFETY_POS_AND_TARGET_POS_IS_OUT_OF_RANGE;
		}
	}

	return ERR_NO_ERROR;
}
int CMotorInterlock_TrayPP::Interlock_y_jog( double dSpd, int nAcc, int nDcc )
{
	int nErr = Interlock_TrayPP_Common();
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
// 	nErr = Interlock_y_common();
// 	if( nErr != ERR_NO_ERROR ) {
// 		return nErr;
// 	}

	//Transfer X 원점 Check.
	OneAxis* pMotorTransferX = g_TaskTransfer.GetAxis(CTaskTrayBasePP::eAxisX);
	if (pMotorTransferX->IsOrgCompleted(nullptr) != TRUE) {
		return ERR_MOT_INTERLOCK_TRANSFER_X_IS_NOT_INIT;
	}

	// Transfer가 안전 위치에 있지 않으면, Position 이동 영역을 확인하여 범위 초과 시 동작 되지 않고 알람 발생.
	bool bSafetyTransfer = g_TaskTransfer.is_safety_state();
	if (bSafetyTransfer != true) {
		double dCurPosY = 0.0;
		g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisY)->GetCurrentPos(dCurPosY);

		double dWorkEnableMin_um = INTERLOCK_UD_TRAYPP_Y_WORK_ENABLE_POS_TRANSFER*(-1);
		if (dCurPosY < dWorkEnableMin_um) {//MIN_TRAY_PP_WORK_ENABLE_POS_um) {
			return ERR_MOT_INTERLOCK_TRAY_PP_Y_AXIS_MOVE_FAIL_BECAUSE_TRANSFER_IS_NOT_SAFETY_POS_AND_TARGET_POS_IS_OUT_OF_RANGE;
		}
	}

	return ERR_NO_ERROR;
}

int CMotorInterlock_TrayPP::Interlock_Feeder_mov(double dTPos, double dSpd, int nAcc, int nDcc)
{
	int nErr = Interlock_Tray_Feeder_Common();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	//CTaskTrayFeeder::eWorkSite _worksite = g_TaskTrayFeeder.WhereIsWorkSite();

	double dCurPosY_Feeder, dCurPosX_load, dCurPosX_unload, dCurPosY_unload;
	g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eAxisY)->GetCurrentPos(dCurPosY_Feeder);
	g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisX)->GetCurrentPos(dCurPosX_load);
	g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisX)->GetCurrentPos(dCurPosX_unload);
	g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisY)->GetCurrentPos(dCurPosY_unload);

	bool bSafetyZSen = false;
	bool bSafetyZPos = false;
	//Tray Load Hand 영역 확인.
//	if (dTPos >= INTERLOCK_FEEDER_POS_BOUNDARY || dCurPosY_Feeder >= INTERLOCK_FEEDER_POS_BOUNDARY) { //Feeder Load 영역, 영역 하나로 가져 가야 함 빈틈 없이.
		//Tray Load X Origin Check 확인.
		OneAxis* pMotorLoadPpX = g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisX);
		BOOL bLDOriginX = pMotorLoadPpX->IsOrgCompleted(nullptr);
		if (bLDOriginX != TRUE) {
			return ERR_MOT_INTERLOCK_TRAY_PP_1_X_AXIS_IS_NOT_ORIGIN;
		}

		if (INTERLOCK_LD_TRAYPP_X_WORK_ENABLE_FEEDER_START_POS < dCurPosX_load && dCurPosX_load < INTERLOCK_LD_TRAYPP_X_WORK_ENABLE_FEEDER_END_POS)
		{
			BOOL bIsWorkPos = FALSE;

			std::vector<CPoint> vPicker_First;
			vPicker_First.push_back(CPoint(0, 0));

			std::vector<CPoint> vPicker_Last;
			vPicker_Last.push_back(CPoint(MAX_TRAY_PP_CNT_X - 1, MAX_TRAY_PP_CNT_Y - 1));

			CPoint pocket_first(0, 0);
			CPoint pocket_last(MAX_LD_TBL_POCKET_CNT_X - 1, MAX_LD_TBL_POCKET_CNT_Y - 1);

			double dTbl2_Left_Max = g_TaskTrayLoadPP.GetTeachPos(LOC_IDX_TRAY_PP_TBL_2, pocket_first, vPicker_Last, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX);
			double dTbl2_Right_Max = g_TaskTrayLoadPP.GetTeachPos(LOC_IDX_TRAY_PP_TBL_2, pocket_last, vPicker_First, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX);
			double dTbl3_Left_Max = g_TaskTrayLoadPP.GetTeachPos(LOC_IDX_TRAY_PP_TBL_3, pocket_first, vPicker_Last, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX);
			double dTbl3_Right_Max = g_TaskTrayLoadPP.GetTeachPos(LOC_IDX_TRAY_PP_TBL_3, pocket_last, vPicker_First, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX);

			if ((dTbl2_Left_Max - 200.0 <= dCurPosX_load && dCurPosX_load <= dTbl2_Right_Max + 200.0) || (dTbl3_Left_Max - 200.0 <= dCurPosX_load && dCurPosX_load <= dTbl3_Right_Max + 200.0))
			{
				bIsWorkPos = TRUE;
			}			

			if (bIsWorkPos != TRUE)
			{
				//Z Home Sensor확인.
				bSafetyZSen = g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisZ)->IsHomeSensorOn();
				if (bSafetyZSen != true) {
					return ERR_MOT_INTERLOCK_TRAY_PP_1_Z_AXIS_IS_NOT_DETECTED_HOME_SENSOR;
				}

				//Z Safety Position 확인.
				bSafetyZPos = g_TaskTrayLoadPP.WhereIsMotorPosZ();
				if (bSafetyZPos != true) {
					return ERR_MOT_INTERLOCK_TRAY_PP_1_Z_AXIS_IS_NOT_SAFETY_POS;
				}
			}

			// Interlock 강화 [8/9/2024 dohyeong.kim]
			//2호기만 적용 임시
			int nEQPNo = g_DMCont.m_dmEQP.GN(NDM0_Handler_EQP_No);
			if (nEQPNo > DEF_EQUIPMENT_UNIT1)
			{
				std::vector<int> vFeederImpactPkr;
				vFeederImpactPkr.clear();
				BOOL bIsImpact = Interlock_TrayPP_Feeder_Impact_Pos(eTrayPP_1_LD, dCurPosX_load, vFeederImpactPkr);
				if (bIsImpact == TRUE)
				{
					BOOL bHomeSensor = FALSE;
					BOOL bSafetyPos = FALSE;
					for each(auto picker in vFeederImpactPkr)
					{
						if (g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
						{
							bHomeSensor = g_pBbxmnpClient->GetHomeSensorStatus(CBbxmnpClient::PICK_MOTOR_LOAD_TRAY, eBMAxis_LOAD_TRAY_Picker01 + picker);
							if (bHomeSensor != TRUE)
								return ERR_MOT_INTERLOCK_TRAY_PP_1_Z_AXIS_IS_NOT_DETECTED_HOME_SENSOR;
										
							bSafetyPos = g_TaskTrayLoadPP.WhereIsMotorPosZEach(CBbxmnpClient::PICK_MOTOR_LOAD_TRAY, (_eBbxmnpAxis)picker);
							if (bSafetyPos != TRUE)
								return ERR_MOT_INTERLOCK_TRAY_PP_1_Z_AXIS_IS_NOT_SAFETY_POS;
						}
					}
				}
			}
		}
//	}




	//Tray Unload Hand 영역 확인.
//	if (dTPos < INTERLOCK_FEEDER_POS_BOUNDARY || dCurPosY_Feeder < INTERLOCK_FEEDER_POS_BOUNDARY) { //feeder Unload 가 간섭 시작 되는 위치
		//Tray Unload X Origin Check 확인.
		OneAxis* pMotorUnloadPpX = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisX);
		BOOL bUDOriginX = pMotorUnloadPpX->IsOrgCompleted(nullptr);
		if (bUDOriginX != TRUE) {
			return ERR_MOT_INTERLOCK_TRAY_PP_2_X_AXIS_IS_NOT_ORIGIN;
		}

		if (INTERLOCK_UD_TRAYPP_X_WORK_ENABLE_FEEDER_START_POS < dCurPosX_unload && dCurPosX_unload < INTERLOCK_UD_TRAYPP_X_WORK_ENABLE_FEEDER_END_POS && dCurPosY_unload >= -100000.0)
		{
			BOOL bIsWorkPos = FALSE;

			//Picker (3,1)이 Stacker (0,0) Pos에 있을 때
			std::vector<CPoint> vPicker_Last;
			vPicker_Last.push_back(CPoint(MAX_TRAY_PP_CNT_X - 1, MAX_TRAY_PP_CNT_Y - 1));

			CPoint pocket_first(0, 0);
			double dStacker1_Left_Max = g_TaskTrayUnloadPP.GetTeachPos(LOC_IDX_TRAY_PP_CONV_4, pocket_first, vPicker_Last, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX);
			if (dCurPosX_unload >= dStacker1_Left_Max - 200.0)
			{
				bIsWorkPos = TRUE;
			}

			if (bIsWorkPos != TRUE)
			{
				//Z safety 확인.
				bSafetyZSen = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisZ)->IsHomeSensorOn();
				if (bSafetyZSen != true) {
					return ERR_MOT_INTERLOCK_TRAY_PP_2_Z_AXIS_IS_NOT_DETECTED_HOME_SENSOR;
				}

				//Z Safety Position 확인.
				bSafetyZPos = g_TaskTrayUnloadPP.WhereIsMotorPosZ();
				if (bSafetyZPos != true) {
					return ERR_MOT_INTERLOCK_TRAY_PP_2_Z_AXIS_IS_NOT_SAFETY_POS;
				}
			}

			// Interlock 강화 [8/9/2024 dohyeong.kim]
			//2호기만 적용 임시
			int nEQPNo = g_DMCont.m_dmEQP.GN(NDM0_Handler_EQP_No);
			BOOL bCrtlMode = g_TaskSystemCtrl.GetEqpCtrlMode();
			if (nEQPNo > DEF_EQUIPMENT_UNIT1 && bCrtlMode == eEqpCtrlMode_Manual)
			{
				std::vector<int> vFeederImpactPkr;
				vFeederImpactPkr.clear();
				BOOL bIsImpact = Interlock_TrayPP_Feeder_Impact_Pos(eTrayPP_2_UD, dCurPosX_unload, vFeederImpactPkr);
				if (bIsImpact == TRUE)
				{
					BOOL bHomeSensor = FALSE;
					BOOL bSafetyPos = FALSE;
					for each(auto picker in vFeederImpactPkr)
					{
						bHomeSensor = g_pBbxmnpClient->GetHomeSensorStatus(CBbxmnpClient::PICK_MOTOR_UNLOAD_TRAY, eBMAxis_UNLOAD_TRAY_Picker01 + picker);
						if (bHomeSensor != TRUE)
							return ERR_MOT_INTERLOCK_TRAY_PP_2_Z_AXIS_IS_NOT_DETECTED_HOME_SENSOR;

						bSafetyPos = g_TaskTrayUnloadPP.WhereIsMotorPosZEach(CBbxmnpClient::PICK_MOTOR_UNLOAD_TRAY, (_eBbxmnpAxis)picker);
						if (bSafetyPos != TRUE)
							return ERR_MOT_INTERLOCK_TRAY_PP_2_Z_AXIS_IS_NOT_SAFETY_POS;
					}
				}
			}
		}

		//Transfer X 원점 Check.
		OneAxis* pMotorTransferX = g_TaskTransfer.GetAxis(CTaskTrayBasePP::eAxisX);
		if (pMotorTransferX->IsOrgCompleted(nullptr) != TRUE) {
			return ERR_MOT_INTERLOCK_TRANSFER_X_IS_NOT_INIT;
		}

		//Transfer 안전 영역 확인.
		double dFeederCurPosY = 0.0;
		g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eAxisY)->GetCurrentPos(dFeederCurPosY);

		if (dTPos < INTERLOCK_FEEDER_SAFETY_POS_ENC) {
			bool bSafetyTransfer = g_TaskTransfer.is_safety_state();
			if (bSafetyTransfer != TRUE) {
				return ERR_MOT_INTERLOCK_TRAY_FEEDER_MOVE_FAIL_BECAUSE_TRANSFER_IS_NOT_SAFETY_POS;
			}
		}
		
//	}
	return ERR_NO_ERROR;
}


int CMotorInterlock_TrayPP::Interlock_Feeder_org()
{
	int nErr = Interlock_Tray_Feeder_Common();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	bool bSafetyZSen = false;
	bool bSafetyZPos = false;
	///////////////////////////////////Tray Load Hand Z 원점 Check
	//Z Home Sensor확인.
	bSafetyZSen = g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisZ)->IsHomeSensorOn();
	if (bSafetyZSen != true) {
		return ERR_MOT_INTERLOCK_TRAY_PP_1_Z_AXIS_IS_NOT_DETECTED_HOME_SENSOR;
	}

	//Z Safety Position 확인.
	bSafetyZPos = g_TaskTrayLoadPP.WhereIsMotorPosZ();
	if (bSafetyZPos != true) {
		return ERR_MOT_INTERLOCK_TRAY_PP_1_Z_AXIS_IS_NOT_SAFETY_POS;
	}

	///////////////////////////////////Tray Unload Hand Z 원점 Check
	//Z safety 확인.
	bSafetyZSen = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisZ)->IsHomeSensorOn();
	if (bSafetyZSen != true) {
		return ERR_MOT_INTERLOCK_TRAY_PP_2_Z_AXIS_IS_NOT_DETECTED_HOME_SENSOR;
	}

	//Z Safety Position 확인.
	bSafetyZPos = g_TaskTrayUnloadPP.WhereIsMotorPosZ();
	if (bSafetyZPos != true) {
		return ERR_MOT_INTERLOCK_TRAY_PP_2_Z_AXIS_IS_NOT_SAFETY_POS;
	}

	//Transfer가 Feeder에 근접한 상태로 Feeder가 Homing을 진행할 경우 충돌 발생. Transfer가 Safety Position에서 벋어났을 경우 알람 발생
	bool bSafetyTransfer = g_TaskTransfer.is_safety_state();
	bool bSafetyFeeder = g_TaskTrayFeeder.GetSafetySensor();

	if (bSafetyFeeder == false && bSafetyTransfer == false && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
	{
		return ERR_FEEDER_TRANSFER_IS_NOT_SAFETY;
	}

	return ERR_NO_ERROR;
}


int CMotorInterlock_TrayPP::Interlock_Feeder_jog(double dSpd, int nAcc, int nDcc)
{
	int nErr = Interlock_Tray_Feeder_Common();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	double dCurPosY_Feeder, dCurPosX_load, dCurPosX_unload;
	g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eAxisY)->GetCurrentPos(dCurPosY_Feeder);
	g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisX)->GetCurrentPos(dCurPosX_load);
	g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisX)->GetCurrentPos(dCurPosX_unload);

	bool bSafetyZSen = false;
	bool bSafetyZPos = false;
	//Tray Load Hand 영역 확인.
	if (dCurPosY_Feeder >= INTERLOCK_FEEDER_POS_BOUNDARY) {

		//Tray Load X Origin Check 확인.
		OneAxis* pMotorX = g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisX);
		BOOL bOriginX = pMotorX->IsOrgCompleted(nullptr);
		if (bOriginX != TRUE) {
			return ERR_MOT_INTERLOCK_TRAY_PP_1_X_AXIS_IS_NOT_ORIGIN;
		}

		if (INTERLOCK_LD_TRAYPP_X_WORK_ENABLE_FEEDER_START_POS < dCurPosX_load && dCurPosX_load < INTERLOCK_LD_TRAYPP_X_WORK_ENABLE_FEEDER_END_POS)
		{
			//Z safety 확인.
			bSafetyZSen = g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisZ)->IsHomeSensorOn();
			if (bSafetyZSen != true) {
				return ERR_MOT_INTERLOCK_TRAY_PP_1_Z_AXIS_IS_NOT_DETECTED_HOME_SENSOR;
			}

			//Z Safety Position 확인.
			bSafetyZPos = g_TaskTrayLoadPP.WhereIsMotorPosZ();
			if (bSafetyZPos != true) {
				return ERR_MOT_INTERLOCK_TRAY_PP_1_Z_AXIS_IS_NOT_SAFETY_POS;
			}
		}
	}

	//Tray Unload Hand 영역 확인.
	if (dCurPosY_Feeder < INTERLOCK_FEEDER_POS_BOUNDARY) {

		//Tray Unload X Origin Check 확인.
		OneAxis* pMotorX = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisX);
		BOOL bOriginX = pMotorX->IsOrgCompleted(nullptr);
		if (bOriginX != TRUE) {
			return ERR_MOT_INTERLOCK_TRAY_PP_2_X_AXIS_IS_NOT_ORIGIN;
		}

		if (INTERLOCK_UD_TRAYPP_X_WORK_ENABLE_FEEDER_START_POS < dCurPosX_unload && dCurPosX_unload < INTERLOCK_UD_TRAYPP_X_WORK_ENABLE_FEEDER_END_POS)
		{
			//Z safety 확인.
			bSafetyZSen = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisZ)->IsHomeSensorOn();
			if (bSafetyZSen != true) {
				return ERR_MOT_INTERLOCK_TRAY_PP_2_Z_AXIS_IS_NOT_DETECTED_HOME_SENSOR;
			}

			//Z Safety Position 확인.
			bSafetyZPos = g_TaskTrayUnloadPP.WhereIsMotorPosZ();
			if (bSafetyZPos != true) {
				return ERR_MOT_INTERLOCK_TRAY_PP_2_Z_AXIS_IS_NOT_SAFETY_POS;
			}
		}

		//Transfer X 원점 Check.
		OneAxis* pMotorTransferX = g_TaskTransfer.GetAxis(CTaskTrayBasePP::eAxisX);
		if (pMotorTransferX->IsOrgCompleted(nullptr) != TRUE) {
			return ERR_MOT_INTERLOCK_TRANSFER_X_IS_NOT_INIT;
		}

		bool bSafetyTransfer = g_TaskTransfer.is_safety_state();
		if (bSafetyTransfer != TRUE) {
			return ERR_MOT_INTERLOCK_TRAY_FEEDER_MOVE_FAIL_BECAUSE_TRANSFER_IS_NOT_SAFETY_POS;
		}
	}
	return ERR_NO_ERROR;
}


int CMotorInterlock_TrayPP::Interlock_z_org()
{
	int nErr = CBaseMotorInterlock::CommonInterlock();
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	nErr = Interlock_z_common();
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}
	return ERR_NO_ERROR;
}
int CMotorInterlock_TrayPP::Interlock_z_mov(double dTPos, double dSpd, int nAcc, int nDcc)
{
	int nErr = Interlock_z_common(); //연관 축들 원점 check 확인, Motion Done 확인.
	if (nErr != ERR_NO_ERROR) {
		return nErr;
	}

	//Feeder Unit 영역일 때, Feeder Motion Done Check.
	double dTrayCurPosX = 0.0; double dTrayCurPosY = 0.0; double dCurPosY_unload = 0.0;
	OneAxis* pMotorX = (m_nTrayPpIdx == eTrayPP_1_LD) ? g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisX) : g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisX);
	pMotorX->GetCurrentPos(dTrayCurPosX);
	OneAxis* pMotorY = g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisY);
	pMotorY->GetCurrentPos(dTrayCurPosY);

	g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisY)->GetCurrentPos(dCurPosY_unload);

	//변경 필요
	double dFeederPosStart = (m_nTrayPpIdx == eTrayPP_1_LD) ? INTERLOCK_LD_TRAYPP_X_WORK_ENABLE_FEEDER_START_POS : INTERLOCK_UD_TRAYPP_X_WORK_ENABLE_FEEDER_START_POS;
	double dFeederPosEnd = (m_nTrayPpIdx == eTrayPP_1_LD) ? INTERLOCK_LD_TRAYPP_X_WORK_ENABLE_FEEDER_END_POS : INTERLOCK_UD_TRAYPP_X_WORK_ENABLE_FEEDER_END_POS;

	if (dFeederPosStart <= dTrayCurPosX && dTrayCurPosX <= dFeederPosEnd) //Feeder 영역.
	{
		BOOL bIsWorkPos = FALSE;

		if (m_nTrayPpIdx == eTrayPP_1_LD)
		{
			std::vector<CPoint> vPicker_First;
			vPicker_First.push_back(CPoint(0, 0));

			std::vector<CPoint> vPicker_Last;
			vPicker_Last.push_back(CPoint(MAX_TRAY_PP_CNT_X - 1, MAX_TRAY_PP_CNT_Y - 1));

			CPoint pocket_first(0, 0);
			CPoint pocket_last(MAX_LD_TBL_POCKET_CNT_X - 1, MAX_LD_TBL_POCKET_CNT_Y - 1);

			double dTbl2_Left_Max = g_TaskTrayLoadPP.GetTeachPos(LOC_IDX_TRAY_PP_TBL_2, pocket_first, vPicker_Last, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX);
			double dTbl2_Right_Max = g_TaskTrayLoadPP.GetTeachPos(LOC_IDX_TRAY_PP_TBL_2, pocket_last, vPicker_First, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX);
			double dTbl3_Left_Max = g_TaskTrayLoadPP.GetTeachPos(LOC_IDX_TRAY_PP_TBL_3, pocket_first, vPicker_Last, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX);
			double dTbl3_Right_Max = g_TaskTrayLoadPP.GetTeachPos(LOC_IDX_TRAY_PP_TBL_3, pocket_last, vPicker_First, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX);

			if ((dTbl2_Left_Max - 200.0 <= dTrayCurPosX && dTrayCurPosX <= dTbl2_Right_Max + 200.0) || (dTbl3_Left_Max - 200.0 <= dTrayCurPosX && dTrayCurPosX <= dTbl3_Right_Max + 200.0))
			{
				bIsWorkPos = TRUE;
			}
		}
		else {
			//Picker (3,1)이 Stacker (0,0) Pos에 있을 때
			std::vector<CPoint> vPicker_Last;
			vPicker_Last.push_back(CPoint(MAX_TRAY_PP_CNT_X - 1, MAX_TRAY_PP_CNT_Y - 1));

			CPoint pocket_first(0, 0);
			double dStacker1_Left_Max = g_TaskTrayUnloadPP.GetTeachPos(LOC_IDX_TRAY_PP_CONV_4, pocket_first, vPicker_Last, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX);
			if (dTrayCurPosX >= dStacker1_Left_Max - 200.0)
			{
				bIsWorkPos = TRUE;
			}
		}

		if (bIsWorkPos != TRUE)
		{
			//Motion Done Check.
			OneAxis* pMotorFeederY = g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eAxisY);
			if (pMotorFeederY->MotionDone() != ERR_NO_ERROR && dCurPosY_unload >= -100000.0) {
				return ERR_MOT_INTERLOCK_FEEDER_Y_NOT_MOTION_DONE;
			}
		}

	}

	//Loading Table 영역일 때, Table Motion Done Check.
	//1. Module1 영역, Table 1 or Table 2
	double dTablePosLeft = (m_nTrayPpIdx == eTrayPP_1_LD) ? INTERLOCK_LD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_LEFT : INTERLOCK_UD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_LEFT;
	double dTablePosRight = (m_nTrayPpIdx == eTrayPP_1_LD) ? INTERLOCK_LD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_RIGHT : INTERLOCK_UD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_RIGHT;
	if (dTrayCurPosX < dTablePosLeft)
	{
		int nTableIdx = (m_nTrayPpIdx == eTrayPP_1_LD) ? eLoadTbl_2 : eLoadTbl_1;
		OneAxis* pMotorTblY = g_TaskLoadTbl[nTableIdx].GetAxis(CTaskLoadTable::eAxisY);
		if (pMotorTblY->MotionDone() != ERR_NO_ERROR) {
			return (m_nTrayPpIdx == eTrayPP_1_LD) ? ERR_MOT_INTERLOCK_TRAY_PP_1_Z_MOVE_NOT_MOTION_DONE_LOAD_TABLE_2_Y : ERR_MOT_INTERLOCK_TRAY_PP_2_Z_MOVE_NOT_MOTION_DONE_LOAD_TABLE_1_Y;
		}

		OneAxis* pMotorRotate = g_TaskLoadTbl[nTableIdx].GetAxis(CTaskLoadTable::eAxisRotate);
		if (pMotorRotate->MotionDone() != ERR_NO_ERROR) {
			return (m_nTrayPpIdx == eTrayPP_1_LD) ? ERR_MOT_INTERLOCK_TRAY_PP_1_Z_MOVE_NOT_MOTION_DONE_LOAD_TABLE_2_ROTATE : ERR_MOT_INTERLOCK_TRAY_PP_2_Z_MOVE_NOT_MOTION_DONE_LOAD_TABLE_1_ROTATE;
		}
	}

	//2. Module2 영역, Table 3 or Table 4
	if (dTablePosRight < dTrayCurPosX)
	{
		if (dTrayCurPosY >= INTERLOCK_BASE_TRAYPP_Y_POS_WITH_TRAYPP_Z_AND_TBL_POSSIBLE){
			int nTableIdx = (m_nTrayPpIdx == eTrayPP_1_LD) ? eLoadTbl_3 : eLoadTbl_4;
			OneAxis* pMotorTblY = g_TaskLoadTbl[nTableIdx].GetAxis(CTaskLoadTable::eAxisY);
			if (pMotorTblY->MotionDone() != ERR_NO_ERROR) {
				return (m_nTrayPpIdx == eTrayPP_1_LD) ? ERR_MOT_INTERLOCK_TRAY_PP_1_Z_MOVE_NOT_MOTION_DONE_LOAD_TABLE_3_Y : ERR_MOT_INTERLOCK_TRAY_PP_2_Z_MOVE_NOT_MOTION_DONE_LOAD_TABLE_4_Y;
			}

			OneAxis* pMotorRotate = g_TaskLoadTbl[nTableIdx].GetAxis(CTaskLoadTable::eAxisRotate);
			if (pMotorRotate->MotionDone() != ERR_NO_ERROR) {
				return (m_nTrayPpIdx == eTrayPP_1_LD) ? ERR_MOT_INTERLOCK_TRAY_PP_1_Z_MOVE_NOT_MOTION_DONE_LOAD_TABLE_3_ROTATE : ERR_MOT_INTERLOCK_TRAY_PP_2_Z_MOVE_NOT_MOTION_DONE_LOAD_TABLE_4_ROTATE;
			}
		}
	}

	//추후 작업 진행.
	//Hand가 Loading Table 작업 영역 일때, Y축 및 Rotate축이 작업 위치인지 확인.
	/////////////////////////////////////////////////////////////////////////////////

	// 		int nTableIdx = (m_nTrayPpIdx == eTrayPP_1_LD) ? eLoadTbl_2 : eLoadTbl_1;
	// 		int nMotorPosY = g_TaskLoadTbl[nTableIdx].ChkMotorPos(LOC_IDX_TBL_LD_SIDE, CTaskLoadTable::eAxisY);
	// 		if (nMotorPosY == -1) // Table Motion Done
	// 		{
	// 			return (m_nTrayPpIdx == eTrayPP_1_LD) ? ERR_MOT_INTERLOCK_TRAY_PP_1_Z_MOVE_NOT_MOTION_DONE_LOAD_TABLE_2_Y : ERR_MOT_INTERLOCK_TRAY_PP_2_Z_MOVE_NOT_MOTION_DONE_LOAD_TABLE_1_Y;
	// 		}
	// 
	// 		if (nMotorPosY == -2) // Table Position Range
	// 		{
	// 
	// 		}

	/////////////////////////////////////////////////////////////////////////////////
	return ERR_NO_ERROR;
}
int CMotorInterlock_TrayPP::Interlock_z_jog( double dSpd, int nAcc, int nDcc )
{
	int nErr = CBaseMotorInterlock::CommonInterlock();
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}

	nErr = Interlock_z_common();
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}

	OneAxis* pMotorZ = (m_nTrayPpIdx == eTrayPP_1_LD) ? g_TaskTrayLoadPP.GetAxis(CTaskTrayBasePP::eAxisZ) : g_TaskTrayUnloadPP.GetAxis(CTaskTrayBasePP::eAxisZ);
	BOOL bOriginZ = pMotorZ->IsOrgCompleted(nullptr);
	if (bOriginZ != TRUE) {
		return ERR_MOT_INTERLOCK_TRAY_PP_1_Z_AXIS_IS_NOT_ORIGIN + m_nTrayPpIdx;
	}

	return ERR_NO_ERROR;
}
int CMotorInterlock_TrayPP::Interlock_xpitch_org()
{
	int nErr = Interlock_TrayPP_Common();
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}
// 	nErr = Interlock_xpitch_common();
// 	if( nErr != ERR_NO_ERROR ){
// 		return nErr;
// 	}

	return ERR_NO_ERROR;
}
int CMotorInterlock_TrayPP::Interlock_xpitch_mov(double dTPos, double dSpd, int nAcc, int nDcc)
{
	int nErr = Interlock_TrayPP_Common();
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}
// 	nErr = Interlock_xpitch_common(); //servo
// 	if( nErr != ERR_NO_ERROR ){
// 		return nErr;
// 	}

	return ERR_NO_ERROR;
}
int CMotorInterlock_TrayPP::Interlock_xpitch_jog( double dSpd, int nAcc, int nDcc )
{
	int nErr = Interlock_TrayPP_Common();
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
// 	nErr = Interlock_xpitch_common();
// 	if( nErr != ERR_NO_ERROR ) {
// 		return nErr;
// 	}

	return ERR_NO_ERROR;
}
int CMotorInterlock_TrayPP::Interlock_ypitch_org()
{
	int nErr = Interlock_TrayPP_Common();
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}
// 	nErr = Interlock_ypitch_common();
// 	if( nErr != ERR_NO_ERROR ){
// 		return nErr;
// 	}

	return ERR_NO_ERROR;
}
int CMotorInterlock_TrayPP::Interlock_ypitch_mov(double dTPos, double dSpd, int nAcc, int nDcc)
{
	int nErr = Interlock_TrayPP_Common();
	if( nErr != ERR_NO_ERROR ){
		return nErr;
	}
// 	nErr = Interlock_ypitch_common();
// 	if (nErr != ERR_NO_ERROR) {
// 		return nErr;
// 	}

	return ERR_NO_ERROR;
}

int CMotorInterlock_TrayPP::Interlock_ypitch_jog( double dSpd, int nAcc, int nDcc )
{
	int nErr = Interlock_TrayPP_Common();
	if( nErr != ERR_NO_ERROR ) {
		return nErr;
	}
// 	nErr = Interlock_ypitch_common();
// 	if( nErr != ERR_NO_ERROR ) {
// 		return nErr;
// 	}

	return ERR_NO_ERROR;
}

BOOL CMotorInterlock_TrayPP::Interlock_TrayPP_Feeder_Impact_Pos(int nHandType, double dCurPos, std::vector<int> &vImpactPkr)
{	
	std::vector<int> vPkr;
	vPkr.clear();

	if (nHandType == eTrayPP_1_LD)
	{
		if (INTERLOCK_LD_TRAYPP_X_WORK_ENABLE_FEEDER_START_POS < dCurPos && dCurPos < INTERLOCK_LD_TRAYPP_X_WORK_ENABLE_FEEDER_END_POS)
		{
			// Interlock 강화 [8/9/2024 dohyeong.kim]
			if (INTERLOCK_LD_TRAY_PP_PKR4_FEEDER_LEFT_IMPACT_POS <= dCurPos && dCurPos <= INTERLOCK_LD_TRAY_PP_PKR3_FEEDER_LEFT_IMPACT_POS)
			{
				//Feeder 좌측
				//Picker 1 Pos Check
				vPkr.push_back(eDirPicker_Picker04);	vPkr.push_back(eDirPicker_Picker08);
			}
			else if (INTERLOCK_LD_TRAY_PP_PKR3_FEEDER_LEFT_IMPACT_POS <= dCurPos && dCurPos <= INTERLOCK_LD_TRAY_PP_PKR2_FEEDER_LEFT_IMPACT_POS)
			{
				//Feeder 좌측
				//Picker 2 Pos Check
				vPkr.push_back(eDirPicker_Picker03);	vPkr.push_back(eDirPicker_Picker07);
				vPkr.push_back(eDirPicker_Picker04);	vPkr.push_back(eDirPicker_Picker08);
			}
			else if (INTERLOCK_LD_TRAY_PP_PKR2_FEEDER_LEFT_IMPACT_POS <= dCurPos && dCurPos <= INTERLOCK_LD_TRAY_PP_PKR1_FEEDER_LEFT_IMPACT_POS)
			{
				//Feeder 좌측
				//Picker 3 Pos Check
				vPkr.push_back(eDirPicker_Picker02);	vPkr.push_back(eDirPicker_Picker06);
				vPkr.push_back(eDirPicker_Picker03);	vPkr.push_back(eDirPicker_Picker07);
				vPkr.push_back(eDirPicker_Picker04);	vPkr.push_back(eDirPicker_Picker08);

			}
			else if (INTERLOCK_LD_TRAY_PP_PKR2_FEEDER_RIGHT_IMPACT_POS <= dCurPos && dCurPos <= INTERLOCK_LD_TRAY_PP_PKR1_FEEDER_RIGHT_IMPACT_POS)
			{
				//Feeder 우측
				//Picker 1 Pos Check
				vPkr.push_back(eDirPicker_Picker01);	vPkr.push_back(eDirPicker_Picker05);

			}
			else if (INTERLOCK_LD_TRAY_PP_PKR3_FEEDER_RIGHT_IMPACT_POS <= dCurPos && dCurPos <= INTERLOCK_LD_TRAY_PP_PKR2_FEEDER_RIGHT_IMPACT_POS)
			{
				//Feeder 우측
				//Picker 2 Pos Check
				vPkr.push_back(eDirPicker_Picker01);	vPkr.push_back(eDirPicker_Picker05);
				vPkr.push_back(eDirPicker_Picker02);	vPkr.push_back(eDirPicker_Picker06);
			}
			else if (INTERLOCK_LD_TRAY_PP_PKR4_FEEDER_RIGHT_IMPACT_POS <= dCurPos && dCurPos <= INTERLOCK_LD_TRAY_PP_PKR3_FEEDER_RIGHT_IMPACT_POS)
			{
				//Feeder 우측
				//Picker 3 Pos Check
				vPkr.push_back(eDirPicker_Picker01);	vPkr.push_back(eDirPicker_Picker05);
				vPkr.push_back(eDirPicker_Picker02);	vPkr.push_back(eDirPicker_Picker06);
				vPkr.push_back(eDirPicker_Picker03);	vPkr.push_back(eDirPicker_Picker07);
			}
			else
			{
				//Picker 4 Pos Check(All Picker)
				vPkr.push_back(eDirPicker_Picker01);	vPkr.push_back(eDirPicker_Picker05);
				vPkr.push_back(eDirPicker_Picker02);	vPkr.push_back(eDirPicker_Picker06);
				vPkr.push_back(eDirPicker_Picker03);	vPkr.push_back(eDirPicker_Picker07);
				vPkr.push_back(eDirPicker_Picker04);	vPkr.push_back(eDirPicker_Picker08);
			}

			vImpactPkr = vPkr;
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if (INTERLOCK_UD_TRAYPP_X_WORK_ENABLE_FEEDER_START_POS < dCurPos && dCurPos < INTERLOCK_UD_TRAYPP_X_WORK_ENABLE_FEEDER_END_POS)
		{
			// Interlock 강화 [8/9/2024 dohyeong.kim]
			if (INTERLOCK_UD_TRAY_PP_PKR4_FEEDER_LEFT_IMPACT_POS <= dCurPos && dCurPos <= INTERLOCK_UD_TRAY_PP_PKR3_FEEDER_LEFT_IMPACT_POS)
			{
				//Feeder 좌측
				//Picker 1 Pos Check
				vPkr.push_back(eDirPicker_Picker04);	vPkr.push_back(eDirPicker_Picker08);
			}
			else if (INTERLOCK_UD_TRAY_PP_PKR3_FEEDER_LEFT_IMPACT_POS <= dCurPos && dCurPos <= INTERLOCK_UD_TRAY_PP_PKR2_FEEDER_LEFT_IMPACT_POS)
			{
				//Feeder 좌측
				//Picker 2 Pos Check
				vPkr.push_back(eDirPicker_Picker03);	vPkr.push_back(eDirPicker_Picker07);
				vPkr.push_back(eDirPicker_Picker04);	vPkr.push_back(eDirPicker_Picker08);
			}
			else if (INTERLOCK_UD_TRAY_PP_PKR2_FEEDER_LEFT_IMPACT_POS <= dCurPos && dCurPos <= INTERLOCK_UD_TRAY_PP_PKR1_FEEDER_LEFT_IMPACT_POS)
			{
				//Feeder 좌측
				//Picker 3 Pos Check
				vPkr.push_back(eDirPicker_Picker02);	vPkr.push_back(eDirPicker_Picker06);
				vPkr.push_back(eDirPicker_Picker03);	vPkr.push_back(eDirPicker_Picker07);
				vPkr.push_back(eDirPicker_Picker04);	vPkr.push_back(eDirPicker_Picker08);

			}
			else if (INTERLOCK_UD_TRAY_PP_PKR2_FEEDER_RIGHT_IMPACT_POS <= dCurPos && dCurPos <= INTERLOCK_UD_TRAY_PP_PKR1_FEEDER_RIGHT_IMPACT_POS)
			{
				//Feeder 우측
				//Picker 1 Pos Check
				vPkr.push_back(eDirPicker_Picker01);	vPkr.push_back(eDirPicker_Picker05);

			}
			else if (INTERLOCK_UD_TRAY_PP_PKR3_FEEDER_RIGHT_IMPACT_POS <= dCurPos && dCurPos <= INTERLOCK_UD_TRAY_PP_PKR2_FEEDER_RIGHT_IMPACT_POS)
			{
				//Feeder 우측
				//Picker 2 Pos Check
				vPkr.push_back(eDirPicker_Picker01);	vPkr.push_back(eDirPicker_Picker05);
				vPkr.push_back(eDirPicker_Picker02);	vPkr.push_back(eDirPicker_Picker06);
			}
			else if (INTERLOCK_UD_TRAY_PP_PKR4_FEEDER_RIGHT_IMPACT_POS <= dCurPos && dCurPos <= INTERLOCK_UD_TRAY_PP_PKR3_FEEDER_RIGHT_IMPACT_POS)
			{
				//Feeder 우측
				//Picker 3 Pos Check
				vPkr.push_back(eDirPicker_Picker01);	vPkr.push_back(eDirPicker_Picker05);
				vPkr.push_back(eDirPicker_Picker02);	vPkr.push_back(eDirPicker_Picker06);
				vPkr.push_back(eDirPicker_Picker03);	vPkr.push_back(eDirPicker_Picker07);
			}
			else
			{
				//Picker 4 Pos Check(All Picker)
				vPkr.push_back(eDirPicker_Picker01);	vPkr.push_back(eDirPicker_Picker05);
				vPkr.push_back(eDirPicker_Picker02);	vPkr.push_back(eDirPicker_Picker06);
				vPkr.push_back(eDirPicker_Picker03);	vPkr.push_back(eDirPicker_Picker07);
				vPkr.push_back(eDirPicker_Picker04);	vPkr.push_back(eDirPicker_Picker08);
			}

			vImpactPkr = vPkr;
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	

}