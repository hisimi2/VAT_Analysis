#include "StdAfx.h"
#include "VisionComm_TestHand_1.h"
#include "ConfigData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CVisionComm_TestHand_1::CVisionComm_TestHand_1(int nVisionPcIdx, CString strVisionName)
	:CVisionComm(nVisionPcIdx, strVisionName)
{
	memset(m_nIsReceived, 0, sizeof(m_nIsReceived));
	m_bVisionComm = TRUE;
	m_vRcvData.clear();
	ST_PCD_BACKLASH_RCV_DATA vData;
	for (int i = 0; i < VAT_HAND_MAX; i++) {
		m_vRcvData.push_back(vData);
	}

	::InitializeCriticalSection(m_csSend);
	::InitializeCriticalSection(m_csRcv);
}


CVisionComm_TestHand_1::CVisionComm_TestHand_1(void)
{
	memset(m_nIsReceived, 0, sizeof(m_nIsReceived));
	m_bVisionComm = TRUE;


	::InitializeCriticalSection(m_csSend);
	::InitializeCriticalSection(m_csRcv);
}


CVisionComm_TestHand_1::~CVisionComm_TestHand_1(void)
{
	::DeleteCriticalSection(m_csSend);
	::DeleteCriticalSection(m_csRcv);
}


ST_MESSAGE_MAP CVisionComm_TestHand_1::MessageMap[] ={
	{2,3,   CVisionComm_TestHand_1::OnReceiveS2F3 },
	{2,4,   CVisionComm_TestHand_1::OnReceiveS2F4 },
	{2,41,  CVisionComm_TestHand_1::OnReceiveS2F41 },
	{2,42,  CVisionComm_TestHand_1::OnReceiveS2F42 },
	{5, 1,  CVisionComm_TestHand_1::OnReceiveS5F1  },
	{5, 2,  CVisionComm_TestHand_1::OnReceiveS5F2  },
	{6,11,  CVisionComm_TestHand_1::OnReceiveS6F11 },
	{6,12,  CVisionComm_TestHand_1::OnReceiveS6F12 },
	{107,1, CVisionComm_TestHand_1::OnReceiveS107F1 },
	{107,2, CVisionComm_TestHand_1::OnReceiveS107F2 },
	{107,3, CVisionComm_TestHand_1::OnReceiveS107F3 },
	{107,4, CVisionComm_TestHand_1::OnReceiveS107F4 },
	{107,5, CVisionComm_TestHand_1::OnReceiveS107F5 },
	{107,6, CVisionComm_TestHand_1::OnReceiveS107F6 },
	{107,9, CVisionComm_TestHand_1::OnReceiveS107F9 },
	{-1,-1, NULL},
};


int CVisionComm_TestHand_1::Initialize(CWnd* pParentWnd, char* szDriverFileName)
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
	sprintf_s(szDefault, sizeof(szDefault), "VisionComm_TestHand1.log");
	ConfigData.GetString( "MC TO VISION COMM CONFIG", "TestHand1 Log File Name", szDefault, szBuff);
	sprintf_s(szLogFileName, sizeof(szLogFileName), "%s%s", SZ_LOG_FILE_PATH, szBuff);
	m_Logger.SetFileName(szLogFileName);
	//sprintf_s(szLogFileName, sizeof(szLogFileName), "%s%s", SZ_LOG_FILE_PATH, szBuff);

	int nRet = CIPCSock::Initialize(pParentWnd, nSocketType, szIp, nPort, CVisionComm_TestHand_1::MessageMap, szLogFileName);
	SendLogMessage("IPC Vision TestHand1 Comm Initialized. [ERR=%d]", nRet);

	if( nRet != 0 ){
		AfxMessageBox("can not init vision socket!!!");
		return -1;
	}
	nRet = CIPCSock::SocketConnect(nSocketType, szIp, nPort);
	SendLogMessage("IPC Vision TestHand1 Comm connect. [%s, IP:%s, PORT:%d] [ERR=%d]", (nSocketType==eSocketTypeClient)?"Client":"Server", szIp, nPort, nRet);
	if( nRet != 0 ){
		AfxMessageBox("can not connect vision socket!!!");
		return -1;
	}
	return nRet;
}

void CVisionComm_TestHand_1::Finalize()
{
	SendLogMessage( " IPC Vision TestHand1 Comm Finalized " );
	//SendLogMessage("IPC Vision Test Site Up Comm Finalized.");
	CIPCSock::Finalize();
}


LRESULT CVisionComm_TestHand_1::OnTWIPCEvent(WPARAM wParam, LPARAM lParam)
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
			TRACE("TestHand1 Vision Connected.\n");
			SendLogMessage("TestHand1 Vision Connected");
			KillTimer(0);
			SendLogMessage("TestHand1 Vision KillTimer");
			Sleep(1000); //Vision 통신 연결 후 1초 Sleep 이후에, Set cok 
			Cmd_SetCok();
		}break;
	case IPC_EVT_DISCONNECTED:
		{
			TRACE("TestHand1 Vision Disconnected.\n");
			SendLogMessage("TestHand1 Vision Disconnected");
			m_csCmdDone.Lock();
			for (int i = 0; i < (int)m_VisionMsg.size(); i++)
				m_VisionMsg[i].nReceived = eVisionMsgWaitState_Canceled;

			for (int i = 0; i < VAT_HAND_MAX; i++)
			{
				m_nIsReceived[i] = CAM_STATUS_DISCONNECT;
			}

			//VAT
			for (int nCam = VAT_CAM_LD_TRAY_PP_UPPER; nCam <= VAT_CAM_TEST_PP_1_LOWER; nCam++)
			{
				g_VatVisionMgr[nCam].SetReceiveState(VAT_RCV_STATUS_DISCONNECT);
			}			

			m_csCmdDone.Unlock();

			//int nWaitMsgCount = m_VisionCmdResult.size();
			//for(int i=0; i<8; i++)
			//{
			//	//_tVisionMsg* pItem = (_tVisionMsg*)m_VisionCmdResult[i];
			//	//pItem->nReceived = eVisionMsgWaitState_Canceled;
			//	g_pTestSiteInspUp->m_VisionMsg[i].nReceived = eVisionMsgWaitState_Canceled; 
			//}
			//g_pTestSiteInspUp->m_csCmdDone.Unlock();
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
	case IPC_EVT_CLIENT_CAN_NOT_CONNECT:
		{

		}break;
	}
	return 0L;
}



//==============================================================================
//
//==============================================================================
int CVisionComm_TestHand_1::OnReceiveS2F3(CPacket* pPacket)
{
	CPacketBody_S2F4* pBody = (CPacketBody_S2F4*)pPacket->pBody;
	
	g_pVisionTestHand_1->SendLogMessage("[rcv] S2F4. MSGID : %d, DataCount=%d", pPacket->pHead->stData.nMsgId, pBody->stData.nDataCount);
	//g_pTestSiteInspUp->SendLogMessage("[rcv] S2F4. MSGID : %d, DataCount=%d", pPacket->pHead->stData.nMsgId, pBody->stData.nDataCount);
	for(int i=0; i<pBody->stData.nDataCount; i++){
		g_pVisionTestHand_1->SendLogMessage("      Data %d = %s", i + 1, pBody->stData.szDataValue[i]);
		//g_pTestSiteInspUp->SendLogMessage("      Data %d = %s", i+1, pBody->stData.szDataValue[i]);
	}
	return 0;
}



//========================================================================================
//
//========================================================================================
int CVisionComm_TestHand_1::OnReceiveS2F4(CPacket* pPacket)
{
	CPacketBody_S2F4* pBody = (CPacketBody_S2F4*)pPacket->pBody;

	int nReqCount =0;

	g_pVisionTestHand_1->m_csCmdDone.Lock();

	int nIndx = -1;
	for (int i = 0; i<4; i++)
	{
		//if( g_pTestSiteInspUp->m_VisionMsg[i].nMsgId == pPacket->pHead->stData.nMsgId){
		if( g_pVisionTestHand_1->m_VisionMsg[i].nMsgId == pPacket->pHead->stData.nMsgId )
		{
			nIndx = i;
			//nReqCount = g_pTestSiteInspUp->m_VisionMsg[i].nDataCount = pBody->stData.nDataCount;
			nReqCount = g_pVisionTestHand_1->m_VisionMsg[i].nDataCount = pBody->stData.nDataCount;
			g_pVisionTestHand_1->m_VisionMsg[i].nStatus = pBody->stData.nStatus;
			for(int j=0; j<pBody->stData.nDataCount; j++)
			{
				//g_pTestSiteInspUp->m_VisionMsg[i].vReqData.push_back(pBody->stData.szDataValue[j]);
				//strcpy_s(g_pTestSiteInspUp->m_VisionMsg[i].szData[j], pBody->stData.szDataValue[j]);
				strcpy_s(g_pVisionTestHand_1->m_VisionMsg[i].szData[j], pBody->stData.szDataValue[j]);
			}
			//g_pTestSiteInspUp->m_VisionMsg[i].vReqData = pPacket->pHead.stData;
			//g_pTestSiteInspUp->m_VisionMsg[i].nReceived = eVisionMsgWaitState_Received;
			g_pVisionTestHand_1->m_VisionMsg[i].nReceived = eVisionMsgWaitState_Received;
			//(int)g_pTestSiteInspUp->m_VisionMsg[i].vReqData.size();
		}

// 		if( g_pTestSiteInspUp->m_VisionMsg[i].nMsgId == pPacket->pHead->stData.nMsgId){
// 			for(int j=0; j<pBody->stData.nDataCount; j++)
// 			{
// 				g_pTestSiteInspUp->m_VisionMsg[i].vReqData.push_back(pBody->stData.szDataValue[j]);
// 			}
// 			//g_pTestSiteInspUp->m_VisionMsg[i].vReqData = pPacket->pHead.stData;
// 			g_pTestSiteInspUp->m_VisionMsg[i].nReceived = eVisionMsgWaitState_Received;
// 			nReqCount = (int)g_pTestSiteInspUp->m_VisionMsg[i].vReqData.size();
// 		}
	}
	//g_pTestSiteInspUp->m_csCmdDone.Unlock();
	g_pVisionTestHand_1->m_csCmdDone.Unlock();

	//g_pTestSiteInspUp->SendLogMessage("[rcv] S2F4. MSGID : %d, DataCount=%d, vReqDataCount = %d", pPacket->pHead->stData.nMsgId, pBody->stData.nDataCount,nReqCount);
	g_pVisionTestHand_1->SendLogMessage("[rcv] S2F4. MSGID : %d, DataCount=%d, vReqDataCount = %d", pPacket->pHead->stData.nMsgId, pBody->stData.nDataCount, nReqCount);
	for(int i=0; i<pBody->stData.nDataCount; i++){
		//g_pTestSiteInspUp->SendLogMessage("      Data %d = %s", i+1, pBody->stData.szDataValue[i]);
		g_pVisionTestHand_1->SendLogMessage("      Data %d = %s", i + 1, pBody->stData.szDataValue[i]);
	}

	if (nIndx != -1)
	{
		for (int i = 0; i< nReqCount; i++) {
			//g_pTestSiteInspUp->SendLogMessage("vReqData Data %d = %s", i+1, g_pTestSiteInspUp->m_VisionMsg[nIndx].szData[i]);
			g_pVisionTestHand_1->SendLogMessage("vReqData Data %d = %s", i + 1, g_pVisionTestHand_1->m_VisionMsg[nIndx].szData[i]);
		}
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
int CVisionComm_TestHand_1::OnReceiveS2F41(CPacket* pPacket)
{
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	//g_pTestSiteInspUp->SendLogMessage("[TestSiteUp rcv] S2F41. MSGID : %d, DataCount=%d", pPacket->pHead->stData.nMsgId, pBody->stData.nCmd);
	g_pVisionTestHand_1->SendLogMessage("[TestSiteUp rcv] S2F41. MSGID : %d, DataCount=%d", pPacket->pHead->stData.nMsgId, pBody->stData.nCmd);
	for(int i=0; i<pBody->stData.nParamCount; i++){
		//g_pTestSiteInspUp->SendLogMessage("      Data %d = %s", i+1, pBody->stData.szParam[i]);
		g_pVisionTestHand_1->SendLogMessage("      Data %d = %s", i + 1, pBody->stData.szParam[i]);
	}

	CPacket* pRePacket = new CPacket(2,42);
	pRePacket->pHead->stData.nMsgId = pPacket->pHead->stData.nMsgId;
	CPacketBody_S2F42* pReBody = (CPacketBody_S2F42*)pRePacket->pBody;
	pReBody->stData.nRCMDACK = 0;
	//g_pTestSiteInspUp->Send(pRePacket, FALSE);
	g_pVisionTestHand_1->Send(pRePacket, FALSE);
	//g_pTestSiteInspUp->SendLogMessage("[snd] TestSiteDown OnReceiveS2F41 Ack S2F42, MsgId : %d, RCMDACK=%d", pRePacket->pHead->stData.nMsgId, pReBody->stData.nRCMDACK);
	g_pVisionTestHand_1->SendLogMessage("[snd] TestSiteDown OnReceiveS2F41 Ack S2F42, MsgId : %d, RCMDACK=%d", pRePacket->pHead->stData.nMsgId, pReBody->stData.nRCMDACK);
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_TestHand_1::OnReceiveS2F42(CPacket* pPacket)
{
	CPacketBody_S2F42* pBody = (CPacketBody_S2F42*)pPacket->pBody;
	

	if (pPacket->pHead->stData.nMsgId == VISION_RCMD_REQ_BARCORDE_LIGHT_ON_OFF) {
		g_pVisionTestHand_1->m_csCmdDone.Lock();

		g_pVisionTestHand_1->m_BarcodeLightMsg.nRcmdAck = pBody->stData.nRCMDACK;
		g_pVisionTestHand_1->m_BarcodeLightMsg.nReceived = eVisionMsgWaitState_Received;


		g_pVisionTestHand_1->m_csCmdDone.Unlock();
		return 0;
	}


	//g_pTestSiteInspUp->m_csCmdDone.Lock();
	g_pVisionTestHand_1->m_csCmdDone.Lock();
	
	//int nWaitMsgCount = g_pTestSiteInspUp->m_VisionCmdResult.size();
	//for(int i=0; i<nWaitMsgCount; i++)
	//{
	//	_tVisionMsg* pItem = (_tVisionMsg*)g_pTestSiteInspUp->m_VisionCmdResult[i];
	//	if( pItem->nMsgId == pPacket->pHead->stData.nMsgId )
	//	{
	//		pItem->nRcmdAck = pBody->stData.nRCMDACK;
	//		pItem->nReceived = eVisionMsgWaitState_Received;
	//		break;
	//	}
	//}
	for(int i=0; i<4; i++){
		//if(g_pTestSiteInspUp->m_VisionMsg[i].nMsgId == pPacket->pHead->stData.nMsgId)
		if(g_pVisionTestHand_1->m_VisionMsg[i].nMsgId == pPacket->pHead->stData.nMsgId)
		{
			//g_pTestSiteInspUp->m_VisionMsg[i].nRcmdAck = pBody->stData.nRCMDACK;
			g_pVisionTestHand_1->m_VisionMsg[i].nRcmdAck = pBody->stData.nRCMDACK;
			
			//g_pTestSiteInspUp->m_VisionMsg[i].nReceived = eVisionMsgWaitState_Received;
			g_pVisionTestHand_1->m_VisionMsg[i].nReceived = eVisionMsgWaitState_Received;
		}
	}
	//g_pTestSiteInspUp->m_csCmdDone.Unlock();
	g_pVisionTestHand_1->m_csCmdDone.Unlock();

	//g_pTestSiteInspUp->SendLogMessage("[rcv] S2F42. MSGID : %d, RCMDACK=%d", pPacket->pHead->stData.nMsgId, pBody->stData.nRCMDACK);
	g_pVisionTestHand_1->SendLogMessage("[rcv] S2F42. MSGID : %d, RCMDACK=%d", pPacket->pHead->stData.nMsgId, pBody->stData.nRCMDACK);
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_TestHand_1::OnReceiveS5F1 (CPacket* pPacket)
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
int CVisionComm_TestHand_1::OnReceiveS5F2 (CPacket* pPacket)
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
int CVisionComm_TestHand_1::OnReceiveS6F11(CPacket* pPacket)
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
int CVisionComm_TestHand_1::OnReceiveS6F12(CPacket* pPacket)
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
int CVisionComm_TestHand_1::OnReceiveS107F1(CPacket* pPacket)
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
int CVisionComm_TestHand_1::OnReceiveS107F2(CPacket* pPacket)
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
int CVisionComm_TestHand_1::OnReceiveS107F3(CPacket* pPacket)
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
int CVisionComm_TestHand_1::OnReceiveS107F4(CPacket* pPacket)
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
int CVisionComm_TestHand_1::OnReceiveS107F5(CPacket* pPacket)
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
int CVisionComm_TestHand_1::OnReceiveS107F6(CPacket* pPacket)
{
	return 0;
}

int CVisionComm_TestHand_1::OnReceiveS107F9(CPacket* pPacket)
{
	BOOL vat_in_process = g_DMCont.m_dmVAT.GB(BDM14_VAT_TEACHING_USE);

	if (vat_in_process) {
		OnReceive_VAT(pPacket);
	}
	else { // Pcd/Backlash
		OnReceive_PcdBacklash(pPacket);
	}

	return 0;

	//기존 방식 주석  [7/22/2025 dohyeong.kim]
// 	CPacketBody_S107F9* pRcvBody = (CPacketBody_S107F9*)pPacket->pBody;
// 	int nBodySize = pRcvBody->GetSize();
// 
// 	// Rcv data logging
// 	CString strLog = _T(""), strData = _T("");
// 	int nInspSelType = 0;
// 	int nArea = VAT_AREA_TRAY_LOADPP;
// 
// 	//VAT Offset Data Recieve 변수.
// 	double dX = 0.0, dY = 0.0;
// 
// 	//Z Auto Focus 속도 제어 시, 사용.
// 	double dHandUpDN = 0.0;
// 	double dSpdFastSlow = 0.0;
// 
// 	if (pRcvBody->stData.nStatus != CAM_STATUS_SUCCESS) {
// 		// SendMessage();
// 		g_pVisionTestHand_1->SetReceiveState(_eVAT_AREA(nArea), pRcvBody->stData.nStatus);
// 
// 		ST_VAT_RCV_DATA vData;
// 		vData.nAreaData = nArea;
// 		vData.nState = pRcvBody->stData.nStatus;
// 		g_pVisionTestHand_1->SetReceiveData(vData);
// 
// 		return 0;
// 	}
// 
// 	switch (pRcvBody->stData.nDataID)
// 	{
// 	case VISION_RESULT_VAT_LD_UPPER:
// 	{
// 		nArea = VAT_AREA_TRAY_LOADPP;
// 		strLog = _T("[RCV] TRAY LOAD PP ");
// 		break;
// 	}
// 	case VISION_RESULT_VAT_ULD_UPPER:
// 	{
// 		nArea = VAT_AREA_TRAY_UNLOADPP;
// 		strLog = _T("[RCV] TRAY UNLOAD PP ");
// 		break;
// 	}
// 	case VISION_RESULT_VAT_TEST_1_LD_UPPER:
// 	{
// 		nArea = VAT_AREA_TESTPP1_LOAD;
// 		strLog = _T("[RCV] TESTPP1 LOAD ");
// 		break;
// 	}
// 	case VISION_RESULT_VAT_TEST_1_ULD_UPPER:
// 	{
// 		nArea = VAT_AREA_TESTPP1_UNLOAD;
// 		strLog = _T("[RCV] TESTPP1 UNLOAD ");
// 		break;
// 	}
// 	case VISION_RESULT_VAT_TEST_2_LD_UPPER:
// 	{
// 		nArea = VAT_AREA_TESTPP2_LOAD;
// 		strLog = _T("[RCV] TESTPP2 LOAD ");
// 		break;
// 	}
// 	case VISION_RESULT_VAT_TEST_2_ULD_UPPER:
// 	{
// 		nArea = VAT_AREA_TESTPP2_UNLOAD;
// 		strLog = _T("[RCV] TESTPP2 UNLOAD ");
// 		break;
// 	}
// 	default:
// 	{
// 		break;
// 	}
// 	}
// 
// 	// DataID check
// 	ST_VAT_RCV_DATA vData;
// 	vData.nAreaData = nArea;
// 	vData.nState = pRcvBody->stData.nStatus;
// 
// 	strLog += "MEASURE HOLE CENTER";
// 
// 	vData.dX = atof(pRcvBody->stData.szData[1])*1000.0;
// 	vData.dY = atof(pRcvBody->stData.szData[2])*1000.0;
// 
// 	strData.Format("X:%.2f um, Y:%.2f um", vData.dX, vData.dY);
// 	strLog += strData;
// 	g_pVisionTestHand_1->SendLogMessage(strLog.GetString());
// 
// 	g_pVisionTestHand_1->SetReceiveData(vData);
// 
// 	g_pVisionTestHand_1->SetReceiveState(_eVAT_AREA(nArea), CAM_STATUS_SUCCESS);
// 
// 	g_pVisionTestHand_1->SendLogMessage("[Rcv] S107F9. MSGID : %d", pPacket->pHead->stData.nMsgId);
// 	return 0;
}





void CVisionComm_TestHand_1::SetReceiveState(int nHand, int nState)
{
	m_nIsReceived[nHand] = nState;
}

int CVisionComm_TestHand_1::GetReceiveState(int nHand)
{
	if (m_bVisionComm == FALSE) {
		return CAM_STATUS_SUCCESS;
	}
	return m_nIsReceived[nHand];
}


void CVisionComm_TestHand_1::SetVisionComm(BOOL bState)
{
	m_bVisionComm = bState;
}

BOOL CVisionComm_TestHand_1::GetVisionComm()
{
	return m_bVisionComm;
}


// Pcd/Backlash
int CVisionComm_TestHand_1::SendMeasure_PcdBacklash(int nMeasureType/*=enum eInspVAT_MeasureType*/, int nWorkPos/*=enum _eVAT_AREA*/)
{
	CPacket* pPacket = new CPacket(107, 9);
	CPacketBody_S107F9* pBody = (CPacketBody_S107F9*)pPacket->pBody;

	CString strLog = _T("");

	pBody->stData.nDataID = nMeasureType;
	strLog.Format("[SND] nMeasureType = %d", nMeasureType);

	int nCamNo = 0;
	switch (nWorkPos)
	{
		case VAT_TRAY_LOAD_PP:		nCamNo = VAT_CAM_INDEX_TRAY_LOADER_UPPER; break;
		case VAT_TRAY_UNLOAD_PP:		nCamNo = VAT_CAM_INDEX_TRAY_UNLOADER_UPPER; break;
		case VAT_TEST_PP1_LOAD:	nCamNo = VAT_CAM_INDEX_TEST_1_LD_UPPER; break;
		case VAT_TEST_PP1_UNLOAD:	nCamNo = VAT_CAM_INDEX_TEST_1_ULD_UPPER; break;
		case VAT_TEST_PP2_LOAD:	nCamNo = VAT_CAM_INDEX_TEST_2_LD_UPPER; break;
		case VAT_TEST_PP2_UNLOAD:	nCamNo = VAT_CAM_INDEX_TEST_2_ULD_UPPER; break;
		default: ASSERT("nWorkPos invalid" == 0);
			return -1;
	}

	sprintf_s(pBody->stData.szData[0], sizeof(pBody->stData.szData[0]), "%d", nCamNo);
	Send(pPacket, FALSE);

	m_vRcvData[nWorkPos].nState = CAM_STATUS_IDLE;
	SendLogMessage(strLog.GetBuffer(0));

	return 0;
}


void CVisionComm_TestHand_1::OnReceive_PcdBacklash(CPacket* pPacket)
{
	CPacketBody_S107F9* pRcvBody = (CPacketBody_S107F9*)pPacket->pBody;

	// Rcv data logging
	CString strLog = _T(""), strData = _T("");
	int nHand = VAT_HAND_LOAD_TRAYPP;

	//VAT Offset Data Recieve 변수.
	double dX = 0.0, dY = 0.0;

	if (pRcvBody->stData.nStatus != CAM_STATUS_SUCCESS) {
		// SendMessage();
		g_pVisionTestHand_1->SetReceiveState(nHand, pRcvBody->stData.nStatus);

		ST_PCD_BACKLASH_RCV_DATA vData;
		vData.nHand = nHand;
		vData.nState = pRcvBody->stData.nStatus;
		g_pVisionTestHand_1->SetReceiveData(vData);

		return;
	}

	switch (pRcvBody->stData.nDataID)
	{
	case VISION_RESULT_VAT_LD_UPPER:
	{
		nHand = VAT_HAND_LOAD_TRAYPP;
		strLog = _T("[RCV] TRAY LOAD PP ");
		break;
	}
	case VISION_RESULT_VAT_ULD_UPPER:
	{
		nHand = VAT_HAND_UNLOAD_TRAYPP;
		strLog = _T("[RCV] TRAY UNLOAD PP ");
		break;
	}
	case VISION_RESULT_VAT_TEST_1_LD_UPPER:
	{
		nHand = VAT_HAND_LOAD_TESTPP1;
		strLog = _T("[RCV] TESTPP1 LOAD ");
		break;
	}
	case VISION_RESULT_VAT_TEST_1_ULD_UPPER:
	{
		nHand = VAT_HAND_UNLOAD_TESTPP1;
		strLog = _T("[RCV] TESTPP1 UNLOAD ");
		break;
	}
	case VISION_RESULT_VAT_TEST_2_LD_UPPER:
	{
		nHand = VAT_HAND_LOAD_TESTPP2;
		strLog = _T("[RCV] TESTPP2 LOAD ");
		break;
	}
	case VISION_RESULT_VAT_TEST_2_ULD_UPPER:
	{
		nHand = VAT_HAND_UNLOAD_TESTPP2;
		strLog = _T("[RCV] TESTPP2 UNLOAD ");
		break;
	}
	default:
	{
		break;
	}
	}

	// DataID check
	ST_PCD_BACKLASH_RCV_DATA vData;
	vData.nHand = nHand;
	vData.nState = pRcvBody->stData.nStatus;

	strLog += "MEASURE HOLE CENTER";

	vData.dX = atof(pRcvBody->stData.szData[1])*1000.0;
	vData.dY = atof(pRcvBody->stData.szData[2])*1000.0;

	strData.Format("X:%.2f um, Y:%.2f um", vData.dX, vData.dY);
	strLog += strData;
	g_pVisionTestHand_1->SendLogMessage(strLog.GetString());

	g_pVisionTestHand_1->SetReceiveData(vData);

	g_pVisionTestHand_1->SetReceiveState(nHand, CAM_STATUS_SUCCESS);

	g_pVisionTestHand_1->SendLogMessage("[Rcv] S107F9. MSGID : %d", pPacket->pHead->stData.nMsgId);
}



void CVisionComm_TestHand_1::SetReceiveData(ST_PCD_BACKLASH_RCV_DATA vRcvData)
{
	m_vRcvData[vRcvData.nHand] = vRcvData;
}

void CVisionComm_TestHand_1::GetReceiveData(int nAreaVAT, ST_PCD_BACKLASH_RCV_DATA &vRcvData)
{
	vRcvData = m_vRcvData[nAreaVAT];
}

int CVisionComm_TestHand_1::GetHandTypeIdx(_eTestPPIdx eTestPPIdx, eTestPPType eHandType)
{
	int nHandTypeIdx = -1;
	int Idx = (eTestPPIdx *eMaxTestPPTypeCount) + eHandType;

	switch (Idx)
	{
	case eTestPP_1_LoadType: { nHandTypeIdx = VAT_HAND_LOAD_TESTPP1; }break;
	case eTestPP_1_UnloadType: { nHandTypeIdx = VAT_HAND_UNLOAD_TESTPP1; }break;
	case eTestPP_2_LoadType: { nHandTypeIdx = VAT_HAND_LOAD_TESTPP2; }break;
	case eTestPP_2_UnloadType: { nHandTypeIdx = VAT_HAND_UNLOAD_TESTPP2; }break;
	default:
		break;
	}

	return nHandTypeIdx;
}


//=========================================================================================Vision Auto Teaching

int CVisionComm_TestHand_1::SendMeasure_VAT(CPacket* pPacket)
{
	//Hand 연동 측정을 위해, 임계 영역 설정.
	m_csSend.Lock();

	Send(pPacket, FALSE);

	m_csSend.Unlock();

	return 0;
}






void CVisionComm_TestHand_1::OnReceive_VAT(CPacket* pPacket)
{
	//Data Receive 임계 영역 설정.
	g_pVisionTestHand_1->m_csRcv.Lock();

	CPacketBody_S107F9* pRcvBody = (CPacketBody_S107F9*)pPacket->pBody;
	
	int nCam = -1;
	switch (pRcvBody->stData.nDataID)
	{
		case VISION_RCV_DATAID_VAT_LD_UPPER:			nCam = VAT_CAM_LD_TRAY_PP_UPPER; break;
		case VISION_RCV_DATAID_VAT_LD_LOWER:			nCam = VAT_CAM_LD_TRAY_PP_LOWER; break;
		case VISION_RCV_DATAID_VAT_ULD_UPPER:			nCam = VAT_CAM_UD_TRAY_PP_UPPER; break;
		case VISION_RCV_DATAID_VAT_ULD_LOWER:			nCam = VAT_CAM_UD_TRAY_PP_LOWER; break;
		case VISION_RCV_DATAID_VAT_TEST_1_LD_UPPER:		nCam = VAT_CAM_LD_TEST_PP_1_UPPER; break;
		case VISION_RCV_DATAID_VAT_TEST_1_ULD_UPPER:	nCam = VAT_CAM_UD_TEST_PP_1_UPPER; break;
		case VISION_RCV_DATAID_VAT_TEST_1_LOWER: 		nCam = VAT_CAM_TEST_PP_1_LOWER; break;
		default:
			break;
	}

	if (!(VAT_CAM_LD_TRAY_PP_UPPER <= nCam && nCam <= VAT_CAM_TEST_PP_1_LOWER)) {
		g_pVisionTestHand_1->SendLogMessage("[Rcv ERROR] [Data ID Error] [CAM : %d]", nCam);
		g_pVisionTestHand_1->m_csRcv.Unlock();
		return;
	}

	g_pVisionTestHand_1->m_csRcv.Unlock();

	//Data 해당 Cam 객체에서 독립적으로 관리.
	g_VatVisionMgr[nCam].OnReceive(pPacket);
}