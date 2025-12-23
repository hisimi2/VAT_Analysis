#include "StdAfx.h"
#include "LBaseSock.h"
#include "IPCSock.h"
#include "IPCSock_Imp.h"
#include "NoCWnd.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//CIPCSock_Imp* g_pIPCSock_Imp = NULL;
int  CPacketHead::m_nTheMsgid = 0;

/*=============================================================================
  AUTHOR   : ±è¼±Áø
  NAME     : 
  PARAMS   : 
  RETURN   : id number
  COMMENTS : window create id number
==============================================================================*/
UINT g_nChildWndID = 1000;
UINT g_fnMakeChildWndID()
{
	return ++g_nChildWndID;
}



/*=============================================================================
  AUTHOR   : ³ª¸íÁø  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CIPCSock::CIPCSock(void)
{
	m_nSocketType = 0;
	memset(m_szIp, 0x00, sizeof(m_szIp));
	memset(m_szLogName, 0x00, sizeof(m_szLogName));
	m_nPort = 0;
	m_pIPCSock_Imp = NULL;
	for(int i=0; i<1000; i++){
		m_MessageMap[i].nS = -1;
		m_MessageMap[i].nF = -1;
		m_MessageMap[i].fnReceiveCallback = NULL;
	}
    m_pParentWnd = NULL;
    m_nMapCount = 0;
}



/*=============================================================================
  AUTHOR   : ³ª¸íÁø  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CIPCSock::~CIPCSock(void)
{
	if(m_pIPCSock_Imp != NULL)
	{
		((CIPCSock_Imp*)m_pIPCSock_Imp)->Finallize();
		delete (CIPCSock_Imp*)m_pIPCSock_Imp;
		m_pIPCSock_Imp = NULL;
	}
}


/*=============================================================================
  AUTHOR   : ³ª¸íÁø  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
BEGIN_MESSAGE_MAP(CIPCSock, CWnd)
	ON_MESSAGE(UM_TWIPCSOCK_RCV_EVENT, OnTWIPCEvent)
	ON_WM_TIMER()
END_MESSAGE_MAP()


/*=============================================================================
  AUTHOR   : ±è¼±Áø
  NAME     : 
  PARAMS   : nSocketType = 0: client 1:Server
             szIp        = using IP
			 nPort       = using port number
  RETURN   : 0   : ok
             etc : socket problem
  COMMENTS : socket connect
==============================================================================*/
int CIPCSock::SocketConnect(int nSocketType, char* szIp, int nPort )
{	
	if(m_pIPCSock_Imp != NULL){
		((CIPCSock_Imp*)m_pIPCSock_Imp)->Finallize();
		delete (CIPCSock_Imp*)m_pIPCSock_Imp;
		m_pIPCSock_Imp = NULL;
	}
	m_pIPCSock_Imp = new CIPCSock_Imp;
	int nRet = ((CIPCSock_Imp*)m_pIPCSock_Imp)->Initialize( this, m_MessageMap, m_szLogName );

	m_nSocketType = nSocketType;
	strncpy_s(m_szIp, sizeof(m_szIp), szIp, strlen(szIp));
	m_nPort = nPort;
	
	SendLogMessage("[CIPCSock] SocketConnect");

	return ((CIPCSock_Imp*)m_pIPCSock_Imp)->SocketConnect(nSocketType, szIp, nPort);
}

/*=============================================================================
  AUTHOR   : ÀÌ¿ø¿ë
  NAME     : 
  PARAMS   : 
  RETURN   : 0   : ok
             etc : socket problem
  COMMENTS : socket connect
==============================================================================*/
int  CIPCSock::SocketConnect()
{
	return this->SocketConnect( m_nSocketType, m_szIp, m_nPort );
}

int CIPCSock::GetSocketConnect(int& nSocketType, CString& sIp, int& nPort)
{
	nSocketType = m_nSocketType;
	sIp = m_szIp;
	nPort = m_nPort;

	return 0;
}


/*=============================================================================
  AUTHOR   : ±è¼±Áø
  NAME     : 
  PARAMS   : 
  RETURN   : none
  COMMENTS : socket close
==============================================================================*/
int CIPCSock::SocketClose()
{
	if(m_pIPCSock_Imp != NULL){
		((CIPCSock_Imp*)m_pIPCSock_Imp)->Finallize();
		delete (CIPCSock_Imp*)m_pIPCSock_Imp;
		m_pIPCSock_Imp = NULL;
	}

	return 0;
}

/*=============================================================================
  AUTHOR   : ±è¼±Áø
  NAME     : 
  PARAMS   : none
  RETURN   : TRUE  : connected
             FALSE : not connected
  COMMENTS : check socket connected
==============================================================================*/
BOOL CIPCSock::IsConnected()
{
	if( m_pIPCSock_Imp == NULL ){ return FALSE; }	// not connect
	return ((CIPCSock_Imp*)m_pIPCSock_Imp)->IsConnected();
}


/*=============================================================================
  AUTHOR   : ±è¼±Áø
  NAME     : 
  PARAMS   : szIP : IP
  RETURN   : 
  COMMENTS : ping check
==============================================================================*/
BOOL CIPCSock::IsPing( char* szIp )
{
	return TRUE;
}


/*=============================================================================
  AUTHOR   : ±è¼±Áø
  NAME     : 
  PARAMS   : 
  RETURN   : 0 : OK
  COMMENTS : packet data Send
==============================================================================*/
int CIPCSock::Send( CPacket* pPacket, BOOL bTmReplyCheck)
{
	if(pPacket == NULL)         { return -1; }
	if( m_pIPCSock_Imp == NULL ){ return -3; }

	if( IsConnected() != TRUE ){
		return -2;
	}
	CPacket* pNewPacket = new CPacket(pPacket->pHead->stData.nS, pPacket->pHead->stData.nF);
	pNewPacket->CopyFromObject( *pPacket );

	//TRACE("send message: S%d,F%d,MsgId=%d, BodySize=%d\n, ", pNewPacket->pHead->stData.nS, pNewPacket->pHead->stData.nF, pNewPacket->pHead->stData.nMsgId, pNewPacket->pBody->GetSize());
	SendLogMessage("[CIPCSock] send message: S%d,F%d,MsgId=%d, BodySize=%d\n, ", pNewPacket->pHead->stData.nS, pNewPacket->pHead->stData.nF, pNewPacket->pHead->stData.nMsgId, pNewPacket->pBody->GetSize());
	return ((CIPCSock_Imp*)m_pIPCSock_Imp)->AddSendPacket(pNewPacket, bTmReplyCheck);
}


/*=============================================================================
  AUTHOR   : ±è¼±Áø
  NAME     : 
  PARAMS   : pParentWnd  = From Client CWnd, if NULL will be NOT WINDOW BASED
             nSocketType = 0: Client, 1: Server
			 szIp        = user setting IP
			 nPort       = user setting port number
  RETURN   : 0 : not Error
  COMMENTS : socket create & use socket data thread initialize
==============================================================================*/
int CIPCSock::Initialize(CWnd* pParentWnd, int nSocketType, char* szIp, int nPort, ST_MESSAGE_MAP* msg_map, char* szLogName )
{	                   
	m_pParentWnd = pParentWnd;

	if(GetParentWnd())
	BOOL bStat = Create(NULL, _T("CHILD_WND"), WS_VISIBLE | WS_OVERLAPPED, CRect(0,0,0,0), pParentWnd, g_fnMakeChildWndID(), NULL);
	else {
		NoCWndThread::StartSockEvnetThread(this);
	}

	for(int i=0; msg_map[ i ].fnReceiveCallback != NULL; i++){
		m_MessageMap[i] = msg_map[i];
	}
	m_nSocketType = nSocketType;
	strncpy_s(m_szIp, sizeof(m_szIp), szIp, strlen(szIp));
	m_nPort = nPort;

	strncpy_s(m_szLogName, sizeof(m_szLogName), szLogName, strlen(szLogName));
	m_Logger.SetFileName(m_szLogName);
	return 0;
}


/*=============================================================================
  AUTHOR   : ±è¼±Áø
  NAME     : 
  PARAMS   : None
  RETURN   : None
  COMMENTS : use socket data thread kill & socket close
==============================================================================*/
void CIPCSock::Finalize()
{
	if(m_pIPCSock_Imp != NULL){
		((CIPCSock_Imp*)m_pIPCSock_Imp)->Finallize();
		delete (CIPCSock_Imp*)m_pIPCSock_Imp;
		m_pIPCSock_Imp = NULL;
	}

	if(GetParentWnd())
	::DestroyWindow(*this);
	else
		NoCWndThread::KillSockEventThread();
}



/*=============================================================================
  AUTHOR   : ±è¼±Áø
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : socket status event
==============================================================================*/  
LRESULT CIPCSock::OnTWIPCEvent(WPARAM wParam, LPARAM lParam)
{
	if( wParam == IPC_EVT_DISCONNECTED ){
		if (GetParentWnd()) 
			KillTimer(0);
		else
			NoCWndThread::KillTimer(0);

		if( m_nSocketType == eSocketTypeServer ){
			if(m_pIPCSock_Imp != NULL){
				((CIPCSock_Imp*)m_pIPCSock_Imp)->Finallize();
				delete (CIPCSock_Imp*)m_pIPCSock_Imp;
				m_pIPCSock_Imp = NULL;
			}
			m_pIPCSock_Imp = new CIPCSock_Imp;
			int nRet = ((CIPCSock_Imp*)m_pIPCSock_Imp)->Initialize( this, m_MessageMap, m_szLogName );
			((CIPCSock_Imp*)m_pIPCSock_Imp)->SocketConnect(m_nSocketType, m_szIp, m_nPort);
			SendLogMessage("[CIPCSock][OnTWIPCEvent][SocketConnect]  m_nSocketType=%d ,m_szIp=%d, m_nPort=%d wParam=%d\n, ", m_nSocketType, m_szIp, m_nPort, (int)wParam);
		}
		else
		{
			if( m_nSocketType == eSocketTypeClient ){
				if (GetParentWnd())
				{
					SendLogMessage("[CIPCSock][OnTWIPCEvent][Retry Timer] wParam=%d", (int)wParam);
					SetTimer(0, CLIENT_DISCONNECT_RETRY_TM, NULL);
				}
					
				else {
					SendLogMessage("[CIPCSock][OnTWIPCEvent][Retry Timer NoCWndThread] wParam=%d", (int)wParam);
					NoCWndThread::SetTimer(0, CLIENT_DISCONNECT_RETRY_TM, NULL);			
				}
					
			}
 		}
 	}
	else if(wParam == IPC_EVT_CONNECTED)
	{		
		if (GetParentWnd()) {
			KillTimer(0);
			SendLogMessage("[CIPCSock][OnTWIPCEvent][IPC_EVT_CONNECTED][KillTimer] wParam=%d", (int)wParam);
		}
			
		else	
		{
			NoCWndThread::KillTimer(0);
			SendLogMessage("[CIPCSock][OnTWIPCEvent][IPC_EVT_CONNECTED][NoCWndThread KillTimer] wParam=%d", (int)wParam);
		}
	}
	else if (wParam == IPC_EVT_CLIENT_CAN_NOT_CONNECT)
	{
		KillTimer(0);
	
		if (m_nSocketType == eSocketTypeClient) {
			if (GetParentWnd())
			{
				SetTimer(0, CLIENT_DISCONNECT_RETRY_TM, NULL);
				SendLogMessage("[CIPCSock][OnTWIPCEvent] [IPC_EVT_CLIENT_CAN_NOT_CONNECT] wParam=%d", (int)wParam);
			}			
			else {
				NoCWndThread::SetTimer(0, CLIENT_DISCONNECT_RETRY_TM, NULL);
				SendLogMessage("[CIPCSock][OnTWIPCEvent] [NoCWndThread IPC_EVT_CLIENT_CAN_NOT_CONNECT] wParam=%d", (int)wParam);
			}
				
		}
	}

	return 0L;
}


/*=============================================================================
  AUTHOR   : ³ª¸íÁø  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CIPCSock::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 0)
	{
		if(GetParentWnd())
		KillTimer(nIDEvent);
		else
			NoCWndThread::KillTimer(nIDEvent);

		if(m_pIPCSock_Imp != NULL){
			((CIPCSock_Imp*)m_pIPCSock_Imp)->Finallize();
			delete (CIPCSock_Imp*)m_pIPCSock_Imp;
			m_pIPCSock_Imp = NULL;
		}

		if (m_pIPCSock_Imp == NULL)
		{
			m_pIPCSock_Imp = new CIPCSock_Imp;
			int nRet = ((CIPCSock_Imp*)m_pIPCSock_Imp)->Initialize(this, m_MessageMap, m_szLogName);
			((CIPCSock_Imp*)m_pIPCSock_Imp)->SocketConnect(m_nSocketType, m_szIp, m_nPort);
			SendLogMessage("[CIPCSock][OnTimer][SocketConnect] m_nSocketType=%d ,m_szIp=%s, m_nPort=%d\n, ", m_nSocketType, m_szIp, m_nPort);
		}
		else {
			KillTimer(nIDEvent);
			m_pIPCSock_Imp = NULL;
		}
	}

	if(GetParentWnd())
	{
		SendLogMessage("[CIPCSock][OnTimer][Retry Timer]");
		SetTimer(nIDEvent, CLIENT_DISCONNECT_RETRY_TM , NULL);
		CWnd::OnTimer(nIDEvent);
	}
	else
	{
		SendLogMessage("[CIPCSock][OnTimer][NoCWndThread][Retry Timer]");
		NoCWndThread::SetTimer(nIDEvent, CLIENT_DISCONNECT_RETRY_TM , NULL);
	}
}


/*=============================================================================
  AUTHOR   : ³ª¸íÁø  [2015-09-15]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CIPCSock::SetTmOutLimit( int nMilliSec )
{
	((CIPCSock_Imp*)m_pIPCSock_Imp)->SetTmOutLimit(nMilliSec);
}

void CIPCSock::ChangeSetting(char* szIp, int nPort)
{
	strncpy_s(m_szIp, sizeof(m_szIp), szIp, strlen(szIp));
	m_nPort = nPort;
}

void CIPCSock::SendLogMessage(const char* fmt, ...)
{
	char szEventMessage[1024] = { 0, };
	va_list va;
	va_start(va, fmt);
	vsprintf_s(szEventMessage, sizeof(szEventMessage), fmt, va);
	va_end(va);

	m_Logger.LOG(szEventMessage);
}