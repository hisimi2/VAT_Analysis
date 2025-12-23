// DailyReport.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "DailyReport.h"
#include "afxdialogex.h"
#include "DlgAlarmZoom.h"


enum PreiodKind
{
	eOneDay,
	ePreiod,
};
// CDailyReport 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDailyReport, CDialogEx)

CDailyReport::CDailyReport(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_DAILY_REPORT, pParent)
{
	m_bSelectKind = eOneDay;
	m_stDailyData.Clear();
	m_nJamCnt = 0;
	m_vDataListErrLog.clear();
	ZeroMemory(m_nPartJamCnt, sizeof(m_nPartJamCnt));
}

CDailyReport::~CDailyReport()
{
}

void CDailyReport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_ONEDAY, m_bSelectKind);
	DDX_Control(pDX, IDC_START_DATE, m_tStartDate);
	DDX_Control(pDX, IDC_END_DATE, m_tEndDate);
	DDX_Control(pDX, IDC_SPREAD_TOTAL_DATA, m_SPRD_TotalData);
	DDX_Control(pDX, IDC_SPREAD_ART_YIELD, m_SPRD_ARTYield);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_BUTTON_APPLY, m_BtnApplay);
	DDX_Control(pDX, IDC_BUTTON_EXPERT, m_BtnExpert);
	DDX_Control(pDX, IDC_SPREAD_PART_COUNT, m_SPRD_PartJamCnt);
	DDX_Control(pDX, IDC_LIST_ERROR_LIST, m_List);
	DDX_Control(pDX, IDC_CHECK_DAILY_REPORT_JAM_ON_OFFLINE, m_ChkOnLine_OffLine_View);
}


BEGIN_MESSAGE_MAP(CDailyReport, CDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_ONEDAY, IDC_RADIO_PERIOD, OnClickedDateKind)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDailyReport::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_EXPERT, &CDailyReport::OnBnClickedButtonExpert)
	ON_BN_CLICKED(IDCANCEL, &CDailyReport::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_DAILY_REPORT_JAM_ON_OFFLINE, &CDailyReport::OnBnClickedCheckDailyReportJamOnOffline)

	ON_MESSAGE(WM_USER + 100, &CDailyReport::OnListDoubleClick)
END_MESSAGE_MAP()


// CDailyReport 메시지 처리기입니다.



BOOL CDailyReport::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	Uploaddata(TRUE);

	return TRUE;
}

void CDailyReport::Uploaddata(int nInit)
{
	m_SPRD_TotalData.SetMaxCols(1);
	m_SPRD_TotalData.SetMaxRows(5);

	m_SPRD_ARTYield.SetMaxCols(3);
	m_SPRD_ARTYield.SetMaxRows(3);

	m_SPRD_PartJamCnt.SetMaxCols(1);
	m_SPRD_PartJamCnt.SetMaxRows(ePos_MaxPosition - 1);

	m_bSelectKind = eOneDay;

	GetDlgItem(IDC_END_DATE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_1)->ShowWindow(SW_HIDE);

	CTime cTmCurrent = CTime::GetCurrentTime();
	CString strCurTime = _T("");
	strCurTime.Format("%04d-%02d-%02d", cTmCurrent.GetYear(), cTmCurrent.GetMonth(), cTmCurrent.GetDay());

	int nJam_OnLine_OffLine_View = g_DMCont.m_dmHandlerOpt.GB(BDM11_JAM_ONLINE_OFFLINE_VIEW);
	m_ChkOnLine_OffLine_View.SetCheck(nJam_OnLine_OffLine_View);

	if(nInit == TRUE)
		InitSpread();

	LoadData(strCurTime, strCurTime);

	InitData();

	OnBtnGUI();
}

void CDailyReport::OnClickedDateKind(UINT nID)
{
	switch (nID)
	{
	case IDC_RADIO_ONEDAY:
	{
		GetDlgItem(IDC_END_DATE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_1)->ShowWindow(SW_HIDE);
		m_bSelectKind = eOneDay;
	}break;
	case IDC_RADIO_PERIOD:
	{
		GetDlgItem(IDC_END_DATE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_1)->ShowWindow(SW_SHOW);
		m_bSelectKind = ePreiod;
	}break;
	default:
		break;
	}
}

void CDailyReport::LoadData(CString strStartData, CString strEndDate)
{
	m_stDailyData.Clear();
	m_nJamCnt = 0;

	g_DB.SELECT_DAILY_DATA_TBL(strStartData, strEndDate, &m_stDailyData);

	
	CString strErrStartData = _T(""), strErrEndData = _T("");

	strErrStartData.Format(_T("%s 00:00:00.000"), strStartData);
	strErrEndData.Format(_T("%s 23:59:59.999"), strEndDate);

	m_vDataListErrLog.clear();
	int nJam_OnLine_OffLine_View = g_DMCont.m_dmHandlerOpt.GB(BDM11_JAM_ONLINE_OFFLINE_VIEW);
	g_DB_LOG.SELECT_ERROR_LOG_TBL_LEVEL(strErrStartData, strErrEndData, eAlarm_Lv3, m_vDataListErrLog, TRUE, nJam_OnLine_OffLine_View);
	m_nJamCnt = (int)m_vDataListErrLog.size();

	LoadSpread();
}

void CDailyReport::OnBnClickedButtonApply()
{
	UpdateData(TRUE);

	COleDateTime oleDate;
	CString strStartTm = _T("");
	CString strEndTm = _T("");
	
	//Get Start Time&Date
	m_tStartDate.GetTime(oleDate);
	strStartTm.Format("%04d-%02d-%02d", oleDate.GetYear(), oleDate.GetMonth(), oleDate.GetDay());

	if (m_bSelectKind == ePreiod) {
		//Get End Time&Date
		m_tEndDate.GetTime(oleDate);
		strEndTm.Format("%04d-%02d-%02d", oleDate.GetYear(), oleDate.GetMonth(), oleDate.GetDay());
	}else{
		strEndTm.Format("%s", (LPCTSTR)strStartTm);
	}
	
	LoadData(strStartTm, strEndTm);
	InitData();
}

void CDailyReport::InitData()
{

	CString strTmp = _T("");
	int nJamRate =0;
	strTmp.Format(_T("%d"), m_stDailyData.nLoadCnt);
	m_SPRD_TotalData.SetTextMatrix(1, 1, strTmp);
	strTmp.Format(_T("%d"), m_stDailyData.nUnloadCnt);
	m_SPRD_TotalData.SetTextMatrix(2, 1, strTmp);
	strTmp.Format(_T("%d"), m_stDailyData.nPassCnt);
	m_SPRD_TotalData.SetTextMatrix(3, 1, strTmp);
	strTmp.Format(_T("%.02f%%"), m_stDailyData.nLoadCnt > 0 ?  ((double)m_stDailyData.nPassCnt / (double)m_stDailyData.nLoadCnt)*100.0 : 0.0 );
	m_SPRD_TotalData.SetTextMatrix(4, 1, strTmp);
	if (m_nJamCnt <= 0)
		nJamRate = m_stDailyData.nUnloadCnt;
	else
		nJamRate = (m_stDailyData.nUnloadCnt / m_nJamCnt);

	strTmp.Format(_T("%d / %d (%d)"), m_nJamCnt ,m_stDailyData.nUnloadCnt, nJamRate);
	m_SPRD_TotalData.SetTextMatrix(5, 1, strTmp);

	strTmp.Format(_T("%d"), m_stDailyData.nART_FT_Total_Cnt);
	m_SPRD_ARTYield.SetTextMatrix(1, 1, strTmp);
	strTmp.Format(_T("%d"), m_stDailyData.nART_FT_Pass_Cnt);
	m_SPRD_ARTYield.SetTextMatrix(1, 2, strTmp);
	strTmp.Format(_T("%.02f%%"), m_stDailyData.nART_FT_Total_Cnt > 0 ? ((double)m_stDailyData.nART_FT_Pass_Cnt / (double)m_stDailyData.nART_FT_Total_Cnt) * 100.0 : 0.0);
	m_SPRD_ARTYield.SetTextMatrix(1, 3, strTmp);

	strTmp.Format(_T("%d"), m_stDailyData.nART_RT1_Total_Cnt);
	m_SPRD_ARTYield.SetTextMatrix(2, 1, strTmp);
	strTmp.Format(_T("%d"), m_stDailyData.nART_RT1_Pass_Cnt);
	m_SPRD_ARTYield.SetTextMatrix(2, 2, strTmp);
	strTmp.Format(_T("%.02f%%"), m_stDailyData.nART_RT1_Total_Cnt > 0 ? ((double)m_stDailyData.nART_RT1_Pass_Cnt / (double)m_stDailyData.nART_RT1_Total_Cnt) * 100.0 : 0.0);
	m_SPRD_ARTYield.SetTextMatrix(2, 3, strTmp);

	strTmp.Format(_T("%d"), m_stDailyData.nART_RT2_Total_Cnt);
	m_SPRD_ARTYield.SetTextMatrix(3, 1, strTmp);
	strTmp.Format(_T("%d"), m_stDailyData.nART_RT2_Pass_Cnt);
	m_SPRD_ARTYield.SetTextMatrix(3, 2, strTmp);
	strTmp.Format(_T("%.02f%%"), m_stDailyData.nART_RT2_Total_Cnt > 0 ? ((double)m_stDailyData.nART_RT2_Pass_Cnt / (double)m_stDailyData.nART_RT2_Total_Cnt) * 100.0 : 0.0);
	m_SPRD_ARTYield.SetTextMatrix(3, 3, strTmp);

	InitPartCount();
}

void CDailyReport::OnBnClickedButtonExpert()
{
	UpdateData(TRUE);
	COleDateTime oleDate;

	//Get Start Time&Date
	m_tStartDate.GetTime(oleDate);

	CString strStartTm = _T("");
	CString strEndTm = _T("");

	strStartTm.Format("%04d-%02d-%02d", oleDate.GetYear(), oleDate.GetMonth(), oleDate.GetDay());

	if (m_bSelectKind == ePreiod) {
		//Get End Time&Date
		m_tEndDate.GetTime(oleDate);
		strEndTm.Format("%04d-%02d-%02d", oleDate.GetYear(), oleDate.GetMonth(), oleDate.GetDay());
	}
	else {
		strEndTm.Format(_T("%s"), strStartTm);
	}

	LoadData(strStartTm, strEndTm);

	// set folder & file
	CFile	DataFile;
	
	CString sFilePath = _T("D:/Techwing/LogExpData"), sFileName = _T(""), sDataFileName = _T("");

	sFileName.Format(_T("DailyReport_%s_%s"), (LPCTSTR)strStartTm, (LPCTSTR)strEndTm);
	sFileName.Replace(":", "_");
	sFileName.Replace(" ", "_");
	sDataFileName.Format("%s/%s.csv", (LPCTSTR)sFilePath, (LPCTSTR)sFileName);

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
	if (!DataFile.Open(sDataFileName, CFile::modeWrite | CFile::modeCreate))
	{
		CString sMsg;
		sMsg.Format("File Creation Fail!\n%s", (LPCTSTR)sDataFileName);
		AfxMessageBox(sMsg);
		return;
	}

	CString tmpMsg = _T(""), strMsg = _T("");
	CString FullData = _T("");
	
	tmpMsg.Format(_T("Date:,%s ~ %s"), (LPCTSTR)strStartTm, (LPCTSTR)strEndTm);
	FullData += tmpMsg;
	FullData += "\r\n";
	FullData += "\r\n";

	FullData += _T("Total Data,\r\n");
	FullData += _T(",Count,\r\n");
	tmpMsg.Format(_T("Load,%d,\r\n"),m_stDailyData.nLoadCnt);
	FullData += tmpMsg;

	tmpMsg.Format(_T("Unload,%d,\r\n"), m_stDailyData.nUnloadCnt);
	FullData += tmpMsg;

	tmpMsg.Format(_T("Pass,%d,\r\n"), m_stDailyData.nPassCnt);
	FullData += tmpMsg;

	tmpMsg.Format(_T("Yield,%.02f%%,\r\n"), m_stDailyData.nLoadCnt > 0 ? ((double)m_stDailyData.nPassCnt / (double)m_stDailyData.nLoadCnt)*100.0 : 0.0);
	FullData += tmpMsg;

	tmpMsg.Format(_T("Jam Count (Rate),%d / %d,\r\n"), m_nJamCnt, m_stDailyData.nUnloadCnt);
	FullData += tmpMsg;

	FullData += "\r\n";
	FullData += _T("Auto Retest Yield,\r\n");
	FullData += _T(",Total,Pass,Yield,\r\n");
	
	tmpMsg.Format(_T("FT,%d,%d,%.02f%%,\r\n"), m_stDailyData.nART_FT_Total_Cnt,m_stDailyData.nART_FT_Pass_Cnt, m_stDailyData.nART_FT_Total_Cnt > 0 ? ((double)m_stDailyData.nART_FT_Pass_Cnt / (double)m_stDailyData.nART_FT_Total_Cnt) * 100.0 : 0.0);
	FullData += tmpMsg;

	tmpMsg.Format(_T("RT1,%d,%d,%.02f%%,\r\n"), m_stDailyData.nART_RT1_Total_Cnt, m_stDailyData.nART_RT1_Pass_Cnt, m_stDailyData.nART_RT1_Total_Cnt > 0 ? ((double)m_stDailyData.nART_RT1_Pass_Cnt / (double)m_stDailyData.nART_RT1_Total_Cnt) * 100.0 : 0.0);
	FullData += tmpMsg;

	tmpMsg.Format(_T("RT2,%d,%d,%.02f%%,\r\n"), m_stDailyData.nART_RT2_Total_Cnt, m_stDailyData.nART_RT2_Pass_Cnt, m_stDailyData.nART_RT2_Total_Cnt > 0 ? ((double)m_stDailyData.nART_RT2_Pass_Cnt / (double)m_stDailyData.nART_RT2_Total_Cnt) * 100.0 : 0.0);
	FullData += tmpMsg;

	FullData += "\r\n";

	tmpMsg.Format(_T("Part Jam Count List,\r\n"));
	FullData += tmpMsg;

	tmpMsg.Format(_T("Part Name,Count,\r\n"));
	FullData += tmpMsg;

	tmpMsg.Format(_T("Stacker & Conveyor 1,%d,\r\n"),m_nPartJamCnt[ePos_Stacker_1]);
	FullData += tmpMsg;

	tmpMsg.Format(_T("Stacker & Conveyor 2,%d,\r\n"), m_nPartJamCnt[ePos_Stacker_2]);
	FullData += tmpMsg;

	tmpMsg.Format(_T("Stacker & Conveyor 3,%d,\r\n"), m_nPartJamCnt[ePos_Stacker_3]);
	FullData += tmpMsg;
	
	tmpMsg.Format(_T("Stacker & Conveyor 4,%d,\r\n"), m_nPartJamCnt[ePos_Stacker_4]);
	FullData += tmpMsg;

	tmpMsg.Format(_T("Stacker & Conveyor 5,%d,\r\n"), m_nPartJamCnt[ePos_Stacker_5]);
	FullData += tmpMsg;

	tmpMsg.Format(_T("Stacker & Conveyor 6,%d,\r\n"), m_nPartJamCnt[ePos_Stacker_6]);
	FullData += tmpMsg;

	tmpMsg.Format(_T("Stacker & Conveyor 7,%d,\r\n"), m_nPartJamCnt[ePos_Stacker_7]);
	FullData += tmpMsg;

	tmpMsg.Format(_T("Tray Unload PP,%d,\r\n"), m_nPartJamCnt[ePos_TrayPP_2]);
	FullData += tmpMsg;

	tmpMsg.Format(_T("Tray Load PP,%d,\r\n"), m_nPartJamCnt[ePos_TrayPP_1]);
	FullData += tmpMsg;

	tmpMsg.Format(_T("Clean Buffer Table 1,%d,\r\n"), m_nPartJamCnt[ePos_CleanBuf_1]);
	FullData += tmpMsg;

	tmpMsg.Format(_T("Clean Buffer Table 2,%d,\r\n"), m_nPartJamCnt[ePos_CleanBuf_2]);
	FullData += tmpMsg;

	tmpMsg.Format(_T("Tray Feeder,%d,\r\n"), m_nPartJamCnt[ePos_Feeder]);
	FullData += tmpMsg;

	tmpMsg.Format(_T("Load Table 1,%d,\r\n"), m_nPartJamCnt[ePos_LoadTbl_1]);
	FullData += tmpMsg;

	tmpMsg.Format(_T("Load Table 2,%d,\r\n"), m_nPartJamCnt[ePos_LoadTbl_2]);
	FullData += tmpMsg;

	tmpMsg.Format(_T("Load Table 3,%d,\r\n"), m_nPartJamCnt[ePos_LoadTbl_3]);
	FullData += tmpMsg;

	tmpMsg.Format(_T("Load Table 4,%d,\r\n"), m_nPartJamCnt[ePos_LoadTbl_4]);
	FullData += tmpMsg;

	tmpMsg.Format(_T("Test PP 1,%d,\r\n"), m_nPartJamCnt[ePos_TestPP_1]);
	FullData += tmpMsg;
	
	tmpMsg.Format(_T("Test PP 1,%d,\r\n"), m_nPartJamCnt[ePos_TestPP_2]);
	FullData += tmpMsg;

// 	tmpMsg.Format(_T("Test PP 1_X1,%d,\r\n"), m_nPartJamCnt[ePos_TestPP_1_X1]);
// 	FullData += tmpMsg;
// 
// 	tmpMsg.Format(_T("Test PP 1_X2,%d,\r\n"), m_nPartJamCnt[ePos_TestPP_1_X2]);
// 	FullData += tmpMsg;
// 
// 	tmpMsg.Format(_T("Test PP 2_X1,%d,\r\n"), m_nPartJamCnt[ePos_TestPP_2_X1]);
// 	FullData += tmpMsg;
// 
// 	tmpMsg.Format(_T("Test PP 2_X2,%d,\r\n"), m_nPartJamCnt[ePos_TestPP_2_X2]);
// 	FullData += tmpMsg;

	for (int i = 0; i < eMaxPressUnitCount; i++) {
		tmpMsg.Format(_T("Station %d ,%d,\r\n"), i+1, m_nPartJamCnt[ePos_Station_1+i]);
		FullData += tmpMsg;
	}
	
	tmpMsg.Format(_T("Transfer,%d,\r\n"), m_nPartJamCnt[ePos_Transfer]);
	FullData += tmpMsg;

	DataFile.Write(FullData.GetBuffer(FullData.GetLength() + 1), FullData.GetLength() + 1);
	DataFile.Close();
}

void CDailyReport::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnApplay.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnApplay.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnExpert.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnExpert.SetAlign(CButtonST::ST_ALIGN_VERT);
}

void CDailyReport::InitPartCount()
{
	int nMsgCount = (int)m_vDataListErrLog.size();
	
	ZeroMemory(m_nPartJamCnt, sizeof(m_nPartJamCnt));

	for (int i = 0; i < nMsgCount; i++)
	{
		CString strTmp = _T("");
	
		strTmp.Format("%s", m_vDataListErrLog[i].szPart);
		int nPartNum = m_vDataListErrLog[i].nPosition;
		if (ePos_Stacker_1 <= nPartNum && nPartNum <= ePos_Station_8) {
			m_nPartJamCnt[nPartNum]++;
		}
	}

	CString strTmp = _T("");

	for (int i = 1; i < ePos_MaxPosition; i++) {
		strTmp.Format(_T("%d"), m_nPartJamCnt[i]);
		m_SPRD_PartJamCnt.SetTextMatrix(i, 1, strTmp);
	}
}

void CDailyReport::InitSpread()
{
	m_List.InsertColumn(0, "ALID", 0, 50);		// Alarm Code
	m_List.InsertColumn(1, "PART", 0, 90);		// Alarm Part
	m_List.InsertColumn(2, "MESSAGE", 0, 850);	// Alarm Message
	m_List.InsertColumn(3, "Occ Time", 0, 200);	// Occurred Time

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | 0x00004000);
}

void CDailyReport::LoadSpread() 
{
	m_List.DeleteAllItems();

	for (int i = 0; i < (int)m_vDataListErrLog.size(); i++) {
		CString str[4];

		str[0].Format("%s", m_vDataListErrLog[i].szEQP_ALID);
		str[1].Format("%s", m_vDataListErrLog[i].szPart);
		str[2] = m_vDataListErrLog[i].szMessage;
		str[3] = m_vDataListErrLog[i].strOccurred_Time;

		m_List.AddItem(str[0]);

		for (int j = 1; j < 4; j++)
		{
			m_List.SetItemText(i, j, str[j]);
		}
	}

	m_List.Complete();
}

void CDailyReport::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


void CDailyReport::OnBnClickedCheckDailyReportJamOnOffline()
{
	BOOL bFlag = FALSE;
	if (m_ChkOnLine_OffLine_View.GetCheck())
	{
		bFlag = TRUE;
	}
	else {
		bFlag = FALSE;
	}

	g_DMCont.m_dmHandlerOpt.SB(BDM11_JAM_ONLINE_OFFLINE_VIEW, bFlag);

	Uploaddata(FALSE);
}


LRESULT CDailyReport::OnListDoubleClick(WPARAM wParam, LPARAM lParam)
{
	int nItem = (int)(lParam >> 16);
	int nSubItem = (int)(lParam & 0xFFFF);

	if (nItem < 0 || nSubItem < 0)
		return 0;

	CDlgAlarmZoom dlg;
	strcpy_s(dlg.m_stDataListErrLog.szEQP_ALID, sizeof(dlg.m_stDataListErrLog.szEQP_ALID), m_List.GetItemText(nItem, 0));
	strcpy_s(dlg.m_stDataListErrLog.szPart, sizeof(dlg.m_stDataListErrLog.szPart), m_List.GetItemText(nItem, 1));
	strcpy_s(dlg.m_stDataListErrLog.szMessage, sizeof(dlg.m_stDataListErrLog.szMessage), m_List.GetItemText(nItem, 2));
	dlg.m_stDataListErrLog.strOccurred_Time.Format("%s", m_List.GetItemText(nItem, 3));
	dlg.DoModal();

	return 0;
}