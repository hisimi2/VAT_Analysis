#include "StdAfx.h"
#include "DeviceDataMem_Imp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDeviceDataMem_Imp::CDeviceDataMem_Imp(void)
{
	m_pData = NULL;
	m_pcMapName = NULL;

	m_pSync = NULL;

	m_pSync = new CRITICAL_SECTION;

	try
	{
		::InitializeCriticalSection(m_pSync);
	}
	catch(...)
	{
		AfxMessageBox("CLogger Initialize Error !!!");
	}
}


CDeviceDataMem_Imp::~CDeviceDataMem_Imp(void)
{
	DestroyMapData();
	if( m_pcMapName != NULL ){
		delete [] m_pcMapName;
		m_pcMapName = NULL;
	}

	if(m_pSync){
		::DeleteCriticalSection(m_pSync);
		delete m_pSync; 
		m_pSync=NULL;		
	}
}

int	   CDeviceDataMem_Imp::CreateMapData(LPCTSTR strMapName)
{
	int nLen = strlen(strMapName) + 1;
	m_pcMapName = new char[nLen];
	memset(m_pcMapName, 0x00, nLen);
	strcpy_s(m_pcMapName, nLen, strMapName);

	m_fnCreateDirectory(SZ_DATA_MEM_PATH);

	char szFileName[MAX_PATH]={0,};
	sprintf_s(szFileName, sizeof(szFileName), "%s\\%s.map", SZ_DATA_MEM_PATH, strMapName);
	BOOL bCreated = m_SharedMem.Create(szFileName, strMapName, sizeof(ST_DEVICE_DATA_MEM), _MODE_READWRITE);
	if( bCreated != TRUE ){
		CString strMsg;
		strMsg.Format("can not create %s.", strMapName);
		AfxMessageBox(strMsg);
		return -1;
	}

	m_pData = m_SharedMem.GetPoint();
	
	return 0;
}

int    CDeviceDataMem_Imp::OpenMapData(LPCTSTR strMapName)
{
	int nLen = strlen(strMapName) + 1;
	m_pcMapName = new char[nLen];
	memset(m_pcMapName, 0x00, nLen);
	strcpy_s(m_pcMapName, nLen, strMapName);

	m_fnCreateDirectory(SZ_DATA_MEM_PATH);

	BOOL bCreated = m_SharedMem.Open(strMapName, _MODE_READWRITE);
	if( bCreated != TRUE ){
		CString strMsg;
		strMsg.Format("can not open %s.", strMapName);
		AfxMessageBox(strMsg);
		return -1;
	}

	m_pData = m_SharedMem.GetPoint();

	return 0;
}

void   CDeviceDataMem_Imp::DestroyMapData()
{

}

// pocket이 있는 Location (Setplate, Tray PP, Loading Table, Test PP)
int CDeviceDataMem_Imp::GetDeviceData(int nLoc, int nX, int nY, ST_DD_DEVICE* lpstBuff)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );
	ASSERT( (nX>=0)   && (nX<DEVICE_DATA_MAX_X) );
	ASSERT( (nY>=0)   && (nY<DEVICE_DATA_MAX_Y) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[nY][nX]);	
	memcpy(lpstBuff, lpstData, sizeof(ST_DD_DEVICE));

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}

int CDeviceDataMem_Imp::SetDeviceData(int nLoc, int nX, int nY, ST_DD_DEVICE* lpstBuff)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );
	ASSERT( (nX>=0)   && (nX<DEVICE_DATA_MAX_X) );
	ASSERT( (nY>=0)   && (nY<DEVICE_DATA_MAX_Y) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[nY][nX]);	
	memcpy(lpstData, lpstBuff, sizeof(ST_DD_DEVICE));

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}

int CDeviceDataMem_Imp::GetDeviceExist(int nLoc, int nX, int nY)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );
	ASSERT( (nX>=0)   && (nX<DEVICE_DATA_MAX_X) );
	ASSERT( (nY>=0)   && (nY<DEVICE_DATA_MAX_Y) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[nY][nX]);	
	int nExist = lpstData->sExist;

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return nExist;
}

int CDeviceDataMem_Imp::SetDeviceExist(int nLoc, int nX, int nY, int nExist)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );
	ASSERT( (nX>=0)   && (nX<DEVICE_DATA_MAX_X) );
	ASSERT( (nY>=0)   && (nY<DEVICE_DATA_MAX_Y) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[nY][nX]);	
	lpstData->sExist = nExist;

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}

int CDeviceDataMem_Imp::GetDeviceBinResult(int nLoc, int nX, int nY, char* szBinResult, int nSize)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );
	ASSERT( (nX>=0)   && (nX<DEVICE_DATA_MAX_X) );
	ASSERT( (nY>=0)   && (nY<DEVICE_DATA_MAX_Y) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[nY][nX]);	
	strncpy_s(szBinResult, nSize, lpstData->szBinResult, strlen(lpstData->szBinResult)+1);

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}
int CDeviceDataMem_Imp::SetDeviceBinResult(int nLoc, int nX, int nY, char* szBinResult)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );
	ASSERT( (nX>=0)   && (nX<DEVICE_DATA_MAX_X) );
	ASSERT( (nY>=0)   && (nY<DEVICE_DATA_MAX_Y) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[nY][nX]);	
	strncpy_s(lpstData->szBinResult, sizeof(lpstData->szBinResult), szBinResult, strlen(szBinResult)+1);

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}

int CDeviceDataMem_Imp::GetDeviceBinTestCount(int nLoc, int nX, int nY)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );
	ASSERT( (nX>=0)   && (nX<DEVICE_DATA_MAX_X) );
	ASSERT( (nY>=0)   && (nY<DEVICE_DATA_MAX_Y) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[nY][nX]);	
	int nTestCnt = lpstData->nTestingCnt;

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return nTestCnt;
}
int CDeviceDataMem_Imp::SetDeviceBinTestCount(int nLoc, int nX, int nY, int nTestCnt)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );
	ASSERT( (nX>=0)   && (nX<DEVICE_DATA_MAX_X) );
	ASSERT( (nY>=0)   && (nY<DEVICE_DATA_MAX_Y) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[nY][nX]);	
	lpstData->nTestingCnt = nTestCnt;

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}

int CDeviceDataMem_Imp::GetDeviceTestSiteNum(int nLoc, int nX, int nY, char* szTestSiteNum, int nSize)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );
	ASSERT( (nX>=0)   && (nX<DEVICE_DATA_MAX_X) );
	ASSERT( (nY>=0)   && (nY<DEVICE_DATA_MAX_Y) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[nY][nX]);	
	strncpy_s(szTestSiteNum, nSize, lpstData->szTestSiteNum, strlen(lpstData->szTestSiteNum)+1);

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}

int CDeviceDataMem_Imp::SetDeviceTestSiteNum(int nLoc, int nX, int nY, char* szTestSiteNum)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );
	ASSERT( (nX>=0)   && (nX<DEVICE_DATA_MAX_X) );
	ASSERT( (nY>=0)   && (nY<DEVICE_DATA_MAX_Y) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[nY][nX]);	
	strncpy_s(lpstData->szTestSiteNum, sizeof(lpstData->szTestSiteNum), szTestSiteNum, strlen(szTestSiteNum)+1);

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}

// pocket이 없는 Location (Press Unit, Test Site), 0,0을 사용 한다.
int CDeviceDataMem_Imp::GetDeviceData(int nLoc, ST_DD_DEVICE* lpstBuff)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[0][0]);	
	memcpy(lpstBuff, lpstData, sizeof(ST_DD_DEVICE));

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}

int CDeviceDataMem_Imp::SetDeviceData(int nLoc, ST_DD_DEVICE* lpstBuff)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[0][0]);	
	memcpy(lpstData, lpstBuff, sizeof(ST_DD_DEVICE));

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}

int CDeviceDataMem_Imp::GetDeviceCount( int nLoc )
{
	ASSERT( ( nLoc >= 0 ) && ( nLoc<DEVICE_DATA_MAX_STAGE_LOC ) );

	if( m_pSync ) ::EnterCriticalSection( m_pSync );

	ST_DD_STAGE* lpstData = ( ST_DD_STAGE* )( &( ( ST_DEVICE_DATA_MEM* )m_pData )->Loc[ nLoc ] );
	int RowMax = lpstData->TrayData.nRowMax;
	int ColMax = lpstData->TrayData.nColMax;
	int cnt = 0;
	for( int row = 0; row < RowMax; row++ ) {
		for( int col = 0; col < ColMax; col++ ) {
			if( lpstData->Device[ row ][ col ].sExist == 1 )
				cnt++;
		}
	}

	if( m_pSync ) ::LeaveCriticalSection( m_pSync );

	return cnt;
}

int CDeviceDataMem_Imp::GetDeviceExist(int nLoc)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[0][0]);	
	int nExist = lpstData->sExist;

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return nExist;
}
int CDeviceDataMem_Imp::SetDeviceExist(int nLoc, int nExist)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[0][0]);	
	lpstData->sExist = nExist;

	if(m_pSync) ::LeaveCriticalSection(m_pSync);
	
	return 0;
}

int CDeviceDataMem_Imp::GetDeviceBinResult(int nLoc, char* szBinResult, int nSize)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[0][0]);	
	strncpy_s(szBinResult, nSize, lpstData->szBinResult, strlen(lpstData->szBinResult)+1);

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}
int CDeviceDataMem_Imp::SetDeviceBinResult(int nLoc, char* szBinResult)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[0][0]);	
	strncpy_s(lpstData->szBinResult, sizeof(lpstData->szBinResult), szBinResult, strlen(szBinResult)+1);

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}

int CDeviceDataMem_Imp::GetDeviceBinTestCount(int nLoc)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[0][0]);
	int nTestCnt = lpstData->nTestingCnt;

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return nTestCnt;
}
int CDeviceDataMem_Imp::SetDeviceBinTestCount(int nLoc, int nTestCnt)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[0][0]);	
	lpstData->nTestingCnt = nTestCnt;

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}
int CDeviceDataMem_Imp::GetDeviceTestSiteNum(int nLoc, char* szTestSiteNum, int nSize)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[0][0]);	
	strncpy_s(szTestSiteNum, nSize, lpstData->szTestSiteNum, strlen(lpstData->szTestSiteNum)+1);

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}
int CDeviceDataMem_Imp::SetDeviceTestSiteNum(int nLoc, char* szTestSiteNum)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[0][0]);	
	strncpy_s(lpstData->szTestSiteNum, sizeof(lpstData->szTestSiteNum), szTestSiteNum, strlen(szTestSiteNum)+1);

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}

int CDeviceDataMem_Imp::GetSiteDvcTestingInfo(int nLoc)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[0][0]);	
	int nInfo = lpstData->nTestingInfo;

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return nInfo;
}
int CDeviceDataMem_Imp::SetSiteDvcTestingInfo(int nLoc, int nTestStatus)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[0][0]);	
	lpstData->nTestingInfo = nTestStatus;

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}

int CDeviceDataMem_Imp::GetSiteDvcTestingInfo(int nLoc, int nX, int nY)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[nY][nX]);	
	int nInfo = lpstData->nTestingInfo;

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return nInfo;
}
int CDeviceDataMem_Imp::SetSiteDvcTestingInfo(int nLoc, int nTestStatus, int nX, int nY)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_DEVICE* lpstData = (ST_DD_DEVICE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc].Device[nY][nX]);	
	lpstData->nTestingInfo = nTestStatus;

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}

// Location에 의한 접근.
int CDeviceDataMem_Imp::GetDeviceData(int nLoc, ST_DD_STAGE* lpstBuff)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_STAGE* lpstData = (ST_DD_STAGE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc]);	
	memcpy(lpstBuff, lpstData, sizeof(ST_DD_STAGE));

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}
int CDeviceDataMem_Imp::SetDeviceData(int nLoc, ST_DD_STAGE* lpstBuff)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_STAGE* lpstData = (ST_DD_STAGE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc]);	
	memcpy(lpstData, lpstBuff, sizeof(ST_DD_STAGE));

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}

BOOL CDeviceDataMem_Imp::m_fnCreateDirectory( LPCSTR pszDirectory )
{
	CFileStatus fs;
	if ( CFile::GetStatus( pszDirectory, fs ) )
	{
		if ( (fs.m_attribute & FILE_ATTRIBUTE_DIRECTORY) != 0 )
		{
			return TRUE;
		}
	}

	CHAR szTemp[MAX_PATH]={0,};
	LPSTR pszFilename;
	if ( !GetFullPathName( pszDirectory, sizeof( szTemp ), szTemp, &pszFilename ) )
	{
		return FALSE;
	}

	if ( pszFilename == NULL )
	{
		szTemp[lstrlen( szTemp ) - 1] = '\0';
		if( szTemp[lstrlen( szTemp ) - 1] == ':' )
			return TRUE;
		if( szTemp[lstrlen( szTemp ) - 2] == '\\' )
			return TRUE;
		return m_fnCreateDirectory( szTemp );
	}

	if ( pszFilename >= &szTemp[2] )
	{
		if ( pszFilename[-2] == ':' )
		{
			return CreateDirectory( szTemp, NULL );
		}
	}

	pszFilename[-1] = '\0';

	if ( !CFile::GetStatus( szTemp, fs ) )
	{
		if ( !m_fnCreateDirectory( szTemp ) )
		{
			return FALSE;
		}
		pszFilename[-1] = '\\';
		return CreateDirectory( szTemp, NULL );
	}

	if ( (fs.m_attribute & FILE_ATTRIBUTE_DIRECTORY) != 0 )
	{
		pszFilename[-1] = '\\';
		return CreateDirectory( szTemp, NULL );
	}

	return FALSE;
}

int CDeviceDataMem_Imp::GetTrayData(int nLoc, ST_DD_TRAY& TrayData)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_STAGE* lpstData = (ST_DD_STAGE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc]);	
	
	TrayData = lpstData->TrayData;

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}
int CDeviceDataMem_Imp::SetTrayData(int nLoc, ST_DD_TRAY TrayData)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_STAGE* lpstData = (ST_DD_STAGE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc]);	

	lpstData->TrayData = TrayData;

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}

int CDeviceDataMem_Imp::GetTrayExist(int nLoc)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );
	BOOL bTrayExist = 0;

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_STAGE* lpstData = (ST_DD_STAGE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc]);	

	bTrayExist = lpstData->TrayData.bTrayExist;

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return bTrayExist;
}
int CDeviceDataMem_Imp::SetTrayExist(int nLoc, BOOL bExist)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_STAGE* lpstData = (ST_DD_STAGE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc]);	

	lpstData->TrayData.bTrayExist = bExist;

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}

int CDeviceDataMem_Imp::GetTrayId(int nLoc, CString& strTrayId)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_STAGE* lpstData = (ST_DD_STAGE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc]);	

	strTrayId = lpstData->TrayData.szTrayId;

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}

int CDeviceDataMem_Imp::SetTrayId(int nLoc, CString strTrayId)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_STAGE* lpstData = (ST_DD_STAGE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc]);	

	ZeroMemory(lpstData->TrayData.szTrayId, sizeof(char)*32);
	sprintf_s(lpstData->TrayData.szTrayId, sizeof(char)*32, "%s", strTrayId.GetString() );

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}

BOOL CDeviceDataMem_Imp::IsEmptyTray(int nLoc)
{
	BOOL bEmptyTray = TRUE;
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_STAGE* lpstData = (ST_DD_STAGE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc]);
	for(int y=0; (y<DEVICE_DATA_MAX_Y)&&(TRUE==bEmptyTray); y++){
		for(int x=0; (x<DEVICE_DATA_MAX_X)&&(TRUE==bEmptyTray); x++){
			if( 0 != lpstData->Device[y][x].sExist && strcmp(lpstData->Device[y][x].szBinResult, "-") != 0 )
			{
				bEmptyTray = FALSE;
				break;
			}
		}
	}

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return bEmptyTray;
}


int CDeviceDataMem_Imp::GetStageData(int nLoc, ST_DD_STAGE& StageData)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_STAGE* lpstData = (ST_DD_STAGE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc]);	
	
	StageData = *lpstData;

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}

int CDeviceDataMem_Imp::SetStageData(int nLoc, ST_DD_STAGE StageData)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_STAGE* lpstData = (ST_DD_STAGE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc]);	

	*lpstData = StageData;

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}

int CDeviceDataMem_Imp::SwapStageData(int nFromLoc, int nToLoc)
{
	ASSERT( (nFromLoc>=0) && (nFromLoc<DEVICE_DATA_MAX_STAGE_LOC) );
	ASSERT( (nToLoc>=0) && (nToLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_STAGE* lpstDataFrom = (ST_DD_STAGE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nFromLoc]);	
	ST_DD_STAGE* lpstDataTo = (ST_DD_STAGE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nToLoc]);
	ST_DD_STAGE* pTempData = new ST_DD_STAGE;

	if( pTempData == NULL){
		ASSERT(0);
	}
	
	*pTempData = *lpstDataTo;
	*lpstDataTo = *lpstDataFrom;
	*lpstDataFrom = *pTempData;

	delete pTempData;

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}

int CDeviceDataMem_Imp::ClearDeviceData(int nLoc)
{
	ASSERT( (nLoc>=0) && (nLoc<DEVICE_DATA_MAX_STAGE_LOC) );

	if(m_pSync) ::EnterCriticalSection(m_pSync);

	ST_DD_STAGE* lpstData = (ST_DD_STAGE*)(&((ST_DEVICE_DATA_MEM*)m_pData)->Loc[nLoc]);	
	lpstData->clear();

	if(m_pSync) ::LeaveCriticalSection(m_pSync);

	return 0;
}

int CDeviceDataMem_Imp::ClearDeviceDataAll()
{
	for(int i=0; i<DEVICE_DATA_MAX_STAGE_LOC; i++){
		if (i == eDeviceStage_TEST_PP_1_CLEAN_BUFF || i == eDeviceStage_TEST_PP_2_CLEAN_BUFF)
			continue;
		//if( i >= eDeviceStage_TEST_PP_1_BUFF && i <= eDeviceStage_TEST_PP_4_BUFF){
		//	continue; // buffer table is selection UI
		//}
		ClearDeviceData(i);
	}
	return 0;
}
