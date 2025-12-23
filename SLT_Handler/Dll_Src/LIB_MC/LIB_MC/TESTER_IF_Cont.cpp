#include "StdAfx.h"
#include "TESTER_IF_Cont.h"
#include "IFComm_GPIB.h"
#include "IFComm_Simul.h"
#include "ConfigData.h"

CTESTER_IF_Cont::CTESTER_IF_Cont(void)
{
	m_pClientWnd = NULL;
	m_pTesterIF_Imp = NULL;
	memset(&m_szInitFileName,0,sizeof(m_szInitFileName));
}


CTESTER_IF_Cont::~CTESTER_IF_Cont(void)
{
}

int CTESTER_IF_Cont::Initialize( CWnd* pClientWnd, ST_COMM_DATA stCommData, ST_TESTER_MESSAGE_MAP* pMessageMap )
{
	m_pTesterIF_Imp = NULL;
	sprintf_s(m_szInitFileName, sizeof(m_szInitFileName),  "%s", stCommData.szInitFileName);
	m_pClientWnd = pClientWnd;
	m_stCommData = stCommData;

	int nRes = 0;

	switch( m_stCommData.nIF_Type )
	{
	case eCOM_GPIB:
		m_pTesterIF_Imp = new CIFComm_GPIB;
		break;
	case eCOM_RS232C:
		break;
	case eCOM_DIO:
		break;
	case eCOM_SIMUL:
		m_pTesterIF_Imp = new CIFComm_Simul;
		AfxMessageBox( _T( "Tester Communication is Simulation!!!" ) );
		break;
	default:
		//VERIFY( false && "Not Allowed Type" );
		VERIFY();
		break;
	}

	nRes = ((CIFComm*)m_pTesterIF_Imp)->Initialize( 
		this, pClientWnd->GetSafeHwnd(), pClientWnd, stCommData, pMessageMap );

	return nRes;
}

void CTESTER_IF_Cont::Finalize()
{
	if( ((CIFComm*)m_pTesterIF_Imp) != NULL )
	{
		((CIFComm*)m_pTesterIF_Imp)->Finalize();
		delete m_pTesterIF_Imp;
		m_pTesterIF_Imp = NULL;
	}		
}

bool CTESTER_IF_Cont::IsConnected()
{
	bool bRes = false;

	bRes = ((CIFComm*)m_pTesterIF_Imp)->IsConnected();

	return bRes;
}

int CTESTER_IF_Cont::SendCMD( CTesterIFCmd* pPacket )
{
	if( pPacket == NULL ) return ERROR_INVALID_DATA;

 	if( ((CIFComm*)m_pTesterIF_Imp)->IsConnected() != TRUE ){
 		delete pPacket;
 		pPacket = NULL;
 		return ERROR_NOT_CONNECTED;
 	}

	((CIFComm*)m_pTesterIF_Imp)->AddSndCMD( pPacket );

	return ERROR_SUCCESS;
}

int CTESTER_IF_Cont::SRQLineCheck()
{
	int nRes = 0;
	
	nRes = ((CIFComm*)m_pTesterIF_Imp)->SRQLineCheck();
	
	return nRes;
}

int CTESTER_IF_Cont::SendCMD_Fast( CTesterIFCmd* pPacket )
{
	if( pPacket == NULL ) return ERROR_INVALID_DATA;

	if( ((CIFComm*)m_pTesterIF_Imp)->IsConnected() != TRUE ){
		delete pPacket;
		pPacket = NULL;
		return ERROR_NOT_CONNECTED;
	}
	((CIFComm_GPIB*)m_pTesterIF_Imp)->AddSndCMD_Fast( pPacket );
	
	return ERROR_SUCCESS;
}

/*=============================================================================
  AUTHOR   : ±è¼±Áø
  NAME     : MakeCmd
  PARAMS   : nCmd : Communication Command
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CTesterIFCmd* CTESTER_IF_Cont::MakeCmd(CString strCmd, CParamList& cParamList)
{
	CTesterIFCmd* cIFCmd = NULL;//new CTesterIFCmd;

	if( strCmd == TESTER_CMD_HTOT_FULLSITE )
	{
		cIFCmd = ((CIFComm*)m_pTesterIF_Imp)->m_fnMakeFullsite(cParamList);
	}
	else if( strCmd == TESTER_CMD_HTOT_ECHO )
	{
		cIFCmd = ((CIFComm*)m_pTesterIF_Imp)->m_fnMakeEcho(cParamList);
	}
	else
	{
		// not allowed cmd
		cIFCmd = NULL;
	}
	
	return cIFCmd;
}


