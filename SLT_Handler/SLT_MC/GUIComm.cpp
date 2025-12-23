#include "StdAfx.h"
#include "GUIComm.h"
#include "ConfigData.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CGUIComm::CGUIComm(void)
{
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CGUIComm::~CGUIComm(void)
{
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
ST_MESSAGE_MAP CGUIComm::MessageMap[] ={
	{2,3,  CGUIComm::OnReceiveS2F3 },
	{2,4,  CGUIComm::OnReceiveS2F4 },
	{2,41,  CGUIComm::OnReceiveS2F41 },
	{2,42,  CGUIComm::OnReceiveS2F42 },
	{5, 1,  CGUIComm::OnReceiveS5F1  },
	{5, 2,  CGUIComm::OnReceiveS5F2  },
	{6,11,  CGUIComm::OnReceiveS6F11 },
	{6,12,  CGUIComm::OnReceiveS6F12 },
	{107,1, CGUIComm::OnReceiveS107F1 },
	{107,2, CGUIComm::OnReceiveS107F2 },
	{107,3, CGUIComm::OnReceiveS107F3 },
	{107,4, CGUIComm::OnReceiveS107F4 },
	{107,5, CGUIComm::OnReceiveS107F5 },
	{107,6, CGUIComm::OnReceiveS107F6 },
	{-1,-1, NULL},
};


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CGUIComm::Initialize(CWnd* pParentWnd, char* szDriverFileName)
{
	int nSocketType = 0;
	char szIp[128] = {0,};
	int nPort = 0;

	char szDefault[128];
	char szBuff[512] = {0, };
	char szLogFileName[ 512 ] = { 0, };

	CConfigData ConfigData(szDriverFileName);
	ConfigData.GetString("MC TO GUI COMM CONFIG", "IP", "127.0.0.1", szIp);
	nPort = ConfigData.GetInt("MC TO GUI COMM CONFIG", "Port", "5000");
	nSocketType = ConfigData.GetInt("MC TO GUI COMM CONFIG", "Socket Type", "1");

	memset(szDefault, 0x00, sizeof(szDefault));
	sprintf_s(szDefault, sizeof(szDefault), "IPC_MC_Comm.log");
	ConfigData.GetString("MC TO GUI COMM CONFIG", "IPC MC Comm Log File Name", szDefault, szBuff);
	sprintf_s(szLogFileName, sizeof(szLogFileName), "%s%s", SZ_LOG_FILE_PATH, szBuff);
	m_Logger.SetFileName(szLogFileName);
	sprintf_s(szLogFileName, sizeof(szLogFileName), "%sGUI%s", SZ_LOG_FILE_PATH, szBuff);

	int nRet = CIPCSock::Initialize(pParentWnd, nSocketType, szIp, nPort, CGUIComm::MessageMap, szLogFileName);
	SendLogMessageToView("IPC GUI Comm Initialized. [ERR=%d]", nRet);
	if( nRet != 0 ){
		AfxMessageBox("can not init gui comm socket!!!");
		return -1;
	}
	nRet = SocketConnect();
	SendLogMessageToView("IPC GUI Comm Connect. [%s, IP:%s, PORT:%d] [ERR=%d]", (nSocketType==eSocketTypeClient)?"Client":"Server", szIp, nPort, nRet);
	if( nRet != 0 ){
		AfxMessageBox("can not connect gui comm socket!!!");
		return -1;
	}

	return nRet;
}

void CGUIComm::Finalize()
{
	SendLogMessageToView("IPC GUI Comm Finalized.");
	CIPCSock::Finalize();
}


LRESULT CGUIComm::OnTWIPCEvent(WPARAM wParam, LPARAM lParam)
{
	CIPCSock::OnTWIPCEvent(wParam, lParam);

	int nEventID = wParam;
	int nObserverCount = GetObserverCount();
	for(int i=0; i<nObserverCount; i++){
		CWnd* pWnd = GetObserverByIndex(i);
		pWnd->SendMessage(UM_GUI_COMM_SOCK_EVENT, (WPARAM)NULL, (LPARAM)nEventID);
	}

	switch ( nEventID )
	{
	case IPC_EVT_CONNECTED:
		{
			SendLogMessageToView("Connected.");
		}break;
	case IPC_EVT_DISCONNECTED:
		{
			SendLogMessageToView("Disconnected.");
		}break;
	case IPC_EVT_RCV_EMPTY:
		{
			SendLogMessageToView("empty message received.");
		}break;
	case IPC_EVT_RCV_ERROR:
		{
			SendLogMessageToView("error message received.");
		}break;
	case IPC_EVT_TM_OVER_MES:
		{
			SendLogMessageToView("timeout message received.");
		}break;
	}

	return 0L;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CGUIComm::OnReceiveS2F3(CPacket* pPacket)
{
	g_GuiComm.SendLogMessageToView("[rcv] S%dF%d. MSGID : %d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	CPacket* pReplyPacket = new CPacket(2,4);
	pReplyPacket->pHead->stData.nMsgId = pPacket->pHead->stData.nMsgId;
	CPacketBody_S2F4* pReplyBody = (CPacketBody_S2F4*)pReplyPacket->pBody;
	g_GuiComm.Send( pReplyPacket, FALSE);
	g_GuiComm.SendLogMessageToView("[snd] S%dF%d", pReplyPacket->pHead->stData.nS, pReplyPacket->pHead->stData.nF);
	delete pReplyPacket;
	pReplyPacket = NULL;

	return 0;
}

int CGUIComm::OnReceiveS2F4(CPacket* pPacket)
{
	g_GuiComm.SendLogMessageToView("[rcv] S%dF%d. MSGID : %d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	return 0;
}

int CGUIComm::OnReceiveS2F41(CPacket* pPacket)
{
	g_GuiComm.SendLogMessageToView("[rcv] S%dF%d. MSGID : %d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

    
    int nAck = acknowledge;
    // Receive data & Excute cmd
    CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
    int nCmd = pBody->stData.nCmd;
    // OneCycle & Stop Cmd
    switch(nCmd)
    {
    case GUI_RCMD_REQ_PAUSE:
        {
            int nParamData = atoi(pBody->stData.szParam[0]);
            switch(nParamData)
            {
            case eAutoLotCmd_Pause:
                {
                    g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_Pause, NULL, eEqpCtrlMode_Auto);
                }break;
            }
        }break;
    case GUI_RCMD_REQ_SHOW_DLG:
        {
			if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
			{
				int nParamData = atoi(pBody->stData.szParam[0]);
				CMainFrame* pMainFrm= (CMainFrame*)AfxGetMainWnd();			
				pMainFrm->PostMessage(UM_GUI_COMM_DLG_EVENT, (WPARAM)nParamData, NULL);
			}
        }break;     
	case GUI_RCMD_REQ_YIELD_MONITOR_CLEAR:
		{
			g_TaskSystemCtrl.YieldContiFailClear();
			g_TaskSystemCtrl.YieldSampleClear();
			g_TaskSystemCtrl.Yield1stFailClear();
			//g_TaskSystemCtrl.YieldVarianceClear();
		}break;
	case GUI_RCMD_REQ_BIN_CATEGORY_REFRESH:
		{
			ZeroMemory(&g_BinCategory, sizeof(ST_CATE_INFO));
			char szData[MAX_PATH] = {0,};
			g_DMCont.m_dmHandlerOpt.GS(SDM11_CategoryName, szData, sizeof(szData));
			g_CokMgr.LoadCategoryInfo(szData, g_BinCategory);
			//HW Bin 설정 되어있지 않는 Setplate Tray Load 금지
			g_TaskSystemCtrl.SetUseUnloadSetplate(g_BinCategory);
			g_DMCont.m_dmEQP.SB( BDM0_CATEGORY_CHANGE, TRUE );
		}break;
	case GUI_RCMD_REQ_PACKAGE_CONVERSION:
		{
			ZeroMemory(&g_BinCategory, sizeof(ST_CATE_INFO));
			char szData[MAX_PATH] = {0,};
			g_DMCont.m_dmHandlerOpt.GS(SDM11_CategoryName, szData, sizeof(szData));
			g_CokMgr.LoadCategoryInfo(szData, g_BinCategory);
			//HW Bin 설정 되어있지 않는 Setplate Tray Load 금지
			g_TaskSystemCtrl.SetUseUnloadSetplate(g_BinCategory);

			char szDrvFile[128]={0,};
			char szCokName[128]={0,};
			g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));

			char szCurDir[512]={0,};
			GetCurrentDirectory(sizeof(szCurDir), szCurDir);
			sprintf_s(szDrvFile, sizeof(szDrvFile), "%s\\sys_data\\%s", szCurDir, SZ_DRIVER_FILE_NAME);

			g_DMCont.m_Teach.Initialize(szDrvFile, szCokName);
			g_DMCont.m_Teach.Load();

			g_pBarcode->Cmd_SetCok();
			//g_pTestSiteInspUp->Cmd_SetCok();
			//g_pTestSiteInspDn->Cmd_SetCok();
			g_pVisionTestHand_1->Cmd_SetCok();
			g_pVisionTestHand_2->Cmd_SetCok();

			g_TaskTestPP[eTestPP_1].GetUserOffsetData();
			g_TaskTestPP[eTestPP_2].GetUserOffsetData();
		}break;
	case GUI_RCMD_REQ_USER_OFFSET_REFESH:
		{
			g_TaskTestPP[eTestPP_1].GetUserOffsetData();
			g_TaskTestPP[eTestPP_2].GetUserOffsetData();
 
// 			g_TaskTrayPP[m_eTrayIdx].LoadPickerOffSet();
		}break;
	case GUI_RCMD_REQ_SITE_VISION_SET_CHNG:
		{
			BOOL bUseNoUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_VisionUse);
			//g_pTestSiteInspUp->Cmd_VisionUseNoUse((int)bUseNoUse);
			//g_pTestSiteInspDn->Cmd_VisionUseNoUse((int)bUseNoUse);

			g_pVisionTestHand_1->Cmd_VisionUseNoUse((int)bUseNoUse);
			g_pVisionTestHand_2->Cmd_VisionUseNoUse((int)bUseNoUse);
		}break;
	case GUI_RCMD_REQ_2D_VISION_SET_CHNG:
		{
			BOOL bUseNoUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use);
			g_pBarcode->Cmd_VisionUseNoUse((int)bUseNoUse);
		}break;
	case GUI_RCMD_REQ_VIBRATOR_CHANGE:
		{
			
		}break;
	case GUI_RCMD_REQ_TEACHING_RELOAD:
		{// Contact Control 화면에서 Press Teaching 값 변경시 ReLoad해줘야 한다.
			g_DMCont.m_Teach.PressTeachingDataLoad();
		}break;
	case GUI_RCMD_REQ_MAKE_SUMMARY:
		{
			g_TaskSystemCtrl.MakeSummary();
		}break;
	case GUI_RCMD_REQ_YIELD_MONITOR_CLEAR_EACH:
		{
			int nParamData = atoi(pBody->stData.szParam[0]);
			//int nInlinePara = atoi(pBody->stData.szParam[1]);
			int nYieldMode = atoi(pBody->stData.szParam[1]);
			int nSocketNo = atoi(pBody->stData.szParam[2]);
			int nX = nSocketNo % STATION_MAX_X_SITE;
			int nY = nSocketNo / STATION_MAX_X_SITE;
			int nIdx = nParamData >= 8 ? (nParamData - 6 - (eTestPP_1*2)) : (nParamData - (eTestPP_1 * 2));
			
// 			for (int nX = 0; nX < STATION_MAX_X_SITE; nX++) {
// 				for (int nY = 0; nY < STATION_MAX_Y_SITE; nY++) {
					//g_TaskSystemCtrl.YieldVarianceClear(nIdx, nX, nY);
					if (nYieldMode == eYield)
					{
						g_TaskSystemCtrl.YieldSampleClear(nIdx, nX, nY);
						g_TaskSystemCtrl.YieldContiFailClear(nIdx, nX, nY);
					}
					else
						g_TaskSystemCtrl.Yield1stFailClear(nIdx, nX, nY);
// 				}
// 			}
			
		}break;
	case GUI_RCMD_REQ_DEGAULT_BIN_CATEGORY_REFRESH:
		{
			char szData[MAX_PATH] = { 0, };

			g_DMCont.m_dmHandlerOpt.GS(SDM11_SLT_ProgramName, szData, sizeof(szData));
			g_DefaultBinCategory.clear();
			// ZeroMemory(&g_DefaultBinCategory, sizeof(ST_Set_DefaultBin)); <--- cpp check error : Using 'memset' on struct that contains a 'std::vector'.
			g_CokMgr.LoadSltPgmDefaultBin(szData, g_DefaultBinCategory);
		}break;
// 	case GUI_RCMD_REQ_TESTER_ONLINE_MODE_CHANGE:
// 		{
// #if DEF_TESTER_COMM_SPEC_ONE_SOCKET == 0
// 					int nTesterOption = g_DMCont.m_dmEQP.GN(NDM11_Handler_Tester_Option);
// 					if (nTesterOption == eTester_EachSocket)
// 					{
// 						break;
// 					}
// 		
// 					int nParamData = atoi(pBody->stData.szParam[0]);
// 		
// 					WPARAM wParam = 0;
// 					LPARAM lParam = 0;
// 					ST_TESTER_ACTION_PARAM stActionParam[STATION_MAX_PARA];
// 		
// 					BOOL bAutoGrrUse = FALSE;
// 					int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
// 					if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
// 						bAutoGrrUse = TRUE;
// 		
// 					g_TaskSystemCtrl.m_csHeartBeat.Lock();
// 		
// 					CString strStationNo = _T(""), strTesterNo = _T("");
// 		
// 					if (nTesterOption == eTester_Single)
// 					{
// 						for (int i = 0; i < eMaxPressUnitCount; i++) {
// 							int nStationID = i + 1;
// 							BOOL bConnect = g_AppleTestIF.IsConnected(nStationID, nTesterOption);
// 							if (bConnect) {
// 								int nProjectOption = g_DMCont.m_dmEQP.GN(NDM11_Handler_Project_Name);
// 							
// 								g_DMCont.m_dmEQP.SB(BDM0_TESTER_CONNECTION_1 + i, FALSE);
// 								
// 								wParam = (WPARAM)&stActionParam;
// 								lParam = nParamData;
// 								g_AppleTestIF.SendAction(nStationID, ACTION_SET_TEST_MODE, wParam, lParam, NULL, nTesterOption);
// 							}
// 							else {
// 								int nRetSocket = g_TaskPressUnit[i].GetSocketOnCnt(bAutoGrrUse);
// 								if (nRetSocket > 0)
// 								{
// 									CString strNo = _T("");
// 									strNo.Format("(%d)", i + 1);
// 									strStationNo += strNo;
// 								}
// 							}
// 						}
// 					}
// 					else if (nTesterOption == eTester_Dual)
// 					{
// 						for (int i = 0; i < MAX_TESTER_DUAL; i++) {
// 							int nStationID = i+1;
// 							BOOL bConnect = g_AppleTestIF.IsConnected(nStationID);
// 							if (bConnect) {
// 								int nProjectOption = g_DMCont.m_dmEQP.GN(NDM11_Handler_Project_Name);
// 		
// 								if (nProjectOption == DEF_PROJECT_MAUI) {
// 									g_DMCont.m_dmEQP.SB(BDM0_TESTER_CONNECTION_1 + (i/2), FALSE);
// 								}
// 								wParam = (WPARAM)&stActionParam;
// 								lParam = nParamData;
// 								g_AppleTestIF.SendAction(nStationID, ACTION_SET_TEST_MODE, wParam, lParam, NULL);
// 							}
// 							else {
// 								int nRetSocket = g_TaskPressUnit[i/2].GetDirDualSocketOnCnt(bAutoGrrUse, i % 2);
// 								if (nRetSocket > 0)
// 								{
// 									CString strTestNo = _T("");
// 									strTestNo.Format("(%d-%d)", (i / 2)+1, (i % 2) +1 );
// 									strTesterNo += strTestNo;
// 								}
// 							}
// 						}
// 					}
// 		
// 					g_TaskSystemCtrl.m_csHeartBeat.Unlock();
// 		
// 					if (strStationNo.IsEmpty() == FALSE || strTesterNo.IsEmpty() == FALSE)
// 					{
// 						CParamList AlarmData;
// 						if (nTesterOption == eTester_Single)
// 							AlarmData.m_fnAddItemFormat(_T(" Tester is not connect! STATION NO=%s "), strStationNo);
// 						else
// 							AlarmData.m_fnAddItemFormat(_T(" Tester is not connect! TESTER NO=%s "), strTesterNo);
// 		
// 						g_Error.AlarmReport(ERR_ON_STATION_NOT_CONNECT_TESTER, AlarmData, &g_TaskSystemCtrl);
// 					}	
// 		#endif
// 				}break;
	case GUI_RCMD_REQ_MODULE_SITE_ON_IS_POSSIBLE:
		{
			for (int nModuleIdx = 0; nModuleIdx < MAX_TEST_PP_CNT; nModuleIdx++) {
				BOOL bIsPossible = TRUE;
				//Running 중에는 Module의 Unit 상태에 따라 Site On을 시킬수 있다.
				if (g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY) != TRUE) {
					if (g_TaskTestPP[nModuleIdx].IsInitNeed() == TRUE) {
						bIsPossible = FALSE;
					}	
					else {
						for (int i = 0; i < eMaxATS_Area; i++) {
							int nPressIdx = g_TaskAutoTestPp[nModuleIdx].ConvertPressIdx(i);
							if (g_TaskPressUnit[nPressIdx].IsInitNeed() == TRUE) {
								bIsPossible = FALSE;
							}
						}
					}
					for (int i = 0; i < MAX_LOAD_TABLE/2; i++) {

						int nTableIdx = g_TaskAutoTestPp[nModuleIdx].ConvertLoadTableIdx(i);
						if (g_TaskLoadTbl[nTableIdx].IsInitNeed() == TRUE) {
							bIsPossible = FALSE;
						}
					}
				}
				g_DMCont.m_dmEQP.SB(BDM0_MODULE1_SITE_ON_IS_POSSIBLE + nModuleIdx, bIsPossible);
			}
		}break;
	case GUI_RCMD_REQ_TRAYPP_SAFETY_POS_MOVE:
		{
			//g_Motor[eAXIS_TRAY_FEEDER_Y]->Move(0, TEACH_MOVE_SPD_LOW, TEACH_MOVE_ACC_LOW, TEACH_MOVE_DEC_LOW);
		}break;
	case GUI_RCMD_REQ_MULTI_ORIGIN:
		{
			int nParamData = pBody->stData.nParamCount;
			CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
			pMainFrm->PostMessage(UM_GUI_MULTI_ORIGIN, (WPARAM)nParamData, NULL);	
		}break;
	case GUI_RCMD_REQ_TESTER_BATCH_FILE_EXCUTE:
		{
			char szTesterBatchFilePath[MAX_PATH] = { 0, };
			g_DMCont.m_dmHandlerOpt.GS(SDM11_Tester_BatchFile_Path, szTesterBatchFilePath, sizeof(szTesterBatchFilePath));

			char szBatPath[MAX_PATH] = { 0, };

			sprintf_s(szBatPath, sizeof(szBatPath), "%s", szTesterBatchFilePath);
			int nErr = (int)ShellExecute(NULL, "open", szBatPath, NULL, NULL, SW_SHOWNORMAL);
			if (nErr <= 32) { // ShellExecute return 값 32이하 면 Error
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Failed to execute Batch File. = %s", szTesterBatchFilePath);
				g_Error.AlarmReport(ERR_TESTER_BATCH_FILE_NOT_EXIST, AlarmData, NULL);
			}

		}break;
	case GUI_RCMD_REQ_LOAD_SUPPLY:
	{
		BOOL bAutoGrrUse = g_TaskSystemCtrl.GetAutoGrrUse();
		BOOL bAutoLoopUse = g_TaskSystemCtrl.GetAutoLoopUse();


		if (bAutoGrrUse == TRUE || bAutoLoopUse == TRUE)
		{
			break;
		}
		bool bFinish = g_TaskSystemCtrl.IsRunningFinish();
		if(bFinish == true)
			break;

		g_DMCont.m_dmEQP.SB(BDM0_SYS_LOAD_SUPPLY, TRUE);
		g_TaskAllAutoStacker.SetLoadTraySupplyEnd(false);

	}break;
	
    }
    
    // Reply Data send
    CPacket* pReplyPacket = new CPacket(2,42);
    pReplyPacket->pHead->stData.nMsgId = pPacket->pHead->stData.nMsgId;
    CPacketBody_S2F42* pReplyBody = (CPacketBody_S2F42*)pReplyPacket->pBody;
    pReplyBody->stData.nRCMDACK = nAck;
	g_GuiComm.Send( pReplyPacket, FALSE);
	g_GuiComm.SendLogMessageToView("[snd] S%dF%d", pReplyPacket->pHead->stData.nS, pReplyPacket->pHead->stData.nF);
	delete pReplyPacket;
	pReplyPacket = NULL;


    if(nCmd == GUI_RCMD_REQ_FINALIZE_PGM)
    {
		g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_Pause, NULL, eEqpCtrlMode_Auto);
        CMainFrame* pMainFrm= (CMainFrame*)AfxGetMainWnd();
        pMainFrm->SendMessage(UM_GUI_COMM_FINALIZE_PGM, (WPARAM)NULL, NULL);
    }

	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CGUIComm::OnReceiveS2F42(CPacket* pPacket)
{
	g_GuiComm.SendLogMessageToView("[rcv] S%dF%d. MSGID : %d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CGUIComm::OnReceiveS5F1 (CPacket* pPacket)
{
	g_GuiComm.SendLogMessageToView("[rcv] S%dF%d. MSGID : %d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	CPacket* pReplyPacket = new CPacket(5,2);
	pReplyPacket->pHead->stData.nMsgId = pPacket->pHead->stData.nMsgId;
	CPacketBody_S5F2* pReplyBody = (CPacketBody_S5F2*)pReplyPacket->pBody;
	g_GuiComm.Send( pReplyPacket, FALSE);
	g_GuiComm.SendLogMessageToView("[snd] S%dF%d", pReplyPacket->pHead->stData.nS, pReplyPacket->pHead->stData.nF);
	delete pReplyPacket;
	pReplyPacket = NULL;

	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CGUIComm::OnReceiveS5F2 (CPacket* pPacket)
{
	g_GuiComm.SendLogMessageToView("[rcv] S%dF%d. MSGID : %d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CGUIComm::OnReceiveS6F11(CPacket* pPacket)
{
	g_GuiComm.SendLogMessageToView("[rcv] S%dF%d. MSGID : %d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	CPacket* pReplyPacket = new CPacket(6,12);
	pReplyPacket->pHead->stData.nMsgId = pPacket->pHead->stData.nMsgId;
	CPacketBody_S6F12* pReplyBody = (CPacketBody_S6F12*)pReplyPacket->pBody;
	g_GuiComm.Send( pReplyPacket, FALSE);
	g_GuiComm.SendLogMessageToView("[snd] S%dF%d", pReplyPacket->pHead->stData.nS, pReplyPacket->pHead->stData.nF);
	delete pReplyPacket;
	pReplyPacket = NULL;

	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CGUIComm::OnReceiveS6F12(CPacket* pPacket)
{
	g_GuiComm.SendLogMessageToView("[rcv] S%dF%d. MSGID : %d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CGUIComm::OnReceiveS107F1(CPacket* pPacket)
{
	g_GuiComm.SendLogMessageToView("[rcv] S%dF%d. MSGID : %d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	CPacket* pReplyPacket = new CPacket(107,2);
	pReplyPacket->pHead->stData.nMsgId = pPacket->pHead->stData.nMsgId;
	CPacketBody_S107F2* pReplyBody = (CPacketBody_S107F2*)pReplyPacket->pBody;
	g_GuiComm.Send( pReplyPacket, FALSE);
	g_GuiComm.SendLogMessageToView("[snd] S%dF%d", pReplyPacket->pHead->stData.nS, pReplyPacket->pHead->stData.nF);
	delete pReplyPacket;
	pReplyPacket = NULL;

	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CGUIComm::OnReceiveS107F2(CPacket* pPacket)
{
	g_GuiComm.SendLogMessageToView("[rcv] S%dF%d. MSGID : %d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CGUIComm::OnReceiveS107F3(CPacket* pPacket)
{
	g_GuiComm.SendLogMessageToView("[rcv] S%dF%d. MSGID : %d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	CPacket* pReplyPacket = new CPacket(107,4);
	pReplyPacket->pHead->stData.nMsgId = pPacket->pHead->stData.nMsgId;
	CPacketBody_S107F4* pReplyBody = (CPacketBody_S107F4*)pReplyPacket->pBody;
	g_GuiComm.Send( pReplyPacket, FALSE);
	g_GuiComm.SendLogMessageToView("[snd] S%dF%d", pReplyPacket->pHead->stData.nS, pReplyPacket->pHead->stData.nF);
	delete pReplyPacket;
	pReplyPacket = NULL;

	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CGUIComm::OnReceiveS107F4(CPacket* pPacket)
{
	g_GuiComm.SendLogMessageToView("[rcv] S%dF%d. MSGID : %d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CGUIComm::OnReceiveS107F5(CPacket* pPacket)
{
	g_GuiComm.SendLogMessageToView("[rcv] S%dF%d. MSGID : %d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	CPacket* pReplyPacket = new CPacket(107,6);
	pReplyPacket->pHead->stData.nMsgId = pPacket->pHead->stData.nMsgId;
	CPacketBody_S107F6* pReplyBody = (CPacketBody_S107F6*)pReplyPacket->pBody;
	g_GuiComm.Send( pReplyPacket, FALSE);
	g_GuiComm.SendLogMessageToView("[snd] S%dF%d", pReplyPacket->pHead->stData.nS, pReplyPacket->pHead->stData.nF);
	delete pReplyPacket;
	pReplyPacket = NULL;

	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CGUIComm::OnReceiveS107F6(CPacket* pPacket)
{
	g_GuiComm.SendLogMessageToView("[rcv] S%dF%d. MSGID : %d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	return 0;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-11-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CGUIComm::AttachObserver(CWnd* pObserver)
{
	m_csObserver.Lock();
	if( CheckDuplicate(pObserver) == FALSE ){ // 중복이 아니면 리스트에 추가 함.
		m_listObserver.AddTail(pObserver);
	}
	m_csObserver.Unlock();
	return 0;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-11-04]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CGUIComm::DetachObserver(CWnd* pObserver)
{
	m_csObserver.Lock();
	POSITION pos = m_listObserver.GetHeadPosition();
	POSITION pos_pre = NULL;
	int nCount = m_listObserver.GetCount();
	for(int i=0; i<nCount; i++){
		pos_pre = pos;
		CWnd* pItem = m_listObserver.GetNext(pos);
		if( pItem != NULL ){
			if( pItem == pObserver ){
				m_listObserver.RemoveAt(pos_pre);
				break;
			}
		}
	}
	m_csObserver.Unlock();
	return 0;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-11-04]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CGUIComm::ClearObserver()
{
	m_csObserver.Lock();
	POSITION pos = m_listObserver.GetHeadPosition();
	int nCount = m_listObserver.GetCount();
	for(int i=0; i<nCount; i++){
		CWnd* pItem = m_listObserver.GetNext(pos);
		if( pItem != NULL ){
			delete pItem;
			pItem = NULL;
		}
	}
	m_listObserver.RemoveAll();
	m_csObserver.Unlock();
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-11-04]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CGUIComm::GetObserverCount()
{
	m_csObserver.Lock();
	int nCount = m_listObserver.GetCount();
	m_csObserver.Unlock();
	return nCount;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-11-04]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CWnd* CGUIComm::GetObserverByIndex(int nIndex)
{
	m_csObserver.Lock();
	POSITION pos = m_listObserver.FindIndex(nIndex);
	CWnd* pItem = (CWnd*)m_listObserver.GetAt(pos);
	if( pItem != NULL ){
		m_csObserver.Unlock();
		return pItem;
	}
	m_csObserver.Unlock();
	return NULL;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-11-04]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
BOOL CGUIComm::CheckDuplicate(CWnd* pItem)
{
	m_csObserver.Lock();
	POSITION pos = m_listObserver.GetHeadPosition();
	int nCount = m_listObserver.GetCount();
	for(int i=0; i<nCount; i++){
		CWnd* pItem = (CWnd*)m_listObserver.GetNext(pos);
		if( pItem != NULL ){
//			if( pItem == pItem){
				m_csObserver.Unlock();
				return TRUE;
//			}
		}
	}
	m_csObserver.Unlock();
	return FALSE;
}

void CGUIComm::SendLogMessageToView(const char* fmt, ...)
{
	char szEventMessage[1024]={0,};
	va_list va;
	va_start(va, fmt);
	vsprintf_s(szEventMessage, sizeof(szEventMessage), fmt, va);
	va_end(va);

	m_Logger.LOG(szEventMessage);
	int nObserverCount = GetObserverCount();
	for(int i=0; i<nObserverCount; i++){
		CWnd* pWnd = GetObserverByIndex(i);
		pWnd->SendMessage(UM_GUI_COMM_LOG_MSG, (WPARAM)NULL, (LPARAM)szEventMessage);
	}
}


int CGUIComm::Send_EventReport(int nEvtId)
{
	int nErr = 0;

	if( !IsConnected() ){
		return -1;
	}

	CPacket* pPacket = new CPacket(6,11);
	CPacketBody_S6F11* pBody = (CPacketBody_S6F11*)pPacket->pBody;
	pBody->stData.nCEID = nEvtId;
	nErr  = CIPCSock::Send(pPacket, FALSE);
	SendLogMessageToView("[send] S%dF%d, [ERR:%d]", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, nErr);

	delete pPacket;
	pPacket = NULL;

	return nErr;
}

int CGUIComm::Send_AlarmReport(int nAlid, int nAlst)
{
	int nErr = 0;
	
	if( !IsConnected() ){
		return -1;
	}

	CPacket* pPacket = new CPacket(5, 1);
	CPacketBody_S5F1* pBody = (CPacketBody_S5F1*)pPacket->pBody;
	pBody->stData.nALID = nAlid;
	pBody->stData.nALST = nAlst;
	nErr = CIPCSock::Send(pPacket, FALSE);
	SendLogMessageToView("[send] S%dF%d, [ERR:%d]", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, nErr);

	delete pPacket;
	pPacket = NULL;

	return nErr;
}

/*=============================================================================
  DESCRIPT : GUI에 메시지를 보내 AutoGRR Cancel 버튼을 활성/비활성하게 하는 함수
  KEY WORD : 
  ARGUMENT : bEnable = TRUE (활성), FALSE (비활성)
  RETURN   : 
 ==============================================================================*/
int CGUIComm::EnableAutoGrrCancel(BOOL bEnable /*= TRUE*/)
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = GUI_RCMD_REQ_AUTOGRR_CANCEL;
	pBody->stData.nParamCount = 1;
	sprintf_s(pBody->stData.szParam[0], "%d", bEnable);

	int nErr = CIPCSock::Send(pPacket, FALSE);
	SendLogMessageToView("[send] S%dF%d, [ERR:%d]", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, nErr);

	delete pPacket;
	pPacket = NULL;

	return nErr;
}

/*=============================================================================
  DESCRIPT : Tray PP Skip to Assign Button 선택 시 Data Assign Dialog Show  
  KEY WORD : 
  ARGUMENT : 
  RETURN   : 
 ==============================================================================*/
int CGUIComm::SendDataAssignShow(int nSetplateNum)
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = GUI_RCMD_REQ_DATA_ASSIGN_SHOW;
	pBody->stData.nParamCount = 1;
	sprintf_s(pBody->stData.szParam[0] , "%d", nSetplateNum);

	int nErr = CIPCSock::Send(pPacket, FALSE);
	SendLogMessageToView("[send] S%dF%d, [ERR:%d]", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, nErr);

	delete pPacket;
	pPacket = NULL;

	return nErr;
}


int CGUIComm::SendTestPpOffSetShow()
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = GUI_RCMD_REQ_TESTPP_OFFSET_SHOW;
	pBody->stData.nParamCount = 0;

	int nErr = CIPCSock::Send(pPacket, FALSE);
	SendLogMessageToView("[send] S%dF%d, [ERR:%d]", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, nErr);

	delete pPacket;
	pPacket = NULL;

	return nErr;
}

int CGUIComm::SendLotClearEnd()
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = GUI_RCMD_REQ_MAKE_SUMMARY_END;
	pBody->stData.nParamCount = 0;

	int nErr = CIPCSock::Send(pPacket, FALSE);
	SendLogMessageToView("[send] S%dF%d, [ERR:%d]", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, nErr);

	delete pPacket;
	pPacket = NULL;

	return nErr;
}

int CGUIComm::SendLotinfoShow()
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = GUI_RCMD_REQ_LOTINFO_SHOW;
	pBody->stData.nParamCount = 0;

	int nErr = CIPCSock::Send(pPacket, FALSE);
	SendLogMessageToView("[send] S%dF%d, [ERR:%d]", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, nErr);

	delete pPacket;
	pPacket = NULL;

	return nErr;
}

int CGUIComm::SendQAQTYShow()
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = GUI_RCMD_REQ_QA_QTY_SHOW;
	pBody->stData.nParamCount = 0;

	int nErr = CIPCSock::Send(pPacket, FALSE);
	SendLogMessageToView("[send] S%dF%d, [ERR:%d]", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, nErr);

	delete pPacket;
	pPacket = NULL;

	return nErr;
}

int CGUIComm::SendDoorStatusView(BOOL bCoverLock)
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = GUI_RCMD_REQ_DOOR_STATUS_VIEW;
	pBody->stData.nParamCount = 1;
	sprintf_s(pBody->stData.szParam[0], "%d", bCoverLock);

	int nErr = CIPCSock::Send(pPacket, FALSE);
	SendLogMessageToView("[send] S%dF%d, [ERR:%d]", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, nErr);

	delete pPacket;
	pPacket = NULL;

	return nErr;
}