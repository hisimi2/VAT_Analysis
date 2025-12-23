// RegisterTrayFile.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "RegisterTrayFile.h"
#include "afxdialogex.h"


// CRegisterTrayFile 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRegisterTrayFile, CDialog)

CRegisterTrayFile::CRegisterTrayFile(CWnd* pParent /*=NULL*/)
	: CDialog(CRegisterTrayFile::IDD, pParent)
{
	m_strTrayFile = _T("");


	m_nTrayDiv_Y = 0;
	m_nTrayDiv_X = 0;
	m_dTray_YPitch = 0.0;
	m_dTray_YStart = 0.0;
	m_dTray_XPitch = 0.0;
	m_dTray_XStart = 0.0;
	m_dTrayThickness = 0.0;

    m_nLdTblDiv_X = 0;
    m_nLdTblDiv_Y = 0;
	m_dLdTbl_XPitch = 0.0;
	m_dLdTbl_YPitch = 0.0;

    m_nTestSiteDiv_X = 0;
    m_nTestSiteDiv_Y = 0;
    m_dTestSite_XPitch = 0.0;
    m_dTestSite_YPitch = 0.0;

	m_dTestPPSite_XPitch = 0.0;
	m_dTestPPSite_YPitch = 0.0;

// 	m_nTestBuffTblDiv_X = 0;
// 	m_nTestBuffTblDiv_Y = 0;
// 	m_dTestBuffTbl_XPitch = 0.0;
// 	m_dTestBuffTbl_YPitch = 0.0;

	m_nTestCleanDiv_X = 0;
	m_nTestCleanDiv_Y = 0;
	m_dTestClean_XPitch = 0.0;
	m_dTestClean_YPitch = 0.0;

	m_dTrayPPLtPitchX = 0.0;
	m_dTrayPPLtPitchY = 0.0;

	m_dTestPP_PitchX = 0.0;
	m_dTestPP_PitchY = 0.0;

	m_nTempMode = eTEMP_AMBIENT;
}

CRegisterTrayFile::~CRegisterTrayFile()
{
}

void CRegisterTrayFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_BtnSave);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Text(pDX, IDC_EDIT_REG_TRAYFILE, m_strTrayFile);
	DDX_Text(pDX, IDC_EDIT_X_DIV_TRAY, m_nTrayDiv_X);
	DDX_Text(pDX, IDC_EDIT_XPITCH_TRAY, m_dTray_XPitch);
	DDX_Text(pDX, IDC_EDIT_XSTART_TRAY, m_dTray_XStart);
	DDX_Text(pDX, IDC_EDIT_Y_DIV_TRAY, m_nTrayDiv_Y);
	DDX_Text(pDX, IDC_EDIT_YPITCH_TRAY, m_dTray_YPitch);
	DDX_Text(pDX, IDC_EDIT_YSTART_TRAY, m_dTray_YStart);
	DDX_Control(pDX, IDC_EDIT_YSTART_TRAY , m_editYStartTray );
	DDX_Control(pDX, IDC_EDIT_YPITCH_TRAY , m_editYPitchTray );
	DDX_Control(pDX, IDC_EDIT_XSTART_TRAY , m_editXStartTray );
	DDX_Control(pDX, IDC_EDIT_XPITCH_TRAY , m_editXPitchTray );
	DDX_Control(pDX, IDC_EDIT_X_DIV_TRAY  , m_editXDivTray   );
	DDX_Control(pDX, IDC_EDIT_Y_DIV_TRAY  , m_editYDivTray   );
	DDX_Control(pDX, IDC_EDIT_TRAY_THICKNESS, m_EditTrayThickness);
	DDX_Text(pDX, IDC_EDIT_TRAY_THICKNESS, m_dTrayThickness);

	DDX_Text(pDX, IDC_EDIT_XPITCH_LD_TBL, m_dLdTbl_XPitch);
	DDX_Text(pDX, IDC_EDIT_YPITCH_LD_TBL, m_dLdTbl_YPitch);
	DDX_Control(pDX, IDC_EDIT_XPITCH_LD_TBL , m_editLdTbl_XPitch  );
	DDX_Control(pDX, IDC_EDIT_YPITCH_LD_TBL , m_editLdTbl_YPitch  );
	DDX_Text(pDX, IDC_EDIT_X_DIV_LD_TBL, m_nLdTblDiv_X);
	DDX_Text(pDX, IDC_EDIT_Y_DIV_LD_TBL, m_nLdTblDiv_Y);
	DDX_Control(pDX, IDC_EDIT_X_DIV_LD_TBL , m_editLdTblDiv_X );
	DDX_Control(pDX, IDC_EDIT_Y_DIV_LD_TBL , m_editLdTblDiv_Y );

	DDX_Text(pDX, IDC_EDIT_XPITCH_TEST1_SITE, m_dTestSite_XPitch);
	DDX_Text(pDX, IDC_EDIT_YPITCH_TEST1_SITE, m_dTestSite_YPitch);
	DDX_Text(pDX, IDC_EDIT_X_DIV_TEST1_SITE, m_nTestSiteDiv_X);
	DDX_Text(pDX, IDC_EDIT_Y_DIV_TEST1_SITE, m_nTestSiteDiv_Y);

	DDX_Text(pDX, IDC_EDIT_XPITCH_TEST_PP_SITE, m_dTestPPSite_XPitch);
	DDX_Text(pDX, IDC_EDIT_YPITCH_TEST_PP_SITE, m_dTestPPSite_YPitch);

	DDX_Control(pDX, IDC_EDIT_XPITCH_TEST1_SITE , m_editXPitchTestSite  );
	DDX_Control(pDX, IDC_EDIT_YPITCH_TEST1_SITE , m_editYPitchTestSite  );
	DDX_Control(pDX, IDC_EDIT_X_DIV_TEST1_SITE , m_editTestXDivTestSite  );
	DDX_Control(pDX, IDC_EDIT_Y_DIV_TEST1_SITE , m_editTestYDivTestSite  );

	DDX_Control(pDX, IDC_EDIT_XPITCH_TEST_PP_SITE, m_editXPitchTestppSite);
	DDX_Control(pDX, IDC_EDIT_YPITCH_TEST_PP_SITE, m_editYPitchTestppSite);

// 	DDX_Text(pDX, IDC_EDIT_X_DIV_BUFF_TBL1, m_nTestBuffTblDiv_X);
// 	DDX_Text(pDX, IDC_EDIT_Y_DIV_BUFF_TBL1, m_nTestBuffTblDiv_Y);
// 	DDX_Text(pDX, IDC_EDIT_XPITCH_BUFF_TBL1, m_dTestBuffTbl_XPitch);
// 	DDX_Text(pDX, IDC_EDIT_YPITCH_BUFF_TBL1, m_dTestBuffTbl_YPitch);
// 	DDX_Control(pDX, IDC_EDIT_X_DIV_BUFF_TBL1 , m_editTestBuffTblDiv_X );
// 	DDX_Control(pDX, IDC_EDIT_Y_DIV_BUFF_TBL1 , m_editTestBuffTblDiv_Y );
// 	DDX_Control(pDX, IDC_EDIT_XPITCH_BUFF_TBL1 , m_editTestBuffTbl_XPitch  );
// 	DDX_Control(pDX, IDC_EDIT_YPITCH_BUFF_TBL1 , m_editTestBuffTbl_YPitch  );

	DDX_Text(pDX, IDC_EDIT_X_DIV_CLEAN1_TBL, m_nTestCleanDiv_X);
	DDX_Text(pDX, IDC_EDIT_Y_DIV_CLEAN1_TBL, m_nTestCleanDiv_Y);
	DDX_Text(pDX, IDC_EDIT_XPITCH_CLEAN1_TBL, m_dTestClean_XPitch);
	DDX_Text(pDX, IDC_EDIT_YPITCH_CLEAN1_TBL, m_dTestClean_YPitch);
	DDX_Control(pDX, IDC_EDIT_X_DIV_CLEAN1_TBL , m_editCleanXDivBuffTbl );
	DDX_Control(pDX, IDC_EDIT_Y_DIV_CLEAN1_TBL , m_editCleanYDivBuffTbl );
	DDX_Control(pDX, IDC_EDIT_XPITCH_CLEAN1_TBL , m_editCleanBuffTbl_XPitch  );
	DDX_Control(pDX, IDC_EDIT_YPITCH_CLEAN1_TBL , m_editCleanBuffTbl_YPitch  );

	DDX_Text(pDX, IDC_EDIT_TRAYPP1_LT_PITCH_X, m_dTrayPPLtPitchX);
	DDX_Text(pDX, IDC_EDIT_TRAYPP1_LT_PITCH_Y, m_dTrayPPLtPitchY);
	DDX_Control(pDX, IDC_EDIT_TRAYPP1_LT_PITCH_X , m_editTrayPPLtPitchX  );
	DDX_Control(pDX, IDC_EDIT_TRAYPP1_LT_PITCH_Y , m_editTrayPPLtPitchY  );

	DDX_Text(pDX, IDC_EDIT_TESTPP_PITCH_X, m_dTestPP_PitchX);
	DDX_Text(pDX, IDC_EDIT_TESTPP_PITCH_Y, m_dTestPP_PitchY);
	DDX_Control(pDX, IDC_EDIT_TESTPP_PITCH_X , m_editTestPP_PitchX  );
	DDX_Control(pDX, IDC_EDIT_TESTPP_PITCH_Y , m_editTestPP_PitchY  );
}


BEGIN_MESSAGE_MAP(CRegisterTrayFile, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CRegisterTrayFile::OnBnClickedButtonSave)
	ON_EN_SETFOCUS(IDC_EDIT_XSTART_TRAY, &CRegisterTrayFile::OnEnSetfocusEditXstartTray)
	ON_EN_SETFOCUS(IDC_EDIT_XPITCH_TRAY, &CRegisterTrayFile::OnEnSetfocusEditXpitchTray)
	ON_EN_SETFOCUS(IDC_EDIT_YSTART_TRAY, &CRegisterTrayFile::OnEnSetfocusEditYstartTray)
	ON_EN_SETFOCUS(IDC_EDIT_YPITCH_TRAY, &CRegisterTrayFile::OnEnSetfocusEditYpitchTray)
	ON_EN_SETFOCUS(IDC_EDIT_X_DIV_TRAY, &CRegisterTrayFile::OnEnSetfocusEditXDivTray)
	ON_EN_SETFOCUS(IDC_EDIT_Y_DIV_TRAY, &CRegisterTrayFile::OnEnSetfocusEditYDivTray)
	ON_EN_SETFOCUS(IDC_EDIT_XPITCH_LD_TBL, &CRegisterTrayFile::OnEnSetfocusEditXpitchLdTbl)
	ON_EN_SETFOCUS(IDC_EDIT_YPITCH_LD_TBL, &CRegisterTrayFile::OnEnSetfocusEditYpitchLdTbl)
    ON_EN_SETFOCUS(IDC_EDIT_X_DIV_LD_TBL, &CRegisterTrayFile::OnEnSetfocusEditXDivLdTbl)
    ON_EN_SETFOCUS(IDC_EDIT_Y_DIV_LD_TBL, &CRegisterTrayFile::OnEnSetfocusEditYDivLdTbl)
    ON_EN_SETFOCUS(IDC_EDIT_XPITCH_BUFF_TBL1, &CRegisterTrayFile::OnEnSetfocusEditXpitchBuffTbl)
    ON_EN_SETFOCUS(IDC_EDIT_YPITCH_BUFF_TBL1, &CRegisterTrayFile::OnEnSetfocusEditYpitchBuffTbl)
    ON_EN_SETFOCUS(IDC_EDIT_X_DIV_BUFF_TBL1, &CRegisterTrayFile::OnEnSetfocusEditXDivBuffTbl)
    ON_EN_SETFOCUS(IDC_EDIT_Y_DIV_BUFF_TBL1, &CRegisterTrayFile::OnEnSetfocusEditYDivBuffTbl)
    ON_EN_SETFOCUS(IDC_EDIT_TRAY_THICKNESS, &CRegisterTrayFile::OnEnSetfocusEditTrayThickness)
	ON_EN_SETFOCUS(IDC_EDIT_XPITCH_TEST1_SITE, &CRegisterTrayFile::OnEnSetfocusEditXpitchTest1Site)
	ON_EN_SETFOCUS(IDC_EDIT_YPITCH_TEST1_SITE, &CRegisterTrayFile::OnEnSetfocusEditYpitchTest1Site)
	ON_EN_SETFOCUS(IDC_EDIT_X_DIV_TEST1_SITE, &CRegisterTrayFile::OnEnSetfocusEditXDivTest1Site)
	ON_EN_SETFOCUS(IDC_EDIT_Y_DIV_TEST1_SITE, &CRegisterTrayFile::OnEnSetfocusEditYDivTest1Site)
	ON_EN_SETFOCUS(IDC_EDIT_XPITCH_CLEAN1_TBL, &CRegisterTrayFile::OnEnSetfocusEditXpitchClean1Tbl)
	ON_EN_SETFOCUS(IDC_EDIT_YPITCH_CLEAN1_TBL, &CRegisterTrayFile::OnEnSetfocusEditYpitchClean1Tbl)
	ON_EN_SETFOCUS(IDC_EDIT_X_DIV_CLEAN1_TBL, &CRegisterTrayFile::OnEnSetfocusEditXDivClean1Tbl)
	ON_EN_SETFOCUS(IDC_EDIT_Y_DIV_CLEAN1_TBL, &CRegisterTrayFile::OnEnSetfocusEditYDivClean1Tbl)
	ON_EN_SETFOCUS(IDC_EDIT_TRAYPP1_LT_PITCH_X, &CRegisterTrayFile::OnEnSetfocusEditTraypp1LtPitchX)
	ON_EN_SETFOCUS(IDC_EDIT_TRAYPP1_LT_PITCH_Y, &CRegisterTrayFile::OnEnSetfocusEditTraypp1LtPitchY)
	ON_EN_SETFOCUS(IDC_EDIT_TESTPP_PITCH_X, &CRegisterTrayFile::OnEnSetfocusEditTestppPitchX)
	ON_EN_SETFOCUS(IDC_EDIT_TESTPP_PITCH_Y, &CRegisterTrayFile::OnEnSetfocusEditTestppPitchY)
	ON_EN_SETFOCUS(IDC_EDIT_XPITCH_TEST_PP_SITE, &CRegisterTrayFile::OnEnSetfocusEditXpitchTestPpSite)
	ON_EN_SETFOCUS(IDC_EDIT_YPITCH_TEST_PP_SITE, &CRegisterTrayFile::OnEnSetfocusEditYpitchTestPpSite)
END_MESSAGE_MAP()


// CRegisterTrayFile 메시지 처리기입니다.


BOOL CRegisterTrayFile::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnBtnGUI();
	OnEditBoxGUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CRegisterTrayFile::OnBnClickedButtonSave()
{
	UpdateData(TRUE);

	if(m_strTrayFile.IsEmpty()){
		return;
	}

	char szCokName[512] = {0,};
	ST_COK_INFO   stCokData;
	ST_BASIC_INFO stBasicData;
	sprintf_s(szCokName, sizeof(szCokName), m_strTrayFile);

	g_COK_FileMgr.LoadCokInfo(szCokName, stCokData);
	g_COK_FileMgr.LoadBasicInfo(szCokName, stBasicData);

	stCokData.aTrayForm.nStart_PitchX  = (int)(m_dTray_XStart*1000);
	stCokData.aTrayForm.nStart_PitchY  = (int)(m_dTray_YStart*1000);
	stCokData.aTrayForm.nPitchX        = (int)(m_dTray_XPitch*1000);
	stCokData.aTrayForm.nPitchY        = (int)(m_dTray_YPitch*1000);	
	stCokData.aTrayForm.nCnt_X		   = m_nTrayDiv_X;
	stCokData.aTrayForm.nCnt_Y		   = m_nTrayDiv_Y;
	stCokData.aTrayForm.nThickness     = (int)(m_dTrayThickness*1000);

	stCokData.nLoadTbl_Xpitch		   = (int)(m_dLdTbl_XPitch*1000);
	stCokData.nLoadTbl_Ypitch		   = (int)(m_dLdTbl_YPitch*1000);
    stCokData.nLoadTbl_XCnt            = m_nLdTblDiv_X;
    stCokData.nLoadTbl_YCnt            = m_nLdTblDiv_Y;

	stCokData.nTestPP_ST_XPitch			=	(int)(m_dTestPPSite_XPitch * 1000); //m_dTestPP_ST_XPitch
	stCokData.nTestPP_ST_YPitch			=	(int)(m_dTestPPSite_XPitch * 1000); //m_dTestPP_ST_YPitch

    stCokData.nTestSite_Xpitch		   = (int)(m_dTestSite_XPitch*1000);
    stCokData.nTestSite_Ypitch		   = (int)(m_dTestSite_YPitch*1000);
    stCokData.nTestSite_XCnt            = m_nTestSiteDiv_X;
    stCokData.nTestSite_YCnt            = m_nTestSiteDiv_Y;



// 	stCokData.nTestBuffTbl_Xpitch		   = (int)(m_dTestBuffTbl_XPitch*1000);
// 	stCokData.nTestBuffTbl_Ypitch		   = (int)(m_dTestBuffTbl_YPitch*1000);
// 	stCokData.nTestBuffTbl_XCnt            = m_nTestBuffTblDiv_X;
// 	stCokData.nTestBuffTbl_YCnt            = m_nTestBuffTblDiv_Y;

	stCokData.nTestClean_Xpitch		   = (int)(m_dTestClean_XPitch*1000);
	stCokData.nTestClean_Ypitch		   = (int)(m_dTestClean_YPitch*1000);
	stCokData.nTestClean_XCnt            = m_nTestCleanDiv_X;
	stCokData.nTestClean_YCnt            = m_nTestCleanDiv_Y;

	stCokData.nTrayPPLtPitchX	=	(int)(m_dTrayPPLtPitchX*1000);
	stCokData.nTrayPPLtPitchY	=	(int)(m_dTrayPPLtPitchY*1000);

	stCokData.nTestPP_Xpitch	=	(int)(m_dTestPP_PitchX*1000);
	stCokData.nTestPP_Ypitch	=	(int)(m_dTestPP_PitchY*1000);

	g_COK_FileMgr.SaveCokInfo(szCokName, &stCokData);

	stBasicData.bTempMode = (BOOL)m_nTempMode;
	g_COK_FileMgr.SaveBasicInfo(szCokName, &stBasicData);

	MakeLog("[MENU = New Tray File][Tray File Name = %s ][ Create ]", m_strTrayFile);

}

void CRegisterTrayFile::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);
}

void CRegisterTrayFile::OnEditBoxGUI()
{
	m_editYStartTray .SetBackColor(COLOR_WWHITE);
	m_editYPitchTray .SetBackColor(COLOR_WWHITE);
	m_editXStartTray .SetBackColor(COLOR_WWHITE);
	m_editXPitchTray .SetBackColor(COLOR_WWHITE);
	m_editXDivTray   .SetBackColor(COLOR_WWHITE);
	m_editYDivTray   .SetBackColor(COLOR_WWHITE);
	m_EditTrayThickness.SetBackColor(COLOR_WWHITE);

	m_editLdTbl_XPitch.SetBackColor(COLOR_WWHITE);
	m_editLdTbl_YPitch.SetBackColor(COLOR_WWHITE);
    m_editLdTblDiv_X.SetBackColor(COLOR_WWHITE);
    m_editLdTblDiv_Y.SetBackColor(COLOR_WWHITE);

	m_editXPitchTestSite.SetBackColor(COLOR_WWHITE);
	m_editYPitchTestSite.SetBackColor(COLOR_WWHITE);  
	m_editTestXDivTestSite.SetBackColor(COLOR_WWHITE);
	m_editTestYDivTestSite.SetBackColor(COLOR_WWHITE);

	m_editXPitchTestppSite.SetBackColor(COLOR_WWHITE);
	m_editYPitchTestppSite.SetBackColor(COLOR_WWHITE);
	

// 	m_editTestBuffTbl_XPitch.SetBackColor(COLOR_WWHITE);
// 	m_editTestBuffTbl_YPitch.SetBackColor(COLOR_WWHITE);
// 	m_editTestBuffTblDiv_X.SetBackColor(COLOR_WWHITE);
// 	m_editTestBuffTblDiv_Y.SetBackColor(COLOR_WWHITE);

	m_editCleanBuffTbl_XPitch.SetBackColor(COLOR_WWHITE);
	m_editCleanBuffTbl_YPitch.SetBackColor(COLOR_WWHITE);
	m_editCleanXDivBuffTbl.SetBackColor(COLOR_WWHITE);
	m_editCleanYDivBuffTbl.SetBackColor(COLOR_WWHITE);

	m_editTrayPPLtPitchX.SetBackColor(COLOR_WWHITE);
	m_editTrayPPLtPitchY.SetBackColor(COLOR_WWHITE);

	m_editTestPP_PitchX.SetBackColor(COLOR_WWHITE);
	m_editTestPP_PitchY.SetBackColor(COLOR_WWHITE);

	m_editYStartTray .SetTextSize(30);
	m_editYPitchTray .SetTextSize(30);
	m_editXStartTray .SetTextSize(30);
	m_editXPitchTray .SetTextSize(30);
	m_editXDivTray   .SetTextSize(25);
	m_editYDivTray   .SetTextSize(25);
	m_EditTrayThickness.SetTextSize(30);
	m_editLdTbl_XPitch.SetTextSize(25);
	m_editLdTbl_YPitch.SetTextSize(25);
    m_editLdTblDiv_X.SetTextSize(30);
    m_editLdTblDiv_Y.SetTextSize(30);

	m_editXPitchTestSite.SetTextSize(30);
	m_editYPitchTestSite.SetTextSize(30);
	m_editTestXDivTestSite.SetTextSize(30);
	m_editTestYDivTestSite.SetTextSize(30);

// 	m_editTestBuffTbl_XPitch.SetTextSize(30);
// 	m_editTestBuffTbl_YPitch.SetTextSize(30);
// 	m_editTestBuffTblDiv_X.SetTextSize(30);
// 	m_editTestBuffTblDiv_Y.SetTextSize(30);

	m_editCleanBuffTbl_XPitch.SetTextSize(30);
	m_editCleanBuffTbl_YPitch.SetTextSize(30);
	m_editCleanXDivBuffTbl.SetTextSize(30);
	m_editCleanYDivBuffTbl.SetTextSize(30);

	m_editTrayPPLtPitchX.SetTextSize(30);
	m_editTrayPPLtPitchY.SetTextSize(30);

	m_editTestPP_PitchX.SetTextSize(30);
	m_editTestPP_PitchY.SetTextSize(30);

	m_editXPitchTestppSite.SetTextSize(30);
	m_editYPitchTestppSite.SetTextSize(30);
}

void CRegisterTrayFile::OnEnSetfocusEditXstartTray()
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


void CRegisterTrayFile::OnEnSetfocusEditXpitchTray()
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


void CRegisterTrayFile::OnEnSetfocusEditYstartTray()
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


void CRegisterTrayFile::OnEnSetfocusEditYpitchTray()
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


void CRegisterTrayFile::OnEnSetfocusEditXDivTray()
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


void CRegisterTrayFile::OnEnSetfocusEditYDivTray()
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

void CRegisterTrayFile::OnEnSetfocusEditXpitchLdTbl()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dLdTbl_XPitch);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table X Pitch", "100", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_XPITCH_LD_TBL)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CRegisterTrayFile::OnEnSetfocusEditYpitchLdTbl()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dLdTbl_YPitch);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table Y Pitch", "100", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_YPITCH_LD_TBL)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}

void CRegisterTrayFile::OnEnSetfocusEditXDivLdTbl()
{
    char szVal[10]    = {0,};
    char szRetVal[64] = {0,};
    _itoa_s(m_nLdTblDiv_X, szVal, sizeof(szVal));

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table X Division", "100", "1");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_X_DIV_LD_TBL)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CRegisterTrayFile::OnEnSetfocusEditYDivLdTbl()
{
    char szVal[10]    = {0,};
    char szRetVal[64] = {0,};
    _itoa_s(m_nLdTblDiv_Y, szVal, sizeof(szVal));

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table Y Division", "100", "1");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_Y_DIV_LD_TBL)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CRegisterTrayFile::OnEnSetfocusEditXpitchBuffTbl()
{
    char szVal[10]    = {0,};
    char szRetVal[64] = {0,};
    sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTestSite_XPitch);

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Buff Table X Pitch", "100", "0");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_XPITCH_BUFF_TBL1)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CRegisterTrayFile::OnEnSetfocusEditYpitchBuffTbl()
{
    char szVal[10]    = {0,};
    char szRetVal[64] = {0,};
    sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTestSite_YPitch);

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Buff Table Y Pitch", "100", "0");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_YPITCH_BUFF_TBL1)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CRegisterTrayFile::OnEnSetfocusEditXDivBuffTbl()
{
    char szVal[10]    = {0,};
    char szRetVal[64] = {0,};
    _itoa_s(m_nTestSiteDiv_X, szVal, sizeof(szVal));

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Buff Table X Division", "100", "1");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_X_DIV_BUFF_TBL1)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CRegisterTrayFile::OnEnSetfocusEditYDivBuffTbl()
{
    char szVal[10]    = {0,};
    char szRetVal[64] = {0,};
    _itoa_s(m_nTestSiteDiv_Y, szVal, sizeof(szVal));

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Buff Table Y Division", "100", "1");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_Y_DIV_BUFF_TBL1)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CRegisterTrayFile::OnEnSetfocusEditTrayThickness()
{
    char szVal[10]    = {0,};
    char szRetVal[64] = {0,};
    sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTrayThickness);

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray Thickness", "100", "0");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_TRAY_THICKNESS)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CRegisterTrayFile::MakeLog(LPCTSTR fmt, ...)
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


void CRegisterTrayFile::OnEnSetfocusEditXpitchTest1Site()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTestSite_XPitch);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test Site X Pitch", "100", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_XPITCH_TEST1_SITE)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CRegisterTrayFile::OnEnSetfocusEditYpitchTest1Site()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTestSite_YPitch);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test Site Y Pitch", "100", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_YPITCH_TEST1_SITE)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CRegisterTrayFile::OnEnSetfocusEditXDivTest1Site()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nTestSiteDiv_X, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test Site X Division", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_X_DIV_TEST1_SITE)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CRegisterTrayFile::OnEnSetfocusEditYDivTest1Site()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nTestSiteDiv_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test Site Y Division", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_Y_DIV_TEST1_SITE)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CRegisterTrayFile::OnEnSetfocusEditXpitchClean1Tbl()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTestClean_XPitch);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test Cleaning X Pitch", "100", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_XPITCH_CLEAN1_TBL)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CRegisterTrayFile::OnEnSetfocusEditYpitchClean1Tbl()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTestClean_YPitch);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test Cleaning Y Pitch", "100", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_YPITCH_CLEAN1_TBL)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CRegisterTrayFile::OnEnSetfocusEditXDivClean1Tbl()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nTestCleanDiv_X, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test Cleaning X Division", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_X_DIV_CLEAN1_TBL)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CRegisterTrayFile::OnEnSetfocusEditYDivClean1Tbl()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nTestCleanDiv_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test Cleaning X Division", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_Y_DIV_CLEAN1_TBL)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CRegisterTrayFile::OnEnSetfocusEditTraypp1LtPitchX()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTrayPPLtPitchX);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray PP Load Table Pitch X", "100", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_TRAYPP1_LT_PITCH_X)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CRegisterTrayFile::OnEnSetfocusEditTraypp1LtPitchY()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTrayPPLtPitchY);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray PP Load Table Pitch Y", "100", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_TRAYPP1_LT_PITCH_Y)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CRegisterTrayFile::OnEnSetfocusEditTestppPitchX()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTestPP_PitchX);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP Pitch X", "100", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_TESTPP_PITCH_X)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CRegisterTrayFile::OnEnSetfocusEditTestppPitchY()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTestPP_PitchY);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP Pitch Y", "100", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_TESTPP_PITCH_Y)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CRegisterTrayFile::OnEnSetfocusEditXpitchTestPpSite()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTestPPSite_XPitch);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP Station Pitch Y", "100", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_XPITCH_TEST_PP_SITE)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CRegisterTrayFile::OnEnSetfocusEditYpitchTestPpSite()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTestPPSite_YPitch);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP Station Pitch Y", "100", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_YPITCH_TEST_PP_SITE)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}
