#include "StdAfx.h"
#include "LBaseSock.h"
#include "TechwingSock.h"
#include "IPCSock_Imp.h"

#include "NoCWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	DEF_NOT_EMPTY (0)

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CIPCSock_Imp::CIPCSock_Imp(void)
{
	m_nSocketType              = 0;
	m_nTmOutMilliSec           = 30000;
	m_pClientSock              = NULL;

	m_fnEventCallback          = NULL;
	m_ClientHWnd               = NULL;

	m_bStatusConnect           = FALSE;
	m_bStopRcvPacketProc       = FALSE;
	m_bStopSndPacketProc       = FALSE;
	m_bTimeOutProc             = FALSE;
	m_bTerminatedRcvPacketProc = FALSE;
	m_bTerminatedSndPacketProc = FALSE;
	m_bCsIsLock                  = FALSE;

	m_nSplitCount = 0;
	memset( m_szSplit, 0, sizeof(m_szSplit));
	memset(m_szLogName, 0x00, sizeof(m_szLogName));

	m_DqSnd.clear();
	m_DqRcv.clear();

	for(int i=0; i<1000; i++){
		m_MessageMap[i].nS = -1;
		m_MessageMap[i].nF = -1;
		m_MessageMap[i].fnReceiveCallback = NULL;
	}

	m_pThreadSnd = NULL;
	m_pThreadRcv = NULL;
	m_pThreadTimeOut = NULL;
	m_bTerminatedTimeOutProc = FALSE;

	memset(m_strDataBuf, 0x00, sizeof(m_strDataBuf));
	
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CIPCSock_Imp::~CIPCSock_Imp(void)
{
	if( m_pClientSock != NULL ){
		m_pClientSock->Close();
		delete m_pClientSock;
		m_pClientSock = NULL;
	}

	while(!m_DqSnd.empty()){
		CPacket* pPacket = (CPacket*)(m_DqSnd.front());
		m_DqSnd.pop_front();
		delete pPacket;
		pPacket = NULL;
	}

	while(!m_DqRcv.empty() ){
		CPacket* pPacket = (CPacket*)(m_DqRcv.front());
		m_DqRcv.pop_front();
		delete pPacket;
		pPacket = NULL;
	}
	while( !m_DqTmOut.empty() ){
		CTimeOutPacket* pItem = m_DqTmOut.front();
		m_DqTmOut.pop_front();
		delete pItem;
		pItem = NULL;
	}

	Close();
}

BOOL CIPCSock_Imp::CriticalRcvLock(BOOL bLockUnlock)
{
	if(bLockUnlock == TRUE)
	{ 
		m_csRcvQ.Lock();
		m_bCsIsLock = TRUE;
	}
	else
	{
		m_csRcvQ.Unlock(); 
		m_bCsIsLock = FALSE;
	}

	return m_bCsIsLock;
}

BOOL CIPCSock_Imp::CriticalRcvIsLock()
{
	return m_bCsIsLock;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : pParentWnd = IPCSock CWnd
             pClientWnd = Client CWnd
			 msg_map    = user message map
			 nMapCount  = map count
  RETURN   : 
  COMMENTS : socket thread initialize
==============================================================================*/
int CIPCSock_Imp::Initialize(CWnd* pClientWnd, ST_MESSAGE_MAP* msg_map, char* szLogName )
{
	if(pClientWnd && pClientWnd->m_hWnd){
		m_ClientHWnd = pClientWnd->GetSafeHwnd();
		m_SocketHandlingMode = LBaseSock::eWSAAsyncSelect;
	}else{
		m_ClientHWnd = NULL;
		m_SocketHandlingMode = LBaseSock::eWSAEventSelect;
	}

	for(int i=0; msg_map[i].fnReceiveCallback != NULL; i++){
		m_MessageMap[i] = msg_map[i];
	}

	strncpy_s(m_szLogName, sizeof(m_szLogName), szLogName, strlen(szLogName));
	m_Logger.SetFileName(m_szLogName);

	m_bStopRcvPacketProc = FALSE;
	m_bStopSndPacketProc = FALSE;
	m_bTimeOutProc       = FALSE;
	m_bTerminatedRcvPacketProc = FALSE;
	m_bTerminatedSndPacketProc = FALSE;

	m_pThreadRcv     = AfxBeginThread( CIPCSock_Imp::ThrRcvPacketProc, this );
	m_pThreadSnd     = AfxBeginThread( CIPCSock_Imp::ThrSndPacketProc, this );
	m_pThreadTimeOut = AfxBeginThread( CIPCSock_Imp::ThrPacketTmOutProc, this );

	return 0;
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : none
  RETURN   : none
  COMMENTS : socket thread finallize & socket close
==============================================================================*/
void CIPCSock_Imp::Finallize()
{
	m_bStopRcvPacketProc = TRUE;
	m_bStopSndPacketProc = TRUE;
	m_bTimeOutProc       = TRUE;

	int nCount = 0;
	if( m_pThreadRcv != NULL ){
		while( ::WaitForSingleObject( m_evtThreadTerminatedRcv, 1 ) == WAIT_TIMEOUT ){
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
		while( ::WaitForSingleObject( m_evtThreadTerminatedSnd, 1 ) == WAIT_TIMEOUT ){
			nCount++;
			if( nCount > 500 ){
				::TerminateThread( m_pThreadSnd->m_hThread, 0);
				break;
			}
		}
	}
	m_pThreadSnd = NULL;

	nCount = 0;
	if( m_pThreadTimeOut != NULL ){
		while( ::WaitForSingleObject( m_evtThreadTerminatedTimeOut, 1 ) == WAIT_TIMEOUT ){
			nCount++;
			if( nCount > 500 ){
				::TerminateThread( m_pThreadTimeOut->m_hThread, 0);
				break;
			}
		}
	}
	m_pThreadTimeOut = NULL;

	if( m_pClientSock != NULL ){
		m_pClientSock->Close();
		delete m_pClientSock;
		m_pClientSock = NULL;
	}

	Close();
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : nSocketType = 0:Client, 1:Server
             szIp        = using Ip
			 nPort       = using port number
  RETURN   : 0   : OK
             etc : problem
  COMMENTS : socket connect
==============================================================================*/
int CIPCSock_Imp::SocketConnect(int nSocketType, char* szIp, int nPort )
{
	CString sSockCalss = _T("");
	int nRet     = 0;
	int nRetOpen = 0;
	m_nSocketType = nSocketType;

	if(nSocketType == eSocketTypeClient){
		sSockCalss = _T("TWIPC_CLIENT");
		nRetOpen   = Open(sSockCalss);
		nRet = Connect(szIp, nPort);
	}else{
		sSockCalss = _T("TWIPC_SERVER");
		nRetOpen   = Open(sSockCalss);
		nRet = Listen(szIp, nPort);
	}

	if(nRetOpen != 0){
		AfxMessageBox("socket Open Fail!!!");
	}
	
	return nRet;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : receive packet parse thread & receive
==============================================================================*/
UINT CIPCSock_Imp::ThrRcvPacketProc( LPVOID lpVoid )
{
	CIPCSock_Imp* pSelf = (CIPCSock_Imp*)lpVoid;

	//TRACE(_T("Start thread - Receive Packet Thread. \n"));

	while(1){
		if( pSelf->m_bStopRcvPacketProc == TRUE ) break;

		if( pSelf->m_DqRcv.empty() == DEF_NOT_EMPTY && pSelf->CriticalRcvIsLock() == FALSE){
			pSelf->PopRcvPacket();
			Sleep(10);
		}else{
			Sleep(25);
		}
	}
	pSelf->m_bTerminatedRcvPacketProc = TRUE;

	//TRACE(_T("Terminated thread - Receive Packet Thread. \n"));
	pSelf->m_evtThreadTerminatedRcv.SetEvent();

	return 0;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : pPacket = receive data
  RETURN   : 0 : ok
  COMMENTS : deque add receive packet
==============================================================================*/
void CIPCSock_Imp::AddReceivePacket( CPacket* pPacket )
{
	CriticalRcvLock(TRUE);
	m_DqRcv.push_back(pPacket);
	CriticalRcvLock(FALSE);

	//TRACE("[REV add] MsgId : %d-%d, Stream:%d, Func:%d\n", pPacket->pHead->m_nTheMsgid, pPacket->pHead->stData.nMsgId, pPacket->pHead->stData.nS, pPacket->pHead->stData.nF);

	DeleteTmOutPacket(pPacket->pHead->stData.nMsgId);
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : none
  RETURN   : none
  COMMENTS : deque pop receive data & receive data event call back
==============================================================================*/
void CIPCSock_Imp::PopRcvPacket()
{
// 	for(int i=0; i < (int)m_DqRcv.size(); i++){
// 		CPacket* pPacket1 = (CPacket*)m_DqRcv[i];
// 		//TRACE("[DATA %d] MsgId : %d-%d, Stream:%d, Func:%d\n", i, pPacket1->pHead->m_nTheMsgid, pPacket1->pHead->stData.nMsgId, pPacket1->pHead->stData.nS, pPacket1->pHead->stData.nF);
// 	}

	CriticalRcvLock(TRUE);
	CPacket* pPacket = (CPacket*)m_DqRcv.front();
	CriticalRcvLock(FALSE);

	//TRACE("[REV] MsgId : %d-%d, Stream:%d, Func:%d\n", pPacket->pHead->m_nTheMsgid, pPacket->pHead->stData.nMsgId, pPacket->pHead->stData.nS, pPacket->pHead->stData.nF);

	int Index = 0;
	while(m_MessageMap[Index].fnReceiveCallback != NULL ){
		if( m_MessageMap[Index].nS == pPacket->pHead->stData.nS ){
			if( m_MessageMap[Index].nF == pPacket->pHead->stData.nF ){
				m_MessageMap[Index].fnReceiveCallback(pPacket);
				break;
			}
		}
		Index++;
	}
	CriticalRcvLock(TRUE);
	m_DqRcv.pop_front();
	CriticalRcvLock(FALSE);

	m_Logger.LOG("[REV] MsgId : %d-%d, Stream:%d, Func:%d\n", pPacket->pHead->m_nTheMsgid, pPacket->pHead->stData.nMsgId, pPacket->pHead->stData.nS, pPacket->pHead->stData.nF);

	delete pPacket;
	pPacket = NULL;
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : check send Packet timeout
==============================================================================*/
UINT CIPCSock_Imp::ThrPacketTmOutProc( LPVOID lpVoid )
{
	CIPCSock_Imp* pSelf = (CIPCSock_Imp*)lpVoid;

	//TRACE(_T("Start thread - Time Out Thread. \n"));

	while(1){
		if( pSelf->m_bTimeOutProc == TRUE ) break;

		if( pSelf->m_DqTmOut.empty() == DEF_NOT_EMPTY){
			int nRet = pSelf->PopTmOutPakcetChkTmOut();
			if(nRet == -1){
				if(pSelf->m_ClientHWnd)
					SendMessage(pSelf->m_ClientHWnd, UM_TWIPCSOCK_RCV_EVENT, (WPARAM)IPC_EVT_TM_OVER_MES, (LPARAM)NULL);
				else
					NoCWndThread::SendMessage(UM_TWIPCSOCK_RCV_EVENT, (WPARAM)IPC_EVT_TM_OVER_MES, (LPARAM)NULL);
			}
			Sleep(1);
		}
		else
			Sleep(25);
	}
	pSelf->m_bTerminatedTimeOutProc = TRUE;

	//TRACE(_T("Terminated thread - Time Out Thread. \n"));
	pSelf->m_evtThreadTerminatedTimeOut.SetEvent();

	return 0;
}

int CIPCSock_Imp::PopTmOutPakcetChkTmOut()
{
	double dTickCount         = (double)GetTickCount64();

	m_csTmOutQ.Lock();	

	int nCount = 0;
	for(DqTmOutIter = m_DqTmOut.begin(); DqTmOutIter != m_DqTmOut.end(); DqTmOutIter++, nCount++)
	{
		CTimeOutPacket* pTmOutPacket = (CTimeOutPacket*)m_DqTmOut.at(nCount);

		double dTC_Calc = dTickCount - pTmOutPacket->m_dTickCountStart;
		if(dTC_Calc > (double)m_nTmOutMilliSec){
			m_DqTmOut.erase( DqTmOutIter);

			//if(pTmOutPacket != NULL){
				delete pTmOutPacket;
//			}			
			pTmOutPacket = NULL;
			m_csTmOutQ.Unlock();	
			return -1;
		}
	}

	m_csTmOutQ.Unlock();

	return 0;
}

void CIPCSock_Imp::DeleteTmOutPacket(int a_nMsgId)
{
	double dTickCount = (double)GetTickCount64();

	m_csTmOutQ.Lock();	

	int nCount = 0;
	for(DqTmOutIter = m_DqTmOut.begin(); DqTmOutIter != m_DqTmOut.end(); DqTmOutIter++, nCount++)
	{
		CTimeOutPacket* pTmOutPacket = (CTimeOutPacket*)m_DqTmOut.at(nCount);

		if(a_nMsgId == pTmOutPacket->m_pPacket->pHead->stData.nMsgId){
			m_DqTmOut.erase( DqTmOutIter);
			delete pTmOutPacket;
			pTmOutPacket = NULL;
			break;
		}
	}

	m_csTmOutQ.Unlock();
}

void CIPCSock_Imp::AddTmOutPacket(CPacket* pPacket, double dTickCount)
{
	m_csTmOutQ.Lock();

	//DWORD dwTickCount         = ::GetTickCount();
	CTimeOutPacket* pTmPacket = new CTimeOutPacket(dTickCount, pPacket);
	m_DqTmOut.push_back(pTmPacket);

	m_csTmOutQ.Unlock();
	
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : send packet parse thread & send
==============================================================================*/
UINT CIPCSock_Imp::ThrSndPacketProc( LPVOID lpVoid )
{
	CIPCSock_Imp* pSelf = (CIPCSock_Imp*)lpVoid;

	//TRACE(_T("Start thread - Send Packet Thread. \n"));

	while(1){
		if( pSelf->m_bStopSndPacketProc == TRUE ) break;

		if( pSelf->m_DqSnd.empty() == DEF_NOT_EMPTY){
			pSelf->PopSndPacket();
		}
		else
			Sleep(1);
	}

	pSelf->m_bTerminatedSndPacketProc = TRUE;

	//TRACE(_T("Terminated thread - Send Packet Thread. \n"));
	pSelf->m_evtThreadTerminatedSnd.SetEvent();

	return 0;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : pPacket = send data
  RETURN   : 0 : ok
  COMMENTS : deque add send packet
==============================================================================*/
int CIPCSock_Imp::AddSendPacket( CPacket* pPacket, BOOL bTmReplyCheck)
{
	m_csSndQ.Lock();

	//AfxTrace(" Add Send Packet :: START \n");
	if(bTmReplyCheck == TRUE){
		double dTickCount         = (double)GetTickCount64();
		AddTmOutPacket(pPacket, dTickCount);

		//AfxTrace(" Add Send Packet :: Tc=%d, ", dwTickCount);
	}
	
	m_DqSnd.push_back(pPacket);
	m_csSndQ.Unlock();

	//AfxTrace(" Add Send Packet :: END \n");

	return 0;
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : none
  RETURN   : none
  COMMENTS : deque pop send data
==============================================================================*/
void CIPCSock_Imp::PopSndPacket()
{
	/* copy send packet Q*/
	while( m_DqSnd.empty() == DEF_NOT_EMPTY )
	{
		m_csSndQ.Lock();
		CPacket* pkt = m_DqSnd.front();
		m_csSndQ.Unlock();

		if( pkt )
		{
			int nRet = SendPacket( *pkt);
			if(nRet == SOCKET_ERROR)
			{
				nRet = WSAGetLastError();
				if(nRet == WSAEWOULDBLOCK){
					//AfxTrace("WSAEWOULDBLOCK\n");
					if(WaitForSingleObject(m_hSendBlockingEvent, INFINITE) == WAIT_OBJECT_0){
						ResetEvent(m_hSendBlockingEvent);
						continue;
					}
					//else
					//	AfxTrace("Send Timeout\n");
				}
			}
			delete pkt;
		}
		m_csSndQ.Lock();
		m_DqSnd.pop_front();
		m_csSndQ.Unlock();
	}
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : pPacket = send packet data
  RETURN   : Err = -1:Socket Err
  COMMENTS : Send packet data
==============================================================================*/
int CIPCSock_Imp::SendPacket(CPacket& pPacket)
{
	if(IsConnected() == FALSE){
		return -1;
	}

	LBaseSock* pSock = NULL;
	if( m_nSocketType == eSocketTypeClient ){
		pSock = this;
	}else{
		pSock = m_pClientSock;
	}


	//TRACE("[SEND]MsgId:%d-%d, nS:%d, nF:%d\n",pPacket.pHead->m_nTheMsgid, pPacket.pHead->stData.nMsgId, pPacket.pHead->stData.nS, pPacket.pHead->stData.nF);

	int nSize = pPacket.pHead->GetSize() + pPacket.pBody->GetSize();
	BYTE* pByte = new BYTE [nSize];
	memcpy(pByte, &pPacket.pHead->stData, pPacket.pHead->GetSize());
	memcpy(pByte+pPacket.pHead->GetSize(), pPacket.pBody->GetDataPtr(), pPacket.pBody->GetSize());

	int nErr = pSock->Send(pByte, nSize);

	delete [] pByte;


	if(nErr == SOCKET_ERROR){
		// afxMessageBox 호출 부분 - Vision 프로그램과 디버깅 진행 중 강제 종료 하면, Handler OS 먹통 현상으로 인해 임시 주석 - [현지 검증 필요] [12/19/2025 donghyun.shin]
		//AfxMessageBox("Socket Error!!!! Can't send data!!");
	}

	return nErr;
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : nErrorCode = 0  : ok
                         etc : err
  RETURN   : 
  COMMENTS : socket accept
==============================================================================*/
void CIPCSock_Imp::OnAccept(int nErrorCode)
{
	if(!nErrorCode){
		m_bStatusConnect = TRUE;
	}else{
		m_bStatusConnect = FALSE;
	}

	if( m_pClientSock != NULL ){
		delete m_pClientSock;
		m_pClientSock = NULL;
	}
	m_pClientSock = new CTechwingSock;
	m_pClientSock->SetServerSock(this);

	int nError = 0;
	nError = m_pClientSock->Open("TWIPC_COMM_SOCK");
	
	if( nError != 0 ){
		// Error Log
	}
	nError = m_pClientSock->Accept(*this);
	if( nError != 0 ){
		// Error Log
	}
	int nRet;

	if(m_ClientHWnd)
		nRet = SendMessage(m_ClientHWnd, UM_TWIPCSOCK_RCV_EVENT, (WPARAM)IPC_EVT_CONNECTED, (LPARAM)nError);
	else
		nRet = NoCWndThread::SendMessage(UM_TWIPCSOCK_RCV_EVENT, (WPARAM)IPC_EVT_CONNECTED, (LPARAM)nError);
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : nErrorCode = 0  : ok
                         etc : err
  RETURN   : 
  COMMENTS : socket close
==============================================================================*/
void CIPCSock_Imp::OnClose(int nErrorCode)
{
	int nRet = 0;
	m_bStatusConnect = FALSE;
	if(m_ClientHWnd)
		nRet = PostMessage(m_ClientHWnd, UM_TWIPCSOCK_RCV_EVENT, (WPARAM)IPC_EVT_DISCONNECTED, (LPARAM)NULL);
	else
		nRet = NoCWndThread::PostMessage(UM_TWIPCSOCK_RCV_EVENT, (WPARAM)IPC_EVT_DISCONNECTED, (LPARAM)NULL);
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : nErrorCode = 0  : ok
                         etc : err
  RETURN   : 
  COMMENTS : socket connect
==============================================================================*/
void CIPCSock_Imp::OnConnect(int nErrorCode)
{
	if(!nErrorCode){
		m_bStatusConnect = TRUE;
		if(m_ClientHWnd)
			SendMessage(m_ClientHWnd, UM_TWIPCSOCK_RCV_EVENT, (WPARAM)IPC_EVT_CONNECTED, (LPARAM)NULL);
		else
			NoCWndThread::SendMessage(UM_TWIPCSOCK_RCV_EVENT, (WPARAM)IPC_EVT_CONNECTED, (LPARAM)NULL);
	}else{
		if (m_ClientHWnd)
			SendMessage(m_ClientHWnd, UM_TWIPCSOCK_RCV_EVENT, (WPARAM)IPC_EVT_CLIENT_CAN_NOT_CONNECT, (LPARAM)NULL);
		else
			NoCWndThread::SendMessage(UM_TWIPCSOCK_RCV_EVENT, (WPARAM)IPC_EVT_CLIENT_CAN_NOT_CONNECT, (LPARAM)NULL);

		m_bStatusConnect = FALSE;
	}	
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CIPCSock_Imp::OnReceive(char* sData, int nRdLength)
{
	int      nRet = 0;
	char	 szPacket[40000]={0};
	CString  strData = _T("");

	if( nRdLength < 0 ) {
		return;
	}
	CriticalRcvLock(TRUE);

	if( m_nSplitCount != 0 ){
		memcpy(&m_szSplit[m_nSplitCount], sData, nRdLength);	//한번에 들어오지 못한 데이터를 이후로 붙임
		memcpy(sData, m_szSplit, m_nSplitCount + nRdLength );	//수신 데이터에 다시 붙여쓴다.
		nRdLength += m_nSplitCount;
		memset(m_szSplit, 0, sizeof(m_szSplit));
		m_nSplitCount = 0;
	}

	int nRemainMsgLen = nRdLength;
	char* pStart = &sData[0];
	
	while(nRemainMsgLen != 0)
	{
		CPacketHead Header;
		memcpy((void*)&Header, pStart, sizeof(CPacketHead));

		if( nRemainMsgLen >= Header.GetSize() ){			// Header가 덜 들어온 경우 확인
			if( nRemainMsgLen >= Header.GetPacketSize() ){	// Data가 덜 들어온 경우 확인

				memset(szPacket, 0, sizeof(szPacket));
				memcpy(szPacket, pStart,  Header.GetPacketSize() );

				pStart +=  Header.GetPacketSize();
				do_DataParse(szPacket);
				nRemainMsgLen -= Header.GetPacketSize();
			}else{	
				if( nRemainMsgLen > 0 )
				{					
					memcpy(&m_szSplit[m_nSplitCount], pStart, nRemainMsgLen);
					m_nSplitCount += nRemainMsgLen;
				}
				break;
			}
		}else{	
			if( nRemainMsgLen > 0 )
			{				
				memcpy(&m_szSplit[m_nSplitCount], pStart, nRemainMsgLen);
				m_nSplitCount += nRemainMsgLen;
			}
			break;
		}
	}

	CriticalRcvLock(FALSE);
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CIPCSock_Imp::OnReceive(int nErrorCode)
{
	int      nRdLength = 0;
	char     sData[40000]={0};

	if( nErrorCode ){
		return;
	}

	nRdLength = Receive(sData, MAX_RCV_BUFF);

	if( nRdLength < 0 ) {
		return;
	}

	OnReceive(sData, nRdLength);
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : receivce data parse & add deque
==============================================================================*/
int CIPCSock_Imp::do_DataParse(char sData[])
{
	memset(m_strDataBuf,0,sizeof(m_strDataBuf));

	CPacketHead Header;
	memcpy((void*)&Header, sData, sizeof(CPacketHead));

	CPacket* pPacket = NULL;
	int nPacketBodyLength = Header.GetPacketSize()-Header.GetSize();

	switch(Header.stData.nS)
	{
	case 2:
		{
			switch(Header.stData.nF)
			{
			case 3: { if( nPacketBodyLength  != sizeof(ST_PACKET_BODY_S2F3)) return -1; }break;
			case 4: { if( nPacketBodyLength  != sizeof(ST_PACKET_BODY_S2F4)) return -1; }break;
			case 41: { if( nPacketBodyLength  != sizeof(ST_PACKET_BODY_S2F41)) return -1; }break;
			case 42: { if( nPacketBodyLength  != sizeof(ST_PACKET_BODY_S2F42)) return -1; }break;
			default: { return -2; }
			}
		}break;
	case 6:
		{
			switch(Header.stData.nF)
			{
			case 11: { if( nPacketBodyLength  != sizeof(ST_PACKET_BODY_S6F11)) return -1; }break;
			case 12: { if( nPacketBodyLength  != sizeof(ST_PACKET_BODY_S6F12)) return -1; }break;
			default: { return -2; }
			}
		}break;
	case 107:
		{
			switch(Header.stData.nF)
			{
			case 1:	 { if( nPacketBodyLength  != sizeof(ST_PACKET_BODY_S107F1)) return -1; }break;
			case 2:	 { if( nPacketBodyLength  != sizeof(ST_PACKET_BODY_S107F2)) return -1; }break;
			case 3:	 { if( nPacketBodyLength  != sizeof(ST_PACKET_BODY_S107F3)) return -1; }break;
			case 4:	 { if( nPacketBodyLength  != sizeof(ST_PACKET_BODY_S107F4)) return -1; }break;
			case 5:	 { if( nPacketBodyLength  != sizeof(ST_PACKET_BODY_S107F5)) return -1; }break;
			case 6:	 { if( nPacketBodyLength  != sizeof(ST_PACKET_BODY_S107F6)) return -1; }break;
			case 9:	 { if( nPacketBodyLength  != sizeof(ST_PACKET_BODY_S107F9)) return -1; }break;
			default: { return -2; }
			}
		}break;
	default: { return -2; }
	}

	pPacket = new CPacket(Header.stData.nS, Header.stData.nF);
	pPacket->CopyFromByte(sData);
	//pPacket->CopyFromByte(sData);

	m_Logger.LOG("[REV S%dF%d] MsgId : %d-%d\n", pPacket->pHead->stData.nS, pPacket->pHead->stData.nF, pPacket->pHead->m_nTheMsgid, pPacket->pHead->stData.nMsgId);
	
	AddReceivePacket(pPacket);
	
	return 0;
}


/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : 
  RETURN   : m_bStatusConnect  TRUE  : connected
                               FALSE : not connected
  COMMENTS : check socket connected
==============================================================================*/
BOOL CIPCSock_Imp::IsConnected()
{
	return m_bStatusConnect;
}

void CIPCSock_Imp::SetTmOutLimit(int nLimitTmMilSec/*Milli Second*/)
{
	m_nTmOutMilliSec = nLimitTmMilSec;
}

