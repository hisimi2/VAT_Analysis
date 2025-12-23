#include "StdAfx.h"
#include "IFComm.h"


CIFComm::CIFComm(void)
{
	m_pClientWnd = NULL;
	m_pParentWnd = NULL;
	m_hClientWnd = NULL;
	m_bConnected = FALSE;
	m_bStopRcvCmdProc = FALSE;
	m_bStopSndCmdProc = FALSE;
	m_bTerminatedRcvCmdProc = FALSE;
	m_bTerminatedSndCmdProc = FALSE;

	m_pThreadRcv = NULL;
	m_pThreadSnd = NULL;

	ZeroMemory(m_szInitFileName, sizeof(m_szInitFileName));

	m_pCSRcv = new CRITICAL_SECTION;
	::InitializeCriticalSection( m_pCSRcv );

	m_pCSSnd = new CRITICAL_SECTION;
	::InitializeCriticalSection( m_pCSSnd );
	m_IF_Type = 0;
	m_nTIF_StdPara = 0;
	
	memset(m_MessageMap,0x00,sizeof(m_MessageMap));

}


CIFComm::~CIFComm(void)
{
	ClearRcvCMD();
	ClearSndCMD();

	if( m_pCSRcv != NULL ){
		::DeleteCriticalSection( m_pCSRcv );
		delete m_pCSRcv; m_pCSRcv = NULL;
	}
	if( m_pCSSnd != NULL ){
		::DeleteCriticalSection( m_pCSSnd );
		delete m_pCSSnd; m_pCSSnd = NULL;
	}
}

int	CIFComm::Initialize(CWnd* pParentWnd, HWND hClientWnd, CWnd* pClientWnd, ST_COMM_DATA stCommData, ST_TESTER_MESSAGE_MAP* pMessageMap )
{
	int nRet = this->Init_Board();

	if( nRet != 0 )
	{
		CString strMsg;
		strMsg.Format( _T( "%s class is init board fail!! \nWinError : %d" ), this->GetRuntimeClass()->m_lpszClassName, nRet );
		AfxMessageBox( strMsg );
		return nRet;
	}

	// init file path
	sprintf_s(m_szInitFileName, sizeof(m_szInitFileName), "%s", stCommData.szInitFileName);
	CConfigData ConfigData(m_szInitFileName);
	char szBuff[512]={0,};
	char szLogFileName[512]={0,};

	// LOG File Name
	ConfigData.GetString("TESTER CONFIG", "Log File Name", "IF_Tester.log", szBuff);
	sprintf_s(szLogFileName, sizeof(szLogFileName), "%s%s", SZ_LOG_FILE_PATH, szBuff);
	m_Logger.SetFileName(szLogFileName);	

	m_nTIF_StdPara = ConfigData.GetInt("TESTER CONFIG", "Tester Standard Para", "32");	// Tester Standard Para

	m_pParentWnd = pParentWnd;
	m_hClientWnd = hClientWnd;
	m_pClientWnd = pClientWnd;
	m_stCommData = stCommData;

	for( int i = 0; ; i++ )
	{
		if( pMessageMap[ i ].fnReceiveCallback == NULL )
			break;

		m_MessageMap[ i ] = pMessageMap[ i ];
	}

	m_bStopRcvCmdProc = FALSE;
	m_bStopSndCmdProc = FALSE;

	m_pThreadRcv = AfxBeginThread( CIFComm::m_fnRcvPacketProc, this );
	m_pThreadSnd = AfxBeginThread( CIFComm::m_fnSndPacketProc, this );

	return ERROR_SUCCESS;
}

void CIFComm::Finalize()
{
	this->Final_Board();

	m_bStopRcvCmdProc = TRUE;
	m_bStopSndCmdProc = TRUE;

	int nCount = 0;
	if( m_pThreadRcv != NULL ){
		while( ::WaitForSingleObject( m_evtThreadTerminated_Rcv, 1 ) == WAIT_TIMEOUT ){
			nCount++;
			if( nCount > 500 ){
				::TerminateThread( m_pThreadRcv->m_hThread, 0);
				break;
			}
		}
	}
	m_pThreadRcv = NULL;

	nCount = 0;
	if( m_pThreadSnd != NULL ){
		while( ::WaitForSingleObject( m_evtThreadTerminated_Snd, 1 ) == WAIT_TIMEOUT ){
			nCount++;
			if( nCount > 500 ){
				::TerminateThread( m_pThreadSnd->m_hThread, 0);
				break;
			}
		}
	}
	m_pThreadSnd = NULL;
}

UINT CIFComm::m_fnRcvPacketProc(void* lpVoid)	// receive CMD 처리
{	
	CIFComm* pSelf = (CIFComm*)lpVoid;
	while(1){
		CTesterIFCmd* pPacket  = NULL;

		pSelf->GetBoardRcvCMD(); //무엇인가 읽을 것이 있는지에 대해서 확인.

		if( pSelf->m_bStopRcvCmdProc == TRUE ) break;

		if( pSelf->GetRcvCMDCount() != 0 ){
			pSelf->PopRcvCMD();
		}

		Sleep(1);
	}
	pSelf->m_bTerminatedRcvCmdProc = TRUE;
	pSelf->m_evtThreadTerminated_Rcv.SetEvent();

	return 0;
}

UINT CIFComm::m_fnSndPacketProc(void* lpVoid)	// 전송 할 Packet 처리
{
	CIFComm* pSelf = (CIFComm*)lpVoid;
	while(1){

		CTesterIFCmd* pPacket = NULL;

		if( pSelf->m_bStopSndCmdProc == TRUE ) break;

		int nCount = pSelf->GetSndCMDCount();
		while( nCount-- )
		{
			pSelf->PopSndCMD();
		}
		Sleep(1);
	}

	pSelf->m_bTerminatedSndCmdProc = TRUE;
	pSelf->m_evtThreadTerminated_Snd.SetEvent();

	return 0;
}

bool CIFComm::IsConnected()
{
	return m_bConnected;
}

void CIFComm::AddRcvCMD(CTesterIFCmd* pPacket)
{
	::EnterCriticalSection( m_pCSRcv );
	m_QRcv.AddTail( pPacket );
	m_Logger.LOG("Rcv Command Add : [%s] Rcv data : [%s]", pPacket->strCmd, pPacket->strRcvData);
	::LeaveCriticalSection( m_pCSRcv );
}

int CIFComm::GetRcvCMDCount()
{
	::EnterCriticalSection( m_pCSRcv );
	int nCount = m_QRcv.GetCount();
	::LeaveCriticalSection( m_pCSRcv );

	return nCount;
}

CTesterIFCmd* CIFComm::GetRcvCMD()
{
	::EnterCriticalSection( m_pCSRcv );

	CTesterIFCmd* pPacket = NULL;
	if( m_QRcv.GetCount() == 0 ){
		::LeaveCriticalSection( m_pCSRcv );
		return NULL;
	}
	pPacket = m_QRcv.GetHead();

	::LeaveCriticalSection( m_pCSRcv );

	return pPacket;
}

void CIFComm::DelRcvCMD()
{
	::EnterCriticalSection( m_pCSRcv );

	if( m_QRcv.GetCount() == 0 ){
		::LeaveCriticalSection( m_pCSRcv );
		return;
	}

	CTesterIFCmd* pPacket = NULL;
	pPacket = m_QRcv.GetHead();

	if( pPacket != NULL ){
		delete pPacket; 
		pPacket = NULL;
	}
	m_QRcv.RemoveHead();

	::LeaveCriticalSection( m_pCSRcv );
}

void CIFComm::ClearRcvCMD()
{
	::EnterCriticalSection( m_pCSRcv );

	CTesterIFCmd* pPacket = NULL;

	while( !m_QRcv.IsEmpty() ){
		pPacket = NULL;
		pPacket = m_QRcv.GetHead();
		if( pPacket != NULL ){
			delete pPacket;
			pPacket = NULL;
		}
		m_QRcv.RemoveHead();
	}

	::LeaveCriticalSection( m_pCSRcv );
}

void CIFComm::PopRcvCMD()
{
	::EnterCriticalSection( m_pCSRcv );

	CTesterIFCmd* pPacket = NULL;
	if( m_QRcv.GetCount() == 0 ){
		::LeaveCriticalSection( m_pCSRcv );
		return;
	}

	pPacket = m_QRcv.GetHead();
	if( pPacket != NULL ){
		SendToClient( pPacket );

		delete pPacket;
		pPacket = NULL;

		m_QRcv.RemoveHead();
	}

	::LeaveCriticalSection( m_pCSRcv );
}

void CIFComm::AddSndCMD_Fast(CTesterIFCmd* pPacket)
{
	::EnterCriticalSection( m_pCSSnd );
	m_QSnd.AddHead( pPacket );
	::LeaveCriticalSection( m_pCSSnd );	
}

void CIFComm::AddSndCMD(CTesterIFCmd* pPacket)
{
	::EnterCriticalSection( m_pCSSnd );
	m_QSnd.AddTail( pPacket );
	::LeaveCriticalSection( m_pCSSnd );	
}

int CIFComm::GetSndCMDCount()
{
	::EnterCriticalSection( m_pCSSnd );
	int nCount = m_QSnd.GetCount();
	::LeaveCriticalSection( m_pCSSnd );

	return nCount;
}

CTesterIFCmd* CIFComm::GetSndCMD()
{
	::EnterCriticalSection( m_pCSSnd );

	CTesterIFCmd* pPacket = NULL;
	if( m_QSnd.GetCount() == 0 ){
		::LeaveCriticalSection( m_pCSSnd );
		return NULL;
	}
	pPacket = m_QSnd.GetHead();

	::LeaveCriticalSection( m_pCSSnd );

	return pPacket;
}

void CIFComm::DelSndCMD()
{
	::EnterCriticalSection( m_pCSSnd );

	if( m_QSnd.GetCount() == 0 ){
		::LeaveCriticalSection( m_pCSSnd );
		return;
	}

	CTesterIFCmd* pPacket = NULL;
	pPacket = m_QSnd.GetHead();
	if( pPacket != NULL ){
		delete pPacket;
		pPacket = NULL;
	}
	m_QSnd.RemoveHead();

	::LeaveCriticalSection( m_pCSSnd );
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : m_fnClearSndCMD
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CIFComm::ClearSndCMD()
{
	::EnterCriticalSection( m_pCSSnd );

	CTesterIFCmd* pPacket = NULL;

	while( !m_QSnd.IsEmpty() ){
		pPacket = m_QSnd.GetHead();
		if( pPacket != NULL ){
			delete pPacket;
			pPacket = NULL;
		}
		m_QSnd.RemoveHead();
	}

	::LeaveCriticalSection( m_pCSSnd );
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : m_fnPopSndCMD
  PARAMS   : None
  RETURN   : None
  COMMENTS : Pop Send Command 
==============================================================================*/
void CIFComm::PopSndCMD()
{
	::EnterCriticalSection( m_pCSSnd );

	CTesterIFCmd* pPacket = NULL;
	if( m_QSnd.GetCount() == 0 ){
		::LeaveCriticalSection( m_pCSSnd );
		return;
	}
	pPacket = m_QSnd.GetHead();
	if( pPacket != NULL ){
		do_SendCMD( *pPacket );

		delete pPacket;
		pPacket = NULL;

		m_QSnd.RemoveHead();
	}

	::LeaveCriticalSection( m_pCSSnd );
}

int	CIFComm::SendToClient(CTesterIFCmd* pPacket)
{
	if( pPacket == NULL ) return ERROR_INVALID_DATA;

	//CString strCmd = pPacket->strCmd;
	m_Logger.LOG("Snd Command Client : [%s]", pPacket->strCmd);


	int nIndex = 0;
	while( m_MessageMap[ nIndex ].fnReceiveCallback != NULL )
	{
		if( m_MessageMap[ nIndex ].strCmd == pPacket->strCmd )
		{
			m_MessageMap[ nIndex ].fnReceiveCallback( pPacket );
			break;
		}
		nIndex++;
	}

	return ERROR_SUCCESS;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : m_fnMakeFullsite
  PARAMS   : cParamList : 0번에는 para수가 와야한다.
             1~ 해당Site Enable/Disable 정보
  RETURN   : CTesterIFCmd 에 SndData에 str을 만들어서 Return
  COMMENTS : Site 및 Enable/Disable에 따라 Fullsite를 만든다.
==============================================================================*/
CTesterIFCmd* CIFComm::m_fnMakeFullsite(CParamList& cParamList)
{
	CTesterIFCmd* cIFCmd = new CTesterIFCmd;
	int nPara     = cParamList.m_fnGetItemInt(0);
	int nTotalCnt = cParamList.m_fnGetCount();
	int nDataBit  = 0;

	char szSiteMap[1024] = {0,};
	sprintf_s(szSiteMap,sizeof(szSiteMap),"Fullsites ");

	int nNeedZero = (m_nTIF_StdPara-nPara)/4;		// Tester Standard에 맞추어 기준만큼 setting을하여 보내야한다.
	for(int i = 0; i < nNeedZero; i++){
		strcat_s(szSiteMap,sizeof(szSiteMap),"0");
	}

	int* pnSiteEnable = new int[nTotalCnt];
	memset(pnSiteEnable, 0x00, nTotalCnt);

	for(int nCnt = (nPara-1); nCnt >= 0 ; nCnt--)	// Start Max
	{
		int nHexBit   = 1;	// Shift Bit
		pnSiteEnable[nCnt] = cParamList.m_fnGetItemInt(nCnt+1);

		if(pnSiteEnable[nCnt]){					// Site On : 해당 자리 Bit를 1로 만든다. ex)Site3=0100, Site2=0010
			nHexBit  = (nHexBit << (nCnt%4));	
		}else{
			nHexBit = 0;						// Site Off : 해당 자리 Bit를 0
		}
		
		nDataBit += nHexBit;	// 해당 자리에 대해서 1로 만듬.

		if( (nCnt%4) == 0 )
		{
			char dt[2]={0,};
			sprintf_s(dt,sizeof(dt),"%X",nDataBit);
			strcat_s(szSiteMap,sizeof(szSiteMap),dt);
			nDataBit = 0;
		}
	}
	
	delete [] pnSiteEnable;           
	pnSiteEnable = NULL;

	cIFCmd->strCmd = TESTER_CMD_HTOT_FULLSITE;
	cIFCmd->strSndData = szSiteMap;

	return cIFCmd;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : m_fnMakeEcho
  PARAMS   : cParamList
             0번 :: Binon으로 들어온 str RcvData가 들어있어야 한다.
  RETURN   : Bin on에 응답하는 Echo를 만들어서 SndData에 저장.
  COMMENTS : BinOn 후 Echo 하는 함수.
==============================================================================*/
CTesterIFCmd* CIFComm::m_fnMakeEcho(CParamList& cParamList)
{
	CTesterIFCmd* cIFCmd = new CTesterIFCmd;
	char szSendData[2048] = {0,};

	sprintf_s(szSendData, sizeof(szSendData), "ECHO:");
	CString strRcvData = cParamList.m_fnGetItemString(0);

	if(-1!=strRcvData.Find("\r"))	//뒤에있는 종결기를 짜른다..
	{
		int a = strRcvData.Find('\r');
		strRcvData.SetAt(a, '\0');
	}

	strRcvData.Delete(0,6);	// remove text (BINON:)
	strcat_s(szSendData, sizeof(szSendData), strRcvData);

	cIFCmd->strCmd = TESTER_CMD_HTOT_ECHO;
	cIFCmd->strSndData = szSendData;

	return cIFCmd;
}