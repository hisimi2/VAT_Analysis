#include "StdAfx.h"
#include "PressUnitCmd_OnLineLoopTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPressUnitCmd_OnLineLoopTest::CPressUnitCmd_OnLineLoopTest(void)
{
    m_nCmdId = eSeqCmd_PressUnit_OnlineLoopTest;
	m_nUnitIdx=0;
	StringCchCopy(m_szCmdName, _countof(m_szCmdName), CString(__func__));

    m_bSotErr        = FALSE;
    m_bWaitResultErr = FALSE;
    m_bNotRevData    = FALSE;
	m_tcSotDelayTime = 0.0;
	m_nTesterIndex = 0;

	m_nSiteCntX = 0;
	m_nSiteCntY = 0;
	m_nBinSortType = eSortType_SwBin;
	m_bTesterErr = FALSE;

	m_nCmdStage = 0;
	m_tcTimeout = 0.0;
	m_tcMaxTestTimeout = 0.0;
	m_tcTesterTestTime = 0.0;
	m_b2dMisMatch = FALSE;

	m_nLoopCount = 0;
	m_nRealTestCnt = 0;

	ZeroMemory(m_stActionParam, sizeof(ST_TESTER_ACTION_PARAM) * STATION_MAX_PARA);
	memset(m_bDutUseNotUse, 0, sizeof(m_bDutUseNotUse));
}


CPressUnitCmd_OnLineLoopTest::~CPressUnitCmd_OnLineLoopTest(void)
{

}


//==============================================================================
//
//==============================================================================
int CPressUnitCmd_OnLineLoopTest::CheckParam()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	m_nUnitIdx = pTaskPressUnit->m_nUnitIdx;

	m_tcMaxTestTimeout = (double)g_DMCont.m_dmHandlerOpt.GN(NDM11_SltPgm_TestTmOut);

	m_nSiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	m_nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	m_nBinSortType = g_DMCont.m_dmEQP.GN(NDM0_SortType);// enum eSortingType{   

	return ERR_NO_ERROR;
}


//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_OnLineLoopTest::OnStop()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Aborting);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_OnLineLoopTest::OnRetry()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_OnLineLoopTest::OnSkip()
{
	ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_AlarmComplete);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_OnLineLoopTest::OnPause()
{
	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Pause);
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
int  CPressUnitCmd_OnLineLoopTest::OnResume()
{
	//if(GetCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Pause || GetCmdStatus() == CBaseSeqCmd::eSeqCmdStatus_Alarm){
	//	SetCmdStatus(CBaseSeqCmd::eSeqCmdStatus_Recovery);
	//}
	return ERR_NO_ERROR;
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_OnLineLoopTest::State_Setup()
{
	return CBaseSeqCmd::State_Setup();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_OnLineLoopTest::State_Executing()
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
    
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
#if DEF_TESTER_COMM_SPEC_ONE_SOCKET 

	int nDirSiteOnOff[eEachTesterSocket::eMaxTesterForYanmade] = {0, };

	int nSearch = 0; 
	for (nSearch = 0; nSearch < eEachTesterSocket::eMaxTesterForYanmade; nSearch++)
	{
		nDirSiteOnOff[nSearch] = (int)m_bDutUseNotUse[nSearch];
	}
#else
	int nDirSiteOnOff[eMaxTester] = { 1,1 };
#endif



	switch( m_nStep )
	{		
	case 0:			// check interlock
		{
			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d]",m_szCmdName ,m_nStep);
			
			//g_DMCont.m_dmEQP.SB(BDM0_TEST_STATUS_SITE1+m_nUnitIdx, TRUE);    // Set testing status on
            // set flag initial
            FlagReset();

			m_tcTesterTestTime = (double)GetTickCount64();
			m_nLast_Executing_Step = m_nStep = 1001;
			//::GetLocalTime(&m_sysTmTestStart);
		}break;

	case 1001: // 4: Query tester status
		{
			m_bWaitResultErr   = TRUE;
			int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
			double dSpanTime = (double)GetTickCount64() - m_tcTesterTestTime;
			if( dSpanTime > m_tcSotDelayTime )
			{
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [SOT]",m_szCmdName ,m_nStep);
				m_tcTesterTestTime = (double)GetTickCount64();
				int nRetStatus = 0;
				// change action tester
				int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
				if (nTesterOption == eTester_Dual)
					nRetStatus = g_AppleTestIF.SendAction(m_nTesterIndex + 1, ACTION_STATUS, (WPARAM)NULL, (LPARAM)NULL, nDirSiteOnOff, nTesterOption);
				else // each socket mode에서도 같이 사용 한다.
					nRetStatus = g_AppleTestIF.SendAction(m_nUnitIdx + 1, ACTION_STATUS, (WPARAM)NULL, (LPARAM)NULL, nDirSiteOnOff, nTesterOption);


				if( nRetStatus > ERR_NO_ERROR )
				{
					CParamList AlarmData;
					AlarmData.m_fnAddItemFormat("[ Site %d ]", m_nUnitIdx+1);
					AlarmData.m_fnAddItemFormat("Tester is disconnect!! please check tester and Handler..");
					ReportAlarm(ERR_TESTER_DISCONNECTED_DN1+m_nUnitIdx, AlarmData);
					break;
				}
				m_nLast_Executing_Step = m_nStep++;
			}                
		}break;
	case 1002: // Wait Status and information
		{
            CParamList AlarmData;

			double dSpanFullTestTm = (double)GetTickCount64() - m_tcTesterTestTime;
			if( dSpanFullTestTm > m_tcMaxTestTimeout )
			{
				AlarmData.m_fnAddItemFormat("[ Site %d ]", m_nUnitIdx+1);
                AlarmData.m_fnAddItemFormat("Max test time over!! [OverTime-%.2f]", dSpanFullTestTm/1000.0);
                AlarmData.m_fnAddItemFormat("Tester is not change status IDLE...");
				ReportAlarm(ERR_TESTER_MAX_TEST_TIME_OVER_DN1+m_nUnitIdx, AlarmData);
				break;
			}

			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [wait status]", m_szCmdName, m_nStep);
	
			//pTaskPressUnit->DualTesterSiteOnOffCheck(nDirSiteOnOff);
			//pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Get Tester Status]",m_szCmdName ,m_nStep);
			// Receive data check from tester
			int nRet = 0;
			int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
			if (nTesterOption == eTester_Dual)
				nRet = g_AppleTestIF.GetStatus(m_nTesterIndex + 1, nDirSiteOnOff, nTesterOption);
			else if (nTesterOption == eTester_Single)
				nRet = g_AppleTestIF.GetStatus(m_nUnitIdx+1, nDirSiteOnOff, nTesterOption); // -1: Error //  0: Init //	1: Idle //	2: Testing //	3: Error
			else
			{
				nRet = g_AppleTestIF.GetStatus(m_nUnitIdx + 1, nDirSiteOnOff, nTesterOption); // -1: Error //  0: Init //	1: Idle //	2: Testing //	3: ErrornRet = g_AppleTestIF.GetStatus(m_nUnitIdx+1, nDirSiteOnOff); // -1: Error //  0: Init //	1: Idle //	2: Testing //	3: Error
			}

			if( nRet == STATUS_ERROR)
			{
				AlarmData.m_fnAddItemFormat("[ Site %d ]", m_nUnitIdx+1);
                AlarmData.m_fnAddItemFormat("handler receive from tester [tester status error ]");
                AlarmData.m_fnAddItemFormat("Handler send to tester query [ Action status ]");
				ReportAlarm(ERR_RECEIVE_TESTER_DN1+m_nUnitIdx, AlarmData);
				break;
			}
            else if( nRet == RET_ERROR){
				AlarmData.m_fnAddItemFormat("[ Site %d ]", m_nUnitIdx+1);
                AlarmData.m_fnAddItemFormat("Tester is disconnect!! please check tester and Handler..");
                ReportAlarm(ERR_TESTER_DISCONNECTED_DN1+m_nUnitIdx, AlarmData);
                break;
            }
			else if(nRet == STATUS_IDLE){ 
                FlagReset();
				m_tcTesterTestTime = (double)GetTickCount64();
				m_nLast_Executing_Step = m_nStep = 2000; 
			}
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
			double dSpanFullTestTm = (double)GetTickCount64() - m_tcTesterTestTime;

			if( dSpanFullTestTm > m_tcMaxTestTimeout )
			{
				AlarmData.m_fnAddItemFormat("[ Site %d ]", m_nUnitIdx+1);
                AlarmData.m_fnAddItemFormat("Max test time over!! [OverTime-%.2f]", dSpanFullTestTm/1000.0);
                AlarmData.m_fnAddItemFormat("Handler send to query[Start test], but Tester is not the answer.");
				ReportAlarm(ERR_TESTER_MAX_TEST_TIME_OVER_DN1+m_nUnitIdx, AlarmData);
				break;
			}

// #if DEF_TESTER_COMM_SPEC_ONE_SOCKET 
// 			int nDirSiteOnOff[eEachTesterSocket::eMaxTesterForYanmade] = { 1,1,1,1,1,1,1,1 };
// #else
// 			int nDirSiteOnOff[eMaxTester] = { 1,1 };
// #endif

			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Send Information to Tester]",m_szCmdName ,m_nStep);
			int nRet = 0;
			int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
			if (nTesterOption == eTester_Dual)
				nRet = g_AppleTestIF.SendAction(m_nTesterIndex + 1, ACTION_START, (WPARAM)m_stActionParam, (LPARAM)NULL, nDirSiteOnOff, nTesterOption);
			else // each mode 같이 사용 한다.
				nRet = g_AppleTestIF.SendAction(m_nUnitIdx + 1,  ACTION_START, (WPARAM)m_stActionParam, (LPARAM)NULL, nDirSiteOnOff, nTesterOption);

			m_bSotErr   = TRUE;
			if( nRet != ERR_NO_ERROR)
			{
				int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
				int nRetStatus = 0;
				if (nTesterOption == eTester_Dual)
					nRetStatus = g_AppleTestIF.GetStatus(m_nTesterIndex + 1, nDirSiteOnOff);
				else
					nRetStatus = g_AppleTestIF.GetStatus(m_nUnitIdx + 1, nDirSiteOnOff); // -1: Error //  0: Init //	1: Idle //	2: Testing //	3: Error

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
            CParamList AlarmData;
			double dSpanFullTestTm = (double)GetTickCount64() - m_tcTesterTestTime;
			if( dSpanFullTestTm > m_tcMaxTestTimeout )
			{
				AlarmData.m_fnAddItemFormat("[ Station %d ]", m_nUnitIdx+1);
                AlarmData.m_fnAddItemFormat("Max test time over!! [OverTime %.2f]", dSpanFullTestTm/1000.0);
                AlarmData.m_fnAddItemFormat("Handler send to query[Start test], but Tester is not the answer.");
				ReportAlarm(ERR_TESTER_MAX_TEST_TIME_OVER_DN1+m_nUnitIdx, AlarmData);
				break;
			}
	//		int nDirSiteOnOff[eMaxTester] = { 1,1};
			//pTaskPressUnit->DualTesterSiteOnOffCheck(nDirSiteOnOff);
			int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
			int nRet = 0;
			if (nTesterOption == eTester_Dual)
				nRet = g_AppleTestIF.GetStatus(m_nTesterIndex + 1, nDirSiteOnOff, nTesterOption);
			else
				nRet = g_AppleTestIF.GetStatus(m_nUnitIdx + 1, nDirSiteOnOff, nTesterOption); // -1: Error //  0: Init //	1: Idle //	2: Testing //	3: Error

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

			// Receive 받은게 있으면 다음 Step으로 가서 처리한다.
			if( nRet == STATUS_TESTING){
                FlagReset();
				/*m_tcTimeout = */m_tcTesterTestTime = (double)GetTickCount64();
				m_nLast_Executing_Step = m_nStep++;
				pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Wait Test Result]",m_szCmdName ,m_nStep);
			}
		}break;
	case 2002: // Wait Test Result
		{
			double dSpanTime = (double)GetTickCount64() - m_tcTimeout;
            double dSpanTestTime = (double)GetTickCount64() - m_tcTesterTestTime;
            CParamList AlarmData;
            m_bWaitResultErr = TRUE;

			if( dSpanTestTime > m_tcMaxTestTimeout )
			{
				AlarmData.m_fnAddItemFormat("[ Station %d ]", m_nUnitIdx+1);
                AlarmData.m_fnAddItemFormat("Max test time over!! [OverTime-%.2f]", dSpanTime/1000.0);
                AlarmData.m_fnAddItemFormat("Handler send to query[Start test], but Tester is not the answer.");
				//ReportAlarm(ERR_TESTER_MAX_TEST_TIME_OVER_DN1+m_nUnitIdx, AlarmData);
				g_Error.AlarmReport(ERR_TESTER_MAX_TEST_TIME_OVER_DN1 + m_nUnitIdx, AlarmData, &g_TaskSystemCtrl);
				
				ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
				break;
			}
			//int nDirSiteOnOff[eMaxTester] = { 1,1 };
			//pTaskPressUnit->DualTesterSiteOnOffCheck(nDirSiteOnOff);
			int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
			int nRet = 0;
			if (nTesterOption == eTester_Dual)
				nRet = g_AppleTestIF.GetStatus(m_nTesterIndex + 1, nDirSiteOnOff, nTesterOption);
			else
				nRet = g_AppleTestIF.GetStatus(m_nUnitIdx + 1, nDirSiteOnOff, nTesterOption); // -1: Error //  0: Init //	1: Idle //	2: Testing //	3: Error

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
			if( nRet != STATUS_TESTING){
                FlagReset();
                m_tcTesterTestTime = (double)GetTickCount64();
				m_nLast_Executing_Step = m_nStep = 4000;
			}
 		}break;
	case 4000:
		{
			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Change test Status(Test End)]",m_szCmdName ,m_nStep);
			ResultData rData;
			//int nDirSiteOnOff[eMaxTester] = { 1,1 };
			//pTaskPressUnit->DualTesterSiteOnOffCheck(nDirSiteOnOff);
			int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
			BOOL bRet = 0;
			if (nTesterOption == eTester_Dual)
				bRet = g_AppleTestIF.GetResultDataFromTester(m_nTesterIndex + 1, &rData, nDirSiteOnOff, nTesterOption);
			else
				bRet = g_AppleTestIF.GetResultDataFromTester(m_nUnitIdx + 1, &rData, nDirSiteOnOff, nTesterOption);
		
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

			pTaskPressUnit->MakeLog("[%s] [EXEC=%8d] [Data Handling]",m_szCmdName ,m_nStep);
			// change test status

			m_nRealTestCnt++;
			if (m_nRealTestCnt < m_nLoopCount)
			{
				m_nLast_Executing_Step = m_nStep = 0;
				break;
			}
			ReportCmdEnd(CBaseSeqCmd::eSeqCmdStatus_NormalComplete);
		}break;
	}
	return CBaseSeqCmd::State_Executing();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_OnLineLoopTest::State_Pause()
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
BOOL CPressUnitCmd_OnLineLoopTest::State_Recovery()
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
BOOL CPressUnitCmd_OnLineLoopTest::State_NormalComplete()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	return CBaseSeqCmd::State_NormalComplete();
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_OnLineLoopTest::State_Alarm()
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
BOOL CPressUnitCmd_OnLineLoopTest::State_AlarmComplete()
{
    CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;

	//	// some time during testing continue tester
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);

	if (nTesterOption == eTester_Dual)
		g_AppleTestIF.SendAction(m_nTesterIndex + 1, ACTION_STOP, (WPARAM)NULL, (LPARAM)NULL, m_bDutUseNotUse, nTesterOption);
	else
		g_AppleTestIF.SendAction(m_nUnitIdx+1, ACTION_STOP, (WPARAM)NULL, (LPARAM)NULL, m_bDutUseNotUse, nTesterOption);

	return CBaseSeqCmd::State_AlarmComplete(); 
}

//==============================================================================
//
//==============================================================================
BOOL CPressUnitCmd_OnLineLoopTest::State_Aborting()
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
BOOL CPressUnitCmd_OnLineLoopTest::State_AbortComplete()
{
	CTaskPressUnit* pTaskPressUnit = (CTaskPressUnit*)m_pReceive;
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	if (nTesterOption == eTester_Dual)
		g_AppleTestIF.SendAction(m_nTesterIndex + 1, ACTION_STOP, (WPARAM)NULL, (LPARAM)NULL, m_bDutUseNotUse, nTesterOption);
	else
		g_AppleTestIF.SendAction(m_nUnitIdx + 1, ACTION_STOP, (WPARAM)NULL, (LPARAM)NULL, m_bDutUseNotUse, nTesterOption);

	return CBaseSeqCmd::State_AbortComplete(); 
}

void CPressUnitCmd_OnLineLoopTest::FlagReset()
{
    m_bSotErr          = FALSE;
    m_bWaitResultErr   = FALSE;
    m_bNotRevData      = FALSE;
    m_b2dMisMatch      = FALSE;
	m_bTesterErr       = FALSE;
}