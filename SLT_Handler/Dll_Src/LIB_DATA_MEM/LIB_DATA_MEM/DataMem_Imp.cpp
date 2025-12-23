#include "StdAfx.h"
#include "DataMem_Imp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDataMem_Imp::CDataMem_Imp(void)
{
	m_pData = NULL;
	m_pcMapName = NULL;
}


CDataMem_Imp::~CDataMem_Imp(void)
{
	DestroyMapData();
	if( m_pcMapName != NULL ){
		delete [] m_pcMapName;
		m_pcMapName = NULL;
	}
}

int CDataMem_Imp::CreateMapData(LPCTSTR strMapName)
{
	int nLen = strlen(strMapName) + 1;
	m_pcMapName = new char[nLen];
	memset(m_pcMapName, 0x00, nLen);
	strcpy_s(m_pcMapName, nLen, strMapName);

	m_fnCreateDirectory(SZ_DATA_MEM_PATH);

	char szFileName[MAX_PATH]={0,};
	sprintf_s(szFileName, sizeof(szFileName), "%s\\%s.map", SZ_DATA_MEM_PATH, strMapName);
	BOOL bCreated = m_SharedMem.Create(szFileName, strMapName, sizeof(ST_DATA_MEM), _MODE_READWRITE);
	if( bCreated != TRUE ){
		CString strMsg;
		strMsg.Format("can not create %s.", strMapName);
		AfxMessageBox(strMsg);
		return -1;
	}

	m_pData = m_SharedMem.GetPoint();

	return 0;
}

int CDataMem_Imp::OpenMapData(LPCTSTR strMapName)
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

void CDataMem_Imp::DestroyMapData()
{
}

BOOL CDataMem_Imp::GB(int nOffset)
{
	ASSERT( nOffset>=0 && nOffset < MAX_DM_ITEM_COUNT);
	if( nOffset<0 || nOffset >= MAX_DM_ITEM_COUNT ) return FALSE;
	return ((ST_DATA_MEM*)m_pData)->B[nOffset];
}

int CDataMem_Imp::SB(int nOffset, BOOL bData)
{
	ASSERT( nOffset>=0 && nOffset < MAX_DM_ITEM_COUNT);
	if( nOffset<0 || nOffset >= MAX_DM_ITEM_COUNT ) return -1;
	((ST_DATA_MEM*)m_pData)->B[nOffset] = bData;
	return 0;
}

int CDataMem_Imp::GN(int nOffset)
{
	ASSERT( nOffset>=0 && nOffset < MAX_DM_ITEM_COUNT);
	if( nOffset < 0 || nOffset >= MAX_DM_ITEM_COUNT ) return 0;
	return ((ST_DATA_MEM*)m_pData)->N[nOffset];
}

int CDataMem_Imp::SN(int nOffset, int nData)
{
	ASSERT( nOffset>=0 && nOffset < MAX_DM_ITEM_COUNT);
	if( nOffset<0 || nOffset >= MAX_DM_ITEM_COUNT ) return -1;
	((ST_DATA_MEM*)m_pData)->N[nOffset] = nData;
	return 0;
}

double CDataMem_Imp::GD(int nOffset)
{
	ASSERT( nOffset>=0 && nOffset < MAX_DM_ITEM_COUNT);
	if( nOffset < 0 || nOffset >= MAX_DM_ITEM_COUNT ) return 0.0;
	return ((ST_DATA_MEM*)m_pData)->D[nOffset];
}

int CDataMem_Imp::SD(int nOffset, double dData)
{
	ASSERT( nOffset>=0 && nOffset < MAX_DM_ITEM_COUNT);
	if( nOffset < 0 || nOffset >= MAX_DM_ITEM_COUNT ) return -1;
	((ST_DATA_MEM*)m_pData)->D[nOffset] = dData;
	return 0;
}

int CDataMem_Imp::GS(int nOffset, char* pcBuff, int nBuffsize)
{
	ASSERT( nOffset>=0 && nOffset < MAX_DM_ITEM_COUNT);
	if( nOffset < 0 || nOffset >= MAX_DM_ITEM_COUNT ) return -1;
	sprintf_s(pcBuff, nBuffsize, ((ST_DATA_MEM*)m_pData)->S[nOffset]);
	return 0;
}

int CDataMem_Imp::SS(int nOffset, char* pcData, int nSize)
{
	ASSERT( nOffset>=0 && nOffset < MAX_DM_ITEM_COUNT);
	if( nOffset < 0 || nOffset >= MAX_DM_ITEM_COUNT ) return -1;

	memset(((ST_DATA_MEM*)m_pData)->S[nOffset], 0x00, MAX_DM_S_ITEM_LEN);
	sprintf_s(((ST_DATA_MEM*)m_pData)->S[nOffset], MAX_DM_S_ITEM_LEN, pcData);
	return 0;
}

void CDataMem_Imp::BroadCast()
{
	
}

int CDataMem_Imp::RegisterObserver(CDataMemObserver* pObserver)
{
	return m_listDataMemObserver.m_fnAddData(pObserver);
}

int CDataMem_Imp::UnregisterObserver(CDataMemObserver* pObserver)
{
	m_listDataMemObserver.m_fnDeleteData(pObserver);
	return 0;
}


// void CDataMem_Imp::ThreadProc1()
// {
// }


BOOL CDataMem_Imp::m_fnCreateDirectory( LPCSTR pszDirectory )
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
