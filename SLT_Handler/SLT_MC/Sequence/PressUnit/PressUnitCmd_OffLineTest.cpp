#include "StdAfx.h"
#include "PressUnitCmd_OffLineTest.h"
#include <random>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPressUnitCmd_OffLineTest::CPressUnitCmd_OffLineTest(void)
{
	ZeroMemory(m_bSocketDvc, sizeof(m_bSocketDvc));

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
	dOffSetTime = 0.0;

}


CPressUnitCmd_OffLineTest::~CPressUnitCmd_OffLineTest(void)
{
}


//==============================================================================
//
//==============================================================================
int CPressUnitCmd_OffLineTest::CheckParam()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	m_nUnitIdx = pTaskPressUnit->m_nUnitIdx;

	BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
	BOOL bAutoLoopUse = g_TaskSystemCtrl.GetAutoLoopUse();
	ST_DD_DEVICE stDvcData;

	m_nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	m_nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	if (bAutoGrrUse)
	{
		m_tcSotDelayTime = (double)g_DMCont.m_dmHandlerOpt.GN(NDM11_GRR_SOT_DELAY_TIME);
	}
	else {
		if(bAutoLoopUse != TRUE)
			m_tcSotDelayTime = (double)g_DMCont.m_dmHandlerOpt.GN(NDM11_SltPgm_SotDelayTm);
		else {
			m_tcSotDelayTime = (double)g_DMCont.m_dmHandlerOpt.GN(NDM11_LOOP_SOT_DELAY_TIME);
			int nMaxTestCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoLoopTestCount);

			int nRePickPlaceCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoRePickPlaceCount);
			int nReContinueCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoLoopTestRecontactCount);

			for (int nY = 0; nY < m_nSiteDivY; nY++)
			{
				for (int nX = 0; nX < m_nSiteDivX; nX++)
				{
					stDvcData.clear();
					pTaskPressUnit->GetSiteDeviceData(&stDvcData, nX, nY);

					stDvcData.stLoopTestInfo[m_nUnitIdx].nMaxTestCnt = nMaxTestCount;
					stDvcData.stLoopTestInfo[m_nUnitIdx].nSiteCntX = m_nSiteDivX;
					stDvcData.stLoopTestInfo[m_nUnitIdx].nSiteCntY = m_nSiteDivY;

					stDvcData.stLoopTestInfo[m_nUnitIdx].nRePickPlaceCnt = nRePickPlaceCnt;
					stDvcData.stLoopTestInfo[m_nUnitIdx].nReContinueCnt = nReContinueCnt;

					pTaskPressUnit->SetSiteDeviceData(&stDvcData, nX, nY);
				}
			}	
		}
	}



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

/*  Test Time Random case 1
// 	m_numbers.clear();
// 	std::random_device rd;
// 	std::mt19937 gen(rd());              
// 	std::uniform_int_distribution<> dist(1, 10000);  
// 
// 	int randomNumber = dist(gen);
// 
// 	m_numbers.push_back(randomNumber);
*/

/*  Test Time Random case 2
// 	int MAX_LENGTH = 10000;
// 	m_numbers.resize(MAX_LENGTH);
// 	for (int i = 0; i < MAX_LENGTH; i++) {
// 		m_numbers[i] = i;
// 	}
// 	random_shuffle(m_numbers.begin(), m_numbers.end());


*/

	memset(m_bSocketDvc, 0x00, sizeof(m_bSocketDvc));
	for (int nY = 0; nY < m_nSiteDivY; nY++)
	{
		for (int nX = 0; nX < m_nSiteDivX; nX++)
		{
			m_stDvcData[nX][nY].clear();
		}
	}
	return ERR_NO_ERROR;
}


//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_OffLineTest::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_OffLineTest::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_OffLineTest::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_OffLineTest::OnPause()
{
	//SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_OffLineTest::OnResume()
{
	if(GetCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Executing){

	}else{
		SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	}	
    return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_OffLineTest::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_OffLineTest::State_Executing()
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
			g_DMCont.m_dmEQP.SB(BDM0_TEST_STATUS_SITE1+m_nUnitIdx, TRUE);    // Set testing status on

			if(g_BinCategory.nIndexCount == 0){
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Bin category is not register. category count=0.");
				ReportAlarm(ERR_SYSTEM_BASE, AlarmData);
				return FALSE;
			}

			//Tester 개별 진행 상황 추가
			pTaskPressUnit->SetTestStatusHistory(CTaskPressUnit::eSetPrepare);

			// index time ( SOT )
			BOOL bChkIndex = g_TaskSystemCtrl.m_tIndexTime[m_nUnitIdx].IsChecking();
			if( bChkIndex == TRUE){
				double dIndexTm = g_TaskSystemCtrl.m_tIndexTime[m_nUnitIdx].CompareTime(FALSE);	
				g_DMCont.m_dmEQP.SD(DDM0_TmIndexPress1+m_nUnitIdx, dIndexTm);
				g_TaskSystemCtrl.m_tIndexTime[m_nUnitIdx].SetTimerCheckEnd();
				
				CString strSummaryTime = _T("");
				int nIndexCount = g_DMCont.m_dmContac_Dn.GN(NDM1_INDEX_RECORD_IGNORE_STATION1 + m_nUnitIdx);
				if (nIndexCount < MAX_IGNORE_INDEX_REC_COUNT || g_TaskAllAutoStacker.GetLoadTraySupplyEnd() == TRUE || g_TaskSystemCtrl.IsOneCycleModeOn() == true)
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

				pTaskPressUnit->MakeLog("Index Time, Press %d, %.2f, %s", m_nUnitIdx+1, dIndexTm, strSummaryTime);
			}

			ST_DD_DEVICE pDvcData;
			BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
			for( int nY=0; nY < m_nSiteDivY; nY++ )
			{
				for( int nX=0; nX < m_nSiteDivX; nX++ )
				{
					pDvcData.clear();
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1+m_nUnitIdx, nX, nY, &pDvcData);

					int nSiteOnOff = (bAutoGrrUse == TRUE ? pTaskPressUnit->GetSocketOnOffGrr(nX, nY) : pTaskPressUnit->GetSocketOnOff(nX, nY));

					if( nSiteOnOff == DEF_SITE_ON && pDvcData.sExist == DEF_EXIST && strcmp(pDvcData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) != 0 ){
						pDvcData.nRealTestingCnt++;
						g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_SITE_1+m_nUnitIdx, nX, nY, &pDvcData);

						pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_TestStart);

						m_bSocketDvc[nX][nY] = TRUE;
						m_stDvcData[nX][nY] = pDvcData;
					}

					pTaskPressUnit->MakeLog("[SND H->T %d-%d-%d 2D Barcode Id = %s", m_nUnitIdx + 1, nX + 1, nY + 1, pDvcData.sz2DBarcode);
				}
			}

			g_DMCont.m_dmEQP.SN(NDM0_Press1_TestTm+m_nUnitIdx, 0);

			::GetLocalTime(&m_sysTmTestStart);
			g_SOT[ePressUnit_Down_1 + m_nUnitIdx] = m_sysTmTestStart;

			//Loading Table 미리 대기를 위한 Contact PrePare 상태 변경.
			g_DMCont.m_dmContac_Up.SN(NDM2_PRESS_1_PRE_CONTACT_STATUS + m_nUnitIdx, FALSE);


			m_tcCheckTime = (double)GetTickCount64();
			m_nStep = 5000;
			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Test](STR)",m_szCmdName ,m_nStep);
			pTaskPressUnit->SetTestStatusHistory(CTaskPressUnit::eSetStatus);


		}break;
	case 5000:
		{
		

			//double dOffSetTime = (m_nUnitIdx % 2 == 0 ? 0 : -20000);
			double dSpanTime = (double)GetTickCount64() - m_tcCheckTime;
			double dOfflineTestTm = (double)g_DMCont.m_dmHandlerOpt.GN(NDM11_TM_OffLineTest);
			g_DMCont.m_dmEQP.SN(NDM0_Press1_TestTm+m_nUnitIdx, (int)dSpanTime);

			if(dSpanTime > dOfflineTestTm/* + m_numbers[0]*/){//+ dOffSetTime){
				m_nLast_Executing_Step = m_nStep = 10000;				
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Test](END)",m_szCmdName ,m_nStep);
				break;
			}

			if (dSpanTime > m_tcMaxTestTimeout)
			{
				CParamList AlarmData;
				pTaskPressUnit->SetTestTimeOutHandlerErrorBin(m_nCmdStage, m_sysTmTestStart);

				AlarmData.m_fnAddItemFormat("[ Station %d ]", m_nUnitIdx + 1);
				AlarmData.m_fnAddItemFormat("Max test time over!! [OverTime-%.2f]", dSpanTime / 1000.0);
				AlarmData.m_fnAddItemFormat("Handler send to query[Start test], but Tester is not the answer.");
				//ReportAlarm(ERR_TESTER_MAX_TEST_TIME_OVER_DN1+m_nUnitIdx, AlarmData);
				g_Error.AlarmReport(ERR_TESTER_MAX_TEST_TIME_OVER_DN1 + m_nUnitIdx, AlarmData, &g_TaskSystemCtrl);

				ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
				break;
			}
		}break;

	case 10000: // Err String
		{
			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Data Handling](STR)",m_szCmdName ,m_nStep);
			ST_DD_DEVICE stDvcData;
			BOOL bNeedPassWord      = FALSE;
			int nTaskPressIdx       = pTaskPressUnit->m_nUnitIdx;
			BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);

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

			BOOL bGrrCancel = g_TaskSystemCtrl.GetAutoGrrCancel();
			BOOL bLoopCancel = g_TaskSystemCtrl.GetAutoLoopCancel();


			BOOL bAutoGrrUseMode = g_TaskSystemCtrl.GetAutoGrrUse();
			BOOL bSwapOption = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrDeviceSwapUse);
			BOOL bAutoLoopUseMode = g_TaskSystemCtrl.GetAutoLoopUse();

			BOOL bAllSiteFail = TRUE;

			BOOL bRandom = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_OFFLINEBIN_RANDOM);
			BOOL bYieldCut = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_OFFLINEBIN_YIELDCUT);

			int nGrrMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrMode);

			int nLoopMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoLoopMode);

			int nTestKindCnt[3] = { 0, }, nTestPassKindCnt[3] = { 0, }; // Daily Report data 0 : FT, 1: RT1, 2: RT2

			if(bRandom==TRUE)
				srand((unsigned)time(NULL));
			
			for( int nY=0; nY < m_nSiteDivY; nY++ )
			{
				for( int nX=0; nX < m_nSiteDivX; nX++ )
				{
gotoTestResultRetry:
					stDvcData.clear();
					pTaskPressUnit->GetSiteDeviceData(&stDvcData, nX, nY);

					int nOnOff = (bAutoGrrUseMode == TRUE ? pTaskPressUnit->GetSocketOnOffGrr(nX, nY) : pTaskPressUnit->GetSocketOnOff(nX, nY));

					if( nOnOff == DEF_SITE_ON && stDvcData.sExist == DEF_EXIST &&  strcmp(stDvcData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) != 0 )
					{
						int nCateIndexCnt = g_BinCategory.nIndexCount;
						// 0에 대한 예외 처리가 필요함.
						if(nCateIndexCnt == 0){
							AfxMessageBox("is not accept bin!!!!! please check bin category!!");
						}

						//Error code compare result data
						CString strTRL = _T(""), strStartTm = _T(""), strEndTm = _T("");
						BOOL bChkRegCateInfo = FALSE;
						int nBinColor = -1;
						int nCurTestCnt = stDvcData.nTestingCnt;

						// case 1 :: category에 등록이 되어있는 경우
						if (bRandom == FALSE)
							srand((unsigned int)time(NULL));
						int nRandExist = 0;

						bool bSkipToFailBin = false;

						// bin category에 pass/fail이 모두 들어있는가?
						if (m_bChkPassFail == true && bYieldCut == TRUE )
						{
							int nArtTtCnt = g_DMCont.m_dmEQP.GN(NDM0_ART_FT_TOTAL);
							int nArtGdCnt = g_DMCont.m_dmEQP.GN(NDM0_ART_FT_PASS);
							double FirstYield = (nArtTtCnt > 0 ? ((double)nArtGdCnt / (double)nArtTtCnt) * 100.0 : 100);
							double dYieldCutPer = g_DMCont.m_dmHandlerOpt.GD(DDM11_OffLine_YieldCutPer);
							if ( FirstYield < dYieldCutPer )
							{
								bSkipToFailBin = true;
							}
						}
						

						while(1){
							//srand((unsigned)time(NULL));
							Sleep(1);
							nRandExist = rand()%nCateIndexCnt;
							if (strcmp(g_BinCategory.aCateInfo[nRandExist].szDesc, DEF_2D_READ_ERR_DEVICE) != 0
								&& strcmp(g_BinCategory.aCateInfo[nRandExist].szDesc, DEF_HANDLER_ERR_DEVICE) != 0
								&& strcmp(g_BinCategory.aCateInfo[nRandExist].szDesc, DEF_TEST_TIME_OUT_ERR_DEVICE) != 0
								&& strcmp(g_BinCategory.aCateInfo[nRandExist].szDesc, DEF_TEST_RESULT_TP101) != 0)
							{
								if (bSkipToFailBin == true && atoi(g_BinCategory.aCateInfo[nRandExist].szPassFail) == eFailBin)
								{
									continue;
								}
								break;
							}								
						}
						char szHWBin[16] = {0,};

						if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AlwayBin1Sorting) == TRUE)
						{
							BOOL bChkRegCateInfo = FALSE;
							strcpy_s(stDvcData.szErrString, sizeof(stDvcData.szErrString), "PASS");
							for (int i = 0; i < g_BinCategory.nIndexCount; i++) {
								if (strcmp(g_BinCategory.aCateInfo[i].szDesc, stDvcData.szErrString) == 0)
								{
									strcpy_s(stDvcData.szBinResult, sizeof(stDvcData.szBinResult), g_BinCategory.aCateInfo[i].szSWBin);
									strcpy_s(stDvcData.szErrorCode, sizeof(stDvcData.szErrorCode), g_BinCategory.aCateInfo[nRandExist].szCode);
									strcpy_s(stDvcData.szTesterHWBin, sizeof(stDvcData.szTesterHWBin), g_BinCategory.aCateInfo[nRandExist].szHWBin_Tester);
									sprintf_s(szHWBin, sizeof(szHWBin), g_BinCategory.aCateInfo[i].szHWBin);
									stDvcData.nBinColor = nBinColor = atoi(g_BinCategory.aCateInfo[i].szPassFail); /*Pass/Fail 받은 것*/
									bChkRegCateInfo = TRUE;
								}
							}

							if (bChkRegCateInfo == FALSE) {
								strcpy_s(stDvcData.szBinResult, sizeof(stDvcData.szBinResult), g_BinCategory.aCateInfo[nRandExist].szSWBin);
								strcpy_s(stDvcData.szErrString, sizeof(stDvcData.szErrString), g_BinCategory.aCateInfo[nRandExist].szDesc);
								strcpy_s(stDvcData.szErrorCode, sizeof(stDvcData.szErrorCode), g_BinCategory.aCateInfo[nRandExist].szCode);
								strcpy_s(stDvcData.szTesterHWBin, sizeof(stDvcData.szTesterHWBin), g_BinCategory.aCateInfo[nRandExist].szHWBin_Tester);
								strcpy_s(szHWBin, sizeof(szHWBin), g_BinCategory.aCateInfo[nRandExist].szHWBin);
								stDvcData.nBinColor = nBinColor = atoi(g_BinCategory.aCateInfo[nRandExist].szPassFail);
							}
						}
						else{
							strcpy_s(stDvcData.szBinResult, sizeof(stDvcData.szBinResult), g_BinCategory.aCateInfo[nRandExist].szSWBin);
							strcpy_s(stDvcData.szErrString, sizeof(stDvcData.szErrString), g_BinCategory.aCateInfo[nRandExist].szDesc);
							strcpy_s(stDvcData.szErrorCode, sizeof(stDvcData.szErrorCode), g_BinCategory.aCateInfo[nRandExist].szCode);
							strcpy_s(stDvcData.szTesterHWBin, sizeof(stDvcData.szTesterHWBin), g_BinCategory.aCateInfo[nRandExist].szHWBin_Tester);
							strcpy_s(szHWBin, sizeof(szHWBin), g_BinCategory.aCateInfo[nRandExist].szHWBin);			
							stDvcData.nBinColor = nBinColor = atoi(g_BinCategory.aCateInfo[nRandExist].szPassFail);
						}

						sprintf_s( stDvcData.szTestSiteNum, sizeof( stDvcData.szTestSiteNum ), "%d", m_nUnitIdx );

						if (bAutoGrrUseMode != TRUE)
						{
							if (bAutoLoopUseMode != TRUE) {  //FT Mode
								ASSERT(nCurTestCnt >= eARB_NotTest && nCurTestCnt <= eARB_3stTestEnd);
								{	// JAM RATE DATA RECORD
									int nDvcTestCnt = nCurTestCnt;
									if (bAutoRetestUseMode == TRUE && g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoRetest) == eAutoRetest_AB && nCurTestCnt > eARB_NotTest) {
										nDvcTestCnt -= 1; //AB의 경우는 1stEnd 후에 바로 2stEnd로 바꾸어주기 때문에 빼야함.
									}

									stDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].nHwBin = atoi(szHWBin) - 1;
									stDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].nPF = nBinColor;
									stDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].nSiteNo = m_nUnitIdx;
									stDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].cSubSiteNo = CPoint(nX, nY);

									sprintf_s(stDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrStr, sizeof(stDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrStr), stDvcData.szErrString);
									sprintf_s(stDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrCode, sizeof(stDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrCode), stDvcData.szErrorCode);
									sprintf_s(stDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szTesterHwBin, sizeof(stDvcData.stRecInfo.stDvcInfo[nDvcTestCnt].szTesterHwBin), stDvcData.szTesterHWBin);

									int nArtCnt = g_DMCont.m_dmEQP.GN(NDM0_ART_FT_TOTAL + (nDvcTestCnt * 2));
									g_DMCont.m_dmEQP.SN(NDM0_ART_FT_TOTAL + (nDvcTestCnt * 2), ++nArtCnt);
									if (nBinColor == eGoodBin) {
										nArtCnt = g_DMCont.m_dmEQP.GN(NDM0_ART_FT_PASS + (nDvcTestCnt * 2));
										g_DMCont.m_dmEQP.SN(NDM0_ART_FT_PASS + (nDvcTestCnt * 2), ++nArtCnt);
										nTestPassKindCnt[nDvcTestCnt]++;
									}
									nTestKindCnt[nDvcTestCnt]++;
								}
								pTaskPressUnit->PushYieldData(nX, nY, nBinColor, stDvcData.nTestingCnt);
							}
							else{
								if(bLoopCancel == FALSE) //Loop Mode
									stDvcData.stLoopTestInfo[m_nUnitIdx].testDoneAll();	
								else {
									int nMaxTestCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoLoopTestCount);
									for (int nTestCnt = stDvcData.stLoopTestInfo[m_nUnitIdx].nTestCnt[nX][nY]; nTestCnt < nMaxTestCount; nTestCnt++)
									{
										stDvcData.stLoopTestInfo[m_nUnitIdx].testDoneAll();
									}
								}
							}				
						}
						else { // Grr Mode 

							int nFindIdx = stDvcData.findIndexTestSite(stDvcData.szTestSiteNum);
							ASSERT(nFindIdx >= 0);

							if (bGrrCancel == TRUE) {
								int nMaxTestCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrTestCount);
								for (int nTestCnt = stDvcData.stGrrTestInfo[nFindIdx].nTestCnt[nX][nY]; nTestCnt < nMaxTestCount; nTestCnt++)
								{
									stDvcData.stGrrTestInfo[nFindIdx].testDoneAll();
								}
							}
							else { /*bGrrCancel == FALSE*/
 								if (bSwapOption == TRUE) {
 									stDvcData.stGrrTestInfo[nFindIdx].testDone(nX, nY);	// 해당 Site의 Test 횟수를 증가시켜줌
 								}
 								else {
 									stDvcData.stGrrTestInfo[nFindIdx].testDoneAll();	// 해당 Site의 Test 횟수를 증가시켜줌
 								}
							}

							// Create DB Data
							_tGRR_DATA stGrrData;
							sprintf(stGrrData.szUnique, "%s", stDvcData.szUniqueID);
							sprintf(stGrrData.sz2DID, "%s", stDvcData.sz2DBarcode);
							sprintf(stGrrData.szSiteNum, "%d", atoi(stDvcData.szTestSiteNum) + 1);
							stGrrData.nTestCntNo = stDvcData.stGrrTestInfo[nFindIdx].nTestCnt[nX][nY];
							sprintf(stGrrData.szSubSiteNo, "%d", (nX + 1 + (nY * STATION_MAX_X_SITE)));
							sprintf(stGrrData.szTestResult, (atoi(stDvcData.szBinResult) == eGoodBin ? "PASS" : "FAIL"));
							sprintf(stGrrData.szResultDescription, "%s", stDvcData.szErrString);

							g_DB.ASYNC_INSERT_GRRDATA_TBL(stGrrData);

							char szGrrSiteHis[MAX_PATH];//{ 0, };
							g_DMCont.m_dmContac_Dn.GS(SDM1_SiteGrrPF_History1_1 + m_nUnitIdx*STATION_MAX_PARA + nX + nY * STATION_MAX_X_SITE, szGrrSiteHis, sizeof(szGrrSiteHis));
							CString strGrrSiteHis = _T("");
							strGrrSiteHis.Format("%s%s", szGrrSiteHis, (atoi(stDvcData.szBinResult) == eGoodBin ? "P" : "F"));
							int nlength = strGrrSiteHis.GetLength();
							if(nlength >= MAX_STRING_LENGTH)
								g_DMCont.m_dmContac_Dn.SS(SDM1_SiteGrrPF_History1_1 + m_nUnitIdx*STATION_MAX_PARA + nX + nY * STATION_MAX_X_SITE, (LPSTR)(LPCTSTR)strGrrSiteHis, strGrrSiteHis.GetLength() + 1);
						}
						double dSpanTime = (double)GetTickCount64() - m_tcCheckTime;
						g_DMCont.m_dmEQP.SN(NDM0_Press1_TestTm+m_nUnitIdx, (int)dSpanTime);
						if( nBinColor == eGoodBin && (g_TaskSystemCtrl.m_dBestTestTime > dSpanTime || g_TaskSystemCtrl.m_dBestTestTime == 0) ){
							g_TaskSystemCtrl.m_dBestTestTime = dSpanTime/1000.0;
						}

						SYSTEMTIME timeEnd;
						::GetLocalTime( &timeEnd );
//						g_EOT[ePressUnit_Down_1 + m_nUnitIdx] = timeEnd;
						char szCompany[256] = {0,};
						g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
						// Record Test Result log file
						
						strStartTm.Format("%04d%02d%02d%02d%02d%02d%03d"
							, m_sysTmTestStart.wYear, m_sysTmTestStart.wMonth, m_sysTmTestStart.wDay
							, m_sysTmTestStart.wHour, m_sysTmTestStart.wMinute, m_sysTmTestStart.wSecond, m_sysTmTestStart.wMilliseconds);
						strEndTm.Format("%04d%02d%02d%02d%02d%02d%03d"
							, timeEnd.wYear, timeEnd.wMonth, timeEnd.wDay
							, timeEnd.wHour, timeEnd.wMinute, timeEnd.wSecond, timeEnd.wMilliseconds);
						

						int nStationID = ((STATION_MAX_PARA*m_nUnitIdx) + nX + nY*m_nSiteDivX) + 1;

						char szEqpName[128] = {0,};
						g_DMCont.m_dmHandlerOpt.GS(SDM11_UDP_HOST_NAME, szEqpName, sizeof(szEqpName));

						//송도 Amkor 요청사항. 대문자 -> 소문자 변환
						CString strBinResult = (stDvcData.nBinColor == eGoodBin) ? _T("PASS") : _T("FAIL");
						strBinResult.MakeLower();

						strTRL.Format(_T("%s,%d,%s,%s,%s,%s,%s")
							,stDvcData.sz2DBarcode, nStationID
							, (LPCTSTR)strStartTm, (LPCTSTR)strEndTm, strBinResult/*(stDvcData.nBinColor == eGoodBin ? _T("PASS") : _T("FAIL"))*/
							,stDvcData.szErrString, szHWBin);
						TWLOG.MakeLog(static_cast<int>(eLogID_PRODUCTCSV), Debug, "", __LINE__, NULL, strTRL);
//						TWLOG.MakeLog(static_cast<int>(eLogID_2DIDCSV), Debug, "", __LINE__, NULL, strTRL);

						stDvcData.cSubSiteNum = CPoint(nX,nY);
						
						BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stDvcData);

						if (strcmp(stDvcData.szErrString, DEF_TEST_RESULT_TP101) != 0 && bIsDefaultBin != TRUE)
						{
							// change status and auto retest
							int nCaclTestCnt = pTaskPressUnit->GetCalcTestingCnt(nCurTestCnt, stDvcData);
							stDvcData.nTestingCnt = nCurTestCnt = nCaclTestCnt;
						}
										
						// set data
						pTaskPressUnit->SetSiteDeviceData(&stDvcData, nX, nY);

						// set fail history
						pTaskPressUnit->SetFailHistoryAndPFcnt(nX, nY, nBinColor, nCurTestCnt);

						// set hw bin history
						pTaskPressUnit->SetHwBinHistory(nX, nY, atoi(szHWBin) );

						// set last error string
						g_DMCont.m_dmContac_Dn.SS(SDM1_SiteLastDesc1_1 + m_nUnitIdx*STATION_MAX_PARA + nX + nY*STATION_MAX_X_SITE, stDvcData.szErrString, sizeof(stDvcData.szErrString));

						//GRR Mode에 따른 Device 상태 변경
						if (bAutoGrrUseMode) {
							int nFindIdx = 0;
							nFindIdx = stDvcData.findIndexTestSite(stDvcData.szTestSiteNum);
							ASSERT(nFindIdx >= 0);


							BOOL bChkTestEnd = (bSwapOption == TRUE) ? stDvcData.stGrrTestInfo[nFindIdx].chkTestEnd(nX, nY) : stDvcData.stGrrTestInfo[nFindIdx].chkTestEnd();
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
						}				
						else {
							// set test end						
							if(bAutoLoopUseMode == TRUE){
								if (stDvcData.stLoopTestInfo[m_nUnitIdx].chkTestEnd() == TRUE) {
									pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_TestEnd);
								}
								else{
									if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0)
									{
										if (nLoopMode == ePressContinueTest)
											pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_DvcLoading);
										else
											pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_TestEnd);
									}
									else {
										if (stDvcData.stLoopTestInfo[m_nUnitIdx].chkRePickPlace() == FALSE)
											pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_DvcLoading);
										else
											pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_TestEnd);
									}

								}								
							}			
							else{
								pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_TestEnd);
							}
						}

						// Station Site On 되어있는 All Site Fail 확인
						if (stDvcData.nBinColor == eGoodBin) {
							bAllSiteFail = FALSE;
						}




					} // Socket Device Data 읽지 못하는 문제로 Data 처리 안되는 현상 발생 함. Retry 후 back Up 했던 Device Data를 다시 넣어 준다. [5/27/2025 donghyun.shin]
					else if (nOnOff == DEF_SITE_ON && stDvcData.sExist == DEF_NOT_EXIST && strcmp(stDvcData.szBinResult, DEF_SOCKET_CLEAN_DEVICE) != 0 &&
						m_bSocketDvc[nX][nY] == TRUE)
					{
						pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [TesterResult SetSiteDeviceData goto] [x=%d,y%d]", m_szCmdName, m_nStep, nX, nY);
						pTaskPressUnit->SetSiteDeviceData(&m_stDvcData[nX][nY], nX, nY);
						Sleep(200);
						goto gotoTestResultRetry;
					}// end Exist & site on
					else if(nOnOff == DEF_SITE_ON){
						pTaskPressUnit->SetNotTestHistory(nX, nY);

						// Station Site On 되어있는 All Site Fail 확인
						bAllSiteFail = FALSE;
					}
					
				} // end nX
			}// end nY

			SYSTEMTIME timeEnd;
			::GetLocalTime(&timeEnd);
			g_EOT[ePressUnit_Down_1 + m_nUnitIdx] = timeEnd;


			 // Station Site On 되어있는 All Site 연속 Fail Count
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

			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Data Handling](END)",m_szCmdName ,m_nStep);

			// index time ( EOT )
			BOOL bChkIndex = g_TaskSystemCtrl.m_tIndexTime[m_nUnitIdx].IsChecking();
			if (bChkIndex == FALSE) {
				g_TaskSystemCtrl.m_tIndexTime[m_nUnitIdx].StartTimer();
			}
				
// 			CString strData = _T("");
// 			g_TaskPressUnit->SetTextBinSolution(strData, m_nUnitIdx);
// 

// 			char strMultibyte[] = _T("TEST");
// 			strcpy_s(strMultibyte, sizeof(strMultibyte), strMultibyte);
		/*	유니코드
			wchar_t strUnicode[512] = { 0, };
			int nLen = MultiByteToWideChar(CP_ACP, 0, str, str.GetLength(), NULL, NULL);
			MultiByteToWideChar(CP_ACP, 0, str, str.GetLength(), strUnicode, nLen);
			
			g_pConectSocket->Send(strUnicode, nLen);
		*/
// 			wchar_t strUnicode[512] = { 0, };
// 			int nLen = MultiByteToWideChar(CP_ACP, 0, strData, strData.GetLength(), NULL, NULL);
//			MultiByteToWideChar(CP_ACP, 0, strData, strData.GetLength(), strUnicode, nLen);
// 			if(g_pConectSocket != NULL)
// 				g_pConectSocket->Send(strUnicode, sizeof(strUnicode));

			//Press가 LD/UD Table 필요한 상태
//			pTaskPressUnit->SetPressReqTable(TRUE, CTaskLoadTable::eMaxTblType);


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
BOOL CPressUnitCmd_OffLineTest::State_Pause()
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
BOOL CPressUnitCmd_OffLineTest::State_Recovery()
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

			ST_DD_DEVICE pDvcData;
			BOOL bExistInSite = FALSE;
			for( int nY=0; nY < m_nSiteDivY; nY++ )
			{
				for( int nX=0; nX < m_nSiteDivX; nX++ )
				{
					pDvcData.clear();
					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1+m_nUnitIdx, nX, nY, &pDvcData);

					int Addr = nX + nY*STATION_MAX_X_SITE + m_nUnitIdx*STATION_MAX_PARA;
					int nSiteOnOff = g_DMCont.m_dmHandlerOpt.GN( NDM11_SocketOnOff_Site1_1+ Addr);

					if( (nSiteOnOff == DEF_SITE_ON || nSiteOnOff == DEF_SITE_OFF_RESERVATION || nSiteOnOff == DEF_SITE_OFF_RESERVATION_YIELD)&& pDvcData.sExist == DEF_EXIST){
						pDvcData.nRealTestingCnt++;
						pDvcData.nTestingInfo = eTestSite_Contacting;
						g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_SITE_1+m_nUnitIdx, nX, nY, &pDvcData);

						pTaskPressUnit->SetSiteDvcTestingInfo(nX, nY, eTestSite_Contacting);
					}
				}
			}
				

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
BOOL CPressUnitCmd_OffLineTest::State_NormalComplete()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	//pTaskPressUnit->SendEvent(CEID_PRESS_UNIT_TEST_END);

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
		BOOL bSiteError = FALSE;
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
			g_Error.AlarmReport(ERR_TESTER_DEFAULT_BIN_FAIL_STATION_1 + m_nUnitIdx, AlarmData, &g_TaskSystemCtrl,vSiteOff);
			strErrMsg.Empty();
		}


		if (g_DMCont.m_dmHandlerOpt.GB(BDM11_SltPgm_SiteOff_TestTimeOut))
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
BOOL CPressUnitCmd_OffLineTest::State_Alarm()
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
BOOL CPressUnitCmd_OffLineTest::State_AlarmComplete()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	//pTaskPressUnit->SendEvent(CEID_PRESS_UNIT_TEST_END);

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	ST_DD_DEVICE pDvcData;
	BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);

	BOOL bAutoGrrUseMode = g_TaskSystemCtrl.GetAutoGrrUse();
	BOOL bAutoLoopUseMode = g_TaskSystemCtrl.GetAutoLoopUse();

	BOOL bGrrCancel = g_TaskSystemCtrl.GetAutoGrrCancel();
	BOOL bLoopCancel = g_TaskSystemCtrl.GetAutoLoopCancel();

	
	CString strResult = _T("");
	// Get local time
	SYSTEMTIME timeEnd;
	::GetLocalTime( &timeEnd );
	//g_EOT[ePressUnit_Down_1 + m_nUnitIdx] = timeEnd;

	int nTestKindCnt[3] = { 0, }, nTestPassKindCnt[3] = { 0, }; // Daily Report data 0 : FT, 1: RT1, 2: RT2

	for(int nY=0; nY< m_nSiteDivY  ; nY++)
	{
		for(int nX=0; nX< m_nSiteDivX; nX++)
		{
			int nOnOff = DEF_SITE_OFF;
			if (bAutoGrrUseMode == TRUE)
				nOnOff = pTaskPressUnit->GetSocketOnOffGrr(nX, nY);
			else
				nOnOff = pTaskPressUnit->GetSocketOnOff(nX, nY);

			// get device data
			pDvcData.clear();
			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_TEST_SITE_1+m_nUnitIdx, nX, nY, &pDvcData);

			if (nOnOff == DEF_SITE_ON && pDvcData.sExist == eDeviceData_Exist)
			{
				//Error code compare result data
				CString strTRL = _T(""), strStartTm = _T(""), strEndTm = _T("");
				char szHWbin[16] = { 0, };
				BOOL bChkRegCateInfo = FALSE;
				int  nBinColor = eFailBin;

// 				// sot error?
				int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

				strcpy_s(pDvcData.szErrString, sizeof(pDvcData.szErrString), DEF_HANDLER_ERR_DEVICE);
				pTaskPressUnit->MakeLog("Skip to online test");
				strcpy_s(pDvcData.szBinResult, sizeof(pDvcData.szBinResult), "98");
				sprintf_s(szHWbin, sizeof(szHWbin), "4");
				pDvcData.nBinColor = nBinColor;
				strResult = "fail";
				
// sot error?
				//for (int i = 0; i < g_BinCategory.nIndexCount; i++)
				//{
				//	if (strcmp(pDvcData.szErrString, g_BinCategory.aCateInfo[i].szDesc) == 0) {
				//		pTaskPressUnit->MakeLog("Skip to online test");
				//		strcpy_s(pDvcData.szBinResult, sizeof(pDvcData.szBinResult), g_BinCategory.aCateInfo[i].szSWBin);
				//		sprintf_s(szHWbin, sizeof(szHWbin), g_BinCategory.aCateInfo[i].szHWBin);
				//		// Bin Color = Msg Data Pass/Fail
				//		pDvcData.nBinColor = atoi(g_BinCategory.aCateInfo[i].szPassFail); /*Pass/Fail 받은 것*/
				//		break;
				//	}
				//}

				char szCompany[256] = { 0, };
				g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

				// Record Test Result log file
				strStartTm.Format("%04d%02d%02d%02d%02d%02d%03d"
					, m_sysTmTestStart.wYear, m_sysTmTestStart.wMonth, m_sysTmTestStart.wDay
					, m_sysTmTestStart.wHour, m_sysTmTestStart.wMinute, m_sysTmTestStart.wSecond, m_sysTmTestStart.wMilliseconds);
				strEndTm.Format("%04d%02d%02d%02d%02d%02d%03d"
					, timeEnd.wYear, timeEnd.wMonth, timeEnd.wDay
					, timeEnd.wHour, timeEnd.wMinute, timeEnd.wSecond, timeEnd.wMilliseconds);

				int nStationID = ((STATION_MAX_PARA*m_nUnitIdx) + nX + nY*m_nSiteDivX) + 1;

				//송도 Amkor 요청사항. 대문자 -> 소문자 변환
				CString strBinResult = (pDvcData.nBinColor == eGoodBin) ? _T("PASS") : _T("FAIL");
				strBinResult.MakeLower();

				strTRL.Format(_T("%s,%d,%s,%s,%s,%s,%s")
					, pDvcData.sz2DBarcode, nStationID
					, (LPCTSTR)strStartTm, (LPCTSTR)strEndTm, strBinResult/*(pDvcData.nBinColor == eGoodBin ? _T("PASS") : _T("FAIL"))*/
					, pDvcData.szErrString, szHWbin);
				TWLOG.MakeLog(static_cast<int>(eLogID_PRODUCTCSV), Debug, "", __LINE__, NULL, strTRL);
//				TWLOG.MakeLog(static_cast<int>(eLogID_2DIDCSV), Debug, "", __LINE__, NULL, strTRL);

				sprintf_s(pDvcData.szTestSiteNum, sizeof(pDvcData.szTestSiteNum), "%d", m_nUnitIdx);
				pDvcData.cSubSiteNum = CPoint(nX, nY);

				// Set Testing Count
				int nTestCnt = pDvcData.nTestingCnt;
				if (bAutoGrrUseMode != TRUE)
				{	// JAM RATE DATA RECORD
					if (bAutoLoopUseMode != TRUE) {
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
							pDvcData.stGrrTestInfo[nFindIdx].testDone(nX, nY);
						}
						else {
							pDvcData.stGrrTestInfo[nFindIdx].testDoneAll();
						}
					}



// 					if (bGrrCancel == FALSE) {
// 						pDvcData.stGrrTestInfo[nFindIdx].testDoneAll();	// 해당 Site의 Test 횟수를 증가시켜줌
// 					}
// 					else
// 					{
// 						int nMaxTestCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrTestCount);
// 						for (int nTestCnt = pDvcData.stGrrTestInfo[nFindIdx].nTestCnt[nX][nY]; nTestCnt < nMaxTestCount; nTestCnt++)
// 						{
// 							pDvcData.stGrrTestInfo[nFindIdx].testDoneAll();
// 						}
// 					}

																		// Create DB Data
					_tGRR_DATA stGrrData;
					sprintf(stGrrData.szUnique, "%s", pDvcData.szUniqueID);
					sprintf(stGrrData.sz2DID, "%s", pDvcData.sz2DBarcode);
					sprintf(stGrrData.szSiteNum, "%d", atoi(pDvcData.szTestSiteNum) + 1);
					stGrrData.nTestCntNo = pDvcData.stGrrTestInfo[nFindIdx].nTestCnt[nX][nY];
					sprintf(stGrrData.szSubSiteNo, "%d", (nX + 1 + (nY * STATION_MAX_X_SITE)) );
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

				//int nCaclTestCnt = pTaskPressUnit->GetCalcTestingCnt(nTestCnt, pDvcData);
				//pDvcData.nTestingCnt = nTestCnt = nCaclTestCnt;
				
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

	g_TaskSystemCtrl.AddDailyReportData(stDailyData);

	// index time ( EOT )
	g_TaskSystemCtrl.m_tIndexTime[m_nUnitIdx].StartTimer();

	pTaskPressUnit->CalYieldMonitoring();
    // Alarm
	return CBaseSeqCmd::State_AlarmComplete(); 
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_OffLineTest::State_Aborting()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	if( m_nStep_New != -1 ){
		m_nStep = m_nStep_New;
		m_nStep_New = -1;
	}
	if( m_nStep_Pre != m_nStep ){
		pTaskPressUnit->MakeLog("[CMD: %s] [ABORT] [Step=%8d]", m_szCmdName, m_nStep);
	}
	m_nStep_Pre = m_nStep;

	switch( m_nStep )
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
BOOL CPressUnitCmd_OffLineTest::State_AbortComplete()
{
	return CBaseSeqCmd::State_AbortComplete(); 
}
