#pragma once

#include "DataMem.h"
#include "DEF_DM_0.h"
#include "DEF_DM_1.h"
#include "DEF_DM_2.h"
#include "DEF_DM_3.h"
#include "DEF_DM_4.h"
#include "DEF_DM_5.h"
#include "DEF_DM_6.h"
#include "DEF_DM_7.h"
#include "DEF_DM_8.h"
#include "DEF_DM_9.h"
#include "DEF_DM_10.h"
#include "DEF_DM_11.h"
#include "DEF_DM_12.h"
#include "DEF_DM_13.h"
#include "DEF_DM_14.h"

class CDataMemCont
{
public:
	CDataMemCont(void);
	~CDataMemCont(void);

	CDataMem	m_dmEQP;
	CDataMem	m_dmContac_Dn;
	CDataMem	m_dmContac_Up;
	CDataMem	m_dmShuttle;
	CDataMem	m_dmTestPP;
	CDataMem	m_dmTrayPP;
	CDataMem	m_dmSetplate;
	CDataMem	m_dmStacker;
	CDataMem	m_dmStackerXfer;
	CDataMem	m_dmCTrayXfer;
	CDataMem	m_dmExStacker;
	CDataMem	m_dmHandlerOpt;
	CDataMem	m_dmCOK;
	CDataMem	m_dm2DID;
	CDataMem	m_dmFeeder;
	CDataMem	m_dmVAT;

	int Create();
	void Destroy();

};

