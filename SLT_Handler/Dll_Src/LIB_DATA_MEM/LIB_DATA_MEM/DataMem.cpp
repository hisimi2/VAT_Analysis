#include "StdAfx.h"
#include "DataMem.h"
#include "DataMem_Imp.h"


CDataMem::CDataMem(void)
{
	m_pDataObj = NULL;
}


CDataMem::~CDataMem(void)
{
	Destroy();
}


int    CDataMem::Create(LPCTSTR strMapName)
{
	m_pDataObj = new CDataMem_Imp;
	return ((CDataMem_Imp*)m_pDataObj)->CreateMapData(strMapName);
}

int    CDataMem::Open(LPCTSTR strMapName)
{
	m_pDataObj = new CDataMem_Imp;
	return ((CDataMem_Imp*)m_pDataObj)->OpenMapData(strMapName);
}
void   CDataMem::Destroy()
{
	if( m_pDataObj != NULL ){
		((CDataMem_Imp*)m_pDataObj)->DestroyMapData();
		delete m_pDataObj;
		m_pDataObj = NULL;
	}
}

BOOL   CDataMem::GB(int nOffset)
{
	return ((CDataMem_Imp*)m_pDataObj)->GB(nOffset);
}

int    CDataMem::SB(int nOffset, BOOL bData)
{
	return ((CDataMem_Imp*)m_pDataObj)->SB(nOffset, bData);
}

int    CDataMem::GN(int nOffset)
{
	return ((CDataMem_Imp*)m_pDataObj)->GN(nOffset);
}

int    CDataMem::SN(int nOffset, int nData)
{
	return ((CDataMem_Imp*)m_pDataObj)->SN(nOffset, nData);
}

double CDataMem::GD(int nOffset)
{
	return ((CDataMem_Imp*)m_pDataObj)->GD(nOffset);
}
int    CDataMem::SD(int nOffset, double dData)
{
	return ((CDataMem_Imp*)m_pDataObj)->SD(nOffset, dData);
}

int    CDataMem::GS(int nOffset, char* pcBuff, int nBuffSize)
{
	return ((CDataMem_Imp*)m_pDataObj)->GS(nOffset, pcBuff, nBuffSize);
}
int    CDataMem::SS(int nOffset, char* pcData, int nLen)
{
	return ((CDataMem_Imp*)m_pDataObj)->SS(nOffset, pcData, nLen);
}

void   CDataMem::BroadCast()
{
	((CDataMem_Imp*)m_pDataObj)->BroadCast();
}

int    CDataMem::RegisterObserver(CDataMemObserver* pObserver)
{
	return ((CDataMem_Imp*)m_pDataObj)->RegisterObserver(pObserver);
}
int    CDataMem::UnregisterObserver(CDataMemObserver* pObserver)
{
	return ((CDataMem_Imp*)m_pDataObj)->UnregisterObserver(pObserver);
}
