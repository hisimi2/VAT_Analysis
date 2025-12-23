// EventLogDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "EventLogDlg.h"
#include "afxdialogex.h"

std::vector<ST_ERROR_MESSAGE> CEventLogDlg::vDataErrLog;
std::vector<ST_TEACH_LOG> CEventLogDlg::vDataTeachLog;

enum eEventTabPageIndexs
{
	eEventTab_Log,
	eEvenrTab_Frequency,
	eEvenrTab_TeachLog,
};
// CEventLogDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEventLogDlg, CDialog)

CEventLogDlg::CEventLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEventLogDlg::IDD, pParent)
	, m_nErrLevel(0)
{
	vDataErrLog.clear();
	m_bSortAscending = false;
	vDataTeachLog.clear();
}

CEventLogDlg::~CEventLogDlg()
{
}
void CEventLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_START_DATE, m_tStartDate);
	DDX_Control(pDX, IDC_START_TIME, m_tStartTime);
	DDX_Control(pDX, IDC_END_DATE, m_tEndDate);
	DDX_Control(pDX, IDC_END_TIME, m_tEndTime);
	DDX_CBIndex(pDX, IDC_COMBO_LEVEL, m_nErrLevel);
	DDX_Control(pDX, IDC_TAB_LOG, m_tabEvent);

	DDX_Control(pDX, IDC_BUTTON_ALL_LOG, m_BtnAllLog);
	DDX_Control(pDX, IDC_BUTTON_LOG_LEVEL0, m_BtnLogLevel0);
	DDX_Control(pDX, IDC_BUTTON_LOG_LEVEL1, m_BtnLogLevel1);
	DDX_Control(pDX, IDC_BUTTON_LOG_LEVEL2, m_BtnLogLevel2);
	DDX_Control(pDX, IDC_BUTTON_LOG_LEVEL3, m_BtnLogLevel3);
	DDX_Control(pDX, IDC_BUTTON_EXPERT, m_BtnLogExpert);
	DDX_Control(pDX, IDC_BUTTON_DISPLAY, m_BtnLogDisplay);

}


BEGIN_MESSAGE_MAP(CEventLogDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CEventLogDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_ALL_LOG, &CEventLogDlg::OnBnClickedButtonAllLog)
	ON_BN_CLICKED(IDC_BUTTON_LOG_LEVEL0, &CEventLogDlg::OnBnClickedButtonLogLevel0)
	ON_BN_CLICKED(IDC_BUTTON_LOG_LEVEL1, &CEventLogDlg::OnBnClickedButtonLogLevel1)
	ON_BN_CLICKED(IDC_BUTTON_LOG_LEVEL2, &CEventLogDlg::OnBnClickedButtonLogLevel2)
	ON_BN_CLICKED(IDC_BUTTON_LOG_LEVEL3, &CEventLogDlg::OnBnClickedButtonLogLevel3)
	ON_BN_CLICKED(IDC_BUTTON_EXPERT, &CEventLogDlg::OnBnClickedButtonExpert)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY, &CEventLogDlg::OnBnClickedButtonDisplay)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_LOG, &CEventLogDlg::OnTcnSelchangeTabLog)
END_MESSAGE_MAP()

// CEventLogDlg 메시지 처리기입니다.
BOOL CEventLogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnInitDialog_MainTabPages();
	
	OnBtnGUI();
	LoadDB_ErrMsg(eAlarm_All);	
	LoadDB_Teach(eAlarm_All);
	InitListGUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CEventLogDlg::InitListGUI()
{
	g_LibCommCtrl.ProgressBarCreate();
	g_LibCommCtrl.ProgressBarSetStatus("Load Event Log Data");
	g_LibCommCtrl.ProgressBarSetPos(10);

	m_pgEventLog.InitListGUI();

	g_LibCommCtrl.ProgressBarSetPos(60);

	m_pgEventFrequency.InitListGUI();

	g_LibCommCtrl.ProgressBarDestroy();

	m_pgEventTeachLog.InitListGUI();
}

void CEventLogDlg::LoadDB_ErrMsg(int nSelectAlarm)
{
	vDataErrLog.clear();

	CTime cTmCurrent = CTime::GetCurrentTime();
	CString strCurTime = _T("");
	strCurTime.Format("%04d-%02d-%02d %02d:%02d:%02d.000", cTmCurrent.GetYear(), cTmCurrent.GetMonth(), cTmCurrent.GetDay()
		, cTmCurrent.GetHour(), cTmCurrent.GetMinute(), cTmCurrent.GetSecond());
	CString strStartTime = _T("");

	int nYear = cTmCurrent.GetYear();
	int nMonth = cTmCurrent.GetMonth();

	if (nMonth - 1 < 1)
	{
		nYear -= 1;
		nMonth = 12;
	}
	else {
		nMonth -= 1;
	}

	strStartTime.Format("%04d-%02d-01 00:00:000.000", nYear, nMonth);

	switch(nSelectAlarm)
	{
	case eAlarm_Lv0:
		g_DB_LOG.SELECT_ERROR_LOG_TBL_LEVEL(strStartTime, strCurTime, eAlarm_Lv0, vDataErrLog);
		break;
	case eAlarm_Lv1:
		g_DB_LOG.SELECT_ERROR_LOG_TBL_LEVEL(strStartTime, strCurTime, eAlarm_Lv1, vDataErrLog);
		break;
	case eAlarm_Lv2:
		g_DB_LOG.SELECT_ERROR_LOG_TBL_LEVEL(strStartTime, strCurTime, eAlarm_Lv2, vDataErrLog);
		break;
	case eAlarm_Lv3:
		g_DB_LOG.SELECT_ERROR_LOG_TBL_LEVEL(strStartTime, strCurTime, eAlarm_Lv3, vDataErrLog);
		break;
	case eAlarm_All:
		g_DB_LOG.SELECT_ERROR_LOG_TBL(strStartTime, strCurTime, vDataErrLog);
		break;
	case eAlarm_Period:
		{
			UpdateData(TRUE);

			COleDateTime oleDate;
			COleDateTime oleTime;

			//Get Start Time&Date
			m_tStartDate.GetTime(oleDate);
			m_tStartTime.GetTime(oleTime);
			CString strStartTm = _T("");
			strStartTm.Format("%04d-%02d-%02d %02d:%02d:%02d.000", oleDate.GetYear(), oleDate.GetMonth(), oleDate.GetDay(), oleTime.GetHour(), oleTime.GetMinute(), oleTime.GetSecond());

			//Get End Time&Date
			m_tEndDate.GetTime(oleDate);
			m_tEndTime.GetTime(oleTime);
			CString strEndTm = _T("");
			strEndTm.Format("%04d-%02d-%02d %02d:%02d:%02d", oleDate.GetYear(), oleDate.GetMonth(), oleDate.GetDay(), oleTime.GetHour(), oleTime.GetMinute(), oleTime.GetSecond());

			if(m_nErrLevel != eErrLevel_All)
				g_DB_LOG.SELECT_ERROR_LOG_TBL_LEVEL(strStartTm, strEndTm, m_nErrLevel, vDataErrLog);
			else
				g_DB_LOG.SELECT_ERROR_LOG_TBL(strStartTm, strEndTm, vDataErrLog);
		}break;
	default:
		g_DB_LOG.SELECT_ERROR_LOG_TBL(strStartTime, strCurTime, vDataErrLog);
		break;
	}
}
void CEventLogDlg::LoadDB_Teach(int nSelectAlarm)
{
	vDataTeachLog.clear();

	CTime cTmCurrent = CTime::GetCurrentTime();
	CString strCurTime = _T("");
	strCurTime.Format("%04d-%02d-%02d %02d:%02d:%02d.000", cTmCurrent.GetYear(), cTmCurrent.GetMonth(), cTmCurrent.GetDay()
		, cTmCurrent.GetHour(), cTmCurrent.GetMinute(), cTmCurrent.GetSecond());
	CString strStartTime = _T("");

	int nYear = cTmCurrent.GetYear();
	int nMonth = cTmCurrent.GetMonth();

	if (nMonth - 1 < 1)
	{
		nYear -= 1;
		nMonth = 12;
	}
	else {
		nMonth -= 1;
	}

	strStartTime.Format("%04d-%02d-01 00:00:000.000", nYear, nMonth);
	switch (nSelectAlarm)
	{
		case eAlarm_All:
		{
			g_DB_Teach.SELECT_TEACH_LOG_TBL(strStartTime, strCurTime, vDataTeachLog);
		}break;
		case eAlarm_Period:
		{
			UpdateData(TRUE);

			COleDateTime oleDate;
			COleDateTime oleTime;

			//Get Start Time&Date
			m_tStartDate.GetTime(oleDate);
			m_tStartTime.GetTime(oleTime);
			CString strStartTm = _T("");
			strStartTm.Format("%04d-%02d-%02d %02d:%02d:%02d.000", oleDate.GetYear(), oleDate.GetMonth(), oleDate.GetDay(), oleTime.GetHour(), oleTime.GetMinute(), oleTime.GetSecond());

			//Get End Time&Date
			m_tEndDate.GetTime(oleDate);
			m_tEndTime.GetTime(oleTime);
			CString strEndTm = _T("");
			strEndTm.Format("%04d-%02d-%02d %02d:%02d:%02d", oleDate.GetYear(), oleDate.GetMonth(), oleDate.GetDay(), oleTime.GetHour(), oleTime.GetMinute(), oleTime.GetSecond());

			g_DB_Teach.SELECT_TEACH_LOG_TBL(strStartTm, strEndTm, vDataTeachLog);
		}break;
	}
		

	
}

void CEventLogDlg::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnAllLog.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnAllLog.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnLogLevel0.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnLogLevel0.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnLogLevel1.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnLogLevel1.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnLogLevel2.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnLogLevel2.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnLogLevel3.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnLogLevel3.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnLogExpert.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnLogExpert.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnLogDisplay.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnLogDisplay.SetAlign(CButtonST::ST_ALIGN_VERT);

}

void CEventLogDlg::WritePartName(int nPartNum, CString & strPartName)
{
	switch (nPartNum)
	{
	case ePos_Stacker_1:
	{
		strPartName.Format("STACKER_1");
	}break;
	case ePos_Stacker_2:
	{
		strPartName.Format("STACKER_2");
	}break;
	case ePos_Stacker_3:
	{
		strPartName.Format("STACKER_3");
	}break;
	case ePos_Stacker_4:
	{
		strPartName.Format("STACKER_4");
	}break;
	case ePos_Stacker_5:
	{
		strPartName.Format("STACKER_5");
	}break;
	case ePos_Stacker_6:
	{
		strPartName.Format("STACKER_6");
	}break;
	case ePos_Stacker_7:
	{
		strPartName.Format("STACKER_7");
	}break;
	case ePos_LoadTbl_1:
	{
		strPartName.Format("LOADING_TABLE_1");
	}break;
	case ePos_LoadTbl_2:
	{
		strPartName.Format("LOADING_TABLE_2");
	}break;
	case ePos_LoadTbl_3:
	{
		strPartName.Format("LOADING_TABLE_3");
	}break;
	case ePos_LoadTbl_4:
	{
		strPartName.Format("LOADING_TABLE_4");
	}break;
	case ePos_TrayPP_1:
	{
		strPartName.Format("TRAY_PP_1");
	}break;
	case ePos_TrayPP_2:
	{
		strPartName.Format("TRAY_PP_2");
	}break;
	case ePos_Feeder:
	{
		strPartName.Format("FEEDER");
	}break;
	case ePos_CleanBuf_1:
	{
		strPartName.Format("CLEAN_TABLE_1");
	}break;
	case ePos_CleanBuf_2:
	{
		strPartName.Format("CLEAN_TABLE_2");
	}break;
	case ePos_TestPP_1:
	{
		strPartName.Format("TEST_PP_1");
	}break;
	case ePos_TestPP_2:
	{
		strPartName.Format("TEST_PP_2");
	}break;
	case ePos_Station_1:
	{
		strPartName.Format("PRESS_UNIT_1");
	}break;
	case ePos_Station_2:
	{
		strPartName.Format("PRESS_UNIT_2");
	}break;
	case ePos_Station_3:
	{
		strPartName.Format("PRESS_UNIT_3");
	}break;
	case ePos_Station_4:
	{
		strPartName.Format("PRESS_UNIT_4");
	}break;
	case ePos_Station_5:
	{
		strPartName.Format("PRESS_UNIT_5");
	}break;
	case ePos_Station_6:
	{
		strPartName.Format("PRESS_UNIT_6");
	}break;
	case ePos_Station_7:
	{
		strPartName.Format("PRESS_UNIT_7");
	}break;
	case ePos_Station_8:
	{
		strPartName.Format("PRESS_UNIT_8");
	}break;
	case ePos_Transfer:
	{
		strPartName.Format("TRANSFER");
	}break;

	default:
	{
		strPartName.Format("SYSTEM");
	}break;
	}
}

void CEventLogDlg::MakeLotHistoryLog(CString strStartTm, CString strEndTm)
{
	CFile	LotHistoryFile;

	CString sFilePath = _T("D:/Techwing/LogExpData"), sFileName = _T(""), sDataFileName = _T(""), strLevel = _T(""), sFrequencyFileName = _T("");

	sFileName.Format("LotHistory_%s_%s", (LPCTSTR)strStartTm, (LPCTSTR)strEndTm);
	sFileName.Replace(":", "_");
	sFileName.Replace(" ", "_");

	sDataFileName.Format("%s/%s.csv", (LPCTSTR)sFilePath, (LPCTSTR)sFileName);
	sFrequencyFileName.Format("%s/%s_Frequency.csv", (LPCTSTR)sFilePath, (LPCTSTR)sFileName);

	// make folder
	CString csPrefix(_T("")), csToken(_T(""));
	int nStart = 0, nEnd;
	while ((nEnd = sFilePath.Find('/', nStart)) >= 0)
	{
		CString csToken = sFilePath.Mid(nStart, nEnd - nStart);
		CreateDirectory(csPrefix + csToken, NULL);

		csPrefix += csToken;
		csPrefix += _T("/");
		nStart = nEnd + 1;
		NEXT;
	}
	csToken = sFilePath.Mid(nStart);
	CreateDirectory(csPrefix + csToken, NULL);

	//open file
	if (!LotHistoryFile.Open(sDataFileName, CFile::modeWrite | CFile::modeCreate))
	{
		CString sMsg;
		sMsg.Format("File Creation Fail!\n%s", (LPCTSTR)sDataFileName);	// By mjna [140801]: Spreadtrum RS232C communication, change to english
		AfxMessageBox(sMsg);
		return;
	}

	std::vector<ST_LOTHISTORY> vLogData;

	g_DB_LotHistory.SELECT_LOTHISTORY_TBL(strStartTm, strEndTm, vLogData);

	int nMsgCount = (int)vLogData.size();

	CString tmpMsg = _T(""), strMsg = _T("");
	CString FullData = _T("EQP NO, PKG Name, DeviceSize X, DeviceSize Y, OnOffline, Lot No, Date, Stacker & Conveyor 1, Stacker & Conveyor 2, Stacker & Conveyor 3, Stacker & Conveyor 4, Stacker & Conveyor 5, Stacker & Conveyor 6, Stacker & Conveyor 7, Load Table 1, Load Table 2, Load Table 3, Load Table 4, Tray Load PP, Tray Unload PP, Tray Feeder, Clean Buffer Table 1, Clean Buffer Table 2, Test PP 1, Test PP 2, Station 1, Station 2, Station 3, Station 4, Station 5, Station 6, Station 7, Station 8, Transfer, Jam Total, Load Device, Unload Device, Retest1 Device, Retest2 Device, Pass Bin, Fail Bin, Start Time, End Time, Run Time, AvgIndexTime, UPH");
	FullData += "\r\n";

	for (int i = 0; i < nMsgCount; i++) 
	{
		strMsg.Empty();
		tmpMsg.Empty();

		CString strPartJamCnt = _T(""), tmpPartJamCnt = _T(""), strOnOffline = _T("");
		int nJamTotalCNT = 0;

		for (int j =0; j <ePos_MaxPosition-1; j++)
		{
			strPartJamCnt.Format("%d", vLogData[i].nPartJamCnt[j]);
			tmpPartJamCnt += strPartJamCnt;
			nJamTotalCNT += vLogData[i].nPartJamCnt[j];

			if (j != ePos_MaxPosition - 2)
			{
				tmpPartJamCnt += ",";
			}
		}

		if (vLogData[i].nOnLine_OffLine == FALSE)
		{
			strOnOffline.Format("Online");
		}
		else
		{
			strOnOffline.Format("Offline");
		}

		tmpMsg.Format("%s,%s,%.2f,%.2f,%s,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%s,%s,%s,%.2f,%d,\r\n", 
			vLogData[i].szEQP_ID, 
			vLogData[i].szPKGName, 
			vLogData[i].dDeviceSizeX, 
			vLogData[i].dDeviceSizeY,
			strOnOffline,
			vLogData[i].szLotNo_Info, 
			vLogData[i].strLotEnd_Time, 
			tmpPartJamCnt, 
			nJamTotalCNT,
			vLogData[i].nLoadCnt, 
			vLogData[i].nUnloadCnt, 
			vLogData[i].nRetest[0], 
			vLogData[i].nRetest[1], 
			vLogData[i].nPassCnt, 
			vLogData[i].nFailCnt,
			vLogData[i].strLotStart_Time,
			vLogData[i].strLotEnd_Time,
			vLogData[i].strLotRunning_Time,
			vLogData[i].dAvgIndexTime,
			vLogData[i].nUPH
		);

		FullData += tmpMsg;
	}

	LotHistoryFile.Write(FullData.GetBuffer(FullData.GetLength() + 1), FullData.GetLength() + 1);
	LotHistoryFile.Close();
}

void CEventLogDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnCancel();
}


void CEventLogDlg::OnBnClickedButtonAllLog()
{
	LoadDB_ErrMsg(eAlarm_All);
	LoadDB_Teach(eAlarm_All);
	InitListGUI();
}


void CEventLogDlg::OnBnClickedButtonLogLevel0()
{
	LoadDB_ErrMsg(eAlarm_Lv0);
	InitListGUI();
}


void CEventLogDlg::OnBnClickedButtonLogLevel1()
{
	LoadDB_ErrMsg(eAlarm_Lv1);
	InitListGUI();
}


void CEventLogDlg::OnBnClickedButtonLogLevel2()
{
	LoadDB_ErrMsg(eAlarm_Lv2);
	InitListGUI();
}


void CEventLogDlg::OnBnClickedButtonLogLevel3()
{
	LoadDB_ErrMsg(eAlarm_Lv3);
	InitListGUI();
}

void CEventLogDlg::OnBnClickedButtonExpert()
{
	g_LibCommCtrl.ProgressBarCreate();
	g_LibCommCtrl.ProgressBarSetStatus("Exprot Log Data");
	g_LibCommCtrl.ProgressBarSetPos(10);

	UpdateData(TRUE);
	COleDateTime oleDate;
	COleDateTime oleTime;

	//Get Start Time&Date
	m_tStartDate.GetTime(oleDate);
	m_tStartTime.GetTime(oleTime);
	CString strStartTm = _T("");
	strStartTm.Format("%04d-%02d-%02d %02d:%02d:%02d.0", oleDate.GetYear(), oleDate.GetMonth(), oleDate.GetDay(), oleTime.GetHour(), oleTime.GetMinute(), oleTime.GetSecond());

	//Get End Time&Date
	m_tEndDate.GetTime(oleDate);
	m_tEndTime.GetTime(oleTime);
	CString strEndTm = _T("");
	strEndTm.Format("%04d-%02d-%02d %02d:%02d:%02d.0", oleDate.GetYear(), oleDate.GetMonth(), oleDate.GetDay(), oleTime.GetHour(), oleTime.GetMinute(), oleTime.GetSecond());

	// set folder & file
	CFile	DataFile;
	CFile   FrequencyFile;

	CString sFilePath=_T("D:/Techwing/LogExpData"), sFileName = _T(""), sDataFileName=_T(""), strLevel = _T(""), sFrequencyFileName =_T("");
	switch(m_nErrLevel)
	{
	case eErrLevel_Log      : { strLevel.Format("Log"); } break;
	case eErrLevel_Message  : { strLevel.Format("Mes"); } break;
	case eErrLevel_Warn     : { strLevel.Format("War"); } break;
	case eErrLevel_Jam      : { strLevel.Format("Jam"); } break;
	case eErrLevel_Critical : { strLevel.Format("Cri"); } break;
	case eErrLevel_All: { strLevel.Format("All"); } break;
	}
	sFileName.Format("%s_%s_%s", (LPCTSTR)strLevel, (LPCTSTR)strStartTm, (LPCTSTR)strEndTm);
	sFileName.Replace(":", "_");
	sFileName.Replace(" ", "_");
	sDataFileName.Format("%s/%s.csv", (LPCTSTR)sFilePath, (LPCTSTR)sFileName);
	sFrequencyFileName.Format("%s/%s_Frequency.csv", (LPCTSTR)sFilePath, (LPCTSTR)sFileName);

	// make folder
	CString csPrefix(_T("")), csToken(_T(""));
	int nStart = 0, nEnd;
	while( (nEnd = sFilePath.Find('/', nStart)) >= 0)
	{
		CString csToken = sFilePath.Mid(nStart, nEnd-nStart);
		CreateDirectory(csPrefix + csToken, NULL);

		csPrefix += csToken;
		csPrefix += _T("/");
		nStart = nEnd+1;
		NEXT;
	} 
	csToken = sFilePath.Mid(nStart);
	CreateDirectory(csPrefix + csToken, NULL);

	//open file
	if( !DataFile.Open(sDataFileName, CFile::modeWrite | CFile::modeCreate))
	{
		CString sMsg;
		sMsg.Format("File Creation Fail!\n%s", (LPCTSTR)sDataFileName);	// By mjna [140801]: Spreadtrum RS232C communication, change to english
		AfxMessageBox(sMsg);
		return;
	}

	//open file
	if (!FrequencyFile.Open(sFrequencyFileName, CFile::modeWrite | CFile::modeCreate))
	{
		CString sMsg;
		sMsg.Format("File Creation Fail!\n%s", (LPCTSTR)sFrequencyFileName);	// By mjna [140801]: Spreadtrum RS232C communication, change to english
		AfxMessageBox(sMsg);
		return;
	}

	std::vector<ST_ERROR_MESSAGE> vLogData;

	if(m_nErrLevel != eErrLevel_All)
		g_DB_LOG.SELECT_ERROR_LOG_TBL_LEVEL(strStartTm, strEndTm, m_nErrLevel, vLogData);
	else
		g_DB_LOG.SELECT_ERROR_LOG_TBL(strStartTm, strEndTm, vLogData);
	
	g_LibCommCtrl.ProgressBarSetPos(30);

	int nMsgCount = (int)vLogData.size();

	CString tmpMsg = _T(""),  strMsg = _T("");
// 	CString FullData = _T("OccuredTime, ALID, PART, Message, Recovery, User, StopTime,");
// 	FullData += "\r\n";
	CString FullData = _T("LotNo, Data, OnOffline, ALID, Message, OccuredTime, StopTime, Recovery, PART, User,");
	FullData += "\r\n";

	for(int i=0; i<nMsgCount; i++){
		strMsg.Empty();
		tmpMsg.Empty();

		strMsg.Format("%s", vLogData[i].szMessage);
		strMsg.Replace("\r\n","");
		strMsg.Replace(",","");
		strMsg.Replace("\n","");

		CString strPart = _T(""), strDate = _T(""), strOnOffline = _T("") ;
		for (int j = 0; j < 10; j++)
		{
			strDate += vLogData[i].strOccurred_Time[j];
		}
		
		if (vLogData[i].nError_Level == eErrLevel_Jam)
		{
			WritePartName(vLogData[i].nPosition, strPart);
		} 
		else
		{
			strPart = vLogData[i].szPart;
		}
		
		if (vLogData[i].nOnLine_OffLine == FALSE)
		{
			strOnOffline.Format("Online");
		} 
		else
		{
			strOnOffline.Format("Offline");
		}

		tmpMsg.Format("%s,%s,%s,%s,%s,%s,%d,%s,%s,%s,\r\n", vLogData[i].szLotNo_Info, strDate, strOnOffline, vLogData[i].szEQP_ALID, strMsg, vLogData[i].strOccurred_Time, vLogData[i].nStop_Time, vLogData[i].szRecovery, strPart, vLogData[i].szUser_Info);

		FullData += tmpMsg;

	}


	DataFile.Write(FullData.GetBuffer(FullData.GetLength()+1), FullData.GetLength()+1);
	DataFile.Close();

	CString FrequencyFullData = _T("LotNo, Data, OnOffline, ALID, Message, OccuredTime, Jam Clear Time, StopTime, Recovery, PART, User,");
	FrequencyFullData += "\r\n";
	
	g_LibCommCtrl.ProgressBarSetPos(60);

	FrequencyFullData += m_pgEventFrequency.MakeExpertData(vLogData);

	FrequencyFile.Write(FrequencyFullData.GetBuffer(FrequencyFullData.GetLength() + 1), FrequencyFullData.GetLength() + 1);
	FrequencyFile.Close();

	g_LibCommCtrl.ProgressBarSetPos(100);
	g_LibCommCtrl.ProgressBarDestroy();

	MakeLotHistoryLog(strStartTm, strEndTm);
}


void CEventLogDlg::OnBnClickedButtonDisplay()
{
	LoadDB_ErrMsg(eAlarm_Period);
	LoadDB_Teach(eAlarm_Period);
	InitListGUI();
}

void CEventLogDlg::OnInitDialog_MainTabPages()
{
	/*inti tab control*/
	m_tabEvent.SetMinTabWidth(220);
	m_tabEvent.DeleteAllItems();
	m_tabEvent.InsertItem(eEventTab_Log, "Event Log");
	m_tabEvent.InsertItem(eEvenrTab_Frequency, "Event Log Frequency");
	m_tabEvent.InsertItem(eEvenrTab_TeachLog, "Teach Log");
	CRect rect;
	m_tabEvent.GetClientRect(&rect);
	m_pgEventLog.Create(CEventLog_List::IDD, &m_tabEvent);
	m_pgEventLog.SetWindowPos(NULL, 0, 25, rect.Width(), rect.Height() - 15, SWP_SHOWWINDOW | SWP_NOZORDER);

	m_pgEventFrequency.Create(CEventLog_Frequency::IDD, &m_tabEvent);
	m_pgEventFrequency.SetWindowPos(NULL, 0, 25, rect.Width(), rect.Height() - 15, SWP_NOZORDER);

	m_pgEventTeachLog.Create(CEventLog_Teach::IDD, &m_tabEvent);
	m_pgEventTeachLog.SetWindowPos(NULL, 0, 25, rect.Width(), rect.Height() - 15, SWP_NOZORDER);
	

	OnTcnSelchangeTabLog(nullptr, nullptr);
}

void CEventLogDlg::UIShowHide(int nTabType)
{
	if (nTabType == eEvenrTab_TeachLog)
	{
		//GetDlgItem(IDC_BUTTON_ALL_LOG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_LOG_LEVEL0)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_LOG_LEVEL1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_LOG_LEVEL2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_LOG_LEVEL3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_LEVEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_EXPERT)->ShowWindow(SW_HIDE);
	}
	else
	{
		//GetDlgItem(IDC_BUTTON_ALL_LOG)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_LOG_LEVEL0)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_LOG_LEVEL1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_LOG_LEVEL2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_LOG_LEVEL3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_LEVEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_EXPERT)->ShowWindow(SW_SHOW);
	}
}

void CEventLogDlg::OnTcnSelchangeTabLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int tab_index = m_tabEvent.GetCurSel();
	switch (tab_index)
	{
		case eEventTab_Log:
		{
			m_pgEventLog.ShowWindow(SW_SHOW);
			m_pgEventFrequency.ShowWindow(SW_HIDE);
			m_pgEventTeachLog.ShowWindow(SW_HIDE);
		}break;
		case eEvenrTab_Frequency:
		{
			m_pgEventLog.ShowWindow(SW_HIDE);
			m_pgEventFrequency.ShowWindow(SW_SHOW);
			m_pgEventTeachLog.ShowWindow(SW_HIDE);
		}break;
		case eEvenrTab_TeachLog:
		{
			m_pgEventLog.ShowWindow(SW_HIDE);
			m_pgEventFrequency.ShowWindow(SW_HIDE);
			m_pgEventTeachLog.ShowWindow(SW_SHOW);
		}break;
	}

	UIShowHide(tab_index);

	if (pResult)
		*pResult = 0;
}

void CEventLogDlg::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = { 0, };
	if (fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_System_GUI), Debug, "", __LINE__, NULL, tmpMsg);
}