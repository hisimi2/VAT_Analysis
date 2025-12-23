#include "StdAfx.h"
#include "SankyoDrv.h"
#include "SankyoDrv_Imp.h"

IMPLEMENT_DYNAMIC(CSankyoDrv, CWnd)

CSankyoDrv::CSankyoDrv(void)
{
	m_pDrvObj = NULL;
}


CSankyoDrv::~CSankyoDrv(void)
{
	if( m_pDrvObj ){
		((CSankyoDrv_Imp*)m_pDrvObj)->Finalize();
		delete m_pDrvObj;
		m_pDrvObj = NULL;
	}
}


BEGIN_MESSAGE_MAP(CSankyoDrv, CWnd)
END_MESSAGE_MAP()

int CSankyoDrv::Initialize(Sankyo::ST_SANKYO_INIT_INFO stInitInfo)
{
	m_pDrvObj = new CSankyoDrv_Imp;
	return ((CSankyoDrv_Imp*)m_pDrvObj)->Initialize(stInitInfo);
}

void CSankyoDrv::Finalize()
{
	((CSankyoDrv_Imp*)m_pDrvObj)->Finalize();
}

BOOL CSankyoDrv::IsConnected()
{
	return ((CSankyoDrv_Imp*)m_pDrvObj)->IsConnected();
}

BOOL CSankyoDrv::IsAutoRunning()
{
	return ((CSankyoDrv_Imp*)m_pDrvObj)->IsAutoRunning();
}

double CSankyoDrv::GetTorque(int nStation)
{
	return ((CSankyoDrv_Imp*)m_pDrvObj)->GetTorque(nStation);
}

void CSankyoDrv::GetTorqueAll(std::vector<double>& vCont)
{
	return ((CSankyoDrv_Imp*)m_pDrvObj)->GetTorqueAll(vCont);
}

void CSankyoDrv::ShowManualDialog()
{
	((CSankyoDrv_Imp*)m_pDrvObj)->ShowManualDialog();
}

void CSankyoDrv::Op_Action(int nAction)
{
	((CSankyoDrv_Imp*)m_pDrvObj)->Op_Action(nAction);
}

void CSankyoDrv::Op_RunMode(int nRunMode)
{
	((CSankyoDrv_Imp*)m_pDrvObj)->Op_RunMode(nRunMode);
}

int CSankyoDrv::Setup_Subscription(int nStation, bool bEnable)
{
	return ((CSankyoDrv_Imp*)m_pDrvObj)->Setup_Subscription(nStation, bEnable);
}

int CSankyoDrv::Setup_Subscription(std::list<int> vSubscriptionList)
{
	return ((CSankyoDrv_Imp*)m_pDrvObj)->Setup_Subscription(vSubscriptionList);
}

bool CSankyoDrv::Setup_GetSubscription(int nStation)
{
	return ((CSankyoDrv_Imp*)m_pDrvObj)->Setup_GetSubscription(nStation);
}

void CSankyoDrv::Setup_GetSubscriptionAll(std::list<int>& vSubscriptedStation)
{
	((CSankyoDrv_Imp*)m_pDrvObj)->Setup_GetSubscriptionAll(vSubscriptedStation);
}

BOOL CSankyoDrv::GetAutoRunningHeartBit()
{
	return ((CSankyoDrv_Imp*)m_pDrvObj)->GetAutoRunningHeartBit();
}