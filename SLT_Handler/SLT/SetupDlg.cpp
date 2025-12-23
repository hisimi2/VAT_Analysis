// SetupDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "SetupDlg.h"
#include "afxdialogex.h"


// CSetupDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupDlg, CDialog)

CSetupDlg::CSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupDlg::IDD, pParent)
{
	m_bBeforeAutoSkip = FALSE;
	m_bAutoSkip = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoSkipUse);
	m_nAutoSkipCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_AutoSkipCount);
	m_nLogflag = FALSE;
}

CSetupDlg::~CSetupDlg()
{

}

void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_OFFSET, m_BtnOffset);
	DDX_Control(pDX, IDC_BUTTON_SPEED_TRAY_PP, m_BtnSpeed_TrayPP);
	DDX_Control(pDX, IDC_BUTTON_SITE_SET, m_BtnSiteSetting);
	DDX_Control(pDX, IDC_BUTTON_CATE_SETUP, m_BtnCategorySetup);
	DDX_Control(pDX, IDC_BUTTON_IF_TEST, m_BtnIFSetting);
	DDX_Control(pDX, IDC_BUTTON_DEVICE_SETUP, m_BtnDvcSetup);
	DDX_Control(pDX, IDC_BUTTON_SLT_PGM, m_BtnSLTPgm);
	DDX_Control(pDX, IDC_BUTTON_YIELD_CTRL, m_BtnYieldCtrl);
	DDX_Control(pDX, IDC_BUTTON_YIELD_MONITOR, m_BtnYieldMonitor);
	DDX_Control(pDX, IDC_BUTTON_VIBRATOR, m_BtnVibrator);
	DDX_Control(pDX, IDC_BUTTON_CCD_SETUP, m_BtnVision);
	DDX_Control(pDX, IDC_BUTTON_IO_MONITOR, m_BtnIOMonitor);
	DDX_Control(pDX, IDC_BUTTON_POS_CHECK, m_BtnPosCheck);
	DDX_Control(pDX, IDC_BUTTON_EVENT_LOG, m_BtnEventLog);
	DDX_Control(pDX, IDC_BUTTON_TRAY_CHANGE_FILE, m_BtnChangeFile);
	DDX_Control(pDX, IDC_BUTTON_SPEED_TEST_PP, m_BtnSpeed_TestPP);
	DDX_Control(pDX, IDC_BUTTON_2D_BARCODE_VALIDATE, m_Btn2dBarcodeValidate);
	DDX_Control(pDX, IDC_BUTTON_SPEED_SHUTTLE, m_BtnSpeed_LoadTbl);
	DDX_Control(pDX, IDC_BUTTON_SPEED_CONTACTOR, m_BtnSpeed_PressUnit);
	DDX_Control(pDX, IDC_BUTTON_SPEED_ALL, m_BtnSpeedAll);
	DDX_Control(pDX, IDC_BUTTON_SPEED_TANSFER, m_BtnTransfer);
    DDX_Control(pDX, IDC_BUTTON_SOCKET_CLEAN, m_BtnSocketClean);
	DDX_Control(pDX, IDC_BUTTON_TOWER, m_BtnTower);
	DDX_Control(pDX, IDC_BUTTON_DATA_ASSIGN, m_BtnDataAssign);
	DDX_Check(pDX, IDC_CHECK_AUTO_SKIP2, m_bAutoSkip);
	DDX_Control(pDX, IDC_EDIT_AUTO_SKIP_CNT2, m_CtrlAutoSkipCnt);
	DDX_Text(pDX, IDC_EDIT_AUTO_SKIP_CNT2, m_nAutoSkipCnt);
	DDX_Control(pDX, IDC_CHECK_AUTO_SKIP2, m_BtnAutoSkip);

	DDX_Control(pDX, IDC_BUTTON_SPEED_FEEDER, m_BtnSpeed_Feeder);
	DDX_Control(pDX, IDC_BUTTON_BARCODE_SETTING, m_BtnBarcodeSetting);
	
	DDX_Control(pDX, IDC_BUTTON_SOCKET_COUNT, m_BtnSocketCount);
	DDX_Control(pDX, IDC_BUTTON_DAILY_REPORT, m_BtnDailyReport);
	DDX_Control(pDX, IDC_BUTTON_GRR_SETTING_SETUP, m_BtnGrrSetting);
	DDX_Control(pDX, IDC_BUTTON_LOOP_SETTING_SETUP, m_BtnLoopSetting);

	DDX_Control(pDX, IDC_BUTTON_QA_SETTING_SETUP, m_BtnQASetting);
	
}

BOOL CSetupDlg::CheckEQPRunning()
{
	BOOL bRet = FALSE;
	if( g_DMCont.m_dmEQP.GB( BDM0_EQP_RUN_STATE ) ) {
		AfxMessageBox( _T( "Not available while running" ) );
		bRet = TRUE;
	}
	return bRet;
}


BEGIN_MESSAGE_MAP(CSetupDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_OFFSET, &CSetupDlg::OnBnClickedButtonOffset)
	ON_BN_CLICKED(IDC_BUTTON_SPEED_TRAY_PP, &CSetupDlg::OnBnClickedButtonSpeed)
	ON_BN_CLICKED(IDC_BUTTON_SITE_SET, &CSetupDlg::OnBnClickedButtonSiteSet)
	ON_BN_CLICKED(IDC_BUTTON_CATE_SETUP, &CSetupDlg::OnBnClickedButtonCateSetup)
	ON_BN_CLICKED(IDC_BUTTON_IF_TEST, &CSetupDlg::OnBnClickedButtonIfTest)
	ON_BN_CLICKED(IDC_BUTTON_DEVICE_SETUP, &CSetupDlg::OnBnClickedButtonDeviceSetup)
	ON_BN_CLICKED(IDC_BUTTON_SLT_PGM, &CSetupDlg::OnBnClickedButtonSltPgm)
	ON_BN_CLICKED(IDC_BUTTON_YIELD_CTRL, &CSetupDlg::OnBnClickedButtonYieldCtrl)
	ON_BN_CLICKED(IDC_BUTTON_IO_MONITOR, &CSetupDlg::OnBnClickedButtonIoMonitor)
	ON_BN_CLICKED(IDC_BUTTON_POS_CHECK, &CSetupDlg::OnBnClickedButtonPosCheck)
	ON_BN_CLICKED(IDC_BUTTON_EVENT_LOG, &CSetupDlg::OnBnClickedButtonEventLog)
	ON_BN_CLICKED(IDC_BUTTON_SPEED_SHUTTLE, &CSetupDlg::OnBnClickedButtonSpeedShuttle)
	ON_BN_CLICKED(IDC_BUTTON_SPEED_CONTACTOR, &CSetupDlg::OnBnClickedButtonSpeedContactor)
	ON_BN_CLICKED(IDC_BUTTON_SPEED_TEST_PP, &CSetupDlg::OnBnClickedButtonSpeedTestPp)
	ON_BN_CLICKED(IDC_BUTTON_SPEED_ALL, &CSetupDlg::OnBnClickedButtonSpeedAll)
	ON_BN_CLICKED(IDC_BUTTON_SPEED_TANSFER, &CSetupDlg::OnBnClickedButtonSpeedTansfer)
	ON_BN_CLICKED(IDC_BUTTON_TRAY_SETUP, &CSetupDlg::OnBnClickedButtonTraySetup)
	ON_BN_CLICKED(IDC_BUTTON_YIELD_MONITOR, &CSetupDlg::OnBnClickedButtonYieldMonitor)
	ON_BN_CLICKED(IDC_BUTTON_CCD_SETUP, &CSetupDlg::OnBnClickedButtonCcdSetup)
	ON_BN_CLICKED(IDC_BUTTON_VIBRATOR, &CSetupDlg::OnBnClickedButtonVibrator)
    ON_BN_CLICKED(IDC_BUTTON_AIO_MONITOR, &CSetupDlg::OnBnClickedButtonAioMonitor)
    ON_BN_CLICKED(IDC_BUTTON_SOCKET_CLEAN, &CSetupDlg::OnBnClickedButtonSocketClean)
	ON_BN_CLICKED(IDC_BUTTON_FTP, &CSetupDlg::OnBnClickedButtonFtp)
	ON_BN_CLICKED(IDC_BUTTON_TOWER, &CSetupDlg::OnBnClickedButtonTower)
	ON_BN_CLICKED(IDC_BUTTON_DATA_ASSIGN, &CSetupDlg::OnBnClickedButtonDataAssign)
	ON_BN_CLICKED(IDC_BUTTON_OFFSET_TRAY, &CSetupDlg::OnBnClickedButtonOffsetTray)
	ON_BN_CLICKED(IDC_BUTTON_TRAY_ASSIGN, &CSetupDlg::OnBnClickedButtonTrayAssign)
	ON_BN_CLICKED(IDC_CHECK_AUTO_SKIP2, &CSetupDlg::OnBnClickedCheckAutoSkip2)
	ON_EN_SETFOCUS(IDC_EDIT_AUTO_SKIP_CNT2, &CSetupDlg::OnEnSetfocusEditAutoSkipCnt2)
	ON_BN_CLICKED(IDC_BUTTON_SOCKET_COUNT, &CSetupDlg::OnBnClickedButtonSocketCount)
	ON_BN_CLICKED(IDC_BUTTON_DAILY_REPORT, &CSetupDlg::OnBnClickedButtonDailyReport)
	ON_BN_CLICKED(IDC_BUTTON_GRR_SETTING_SETUP, &CSetupDlg::OnBnClickedButtonGrrSettingSetup)
	ON_BN_CLICKED(IDC_BUTTON_CB_DATA_OPSTION, &CSetupDlg::OnBnClickedButtonCbDataOpstion)
	ON_BN_CLICKED(IDOK, &CSetupDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_LOOP_SETTING_SETUP, &CSetupDlg::OnBnClickedButtonLoopSettingSetup)
	ON_BN_CLICKED(IDC_BUTTON_AUDIT_SETTING_SETUP, &CSetupDlg::OnBnClickedButtonAuditSettingSetup)
	ON_MESSAGE(UM_AUTO_SKIP_UPDATE, OnReceivedMsg)

	ON_BN_CLICKED(IDC_BUTTON_IONIZER, &CSetupDlg::OnBnClickedButtonIonizer)
	ON_BN_CLICKED(IDC_BUTTON_SPEED_FEEDER, &CSetupDlg::OnBnClickedButtonSpeedFeeder)
	ON_BN_CLICKED(IDC_BUTTON_BARCODE_SETTING, &CSetupDlg::OnBnClickedButtonBarcodeSetting)
	ON_BN_CLICKED(IDC_BUTTON_2D_BARCODE_VALIDATE, &CSetupDlg::OnBnClickedButton2dBarcodeValidate)
	ON_BN_CLICKED(IDC_BUTTON_QA_SETTING_SETUP, &CSetupDlg::OnBnClickedButtonQaSettingSetup)
END_MESSAGE_MAP()


// CSetupDlg 메시지 처리기입니다.


BOOL CSetupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnBtnGUI();
	UpdateTrayFileName();

	m_nLogflag =FALSE;
	OnBtnAutoSkipDisplay();

	CompanySetting();

    // 반투명 윈도우
//     typedef BOOL(WINAPI *SLWA)(HWND,COLORREF,BYTE,DWORD);   
//     SLWA pSetLayeredWindowAttributes = NULL;
//     HINSTANCE hmodUSER32 = LoadLibrary("USER32.DLL");
//     pSetLayeredWindowAttributes = (SLWA)GetProcAddress(hmodUSER32, "SetLayeredWindowAttributes");
//     HWND hwnd = this->m_hWnd;
// 
//     SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
//     pSetLayeredWindowAttributes(hwnd, 0, (255*100)/100, LWA_ALPHA);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSetupDlg::UpdateTrayFileName()
{
	char szTrayFileName[MAX_PATH] = {0,};	
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName,szTrayFileName,sizeof(szTrayFileName));
	char szSltPgmName[MAX_PATH] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_SLT_ProgramName, szSltPgmName, sizeof(szSltPgmName));
	char szCategoryName[MAX_PATH] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_CategoryName, szCategoryName, sizeof(szCategoryName));

	CString strTotalName;
	strTotalName.Format("%s / %s / %s", szTrayFileName, szSltPgmName, szCategoryName);
	GetDlgItem(IDC_EDIT_TRAY_FILE)->SetWindowText((LPCTSTR)strTotalName);
}

void CSetupDlg::OnBtnGUI()
{
	m_BtnOffset.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnOffset.SetIcon(IDI_BTN_OFFSET);
	m_BtnOffset.SetAlign(CButtonST::ST_ALIGN_VERT);
	
	m_BtnSpeedAll.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSpeedAll.SetIcon(IDI_BTN_SPEED4);
	m_BtnSpeedAll.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSpeed_TrayPP.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSpeed_TrayPP.SetIcon(IDI_BTN_SPEED);
	m_BtnSpeed_TrayPP.SetAlign(CButtonST::ST_ALIGN_VERT);	

	m_BtnSpeed_TestPP.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSpeed_TestPP.SetIcon(IDI_BTN_SPEED2);
	m_BtnSpeed_TestPP.SetAlign(CButtonST::ST_ALIGN_VERT);	

	m_BtnSpeed_LoadTbl.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSpeed_LoadTbl.SetIcon(IDI_BTN_SPEED3);
	m_BtnSpeed_LoadTbl.SetAlign(CButtonST::ST_ALIGN_VERT);	

	m_BtnSpeed_PressUnit.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSpeed_PressUnit.SetIcon(IDI_BTN_SPEED4);
	m_BtnSpeed_PressUnit.SetAlign(CButtonST::ST_ALIGN_VERT);	

	m_BtnTransfer.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnTransfer.SetIcon(IDI_BTN_SPEED2);
	m_BtnTransfer.SetAlign(CButtonST::ST_ALIGN_VERT);	

	m_BtnSiteSetting.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSiteSetting.SetIcon(IDI_BTN_SITEMAP);
	m_BtnSiteSetting.SetAlign(CButtonST::ST_ALIGN_VERT);	

	m_BtnCategorySetup.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCategorySetup.SetIcon(IDI_BTN_CATE_SETUP);
	m_BtnCategorySetup.SetAlign(CButtonST::ST_ALIGN_VERT);	

	m_BtnIFSetting.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnIFSetting.SetIcon(IDI_BTN_IF_SETTING);
	m_BtnIFSetting.SetAlign(CButtonST::ST_ALIGN_VERT);	

	m_BtnDvcSetup.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnDvcSetup.SetIcon(IDI_BTN_DVC_SETUP);
	m_BtnDvcSetup.SetAlign(CButtonST::ST_ALIGN_VERT);	

	m_BtnSLTPgm.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSLTPgm.SetIcon(IDI_BTN_SLT_PGM);
	m_BtnSLTPgm.SetAlign(CButtonST::ST_ALIGN_VERT);	

	m_BtnYieldCtrl.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnYieldCtrl.SetIcon(IDI_BTN_CONTACT);
	m_BtnYieldCtrl.SetAlign(CButtonST::ST_ALIGN_VERT);	

	m_BtnYieldMonitor.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnYieldMonitor.SetIcon(IDI_BTN_YIELD);
	m_BtnYieldMonitor.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnIOMonitor.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnIOMonitor.SetIcon(/*IDI_BTN_IO_MONITOR*/IDI_ICON_DIO);
	m_BtnIOMonitor.SetAlign(CButtonST::ST_ALIGN_VERT);	

	m_BtnPosCheck.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnPosCheck.SetIcon(IDI_BTN_POSITION);
	m_BtnPosCheck.SetAlign(CButtonST::ST_ALIGN_VERT);	

	m_BtnEventLog.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnEventLog.SetIcon(IDI_MAIN_EVENTLOG);
	m_BtnEventLog.SetAlign(CButtonST::ST_ALIGN_VERT);	

	m_BtnChangeFile.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnChangeFile.SetIcon(IDI_BTN_FILE_CHANGE);
	m_BtnChangeFile.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnVibrator.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnVibrator.SetIcon(IDI_BTN_VIBRATOR);
	m_BtnVibrator.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnVision.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnVision.SetIcon(IDI_BTN_ONLINE);
	m_BtnVision.SetAlign(CButtonST::ST_ALIGN_VERT);	

    m_BtnSocketClean.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
    m_BtnSocketClean.SetIcon(IDI_BTN_CLEANING);
    m_BtnSocketClean.SetAlign(CButtonST::ST_ALIGN_VERT);    

	
	m_BtnTower.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnTower.SetIcon(IDI_ICON_TOWER);
	m_BtnTower.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnDataAssign.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnDataAssign.SetIcon(IDI_BTN_DATAASSIGN);
	m_BtnDataAssign.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_CtrlAutoSkipCnt.SetBackColor(COLOR_WWHITE);
	m_CtrlAutoSkipCnt.SetTextSize(30);

	m_BtnSocketCount.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnSocketCount.SetIcon(IDI_BTN_SOKECT_COUNT);
	m_BtnSocketCount.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnDailyReport.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnDailyReport.SetIcon(IDI_BTN_DAILY);
	m_BtnDailyReport.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnGrrSetting.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnGrrSetting.SetIcon(IDI_BTN_GRR_SETTING);
	m_BtnGrrSetting.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnLoopSetting.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnLoopSetting.SetIcon(IDI_BTN_LOOP);
	m_BtnLoopSetting.SetAlign(CButtonST::ST_ALIGN_VERT);
	
	m_BtnSpeed_Feeder.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnSpeed_Feeder.SetIcon(IDI_BTN_SPEED);
	m_BtnSpeed_Feeder.SetAlign(CButtonST::ST_ALIGN_VERT);
		
	m_BtnBarcodeSetting.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnBarcodeSetting.SetIcon(IDI_ICON_2D_BARCODE);
	m_BtnBarcodeSetting.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_Btn2dBarcodeValidate.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_Btn2dBarcodeValidate.SetIcon(IDI_ICON_2D_BARCODE_VALIDATE);
	m_Btn2dBarcodeValidate.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnQASetting.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnQASetting.SetAlign(CButtonST::ST_ALIGN_VERT);
	
}

void CSetupDlg::OnBnClickedButtonOffset()
{
	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_EQP_RUN_STATE);
	if(bLotState==TRUE){
		return;
	}
	
	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[Offset Test Pp Dialog Show]");

		CUserOffSetTestPp_MP DO;
		DO.DoModal();
	}
}

void CSetupDlg::OnBnClickedButtonSpeed()
{
	if( CheckEQPRunning() == TRUE )
		return;

	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[Tray PP Speed & Time Dialog Show]");

		CSPD_TM_TRAY_PP DO;
		DO.DoModal();
	}
}

void CSetupDlg::OnBnClickedButtonSiteSet()
{
	if( CheckEQPRunning() == TRUE )
		return;


	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[Site Setting Dialog Show]");
		CSiteSetDlg_128Para DO;
		DO.DoModal();
	}
}

void CSetupDlg::OnBnClickedButtonCateSetup()
{
	if( CheckEQPRunning() == TRUE )
		return;

	
	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[Category Setup Dialog Show]");
		CCategorySetupDlg DO;
		DO.DoModal();
	}
	
}

void CSetupDlg::OnBnClickedButtonIfTest()
{
	if( CheckEQPRunning() == TRUE )
		return;

	MakeLog("[Interface Testing Dialog Show]");

    g_McComm.OnSendDlgShow(eDlgProcCmd_TesterIfTest);
}

void CSetupDlg::OnBnClickedButtonDeviceSetup()
{
	if( CheckEQPRunning() == TRUE )
		return;

	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[Device Setup Dialog Show]");

		CDeviceSetup DO;
		DO.DoModal();
	}

	UpdateTrayFileName();	// Tray File을 update해주어야 한다.
}

void CSetupDlg::OnBnClickedButtonSltPgm()
{
	if( CheckEQPRunning() == TRUE )
		return;

	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[SLT Program Dialog Show]");

		CSltPgmSetDlg DO;
		DO.DoModal();
	}
}

void CSetupDlg::OnBnClickedButtonYieldCtrl()
{
	if( CheckEQPRunning() == TRUE )
		return;

	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[Contact Control Dialog Show]");

		CYieldSetDlg DO;
		DO.DoModal();
	}
}

void CSetupDlg::OnBnClickedButtonIoMonitor()
{
	if( CheckEQPRunning() == TRUE )
		return;

	MakeLog("[Dio Monitor Dialog Show]");

	g_McComm.OnSendDlgShow(eDlgProcCmd_IoMonitorDIO);
}

void CSetupDlg::OnBnClickedButtonPosCheck()
{
	if( CheckEQPRunning() == TRUE )
		return;

	g_McComm.OnSendDlgShow(eDlgProcCmd_Position);

	MakeLog("[Manual All Dialog Show]");
}

void CSetupDlg::OnBnClickedButtonEventLog()
{
	MakeLog("[Event Log Dialog Show]");

	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		CEventLogDlg DO;
		DO.DoModal();
	}
}

void CSetupDlg::OnBnClickedButtonSpeedShuttle()
{
	if( CheckEQPRunning() == TRUE )
		return;

	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[Loading Table Speed & Time Dialog Show]");

		CSPD_TM_SHUTTLE DO;
		DO.DoModal();
	}
}

void CSetupDlg::OnBnClickedButtonSpeedContactor()
{
	if( CheckEQPRunning() == TRUE )
		return;

	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[PressUnit Speed & Time Dialog Show]");

		CSPD_TM_CONTACTOR DO;
		DO.DoModal();
	}
}

void CSetupDlg::OnBnClickedButtonSpeedTestPp()
{
	if( CheckEQPRunning() == TRUE )
		return;

	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[Test PP Speed & Time Dialog Show]");

		CSPD_TM_TEST_PP DO;
		DO.DoModal();
	}
}

void CSetupDlg::OnBnClickedButtonSpeedAll()
{
	if( CheckEQPRunning() == TRUE )
		return;
	
	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[ALL Speed Dialog Show]");
		CSPD_MotorAll DO;
		DO.DoModal();
	}
}


void CSetupDlg::OnBnClickedButtonSpeedTansfer()
{
	if( CheckEQPRunning() == TRUE )
		return;

	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[Transfer Speed & Time Dialog Show]");

		CSPD_TM_Transfer DO;
		DO.DoModal();
	}
}


void CSetupDlg::OnBnClickedButtonTraySetup()
{
	if( CheckEQPRunning() == TRUE )
		return;

	MakeLog("[Tray Setup Dialog Show]");

	CTrayFileSetupDlg DO;
	DO.DoModal();
}


BOOL CSetupDlg::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_ESCAPE ){
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CSetupDlg::OnBnClickedButtonYieldMonitor()
{
	if( CheckEQPRunning() == TRUE )
		return;

	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[Yield Monitoring Dialog Show]");

		CYieldMonitorDlg DO;
		DO.DoModal();
	}
}


void CSetupDlg::OnBnClickedButtonCcdSetup()
{
	MakeLog("[Vision Dialog Show]");

	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		CCCDSetupDlg DO;
		DO.DoModal();
	}
}


void CSetupDlg::OnBnClickedButtonVibrator()
{
	if( CheckEQPRunning() == TRUE )
		return;

	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[Setplate Vibrator Dialog Show]");

		CVibratorDlg DO;
		DO.DoModal();
	}
}


void CSetupDlg::OnBnClickedButtonAioMonitor()
{
	if( CheckEQPRunning() == TRUE )
		return;

	MakeLog("[Aio Monitor Dialog Show]");

    g_McComm.OnSendDlgShow(eDlgProcCmd_IoMonitorAIO);
}


void CSetupDlg::OnBnClickedButtonSocketClean()
{
	if( CheckEQPRunning() == TRUE )
		return;

	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[Socket Cleaning Dialog Show]");

		CSocketCleaning DO;
		DO.DoModal();
	}
}


void CSetupDlg::OnBnClickedButtonFtp()
{
	MakeLog("[FTP File Transfer Dialog Show]");


}


void CSetupDlg::OnBnClickedButtonTower()
{
	if( CheckEQPRunning() == TRUE )
		return;

	MakeLog("[Signal Tower Dialog Show]");

	g_McComm.OnSendDlgShow(eDlgProcCmd_SignalTower);
}


void CSetupDlg::OnBnClickedButtonDataAssign()
{
	if (CheckEQPRunning() == TRUE)
		return;

	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("Data Assign Dialog Show]");

		CDataAssignDlg DO;
		DO.DoModal();
	}
}

void CSetupDlg::MakeLog(LPCTSTR fmt, ...)
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

void CSetupDlg::OnBnClickedButtonOffsetTray()
{

}


void CSetupDlg::OnBnClickedButtonTrayAssign()
{
// 	if( CheckEQPRunning() == TRUE )
// 		return;
// 
// 	MakeLog("[Tray Assign Dialog Show]");
// 	
// 	CTrayAssign DO;
// 	DO.DoModal();
}


void CSetupDlg::OnBnClickedCheckAutoSkip2()
{
	UpdateData(FALSE);
	m_nLogflag = TRUE;

	m_bAutoSkip = !m_bAutoSkip;
	if(m_bAutoSkip){
		GetDlgItem(IDCANCEL)->SetFocus();
		//int nUserLevel = pSltView->m_nLogInLv;
		int nRetLevel  = 0;

		BOOL bRet = g_LibCommCtrl.ShowLogInBox(0, &nRetLevel);

		if(RETURN_OK == bRet && nRetLevel >= eUSERLEVEL_TECHNICIAN){
		}else{
			AfxMessageBox("User Level is low!!");
			return;
		}
	}
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_AutoSkipUse, m_bAutoSkip);
	OnBtnAutoSkipDisplay();

	char szCokName[512] = {0,};
	ST_BASIC_INFO   stBasicData;
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
	g_COK_FileMgr.LoadBasicInfo(szCokName, stBasicData);

	stBasicData.bAutoSkipMode = m_bAutoSkip;

	g_COK_FileMgr.SaveBasicInfo(szCokName, &stBasicData);
}


void CSetupDlg::OnEnSetfocusEditAutoSkipCnt2()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nAutoSkipCnt, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "AutoSkip Count", "30", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_AUTO_SKIP_CNT2)->SetWindowText(szRetVal);

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

void CSetupDlg::OnBtnAutoSkipDisplay()
{
	m_bAutoSkip = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoSkipUse);
	if (m_bAutoSkip) {
		m_BtnAutoSkip.SetShade(CShadeButtonST::SHS_METAL, 8, 20, 0, RGB(55, 55, 255));
		m_BtnAutoSkip.SetIcon(IDI_BTN_AUTOSKIP_ON);
		m_BtnAutoSkip.SetWindowText("Auto Skip On");
		// 		if(m_nLogflag == TRUE)
		// 		{
		// 			MakeLog("[MENU = Input Auto Skip ][BEFORE= Auto Skip Off, AFTER= Auto Skip On]");
		// 		}
		if ((m_bAutoSkip != m_bBeforeAutoSkip) && m_nLogflag == TRUE)
		{
			MakeLog("[MENU = Input Auto Skip ][BEFORE= Auto Skip Off, AFTER= Auto Skip On]");
		}
	}
	else {
		m_BtnAutoSkip.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
		m_BtnAutoSkip.SetIcon(IDI_BTN_AUTOSKIP_OFF);
		m_BtnAutoSkip.SetWindowText("Auto Skip Off");
		// 		if(m_nLogflag == TRUE)
		// 		{
		// 			MakeLog("[MENU = Input Auto Skip ][BEFORE= Auto Skip On, AFTER= Auto Skip Off]");
		// 		}
		if ((m_bAutoSkip != m_bBeforeAutoSkip) && m_nLogflag == TRUE)
		{
			MakeLog("[MENU = Input Auto Skip ][BEFORE= Auto Skip On, AFTER= Auto Skip Off]");
		}
	}

	m_bBeforeAutoSkip = m_bAutoSkip;

	m_BtnAutoSkip.SetAlign(CButtonST::ST_ALIGN_VERT);
	//m_nLogflag = TRUE;
}

void CSetupDlg::OnBnClickedButtonSocketCount()
{
	MakeLog("[Socket Count Dialog Show]");

	g_McComm.OnSendDlgShow(eDlgProcCmd_SocketContactCount);
}


void CSetupDlg::OnBnClickedButtonDailyReport()
{
	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		CDailyReport DO;
		DO.DoModal();
	}
}


void CSetupDlg::OnBnClickedButtonGrrSettingSetup()
{
	if (CheckEQPRunning() == TRUE)
		return;

	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[GrrSetting Dialog Show]");
		CAutoGrrSetDlg dlg;
		dlg.DoModal();
	}
}


void CSetupDlg::CompanySetting()
{
	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
		//GetDlgItem(IDC_BUTTON_CB_DATA_OPSTION)->ShowWindow(SW_SHOW);

	}
	else{
		GetDlgItem(IDC_BUTTON_CB_DATA_OPSTION)->ShowWindow(SW_HIDE);
	}

	

// 	if (nProjectOption == DEF_PROJECT_MAUI ){
// 		GetDlgItem(IDC_BUTTON_LOOP_SETTING_SETUP)->ShowWindow(SW_HIDE);
// 	}
// 	else {
// 		GetDlgItem(IDC_BUTTON_LOOP_SETTING_SETUP)->ShowWindow(SW_SHOW);
// 	}
	
// 	if (nProjectOption != DEF_PROJECT_MAUI && strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
// 		GetDlgItem(IDC_BUTTON_AUDIT_SETTING_SETUP)->ShowWindow(SW_SHOW);
// 	}
// 	else {
// 		GetDlgItem(IDC_BUTTON_AUDIT_SETTING_SETUP)->ShowWindow(SW_HIDE);
// 	}


	GetDlgItem(IDC_BUTTON_IONIZER)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_BUTTON_GRR_SETTING_SETUP)->ShowWindow(SW_SHOW);
	

	/*
	if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
		GetDlgItem(IDC_BUTTON_SOCKET_COUNT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_SOCKET_CLEAN)->ShowWindow(SW_SHOW);
		//GetDlgItem(IDC_BUTTON_GRR_SETTING_SETUP)->ShowWindow(SW_SHOW);
	}
	else if(strcmp(szCompany, DEF_COMPANY_USI) == 0){
		GetDlgItem(IDC_BUTTON_SOCKET_COUNT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_SOCKET_CLEAN)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_BUTTON_GRR_SETTING_SETUP)->ShowWindow(SW_HIDE);
	}
	GetDlgItem(IDC_BUTTON_GRR_SETTING_SETUP)->ShowWindow(SW_SHOW);*/
}

void CSetupDlg::OnBnClickedButtonCbDataOpstion()
{

}


void CSetupDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnOK();
}


void CSetupDlg::OnBnClickedButtonLoopSettingSetup()
{
	if (CheckEQPRunning() == TRUE)
		return;

	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[LoopSetting Dialog Show]");
		CAutoLoopSetDlg dlg;
		dlg.DoModal();
	}
}


void CSetupDlg::OnBnClickedButtonAuditSettingSetup()
{

}

LRESULT CSetupDlg::OnReceivedMsg(WPARAM wParam, LPARAM lParam)
{
	OnBtnAutoSkipDisplay();
	return 0L;
}

void CSetupDlg::OnBnClickedButtonIonizer()
{

}


void CSetupDlg::OnBnClickedButtonSpeedFeeder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (CheckEQPRunning() == TRUE)
		return;

	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[Feeder Speed & Time Dialog Show]");

		CSPD_TM_FEEDER DO;
		DO.DoModal();
	}
}


void CSetupDlg::OnBnClickedButtonBarcodeSetting()
{
	if (CheckEQPRunning() == TRUE)
		return;

	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		g_McComm.OnSendDlgShow(eDlgProcCmd_BarcodeSetting);

		MakeLog("[Barcode Setting Dialog Show]");
	}

}


void CSetupDlg::OnBnClickedButton2dBarcodeValidate()
{
	if (CheckEQPRunning() == TRUE)
		return;

	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[2d Barcode Validate Setting Dialog Show]");

		CBarcodeValidateSetupDlg DO;
		DO.DoModal();

	}
}


void CSetupDlg::OnBnClickedButtonQaSettingSetup()
{
	if (IsLotOnGoing() == TRUE)
		return;

	if (g_DMCont.m_dmEQP.GB(BDM0_OPEN_SLT_UI_DLG_HISTORY) == FALSE)
	{
		MakeLog("[QA Mode Set Dialog Show]");

		CAutoQASetDlg dlg;
		dlg.DoModal();
	}
}


BOOL CSetupDlg::IsLotOnGoing()
{
	BOOL bRet = FALSE;
	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
	if (bLotState == eHandlerLotMode_OnGoing && nRunMode == eRUN_MODE_ONLINE) {
		AfxMessageBox("Handler is Running!! Please save after lot end");
		bRet = TRUE;
	}
	return bRet;
}