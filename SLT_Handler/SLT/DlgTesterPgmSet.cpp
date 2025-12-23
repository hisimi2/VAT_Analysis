// DlgTesterPgmSet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "DlgTesterPgmSet.h"
#include "afxdialogex.h"
#include "SystemData.h"


IMPLEMENT_DYNAMIC(CDlgTesterPgmSet, CDialogEx)

CDlgTesterPgmSet::CDlgTesterPgmSet(CWnd* pParent /*=NULL*/)
: CDialogEx(CDlgTesterPgmSet::IDD, pParent)
{

	m_strPgmName = _T("");		
	m_strBeforePgmName = _T("");
	m_strPassString = _T("");
	m_strFailCodeHeader = _T("");
	m_strStartLine = _T("");
	m_strEndLine = _T("");

	 m_nNewModifyType = 0;
	m_dBootTmOut = 0.0;
	m_dTestTmOut = 0.0;
	m_dSotDelay = 0.0;
	m_dPreCooling = 0.0;
	m_dAC_PowerOnDelay = 0.0;
	m_dAC_PowerOffDelay = 0.0;
	m_dATX_PowerOnDelay = 0.0;
	m_nFailCodeDigital = 0;
	m_dMinPassTm = 0.0;
	m_nBiosBootCnt = 0;
	m_dBiosBootTmOut = 0.0;
	m_nBiosS3Cnt = 0;
	m_dBiosS3TmOut = 0.0;
	m_dBiosS3Delay = 0.0;
	m_nSetTemp = 0;
	m_nGuardBand = 0;
	m_dAGP_Delay = 0.0;
	m_nBaudRate = 0;

	m_bMD_SiteOff_TestTimeOut = FALSE;
	m_bMD_SiteOff_DefaultBin = FALSE;
	m_bMD_Sort_as_DefaultBin = FALSE;

	m_bMD_HangReboot = FALSE;
	m_bMD_BiosBoot = FALSE;
	m_bMD_BiosS3 = FALSE;
	m_bMD_TempMonitor = FALSE;
	m_bMD_ExtentionPwrAgp = FALSE;
	m_bMD_BootTmOut = FALSE;
	m_bMD_PreCooling = FALSE;
	m_bMD_AcPwrOnDly = FALSE;
	m_bMD_AcPwrOff = FALSE;
	m_bMD_AtxPowerOn = FALSE;
	m_bMD_PassStr = FALSE;
	m_bMD_FailCodeHead = FALSE;
	m_bMD_FailCodeDig = FALSE;
	m_bMD_StartLine = FALSE;
	m_bMD_EndLine = FALSE;
	m_bMD_MinPassTm = FALSE;
	m_bMD_BaudRate = FALSE;

	m_dNotTestingTmOut = 0.0;
}

CDlgTesterPgmSet::~CDlgTesterPgmSet()
{
}

void CDlgTesterPgmSet::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BTN_PGM_SAVE, m_BtnSave);
    DDX_Control(pDX, IDCANCEL, m_BtnCancel);
    DDX_Control(pDX, IDC_EDIT_BOOT_TIMEOUT, m_EditBootTmOut);
    DDX_Control(pDX, IDC_EDIT_TEST_TIMEOUT, m_EditTestTmOut);
	DDX_Control(pDX, IDC_EDIT_SOT_DELAY, m_EditSotDelay);
    DDX_Control(pDX, IDC_EDIT_PRE_COOLING, m_EditPreCooling);
    DDX_Control(pDX, IDC_EDIT_AC_POWER_ON_DELAY, m_EditAC_PowerOnDelay);
    DDX_Control(pDX, IDC_EDIT_AC_POWER_OFF_DELAY, m_EditAC_PowerOffDelay);
    DDX_Control(pDX, IDC_EDIT_ATX_POWER_DELAY, m_EditATX_PowerOnDelay);
    DDX_Control(pDX, IDC_EDIT_FAIL_CODE_DIG_NO, m_EditFailCodeDigital);
    DDX_Control(pDX, IDC_EDIT_MINI_PASS_TM, m_EditMinPassTm);
    DDX_Control(pDX, IDC_EDIT_BIOS_BOOT_CNT, m_EditBiosBootCnt);
    DDX_Control(pDX, IDC_EDIT_BIOS_BOOT_TIMEOUT, m_EditBiosBootTmOut);
    DDX_Control(pDX, IDC_EDIT_BIOS_S3_CNT, m_EditBiosS3Cnt);
    DDX_Control(pDX, IDC_EDIT_BIOS_S3_TIMEOUT, m_EditBiosS3TmOut);
    DDX_Control(pDX, IDC_EDIT_BIOS_S3_DELAY, m_EditBiosS3Delay);
    DDX_Control(pDX, IDC_EDIT_TEMP_SETTEMP, m_EditSetTemp);
    DDX_Control(pDX, IDC_EDIT_TMP_GUARD, m_EditGuardBand);
    DDX_Control(pDX, IDC_EDIT_EXT_CARD_DELAY, m_EditAGP_Delay);
	DDX_Control(pDX, IDC_EDIT_NOT_TESTING_TIMEOUT, m_EditNotTestingTmOut);
    DDX_Radio(pDX, IDC_RADIO_BAUD_RATE1, m_nBaudRate);
    DDX_Text(pDX, IDC_EDIT_PGM_NAME, m_strPgmName);
    DDX_Text(pDX, IDC_EDIT_BOOT_TIMEOUT, m_dBootTmOut);
    DDX_Text(pDX, IDC_EDIT_TEST_TIMEOUT, m_dTestTmOut);
    DDX_Text(pDX, IDC_EDIT_PRE_COOLING, m_dPreCooling);
    DDX_Text(pDX, IDC_EDIT_AC_POWER_ON_DELAY, m_dAC_PowerOnDelay);
    DDX_Text(pDX, IDC_EDIT_AC_POWER_OFF_DELAY, m_dAC_PowerOffDelay);
    DDX_Text(pDX, IDC_EDIT_ATX_POWER_DELAY, m_dATX_PowerOnDelay);
    DDX_Text(pDX, IDC_EDIT_FAIL_CODE_DIG_NO, m_nFailCodeDigital);
    DDX_Text(pDX, IDC_EDIT_MINI_PASS_TM, m_dMinPassTm);
    DDX_Text(pDX, IDC_EDIT_BIOS_BOOT_CNT, m_nBiosBootCnt);
    DDX_Text(pDX, IDC_EDIT_BIOS_BOOT_TIMEOUT, m_dBiosBootTmOut);
    DDX_Text(pDX, IDC_EDIT_BIOS_S3_CNT, m_nBiosS3Cnt);
    DDX_Text(pDX, IDC_EDIT_BIOS_S3_TIMEOUT, m_dBiosS3TmOut);
    DDX_Text(pDX, IDC_EDIT_BIOS_S3_DELAY, m_dBiosS3Delay);
    DDX_Text(pDX, IDC_EDIT_TEMP_SETTEMP, m_nSetTemp);
    DDX_Text(pDX, IDC_EDIT_TMP_GUARD, m_nGuardBand);
    DDX_Text(pDX, IDC_EDIT_EXT_CARD_DELAY, m_dAGP_Delay);
    DDX_Text(pDX, IDC_EDIT_PASS_STRING, m_strPassString);
    DDX_Text(pDX, IDC_EDIT_FAIL_CODE_HEADER, m_strFailCodeHeader);
    DDX_Text(pDX, IDC_EDIT_START_LINE, m_strStartLine);
    DDX_Text(pDX, IDC_EDIT_END_LINE, m_strEndLine);

	DDX_Text(pDX, IDC_EDIT_NOT_TESTING_TIMEOUT, m_dNotTestingTmOut);
	

    DDX_Radio(pDX, IDC_RADIO_HANG_REBOOT1, m_bMD_HangReboot);
    DDX_Radio(pDX, IDC_RADIO_BIOS_BOOT1, m_bMD_BiosBoot);
    DDX_Radio(pDX, IDC_RADIO_BIOS_S3_1, m_bMD_BiosS3);
    DDX_Radio(pDX, IDC_RADIO_TMP_MONITOR1, m_bMD_TempMonitor);
    DDX_Radio(pDX, IDC_RADIO_EXT_CARD_OFF, m_bMD_ExtentionPwrAgp);

    DDX_Check(pDX, IDC_CHK_PRE_COOLING, m_bMD_PreCooling      );
    DDX_Check(pDX, IDC_CHK_BOOT, m_bMD_BootTmOut              );     
    DDX_Check(pDX, IDC_CHK_AC_POW_DELAY, m_bMD_AcPwrOnDly     );
    DDX_Check(pDX, IDC_CHK_AC_POW_OFF, m_bMD_AcPwrOff         );
    DDX_Check(pDX, IDC_CHK_ATX_POW_ON, m_bMD_AtxPowerOn       );
    DDX_Check(pDX, IDC_CHK_PASS_STRING, m_bMD_PassStr         );
    DDX_Check(pDX, IDC_CHK_FAIL_HEADER, m_bMD_FailCodeHead    );
    DDX_Check(pDX, IDC_CHK_FAIL_DIGITAL, m_bMD_FailCodeDig    );
    DDX_Check(pDX, IDC_CHK_START_LINE, m_bMD_StartLine        );
    DDX_Check(pDX, IDC_CHK_END_LINE, m_bMD_EndLine            );
    DDX_Check(pDX, IDC_CHK_MIN_PASS_TM, m_bMD_MinPassTm       );
    DDX_Check(pDX, IDC_CHK_BAUD_RATE, m_bMD_BaudRate          );

	DDX_Text(pDX, IDC_EDIT_SOT_DELAY, m_dSotDelay);

	DDX_Check( pDX, IDC_CHECK_TIME_OUT, m_bMD_SiteOff_TestTimeOut );
	DDX_Check( pDX, IDC_CHECK_DEFAULT_BIN, m_bMD_SiteOff_DefaultBin );
	DDX_Radio( pDX, IDC_RADIO_DEFAULT_BIN_ON, m_bMD_Sort_as_DefaultBin );
	DDX_Control( pDX, IDC_SPREAD_ERROR_STRING, m_SprdErrorString );

	DDX_Control( pDX, IDC_BTN_DEFAULT_INSERT, m_BtnInsert );
	DDX_Control( pDX, IDC_BTN_DEFAULT_DELETE, m_BtnDelete );
}


BEGIN_MESSAGE_MAP(CDlgTesterPgmSet, CDialogEx)
    ON_EN_SETFOCUS(IDC_EDIT_BOOT_TIMEOUT, &CDlgTesterPgmSet::OnEnSetfocusEditBootTimeout)
    ON_EN_SETFOCUS(IDC_EDIT_TEST_TIMEOUT, &CDlgTesterPgmSet::OnEnSetfocusEditTestTimeOut)
    ON_EN_SETFOCUS(IDC_EDIT_PRE_COOLING, &CDlgTesterPgmSet::OnEnSetfocusEditPreCooling)
	ON_EN_SETFOCUS(IDC_EDIT_NOT_TESTING_TIMEOUT, &CDlgTesterPgmSet::OnEnSetfocusEditNotTestingTimeout)
    ON_EN_SETFOCUS(IDC_EDIT_AC_POWER_ON_DELAY, &CDlgTesterPgmSet::OnEnSetfocusEditAcPowerOnDelay)
    ON_EN_SETFOCUS(IDC_EDIT_AC_POWER_OFF_DELAY, &CDlgTesterPgmSet::OnEnSetfocusEditAcPowerOffDelay)
    ON_EN_SETFOCUS(IDC_EDIT_ATX_POWER_DELAY, &CDlgTesterPgmSet::OnEnSetfocusEditAtxPowerDelay)
    ON_EN_SETFOCUS(IDC_EDIT_FAIL_CODE_DIG_NO, &CDlgTesterPgmSet::OnEnSetfocusEditFailCodeDigNo)
    ON_EN_SETFOCUS(IDC_EDIT_MINI_PASS_TM, &CDlgTesterPgmSet::OnEnSetfocusEditMiniPassTm)
    ON_EN_SETFOCUS(IDC_EDIT_BIOS_BOOT_CNT, &CDlgTesterPgmSet::OnEnSetfocusEditBiosBootCnt)
    ON_EN_SETFOCUS(IDC_EDIT_BIOS_BOOT_TIMEOUT, &CDlgTesterPgmSet::OnEnSetfocusEditBiosBootTimeout)
    ON_EN_SETFOCUS(IDC_EDIT_BIOS_S3_CNT, &CDlgTesterPgmSet::OnEnSetfocusEditBiosS3Cnt)
    ON_EN_SETFOCUS(IDC_EDIT_BIOS_S3_TIMEOUT, &CDlgTesterPgmSet::OnEnSetfocusEditBiosS3Timeout)
    ON_EN_SETFOCUS(IDC_EDIT_BIOS_S3_DELAY, &CDlgTesterPgmSet::OnEnSetfocusEditBiosS3Delay)
    ON_EN_SETFOCUS(IDC_EDIT_TEMP_SETTEMP, &CDlgTesterPgmSet::OnEnSetfocusEditTempSettemp)
    ON_EN_SETFOCUS(IDC_EDIT_TMP_GUARD, &CDlgTesterPgmSet::OnEnSetfocusEditTmpGuard)
    ON_EN_SETFOCUS(IDC_EDIT_EXT_CARD_DELAY, &CDlgTesterPgmSet::OnEnSetfocusEditExtCardDelay)
    ON_BN_CLICKED(IDC_BTN_PGM_SAVE, &CDlgTesterPgmSet::OnBnClickedBtnPgmSave)
	ON_EN_SETFOCUS(IDC_EDIT_SOT_DELAY, &CDlgTesterPgmSet::OnEnSetfocusEditSotDelay)
	ON_BN_CLICKED( IDC_BTN_DEFAULT_INSERT, &CDlgTesterPgmSet::OnBnClickedBtnDefaultInsert )
	ON_BN_CLICKED( IDC_BTN_DEFAULT_DELETE, &CDlgTesterPgmSet::OnBnClickedBtnDefaultDelete )

END_MESSAGE_MAP()


// CDlgTesterPgmSet 메시지 처리기입니다.


BOOL CDlgTesterPgmSet::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    if(m_nNewModifyType == eSltPgm_ItemModify){
        OnUpdateData();
    }else if(m_nNewModifyType == eSltPgm_ItemNew){
        m_dAC_PowerOffDelay   = 0;
        m_dAC_PowerOnDelay    = 0;
        m_dATX_PowerOnDelay   = 0;
        m_dBootTmOut          = 0;
        m_nFailCodeDigital    = 0;
        m_dTestTmOut          = 0;
		m_dSotDelay           = 0;
        m_dPreCooling         = 0;
        m_dMinPassTm          = 0;
        m_nBiosBootCnt        = 0;
        m_dBiosBootTmOut      = 0;
        m_nBiosS3Cnt          = 0;
        m_dBiosS3TmOut        = 0;
        m_dBiosS3Delay        = 0;
        m_nSetTemp            = 0;
        m_nGuardBand          = 0;
        m_dAGP_Delay          = 0;
        m_nBaudRate           = 0;
        m_bMD_BootTmOut       = FALSE;
        m_bMD_PreCooling      = FALSE;
        m_bMD_AcPwrOnDly      = FALSE;
        m_bMD_AcPwrOff        = FALSE;
        m_bMD_AtxPowerOn      = FALSE;
        m_bMD_PassStr         = FALSE;
        m_bMD_FailCodeHead    = FALSE;
        m_bMD_FailCodeDig     = FALSE;
        m_bMD_StartLine       = FALSE;
        m_bMD_EndLine         = FALSE;
        m_bMD_MinPassTm       = FALSE;
        m_bMD_BaudRate        = FALSE;     
        m_bMD_HangReboot      = FALSE;
        m_bMD_BiosBoot        = FALSE;
        m_bMD_BiosS3          = FALSE; 
        m_bMD_TempMonitor     = FALSE;
        m_bMD_ExtentionPwrAgp = FALSE;
        m_strFailCodeHeader   = _T("");
        m_strPassString       = _T("");
        m_strStartLine        = _T("");
        m_strEndLine          = _T("");

		m_bMD_SiteOff_TestTimeOut = FALSE;
		m_bMD_SiteOff_DefaultBin = FALSE;
		m_bMD_Sort_as_DefaultBin = /*FALSE*/TRUE;

		m_SprdErrorString.SetMaxCols( 1 );
		m_SprdErrorString.SetMaxRows( 1 );

		m_dNotTestingTmOut = 0.0;

        UpdateData(FALSE);
    }

    OnBtnGUI();
    OnEditBoxGUI();

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	
		GetDlgItem(IDC_STATIC_NOT_TESTING_TIMEOUT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_NOT_TESTING_TIMEOUT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_SEC16)->ShowWindow(SW_HIDE);
	

    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgTesterPgmSet::OnUpdateData()
{
    ST_SLT_PGM stPgmType;
    g_COK_FileMgr.LoadSltPgm( ((LPSTR)(LPCTSTR)m_strPgmName) , stPgmType);

    m_dAC_PowerOffDelay  = (double)stPgmType.nAcPowerOffDelayTm/1000.0;
    m_dAC_PowerOnDelay   = (double)stPgmType.nAcPowerOnDelayTm /1000.0;
    m_dATX_PowerOnDelay  = (double)stPgmType.nAtxPowerOnDelayTm/1000.0;
    m_dBootTmOut         = (double)stPgmType.nBootTmOut        /1000.0;
    m_nFailCodeDigital   = stPgmType.nFailCodeDigitalNo            ;
    m_dTestTmOut         = (double)stPgmType.nTestTmOut        /1000.0;
	m_dSotDelay          = (double)stPgmType.nSotDelayTm       /1000.0;
    m_dPreCooling        = (double)stPgmType.nPreCoolingTm     /1000.0;
    m_dMinPassTm         = (double)stPgmType.nMinPassTm        /1000.0;
    m_nBiosBootCnt       = stPgmType.nBiosBootCnt                  ;
    m_dBiosBootTmOut     = (double)stPgmType.nBiosTmOut        /1000.0;
    m_nBiosS3Cnt         = stPgmType.nBiosS3Cnt                    ;
    m_dBiosS3TmOut       = (double)stPgmType.nBiosS3TmOut      /1000.0;
    m_dBiosS3Delay       = (double)stPgmType.nBiosS3DelayTm    /1000.0;
    m_nSetTemp           = stPgmType.nSetTemp                      ;
    m_nGuardBand         = stPgmType.nTempGuardBand                ;
    m_dAGP_Delay         = (double)stPgmType.nAgpDelayTm       /1000.0;
    m_nBaudRate          = stPgmType.bMD_BaudRate                  ; 

	m_dNotTestingTmOut = (double)stPgmType.nNotTestingTmOut / 1000.0;

    m_bMD_BootTmOut       = stPgmType.bMD_BootTmOut	     ;
    m_bMD_PreCooling      = stPgmType.bMD_PreCooling     ;
    m_bMD_AcPwrOnDly      = stPgmType.bMD_AcPwrOnDly     ;
    m_bMD_AcPwrOff        = stPgmType.bMD_AcPwrOff       ;
    m_bMD_AtxPowerOn      = stPgmType.bMD_AtxPowerOn     ;
    m_bMD_PassStr         = stPgmType.bMD_PassStr        ;
    m_bMD_FailCodeHead    = stPgmType.bMD_FailCodeHead   ;
    m_bMD_FailCodeDig     = stPgmType.bMD_FailCodeDig    ;
    m_bMD_StartLine       = stPgmType.bMD_StartLine      ;
    m_bMD_EndLine         = stPgmType.bMD_EndLine        ;
    m_bMD_MinPassTm       = stPgmType.bMD_MinPassTm      ;
    m_bMD_BaudRate        = stPgmType.bMD_BaudRate       ;     
    m_bMD_HangReboot      = stPgmType.bMD_HangReboot     ;
    m_bMD_BiosBoot        = stPgmType.bMD_BiosBoot       ;
    m_bMD_BiosS3          = stPgmType.bMD_BiosS3         ; 
    m_bMD_TempMonitor     = stPgmType.bMD_TempMonitor    ;
    m_bMD_ExtentionPwrAgp = stPgmType.bMD_ExtentionPwrAGP;
    
	m_bMD_SiteOff_TestTimeOut = stPgmType.bMD_SiteOffTestTimeOut;
	m_bMD_SiteOff_DefaultBin = stPgmType.bMD_SiteOffDefaultBin;
	m_bMD_Sort_as_DefaultBin = /*stPgmType.bMD_Sort_as_DefaultBin*/TRUE;

    m_strFailCodeHeader.Format("%s", stPgmType.szFailCodeHeader);
    m_strPassString.Format("%s", stPgmType.szPassString);
    m_strStartLine.Format("%s", stPgmType.szStartLine);
    m_strEndLine.Format("%s", stPgmType.szEndLine);

    m_stBeforePgmType = stPgmType;
	m_strBeforePgmName = m_strPgmName;
	
	UpdateSpread();
	DisableControl( m_strPgmName );
    UpdateData(FALSE);
}


BOOL CDlgTesterPgmSet::PreTranslateMessage(MSG* pMsg)
{
    if( pMsg->message == WM_KEYDOWN ){
        if( pMsg->wParam == VK_ESCAPE ){
            return TRUE;
        }
    }

    return CDialogEx::PreTranslateMessage(pMsg);
}

void CDlgTesterPgmSet::OnBtnGUI()
{
    m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
    m_BtnCancel.SetIcon(IDI_BTN_EXIT);
    m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

    m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
    m_BtnSave.SetIcon(IDI_BTN_SAVE);
    m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnInsert.SetShade( CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB( 55, 55, 255 ) );
	m_BtnInsert.SetAlign( CButtonST::ST_ALIGN_VERT );

	m_BtnDelete.SetShade( CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB( 55, 55, 255 ) );
	m_BtnDelete.SetAlign( CButtonST::ST_ALIGN_VERT );
}

void CDlgTesterPgmSet::OnEditBoxGUI()
{
    m_EditBootTmOut.SetBackColor(COLOR_WWHITE);
    m_EditTestTmOut.SetBackColor(COLOR_WWHITE);
    m_EditPreCooling.SetBackColor(COLOR_WWHITE);
    m_EditAC_PowerOnDelay.SetBackColor(COLOR_WWHITE);
    m_EditAC_PowerOffDelay.SetBackColor(COLOR_WWHITE);
    m_EditATX_PowerOnDelay.SetBackColor(COLOR_WWHITE);
    m_EditFailCodeDigital.SetBackColor(COLOR_WWHITE);
    m_EditMinPassTm.SetBackColor(COLOR_WWHITE);
    m_EditBiosBootCnt.SetBackColor(COLOR_WWHITE);
    m_EditBiosBootTmOut.SetBackColor(COLOR_WWHITE);
    m_EditBiosS3Cnt.SetBackColor(COLOR_WWHITE);
    m_EditBiosS3TmOut.SetBackColor(COLOR_WWHITE);
    m_EditBiosS3Delay.SetBackColor(COLOR_WWHITE);
    m_EditSetTemp.SetBackColor(COLOR_WWHITE);
    m_EditGuardBand.SetBackColor(COLOR_WWHITE);
    m_EditAGP_Delay.SetBackColor(COLOR_WWHITE);
	m_EditSotDelay.SetBackColor(COLOR_WWHITE);
	m_EditNotTestingTmOut.SetBackColor(COLOR_WWHITE);

    m_EditBootTmOut.SetTextSize(30);
    m_EditTestTmOut.SetTextSize(30);
    m_EditPreCooling.SetTextSize(30);
    m_EditAC_PowerOnDelay.SetTextSize(30);
    m_EditAC_PowerOffDelay.SetTextSize(30);
    m_EditATX_PowerOnDelay.SetTextSize(30);
    m_EditFailCodeDigital.SetTextSize(30);
    m_EditMinPassTm.SetTextSize(30);
    m_EditBiosBootCnt.SetTextSize(30);
    m_EditBiosBootTmOut.SetTextSize(30);
    m_EditBiosS3Cnt.SetTextSize(30);
    m_EditBiosS3TmOut.SetTextSize(30);
    m_EditBiosS3Delay.SetTextSize(30);
    m_EditSetTemp.SetTextSize(30);
    m_EditGuardBand.SetTextSize(30);
    m_EditAGP_Delay.SetTextSize(30);
	m_EditSotDelay.SetTextSize(30);
	m_EditNotTestingTmOut.SetTextSize(30);
}

void CDlgTesterPgmSet::OnEnSetfocusEditBootTimeout()
{
    char szVal[10]    = {0,};
    char szRetVal[64] = {0,};
    sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dBootTmOut);

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Boot Time out", "99999", "0");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_BOOT_TIMEOUT)->SetWindowText(szRetVal);
    }
}


void CDlgTesterPgmSet::OnEnSetfocusEditTestTimeOut()
{
    char szVal[10]    = {0,};
    char szRetVal[64] = {0,};
    sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTestTmOut);

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test Time out", "99999", "0");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_TEST_TIMEOUT)->SetWindowText(szRetVal);
    }
}


void CDlgTesterPgmSet::OnEnSetfocusEditPreCooling()
{
    char szVal[10]    = {0,};
    char szRetVal[64] = {0,};
    sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dPreCooling);

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Pre Cooling time", "99999", "0");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_PRE_COOLING)->SetWindowText(szRetVal);
    }
}


void CDlgTesterPgmSet::OnEnSetfocusEditAcPowerOnDelay()
{
    char szVal[10]    = {0,};
    char szRetVal[64] = {0,};
    sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dAC_PowerOnDelay);

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "AC Power on delay", "99999", "0");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_AC_POWER_ON_DELAY)->SetWindowText(szRetVal);
    }
}


void CDlgTesterPgmSet::OnEnSetfocusEditAcPowerOffDelay()
{
    char szVal[10]    = {0,};
    char szRetVal[64] = {0,};
    sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dAC_PowerOffDelay);

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "AC Power off delay", "99999", "0");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_AC_POWER_OFF_DELAY)->SetWindowText(szRetVal);
    }
}


void CDlgTesterPgmSet::OnEnSetfocusEditAtxPowerDelay()
{
    char szVal[10]    = {0,};
    char szRetVal[64] = {0,};
    sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dATX_PowerOnDelay);

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "ATX Power on delay", "99999", "0");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_ATX_POWER_DELAY)->SetWindowText(szRetVal);
    }
}


void CDlgTesterPgmSet::OnEnSetfocusEditFailCodeDigNo()
{
    char szVal[10] = {0,};
    char szRetVal[64] = {0,};
    _itoa_s(m_nFailCodeDigital, szVal, sizeof(szVal));

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Fail Code Digital No", "99999", "0");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_FAIL_CODE_DIG_NO)->SetWindowText(szRetVal);
    }

}


void CDlgTesterPgmSet::OnEnSetfocusEditMiniPassTm()
{
    char szVal[10]    = {0,};
    char szRetVal[64] = {0,};
    sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dMinPassTm);

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Minimum Pass Time", "99999", "0");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_MINI_PASS_TM)->SetWindowText(szRetVal);
    }
}


void CDlgTesterPgmSet::OnEnSetfocusEditBiosBootCnt()
{
    char szVal[10] = {0,};
    char szRetVal[64] = {0,};
    _itoa_s(m_nBiosBootCnt, szVal, sizeof(szVal));

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "BIOS Boot Count", "10000", "0");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_BIOS_BOOT_CNT)->SetWindowText(szRetVal);
    }
}


void CDlgTesterPgmSet::OnEnSetfocusEditBiosBootTimeout()
{
    char szVal[10]    = {0,};
    char szRetVal[64] = {0,};
    sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dBiosBootTmOut);

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "BIOS Boot Time Out", "99999", "0");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_BIOS_BOOT_TIMEOUT)->SetWindowText(szRetVal);
    }
}

void CDlgTesterPgmSet::OnEnSetfocusEditBiosS3Cnt()
{
    char szVal[10] = {0,};
    char szRetVal[64] = {0,};
    _itoa_s(m_nBiosS3Cnt, szVal, sizeof(szVal));

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "BIOS S3 Count", "10000", "0");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_BIOS_S3_CNT)->SetWindowText(szRetVal);
    }
}


void CDlgTesterPgmSet::OnEnSetfocusEditBiosS3Timeout()
{
    char szVal[10]    = {0,};
    char szRetVal[64] = {0,};
    sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dBiosS3TmOut);

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "BIOS S3 Time out", "99999", "0");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_BIOS_S3_TIMEOUT)->SetWindowText(szRetVal);
    }
}


void CDlgTesterPgmSet::OnEnSetfocusEditBiosS3Delay()
{
    char szVal[10]    = {0,};
    char szRetVal[64] = {0,};
    sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dBiosS3Delay);

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "BIOS S3 delay time", "99999", "0");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_BIOS_S3_DELAY)->SetWindowText(szRetVal);
    }
}


void CDlgTesterPgmSet::OnEnSetfocusEditTempSettemp()
{
    char szVal[10] = {0,};
    char szRetVal[64] = {0,};
    _itoa_s(m_nSetTemp, szVal, sizeof(szVal));

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Temp Setting degree", "125", "0");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_TEMP_SETTEMP)->SetWindowText(szRetVal);
    }
}


void CDlgTesterPgmSet::OnEnSetfocusEditTmpGuard()
{
    char szVal[10] = {0,};
    char szRetVal[64] = {0,};
    _itoa_s(m_nGuardBand, szVal, sizeof(szVal));

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Temp guard band", "10", "0");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_TMP_GUARD)->SetWindowText(szRetVal);
    }
}


void CDlgTesterPgmSet::OnEnSetfocusEditExtCardDelay()
{
    char szVal[10]    = {0,};
    char szRetVal[64] = {0,};
    sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dAGP_Delay);

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "AGP Extention card delay", "99999", "0");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_EXT_CARD_DELAY)->SetWindowText(szRetVal);
    }
}



void CDlgTesterPgmSet::OnBnClickedBtnPgmSave()
{
    UpdateData(TRUE);

    ST_SLT_PGM stPgmType;

    char szPgmName[256] = {0,};
    sprintf_s(szPgmName, sizeof(szPgmName), m_strPgmName);

    stPgmType.nAcPowerOffDelayTm  = (int)(m_dAC_PowerOffDelay*1000);
    stPgmType.nAcPowerOnDelayTm   = (int)(m_dAC_PowerOnDelay*1000);
    stPgmType.nAtxPowerOnDelayTm  = (int)(m_dATX_PowerOnDelay*1000);
    stPgmType.nBootTmOut          = (int)(m_dBootTmOut*1000);
    stPgmType.nFailCodeDigitalNo  = m_nFailCodeDigital;
    stPgmType.nTestTmOut          = (int)(m_dTestTmOut*1000);
	stPgmType.nSotDelayTm         = (int)(m_dSotDelay*1000);
    stPgmType.nPreCoolingTm       = (int)(m_dPreCooling*1000);
    stPgmType.nMinPassTm          = (int)(m_dMinPassTm*1000);
    stPgmType.nBiosBootCnt        = m_nBiosBootCnt;
    stPgmType.nBiosTmOut          = (int)(m_dBiosBootTmOut*1000);
    stPgmType.nBiosS3Cnt          = m_nBiosS3Cnt;
    stPgmType.nBiosS3TmOut        = (int)(m_dBiosS3TmOut*1000);
    stPgmType.nBiosS3DelayTm      = (int)(m_dBiosS3Delay*1000);
    stPgmType.nSetTemp            = m_nSetTemp;
    stPgmType.nTempGuardBand      = m_nGuardBand;
    stPgmType.nAgpDelayTm         = (int)(m_dAGP_Delay*1000);
    stPgmType.nBaudRate           = m_nBaudRate;
	stPgmType.nNotTestingTmOut = (int)(m_dNotTestingTmOut * 1000);

	
    stPgmType.bMD_BootTmOut	      = m_bMD_BootTmOut      ;
    stPgmType.bMD_PreCooling      = m_bMD_PreCooling     ;
    stPgmType.bMD_AcPwrOnDly      = m_bMD_AcPwrOnDly     ;
    stPgmType.bMD_AcPwrOff        = m_bMD_AcPwrOff       ;
    stPgmType.bMD_AtxPowerOn      = m_bMD_AtxPowerOn     ;
    stPgmType.bMD_PassStr         = m_bMD_PassStr        ;
    stPgmType.bMD_FailCodeHead    = m_bMD_FailCodeHead   ;
    stPgmType.bMD_FailCodeDig     = m_bMD_FailCodeDig    ;
    stPgmType.bMD_StartLine       = m_bMD_StartLine      ;
    stPgmType.bMD_EndLine         = m_bMD_EndLine        ;
    stPgmType.bMD_MinPassTm       = m_bMD_MinPassTm      ;
    stPgmType.bMD_BaudRate        = m_bMD_BaudRate       ;     
    stPgmType.bMD_HangReboot      = m_bMD_HangReboot     ;
    stPgmType.bMD_BiosBoot        = m_bMD_BiosBoot       ;
    stPgmType.bMD_BiosS3          = m_bMD_BiosS3         ; 
    stPgmType.bMD_TempMonitor     = m_bMD_TempMonitor    ;
    stPgmType.bMD_ExtentionPwrAGP = m_bMD_ExtentionPwrAgp;

	stPgmType.bMD_SiteOffTestTimeOut = m_bMD_SiteOff_TestTimeOut;
	stPgmType.bMD_SiteOffDefaultBin = m_bMD_SiteOff_DefaultBin;
	stPgmType.bMD_Sort_as_DefaultBin = m_bMD_Sort_as_DefaultBin;

    sprintf_s(stPgmType.szFailCodeHeader, sizeof(stPgmType.szFailCodeHeader), m_strFailCodeHeader);
    sprintf_s(stPgmType.szPassString    , sizeof(stPgmType.szPassString)    , m_strPassString);
    sprintf_s(stPgmType.szStartLine     , sizeof(stPgmType.szStartLine)     , m_strStartLine);
    sprintf_s(stPgmType.szEndLine       , sizeof(stPgmType.szEndLine)       , m_strEndLine);

    g_COK_FileMgr.SaveSltPgm(szPgmName, &stPgmType);

    char szData[MAX_PATH] = {0,};
    g_DMCont.m_dmHandlerOpt.GS(SDM11_SLT_ProgramName, szData, sizeof(szData));
    if(strcmp(szData, m_strPgmName) == 0){
        g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_BootTmOut	    , stPgmType.bMD_BootTmOut	   );
        g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_PreCooling      , stPgmType.bMD_PreCooling     );
        g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_AcPwrOnDly      , stPgmType.bMD_AcPwrOnDly     );
        g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_AcPwrOff        , stPgmType.bMD_AcPwrOff       );
        g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_AtxPowerOn      , stPgmType.bMD_AtxPowerOn     );    
        g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_FailCodeDig     , stPgmType.bMD_FailCodeDig    );    
        g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_MinPassTm       , stPgmType.bMD_MinPassTm      );
        g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_BaudRate        , stPgmType.bMD_BaudRate       );
        g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_HangReboot      , stPgmType.bMD_HangReboot     );
        g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_BiosBoot        , stPgmType.bMD_BiosBoot       );
        g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_BiosS3          , stPgmType.bMD_BiosS3         );
        g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_TempMonitor     , stPgmType.bMD_TempMonitor    );
        g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_ExtentionPwrAGP , stPgmType.bMD_ExtentionPwrAGP);
        g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_PassStr         , stPgmType.bMD_PassStr        );
        g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_FailCodeHead    , stPgmType.bMD_FailCodeHead   );
        g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_StartLine       , stPgmType.bMD_StartLine      );
        g_DMCont.m_dmHandlerOpt.SB(BDM11_SltPgm_EndLine         , stPgmType.bMD_EndLine        );
		
		g_DMCont.m_dmHandlerOpt.SB( BDM11_SltPgm_SiteOff_TestTimeOut, stPgmType.bMD_SiteOffTestTimeOut );
		g_DMCont.m_dmHandlerOpt.SB( BDM11_SltPgm_SiteOff_DefaultBin, stPgmType.bMD_SiteOffDefaultBin );
		g_DMCont.m_dmHandlerOpt.SB( BDM11_SltPgm_Sort_as_DefaultBin, /*stPgmType.bMD_Sort_as_DefaultBin */TRUE);

		g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_SotDelayTm        , stPgmType.nSotDelayTm	      );
        g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_TestTmOut         , stPgmType.nTestTmOut	      );
        g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_FailCodeDigitalNo , stPgmType.nFailCodeDigitalNo  );
        g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_PreCooling        , stPgmType.nPreCoolingTm       );
        g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_AcPowerOn         , stPgmType.nAcPowerOnDelayTm   );
        g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_AcPowerOff        , stPgmType.nAcPowerOffDelayTm  );    
        g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_AtxPowerOn        , stPgmType.nAtxPowerOnDelayTm  );    
        g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_MinPassTm         , stPgmType.nMinPassTm          );
        g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_BaudRate          , stPgmType.nBaudRate           );
        g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_BiosBootCnt       , stPgmType.nBiosBootCnt        );
        g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_BiosTmOut         , stPgmType.nBiosTmOut          );
        g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_BiosS3Cnt         , stPgmType.nBiosS3Cnt          );
        g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_S3TmOut           , stPgmType.nBiosS3TmOut        );
        g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_BiosS3DelayTm     , stPgmType.nBiosS3DelayTm      );
        g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_SetTemp           , stPgmType.nSetTemp            );
        g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_TempGuardBand     , stPgmType.nTempGuardBand      );
        g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_AgpDelayTm        , stPgmType.nAgpDelayTm         );
		g_DMCont.m_dmHandlerOpt.SN(NDM11_SltPgm_NotTestingTmOut, stPgmType.nNotTestingTmOut);
		

        g_DMCont.m_dmHandlerOpt.SS(SDM11_SltPgm_PassStr	         , stPgmType.szPassString    , strlen(stPgmType.szPassString));
        g_DMCont.m_dmHandlerOpt.SS(SDM11_SltPgm_FailCodeHeader	 , stPgmType.szFailCodeHeader, strlen(stPgmType.szFailCodeHeader));
        g_DMCont.m_dmHandlerOpt.SS(SDM11_SltPgm_StartLine	     , stPgmType.szStartLine     , strlen(stPgmType.szStartLine));
        g_DMCont.m_dmHandlerOpt.SS(SDM11_SltPgm_EndLine	         , stPgmType.szEndLine       , strlen(stPgmType.szEndLine));
    }
    std::vector<std::pair<std::pair<CString,int>,std::pair<CString,CString>>> vData;

	m_stBeforePgmType.IsSameTestPgmSet(stPgmType, vData);

	if(m_nNewModifyType == eSltPgm_ItemNew)
	{
		MakeLog("[MENU = Tester Program Set, Program Name = %s ][ Create ]",m_strPgmName);
	}
	else if(m_strBeforePgmName != m_strPgmName)
	{
		MakeLog("[MENU = Tester Program Set, Program Name = %s, change to %s ]", m_strBeforePgmName, m_strPgmName);
	}
	if(!(vData.empty()))
	{
		DeviceSetupChangeLog(vData);
	}

	m_stBeforePgmType = stPgmType;
	m_strBeforePgmName = m_strPgmName;

	SaveSpread();
}

void CDlgTesterPgmSet::OnEnSetfocusEditSotDelay()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dSotDelay);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Sot Delay Time", "99999", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_SOT_DELAY)->SetWindowText(szRetVal);
	}
}
void CDlgTesterPgmSet::MakeLog(LPCTSTR fmt, ...)
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


void CDlgTesterPgmSet::DeviceSetupChangeLog(std::vector<std::pair<std::pair<CString,int>,std::pair<CString,CString>>> vData)
{
	for(int i = 0 ; i< (int)vData.size(); i++)
	{
		std::pair<std::pair<CString,int>,std::pair<CString,CString>> p;

		p = vData[i];
		if(p.first.second == 1) // Check Box, Radio Button
		{
			if(p.second.second == "0") //OFF
			{
				MakeLog("[MENU = Tester Program Set, Program Name = %s, DATA NAME = %s ][BEFORE=ON, AFTER=OFF]", m_strPgmName, p.first.first);
			}
			else if(p.second.second == "1")
			{
				MakeLog("[MENU = Tester Program Set, Program Name = %s, DATA NAME = %s ][BEFORE=OFF, AFTER=ON]", m_strPgmName, p.first.first);
			}
		}
		else if(p.first.second == 2)
		{
			if(p.first.first == "Baud Rate")
			{
				int nBeforeBaudRate= 9600, nAfterBaudRate= 9600;

				switch(_ttoi(p.second.first))
				{
				case 0: nBeforeBaudRate = 9600;   break;
				case 1: nBeforeBaudRate = 19200;  break;
				case 2: nBeforeBaudRate = 38400;  break;
				case 3: nBeforeBaudRate = 115200; break;
				}
				switch(_ttoi(p.second.second))
				{
				case 0: nAfterBaudRate = 9600;   break;
				case 1: nAfterBaudRate = 19200;  break;
				case 2: nAfterBaudRate = 38400;  break;
				case 3: nAfterBaudRate = 115200; break;
				}
				MakeLog("[MENU = Tester Program Set, Program Name = %s, DATA NAME = %s ][BEFORE=%d, AFTER=%d]", m_strPgmName, p.first.first, nBeforeBaudRate, nAfterBaudRate);
			}
		}
		else
		{
			MakeLog("[MENU = Tester Program Set, Program Name = %s, DATA NAME = %s ][BEFORE=%s, AFTER=%s]", m_strPgmName, p.first.first, p.second.first, p.second.second);
		}
		
	
	}
}

void CDlgTesterPgmSet::UpdateSpread()
{
	ST_Set_DefaultBin stDefaultBin;

	g_COK_FileMgr.LoadSltPgmDefaultBin( ( ( LPSTR )( LPCTSTR )m_strPgmName ), stDefaultBin );

	int nCount = ( int )stDefaultBin.vDefaultBin.size();
	m_SprdErrorString.SetMaxCols( 1 );
	m_SprdErrorString.SetMaxRows( nCount );

	for( int i = 0; i < nCount; i++ )
	{
		m_SprdErrorString.SetTextMatrix( i + 1, 1, stDefaultBin.vDefaultBin[ i ] );
	}
}

void CDlgTesterPgmSet::SaveSpread()
{
	ST_Set_DefaultBin stDefaultBin;

	int nCnt = m_SprdErrorString.GetMaxRows();
	CString strTmpData = _T( "" );
	for( int i = 0; i < nCnt; i++ )
	{
		strTmpData.Empty();
		strTmpData = m_SprdErrorString.GetTextMatrix( i + 1, 1 );
		strTmpData.Replace( " ", "" );
		if( strTmpData.IsEmpty() != TRUE ) {
			strTmpData.MakeUpper();
			stDefaultBin.vDefaultBin.push_back( strTmpData );
		}
	}

	g_COK_FileMgr.SaveSltPgmDefaultBin( ( ( LPSTR )( LPCTSTR )m_strPgmName ), &stDefaultBin );
	g_McComm.CmdDefaultBinCategoryRefresh();
}

void CDlgTesterPgmSet::OnBnClickedBtnDefaultInsert()
{
	int nMaxRow = m_SprdErrorString.GetMaxRows();
	int nSelectSel = m_SprdErrorString.GetSelBlockRow();
	nMaxRow += 1;
	m_SprdErrorString.InsertRows( nMaxRow, 1 );

	m_SprdErrorString.SetMaxRows( nMaxRow );
	m_SprdErrorString.SetReDraw( TRUE );
}


void CDlgTesterPgmSet::OnBnClickedBtnDefaultDelete()
{
	int nMaxRow = m_SprdErrorString.GetMaxRows();
	nMaxRow -= 1;
	if( nMaxRow < 0 ) {
		nMaxRow = 1;
		return;
	}

	int nSelectSel = m_SprdErrorString.GetSelBlockRow();
	m_SprdErrorString.DeleteRows( nSelectSel, 1 );

	m_SprdErrorString.SetMaxRows( nMaxRow );
	m_SprdErrorString.SetReDraw( TRUE );
}

void CDlgTesterPgmSet::DisableControl( CString strSltPgmName )
{
	char szData[ MAX_PATH ] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS( SDM11_SLT_ProgramName, szData, sizeof( szData ) );
	BOOL bEnAble = TRUE;

	BOOL bLotState = g_DMCont.m_dmEQP.GB( BDM0_SYS_LOT_STATUS );
	if( bLotState == eHandlerLotMode_OnGoing ) {
		if( strSltPgmName.Compare( szData ) == 0 ) {
			bEnAble = FALSE;
		}
	}

	GetDlgItem( IDC_BTN_DEFAULT_INSERT )->EnableWindow( bEnAble );
	//GetDlgItem( IDC_BTN_DEFAULT_DELETE )->EnableWindow( bEnAble );
	GetDlgItem( IDC_CHECK_TIME_OUT )->EnableWindow( bEnAble );
	GetDlgItem( IDC_CHECK_DEFAULT_BIN )->EnableWindow( bEnAble );
	GetDlgItem( IDC_RADIO_DEFAULT_BIN_ON )->EnableWindow( bEnAble );
	GetDlgItem( IDC_RADIO_DEFAULT_BIN_OFF )->EnableWindow( bEnAble );

}

void CDlgTesterPgmSet::OnEnSetfocusEditNotTestingTimeout()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dNotTestingTmOut);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Not Testing Time out", "99999", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_NOT_TESTING_TIMEOUT)->SetWindowText(szRetVal);
	}
}


