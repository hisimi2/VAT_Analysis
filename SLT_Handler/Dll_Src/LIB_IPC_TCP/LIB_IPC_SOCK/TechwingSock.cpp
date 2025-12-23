#include "StdAfx.h"
#include "LBaseSock.h"
#include "IPCSock_Imp.h"
#include "TechwingSock.h"



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-03]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CTechwingSock::CTechwingSock(LBaseSock::eSocketEventHandling mode /* = LBaseSock::eWSAAsyncSelect */)
	: LBaseSock(mode)
{
	m_pServerSock = NULL;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-09-03]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CTechwingSock::~CTechwingSock(void)
{
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-03]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CTechwingSock::OnAccept(int nErrorCode)
{

}


void CTechwingSock::OnSend( int nErrorCode )
{
	if(m_pServerSock)
		if(m_pServerSock->m_hSendBlockingEvent)
			SetEvent(m_pServerSock->m_hSendBlockingEvent);
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-03]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CTechwingSock::OnClose(int nErrorCode)
{
	((LBaseSock*)m_pServerSock)->OnClose(nErrorCode);
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-03]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CTechwingSock::OnReceive(int nErrorCode)
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

	((CIPCSock_Imp*)m_pServerSock)->OnReceive(sData, nRdLength);
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-03]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CTechwingSock::OnConnect(int nErrorCode)
{
	((CIPCSock_Imp*)m_pServerSock)->OnConnect(nErrorCode);
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-09-03]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
int CTechwingSock::SetServerSock(CIPCSock_Imp* pServerSock)
{
	m_SocketHandlingMode = pServerSock->m_SocketHandlingMode;
	m_pServerSock = pServerSock;
	return 0;
}
