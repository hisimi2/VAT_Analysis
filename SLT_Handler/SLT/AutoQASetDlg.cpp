// AutoQASetDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "AutoQASetDlg.h"
#include "afxdialogex.h"


// CAutoQASetDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAutoQASetDlg, CDialog)

CAutoQASetDlg::CAutoQASetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_QA_MODE_SET, pParent)
{
	char szLotSize[MAX_PATH] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_DEFAULT_QA_QTY, szLotSize, MAX_PATH);
	m_nBeforeUnitCount = m_nUnitCount = atoi(szLotSize);
}

CAutoQASetDlg::~CAutoQASetDlg()
{
}

void CAutoQASetDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_EDIT_NUMBER_OF_UNITS, m_editUnitCount);
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAutoQASetDlg, CDialog)
	ON_BN_CLICKED(IDC_SITESET_SAVE, &CAutoQASetDlg::OnBnClickedSitesetSave)
	ON_BN_CLICKED(IDCANCEL, &CAutoQASetDlg::OnBnClickedCancel)
	ON_EN_SETFOCUS(IDC_EDIT_NUMBER_OF_UNITS, &CAutoQASetDlg::OnEnSetfocusEditNumberOfUnits)
END_MESSAGE_MAP()


// CAutoQASetDlg 메시지 처리기입니다.
BOOL CAutoQASetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_editUnitCount.SetBackColor(COLOR_WWHITE);
	m_editUnitCount.SetTextSize(30);

	SetDlgItemInt(IDC_EDIT_NUMBER_OF_UNITS, m_nUnitCount);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}




void CAutoQASetDlg::OnBnClickedSitesetSave()
{
	m_nUnitCount = GetDlgItemInt(IDC_EDIT_NUMBER_OF_UNITS);

	char szLotSize[64] = { 0, };
	CString str; 
	str.Format("%d", m_nUnitCount);
	sprintf_s(szLotSize, sizeof(szLotSize), str);


	char szCokName[512] = { 0, };
	ST_BASIC_INFO   stBasicData, stBeforeBasicData;
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
	g_COK_FileMgr.LoadBasicInfo(szCokName, stBasicData);

	stBeforeBasicData = stBasicData;

	g_DMCont.m_dmHandlerOpt.SS(SDM11_DEFAULT_QA_QTY, szLotSize, MAX_PATH);

	int nFTRCQC = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	if (nFTRCQC == eStartMD_QA || nFTRCQC == eStartMD_QA_Continue)
	{//QA Mode 일때만, QTY 수량 동기화를 위해 변경 해
		g_DMCont.m_dmHandlerOpt.SS(SDM11_QTY, szLotSize, MAX_PATH);
	}

	stBasicData.nAutoQADefaultCount = m_nUnitCount;

	g_COK_FileMgr.SaveBasicInfo(szCokName, &stBasicData);

	std::vector<std::pair<CString, std::pair<CString, CString>>>vData;

	stBeforeBasicData.IsSameQASiteSet(stBasicData, vData);

	if (!(vData.empty()))
	{
		SiteSetChangeLog(vData);
	}


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
void CAutoQASetDlg::SiteSetChangeLog(std::vector<std::pair<CString, std::pair<CString, CString>>> vData)
{
	for (int i = 0; i < (int)vData.size(); i++) {
		std::pair<CString, std::pair<CString, CString>> p;

		p = vData[i];

		MakeLog("[MENU = QA Setting, DATA NAME = %s ][BEFORE=%s, AFTER=%s]", p.first, p.second.first, p.second.second);
	}
}

void CAutoQASetDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnCancel();
}


void CAutoQASetDlg::OnEnSetfocusEditNumberOfUnits()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };

	sprintf_s(szVal, sizeof(szVal), "%d", m_nUnitCount);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load QA Unit Count", "400", "1");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_NUMBER_OF_UNITS)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}

void CAutoQASetDlg::MakeLog(LPCTSTR fmt, ...)
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


