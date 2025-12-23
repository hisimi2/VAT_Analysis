// SPD_TM_FEEDER.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "SPD_TM_FEEDER.h"
#include "afxdialogex.h"


// CSPD_TM_FEEDER 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSPD_TM_FEEDER, CDialogEx)

CSPD_TM_FEEDER::CSPD_TM_FEEDER(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SPD_TM_FEEDER, pParent)
{
	m_nSPD_Feeder_Y = g_DMCont.m_dmFeeder.GN(NDM10_SPD_Feeder_Y);
	m_nACC_Feeder_Y = g_DMCont.m_dmFeeder.GN(NDM10_ACC_Feeder_Y);
	m_nDCC_Feeder_Y = g_DMCont.m_dmFeeder.GN(NDM10_DCC_Feeder_Y);

	m_dFeeder_UpDownWait = (double)g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_UPDN_WAITTIME) / 1000.0;
	m_dFeeder_ClampWait = (double)g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_CLAMP_WAITTIME) / 1000.0;
	m_dFeeder_StepOffset = (double)g_DMCont.m_dmFeeder.GD(DDM10_FEEDER_STEP_OFFSET) / 1000.0;
	m_dFeeder_AlignWait = (double)g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_ALIGN_WAITTIME) / 1000.0;
}

CSPD_TM_FEEDER::~CSPD_TM_FEEDER()
{
}

void CSPD_TM_FEEDER::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BUTTON_SAVE, m_BtnSave);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);

	DDX_Text(pDX, IDC_EDIT_SPD_FEEDER_Y, m_nSPD_Feeder_Y);
	DDX_Text(pDX, IDC_EDIT_ACC_FEEDER_Y, m_nACC_Feeder_Y);
	DDX_Text(pDX, IDC_EDIT_DCC_FEEDER_Y, m_nDCC_Feeder_Y);

	DDX_Control(pDX, IDC_EDIT_SPD_FEEDER_Y, m_editSPD_Feeder_Y);
	DDX_Control(pDX, IDC_EDIT_ACC_FEEDER_Y, m_editACC_Feeder_Y);
	DDX_Control(pDX, IDC_EDIT_DCC_FEEDER_Y, m_editDCC_Feeder_Y);

	DDX_Text(pDX, IDC_EDIT_FEEDER_CYL_UPDOWN_WAIT, m_dFeeder_UpDownWait);
	DDX_Text(pDX, IDC_EDIT_FEEDER_CYL_CLAMP_WAIT, m_dFeeder_ClampWait);
	DDX_Text(pDX, IDC_EDIT_FEEDER_STEP_MOVE_OFFSET, m_dFeeder_StepOffset);
	DDX_Text(pDX, IDC_EDIT_FEEDER_CYL_ALIGN_WAIT, m_dFeeder_AlignWait);

	DDX_Control(pDX, IDC_EDIT_FEEDER_CYL_UPDOWN_WAIT, m_editFeeder_UpDownWait);
	DDX_Control(pDX, IDC_EDIT_FEEDER_CYL_CLAMP_WAIT, m_editFeeder_ClampWait);
	DDX_Control(pDX, IDC_EDIT_FEEDER_STEP_MOVE_OFFSET, m_editFeeder_StepOffset);
	DDX_Control(pDX, IDC_EDIT_FEEDER_CYL_ALIGN_WAIT, m_editFeeder_AlignOffset);
}


BEGIN_MESSAGE_MAP(CSPD_TM_FEEDER, CDialogEx)
	ON_EN_SETFOCUS(IDC_EDIT_FEEDER_CYL_UPDOWN_WAIT, &CSPD_TM_FEEDER::OnEnSetfocusEditFeederCylUpdownWait)
	ON_EN_SETFOCUS(IDC_EDIT_FEEDER_CYL_CLAMP_WAIT, &CSPD_TM_FEEDER::OnEnSetfocusEditFeederCylClampWait)
	ON_EN_SETFOCUS(IDC_EDIT_FEEDER_STEP_MOVE_OFFSET, &CSPD_TM_FEEDER::OnEnSetfocusEditFeederStepMoveOffset)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_FEEDER_Y, &CSPD_TM_FEEDER::OnEnSetfocusEditSpdFeederY)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_FEEDER_Y, &CSPD_TM_FEEDER::OnEnSetfocusEditAccFeederY)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_FEEDER_Y, &CSPD_TM_FEEDER::OnEnSetfocusEditDccFeederY)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CSPD_TM_FEEDER::OnBnClickedButtonSave)
	ON_EN_SETFOCUS(IDC_EDIT_FEEDER_CYL_ALIGN_WAIT, &CSPD_TM_FEEDER::OnEnSetfocusEditFeederCylAlignWait)
END_MESSAGE_MAP()


// CSPD_TM_FEEDER 메시지 처리기입니다.

BOOL CSPD_TM_FEEDER::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnBtnGUI();
	OnEditBoxGUI();

// 	char szCompany[16] = { 0, };
// 	CString strCustomer = _T("");
// 	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
// 	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
// 
// 	int	nRetryCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_LOADING_TABLE_VIBRATOR_RETRY_CNT);
// 	double dDelayTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_LOADING_TABLE_VIBRATOR_DELAY_TIME);
// 	double dWaitTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_LOADING_TABLE_WAIT_TIME);


// 	CString strReCnt, strDelayTime, strWaitTime;
// 	strReCnt.Format("%d", nRetryCnt);
// 	m_EditReCnt.SetWindowText(strReCnt);
// 
// 	strDelayTime.Format("%.2f", dDelayTime);
// 	m_EditDelayTime.SetWindowText(strDelayTime);
// 
// 	strWaitTime.Format("%.1f", dWaitTime);
// 	m_EditWaitTime.SetWindowText(strWaitTime);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSPD_TM_FEEDER::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);
}

void CSPD_TM_FEEDER::OnEditBoxGUI()
{
	m_editSPD_Feeder_Y.SetBackColor(COLOR_WWHITE);
	m_editACC_Feeder_Y.SetBackColor(COLOR_WWHITE);
	m_editDCC_Feeder_Y.SetBackColor(COLOR_WWHITE);
	m_editFeeder_UpDownWait.SetBackColor(COLOR_WWHITE);
	m_editFeeder_ClampWait.SetBackColor(COLOR_WWHITE);
	m_editFeeder_StepOffset.SetBackColor(COLOR_WWHITE);
	m_editFeeder_AlignOffset.SetBackColor(COLOR_WWHITE);

	m_editSPD_Feeder_Y.SetTextSize(30);
	m_editACC_Feeder_Y.SetTextSize(30);
	m_editDCC_Feeder_Y.SetTextSize(30);
	m_editFeeder_UpDownWait.SetTextSize(30);
	m_editFeeder_ClampWait.SetTextSize(30);
	m_editFeeder_StepOffset.SetTextSize(30);
	m_editFeeder_AlignOffset.SetTextSize(30);
}

void CSPD_TM_FEEDER::OnEnSetfocusEditFeederCylUpdownWait()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dFeeder_UpDownWait);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Feeder Up/Down Cylinder Wait Time", "5", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_FEEDER_CYL_UPDOWN_WAIT)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_FEEDER::OnEnSetfocusEditFeederCylClampWait()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dFeeder_ClampWait);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Feeder Clamp Cylinder Wait Time", "5", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_FEEDER_CYL_CLAMP_WAIT)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_FEEDER::OnEnSetfocusEditFeederStepMoveOffset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dFeeder_StepOffset);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Feeder Step Offset", "3.0", "-3.0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_FEEDER_STEP_MOVE_OFFSET)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_FEEDER::OnEnSetfocusEditSpdFeederY()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_Feeder_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Feeder Y Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_FEEDER_Y)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_FEEDER::OnEnSetfocusEditAccFeederY()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_Feeder_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Feeder Y Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_FEEDER_Y)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_FEEDER::OnEnSetfocusEditDccFeederY()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_Feeder_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Feeder Y Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_FEEDER_Y)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}

void CSPD_TM_FEEDER::OnBnClickedButtonSave()
{
	UpdateData(TRUE);

	g_DMCont.m_dmFeeder.SN(NDM10_SPD_Feeder_Y, m_nSPD_Feeder_Y);
	g_DMCont.m_dmFeeder.SN(NDM10_ACC_Feeder_Y, m_nACC_Feeder_Y);
	g_DMCont.m_dmFeeder.SN(NDM10_DCC_Feeder_Y, m_nDCC_Feeder_Y);

	g_DMCont.m_dmFeeder.SN(NDM10_FEEDER_UPDN_WAITTIME, (int)(m_dFeeder_UpDownWait * 1000));
	g_DMCont.m_dmFeeder.SN(NDM10_FEEDER_CLAMP_WAITTIME, (int)(m_dFeeder_ClampWait * 1000));
	g_DMCont.m_dmFeeder.SN(NDM10_FEEDER_ALIGN_WAITTIME, (int)(m_dFeeder_AlignWait * 1000));

	g_DMCont.m_dmFeeder.SD(DDM10_FEEDER_STEP_OFFSET, m_dFeeder_StepOffset * 1000);

	char szCokName[512] = { 0, };
	ST_COK_INFO     stCokData, stBeforeCokData;
	//ST_BASIC_INFO   stBasicData, stBeforeBasicData;
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
	g_COK_FileMgr.LoadCokInfo(szCokName, stCokData);
	//g_COK_FileMgr.LoadBasicInfo(szCokName, stBasicData);

	stBeforeCokData = stCokData;
	//stBeforeBasicData = stBasicData;

	stCokData.aFeeder_SPD.aAxisForm[eAxis_Feeder_Y].nSPD = g_DMCont.m_dmFeeder.GN(NDM10_SPD_Feeder_Y);
	stCokData.aFeeder_SPD.aAxisForm[eAxis_Feeder_Y].nACC = g_DMCont.m_dmFeeder.GN(NDM10_ACC_Feeder_Y);
	stCokData.aFeeder_SPD.aAxisForm[eAxis_Feeder_Y].nDCC = g_DMCont.m_dmFeeder.GN(NDM10_DCC_Feeder_Y);
	
	stCokData.nFeeder_UpDn_WaitTm = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_UPDN_WAITTIME);
	stCokData.nFeeder_Clamp_WaitTm = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_CLAMP_WAITTIME);
	stCokData.dFeeder_StepOffset = g_DMCont.m_dmFeeder.GD(DDM10_FEEDER_STEP_OFFSET);
	stCokData.nFeeder_AlignWait = g_DMCont.m_dmFeeder.GN(NDM10_FEEDER_ALIGN_WAITTIME);

	g_COK_FileMgr.SaveCokInfo(szCokName, &stCokData);

	//GUI Data Change Log
	std::vector<std::pair<CString, std::pair<double, double>>> vData;  // vData.first = stCokData, vData.second.first = BeforeData vData.second.second = AfterData

	stBeforeCokData.IsSameFeeder(stCokData, vData);
// 	stBeforeCokData.IsSameLoadTable(stCokData, vData);
// 	stBeforeBasicData.IsSameLoadTable(stBasicData, vData);

	if (!(vData.empty()))
	{
		FeederSpeedTimeChangeLog(vData);
	}
}

// vData.first = stCokData, vData.second.first = BeforeData vData.second.second = AfterData
void CSPD_TM_FEEDER::FeederSpeedTimeChangeLog(std::vector<std::pair<CString, std::pair<double, double>>> vData)
{
	for (int i = 0; i < (int)vData.size(); i++)
	{
		std::pair<CString, std::pair<double, double>> p;

		p = vData[i];

		MakeLog("[MENU = Feeder Speed & Time, DATA NAME = %s ][BEFORE=%.2f, AFTER=%.2f]", p.first, p.second.first, p.second.second);
	}
}

void CSPD_TM_FEEDER::MakeLog(LPCTSTR fmt, ...)
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

void CSPD_TM_FEEDER::OnEnSetfocusEditFeederCylAlignWait()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dFeeder_AlignWait);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Feeder Align Cylinder Wait Time", "5", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_FEEDER_CYL_ALIGN_WAIT)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}
