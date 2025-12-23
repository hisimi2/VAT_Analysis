// UserOper.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "UserOper.h"
#include "afxdialogex.h"


// CUserOper 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUserOper, CDialog)

CUserOper::CUserOper(CWnd* pParent /*=NULL*/)
	: CDialog(CUserOper::IDD, pParent)
{
	m_bAutoSkip = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoSkipUse);
	m_nAutoSkipCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_AutoSkipCount);
	m_nLogflag = 0;
}

CUserOper::~CUserOper()
{
	m_nLogflag =FALSE;
}

void CUserOper::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_YIELD, m_BtnYieldCtrl);
	DDX_Control(pDX, IDC_BUTTON_COUNTER, m_BtnCounter);
	DDX_Control(pDX, IDC_BUTTON_LOT_INFO, m_BtnLotInfo);
	DDX_Control(pDX, IDC_BUTTON_SITE_SET, m_BtnSiteSet);
	DDX_Control(pDX, IDC_CHECK_AUTO_SKIP, m_BtnAutoSkip);
	DDX_Control(pDX, IDC_BUTTON_PRINT, m_BtnPrint);
	DDX_Check(pDX, IDC_CHECK_AUTO_SKIP, m_bAutoSkip);
	DDX_Control(pDX, IDC_BUTTON_FilePath, m_BtnFilePath);
	DDX_Control(pDX, IDC_EDIT_AUTO_SKIP_CNT, m_CtrlAutoSkipCnt);
	DDX_Text(pDX, IDC_EDIT_AUTO_SKIP_CNT, m_nAutoSkipCnt);
}


BEGIN_MESSAGE_MAP(CUserOper, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_YIELD, &CUserOper::OnBnClickedButtonYield)
	ON_BN_CLICKED(IDC_BUTTON_SITE_SET, &CUserOper::OnBnClickedButtonSiteSet)
	ON_BN_CLICKED(IDC_BUTTON_COUNTER, &CUserOper::OnBnClickedButtonCounter)
	ON_BN_CLICKED(IDC_BUTTON_LOT_INFO, &CUserOper::OnBnClickedButtonLotInfo)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, &CUserOper::OnBnClickedButtonPrint)
	ON_BN_CLICKED(IDC_CHECK_AUTO_SKIP, &CUserOper::OnBnClickedCheckAutoSkip)
	ON_BN_CLICKED(IDC_BUTTON_FilePath, &CUserOper::OnBnClickedButtonFilepath)
	ON_EN_SETFOCUS(IDC_EDIT_AUTO_SKIP_CNT, &CUserOper::OnEnSetfocusEditAutoSkipCnt)
END_MESSAGE_MAP()


// CUserOper 메시지 처리기입니다.


BOOL CUserOper::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_nLogflag =FALSE;
	OnBtnGUI();
	OnBtnAutoSkipDisplay();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CUserOper::OnBtnGUI()
{
	m_BtnYieldCtrl.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnYieldCtrl.SetIcon(IDI_BTN_YIELD);
	m_BtnYieldCtrl.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnCounter.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCounter.SetIcon(IDI_BTN_COUNTER);
	m_BtnCounter.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnLotInfo.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnLotInfo.SetIcon(IDI_BTN_LOT_INFO);
	m_BtnLotInfo.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSiteSet.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSiteSet.SetIcon(IDI_BTN_SITEMAP);
	m_BtnSiteSet.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnPrint.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnPrint.SetIcon(IDI_BTN_PRINT);
	m_BtnPrint.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnFilePath.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));	
	m_BtnFilePath.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_CtrlAutoSkipCnt.SetBackColor(COLOR_WWHITE);
	m_CtrlAutoSkipCnt.SetTextSize(30);
}

void CUserOper::OnBtnAutoSkipDisplay()
{
	
	if(m_bAutoSkip){
		m_BtnAutoSkip.SetShade(CShadeButtonST::SHS_METAL,8,20,0,RGB(55,55,255));
		m_BtnAutoSkip.SetIcon(IDI_BTN_AUTOSKIP_ON);
		m_BtnAutoSkip.SetWindowText("Auto Skip On");
		if(m_nLogflag == TRUE)
		{
			MakeLog("[MENU = Input Auto Skip ][BEFORE= Auto Skip Off, AFTER= Auto Skip On]");
		}
	}else{
		m_BtnAutoSkip.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
		m_BtnAutoSkip.SetIcon(IDI_BTN_AUTOSKIP_OFF);
		m_BtnAutoSkip.SetWindowText("Auto Skip Off");
		if(m_nLogflag == TRUE)
		{
			MakeLog("[MENU = Input Auto Skip ][BEFORE= Auto Skip On, AFTER= Auto Skip Off]");
		}
	}
	
	m_BtnAutoSkip.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_nLogflag = TRUE;
}

BOOL CUserOper::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialog::DestroyWindow();
}

void CUserOper::OnBnClickedButtonYield()
{
	MakeLog("[Contact Control Dialog Show]");
	
	CYieldSetDlg DO;
	DO.DoModal();
}

void CUserOper::OnBnClickedButtonSiteSet()
{
	MakeLog("[Site Setting Dialog Show]");
}

void CUserOper::OnBnClickedButtonCounter()
{

}

void CUserOper::OnBnClickedButtonLotInfo()
{
	MakeLog("[Lot Information Dialog Show]");

	//char szCompany[16] = {0,};
	//g_DMCont.m_dmHandlerOpt.GS(SDM11_Company,szCompany,sizeof(szCompany));

	//if(strcmp(szCompany,"AMKOR")==0){
	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	CLotInfoAmkorDlg DO;
	DO.DoModal();
	
	//}else{
	//	CLotInfoDlg DO;
	//	DO.DoModal();
	//}
}

void CUserOper::OnBnClickedButtonPrint()
{
	MakeLog("[Print Old Report Dialog Show]");

	char szFileName[512] = {0,};
	g_DMCont.m_dmEQP.GS(SDM0_LotDoneFileName, szFileName, sizeof(szFileName));
	CString strFile = _T("");
	strFile.Format("%s\\%s", SZ_DIR_PATH_SUMMARY, szFileName);

	CString strDir = SZ_DIR_PATH_SUMMARY;
	CFileFind findfile;
	BOOL bFind =findfile.FindFile(strDir+"\\*.*");
	while(bFind)
	{
		bFind = findfile.FindNextFile(); 
		CString str = findfile.GetFileName();  
		//_summary_production_
		if(str.Find("_summary_production_") == -1){
			continue;
		}
		if(strcmp(szFileName, str)==0){
			ShellExecute(NULL, "open", strFile, NULL, NULL, SW_SHOW);
		}
		Sleep(1);
	}
}


void CUserOper::OnBnClickedCheckAutoSkip()
{
	UpdateData(FALSE);

	m_bAutoSkip = !m_bAutoSkip;
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_AutoSkipUse, m_bAutoSkip);
	OnBtnAutoSkipDisplay();

	char szCokName[512] = {0,};
	ST_BASIC_INFO   stBasicData;
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
	g_COK_FileMgr.LoadBasicInfo(szCokName, stBasicData);

	stBasicData.bAutoSkipMode = m_bAutoSkip;

	g_COK_FileMgr.SaveBasicInfo(szCokName, &stBasicData);
}


BOOL CUserOper::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_ESCAPE ){
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CUserOper::MakeLog(LPCTSTR fmt, ...)
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


void CUserOper::OnBnClickedButtonFilepath()
{
	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		CFilePathDlg DO;
		DO.DoModal();
	}
}


void CUserOper::OnEnSetfocusEditAutoSkipCnt()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nAutoSkipCnt, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "AutoSkip Count", "30", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_AUTO_SKIP_CNT)->SetWindowText(szRetVal);

		UpdateData(TRUE);
		char szCokName[512] = {0,};
		ST_BASIC_INFO   stBasicData;
		g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
		g_COK_FileMgr.LoadBasicInfo(szCokName, stBasicData);

		stBasicData.nAutoSkipCount = m_nAutoSkipCnt;
		g_COK_FileMgr.SaveBasicInfo(szCokName, &stBasicData);

		g_DMCont.m_dmHandlerOpt.SN(NDM11_AutoSkipCount, m_nAutoSkipCnt);
	}
}
