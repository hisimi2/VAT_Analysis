// AutoLoopSetDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "AutoLoopSetDlg.h"
#include "afxdialogex.h"


// CAutoLoopSetDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAutoLoopSetDlg, CDialog)

CAutoLoopSetDlg::CAutoLoopSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_LOOP_SET, pParent)
{
	m_dLoopSotDelay = 0;
	m_nAutoLoopTestCount = 0;
	m_nAutoLoopUnitCount = 0;
	m_nUnitCount = 0;

	m_nUnitRePickPlaceCount = 0;
	m_nUnitLoopTestContactCount = 0;

	memset(m_szCompany, 0x00, sizeof(m_szCompany));
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, m_szCompany, sizeof(m_szCompany));

	m_nAutoLoopmode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoLoopMode);
	

}

CAutoLoopSetDlg::~CAutoLoopSetDlg()
{
}

void CAutoLoopSetDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_EDIT_NUMBER_OF_UNITS, m_editUnitCount);
	DDX_Control(pDX, IDC_EDIT_NUMBER_OF_TIMES, m_editTestCount);
	DDX_Radio(pDX, IDC_RADIO_NORMAL_MODE, m_nAutoLoopmode);
	DDX_Control(pDX, IDC_EDIT_LOOP_SOT_DELAY_TIME, m_editLoopSotDelay);;

	DDX_Text(pDX, IDC_EDIT_LOOP_SOT_DELAY_TIME, m_dLoopSotDelay);

	DDX_Control(pDX, IDC_EDIT_REPICKPLACE_COUNT, m_editLoopRePickPlaceCount);
	DDX_Control(pDX, IDC_EDIT_LOOPTEST_COUNT, m_editLoopReContactCount);
	
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAutoLoopSetDlg, CDialog)
	ON_BN_CLICKED(IDC_SITESET_SAVE, &CAutoLoopSetDlg::OnBnClickedSitesetSave)
	ON_EN_SETFOCUS(IDC_EDIT_NUMBER_OF_TIMES, &CAutoLoopSetDlg::OnEnChangeEditNumberOfTimes)
	ON_EN_SETFOCUS(IDC_EDIT_LOOP_SOT_DELAY_TIME, &CAutoLoopSetDlg::OnEnChangeEditLoopSotDelayTime)
	ON_BN_CLICKED(IDC_RADIO_NORMAL_MODE, &CAutoLoopSetDlg::OnBnClickedRadioNormalMode)
	ON_BN_CLICKED(IDC_RADIO_CONTIUNE_MODE, &CAutoLoopSetDlg::OnBnClickedRadioContiuneMode)
	ON_BN_CLICKED(IDC_RADIO_RECONTACT_MODE, &CAutoLoopSetDlg::OnBnClickedRadioRecontactMode)
	ON_EN_SETFOCUS(IDC_EDIT_NUMBER_OF_UNITS, &CAutoLoopSetDlg::OnEnChangeEditNumberOfUnits)

	ON_EN_SETFOCUS(IDC_EDIT_REPICKPLACE_COUNT, &CAutoLoopSetDlg::OnEnChangeEditRepickplaceCount)
	ON_EN_SETFOCUS(IDC_EDIT_LOOPTEST_COUNT, &CAutoLoopSetDlg::OnEnChangeEditLooptestCount)

	//ON_BN_CLICKED(IDC_RADIO_NORMAL_MODE, &CAutoLoopSetDlg::OnBnClickedRadioNormalMode)

END_MESSAGE_MAP()


// CAutoLoopSetDlg 메시지 처리기입니다.


BOOL CAutoLoopSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	OnInitGUI();
	LoadData();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAutoLoopSetDlg::OnInitGUI()
{
	m_editTestCount.SetBackColor(COLOR_WWHITE);
	m_editTestCount.SetTextSize(30);

	m_editUnitCount.SetBackColor(COLOR_WWHITE);
	m_editUnitCount.SetTextSize(30);

	m_editLoopSotDelay.SetBackColor(COLOR_WWHITE);
	m_editLoopSotDelay.SetTextSize(30);

	m_editLoopRePickPlaceCount.SetBackColor(COLOR_WWHITE);
	m_editLoopRePickPlaceCount.SetTextSize(30);

	m_editLoopReContactCount.SetBackColor(COLOR_WWHITE);
	m_editLoopReContactCount.SetTextSize(30);


	GetDlgItem(IDC_EDIT_REPICKPLACE_COUNT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_LOOPTEST_COUNT)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_RADIO_NORMAL_MODE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_NUMBER_OF_TIMES)->EnableWindow(TRUE);

	GetDlgItem(IDC_STATIC_RECONTACT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_REPICKPLACE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_RECONTACT_CNT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_REPICKPLACE_CNT)->ShowWindow(SW_HIDE);
	

}

void CAutoLoopSetDlg::OnBnClickedSitesetSave()
{
	BOOL bModuleOn[2] = { FALSE, };
	UpdateData(TRUE);

	int nCount = GetDlgItemInt(IDC_EDIT_NUMBER_OF_UNITS);
	if (nCount % 4 != 0)
	{
		AfxMessageBox(_T("unit count is not even number. please check it [4, 8, 12,..... 128]"), MB_ICONHAND | MB_TOPMOST);
		return;
	}

	char szCokName[512] = { 0, };
	ST_BASIC_INFO   stBasicData, stBeforeBasicData;
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
	g_COK_FileMgr.LoadBasicInfo(szCokName, stBasicData);

	stBeforeBasicData = stBasicData;

	m_nAutoLoopTestCount = GetDlgItemInt(IDC_EDIT_NUMBER_OF_TIMES);
	m_dLoopSotDelay = GetDlgItemInt(IDC_EDIT_LOOP_SOT_DELAY_TIME);
	m_nUnitCount = GetDlgItemInt(IDC_EDIT_NUMBER_OF_UNITS);
	
	g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_AutoLoopMode, m_nAutoLoopmode);
	
	g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_AutoLoopTestCount, m_nAutoLoopTestCount);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_LOOP_SOT_DELAY_TIME, (m_dLoopSotDelay * 1000.0));
	
	g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_AutoLoopUnitCount, m_nUnitCount);



	stBasicData.nAutoLoopTestCount = m_nAutoLoopTestCount;
	stBasicData.nLoopSotDelay = m_dLoopSotDelay * 1000.0;
	stBasicData.nAutoLoopUnitCount = m_nUnitCount;
	stBasicData.nAutoLoopmode = m_nAutoLoopmode;
	stBasicData.nAutoLoopRePickPlaceCount = m_nUnitRePickPlaceCount;
	stBasicData.nAutoLoopReContactCount = m_nUnitLoopTestContactCount;

	g_COK_FileMgr.SaveBasicInfo(szCokName, &stBasicData);

	LoadData();

	std::vector<std::pair<CString, std::pair<CString, CString>>>vData;

	stBeforeBasicData.IsSameLOOPSiteSet(stBasicData, vData);

	if (!(vData.empty()))
	{
		SiteSetChangeLog(vData);
	}
}

void CAutoLoopSetDlg::SiteSetChangeLog(std::vector<std::pair<CString, std::pair<CString, CString>>> vData)
{
	for (int i = 0; i < (int)vData.size(); i++) {
		std::pair<CString, std::pair<CString, CString>> p;

		p = vData[i];

		MakeLog("[MENU = LOOP Setting, DATA NAME = %s ][BEFORE=%s, AFTER=%s]", p.first, p.second.first, p.second.second);
	}
}


void CAutoLoopSetDlg::LoadData()
{
	UpdateData(TRUE);

	m_nAutoLoopTestCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoLoopTestCount);
	m_dLoopSotDelay = g_DMCont.m_dmHandlerOpt.GN(NDM11_LOOP_SOT_DELAY_TIME) / 1000.0;
	m_nAutoLoopUnitCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoLoopUnitCount);

	m_nUnitRePickPlaceCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoRePickPlaceCount);
	m_nUnitLoopTestContactCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoLoopTestRecontactCount);


	SetDlgItemInt(IDC_EDIT_NUMBER_OF_TIMES, m_nAutoLoopTestCount);
	SetDlgItemInt(IDC_EDIT_LOOP_SOT_DELAY_TIME, m_dLoopSotDelay);
	SetDlgItemInt(IDC_EDIT_NUMBER_OF_UNITS, m_nAutoLoopUnitCount);

	SetDlgItemInt(IDC_EDIT_REPICKPLACE_COUNT, m_nUnitRePickPlaceCount);
	SetDlgItemInt(IDC_EDIT_LOOPTEST_COUNT, m_nUnitLoopTestContactCount);

	UpdateData(FALSE);
}

void CAutoLoopSetDlg::OnEnChangeEditNumberOfTimes()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };

	sprintf_s(szVal, sizeof(szVal), "%d", m_nAutoLoopTestCount);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load AutoLoop Test Count", "50000", "1");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_NUMBER_OF_TIMES)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CAutoLoopSetDlg::OnEnChangeEditLoopSotDelayTime()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };

	sprintf_s(szVal, sizeof(szVal), "%.2f", m_dLoopSotDelay);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "AutoLoop SOT DelayTime", "10", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_LOOP_SOT_DELAY_TIME)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}

void CAutoLoopSetDlg::OnBnClickedRadioNormalMode()
{
	m_nAutoLoopmode = eNormal_Mode;
}



void CAutoLoopSetDlg::OnBnClickedRadioContiuneMode()
{
	m_nAutoLoopmode = ePressContinueTest;
}


void CAutoLoopSetDlg::OnBnClickedRadioRecontactMode()
{
	m_nAutoLoopmode = ePressReContact;
}



void CAutoLoopSetDlg::OnEnChangeEditNumberOfUnits()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };

	sprintf_s(szVal, sizeof(szVal), "%d", m_nAutoLoopUnitCount);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load AutoLoop Unit Count", "128", "4");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_NUMBER_OF_UNITS)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}




void CAutoLoopSetDlg::OnEnChangeEditRepickplaceCount()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };

	sprintf_s(szVal, sizeof(szVal), "%d", m_nUnitRePickPlaceCount);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "AutoLoop RePickPlaceCount", "5000", "1");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_REPICKPLACE_COUNT)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CAutoLoopSetDlg::OnEnChangeEditLooptestCount()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };

	sprintf_s(szVal, sizeof(szVal), "%d", m_nUnitLoopTestContactCount);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "AutoLoop ReContactCount", "5000", "1");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_LOOPTEST_COUNT)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}

void CAutoLoopSetDlg::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = { 0, };
	if (fmt) {
		va_list argList;
		va_start(argList, fmt);
		_vstprintf(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_System_GUI), Debug, "", __LINE__, NULL, tmpMsg);
}