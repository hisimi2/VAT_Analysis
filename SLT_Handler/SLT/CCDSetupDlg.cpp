#include "StdAfx.h"
#include "CCDSetupDlg.h"
#include "SLT.h"
#include "afxdialogex.h"
#include "MainFrm.h"

IMPLEMENT_DYNAMIC(CCCDSetupDlg, CDialog)

CCCDSetupDlg::CCCDSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCCDSetupDlg::IDD, pParent)
{
}


CCCDSetupDlg::~CCCDSetupDlg(void)
{
}

void CCCDSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_2D_BARCODE, m_Btn2Dbarcode);
	DDX_Control(pDX, IDC_TESTSITE_UP, m_BtnTestUpSite);
	DDX_Control(pDX, IDC_TESTSITE_DOWN, m_BtnTestDownSite);
}

BEGIN_MESSAGE_MAP(CCCDSetupDlg, CDialog)
	ON_BN_CLICKED(IDC_2D_BARCODE, &CCCDSetupDlg::OnBnClicked2dBarcode)
	ON_BN_CLICKED(IDC_TESTSITE_UP, &CCCDSetupDlg::OnBnClickedTestsiteUp)
	ON_BN_CLICKED(IDC_TESTSITE_DOWN, &CCCDSetupDlg::OnBnClickedTestsiteDown)
END_MESSAGE_MAP()

BOOL CCCDSetupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	
	char szDriverFileName[MAX_PATH] = { 0, };
	char szTemp[512] = { 0, };
	::GetCurrentDirectory(sizeof(szTemp), szTemp);
	sprintf_s(szDriverFileName, sizeof(szDriverFileName), "%s\\sys_data\\%s", szTemp, SZ_DRIVER_FILE_NAME);

	CConfigData	 ConfigData(szDriverFileName);
	
	CString strIP_PP1 = _T("");
	CString strIP_PP2 = _T("");
	char szData[512] = { 0, };
	ConfigData.GetString("MC TO VISION COMM CONFIG", "TEST HAND 1 IP", "127.0.0.1", szData);
	strIP_PP1 = szData;
	ConfigData.GetString("MC TO VISION COMM CONFIG", "TEST HAND 2 IP", "127.0.0.1", szData);
	strIP_PP2 = szData;

	if (strIP_PP1 == strIP_PP2)
	{
		GetDlgItem(IDC_TESTSITE_DOWN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TESTSITE_UP)->SetWindowTextA(_T("Vision"));
	}
		

	OnBtnGUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCCDSetupDlg::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);	

	m_Btn2Dbarcode.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_Btn2Dbarcode.SetIcon(IDI_BTN_BARCODE);
	m_Btn2Dbarcode.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnTestUpSite.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnTestUpSite.SetIcon(IDI_BTN_VISION_UP);
	m_BtnTestUpSite.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnTestDownSite.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnTestDownSite.SetIcon(IDI_BTN_VISION_DOWN);
	m_BtnTestDownSite.SetAlign(CButtonST::ST_ALIGN_VERT);
}

// mstsc.exe : 원격 데스크톱 응용프로그램
void CCCDSetupDlg::OnBnClicked2dBarcode()
{
	ShellExecute(NULL, "open", "mstsc.exe", "/v:100.100.100.103", NULL, SW_SHOW);
}


void CCCDSetupDlg::OnBnClickedTestsiteUp()
{
	ShellExecute(NULL, "open", "mstsc.exe", "/v:192.168.1.60", NULL, SW_SHOW);
}


void CCCDSetupDlg::OnBnClickedTestsiteDown()
{
	ShellExecute(NULL, "open", "mstsc.exe", "/v:192.168.1.61", NULL, SW_SHOW);
}
