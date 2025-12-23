// MainLowerDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "MainLowerDlg.h"
#include "afxdialogex.h"

#include "DEF_LIB_COMMCTRL.h"
#include "SLTView.h"
#include "MainFrm.h"
// CMainLowerDlg 대화 상자입니다.



IMPLEMENT_DYNAMIC(CMainLowerDlg, CDialog)

CMainLowerDlg::CMainLowerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainLowerDlg::IDD, pParent),
	m_pgYieldPer(CMainLower_TabYield::YM_PERCENTAGE, NULL),
	m_pgYieldCnt(CMainLower_TabYield::YM_COUNT, NULL),
	m_pgYield1stFail(CMainLower_TabYield::YM_1ST_FAIL, NULL),
	m_pgYield2DIDFail(CMainLower_TabYield::YM_2DID_FAIL, NULL)
{
	m_pParentWnd = pParent;
	m_strCategoryName = _T("");
	m_dlgGrrResult = NULL;


	m_hBrushGreen = NULL;
	m_hBrushPink = NULL;
	m_hBrushBlue = NULL;
	m_hBrushPurple = NULL;

	m_nOEEChkLastMin = -1;
	m_bChkYear = FALSE;

	m_nTesterMode = eTesterIF_Mode_Normal;

	m_nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);


}

CMainLowerDlg::~CMainLowerDlg()
{
	DeleteObject(m_hBrushGreen);
	DeleteObject(m_hBrushPink);
	DeleteObject(m_hBrushBlue);
	DeleteObject(m_hBrushPurple);

	if (m_dlgGrrResult != NULL) {
		m_dlgGrrResult->DestroyWindow();
		delete m_dlgGrrResult;
		m_dlgGrrResult = NULL;
	}	
}

void CMainLowerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_YIELD_CTRL, m_StaticYield);
	DDX_Control(pDX, IDC_CONT_FAIL, m_StaticContiFail);
	DDX_Control(pDX, IDC_STATIC_ONOFFLINE, m_StaticRunMode);
	DDX_Control(pDX, IDC_STATIC_RUNSTOP, m_StaticRunStop);
	DDX_Control(pDX, IDC_STATIC_DVC_MODE, m_StaticDvcMode);
	DDX_Control(pDX, IDC_LIST_LOTDATA, m_ListLotData);
	DDX_Control(pDX, IDC_BTN_LOT_INFO, m_BtnLotInfo);
	DDX_Control(pDX, IDC_BTN_TRAYMAP, m_BtnTrayMap);
	DDX_Control(pDX, IDC_BTN_LOT_DONE, m_BtnLotDone);
	DDX_Control(pDX, IDC_BTN_CLEAR, m_BtnClearCnt);
	DDX_Control(pDX, IDC_SPREAD_BINVIEW, m_SprdOutputBinView);
	//	DDX_Control(pDX, IDC_SPREAD_SITEYIELD, m_SpredSiteYield);
	DDX_Control(pDX, IDC_SPREAD_BIN_CATEGORY, m_SpredBinCount);
	DDX_Control(pDX, IDC_SPREAD_ART_CNT, m_SpredArtCnt);
	//DDX_Radio(pDX, IDC_RADIO_YIELD1, m_nSiteYieldInfo);
	DDX_Control(pDX, IDC_2D_VISION, m_StaticVision2D);
	DDX_Control(pDX, IDC_TEST_SITE_VISION, m_StaticVisionTestSite);
	DDX_Control(pDX, IDC_AUTO_RETEST, m_StaticAutoRetest);
	DDX_Control(pDX, IDC_AUTO_SKIP, m_StaticAutoSkip);
	DDX_Control(pDX, IDC_STATIC_AUTOGRR, m_StaticAutoGrr);
	DDX_Control(pDX, IDC_BTN_GRRRESULT, m_BtnGrrResult);
	DDX_Control(pDX, IDC_YIELD_VARI, m_StaticVariance);
	DDX_Control(pDX, IDC_AUTO_CLEAN, m_StaticSocketClean);
	//DDX_Control(pDX, IDC_STATIC_SWITCH, m_StaticSwitch);
	DDX_Control(pDX, IDC_BTN_SUMMARY, m_BtnMakeSummary);
	DDX_Control(pDX, IDC_MAIN_TAB, m_tabResult);
	DDX_Control(pDX, IDC_BUTTON_2DID_CSV, m_BtnCSVFile);
	DDX_Control(pDX, IDC_BUTTON_SUMMARY, m_BtnSummaryCSVFile);
	DDX_Control(pDX, IDC_BUTTON_2DID_CSV_VIEW, m_Btn2DIDView);
	DDX_Control(pDX, IDC_BTN_LOG, m_BtnEventLog);
	DDX_Control(pDX, IDC_BTN_OEE, m_BtnOEE);
	DDX_Control(pDX, IDC_BTN_HANDLER_LOG, m_BtnHandlerLog);
	DDX_Control(pDX, IDC_BTN_SERVO_MOTOR, m_BtnServoMotor);
	DDX_Control(pDX, IDC_BUTTON_BATCH_FILE, m_BtnBatchFile);
	DDX_Control(pDX, IDC_BTN_QA_MODE, m_BtnQAModeSet);
	
}


BEGIN_MESSAGE_MAP(CMainLowerDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_MAIN_DISPLAY, OnUserFuncMsg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_LOT_INFO, &CMainLowerDlg::OnBnClickedBtnLotInfo)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CMainLowerDlg::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_BTN_LOT_DONE, &CMainLowerDlg::OnBnClickedBtnLotDone)
	ON_BN_CLICKED(IDC_BTN_TRAYMAP, &CMainLowerDlg::OnBnClickedBtnTraymap)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_GRRRESULT, &CMainLowerDlg::OnBnClickedBtnGrrresult)
	ON_BN_CLICKED(IDC_BTN_SUMMARY, &CMainLowerDlg::OnBnClickedBtnSummary)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MAIN_TAB, &CMainLowerDlg::OnTcnSelchangeMainTab)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_LOG, &CMainLowerDlg::OnBnClickedBtnLog)
	ON_BN_CLICKED(IDC_BUTTON_2DID_CSV, &CMainLowerDlg::OnBnClickedButton2didCsv)
	ON_BN_CLICKED(IDC_BUTTON_2DID_CSV_VIEW, &CMainLowerDlg::OnBnClickedButton2didCsvView)
	ON_BN_CLICKED(IDC_BUTTON_SUMMARY, &CMainLowerDlg::OnBnClickedButtonSummary)
	ON_STN_DBLCLK(IDC_AUTO_RETEST, &CMainLowerDlg::OnStnClickedAutoRetest)
	ON_STN_DBLCLK(IDC_TEST_SITE_VISION, &CMainLowerDlg::OnStnClickedTestSiteVision)
	ON_STN_DBLCLK(IDC_YIELD_CTRL, &CMainLowerDlg::OnStnClickedYieldCtrl)
	ON_STN_DBLCLK(IDC_AUTO_SKIP, &CMainLowerDlg::OnStnClickedAutoSkip)
	ON_STN_DBLCLK(IDC_2D_VISION, &CMainLowerDlg::OnStnClicked2dVision)
	ON_STN_DBLCLK(IDC_AUTO_CLEAN, &CMainLowerDlg::OnStnClickedAutoClean)
	ON_BN_CLICKED(IDC_BTN_OEE, &CMainLowerDlg::OnBnClickedBtnOee)
	ON_BN_CLICKED(IDC_BUTTON_SOCKET_DIAGNOSIS, &CMainLowerDlg::OnBnClickedButtonSocketDiagnosis)
	ON_BN_CLICKED(IDC_BTN_HANDLER_LOG, &CMainLowerDlg::OnBnClickedBtnHandlerLog)
	//ON_STN_CLICKED(IDC_STATIC_SWITCH, &CMainLowerDlg::OnStnClickedStaticSwitch)
	ON_BN_CLICKED(IDC_BTN_SERVO_MOTOR, &CMainLowerDlg::OnBnClickedBtnServoMotor)
	ON_BN_CLICKED(IDC_BUTTON_BATCH_FILE, &CMainLowerDlg::OnBnClickedButtonBatchFile)
	ON_BN_CLICKED(IDC_BTN_QA_MODE, &CMainLowerDlg::OnBnClickedBtnQaMode)
END_MESSAGE_MAP()


// CMainLowerDlg 메시지 처리기입니다.


BOOL CMainLowerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnInitDialog_MainTabPages();

	OnGUI();

	char szData[MAX_PATH] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_CategoryName, szData, sizeof(szData));
	char szTemp[512] = { 0, };
	::GetCurrentDirectory(sizeof(szTemp), szTemp);
	CString strDir = _T("");
	strDir.Format("%s\\sys_data\\HandlerRecipe\\%s\\%s", szTemp, SZ_CATEGORY_FOLDER, szData);
	g_COK_FileMgr.LoadCategoryInfo(szData, m_stCategory);

	m_strCategoryName.Format("%s", szData);

	SetDlgItemText(IDC_STATIC_FINAL_DATA, _T("Final Total Data"));
	SetDlgItemText(IDC_STATIC_ART_COUNT, _T("ART Count"));

	g_DMCont.m_dmEQP.SB( BDM0_CATEGORY_CHANGE, FALSE );

	SetTimer(0, 800, NULL);
	SetTimer(1, 1000, NULL);

	char szCustomer[128] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCustomer, sizeof(szCustomer));

	
	GetDlgItem(IDC_BTN_SUMMARY)->ShowWindow(SW_SHOW);
	m_BtnMakeSummary.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnMakeSummary.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_dlgGrrResult = new CAutoGrrResultDlg;
	m_dlgGrrResult->Create(IDD_DIALOG_AUTOGRR_RESULT, this);

	m_hBrushGreen = CreateSolidBrush(RGB(47, 237, 40));  // 배경색
	m_hBrushPink  = CreateSolidBrush(RGB(237, 160, 199));  // 배경색
	m_hBrushBlue = CreateSolidBrush(RGB(100, 100, 199));  // 배경색
	m_hBrushPurple = CreateSolidBrush(RGB(199, 160, 237));
	
	CompanySetting();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMainLowerDlg::OnInitDialog_MainTabPages()
{
	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);

	/*inti tab control*/
	m_tabResult.SetMinTabWidth(200);

	m_tabResult.DeleteAllItems();
	m_tabResult.InsertItem( MTPI_PASSFAIL		, "Pass/Fail"		);
	m_tabResult.InsertItem( MTPI_YIELD_PER		, "Yields (%)"		);
	m_tabResult.InsertItem( MTPI_YIELD_CNT		, "Yields (Count)"	);
	m_tabResult.InsertItem( MTPI_LAST_DESC		, "Last Description");
	//m_tabResult.InsertItem(MTPI_DOOR_STATUS, "Door Status");

	
	m_pgPassFail.Create(CMainLower_TabPassFail::IDD, &m_tabResult);
	m_pgYieldPer.Create(CMainLower_TabYield::IDD, &m_tabResult);
	m_pgYieldCnt.Create(CMainLower_TabYield::IDD, &m_tabResult);
	m_pgLastDesc.Create(CMainLower_TabDesc::IDD, &m_tabResult);

	OnTcnSelchangeMainTab(nullptr, nullptr);
}

void CMainLowerDlg::OnTcnSelchangeMainTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);

	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int tab_index = m_tabResult.GetCurSel();



	switch (tab_index)
	{
	case MTPI_PASSFAIL:
	{
		m_pgPassFail.ShowWindow(SW_SHOW);
		m_pgYieldPer.ShowWindow(SW_HIDE);
		m_pgYieldCnt.ShowWindow(SW_HIDE);
		m_pgLastDesc.ShowWindow(SW_HIDE);

	}break;
	case MTPI_YIELD_PER:
	{
		m_pgPassFail.ShowWindow(SW_HIDE);
		m_pgYieldPer.ShowWindow(SW_SHOW);
		m_pgYieldCnt.ShowWindow(SW_HIDE);
		m_pgLastDesc.ShowWindow(SW_HIDE);

	}break;
	case MTPI_YIELD_CNT:
	{
		m_pgPassFail.ShowWindow(SW_HIDE);
		m_pgYieldPer.ShowWindow(SW_HIDE);
		m_pgYieldCnt.ShowWindow(SW_SHOW);
		m_pgLastDesc.ShowWindow(SW_HIDE);

	}break;
	case MTPI_LAST_DESC:
	{
		m_pgPassFail.ShowWindow(SW_HIDE);
		m_pgYieldPer.ShowWindow(SW_HIDE);
		m_pgYieldCnt.ShowWindow(SW_HIDE);
		m_pgLastDesc.ShowWindow(SW_SHOW);

	}break;
	case MTPI_DOOR_STATUS:
	{
		m_pgPassFail.ShowWindow(SW_HIDE);
		m_pgYieldPer.ShowWindow(SW_HIDE);
		m_pgYieldCnt.ShowWindow(SW_HIDE);
		m_pgLastDesc.ShowWindow(SW_HIDE);

	}break;
	
	}
	

	if(pResult)
		*pResult = 0;
}

LRESULT CMainLowerDlg::OnUserFuncMsg(WPARAM wParam, LPARAM lParam)
{
	return 0L;
}

void CMainLowerDlg::fn_OnUpdateLotData()
{
	
	m_ListLotData.ResetContent();

	char szRetBuff[256] = { 0, };
	CString strTextDisplay = _T("");

	g_DMCont.m_dmHandlerOpt.GS(SDM11_LotNo, szRetBuff, sizeof(szRetBuff));
	strTextDisplay.Format("Lot No         : %s", szRetBuff);
	m_ListLotData.AddString(strTextDisplay);

	//g_DMCont.m_dmHandlerOpt.GS(SDM11_ScheduleNo, szRetBuff, sizeof(szRetBuff));
	//strTextDisplay.Format("Process ID  : %s", szRetBuff);
	//m_ListLotData.AddString(strTextDisplay);

	//g_DMCont.m_dmHandlerOpt.GS(SDM11_CustomerName, szRetBuff, sizeof(szRetBuff));
	//strTextDisplay.Format("Customer    : %s", szRetBuff);
	//m_ListLotData.AddString(strTextDisplay);

	//g_DMCont.m_dmHandlerOpt.GS(SDM11_DeviceName, szRetBuff, sizeof(szRetBuff));
	//strTextDisplay.Format("Device ID   : %s", szRetBuff);
	//m_ListLotData.AddString(strTextDisplay);

	g_DMCont.m_dmHandlerOpt.GS(SDM11_ProgramName, szRetBuff, sizeof(szRetBuff));
	strTextDisplay.Format("Tester Version : %s", szRetBuff);
	m_ListLotData.AddString(strTextDisplay);

	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szRetBuff, sizeof(szRetBuff));
	strTextDisplay.Format("TrayFile       : %s", szRetBuff);
	m_ListLotData.AddString(strTextDisplay);

	g_DMCont.m_dmHandlerOpt.GS(SDM11_SLT_ProgramName, szRetBuff, sizeof(szRetBuff));
	strTextDisplay.Format("SLT Pgm        : %s", szRetBuff);
	m_ListLotData.AddString(strTextDisplay);

	g_DMCont.m_dmHandlerOpt.GS(SDM11_CategoryName, szRetBuff, sizeof(szRetBuff));
	strTextDisplay.Format("BinCategory    : %s", szRetBuff);
	m_ListLotData.AddString(strTextDisplay);

	g_DMCont.m_dmHandlerOpt.GS(SDM11_QTY, szRetBuff, sizeof(szRetBuff));
	strTextDisplay.Format("QTY            : %s", szRetBuff);
	m_ListLotData.AddString(strTextDisplay);

	int nRetCtrl = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	switch (nRetCtrl)
	{
	case eStartMD_Initial: { sprintf_s(szRetBuff, sizeof(szRetBuff), "FT");					}break;
	case eStartMD_Retest: { sprintf_s(szRetBuff, sizeof(szRetBuff), "RT");					}break;
	case eStartMD_QA: { sprintf_s(szRetBuff, sizeof(szRetBuff), "QA");					}break;
	case eStartMD_AutoGRR: { sprintf_s(szRetBuff, sizeof(szRetBuff), "Auto GRR");			}break;
	case eStartMD_AutoLoop: { sprintf_s(szRetBuff, sizeof(szRetBuff), "Auto Loop");			}break;

	case eStartMD_AutoGRR_Continue: { sprintf_s(szRetBuff, sizeof(szRetBuff), "AutoGRR_CONTINUE");	}break;
	case eStartMD_RetestContinue: { sprintf_s(szRetBuff, sizeof(szRetBuff), "RT_CONTINUE");			}break;
	case eStartMD_InitContinue: { sprintf_s(szRetBuff, sizeof(szRetBuff), "FT_CONTINUE");			}break;
	case eStartMD_QA_Continue: { sprintf_s(szRetBuff, sizeof(szRetBuff), "QA_CONTINUE");			}break;	
	case eStartMD_AutoLoop_Continue: { sprintf_s(szRetBuff, sizeof(szRetBuff), "AutoLoop_CONTINUE");}break;

	}
	strTextDisplay.Format("Lot Control    : %s", szRetBuff);
	m_ListLotData.AddString(strTextDisplay);

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	
	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
	int nMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_TESTER_IF_PRO_AUDIT);
	
	if (nRunMode == eRUN_MODE_ONLINE && m_nTesterMode != nMode) {	
		m_nTesterMode = nMode;
		switch (nMode)
		{
		case eTesterIF_Mode_Normal:
		{
			strTextDisplay.Format("Tester Mode    : Production");
			m_ListLotData.AddString(strTextDisplay);
		}break;
		case eTesterIF_Mode_Grr:
		{
			strTextDisplay.Format("Tester Mode    : GRR");
			m_ListLotData.AddString(strTextDisplay);
		}break;
		case eTesterIF_Mode_LoopTest:
		{
			strTextDisplay.Format("Tester Mode    : LoopTest");
			m_ListLotData.AddString(strTextDisplay);
		}break;
		case eTesterIF_Mode_Audit:
		{
			strTextDisplay.Format("Tester Mode    : Audit");
			m_ListLotData.AddString(strTextDisplay);
		}break;
		}
	}

// 	if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0 && nRetCtrl == eStartMD_Auto2DIDSort || nRetCtrl == eStartMD_Auto2DIDSort_Continue){	
// 		GetDlgItem(IDC_BUTTON_2DID_CSV_VIEW)->ShowWindow(SW_SHOW);
// 	}
// 	else {
		GetDlgItem(IDC_BUTTON_2DID_CSV_VIEW)->ShowWindow(SW_HIDE);
//	}


	GetDlgItem(IDC_BTN_OEE)->ShowWindow(SW_HIDE);

	
}

void CMainLowerDlg::OnGUI()
{
	m_StaticYield.m_fnSetFont("Fixedsys");
	m_StaticYield.SetTextSize(15);
	m_StaticYield.SetBackColor(COLOR_NOMAL);

	m_StaticContiFail.m_fnSetFont("Fixedsys");
	m_StaticContiFail.SetTextSize(15);
	m_StaticContiFail.SetBackColor(COLOR_NOMAL);

	m_StaticAutoSkip.m_fnSetFont("Fixedsys");
	m_StaticAutoSkip.SetTextSize(15);
	m_StaticAutoSkip.SetBackColor(COLOR_NOMAL);

	m_StaticAutoRetest.m_fnSetFont("Fixedsys");
	m_StaticAutoRetest.SetTextSize(15);
	m_StaticAutoRetest.SetBackColor(COLOR_NOMAL);

	m_StaticVision2D.m_fnSetFont("Fixedsys");
	m_StaticVision2D.SetTextSize(15);
	m_StaticVision2D.SetBackColor(COLOR_NOMAL);

	m_StaticVisionTestSite.m_fnSetFont("Fixedsys");
	m_StaticVisionTestSite.SetTextSize(15);
	m_StaticVisionTestSite.SetBackColor(COLOR_NOMAL);

	m_StaticAutoGrr.m_fnSetFont("Fixedsys");
	m_StaticAutoGrr.SetTextSize(15);
	m_StaticAutoGrr.SetBackColor(COLOR_NOMAL);

	m_StaticSocketClean.m_fnSetFont("Fixedsys");
	m_StaticSocketClean.SetTextSize(15);
	m_StaticSocketClean.SetBackColor(COLOR_NOMAL);

	m_StaticVariance.m_fnSetFont("Fixedsys");
	m_StaticVariance.SetTextSize(15);
	m_StaticVariance.SetBackColor(COLOR_NOMAL);

	m_BtnLotInfo.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnLotInfo.SetIcon(IDI_BTN_LOT_INFO);
	m_BtnLotInfo.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnTrayMap.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnTrayMap.SetIcon(IDI_BTN_TRAYMAP);
	m_BtnTrayMap.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnLotDone.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnLotDone.SetIcon(IDI_BTN_PRINT);
	m_BtnLotDone.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnClearCnt.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnClearCnt.SetIcon(IDI_BTN_CLEAR);
	m_BtnClearCnt.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnGrrResult.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnGrrResult.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnCSVFile.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnCSVFile.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSummaryCSVFile.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnSummaryCSVFile.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_Btn2DIDView.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_Btn2DIDView.SetAlign(CButtonST::ST_ALIGN_VERT);
	
	m_BtnEventLog.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnEventLog.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnOEE.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnOEE.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnHandlerLog.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnHandlerLog.SetAlign(CButtonST::ST_ALIGN_VERT);


	m_BtnServoMotor.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnServoMotor.SetIcon(IDI_BTN_MOTOR_MONITOR);
 	m_BtnServoMotor.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnBatchFile.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnBatchFile.SetAlign(CButtonST::ST_ALIGN_VERT);
	
	m_BtnQAModeSet.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnQAModeSet.SetAlign(CButtonST::ST_ALIGN_VERT);

	

	//m_StaticSwitch.m_fnSetFont("Fixedsys");
// 	m_StaticSwitch.SetTextSize(23);
// 	GetDlgItem(IDC_STATIC_SWITCH)->SetWindowTextA("Servo Motor");
// 	m_StaticSwitch.SetBackColor(COLOR_YELLOW);

	m_StaticRunStop.SetTextSize(30);
	m_StaticRunMode.SetTextSize(30);
	m_StaticDvcMode.SetTextSize(15);
}

void CMainLowerDlg::fn_OnUpdateYield()
{
	CString strEditText = _T("Yield : ");
	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	
	BOOL bUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_YieldCtrl);
	if (bUseMode == eOPTION_USE) {
		m_StaticYield.SetBackColor(COLOR_GREEN);
	}
	else {
		m_StaticYield.SetBackColor(COLOR_LLIGHTGRAY);
	}

	double dRetData = g_DMCont.m_dmHandlerOpt.GD(DDM11_Low_Yield);
	CString strYield = _T("");
	CString str1stFail = _T("");
	strYield.Format("%0.2f %%", dRetData);
	strEditText += strYield;
	GetDlgItem(IDC_YIELD_CTRL)->SetWindowTextA(strEditText);
}

void CMainLowerDlg::fn_OnUpdateVariance()
{
	CString strEditText1 = _T("All Contiue Fail : ");

	BOOL bUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_YieldCtrl);
	if (bUseMode == eOPTION_USE) {
		m_StaticVariance.SetBackColor(COLOR_GREEN);
	}
	else {
		m_StaticVariance.SetBackColor(COLOR_LLIGHTGRAY);
	}

	int nCount1 = g_DMCont.m_dmHandlerOpt.GN(NDM11_ContinueFail_FailCnt_Station);
	CString strFailCnt = _T("");
	strFailCnt.Format("%d Ea", nCount1);
	strEditText1 += strFailCnt;

	GetDlgItem(IDC_YIELD_VARI)->SetWindowTextA(strEditText1);
}

void CMainLowerDlg::fn_OnUpdateSocketClean()
{
	CString strEditText = _T("Auto Cleaning : ");
	CString strOnOff = _T( "" );

	BOOL bUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_SocketClean);
	int nTestCnt = g_DMCont.m_dmHandlerOpt.GN( NDM11_SockClnContactCnt );
	if (bUseMode == eOPTION_USE) {
		m_StaticSocketClean.SetBackColor(COLOR_GREEN);
		strOnOff.Format( _T( "On / %d Times" ), nTestCnt );
	}
	else {
		m_StaticSocketClean.SetBackColor(COLOR_LLIGHTGRAY);
		strOnOff.Format( _T( "Off" ));
	}
	strEditText += strOnOff;

	// 	int nContactTimes = g_DMCont.m_dmHandlerOpt.GN(NDM11_LotSocketCleanCount);
	// 	CString strYield = _T("");
	// 	strYield.Format("Count %d Times", nContactTimes);
	// 	strEditText += strYield;

	GetDlgItem(IDC_AUTO_CLEAN)->SetWindowTextA(strEditText);


}

void CMainLowerDlg::fn_OnUpdateContactForce()
{
	CString strEditText = _T("");
	CString strForce = _T("");
	double dForce = g_DMCont.m_dmHandlerOpt.GD(DDM11_ContactForce_gf);

	strEditText.Format("Force : %.2f Kgf", dForce / 1000.0);
	GetDlgItem(IDC_CONTACT_FORCE)->SetWindowTextA(strEditText);
}

void CMainLowerDlg::fn_OnUpdateAutoSkip()
{
	CString strEditText1 = _T("");
	CString strOnOff = _T("");

	BOOL bUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoSkipUse);
	if (bUseMode == eOPTION_USE) {
		m_StaticAutoSkip.SetBackColor(COLOR_GREEN);
		strOnOff = _T("On");
	}
	else {
		m_StaticAutoSkip.SetBackColor(COLOR_LLIGHTGRAY);
		strOnOff = _T("Off");
	}

	strEditText1.Format("AutoSkip : %s", strOnOff);
	GetDlgItem(IDC_AUTO_SKIP)->SetWindowTextA(strEditText1);
}

void CMainLowerDlg::fn_OnUpdateVision2D()
{
	CString strEditText1 = _T("");
	CString strOnOff = _T("");

	BOOL bUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_BarCode_Use);
	if (bUseMode == eOPTION_USE) {
		m_StaticVision2D.SetBackColor(COLOR_GREEN);
		//strOnOff = _T("On");
	
		char szCompany[16] = { 0, };
		g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
		int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
		if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0 /*&& nProjectOption == DEF_PROJECT_WATT*/) {

			int n2dMarkUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_MARK_OPT_USE);
			
			if (n2dMarkUse == TRUE)
			{
				char szDeviceName[MAX_PATH] = { 0, };
				g_DMCont.m_dmHandlerOpt.GS(SDM11_2D_DEVICE_NAME, szDeviceName, sizeof(szDeviceName));

				CString strDeviceName = _T("");
				strDeviceName.Format("%s", szDeviceName);

				//Device Empty 예외처리.
				if (strDeviceName == _T("")) {
					strDeviceName = _T("EMPTY");
				}

				strEditText1.Format("2DID : %s", strDeviceName);
				GetDlgItem(IDC_2D_VISION)->SetWindowTextA(strEditText1);
			}
			else {
				strOnOff = _T("On/No Mark");
				strEditText1.Format("Barcode : %s", strOnOff);
				GetDlgItem(IDC_2D_VISION)->SetWindowTextA(strEditText1);
			}
			//(n2dMarkType == e2DMarkType_Left) ? strOnOff = _T("On/LEFT") :strOnOff = _T("On/RIGHT");
		}
		else {
			strOnOff = _T("On"); 
			strEditText1.Format("Barcode : %s", strOnOff);
			GetDlgItem(IDC_2D_VISION)->SetWindowTextA(strEditText1);
		}
		
	}
	else {
// 		double dWaitTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_LOADING_TABLE_WAIT_TIME);
// 		if (dWaitTime > 0.0)
// 		{
// 			m_StaticVision2D.SetBackColor(COLOR_LLIGHTYELLOW);
// 			strOnOff.Format("Off / %.1fs", dWaitTime);
// 		}
// 		else
// 		{
			//strEditText1.Format("2D-Barcode : %s", strOnOff);
			m_StaticVision2D.SetBackColor(COLOR_LLIGHTGRAY);
			strOnOff = _T("Off");
			strEditText1.Format("Barcode : %s", strOnOff);
			GetDlgItem(IDC_2D_VISION)->SetWindowTextA(strEditText1);
/*		}*/
	
	}



// 	strEditText1.Format("2D-Barcode : %s", strOnOff);
// 	GetDlgItem(IDC_2D_VISION)->SetWindowTextA(strEditText1);
}

void CMainLowerDlg::fn_OnUpdateAutoRetest()
{
	CString strEditText1 = _T("");
	CString strOnOff = _T("");

	BOOL bUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
	if (bUseMode == eOPTION_USE) {
		m_StaticAutoRetest.SetBackColor(COLOR_GREEN);
		strOnOff = _T("On");
	}
	else {
		m_StaticAutoRetest.SetBackColor(COLOR_LLIGHTGRAY);
		strOnOff = _T("Off");
	}

	int nSelMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoRetest);
	switch (nSelMode)
	{
		case eAutoRetest_Aonly: { strOnOff += _T("(A only)"); }break;
		case eAutoRetest_AB: { strOnOff += _T("(AB)");     }break;
		case eAutoRetest_AAB: { strOnOff += _T("(AAB)");    }break;
		case eAutoRetest_AA: { strOnOff += _T("(AA)");     }break;
	}

	strEditText1.Format("ART : %s", strOnOff);
	GetDlgItem(IDC_AUTO_RETEST)->SetWindowTextA(strEditText1);
}

void CMainLowerDlg::fn_OnUpdateAutoGrr()
{
	CString strEditText1 = _T("");
	CString strOnOff = _T("");

	int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	
	
	BOOL bUseAutoGrrMode = g_DMCont.m_dmHandlerOpt.GB( BDM11_MD_AutoGrrUse );
	int nUnitCnt = g_DMCont.m_dmHandlerOpt.GN( NDM11_MD_AutoGrrUnitCount );
	int nTestCnt = g_DMCont.m_dmHandlerOpt.GN( NDM11_MD_AutoGrrTestCount );

	int nLoopTestCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoLoopTestCount);
	int nLoopUnitCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoLoopUnitCount);

	int nAuditUnitCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoAuditUnitCount);

	if (/*bUseAutoGrrMode == TRUE*/nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
	{
		m_StaticAutoGrr.SetBackColor(COLOR_GREEN);
		strOnOff.Format(_T("On/%dea/%dTimes"), nUnitCnt, nTestCnt);

		BOOL bAudit = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrAuditLoopUse);
		if (bAudit)
			strOnOff += _T("/Audit");


	}
	else if (nLotStartMode == eStartMD_AutoLoop || nLotStartMode == eStartMD_AutoLoop_Continue)
	{
		m_StaticAutoGrr.SetBackColor(COLOR_GREEN);
		strOnOff.Format(_T("On/%dTimes/%dUnit"), nLoopTestCnt, nLoopUnitCnt);
	}
	else if (nLotStartMode == eStartMD_QA || nLotStartMode == eStartMD_QA_Continue)
	{
		CString strQAStatus;
		int nQAStatus = g_DMCont.m_dmEQP.GN(NDM0_QA_Status);
		switch (nQAStatus) {
			case eQAStatus::eQAStatus_Load: {strQAStatus = _T("Load");}break;
			case eQAStatus::eQAStatus_LoadEnd: {strQAStatus = _T("LoadEnd");}break;
			case eQAStatus::eQAStatus_EmptyLoad: {strQAStatus = _T("EmptyLoad");}break;
			case eQAStatus::eQAStatus_End: {strQAStatus = _T("End");}break;
		}
		char szRetBuff[MAX_PATH] = { 0, };
		g_DMCont.m_dmHandlerOpt.GS(SDM11_QTY, szRetBuff, MAX_PATH);;
		m_StaticAutoGrr.SetBackColor(RGB(237, 160, 199));
		strOnOff.Format(_T("/QTY:%s/%s"), szRetBuff, strQAStatus);
	}
	else
	{
		m_StaticAutoGrr.SetBackColor(COLOR_LLIGHTGRAY);
		strOnOff.Format(_T("Off"));
	}

	if (nLotStartMode == eStartMD_AutoLoop || nLotStartMode == eStartMD_AutoLoop_Continue)
		strEditText1.Format("LOOPING : %s", strOnOff);
	else if(nLotStartMode == eStartMD_QA || nLotStartMode == eStartMD_QA_Continue)
		strEditText1.Format("QA_Mode%s", strOnOff);
	else
		strEditText1.Format("GRR : %s", strOnOff);

	m_StaticAutoGrr.SetWindowTextA(strEditText1);
}

void CMainLowerDlg::fn_OnUpdateVisionTestSite()
{
	CString strEditText1 = _T("");
	CString strOnOff = _T("");

	BOOL bUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_VisionUse);
	if (bUseMode == eOPTION_USE) {
		m_StaticVisionTestSite.SetBackColor(COLOR_GREEN);
		strOnOff = _T("On");
	}
	else {
// 		double dWaitTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_TESTPP_WAIT_TIME);
// 		if (dWaitTime > 0.0)
// 		{
// 			m_StaticVisionTestSite.SetBackColor(COLOR_LLIGHTYELLOW);
// 			strOnOff.Format("Off / %.2fs", dWaitTime);
// 		}
// 		else
		{
			m_StaticVisionTestSite.SetBackColor(COLOR_LLIGHTGRAY);
			strOnOff = _T("Off");
		}
	}

	strEditText1.Format("VisionTestSite : %s", strOnOff);
	GetDlgItem(IDC_TEST_SITE_VISION)->SetWindowTextA(strEditText1);
}

void CMainLowerDlg::fn_OnUpdateContinueFail()
{
	CString strEditText1 = _T("Contiue Fail : ");

	BOOL bUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_YieldCtrl);
	if (bUseMode == eOPTION_USE) {
		m_StaticContiFail.SetBackColor(COLOR_GREEN);
	}
	else {
		m_StaticContiFail.SetBackColor(COLOR_LLIGHTGRAY);
	}

	int nCount1 = g_DMCont.m_dmHandlerOpt.GN(NDM11_ContinueFail_FailCnt);
	CString strFailCnt = _T("");
	strFailCnt.Format("%d Ea", nCount1);
	strEditText1 += strFailCnt;

	GetDlgItem(IDC_CONT_FAIL)->SetWindowTextA(strEditText1);
}

void CMainLowerDlg::fn_OnUpdateRunMode()
{
	
	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);

	if (nRunMode == eRUN_MODE_ONLINE) {
		GetDlgItem(IDC_STATIC_ONOFFLINE)->SetWindowText(_T("ON-LINE"));
		m_StaticRunMode.SetBackColor(COLOR_GREEN);
	}
	else if (nRunMode == eRUN_MODE_OFFLINE) {
		m_StaticRunMode.SetBackColor(COLOR_RED);
		GetDlgItem(IDC_STATIC_ONOFFLINE)->SetWindowText(_T("OFF-LINE"));
	}
	else if (nRunMode == eRUN_MODE_DEMO) {
		m_StaticRunMode.SetBackColor(COLOR_YELLOW);
		GetDlgItem(IDC_STATIC_ONOFFLINE)->SetWindowText(_T("Demo"));
	}

	
	int nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
	if (nDeviceMode == eDeviceMode) {
		GetDlgItem(IDC_STATIC_DVC_MODE)->SetWindowText("Device Mode");
		m_StaticDvcMode.SetBackColor(COLOR_GREEN);
	}
	else if (nDeviceMode == eNoDeviceTrayExist) {
		GetDlgItem(IDC_STATIC_DVC_MODE)->SetWindowText("NoDevice/TrayExist");
		m_StaticDvcMode.SetBackColor(COLOR_YELLOW);
	}
	else if (nDeviceMode == eNoDeviceNoTray) {
		GetDlgItem(IDC_STATIC_DVC_MODE)->SetWindowText("NoDevice/NoTray");
		m_StaticDvcMode.SetBackColor(COLOR_RED);
	}
}

void CMainLowerDlg::OnTimer(UINT_PTR nIDEvent)
{
	UpdateData();

	fn_OnUpdateLotStatus();
	fn_OnUpdateRunMode();
	fn_OnUpdateYield();
	fn_OnUpdateContinueFail();
	fn_OnUpdateLotData();
	fn_OnUpdateBinView();
	fn_OnUpdateAutoSkip();
	fn_OnUpdateVision2D();
	fn_OnUpdateVisionTestSite();
	fn_OnUpdateAutoRetest();
	fn_OnUpdateAutoGrr();
	fn_OnUpdateVariance();
	fn_OnUpdateSocketClean();
	fn_OnUpdateContactForce();
//	fn_OnUpdateSwitchMode();
	fn_OnRefreshTab();

	if (nIDEvent == 0)
	{
		KillTimer(nIDEvent);
		
		// Show Lot Data Count
		{
			CString strData = _T("");

			strData.Format("%d", g_DMCont.m_dmEQP.GN(NDM0_TotalLoadCnt));
			m_SpredBinCount.SetTextMatrix(1, 1, strData);

			strData.Format("%d", g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt));
			m_SpredBinCount.SetTextMatrix(2, 1, strData);

			strData.Format("%d", g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt));
			m_SpredBinCount.SetTextMatrix(3, 1, strData);

			strData.Format("%d", g_DMCont.m_dmEQP.GN(NDM0_CurLotPassBinCnt));
			m_SpredBinCount.SetTextMatrix(4, 1, strData);

			strData.Format("%3.2f", (double)g_DMCont.m_dmEQP.GN(NDM0_CurLotPassBinCnt) / (double)g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt)*100.0);
			if (g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt) == 0) {
				m_SpredBinCount.SetTextMatrix(4, 2, "0%");
			}
			else {
				m_SpredBinCount.SetTextMatrix(4, 2, strData + "%");
			}

			strData.Format("%d", g_DMCont.m_dmEQP.GN(NDM0_CurLotFailBinCnt));
			m_SpredBinCount.SetTextMatrix(5, 1, strData);

			strData.Format("%3.2f", (double)g_DMCont.m_dmEQP.GN(NDM0_CurLotFailBinCnt) / (double)g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt)*100.0);
			if (g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt) == 0) {
				m_SpredBinCount.SetTextMatrix(5, 2, "0%");
			}
			else {
				m_SpredBinCount.SetTextMatrix(5, 2, strData + "%");
			}

			strData.Format("%d", g_DMCont.m_dmEQP.GN(NDM0_CurLotOutput1Cnt));
			m_SpredBinCount.SetTextMatrix(6, 1, strData);
			strData.Format("%3.2f", (double)g_DMCont.m_dmEQP.GN(NDM0_CurLotOutput1Cnt) / (double)g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt)*100.0);
			if (g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt) == 0) {
				m_SpredBinCount.SetTextMatrix(6, 2, "0%");
			}
			else {
				m_SpredBinCount.SetTextMatrix(6, 2, strData + "%");
			}

			strData.Format("%d", g_DMCont.m_dmEQP.GN(NDM0_CurLotOutput2Cnt));
			m_SpredBinCount.SetTextMatrix(7, 1, strData);
			strData.Format("%3.2f", (double)g_DMCont.m_dmEQP.GN(NDM0_CurLotOutput2Cnt) / (double)g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt)*100.0);
			if (g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt) == 0) {
				m_SpredBinCount.SetTextMatrix(7, 2, "0%");
			}
			else {
				m_SpredBinCount.SetTextMatrix(7, 2, strData + "%");
			}

			strData.Format("%d", g_DMCont.m_dmEQP.GN(NDM0_CurLotOutput3Cnt));
			m_SpredBinCount.SetTextMatrix(8, 1, strData);
			strData.Format("%3.2f", (double)g_DMCont.m_dmEQP.GN(NDM0_CurLotOutput3Cnt) / (double)g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt)*100.0);
			if (g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt) == 0) {
				m_SpredBinCount.SetTextMatrix(8, 2, "0%");
			}
			else {
				m_SpredBinCount.SetTextMatrix(8, 2, strData + "%");
			}

			strData.Format("%d", g_DMCont.m_dmEQP.GN(NDM0_CurLotOutput4Cnt));
			m_SpredBinCount.SetTextMatrix(9, 1, strData);
			strData.Format("%3.2f", (double)g_DMCont.m_dmEQP.GN(NDM0_CurLotOutput4Cnt) / (double)g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt)*100.0);
			if (g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt) == 0) {
				m_SpredBinCount.SetTextMatrix(9, 2, "0%");
			}
			else {
				m_SpredBinCount.SetTextMatrix(9, 2, strData + "%");
			}
		}

		// ART Cnt
		{
			for (int i = 0; i < 3; i++) {
				int nArtTtCnt = g_DMCont.m_dmEQP.GN(NDM0_ART_FT_TOTAL + (i * 2));
				m_SpredArtCnt.SetInteger(1, 1 + i, nArtTtCnt);
				int nArtGdCnt = g_DMCont.m_dmEQP.GN(NDM0_ART_FT_PASS + (i * 2));
				m_SpredArtCnt.SetInteger(2, 1 + i, nArtGdCnt);
				CString strPer = _T("");
				strPer.Format("%.2f%%", (nArtTtCnt == 0 ? 0.0 : ((double)nArtGdCnt / (double)nArtTtCnt) * 100.0));
				m_SpredArtCnt.SetTextMatrix(1 + i, 3, strPer);
			}
		}
		int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
		if (nLotStartMode == eStartMD_QA || nLotStartMode == eStartMD_QA_Continue)
		{
			GetDlgItem(IDC_BTN_GRRRESULT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BTN_QA_MODE)->ShowWindow(SW_SHOW);
		}
		else {
			GetDlgItem(IDC_BTN_GRRRESULT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BTN_QA_MODE)->ShowWindow(SW_HIDE);
		}
		

		SetTimer(0, 800, NULL);
	}

	if (nIDEvent == 1) {
		KillTimer(nIDEvent);
		OnUpdateUPH();
		SetTimer(1, 1000, NULL);
	}

	CDialog::OnTimer(nIDEvent);
}


void CMainLowerDlg::OnBnClickedBtnLotInfo()
{
	BOOL bLotState = g_DMCont.m_dmEQP.GB( BDM0_EQP_RUN_STATE );
	if( bLotState == TRUE ) {
		AfxMessageBox( _T( "Not available while running" ) );
		return;
	}

	//char szCompany[16] = { 0, };
	//g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
	CSLTView* pParentDlg = (CSLTView*)m_pParentWnd;
	MakeLog("[Lot Info Dialog Show]");

	//if (strcmp(szCompany, "AMKOR") == 0) {
	g_DMCont.m_dmEQP.SB(BDM0_OPEN_LOTINFO_DLG_HISTORY, TRUE);

	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);


	CLotInfoAmkorAPLDlg DO;
	DO.m_pSltView = pParentDlg;
	DO.SetLogInLevel(pParentDlg->m_nLogInLv);
	DO.DoModal();
	
		
	g_DMCont.m_dmEQP.SB(BDM0_OPEN_LOTINFO_DLG_HISTORY, FALSE);
	//}
	//else {
	//	CLotInfoDlg DO;
	//	DO.SetLogInLevel(pParentDlg->m_nLogInLv);
	//	DO.DoModal();
	//}
}


void CMainLowerDlg::OnBnClickedBtnClear()
{
	MakeLog("[Clear Dialog Show]");

	CClearCntDlg_64Para DO;
	DO.DoModal();
}


void CMainLowerDlg::OnBnClickedBtnLotDone()
{
	MakeLog("[Print Button Click]");

	char szFileName[512] = { 0, };
	g_DMCont.m_dmEQP.GS(SDM0_LotDoneFileName, szFileName, sizeof(szFileName));
	CString strFile = _T("");
	strFile.Format("%s\\%s", SZ_DIR_PATH_SUMMARY, szFileName);

	CString strDir = SZ_DIR_PATH_SUMMARY;
	CFileFind findfile;
	BOOL bFind = findfile.FindFile(strDir + "\\*.*");
	while (bFind)
	{
		bFind = findfile.FindNextFile();
		CString str = findfile.GetFileName();
		//_summary_production_
		if (str.Find("_summary_production_") == -1) {
			continue;
		}
		if (strcmp(szFileName, str) == 0) {
			ShellExecute(NULL, "open", strFile, NULL, NULL, SW_SHOW);
		}
		Sleep(1);
	}
}


void CMainLowerDlg::OnBnClickedBtnTraymap()
{
	MakeLog("[Tray View Dialog Show]");

	CTrayMapDlg DO;
	DO.DoModal();
}


BOOL CMainLowerDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_ESCAPE) {
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CMainLowerDlg::fn_OnUpdateBinView()
{
	char szData[MAX_PATH] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_CategoryName, szData, sizeof(szData));
	if (strcmp(m_strCategoryName, szData) != 0 || g_DMCont.m_dmEQP.GB( BDM0_CATEGORY_CHANGE ) ) {
		char szTemp[512] = { 0, };
		::GetCurrentDirectory(sizeof(szTemp), szTemp);
		CString strDir = _T("");
		strDir.Format("%s\\sys_data\\HandlerRecipe\\%s\\%s", szTemp, SZ_CATEGORY_FOLDER, szData);
		g_COK_FileMgr.LoadCategoryInfo(szData, m_stCategory);
		m_strCategoryName.Format("%s", szData);

		int nMaxCol = m_SprdOutputBinView.GetMaxCols();
		int nMaxRows = m_SprdOutputBinView.GetMaxRows();
		for (int nCols = 0; nCols < nMaxCol; nCols++) {
			for (int nRows = 0; nRows < nMaxRows - 1; nRows++) {
				m_SprdOutputBinView.SetRow(nRows);
				m_SprdOutputBinView.SetCol(nCols + 1);
				m_SprdOutputBinView.SetBackColor(COLOR_WWHITE);
				m_SprdOutputBinView.SetTextMatrix(nRows + 1, nCols + 1, _T(""));
			}
		}
		g_DMCont.m_dmEQP.SB( BDM0_CATEGORY_CHANGE, FALSE);
	}

	m_SprdOutputBinView.SetFontBold(TRUE);

	std::vector<ST_CATE_SUB_INFO> vOutput[4];
	for (int i = 0; i < m_stCategory.nIndexCount; i++)
	{
		int nHWbin = atoi(m_stCategory.aCateInfo[i].szHWBin);
		switch (nHWbin)
		{
			case 1: { vOutput[0].push_back(m_stCategory.aCateInfo[i]); } break;
			case 2: { vOutput[1].push_back(m_stCategory.aCateInfo[i]); } break;
			case 3: { vOutput[2].push_back(m_stCategory.aCateInfo[i]); } break;
			case 4: { vOutput[3].push_back(m_stCategory.aCateInfo[i]); } break;
		}
	}

	CString strOutPut = _T("");
	for (int i = 0; i < 4; i++)
	{
		if ((int)vOutput[i].size() > 0)
		{
			int nRow = 1;

			for each(auto val in vOutput[i])
			{
				int nBinColor = atoi(val.szPassFail);
				if (nBinColor == eFailBin) {
					m_SprdOutputBinView.SetRow(nRow);
					m_SprdOutputBinView.SetCol(i + 1);
					m_SprdOutputBinView.SetBackColor(COLOR_LLIGHTBROWN);
				}
				else {
					m_SprdOutputBinView.SetRow(nRow);
					m_SprdOutputBinView.SetCol(i + 1);
					m_SprdOutputBinView.SetBackColor(COLOR_GREEN);
				}
				nRow++;
			}

			nRow = 1;
			CString strHWBin_Tester;
			for (std::vector<ST_CATE_SUB_INFO>::iterator it = vOutput[i].begin(); it != vOutput[i].end(); ++it) {
				strOutPut.Empty();
				strOutPut += it->szDesc;
				if (DEF_PROJECT_WATT == m_nProjectName)
				{
					strHWBin_Tester.Format("(%s)", it->szHWBin_Tester);
					strOutPut += strHWBin_Tester;
				}
				

				m_SprdOutputBinView.SetTextMatrix(nRow, i + 1, strOutPut);
				nRow++;

			}
		}
	}
}

void CMainLowerDlg::fn_OnUpdateLotStatus()
{
	CString strData = _T("");

	int nLotState = g_DMCont.m_dmEQP.GN(NDM0_LotStatus);

	switch (nLotState)
	{
		case eAutoLotState_InitNeed: { strData.Format("InitNeed");						} break;
		case eAutoLotState_Initialize: { strData.Format("Initialize");					} break;
		case eAutoLotState_Idle: { strData.Format("Idle");							} break;
		case eAutoLotState_Executing: { strData.Format("Executing");					} break;
		case eAutoLotState_Pause: { strData.Format("Pause");						} break;
		case eAutoLotState_Resume: { strData.Format("Resume");						} break;
		case eAutoLotState_AlarmStop: { strData.Format("AlarmStop");					} break;
		case eAutoLotState_NormalComplete: { strData.Format("NormalComplete");				} break;
		case eAutoLotState_CleanOut: { strData.Format("CleanOut");						} break;
	}

	SetDlgItemText(IDC_STATIC_LOT_STATUS, strData);

	switch (nLotState)
	{
		case eAutoLotState_InitNeed:
		case eAutoLotState_Pause:
		case eAutoLotState_AlarmStop:
		case eAutoLotState_NormalComplete:
			{
				strData.Format("STOP");
				m_StaticRunStop.SetBackColor(COLOR_RED);
			}break;
		case eAutoLotState_Initialize:
		case eAutoLotState_Idle:
		case eAutoLotState_Executing:
		case eAutoLotState_Resume:
			{
				if (g_DMCont.m_dmEQP.GB(BDM0_SYS_AUTO_GRR_CANCEL) && g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse)) {
					strData.Format("GRR Cancel");
					m_StaticRunStop.SetBackColor(COLOR_YELLOW);
					break;
				}

				if (g_DMCont.m_dmEQP.GB(BDM0_SYS_AUTO_LOOP_CANCEL) && g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoLoopUse)) {
					strData.Format("Loop Cancel");
					m_StaticRunStop.SetBackColor(COLOR_YELLOW);
					break;
				}

				if (g_DMCont.m_dmEQP.GB(BDM0_SYS_AUTO_AUDIT_CANCEL) && g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoAuditUse)) {
					strData.Format("Audit Cancel");
					m_StaticRunStop.SetBackColor(COLOR_YELLOW);
					break;
				}
			
				m_StaticRunStop.SetBackColor( COLOR_GREEN );
				if( g_DMCont.m_dmEQP.GB(BDM0_SYS_ONE_CYCLE) != TRUE )
					strData.Format("START");
				else {
					if (g_DMCont.m_dmEQP.GB(BDM0_CLEAN_DEVICE_SKIP_SYS_ONE_CYCLE) != TRUE)
					{
						strData.Format("ONE CYCLE");
						m_StaticRunStop.SetBackColor(COLOR_YELLOW);
					}
					else {					
						strData.Format(_T("CLEAN SKIP"));
						m_StaticRunStop.SetBackColor(COLOR_YELLOW);
					}

				} 
			}break;
		case eAutoLotState_CleanOut:
			{
				strData.Format("CLEAN OUT");
				m_StaticRunStop.SetBackColor(COLOR_LLIGHTBROWN);
			}break;
	}

	SetDlgItemText(IDC_STATIC_RUNSTOP, strData);
}

void CMainLowerDlg::OnBnClickedBtnVibrator()
{
	CVibratorDlg DO;
	DO.DoModal();
}



void CMainLowerDlg::OnUpdateUPH()
{
	int nUPH = g_DMCont.m_dmEQP.GN(NDM0_UPH);
	int nGrossUPH = g_DMCont.m_dmEQP.GN(NDM0_GROSS_UPH);
	int nStopTm = g_DMCont.m_dmEQP.GN(NDM0_LotStopTime);
	int nRunTm = g_DMCont.m_dmEQP.GN(NDM0_LotRunTime);
	int nJamCnt = g_DMCont.m_dmEQP.GN(NDM0_CurLotJamCount);
	int nLotSort = g_DMCont.m_dmEQP.GN(NDM0_CurLotSortingCnt);
	int nCycleUPH = g_DMCont.m_dmEQP.GN(NDM0_CYCLE_UPH);
	int bCycleUPH = g_DMCont.m_dmEQP.GB(BDM0_CYCLE_UPH_CHECK);

	CString strUPH = _T("");
	if (nGrossUPH <= 0) { nGrossUPH = 0; }
	// all time uph

	if (bCycleUPH == TRUE && nCycleUPH > 0)
	{
		strUPH.Format("Cycle UPH : %d", nCycleUPH);
	}
	else {
		strUPH.Format("Cycle UPH : calculation");
	}

	SetDlgItemText(IDC_STATIC_CYCLE_UPH, strUPH);

	strUPH.Format("Gross UPH : %d", nGrossUPH);
	SetDlgItemText(IDC_STATIC_UPH_GROSS, strUPH);

	// uph ( minus stop time )
	if (nUPH <= 0) { nUPH = 0; }

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));


	strUPH.Format("UPH : %d", nUPH);
	SetDlgItemText(IDC_STATIC_UPH, strUPH);
	

	// lot stop time
	int nHour = nStopTm / 3600;
	int nMin = (nStopTm - (nHour * 3600)) / 60;
	int nSec = (nStopTm - (nHour * 3600) - (nMin * 60));
	strUPH.Format("STOP TIME : %d:%02d:%02d", nHour, nMin, nSec);
	SetDlgItemText(IDC_STATIC_STOP_TIME, strUPH);

	// Lot run time
	nHour = nRunTm / 3600;
	nMin = (nRunTm - (nHour * 3600)) / 60;
	nSec = (nRunTm - (nHour * 3600) - (nMin * 60));
	strUPH.Format("LOT TIME : %d:%02d:%02d", nHour, nMin, nSec);
	SetDlgItemText(IDC_STATIC_RUN_TIME, strUPH);

	// lot jam rate
	strUPH.Format("JamRate : %d / %d", nJamCnt, nLotSort);
	SetDlgItemText(IDC_STATIC_CUR_JAM, strUPH);

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
		strUPH.Format("Total Index : %.2f", dCalc);
	}
	else {
		strUPH.Format("Total Index : calculation" );
	}
	
	SetDlgItemText(IDC_STATIC_SUM_INDEX, strUPH);
}
void CMainLowerDlg::fn_OnRefreshTab()
{
	switch( m_tabResult.GetCurSel() )
	{
	case MTPI_PASSFAIL: m_pgPassFail.Refresh(); break;
	case MTPI_YIELD_PER: m_pgYieldPer.Refresh(); break;
	case MTPI_YIELD_CNT: m_pgYieldCnt.Refresh(); break;
	case MTPI_LAST_DESC: m_pgLastDesc.Refresh(); break;
	//case MTPI_DOOR_STATUS: m_pgDoorStatus.Refresh(); break;
	case MTPI_YIELD_1ST_FAIL: m_pgYield1stFail.Refresh(); break;
	case MTPI_YIELD_2DID_FAIL: m_pgYield2DIDFail.Refresh(); break;
	}
}

void CMainLowerDlg::OnClose()
{
	KillTimer(0);
	KillTimer(1);

	CDialog::OnClose();
}


void CMainLowerDlg::MakeLog(LPCTSTR fmt, ...)
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

void CMainLowerDlg::OnBnClickedBtnGrrresult()
{
	MakeLog( "[GRR Result Dialog Show]" );

	if (m_dlgGrrResult->IsWindowVisible() == FALSE)
	{
		m_dlgGrrResult->Finalize();
		m_dlgGrrResult->Initialize();
		m_dlgGrrResult->ShowWindow(SW_SHOW);
	}
	
	//CAutoGrrResultDlg DO;
	//DO.DoModal();
	
}

void CMainLowerDlg::fn_OnUpdateSwitchMode()
{
// 	BOOL bFrnotMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_FrontSW);
// 	if (bFrnotMode == DEF_ON) {
// 		m_StaticSwitch.SetBackColor(COLOR_GREEN);
// 		GetDlgItem(IDC_STATIC_SWITCH)->SetWindowTextA("Front Switch Mode");
// 	}
// 	else if (bFrnotMode == DEF_OFF) {
// 		m_StaticSwitch.SetBackColor(COLOR_YELLOW);
// 		GetDlgItem(IDC_STATIC_SWITCH)->SetWindowTextA("Rear Switch Mode");
// 	}
// 	else {
// 		m_StaticSwitch.SetBackColor(COLOR_NOMAL);
// 		GetDlgItem(IDC_STATIC_SWITCH)->SetWindowTextA("Switch Mode");
// 	}
}

void CMainLowerDlg::OnBnClickedBtnSummary()
{
	MakeLog("[Make Summary Button Click]");
	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
	if (bLotState == eHandlerLotMode_OnGoing) {
		AfxMessageBox("Make Summary is only Lot End");
		MakeLog("[Make Summary Fail Not Lot End]");
		return;
	}
	if (g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt) == 0) {
		AfxMessageBox("Can not make Summary. ");
		MakeLog("[Make Summary Fail Load Cnt =0]");
		return;
	}

	char szBatchPathBuff[MAX_PATH] = { 0, }, szLotNo[MAX_PATH] = { 0, };

	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);

	if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_BatchFile_Use) == DEF_ON && nRunMode == eRUN_MODE_ONLINE) {
		g_DMCont.m_dmHandlerOpt.GS(SDM11_BatchFile_Path, szBatchPathBuff, MAX_PATH);
		CFileFind findfile;
		BOOL bFind = findfile.FindFile(szBatchPathBuff);
		if (!bFind) {
			AfxMessageBox("No batch file found.");
			MakeLog("[Batch File Use Mode ON, but batch file not found!!]");
			return;
		}
	}

	int nRet = AfxMessageBox("Do you want make Summary?", MB_TOPMOST | MB_YESNO);

	if (nRet != IDYES) {
		MakeLog("[Make Summary Button Click YES/NO [NO] ]");
		return;
	}
	MakeLog("[Make Summary Button Click YES/NO [YES] ]");
	g_McComm.MakeSummary();

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
	if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
		int nErr = ObsidianFileCreate();
		if(nErr != NO_ERROR)
			MakeLog("[Make Summary Button Click ObsidianFileCreate Fail ]");

		nErr = DeleteLogFileCreate();
		if (nErr != NO_ERROR)
			MakeLog("[Make Summary Button Click DeleteLogFileCreate Fail ]");

	}

	for (int i = 0; i < eMaxPressUnitCount; i++)
	{
		g_DMCont.m_dmHandlerOpt.SB(BDM11_MANU_CLN_REV_SITE1 + i, FALSE);
	}
	

	if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_BatchFile_Use) == DEF_ON && nRunMode == eRUN_MODE_ONLINE) {
		g_DMCont.m_dmHandlerOpt.GS(SDM11_LotNo, szLotNo, MAX_PATH);

		int nErr = (int)ShellExecute(NULL, "open", szBatchPathBuff, szLotNo, NULL, SW_SHOWNORMAL);
		if (nErr <= 32) { // ShellExecute return 값 32이하 면 Error
			CString strErrMsg = _T("Mac Log Copy Batch File 을 실행 하지 못하였습니다. Mac Log 이동 바랍니다");
			AfxMessageBox(strErrMsg);
			MakeLog("[Failed to execute Batch File.]");
		}
	}

	//2D Mark Ignore 초기화.
	g_DMCont.m_dmHandlerOpt.SS(SDM11_2D_MARK_IGNORE_REL_TEST, "", MAX_PATH);
}


HBRUSH CMainLowerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	//if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {

		if (pWnd->GetDlgCtrlID() == IDC_LIST_LOTDATA /* 색 바꾸려는 컨트롤의 id*/)
		{
			hbr = (HBRUSH)::GetSysColorBrush(COLOR_WINDOW);  // 배경 브러쉬

			int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
			if (nRunMode == eRUN_MODE_ONLINE) {
// 				int nMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_TESTER_IF_PRO_AUDIT);
// 				switch (nMode)
// 				{
// 				case eTesterIF_Mode_Audit:
// 				{
// 					pDC->SetBkColor(RGB(237, 160, 199)); // 배경색
// 					hbr = m_hBrushPink;
// 				}break;
// 				case eTesterIF_Mode_Grr:
// 				{
// 					pDC->SetBkColor(RGB(100, 100, 199)); // 배경색
// 					hbr = m_hBrushBlue;
// 				}break;
// 				case eTesterIF_Mode_LoopTest:
// 				{
// 					pDC->SetBkColor(RGB(199, 160, 237)); // 배경색
// 					hbr = m_hBrushPurple;
// 				}break;
// 				default:
// 					pDC->SetBkColor(RGB(47, 237, 40)); // 배경색
// 					hbr = m_hBrushGreen;
// 					break;
// 				}
				int nRetCtrl = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
				switch (nRetCtrl)
				{
				case eStartMD_QA:
				case eStartMD_QA_Continue:
				{
					pDC->SetBkColor(RGB(237, 160, 199)); // 배경색
					hbr = m_hBrushPink;
				}break;
				case eStartMD_AutoGRR:
				case eStartMD_AutoGRR_Continue:
				{
					pDC->SetBkColor(RGB(100, 100, 199)); // 배경색
					hbr = m_hBrushBlue;
				}break;
// 				case eTesterIF_Mode_LoopTest:
// 				{
// 					pDC->SetBkColor(RGB(199, 160, 237)); // 배경색
// 					hbr = m_hBrushPurple;
// 				}break;
				default:
					pDC->SetBkColor(RGB(47, 237, 40)); // 배경색
					hbr = m_hBrushGreen;
					break;
				}
			}
		}
	//}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void CMainLowerDlg::OnBnClickedBtnLog()
{
	MakeLog("[Click Event log");
	CEventLogDlg DO;
	DO.DoModal();
}


void CMainLowerDlg::CompanySetting()
{
	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
	
	//	GetDlgItem(IDC_AUTO_CLEAN)->ShowWindow(SW_SHOW);
	//	GetDlgItem(IDC_STATIC_AUTOGRR)->ShowWindow(SW_SHOW);
	//	GetDlgItem(IDC_BTN_GRRRESULT)->ShowWindow(SW_SHOW);
	//}
	//else if (strcmp(szCompany, DEF_COMPANY_USI) == 0) {
	//	GetDlgItem(IDC_AUTO_CLEAN)->ShowWindow(SW_HIDE);
	//	GetDlgItem(IDC_STATIC_AUTOGRR)->ShowWindow(SW_HIDE);
	//	GetDlgItem(IDC_BTN_GRRRESULT)->ShowWindow(SW_HIDE);
	//}

	/*if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
		GetDlgItem(IDC_AUTO_CLEAN)->ShowWindow(SW_SHOW);
	}
	else if (strcmp(szCompany, DEF_COMPANY_USI) == 0) {
		GetDlgItem(IDC_AUTO_CLEAN)->ShowWindow(SW_HIDE);
	}*/

	GetDlgItem(IDC_AUTO_CLEAN)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_AUTOGRR)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BTN_GRRRESULT)->ShowWindow(SW_SHOW);
}

void CMainLowerDlg::OnBnClickedButton2didCsv()
{
	CString strPath = "D:\\ProductCSV\\Temporary\\";
	CFileFind finder;

	BOOL bWorking = finder.FindFile(strPath + "*.*");

	CString fileName;

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsArchived())
		{
			CString _filename = finder.GetFileName();

			if(_filename == "." || _filename == ".." || _filename == "Thumbs.db")
				continue;
			
			fileName = finder.GetFileName();
		}
	}
	strPath += fileName;
	ShellExecute(NULL, "open", strPath, NULL, NULL, SW_SHOW);
}


void CMainLowerDlg::OnBnClickedButton2didCsvView()
{

}


void CMainLowerDlg::OnBnClickedButtonSummary()
{
	CTime cTime = CTime::GetCurrentTime();

	CString strPath = _T("");
	strPath.Format("D:\\Techwing\\Summary\\%04d\\%d\\", cTime.GetYear(), cTime.GetMonth());
	CFileFind finder;

	ShellExecute(NULL, "open", strPath, NULL, NULL, SW_SHOW);
}


void CMainLowerDlg::OnStnClickedAutoRetest()
{
	if (CheckEQPRunning() == TRUE)
		return;

	
	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
	if (bLotState == eHandlerLotMode_OnGoing) {
		AfxMessageBox(_T("Not available while running"));
		return;
	}

	fn_OnOffControl(BDM11_MD_AutoRetestSiteUse);
}


void CMainLowerDlg::OnStnClickedTestSiteVision()
{
	if (CheckEQPRunning() == TRUE)
		return;

	fn_OnOffControl(BDM11_MD_VisionUse);
}


void CMainLowerDlg::OnStnClickedYieldCtrl()
{
	if (CheckEQPRunning() == TRUE)
		return;

	fn_OnOffControl(BDM11_MD_YieldCtrl);
}


void CMainLowerDlg::OnStnClickedAutoSkip()
{
	if (CheckEQPRunning() == TRUE)
		return;

	fn_OnOffControl(BDM11_MD_AutoSkipUse);
}


void CMainLowerDlg::OnStnClicked2dVision()
{
	if (CheckEQPRunning() == TRUE)
		return;

	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
	if (bLotState == eHandlerLotMode_OnGoing && nRunMode == eRUN_MODE_ONLINE) {
		AfxMessageBox(_T("Not available while running"));
		return;
	}

	fn_OnOffControl(BDM11_MD_2D_BarCode_Use);
}


void CMainLowerDlg::OnStnClickedAutoClean()
{
	if (CheckEQPRunning() == TRUE)
		return;

	fn_OnOffControl(BDM11_MD_SocketClean);
}

void CMainLowerDlg::fn_OnOffControl(int nUseModeNum)
{
	BOOL bUseMode = g_DMCont.m_dmHandlerOpt.GB(nUseModeNum);
	bUseMode = !bUseMode;

	if (bUseMode)
	{
		int nRetLevel = 0;
		BOOL bRet = g_LibCommCtrl.ShowLogInBox(0, &nRetLevel);

		if (RETURN_OK == bRet && nRetLevel >= eUSERLEVEL_TECHNICIAN) {
		}
		else {
			AfxMessageBox("User Level is low!!");
			return;
		}
	}

	if (bUseMode == eOPTION_USE)
	{
		g_DMCont.m_dmHandlerOpt.SB(nUseModeNum, TRUE);
	}
	else
	{
		g_DMCont.m_dmHandlerOpt.SB(nUseModeNum, FALSE);
	}

	char szCokName[512] = { 0, };
	ST_BASIC_INFO   stBasicData;
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
	g_COK_FileMgr.LoadBasicInfo(szCokName, stBasicData);

	switch (nUseModeNum)
	{
	case eAutoRetestSiteUse:
		stBasicData.bAutoRetestSiteUse = bUseMode;
		if (bUseMode)
		{
			MakeLog("[MENU = Input Auto Retest Use ][BEFORE= Auto Retest Off, AFTER= Auto Retest On]");
		}
		else
		{
			MakeLog("[MENU = Input Auto Retest Use ][BEFORE= Auto Retest On, AFTER= Auto Retest Off]");
		}
		break;

	case eAutoSkipUse:
		stBasicData.bAutoSkipMode = bUseMode;
		if (bUseMode)
		{
			MakeLog("[MENU = Input Auto Skip ][BEFORE= Auto Skip Off, AFTER= Auto Skip On]");
		}
		else
		{
			MakeLog("[MENU = Input Auto Skip ][BEFORE= Auto Skip On, AFTER= Auto Skip Off]");
		}
		break;

	case eVisionUse:
		stBasicData.nVision = bUseMode;
		if (bUseMode)
		{
			MakeLog("[MENU = Input Vision Use ][BEFORE= Vision Off, AFTER= Vision On]");
		}
		else
		{
			MakeLog("[MENU = Input Vision Use ][BEFORE= Vision On, AFTER= Vision Off]");
		}
		break;

// 	case e2DBarcodeUse:
// 		//stBasicData.b2dBarcode = bUseMode;
// 		if (bUseMode)
// 		{
// 			MakeLog("[MENU = Input 2D_Barcode Use ][BEFORE= 2D_Barcode Off, AFTER= 2D_Barcode On]");
// 		}
// 		else
// 		{
// 			MakeLog("[MENU = Input 2D_Barcode Use ][BEFORE= 2D_Barcode On, AFTER= 2D_Barcode Off]");
// 		}
// 		break;

	case eAutoCleanUse:
		stBasicData.bMD_SocketClean = bUseMode;
		if (bUseMode)
		{
			MakeLog("[MENU = Input Auto Clean Use ][BEFORE= Auto Clean Off, AFTER= Auto Clean On]");
		}
		else
		{
			MakeLog("[MENU = Input Auto Clean Use ][BEFORE= Auto Clean On, AFTER= Auto Clean Off]");
		}
		break;

	case eYieldCtrlUse:
		if (bUseMode)
		{
			MakeLog("[MENU = Input Yield Control Use ][BEFORE= Yield Control Off, AFTER= Yield Control On]");
		}
		else
		{
			MakeLog("[MENU = Input Yield Control Use ][BEFORE= Yield Control On, AFTER= Yield Control Off]");
		}
		stBasicData.bYieldMonitor = bUseMode;
	}

	g_COK_FileMgr.SaveBasicInfo(szCokName, &stBasicData);

	//Barcode는 COK File에 저장.
	if (nUseModeNum == e2DBarcodeUse)
	{
		if (bUseMode)
		{
			MakeLog("[MENU = Input 2D_Barcode Use ][BEFORE= 2D_Barcode Off, AFTER= 2D_Barcode On]");
		}
		else
		{
			MakeLog("[MENU = Input 2D_Barcode Use ][BEFORE= 2D_Barcode On, AFTER= 2D_Barcode Off]");
		}

		ST_COK_INFO     stCokData;
		g_COK_FileMgr.LoadCokInfo(szCokName, stCokData);

		stCokData.b2dBarcodeUse = bUseMode;

		g_COK_FileMgr.SaveCokInfo(szCokName, &stCokData);
	}

}

BOOL CMainLowerDlg::CheckEQPRunning()
{
	BOOL bRet = FALSE;
	if (g_DMCont.m_dmEQP.GB(BDM0_EQP_RUN_STATE)) {
		AfxMessageBox(_T("Not available while running"));
		bRet = TRUE;
	}
	return bRet;
}

void CMainLowerDlg::OnBnClickedBtnOee()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	COEE cOEEData;
	cOEEData.InitData();
	
	CString strPath = _T("");
	strPath.Format("D:\\Techwing\\OEE");
	ShellExecute(NULL, "open", strPath, NULL, NULL, SW_SHOW);
}


void CMainLowerDlg::OnBnClickedButtonSocketDiagnosis()
{

}

int CMainLowerDlg::ObsidianFileCreate()
{
	CFileFind cFileFind;
	CString strTargetPath, strTargetFullPath;
	FILE *fp = NULL;

	char szBuffer[256] = { 0, };

	g_DMCont.m_dmHandlerOpt.GS(SDM0_OBSIDIAN_CREATE_PATH, szBuffer, sizeof(szBuffer));

	strTargetPath.Format("%s", szBuffer);
	strTargetFullPath.Format("%s\\Obsidian.txt", strTargetPath);

	if (GetFileAttributes(strTargetPath) == INVALID_FILE_ATTRIBUTES)
	{
		return 1;
	}

	if (cFileFind.FindFile(strTargetFullPath))
	{
		DeleteFile(strTargetFullPath);
	}

	cFileFind.Close();

	fopen_s(&fp, strTargetFullPath, "w");
	if (fp)
	{
		fputs("True", fp);

	}

	fclose(fp);

	return NO_ERROR;
}

int CMainLowerDlg::DeleteLogFileCreate()
{
	CFileFind cFileFind;
	CString strTargetPath, strTargetFullPath;
	FILE *fp = NULL;

	char szBuffer[256] = { 0, };

	g_DMCont.m_dmHandlerOpt.GS(SDM0_DELETE_LOG_CREATE_PATH, szBuffer, sizeof(szBuffer));

	strTargetPath.Format("%s", szBuffer);
	strTargetFullPath.Format("%s\\log_delete.txt", strTargetPath);

	if (GetFileAttributes(strTargetPath) == INVALID_FILE_ATTRIBUTES)
	{
		return 1;
	}

	if (cFileFind.FindFile(strTargetFullPath))
	{
		DeleteFile(strTargetFullPath);
	}

	cFileFind.Close();

	fopen_s(&fp, strTargetFullPath, "w");
	if (fp)
	{
		fputs("True", fp);

	}

	fclose(fp);

	return NO_ERROR;
}

void CMainLowerDlg::OnBnClickedBtnHandlerLog()
{
	CTime cTmCurrent = CTime::GetCurrentTime();
	CString strPath;
	strPath.Format("D:\\Techwing\\HandlerLog\\%4d%02d%02d", cTmCurrent.GetYear(), cTmCurrent.GetMonth(), cTmCurrent.GetDay());
	CFileFind finder;

	BOOL bWorking = finder.FindFile(strPath + "*.*");

	CString fileName;

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsArchived())
		{
			CString _filename = finder.GetFileName();

			if (_filename == "." || _filename == ".." || _filename == "Thumbs.db")
				continue;

			fileName = finder.GetFileName();
		}
	}
	strPath += fileName;
	ShellExecute(NULL, "open", strPath, NULL, NULL, SW_SHOW);
}


// void CMainLowerDlg::OnStnClickedStaticSwitch()
// {
// 	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
// 	g_McComm.OnSendDlgShow(eDlgProcCmd_ServoMotor);
// }


BOOL CMainLowerDlg::IsEQPRunning()
{
	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_EQP_RUN_STATE);
	if (bLotState == TRUE) {
		AfxMessageBox(_T("Not available while running"));
		return TRUE;
	}
	return FALSE;
}

void CMainLowerDlg::OnBnClickedBtnServoMotor()
{
	if (IsEQPRunning() == TRUE)
		return;

	MakeLog("[Motor Monitor Dialog Show]");
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//g_McComm.OnSendDlgShow(eDlgProcCmd_ServoMotor);

	//임시
	g_McComm.OnSendDlgShow(eDlgProcCmd_VAT);
}



void CMainLowerDlg::OnBnClickedButtonBatchFile()
{
	char szBatchFilePath[MAX_PATH] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_BatchFile_Path, szBatchFilePath, sizeof(szBatchFilePath));

	char szBatPath[MAX_PATH] = { 0, };

	sprintf_s(szBatPath, sizeof(szBatPath), "%s", szBatchFilePath);
	int nErr = (int)ShellExecute(NULL, "open", szBatPath, NULL, NULL, SW_SHOWNORMAL);
	if (nErr <= 32) { // ShellExecute return 값 32이하 면 Error
		g_McComm.OnSendDlgShow(eDlgProcCmd_BatchFileErr);
	}
}


void CMainLowerDlg::OnBnClickedBtnQaMode()
{
	MakeLog("[QA Mode Set Dialog Show]");

	CAutoQASetDlg dlg;
	dlg.DoModal();

}
