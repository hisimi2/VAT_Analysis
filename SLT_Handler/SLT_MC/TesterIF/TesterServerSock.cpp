#include "StdAfx.h"
//#include "TesterIF.h"
#include "TesterServerSock.h"
#include <afxwin.h>
#include "TesterClientSock.h"
#include "LBaseSock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTesterServerSock::CTesterServerSock(void)
{
	m_pTesterIF = NULL;
	m_bConnect = FALSE;
}

CTesterServerSock::~CTesterServerSock(void)
{
	DeleteAllClientSock();
}


BOOL CTesterServerSock::DeleteAllClientSock()
{
	POSITION pos = m_listClient.GetHeadPosition();

	while(pos != NULL)
	{
		CTesterClientSock* cs = m_listClient.GetNext(pos);
		delete cs;
	}

	m_listClient.RemoveAll();

	return TRUE;
}

CTesterClientSock* CTesterServerSock::GetNotConnectSock()
{
	POSITION pos = m_listClient.GetHeadPosition();
	CTesterClientSock* retCs = NULL;

	while(pos != NULL)
	{
		CTesterClientSock* cs = m_listClient.GetNext(pos);
		if(cs->m_bConnect == FALSE)
		{
			retCs = cs;
			break;
		}
	}

	return retCs;
}


void CTesterServerSock::OnAccept(int nErrorCode)
{
	CTesterClientSock* cs = GetNotConnectSock();
	BOOL addFlag = FALSE;

	if(cs == NULL)
	{
		addFlag = TRUE;
		cs = new CTesterClientSock;
	}

	if(cs->Open("Tester") != EWS_NO_ERR)
	{
		delete cs;
		AfxMessageBox("OnAccept Open Error!!");
		return;
	}

	if(cs->Accept(*this) != EWS_NO_ERR)
	{
		delete cs;
		AfxMessageBox("OnAccept Accept Error!!");
		return;
	}

	cs->m_pTesterIF = m_pTesterIF;
	//cs->m_pServerSock = this;
	cs->m_bConnect = TRUE;

	if(addFlag == TRUE)
		m_listClient.AddTail(cs);
}

void CTesterServerSock::OnClose(int nErrorCode)
{
}
