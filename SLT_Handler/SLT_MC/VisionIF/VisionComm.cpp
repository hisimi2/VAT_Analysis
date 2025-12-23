
#include "StdAfx.h"
#include "VisionComm.h"
#include "ConfigData.h"
#include <string>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CVisionComm::CVisionComm(int nVisionPcIdx, CString strVisionName, int nCamCompanyType)
	:m_nVisionPcIdx(nVisionPcIdx), m_strVisionPcName(strVisionName), m_nCamCompanyType(nCamCompanyType)
{
	ZeroMemory(m_szDriverFileName, sizeof(char)*MAX_PATH);

	m_VisionMsg.clear();
	m_VisionMsg.reserve(eMaxPressUnitCount);
	ST_VISON_MSG stVisionMsg;

	for(int i=0; i<(eMaxPressUnitCount); i++)
	{
		m_VisionMsg.push_back(stVisionMsg);
	}


	for (int i = 0; i < eMaxTestPPCount; i++)
	{
		m_BarcodeLightMsg.clear();
	}

}

CVisionComm::CVisionComm()
{
	m_nVisionPcIdx = 0;
	m_strVisionPcName = _T("");
	m_nCamCompanyType = 0;
	m_szDriverFileName[MAX_PATH] = { 0, };
	::InitializeCriticalSection( m_csCmdDone );
}


CVisionComm::~CVisionComm(void)
{
	::DeleteCriticalSection( m_csCmdDone );
}

void CVisionComm::ReadConnectConfig(CString &a_strIp, int& a_nPort, int nCamNo)
{
	CConfigData ConfigData(m_szDriverFileName);
	char szKey[128]     = {0,};
	int nPort = 0;
	char szIp[64] = {0,};
	if( m_nCamCompanyType == e2dVision_TYPE_TW){
		char szDefault[128] = {0,};

		// ip address
		ZeroMemory(szKey, sizeof(szKey));
		sprintf_s(szKey, sizeof(szKey), "%s IP", m_strVisionPcName.GetString() );
		ConfigData.GetString("MC TO VISION COMM CONFIG", szKey, "127.0.0.1", szIp);

		// port number
		ZeroMemory(szKey, sizeof(szKey));
		sprintf_s(szKey, sizeof(szKey), "%s Port", m_strVisionPcName.GetString() );

		ZeroMemory(szDefault, sizeof(szDefault));
		sprintf_s(szDefault, sizeof(szDefault), "%d", 8000+m_nVisionPcIdx);

		nPort = ConfigData.GetInt("MC TO VISION COMM CONFIG", szKey, szDefault);
	}
	else if(m_nCamCompanyType == e2dVision_TYPE_Keyence){		
		sprintf_s(szKey, sizeof(szKey), "KEYENCE_2DIP_%s", ((nCamNo == 0 || nCamNo == 1) ? _T("LEFT") : _T("RIGHT")));
		ConfigData.GetString("MC TO VISION COMM CONFIG", szKey, "127.0.0.1", szIp);
		sprintf_s(szKey, sizeof(szKey), "KEYENCE_2DPORT_%s", ((nCamNo == 0 || nCamNo == 1) ? _T("LEFT") : _T("RIGHT")));
		nPort = ConfigData.GetInt("MC TO VISION COMM CONFIG", szKey , "8000");
	}

	a_strIp = szIp;
	a_nPort = nPort;
}

void CVisionComm::WriteConnectConfig(CString a_strIp, int a_nPort, int nCamNo)
{
	char szIp[64] = {0,};
	char szKey[128]= {0,};
	int nPort = 0;
	
	CConfigData ConfigData(m_szDriverFileName);

	// ip address
	sprintf_s(szIp, sizeof(szIp), "%s", a_strIp.GetString() );

	if( m_nCamCompanyType == e2dVision_TYPE_TW){
		sprintf_s(szKey, sizeof(szKey), "%s IP", m_strVisionPcName.GetString() );
		ConfigData.Set("MC TO VISION COMM CONFIG", szKey, szIp);

		// port number
		ZeroMemory(szKey, sizeof(szKey));
		sprintf_s(szKey, sizeof(szKey), "%s Port", m_strVisionPcName.GetString() );

		ConfigData.Set("MC TO VISION COMM CONFIG", szKey, a_nPort);
	}
	else if(m_nCamCompanyType == e2dVision_TYPE_Keyence){
		sprintf_s(szKey, sizeof(szKey), "KEYENCE_2DIP_%s", ((nCamNo == 0 || nCamNo == 1) ? _T("LEFT") : _T("RIGHT")));
		ConfigData.Set("MC TO VISION COMM CONFIG", szKey, szIp);
		sprintf_s(szKey, sizeof(szKey), "KEYENCE_2DPORT_%s", ((nCamNo == 0 || nCamNo == 1) ? _T("LEFT") : _T("RIGHT")));
		ConfigData.Set("MC TO VISION COMM CONFIG", szKey , a_nPort);
	}
}


int CVisionComm::Cmd_SetCok(_tVisionMsg* pVisionMsg/*=NULL*/)
{
	CString strEqpType;
	CString strCokName;
	CString strDeviceSizeX;
	CString strDeviceSizeY;
	CString strPocketCountX;
	CString strPocketCountY;

	char szBuff[512]={0,};

	strEqpType.Format("%d", CVisionComm::eEqpType_SLT);

	ZeroMemory(szBuff, sizeof(szBuff));
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szBuff, sizeof(szBuff));
	strCokName = szBuff;

	double dDeviceSizeX = (double)g_DMCont.m_dmHandlerOpt.GN(NDM11_DVC_Dimen_X) / 1000.0;
	double dDeviceSizeY = (double)g_DMCont.m_dmHandlerOpt.GN(NDM11_DVC_Dimen_Y) / 1000.0;

	strDeviceSizeX.Format("%0.1f", dDeviceSizeX);
	strDeviceSizeY.Format("%0.1f", dDeviceSizeY);

	if( m_nVisionPcIdx == eVision_BarCode )
	{
		int nPocketCntX = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Div_X );
		int nPocketCntY = g_DMCont.m_dmShuttle.GN( NDM3_LdTbl_Div_Y );

		strPocketCountX.Format("%d", nPocketCntX);
		strPocketCountY.Format("%d", nPocketCntY);
	}
	else
	{
		int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
		int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

		strPocketCountX.Format("%d", nSiteDivX);
		strPocketCountY.Format("%d", nSiteDivY);
	}

	//int nProjectOption = g_DMCont.m_dmEQP.GN(NDM11_Handler_Project_Name);
	int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	char cSocketCnt[2] = "";

	sprintf_s(cSocketCnt, sizeof(cSocketCnt), "%d", nSiteDivX*nSiteDivY);
	
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = VISION_RCMD_REQ_SET_COK;
	pBody->stData.nParamCount = 2;
	sprintf_s(pBody->stData.szParam[0] , "%s", strCokName.GetString() );
	sprintf_s(pBody->stData.szParam[1], "%s", cSocketCnt);


//	sprintf_s(pBody->stData.szParam[1] , "%s,%s", strDeviceSizeX.GetString(), strDeviceSizeY.GetString());
//	sprintf_s(pBody->stData.szParam[2] , "%s,%s", strPocketCountX.GetString(), strPocketCountY.GetString());

// 	sprintf_s(pBody->stData.szParam[0] , "%s", strEqpType.GetString() );
// 	sprintf_s(pBody->stData.szParam[1] , "%s", strCokName.GetString() );
// 	sprintf_s(pBody->stData.szParam[2] , "%s", strDeviceSizeX.GetString() );
// 	sprintf_s(pBody->stData.szParam[3] , "%s", strDeviceSizeY.GetString() );
// 	sprintf_s(pBody->stData.szParam[4] , "%s", strPocketCountX.GetString() );
// 	sprintf_s(pBody->stData.szParam[5] , "%s", strPocketCountY.GetString() );

	int nErr = CIPCSock::Send(pPacket, FALSE);
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
			m_csCmdDone.Lock();
			m_VisionCmdResult.push_back(pWaitVisionMsg);
			m_csCmdDone.Unlock();
		}
		nErr = ERR_NO_ERROR;
		SendLogMessage("[snd] S2F41, MsgId : %d, CMD : %d", pPacket->pHead->stData.nMsgId, pBody->stData.nCmd);
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

	return nErr;
}

int CVisionComm::Cmd_ReqCleanDisk(_tVisionMsg* pVisionMsg/*=NULL*/)
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = VISION_RCMD_REQ_CLEAN_DISK;
	pBody->stData.nParamCount = 0;
	
	int nErr = CIPCSock::Send(pPacket, FALSE);
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
			m_csCmdDone.Lock();
			m_VisionCmdResult.push_back(pWaitVisionMsg);
			m_csCmdDone.Unlock();
		}
		nErr = ERR_NO_ERROR;
		SendLogMessage("[snd] S2F41, MsgId : %d, CMD : %d", pPacket->pHead->stData.nMsgId, pBody->stData.nCmd);
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

	return nErr;
}

int CVisionComm::Cmd_ReqVisionShutDown(_tVisionMsg* pVisionMsg/*=NULL*/)
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = VISION_RCMD_REQ_SHUTDOWN;
	pBody->stData.nParamCount = 0;

	int nErr = CIPCSock::Send(pPacket, FALSE);
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
			m_csCmdDone.Lock();
			m_VisionCmdResult.push_back(pWaitVisionMsg);
			m_csCmdDone.Unlock();
		}
		nErr = ERR_NO_ERROR;
		SendLogMessage("[snd] S2F41, MsgId : %d, CMD : %d", pPacket->pHead->stData.nMsgId, pBody->stData.nCmd);
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

	return nErr;
}

int CVisionComm::Cmd_ReqMeasureExist_HandCam(int nCamNo,_tVisionMsg* pVisionMsg, std::vector<CPoint> vPicker, CPoint cPocket, int nPressIdx/*=StationNo*/,int nDvcType)
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = VISION_RCMD_REQ_MEASURE_EXIST_HAND_CAM;// VISION_RCMD_REQ_MEASURE_EXIST;

	//pBody->stData.nParamCount = 5;

	sprintf_s(pBody->stData.szParam[0], sizeof(pBody->stData.szParam[0]), "%d", nCamNo);
	// 1번은 아래서 계산해서 넣는다.
	sprintf_s(pBody->stData.szParam[2], sizeof(pBody->stData.szParam[2]), "%d", nPressIdx);
	sprintf_s(pBody->stData.szParam[3], sizeof(pBody->stData.szParam[3]), "%d", cPocket.y);

	BOOL bVisionCleaningDvcMode = g_DMCont.m_dmEQP.GB(BDM0_MD_VISION_CLEANING_DEVICE);

	if (bVisionCleaningDvcMode == DEF_ON) {
		sprintf_s(pBody->stData.szParam[4], sizeof(pBody->stData.szParam[4]), "%d", nDvcType);
		pBody->stData.nParamCount = 5;
	}
	else {
		pBody->stData.nParamCount = 4;
	}
	

	char szDeviceExsit[64]={0,};
	if( m_nVisionPcIdx == eVision_BarCode )
	{
		if( (nCamNo >= VISION_CAM_ID_2DBRACODE_1) || (nCamNo <= VISION_CAM_ID_2DBRACODE_4) )
		{
			int nIdx = nCamNo - VISION_CAM_ID_2DBRACODE_1;
			for(int y=0; y<MAX_LD_TBL_POCKET_CNT_Y; y++)
			{
				char szTemp[128]={0,};	
				for(int x=0; x<MAX_LD_TBL_POCKET_CNT_X/2; x++) // 좌측 2열만 vision check 함.
				{
					int nExist = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_LD_TBL_1+nIdx, x, y);
					if( nExist == DEF_EXIST )
					{
						strcat_s(szTemp, sizeof(szTemp), "1");
					}
					else
					{
						strcat_s(szTemp, sizeof(szTemp), "0");
					}
				}
				strcat_s(szTemp, sizeof(szTemp), ";");
				strcat_s(szDeviceExsit, sizeof(szDeviceExsit), szTemp);
			}
			strcat_s(szDeviceExsit, sizeof(szDeviceExsit), "00");
		}
	}
	else
	{
		//int nIdx = -1;
		//if( nCamNo >= VISION_CAM_ID_TESTSITE_DOWN_1 && nCamNo <= VISION_CAM_ID_TESTSITE_DOWN_8 )
		//	nIdx = nCamNo - VISION_CAM_ID_TESTSITE_DOWN_1;
		//else if( nCamNo >= VISION_CAM_ID_TESTSITE_UP_1 && nCamNo <= VISION_CAM_ID_TESTSITE_UP_8 )
		//	nIdx = nCamNo - VISION_CAM_ID_TESTSITE_UP_1 + ePressUnit_Up_1;

		CString strSubSite = _T("");
		for( int nPicker = 0 ; nPicker < MAX_TEST_PP_PKR_CNT_X; nPicker++ )
		{
			BOOL bFindPicker = FALSE;
			for (std::vector<CPoint>::iterator it = vPicker.begin(); it != vPicker.end(); ++it)
			{
				if( it->x == nPicker )
				{
					strSubSite += _T("1");
					strcat_s(szDeviceExsit, sizeof(szDeviceExsit), strSubSite);
					bFindPicker = TRUE;
					break;
				}
			}

			if( bFindPicker == FALSE){
				strSubSite += _T("0");
				strcat_s(szDeviceExsit, sizeof(szDeviceExsit), strSubSite);
			}

			if( nPicker != MAX_TEST_PP_PKR_CNT_X-1)
				strcat_s(szDeviceExsit, sizeof(szDeviceExsit), ",");

			strSubSite.Empty();
		}
	}
	sprintf_s(pBody->stData.szParam[1], "%s", szDeviceExsit);

	int nErr = CIPCSock::Send(pPacket, FALSE);
	if( nErr == 0 )
	{
		int nRealCamNo = Def_CamNoChangeRealNo(nCamNo);
		m_VisionMsg[nRealCamNo].clear();
		m_VisionMsg[nRealCamNo].nMsgId = pPacket->pHead->stData.nMsgId;
		m_VisionMsg[nRealCamNo].nReceived = eVisionMsgWaitState_Waiting;

		nErr = ERR_NO_ERROR;
		SendLogMessage("[snd Measure Exist] S2F41, MsgId : %d, CMD : %d, Cam: %d", pPacket->pHead->stData.nMsgId, pBody->stData.nCmd, nRealCamNo +1);
		for(int i=0; i<pBody->stData.nParamCount; i++)
		{
			SendLogMessage("[snd Measure Exist] Param%d : %s", i+1, pBody->stData.szParam[i]);
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

	return nErr;
}

int CVisionComm::Cmd_ReqMeasureExist(int nCamNo,_tVisionMsg* pVisionMsg/*=NULL*/)
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = VISION_RCMD_REQ_MEASURE_EXIST;

	pBody->stData.nParamCount = 2;

	sprintf_s(pBody->stData.szParam[0], sizeof(pBody->stData.szParam[0]), "%d", nCamNo);

	char szDeviceExsit[64]={0,};
	if( m_nVisionPcIdx == eVision_BarCode )
	{
		if( (nCamNo >= VISION_CAM_ID_2DBRACODE_1) || (nCamNo <= VISION_CAM_ID_2DBRACODE_4) )
		{
			int nIdx = nCamNo - VISION_CAM_ID_2DBRACODE_1;
			for(int y=0; y<MAX_LD_TBL_POCKET_CNT_Y; y++)
			{
				char szTemp[128]={0,};	
				for(int x=0; x<MAX_LD_TBL_POCKET_CNT_X/2; x++) // 좌측 2열만 vision check 함.
				{
					int nExist = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_LD_TBL_1+nIdx, x, y);
					if( nExist == DEF_EXIST )
					{
						strcat_s(szTemp, sizeof(szTemp), "1");
					}
					else
					{
						strcat_s(szTemp, sizeof(szTemp), "0");
					}
				}
				strcat_s(szTemp, sizeof(szTemp), ";");
				strcat_s(szDeviceExsit, sizeof(szDeviceExsit), szTemp);
			}
			strcat_s(szDeviceExsit, sizeof(szDeviceExsit), "00");
		}
	}
	else
	{
		int nIdx = -1;
		if( nCamNo >= VISION_CAM_ID_TESTSITE_DOWN_1 && nCamNo <= VISION_CAM_ID_TESTSITE_DOWN_8 )
			nIdx = nCamNo - VISION_CAM_ID_TESTSITE_DOWN_1;
// 		else if( nCamNo >= VISION_CAM_ID_TESTSITE_UP_1 && nCamNo <= VISION_CAM_ID_TESTSITE_UP_8 )
// 			nIdx = nCamNo - VISION_CAM_ID_TESTSITE_UP_1 + ePressUnit_Up_1;

		BOOL bIsFirst = TRUE;
		CString strSubSite = _T("");

		for( int nX=0; nX < STATION_MAX_X_SITE; nX++ ){
			int nExist = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TEST_SITE_1+nIdx, nX, 0);
			int nSiteAddr = NDM11_SocketOnOff_Site1_1+(STATION_MAX_X_SITE*nIdx)+nX;

			if(bIsFirst == FALSE) { strSubSite += _T(","); }

			if( nExist == DEF_EXIST && (g_DMCont.m_dmHandlerOpt.GN(nSiteAddr) == DEF_SITE_ON || g_DMCont.m_dmHandlerOpt.GN(nSiteAddr) == DEF_SITE_OFF_RESERVATION || g_DMCont.m_dmHandlerOpt.GN(nSiteAddr) == DEF_SITE_OFF_RESERVATION_YIELD) ){
				strSubSite += _T("1");
				strcat_s(szDeviceExsit, sizeof(szDeviceExsit), strSubSite);
			}else{
				strSubSite += _T("0");
				strcat_s(szDeviceExsit, sizeof(szDeviceExsit), strSubSite);
			}
			strSubSite.Empty();

			bIsFirst = FALSE;
		}
		//sprintf_s(szDeviceExsit, "0");
	}
	sprintf_s(pBody->stData.szParam[1], "%s", szDeviceExsit);

	int nErr = CIPCSock::Send(pPacket, FALSE);
	if( nErr == 0 )
	{
		//if( pVisionMsg != NULL )
		//{
		//	_tVisionMsg* pWaitVisionMsg = new _tVisionMsg;
		//	pWaitVisionMsg->nMsgId = pPacket->pHead->stData.nMsgId;
		//	pWaitVisionMsg->nS = pPacket->pHead->stData.nS;
		//	pWaitVisionMsg->nF = pPacket->pHead->stData.nF;
		//	pWaitVisionMsg->nRcmd = pBody->stData.nCmd;
		//	pWaitVisionMsg->nReceived = eVisionMsgWaitState_Waiting;
		//	*pVisionMsg = *pWaitVisionMsg;
		//	m_csCmdDone.Lock();
		//	m_VisionCmdResult.push_back(pWaitVisionMsg);
		//	m_csCmdDone.Unlock();
		//}

		int nRealCamNo = Def_CamNoChangeRealNo(nCamNo);
		ASSERT(nRealCamNo>=0 && nRealCamNo<8);
		m_VisionMsg[nRealCamNo].clear();
		m_VisionMsg[nRealCamNo].nMsgId = pPacket->pHead->stData.nMsgId;
		m_VisionMsg[nRealCamNo].nReceived = eVisionMsgWaitState_Waiting;

		nErr = ERR_NO_ERROR;
		SendLogMessage("[snd Measure Exist] S2F41, MsgId : %d, CMD : %d, Cam: %d", pPacket->pHead->stData.nMsgId, pBody->stData.nCmd, nRealCamNo+1);
		for(int i=0; i<pBody->stData.nParamCount; i++)
		{
			SendLogMessage("[snd Measure Exist] Param%d : %s", i+1, pBody->stData.szParam[i]);
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

	return nErr;
}

int CVisionComm::Cmd_VisionUseNoUse( int nUseNoUse, _tVisionMsg* pVisionMsg/*=NULL*/ )
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = VISION_RCMD_REQ_VISION_USE_NOUSE;
	pBody->stData.nParamCount = 1;
	sprintf_s(pBody->stData.szParam[0], "%d", nUseNoUse);

	int nErr = CIPCSock::Send(pPacket, FALSE);
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
			m_csCmdDone.Lock();
			m_VisionCmdResult.push_back(pWaitVisionMsg);
			m_csCmdDone.Unlock();
		}

		nErr = ERR_NO_ERROR;
		SendLogMessage("[snd] S2F41, MsgId : %d, CMD : %d", pPacket->pHead->stData.nMsgId, pBody->stData.nCmd);
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

	return nErr;
}

int CVisionComm::Cmd_ReqInspReady(int nCamNo,_tVisionMsg* pVisionMsg/*=NULL*/)
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = VISION_RCMD_REQ_INSP_READY;
	pBody->stData.nParamCount = 2;

	sprintf_s(pBody->stData.szParam[0], "%d", nCamNo);

	char szDeviceExsit[128]={0,};
	if( m_nVisionPcIdx == eVision_BarCode )
	{
		if( (nCamNo >= VISION_CAM_ID_2DBRACODE_1) || (nCamNo <= VISION_CAM_ID_2DBRACODE_4) )
		{
			int nIdx = nCamNo - VISION_CAM_ID_2DBRACODE_1;
			for(int y=0; y<MAX_LD_TBL_POCKET_CNT_Y; y++)
			{
				char szTemp[128]={0,};	
				for(int x=0; x<MAX_LD_TBL_POCKET_CNT_X/2; x++) // 좌측 2열만 vision check 함.
				{
					int nExist = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_LD_TBL_1+nIdx, x, y);
					if( nExist == DEF_EXIST )
					{
						strcat_s(szTemp, sizeof(szTemp), "1");
					}
					else
					{
						strcat_s(szTemp, sizeof(szTemp), "0");
					}
				}
				strcat_s(szTemp, sizeof(szTemp), ";");
				strcat_s(szDeviceExsit, sizeof(szDeviceExsit), szTemp);
			}
			strcat_s(szDeviceExsit, sizeof(szDeviceExsit), "00");
		}
	}
	else
	{
		sprintf_s(szDeviceExsit, "0");
	}
	sprintf_s(pBody->stData.szParam[1], "%s", szDeviceExsit);	

	int nErr = CIPCSock::Send(pPacket, FALSE);
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
			m_csCmdDone.Lock();
			m_VisionCmdResult.push_back(pWaitVisionMsg);
			m_csCmdDone.Unlock();
		}
		nErr = ERR_NO_ERROR;
		SendLogMessage("[snd] S2F41, MsgId : %d, CMD : %d", pPacket->pHead->stData.nMsgId, pBody->stData.nCmd);
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

	return nErr;
}

int CVisionComm::Cmd_ReqMeasure_HandCam(int nCamNo,_tVisionMsg* pVisionMsg, std::vector<CPoint> vPicker, CPoint cPocket, int nPressIdx/*=StationNo*/)
{
	/*
	CPacket* pPacket = new CPacket(2, 3);
	CPacketBody_S2F3* pBody = (CPacketBody_S2F3*)pPacket->pBody;
	//pBody->stData.nCmd = VISION_RCMD_REQ_MEASURE_HAND_CAM;//VISION_RCMD_REQ_MEASURE;

	pBody->stData.nMsgID = nCamNo == VISION_CAM_ID_TEST_HAND_1 ? VISION_REQ_MEASURE_TEST_HAND_1_EMPTY: VISION_REQ_MEASURE_TEST_HAND_2_EMPTY;//VISION_RCMD_REQ_MEASURE;
	pBody->stData.nStatus = 0;
	pBody->stData.nDataCount = 2;
	//sprintf_s(pBody->stData.szParam[0], sizeof(pBody->stData.szParam[0]), "%d", nCamNo);
	// 1번은 아래 계산해서 넣는다.
	sprintf_s(pBody->stData.szDataName[1], sizeof(pBody->stData.szDataName[1]), "%d", nPressIdx);
	//sprintf_s(pBody->stData.szParam[3], sizeof(pBody->stData.szParam[3]), "%d", cPocket.y);
	

	char szDeviceExsit[64]={0,};
	if( m_nVisionPcIdx == eVision_BarCode )
	{
		if( (nCamNo >= VISION_CAM_ID_2DBRACODE_1) || (nCamNo <= VISION_CAM_ID_2DBRACODE_4) )
		{
			int nIdx = nCamNo - VISION_CAM_ID_2DBRACODE_1;
			for(int y=0; y<MAX_LD_TBL_POCKET_CNT_Y; y++)
			{
				char szTemp[128]={0,};	
				for(int x=0; x<MAX_LD_TBL_POCKET_CNT_X/2; x++) // 좌측 2열만 vision check 함.
				{
					int nExist = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_LD_TBL_1+nIdx, x, y);
					// 					ST_DD_DEVICE tDeviceData;
					// 					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1+nIdx, x, y, &tDeviceData);
					// 					if( tDeviceData.sExist == DEF_EXIST )
					if( nExist == DEF_EXIST )
					{
						strcat_s(szTemp, sizeof(szTemp), "1");
					}
					else
					{
						strcat_s(szTemp, sizeof(szTemp), "0");
					}
				}
				strcat_s(szTemp, sizeof(szTemp), ";");
				strcat_s(szDeviceExsit, sizeof(szDeviceExsit), szTemp);
			}
			strcat_s(szDeviceExsit, sizeof(szDeviceExsit), "00");
		}
	}
	else
	{
		int nIdx = -1;
		if( nCamNo >= VISION_CAM_ID_TESTSITE_DOWN_1 && nCamNo <= VISION_CAM_ID_TESTSITE_DOWN_8 )
			nIdx = nCamNo - VISION_CAM_ID_TESTSITE_DOWN_1;
// 		else if( nCamNo >= VISION_CAM_ID_TESTSITE_UP_1 && nCamNo <= VISION_CAM_ID_TESTSITE_UP_8 )
// 			nIdx = nCamNo - VISION_CAM_ID_TESTSITE_UP_1 + ePressUnit_Up_1;

		BOOL bIsFirst = TRUE;
		CString strSubSite ;

		ST_DD_DEVICE pDvcData;

		for (int nPickerY = 0; nPickerY < MAX_TEST_PP_PKR_CNT_Y; nPickerY++)
		{
			for (int nPickerX = 0; nPickerX < MAX_TEST_PP_PKR_CNT_X; nPickerX++)
			{
				//BOOL bFindPicker = FALSE;
				//for (std::vector<CPoint>::iterator it = vPicker.begin(); it != vPicker.end(); ++it)
				//{
				//	if( it->x == nPicker )
				//	{
				//		strSubSite += _T("1");
				//		strcat_s(szDeviceExsit, sizeof(szDeviceExsit), strSubSite);
				//		bFindPicker = TRUE;
				//		break;
				//	}
				//}
				//if (bFindPicker== FALSE){
				//	strSubSite += _T("0");
				//	strcat_s(szDeviceExsit, sizeof(szDeviceExsit), strSubSite);
				//}
				int nExist = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TEST_SITE_1 + nPressIdx, nPickerX, nPickerY);

				if (nExist == DEF_EXIST) {
					strSubSite += _T("0");
					strcat_s(szDeviceExsit, sizeof(szDeviceExsit), strSubSite);
				}
				else {
					strSubSite += _T("1");
					strcat_s(szDeviceExsit, sizeof(szDeviceExsit), strSubSite);
				}

				if (nPickerX*nPickerY != MAX_TEST_PP_PKR_CNT - 1)
				{
					//strcat_s(szDeviceExsit, sizeof(szDeviceExsit), ",");
				}

				strSubSite.Empty();
				bIsFirst = FALSE;
			}
		}
 	}
	int nGetSize = pBody->GetSize();
	sprintf_s(pBody->stData.szDataName[0], "%s", szDeviceExsit);

	int nErr = CIPCSock::Send(pPacket, FALSE);
	if( nErr == 0 )
	{
		int nRealCamNo = Def_CamNoChangeRealNo(nCamNo);
		ASSERT(nRealCamNo>=0 && nRealCamNo<8);
		m_VisionMsg[nRealCamNo].clear();
		m_VisionMsg[nRealCamNo].nMsgId = pPacket->pHead->stData.nMsgId;
		m_VisionMsg[nRealCamNo].nReceived = eVisionMsgWaitState_Waiting;

		nErr = ERR_NO_ERROR;

		CString strCamPos = (nCamNo >= VISION_CAM_ID_TESTSITE_DOWN_1) ? _T("Down"): _T("UP");
		SendLogMessage("[Send Measure] S2F41, Cam: %s %d,  MsgId : %d, CMD : %d", strCamPos,nRealCamNo+1, pPacket->pHead->stData.nMsgId, pBody->stData.nMsgID);
		for(int i=0; i<pBody->stData.nDataCount; i++)
		{
			SendLogMessage("[Send Measure] S2F41, Cam: %s %d, Param%d : %s", strCamPos, nRealCamNo+1, i+1, pBody->stData.szDataName[i]);
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

	return nErr;
	*/
return 0;
}


int CVisionComm::Cmd_ReqMeasure_StationCam(int nStationNo, _tVisionMsg* pVisionMsg, std::vector<CPoint> vPicker, CPoint cPocket, int nPressIdx/*=StationNo*/)
{
	CPacket* pPacket = new CPacket(2, 3);
	CPacketBody_S2F3* pBody = (CPacketBody_S2F3*)pPacket->pBody;
	//pBody->stData.nCmd = VISION_RCMD_REQ_MEASURE_HAND_CAM;//VISION_RCMD_REQ_MEASURE;

	pBody->stData.nMsgID = Def_StationNoChangeMessageNo(nStationNo);
	pBody->stData.nStatus = 0;
	pBody->stData.nDataCount = 2;
	//sprintf_s(pBody->stData.szParam[0], sizeof(pBody->stData.szParam[0]), "%d", nCamNo);
	// 1번은 아래 계산해서 넣는다.
	sprintf_s(pBody->stData.szDataName[1], sizeof(pBody->stData.szDataName[1]), "%d", nPressIdx);
	//sprintf_s(pBody->stData.szParam[3], sizeof(pBody->stData.szParam[3]), "%d", cPocket.y);


	char szDeviceExsit[64] = { 0, };
	if (m_nVisionPcIdx == eVision_BarCode)
	{
		if ((nStationNo >= VISION_CAM_ID_2DBRACODE_1) || (nStationNo <= VISION_CAM_ID_2DBRACODE_4))
		{
			int nIdx = nStationNo - VISION_CAM_ID_2DBRACODE_1;
			for (int y = 0; y < MAX_LD_TBL_POCKET_CNT_Y; y++)
			{
				char szTemp[128] = { 0, };
				for (int x = 0; x < MAX_LD_TBL_POCKET_CNT_X / 2; x++) // 좌측 2열만 vision check 함.
				{
					int nExist = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_LD_TBL_1 + nIdx, x, y);
					// 					ST_DD_DEVICE tDeviceData;
					// 					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1+nIdx, x, y, &tDeviceData);
					// 					if( tDeviceData.sExist == DEF_EXIST )
					if (nExist == DEF_EXIST)
					{
						strcat_s(szTemp, sizeof(szTemp), "1");
					}
					else
					{
						strcat_s(szTemp, sizeof(szTemp), "0");
					}
				}
				strcat_s(szTemp, sizeof(szTemp), ";");
				strcat_s(szDeviceExsit, sizeof(szDeviceExsit), szTemp);
			}
			strcat_s(szDeviceExsit, sizeof(szDeviceExsit), "00");
		}
	}
	else
	{
		int nIdx = -1;
		if (nStationNo >= VISION_CAM_ID_TESTSITE_DOWN_1 && nStationNo <= VISION_CAM_ID_TESTSITE_DOWN_8)
			nIdx = nStationNo - VISION_CAM_ID_TESTSITE_DOWN_1;
		// 		else if( nCamNo >= VISION_CAM_ID_TESTSITE_UP_1 && nCamNo <= VISION_CAM_ID_TESTSITE_UP_8 )
		// 			nIdx = nCamNo - VISION_CAM_ID_TESTSITE_UP_1 + ePressUnit_Up_1;

		BOOL bIsFirst = TRUE;
		CString strSubSite;

		ST_DD_DEVICE pDvcData;

		int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
		int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

		for (int nPickerY = 0; nPickerY < nSiteDivY/*MAX_TEST_PP_PKR_CNT_Y*/; nPickerY++)
		{
			for (int nPickerX = 0; nPickerX < nSiteDivX/*MAX_TEST_PP_PKR_CNT_X*/; nPickerX++)
			{
				//BOOL bFindPicker = FALSE;
				//for (std::vector<CPoint>::iterator it = vPicker.begin(); it != vPicker.end(); ++it)
				//{
				//	if( it->x == nPicker )
				//	{
				//		strSubSite += _T("1");
				//		strcat_s(szDeviceExsit, sizeof(szDeviceExsit), strSubSite);
				//		bFindPicker = TRUE;
				//		break;
				//	}
				//}
				//if (bFindPicker== FALSE){
				//	strSubSite += _T("0");
				//	strcat_s(szDeviceExsit, sizeof(szDeviceExsit), strSubSite);
				//}
				int nExist = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TEST_SITE_1 + nPressIdx, nPickerX, nPickerY);

				if (nExist == DEF_EXIST) {
					strSubSite += _T("0");
					strcat_s(szDeviceExsit, sizeof(szDeviceExsit), strSubSite);
				}
				else {
					strSubSite += _T("1");
					strcat_s(szDeviceExsit, sizeof(szDeviceExsit), strSubSite);
				}

				if (nPickerX*nPickerY != MAX_TEST_PP_PKR_CNT - 1)
				{
					//strcat_s(szDeviceExsit, sizeof(szDeviceExsit), ",");
				}

				strSubSite.Empty();
				bIsFirst = FALSE;
			}
		}
	}
	int nGetSize = pBody->GetSize();
	sprintf_s(pBody->stData.szDataName[0], "%s", szDeviceExsit);

	int nErr = CIPCSock::Send(pPacket, FALSE);
	if (nErr == 0)
	{
		int nRealCamNo = nPressIdx;//Def_CamNoChangeRealNo(nPressIdx);
		ASSERT(nRealCamNo >= 0 && nRealCamNo < 8);
		m_VisionMsg[nRealCamNo].clear();
		m_VisionMsg[nRealCamNo].nMsgId = pPacket->pHead->stData.nMsgId;
		m_VisionMsg[nRealCamNo].nReceived = eVisionMsgWaitState_Waiting;

		nErr = ERR_NO_ERROR;

		CString strCamPos = (nStationNo >= VISION_CAM_ID_TESTSITE_DOWN_1) ? _T("Down") : _T("UP");
		SendLogMessage("[Send Measure] S2F41, Cam: %s %d,  MsgId : %d, CMD : %d", strCamPos, nRealCamNo + 1, pPacket->pHead->stData.nMsgId, pBody->stData.nMsgID);
		for (int i = 0; i < pBody->stData.nDataCount; i++)
		{
			SendLogMessage("[Send Measure] S2F41, Cam: %s %d, Param%d : %s", strCamPos, nRealCamNo + 1, i + 1, pBody->stData.szDataName[i]);
		}
	}
	else
	{
		if (nErr == -1) {
			nErr = ERR_VISION_SOCK_SEND_FAIL_INVALID_PACKET;
		}
		else if (nErr == -2) {
			nErr = ERR_VISION_SOCK_SEND_FAIL_NOT_ESTABLISHED;
		}
		else if (nErr == -3) {
			nErr = ERR_VISION_SOCK_SEND_FAIL_NOT_CONNECT;
		}
	}

	delete pPacket;
	pPacket = NULL;

	return nErr;
}
int CVisionComm::Cmd_VisionReady_HandCam(BOOL bCamState)
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = VISION_RCMD_REQ_INSP_READY;
	pBody->stData.nParamCount = 1;
	sprintf_s(pBody->stData.szParam[0], "%d", bCamState);

	int nErr = CIPCSock::Send(pPacket, FALSE);
	if (nErr == 0)
	{
		//if (pVisionMsg != NULL)
		//{
		//	_tVisionMsg* pWaitVisionMsg = new _tVisionMsg;
		//	pWaitVisionMsg->nMsgId = pPacket->pHead->stData.nMsgId;
		//	pWaitVisionMsg->nS = pPacket->pHead->stData.nS;
		//	pWaitVisionMsg->nF = pPacket->pHead->stData.nF;
		//	pWaitVisionMsg->nRcmd = pBody->stData.nCmd;
		//	pWaitVisionMsg->nReceived = eVisionMsgWaitState_Waiting;
		//	*pVisionMsg = *pWaitVisionMsg;
		//	m_csCmdDone.Lock();
		//	m_VisionCmdResult.push_back(pWaitVisionMsg);
		//	m_csCmdDone.Unlock();
		//}

		nErr = ERR_NO_ERROR;
		SendLogMessage("[snd] S2F41, MsgId : %d, CMD : %d", pPacket->pHead->stData.nMsgId, pBody->stData.nCmd);
		for (int i = 0; i<pBody->stData.nParamCount; i++)
		{
			SendLogMessage("[snd] Param%d : %s", i + 1, pBody->stData.szParam[i]);
		}
	}
	else
	{
		if (nErr == -1) {
			nErr = ERR_VISION_SOCK_SEND_FAIL_INVALID_PACKET;
		}
		else if (nErr == -2) {
			nErr = ERR_VISION_SOCK_SEND_FAIL_NOT_ESTABLISHED;
		}
		else if (nErr == -3) {
			nErr = ERR_VISION_SOCK_SEND_FAIL_NOT_CONNECT;
		}
	}

	delete pPacket;
	pPacket = NULL;

	return nErr;
}

int CVisionComm::Cmd_VisionReady_StationCam(BOOL bCamState, int nStationNo)
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = VISION_RCMD_REQ_INSP_READY;
	pBody->stData.nParamCount = 1;
	sprintf_s(pBody->stData.szParam[0], "%d", bCamState);
	sprintf_s(pBody->stData.szParam[1], "%d", nStationNo);

	int nErr = CIPCSock::Send(pPacket, FALSE);
	if (nErr == 0)
	{
		//if (pVisionMsg != NULL)
		//{
		//	_tVisionMsg* pWaitVisionMsg = new _tVisionMsg;
		//	pWaitVisionMsg->nMsgId = pPacket->pHead->stData.nMsgId;
		//	pWaitVisionMsg->nS = pPacket->pHead->stData.nS;
		//	pWaitVisionMsg->nF = pPacket->pHead->stData.nF;
		//	pWaitVisionMsg->nRcmd = pBody->stData.nCmd;
		//	pWaitVisionMsg->nReceived = eVisionMsgWaitState_Waiting;
		//	*pVisionMsg = *pWaitVisionMsg;
		//	m_csCmdDone.Lock();
		//	m_VisionCmdResult.push_back(pWaitVisionMsg);
		//	m_csCmdDone.Unlock();
		//}

		nErr = ERR_NO_ERROR;
		SendLogMessage("[snd] S2F41, MsgId : %d, CMD : %d", pPacket->pHead->stData.nMsgId, pBody->stData.nCmd);
		for (int i = 0; i < pBody->stData.nParamCount; i++)
		{
			SendLogMessage("[snd] Param%d : %s", i + 1, pBody->stData.szParam[i]);
		}
	}
	else
	{
		if (nErr == -1) {
			nErr = ERR_VISION_SOCK_SEND_FAIL_INVALID_PACKET;
		}
		else if (nErr == -2) {
			nErr = ERR_VISION_SOCK_SEND_FAIL_NOT_ESTABLISHED;
		}
		else if (nErr == -3) {
			nErr = ERR_VISION_SOCK_SEND_FAIL_NOT_CONNECT;
		}
	}

	delete pPacket;
	pPacket = NULL;

	return nErr;
}


int CVisionComm::Cmd_ReqMeasure_BarcodeLED(int nLEDOnOff, int nLEDIdx)
{
	//  [3/5/2025 donghyun.shin]아직 Barcode Led 기구물이 장착 되지 않아서 임시로 주석 처리 함
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = VISION_RCMD_REQ_BARCORDE_LIGHT_ON_OFF;
	//pBody->stData.nParamCount = 0; //미사용
	sprintf_s(pBody->stData.szParam[0], "%d", nLEDOnOff); //LED ON/OFF
	sprintf_s(pBody->stData.szParam[1], "%d", BARCODE_LIGHT_VISION_CHANNEL); //Vision Controller Channel
	
	int nErr = CIPCSock::Send(pPacket, FALSE);
	if (nErr == 0)
	{
		m_BarcodeLightMsg.clear();
		m_BarcodeLightMsg.nMsgId = pPacket->pHead->stData.nMsgId;
		m_BarcodeLightMsg.nReceived = eVisionMsgWaitState_Waiting;

		SendLogMessage("[snd] S2F41, LEDIdx : %d, MsgId : %d, CMD : %d", nLEDIdx +1, pPacket->pHead->stData.nMsgId, pBody->stData.nCmd);
		for (int i = 0; i < pBody->stData.nParamCount; i++)
		{
			SendLogMessage("[snd] Param%d : %s", i + 1, pBody->stData.szParam[i]);
		}
	}
	else
	{
		if (nErr == -1) {
			nErr = ERR_VISION_SOCK_SEND_FAIL_INVALID_PACKET;
		}
		else if (nErr == -2) {
			nErr = ERR_VISION_SOCK_SEND_FAIL_NOT_ESTABLISHED;
		}
		else if (nErr == -3) {
			nErr = ERR_VISION_SOCK_SEND_FAIL_NOT_CONNECT;
		}
	}

	delete pPacket;
	pPacket = NULL;

	return nErr;
}



int CVisionComm::Cmd_ReqMeasure(int nCamNo,_tVisionMsg* pVisionMsg/*=NULL*/)
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = VISION_RCMD_REQ_MEASURE;

	pBody->stData.nParamCount = 2;
	sprintf_s(pBody->stData.szParam[0], sizeof(pBody->stData.szParam[0]), "%d", nCamNo);
	
	char szDeviceExsit[64]={0,};
	if( m_nVisionPcIdx == eVision_BarCode )
	{
		if( (nCamNo >= VISION_CAM_ID_2DBRACODE_1) || (nCamNo <= VISION_CAM_ID_2DBRACODE_4) )
		{
			int nIdx = nCamNo - VISION_CAM_ID_2DBRACODE_1;
			for(int y=0; y<MAX_LD_TBL_POCKET_CNT_Y; y++)
			{
				char szTemp[128]={0,};	
				for(int x=0; x<MAX_LD_TBL_POCKET_CNT_X/2; x++) // 좌측 2열만 vision check 함.
				{
					int nExist = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_LD_TBL_1+nIdx, x, y);
// 					ST_DD_DEVICE tDeviceData;
// 					g_DMCont.m_dmDevice.GetDeviceData(eDeviceStage_LD_TBL_1+nIdx, x, y, &tDeviceData);
// 					if( tDeviceData.sExist == DEF_EXIST )
					if( nExist == DEF_EXIST )
					{
						strcat_s(szTemp, sizeof(szTemp), "1");
					}
					else
					{
						strcat_s(szTemp, sizeof(szTemp), "0");
					}
				}
				strcat_s(szTemp, sizeof(szTemp), ";");
				strcat_s(szDeviceExsit, sizeof(szDeviceExsit), szTemp);
			}
			strcat_s(szDeviceExsit, sizeof(szDeviceExsit), "00");
		}
	}
	else
	{
		int nIdx = -1;
		if( nCamNo >= VISION_CAM_ID_TESTSITE_DOWN_1 && nCamNo <= VISION_CAM_ID_TESTSITE_DOWN_8 )
			nIdx = nCamNo - VISION_CAM_ID_TESTSITE_DOWN_1;
// 		else if( nCamNo >= VISION_CAM_ID_TESTSITE_UP_1 && nCamNo <= VISION_CAM_ID_TESTSITE_UP_8 )
// 			nIdx = nCamNo - VISION_CAM_ID_TESTSITE_UP_1 + ePressUnit_Up_1;

		BOOL bIsFirst = TRUE;
		CString strSubSite =_T("");

		int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
		int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

		for(int nY=0; nY < nSiteDivY; nY++ )
		{
			for(int nX=0; nX < nSiteDivX; nX++ )
			{
				int nExist = g_DMCont.m_dmDevice.GetDeviceExist(eDeviceStage_TEST_SITE_1+nIdx, nX, nY);
				int nSiteAddr = NDM11_SocketOnOff_Site1_1+(STATION_MAX_PARA*nIdx)+nX + nY*STATION_MAX_X_SITE;
				int nSiteOnOff =  g_DMCont.m_dmHandlerOpt.GN( NDM11_SocketOnOff_Site1_1 + nSiteAddr );

				if(bIsFirst == FALSE){ strSubSite += _T(","); }

				if( nExist == DEF_NOT_EXIST && (nSiteOnOff == DEF_SITE_ON || nSiteOnOff == DEF_SITE_OFF_RESERVATION || nSiteOnOff == DEF_SITE_OFF_RESERVATION_YIELD)){
					strSubSite += _T("1");
					strcat_s(szDeviceExsit, sizeof(szDeviceExsit), strSubSite);
				}else{
					strSubSite += _T("0");
					strcat_s(szDeviceExsit, sizeof(szDeviceExsit), strSubSite);
				}
				strSubSite.Empty();
				bIsFirst = FALSE;
			}
		}
	}
	sprintf_s(pBody->stData.szParam[1], "%s", szDeviceExsit);

	int nErr = CIPCSock::Send(pPacket, FALSE);
	if( nErr == 0 )
	{
		int nRealCamNo = Def_CamNoChangeRealNo(nCamNo);
		ASSERT(nRealCamNo>=0 && nRealCamNo<8);
		m_VisionMsg[nRealCamNo].clear();
		m_VisionMsg[nRealCamNo].nMsgId = pPacket->pHead->stData.nMsgId;
		m_VisionMsg[nRealCamNo].nReceived = eVisionMsgWaitState_Waiting;

		nErr = ERR_NO_ERROR;

		CString strCamPos = (nCamNo >= VISION_CAM_ID_TESTSITE_DOWN_1) ? _T("Down"): _T("UP");
		SendLogMessage("[Send Measure] S2F41, Cam: %s %d,  MsgId : %d, CMD : %d", strCamPos,nRealCamNo+1, pPacket->pHead->stData.nMsgId, pBody->stData.nCmd);
		for(int i=0; i<pBody->stData.nParamCount; i++)
		{
			SendLogMessage("[Send Measure] S2F41, Cam: %s %d, Param%d : %s", strCamPos, nRealCamNo+1, i+1, pBody->stData.szParam[i]);
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

 	return nErr;
}

//int CVisionComm::GetCmdDone(_tVisionMsg* pVisionMsg)
//{
//	m_csCmdDone.Lock();
//
//	int nMsgCount = m_VisionCmdResult.size();
//	//AfxTrace("First Delete Data List Count=%d, Idx=%d\n", nMsgCount, m_nVisionPcIdx);
//	if( nMsgCount == 0 )
//	{
//		m_csCmdDone.Unlock();
//		SendLogMessage("m_VisionCmdResult count = %d, eVisionMsgWaitState_Canceled",m_VisionCmdResult.size());
//		return eVisionMsgWaitState_Canceled;
//	}
//	for(int i=0; i<nMsgCount; i++)
//	{
//		//AfxTrace("(%d) Delete Data List Count=%d\n",m_nVisionPcIdx, m_VisionCmdResult.m_fnGetCount());
//		_tVisionMsg* pItem = (_tVisionMsg*)m_VisionCmdResult[i];
//		if( pItem->nMsgId == pVisionMsg->nMsgId)
//		{
//			if( pItem->nReceived == eVisionMsgWaitState_Received)
//			{
//				*pVisionMsg = *pItem; // 데이터를 복사해 주고 리스트에서 삭제한다.
//				m_VisionCmdResult.erase(m_VisionCmdResult.begin()+i);
//				i--;
//				SendLogMessage("m_VisionCmdResult count = %d, pVisionMsg->vReqData count = %d, pItem->vReqData count = %d",m_VisionCmdResult.size(),(int)pVisionMsg->vReqData.size(),(int)pItem->vReqData.size());
//				//AfxTrace("(%d) Delete Data MsgID=%d\n", m_nVisionPcIdx, pVisionMsg->nMsgId);
//				m_csCmdDone.Unlock();
//				return eVisionMsgWaitState_Received;
//			}
//			else if( pItem->nReceived == eVisionMsgWaitState_Canceled )
//			{
//				m_VisionCmdResult.erase(m_VisionCmdResult.begin()+i);
//				i--;
//				m_csCmdDone.Unlock();
//				return eVisionMsgWaitState_Canceled;
//			}
//		}
//	}
//
//	m_csCmdDone.Unlock();
//
//	return eVisionMsgWaitState_Waiting;
//}

int CVisionComm::GetCmdDone(int nCamNo)
{
	//m_csCmdDone.Lock();
	int nRealCamNo = nCamNo;
		
		//Def_CamNoChangeRealNo(nCamNo);
	//if( m_VisionMsg[nRealCamNo].vReqData.size() == 0)
	//{
	//	m_csCmdDone.Unlock();
	//	SendLogMessage("MSG_ID : %d, eVisionMsgWaitState_Canceled, m_VisionMsg[%d].vReqData Data Count = %d",m_VisionMsg[nRealCamNo].nMsgId,nRealCamNo,(int)m_VisionMsg[nRealCamNo].vReqData.size());
	//	return eVisionMsgWaitState_Canceled;
	//}

	if( m_VisionMsg[nRealCamNo].nReceived == eVisionMsgWaitState_Received)
	{
		SendLogMessage("MSG_ID : %d, eVisionMsgWaitState_Received, m_VisionMsg[%d].vReqData Data Count = %d",m_VisionMsg[nRealCamNo].nMsgId,nRealCamNo,(int)m_VisionMsg[nRealCamNo].nDataCount);
		return eVisionMsgWaitState_Received;
	}
	else if( m_VisionMsg[nRealCamNo].nReceived == eVisionMsgWaitState_Canceled )
	{
		SendLogMessage("MSG_ID : %d, eVisionMsgWaitState_Canceled, m_VisionMsg[%d].vReqData Data Count = %d",m_VisionMsg[nRealCamNo].nMsgId,nRealCamNo,(int)m_VisionMsg[nRealCamNo].nDataCount);
		return eVisionMsgWaitState_Canceled;
	}

	//m_csCmdDone.Unlock();

	return eVisionMsgWaitState_Waiting;
}


void CVisionComm::SendLogMessage(const char* fmt, ...)
{
	char szEventMessage[1024]={0,};
	va_list va;
	va_start(va, fmt);
	vsprintf_s(szEventMessage, sizeof(szEventMessage), fmt, va);
	va_end(va);

	m_Logger.LOG(szEventMessage);
// 	int nObserverCount = GetObserverCount();
// 	for(int i=0; i<nObserverCount; i++){
// 		CWnd* pWnd = GetObserverByIndex(i);
// 		pWnd->SendMessage(UM_VISION_IF_LOG_MSG, (WPARAM)m_nVisionPcIdx, (LPARAM)szEventMessage);
// 	}
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-11-04]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CVisionComm::AttachObserver(CWnd* pObserver)
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
int CVisionComm::DetachObserver(CWnd* pObserver)
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
void CVisionComm::ClearObserver()
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
int CVisionComm::GetObserverCount()
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
CWnd* CVisionComm::GetObserverByIndex(int nIndex)
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
BOOL CVisionComm::CheckDuplicate(CWnd* pItem)
{
	m_csObserver.Lock();
	POSITION pos = m_listObserver.GetHeadPosition();
	int nCount = m_listObserver.GetCount();
	for(int i=0; i<nCount; i++){
		CWnd* pItem = (CWnd*)m_listObserver.GetNext(pos);
		if( pItem != NULL ){
	//		if( pItem == pItem){
				m_csObserver.Unlock();
				return TRUE;
	//		}
		}
	}
	m_csObserver.Unlock();
	return FALSE;
}

/*
*/
int CVisionComm::Cmd_LaserOnOff(int nCamNo, BOOL bOnOff ,_tVisionMsg* pVisionMsg/*=NULL*/, BOOL bRevChk)
{
	CPacket* pPacket = new CPacket(2, 41);
	CPacketBody_S2F41* pBody = (CPacketBody_S2F41*)pPacket->pBody;
	pBody->stData.nCmd = (bOnOff == DEF_ON ? VISION_RCMD_REQ_LASER_ON : VISION_RCMD_REQ_LASER_OFF);
	pBody->stData.nParamCount = 1;
	sprintf_s(pBody->stData.szParam[0], sizeof(pBody->stData.szParam[0]), "%d", nCamNo);

	int nErr = CIPCSock::Send(pPacket, FALSE);
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
			m_csCmdDone.Lock();
			m_VisionCmdResult.push_back(pWaitVisionMsg);
			m_csCmdDone.Unlock();
		}
		nErr = ERR_NO_ERROR;

		SendLogMessage("[Send Laser %s] S2F41, Cam:%d MsgId : %d, CMD : %d", (bOnOff==DEF_ON ? _T("On") : _T("Off")), nCamNo, pPacket->pHead->stData.nMsgId, pBody->stData.nCmd);
		for(int i=0; i<pBody->stData.nParamCount; i++)
		{
			SendLogMessage("[Send Laser %s] S2F41, Cam:%d, Param%d : %s", (bOnOff==DEF_ON ? _T("On") : _T("Off")), nCamNo, i+1, pBody->stData.szParam[i]);
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

	return nErr;
}

int CVisionComm::Cmd_ReqMeasureResult(int nCamNo,_tVisionMsg* pVisionMsg/*=NULL*/)
{
	//ASSERT(nCamNo >= VISION_CAM_ID_TESTSITE_UP_1 && nCamNo <= VISION_CAM_ID_TESTSITE_DOWN_8);

// 	int nIdx = nCamNo - VISION_CAM_ID_TEST_HAND_1;
// 
// 	CPacket* pPacket = new CPacket(2,3);
// 	CPacketBody_S2F3* pBody = (CPacketBody_S2F3*)pPacket->pBody;
// 			
// 	pBody->stData.nMsgID = VISION_REQ_RESULT_TEST_HAND_1_EMPTY;
// 	pBody->stData.nDataCount = 1;
// 	sprintf_s(pBody->stData.szDataName[0], sizeof(pBody->stData.szDataName[0]), "%d", VISION_DATAID_CAM_BUSY_TEST_HAND_(nIdx));
// 	//sprintf_s(pBody->stData.szDataName[1], sizeof(pBody->stData.szDataName[1]), "%d", VISION_DATAID_RESULT_DATA_POCKET_DOWN_(nIdx));
// 
 //	int nErr = CIPCSock::Send(pPacket, FALSE);
// 	if( nErr == 0 )
// 	{
// 		int nRealCamNo = Def_CamNoChangeRealNo(nCamNo);
// 		ASSERT(nRealCamNo>=0 && nRealCamNo<8);
// 		m_VisionMsg[nRealCamNo].clear();
// 		m_VisionMsg[nRealCamNo].nMsgId = pPacket->pHead->stData.nMsgId;
// 		m_VisionMsg[nRealCamNo].nReceived = eVisionMsgWaitState_Waiting;
// 		
// 		nErr = ERR_NO_ERROR;
// 		SendLogMessage("[snd] S2F3, MsgId : %d", pPacket->pHead->stData.nMsgId);
// 		for(int i=0; i<pBody->stData.nDataCount; i++)
// 		{
// 			SendLogMessage("DATAID%d : %s", i+1, pBody->stData.szDataName[i]);
// 		}
// 	}
// 	else
// 	{
// 		if( nErr == -1 ){
// 			nErr = ERR_VISION_SOCK_SEND_FAIL_INVALID_PACKET;
// 		}else if( nErr == -2 ){
// 			nErr = ERR_VISION_SOCK_SEND_FAIL_NOT_ESTABLISHED;
// 		}else /*if( nErr == -3 )*/{
// 			nErr = ERR_VISION_SOCK_SEND_FAIL_NOT_CONNECT;
// 		}
// 	}
// 
// 	delete pPacket;
// 	pPacket = NULL;

//	return nErr;
	return 0;
}

int CVisionComm::Def_CamNoChangeRealNo(int nCamNo)
{
	//ASSERT(nCamNo >= VISION_CAM_ID_TESTSITE_DOWN_1&& nCamNo <= VISION_CAM_ID_TESTSITE_UP_1);

	int nRealCamNo = -1;
	switch(nCamNo)
	{
	case VISION_CAM_ID_TESTSITE_UP_1 :
	case VISION_CAM_ID_TESTSITE_DOWN_1:
		{
			nRealCamNo = 0;
		}break;
	case VISION_CAM_ID_TESTSITE_UP_2:
	case VISION_CAM_ID_TESTSITE_DOWN_2:
		{
			nRealCamNo = 1;
		}break;
	case VISION_CAM_ID_TESTSITE_UP_3 :
	case VISION_CAM_ID_TESTSITE_DOWN_3:
		{
			nRealCamNo = 2;
		}break;
	case VISION_CAM_ID_TESTSITE_UP_4:
	case VISION_CAM_ID_TESTSITE_DOWN_4:
		{
			nRealCamNo = 3;
		}break;
	case VISION_CAM_ID_TESTSITE_UP_5:
	case VISION_CAM_ID_TESTSITE_DOWN_5:
		{
			nRealCamNo = 4;
		}break;
	case VISION_CAM_ID_TESTSITE_UP_6:
	case VISION_CAM_ID_TESTSITE_DOWN_6:
		{
			nRealCamNo = 5;
		}break;
	case VISION_CAM_ID_TESTSITE_UP_7:
	case VISION_CAM_ID_TESTSITE_DOWN_7:
		{
			nRealCamNo = 6;
		}break;
	case VISION_CAM_ID_TESTSITE_UP_8:
	case VISION_CAM_ID_TESTSITE_DOWN_8:
		{
			nRealCamNo = 7;
		}break;
	case VISION_CAM_ID_TEST_HAND_1:
		{
			nRealCamNo = 0;
		}break;
	case VISION_CAM_ID_TEST_HAND_2:
		{
			nRealCamNo = 1;
		}break;
	}

	return nRealCamNo;
}

int CVisionComm::Def_StationNoChangeMessageNo(int nStationNo)
{
	//ASSERT(nCamNo >= VISION_CAM_ID_TESTSITE_DOWN_1&& nCamNo <= VISION_CAM_ID_TESTSITE_UP_1);

	int nMessageNo = -1;
	switch (nStationNo)
	{
	case STATION_1_SITE:
	{
		nMessageNo = VISION_REQ_MEASURE_STATION_1_EMPTY;
	}break;
	case STATION_2_SITE:
	{
		nMessageNo = VISION_REQ_MEASURE_STATION_2_EMPTY;
	}break;
	case STATION_3_SITE:
	{
		nMessageNo = VISION_REQ_MEASURE_STATION_3_EMPTY;
	}break;
	case STATION_4_SITE:
	{
		nMessageNo = VISION_REQ_MEASURE_STATION_4_EMPTY;
	}break;
	case STATION_5_SITE:
	{
		nMessageNo = VISION_REQ_MEASURE_STATION_5_EMPTY;
	}break;
	case STATION_6_SITE:
	{
		nMessageNo = VISION_REQ_MEASURE_STATION_6_EMPTY;
	}break;
	case STATION_7_SITE:
	{
		nMessageNo = VISION_REQ_MEASURE_STATION_7_EMPTY;
	}break;
	case STATION_8_SITE:
	{
		nMessageNo = VISION_REQ_MEASURE_STATION_8_EMPTY;
	}break;
	}

	return nMessageNo;
}

// int CVisionComm::Def_CamNoChangeRealNo(int nStationNo)
// {
// 	//ASSERT(nCamNo >= VISION_CAM_ID_TESTSITE_DOWN_1&& nCamNo <= VISION_CAM_ID_TESTSITE_UP_1);
// 
// 	int nRealCamNo = -1;
// 	switch (nStationNo)
// 	{
// 	case STATION_1_SITE:
// 	case STATION_5_SITE:
// 	{
// 		nRealCamNo = STATION_1_SITE;
// 	}break;
// 	case STATION_2_SITE:
// 	case STATION_6_SITE:
// 	{
// 		nRealCamNo = STATION_2_SITE;
// 	}break;
// 	case STATION_3_SITE:
// 	case STATION_7_SITE:
// 	{
// 		nRealCamNo = STATION_3_SITE;
// 	}break;
// 	case STATION_4_SITE:
// 	case STATION_8_SITE:
// 	{
// 		nRealCamNo = STATION_4_SITE;
// 	}break;
// 	}
// 	return nRealCamNo;
// }

int CVisionComm::GetCmdResultData(int nStationIdx, int* nRevData/* = NULL*/,CString& strRevPosResult/*=NULL*/)
{
	if( nRevData != NULL)
	{
		nRevData[eResData_DataCnt] = m_VisionMsg[nStationIdx].nDataCount;
		nRevData[eResData_ResultStatus] = m_VisionMsg[nStationIdx].nStatus;
		nRevData[eResData_Error_Discription] = atoi(m_VisionMsg[nStationIdx].szData[0]);
		//nRevData[eResData_ResultPos] = atoi(m_VisionMsg[nHandIdx].szData[2]);
		strRevPosResult.Format("%s",m_VisionMsg[nStationIdx].szData[1]);
	}	

	return m_VisionMsg[nStationIdx].nRcmdAck;
}


int CVisionComm::ConvertDataIdToVatCam(int nDataId)
{
	int nCam = -1;
	switch (nDataId)
	{
	case VISION_MEASURE_VAT_LD_UPPER:			nCam = VAT_CAM_LD_TRAY_PP_UPPER; break;
	case VISION_MEASURE_VAT_LD_LOWER:			nCam = VAT_CAM_LD_TRAY_PP_LOWER; break;
	case VISION_MEASURE_VAT_ULD_UPPER:			nCam = VAT_CAM_UD_TRAY_PP_UPPER; break;
	case VISION_MEASURE_VAT_ULD_LOWER:			nCam = VAT_CAM_UD_TRAY_PP_LOWER; break;
	case VISION_MEASURE_VAT_TEST_1_LD_UPPER:	nCam = VAT_CAM_LD_TEST_PP_1_UPPER; break;
	case VISION_MEASURE_VAT_TEST_1_ULD_UPPER:	nCam = VAT_CAM_UD_TEST_PP_1_UPPER; break;
	case VISION_MEASURE_VAT_TEST_1_LOWER: 		nCam = VAT_CAM_TEST_PP_1_LOWER; break;
	case VISION_MEASURE_VAT_TEST_2_LD_UPPER:	nCam = VAT_CAM_LD_TEST_PP_2_UPPER; break;
	case VISION_MEASURE_VAT_TEST_2_ULD_UPPER:	nCam = VAT_CAM_UD_TEST_PP_2_UPPER; break;
	case VISION_MEASURE_VAT_TEST_2_LOWER: 		nCam = VAT_CAM_TEST_PP_2_LOWER; break;
	default:
		ASSERT("nWorkPos invalid" == 0);
		return -1;
	}

	return nCam;
}
