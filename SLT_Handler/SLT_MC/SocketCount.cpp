// SocketCount.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SocketCount.h"
#include "afxdialogex.h"


// CSocketCount 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSocketCount, CDialog)

enum eCol {
	eCol_SocketID =1,
	eCol_Station,
	eCol_Site,
	eCol_TD,
	eCol_TotalTD,
	eCol_Limit,
	eCol_Max = 7,
};

CSocketCount::CSocketCount(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_SOCKET_COUNT, pParent)
{
	m_nMaxOnlineRowNum = 0;
	m_stCountInfo.clear();
	m_nLastSel_Row = 0;
	m_bDataDelete = FALSE;
	m_bClick = FALSE;

	m_bAddLimit = FALSE;
	m_nLimit = 0;
	m_nAddLimit = 0;
	m_szPathFileName[MAX_PATH] = {0,};

	m_nTesterCount = 0;
	m_nSocketCount = 0;
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);

	if (nTesterOption != eTester_Dual)
	{
		m_nTesterCount = MAX_TESTER_SINGLE;
		m_nSocketCount = STATION_MAX_PARA;
	}		
	else
	{
		m_nTesterCount = MAX_TESTER_DUAL;
		m_nSocketCount = STATION_MAX_PARA/2;
	}
		
}

CSocketCount::~CSocketCount()
{
}

void CSocketCount::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPREAD_COUNT, m_SprdSocketCount);
	DDX_Control(pDX, IDC_BUTTON_SELECT_DELETE, m_BtnSelectDelete);
	DDX_Control(pDX, IDC_BUTTON_ALL_DELETE, m_BtnAllDelete);
	DDX_Control(pDX, IDC_BUTTON_FIND_ID, m_BtnFind);
	DDX_Control(pDX, IDC_BUTTON_ALL_DATA_SHOW, m_BtnAllDataShow);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_BtnSave);
	DDX_Control(pDX, IDCANCEL, m_BtnExit);
	DDX_Control(pDX, IDC_SPREAD_COUNT_OFFLINE, m_SprdSocketCount_Offline);
	DDX_Control(pDX, IDC_BUTTON_SET_ADD_LIMIT, m_BtnAddLimit);
	DDX_Control(pDX, IDC_BUTTON_ALL_DATA_UPDATE, m_BtnAllDataUpdate);
	

	DDX_Text(pDX, IDC_EDIT_LIMIT_COUNT, m_nLimit);
	DDX_Text(pDX, IDC_EDIT_ADD_LIMIT_COUNT, m_nAddLimit);
}


BOOL CSocketCount::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if ((pMsg->message == WM_SYSKEYDOWN) && (pMsg->wParam == VK_F4))
	{
		return TRUE;
	}
	if ((pMsg->message == WM_KEYDOWN || pMsg->message == WM_LBUTTONDBLCLK) && GetFocus() == GetDlgItem(IDC_SPREAD_COUNT))
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CSocketCount, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_DELETE, &CSocketCount::OnBnClickedButtonSelectDelete)
	ON_BN_CLICKED(IDC_BUTTON_ALL_DELETE, &CSocketCount::OnBnClickedButtonAllDelete)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CSocketCount::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_FIND_ID, &CSocketCount::OnBnClickedButtonFindId)
	ON_BN_CLICKED(IDC_BUTTON_ALL_DATA_SHOW, &CSocketCount::OnBnClickedButtonAllDataShow)
	ON_EN_SETFOCUS(IDC_EDIT_LIMIT_COUNT, &CSocketCount::OnEnSetfocusEditLimitCount)
	ON_EN_SETFOCUS(IDC_EDIT_ADD_LIMIT_COUNT, &CSocketCount::OnEnSetfocusEditAddLimitCount)
	ON_BN_CLICKED(IDC_BUTTON_SET_ADD_LIMIT, &CSocketCount::OnBnClickedButtonSetAddLimit)
	ON_BN_CLICKED(IDC_BUTTON_ALL_DATA_UPDATE, &CSocketCount::OnBnClickedButtonAllDataUpdate)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


BOOL CSocketCount::OnInitDialog()
{
	CDialog::OnInitDialog();
	g_DMCont.m_dmEQP.SB(BDM0_OPEN_SLT_UI_DLG_HISTORY, TRUE);
	Initialize();
	OnBtnGUI();
	return TRUE;
}

void CSocketCount::Initialize()
{
	CString strFilePath = _T("");

	CStdioFile f;
	CString strTitle = _T("");
	CString strFileLine = _T("");
	CString strFileSocketID = _T("");
	CString strFileTD = _T("");
	CString strFIleTotalTD = _T("");
	CString strLimit = _T("");

	int nCount = 0;
	//ST_SOCKET_CONTACT_COUNT stCountInfo;
	BOOL bFileExist = TRUE;
	CString strDir = _T("D:\\Techwing\\Socket_Contact");

	char szEqpName[64] = { 0, };
	g_DMCont.m_dmEQP.GS(SDM0_HANDER_ID, szEqpName, sizeof(szEqpName));

	m_stCountInfo.clear();

	if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(strDir, NULL);
	}

	strFilePath.Format(_T("%s\\%s.csv"), strDir, szEqpName);
	//file 존재하는지 확인
	if (!f.Open(strFilePath, CFile::modeReadWrite))
	{
		//f.Close();
		bFileExist = FALSE;
	}

	if (bFileExist) {
		f.SeekToBegin();

		while (f.ReadString(strFileLine) != NULL)
		{
			ST_SOCKEID_INFO stInfo;

			AfxExtractSubString(strFileSocketID, strFileLine, 0, ',');

			if (strFileSocketID.Compare("Socket ID") != 0) {
				AfxExtractSubString(strFileTD, strFileLine, 1, ',');
				AfxExtractSubString(strFIleTotalTD, strFileLine, 2, ',');
				AfxExtractSubString(strLimit, strFileLine, 3, ',');

				stInfo.strSocketID = strFileSocketID;
				stInfo.nTD = atoi(strFileTD);
				stInfo.nTotalTD = atoi(strFIleTotalTD);
				stInfo.nLimit = atoi(strLimit);

				m_stCountInfo.vSocketCountactInfo.push_back(stInfo);
				nCount++;
			}
			Sleep(1);
		}
		m_stCountInfo.nIndexCount = nCount;
		f.Close();
	}

	StationNumSetting();

	m_nLimit = g_DMCont.m_dmEQP.GN(NDM0_SocketContactCntLimit);
	m_nAddLimit = g_DMCont.m_dmEQP.GN(NDM0_AddSocketContactLimit);

	DisplaySpread(m_stCountInfo);

	ZeroMemory(m_szPathFileName, sizeof(m_szPathFileName));
	char szPathTemp[512] = { 0, };
	::GetCurrentDirectory(sizeof(szPathTemp), szPathTemp);
	sprintf_s(m_szPathFileName, sizeof(m_szPathFileName), "%s\\sys_data\\%s", szPathTemp, SZ_DRIVER_FILE_NAME);

	UpdateData(FALSE);
}

void CSocketCount::DisplaySpread(ST_SOCKET_CONTACT_COUNT stCountInfo)
{
	m_SprdSocketCount.SetMaxRows(stCountInfo.nIndexCount);
	m_SprdSocketCount_Offline.SetMaxRows(stCountInfo.nIndexCount);

	int nOnlineRow = 0, nOfflineRow = 0;
	CString strTemp = _T("");

	for (int i = 0; i < stCountInfo.nIndexCount; i++) {
		if (stCountInfo.vSocketCountactInfo[i].nStation != -1 && stCountInfo.vSocketCountactInfo[i].nSite != -1) {

			strTemp = _T("");
			//Socket ID
			m_SprdSocketCount.SetTextMatrix(nOnlineRow + 1, eCol_SocketID, stCountInfo.vSocketCountactInfo[i].strSocketID);

			//Station
			strTemp.Format(_T("%d"), stCountInfo.vSocketCountactInfo[i].nStation);
			m_SprdSocketCount.SetTextMatrix(nOnlineRow + 1, eCol_Station, strTemp);

			//Site 
			strTemp.Format(_T("%d"), stCountInfo.vSocketCountactInfo[i].nSite);
			m_SprdSocketCount.SetTextMatrix(nOnlineRow + 1, eCol_Site, strTemp);

			//TD
			strTemp.Format(_T("%d"), stCountInfo.vSocketCountactInfo[i].nTD);
			m_SprdSocketCount.SetTextMatrix(nOnlineRow + 1, eCol_TD, strTemp);

			//Total TD
			strTemp.Format(_T("%d"), stCountInfo.vSocketCountactInfo[i].nTotalTD);
			m_SprdSocketCount.SetTextMatrix(nOnlineRow + 1, eCol_TotalTD, strTemp);

			strTemp.Format(_T("%d"), stCountInfo.vSocketCountactInfo[i].nLimit);
			m_SprdSocketCount.SetTextMatrix(nOnlineRow + 1, eCol_Limit, strTemp);
			
			OLE_COLOR color = COLOR_WWHITE;

			if ( stCountInfo.vSocketCountactInfo[i].nTD > stCountInfo.vSocketCountactInfo[i].nLimit) {
				color = COLOR_YELLOW;
			}

			for (int nCol = eCol_SocketID; nCol < eCol_Max; nCol++) {
				m_SprdSocketCount.SetCol(nCol);
				m_SprdSocketCount.SetBackColor(color);
			}

			nOnlineRow++;
		}
		else {

			strTemp = _T("");
			//Socket ID
			m_SprdSocketCount_Offline.SetTextMatrix(nOfflineRow + 1, 1, stCountInfo.vSocketCountactInfo[i].strSocketID);

			//TD
			strTemp.Format(_T("%d"), stCountInfo.vSocketCountactInfo[i].nTD);
			m_SprdSocketCount_Offline.SetTextMatrix(nOfflineRow + 1, 2, strTemp);

			//Total TD
			strTemp.Format(_T("%d"), stCountInfo.vSocketCountactInfo[i].nTotalTD);
			m_SprdSocketCount_Offline.SetTextMatrix(nOfflineRow + 1, 3, strTemp);

			nOfflineRow++;
		}
	}

	m_nMaxOnlineRowNum = nOnlineRow;

	m_SprdSocketCount.SetMaxRows(nOnlineRow);
	m_SprdSocketCount_Offline.SetMaxRows(nOfflineRow);	
}

void CSocketCount::OnBtnGUI()
{
	m_BtnSelectDelete.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnSelectDelete.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnAllDelete.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnAllDelete.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnFind.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnFind.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnAllDataShow.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnAllDataShow.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnExit.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnExit.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnAddLimit.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnAddLimit.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnAllDataUpdate.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnAllDataUpdate.SetAlign(CButtonST::ST_ALIGN_VERT);
}

void CSocketCount::OnBnClickedButtonSelectDelete()
{
	if (m_bClick) {
		CString strLog = _T("");
		int nSelectSel = m_SprdSocketCount.GetSelBlockRow();

		m_SprdSocketCount.SetTextMatrix(nSelectSel, 4, "0");
		CString strSocketID = m_SprdSocketCount.GetTextMatrix(nSelectSel, 1);

		for (int i = 0; i < m_stCountInfo.nIndexCount; i++)
		{
			if (m_stCountInfo.vSocketCountactInfo[i].strSocketID.Compare(strSocketID) == 0 && m_stCountInfo.vSocketCountactInfo[i].nTD != 0) {
				m_stCountInfo.vSocketCountactInfo[i].nTD = 0;
				m_bDataDelete = TRUE;
			}
		}
		strLog.Format(_T("Select Delete Button Click [ %s ]"),strSocketID);
		MakeLog(strLog);
	}
}


void CSocketCount::OnBnClickedButtonAllDelete()
{
	MakeLog("All TD Delete Button Click");

	for (int i = 0; i < m_nMaxOnlineRowNum; i++) {
		m_SprdSocketCount.SetTextMatrix(i+1, 4, "0");
	}

	for (int nDataCnt = 0; nDataCnt < m_stCountInfo.nIndexCount; nDataCnt++) {
		if (m_stCountInfo.vSocketCountactInfo[nDataCnt].nStation != -1 && m_stCountInfo.vSocketCountactInfo[nDataCnt].nSite != -1) {
			m_stCountInfo.vSocketCountactInfo[nDataCnt].nTD = 0;
		}
	}

	m_bDataDelete = TRUE;
}


void CSocketCount::OnBnClickedButtonSave()
{
	UpdateData(TRUE);

	MakeLog("Save Button Click");

	BOOL bSaveData = FALSE;

	CConfigData ConfigPathData(m_szPathFileName);

	if (g_DMCont.m_dmEQP.GN(NDM0_SocketContactCntLimit) != m_nLimit)
	{
		g_DMCont.m_dmEQP.SN(NDM0_SocketContactCntLimit, m_nLimit);
		ConfigPathData.Set("SocketContactCnt", "Limit", m_nLimit);
		
		for (int i = 0; i < m_stCountInfo.nIndexCount; i++) {
			m_stCountInfo.vSocketCountactInfo[i].nLimit = m_nLimit;
		}
		bSaveData = TRUE;
	}

	if (g_DMCont.m_dmEQP.GN(NDM0_AddSocketContactLimit) != m_nAddLimit) {
		g_DMCont.m_dmEQP.SN(NDM0_AddSocketContactLimit, m_nAddLimit);
		ConfigPathData.Set("SocketContactCnt", "Add Limit", m_nAddLimit);
	}

	if (m_bDataDelete || bSaveData || m_bAddLimit) {
		CString strFilePath = _T("");

		CStdioFile f;
		CString strFileLine = _T("");
		int nCount = 0;
		BOOL bFileExist = TRUE;
		CString strDir = _T("D:\\Techwing\\Socket_Contact");

		char szEqpName[64] = { 0, };
		g_DMCont.m_dmEQP.GS(SDM0_HANDER_ID, szEqpName, sizeof(szEqpName));

		if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
			CreateDirectory(strDir, NULL);
		}

		strFilePath.Format(_T("%s\\%s.csv"), strDir, szEqpName);
		//file 존재하는지 확인
		if (!f.Open(strFilePath, CFile::modeReadWrite))
		{
			//f.Close();
			bFileExist = FALSE;
		}

		if (bFileExist) {
			f.Close();

			remove(strFilePath);

		}

		CStdioFile file;
		//File 생성
		if (!file.Open(strFilePath, CFile::modeReadWrite | CFile::modeCreate))
		{
			AfxMessageBox(_T("Socket ID Count File Create Fail!!"));
			return;
		}

		CString strTmp = _T("");
		strFileLine.Empty();

		strTmp += _T("Socket ID, TD, Total TD, Limit");
		strTmp += _T("\n");
		for (int i = 0; i < m_stCountInfo.nIndexCount; i++) {
			strFileLine.Format(_T("%s,%d,%d,%d"), m_stCountInfo.vSocketCountactInfo[i].strSocketID, m_stCountInfo.vSocketCountactInfo[i].nTD, m_stCountInfo.vSocketCountactInfo[i].nTotalTD,m_stCountInfo.vSocketCountactInfo[i].nLimit);
			strFileLine += _T("\n");
			strTmp += strFileLine;
		}

		file.SeekToEnd();
		file.WriteString(strTmp);
		file.Close();

		DisplaySpread(m_stCountInfo);

		m_bDataDelete = FALSE;
		bSaveData = FALSE;
		m_bAddLimit = FALSE;
	}
}

void CSocketCount::OnBnClickedButtonFindId()
{
	CString strFindId = _T("");

	GetDlgItem(IDC_EDIT_FIND_ID)->GetWindowText(strFindId);

	CString strLog = _T("");

	strLog.Format(_T("Find Button Click [ %s ] "),strFindId);
	MakeLog(strLog);

	ST_SOCKET_CONTACT_COUNT stCountInfo;
	int nCount = 0;

	for (int i = 0; i < m_stCountInfo.nIndexCount; i++)
	{
		if (m_stCountInfo.vSocketCountactInfo[i].strSocketID.Compare(strFindId) == 0) {
			stCountInfo.vSocketCountactInfo.push_back(m_stCountInfo.vSocketCountactInfo[i]);
			nCount++;
		}
	}

	if (nCount > 0) {
		stCountInfo.nIndexCount = nCount;
		DisplaySpread(stCountInfo);
	}
	else {
		m_SprdSocketCount.SetMaxRows(0);
	}
}
BEGIN_EVENTSINK_MAP(CSocketCount, CDialogEx)
	ON_EVENT(CSocketCount, IDC_SPREAD_COUNT, 5, CSocketCount::ClickSpreadCount, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


void CSocketCount::ClickSpreadCount(long Col, long Row)
{
	m_SprdSocketCount.SetRow(m_nLastSel_Row);

	OLE_COLOR color;
	CString strLimit = m_SprdSocketCount.GetTextMatrix(m_nLastSel_Row, eCol_Limit);
	CString strTD = m_SprdSocketCount.GetTextMatrix(m_nLastSel_Row, eCol_TD);

	if (atoi(strTD) > atoi(strLimit)) {
		color = COLOR_YELLOW;
	}
	else {
		color = COLOR_WWHITE;
	}

	for (int nCol = eCol_SocketID; nCol < eCol_Max; nCol++) {
		m_SprdSocketCount.SetCol(nCol);
		m_SprdSocketCount.SetBackColor(color);
	}
	
	m_SprdSocketCount.SetRow(Row);
	for (int nCol = eCol_SocketID; nCol < eCol_Max; nCol++) {
		m_SprdSocketCount.SetCol(nCol);
		m_SprdSocketCount.SetBackColor(COLOR_RED);
	}
	m_nLastSel_Row = Row;

	m_bClick = TRUE;
}


void CSocketCount::OnBnClickedButtonAllDataShow()
{
	MakeLog(_T("All Data Show Button Click"));
	DisplaySpread(m_stCountInfo);
}

void CSocketCount::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = { 0, };
	if (fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_SYSTEM), Debug, "", __LINE__, NULL, tmpMsg);
}

bool compare(ST_SOCKEID_INFO a, ST_SOCKEID_INFO b) {
	
	if (a.nStation < b.nStation) {
		return TRUE;
	}
	else if (a.nStation == b.nStation) {
		if (a.nSite < b.nSite) {
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	return FALSE;
}

void CSocketCount::StationNumSetting()
{
	CString strTemp = _T("");
	BOOL bIsOnline = FALSE;
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	int nOnlineRow = 0, nOfflineRow = 0;
	CTesterStatus cTesterStatus;
	//CAgentStatus cAgentStatus;
	for (int nSocketIdx = 0; nSocketIdx < m_stCountInfo.nIndexCount; nSocketIdx++) {
		bIsOnline = FALSE;

		for (int nTesterIdx = 0; nTesterIdx < m_nTesterCount; nTesterIdx++) { // ID 탐색
			BOOL bRegister = FALSE, bConnect =FALSE;
			if (nTesterOption != eTester_Agent){
				bRegister = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + nTesterIdx);
				bConnect = g_AppleTestIF.IsConnected(nTesterIdx + 1);
			}
			else{
				bRegister = g_DMCont.m_dmEQP.GB(BDM0_AGENT_CONNECTION_1 + nTesterIdx);
				//bConnect = g_AppleAgentIF.IsConnected(nTesterIdx + 1);
			}

			if (bRegister == TRUE && bConnect == TRUE)
			{
				if (nTesterOption != eTester_Agent) 
					cTesterStatus = g_AppleTestIF.GetTesterStatus(nTesterIdx + 1);
				else
					//cAgentStatus = g_AppleAgentIF.GetAgentStatus(nTesterIdx + 1);

				for (int i = 0; i < m_nSocketCount; i++) {
					CString strSocketID = _T("");

					if (nTesterOption != eTester_Agent)
					{						
						if (nTesterOption == eTester_EachSocket)
						{
							strSocketID.Format("%d", cTesterStatus.m_nRegiDutIdentityNo[i]);	// 물리 dut 넘버 지정.
						}
						else
						{
							strSocketID = cTesterStatus.m_sSocketID_Orginal[i];
						}
					}
					else
						//strSocketID.Format("%d_%d", cAgentStatus.m_nStationID, i+1);

					if (m_stCountInfo.vSocketCountactInfo[nSocketIdx].strSocketID.Compare(strSocketID) == 0) {
						if (nTesterOption != eTester_Dual) {
							m_stCountInfo.vSocketCountactInfo[nSocketIdx].nStation = nTesterIdx + 1;
							m_stCountInfo.vSocketCountactInfo[nSocketIdx].nSite = i + 1;
						}
						else
						{ 
							m_stCountInfo.vSocketCountactInfo[nSocketIdx].nStation = (nTesterIdx / 2) + 1;
							if(nTesterIdx % 2 == 0)
								m_stCountInfo.vSocketCountactInfo[nSocketIdx].nSite = i + 1;
							else
								m_stCountInfo.vSocketCountactInfo[nSocketIdx].nSite = m_nSocketCount + i + 1;
						}
						
						bIsOnline = TRUE;
						break;
					}
				}
			}
			if (bIsOnline) {
				break;
			}
		}

		if (bIsOnline == FALSE) {
			//Offline Socket List로 
			
			m_stCountInfo.vSocketCountactInfo[nSocketIdx].nStation = -1;
			m_stCountInfo.vSocketCountactInfo[nSocketIdx].nSite = -1;
		}
	}

	sort(m_stCountInfo.vSocketCountactInfo.begin(), m_stCountInfo.vSocketCountactInfo.end(), compare);
}

void CSocketCount::OnEnSetfocusEditLimitCount()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	CString strVal = _T("");
	GetDlgItem(IDC_EDIT_LIMIT_COUNT)->GetWindowText(strVal);

	sprintf_s(szVal, sizeof(szVal), "%s", strVal.GetString());

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Limit Count", "100000", "0");

	if (nRet == RETURN_OK)
	{
		GetDlgItem(IDC_EDIT_LIMIT_COUNT)->SetWindowText(szRetVal);
	}
}

void CSocketCount::OnEnSetfocusEditAddLimitCount()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	CString strVal = _T("");
	GetDlgItem(IDC_EDIT_ADD_LIMIT_COUNT)->GetWindowText(strVal);
	
	sprintf_s(szVal, sizeof(szVal), "%s", strVal.GetString());

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Add Limit Count", "9999999", "0");

	if (nRet == RETURN_OK)
	{
		GetDlgItem(IDC_EDIT_ADD_LIMIT_COUNT)->SetWindowText(szRetVal);
	}
}

void CSocketCount::OnBnClickedButtonSetAddLimit()
{
	UpdateData(TRUE);

	if (m_bClick) {
		CString strLog = _T("");
		int nSelectSel = m_SprdSocketCount.GetSelBlockRow();

		CString strLimit = m_SprdSocketCount.GetTextMatrix(nSelectSel, eCol_Limit);

		int nLimit = atoi(strLimit) + m_nAddLimit;

		strLimit.Format(_T("%d"), nLimit);
		m_SprdSocketCount.SetTextMatrix(nSelectSel, eCol_Limit, strLimit);

		CString strSocketID = m_SprdSocketCount.GetTextMatrix(nSelectSel, 1);

		for (int i = 0; i < m_stCountInfo.nIndexCount; i++)
		{
			if (m_stCountInfo.vSocketCountactInfo[i].strSocketID.Compare(strSocketID) == 0 ) {
				m_stCountInfo.vSocketCountactInfo[i].nLimit = nLimit;
			}
		}

		m_bAddLimit = TRUE;

		strLog.Format(_T("Add Selected Socket_ID Limit [ Socket ID : %s ]"), strSocketID);
		MakeLog(strLog);
	}
}


void CSocketCount::OnBnClickedButtonAllDataUpdate()
{
	if (g_TaskSystemCtrl.GetSocketIDContactCnt() > 0) {
		g_TaskSystemCtrl.MakeSocketIDContactCnt();
	}
	g_TaskSystemCtrl.ClearSocketIDContactCnt();

	Initialize();
	//}	
}


void CSocketCount::OnDestroy()
{
	CDialog::OnDestroy();
	g_DMCont.m_dmEQP.SB(BDM0_OPEN_SLT_UI_DLG_HISTORY, FALSE);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
