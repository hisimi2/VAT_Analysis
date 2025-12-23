// CYieldMonitorDlg.cpp : 구현 파일입니다.

#include "YieldMonitorDlg.h"
#include "stdafx.h"
#include "SLT.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CYieldMonitorDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CYieldMonitorDlg, CDialog)

CYieldMonitorDlg::CYieldMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CYieldMonitorDlg::IDD, pParent)
{
	m_bYieldMonitor		   = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_YieldCtrl);
	m_bEventType		   = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Event_Type);
	m_bRetestYield		   = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Retest_Yield);
	m_nSampleQuantity	   = g_DMCont.m_dmHandlerOpt.GN(NDM11_Sample_Quantity);
	m_nPresetCounter	   = g_DMCont.m_dmHandlerOpt.GN(NDM11_Preset_Counter);
	m_dLowYield			   = g_DMCont.m_dmHandlerOpt.GD(DDM11_Low_Yield);
	m_nContinueFailureCnt  = g_DMCont.m_dmHandlerOpt.GN(NDM11_ContinueFail_FailCnt);
    m_bAutoKill            = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoKill);
	m_dVariance            = g_DMCont.m_dmHandlerOpt.GD(DDM11_VarianceYield);
	m_bConsecutive_Fail_AutoKill = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Consecutive_Fail_AutoKill);
	m_bConsecutive_Fail_AutoKill_Station = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Consecutive_Fail_AutoKill_Station);
	m_nContinueFailureCnt_Station = g_DMCont.m_dmHandlerOpt.GN(NDM11_ContinueFail_FailCnt_Station);

	m_n1stFailCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_1ST_FailCnt);
	m_bAuto1stFailKill = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoKill_1st_Fail);
		
}

CYieldMonitorDlg::~CYieldMonitorDlg(void)
{
}
void CYieldMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
 	CDialog::DoDataExchange(pDX);

  	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
  	DDX_Control(pDX, IDC_YEILD_SAVE, m_BtnSave);
 	DDX_Control(pDX, IDC_EDIT_SOCKET_COUNT, m_editFailureCount);
 	DDX_Control(pDX, IDC_EDIT_QA_SAMPLE, m_editQaSampleCount);
 	DDX_Control(pDX, IDC_EDIT_QA_PRESET, m_editQaPresetCount);
 	DDX_Control(pDX, IDC_EDIT_QA_LOW, m_editQaLowYield);
 	DDX_Control(pDX, IDC_RADIO_YEILD_OFF, m_BtnYieldMonitor);
    DDX_Radio(pDX, IDC_RADIO_YEILD_OFF, m_bYieldMonitor);
    DDX_Radio(pDX, IDC_RADIO_AUTOKILL_OFF, m_bAutoKill);
    DDX_Radio(pDX, IDC_RADIO_EVENT_OPCALL, m_bEventType);
   	DDX_Radio(pDX, IDC_RADIO_RE_YEILD_DIS, m_bRetestYield);
	DDX_Text(pDX, IDC_EDIT_SOCKET_COUNT, m_nContinueFailureCnt);
	DDX_Text(pDX, IDC_EDIT_QA_SAMPLE, m_nSampleQuantity);
	DDX_Text(pDX, IDC_EDIT_QA_PRESET, m_nPresetCounter);
	DDX_Text(pDX, IDC_EDIT_QA_LOW, m_dLowYield);
    DDX_Control(pDX, IDC_EDIT_AUTOKILL, m_editAutokill);
	DDX_Control(pDX, IDC_EDIT_QA_VARIANCE, m_editQaVariance);
	DDX_Text(pDX, IDC_EDIT_QA_VARIANCE, m_dVariance);
	DDX_Radio(pDX, IDC_RADIO_AUTOKILL_OFF2, m_bConsecutive_Fail_AutoKill);
	DDX_Radio(pDX, IDC_RADIO_AUTOKILL_OFF_STATION, m_bConsecutive_Fail_AutoKill_Station);
	DDX_Radio(pDX, IDC_RADIO_1ST_FAIL_AUTOKILL_OFF, m_bAuto1stFailKill);

	DDX_Text(pDX, IDC_EDIT_FAILURE_COUNT_STATION, m_nContinueFailureCnt_Station);
	DDX_Control(pDX, IDC_EDIT_FAILURE_COUNT_STATION, m_editFailureCount_Station);
	DDX_Text(pDX, IDC_EDIT_QA_1ST_FAIL, m_n1stFailCnt);
	DDX_Control(pDX, IDC_EDIT_QA_1ST_FAIL, m_edit1stFailCount);
	
}

BEGIN_MESSAGE_MAP(CYieldMonitorDlg, CDialog)
	ON_EN_SETFOCUS(IDC_EDIT_QA_SAMPLE, &CYieldMonitorDlg::OnEnSetfocusEditQaSample)
	ON_EN_SETFOCUS(IDC_EDIT_QA_PRESET, &CYieldMonitorDlg::OnEnSetfocusEditQaPreset)
	ON_EN_SETFOCUS(IDC_EDIT_QA_LOW, &CYieldMonitorDlg::OnEnSetfocusEditQaLow)
	ON_EN_SETFOCUS(IDC_EDIT_SOCKET_COUNT, &CYieldMonitorDlg::OnEnSetfocusEditFailureCount)
	ON_BN_CLICKED(IDC_YEILD_SAVE, &CYieldMonitorDlg::OnBnClickedYeildSave)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_YEILD_OFF, IDC_RADIO_YEILD_ON,OnClickedYeildMonitor)
	ON_EN_SETFOCUS(IDC_EDIT_QA_VARIANCE, &CYieldMonitorDlg::OnEnSetfocusEditQaVariance)
	ON_EN_SETFOCUS(IDC_EDIT_FAILURE_COUNT_STATION, &CYieldMonitorDlg::OnEnSetfocusEditFailureCountStation)
	ON_EN_SETFOCUS(IDC_EDIT_QA_1ST_FAIL, &CYieldMonitorDlg::OnEnSetfocusEditQa1stFail)
END_MESSAGE_MAP()

BOOL CYieldMonitorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	char szTemp[10];

	if(m_bYieldMonitor == 0){ //Yield Mode OnOff
		GetDlgItem(IDC_EDIT_QA_SAMPLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_QA_PRESET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_QA_LOW)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FAILURE_COUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_QA_VARIANCE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FAILURE_COUNT_STATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_QA_1ST_FAIL)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_QA_SAMPLE)->SetWindowTextA("-");
		GetDlgItem(IDC_EDIT_QA_PRESET)->SetWindowTextA("-");
		GetDlgItem(IDC_EDIT_QA_LOW)->SetWindowTextA("-");
		GetDlgItem(IDC_EDIT_QA_VARIANCE)->SetWindowTextA("-");
		GetDlgItem(IDC_EDIT_FAILURE_COUNT)->SetWindowTextA("-");
		GetDlgItem(IDC_EDIT_FAILURE_COUNT_STATION)->SetWindowTextA("-");
		GetDlgItem(IDC_EDIT_QA_1ST_FAIL)->SetWindowTextA("-");

		GetDlgItem(IDC_RADIO_EVENT_OPCALL)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_EVENT_MESSAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RE_YEILD_DIS)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RE_YEILD_EN)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTOKILL_OFF)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTOKILL_ON)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTOKILL_OFF2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTOKILL_ON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTOKILL_OFF_STATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTOKILL_ON_STATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_1ST_FAIL_AUTOKILL_OFF)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_1ST_FAIL_AUTOKILL_ON)->EnableWindow(FALSE);
	}
	else{
		GetDlgItem(IDC_EDIT_QA_SAMPLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_QA_PRESET)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_QA_LOW)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FAILURE_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_QA_VARIANCE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_QA_1ST_FAIL)->EnableWindow(TRUE);


		sprintf_s(szTemp,"%0.2f",m_dLowYield);
		GetDlgItem(IDC_EDIT_QA_LOW)->SetWindowTextA(szTemp);
		sprintf_s(szTemp, "%0.2f", m_dVariance);
		GetDlgItem(IDC_EDIT_QA_VARIANCE)->SetWindowTextA(szTemp);

		GetDlgItem(IDC_RADIO_EVENT_OPCALL)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_EVENT_MESSAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RE_YEILD_DIS)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RE_YEILD_EN)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_AUTOKILL_OFF)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_AUTOKILL_ON)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_AUTOKILL_OFF2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_AUTOKILL_ON2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_AUTOKILL_OFF_STATION)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_AUTOKILL_ON_STATION)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_1ST_FAIL_AUTOKILL_OFF)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_1ST_FAIL_AUTOKILL_ON)->EnableWindow(TRUE);
	}

    if(m_bAutoKill){
        GetDlgItem(IDC_EDIT_AUTOKILL)->EnableWindow(TRUE);
    }else{
        GetDlgItem(IDC_EDIT_AUTOKILL)->EnableWindow(FALSE);
    }

	OnBtnGUI();
	OnEditBoxGUI();

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	GetDlgItem(IDC_STATIC_QA_1ST_FAIL)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_QA_1ST_FAIL)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_SOCKET_ICS7)->ShowWindow(FALSE);
	GetDlgItem(IDC_RADIO_1ST_FAIL_AUTOKILL_OFF)->ShowWindow(FALSE);
	GetDlgItem(IDC_RADIO_1ST_FAIL_AUTOKILL_ON)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_1ST_FAIL)->ShowWindow(FALSE);
		
 	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CYieldMonitorDlg::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);
	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);
}

void CYieldMonitorDlg::OnEditBoxGUI()
{
	m_editFailureCount.SetBackColor(COLOR_WWHITE);
	m_editQaSampleCount.SetBackColor(COLOR_WWHITE);
	m_editQaPresetCount.SetBackColor(COLOR_WWHITE);
	m_editQaLowYield.SetBackColor(COLOR_WWHITE);
    m_editAutokill.SetBackColor(COLOR_WWHITE);
	m_editQaVariance.SetBackColor(COLOR_WWHITE);
	m_editFailureCount_Station.SetBackColor(COLOR_WWHITE);
	m_edit1stFailCount.SetBackColor(COLOR_WWHITE);
	
	m_editFailureCount.SetTextSize(30);
	m_editQaSampleCount.SetTextSize(30);
	m_editQaPresetCount.SetTextSize(30);
	m_editQaLowYield.SetTextSize(30);
    m_editAutokill.SetTextSize(30);
	m_editQaVariance.SetTextSize(30);
	m_editFailureCount_Station.SetTextSize(30);
	m_edit1stFailCount.SetTextSize(30);
}

void CYieldMonitorDlg::OnEnSetfocusEditQaSample()
{
	char szVal[10] = {0,};
	char szRetVal[64] = {0,};

	sprintf_s(szVal, sizeof(szVal), "%d", m_nSampleQuantity);
	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Sample Count", "10000", "1");
	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_QA_SAMPLE)->SetWindowText(szRetVal);
	}
}

void CYieldMonitorDlg::OnEnSetfocusEditQaPreset()
{
	int nRet;
	char szVal[10] = {0,};
	char szRetVal[64] = {0,};

	sprintf_s(szVal, sizeof(szVal), "%d", m_nPresetCounter);
	nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Preset Count", "10000", "1");
	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_QA_PRESET)->SetWindowText(szRetVal);
	}
}

void CYieldMonitorDlg::OnEnSetfocusEditQaLow()
{
	int nRet;
	char szVal[10] = {0,};
	char szRetVal[64] = {0,};

	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dLowYield);
	nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Low Yield", "100", "0");
	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_QA_LOW)->SetWindowText(szRetVal);
	}
}

void CYieldMonitorDlg::OnEnSetfocusEditFailureCount()
{
	int nRet;
	char szVal[10] = {0,};
	char szRetVal[64] = {0,};

	sprintf_s(szVal, sizeof(szVal), "%d", m_nContinueFailureCnt);
	nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Continue Failure Count (Each Site)", "500", "1");
	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_FAILURE_COUNT)->SetWindowText(szRetVal);
	}
}

void CYieldMonitorDlg::OnBnClickedYeildSave()
{
	char szCokName[512] = {0,};
	ST_BASIC_INFO   stBasicData, stBeforeBasicData;
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
	g_COK_FileMgr.LoadBasicInfo(szCokName, stBasicData);

	stBeforeBasicData= stBasicData;

	char szTemp[10];

	int nYieldMonitorOnOff = GetCheckedRadioButton(IDC_RADIO_YEILD_OFF, IDC_RADIO_YEILD_ON);
	if(nYieldMonitorOnOff == IDC_RADIO_YEILD_OFF){
		m_bYieldMonitor = FALSE;	// OFF
	} else{
		m_bYieldMonitor = TRUE;		// ON
	}

	if(m_bYieldMonitor == TRUE){
		UpdateData(TRUE);

		int nEvnetType   = GetCheckedRadioButton(IDC_RADIO_EVENT_OPCALL, IDC_RADIO_EVENT_MESSAGE);
		if(nEvnetType == IDC_RADIO_EVENT_OPCALL){
			m_bEventType = FALSE;	// Handler Stop
		} else{
			m_bEventType = TRUE;	// Warning
		}

		int nRetestYield = GetCheckedRadioButton(IDC_RADIO_RE_YEILD_DIS, IDC_RADIO_RE_YEILD_EN);
		if(nRetestYield == IDC_RADIO_RE_YEILD_DIS){
			m_bRetestYield = FALSE;	// OFF
		} else{
			m_bRetestYield = TRUE;	// ON
		}

		g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_YieldCtrl   , m_bYieldMonitor);
		g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_Event_Type  , m_bEventType);
		g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_Retest_Yield, m_bRetestYield);

        // continue fail
        GetDlgItem(IDC_EDIT_FAILURE_COUNT)->GetWindowText( szTemp, sizeof(szTemp) );
        m_nContinueFailureCnt = atoi(szTemp);
        g_DMCont.m_dmHandlerOpt.SN(NDM11_ContinueFail_FailCnt, m_nContinueFailureCnt);

        // sampl count
        GetDlgItem(IDC_EDIT_QA_SAMPLE)->GetWindowText( szTemp, sizeof(szTemp) );
        m_nSampleQuantity = atoi(szTemp);
        g_DMCont.m_dmHandlerOpt.SN(NDM11_Sample_Quantity, m_nSampleQuantity);

        // preset count
        GetDlgItem(IDC_EDIT_QA_PRESET)->GetWindowText( szTemp, sizeof(szTemp) );
        m_nPresetCounter = atoi(szTemp);
        g_DMCont.m_dmHandlerOpt.SN(NDM11_Preset_Counter, m_nPresetCounter);

        // setting yield
        GetDlgItem(IDC_EDIT_QA_LOW)->GetWindowText( szTemp, sizeof(szTemp) );
        m_dLowYield = atof(szTemp);
        g_DMCont.m_dmHandlerOpt.SD(DDM11_Low_Yield, m_dLowYield);

		// setting variance
		GetDlgItem(IDC_EDIT_QA_VARIANCE)->GetWindowText( szTemp, sizeof(szTemp) );
		m_dVariance = atof(szTemp);
		g_DMCont.m_dmHandlerOpt.SD(DDM11_VarianceYield, m_dVariance);

		// continue fail
		GetDlgItem(IDC_EDIT_FAILURE_COUNT_STATION)->GetWindowText(szTemp, sizeof(szTemp));
		m_nContinueFailureCnt_Station = atoi(szTemp);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_ContinueFail_FailCnt_Station, m_nContinueFailureCnt_Station);

		// 1st Fail count
		GetDlgItem(IDC_EDIT_QA_1ST_FAIL)->GetWindowText(szTemp, sizeof(szTemp));
		m_n1stFailCnt = atoi(szTemp);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_1ST_FailCnt, m_n1stFailCnt);

		stBasicData.bYieldMonitor 		 = m_bYieldMonitor;
		stBasicData.bEventType			 = m_bEventType;
		stBasicData.bRetestYield		 = m_bRetestYield;
		stBasicData.nContinueFailureCnt	 = m_nContinueFailureCnt;
		stBasicData.nSampleQuantity		 = m_nSampleQuantity;
		stBasicData.nPresetCounter		 = m_nPresetCounter;
		stBasicData.dLowYield			 = m_dLowYield;
		stBasicData.dVariance            = m_dVariance;
		stBasicData.nContinueFailureCnt_Station = m_nContinueFailureCnt_Station;
		stBasicData.n1stFailCnt = m_n1stFailCnt;
	}
	else {
		g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_YieldCtrl, m_bYieldMonitor);
		stBasicData.bYieldMonitor = m_bYieldMonitor;

        g_McComm.CmdOnlyOneCmdNoParam(GUI_RCMD_REQ_YIELD_MONITOR_CLEAR);
	}

    // AutoKill (Auto Socket Off)
    int nAutoKill = GetCheckedRadioButton(IDC_RADIO_AUTOKILL_OFF, IDC_RADIO_AUTOKILL_ON);
    if(nAutoKill == IDC_RADIO_AUTOKILL_OFF){ 
        g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_AutoKill, DEF_OFF); 
        stBasicData.bMD_Autokill   = DEF_OFF;
    }
    else{
        g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_AutoKill, DEF_ON);
        stBasicData.bMD_Autokill   = DEF_ON;
    }    

	nAutoKill = GetCheckedRadioButton(IDC_RADIO_AUTOKILL_OFF2, IDC_RADIO_AUTOKILL_ON2);
	if (nAutoKill == IDC_RADIO_AUTOKILL_OFF2) {
		g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_Consecutive_Fail_AutoKill, DEF_OFF);
		stBasicData.bMD_Consecutive_Fail_AutoKill = DEF_OFF;
	}
	else {
		g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_Consecutive_Fail_AutoKill, DEF_ON);
		stBasicData.bMD_Consecutive_Fail_AutoKill = DEF_ON;
	}

	nAutoKill = GetCheckedRadioButton(IDC_RADIO_AUTOKILL_OFF_STATION, IDC_RADIO_AUTOKILL_ON_STATION);
	if (nAutoKill == IDC_RADIO_AUTOKILL_OFF_STATION) {
		g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_Consecutive_Fail_AutoKill_Station, DEF_OFF);
		stBasicData.bMD_Consecutive_Fail_AutoKill_Station = DEF_OFF;
	}
	else {
		g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_Consecutive_Fail_AutoKill_Station, DEF_ON);
		stBasicData.bMD_Consecutive_Fail_AutoKill_Station = DEF_ON;
	}

	int n1stFailAutoKill = GetCheckedRadioButton(IDC_RADIO_1ST_FAIL_AUTOKILL_OFF, IDC_RADIO_1ST_FAIL_AUTOKILL_ON);
	if (n1stFailAutoKill == IDC_RADIO_1ST_FAIL_AUTOKILL_OFF) {
		g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_AutoKill_1st_Fail, DEF_OFF);
		stBasicData.bMD_1stFailAutokill = DEF_OFF;
	}
	else {
		g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_AutoKill_1st_Fail, DEF_ON);
		stBasicData.bMD_1stFailAutokill = DEF_ON;
	}


	g_COK_FileMgr.SaveBasicInfo(szCokName, &stBasicData);

	//GUI Data Change Log
	std::vector<std::pair<std::pair<CString,int>,std::pair<double,double>>> vData;

	stBeforeBasicData.IsSameYieldMonitor(stBasicData,vData);

	if(!(vData.empty()))
	{
		YieldMonitorChangeLog(vData);
	}
}

void CYieldMonitorDlg::OnClickedYeildMonitor(UINT nID)
{
	switch (nID)
	{
	case IDC_RADIO_YEILD_OFF :
		GetDlgItem(IDC_EDIT_QA_SAMPLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_QA_PRESET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_QA_LOW)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_QA_VARIANCE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FAILURE_COUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FAILURE_COUNT_STATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_QA_1ST_FAIL)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_QA_SAMPLE)->SetWindowTextA("-");
		GetDlgItem(IDC_EDIT_QA_PRESET)->SetWindowTextA("-");
		GetDlgItem(IDC_EDIT_QA_LOW)->SetWindowTextA("-");
		GetDlgItem(IDC_EDIT_QA_VARIANCE)->SetWindowTextA("-");
		GetDlgItem(IDC_EDIT_FAILURE_COUNT)->SetWindowTextA("-");
		GetDlgItem(IDC_EDIT_FAILURE_COUNT_STATION)->SetWindowTextA("-");
		GetDlgItem(IDC_EDIT_QA_1ST_FAIL)->SetWindowTextA("-");

		GetDlgItem(IDC_RADIO_EVENT_OPCALL)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_EVENT_MESSAGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RE_YEILD_DIS)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RE_YEILD_EN)->EnableWindow(FALSE);

		GetDlgItem(IDC_RADIO_AUTOKILL_OFF)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTOKILL_ON)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTOKILL_OFF2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTOKILL_ON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTOKILL_OFF_STATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTOKILL_ON_STATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_1ST_FAIL_AUTOKILL_OFF)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_1ST_FAIL_AUTOKILL_ON)->EnableWindow(FALSE);
		break;
	case IDC_RADIO_YEILD_ON :
		GetDlgItem(IDC_EDIT_QA_SAMPLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_QA_PRESET)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_QA_LOW)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_QA_VARIANCE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FAILURE_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FAILURE_COUNT_STATION)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_QA_1ST_FAIL)->EnableWindow(TRUE);

		GetDlgItem(IDC_RADIO_EVENT_OPCALL)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_EVENT_MESSAGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RE_YEILD_DIS)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RE_YEILD_EN)->EnableWindow(TRUE);

		GetDlgItem(IDC_RADIO_AUTOKILL_OFF)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_AUTOKILL_ON)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_AUTOKILL_OFF2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_AUTOKILL_ON2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_AUTOKILL_OFF_STATION)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_AUTOKILL_ON_STATION)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_1ST_FAIL_AUTOKILL_OFF)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_1ST_FAIL_AUTOKILL_ON)->EnableWindow(TRUE);

		// 정수를 문자열로 변환하여 szTemp 변수에 저장 
		char szTemp[10];
		_itoa_s(m_nSampleQuantity, szTemp, 10);
		GetDlgItem(IDC_EDIT_QA_SAMPLE)->SetWindowTextA(szTemp);
		_itoa_s(m_nPresetCounter, szTemp, 10);
		GetDlgItem(IDC_EDIT_QA_PRESET)->SetWindowTextA(szTemp);

		sprintf_s(szTemp,"%0.2f",m_dLowYield);
		GetDlgItem(IDC_EDIT_QA_LOW)->SetWindowTextA(szTemp);

		sprintf_s(szTemp,"%0.2f",m_dVariance);
		GetDlgItem(IDC_EDIT_QA_VARIANCE)->SetWindowTextA(szTemp);

		_itoa_s(m_nContinueFailureCnt, szTemp, 10);
		GetDlgItem(IDC_EDIT_FAILURE_COUNT)->SetWindowTextA(szTemp);

		_itoa_s(m_nContinueFailureCnt_Station, szTemp, 10);
		GetDlgItem(IDC_EDIT_FAILURE_COUNT_STATION)->SetWindowTextA(szTemp);

		_itoa_s(m_n1stFailCnt, szTemp, 10);
		GetDlgItem(IDC_EDIT_QA_1ST_FAIL)->SetWindowTextA(szTemp);
 		break;
	}
}


void CYieldMonitorDlg::MakeLog(LPCTSTR fmt, ...)
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


void CYieldMonitorDlg::YieldMonitorChangeLog(std::vector<std::pair<std::pair<CString,int>,std::pair<double,double>>> vData)
{
	for(int i = 0 ; i< (int)vData.size(); i++)
	{
		std::pair<std::pair<CString,int>,std::pair<double,double>> p;

		p = vData[i];
		if(p.first.second == 1) // Check Box, Radio Button
		{
			if(p.first.first == "Event Type")
			{
				if(p.second.second == 0) 
				{
					MakeLog("[MENU = Yield Monitoring, DATA NAME = %s ][BEFORE=Warning Message Only, AFTER=Operator Call (Handler Stop)]", p.first.first);
				}
				else if(p.second.second == 1)
				{
					MakeLog("[MENU = Yield Monitoring, DATA NAME = %s ][BEFORE=Operator Call (Handler Stop), AFTER=Warning Message Only]", p.first.first);
				}
			}
			else if(p.first.first == "Re-Test Mode Yield Monitoring")
			{
				if(p.second.second == 0)
				{
					MakeLog("[MENU = Yield Monitoring, DATA NAME = %s ][BEFORE=Enable, AFTER=Disable]", p.first.first);
				}
				else if(p.second.second == 1)
				{
					MakeLog("[MENU = Yield Monitoring, DATA NAME = %s ][BEFORE=Disable, AFTER=Enable]", p.first.first);
				}
			}
			else if(p.second.second == 0) //OFF
			{
				MakeLog("[MENU = Yield Monitoring, DATA NAME = %s ][BEFORE=ON, AFTER=OFF]", p.first.first);
			}
			else if(p.second.second == 1)
			{
				MakeLog("[MENU = Yield Monitoring, DATA NAME = %s ][BEFORE=OFF, AFTER=ON]", p.first.first);
			}
		}
		else
		{
			MakeLog("[MENU = Yield Monitoring, DATA NAME = %s ][BEFORE= %.2f, AFTER= %.2f]", p.first.first, p.second.first, p.second.second);
		}
	}
}
	


void CYieldMonitorDlg::OnEnSetfocusEditQaVariance()
{
	int nRet;
	char szVal[10] = {0,};
	char szRetVal[64] = {0,};

	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dVariance);
	nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Variance Yield", "100", "0");
	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_QA_VARIANCE)->SetWindowText(szRetVal);
	}
}


void CYieldMonitorDlg::OnEnSetfocusEditFailureCountStation()
{
	int nRet;
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };

	sprintf_s(szVal, sizeof(szVal), "%d", m_nContinueFailureCnt_Station);
	nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Continue Failure Count (Station)", "500", "1");
	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_FAILURE_COUNT_STATION)->SetWindowText(szRetVal);
	}
}


void CYieldMonitorDlg::OnEnSetfocusEditQa1stFail()
{
	int nRet;
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };

	sprintf_s(szVal, sizeof(szVal), "%d", m_n1stFailCnt);
	nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "1st Fail", "100", "0");
	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_QA_1ST_FAIL)->SetWindowText(szRetVal);
	}
}
