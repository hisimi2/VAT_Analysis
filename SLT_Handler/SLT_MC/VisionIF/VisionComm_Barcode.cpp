#include "StdAfx.h"
#include "VisionComm_Barcode.h"
#include "ConfigData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CVisionComm_Barcode::CVisionComm_Barcode(int nVisionPcIdx, CString strVisionName, int nCamCompanyType)
	:CVisionComm(nVisionPcIdx, strVisionName, nCamCompanyType)
{
	m_pDualSock1 = NULL;
	m_pDualSock2 = NULL;
	m_pDualSock3 = NULL;
	m_pDualSock4 = NULL;
	m_nRecvLen_L =0;
	m_nRecvLen_R =0;
}

CVisionComm_Barcode::CVisionComm_Barcode(void)
{
	m_pDualSock1 = NULL;
	m_pDualSock2 = NULL;
	m_pDualSock3 = NULL;
	m_pDualSock4 = NULL;
	m_nRecvLen_L = 0;
	m_nRecvLen_R = 0;
}


CVisionComm_Barcode::~CVisionComm_Barcode(void)
{	
}

ST_MESSAGE_MAP CVisionComm_Barcode::MessageMap[] ={
	{2,3,  CVisionComm_Barcode::OnReceiveS2F3    },
	{2,4,  CVisionComm_Barcode::OnReceiveS2F4    },
	{2,41,  CVisionComm_Barcode::OnReceiveS2F41  },
	{2,42,  CVisionComm_Barcode::OnReceiveS2F42  },
	{5, 1,  CVisionComm_Barcode::OnReceiveS5F1   },
	{5, 2,  CVisionComm_Barcode::OnReceiveS5F2   },
	{6,11,  CVisionComm_Barcode::OnReceiveS6F11  },
	{6,12,  CVisionComm_Barcode::OnReceiveS6F12  },
	{107,1, CVisionComm_Barcode::OnReceiveS107F1 },
	{107,2, CVisionComm_Barcode::OnReceiveS107F2 },
	{107,3, CVisionComm_Barcode::OnReceiveS107F3 },
	{107,4, CVisionComm_Barcode::OnReceiveS107F4 },
	{107,5, CVisionComm_Barcode::OnReceiveS107F5 },
	{107,6, CVisionComm_Barcode::OnReceiveS107F6 },
	{-1,-1, NULL},
};


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_Barcode::Initialize(CWnd* pParentWnd, char* szDriverFileName)
{
	//if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
	//	return 0;

	char szIp[64] = {0,};
	int nPort = 0;

	char szKey[128]     = {0,};
	char szDefault[128] = {0,};
	char szBuff[512]    = {0,};
	char szLogFileName[512] = { 0, };

	strcpy_s(m_szDriverFileName, sizeof(m_szDriverFileName), szDriverFileName);
	CConfigData ConfigData(szDriverFileName);

	int nSocketType   = ConfigData.GetInt("MC TO VISION COMM CONFIG", "Socket Type", "0");

	memset(szDefault, 0x00, sizeof(szDefault));
	sprintf_s(szDefault, sizeof(szDefault), "VisionComm_2DBarcode.log");
	ConfigData.GetString("MC TO VISION COMM CONFIG", "2DBarcode Log File Name", szDefault, szBuff);
	sprintf_s(szLogFileName, sizeof(szLogFileName), "%s%s", SZ_LOG_FILE_PATH, szBuff);
	m_Logger.SetFileName(szLogFileName);
	sprintf_s(szLogFileName, sizeof(szLogFileName), "%sBarcode%s", SZ_LOG_FILE_PATH, szBuff);

	int	nRet = 0;

	if(m_nCamCompanyType == e2dVision_TYPE_TW){
		ZeroMemory(szKey, sizeof(szKey));
		sprintf_s(szKey, sizeof(szKey), "%s IP", m_strVisionPcName.GetString() );
		ConfigData.GetString("MC TO VISION COMM CONFIG", szKey, "127.0.0.1", szIp);

		ZeroMemory(szKey, sizeof(szKey));
		sprintf_s(szKey, sizeof(szKey), "%s Port", m_strVisionPcName.GetString() );

		ZeroMemory(szDefault, sizeof(szDefault));
		sprintf_s(szDefault, sizeof(szDefault), "%d", 8000+m_nVisionPcIdx);

		nPort = ConfigData.GetInt("MC TO VISION COMM CONFIG", szKey, szDefault);

		nRet = CIPCSock::Initialize(pParentWnd, nSocketType, szIp, nPort, CVisionComm_Barcode::MessageMap, szLogFileName);
		SendLogMessage("IPC Vision 2DBarcode Comm Initialized. [ERR=%d]", nRet);
		if( nRet != 0 ){
			AfxMessageBox("can not init vision socket!!!");
			return -1;
		}
		nRet = CIPCSock::SocketConnect(nSocketType, szIp, nPort);
		SendLogMessage("IPC Vision 2DBarcode Comm connect. [%s, IP:%s, PORT:%d] [ERR=%d]", (nSocketType==eSocketTypeClient)?"Client":"Server", szIp, nPort, nRet);
		if( nRet != 0 ){
			AfxMessageBox("can not connect vision socket!!!");
			return -1;
		}	
	}else if( m_nCamCompanyType == e2dVision_TYPE_Keyence ){

		nSocketType   = ConfigData.GetInt("MC TO VISION COMM CONFIG", "KEYENCE_2D_SOCKET_TYPE", "0");

		m_pDualSock1 = new CDualTCP_Sock;
		m_pDualSock2 = new CDualTCP_Sock;
		m_pDualSock3 = new CDualTCP_Sock;
		m_pDualSock4 = new CDualTCP_Sock;

		//AfxMessageBox("dd");

   		ConfigData.GetString("MC TO VISION COMM CONFIG", "KEYENCE_2DIP_LEFT_REAR", "100.100.100.111", szIp);
   		int nPort = ConfigData.GetInt("MC TO VISION COMM CONFIG", "KEYENCE_2DPORT_LEFT_REAR" , "8050");
		m_strLeft2dIp[0] = szIp;
   		if(! m_pDualSock1->Initialize_Port( szIp, nPort, nSocketType ) ) {
   			AfxMessageBox("vision barcode table Cam1 Connection Fail !!\n");
   			return -1;
   		}
 
  		ConfigData.GetString("MC TO VISION COMM CONFIG", "KEYENCE_2DIP_LEFT_FRONT", "100.100.100.112", szIp);		
  		nPort = ConfigData.GetInt("MC TO VISION COMM CONFIG", "KEYENCE_2DPORT_LEFT_FRONT"  , "8050");
		m_strLeft2dIp[1] = szIp;
  		if(! m_pDualSock2->Initialize_Port( szIp, nPort, nSocketType ) ) {
  			AfxMessageBox("vision barcode table Cam2 Connection Fail !!\n");
  			return -1;
  		}

 		ConfigData.GetString("MC TO VISION COMM CONFIG", "KEYENCE_2DIP_RIGHT_REAR", "100.100.100.113", szIp);
 		nPort = ConfigData.GetInt("MC TO VISION COMM CONFIG", "KEYENCE_2DPORT_RIGHT_REAR", "8050");
		m_strRight2dIp[0] = szIp;
 		if(! m_pDualSock3->Initialize_Port( szIp, nPort, nSocketType ) ) {
 			AfxMessageBox("vision barcode table Cam3 Connection Fail !!\n");
 			return -1;
 		}
 
		ConfigData.GetString("MC TO VISION COMM CONFIG", "KEYENCE_2DIP_RIGHT_FRONT", "100.100.100.114", szIp);
		nPort = ConfigData.GetInt("MC TO VISION COMM CONFIG", "KEYENCE_2DPORT_RIGHT_FRONT", "8050");
		m_strRight2dIp[1] = szIp;
		if (!m_pDualSock4->Initialize_Port(szIp, nPort, nSocketType)) {
			AfxMessageBox("vision barcode table Cam4 Connection Fail !!\n");
			return -1;
  		}
	}
	
	return nRet;
}

void CVisionComm_Barcode::Finalize()
{
	//if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
	//	return;

	SendLogMessage("IPC Vision 2DBarcode Comm Finalized.");
	if(m_nCamCompanyType == e2dVision_TYPE_TW){
		CIPCSock::Finalize();
	}else{
		m_pDualSock1->Finalize_DoublePort();
		Sleep(1000);
		delete m_pDualSock1;
		m_pDualSock1 = NULL;	

		m_pDualSock2->Finalize_DoublePort();
		Sleep(1000);
		delete m_pDualSock2;
		m_pDualSock2 = NULL;	

		m_pDualSock3->Finalize_DoublePort();
		Sleep(1000);
		delete m_pDualSock3;
		m_pDualSock3 = NULL;

		m_pDualSock4->Finalize_DoublePort();
		Sleep(1000);
		delete m_pDualSock4;
		m_pDualSock4 = NULL;		
	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-11-04]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_Barcode::OnReceiveS2F3 (CPacket* pPacket)
{
	g_pBarcode->SendLogMessage("[rcv] S2F3. MSGID : %d", pPacket->pHead->stData.nMsgId);
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-11-04]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS :
			  typedef struct _ST_PACKET_BODY_S2F4
			  {
				int    nDataCount;
				char   szDataValue[10][64];
			  }ST_PACKET_BODY_S2F4;
==============================================================================*/
int CVisionComm_Barcode::OnReceiveS2F4 (CPacket* pPacket)
{
	CPacketBody_S2F4* pBody = (CPacketBody_S2F4*)(pPacket->pBody);
	
	g_pBarcode->m_csCmdDone.Lock();

	int nWaitMsgCount = g_pBarcode->m_VisionCmdResult.size();
	for(int i=0; i<nWaitMsgCount; i++)
	{
		_tVisionMsg* pItem = (_tVisionMsg*)g_pBarcode->m_VisionCmdResult[i];
		if( pItem->nMsgId == pPacket->pHead->stData.nMsgId )
		{
			for(int j=0; j<(int)pItem->vReqData.size(); j++)
			{
				strcpy_s(pItem->vReqData[j].szDataValue, sizeof(pItem->vReqData[j].szDataValue), pBody->stData.szDataValue[j]);
			}
			pItem->nReceived = eVisionMsgWaitState_Received;
			break;
		}
	}
	g_pBarcode->m_csCmdDone.Unlock();

	g_pBarcode->SendLogMessage("[rcv] S2F4. MSGID : %d, DataCount=%d", pPacket->pHead->stData.nMsgId, pBody->stData.nDataCount);
	for(int i=0; i<pBody->stData.nDataCount; i++){
		g_pBarcode->SendLogMessage("      Data %d = %s", i+1, pBody->stData.szDataValue[i]);
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
int CVisionComm_Barcode::OnReceiveS2F41(CPacket* pPacket)
{
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	g_pBarcode->SendLogMessage("[rcv] S2F41. MSGID : %d, CMD = %d, Count=%d", pPacket->pHead->stData.nMsgId, pBody->stData.nCmd, pBody->stData.nParamCount);
	for(int i=0; i<pBody->stData.nParamCount; i++){
		g_pBarcode->SendLogMessage("      Data %d = %s", i+1, pBody->stData.szParam[i]);
	}

	int nAck = acknowledge;
    
	if( g_START_SW.GetStatus() != FALSE || eEqpCtrlMode_Manual != CBaseTaskManager::GetEqpCtrlMode())
	{
		// 메뉴얼 모드에서만 shuttle 이동 가능.
		nAck = can_not_perform_now;
	}
	else
	{
		BOOL bVisionUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use);
		if( bVisionUseMode != TRUE )
		{
			nAck = can_not_perform_now;
		}
		else
		{
			if( pBody->stData.nCmd != VISION_RCMD_REQ_SHUTTLE_MOVE)	
			{
				nAck = cmd_does_not_exist;
			}
			else
			{
				if( pBody->stData.nParamCount != 1 )	
				{
					nAck = parameter_is_invalid;
				}
				else
				{
					int nLoadTblNum = atoi(pBody->stData.szParam[0]);
					if( nLoadTblNum < eLoadTbl_1 || nLoadTblNum >= eMaxLoadTblCount )
					{
						nAck = parameter_is_invalid;
					}
				}
			}
		}
	}

	CPacket* pReplyPacket = new CPacket(2,42);
	pReplyPacket->pHead->stData.nMsgId = pPacket->pHead->stData.nMsgId;
	CPacketBody_S2F42* pReplyBody = (CPacketBody_S2F42*)pReplyPacket->pBody;
	pReplyBody->stData.nRCMDACK = nAck;
	g_pBarcode->Send( pReplyPacket, FALSE);
	g_pBarcode->SendLogMessage("[snd] S2F42, RCMDACK=%d", nAck);

	if( nAck == acknowledge )
	{
// 		int nLoadTblIdx = atoi(pBody->stData.szParam[0]);
// 		CLoadTblCmd_VisionTeach* pSeqCmd = new CLoadTblCmd_VisionTeach;
// 		int nErr = g_TaskSystemCtrl.SendCommand(g_TaskLoadTbl[nLoadTblIdx], pSeqCmd, eEqpCtrlMode_Manual);
// 		if( nErr != ERR_NO_ERROR )
// 		{
// 			CParamList AlarmData;
// 			AlarmData.m_fnAddItemFormat("Load Table Number = %d", nLoadTblIdx+1);
// 			g_Error.AlarmReport(nErr, AlarmData, NULL);
// 		}
	}
	
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
int CVisionComm_Barcode::OnReceiveS2F42(CPacket* pPacket)
{
	CPacketBody_S2F42* pBody = (CPacketBody_S2F42*)pPacket->pBody;

	g_pBarcode->m_csCmdDone.Lock();

	int nWaitMsgCount = g_pBarcode->m_VisionCmdResult.size();
	for(int i=0; i<nWaitMsgCount; i++)
	{
		_tVisionMsg* pItem = (_tVisionMsg*)g_pBarcode->m_VisionCmdResult[i];
		if( pItem->nMsgId == pPacket->pHead->stData.nMsgId )
		{
			pItem->nRcmdAck = pBody->stData.nRCMDACK;
			pItem->nReceived = eVisionMsgWaitState_Received;
			break;
		}
	}

	g_pBarcode->m_csCmdDone.Unlock();

	g_pBarcode->SendLogMessage("[rcv] S2F42. MSGID : %d, RCMDACK=%d", pPacket->pHead->stData.nMsgId, pBody->stData.nRCMDACK);
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_Barcode::OnReceiveS5F1 (CPacket* pPacket)
{
	g_pBarcode->SendLogMessage("[rcv] S5F1. MSGID : %d", pPacket->pHead->stData.nMsgId);
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_Barcode::OnReceiveS5F2 (CPacket* pPacket)
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
int CVisionComm_Barcode::OnReceiveS6F11(CPacket* pPacket)
{
	g_pBarcode->SendLogMessage("[rcv] S6F11. MSGID : %d", pPacket->pHead->stData.nMsgId);
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_Barcode::OnReceiveS6F12(CPacket* pPacket)
{
	g_pBarcode->SendLogMessage("[rcv] S6F12. MSGID : %d", pPacket->pHead->stData.nMsgId);
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_Barcode::OnReceiveS107F1(CPacket* pPacket)
{
	g_pBarcode->SendLogMessage("[rcv] S107F1. MSGID : %d", pPacket->pHead->stData.nMsgId);
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_Barcode::OnReceiveS107F2(CPacket* pPacket)
{
	g_pBarcode->SendLogMessage("[rcv] S107F2. MSGID : %d", pPacket->pHead->stData.nMsgId);
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_Barcode::OnReceiveS107F3(CPacket* pPacket)
{
	g_pBarcode->SendLogMessage("[rcv] S107F3. MSGID : %d", pPacket->pHead->stData.nMsgId);
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_Barcode::OnReceiveS107F4(CPacket* pPacket)
{
	g_pBarcode->SendLogMessage("[rcv] S107F4. MSGID : %d", pPacket->pHead->stData.nMsgId);
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_Barcode::OnReceiveS107F5(CPacket* pPacket)
{
	g_pBarcode->SendLogMessage("[rcv] S107F5. MSGID : %d", pPacket->pHead->stData.nMsgId);
	return 0;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_Barcode::OnReceiveS107F6(CPacket* pPacket)
{
	g_pBarcode->SendLogMessage("[rcv] S107F6. MSGID : %d", pPacket->pHead->stData.nMsgId);
	return 0;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
LRESULT CVisionComm_Barcode::OnTWIPCEvent(WPARAM wParam, LPARAM lParam)
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
			TRACE("BarCode Vision Connected.\n");
		}break;
	case IPC_EVT_DISCONNECTED:
		{
			TRACE("BarCode Vision Disconnected.\n");
			g_pBarcode->m_csCmdDone.Lock();
			m_VisionCmdResult.clear();
			int nWaitMsgCount = m_VisionCmdResult.size();
			for(int i=0; i<nWaitMsgCount; i++)
			{
				_tVisionMsg* pItem = (_tVisionMsg*)m_VisionCmdResult[i];
				pItem->nReceived = eVisionMsgWaitState_Canceled;
			}
			g_pBarcode->m_csCmdDone.Unlock();
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


/*=============================================================================
  AUTHOR   : 나명진  [2015-11-04]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_Barcode::Cmd_ReqMeasureResult(int nCamNo,_tVisionMsg* pVisionMsg/*=NULL*/, int nDirPocket/*enum e2dPocketDirection*/)
{
	if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
		char szResultData[128] = { "2D_READ_ERROR" };
		strncpy_s(pVisionMsg->vReqData[nDirPocket%2  + (nCamNo % 2) * 2 + (nDirPocket / 2) * 4].szDataValue, szResultData, 13);
		return ERR_NO_ERROR;
	}
		

	int nErr = 0;
	if( m_nCamCompanyType == e2dVision_TYPE_TW){
		// barcode의 경우 camera 1ea당 4개의 결과를 가진다.
		int nDataId[4][5] = 
		{
			{
				VISION_DATAID_CAM_BUSY_LOAD_TBL1,
					VISION_DATAID_RESULT_DATA_CAM1_BAR_CODE_X1Y1, 
					VISION_DATAID_RESULT_DATA_CAM1_BAR_CODE_X2Y1, 
					VISION_DATAID_RESULT_DATA_CAM1_BAR_CODE_X1Y2, 
					VISION_DATAID_RESULT_DATA_CAM1_BAR_CODE_X2Y2
			},
			{
				VISION_DATAID_CAM_BUSY_LOAD_TBL2,
					VISION_DATAID_RESULT_DATA_CAM2_BAR_CODE_X1Y1, 
					VISION_DATAID_RESULT_DATA_CAM2_BAR_CODE_X2Y1, 
					VISION_DATAID_RESULT_DATA_CAM2_BAR_CODE_X1Y2, 
					VISION_DATAID_RESULT_DATA_CAM2_BAR_CODE_X2Y2
				},
				{
					VISION_DATAID_CAM_BUSY_LOAD_TBL3,
						VISION_DATAID_RESULT_DATA_CAM3_BAR_CODE_X1Y1, 
						VISION_DATAID_RESULT_DATA_CAM3_BAR_CODE_X2Y1, 
						VISION_DATAID_RESULT_DATA_CAM3_BAR_CODE_X1Y2, 
						VISION_DATAID_RESULT_DATA_CAM3_BAR_CODE_X2Y2
				},
				{
					VISION_DATAID_CAM_BUSY_LOAD_TBL4,
						VISION_DATAID_RESULT_DATA_CAM4_BAR_CODE_X1Y1, 
						VISION_DATAID_RESULT_DATA_CAM4_BAR_CODE_X2Y1, 
						VISION_DATAID_RESULT_DATA_CAM4_BAR_CODE_X1Y2, 
						VISION_DATAID_RESULT_DATA_CAM4_BAR_CODE_X2Y2
					}
		};

		int nLoadTblIdx = nCamNo - VISION_CAM_ID_2DBRACODE_1;

		CPacket* pPacket = new CPacket(2,3);
		CPacketBody_S2F3* pBody = (CPacketBody_S2F3*)pPacket->pBody;
		pBody->stData.nDataCount = 5;
		sprintf_s(pBody->stData.szDataName[0], sizeof(pBody->stData.szDataName[0]), "%d", nDataId[nLoadTblIdx][0]);
		sprintf_s(pBody->stData.szDataName[1], sizeof(pBody->stData.szDataName[1]), "%d", nDataId[nLoadTblIdx][1]);
		sprintf_s(pBody->stData.szDataName[2], sizeof(pBody->stData.szDataName[2]), "%d", nDataId[nLoadTblIdx][2]);
		sprintf_s(pBody->stData.szDataName[3], sizeof(pBody->stData.szDataName[3]), "%d", nDataId[nLoadTblIdx][3]);
		sprintf_s(pBody->stData.szDataName[4], sizeof(pBody->stData.szDataName[4]), "%d", nDataId[nLoadTblIdx][4]);

		nErr = CIPCSock::Send(pPacket, FALSE);
		if( nErr == 0 )
		{
			if( pVisionMsg != NULL )
			{
				_tVisionMsg* pWaitVisionMsg = new _tVisionMsg;
				pWaitVisionMsg->nMsgId = pPacket->pHead->stData.nMsgId;
				pWaitVisionMsg->nS = pPacket->pHead->stData.nS;
				pWaitVisionMsg->nF = pPacket->pHead->stData.nF;
				for(int i=0; i<5; i++){
					_tVisionReqData reqData;
					reqData.nDataId = nDataId[nLoadTblIdx][i];
					pWaitVisionMsg->vReqData.push_back(reqData);
				}
				pWaitVisionMsg->nReceived = eVisionMsgWaitState_Waiting;
				*pVisionMsg = *pWaitVisionMsg;

				g_pBarcode->m_csCmdDone.Lock();
				m_VisionCmdResult.push_back(pWaitVisionMsg);
				g_pBarcode->m_csCmdDone.Unlock();
			}
			nErr = ERR_NO_ERROR;
			SendLogMessage("[snd] S2F3");
			for(int i=0; i<pBody->stData.nDataCount; i++)
			{
				SendLogMessage("DATAID%d : %s", i+1, pBody->stData.szDataName[i]);
			}
		}
		else
		{
			if( nErr == -1 ){
				nErr = ERR_VISION_SOCK_SEND_FAIL_INVALID_PACKET;
			}else if( nErr == -2 ){
				nErr = ERR_VISION_SOCK_SEND_FAIL_NOT_ESTABLISHED;
			}else if( nErr == -3 ){
				nErr = ERR_VISION_SOCK_SEND_FAIL_NOT_CONNECT;
			}
		}

		delete pPacket;
		pPacket = NULL;
	}
	else if(m_nCamCompanyType == e2dVision_TYPE_Keyence){
		char szTerm[2] = { CR, 0 };
		int nTimeOut = (int)(VISION_TIME_OUT*1000);

		char szResult[128] = { 0, };
		int nErr = -1,nLen = 0, nDataPos=-1;
		switch (nCamNo)
		{
		case VISION_CAM_ID_2DBRACODE_1:
			{
				nErr = m_pDualSock1->Cmd_RecvStr_Port1(szResult, szTerm, 128, nTimeOut, &nLen);
			}break;
		case VISION_CAM_ID_2DBRACODE_2:
			{
				nErr = m_pDualSock2->Cmd_RecvStr_Port1(szResult, szTerm, 128, nTimeOut, &nLen);
			}break;
		case VISION_CAM_ID_2DBRACODE_3:
			{
				nErr = m_pDualSock3->Cmd_RecvStr_Port1(szResult, szTerm, 128, nTimeOut, &nLen);
			}break;
		case VISION_CAM_ID_2DBRACODE_4:
			{
				nErr = m_pDualSock4->Cmd_RecvStr_Port1(szResult, szTerm, 128, nTimeOut, &nLen);
			}break;
		default:
			break;
		}

		if (nErr == -1) {
			return ERR_VISION_SOCK_SEND_FAIL_NOT_CONNECT;
		}
		if (nErr == -2) {
			return ERR_VISION_CMD_RESP_DATA_IS_WRONG;
		}
		if (nErr == -4) {
			return ERR_LOAD_TBL_1_VISION_CMD_RESP_TIME_OUT_SKIP + (nCamNo >= VISION_CAM_ID_2DBRACODE_3 ? 1 : 0);
		}

		strncpy_s(pVisionMsg->vReqData[nDirPocket % 2 + (nCamNo % 2) * 2 + (nDirPocket / 2) * 4].szDataValue, szResult, nLen);
	}

	return nErr;
}

int CVisionComm_Barcode::Cmd_Req2DMeasureResult(int nCamNo, int nTblIdx, TokenArray* str2DErrorData, _tVisionMsg* pVisionMsg /*= NULL*/)
{
	if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_) {
		//Simulation 2D Test
		TokenArray strTokens;
		if (nCamNo == 2) {
			strTokens.push_back("AK5GW0000J0A4X00ZA");
			strTokens.push_back("AK5GW0000J0A3X00ZA");
			strTokens.push_back("AK5GW0000J0A2X00ZA");
			strTokens.push_back("AK5GW0000J0A1X00Z9");
		}
		else {
			strTokens.push_back("AK5GW0000J0A8X00Z9");
			strTokens.push_back("AK5GW0000J0A7X00Z9");
			strTokens.push_back("AK5GW0000J0A6X00Z8");
			strTokens.push_back("AK5GW0000J0A5X00Z0");
		}

		int nPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);
		int nPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
		int nPocketNum = 0;

		ST_DD_DEVICE tDeviceData;
		tDeviceData.clear();

		for (int i = 0; i < nPocketCntX; i++) {

			int nPocketX = i;
			int nPocketY = 0;


			ChangePocketNum(nCamNo, nPocketX, nPocketY, nPocketNum);

			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1 + nTblIdx, nPocketX, nPocketY, &tDeviceData);

			//////str token 개수가 4개 이하 일때, Device가 없으면 Data가 안들어온것.
			if (tDeviceData.sExist == eDeviceData_Exist || g_TaskLoadTbl[nTblIdx].GetEqpCtrlMode() == eEqpCtrlMode_Manual)
			{
				strncpy_s(pVisionMsg->vReqData[nPocketNum].szDataValue, strTokens[i], strlen(strTokens[i]));
			}
		}


		//2D 수신 Error
		//char szResultData[128] = { "2D_READ_ERROR" };

		//*str2DErrorData = strTokens;
		//strncpy_s(pVisionMsg->vReqData[nDirPocket % 2 + (nCamNo % 2) * 2 + (nDirPocket / 2) * 4].szDataValue, szResultData, 13);
		return ERR_NO_ERROR;
	}

	int nErr = 0;
	if (m_nCamCompanyType == e2dVision_TYPE_Keyence) {
		char szTerm[2] = { CR, 0 };

		TokenArray strTokens;
		int nTimeOut = (int)(VISION_TIME_OUT * 1000);

		char szResult[128] = { 0, };
		int nErr = -1, nLen = 0;
		switch (nCamNo)
		{
		case VISION_CAM_ID_2DBRACODE_1:
		{
			//nErr = m_pDualSock1->Cmd_RecvStr_Port1(szResult, szTerm, 128, nTimeOut, &nLen); //4
			nErr = m_pDualSock1->Cmd_2DRecvStr_Port1(&strTokens, nTblIdx, nTimeOut, 128); //4
		}break;
		case VISION_CAM_ID_2DBRACODE_2:
		{
			//nErr = m_pDualSock2->Cmd_RecvStr_Port1(szResult, szTerm, 128, nTimeOut, &nLen); //4
			nErr = m_pDualSock2->Cmd_2DRecvStr_Port1(&strTokens, nTblIdx, nTimeOut, 128); //4
		}break;
		case VISION_CAM_ID_2DBRACODE_3:
		{
			//nErr = m_pDualSock3->Cmd_RecvStr_Port1(szResult, szTerm, 128, nTimeOut, &nLen);
			nErr = m_pDualSock3->Cmd_2DRecvStr_Port1(&strTokens, nTblIdx, nTimeOut, 128);
		}break;
		case VISION_CAM_ID_2DBRACODE_4:
		{
			//nErr = m_pDualSock4->Cmd_RecvStr_Port1(szResult, szTerm, 128, nTimeOut, &nLen);
			nErr = m_pDualSock4->Cmd_2DRecvStr_Port1(&strTokens, nTblIdx, nTimeOut, 128);
		}break;
		default:
			break;
		}

		//strncpy_s(pVisionMsg->vReqData[nDirPocket % 2 + (nCamNo % 2) * 2 + (nDirPocket / 2) * 4].szDataValue, szResult, nLen);
		
		//Device 존재, 
		//Error 아니면  data 저장.
		ST_DD_DEVICE tDeviceData;
		tDeviceData.clear();

		int nPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);
		int nPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
		int nPocketNum = 0;

		//Error 처리
		if (nErr != ERR_NO_ERROR) {
			//Alarm Complete Skip을 위한 Error Pocket 저장
// 			CPoint pocket;
// 			for (int i = 0; i < nPocketCntX; i++)
// 			{
// 				int nPocketX = i;
// 				int nPocketY = 0;
// 				ChangePocketNum(nCamNo, nPocketX, nPocketY, nPocketNum); // [PKG]옵션 처리.
// 
// 				pocket.y = nPocketY;
// 				pocket.x = nPocketX;
// 				v2DErrorData->push_back(pocket);
//   			}

			//Barcode Error Data Log 저장을 위함.
			*str2DErrorData = strTokens;

 			
// 			if (nErr == -1) {
// 				nErrRet = ERR_VISION_SOCK_SEND_FAIL_NOT_CONNECT;
// 			}
			int nErrRet = 0;
			if (nErr == -2) {
				//Data 4개 안들어옴. //수신 데어터 갯수가 부족합니다.
				nErrRet = ERR_VISION_CMD_RESP_DATA_IS_WRONG;		
			}
			else if (nErr == -4) {
				//2D Barcode 응답이 시간내에 데이터 수신이 되지 않았습니다.
				nErrRet = ERR_LOAD_TBL_2_VISION_CMD_RESP_TIME_OUT_SKIP + (nCamNo >= VISION_CAM_ID_2DBRACODE_3 ? 1 : 0);
			}
			else { //nErr == -1
				nErrRet = ERR_VISION_SOCK_SEND_FAIL_NOT_CONNECT;
			}
			return nErrRet;

// 			if (nErrRet != 0) {
// 				return nErrRet;
// 			}
		}

		for (int i = 0; i < nPocketCntX; i++) {

			int nPocketX = i; 
			int nPocketY = 0;


			ChangePocketNum(nCamNo, nPocketX, nPocketY, nPocketNum);

			g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1 + nTblIdx, nPocketX, nPocketY, &tDeviceData);

			//////str token 개수가 4개 이하 일때, Device가 없으면 Data가 안들어온것.
			if (tDeviceData.sExist == eDeviceData_Exist || g_TaskLoadTbl[nTblIdx].GetEqpCtrlMode() == eEqpCtrlMode_Manual)
			{
				strncpy_s(pVisionMsg->vReqData[nPocketNum].szDataValue, strTokens[i], strlen(strTokens[i]));
			}
		}
	}

	return nErr;
}


int CVisionComm_Barcode::Cmd_ShuttleMoveComplte(int nLoadTblIdx, _tVisionMsg* pVisionMsg/*=NULL*/)
{
	int nErr = 0;
	if( m_nCamCompanyType == e2dVision_TYPE_TW){
		CPacket* pPacket = new CPacket(2, 41);
		CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
		pBody->stData.nCmd = VISION_RCMD_RPT_SHUTTLE_MOVE_COMP;
		pBody->stData.nParamCount = 1;

		sprintf_s(pBody->stData.szParam[0], "%d", nLoadTblIdx);

		nErr = CIPCSock::Send(pPacket, FALSE);
		if( nErr == 0 )
		{
			if( pVisionMsg != NULL )
			{
				_tVisionMsg* pWaitVisionMsg = new _tVisionMsg;
				pWaitVisionMsg->nMsgId = pPacket->pHead->stData.nMsgId;
				pWaitVisionMsg->nS = pPacket->pHead->stData.nS;
				pWaitVisionMsg->nF = pPacket->pHead->stData.nF;
				pWaitVisionMsg->nRcmd = pBody->stData.nCmd;
				pWaitVisionMsg->nReceived = eVisionMsgWaitState_Waiting;
				*pVisionMsg = *pWaitVisionMsg;
				g_pBarcode->m_csCmdDone.Lock();
				m_VisionCmdResult.push_back(pWaitVisionMsg);
				g_pBarcode->m_csCmdDone.Unlock();
			}
			nErr = ERR_NO_ERROR;
			SendLogMessage("[snd] S2F41, CMD : %d", pBody->stData.nCmd);
			for(int i=0; i<pBody->stData.nParamCount; i++)
			{
				SendLogMessage("[snd] Param%d : %s", i+1, pBody->stData.szParam[i]);
			}
		}
		else
		{
			if( nErr == -1){
				nErr = ERR_VISION_SOCK_SEND_FAIL_INVALID_PACKET;
			}else if( nErr == -2 ){
				nErr = ERR_VISION_SOCK_SEND_FAIL_NOT_ESTABLISHED;
			}else if( nErr == -3 ){
				nErr = ERR_VISION_SOCK_SEND_FAIL_NOT_CONNECT;
			}
		}

		delete pPacket;
		pPacket = NULL;
	}
	

	return nErr;
}

/*=============================================================================
  AUTHOR   : 나명진  [2015-11-04]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm_Barcode::Cmd_SetCok(_tVisionMsg* pVisionMsg)
{
	if( m_nCamCompanyType == e2dVision_TYPE_TW){
		return CVisionComm::Cmd_SetCok(pVisionMsg);
	}

	return 0;
}
int CVisionComm_Barcode::Cmd_ReqCleanDisk(_tVisionMsg* pVisionMsg)
{
	if( m_nCamCompanyType == e2dVision_TYPE_TW){
		return CVisionComm::Cmd_ReqCleanDisk(pVisionMsg);
	}
	return 0;
}
int CVisionComm_Barcode::Cmd_ReqVisionShutDown(_tVisionMsg* pVisionMsg)
{
	if( m_nCamCompanyType == e2dVision_TYPE_TW){
		return CVisionComm::Cmd_ReqVisionShutDown(pVisionMsg);
	}
	return 0;
}
int CVisionComm_Barcode::Cmd_ReqInspReady(int nCamNo,_tVisionMsg* pVisionMsg)
{
	if( m_nCamCompanyType == e2dVision_TYPE_TW){
		return CVisionComm::Cmd_ReqInspReady(nCamNo, pVisionMsg);
	}
	return 0;
}

int CVisionComm_Barcode::Cmd_Req2DMeasure(int nCamNo/*, _tVisionMsg* pVisionMsg, int nDirPocket*/)
{
	if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
		return ERR_NO_ERROR;
	
	if (m_nCamCompanyType == e2dVision_TYPE_Keyence)
	{
		char szPacket[255] = { 0, };
		ZeroMemory(szPacket, sizeof(szPacket));
		BOOL bErrWrt = FALSE;

  		sprintf_s(szPacket, "abcd\r");
		switch (nCamNo)
		{
		case VISION_CAM_ID_2DBRACODE_1:
		{
			bErrWrt = m_pDualSock1->Cmd_WrtStr_Port1(szPacket, strlen(szPacket));
		}break;
		case VISION_CAM_ID_2DBRACODE_2:
		{
			bErrWrt = m_pDualSock2->Cmd_WrtStr_Port1(szPacket, strlen(szPacket));
		}break;
		case VISION_CAM_ID_2DBRACODE_3:
		{
			bErrWrt = m_pDualSock3->Cmd_WrtStr_Port1(szPacket, strlen(szPacket));
		}break;
		case VISION_CAM_ID_2DBRACODE_4:
		{
			bErrWrt = m_pDualSock4->Cmd_WrtStr_Port1(szPacket, strlen(szPacket));
		}break;
		default: ASSERT(0);	break;
		}

		if (!bErrWrt)
			return ERR_LOAD_TBL_2_VISION_ALARMED + (nCamNo / 2);
	}
	return 0;
}


int CVisionComm_Barcode::Cmd_ReqMeasure(int nCamNo,_tVisionMsg* pVisionMsg, int nDirPocket)
{
	if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
		return ERR_NO_ERROR;

	if( m_nCamCompanyType == e2dVision_TYPE_TW){
		return CVisionComm::Cmd_ReqMeasure(nCamNo, pVisionMsg);
	}
	else if(m_nCamCompanyType == e2dVision_TYPE_Keyence)
	{
		char szPacket[255]= {0,};
		ZeroMemory( szPacket  , sizeof( szPacket  ) );
		BOOL bErrWrt = FALSE;

		int nBank = 0;

		switch (nDirPocket)
		{
			case e2dPocketDirection_REAR_LEFT:
			case e2dPocketDirection_FRONT_LEFT:
			{
				nBank = 0;
			}break;

			case e2dPocketDirection_REAR_RIGHT:
			case e2dPocketDirection_FRONT_RIGHT:
			{
				nBank = 1;
			}break;
		}
		
	//	sprintf_s(szPacket, "%cLON,0%d%c", STX, (nBank + 1), ETX);
		sprintf_s(szPacket, "%cLON%c", STX,ETX);
		switch (nCamNo)
		{
		case VISION_CAM_ID_2DBRACODE_1:
			{				
				bErrWrt = m_pDualSock1->Cmd_WrtStr_Port1(szPacket, strlen(szPacket));
			}break;
		case VISION_CAM_ID_2DBRACODE_2:
			{
				bErrWrt = m_pDualSock2->Cmd_WrtStr_Port1(szPacket, strlen(szPacket));
			}break;
		case VISION_CAM_ID_2DBRACODE_3:
			{
				bErrWrt = m_pDualSock3->Cmd_WrtStr_Port1(szPacket, strlen(szPacket));
			}break;
		case VISION_CAM_ID_2DBRACODE_4:
			{
				bErrWrt = m_pDualSock4->Cmd_WrtStr_Port1(szPacket, strlen(szPacket));
			}break;
		default: ASSERT(0);	break;
		}

		if (!bErrWrt) 
			return ERR_LOAD_TBL_1_VISION_ALARMED + (nCamNo <= VISION_CAM_ID_2DBRACODE_2 ? 0 : 1);
	}
	return 0;
}

int CVisionComm_Barcode::Cmd_KeyenceLOff(int nCamNo)
{
	if (g_IO.GetDriverType() == _DIO_TYPE_SIMUL_)
		return ERR_NO_ERROR;

	if(m_nCamCompanyType == e2dVision_TYPE_Keyence)
	{
		char szPacket[255]= {0,};
		ZeroMemory( szPacket  , sizeof( szPacket  ) );
		sprintf_s( szPacket , "LON\r");

		BOOL bErrWrt = FALSE;

		//임시
	//	bErrWrt = m_pDualSock2->Cmd_WrtStr_Port1(szPacket, strlen(szPacket));

 		switch (nCamNo)
 		{
 		case VISION_CAM_ID_2DBRACODE_1:
 			{
				return 0;
 				bErrWrt = m_pDualSock1->Cmd_WrtStr_Port1(szPacket, strlen(szPacket));
 			}break;
 		case VISION_CAM_ID_2DBRACODE_2:
 			{
 				bErrWrt = m_pDualSock2->Cmd_WrtStr_Port1(szPacket, strlen(szPacket));
 			}break;
 		case VISION_CAM_ID_2DBRACODE_3:
 			{
 				bErrWrt = m_pDualSock3->Cmd_WrtStr_Port1(szPacket, strlen(szPacket));
 			}break;
 		case VISION_CAM_ID_2DBRACODE_4:
 			{
 				bErrWrt = m_pDualSock4->Cmd_WrtStr_Port1(szPacket, strlen(szPacket));
 			}break;
 		default: ASSERT(0);	break;
 		}
		if (!bErrWrt)
			return ERR_LOAD_TBL_1_VISION_ALARMED + (nCamNo <= VISION_CAM_ID_2DBRACODE_2 ? 0 : 1);

		//int nBankOffSet = 0;
		//if( (nCamNo - VISION_CAM_ID_2DBRACODE_1) %2 != 0){
		//	nBankOffSet = 2;
		//}

		//if( nCamNo == VISION_CAM_ID_2DBRACODE_1 || nCamNo == VISION_CAM_ID_2DBRACODE_2 ){
		//	switch(nDirPocket)
		//	{
		//	case e2dPocketDirection_REAR_LEFT   :
		//	case e2dPocketDirection_REAR_RIGHT  :
		//		{
		//			bErrWrt = m_pDualSock1->Cmd_WrtStr_Port1(szPacket, strlen(szPacket));
		//		}break;
		//	case e2dPocketDirection_FRONT_LEFT  :
		//	case e2dPocketDirection_FRONT_RIGHT :
		//		{
		//			bErrWrt = m_pDualSock2->Cmd_WrtStr_Port1(szPacket, strlen(szPacket));
		//		}break;
		//	}
		//	if( !bErrWrt ) { 
		//		return ERR_LOAD_TBL_VISION_ALARMED;	
		//	}
		//}else{
		//	switch(nDirPocket)
		//	{
		//	case e2dPocketDirection_REAR_LEFT   :
		//	case e2dPocketDirection_REAR_RIGHT  :
		//		{
		//			bErrWrt = m_pDualSock3->Cmd_WrtStr_Port1(szPacket, strlen(szPacket));
		//		}break;
		//	case e2dPocketDirection_FRONT_LEFT  :
		//	case e2dPocketDirection_FRONT_RIGHT :
		//		{
		//			bErrWrt = m_pDualSock4->Cmd_WrtStr_Port1(szPacket, strlen(szPacket));
		//		}break;
		//	}
		//	if( !bErrWrt ) { return ERR_LOAD_TBL_VISION_ALARMED;	}
		//}
	}
	return 0;
}

int CVisionComm_Barcode::Cmd_ReqMeasureExist(int nCamNo,_tVisionMsg* pVisionMsg)
{
	if( m_nCamCompanyType == e2dVision_TYPE_TW){
		return CVisionComm::Cmd_ReqMeasureExist(nCamNo, pVisionMsg);
	}
	return 0;
}
int CVisionComm_Barcode::Cmd_VisionUseNoUse( int nUseNoUse, _tVisionMsg* pVisionMsg)
{
	if( m_nCamCompanyType == e2dVision_TYPE_TW){
		return CVisionComm::Cmd_VisionUseNoUse(nUseNoUse, pVisionMsg);
	}
	return 0;
}
int CVisionComm_Barcode::GetCmdDone(_tVisionMsg* pVisionMsg)
{
	if( m_nCamCompanyType == e2dVision_TYPE_TW){
	//	CVisionComm::GetCmdDone(pVisionMsg);
	}	

	return 0;
}

void CVisionComm_Barcode::ChangePocketNum(int nCamNo, int &nPocketX, int &nPocketY, int &nPocketNum)
{
//	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM11_Handler_Project_Name);
	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
	int nPocketCntX = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_X);
	int nPocketCntY = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_Div_Y);

	int nXBuff = 0, nYBuff = 0;

	if (nBoardType == eBoardDir_Forward)
	{
		//X, Y Pocket Change
		if (nCamNo < VISION_CAM_ID_2DBRACODE_3) {
			nXBuff = 3 - nPocketX; // 마지막 Pocket X 부터 2d 촬영 -> 3,2,1,0
			nYBuff = (nCamNo == VISION_CAM_ID_2DBRACODE_1) ? 0 : 2;
		}
		else {
			nXBuff = nPocketX; //첫번째 Pocket X 부터 2d 촬영 -> 0,1,2,3
			nYBuff = (nCamNo == VISION_CAM_ID_2DBRACODE_3) ? 2 : 0;
		}

		//X, Y Pocket Save.
		nPocketX = nXBuff;
		nPocketY = nYBuff;

		//Pocket Numbering. 
		//Table 방향 상관 없이 (X,Y)에 해당 되는 Numbering 지정 -> ex) (0,0)은 Num 0.
		nPocketNum = nPocketX + (nPocketCntY*(nPocketY / 2));
	}
	else
	{
		//X, Y Pocket Change
		if (nCamNo < VISION_CAM_ID_2DBRACODE_3) {
			nXBuff = nPocketX; //첫번째 Pocket X 부터 2d 촬영 -> 0,1,2,3
							   //nXBuff = 3 - nPocketX; // 마지막 Pocket X 부터 2d 촬영 -> 3,2,1,0
			nYBuff = (nCamNo == VISION_CAM_ID_2DBRACODE_1) ? 3 : 1;
		}
		else {
			nXBuff = abs(nPocketX - 3); //첫번째 Pocket X 부터 2d 촬영 -> 0,1,2,3
			nYBuff = (nCamNo == VISION_CAM_ID_2DBRACODE_3) ? 1 : 3;
		}

		//X, Y Pocket Save.
		nPocketX = nXBuff;
		nPocketY = nYBuff;

		//Pocket Numbering. 
		//Table 방향 상관 없이 (X,Y)에 해당 되는 Numbering 지정 -> ex) (0,0)은 Num 0.
		nPocketNum = nPocketX + (nPocketCntY*(nPocketY / 3));
 	}



// 	//임시
// 	//X, Y Pocket Change
// 	if (nCamNo < VISION_CAM_ID_2DBRACODE_3) {
// 		nXBuff = 3 - nPocketX; // 마지막 Pocket X 부터 2d 촬영 -> 3,2,1,0
// 		nYBuff = (nCamNo == VISION_CAM_ID_2DBRACODE_1) ? 0 : 2;
// 	}
// 	else {
// 		nXBuff = nPocketX; //첫번째 Pocket X 부터 2d 촬영 -> 0,1,2,3
// 		nYBuff = (nCamNo == VISION_CAM_ID_2DBRACODE_3) ? 2 : 0;
// 	}




// 	//X, Y Pocket Save.
// 	nPocketX = nXBuff;
// 	nPocketY = nYBuff;
// 
// 	//Pocket Numbering. 
// 	//Table 방향 상관 없이 (X,Y)에 해당 되는 Numbering 지정 -> ex) (0,0)은 Num 0.
// 	nPocketNum = nPocketX + (nPocketCntY*(nPocketY / 2));

}

void CVisionComm_Barcode::Erase2dPicture(int nTblIdx)
{	
	CString strIp = _T("");

	for (int i = 0; i < 2; i++) //Barcode 2ea Erase
	{
		//Left Table
		if (nTblIdx / 2 == 0)
		{
			strIp = m_strLeft2dIp[i];
		}
		//Right Table
		else
		{
			strIp = m_strRight2dIp[i];
		}

		CInternetSession session;
		CFtpConnection* pConnect = NULL;
		CFtpFileFind* pFileFind = NULL;
		CString strDir, StrResult;

		CString cstrFindFile;

		try
		{
			pConnect = session.GetFtpConnection(strIp, "abc", "123");
			if (pConnect == NULL) // 연결 실패 하면 Return 처리. 프로그램 먹통 방지
				return;

			pFileFind = new CFtpFileFind(pConnect); //파일 찾기

			BOOL bExist = pFileFind->FindFile("IMAGE");

			while (bExist)
			{
				bExist = pFileFind->FindNextFile();



				if (bExist != TRUE) {
					pConnect->Remove(pFileFind->GetFilePath());
					break;
				}

				CString strFileName = pFileFind->GetFilePath();

				if (pFileFind->IsDots())
				{
					continue;
				}

				int nRt = pConnect->Remove(strFileName);
			}
		}
		catch (CInternetException *e)
		{
			//e->ReportError(MB_ICONEXCLAMATION);
			pConnect = NULL;
			e->Delete();
		}

		if (pConnect != NULL)
			pConnect->Close();

		if (pFileFind != NULL)
			delete pFileFind;
	}
}

void CVisionComm_Barcode::AddRetryCountCurLot(int nBarcodeNo)
{
	int NDM3_ID = (nBarcodeNo == 0) ? NDM3_BARCODE1_RETRY_CNT : NDM3_BARCODE2_RETRY_CNT;

	int nRetryCnt = g_DMCont.m_dmShuttle.GN(NDM3_ID);

	nRetryCnt++;

	g_DMCont.m_dmShuttle.SN(NDM3_ID, nRetryCnt);
}


void CVisionComm_Barcode::AddTriggerTotalCount(int nBarcodeNo, int nTrrigerCnt)
{
	int NDM3_ID = (nBarcodeNo == 0) ? NDM3_BARCODE1_TOTAL_TRIGGER_CNT : NDM3_BARCODE2_TOTAL_TRIGGER_CNT;

	int nCnt = g_DMCont.m_dmShuttle.GN(NDM3_ID);

	nCnt += nTrrigerCnt;

	g_DMCont.m_dmShuttle.SN(NDM3_ID, nCnt);
}