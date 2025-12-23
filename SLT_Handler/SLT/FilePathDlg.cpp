// FilePathDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "FilePathDlg.h"
#include "afxdialogex.h"
#include "SLTView.h"
#include "MainFrm.h"

// FilePathDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFilePathDlg, CDialog)

CFilePathDlg::CFilePathDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFilePathDlg::IDD, pParent)
{
	m_strSummaryPath = _T("");
	m_strBeforeSummaryPath = _T("");
	m_strBeforeNetWorkSummaryPath = _T("");
	m_strBatchPath = _T("");
	m_strBeforeBatchPath = _T("");
	m_strLotInfoPath = _T("");
	m_strBeforeLotInfoPath = _T("");
	m_strTesterBatchPath = _T("");
	m_str2didCsvPath = _T("");
	m_strBefore2didCsvPath = _T("");
	m_strHitSumamryPath = _T("");
	m_strBeforeHitSumamryPath = _T("");

	m_bBeforeCheckBatchFileUse = FALSE;
	m_bBeforeTesterVersionOff = FALSE;
	m_bBeforeTesterCheckBatchFileUse = FALSE;
}

CFilePathDlg::~CFilePathDlg()
{
}

void CFilePathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SUMMARY_PATH, m_strSummaryPath);
	DDX_Text(pDX, IDC_EDIT_NETWORK_SUMMARY_PATH, m_strNetWorkSummaryPath);
	DDX_Text(pDX, IDC_EDIT_2DID_CSV_PATH, m_str2didCsvPath);
	DDX_Text(pDX, IDC_EDIT_BATCH_PATH, m_strBatchPath);
	DDX_Text(pDX, IDC_EDIT_TESTER_BATCH_PATH, m_strTesterBatchPath);
	DDX_Text(pDX, IDC_EDIT_LOTINFO_PATH, m_strLotInfoPath);
	DDX_Text(pDX, IDC_EDIT_HIT_SUMMARY_PATH, m_strHitSumamryPath);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_BtnSave);
	DDX_Control(pDX, IDC_BUTTON_PATH, m_BtnPath);
	DDX_Control(pDX, IDC_BUTTON_BATCH_PATH, m_BtnBatchPath);
	DDX_Control(pDX, IDC_BUTTON_LOTINFO_PATH, m_BtnLotInfoPath);
	DDX_Control(pDX, IDC_CHECK_BATCH_FILE, m_CheckBatchFileUse);
	DDX_Control(pDX, IDC_CHECK_TESTER_VERSION_CHECK_OFF, m_CheckTesterVersionOff);
	DDX_Control(pDX, IDC_CHECK_TESTER_BATCH_FILE_USE, m_CheckTesterBatchFileUse);
}


BEGIN_MESSAGE_MAP(CFilePathDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CFilePathDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_PATH, &CFilePathDlg::OnBnClickedButtonPath)
	ON_BN_CLICKED(IDC_BUTTON_BATCH_PATH, &CFilePathDlg::OnBnClickedButtonBatchPath)
	ON_BN_CLICKED(IDC_BUTTON_LOTINFO_PATH, &CFilePathDlg::OnBnClickedButtonLotinfoPath)
	ON_BN_CLICKED(IDC_BUTTON1, &CFilePathDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_TESTER_BATCH_PATH, &CFilePathDlg::OnBnClickedButtonTesterBatchPath)
	ON_BN_CLICKED(IDC_BUTTON_NETWORK_PATH, &CFilePathDlg::OnBnClickedButtonNetworkPath)
	ON_BN_CLICKED(IDC_BUTTON_2DID_CSV_PATH, &CFilePathDlg::OnBnClickedButton2didCsvPath)
	ON_BN_CLICKED(IDC_BUTTON_HIT_SUMMARY_PATH, &CFilePathDlg::OnBnClickedButtonHitSummaryPath)
END_MESSAGE_MAP()

BOOL CFilePathDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	OnBtnGUI();
	SetDisplayUserData();
	MakeLog("[File Path Dialog Show]");
	return TRUE;
}

void CFilePathDlg::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnPath.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnPath.SetAlign(CButtonST::ST_ALIGN_VERT);
	
	m_BtnBatchPath.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnBatchPath.SetAlign(CButtonST::ST_ALIGN_VERT);
	
	m_BtnLotInfoPath.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnLotInfoPath.SetAlign(CButtonST::ST_ALIGN_VERT);
	
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CSLTView* pSltView = (CSLTView*)pMainFrm->GetActiveView();

	int nUserLevel = pSltView->m_nLogInLv;
	int nRetLevel = 0;

	// 이전 선택이 Use였고, 현재 변경 된 Status가 Not Use 일 경우만 판단 & 사용자 Level이 Engineer 이하 등급일 경우[현재 로그인 상태 포함]

	BOOL bRet = g_LibCommCtrl.ShowLogInBox(nUserLevel, &nRetLevel);

	if (RETURN_OK == bRet) {
		if (nRetLevel >= eUSERLEVEL_ENGINEER) {
// 			GetDlgItem(IDC_STATIC_TESTER_BATCH_FILE)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_STATIC_TESTER_BATCH)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_EDIT_TESTER_BATCH_PATH)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_BUTTON_TESTER_BATCH_PATH)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_CHECK_TESTER_BATCH_FILE_USE)->ShowWindow(SW_SHOW);			
		}
		else {
// 			GetDlgItem(IDC_STATIC_TESTER_BATCH_FILE)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_STATIC_TESTER_BATCH)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_EDIT_TESTER_BATCH_PATH)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_BUTTON_TESTER_BATCH_PATH)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_CHECK_TESTER_BATCH_FILE_USE)->ShowWindow(SW_HIDE);
			
		}
	}
	else
	{
		CDialog::OnOK();
	}
	

}

void CFilePathDlg::SetDisplayUserData()
{
	char szSummaryPathBuff[MAX_PATH]={0,};
	char szBatchPathBuff[MAX_PATH]={0,}, szLotInfoPathBuff[MAX_PATH] ={0,}, szTesterBatchPathBuff[MAX_PATH] = { 0, };
	char szFilePath[MAX_PATH] = {0,};
	char szTemp[MAX_PATH]={0,};
	char szNetWorkSummaryPathBuff[MAX_PATH] = { 0, };
	char sz2didCsvPath[MAX_PATH] = { 0, };
	char szHitSummaryPath[MAX_PATH] = { 0, };
	//::GetCurrentDirectory(sizeof(szTemp), szTemp);
	//
	//sprintf_s(szFilePath, sizeof( szFilePath ), "%s\\sys_data\\%s",szTemp, SZ_FILE_PATH);
	//
	//CConfigData CConfig(szFilePath);
	//CConfig.GetString("FILE PATH","Summary File Path", "", szSummaryPathBuff);
	//CConfig.GetString("FILE PATH","Batch File Path", "", szBatchPathBuff);
	//CConfig.GetString("FILE PATH","LotInfo File Path","", szLotInfoPathBuff);

	g_DMCont.m_dmHandlerOpt.GS(SDM11_SummaryFile_Path,szSummaryPathBuff,sizeof(szSummaryPathBuff));
	g_DMCont.m_dmHandlerOpt.GS(SDM11_BatchFile_Path, szBatchPathBuff, sizeof(szBatchPathBuff));
	g_DMCont.m_dmHandlerOpt.GS(SDM11_LotInfoFile_Path, szLotInfoPathBuff, sizeof(szLotInfoPathBuff));
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Tester_BatchFile_Path, szTesterBatchPathBuff, sizeof(szTesterBatchPathBuff));
	g_DMCont.m_dmHandlerOpt.GS(SDM11_NetWorkSummaryFile_Path, szNetWorkSummaryPathBuff, sizeof(szNetWorkSummaryPathBuff));
	g_DMCont.m_dmHandlerOpt.GS(SDM11_2did_Csv_File_Path, sz2didCsvPath, sizeof(sz2didCsvPath));
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Hit_Summary_File_Path, szHitSummaryPath, sizeof(szHitSummaryPath));
	
	int nCheck = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_BatchFile_Use); 
	m_CheckBatchFileUse.SetCheck(nCheck);

	nCheck = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TesterBatchFile_Use);
	m_CheckTesterBatchFileUse.SetCheck(nCheck);
	

	int nTesterVersionCheckOff= g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TESTER_VERSION_CHECK_OFF);
	m_CheckTesterVersionOff.SetCheck(nTesterVersionCheckOff);


	SetDlgItemText(IDC_EDIT_SUMMARY_PATH,szSummaryPathBuff);
	SetDlgItemText(IDC_EDIT_BATCH_PATH,szBatchPathBuff);
	SetDlgItemText(IDC_EDIT_LOTINFO_PATH,szLotInfoPathBuff);
	SetDlgItemText(IDC_EDIT_TESTER_BATCH_PATH, szTesterBatchPathBuff);
	SetDlgItemText(IDC_EDIT_NETWORK_SUMMARY_PATH, szNetWorkSummaryPathBuff);
	SetDlgItemText(IDC_EDIT_2DID_CSV_PATH, sz2didCsvPath);
	SetDlgItemText(IDC_EDIT_HIT_SUMMARY_PATH, szHitSummaryPath);

	m_strSummaryPath = szSummaryPathBuff;	
	m_strBatchPath = szBatchPathBuff;
	m_strLotInfoPath = szLotInfoPathBuff;
	m_strTesterBatchPath = szTesterBatchPathBuff;
	m_strNetWorkSummaryPath = szNetWorkSummaryPathBuff;
	m_str2didCsvPath = sz2didCsvPath;
	m_strHitSumamryPath = szHitSummaryPath;

	m_strBeforeBatchPath = m_strBatchPath;
	m_strBeforeLotInfoPath = m_strLotInfoPath;
	m_strBeforeSummaryPath = m_strSummaryPath;
	m_strBefore2didCsvPath = m_str2didCsvPath;
	m_strBeforeNetWorkSummaryPath = m_strNetWorkSummaryPath;
	m_strBeforeTesterBatchPath = m_strTesterBatchPath;
	m_strBeforeHitSumamryPath = m_strHitSumamryPath;

	m_bBeforeCheckBatchFileUse = m_CheckBatchFileUse.GetCheck();
	m_bBeforeTesterVersionOff = m_CheckTesterVersionOff.GetCheck();

	m_bBeforeTesterCheckBatchFileUse = m_CheckTesterBatchFileUse.GetCheck();
}

void CFilePathDlg::OnBnClickedButtonSave()
{
	char szSummaryPathBuff[MAX_PATH]={0,};
	char szBatchPathBuff[MAX_PATH]={0,}, szLotInfoBuff[MAX_PATH] ={0,}, szTesterBatchPathBuff[MAX_PATH] = { 0, };
	char szNetWorkSummaryPathBuff[MAX_PATH] = { 0, };
	char sz2didCsvPathBuff[MAX_PATH] = { 0, };
	char szHitSummaryPathBuff[MAX_PATH] = { 0, };

	GetDlgItemText(IDC_EDIT_SUMMARY_PATH, m_strSummaryPath);
	sprintf_s(szSummaryPathBuff, sizeof(szSummaryPathBuff), "%s", (LPCTSTR)m_strSummaryPath);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_SummaryFile_Path, szSummaryPathBuff, MAX_PATH);

	GetDlgItemText(IDC_EDIT_BATCH_PATH, m_strBatchPath);
	sprintf_s(szBatchPathBuff, sizeof(szBatchPathBuff), "%s", (LPCTSTR)m_strBatchPath);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_BatchFile_Path, szBatchPathBuff, MAX_PATH);

	GetDlgItemText(IDC_EDIT_LOTINFO_PATH, m_strLotInfoPath);
	sprintf_s(szLotInfoBuff, sizeof(szLotInfoBuff), "%s", (LPCTSTR)m_strLotInfoPath);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_LotInfoFile_Path, szLotInfoBuff, MAX_PATH);

	GetDlgItemText(IDC_EDIT_TESTER_BATCH_PATH, m_strTesterBatchPath);
	sprintf_s(szTesterBatchPathBuff, sizeof(szTesterBatchPathBuff), "%s", (LPCTSTR)m_strTesterBatchPath);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_Tester_BatchFile_Path, szTesterBatchPathBuff, MAX_PATH);

	GetDlgItemText(IDC_EDIT_NETWORK_SUMMARY_PATH, m_strNetWorkSummaryPath);
	sprintf_s(szNetWorkSummaryPathBuff, sizeof(szNetWorkSummaryPathBuff), "%s", (LPCTSTR)m_strNetWorkSummaryPath);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_NetWorkSummaryFile_Path, szNetWorkSummaryPathBuff, MAX_PATH);

	GetDlgItemText(IDC_EDIT_2DID_CSV_PATH, m_str2didCsvPath);
	sprintf_s(sz2didCsvPathBuff, sizeof(sz2didCsvPathBuff), "%s", (LPCTSTR)m_str2didCsvPath);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_2did_Csv_File_Path, sz2didCsvPathBuff, MAX_PATH);

	GetDlgItemText(IDC_EDIT_HIT_SUMMARY_PATH, m_strHitSumamryPath);
	sprintf_s(szHitSummaryPathBuff, sizeof(szHitSummaryPathBuff), "%s", (LPCTSTR)m_strHitSumamryPath);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_Hit_Summary_File_Path, szHitSummaryPathBuff, MAX_PATH);

	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_BatchFile_Use,m_CheckBatchFileUse.GetCheck());
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_TesterBatchFile_Use, m_CheckTesterBatchFileUse.GetCheck());
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_TESTER_VERSION_CHECK_OFF, m_CheckTesterVersionOff.GetCheck());

	char szFilePath[MAX_PATH] = {0,};
	char szTemp[512]={0,};
	::GetCurrentDirectory(sizeof(szTemp), szTemp);

	sprintf_s(szFilePath, sizeof( szFilePath ), "%s\\sys_data\\%s",szTemp, SZ_FILE_PATH);
	CConfigData CConfig(szFilePath);

	CConfig.Set("FILE PATH","Summary File Path", szSummaryPathBuff);
	CConfig.Set("FILE PATH","Batch File Path", szBatchPathBuff);
	CConfig.Set("FILE PATH","LotInfo File Path", szLotInfoBuff);
	CConfig.Set("FILE PATH", "Tester Batch File Path", szTesterBatchPathBuff);
	CConfig.Set("FILE PATH","Batch File Use", m_CheckBatchFileUse.GetCheck());
	CConfig.Set("FILE PATH", "Tester Version Check Off", m_CheckTesterVersionOff.GetCheck());
	CConfig.Set("FILE PATH", "Tester Batch File Use", m_CheckTesterBatchFileUse.GetCheck());
	CConfig.Set("FILE PATH", "Net Work Summary File Path", szNetWorkSummaryPathBuff);
	CConfig.Set("FILE PATH", "2did Csv File Path", sz2didCsvPathBuff);
	CConfig.Set("FILE PATH", "Hit Summary Path", szHitSummaryPathBuff);
	IsSamPath();

	m_strBeforeBatchPath = m_strBatchPath;
	m_strBeforeLotInfoPath = m_strLotInfoPath;
	m_strBeforeSummaryPath = m_strSummaryPath;
	m_strBeforeNetWorkSummaryPath = m_strNetWorkSummaryPath;
	m_strBefore2didCsvPath = m_str2didCsvPath;
	m_strBeforeTesterBatchPath = m_strTesterBatchPath;
	m_strBeforeHitSumamryPath = m_strHitSumamryPath;

	m_bBeforeCheckBatchFileUse = m_CheckBatchFileUse.GetCheck();
	m_bBeforeTesterVersionOff = m_CheckTesterVersionOff.GetCheck();
	m_bBeforeTesterCheckBatchFileUse = m_CheckTesterBatchFileUse.GetCheck();
}

void CFilePathDlg::OnBnClickedButtonPath()
{
	ITEMIDLIST *pidBrowse;
	char		pszPathname[MAX_PATH] ={0,};
	BROWSEINFO	BrInfo;

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;

	memset(&BrInfo,0,sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = _T("Select Directory");
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE;//BIF_RETURNONLYFSDIRS||BIF_NEWDIALOGSTYLE ;

	pidBrowse = ::SHBrowseForFolderA(&BrInfo);
	
	//폴더 선택 완료 후 진행
	if(pidBrowse != NULL)
	{
		SHGetPathFromIDList(pidBrowse,pszPathname);
		m_strSummaryPath = pszPathname;
		UpdateData(FALSE);
	}
}

void CFilePathDlg::OnBnClickedButtonLotinfoPath()
{
	ITEMIDLIST *pidBrowse;
	char		pszPathname[MAX_PATH] ={0,};
	BROWSEINFO	BrInfo;

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;

	memset(&BrInfo,0,sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = _T("Select Directory");
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE;//BIF_RETURNONLYFSDIRS||BIF_NEWDIALOGSTYLE ;

	pidBrowse = ::SHBrowseForFolderA(&BrInfo);

	//폴더 선택 완료 후 진행
	if(pidBrowse != NULL)
	{
		SHGetPathFromIDList(pidBrowse,pszPathname);
		m_strLotInfoPath = pszPathname;
		UpdateData(FALSE);
	}
}

void CFilePathDlg::OnBnClickedButtonBatchPath()
{
	CString szFilter ="All Files(*.bat)|*.bat||";
	CString filetype ="*.*";

	CFileDialog dlg(TRUE, NULL, NULL, OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST , szFilter);

	if(dlg.DoModal() == IDOK)
	{
		CString strBatchPath = dlg.GetPathName();
		m_strBatchPath = strBatchPath;
		UpdateData(FALSE);
	}

}

void CFilePathDlg::IsSamPath()
{
	if(m_strSummaryPath !=m_strBeforeSummaryPath)
	{
		MakeLog("[MENU = File Path, DATA NAME = Summary ][BEFORE=%s, AFTER=%s]", m_strBeforeSummaryPath, m_strSummaryPath);
	}

	if(m_strBatchPath != m_strBeforeBatchPath)
	{
		MakeLog("[MENU = File Path, DATA NAME = Batch ][BEFORE=%s, AFTER=%s]", m_strBeforeBatchPath, m_strBatchPath);
	}

	if(m_strLotInfoPath != m_strBeforeLotInfoPath)
	{
		MakeLog("[MENU = File Path, DATA NAME = LotInfo ][BEFORE=%s, AFTER=%s]", m_strBeforeLotInfoPath, m_strLotInfoPath);
	}

	if (m_strTesterBatchPath != m_strBeforeTesterBatchPath)
	{
		MakeLog("[MENU = File Path, DATA NAME = Tester Batch ][BEFORE=%s, AFTER=%s]", m_strBeforeTesterBatchPath, m_strTesterBatchPath);
	}

	if (m_str2didCsvPath != m_strBefore2didCsvPath)
	{
		MakeLog("[MENU = File Path, DATA NAME = 2did csv ][BEFORE=%s, AFTER=%s]", m_strBefore2didCsvPath, m_str2didCsvPath);
	}

	if (m_strHitSumamryPath != m_strBeforeHitSumamryPath)
	{
		MakeLog("[MENU = File Path, DATA NAME = 2did csv ][BEFORE=%s, AFTER=%s]", m_strBeforeHitSumamryPath, m_strHitSumamryPath);
	}

	if (m_CheckTesterBatchFileUse.GetCheck() != m_bBeforeTesterCheckBatchFileUse)
	{
		CString strBeforeLog = _T(""), strLog = _T("");

		if (m_bBeforeTesterCheckBatchFileUse == 1) {
			strBeforeLog = _T("Use");
			strLog = _T("No Use");
		}
		else {
			strBeforeLog = _T("No Use");
			strLog = _T("Use");
		}
		MakeLog("[MENU = File Path, DATA NAME = Tester Batch File Use][BEFORE=%s, AFTER=%s]", strBeforeLog, strLog);
	}

	if(m_CheckBatchFileUse.GetCheck() != m_bBeforeCheckBatchFileUse)
	{
		CString strBeforeLog = _T(""), strLog = _T("");

		if(m_bBeforeCheckBatchFileUse == 1){
			strBeforeLog = _T("Use");
			strLog = _T("No Use");
		}else{
			strBeforeLog = _T("No Use");
			strLog = _T("Use");
		}
		MakeLog("[MENU = File Path, DATA NAME = Batch File Use][BEFORE=%s, AFTER=%s]", strBeforeLog, strLog);
	}
	if (m_CheckTesterVersionOff.GetCheck() != m_bBeforeTesterVersionOff)
	{
		CString strBeforeLog = _T(""), strLog = _T("");

		if (m_bBeforeTesterVersionOff == 1) {
			strBeforeLog = _T("Use");
			strLog = _T("No Use");
		}
		else {
			strBeforeLog = _T("No Use");
			strLog = _T("Use");
		}
		MakeLog("[MENU = File Path, DATA NAME = 4Para Option Use][BEFORE=%s, AFTER=%s]", strBeforeLog, strLog);
	}
}

void CFilePathDlg::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = {0,};
	if(fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_System_GUI), Debug, "", __LINE__, NULL, tmpMsg);
}



void CFilePathDlg::OnBnClickedButton1()
{

}


void CFilePathDlg::OnBnClickedButtonTesterBatchPath()
{
	CString szFilter = "All Files(*.bat)|*.bat||";
	CString filetype = "*.*";

	CFileDialog dlg(TRUE, NULL, NULL, OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST, szFilter);

	if (dlg.DoModal() == IDOK)
	{
		CString strTesterBatchPath = dlg.GetPathName();
		m_strTesterBatchPath = strTesterBatchPath;
		UpdateData(FALSE);
	}
}


void CFilePathDlg::OnBnClickedButtonNetworkPath()
{
	ITEMIDLIST *pidBrowse;
	char		pszPathname[MAX_PATH] = { 0, };
	BROWSEINFO	BrInfo;

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;

	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = _T("Select Directory");
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;//BIF_RETURNONLYFSDIRS||BIF_NEWDIALOGSTYLE ;

	pidBrowse = ::SHBrowseForFolderA(&BrInfo);

	//폴더 선택 완료 후 진행
	if (pidBrowse != NULL)
	{
		SHGetPathFromIDList(pidBrowse, pszPathname);
		m_strNetWorkSummaryPath = pszPathname;
		UpdateData(FALSE);
	}
}


void CFilePathDlg::OnBnClickedButton2didCsvPath()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ITEMIDLIST *pidBrowse;
	char		pszPathname[MAX_PATH] = { 0, };
	BROWSEINFO	BrInfo;

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;

	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = _T("Select Directory");
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;//BIF_RETURNONLYFSDIRS||BIF_NEWDIALOGSTYLE ;

	pidBrowse = ::SHBrowseForFolderA(&BrInfo);

	//폴더 선택 완료 후 진행
	if (pidBrowse != NULL)
	{
		SHGetPathFromIDList(pidBrowse, pszPathname);
		m_str2didCsvPath = pszPathname;
		UpdateData(FALSE);
	}
}


void CFilePathDlg::OnBnClickedButtonHitSummaryPath()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ITEMIDLIST *pidBrowse;
	char		pszPathname[MAX_PATH] = { 0, };
	BROWSEINFO	BrInfo;

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;

	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = _T("Select Directory");
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;//BIF_RETURNONLYFSDIRS||BIF_NEWDIALOGSTYLE ;

	pidBrowse = ::SHBrowseForFolderA(&BrInfo);

	//폴더 선택 완료 후 진행
	if (pidBrowse != NULL)
	{
		SHGetPathFromIDList(pidBrowse, pszPathname);
		m_strHitSumamryPath = pszPathname;
		UpdateData(FALSE);
	}
}
