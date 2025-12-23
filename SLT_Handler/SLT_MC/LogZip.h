#pragma once

#include "afxdialogex.h"
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <iostream>

class CLogZip
{
public:
	CLogZip();
	~CLogZip();
	CWinThread* m_pThread;
	double m_dLogTimeCheck;
	double m_dDumpTimeCheck;
	typedef struct stLogFileName
	{
		CString strFullPath;
		CString strFileName;
	}stLogFileName;
private:
	std::vector<stLogFileName> m_veLogFileNames;
	CString m_strTime;
	CString m_strMonth;

public:
	void CreateLogZip();
	int LogFolderInFileNameSearch();
	void DateCal();
	void DeleteOldestFile(void);
	std::vector<std::string> GetRunningProcessNames();
};

