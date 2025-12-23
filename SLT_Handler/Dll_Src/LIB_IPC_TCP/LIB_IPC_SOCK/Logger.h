#ifndef	_CLOGGER_H_
#define	_CLOGGER_H_

#include <afxmt.h>

#define MAX_BUFF			(4096)
//----------------------------------------------------------------------------
// CLogger
//----------------------------------------------------------------------------
class /*AFX_EXT_CLASS*/ CLogger
{
public:
	CLogger();
	~CLogger();


public:
	CString	GetFileName( void );
	void	SetFileName( LPCTSTR szFileName );
	void	SetLogString( LPCTSTR szLog, BOOL bWriteNow = TRUE );
	void	LOG( const char* fmt, ... );
	void	WriteLog( void );
	void	ClearLog( void );
	void	m_fnSetLogSaveDays( int nDay ) { m_nSaveDays = nDay; };

private:
	BOOL	WriteLogBuff( void );
	void	BackupFile( void );
	void	DeleteOldestFile( void );
	BOOL	m_fnCreateDirectory( LPCSTR pszDirectory );

private:
	CCriticalSection	m_CsLogSet;
	CStringList		    m_listLog;
	CString			    m_cstrFileName;
	int				    m_nSaveDays;
	CString			    m_strDirPath;
	CString			    m_strFileName;
	CRITICAL_SECTION*   m_pSync;
};

#endif	// _CLOGGER_H_
