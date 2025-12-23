#ifndef __LOG_H__
#define __LOG_H__

#include "TWLogger.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum eLogID
{
	eLogID_Auto_Stacker1 = 0,
	eLogID_Auto_Stacker2,
	eLogID_Auto_Stacker3,
	eLogID_Auto_Stacker4,
	eLogID_Auto_Stacker5,
	eLogID_Auto_Stacker6,
	eLogID_Auto_Stacker7,
	eLogID_Auto_Press_1,
	eLogID_Auto_Press_2,
	eLogID_Auto_Press_3,
	eLogID_Auto_Press_4,
	eLogID_Auto_Press_5,
	eLogID_Auto_Press_6,
	eLogID_Auto_Press_7,
	eLogID_Auto_Press_8,
	eLogID_Auto_Test_1,
	eLogID_Auto_Test_2,
	eLogID_Auto_LoadTbl_1,
	eLogID_Auto_LoadTbl_2,
	eLogID_Auto_LoadTbl_3,
	eLogID_Auto_LoadTbl_4,
	eLogID_Auto_Tray_1,
	eLogID_Auto_Tray_2, 
	eLogID_Auto_TrayFeeder,
	eLogID_Auto_Transfer,
	eLogID_IPC_GUI_Comm,
	eLogID_IPC_MC_Comm,
	eLogID_LoadTable_1,
	eLogID_LoadTable_2,
	eLogID_LoadTable_3,
	eLogID_LoadTable_4,
	eLogID_PressUnit_1,
	eLogID_PressUnit_2,
	eLogID_PressUnit_3,
	eLogID_PressUnit_4,
	eLogID_PressUnit_5,
	eLogID_PressUnit_6,
	eLogID_PressUnit_7,
	eLogID_PressUnit_8,

	eLogID_Stacker_1,
	eLogID_Stacker_2,
	eLogID_Stacker_3,
	eLogID_Stacker_4,
	eLogID_Stacker_5,
	eLogID_Stacker_6,
	eLogID_Stacker_7,

	eLogID_TestPP_1,
	eLogID_TestPP_2,
	eLogID_LoadUnder,
	eLogID_UnloadUnder,
	eLogID_Vibrator,
	eLogID_Transfer,
	eLogID_TesterIF,
	eLogID_Auto_SYSTEM,
	eLogID_TrayPP_1,
	eLogID_TrayPP_2,
	eLogID_TrayFeeder,
	eLogID_Agent_1,
	eLogID_Agent_2,
	eLogID_Agent_3,
	eLogID_Agent_4,
	eLogID_Agent_5,
	eLogID_Agent_6,
	eLogID_Agent_7,
	eLogID_Agent_8,

	eMaxLogCount,

	eLogID_OEEDATA,
	eLogID_PRODUCTCSV,
	eLogID_TCPIP,
	eLogID_System_GUI,
	eLogID_SYSTEM,
	eLogID_Bonjour,
	eLogID_AgentIF,
	eLogID_Teaching,
	eLogID_CylinderTime,
	eLogID_Teaching_Offset,
	eLogID_ALARM,

	eLogID_TCPIP_TESTER_REG,
	eLogID_TCPIP_TESTER_STAUS,
	eLogID_TCPIP_TEST_RESULT,
	eLogID_TCPIP_TESTER_INFORMATION,
	eLogID_TCPIP_HEART_BEAT,
	eLogID_TCPIP_TESTER_SCRIPT_INFORMATION,
	eLogID_TCPIP_SET_TEST_MODE,
	eLogID_TCPIP_APL_INFO,
	
// 	eLogID_TCPIP_1,
// 	eLogID_TCPIP_2,
// 	eLogID_TCPIP_3,
// 	eLogID_TCPIP_4,
// 	eLogID_TCPIP_5,
// 	eLogID_TCPIP_6,
// 	eLogID_TCPIP_7,
// 	eLogID_TCPIP_8,

	eLogID_2DIDCSV,

	//VAT Log ID
	eLogID_VAT_Auto_System,
	eLogID_VAT_Auto_Tray_1,
	eLogID_VAT_Auto_Tray_2,
	eLogID_VAT_Auto_LD_Test_1,
	eLogID_VAT_Auto_UD_Test_1,
	eLogID_VAT_Auto_LD_Test_2,
	eLogID_VAT_Auto_UD_Test_2,

	eLogID_VAT_Tray_1,
	eLogID_VAT_Tray_2,
	eLogID_VAT_Test_1,
	eLogID_VAT_Test_2,

	eLogID_VAT_LoadTable_1,
	eLogID_VAT_LoadTable_2,
	eLogID_VAT_LoadTable_3,
	eLogID_VAT_LoadTable_4,

	eLogID_VAT_GUI,

	eMaxVatLogCount,
};


enum eLogLevel
{
	Info,		
	Debug,
	Warning,
	Errorr
};

class AFX_EXT_CLASS CLog/* : public CObject*/
{
private:	
	CLog();
	virtual ~CLog();

	static CLog *pInstance;

	CMap<int, int, CTWLogger*, CTWLogger*&> m_Log;
	CWinThread* m_pThread;
	HANDLE m_hThread;

	void loglevelToString(eLogLevel level, LPTSTR strLevel);
	void AddLogEntry(eLogID id, CTWLogger* pLogger);
	void AddLogEntry(int key, CTWLogger* pLogger);
	void RemoveLogEntry(eLogID id);
	void RemoveLogEntry(int key);
public:

	BOOL m_bRun;
	CStringArray m_arRootPath;

	static CLog& getInstance()
	{
		if (pInstance == NULL)
			pInstance = new CLog();
		return *pInstance;

	}
	static CLog* getInstancePtr()
	{
		if (pInstance == NULL)
			pInstance = new CLog();
		return pInstance;
	}
	static void releaseInstance()
	{
		if (pInstance != NULL)
			delete pInstance;
	}

	int MakeLog(int key, eLogLevel level, LPCTSTR file, INT line, LPCTSTR func, LPCTSTR fmt, ...);
	int CreateLog(eLogID id, LPCTSTR name);
	int CreateLog(int id, LPCTSTR name);
	int Run();
	int Stop();
	void GetLogger(int id, CTWLogger* pLogger);
    void GetLogger(eLogID id, CTWLogger* pLogger);
	CMap<int, int, CTWLogger*, CTWLogger*&>* GetLogPtr();

	BOOL SetRootPath(int key, LPCTSTR root);
	BOOL SetExtension(int key, LPCTSTR ext);
	BOOL SetLogItem(int key, int item);
	BOOL SetStoreType(int key, LogFileType::eLogFileType type);
	BOOL SetPathType(int key, LogPathType::eLogPathType type);
	BOOL SetIpAddress(int key, LPCTSTR Ip);
	BOOL Enable(int key, BOOL enable);
	BOOL SetSubDir(int key, LPCTSTR dir);
	BOOL SetFileName(int key, LPCTSTR file);
    BOOL SetLotName(int key, LPCTSTR LotId);
    BOOL SetDeletePath(int key, LPCTSTR Path);
	BOOL SetHeader(int key, LPCTSTR header);

	int DeleteExpiredFile(CString strFolder);
	void CheckExpiredByFileName(LPCTSTR filename);
	void CheckExpiredByCreatedTime(LPCTSTR filename);

	void SetScheduleNo(int key, LPCTSTR ScheduleNo);
	void DataBackUp_Del(int key);
};

UINT ThreadDeleteLog(LPVOID pParam);

#endif  //__LOG_H__