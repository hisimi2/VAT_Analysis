#include "StdAfx.h"
#include "NetDriver.h"

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <Winnetwk.h>

CNetDriver::CNetDriver(void)
{
	m_pMainFrameWnd = NULL;
}


CNetDriver::~CNetDriver(void)
{
}

int CNetDriver::Initialize(CWnd* pMainFrameWnd)
{
	m_pMainFrameWnd = pMainFrameWnd;
	return 0;
}

int CNetDriver::CopyFileFromMac(int nFileType/*enum eFileTransType{*/, int nPressIdx, CString strLotStartTm/*FM:2016-06-16*/, CString strLotEndTm/*FM:2016-06-16*/)
{
	CFileStatus cfStatus;	
	CFileFind   cfFinder;

	// get path
	char szFilePath[256] = {0,};
	g_DMCont.m_dmEQP.GS(SDM0_FTP_MAC_SITE1+nPressIdx, szFilePath, sizeof(szFilePath));

	CString strDriveName = _T("");
	strDriveName.Format("%c:", szFilePath[0]);

	// checking network driver
	char szRemoteName[256] = {0,};
	int  nRemoteNameLen = 256;
	DWORD dRetErr = WNetGetConnection(strDriveName, szRemoteName, (LPDWORD)&nRemoteNameLen);

	if(dRetErr != NO_ERROR){
		AfxMessageBox("not connection!");
		return -1;
	}

	// check file and not exist folder path is make folder
	CString strDestPath = _T("");
	if(nFileType == eFileTransType_CSV){
		strDestPath.Format("%s", MAC_FILE_COPY_PATH_CSV);
	}else{
		strDestPath.Format("%s", MAC_FILE_COPY_PATH_LOG);
	}
	if(!CFile::GetStatus(strDestPath, cfStatus)){
		CreateDirectory(strDestPath, NULL);
	}


	// find tester ID
	int nIdx = g_AppleTestIF.GetStationID(nPressIdx+1);
	CString strIdx = _T("");
	strIdx.Format("[%d]", nIdx);

	CString strSubFile = _T("");
	CString strFindFile = _T("");
	
	// cut need path
	CString strUserPutPath = szFilePath;
	strUserPutPath.Replace("/", "\\");
	int retval= 0;
	retval = strUserPutPath.Find(':');
	strUserPutPath = strUserPutPath.Mid(retval+2); 

	// make full path
	strSubFile += szRemoteName;
	strSubFile += "\\";
	strSubFile += strUserPutPath;

	// Package name
	char szPkgInfo[512] = {0,};    
	g_DMCont.m_dmHandlerOpt.GS(SDM11_LotNo   ,szPkgInfo,sizeof(szPkgInfo));
	strSubFile += "\\";
	strSubFile += szPkgInfo;

	// .log file은 1 Depth가 더 있음.
	if(nFileType == eFileTransType_LOG){
		strSubFile += "\\TestLog";
	}

	strSubFile += "\\*.*";

	// check file
	BOOL bDownLoad = cfFinder.FindFile(strSubFile);
	double dStartTm = (double)GetTickCount64();
	while (bDownLoad)
	{
		double dSpanTm = (double)GetTickCount64() - dStartTm;

		if( dSpanTm >= 600000) // checking 600sec
		{
			AfxMessageBox(" file down load time over!!!!");
			break;
		}
		bDownLoad   = cfFinder.FindNextFile(); // set file
		strFindFile = cfFinder.GetFileName();  // get file name

		if(strFindFile.Compare(_T(".")) == 0 ||
			strFindFile.Compare(_T("..")) == 0 ||
			strFindFile.Compare(_T("Thumbs.db")) == 0) continue;

		CString strFileType = _T("");
		if(nFileType == eFileTransType_CSV){
			strFileType = _T(".csv");
		}else{
			strFileType = _T(".log");
		}

		// checking naming ( Tester ID && file type && (LotStarTime or LotEndTime import naming)  )
		if(strFindFile.Find(strIdx) >= 0 && strFindFile.Find(strFileType) >= 0 && (strFindFile.Find(strLotStartTm) >= 0 || strFindFile.Find(strLotEndTm) >= 0) ){
			CString strSource = _T(""); // Mac path
			CString strNewName = _T("");

			// .log file은 1 Depth가 더 있음.
			if(nFileType == eFileTransType_LOG){
				strSource.Format(_T("%s\\%s\\TestLog\\%s"), szFilePath, szPkgInfo, (LPCTSTR)strFindFile);
				strNewName = strFindFile;
				strNewName.Replace(".log", "");
			}else{
				strSource.Format(_T("%s\\%s\\%s"), szFilePath, szPkgInfo, (LPCTSTR)strFindFile);
				CString strRemoveName = _T("");
				strRemoveName = strFindFile;
				strRemoveName .Replace(".csv", "");

				char szScheduleNo[256] = {0,};
				g_DMCont.m_dmHandlerOpt.GS(SDM11_ScheduleNo, szScheduleNo, sizeof(szScheduleNo));

				SYSTEMTIME time;
				::GetLocalTime( &time );
				strNewName.Format(_T("%s_%s_%04d%02d%02d%02d%02d%02d"), (LPCTSTR)strRemoveName, szScheduleNo, time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
			}
			

			CString strDest = _T("");   // handler path
			strDest.Format(_T("%s\\%s%s"), (LPCTSTR)strDestPath, (LPCTSTR)strNewName, (LPCTSTR)strFileType);
			CopyFile(strSource, strDest, FALSE);
			//Sleep(10);
			dStartTm = (double)GetTickCount64();
		}
		Sleep(1);
	}

	cfFinder.Close();

	return ERR_NO_ERROR;
}

int CNetDriver::MakeZipMacFile(int nPressIdx)
{
	CFileStatus cfStatus;	
	CFileFind   cfFinder;

	// check file and not exist folder path is make folder
	CString strSourcePath = _T("");
	strSourcePath.Format("%s", MAC_FILE_COPY_PATH_LOG);

	if(!CFile::GetStatus(strSourcePath, cfStatus)){
		CreateDirectory(strSourcePath, NULL);
	}

	if(!CFile::GetStatus(MAC_FILE_UPDATE_TMP_PATH, cfStatus)){
		CreateDirectory(MAC_FILE_UPDATE_TMP_PATH, NULL);
	}

	// find tester ID
	int nIdx = g_AppleTestIF.GetStationID(nPressIdx+1);
	CString strIdx = _T("");
	strIdx.Format("[%d]", nIdx);

	CString strSrc = _T("");
	strSrc += strSourcePath;
	strSrc += "\\*.*";

	char szEqpName[64] = {0,};
	g_DMCont.m_dmEQP.GS(SDM0_HANDER_ID, szEqpName, sizeof(szEqpName));

	char szProcessCode[64] = {0,};    
	g_DMCont.m_dmHandlerOpt.GS(SDM11_ScheduleNo, szProcessCode, sizeof(szProcessCode));

	BOOL bDownLoad = cfFinder.FindFile(strSrc);
	char szDestPath[512] = {0,};
	sprintf_s(szDestPath, sizeof(szDestPath), "%s\\TestLog_%s_%s_%s.zip", MAC_FILE_UPDATE_TMP_PATH, szEqpName, szProcessCode, strIdx.GetString() ); //imsi mac netdrive zip naming
	
	HZIP hz = CreateZip(szDestPath, NULL);

	double dStartTm = (double)GetTickCount64();
	CString strFindFile = _T("");
	while (bDownLoad)
	{
		double dSpanTm = (double)GetTickCount64() - dStartTm;

		if( dSpanTm >= 600000) // checking 600sec
		{
			AfxMessageBox("file zip time over!!!!");
			break;
		}
		bDownLoad   = cfFinder.FindNextFile(); // set file
		strFindFile = cfFinder.GetFileName();  // get file name

		if(strFindFile.Compare(_T(".")) == 0 ||
			strFindFile.Compare(_T("..")) == 0 ||
			strFindFile.Compare(_T("Thumbs.db")) == 0) continue;

		CString strFileType = _T(".log");

		// checking naming ( file type )
		if( strFindFile.Find(strFileType) >= 0 && strFindFile.Find(strIdx) >= 0 ){
			char szDestFileName[512]   = {0,};
			sprintf_s(szDestFileName, sizeof(szDestFileName), "%s", strFindFile.GetString() );
			char szSourceFileName[512] = {0,};
			sprintf_s(szSourceFileName, sizeof(szSourceFileName), "%s\\%s", strSourcePath.GetString(), strFindFile.GetString() );
			ZipAdd(hz, szDestFileName,  szSourceFileName);
			//Sleep(10);
			dStartTm = (double)GetTickCount64();
		}
		Sleep(1);
	}

	CloseZip(hz);

	DelFileFromMac(eFileTransType_LOG, strSourcePath);

	cfFinder.Close();

	//HWND hWnd = m_pMainFrameWnd->GetSafeHwnd();
	//CWnd* pWnd = CWnd::FromHandle(hWnd);
	//::SendMessage(hWnd, UM_ALARM_REPORT, (WPARAM), (LPARAM)&AlarmData);

	return ERR_NO_ERROR;
}

int CNetDriver::DelFileFromMac(int nFileType, CString szSrcPath)
{
	CFileStatus cfStatus;	
	CFileFind   cfFinder;

	CString strFileType = _T("");
	if(nFileType == eFileTransType_CSV){
		strFileType = _T(".csv");
	}else{
		strFileType = _T(".log");
	}

	CString strSrc = _T("");
	strSrc += szSrcPath;
	strSrc += "\\*.*";

	BOOL bDownLoad = cfFinder.FindFile(strSrc);

	CString strFindFile = _T("");
	while (bDownLoad)
	{
		bDownLoad   = cfFinder.FindNextFile(); // set file
		strFindFile = cfFinder.GetFileName();  // get file name

		if(strFindFile.Compare(_T(".")) == 0 ||
			strFindFile.Compare(_T("..")) == 0 ||
			strFindFile.Compare(_T("Thumbs.db")) == 0) continue;

		CString strFileType = _T("");
		if(nFileType == eFileTransType_CSV){
			strFileType = _T(".csv");
		}else{
			strFileType = _T(".log");
		}

		// checking naming ( file type )
		if( strFindFile.Find(strFileType) >= 0 ){
			char szSourceFileName[512] = {0,};
			sprintf_s(szSourceFileName, sizeof(szSourceFileName), "%s\\%s", szSrcPath.GetString(), strFindFile.GetString() );
			CFile::Remove(szSourceFileName);
			Sleep(10);
		}
		Sleep(1);
	}

	cfFinder.Close();

	return ERR_NO_ERROR;
}