#include "StdAfx.h"
#include "RegulatorIF.h"
#include "RegulatorComm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CRegulatorIF::CRegulatorIF(void)
{
	m_pParentWnd = NULL;
	m_pDrv = new CRegulatorComm;
	memset(m_szLogfilename, 0, sizeof(m_szLogfilename));
}


CRegulatorIF::~CRegulatorIF(void)
{
	delete m_pDrv;
}

BOOL CRegulatorIF::Initialize( CWnd* pParentWnd, ST_REGUL_DATA &stSetData, char* szLogFileName/*, FN_PARENT_CALLBACK cbAlarm */)
{
	m_stSetData = stSetData;
	m_pParentWnd = pParentWnd;
	sprintf_s(m_szLogfilename, sizeof(m_szLogfilename), szLogFileName);

	return ((CRegulatorComm*)m_pDrv)->InitRs232c(pParentWnd, stSetData, szLogFileName/*, cbAlarm*/);
}

void CRegulatorIF::Finalize()
{
	return ((CRegulatorComm*)m_pDrv)->Close();
}

BOOL CRegulatorIF::IsConnected()
{
	return ((CRegulatorComm*)m_pDrv)->IsConnected();
}

int CRegulatorIF::GetCommand()
{
	return ((CRegulatorComm*)m_pDrv)->GetCommand();
}

void CRegulatorIF::SetCommand( int nCommand, double dMPa /*= 0.0*/ )
{
	return ((CRegulatorComm*)m_pDrv)->SetCommand(nCommand, dMPa);
}

void CRegulatorIF::SetRegulator( double dSetPressure )
{
	return ((CRegulatorComm*)m_pDrv)->SetRegulator(dSetPressure);
}

BOOL CRegulatorIF::GetPress( double dRegulatorPermit )
{
	return ((CRegulatorComm*)m_pDrv)->RegulatorPressCheck(dRegulatorPermit);
}

int CRegulatorIF::GetDecFromMpa( double dMpa )
{
	return ((CRegulatorComm*)m_pDrv)->GetDecFromMpa(dMpa);
}

double CRegulatorIF::GetSettingMpa()
{
	return ((CRegulatorComm*)m_pDrv)->GetSettingMpa();
}

void CRegulatorIF::GetRecvData( int *nState, double *dMPa)
{
	return ((CRegulatorComm*)m_pDrv)->GetRecvData(nState, dMPa);
}

void CRegulatorIF::SetRecvData( int nState, int nDecMPa )
{
	return ((CRegulatorComm*)m_pDrv)->SetRecvData(nState, nDecMPa);
}

BOOL CRegulatorIF::SetReconnected()
{
	((CRegulatorComm*)m_pDrv)->Close();
	Sleep(50);
	return ((CRegulatorComm*)m_pDrv)->InitRs232c(m_pParentWnd, m_stSetData, m_szLogfilename/*, cbAlarm*/);
}


