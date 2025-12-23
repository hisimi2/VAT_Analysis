#include "stdafx.h"
#include "mmwinFtp.h"

#include "Ping.h"


CInternetSession *g_pFtpSession = NULL;
CFtpConnection *g_pFtpConnection = NULL;

int g_nDFTransType = 0;

CmmwinFtp::CmmwinFtp(CWnd* pParent)
{
	m_pParent = pParent;

	// Initialize
	g_pFtpSession	= NULL;
	g_pFtpConnection	= NULL;

	WORD wVersionRequested; 
	WSADATA wsaData; 
	wVersionRequested = MAKEWORD( 2, 0 );
	WSAStartup( wVersionRequested , &wsaData );

// 	m_FtpLogger = NULL;
// 	m_FtpLogger = new CLogger;
	m_FtpLogger.SetFileName( FTP_LOG_FILE_NAME );

	char szBuff[256];
	char szIniFilePass[256];
	char szCurDir[1024] ={0};

	GetCurrentDirectory(256, szCurDir);
	sprintf_s( szIniFilePass, "%s\\%s", szCurDir, FTP_INIFILE );
	memset( szBuff, 0x00, sizeof(szBuff) );
	GetPrivateProfileString( SEC_LOG, KEY_LOG_HISTORY, _T("21"),
								szBuff, sizeof(szBuff), szIniFilePass );
	int nLogHistory = atoi(szBuff);
	sprintf_s( szBuff, "%d", nLogHistory );
	WritePrivateProfileString( SEC_LOG, KEY_LOG_HISTORY, szBuff, szIniFilePass );
	m_FtpLogger.m_fnSetLogSaveDays( nLogHistory );

	m_FtpLogger.SetLogString( "Ftp Log Start", TRUE );
}

CmmwinFtp::~CmmwinFtp()
{
	// FTP Close
	FtpClose( FTP_PTN_SERVER );

	WSACleanup();

	m_FtpLogger.SetLogString( "Ftp Log End", TRUE );
// 	if( m_FtpLogger )
// 	{
// 		delete m_FtpLogger;
// 		m_FtpLogger = NULL;
// 	}
}

int CmmwinFtp::FtpConnectGet( LPCTSTR  pstrServerName, LPCTSTR pstrRemoteFile, LPCTSTR pstrLocalFile )
{
	CInternetSession IFtp( NULL, 1, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0 );
	CString cstrRemoteFile;

	cstrRemoteFile = pstrRemoteFile;
	cstrRemoteFile.Replace( '\\', '/' );

	return FTP_RET_OK;
}

int CmmwinFtp::FtpConnectPut( LPCTSTR  pstrServerName, LPCTSTR pstrLocalFile, LPCTSTR pstrRemoteFile )
{
 	CString cstrRemoteFile;
	int nRet;

	CWaitCursor wait;
	cstrRemoteFile = pstrRemoteFile;
	cstrRemoteFile.Replace( '\\', '/' );

	nRet = FtpConnect( pstrServerName );
	if( nRet != FTP_RET_OK ){
		FtpClose( pstrServerName );
		return FTP_CONECT_ERROR;
	}

	// Put
	nRet = FtpPut( pstrServerName, pstrLocalFile, cstrRemoteFile );
	if(nRet != FTP_RET_OK){
		FtpClose( pstrServerName );
		return FTP_TRANSFER_ERROR;
	}

	FtpClose( pstrServerName );

	return FTP_RET_OK;
}

int CmmwinFtp::FtpConnectPutDir( LPCTSTR  pstrServerName, LPCTSTR pstrLocalDir, LPCTSTR pstrRemoteDir )
{
	CInternetSession IFtp( NULL, 1, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0 );
	CString cstrRemoteDir;

	cstrRemoteDir = pstrRemoteDir;
	cstrRemoteDir.Replace( '\\', '/' );

	return FTP_RET_OK;
}

int CmmwinFtp::FtpConnect( LPCTSTR  pstrServerName )
{
	CInternetSession	*pSession = NULL;
	CFtpConnection		*pFtpCon = NULL;
	char szIpAddr[256];
	char szLoginName[256];
	char szPassword[256];
	int  nPortNo;
	char szTransferType[256];
	char szIniFilePass[256];

	char szLog[512];
	sprintf_s( szLog, "Ftp Connect Start, %s", pstrServerName );
	m_FtpLogger.SetLogString( szLog, TRUE );

	CWaitCursor wait;

	if( !strcmp(pstrServerName, FTP_PTN_SERVER) )
	{
		if( g_pFtpSession != NULL ){
			g_pFtpSession->Close();
			delete g_pFtpSession;
		}
		g_pFtpSession = new CInternetSession( NULL, 1, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0 );
		pSession = g_pFtpSession;
		pFtpCon  = g_pFtpConnection;
	}
	else
	{
		return FTP_RET_NG;
	}
	if( pSession == NULL ){
		m_FtpLogger.SetLogString( "InternetSession Pointer Get Failed", TRUE );
		return FTP_RET_NG;
	}

	memset(szIpAddr,		0x00, sizeof(szIpAddr));
	memset(szLoginName,		0x00, sizeof(szLoginName));
	memset(szPassword,		0x00, sizeof(szPassword));
	memset(szTransferType,  0x00, sizeof(szTransferType));

	char szCurDir[1024] = {0};
	GetCurrentDirectory(256, szCurDir);
	sprintf_s( szIniFilePass, "%s\\%s", szCurDir, FTP_INIFILE );

	GetPrivateProfileString(pstrServerName,
							FTP_IPADDR,
							NULL,
							szIpAddr,
							sizeof(szIpAddr),
							szIniFilePass);
	GetPrivateProfileString(pstrServerName,
							FTP_LOGINNAME,
							NULL,
							szLoginName,
							sizeof(szLoginName),
							szIniFilePass);
	GetPrivateProfileString(pstrServerName,
							FTP_PASSWORD,
							NULL,
							szPassword,
							sizeof(szPassword),
							szIniFilePass);
	nPortNo = GetPrivateProfileInt(pstrServerName,
								   FTP_PORTNO,
								   INTERNET_INVALID_PORT_NUMBER,
								   szIniFilePass);
	GetPrivateProfileString(FTP_TRANSFER_TYPE,
							FTP_DEF_TRANS_TYPE,
							NULL,
							szTransferType,
							sizeof(szTransferType),
							szIniFilePass);
	if( !strcmp( szTransferType, "binary" ) ){
		g_nDFTransType = FTP_TRANSFER_TYPE_BINARY;
	}	
	else{
		g_nDFTransType = FTP_TRANSFER_TYPE_ASCII;
	}

	BOOL bRet = Ping( szIpAddr );
	if( bRet != TRUE ){
		return FTP_RET_NG;
	}

	TRY
	{
		pFtpCon = pSession->GetFtpConnection( szIpAddr, szLoginName, szPassword, (unsigned short)nPortNo );
	}
	CATCH_ALL(e)
	{
		m_FtpLogger.SetLogString( "InternetSession Pointer Get Failed", TRUE );
		return FTP_RET_NG;
	}
	END_CATCH_ALL

	if( !strcmp(pstrServerName, FTP_PTN_SERVER) )
	{
		g_pFtpConnection = pFtpCon;
	}

	m_FtpLogger.SetLogString( "Ftp Connect End", TRUE );

	return FTP_RET_OK;
}

int CmmwinFtp::FtpSetCurDir( LPCTSTR pstrServerName, LPCTSTR pstrRemoteDir)
{
	
	if( !strcmp(pstrServerName, FTP_PTN_SERVER) )
	{
		g_pFtpConnection->SetCurrentDirectory(pstrRemoteDir);
	}
	return 0;
}

int CmmwinFtp::FtpGet( LPCTSTR  pstrServerName, LPCTSTR pstrRemoteFile, LPCTSTR pstrLocalFile )
{
	BOOL bOK;
	CString cstrRemoteFile;
	int nTransferType;
	CFtpConnection *pFtp = NULL;
	char szLog[512];


	CWaitCursor wait;

	cstrRemoteFile = pstrRemoteFile;
	cstrRemoteFile.Replace( '\\', '/' );

	if( !strcmp(pstrServerName, FTP_PTN_SERVER) )
	{
		if( g_pFtpConnection == NULL ){
			return FTP_RET_NG;
		}
		pFtp  = g_pFtpConnection;
	}
	else
	{
		return FTP_RET_NG;
	}

	if( cstrRemoteFile.Right(4) == ".JPG" ){
		nTransferType = FTP_TRANSFER_TYPE_BINARY;
	}
	else{
		nTransferType = g_nDFTransType;
	}

	bOK = pFtp->GetFile( cstrRemoteFile, pstrLocalFile, TRUE, FILE_ATTRIBUTE_NORMAL, nTransferType );
	if ( !bOK ){
		sprintf_s( szLog, "GetFile NG, Local:%s, Remote:%s", cstrRemoteFile.GetString(), pstrLocalFile );
		m_FtpLogger.SetLogString( szLog, TRUE );
		return FTP_RET_NG;
	}
	sprintf_s( szLog, "GetFile OK, Local:%s, Remote:%s", cstrRemoteFile.GetString(), pstrLocalFile );
	m_FtpLogger.SetLogString( szLog, TRUE );
	return FTP_RET_OK;
}

int CmmwinFtp::FtpPut( LPCTSTR  pstrServerName, LPCTSTR pstrLocalFile, LPCTSTR pstrRemoteFile )
{
	BOOL bOK=FALSE;
	char szFileName[256]={0};
	CString cstrLocalFile="";
	CString cstrRemoteFile="";
	int nTransferType=0;
	CFtpConnection *pFtp = NULL;
	char szLog[512]={0};

	CWaitCursor wait;

	cstrRemoteFile = pstrRemoteFile;
	cstrRemoteFile.Replace( '\\', '/' );

	if( !strcmp(pstrServerName, FTP_PTN_SERVER) )
	{
		if( g_pFtpConnection == NULL ){
			return FTP_RET_NG;
		}
		pFtp  = g_pFtpConnection;
	}
	else
	{
		return FTP_RET_NG;
	}

	int nRtn = CreateDirectory( cstrRemoteFile, pFtp, szFileName );
	if ( nRtn != FTP_RET_OK ){
		return FTP_RET_NG;
	}

	cstrLocalFile = pstrLocalFile;
	cstrLocalFile.MakeUpper();
	if( cstrLocalFile.Right(4) == ".JPG" ){
		nTransferType = FTP_TRANSFER_TYPE_BINARY;
	}
	else{
		nTransferType = g_nDFTransType;
	}

	bOK = pFtp->PutFile( pstrLocalFile, szFileName, nTransferType );
	if ( !bOK ){
		DWORD dwError = GetLastError();
		sprintf_s( szLog, "PutFile Failed, Local:%s, Remote:%s, ErrorCode:%lu", pstrLocalFile, szFileName, dwError );
		m_FtpLogger.SetLogString( szLog, TRUE );
		return FTP_RET_NG;
	}
	sprintf_s( szLog, "PutFile OK, Local:%s, Remote:%s", pstrLocalFile, szFileName );
	m_FtpLogger.SetLogString( szLog, TRUE );
	return FTP_RET_OK;
}

int CmmwinFtp::FtpMput( LPCTSTR  pstrServerName, LPCTSTR pstrLocalDir, LPCTSTR pstrRemoteDir )
{
	BOOL bOK;
	CString cstrRemoteDir;
	CFtpConnection *pFtp = NULL;

	CWaitCursor wait;

	cstrRemoteDir = pstrRemoteDir;
	cstrRemoteDir.Replace( '\\', '/' );

	if( !strcmp(pstrServerName, FTP_PTN_SERVER) )
	{
		if( g_pFtpConnection == NULL ){
			return FTP_RET_NG;
		}
		pFtp  = g_pFtpConnection;
	}
	else
	{
		return FTP_RET_NG;
	}

	bOK = FtpCopyDir( pstrLocalDir, cstrRemoteDir, pFtp );
	if( bOK != TRUE ){
		return FTP_RET_NG;
	}

	return FTP_RET_OK;
}

BOOL CmmwinFtp::FtpCopyDir( LPCTSTR strSrc, LPCTSTR strDst, CFtpConnection *pFtp )
{
	HANDLE hFile;
	WIN32_FIND_DATA stFind;
	char szFile[256];
	CString cstrLocalFile;
	int nTransferType;
	char szLog[512];

	if ( CreateDirectory( strDst,pFtp,szFile ) ){
		return FALSE;
	}

	CString strSearch = strSrc;
	strSearch += "*.*";

	hFile = ::FindFirstFile( strSearch, &stFind );

	if ( hFile == INVALID_HANDLE_VALUE ){
		DWORD	dwErr = ::GetLastError();
		sprintf_s( szLog, "FindFirstFile Failed, Search:%s, ErrorCode:%lu", strSearch.GetString(), dwErr );
		m_FtpLogger.SetLogString( szLog, TRUE );
		return	FALSE;
	}

	while( ::FindNextFile( hFile, &stFind))
	{
		Sleep(1);
		if( stFind.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY ){
			if( !strcmp( stFind.cFileName, ".." ))	continue;

			CString strNextSrc = strSrc;
			strNextSrc += stFind.cFileName ;
			strNextSrc += "\\";
			CString strNextDst = strDst;
			strNextDst += stFind.cFileName;
			strNextDst += "/";

			BOOL bRtn = FtpCopyDir( strNextSrc, strNextDst ,pFtp );
			if ( bRtn != TRUE ){
				::FindClose(hFile);
				sprintf_s( szLog, "CopyDir Failed, Src:%s, Dst:%s", strSrc, strDst );
				m_FtpLogger.SetLogString( szLog, TRUE );
				return	FALSE;
			}
		} 
		else
		{
			CString strNextSrc = strSrc;
			strNextSrc += stFind.cFileName ;
			CString strNextDst = strDst;
			strNextDst += stFind.cFileName;

			cstrLocalFile = strNextSrc;
			if( cstrLocalFile.Right(4) == ".JPG" ){
				nTransferType = FTP_TRANSFER_TYPE_BINARY;
			}
			else{
				nTransferType = g_nDFTransType;
			}
			BOOL bOK = pFtp->PutFile( strNextSrc,stFind.cFileName, nTransferType );
			if ( !bOK ){
				::FindClose(hFile);
				sprintf_s( szLog, "PutFile Failed, Local:%s, Remote:%s", strNextSrc.GetString(), stFind.cFileName );
				m_FtpLogger.SetLogString( szLog, TRUE );
				return TRUE;
			}
			sprintf_s( szLog, "PutFile OK, Local:%s, Remote:%s", strNextSrc.GetString(), stFind.cFileName );
			m_FtpLogger.SetLogString( szLog, TRUE );
		}		
	}
	::FindClose(hFile);

	return TRUE;
}

int CmmwinFtp::CreateDirectory( LPCSTR pszDirectory, CFtpConnection *pFtp, char *szFileName )
{
	CFileStatus	clStatus;
	CString		cstrDir;
	int	nIndex;
	CString cstrFileName = pszDirectory;

	char szLog[512];
	sprintf_s( szLog, "CreateDirectory Start, DirectoryName:%s, FileName:%s", pszDirectory, szFileName );
	m_FtpLogger.SetLogString( szLog, TRUE );

	BOOL bOK;

	if ( cstrFileName.Find( '/' ) == 0 )
	{
		cstrDir = "/";
		bOK = pFtp->SetCurrentDirectory( cstrDir );
		if ( !bOK ){
			sprintf_s( szLog, "SetCurrentDirectory Failed, DirectoryName:%s", cstrDir.GetString() );
			m_FtpLogger.SetLogString( szLog, TRUE );
			return FTP_RET_NG;
		}
		cstrFileName = cstrFileName.Right( cstrFileName.GetLength() - 1 );
	}

	while ( ( nIndex = cstrFileName.Find( '/' ) ) >= 0 )
	{
		cstrDir = cstrFileName.Left( nIndex );
		cstrFileName = cstrFileName.Right(
								cstrFileName.GetLength() - nIndex - 1 );
		bOK = pFtp->SetCurrentDirectory( cstrDir );
		if ( !bOK ){
			bOK = pFtp->CreateDirectory( cstrDir );
			if ( !bOK ){
				DWORD nErr = GetLastError();
				sprintf_s( szLog, "CreateDirectory Failed, DirectoryName:%s, ErrorCode=%lu", cstrDir.GetString(), nErr );
				m_FtpLogger.SetLogString( szLog, TRUE );
			}
			bOK = pFtp->SetCurrentDirectory( cstrDir );
			if ( !bOK ){
				sprintf_s( szLog, "SetCurrentDirectory Failed, DirectoryName:%s", cstrDir.GetString() );
				m_FtpLogger.SetLogString( szLog, TRUE );
				return FTP_RET_NG;
			}
		}
		Sleep(1);
	}
	strcpy_s( szFileName, strlen( szFileName ), cstrFileName );

	sprintf_s( szLog, "CreateDirectory End, DirectoryName:%s, FileName:%s", pszDirectory, szFileName );
	m_FtpLogger.SetLogString( szLog, TRUE );

	return FTP_RET_OK;
}

void CmmwinFtp::FtpClose( LPCTSTR  pstrServerName )
{
	char szLog[512];
//	if(m_FtpLogger) {
		sprintf_s( szLog, "Ftp Connect Close Start, %s", pstrServerName );
		m_FtpLogger.SetLogString( szLog, TRUE );
//	}

	if( !strcmp(pstrServerName, FTP_PTN_SERVER) )
	{
		if ( g_pFtpConnection != NULL ){
			g_pFtpConnection->Close();
			delete g_pFtpConnection;
			g_pFtpConnection = NULL;
		}
		if ( g_pFtpSession != NULL ){
			g_pFtpSession->Close();
			delete g_pFtpSession;
			g_pFtpSession = NULL;
		}
	}
	else
	{
		return;
	}

//	if(m_FtpLogger) {
		sprintf_s( szLog, "Ftp Connect Close End, %s", pstrServerName );
		m_FtpLogger.SetLogString( szLog, TRUE );
//	}
}

BOOL CmmwinFtp::Ping( char* szIp )
{
	char szLog[512];
	sprintf_s( szLog, "Ping Start, IP:%s", szIp );
	m_FtpLogger.SetLogString( szLog, TRUE );

	CPing *p = new CPing;
	int nCount = 0;
	if( p->m_wStatus == PING_READY && p->DoPing(szIp))
	{
		while( nCount < 6 )
		{
			Sleep(250);
			if( p->m_wStatus == PING_DONE)
			{
				if( p->m_wRetCode == PING_NOERROR )
				{
					sprintf_s( szLog, "Ping OK, IP:%s", szIp );
					m_FtpLogger.SetLogString( szLog, TRUE );
					// Ping Ok
					p->ClearPing();
					delete p;
					p = NULL;
					return TRUE;
				}
			}
			nCount++;
		}
		if( p != NULL )
		{
			p->ClearPing();
			delete p;
			p = NULL;
		}
	}
	if( p != NULL )
	{
		p->ClearPing();
		delete p;
		p = NULL;
	}

	sprintf_s( szLog, "Ping NG, IP:%s", szIp );
	m_FtpLogger.SetLogString( szLog, TRUE );

	// Ping Failed
	return FALSE;
}

void CmmwinFtp::GetIpAddr(char* szIpAddr)
{
	char szCurDir[1024] = {0};
	char szIniFilePass[1024] = {0};
	GetCurrentDirectory(256, szCurDir);
	sprintf_s( szIniFilePass, "%s\\%s", szCurDir, FTP_INIFILE );
	
	char szTemp[100] = {0};
	GetPrivateProfileString(FTP_PTN_SERVER,
		FTP_IPADDR,
		NULL,
		szTemp,
		sizeof(szTemp),
		szIniFilePass);

	sprintf_s(szIpAddr, strlen( szIpAddr ), szTemp);
}

int CmmwinFtp::FtpMGet( LPCTSTR pstrServerName, LPCTSTR pstrLocalDir, LPCTSTR pstrRemoteDir )
{
	BOOL bOK;
	CString cstrRemoteDir;
	CFtpConnection *pFtp = NULL;

	char szLog[100] = {0};
	
	/*********************************************************************/
	memset(szLog, 0x00, sizeof(szLog));
	sprintf_s(szLog, "[%-8.8s][%-8.8s] %s\r\n", "MmwinFtp", "Mget", pstrServerName );
	m_FtpLogger.SetLogString( szLog, TRUE );
	memset(szLog, 0x00, sizeof(szLog));
	sprintf_s(szLog, "   %s\r\n", pstrLocalDir );
	m_FtpLogger.SetLogString( szLog, TRUE );
	memset(szLog, 0x00, sizeof(szLog));
	sprintf_s(szLog, "   %s\r\n", pstrRemoteDir );
	m_FtpLogger.SetLogString( szLog, TRUE );
	/*********************************************************************/
	
	CWaitCursor wait;
	
	cstrRemoteDir = pstrRemoteDir;
	cstrRemoteDir.Replace( '\\', '/' );
	
	if( !strcmp(pstrServerName, FTP_PTN_SERVER) )
	{
		if( g_pFtpConnection == NULL ){
			return FTP_RET_NG;
		}
		pFtp  = g_pFtpConnection;
	}
	else
	{
		return FTP_RET_NG;
	}

	bOK = FtpCopyDirFromServer( pstrLocalDir, cstrRemoteDir, pFtp );
	if( bOK != TRUE ){
		return FTP_RET_NG;
	}
	
	return FTP_RET_OK;
}

BOOL CmmwinFtp::FtpCopyDirFromServer( LPCTSTR strLocal, LPCTSTR strRemote, CFtpConnection *pFtp )
{
	CString cstrLocalFile;
	CString cstrRemoteFile;
	CString cstrFileName;
	char szLog[512];
	DWORD	dwErr;

	CFtpFileFind cFtpfind(pFtp);
	CFileFind cFileFind;

	BOOL bFind = cFtpfind.FindFile( strRemote );
	
	if ( bFind == FALSE ){
		dwErr = ::GetLastError();
		sprintf_s( szLog, "FindFirstFile Failed, ErrorCode:%lu", dwErr );
		m_FtpLogger.SetLogString( szLog, TRUE );

		cFileFind.Close();
		cFtpfind.Close();
		return	FALSE;
	}

	FtpSetCurDir(FTP_PTN_SERVER, strRemote);

	bFind = cFtpfind.FindFile("");

	int nIgnore = 0;
		
	while(bFind)
	{
		Sleep(1);

		bFind = cFtpfind.FindNextFile();
		if (cFtpfind.IsDots())
			continue;
		else if(cFtpfind.IsDirectory())
			continue;
		else{
			cstrFileName = cFtpfind.GetFileName();
			cstrRemoteFile.Format(_T("%s/%s"), (LPCTSTR)strRemote, (LPCTSTR)cstrFileName);
			cstrLocalFile.Format(_T("%s\\%s"), (LPCTSTR)strLocal, (LPCTSTR)cstrFileName);
			if(cFileFind.FindFile(cstrLocalFile)){
				char szMsg[100] = {0};
				sprintf_s(szMsg, "Local 디렉토리에 %s파일이 있습니다.\r\n신규 PTN 파일로 변경하시겠습니까?", cstrFileName.GetString() );
				int nID = IDNO;
				if(nIgnore == 0) {
					nID = AfxMessageBox(szMsg, MB_YESNO);
					if(nID == IDYES) {
						FtpGet(FTP_PTN_SERVER, cstrRemoteFile, cstrLocalFile);
						
						sprintf_s(szMsg, "모들 파일에 대해서 신규 PTN 파일로 변경하시겠습니까?");
						nID = AfxMessageBox(szMsg, MB_YESNO);
						if(nID == IDYES){
							nIgnore = 1;
						}
					}
				} else {
					FtpGet(FTP_PTN_SERVER, cstrRemoteFile, cstrLocalFile);
				}
			} else {
				FtpGet(FTP_PTN_SERVER, cstrRemoteFile, cstrLocalFile);
			}
		}
	}

	cFileFind.Close();
	cFtpfind.Close();

	return TRUE;
}

void CmmwinFtp::GetRemoteDir(char* szDir)
{
	char szCurDir[1024] = {0};
	char szIniFilePass[1024] = {0};
	GetCurrentDirectory(256, szCurDir);
	sprintf_s( szIniFilePass, "%s\\%s", szCurDir, FTP_INIFILE );
	
	char szTemp[100] = {0};
	GetPrivateProfileString(FTP_PTN_SERVER,
		FTP_REMOTE_DIR,
		NULL,
		szTemp,
		sizeof(szTemp),
		szIniFilePass);
	
	sprintf_s(szDir, strlen( szDir ), szTemp);
}