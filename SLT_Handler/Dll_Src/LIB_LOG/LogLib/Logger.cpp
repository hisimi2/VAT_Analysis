// Logger.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <share.h>
#include "Logger.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLogger

CLogger::CLogger():m_strName(_T("TWInfo"))
{
	SetDefaultOptions();
	InitializeCriticalSection(&m_csLog);
}

CLogger::CLogger(LPCTSTR name):m_strName(name)
{
	SetDefaultOptions();
	InitializeCriticalSection(&m_csLog);
}

CLogger::~CLogger()
{
	m_bEnabled = FALSE;
	m_bRun = FALSE;
	DWORD result = ::WaitForSingleObject(m_hThread, 1000);
	if (result == WAIT_TIMEOUT)
		::TerminateThread(m_hThread, 0);

	if (m_hObject != NULL)
	{
		::CloseHandle(m_hObject);
		m_hObject = NULL;
	}
	DeleteCriticalSection(&m_csLog);

	// clear queue, 이 코드 필요할까? ^^;
	while (!m_qMessage.empty())
	{
		m_qMessage.front().Empty();
		m_qMessage.pop();
	}

	m_strRootPath.Empty();
	m_strSubDir.Empty();
	m_strLogExt.Empty();
	m_strFileName.Empty();
	m_strName.Empty();
	m_strHeader.Empty();
}

void CLogger::SetDefaultOptions()
{
	m_hObject = NULL;
	m_hThread = NULL;
	m_strRootPath = _T("D:\\Log");
	m_strLogExt = _T("log");
	m_strSubDir = _T("");
	m_strHeader = _T("");
	m_nLogItem = LogItem::Time|LogItem::Filename|LogItem::LineNumber|LogItem::Level|LogItem::NewLine;
	m_eStoreType = LogFileType::eHour;
	m_ePathType = LogPathType::NAME;
	m_nCapacity = 5 * 1024 * 1024;	// default 5MB
	m_bEnabled = FALSE;
	m_bRun = FALSE;
}

// CLogger 멤버 함수
void CLogger::Enable(BOOL enable)
{
	m_bEnabled = enable;
	if (!enable)
	{
		m_bRun = FALSE;
		::SetEvent(m_hObject);
	}
}

BOOL CLogger::IsEnabled()
{
	return m_bEnabled;
}

void CLogger::SetExtension(LPCTSTR ext)
{
	m_strLogExt = ext;
}

void CLogger::SetFileName(LPCTSTR fname)
{
	m_strFileName = fname;
}

void CLogger::SetRootPath(LPCTSTR root)
{
	m_strRootPath = root;
}

void CLogger::SetSubDir(LPCTSTR sub)
{
	m_strSubDir = sub;
}

void CLogger::SetLoggerName(LPCTSTR name)
{
	m_strName = name;
}

void CLogger::SetHeader(LPCTSTR header)
{
	m_strHeader = header;
}

void CLogger::SetLogItem(int item)
{
	m_nLogItem = item;
}

int CLogger::GetLogItem()
{
	return m_nLogItem;
}

CString CLogger::GetExtension()
{
	return m_strLogExt;
}

CString CLogger::GetRootPath()
{
	return m_strRootPath;
}

CString CLogger::GetLoggerName()
{
	return m_strName;
}

LogFileType::eLogFileType CLogger::GetStoreType()
{
	return m_eStoreType;
}

LogPathType::eLogPathType CLogger::GetPathType()
{
	return m_ePathType;
}

void CLogger::SetStoreType(LogFileType::eLogFileType type)
{
	m_eStoreType = type;
}
void CLogger::SetPathType(LogPathType::eLogPathType type)
{
	m_ePathType = type;
}

BOOL CLogger::Run()
{
	if (!m_bEnabled)
		return FALSE;

	if (m_bRun)
		return FALSE;

	CString name = _T("EVT_LOG_") + m_strName;

	m_hObject = ::CreateEvent(NULL, FALSE, FALSE, name);

	if (m_hObject == NULL)
		return FALSE;

	m_bRun = TRUE;

	if (m_hThread == 0)
	{
		CWinThread *pThread = AfxBeginThread(ThreadProc, (LPVOID)this);

		if (pThread == NULL)
		{
			AfxMessageBox(_T("ThreadWriteLog creation failed"));
			m_bRun = FALSE;
			return FALSE;
		}
		m_hThread = pThread->m_hThread;

		return TRUE;
	}

	return FALSE;
}

void CLogger::PutLog(LPCTSTR msg)
{
	EnterCriticalSection(&m_csLog);
	m_qMessage.push(msg);
	::SetEvent(m_hObject);
	LeaveCriticalSection(&m_csLog);
}

UINT CLogger::WriteProcedure()
{
	CString msg;

	do 
	{
		WaitForSingleObject(m_hObject, INFINITE);

		EnterCriticalSection(&m_csLog);
		while (!m_qMessage.empty())
		{
			msg = m_qMessage.front();
			m_qMessage.front().Empty();
			m_qMessage.pop();
			WriteLog(msg);
		}
		LeaveCriticalSection(&m_csLog);
		msg.Empty();
	} while (m_bRun);

	return 0;
}

int CLogger::WriteLog(CString msg)
{
	int hFile;
	CString strFullName;

	strFullName = GetFullPath() + _T("\\") + GetFileName() + _T(".") + m_strLogExt;

	int oflag = 0;

	hFile = _tsopen((LPCTSTR)strFullName, O_CREAT | O_WRONLY | O_APPEND, _SH_DENYWR, _S_IREAD | _S_IWRITE );
	if( -1 == hFile){
		Sleep( 20 );
		hFile = _tsopen((LPCTSTR)strFullName, O_CREAT | O_WRONLY | O_APPEND, _SH_DENYWR, _S_IREAD | _S_IWRITE );
		if( -1 == hFile ) {
			return -1;
		}
	}

	if (_filelength(hFile) == 0 && m_nLogItem & static_cast<int>(LogItem::Header) && !m_strHeader.IsEmpty())
	{
#ifdef _UNICODE
		LPCSTR pszUTF8 = W2A((LPCTSTR)m_strHeader, CP_UTF8);
		_write(hFile, pszUTF8, strlen(pszUTF8));
#else
		_write(hFile, (LPCTSTR)m_strHeader, m_strHeader.GetLength()*sizeof(TCHAR));
#endif
	}
#ifdef _UNICODE
	LPCSTR pszUTF8 = W2A((LPCTSTR)msg, CP_UTF8);	// not tested, if define _UNICODE, you must verify.
// 	char szUtf8[500];
// 	int nLen = WideCharToMultiByte(CP_UTF8, 0, (LPCTSTR)msg, msg.GetLength(), NULL, 0, NULL, NULL);
//	char* szUtf8 = new char[nLen + 1];
// 	WideCharToMultiByte(CP_UTF8, 0, (LPCTSTR)msg, msg.GetLength(), szUtf8, nLen, NULL, NULL);
	_write(hFile, pszUTF8, strlen(pszUTF8));
//	delete[] szUtf8;
#else
	_write(hFile, (LPCTSTR)msg, msg.GetLength()*sizeof(TCHAR));
#endif
	_close(hFile);

	msg.Empty();

	return 0;
}

int CLogger::CheckDirectory(LPCTSTR dir, BOOL bCreate)
{
	TCHAR drive[_MAX_DRIVE];
	TCHAR path[_MAX_DIR];
	TCHAR fileName[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];

	_tsplitpath(dir, drive, path, fileName, ext);

	TCHAR szFullPath[_MAX_PATH];
	_stprintf(szFullPath, _T("%s%s\0"), drive, path);

	if (_taccess(szFullPath, 0) == 0) // if exist
	{
		return 0;
	}

	if (bCreate)
	{
		TCHAR* pPath = szFullPath;
		TCHAR* pCurStr = szFullPath;

		BOOL bNetPath = FALSE;
		if (*pCurStr == _T('\\') && *(pCurStr+1) == _T('\\'))
		{
			bNetPath = TRUE;
			pCurStr+=2;
		}

		while (true)
		{
			if (*pCurStr == _T('\\') || *pCurStr == _T('/') || *pCurStr == _T('\0'))
			{
				if (*pCurStr == _T('\\') && bNetPath)
				{
					bNetPath = FALSE;
					pCurStr++;
					continue;
				}
				TCHAR ch = *pCurStr;
				*pCurStr = _T('\0');

				if (_taccess(pPath, 0) != 0) // if not exist
				{
					if (CreateDirectory(pPath, NULL) == FALSE)
					{
						return -2; // fail to create directory
					}
				}
				*pCurStr = ch;

				if (*pCurStr == _T('\0'))
					break;
			}
			pCurStr++;
		}
	}
	else
		return -1; // directory does not exist

	return 0;
}

CString CLogger::GetFullPath()
{
	CString strFullPath;
	CString strPath = _T("");
	SYSTEMTIME st;
	GetLocalTime(&st);

	switch(m_ePathType)
	{
	case LogPathType::Specified:
		strPath = m_strSubDir;
		break;
	case LogPathType::DAY:
		strPath.Format(_T("%04d%02d%02d"), st.wYear, st.wMonth, st.wDay);
		break;
	case LogPathType::MONTH:
		strPath.Format(_T("%04d-%02d"), st.wYear, st.wMonth);
		break;
	case LogPathType::NAME:
		strPath.Format(_T("%s"), m_strName);
		break;
	case LogPathType::ROOT:
		break;
	case LogPathType::DAY_NAME:
		strPath.Format(_T("%04d%02d%02d\\%s"), st.wYear, st.wMonth, st.wDay, m_strName);
		break;
	case LogPathType::NAME_MONTH:
		strPath.Format(_T("%s\\%04d-%02d"), m_strName, st.wYear, st.wMonth);
		break;
	case LogPathType::NAME_DAY:
		strPath.Format(_T("%s\\%04d%02d%02d"), m_strName, st.wYear, st.wMonth, st.wDay);
		break;
	default:
		break;
	}
	strFullPath = (strPath.IsEmpty()) ? (m_strRootPath + _T("\\")) : ((m_strRootPath + _T("\\") + strPath + _T("\\")));
	CheckDirectory(strFullPath, TRUE);

	return strFullPath;
}

CString CLogger::GetFileName()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	int nFileNumber = 1;
	CString strFullName;
	CString strFileName;

	switch (m_eStoreType)
	{
	case LogFileType::eSpecified:
		strFileName = m_strFileName;
		break;
	case LogFileType::e12Hour:
		if (st.wHour > 11)
			strFileName.Format(_T("%04d%02d%02d(PM)-%s"), st.wYear, st.wMonth, st.wDay, m_strName);
		else
			strFileName.Format(_T("%04d%02d%02d(AM)-%s"), st.wYear, st.wMonth, st.wDay, m_strName);
		break;
	case LogFileType::eDay:
		strFileName.Format(_T("%04d%02d%02d-%s"), st.wYear, st.wMonth, st.wDay, m_strName);
		break;
	case LogFileType::eCapacity:		
		while (TRUE)
		{
			strFileName.Format(_T("%04d%02d%02d(%02d)-%s-%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, m_strName, nFileNumber);
			strFullName = GetFullPath() + _T("\\") + strFileName + _T(".") + m_strLogExt;
			if (NeedCreateNew(strFullName))
			{
				nFileNumber++;
				continue;
			}
			break;
		}
		break;
	case LogFileType::eHour:
		strFileName.Format(_T("%04d%02d%02d(%02d)-%s"), st.wYear, st.wMonth, st.wDay, st.wHour, m_strName);
		break;
	default:
		strFileName.Format(_T("%04d%02d%02d(%02d)-%s"), st.wYear, st.wMonth, st.wDay, st.wHour, m_strName);
		break;
	}

	return strFileName;
}

BOOL CLogger::NeedCreateNew(LPCTSTR filename)
{
	WIN32_FIND_DATA wfd;
	HANDLE hSrch;
	DWORD nSize = -1;
	BOOL bRet = FALSE;

	hSrch = FindFirstFile(filename, &wfd);
	if (hSrch == INVALID_HANDLE_VALUE)
	{
		return bRet;
	}

	nSize = /*wfd.nFileSizeHigh*(MAXDWORD+1) +*/ wfd.nFileSizeLow;	// log 파일이 4GB 넘는 경우는 없다고 보므로..., 4GB이상도 고려하면 /* */ 해제후 type __int64로 변경.

	FindClose(hSrch);

	if (nSize >= m_nCapacity)
		bRet = TRUE;

	return bRet;
}

UINT CLogger::ThreadProc(LPVOID pParam)
{
	CLogger *pThis = reinterpret_cast<CLogger*>(pParam);

	return pThis->WriteProcedure();
}
