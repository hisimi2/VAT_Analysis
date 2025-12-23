// Logger.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <share.h>
#include "TWLogger.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLogger

CTWLogger::CTWLogger():m_strName(_T("TWInfo"))
{
	SetDefaultOptions();
	InitializeCriticalSection(&m_csLog);
}

CTWLogger::CTWLogger(LPCTSTR name):m_strName(name)
{
	SetDefaultOptions();
	InitializeCriticalSection(&m_csLog);
}

CTWLogger::~CTWLogger()
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
	m_strLotId.Empty();
	m_strDeletePath.Empty();
}

void CTWLogger::SetDefaultOptions()
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
	m_strIp = _T("None");
	m_strScheduleNo =_T("");

    m_strDeletePath.Empty();
}

// CLogger 멤버 함수
void CTWLogger::Enable(BOOL enable)
{
	m_bEnabled = enable;
	if (!enable)
	{
		m_bRun = FALSE;
		::SetEvent(m_hObject);
	}
}

BOOL CTWLogger::IsEnabled()
{
	return m_bEnabled;
}

void CTWLogger::SetExtension(LPCTSTR ext)
{
	m_strLogExt = ext;
}

void CTWLogger::SetFileName(LPCTSTR fname)
{
	m_strFileName = fname;
}

void CTWLogger::SetLotName(LPCTSTR PkgName)
{
    m_strLotId = PkgName;
}

void CTWLogger::SetDeletePath(LPCTSTR Path)
{
    m_strDeletePath = Path;
}

void CTWLogger::SetRootPath(LPCTSTR root)
{
	m_strRootPath = root;
}

void CTWLogger::SetSubDir(LPCTSTR sub)
{
	m_strSubDir = sub;
}

void CTWLogger::SetLoggerName(LPCTSTR name)
{
	m_strName = name;
}

void CTWLogger::SetHeader(LPCTSTR header)
{
	m_strHeader = header;
}

void CTWLogger::SetLogItem(int item)
{
	m_nLogItem = item;
}

int CTWLogger::GetLogItem()
{
	return m_nLogItem;
}

CString CTWLogger::GetExtension()
{
	return m_strLogExt;
}

CString CTWLogger::GetRootPath()
{
	return m_strRootPath;
}

CString CTWLogger::GetLoggerName()
{
	return m_strName;
}

LogFileType::eLogFileType CTWLogger::GetStoreType()
{
	return m_eStoreType;
}

LogPathType::eLogPathType CTWLogger::GetPathType()
{
	return m_ePathType;
}

void CTWLogger::SetStoreType(LogFileType::eLogFileType type)
{
	m_eStoreType = type;
}
void CTWLogger::SetPathType(LogPathType::eLogPathType type)
{
	m_ePathType = type;
}

void CTWLogger::SetIpAddress( LPCTSTR strIp )
{
	m_strIp = strIp;
}

void CTWLogger::SetScheduleNo( LPCTSTR strScheduleNo)
{
	m_strScheduleNo = strScheduleNo;
}

BOOL CTWLogger::Run()
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

void CTWLogger::PutLog(LPCTSTR msg)
{
	EnterCriticalSection(&m_csLog);
	m_qMessage.push(msg);
	::SetEvent(m_hObject);
	LeaveCriticalSection(&m_csLog);
}

UINT CTWLogger::WriteProcedure()
{
	CString msg;

	do 
	{
		WaitForSingleObject(m_hObject, INFINITE);

		if (!m_qMessage.empty())
		{
			std::queue<CString> qMsg;
			EnterCriticalSection(&m_csLog);
			std::swap(qMsg, m_qMessage);
			LeaveCriticalSection(&m_csLog);

			while (!qMsg.empty())
			{
				msg += qMsg.front();
				qMsg.front().Empty();
				qMsg.pop();
			}
			WriteLog(msg);
			msg.Empty();
		}
		

		//EnterCriticalSection(&m_csLog);
		//while (!m_qMessage.empty())
		//{
		//	msg = m_qMessage.front();
		//	m_qMessage.front().Empty();
		//	m_qMessage.pop();
		//	WriteLog(msg);
		//}
		//LeaveCriticalSection(&m_csLog);
		//msg.Empty();
	} while (m_bRun);

	return 0;
}

int CTWLogger::WriteLog(CString msg)
{
	int hFile;
	CString strFullName;

	strFullName = GetFullPath() + _T("\\") + GetFileName() + _T(".") + m_strLogExt;

	//int oflag = 0;

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

int CTWLogger::CheckDirectory(LPCTSTR dir, BOOL bCreate)
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

CString CTWLogger::GetFullPath()
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
		strPath.Format(_T("%04d_%02d"), st.wYear, st.wMonth);
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
		strPath.Format(_T("%s\\%04d_%02d"), m_strName, st.wYear, st.wMonth);
		break;
	case LogPathType::NAME_DAY:
		strPath.Format(_T("%s\\%04d%02d%02d"), m_strName, st.wYear, st.wMonth, st.wDay);
		break;
	case LogPathType::MONTH_DAY:
		{
			strPath.Format(_T("%04d_%02d\\%02d"), st.wYear, st.wMonth, st.wDay);			
		}break;
	default:
		break;
	}
	strFullPath = (strPath.IsEmpty()) ? (m_strRootPath + _T("\\")) : ((m_strRootPath + _T("\\") + strPath + _T("\\")));
	CheckDirectory(strFullPath, TRUE);

	return strFullPath;
}

CString CTWLogger::GetFileName()
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
		strFileName.Format(_T("%04d%02d%02d_%s"), st.wYear, st.wMonth, st.wDay, m_strName);
		break;
	case LogFileType::eCapacity:		
		while (TRUE)
		{
			strFileName.Format(_T("%04d%02d%02d(%02d)_%s_%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, m_strName, nFileNumber);
			strFullName = GetFullPath() + _T("\\") + strFileName + _T(".") + m_strLogExt;
			if (NeedCreateNew(strFullName))
			{
				nFileNumber++;
				continue;
			}
			break;
		}
		break;
	case LogFileType::eIpDateTime:
		{
//             if(m_strFileName.IsEmpty() != TRUE){
//                 strFileName = m_strFileName;
//                 strFullName = GetFullPath() + _T("\\") + strFileName + _T(".") + m_strLogExt;
//                 if(NeedCreateNew(strFullName)){
//                     strFileName.Format(_T("%s"),strIP);
//                     m_strFileName = strFileName;
//                 }
//             }else{
                strFileName.Format(_T("%s"), m_strIp);
                m_strFileName = strFileName;
//            }            
		}break;
	case LogFileType::eHour:
		strFileName.Format(_T("%04d%02d%02d(%02d)_%s"), st.wYear, st.wMonth, st.wDay, st.wHour, m_strName);
		break;
    case LogFileType::eLotIdTime :   // 20160422FPGA_20160422_102132.csv
        {
//             if(m_strFileName.IsEmpty() != TRUE){
//                 strFileName = m_strFileName;
//                 strFullName = GetFullPath() + _T("\\") + strFileName + _T(".") + m_strLogExt;
//                 if(NeedCreateNew(strFullName)){
//                     strFileName.Format(_T("%s_%04d%02d%02d_%02d%02d%02d"), m_strLotId, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
//                     CString strBackUpName = GetFullPath() + _T("\\") + strFileName + _T(".") + m_strLogExt;
//                     MoveFile(strFullName, strBackUpName);
//                     
//                     strFileName.Format(_T("%s"), m_strLotId);
//                     m_strFileName = strFileName;
// 
//                     if(m_strDeletePath.IsEmpty() != TRUE){
//                         DeleteOldestFile();
//                     }                    
//                 }
//             }else{
                //strFileName.Format(_T("%s_%04d%02d%02d_%02d%02d%02d"), m_strLotId, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
                
                strFileName.Format(_T("%s"), m_strLotId);
                m_strFileName = strFileName;
//            }
        }break;
	default:
		strFileName.Format(_T("%04d%02d%02d(%02d)_%s"), st.wYear, st.wMonth, st.wDay, st.wHour, m_strName);
		break;
	}

	return strFileName;
}

BOOL CTWLogger::NeedCreateNew(LPCTSTR filename)
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

UINT CTWLogger::ThreadProc(LPVOID pParam)
{
	CTWLogger *pThis = reinterpret_cast<CTWLogger*>(pParam);

	return pThis->WriteProcedure();
}

void CTWLogger::DeleteOldestFile( void )
{
    // 삭제 설정 기간에 따라 timeLimit 설정
    CTime timeLimit = CTime::GetCurrentTime();
    timeLimit -= CTimeSpan( 30, 0, 0, 0 );

    // cstrFindFile_YYYYMM : 파일 찾는데 필요한 경로 저장하는 변수
    CString cstrFindFile_YYYYMM = m_strDeletePath;//"D:\\TechWing\\Logs\\TCPIP";
    cstrFindFile_YYYYMM += "\\*.*";
    CFileFind Finder_YYYYMM;	// YYYY_MM 폴더 Find
    BOOL bExist_YYYYMM = Finder_YYYYMM.FindFile( cstrFindFile_YYYYMM );
    while( bExist_YYYYMM )	// D:\\TechWing\\LOG\\TCPIP\\YYYY_MM\\DD	YYYY_MM 폴더 탐색
    {
        bExist_YYYYMM = Finder_YYYYMM.FindNextFile();	
        if( Finder_YYYYMM.IsDots()) continue;
        else if( Finder_YYYYMM.IsDirectory() ){

            // cstrDelFileFindPath : 폴더 경로 저장
            CString cstrDelFileFindPath = Finder_YYYYMM.GetFilePath();
            int nBaseIndex_YYYYMM = cstrDelFileFindPath.ReverseFind('\\') + 1;
            char szTemp[128]={0};
            char szYear[32]={0};
            char szMonth[32]={0};
            char szDay[32]={0};

            sprintf_s(szYear, sizeof(szYear), "%s", cstrDelFileFindPath.Mid(nBaseIndex_YYYYMM,4).GetString() );
            sprintf_s(szMonth, sizeof(szMonth), "%s", cstrDelFileFindPath.Mid(nBaseIndex_YYYYMM+5,2).GetString() );
            int Y = atoi(szYear);
            int M = atoi(szMonth);

            cstrDelFileFindPath += "\\*.*";

            CFileFind Finder_DD;	// DD 폴더 Find
            BOOL bExist_DD = Finder_DD.FindFile( cstrDelFileFindPath );
            while( bExist_DD ){		// D:\\TechWing\\LOG\\TCPIP\\YYYY_MM\\DD	DD폴더 탐색
                bExist_DD = Finder_DD.FindNextFile();
                if( Finder_DD.IsDots()) continue;
                else if( Finder_DD.IsDirectory() ){
                    CString cstrFileName_DD = Finder_DD.GetFilePath();
                    int nBaseIndex_DD = cstrFileName_DD.ReverseFind('\\') + 1;

                    sprintf_s(szDay, sizeof(szDay), "%s", cstrFileName_DD.Mid(nBaseIndex_DD, 2).GetString() );
                    int D = atoi(szDay);

                    CTime DirTime(Y,M,D,0,0,0);

                    if( DirTime < timeLimit )
                    {
                        SubFileDelete(cstrFileName_DD);						// 폴더 안 파일들 삭제
                        int nRtn = ::RemoveDirectory( cstrFileName_DD );	// 폴더 삭제
                        nRtn = ::GetLastError();
                    }
                }
            }
            Finder_DD.Close();	// 검색 종료

            cstrDelFileFindPath = Finder_YYYYMM.GetFilePath();
            int nRtn = ::RemoveDirectory( cstrDelFileFindPath );	// 빈 폴더 삭제
            nRtn = ::GetLastError();
        }
    }
    Finder_YYYYMM.Close();	// 검색 종료
}

void CTWLogger::SubFileDelete(CString cstrFileName)
{
    CString cstrFindFile;
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
}

void CTWLogger::OeeBackUp_Del()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString strFullName = _T("");
	CString strFileName = _T("");

	strFileName = m_strFileName;
	strFullName = GetFullPath() + _T("\\") + strFileName + _T(".") + m_strLogExt;

	strFileName.Format(_T("%s_%04d_%02d_%02d_%02d%02d%02d%02d"), m_strFileName, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	CString strBackUpName = GetFullPath() + _T("\\") + strFileName + _T(".") + m_strLogExt;
	MoveFile(strFullName, strBackUpName);
}

void CTWLogger::ProcBackUp_Del()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString strFullName = _T("");
	CString strFileName = _T("");

	strFileName = m_strFileName;
	strFullName = GetFullPath() + _T("\\") + strFileName + _T(".") + m_strLogExt;

	strFileName.Format(_T("%s_%s_%04d%02d%02d_%02d%02d%02d"), m_strLotId, m_strScheduleNo, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	CString strBackUpName = GetFullPath() + _T("\\") + strFileName + _T(".") + m_strLogExt;
	MoveFile(strFullName, strBackUpName);
}
