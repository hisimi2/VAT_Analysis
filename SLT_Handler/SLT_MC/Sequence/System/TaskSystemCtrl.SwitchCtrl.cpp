#include "StdAfx.h"
#include "TaskSystemCtrl.h"
#include "ConfigData.h"
#include "PressUnitConvert.h"

/*=============================================================================
  AUTHOR   : 나명진  [2015-11-06]
  NAME     : 
  PARAMS   : a_vSignal : signal io id들
             a_bSigStateForAlarm : alarm일때의 signal 상태 정의
			 a_nAlarmId : 발생 시켜야 할 alarm_id
  RETURN   : 
  COMMENTS : 
==============================================================================*/

void CTaskSystemCtrl::System_Signal_AlarmReport(std::vector<int>& a_vSignal, BOOL a_bSigStateForAlarm, int a_nAlarmId, BOOL bType/*=eSensor_TypeA*/)
{
	CParamList AlarmData;
	for(int i=0; i<(int)a_vSignal.size(); i++)
	{
		BOOL bSigState = a_bSigStateForAlarm;
		if(bType == eSensor_TypeB){
			bSigState = !bSigState;
		}

		if( g_IO.in(a_vSignal[i]) == bSigState )
		{
			CMyList rDiInfo;
			g_DB.SELECT_DIO_TBL(_tDIO_LS::eDI, a_vSignal[i], 1, rDiInfo);
			if( rDiInfo.m_fnGetCount() > 0 )
			{
				_tDIO_LS* pItem = (_tDIO_LS*)rDiInfo.m_fnGetData(0);
				if( pItem )
				{
					pItem->name.TrimLeft();
					pItem->name.TrimRight();
					AlarmData.m_fnAddItemFormat("%s : %s = %s", pItem->label ,pItem->name, (g_IO.in(a_vSignal[i]))?_T("ON"):_T("OFF") );
				}
			}
		}
	}
	g_Error.AlarmReport(a_nAlarmId, AlarmData, this);
}

void CTaskSystemCtrl::_Check_Regulator()
{
	double dForcePerPin = g_DMCont.m_dmHandlerOpt.GD(DDM11_DvcContactForce_GF);
	int nForcePin       = g_DMCont.m_dmHandlerOpt.GN(NDM11_DvcContactForce_Pin);
	if( dForcePerPin <= 0 || nForcePin <= 0){
		return;
	}

	CString strRegulatorName = _T("");
	CParamList AlarmData;

	int nChk=0, nChkSocket=0;
	CString strNo = _T("");
	for(int i=0; i< eMaxPressUnitCount; i++){
		BOOL bRet = g_TaskRegulator[i].ChkPermitNotOver(10.0); // +- 10% check
		nChkSocket = g_TaskPressUnit[i].GetSocketOnCnt();

		if( bRet == FALSE && nChkSocket>0){
			strNo.Format("[%d]",i+1);
			strRegulatorName += strNo;
			nChk++;
		}
	}

	for (int i = 0; i < eMaxPressUnitCount; i++) {
		double dCurPressure = g_TaskRegulator[i].GetCurrentPressure();
		CPressUnitConvert cvt(CPressUnitConvert::MPa, dCurPressure);
		int nCurPressureUnit = CPressUnitConvert::MPa;
		double dDisplayPressure = cvt.ConvertTo(nCurPressureUnit);
		int nReguratorNo;
		switch (i)
		{
		case ePressUnit_Down_1: case ePressUnit_Down_2: case ePressUnit_Down_3: case ePressUnit_Down_4:
		{
			nReguratorNo = ePressRegul_Left;
		}break;
		case ePressUnit_Down_5: case ePressUnit_Down_6: case ePressUnit_Down_7: case ePressUnit_Down_8:
		{
			nReguratorNo = ePressRegul_Right;
		}break;
		}

		g_DMCont.m_dmHandlerOpt.SD(DDM11_CurPressure1+nReguratorNo, dDisplayPressure);
	}

	if (      g_IO.in(DI_MAIN_AIR1_SEN) != DEF_ON
		   || g_IO.in(DI_MAIN_AIR2_SEN) != DEF_ON)
	{
		if (m_tcChkMainAIRPressProblemForEP_Reg.IsChecking() != TRUE)
		{
			m_tcChkMainAIRPressProblemForEP_Reg.SetCheckTime(3.0);
			m_tcChkMainAIRPressProblemForEP_Reg.StartTimer();
		}
		else
		{
			int nIdxFirstVal = BDM0_TEST_STATUS_SITE1;
			if (m_tcChkMainAIRPressProblemForEP_Reg.CheckOverTime() == TRUE)
			{
				for (int i = 0; i < eMaxPressUnitCount; i++)
				{
					nIdxFirstVal = BDM0_TEST_STATUS_SITE1;	// left station 탐색

					if (i == 1)
					{
						nIdxFirstVal = BDM0_TEST_STATUS_SITE5;	// right station 탐색
					}

					int nSearch = 0;
					BOOL bZeroOK = TRUE;

					for (nSearch = 0; nSearch < EACH_LINE_STATION_CNT; nSearch++)	// station 4개중 한개로도 Test중이면 EP ragulator 끄지 않는다.
					{
						BOOL bVal = g_DMCont.m_dmEQP.GB(nIdxFirstVal + nSearch);	// Tester test 중인지 확인 한다.

						if (bVal == TRUE)
						{
							bZeroOK = FALSE; // test 중인 station이 있으면 해당 EP regulator는 0.0으로 설정 하지 않는다.
						}
					}

					if( bZeroOK == TRUE)
					{
						g_TaskRegulator[i].do_SetForcePerPin(0.0);
					}
				}

				m_tcChkMainAIRPressProblemForEP_Reg.SetTimerCheckEnd();
			}
		}

		return;//  Main AIr 이상 알람이 우선이라 Main Air 공급 상태 이상시 EP regulator 알람 미발생. 
	}
	else
	{
		m_tcChkMainAIRPressProblemForEP_Reg.SetTimerCheckEnd();
	}

	BOOL bCrtlMode = g_TaskSystemCtrl.GetEqpCtrlMode();
	int nLotStatus = GetCurrentLotState();
	if ( g_DMCont.m_dmEQP.GB(BDM0_EQP_RUN_STATE) == TRUE && bCrtlMode == eEqpCtrlMode_Auto 
		&& (nLotStatus == eAutoLotState_Executing || nLotStatus == eAutoLotState_Resume ))
	{
		if( nChk == 0 )
		{
			m_nRetryRegulator = 0;
			m_tcRegulator.SetTimerCheckEnd();
		}
		if( nChk > 0 && m_tcRegulator.IsChecking() == FALSE && g_DMCont.m_dmEQP.GB(BDM0_EQP_RUN_STATE) == TRUE)
		{
			m_tcRegulator.SetCheckTime(3.0);
			m_tcRegulator.StartTimer();
		}

		if(m_tcRegulator.CheckOverTime() && m_tcRegulator.IsChecking() )
		{
			m_tcRegulator.SetTimerCheckEnd();

			if (m_nRetryRegulator < 2) // 2회 retry 시도를 한다.
			{
				for (int i = 0; i < eMaxPressUnitCount; i++) {
					BOOL bRet = g_TaskRegulator[i].ChkPermitNotOver(10.0); // +- 10% check
					if (bRet == FALSE && g_TaskPressUnit[i].GetSocketOnCnt() > 0) {
						g_TaskRegulator[i].do_SetForcePerPin(dForcePerPin);
					}
				}
				m_nRetryRegulator++;
			}
			else {
				if (g_Error.IsSetAlarm(ERR_SYS_REGULATOR_PRESS_IS_ALARM) == FALSE) {
					AlarmData.m_fnAddItemFormat("Problem Regulator No = %s", strRegulatorName);
					g_Error.AlarmReport(ERR_SYS_REGULATOR_PRESS_IS_ALARM, AlarmData, this);
				}
				OnLotCommand(eAutoLotCmd_AlarmPause, NULL, NULL);
				m_nRetryRegulator = 0;
			}
		}
	}
}

void CTaskSystemCtrl::_Check_Sig_EMO()
{
	int nChk=0;
	for(int i=0; i<(int)m_vEMO_Sig.size(); i++){
		nChk += g_IO.in(m_vEMO_Sig[i]);
	}

    if( nChk < (int)m_vEMO_Sig.size())
    {
        //SendEvent(CEID_EMO_SW_OFF);
		if( g_Error.IsSetAlarm(ERR_SYS_EMO_IS_ON) == FALSE){
			System_Signal_AlarmReport(m_vEMO_Sig, DEF_OFF, ERR_SYS_EMO_IS_ON);
		}
        
        OnLotCommand(eAutoLotCmd_AlarmPause, NULL, NULL);        
    }
}

//void CTaskSystemCtrl::_Check_Sig_Main_Air_VacCyl()
//{
//	int nChk=0;
//	for(int i=0; i<(int)m_vMainAir_VacCyl_Sig.size(); i++){
//		nChk += !(g_IO.in(m_vMainAir_VacCyl_Sig[i]));
//	}
//
//	if( nChk == 0 ){
//		//SendEvent(CEID_MAIN_AIR_SIG_IS_ON);
//		m_tcMainVacCylAir.SetTimerCheckEnd();
//	}
//	if( nChk != 0 && m_tcMainVacCylAir.IsChecking() == FALSE){
//		m_tcMainVacCylAir.SetCheckTime(3.0);
//		m_tcMainVacCylAir.StartTimer();
//	}
//	if(m_tcMainVacCylAir.CheckOverTime() && m_tcMainVacCylAir.IsChecking() )
//	{
//		//SendEvent(CEID_MAIN_AIR_SIG_IS_OFF);
//		m_tcMainVacCylAir.SetTimerCheckEnd();
//		if( g_Error.IsSetAlarm(ERR_SYS_MAIN_AIR_IS_ALARM) == FALSE){
//			System_Signal_AlarmReport(m_vMainAir_VacCyl_Sig, DEF_OFF, ERR_SYS_MAIN_AIR_IS_ALARM);
//		}
//		OnLotCommand(eAutoLotCmd_AlarmPause, NULL, NULL);
//	}
//}

void CTaskSystemCtrl::_Check_PressTestMainAir()
{
		int nChk=0;
		for(int i=0; i<(int)m_vMainAir_PressTest_Sig.size(); i++){
			nChk += !(g_IO.in(m_vMainAir_PressTest_Sig[i]));
		}
	
		if( nChk == 0 ){
			m_tcMainPressTestAir.SetTimerCheckEnd();
		}
		if( nChk != 0 && m_tcMainPressTestAir.IsChecking() == FALSE ){
			m_tcMainPressTestAir.SetCheckTime(3.0);
			m_tcMainPressTestAir.StartTimer();
		}
		if(m_tcMainPressTestAir.CheckOverTime() && m_tcMainPressTestAir.IsChecking() ){
			m_tcMainPressTestAir.SetTimerCheckEnd();
			if( g_Error.IsSetAlarm(ERR_SYS_PRESS_TEST_MAIN_IS_ALARM) == FALSE){
				System_Signal_AlarmReport(m_vMainAir_PressTest_Sig, DEF_OFF, ERR_SYS_PRESS_TEST_MAIN_IS_ALARM);
			}
			OnLotCommand(eAutoLotCmd_AlarmPause, NULL, NULL);
		}
}

void CTaskSystemCtrl::_Check_StackerCtrayMainAir()
{
	int nChk = 0;
	for (int i = 0; i<(int)m_vMainAir_StackerCtray_Sig.size(); i++) {
		nChk += !(g_IO.in(m_vMainAir_StackerCtray_Sig[i]));
	}

	if (nChk == 0) {
		m_tcMainStackerCtrayAir.SetTimerCheckEnd();
	}
	if (nChk != 0 && m_tcMainStackerCtrayAir.IsChecking() == FALSE) {
		m_tcMainStackerCtrayAir.SetCheckTime(3.0);
		m_tcMainStackerCtrayAir.StartTimer();
	}
	if (m_tcMainStackerCtrayAir.CheckOverTime() && m_tcMainStackerCtrayAir.IsChecking()) {
		m_tcMainStackerCtrayAir.SetTimerCheckEnd();
		if (g_Error.IsSetAlarm(ERR_SYS_STACKER_TRAY_IS_ALARM) == FALSE) {
			System_Signal_AlarmReport(m_vMainAir_StackerCtray_Sig, DEF_OFF, ERR_SYS_STACKER_TRAY_IS_ALARM);
		}
		OnLotCommand(eAutoLotCmd_AlarmPause, NULL, NULL);
	}
}

void CTaskSystemCtrl::_Check_Sig_MainPow()
{
	int nChk=0;
	for(int i=0; i<(int)m_vMainPow_Sig.size(); i++){
		nChk += !g_IO.in(m_vMainPow_Sig[i]);
	}

	if( nChk == 0){
		SendEvent(CEID_MAIN_POWER_ON);
		m_tcMainPow.SetTimerCheckEnd();
	}
	if( nChk != 0 && m_tcMainPow.IsChecking() == FALSE){
		m_tcMainPow.SetCheckTime(3.0);
		m_tcMainPow.StartTimer();
	}
	if( m_tcMainPow.CheckOverTime() && m_tcMainPow.IsChecking() ){
		SendEvent(CEID_MAIN_POWER_OFF);
		m_tcMainPow.SetTimerCheckEnd();
		if( g_Error.IsSetAlarm(ERR_SYS_MAIN_POW_IS_OFF) == FALSE){
			System_Signal_AlarmReport(m_vMainPow_Sig, DEF_OFF, ERR_SYS_MAIN_POW_IS_OFF);
		}
		OnLotCommand(eAutoLotCmd_AlarmPause, NULL, NULL);
	}
}

void CTaskSystemCtrl::_Check_Sig_ServoPow()
{
// 	int nChk=0;
// 	for(int i=0; i<(int)m_vServoPow_Sig.size(); i++){
// 		nChk += !g_IO.in(m_vServoPow_Sig[i]);
// 	}
// 
// 	if( nChk == 0){
// 		SendEvent(CEID_SERVO_POWER_SIG_ON);
// 		m_tcServoPow.SetTimerCheckEnd();
// 	}
// 	if( nChk != 0 && m_tcServoPow.IsChecking() == FALSE ){
// 		m_tcServoPow.SetCheckTime(3.0);
// 		m_tcServoPow.StartTimer();
// 	}
// 	if( m_tcServoPow.IsChecking() && m_tcServoPow.CheckOverTime() ){
// 		SendEvent(CEID_SERVO_POWER_SIG_OFF);
// 		m_tcServoPow.SetTimerCheckEnd();
// 		if( g_Error.IsSetAlarm(ERR_SYS_SERVO_POW_IS_OFF) == FALSE && g_COVER_SW.GetStatus() ){
// 			System_Signal_AlarmReport(m_vServoPow_Sig, DEF_OFF, ERR_SYS_SERVO_POW_IS_OFF);
// 			OnLotCommand(eAutoLotCmd_AlarmPause, NULL, NULL);
// 		}		
// 	}
}

void CTaskSystemCtrl::_Check_Sig_Ion_Alarm()
{
	BOOL bRetOneCycle = g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY);
	int nIonizer = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_ALL_UNIT_IONIZER);

	if ((nIonizer && bRetOneCycle) || (nIonizer && g_TaskSystemCtrl.GetCurrentLotState() == eAutoLotState_Idle))
	{
		m_tcIonAlarm.SetTimerCheckEnd();
		return; 
	}

	int nChk=0;
	for(int i=0; i<(int)m_vIonAlarm_Sig.size(); i++){
		nChk += !g_IO.in(m_vIonAlarm_Sig[i]);
	}

	if( nChk == (int)m_vIonAlarm_Sig.size()){
		//SendEvent(CEID_ION_ALARM_ON);
		m_tcIonAlarm.SetTimerCheckEnd();
	}
	if( nChk != 0 && m_tcIonAlarm.IsChecking() == FALSE ){
		m_tcIonAlarm.SetCheckTime(3.0);
		m_tcIonAlarm.StartTimer();
	}
	if( m_tcIonAlarm.IsChecking() && m_tcIonAlarm.CheckOverTime() ){
		//SendEvent(CEID_ION_ALARM_OFF);
		m_tcIonAlarm.SetTimerCheckEnd();

		if( g_Error.IsSetAlarm(ERR_ION_ALARM) == FALSE){
			System_Signal_AlarmReport(m_vIonAlarm_Sig, DEF_OFF, ERR_ION_ALARM);
		}
		OnLotCommand(eAutoLotCmd_AlarmPause, NULL, NULL);
	}
}

void CTaskSystemCtrl::_Check_Sig_Ion_Fan_Alarm()
{
	BOOL bRetOneCycle = g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY);
	int nIonizer = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_ALL_UNIT_IONIZER);
	if ((nIonizer && bRetOneCycle) || (nIonizer && g_TaskSystemCtrl.GetCurrentLotState() == eAutoLotState_Idle))
	{
		m_tcIonFanAlarm.SetTimerCheckEnd();
		return;
	}

	std::vector<int> vIonFanAlarm_Sig;
	vIonFanAlarm_Sig.clear();

	int nChk=0;
	for(int i=0; i<(int)m_vIonFanAlarm_Sig.size(); i++){
		nChk += g_IO.in(m_vIonFanAlarm_Sig[i]);
	}

	if( nChk == (int)m_vIonFanAlarm_Sig.size()){
		//SendEvent(CEID_ION_FAN_ALARM_OFF);
		m_tcIonFanAlarm.SetTimerCheckEnd();
	}
	if( nChk != 0 && m_tcIonFanAlarm.IsChecking() == FALSE ){
		m_tcIonFanAlarm.SetCheckTime(3.0);
		m_tcIonFanAlarm.StartTimer();
	}
	if( m_tcIonFanAlarm.IsChecking() && m_tcIonFanAlarm.CheckOverTime() ){
		m_tcIonFanAlarm.SetTimerCheckEnd();
		//SendEvent(CEID_ION_FAN_ALARM_ON);

		if( g_Error.IsSetAlarm(ERR_ION_FAN_ALARM) == FALSE){
			System_Signal_AlarmReport(m_vIonFanAlarm_Sig, DEF_ON, ERR_ION_FAN_ALARM);
		}
		OnLotCommand(eAutoLotCmd_AlarmPause, NULL, NULL);
	}
}

void CTaskSystemCtrl::_Check_Sig_CPTrip()
{
	int nChk=0;
	for(int i=0; i<(int)m_vCPTrip_Sig.size(); i++){
		nChk += !g_IO.in(m_vCPTrip_Sig[i]);
	}

	if( nChk == 0 ){
		//SendEvent(CEID_CP_TRIP_SIG_IS_ON);
		m_tcCPTrip.SetTimerCheckEnd();
	}
	if( nChk != 0 && m_tcCPTrip.IsChecking() == FALSE ){
		//SendEvent(CEID_CP_TRIP_SIG_IS_OFF);
		m_tcCPTrip.SetCheckTime(3.0);
		m_tcCPTrip.StartTimer();
	}
	if( m_tcCPTrip.IsChecking() && m_tcCPTrip.CheckOverTime() ){
		//SendEvent(CEID_CP_TRIP_SIG_IS_OFF);
		m_tcCPTrip.SetTimerCheckEnd();
		if( g_Error.IsSetAlarm(ERR_CP_IS_TRIP) == FALSE){
			System_Signal_AlarmReport(m_vCPTrip_Sig, DEF_OFF, ERR_CP_IS_TRIP);
		}
		OnLotCommand(eAutoLotCmd_AlarmPause, NULL, NULL);
	}
}


void CTaskSystemCtrl::_Check_Sig_DownDoor()
{
	for (int i = 0; i < (int)m_vDownDoor_Sig.size(); i++) {
		 g_DMCont.m_dmEQP.SB(BDM0_FRONT_LEFT_DOORLOCK_SEN+i, g_IO.in(m_vDownDoor_Sig[i]));
	}

    if(g_COVER_SW.GetStatus())// 시정조치 Cover 되어있을때 확인하게
	{
		int nChk=0;
		if( !DEF_IO_CHECK_MODE ) {
			for (int i = 0; i < (int)m_vDownDoor_Sig.size(); i++) {
				nChk += !(g_IO.in(m_vDownDoor_Sig[i]));
			}
		}

		if(nChk == 0){
			//SendEvent(CEID_DOWN_DOOR_SIG_IS_OFF);
			m_tcDownDoor.SetTimerCheckEnd();
		}
		if(nChk != 0 && m_tcDownDoor.IsChecking() == FALSE ){
		    m_tcDownDoor.SetCheckTime(3.0);
		    m_tcDownDoor.StartTimer();
		}
		if( m_tcDownDoor.CheckOverTime() && m_tcDownDoor.IsChecking())
		{
		    m_tcDownDoor.SetTimerCheckEnd();
			//SendEvent(CEID_DOWN_DOOR_SIG_IS_ON);

 			if( g_Error.IsSetAlarm(ERR_DOOR_IS_OFF) == FALSE){
 				System_Signal_AlarmReport(m_vDownDoor_Sig, DEF_OFF, ERR_DOOR_IS_OFF);
 			}        
		     OnLotCommand(eAutoLotCmd_AlarmPause, NULL, NULL);        
		}
	}
}

void CTaskSystemCtrl::_Check_Switch_Mode()
{
	BOOL bBeforeMode = m_bFrontMode;

// 	if(g_IO.in(DI_FRONT_SW_SEN) == DEF_ON)
// 	{
// 		m_bFrontMode = DEF_ON;
// 	}else if(g_IO.in(DI_REAR_SW_SEN)== DEF_ON){
// 		m_bFrontMode = DEF_OFF;
// 	}else{
// 		m_bFrontMode = DEF_DESABLE;
// 	}
// 	if(bBeforeMode != m_bFrontMode){
// 		g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_FrontSW,m_bFrontMode);
// 	}
}

void CTaskSystemCtrl::_Check_Recent_OS_Version()
{
	bool version_correct = g_DMCont.m_dmHandlerOpt.GB(BDM11_OS_RECENT_VER);
	if (!version_correct) {
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Handler OS is not the latest version");
		g_Error.AlarmReport(ERR_HANDLER_OS_VERSION_FAIL, AlarmData, this);
	}
}

void CTaskSystemCtrl::_Check_HddRemainSize()
{
	int nLimit = g_DMCont.m_dmEQP.GN(NDM0_HDD_SIZE_ALARM);

	ULARGE_INTEGER avail, total, free;
	GetDiskFreeSpaceEx(_T("D:/"), &avail, &total, &free );
	
	double disk_size= (int)(total.QuadPart>>20); //MBytes로 처리 

	//GBytes total.QuadPart>>30 , KBytes : total.QuadPart>>10
	//disk_free_size;
	double disk_free_size = (int)(free.QuadPart>>20);
	double dRemainSize = (disk_free_size/disk_size) * 100.0;
	if( nLimit >= dRemainSize ){
		if( g_Error.IsSetAlarm(ERR_HARD_DISK_LOW_ALARM) == FALSE){
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Hard Disk remain space lower [Set=%d per, Current=%.2f per]", nLimit, dRemainSize);
			g_Error.AlarmReport(ERR_HARD_DISK_LOW_ALARM, AlarmData, this);
		}
	}

	int nLimitInterlock = g_DMCont.m_dmEQP.GN(NDM0_HDD_SIZE_INTERLOCK);
	if( nLimitInterlock >= dRemainSize ){
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Hard Disk remain space lower, Can not running [Set=%d per, Current=%.2f per]", nLimitInterlock, dRemainSize);
		g_Error.AlarmReport(ERR_HARD_DISK_LOW_INTERLOCK, AlarmData, NULL);
		OnLotCommand(eAutoLotCmd_AlarmPause, NULL, NULL); 
	}
}

void CTaskSystemCtrl::_Check_Sig_VisionBoardReset()
{
	if (m_VisionBoardReset.IsChecking() && m_VisionBoardReset.CheckOverTime()) {
		m_VisionBoardReset.SetTimerCheckEnd();

		for (int i = 0; i < (int)m_vVisionBoardReset_Sig.size(); i++) {
			if (g_IO.out(m_vVisionBoardReset_Sig[i]) == DEF_OFF)
				g_IO.out(m_vVisionBoardReset_Sig[i], DEF_ON);
		}
		m_VisionBoardReset.SetCheckTime(3.0);
		m_VisionBoardReset.StartTimer();
	}
}

void CTaskSystemCtrl::_Check_Online_Heart_Beat()
{
// #if DEF_TESTER_COMM_SPEC_ONE_SOCKET
// 	return;// 사용 하지 않아 바로 반환 처리.
// #endif

 	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
// 	if (nTesterOption == eTester_EachSocket) {
// 		return; // 사용 하지 않아 바로 반환 처리.
// 	}

//	CTesterStatus cTesterStatus;
	BOOL bAutoGrrUse  = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);
	int nRetSocket =0;
	BOOL bConnect = FALSE;
	//int nTesterOption = g_DMCont.m_dmEQP.GN(NDM11_Handler_Tester_Option);
	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
	int nStationNum = 0;
	if (nRunMode == eRUN_MODE_ONLINE)
	{
		m_csHeartBeat.Lock();

		int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
		int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

		if (nTesterOption == eTester_EachSocket)
		{
			for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++) {
				for (int nY = 0; nY < nSiteDivY; nY++) {
					for (int nX = 0; nX < nSiteDivX; nX++) {
						int nOnCnt = g_TaskPressUnit[nStation].GetSocketOnOff(nX, nY);
						int nEachSocketNumber = nX + nY*STATION_MAX_X_SITE + (nStation * nSiteDivY * nSiteDivX);
						BOOL bRegister = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + nEachSocketNumber);
						bConnect = g_AppleTestIF.IsConnected(nEachSocketNumber + 1);//SendAction(i, ACTION_STATUS);
						if (nOnCnt > 0 && bRegister == TRUE && bConnect == TRUE)
						{
							CTesterStatus cTesterStatus = g_AppleTestIF.GetTesterStatus(nEachSocketNumber + 1);

							if (cTesterStatus.m_bHeartBit == TRUE) {
								CTime cChkTime = CTime::GetCurrentTime();
								if (cChkTime - cTesterStatus.m_cChkTimeStamp > 8)
								{
									if (g_Error.IsSetAlarm(ERR_TESTER_SOCKET_1_HEART_BEAT + nEachSocketNumber) == FALSE) {
										g_AppleTestIF.HeartBeatOverCutClient(nEachSocketNumber + 1, eIdentityNo);
										CParamList AlarmData;
										AlarmData.m_fnAddItemFormat("[Station %d] - [Site %d] Hear Beat Error!! Please check for Tester", nStation+1, (nX + nY*STATION_MAX_X_SITE) +1);
										g_Error.AlarmReport(ERR_TESTER_SOCKET_1_HEART_BEAT + nEachSocketNumber, AlarmData, &g_TaskSystemCtrl);
									}
									OnLotCommand(eAutoLotCmd_AlarmPause, NULL, NULL);
								}
							}
						}
					}
				}
			}
		}
		else if (nTesterOption == eTester_Dual)
		{
			for (int i = 0; i < MAX_TESTER_DUAL; i++) {
				int nOnCnt = g_TaskPressUnit[i / 2].GetDirDualSocketOnCnt(bAutoGrrUse, i % 2);
				BOOL bRegister = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + i);
				bConnect = g_AppleTestIF.IsConnected(i + 1);//SendAction(i, ACTION_STATUS);
				if (nOnCnt > 0 && bRegister == TRUE && bConnect == TRUE)
				{
					CTesterStatus cTesterStatus = g_AppleTestIF.GetTesterStatus(i + 1);

					if (cTesterStatus.m_bHeartBit == TRUE){
						CTime cChkTime = CTime::GetCurrentTime();
						if (cChkTime - cTesterStatus.m_cChkTimeStamp > 8) 
						{
							if (g_Error.IsSetAlarm(ERR_D_TESTER_1_HEART_BEAT + i) == FALSE) {
								g_AppleTestIF.HeartBeatOverCutClient(i + 1);
								CParamList AlarmData;
								double dStationNum = ((i + 1.0) / 2.0);
								nStationNum = (int)round(dStationNum);
								AlarmData.m_fnAddItemFormat("Station %d Hear Beat Error!! Please check for Tester", nStationNum);
								g_Error.AlarmReport(ERR_D_TESTER_1_HEART_BEAT + i, AlarmData, &g_TaskSystemCtrl);
							}
							OnLotCommand(eAutoLotCmd_AlarmPause, NULL, NULL);
						}
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < eMaxPressUnitCount; i++) {
				int nOnCnt = g_TaskPressUnit[i].GetSocketOnCnt(bAutoGrrUse);
				BOOL bRegister = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + i);
				bConnect = g_AppleTestIF.IsConnected(i + 1);//SendAction(i, ACTION_STATUS);
				if (nOnCnt > 0 && bRegister == TRUE && bConnect == TRUE)
				{
					CTesterStatus cTesterStatus = g_AppleTestIF.GetTesterStatus(i + 1);

					if (cTesterStatus.m_bHeartBit == TRUE)
					{
						CTime cChkTime = CTime::GetCurrentTime();
						if (cChkTime - cTesterStatus.m_cChkTimeStamp > 8) {
							if (g_Error.IsSetAlarm(ERR_TESTER_1_HEART_BEAT + i) == FALSE) {
								g_AppleTestIF.HeartBeatOverCutClient(i + 1);
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat("Station %d Hear Beat Error!! Please check for Tester", i + 1);
								g_Error.AlarmReport(ERR_TESTER_1_HEART_BEAT + i, AlarmData, &g_TaskSystemCtrl);
							}
							OnLotCommand(eAutoLotCmd_AlarmPause, NULL, NULL);
							//}
							//else {
							//	m_nHeartBeatChk[i]++;
							//	g_AppleTestIF.SetTesterTimeStamp(i + 1, "");
							//	continue;
							//}
						}

						char szCompany[16] = { 0, };
						g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

						if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {

							int nYear = atoi(cTesterStatus.m_sTimeStamp.Left(4));
							int nMonth = atoi(cTesterStatus.m_sTimeStamp.Mid(4, 2));
							int nDay = atoi(cTesterStatus.m_sTimeStamp.Mid(6, 2));
							int nHour = atoi(cTesterStatus.m_sTimeStamp.Mid(8, 2));
							int nMinute = atoi(cTesterStatus.m_sTimeStamp.Mid(10, 2));
							int nSecond = atoi(cTesterStatus.m_sTimeStamp.Mid(12, 2));

							BOOL bHeatbeatDataError = FALSE;

							if ((nYear >= 1900)
								&& (nMonth >= 1 && nMonth <= 12)
								&& (nDay >= 1 && nDay <= 31)
								&& (nHour >= 0 && nHour <= 23)
								&& (nMinute >= 0 && nMinute <= 59)
								&& (nSecond >= 0 && nSecond <= 59)) {

								CTime cTesterTime = CTime(nYear, nMonth, nDay, nHour, nMinute, nSecond);

								CTime timeLimitNegative = cChkTime - CTimeSpan(0, 0, 30, 0);
								CTime timeLimitPositive = cChkTime + CTimeSpan(0, 0, 30, 0);

								if (timeLimitNegative > cTesterTime || cTesterTime > timeLimitPositive) {
									bHeatbeatDataError = TRUE;
								}
							}
							else {
								bHeatbeatDataError = TRUE;
							}

							if (bHeatbeatDataError) {
								g_AppleTestIF.HeartBeatOverCutClient(i + 1);
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat("Station %d Hear Beat Time Data Error!! Please check for Tester", i + 1);
								g_Error.AlarmReport(ERR_TESTER_1_HEART_BEAT_TIME_DATA_ERROR + i, AlarmData, &g_TaskSystemCtrl);
								OnLotCommand(eAutoLotCmd_AlarmPause, NULL, NULL);
							}
						}
					}
				}
				//m_nHeartBeatChk[i] = 0;
			}
		}

		m_csHeartBeat.Unlock();
	}
}
// tester information을 요청 한다.
void CTaskSystemCtrl::_Check_Online_ReqTesterInformation()
{
	// 소켓 생성 후 Tester Information 셋팅 안된 통신 소켓을 탐색 한다.
	g_AppleTestIF.m_TesterMgr.SendQueryInformation();
}
void CTaskSystemCtrl::_Check_Online_ReqTesterTestMode()
{
	// 소켓 생성 후 Tester TestMode 셋팅 안된 통신 소켓을 탐색 한다.
	g_AppleTestIF.m_TesterMgr.SendQuerySetTestMode();
}

void CTaskSystemCtrl::_Check_Online_ReqTesterAPLInfo()
{
	// 소켓 생성 후 Tester TestMode 셋팅 안된 통신 소켓을 탐색 한다.
	g_AppleTestIF.m_TesterMgr.SendQuerySetAPLInfo();
}
void CTaskSystemCtrl::_Check_HandlerTesterModePopup()
{
	CParamList AlarmData;
	CString strTesterMode = _T(""), strHandlerMode = _T("");
	g_TaskSystemCtrl.LotTesterModeText(strHandlerMode, strTesterMode);
	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	BOOL bCrtlMode = g_TaskSystemCtrl.GetEqpCtrlMode();
	int nLotStatus = GetCurrentLotState();

	int nMD_TesterIF = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_TESTER_IF_PRO_AUDIT);
	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-16]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 스위치 또는 각종 외부 신호에 대한 이벤트를 발생 시킨다.
==============================================================================*/
void CTaskSystemCtrl::Switch_Action()
{
	_Check_Sig_EMO();
	_Check_PressTestMainAir();
	_Check_StackerCtrayMainAir();
	_Check_Sig_CPTrip();
	_Check_Sig_MainPow();
	_Check_Sig_ServoPow();
 	_Check_Sig_Ion_Alarm();	
 	_Check_Sig_Ion_Fan_Alarm();
	_Check_Sig_DownDoor();	
	_Check_Regulator();
	_Check_Switch_Mode();
	_Check_HddRemainSize();
	_Check_Recent_OS_Version();
	_Check_Sig_VisionBoardReset();

//  #if DEF_TESTER_COMM_SPEC_ONE_SOCKET == 0	// yanmade tester 통신 protocol 사양에서는 미사용
//  	_Check_Online_Heart_Beat();			
//  #else
//  	_Check_Online_ReqTesterInformation();
//  #endif

	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	if (nTesterOption == eTester_EachSocket) {
		_Check_Online_ReqTesterInformation();
	}
	
	if (nProjectOption == DEF_PROJECT_WATT)
	{
		_Check_Online_ReqTesterTestMode();
		Sleep(10);
		_Check_Online_ReqTesterAPLInfo();
	}
/*	else {*/
		_Check_Online_Heart_Beat();
//	}

 	UpdateDailyReport();
	UpdateStopRunTm();
	UpdateCycleUPH();
	
	// start switch
	if( g_START_SW.GetDF_On() ){
		SendEvent(CEID_START_SW_ON);
	}

	// stop switch
	if( g_STOP_SW.GetDF_On() ){
		SendEvent(CEID_STOP_SW_ON);
	}

	// cover switch
	if( g_COVER_SW.GetDF_On() ){
		SendEvent(CEID_COVER_SW_ON);
	}
	if( g_COVER_SW.GetDF_Off() ){
		SendEvent(CEID_COVER_SW_OFF);
	}

	// sound switch
	if( g_SOUND_SW.GetDF_On() ){
		SendEvent(CEID_SOUND_SW_DOWN);
	}
	
	// lamp switch
	if( g_LAMP_SW.GetDF_On() ){
		SendEvent(CEID_LAMP_SW_ON);
	}
	if( g_LAMP_SW.GetDF_Off() ){
		SendEvent(CEID_LAMP_SW_OFF);
	}

	
// 	if (m_tcClientTimeChcek.IsChecking() == TRUE)
// 	{
// 		if (g_pConectSocket != NULL)
// 		{
// 			if (m_tcClientTimeChcek.CheckOverTime() == TRUE && g_pConectSocket->getIsConnect() == FALSE)
// 			{
// 				g_pConectSocket->Reconnect();
// 				m_tcClientTimeChcek.ResetStartTime();
// 			}		
// 			g_DMCont.m_dmEQP.SB(BDM0_BINSOLUTION_AGENT_CONNECTION, g_pConectSocket->getIsConnect());
// 		}			
// 		else
// 		{
// 			g_DMCont.m_dmEQP.SB(BDM0_BINSOLUTION_AGENT_CONNECTION, FALSE);
// 		}
// 	}
// 	else
// 	{
// 		m_tcClientTimeChcek.SetCheckTime(10.0);
// 		m_tcClientTimeChcek.StartTimer();
// 	}


	//jira 403
	// check 중이고, check 시간 넘고, 멈춤상태일때,
	// start 버튼 누르면 강제로 off 하도록 되어있기 때문에 체크 미 필요.
	if(m_tcNoDeviceCheck.IsChecking() == TRUE )
	{
		if (m_tcNoDeviceCheck.CheckOverTime() == TRUE && g_STOP_SW.GetStatus() == TRUE)
		{
			int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
			if (nDeviceMode != eDeviceMode)
			{
				BOOL bEjectorDlgOpen = g_DMCont.m_dmEQP.GB(BDM0_OPEN_EJECTOR_DLG_HISTORY);
				BOOL bHeightCalEjectorDlgOpen = g_DMCont.m_dmEQP.GB(BDM0_OPEN_HEIGHT_CAL_EJECTOR_DLG_HISTORY);
				
				if (bEjectorDlgOpen != TRUE && bHeightCalEjectorDlgOpen != TRUE)
				{
					g_TaskTrayLoadPP.do_NoDeviceResetAll();
					g_TaskTrayUnloadPP.do_NoDeviceResetAll();
					for (int nModuleIdx = 0; nModuleIdx < MAX_TEST_PP_CNT; nModuleIdx++)
						g_TaskTestPP[nModuleIdx].do_NoDeviceResetAll();
				}

				m_tcNoDeviceCheck.ResetStartTime();
			}
		}
	}
	else
	{
		m_tcNoDeviceCheck.SetCheckTime(600.0);
		m_tcNoDeviceCheck.StartTimer();
	}		
}

void CTaskSystemCtrl::On_Emo_Switch_On(CParamList* pDataList)
{
}
void CTaskSystemCtrl::On_Emo_Switch_Off(CParamList* pDataList)
{
}
void CTaskSystemCtrl::On_DoorLock_Switch_On(CParamList* pDataList)
{
}
void CTaskSystemCtrl::On_DoorLock_Switch_Off(CParamList* pDataList)
{
}
void CTaskSystemCtrl::On_MainPower_Switch_On(CParamList* pDataList)
{
}
void CTaskSystemCtrl::On_MainPower_Switch_Off(CParamList* pDataList)
{
}
void CTaskSystemCtrl::On_SiteSlide_On(CParamList* pDataList)
{
}
void CTaskSystemCtrl::On_SiteSlide_Off(CParamList* pDataList)
{
}
void CTaskSystemCtrl::On_Start_Switch_On(CParamList* pDataList)
{
	g_TaskSystemCtrl.DeActionTowerAndSound(eSIGNAL_PAUSE);
	g_TaskSystemCtrl.DeActionTowerAndSound(eSIGNAL_FINISH);
	g_TaskSystemCtrl.DeActionTowerAndSound(eSIGNAL_REQUEST);
	g_TaskSystemCtrl.DeActionTowerAndSound(eSIGNAL_JAM_CALL);
	g_TaskSystemCtrl.ActionTowerAndSound(eSIGNAL_RUNNING);
}
void CTaskSystemCtrl::On_Stop_Switch_On(CParamList* pDataList)
{
	//g_TaskSystemCtrl.DeActionTowerAndSound(eSIGNAL_RUNNING);
	//g_TaskSystemCtrl.DeActionTowerAndSound(eSIGNAL_REQUEST);
	//g_TaskSystemCtrl.DeActionTowerAndSound(eSIGNAL_JAM_CALL);
	g_TaskSystemCtrl.ActionTowerAndSound(eSIGNAL_PAUSE);
}
void CTaskSystemCtrl::On_Cover_Switch_On(CParamList* pDataList)
{
}
void CTaskSystemCtrl::On_Cover_Switch_Off(CParamList* pDataList)
{
}
void CTaskSystemCtrl::On_UndStkLoad_Switch_On(CParamList* pDataList)
{
}
void CTaskSystemCtrl::On_UndStkLoad_Switch_Off(CParamList* pDataList)
{
}
void CTaskSystemCtrl::On_UndStkUnload_Switch_On(CParamList* pDataList)
{
}
void CTaskSystemCtrl::On_UndStkUnload_Switch_Off(CParamList* pDataList)
{
}
void CTaskSystemCtrl::On_Sound_Switch_Down(CParamList* pDataList)
{
    g_TaskSystemCtrl.DeActionTowerAndSound(eSIGNAL_FINISH);
	do_Buzzer(0);
}
void CTaskSystemCtrl::On_Sound_Switch_Up(CParamList* pDataList)
{
}
void CTaskSystemCtrl::On_Lamp_Switch_On(CParamList* pDataList)
{
}
void CTaskSystemCtrl::On_Lamp_Switch_Off(CParamList* pDataList)
{
}
void CTaskSystemCtrl::On_ServoPower_Switch_On(CParamList* pDataList)
{
}
void CTaskSystemCtrl::On_ServoPower_Switch_Off(CParamList* pDataList)
{
}
void CTaskSystemCtrl::On_IonAlarm_On(CParamList* pDataList)
{
}
void CTaskSystemCtrl::On_IonAlarm_Off(CParamList* pDataList)
{
}
void CTaskSystemCtrl::On_IonFanAlarm_On(CParamList* pDataList)
{
}
void CTaskSystemCtrl::On_IonFanAlarm_Off(CParamList* pDataList)
{
}
