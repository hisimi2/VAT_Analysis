#include "StdAfx.h"
#include "DataMemCont.h"


CDataMemCont::CDataMemCont(void)
{
}


CDataMemCont::~CDataMemCont(void)
{
}


int CDataMemCont::Create()
{
	int nRet = 0;
	m_dmEQP.Create(SZ_DM_NAME_0);
	m_dmContac_Dn.Create(SZ_DM_NAME_1);
	m_dmContac_Up.Create(SZ_DM_NAME_2);
	m_dmShuttle.Create(SZ_DM_NAME_3);
	m_dmTestPP.Create(SZ_DM_NAME_4);
	m_dmTrayPP.Create(SZ_DM_NAME_5);
	m_dmSetplate.Create(SZ_DM_NAME_6);
	m_dmStacker.Create(SZ_DM_NAME_7);
	m_dmStackerXfer.Create(SZ_DM_NAME_8);
	m_dmCTrayXfer.Create(SZ_DM_NAME_9);
	//m_dmExStacker.Create(SZ_DM_NAME_10);
	m_dmHandlerOpt.Create(SZ_DM_NAME_11);
	m_dmCOK.Create(SZ_DM_NAME_12);
	m_dm2DID.Create(SZ_DM_NAME_13);
	m_dmFeeder.Create(SZ_DM_NAME_10);
	m_dmVAT.Create(SZ_DM_NAME_14);
	return nRet;
}

void CDataMemCont::Destroy()
{
	m_dmEQP.Destroy();
	m_dmContac_Dn.Destroy();
	m_dmContac_Up.Destroy();
	m_dmShuttle.Destroy();
	m_dmTestPP.Destroy();
	m_dmTrayPP.Destroy();
	m_dmSetplate.Destroy();
	m_dmStacker.Destroy();
	m_dmStackerXfer.Destroy();
	m_dmCTrayXfer.Destroy();
	//m_dmExStacker.Destroy();
	m_dmHandlerOpt.Destroy();
	m_dmCOK.Destroy();
	m_dm2DID.Destroy();
	m_dmFeeder.Destroy();
	m_dmVAT.Destroy();
}
