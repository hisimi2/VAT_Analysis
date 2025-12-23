// LotInfoDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "LotInfoAmkorDlg.h"
#include "afxdialogex.h"

#include "DEF_LIB_COMMCTRL.h"
#include "SLTView.h"
#include "MainFrm.h"

#include <fstream>

enum eCOPY_LAST_EVENT
{
	eLotNo = 0,
	eSchedule,
	eOperID,
};
// CLotInfoAmkorDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLotInfoAmkorDlg, CDialogEx)

CLotInfoAmkorDlg::CLotInfoAmkorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLotInfoAmkorDlg::IDD, pParent)
{
	m_nLogInLevel = 0;
	m_nFTRCQC = 0;
	m_bLotNormalRC = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_LotStartMode);
	m_bVisionUseNoUseBak =m_bVisionUse = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_VisionUse);
	m_nQaSampleCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_QaSampleCount);
	m_nModeTesterIF = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_TESTER_IF_PRO_AUDIT);

	memset(m_szCompany, 0x00, sizeof(m_szCompany));
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, m_szCompany, sizeof(m_szCompany));

	m_nTheoreticalUPH = 0;
}

CLotInfoAmkorDlg::~CLotInfoAmkorDlg()
{
}

void CLotInfoAmkorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LOT_NO, m_strLotNo);
	DDX_Text(pDX, IDC_EDIT_OPER_ID, m_strOperID);
	DDX_Text(pDX, IDC_EDIT_PROGRAM, m_strPGM_Name);
	DDX_Text(pDX, IDC_EDIT_DEVICE, m_strDeviceName);
	DDX_Text(pDX, IDC_EDIT_CUSTOM, m_strCustomName);
	DDX_Text(pDX, IDC_EDIT_PROCESS, m_strScheduleNo);
	DDX_Text(pDX, IDC_EDIT_HOSTNAME, m_strHostName);
	DDX_Text(pDX, IDC_EDIT_PROCESS_ID, m_strProcessID);
	DDX_Text(pDX, IDC_EDIT_TEMP, m_strTemp);
	DDX_Text(pDX, IDC_EDIT_RETEST_CODE, m_strRetestCode);
	DDX_Text(pDX, IDC_EDIT_QTY, m_strQty);
	DDX_Text(pDX, IDC_EDIT_ART, m_strArt);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_BtnSave);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_BUTTON_GET_INFO, m_BtnGetInfo);
	DDX_Control(pDX, IDC_BUTTON_LOCK, m_BtnLock);

	DDX_Text(pDX, IDC_EDIT_2DID_SORT_PATH, m_str2DIDSortPath);
	DDX_Text(pDX, IDC_EDIT_THEORETICAL, m_strTheoreticalUPH);

	//DDX_Radio(pDX, IDC_RADIO_MODE, m_bLotNormalRC);
	DDX_Radio(pDX, IDC_RADIO_NOT_VISION, m_bVisionUse);
	DDX_Radio(pDX, IDC_RADIO_FT, m_nFTRCQC);

	DDX_Text(pDX, IDC_EDIT_QA_CNT, m_nQaSampleCnt);
	DDX_Control(pDX, IDC_EDIT_QA_CNT, m_editQaSample);

	DDX_Radio(pDX, IDC_RADIO_PRODUCTION, m_nModeTesterIF);
}


BEGIN_MESSAGE_MAP(CLotInfoAmkorDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CLotInfoAmkorDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_RADIO_VISION, &CLotInfoAmkorDlg::OnBnClickedRadioVision)
	ON_BN_CLICKED(IDC_RADIO_NOT_VISION, &CLotInfoAmkorDlg::OnBnClickedRadioNotVision)
	ON_BN_CLICKED(IDC_BUTTON_GET_INFO, &CLotInfoAmkorDlg::OnBnClickedGetInfo)
	ON_BN_CLICKED(IDC_BUTTON_LOCK, &CLotInfoAmkorDlg::OnBnClickedLock)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_FT, IDC_RADIO_AUTO_2DID_SORT_CONTINUE, OnRadioLotModeCtrl)
	ON_EN_SETFOCUS(IDC_EDIT_QA_CNT, &CLotInfoAmkorDlg::OnEnSetfocusEditQaCnt)
	ON_BN_CLICKED(IDC_RADIO_PRODUCTION, &CLotInfoAmkorDlg::OnBnClickedRadioProduction)
	ON_BN_CLICKED(IDC_RADIO_AUDIT, &CLotInfoAmkorDlg::OnBnClickedRadioAudit)
	ON_BN_CLICKED(IDC_RADIO_LOOP, &CLotInfoAmkorDlg::OnBnClickedRadioLoop)
	ON_BN_CLICKED(IDC_RADIO_GRR, &CLotInfoAmkorDlg::OnBnClickedRadioGrr)
	ON_BN_CLICKED(IDC_BUTTON_2DID_SORT_PATH, &CLotInfoAmkorDlg::OnBnClickedButton2didSortPath)
END_MESSAGE_MAP()


// CLotInfoAmkorDlg 메시지 처리기입니다.


BOOL CLotInfoAmkorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();	
	
	OnBtnGUI();				// overide button GUI image 

	CButton* pBtnNotUse = (CButton*)GetDlgItem(IDC_RADIO_NOT_VISION);
	CButton* pBtnUse = (CButton*)GetDlgItem(IDC_RADIO_VISION);
	if (m_bVisionUse == eOPTION_USE)
	{
		pBtnUse->SetCheck(TRUE);
		pBtnNotUse->SetCheck(FALSE);
	}
	else
	{
		pBtnUse->SetCheck(FALSE);
		pBtnNotUse->SetCheck(TRUE);
	}

	pBtnUse = NULL;
	pBtnNotUse = NULL;


	CompanySetting();

	//BOOL bLockState = g_DMCont.m_dmEQP.GB(BDM0_LOT_INFO_IS_LOCK);
	//
	//if(bLockState == TRUE){
	//	LotInfoLock(FALSE);
	//}else{
	//	LotInfoLock(TRUE);
	//}
	
	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
	if(bLotState == eHandlerLotMode_OnGoing){
		GetDlgItem(IDC_RADIO_FT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RC)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_QA)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTO_GRR)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_FT_CONTINUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RC_CONTINUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_QA_CONTINUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTO_GRR_CONTINUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_QA_CNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_EA)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PRODUCTION)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_LOOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_GRR)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTO_LOOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTO_LOOP_CONTINUE)->EnableWindow(FALSE);
		LotInfoLock(FALSE);
	}
	
	if (m_nLogInLevel < eUSERLEVEL_ENGINEER)
	{
		GetDlgItem(IDC_RADIO_AUTO_GRR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_AUTO_GRR_CONTINUE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_AUTO_LOOP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_AUTO_LOOP_CONTINUE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_AUTO_AUDIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_AUTO_AUDIT_CONTINUE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_AUTO_2DID_SORT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_AUTO_2DID_SORT_CONTINUE)->ShowWindow(SW_HIDE);
	}

	
	SetDisplayUserData(FALSE);	// setting display data
	UpdateData(TRUE);

	m_nFTRCQC = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);

	int nUserLevel = m_nLogInLevel;

	if (g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt) == 0 && bLotState != eHandlerLotMode_OnGoing && nUserLevel == eUSERLEVEL_OPERATOR) {
		m_nModeTesterIF = eTesterIF_Mode_Normal;
	}

	UpdateData(FALSE);

	CheckHandlerMode(m_nFTRCQC);

	if (nUserLevel < eUSERLEVEL_TECHNICIAN)
	{
		GetDlgItem(IDC_RADIO_PRODUCTION)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_LOOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_GRR)->EnableWindow(FALSE);

		GetDlgItem(IDC_RADIO_NOT_VISION)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_VISION)->EnableWindow(FALSE);

		GetDlgItem(IDC_RADIO_FT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RC)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_QA)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTO_GRR)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_FT_CONTINUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RC_CONTINUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_QA_CONTINUE)->EnableWindow(FALSE);
	}
	else if (nUserLevel == eUSERLEVEL_TECHNICIAN)
	{
		GetDlgItem(IDC_RADIO_AUDIT)->EnableWindow(FALSE);
	}

	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
	if (nRunMode != eRUN_MODE_ONLINE) {
		GetDlgItem(IDC_RADIO_PRODUCTION)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_LOOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_GRR)->EnableWindow(FALSE);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CLotInfoAmkorDlg::SetDisplayUserData(BOOL bFlag)
{
	char szRetBuff[MAX_PATH]={0,};

	if (bFlag == TRUE)
	{
		//Lot Info Dialog 에있는 data
		GetDlgItemText(IDC_EDIT_LOT_NO, m_strLotNo);
		m_strLotNo.Replace(" ", "");
		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strLotNo);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_LotNo,szRetBuff,sizeof(szRetBuff));

		GetDlgItemText(IDC_EDIT_CUSTOM, m_strCustomName);
		//m_strCustomName.Replace(" ", "");
		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strCustomName);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_CustomerName,szRetBuff,sizeof(szRetBuff));

		GetDlgItemText(IDC_EDIT_DEVICE, m_strDeviceName);
		m_strDeviceName.Replace(" ", "");
		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strDeviceName);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_DeviceName,szRetBuff,sizeof(szRetBuff));

		GetDlgItemText(IDC_EDIT_PROCESS, m_strScheduleNo);
		m_strScheduleNo.Replace(" ", "");
		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strScheduleNo);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_ScheduleNo,szRetBuff,sizeof(szRetBuff));

		GetDlgItemText(IDC_EDIT_RETEST_CODE, m_strRetestCode);
		m_strRetestCode.Replace(" ", "");
		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strRetestCode);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_RetestCode,szRetBuff,sizeof(szRetBuff));

		GetDlgItemText(IDC_EDIT_OPER_ID, m_strOperID);
		m_strOperID.Replace(" ", "");
		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strOperID);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_OperatorID,szRetBuff,sizeof(szRetBuff));

		GetDlgItemText(IDC_EDIT_PROGRAM, m_strPGM_Name);
		m_strPGM_Name.Replace(" ", "");
		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strPGM_Name);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_ProgramName,szRetBuff,sizeof(szRetBuff));

		GetDlgItemText(IDC_EDIT_PROCESS_ID, m_strProcessID);
		m_strProcessID.Replace(" ", "");
		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strProcessID);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_Operation , szRetBuff, MAX_PATH);
		
		GetDlgItemText(IDC_EDIT_TEMP, m_strTemp);
		m_strTemp.Replace(" ", "");
		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strTemp);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_Temp , szRetBuff, MAX_PATH);

		GetDlgItemText(IDC_EDIT_QTY, m_strQty);
		m_strQty.Replace(" ", "");
		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strQty);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_QTY , szRetBuff, MAX_PATH);

		GetDlgItemText(IDC_EDIT_ART, m_strArt);
		m_strArt.Replace(" ", "");
		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strArt);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_ART , szRetBuff, MAX_PATH);

		GetDlgItemText(IDC_EDIT_2DID_SORT_PATH, m_str2DIDSortPath);
		m_str2DIDSortPath.Replace(" ", "");
		sprintf_s(szRetBuff, sizeof(szRetBuff), m_str2DIDSortPath);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_2DID_Sort_FilePath, szRetBuff, MAX_PATH);
			
		m_nTheoreticalUPH = GetDlgItemInt(IDC_EDIT_THEORETICAL);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_Theoretical_UPH, m_nTheoreticalUPH);

		//Lot Info File에 있는 Data
		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strTpRev);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_TP_REV,szRetBuff,sizeof(szRetBuff));
	
		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strSoakTime);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_Soak_Time,szRetBuff,sizeof(szRetBuff));
	
		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strFuse);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_FUSE,szRetBuff,sizeof(szRetBuff));
	
		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strHandler);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_HANDLER,szRetBuff,sizeof(szRetBuff));
	
		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strLoadBdNo);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_LOAD_BD_NO,szRetBuff,sizeof(szRetBuff));
	
		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strSocketId);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_SOCKER_ID,szRetBuff,sizeof(szRetBuff));
	
		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strCKitID);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_C_KIT_ID,szRetBuff,sizeof(szRetBuff));
	
		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strFoundryLotId);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_FOUNDRY_LOTID,szRetBuff,sizeof(szRetBuff));
	
		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strAssemblyStartdate);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_Assembly_startdate,szRetBuff,sizeof(szRetBuff));
	
		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strAssemblyEnddate);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_Assembly_enddate,szRetBuff,sizeof(szRetBuff));

		sprintf_s(szRetBuff,sizeof(szRetBuff),m_StrAssyVendorLotid);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_ASSY_vendor_lotid,szRetBuff,sizeof(szRetBuff));
	
		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strTestVendorLotid);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_TEST_vendor_lotid,szRetBuff,sizeof(szRetBuff));
	
		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strBotSubVendor);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_BOT_SUB_Vendor,szRetBuff,sizeof(szRetBuff));

		sprintf_s(szRetBuff,sizeof(szRetBuff),m_strNickname);
		g_DMCont.m_dmHandlerOpt.SS(SDM11_Nickname,szRetBuff,sizeof(szRetBuff));
	
	}
	else
	{
		g_DMCont.m_dmHandlerOpt.GS(SDM11_LotNo       , szRetBuff, MAX_PATH);
		SetDlgItemText(IDC_EDIT_LOT_NO, szRetBuff);

		g_DMCont.m_dmHandlerOpt.GS(SDM11_CustomerName, szRetBuff, MAX_PATH);
		SetDlgItemText(IDC_EDIT_CUSTOM, szRetBuff);

		g_DMCont.m_dmHandlerOpt.GS(SDM11_DeviceName  , szRetBuff, MAX_PATH);
		m_strDeviceName = szRetBuff;
		SetDlgItemText(IDC_EDIT_DEVICE, szRetBuff);

		g_DMCont.m_dmHandlerOpt.GS(SDM11_ScheduleNo  , szRetBuff, MAX_PATH);
		m_strScheduleNo	= (LPSTR)szRetBuff;
		SetDlgItemText(IDC_EDIT_PROCESS, m_strScheduleNo);

		g_DMCont.m_dmHandlerOpt.GS(SDM11_RetestCode  , szRetBuff, MAX_PATH);
		SetDlgItemText(IDC_EDIT_RETEST_CODE,szRetBuff);

		g_DMCont.m_dmHandlerOpt.GS(SDM11_OperatorID  , szRetBuff, MAX_PATH);
		SetDlgItemText(IDC_EDIT_OPER_ID, szRetBuff);

		g_DMCont.m_dmHandlerOpt.GS(SDM11_ProgramName , szRetBuff, MAX_PATH);
		SetDlgItemText(IDC_EDIT_PROGRAM, szRetBuff);
		
		g_DMCont.m_dmEQP.GS(SDM0_HANDER_ID, szRetBuff, sizeof(szRetBuff));
		SetDlgItemText(IDC_EDIT_HOSTNAME, szRetBuff);

		g_DMCont.m_dmHandlerOpt.GS(SDM11_Operation , szRetBuff, MAX_PATH);
		SetDlgItemText(IDC_EDIT_PROCESS_ID, szRetBuff);
		
		g_DMCont.m_dmHandlerOpt.GS(SDM11_Temp , szRetBuff, MAX_PATH);
		SetDlgItemText(IDC_EDIT_TEMP, szRetBuff);

		g_DMCont.m_dmHandlerOpt.GS(SDM11_QTY , szRetBuff, MAX_PATH);
		SetDlgItemText(IDC_EDIT_QTY, szRetBuff);

		g_DMCont.m_dmHandlerOpt.GS(SDM11_ART, szRetBuff, MAX_PATH);
		SetDlgItemText(IDC_EDIT_ART,szRetBuff);

		g_DMCont.m_dmHandlerOpt.GS(SDM11_2DID_Sort_FilePath, szRetBuff, MAX_PATH);
		SetDlgItemText(IDC_EDIT_2DID_SORT_PATH, szRetBuff);

		m_nTheoreticalUPH = g_DMCont.m_dmHandlerOpt.GN(NDM11_Theoretical_UPH);
		SetDlgItemInt(IDC_EDIT_THEORETICAL, m_nTheoreticalUPH);
	}
}

void CLotInfoAmkorDlg::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnGetInfo.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnGetInfo.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnLock.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnLock.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_editQaSample.SetBackColor(COLOR_WWHITE);
	m_editQaSample.SetTextSize(20);
	
}

void CLotInfoAmkorDlg::OnCopyLastButtonEvent(int nEventNo)
{
	char szRetBuff[MAX_PATH] = {0,};

	switch(nEventNo)
	{
	case eLotNo:
		{
			g_DMCont.m_dmHandlerOpt.GS(SDM11_REC_LotNo, szRetBuff, MAX_PATH);
			m_strLotNo = (LPSTR)szRetBuff;
			GetDlgItem(IDC_EDIT_LOT_NO)->SetWindowText(szRetBuff);
		}break;
	case eSchedule:
		{
			g_DMCont.m_dmHandlerOpt.GS(SDM11_REC_ScheduleNo, szRetBuff, MAX_PATH);
			m_strScheduleNo = (LPSTR)szRetBuff;
//			GetDlgItem(IDC_EDIT_SCHEDULE)->SetWindowText(szRetBuff);
		}break;
	case eOperID:
		{
			g_DMCont.m_dmHandlerOpt.GS(SDM11_REC_OperatorID, szRetBuff, MAX_PATH);
			m_strOperID = (LPSTR)szRetBuff;
			GetDlgItem(IDC_EDIT_OPER_ID)->SetWindowText(szRetBuff);
		}break;
	}
}

void CLotInfoAmkorDlg::OnBnClickedButtonSave()
{
	GetDlgItemText(IDC_EDIT_LOT_NO, m_strLotNo);
	
// 	// Lot 정보가 아무것도 입력되지 않았을 때
// 	if( m_strOperID.GetLength() == 0 &&	
// 		m_strScheduleNo.GetLength() == 0 && 
// 		m_strLotNo.GetLength() == 0 ) {
// 			AfxMessageBox(_T("Lot 정보가 없습니다."));
// 			return;
// 	}
	BOOL bIsLotInfoEmpty = FALSE;

	bIsLotInfoEmpty = SaveInrolock();

	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
	if (nRunMode == eRUN_MODE_ONLINE) {
		//if (strcmp(m_szCompany, DEF_COMPANY_AMKOR) == 0) {
			CString strMode = _T("");
			switch (m_nModeTesterIF)
			{
				case eTesterIF_Mode_Normal: strMode = _T("PRODUCTION"); break;
				case eTesterIF_Mode_Grr: strMode = _T("GRR"); break;
				case eTesterIF_Mode_LoopTest: strMode = _T("LOOP TEST"); break;
				case eTesterIF_Mode_Audit: strMode = _T("AUDIT"); break;
			}

			CString strModeMsg = _T("");
			strModeMsg.Format("Do you check TESTER MODE = %s ??", strMode);
			int nRet = AfxMessageBox(strModeMsg, MB_TOPMOST | MB_YESNO);
			if (nRet != IDYES) {
				return;
			}

		//}
		BOOL bFindTestingStation = FALSE;
		for (int Index = 0; Index < eMaxPressUnitCount; Index++) {
			if (g_DMCont.m_dmEQP.GB(BDM0_TEST_STATUS_SITE1 + Index) == TRUE)
				bFindTestingStation = TRUE;
		}
		if (bFindTestingStation == TRUE) {
			AfxMessageBox(_T("Station does not end !!"), MB_TOPMOST);
			return;
		}
	}

	if(bIsLotInfoEmpty != TRUE){
		if(m_strLotNo.GetLength() > 32){
			AfxMessageBox(_T("Lot No Max Length is 32..please checking!!"));
			return;
		}
	
		//if (m_strLotNo.Find(",") != -1 || m_strLotNo.Find(";") != -1 || m_strLotNo.Find("/") != -1 || m_strLotNo.Find("\\") != -1 ){
		if(FileNameLimit(m_strLotNo) == FALSE){
			//Lot No에 ',' , ';' 이 있으면 안된다 
			AfxMessageBox(_T("Lot No is should not be included. ',' or ';' or '/' or '\\' or ' | ' ':' '*' '?' ' < ' '>'  "));
			return;
		}

		BOOL bErr = SummaryInterlock();
		if(bErr == TRUE){
			AfxMessageBox("Please Make Summary!!");
			return;
		}

		BOOL nCheck = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TesterBatchFile_Use);
// 		if (m_nFTRCQC == eStartMD_Auto2DIDSort || m_nFTRCQC == eStartMD_Auto2DIDSort_Continue)
// 		{
			if (nRunMode == eRUN_MODE_ONLINE) {
				if (nCheck == DEF_ON)
				{
					g_McComm.CmdTesterBatchFileExcute();
				}
			}
//		}
		SetDisplayUserData(TRUE);

		g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_LotStartMode, m_bLotNormalRC);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_FTRTQCContinue, m_nFTRCQC);
		g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_VisionUse, m_bVisionUse);		

		char szCokName[512] = {0,};
		ST_BASIC_INFO   stBasicData, stBeforeBasicData;
		g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
		g_COK_FileMgr.LoadBasicInfo(szCokName, stBasicData);

		stBeforeBasicData = stBasicData;

		stBasicData.nFTRTQC	   = (int)g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
		stBasicData.nStartMode = (int)g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_LotStartMode);
		stBasicData.nVision = (int)g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_VisionUse);
		strcpy_s(stBasicData.szLotNo, sizeof(stBasicData.szLotNo), m_strLotNo);
		strcpy_s(stBasicData.szScheduleNo, sizeof(stBasicData.szScheduleNo), m_strScheduleNo);
		strcpy_s(stBasicData.szOperID, sizeof(stBasicData.szOperID), m_strOperID);

		//UpdateData();
		if( m_nFTRCQC == eStartMD_QA || m_nFTRCQC == eStartMD_QA_Continue)
		{
			g_DMCont.m_dmHandlerOpt.SN(NDM11_QaSampleCount, m_nQaSampleCnt);
		}

		g_COK_FileMgr.SaveBasicInfo(szCokName, &stBasicData);
		if(m_bVisionUseNoUseBak != m_bVisionUse){
			g_McComm.CmdVisionUseNouse(eVisionPos_TestSite);
		}

		BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
		if ( nRunMode == eRUN_MODE_ONLINE && bLotState != eHandlerLotMode_OnGoing /*&& strcmp(m_szCompany, DEF_COMPANY_AMKOR) == 0*/)
		{
			MakeLog("[MENU = Lot Information, DATA NAME = Tester Mode ][BEFORE=%d, AFTER=%d]", g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_TESTER_IF_PRO_AUDIT), m_nModeTesterIF);
			g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_TESTER_IF_PRO_AUDIT, m_nModeTesterIF);
			//g_McComm.CmdTesterOnlineModeChange(m_nModeTesterIF);
		}

		std::vector<std::pair<CString,std::pair<CString,CString>>> vData;

		stBeforeBasicData.IsSameLotInfo(stBasicData,vData);

		if(!(vData.empty()))
		{
			LotInfoChangeLog(vData);
		}
	}
	
}

void CLotInfoAmkorDlg::OnBnClickedRadioVision()
{
	m_bVisionUse = eOPTION_USE;
	//UpdateData(TRUE);
}


void CLotInfoAmkorDlg::OnBnClickedRadioNotVision()
{
	// AfxGetMainWnd() 메인 윈도우의 정보 얻어온다.
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	// GetActiveView() 현재 활성화된 창의 정보를 얻어온다.
	CSLTView* pSltView = (CSLTView*)pMainFrm->GetActiveView();
	//CButton* pBtnNotUse = (CButton*)GetDlgItem(IDC_RADIO_NOT_VISION);
	//CButton* pBtnUse = (CButton*)GetDlgItem(IDC_RADIO_VISION);

	GetDlgItem(IDCANCEL)->SetFocus();

	int nUserLevel = pSltView->m_nLogInLv;
	int nRetLevel  = 0;

	// 이전 선택이 Use였고, 현재 변경 된 Status가 Not Use 일 경우만 판단 & 사용자 Level이 Engineer 이하 등급일 경우[현재 로그인 상태 포함]
	if(m_bVisionUse == eOPTION_USE /*&& nUserLevel < eUSERLEVEL_ENGINEER*/){
		BOOL bRet = g_LibCommCtrl.ShowLogInBox(nUserLevel, &nRetLevel);

		if(RETURN_OK == bRet ){
			if(nRetLevel >= eUSERLEVEL_ENGINEER){
				m_bVisionUse = eOPTION_NOT_USE;
			}else{
				m_bVisionUse = eOPTION_USE;
				AfxMessageBox("User level is not apply selection.");				
			}			
		}
		else{
			m_bVisionUse = eOPTION_USE;
		}
	}
	//UpdateData(TRUE);
}

void CLotInfoAmkorDlg::SetLogInLevel(int nLogInLevel)
{
	m_nLogInLevel = nLogInLevel;
}


/*=============================================================================
  DESCRIPT : Amkor 요청 사항 LotInfo.txt 에서 LotInfo정보를 불러온다
  KEY WORD : 
  ARGUMENT : 
  RETURN   : 
 ==============================================================================*/
void CLotInfoAmkorDlg::OnBnClickedGetInfo()
{
	char szTemp[MAX_PATH] ={0,}, szRetBuff[MAX_PATH]={0,};
	CString strLotInfo =_T(""),  strTitle = _T(""), strData = _T("");
	char szLotInfoPath[MAX_PATH] ={0,};
	CString strLotInfoPath =_T("");

	g_DMCont.m_dmHandlerOpt.GS(SDM11_LotInfoFile_Path,szLotInfoPath,sizeof(szLotInfoPath));

	strLotInfoPath.Format("%s\\APL Info.txt",szLotInfoPath);
	std::ifstream ifile;
	
	ifile.open(strLotInfoPath);

	if(ifile.is_open())
	{
		while(ifile.getline(szTemp,sizeof(szTemp)))
		{
			strLotInfo = szTemp;

			int nIndx = strLotInfo.Find(':');
			CString strTitle = strLotInfo.Left(nIndx);
			CString strData = strLotInfo.Mid(nIndx+1);

			strTitle.MakeUpper();

			// Lot Info Dialog 에 표시 되는 항목
			if(strTitle.Compare("LOT") == 0 ){
				m_strLotNo = strData;
				SetDlgItemText(IDC_EDIT_LOT_NO,m_strLotNo);
			}
			else if(strTitle.Compare("CUST") == 0){
				m_strCustomName = strData;
				SetDlgItemText(IDC_EDIT_CUSTOM,m_strCustomName);
			}
			else if(strTitle.Compare("DEVICE") == 0){
				m_strDeviceName = strData;
				SetDlgItemText(IDC_EDIT_DEVICE,m_strDeviceName);
			}
			else if(strTitle.Compare("RETEST") == 0){
				m_strScheduleNo = strData;
				SetDlgItemText(IDC_EDIT_PROCESS,m_strScheduleNo);
				
				if(strData.Find("FT1") != -1){
					sprintf_s(szRetBuff,sizeof(szRetBuff),"0");
				}else{
					sprintf_s(szRetBuff,sizeof(szRetBuff),strData.Right(1));
				}

				//QA Mode 임시 삭제 
				//if(strData.Find("QA") != -1){
				//	CheckRadioButton(IDC_RADIO_FT,IDC_RADIO_AUTO_GRR_CONTINUE,IDC_RADIO_QA);
				//	m_nFTRCQC = eStartMD_QA;
				//}else{
					CheckRadioButton(IDC_RADIO_FT, IDC_RADIO_AUTO_LOOP_CONTINUE,IDC_RADIO_FT);
					m_nFTRCQC = eStartMD_Initial;
				//}
				m_strRetestCode.Format("%s", szRetBuff);
				SetDlgItemText(IDC_EDIT_RETEST_CODE, szRetBuff);
			}
			else if(strTitle.Compare("TP_PROG") == 0){
				m_strPGM_Name = strData;
				SetDlgItemText(IDC_EDIT_PROGRAM,m_strPGM_Name);
			}
			else if(strTitle.Compare("OPERATION") == 0){
				m_strProcessID = strData;
				SetDlgItemText(IDC_EDIT_PROCESS_ID,m_strProcessID);
			}
			else if(strTitle.Compare("TEMP") == 0){
				m_strTemp = strData;
				SetDlgItemText(IDC_EDIT_TEMP,m_strTemp);
			}
			else if(strTitle.Compare("QTY") == 0){
				m_strQty = strData;
				SetDlgItemText(IDC_EDIT_QTY,m_strQty);
			}
			else if(strTitle.Compare("OPERATOR_ID") == 0){
				m_strOperID = strData;
				SetDlgItemText(IDC_EDIT_OPER_ID,m_strOperID);
			}

			// Lot Info Dialog 에 표시 되지 않는 항목
			else if(strTitle.Compare("TP_REV") == 0){
				m_strTpRev = strData;
			}
			else if(strTitle.Compare("SOAK_TIME") == 0){
				m_strSoakTime = strData;
			}
			else if(strTitle.Compare("FUSE") == 0){
				m_strFuse = strData;
			}
			else if(strTitle.Compare("HANDLER") == 0){
				m_strHandler = strData;
			}
			else if(strTitle.Compare("LOAD_BD_NO") == 0){
				m_strLoadBdNo = strData;
			}
			else if(strTitle.Compare("SOCKET_ID") == 0){
				m_strSocketId = strData;
			}
			else if(strTitle.Compare("C_KIT_ID") == 0){
				m_strCKitID = strData;
			}
			else if(strTitle.Compare("FOUNDRY_LOTID") == 0){
				m_strFoundryLotId = strData; 
			}
			else if(strTitle.Compare("ASSEMBLY_STARTDATE") == 0){
				m_strAssemblyStartdate = strData;
			}
			else if(strTitle.Compare("ASSEMBLY_ENDDATE") == 0){
				m_strAssemblyEnddate = strData;
			}
			else if(strTitle.Compare("ASSY_VENDOR_LOTID") == 0){
				m_StrAssyVendorLotid = strData;
			}
			else if(strTitle.Compare("TEST_VENDOR_LOTID") == 0){
				m_strTestVendorLotid = strData;
			}
			else if(strTitle.Compare("BOT_SUB_VENDOR") == 0){
				m_strBotSubVendor = strData;
			}
			else if(strTitle.Compare("NICKNAME") == 0){
				m_strNickname = strData;
			}
			NEXT;
		}
		ifile.close();
		int nAutoRetestKind = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoRetest);
		 
		BOOL bUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
		if(bUseMode == eOPTION_USE){
			switch(nAutoRetestKind){
			case eAutoRetest_Aonly: m_strArt = _T("Aonly"); break;
			case eAutoRetest_AB:    m_strArt = _T("AB");    break;
			case eAutoRetest_AAB:   m_strArt = _T("AAB");   break;
			case eAutoRetest_AA:    m_strArt = _T("AA");    break;
			}
		}else{
			m_strArt.Format("No Use");
		}
		SetDlgItemText(IDC_EDIT_ART, m_strArt);

		g_DMCont.m_dmEQP.GS(SDM0_HANDER_ID, szRetBuff, sizeof(szRetBuff));
		m_strHostName = (LPSTR)szRetBuff;
		SetDlgItemText(IDC_EDIT_HOSTNAME, szRetBuff);

		LotInfoLock(FALSE);
	}else{
		AfxMessageBox("Can't open is LotInfo File");
	}
	
}

void CLotInfoAmkorDlg::OnBnClickedLock()
{	
	BOOL bLockState = g_DMCont.m_dmEQP.GB(BDM0_LOT_INFO_IS_LOCK);
	LotInfoLock(bLockState);
}

void CLotInfoAmkorDlg::LotInfoLock(BOOL bLockState)
{
	CString strLockBtnName =_T("");

	m_BtnLock.GetWindowText(strLockBtnName);
	strLockBtnName.MakeUpper();

	if(bLockState == FALSE){
		GetDlgItem(IDC_EDIT_LOT_NO)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OPER_ID)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PROGRAM)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEVICE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CUSTOM)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PROCESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_HOSTNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PROCESS_ID)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEMP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RETEST_CODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_QTY)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ART)->EnableWindow(FALSE);
		//GetDlgItem(IDC_EDIT_QA_CNT)->EnableWindow(FALSE);
		//GetDlgItem(IDC_STATIC_EA)->EnableWindow(FALSE);

		m_BtnLock.SetWindowText("UnLock");
		g_DMCont.m_dmEQP.SB(BDM0_LOT_INFO_IS_LOCK,TRUE);
	}else{
		GetDlgItem(IDC_EDIT_LOT_NO)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_OPER_ID)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PROGRAM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DEVICE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CUSTOM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PROCESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_HOSTNAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PROCESS_ID)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TEMP)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RETEST_CODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_QTY)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ART)->EnableWindow(TRUE);
		//GetDlgItem(IDC_EDIT_QA_CNT)->EnableWindow(TRUE);
		//GetDlgItem(IDC_STATIC_EA)->EnableWindow(TRUE);
		m_BtnLock.SetWindowText("Lock");
		g_DMCont.m_dmEQP.SB(BDM0_LOT_INFO_IS_LOCK,FALSE);
	}
}

BOOL CLotInfoAmkorDlg::SaveInrolock()
{
	CString strLotData = _T("");
	BOOL bIsLotInfoEmpty = FALSE;
	//int nLotData[12] = {IDC_EDIT_LOT_NO, IDC_EDIT_CUSTOM, IDC_EDIT_DEVICE, IDC_EDIT_PROCESS, IDC_EDIT_OPER_ID, IDC_EDIT_PROGRAM,
	//	                IDC_EDIT_HOSTNAME, IDC_EDIT_PROCESS_ID, IDC_EDIT_TEMP, IDC_EDIT_RETEST_CODE, IDC_EDIT_QTY, IDC_EDIT_ART};
	//CString strMsg[12] ={"Lot No", "Customer Name","Device Name", "Process", "Operator ID", "Program Name", "Host Name", "Process ID", "Temp", "Retest Code", "Lot Size", "ART"};

	int nLotData[2] = { IDC_EDIT_LOT_NO,IDC_EDIT_PROGRAM};
	int nTheoreticlaData = 0;
	CString strMsg[2] = { "Lot No","Tester Program Version"};

	CString strMsgData = _T("");

	for(int i=0; i< /*12*/2; i++)
	{
		GetDlgItemText(nLotData[i],strLotData);
		if(strLotData.IsEmpty()){
			strMsgData.Format("%s is Empty",strMsg[i]);
			AfxMessageBox(strMsgData);
			bIsLotInfoEmpty = TRUE;
			break;
		}
	}
	char szCompany[16] = { 0, };

	return bIsLotInfoEmpty;
}

void CLotInfoAmkorDlg::MakeLog(LPCTSTR fmt, ...)
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


void CLotInfoAmkorDlg::LotInfoChangeLog(std::vector<std::pair<CString,std::pair<CString,CString>>> vData)
{
	for(int i = 0 ; i< (int)vData.size(); i++)
	{
		std::pair<CString,std::pair<CString,CString>> p;

		p = vData[i];

		MakeLog("[MENU = Lot Information, DATA NAME = %s ][BEFORE=%s, AFTER=%s]", p.first, p.second.first, p.second.second);
		
	}
}

void CLotInfoAmkorDlg::OnRadioLotModeCtrl( UINT nID )
{
	if (nID != IDC_RADIO_FT && nID != IDC_RADIO_FT_CONTINUE)
	{
		int nRet = ShowLogInBox();
		if (nRet == FALSE)
		{
			m_nFTRCQC = eStartMD_Initial;
			UpdateData(FALSE);
			return;
		}
	}

	switch(nID)
	{
	case IDC_RADIO_FT: { AfxMessageBox("Select FT Mode?"); m_nFTRCQC = eStartMD_Initial; } break;
	case IDC_RADIO_RC:  { AfxMessageBox("Select RT Mode?"); m_nFTRCQC = eStartMD_Retest;  } break;
	case IDC_RADIO_AUTO_GRR: { AfxMessageBox("Select GRR Mode?"); m_nFTRCQC = eStartMD_AutoGRR;  } break;
	case IDC_RADIO_QA:  { AfxMessageBox("Select QA Mode?"); m_nFTRCQC = eStartMD_QA;  } break;
	case IDC_RADIO_AUTO_LOOP: { AfxMessageBox("Select Loop Mode?"); m_nFTRCQC = eStartMD_AutoLoop;  } break;

	case IDC_RADIO_FT_CONTINUE:	{ AfxMessageBox("Select FT Continue Mode?"); m_nFTRCQC = eStartMD_InitContinue;  } break;
	case IDC_RADIO_RC_CONTINUE:  { AfxMessageBox("Select RT Continue Mode?"); m_nFTRCQC = eStartMD_RetestContinue;  } break;
	case IDC_RADIO_AUTO_GRR_CONTINUE: { AfxMessageBox("Select GRR Continue Mode?"); m_nFTRCQC = eStartMD_AutoGRR_Continue;  } break;
	case IDC_RADIO_QA_CONTINUE:  { AfxMessageBox("Select QA Continue Mode?");  m_nFTRCQC = eStartMD_QA_Continue; } break;
	case IDC_RADIO_AUTO_LOOP_CONTINUE: { AfxMessageBox("Select LOOP Continue Mode?"); m_nFTRCQC = eStartMD_AutoLoop_Continue;  } break;
	
		
	default: { }break;
	}

// 	if (m_nFTRCQC == eStartMD_Auto2DIDSort || m_nFTRCQC == eStartMD_Auto2DIDSort_Continue)
// 		BarcodeSortShowHide(SW_SHOW);	
// 	else
		BarcodeSortShowHide(SW_HIDE);

	//UpdateData(FALSE);
}


void CLotInfoAmkorDlg::OnEnSetfocusEditQaCnt()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nQaSampleCnt, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Sample Count", "999999", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_QA_CNT)->SetWindowText(szRetVal);
	}
}

BOOL CLotInfoAmkorDlg::SummaryInterlock()
{
	char szLotNo[128] ={0,},szBeforeLotNo[128] ={0,}, szScheduleNo[128]={0,}, szBeforeScheduleNo[128]={0,};
	CString strLotNo=_T(""), strScheduleNo =_T("");
	BOOL bErr = TRUE;
	g_DMCont.m_dmHandlerOpt.GS(SDM11_LotNo, szBeforeLotNo, sizeof(szBeforeLotNo));

	g_DMCont.m_dmHandlerOpt.GS(SDM11_ScheduleNo, szBeforeScheduleNo, sizeof(szBeforeScheduleNo));

	int nLotMode = 0, nAfterLotMode =0;
	nLotMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);

	if(strcmp(szBeforeLotNo,"")==0  || g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt) == 0)
	{
		bErr = FALSE;
		return bErr;
	}

	switch (nLotMode) 
	{
		case eStartMD_Initial:
		case eStartMD_InitContinue: 
		{
			nLotMode = eStartMD_Initial;
		}break;
		case eStartMD_QA:
		case eStartMD_QA_Continue: 
		{
			nLotMode = eStartMD_QA;
		}break;
		case eStartMD_Retest:
		case eStartMD_RetestContinue: 
		{
			nLotMode = eStartMD_Retest;
		}break;
		case eStartMD_AutoGRR:
		case eStartMD_AutoGRR_Continue: 
		{
			nLotMode = eStartMD_AutoGRR;
		}break;
		case eStartMD_AutoLoop:
		case eStartMD_AutoLoop_Continue: 
		{
			nLotMode = eStartMD_AutoLoop;
		}break;
	}

	switch(m_nFTRCQC)
	{
		case eStartMD_Initial:
		case eStartMD_InitContinue:
		{
			nAfterLotMode = eStartMD_Initial;							   
		}break;
		case eStartMD_QA:
		case eStartMD_QA_Continue:
		{
			nAfterLotMode = eStartMD_QA;
		}break;
		case eStartMD_Retest:
		case eStartMD_RetestContinue:
		{
			nAfterLotMode = eStartMD_Retest;
		}break;
		case eStartMD_AutoGRR:
		case eStartMD_AutoGRR_Continue:
		{
			nAfterLotMode = eStartMD_AutoGRR;
		}break;
		case eStartMD_AutoLoop:
		case eStartMD_AutoLoop_Continue: 
		{
			nAfterLotMode = eStartMD_AutoLoop;
		}break;
	}

	GetDlgItemText(IDC_EDIT_LOT_NO,strLotNo);
	GetDlgItemText(IDC_EDIT_PROCESS,strScheduleNo);
	strLotNo.Replace(" ","");
	strScheduleNo.Replace(" ","");

	if(strcmp(szBeforeLotNo,strLotNo) != 0 /*|| strcmp(szBeforeScheduleNo,strScheduleNo) !=0*/ || (/*nLotMode !=eStartMD_AutoGRR &&*/ nLotMode != nAfterLotMode)){
		MakeLog("[Lot Information save is Fail. Can not make Summary. Lot Information is Different]");
// 		CString strDatePath = _T(""), strFileName =_T(""), strFindFileName = _T("");
// 		int nChkCount =0;
// 
// 		bErr = TRUE;
// 
// 		SYSTEMTIME time;
// 		::GetLocalTime( &time );
// 		
// 		if(time.wDay == 1){
// 			nChkCount = 1;
// 		}
// 		CString strPath = _T("");
// 		strPath.Format("%s", SZ_DIR_PATH_SUMMARY);
// 
// 		int nMonth = 0; 
// 		int nYear  = 0;
// 
// 		for(int i =0; i<=nChkCount; i++){
// 			strFindFileName.Format("%s_%s",szBeforeLotNo,szBeforeScheduleNo);
// 
// 			nMonth = time.wMonth;
// 			nYear  = time.wYear;
// 
// 			nMonth -= i;
// 
// 			if(nMonth <=0){ // 년도가 변경 되었을 경우
// 				nYear -= i;
// 				nMonth = 12;
// 			}
// 
// 			CString strMonth =_T("");
// 			if(nMonth >=10){
// 				strMonth.Format("%02d",nMonth);
// 			}else{
// 				strMonth.Format("%01d",nMonth);
// 			}
// 			strDatePath.Format("\\%04d\\%s\\*.csv",nYear,strMonth);
// 
// 			strPath += strDatePath;
// 
// 			CFileFind finder;
// 			BOOL bFindFile = finder.FindFile(strPath);
// 			while(bFindFile){
// 				bFindFile = finder.FindNextFile();
// 				if(finder.IsDots()) continue;
// 				if(finder.IsDirectory())continue;
// 
// 				strFileName = finder.GetFileName();
// 
// 				if(strFileName.Find(strFindFileName) != -1){
// 					bErr = FALSE;
// 					break;
// 				}
// 			}
// 		}
// 	k
		bErr = TRUE;
	}else{
		bErr = FALSE;
	}

	return bErr;
}

void CLotInfoAmkorDlg::OnBnClickedRadioProduction()
{
	m_nModeTesterIF = eTesterIF_Mode_Normal;
}


void CLotInfoAmkorDlg::OnBnClickedRadioAudit()
{
	int nRet = ShowLogInBox();

	if (nRet == FALSE)
	{
		m_nModeTesterIF = eTesterIF_Mode_Normal;
		UpdateData(FALSE);
		return;
	}

	m_nModeTesterIF = eTesterIF_Mode_Audit;
}


void CLotInfoAmkorDlg::OnBnClickedRadioLoop()
{
	int nRet = ShowLogInBox();

	if (nRet == FALSE)
	{
		m_nModeTesterIF = eTesterIF_Mode_Normal;
		UpdateData(FALSE);
		return;
	}
	

	m_nModeTesterIF = eTesterIF_Mode_LoopTest;
}


void CLotInfoAmkorDlg::OnBnClickedRadioGrr()
{
	int nRet = ShowLogInBox();

	if (nRet == FALSE)
	{
		m_nModeTesterIF = eTesterIF_Mode_Normal;
		UpdateData(FALSE);
		return;
	}

	m_nModeTesterIF = eTesterIF_Mode_Grr;
}

void CLotInfoAmkorDlg::CompanySetting()
{

 	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	
	GetDlgItem(IDC_RADIO_AUTO_AUDIT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_AUTO_AUDIT_CONTINUE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_AUTO_LOOP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_AUTO_LOOP_CONTINUE)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_RADIO_AUTO_2DID_SORT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_AUTO_2DID_SORT_CONTINUE)->ShowWindow(SW_HIDE);
	BarcodeSortShowHide(SW_HIDE);
	
	

	GetDlgItem(IDC_STATIC_THEORY_UPH)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_THEORETICAL)->ShowWindow(SW_HIDE);
	
}



void CLotInfoAmkorDlg::OnBnClickedButton2didSortPath()
{
	CString szFilter = "All Files(*.csv)|*.csv||";
	CString filetype = "*.*";

	CFileDialog dlg(TRUE, NULL, NULL, OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST, szFilter);

	if (dlg.DoModal() == IDOK)
	{
		CString str2DIDSortPath = dlg.GetPathName();
		m_str2DIDSortPath = str2DIDSortPath;
		UpdateData(FALSE);
	}
}

void CLotInfoAmkorDlg::BarcodeSortShowHide(int nShow)
{
	if (nShow == SW_SHOW)
	{
		GetDlgItem(IDC_STATIC_2DID_SORT_PATH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_2DID_SORT_PATH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_2DID_SORT_PATH)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_STATIC_2DID_SORT_PATH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_2DID_SORT_PATH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_2DID_SORT_PATH)->ShowWindow(SW_HIDE);
	}
}

BOOL CLotInfoAmkorDlg::FileNameLimit(CString strName)
{
	BOOL bFind = TRUE;
	if (strName.Find(_T("\\")) != -1) bFind = FALSE;
	else if (strName.Find(_T("/")) != -1) bFind = FALSE;
	else if (strName.Find(_T(":")) != -1) bFind = FALSE;
	else if (strName.Find(_T("*")) != -1) bFind = FALSE;
	else if (strName.Find(_T("?")) != -1) bFind = FALSE;
	else if (strName.Find(_T('"')) != -1) bFind = FALSE;
	else if (strName.Find(_T("<")) != -1) bFind = FALSE;
	else if (strName.Find(_T(">")) != -1) bFind = FALSE;
	else if (strName.Find(_T("|")) != -1) bFind = FALSE;
	else if (strName.Find(_T(";")) != -1) bFind = FALSE;
	else if (strName.Find(_T(",")) != -1) bFind = FALSE;

	if (bFind == FALSE) return FALSE;

	return TRUE;
}


BOOL CLotInfoAmkorDlg::ShowLogInBox()
{
	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);


	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	//CSLTView* pSltView = (CSLTView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();


	int nUserLevel = m_pSltView->m_nLogInLv;
	int nRetLevel = 0;

	// 이전 선택이 Use였고, 현재 변경 된 Status가 Not Use 일 경우만 판단 & 사용자 Level이 Engineer 이하 등급일 경우[현재 로그인 상태 포함]

	BOOL bRet = g_LibCommCtrl.ShowLogInBox(nUserLevel, &nRetLevel);

	if (RETURN_OK == bRet) 
		return TRUE;	
	else
		return FALSE;
}

void CLotInfoAmkorDlg::CheckHandlerMode(int nID)
{
	int nIDC = IDC_RADIO_FT;
	switch (nID)
	{
		case eStartMD_Initial: { nIDC = IDC_RADIO_FT; } break;
		case eStartMD_Retest: {nIDC = IDC_RADIO_RC;  } break;
		case eStartMD_AutoGRR: {  nIDC = IDC_RADIO_AUTO_GRR;  } break;
		case eStartMD_QA: {  nIDC = IDC_RADIO_QA;  } break;
		case eStartMD_AutoLoop: {nIDC = IDC_RADIO_AUTO_LOOP;  } break;
		case eStartMD_InitContinue: {  nIDC = IDC_RADIO_FT_CONTINUE;  } break;
		case eStartMD_RetestContinue: {  nIDC = IDC_RADIO_RC_CONTINUE;  } break;
		case eStartMD_AutoGRR_Continue: { nIDC = IDC_RADIO_AUTO_GRR_CONTINUE;  } break;
		case eStartMD_QA_Continue: {   nIDC = IDC_RADIO_QA_CONTINUE; } break;
		case eStartMD_AutoLoop_Continue: { nIDC = IDC_RADIO_AUTO_LOOP_CONTINUE;  } break;

		default: { }break;
	}

	CButton* pButton = (CButton*)GetDlgItem(nIDC);
	pButton->SetCheck(true);
}