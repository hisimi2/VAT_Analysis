// TrayFileSetupDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "TrayFileSetupDlg.h"
#include "afxdialogex.h"
#include "direct.h"
#include "afx.h"

// CTrayFileSetupDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTrayFileSetupDlg, CDialog)

CTrayFileSetupDlg::CTrayFileSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTrayFileSetupDlg::IDD, pParent)
	, m_dTrayPPLtPitchX(0)
	, m_dTrayPPLtPitchY(0)
{

	m_strTrayFile = _T("");
	m_strGetSelectListbox = _T("");

	m_nTrayDiv_Y = 0;
	m_nTrayDiv_X = 0;
	m_dTray_YPitch = 0.0;
	m_dTray_YStart = 0.0;
	m_dTray_XPitch = 0.0;
	m_dTray_XStart = 0.0;
	m_dTray_Thickness = 0.0;

	m_dLdTbl_XPitch = 0.0;
	m_dLdTbl_YPitch = 0.0;

	m_dTestPP_ST_XPitch = 0.0;
	m_dTestPP_ST_YPitch = 0.0;

	m_nTestCleanDiv_X	 = 0;
	m_nTestCleanDiv_Y	 = 0;
	m_dTestClean_XPitch	 = 0.0;
	m_dTestClean_YPitch	 = 0.0;

	m_dTrayPPLtPitchX = 0.0;
	m_dTrayPPLtPitchY = 0.0;

	m_dTestPP_PitchX = 0.0;
	m_dTestPP_PitchY = 0.0;

	m_nTempMode = eTEMP_AMBIENT;

	m_nTrayFile = 0;

	m_nLdTblDiv_X = 0;
	m_nLdTblDiv_Y = 0;
	 
	m_nTestSiteDiv_X = 0;
	m_nTestSiteDiv_Y = 0;
	
	m_dTestSite_XPitch = 0.0;
	m_dTestSite_YPitch = 0.0;

	m_bDvcHorVer = FALSE;
}

CTrayFileSetupDlg::~CTrayFileSetupDlg()
{
}

void CTrayFileSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_BASE_SET, m_BtnBaseSet);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_BtnDel);
	DDX_Control(pDX, IDC_BUTTON_KIT_CHANGE, m_BtnKitChange);
	DDX_Control(pDX, IDC_BUTTON_TRAYFILE_NEW, m_BtnNew);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_BtnSave);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Text(pDX, IDC_EDIT_LIST, m_strTrayFile);

	DDX_Text(pDX, IDC_EDIT_X_DIV_TRAY, m_nTrayDiv_X);
	DDX_Text(pDX, IDC_EDIT_XPITCH_TRAY, m_dTray_XPitch);
	DDX_Text(pDX, IDC_EDIT_XSTART_TRAY, m_dTray_XStart);
	DDX_Text(pDX, IDC_EDIT_Y_DIV_TRAY, m_nTrayDiv_Y);
	DDX_Text(pDX, IDC_EDIT_YPITCH_TRAY, m_dTray_YPitch);
	DDX_Text(pDX, IDC_EDIT_YSTART_TRAY, m_dTray_YStart);

	DDX_Control(pDX, IDC_LIST_TRAYFILE, m_ListTrayFile);

	DDX_Control(pDX, IDC_EDIT_YSTART_TRAY, m_editYStartTray);
	DDX_Control(pDX, IDC_EDIT_YPITCH_TRAY, m_editYPitchTray);
	DDX_Control(pDX, IDC_EDIT_XSTART_TRAY, m_editXStartTray);
	DDX_Control(pDX, IDC_EDIT_XPITCH_TRAY, m_editXPitchTray);
	DDX_Control(pDX, IDC_EDIT_X_DIV_TRAY, m_editXDivTray);
	DDX_Control(pDX, IDC_EDIT_Y_DIV_TRAY, m_editYDivTray);
	DDX_LBIndex(pDX, IDC_LIST_TRAYFILE, m_nTrayFile);
	DDX_Control(pDX, IDC_EDIT_TRAY_THICKNESS, m_EditTrayThickness);
	DDX_Text(pDX, IDC_EDIT_TRAY_THICKNESS, m_dTray_Thickness);


	DDX_Text(pDX, IDC_EDIT_XPITCH_LD_TBL2, m_dLdTbl_XPitch);
	DDX_Text(pDX, IDC_EDIT_YPITCH_LD_TBL2, m_dLdTbl_YPitch);
	DDX_Control(pDX, IDC_EDIT_XPITCH_LD_TBL2, m_editLdTbl_XPitch);
	DDX_Control(pDX, IDC_EDIT_YPITCH_LD_TBL2, m_editLdTbl_YPitch);
	DDX_Text(pDX, IDC_EDIT_X_DIV_LD_TBL, m_nLdTblDiv_X);
	DDX_Text(pDX, IDC_EDIT_Y_DIV_LD_TBL, m_nLdTblDiv_Y);
	DDX_Control(pDX, IDC_EDIT_X_DIV_LD_TBL, m_editXDivLdTbl);
	DDX_Control(pDX, IDC_EDIT_Y_DIV_LD_TBL, m_editYDivLdTbl);

	DDX_Control(pDX, IDC_EDIT_TESTPP_ST_PITCH_X, m_editTestPP_ST_XPitch);
	DDX_Control(pDX, IDC_EDIT_TESTPP_ST_PITCH_Y, m_editTestPP_ST_YPitch);
	DDX_Text(pDX, IDC_EDIT_TESTPP_ST_PITCH_X, m_dTestPP_ST_XPitch);
	DDX_Text(pDX, IDC_EDIT_TESTPP_ST_PITCH_Y, m_dTestPP_ST_YPitch);


	DDX_Text(pDX, IDC_EDIT_XPITCH_CLEAN_TBL, m_dTestClean_XPitch);
	DDX_Text(pDX, IDC_EDIT_YPITCH_CLEAN_TBL, m_dTestClean_YPitch);
	DDX_Text(pDX, IDC_EDIT_X_DIV_CLEAN_TBL, m_nTestCleanDiv_X);
	DDX_Text(pDX, IDC_EDIT_Y_DIV_CLEAN_TBL, m_nTestCleanDiv_Y);
	DDX_Control(pDX, IDC_EDIT_XPITCH_CLEAN_TBL, m_editCleanBuffTbl_XPitch);
	DDX_Control(pDX, IDC_EDIT_YPITCH_CLEAN_TBL, m_editCleanBuffTbl_YPitch);
	DDX_Control(pDX, IDC_EDIT_X_DIV_CLEAN_TBL, m_editCleanXDivBuffTbl);
	DDX_Control(pDX, IDC_EDIT_Y_DIV_CLEAN_TBL, m_editCleanYDivBuffTbl);

	//Test Site 추가
	DDX_Text(pDX, IDC_EDIT_XPITCH_TEST_SITE, m_dTestSite_XPitch);
	DDX_Text(pDX, IDC_EDIT_YPITCH_TEST_SITE, m_dTestSite_YPitch);
	DDX_Text(pDX, IDC_EDIT_X_DIV_TEST_SITE, m_nTestSiteDiv_X);
	DDX_Text(pDX, IDC_EDIT_Y_DIV_TEST_SITE, m_nTestSiteDiv_Y);

	DDX_Control(pDX, IDC_EDIT_XPITCH_TEST_SITE, m_editXPitchTestSite);
	DDX_Control(pDX, IDC_EDIT_YPITCH_TEST_SITE, m_editYPitchTestSite);
	DDX_Control(pDX, IDC_EDIT_X_DIV_TEST_SITE, m_editTestXDivTestSite);
	DDX_Control(pDX, IDC_EDIT_Y_DIV_TEST_SITE, m_editTestYDivTestSite);



	DDX_Control(pDX, IDC_EDIT_TRAYPP_LT_PITCH_X, m_editTrayPPLtPitchX);
	DDX_Control(pDX, IDC_EDIT_TRAYPP_LT_PITCH_Y, m_editTrayPPLtPitchY);
	DDX_Text(pDX, IDC_EDIT_TRAYPP_LT_PITCH_X, m_dTrayPPLtPitchX);
	DDX_Text(pDX, IDC_EDIT_TRAYPP_LT_PITCH_Y, m_dTrayPPLtPitchY);

	DDX_Control(pDX, IDC_EDIT_TESTPP_PITCH_X1, m_editTrayPP_PitchX);
	DDX_Control(pDX, IDC_EDIT_TESTPP_PITCH_Y1, m_editTrayPP_PitchY);
	DDX_Text(pDX, IDC_EDIT_TESTPP_PITCH_X1, m_dTestPP_PitchX);
	DDX_Text(pDX, IDC_EDIT_TESTPP_PITCH_Y1, m_dTestPP_PitchY);
}


BEGIN_MESSAGE_MAP(CTrayFileSetupDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_TRAYFILE_NEW, &CTrayFileSetupDlg::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CTrayFileSetupDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CTrayFileSetupDlg::OnBnClickedButtonDelete)
	ON_LBN_SELCHANGE(IDC_LIST_TRAYFILE, &CTrayFileSetupDlg::OnLbnSelchangeListTrayfile)
	ON_EN_SETFOCUS(IDC_EDIT_XSTART_TRAY, &CTrayFileSetupDlg::OnEnSetfocusEditXstartTray)
	ON_EN_SETFOCUS(IDC_EDIT_XPITCH_TRAY, &CTrayFileSetupDlg::OnEnSetfocusEditXpitchTray)
	ON_EN_SETFOCUS(IDC_EDIT_YSTART_TRAY, &CTrayFileSetupDlg::OnEnSetfocusEditYstartTray)
	ON_EN_SETFOCUS(IDC_EDIT_YPITCH_TRAY, &CTrayFileSetupDlg::OnEnSetfocusEditYpitchTray)
	ON_EN_SETFOCUS(IDC_EDIT_X_DIV_TRAY, &CTrayFileSetupDlg::OnEnSetfocusEditXDivTray)
	ON_EN_SETFOCUS(IDC_EDIT_Y_DIV_TRAY, &CTrayFileSetupDlg::OnEnSetfocusEditYDivTray)
	ON_EN_SETFOCUS(IDC_EDIT_XPITCH_LD_TBL2, &CTrayFileSetupDlg::OnEnSetfocusEditXpitchLdTbl)
	ON_EN_SETFOCUS(IDC_EDIT_YPITCH_LD_TBL2, &CTrayFileSetupDlg::OnEnSetfocusEditYpitchLdTbl)
	ON_EN_SETFOCUS(IDC_EDIT_TRAY_THICKNESS, &CTrayFileSetupDlg::OnEnSetfocusEditTrayThickness)
    ON_EN_SETFOCUS(IDC_EDIT_X_DIV_LD_TBL, &CTrayFileSetupDlg::OnEnSetfocusEditXDivLdTbl)
    ON_EN_SETFOCUS(IDC_EDIT_Y_DIV_LD_TBL, &CTrayFileSetupDlg::OnEnSetfocusEditYDivLdTbl)
	ON_EN_SETFOCUS(IDC_EDIT_XPITCH_TEST_SITE, &CTrayFileSetupDlg::OnEnSetfocusEditXpitchTestSite)
	ON_EN_SETFOCUS(IDC_EDIT_YPITCH_TEST_SITE, &CTrayFileSetupDlg::OnEnSetfocusEditYpitchTestSite)
	ON_EN_SETFOCUS(IDC_EDIT_X_DIV_TEST_SITE, &CTrayFileSetupDlg::OnEnSetfocusEditXDivTestSite)
	ON_EN_SETFOCUS(IDC_EDIT_Y_DIV_TEST_SITE, &CTrayFileSetupDlg::OnEnSetfocusEditYDivTestSite)
	ON_EN_SETFOCUS(IDC_EDIT_TRAYPP_LT_PITCH_X, &CTrayFileSetupDlg::OnEnSetfocusEditTrayppLtPitchX)
	ON_EN_SETFOCUS(IDC_EDIT_TRAYPP_LT_PITCH_Y, &CTrayFileSetupDlg::OnEnSetfocusEditTrayppLtPitchY)
	ON_EN_SETFOCUS(IDC_EDIT_XPITCH_CLEAN_TBL, &CTrayFileSetupDlg::OnEnSetfocusEditXpitchCleanTbl)
	ON_EN_SETFOCUS(IDC_EDIT_YPITCH_CLEAN_TBL, &CTrayFileSetupDlg::OnEnSetfocusEditYpitchCleanTbl)
	ON_EN_SETFOCUS(IDC_EDIT_X_DIV_CLEAN_TBL, &CTrayFileSetupDlg::OnEnSetfocusEditXDivCleanTbl)
	ON_EN_SETFOCUS(IDC_EDIT_Y_DIV_CLEAN_TBL, &CTrayFileSetupDlg::OnEnSetfocusEditYDivCleanTbl)
	ON_EN_SETFOCUS(IDC_EDIT_TESTPP_PITCH_X1, &CTrayFileSetupDlg::OnEnSetfocusEditTestppPitchX1)
	ON_EN_SETFOCUS(IDC_EDIT_TESTPP_PITCH_Y1, &CTrayFileSetupDlg::OnEnSetfocusEditTestppPitchY1)
	ON_EN_SETFOCUS(IDC_EDIT_TESTPP_ST_PITCH_X, &CTrayFileSetupDlg::OnEnSetfocusEditTestppStPitchX)
	ON_EN_SETFOCUS(IDC_EDIT_TESTPP_ST_PITCH_Y, &CTrayFileSetupDlg::OnEnSetfocusEditTestppStPitchY)
END_MESSAGE_MAP()


// CTrayFileSetupDlg 메시지 처리기입니다.


BOOL CTrayFileSetupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	char szData[MAX_PATH] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szData, sizeof(szData));
	GetDlgItem(IDC_EDIT_LIST)->SetWindowText((LPCTSTR)szData);
	m_strGetSelectListbox = m_strTrayFile = (LPCTSTR)szData;

	OnBtnGUI();
	OnEditBoxGUI();
	InitListData();
	InitTrayData((LPCTSTR)szData);		

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTrayFileSetupDlg::OnEditBoxGUI()
{
	m_editYStartTray    .SetBackColor(COLOR_WWHITE);
	m_editYPitchTray    .SetBackColor(COLOR_WWHITE);
	m_editXStartTray    .SetBackColor(COLOR_WWHITE);
	m_editXPitchTray    .SetBackColor(COLOR_WWHITE);
	m_editXDivTray      .SetBackColor(COLOR_WWHITE);
	m_editYDivTray      .SetBackColor(COLOR_WWHITE);
	m_EditTrayThickness .SetBackColor(COLOR_WWHITE);
	m_editLdTbl_XPitch  .SetBackColor(COLOR_WWHITE);
	m_editLdTbl_YPitch  .SetBackColor(COLOR_WWHITE);
    m_editXDivLdTbl     .SetBackColor(COLOR_WWHITE); 
    m_editYDivLdTbl     .SetBackColor(COLOR_WWHITE);

	m_editTestPP_ST_XPitch.SetBackColor(COLOR_WWHITE);
	m_editTestPP_ST_YPitch.SetBackColor(COLOR_WWHITE);

	m_editCleanXDivBuffTbl   .SetBackColor(COLOR_WWHITE); 
	m_editCleanYDivBuffTbl   .SetBackColor(COLOR_WWHITE); 
	m_editCleanBuffTbl_XPitch.SetBackColor(COLOR_WWHITE); 
	m_editCleanBuffTbl_YPitch.SetBackColor(COLOR_WWHITE); 

	m_editTrayPPLtPitchX.SetBackColor(COLOR_WWHITE);
	m_editTrayPPLtPitchY.SetBackColor(COLOR_WWHITE);

	m_editTrayPP_PitchX.SetBackColor(COLOR_WWHITE);
	m_editTrayPP_PitchY.SetBackColor(COLOR_WWHITE);

	m_editYStartTray    .SetTextSize(30);
	m_editYPitchTray    .SetTextSize(30);
	m_editXStartTray    .SetTextSize(30);
	m_editXPitchTray    .SetTextSize(30);
	m_editXDivTray      .SetTextSize(25);
	m_editYDivTray      .SetTextSize(25);
	m_EditTrayThickness .SetTextSize(30);
	m_editLdTbl_XPitch  .SetTextSize(25);
	m_editLdTbl_YPitch  .SetTextSize(25);
    m_editXDivLdTbl     .SetTextSize(30);
    m_editYDivLdTbl     .SetTextSize(30);

	m_editTestPP_ST_XPitch.SetTextSize(25);
	m_editTestPP_ST_YPitch.SetTextSize(25);

	m_editCleanXDivBuffTbl   .SetTextSize(30); 
	m_editCleanYDivBuffTbl   .SetTextSize(30); 
	m_editCleanBuffTbl_XPitch.SetTextSize(30); 
	m_editCleanBuffTbl_YPitch.SetTextSize(30); 

	m_editTestXDivTestSite.SetBackColor(COLOR_WWHITE); 
	m_editTestYDivTestSite.SetBackColor(COLOR_WWHITE); 
	m_editXPitchTestSite.SetBackColor(COLOR_WWHITE);
	m_editYPitchTestSite.SetBackColor(COLOR_WWHITE);
	m_editTestXDivTestSite.SetTextSize(30);
	m_editTestYDivTestSite.SetTextSize(30);
	m_editXPitchTestSite.SetTextSize(30);
	m_editYPitchTestSite.SetTextSize(30);

	m_editTrayPPLtPitchX.SetTextSize(30);
	m_editTrayPPLtPitchY.SetTextSize(30);

	m_editTrayPP_PitchX.SetTextSize(30);
	m_editTrayPP_PitchY.SetTextSize(30);
}

void CTrayFileSetupDlg::InitListData()
{
	m_ListTrayFile.ResetContent();

	_chdir("SYS_DATA");
	_chdir("HandlerRecipe");
	_chdir(SZ_TRAY_FOLDER);
	CFileFind finder;
	BOOL endf = finder.FindFile();

	while(endf) 
	{
		endf = finder.FindNextFile();
		CString strExt = finder.GetFileName();

		// TrayFile의 경우 Package단위로 Folder내부로 관리하기 때문에 예외처리
		if(!finder.IsDots() && finder.IsDirectory()){		
			m_ListTrayFile.AddString(strExt);
		}
		NEXT;
	}
	_chdir("..");
	_chdir("..");
	_chdir("..");

	CString strSetPKGData = _T("");
	CString strData = _T("");
	GetDlgItem(IDC_EDIT_LIST)->GetWindowText(strSetPKGData);
	for(int i=0; i< m_ListTrayFile.GetCount(); i++)
	{
		strData.Empty();
		m_ListTrayFile.GetText(i, strData);
		if(strData == strSetPKGData){
			m_ListTrayFile.SetCurSel(i);
			m_nTrayFile = i;
		}
	}
}

void CTrayFileSetupDlg::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnBaseSet.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnBaseSet.SetIcon(IDI_BTN_BASE);
	m_BtnBaseSet.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnDel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnDel.SetIcon(IDI_BTN_CLEAR);
	m_BtnDel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnKitChange.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnKitChange.SetIcon(IDI_BTN_KIT_CHAGE);
	m_BtnKitChange.SetAlign(CButtonST::ST_ALIGN_VERT);
		
	m_BtnNew.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnNew.SetIcon(IDI_BTN_NEW);
	m_BtnNew.SetAlign(CButtonST::ST_ALIGN_VERT);
}

void CTrayFileSetupDlg::InitTrayData(CString strRevName)
{
	char szTempCokName[512] = {0,};
	sprintf_s(szTempCokName, sizeof(szTempCokName), "%s", (LPCTSTR)strRevName);
	g_COK_FileMgr.LoadCokInfo(szTempCokName, m_stCokInfo);
	g_COK_FileMgr.LoadBasicInfo(szTempCokName, m_stBasicInfo);

	m_nTrayDiv_X     = m_stCokInfo.aTrayForm.nCnt_X;
	m_nTrayDiv_Y     = m_stCokInfo.aTrayForm.nCnt_Y;
	m_dTray_XPitch   = (double)m_stCokInfo.aTrayForm.nPitchX/1000.0;
	m_dTray_YPitch   = (double)m_stCokInfo.aTrayForm.nPitchY/1000.0;
	m_dTray_XStart   = (double)m_stCokInfo.aTrayForm.nStart_PitchX/1000.0;
	m_dTray_YStart   = (double)m_stCokInfo.aTrayForm.nStart_PitchY/1000.0;
	m_dTray_Thickness= (double)m_stCokInfo.aTrayForm.nThickness/1000.0;

    m_nLdTblDiv_X    = m_stCokInfo.nLoadTbl_XCnt;
    m_nLdTblDiv_Y    = m_stCokInfo.nLoadTbl_YCnt;
	m_dLdTbl_XPitch  = (double)m_stCokInfo.nLoadTbl_Xpitch/1000.0;
	m_dLdTbl_YPitch  = (double)m_stCokInfo.nLoadTbl_Ypitch/1000.0;

	m_dTestPP_ST_XPitch	= (double)m_stCokInfo.nTestPP_ST_XPitch / 1000.0;
	m_dTestPP_ST_YPitch	= (double)m_stCokInfo.nTestPP_ST_YPitch / 1000.0;

	m_nTempMode      = (int)m_stBasicInfo.bTempMode;

	m_dTestClean_XPitch = (double)m_stCokInfo.nTestClean_Xpitch / 1000.0;
	m_dTestClean_YPitch = (double)m_stCokInfo.nTestClean_Ypitch / 1000.0;
	m_nTestCleanDiv_X   = m_stCokInfo.nTestClean_XCnt;
	m_nTestCleanDiv_Y   = m_stCokInfo.nTestClean_YCnt;

	m_dTestSite_XPitch = (double)m_stCokInfo.nTestSite_Xpitch / 1000.0;
	m_dTestSite_YPitch = (double)m_stCokInfo.nTestSite_Ypitch / 1000.0;
	m_nTestSiteDiv_X   = m_stCokInfo.nTestSite_XCnt;
	m_nTestSiteDiv_Y   = m_stCokInfo.nTestSite_YCnt;

	m_dTrayPPLtPitchX = (double)m_stCokInfo.nTrayPPLtPitchX / 1000.0;
	m_dTrayPPLtPitchY = (double)m_stCokInfo.nTrayPPLtPitchY / 1000.0;

	m_dTestPP_PitchX = (double)m_stCokInfo.nTestPP_Xpitch / 1000.0;
	m_dTestPP_PitchY = (double)m_stCokInfo.nTestPP_Ypitch / 1000.0;

	UpdateData(FALSE);
}

void CTrayFileSetupDlg::OnBnClickedButtonNew()
{
	CRegisterTrayFile DO;
	DO.DoModal();

	InitListData();
}


void CTrayFileSetupDlg::OnBnClickedButtonSave()
{//m_strGetSelectListbox = File Name
	char szTrayFileName[MAX_PATH] = {0,};	
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName,szTrayFileName,sizeof(szTrayFileName));

	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
	if(bLotState == eHandlerLotMode_OnGoing && m_strGetSelectListbox.Compare(szTrayFileName) == 0){
		AfxMessageBox("Handler is Running!! Please save after lot end");
		return;
	}

	ST_COK_INFO   stBeforeCokInfo = m_stCokInfo;
	ST_BASIC_INFO stBeforeBasicInfo = m_stBasicInfo;

	char szTempCokName[512] = {0,};
	sprintf_s(szTempCokName, sizeof(szTempCokName), "%s", (LPCTSTR)m_strGetSelectListbox);

	UpdateData(TRUE);
	if(m_strGetSelectListbox.Compare(szTrayFileName) == 0)
	{
		// SJ TODO :: InterLock [Lot 진행 중일 때는 해당 Pkg 변경 불가 해야한다]
		g_DMCont.m_dmTrayPP.SN(NDM5_TrayDiv_X, m_nTrayDiv_X);
		g_DMCont.m_dmTrayPP.SN(NDM5_TrayDiv_Y, m_nTrayDiv_Y);
		g_DMCont.m_dmTrayPP.SN(NDM5_Tray_Start_X, (int)(m_dTray_XStart*1000));
		g_DMCont.m_dmTrayPP.SN(NDM5_Tray_Start_Y, (int)(m_dTray_YStart*1000));
		g_DMCont.m_dmTrayPP.SN(NDM5_Tray_Pitch_X, (int)(m_dTray_XPitch*1000));
		g_DMCont.m_dmTrayPP.SN(NDM5_Tray_Pitch_Y, (int)(m_dTray_YPitch*1000));
		g_DMCont.m_dmTrayPP.SN(NDM5_Tray_Thickness, (int)(m_dTray_Thickness*1000));

		g_DMCont.m_dmShuttle.SN(NDM3_LdTbl_Div_X , m_nLdTblDiv_X);
		g_DMCont.m_dmShuttle.SN(NDM3_LdTbl_Div_Y , m_nLdTblDiv_Y);
		g_DMCont.m_dmShuttle.SN(NDM3_LdTbl_Pitch_X,  (int)(m_dLdTbl_XPitch*1000));
		g_DMCont.m_dmShuttle.SN(NDM3_LdTbl_Pitch_Y,  (int)(m_dLdTbl_YPitch*1000));

		g_DMCont.m_dmTestPP.SN(NDM4_TestCleanTbl_Div_X  ,	m_nTestCleanDiv_X);
		g_DMCont.m_dmTestPP.SN(NDM4_TestCleanTbl_Div_Y  ,	m_nTestCleanDiv_Y);
		g_DMCont.m_dmTestPP.SN(NDM4_TestCleanTbl_Pitch_X,	(int)(m_dTestClean_XPitch*1000));
		g_DMCont.m_dmTestPP.SN(NDM4_TestCleanTbl_Pitch_Y,	(int)(m_dTestClean_YPitch*1000));
		
		g_DMCont.m_dmTestPP.SN(NDM4_TestPP_ST_Pitch_X, (int)(m_dTestPP_ST_XPitch * 1000));
		g_DMCont.m_dmTestPP.SN(NDM4_TestPP_ST_Pitch_Y, (int)(m_dTestPP_ST_YPitch * 1000));
		

		g_DMCont.m_dmTestPP.SN(NDM4_TestSite_Div_X, m_nTestSiteDiv_X);
		g_DMCont.m_dmTestPP.SN(NDM4_TestSite_Div_Y, m_nTestSiteDiv_Y);
		g_DMCont.m_dmTestPP.SN(NDM4_TestSite_Pitch_X, (int)(m_dTestSite_XPitch*1000));
		g_DMCont.m_dmTestPP.SN(NDM4_TestSite_Pitch_Y, (int)(m_dTestSite_YPitch*1000));

		g_DMCont.m_dmTrayPP.SN(NDM5_TrayPP_LdTbl_Pitch_X, (int)(m_dTrayPPLtPitchX * 1000));
		g_DMCont.m_dmTrayPP.SN(NDM5_TrayPP_LdTbl_Pitch_Y, (int)(m_dTrayPPLtPitchY * 1000));

		g_DMCont.m_dmTestPP.SN(NDM4_TestPP_Pitch_X,	(int)(m_dTestPP_PitchX*1000));
		g_DMCont.m_dmTestPP.SN(NDM4_TestPP_Pitch_Y,	(int)(m_dTestPP_PitchY*1000));
	}

	m_stCokInfo.aTrayForm.nCnt_X        = m_nTrayDiv_X;
	m_stCokInfo.aTrayForm.nCnt_Y        = m_nTrayDiv_Y;
	m_stCokInfo.aTrayForm.nPitchX       = (int)(m_dTray_XPitch*1000);
	m_stCokInfo.aTrayForm.nPitchY       = (int)(m_dTray_YPitch*1000);
	m_stCokInfo.aTrayForm.nStart_PitchX = (int)(m_dTray_XStart*1000);
	m_stCokInfo.aTrayForm.nStart_PitchY = (int)(m_dTray_YStart*1000);
	m_stCokInfo.aTrayForm.nThickness    = (int)(m_dTray_Thickness*1000);

    m_stCokInfo.nLoadTbl_XCnt            = m_nLdTblDiv_X;
    m_stCokInfo.nLoadTbl_YCnt            = m_nLdTblDiv_Y;
	m_stCokInfo.nLoadTbl_Xpitch          = (int)(m_dLdTbl_XPitch*1000);
	m_stCokInfo.nLoadTbl_Ypitch          = (int)(m_dLdTbl_YPitch*1000);

	m_stCokInfo.nTestPP_ST_XPitch		=	(int)(m_dTestPP_ST_XPitch * 1000);
	m_stCokInfo.nTestPP_ST_YPitch		=	(int)(m_dTestPP_ST_YPitch * 1000);


	m_stCokInfo.nTestClean_XCnt            = m_nTestCleanDiv_X;
	m_stCokInfo.nTestClean_YCnt            = m_nTestCleanDiv_Y;
	m_stCokInfo.nTestClean_Xpitch          = (int)(m_dTestClean_XPitch*1000);
	m_stCokInfo.nTestClean_Ypitch          = (int)(m_dTestClean_YPitch*1000);

	m_stBasicInfo.bTempMode				 = (BOOL)m_nTempMode;

	m_stCokInfo.nTestSite_XCnt            = m_nTestSiteDiv_X;
	m_stCokInfo.nTestSite_YCnt            = m_nTestSiteDiv_Y;
	m_stCokInfo.nTestSite_Xpitch          = (int)(m_dTestSite_XPitch*1000);
	m_stCokInfo.nTestSite_Ypitch          = (int)(m_dTestSite_YPitch*1000);

	m_stCokInfo.nTrayPPLtPitchX			  = (int)(m_dTrayPPLtPitchX * 1000);
	m_stCokInfo.nTrayPPLtPitchY			  = (int)(m_dTrayPPLtPitchY * 1000);

	m_stCokInfo.nTestPP_Xpitch			  = (int)(m_dTestPP_PitchX * 1000);
	m_stCokInfo.nTestPP_Ypitch			  = (int)(m_dTestPP_PitchY * 1000);

	//int nParaType = g_DMCont.m_dmHandlerOpt.GN(NDM11_Tester_Para);//
	//if( nParaType == eTester_64Para ){
	//	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_SocketClean , eOPTION_NOT_USE );
	//}

	g_COK_FileMgr.SaveCokInfo(szTempCokName  , &m_stCokInfo);
	g_COK_FileMgr.SaveBasicInfo(szTempCokName, &m_stBasicInfo);

	//GUI Data Change Log
	std::vector<std::pair<CString,std::pair<double,double>>> vData;  // vData.first = stCokData, vData.second.first = BeforeData vData.second.second = AfterData

	stBeforeCokInfo.IsSameTrayFile(m_stCokInfo, vData);
	stBeforeBasicInfo.IsSameTrayFile(m_stBasicInfo, vData);

	if(!(vData.empty()))
	{
		TrayFileSetupChangeLog(vData);
	}
}


void CTrayFileSetupDlg::OnBnClickedButtonDelete()
{
	UpdateData(TRUE);
	int nRetMsg = 0;

	if(m_strGetSelectListbox == m_strTrayFile){
		AfxMessageBox("can't delete this file!! Because this is setup file...!!!");
		return;
	}else{
		nRetMsg = AfxMessageBox(" Do you want to delete?", MB_TOPMOST | MB_YESNO);

		if(nRetMsg != IDYES)
		{
			return;
		}
		else
		{
			MakeLog("[MENU = Tray File Setup, File Name = %s ][ Delete ]", m_strGetSelectListbox);
		}
	}

	char szTemp[512]={0,};
	::GetCurrentDirectory(sizeof(szTemp), szTemp);

	CString strDir = _T("");
	strDir.Format("%s\\sys_data\\HandlerRecipe\\%s\\%s",szTemp,SZ_TRAY_FOLDER,m_strGetSelectListbox);

	CString cstrFindFile = strDir + "\\*.*";
	CFileFind Finder;
	BOOL bExist = Finder.FindFile( cstrFindFile );
	while( bExist )
	{
		NEXT;
		bExist = Finder.FindNextFile();	
		if( Finder.IsDots()){ continue; }
		else
		{
			CString cstrFileName = Finder.GetFilePath();
			CFileStatus status;
			CFile::GetStatus( cstrFileName, status );
			::DeleteFile(cstrFileName);
		}
	}
	Finder.Close();
	int nRtn = ::RemoveDirectory( strDir );
	nRtn = ::GetLastError();

	CString strSetPKGData = _T("");
	GetDlgItem(IDC_EDIT_LIST)->GetWindowText(strSetPKGData);
	char szTempCok[256] = {0,};
	sprintf_s(szTempCok, sizeof(szTempCok), strSetPKGData);
	InitListData();
	InitTrayData(szTempCok);	
}


void CTrayFileSetupDlg::OnLbnSelchangeListTrayfile()
{
	m_ListTrayFile.GetText(m_ListTrayFile.GetCurSel(),m_strGetSelectListbox);
	m_nTrayFile = m_ListTrayFile.GetCurSel();
	InitTrayData(m_strGetSelectListbox);
}


void CTrayFileSetupDlg::OnEnSetfocusEditXstartTray()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTray_XStart);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray X Start", "100", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_XSTART_TRAY)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CTrayFileSetupDlg::OnEnSetfocusEditXpitchTray()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTray_XPitch);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray X Pitch", "100", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_XPITCH_TRAY)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CTrayFileSetupDlg::OnEnSetfocusEditYstartTray()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTray_YStart);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray Y Start", "100", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_YSTART_TRAY)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CTrayFileSetupDlg::OnEnSetfocusEditYpitchTray()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTray_YPitch);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray Y Pitch", "100", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_YPITCH_TRAY)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CTrayFileSetupDlg::OnEnSetfocusEditXDivTray()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nTrayDiv_X, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray X Division", "100", "2");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_X_DIV_TRAY)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CTrayFileSetupDlg::OnEnSetfocusEditYDivTray()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nTrayDiv_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray Y Division", "100", "2");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_Y_DIV_TRAY)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}

void CTrayFileSetupDlg::OnEnSetfocusEditTrayThickness()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTray_Thickness);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray Thickness", "10", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_TRAY_THICKNESS)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}

void CTrayFileSetupDlg::OnEnSetfocusEditXpitchLdTbl()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dLdTbl_XPitch);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table X Pitch", "100", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_XPITCH_LD_TBL2)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CTrayFileSetupDlg::OnEnSetfocusEditYpitchLdTbl()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dLdTbl_YPitch);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table Y Pitch", "100", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_YPITCH_LD_TBL2)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}

void CTrayFileSetupDlg::OnEnSetfocusEditXDivLdTbl()
{
    char szVal[10]    = {0,};
    char szRetVal[64] = {0,};
    _itoa_s(m_nLdTblDiv_X, szVal, sizeof(szVal));

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "LoadTable X Division", "100", "1");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_X_DIV_LD_TBL)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CTrayFileSetupDlg::OnEnSetfocusEditYDivLdTbl()
{
    char szVal[10]    = {0,};
    char szRetVal[64] = {0,};
    _itoa_s(m_nLdTblDiv_Y, szVal, sizeof(szVal));

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "LoadTable Y Division", "100", "1");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_Y_DIV_LD_TBL)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}

void CTrayFileSetupDlg::MakeLog(LPCTSTR fmt, ...)
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

// vData.first = stCokData, vData.second.first = BeforeData vData.second.second = AfterData
void CTrayFileSetupDlg::TrayFileSetupChangeLog(std::vector<std::pair<CString,std::pair<double,double>>> vData)
{
	for(int i = 0 ; i< (int)vData.size(); i++)
	{
		std::pair<CString,std::pair<double,double>> p;

		p = vData[i];
		if(p.first == "Plate Form Temp")
		{
			if(p.second.second == 0)
			{
				MakeLog("[MENU = Tray File Setup, File Name = %s, DATA NAME = Plate Form Temp ][BEFORE= High Temp , AFTER= Ambient ]", m_strGetSelectListbox);
			}
			else if(p.second.second == 1)
			{
				MakeLog("[MENU = Tray File Setup, File Name = %s, DATA NAME = Plate Form Temp ][BEFORE= Ambient , AFTER= High Temp ]", m_strGetSelectListbox);
			}
		}
		else
		{
			MakeLog("[MENU = Tray File Setup, File Name = %s, DATA NAME = %s ][BEFORE=%.2f, AFTER=%.2f]", m_strGetSelectListbox, p.first, p.second.first, p.second.second);
		}
		
	}
}

void CTrayFileSetupDlg::OnEnSetfocusEditXpitchTestSite()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTestSite_XPitch);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test Site X Pitch", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_XPITCH_TEST_SITE)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CTrayFileSetupDlg::OnEnSetfocusEditYpitchTestSite()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTestSite_YPitch);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test Site Y Pitch", "200", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_YPITCH_TEST_SITE)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CTrayFileSetupDlg::OnEnSetfocusEditXDivTestSite()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nTestSiteDiv_X, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test Site X Division", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_X_DIV_TEST_SITE)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CTrayFileSetupDlg::OnEnSetfocusEditYDivTestSite()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nTestSiteDiv_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test Site Y Division", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_Y_DIV_TEST_SITE)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CTrayFileSetupDlg::OnEnSetfocusEditTrayppLtPitchX()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTrayPPLtPitchX);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray PP Load Table Pitch X", "100", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_TRAYPP_LT_PITCH_X)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CTrayFileSetupDlg::OnEnSetfocusEditTrayppLtPitchY()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTrayPPLtPitchY);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray PP Load Table Pitch Y", "100", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_TRAYPP_LT_PITCH_Y)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CTrayFileSetupDlg::OnEnSetfocusEditXpitchCleanTbl()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTestClean_XPitch);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Clean Buff X Pitch", "100", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_XPITCH_CLEAN_TBL)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CTrayFileSetupDlg::OnEnSetfocusEditYpitchCleanTbl()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTestClean_YPitch);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Clean Buff Y Pitch", "100", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_YPITCH_CLEAN_TBL)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CTrayFileSetupDlg::OnEnSetfocusEditXDivCleanTbl()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nTestCleanDiv_X, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Clean Buff X Division", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_X_DIV_CLEAN_TBL)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CTrayFileSetupDlg::OnEnSetfocusEditYDivCleanTbl()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nTestCleanDiv_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Clean Buff Y Division", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_Y_DIV_CLEAN_TBL)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CTrayFileSetupDlg::OnEnSetfocusEditTestppPitchX1()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTestPP_PitchX);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP Pitch X", "100", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_TESTPP_PITCH_X1)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CTrayFileSetupDlg::OnEnSetfocusEditTestppPitchY1()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTestPP_PitchY);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP Pitch Y", "100", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_TESTPP_PITCH_Y1)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CTrayFileSetupDlg::OnEnSetfocusEditTestppStPitchX()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTestPP_ST_XPitch);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP ST Pitch X", "100", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_TESTPP_ST_PITCH_X)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CTrayFileSetupDlg::OnEnSetfocusEditTestppStPitchY()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTestPP_ST_YPitch);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP ST Pitch Y", "100", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_TESTPP_ST_PITCH_Y)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}
