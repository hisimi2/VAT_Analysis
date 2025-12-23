#include "StdAfx.h"
#include "DeviceDataMem.h"
#include "DeviceDataMem_Imp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDeviceDataMem::CDeviceDataMem(void)
{
	m_pDataObj = NULL;
}


CDeviceDataMem::~CDeviceDataMem(void)
{
	DestroyMapData();
}


int CDeviceDataMem::CreateMapData(LPCTSTR strMapName)
{
	m_pDataObj = new CDeviceDataMem_Imp;
	return ((CDeviceDataMem_Imp*)m_pDataObj)->CreateMapData(strMapName);
}

int CDeviceDataMem::OpenMapData(LPCTSTR strMapName)
{
	m_pDataObj = new CDeviceDataMem_Imp;
	return ((CDeviceDataMem_Imp*)m_pDataObj)->OpenMapData(strMapName);
}
void CDeviceDataMem::DestroyMapData()
{
	if( m_pDataObj != NULL ){
		((CDeviceDataMem_Imp*)m_pDataObj)->DestroyMapData();
		delete m_pDataObj;
		m_pDataObj = NULL;
	}
}

// pocket이 있는 Location (Setplate, Tray PP, Loading Table, Test PP)
int CDeviceDataMem::GetDeviceData(int nLoc, int nX, int nY, ST_DD_DEVICE* lpstBuff)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->GetDeviceData(nLoc, nX, nY, lpstBuff);
}
int CDeviceDataMem::SetDeviceData(int nLoc, int nX, int nY, ST_DD_DEVICE* lpstBuff)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->SetDeviceData(nLoc, nX, nY, lpstBuff);
}

int CDeviceDataMem::GetDeviceExist(int nLoc, int nX, int nY)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->GetDeviceExist(nLoc, nX, nY);
}
int CDeviceDataMem::SetDeviceExist(int nLoc, int nX, int nY, int nExist)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->SetDeviceExist(nLoc, nX, nY, nExist);
}

int CDeviceDataMem::GetDeviceBinResult(int nLoc, int nX, int nY, char* szBinResult, int nSize)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->GetDeviceBinResult(nLoc, nX, nY, szBinResult, nSize);
}
int CDeviceDataMem::SetDeviceBinResult(int nLoc, int nX, int nY, char* szBinResult)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->SetDeviceBinResult(nLoc, nX, nY, szBinResult);
}

int CDeviceDataMem::GetDeviceBinTestCount(int nLoc, int nX, int nY)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->GetDeviceBinTestCount(nLoc, nX, nY);
}
int CDeviceDataMem::SetDeviceBinTestCount(int nLoc, int nX, int nY, int nTestCnt)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->SetDeviceBinTestCount(nLoc, nX, nY, nTestCnt);
}
int CDeviceDataMem::GetDeviceTestSiteNum(int nLoc, int nX, int nY, char* szTestSiteNum, int nSize)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->GetDeviceTestSiteNum(nLoc, nX, nY, szTestSiteNum, nSize);
}
int CDeviceDataMem::SetDeviceTestSiteNum(int nLoc, int nX, int nY, char* szTestSiteNum)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->SetDeviceTestSiteNum(nLoc, nX, nY, szTestSiteNum);
}

// pocket이 없는 Location (Press Unit, Test Site), 0,0을 사용 한다.
int CDeviceDataMem::GetDeviceData(int nLoc, ST_DD_DEVICE* lpstBuff)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->GetDeviceData(nLoc, lpstBuff);
}
int CDeviceDataMem::SetDeviceData(int nLoc, ST_DD_DEVICE* lpstBuff)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->SetDeviceData(nLoc, lpstBuff);
}

int CDeviceDataMem::GetDeviceCount( int nLoc )
{
	return ( ( CDeviceDataMem_Imp* )m_pDataObj )->GetDeviceCount( nLoc );
}

int CDeviceDataMem::GetDeviceExist(int nLoc)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->GetDeviceExist(nLoc);
}
int CDeviceDataMem::SetDeviceExist(int nLoc, int nExist)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->SetDeviceExist(nLoc, nExist);
}

int CDeviceDataMem::GetDeviceBinResult(int nLoc, char* szBinResult, int nSize)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->GetDeviceBinResult(nLoc, szBinResult, nSize);
}
int CDeviceDataMem::SetDeviceBinResult(int nLoc, char* szBinResult)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->SetDeviceBinResult(nLoc, szBinResult);
}

int CDeviceDataMem::GetSiteDvcTestingInfo(int nLoc)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->GetSiteDvcTestingInfo(nLoc);
}
int CDeviceDataMem::SetSiteDvcTestingInfo(int nLoc, int nTestStatus)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->SetSiteDvcTestingInfo(nLoc, nTestStatus);
}

int CDeviceDataMem::GetSiteDvcTestingInfo(int nLoc, int nX, int nY)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->GetSiteDvcTestingInfo(nLoc, nX, nY);
}
int CDeviceDataMem::SetSiteDvcTestingInfo(int nLoc, int nTestStatus, int nX, int nY)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->SetSiteDvcTestingInfo(nLoc, nTestStatus, nX, nY);
}


// Location에 의한 접근.
int CDeviceDataMem::GetDeviceData(int nLoc, ST_DD_STAGE* lpstBuff)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->GetDeviceData(nLoc, lpstBuff);
}

int CDeviceDataMem::SetDeviceData(int nLoc, ST_DD_STAGE* lpstBuff)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->SetDeviceData(nLoc, lpstBuff);
}


int CDeviceDataMem::GetTrayData(int nLoc, ST_DD_TRAY& TrayData)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->GetTrayData(nLoc, TrayData);
}
int CDeviceDataMem::SetTrayData(int nLoc, ST_DD_TRAY TrayData)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->SetTrayData(nLoc, TrayData);
}

int CDeviceDataMem::GetTrayExist(int nLoc)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->GetTrayExist(nLoc);
}
int CDeviceDataMem::SetTrayExist(int nLoc, BOOL bExist)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->SetTrayExist(nLoc, bExist);
}

int CDeviceDataMem::GetTrayId(int nLoc, CString& strTrayId)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->GetTrayId(nLoc, strTrayId);
}
int CDeviceDataMem::SetTrayId(int nLoc, CString strTrayId)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->SetTrayId(nLoc, strTrayId);
}

BOOL CDeviceDataMem::IsEmptyTray(int nLoc)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->IsEmptyTray(nLoc);
}

int CDeviceDataMem::GetStageData(int nLoc, ST_DD_STAGE& StageData)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->GetStageData(nLoc, StageData);
}
int CDeviceDataMem::SetStageData(int nLoc, ST_DD_STAGE StageData)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->SetStageData(nLoc, StageData);
}

int CDeviceDataMem::SwapStageData(int nFromLoc, int nToLoc)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->SwapStageData(nFromLoc, nToLoc);
}

int CDeviceDataMem::ClearDeviceData(int nLoc)
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->ClearDeviceData(nLoc);
}

int CDeviceDataMem::ClearDeviceDataAll()
{
	return ((CDeviceDataMem_Imp*)m_pDataObj)->ClearDeviceDataAll();
}
