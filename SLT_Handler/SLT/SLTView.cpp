
// SLTView.cpp : CSLTView 클래스의 구현
//

#include "stdafx.h"
#include "MainFrm.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터c 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "SLT.h"
#endif

#include "SLTDoc.h"
#include "SLTView.h"

#include "MCComm.h"
#include "..\Dll_Src\LIB_CommCtrl\LIB_CommCtrl\LogInUserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



#define DLG_MOVE_POS_START 128


#define PGM_VERSION_AMKOR  _T("KR1.1.13A_251208")




// CSLTView

//#define LOG_OUT_TIME (180) //초
#define LOG_OUT_TIME_HALF_DAY (720) //분

IMPLEMENT_DYNCREATE(CSLTView, CFormView)

BEGIN_MESSAGE_MAP(CSLTView, CFormView)
	ON_BN_CLICKED(IDC_BTN_LV_USER, &CSLTView::OnBnClickedBtnLvUser)
	ON_BN_CLICKED(IDC_BTN_LV_SETUP, &CSLTView::OnBnClickedBtnLvSetup)
	ON_BN_CLICKED(IDC_BTN_LV_ENGR, &CSLTView::OnBnClickedBtnLvEngr)
	ON_BN_CLICKED(IDC_BTN_LV_MAIN, &CSLTView::OnBnClickedBtnLvMain)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CSLTView::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CSLTView::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_BTN_LV_SUPER, &CSLTView::OnBnClickedBtnLvSuper)
	ON_WM_DESTROY()
	ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BTN_ONECYCLE, &CSLTView::OnBnClickedBtnOnecycle)
    ON_BN_CLICKED(IDC_BTN_STOP, &CSLTView::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CSLTView::OnBnClickedBtnCancel)
	ON_STN_DBLCLK(IDC_STATIC_LOGO, &CSLTView::OnStnDblclickStaticLogo)
	ON_BN_CLICKED(IDC_BTN_CAP, &CSLTView::OnBnClickedBtnCap)
	ON_BN_CLICKED(IDC_BTN_LOOP_CANCEL, &CSLTView::OnBnClickedBtnLoopCancel)
	ON_BN_CLICKED(IDC_BTN_AUDIT_CANCEL, &CSLTView::OnBnClickedBtnAuditCancel)
	ON_BN_CLICKED(IDC_BTN_MULTI_ORIGIN, &CSLTView::OnBnClickedBtnMultiOrigin)
	ON_STN_DBLCLK(IDC_LOAD_SUPPLY, &CSLTView::OnStnDblclickLoadSupply)

END_MESSAGE_MAP()

// CSLTView 생성/소멸



CSLTView::CSLTView()
	: CFormView(CSLTView::IDD)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_pDlgUser  = NULL;
	m_pDlgEngr  = NULL;
	m_pDlgSetup = NULL;
	m_pDlgMain  = NULL;
	m_pDlgSuper = NULL;
	m_nLogInLv	= USERLEVEL_OPERATOR;
	m_nLoginTime = 0;
	m_pMainDlg = NULL;
}

CSLTView::~CSLTView()
{
	DestroyDialog();

	TWLOG.Stop();
	TWLOG.releaseInstance();
}

void CSLTView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_LV_USER, m_BtnUser);
	DDX_Control(pDX, IDC_BTN_LV_SETUP, m_BtnSetup);
	DDX_Control(pDX, IDC_BTN_LV_ENGR, m_BtnEngr);
	DDX_Control(pDX, IDC_BTN_LV_MAIN, m_BtnMain);
	DDX_Control(pDX, IDC_BTN_EXIT, m_BtnExit);
	DDX_Control(pDX, IDC_BTN_LOGIN, m_BtnLogIn);
	DDX_Control(pDX, IDC_BTN_LV_SUPER, m_BtnSuper);
	DDX_Control(pDX, IDC_BTN_ONECYCLE, m_BtnOneCycle);
	DDX_Control(pDX, IDC_BTN_STOP, m_BtnStop);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_BTN_LOOP_CANCEL, m_BtnLoopCancel);
	DDX_Control(pDX, IDC_BTN_AUDIT_CANCEL, m_BtnAuditCancel);
	DDX_Control(pDX, IDC_CUR_PRESSURE1,m_StaticCurPressure1);
	DDX_Control(pDX, IDC_CUR_PRESSURE2, m_StaticCurPressure2);
	DDX_Control(pDX, IDC_CB_CHECK_ONOFF, m_StaticCBCheckOnOff);
	DDX_Control(pDX, IDC_CHECK_SLT_MC_CONNECT, m_StaticSLTMC_Connect);
	DDX_Control(pDX, IDC_BTN_MULTI_ORIGIN, m_BtnMultiOrigin);
	DDX_Control(pDX, IDC_LOAD_SUPPLY, m_StaticLoadSupply);
	DDX_Control(pDX, IDC_BTN_CAP, m_BtnCapture);
	DDX_Control(pDX, IDC_CHECK_MOTOR_CONNECT, m_StaticMotor_Connect);
	
}

BOOL CSLTView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CSLTView::Init_SLT_SetupFile()
{
	char szTemp[512]={0,};
	::GetCurrentDirectory(sizeof(szTemp), szTemp);

	char szFilePath[MAX_PATH] = {0,};
	char szLastPackageName[256] = {0,};
	sprintf_s(szFilePath, sizeof( szFilePath ), "%s\\sys_data\\%s",szTemp, SZ_SLT_SYS_FILE_NAME);
	CConfigData CConfig(szFilePath);
	CConfig.GetString("Last Package used", "Name", "Test", szLastPackageName);

	CSystemData cSysData;
	ST_SLT_SYSTEM lpstData;
	cSysData.Initialize(szLastPackageName);

	g_COK_FileMgr.Initialize(szTemp);

	cSysData.UpdateFileDataToSM();
	
	MakeLog("[SLT Software Start]");
}

void CSLTView::Init_GlobalObject()
{
	char szDriverFileName[MAX_PATH]={0,};
	char szDBUpdateFile[MAX_PATH] = { 0, };
	char szTemp[512]={0,};
	::GetCurrentDirectory(sizeof(szTemp), szTemp);
	sprintf_s(szDriverFileName, sizeof(szDriverFileName), "%s\\sys_data\\%s", szTemp, SZ_DRIVER_FILE_NAME);

	g_DMCont.Create();
	g_DvcDataMem.CreateMapData(_T("SLT_DEVICE_DATA_MEM"));

	// 0 : SYSTEM DB  1: LOG DB
	g_DB.Create(szDriverFileName, strlen(szDriverFileName), eDB_TYPE_SYSTEM);
	g_DB_LOG.Create(szDriverFileName, strlen(szDriverFileName), eDB_TYPE_LOG);	
	g_DB_Teach.Create(szDriverFileName, strlen(szDriverFileName), eDB_TYPE_TEACH);
	g_DB_LotHistory.Create(szDriverFileName, strlen(szDriverFileName), eDB_TYPE_LOTHISTORY);
	//Log
	TWLOG.CreateLog(eLogID_System_GUI, _T("System_GUI"));

	TWLOG.SetRootPath(eLogID_System_GUI, _T("D:\\Techwing\\HandlerLog"));
	TWLOG.SetStoreType(eLogID_System_GUI, LogFileType::eDay);
	TWLOG.SetPathType(eLogID_System_GUI, LogPathType::DAY);
	TWLOG.SetLogItem(eLogID_System_GUI, LogItem::Time|LogItem::NewLine|LogItem::Date);

	TWLOG.CreateLog(eLogID_Teaching_Offset, _T("Teaching_Offset"));
	TWLOG.SetRootPath(eLogID_Teaching_Offset, _T("D:\\Techwing\\HandlerLog"));
	TWLOG.SetStoreType(eLogID_Teaching_Offset, LogFileType::eDay);
	TWLOG.SetPathType(eLogID_Teaching_Offset, LogPathType::DAY);
	TWLOG.SetLogItem(eLogID_Teaching_Offset, LogItem::Time | LogItem::NewLine | LogItem::Date);



	TWLOG.Run();



	char szFolderName[MAX_PATH]={0,};
	// Tray Files folder check
	sprintf_s(szFolderName, sizeof(szFolderName), "%s\\sys_data\\HandlerRecipe", szTemp);
	if (GetFileAttributes(szFolderName) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(szFolderName,NULL);
	}
	// Tray Files folder check
	sprintf_s(szFolderName, sizeof(szFolderName), "%s\\sys_data\\HandlerRecipe\\%s", szTemp, SZ_TRAY_FOLDER);
	if (GetFileAttributes(szFolderName) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(szFolderName,NULL);
	}

	// SLT_Program Files folder check
	sprintf_s(szFolderName, sizeof(szFolderName), "%s\\sys_data\\HandlerRecipe\\%s", szTemp, SZ_SLT_PGM_FOLDER);
	if (GetFileAttributes(szFolderName) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(szFolderName,NULL);
	}

	// Category Files folder check
	sprintf_s(szFolderName, sizeof(szFolderName), "%s\\sys_data\\HandlerRecipe\\%s", szTemp, SZ_CATEGORY_FOLDER);
	if (GetFileAttributes(szFolderName) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(szFolderName,NULL);
	}
	sprintf_s(szFolderName, sizeof(szFolderName), "%s", SZ_CAPUTRE_PATH);
	if (GetFileAttributes(szFolderName) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(szFolderName, NULL);
	}

	//2D Mark folder check
	char szCokName[512] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
	sprintf_s(szFolderName, sizeof(szFolderName), "%s\\sys_data\\HandlerRecipe\\%s\\%s\\%s", szTemp, SZ_TRAY_FOLDER, szCokName, SZ_2D_MARK_FOLDER);
	if (GetFileAttributes(szFolderName) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(szFolderName, NULL);
	}


	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, m_szCompany, sizeof(m_szCompany));



	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	
	GetDlgItem(IDC_TEST_HAND_MOVE_SENSOR)->ShowWindow(SW_HIDE);
	
	//OS Version 확인.

	BOOL bRecentVersion = ChkOSVerion();
	if (bRecentVersion == TRUE) {
		g_DMCont.m_dmHandlerOpt.SB(BDM11_OS_RECENT_VER, true);
	}
	else {
		//Alarm
		g_DMCont.m_dmHandlerOpt.SB(BDM11_OS_RECENT_VER, false);
	}

	g_DMCont.m_dmEQP.SN(NDM0_Handler_Login_Level, eUSERLEVEL_OPERATOR);

}

void CSLTView::Close_GlobalObject()
{
	char szData[MAX_PATH] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szData, sizeof(szData));

	BOOL bOption = g_DMCont.m_dmEQP.GB(BDM0_MD_DOTMATRIX);
	if (bOption == eOPTION_USE)
	{
		g_DotMatrix.ComClose();
	}

	g_McComm.Finalize();

	g_COK_FileMgr.Finalize((char*)szData);

	g_DB.Destroy();
	g_DB_LOG.Destroy();
	g_DB_Teach.Destroy();
	g_DB_LotHistory.Destroy();
	g_DMCont.Destroy();
	g_DvcDataMem.DestroyMapData();	

}

void CSLTView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	int i=0;
	char szDriverFileName[MAX_PATH]={0,};
	char szTemp[512]={0,};
	::GetCurrentDirectory(sizeof(szTemp), szTemp);
	sprintf_s(szDriverFileName, sizeof(szDriverFileName), "%s\\sys_data\\%s", szTemp, SZ_DRIVER_FILE_NAME);

	Init_GlobalObject();

	Init_SLT_SetupFile();

	OnBtnGUI();

	g_McComm.Initialize(this, szDriverFileName);

   
    char szEqpName[MAX_PATH] = {0,};
    char szEqpVersion[MAX_PATH] = {0,};
    CConfigData cModel(szDriverFileName);
    cModel.GetString("Model", "EQP ID", "TW-SL421", szEqpName);
    CString strEqpId = _T("");
    strEqpId.Format("TW-SL421(%s)", szEqpName);
    GetDlgItem(IDC_STATIC_EQP_NAME)->SetWindowText(strEqpId);
	g_DMCont.m_dmEQP.SS(SDM0_HANDER_ID, szEqpName, sizeof(szEqpName));
   // cModel.GetString("Model", "EQP VERSION", "KR 1.0.0", szEqpName);

	//  [12/28/2020 jaehoon.jung] project Name 추가.
	LoadProjectName();
	//

    CRect		rect;
    SetWindowPos(&wndNoTopMost, 0,0, rect.Width(), rect.Height(), SWP_SHOWWINDOW);

    // LOG File Name
    CConfigData ConfigData(szDriverFileName);
    char szLogFileName[ 512 ] = { 0, };
    char szBuff[512] = {0, };

    ConfigData.GetString( "SYSTEM LOG CONIFG", "Log File Name", "System.log", szBuff);
    sprintf_s(szLogFileName, sizeof(szLogFileName), "%s%s", SZ_LOG_FILE_PATH, szBuff);
    m_Logger.SetFileName(szLogFileName);

	m_pMainDlg = new CMainLowerDlg(this);
	m_pDlgMain = m_pMainDlg;
	m_pDlgMain->Create(IDD_SLT_MAIN_LOWER);
	m_pDlgMain->ShowWindow(SW_SHOW);
	CRect cRectMaximize;
	m_pDlgMain->GetWindowRect(&cRectMaximize); 
	m_pDlgMain->MoveWindow(0,DLG_MOVE_POS_START,cRectMaximize.Width(),cRectMaximize.Height(),TRUE);

	m_pDlgUser = new CUserOper;
	m_pDlgUser->Create(IDD_MAIN_DLG_USER);
	m_pDlgUser->ShowWindow(SW_HIDE);
	cRectMaximize;
	m_pDlgUser->GetWindowRect(&cRectMaximize); 
	m_pDlgUser->MoveWindow(0,DLG_MOVE_POS_START,cRectMaximize.Width(),cRectMaximize.Height(),TRUE);

	m_pDlgSetup = new CSetupDlg;
	m_pDlgSetup->Create(IDD_MAIN_DLG_SETUP);
	m_pDlgSetup->ShowWindow(SW_HIDE);
	cRectMaximize;
	m_pDlgSetup->GetWindowRect(&cRectMaximize); 
	m_pDlgSetup->MoveWindow(0,DLG_MOVE_POS_START,cRectMaximize.Width(),cRectMaximize.Height(),TRUE);

	m_pDlgEngr = new CEngrDlg;
	m_pDlgEngr->Create(IDD_MAIN_DLG_ENGR);
	m_pDlgEngr->ShowWindow(SW_HIDE);
	cRectMaximize;
	m_pDlgEngr->GetWindowRect(&cRectMaximize); 
	m_pDlgEngr->MoveWindow(0,DLG_MOVE_POS_START,cRectMaximize.Width(),cRectMaximize.Height(),TRUE);

	m_pDlgSuper = new CSupervisorDlg;
	m_pDlgSuper->Create(IDD_MAIN_DLG_SUPER);
	m_pDlgSuper->ShowWindow(SW_HIDE);
	cRectMaximize;
	m_pDlgSuper->GetWindowRect(&cRectMaximize); 
	m_pDlgSuper->MoveWindow(0,DLG_MOVE_POS_START,cRectMaximize.Width(),cRectMaximize.Height(),TRUE);

	CreateViewDialog(SW_MAIN_WINDOW);
	UserLevelDisplayMainUpdate();

	int nSimul = ConfigData.GetInt("DIO CONFIG", "Driver Type", "0");

	if (nSimul != 0)
	{
		int nDotMatrix = cModel.GetInt("MODE", "DotMatrix", "0");
		g_DMCont.m_dmEQP.SB(BDM0_MD_DOTMATRIX, (BOOL)nDotMatrix);
		if (nDotMatrix == eOPTION_USE)
		{
			g_DotMatrix.Initialize(szDriverFileName);
			g_DotMatrix.ComOpen();

			SetDotMatrixUpdate();
		}
	}

	m_nStaticCurPressure[0] = IDC_CUR_PRESSURE1;
	m_nStaticCurPressure[1] = IDC_CUR_PRESSURE2;
	m_nStaticCBCheckOnOff = IDC_CB_CHECK_ONOFF;
	m_nStaticSLTMC_Connect = IDC_CHECK_SLT_MC_CONNECT;
	m_nStaticLoadSupply = IDC_LOAD_SUPPLY;
	m_nStaticPickerMode = IDC_CHECK_PICKER_MODE;
	m_nStaticTestHandMoveSensor = IDC_TEST_HAND_MOVE_SENSOR;
	m_nStaticMotor_Connect = IDC_CHECK_MOTOR_CONNECT;

	m_nStaticTestppStationFrontRearPickPlace = IDC_CHECK_TESTPP_STATION_FRONT_REAR_PICK_PLACE;

	SetTimer(0, 1000, NULL);
	SetTimer(1, 200, NULL);
}


// CSLTView 진단

#ifdef _DEBUG
void CSLTView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSLTView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSLTDoc* CSLTView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSLTDoc)));
	return (CSLTDoc*)m_pDocument;
}
#endif //_DEBUG


// CSLTView 메시지 처리기

void CSLTView::OnBtnGUI()
{
	m_BtnUser.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnUser.SetIcon(IDI_BTN_USER);
	m_BtnUser.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSetup.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSetup.SetIcon(IDI_BTN_SETUP);
	m_BtnSetup.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnEngr.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnEngr.SetIcon(IDI_BTN_ENGR);
	m_BtnEngr.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnMain.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnMain.SetIcon(IDI_BTN_MAIN);
	m_BtnMain.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnExit.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnExit.SetIcon(IDI_EXIT);
	m_BtnExit.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnLogIn.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnLogIn.SetIcon(IDI_BTN_OPER);
	m_BtnLogIn.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSuper.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSuper.SetIcon(IDI_BTN_SUPERVISOR);
	m_BtnSuper.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnOneCycle.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnOneCycle.SetIcon(IDI_BTN_ONECYCLE);
	m_BtnOneCycle.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnStop.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnStop.SetIcon(IDI_BTN_STOP);
	m_BtnStop.SetAlign(CButtonST::ST_ALIGN_VERT);
	
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnLoopCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnLoopCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnAuditCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnAuditCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnMultiOrigin.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnMultiOrigin.SetIcon(IDI_BTN_MOTOR);
	m_BtnMultiOrigin.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnCapture.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnCapture.SetAlign(CButtonST::ST_ALIGN_VERT);
		
	m_StaticCurPressure1.m_fnSetFont("Fixedsys");
	m_StaticCurPressure1.SetTextSize(11);
	m_StaticCurPressure1.SetWeight(FW_BOLD);
	m_StaticCurPressure1.SetBackColor(COLOR_NOMAL);

	m_StaticCurPressure2.m_fnSetFont("Fixedsys");
	m_StaticCurPressure2.SetTextSize(11);
	m_StaticCurPressure2.SetWeight(FW_BOLD);
	m_StaticCurPressure2.SetBackColor(COLOR_NOMAL);

	m_StaticCBCheckOnOff.m_fnSetFont("Fixedsys");
	m_StaticCBCheckOnOff.SetTextSize(11);
	m_StaticCBCheckOnOff.SetWeight(FW_BOLD);
	m_StaticCBCheckOnOff.SetBackColor(COLOR_NOMAL);

	m_StaticSLTMC_Connect.m_fnSetFont("Fixedsys");
	m_StaticSLTMC_Connect.SetTextSize(12);
	m_StaticSLTMC_Connect.SetWeight(FW_BOLD);
	m_StaticSLTMC_Connect.SetBackColor(COLOR_NOMAL);
	
	m_StaticLoadSupply.m_fnSetFont("Fixedsys");
	m_StaticLoadSupply.SetTextSize(12);
	m_StaticLoadSupply.SetWeight(FW_BOLD);
	m_StaticLoadSupply.SetBackColor(COLOR_NOMAL);

	m_StaticMotor_Connect.m_fnSetFont("Fixedsys");
	m_StaticMotor_Connect.SetTextSize(12);
	m_StaticMotor_Connect.SetWeight(FW_BOLD);
	m_StaticMotor_Connect.SetBackColor(COLOR_NOMAL);
}

void CSLTView::UserLevelDisplayMainUpdate()
{
	switch(m_nLogInLv)
	{
	case USERLEVEL_TECHNICIAN:
		{
			m_BtnSetup.ShowWindow(SW_SHOW);
			m_BtnEngr.ShowWindow(SW_HIDE);
			m_BtnSuper.ShowWindow(SW_HIDE);
			m_BtnLogIn.SetWindowText("Technician");
			m_BtnLogIn.SetIcon(IDI_BTN_SETUP);

		}break;
	case USERLEVEL_ENGINEER:
		{
			m_BtnSetup.ShowWindow(SW_SHOW);
			m_BtnEngr.ShowWindow(SW_SHOW);
			m_BtnSuper.ShowWindow(SW_HIDE);
			m_BtnLogIn.SetWindowText("Engineer");
			m_BtnLogIn.SetIcon(IDI_BTN_ENGR);
		}break;
	case USERLEVEL_SUPERVISOR:
		{
			m_BtnSetup.ShowWindow(SW_SHOW);
			m_BtnEngr.ShowWindow(SW_SHOW);
			m_BtnSuper.ShowWindow(SW_SHOW);
			m_BtnLogIn.SetWindowText("Supervisor");
			m_BtnLogIn.SetIcon(IDI_BTN_SUPERVISOR);
		}break;
	default:
		{
			m_BtnSetup.ShowWindow(SW_HIDE);
			m_BtnEngr.ShowWindow(SW_HIDE);
			m_BtnSuper.ShowWindow(SW_HIDE);
			m_BtnLogIn.SetWindowText("Operator");
			m_BtnLogIn.SetIcon(IDI_BTN_OPER);
		}break;
	}
}

/*=============================================================================
  AUTHOR   : 김선진
  NAME     : DestroyDialog
  PARAMS   : None
  RETURN   : None
  COMMENTS : Modeless dialog 가 해당 화면이 아닐 경우 destory 하도록 한다.
==============================================================================*/
void CSLTView::DestroyDialog()
{
	if(m_pDlgMain != NULL){
		m_pDlgMain->DestroyWindow();
		delete m_pDlgMain;
		m_pDlgMain  = NULL;
	}

	if(m_pDlgUser != NULL){
		m_pDlgUser->DestroyWindow();
		delete m_pDlgUser;
		m_pDlgUser  = NULL;
	}

	if(m_pDlgSetup != NULL){
		m_pDlgSetup->DestroyWindow();
		delete m_pDlgSetup;
		m_pDlgSetup = NULL;
	}

	if(m_pDlgEngr != NULL){
		m_pDlgEngr->DestroyWindow();
		delete m_pDlgEngr;
		m_pDlgEngr  = NULL;
	}

	if(m_pDlgSuper != NULL){
		m_pDlgSuper->DestroyWindow();
		delete m_pDlgSuper;
		m_pDlgSuper  = NULL;
	}
}

void CSLTView::OnBnClickedBtnLvUser()
{
	CreateViewDialog(SW_LEVEL_USER);

	MakeLog("[Oper Menu Click]");
}


void CSLTView::OnBnClickedBtnLvSetup()
{
	CreateViewDialog(SW_LEVEL_SETUP);

	m_pDlgSetup->SendMessage(UM_AUTO_SKIP_UPDATE);
	MakeLog("[Tech Menu Click]");
}


void CSLTView::OnBnClickedBtnLvEngr()
{
	CreateViewDialog(SW_LEVEL_ENGR);

	MakeLog("[Eng'r Menu Click]");
}


void CSLTView::OnBnClickedBtnLvMain()
{
	CreateViewDialog(SW_MAIN_WINDOW);

	MakeLog("[Main Menu Click]");
}


void CSLTView::OnBnClickedBtnExit()
{
	char szProcName[ 512 ] = { 0, };
#ifdef _DEBUG
        sprintf_s(szProcName, sizeof(szProcName), "SLT_MC_D.exe");
#else
        sprintf_s(szProcName, sizeof(szProcName), "SLT_MC.exe");
#endif
	HWND hWnd = ::FindWindow( NULL, szProcName );

	BOOL bLotState = g_DMCont.m_dmEQP.GB( BDM0_EQP_RUN_STATE );
	if( bLotState == TRUE && hWnd ) {
		AfxMessageBox( _T( "Not available while running" ) );
		return;
	}

    int nRet = AfxMessageBox("Do you want Exit Software??", MB_YESNO|MB_TOPMOST );
    m_Logger.LOG("Do you want Exit Software??");

    if( nRet != IDYES ){
        return;
    }

	//Socket On/Off 저장.
	int nTestSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nTestSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);

	char szCokName[512] = { 0, };
	ST_BASIC_INFO   stBasicData;
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));

	g_COK_FileMgr.LoadBasicInfo(szCokName, stBasicData);

	for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++)
	{
		for (int nCol = 0; nCol < nTestSiteDivY; nCol++)
		{
			for (int nRow = 0; nRow < nTestSiteDivX; nRow++)
			{
				int nNum = 0;
				if (nTestSiteDivY == STATION_MAX_Y_SITE)
				{
					if (nBoardType == eBoardDir_Forward) {
						if (nStation <= ePressUnit_Down_4)
							nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(nRow - 3) + nCol * 4;
						else
							nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nRow + abs(nCol - 1) * 4;
					}
					else {
						if (nStation <= ePressUnit_Down_4)
							nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nRow + abs(nCol - 1) * 4;
						else
							nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(nRow - 3) + nCol * 4;
					}
				}
				else if (nTestSiteDivY == STATION_MAX_Y_SITE / 2)
				{
					if (nBoardType == eBoardDir_Forward) {
						if (nStation <= ePressUnit_Down_4)
							nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(nRow - 3) + nCol * 4;
						else
							nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + (nRow + (nCol * 4));
					}
					else {
						if (nStation <= ePressUnit_Down_4)
							nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + (nRow + (nCol * 4));
						else
							nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(nRow - 3) + nCol * 4;
					}
				}
				
				int nSiteOnOff = g_DMCont.m_dmHandlerOpt.GN(/*NDM11_SocketOnOff_Site1_1*/nNum);
				stBasicData.nSiteOnOff[nStation][nRow][nCol] = nSiteOnOff;
			}
		}
	}

	g_COK_FileMgr.SaveBasicInfo(szCokName, &stBasicData);


    m_Logger.LOG("SLT MC Close.");
	MakeLog("[SLT Software Exit]");
    
    if( hWnd )
    {
		m_Logger.LOG("[FIND] SLT MC Close.");
		MakeLog("[[FIND] SLT Software Exit]");
		g_McComm.CmdOnlyOneCmdNoParam(GUI_RCMD_REQ_FINALIZE_PGM);
        Sleep( 1000 );
    }

	CMainFrame* m_pMain = (CMainFrame*)AfxGetMainWnd();
	m_pMain->DestroyWindow();
}


void CSLTView::OnBnClickedBtnLogin()
{
    if(m_nLogInLv > eUSERLEVEL_OPERATOR)
    {
        m_nLogInLv = eUSERLEVEL_OPERATOR;
		g_DMCont.m_dmEQP.SN(NDM0_Handler_Login_Level, m_nLogInLv);
        UserLevelDisplayMainUpdate();
		CreateViewDialog(SW_MAIN_WINDOW);
		MakeLog("[ID: %s User LogOut ]",m_szLogInID);
    }
    else
    {
        int nUserLevel = m_nLogInLv;	
        int nRetLevel = 0;
		char szGetUserID[10] ={0};

        BOOL bRet = g_LibCommCtrl.ShowLogInBox2(nUserLevel, &nRetLevel, szGetUserID ,&m_nLoginTime);
		
		if (m_nLoginTime == 0) { m_nLoginTime = LOG_OUT_TIME_HALF_DAY;}
		memcpy(m_szLogInID, szGetUserID, sizeof(m_szLogInID));

        if(bRet == RETURN_CANCEL){
            AfxMessageBox("LogIn Fail!!");
        }
        else
        {
			m_tcLoginTime = (double)GetTickCount64();
            m_nLogInLv = nRetLevel;
			g_DMCont.m_dmEQP.SN(NDM0_Handler_Login_Level, m_nLogInLv);
			
            UserLevelDisplayMainUpdate();
			MakeLog("[ID: %s User LogIn ]",szGetUserID);
		}
    }	
}

void CSLTView::OnBnClickedBtnLvSuper()
{
	CreateViewDialog(SW_LEVEL_SUPER);

	MakeLog("[Super Menu Click]");
}


void CSLTView::OnDestroy()
{
	CFormView::OnDestroy();

	KillTimer(0);
	KillTimer(1);

	Close_GlobalObject();
	DestroyDialog();
}



BOOL CSLTView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if( pMsg->message == WM_KEYDOWN ){
		if( pMsg->wParam == VK_ESCAPE ){
			return TRUE;
		}
	}

	return CFormView::PreTranslateMessage(pMsg);
}

void CSLTView::Display_OneCycle_GrrCancel_Button() 
{
	int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN( NDM11_MD_FTRTQCContinue );
	switch( nLotStartMode )
	{
		case eStartMD_Initial:
		case eStartMD_InitContinue:
		case eStartMD_QA:
		case eStartMD_QA_Continue:
		case eStartMD_Retest:
		case eStartMD_RetestContinue:
				DisableAutoGRRCancel();
				DisableAutoLOOPCancel();
			break;
		case eStartMD_AutoGRR:
		case eStartMD_AutoGRR_Continue:
			{
				EnableAutoGRRCancel();
				DisableAutoLOOPCancel();
			}break;
		case eStartMD_AutoLoop:
		case eStartMD_AutoLoop_Continue:
			{
				EnableAutoLOOPCancel();
		}break;
	}
}

void CSLTView::OnTimer(UINT_PTR nIDEvent)
{

	////////////// Test Code

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CLogInUserDlg* pLoginUserDlg = (CLogInUserDlg*)pMainFrm->GetActiveView();
	int nUserLevel = pLoginUserDlg->m_nLoginTime;
	CString str;
	CStaticTrans *pStatic = NULL;
	/////////////

	switch(nIDEvent)
	{
	case 0:	//Watch time display
		{
			CTime cTmCurrent = CTime::GetCurrentTime();
			CString strTime = _T("");
			strTime.Format("%d-%d-%d %02d:%02d:%02d",cTmCurrent.GetYear() , cTmCurrent.GetMonth() , cTmCurrent.GetDay()
				                                    , cTmCurrent.GetHour(), cTmCurrent.GetMinute(), cTmCurrent.GetSecond());
			GetDlgItem(IDC_STATIC_TIME)->SetWindowText(strTime);
			int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
			Display_OneCycle_GrrCancel_Button();

			if (m_nLogInLv > eUSERLEVEL_OPERATOR) {
				double dSpanTime = (double)GetTickCount64() - m_tcLoginTime;
				if (dSpanTime > m_nLoginTime*1000.0 * 60.0) {
					m_nLogInLv = eUSERLEVEL_OPERATOR;
				
					UserLevelDisplayMainUpdate();
					char szGetUserID[10] = { 0 };
					memcpy(m_szLogInID, szGetUserID, sizeof(m_szLogInID));
				}
			}

			for (int i = 0; i < 2; i++) {
				double nCurPressureUnit[2] = {};
				pStatic = (CStaticTrans*)GetDlgItem(m_nStaticCurPressure[i]);
				nCurPressureUnit[i] = g_DMCont.m_dmHandlerOpt.GD(DDM11_CurPressure1+i);
				
				str.Format("Mdl%d Pressure = %0.3f", i + 1, nCurPressureUnit[i]);
				pStatic->SetWindowTextA(str);

				if (nCurPressureUnit[i] > 0)
					pStatic->SetBackColor(COLOR_GREEN);
				else
					pStatic->SetBackColor(COLOR_RED);
			}

			if (strcmp(m_szCompany, DEF_COMPANY_AMKOR) == 0)
			{
				pStatic = (CStaticTrans*)GetDlgItem(m_nStaticCBCheckOnOff);
				BOOL bCBCheckOnOff = g_DMCont.m_dmHandlerOpt.GB(BDM0_CB_DATA_CHECK_ONOFF);

				pStatic->SetWindowTextA(str);
				if (bCBCheckOnOff == DEF_ON) {
					pStatic->SetBackColor(COLOR_GREEN);
					str.Format("CB Check ON");
				}
				else {
					pStatic->SetBackColor(COLOR_RED);
					str.Format("CB Check OFF");
				}
				pStatic->SetWindowTextA(str);

// 				pStatic = (CStaticTrans*)GetDlgItem(m_nStaticBatchFileOn);
// 				BOOL bTesterBatchFile = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TesterBatchFile_Use);
// 				pStatic->SetWindowTextA(str);
// 				if (bTesterBatchFile == DEF_ON) {
// 					pStatic->SetBackColor(COLOR_GREEN);
// 					str.Format("TesterBatch ON");
// 				}
// 				else {
// 					pStatic->SetBackColor(COLOR_RED);
// 					str.Format("TesterBatch OFF");
// 				}
// 				pStatic->SetWindowTextA(str);

				
								
			}	

			char szProcName[32] = { 0, };
#ifdef _DEBUG
			sprintf_s(szProcName, sizeof(szProcName), "SLT_MC_D.exe");
#else
			sprintf_s(szProcName, sizeof(szProcName), "SLT_MC.exe");
#endif
			HWND hWnd = ::FindWindow(NULL, szProcName);

			pStatic = (CStaticTrans*)GetDlgItem(m_nStaticSLTMC_Connect);
			BOOL bMCConnect = g_McComm.IsConnected();

			pStatic->SetWindowTextA(str);
			if (bMCConnect == DEF_ON && hWnd) {
				pStatic->SetBackColor(COLOR_GREEN);
				str.Format("SLT_MC Connected");
			}
			else {
				pStatic->SetBackColor(COLOR_RED);
				str.Format("SLT_MC DisConnected");
			}
			pStatic->SetWindowTextA(str);

			str = _T("");
			pStatic = (CStaticTrans*)GetDlgItem(m_nStaticMotor_Connect);
			int nMotorConnect = g_DMCont.m_dmEQP.GN(NDM0_MAIN_POWER_CONTROLER_CONNETION_STATUS);
			if (bMCConnect == DEF_OFF)
				nMotorConnect = eSLTMC_Disconnect;

			switch (nMotorConnect)
			{
				case eCommIdle: 
				case eSLTMC_Disconnect:
				{
					pStatic->SetBackColor(COLOR_RED);
					str.Format("Motor DisConnected");
				}break;
				case eCommReconnectProcessing_WMX:
				case eCommReconnectProcessed_WMX:
				{
					pStatic->SetBackColor(COLOR_YELLOW);
					str.Format("Motor WMX Connecting");
				}break;
				case eCommReconnectProcessing_MotorPicker:
				case eCommReconnectProcessed_MotorPicker:
				{
					pStatic->SetBackColor(COLOR_YELLOW);
					str.Format("Motor Picker Connecting");
				}break;
				case eCommReconnectOK:
				{
					pStatic->SetBackColor(COLOR_GREEN);
					str.Format("Motor Connected");
				}break;
			}
	
			pStatic->SetWindowTextA(str);

			if (strcmp(m_szCompany, DEF_COMPANY_AMKOR) == 0)
			{
				pStatic = (CStaticTrans*)GetDlgItem(m_nStaticLoadSupply);
				int bLoadSupply = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOAD_SUPPLY);
				if (bLoadSupply == DEF_ON) {
					pStatic->SetBackColor(COLOR_GREEN);
				}
				else
					pStatic->SetBackColor(COLOR_RED);
							
				str.Format("Load Supply");
				pStatic->SetWindowTextA(str);
			}

			int bTestHand = (int)g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TEST_HAND_PNP_SITE_MOVE_ARRIVED_SEN);
			int bTestppStationFRPnP = (int)g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TESTPP_STATION_FR_PNP);
			
		}break;
		case 1:
		{
			int nOriginStatus = g_DMCont.m_dmHandlerOpt.GN(NDM11_MULTIORIGIN_HISTORY);
			if (nOriginStatus == eMulti_Origin_Start)
			{
				m_BtnStop.SetShade(CShadeButtonST::SHS_SOFTBUMP, 255, 255, 255, RGB(0, 0, 255));
				m_BtnMultiOrigin.SetShade(CShadeButtonST::SHS_SOFTBUMP, 255, 255, 255, RGB(255, 0, 0));
				Invalidate(FALSE);
			}		
			else if (nOriginStatus == eMulti_Origin_End)
			{
				m_BtnStop.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
				m_BtnMultiOrigin.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
				Invalidate(FALSE);
				g_DMCont.m_dmHandlerOpt.SN(NDM11_MULTIORIGIN_HISTORY, eMulti_Origin_None);
			}
			else
			{
				m_BtnStop.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
				m_BtnMultiOrigin.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
			}
				
			
		}break;

	}

	CFormView::OnTimer(nIDEvent);
}


void CSLTView::OnBnClickedBtnOnecycle()
{
	int nRet = AfxMessageBox("Do you want One Cycle??", MB_YESNO | MB_TOPMOST);
	m_Logger.LOG("Do you want One Cycle??");
	BOOL bCleanDeviceSkipOneCycle = g_DMCont.m_dmEQP.GB(BDM0_CLEAN_DEVICE_SKIP_SYS_ONE_CYCLE);

	if (nRet != IDYES) {
		return;
	}
	if (bCleanDeviceSkipOneCycle == TRUE) {
		AfxMessageBox(_T("Clean Device skipped. One cycle in progress and cannot be changed"));
		return;
	}

	int nOneCycle = g_DMCont.m_dmEQP.GB(BDM0_SYS_ONE_CYCLE);

	if(nOneCycle == FALSE)
		g_DMCont.m_dmEQP.SB(BDM0_SYS_ONE_CYCLE, TRUE);
	else 
	//if( g_DMCont.m_dmEQP.GB(BDM0_SYS_ONE_CYCLE) != TRUE )
		g_DMCont.m_dmEQP.SB( BDM0_SYS_ONE_CYCLE, FALSE );

	for (int i = 0; i < 8; i++) {
		g_DMCont.m_dmContac_Up.SN(NDM2_PRESS_1_PRE_CONTACT_STATUS + i, FALSE);
	}

	MakeLog("[ONE CYCLE Button Click]");
}


void CSLTView::OnBnClickedBtnStop()
{
    g_McComm.CmdProcessControl(eAutoLotCmd_Pause);

	//if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MULTIORIGIN_HISTORY))
	//{
		g_McComm.CmdMultiOrigin(eMultiOriginStop);
	//}
	Sleep(20);
	Invalidate(FALSE);
	MakeLog("[STOP Button Click]");
}
void CSLTView::OnBnClickedBtnCancel()
{
	int nRet = AfxMessageBox("Do you want GRR Cancel??", MB_YESNO | MB_TOPMOST);
	m_Logger.LOG("Do you want GRR Cancel ??");

	if (nRet != IDYES) {
		return;
	}

	g_DMCont.m_dmEQP.SB( BDM0_SYS_AUTO_GRR_CANCEL, TRUE );

	MakeLog("[AutoGRR Cancel Button Click]");
}

void CSLTView::EnableAutoGRRCancel()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_BTN_CANCEL);
	pBtn->ShowWindow(SW_SHOW);
	pBtn = (CButton*)GetDlgItem(IDC_BTN_ONECYCLE);
	pBtn->ShowWindow(SW_HIDE);
}

void CSLTView::DisableAutoGRRCancel()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_BTN_CANCEL);
	pBtn->ShowWindow(SW_HIDE);
	pBtn = (CButton*)GetDlgItem(IDC_BTN_ONECYCLE);
	pBtn->ShowWindow(SW_SHOW);
}
/*=============================================================================
  AUTHOR   : 문성림
  NAME     : 
  PARAMS   : nBtnClickWindow : Event 발생한 Show Window Level 상수
  RETURN   : void
  COMMENTS : nBtnClickWindow의 창을 생성한다
==============================================================================*/
void CSLTView::CreateViewDialog(int nBtnClickWindow)
{
	if (m_pDlgMain == NULL)
		return;

	switch(nBtnClickWindow)
	{
	case SW_MAIN_WINDOW:
		{
			m_pDlgMain->ShowWindow(SW_SHOW);
			m_pDlgUser->ShowWindow(SW_HIDE);
			m_pDlgSetup->ShowWindow(SW_HIDE);
			m_pDlgEngr->ShowWindow(SW_HIDE);
			m_pDlgSuper->ShowWindow(SW_HIDE);
		}break;
	case SW_LEVEL_USER:
		{
			m_pDlgMain->ShowWindow(SW_HIDE);
			m_pDlgUser->ShowWindow(SW_SHOW);
			m_pDlgSetup->ShowWindow(SW_HIDE);
			m_pDlgEngr->ShowWindow(SW_HIDE);
			m_pDlgSuper->ShowWindow(SW_HIDE);
		}break;
	case SW_LEVEL_SETUP:
		{
			m_pDlgMain->ShowWindow(SW_HIDE);
			m_pDlgUser->ShowWindow(SW_HIDE);
			m_pDlgSetup->ShowWindow(SW_SHOW);
			m_pDlgEngr->ShowWindow(SW_HIDE);
			m_pDlgSuper->ShowWindow(SW_HIDE);
		}break;
	case SW_LEVEL_ENGR:
		{
			m_pDlgMain->ShowWindow(SW_HIDE);
			m_pDlgUser->ShowWindow(SW_HIDE);
			m_pDlgSetup->ShowWindow(SW_HIDE);
			m_pDlgEngr->ShowWindow(SW_SHOW);
			m_pDlgSuper->ShowWindow(SW_HIDE);
		}break;
	case SW_LEVEL_SUPER:
		{
			m_pDlgMain->ShowWindow(SW_HIDE);
			m_pDlgUser->ShowWindow(SW_HIDE);
			m_pDlgSetup->ShowWindow(SW_HIDE);
			m_pDlgEngr->ShowWindow(SW_HIDE);
			m_pDlgSuper->ShowWindow(SW_SHOW);
		}break;
	}
	ButtonClickEvent(nBtnClickWindow);
}
void CSLTView::MakeLog(LPCTSTR fmt, ...)
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

void CSLTView::OnStnDblclickStaticLogo()
{
	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {
		CUpdateHistory dlg;
		dlg.DoModal();
	}
}

void CSLTView::ButtonClickEvent(int nLevel)
{
	switch(nLevel){
	case SW_MAIN_WINDOW:
		{
			m_BtnUser.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
			m_BtnUser.SetAlign(CButtonST::ST_ALIGN_VERT);
			m_BtnSetup.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
			m_BtnSetup.SetAlign(CButtonST::ST_ALIGN_VERT);
			m_BtnEngr.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
			m_BtnEngr.SetAlign(CButtonST::ST_ALIGN_VERT);
			m_BtnSuper.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
			m_BtnSuper.SetAlign(CButtonST::ST_ALIGN_VERT);
			GetDlgItem(IDC_BTN_CAP)->ShowWindow(SW_SHOW);
		}break;
	case SW_LEVEL_USER:
		{
			m_BtnUser.SetShade(CShadeButtonST::SHS_METAL,30,20,10,RGB(0,0,0));
			m_BtnUser.SetAlign(CButtonST::ST_ALIGN_VERT);
			m_BtnSetup.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
			m_BtnSetup.SetAlign(CButtonST::ST_ALIGN_VERT);
			m_BtnEngr.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
			m_BtnEngr.SetAlign(CButtonST::ST_ALIGN_VERT);
			m_BtnSuper.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
			m_BtnSuper.SetAlign(CButtonST::ST_ALIGN_VERT);
			GetDlgItem(IDC_BTN_CAP)->ShowWindow(SW_HIDE);
		}break;
	case SW_LEVEL_SETUP:
		{
			m_BtnUser.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
			m_BtnUser.SetAlign(CButtonST::ST_ALIGN_VERT);
			m_BtnSetup.SetShade(CShadeButtonST::SHS_METAL,30,20,10,RGB(0,0,0));
			m_BtnSetup.SetAlign(CButtonST::ST_ALIGN_VERT);
			m_BtnEngr.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
			m_BtnEngr.SetAlign(CButtonST::ST_ALIGN_VERT);
			m_BtnSuper.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
			m_BtnSuper.SetAlign(CButtonST::ST_ALIGN_VERT);
			GetDlgItem(IDC_BTN_CAP)->ShowWindow(SW_HIDE);
		}break;
	case SW_LEVEL_ENGR:
		{
			m_BtnUser.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
			m_BtnUser.SetAlign(CButtonST::ST_ALIGN_VERT);
			m_BtnSetup.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
			m_BtnSetup.SetAlign(CButtonST::ST_ALIGN_VERT);
			m_BtnEngr.SetShade(CShadeButtonST::SHS_METAL,30,20,10,RGB(0,0,0));
			m_BtnEngr.SetAlign(CButtonST::ST_ALIGN_VERT);
			m_BtnSuper.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
			m_BtnSuper.SetAlign(CButtonST::ST_ALIGN_VERT);
			GetDlgItem(IDC_BTN_CAP)->ShowWindow(SW_HIDE);
		}break;
	case SW_LEVEL_SUPER:
		{
			m_BtnUser.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
			m_BtnUser.SetAlign(CButtonST::ST_ALIGN_VERT);
			m_BtnSetup.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
			m_BtnSetup.SetAlign(CButtonST::ST_ALIGN_VERT);
			m_BtnEngr.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
			m_BtnEngr.SetAlign(CButtonST::ST_ALIGN_VERT);
			m_BtnSuper.SetShade(CShadeButtonST::SHS_METAL,30,20,10,RGB(0,0,0));
			m_BtnSuper.SetAlign(CButtonST::ST_ALIGN_VERT);
			GetDlgItem(IDC_BTN_CAP)->ShowWindow(SW_HIDE);
		}break;
	}



	//SHS_NOISE = 0,
	//	SHS_DIAGSHADE,
	//	SHS_HSHADE,
	//	SHS_VSHADE,
	//	SHS_HBUMP,
	//	SHS_VBUMP,
	//	SHS_SOFTBUMP,
	//	SHS_HARDBUMP,
	//	SHS_METAL
	
}


void CSLTView::LoadProjectName()
{
	//Customer
	char szPathFileName[MAX_PATH] = { 0, };
	char szPathTemp[512] = { 0, };
	CString strTitle = _T("");
	CString strProjectName = _T("");
	CString strCompany = _T("");
	CString strTesterOption = _T("");
	CString strDebug = _T("");
	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	sprintf_s(szPathFileName, sizeof(szPathFileName), "%s\\sys_data\\%s", szPathTemp, SZ_DRIVER_FILE_NAME);
	CConfigData ConfigCustomer(szPathFileName);

	int nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);

	switch (nProjectName)
	{
		case DEF_PROJECT_LOWLAND:
		{
			strProjectName = "LowLand";
		}break;
		case DEF_PROJECT_CAMPBELTOWN:
		{
			strProjectName = "Campbeltown";
		}break;
		case DEF_PROJECT_WATT:
		{
			strProjectName = "Watt";
		}break;
		case DEF_PROJECT_CARPENTERS:
		{
			strProjectName = "Carpenters";
		}
	default:
		break;
	}


	if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0)
	{
		GetDlgItem(IDC_STATIC_VERSION)->SetWindowText(PGM_VERSION_AMKOR);
		strCompany = DEF_COMPANY_AMKOR;
	}

	if (strcmp(szCompany, DEF_COMPANY_AMKOR) != 0)
	{// amkor 제외 강제로 barcode/test pp retry 는 0
		g_DMCont.m_dmTestPP.SN(NDM4_TestPP1_RetryCnt, 0);
		g_DMCont.m_dmTestPP.SN(NDM4_TestPP2_RetryCnt, 0);
		g_DMCont.m_dmShuttle.SN(NDM3_LdTbl_BarcodeMaxRetry, 0);
	}

	if (nTesterOption == eTester_Dual)
		strTesterOption = "/D-T"; // DualTester [12/13/2021 donghyun.shin]
	else if(nTesterOption == eTester_Agent)
		strTesterOption = "/A-T"; //- Agent
	else if (nTesterOption == eTester_EachSocket)
		strTesterOption = "/E-S"; //- Agent

	if (DEBUG_SERVER == TRUE)
		strDebug = "-Debug";

	strTitle.Format("%s/%s%s %s", strProjectName, strCompany, strTesterOption, strDebug);
	GetDlgItem(IDC_STATIC_PROJECT_NAME)->SetWindowText(strTitle);

}

BOOL CSLTView::XCopy(LPCTSTR Src, LPCTSTR Dest)
{
	HANDLE hSrch = NULL;
	WIN32_FIND_DATA wfd;
	BOOL bResult = TRUE;
	TCHAR WildCard[MAX_PATH] = { 0, };
	TCHAR SrcFile[MAX_PATH] = { 0, };
	TCHAR DestFile[MAX_PATH] = { 0, };

	wsprintf(WildCard, "%s\\*.*", Src);
	CreateDirectory(Dest, NULL);
	hSrch = FindFirstFile(WildCard, &wfd);
	if (hSrch == INVALID_HANDLE_VALUE)
		return FALSE;
	while (bResult) {
		wsprintf(SrcFile, "%s\\%s", Src, wfd.cFileName);
		wsprintf(DestFile, "%s\\%s", Dest, wfd.cFileName);
		// 서브 디렉토리가 발견되면 서브 디렉토리를 복사한다.
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (lstrcmp(wfd.cFileName, ".") && lstrcmp(wfd.cFileName, "..")) {
				XCopy(SrcFile, DestFile);
			}
		}
		else {
			CopyFile(SrcFile, DestFile, FALSE);
		}
		bResult = FindNextFile(hSrch, &wfd);
	}
	FindClose(hSrch);
	return TRUE;
}

// jira 311
void CSLTView::OnBnClickedBtnCap()
{
	int nRet = AfxMessageBox("you want capture?", MB_TOPMOST | MB_YESNO);
	if(nRet == IDYES)
	{
		Sleep(10);
		MakeLog("click capture");

		HDC h_dc = ::GetWindowDC(NULL);
		CImage tips_image;

		int cx = ::GetSystemMetrics(SM_CXSCREEN);
		int cy = ::GetSystemMetrics(SM_CYSCREEN);
		int color_depth = ::GetDeviceCaps(h_dc, BITSPIXEL);

		tips_image.Create(cx, cy, color_depth, 0);
		::BitBlt(tips_image.GetDC(), 0, 0, cx, cy, h_dc, 0, 0, SRCCOPY);

		CTime cTmCurrent = CTime::GetCurrentTime();
		CString strCurTime = _T("");
		strCurTime.Format("%04d%02d%02d_%02d%02d%02d", cTmCurrent.GetYear(), cTmCurrent.GetMonth(), cTmCurrent.GetDay(),
			cTmCurrent.GetHour(), cTmCurrent.GetMinute(), cTmCurrent.GetSecond());

		CString strDir;
		strDir.Format("%s\\%s", SZ_CAPUTRE_PATH, strCurTime);
		if (GetFileAttributes(strDir) == INVALID_FILE_ATTRIBUTES) {
			CreateDirectory(strDir, NULL);
		}

		CString strPathDest;
		strPathDest.Format("%s\\%s\\mainimage.jpeg", SZ_CAPUTRE_PATH, strCurTime);
		tips_image.Save(strPathDest, Gdiplus::ImageFormatJPEG);

		::ReleaseDC(NULL, h_dc);
		tips_image.ReleaseDC();

		char szDriverFileName[MAX_PATH] = { 0, };
		char szTemp[512] = { 0, };
		::GetCurrentDirectory(sizeof(szTemp), szTemp);
		sprintf_s(szDriverFileName, sizeof(szDriverFileName), "%s\\sys_data", szTemp);

		strPathDest.Format("%s\\%s\\sys_data", SZ_CAPUTRE_PATH, strCurTime);
		BOOL bRet = XCopy(szDriverFileName, strPathDest);
		if (bRet == FALSE)
		{
			MakeLog("sys data copy Capture fail!");
			AfxMessageBox("Capture is fail!!", MB_TOPMOST);
			return;
		}

		time_t timer;
		struct tm *t;
		timer = time(NULL);
		t = (struct tm *)localtime(&timer);

	

		CString strPathSrc;
		strPathSrc.Format("D:\\Techwing\\HandlerLog\\%04d%02d%02d",t->tm_year+1900, t->tm_mon+1, t->tm_mday);
		strPathDest.Format("%s\\%s\\HandlerLog_%04d%02d%02d", SZ_CAPUTRE_PATH, strCurTime, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
		bRet = XCopy(strPathSrc, strPathDest);
		if (bRet == FALSE)
		{
			MakeLog("Handler today log file  copy Capture fail!");
			AfxMessageBox("Handler today log file is fail!!", MB_TOPMOST);
			return;
		}
		timer = time(NULL) - (24 * 60 * 60);
		t = (struct tm *)localtime(&timer);
		strPathSrc.Format("D:\\Techwing\\HandlerLog\\%04d%02d%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
		strPathDest.Format("%s\\%s\\HandlerLog_%04d%02d%02d", SZ_CAPUTRE_PATH, strCurTime, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
		bRet = XCopy(strPathSrc, strPathDest);
		if (bRet == FALSE)
		{
			MakeLog("Handler yesterday log file  copy Capture fail!");
			AfxMessageBox("Handler yesterday log file is fail!!", MB_TOPMOST);
			return;
		}

		
		MakeLog("Capture success!");
		CString strMsg;
		strMsg.Format("Capture is success[D:\\techwing\\capture\\%s]!!", strCurTime);
		AfxMessageBox(strMsg, MB_TOPMOST);
	}
}

void CSLTView::OnBnClickedBtnLoopCancel()
{
	int nRet = AfxMessageBox("Do you want LOOP Cancel??", MB_YESNO | MB_TOPMOST);
	m_Logger.LOG("Do you want LOOP Cancel ??");

	if (nRet != IDYES) {
		return;
	}

	g_DMCont.m_dmEQP.SB(BDM0_SYS_AUTO_LOOP_CANCEL, TRUE);

	MakeLog("[AutoLOOP Cancel Button Click]");
}

void CSLTView::EnableAutoLOOPCancel()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_BTN_LOOP_CANCEL);
	pBtn->ShowWindow(SW_SHOW);
	pBtn = (CButton*)GetDlgItem(IDC_BTN_ONECYCLE);
	pBtn->ShowWindow(SW_HIDE);
	pBtn = (CButton*)GetDlgItem(IDC_BTN_CANCEL);
	pBtn->ShowWindow(SW_HIDE);
	pBtn = (CButton*)GetDlgItem(IDC_BTN_AUDIT_CANCEL);
	pBtn->ShowWindow(SW_HIDE);
}

void CSLTView::DisableAutoLOOPCancel()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_BTN_LOOP_CANCEL);
	pBtn->ShowWindow(SW_HIDE);
	pBtn = (CButton*)GetDlgItem(IDC_BTN_AUDIT_CANCEL);
	pBtn->ShowWindow(SW_HIDE);
}



void CSLTView::OnBnClickedBtnAuditCancel()
{
	int nRet = AfxMessageBox("Do you want Audit Cancel??", MB_YESNO | MB_TOPMOST);
	m_Logger.LOG("Do you want Audit Cancel ??");

	if (nRet != IDYES) {
		return;
	}

	g_DMCont.m_dmEQP.SB(BDM0_SYS_AUTO_AUDIT_CANCEL, TRUE);

	MakeLog("[AutoAudit Cancel Button Click]");
}

void CSLTView::EnableAutoAuditCancel()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_BTN_AUDIT_CANCEL);
	pBtn->ShowWindow(SW_SHOW);
	pBtn = (CButton*)GetDlgItem(IDC_BTN_ONECYCLE);
	pBtn->ShowWindow(SW_HIDE);
	pBtn = (CButton*)GetDlgItem(IDC_BTN_CANCEL);
	pBtn->ShowWindow(SW_HIDE);
	pBtn = (CButton*)GetDlgItem(IDC_BTN_LOOP_CANCEL);
	pBtn->ShowWindow(SW_HIDE);
}

void CSLTView::OnBnClickedBtnMultiOrigin()
{
	if (g_DMCont.m_dmEQP.GB(BDM0_EQP_RUN_STATE)) {
		AfxMessageBox(_T("Not available while running"));
		return;
	}

	g_McComm.CmdMultiOrigin(eMultiOriginExecuting);
	MakeLog("[Multi Origin Button Click]");
}

void CSLTView::OnStnDblclickLoadSupply()
{
	MakeLog("[Tray Supply Button Click]");

	int nOneCycle = g_DMCont.m_dmEQP.GB(BDM0_SYS_ONE_CYCLE);
	if (nOneCycle == TRUE)
	{
		AfxMessageBox("One Cycle is in progress. It cannot be changed!!");
		return;
	}

	int nLoadSupply = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOAD_SUPPLY);
	if (nLoadSupply == TRUE)
	{
		AfxMessageBox("Load Tray Supply is in progress.!!");
		return;
	}

	int nYesNo = AfxMessageBox("Load Supply End Cancel. Do you want?", MB_YESNO);
	if (nYesNo != IDYES) {
		return;
	}

	int nRetLevel = 0;
	BOOL bRet = g_LibCommCtrl.ShowLogInBox(0, &nRetLevel);

	if (RETURN_OK == bRet && nRetLevel >= eUSERLEVEL_TECHNICIAN) {
	}
	else {
		AfxMessageBox("User Level is low!!");
		return;
	}

	g_McComm.CmdLoadSupply();

	MakeLog("[Tray Supply Button Click]");
}
void CSLTView::TPUT_SocketLock()
{
	int nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nSite = NDM11_SocketOnOff_Site1_1;
	int nGrrSite = NDM11_GRR_SocketOnOff_Site1_1;
	int nAuditSite = NDM11_Audit_SocketOnOff_Site1_1;
	int nIdx = 0;
	for (int Index = 0; Index < eMaxPressUnitCount; Index++)
	{
		for (int nY = 0; nY < STATION_MAX_Y_SITE; nY++)
		{
			for (int nX = 0; nX < STATION_MAX_X_SITE; nX++)
			{			
				nIdx = nX + nY*STATION_MAX_X_SITE;

				if(nIdx == 0 || nIdx == 2)
				{
						
				}
				else
				{
					g_DMCont.m_dmHandlerOpt.SN(/*NDM11_SocketOnOff_Site1_1*/nSite + STATION_MAX_PARA*Index + nX + nY*STATION_MAX_X_SITE, DEF_SITE_SOCKET_LOCK);
					g_DMCont.m_dmHandlerOpt.SN(/*NDM11_SocketOnOff_Site1_1*/nGrrSite + STATION_MAX_PARA*Index + nX + nY*STATION_MAX_X_SITE, DEF_SITE_SOCKET_LOCK);
					g_DMCont.m_dmHandlerOpt.SN(/*NDM11_SocketOnOff_Site1_1*/nAuditSite + STATION_MAX_PARA*Index + nX + nY*STATION_MAX_X_SITE, DEF_SITE_SOCKET_LOCK);
				}				
			}
		}
	}
}

void CSLTView::TPUT_SocketUnLock()
{
	int nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nSite = NDM11_SocketOnOff_Site1_1;
	int nGrrSite = NDM11_GRR_SocketOnOff_Site1_1;
	int nAuditSite = NDM11_Audit_SocketOnOff_Site1_1;
	int nIdx = 0;
	for (int Index = 0; Index < eMaxPressUnitCount; Index++)
	{
		for (int nY = 0; nY < STATION_MAX_Y_SITE; nY++)
		{
			for (int nX = 0; nX < STATION_MAX_X_SITE; nX++)
			{
				if (DEF_SITE_SOCKET_LOCK == g_DMCont.m_dmHandlerOpt.GN(/*NDM11_SocketOnOff_Site1_1*/nSite + STATION_MAX_PARA*Index + nX + nY*STATION_MAX_X_SITE))
					g_DMCont.m_dmHandlerOpt.SN(/*NDM11_SocketOnOff_Site1_1*/nSite + STATION_MAX_PARA*Index + nX + nY*STATION_MAX_X_SITE, DEF_SITE_OFF);

				if(DEF_SITE_SOCKET_LOCK == g_DMCont.m_dmHandlerOpt.GN(/*NDM11_SocketOnOff_Site1_1*/nGrrSite + STATION_MAX_PARA*Index + nX + nY*STATION_MAX_X_SITE))
					g_DMCont.m_dmHandlerOpt.SN(/*NDM11_SocketOnOff_Site1_1*/nGrrSite + STATION_MAX_PARA*Index + nX + nY*STATION_MAX_X_SITE, DEF_SITE_OFF);

				if (DEF_SITE_SOCKET_LOCK == g_DMCont.m_dmHandlerOpt.GN(/*NDM11_SocketOnOff_Site1_1*/nAuditSite + STATION_MAX_PARA*Index + nX + nY*STATION_MAX_X_SITE))
					g_DMCont.m_dmHandlerOpt.SN(/*NDM11_SocketOnOff_Site1_1*/nAuditSite + STATION_MAX_PARA*Index + nX + nY*STATION_MAX_X_SITE, DEF_SITE_OFF);
			}
		}
	}
}

BOOL CSLTView::ChkOSVerion()
{
	//OS ver 날짜와 현재 날짜 비교.
	//현재 날짜가 OS ver 날짜 보다 이전이면 OS Version 최신이 아님.

	
	//////OS version Date 확인.
	CString strVersion = PGM_VERSION_AMKOR;
	int version_date = _ttoi(strVersion.Right(6));


	//////Cur version Date 확인.
	char szFilePath[MAX_PATH] = { 0, };
	char szCurVer[256] = { 0, };
	CString strCurVersion = _T("");
	sprintf_s(szFilePath, sizeof(szFilePath), "%s", SZ_DIR_PATH_HANDLER_VERSION);
	CConfigData CConfig(szFilePath);
	CConfig.GetString("OS Version", "Date", "", szCurVer);

	int current_date = atoi(szCurVer);

	if (current_date > version_date) {
		return FALSE;
	}
	else {
		//현재 저장된 Version보다 OS Version이 더 클경우 저장.
		char cVersion[128] = { 0, };
		itoa(version_date, cVersion, 10); //int to char
		CConfig.Set("OS Version", "Date",cVersion);
		return TRUE;
	}
}
void CSLTView::TabSelect(BOOL bCoverLock)
{
// 	if (m_pMainDlg == NULL)
// 		return;
// 
// 	// Cover Lock이면 Pass/Fail UI, Unlock이면 Door Status
// 	int nSel = bCoverLock == DEF_LOCK ? CMainLowerDlg::eMainTabPageIndexs::MTPI_PASSFAIL : CMainLowerDlg::eMainTabPageIndexs::MTPI_DOOR_STATUS;
// 	m_pMainDlg->m_tabResult.SetCurSel(nSel);
// 	m_pMainDlg->OnTcnSelchangeMainTab(nullptr, nullptr);
}

