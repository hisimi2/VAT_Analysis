#include "StdAfx.h"
#include "LoadTblCmd_MovePos.h"
#include "DEF_DEVICE_DATA.h"

#include <vector>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _DEF_REL_TEST	 _T("REL_")
//==============================================================================
//
//==============================================================================
CLoadTblCmd_MovePos::CLoadTblCmd_MovePos( void )
{
	m_nCmdId = eSeqCmd_LoadTbl_MovePos;
	m_nCmdStage = 0;
	m_nRetryCount = 0;
	//m_bRetryVision = FALSE;
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));
	m_nBarcodeMaxRetry = 0;
	m_bIsBarcodeDup = FALSE;

	m_nTableIdx = 0;
	m_nImpactMoveCnt = 0;

	m_nPocketCntX = 0;
	m_nPocketCntY = 0;
	m_nBackVibratorStep = 0;
	m_nVibCurTryCnt = 0;
	m_nMoveStep = 0;
	//m_nStepRow = 0;
}



//==============================================================================
//
//==============================================================================
CLoadTblCmd_MovePos::~CLoadTblCmd_MovePos( void )
{
}


//==============================================================================
//
//==============================================================================
int CLoadTblCmd_MovePos::CheckParam()
{
	CTaskLoadTable* pTaskLoadTbl = (CTaskLoadTable*)m_pReceive;

	m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE);
	m_tcTimeout.StartTimer();

	m_nTableIdx = pTaskLoadTbl->m_nTableIdx;

	m_nPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);
	m_nPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);

	m_nBarcodeMaxRetry = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_BarcodeMaxRetry);

	// 1. Setplate로 Move 할 경우 시간을 기록 한다.
	// 2. 시간 기록을 하여야, Tray PP에서 작업 할 때 순서처리 가능 함.
	//if( m_nCmdStage ==  LOC_IDX_TBL_LD_SIDE && CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto ) {
	//	CTime time = CTime::GetCurrentTime();
	//	pTaskLoadTbl->SetMoveSetPlateTm( time );
	//}

	m_nWorkCmdStage = m_nCmdStage;

	m_nVibCurTryCnt = 0;

	return ERR_NO_ERROR;
}


//==============================================================================
//
//==============================================================================
int  CLoadTblCmd_MovePos::OnStop()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Aborting );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CLoadTblCmd_MovePos::OnRetry()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CLoadTblCmd_MovePos::OnSkip()
{
	ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_AlarmComplete );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CLoadTblCmd_MovePos::OnPause()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Pause );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CLoadTblCmd_MovePos::OnResume()
{
	SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Recovery );
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CLoadTblCmd_MovePos::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}

//==============================================================================
//
//==============================================================================
BOOL CLoadTblCmd_MovePos::State_Executing()
{
	// check alarm
	CTaskLoadTable* pTaskLoadTbl = ( CTaskLoadTable* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		//pTaskLoadTbl->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	for( int i = 0; i < CTaskLoadTable::eMaxAxisCount; i++ ) {
		OneAxis* pMotor = pTaskLoadTbl->GetAxis( i );
		if( pMotor != NULL )
		{
			if( pMotor->ChkMotorUsable() != ERR_NO_ERROR ) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat( "Load Table %d check motor usable Cmd Error", m_nTableIdx + 1 );
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(ERR_LOAD_TBL_1_MOTOR_ALARMED + m_nTableIdx, AlarmData );
				return FALSE;
			}
		}
	}

	//임시
// 	CParamList AlarmData;
// 	AlarmData.m_fnAddItemFormat("Load Table arrived safety sensor is detected");
// 	ReportAlarm(ERR_CP_IS_TRIP, AlarmData);
//    	return FALSE;

	int nTestPPIdx = (pTaskLoadTbl->m_nTableIdx / 2);
	if (m_nCmdStage >= LOC_IDX_LD_TBL_TEST_SIDE_1 && g_TaskTestPP[nTestPPIdx].GetSeqCmdStatus() != CBaseSeqCmd::eSeqCmdStatus_Ready /* && vLocList[CTaskLoadTable::eAxisY]> CTaskLoadTable::eYPOS_LoadSide*/)
	{
		int nTestPPType = g_TaskTestPP[nTestPPIdx].m_nTestPPType;

		std::vector<int>vLocTestPpList;
		vLocTestPpList.clear();
		g_TaskTestPP[nTestPPIdx].WhereIsMotorPos(nTestPPType, vLocTestPpList, CTaskTestPP::eAxisAll);

		int nErrSenZ = g_TaskTestPP[nTestPPIdx].Chk_Z_Safety_Sen_All(nTestPPType);
		int nAxisX = (nTestPPType == eTestPPLoadType) ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2;
		if (nErrSenZ != ERR_NO_ERROR && vLocTestPpList[nAxisX] != CTaskTestPP::ePosX_StationAndPosiLimit) {
			return FALSE;
		}
	}


	switch( m_nStep )
	{
		case 0:	// check interlock
			{
				pTaskLoadTbl->MakeLog( "[%s] [EXEC=%8d]", m_szCmdName, m_nStep );

				m_tcTimeout.SetCheckTime(TIME_OUT_SHORT);
				m_tcTimeout.StartTimer();
				m_nStep = 1000;
			}break;
		//checking Load Table Vibrator OFF
		case 1000:
			{
				if( m_tcTimeout.CheckOverTime() == TRUE )
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( "OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1 );
					ReportAlarm(ERR_LOAD_TBL_1_VIBRATOR_CYLINDER_BWD_TIMEOUT + m_nTableIdx, AlarmData );
					break;
				}

				if (g_DMCont.m_dmEQP.GN(NDM0_TABLE_VIBRATOR_OPTION) == DEF_ON)
				{
					int nErr = pTaskLoadTbl->chk_cyl_vibrator_onoff(DEF_OFF);
					if (nErr != ERR_NO_ERROR)
					{
						pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] Interlock Vibrator On -> Off", m_szCmdName, m_nStep);
						m_nBackVibratorStep = m_nStep;
						m_nStep = do_Vibrator_Off;
						break;
					}					
				}


				//임시
				m_nStep = 3000;
// 				if( g_TaskAutoLdTable[ m_nTableIdx ].ChkMoveLoadSite() == ERR_NO_ERROR
// 					&& g_TaskAutoLdTable[ m_nTableIdx ].ChkMoveTestSite() == ERR_NO_ERROR )
// 				{
// 					m_nImpactMoveCnt = 0;
// 					m_nStep = 3000;
// 				}
			}break;

		case 3000: // check loop
			{
				m_nRetryCount = 0;
				if( m_nCmdStage >= LOC_IDX_LD_TBL_TEST_SIDE_1 ) { // move test site
					m_nStep = 20000;
				}
				else {										   // move Load site
					m_nStep = 12000;
				}
			}break;
		case 10000: //ftp 서버 2d barcode 삭제
		{
// 			CInternetSession session;
// 			CFtpConnection* pConnect = NULL;
// 			CFtpFileFind* pFileFind = NULL;
// 			CString strDir, StrResult;
// 
// 			try
// 			{
// 				pConnect = session.GetFtpConnection()
// 			}
// 			catch (CInternetException *e)
// 			{
// 				e->ReportError(MB_ICONEXCLAMATION);
// 			}

		}break;

			//=======================================================================
			// move to setplate
			//=======================================================================
		case 12000: // check arrived seet sensor 
			{
				BOOL bArrivedSen = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_LoadTblArrived);
				BOOL bIsAfterMoveChk = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_LoadTbl_AfterMove_Arrived);
				BOOL bVibratorOnOff = g_DMCont.m_dmHandlerOpt.GN(NDM11_LOADING_TABLE_VIBRATOR_OPTION);
				if (bArrivedSen && bIsAfterMoveChk && bVibratorOnOff != DEF_ON) {
					pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [After Move Check]", m_szCmdName, m_nStep);
					m_nStep = 12100;
					break;
				}


				pTaskLoadTbl->MakeLog( "[%s] [EXEC=%8d] [Arrive Sensor Check](STR)", m_szCmdName, m_nStep );
				CString strErr = _T("");
				std::vector<CPoint> vFailPocket;
				std::vector<CString> vErrHistory;

				BOOL bRetErr = ChkErrArrivedSen(strErr, vFailPocket, vErrHistory);
				BOOL bImpactTbl = g_DMCont.m_dmHandlerOpt.GB( BDM11_MD_ImpactTbl );

				if( bRetErr != ERR_NO_ERROR && pTaskLoadTbl->m_bLoadTableAlarmMove != TRUE) {
// 					if( bImpactTbl == TRUE && m_nImpactMoveCnt < LOAD_TABLE_IMPACT_COUNT ) { // Impact Table
// 						m_nStep = _eSeqStep_Impact_Move_CurPos;
// 						break;
// 					}

					if (g_DMCont.m_dmHandlerOpt.GN(NDM11_LOADING_TABLE_VIBRATOR_OPTION) == DEF_ON)
					{
						int nVibRetryCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_LOADING_TABLE_VIBRATOR_RETRY_CNT);
						if (m_nVibCurTryCnt >= nVibRetryCnt)   // Vibrator를 Retry 횟수만큼 완료
						{
							m_nVibCurTryCnt = 0;
						}
						else     //Vibrator를 Retry 시작
						{
							m_nBackVibratorStep = m_nStep;
							m_nStep = do_Vibrator_On; // Vibrator 동작 Case								
							break;
						}
					}


					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( "Load Table arrived safety sensor is detected!! [Load table Number=%d], %s", m_nTableIdx + 1, strErr );
					if ((int)vErrHistory.size() > 0) {
						for (int i = 0; i < (int)vErrHistory.size(); i++) {
							AlarmData.m_fnAddItemFormat(vErrHistory[i]);
						}
					}

					ReportAlarm(ERR_LOAD_TBL_1_ARRIVE_SAFETY_SENSOR + m_nTableIdx, AlarmData, vFailPocket );
					break;
				}
				pTaskLoadTbl->MakeLog( "[%s] [EXEC=%8d] [Arrive Sensor Check](END)", m_szCmdName, m_nStep );
				m_nStep = 12100;
			}break;

	/////////////////////////////////////
		case 12100: //Erase 2D Picture && LED ON
		{
			if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use) != TRUE || pTaskLoadTbl->GetTableUsage() == eUnloadType ||
				g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK) != eDeviceMode || g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
			{
				m_nStep = 13000; //Move Y
				break;
			}

			//2D Barcode Light On
			int nBarcodeIdx = m_nTableIdx / 2;
			if (nBarcodeIdx == eTestPP_1) {
				g_pVisionTestHand_1->Cmd_ReqMeasure_BarcodeLED(eBarcodeLight_ON, nBarcodeIdx);
			}
			else {
				g_pVisionTestHand_2->Cmd_ReqMeasure_BarcodeLED(eBarcodeLight_ON, nBarcodeIdx);
			}
			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Barcode LED ON][Barcode Index=%d](STR)", m_szCmdName, m_nStep, nBarcodeIdx);

			// 광학 기술팀 권의혁 전임 요청으로 Barcode FTP 이미지 삭제 기능 제거 [5/8/2025 donghyun.shin]
// 			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Erase 2D Picture](STR)", m_szCmdName, m_nStep);
// 			if(g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
// 				g_pBarcode->Erase2dPicture(m_nTableIdx);
// 
// 			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Erase 2D Picture](END)", m_szCmdName, m_nStep);
			m_nStep = 13000;
		}break;
		case 13000: //move load side
		{
			//Loading Table이  Test Site에 존재할 경우, Rotate Test 방향 Move 
			std::vector<int> vLocList;
			pTaskLoadTbl->WhereIsMotorPos(vLocList, CTaskLoadTable::eAxisAll);
			if (pTaskLoadTbl->chk_rotate_direction(CTaskLoadTable::eDir_TestSite) != ERR_NO_ERROR && vLocList[CTaskLoadTable::eAxisY] > CTaskLoadTable::eYPOS_LoadSide && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
			{
				//Fram 기둥 영역 안에 있을 때, Rotate 안되도록 Block
				//Front Stacker 충돌 위치에 있을때, Rotate 안되도록 Block
				OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
				double dCurPosY = 0.0;
				pMotor->GetCurrentPos(dCurPosY);
				if ((INTERLOCK_LOADING_TABLE_Y_FRAM_IMPACT_LOC_START <= dCurPosY && dCurPosY <= INTERLOCK_LOADING_TABLE_Y_FRAM_IMPACT_LOC_END)
					|| dCurPosY <= INTERLCOK_LOADING_TABLE_Y_STACKER_IMPACT_LOC)
				{
					m_nStep = 13002;
					break;
				}

				pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Rotate to %s Pos](STR)", m_szCmdName, m_nStep, pTaskLoadTbl->GetDirName(m_nCmdStage));
				int nErr = pTaskLoadTbl->do_MovePos(m_nCmdStage + 1, CTaskLoadTable::eAxisRotate);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Load Table %d, move rotate %s command error", m_nTableIdx + 1, pTaskLoadTbl->GetDirName(m_nCmdStage));
					OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisRotate);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
				m_tcTimeout.SetCheckTime((MOTOR_TIME_OUT_MIDDLE));
				m_tcTimeout.StartTimer();
				m_nStep++;
				break;
			}
			else
			{
				m_nStep = 13002;
				break;
			}
		}break;
		case 13001:
			{
				OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisRotate);
				int nAxisDone = pMotor->AxisDone();
				if (nAxisDone == ERR_NO_ERROR) {
					//Rotate Sensor 확인.
 					if (pTaskLoadTbl->chk_rotate_direction(CTaskLoadTable::eDir_TestSite) != ERR_NO_ERROR && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_) {
 						CParamList AlarmData;
 						AlarmData.m_fnAddItemFormat("Load Table %d, rotate direction state error, state : %s ", m_nTableIdx + 1, pTaskLoadTbl->GetDirName(m_nCmdStage));
 						ReportAlarm(ERR_LOAD_TBL_ROTATE_STATE_TEST_SITE_FAIL, AlarmData);
 						break;
 					}

					pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Rotate to %s Pos](END)", m_szCmdName, m_nStep, pTaskLoadTbl->GetDirName(m_nCmdStage));
					m_nStep++;
				}
				else
				{
					if (m_tcTimeout.CheckOverTime() == TRUE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
						ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
						break;
					}

					if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
						ReportAlarm(nAxisDone, AlarmData);
					}
				}


		}break;
		////////////////////////////////
		// Unload Table만 해당.
		////////////////////////////////
		case 13002: //Pre Move Y
			{
// 				std::vector<int> vLocList;
// 				pTaskLoadTbl->WhereIsMotorPos(vLocList, CTaskLoadTable::eAxisAll);
//  				if (pTaskLoadTbl->chk_rotate_direction(CTaskLoadTable::eDir_TestSite) != ERR_NO_ERROR &&vLocList[CTaskLoadTable::eAxisY] != CTaskLoadTable::eYPOS_NegaToLoadSide && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_) {
//  					CParamList AlarmData;
//  					AlarmData.m_fnAddItemFormat("Load Table %d, rotate direction state error, state : %s ", m_nTableIdx + 1, pTaskLoadTbl->GetDirName(m_nCmdStage));
//  					ReportAlarm(ERR_LOAD_TBL_ROTATE_STATE_TEST_SITE_FAIL, AlarmData);
//  					break;
//  				}

				//Table Load Type일 경우, Move Y Load Position
				if (pTaskLoadTbl->GetTableUsage() == eLoadType)
				{
					m_nStep = 13100; //Move Y
					break;
   				}

				int nMotorPos = pTaskLoadTbl->ChkMotorPos(LOC_IDX_TBL_LD_SIDE, CTaskLoadTable::eAxisAll);
				if (nMotorPos == ERR_NO_ERROR) {
					m_nStep = 13100; //Move Y
					break;
				}

				int nEQPNo = g_DMCont.m_dmEQP.GN(NDM0_Handler_EQP_No);
				if (pTaskLoadTbl->GetTableUsage() == eUnloadType && nEQPNo > DEF_EQUIPMENT_UNIT1)
				{
					//Pre Move Y Pos
					pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Move to %s  Pre Pos](STR)", m_szCmdName, m_nStep, pTaskLoadTbl->GetLocName(m_nCmdStage));
					double dPreOffset = 0.0;
					int nAxisNo[CTaskLoadTable::eMaxAxisCount] = { CTaskLoadTable::eMotor::eAxisY ,CTaskLoadTable::eMotor::eAxisRotate };
					for (int i = 0; i < CTaskLoadTable::eMaxAxisCount; i++) {
						if (i == CTaskLoadTable::eAxisY) {
							dPreOffset = LOAD_TABLE_MOVE_OFFSET_Y;
						}
						else {
							dPreOffset = 0.0;
						}
						int nErr = pTaskLoadTbl->do_MovePos(m_nCmdStage, i, 100, dPreOffset);
						if (nErr != ERR_NO_ERROR) {
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("Load Table %d, move %s command error", m_nTableIdx + 1, pTaskLoadTbl->GetLocName(m_nCmdStage));
							OneAxis* pMotor = pTaskLoadTbl->GetAxis((CTaskLoadTable::eMotor)nAxisNo[i]);
							AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
							ReportAlarm(nErr, AlarmData);
							break;
						}
					}
					m_nStep = 13005; //Pre Move End & Move Rotate Done;
				}
				else
				{
					//Pre Move Y Pos
					pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Move to %s  Pre Pos](STR)", m_szCmdName, m_nStep, pTaskLoadTbl->GetLocName(m_nCmdStage));
					int nErr = pTaskLoadTbl->do_MovePos(LOC_IDX_TBL_LD_SIDE, CTaskLoadTable::eAxisY, 100, LOAD_TABLE_MOVE_OFFSET_Y);
					if (nErr != ERR_NO_ERROR) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Load Table %d, pre move %s command error", m_nTableIdx + 1, pTaskLoadTbl->GetLocName(m_nCmdStage));
						OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(nErr, AlarmData);
						break;
					}
					//m_tcTimeout.SetCheckTime((MOTOR_TIME_OUT_MIDDLE / 3.0));
					//m_tcTimeout.StartTimer();
					m_nStep++; //Move Done Check
				}
				m_tcTimeout.SetCheckTime((MOTOR_TIME_OUT_MIDDLE / 3.0));
				m_tcTimeout.StartTimer();
			}break;
		case 13003: //Pre Move Y Done
		{
			OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Move to %s Pre Pos](END)", m_szCmdName, m_nStep, pTaskLoadTbl->GetLocName(m_nCmdStage));
				m_nStep++; //15000;
			}
			else {
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_LOAD_TBL_1_MOVE_FAIL + m_nTableIdx, AlarmData);
				}
			}
		}break;
		case 13004: //Move Rotate Load Side
		{
			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Rotate to %s Pos](STR)", m_szCmdName, m_nStep, pTaskLoadTbl->GetDirName(m_nCmdStage));
			int nErr = pTaskLoadTbl->do_MovePos(m_nCmdStage, CTaskLoadTable::eAxisRotate);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Load Table %d, move rotate %s command error", m_nTableIdx + 1, pTaskLoadTbl->GetDirName(m_nCmdStage));
				OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisRotate);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime((MOTOR_TIME_OUT_SHORT));
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
		case 13005: //Pre Move End & Move Rotate Done
		{
			int nAxisCount = CTaskLoadTable::eMaxAxisCount;
			int nCompChk = 0;
			for (int i = 0; i < nAxisCount; i++) {
				OneAxis* pMotor = pTaskLoadTbl->GetAxis(i);
				if (pMotor == NULL)
				{
					nCompChk++;
					continue;
				}
				int nAxisDone = pMotor->AxisDone();
				if (nAxisDone == ERR_NO_ERROR) {
					nCompChk++;
				}
				else {
					if (m_tcTimeout.CheckOverTime() == TRUE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
						AlarmData.m_fnAddItemFormat("%s Axis", pMotor->GetAxisName());
						ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
						break;
					}

					if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("%s Axis", pMotor->GetAxisName());
						ReportAlarm(nAxisDone, AlarmData);
					}
				}
			}
			if (nCompChk == nAxisCount) {
				//Rotate Sensor 확인.
				if (pTaskLoadTbl->chk_rotate_direction(CTaskLoadTable::eDir_LoadSite) != ERR_NO_ERROR && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Load Table %d, rotate direction state error, state : %s ", m_nTableIdx + 1, pTaskLoadTbl->GetDirName(m_nCmdStage));
					ReportAlarm(ERR_LOAD_TBL_ROTATE_STATE_TRAY_SITE_FAIL, AlarmData);
					break;
				}

				pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Rotate to %s Pos](END)", m_szCmdName, m_nStep, pTaskLoadTbl->GetDirName(m_nCmdStage));
				m_nStep = 13100;
			}
		}break;
		case 13100: //Move Y Target Pos.
		{
			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Move to %s Pos](STR)", m_szCmdName, m_nStep, pTaskLoadTbl->GetLocName(m_nCmdStage));

//			nErr = pTaskLoadTbl->do_MovePos(m_nCmdStage, CTaskLoadTable::eAxisY);
			int nErr = ERR_NO_ERROR;
			if (pTaskLoadTbl->GetTableUsage() == eLoadType) {
				int nAxisNo[CTaskLoadTable::eMaxAxisCount] = { CTaskLoadTable::eMotor::eAxisY ,CTaskLoadTable::eMotor::eAxisRotate };
				for (int i = 0; i < CTaskLoadTable::eMaxAxisCount; i++) {
					nErr = pTaskLoadTbl->do_MovePos(m_nCmdStage, i, 100, 0.0, m_nMoveStep);
					if (nErr != ERR_NO_ERROR) {
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Load Table %d, move %s command error", m_nTableIdx + 1, pTaskLoadTbl->GetLocName(m_nCmdStage));
						OneAxis* pMotor = pTaskLoadTbl->GetAxis((CTaskLoadTable::eMotor)nAxisNo[i]);
						AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
						ReportAlarm(nErr, AlarmData);
						break;
					}
				}
				m_nStep = 15001;
			}
			else {
				nErr = pTaskLoadTbl->do_MovePos(m_nCmdStage, CTaskLoadTable::eAxisY);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Load Table %d, move %s command error", m_nTableIdx + 1, pTaskLoadTbl->GetLocName(m_nCmdStage));
					OneAxis* pMotor = pTaskLoadTbl->GetAxis((CTaskLoadTable::eMotor)eAxisY);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
				m_nStep++;
			}

			m_tcTimeout.SetCheckTime((MOTOR_TIME_OUT_MIDDLE / 3.0));
			m_tcTimeout.StartTimer();
//			m_nStep++;
		}break;
		case 13101: //Move Y Done Check
		{
			OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				//int nLoadTblTestSitePos = pTaskLoadTbl->ChkMotorPos(LOC_IDX_TBL_LD_SIDE, CTaskLoadTable::eAxisY);
				pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Move to %s Pos](END)", m_szCmdName, m_nStep, pTaskLoadTbl->GetLocName(m_nCmdStage));
				m_nStep = 15000; //15000;
			}
			else {
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_LOAD_TBL_1_MOVE_FAIL + m_nTableIdx, AlarmData);
				}
			}
		}break;
		case 15000: //Move Rotate Load Side
		{
			//Rotate Sensor 확인.
			//이미 Rotate 방향 Load Side라면 Step PASS
			if (pTaskLoadTbl->chk_rotate_direction(CTaskLoadTable::eDir_LoadSite) == ERR_NO_ERROR && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_) {
				m_nStep = 16000;
				break;
			}

			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Rotate to %s Pos](STR)", m_szCmdName, m_nStep, pTaskLoadTbl->GetDirName(m_nCmdStage));
			int nErr = pTaskLoadTbl->do_MovePos(m_nCmdStage, CTaskLoadTable::eAxisRotate);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Load Table %d, move rotate %s command error", m_nTableIdx + 1, pTaskLoadTbl->GetDirName(m_nCmdStage));
				OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisRotate);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime((MOTOR_TIME_OUT_MIDDLE));
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
		case 15001: //Move Y & Rotate Done
		{
			int nAxisCount = CTaskLoadTable::eMaxAxisCount;
			int nCompChk = 0;
			for (int i = 0; i < nAxisCount; i++) {
				OneAxis* pMotor = pTaskLoadTbl->GetAxis(i);
				if (pMotor == NULL)
				{
					nCompChk++;
					continue;
				}
				int nAxisDone = pMotor->AxisDone();
				if (nAxisDone == ERR_NO_ERROR) {
					nCompChk++;
				}
				else {
					if (m_tcTimeout.CheckOverTime() == TRUE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
						ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
						break;
					}

					if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
						ReportAlarm(nAxisDone, AlarmData);
					}
				}
			}
			if (nCompChk == nAxisCount) {
				//Rotate Sensor 확인.
				if (pTaskLoadTbl->chk_rotate_direction(CTaskLoadTable::eDir_LoadSite) != ERR_NO_ERROR && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Load Table %d, rotate direction state error, state : %s ", m_nTableIdx + 1, pTaskLoadTbl->GetDirName(m_nCmdStage));
					ReportAlarm(ERR_LOAD_TBL_ROTATE_STATE_TRAY_SITE_FAIL, AlarmData);
					break;
				}

				if (pTaskLoadTbl->GetTableUsage() == eLoadType) {
					pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Move to %s Pos](END)", m_szCmdName, m_nStep, pTaskLoadTbl->GetLocName(m_nCmdStage));
				}
				pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Rotate to %s Pos](END)", m_szCmdName, m_nStep, pTaskLoadTbl->GetDirName(m_nCmdStage));
				m_nStep = 16000;
			}
		}break;
		case 16000:
			{
				pTaskLoadTbl->MakeLog( "[%s] [EXEC=%8d] [Arrive Sensor Check](STR)", m_szCmdName, m_nStep );
				CString strErr = _T("");
				std::vector<CPoint> vFailPocket;
				std::vector<CString> vErrHistory;

				BOOL bRetErr = ChkErrArrivedSen(strErr,vFailPocket,vErrHistory);
				if( bRetErr != ERR_NO_ERROR && pTaskLoadTbl->m_bLoadTableAlarmMove != TRUE) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( "Load Table arrived safety sensor is detected!! [Load table Number=%d], %s", m_nTableIdx + 1, strErr );
					
					if (g_DMCont.m_dmHandlerOpt.GN(NDM11_LOADING_TABLE_VIBRATOR_OPTION) == DEF_ON)
					{
						int nVibRetryCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_LOADING_TABLE_VIBRATOR_RETRY_CNT);
						if (m_nVibCurTryCnt >= nVibRetryCnt)   // Vibrator를 Retry 횟수만큼 완료
						{
							m_nVibCurTryCnt = 0;
						}
						else     //Vibrator를 Retry 시작
						{
							m_nBackVibratorStep = m_nStep;
							m_nStep = do_Vibrator_On; // Vibrator 동작 Case								
							break;
						}
					}

					if ((int)vErrHistory.size() > 0) {
						for (int i = 0; i < (int)vErrHistory.size(); i++) {
							AlarmData.m_fnAddItemFormat(vErrHistory[i]);
						}
					}

					ReportAlarm(ERR_LOAD_TBL_1_ARRIVE_SAFETY_SENSOR + m_nTableIdx, AlarmData, vFailPocket );
					break;
				}
				pTaskLoadTbl->MakeLog( "[%s] [EXEC=%8d] [Arrive Sensor Check](END)", m_szCmdName, m_nStep );
				m_nStep = 40000;
			}break;

		//=======================================================================
		// move to test site
		//=======================================================================
		case 20000: //Pre Move Y
		{
			if (pTaskLoadTbl->GetTableUsage() != eUnloadType) {
				m_nStep = 20100;
				break;
			}

			std::vector<int> vLocList;
			pTaskLoadTbl->WhereIsMotorPos(vLocList, CTaskLoadTable::eAxisAll);
			
			if (vLocList[CTaskLoadTable::eAxisY] > CTaskLoadTable::eYPOS_LoadSideToSitePos1)
			{
				m_nStep = 20100;
				break;
			}

			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Move to %s Pre Pos](STR)", m_szCmdName, m_nStep, pTaskLoadTbl->GetLocName(m_nCmdStage));
			int nErr = pTaskLoadTbl->do_MovePos(LOC_IDX_TBL_LD_SIDE, CTaskLoadTable::eAxisY, 100, LOAD_TABLE_MOVE_OFFSET_Y);
			if (nErr != ERR_NO_ERROR)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Loading Table = %d, can not move %s.", m_nTableIdx + 1, pTaskLoadTbl->GetLocName(m_nCmdStage));
				OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			
			m_tcTimeout.SetCheckTime((MOTOR_TIME_OUT_MIDDLE / 3.0));
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
		case 20001: //Pre Move Y Done 
		{
			int nAxisCount = CTaskLoadTable::eMaxAxisCount;
			int nCompChk = 0;
			for (int i = 0; i < nAxisCount; i++) {
				OneAxis* pMotor = pTaskLoadTbl->GetAxis(i);
				if (pMotor == NULL)
				{
					nCompChk++;
					continue;
				}
				int nAxisDone = pMotor->AxisDone();
				if (nAxisDone == ERR_NO_ERROR) {
					nCompChk++;
				}
				else {
					if (m_tcTimeout.CheckOverTime() == TRUE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
						ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
						break;
					}

					if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
						ReportAlarm(nAxisDone, AlarmData);
					}
				}
			}
			if (nCompChk == nAxisCount) {
				pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Move to %s Pre Pos](END)", m_szCmdName, m_nStep, pTaskLoadTbl->GetLocName(m_nCmdStage));
				m_nStep = 20100;
			}
		}break;
		case 20100: // Impact 기능 체크
		{			
			// Load Type Table만 Impact 기능 가능함
			if (pTaskLoadTbl->GetTableUsage() == eUnloadType)
			{
				int nEQPNo = g_DMCont.m_dmEQP.GN(NDM0_Handler_EQP_No);
				if (nEQPNo > DEF_EQUIPMENT_UNIT1){
					m_nStep = 20130; //Move Y & Rotate
				}
				else {
					m_nStep = 20150;
				}
				break;
			}

			// Imapact 진행 후, AfterMove Check 하도록 변경 [8/1/2025 dohyeong.kim]
// 			bool bArrivedSen = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_LoadTblArrived);
// 			bool bIsAfterMoveChk = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_LoadTbl_AfterMove_Arrived);
// 			if (bArrivedSen && bIsAfterMoveChk) {
// 				pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [After Move Check]", m_szCmdName, m_nStep);
// 				m_nStep = 20150;
// 				break;
// 			}


			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Arrive Sensor Check](STR)", m_szCmdName, m_nStep);

			CString strErr = _T("");
			std::vector<CPoint> vFailPocket;
			std::vector<CString> vErrHistory;

			BOOL bRetErr = ChkErrArrivedSen(strErr, vFailPocket, vErrHistory);
			BOOL bImpactTbl = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_ImpactTbl);

			if (bRetErr != ERR_NO_ERROR)
			{
				if (bImpactTbl == TRUE && m_nImpactMoveCnt < LOAD_TABLE_IMPACT_COUNT) { // Impact Table
					m_nStep = 20110;
					break;
				}

				BOOL bArrivedSen = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_LoadTblArrived);
				BOOL bIsAfterMoveChk = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_LoadTbl_AfterMove_Arrived);
				BOOL bVibratorOnOff = g_DMCont.m_dmHandlerOpt.GN(NDM11_LOADING_TABLE_VIBRATOR_OPTION);
				if (bArrivedSen && bIsAfterMoveChk && bVibratorOnOff != DEF_ON) {
					pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [After Move Check]", m_szCmdName, m_nStep);
					m_nStep = 20150;
					break;
				}

				if (g_DMCont.m_dmHandlerOpt.GN(NDM11_LOADING_TABLE_VIBRATOR_OPTION) == DEF_ON)
				{
					int nVibRetryCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_LOADING_TABLE_VIBRATOR_RETRY_CNT);
					if (m_nVibCurTryCnt >= nVibRetryCnt)   // Vibrator를 Retry 횟수만큼 완료
					{
						m_nVibCurTryCnt = 0;
					}
					else     //Vibrator를 Retry 시작
					{
						m_nBackVibratorStep = m_nStep;
						m_nStep = do_Vibrator_On; // Vibrator 동작 Case								
						break;
					}
				}


				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Load Table arrived safety sensor is detected!! [Load table Number=%d], %s", m_nTableIdx + 1, strErr);

				if ((int)vErrHistory.size() > 0) {
					for (int i = 0; i < (int)vErrHistory.size(); i++) {
						AlarmData.m_fnAddItemFormat(vErrHistory[i]);
					}
				}
				
				ReportAlarm(ERR_LOAD_TBL_1_ARRIVE_SAFETY_SENSOR + m_nTableIdx, AlarmData, vFailPocket);
				break;
			}
			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Arrive Sensor Check](END)", m_szCmdName, m_nStep);
			m_nStep = 20150;
		}break;
		case 20110: //Impact Begin
		{
			int nCmdStage = -1;
			if (m_nCmdStage >= LOC_IDX_LD_TBL_TEST_SIDE_1)
				nCmdStage = LOC_IDX_TBL_LD_SIDE;

			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Move to %s Pos](STR)", m_szCmdName, m_nStep, pTaskLoadTbl->GetLocName(nCmdStage));
			// move start org position.
			int nErr = pTaskLoadTbl->do_MovePos(nCmdStage, CTaskLoadTable::eAxisY);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_LONG);
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
		case 20111:
		{
			OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Move to Pos](END)", m_szCmdName, m_nStep);
				if (m_nImpactMoveCnt >= LOAD_TABLE_IMPACT_COUNT) { // nomal seq
					m_nStep = 20100;
				}
				else {					  //checking impact loading tbl
					m_nStep++;
				}
			}
			else
			{
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(nAxisDone, AlarmData);
				}
			}
		}break;
		case 20112:
		{
			int nCmdStage = -1;
			double dOffsetPos = 0.0;
			if (m_nCmdStage >= LOC_IDX_LD_TBL_TEST_SIDE_1) {
				nCmdStage = LOC_IDX_TBL_LD_SIDE;
				dOffsetPos = ((LOAD_TABLE_IMPACT_DIST) + LOAD_TABLE_IMPACT_MOVE_DIST);
			}
				
			if (m_nImpactMoveCnt >= LOAD_TABLE_IMPACT_COUNT) {
				m_nStep = 20000;
				break;
			}

			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Move to %s Pos](STR)", m_szCmdName, m_nStep, pTaskLoadTbl->GetLocName(nCmdStage));
			// move start org position.
			int nErr = pTaskLoadTbl->do_MovePos(nCmdStage, CTaskLoadTable::eAxisY, 100, dOffsetPos);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_LONG);
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
		case 20113:
		{
			OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
			if (m_tcTimeout.CheckOverTime() == TRUE)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
				AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
				ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
				break;
			}

	
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				m_nImpactMoveCnt++;
				m_nStep++;
			}
			else
			{
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(nAxisDone, AlarmData);
				}
			}
		}break;
		case 20114:
		{
			int nCmdStage = -1;
			double dOffsetPos = 0.0;
			if (m_nCmdStage >= LOC_IDX_LD_TBL_TEST_SIDE_1){
				nCmdStage = LOC_IDX_TBL_LD_SIDE;
				dOffsetPos = (LOAD_TABLE_IMPACT_MOVE_DIST);
			}

			// move start org position.
			int nErr = pTaskLoadTbl->do_MovePos(nCmdStage, CTaskLoadTable::eAxisY, 100, dOffsetPos);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_LONG);
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
		case 20115:
		{
			OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
			if (m_tcTimeout.CheckOverTime() == TRUE)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
				AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
				ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
				break;
			}

		
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				m_nStep = 20112;
			}
			else
			{
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(nAxisDone, AlarmData);
				}
			}
		}break;
		//////////////////////////////////////// Impact Table end

		case 20130: //2호기 & Unload Table만 해당.
		{
			//Table Move Step 추가 [10/2/2024 dohyeong.kim]
			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Move to %s Pos](STR)", m_szCmdName, m_nStep, pTaskLoadTbl->GetLocName(m_nCmdStage));
			
			int table_col_step = pTaskLoadTbl->GetTableColStep();
			pTaskLoadTbl->MakeLog("[Move Table Step = %d]", table_col_step);
			int nAxisNo[CTaskLoadTable::eMaxAxisCount] = { CTaskLoadTable::eMotor::eAxisY ,CTaskLoadTable::eMotor::eAxisRotate };
			for (int i = 0; i < CTaskLoadTable::eMaxAxisCount; i++) {
				int nErr = pTaskLoadTbl->do_MovePos(m_nCmdStage, i, 100, 0.0, table_col_step);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Load Table %d, move %s command error", m_nTableIdx + 1, pTaskLoadTbl->GetLocName(m_nCmdStage));
					OneAxis* pMotor = pTaskLoadTbl->GetAxis((CTaskLoadTable::eMotor)nAxisNo[i]);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nErr, AlarmData);
					break;
				}
			}

			m_nStep = 25001; // check Axis Y & Rotate
		}break;


		case 20150: //Move Rotate
		{
			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Rotate to %s Pos](STR)", m_szCmdName, m_nStep, pTaskLoadTbl->GetDirName(m_nCmdStage));
			int nErr = pTaskLoadTbl->do_MovePos(m_nCmdStage, CTaskLoadTable::eAxisRotate);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Load Table %d, move rotate %s command error", m_nTableIdx + 1, pTaskLoadTbl->GetDirName(m_nCmdStage));
				OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisRotate);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime((MOTOR_TIME_OUT_MIDDLE / 3.0));
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
		case 20151:
		{		
			OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisRotate);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				//Rotate Sensor 확인.
				if (pTaskLoadTbl->chk_rotate_direction(CTaskLoadTable::eDir_TestSite) != ERR_NO_ERROR && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_) {	
					if (m_tcTimeout.CheckOverTime() == TRUE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Load Table %d, rotate direction state error, state : %s ", m_nTableIdx + 1, pTaskLoadTbl->GetDirName(m_nCmdStage));
						ReportAlarm(ERR_LOAD_TBL_ROTATE_STATE_TEST_SITE_FAIL, AlarmData);
					}				
					break;
				}

				pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Rotate to %s Pos](END)", m_szCmdName, m_nStep, pTaskLoadTbl->GetDirName(m_nCmdStage));
				m_nStep = 20200;
			}
			else 
			{
				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(nAxisDone, AlarmData);
				}
				else
				{
					if (m_tcTimeout.CheckOverTime() == TRUE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
						ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
					}
				}
			}
		}break;
		case 20200: // check arrived sen
			{	
			// Imapact 진행 후, AfterMove Check 하도록 변경 [8/1/2025 dohyeong.kim]
// 				bool bArrivedSen = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_LoadTblArrived);
// 				bool bIsAfterMoveChk = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_LoadTbl_AfterMove_Arrived);
// 				if (bArrivedSen && bIsAfterMoveChk) {
// 					pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [After Move Check]", m_szCmdName, m_nStep);
// 					m_nStep = 21000;
// 					break;
// 				}

				pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Arrive Sensor Check](STR)", m_szCmdName, m_nStep);
				
				CString strErr = _T("");
				std::vector<CPoint> vFailPocket;
				std::vector<CString> vErrHistory;

				BOOL bRetErr = ChkErrArrivedSen(strErr,vFailPocket,vErrHistory);
				BOOL bImpactTbl = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_ImpactTbl);

				if (bRetErr != ERR_NO_ERROR && pTaskLoadTbl->m_bLoadTableAlarmMove != TRUE) 
				{
					if (bImpactTbl == TRUE && m_nImpactMoveCnt < LOAD_TABLE_IMPACT_COUNT) { // Impact Table
						m_nStep = _eSeqStep_Impact_Move_CurPos;
						break;
					}

					BOOL bArrivedSen = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_LoadTblArrived);
					BOOL bIsAfterMoveChk = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_LoadTbl_AfterMove_Arrived);
					BOOL bVibratorOnOff = g_DMCont.m_dmHandlerOpt.GN(NDM11_LOADING_TABLE_VIBRATOR_OPTION);
					if (bArrivedSen && bIsAfterMoveChk && bVibratorOnOff != DEF_ON) {
						pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [After Move Check]", m_szCmdName, m_nStep);
						m_nStep = 21000;
						break;
					}

					if (g_DMCont.m_dmHandlerOpt.GN(NDM11_LOADING_TABLE_VIBRATOR_OPTION) == DEF_ON)
					{
						int nVibRetryCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_LOADING_TABLE_VIBRATOR_RETRY_CNT);
						if (m_nVibCurTryCnt >= nVibRetryCnt)   // Vibrator를 Retry 횟수만큼 완료
						{
							m_nVibCurTryCnt = 0;
						}
						else     //Vibrator를 Retry 시작
						{
							m_nBackVibratorStep = m_nStep;
							m_nStep = do_Vibrator_On; // Vibrator 동작 Case								
							break;
						}
					}

					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Load Table arrived safety sensor is detected!! [Load table Number=%d], %s", m_nTableIdx + 1, strErr);

					if ((int)vErrHistory.size() > 0) {
						for (int i = 0; i < (int)vErrHistory.size(); i++) {
							AlarmData.m_fnAddItemFormat(vErrHistory[i]);
						}
					}

					ReportAlarm(ERR_LOAD_TBL_1_ARRIVE_SAFETY_SENSOR + m_nTableIdx, AlarmData, vFailPocket);
					break;
				}
				pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Arrive Sensor Check](END)", m_szCmdName, m_nStep);
				m_nStep = 21000;

			}break;

		//Trigger Initial, 2d Barcode 정보 check.
		case 21000:
			{	
				pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D Initialize](STR)", m_szCmdName, m_nStep);
				//Unload Table은 2D Check 안함.

				int nChkTableType = pTaskLoadTbl->GetTableUsage();
				std::vector<int> vLocList;
				vLocList.clear();
				pTaskLoadTbl->WhereIsMotorPos(vLocList, CTaskLoadTable::eAxisAll);
				if ((nChkTableType == eLoadType && (pTaskLoadTbl->m_bLoadTableSkip == TRUE || pTaskLoadTbl->m_bLoadTableAlarmMove == TRUE)
					&& CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto)
					&& g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
				{
//					m_v2DErrorData.clear(); //Alarm complete 완료 후, clear
					if (pTaskLoadTbl->m_bLoadTableSkip == TRUE && g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use) == TRUE) {
						pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D Barcode User Select Skip ]", m_szCmdName, m_nStep);
						pTaskLoadTbl->m_bLoadTableSkip = FALSE;
						m_nStep = 26500;
					}
					else {
						pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D Barcode User Select Axis Move ][LOC = %s]", m_szCmdName, m_nStep, pTaskLoadTbl->GetLocName(m_nCmdStage));
						m_nStep = 29000; //Y Move
					}
					//m_nStep = 29000;
					break;
				}

				
				//Retry Count 0이 아니면 2d Position으로 이동하여 재촬영 해야 함.
				if (vLocList[CTaskLoadTable::eAxisY] > CTaskLoadTable::eYPOS_LoadSideToSitePos1 && 
					(m_nCmdStage >= LOC_IDX_LD_TBL_TEST_SIDE_1 && m_nCmdStage <= LOC_IDX_LD_TBL_TEST_SIDE_4)
					&& m_nRetryCount == 0)
				{
					pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D Barcode Pos Skip]", m_szCmdName, m_nStep);
					m_nStep = 29100; //do_MovePos
					break;
				}

				if( g_DMCont.m_dmHandlerOpt.GB( BDM11_MD_2D_BarCode_Use ) != TRUE || nChkTableType == eUnloadType) {
					m_nStep = 25000;
					break;
				}
				else {
					//Trigger Initial
					//pTaskLoadTbl->InitialTrigger(m_nTableIdx);
/*					g_trigger_generator.SetStartTrigger(m_nTableIdx);*/


					//Trigger Initial
					//g_trigger_generator.SetStartTrigger(m_nTableIdx);

					int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN( NDM11_SYS_MD_DEVICE_CHK );
					int nReserved = pTaskLoadTbl->GetWorkReserve();
					BOOL bNotScanDevice = FALSE;
					//int nProjectOption = g_DMCont.m_dmEQP.GN(NDM11_Handler_Project_Name);
					int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
					if( (nDeviceMode == eNoDeviceTrayExist || nDeviceMode == eDeviceMode || g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) && CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto )
					{			
						// load pocket에서 디바이스 존재하는데, 바코드 정보 없는 디바이스가 있는지 확인한다.
						for( int y = 0; y < MAX_LD_TBL_POCKET_CNT_Y; y++ )
						{
							if (nBoardType == eBoardDir_Forward) {
								if (y == 1 || y == 3) continue;						
							}
							else {
								if (y == 0 || y == 2) continue;
							}
							
							for( int x = 0; x < MAX_LD_TBL_POCKET_CNT_X; x++ )
							{
								ST_DD_DEVICE tDeviceData;
								g_DMCont.m_dmDevice.GetDeviceData( eDeviceStage_LD_TBL_1 + m_nTableIdx, x, y, &tDeviceData );
								if( tDeviceData.sExist == eDvcExist && strcmp( tDeviceData.sz2DBarcode, "" ) == 0 ) {
									bNotScanDevice = TRUE;
								}
							}
						}
					}


					//기존 방식 eYPOS_LoadSideToSitePos1 초과 영역 && Table Device 2d Data 있을 경우 Pass.
					//추가 방식 eYPOS_LoadSideToSitePos1 이상 영역 && TblColStep이 0이 아닐 경우 Pass
					// -> Station 1영역에서 Table Step Move 했을 경우 eYPOS_LoadSideToSitePos1 영역 안에 들어오기 때문에 예외처리 진행.
					if( g_DMCont.m_dmHandlerOpt.GB( BDM11_MD_2D_BarCode_Use ) == TRUE && CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto
						&& (nDeviceMode == eDeviceMode || nDeviceMode == eNoDeviceTrayExist) && m_nCmdStage >= LOC_IDX_LD_TBL_TEST_SIDE_1 && m_nCmdStage <= LOC_IDX_LD_TBL_TEST_SIDE_4  /*nReserved == RID_TEST_PP_( (m_nTableIdx / 2)*/
						&& ((vLocList[CTaskLoadTable::eAxisY] > CTaskLoadTable::eYPOS_LoadSideToSitePos1) 
							|| ((vLocList[CTaskLoadTable::eAxisY] >= CTaskLoadTable::eYPOS_LoadSideToSitePos1) && (pTaskLoadTbl->GetTableColStep() != 0)))
						&& (pTaskLoadTbl->GetTableColStep() != 0)
						&& bNotScanDevice == FALSE 
						&& m_nRetryCount == 0)
					{
						// not checking
						nReserved = RID_TEST_PP_((m_nTableIdx / 2));
						m_nStep = 29000;
					}
					else 
					{
						// move 2d barcode position
						g_trigger_generator.SetTriggerPos(m_nTableIdx);
						pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D Initialize SetTriggerPos](STR)", m_szCmdName, m_nStep);
						//Buffer Clear.
						int nErr = ERR_NO_ERROR;
						for (int i = 0; i < 2; i++) {
							int nIdx = (m_nTableIdx / 2 == 0) ? VISION_CAM_ID_2DBRACODE_1 : VISION_CAM_ID_2DBRACODE_3;
							nErr = g_pBarcode->Cmd_Req2DMeasure(VISION_CAM_ID_2DBRACODE_(i + nIdx));
							if (nErr != ERR_NO_ERROR)
							{
								CParamList AlarmData;
								AlarmData.m_fnAddItemFormat("2D Barcode Loadtable=%d Cam=%d", m_nTableIdx + 1, m_nTableIdx * 2 + 1);
								ReportAlarm(nErr, AlarmData);
								break;
							}
 						}

						m_nStep = 21100;
					}
				}
			}break;
		case 21100: //Cylinder Check
		{
			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D cylinder position check](STR)", m_szCmdName, m_nStep);
			/*int nLoadDvcCnt = pTaskLoadTbl->GetLoadDeviceCnt();*/
			int nLoadDvcCnt = 4; //임시
			m_nStep = 22000;
			break;
			////////////////////////////
			if (nLoadDvcCnt > 0) {
				int nErr = ((m_nTableIdx / 2) == 0 ) ? pTaskLoadTbl->chk_cyl_2DBarcode_LeftRight(DEF_RIGHT) : pTaskLoadTbl->chk_cyl_2DBarcode_LeftRight(DEF_LEFT);
				if (nErr != ERR_NO_ERROR) {
					((m_nTableIdx / 2) == 0) ? pTaskLoadTbl->do_cyl_2DBarcode_LeftRight(DEF_RIGHT) : pTaskLoadTbl->do_cyl_2DBarcode_LeftRight(DEF_LEFT);
					m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_SHORT*4.0);
					m_tcTimeout.StartTimer();
					m_nStep++;
				}
				else {
					pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D cylinder position check](END)", m_szCmdName, m_nStep);
					m_nStep = 22000;
				}
			}
			else
			{
				m_nStep = 25000;
			}
		}break;
		case 21101:
		{
			if (m_tcTimeout.CheckOverTime() == TRUE)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Time Over!! 2d barcode cylinder action, LoadTbl=%d", m_nTableIdx + 1);
				ReportAlarm(ERR_LOAD_TBL_1_2D_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
				break;
			}

			int nLoadDvcCnt = pTaskLoadTbl->GetLoadDeviceCnt();
			if (nLoadDvcCnt > 0) {
				int nErr = ((m_nTableIdx / 2) == 0) ? pTaskLoadTbl->chk_cyl_2DBarcode_LeftRight(DEF_RIGHT) : pTaskLoadTbl->chk_cyl_2DBarcode_LeftRight(DEF_LEFT);
				if (nErr == ERR_NO_ERROR) {
					pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D cylinder position check](END)", m_szCmdName, m_nStep);
					m_nStep = 22000;
					break;
				}
			}
			else
			{
				m_nStep = 25000;
			}
		}break;

		case 22000:
		{
			//m_nRetryCount = 0;
			if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use) != TRUE) {
				m_nStep = 25000; break;
			}
			//_tVisionMsg     m_tVisionMsgMeasureResult;
			/*
				nMsgId = src.nMsgId;
				nS = src.nS;
				nF = src.nF;
				nRcmd = src.nRcmd;
				nReceived = src.nReceived;
				nRcmdAck = src.nRcmdAck;
				vReqData = src.vReqData;
			*/
			m_tVisionMsgMeasureResult.clear();

			_tVisionReqData stReqData;
			for (int nMaxPocket = 0; nMaxPocket < m_nPocketCntX*(m_nPocketCntY / 2); nMaxPocket++) {
				m_tVisionMsgMeasureResult.vReqData.push_back(stReqData);
			}

			int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
			if (nDeviceMode == eNoDeviceTrayExist || nDeviceMode == eDeviceMode || g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
			{
				// 완료 되었던 상태에서 rescan 할 경우, 2D device data를 지워야 한다.

				// auto mode 일 때,
				// Table pocket의 device 존재하고, 바코드 정보 있을 때, -> 바코드 정보 없앤다.
				// device가 front쪽에 있는지, rear 쪽에 있는지 확인한다.
				ST_DD_DEVICE tDeviceData;

				//int nProjectOption = g_DMCont.m_dmEQP.GN(NDM11_Handler_Project_Name);
				int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
				for (int y = 0; y < m_nPocketCntY; y++)
				{
					
					if (nBoardType == eBoardDir_Forward) {
						if (y == 1 || y == 3) continue; //Buffer는 확인 안함.				
					}
					else {
						if (y == 0 || y == 2) continue; //Buffer는 확인 안함.
					}
					
					for (int x = 0; x < m_nPocketCntX; x++)
					{
						tDeviceData.clear();
						g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1 + m_nTableIdx, x, y, &tDeviceData);
						if (tDeviceData.sExist == eDvcExist && strcmp(tDeviceData.sz2DBarcode, "") != 0 && CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto /*|| CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Manual*/) {
							pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D Data Delete] [X:%d, Y:%d]", m_szCmdName, m_nStep, x, y);
							g_2DBarcodeChk.DelData(tDeviceData.sz2DBarcode);
						}
					}
				}

			}

			//retry시, Load Side로 이동
			if ( m_nRetryCount != 0 && g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use) == TRUE && pTaskLoadTbl->GetTableUsage() == eLoadType) {
				m_nStep++;
				break;
			}

			m_nStep = 25000;
		}break;
		case 22001:
		{
			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Barcode Retry=%d][Move to %s Pos](STR)", m_szCmdName, m_nStep, m_nRetryCount, pTaskLoadTbl->GetLocName(m_nCmdStage));

			int nErr = ERR_NO_ERROR;
			nErr = pTaskLoadTbl->do_MovePos(LOC_IDX_TBL_LD_SIDE, CTaskLoadTable::eAxisY);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Load Table %d, move %s command error", m_nTableIdx + 1, pTaskLoadTbl->GetLocName(m_nCmdStage));
				OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			
			m_nStep++;

			m_tcTimeout.SetCheckTime((MOTOR_TIME_OUT_MIDDLE / 3.0));
			m_tcTimeout.StartTimer();
		}break;
		case 22002:
		{
			OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Barcode Retry=%d][Move to %s Pos](END)", m_szCmdName, m_nStep, m_nRetryCount, pTaskLoadTbl->GetLocName(m_nCmdStage));
				m_nStep = 25000;
			}
			else {
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_LOAD_TBL_1_MOVE_FAIL + m_nTableIdx, AlarmData);
				}
			}
		}break;
		//////////////////
		//Move Test Site
		//////////////////
		case 25000: // Move Axis Y
		{
			//////////////////////////////////////////////////////////////////////////////////////
			int nCmdStage = 0;

			double dOffsetY = 0.0;
			double dSpdOffset = 100.0;
			int	   table_col_step = 0;
		

			if (pTaskLoadTbl->GetTableUsage() == eLoadType && g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use) == TRUE ) {
 				int n2dSpd[4] = { NDM3_SPD_LoadTbl1_Y , NDM3_SPD_LoadTbl2_Y , NDM3_SPD_LoadTbl3_Y , NDM3_SPD_LoadTbl4_Y };
 				double dRateSpd = (double)g_DMCont.m_dmShuttle.GN(n2dSpd[m_nTableIdx]);
// 				if (dRateSpd > 30) {
// 					//speed 50%에 맞추기 위한 Offset 생성.
// 					dSpdOffset = (30 * 100) / dRateSpd;
// 				}

				int nBarcodeSpdLimit = g_DMCont.m_dmShuttle.GN(NDM3_SPD_Y_Barcode_Limit);
				if (10 > nBarcodeSpdLimit || nBarcodeSpdLimit > 100) //barcode limit이 10 이하 또는 100 이상이면 limit값 10으로 설정
				{
					nBarcodeSpdLimit = 10;
				}

				dSpdOffset = (nBarcodeSpdLimit * 100) / dRateSpd;

				if (dSpdOffset <= 0 || dSpdOffset >= 100.0) //전체 offset Interlock
				{
					dSpdOffset = 100.0;
				}

				//임시 수정.			
				//dSpdOffset = 100.0;


				double d2dPos = pTaskLoadTbl->GetTeachPos(LOC_IDX_LD_TBL_TEST_SIDE_2D, CTaskLoadTable::eAxisY);
				int nLdTblPitchX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Pitch_X);
				dOffsetY = 4 * nLdTblPitchX + 10000.0;
				int n2DTestSiteMove = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_TEST_SITE_MOVE);
				//2D Test Site Move Option이 Check 되어 있으면 2D Read하고 바로 Test Site Traget Cmd Stage로 이동한다.
				if(n2DTestSiteMove == TRUE)
					nCmdStage = m_nCmdStage;
				else
					nCmdStage = LOC_IDX_LD_TBL_TEST_SIDE_2D;
			}
			else {
				nCmdStage = m_nCmdStage;

				// Table Move STep 추가. [10/2/2024 dohyeong.kim]
				// 2D 사용 시, Unlaod Table Move
				// 2D 미사용 시, Load/Unload Table Move
				table_col_step = pTaskLoadTbl->GetTableColStep();
			}


			/////////////////////////////////////////////////////////////////////////////////////

			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Move to %s Pos](STR)", m_szCmdName, m_nStep, pTaskLoadTbl->GetLocName(m_nCmdStage));
			pTaskLoadTbl->MakeLog("[Move Table Step = %d]", table_col_step);
			int nErr = pTaskLoadTbl->do_MovePos(nCmdStage, CTaskLoadTable::eAxisY, dSpdOffset, dOffsetY/*, table_col_step*/);
			if (nErr != ERR_NO_ERROR)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Loading Table = %d, can not move %s.", m_nTableIdx + 1, pTaskLoadTbl->GetLocName(m_nCmdStage));
				OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}

			m_tcTimeout.SetCheckTime((MOTOR_TIME_OUT_MIDDLE / 3.0));
			m_tcTimeout.StartTimer();

			m_nStep++;
		}break;
		case 25001:
		{		
			int nAxisCount = CTaskLoadTable::eMaxAxisCount;
			int nCompChk = 0;
			for (int i = 0; i < nAxisCount; i++) {
				OneAxis* pMotor = pTaskLoadTbl->GetAxis(i);
				if (pMotor == NULL)
				{
					nCompChk++;
					continue;
				}
				int nAxisDone = pMotor->AxisDone();
				if (nAxisDone == ERR_NO_ERROR) {
					nCompChk++;
				}
				else {
					if (m_tcTimeout.CheckOverTime() == TRUE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
						AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
						ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
						break;
					}

					if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
					{
						CParamList AlarmData;
						AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
						ReportAlarm(nAxisDone, AlarmData);
					}
				}
			}
			if (nCompChk == nAxisCount) {
				pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Move to %s Pos][](END)", m_szCmdName, m_nStep, pTaskLoadTbl->GetLocName(m_nCmdStage));

				int nTblType = pTaskLoadTbl->GetTableUsage();
				if((g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use) == TRUE &&
					g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK) == eDeviceMode&&
					LOC_IDX_LD_TBL_TEST_SIDE_1 <= m_nCmdStage && m_nCmdStage <= LOC_IDX_LD_TBL_TEST_SIDE_4
					&& nTblType == eLoadType
					)|| g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
				{
					m_nStep = 26000;
				}
				else {
					m_nStep = 29000;
				}			
			}
		}break;
		case 26000: //2D Check
		{
			int nTblType = pTaskLoadTbl->GetTableUsage();
			int nReserved = pTaskLoadTbl->GetWorkReserve();
			if ((g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use) == TRUE &&
				g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK) == eDeviceMode&&
				LOC_IDX_LD_TBL_TEST_SIDE_1 <= m_nCmdStage && m_nCmdStage <= LOC_IDX_LD_TBL_TEST_SIDE_4 && nTblType == eLoadType) /*|| g_IO.GetDriverType() == _DIO_TYPE_SIMUL_*/)
				/* nReserved != RID_TEST_PP_(m_nTableIdx)*/
				/*&& CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto*/ //Manual 디버깅 용. //bool type 확인으로 변경 해도 되겠다.
			{
				//int nProjectOption = g_DMCont.m_dmEQP.GN(NDM11_Handler_Project_Name);
				int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
				BOOL bDeviceExist = FALSE;
				for (int y = 0; y < m_nPocketCntY; y++)
				{
					if (nBoardType == eBoardDir_Forward) {
						if (y == 1 || y == 3) continue; //Buffer는 확인 안함.		
					}
					else {
						if (y == 0 || y == 2) continue; //Buffer는 확인 안함.
					}
					for (int x = 0; x < m_nPocketCntX; x++)
					{
						ST_DD_DEVICE tDeviceData;
						tDeviceData.clear();
						g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1 + m_nTableIdx, x, y, &tDeviceData);
						if (tDeviceData.sExist == eDvcExist && CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto)
							bDeviceExist = TRUE;
					}
				}

				if (bDeviceExist != TRUE && CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto) {
					m_nStep = 29000;
					break;
				}

				int nCamNo = (m_nTableIdx / 2 == 0) ? VISION_CAM_ID_2DBRACODE_1 : VISION_CAM_ID_2DBRACODE_3;
				int nErr = ERR_NO_ERROR;

				TokenArray str2DErrorData;

				for (int i = 0; i < 2; i++)
				{
					Sleep(200);
					pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D Barcode Request Check][Cam No : %d](STR)", m_szCmdName, m_nStep, nCamNo + i);
					nErr = g_pBarcode->Cmd_Req2DMeasureResult(nCamNo + i, m_nTableIdx, &str2DErrorData, &m_tVisionMsgMeasureResult);

					int nTriggerCountTest = g_trigger_generator.GetTriggerCount(m_nTableIdx / 2);
					if (nErr != ERR_NO_ERROR)
					{
						pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D Barcode Data Request Error]", m_szCmdName, m_nStep);
						for (int i = 0; i < (int)str2DErrorData.size(); i++)
						{
							// 								strcat_s(szTemp, sizeof(szTemp), str2DErrorData[i]);
							// 								strcat_s(szTemp, sizeof(szTemp), "//");
							pTaskLoadTbl->MakeLog("[Data %d : ][%s]", i, str2DErrorData[i]);
						}

						int nTriggerCount = g_trigger_generator.GetTriggerCount(m_nTableIdx / 2);
						pTaskLoadTbl->MakeLog("[Loading Table = %d][Trigger Count : %d]", m_nTableIdx + 1, nTriggerCount);


						int nMaxRetryCnt = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_BarcodeMaxRetry);
						if (m_nRetryCount >= nMaxRetryCnt)
						{
							m_nRetryCount = 0;
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("Loading Table = %d, 2d Barcode RecvData error! [Cam = %d ][Data Count = %d ]", m_nTableIdx + 1, nCamNo + i, str2DErrorData.size());
							ReportAlarm(nErr, AlarmData);
							break;
						}
						else
						{
							g_pBarcode->AddRetryCountCurLot(m_nTableIdx / 2);
							m_nRetryCount++;
							m_tVisionMsgMeasureResult.vReqData.clear();
							pTaskLoadTbl->MakeLog("Loading Table = %d, [Rcv Error]2d Barcode Retry Cnt = %d", m_nTableIdx + 1, m_nRetryCount);
							m_nStep = 20100;
							break;
						}
					}
				}

				if (nErr != ERR_NO_ERROR)
					break;

				pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D Barcode Request Check](End)", m_szCmdName, m_nStep);

				m_nStep = 27000;
				break;
			}

			m_nStep = 29000;
		}break;
		case 26500: //2D Alarm 후 SKIP 진행 시, 나머지 Data 유효성 Check 진행.
		{
			int nDataCnt = 0;
		
			m_tVisionMsgMeasureResult.clear();
			_tVisionReqData stReqData;
			for (int nMaxPocket = 0; nMaxPocket < m_nPocketCntX*(m_nPocketCntY / 2); nMaxPocket++) {
				m_tVisionMsgMeasureResult.vReqData.push_back(stReqData);
			}





			std::vector<CPoint> vTblPocket; vTblPocket.clear();

			for (int nY = 0; nY < MAX_LD_TBL_POCKET_CNT_Y; nY++)
			{
				for (int nX = 0; nX < MAX_LD_TBL_POCKET_CNT_X; nX++)
				{
					char setting = g_TaskLoadTbl[m_nTableIdx].m_LoadTbl_pocket_setting[nY][nX];
					if (setting == 'L') {
						vTblPocket.push_back(CPoint(nX, nY));
					}
//						vTblPocket.push_back(CPoint(nX, nY));
				}
			}

			for (int i = 0; i < (int)vTblPocket.size(); i++)
			{
				ST_DD_DEVICE tDeviceData;
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1 + m_nTableIdx, vTblPocket[i].x, vTblPocket[i].y, &tDeviceData);

				CString str2dData = _T("");
				str2dData.Format("%s", tDeviceData.sz2DBarcode);

				strncpy_s(m_tVisionMsgMeasureResult.vReqData[i].szDataValue, str2dData, strlen(str2dData));

// 				if (tDeviceData.sExist == eDvcExist)
// 				{
// 					//g_pBarcode->ChangePocketNum();
// 					int nIdx = nX + (nY * 4);
// 					//strncpy_s(pVisionMsg->vReqData[nPocketNum].szDataValue, strTokens[i], strlen(strTokens[i]));
// 					m_tVisionMsgMeasureResult.vReqData[nIdx] = tDeviceData.sz2DBarcode;
// 
// 				}
			}

			m_nStep = 27000;

		}break;

		/* 2D Barcode Data Error Check Start
		1. Full Error.			(OK)
		
		3. Is Short Length		(Retry, Skip)
		4. Same Length			(Retry, Skip)


		2. 2D Barcode Mark		(Retry, Skip)


		5. Duplication Check.	(Retry, Skip)
		*/
		case 27000:
		{		
			m_v2DErrorData.clear();

			//여기 까지 Data 8개 들어 왔는지 확인 완료.
			int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
			
			//============================================================================================================ Full Error.
			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D BARCODE DATA VALIDATE CHECK](STR)", m_szCmdName, m_nStep);

			bool bRetFullErr = g_2DBarcodeChk.IsFullError(m_tVisionMsgMeasureResult.vReqData);
			if (bRetFullErr)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Loading Table = %d, The 2D Barcode Date is all ERROR.\n", m_nTableIdx + 1);
				ReportAlarm(ERR_LOAD_TBL_1_VISION_BARCODE_IS_FULL_ERROR + m_nTableIdx, AlarmData);
				break;
			}

			//=========================================================================================================== Is Short Length.
			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D Barcode Check 2d short length ](STR)", m_szCmdName, m_nStep);
			//2d Length is Short
		
			BOOL bRetShortErr = g_2DBarcodeChk.IsShortLen(m_nTableIdx, m_tVisionMsgMeasureResult.vReqData, m_v2DErrorData);
			if (bRetShortErr == TRUE )
			{
				int nMaxRetryCnt = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_BarcodeMaxRetry);
				if (m_nRetryCount >= nMaxRetryCnt)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Loading Table = %d, The 2D Barcode length is short.\n", m_nTableIdx + 1);
					int nPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
					int nPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);
					std::vector<CPoint> vTablePocket; vTablePocket.clear();

					for (int nY = 0; nY < nPocketCntY; nY++)
					{
						for (int nX = 0; nX < nPocketCntX; nX++)
						{
							char setting = g_TaskLoadTbl[m_nTableIdx].m_LoadTbl_pocket_setting[nY][nX];
							if (setting == 'L')
								vTablePocket.push_back(CPoint(nX, nY));
						}
					}

					for (int i = 0; i < (int)vTablePocket.size(); i++)
					{
						AlarmData.m_fnAddItemFormat("[x:%d,y:%d] : %s ", vTablePocket[i].x + 1, vTablePocket[i].y + 1, m_tVisionMsgMeasureResult.vReqData[i].szDataValue);
					}


					std::vector<CPoint> m_v2DErrorAlarm;
					m_v2DErrorAlarm.clear();
					m_v2DErrorAlarm = m_v2DErrorData;
					ReportAlarm(ERR_LOAD_TBL_1_VISION_BARCODE_IS_SHORT + m_nTableIdx, AlarmData, m_v2DErrorAlarm);
					break;
				}
				else
				{
					g_pBarcode->AddRetryCountCurLot(m_nTableIdx / 2);
					m_nRetryCount++;
					m_tVisionMsgMeasureResult.vReqData.clear();
					pTaskLoadTbl->MakeLog("Loading Table = %d, [2d Length Error]2d Barcode Retry Cnt = %d", m_nTableIdx + 1, m_nRetryCount);
					m_nStep = 20100;
					break;
				}
			}
			

			//=========================================================================================================== 2d Barcode Same Length.
			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D Barcode Check 2d Same length ](STR)", m_szCmdName, m_nStep);

			BOOL b2dLengthCheck = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_LENGTH_OPT_USE);
			if (b2dLengthCheck == eOPTION_USE)
			{
				// 2d Same length  
				std::vector<_ST_2DID_SAME_LENGTH> v2DSameLength;
				v2DSameLength.clear();
				BOOL bLength = g_2DBarcodeChk.IsSameLength(m_nTableIdx, m_tVisionMsgMeasureResult.vReqData, m_v2DErrorData, v2DSameLength);
				if ((int)v2DSameLength.size() > 0 && bLength == FALSE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Loading Table = %d, 2d Barcode Lenght error!", m_nTableIdx + 1);
					for (std::vector<_ST_2DID_SAME_LENGTH>::iterator iter = v2DSameLength.begin(); iter != v2DSameLength.end(); iter++) {
						AlarmData.m_fnAddItemFormat("[Pocket [X = %d , Y = %d ] Barcode =[ %s ] Barcode Len = %d", iter->nX + 1, iter->nY + 1, iter->strBarcode, iter->strBarcode.GetLength());
					}

					std::vector<CPoint> vFailPocket;
					vFailPocket.clear();
					vFailPocket = m_v2DErrorData;
					ReportAlarm(ERR_LOAD_TBL_1_VISION_BARCODE_2D_LENGTH + m_nTableIdx, AlarmData, vFailPocket);
					break;
				}
				pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D Barcode Check 2d Same length ](END)", m_szCmdName, m_nStep);
			}

			//=========================================================================================================== 2D Barcode Makr(Left/Right)
			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D Barcode Check 2d Mark](STR)", m_szCmdName, m_nStep);
			m_v2DMarkErrorData.clear();

			char sz2dMarkIgnore[MAX_PATH] = { 0, };
			g_DMCont.m_dmHandlerOpt.GS(SDM11_2D_MARK_IGNORE_REL_TEST, sz2dMarkIgnore, sizeof(sz2dMarkIgnore));
			BOOL b2dMarkIgnore = (strcmp(sz2dMarkIgnore, _DEF_REL_TEST) == 0) ? TRUE : FALSE;

			BOOL b2dMarkCheck = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_MARK_OPT_USE);
			if (b2dMarkCheck == eOPTION_USE && b2dMarkIgnore != TRUE)
			{
				std::vector<std::pair<CPoint, CString>> vError;
				vError.clear();


				BOOL  bRet2dMarkValidate = g_2DBarcodeChk.IsMarkValidate(m_nTableIdx, m_tVisionMsgMeasureResult.vReqData, vError);
				if (bRet2dMarkValidate != TRUE) //Auto Retry 없음.
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Loading Table = %d, The 2D Barcode Mark is invalidate", m_nTableIdx + 1);


					//Device Type 및 Mart List 표기
					char szDeviceName[MAX_PATH] = { 0, };
					char szMarkList[MAX_PATH] = { 0, };

					g_DMCont.m_dmHandlerOpt.GS(SDM11_2D_DEVICE_NAME, szDeviceName, sizeof(szDeviceName));
					g_DMCont.m_dmHandlerOpt.GS(SDM11_2D_MARK_LIST, szMarkList, sizeof(szMarkList));

					//str2dMark.Format("%s", szMark);
					//Mark Type 표기.
					AlarmData.m_fnAddItemFormat("[Device Name : %s]", szDeviceName);

					AlarmData.m_fnAddItemFormat("[MARK List : %s]", szMarkList);

					int n2dFullLength = g_DMCont.m_dmHandlerOpt.GN(NDM11_LOADING_TABLE_2DID_LENGTH);
					int nStartIdx = g_DMCont.m_dmHandlerOpt.GN(NDM11_2D_MARK_START_INDEX);
					AlarmData.m_fnAddItemFormat("[PKG Length : %d][Start Idx : %d]", n2dFullLength, nStartIdx);

					//Data 표기.
					for (int i = 0; i < (int)vError.size(); i++) {
						//first : CPoint
						//second : Error String
						//m_v2DErrorData.push_back(vError[i].first); //Error Data Save
						m_v2DMarkErrorData.push_back(vError[i].first); //Error Data Save
						AlarmData.m_fnAddItemFormat("[x:%d,y:%d] : %s ", vError[i].first.x + 1, vError[i].first.y + 1, vError[i].second);
					}

					std::vector<CPoint> vFailPocket;
					vFailPocket.clear();
					vFailPocket = m_v2DMarkErrorData;
					ReportAlarm(ERR_LOAD_TBL_1_VISION_BARCODE_MARK_INVALIDATE + m_nTableIdx, AlarmData, vFailPocket);
					break;
				}

				pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D Barcode Check 2d Mark](END)", m_szCmdName, m_nStep);
			}
			//=========================================================================================================== 2d Duplication Check.
			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D Barcode Check 2d Duplication Check ](STR)", m_szCmdName, m_nStep);

			//기존 No Duplication Check 방식
			//BOOL bNoDupCheck_2D = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_NoDupCheck_2D);

			BOOL b2dDupCheck = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_DUPLICATION_OPT_USE);

			BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
			BOOL bRetDupErr = FALSE;
			if (/*bNoDupCheck_2D != eOPTION_USE*/b2dDupCheck == eOPTION_USE && bAutoGrrUse == FALSE /*&& m_v2DErrorData.size() == 0*//*Length Error시, Pass*/)
			{
				m_v2DBARCODE_FOR_CANCEL.clear();
				m_v2D_DUP_COORDINATE.clear();
				bRetDupErr = g_2DBarcodeChk.IsDupData(m_nTableIdx, m_tVisionMsgMeasureResult.vReqData, m_v2DBARCODE_FOR_CANCEL, m_v2D_DUP_COORDINATE);
				m_bIsBarcodeDup = FALSE;

				if (bRetDupErr == TRUE) {
					std::vector<CPoint> vFailPocket;
					m_bIsBarcodeDup = TRUE;
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Loading Table = %d, The 2D Occur Duplication", m_nTableIdx + 1);

					//get coordinate data
					CString strCoordi = _T("Duplicate Coordinate = ");

					for (int i = 0; i < (int)m_v2D_DUP_COORDINATE.size(); i++) {
						CString strTmpMsg = _T("");

						int nX = m_v2D_DUP_COORDINATE[i].x;
						int nY = m_v2D_DUP_COORDINATE[i].y;

						int nIdx = 0;

						if (nBoardType == eBoardDir_Forward) {
							nIdx = nX + (4 * (nY / 2));			
						}
						else {
							nIdx = nX + (4 * (nY / 3));
						}

						strTmpMsg.Format("[x:%d,y:%d] : %s\n", m_v2D_DUP_COORDINATE[i].x + 1, m_v2D_DUP_COORDINATE[i].y + 1, m_tVisionMsgMeasureResult.vReqData[nIdx].szDataValue);
						strCoordi += strTmpMsg;
						vFailPocket.push_back(CPoint(nX, nY));
					}

					if (strCoordi.IsEmpty() != TRUE) 
					{
						AlarmData.m_fnAddItemFormat("Table %d, %s", m_nTableIdx + 1, strCoordi);
					}
							
					ReportAlarm(ERR_LOAD_TBL_1_VISION_BARCODE_DUP + m_nTableIdx, AlarmData, vFailPocket);
					break;
					}
				}
				pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D Barcode Check 2d Duplication Check ](END)", m_szCmdName, m_nStep);



				//2D Data Save
				//std::vector<CPoint> vLoadPicker; vLoadPicker.clear();
				std::vector<CPoint> vTblPocket; vTblPocket.clear();

				for (int nY = 0; nY < MAX_LD_TBL_POCKET_CNT_Y; nY++) 
				{
					for (int nX = 0; nX < MAX_LD_TBL_POCKET_CNT_X; nX++) 
					{
						char setting = g_TaskLoadTbl[m_nTableIdx].m_LoadTbl_pocket_setting[nY][nX];
						if (setting == 'L')
							vTblPocket.push_back(CPoint(nX, nY));
					}
				}

				int nPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
				int nPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);
//					int nPocketNum = 0;

				for (int i = 0; i < (int)vTblPocket.size(); i++)
				{
					ST_DD_DEVICE tDeviceData;
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1 + m_nTableIdx, vTblPocket[i].x, vTblPocket[i].y, &tDeviceData);
					if (tDeviceData.sExist == eDvcExist || g_TaskLoadTbl[m_nTableIdx].GetEqpCtrlMode() == eEqpCtrlMode_Manual) {
						if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
						{
// 							int nIdx = 0;
// 							BOOL bChk = FALSE;
// 							ST_DD_DEVICE tEmptyDeviceData;
// 							CString strErr = _T("");
// 
// 							for (int j = 0; j < g_BinCategory.nIndexCount; j++) {
// 								//받은 Error code랑 User가 Setting한 Error Code랑 비교
// 								strErr.Format("%s", g_BinCategory.aCateInfo[j].szDesc);
// 								strErr.MakeUpper();
// 								if (strcmp(strErr, DEF_2D_READ_ERR_DEVICE) == 0) {
// 									nIdx = j;
// 									bChk = TRUE;
// 									break;
// 								}
// 							}
// 
// 							//Category 2D Error Sorting
// 							if (bChk) {
// 								tEmptyDeviceData.sExist = DEF_EXIST;
// 								strcpy_s(tEmptyDeviceData.sz2DBarcode, sizeof(tEmptyDeviceData.sz2DBarcode), m_tVisionMsgMeasureResult.vReqData[i].szDataValue);
// 								sprintf_s(tEmptyDeviceData.szBinResult, sizeof(tEmptyDeviceData.szBinResult), "%s", g_BinCategory.aCateInfo[nIdx].szSWBin);
// 								sprintf_s(tEmptyDeviceData.szErrString, sizeof(tEmptyDeviceData.szErrString), "%s", g_BinCategory.aCateInfo[nIdx].szDesc);
// 								sprintf_s(tEmptyDeviceData.szErrorCode, sizeof(tEmptyDeviceData.szErrorCode), "%s", g_BinCategory.aCateInfo[nIdx].szCode);
// 								tEmptyDeviceData.nBinColor = atoi(g_BinCategory.aCateInfo[nIdx].szPassFail);
// 
// 								g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_LD_TBL_1 + m_nTableIdx, vTblPocket[i].x, vTblPocket[i].y, &tEmptyDeviceData);
// 							}
							strcpy_s(tDeviceData.sz2DBarcode, sizeof(tDeviceData.sz2DBarcode), m_tVisionMsgMeasureResult.vReqData[i].szDataValue);
							g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_LD_TBL_1 + m_nTableIdx, vTblPocket[i].x, vTblPocket[i].y, &tDeviceData);
							pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D Barcode Data Save] [Pocket x=%d,y=%d] [2D Data = %s]", m_szCmdName, m_nStep, vTblPocket[i].x, vTblPocket[i].y, m_tVisionMsgMeasureResult.vReqData[i].szDataValue);
						}
						else {
							strcpy_s(tDeviceData.sz2DBarcode, sizeof(tDeviceData.sz2DBarcode), m_tVisionMsgMeasureResult.vReqData[i].szDataValue);
							g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_LD_TBL_1 + m_nTableIdx, vTblPocket[i].x, vTblPocket[i].y, &tDeviceData);
							pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D Barcode Data Save] [Pocket x=%d,y=%d] [2D Data = %s]", m_szCmdName, m_nStep, vTblPocket[i].x, vTblPocket[i].y, m_tVisionMsgMeasureResult.vReqData[i].szDataValue);
						}
					}
				}

// 			if (bRetShortErr == TRUE || bRetDupErr == TRUE) {
// 				pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D Barcode Error][Short Error = %d][Duplicate Error = %d]", m_szCmdName, m_nStep, bRetShortErr, bRetDupErr);
// 				break;
// 			}
			
			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [2D Barcode Check](END)", m_szCmdName, m_nStep);			
			//}

			//2D Barcode Light On
			int nBarcodeIdx = m_nTableIdx / 2;
			if (nBarcodeIdx == eTestPP_1) {
				g_pVisionTestHand_1->Cmd_ReqMeasure_BarcodeLED(eBarcodeLight_OFF, nBarcodeIdx);
			}
			else {
				g_pVisionTestHand_2->Cmd_ReqMeasure_BarcodeLED(eBarcodeLight_OFF, nBarcodeIdx);
			}
			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Barcode LED OFF][Barcode Index=%d](STR)", m_szCmdName, m_nStep, nBarcodeIdx);



			m_nStep = 29000;
		}break;
		case 29000:
			{
				// check arrived seet sensor
				pTaskLoadTbl->MakeLog( "[%s] [EXEC=%8d] [Arrive Sensor Check](STR)", m_szCmdName, m_nStep );
				CString strErr = _T("");
				std::vector<CPoint> vFailPocket;
				std::vector<CString> vErrHistory;

				BOOL bRetErr = ChkErrArrivedSen(strErr, vFailPocket, vErrHistory);
				if( bRetErr != ERR_NO_ERROR && pTaskLoadTbl->m_bLoadTableAlarmMove != TRUE ) {
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( "Load Table arrived safety sensor is detected!! [Load table Number=%d], %s", m_nTableIdx + 1, strErr );
					
					if (g_DMCont.m_dmHandlerOpt.GN(NDM11_LOADING_TABLE_VIBRATOR_OPTION) == DEF_ON)
					{
						int nVibRetryCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_LOADING_TABLE_VIBRATOR_RETRY_CNT);
						if (m_nVibCurTryCnt >= nVibRetryCnt)   // Vibrator를 Retry 횟수만큼 완료
						{
							m_nVibCurTryCnt = 0;
						}
						else     //Vibrator를 Retry 시작
						{
							m_nBackVibratorStep = m_nStep;
							m_nStep = do_Vibrator_On; // Vibrator 동작 Case								
							break;
						}
					}

					if ((int)vErrHistory.size() > 0) {
						for (int i = 0; i < (int)vErrHistory.size(); i++) {
							AlarmData.m_fnAddItemFormat(vErrHistory[i]);
						}
					}
					ReportAlarm(ERR_LOAD_TBL_1_ARRIVE_SAFETY_SENSOR + m_nTableIdx, AlarmData, vFailPocket );
					break;
				}
				pTaskLoadTbl->MakeLog( "[%s] [EXEC=%8d] [Arrive Sensor Check](END)", m_szCmdName, m_nStep );

				if (pTaskLoadTbl->GetTableUsage() == eLoadType && g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use) == TRUE) {
					m_nStep = 29100;
				}
				else {
					m_nStep = 40000;
				}
				//m_nStep = 40000;
			}break;
		case 29100:
		{
			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Move to %s Pos](STR)", m_szCmdName, m_nStep, pTaskLoadTbl->GetLocName(m_nCmdStage));
			int table_col_step = pTaskLoadTbl->GetTableColStep();
			pTaskLoadTbl->MakeLog("[%s] [Move Table Step = %d]", m_szCmdName, table_col_step);
			int nErr = pTaskLoadTbl->do_MovePos(m_nCmdStage, CTaskLoadTable::eAxisY, 100, 0.0, table_col_step);
			if (nErr != ERR_NO_ERROR)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Loading Table = %d, can not move %s.", m_nTableIdx + 1, pTaskLoadTbl->GetLocName(m_nCmdStage));
				OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}

			m_tcTimeout.SetCheckTime((MOTOR_TIME_OUT_MIDDLE / 3.0));
			m_tcTimeout.StartTimer();

			m_nStep++;
		}break;
		case 29101:
		{
			OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);

			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Move to %s Pos][](END)", m_szCmdName, m_nStep, pTaskLoadTbl->GetLocName(m_nCmdStage));
				m_nStep = 40000;
			}
			else {
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(nAxisDone, AlarmData);
				}
			}
		}break;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// Impact Table
		case _eSeqStep_Impact_Move_CurPos: //30000
		{
			int nCmdStage = -1;
	
			if (m_nCmdStage >= LOC_IDX_LD_TBL_TEST_SIDE_1) {
				nCmdStage = LOC_IDX_TBL_LD_SIDE;
			}
			else if (m_nCmdStage == LOC_IDX_TBL_LD_SIDE) {
				std::vector<int> vLocList;
				pTaskLoadTbl->WhereIsMotorPos(vLocList, CTaskLoadTable::eAxisAll);

				//현재 Position에 따라 LOC_IDX_LD_TBL_TEST_SIDE_1 ~ LOC_IDX_LD_TBL_TEST_SIDE_4 결정.
				InvertIdxCurPosToTeachPos(vLocList[CTaskLoadTable::eAxisY], nCmdStage);
			}

			double dOffset = 0.0;
			if (pTaskLoadTbl->GetTableUsage() == eUnloadType)
			{
				dOffset = LOAD_TABLE_MOVE_OFFSET_Y;
			}

			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Move to %s Pos](STR)", m_szCmdName, m_nStep, pTaskLoadTbl->GetLocName(nCmdStage));
			int nErr = pTaskLoadTbl->do_MovePos(nCmdStage, CTaskLoadTable::eAxisY, 100, dOffset);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_MIDDLE / 3.0);
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
		case _eSeqStep_Impact_Move_CurPos_Axisdone: //30001
		{
			OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Move to Pos](END)", m_szCmdName, m_nStep);
// 				if (m_nImpactMoveCnt >= LOAD_TABLE_IMPACT_COUNT) { // nomal seq
// 					m_nStep = 12000;
// 				}
// 				else {					  //checking impact loading tbl
					m_nStep = _eSeqStep_Impact_Move_First_Pos;
//				}
			}
			else {
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nAxisDone, AlarmData);
				}
			}
		}break;
		case _eSeqStep_Impact_Move_First_Pos: //30002
		{
			int nCmdStage = -1;
			double dOffsetPos = 0.0;
			if (m_nCmdStage >= LOC_IDX_LD_TBL_TEST_SIDE_1) {
				nCmdStage = LOC_IDX_TBL_LD_SIDE;
				dOffsetPos = (LOAD_TABLE_IMPACT_DIST + LOAD_TABLE_IMPACT_MOVE_DIST);
			}
			else if (m_nCmdStage == LOC_IDX_TBL_LD_SIDE) {
				std::vector<int> vLocList;
				pTaskLoadTbl->WhereIsMotorPos(vLocList, CTaskLoadTable::eAxisAll);

				//현재 Position에 따라 LOC_IDX_LD_TBL_TEST_SIDE_1 ~ LOC_IDX_LD_TBL_TEST_SIDE_4 결정.
				InvertIdxCurPosToTeachPos(vLocList[CTaskLoadTable::eAxisY], nCmdStage);
				dOffsetPos = (LOAD_TABLE_IMPACT_DIST + LOAD_TABLE_IMPACT_MOVE_DIST) * -1.0;
			}
			if (m_nImpactMoveCnt >= LOAD_TABLE_IMPACT_COUNT) {
				if (m_nCmdStage >= LOC_IDX_LD_TBL_TEST_SIDE_1)
					m_nStep = 20150;
				else
					m_nStep = 12000;

				break;
			}


			if (pTaskLoadTbl->GetTableUsage() == eUnloadType)
			{
				dOffsetPos += LOAD_TABLE_MOVE_OFFSET_Y;
			}

			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [Move to %s Pos](STR)", m_szCmdName, m_nStep, pTaskLoadTbl->GetLocName(nCmdStage));
			// move start org position.
			int nErr = pTaskLoadTbl->do_MovePos(nCmdStage, CTaskLoadTable::eAxisY, 100, dOffsetPos);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_LONG);
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
		case _eSeqStep_Impact_Move_First_Pos_Axisdone:  //30003
		{
			OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				m_nImpactMoveCnt++;
				m_nStep++;
			}
			else {
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nAxisDone, AlarmData);
				}
			}
		}break;
		case _eSeqStep_Impact_Move_Second_Pos:  //30004
		{
			int nCmdStage = -1;
			double dOffsetPos = 0.0;
			if (m_nCmdStage >= LOC_IDX_LD_TBL_TEST_SIDE_1) {
				nCmdStage = LOC_IDX_TBL_LD_SIDE;
				dOffsetPos = (LOAD_TABLE_IMPACT_MOVE_DIST);
			}
			else if (m_nCmdStage == LOC_IDX_TBL_LD_SIDE) {
				std::vector<int> vLocList;
				pTaskLoadTbl->WhereIsMotorPos(vLocList, CTaskLoadTable::eAxisAll);

				//현재 Position에 따라 LOC_IDX_LD_TBL_TEST_SIDE_1 ~ LOC_IDX_LD_TBL_TEST_SIDE_4 결정.
				InvertIdxCurPosToTeachPos(vLocList[CTaskLoadTable::eAxisY], nCmdStage);
				dOffsetPos = (LOAD_TABLE_IMPACT_MOVE_DIST) * -1.0;
			}

			if (pTaskLoadTbl->GetTableUsage() == eUnloadType)
			{
				dOffsetPos += LOAD_TABLE_MOVE_OFFSET_Y;
			}

			// move start org position.
			int nErr = pTaskLoadTbl->do_MovePos(nCmdStage, CTaskLoadTable::eAxisY, 100, dOffsetPos);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime(MOTOR_TIME_OUT_LONG);
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;
		case _eSeqStep_Impact_Move_Second_Pos_Axisdone: //30005
		{


			OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				m_nStep = _eSeqStep_Impact_Move_First_Pos;
			}
			else {
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
					break;
				}

				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
					ReportAlarm(nAxisDone, AlarmData);
					break;
				}
			}
		}break;
		case do_Vibrator_On:
		{
			pTaskLoadTbl->do_SideVibratorOnOff(DEF_ON);

			tmr.StartTmChk();

			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [do_Vibrator_On]", m_szCmdName, m_nStep);
			m_nStep = do_Vibrator_On_Wait;

			break;
		}
		case do_Vibrator_On_Wait:
		{
			// Vibrator Delay Time 추가 [0보다 작거나 '-' 값이 들어가면 최소값 0.1로 만듬] [8/4/2020 donghyun.shin]
			double dVibDelayTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_LOADING_TABLE_VIBRATOR_DELAY_TIME);

			if (dVibDelayTime <= 0)
				dVibDelayTime = 0.05;

			tmr.EndTmChk();
			if (tmr.GetTmDistanceMS() < dVibDelayTime*1000.0)
				break;

			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [do_Vibrator_On_Wait]", m_szCmdName, m_nStep);
			m_nStep = do_Vibrator_Off;

			break;
		}
		case do_Vibrator_Off:
		{
			pTaskLoadTbl->do_SideVibratorOnOff(DEF_OFF);

			tmr.StartTmChk();
			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [do_Vibrator_Off]", m_szCmdName, m_nStep);

			m_tcTimeout.SetCheckTime(TIME_OUT_SHORT);
			m_tcTimeout.StartTimer();

			m_nStep = do_Vibrator_Off_Wait;		
			break;
		}
		case do_Vibrator_Off_Wait:
		{
			// Vibrator Delay Time 추가 [0보다 작거나 '-' 값이 들어가면 최소값 0.1로 만듬] [8/4/2020 donghyun.shin]
			double dVibDelayTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_LOADING_TABLE_VIBRATOR_DELAY_TIME);
			if (dVibDelayTime <= 0)
				dVibDelayTime = 0.05;

			tmr.EndTmChk();
			if (tmr.GetTmDistanceMS() < dVibDelayTime*1000.0)
				break;

			if (m_tcTimeout.CheckOverTime() == TRUE)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
				ReportAlarm(ERR_LOAD_TBL_1_VIBRATOR_CYLINDER_BWD_TIMEOUT + m_nTableIdx, AlarmData);
				break;
			}

			int nErr = pTaskLoadTbl->chk_cyl_vibrator_onoff(DEF_OFF);
			if (nErr != ERR_NO_ERROR)
			{
				break;
			}
	
			pTaskLoadTbl->MakeLog("[%s] [EXEC=%8d] [do_Vibrator_Off_Wait]", m_szCmdName, m_nStep);
			m_nStep = Vibrator_complete;
			break;
		}
		case Vibrator_complete:
		{
			m_nVibCurTryCnt++;
			m_nStep = m_nBackVibratorStep;
			break;
		}
///////////////////////////////////////////////////////////////////////////////////////////////////////
		case 40000:
			{
				ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_NormalComplete );
			}break;
	}
	return CBaseSeqCmd::State_Executing();
}

//==============================================================================
//
//==============================================================================
BOOL CLoadTblCmd_MovePos::State_Pause()
{
	CTaskLoadTable* pTaskLoadTbl = ( CTaskLoadTable* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTaskLoadTbl->MakeLog( "[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
		case 0:
			{
				int nAxisCount = CTaskLoadTable::eMaxAxisCount;
				for( int i = 0; i < nAxisCount; i++ ) {
					OneAxis* pMotor = pTaskLoadTbl->GetAxis( i );
					if( pMotor != NULL )
					{
						//pMotor->SetSlowStop();
						bool bCloseLimit = pMotor->CheckLimitClose();
						double dCurPos = 0;
						pMotor->GetCurrentPos(dCurPos);
						if (bCloseLimit == false)
						{
							pTaskLoadTbl->MakeLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
							pMotor->SetSlowStop();
						}
					}
				}
				m_nStep++;
			}break;
		case 1:
			{

			}break;
		default:
		{
			m_nStep = 0;
		}break;
	}

	return CBaseSeqCmd::State_Pause();
}

//==============================================================================
//
//==============================================================================
BOOL CLoadTblCmd_MovePos::State_Recovery()
{
	CTaskLoadTable* pTaskLoadTbl = ( CTaskLoadTable* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		//pTaskLoadTbl->MakeLog("[CMD: %s] [RECOVERY] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
		case 0:	// check interlock
		{
			pTaskLoadTbl->MakeLog("[%s] [RECOVERY=%8d]", m_szCmdName, m_nStep);

			m_tcTimeout.SetCheckTime(TIME_OUT_SHORT);
			m_tcTimeout.StartTimer();
			m_nStep++;
		}break;	
		case 1: //checking Load Table Vibrator OFF
		{
			if (m_tcTimeout.CheckOverTime() == TRUE)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
				ReportAlarm(ERR_LOAD_TBL_1_VIBRATOR_CYLINDER_BWD_TIMEOUT + m_nTableIdx, AlarmData);
				break;
			}

			if (g_DMCont.m_dmEQP.GN(NDM0_TABLE_VIBRATOR_OPTION) == DEF_ON)
			{
				int nErr = pTaskLoadTbl->chk_cyl_vibrator_onoff(DEF_OFF);
				if (nErr != ERR_NO_ERROR)
				{
					pTaskLoadTbl->MakeLog("[%s] [RECOVERY=%8d] Interlock Vibrator On -> Off", m_szCmdName, m_nStep);
					m_nBackVibratorStep = m_nStep;
					m_nStep = do_Vibrator_Off;
					break;
				}
			}

			m_nStep++;
		}break;
		case 2: // 현재 위치와 마지막 위치를 비교 한다.
			{
				//2D Barcode Light On
				if (pTaskLoadTbl->GetTableUsage() == eLoadType)
				{
					int nBarcodeIdx = m_nTableIdx / 2;
					if (nBarcodeIdx == eTestPP_1) {
						g_pVisionTestHand_1->Cmd_ReqMeasure_BarcodeLED(eBarcodeLight_ON, nBarcodeIdx);
					}
					else {
						g_pVisionTestHand_2->Cmd_ReqMeasure_BarcodeLED(eBarcodeLight_ON, nBarcodeIdx);				
					}
					pTaskLoadTbl->MakeLog("[%s] [RECOVERY=%8d] [Barcode LED ON][Barcode Index=%d](STR)", m_szCmdName, m_nStep, nBarcodeIdx);
				}

				pTaskLoadTbl->MakeLog( "[%s] [RECOVERY=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
				m_tcTimeout.SetCheckTime( ( MOTOR_TIME_OUT_MIDDLE / 3.0 ) );
				m_tcTimeout.StartTimer();
				m_nStep++;
			}break;
		case 3:
			{
				if( m_tcTimeout.CheckOverTime() == TRUE )
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat( "OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1 );
					ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData );
					break;
				}

				pTaskLoadTbl->m_bLoadTableAlarmMove = FALSE;

				int nchkLoadSite = g_TaskAutoLdTable[m_nTableIdx].ChkMoveLoadSite();
				int nchkTestSite = g_TaskAutoLdTable[m_nTableIdx].ChkMoveTestSite();
				if(nchkLoadSite == ERR_NO_ERROR
					&& nchkTestSite == ERR_NO_ERROR )
				{
					if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use) == TRUE && pTaskLoadTbl->GetTableUsage() == eLoadType && 
						((int)m_v2D_DUP_COORDINATE.size() > 0 || (int)m_v2DErrorData.size() > 0 || (int)m_v2DMarkErrorData.size() > 0))
					{
						pTaskLoadTbl->MakeLog("[%s] [RECOVERY=%8d] [Duplicate Count = %d] [2D ]", m_szCmdName, m_nStep, (int)m_v2D_DUP_COORDINATE.size());
						m_nStep = 1000;
						break;
					}


					//pTaskLoadTbl->MakeLog("[%s] [RECOVERY=%8d] [LastStep=%d][CmdStage=%s] move to Load side[STR]", m_szCmdName, m_nStep, m_nLast_Executing_Step, pTaskLoadTbl->GetLocName(m_nCmdStage));
					std::vector<int> vLocList;
					pTaskLoadTbl->WhereIsMotorPos(vLocList, CTaskLoadTable::eAxisAll);

					//Load Site 확인
					if( m_nCmdStage >= LOC_IDX_LD_TBL_TEST_SIDE_1 && m_nCmdStage <= LOC_IDX_LD_TBL_TEST_SIDE_4) {
						if (vLocList[CTaskLoadTable::eAxisY] <= CTaskLoadTable::eYPOS_LoadSideToSitePos1 && pTaskLoadTbl->GetTableUsage() == eLoadType) {
							m_nStep = 1000;
							break;
						}

						
						//Test Site 확인
						if (pTaskLoadTbl->GetTableUsage() == eLoadType 
							&& vLocList[CTaskLoadTable::eAxisY] >= CTaskLoadTable::eYPOS_LoadSideToSitePos1)
						{
							BOOL bNotExistData = FALSE;
							ST_DD_DEVICE tDeviceData;
							int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
							for (int y = 0; y < m_nPocketCntY; y++)
							{

								if (nBoardType == eBoardDir_Forward) {
									if (y == 1 || y == 3) continue; //Buffer는 확인 안함.				
								}
								else {
									if (y == 0 || y == 2) continue; //Buffer는 확인 안함.
								}

								for (int x = 0; x < m_nPocketCntX; x++)
								{
									tDeviceData.clear();
									g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1 + m_nTableIdx, x, y, &tDeviceData);
									if (tDeviceData.sExist == eDvcExist && strcmp(tDeviceData.sz2DBarcode, "") == 0 && CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto /*|| CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Manual*/) {
										pTaskLoadTbl->MakeLog("[%s] [RECOVERY=%8d] [2D Data Not Exist] [X:%d, Y:%d]", m_szCmdName, m_nStep, x, y);
										bNotExistData = TRUE;
									}
								}
							}
							if (bNotExistData == TRUE)
							{
								m_nStep = 1000;
								break;
							}
						}



					}
					else {					
// 						pTaskLoadTbl->MakeLog("[%s] [RECOVERY=%8d] [LastStep=%d][CmdStage=%s] check move to Load Pre Position", m_szCmdName, m_nStep, m_nLast_Executing_Step, pTaskLoadTbl->GetLocName(m_nCmdStage));
// 						double dCurYPos = 0.0;
// 						pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY)->GetCurrentPos(dCurYPos);
// 						pTaskLoadTbl->MakeLog("Y Pos = %.3f, vloc = %d", dCurYPos, vLocList[CTaskLoadTable::eAxisY]);
// 						int ntest = vLocList[CTaskLoadTable::eAxisY];
// 						if (pTaskLoadTbl->GetTableUsage() == eUnloadType && vLocList[CTaskLoadTable::eAxisY] <= CTaskLoadTable::eYPOS_LoadSideToSitePos1) {
// 							m_nStep = 1000;
// 							break;
// 						}			
					}

					OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
					double dCurPosY = 0.0;
					pMotor->GetCurrentPos(dCurPosY);
					if (pTaskLoadTbl->GetTableUsage() == eUnloadType)
					{
						if ((INTERLOCK_LOADING_TABLE_Y_FRAM_IMPACT_LOC_START <= dCurPosY && dCurPosY <= INTERLOCK_LOADING_TABLE_Y_FRAM_IMPACT_LOC_END)
							|| dCurPosY <= INTERLCOK_LOADING_TABLE_Y_STACKER_IMPACT_LOC)
						{

						}
						else {
							m_nStep = 1000;
							break;
						}
					}
					
					pTaskLoadTbl->MakeLog("[%s] [RECOVERY=%8d] [LastStep=%d][CmdStage=%s] change executing", m_szCmdName, m_nStep, m_nLast_Executing_Step, pTaskLoadTbl->GetLocName(m_nCmdStage));
					SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing);
					break;				
				}
			}break;
// 		case 10:
// 		{
// 			pTaskLoadTbl->MakeLog("[%s] [RECOVERY=%8d] [Check Rotate Direction Before Move to %s Pos] [LastStep=%d] (STR)", m_szCmdName, m_nStep, pTaskLoadTbl->GetLocName(LOC_IDX_TBL_LD_SIDE), m_nLast_Executing_Step);
// 			
// 		}break;
		case 1000:
		{
			pTaskLoadTbl->MakeLog("[%s] [RECOVERY=%8d] [Rotate Move to %s Pos] [LastStep=%d] (STR)", m_szCmdName, m_nStep, pTaskLoadTbl->GetLocName(LOC_IDX_LD_TBL_TEST_SIDE_1), m_nLast_Executing_Step);
			int nErr = pTaskLoadTbl->do_MovePos(LOC_IDX_LD_TBL_TEST_SIDE_1, CTaskLoadTable::eAxisRotate);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisRotate);
				AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
				ReportAlarm(nErr, AlarmData);
				break;
			}
			m_tcTimeout.SetCheckTime((MOTOR_TIME_OUT_MIDDLE / 3.0));
			m_tcTimeout.StartTimer();
			m_nStep = 1001;
		}break;
		case 1001:
		{
			OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisRotate);

			int nAxisDone = pMotor->AxisDone();
			if (nAxisDone == ERR_NO_ERROR) {
				pTaskLoadTbl->MakeLog("[%s] [RECOVERY=%8d] [Rotate Move to %s Pos] [LastStep=%d] (END)", m_szCmdName, m_nStep, pTaskLoadTbl->GetLocName(LOC_IDX_TBL_LD_SIDE), m_nLast_Executing_Step);
				if(pTaskLoadTbl->GetTableUsage() == eUnloadType)
				{
					SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing);
				}
				else {
					m_nStep = 1002;
					break;
				}
				
			}
			else
			{
				if (m_tcTimeout.CheckOverTime() == TRUE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
					break;
				}
				if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("%s Axis Done Fail", pMotor->GetAxisName());
					AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
					ReportAlarm(nAxisDone, AlarmData);
				}
			}
		}break;
		case 1002: // Move setplate, vision 검사를 다시 하여야 한다.
			{
				pTaskLoadTbl->MakeLog( "[%s] [RECOVERY=%8d] [Y Axis Move to %s Pos] [LastStep=%d] (STR)", m_szCmdName, m_nStep, pTaskLoadTbl->GetLocName(  LOC_IDX_TBL_LD_SIDE ), m_nLast_Executing_Step);

				double dOffset = 0.0;
				if (pTaskLoadTbl->GetTableUsage() == eUnloadType) {
					dOffset = LOAD_TABLE_MOVE_OFFSET_Y;
				}
 				int nErr = pTaskLoadTbl->do_MovePos(  LOC_IDX_TBL_LD_SIDE, CTaskLoadTable::eAxisY, 100, dOffset );
 				if( nErr != ERR_NO_ERROR ) {
 					CParamList AlarmData;
					OneAxis* pMotor = pTaskLoadTbl->GetAxis(CTaskLoadTable::eAxisY);
					AlarmData.m_fnAddItemFormat("Axis=%s", pMotor->GetAxisName());
 					ReportAlarm( nErr, AlarmData );
 					break;
 				}
				m_tcTimeout.SetCheckTime( ( MOTOR_TIME_OUT_MIDDLE / 3.0 ) );
				m_tcTimeout.StartTimer();
				m_nStep = 2000;
			}break;
		case 2000:
			{
				int nAxisCount = CTaskLoadTable::eMaxAxisCount;
				int nCompChk = 0;
				for( int i = 0; i < CTaskLoadTable::eMaxAxisCount; i++ ) {
					OneAxis* pMotor = pTaskLoadTbl->GetAxis( i );
					if( pMotor == NULL )
					{
						nCompChk++;
						continue;
					}
					int nAxisDone = pMotor->AxisDone();
					if( nAxisDone == ERR_NO_ERROR ) {
						nCompChk++;
					}
					else 
					{
						if (m_tcTimeout.CheckOverTime() == TRUE)
						{
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
							AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
							ReportAlarm(ERR_LOAD_TBL_1_MOVE_TIMEOUT + m_nTableIdx, AlarmData);
							break;
						}
						if (nAxisDone != ERR_MOTOR_CMN_IN_MOVE_STATE)
						{
							CParamList AlarmData;
							AlarmData.m_fnAddItemFormat("%s Axis Done Fail", pMotor->GetAxisName());
							AlarmData.m_fnAddItemFormat("Axis=%s ", pMotor->GetAxisName());
							ReportAlarm(nAxisDone, AlarmData);
						}
					}
				}
				if( nCompChk == nAxisCount ) {
					pTaskLoadTbl->MakeLog("[%s] [RECOVERY=%8d] [Move to %s Pos] [LastStep=%d] (END)", m_szCmdName, m_nStep, pTaskLoadTbl->GetLocName(LOC_IDX_TBL_LD_SIDE), m_nLast_Executing_Step);
					SetCmdStatus( CBaseSeqCmd::eSeqCmdStatus_Executing );
				}
			}break;
		case do_Vibrator_Off:
		{
			pTaskLoadTbl->do_SideVibratorOnOff(DEF_OFF);

			tmr.StartTmChk();
			pTaskLoadTbl->MakeLog("[%s] [RECOVERY=%8d] [do_Vibrator_Off]", m_szCmdName, m_nStep);
			m_nStep = do_Vibrator_Off_Wait;

			m_tcTimeout.SetCheckTime(TIME_OUT_SHORT);
			m_tcTimeout.StartTimer();

			break;
		}
		case do_Vibrator_Off_Wait:
		{
			// Vibrator Delay Time 추가 [0보다 작거나 '-' 값이 들어가면 최소값 0.1로 만듬] [8/4/2020 donghyun.shin]
			double dVibDelayTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_LOADING_TABLE_VIBRATOR_DELAY_TIME);

			if (dVibDelayTime <= 0)
				dVibDelayTime = 0.05;

			tmr.EndTmChk();
			if (tmr.GetTmDistanceMS() < dVibDelayTime*1000.0)
				break;

			if (m_tcTimeout.CheckOverTime() == TRUE)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("OverTime(sec)=%.3f, LoadTbl=%d", m_tcTimeout.GetCheckTime(), m_nTableIdx + 1);
				ReportAlarm(ERR_LOAD_TBL_1_VIBRATOR_CYLINDER_BWD_TIMEOUT + m_nTableIdx, AlarmData);
				break;
			}

			int nErr = pTaskLoadTbl->chk_cyl_vibrator_onoff(DEF_OFF);
			if (nErr != ERR_NO_ERROR)
			{
				break;
			}

			pTaskLoadTbl->MakeLog("[%s] [RECOVERY=%8d] [do_Vibrator_Off_Wait]", m_szCmdName, m_nStep);
			m_nStep = Vibrator_complete;
			break;
		}
		case Vibrator_complete:
		{
			m_nStep = m_nBackVibratorStep;
			break;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		default:
		{
			m_nStep = 0;
		}break;
	}

	return CBaseSeqCmd::State_Recovery();
}

//==============================================================================
//
//==============================================================================
BOOL CLoadTblCmd_MovePos::State_NormalComplete()
{
	CTaskLoadTable* pTaskLoadTbl = ( CTaskLoadTable* )m_pReceive;

	if (CBaseTaskManager::m_bEqpControlMode == eSeqMode_Auto)
	{
		if (m_nCmdStage ==  LOC_IDX_TBL_LD_SIDE) {
//			pTaskLoadTbl->SetWorkReserve( RID_TRAY_PP_(RIDX_TRAY_PP_1) );

			int nIdx = ((m_nTableIdx / 2) == 0) ? RIDX_TRAY_PP_1 : RIDX_TRAY_PP_2;

			if(m_nTableIdx == eLoadTbl_2 || m_nTableIdx == eLoadTbl_3)
				pTaskLoadTbl->SetWorkReserve(RID_TRAY_PP_(eTrayPP_1_LD));
			else if (m_nTableIdx == eLoadTbl_1 || m_nTableIdx == eLoadTbl_4)
				pTaskLoadTbl->SetWorkReserve(RID_TRAY_PP_(eTrayPP_2_UD));

			// 1. Setplate로 Move 할 경우 시간을 기록 한다.
			// 2. 시간 기록을 하여야, Tray PP에서 작업 할 때 순서처리 가능 함.
			//if (m_nCmdStage ==  LOC_IDX_TBL_LD_SIDE && CBaseTaskManager::m_bEqpControlMode == eEqpCtrlMode_Auto) {
			CTime time = CTime::GetCurrentTime();
			pTaskLoadTbl->SetMoveSetPlateTm(time);
			//}

		}
		else {
			pTaskLoadTbl->SetWorkReserve( RID_TEST_PP_(m_nTableIdx /2) );
		}
	}

	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CLoadTblCmd_MovePos::State_Alarm()
{
	CTaskLoadTable* pTaskLoadTbl = ( CTaskLoadTable* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTaskLoadTbl->MakeLog( "[CMD: %s] [ALARM] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
		case 0: // alarm 발생 했을 때 처리해 줘야 할꺼 처리 해줌
			{
// 				int nCamNo = (m_nTableIdx / 2 == 0) ? VISION_CAM_ID_2DBRACODE_1 : VISION_CAM_ID_2DBRACODE_3;
// 				for (int i = 0; i < 2; i++)
// 				{
// 					g_pBarcode->Cmd_KeyenceLOff(nCamNo);
// 				}

				int nAxisCount = CTaskLoadTable::eMaxAxisCount;
				for (int i = 0; i < nAxisCount; i++) {
					OneAxis* pMotor = pTaskLoadTbl->GetAxis(i);
					if (pMotor != NULL)
					{
						//pMotor->SetSlowStop();
						bool bCloseLimit = pMotor->CheckLimitClose();
						double dCurPos = 0;
						pMotor->GetCurrentPos(dCurPos);
						if (bCloseLimit == false)
						{
							pTaskLoadTbl->MakeLog("[Motor=%s / SetSlowStop = %.2fmm Pos]", pMotor->GetAxisName(), dCurPos / 1000.0);
							pMotor->SetSlowStop();
						}
					}
				}

// 				// 미리 좌측으로 움직여 놓는다.
// 				if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use) == TRUE && m_nCmdStage == LOC_IDX_LD_TBL_TEST_SIDE_1) {
// 					pTaskLoadTbl->do_cyl_2DBarcode_LeftRight(DEF_LEFT);
// 				}

				m_nStep++;
			}break;
		case 1: // Retry, Skip을 기다림.
			{
			}break;
		default:
		{
			m_nStep = 0;
		}break;
	}
	return CBaseSeqCmd::State_Alarm();
}

//==============================================================================
//
//==============================================================================
BOOL CLoadTblCmd_MovePos::State_AlarmComplete()
{
	CTaskLoadTable* pTaskLoadTbl = (CTaskLoadTable*)m_pReceive;
	BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
	BOOL bAutoLoopUse = g_TaskSystemCtrl.GetAutoLoopUse();


// 	int nCamNo = (m_nTableIdx / 2 == 0) ? VISION_CAM_ID_2DBRACODE_1 : VISION_CAM_ID_2DBRACODE_3;
// 	for (int i = 0; i < 2; i++)
// 	{
// 		g_pBarcode->Cmd_KeyenceLOff(nCamNo);
// 	}

	pTaskLoadTbl->MakeLog("[%s] [AlarmComplete=%8d] [STR]", m_szCmdName, m_nStep);


	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
// 	// 미리 좌측으로 움직여 놓는다.
// 	if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use) == TRUE && m_nCmdStage == LOC_IDX_LD_TBL_TEST_SIDE_1) {
// 		pTaskLoadTbl->do_cyl_2DBarcode_LeftRight(DEF_LEFT);
// 	}
	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
	if (nRunMode == eRUN_MODE_ONLINE || nRunMode == eRUN_MODE_OFFLINE) {
		if (m_bIsBarcodeDup == TRUE) {
			if ((int)m_v2D_DUP_COORDINATE.size() > 0) {
				for (int i = 0; i < (int)m_v2D_DUP_COORDINATE.size(); i++)
				{
					int nIdx = 0;
					BOOL bChk = FALSE;
					ST_DD_DEVICE tEmptyDeviceData;
					CString strErr = _T("");
					int n2dReadErrorCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLot2D_Read_Error_Cnt);

					for (int i = 0; i < g_BinCategory.nIndexCount; i++) {
						// 받은 Error code랑 User가 Setting한 Error Code랑 비교
						strErr.Format("%s", g_BinCategory.aCateInfo[i].szDesc);
						strErr.MakeUpper();
						if (strcmp(strErr, DEF_2D_READ_ERR_DEVICE) == 0) {
							nIdx = i;
							bChk = TRUE;
							break;
						}
					}

					if (bAutoGrrUse != TRUE && bAutoLoopUse != TRUE)
					{
						if (bChk) {
							//2D Barcode Read Error sorting 관련
//							for (int i = 0; i < (int)m_v2D_DUP_COORDINATE.size(); i++) {
								tEmptyDeviceData.sExist = DEF_EXIST;
								sprintf_s(tEmptyDeviceData.szBinResult, sizeof(tEmptyDeviceData.szBinResult), "%s", g_BinCategory.aCateInfo[nIdx].szSWBin);
								sprintf_s(tEmptyDeviceData.szErrString, sizeof(tEmptyDeviceData.szErrString), "%s", g_BinCategory.aCateInfo[nIdx].szDesc);
								sprintf_s(tEmptyDeviceData.szErrorCode, sizeof(tEmptyDeviceData.szErrorCode), "%s", g_BinCategory.aCateInfo[nIdx].szCode);
								sprintf_s(tEmptyDeviceData.szTesterHWBin, sizeof(tEmptyDeviceData.szTesterHWBin), "%s", g_BinCategory.aCateInfo[nIdx].szHWBin_Tester);
								tEmptyDeviceData.nBinColor = atoi(g_BinCategory.aCateInfo[nIdx].szPassFail);
								tEmptyDeviceData.nTestingCnt = eARB_3stTestEnd;
								g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_LD_TBL_1 + m_nTableIdx, m_v2D_DUP_COORDINATE[i].x, m_v2D_DUP_COORDINATE[i].y, &tEmptyDeviceData);
								n2dReadErrorCnt++;
//							}
							g_DMCont.m_dmEQP.SN(NDM0_CurLot2D_Read_Error_Cnt, n2dReadErrorCnt);
						}
					}
				}
			}
		}
//2D Mark Error
		if ((int)m_v2DMarkErrorData.size() > 0) {
			int nIdx = 0;
			BOOL bChk = FALSE;
			ST_DD_DEVICE tEmptyDeviceData;
			tEmptyDeviceData.clear();
			CString strErr = _T("");
			int n2dReadErrorCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLot2D_Read_Error_Cnt);

			for (int i = 0; i < g_BinCategory.nIndexCount; i++) {
				// 받은 Error code랑 User가 Setting한 Error Code랑 비교
				strErr.Format("%s", g_BinCategory.aCateInfo[i].szDesc);
				strErr.MakeUpper();
				if (strcmp(strErr, nProjectOption == DEF_PROJECT_WATT ? DEF_TEST_RESULT_WRONG_APN : DEF_2D_READ_ERR_DEVICE) == 0) {
					nIdx = i;
					bChk = TRUE;
					break;
				}
			}

			if (bAutoGrrUse != TRUE && bAutoLoopUse != TRUE)
			{
				if (bChk) {
					//2D Barcode Read Error sorting 관련
					for (int i = 0; i < (int)m_v2DMarkErrorData.size(); i++) {
						//tEmptyDeviceData.clear();
						tEmptyDeviceData.sExist = DEF_EXIST;
						sprintf_s(tEmptyDeviceData.szBinResult, sizeof(tEmptyDeviceData.szBinResult), "%s", g_BinCategory.aCateInfo[nIdx].szSWBin);
						sprintf_s(tEmptyDeviceData.szErrString, sizeof(tEmptyDeviceData.szErrString), "%s", g_BinCategory.aCateInfo[nIdx].szDesc);
						sprintf_s(tEmptyDeviceData.szErrorCode, sizeof(tEmptyDeviceData.szErrorCode), "%s", g_BinCategory.aCateInfo[nIdx].szCode);
						sprintf_s(tEmptyDeviceData.szTesterHWBin, sizeof(tEmptyDeviceData.szTesterHWBin), "%s", g_BinCategory.aCateInfo[nIdx].szHWBin_Tester);
						tEmptyDeviceData.nBinColor = atoi(g_BinCategory.aCateInfo[nIdx].szPassFail);
						tEmptyDeviceData.nTestingCnt = eARB_3stTestEnd;
						g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_LD_TBL_1 + m_nTableIdx, m_v2DMarkErrorData[i].x, m_v2DMarkErrorData[i].y, &tEmptyDeviceData);
						n2dReadErrorCnt++;
					}
					g_DMCont.m_dmEQP.SN(NDM0_CurLot2D_Read_Error_Cnt, n2dReadErrorCnt);
				}
			}
			/*for( int i = 0; i < ( int )m_v2DErrorData.size(); i++ ) {
			ST_DD_DEVICE tEmptyDeviceData;
			g_DMCont.m_dmDevice.SetDeviceData( eDeviceStage_LD_TBL_1 + m_nTableIdx, m_v2DErrorData[ i ].x, m_v2DErrorData[ i ].y, &tEmptyDeviceData );
			}*/
		}

// 2D Error
		if ((int)m_v2DErrorData.size() > 0) {
			int nIdx = 0;
			BOOL bChk = FALSE;
			ST_DD_DEVICE tEmptyDeviceData;
			tEmptyDeviceData.clear();
			CString strErr = _T("");
			int n2dReadErrorCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLot2D_Read_Error_Cnt);

			for (int i = 0; i < g_BinCategory.nIndexCount; i++) {
				// 받은 Error code랑 User가 Setting한 Error Code랑 비교
				strErr.Format("%s", g_BinCategory.aCateInfo[i].szDesc);
				strErr.MakeUpper();
				if (strcmp(strErr, DEF_2D_READ_ERR_DEVICE) == 0) {
					nIdx = i;
					bChk = TRUE;
					break;
				}
			}

			if (bAutoGrrUse != TRUE && bAutoLoopUse != TRUE )
			{
				if (bChk) {
					//2D Barcode Read Error sorting 관련
					for (int i = 0; i < (int)m_v2DErrorData.size(); i++) {
						//tEmptyDeviceData.clear();
						tEmptyDeviceData.sExist = DEF_EXIST;
						sprintf_s(tEmptyDeviceData.szBinResult, sizeof(tEmptyDeviceData.szBinResult), "%s", g_BinCategory.aCateInfo[nIdx].szSWBin);
						sprintf_s(tEmptyDeviceData.szErrString, sizeof(tEmptyDeviceData.szErrString), "%s", g_BinCategory.aCateInfo[nIdx].szDesc);
						sprintf_s(tEmptyDeviceData.szErrorCode, sizeof(tEmptyDeviceData.szErrorCode), "%s", g_BinCategory.aCateInfo[nIdx].szCode);
						sprintf_s(tEmptyDeviceData.szTesterHWBin, sizeof(tEmptyDeviceData.szTesterHWBin), "%s", g_BinCategory.aCateInfo[nIdx].szHWBin_Tester);
						tEmptyDeviceData.nBinColor = atoi(g_BinCategory.aCateInfo[nIdx].szPassFail);
						tEmptyDeviceData.nTestingCnt = eARB_3stTestEnd;
						g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_LD_TBL_1 + m_nTableIdx, m_v2DErrorData[i].x, m_v2DErrorData[i].y, &tEmptyDeviceData);
						n2dReadErrorCnt++;
					}
					g_DMCont.m_dmEQP.SN(NDM0_CurLot2D_Read_Error_Cnt, n2dReadErrorCnt);
				}
			}
			/*for( int i = 0; i < ( int )m_v2DErrorData.size(); i++ ) {
				ST_DD_DEVICE tEmptyDeviceData;
				g_DMCont.m_dmDevice.SetDeviceData( eDeviceStage_LD_TBL_1 + m_nTableIdx, m_v2DErrorData[ i ].x, m_v2DErrorData[ i ].y, &tEmptyDeviceData );
			}*/
		}


		//Error Device 말고 정상적인 Data 처리.


		if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use) == TRUE) {
			std::vector<CPoint> vTblPocket;
			vTblPocket.clear();

			for (int nY = 0; nY < MAX_LD_TBL_POCKET_CNT_Y; nY++) {
				for (int nX = 0; nX < MAX_LD_TBL_POCKET_CNT_X; nX++) {
					char setting = g_TaskLoadTbl[m_nTableIdx].m_LoadTbl_pocket_setting[nY][nX];
					if (setting == 'L')
						vTblPocket.push_back(CPoint(nX, nY));
				}
			}

			for (int i = 0; i < (int)vTblPocket.size(); i++)
			{
				ST_DD_DEVICE tDeviceData;
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1 + m_nTableIdx, vTblPocket[i].x, vTblPocket[i].y, &tDeviceData);

				//Table에 Device가 있고, Barcode Data가 없을 때.
				if (tDeviceData.sExist == eDvcExist && strcmp(tDeviceData.szErrString, DEF_2D_READ_ERR_DEVICE) != 0)
				{
					strcpy_s(tDeviceData.sz2DBarcode, sizeof(tDeviceData.sz2DBarcode), m_tVisionMsgMeasureResult.vReqData[i].szDataValue);
					g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_LD_TBL_1 + m_nTableIdx, vTblPocket[i].x, vTblPocket[i].y, &tDeviceData);
					pTaskLoadTbl->MakeLog("[%s] [2D Barcode Data Save] [Pocket x=%d,y=%d] [2D Data = %s]", m_szCmdName, vTblPocket[i].x, vTblPocket[i].y, m_tVisionMsgMeasureResult.vReqData[i].szDataValue);
				}
			}

		}
	}

	pTaskLoadTbl->MakeLog("[%s] [AlarmComplete=%8d] [END]", m_szCmdName, m_nStep);

	return CBaseSeqCmd::State_AlarmComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CLoadTblCmd_MovePos::State_Aborting()
{
	CTaskLoadTable* pTaskLoadTbl = ( CTaskLoadTable* )m_pReceive;
	if( m_nStep_New != -1 ) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ) {
		pTaskLoadTbl->MakeLog( "[CMD: %s] [ABORT] [Step=%8d]", m_szCmdName, m_nStep );
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
		case 0:
			{
				int nAxisCount = CTaskLoadTable::eMaxAxisCount;
				for( int i = 0; i < nAxisCount; i++ ) {
					OneAxis* pMotor = pTaskLoadTbl->GetAxis( i );
					if( pMotor != NULL )
					{
						pMotor->SetSlowStop();
					}
				}
				m_nStep++;
			}break;
		case 1:
			{
				ReportCmdEnd( CBaseSeqCmd::eSeqCmdStatus_AbortComplete ); // 명령 진행 중 STOP 명령을 받아 명령이 중단 됨.
			}break;
		default:
		{
			m_nStep = 0;
		}break;
	}
	return CBaseSeqCmd::State_Aborting();
}

//==============================================================================
//
//==============================================================================
BOOL CLoadTblCmd_MovePos::State_AbortComplete()
{
// 	int nCamNo = (m_nTableIdx / 2 == 0) ? VISION_CAM_ID_2DBRACODE_1 : VISION_CAM_ID_2DBRACODE_3;
// 	for (int i = 0; i < 2; i++)
// 	{
// 		g_pBarcode->Cmd_KeyenceLOff(nCamNo);
// 	}

	return CBaseSeqCmd::State_AbortComplete();
}


BOOL CLoadTblCmd_MovePos::ChkErrArrivedSen(CString& strErr, std::vector<CPoint>& vFailPocket, std::vector<CString>& vErrHistor)
{
	CTaskLoadTable* pTaskLoadTbl = ( CTaskLoadTable* )m_pReceive;

	BOOL bArrivedSen = g_DMCont.m_dmHandlerOpt.GB( BDM11_MD_LoadTblArrived );
	CString strTemp = _T("");

	if( bArrivedSen ) {
		int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN( NDM11_SYS_MD_DEVICE_CHK );
		if( nDeviceMode == eDeviceMode ) {
			BOOL bAlarmSensor = FALSE;

			for( int nY = 0; nY < MAX_LD_TBL_POCKET_CNT_Y; nY++ )
			{
				BOOL bCheckMode = FALSE;
				for( int nX = 0; nX < MAX_LD_TBL_POCKET_CNT_X && bCheckMode == FALSE; nX++ )
				{
					int nExist = 0;
					pTaskLoadTbl->GetDeviceDataExist( nX, nY, &nExist );
					if( nExist == TRUE || eDeviceMode != nDeviceMode ) { bCheckMode = TRUE; }
				}

				// Exist device & checking sensor [ B-Type ]
				if( bCheckMode == TRUE ) {
					int nRetSen = pTaskLoadTbl->ChkDI( CTaskLoadTable::xLOAD_TABLE_DEVICE1_SEN + nY, DEF_OFF );

					if( nRetSen == ERR_NO_ERROR ) {
						strTemp.Format("(Y =%d)", nY + 1);
						strErr += strTemp;
						//Error pocket
						for (int nX = 0; nX < MAX_LD_TBL_POCKET_CNT_X; nX++) {
							vFailPocket.push_back(CPoint(nX, nY));
						}
						// Pick History
						ST_DD_DEVICE stDvc;
						for (int nX = 0; nX < MAX_LD_TBL_POCKET_CNT_X; nX++) {
							g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1 + m_nTableIdx, nX, nY, &stDvc);

							if (stDvc.sExist == DEF_EXIST) {
								strTemp.Format(_T("[Pocket X=%d Y=%d] Device Pick History Stage: %s, Pocket: X=%d Y=%d, Picker: X=%d Y=%d"),
									nX + 1, nY + 1, g_TaskSystemCtrl.GetHistoryStageName(stDvc.stHistory.nStage), stDvc.stHistory.cPocketNum.x + 1, stDvc.stHistory.cPocketNum.y + 1, stDvc.stHistory.cPicker.x + 1, stDvc.stHistory.cPicker.y + 1);

								vErrHistor.push_back(strTemp);
							}
						}

						bAlarmSensor = TRUE; //Error detected
					}
				}
			}

			if( TRUE == bAlarmSensor ) {
				return TRUE;
			}
		}
	}

	return ERR_NO_ERROR;
}

void CLoadTblCmd_MovePos::InvertIdxCurPosToTeachPos(int nCurPos, int &nCmdStage)
{
	if (nCurPos == CTaskLoadTable::eYPOS_SitePos1)
		nCmdStage = LOC_IDX_LD_TBL_TEST_SIDE_1;
	else if (nCurPos == CTaskLoadTable::eYPOS_SitePos2)
		nCmdStage = LOC_IDX_LD_TBL_TEST_SIDE_2;
	else if (nCurPos == CTaskLoadTable::eYPOS_SitePos3)
		nCmdStage = LOC_IDX_LD_TBL_TEST_SIDE_3;
	else if (nCurPos == CTaskLoadTable::eYPOS_SitePos4)
		nCmdStage = LOC_IDX_LD_TBL_TEST_SIDE_4;
	else
		nCmdStage = LOC_IDX_LD_TBL_TEST_SIDE_1;
}