#pragma once

#include <afxmt.h>
#include <deque>
#include <vector>
#include <string.h>
#include "zip.h"
#include "CppSQLite3.h"
#include "Logger.h"
#include <afxinet.h>

#ifndef __FN_CALLBACK_RECEIVE__
#define __FN_CALLBACK_RECEIVE__
typedef void (*FN_CALLBACK)(CString);
#endif //__FN_CALLBACK_TWIPC_RECEIVE__

enum eErrorCode
{
	eErrorCode_MacFile_NoExist = 0,
	eErrorCode_CopyFile_Mac_to_Temp,
	eErrorCode_CopyFile_Temp_to_Server,
	eErrorCode_Zip,
	eErrorCode_FTP
};

enum eFtp_Type
{
 	eFTP_Server =0,
 	eFTP_Network
};

class AFX_EXT_CLASS CFtpManager
{
public:
	CFtpManager(void);
	~CFtpManager(void);

	// Initialize & Finalize
	int Initialize();
	void Finalize();

	//Call back 
	void CallBackConnect(FN_CALLBACK fnCallback);

	//File 확인 
	void ChkFolderFromMac(int nTesterId);
	int ChkFileCntFromTemp(CString strFolderName);
	int ChkFileFromServer();
	void FolderCheck(int nTesterId);

	//압축
	BOOL MakeZipLogFile(CString strFolderName, CString strNetPath);

	//File 이동

	BOOL TransferLogFileToServer(CString strZipFilePath, CString strFolderName, CString strNetPath);
	BOOL TransferCsvFileToServer(CString strMacPath, CString strNetPath);
	BOOL CopyFileFromMac(int nTesterId, CString str2DBarcode);

	//File 삭제
	BOOL DelFile(CString strFolderName,CString strFileName);
	BOOL DelFolder(CString strFolderName);
	BOOL DelZipFile(CString strZipPath);

	// File Path
	void SetFilePath(int nSite, CString strMacPath, CString strNetPath);

	void FtpDataSet(int nFtpType,CString strIP, CString strID, CString strPassword);
	
	//Lot Number
	void SetLotNo(CString strLotNo);

	//Site Number
	void SetSiteNum(int nSiteNum);

	// Insert Data
	void InsertData(int nTesterId, CString str2DBarcode);

	//Folder create
	void LocalFolderCreate(CString strFolderName);
	void ServerFolderCreate(CString strFolderName, CString strNetPath);

	//Data Clear
	void DataClear();

	//Process
	void FtpProcess();
	void FtpLotEndProcess();

	//Lot End 
	void SetLotEnd(BOOL bLotEnd);

	//Critical section
	void SetCritical(BOOL bOn);
	BOOL GetCritical();

	void Recovery();

	//DB
	int Select_DB(std::deque<std::pair<int, CString>> &qData);
	int Insert_DB(int nTesterId, CString str2DBarcode);
	int Create_DB();
	int Delete_DB(int nTesterId, CString str2DBarcode);
	int Delete_DB_All_Data();
	int ChkTable();
	int CreateTable();
	int ConnectDatabase(const char* szDBPath);
	int Count_DB();
	void CloseDatabase();

	//FTP 
	BOOL ConnectFtp(CString strIP, CString strID, CString strPassword);
	void FtpClose();
	BOOL FtpSend(CString strLocalPath,CString strServerPath);
	int FTPCreateDirectory( LPCSTR pszDirectory, CFtpConnection *pFtp);

private:
	CCriticalSection m_cs;

	CWinThread*	 m_pThread;
	CppSQLite3DB m_db;
	CLogger      m_Logger;

	BOOL m_bRunStop;
	BOOL m_bLotEnd;
	BOOL m_bCritical;
	int  m_nFtpType;

	CString m_strMacPath;
	CString m_strNetPath;
	CString m_strLotNo;
	CString m_strFtpIP;
	CString m_strFtpID;
	CString m_strFtpPW;

	std::deque<std::pair<int, CString>> m_qData;
	std::vector<std::pair<CString,std::pair<CString,CString>>> m_vFolderName;   //First: FolderName , second.first: Mac Path, second.second: Server Path
	std::vector<std::pair<int,std::pair<CString, CString>>> m_vPathData;        //First: TesterID, second.first: Mac Path, second,second: Server Path

	static UINT ThreadProc(LPVOID lpVoid);
	FN_CALLBACK m_fnCallback;
};

