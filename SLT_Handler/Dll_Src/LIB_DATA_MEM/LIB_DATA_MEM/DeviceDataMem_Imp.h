#pragma once

#include "ThreadBase.h"
#include "SharedMem.h"
#include "DEF_DEVICE_DATA.h"

//==============================================================================
//
//==============================================================================
class CDeviceDataMem_Imp : public CWnd
{
public:
	CDeviceDataMem_Imp(void);
	~CDeviceDataMem_Imp(void);

	int	   CreateMapData(LPCTSTR strMapName);
	int    OpenMapData(LPCTSTR strMapName);
	void   DestroyMapData();

	// pocket이 있는 Location (Setplate, Tray PP, Loading Table, Test PP)
	int GetDeviceData(int nLoc, int nX, int nY, ST_DD_DEVICE* lpstBuff);
	int SetDeviceData(int nLoc, int nX, int nY, ST_DD_DEVICE* lpstBuff);

	int GetDeviceExist(int nLoc, int nX, int nY);
	int SetDeviceExist(int nLoc, int nX, int nY, int nExist);

	int GetDeviceBinResult(int nLoc, int nX, int nY, char* szBinResult, int nSize);
	int SetDeviceBinResult(int nLoc, int nX, int nY, char* szBinResult);

	int GetDeviceBinTestCount(int nLoc, int nX, int nY);
	int SetDeviceBinTestCount(int nLoc, int nX, int nY, int nTestCnt);

	int GetDeviceTestSiteNum(int nLoc, int nX, int nY, char* szTestSiteNum, int nSize);
	int SetDeviceTestSiteNum(int nLoc, int nX, int nY, char* szTestSiteNum);

	// pocket이 없는 Location (Press Unit, Test Site), 0,0을 사용 한다.
	int GetDeviceData(int nLoc, ST_DD_DEVICE* lpstBuff);
	int SetDeviceData(int nLoc, ST_DD_DEVICE* lpstBuff);

	int GetDeviceCount( int nLoc );
	int GetDeviceExist(int nLoc);
	int SetDeviceExist(int nLoc, int nExist);

	int GetDeviceBinResult(int nLoc, char* szBinResult, int nSize);
	int SetDeviceBinResult(int nLoc, char* szBinResult);

	int GetDeviceBinTestCount(int nLoc);
	int SetDeviceBinTestCount(int nLoc, int nTestCnt);

	int GetDeviceTestSiteNum(int nLoc, char* szTestSiteNum, int nSize);
	int SetDeviceTestSiteNum(int nLoc, char* szTestSiteNum);

	int GetSiteDvcTestingInfo(int nLoc);
	int SetSiteDvcTestingInfo(int nLoc, int nTestStatus);
	int GetSiteDvcTestingInfo(int nLoc, int nX, int Y);
	int SetSiteDvcTestingInfo(int nLoc, int nTestStatus, int nX, int nY);

	// Location에 의한 접근.
	int GetDeviceData(int nLoc, ST_DD_STAGE* lpstBuff);
	int SetDeviceData(int nLoc, ST_DD_STAGE* lpstBuff);

	// Tray Data Handling
	int GetTrayData(int nLoc, ST_DD_TRAY& TrayData);
	int SetTrayData(int nLoc, ST_DD_TRAY TrayData);

	int GetTrayExist(int nLoc);
	int SetTrayExist(int nLoc, BOOL bExist);

	int GetTrayId(int nLoc, CString& strTrayId);
	int SetTrayId(int nLoc, CString strTrayId);

	BOOL IsEmptyTray(int nLoc);

	// Stage Data Handling
	int GetStageData(int nLoc, ST_DD_STAGE& StageData);
	int SetStageData(int nLoc, ST_DD_STAGE StageData);

	int SwapStageData(int nFromLoc, int nToLoc);

	int ClearDeviceData(int nLoc);

	int ClearDeviceDataAll();
	

private:
	CSharedMem					m_SharedMem;
	void*						m_pData;	// ST_DEVICE_DATA_MEM
	char*						m_pcMapName;

private:
	CRITICAL_SECTION* m_pSync;

private:
	BOOL    m_fnCreateDirectory( LPCSTR pszDirectory );
};

