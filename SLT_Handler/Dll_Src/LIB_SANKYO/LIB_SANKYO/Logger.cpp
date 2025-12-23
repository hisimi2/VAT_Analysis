#include "stdafx.h"
#include "Logger.h"

/*===================================================================

===================================================================*/
#define	MAX_LOG				100
#define	MAX_FILE_SIZE		3000000

#define LOG_SAVE_DAYS			(30)
/*===================================================================

===================================================================*/
CLogger::CLogger()
{
	m_cstrFileName = "log.txt";

	m_listLog.RemoveAll();

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
	m_nSaveDays = 0;
}


/*===================================================================

===================================================================*/
CLogger::~CLogger()
{
	WriteLog();

	m_listLog.RemoveAll();

	if(m_pSync){
		::DeleteCriticalSection(m_pSync);
		delete m_pSync; 
		m_pSync=NULL;		
	}
}


/*===================================================================

===================================================================*/
CString CLogger::GetFileName( void )
{
	return m_cstrFileName;
}

/*===================================================================

===================================================================*/
void CLogger::SetFileName( LPCTSTR szFileName )
{
 	m_cstrFileName = szFileName;

	int nIndex = m_cstrFileName.Find( ':' );
	if( nIndex == -1 )
	{
		char szBuff[128];
		::GetCurrentDirectory( sizeof(szBuff), szBuff );
		CString cstrFileName = '\\' + m_cstrFileName;
		m_cstrFileName = szBuff + cstrFileName;
	}

	nIndex = m_cstrFileName.ReverseFind( '\\' );
	if( nIndex != -1 )
	{
		CString cstrParentDir = m_cstrFileName.Left( nIndex );
		m_fnCreateDirectory( cstrParentDir );
		m_strDirPath = cstrParentDir;
		m_strFileName = m_cstrFileName.Mid(nIndex+1);
	}
}

/*===================================================================

===================================================================*/
BOOL CLogger::m_fnCreateDirectory( LPCSTR pszDirectory )
{
	CFileStatus fs;
	if ( CFile::GetStatus( pszDirectory, fs ) )
	{
		if ( (fs.m_attribute & FILE_ATTRIBUTE_DIRECTORY) != 0 )
		{
			return TRUE;
		}
	}

	CHAR szTemp[MAX_PATH];
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

/*===================================================================

===================================================================*/
void CLogger::SetLogString( LPCTSTR szLog, BOOL bWriteNow /*= TRUE*/ )
{
	if(m_pSync) ::EnterCriticalSection(m_pSync);

	SYSTEMTIME time;
	::GetLocalTime( &time );

	char szBuff[1024]={0};	// By mjna[140717]: Debug with BEOS, 배열크기 줄임.
	int len = sprintf_s( szBuff, sizeof(szBuff), "%02d/%02d %02d:%02d:%02d.%03d %.256s\n",
								time.wMonth,
								time.wDay,
								time.wHour,
								time.wMinute,
								time.wSecond,
								time.wMilliseconds,
								szLog );

	m_listLog.AddTail( szBuff );
	if( m_listLog.GetCount() > MAX_LOG )
	{
		bWriteNow = TRUE;
	}

	if( bWriteNow )		WriteLogBuff();

	if(m_pSync) ::LeaveCriticalSection(m_pSync);
}

/*===================================================================

===================================================================*/
void CLogger::LOG( const char* fmt, ... )
{
	char szBuff[MAX_BUFF]={0};
	va_list va;
	va_start(va, fmt);
	vsprintf_s(szBuff, sizeof(szBuff), fmt, va);
	va_end(va);
	SetLogString(szBuff);
}

/*===================================================================

===================================================================*/
void CLogger::WriteLog( void )
{
	if(m_pSync) ::EnterCriticalSection(m_pSync);

	WriteLogBuff();

	if(m_pSync) ::LeaveCriticalSection(m_pSync);
}


/*===================================================================

===================================================================*/
void CLogger::ClearLog( void )
{
	m_listLog.RemoveAll();
}


/*===================================================================

===================================================================*/
BOOL CLogger::WriteLogBuff( void )
{
	SYSTEMTIME time;
	::GetLocalTime( &time );

 	if( m_listLog.IsEmpty() )	return TRUE;

	CStdioFile file;
	CString strLogFile;
	strLogFile.Format(_T("%s\\%04d%02d%02d"), m_strDirPath, time.wYear, time.wMonth, time.wDay);
	m_fnCreateDirectory(strLogFile);

	strLogFile += "\\";
	strLogFile += m_strFileName;
	if( !file.Open( strLogFile, CFile::modeCreate | CFile::modeNoTruncate |
									CFile::modeWrite | CFile::typeText ) )
	{
		return FALSE;
	}
	file.SeekToEnd();

	CString cstrLog;
	POSITION pos;
	for( pos = m_listLog.GetHeadPosition(); pos != NULL; )
	{
		cstrLog = m_listLog.GetNext( pos );
		file.WriteString( cstrLog );
	}

	m_listLog.RemoveAll();

	file.Close();

	m_cstrFileName = _T("");
	m_cstrFileName = strLogFile;

	CFileStatus rStatus;
	CFile::GetStatus( m_cstrFileName, rStatus );
	if( rStatus.m_size > MAX_FILE_SIZE )
	{
		BackupFile();
		DeleteOldestFile();
	}

	return TRUE;
}


/*===================================================================

===================================================================*/
void CLogger::BackupFile( void )
{
	CString cstrPre, cstrExt;
	CTime time = CTime::GetCurrentTime();
	int nIndex = m_cstrFileName.ReverseFind( '.' );
	if( nIndex != -1 )
	{
		cstrPre = m_cstrFileName.Left( nIndex );
		cstrExt = m_cstrFileName.Mid( nIndex );
	}
	else
	{
		cstrPre = m_cstrFileName;
		cstrExt = "";
	}
	CString cstrOldFileName;
	cstrOldFileName.Format( _T("%s%02d%02d%02d%02d%02d%s"),
					cstrPre,
					time.GetMonth(),
					time.GetDay(),
					time.GetHour(),
					time.GetMinute(),
					time.GetSecond(),
					cstrExt );

	::DeleteFile( cstrOldFileName );
	::MoveFile( m_cstrFileName, cstrOldFileName );
}




/*===================================================================

===================================================================*/
void CLogger::DeleteOldestFile( void )
{
	CTime timeLimit = CTime::GetCurrentTime();
	timeLimit -= CTimeSpan( LOG_SAVE_DAYS, 0, 0, 0 );

	CString cstrFindFile;
	cstrFindFile = m_strDirPath + "\\*.*";

	CFileFind Finder;
	BOOL bExist = Finder.FindFile( cstrFindFile );
	while( bExist )
	{
		bExist = Finder.FindNextFile();	
		if( Finder.IsDots()) continue;
		else if( Finder.IsDirectory() )
		{
			CString cstrFileName = Finder.GetFilePath();
			CFileStatus status;
			CFile::GetStatus( cstrFileName, status );

			if( status.m_mtime < timeLimit )
			{
				cstrFindFile = cstrFileName;
				cstrFindFile += "\\*.*";

				CFileFind subFind;
				BOOL bSubExist = subFind.FindFile(cstrFindFile);
				while(bSubExist)
				{
					bSubExist = subFind.FindNextFile();
					if(subFind.IsDots()) continue;
					if(subFind.IsDirectory()) continue;
					CString strSubFileName = subFind.GetFilePath();
					::DeleteFile(strSubFileName);
				}
				subFind.Close();

				int nRtn = ::RemoveDirectory( cstrFileName );
				nRtn = ::GetLastError();
			}
		}
	}

	Finder.Close();
}
