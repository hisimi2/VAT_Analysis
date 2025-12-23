#include "StdAfx.h"
#include "ProcJamRate.h"

#include <fstream>
#include <sstream>
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const int DEFAULT_VEC_CAPA = 40000;

CProcJamRate::CProcJamRate(void)
{
	m_vOnlyOneDvc.clear();
	m_vOnlyOneDvc.reserve(DEFAULT_VEC_CAPA);
	m_vFirstData.reserve(DEFAULT_VEC_CAPA);
	m_vSecondData.reserve(DEFAULT_VEC_CAPA);

	m_nInputJamCnt = 0;
	m_nOutputJamCnt = 0;
}


CProcJamRate::~CProcJamRate(void)
{
}

BOOL CProcJamRate::m_fnCreateDirectory( LPCSTR pszDirectory )
{
	CFileStatus fs;
	if ( CFile::GetStatus( pszDirectory, fs ) )
	{
		if ( (fs.m_attribute & FILE_ATTRIBUTE_DIRECTORY) != 0 )
		{
			return TRUE;
		}
	}

	CHAR szTemp[MAX_PATH];
	LPSTR pszFilename;
	if ( !GetFullPathName( pszDirectory, sizeof( szTemp ), szTemp, &pszFilename ) )
	{
		return FALSE;
	}

	if ( pszFilename == NULL )
	{
		szTemp[lstrlen( szTemp ) - 1] = '\0';
		if( szTemp[lstrlen( szTemp ) - 1] == ':' )
			return TRUE;
		if( szTemp[lstrlen( szTemp ) - 2] == '\\' )
			return TRUE;
		return m_fnCreateDirectory( szTemp );
	}

	if ( pszFilename >= &szTemp[2] )
	{
		if ( pszFilename[-2] == ':' )
		{
			return CreateDirectory( szTemp, NULL );
		}
	}

	pszFilename[-1] = '\0';

	if ( !CFile::GetStatus( szTemp, fs ) )
	{
		if ( !m_fnCreateDirectory( szTemp ) )
		{
			return FALSE;
		}
		pszFilename[-1] = '\\';
		return CreateDirectory( szTemp, NULL );
	}

	if ( (fs.m_attribute & FILE_ATTRIBUTE_DIRECTORY) != 0 )
	{
		pszFilename[-1] = '\\';
		return CreateDirectory( szTemp, NULL );
	}

	return FALSE;
}


int CProcJamRate::SaveLotJamRate_Summary()
{
	int nErr = NO_ERROR;
	int nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);


	nErr = SaveLotJamRate_Amkor_TWSL421();


	if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_BatchFile_Use) == DEF_ON) {
		char szBatchFilePath[MAX_PATH] = { 0, };
		g_DMCont.m_dmHandlerOpt.GS(SDM11_BatchFile_Path, szBatchFilePath, sizeof(szBatchFilePath));

		char szBatPath[MAX_PATH] = { 0, };

		sprintf_s(szBatPath, sizeof(szBatPath), "%s", szBatchFilePath);
		int nErr = (int)ShellExecute(NULL, "open", szBatPath, NULL, NULL, SW_SHOWNORMAL);
		if (nErr <= 32) { // ShellExecute return 값 32이하 면 Error
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Failed to execute Batch File. = %s", szBatchFilePath);
			g_Error.AlarmReport(ERR_TESTER_BATCH_FILE_NOT_EXIST, AlarmData, NULL);
		}
	}

	SaveLotHistory();
	//nErr = SaveLotJamRate_Amkor();
	
	return nErr;
}

int CProcJamRate::SaveAmkorLotSummaryFileCreate()
{
	CFileFind cFileFind;
	CString strTargetPath = _T(""), strTargetFullPath = _T("");
	FILE *fp = NULL;

	// make data string
	CString strTmpData = _T("");
	CString strLotData = _T("");
	CString strDatePath = _T("");
	CString strFileName = _T("");

	SYSTEMTIME time;
	::GetLocalTime(&time);



	char szLotName[512] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_LotNo, szLotName, sizeof(szLotName));

	int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	
	strTargetPath.Format("D:\\Amkor_Lot_Summary\\%04d\\%02d", time.wYear, time.wMonth);
	strTargetFullPath.Format("%s\\%s_%04d%02d%02d_%02d%02d%02d.csv", strTargetPath, szLotName, time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	strFileName.Format("\\%s_%04d%02d%02d_%02d%02d%02d.csv", szLotName, time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	strDatePath.Format(_T("\\%04d\\%02d"), time.wYear, time.wMonth);

	//Folder check&Create
	m_fnCreateDirectory(strTargetPath);

	//file Open
	CStdioFile fileLocal;

	if (fileLocal.Open(strTargetFullPath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText)) {
		fileLocal.SeekToEnd();
	}
	else
	{
		return 1;
	}
	//1행
	strTmpData.Format("[Station Each]\n");
	strLotData += strTmpData;

	//2행
	strTmpData.Format(",");
	strLotData += strTmpData;
	for (int i = 0; i < eMaxPressUnitCount; i++)
	{
		strTmpData.Format("Station%d,", i+1);
		strLotData += strTmpData;
	}
	strTmpData.Format("\n");
	strLotData += strTmpData;

	//3행
	strTmpData.Format("Index Avg,");
	strLotData += strTmpData;
	for (int i = 0; i < eMaxPressUnitCount; i++)
	{
		double dAvgIndexTimeEach = g_DMCont.m_dmContac_Dn.GD(DDM1_SUMMARY_INDEX_STATION1 + i);
		
		if (dAvgIndexTimeEach == 0)
		{
			int nSiteOnOff = 0;
			int nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
			int nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

			//site 전체 Off시 Off로 표기 
			for (int j = 0; j < (nTestSite_Div_X*nTestSite_Div_Y); j++)
			{
				//GRR은 별도 확인
				if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
				{
					if (g_DMCont.m_dmHandlerOpt.GN(NDM11_GRR_SocketOnOff_Site1_1 + j + (i*STATION_MAX_PARA)))
					{
						nSiteOnOff = g_DMCont.m_dmHandlerOpt.GN(NDM11_GRR_SocketOnOff_Site1_1 + j + (i*STATION_MAX_PARA));
					}
				}
				else
				{
					if (g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + j + (i*STATION_MAX_PARA)))
					{
						nSiteOnOff = g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + j + (i*STATION_MAX_PARA));
						break;
					}
				}
			}

			if (nSiteOnOff == DEF_SITE_ON)
			{
				//3개 미만 측정 시 IndexTime 정확하지 않기 때문에 Calculation으로 표시
				strTmpData.Format("Calculation,");
				strLotData += strTmpData;
			} 
			else
			{
				//Site가 모두 Off시 Site OFF으로 표시
				strTmpData.Format("Station OFF,");
				strLotData += strTmpData;
			}
			
		} 
		else
		{
			strTmpData.Format("%.2f,", dAvgIndexTimeEach);
			strLotData += strTmpData;
		}
	}
	strTmpData.Format("\n");
	strLotData += strTmpData;

	//4행,5행
	strTmpData.Format("Test Time Avg,");
	strLotData += strTmpData;
	for (int i = 0; i < eMaxPressUnitCount; i++)
	{
		double dAvgTestTimeEach = g_DMCont.m_dmContac_Up.GN(NDM2_TEST_TIME_ACC_TIME_AVG_STATION1 + i);

		if (dAvgTestTimeEach == 0)
		{
			int nSiteOnOff = 0;
			int nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
			int nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

			//site 전체 Off시 Off로 표기 
			for (int j = 0; j < (nTestSite_Div_X*nTestSite_Div_Y); j++)
			{
				//GRR은 별도 확인
				if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
				{
					if (g_DMCont.m_dmHandlerOpt.GN(NDM11_GRR_SocketOnOff_Site1_1 + j + (i*STATION_MAX_PARA)))
					{
						nSiteOnOff = g_DMCont.m_dmHandlerOpt.GN(NDM11_GRR_SocketOnOff_Site1_1 + j + (i*STATION_MAX_PARA));
					}
				}
				else
				{
					if (g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + j + (i*STATION_MAX_PARA)))
					{
						nSiteOnOff = g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + j + (i*STATION_MAX_PARA));
					}
				}
			}

			if (nSiteOnOff == DEF_SITE_ON)
			{
				//3개 미만 측정 시 IndexTime 정확하지 않기 때문에 Calculation으로 표시
				strTmpData.Format("Calculation,");
				strLotData += strTmpData;
			}
			else
			{
				//Site가 모두 Off시 Site OFF으로 표시
				strTmpData.Format("Station OFF,");
				strLotData += strTmpData;
			}
		}
		else
		{
			strTmpData.Format("%.2f,", dAvgTestTimeEach / 1000);
			strLotData += strTmpData;
		}
	}
	strTmpData.Format("\n");
	strLotData += strTmpData;
	strTmpData.Format("\n");
	strLotData += strTmpData;

	//6행
	strTmpData.Format("[Station Total]\n");
	strLotData += strTmpData;

	//7행,8행,9행
	double dSummaryIndexTime = 0;
	int nSummaryTestTime = 0;

	int nUseIndexCnt = 0;
	int nUseTestCnt = 0;

	for (int i = 0; i < eMaxPressUnitCount; i++)
	{
		double dIndexTime = g_DMCont.m_dmContac_Dn.GD(DDM1_SUMMARY_INDEX_STATION1 + i);
		int nTestTime = g_DMCont.m_dmContac_Up.GN(NDM2_TEST_TIME_ACC_TIME_AVG_STATION1 + i);

		if (dIndexTime > 0)
		{
			nUseIndexCnt++;
			dSummaryIndexTime += dIndexTime;
		}
		if (nTestTime > 0)
		{
			nUseTestCnt++;
			nSummaryTestTime += nTestTime;
		}
	}

	if (nUseIndexCnt == 0)
	{
		strTmpData.Format("Index Avg,Calculation\n");
		strLotData += strTmpData;
	}
	else
	{
		strTmpData.Format("Index Avg,%.2f\n", dSummaryIndexTime / nUseIndexCnt);
		strLotData += strTmpData;
	}
	
	if (nUseTestCnt == 0)
	{
		strTmpData.Format("Test Avg,Calculation\n");
		strLotData += strTmpData;
	} 
	else
	{
		strTmpData.Format("Test Avg,%.2f\n", (double)nSummaryTestTime / 1000 / nUseTestCnt);
		strLotData += strTmpData;
	}
	
	strTmpData.Format("\n");
	strLotData += strTmpData;

	//10행
	strTmpData.Format("[Lot Total]\n");
	strLotData += strTmpData;

	//10행
	strTmpData.Format("Mode");
	strLotData += strTmpData;

	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);

	if (nRunMode == eRUN_MODE_OFFLINE)
	{
		strTmpData.Format(",Offline");
		strLotData += strTmpData;
	} 
	else
	{
		strTmpData.Format(",Online");
		strLotData += strTmpData;
	}

	

	switch (nLotStartMode)
	{
		case eStartMD_Initial:
		case eStartMD_InitContinue:
		{
			strTmpData.Format("/FT\n");
			strLotData += strTmpData;
		}break;
		case eStartMD_QA:
		case eStartMD_QA_Continue:
		{
			strTmpData.Format("/QA\n");
			strLotData += strTmpData;
		}break;
		case eStartMD_Retest:
		case eStartMD_RetestContinue:
		{
			strTmpData.Format("/Retest\n");
			strLotData += strTmpData;
		}break;
		case eStartMD_AutoGRR:
		case eStartMD_AutoGRR_Continue:
		{
			strTmpData.Format("/GRR\n");
			strLotData += strTmpData;
		}break;
		case eStartMD_AutoLoop:
		case eStartMD_AutoLoop_Continue:
		{
			strTmpData.Format("/Loop\n");
			strLotData += strTmpData;
		}break;
	}


	//11행
	int nUPH = g_DMCont.m_dmEQP.GN(NDM0_UPH);
	int nGrossUPH = g_DMCont.m_dmEQP.GN(NDM0_GROSS_UPH);
	int nRunTm = g_DMCont.m_dmEQP.GN(NDM0_LotRunTime);
	int nStopTm = g_DMCont.m_dmEQP.GN(NDM0_LotStopTime);
	int nCycleUPH = g_DMCont.m_dmEQP.GN(NDM0_CYCLE_UPH);
	int bCycleUPH = g_DMCont.m_dmEQP.GB(BDM0_CYCLE_UPH_CHECK);
	int nJamCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotJamCount);
	int nCurLoadCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt);
	int nCurPassCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotPassBinCnt);
	int nCurFailCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotFailBinCnt);
// 	CString sCurPassRate = _T("");
// 	CString sCurFailRate = _T("");

// 	sCurPassRate.Format("%3.2f", (double)g_DMCont.m_dmEQP.GN(NDM0_CurLotPassBinCnt) / (double)g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt)*100.0);
// 	if (g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt) == 0) {
// 		sCurPassRate.Format("0%");
// 	}
// 	else {
// 		sCurPassRate += "%";
// 	}

// 	sCurFailRate.Format("%3.2f", (double)g_DMCont.m_dmEQP.GN(NDM0_CurLotFailBinCnt) / (double)g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt)*100.0);
// 	if (g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt) == 0) {
// 		sCurFailRate.Format("0%");
// 	}
// 	else {
// 		sCurFailRate += "%";
// 	}

	strTmpData.Format("UPH,%d\n", nUPH);
	strLotData += strTmpData;
	strTmpData.Format("Gross UPH,%d\n", nGrossUPH);
	strLotData += strTmpData;

	// Lot run time
	int nHour = nRunTm / 3600;
	int nMin = (nRunTm - (nHour * 3600)) / 60;
	int nSec = (nRunTm - (nHour * 3600) - (nMin * 60));
	strTmpData.Format("Lot Time,%d:%02d:%02d\n", nHour, nMin, nSec);
	strLotData += strTmpData;

	// lot stop time
	nHour = nStopTm / 3600;
	nMin = (nStopTm - (nHour * 3600)) / 60;
	nSec = (nStopTm - (nHour * 3600) - (nMin * 60));
	strTmpData.Format("Stop Time,%d:%02d:%02d\n", nHour, nMin, nSec);
	strLotData += strTmpData;

	if (bCycleUPH == TRUE && nCycleUPH > 0)
	{
		strTmpData.Format("Cycle UPH,%d\n", nCycleUPH);
	}
	else {
		strTmpData.Format("Cycle UPH,Calculation\n");
	}
	strLotData += strTmpData;
	strTmpData.Format("Jamrate,%d\n", nJamCnt);
	strLotData += strTmpData;
	strTmpData.Format("Load,%d\n", nCurLoadCnt);
	strLotData += strTmpData;
	strTmpData.Format("Pass,%d\n", nCurPassCnt);
	strLotData += strTmpData;
	strTmpData.Format("Fail,%d\n", nCurFailCnt);
	strLotData += strTmpData;

	fileLocal.WriteString(strLotData);
	fileLocal.Close();


	// File Local -> Server로 복사
	char szFilePath[MAX_PATH] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Hit_Summary_File_Path, szFilePath, MAX_PATH);

	int nSummaryInterlock = g_TaskSystemCtrl.SummaryNetworkDriveInterlockCheck(szFilePath);

	if (nSummaryInterlock != ERR_NO_ERROR) {
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Hit Summary Network Driver disconnect!!. Please check Summary Path = [%s]", szFilePath);
		g_Error.AlarmReport(nSummaryInterlock, AlarmData, NULL);
		return nSummaryInterlock;
	}

	CString strPath = szFilePath;
	CString strNewFilePath, strMsg;
	strPath += strDatePath;
	m_fnCreateDirectory(strPath);
	strNewFilePath.Format("%s%s", strPath, strFileName);
	strMsg.Format(_T("[HIT Summary] strTargetFullPath : %s || strNewFilePath : %s"), strTargetFullPath, strNewFilePath);
	TWLOG.MakeLog(static_cast<int>(eLogID_SYSTEM), Debug, "", __LINE__, NULL, strMsg);

	//Local Drive에 복사 할 파일이 있는지 체크 한다. 1초 동안 확인 했는대 없으면 그냥 넘어 간다.
	double dTimeCheck = (double)GetTickCount64();
	double dSpanTime;
	BOOL bWorking = FALSE;
	CFileFind finder;
	do
	{
		dSpanTime = (double)GetTickCount64() - dTimeCheck;
		bWorking = finder.FindFile(strTargetFullPath);
		if (bWorking)
			break;

		Sleep(10);

	} while (dSpanTime < 1 * 1000.0); //1sec

	finder.Close();

	CopyFile(strTargetFullPath, strNewFilePath, FALSE);


	return NO_ERROR;
}

/*=============================================================================
AUTHOR   : 손정일
NAME     : 
PARAMS   : 
RETURN   :  
COMMENTS : 현재 진행중인 Lot의 정보를 Summary 한다.(Amkor)
==============================================================================*/
int CProcJamRate::SaveLotJamRate_Amkor()
{
	std::vector<ST_PRODUCTION_DATA> vProductionData;
	// make folder
	char szPathBuff[MAX_PATH]={0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_SummaryFile_Path,szPathBuff,MAX_PATH);

	CString strPath = szPathBuff;
	CString strDatePath = _T("");
	
	CString strLocalPath = SZ_DIR_PATH_SUMMARY;
	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
	SYSTEMTIME time;
	::GetLocalTime( &time );

	int nMonth = time.wMonth;
	CString strMonth =_T("");
	if(nMonth >=10){
		strMonth.Format("%02d",time.wMonth);
	}else{
		strMonth.Format("%01d",time.wMonth);
	}
	strDatePath.Format(_T("\\%04d\\%s"),time.wYear, (LPCTSTR)strMonth);
	
	if(nRunMode == eRUN_MODE_ONLINE){
		strPath += strDatePath;
		m_fnCreateDirectory(strPath);
	}

	strLocalPath += strDatePath;



	m_fnCreateDirectory(strLocalPath);

	// GetData :: Lot Start/End Time
	char szTmStart[128] = {0,};
	g_DMCont.m_dmEQP.GS(SDM0_CurLotStartTm, szTmStart, sizeof(szTmStart));
	CString strTmLotEnd = _T("");
	strTmLotEnd.Format("%04d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	// make data string
	CString strJamData = _T("");
	CString strTmpData = _T("");

	// Get Need data
	char szProcessCode[64] = {0,};    
	g_DMCont.m_dmHandlerOpt.GS(SDM11_ScheduleNo, szProcessCode, sizeof(szProcessCode));
	char szLotID[64] = {0,}; 
	g_DMCont.m_dmHandlerOpt.GS(SDM11_LotNo,szLotID,sizeof(szLotID));
	char szCustomer[64] = {0,}; 
	g_DMCont.m_dmHandlerOpt.GS(SDM11_CustomerName,szCustomer,sizeof(szCustomer));
	char szDeviceName[64] = {0,}; 
	g_DMCont.m_dmHandlerOpt.GS(SDM11_DeviceName,szDeviceName,sizeof(szDeviceName));
	char szEqpName[64] = {0,};
	g_DMCont.m_dmEQP.GS(SDM0_HANDER_ID, szEqpName, sizeof(szEqpName));
	char szOperID[64] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_OperatorID,szOperID,sizeof(szOperID));
	char szProgramID[512]  = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_ProgramName    , szProgramID    , sizeof(szProgramID));
	char szFWID[512]       = {0,};
	g_DMCont.m_dmEQP.GS(SDM0_FWID, szFWID, sizeof(szFWID));		
	char szStringFWID[128] = {0,};
	g_DMCont.m_dmEQP.GS(SDM0_StaringFWID, szStringFWID, sizeof(szStringFWID));
	char szOperation[64] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Operation, szOperation, sizeof(szOperation));
	char szTemp[64] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Temp, szTemp, sizeof(szTemp));
	char szRetestCode[64] ={0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_RetestCode, szRetestCode, sizeof(szRetestCode));
	char szLotSize[64] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_QTY, szLotSize, sizeof(szLotSize));
	char szART[64] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_ART, szART, sizeof(szART));

	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	char szLotMode[64] = { 0, };
	
	int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
		strcpy_s(szProcessCode, "GRR");
	else if (nLotStartMode == eStartMD_AutoLoop || nLotStartMode == eStartMD_AutoLoop_Continue)
		strcpy_s(szProcessCode, "LOOP");
	else if (nLotStartMode == eStartMD_QA || nLotStartMode == eStartMD_QA_Continue)
		strcpy_s(szProcessCode, "QA");
	
	
	int nGoodBinCnt[eMaxPressUnitCount] = {0,};
	int nSiteTestCnt[eMaxPressUnitCount]= {0,};

	int nAutoRetestKind = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoRetest);
	BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
	int nRetestCount = 1;
	if(bAutoRetestUseMode == TRUE){
		switch (nAutoRetestKind)
		{
		case eAutoRetest_Aonly : {nRetestCount = 1;} break;
		case eAutoRetest_AA:
		case eAutoRetest_AB:
			{
				nRetestCount = 2;
			}
			break;
		case  eAutoRetest_AAB: {nRetestCount = 3;} break;
		}
	}

	CString strFileName = _T("");
	char szLotName[512] = { 0, };
  	g_DMCont.m_dmHandlerOpt.GS(SDM11_LotNo, szLotName, sizeof(szLotName));
	strFileName.Format("\\%s_%s_%s_%s_%04d%02d%02d_%02d%02d%02d_SUMMARY.csv",szEqpName,szDeviceName,szLotName,szProcessCode,time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	strPath += strFileName;
	strLocalPath += strFileName;
	//// file open
	CStdioFile file, fileLocal;
	BOOL bIsFileErr = TRUE, bIsLocalFileErr = TRUE;

	if(nRunMode == eRUN_MODE_ONLINE){
		if(file.Open( strPath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText )){
			file.SeekToEnd();
			bIsFileErr = FALSE;
		}else{
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Summary Network Driver Create Fail. Please check Local Summary Path = %s", strFileName);
			g_Error.AlarmReport(ERR_SUMMARY_NETWORK_DRIVER_NOT_CONNECTED, AlarmData, NULL);
			return ERR_SUMMARY_NETWORK_DRIVER_NOT_CONNECTED;
		}
	}

	if(fileLocal.Open( strLocalPath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText )){
		bIsLocalFileErr = FALSE;
		fileLocal.SeekToEnd();
	}

	if(bIsFileErr == TRUE && bIsLocalFileErr == TRUE){
		return ERR_SUMMARY_NETWORK_DRIVER_NOT_CONNECTED;
	}

	strTmpData.Format("Summary Header\n");
	strJamData += strTmpData;
	strTmpData.Format("LotID,%s\n",szLotID);
	strJamData += strTmpData;
	strTmpData.Format("Customer,%s\n",szCustomer);
	strJamData += strTmpData;
	strTmpData.Format("Device,%s\n",szDeviceName);
	strJamData += strTmpData;
	strTmpData.Format("ProgramName,%s\n",szProgramID);
	strJamData += strTmpData;
	strTmpData.Format("Process,%s\n",szProcessCode);
	strJamData += strTmpData;
	strTmpData.Format("ProcessID,%s\n",szOperation);
	strJamData += strTmpData;
	strTmpData.Format("Handler,%s\n",szEqpName);
	strJamData += strTmpData;
	strTmpData.Format("OperatorID,%s\n",szOperID);
	strJamData += strTmpData;
	strTmpData.Format("Temp,%s\n",szTemp);
	strJamData += strTmpData;
	strTmpData.Format("szRetestCode,%s\n",szRetestCode);
	strJamData += strTmpData;
	strTmpData.Format("StartTime,%s\n",szTmStart);
	strJamData += strTmpData;
	strTmpData.Format("EndTime,%s\n", (LPCTSTR)strTmLotEnd);
	strJamData += strTmpData;
	strTmpData.Format("LotSize,%s\n",szLotSize);
	strJamData += strTmpData;
	strTmpData.Format("FWID, %s\n",szFWID);
	strJamData += strTmpData;
	strTmpData.Format("StarlingFWID, %s\n",szStringFWID);
	strJamData += strTmpData;
	strTmpData.Format("ART,%s\n",szART);
	strJamData += strTmpData;

	int nUPH = g_DMCont.m_dmEQP.GN(NDM0_UPH);
	int nGrossUPH = g_DMCont.m_dmEQP.GN(NDM0_GROSS_UPH);

	strTmpData.Format("UPH,%d\n", nUPH);
	strJamData += strTmpData;
	strTmpData.Format("Gross UPH,%d\n", nGrossUPH);
	strJamData += strTmpData;

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
		int nUseIdxCnt = 0;
		double dSummaryIndex = 0;
		for (int nIndex = 0; nIndex < eMaxPressUnitCount; nIndex++)
		{
			double dIndexTime = g_DMCont.m_dmContac_Dn.GD(DDM1_SUMMARY_INDEX_STATION1 + nIndex);
			if (dIndexTime > 0)
			{
				dSummaryIndex += dIndexTime;
				nUseIdxCnt++;
			}
		}
		strTmpData.Format("Index Time,%.2f\n", (dSummaryIndex != 0 && nUseIdxCnt !=0 ? dSummaryIndex / (double)nUseIdxCnt : 0.0) );
		strJamData += strTmpData;
	}

	strJamData += "\n";

	strTmpData.Format("HBIN Summary\n");
	strJamData += strTmpData;

	strTmpData.Format("ART,IN,HB1,HB2,HB3,HB4,Yield\n");
	strJamData += strTmpData;

	BOOL bHwBinPF[4] = {FALSE,};
	
	CString strStationSiteYield =_T("");
	int SiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int SiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	for(int nInfoIdx = 0; nInfoIdx <nRetestCount; nInfoIdx ++)
	{
		int nJamRateData[eMaxJamRateData] = {0,};
		int nStationSiteYield[eMaxPressUnitCount][STATION_MAX_X_SITE][STATION_MAX_Y_SITE][eMaxJamRateData] = {0,};

		int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(/*NDM11_MD_FTRTQCContinue [Make Summary]*/NDM11_MD_LOT_MODE);
		switch(nLotStartMode)
		{
		case eStartMD_Initial:
		case eStartMD_InitContinue:
		case eStartMD_QA:
		case eStartMD_QA_Continue:
			{
				if(nInfoIdx == eJamRateCode_Normal){
					nJamRateData[eJamRateData_LoadCnt] = g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt);
					m_vFirstData = m_vOnlyOneDvc;
					SaveBackUpData(eJamRateDataType_First, m_vFirstData);				
				}
			}break;
		case eStartMD_Retest:
		case eStartMD_RetestContinue:
			{
				if(nInfoIdx == eJamRateCode_Normal){
					nJamRateData[eJamRateData_LoadCnt] = g_DMCont.m_dmEQP.GN(NDM0_CurLotRT_LoadingCnt);
					m_vSecondData = m_vOnlyOneDvc;
					SaveBackUpData(eJamRateDataType_Last, m_vSecondData);
				}
			}break;
		}

		for(int i=0; i < g_BinCategory.nIndexCount; i++){
			if( strcmp(g_BinCategory.aCateInfo[i].szHWBin, "1") == 0 ){
				bHwBinPF[0] = atoi(g_BinCategory.aCateInfo[i].szPassFail);
			}
			if( strcmp(g_BinCategory.aCateInfo[i].szHWBin, "2") == 0 ){
				bHwBinPF[1] = atoi(g_BinCategory.aCateInfo[i].szPassFail);
			}
			if( strcmp(g_BinCategory.aCateInfo[i].szHWBin, "3") == 0 ){
				bHwBinPF[2] = atoi(g_BinCategory.aCateInfo[i].szPassFail);
			}
			if( strcmp(g_BinCategory.aCateInfo[i].szHWBin, "4") == 0 ){
				bHwBinPF[3] = atoi(g_BinCategory.aCateInfo[i].szPassFail);
			}
		}

		for (std::vector<ST_REC_DATA>::iterator it = m_vOnlyOneDvc.begin() ; it != m_vOnlyOneDvc.end(); ++it){
			if( strcmp(it->stDvcInfo[nInfoIdx].szErrStr, "") != 0)
			{
				switch(it->stDvcInfo[nInfoIdx].nHwBin)
				{
				case 0: { nJamRateData[eJamRateData_Output1Cnt] += 1; } break;
				case 1: { nJamRateData[eJamRateData_Output2Cnt] += 1; } break;
				case 2: { nJamRateData[eJamRateData_Output3Cnt] += 1; } break;
				case 3: { nJamRateData[eJamRateData_Output4Cnt] += 1; } break;
				}
				if(it->stDvcInfo[nInfoIdx].nPF == eGoodBin){
					nJamRateData[eJamRateData_PassBinCnt] += 1;
					nGoodBinCnt[it->stDvcInfo[nInfoIdx].nSiteNo] += 1;
					nStationSiteYield[it->stDvcInfo[nInfoIdx].nSiteNo][it->stDvcInfo[nInfoIdx].cSubSiteNo.x][it->stDvcInfo[nInfoIdx].cSubSiteNo.y][eJamRateData_PassBinCnt] += 1;
				}else{
					nJamRateData[eJamRateData_FailBinCnt] += 1;
					nStationSiteYield[it->stDvcInfo[nInfoIdx].nSiteNo][it->stDvcInfo[nInfoIdx].cSubSiteNo.x][it->stDvcInfo[nInfoIdx].cSubSiteNo.y][eJamRateData_FailBinCnt] += 1;
				}
				
				nSiteTestCnt[it->stDvcInfo[nInfoIdx].nSiteNo] += 1;
				
				nStationSiteYield[it->stDvcInfo[nInfoIdx].nSiteNo][it->stDvcInfo[nInfoIdx].cSubSiteNo.x][it->stDvcInfo[nInfoIdx].cSubSiteNo.y][eJamRateData_LoadCnt] += 1;
			}
		}
		nJamRateData[eJamRateData_SortCnt] = nJamRateData[eJamRateData_Output1Cnt]+nJamRateData[eJamRateData_Output2Cnt]+nJamRateData[eJamRateData_Output3Cnt]+nJamRateData[eJamRateData_Output4Cnt];
		// Second or Third
		if( (nInfoIdx%3) != 0 ){
			nJamRateData[eJamRateData_LoadCnt] = nJamRateData[eJamRateData_PassBinCnt]+nJamRateData[eJamRateData_FailBinCnt];
		}

		ST_PRODUCTION_DATA stProductionData;
		sprintf_s(stProductionData.szLotTitle, sizeof(stProductionData.szLotTitle), "%d", nInfoIdx);
		stProductionData.nInCount = nJamRateData[eJamRateData_LoadCnt];
		stProductionData.nOutCount = nJamRateData[eJamRateData_PassBinCnt];
		stProductionData.nHwBinCount[0] = nJamRateData[eJamRateData_Output1Cnt];
		stProductionData.nHwBinCount[1] = nJamRateData[eJamRateData_Output2Cnt];
		stProductionData.nHwBinCount[2] = nJamRateData[eJamRateData_Output3Cnt];
		stProductionData.nHwBinCount[3] = nJamRateData[eJamRateData_Output4Cnt];
		stProductionData.dYield = ( (nJamRateData[eJamRateData_LoadCnt] > 0) ? ( (double)nJamRateData[eJamRateData_PassBinCnt]/((double)nJamRateData[eJamRateData_LoadCnt])*100.0 ) : 0);
		stProductionData.nInputJam  = (nInfoIdx==0?m_nInputJamCnt:0);
		stProductionData.nOutputJam = (nInfoIdx==0?m_nOutputJamCnt:0);

		vProductionData.push_back(stProductionData);

		double dSiteYield =0.0;
		
		strTmpData.Format("%d,,,,,",nInfoIdx);
		strStationSiteYield += strTmpData;

		for(int nStation = 0; nStation<eMaxPressUnitCount; nStation++){
			for( int nY =0; nY < SiteCntY; nY++ )
			{
				for( int nX =0; nX < SiteCntX; nX++ )
				{
					dSiteYield =( (nStationSiteYield[nStation][nX][nY][eJamRateData_LoadCnt] >0) ? ( (double)nStationSiteYield[nStation][nX][nY][eJamRateData_PassBinCnt] / (double)nStationSiteYield[nStation][nX][nY][eJamRateData_LoadCnt] * 100.0) : 0);
					strTmpData.Format("%.2f%%,",dSiteYield);
					strStationSiteYield += strTmpData;
				}
			}
		}
		strStationSiteYield += "\n";
	}
	

	ST_PRODUCTION_DATA stFinalData;
	// get last h/w bin data

	for(int i=0; i<4; i++){
		if(bHwBinPF[i] != eGoodBin){
			//FT Lot HW bin
			if(nAutoRetestKind == eAutoRetest_AAB && bAutoRetestUseMode==TRUE){
				stFinalData.nHwBinCount[i] = vProductionData[2].nHwBinCount[i];	// using R2
			}else if((nAutoRetestKind == eAutoRetest_AA || nAutoRetestKind == eAutoRetest_AB) && bAutoRetestUseMode==TRUE){
				stFinalData.nHwBinCount[i] = vProductionData[1].nHwBinCount[i];	// using R1
			}else{
				stFinalData.nHwBinCount[i] = vProductionData[0].nHwBinCount[i];	// using Normal
			}
		
		}
	}
	stFinalData.nInCount = g_DMCont.m_dmEQP.GN(NDM0_CurLotRT_LoadingCnt);

	for (std::vector<ST_PRODUCTION_DATA>::iterator it = vProductionData.begin() ; it != vProductionData.end(); ++it){
		strTmpData.Format("%s,%d,%d,%d,%d,%d,%.2f%%\n",
			it->szLotTitle, it->nInCount,			
			it->nHwBinCount[0], it->nHwBinCount[1], it->nHwBinCount[2], it->nHwBinCount[3],it->dYield);
		strJamData += strTmpData;

		// summury final data
		if( strcmp(it->szLotTitle, "0") == 0){
			stFinalData.nInCount = it->nInCount;
		}
		stFinalData.nOutCount += it->nOutCount;

		for(int i=0; i<4; i++){
			if(bHwBinPF[i] == eGoodBin){
				stFinalData.nHwBinCount[i] += it->nHwBinCount[i];
			}
		}
	}

	strTmpData.Format("Final,%d,%d,%d,%d,%d,%.2f%%\n\n",
		stFinalData.nInCount,
		g_DMCont.m_dmEQP.GN( NDM0_CurLotOutput1Cnt ), g_DMCont.m_dmEQP.GN( NDM0_CurLotOutput2Cnt ), g_DMCont.m_dmEQP.GN( NDM0_CurLotOutput3Cnt ), g_DMCont.m_dmEQP.GN( NDM0_CurLotOutput4Cnt ),
		//stFinalData.nHwBinCount[0],stFinalData.nHwBinCount[1],stFinalData.nHwBinCount[2],stFinalData.nHwBinCount[3],
		(stFinalData.nInCount > 0 ? ((double)stFinalData.nOutCount/(double)stFinalData.nInCount) *100.0 : 0) );
	strJamData += strTmpData;

	strTmpData.Format( "2D Read Error Count,%d\n", g_DMCont.m_dmEQP.GN( NDM0_CurLot2D_Read_Error_Cnt ) );
	strJamData += strTmpData;
	strJamData += "\n";

	switch(nAutoRetestKind)
	{
		case eAutoRetest_AA:
		case eAutoRetest_AB:
			{
				strJamData +="\n";
			}break;
		case eAutoRetest_Aonly:
			{
				strJamData += "\n\n";
			}break;
	}

	strTmpData.Format(_T("SBIN Summary\n"));
	strJamData += strTmpData;
	strTmpData.Format(_T("ART,PF,Error String,HB,Total,"));
	strJamData += strTmpData;

	strTmpData = _T( "" );
	for( int nStation = 0; nStation < eMaxPressUnitCount; nStation++ ) {
		strTmpData.Format( _T( "Station%d" ), nStation + 1 );
		for( int nY = 0; nY < STATION_MAX_Y_SITE; nY++ ) {
			for( int nX = 0; nX < STATION_MAX_X_SITE; nX++ ) {
				strTmpData += _T( "," );
			}
		}
		strJamData += strTmpData;
	}
	strJamData += _T("\n");
	
	
	CString strTmpTestID = _T(""),strTestID =_T("");
	for(int i=0; i<eMaxPressUnitCount; i++){
		BOOL bConnect = g_AppleTestIF.IsConnected(i+1);
		strTmpTestID.Empty();
		if (bConnect == TRUE) {
			CTesterStatus cTesterStatus = g_AppleTestIF.GetTesterStatus(i+1);
			strTmpTestID = cTesterStatus.m_sTesterID_Orginal;
		}
		else {
			strTmpTestID.Format(_T("-1"));
		}
		for( int nY = 0; nY < STATION_MAX_Y_SITE; nY++ ) {
			for( int nX = 0; nX < STATION_MAX_X_SITE; nX++ ) {
				strTmpTestID += _T( "," );
			}
		}
		strTestID += strTmpTestID;
	}
	strTestID += "\n";
	strJamData +=",,,,,";
	strJamData += strTestID;

	strTmpData.Format(",,,,,");
	CString strTmpIndex = _T( "" );
	for(int nStation=0; nStation<eMaxPressUnitCount; nStation++){
		int index_number = 1;
		for( int nY = 0; nY < STATION_MAX_Y_SITE; nY++ ) {
			for( int nX = 0; nX < STATION_MAX_X_SITE; nX++ ) {
				strTmpIndex.Format( _T("%d,"),index_number++ );
				strTmpData += strTmpIndex;
			}
		}
		
	}
	strJamData += strTmpData + _T("\n");

	strJamData += strStationSiteYield;
	strJamData += "\n";

	for(int nInfoIdx = 0; nInfoIdx< nRetestCount; nInfoIdx++)
	{
		std::vector<ST_DVC_DATA> vErrList;
		for (std::vector<ST_REC_DATA>::iterator it = m_vOnlyOneDvc.begin(); it != m_vOnlyOneDvc.end(); ++it){
			if( strcmp(it->stDvcInfo[nInfoIdx].szErrStr, "") == 0 /*&& strcmp(it->stDvcInfo[nInfoIdx].szErrCode, "") == 0*/){ continue; }

			BOOL bFindSame = FALSE;
			for (std::vector<ST_DVC_DATA>::iterator it2 = vErrList.begin(); it2 != vErrList.end(); ++it2){
				if( /*strcmp( it2->szErrCode, it->stDvcInfo[nInfoIdx].szErrCode) ==0 &&*/ strcmp( it2->szErrStr, it->stDvcInfo[nInfoIdx].szErrStr) ==0 ){
					bFindSame = TRUE;
				}
			}

			if(bFindSame ==FALSE){
				ST_DVC_DATA stCopyData;
				stCopyData = it->stDvcInfo[nInfoIdx];
				vErrList.push_back(stCopyData);
			}		
		}

		for(std::vector<ST_DVC_DATA>::iterator it2 = vErrList.begin(); it2 != vErrList.end(); ++it2)
		{
			int nSiteBinCount[eMaxPressUnitCount][STATION_MAX_X_SITE][STATION_MAX_Y_SITE] = {0,};
			CString strInfo = _T(""); // Error Code,PF,Error String,HB
			strTmpData.Empty();
			for (std::vector<ST_REC_DATA>::iterator it = m_vOnlyOneDvc.begin(); it != m_vOnlyOneDvc.end(); ++it){
				if( strcmp(it->stDvcInfo[nInfoIdx].szErrStr, it2->szErrStr)==0 /*&& strcmp(it->stDvcInfo[nInfoIdx].szErrCode, it2->szErrCode)==0*/ ){
					nSiteBinCount[it->stDvcInfo[nInfoIdx].nSiteNo][it->stDvcInfo[nInfoIdx].cSubSiteNo.x][it->stDvcInfo[nInfoIdx].cSubSiteNo.y]/*[cp.x + (cp.y * 2)]*/ += 1; //Site Count

					if(strInfo.IsEmpty() == TRUE){
						strInfo.Format("%d,%s,%s,%d", nInfoIdx,/* it2->szErrCode,*/ (it2->nPF==eGoodBin ? _T("P") : _T("F")), it2->szErrStr, it2->nHwBin+1);
					}
				}
			}

			// find some data
			if(strInfo.IsEmpty() != TRUE){ 
				int nSumCount=0;
				CString strData = _T(""); // Other Remain Data
				CString strUnitData =_T("");
				CString strTotalData = _T("");
				for(int i=ePressUnit_Down_1; i<eMaxPressUnitCount; i++){
					for( int nY =0; nY < SiteCntY; nY++ )
					{
						for( int nX =0; nX < SiteCntX; nX++ )
						{
							strData.Format(",%d", nSiteBinCount[i][nX][nY] );
							strUnitData += strData;

							nSumCount += nSiteBinCount[i][nX][nY]; // Total
						}
					}
				}
			
				strTotalData.Format(",%d",nSumCount);
				strTmpData += strInfo;
				strTmpData += strTotalData;
				strTmpData += strUnitData;
				strTmpData += "\n";
				strJamData += strTmpData;
			}
		}
	}
	//strJamData += _T("\nEND");

	if(bIsFileErr == FALSE){
		file.WriteString( strJamData );
		file.Close();
	}

	if(bIsLocalFileErr == FALSE){
		fileLocal.WriteString( strJamData );
		fileLocal.Close();
	}

	return ERR_NO_ERROR;
}

int CProcJamRate::SaveLotJamRate_Amkor_TWSL421()
{
	std::vector<ST_PRODUCTION_DATA> vProductionData;
	// make folder
	char szPathBuff[MAX_PATH] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_SummaryFile_Path, szPathBuff, MAX_PATH);

	CString strPath = szPathBuff;
	CString strDatePath = _T("");

	CString strLocalPath = SZ_DIR_PATH_SUMMARY;
	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
	SYSTEMTIME time;
	::GetLocalTime(&time);

	int nMonth = time.wMonth;
	CString strMonth = _T("");
	if (nMonth >= 10) {
		strMonth.Format("%02d", time.wMonth);
	}
	else {
		strMonth.Format("%01d", time.wMonth);
	}
	strDatePath.Format(_T("\\%04d\\%s"), time.wYear, (LPCTSTR)strMonth);

	if (nRunMode == eRUN_MODE_ONLINE) {
		strPath += strDatePath;
		m_fnCreateDirectory(strPath);
	}

	strLocalPath += strDatePath;



	m_fnCreateDirectory(strLocalPath);

	// GetData :: Lot Start/End Time
	char szTmStart[128] = { 0, };
	g_DMCont.m_dmEQP.GS(SDM0_CurLotStartTm, szTmStart, sizeof(szTmStart));
	CString strTmLotEnd = _T("");
	strTmLotEnd.Format("%04d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	// make data string
	CString strJamData = _T("");
	CString strTmpData = _T("");

	// Get Need data
	char szProcessCode[64] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_ScheduleNo, szProcessCode, sizeof(szProcessCode));
	char szLotID[64] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_LotNo, szLotID, sizeof(szLotID));
	char szCustomer[64] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_CustomerName, szCustomer, sizeof(szCustomer));
	char szDeviceName[64] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_DeviceName, szDeviceName, sizeof(szDeviceName));
	char szEqpName[64] = { 0, };
	g_DMCont.m_dmEQP.GS(SDM0_HANDER_ID, szEqpName, sizeof(szEqpName));
	char szOperID[64] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_OperatorID, szOperID, sizeof(szOperID));
	char szProgramID[512] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_ProgramName, szProgramID, sizeof(szProgramID));
	char szProgramInfo[512] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM0_TESTER_INFOR_PROGRAM, szProgramInfo, sizeof(szProgramInfo));
	
	char szFWID[512] = { 0, };
	g_DMCont.m_dmEQP.GS(SDM0_FWID, szFWID, sizeof(szFWID));
	char szStringFWID[128] = { 0, };
	g_DMCont.m_dmEQP.GS(SDM0_StaringFWID, szStringFWID, sizeof(szStringFWID));
	char szOperation[64] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Operation, szOperation, sizeof(szOperation));
	char szTemp[64] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Temp, szTemp, sizeof(szTemp));
	char szRetestCode[64] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_RetestCode, szRetestCode, sizeof(szRetestCode));
	char szLotSize[64] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_QTY, szLotSize, sizeof(szLotSize));
	char szART[64] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_ART, szART, sizeof(szART));

	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	char szLotMode[64] = { 0, };
	BOOL bQAModeUse = FALSE;
	int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
		strcpy_s(szProcessCode, "GRR");
	else if (nLotStartMode == eStartMD_AutoLoop || nLotStartMode == eStartMD_AutoLoop_Continue)
		strcpy_s(szProcessCode, "LOOP");
	else if (nLotStartMode == eStartMD_QA || nLotStartMode == eStartMD_QA_Continue)
	{
		bQAModeUse = TRUE;
	}
		


	int nGoodBinCnt[eMaxPressUnitCount] = { 0, };
	int nSiteTestCnt[eMaxPressUnitCount] = { 0, };

	int nAutoRetestKind = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoRetest);
	BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
	int nRetestCount = 1;
	if (bAutoRetestUseMode == TRUE) {
		switch (nAutoRetestKind)
		{
		case eAutoRetest_Aonly: {nRetestCount = 1; } break;
		case eAutoRetest_AA:
		case eAutoRetest_AB:
		{
			nRetestCount = 2;
		}
		break;
		case  eAutoRetest_AAB: {nRetestCount = 3; } break;
		}
	}

	CString strFileName = _T("");
	char szLotName[512] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_LotNo, szLotName, sizeof(szLotName));
	strFileName.Format("\\%s_%s_%s_%s_%04d%02d%02d_%02d%02d%02d_SUMMARY.csv", szEqpName, szDeviceName, szLotName, szProcessCode, time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	strPath += strFileName;
	strLocalPath += strFileName;
	//// file open
	CStdioFile file, fileLocal;
	BOOL bIsFileErr = TRUE, bIsLocalFileErr = TRUE;

	if (nRunMode == eRUN_MODE_ONLINE) {
		if (file.Open(strPath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText)) {
			file.SeekToEnd();
			bIsFileErr = FALSE;
		}
		else {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Summary Network Driver Create Fail. Please check Local Summary Path = %s", strFileName);
			g_Error.AlarmReport(ERR_SUMMARY_NETWORK_DRIVER_NOT_CONNECTED, AlarmData, NULL);
			return ERR_SUMMARY_NETWORK_DRIVER_NOT_CONNECTED;
		}
	}

	if (fileLocal.Open(strLocalPath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText)) {
		bIsLocalFileErr = FALSE;
		fileLocal.SeekToEnd();
	}

	if (bIsFileErr == TRUE && bIsLocalFileErr == TRUE) {
		return ERR_SUMMARY_NETWORK_DRIVER_NOT_CONNECTED;
	}

	strTmpData.Format("Summary Header\n");
	strJamData += strTmpData;
	strTmpData.Format("LotID,%s\n", szLotID);
	strJamData += strTmpData;
	strTmpData.Format("Customer,%s\n", szCustomer);
	strJamData += strTmpData;
	strTmpData.Format("Device,%s\n", szDeviceName);
	strJamData += strTmpData;
	strTmpData.Format("ProgramName,%s\n", szProgramID);
	strJamData += strTmpData;
	strTmpData.Format("Rev,%s\n", szProgramInfo);
	strJamData += strTmpData;
	strTmpData.Format("Process,%s\n", szProcessCode);
	strJamData += strTmpData;
	strTmpData.Format("ProcessID,%s\n", szOperation);
	strJamData += strTmpData;
	strTmpData.Format("Handler,%s\n", szEqpName);
	strJamData += strTmpData;
	strTmpData.Format("OperatorID,%s\n", szOperID);
	strJamData += strTmpData;
	strTmpData.Format("Temp,%s\n", szTemp);
	strJamData += strTmpData;
	strTmpData.Format("RetestCode,%s\n", szProcessCode);
	strJamData += strTmpData;
	strTmpData.Format("StartTime,%s\n", szTmStart);
	strJamData += strTmpData;
	strTmpData.Format("EndTime,%s\n", (LPCTSTR)strTmLotEnd);
	strJamData += strTmpData;
	strTmpData.Format("LotSize,%s\n", szLotSize);
	strJamData += strTmpData;
	strTmpData.Format("FWID, %s\n", szFWID);
	strJamData += strTmpData;
	strTmpData.Format("StarlingFWID, %s\n", szStringFWID);
	strJamData += strTmpData;
	//strTmpData.Format("ART,%s\n", szART);
	strTmpData.Format("ART,AAB\n"/*, szART*/);
	strJamData += strTmpData;

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	strJamData += "\n";

	strTmpData.Format("HBIN Summary\n");
	strJamData += strTmpData;

// 	if (nProjectOption == DEF_PROJECT_WATT)
// 	{
// 		strTmpData.Format("ART,IN,HB1,HB3,HB4,HB7,Yield\n");
// 	}
//	else {
		strTmpData.Format("ART,IN,HB1,HB2,HB3,HB4,Yield\n");
//	}
	
	strJamData += strTmpData;

	BOOL bHwBinPF[4] = { FALSE, };

	CString strStationSiteYield = _T("");
	int SiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int SiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	for (int nInfoIdx = 0; nInfoIdx < nRetestCount; nInfoIdx++)
	{
		int nJamRateData[eMaxJamRateData] = { 0, };
		int nStationSiteYield[eMaxPressUnitCount][STATION_MAX_X_SITE][STATION_MAX_Y_SITE][eMaxJamRateData] = { 0, };

		int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(/*NDM11_MD_FTRTQCContinue [Make Summary]*/NDM11_MD_LOT_MODE);
		switch (nLotStartMode)
		{
		case eStartMD_Initial:
		case eStartMD_InitContinue:
		case eStartMD_QA:
		case eStartMD_QA_Continue:
		{
			if (nInfoIdx == eJamRateCode_Normal) {
				nJamRateData[eJamRateData_LoadCnt] = g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt);
				m_vFirstData = m_vOnlyOneDvc;
				SaveBackUpData(eJamRateDataType_First, m_vFirstData);
			}
		}break;
		case eStartMD_Retest:
		case eStartMD_RetestContinue:
		{
			if (nInfoIdx == eJamRateCode_Normal) {
				nJamRateData[eJamRateData_LoadCnt] = g_DMCont.m_dmEQP.GN(NDM0_CurLotRT_LoadingCnt);
				m_vSecondData = m_vOnlyOneDvc;
				SaveBackUpData(eJamRateDataType_Last, m_vSecondData);
			}
		}break;
		}

		for (int i = 0; i < g_BinCategory.nIndexCount; i++) {
			if (strcmp(g_BinCategory.aCateInfo[i].szHWBin, "1") == 0) {
				bHwBinPF[0] = atoi(g_BinCategory.aCateInfo[i].szPassFail);
			}
			if (strcmp(g_BinCategory.aCateInfo[i].szHWBin, "2") == 0) {
				bHwBinPF[1] = atoi(g_BinCategory.aCateInfo[i].szPassFail);
			}
			if (strcmp(g_BinCategory.aCateInfo[i].szHWBin, "3") == 0) {
				bHwBinPF[2] = atoi(g_BinCategory.aCateInfo[i].szPassFail);
			}
			if (strcmp(g_BinCategory.aCateInfo[i].szHWBin, "4") == 0) {
				bHwBinPF[3] = atoi(g_BinCategory.aCateInfo[i].szPassFail);
			}
		}

		for (std::vector<ST_REC_DATA>::iterator it = m_vOnlyOneDvc.begin(); it != m_vOnlyOneDvc.end(); ++it) {
			if (strcmp(it->stDvcInfo[nInfoIdx].szErrStr, "") != 0)
			{
				switch (it->stDvcInfo[nInfoIdx].nHwBin)
				{
				case 0: { nJamRateData[eJamRateData_Output1Cnt] += 1; } break;
				case 1: { nJamRateData[eJamRateData_Output2Cnt] += 1; } break;
				case 2: { nJamRateData[eJamRateData_Output3Cnt] += 1; } break;
				case 3: { nJamRateData[eJamRateData_Output4Cnt] += 1; } break;
				}
				if (it->stDvcInfo[nInfoIdx].nPF == eGoodBin) {
					nJamRateData[eJamRateData_PassBinCnt] += 1;
					nGoodBinCnt[it->stDvcInfo[nInfoIdx].nSiteNo] += 1;
					nStationSiteYield[it->stDvcInfo[nInfoIdx].nSiteNo][it->stDvcInfo[nInfoIdx].cSubSiteNo.x][it->stDvcInfo[nInfoIdx].cSubSiteNo.y][eJamRateData_PassBinCnt] += 1;
				}
				else {
					nJamRateData[eJamRateData_FailBinCnt] += 1;
					nStationSiteYield[it->stDvcInfo[nInfoIdx].nSiteNo][it->stDvcInfo[nInfoIdx].cSubSiteNo.x][it->stDvcInfo[nInfoIdx].cSubSiteNo.y][eJamRateData_FailBinCnt] += 1;
				}

				nSiteTestCnt[it->stDvcInfo[nInfoIdx].nSiteNo] += 1;

				nStationSiteYield[it->stDvcInfo[nInfoIdx].nSiteNo][it->stDvcInfo[nInfoIdx].cSubSiteNo.x][it->stDvcInfo[nInfoIdx].cSubSiteNo.y][eJamRateData_LoadCnt] += 1;
			}
		}
		nJamRateData[eJamRateData_SortCnt] = nJamRateData[eJamRateData_Output1Cnt] + nJamRateData[eJamRateData_Output2Cnt] + nJamRateData[eJamRateData_Output3Cnt] + nJamRateData[eJamRateData_Output4Cnt];
		// Second or Third
		if ((nInfoIdx % 3) != 0) {
			nJamRateData[eJamRateData_LoadCnt] = nJamRateData[eJamRateData_PassBinCnt] + nJamRateData[eJamRateData_FailBinCnt];
		}
	

		ST_PRODUCTION_DATA stProductionData;
		sprintf_s(stProductionData.szLotTitle, sizeof(stProductionData.szLotTitle), "%d", nInfoIdx);
		stProductionData.nInCount = nJamRateData[eJamRateData_LoadCnt];
		stProductionData.nOutCount = nJamRateData[eJamRateData_PassBinCnt];
		stProductionData.nHwBinCount[0] = nJamRateData[eJamRateData_Output1Cnt];
		stProductionData.nHwBinCount[1] = nJamRateData[eJamRateData_Output2Cnt];
		stProductionData.nHwBinCount[2] = nJamRateData[eJamRateData_Output3Cnt];
		stProductionData.nHwBinCount[3] = nJamRateData[eJamRateData_Output4Cnt];
		
		int nQAEmptyLoadCount = 0;
		if (bQAModeUse == TRUE && nInfoIdx == 0)
		{		
			nQAEmptyLoadCount = g_DMCont.m_dmEQP.GN(NDM0_CurLotQAEmptyLoadCnt);			
		}
		stProductionData.dYield = ((nJamRateData[eJamRateData_LoadCnt] > 0) ? ((double)nJamRateData[eJamRateData_PassBinCnt] / (double)((nJamRateData[eJamRateData_LoadCnt] - nQAEmptyLoadCount))*100.0) : 0);
		stProductionData.nInputJam = (nInfoIdx == 0 ? m_nInputJamCnt : 0);
		stProductionData.nOutputJam = (nInfoIdx == 0 ? m_nOutputJamCnt : 0);

		vProductionData.push_back(stProductionData);

		double dSiteYield = 0.0;

		strTmpData.Format("%d,,,,,", nInfoIdx);
		strStationSiteYield += strTmpData;

		for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++) {
			for (int nY = 0; nY < SiteCntY; nY++)
			{
				for (int nX = 0; nX < SiteCntX; nX++)
				{
					dSiteYield = ((nStationSiteYield[nStation][nX][nY][eJamRateData_LoadCnt] > 0) ? ((double)nStationSiteYield[nStation][nX][nY][eJamRateData_PassBinCnt] / (double)(nStationSiteYield[nStation][nX][nY][eJamRateData_LoadCnt])* 100.0) : 0);
					strTmpData.Format("%.2f%%,", dSiteYield);
					strStationSiteYield += strTmpData;
				}
			}
		}
		strStationSiteYield += "\n";
	}


	if (bQAModeUse == TRUE)
	{
		int nLotSize = atoi(szLotSize);
		vProductionData[0].nInCount = nLotSize + g_DMCont.m_dmEQP.GN(NDM0_QaSampleAddSumLoadCnt);

		int nCurQAEmptyCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotQAEmptyLoadCnt);
		int nCount = g_DMCont.m_dmEQP.GN(NDM0_CurLotOutput1Cnt);

		g_DMCont.m_dmEQP.SN(NDM0_CurLotOutput1Cnt, nCount - nCurQAEmptyCnt);
	}


	ST_PRODUCTION_DATA stFinalData;
	// get last h/w bin data

	for (int i = 0; i < 4; i++) {
		if (bHwBinPF[i] != eGoodBin) {
			//FT Lot HW bin
			if (nAutoRetestKind == eAutoRetest_AAB && bAutoRetestUseMode == TRUE) {
				stFinalData.nHwBinCount[i] = vProductionData[2].nHwBinCount[i];	// using R2
			}
			else if ((nAutoRetestKind == eAutoRetest_AA || nAutoRetestKind == eAutoRetest_AB) && bAutoRetestUseMode == TRUE) {
				stFinalData.nHwBinCount[i] = vProductionData[1].nHwBinCount[i];	// using R1
			}
			else {
				stFinalData.nHwBinCount[i] = vProductionData[0].nHwBinCount[i];	// using Normal
			}

		}
	}
	stFinalData.nInCount = g_DMCont.m_dmEQP.GN(NDM0_CurLotRT_LoadingCnt) + g_DMCont.m_dmEQP.GN(NDM0_QaSampleAddSumLoadCnt);

	for (std::vector<ST_PRODUCTION_DATA>::iterator it = vProductionData.begin(); it != vProductionData.end(); ++it) {
		strTmpData.Format("%s,%d,%d,%d,%d,%d,%.2f%%\n",
			it->szLotTitle, it->nInCount,
			it->nHwBinCount[0], it->nHwBinCount[1], it->nHwBinCount[2], it->nHwBinCount[3], it->dYield);
		strJamData += strTmpData;

		// summury final data
		if (strcmp(it->szLotTitle, "0") == 0) {
			stFinalData.nInCount = it->nInCount;
		}
		stFinalData.nOutCount += it->nOutCount;

		for (int i = 0; i < 4; i++) {
			if (bHwBinPF[i] == eGoodBin) {
				stFinalData.nHwBinCount[i] += it->nHwBinCount[i];
			}
		}
	}

	

	strTmpData.Format("Final,%d,%d,%d,%d,%d,%.2f%%\n\n",
		stFinalData.nInCount,
		g_DMCont.m_dmEQP.GN(NDM0_CurLotOutput1Cnt), g_DMCont.m_dmEQP.GN(NDM0_CurLotOutput2Cnt), g_DMCont.m_dmEQP.GN(NDM0_CurLotOutput3Cnt), g_DMCont.m_dmEQP.GN(NDM0_CurLotOutput4Cnt),
		//stFinalData.nHwBinCount[0],stFinalData.nHwBinCount[1],stFinalData.nHwBinCount[2],stFinalData.nHwBinCount[3],
		(stFinalData.nInCount > 0 ? ((double)stFinalData.nOutCount / (double)stFinalData.nInCount) *100.0 : 0));

	strJamData += strTmpData;

	switch (nAutoRetestKind)
	{
	case eAutoRetest_AA:
	case eAutoRetest_AB:
	{
		strJamData += "\n";
	}break;
	case eAutoRetest_Aonly:
	{
		strJamData += "\n\n";
	}break;
	}

	strTmpData.Format(_T("SBIN Summary\n"));
	strJamData += strTmpData;
	strTmpData.Format(_T("ART,Error Code,PF,Error String,HB,Total,"));
	strJamData += strTmpData;


	int nMaxSite = eMaxPressUnitCount * (SiteCntX * SiteCntY);

	strTmpData = _T("");
	for (int nSite = 0; nSite < nMaxSite; nSite++) {
		strTmpData.Format(_T("Site%d,"), nSite + 1);
		strJamData += strTmpData;
	}
	strJamData += _T("\n");
 	strJamData += _T("\n");

	for (int nInfoIdx = 0; nInfoIdx < nRetestCount; nInfoIdx++)
	{
		std::vector<ST_DVC_DATA> vErrList;
		for (std::vector<ST_REC_DATA>::iterator it = m_vOnlyOneDvc.begin(); it != m_vOnlyOneDvc.end(); ++it) {
			if (strcmp(it->stDvcInfo[nInfoIdx].szErrStr, "") == 0 /*&& strcmp(it->stDvcInfo[nInfoIdx].szErrCode, "") == 0*/) { continue; }

			BOOL bFindSame = FALSE;
			for (std::vector<ST_DVC_DATA>::iterator it2 = vErrList.begin(); it2 != vErrList.end(); ++it2) {
				if ( /*strcmp( it2->szErrCode, it->stDvcInfo[nInfoIdx].szErrCode) ==0 &&*/ strcmp(it2->szErrStr, it->stDvcInfo[nInfoIdx].szErrStr) == 0) {
					bFindSame = TRUE;
				}
			}

			if (bFindSame == FALSE) {
				ST_DVC_DATA stCopyData;
				stCopyData = it->stDvcInfo[nInfoIdx];
				vErrList.push_back(stCopyData);
			}
		}

		for (std::vector<ST_DVC_DATA>::iterator it2 = vErrList.begin(); it2 != vErrList.end(); ++it2)
		{
			int nSiteBinCount[eMaxPressUnitCount][STATION_MAX_X_SITE][STATION_MAX_Y_SITE] = { 0, };
			CString strInfo = _T(""); // Error Code,PF,Error String,HB
			strTmpData.Empty();
			for (std::vector<ST_REC_DATA>::iterator it = m_vOnlyOneDvc.begin(); it != m_vOnlyOneDvc.end(); ++it) {
				if (strcmp(it->stDvcInfo[nInfoIdx].szErrStr, it2->szErrStr) == 0 /*&& strcmp(it->stDvcInfo[nInfoIdx].szErrCode, it2->szErrCode)==0*/) {
					nSiteBinCount[it->stDvcInfo[nInfoIdx].nSiteNo][it->stDvcInfo[nInfoIdx].cSubSiteNo.x][it->stDvcInfo[nInfoIdx].cSubSiteNo.y]/*[cp.x + (cp.y * 2)]*/ += 1; //Site Count

					if (strInfo.IsEmpty() == TRUE) {
						strInfo.Format("%d,%s,%s,%s,%d", nInfoIdx,/* it2->szErrCode,*/ nProjectOption == DEF_PROJECT_WATT ? it2->szTesterHwBin :it2->szErrCode, (it2->nPF == eGoodBin ? _T("P") : _T("F")), it2->szErrStr, it2->nHwBin + 1);
					}
				}
			}

			// find some data
			if (strInfo.IsEmpty() != TRUE) {
				int nSumCount = 0;
				CString strData = _T(""); // Other Remain Data
				CString strUnitData = _T("");
				CString strTotalData = _T("");
				for (int i = ePressUnit_Down_1; i < eMaxPressUnitCount; i++) {
					for (int nY = 0; nY < SiteCntY; nY++)
					{
						for (int nX = 0; nX < SiteCntX; nX++)
						{
							strData.Format(",%d", nSiteBinCount[i][nX][nY]);
							strUnitData += strData;

							nSumCount += nSiteBinCount[i][nX][nY]; // Total
						}
					}
				}

				strTotalData.Format(",%d", nSumCount);
				strTmpData += strInfo;
				strTmpData += strTotalData;
				strTmpData += strUnitData;
				strTmpData += "\n";
				strJamData += strTmpData;
			}
		}
	}
	//strJamData += _T("\nEND");

	if (bIsFileErr == FALSE) {
		file.WriteString(strJamData);
		file.Close();
	}

	if (bIsLocalFileErr == FALSE) {
		fileLocal.WriteString(strJamData);
		fileLocal.Close();
	}

	if (nRunMode == eRUN_MODE_ONLINE)
	{
		char szNetWorkPathBuff[MAX_PATH] = { 0, };
		g_DMCont.m_dmHandlerOpt.GS(SDM11_NetWorkSummaryFile_Path, szNetWorkPathBuff, MAX_PATH);

		int nSummaryInterlock = g_TaskSystemCtrl.SummaryNetworkDriveInterlockCheck(szNetWorkPathBuff);

		if (nSummaryInterlock != ERR_NO_ERROR) {
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("Summary Network Driver disconnect!!. Please check Summary Path = [%s]");
			g_Error.AlarmReport(nSummaryInterlock, AlarmData, NULL);
			return nSummaryInterlock;
		}

		CString strPath = szNetWorkPathBuff;
		CString strNewFilePath, strMsg;
		strPath += strDatePath;
		m_fnCreateDirectory(strPath);
		strNewFilePath.Format("%s%s", strPath, strFileName);
		strMsg.Format(_T("[MakeSummary]strLocalPath : %s || strNewFilePath : %s"), strLocalPath, strNewFilePath);
		TWLOG.MakeLog(static_cast<int>(eLogID_SYSTEM), Debug, "", __LINE__, NULL, strMsg);

	
		//Local Drive에 복사 할 파일이 있는지 체크 한다. 1초 동안 확인 했는대 없으면 그냥 넘어 간다.
		double dTimeCheck = (double)GetTickCount64();
		double dSpanTime;
		BOOL bWorking = FALSE;
		CFileFind finder;
		do 
		{
			dSpanTime = (double)GetTickCount64() - dTimeCheck;
			bWorking = finder.FindFile(strLocalPath);
			if (bWorking)
				break;

			Sleep(10);

		} while (dSpanTime < 1*1000.0); //1sec

		finder.Close();

		CopyFile(strLocalPath, strNewFilePath, FALSE);
	}

	return ERR_NO_ERROR;
}

int CProcJamRate::SaveLotHistory()
{
	_tLOTHISTORY_LOG tLotHistory;

	SYSTEMTIME time;
	::GetLocalTime(&time);
	CString strTmLotEnd = _T("");
	strTmLotEnd.Format("%04d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	std::vector<ST_ERROR_MESSAGE> vDataErrLog;
	vDataErrLog.clear();

	char szTmStart[128] = { 0, };
	g_DMCont.m_dmEQP.GS(SDM0_CurLotStartTm, szTmStart, sizeof(szTmStart));

	//g_DBErr.SELECT_ERROR_LOG_TBL(szTmStart, strTmLotEnd, vDataErrLog);
	g_DBErr.SELECT_ERROR_LOG_TBL_LEVEL(szTmStart, strTmLotEnd, eAlarm_Lv3, vDataErrLog);

	int nMsgCount = (int)vDataErrLog.size();
	int nPartJamCnt[ePos_MaxPosition];

	ZeroMemory(nPartJamCnt, sizeof(nPartJamCnt));

	for (int i = 0; i < nMsgCount; i++)
	{
		int nPartNum = vDataErrLog[i].nPosition;
		if (ePos_Stacker_1 <= nPartNum && nPartNum <= ePos_Station_8) {
			nPartJamCnt[nPartNum]++;
		}
	}

	int nDeviceSizeX = g_DMCont.m_dmHandlerOpt.GN(NDM11_DVC_Dimen_X);
	int nDeviceSizeY = g_DMCont.m_dmHandlerOpt.GN(NDM11_DVC_Dimen_Y);

	sprintf_s(tLotHistory.szEQP_ID, sizeof(tLotHistory.szEQP_ID), "%d", g_DMCont.m_dmEQP.GN(NDM0_Handler_EQP_No));
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, tLotHistory.szPKGName, sizeof(tLotHistory.szPKGName));
	tLotHistory.dDeviceSizeX = (double)nDeviceSizeX /1000;
	tLotHistory.dDeviceSizeY = (double)nDeviceSizeY / 1000;
	g_DMCont.m_dmHandlerOpt.GS(SDM11_LotNo, tLotHistory.szLotNo_Info, sizeof(tLotHistory.szLotNo_Info));
	tLotHistory.strLotEnd_Time = strTmLotEnd;
	tLotHistory.strLotStart_Time = szTmStart;
	tLotHistory.nOnLine_OffLine = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);

	int nUseIdxCnt = 0;
	double dSummaryIndex = 0;
	for (int nIndex = 0; nIndex < eMaxPressUnitCount; nIndex++)
	{
		double dIndexTime = g_DMCont.m_dmContac_Dn.GD(DDM1_SUMMARY_INDEX_STATION1 + nIndex);
		if (dIndexTime > 0)
		{
			dSummaryIndex += dIndexTime;
			nUseIdxCnt++;
		}
	}
	double dCalc = dSummaryIndex / (double)nUseIdxCnt;

	if (dCalc > 0)
	{
		tLotHistory.dAvgIndexTime = dCalc;
	}
	else {
		tLotHistory.dAvgIndexTime = 0;
	}

	tLotHistory.nUPH = g_DMCont.m_dmEQP.GN(NDM0_UPH);

	int nRunTm = g_DMCont.m_dmEQP.GN(NDM0_LotRunTime);
	CString str = _T("");
	// Lot run time
	int nHour = nRunTm / 3600;
	int nMin = (nRunTm - (nHour * 3600)) / 60;
	int nSec = (nRunTm - (nHour * 3600) - (nMin * 60));
	tLotHistory.strLotRunning_Time.Format("%d:%02d:%02d", nHour, nMin, nSec);
	

	for (int i = 0; i < ePos_MaxPosition-1; i++)
	{
		tLotHistory.nPartJamCnt[i] = nPartJamCnt[i+1];
	}
	tLotHistory.nLoadCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt);
	tLotHistory.nUnloadCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt);
	tLotHistory.nRetest[0] = g_DMCont.m_dmEQP.GN(NDM0_ART_RT1_TOTAL);
	tLotHistory.nRetest[1] = g_DMCont.m_dmEQP.GN(NDM0_ART_RT2_TOTAL);
	tLotHistory.nPassCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotPassBinCnt);
	tLotHistory.nFailCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotFailBinCnt);

	g_DBLotHistory.INSERT_LOTHISTORY_TBL(tLotHistory);
	return 0;
}

void CProcJamRate::PushDeviceData( ST_REC_DATA stData )
{
	m_vOnlyOneDvc.push_back(stData);
	SaveBackUpData(eJamRateDataType_Current, m_vOnlyOneDvc);
}

void CProcJamRate::ResetAllData()
{
	std::vector<ST_REC_DATA>().swap(m_vOnlyOneDvc);
	std::vector<ST_REC_DATA>().swap(m_vFirstData);
	std::vector<ST_REC_DATA>().swap(m_vSecondData);
	std::vector<ST_PRODUCTION_DATA>().swap(m_vProductionData);

	m_vOnlyOneDvc.clear();
	m_vFirstData.clear();
	m_vSecondData.clear();
	m_vProductionData.clear();

	m_nInputJamCnt=0;
	m_nOutputJamCnt=0;

	SaveBackUpData(eJamRateDataType_Current, m_vOnlyOneDvc);
	SaveBackUpData(eJamRateDataType_First, m_vFirstData);
	SaveBackUpData(eJamRateDataType_Last, m_vSecondData);
	
	ProductCsvTransfer();
}

void CProcJamRate::ClearRecDvcData()
{
	std::vector<ST_REC_DATA>().swap(m_vOnlyOneDvc);
	m_vOnlyOneDvc.clear();

	m_nInputJamCnt=0;
	m_nOutputJamCnt=0;

	SaveBackUpData(eJamRateDataType_Current, m_vOnlyOneDvc);
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : 
  RETURN   :  
  COMMENTS : Bin category data를 설정한다.
==============================================================================*/
void CProcJamRate::SetBinCateItemList()
{
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : 
  PARAMS   : 
  RETURN   :  
  COMMENTS : Tester의 Infomation 을 보여준다.
==============================================================================*/



void CProcJamRate::WriteRecord(std::ostream& os, const std::vector<ST_REC_DATA> &vec)
{
	if( (int)vec.size() <= 0) { return; }
	/*typename*/ std::vector<ST_REC_DATA>::size_type size = vec.size();
	os.write((char*)&size, sizeof(size));
	os.write((char*)&vec[0], vec.size() * sizeof(ST_REC_DATA));
}

void CProcJamRate::ReadRecord(std::istream& is, std::vector<ST_REC_DATA> &vec)
{
	/*typename*/ std::vector<ST_REC_DATA>::size_type size = 0;
	is.read((char*)&size, sizeof(size));

	if(size > 0){
		vec.resize(size);
		is.read((char*)&vec[0], vec.size() * sizeof(ST_REC_DATA));
	}	
}
void CProcJamRate::SaveBackUpData(int nType/*enum eJamRateDataType{*/, const std::vector<ST_REC_DATA> &vec)
{
	char szPath[1024] = {0,};
	char szTemp[512] = {0,};
	::GetCurrentDirectory(sizeof(szTemp), szTemp);
	switch(nType)
	{
	case eJamRateDataType_Current: { sprintf_s(szPath, sizeof(szPath), "%s\\sys_data\\BakCurrent.dat", szTemp); } break;
	case eJamRateDataType_First  : { sprintf_s(szPath, sizeof(szPath), "%s\\sys_data\\BakFirst.dat", szTemp);   } break;
	case eJamRateDataType_Last   : { sprintf_s(szPath, sizeof(szPath), "%s\\sys_data\\BakLast.dat", szTemp);    } break;
	default: { sprintf_s(szPath, sizeof(szPath), "%s\\sys_data\\BakCurrent.dat", szTemp); } break;
	}
	std::ofstream out(szPath, std::ios::out | std::ios::binary);
	WriteRecord(out, vec);
	out.close();
}

void CProcJamRate::LoadBackUpData(int nType/*enum eJamRateDataType{*/)
{
	char szPath[1024] = {0,};
	char szTemp[512] = {0,};
	::GetCurrentDirectory(sizeof(szTemp), szTemp);
	std::vector<ST_REC_DATA> vec;

	switch(nType)
	{
	case eJamRateDataType_Current: { sprintf_s(szPath, sizeof(szPath), "%s\\sys_data\\BakCurrent.dat", szTemp); } break;
	case eJamRateDataType_First  : { sprintf_s(szPath, sizeof(szPath), "%s\\sys_data\\BakFirst.dat", szTemp);   } break;
	case eJamRateDataType_Last   : { sprintf_s(szPath, sizeof(szPath), "%s\\sys_data\\BakLast.dat", szTemp);    } break;
	default: { sprintf_s(szPath, sizeof(szPath), "%s\\sys_data\\BakCurrent.dat", szTemp); } break;
	}
	std::ifstream in(szPath, std::ios::in | std::ios::binary);
	ReadRecord(in, vec);

	switch(nType)
	{
	case eJamRateDataType_Current: { m_vOnlyOneDvc = vec; } break;
	case eJamRateDataType_First  : { m_vFirstData  = vec; } break;
	case eJamRateDataType_Last   : { m_vSecondData = vec; } break;
	}
}

void CProcJamRate::PushInOutJam( int nInOutType, int nCount )
{
	switch(nInOutType)
	{
	case eJamType_Input:
		{
			m_nInputJamCnt+=nCount;
		}break;
	case eJamType_Output:
		{
			m_nOutputJamCnt+=nCount;
		}break;
	}
}


void CProcJamRate::ProductCsvTransfer()
{
	CString strExistingFileName = _T(""), strNewFileName = _T(""), strExtraFileName = _T(""), strMsg = _T("");
	char szLotId[512] = { 0, }, szScheduleID[512] = { 0, }, sz2didCsvPath[512] = { 0, };

	g_DMCont.m_dmHandlerOpt.GS(SDM11_2did_Csv_File_Path, sz2didCsvPath, sizeof(sz2didCsvPath));
	g_DMCont.m_dmHandlerOpt.GS(SDM11_ScheduleNo, szScheduleID, sizeof(szScheduleID));
	g_DMCont.m_dmHandlerOpt.GS(SDM11_LotNo, szLotId, sizeof(szLotId));

	SYSTEMTIME st;
	GetLocalTime(&st);

	strExistingFileName.Format("D:\\ProductCSV\\Temporary\\%s.txt", szLotId);
	int nResult = access(strExistingFileName, 0);

	if (nResult == 0) {// Temporary 폴더에 해당하는 File 이 있으면
		//기존 D: 폴더 복사
		strNewFileName.Format("D:\\ProductCSV\\%s_%s_%04d%02d%02d_%02d%02d%02d.csv", szLotId, szScheduleID, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

		BOOL bErr = CopyFile(strExistingFileName, strNewFileName, FALSE);
		if (bErr == 0) { // bErr == 0 이면 Copy가 되지 않았다
			bErr = CopyFile(strExistingFileName, strNewFileName, FALSE);
			if (bErr == 0) {
				strMsg.Format(_T("ProductCSV File is not Transfer Lot No : %s"), szLotId);
				TWLOG.MakeLog(static_cast<int>(eLogID_SYSTEM), Debug, "", __LINE__, NULL, strMsg);
				AfxMessageBox("ProductCSV File is copy fail!!!");
			}
		}

		//고객사 설정 경로 폴더 복사
		CString strSetCsvPath = sz2didCsvPath;
		strExtraFileName.Format("%s\\%s_%s_%04d%02d%02d_%02d%02d%02d.csv", strSetCsvPath, szLotId, szScheduleID, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		bErr = CopyFile(strExistingFileName, strExtraFileName, FALSE);
		if (bErr == 0) { // bErr == 0 이면 Copy가 되지 않았다
			bErr = CopyFile(strExistingFileName, strExtraFileName, FALSE);
			if (bErr == 0) {
				strMsg.Format(_T("Setting CSV File is not Transfer Lot No : %s"), szLotId);
				TWLOG.MakeLog(static_cast<int>(eLogID_SYSTEM), Debug, "", __LINE__, NULL, strMsg);
				AfxMessageBox("Setting CSV File is copy fail!!!");
			}
		}
		

		if (bErr != 0) {
			DeleteFile(strExistingFileName);
			strMsg.Format(_T("Product Csv File Transfer Lot No: %s"), szLotId);
			TWLOG.MakeLog(static_cast<int>(eLogID_SYSTEM), Debug, "", __LINE__, NULL, strMsg);
		}
	}
}
