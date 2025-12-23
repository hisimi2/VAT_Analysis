#include "StdAfx.h"
#include "PressUnitCmd_OnLineTestApple.h"
#include "TesterIF/TesterIF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPressUnitCmd_OnLineTestApple::CPressUnitCmd_OnLineTestApple(void)
{
	ZeroMemory(m_bSocketDvc, sizeof(m_bSocketDvc));

    m_nCmdId = eSeqCmd_PressUnit_OnlineTest;
	m_nUnitIdx=0;
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));

    m_bSotErr        = FALSE;
    m_bWaitResultErr = FALSE;
    m_bNotRevData    = FALSE;
	m_tcSotDelayTime = 0.0;

	m_nSiteCntX = 0;
	m_nSiteCntY = 0;
	m_nBinSortType = eSortType_SwBin;
	m_nHE_CategoryIdx = -1;
	m_bTesterErr = FALSE;

	m_nCmdStage = 0;
	m_tcTimeout = 0.0;
	m_tcMaxTestTimeout = 0.0;
	m_tcTesterTestTime = 0.0;

	m_tcNotTestingTimeout = 0.0;
	m_tcTesterNotTestingTime = 0.0;

	m_b2dMisMatch = FALSE;
}


CPressUnitCmd_OnLineTestApple::~CPressUnitCmd_OnLineTestApple(void)
{

}


//==============================================================================
//
//==============================================================================
int CPressUnitCmd_OnLineTestApple::CheckParam()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	m_nUnitIdx = pTaskPressUnit->m_nUnitIdx;

	m_nSiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	m_nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	m_tcMaxTestTimeout = (double)g_DMCont.m_dmHandlerOpt.GN(NDM11_SltPgm_TestTmOut);
	m_tcNotTestingTimeout = (double)g_DMCont.m_dmHandlerOpt.GN(NDM11_SltPgm_NotTestingTmOut);

	if (m_tcNotTestingTimeout <= 0.0)
		m_tcNotTestingTimeout = 30000.0;

	ST_DD_DEVICE stDvcData;

	BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
	BOOL bAutoLoopUse = g_TaskSystemCtrl.GetAutoLoopUse();

	if (bAutoGrrUse)
	{
		m_tcSotDelayTime = (double)g_DMCont.m_dmHandlerOpt.GN(NDM11_GRR_SOT_DELAY_TIME);
	}
	else {
		if (bAutoLoopUse != TRUE)
			m_tcSotDelayTime = (double)g_DMCont.m_dmHandlerOpt.GN(NDM11_SltPgm_SotDelayTm);
		else {
			m_tcSotDelayTime = (double)g_DMCont.m_dmHandlerOpt.GN(NDM11_LOOP_SOT_DELAY_TIME);
			int nMaxTestCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoLoopTestCount);

			int nRePickPlaceCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoRePickPlaceCount);
			int nReContinueCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoLoopTestRecontactCount);

			for (int nY = 0; nY < m_nSiteCntY; nY++)
			{
				for (int nX = 0; nX < m_nSiteCntX; nX++)
				{
					stDvcData.clear();
					pTaskPressUnit->GetSiteDeviceData(&stDvcData, nX, nY);

					stDvcData.stLoopTestInfo[m_nUnitIdx].nMaxTestCnt = nMaxTestCount;
					stDvcData.stLoopTestInfo[m_nUnitIdx].nSiteCntX = m_nSiteCntX;
					stDvcData.stLoopTestInfo[m_nUnitIdx].nSiteCntY = m_nSiteCntY;

					stDvcData.stLoopTestInfo[m_nUnitIdx].nRePickPlaceCnt = nRePickPlaceCnt;
					stDvcData.stLoopTestInfo[m_nUnitIdx].nReContinueCnt = nReContinueCnt;

					pTaskPressUnit->SetSiteDeviceData(&stDvcData, nX, nY);
				}
			}
		}
	}
	

	m_nBinSortType = g_DMCont.m_dmEQP.GN(NDM0_SortType);// enum eSortingType{   

	for (int i = 0; i < g_BinCategory.nIndexCount; i++)
	{
		CString strErr(g_BinCategory.aCateInfo[i].szDesc);
		if (strErr.Find(DEF_HANDLER_ERR_DEVICE) == -1) {
			m_nHE_CategoryIdx = i;
			break;
		}
	}

	memset(m_bSocketDvc, 0x00, sizeof(m_bSocketDvc));

	for (int nY = 0; nY < m_nSiteCntY; nY++)
	{
		for (int nX = 0; nX < m_nSiteCntX; nX++)
		{
			m_stDvcData[nX][nY].clear();
		}
	}

	m_nWorkCmdStage = m_nCmdStage;

	return ERR_NO_ERROR;
}


//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_OnLineTestApple::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_OnLineTestApple::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_OnLineTestApple::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_OnLineTestApple::OnPause()
{
	//SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_OnLineTestApple::OnResume()
{
	if(GetCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Pause || GetCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Alarm){
		SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	}
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_OnLineTestApple::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_OnLineTestApple::State_Executing()
{
	// check alarm
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		//pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;
    
	switch( m_nStep )
	{		
	case 0:			// check interlock
		{
			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d]",m_szCmdName ,m_nStep);
			
			g_DMCont.m_dmEQP.SN(NDM0_Press1_TestTm + m_nUnitIdx, 0);

			g_DMCont.m_dmEQP.SB(BDM0_TEST_STATUS_SITE1+m_nUnitIdx, TRUE);    // Set testing status on

			// step 2000번에서 -> 0 으로 시점 이동 [5/21/2025 donghyun.shin]
			::GetLocalTime(&m_sysTmTestStart);
			g_SOT[ePressUnit_Down_1 + m_nUnitIdx] = m_sysTmTestStart;
            // set flag initial
            FlagReset();
			
			//Single, Dual, Each 동일하게 사용
			int nSocketTesterSiteOnOff[eEachTesterSocket::eMaxTesterForYanmade] = { 0, };
			pTaskPressUnit->TesterSiteOnOffCheck(nSocketTesterSiteOnOff);

			// Test 결과 남아 있는 경우가 있어서, SOT 전에 Reset 
			g_AppleTestIF.m_TesterMgr.LastResultReset(m_nUnitIdx + 1, nSocketTesterSiteOnOff);

			ST_DD_DEVICE pDvcData;
			BOOL bExistInSite = FALSE;
			BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();

			for (int nY = 0; nY < m_nSiteCntY; nY++)
			{
				for (int nX = 0; nX < m_nSiteCntX; nX++)
				{
					pDvcData.clear();
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &pDvcData);

					//int Addr = nX + nY*STATION_MAX_X_SITE + m_nUnitIdx*STATION_MAX_PARA;
					//int nSiteOnOff = g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + Addr);

					int nSiteOnOff = (bAutoGrrUse == TRUE ? pTaskPressUnit->GetSocketOnOffGrr(nX, nY) : pTaskPressUnit->GetSocketOnOff(nX, nY)); // socket on/off 여부 확인.

					if (nSiteOnOff == DEF_SITE_ON && pDvcData.sExist == DEF_EXIST) {
						pDvcData.nRealTestingCnt++;
						g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &pDvcData);
						pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_TestStart);
					}
				}
			}

			pTaskPressUnit->SetTestStatusHistory(CTaskPressUnit::eSetPrepare);

			m_tcTesterTestTime = (double)GetTickCount64();
			m_nLast_Executing_Step = m_nStep = 1001;
		}break;

	case 1001: // 4: Query tester status
		{
			m_bWaitResultErr   = TRUE;
			double dSpanTime = (double)GetTickCount64() - m_tcTesterTestTime;
			if( dSpanTime > m_tcSotDelayTime )
			{
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [SOT]",m_szCmdName ,m_nStep);
				// index time ( SOT )
				BOOL bChkIndex = g_TaskSystemCtrl.m_tIndexTime[m_nUnitIdx].IsChecking();
				if( bChkIndex == TRUE){
					double dIndexTm = g_TaskSystemCtrl.m_tIndexTime[m_nUnitIdx].CompareTime(FALSE);
					g_DMCont.m_dmEQP.SD(DDM0_TmIndexPress1 + m_nUnitIdx, dIndexTm);
					g_TaskSystemCtrl.m_tIndexTime[m_nUnitIdx].SetTimerCheckEnd();

					CString strSummaryTime = _T("");
					int nIndexCount = g_DMCont.m_dmContac_Dn.GN(NDM1_INDEX_RECORD_IGNORE_STATION1 + m_nUnitIdx);
					if (nIndexCount <= MAX_IGNORE_INDEX_REC_COUNT || g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == TRUE || g_TaskSystemCtrl.IsOneCycleModeOn() == true)
					{
						g_DMCont.m_dmContac_Dn.SN(NDM1_INDEX_RECORD_IGNORE_STATION1 + m_nUnitIdx, ++nIndexCount);
					}
					else {
						double dStopTime = g_DMCont.m_dmContac_Dn.GD(DDM1_STOP_INDEX_STATION1 + m_nUnitIdx);
						int nCntAcc = g_DMCont.m_dmContac_Dn.GN(NDM1_INDEX_TIME_ACC_CNT_STATION1 + m_nUnitIdx);
						nCntAcc += 1;
						double dTimeAcc = g_DMCont.m_dmContac_Dn.GD(DDM1_INDEX_TIME_ACC_TIME_STATION1 + m_nUnitIdx);
						double dRecordTime = dIndexTm - dStopTime;
						double dSummaryIdx = dTimeAcc + dRecordTime;

						g_DMCont.m_dmContac_Dn.SD(DDM1_INDEX_TIME_ACC_TIME_STATION1 + m_nUnitIdx, dSummaryIdx);
						g_DMCont.m_dmContac_Dn.SN(NDM1_INDEX_TIME_ACC_CNT_STATION1 + m_nUnitIdx, nCntAcc);
						g_DMCont.m_dmContac_Dn.SD(DDM1_SUMMARY_INDEX_STATION1 + m_nUnitIdx, dSummaryIdx / nCntAcc);

						strSummaryTime.Format("Summary Index = %.2f [ (Acc=%.2f + Rec=%.2f(Index=%.2f-Stop=%2.f) / Cnt=%d]", dSummaryIdx / nCntAcc, dTimeAcc, dRecordTime, dIndexTm, dStopTime, nCntAcc);
					}
					g_DMCont.m_dmContac_Dn.SD(DDM1_STOP_INDEX_STATION1 + m_nUnitIdx, 0);

					pTaskPressUnit->MakeLog("Index Time, Press %d, %.2f, %s", m_nUnitIdx + 1, dIndexTm, strSummaryTime);
				}

				//Single, Dual, Each 동일하게 사용
				int nTesterSiteOnOff[eEachTesterSocket::eMaxTesterForYanmade] = { 0, };

				int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);

				pTaskPressUnit->TesterSiteOnOffCheck(nTesterSiteOnOff);

				m_tcTesterTestTime = (double)GetTickCount64();
				m_tcTesterNotTestingTime = (double)GetTickCount64();

				// change action tester
				int nRetStatus = g_AppleTestIF.SendAction(m_nUnitIdx+1, ACTION_STATUS,(WPARAM)NULL, (LPARAM)NULL, nTesterSiteOnOff, nTesterOption);

				if( nRetStatus > ERR_NO_ERROR ) // dut 별로 데이터 송신 하여 1개라도 이상 있을 경우에는 알람 발생.
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("[ Site %d ]", m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("Tester is disconnect!! please check tester and Handler..");
					ReportAlarm(ERR_TESTER_DISCONNECTED_DN1 + m_nUnitIdx, AlarmData);
					break;
				}

				m_nLast_Executing_Step = m_nStep++;
			}
                
		}break;
	case 1002: // Wait Status and information
		{
			int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
			if (nTesterOption == eTester_EachSocket)
			{
				int nSocketTesterSiteOnOff[eEachTesterSocket::eMaxTesterForYanmade] = { 0, };
				pTaskPressUnit->TesterSiteOnOffCheck(nSocketTesterSiteOnOff);
				int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
				CParamList AlarmData;

				char szCompany[16] = { 0, };
				g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

				double dSpanFullTestTm = (double)GetTickCount64() - m_tcTesterTestTime;
				if (dSpanFullTestTm > m_tcMaxTestTimeout)
				{
					AlarmData.m_fnAddItemFormat("[ Site %d ]", m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("Max test time over!! [OverTime-%.2f]", dSpanFullTestTm / 1000.0);
					AlarmData.m_fnAddItemFormat("Tester is not change status IDLE...");
					ReportAlarm(ERR_TESTER_MAX_TEST_TIME_OVER_DN1 + m_nUnitIdx, AlarmData);
					break;
				}

				int nTesterNum = 0;

				int nRet = g_AppleTestIF.GetStatus(m_nUnitIdx + 1, nSocketTesterSiteOnOff, nTesterOption, &nTesterNum); // -1: Error //  0: Init //	1: Idle //	2: Testing //	3: Error

				if (nRet == STATUS_ERROR)
				{
					AlarmData.m_fnAddItemFormat("[ Station %d ]", m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("handler receive from tester [tester status error ]");
					AlarmData.m_fnAddItemFormat("Handler send to tester query [ Action status ]");
					ReportAlarm(ERR_RECEIVE_TESTER_DN1 + m_nUnitIdx, AlarmData);
					break;
				}
				else if (nRet == RET_ERROR) {
					AlarmData.m_fnAddItemFormat("[ Site %d ]", m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("Tester is disconnect!! please check tester and Handler..");
					ReportAlarm(ERR_TESTER_DISCONNECTED_DN1 + m_nUnitIdx, AlarmData);
					break;
				}
				else if (nRet == STATUS_IDLE) {
					FlagReset();
					m_tcTesterTestTime = (double)GetTickCount64();
					m_tcTesterNotTestingTime = (double)GetTickCount64();
					m_nLast_Executing_Step = m_nStep = 2000;
				}
			}
			else
			{
				int nDualTesterSiteOnOff[eMaxTester] = { 0, };
				pTaskPressUnit->TesterSiteOnOffCheck(nDualTesterSiteOnOff);
				int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
				CParamList AlarmData;

				char szCompany[16] = { 0, };
				g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

				double dSpanFullTestTm = (double)GetTickCount64() - m_tcTesterTestTime;
				if (dSpanFullTestTm > m_tcMaxTestTimeout)
				{
					AlarmData.m_fnAddItemFormat("[ Site %d ]", m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("Max test time over!! [OverTime-%.2f]", dSpanFullTestTm / 1000.0);
					AlarmData.m_fnAddItemFormat("Tester is not change status IDLE...");
					ReportAlarm(ERR_TESTER_MAX_TEST_TIME_OVER_DN1 + m_nUnitIdx, AlarmData);
					break;
				}

				int nTesterNum = 0;

				int nRet = g_AppleTestIF.GetStatus(m_nUnitIdx + 1, nDualTesterSiteOnOff, nTesterOption, &nTesterNum); // -1: Error //  0: Init //	1: Idle //	2: Testing //	3: Error

				if (nRet == STATUS_ERROR)
				{
					AlarmData.m_fnAddItemFormat("[ Site %d ]", m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("handler receive from tester [tester status error ]");
					AlarmData.m_fnAddItemFormat("Handler send to tester query [ Action status ]");
					ReportAlarm(ERR_RECEIVE_TESTER_DN1 + m_nUnitIdx, AlarmData);
					break;
				}
				else if (nRet == RET_ERROR) {
					AlarmData.m_fnAddItemFormat("[ Site %d ]", m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("Tester is disconnect!! please check tester and Handler..");
					ReportAlarm(ERR_TESTER_DISCONNECTED_DN1 + m_nUnitIdx, AlarmData);
					break;
				}
				else if (nRet == STATUS_IDLE) {
					FlagReset();
					m_tcTesterTestTime = (double)GetTickCount64();
					m_tcTesterNotTestingTime = (double)GetTickCount64();
					m_nLast_Executing_Step = m_nStep = 2000;
				}
			}
//#endif
		}break;
	case 2000: // Start Testing == same gpib srq41
		{
			/* Action
			1: Start test
			2: Stop test
			3: Reset
			4: Query tester status
			5: Query tester information */
            CParamList AlarmData;

			char szCompany[16] = { 0, };
			g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));


			double dSpanFullTestTm = (double)GetTickCount64() - m_tcTesterTestTime;

			if( dSpanFullTestTm > m_tcMaxTestTimeout )
			{
				AlarmData.m_fnAddItemFormat("[ Site %d ]", m_nUnitIdx+1);
                AlarmData.m_fnAddItemFormat("Max test time over!! [OverTime-%.2f]", dSpanFullTestTm/1000.0);
                AlarmData.m_fnAddItemFormat("Handler send to query[Start test], but Tester is not the answer.");
				ReportAlarm(ERR_TESTER_MAX_TEST_TIME_OVER_DN1+m_nUnitIdx, AlarmData);
				break;
			}

			// Process ID
			char szRetBuff[MAX_PATH] = { 0, };
			g_DMCont.m_dmHandlerOpt.GS(SDM11_ScheduleNo, szRetBuff, MAX_PATH);

			// make send information
			ST_TESTER_ACTION_PARAM stActionParam[ STATION_MAX_PARA ];//stActionParam;
			ZeroMemory( stActionParam, sizeof( ST_TESTER_ACTION_PARAM ) * STATION_MAX_PARA );
			int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
			CTesterStatus cTesterStatus = g_AppleTestIF.GetTesterStatus(m_nUnitIdx+1, nTesterOption);
			BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();

			ST_DD_DEVICE pDvcData;

			for( int nY=0; nY < m_nSiteCntY; nY++ )
			{
				for( int nX=0; nX < m_nSiteCntX; nX++ )
				{
					pDvcData.clear();

					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &pDvcData);

					int nTargetNo = (nX + nY*STATION_MAX_X_SITE);

					int nSiteOnOff = (bAutoGrrUse == TRUE ? pTaskPressUnit->GetSocketOnOffGrr(nX, nY) : pTaskPressUnit->GetSocketOnOff(nX, nY));

					if( nSiteOnOff != DEF_SITE_ON || pDvcData.sExist == DEF_NOT_EXIST || strcmp(pDvcData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0)
					{ 
						stActionParam[ nTargetNo ].m_bSiteOn = false;
						continue;
					}

					//Socket Device Data Flag
					m_bSocketDvc[nX][nY] = TRUE;

					m_stDvcData[nX][nY] = pDvcData;

					// Site On/Off
					stActionParam[ nTargetNo ].m_bSiteOn = true;
					// Lot ID
					g_DMCont.m_dmHandlerOpt.GS( SDM11_LotNo, stActionParam[ nTargetNo ].m_cLotNum, sizeof( stActionParam[ nTargetNo ].m_cLotNum ) ); 

					if (nTesterOption == eTester_EachSocket)
					{
						// setting send data // barcode number
						sprintf(stActionParam[nTargetNo].m_c2DID, "%s", pDvcData.sz2DBarcode);
					}
					else
					{
						// Tester ID
						sprintf(stActionParam[nTargetNo].m_cTesterID, "%s", cTesterStatus.m_sTesterID_Orginal.GetString());
						// Socket ID
						sprintf(stActionParam[nTargetNo].m_cSocketID, "%s", cTesterStatus.m_sSocketID_Orginal[nTargetNo].GetString());
						// setting send data // barcode number
						sprintf(stActionParam[nTargetNo].m_c2DID, "%s", pDvcData.sz2DBarcode);

// 						sprintf(stActionParam[nTargetNo].m_cFTRT, "%d", pDvcData.nTestingCnt);
// 
// 						sprintf(stActionParam[nTargetNo].m_cRTCode, "%s", szRetBuff);
					}
					
					pTaskPressUnit->MakeLog( "[SND H->T %d-%d-%d 2D Barcode Id = %s", m_nUnitIdx+1, nX+1, nY+1, pDvcData.sz2DBarcode );

					char szCompany[16] = { 0, };
					g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

					//if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
						//Socket ID 별 Contact Count
					if (nTesterOption == eTester_EachSocket)
					{
						CString strIndentityDutNo;
						strIndentityDutNo.Format("%d", cTesterStatus.m_nRegiDutIdentityNo[nTargetNo]);
						g_TaskSystemCtrl.AddSocketIDContactCnt(strIndentityDutNo);
					}
					else {
						g_TaskSystemCtrl.AddSocketIDContactCnt(cTesterStatus.m_sSocketID_Orginal[nTargetNo]);
					}
					 // contact 횟수 증가 시킨다.
					//}
				}
			}
			int nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	
			::GetLocalTime(&m_sysTmTestStart);
// 시점 이동  [5/21/2025 donghyun.shin] //			
//			g_EOT[ePressUnit_Down_1 + m_nUnitIdx] = m_sysTmTestStart;

			//Loading Table 미리 대기를 위한 Contact PrePare 상태 변경.
			g_DMCont.m_dmContac_Up.SN(NDM2_PRESS_1_PRE_CONTACT_STATUS + m_nUnitIdx, FALSE);

			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Send Information to Tester]",m_szCmdName ,m_nStep);


			//Single, Dual, Each 동일하게 사용
			int nTesterSiteOnOff[eEachTesterSocket::eMaxTesterForYanmade] = { 0, };

			pTaskPressUnit->TesterSiteOnOffCheck(nTesterSiteOnOff); // 해당 station의 dut on/off 상태 확인.

			int nRet = g_AppleTestIF.SendAction(m_nUnitIdx+1, ACTION_START, (WPARAM)stActionParam, (LPARAM)NULL, nTesterSiteOnOff, nTesterOption);
			m_bSotErr   = TRUE;
			if( nRet != ERR_NO_ERROR)
			{
                int nRetStatus = g_AppleTestIF.GetStatus(m_nUnitIdx+1, nTesterSiteOnOff, nTesterOption);
                if( nRetStatus == RET_ERROR){
					AlarmData.m_fnAddItemFormat("[ Station %d ]", m_nUnitIdx+1);
                    AlarmData.m_fnAddItemFormat("Tester is disconnect!! please check tester and Handler..");
                    ReportAlarm(ERR_TESTER_DISCONNECTED_DN1+m_nUnitIdx, AlarmData);
                    break;
                }
                
                if( nRetStatus == STATUS_ERROR){
					AlarmData.m_fnAddItemFormat("[ Station %d ]", m_nUnitIdx+1);
                    AlarmData.m_fnAddItemFormat("handler receive from tester [tester status error ]");
                    AlarmData.m_fnAddItemFormat("Handler send to tester query [ Action status ]");
                    ReportAlarm(ERR_RECEIVE_TESTER_DN1+m_nUnitIdx, AlarmData);
                    break;
                }
                
                AlarmData.m_fnAddItemFormat("Action Start send error");
				ReportAlarm(ERR_TEST_SITE_SEND_ACTION_DN1+m_nUnitIdx, AlarmData);
				break;
			}else{
				m_tcTimeout = m_tcTesterTestTime = (double)GetTickCount64();
				m_nLast_Executing_Step = m_nStep++;
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Wait Test Start]",m_szCmdName ,m_nStep);
			}			
		}break;
	case 2001: // Wait Test Start
		{
			double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
            g_DMCont.m_dmEQP.SN(NDM0_Press1_TestTm+m_nUnitIdx, (int)dSpanTime);
            CParamList AlarmData;

			int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);

			//Single, Dual, Each 동일하게 사용
			int nDualTesterSiteOnOff[eEachTesterSocket::eMaxTesterForYanmade] = { 0, };

			pTaskPressUnit->TesterSiteOnOffCheck(nDualTesterSiteOnOff);

			double dSpanFullTestTm = (double)GetTickCount64() - m_tcTesterTestTime;
			if( dSpanFullTestTm > m_tcMaxTestTimeout )
			{
				AlarmData.m_fnAddItemFormat("[ Station %d ]", m_nUnitIdx+1);
                AlarmData.m_fnAddItemFormat("Max test time over!! [OverTime %.2f]", dSpanFullTestTm/1000.0);
                AlarmData.m_fnAddItemFormat("Handler send to query[Start test], but Tester is not the answer.");
				ReportAlarm(ERR_TESTER_MAX_TEST_TIME_OVER_DN1+m_nUnitIdx, AlarmData);
				break;
			}

			int nRet = g_AppleTestIF.GetStatus(m_nUnitIdx+1, nDualTesterSiteOnOff, nTesterOption);
			if( nRet == STATUS_ERROR){
				AlarmData.m_fnAddItemFormat("[ Station %d ]", m_nUnitIdx+1);
                AlarmData.m_fnAddItemFormat("handler receive from tester [tester status error ]");
                AlarmData.m_fnAddItemFormat("Handler send to tester query [ Action status ]");
				ReportAlarm(ERR_RECEIVE_TESTER_DN1+m_nUnitIdx, AlarmData);
				break;
			}
            if( nRet == RET_ERROR){
				AlarmData.m_fnAddItemFormat("[ Station %d ]", m_nUnitIdx+1);
                AlarmData.m_fnAddItemFormat("Tester is disconnect!! please check tester and Handler..");
                ReportAlarm(ERR_TESTER_DISCONNECTED_DN1+m_nUnitIdx, AlarmData);
                break;
            }

			// Tester 상태 History Setting
			pTaskPressUnit->SetTestStatusHistory(CTaskPressUnit::eSetStatus);


			// Receive 받은게 있으면 다음 Step으로 가서 처리한다.
			if( nRet == STATUS_TESTING){
                FlagReset();
				/*m_tcTimeout = */m_tcTesterTestTime = (double)GetTickCount64();
				m_nLast_Executing_Step = m_nStep++;

				//TestStatusHistory여기서 처리

				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Wait Test Result]",m_szCmdName ,m_nStep);
			}
		}break;
	case 2002: // Wait Test Result
		{
			double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
            double dSpanTestTime = (double)GetTickCount64()- m_tcTesterTestTime;
            g_DMCont.m_dmEQP.SN(NDM0_Press1_TestTm+m_nUnitIdx, (int)dSpanTime);
            CParamList AlarmData;
            m_bWaitResultErr = TRUE;
			
// #if DEF_TESTER_COMM_SPEC_ONE_SOCKET
// 			int nTesterSiteOnOff[eEachTesterSocket::eMaxTesterForYanmade] = { 0, };
// #else
// 			int nTesterSiteOnOff[eMaxTester] = { 0, };
// #endif

			//Single, Dual, Each 동일하게 사용
			int nTesterSiteOnOff[eEachTesterSocket::eMaxTesterForYanmade] = { 0, };

			pTaskPressUnit->TesterSiteOnOffCheck(nTesterSiteOnOff);
			int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);

			if( dSpanTestTime > m_tcMaxTestTimeout )
			{
				pTaskPressUnit->SetTestTimeOutHandlerErrorBin(m_nCmdStage, m_sysTmTestStart);

				AlarmData.m_fnAddItemFormat("[ Station %d ]", m_nUnitIdx+1);
                AlarmData.m_fnAddItemFormat("Max test time over!! [OverTime-%.2f]", dSpanTime/1000.0);
                AlarmData.m_fnAddItemFormat("Handler send to query[Start test], but Tester is not the answer.");
				//ReportAlarm(ERR_TESTER_MAX_TEST_TIME_OVER_DN1+m_nUnitIdx, AlarmData);
				g_Error.AlarmReport(ERR_TESTER_MAX_TEST_TIME_OVER_DN1 + m_nUnitIdx, AlarmData, &g_TaskSystemCtrl);
				m_nLast_Executing_Step = m_nStep = 5000;
				SetCmdStatus(eSeqCmdStatus_Alarm);
				g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);

				//ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
				break;
			}

			int nRet = g_AppleTestIF.GetStatus(m_nUnitIdx+1, nTesterSiteOnOff, nTesterOption);
			if( nRet == STATUS_ERROR)
			{
				AlarmData.m_fnAddItemFormat("[ Station %d ]", m_nUnitIdx+1);
                AlarmData.m_fnAddItemFormat("handler receive from tester [tester status error ]");
                AlarmData.m_fnAddItemFormat("Handler send to tester query [ Action status ]");
				ReportAlarm(ERR_RECEIVE_TESTER_DN1+m_nUnitIdx, AlarmData);
				break;
			}
            if( nRet == RET_ERROR){
				AlarmData.m_fnAddItemFormat("[ Station %d ]", m_nUnitIdx+1);
                AlarmData.m_fnAddItemFormat("Tester is disconnect!! please check tester and Handler..");
                ReportAlarm(ERR_TESTER_DISCONNECTED_DN1+m_nUnitIdx, AlarmData);
                break;
            }

			// Receive 받은게 있으면 다음 Step으로 가서 처리한다.
			if( nRet != STATUS_TESTING && nRet != STATUS_RESULT_WAIT ){			
                FlagReset();
                m_tcTesterTestTime = (double)GetTickCount64();
				m_nLast_Executing_Step = m_nStep = 4000;			
			}
 		}break;
	case 4000:
		{
			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Change test Status(Test End)]",m_szCmdName ,m_nStep);

			//ST_DD_DEVICE pDvcData;
			int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
			char szCompany[16] = { 0, };
			g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

			// Summary 기록할 평균 Test time
			if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0)
			{
				int nSpanTime = g_DMCont.m_dmEQP.GN(NDM0_Press1_TestTm + m_nUnitIdx);
				int nCntAcc = g_DMCont.m_dmContac_Up.GN(NDM2_TEST_TIME_ACC_CNT_STATION1 + m_nUnitIdx);
				int nTimeAccAvg = g_DMCont.m_dmContac_Up.GN(NDM2_TEST_TIME_ACC_TIME_AVG_STATION1 + m_nUnitIdx);
				//평균 x 횟수 = 총 시간
				int nAccTime = nTimeAccAvg * nCntAcc + nSpanTime;
				nCntAcc++;

				g_DMCont.m_dmContac_Up.SN(NDM2_TEST_TIME_ACC_CNT_STATION1 + m_nUnitIdx, nCntAcc);
				g_DMCont.m_dmContac_Up.SN(NDM2_TEST_TIME_ACC_TIME_AVG_STATION1 + m_nUnitIdx, nAccTime / nCntAcc);
			}

			//Single, Dual, Each 동일하게 사용
			int nDirSiteOnOff[eEachTesterSocket::eMaxTesterForYanmade] = { 0, };

			pTaskPressUnit->TesterSiteOnOffCheck(nDirSiteOnOff);

			ResultData rData;
			int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
			BOOL bRet = g_AppleTestIF.GetResultDataFromTester(m_nUnitIdx+1, &rData, nDirSiteOnOff, nTesterOption);

			if( bRet == FALSE )
			{
				double dSpanTestTime = (double)GetTickCount64() - m_tcTesterTestTime;
				if( dSpanTestTime <= m_tcMaxTestTimeout )
				{
					break;
				}
				else {
					m_bNotRevData = TRUE;
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("[ station %d ]", m_nUnitIdx + 1);
					AlarmData.m_fnAddItemFormat("Tester result data error!!");
					ReportAlarm(ERR_TESTER_RECEIVE_DATA_DN1 + m_nUnitIdx, AlarmData);
					break;
				}
			}

			// == Barcode MissMatch
			ST_DD_DEVICE pReadData;
			CString strErr = _T(""), strBarcodeErr=_T(""), strConfirmMissID = _T(""), strMissMatchBarcodeSiteNumber = _T("");;

			CTesterStatus cTesterStatus = g_AppleTestIF.GetTesterStatus(m_nUnitIdx+1, nTesterOption);

			BOOL bAutoGrrUseMode = g_TaskSystemCtrl.GetAutoGrrUse();
			BOOL bSwapOption = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrDeviceSwapUse);

			int n2DIDErrorSiteNumber[STATION_MAX_PARA] = {0,};

			// check receive data
			for(int nY=0; nY< m_nSiteCntY; nY++)
			{
				for(int nX=0; nX< m_nSiteCntX; nX++)
				{
					pReadData.clear();
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1+m_nUnitIdx, nX, nY, &pReadData);
					//int nSiteOnOff = pTaskPressUnit->GetSocketOnOff(nX, nY);
					int nSiteOnOff = (bAutoGrrUseMode == TRUE ? pTaskPressUnit->GetSocketOnOffGrr(nX, nY) : pTaskPressUnit->GetSocketOnOff(nX, nY));

					if(    nSiteOnOff == DEF_SITE_ON 
						&& pReadData.sExist == DEF_EXIST
						&& strcmp(pReadData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) != 0)
					{
						int nPos = (nX+STATION_MAX_X_SITE*nY);
						// not receive data
						if( rData.m_nTestResult[ (nX+STATION_MAX_X_SITE*nY) ] == -1) // Tester에서 수신한 데이터 확인.
						{
							m_bNotRevData = TRUE;
							CString strResErr = _T("");
							strResErr.Format("[%d Site=%d-%d, receive data error!!]", m_nUnitIdx + 1, nX + 1, nY + 1);
							strErr += "\n";
							strErr += strResErr;
						}

						// Barcode miss match	
						if(strcmp(rData.m_cDutSn[ nPos ], pReadData.sz2DBarcode) != 0){	// 핸들러 shard memory data와 테스터 수신 데이터 불일치.
							m_b2dMisMatch = TRUE;
							CString strMissMatchBarcode = _T(""), strSiteNumber = _T("");
							strMissMatchBarcode.Format("[%d Site=%d-%d, Tester2D=%s, Read2D=%s]", m_nUnitIdx + 1, nX+1, nY+1, rData.m_cDutSn[nPos], pReadData.sz2DBarcode );
							n2DIDErrorSiteNumber[nX + (nY  * STATION_MAX_X_SITE)] = TRUE;
							strBarcodeErr += "\n";
							strBarcodeErr += strMissMatchBarcode;
						}
					}
				}
			}
			if( strErr.IsEmpty() != TRUE )
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("[ station %d ] Tester result data error!!", m_nUnitIdx+1);
				AlarmData.m_fnAddItemFormat("[ %s ]", strErr);
				ReportAlarm(ERR_TESTER_RECEIVE_DATA_DN1+m_nUnitIdx, AlarmData);
				break;
			}

			if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use) == TRUE) { // SLT 2DID 검사 하지 않는 조건 추가
				if (strBarcodeErr.IsEmpty() != TRUE)
				{
					// Marlin에 경우 2DID 알람 발생해도 전체 FAIL 처리 하면 안됨. [4/8/2021 donghyun.shin]

					pTaskPressUnit->SetTest2DIDHandlerErrorBin(m_nCmdStage, m_sysTmTestStart, n2DIDErrorSiteNumber);
					CParamList AlarmData;
		 			AlarmData.m_fnAddItemFormat("[ station %d ]Tester result 2D barcode miss match!!!", m_nUnitIdx + 1);
		 			AlarmData.m_fnAddItemFormat("[ %s ]", strBarcodeErr);
					g_Error.AlarmReport(ERR_2D_MISMATCH_DN1 + m_nUnitIdx, AlarmData, &g_TaskSystemCtrl);
					m_nLast_Executing_Step = m_nStep = 5000;
					SetCmdStatus(eSeqCmdStatus_Alarm);
					g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
					//ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
					break;
				}
			}
				
			if( strConfirmMissID.IsEmpty() != TRUE)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("[ Station=%d ] Confirm miss error", m_nUnitIdx+1);
				AlarmData.m_fnAddItemFormat("[ %s ]", strConfirmMissID);
				ReportAlarm(ERR_TESTER_RECEIVE_DATA_DN1+m_nUnitIdx, AlarmData);
				break;
			}
			if( m_bWaitResultErr == TRUE )
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("[ ERROR ] Handler receive from Tester");
				AlarmData.m_fnAddItemFormat("[ ERROR ] Please reboot Station(%d) Tester", m_nUnitIdx+1);
				ReportAlarm(ERR_RECEIVE_TESTER_DN1+m_nUnitIdx, AlarmData);
				break;
			}

			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Data Handling]",m_szCmdName ,m_nStep);
			// change test status

			//꺼내온 Data가지고 처리한다. // Data 처리			
			BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
			BOOL bGrrCancel = g_TaskSystemCtrl.GetAutoGrrCancel();
			BOOL bLoopCancel = g_TaskSystemCtrl.GetAutoLoopCancel();

			int nGrrMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrMode);
						
			
			// Get local time
			SYSTEMTIME timeEnd;
			::GetLocalTime( &timeEnd );
			g_EOT[ePressUnit_Down_1 + m_nUnitIdx] = timeEnd;

			//Error code compare result data
			CString strTRL = _T(""), strStartTm = _T(""), strEndTm = _T("");
			char szHWbin[16] = {0,};			
			int  nBinColor = -1;

			BOOL bAllSiteFail = TRUE;
			BOOL bAutoLoopUseMode = g_TaskSystemCtrl.GetAutoLoopUse();
			int nLoopMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoLoopMode);

			int nTestKindCnt[3] = { 0, }, nTestPassKindCnt[3] = { 0, }; // Daily Report data 0 : FT, 1: RT1, 2: RT2

			for(int nY=0; nY< m_nSiteCntY && m_bNotRevData == FALSE ; nY++)
			{
				for(int nX=0; nX< m_nSiteCntX && m_bNotRevData == FALSE; nX++)
				{
gotoTestResultRetry:
					pReadData.clear();
					pTaskPressUnit->GetSiteDeviceData(&pReadData, nX, nY);

					int nSiteOnOff = (bAutoGrrUseMode == TRUE ? pTaskPressUnit->GetSocketOnOffGrr(nX, nY) : pTaskPressUnit->GetSocketOnOff(nX, nY));


					int nTargetPos = nX + nY*STATION_MAX_X_SITE;

					if( nSiteOnOff == DEF_SITE_ON && pReadData.sExist == DEF_EXIST && strcmp(pReadData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) != 0 )
					{
						BOOL bChkRegCateInfo = FALSE;
						// case 1 :: category에 등록이 되어있는 경우
						for(int i=0; i < g_BinCategory.nIndexCount; i++){
							if( strcmp(g_BinCategory.aCateInfo[i].szDesc, rData.m_cErrorStr[nTargetPos] ) == 0 )
							{
								strcpy_s(pReadData.szBinResult, sizeof(pReadData.szBinResult), g_BinCategory.aCateInfo[i].szSWBin);
								sprintf_s(szHWbin, sizeof(szHWbin), g_BinCategory.aCateInfo[i].szHWBin);
								pReadData.nBinColor = rData.m_nTestResult[nTargetPos]; /*Pass/Fail 받은 것*/
								strcpy_s(pReadData.szErrString, sizeof(pReadData.szErrString), rData.m_cErrorStr[nTargetPos]);
								strcpy_s(pReadData.szErrorCode, sizeof(pReadData.szErrorCode), rData.m_cErrorCode[nTargetPos]);
								strcpy_s(pReadData.szTesterHWBin, sizeof(pReadData.szTesterHWBin), g_BinCategory.aCateInfo[i].szHWBin_Tester);
								nBinColor = pReadData.nBinColor;
								bChkRegCateInfo = TRUE;
							}
						}
						// case 2 :: category에 등록이 안되어 있는 경우
						if(bChkRegCateInfo == FALSE){
							for(int i=0; i < g_BinCategory.nIndexCount; i++){
								if(atoi(g_BinCategory.aCateInfo[i].szSWBin) == rData.m_nTestResult[nTargetPos]){
									strcpy_s(pReadData.szBinResult, sizeof(pReadData.szBinResult), g_BinCategory.aCateInfo[i].szSWBin);
									sprintf_s(szHWbin, sizeof(szHWbin), g_BinCategory.aCateInfo[i].szHWBin);
									// Bin Color = Msg Data Pass/Fail
									nBinColor = pReadData.nBinColor = rData.m_nTestResult[nTargetPos]; /*Pass/Fail 받은 것*/
									strcpy_s(pReadData.szErrString, sizeof(pReadData.szErrString), rData.m_cErrorStr[nTargetPos]);
									strcpy_s(pReadData.szErrorCode, sizeof(pReadData.szErrorCode), rData.m_cErrorCode[nTargetPos]);
									strcpy_s(pReadData.szTesterHWBin, sizeof(pReadData.szTesterHWBin), g_BinCategory.aCateInfo[i].szHWBin_Tester);
								}
							}
						}
						// Record Test Result log file
						strStartTm.Format("%04d%02d%02d%02d%02d%02d%03d", m_sysTmTestStart.wYear, m_sysTmTestStart.wMonth, m_sysTmTestStart.wDay, m_sysTmTestStart.wHour, m_sysTmTestStart.wMinute, m_sysTmTestStart.wSecond, m_sysTmTestStart.wMilliseconds);
						strEndTm.Format("%04d%02d%02d%02d%02d%02d%03d"  , timeEnd.wYear, timeEnd.wMonth, timeEnd.wDay, timeEnd.wHour, timeEnd.wMinute, timeEnd.wSecond, timeEnd.wMilliseconds);
						

						/* Product CSV foramt [ AMKOR ]
						1. Handler Send Barcode : SLT Handler에서 읽은 barcode
						2. Tester Send Barcode : Tester 에서 결과로 보내준 barcode
						3. SLT ID : Handler hostname
						4. Station ID : Station No ( 1 ~ 16 )
						5. Site ID : site No ( 1 ~ 4 ) // MP의 경우는 1 ~ 8
						6. Tester ID : TesterID
						7. Socket ID : Socket ID
						8. Tester Start/End time : 테스트 시작 시간, 테스트 종료 시간( Bin 결과 받은 시간 )
						9. Test Result :Tester로부터 받은 Pass/Fail
						10. Test ErrorString : Tester로 받은 ErrorString
						11. HW Bin : SLT Handler에 sort 될 unload #No 위치
						*/
						
						char szEqpName[128] = {0,};
						g_DMCont.m_dmHandlerOpt.GS(SDM11_UDP_HOST_NAME, szEqpName, sizeof(szEqpName));

						unsigned int nStationID;

						if (nTesterOption == eTester_Single)
							nStationID = rData.m_nStationID;
						else if (nTesterOption == eTester_Dual)
							nStationID = rData.m_nStationID / 2 + 1;
						else
							nStationID = m_nUnitIdx + 1;	// station No. 지정 한다.
						
						int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);

						//송도 Amkor 요청사항. 대문자 -> 소문자 변환
						CString strBinResult = (rData.m_nTestResult[nTargetPos] == eGoodBin) ? _T("PASS") : _T("FAIL");
						strBinResult.MakeLower();

//						if (nTesterOption == eTester_EachSocket)
						{
							CString strStationID = _T("");
							nStationID = (((m_nSiteCntX*m_nSiteCntY)*m_nUnitIdx) + nX + nY*m_nSiteCntX) +1;
							CTesterStatus cTesterStatus = g_AppleTestIF.GetTesterStatus(nStationID);
							strStationID.Format("%d", cTesterStatus.m_nStationID);

							strTRL.Format(_T("%s,%s,%s,%s,%s,%s,%s")
								, pReadData.sz2DBarcode
								, strStationID
								, (LPCTSTR)strStartTm, (LPCTSTR)strEndTm				
								, strBinResult/*(rData.m_nTestResult[nTargetPos] == eGoodBin ? _T("PASS") : _T("FAIL"))*/
								, rData.m_cErrorStr[nTargetPos], szHWbin);
 						}
// 						else
// 						{
// 							strTRL.Format(_T("%s,%s,%s,%u,%d,%s,%s,%s,%s,%s,%s,%s")
// 								, pReadData.sz2DBarcode, rData.m_cDutSn[nTargetPos], szEqpName
// 								, nStationID, nTargetPos + 1
// 								, (LPCTSTR)(cTesterStatus.m_sTesterID_Orginal), (LPCTSTR)(cTesterStatus.m_sSocketID_Orginal[nTargetPos]), (LPCTSTR)strStartTm, (LPCTSTR)strEndTm
// 								, strBinResult/*(rData.m_nTestResult[nTargetPos] == eGoodBin ? _T("PASS") : _T("FAIL"))*/
// 								, rData.m_cErrorStr[nTargetPos], szHWbin);
// 						}

						TWLOG.MakeLog(static_cast<int>(eLogID_PRODUCTCSV), Debug, "", __LINE__, NULL, strTRL);

						sprintf_s(pReadData.szTestSiteNum, sizeof(pReadData.szTestSiteNum),"%d", m_nUnitIdx);
						pReadData.cSubSiteNum = CPoint(nX,nY);
												
						// Set Testing Count
						int nTestCnt = pReadData.nTestingCnt;
						if (bAutoGrrUseMode != TRUE)
						{	// JAM RATE DATA RECORD
							if (bAutoLoopUseMode != TRUE)
							{
								ASSERT(nTestCnt >= eARB_NotTest && nTestCnt <= eARB_3stTestEnd);
								int nDvcTestCnt = nTestCnt;
								if (bAutoRetestUseMode == TRUE && g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoRetest) == eAutoRetest_AB && nTestCnt > eARB_NotTest) {
									nDvcTestCnt -= 1; //AB의 경우는 1stEnd 후에 바로 2stEnd로 바꾸어주기 때문에 빼야함.
								}
								pReadData.stRecInfo.stDvcInfo[nDvcTestCnt].nHwBin = atoi(szHWbin) - 1;
								pReadData.stRecInfo.stDvcInfo[nDvcTestCnt].nPF = nBinColor;
								pReadData.stRecInfo.stDvcInfo[nDvcTestCnt].nSiteNo = atoi(pReadData.szTestSiteNum);
								pReadData.stRecInfo.stDvcInfo[nDvcTestCnt].cSubSiteNo = pReadData.cSubSiteNum;
								sprintf_s(pReadData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrStr, sizeof(pReadData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrStr), rData.m_cErrorStr[nTargetPos]);
								sprintf_s(pReadData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrCode, sizeof(pReadData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrCode), rData.m_cErrorCode[nTargetPos]);
								sprintf_s(pReadData.stRecInfo.stDvcInfo[nDvcTestCnt].szTesterHwBin, sizeof(pReadData.stRecInfo.stDvcInfo[nDvcTestCnt].szTesterHwBin), pReadData.szTesterHWBin);

								int nArtCnt = g_DMCont.m_dmEQP.GN(NDM0_ART_FT_TOTAL + (nTestCnt * 2));
								g_DMCont.m_dmEQP.SN(NDM0_ART_FT_TOTAL + (nTestCnt * 2), ++nArtCnt);
								if (nBinColor == eGoodBin) {
									nArtCnt = g_DMCont.m_dmEQP.GN(NDM0_ART_FT_PASS + (nTestCnt * 2));
									g_DMCont.m_dmEQP.SN(NDM0_ART_FT_PASS + (nTestCnt * 2), ++nArtCnt);
									nTestPassKindCnt[nTestCnt]++;
								}

								nTestKindCnt[nTestCnt]++;

								pTaskPressUnit->PushYieldData(nX, nY, nBinColor, nTestCnt);
							}
							else {					
								if (bLoopCancel == FALSE)
									pReadData.stLoopTestInfo[m_nUnitIdx].testDoneAll();
								else {
									int nMaxTestCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoLoopTestCount);
									for (int nTestCnt = pReadData.stLoopTestInfo[m_nUnitIdx].nTestCnt[nX][nY]; nTestCnt < nMaxTestCount; nTestCnt++)
									{
										pReadData.stLoopTestInfo[m_nUnitIdx].testDoneAll();
									}
								}							
							}
						}
						else {
							//if (strcmp(pReadData.szErrString, DEF_TEST_RESULT_TP101) != 0)
							//{
								int nFindIdx = 0;
								nFindIdx = pReadData.findIndexTestSite(pReadData.szTestSiteNum);
								ASSERT(nFindIdx >= 0);								
								
								if (bGrrCancel == TRUE) {
									int nMaxTestCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrTestCount);
									for (int nTestCnt = pReadData.stGrrTestInfo[nFindIdx].nTestCnt[nX][nY]; nTestCnt < nMaxTestCount; nTestCnt++)
									{
										pReadData.stGrrTestInfo[nFindIdx].testDoneAll();
									}
								}
								else {
									if (bSwapOption == TRUE) {
										pReadData.stGrrTestInfo[nFindIdx].testDone(nX, nY);
									}
									else {
										pReadData.stGrrTestInfo[nFindIdx].testDoneAll();
									}
								}

								// Create DB Data
								_tGRR_DATA stGrrData;
								sprintf(stGrrData.szUnique, "%s", pReadData.szUniqueID);
								sprintf(stGrrData.sz2DID, "%s", pReadData.sz2DBarcode);
								sprintf(stGrrData.szSiteNum, "%d", atoi(pReadData.szTestSiteNum) + 1);
								sprintf(stGrrData.szSubSiteNo, "%d", (nX + 1 + (nY * STATION_MAX_X_SITE)));
								stGrrData.nTestCntNo = pReadData.stGrrTestInfo[nFindIdx].nTestCnt[nX][nY];
								sprintf(stGrrData.szTestResult, (atoi(pReadData.szBinResult) == eGoodBin ? "PASS" : "FAIL"));
								sprintf(stGrrData.szResultDescription, "%s", rData.m_cErrorStr[nTargetPos]);

								g_DB.ASYNC_INSERT_GRRDATA_TBL(stGrrData);

								char szGrrSiteHis[MAX_PATH] = { 0, };
								g_DMCont.m_dmContac_Dn.GS(SDM1_SiteGrrPF_History1_1 + m_nUnitIdx*STATION_MAX_PARA + nX + nY * STATION_MAX_X_SITE, szGrrSiteHis, sizeof(szGrrSiteHis));
								CString strGrrSiteHis = _T("");
								strGrrSiteHis.Format("%s%s", szGrrSiteHis, (atoi(pReadData.szBinResult) == eGoodBin ? "P" : "F"));
								int nlength = strGrrSiteHis.GetLength();
								if (nlength >= MAX_STRING_LENGTH)
									g_DMCont.m_dmContac_Dn.SS(SDM1_SiteGrrPF_History1_1 + m_nUnitIdx*STATION_MAX_PARA + nX + nY * STATION_MAX_X_SITE, (LPSTR)(LPCTSTR)strGrrSiteHis, strGrrSiteHis.GetLength() + 1);
							//}								
						}

						//TP101 Error String 발생시 다른 Station으로 이동

						BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(pReadData);

						if (strcmp(pReadData.szErrString, DEF_TEST_RESULT_TP101) != 0 && bIsDefaultBin != TRUE)
						{
							// change status and auto retest
							int nCaclTestCnt = pTaskPressUnit->GetCalcTestingCnt(nTestCnt, pReadData);
							pReadData.nTestingCnt = nTestCnt = nCaclTestCnt;
						}
							
						// set data
						pTaskPressUnit->SetSiteDeviceData(&pReadData, nX, nY);

						// set fail history
						pTaskPressUnit->SetFailHistoryAndPFcnt(nX, nY, nBinColor, nTestCnt);

						// set hw bin history
						pTaskPressUnit->SetHwBinHistory(nX, nY, atoi(szHWbin));

						// set last error string
						g_DMCont.m_dmContac_Dn.SS(SDM1_SiteLastDesc1_1 + m_nUnitIdx*STATION_MAX_PARA + nTargetPos, pReadData.szErrString, sizeof(pReadData.szErrString) );

						//GRR Mode에 따른 Device 상태 변경
						if (bAutoGrrUseMode) {
							int nFindIdx = 0;
							nFindIdx = pReadData.findIndexTestSite(pReadData.szTestSiteNum);
							ASSERT(nFindIdx >= 0);

							BOOL bChkTestEnd = (bSwapOption == TRUE) ? pReadData.stGrrTestInfo[nFindIdx].chkTestEnd(nX, nY) : pReadData.stGrrTestInfo[nFindIdx].chkTestEnd();
							if (bChkTestEnd == TRUE) {
								pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_TestEnd);
							}
							else {
								if (nGrrMode == ePressContinueTest) {
									pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_DvcLoading);
								}
								else {
									pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_TestEnd);
								}
							}

// 							if (pReadData.stGrrTestInfo[nFindIdx].chkTestEnd() == TRUE) {
// 								pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_TestEnd);
// 							}
// 							else {
// 								if (nGrrMode == ePressContinueTest) {
// 									pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_DvcLoading);
// 								}
// 								else {
// 									pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_TestEnd);
// 								}
// 							}
						}
						else {
							// set test end

							if (bAutoLoopUseMode == TRUE) {
								if (pReadData.stLoopTestInfo[m_nUnitIdx].chkTestEnd() == TRUE) {
									pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_TestEnd);
								}
								else {
									if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0)
									{
										if (nLoopMode == ePressContinueTest)
											pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_DvcLoading);
										else
											pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_TestEnd);
									}
									else {
										if (pReadData.stLoopTestInfo[m_nUnitIdx].chkRePickPlace() == FALSE)
											pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_DvcLoading);
										else
											pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_TestEnd);
									}
								}
							}
							else {
								pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_TestEnd);
							}
						}

						//FTP
						int nFtpUse = g_DMCont.m_dmEQP.GN(NDM0_FTP_USE);
						if (nFtpUse == TRUE) {
							//g_Ftp.InsertData(rData.m_nStationID, pReadData.sz2DBarcode);
						}

						// Station Site On 되어있는 All Site Fail 확인
						if (pReadData.nBinColor == eGoodBin) {
							bAllSiteFail = FALSE;
						}

					}// Socket Device Data 읽지 못하는 문제로 Data 처리 안되는 현상 발생 함. Retry 후 back Up 했던 Device Data를 다시 넣어 준다. [5/27/2025 donghyun.shin]
					else if (nSiteOnOff == DEF_SITE_ON && pReadData.sExist == DEF_NOT_EXIST && strcmp(pReadData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) != 0 &&
						m_bSocketDvc[nX][nY] == TRUE)
					{
						pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [TesterResult SetSiteDeviceData goto] [x=%d,y%d]", m_szCmdName, m_nStep, nX, nY);					
						pTaskPressUnit->SetSiteDeviceData(&m_stDvcData[nX][nY], nX, nY);
						Sleep(200);
						goto gotoTestResultRetry;
					}// end site on+ device exist
					else if(nSiteOnOff == DEF_SITE_ON){
						pTaskPressUnit->SetNotTestHistory(nX, nY);

						// Station Site On 되어있는 All Site Fail 확인
						bAllSiteFail = FALSE;
					}

				}//end nX
			}// end nY

			if (bAllSiteFail) {
				pTaskPressUnit->AddAllStationFailCnt();
			}
			else {
				pTaskPressUnit->ClearAllStationFailCnt();
			}

			//Daily Report
			ST_DAILY_REPORT stDailyData;
			stDailyData.nART_FT_Total_Cnt = nTestKindCnt[eARB_1stTestEnd - 1];
			stDailyData.nART_FT_Pass_Cnt = nTestPassKindCnt[eARB_1stTestEnd - 1];
			stDailyData.nART_RT1_Total_Cnt = nTestKindCnt[eARB_2stTestEnd - 1];
			stDailyData.nART_RT1_Pass_Cnt = nTestPassKindCnt[eARB_2stTestEnd - 1];
			stDailyData.nART_RT2_Total_Cnt = nTestKindCnt[eARB_3stTestEnd - 1];
			stDailyData.nART_RT2_Pass_Cnt = nTestPassKindCnt[eARB_3stTestEnd - 1];

			g_TaskSystemCtrl.AddDailyReportData(stDailyData);

			// Amkor BinSolution
			CString strBinSolutionData = _T("");
			g_TaskPressUnit->SetTextBinSolution(strBinSolutionData, m_nUnitIdx);
	
			int resultCode = ::sendto(g_UDPSockBinSolution, strBinSolutionData, strBinSolutionData.GetLength(), 0,
				(SOCKADDR*)&g_udpserverAddr, sizeof(g_udpserverAddr));
			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [BinSolution=%s]", m_szCmdName, m_nStep, strBinSolutionData);
			

			// index time ( EOT )
			//BOOL bChkIndex = g_TaskSystemCtrl.m_tIndexTime[m_nUnitIdx].IsChecking();
			//if( bChkIndex == FALSE){
				g_TaskSystemCtrl.m_tIndexTime[m_nUnitIdx].StartTimer();
			//}

			m_nLast_Executing_Step = m_nStep = 5000;
		}break;
	case 5000:
		{
			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d]", m_szCmdName, m_nStep);
			m_nLast_Executing_Step = m_nStep = 6000;
		}break;
	case 6000:
		{
			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d]", m_szCmdName, m_nStep);
			ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
		}break;
	}
	return CBaseSeqCmd::State_Executing();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_OnLineTestApple::State_Pause()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskPressUnit->MakeLog("[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0:
		{
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
BOOL CPressUnitCmd_OnLineTestApple::State_Recovery()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskPressUnit->MakeLog("[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;


	switch( m_nStep )
	{
	case 0: // 현재 위치와 마지막 위치를 비교 한다.
		{
            FlagReset();
			SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Executing, m_nLast_Executing_Step);
		}break;
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
BOOL CPressUnitCmd_OnLineTestApple::State_NormalComplete()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;

	BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
	BOOL bAutoLoopUse = g_TaskSystemCtrl.GetAutoLoopUse();
	CString strErrPos, strErrMsg, strTimeoutErrPos, strTimeoutErrMsg;

	pTaskPressUnit->CalYieldMonitoring();

	if (bAutoGrrUse != TRUE && bAutoLoopUse != TRUE)
	{
		BOOL bSiteOffErr = FALSE;
		BOOL bSiteOffUse = FALSE;
		BOOL bSiteErrorCode = FALSE;
		BOOL bSiteTimeOutErrorCode = FALSE;

		int nSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
		int nSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

		std::vector<CPoint> vSiteOff;

		if (g_DMCont.m_dmHandlerOpt.GB(BDM11_SltPgm_SiteOff_DefaultBin) && g_DMCont.m_dmHandlerOpt.GB(BDM11_SltPgm_Sort_as_DefaultBin))
		{
			ST_DD_DEVICE stDvcData;
			vSiteOff.clear();

			for (int nY = 0; nY < nSiteY; nY++)
			{
				for (int nX = 0; nX < nSiteX; nX++)
				{
					stDvcData.clear();
					// get device data
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &stDvcData);
					bSiteOffUse = FALSE;

					//for (int i = 0; i<(int)g_DefaultBinCategory.vDefaultBin.size(); i++) {
					//	if (g_DefaultBinCategory.vDefaultBin[i].Compare(stDvcData.szErrString) == 0) {
					//		bSiteOffUse = TRUE;
					//	}
					//}

					if (g_TaskSystemCtrl.IsDefaultBin(stDvcData)) {
						bSiteOffUse = TRUE;
						bSiteErrorCode = TRUE;
					}

					if (bSiteOffUse) {
						BOOL bIsSiteOff = g_TaskSystemCtrl.CheckAutoRetestRuleSite(m_nUnitIdx, nX, nY);
						if (bIsSiteOff) {
							//Site Off
							g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + nX + (nY*STATION_MAX_X_SITE) + (m_nUnitIdx*STATION_MAX_PARA), DEF_SITE_OFF_ERRORCODE);
							vSiteOff.push_back(CPoint(nX, nY));
							strErrPos.Format(_T("[Station=%d Site=%d ]\n"), m_nUnitIdx + 1, (nX + 1 + (nY * STATION_MAX_X_SITE)));
							strErrMsg += strErrPos;
						}
						else {
							bSiteOffErr = TRUE;
						}
					}
				}
			}
			if ((int)vSiteOff.size() > 0) {
				g_TaskSystemCtrl.SetSiteOnOffData(m_nUnitIdx, vSiteOff, DEF_SITE_OFF_ERRORCODE);
			}
		}

		CParamList AlarmData;

		if (bSiteErrorCode)
		{
			AlarmData.m_fnClear();
			AlarmData.m_fnAddItemFormat("Occurred Default Bin Station (All Site) Failure Alarm!!!!");
			AlarmData.m_fnAddItemFormat(strErrMsg);
			g_Error.AlarmReport(ERR_TESTER_DEFAULT_BIN_FAIL_STATION_1 + m_nUnitIdx, AlarmData, &g_TaskSystemCtrl, vSiteOff);
			strErrMsg.Empty();
		}

		if (g_DMCont.m_dmHandlerOpt.GB(BDM11_SltPgm_SiteOff_TestTimeOut))
		{
			//int nPressAreaIdx = pTaskPressUnit->CovertPressAreaIdx(m_nUnitIdx);
			ST_DD_DEVICE stDvcData;
			vSiteOff.clear();

			for (int nY = 0; nY < nSiteY; nY++)
			{
				for (int nX = 0; nX < nSiteX; nX++)
				{
					stDvcData.clear();
					// get device data
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &stDvcData);

					bSiteOffUse = FALSE;

					if (strcmp(stDvcData.szErrString, DEF_TEST_TIME_OUT_ERR_DEVICE) == 0) {
						bSiteOffUse = TRUE;
						bSiteTimeOutErrorCode = TRUE;
					}

					if (bSiteOffUse) {
						BOOL bIsSiteOff = g_TaskSystemCtrl.CheckAutoRetestRuleSite(m_nUnitIdx, nX, nY);
						if (bIsSiteOff) {
							//Site Off
							g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + nX + (nY*STATION_MAX_X_SITE) + (m_nUnitIdx*STATION_MAX_PARA), DEF_SITE_OFF_ERRORCODE);
							vSiteOff.push_back(CPoint(nX, nY));
							strTimeoutErrPos.Format(_T("[Station=%d Site=%d ]\n"), m_nUnitIdx + 1, (nX + 1 + (nY * STATION_MAX_X_SITE)));
							strTimeoutErrMsg += strTimeoutErrPos;
						}
						else {
							bSiteOffErr = TRUE;
						}
					}
				}
			}
			if ((int)vSiteOff.size() > 0) {
				g_TaskSystemCtrl.SetSiteOnOffData(m_nUnitIdx, vSiteOff, DEF_SITE_OFF_ERRORCODE);
			}
		}

		if (bSiteTimeOutErrorCode)
		{
			AlarmData.m_fnClear();
			AlarmData.m_fnAddItemFormat("Occurred Tester Timeout Station (All Site) Failure Alarm!!!!");
			AlarmData.m_fnAddItemFormat(strTimeoutErrMsg);
			g_Error.AlarmReport(ERR_TESTER_TIMTEOUT_FAIL_STATION_1 + m_nUnitIdx, AlarmData, &g_TaskSystemCtrl, vSiteOff);
			strErrMsg.Empty();
		}

		if (bSiteOffErr) {
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_INTERLOCK_SITE_OFF_ERROR_STATION_1 + m_nUnitIdx, AlarmData, &g_TaskSystemCtrl);
			g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
		}
	}

	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_OnLineTestApple::State_Alarm()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskPressUnit->MakeLog("[CMD: %s] [ALARM] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
	{
	case 0: // alarm 발생 했을 때 처리해 줘야 할꺼 처리 해줌
		{
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
BOOL CPressUnitCmd_OnLineTestApple::State_AlarmComplete()
{
    CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;

	//	// some time during testing continue tester
	//	g_AppleTestIF.SendAction(m_nUnitIdx+1, ACTION_STOP);

	if( m_bTesterErr != TRUE )
	{
		ST_DD_DEVICE pDvcData;
		BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);

		char szCompany[16] = { 0, };
		g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));		

		BOOL bAutoGrrUseMode	= g_TaskSystemCtrl.GetAutoGrrUse();
		BOOL bAutoLoopUseMode = g_TaskSystemCtrl.GetAutoLoopUse();

		BOOL bGrrCancel = g_TaskSystemCtrl.GetAutoGrrCancel();
		BOOL bLoopCancel = g_TaskSystemCtrl.GetAutoLoopCancel();

		CString strResult = _T("fail");
		// Get local time
		SYSTEMTIME timeEnd;
		::GetLocalTime( &timeEnd );
//		g_EOT[ePressUnit_Down_1 + m_nUnitIdx] = timeEnd;

		int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
		CTesterStatus cTesterStatus = g_AppleTestIF.GetTesterStatus(m_nUnitIdx+1, nTesterOption);

		ResultData rData;
// #if DEF_TESTER_COMM_SPEC_ONE_SOCKET
// 		int nDirSiteOnOff[eEachTesterSocket::eMaxTesterForYanmade] = { 0, };
// #else
// 		int nDirSiteOnOff[eMaxTester] = { 0, };
// #endif

		//Single, Dual, Each 동일하게 사용
		int nDirSiteOnOff[eEachTesterSocket::eMaxTesterForYanmade] = { 0, };

		pTaskPressUnit->TesterSiteOnOffCheck(nDirSiteOnOff);

		int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
		BOOL bRet = g_AppleTestIF.GetResultDataFromTester(m_nUnitIdx + 1, &rData, nDirSiteOnOff, nTesterOption);
		int nTestKindCnt[3] = { 0, }, nTestPassKindCnt[3] = { 0, }; // Daily Report data 0 : FT, 1: RT1, 2: RT2
		CString strErrPos, strErrMsg;

		std::vector<CPoint> vSiteOff;
		vSiteOff.clear();
		for(int nY=0; nY< m_nSiteCntY /*&& m_bNotRevData == FALSE*/ ; nY++)
		{
			for(int nX=0; nX< m_nSiteCntX /*&& m_bNotRevData == FALSE*/; nX++)
			{
				pDvcData.clear();
				pTaskPressUnit->GetSiteDeviceData(&pDvcData, nX, nY);

				int nOnOff = DEF_SITE_OFF;
				if( bAutoGrrUseMode == TRUE)
					nOnOff = pTaskPressUnit->GetSocketOnOffGrr(nX, nY);

				else
					nOnOff = pTaskPressUnit->GetSocketOnOff(nX, nY);

				int nTargetPos = nX + nY*STATION_MAX_X_SITE;

				if (/* nOnOff == DEF_SITE_ON &&*/ pDvcData.sExist == DEF_EXIST)
				{
					CString strTRL = _T(""), strStartTm = _T(""), strEndTm = _T("");
					char szHWbin[16] = { 0, };
					BOOL bChkRegCateInfo = FALSE;
					int  nBinColor = eFailBin;

// #if DEF_TESTER_COMM_SPEC_ONE_SOCKET
// 					pTaskPressUnit->SetTestHandlerErrorBin(pDvcData, rData, nTargetPos, szHWbin, nBinColor, strResult);
// #else
// 					strcpy_s(pDvcData.szErrString, sizeof(pDvcData.szErrString), DEF_HANDLER_ERR_DEVICE);
// 					pTaskPressUnit->MakeLog("Skip to online test");
// 					strcpy_s(pDvcData.szBinResult, sizeof(pDvcData.szBinResult), "98");
// 					sprintf_s(szHWbin, sizeof(szHWbin), "4");
// 					pDvcData.nBinColor = nBinColor;
// 					strResult = "fail";					
// #endif

					int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
					//if (nTesterOption == eTester_EachSocket )
					{
						pTaskPressUnit->SetTestHandlerErrorBin(pDvcData, rData, nTargetPos, szHWbin, nBinColor, strResult);
					}
// 					else
// 					{
// 						strcpy_s(pDvcData.szErrString, sizeof(pDvcData.szErrString), DEF_HANDLER_ERR_DEVICE);
// 						pTaskPressUnit->MakeLog("Skip to online test");
// 						strcpy_s(pDvcData.szBinResult, sizeof(pDvcData.szBinResult), "98");
// 						sprintf_s(szHWbin, sizeof(szHWbin), "4");
// 						pDvcData.nBinColor = nBinColor;
// 						strResult = "fail";
// 					}


					strStartTm.Format("%04d%02d%02d%02d%02d%02d%03d", m_sysTmTestStart.wYear, m_sysTmTestStart.wMonth, m_sysTmTestStart.wDay, m_sysTmTestStart.wHour, m_sysTmTestStart.wMinute, m_sysTmTestStart.wSecond, m_sysTmTestStart.wMilliseconds);
					strEndTm.Format("%04d%02d%02d%02d%02d%02d%03d", timeEnd.wYear, timeEnd.wMonth, timeEnd.wDay, timeEnd.wHour, timeEnd.wMinute, timeEnd.wSecond, timeEnd.wMilliseconds);

					/* Product CSV foramt [ AMKOR ]
						1. Handler Send Barcode : SLT Handler에서 읽은 barcode
						2. Tester Send Barcode : Tester 에서 결과로 보내준 barcode
						3. SLT ID : Handler hostname
						4. Station ID : Station No ( 1 ~ 16 )
						5. Site ID : site No ( 1 ~ 4 ) // MP의 경우는 1 ~ 8
						6. Tester ID : TesterID
						7. Socket ID : Socket ID
						8. Tester Start/End time : 테스트 시작 시간, 테스트 종료 시간( Bin 결과 받은 시간 )
						9. Test Result :Tester로부터 받은 Pass/Fail
						10. Test ErrorString : Tester로 받은 ErrorString
						10. Test ErrorCode : Tester로 받은 ErrorCode
						11. HW Bin : SLT Handler에 sort 될 unload #No 위치
					*/

					char szEqpName[128] = { 0, };
					g_DMCont.m_dmHandlerOpt.GS(SDM11_UDP_HOST_NAME, szEqpName, sizeof(szEqpName));

// #if DEF_TESTER_COMM_SPEC_ONE_SOCKET
// 
// 					if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0)
// 					{
// 						strTRL.Format(_T("%s,%s,%s,%d,%d,%d,%s,%s,%s,%s,%s,%s")
// 							, pDvcData.sz2DBarcode
// 							, rData.m_cDutSn[nTargetPos]
// 							, szEqpName
// 							, m_nUnitIdx + 1
// 							, nX + nY*STATION_MAX_X_SITE + 1
// 							, cTesterStatus.m_nRegiDutIdentityNo[(nX + nY*STATION_MAX_X_SITE)]
// 							, (LPCTSTR)strStartTm, (LPCTSTR)strEndTm
// 							, pDvcData.szErrorCode
// 							, strResult
// 							, pDvcData.szErrString							
// 							, szHWbin);
// 					}
// 					else {
// 						strTRL.Format(_T("%s,%s,%s,%d,%d,%d,%s,%s,%s,%s,%s,%s")
// 							, pDvcData.sz2DBarcode
// 							, rData.m_cDutSn[nTargetPos]
// 							, szEqpName
// 							, m_nUnitIdx + 1
// 							, nX + nY*STATION_MAX_X_SITE + 1
// 							, cTesterStatus.m_nRegiDutIdentityNo[(nX + nY*STATION_MAX_X_SITE)]
// 							, (LPCTSTR)strStartTm
// 							, (LPCTSTR)strEndTm
// 							, (rData.m_nTestResult[nTargetPos] == eGoodBin ? _T("PASS") : _T("FAIL"))
// 							, pDvcData.szErrString
// 							, pDvcData.szErrorCode
// 							, szHWbin);
// 
// 					}
// #else
// 
// 					if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) 
// 					{
// 						strTRL.Format(_T("%s,%s,%s,%d,%d,%s,%s,%s,%s,%s,%s,%s")
// 							, pDvcData.sz2DBarcode, "", szEqpName
// 							, m_nUnitIdx + 1, nX + nY*STATION_MAX_X_SITE + 1
// 							, (LPCTSTR)(cTesterStatus.m_sTesterID_Orginal), (LPCTSTR)(cTesterStatus.m_sSocketID_Orginal[(nX + nY*STATION_MAX_X_SITE)]), (LPCTSTR)strStartTm, (LPCTSTR)strEndTm
// 							, strResult
// 							, pDvcData.szErrString, szHWbin);
// 					}
// 					else {
// 						strTRL.Format(_T("%s,%s,%s,%d,%d,%s,%s,%s,%s,%s,%s,%s")
// 							, pDvcData.sz2DBarcode, rData.m_cDutSn[nTargetPos], szEqpName
// 							, m_nUnitIdx + 1, nX + nY*STATION_MAX_X_SITE + 1
// 							, (LPCTSTR)(cTesterStatus.m_sTesterID_Orginal), (LPCTSTR)(cTesterStatus.m_sSocketID_Orginal[(nX + nY*STATION_MAX_X_SITE)]), (LPCTSTR)strStartTm, (LPCTSTR)strEndTm
// 							, (rData.m_nTestResult[nTargetPos] == eGoodBin ? _T("PASS") : _T("FAIL"))
// 							, pDvcData.szErrString, szHWbin);
// 
// 					}
// #endif

					//송도 Amkor 요청사항. 대문자 -> 소문자 변환
					CString strBinResult = (rData.m_nTestResult[nTargetPos] == eGoodBin) ? _T("PASS") : _T("FAIL");
					strBinResult.MakeLower();

//					if (nTesterOption == eTester_EachSocket)
					{
						int nStationID = 0;
						CString strStationID = _T("");
						nStationID = (((m_nSiteCntX*m_nSiteCntY)*m_nUnitIdx) + nX + nY*m_nSiteCntX) + 1;
						CTesterStatus cTesterStatus = g_AppleTestIF.GetTesterStatus(nStationID);
						strStationID.Format("%d", cTesterStatus.m_nStationID);

						strTRL.Format(_T("%s,%s,%s,%s,%s,%s,%s")
							, pDvcData.sz2DBarcode
							, strStationID
							, (LPCTSTR)strStartTm, (LPCTSTR)strEndTm
							, strBinResult/*(rData.m_nTestResult[nTargetPos] == eGoodBin ? _T("PASS") : _T("FAIL"))*/
							, rData.m_cErrorStr[nTargetPos], szHWbin);
					}
// 					else
// 					{
// 						strTRL.Format(_T("%s,%s,%s,%d,%d,%s,%s,%s,%s,%s,%s,%s")
// 							, pDvcData.sz2DBarcode
// 							, ""
// 							, szEqpName
// 							, m_nUnitIdx + 1
// 							, nX + nY*STATION_MAX_X_SITE + 1
// 							, (LPCTSTR)(cTesterStatus.m_sTesterID_Orginal)
// 							, (LPCTSTR)(cTesterStatus.m_sSocketID_Orginal[(nX + nY*STATION_MAX_X_SITE)])
// 							, (LPCTSTR)strStartTm, (LPCTSTR)strEndTm
// 							, strResult
// 							, pDvcData.szErrString, szHWbin);
// 					}

					TWLOG.MakeLog(static_cast<int>(eLogID_PRODUCTCSV), Debug, "", __LINE__, NULL, strTRL);
//					TWLOG.MakeLog(static_cast<int>(eLogID_2DIDCSV), Debug, "", __LINE__, NULL, strTRL);

					sprintf_s(pDvcData.szTestSiteNum, sizeof(pDvcData.szTestSiteNum), "%d", m_nUnitIdx);
					pDvcData.cSubSiteNum = CPoint(nX, nY);

					// Set Testing Count
					int nTestCnt = pDvcData.nTestingCnt;
					if (bAutoGrrUseMode != TRUE)
					{	// JAM RATE DATA RECORD
						if (bAutoLoopUseMode != TRUE )
						{
							ASSERT(nTestCnt >= eARB_NotTest && nTestCnt <= eARB_3stTestEnd);
							int nDvcTestCnt = nTestCnt;
							if (bAutoRetestUseMode == TRUE && g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoRetest) == eAutoRetest_AB && nTestCnt > eARB_NotTest) {
								nDvcTestCnt -= 1; //AB의 경우는 1stEnd 후에 바로 2stEnd로 바꾸어주기 때문에 빼야함.
							}
							pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].nHwBin = atoi(szHWbin) - 1;
							pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].nPF = nBinColor;
							pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].nSiteNo = atoi(pDvcData.szTestSiteNum);
							pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].cSubSiteNo = pDvcData.cSubSiteNum;
							sprintf_s(pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrStr, sizeof(pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrStr), pDvcData.szErrString);
							sprintf_s(pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrCode, sizeof(pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrCode), pDvcData.szErrorCode);
							sprintf_s(pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szTesterHwBin, sizeof(pDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szTesterHwBin), pDvcData.szTesterHWBin);

							int nArtCnt = g_DMCont.m_dmEQP.GN(NDM0_ART_FT_TOTAL + (nTestCnt * 2));
							g_DMCont.m_dmEQP.SN(NDM0_ART_FT_TOTAL + (nTestCnt * 2), ++nArtCnt);
							if (nBinColor == eGoodBin) {
								nArtCnt = g_DMCont.m_dmEQP.GN(NDM0_ART_FT_PASS + (nTestCnt * 2));
								g_DMCont.m_dmEQP.SN(NDM0_ART_FT_PASS + (nTestCnt * 2), ++nArtCnt);
								nTestPassKindCnt[nTestCnt]++;
							}
							nTestKindCnt[nTestCnt]++;

							pTaskPressUnit->PushYieldData(nX, nY, nBinColor, nTestCnt);
						}
						else {
							if (bLoopCancel == FALSE)
								pDvcData.stLoopTestInfo[m_nUnitIdx].testDoneAll();
							else {
								int nMaxTestCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoLoopTestCount);
								for (int nTestCnt = pDvcData.stLoopTestInfo[m_nUnitIdx].nTestCnt[nX][nY]; nTestCnt < nMaxTestCount; nTestCnt++)
								{
									pDvcData.stLoopTestInfo[m_nUnitIdx].testDoneAll();
								}
							}
						}
					}
					else {
						int nFindIdx = pDvcData.findIndexTestSite(pDvcData.szTestSiteNum);
						ASSERT(nFindIdx >= 0);
						//pDvcData.stGrrTestInfo[nFindIdx].testDone(nX, nY);	// 해당 Site의 Test 횟수를 증가시켜줌


						BOOL bSwapOption = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrDeviceSwapUse);
						if (bGrrCancel == TRUE) {
							int nMaxTestCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrTestCount);
							for (int nTestCnt = pDvcData.stGrrTestInfo[nFindIdx].nTestCnt[nX][nY]; nTestCnt < nMaxTestCount; nTestCnt++)
							{
								pDvcData.stGrrTestInfo[nFindIdx].testDoneAll();
							}
						}
						else {
							if (bSwapOption == TRUE) {
								pDvcData.stGrrTestInfo[nFindIdx].testDone(nX, nY);	// 해당 Site의 Test 횟수를 증가시켜줌
							}
							else {
								pDvcData.stGrrTestInfo[nFindIdx].testDoneAll();	// 해당 Site의 Test 횟수를 증가시켜줌
							}
						}


// 						if (bGrrCancel == FALSE)
// 							pDvcData.stGrrTestInfo[nFindIdx].testDoneAll();	// 해당 Site의 Test 횟수를 증가시켜줌
// 						else
// 						{
// 							int nMaxTestCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrTestCount);
// 							for (int nTestCnt = pDvcData.stGrrTestInfo[nFindIdx].nTestCnt[nX][nY]; nTestCnt < nMaxTestCount; nTestCnt++)
// 							{
// 								pDvcData.stGrrTestInfo[nFindIdx].testDoneAll();
// 							}
// 						}

						// Create DB Data
						_tGRR_DATA stGrrData;
						sprintf(stGrrData.szUnique, "%s", pDvcData.szUniqueID);
						sprintf(stGrrData.sz2DID, "%s", pDvcData.sz2DBarcode);
						sprintf(stGrrData.szSiteNum, "%d", atoi(pDvcData.szTestSiteNum) + 1);
						stGrrData.nTestCntNo = pDvcData.stGrrTestInfo[nFindIdx].nTestCnt[nX][nY];
						sprintf(stGrrData.szSubSiteNo, "%d", (nX + 1 + (nY * STATION_MAX_X_SITE)));
						sprintf(stGrrData.szTestResult, (atoi(pDvcData.szBinResult) == eGoodBin ? "PASS" : "FAIL"));
						sprintf(stGrrData.szResultDescription, "%s", pDvcData.szErrString);

						g_DB.ASYNC_INSERT_GRRDATA_TBL(stGrrData);

						char szGrrSiteHis[MAX_PATH] = { 0, };
						g_DMCont.m_dmContac_Dn.GS(SDM1_SiteGrrPF_History1_1 + m_nUnitIdx*STATION_MAX_PARA + nX + nY * STATION_MAX_X_SITE, szGrrSiteHis, sizeof(szGrrSiteHis));
						CString strGrrSiteHis = _T("");
						strGrrSiteHis.Format("%s%s", szGrrSiteHis, (atoi(pDvcData.szBinResult) == eGoodBin ? "P" : "F"));
						int nlength = strGrrSiteHis.GetLength();
						if (nlength >= MAX_STRING_LENGTH)
							g_DMCont.m_dmContac_Dn.SS(SDM1_SiteGrrPF_History1_1 + m_nUnitIdx*STATION_MAX_PARA + nX + nY * STATION_MAX_X_SITE, (LPSTR)(LPCTSTR)strGrrSiteHis, strGrrSiteHis.GetLength() + 1);
					}

					// change status and auto retest
					BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(pDvcData);

					if (bIsDefaultBin != TRUE)
					{
						int nCaclTestCnt = pTaskPressUnit->GetCalcTestingCnt(nTestCnt, pDvcData);
						pDvcData.nTestingCnt = nTestCnt = nCaclTestCnt;
					}
					// set data
					pTaskPressUnit->SetSiteDeviceData(&pDvcData, nX, nY);

					// set last error string
					g_DMCont.m_dmContac_Dn.SS(SDM1_SiteLastDesc1_1 + m_nUnitIdx*STATION_MAX_PARA + nX + nY*STATION_MAX_X_SITE, pDvcData.szErrString, sizeof(pDvcData.szErrString));

					// set fail history
					pTaskPressUnit->SetFailHistoryAndPFcnt(nX, nY, nBinColor, nTestCnt);

					// set hw bin history
					pTaskPressUnit->SetHwBinHistory(nX, nY, atoi(szHWbin));

					// set test end
					pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_TestEnd);

					//FTP
					int nFtpUse = g_DMCont.m_dmEQP.GN(NDM0_FTP_USE);
					if(nFtpUse == TRUE){
						//g_Ftp.InsertData(m_nUnitIdx+1,pDvcData.sz2DBarcode);
					}

				
					if (bAutoGrrUseMode != TRUE && bAutoLoopUseMode != TRUE) {
						BOOL bSiteOffErr = FALSE;
						BOOL bSiteOffUse = FALSE;
						BOOL bSiteErrorCode = FALSE;

						//Auto Site Off 
						if (g_DMCont.m_dmHandlerOpt.GB(BDM11_SltPgm_SiteOff_DefaultBin) && g_DMCont.m_dmHandlerOpt.GB(BDM11_SltPgm_Sort_as_DefaultBin))
						{
							ST_DD_DEVICE stDvcData;

							stDvcData.clear();
							// get device data
							g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &stDvcData);

							//for (int i = 0; i < (int)g_DefaultBinCategory.vDefaultBin.size(); i++) {
							//	if (g_DefaultBinCategory.vDefaultBin[i].Compare(stDvcData.szErrString) == 0) {
							//		bSiteOffUse = TRUE;
							//	}
							//}

							if (g_TaskSystemCtrl.IsDefaultBin(stDvcData)) {
								bSiteOffUse = TRUE;
								bSiteErrorCode = TRUE;
							}

							if (bSiteOffUse) {
								BOOL bIsSiteOff = g_TaskSystemCtrl.CheckAutoRetestRuleSite(m_nUnitIdx, nX, nY);
								if (bIsSiteOff) {
									//Site Off
									g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + nX + (nY*STATION_MAX_X_SITE) + (m_nUnitIdx*STATION_MAX_PARA), DEF_SITE_OFF_ERRORCODE);
									vSiteOff.push_back(CPoint(nX, nY));
									strErrPos.Format(_T("[Station=%d Site=%d ]\n"), m_nUnitIdx + 1, (nX + 1 + (nY * STATION_MAX_X_SITE)));
									strErrMsg += strErrPos;
								}
								else {
									bSiteOffErr = TRUE;
								}
							}
						}

						CParamList AlarmData;
						if (bSiteOffErr) {
							AlarmData.m_fnClear();
							g_Error.AlarmReport(ERR_INTERLOCK_SITE_OFF_ERROR_STATION_1 + m_nUnitIdx, AlarmData, &g_TaskSystemCtrl);
							g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_AlarmPause, NULL, eEqpCtrlMode_Auto);
						}

						if (bSiteErrorCode)
						{
							AlarmData.m_fnClear();
							AlarmData.m_fnAddItemFormat("Occurred Default Bin Station (All Site) Failure Alarm!!!!");
							AlarmData.m_fnAddItemFormat(strErrMsg);
							g_Error.AlarmReport(ERR_TESTER_DEFAULT_BIN_FAIL_STATION_1 + m_nUnitIdx, AlarmData, &g_TaskSystemCtrl, vSiteOff);
							strErrMsg.Empty();
						}
					}

				}
				else if(nOnOff == DEF_SITE_ON && pDvcData.nBinColor != eGoodBin){
					pTaskPressUnit->SetNotTestHistory(nX, nY);
				}
			}
		}

		//Daily Report
		ST_DAILY_REPORT stDailyData;
		stDailyData.nART_FT_Total_Cnt = nTestKindCnt[eARB_1stTestEnd - 1];
		stDailyData.nART_FT_Pass_Cnt = nTestPassKindCnt[eARB_1stTestEnd - 1];
		stDailyData.nART_RT1_Total_Cnt = nTestKindCnt[eARB_2stTestEnd - 1];
		stDailyData.nART_RT1_Pass_Cnt = nTestPassKindCnt[eARB_2stTestEnd - 1];
		stDailyData.nART_RT2_Total_Cnt = nTestKindCnt[eARB_3stTestEnd - 1];
		stDailyData.nART_RT2_Pass_Cnt = nTestPassKindCnt[eARB_3stTestEnd - 1];

		g_TaskSystemCtrl.AddDailyReportData(stDailyData);

		CString strBinSolutionData = _T("");
		g_TaskPressUnit->SetTextBinSolution(strBinSolutionData, m_nUnitIdx);
		int resultCode = ::sendto(g_UDPSockBinSolution, strBinSolutionData, strBinSolutionData.GetLength(), 0,
			(SOCKADDR*)&g_udpserverAddr, sizeof(g_udpserverAddr));


		// index time ( EOT )
		BOOL bChkIndex = g_TaskSystemCtrl.m_tIndexTime[m_nUnitIdx].IsChecking();
		if( bChkIndex == FALSE){
			g_TaskSystemCtrl.m_tIndexTime[m_nUnitIdx].StartTimer();
		}
	}

	pTaskPressUnit->CalYieldMonitoring();
	

return CBaseSeqCmd::State_AlarmComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_OnLineTestApple::State_Aborting()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskPressUnit->MakeLog("[CMD: %s] [ABORT] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0:
	{
		m_nStep++;
	}break;
	case 1:
	{
		ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AbortComplete); // 명령 진행 중 STOP 명령을 받아 명령이 중단 됨.
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
BOOL CPressUnitCmd_OnLineTestApple::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}

void CPressUnitCmd_OnLineTestApple::FlagReset()
{
	m_bSotErr = FALSE;
	m_bWaitResultErr = FALSE;
	m_bNotRevData = FALSE;
	m_b2dMisMatch = FALSE;
	m_bTesterErr = FALSE;
}

