// SPD_TM_SHUTTLE.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "SPD_TM_LoadTbl.h"
#include "afxdialogex.h"
#include "SLTView.h"
#include "MainFrm.h"
#include "DlgTableVacuumCheck.h"

// CSPD_TM_SHUTTLE 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSPD_TM_SHUTTLE, CDialog)

CSPD_TM_SHUTTLE::CSPD_TM_SHUTTLE(CWnd* pParent /*=NULL*/)
	: CDialog(CSPD_TM_SHUTTLE::IDD, pParent)
{
	//g_DMCont.m_dmShuttle.Open(DM11);
	m_nSPD_LoadTbl1_Y = g_DMCont.m_dmShuttle.GN(NDM3_SPD_LoadTbl1_Y);
	m_nACC_LoadTbl1_Y = g_DMCont.m_dmShuttle.GN(NDM3_ACC_LoadTbl1_Y);
	m_nDCC_LoadTbl1_Y = g_DMCont.m_dmShuttle.GN(NDM3_DCC_LoadTbl1_Y);
	m_nSPD_LoadTbl2_Y = g_DMCont.m_dmShuttle.GN(NDM3_SPD_LoadTbl2_Y);
	m_nACC_LoadTbl2_Y = g_DMCont.m_dmShuttle.GN(NDM3_ACC_LoadTbl2_Y);
	m_nDCC_LoadTbl2_Y = g_DMCont.m_dmShuttle.GN(NDM3_DCC_LoadTbl2_Y);
	m_nSPD_LoadTbl3_Y = g_DMCont.m_dmShuttle.GN(NDM3_SPD_LoadTbl3_Y);
	m_nACC_LoadTbl3_Y = g_DMCont.m_dmShuttle.GN(NDM3_ACC_LoadTbl3_Y);
	m_nDCC_LoadTbl3_Y = g_DMCont.m_dmShuttle.GN(NDM3_DCC_LoadTbl3_Y);
	m_nSPD_LoadTbl4_Y = g_DMCont.m_dmShuttle.GN(NDM3_SPD_LoadTbl4_Y);
	m_nACC_LoadTbl4_Y = g_DMCont.m_dmShuttle.GN(NDM3_ACC_LoadTbl4_Y);
	m_nDCC_LoadTbl4_Y = g_DMCont.m_dmShuttle.GN(NDM3_DCC_LoadTbl4_Y);

	m_nSPD_LoadTbl1_Rotate = g_DMCont.m_dmShuttle.GN(NDM3_SPD_LoadTbl1_Rotate);
	m_nACC_LoadTbl1_Rotate = g_DMCont.m_dmShuttle.GN(NDM3_ACC_LoadTbl1_Rotate);
	m_nDCC_LoadTbl1_Rotate = g_DMCont.m_dmShuttle.GN(NDM3_DCC_LoadTbl1_Rotate);
	m_nSPD_LoadTbl2_Rotate = g_DMCont.m_dmShuttle.GN(NDM3_SPD_LoadTbl2_Rotate);
	m_nACC_LoadTbl2_Rotate = g_DMCont.m_dmShuttle.GN(NDM3_ACC_LoadTbl2_Rotate);
	m_nDCC_LoadTbl2_Rotate = g_DMCont.m_dmShuttle.GN(NDM3_DCC_LoadTbl2_Rotate);
	m_nSPD_LoadTbl3_Rotate = g_DMCont.m_dmShuttle.GN(NDM3_SPD_LoadTbl3_Rotate);
	m_nACC_LoadTbl3_Rotate = g_DMCont.m_dmShuttle.GN(NDM3_ACC_LoadTbl3_Rotate);
	m_nDCC_LoadTbl3_Rotate = g_DMCont.m_dmShuttle.GN(NDM3_DCC_LoadTbl3_Rotate);
	m_nSPD_LoadTbl4_Rotate = g_DMCont.m_dmShuttle.GN(NDM3_SPD_LoadTbl4_Rotate);
	m_nACC_LoadTbl4_Rotate = g_DMCont.m_dmShuttle.GN(NDM3_ACC_LoadTbl4_Rotate);
	m_nDCC_LoadTbl4_Rotate = g_DMCont.m_dmShuttle.GN(NDM3_DCC_LoadTbl4_Rotate);

	m_nBarcodeRetry = g_DMCont.m_dmShuttle.GN(NDM3_LdTbl_BarcodeMaxRetry);

	m_n2D_BarcodeBak = 0;

	m_nViratorOption = g_DMCont.m_dmHandlerOpt.GN(NDM11_LOADING_TABLE_VIBRATOR_OPTION);
	m_nVibratorConut = g_DMCont.m_dmHandlerOpt.GN(NDM11_LOADING_TABLE_VIBRATOR_RETRY_CNT);
	m_dVibratorDelayTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_LOADING_TABLE_VIBRATOR_DELAY_TIME);
	m_dLoadTableWaitTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_LOADING_TABLE_WAIT_TIME);
	m_bChkVibUse = 0;

	m_nSPD_Y_Barcode_Limit = g_DMCont.m_dmShuttle.GN(NDM3_SPD_Y_Barcode_Limit);

	m_nNumberOfLoadTable = g_DMCont.m_dmHandlerOpt.GN(NDM11_NUMBER_OF_LOADTABLE);
}

CSPD_TM_SHUTTLE::~CSPD_TM_SHUTTLE()
{
}

void CSPD_TM_SHUTTLE::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BUTTON_SAVE, m_BtnSave);
    DDX_Control(pDX, IDCANCEL, m_BtnCancel);
    DDX_Text(pDX, IDC_EDIT_SPD_SH1_Y, m_nSPD_LoadTbl1_Y);
    DDX_Text(pDX, IDC_EDIT_ACC_SH1_Y, m_nACC_LoadTbl1_Y);
    DDX_Text(pDX, IDC_EDIT_DCC_SH1_Y, m_nDCC_LoadTbl1_Y);
    DDX_Text(pDX, IDC_EDIT_SPD_SH2_Y, m_nSPD_LoadTbl2_Y);
    DDX_Text(pDX, IDC_EDIT_ACC_SH2_Y, m_nACC_LoadTbl2_Y);
    DDX_Text(pDX, IDC_EDIT_DCC_SH2_Y, m_nDCC_LoadTbl2_Y);
	DDX_Text(pDX, IDC_EDIT_SPD_SH3_Y, m_nSPD_LoadTbl3_Y);
	DDX_Text(pDX, IDC_EDIT_ACC_SH3_Y, m_nACC_LoadTbl3_Y);
	DDX_Text(pDX, IDC_EDIT_DCC_SH3_Y, m_nDCC_LoadTbl3_Y);
	DDX_Text(pDX, IDC_EDIT_SPD_SH4_Y, m_nSPD_LoadTbl4_Y);
	DDX_Text(pDX, IDC_EDIT_ACC_SH4_Y, m_nACC_LoadTbl4_Y);
	DDX_Text(pDX, IDC_EDIT_DCC_SH4_Y, m_nDCC_LoadTbl4_Y);

	DDX_Text(pDX, IDC_EDIT_SPD_SH1_ROTATE, m_nSPD_LoadTbl1_Rotate);
	DDX_Text(pDX, IDC_EDIT_ACC_SH1_ROTATE, m_nACC_LoadTbl1_Rotate);
	DDX_Text(pDX, IDC_EDIT_DCC_SH1_ROTATE, m_nDCC_LoadTbl1_Rotate);
	DDX_Text(pDX, IDC_EDIT_SPD_SH2_ROTATE, m_nSPD_LoadTbl2_Rotate);
	DDX_Text(pDX, IDC_EDIT_ACC_SH2_ROTATE, m_nACC_LoadTbl2_Rotate);
	DDX_Text(pDX, IDC_EDIT_DCC_SH2_ROTATE, m_nDCC_LoadTbl2_Rotate);
	DDX_Text(pDX, IDC_EDIT_SPD_SH3_ROTATE, m_nSPD_LoadTbl3_Rotate);
	DDX_Text(pDX, IDC_EDIT_ACC_SH3_ROTATE, m_nACC_LoadTbl3_Rotate);
	DDX_Text(pDX, IDC_EDIT_DCC_SH3_ROTATE, m_nDCC_LoadTbl3_Rotate);
	DDX_Text(pDX, IDC_EDIT_SPD_SH4_ROTATE, m_nSPD_LoadTbl4_Rotate);
	DDX_Text(pDX, IDC_EDIT_ACC_SH4_ROTATE, m_nACC_LoadTbl4_Rotate);
	DDX_Text(pDX, IDC_EDIT_DCC_SH4_ROTATE, m_nDCC_LoadTbl4_Rotate);

    DDX_Control(pDX, IDC_EDIT_SPD_SH1_Y, m_editSPD_LoadTbl1_Y);
    DDX_Control(pDX, IDC_EDIT_ACC_SH1_Y, m_editACC_LoadTbl1_Y);
    DDX_Control(pDX, IDC_EDIT_DCC_SH1_Y, m_editDCC_LoadTbl1_Y);
    DDX_Control(pDX, IDC_EDIT_SPD_SH2_Y, m_editSPD_LoadTbl2_Y);
    DDX_Control(pDX, IDC_EDIT_ACC_SH2_Y, m_editACC_LoadTbl2_Y);
    DDX_Control(pDX, IDC_EDIT_DCC_SH2_Y, m_editDCC_LoadTbl2_Y);
	DDX_Control(pDX, IDC_EDIT_SPD_SH3_Y, m_editSPD_LoadTbl3_Y);
	DDX_Control(pDX, IDC_EDIT_ACC_SH3_Y, m_editACC_LoadTbl3_Y);
	DDX_Control(pDX, IDC_EDIT_DCC_SH3_Y, m_editDCC_LoadTbl3_Y);
	DDX_Control(pDX, IDC_EDIT_SPD_SH4_Y, m_editSPD_LoadTbl4_Y);
	DDX_Control(pDX, IDC_EDIT_ACC_SH4_Y, m_editACC_LoadTbl4_Y);
	DDX_Control(pDX, IDC_EDIT_DCC_SH4_Y, m_editDCC_LoadTbl4_Y);

	DDX_Control(pDX, IDC_EDIT_SPD_SH1_ROTATE, m_editSPD_LoadTbl1_Rotate);
	DDX_Control(pDX, IDC_EDIT_ACC_SH1_ROTATE, m_editACC_LoadTbl1_Rotate);
	DDX_Control(pDX, IDC_EDIT_DCC_SH1_ROTATE, m_editDCC_LoadTbl1_Rotate);
	DDX_Control(pDX, IDC_EDIT_SPD_SH2_ROTATE, m_editSPD_LoadTbl2_Rotate);
	DDX_Control(pDX, IDC_EDIT_ACC_SH2_ROTATE, m_editACC_LoadTbl2_Rotate);
	DDX_Control(pDX, IDC_EDIT_DCC_SH2_ROTATE, m_editDCC_LoadTbl2_Rotate);
	DDX_Control(pDX, IDC_EDIT_SPD_SH3_ROTATE, m_editSPD_LoadTbl3_Rotate);
	DDX_Control(pDX, IDC_EDIT_ACC_SH3_ROTATE, m_editACC_LoadTbl3_Rotate);
	DDX_Control(pDX, IDC_EDIT_DCC_SH3_ROTATE, m_editDCC_LoadTbl3_Rotate);
	DDX_Control(pDX, IDC_EDIT_SPD_SH4_ROTATE, m_editSPD_LoadTbl4_Rotate);
	DDX_Control(pDX, IDC_EDIT_ACC_SH4_ROTATE, m_editACC_LoadTbl4_Rotate);
	DDX_Control(pDX, IDC_EDIT_DCC_SH4_ROTATE, m_editDCC_LoadTbl4_Rotate);

    DDX_Control(pDX, IDC_CHECK_ARRIVED_SEN, m_CtrlArrivedSen);
	DDX_Control(pDX, IDC_AFTER_MOVE_CHECK_ARRIVED_SEN, m_CtrlAfterMoveArrivedSen);
	DDX_Control(pDX, IDC_CHECK_IMPACT, m_BtnImpactTbl);
	DDX_Control(pDX, IDC_CHECK_2D_TEST_SITE_MOVE, m_Btn2dTestSiteMove);
	
	
	DDX_Text(pDX, IDC_EDIT_BARCODE_RETRY, m_nBarcodeRetry);
	DDX_Control(pDX, IDC_EDIT_BARCODE_RETRY, m_editBarcodRetry);

	DDX_Text(pDX, IDC_EDIT_VIB_RE_CNT, m_nVibratorConut);
	DDX_Text(pDX, IDC_EDIT_VIB_DELAY_TIME, m_dVibratorDelayTime);

// 	DDX_Control(pDX, IDC_RADIO_VIBRATOR_USE, m_rbChkVibUse);
// 	DDX_Control(pDX, IDC_RADIO_VIBRATOR_NOT_USE, m_rbChkVibNotUse);
	DDX_Radio(pDX, IDC_RADIO_VIBRATOR_NOT_USE, m_nViratorOption);

	DDX_Control(pDX, IDC_EDIT_VIB_RE_CNT, m_EditReCnt);
	DDX_Control(pDX, IDC_EDIT_VIB_DELAY_TIME, m_EditDelayTime);

	DDX_Control(pDX, IDC_EDIT_LOADTABLE_WAIT_TIME, m_EditWaitTime);
	

	DDX_Text(pDX, IDC_EDIT_BARCODE_LIMIT_SPD, m_nSPD_Y_Barcode_Limit);
	DDX_Control(pDX, IDC_EDIT_BARCODE_LIMIT_SPD, m_editSPD_Y_Barcode_Limit);

	DDX_Radio(pDX, IDC_RADIO_NUMBER_OF_LOADTABLE_4, m_nNumberOfLoadTable);


// 	DDX_Control(pDX, IDC_LIST_2D_MARK_1, m_EditList2DMark1);
// 	DDX_Control(pDX, IDC_LIST_2D_MARK_2, m_EditList2DMark2);
// 	DDX_Control(pDX, IDC_LIST_2D_MARK_3, m_EditList2DMark3);

}


BEGIN_MESSAGE_MAP(CSPD_TM_SHUTTLE, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CSPD_TM_SHUTTLE::OnBnClickedButtonSave)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_SH1_Y, &CSPD_TM_SHUTTLE::OnEnSetfocusEditSpdSh1Y)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_SH1_Y, &CSPD_TM_SHUTTLE::OnEnSetfocusEditAccSh1Y)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_SH1_Y, &CSPD_TM_SHUTTLE::OnEnSetfocusEditDccSh1Y)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_SH2_Y, &CSPD_TM_SHUTTLE::OnEnSetfocusEditSpdSh2Y)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_SH2_Y, &CSPD_TM_SHUTTLE::OnEnSetfocusEditAccSh2Y)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_SH2_Y, &CSPD_TM_SHUTTLE::OnEnSetfocusEditDccSh2Y)
	ON_EN_SETFOCUS(IDC_EDIT_BARCODE_RETRY, &CSPD_TM_SHUTTLE::OnEnSetfocusEditBarcodeRetry)
	ON_EN_SETFOCUS(IDC_EDIT_VIB_RE_CNT, &CSPD_TM_SHUTTLE::OnEnSetfocusEditVibratorCnt)
	ON_EN_SETFOCUS(IDC_EDIT_VIB_DELAY_TIME, &CSPD_TM_SHUTTLE::OnEnSetfocusEditVibratorDelayTime)
	ON_EN_SETFOCUS(IDC_EDIT_LOADTABLE_WAIT_TIME, &CSPD_TM_SHUTTLE::OnEnSetfocusEditLoadTableWaitTime)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_VIBRATOR_USE, IDC_RADIO_VIBRATOR_NOT_USE, &CSPD_TM_SHUTTLE::OnClickedVibratorUse)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_SH3_Y, &CSPD_TM_SHUTTLE::OnEnSetfocusEditSpdSh3Y)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_SH3_Y, &CSPD_TM_SHUTTLE::OnEnSetfocusEditAccSh3Y)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_SH3_Y, &CSPD_TM_SHUTTLE::OnEnSetfocusEditDccSh3Y)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_SH4_Y, &CSPD_TM_SHUTTLE::OnEnSetfocusEditSpdSh4Y)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_SH4_Y, &CSPD_TM_SHUTTLE::OnEnSetfocusEditAccSh4Y)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_SH4_Y, &CSPD_TM_SHUTTLE::OnEnSetfocusEditDccSh4Y)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_SH1_ROTATE, &CSPD_TM_SHUTTLE::OnEnSetfocusEditSpdSh1Rotate)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_SH1_ROTATE, &CSPD_TM_SHUTTLE::OnEnSetfocusEditAccSh1Rotate)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_SH1_ROTATE, &CSPD_TM_SHUTTLE::OnEnSetfocusEditDccSh1Rotate)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_SH2_ROTATE, &CSPD_TM_SHUTTLE::OnEnSetfocusEditSpdSh2Rotate)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_SH2_ROTATE, &CSPD_TM_SHUTTLE::OnEnSetfocusEditAccSh2Rotate)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_SH2_ROTATE, &CSPD_TM_SHUTTLE::OnEnSetfocusEditDccSh2Rotate)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_SH3_ROTATE, &CSPD_TM_SHUTTLE::OnEnSetfocusEditSpdSh3Rotate)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_SH3_ROTATE, &CSPD_TM_SHUTTLE::OnEnSetfocusEditAccSh3Rotate)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_SH3_ROTATE, &CSPD_TM_SHUTTLE::OnEnSetfocusEditDccSh3Rotate)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_SH4_ROTATE, &CSPD_TM_SHUTTLE::OnEnSetfocusEditSpdSh4Rotate)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_SH4_ROTATE, &CSPD_TM_SHUTTLE::OnEnSetfocusEditAccSh4Rotate)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_SH4_ROTATE, &CSPD_TM_SHUTTLE::OnEnSetfocusEditDccSh4Rotate)
	ON_EN_SETFOCUS(IDC_EDIT_BARCODE_LIMIT_SPD, &CSPD_TM_SHUTTLE::OnEnSetfocusEditBarcodeLimitSpd)
	ON_BN_CLICKED(IDC_BUTTON_TABLE_VACUUM_CHECK, &CSPD_TM_SHUTTLE::OnBnClickedButtonTableVacuumCheck)
	ON_BN_CLICKED(IDC_CHECK_ARRIVED_SEN, &CSPD_TM_SHUTTLE::OnBnClickedCheckArrivedSen)
	ON_BN_CLICKED(IDC_RADIO_NUMBER_OF_LOADTABLE_4, &CSPD_TM_SHUTTLE::OnBnClickedRadioNumberOfLoadtable4)
	ON_BN_CLICKED(IDC_RADIO_NUMBER_OF_LOADTABLE_8, &CSPD_TM_SHUTTLE::OnBnClickedRadioNumberOfLoadtable8)
END_MESSAGE_MAP()


// CSPD_TM_SHUTTLE 메시지 처리기입니다.


BOOL CSPD_TM_SHUTTLE::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnBtnGUI();
	OnEditBoxGUI();

    int nChkArrvied = (int)g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_LoadTblArrived);
    m_CtrlArrivedSen.SetCheck(nChkArrvied);

	int nChkArrivedAfterMove = (int)g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_LoadTbl_AfterMove_Arrived);
	m_CtrlAfterMoveArrivedSen.SetCheck(nChkArrivedAfterMove);


	int nImpactTbl = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_ImpactTbl);
	m_BtnImpactTbl.SetCheck(nImpactTbl);



	int n2DTestSiteMove = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_2D_TEST_SITE_MOVE);
	m_Btn2dTestSiteMove.SetCheck(n2DTestSiteMove);



	//BDM11_MD_2D_MARK_VALIDAATE_USE
	/*m_Btn2dTestSiteMove*/


	char szCompany[16] = { 0, };
	CString strCustomer = _T("");
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	int	nRetryCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_LOADING_TABLE_VIBRATOR_RETRY_CNT);
	double dDelayTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_LOADING_TABLE_VIBRATOR_DELAY_TIME);
	double dWaitTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_LOADING_TABLE_WAIT_TIME);
	int n2DIDLength = g_DMCont.m_dmHandlerOpt.GN(NDM11_LOADING_TABLE_2DID_LENGTH);

	int nVibratorOptionINI = g_DMCont.m_dmEQP.GN(NDM0_TABLE_VIBRATOR_OPTION);


	CString strReCnt, strDelayTime, strWaitTime;
	strReCnt.Format("%d", nRetryCnt);
	m_EditReCnt.SetWindowText(strReCnt);

	strDelayTime.Format("%.2f", dDelayTime);
	m_EditDelayTime.SetWindowText(strDelayTime);

	strWaitTime.Format("%.1f", dWaitTime);
	m_EditWaitTime.SetWindowText(strWaitTime);


	if (strcmp(szCompany, DEF_COMPANY_AMKOR) != 0)
	{
		GetDlgItem(IDC_STATIC_BARCODE_RTY)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_BARCODE_RETRY)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_TEXT_TMBAR)->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO_VIBRATOR_USE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_VIBRATOR_NOT_USE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_VIB_CNT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_VIB_RE_CNT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_VIB_DELAY_TIME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_VIB_DELAY_TIME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_GROUP_VIRATOR_OPTION)->ShowWindow(SW_HIDE);
	}

	if (m_nViratorOption == FALSE || nVibratorOptionINI == FALSE)
	{
		GetDlgItem(IDC_STATIC_VIB_CNT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_VIB_RE_CNT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_VIB_DELAY_TIME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_VIB_DELAY_TIME)->ShowWindow(SW_HIDE);

		if (nVibratorOptionINI == FALSE)
		{
			GetDlgItem(IDC_GROUP_VIRATOR_OPTION)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_RADIO_VIBRATOR_NOT_USE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_RADIO_VIBRATOR_USE)->ShowWindow(SW_HIDE);				
		}
	}


	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
	if (bLotState == eHandlerLotMode_OnGoing) {
		GetDlgItem(IDC_STATIC_NUMBER_OF_LOADTABLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_NUMBER_OF_LOADTABLE_4)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_NUMBER_OF_LOADTABLE_8)->EnableWindow(FALSE);
	}

	int nTestSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	if (nTestSiteY == STATION_MAX_Y_SITE)
	{
		GetDlgItem(IDC_STATIC_NUMBER_OF_LOADTABLE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_NUMBER_OF_LOADTABLE_4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_NUMBER_OF_LOADTABLE_8)->ShowWindow(SW_HIDE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSPD_TM_SHUTTLE::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);
}

void CSPD_TM_SHUTTLE::OnEditBoxGUI()
{
	m_editSPD_LoadTbl1_Y.SetBackColor(COLOR_WWHITE);
	m_editACC_LoadTbl1_Y.SetBackColor(COLOR_WWHITE);
	m_editDCC_LoadTbl1_Y.SetBackColor(COLOR_WWHITE);
	m_editSPD_LoadTbl2_Y.SetBackColor(COLOR_WWHITE);
	m_editACC_LoadTbl2_Y.SetBackColor(COLOR_WWHITE);
	m_editDCC_LoadTbl2_Y.SetBackColor(COLOR_WWHITE);
	m_editSPD_LoadTbl3_Y.SetBackColor(COLOR_WWHITE);
	m_editACC_LoadTbl3_Y.SetBackColor(COLOR_WWHITE);
	m_editDCC_LoadTbl3_Y.SetBackColor(COLOR_WWHITE);
	m_editSPD_LoadTbl4_Y.SetBackColor(COLOR_WWHITE);
	m_editACC_LoadTbl4_Y.SetBackColor(COLOR_WWHITE);
	m_editDCC_LoadTbl4_Y.SetBackColor(COLOR_WWHITE);

	m_editSPD_LoadTbl1_Rotate.SetBackColor(COLOR_WWHITE);
	m_editACC_LoadTbl1_Rotate.SetBackColor(COLOR_WWHITE);
	m_editDCC_LoadTbl1_Rotate.SetBackColor(COLOR_WWHITE);
	m_editSPD_LoadTbl2_Rotate.SetBackColor(COLOR_WWHITE);
	m_editACC_LoadTbl2_Rotate.SetBackColor(COLOR_WWHITE);
	m_editDCC_LoadTbl2_Rotate.SetBackColor(COLOR_WWHITE);
	m_editSPD_LoadTbl3_Rotate.SetBackColor(COLOR_WWHITE);
	m_editACC_LoadTbl3_Rotate.SetBackColor(COLOR_WWHITE);
	m_editDCC_LoadTbl3_Rotate.SetBackColor(COLOR_WWHITE);
	m_editSPD_LoadTbl4_Rotate.SetBackColor(COLOR_WWHITE);
	m_editACC_LoadTbl4_Rotate.SetBackColor(COLOR_WWHITE);
	m_editDCC_LoadTbl4_Rotate.SetBackColor(COLOR_WWHITE);


	m_editBarcodRetry.SetBackColor(COLOR_WWHITE);
	m_EditReCnt.SetBackColor(COLOR_WWHITE);
	m_EditDelayTime.SetBackColor(COLOR_WWHITE);
	m_EditWaitTime.SetBackColor(COLOR_WWHITE);

	m_editSPD_LoadTbl1_Y.SetTextSize(30);
	m_editACC_LoadTbl1_Y.SetTextSize(30);
	m_editDCC_LoadTbl1_Y.SetTextSize(30);
	m_editSPD_LoadTbl2_Y.SetTextSize(30);
	m_editACC_LoadTbl2_Y.SetTextSize(30);
	m_editDCC_LoadTbl2_Y.SetTextSize(30);
	m_editSPD_LoadTbl3_Y.SetTextSize(30);
	m_editACC_LoadTbl3_Y.SetTextSize(30);
	m_editDCC_LoadTbl3_Y.SetTextSize(30);
	m_editSPD_LoadTbl4_Y.SetTextSize(30);
	m_editACC_LoadTbl4_Y.SetTextSize(30);
	m_editDCC_LoadTbl4_Y.SetTextSize(30);

	m_editSPD_LoadTbl1_Rotate.SetTextSize(30);
	m_editACC_LoadTbl1_Rotate.SetTextSize(30);
	m_editDCC_LoadTbl1_Rotate.SetTextSize(30);
	m_editSPD_LoadTbl2_Rotate.SetTextSize(30);
	m_editACC_LoadTbl2_Rotate.SetTextSize(30);
	m_editDCC_LoadTbl2_Rotate.SetTextSize(30);
	m_editSPD_LoadTbl3_Rotate.SetTextSize(30);
	m_editACC_LoadTbl3_Rotate.SetTextSize(30);
	m_editDCC_LoadTbl3_Rotate.SetTextSize(30);
	m_editSPD_LoadTbl4_Rotate.SetTextSize(30);
	m_editACC_LoadTbl4_Rotate.SetTextSize(30);
	m_editDCC_LoadTbl4_Rotate.SetTextSize(30);

	m_editBarcodRetry.SetTextSize(30);
	m_EditReCnt.SetTextSize(30);
	m_EditDelayTime.SetTextSize(30);
	m_EditWaitTime.SetTextSize(30);

	m_editSPD_Y_Barcode_Limit.SetBackColor(COLOR_WWHITE);
	m_editSPD_Y_Barcode_Limit.SetTextSize(30);
}

void CSPD_TM_SHUTTLE::OnBnClickedButtonSave()
{
	UpdateData(TRUE);

	g_DMCont.m_dmShuttle.SN(NDM3_SPD_LoadTbl1_Y,m_nSPD_LoadTbl1_Y);
	g_DMCont.m_dmShuttle.SN(NDM3_ACC_LoadTbl1_Y,m_nACC_LoadTbl1_Y);
	g_DMCont.m_dmShuttle.SN(NDM3_DCC_LoadTbl1_Y,m_nDCC_LoadTbl1_Y);
	g_DMCont.m_dmShuttle.SN(NDM3_SPD_LoadTbl2_Y,m_nSPD_LoadTbl2_Y);
	g_DMCont.m_dmShuttle.SN(NDM3_ACC_LoadTbl2_Y,m_nACC_LoadTbl2_Y);
	g_DMCont.m_dmShuttle.SN(NDM3_DCC_LoadTbl2_Y,m_nDCC_LoadTbl2_Y);
	g_DMCont.m_dmShuttle.SN(NDM3_SPD_LoadTbl3_Y, m_nSPD_LoadTbl3_Y);
	g_DMCont.m_dmShuttle.SN(NDM3_ACC_LoadTbl3_Y, m_nACC_LoadTbl3_Y);
	g_DMCont.m_dmShuttle.SN(NDM3_DCC_LoadTbl3_Y, m_nDCC_LoadTbl3_Y);
	g_DMCont.m_dmShuttle.SN(NDM3_SPD_LoadTbl4_Y, m_nSPD_LoadTbl4_Y);
	g_DMCont.m_dmShuttle.SN(NDM3_ACC_LoadTbl4_Y, m_nACC_LoadTbl4_Y);
	g_DMCont.m_dmShuttle.SN(NDM3_DCC_LoadTbl4_Y, m_nDCC_LoadTbl4_Y);

	g_DMCont.m_dmShuttle.SN(NDM3_SPD_LoadTbl1_Rotate, m_nSPD_LoadTbl1_Rotate);
	g_DMCont.m_dmShuttle.SN(NDM3_ACC_LoadTbl1_Rotate, m_nACC_LoadTbl1_Rotate);
	g_DMCont.m_dmShuttle.SN(NDM3_DCC_LoadTbl1_Rotate, m_nDCC_LoadTbl1_Rotate);
	g_DMCont.m_dmShuttle.SN(NDM3_SPD_LoadTbl2_Rotate, m_nSPD_LoadTbl2_Rotate);
	g_DMCont.m_dmShuttle.SN(NDM3_ACC_LoadTbl2_Rotate, m_nACC_LoadTbl2_Rotate);
	g_DMCont.m_dmShuttle.SN(NDM3_DCC_LoadTbl2_Rotate, m_nDCC_LoadTbl2_Rotate);
	g_DMCont.m_dmShuttle.SN(NDM3_SPD_LoadTbl3_Rotate, m_nSPD_LoadTbl3_Rotate);
	g_DMCont.m_dmShuttle.SN(NDM3_ACC_LoadTbl3_Rotate, m_nACC_LoadTbl3_Rotate);
	g_DMCont.m_dmShuttle.SN(NDM3_DCC_LoadTbl3_Rotate, m_nDCC_LoadTbl3_Rotate);
	g_DMCont.m_dmShuttle.SN(NDM3_SPD_LoadTbl4_Rotate, m_nSPD_LoadTbl4_Rotate);
	g_DMCont.m_dmShuttle.SN(NDM3_ACC_LoadTbl4_Rotate, m_nACC_LoadTbl4_Rotate);
	g_DMCont.m_dmShuttle.SN(NDM3_DCC_LoadTbl4_Rotate, m_nDCC_LoadTbl4_Rotate);

	bool btest = (BOOL)m_CtrlArrivedSen.GetCheck();
    g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_LoadTblArrived, (BOOL)m_CtrlArrivedSen.GetCheck());
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_LoadTbl_AfterMove_Arrived, (BOOL)m_CtrlAfterMoveArrivedSen.GetCheck());
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_ImpactTbl, (BOOL)m_BtnImpactTbl.GetCheck());
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_2D_TEST_SITE_MOVE, (BOOL)m_Btn2dTestSiteMove.GetCheck());
	g_DMCont.m_dmHandlerOpt.SN(NDM11_NUMBER_OF_LOADTABLE, m_nNumberOfLoadTable);
	
	g_DMCont.m_dmShuttle.SN(NDM3_LdTbl_BarcodeMaxRetry, m_nBarcodeRetry);

	g_DMCont.m_dmShuttle.SN(NDM3_SPD_Y_Barcode_Limit, m_nSPD_Y_Barcode_Limit);

	g_DMCont.m_dmHandlerOpt.SN(NDM11_LOADING_TABLE_VIBRATOR_OPTION, m_nViratorOption);
	int nRetryCnt = GetDlgItemInt(IDC_EDIT_VIB_RE_CNT);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_LOADING_TABLE_VIBRATOR_RETRY_CNT, nRetryCnt);
	CString strDelayTime;
	GetDlgItemText(IDC_EDIT_VIB_DELAY_TIME, strDelayTime);
	double dDelayTime = atof(strDelayTime);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_LOADING_TABLE_VIBRATOR_DELAY_TIME, dDelayTime);

	CString strWaitTime;
	GetDlgItemText(IDC_EDIT_LOADTABLE_WAIT_TIME, strWaitTime);
	double dWaitTime = atof(strWaitTime);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_LOADING_TABLE_WAIT_TIME, dWaitTime);

	char szCokName[512] = {0,};
	ST_COK_INFO     stCokData, stBeforeCokData;
    ST_BASIC_INFO   stBasicData, stBeforeBasicData;
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));	
	g_COK_FileMgr.LoadCokInfo(szCokName  , stCokData);
    g_COK_FileMgr.LoadBasicInfo(szCokName, stBasicData);



	stBeforeCokData = stCokData;
	stBeforeBasicData = stBasicData;

	int nPosNum[eMaxTblCount] = {NDM3_SPD_LoadTbl1_Y, NDM3_SPD_LoadTbl2_Y, NDM3_SPD_LoadTbl3_Y, NDM3_SPD_LoadTbl4_Y };
	int nPosRotate[eMaxTblCount] = { NDM3_SPD_LoadTbl1_Rotate, NDM3_SPD_LoadTbl2_Rotate, NDM3_SPD_LoadTbl3_Rotate, NDM3_SPD_LoadTbl4_Rotate };
	for(int i=0; i<eMaxTblCount; i++)
	{
		stCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Y].nSPD = g_DMCont.m_dmShuttle.GN(nPosNum[i]);
		stCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Y].nACC = g_DMCont.m_dmShuttle.GN(nPosNum[i]+1);
		stCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Y].nDCC = g_DMCont.m_dmShuttle.GN(nPosNum[i]+2);

		stCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Rotate].nSPD = g_DMCont.m_dmShuttle.GN(nPosRotate[i]);
		stCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Rotate].nACC = g_DMCont.m_dmShuttle.GN(nPosRotate[i] + 1);
		stCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Rotate].nDCC = g_DMCont.m_dmShuttle.GN(nPosRotate[i] + 2);
	}

	stCokData.nLoadTbl_SPD_Barcode_Limit = g_DMCont.m_dmShuttle.GN(NDM3_SPD_Y_Barcode_Limit);

	g_COK_FileMgr.SaveCokInfo(szCokName, &stCokData);

    stBasicData.bLoadTblArrived = m_CtrlArrivedSen.GetCheck();
	stBasicData.bLoadTblAfterMoveArrived = m_CtrlAfterMoveArrivedSen.GetCheck();
	stBasicData.bImpactTbl      = m_BtnImpactTbl.GetCheck();
	stBasicData.b2DTestSiteMove = m_Btn2dTestSiteMove.GetCheck();
	stBasicData.nBarcodeRetryCnt = m_nBarcodeRetry;
	stBasicData.nLoadTableVibratorChkOpt = m_nViratorOption;
	stBasicData.nLoadTableVibratorRetryCnt = nRetryCnt;
	stBasicData.dLoadTableVibratorDelayTime = dDelayTime;

	stBasicData.dLoadTableWaitTime = dWaitTime;

	stBasicData.nNumberOfLoadTable = m_nNumberOfLoadTable;

    g_COK_FileMgr.SaveBasicInfo(szCokName, &stBasicData);

	//GUI Data Change Log
	
	std::vector<std::pair<CString,std::pair<double,double>>> vData;  // vData.first = stCokData, vData.second.first = BeforeData vData.second.second = AfterData
	
	stBeforeCokData.IsSameLoadTable(stCokData, vData);
	stBeforeBasicData.IsSameLoadTable(stBasicData, vData);

	if(!(vData.empty()))
	{
		LoadTableSpeedTiemChangeLog(vData);
	}
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditSpdSh1Y()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nSPD_LoadTbl1_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #1 Y Speed", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_SPD_SH1_Y)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditAccSh1Y()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nACC_LoadTbl1_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #1 Y Accel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_ACC_SH1_Y)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditDccSh1Y()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nDCC_LoadTbl1_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #1 Y Decel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DCC_SH1_Y)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}	
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditSpdSh2Y()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nSPD_LoadTbl2_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #2 Y Speed", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_SPD_SH2_Y)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditAccSh2Y()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nACC_LoadTbl2_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #2 Y Accel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_ACC_SH2_Y)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditDccSh2Y()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nDCC_LoadTbl2_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #2 Y Decel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DCC_SH2_Y)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}

void CSPD_TM_SHUTTLE::OnEnSetfocusEditSpdSh3Y()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_LoadTbl3_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #3 Y Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_SH3_Y)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditAccSh3Y()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_LoadTbl3_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #3 Y Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_SH3_Y)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditDccSh3Y()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_LoadTbl3_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #3 Y Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_SH3_Y)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditSpdSh4Y()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_LoadTbl4_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #4 Y Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_SH4_Y)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditAccSh4Y()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_LoadTbl4_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #4 Y Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_SH4_Y)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditDccSh4Y()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_LoadTbl4_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #4 Y Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_SH4_Y)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::MakeLog(LPCTSTR fmt, ...)
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
void CSPD_TM_SHUTTLE::LoadTableSpeedTiemChangeLog(std::vector<std::pair<CString,std::pair<double,double>>> vData)
{
	for(int i = 0 ; i< (int)vData.size(); i++)
	{
		std::pair<CString,std::pair<double,double>> p;

		p = vData[i];

		if(p.first == "Arrived"|| p.first == "After Move Arrived" || p.first == "Impact Load Table" /*p.first == "2D Barcode" || p.first == "No Duplication Check 2D Barcode"*/)
		{
			if(p.second.second == 0 ) // NoUse
			{
				MakeLog("[MENU = Loading Table Speed & Time, DATA NAME = %s ][BEFORE= Use , AFTER= NoUse ]", p.first);
			}
			else if(p.second.second == 1 )
			{
				MakeLog("[MENU = Loading Table Speed & Time, DATA NAME = %s ][BEFORE= NoUse , AFTER= Use ]", p.first);
			}
		}
		else
		{
			MakeLog("[MENU = Loading Table Speed & Time, DATA NAME = %s ][BEFORE=%.2f, AFTER=%.2f]", p.first, p.second.first, p.second.second);
		}
	}
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditBarcodeRetry()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nBarcodeRetry, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "BarcodeRetry", "5", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_BARCODE_RETRY)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}

void CSPD_TM_SHUTTLE::OnBnClickedRadioVibratorUse()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nViratorOption = CHECK_VIBRATOR_USE;
	GetDlgItem(IDC_STATIC_VIB_CNT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_VIB_RE_CNT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_VIB_DELAY_TIME)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_VIB_DELAY_TIME)->ShowWindow(SW_SHOW);
}

void CSPD_TM_SHUTTLE::OnEnSetfocusEditVibratorCnt()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nVibratorConut, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Vibrator Count", "50", "1");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_VIB_RE_CNT)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}

void CSPD_TM_SHUTTLE::OnEnSetfocusEditVibratorDelayTime()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_dVibratorDelayTime, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Vibrator Delay Time", "0.3", "0.05");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_VIB_DELAY_TIME)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::OnBnClickedRadioVibratorNotUse()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nViratorOption = CHECK_VIBRATOR_NOT_USE;
	GetDlgItem(IDC_STATIC_VIB_CNT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_VIB_RE_CNT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_VIB_DELAY_TIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_VIB_DELAY_TIME)->ShowWindow(SW_HIDE);

	int nUse = g_DMCont.m_dmHandlerOpt.GN(NDM11_LOADING_TABLE_VIBRATOR_OPTION);
	int nReCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_LOADING_TABLE_VIBRATOR_RETRY_CNT);
	double bDeleyTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_LOADING_TABLE_VIBRATOR_DELAY_TIME);
}

void CSPD_TM_SHUTTLE::OnClickedVibratorUse(UINT nID)
{
	switch (nID)
	{
	case IDC_RADIO_VIBRATOR_USE:
		GetDlgItem(IDC_STATIC_VIB_CNT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_VIB_RE_CNT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_VIB_DELAY_TIME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_VIB_DELAY_TIME)->ShowWindow(SW_SHOW);
		break;
	case IDC_RADIO_VIBRATOR_NOT_USE:
		GetDlgItem(IDC_STATIC_VIB_CNT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_VIB_RE_CNT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_VIB_DELAY_TIME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_VIB_DELAY_TIME)->ShowWindow(SW_HIDE);
		break;
	}
}

void CSPD_TM_SHUTTLE::OnEnSetfocusEditLoadTableWaitTime()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_dLoadTableWaitTime, szVal, sizeof(szVal));
	//3, 0
	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table Wait Time", "3", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_LOADTABLE_WAIT_TIME)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}




void CSPD_TM_SHUTTLE::OnEnSetfocusEditSpdSh1Rotate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_LoadTbl1_Rotate, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #1 Rotate Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_SH1_ROTATE)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditAccSh1Rotate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_LoadTbl1_Rotate, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #1 Rotate Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_SH1_ROTATE)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditDccSh1Rotate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_LoadTbl1_Rotate, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #1 Rotate Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_SH1_ROTATE)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditSpdSh2Rotate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_LoadTbl2_Rotate, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #2 Rotate Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_SH2_ROTATE)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditAccSh2Rotate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_LoadTbl2_Rotate, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #2 Rotate Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_SH2_ROTATE)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditDccSh2Rotate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_LoadTbl2_Rotate, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #2 Rotate Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_SH2_ROTATE)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditSpdSh3Rotate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_LoadTbl3_Rotate, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #3 Rotate Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_SH3_ROTATE)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditAccSh3Rotate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_LoadTbl3_Rotate, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #3 Rotate Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_SH3_ROTATE)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditDccSh3Rotate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_LoadTbl3_Rotate, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #3 Rotate Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_SH3_ROTATE)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditSpdSh4Rotate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_LoadTbl4_Rotate, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #4 Rotate Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_SH4_ROTATE)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditAccSh4Rotate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_LoadTbl4_Rotate, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #4 Rotate Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_SH4_ROTATE)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditDccSh4Rotate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_LoadTbl4_Rotate, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table #4 Rotate Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_SH4_ROTATE)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::OnEnSetfocusEditBarcodeLimitSpd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_Y_Barcode_Limit, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Table Barcode Limit SPD", "100", "10");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_BARCODE_LIMIT_SPD)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_SHUTTLE::OnBnClickedButtonTableVacuumCheck()
{
	CDlgTableVacuumCheck dlg;
	dlg.DoModal();
}


void CSPD_TM_SHUTTLE::OnBnClickedCheckArrivedSen()
{
	// After Move Enable/Disable
	// Arrived Sen기능 사용 시에만 After Move 기능도 사용 가능.

	BOOL bArrived = m_CtrlArrivedSen.GetCheck();
	
	GetDlgItem(IDC_AFTER_MOVE_CHECK_ARRIVED_SEN)->EnableWindow(bArrived);
}




void CSPD_TM_SHUTTLE::OnBnClickedRadioNumberOfLoadtable4()
{
	m_nNumberOfLoadTable = eNumberOfLoadTable_4Unit;
}


void CSPD_TM_SHUTTLE::OnBnClickedRadioNumberOfLoadtable8()
{
	m_nNumberOfLoadTable = eNumberOfLoadTable_8Unit;
}
