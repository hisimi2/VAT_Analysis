#if !defined(AFX_MMWINFTP_H__BEC3078E_CA85_4314_B6D0_50D4CCE812D8__INCLUDED_)
#define AFX_MMWINFTP_H__BEC3078E_CA85_4314_B6D0_50D4CCE812D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Logger.h"
#include <afxinet.h>
#include <direct.h>

#define	FTP_RET_OK			0
#define	FTP_RET_NG			-1
#define FTP_CONECT_ERROR	(FTP_RET_NG-1)
#define FTP_TRANSFER_ERROR	(FTP_RET_NG-2)
#define FTP_CLOSE_ERROR		(FTP_RET_NG-3)

#define	FTP_INIFILE			"sys_data\\Ftp_SysParam.dat"
#define	FTP_IPADDR			"IpAddress"
#define	FTP_LOGINNAME		"LoginName"
#define	FTP_PASSWORD		"Password"
#define	FTP_PORTNO			"PortNo"
#define	FTP_PTN_SERVER		"FTP Server"
#define FTP_TRANSFER_TYPE	"TransferType"
#define FTP_DEF_TRANS_TYPE	"DefectFileType"
#define FTP_IMG_TRANS_TYPE	"ImageFileType"
#define SEC_LOG				"Log"
#define	KEY_LOG_HISTORY		"Log History"
#define FTP_REMOTE_DIR		"Remote Dir"
#define	FTP_LOG_FILE_NAME	"d:\\techwing\\Ftp\\FtpLog.txt"



class CmmwinFtp  
{
public:
	CmmwinFtp(CWnd* pParent = NULL);
	virtual ~CmmwinFtp();

	int FtpConnectGet( LPCTSTR pstrServerName, LPCTSTR pstrRemoteFile, LPCTSTR pstrLocalFile );
	int FtpConnectPut( LPCTSTR pstrServerName, LPCTSTR pstrLocalFile, LPCTSTR pstrRemoteFile );
	int FtpConnectPutDir( LPCTSTR pstrServerName, LPCTSTR pstrLocalDir, LPCTSTR pstrRemoteDir );

	int FtpSetCurDir( LPCTSTR pstrServerName, LPCTSTR pstrRemoteDir);

	int FtpConnect( LPCTSTR  pstrServerName );
	int FtpGet( LPCTSTR  pstrServerName, LPCTSTR pstrRemoteFile, LPCTSTR pstrLocalFile );
	int FtpPut( LPCTSTR  pstrServerName, LPCTSTR pstrLocalFile, LPCTSTR pstrRemoteFile );
	int FtpMput( LPCTSTR  pstrServerName, LPCTSTR pstrLocalDir, LPCTSTR pstrRemoteDir );
	int FtpMGet( LPCTSTR pstrServerName, LPCTSTR pstrLocalDir, LPCTSTR pstrRemoteDir );
	int FtpCopyDir( LPCTSTR strSrc, LPCTSTR strDst, CFtpConnection *pFtp );
	int FtpCopyDirFromServer(LPCTSTR strSrc, LPCTSTR strDst, CFtpConnection *pFtp );
	int CreateDirectory( LPCSTR pszDirectory, CFtpConnection *pFtp, char *szFileName );
	void FtpClose( LPCTSTR  pstrServerName );
	BOOL Ping( char* szIp );
	void GetIpAddr(char* szIpAddr);
	void GetRemoteDir(char* szRemoteDir);

protected:
	CWnd* m_pParent;

private:
	CLogger m_FtpLogger;
};

#endif // !defined(AFX_MMWINFTP_H__BEC3078E_CA85_4314_B6D0_50D4CCE812D8__INCLUDED_)
