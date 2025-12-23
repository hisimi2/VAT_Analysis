#include "StdAfx.h"
#include "MCComm.h"

#include "MainFrm.h"
#include "SLTView.h"


ST_MESSAGE_MAP CMCComm::MessageMap[] ={
	{2,3,  CMCComm::OnReceiveS2F3 },
	{2,4,  CMCComm::OnReceiveS2F4 },
	{2,41,  CMCComm::OnReceiveS2F41 },
	{2,42,  CMCComm::OnReceiveS2F42 },
	{5, 1,  CMCComm::OnReceiveS5F1  },
	{5, 2,  CMCComm::OnReceiveS5F2  },
	{6,11,  CMCComm::OnReceiveS6F11 },
	{6,12,  CMCComm::OnReceiveS6F12 },
	{107,1, CMCComm::OnReceiveS107F1 },
	{107,2, CMCComm::OnReceiveS107F2 },
	{107,3, CMCComm::OnReceiveS107F3 },
	{107,4, CMCComm::OnReceiveS107F4 },
	{107,5, CMCComm::OnReceiveS107F5 },
	{107,6, CMCComm::OnReceiveS107F6 },
	{-1,-1, NULL},
};

CMCComm::CMCComm(void)
{
}


CMCComm::~CMCComm(void)
{
}

int CMCComm::Initialize(CWnd* pParentWnd, char* szDriverFileName)
{
	int nSocketType = 0;
	char szIp[128] = {0,};
	int nPort = 0;

	char szDefault[128];
	char szBuff[512] = {0, };
	char szLogFileName[ 512 ] = { 0, };

	CConfigData ConfigData(szDriverFileName);
	ConfigData.GetString("GUI TO MC COMM CONFIG", "IP", "127.0.0.1", szIp);
	nPort = ConfigData.GetInt("GUI TO MC COMM CONFIG", "Port", "6000");
	nSocketType = ConfigData.GetInt("GUI TO MC COMM CONFIG", "Socket Type", "0");

	memset(szDefault, 0x00, sizeof(szDefault));
	sprintf_s(szDefault, sizeof(szDefault), "IPC_GUI_Comm.log");
	ConfigData.GetString("GUI TO MC COMM CONFIG", "IPC GUI Comm Log File Name", szDefault, szBuff);
	sprintf_s(szLogFileName, sizeof(szLogFileName), "%s%s", SZ_LOG_FILE_PATH, szBuff);
	m_Logger.SetFileName(szLogFileName);

	sprintf_s(szLogFileName, sizeof(szLogFileName), "%sGUI%s", SZ_LOG_FILE_PATH, szBuff);

	int nRet = CIPCSock::Initialize(pParentWnd, nSocketType, szIp, nPort, CMCComm::MessageMap, szLogFileName);
	SendLogMessageToView("IPC MC Comm Initialized. [ERR=%d]", nRet);
	if( nRet != 0 ){
		AfxMessageBox("can not init mc comm socket!!!");
		return -1;
	}
	nRet = SocketConnect();
	SendLogMessageToView("IPC MC Comm connect. [%s, IP:%s, PORT:%d] [ERR=%d]", (nSocketType==eSocketTypeClient)?"Client":"Server", szIp, nPort, nRet);
	if( nRet != 0 ){
		AfxMessageBox("can not connect mc comm socket!!!");
		return -1;
	}
	return nRet;
}

void CMCComm::Finalize()
{
	SendLogMessageToView("IPC MC Comm Finalized.");
	CIPCSock::Finalize();
}

LRESULT CMCComm::OnTWIPCEvent(WPARAM wParam, LPARAM lParam)
{
	CIPCSock::OnTWIPCEvent(wParam, lParam);

	int nEventID = wParam;
	int nObserverCount = GetObserverCount();
	for(int i=0; i<nObserverCount; i++){
		CWnd* pWnd = GetObserverByIndex(i);
		pWnd->SendMessage(UM_MC_COMM_SOCK_EVENT, (WPARAM)NULL, (LPARAM)nEventID);
	}

	switch ( nEventID )
	{
	case IPC_EVT_CONNECTED:
		{
			SendLogMessageToView("Connected.");
		}break;
	case IPC_EVT_DISCONNECTED:
		{
			SendLogMessageToView("Disconnected.\n");
		}break;
	case IPC_EVT_RCV_EMPTY:
		{
		}break;
	case IPC_EVT_RCV_ERROR:
		{
		}break;
	case IPC_EVT_TM_OVER_MES:
		{
		}break;
	}

	return 0L;
}

int CMCComm::OnReceiveS2F3(CPacket* pPacket)
{
	g_McComm.SendLogMessageToView("[rcv] S%dF%d. MSGID=%d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	CPacket* pReplyPacket = new CPacket(2,4);
	pReplyPacket->pHead->stData.nMsgId = pPacket->pHead->stData.nMsgId;
	CPacketBody_S2F4* pReplyBody = (CPacketBody_S2F4*)pReplyPacket->pBody;
	g_McComm.Send( pReplyPacket, FALSE);
	g_McComm.SendLogMessageToView("[send] S%dF%d", pReplyPacket->pHead->stData.nS,pReplyPacket->pHead->stData.nF);
	delete pReplyPacket;
	pReplyPacket = NULL;

	return 0;
}

int CMCComm::OnReceiveS2F4(CPacket* pPacket)
{
	g_McComm.SendLogMessageToView("[rcv] S%dF%d. MSGID=%d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	return 0;
}

int CMCComm::OnReceiveS2F41(CPacket* pPacket)
{
	CSLTView* pView = (CSLTView*)g_McComm.m_pParentWnd;

	g_McComm.SendLogMessageToView("[rcv] S%dF%d. MSGID=%d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	int nCmd = pBody->stData.nCmd;
	switch (nCmd)
	{
	case GUI_RCMD_REQ_AUTOGRR_CANCEL:
		{
			int nParamData = atoi(pBody->stData.szParam[0]);
			if (nParamData == TRUE)
			{
				pView->EnableAutoGRRCancel();
			}
			else
			{
				pView->DisableAutoGRRCancel();	
			}
		}break;
	case GUI_RCMD_REQ_DATA_ASSIGN_SHOW:
		{
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			HWND hWnd = pMainFrame->GetSafeHwnd();
			::SendMessage(hWnd, UM_ALARM_REPORT, (WPARAM)(atoi(pBody->stData.szParam[0])), NULL);
// 			CDataAssignDlg DO;
// 			int nParamData = atoi(pBody->stData.szParam[0]);
// 			DO.SetPosNum(nParamData);
// 			DO.SetSkiptoAssignFlag(TRUE);
// 			DO.DoModal();
		}break;

	case GUI_RCMD_REQ_TESTPP_OFFSET_SHOW:
		{
			CUserOffSetTestPp_MP DO;
			DO.DoModal();
		}break;
	case GUI_RCMD_REQ_LOTINFO_SHOW:
		{
			g_DMCont.m_dmEQP.SB(BDM0_OPEN_LOTINFO_DLG_HISTORY, TRUE);
			int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

			CLotInfoAmkorDlg DO;
			DO.m_pSltView = pView;
			DO.SetLogInLevel(pView->m_nLogInLv);
			DO.DoModal();
						
			g_DMCont.m_dmEQP.SB(BDM0_OPEN_LOTINFO_DLG_HISTORY, FALSE);
		}break;
	case GUI_RCMD_REQ_QA_QTY_SHOW:
	{
		CAutoQASetDlg dlg;
		dlg.DoModal();
	}break;
	case GUI_RCMD_REQ_DOOR_STATUS_VIEW:
	{
// 		int nParamData = atoi(pBody->stData.szParam[0]);
// 		pView->CreateViewDialog(CSLTView::eBTN_SHOW_WINDOW::SW_MAIN_WINDOW);
// 		pView->TabSelect(nParamData);
	
	}break;
	case GUI_RCMD_REQ_MAKE_SUMMARY_END:
		{
			AfxMessageBox("Make Summary End!!!");
		}break;
	}

	CPacket* pReplyPacket = new CPacket(2,42);
	pReplyPacket->pHead->stData.nMsgId = pPacket->pHead->stData.nMsgId;
	CPacketBody_S2F42* pReplyBody = (CPacketBody_S2F42*)pReplyPacket->pBody;
	pReplyBody->stData.nRCMDACK = acknowledge;
	g_McComm.Send( pReplyPacket, FALSE);
	g_McComm.SendLogMessageToView("[send] S%dF%d, RCMDACK=%d", pReplyPacket->pHead->stData.nS,
		                                                       pReplyPacket->pHead->stData.nF, 
															   pReplyBody->stData.nRCMDACK);
	delete pReplyPacket;
	pReplyPacket = NULL;

	return 0;
}

int CMCComm::OnReceiveS2F42(CPacket* pPacket)
{
	g_McComm.SendLogMessageToView("[rcv] S%dF%d. MSGID=%d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	return 0;
}

int CMCComm::OnReceiveS5F1 (CPacket* pPacket)
{
	g_McComm.SendLogMessageToView("[rcv] S%dF%d. MSGID=%d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	CPacket* pReplyPacket = new CPacket(5,2);
	pReplyPacket->pHead->stData.nMsgId = pPacket->pHead->stData.nMsgId;
	CPacketBody_S5F2* pReplyBody = (CPacketBody_S5F2*)pReplyPacket->pBody;
	pReplyBody->stData.nACK = acknowledge;
	g_McComm.Send( pReplyPacket, FALSE);
	g_McComm.SendLogMessageToView("[send] S%dF%d", pReplyPacket->pHead->stData.nS,pReplyPacket->pHead->stData.nF);
	delete pReplyPacket;
	pReplyPacket = NULL;

	return 0;
}

int CMCComm::OnReceiveS5F2 (CPacket* pPacket)
{
	g_McComm.SendLogMessageToView("[rcv] S%dF%d. MSGID=%d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	return 0;
}
int CMCComm::OnReceiveS6F11(CPacket* pPacket)
{
	g_McComm.SendLogMessageToView("[rcv] S%dF%d. MSGID=%d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	CPacket* pReplyPacket = new CPacket(6,12);
	pReplyPacket->pHead->stData.nMsgId = pPacket->pHead->stData.nMsgId;
	CPacketBody_S6F12* pReplyBody = (CPacketBody_S6F12*)pReplyPacket->pBody;
	pReplyBody->stData.nACK = acknowledge;
	g_McComm.Send( pReplyPacket, FALSE);
	g_McComm.SendLogMessageToView("[send] S%dF%d", pReplyPacket->pHead->stData.nS,pReplyPacket->pHead->stData.nF);
	delete pReplyPacket;
	pReplyPacket = NULL;

	return 0;
}

int CMCComm::OnReceiveS6F12(CPacket* pPacket)
{
	g_McComm.SendLogMessageToView("[rcv] S%dF%d. MSGID=%d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	return 0;
}

int CMCComm::OnReceiveS107F1(CPacket* pPacket)
{
	g_McComm.SendLogMessageToView("[rcv] S%dF%d. MSGID=%d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	CPacket* pReplyPacket = new CPacket(107,2);
	pReplyPacket->pHead->stData.nMsgId = pPacket->pHead->stData.nMsgId;
	CPacketBody_S107F2* pReplyBody = (CPacketBody_S107F2*)pReplyPacket->pBody;
	g_McComm.Send( pReplyPacket, FALSE);
	g_McComm.SendLogMessageToView("[send] S%dF%d", pReplyPacket->pHead->stData.nS,pReplyPacket->pHead->stData.nF);
	delete pReplyPacket;
	pReplyPacket = NULL;

	return 0;
}

int CMCComm::OnReceiveS107F2(CPacket* pPacket)
{
	g_McComm.SendLogMessageToView("[rcv] S%dF%d. MSGID=%d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	return 0;
}

int CMCComm::OnReceiveS107F3(CPacket* pPacket)
{
	g_McComm.SendLogMessageToView("[rcv] S%dF%d. MSGID=%d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	CPacket* pReplyPacket = new CPacket(107,3);
	pReplyPacket->pHead->stData.nMsgId = pPacket->pHead->stData.nMsgId;
	CPacketBody_S107F4* pReplyBody = (CPacketBody_S107F4*)pReplyPacket->pBody;
	g_McComm.Send( pReplyPacket, FALSE);
	g_McComm.SendLogMessageToView("[send] S%dF%d", pReplyPacket->pHead->stData.nS,pReplyPacket->pHead->stData.nF);
	delete pReplyPacket;
	pReplyPacket = NULL;

	return 0;
}

int CMCComm::OnReceiveS107F4(CPacket* pPacket)
{
	g_McComm.SendLogMessageToView("[rcv] S%dF%d. MSGID=%d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	return 0;
}

int CMCComm::OnReceiveS107F5(CPacket* pPacket)
{
	g_McComm.SendLogMessageToView("[rcv] S%dF%d. MSGID=%d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	CPacket* pReplyPacket = new CPacket(107,6);
	pReplyPacket->pHead->stData.nMsgId = pPacket->pHead->stData.nMsgId;
	CPacketBody_S107F6* pReplyBody = (CPacketBody_S107F6*)pReplyPacket->pBody;
	g_McComm.Send( pReplyPacket, FALSE);
	g_McComm.SendLogMessageToView("[send] S%dF%d", pReplyPacket->pHead->stData.nS,pReplyPacket->pHead->stData.nF);
	delete pReplyPacket;
	pReplyPacket = NULL;

	return 0;
}

int CMCComm::OnReceiveS107F6(CPacket* pPacket)
{
	g_McComm.SendLogMessageToView("[rcv] S%dF%d. MSGID=%d", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->stData.nMsgId);

	return 0;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-11-23]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CMCComm::AttachObserver(CWnd* pObserver)
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
int CMCComm::DetachObserver(CWnd* pObserver)
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
void CMCComm::ClearObserver()
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
int CMCComm::GetObserverCount()
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
CWnd* CMCComm::GetObserverByIndex(int nIndex)
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
BOOL CMCComm::CheckDuplicate(CWnd* pItem)
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

void CMCComm::SendLogMessageToView(const char* fmt, ...)
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
		pWnd->SendMessage(UM_MC_COMM_LOG_MSG, (WPARAM)NULL, (LPARAM)szEventMessage);
	}
}

int CMCComm::CmdProcessControl(int nEvtSeqID)
{
    CPacket* pPacket = new CPacket(2, 41);
    CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
    pBody->stData.nCmd = GUI_RCMD_REQ_PAUSE;
    pBody->stData.nParamCount = 1;
    
    switch(nEvtSeqID)   //  enum eAutoLotCmd{
    {
    case eAutoLotCmd_Pause:
        {
            sprintf_s(pBody->stData.szParam[0] , "%d", eAutoLotCmd_Pause);
        }break;
    default:
        {
            AfxMessageBox(" CmdProcessCtrl parameter Error!!!");
            delete pPacket;
            pPacket = NULL;
            return -1;
        }break;
    }

    int nErr = CIPCSock::Send(pPacket, FALSE);
    if( nErr != 0 ){
        AfxMessageBox(" GUI to MC Send data error!!!");
    }

    delete pPacket;
    pPacket = NULL;

    return 0;
}

int CMCComm::OnSndFinalizeMC()
{
    CPacket* pPacket = new CPacket(2, 41);
    CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
    pBody->stData.nCmd = GUI_RCMD_REQ_FINALIZE_PGM;
    pBody->stData.nParamCount = 0;

    int nErr = CIPCSock::Send(pPacket, FALSE);
    if( nErr != 0 ){
        AfxMessageBox(" Dialog command error!!!");
    }

    delete pPacket;
    pPacket = NULL;

    return 0;
}

int CMCComm::CmdYieldMonitorDataClear()
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = GUI_RCMD_REQ_YIELD_MONITOR_CLEAR;
	pBody->stData.nParamCount = 0;

	int nErr = CIPCSock::Send(pPacket, FALSE);
	if( nErr != 0 ){
		AfxMessageBox(" Dialog command error!!!");
	}

	delete pPacket;
	pPacket = NULL;

	return 0;
}

int CMCComm::CmdBinCategoryRefresh()
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = GUI_RCMD_REQ_BIN_CATEGORY_REFRESH;
	pBody->stData.nParamCount = 0;

	int nErr = CIPCSock::Send(pPacket, FALSE);
	if( nErr != 0 ){
		AfxMessageBox(" Dialog command error!!!");
	}

	delete pPacket;
	pPacket = NULL;

	BOOL bDotMatrix = g_DMCont.m_dmEQP.GB(BDM0_MD_DOTMATRIX);
	if (bDotMatrix == eOPTION_USE)
		SetDotMatrixUpdate();	

	return 0;
}


int CMCComm::CmdOnlyOneCmdNoParam(int nCmdID)
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = nCmdID;
	pBody->stData.nParamCount = 0;

	int nErr = CIPCSock::Send(pPacket, FALSE);
	if( nErr != 0 ){
		AfxMessageBox(" Dialog command error!!!");
	}

	delete pPacket;
	pPacket = NULL;

	return 0;
}

int CMCComm::CmdVisionUseNouse(int nTargetPos)
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;

	if(nTargetPos == eVisionPos_TestSite){
		pBody->stData.nCmd = GUI_RCMD_REQ_SITE_VISION_SET_CHNG;
	}else if(nTargetPos == eVisionPos_2dBarcode){
		pBody->stData.nCmd = GUI_RCMD_REQ_2D_VISION_SET_CHNG;
	}	
	pBody->stData.nParamCount = 0;

	int nErr = CIPCSock::Send(pPacket, FALSE);
	if( nErr != 0 ){
		AfxMessageBox(" Dialog command error!!!");
	}

	delete pPacket;
	pPacket = NULL;

	return 0;
}

int CMCComm::CmdTesterOnlineModeChange(int nMode)
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = GUI_RCMD_REQ_TESTER_ONLINE_MODE_CHANGE;
	pBody->stData.nParamCount = 1;
	sprintf_s(pBody->stData.szParam[0], "%d", nMode);

	int nErr = CIPCSock::Send(pPacket, FALSE);
	if (nErr != 0) {
		AfxMessageBox("can not send tester mode!! please check tester GUI");
	}

	delete pPacket;
	pPacket = NULL;

	return 0;
}

int CMCComm::CmdDefaultBinCategoryRefresh()
{
	CPacket* pPacket = new CPacket( 2, 41 );
	CPacketBody_S2F41* pBody = ( CPacketBody_S2F41* )pPacket->pBody;
	pBody->stData.nCmd = GUI_RCMD_REQ_DEGAULT_BIN_CATEGORY_REFRESH;
	pBody->stData.nParamCount = 0;

	int nErr = CIPCSock::Send( pPacket, FALSE );
	if( nErr != 0 ) {
		AfxMessageBox( " Dialog command error!!!" );
	}

	delete pPacket;
	pPacket = NULL;

	return 0;
}

int CMCComm::CmdTesterBatchFileExcute()
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = GUI_RCMD_REQ_TESTER_BATCH_FILE_EXCUTE;
	pBody->stData.nParamCount = 0;

	int nErr = CIPCSock::Send(pPacket, FALSE);
	if (nErr != 0) {
		AfxMessageBox("can not send tester mode!! please check tester GUI");
	}

	delete pPacket;
	pPacket = NULL;

	return 0;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   :DlgIdx = enum eDlgProcCmd{ 
  RETURN   : 
  COMMENTS : GIU->MC Open Dialog
==============================================================================*/
int CMCComm::OnSendDlgShow(int nDlgIdx)
{
    CPacket* pPacket = new CPacket(2, 41);
    CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
    pBody->stData.nCmd = GUI_RCMD_REQ_SHOW_DLG;
    pBody->stData.nParamCount = 1;

//     CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
//     CSLTView* pSltView = (CSLTView*)pMainFrm->GetActiveView();
//     pSltView->m_nBtnClickWindow = -1;
//     pSltView->DestroyDialog();
    //             delete pSltView->m_pDlgSetup;
    //             pSltView->m_pDlgSetup = NULL;

    switch(nDlgIdx)
    {
    case eDlgProcCmd_Position:
        {
            sprintf_s(pBody->stData.szParam[0] , "%d", eDlgProcCmd_Position);
         }break;
    case eDlgProcCmd_IoMonitorDIO:
        {
            sprintf_s(pBody->stData.szParam[0] , "%d", eDlgProcCmd_IoMonitorDIO);
        }break;
    case eDlgProcCmd_IoMonitorAIO:
        {
            sprintf_s(pBody->stData.szParam[0] , "%d", eDlgProcCmd_IoMonitorAIO);
        }break;
    case eDlgProcCmd_Alignment:
        {
            sprintf_s(pBody->stData.szParam[0] , "%d", eDlgProcCmd_Alignment);
        }break;
    case eDlgProcCmd_MotorMonitor:
        {
            sprintf_s(pBody->stData.szParam[0] , "%d", eDlgProcCmd_MotorMonitor);
        }break;
    case eDlgProcCmd_TesterIfTest:
        {
            sprintf_s(pBody->stData.szParam[0] , "%d", eDlgProcCmd_TesterIfTest);
        }break;
	case eDlgProcCmd_SignalTower:
		{
			sprintf_s(pBody->stData.szParam[0] , "%d", eDlgProcCmd_SignalTower);
		}break;
	case eDlgProcCmd_SocketContactCount:
		{
			sprintf_s(pBody->stData.szParam[0], "%d", eDlgProcCmd_SocketContactCount);
		}break;
	case eDlgProcCmd_ServoMotor:
		{
			sprintf_s(pBody->stData.szParam[0], "%d", eDlgProcCmd_ServoMotor);
		}break;
	case eDlgProcCmd_TesterInfo:
		{
			sprintf_s(pBody->stData.szParam[0], "%d", eDlgProcCmd_TesterInfo);
		}break;
	case eDlgProcCmd_LotInfoSave:
		{
			sprintf_s(pBody->stData.szParam[0], "%d", eDlgProcCmd_LotInfoSave);
		}break;
	case eDlgProcCmd_VAT:
		{
			sprintf_s(pBody->stData.szParam[0], "%d", eDlgProcCmd_VAT);
		}break;
	case eDlgProcCmd_TrayView:
		{
			sprintf_s(pBody->stData.szParam[0], "%d", eDlgProcCmd_TrayView);
		}break;
	case eDlgProcCmd_BatchFileErr:
	{
		sprintf_s(pBody->stData.szParam[0], "%d", eDlgProcCmd_BatchFileErr);
	}break;
	case eDlgProcCmd_BarcodeSetting:
	{
		sprintf_s(pBody->stData.szParam[0], "%d", eDlgProcCmd_BarcodeSetting);
	}break;
    default:
        {
            AfxMessageBox(" Open dialog command Fail!!!");
            delete pPacket;
            pPacket = NULL;
            return -1;
        }break;
    }

    int nErr = CIPCSock::Send(pPacket, FALSE);
    if( nErr != 0 ){
        AfxMessageBox(" Dialog command error!!!");
    }

    delete pPacket;
    pPacket = NULL;

    return 0;
}
/*=============================================================================
  AUTHOR   : 손정일
  NAME     : 
  PARAMS   :
  RETURN   : 
  COMMENTS : Contact Control 화면에서 Press Unit Contact Pos 변경시 
             GIU->MC Teaching 값 ReLoad
==============================================================================*/
int CMCComm::CmdTeachingDataReLoad()
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = GUI_RCMD_REQ_TEACHING_RELOAD;
	pBody->stData.nParamCount = 0;

	int nErr = CIPCSock::Send(pPacket, FALSE);
	if( nErr != 0 ){
		AfxMessageBox(" Dialog command error!!!");
	}

	delete pPacket;
	pPacket = NULL;

	return 0;
}

/*=============================================================================
  AUTHOR   : 손정일
  NAME     : 
  PARAMS   :
  RETURN   : 
  COMMENTS : 여러 개의 Lot이 하나의 Summary로 되어야 할 경우 때문 생성           
==============================================================================*/
int CMCComm::MakeSummary()
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = GUI_RCMD_REQ_MAKE_SUMMARY;
	pBody->stData.nParamCount = 0;

	int nErr = CIPCSock::Send(pPacket, FALSE);
	if( nErr != 0 ){
		AfxMessageBox(" Dialog command error!!!");
	}

	delete pPacket;
	pPacket = NULL;

	return 0;
}

int CMCComm::CmdYieldMonitorDataClear_Each(int nIdx, int nInlinePara, int nYieldMode,int nSocketNo)
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = GUI_RCMD_REQ_YIELD_MONITOR_CLEAR_EACH;
	pBody->stData.nParamCount = 3;

	sprintf_s(pBody->stData.szParam[0] , "%d", nIdx);
	//sprintf_s(pBody->stData.szParam[1] , "%d", nInlinePara);
	sprintf_s(pBody->stData.szParam[1], "%d", nYieldMode);
	sprintf_s(pBody->stData.szParam[2], "%d", nSocketNo);

	int nErr = CIPCSock::Send(pPacket, FALSE);
	if( nErr != 0 ){
		AfxMessageBox(" Dialog command error!!!");
	}

	delete pPacket;
	pPacket = NULL;

	return 0;
}

int CMCComm::CmdModuleSiteOnPossible()
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = GUI_RCMD_REQ_MODULE_SITE_ON_IS_POSSIBLE;
	pBody->stData.nParamCount = 0;

	int nErr = CIPCSock::Send(pPacket, FALSE);
	if (nErr != 0) {
		AfxMessageBox(" Dialog command error!!!");
	}

	delete pPacket;
	pPacket = NULL;

	return 0;
}

int CMCComm::CmdTrayPPSafetyMovePos()
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = GUI_RCMD_REQ_TRAYPP_SAFETY_POS_MOVE;
	pBody->stData.nParamCount = 0;

	int nErr = CIPCSock::Send(pPacket, FALSE);
	if (nErr != 0) {
		AfxMessageBox(" SLT MC Off or TrayPP를 이동 할 수 없는 상태 입니다!!!");
	}

	delete pPacket;
	pPacket = NULL;

	return 0;
}

int CMCComm::CmdMultiOrigin(int nMode)
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = GUI_RCMD_REQ_MULTI_ORIGIN;
	pBody->stData.nParamCount = nMode;

	int nErr = CIPCSock::Send(pPacket, FALSE);
	if (nErr != 0) {
		AfxMessageBox(" Dialog command error!!!");
	}

	delete pPacket;
	pPacket = NULL;

	return 0;
}

int CMCComm::CmdLoadSupply()
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = GUI_RCMD_REQ_LOAD_SUPPLY;

	int nErr = CIPCSock::Send(pPacket, FALSE);
	if (nErr != 0) {
		AfxMessageBox(" Dialog command error!!!");
	}

	delete pPacket;
	pPacket = NULL;

	return 0;
}