#include "StdAfx.h"
#include "OPSwitchAct_Cover.h"
#include <vector>
#include "ParamList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

COPSwitchAct_Cover::COPSwitchAct_Cover(void)
{
	//DI
// 	m_vLockSen.push_back(DI_FRONT_LEFT_DOORLOCK_SEN);
// 	m_vLockSen.push_back(DI_FRONT_RIGHT_DOORLOCK_SEN);
// 	m_vLockSen.push_back(DI_HINGED_RIGHT_DOORLOCK_SEN);
// 	m_vLockSen.push_back(DI_SLIDE_RIGHT_F1_DOORLOCK_SEN);
// 	m_vLockSen.push_back(DI_SLIDE_RIGHT_R2_DOORLOCK_SEN);
// 
// 	m_vLockSen.push_back(DI_REAR_RIGHT_DOORLOCK_SEN);
// 	m_vLockSen.push_back(DI_REAR_LEFT_DOORLOCK_SEN);
// 	m_vLockSen.push_back(DI_SLIDE_LEFT_R2_DOORLOCK_SEN);
// 	m_vLockSen.push_back(DI_SLIDE_LEFT_F1_DOORLOCK_SEN);
// 	m_vLockSen.push_back(DI_HINGED_LEFT_DOORLOCK_SEN);

	//DO
// 	m_vLockOut.push_back(DO_FRONT_LEFT_DOORLOCK);
// 	m_vLockOut.push_back(DO_FRONT_RIGHT_DOORLOCK);
// 	m_vLockOut.push_back(DO_HINGED_RIGHT_DOORLOCK);
// 	m_vLockOut.push_back(DO_SLIDE_RIGHT_F1_DOORLOCK);
// 	m_vLockOut.push_back(DO_SLIDE_RIGHT_R2_DOORLOCK);
// 
// 	m_vLockOut.push_back(DO_REAR_RIGHT_DOORLOCK);
// 	m_vLockOut.push_back(DO_REAR_LEFT_DOORLOCK);
// 	m_vLockOut.push_back(DO_SLIDE_LEFT_R2_DOORLOCK);
// 	m_vLockOut.push_back(DO_SLIDE_LEFT_F1_DOORLOCK);
// 	m_vLockOut.push_back(DO_HINGED_LEFT_DOORLOCK);
}


COPSwitchAct_Cover::~COPSwitchAct_Cover(void)
{
}


void COPSwitchAct_Cover::Initialize()
{
	int nEqpOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_EQP_No);
	//DI
	m_vLockSen.push_back(DI_FRONT_LEFT_DOORLOCK_SEN);
	m_vLockSen.push_back(DI_FRONT_RIGHT_DOORLOCK_SEN);
	m_vLockSen.push_back(DI_HINGED_RIGHT_DOORLOCK_SEN);
	m_vLockSen.push_back(DI_SLIDE_RIGHT_F1_DOORLOCK_SEN);
	m_vLockSen.push_back(DI_SLIDE_RIGHT_R2_DOORLOCK_SEN);

	m_vLockSen.push_back(DI_REAR_RIGHT_DOORLOCK_SEN);
	m_vLockSen.push_back(DI_REAR_LEFT_DOORLOCK_SEN);
	m_vLockSen.push_back(DI_SLIDE_LEFT_R2_DOORLOCK_SEN);
	m_vLockSen.push_back(DI_SLIDE_LEFT_F1_DOORLOCK_SEN);
	m_vLockSen.push_back(DI_HINGED_LEFT_DOORLOCK_SEN);
	if(nEqpOption > DEF_EQUIPMENT_UNIT1)
		m_vLockSen.push_back(DI_FRONT_LEFT2_DOORLOCK);


	//DO
	m_vLockOut.push_back(DO_FRONT_LEFT_DOORLOCK);
	m_vLockOut.push_back(DO_FRONT_RIGHT_DOORLOCK);
	m_vLockOut.push_back(DO_HINGED_RIGHT_DOORLOCK);
	m_vLockOut.push_back(DO_SLIDE_RIGHT_F1_DOORLOCK);
	m_vLockOut.push_back(DO_SLIDE_RIGHT_R2_DOORLOCK);

	m_vLockOut.push_back(DO_REAR_RIGHT_DOORLOCK);
	m_vLockOut.push_back(DO_REAR_LEFT_DOORLOCK);
	m_vLockOut.push_back(DO_SLIDE_LEFT_R2_DOORLOCK);
	m_vLockOut.push_back(DO_SLIDE_LEFT_F1_DOORLOCK);
	m_vLockOut.push_back(DO_HINGED_LEFT_DOORLOCK);
	if (nEqpOption > DEF_EQUIPMENT_UNIT1)
		m_vLockOut.push_back(DO_FRONT_LEFT2_DOORLOCK);
}


void COPSwitchAct_Cover::OPSwitchSet(CString strName, int nSw_IO_Addr, int nLed_IO_Addr, eOP_OUT_ATTRIB eOut_Attrib, BOOL bBlink/*=FALSE*/, int nLedType/*=0*/)
{
	COPSwitchAct::OPSwitchSet(strName, nSw_IO_Addr, nLed_IO_Addr, eOut_Attrib, bBlink, nLedType);
	COPSwitchAct::Add_In(DI_REAR_LEFT_COVER_SW_SEN);
	COPSwitchAct::Add_In(DI_REAR_RIGHT_COVER_SW_SEN);

	//COPSwitchAct::Add_In(DI_REAR_RIGHT_COVER_SW_SEN);
	COPSwitchAct::Add_Out(DO_REAR_LEFT_COVER_SW_LAMP);
	COPSwitchAct::Add_Out(DO_REAR_RIGHT_COVER_SW_LAMP);
	COPSwitchAct::OutPutLed(DEF_OFF);

	int nMotorCount = g_Motor.GetAxisCount();
	for(int i=0; i<nMotorCount; i++)
	{
		g_Motor[i]->ServoOff();
	}
}

void COPSwitchAct_Cover::Cover_Lock()
{
	m_nOperationStep = 1000;
}

void COPSwitchAct_Cover::Cover_Unlock()
{
	m_nOperationStep = 2000;
}

void COPSwitchAct_Cover::Switch_Operation()
{
	switch( m_nOperationStep )
	{
	case 0:
		{
			if( !m_bLockMode )	//Lock되어 있으면 아무것도 하지 않는다.
			{
				if(GetSWStatus())
				{
					if( m_bTemp_key == FALSE )
					{
						m_bTemp_key = TRUE;
						if( !GetStatus())
						{
							m_bTemp_Memo_Flag = TRUE;
							m_nOperationStep = 1000;
						}
						else
						{
							// Start된 상태에서는 cover 버튼 동작되어서는 안됨.
							if( g_START_SW.GetStatus() != FALSE )
							{
								break;
							}

							m_bTemp_Memo_Flag = FALSE;
							m_nOperationStep = 2000;
						}
					}
				}
				else
				{
					m_bTemp_key = FALSE;
					//OutPutLed(DEF_OFF);
				}
			}
		}break;
		
	case 1000: // cover lock
		{
			BOOL bOtionDoor = g_DMCont.m_dmEQP.GB(BDM0_MD_FRONT_SLIDE_DOOR);
		
			int nCnt = 0;
			for( std::vector<int>::size_type i=0; i<m_vLockOut.size(); i++)
			{
				if( bOtionDoor == FALSE && (m_vLockOut[i] == DO_FRONT_LEFT_DOORLOCK || m_vLockOut[i] == DO_FRONT_RIGHT_DOORLOCK) )
					continue;

				g_IO.out(m_vLockOut[i], DEF_ON);
				nCnt++;
				if( nCnt % 4 == 0 && nCnt != 0){
					Switch_CoverSleep(30); // 50 -> 30
				}				
			}
			m_nOperationStep = 1001;
			Switch_CoverSleep(200);
		}break;

	case 1001:
		{
			BOOL bOtionDoor = g_DMCont.m_dmEQP.GB(BDM0_MD_FRONT_SLIDE_DOOR);

			int nChk = 0;
			if( !DEF_IO_CHECK_MODE )
			{
				for( std::vector<int>::size_type i = 0; i < m_vLockSen.size(); i++ ) {
					if (bOtionDoor == FALSE && (m_vLockSen[i] == DI_FRONT_LEFT_DOORLOCK_SEN || m_vLockSen[i] == DI_FRONT_RIGHT_DOORLOCK_SEN))
						continue;

					if( g_IO.in( m_vLockSen[ i ] ) == DEF_OFF ) { nChk++; }
				}
			}
			
			if( nChk == 0 ){
				m_nOperationStep = 1002;
			}
			else
			{
				std::vector<CPoint> vFailDoor; vFailDoor.clear();
				CParamList AlarmData;
				for(std::vector<int>::size_type i=0; i<m_vLockSen.size(); i++)
				{
					if (bOtionDoor == FALSE && (m_vLockSen[i] == DI_FRONT_LEFT_DOORLOCK_SEN || m_vLockSen[i] == DI_FRONT_RIGHT_DOORLOCK_SEN))
						continue;

					if( g_IO.in(m_vLockSen[i]) == DEF_OFF )
					{
						CMyList rDiInfo;
						g_DB.SELECT_DIO_TBL(_tDIO_LS::eDI, m_vLockSen[i], 1, rDiInfo);
						if( rDiInfo.m_fnGetCount() > 0 )
						{
							_tDIO_LS* pItem = (_tDIO_LS*)rDiInfo.m_fnGetData(0);
							if( pItem )
							{
								pItem->name.TrimLeft();
								pItem->name.TrimRight();
								AlarmData.m_fnAddItemFormat("%s = %s", pItem->name, (g_IO.in(m_vLockSen[i]))?_T("ON"):_T("OFF") );
							}
						}
						vFailDoor.push_back(CPoint(i, 0));
					}
				}

				for( std::vector<int>::size_type i=0; i<m_vLockOut.size(); i++){
					//if(bIsDemo == FALSE && (m_vLockOut[i] == DO_LEFT_REAR_DOORLOCK || m_vLockOut[i] == DO_RIGHT_REAR_DOORLOCK)){
					//	continue;
					//}

					g_IO.out(m_vLockOut[i], DEF_OFF);
				}
				Switch_CoverSleep(100);
				g_Error.AlarmReport(ERR_CAN_NOT_COVER_LOCK, AlarmData, NULL, vFailDoor);
				//g_GuiComm.SendDoorStatusView(DEF_UNLOCK);
				m_nOperationStep = 0;
			}
		}break;

	case 1002:
		{
			int nMotorCount = g_Motor.GetAxisCount();
			for (int i = 0; i < nMotorCount; i++) { // Picker Motor , Press Z축 상시 전원 타입이라서 Servo Off하지 않음
				if (i >= eAXIS_TRAY_PP_1_Z || (eAXIS_PRESS_UNIT_DN5 <= i && i <= eAXIS_PRESS_UNIT_DN8) || (eAXIS_PRESS_UNIT_DN1 <= i && i <= eAXIS_PRESS_UNIT_DN4))
				{
				}
				else {
					g_Motor[i]->ServoOff();
					NEXT;
				}
			}

			Switch_CoverSleep(200); // 신규 추가
			g_IO.out(DO_COVER_RESET, DEF_ON);
			Switch_CoverSleep(200);
			g_IO.out(DO_COVER_RESET, DEF_OFF);
			Switch_CoverSleep(200);
			g_IO.out(DO_SERVO_POWER_ON, DEF_ON);
			Switch_CoverSleep(200);

			if( g_IO.in(DI_SERVO_POWER_ON_SEN1) != DEF_ON ){
				m_nOperationStep = 2000;
				CParamList AlarmData;
				CMyList rDiInfo;
				g_DB.SELECT_DIO_TBL(_tDIO_LS::eDI, DI_SERVO_POWER_ON_SEN1, 1, rDiInfo);
				if (rDiInfo.m_fnGetCount() > 0)
				{
					_tDIO_LS* pItem = (_tDIO_LS*)rDiInfo.m_fnGetData(0);
					if (pItem)
					{
						pItem->name.TrimLeft();
						pItem->name.TrimRight();
						AlarmData.m_fnAddItemFormat("%s = %s", pItem->name, (g_IO.in(DI_SERVO_POWER_ON_SEN1)) ? _T("ON") : _T("OFF"));
					}
				}
				g_Error.AlarmReport(ERR_CAN_NOT_COVER_LOCK, AlarmData, NULL);
				break;
			}
			//OutPutLed(DEF_ON);

			/*
			1. 10축당 지연 10ms -> 100ms -> 200ms -> 300ms -> 400ms -> 500ms
			2. 계속 떨어져서 10축에서 5축으로 변경
			3. 100ms -> 200ms -> 300ms -> 400ms -> 500ms
			4. reset 전 100ms 추가하여 다시 10축당으로 변경
			5. 다시 5축당으로 변경
			6. 20축 당으로 변경
			*/
//			int nMotorCount = g_Motor.GetAxisCount();
			for(int i=0; i<nMotorCount; i++)
			{
				if( i%20 == 0 && i != 0){
					Switch_CoverSleep(500);	// 10 -> 100-> 200 -> 300 -> 400
				}		
				g_Motor[i]->ServoOn();	
			}

			Switch_CoverSleep(20);

			SetStatus(DEF_ON);

			//g_GuiComm.SendDoorStatusView(DEF_LOCK);

			g_TaskSystemCtrl.MakeLog("Push Cover Switch [LOCK]");
			m_nOperationStep = 0;
		}break;

	case 2000: // cover unlock
		{
			int nChk = 13;
			for( int i = 0; i<eMaxStackerCount; i++ )
			{
				if( g_TaskAutoStacker[ i ].GetCurrentLotState() != eAutoLotState_AlarmStop
					|| g_TaskAutoStacker[ i ].GetCurrentLotState() != eAutoLotState_Pause
					|| g_TaskAutoStacker[ i ].GetCurrentLotState() != eAutoLotState_InitNeed ) {
					nChk--;
				}
			}

			if (g_TaskAutoTrayLoadPP.GetCurrentLotState() == eAutoLotState_AlarmStop
				|| g_TaskAutoTrayLoadPP.GetCurrentLotState() == eAutoLotState_Pause
				|| g_TaskAutoTrayLoadPP.GetCurrentLotState() == eAutoLotState_InitNeed) {
				nChk--;
			}
	
			if (g_TaskAutoTrayUnloadPP.GetCurrentLotState() == eAutoLotState_AlarmStop
				|| g_TaskAutoTrayUnloadPP.GetCurrentLotState() == eAutoLotState_Pause
				|| g_TaskAutoTrayUnloadPP.GetCurrentLotState() == eAutoLotState_InitNeed) {
				nChk--;
			}

			if (g_TaskAutoTransfer.GetCurrentLotState() == eAutoLotState_AlarmStop
				|| g_TaskAutoTransfer.GetCurrentLotState() == eAutoLotState_Pause
				|| g_TaskAutoTransfer.GetCurrentLotState() == eAutoLotState_InitNeed) {
				nChk--;
			}
			if (g_TaskAutoTrayFeeder.GetCurrentLotState() == eAutoLotState_AlarmStop
				|| g_TaskAutoTrayFeeder.GetCurrentLotState() == eAutoLotState_Pause
				|| g_TaskAutoTrayFeeder.GetCurrentLotState() == eAutoLotState_InitNeed) {
				nChk--;
			}
			for(int i=0; i<eMaxTestPPCount; i++)
			{
				if(g_TaskAutoTestPp[i].GetCurrentLotState() != eAutoLotState_AlarmStop
					|| g_TaskAutoTestPp[i].GetCurrentLotState() != eAutoLotState_Pause
					|| g_TaskAutoTestPp[i].GetCurrentLotState() != eAutoLotState_InitNeed ){
						nChk--;
				}
			}
			if(nChk != 0){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("can't open door!! auto seq not finish!!");
				g_Error.AlarmReport(ERR_CAN_NOT_COVER_UNLOCK, AlarmData, NULL);

				m_nOperationStep = 0;
				break;
			}

// 			// Press Unit이 Testing 상태 일때는 Unlock 하지 못하도록 InterLock 처리하여야 한다. // imsi
// 			int nChkPressStatus = 0;
// 			for(int i=0; i<eMaxPressUnitCount; i++){
// 				BOOL bRetTest = g_DMCont.m_dmEQP.GB(BDM0_TEST_STATUS_SITE1+i);    // Get teting status off
// 				if(bRetTest == TRUE){
// 					nChkPressStatus++;
// 				}
// 			}
// 			// Lock 인데 off 하려는 경우
// 			if(nChkPressStatus != 0 ){
// 				CParamList AlarmData;
// 				AlarmData.m_fnAddItemFormat("press unit is not finish!!! wait for working finish!!!!");
// 				g_Error.AlarmReport(ERR_CAN_NOT_COVER_UNLOCK, AlarmData, NULL);
// 				m_nOperationStep = 0;
// 				break;
// 			}

			//			g_TaskTrayPP[m_eTrayIdx].do_Picker_All(DEF_UP);
			//			for(int i=0; i<MAX_TEST_PP_CNT; i++){
			//				g_TaskTestPP[i].do_Picker(DEF_UP, CTaskTestPP::ePicker_All);
			//			}


// 			// imsi 상시 전원 타입 일 경우 제외 필요 
			int nMotorCount = g_Motor.GetAxisCount();
			for(int i=0; i<nMotorCount; i++){// Picker Motor , Press Z축 상시 전원 타입이라서 Servo Off하지 않음
				if (i >= eAXIS_TRAY_PP_1_Z || (eAXIS_PRESS_UNIT_DN5 <= i && i <= eAXIS_PRESS_UNIT_DN8) || (eAXIS_PRESS_UNIT_DN1 <= i && i <= eAXIS_PRESS_UNIT_DN4)) {
					BOOL IsOrigin = g_Motor[i]->IsOrgCompleted(nullptr);
					if (eAXIS_TRAY_PP_1_Z <= i && i <= eAXIS_TEST_PP_2_Z2 &&
						IsOrigin == TRUE
						/*CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto*/)
					{
						
						double dSafetyPos = 0.0;

						CPoint pocket_0_0(0, 0);
						std::vector<CPoint> vPicker; vPicker.clear();

						for (int nY = 0; nY < MAX_PICKER_Y_CNT; nY++) {
							for (int nX = 0; nX < MAX_PICKER_X_CNT; nX++) {
								vPicker.push_back(CPoint(nX, nY));
							}
						}

// 						switch (i)
// 						{
						if(eAXIS_TRAY_PP_1_Z == i)
						{
							//g_TaskTrayUnloadPP.GetTeachPos(LOC_IDX_TRAY_PP_CONV_4, pocket_0_0, vPicker_0_0, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisX);
							dSafetyPos = g_TaskTrayLoadPP.GetTeachPos(LOC_IDX_TRAY_PP_SAFETY, pocket_0_0, vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
						}
						else if(eAXIS_TEST_PP_1_Z == i || eAXIS_TEST_PP_1_Z2 == i)
						{
							int nHandType = (i == eAXIS_TEST_PP_1_Z) ? eTestPPLoadType : eTestPPUnLoadType;
							int nZAxis = (i == eAXIS_TEST_PP_1_Z) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
							//GetTeachPos(LOC_IDX_TEST_PP_HOME, CPoint(0, 0), vTempPkr, CTaskTestPP::eZPOS_SAFETY, dTargetPos, m_nHandType);

							dSafetyPos = g_TaskTestPP[eTestPP_1].GetTeachPos(LOC_IDX_TEST_PP_HOME, pocket_0_0, vPicker, CTaskTestPP::eZPOS_SAFETY, nZAxis, nHandType);
						}
						else if (eAXIS_TRAY_PP_2_Z == i)
						{
							dSafetyPos = g_TaskTrayUnloadPP.GetTeachPos(LOC_IDX_TRAY_PP_SAFETY, pocket_0_0, vPicker, CTaskTrayBasePP::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
						}
						else if (eAXIS_TEST_PP_2_Z == i || eAXIS_TEST_PP_2_Z2 == i)
						{
							int nHandType = (i == eAXIS_TEST_PP_2_Z) ? eTestPPLoadType : eTestPPUnLoadType;
							int nZAxis = (i == eAXIS_TEST_PP_2_Z) ? CTaskTestPP::eAxisZ1 : CTaskTestPP::eAxisZ2;
							dSafetyPos = g_TaskTestPP[eTestPP_2].GetTeachPos(LOC_IDX_TEST_PP_HOME, pocket_0_0, vPicker, CTaskTestPP::eZPOS_SAFETY, nZAxis, nHandType);
						}

						//Safety Position Move
 						g_Motor[i]->Move(vPicker, dSafetyPos, 1000, 50, 50, TRUE);
					}
				}
				else {
// 					BOOL IsOrigin = g_Motor[i]->IsOrgCompleted(nullptr);
// 					if ((eAXIS_LOAD_TABLE_1_ROTATE <= i && i <= eAXIS_LOAD_TABLE_4_ROTATE) && IsOrigin == TRUE)
// 					{
// 						continue; //ServoOff 안함.
//  				}

					g_Motor[i]->ServoOff();
				}
			}

			//Cover Unlock시, Safety Position Move
		
			//g_bbxmnpClient.Move(eMoveType_INC, _eBbxmnpAxis(i), dTargetPos, 1000, 50, 50, TRUE);


			Sleep(400);
			g_IO.out(DO_SERVO_POWER_ON, DEF_OFF);
			// imsi 상시 전원 타입 일 경우 제외 필요 
			Switch_CoverSleep(100);

			BOOL bOtionDoor = g_DMCont.m_dmEQP.GB(BDM0_MD_FRONT_SLIDE_DOOR);

			for( std::vector<int>::size_type i=0; i<m_vLockOut.size(); i++){
				if (bOtionDoor == FALSE && (m_vLockOut[i] == DO_FRONT_LEFT_DOORLOCK || m_vLockOut[i] == DO_FRONT_RIGHT_DOORLOCK))
					continue;

				g_IO.out(m_vLockOut[i], DEF_OFF);
			}
			Switch_CoverSleep(100);
			SetStatus(DEF_OFF);
			//g_GuiComm.SendDoorStatusView(DEF_UNLOCK);

			g_TaskSystemCtrl.MakeLog("Push Cover Switch [UNLOCK]");
			m_nOperationStep = 0;
		}break;
	}
}

void COPSwitchAct_Cover::Switch_CoverSleep(int nSleep)
{
	if (g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
		Sleep(nSleep);
}
