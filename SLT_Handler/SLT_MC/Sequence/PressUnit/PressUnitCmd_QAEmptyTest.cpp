#include "stdafx.h"
#include "PressUnitCmd_QAEmptyTest.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPressUnitCmd_QAEmptyTest::CPressUnitCmd_QAEmptyTest(void)
{
	m_nCmdId = eSeqCmd_PressUnit_OfflineTest;
	m_nCmdStage = 0;
	m_nRetryCount = 0;
	m_tcTimeout = 0.0;
	m_tcCheckTime = 0.0;
	m_tcSotDelayTime = 0.0;
	m_nUnitIdx = -1;
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));

	m_nSiteDivX = 0;
	m_nSiteDivY = 0;
	m_nBinSortType = eSortType_ErrStr;
	m_tcMaxTestTimeout = 0.0;
	m_bChkPassFail = false;
}


CPressUnitCmd_QAEmptyTest::~CPressUnitCmd_QAEmptyTest(void)
{
}


//==============================================================================
//
//==============================================================================
int CPressUnitCmd_QAEmptyTest::CheckParam()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	m_nUnitIdx = pTaskPressUnit->m_nUnitIdx;

	ST_DD_DEVICE stDvcData;

	m_nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	m_nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	m_tcSotDelayTime = (double)g_DMCont.m_dmHandlerOpt.GN(NDM11_SltPgm_SotDelayTm);

	m_nBinSortType = g_DMCont.m_dmEQP.GN(NDM0_SortType);// enum eSortingType{   

	m_nWorkCmdStage = m_nCmdStage;

	m_tcMaxTestTimeout = (double)g_DMCont.m_dmHandlerOpt.GN(NDM11_SltPgm_TestTmOut);

	bool bPass = false, bFail = false;
	if (g_BinCategory.nIndexCount >= 2)
	{
		for (int i = 0; i < g_BinCategory.nIndexCount; i++)
		{
			if (strcmp(g_BinCategory.aCateInfo[i].szDesc, "PASS") == 0)
				bPass = true;
			if (strcmp(g_BinCategory.aCateInfo[i].szDesc, "FAIL") == 0)
				bFail = true;
		}

		if (bPass == true && bFail == true)
			m_bChkPassFail = true;
	}

	return ERR_NO_ERROR;
}


//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_QAEmptyTest::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_QAEmptyTest::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_QAEmptyTest::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_QAEmptyTest::OnPause()
{
	//SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_QAEmptyTest::OnResume()
{
	if (GetCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Executing) {

	}
	else {
		SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	}
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_QAEmptyTest::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_QAEmptyTest::State_Executing()
{
	// check alarm
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		//pTaskPressUnit->MakeLog("[CMD: %s] [EXECUTE] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0:			// check interlock
	{
		pTaskPressUnit->MakeLog("[%s] [EXEC=%8d]", m_szCmdName, m_nStep);
		//pTaskPressUnit->SendEvent(CEID_PRESS_UNIT_TEST_START);
		g_DMCont.m_dmEQP.SB(BDM0_TEST_STATUS_SITE1 + m_nUnitIdx, TRUE);    // Set testing status on

		::GetLocalTime(&m_sysTmTestStart);
		g_SOT[ePressUnit_Down_1 + m_nUnitIdx] = m_sysTmTestStart;

		if (g_BinCategory.nIndexCount == 0) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Bin category is not register. category count=0.");
			ReportAlarm(ERR_SYSTEM_BASE, AlarmData);
			return FALSE;
		}

		// index time ( SOT )
		BOOL bChkIndex = g_TaskSystemCtrl.m_tIndexTime[m_nUnitIdx].IsChecking();
		if (bChkIndex == TRUE) {
			double dIndexTm = g_TaskSystemCtrl.m_tIndexTime[m_nUnitIdx].CompareTime(FALSE);
			g_DMCont.m_dmEQP.SD(DDM0_TmIndexPress1 + m_nUnitIdx, dIndexTm);
			g_TaskSystemCtrl.m_tIndexTime[m_nUnitIdx].SetTimerCheckEnd();

			CString strSummaryTime = _T("");
			int nIndexCount = g_DMCont.m_dmContac_Dn.GN(NDM1_INDEX_RECORD_IGNORE_STATION1 + m_nUnitIdx);
			if (nIndexCount < MAX_IGNORE_INDEX_REC_COUNT || g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == TRUE || g_TaskSystemCtrl.IsOneCycleModeOn() == true)
			{
				g_DMCont.m_dmContac_Dn.SN(NDM1_INDEX_RECORD_IGNORE_STATION1 + m_nUnitIdx, ++nIndexCount);
				//g_DMCont.m_dmContac_Dn.SB(BDM1_INDEX_RECORD_IGNORE_STATION1 + m_nUnitIdx, FALSE);
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

				//double dRecordTime = g_DMCont.m_dmContac_Dn.GD(DDM1_SUMMARY_INDEX_STATION1 + m_nUnitIdx);

				//double dCalcTime = (dRecordTime + dIndexTm - dStopTime) / (dRecordTime > 0 ? 2.0 : 1.0);
				//g_DMCont.m_dmContac_Dn.SD(DDM1_SUMMARY_INDEX_STATION1 + m_nUnitIdx, dCalcTime);

				strSummaryTime.Format("Summary Index = %.2f [ (Acc=%.2f + Rec=%.2f(Index=%.2f-Stop=%2.f) / Cnt=%d]", dSummaryIdx / nCntAcc, dTimeAcc, dRecordTime, dIndexTm, dStopTime, nCntAcc);
			}
			g_DMCont.m_dmContac_Dn.SD(DDM1_STOP_INDEX_STATION1 + m_nUnitIdx, 0);

			pTaskPressUnit->MakeLog("Index Time, Press %d, %.2f, %s", m_nUnitIdx + 1, dIndexTm, strSummaryTime);
		}

		ST_DD_DEVICE pDvcData;
		
		for (int nY = 0; nY < m_nSiteDivY; nY++)
		{
			for (int nX = 0; nX < m_nSiteDivX; nX++)
			{
				pDvcData.clear();
				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &pDvcData);

				int nSiteOnOff = pTaskPressUnit->GetSocketOnOff(nX, nY);

				if (nSiteOnOff == DEF_SITE_ON && pDvcData.sExist == DEF_EXIST && strcmp(pDvcData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) != 0) {
					pDvcData.nRealTestingCnt++;
					g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &pDvcData);

					pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_TestStart);
				}
			}
		}

		g_DMCont.m_dmEQP.SN(NDM0_Press1_TestTm + m_nUnitIdx, 0);

		::GetLocalTime(&m_sysTmTestStart);
		m_tcCheckTime = (double)GetTickCount64();
		m_nStep = 5000;
		pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Test](STR)", m_szCmdName, m_nStep);
	}break;
	case 5000:
	{
		//double dOffSetTime = (m_nUnitIdx % 2 == 0 ? 0 : -20000);
// 		double dSpanTime = (double)GetTickCount64() - m_tcCheckTime;
// 		double dOfflineTestTm = (double)g_DMCont.m_dmHandlerOpt.GN(NDM11_TM_OffLineTest);
// 		g_DMCont.m_dmEQP.SN(NDM0_Press1_TestTm + m_nUnitIdx, (int)dSpanTime);
// 		if (dSpanTime > dOfflineTestTm) {//+ dOffSetTime){
// 			m_nLast_Executing_Step = m_nStep = 10000;
// 			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Test](END)", m_szCmdName, m_nStep);
// 			break;
// 		}
// 
// 
// 		if (dSpanTime > m_tcMaxTestTimeout)
// 		{
// 			CParamList AlarmData;
// 			pTaskPressUnit->SetTestTimeOutHandlerErrorBin(m_nCmdStage, m_sysTmTestStart);
// 
// 			AlarmData.m_fnAddItemFormat("[ Station %d ]", m_nUnitIdx + 1);
// 			AlarmData.m_fnAddItemFormat("Max test time over!! [OverTime-%.2f]", dSpanTime / 1000.0);
// 			AlarmData.m_fnAddItemFormat("Handler send to query[Start test], but Tester is not the answer.");
// 			//ReportAlarm(ERR_TESTER_MAX_TEST_TIME_OVER_DN1+m_nUnitIdx, AlarmData);
// 			g_Error.AlarmReport(ERR_TESTER_MAX_TEST_TIME_OVER_DN1 + m_nUnitIdx, AlarmData, &g_TaskSystemCtrl);
// 
// 			ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
// 			break;
// 		}
		pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Test](END)", m_szCmdName, m_nStep);
		m_nLast_Executing_Step = m_nStep = 10000;
	}break;

	case 10000: // Err String
	{
		pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Data Handling](STR)", m_szCmdName, m_nStep);
		ST_DD_DEVICE stDvcData;
		BOOL bNeedPassWord = FALSE;
		int nTaskPressIdx = pTaskPressUnit->m_nUnitIdx;
		BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);

		char szCompany[16] = { 0, };
		g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

		//pTaskPressUnit->SetSiteDvcTestingInfo(eTestSite_TestEnd);

		BOOL bAllSiteFail = TRUE;

		BOOL bRandom = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_OFFLINEBIN_RANDOM);
		BOOL bYieldCut = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_OFFLINEBIN_YIELDCUT);

		int nGrrMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrMode);

		int nLoopMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoLoopMode);

		int nTestKindCnt[3] = { 0, }, nTestPassKindCnt[3] = { 0, }; // Daily Report data 0 : FT, 1: RT1, 2: RT2

		SYSTEMTIME timeEnd;
		::GetLocalTime(&timeEnd);
		g_EOT[ePressUnit_Down_1 + m_nUnitIdx] = timeEnd;

		if (bRandom == TRUE)
			srand((unsigned)time(NULL));

		for (int nY = 0; nY < m_nSiteDivY; nY++)
		{
			for (int nX = 0; nX < m_nSiteDivX; nX++)
			{
				stDvcData.clear();
				pTaskPressUnit->GetSiteDeviceData(&stDvcData, nX, nY);

				int nOnOff = pTaskPressUnit->GetSocketOnOff(nX, nY);
				char szHWbin[16] = { 0, };
				if (nOnOff == DEF_SITE_ON && stDvcData.sExist == DEF_EXIST &&  strcmp(stDvcData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) != 0)
				{
					int nCateIndexCnt = g_BinCategory.nIndexCount;
					// 0에 대한 예외 처리가 필요함.
					if (nCateIndexCnt == 0) {
						AfxMessageBox("is not accept bin!!!!! please check bin category!!");
					}

					//Error code compare result data
					CString strTRL = _T(""), strStartTm = _T(""), strEndTm = _T("");
					BOOL bChkRegCateInfo = FALSE;
					int nBinColor = -1;
					int nCurTestCnt = stDvcData.nTestingCnt;
					CString strBarcode = _T("");

					// case 1 :: category에 등록이 되어있는 경우
// 					int nRand = (rand() % 2);
// 					int nSize = g_BarcodeList.m_ve2DIDLiST.size();
// 
// 					int nSizeRand = (rand() % nSize);
// 					if (nRand == TRUE)
// 						strcpy_s(stDvcData.sz2DBarcode, sizeof(stDvcData.sz2DBarcode), g_BarcodeList.m_ve2DIDLiST[nSizeRand]);

			
					strcpy_s(stDvcData.szErrString, sizeof(stDvcData.szErrString), "PASS");
					for (int j = 0; j < g_BinCategory.nIndexCount; j++) {
						if (strcmp(stDvcData.szErrString, g_BinCategory.aCateInfo[j].szDesc) == 0)
						{
							strcpy_s(stDvcData.szBinResult, sizeof(stDvcData.szBinResult), g_BinCategory.aCateInfo[j].szSWBin);
							sprintf_s(szHWbin, sizeof(szHWbin), g_BinCategory.aCateInfo[j].szHWBin);
							stDvcData.nBinColor = eGoodBin; /*Pass/Fail 받은 것*/
							nBinColor = stDvcData.nBinColor;
							break;
						}
					}						
						
				
					sprintf_s(stDvcData.szTestSiteNum, sizeof(stDvcData.szTestSiteNum), "%d", m_nUnitIdx);

					ASSERT(nCurTestCnt >= eARB_NotTest && nCurTestCnt <= eARB_3stTestEnd);
					{	// JAM RATE DATA RECORD
						int nDvcTestCnt = nCurTestCnt;
						if (bAutoRetestUseMode == TRUE && g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoRetest) == eAutoRetest_AB && nCurTestCnt > eARB_NotTest) {
							nDvcTestCnt -= 1; //AB의 경우는 1stEnd 후에 바로 2stEnd로 바꾸어주기 때문에 빼야함.
						}

						stDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].nHwBin = atoi(szHWbin) - 1;
						stDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].nPF = nBinColor;
						stDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].nSiteNo = m_nUnitIdx;
						stDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].cSubSiteNo = CPoint(nX, nY);
						stDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].nHwBin = atoi(szHWbin) - 1;
						stDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].bQAEmpty = TRUE;

						sprintf_s(stDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrStr, sizeof(stDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrStr), stDvcData.szErrString);

						int nArtCnt = g_DMCont.m_dmEQP.GN(NDM0_ART_FT_TOTAL + (nDvcTestCnt * 2));
						g_DMCont.m_dmEQP.SN(NDM0_ART_FT_TOTAL + (nDvcTestCnt * 2), ++nArtCnt);
						if (nBinColor == eGoodBin) {
							nArtCnt = g_DMCont.m_dmEQP.GN(NDM0_ART_FT_PASS + (nDvcTestCnt * 2));
							g_DMCont.m_dmEQP.SN(NDM0_ART_FT_PASS + (nDvcTestCnt * 2), ++nArtCnt);
							nTestPassKindCnt[nDvcTestCnt]++;
						}
						nTestKindCnt[nDvcTestCnt]++;
					}
					
					double dSpanTime = (double)GetTickCount64() - m_tcCheckTime;
					g_DMCont.m_dmEQP.SN(NDM0_Press1_TestTm + m_nUnitIdx, (int)dSpanTime);
					if (nBinColor == eGoodBin && (g_TaskSystemCtrl.m_dBestTestTime > dSpanTime || g_TaskSystemCtrl.m_dBestTestTime == 0)) {
						g_TaskSystemCtrl.m_dBestTestTime = dSpanTime / 1000.0;
					}
					SYSTEMTIME timeEnd;
					::GetLocalTime(&timeEnd);
					char szCompany[256] = { 0, };
					g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
					// Record Test Result log file

					strStartTm.Format("%04d%02d%02d%02d%02d%02d%03d"
						, m_sysTmTestStart.wYear, m_sysTmTestStart.wMonth, m_sysTmTestStart.wDay
						, m_sysTmTestStart.wHour, m_sysTmTestStart.wMinute, m_sysTmTestStart.wSecond, m_sysTmTestStart.wMilliseconds);
					strEndTm.Format("%04d%02d%02d%02d%02d%02d%03d"
						, timeEnd.wYear, timeEnd.wMonth, timeEnd.wDay
						, timeEnd.wHour, timeEnd.wMinute, timeEnd.wSecond, timeEnd.wMilliseconds);

// 					char szEqpName[128] = { 0, };
// 					g_DMCont.m_dmHandlerOpt.GS(SDM11_UDP_HOST_NAME, szEqpName, sizeof(szEqpName));
// 					strTRL.Format(_T("%s,%s,%s,%d,%d,-1,-1,%s,%s,%s,%s,%s")
// 						, stDvcData.sz2DBarcode, strBarcode, szEqpName
// 						, m_nUnitIdx + 1, nX + (nY*m_nSiteDivX) + 1
// 						, (LPCTSTR)strStartTm, (LPCTSTR)strEndTm, (stDvcData.nBinColor == eGoodBin ? _T("PASS") : _T("FAIL"))
// 						, stDvcData.szErrString, szHWbin);
// 					TWLOG.MakeLog(static_cast<int>(eLogID_PRODUCTCSV), Debug, "", __LINE__, NULL, strTRL);

					stDvcData.cSubSiteNum = CPoint(nX, nY);

				
					stDvcData.nTestingCnt = nCurTestCnt = eARB_3stTestEnd;
					

					// set data
					pTaskPressUnit->SetSiteDeviceData(&stDvcData, nX, nY);
					// set fail history
					pTaskPressUnit->SetFailHistoryAndPFcnt(nX, nY, nBinColor, nCurTestCnt);

					// set hw bin history
					pTaskPressUnit->SetHwBinHistory(nX, nY, atoi(szHWbin));

					// set last error string
					g_DMCont.m_dmContac_Dn.SS(SDM1_SiteLastDesc1_1 + m_nUnitIdx*STATION_MAX_PARA + nX + nY*STATION_MAX_X_SITE, stDvcData.szErrString, sizeof(stDvcData.szErrString));

					//GRR Mode에 따른 Device 상태 변경
					
					
					pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_TestEnd);

				} // end Exist & site on
			} // end nX
		}// end nY

		//Daily Report
		ST_DAILY_REPORT stDailyData;
		stDailyData.nART_FT_Total_Cnt = nTestKindCnt[eARB_1stTestEnd - 1];
		stDailyData.nART_FT_Pass_Cnt = nTestPassKindCnt[eARB_1stTestEnd - 1];
		stDailyData.nART_RT1_Total_Cnt = nTestKindCnt[eARB_2stTestEnd - 1];
		stDailyData.nART_RT1_Pass_Cnt = nTestPassKindCnt[eARB_2stTestEnd - 1];
		stDailyData.nART_RT2_Total_Cnt = nTestKindCnt[eARB_3stTestEnd - 1];
		stDailyData.nART_RT2_Pass_Cnt = nTestPassKindCnt[eARB_3stTestEnd - 1];

		pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Data Handling](END)", m_szCmdName, m_nStep);

		// index time ( EOT )
		BOOL bChkIndex = g_TaskSystemCtrl.m_tIndexTime[m_nUnitIdx].IsChecking();
		if (bChkIndex == FALSE) {
			g_TaskSystemCtrl.m_tIndexTime[m_nUnitIdx].StartTimer();
		}

		m_nStep++;
	}break;
	case 10001:
	{
		ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
	}
	break;
	}
	return CBaseSeqCmd::State_Executing();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_QAEmptyTest::State_Pause()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskPressUnit->MakeLog("[CMD: %s] [PAUSE] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
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
BOOL CPressUnitCmd_QAEmptyTest::State_Recovery()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskPressUnit->MakeLog("[CMD: %s] [RECOVERY] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0: // 현재 위치와 마지막 위치를 비교 한다.
	{

// 		ST_DD_DEVICE pDvcData;
// 		BOOL bExistInSite = FALSE;
// 		for (int nY = 0; nY < m_nSiteDivY; nY++)
// 		{
// 			for (int nX = 0; nX < m_nSiteDivX; nX++)
// 			{
// 				pDvcData.clear();
// 				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &pDvcData);
// 
// 				int Addr = nX + nY*STATION_MAX_X_SITE + m_nUnitIdx*STATION_MAX_PARA;
// 				int nSiteOnOff = g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + Addr);
// 
// 				if ((nSiteOnOff == DEF_SITE_ON || nSiteOnOff == DEF_SITE_OFF_RESERVATION) && pDvcData.sExist == DEF_EXIST) {
// 					pDvcData.nRealTestingCnt++;
// 					pDvcData.nTestingInfo = eTestSite_Contacting;
// 					g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &pDvcData);
// 
// 					pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_Contacting);
// 				}
// 			}
// 		}


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
BOOL CPressUnitCmd_QAEmptyTest::State_NormalComplete()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	//pTaskPressUnit->SendEvent(CEID_PRESS_UNIT_TEST_END);

	
	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_QAEmptyTest::State_Alarm()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskPressUnit->MakeLog("[CMD: %s] [ALARM] [Step=%8d] [LastStep=%d]", m_szCmdName, m_nStep, m_nLast_Executing_Step);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0: // alarm 발생 했을 때 처리해5 줘야 할꺼 처리 해줌
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
BOOL CPressUnitCmd_QAEmptyTest::State_AlarmComplete()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	//pTaskPressUnit->SendEvent(CEID_PRESS_UNIT_TEST_END);

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	ST_DD_DEVICE pDvcData;
	BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);

	CString strResult = _T("");
	// Get local time
	SYSTEMTIME timeEnd;
	::GetLocalTime(&timeEnd);

	int nTestKindCnt[3] = { 0, }, nTestPassKindCnt[3] = { 0, }; // Daily Report data 0 : FT, 1: RT1, 2: RT2

	for (int nY = 0; nY < m_nSiteDivY; nY++)
	{
		for (int nX = 0; nX < m_nSiteDivX; nX++)
		{
			int nOnOff = DEF_SITE_OFF;
			
			nOnOff = pTaskPressUnit->GetSocketOnOff(nX, nY);

			// get device data
			pDvcData.clear();
			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1 + m_nUnitIdx, nX, nY, &pDvcData);

			if (nOnOff == DEF_SITE_ON && pDvcData.sExist == eDeviceData_Exist)
			{
				//Error code compare result data
				CString strTRL = _T(""), strStartTm = _T(""), strEndTm = _T("");
				char szHWbin[16] = { 0, };
				BOOL bChkRegCateInfo = FALSE;
				int  nBinColor = eFailBin;

				strcpy_s(pDvcData.szErrString, sizeof(pDvcData.szErrString), "PASS");
				for (int j = 0; j < g_BinCategory.nIndexCount; j++) {
					if (strcmp(pDvcData.szErrString, g_BinCategory.aCateInfo[j].szDesc) == 0)
					{
						strcpy_s(pDvcData.szBinResult, sizeof(pDvcData.szBinResult), g_BinCategory.aCateInfo[j].szSWBin);
						sprintf_s(szHWbin, sizeof(szHWbin), g_BinCategory.aCateInfo[j].szHWBin);
						pDvcData.nBinColor = eGoodBin; /*Pass/Fail 받은 것*/
						nBinColor = pDvcData.nBinColor;
						break;
					}
				}

				char szCompany[256] = { 0, };
				g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));


				sprintf_s(pDvcData.szTestSiteNum, sizeof(pDvcData.szTestSiteNum), "%d", m_nUnitIdx);
				pDvcData.cSubSiteNum = CPoint(nX, nY);

				// Set Testing Count
				int nTestCnt = pDvcData.nTestingCnt;
			
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

				int nArtCnt = g_DMCont.m_dmEQP.GN(NDM0_ART_FT_TOTAL + (nTestCnt * 2));
				g_DMCont.m_dmEQP.SN(NDM0_ART_FT_TOTAL + (nTestCnt * 2), ++nArtCnt);
				if (nBinColor == eGoodBin) {
					nArtCnt = g_DMCont.m_dmEQP.GN(NDM0_ART_FT_PASS + (nTestCnt * 2));
					g_DMCont.m_dmEQP.SN(NDM0_ART_FT_PASS + (nTestCnt * 2), ++nArtCnt);
					nTestPassKindCnt[nTestCnt]++;
				}
				nTestKindCnt[nTestCnt]++;

		
				pDvcData.nTestingCnt = eARB_3stTestEnd;
		

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
			}
			else if (nOnOff == DEF_SITE_ON) {
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

	// index time ( EOT )
	g_TaskSystemCtrl.m_tIndexTime[m_nUnitIdx].StartTimer();

	// Alarm
	return CBaseSeqCmd::State_AlarmComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_QAEmptyTest::State_Aborting()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	if (m_nStep_New != -1) {
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if (m_nStep_Pre != m_nStep) {
		pTaskPressUnit->MakeLog("[CMD: %s] [ABORT] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch (m_nStep)
	{
	case 0:
	{
		// 			ST_DD_DEVICE pDvcData;
		// 			for(int nInlinePara =0; nInlinePara<DEF_INLINE_4PARA; nInlinePara++){			
		// 				pDvcData.clear();
		// 				g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1+m_nUnitIdx, nInlinePara,0, &pDvcData);
		// 
		// 				if(g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1+nInlinePara + (m_nUnitIdx * DEF_INLINE_4PARA)) == DEF_SITE_ON && pDvcData.sExist == DEF_EXIST){
		// 
		// 					int nCateIndexCnt = g_BinCategory.nIndexCount;
		// 					pDvcData.nTestingInfo = eTestSite_TestEnd;
		// 					pDvcData.nBinColor    = eFailBin;
		// 					pDvcData.nTestingCnt  = eARB_3stTestEnd;
		// 
		// 					char szSiteNum[32] = {0,};
		// 					sprintf_s(szSiteNum, sizeof(szSiteNum), "%d", pTaskPressUnit->m_nUnitIdx);
		// 					strcpy_s(pDvcData.szTestSiteNum, sizeof(pDvcData.szTestSiteNum),szSiteNum );
		// 					
		// 					pDvcData.cSubSiteNum = CPoint(nInlinePara,0);
		// 
		// 					strcpy_s(pDvcData.szBinResult, sizeof(pDvcData.szBinResult), "ERROR");
		// 
		// 					g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_SITE_1+m_nUnitIdx, nInlinePara,0, &pDvcData);
		// 
		// 					pTaskPressUnit->SetSiteDvcTestingInfo(eTestSite_TestEnd);
		// 				}
		// 			}

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
BOOL CPressUnitCmd_QAEmptyTest::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete();
}
