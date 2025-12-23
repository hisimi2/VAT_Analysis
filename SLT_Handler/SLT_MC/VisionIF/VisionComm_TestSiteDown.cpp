#include "StdAfx.h"
#include "VisionComm_TestSiteDown.h"
#include "ConfigData.h"
#include "IPCSock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CVisionComm_TestSiteDown::CVisionComm_TestSiteDown(int nVisionPcIdx, CString strVisionName)
	:CVisionComm(nVisionPcIdx, strVisionName)
{
}


CVisionComm_TestSiteDown::CVisionComm_TestSiteDown(void)
{
}


CVisionComm_TestSiteDown::~CVisionComm_TestSiteDown(void)
{
}


ST_MESSAGE_MAP CVisionComm_TestSiteDown::MessageMap[] ={
	{2,3,  CVisionComm_TestSiteDown::OnReceiveS2F3 },
	{2,4,  CVisionComm_TestSiteDown::OnReceiveS2F4 },
	{2,41,  CVisionComm_TestSiteDown::OnReceiveS2F41 },
	{2,42,  CVisionComm_TestSiteDown::OnReceiveS2F42 },
	{5, 1,  CVisionComm_TestSiteDown::OnReceiveS5F1  },
	{5, 2,  CVisionComm_TestSiteDown::OnReceiveS5F2  },
	{6,11,  CVisionComm_TestSiteDown::OnReceiveS6F11 },
	{6,12,  CVisionComm_TestSiteDown::OnReceiveS6F12 },
	{107,1, CVisionComm_TestSiteDown::OnReceiveS107F1 },
	{107,2, CVisionComm_TestSiteDown::OnReceiveS107F2 },
	{107,3, CVisionComm_TestSiteDown::OnReceiveS107F3 },
	{107,4, CVisionComm_TestSiteDown::OnReceiveS107F4 },
	{107,5, CVisionComm_TestSiteDown::OnReceiveS107F5 },
	{107,6, CVisionComm_TestSiteDown::OnReceiveS107F6 },
	{-1,-1, NULL},
};


int CVisionComm_TestSiteDown::Initialize(CWnd* pParentWnd, char* szDriverFileName)
{
	int nSocketType = 0;
	char szIp[128] = {0,};
	int nPort = 0;

	char szKey[128];
	char szDefault[128];
	char szBuff[512] = {0, };
	char szLogFileName[ 512 ] = { 0, };

	strcpy_s(m_szDriverFileName, sizeof(m_szDriverFileName), szDriverFileName);

	CConfigData ConfigData(szDriverFileName);
	ZeroMemory(szKey, sizeof(szKey));
	sprintf_s(szKey, sizeof(szKey), "%s IP", m_strVisionPcName.GetString() );
	ConfigData.GetString("MC TO VISION COMM CONFIG", szKey, "127.0.0.1", szIp);

	ZeroMemory(szKey, sizeof(szKey));
	sprintf_s(szKey, sizeof(szKey), "%s Port", m_strVisionPcName.GetString() );

	ZeroMemory(szDefault, sizeof(szDefault));
	sprintf_s(szDefault, sizeof(szDefault), "%d", 8000+m_nVisionPcIdx);

	nPort = ConfigData.GetInt("MC TO VISION COMM CONFIG", szKey, szDefault);

	nSocketType = ConfigData.GetInt("MC TO VISION COMM CONFIG", "Socket Type", "0");

	memset(szDefault, 0x00, sizeof(szDefault));
	sprintf_s(szDefault, sizeof(szDefault), "VisionComm_TestSiteDn.log");
	ConfigData.GetString( "MC TO VISION COMM CONFIG", "TestSiteDown Log File Name", szDefault, szBuff);
	sprintf_s(szLogFileName, sizeof(szLogFileName), "%s%s", SZ_LOG_FILE_PATH, szBuff);
	m_Logger.SetFileName(szLogFileName);
	sprintf_s(szLogFileName, sizeof(szLogFileName), "%sDown%s", SZ_LOG_FILE_PATH, szBuff);


	int nRet = CIPCSock::Initialize(pParentWnd, nSocketType, szIp, nPort, CVisionComm_TestSiteDown::MessageMap, szLogFileName);
	SendLogMessage("IPC Vision TestSite Down Comm Initialized. [ERR=%d]", nRet);

	if( nRet != 0 ){
		AfxMessageBox("can not init vision socket!!!");
		return -1;
	}
	nRet = CIPCSock::SocketConnect(nSocketType, szIp, nPort);
	SendLogMessage("IPC Vision TestSite Down Comm connect. [%s, IP:%s, PORT:%d] [ERR=%d]", (nSocketType==eSocketTypeClient)?"Client":"Server", szIp, nPort, nRet);
	if( nRet != 0 ){
		AfxMessageBox("can not connect vision socket!!!");
		return -1;
	}
	return nRet;
}

void CVisionComm_TestSiteDown::Finalize()
{
	SendLogMessage("IPC Vision Site Down Comm Finalized.");
	CIPCSock::Finalize();
}

LRESULT CVisionComm_TestSiteDown::OnTWIPCEvent(WPARAM wParam, LPARAM lParam)
{
	CIPCSock::OnTWIPCEvent(wParam, lParam);

	int nEventID = wParam;

	int nObserverCount = GetObserverCount();
	for(int i=0; i<nObserverCount; i++){
		CWnd* pWnd = GetObserverByIndex(i);
		pWnd->SendMessage(UM_VISION_IF_SOCK_EVENT, (WPARAM)m_nVisionPcIdx, (LPARAM)nEventID);
	}

	switch ( nEventID )
	{
	case IPC_EVT_CONNECTED:
		{
			TRACE("Site Down Vision Connected.\n");
			g_pTestSiteInspDn->SendLogMessage("Site Down Vision Connected");
		}break;
	case IPC_EVT_DISCONNECTED:
		{
			TRACE("Site Down Vision Disconnected.\n");
			g_pTestSiteInspDn->SendLogMessage("Site Down Vision Disconnected");
			g_pTestSiteInspDn->m_csCmdDone.Lock();

			//int nWaitMsgCount = m_VisionCmdResult.size();
			for(int i=0; i<8; i++)
			{
				//_tVisionMsg* pItem = (_tVisionMsg*)m_VisionCmdResult[i];
				//pItem->nReceived = eVisionMsgWaitState_Canceled;
				g_pTestSiteInspDn->m_VisionMsg[i].nReceived = eVisionMsgWaitState_Canceled; 
			}
			g_pTestSiteInspDn->m_csCmdDone.Unlock();
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
	return 0;
}



//==============================================================================
//
//==============================================================================
int CVisionComm_TestSiteDown::OnReceiveS2F3(CPacket* pPacket)
{
	CPacketBody_S2F4* pBody = (CPacketBody_S2F4*)pPacket->pBody;
	g_pTestSiteInspDn->SendLogMessage("[rcv] S2F4. MSGID : %d, DataCount=%d", pPacket->pHead->stData.nMsgId, pBody->stData.nDataCount);
	for(int i=0; i<pBody->stData.nDataCount; i++){
		g_pTestSiteInspDn->SendLogMessage("      Data %d = %s", i+1, pBody->stData.szDataValue[i]);
	}
	return 0;
}

int CVisionComm_TestSiteDown::OnReceiveS2F4(CPacket* pPacket)
{
	CPacketBody_S2F4* pBody = (CPacketBody_S2F4*)pPacket->pBody;

	int nReqCount =0;
	//g_pTestSiteInspDn->m_csCmdDone.Lock();

	
	//int nWaitMsgCount = g_pTestSiteInspDn->m_VisionCmdResult.size();
	//for(int i=0; i<nWaitMsgCount; i++)
	//{
	//	_tVisionMsg* pItem = (_tVisionMsg*)g_pTestSiteInspDn->m_VisionCmdResult[i];
	//	if(pItem == NULL) continue;
	//	if( pItem->nMsgId == pPacket->pHead->stData.nMsgId )
	//	{
	//		for(int j=0; j<(int)pItem->vReqData.size(); j++)
	//		{
	//			strcpy_s(pItem->vReqData[j].szDataValue, sizeof(pItem->vReqData[j].szDataValue), pBody->stData.szDataValue[j]);
	//		}
	//		
	//		if((int)pItem->vReqData.size() == 0 && pBody->stData.nDataCount != 0){
	//			for(int j =0; j<pBody->stData.nDataCount; j++){
	//				strcpy_s(pItem->vReqData[j].szDataValue,sizeof(pItem->vReqData[j].szDataValue),pBody->stData.szDataValue[j]);
	//			}
	//		}
	//		pItem->nReceived = eVisionMsgWaitState_Received;
	//		nReqCount = (int)pItem->vReqData.size();
	//		break;
	//	}
	//}
	int nIdex =0;

	for (int i = 0; i<8; i++)
	{
		if( g_pTestSiteInspDn->m_VisionMsg[i].nMsgId == pPacket->pHead->stData.nMsgId){
			nIdex = i;
			nReqCount = g_pTestSiteInspDn->m_VisionMsg[i].nDataCount = pBody->stData.nDataCount;

			for(int j=0; j<pBody->stData.nDataCount; j++)
			{
				//g_pTestSiteInspDn->m_VisionMsg[i].vReqData.push_back(pBody->stData.szDataValue[j]);
				strcpy_s(g_pTestSiteInspDn->m_VisionMsg[i].szData[j], pBody->stData.szDataValue[j]);
			}
			//g_pTestSiteInspDn->m_VisionMsg[i].vReqData = pPacket->pHead.stData;
			g_pTestSiteInspDn->m_VisionMsg[i].nReceived = eVisionMsgWaitState_Received;
			//(int)g_pTestSiteInspDn->m_VisionMsg[i].vReqData.size();
		}
	}

	//g_pTestSiteInspDn->m_csCmdDone.Unlock();

	g_pTestSiteInspDn->SendLogMessage("[rcv] S2F4. MSGID : %d, DataCount=%d, vReqDataCount = %d", pPacket->pHead->stData.nMsgId, pBody->stData.nDataCount,nReqCount);
	for(int i=0; i<pBody->stData.nDataCount; i++){
		g_pTestSiteInspDn->SendLogMessage("      Data %d = %s", i+1, pBody->stData.szDataValue[i]);
	}
	for(int i=0; i< nReqCount; i++){
		g_pTestSiteInspDn->SendLogMessage("vReqData Data %d = %s", i+1, g_pTestSiteInspDn->m_VisionMsg[nIdex].szData[i]);
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
int CVisionComm_TestSiteDown::OnReceiveS2F41(CPacket* pPacket)
{
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	g_pTestSiteInspDn->SendLogMessage("[TestSiteDown rcv] S2F41. MSGID : %d, DataCount=%d", pPacket->pHead->stData.nMsgId, pBody->stData.nCmd);
	for(int i=0; i<pBody->stData.nParamCount; i++){
		g_pTestSiteInspDn->SendLogMessage("      Data %d = %s", i+1, pBody->stData.szParam[i]);
	}

	CPacket* pRePacket = new CPacket(2,42);
	pRePacket->pHead->stData.nMsgId = pPacket->pHead->stData.nMsgId;
	CPacketBody_S2F42* pReBody = (CPacketBody_S2F42*)pRePacket->pBody;
	pReBody->stData.nRCMDACK = 0;
	g_pTestSiteInspDn->Send(pRePacket, FALSE);
	g_pTestSiteInspDn->SendLogMessage("[snd] TestSiteDown OnReceiveS2F41 Ack S2F42, MsgId : %d, RCMDACK=%d", pRePacket->pHead->stData.nMsgId, pReBody->stData.nRCMDACK);
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_TestSiteDown::OnReceiveS2F42(CPacket* pPacket)
{
	CPacketBody_S2F42* pBody = (CPacketBody_S2F42*)pPacket->pBody;
	
	//g_pTestSiteInspDn->m_csCmdDone.Lock();
	
	//int nWaitMsgCount = g_pTestSiteInspDn->m_VisionCmdResult.size();
	//for(int i=0; i<nWaitMsgCount; i++)
	//{
	//	_tVisionMsg* pItem = (_tVisionMsg*)g_pTestSiteInspDn->m_VisionCmdResult[i];
	//	if( pItem->nMsgId == pPacket->pHead->stData.nMsgId )
	//	{
	//		pItem->nRcmdAck = pBody->stData.nRCMDACK;
	//		pItem->nReceived = eVisionMsgWaitState_Received;
	//		break;
	//	}
	//}

	for(int i=0; i<8; i++){
		if(g_pTestSiteInspDn->m_VisionMsg[i].nMsgId == pPacket->pHead->stData.nMsgId)
		{
			g_pTestSiteInspDn->m_VisionMsg[i].nRcmdAck = pBody->stData.nRCMDACK;
			g_pTestSiteInspDn->m_VisionMsg[i].nReceived = eVisionMsgWaitState_Received;
		}
	}
	//g_pTestSiteInspDn->m_csCmdDone.Unlock();

	g_pTestSiteInspDn->SendLogMessage("[rcv] S2F42. MSGID : %d, RCMDACK=%d", pPacket->pHead->stData.nMsgId, pBody->stData.nRCMDACK);
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_TestSiteDown::OnReceiveS5F1 (CPacket* pPacket)
{
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_TestSiteDown::OnReceiveS5F2 (CPacket* pPacket)
{
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_TestSiteDown::OnReceiveS6F11(CPacket* pPacket)
{
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_TestSiteDown::OnReceiveS6F12(CPacket* pPacket)
{
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_TestSiteDown::OnReceiveS107F1(CPacket* pPacket)
{
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_TestSiteDown::OnReceiveS107F2(CPacket* pPacket)
{
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_TestSiteDown::OnReceiveS107F3(CPacket* pPacket)
{
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_TestSiteDown::OnReceiveS107F4(CPacket* pPacket)
{
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_TestSiteDown::OnReceiveS107F5(CPacket* pPacket)
{
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_TestSiteDown::OnReceiveS107F6(CPacket* pPacket)
{
	return 0;
}