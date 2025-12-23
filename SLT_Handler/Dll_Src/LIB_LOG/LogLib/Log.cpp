// Log.cpp : 해당 DLL의 초기화 루틴을 정의합니다.
//

#include "stdafx.h"
#include "Log.h"

#include <stdio.h>
#include <stdarg.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CLog* CLog::pInstance = NULL;

CLog::CLog()
{
	m_Log.InitHashTable(100);
	m_hThread = NULL;
	m_pThread = NULL;
	m_bRun = FALSE;
}

CLog::~CLog()
{
	Stop();

	DWORD result = ::WaitForSingleObject(m_hThread, 1000);
	if (result == WAIT_TIMEOUT)
		::TerminateThread(m_hThread, 0);

	::GetExitCodeThread( m_hThread, &result );
	if( m_pThread != nullptr ) {
		delete m_pThread;
		m_pThread = nullptr;
	}

	m_arRootPath.RemoveAll();

	int key;
	CTWLogger* pLogger;
	POSITION pos = m_Log.GetStartPosition();
	while (pos != NULL)
	{
		m_Log.GetNextAssoc(pos, key, pLogger);		
		delete pLogger;
	}
	m_Log.RemoveAll();
}

int CLog::CreateLog(eLogID id, LPCTSTR name)
{
	return CreateLog(static_cast<int>(id), name);
}

int CLog::CreateLog(int id, LPCTSTR name)
{
	CTWLogger* pLogger = (CTWLogger*) new CTWLogger(name);
	pLogger->Enable(TRUE);
	AddLogEntry(id, pLogger);
	return 0;
}

int CLog::MakeLog(int key, eLogLevel level, LPCTSTR file, int line, LPCTSTR func, LPCTSTR fmt, ...)
{
	CTWLogger* pLogger;
	m_Log.Lookup(key, pLogger);

	if (pLogger == NULL || pLogger->IsEnabled() == FALSE)
		return -1;

	SYSTEMTIME st;

	TCHAR tmpMsg[40960] = {0,};
	TCHAR tmp[20] = {0,};
	CString strMsg = _T("");

	if (fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf(tmpMsg, fmt, argList);
		va_end(argList);
	}

	GetLocalTime(&st);
	int nLogItem = pLogger->GetLogItem();

	if (nLogItem & static_cast<int>(LogItem::Time))
		strMsg.Format(_T("%02d:%02d:%02d.%03d\t"), st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	if (nLogItem & static_cast<int>(LogItem::Filename))
	{
		TCHAR fname[_MAX_FNAME];
		_tsplitpath(file, NULL, NULL, fname, NULL);
		_stprintf(tmp, _T("-%-15s"), fname);
		strMsg += (LPCTSTR)tmp;
	}
	if (nLogItem & static_cast<int>(LogItem::LineNumber))
	{
		_stprintf(tmp, _T("(%5d) "), line);
		strMsg += (LPCTSTR)tmp;
	}
	if (nLogItem & static_cast<int>(LogItem::Date))
	{
		_stprintf(tmp, _T("%04d-%02d-%02d "), st.wYear, st.wMonth, st.wDay);
		strMsg.Insert(0, tmp);
	}
	if (nLogItem & static_cast<int>(LogItem::Function))
	{
		_stprintf(tmp, _T("-%-15s "), func);
		strMsg += (LPCTSTR)tmp;
	}
	if (nLogItem & static_cast<int>(LogItem::Level))
	{
		loglevelToString(level, tmp);
		strMsg += (LPCTSTR)tmp;
	}
	strMsg += (LPCTSTR)tmpMsg;
	if (nLogItem & static_cast<int>(LogItem::NewLine))
		strMsg.Insert(strMsg.GetLength(), _T("\n"));

	pLogger->PutLog(static_cast<LPCTSTR>(strMsg));

	return 0;
}

void CLog::loglevelToString(eLogLevel level, LPTSTR pszLevel)
{
	switch (level)
	{
	case Errorr:
		_tcscpy(pszLevel, _T("- E - "));
		break;

	case Warning:
		_tcscpy(pszLevel, _T("- W - "));
		break;

	case Info:
		_tcscpy(pszLevel, _T("- I - "));
		break;

	case Debug:
		_tcscpy(pszLevel, _T("- D - "));
		break;

	default:
		_tcscpy(pszLevel, _T("- I - "));
		break;
	}
}

void CLog::GetLogger(eLogID id, CTWLogger* pLogger)
{
	GetLogger(static_cast<int>(id), pLogger);
}

void CLog::GetLogger(int key, CTWLogger* pLogger)
{
	m_Log.Lookup(key, pLogger);
}

CMap<int, int, CTWLogger*, CTWLogger*&>* CLog::GetLogPtr()
{
	return &m_Log;
}

void CLog::AddLogEntry(eLogID id, CTWLogger* pLogger)
{
	AddLogEntry(static_cast<int>(id), pLogger);
}
void CLog::AddLogEntry(int key, CTWLogger* pLogger)
{
	m_Log.SetAt(key, pLogger);
}

void CLog::RemoveLogEntry(eLogID id)
{
	RemoveLogEntry(static_cast<int>(id));
}

void CLog::RemoveLogEntry(int key)
{
	m_Log.RemoveKey(key);
}

BOOL CLog::SetHeader(int key, LPCTSTR header)
{
	CTWLogger* pLogger;
	m_Log.Lookup(key, pLogger);

	if (pLogger->GetLogItem() & static_cast<int>(LogItem::Header))
		pLogger->SetHeader(header);
	return TRUE;
}

BOOL CLog::SetSubDir(int key, LPCTSTR dir)
{
	CTWLogger* pLogger;
	m_Log.Lookup(key, pLogger);

	if (pLogger->GetPathType() == LogPathType::Specified)
		pLogger->SetSubDir(dir);
	return TRUE;
}

BOOL CLog::SetFileName(int key, LPCTSTR file)
{
	CTWLogger* pLogger;
	m_Log.Lookup(key, pLogger);

	if (pLogger->GetStoreType() == LogFileType::eSpecified)
		pLogger->SetFileName(file);
	return TRUE;
}

BOOL CLog::SetDeletePath(int key, LPCTSTR Path)
{
    CTWLogger* pLogger;
    m_Log.Lookup(key, pLogger);

    if (pLogger->GetStoreType() == LogFileType::eLotIdTime)
        pLogger->SetDeletePath(Path);
    return TRUE;
}

BOOL CLog::SetLotName(int key, LPCTSTR LotId)
{
    CTWLogger* pLogger;
    m_Log.Lookup(key, pLogger);

    if (pLogger->GetStoreType() == LogFileType::eLotIdTime)
        pLogger->SetLotName(LotId);
    return TRUE;
}

BOOL CLog::SetRootPath(int key, LPCTSTR root)
{
	CTWLogger* pLogger;
	m_Log.Lookup(key, pLogger);

	pLogger->SetRootPath(root);
	return TRUE;
}
BOOL CLog::SetExtension(int key, LPCTSTR ext)
{
	CTWLogger* pLogger;
	m_Log.Lookup(key, pLogger);

	pLogger->SetExtension(ext);
	return TRUE;
}
BOOL CLog::SetLogItem(int key, int item)
{
	CTWLogger* pLogger;
	m_Log.Lookup(key, pLogger);

	pLogger->SetLogItem(item);
	return TRUE;
}
void CLog::SetScheduleNo(int key, LPCTSTR ScheduleNo)
{
	CTWLogger* pLogger;
	m_Log.Lookup(key, pLogger);

	pLogger->SetScheduleNo(ScheduleNo);
}

BOOL CLog::SetStoreType(int key, LogFileType::eLogFileType type)
{
	CTWLogger* pLogger;
	m_Log.Lookup(key, pLogger);

	pLogger->SetStoreType(type);
	return TRUE;
}
BOOL CLog::SetPathType(int key, LogPathType::eLogPathType type)
{
	CTWLogger* pLogger;
	m_Log.Lookup(key, pLogger);

	pLogger->SetPathType(type);
	return TRUE;
}
BOOL CLog::Enable(int key, BOOL enable)
{
	CTWLogger* pLogger;
	m_Log.Lookup(key, pLogger);

	pLogger->Enable(enable);
	return TRUE;
}

void CLog::DataBackUp_Del(int key)
{
	CTWLogger* pLogger;
	m_Log.Lookup(key, pLogger);
	
	if(key == eLogID_OEEDATA){
		pLogger->OeeBackUp_Del();
	}
	if(key == eLogID_PRODUCTCSV){
		pLogger->ProcBackUp_Del();
	}
}

BOOL CLog::SetIpAddress( int key, LPCTSTR Ip )
{
	CTWLogger* pLogger;
	m_Log.Lookup(key, pLogger);

	pLogger->SetIpAddress(Ip);
	return TRUE;
}

int CLog::Stop()
{
	m_bRun = FALSE;

	int nKey;
	CTWLogger* pLogger;
	POSITION pos = m_Log.GetStartPosition();
	while (pos != NULL)
	{
		m_Log.GetNextAssoc(pos, nKey, pLogger);
		pLogger->Enable(FALSE);
	}

	return 0;
}

int CLog::Run()
{
	int nKey;
	CTWLogger* pLogger;
	POSITION pos = m_Log.GetStartPosition();
	while (pos != NULL)
	{
		m_Log.GetNextAssoc(pos, nKey, pLogger);
		pLogger->Run();
	}

	if (m_hThread == 0)
	{
		m_bRun = TRUE;
		m_pThread = AfxBeginThread(ThreadDeleteLog, (LPVOID)this);

		if ( m_pThread == nullptr )
		{
			AfxMessageBox(_T("ThreadDeleteLog creation failed"));
			m_bRun = FALSE;
			return -1;
		}
		m_hThread = m_pThread->m_hThread;
	}

	return 0;
}

int CLog::DeleteExpiredFile(CString strFolder)
{
	strFolder.TrimRight(_T("\\"));
	CFileFind ff;
	BOOL b = ff.FindFile(strFolder + _T("\\*"));
	CStringArray folders;
	while (b)
	{
		Sleep(1);
		b = ff.FindNextFile();
		if (ff.IsDots())
			continue;
		if (ff.IsDirectory())
		{
			folders.Add(ff.GetFilePath());
		}
		else
		{
			CheckExpiredByCreatedTime(ff.GetFilePath());
		}
	}
	
	for (int i=0; i<folders.GetSize(); i++)
	{
		DeleteExpiredFile(folders[i]);
	}
	
	folders.RemoveAll();
	ff.Close();
	return 0;
}

void CLog::CheckExpiredByFileName(LPCTSTR filename)
{
	CString strFilePath = filename;
	CString strFileName = strFilePath.Right(strFilePath.GetLength() - strFilePath.ReverseFind(_T('\\')) - 1);
	int nYear = _ttoi(strFileName.Left(4));
	int nMonth = _ttoi(strFileName.Mid(4, 2));
    int nDay = _ttoi(strFileName.Mid(6, 2));
	
	COleDateTime createdDate = COleDateTime(nYear, nMonth, nDay, 0, 0, 0);
	COleDateTime currentTime = COleDateTime::GetCurrentTime();
	COleDateTimeSpan timeSpan = currentTime - createdDate;
	
	if (timeSpan.GetDays() > 25)	// TODO: get expire day from configuration
	{
		::DeleteFile(strFilePath);
	}
}

void CLog::CheckExpiredByCreatedTime(LPCTSTR filename)
{
	CString strFileName = filename;
	WIN32_FIND_DATA fd = {0};
	HANDLE hFile = FindFirstFile(strFileName, &fd);
	FindClose(hFile);
	
	SYSTEMTIME st, lt;
	FileTimeToSystemTime(&fd.ftLastWriteTime, &st);
	SystemTimeToTzSpecificLocalTime(NULL, &st, &lt);
	COleDateTime createdTime(lt);
	COleDateTime currentTime = COleDateTime::GetCurrentTime();
	COleDateTimeSpan timeSpan = currentTime - createdTime;
	
	if (timeSpan.GetDays() > 25)	// TODO: get expire day from configuration
		::DeleteFile(strFileName);
}

UINT ThreadDeleteLog(LPVOID pParam)
{
	int nKey;
	CTWLogger* pLogger;
	CLog* pThis = reinterpret_cast<CLog*>(pParam);

	if (pThis == NULL/* || typeid(*pThis) == typeid(CLog)*/)
		return -1;

	CMap<int, int, CTWLogger*, CTWLogger*&>* pLog = pThis->GetLogPtr();
	POSITION pos = pLog->GetStartPosition();

	while (pos != NULL)
	{
		pLog->GetNextAssoc(pos, nKey, pLogger);
		if (pLogger->IsEnabled() && pLogger->GetLoggerName().Compare(_T("TestErrCode")) != 0)
		{	
			bool bFindSamePath = false;
			for (int i = 0; i < pThis->m_arRootPath.GetSize(); i++)
			{
				if (pLogger->GetRootPath().Compare(pThis->m_arRootPath.GetAt(i)) == 0)
					bFindSamePath = true;
			}
			if( bFindSamePath == false)
				pThis->m_arRootPath.Add(pLogger->GetRootPath());
		}			
	}

	while (pThis->m_bRun)
	{
		TRY 
		{
			for (int i=0; i<pThis->m_arRootPath.GetSize(); i++)
			{
				pThis->DeleteExpiredFile(pThis->m_arRootPath.GetAt(i));
			}
		}
		CATCH (CMemoryException, e)
		{
			e->ReportError(); 
		}
		END_CATCH

		Sleep(300000);
	}

	return 0;
}