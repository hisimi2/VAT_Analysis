#ifndef __LOGGER_H__
#define  __LOGGER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <queue>

// CLogger 명령 대상입니다.
namespace LogItem
{
	enum eLogItem
	{
		Filename	= 0x1,
		LineNumber	= 0x2,
		Function    = 0x4,
		Date        = 0x8,
		Time     	= 0x10,		
		ThreadId	= 0x20,
		LoggerName  = 0x40,
		Level    	= 0x80,
		NewLine     = 0x100,
		Header      = 0x200
	};
}

namespace LogFileType
{
	enum eLogFileType
	{
		eHour        = 0,	// 20130405(21)-test.log
		eDay         = 1,	// 20130405-test.log
		eCapacity    = 2,   // 20130405(21)-test-01.log
		e12Hour      = 3,	// 20130405(AM)-test.log
		eSpecified   = 4,   // Specified Name
		eIpDateTime  = 5,   // 192.168.0.11_2016_0421_101515_test.log
        eLotIdTime   = 6,   //  FPGA_20160422_102132.csv
	};
}

namespace LogPathType
{
	enum eLogPathType
	{                    // eg.
		DAY = 0,	     // D:\Log\20130312\test.log
		MONTH = 1,       // D:\Log\9월\test.log
		NAME = 2,        // D:\Log\Comm\test.log
		ROOT = 3,        // D:\Log\test.log
		DAY_NAME = 4,    // D:\Log\20130312\Comm\test.log
		NAME_MONTH = 5,  // D:\Log\Comm\9월\test.log
		NAME_DAY = 6,    // D:\Log\Comm\20130312\test.log
		Specified = 7,   // D:\Log\specified sub path\test.log
		TW1,             // special format1
		TW2,             // special format2
		MONTH_DAY,		 // D:\Log\2016_04\21\test.log
	};
}

class CTWLogger
{
public:
	CTWLogger();
	CTWLogger(LPCTSTR name);
	virtual ~CTWLogger();

	void SetDefaultOptions();
	void PutLog(LPCTSTR msg);
	void SetRootPath(LPCTSTR root);
	void SetSubDir(LPCTSTR sub);
	void SetExtension(LPCTSTR ext);
	void SetFileName(LPCTSTR fname);
	void SetLoggerName(LPCTSTR name);
	void SetHeader(LPCTSTR header);
	void SetLogItem(int item);
	void SetStoreType(LogFileType::eLogFileType type);
	void SetPathType(LogPathType::eLogPathType type);
    void SetLotName(LPCTSTR PkgName);
	void SetIpAddress(LPCTSTR Ip);
	LogFileType::eLogFileType GetStoreType();
	LogPathType::eLogPathType GetPathType();
	CString GetFileName();
	CString GetFullPath(); // except filename and extension
	int CheckDirectory(LPCTSTR dir, BOOL bCreate);
	//int Create(LPCTSTR name);
	void Enable(BOOL enable);
	BOOL IsEnabled();
	UINT WriteProcedure();
	int WriteLog(CString msg);
	int GetLogItem();
	CString GetLoggerName();
	CString GetRootPath();
	CString GetExtension();
	BOOL NeedCreateNew(LPCTSTR filename);
	BOOL Run();
	static UINT ThreadProc(LPVOID pParam);

    void SetDeletePath(LPCTSTR Path);
    void DeleteOldestFile( void );
    void SubFileDelete(CString cstrFileName);
	void SetScheduleNo( LPCTSTR strScheduleNo);

	void OeeBackUp_Del();
	void ProcBackUp_Del();

private:
	CString m_strRootPath;
	CString m_strSubDir;
	//CString m_strFullPath; // except filename and extension
	CString m_strLogExt;
	CString m_strFileName;
	CString m_strName;
	CString m_strHeader;
    CString m_strLotId;
    CString m_strDeletePath;
	CString m_strIp;
	CString m_strScheduleNo;

	std::queue<CString> m_qMessage;
	//CMyEvent	m_EvtLogQue;
	BOOL m_bEnabled;
	BOOL m_bRun;

	CRITICAL_SECTION m_csQueue;
	CRITICAL_SECTION m_csLog;

	int m_nLogItem;
	DWORD m_nCapacity;	// unit: Byte
	LogFileType::eLogFileType m_eStoreType;
	LogPathType::eLogPathType m_ePathType;

	HANDLE m_hThread;
	HANDLE m_hObject;
};


#endif  // __LOGGER_H__