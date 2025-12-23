#include "stdafx.h"
#include "LogZip.h"

UINT ThreadLogZip(LPVOID param);

CLogZip::CLogZip()
{
	m_dLogTimeCheck = (double)GetTickCount64();
	m_dDumpTimeCheck = (double)GetTickCount64();
	m_veLogFileNames.clear();
	m_pThread = AfxBeginThread(ThreadLogZip, this);
}

CLogZip::~CLogZip()
{
	WaitForSingleObject(m_pThread->m_hThread, NULL);
}

void CLogZip::DateCal()
{
	CTime cTime = CTime::GetCurrentTime();
	int nYear = 0, nMonth = 0, nDay = 0, nLeapYear = cTime.GetYear();
	int mdays[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	BOOL bLeapYear = FALSE;

	if ((nLeapYear % 4 == 0 && nLeapYear % 100 != 0) || nLeapYear % 400 == 0) //윤년 계산
		bLeapYear = TRUE;

	if (cTime.GetDay() == 1)
	{
		if (cTime.GetMonth() == 1)
		{
			nYear = cTime.GetYear()-1;
			nMonth = 12;
			nDay = mdays[11];
		}
		else if (bLeapYear == TRUE && cTime.GetMonth() == 3 && cTime.GetDay() == 1)
		{
			nYear = cTime.GetYear();
			nMonth = cTime.GetMonth() - 1;
			nDay = 29;
		}
		else
		{
			nYear = cTime.GetYear();
			nMonth = cTime.GetMonth() - 1;
			nDay = mdays[nMonth - 1];
		}
	}
	else
	{
		nYear = cTime.GetYear();
		nMonth = cTime.GetMonth();
		nDay = cTime.GetDay() - 1;	
	}
		
	m_strTime.Format("%04d%02d%02d", nYear, nMonth, nDay);
}

int CLogZip::LogFolderInFileNameSearch()
{
	m_veLogFileNames.clear();

	CString tpath, strPathZip;
	CFileFind finder,finderBackUp;

	DateCal();
	
	strPathZip.Format("%s\\%s.zip", SZ_DIR_PATH_HANDLER_LOG_BACKUP, m_strTime);
	BOOL bFileExist = finderBackUp.FindFile(strPathZip); //
	if (bFileExist == TRUE)
	{
		finderBackUp.Close();
		return TRUE;
	}

	tpath.Format("%s\\%s\\*.*", SZ_DIR_PATH_HANDLER_LOG, m_strTime);
	BOOL bWorking = finder.FindFile(tpath); //

	while (bWorking)
	{
		//다음 파일 or 폴더 가 존재하면다면 TRUE 반환
		bWorking = finder.FindNextFile();

		// folder 일 경우는 continue
		if (finder.IsDirectory() || finder.IsDots())
			continue;

		// 파일 일때
		stLogFileName st_LogFileName;
		//파일의 이름
		CString _fileName = finder.GetFileName();
		CString _FilePath = finder.GetFilePath();
		// 현재폴더 상위폴더 썸네일파일은 제외
		if (_fileName == _T("Thumbs.db")) continue;

		st_LogFileName.strFullPath += _FilePath;
		st_LogFileName.strFileName += _fileName;

		m_veLogFileNames.push_back(st_LogFileName);
	}

	finder.Close();

	return NO_ERROR;
}

void CLogZip::CreateLogZip()
{
	int nError = LogFolderInFileNameSearch();

	if (nError != NO_ERROR) // 이미 해당 날짜에 압축 된 파일이 있으면 건너 뛴다.
	{
		return;
	}

	CFileFind finder;
	HZIP hz;
	ZRESULT zr;
	CString strPathZip;
	strPathZip.Format("%s\\%s.zip", SZ_DIR_PATH_HANDLER_LOG_BACKUP, m_strTime);

	//D:\ZIP\ 경로에 test.zip이란 파일을 생성한다.
	CreateDirectory(SZ_DIR_PATH_HANDLER_LOG_BACKUP, NULL);

	if ((int)m_veLogFileNames.size() == 0)
	{
		return;
	}

	hz = CreateZip(strPathZip, 0);

	//zip파일 생성 실패시 처리
	if (hz == 0)
	{
		//AfxMessageBox("Error: Failed to create Zip");
		return;
	}

	for (int i = 0; i < (int)m_veLogFileNames.size(); i++)
	{
		zr = ZipAdd(hz, m_veLogFileNames[i].strFileName, m_veLogFileNames[i].strFullPath);	//압축할 파일 명"idx.ini"
		if (zr != ZR_OK)
		{
			zr = CloseZip(hz);
			break;
		}
	}
	//ZipAdd 명령 실패시 처리	
	if (zr != ZR_OK)
	{
		//AfxMessageBox("Error: Failed to add Zip");
		zr = CloseZip(hz);
		return;
	}

	//Zip닫기
	zr = CloseZip(hz);

	if (zr != ZR_OK)
	{
		//AfxMessageBox("Error: Failed to close zip");
		return;
	}
}


UINT ThreadLogZip(LPVOID param)
{
	CLogZip* pMain = (CLogZip*)param;
	

	while (1)
	{
		// LogBack Up Check
		double dLogSpanTime = (double)GetTickCount64() - pMain->m_dLogTimeCheck;
		if (dLogSpanTime > 10000.0) //LogBackUp 10초
		{
			pMain->CreateLogZip();
			pMain->m_dLogTimeCheck = (double)GetTickCount64();
			pMain->DeleteOldestFile();
		}	

// 		// Auto Dump Check
// 		if (g_IO.GetDriverType() != _DIO_TYPE_SIMUL_)
// 		{
// 			double dDumpSpanTime = (double)GetTickCount64() - pMain->m_dDumpTimeCheck;
// 			if (dDumpSpanTime > 100000) // Auto Dump 실행 1분
// 			{
// 				//Windows에 실행 되어 있는 프로세스를 검색해서 Auto Dump가 실행이 되어 있는지 확인 한다.
// 				std::string wstDump = "procdump.exe";
// 				auto vProcessName = pMain->GetRunningProcessNames();
// 				BOOL bProcessOff = FALSE;
// 				for (const auto& name : vProcessName)
// 				{
// 					if (wstDump == name)
// 					{
// 						bProcessOff = TRUE;
// 						break;
// 					}
// 				}
// 
// 				//Auto Dump 실행 파일 종료 시 다시 실행 시키는 부분 추가
// 				if (bProcessOff == FALSE) {
// 					char szTemp[512] = { 0, };
// 					::GetCurrentDirectory(sizeof(szTemp), szTemp);
// 					char szBatPath[512] = { 0, };
// 
// 					sprintf_s(szBatPath, sizeof(szBatPath), "%s\\ProcDump_SLT\\AutoDump.bat", szTemp);
// 					int nErr = (int)ShellExecute(NULL, "open", szBatPath, NULL, NULL, SW_SHOWMINIMIZED);
// 					if (nErr <= 32) { // ShellExecute return 값 32이하 면 Error
// 						g_TaskSystemCtrl.MakeLog("[Failed to execute Batch File. = %s]", szBatPath);
// 					}
// 				}
// 
// 				pMain->m_dDumpTimeCheck = (double)GetTickCount64();
// 			}
// 		}

		Sleep(1000);
	}
	
	return 0;
}

std::vector<std::string> CLogZip::GetRunningProcessNames()
{
	std::vector<std::string> processNames;

	DWORD aProcesses[1024], cbNeeded, cProcesses;
	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
		return processNames;

	cProcesses = cbNeeded / sizeof(DWORD);

	for (unsigned int i = 0; i < cProcesses; ++i)
	{
		if (aProcesses[i] == 0)
			continue;

		DWORD processID = aProcesses[i];
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

		if (hProcess)
		{
			HMODULE hMod;
			DWORD cbNeededModule;

			if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeededModule))
			{
				TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
				GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));

				processNames.push_back(szProcessName);
			}

			CloseHandle(hProcess);
		}
	}

	return processNames;
}

void CLogZip::DeleteOldestFile(void)
{
	CTime timeLimit = CTime::GetCurrentTime();
	timeLimit -= CTimeSpan(365, 0, 0, 0);

	CString cstrFindFile,strLogPath;
	strLogPath += SZ_DIR_PATH_HANDLER_LOG_BACKUP;
	cstrFindFile = strLogPath + "\\*.*";

	CFileFind Finder;
	BOOL bExist = Finder.FindFile(cstrFindFile);
	while (bExist)
	{
		bExist = Finder.FindNextFile();
		if (Finder.IsDots()) continue;
		//else if (Finder.IsDirectory())
		{
			CString cstrFileName = Finder.GetFilePath();
			CFileStatus status;
			CFile::GetStatus(cstrFileName, status);
		
			if (status.m_mtime < timeLimit)
			{
				cstrFindFile = cstrFileName;
				cstrFindFile += "\\*.*";
				CFileFind subFind;
			
				BOOL bSubExist = subFind.FindFile(cstrFileName);

				if(Finder.IsDirectory())
					bSubExist = subFind.FindFile(cstrFindFile);
				
				//bSubExist += subFind.FindFile(cstrFileName);
				while (bSubExist)
				{
					bSubExist = subFind.FindNextFile();
					if (subFind.IsDots()) continue;
					if (subFind.IsDirectory()) continue;
					CString strSubFileName = subFind.GetFilePath();
					::DeleteFile(strSubFileName);
				}
				subFind.Close();
				int nRtn = ::RemoveDirectory(cstrFileName);
			}
		}
	}

	Finder.Close();
}