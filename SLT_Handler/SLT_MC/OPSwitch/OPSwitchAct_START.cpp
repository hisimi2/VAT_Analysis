#include "StdAfx.h"
#include "OPSwitchAct_START.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

COPSwitchAct_START::COPSwitchAct_START(void)
{
	m_bSwitchStatus_Bak = FALSE;
	m_nOrgRetryCount = 0;
	m_nBackStep = 0;
}


COPSwitchAct_START::~COPSwitchAct_START(void)
{
}

void COPSwitchAct_START::OPSwitchSet(CString strName, int nSw_IO_Addr, int nLed_IO_Addr, eOP_OUT_ATTRIB eOut_Attrib)
{
	COPSwitchAct::OPSwitchSet(strName, nSw_IO_Addr, nLed_IO_Addr, eOut_Attrib);
	COPSwitchAct::Add_In(DI_REAR_LEFT_START_SW_SEN2);
	COPSwitchAct::Add_In(DI_REAR_RIGHT_START_SW_SEN2);
	COPSwitchAct::Add_Out(DO_REAR_LEFT_START_SW_LAMP);
	COPSwitchAct::Add_Out(DO_REAR_RIGHT_START_SW_LAMP);
	COPSwitchAct::OutPutLed(DEF_OFF);
	int nEqpOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_EQP_No);
	m_vLockSen.push_back(DI_HINGED_LEFT_DOORLOCK_SEN);
	m_vLockSen.push_back(DI_FRONT_LEFT_DOORLOCK_SEN);
	m_vLockSen.push_back(DI_FRONT_RIGHT_DOORLOCK_SEN);
	m_vLockSen.push_back(DI_HINGED_RIGHT_DOORLOCK_SEN);
	m_vLockSen.push_back(DI_REAR_RIGHT_DOORLOCK_SEN);
	m_vLockSen.push_back(DI_REAR_LEFT_DOORLOCK_SEN);
	m_vLockSen.push_back(DI_SLIDE_LEFT_R2_DOORLOCK_SEN);
	m_vLockSen.push_back(DI_SLIDE_RIGHT_R2_DOORLOCK_SEN);
	m_vLockSen.push_back(DI_SLIDE_LEFT_F1_DOORLOCK_SEN);
	m_vLockSen.push_back(DI_SLIDE_RIGHT_F1_DOORLOCK_SEN);
	if (nEqpOption > DEF_EQUIPMENT_UNIT1)
		m_vLockSen.push_back(DI_FRONT_LEFT2_DOORLOCK);
}

void COPSwitchAct_START::Switch_Operation()
{
	if (m_nBackStep != m_nOperationStep) {
		g_TaskSystemCtrl.MakeLog("[Start Switch step = %d]",m_nOperationStep);
		m_nBackStep = m_nOperationStep;
	}
	switch( m_nOperationStep )
	{
	case 0:
        {
			if( !m_bLockMode )	//Lock되어 있으면 아무것도 하지 않는다.
			{
				if( (m_bSwitchStatus_Bak == DEF_OFF) && (GetSWStatus()==DEF_ON) )
				{
					// stop switch 동시 눌렸을 경우 stop을 수행한다.
					// stop switch가 눌려져 있지 않은 상태에서만 START 실행.
					if( m_pcGroup->GetSWStatus() == DEF_OFF) 
					{
						if(m_nOperationStep == 0)
							m_nOperationStep = 1000;
					}
				}
				m_bSwitchStatus_Bak = GetSWStatus();
			}
		}break;
	case 1000:// on
		{
			if (DEF_IO_CHECK_MODE == DEF_ON)
			{
				m_nOperationStep = 0;
				break;
			}

			for (int i = 0; i < eMaxTestPPCount; i++)
				g_StepTestPP_SW[i].SetChangeStatusOnOff(DEF_OFF);
			for (int i = 0; i < eMaxTrayPPCount; i++)
				g_StepTrayPP_SW[i].SetChangeStatusOnOff(DEF_OFF);

			g_Feeder_SW.SetChangeStatusOnOff(DEF_OFF);

			//Disconnection vector Clear
			g_TaskSystemCtrl.ClearDisConnectionStationIdx();

			BOOL bImpossibleAlarm = g_Error.IsImpossibleStartAlarm();

			if (!g_COVER_SW.GetStatus())
			{
				g_COVER_SW.Cover_Lock();
				Sleep(2000);

				// Jira 358
				BOOL bOtionDoor = g_DMCont.m_dmEQP.GB(BDM0_MD_FRONT_SLIDE_DOOR);
				int nChk = 0;
				if (!DEF_IO_CHECK_MODE)
				{
					for (std::vector<int>::size_type i = 0; i < m_vLockSen.size(); i++) {
						if (bOtionDoor == FALSE && (m_vLockSen[i] == DI_SLIDE_LEFT_F1_DOORLOCK_SEN || m_vLockSen[i] == DI_SLIDE_RIGHT_F1_DOORLOCK_SEN))
							continue;
						if (g_IO.in(m_vLockSen[i]) == DEF_OFF) { nChk++; }
					}
				}

				if (nChk != 0)
				{
					CParamList AlarmData;
					std::vector<CPoint> vFailDoor; vFailDoor.clear();
					for (std::vector<int>::size_type i = 0; i < m_vLockSen.size(); i++)
					{
						if (bOtionDoor == FALSE && (m_vLockSen[i] == DI_SLIDE_LEFT_F1_DOORLOCK_SEN || m_vLockSen[i] == DI_SLIDE_RIGHT_F1_DOORLOCK_SEN))
							continue;

						if (g_IO.in(m_vLockSen[i]) == DEF_OFF)
						{
							CMyList rDiInfo;
							g_DB.SELECT_DIO_TBL(_tDIO_LS::eDI, m_vLockSen[i], 1, rDiInfo);
							if (rDiInfo.m_fnGetCount() > 0)
							{
								_tDIO_LS* pItem = (_tDIO_LS*)rDiInfo.m_fnGetData(0);
								if (pItem)
								{
									pItem->name.TrimLeft();
									pItem->name.TrimRight();
									AlarmData.m_fnAddItemFormat("%s = %s", pItem->name, (g_IO.in(m_vLockSen[i])) ? _T("ON") : _T("OFF"));
								}
							}
							vFailDoor.push_back(CPoint(i, 0));
						}
					}

					g_COVER_SW.Cover_Unlock();
					Sleep(100);
					g_Error.AlarmReport(ERR_CAN_NOT_COVER_LOCK, AlarmData, NULL,vFailDoor);
					m_nOperationStep = 0;
					break;
				}
			}

			//int nEQPNo = g_DMCont.m_dmEQP.GN(NDM11_Handler_EQP_No);
			//if (nEQPNo > DEF_EQUIPMENT_UNIT1)
			

			BOOL bErrMotor = TRUE;
			int nMotorCount = g_Motor.GetAxisCount();
			int i;
			for(i=0; i<nMotorCount && bErrMotor == TRUE; i++)
			{
 				bErrMotor = g_Motor[i]->GetServoStat();
			}
			
			if(bErrMotor == FALSE){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Motor is not Servo ON!! [ %s ]", g_Motor[i-1]->GetAxisName());
				g_Error.AlarmReport(ERR_SYS_SERVO_POW_IS_OFF, AlarmData, NULL);
				m_nOperationStep = 0;
				break;
			}

			if( g_MulitOrg.GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready )
			{
				m_nOperationStep = 0;
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Multi Origin is not finish!!");
				g_Error.AlarmReport(ERR_LOT_CMD_CANNOT_PERFORM_NOW, AlarmData, NULL);
				break;
			}

			//int nChkErr = g_TaskSystemCtrl.CheckEqpConditionForStart();
			//if( nChkErr != ERR_NO_ERROR ){
			//	m_nOperationStep = 0;
			//	CParamList AlarmData;
			//	AlarmData.m_fnAddItemFormat("Can not start!!");
			//	g_Error.AlarmReport(nChkErr, AlarmData, NULL);
			//	break;
			//}

			int nLimit = g_DMCont.m_dmEQP.GN(NDM0_HDD_SIZE_INTERLOCK);

			ULARGE_INTEGER avail, total, free;
			GetDiskFreeSpaceEx(_T("D:/"), &avail, &total, &free );
			double disk_size= (int)(total.QuadPart>>20); //MBytes로 처리 
			//disk_free_size;
			double disk_free_size = (int)(free.QuadPart>>20);
			double dRemainSize = (disk_free_size/disk_size) * 100.0;
			if( nLimit >= dRemainSize ){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Hard Disk remain space lower [Set=%d per, Current=%.2f per]", nLimit, dRemainSize);
				g_Error.AlarmReport(ERR_HARD_DISK_LOW_INTERLOCK, AlarmData, NULL);
				m_nOperationStep = 0;
				break;
			}

			if( bImpossibleAlarm != TRUE && bErrMotor == TRUE && g_COVER_SW.GetStatus())
			{
				int nCurrentLotState = eAutoLotState_Unknown;
				std::vector<int> vLotState;
				for( int i = 0; i<eMaxStackerCount; i++ ) {
					nCurrentLotState = g_TaskAutoStacker[ eSTACKER_01 + i ].GetCurrentLotState();
					vLotState.push_back( nCurrentLotState );
				}
				vLotState.push_back(nCurrentLotState);

				nCurrentLotState = g_TaskAutoTrayLoadPP.GetCurrentLotState();
				vLotState.push_back(nCurrentLotState);

				nCurrentLotState = g_TaskAutoTrayUnloadPP.GetCurrentLotState();
				vLotState.push_back(nCurrentLotState);

				nCurrentLotState = g_TaskAutoTrayFeeder.GetCurrentLotState();
				vLotState.push_back(nCurrentLotState);

				nCurrentLotState = g_TaskAutoTransfer.GetCurrentLotState();
				vLotState.push_back(nCurrentLotState);

				for(int i=0; i<eMaxTestPPCount; i++){
					nCurrentLotState = 	g_TaskAutoTestPp[eTestPP_1+i].GetCurrentLotState();
					vLotState.push_back(nCurrentLotState);
				}

				for(std::vector<int>::size_type i=0; i<vLotState.size(); i++){
					if( vLotState[i] == eAutoLotState_InitNeed ){
						nCurrentLotState = eAutoLotState_InitNeed;
					}		
				}

				if(nCurrentLotState == eAutoLotState_InitNeed){
					CTaskTrayBasePP* pTaskTrayPP[] = {&g_TaskTrayLoadPP, &g_TaskTrayUnloadPP};
					for (int i = 0; i < eMaxTrayPPCount; i++)
						pTaskTrayPP[i]->m_bInitialized = FALSE;

					g_TaskTrayFeeder.m_bInitialized = FALSE;

					g_TaskTransfer.m_bInitialized = FALSE;
					for (int i = 0; i < eMaxLoadTblCount; i++)
						g_TaskLoadTbl[i].m_bInitialized = FALSE;
					for (int i = 0; i < eMaxTestPPCount; i++)
						g_TaskTestPP[i].m_bInitialized = FALSE;
					for (int i = 0; i < eMaxPressUnitCount; i++)
						g_TaskPressUnit[i].m_bInitialized = FALSE;


					m_nOperationStep = 1100;
					break;
				}else{
					m_nOperationStep = 2000;
					break;
				}
			}

			if (bImpossibleAlarm == TRUE)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Don't Start Alarm Clear Please!!");
				g_Error.AlarmReport(ERR_IMPOSSIBLE_ALARM_CLEAR, AlarmData, NULL);
				m_nOperationStep = 0;
				break;
			}

			m_nOperationStep = 0;
			
		}break;
	case 1100:
		{
			int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
			g_TaskAllAutoStacker.SetOffLineUseIgnore(DEF_REQ_UNK);
			g_TaskAllAutoStacker.SetOnlineTesterMode(DEF_REQ_UNK);

			char szCompany[16] = { 0, };
			g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

			if(nRunMode == eRUN_MODE_OFFLINE){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Running Mode is OffLine");
				g_Error.AlarmReport(ERR_OFFLINE_USE, AlarmData, &g_TaskSystemCtrl);
				m_nOperationStep++;
				break;
			}else{				
				int nMD_TesterIF = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_TESTER_IF_PRO_AUDIT);
				int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
				int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);

				CString strMode = _T(""), strHandlerMode = _T("");

				switch (nLotStartMode)
				{
					case eStartMD_Initial: strHandlerMode = _T("FT"); break;
					case eStartMD_AutoGRR: strHandlerMode = _T("GRR"); break;
					case eStartMD_AutoLoop: strHandlerMode = _T("LOOP"); break;
				}
				switch (nMD_TesterIF)
				{
					case eTesterIF_Mode_Normal: strMode = _T("PRODUCTION"); break;
					case eTesterIF_Mode_Grr: strMode = _T("GRR"); break;
					case eTesterIF_Mode_LoopTest: strMode = _T("LOOP TEST"); break;
					case eTesterIF_Mode_Audit: strMode = _T("AUDIT"); break;
				}

				if (nMD_TesterIF != eTesterIF_Mode_Normal && strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Check Please!! [ Run Mode=Online ] [ Tester Mode=%s ]", strMode);
					g_Error.AlarmReport(ERR_ONLINE_USE, AlarmData, &g_TaskSystemCtrl);
					m_nOperationStep = 1150;
					break;
				}
// 				else if ((nProjectOption == DEF_PROJECT_MARLIN || nProjectOption == DEF_PROJECT_MUSSEL)&& strcmp(szCompany, DEF_COMPANY_USI) == 0)
// 				{
// 					CParamList AlarmData;
// 					AlarmData.m_fnAddItemFormat("Check Please!! [ Run Mode=Online ] [Handler Mode=%s] [ Tester Mode=%s ]", strHandlerMode,strMode);
// 					g_Error.AlarmReport(ERR_ONLINE_USE, AlarmData, &g_TaskSystemCtrl);
// 					m_nOperationStep = 1150;
// 				}
				else {
					m_nOperationStep = 1160;
					break;
				}
				
			}
		}break;
	case 1101:	// offline
		{
			int nReqAck = g_TaskAllAutoStacker.GetOffLineUseIgnore();
			if (nReqAck != DEF_REQ_UNK)
			{
				if (nReqAck == DEF_REQ_OK) {
					m_nOperationStep = 1200;
				}else{
					m_nOperationStep = 0;
				}
			}
			//int nReqAck = DEF_REQ_UNK;
			//while(1){
			//	nReqAck = g_TaskAllAutoStacker.GetOffLineUseIgnore();
			//	if( nReqAck != DEF_REQ_UNK ){
			//		if( nReqAck == DEF_REQ_OK ){
			//			m_nOperationStep = 1200;
			//			break;
			//		}else{
			//			m_nOperationStep = 0;
			//			break;
			//		}
			//	}
			//	NEXT;
			//}
			//g_TaskAllAutoStacker.SetOffLineUseIgnore(DEF_REQ_UNK);
		}break;
	case 1150: // online
		{
			int nReqAck = g_TaskAllAutoStacker.GetOnlineTesterMode();
			if (nReqAck != DEF_REQ_UNK) {
				if (nReqAck == DEF_REQ_OK) {
					m_nOperationStep = 1160;/*1200;*/
				}
				else {
					m_nOperationStep = 0;
				}
			}
		
			//int nReqAck = DEF_REQ_UNK;
			//while (1) {
			//	nReqAck = g_TaskAllAutoStacker.GetOnlineTesterMode();
			//	if (nReqAck != DEF_REQ_UNK) {
			//		if (nReqAck == DEF_REQ_OK) {
			//			m_nOperationStep = 1200;
			//			break;
			//		}
			//		else {
			//			m_nOperationStep = 0;
			//			break;
			//		}
			//	}
			//	NEXT;
			//}
			//g_TaskAllAutoStacker.SetOnlineTesterMode(DEF_REQ_UNK);
		}break;
	case 1160:
		{
			//g_TaskAllAutoStacker.SetTesterInfoIgnore(DEF_REQ_UNK);

			//int nSiteOnOff = (bAutoGrrUse == TRUE ? pTaskPressUnit->GetSocketOnOffGrr(nX, nY) : pTaskPressUnit->GetSocketOnOff(nX, nY)); // socket on/off 여부 확인.

			int TestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
			int TestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

			int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
			if (nTesterOption != eTester_EachSocket) // Single Tester [8/27/2021 donghyun.shin]
			{
				m_nOperationStep = 1200;
				break;
			}

			BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
			int nFirstSocket = (bAutoGrrUse == TRUE) ? NDM11_GRR_SocketOnOff_Site1_1 : NDM11_SocketOnOff_Site1_1;
 			int nStationID = -1;

			for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++) {
				for (int nY = 0; nY < TestSite_Div_Y; nY++)
				{
					for (int nX = 0; nX < TestSite_Div_X; nX++)
					{
						int nSiteIdx = (nStation * STATION_MAX_PARA) + (nY * TestSite_Div_X) + nX;
						int nTesterIdx = (nStation *(TestSite_Div_X*TestSite_Div_Y)) + (nY * TestSite_Div_X) + nX;
						int nSiteOnOff = g_DMCont.m_dmHandlerOpt.GN(nFirstSocket + nSiteIdx);

						if (nSiteOnOff != DEF_SITE_ON)
						{
							continue;
						}

						nStationID = nTesterIdx + 1;
						if (g_AppleTestIF.IsConnected(nStationID) == TRUE)
						{
							CTesterStatus cTesterStatus = g_AppleTestIF.GetTesterStatus(nStationID);
							if (/*strcmp(cTesterStatus.m_sTesterID, _T("")) == 0*/ cTesterStatus.m_nStationID == 0) //ID 비어 있다면 Alarm
							{
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat("Site %d", nTesterIdx + 1);
								g_Error.AlarmReport(ERR_LOT_START_TESTER_INFO_EMPTY, AlarmData, NULL);
								m_nOperationStep = 0;
								break;
							}
							else //Connect 되어 있고, Tester 정보 있는 상태
							{
								continue;
							}
						}
						else
						{
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("Site %d", nTesterIdx + 1);
							g_Error.AlarmReport(ERR_LOT_START_TESTER_INFO_EMPTY, AlarmData, NULL);
							m_nOperationStep = 0;
							break;
						}
					}
				}
			}
 
 			if (m_nOperationStep == 0)
 				break;

			m_nOperationStep = 1200;
			
		}break;
	case 1200:
		{

			BOOL b2dBarcodeMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use);
			int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);

			g_TaskAllAutoStacker.Set2dUseIgnore(DEF_REQ_UNK);
			if(b2dBarcodeMode != eOPTION_USE && nRunMode == eRUN_MODE_ONLINE){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("2D Barcode is Off");
				g_Error.AlarmReport(ERR_2DBARCODE_NO_USE, AlarmData,  &g_TaskSystemCtrl);
				m_nOperationStep = 0;
				/*m_nOperationStep++;*/
				break;
			}else{
				m_nOperationStep = 1250;
				break;
			}

		}break;
	case 1201:
		{
			int nReqAck = g_TaskAllAutoStacker.Get2dUseIgnore();
			if (nReqAck != DEF_REQ_UNK) {
				if (nReqAck == DEF_REQ_OK) {
					m_nOperationStep = 1250;
				}
				else {
					m_nOperationStep = 0;
				}
			}
		}break;
	case 1250: // 2D Mark No Use
	{
		//BOOL b2dBarcodeMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use);
		BOOL b2dBarcodeMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use);
		if (b2dBarcodeMode != eOPTION_USE) {
			m_nOperationStep = 1300;
			break;
		}

		BOOL b2dMarkUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_MARK_OPT_USE);
		int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);

		g_TaskAllAutoStacker.Set2dMarkUseIgnore(DEF_REQ_UNK);
		if (b2dMarkUse != eOPTION_USE && nRunMode == eRUN_MODE_ONLINE) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("2D Mark is Off");
			g_Error.AlarmReport(ERR_2DBARCODE_MARK_NO_USE, AlarmData, &g_TaskSystemCtrl);
			m_nOperationStep++;
			break;
		}
		else {
			m_nOperationStep = 1300;
			break;
		}

	}break;
	case 1251:
	{
		int nReqAck = g_TaskAllAutoStacker.Get2dMarkUseIgnore();
		if (nReqAck != DEF_REQ_UNK) {
			if (nReqAck == DEF_REQ_OK) {
				m_nOperationStep = 1300;
			}
			else {
				m_nOperationStep = 0;
			}
		}
	}break;
	case 1300:	// 2D Barcode Duplication checking loop
		{
			BOOL b2dBarcodeMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use);
			if( b2dBarcodeMode != eOPTION_USE){
				m_nOperationStep = 1400;
				break;
			}
			g_TaskAllAutoStacker.Set2dDuplicateIgnore(DEF_REQ_UNK);
			//BOOL bNoDupChek_2D = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_NoDupCheck_2D);	
			BOOL b2dDupCheck = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_DUPLICATION_OPT_USE);
			if(/*bNoDupChek_2D == eOPTION_USE*/b2dDupCheck != eOPTION_USE){
				CParamList AlarmData;
				g_Error.AlarmReport(ERR_2DBARCODE_NO_CHECK_MODE_ON, AlarmData, &g_TaskSystemCtrl);
				m_nOperationStep++;
				break;
			}else{
				m_nOperationStep = 1400;
				break;
			}
		}break;
	case 1301:
		{
			int nReqAck = g_TaskAllAutoStacker.Get2dDuplicateIgnore();
			if (nReqAck != DEF_REQ_UNK) {
				if (nReqAck == DEF_REQ_OK) {
					m_nOperationStep = 1400;
					break;
				}
				else {
					m_nOperationStep = 0;
					break;
				}
			}
			//int nReqAck = DEF_REQ_UNK;
			//while(1){
			//	nReqAck = g_TaskAllAutoStacker.Get2dDuplicateIgnore();
			//	if( nReqAck != DEF_REQ_UNK ){
			//		if( nReqAck == DEF_REQ_OK ){
			//			m_nOperationStep = 1400;
			//			break;
			//		}else{
			//			m_nOperationStep = 0;
			//			break;									
			//		}
			//	}	
			//}
			//g_TaskAllAutoStacker.Set2dDuplicateIgnore(DEF_REQ_UNK);
		}break;
	case 1400:
		{
			BOOL bVisionUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_VisionUse);
			
			g_TaskAllAutoStacker.SetVisionOffIgnore(DEF_REQ_UNK);

			if( bVisionUseMode != eOPTION_USE){
				CParamList AlarmData;
				g_Error.AlarmReport(ERR_VISION_OFF, AlarmData,  &g_TaskSystemCtrl);
				m_nOperationStep++;
				break;
			}else{
				m_nOperationStep = 1500;
			}
		}break;
	case 1401:
		{
			int nReqAck = g_TaskAllAutoStacker.GetVisionOffIgnore();
			if (nReqAck != DEF_REQ_UNK) {
				if (nReqAck == DEF_REQ_OK) {
					m_nOperationStep = 1500;
					break;
				}
				else {
					m_nOperationStep = 0;
					break;
				}
			}
			//int nReqAck = DEF_REQ_UNK;
			//while(1){
			//	nReqAck = g_TaskAllAutoStacker.GetVisionOffIgnore();
			//	if( nReqAck != DEF_REQ_UNK ){
			//		if( nReqAck == DEF_REQ_OK ){
			//			m_nOperationStep = 1500;
			//			break;
			//		}else{
			//			m_nOperationStep = 0;
			//			break;									
			//		}
			//	}
			//}
			//g_TaskAllAutoStacker.SetVisionOffIgnore(DEF_REQ_UNK);
		}break;
	case 1500:
		{
			int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);

			g_TaskAllAutoStacker.SetDeviceModeIgnore(DEF_REQ_UNK);

			if( nDeviceMode != eDeviceMode){
				CParamList AlarmData;
				g_Error.AlarmReport(ERR_DEVICE_MODE_NO_USE, AlarmData,  &g_TaskSystemCtrl);
				m_nOperationStep++;
				break;
			}else{
				m_nOperationStep = 1600;
			}
		}break;
	case 1501:
		{
			int nReqAck = g_TaskAllAutoStacker.GetDeviceModeIgnore();
			if (nReqAck != DEF_REQ_UNK) {
				if (nReqAck == DEF_REQ_OK) {
					m_nOperationStep = 1600;
					break;
				}
				else {
					m_nOperationStep = 0;
					break;
				}
			}
			//int nReqAck = DEF_REQ_UNK;
			//while(1){
			//	nReqAck = g_TaskAllAutoStacker.GetDeviceModeIgnore();
			//	if( nReqAck != DEF_REQ_UNK ){
			//		if( nReqAck == DEF_REQ_OK ){
			//			m_nOperationStep = 1600;
			//			break;
			//		}else{
			//			m_nOperationStep = 0;
			//			break;									
			//		}
			//	}
			//}
			//g_TaskAllAutoStacker.SetDeviceModeIgnore(DEF_REQ_UNK);
		}break;
	case 1600:
		{
			BOOL bRetest_Skip[/*MAX_UNLOAD_SETPLATE*/4] = {g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Retest_Skip_HW1), g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Retest_Skip_HW2),
													  g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Retest_Skip_HW3), g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Retest_Skip_HW4)};
			
			BOOL bRetestSKipOn = FALSE;
			for(int i=0; i</*MAX_UNLOAD_SETPLATE*/4; i++){
				if(bRetest_Skip[i] == TRUE){
					bRetestSKipOn = TRUE;
				}
			}
			g_TaskAllAutoStacker.SetRetestSkipIgnore(DEF_REQ_UNK);

			if(bRetestSKipOn == TRUE){
				CParamList AlarmData;
				g_Error.AlarmReport(ERR_RETEST_SKIP_MODE_USE, AlarmData,  &g_TaskSystemCtrl);
				m_nOperationStep++;
				break;
			}else{
				m_nOperationStep = 1700;
				break;
			}
		}break;
	case 1601:
		{
			int nReqAck = DEF_REQ_UNK;
			while(1){
				nReqAck = g_TaskAllAutoStacker.GetRetestSkipIgnore();
				if( nReqAck != DEF_REQ_UNK ){
					if( nReqAck == DEF_REQ_OK ){
						m_nOperationStep = 1700;
						break;
					}else{
						m_nOperationStep = 0;
						break;									
					}
				}
			}
			g_TaskAllAutoStacker.SetRetestSkipIgnore(DEF_REQ_UNK);
		}break;
	case 1700:
		{
			g_TaskAllAutoStacker.SetPressArriveSensorIgnore(DEF_REQ_UNK);

			if (g_DMCont.m_dmHandlerOpt.GB(BDM11_PRS_DEVICE_ARRIVE_ENABLE) == DEF_OFF) {
				CParamList AlarmData;
				g_Error.AlarmReport(ERR_PRESS_ARRIVE_SENSOR_NOT_USE, AlarmData, &g_TaskSystemCtrl);
				m_nOperationStep++;
			}
			else {
				m_nOperationStep = 1710;//1800;
			}
	}break;
	case 1701:
		{
			int nReqAck = g_TaskAllAutoStacker.GetPressArriveSensorIgnore();
			if (nReqAck != DEF_REQ_UNK) {
				if (nReqAck == DEF_REQ_OK) {
					m_nOperationStep = 1710;//1800;
					break;
				}
				else {
					m_nOperationStep = 0;
					break;
				}
			}
		}break;  
	case 1710:	  // GRR,LOOP,Audit Mode확인 [5/10/2021 donghyun.shin]
		{
			g_TaskAllAutoStacker.SetGRRModeIgnore(DEF_REQ_UNK);
			g_TaskAllAutoStacker.SetLoopModeIgnore(DEF_REQ_UNK);
			g_TaskAllAutoStacker.SetAuditModeIgnore(DEF_REQ_UNK);
			g_TaskAllAutoStacker.SetQAModeIgnore(DEF_REQ_UNK);
			
			char szCompany[16] = { 0, };
			g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
			int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

			if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {			
			}
			else {
				m_nOperationStep = 1800;
				break;
			}
			CParamList AlarmData;

			int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
			if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue) {						
				g_Error.AlarmReport(ERR_AUTO_GRR_MODE_USE, AlarmData, &g_TaskSystemCtrl);
				m_nOperationStep = 1720;
			}							
			else if (nLotStartMode == eStartMD_AutoLoop || nLotStartMode == eStartMD_AutoLoop_Continue) {
				g_Error.AlarmReport(ERR_AUTO_LOOP_MODE_USE, AlarmData, &g_TaskSystemCtrl);
				m_nOperationStep = 1730;
			}	
			else if (nLotStartMode == eStartMD_QA || nLotStartMode == eStartMD_QA_Continue)
			{
				g_Error.AlarmReport(ERR_AUTO_QA_MODE_USE, AlarmData, &g_TaskSystemCtrl);
				m_nOperationStep = 1750;
			}
			else 
				m_nOperationStep = 1800;
		}
		break;
	case 1720: // Grr Mode [5/6/2021 donghyun.shin]
		{
			int nReqAck = g_TaskAllAutoStacker.GetGRRModeIgnore();
			if (nReqAck != DEF_REQ_UNK) {
				if (nReqAck == DEF_REQ_OK) {
					m_nOperationStep = 1800;
					break;
				}
				else {
					m_nOperationStep = 0;
					break;
				}
			}
		}
		break;
	case 1730: // Loop Mode [5/6/2021 donghyun.shin]
	{
		int nReqAck = g_TaskAllAutoStacker.GetLoopModeIgnore();
		if (nReqAck != DEF_REQ_UNK) {
			if (nReqAck == DEF_REQ_OK) {
				m_nOperationStep = 1800;
				break;
			}
			else {
				m_nOperationStep = 0;
				break;
			}
		}
	}
	break;
	case 1740: // Audit Mode [5/6/2021 donghyun.shin]
	{
		int nReqAck = g_TaskAllAutoStacker.GetAuditModeIgnore();
		if (nReqAck != DEF_REQ_UNK) {
			if (nReqAck == DEF_REQ_OK) {
				m_nOperationStep = 1800;
				break;
			}
			else {
				m_nOperationStep = 0;
				break;
			}
		}
	}
	break;
	case 1750: // QA Mode 
	{
		int nReqAck = g_TaskAllAutoStacker.GetQAModeIgnore();
		if (nReqAck != DEF_REQ_UNK) {
			if (nReqAck == DEF_REQ_OK) {
				m_nOperationStep = 1800;
				break;
			}
			else {
				m_nOperationStep = 0;
				break;
			}
		}
	}
	break;
	case 1800:
		{
			g_TaskAllAutoStacker.SetAutoRetestIgnore(DEF_REQ_UNK);

			BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);

			char szCompany[16] = { 0, };
			g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

			if (bAutoRetestUseMode != eOPTION_USE && strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
				CParamList AlarmData;
				g_Error.AlarmReport(ERR_AUTO_RETEST_NOT_USE, AlarmData, &g_TaskSystemCtrl);
				m_nOperationStep++;
			}
			else {
				m_nOperationStep = 1900;
			}
		}break;
	case 1801:
		{
			int nReqAck = g_TaskAllAutoStacker.GetAutoRetestIgnore();
			if (nReqAck != DEF_REQ_UNK) {
				if (nReqAck == DEF_REQ_OK) {
					m_nOperationStep = 1900;
					break;
				}
				else {
					m_nOperationStep = 0;
					break;
				}
			}
		}break;
	case 1900:   // CB File 있는 지 없는지 Check[8/26/2020 donghyun.shin]
	{
		// Amkor가 아니면 원래 시퀀스로 넘겨 버린다.
		int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);

		char szCompany[16] = { 0, };
		g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
		BOOL bCBDataAllCheckOnOff = g_DMCont.m_dmHandlerOpt.GB(BDM0_CB_DATA_OFF_ALL_CHECK_TESTER);
		BOOL bCBDataCheckOnOff = g_DMCont.m_dmHandlerOpt.GB(BDM0_CB_DATA_CHECK_ONOFF);
		int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

		if ( nRunMode == eRUN_MODE_OFFLINE || bCBDataCheckOnOff == DEF_OFF/*nProjectOption == DEF_PROJECT_MARLIN*/) {
			m_nOperationStep = 2000;
			break;
		}

		g_TaskAllAutoStacker.SetCBFileNotExistIgnore(DEF_REQ_UNK);

		int nAllTesterAlram = 0, nAllCBFileNotExist = 0, nErrTemp, nErr = 0;
		CString sPath;
		char szMasterMacPath[MAX_PATH];

		g_MasterMac.m_veMasterMacFileNotExist.clear();

		g_DMCont.m_dmHandlerOpt.GS(SDM0_MASTER_MAC_PATH, szMasterMacPath, sizeof(szMasterMacPath));

 		for (int i = 1; i <= eMaxPressUnitCount; i++)
		{
			sPath.Format("%s\\%d.csv", szMasterMacPath, i);
			nErrTemp = g_MasterMac.CBFileNotExist(sPath);
			nAllTesterAlram += g_MasterMac.m_veMasterMacFileNotExist[i - 1].bFileNotExist;

			if (nErrTemp > 0)
			{
				nErr = nErrTemp;
			}
		}

		if (nAllTesterAlram == eMaxPressUnitCount && bCBDataAllCheckOnOff == 0)
		{
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Tester CB (ALL(X16) File or 경로가 존재 하지 않습니다. ");
				g_Error.AlarmReport(ERR_CB_FILE_NOT_ALL_EXIST, AlarmData, &g_TaskSystemCtrl);
				m_nOperationStep = 0;
				break;
			}
		}

		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tester CB [ %s ]번 File or 경로가 존재 하지 않습니다 ", g_MasterMac.CBFileNotExistIndex());
			g_Error.AlarmReport(ERR_CB_FILE_NOT_EXIST, AlarmData, &g_TaskSystemCtrl);
			m_nOperationStep++;
		}
		else {
			m_nOperationStep = 1910;
		}
	}break;
	case 1901:
	{
		int nReqAck = g_TaskAllAutoStacker.GetCBFileNotExistIgnore();
		if (nReqAck != DEF_REQ_UNK) {
			if (nReqAck == DEF_REQ_OK) {
				m_nOperationStep = 2000;
				break;
			}
			else {
				m_nOperationStep = 0;
				break;
			}
		}
	}break;	
	case 1910:	// CB Data On/Off Check [8/26/2020 donghyun.shin]
	{
		g_TaskAllAutoStacker.SetCBDataOnOffIgnore(DEF_REQ_UNK);
		int nAllTesterAlram =0,nErrTemp=0, nErr=0;
		CString sPath;
		char szMasterMacPath[MAX_PATH];

		g_MasterMac.m_veMasterMacInfo.clear();

		g_DMCont.m_dmHandlerOpt.GS(SDM0_MASTER_MAC_PATH,szMasterMacPath,sizeof(szMasterMacPath));
		BOOL bCBDataAllCheckOnOff = g_DMCont.m_dmHandlerOpt.GB(BDM0_CB_DATA_OFF_ALL_CHECK_TESTER);
		
		for (int i = 1; i <= eMaxPressUnitCount; i++)
		{
			sPath.Format("%s\\%d.csv", szMasterMacPath, i);
			nErrTemp = g_MasterMac.CBFileDataRead(sPath);
			nAllTesterAlram += g_MasterMac.m_veMasterMacInfo[i-1].nOnOff;
			if (nErrTemp > 0){
				nErr = nErrTemp;
			}
		}

		if (nAllTesterAlram == 0 && bCBDataAllCheckOnOff == 0) // All Station Alram은 시작을 하지 않는다. [8/26/2020 donghyun.shin]
		{
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Tester (ALL(X16)  CB Data Off!! ");
				g_Error.AlarmReport(ERR_CB_DATA_ALL_OFF, AlarmData, &g_TaskSystemCtrl);
				m_nOperationStep = 0;
				break;
			}
		}

		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tester [ %s ]번 CB Data Off!! ", g_MasterMac.CBDataOnOffIndex());
			g_Error.AlarmReport(ERR_CB_DATA_ONOFF, AlarmData, &g_TaskSystemCtrl);
			m_nOperationStep++;
		}
		else{
			m_nOperationStep = 1920;
		}
	}break;
	case 1911:
	{
		int nReqAck = g_TaskAllAutoStacker.GetCBDataOnOffIgnore();
		if (nReqAck != DEF_REQ_UNK) {
			if (nReqAck == DEF_REQ_OK) {
				m_nOperationStep = 1920;
				break;
			}
			else {
				m_nOperationStep = 0;
				break;
			}
		}
	}break;
	case 1920:   // CB Modify Time Check [8/26/2020 donghyun.shin]
	{
		int nAllTesterAlram = 0, nAllCBFileNotExist = 0, nErrTemp, nErr = 0;
		CString sPath;
		char szMasterMacPath[MAX_PATH];

		char szCompany[16] = { 0, };
		g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
		BOOL bCBDataAllCheckOnOff = g_DMCont.m_dmHandlerOpt.GB(BDM0_CB_DATA_OFF_ALL_CHECK_TESTER);

		g_TaskAllAutoStacker.SetCBModifyTimeCheckIgnore(DEF_REQ_UNK);

		g_MasterMac.m_veMasterMacFileCheck.clear();

		g_DMCont.m_dmHandlerOpt.GS(SDM0_MASTER_MAC_PATH, szMasterMacPath, sizeof(szMasterMacPath));

		for (int i = 1; i <= eMaxPressUnitCount; i++)
		{
			sPath.Format("%s\\%d.csv", szMasterMacPath, i);
			nErrTemp = g_MasterMac.CBFileModifyTimeCheck(sPath);
			nAllTesterAlram += g_MasterMac.m_veMasterMacFileCheck[i - 1].bFileModifyTimeOver;

			if (nErrTemp > 0)
			{
				nErr = nErrTemp;
			}
		}

		if (nAllTesterAlram == eMaxPressUnitCount && bCBDataAllCheckOnOff == 0)
		{
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Tester (ALL(X16) Data Access 6시간 이상 경과 ");
				g_Error.AlarmReport(ERR_CB_DATA_MODIFY_ALL_TIME_OVER, AlarmData, &g_TaskSystemCtrl);
				m_nOperationStep = 0;
				break;
			}
		}

		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Tester [ %s ]번 Data Access 6시간 이상 경과 ", g_MasterMac.CBFileModifyIndex());
			g_Error.AlarmReport(ERR_CB_DATA_MODIFY_TIME_OVER, AlarmData, &g_TaskSystemCtrl);
			m_nOperationStep++;
		}
		else {
			m_nOperationStep = 2000;
		}
	}break;
	case 1921:
		{
		int nReqAck = g_TaskAllAutoStacker.GetCBModifyTimeCheckIgnore();
		if (nReqAck != DEF_REQ_UNK) {
			if (nReqAck == DEF_REQ_OK) {
				m_nOperationStep = 2000;
				break;
			}
			else {
				m_nOperationStep = 0;
				break;
			}
		}
	}break;
	case 2000:
		{
			// 하나의 module에는 최소 2개의 Station 이 살아 있어야 된다. 
			// Module 중 하나는 Skip 해도 2개가 있는 지 확인
			int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
			
			char szCompany[16] = { 0, };
			g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
			int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);

			if (nRunMode == eRUN_MODE_OFFLINE ) {
				m_nOperationStep = 2100;
				break;
			}

			g_TaskAllAutoStacker.SetDisConnectionStation(DEF_REQ_UNK);
			
			int nIdx = 0;
			std::vector<int> vIdx = g_TaskSystemCtrl.GetDisConnectionStationIdx();

			BOOL bDisconnection = g_TaskSystemCtrl.CheckTesterDisConnection(nIdx,vIdx);

			// 문제의 station 반환 nIdx
			if (bDisconnection) {	// CheckTesterDisConnection 함수에서 에러 상태로 반환. 
				BOOL bIsPossibleStationOff = g_TaskSystemCtrl.IsPossibleStationOff(nIdx);

				if (bIsPossibleStationOff) {
					// 사용자 Skip,취소 유무 확인 Message
					CString strErr = _T("");
		
					if (nTesterOption == eTester_Single || nTesterOption == eTester_Agent)
						strErr.Format(_T("[Station : %d]"), nIdx + 1);
					else 
						strErr.Format("[Station : %d, Tester=%d-%d]", nIdx / 2 + 1, nIdx / 2 + 1, (nIdx % 2) + 1);

					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat(strErr);
					g_Error.AlarmReport(ERR_STATION_DISCONNECTION, AlarmData, &g_TaskSystemCtrl);
					g_TaskSystemCtrl.SetDisConnectionStationIdx(nIdx);
					m_nOperationStep++;
					break;
				}
				else {
					//Station Off를 할수 없다 Message
// 					CParamList AlarmData;
// 					g_Error.AlarmReport(ERR_IS_NOT_POSSIBLE_ONLINE_RUNNING, AlarmData, NULL);
// 					g_TaskSystemCtrl.ClearDisConnectionStationIdx();
// 					m_nOperationStep = 0;

					m_nOperationStep = 2100;
					break;
				}

			}
			else {
				//Disconnection 된 Station이 없으면 다음 Step로 이동
				m_nOperationStep = 2100;
				break;
			}
		}break;
	case 2001:
		{
			int nReqAck = g_TaskAllAutoStacker.GetDisConnectionStation();
			if (nReqAck != DEF_REQ_UNK) {
				if (nReqAck == DEF_REQ_OK) {
					//Disconnection 된 다른 Station 유무 확인
					Sleep(300);
					m_nOperationStep = 2000;
					break;
				}
				else {
					m_nOperationStep = 0;
					break;
				}
			}
		}break;

	case 2100:
		{
			int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
			g_TaskAllAutoStacker.SetOffLineUseIgnore(DEF_REQ_UNK);
			g_TaskAllAutoStacker.SetOnlineTesterMode(DEF_REQ_UNK);

			int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
			if (nLotStartMode == eStartMD_QA || nLotStartMode == eStartMD_QA_Continue) 
			{
				int nQaSampleCount = g_TaskSystemCtrl.GetQASampleCount();
				if (nQaSampleCount > 200 || nQaSampleCount <= 0)
				{
					CParamList AlarmData;	
					AlarmData.m_fnAddItemFormat("[QA Setting Count =%d]", nQaSampleCount);
					g_Error.AlarmReport(ERR_QA_MODE_QTY_OVER, AlarmData, &g_TaskSystemCtrl);
					m_nOperationStep = 0;
					break;
				}
			}

			char szCompany[16] = { 0, };
			g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
		
			if (nRunMode == eRUN_MODE_OFFLINE || strcmp(szCompany, DEF_COMPANY_AMKOR) == 0)
			{
				m_nOperationStep = 3000;
				break;
			}
			else
			{
				int nMD_TesterIF = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_TESTER_IF_PRO_AUDIT);
				int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
				int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);

				CString strMode = _T(""), strHandlerMode = _T("");
				g_TaskSystemCtrl.LotTesterModeText(strHandlerMode, strMode);
				if (nRunMode == eRUN_MODE_ONLINE)
				{
					if ((nLotStartMode == eStartMD_Initial || nLotStartMode == eStartMD_InitContinue) && nMD_TesterIF == eTesterIF_Mode_Normal)
					{
						m_nOperationStep = 3000;
						break;
					}
					else
					{
						if (g_GuiComm.SendLotinfoShow() == NO_ERROR)
						{
							m_nOperationStep++;
							Sleep(200);
						}
						else {
							AfxMessageBox(" SLT Program Dialog command error!!!");
							m_nOperationStep = 0;
						}			
						break;
					}
				}
			}
		}
		break;
	case 2101:
		{
			if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_LOTINFO_DLG_HISTORY) == FALSE)
			{
				m_nOperationStep++;
			}
		}
		break;
	case 2102:
		{
			int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
			g_TaskAllAutoStacker.SetOffLineUseIgnore(DEF_REQ_UNK);
			g_TaskAllAutoStacker.SetOnlineTesterMode(DEF_REQ_UNK);

			char szCompany[16] = { 0, };
			g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

			if (nRunMode == eRUN_MODE_OFFLINE || strcmp(szCompany, DEF_COMPANY_AMKOR) == 0 )
			{
				m_nOperationStep = 3000;
				break;
			}
			else 
			{
				int nMD_TesterIF = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_TESTER_IF_PRO_AUDIT);
				int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
				int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);

				CString strMode = _T(""), strHandlerMode = _T("");
				g_TaskSystemCtrl.LotTesterModeText(strHandlerMode, strMode);
				if (nRunMode == eRUN_MODE_ONLINE)
				{
					if ((nLotStartMode == eStartMD_Initial || nLotStartMode == eStartMD_InitContinue) && nMD_TesterIF == eTesterIF_Mode_Normal)
					{
						m_nOperationStep = 3000;
						break;
					}
					else
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Check Please!! [ Run Mode=Online ] [Handler Mode=%s] [ Tester Mode=%s ]", strHandlerMode, strMode);
						g_Error.AlarmReport(ERR_ONLINE_USE, AlarmData, &g_TaskSystemCtrl);
						m_nOperationStep++;
						break;
					}
				}
			}
		}
		break;
	case 2103:
		{
			int nReqAck = g_TaskAllAutoStacker.GetOnlineTesterMode();
			if (nReqAck != DEF_REQ_UNK) {
				if (nReqAck == DEF_REQ_OK) {
					m_nOperationStep = 3000;
				}
				else {
					m_nOperationStep = 0;
				}
			}
		}
		break;
	case 3000:	// 움직이는 축이 있는지 점검한다.
		{
			m_tcMotionDone.SetTimerCheckEnd();
			m_tcMotionDone.SetCheckTime(2.0);
			m_tcMotionDone.StartTimer();
			m_nOperationStep = 3001;

			// jira 403

			int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
			if (nDeviceMode != eDeviceMode && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
			{
				CTaskTrayBasePP* pTaskTrayPP[] = {&g_TaskTrayLoadPP, &g_TaskTrayUnloadPP};
				for (int nModuleIdx = 0; nModuleIdx < MAX_TRAY_PP_CNT; nModuleIdx++)
				{
					int nErr = pTaskTrayPP[nModuleIdx]->GetVacuum_All(eOff);
					if (nErr != ERR_NO_ERROR)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("[ NoDevice mode ]please check sensing device!!");
						g_Error.AlarmReport(ERR_TRAY_PP_1_VAC_SEN_ERR+ nModuleIdx, AlarmData, &g_TaskSystemCtrl);
						m_nOperationStep = 0;
						return;
					}
				}

				for (int nModuleIdx = 0; nModuleIdx < MAX_TEST_PP_CNT; nModuleIdx++)
				{
					for (int i = 0; i < eMaxTestPPTypeCount; i++)
					{
						int nErr = g_TaskTestPP[nModuleIdx].GetVacuum_All(i,DEF_OFF);
						if (nErr != ERR_NO_ERROR)
						{
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("[ NoDevice mode ]please check sensing device!!");
							g_Error.AlarmReport(ERR_TEST_PP_1_X1_VAC_SEN_ERR + g_TaskTestPP[nModuleIdx].GetHandTypeIndex(i), AlarmData, &g_TaskSystemCtrl);
							m_nOperationStep = 0;
							return;
						}
					}
		
				}
				for (int nModuleIdx = 0; nModuleIdx < MAX_TRAY_PP_CNT; nModuleIdx++)
					pTaskTrayPP[nModuleIdx]->do_NoDeviceResetAll();
				for (int nModuleIdx = 0; nModuleIdx < MAX_TEST_PP_CNT; nModuleIdx++)
					g_TaskTestPP[nModuleIdx].do_NoDeviceResetAll();
			}
		}break;
	case 3001:
		{
			m_nOrgRetryCount = 0;
			int nMotionCnt = 0;
			int nMotorCount = g_Motor.GetAxisCount();
			CString strProblemAxis = _T("");
			std::vector<CString> vProblemAxis; 
			vProblemAxis.clear();
			//int nEQPNo = g_DMCont.m_dmEQP.GN(NDM11_Handler_EQP_No);

			for (int i = 0; i<nMotorCount; i++)
			{			
				int nMotionDone = g_Motor[i]->MotionDone();

				if (nMotionDone != ERR_NO_ERROR) {
					nMotionCnt++;
					strProblemAxis += g_Motor[i]->GetAxisName();
					vProblemAxis.push_back(g_Motor[i]->GetAxisName());
				}						
			}
			if (nMotionCnt == 0)
			{
				m_nOperationStep = 3010;
				break;
			}
			else 
			{
				if (m_tcMotionDone.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					for (int i = 0; i < vProblemAxis.size(); i++)
					{
						AlarmData.m_fnAddItemFormat("Axis Name = %s", vProblemAxis[i]);
					}
					
					AlarmData.m_fnAddItemFormat("Motor is not end motion done. wait 3.0sec over time!");
					g_Error.AlarmReport(ERR_MOT_NOT_END_MOTION_DONE, AlarmData, &g_TaskSystemCtrl);
					m_nOperationStep = 0;
				}
			}
		}break;
// 	case 3002: // Test PP Z Origin Servo ON
// 	{
// 		for (int i = 0; i < eMaxTestPPCount; i++) {
// 			OneAxis* pMotorZ1 = g_TaskTestPP[i].GetAxis(CTaskTestPP::eAxisZ1);
// 			OneAxis* pMotorZ2 = g_TaskTestPP[i].GetAxis(CTaskTestPP::eAxisZ2);
// 			OneAxis* pMotor[2] = { pMotorZ1, pMotorZ2 };
// 			for (int j = 0; j < 2; j++) {
// 				pMotor[j]->ClearOrgFlag();
// 				pMotor[j]->SetStop();
// 				Sleep(100);
// 				pMotor[j]->AlarmReset();
// 				Sleep(100);
// 				pMotor[j]->ServoOn();
// 				Sleep(100);
// 				pMotor[j]->EnableSWLimit(DEF_DISABLE);
// 			}
// 		}
// 		m_tcTimeout.SetCheckTime(TIME_OUT_SHORT);
// 		m_tcTimeout.StartTimer();
// 		m_nOperationStep++;
// 		
// 	}break;
// 	case 3003: // Test PP Z Origin
// 	{
// 		int nChk = 0;
// 		for (int i = 0; i < eMaxTestPPCount; i++)
// 		{
// 			OneAxis* pMotorZ1 = g_TaskTestPP[i].GetAxis(CTaskTestPP::eAxisZ1);
// 			OneAxis* pMotorZ2 = g_TaskTestPP[i].GetAxis(CTaskTestPP::eAxisZ2);
// 
// 			OneAxis* pMotor[2] = { pMotorZ1, pMotorZ2 };
// 			int nErrOrg = 0;
// 			int nComp = 0;
// 			for (int j = 0; j < 2; j++)
// 			{
// 				nErrOrg = pMotor[j]->Origin();
// 				if (nErrOrg != NO_ERROR) {
// 					m_nOrgRetryCount++;
// 					if (m_nOrgRetryCount >= MAX_MOTOR_RETRY_COUNT) {
// 						CParamList AlarmData;
// 						AlarmData.m_fnAddItemFormat("Test Pp Multi origin Z axis error");
// 						g_Error.AlarmReport(ERR_TEST_PP_1_MOTOR_ORG_FAIL + g_TaskTestPP[i].m_nTestPPIdx, AlarmData,&g_TaskSystemCtrl);
// 						m_nOperationStep = 0;
// 					}
// 					m_nOperationStep--;
// 					break;
// 				}
// 				else
// 				{
// 					BOOL bCompleted = pMotor[j]->IsOrgCompleted(&nErrOrg);
// 					if (bCompleted)
// 					{
// 						pMotor[j]->EnableSWLimit(DEF_ENABLE);
// 						int nErr = pMotor[j]->ChkMotorUsable();
// 						if (nErr == ERR_NO_ERROR && pMotor[j]->IsHomeSensorOn() == DEF_ON) {
// 							nComp++;
// 						}
// 					}
// 					else {
// 						if (m_tcTimeout.CheckOverTime())
// 						{
// 							CParamList AlarmData;
// 							AlarmData.m_fnAddItemFormat("%s Axis", pMotor[j]->GetAxisName());
// 							g_Error.AlarmReport(ERR_TEST_PP_1_MOTOR_ORG_TIMEOUT + g_TaskTestPP[i].m_nTestPPIdx, AlarmData,&g_TaskSystemCtrl);
// 							break;
// 						}
// 					}
// 				}
// 			}// for end
// 
// 			if (nComp == 2) {
// 				nChk++;
// 			}
// 		}
// 
// 		if (nChk == eMaxTestPPCount) {
// 			m_nOperationStep++;
// 		}
// 	}break;
// 	case 3004:// Test PP Z Safety Move
// 	{
// 		std::vector<CPoint> vPkr;
// 		g_TaskTrayLoadPP.MakeAllPicker(vPkr);
// 		vPkr.push_back(CPoint(0, 0));
// 		for (int i = 0; i < eMaxTestPPCount; i++)
// 		{
// 			OneAxis* pMotorZ1 = g_TaskTestPP[i].GetAxis(CTaskTestPP::eAxisZ1);
// 			OneAxis* pMotorZ2 = g_TaskTestPP[i].GetAxis(CTaskTestPP::eAxisZ2);
// 
// 			OneAxis* pMotor[eTestPPType::eMaxTestPPTypeCount] = { pMotorZ1, pMotorZ2 };
// 			vPkr.clear();
// 			g_TaskTestPP[i].MakeAllPicker(vPkr);
// 			int nAxisNum[2] = { CTaskTestPP::eAxisZ1 ,CTaskTestPP::eAxisZ2 };
// 			for (int nLoadUnload = 0; nLoadUnload < eTestPPType::eMaxTestPPTypeCount; nLoadUnload++)
// 			{
// 				int nErr = g_TaskTestPP[i].do_MovePos(eLocIdxTestPP::LOC_IDX_TEST_PP_HOME, nLoadUnload, CPoint(0, 0), vPkr, CTaskTestPP::eTeachPosZ::eZPOS_SAFETY, nAxisNum[nLoadUnload]);
// 				if (nErr != ERR_NO_ERROR) {
// 					CParamList AlarmData;
// 					g_Error.AlarmReport(nErr, AlarmData,&g_TaskSystemCtrl);
// 					break;
// 				}
// 			}
// 		}
// 
// 		m_tcTimeout.ResetStartTime();
// 		m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_SHORT);
// 		m_nOperationStep++;
// 	}break;
// 	case 3005: // Test PP Z Safety Move Done
// 	{
// 		int nComp = 0;
// 
// 		for (int i = 0; i < eMaxTestPPCount; i++) {
// 
// 			OneAxis* pMotorZ1 = g_TaskTestPP[i].GetAxis(CTaskTestPP::eAxisZ1);
// 			OneAxis* pMotorZ2 = g_TaskTestPP[i].GetAxis(CTaskTestPP::eAxisZ2);
// 
// 			OneAxis* pMotor[eTestPPType::eMaxTestPPTypeCount] = { pMotorZ1, pMotorZ2 };
// 
// 			for (int nLoadUnload = 0; nLoadUnload < eTestPPType::eMaxTestPPTypeCount; nLoadUnload++)
// 			{
// 				OneAxis* pMotorZ = pMotor[nLoadUnload];			
// 				int nAxisDone = pMotorZ->AxisDone();
// 				if (nAxisDone == ERR_NO_ERROR && pMotorZ->IsHomeSensorOn() == TRUE) {
// 					nComp++;
// 				}
// 				else
// 				{
// 					if (m_tcTimeout.CheckOverTime() == TRUE) {
// 						CParamList AlarmData;
// 						g_Error.AlarmReport(ERR_TEST_PP_1_MOTOR_MOVE_TIMEOUT + i, AlarmData,&g_TaskSystemCtrl);
// 						break;
// 					}
// 				}
// 			}
// 		}
// 
// 		if (nComp == eTestPPType::eMaxTestPPTypeCount *2)
// 		{
// 			m_nOperationStep++;
// 		}
// 	}break;
// 	case 3006: // Tray PP Z Servo ON
// 	{
// 		CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
// 		for (int i = 0; i < eMaxTrayPPCount; i++) {
// 			OneAxis* pMotorZ = pTaskTrayPP[i]->GetAxis(CTaskTrayBasePP::eAxisZ);
// 			OneAxis* pMotor[] = { pMotorZ };
// 			for (int j = 0; j < _countof(pMotor); j++) {
// 				pMotor[j]->ClearOrgFlag();
// 				pMotor[j]->SetStop();
// 				Sleep(100);
// 				pMotor[j]->AlarmReset();
// 				Sleep(100);
// 				pMotor[j]->ServoOn();
// 				Sleep(100);
// 				pMotor[j]->EnableSWLimit(DEF_DISABLE);
// 			}
// 		}
// 		m_tcTimeout.SetCheckTime(TIME_OUT_SHORT);
// 		m_tcTimeout.StartTimer();
// 		m_nOperationStep++;
// 	}break;
// 	case 3007:  // Tray PP Z Origin
// 	{
// 		int nChk = 0;
// 		CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
// 		for (int i = 0; i < eMaxTrayPPCount; i++)
// 		{
// 			OneAxis* pMotorZ = pTaskTrayPP[i]->GetAxis(CTaskTrayBasePP::eAxisZ);
// 
// 			OneAxis* pMotor[] = { pMotorZ };
// 			int nErrOrg = 0;
// 			int nComp = 0;
// 			for (int j = 0; j < _countof(pMotor); j++)
// 			{
// 				nErrOrg = pMotor[j]->Origin();
// 				if (nErrOrg != NO_ERROR) {
// 					m_nOrgRetryCount++;
// 					if (m_nOrgRetryCount >= MAX_MOTOR_RETRY_COUNT) {
// 						CParamList AlarmData;
// 						AlarmData.m_fnAddItemFormat("Test Pp Multi origin Z axis error");
// 						g_Error.AlarmReport(ERR_TRAY_PP_1_MOTOR_ORG_FAIL + pTaskTrayPP[i]->GetRobotIdIdx(), AlarmData, &g_TaskSystemCtrl);
// 						m_nOperationStep = 0;
// 					}
// 					m_nOperationStep--;
// 					break;
// 				}
// 				else
// 				{
// 					BOOL bCompleted = pMotor[j]->IsOrgCompleted(&nErrOrg);
// 					if (bCompleted)
// 					{
// 						pMotor[j]->EnableSWLimit(DEF_ENABLE);
// 						int nErr = pMotor[j]->ChkMotorUsable();
// 						if (nErr == ERR_NO_ERROR && pMotor[j]->IsHomeSensorOn() == DEF_ON) {
// 							nChk++;	
// 						}
// 					}
// 					else {
// 						if (m_tcTimeout.CheckOverTime())
// 						{
// 							CParamList AlarmData;
// 							AlarmData.m_fnAddItemFormat("%s Axis", pMotor[j]->GetAxisName());
// 							g_Error.AlarmReport(ERR_TEST_PP_1_MOTOR_ORG_TIMEOUT + pTaskTrayPP[i]->GetRobotIdIdx(), AlarmData, &g_TaskSystemCtrl);
// 							break;
// 						}
// 					}
// 				}
// 			}// for end		
// 			
// 		}
// 
// 		if (nChk == eMaxTestPPCount) {
// 			m_nOperationStep++;
// 		}
// 	}break;
// 	case 3008:  // Tray PP Z Safety Move
// 	{
// 		std::vector<CPoint> vPkr;
// 		g_TaskTrayLoadPP.MakeAllPicker(vPkr);
// 		vPkr.push_back(CPoint(0, 0));
// 		CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
// 		for (int i = 0; i < eMaxTrayPPCount; i++)
// 		{
// 			OneAxis* pMotorZ = pTaskTrayPP[i]->GetAxis(CTaskTrayBasePP::eAxisZ);
// 
// 			OneAxis* pMotor[] = { pMotorZ };
// 			vPkr.clear();
// 			g_TaskTestPP[i].MakeAllPicker(vPkr);
// 			int nErr = pTaskTrayPP[i]->do_MovePos(eLocIdxTrayPP::LOC_IDX_TRAY_PP_SAFETY, CPoint(0, 0), vPkr, CTaskTrayBasePP::eTeachPosZ::eZPOS_SAFETY, CTaskTrayBasePP::eAxisZ);
// 			if (nErr != ERR_NO_ERROR) {
// 				CParamList AlarmData;
// 				g_Error.AlarmReport(nErr, AlarmData, &g_TaskSystemCtrl);
// 				break;
// 			}
// 			
// 		}
// 
// 		m_tcTimeout.ResetStartTime();
// 		m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_SHORT);
// 		m_nOperationStep++;
// 	}break;
// 	case 3009:  // Tray PP Z Safety Move Done
// 	{
// 		int nComp = 0;
// 		CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
// 		for (int i = 0; i < eMaxTrayPPCount; i++) 
// 		{
// 			OneAxis* pMotorZ = pTaskTrayPP[i]->GetAxis(CTaskTrayBasePP::eAxisZ);
// 			pMotorZ->IsHomeSensorOn();
// 			int nAxisDone = pMotorZ->AxisDone();
// 			if (nAxisDone == ERR_NO_ERROR && pMotorZ->IsHomeSensorOn() == TRUE) {
// 				nComp++;
// 			}
// 			else
// 			{
// 				if (m_tcTimeout.CheckOverTime() == TRUE) {
// 					CParamList AlarmData;
// 					g_Error.AlarmReport(ERR_TRAY_PP_1_MOTOR_MOVE_TIMEOUT + i, AlarmData, &g_TaskSystemCtrl);
// 					break;
// 				}
// 			}			
// 		}
// 
// 		if (nComp == eMaxTrayPPCount)
// 		{
// 			m_nOperationStep++;
// 		}
// 	}break;
	case 3010:
		{
			int nErr = g_TaskSystemCtrl.OnPanelButtonStart();
			if(nErr == ERR_NO_ERROR){
				m_nOperationStep = 3011;
				break;
			}
			else if (nErr == ERR_LOT_CMD_CANNOT_PERFORM_NOW) {
				m_nOperationStep = 0;
				break;
			}
			m_nOperationStep = 0;
		}break;
	case 3011:
		{
			// jira 355 추가
		// Tester 검사 도중 영향을 줌으로 주석 처리. [3/11/2021 donghyun.shin]
// 			double dForcePerPin = g_DMCont.m_dmHandlerOpt.GD(DDM11_DvcContactForce_GF);
// 			if (dForcePerPin <= 0.0) {
// 				dForcePerPin = 0.0;
// 			}
// 			for (int i = 0; i < eMaxPressUnitCount; i++)
// 			{
// 				g_TaskRegulator[i].do_SetForcePerPin(dForcePerPin);
// 			}

			SetStatus(TRUE);
			if(!m_bBlinkFlag){
				OutPutLed(1);
			}
			ASSERT(m_pcGroup);
			if(m_pcGroup)	{	//본키가 눌렸을때 꺼져야할 스위치의 포인터
				m_pcGroup->SetStatus(FALSE);
			}

			g_IO.out(DO_FRONT_LAMP_ON, DEF_ON);
			g_IO.out(DO_TEST_SITE_LAMP_ON, DEF_ON);
			g_LAMP_SW.SetStatus(TRUE);

// 			BOOL bUseNoUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_VisionUse);
// 			if (bUseNoUse == eOPTION_USE)
// 			{
// 				g_pVisionTestHand_1->Cmd_VisionReady_HandCam(eVisionLotStateStart);
// 			}
			g_TaskSystemCtrl.MakeLog("Push Start Switch");
			m_nOperationStep = 0;
		}break;
	}// end of switch
}

