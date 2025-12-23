#include "stdafx.h"
#include "Logger.h"

#define	MAX_LOG				(100)
#define	MAX_FILE_SIZE		(3000000)



/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CLogger::CLogger()
{
	int nLogSaveDays=0;
	char szTemp[512]={0};
	// LOG를 하나의 폴더에 몰아 넣기 때문에 삭제 기간을 통일 시킨다.
/*
	GetPrivateProfileString("LOG CONFIG", "Log Save Days", "0", szTemp, sizeof(szTemp)-1, "SYS_DATA\\SystemOption.ini");
	if( atoi(szTemp) <= 0 || atoi(szTemp) > 100){
		nLogSaveDays = LOG_SAVE_DAYS;
		sprintf_s(szTemp, sizeof(szTemp), "%d", nLogSaveDays);
		WritePrivateProfileString("LOG CONFIG", "Log Save Days", szTemp, "SYS_DATA\\SystemOption.ini");
	}
*/

	nLogSaveDays = LOG_SAVE_DAYS;
	m_fnSetLogSaveDays(nLogSaveDays);

	m_cstrFileName = "log.txt";

	m_listLog.RemoveAll();

	m_pSync = NULL;

	m_pSync = new CRITICAL_SECTION;
	m_nSaveDays = 0;
	try
	{
		::InitializeCriticalSection(m_pSync);
	}
	catch(...)
	{
		AfxMessageBox("CLogger Initialize Error !!!");
	}
	
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
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



/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
CString CLogger::GetFileName( void )
{
	return m_cstrFileName;
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
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


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
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


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CLogger::SetLogString( LPCTSTR szLog, BOOL bWriteNow /*= TRUE*/ )
{
	if(m_pSync) ::EnterCriticalSection(m_pSync);

	SYSTEMTIME time;
	::GetLocalTime( &time );

	char szBuff[MAX_BUFF]={0};
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


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CLogger::LOG( const char* fmt, ... )
{
	char szBuff[MAX_BUFF]={0};
	va_list va;
	va_start(va, fmt);
	vsprintf(szBuff, fmt, va);
	va_end(va);
	SetLogString(szBuff);
}


/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CLogger::WriteLog( void )
{
	if(m_pSync) ::EnterCriticalSection(m_pSync);

	WriteLogBuff();

	if(m_pSync) ::LeaveCriticalSection(m_pSync);
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
void CLogger::ClearLog( void )
{
	m_listLog.RemoveAll();
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
BOOL CLogger::WriteLogBuff( void )
{
	SYSTEMTIME time;
	::GetLocalTime( &time );

 	if( m_listLog.IsEmpty() )	return TRUE;
	if( m_strDirPath.IsEmpty() ) return FALSE;

	CStdioFile file;
	CString strLogFile;
	strLogFile.Format(_T("%s\\%04d%02d%02d"), m_strDirPath, time.wYear, time.wMonth, time.wDay);
	m_fnCreateDirectory(strLogFile);

	strLogFile += "\\";
	strLogFile += m_strFileName;
	if( !file.Open( strLogFile, CFile::modeCreate | CFile::modeNoTruncate |
									CFile::modeWrite | CFile::typeText ) )
	{
		m_listLog.RemoveAll();
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
	if(rStatus.m_size > MAX_FILE_SIZE )
	{
		BackupFile();
		DeleteOldestFile();
	}

	return TRUE;
}



/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
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

/*=============================================================================
  AUTHOR   : 나명진  [2015-07-08]
  NAME     : 
  PARAMS   : 
  RETURN   : 
  COMMENTS : 
==============================================================================*/
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
			int Y=0,M=0,D=0, h=0,m=0,s=0;
			int nBaseIndex = cstrFileName.ReverseFind('\\') + 1;
			char szTemp[128]={0};
			char szYear[32]={0};
			char szMonth[32]={0};
			char szDay[32]={0};
			sprintf_s(szYear, sizeof(szYear), "%s", cstrFileName.Mid(nBaseIndex,4).GetString() );
			sprintf_s(szMonth, sizeof(szMonth), "%s", cstrFileName.Mid(nBaseIndex+4,2).GetString() );
			sprintf_s(szDay,  sizeof(szDay), "%s", cstrFileName.Mid(nBaseIndex+6,2).GetString() );
			Y = atoi(szYear);
			M = atoi(szMonth);
			D = atoi(szDay);
			CTime DirTime(Y,M,D,0,0,0);

			if( DirTime < timeLimit )
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
