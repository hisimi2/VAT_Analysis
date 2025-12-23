// YieldSetDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "YieldSetDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CYieldSetDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CYieldSetDlg, CDialog)

CYieldSetDlg::CYieldSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CYieldSetDlg::IDD, pParent)
{

// 	m_nContinueFailCnt  = g_DMCont.m_dmHandlerOpt.GN(NDM11_ContinueFail_FailCnt	);
// 	m_nContinueResetCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_ContinueFail_ResetCnt	);
// 	m_nCtrlWindow       = g_DMCont.m_dmHandlerOpt.GN(NDM11_YieldCtrl_CtrlWindow	);	
	m_dDropHeight       = (double)(g_DMCont.m_dmHandlerOpt.GN(NDM11_DropHeight		)/1000.0);	
	m_dDropTime         = (double)(g_DMCont.m_dmHandlerOpt.GN(NDM11_TM_ContactorDropTimer)/1000.0);	

// 	m_bContinueFailMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_ConinueFail		);
// 	m_bYieldCtrlMode    = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_YieldCtrl			);
	m_bContactMode      = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Contact			);	

/*	m_dYieldLimit       = g_DMCont.m_dmHandlerOpt.GD(DDM11_YieldCtrl_YieldLimit	);	*/
	
	m_dForceGF			= g_DMCont.m_dmHandlerOpt.GD(DDM11_DvcContactForce_GF);
	m_nForcePins		= g_DMCont.m_dmHandlerOpt.GN(NDM11_DvcContactForce_Pin);

	m_dDvcDimenX		= (double)g_DMCont.m_dmHandlerOpt.GN(NDM11_DVC_Dimen_X)/1000.0;
	m_dDvcDimenY		= (double)g_DMCont.m_dmHandlerOpt.GN(NDM11_DVC_Dimen_Y)/1000.0;

	memset(m_dContactPos, 0x00, sizeof(m_dContactPos));
	memset(m_dContactPosBefore, 0x00, sizeof(m_dContactPosBefore));
}

CYieldSetDlg::~CYieldSetDlg()
{
}

void CYieldSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_YIELD_SAVE, m_BtnSave);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);	
	
	DDX_Text(pDX, IDC_EDIT_PINS, m_nForcePins);
	DDX_Text(pDX, IDC_EDIT_PIN_GF, m_dForceGF);
	DDX_Control(pDX, IDC_EDIT_PINS, m_EditForcePins);
	DDX_Control(pDX, IDC_EDIT_PIN_GF, m_EditForceGF);
	DDX_Control(pDX, IDC_EDIT_CALC_DVC_KGF, m_EditContacForce);
	DDX_Control(pDX, IDC_EDIT_DVC_DIMEN_X, m_EditDvcDimenX);
	DDX_Control(pDX, IDC_EDIT_DVC_DIMEN_Y, m_EditDvcDimenY);	
	DDX_Text(pDX, IDC_EDIT_DVC_DIMEN_X, m_dDvcDimenX);
	DDX_Text(pDX, IDC_EDIT_DVC_DIMEN_Y, m_dDvcDimenY);

	DDX_Control(pDX, IDC_EDIT_CONTACT_POS1, m_EditContactPos1);
	DDX_Control(pDX, IDC_EDIT_CONTACT_POS2, m_EditContactPos2);
	DDX_Control(pDX, IDC_EDIT_CONTACT_POS3, m_EditContactPos3);
	DDX_Control(pDX, IDC_EDIT_CONTACT_POS4, m_EditContactPos4);
	DDX_Control(pDX, IDC_EDIT_CONTACT_POS5, m_EditContactPos5);
	DDX_Control(pDX, IDC_EDIT_CONTACT_POS6, m_EditContactPos6);
	DDX_Control(pDX, IDC_EDIT_CONTACT_POS7, m_EditContactPos7);
	DDX_Control(pDX, IDC_EDIT_CONTACT_POS8, m_EditContactPos8);

	DDX_Control(pDX, IDC_EDIT_HEIGHT_CAL_DATA1, m_EditHeightCalData1);
	DDX_Control(pDX, IDC_EDIT_HEIGHT_CAL_DATA2, m_EditHeightCalData2);
	DDX_Control(pDX, IDC_EDIT_HEIGHT_CAL_DATA3, m_EditHeightCalData3);
	DDX_Control(pDX, IDC_EDIT_HEIGHT_CAL_DATA4, m_EditHeightCalData4);
	DDX_Control(pDX, IDC_EDIT_HEIGHT_CAL_DATA5, m_EditHeightCalData5);
	DDX_Control(pDX, IDC_EDIT_HEIGHT_CAL_DATA6, m_EditHeightCalData6);
	DDX_Control(pDX, IDC_EDIT_HEIGHT_CAL_DATA7, m_EditHeightCalData7);
	DDX_Control(pDX, IDC_EDIT_HEIGHT_CAL_DATA8, m_EditHeightCalData8);
}


BEGIN_MESSAGE_MAP(CYieldSetDlg, CDialog)
	ON_BN_CLICKED(IDC_YIELD_SAVE, &CYieldSetDlg::OnBnClickedYieldSave)
	ON_BN_CLICKED(IDC_RADIO_CONTACT1, &CYieldSetDlg::OnBnClickedRadioContact1)
	ON_BN_CLICKED(IDC_RADIO_CONTACT2, &CYieldSetDlg::OnBnClickedRadioContact2)
// 	ON_EN_SETFOCUS(IDC_EDIT_CONTI_CNT, &CYieldSetDlg::OnEnSetfocusEditContiCnt)
// 	ON_EN_SETFOCUS(IDC_EDIT_RESET_CNT, &CYieldSetDlg::OnEnSetfocusEditResetCnt)
// 	ON_EN_SETFOCUS(IDC_EDIT_YIELD_LIMIT, &CYieldSetDlg::OnEnSetfocusEditYieldLimit)
// 	ON_EN_SETFOCUS(IDC_EDIT_CTRL_WINDOW, &CYieldSetDlg::OnEnSetfocusEditCtrlWindow)
	ON_EN_SETFOCUS(IDC_EDIT_DROP_HEIGHT, &CYieldSetDlg::OnEnSetfocusEditDropHeight)
	ON_EN_SETFOCUS(IDC_EDIT_DROP_TIMER, &CYieldSetDlg::OnEnSetfocusEditDropTimer)
	ON_EN_SETFOCUS(IDC_EDIT_PINS, &CYieldSetDlg::OnEnSetfocusEditPins)
	ON_EN_SETFOCUS(IDC_EDIT_PIN_GF, &CYieldSetDlg::OnEnSetfocusEditPinGf)
	ON_EN_SETFOCUS(IDC_EDIT_DVC_DIMEN_X, &CYieldSetDlg::OnEnSetfocusEditDvcDimenX)
	ON_EN_SETFOCUS(IDC_EDIT_DVC_DIMEN_Y, &CYieldSetDlg::OnEnSetfocusEditDvcDimenY)
	ON_EN_SETFOCUS(IDC_EDIT_CONTACT_POS1, &CYieldSetDlg::OnEnSetfocusEditContactPos1)
	ON_EN_SETFOCUS(IDC_EDIT_CONTACT_POS2, &CYieldSetDlg::OnEnSetfocusEditContactPos2)
	ON_EN_SETFOCUS(IDC_EDIT_CONTACT_POS3, &CYieldSetDlg::OnEnSetfocusEditContactPos3)
	ON_EN_SETFOCUS(IDC_EDIT_CONTACT_POS4, &CYieldSetDlg::OnEnSetfocusEditContactPos4)
	ON_EN_SETFOCUS(IDC_EDIT_CONTACT_POS5, &CYieldSetDlg::OnEnSetfocusEditContactPos5)
	ON_EN_SETFOCUS(IDC_EDIT_CONTACT_POS6, &CYieldSetDlg::OnEnSetfocusEditContactPos6)
	ON_EN_SETFOCUS(IDC_EDIT_CONTACT_POS7, &CYieldSetDlg::OnEnSetfocusEditContactPos7)
	ON_EN_SETFOCUS(IDC_EDIT_CONTACT_POS8, &CYieldSetDlg::OnEnSetfocusEditContactPos8)

END_MESSAGE_MAP()


// CYieldSetDlg 메시지 처리기입니다.


BOOL CYieldSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnBtnGUI();
	OnEditBoxGUI();
	SetBtnRadioContact();
	OnUpdateForceData();
	LoadPressContactData();
	LoadPressHeightCalData();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CYieldSetDlg::OnBnClickedYieldSave()
{
	UpdateData( TRUE );
	g_DMCont.m_dmHandlerOpt.SB( BDM11_MD_Contact, m_bContactMode );

	g_DMCont.m_dmHandlerOpt.SN( NDM11_DropHeight, ( int )( m_dDropHeight * 1000 ) );
	g_DMCont.m_dmHandlerOpt.SN( NDM11_TM_ContactorDropTimer, ( int )( m_dDropTime * 1000 ) );

	g_DMCont.m_dmHandlerOpt.SD( DDM11_DvcContactForce_GF, m_dForceGF );
	g_DMCont.m_dmHandlerOpt.SN( NDM11_DvcContactForce_Pin, m_nForcePins );

	CString strForce = _T( "" );
	GetDlgItem( IDC_EDIT_CALC_DVC_KGF )->GetWindowText( strForce );
	g_DMCont.m_dmHandlerOpt.SD( DDM11_ContactForce_gf, atof( strForce ) * 1000.0 );

	//  [12/28/2020 jaehoon.jung]
	if (atof(strForce) > 15.0)
	{
		AfxMessageBox("Contact Force가 Max 범위를 초과하였습니다.");
		return;
	}

	g_DMCont.m_dmHandlerOpt.SN( NDM11_DVC_Dimen_X, ( int )( m_dDvcDimenX * 1000 ) );
	g_DMCont.m_dmHandlerOpt.SN( NDM11_DVC_Dimen_Y, ( int )( m_dDvcDimenY * 1000 ) );

	char szCokName[ 512 ] = { 0, };
	ST_BASIC_INFO   stBasicData, stBeforeBasicData;
	g_DMCont.m_dmHandlerOpt.GS( SDM11_TrayFileName, szCokName, sizeof( szCokName ) );
	g_COK_FileMgr.LoadBasicInfo( szCokName, stBasicData );

	stBeforeBasicData = stBasicData;

	stBasicData.bContactMode = m_bContactMode;
	stBasicData.nDropHeight = ( int )( m_dDropHeight * 1000 );
	stBasicData.nDropTimer = ( int )( m_dDropTime * 1000 );
	stBasicData.nContactForcePins = m_nForcePins;
	stBasicData.dContactcForceGF = m_dForceGF;

	stBasicData.nDvcDimensionX = ( int )( m_dDvcDimenX * 1000 );
	stBasicData.nDvcDimensionY = ( int )( m_dDvcDimenY * 1000 );

	g_COK_FileMgr.SaveBasicInfo( szCokName, &stBasicData );

	SavePressContactData();

	//GUI Data change Log
	std::vector<std::pair<std::pair<CString, int>, std::pair<double, double>>> vData;

	stBeforeBasicData.IsSameYieldSet( stBasicData, vData );

	if( !( vData.empty() ) )
	{
		ConrtactSettingChangeLog( vData );
	}
}


void CYieldSetDlg::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);	
}

void CYieldSetDlg::OnEditBoxGUI()
{

	m_EditForcePins			.SetBackColor(COLOR_WWHITE);
	m_EditForceGF			.SetBackColor(COLOR_WWHITE);
	m_EditContacForce		.SetBackColor(COLOR_WWHITE);
	m_EditDvcDimenX			.SetBackColor(COLOR_WWHITE);
	m_EditDvcDimenY			.SetBackColor(COLOR_WWHITE);
	m_EditContactPos1       .SetBackColor(COLOR_WWHITE);
	m_EditContactPos2       .SetBackColor(COLOR_WWHITE);
	m_EditContactPos3       .SetBackColor(COLOR_WWHITE);
	m_EditContactPos4       .SetBackColor(COLOR_WWHITE);
	m_EditContactPos5       .SetBackColor(COLOR_WWHITE);
	m_EditContactPos6       .SetBackColor(COLOR_WWHITE);
	m_EditContactPos7       .SetBackColor(COLOR_WWHITE);
	m_EditContactPos8       .SetBackColor(COLOR_WWHITE);

	m_EditForcePins			.SetTextSize(30);
	m_EditForceGF			.SetTextSize(30);
	m_EditContacForce		.SetTextSize(30);
	m_EditDvcDimenX			.SetTextSize(30);
	m_EditDvcDimenY			.SetTextSize(30);
	m_EditContactPos1		.SetTextSize(30);
	m_EditContactPos2		.SetTextSize(30);
	m_EditContactPos3		.SetTextSize(30);
	m_EditContactPos4		.SetTextSize(30);
	m_EditContactPos5		.SetTextSize(30);
	m_EditContactPos6		.SetTextSize(30);
	m_EditContactPos7		.SetTextSize(30);
	m_EditContactPos8		.SetTextSize(30);


	CEditTrans *editHeightCal[eMaxPressUnitCount] = { &m_EditHeightCalData1, &m_EditHeightCalData2, &m_EditHeightCalData3,&m_EditHeightCalData4,
													  &m_EditHeightCalData5, &m_EditHeightCalData6, &m_EditHeightCalData7, &m_EditHeightCalData8};

	for (int i = 0; i < eMaxPressUnitCount; i++) {
		editHeightCal[i]->SetBackColor(COLOR_WWHITE);
		editHeightCal[i]->SetTextSize(30);
		editHeightCal[i]->EnableWindow(FALSE);
	}
}

void CYieldSetDlg::SetBtnRadioContact()
{
// 	if(m_bContactMode == eCONTACT_DIRECT){
// 		m_BtnDirect.SetShade(CShadeButtonST::SHS_METAL,8,20,0,RGB(55,55,255));
// 	}else{
// 		m_BtnDirect.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
// 	}
// 	m_BtnDirect.SetIcon(IDI_BTN_DIRECT_MODE);
// 	m_BtnDirect.SetAlign(CButtonST::ST_ALIGN_VERT);
// 
// 
// 	if(m_bContactMode == eCONTACT_DROP){
// 		m_BtnDrop.SetShade(CShadeButtonST::SHS_METAL,8,20,0,RGB(55,55,255));
// 	}else{
// 		m_BtnDrop.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
// 	}	
// 	m_BtnDrop.SetIcon(IDI_BTN_DROP_MODE);
// 	m_BtnDrop.SetAlign(CButtonST::ST_ALIGN_VERT);
}


void CYieldSetDlg::OnBnClickedRadioContact1()
{
	m_bContactMode = eCONTACT_DIRECT;
	SetBtnRadioContact();
}


void CYieldSetDlg::OnBnClickedRadioContact2()
{
	m_bContactMode = eCONTACT_DROP;
	SetBtnRadioContact();
}


// void CYieldSetDlg::OnEnSetfocusEditContiCnt()
// {
// 	char szVal[10]    = {0,};
// 	char szRetVal[64] = {0,};
// 	_itoa_s(m_nContinueFailCnt, szVal, sizeof(szVal));
// 
// 	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Continue fail - Fail count", "1000", "0");
// 
// 	if(nRet == RETURN_OK){
// 		GetDlgItem(IDC_EDIT_CONTI_CNT)->SetWindowText(szRetVal);
// 	}
// }
// 
// 
// void CYieldSetDlg::OnEnSetfocusEditResetCnt()
// {
// 	char szVal[10]    = {0,};
// 	char szRetVal[64] = {0,};
// 	_itoa_s(m_nContinueResetCnt, szVal, sizeof(szVal));
// 
// 	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Continue fail - Reset count", "1000", "0");
// 
// 	if(nRet == RETURN_OK){
// 		GetDlgItem(IDC_EDIT_RESET_CNT)->SetWindowText(szRetVal);
// 	}
// }
// 
//  
// void CYieldSetDlg::OnEnSetfocusEditYieldLimit()
// {
// 	char szVal[10]    = {0,};
// 	char szRetVal[64] = {0,};
// 	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dYieldLimit);
// 
// 	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Yield Control - Yield Limitation", "100", "0");
// 
// 	if(nRet == RETURN_OK){
// 		GetDlgItem(IDC_EDIT_YIELD_LIMIT)->SetWindowText(szRetVal);
// 	}
// }
// 
// 
// void CYieldSetDlg::OnEnSetfocusEditCtrlWindow()
// {
// 	char szVal[10]    = {0,};
// 	char szRetVal[64] = {0,};
// 	_itoa_s(m_nCtrlWindow, szVal, sizeof(szVal));
// 
// 	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Yield Control - Control Window", "1000", "0");
// 
// 	if(nRet == RETURN_OK){
// 		GetDlgItem(IDC_EDIT_CTRL_WINDOW)->SetWindowText(szRetVal);
// 	}
// }


void CYieldSetDlg::OnEnSetfocusEditDropHeight()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dDropHeight);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Drop Height", "20", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DROP_HEIGHT)->SetWindowText(szRetVal);
	}
}


void CYieldSetDlg::OnEnSetfocusEditDropTimer()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dDropTime);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Drop Timer", "20", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DROP_TIMER)->SetWindowText(szRetVal);
	}
}


void CYieldSetDlg::OnEnSetfocusEditPins()
{
	CString strOldData = _T("");
	GetDlgItem(IDC_EDIT_PINS)->GetWindowText(strOldData);

	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nForcePins, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Contact Force Pins(Balls)", "1000", "0");

	CString strGF   = _T("");
	GetDlgItem(IDC_EDIT_PIN_GF)->GetWindowText(strGF);

	double dForce = atof(strGF) * atof(szRetVal) / 1000.0;

	int nStationSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nStationSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	double dCalcForce = dForce * nStationSiteX * nStationSiteY;
	
	if(nRet == RETURN_OK && dCalcForce <= 50.0){
		GetDlgItem(IDC_EDIT_PINS)->SetWindowText(szRetVal);
	}else{
		AfxMessageBox(_T("Contact Force is over limit KG"), MB_TOPMOST);
		GetDlgItem(IDC_EDIT_PINS)->SetWindowText(strOldData);
	}

	OnUpdateForceData();
}


void CYieldSetDlg::OnEnSetfocusEditPinGf()
{
	CString strOldData = _T("");
	GetDlgItem(IDC_EDIT_PIN_GF)->GetWindowText(strOldData);

	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dForceGF);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Contact Force GF", "9999", "0");

	CString strPins = _T("");
	GetDlgItem(IDC_EDIT_PINS)->GetWindowText(strPins);

	double dForce = atof(szRetVal) * atof(strPins) / 1000.0;

	int nStationSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nStationSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	double dCalcForce = dForce * nStationSiteX * nStationSiteY;

	if(nRet == RETURN_OK && dCalcForce <=  80.0 /*50.0*/) //dForce <= 60.0)
	{
		GetDlgItem(IDC_EDIT_PIN_GF)->SetWindowText(szRetVal);
	}else{

		AfxMessageBox(_T("Contact Force is over limit KG"), MB_TOPMOST);
		GetDlgItem(IDC_EDIT_PIN_GF)->SetWindowText(strOldData);
	}

	OnUpdateForceData();
}

void CYieldSetDlg::OnUpdateForceData()
{
	CString strPins = _T("");
	CString strGF   = _T("");
	GetDlgItem(IDC_EDIT_PINS)->GetWindowText(strPins);
	GetDlgItem(IDC_EDIT_PIN_GF)->GetWindowText(strGF);

	double dForce = atof(strGF) * atof(strPins) / 1000.0;

	CString strForceData = _T("");
	strForceData.Format("%.3f", (dForce >= 50 ? 50:dForce));
	GetDlgItem(IDC_EDIT_CALC_DVC_KGF)->SetWindowText(strForceData);
}

void CYieldSetDlg::OnEnSetfocusEditDvcDimenX()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dDvcDimenX);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Device Dimension X", "100", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DVC_DIMEN_X)->SetWindowText(szRetVal);
	}
}


void CYieldSetDlg::OnEnSetfocusEditDvcDimenY()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dDvcDimenY);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Device Dimension Y", "100", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DVC_DIMEN_Y)->SetWindowText(szRetVal);
	}
}

void CYieldSetDlg::LoadPressContactData()
{
	char szDriverFileName[512]={0,};
	char szDrvFile[512]={0,};
	char szCokName[128]={0,};
	char szTeachFile[512]={0,};
	double dContactPos[eMaxPressUnitCount] ={0,};

	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));

	GetCurrentDirectory(sizeof(szDriverFileName), szDriverFileName);
	sprintf_s(szDrvFile, sizeof(szDrvFile), "%s\\sys_data\\", szDriverFileName);

	sprintf_s(szTeachFile, sizeof(szTeachFile), "%s%s\\%s\\%s\\%s", 
	szDrvFile, SZ_HANDLER_RCP_FOLDER, SZ_TRAY_FOLDER, szCokName, SZ_TEACHING_FILE_NAME);

	CConfigData CConfig(szTeachFile);
	
	for(int i=RIDX_PRESS_UNIT_1; i<eMaxPressUnitCount; i++){
		CString strTitle = _T("");
		char szTitle[64] ={0,};

		if( i > RIDX_PRESS_UNIT_8){
			strTitle.Format("LOCATION TEACHING TABLE - Press Unit Up %d",i-7);
		}else{
			strTitle.Format("LOCATION TEACHING TABLE - Press Unit Down %d",i+1);
		}
		sprintf_s(szTitle,sizeof(szTitle),strTitle);
		dContactPos[i] = CConfig.GetDouble(szTitle,"Contact","0");
	}
	int nEditId[eMaxPressUnitCount] = { IDC_EDIT_CONTACT_POS1,IDC_EDIT_CONTACT_POS2,IDC_EDIT_CONTACT_POS3,IDC_EDIT_CONTACT_POS4,IDC_EDIT_CONTACT_POS5,IDC_EDIT_CONTACT_POS6,IDC_EDIT_CONTACT_POS7,
					  IDC_EDIT_CONTACT_POS8 }; //,IDC_EDIT_CONTACT_POS9,IDC_EDIT_CONTACT_POS10,IDC_EDIT_CONTACT_POS11,IDC_EDIT_CONTACT_POS12,IDC_EDIT_CONTACT_POS13,IDC_EDIT_CONTACT_POS14,
	                 // IDC_EDIT_CONTACT_POS15,IDC_EDIT_CONTACT_POS16};

	for (int i=RIDX_PRESS_UNIT_1; i< eMaxPressUnitCount; i++)
	{
		CString strPos = _T("");
		m_dContactPosBefore[i] = dContactPos[i]/1000;
		strPos.Format("%.2f",m_dContactPosBefore[i]);
		GetDlgItem(nEditId[i])->SetWindowText(strPos);
	}	
}
void CYieldSetDlg::SavePressContactData()
{
	char szDriverFileName[512]={0,};
	char szDrvFile[512]={0,};
	char szCokName[128]={0,};
	char szTeachFile[512]={0,};
	double dContactPos[eMaxPressUnitCount] ={0,};
	int nEditId[eMaxPressUnitCount] = { IDC_EDIT_CONTACT_POS1,IDC_EDIT_CONTACT_POS2,IDC_EDIT_CONTACT_POS3,IDC_EDIT_CONTACT_POS4,IDC_EDIT_CONTACT_POS5,IDC_EDIT_CONTACT_POS6,IDC_EDIT_CONTACT_POS7,
		IDC_EDIT_CONTACT_POS8 };//,IDC_EDIT_CONTACT_POS9,IDC_EDIT_CONTACT_POS10,IDC_EDIT_CONTACT_POS11,IDC_EDIT_CONTACT_POS12,IDC_EDIT_CONTACT_POS13,IDC_EDIT_CONTACT_POS14,
// 		IDC_EDIT_CONTACT_POS15,IDC_EDIT_CONTACT_POS16};

	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));

	GetCurrentDirectory(sizeof(szDriverFileName), szDriverFileName);
	sprintf_s(szDrvFile, sizeof(szDrvFile), "%s\\sys_data\\", szDriverFileName);

	sprintf_s(szTeachFile, sizeof(szTeachFile), "%s%s\\%s\\%s\\%s", 
		szDrvFile, SZ_HANDLER_RCP_FOLDER, SZ_TRAY_FOLDER, szCokName, SZ_TEACHING_FILE_NAME);

	CConfigData CConfig(szTeachFile);

	for(int i=RIDX_PRESS_UNIT_1; i<eMaxPressUnitCount; i++){
		CString strTitle = _T("");
		CString strContactPos = _T("");
		char szTitle[64] ={0,};

		if( i > RIDX_PRESS_UNIT_8){
			strTitle.Format("LOCATION TEACHING TABLE - Press Unit Up %d",i-7);
		}else{
			strTitle.Format("LOCATION TEACHING TABLE - Press Unit Down %d",i+1);
		}
		sprintf_s(szTitle,sizeof(szTitle),strTitle);
		
		GetDlgItem(nEditId[i])->GetWindowText(strContactPos);
		m_dContactPos[i] = atof(strContactPos)*1000;
		CConfig.Set(szTitle,"Contact",m_dContactPos[i]);		
	}
	BOOL bisChange = PressContactPosChangeLog();
	if(bisChange == TRUE){
		g_McComm.CmdTeachingDataReLoad();
	}
}


void CYieldSetDlg::LoadPressHeightCalData()
{
	char szCokName[512] = { 0, };
	ST_COK_INFO     stCokData;
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
	g_COK_FileMgr.LoadCokInfo(szCokName, stCokData);

	//IDC_EDIT_HEIGHT_CAL_DATA1
	int nEditId[eMaxPressUnitCount] = { IDC_EDIT_HEIGHT_CAL_DATA1 , IDC_EDIT_HEIGHT_CAL_DATA2 , IDC_EDIT_HEIGHT_CAL_DATA3 , IDC_EDIT_HEIGHT_CAL_DATA4 ,
										IDC_EDIT_HEIGHT_CAL_DATA5 , IDC_EDIT_HEIGHT_CAL_DATA6 , IDC_EDIT_HEIGHT_CAL_DATA7 , IDC_EDIT_HEIGHT_CAL_DATA8 };

	for (int i = ePressUnit_Down_1; i < eMaxPressUnitCount; i++)
	{
		CString strData = _T("");
		strData.Format("%.2f", stCokData.dPressHeightCalData[i]); //mm값.
		GetDlgItem(nEditId[i])->SetWindowText(strData);
	}
}

void CYieldSetDlg::MakeLog(LPCTSTR fmt, ...)
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


void CYieldSetDlg::ConrtactSettingChangeLog(std::vector<std::pair<std::pair<CString,int>,std::pair<double,double>>> vData)
{
	for(int i = 0 ; i< (int)vData.size(); i++)
	{
		std::pair<std::pair<CString,int>,std::pair<double,double>> p;

		p = vData[i];

		if(p.first.second == 1 ) // NoUse
		{
			if(p.second.second == 0)
			{
				MakeLog("[MENU = Contact Setting, DATA NAME = %s ][BEFORE= Drop Mode, AFTER= Direct Mode ]", p.first.first);
			}
			else if(p.second.second == 1)
			{
				MakeLog("[MENU = Contact Setting, DATA NAME = %s ][BEFORE= Direct Mode, AFTER= Drop Mode ]", p.first.first);

			}
			
		}
		else
		{
			MakeLog("[MENU = Contact Setting, DATA NAME = %s ][BEFORE= %.2f , AFTER= %.2f ]", p.first.first, p.second.first, p.second.second);
		}
	}
}

BOOL CYieldSetDlg::PressContactPosChangeLog()
{
	BOOL bisChange = FALSE;
	for(int i =0; i<eMaxPressUnitCount; i++)
	{
		if(m_dContactPos[i]/1000 != m_dContactPosBefore[i]){
			MakeLog("[MENU = Contact Setting, DATA NAME = Press Contact Pos Site %d ][BEFORE= %.2f , AFTER= %.2f ]", i+1,m_dContactPosBefore[i],m_dContactPos[i]/1000);
			bisChange = TRUE;
		}
	}
	return bisChange;
}
void CYieldSetDlg::OnEnSetfocusEditContactPos1()
{
	char szMin[128]={0,};
	char szMax[128]={0,};
	char szRetVal[64] = {0,};
	char szValue_Old[64] = {0,};

	_tMOTOR_PARAM tMotorParam;
	g_DB.SELECT_MOTORPARAM_TBL(eAXIS_PRESS_UNIT_DN1, tMotorParam);

	CString strValue_Old = _T("");
	GetDlgItem(IDC_EDIT_CONTACT_POS1)->GetWindowText(strValue_Old);

	sprintf_s(szValue_Old, sizeof(szValue_Old), "%s", (LPCTSTR)strValue_Old);
	sprintf_s(szMax, sizeof(szMax), "%.2f", tMotorParam.dLimit_Posi/1000.0);
	sprintf_s(szMin, sizeof(szMin), "%.2f", tMotorParam.dLimit_Nega/1000.0);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, sizeof(szRetVal), szValue_Old, "Input"/*szTitle*/, szMax, szMin);
	
	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_CONTACT_POS1)->SetWindowText(szRetVal);
	}
}
void CYieldSetDlg::OnEnSetfocusEditContactPos2()
{
	char szMin[128]={0,};
	char szMax[128]={0,};
	char szRetVal[64] = {0,};
	char szValue_Old[64] = {0,};

	_tMOTOR_PARAM tMotorParam;
	g_DB.SELECT_MOTORPARAM_TBL(eAXIS_PRESS_UNIT_DN2, tMotorParam);

	CString strValue_Old = _T("");
	GetDlgItem(IDC_EDIT_CONTACT_POS2)->GetWindowText(strValue_Old);

	sprintf_s(szValue_Old, sizeof(szValue_Old), "%s", (LPCTSTR)strValue_Old);
	sprintf_s(szMax, sizeof(szMax), "%.2f", tMotorParam.dLimit_Posi/1000.0);
	sprintf_s(szMin, sizeof(szMin), "%.2f", tMotorParam.dLimit_Nega/1000.0);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, sizeof(szRetVal), szValue_Old, "Input"/*szTitle*/, szMax, szMin);

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_CONTACT_POS2)->SetWindowText(szRetVal);
	}
}
void CYieldSetDlg::OnEnSetfocusEditContactPos3()
{
	char szMin[128]={0,};
	char szMax[128]={0,};
	char szRetVal[64] = {0,};
	char szValue_Old[64] = {0,};

	_tMOTOR_PARAM tMotorParam;
	g_DB.SELECT_MOTORPARAM_TBL(eAXIS_PRESS_UNIT_DN3, tMotorParam);

	CString strValue_Old = _T("");
	GetDlgItem(IDC_EDIT_CONTACT_POS3)->GetWindowText(strValue_Old);

	sprintf_s(szValue_Old, sizeof(szValue_Old), "%s", (LPCTSTR)strValue_Old);
	sprintf_s(szMax, sizeof(szMax), "%.2f", tMotorParam.dLimit_Posi/1000.0);
	sprintf_s(szMin, sizeof(szMin), "%.2f", tMotorParam.dLimit_Nega/1000.0);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, sizeof(szRetVal), szValue_Old, "Input"/*szTitle*/, szMax, szMin);

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_CONTACT_POS3)->SetWindowText(szRetVal);
	}
}
void CYieldSetDlg::OnEnSetfocusEditContactPos4()
{
	char szMin[128]={0,};
	char szMax[128]={0,};
	char szRetVal[64] = {0,};
	char szValue_Old[64] = {0,};

	_tMOTOR_PARAM tMotorParam;
	g_DB.SELECT_MOTORPARAM_TBL(eAXIS_PRESS_UNIT_DN4, tMotorParam);

	CString strValue_Old = _T("");
	GetDlgItem(IDC_EDIT_CONTACT_POS4)->GetWindowText(strValue_Old);

	sprintf_s(szValue_Old, sizeof(szValue_Old), "%s", (LPCTSTR)strValue_Old);
	sprintf_s(szMax, sizeof(szMax), "%.2f", tMotorParam.dLimit_Posi/1000.0);
	sprintf_s(szMin, sizeof(szMin), "%.2f", tMotorParam.dLimit_Nega/1000.0);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, sizeof(szRetVal), szValue_Old, "Input"/*szTitle*/, szMax, szMin);

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_CONTACT_POS4)->SetWindowText(szRetVal);
	}
}
void CYieldSetDlg::OnEnSetfocusEditContactPos5()
{
	char szMin[128]={0,};
	char szMax[128]={0,};
	char szRetVal[64] = {0,};
	char szValue_Old[64] = {0,};

	_tMOTOR_PARAM tMotorParam;
	g_DB.SELECT_MOTORPARAM_TBL(eAXIS_PRESS_UNIT_DN5, tMotorParam);

	CString strValue_Old = _T("");
	GetDlgItem(IDC_EDIT_CONTACT_POS5)->GetWindowText(strValue_Old);

	sprintf_s(szValue_Old, sizeof(szValue_Old), "%s", (LPCTSTR)strValue_Old);
	sprintf_s(szMax, sizeof(szMax), "%.2f", tMotorParam.dLimit_Posi/1000.0);
	sprintf_s(szMin, sizeof(szMin), "%.2f", tMotorParam.dLimit_Nega/1000.0);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, sizeof(szRetVal), szValue_Old, "Input"/*szTitle*/, szMax, szMin);

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_CONTACT_POS5)->SetWindowText(szRetVal);
	}
}
void CYieldSetDlg::OnEnSetfocusEditContactPos6()
{
	char szMin[128]={0,};
	char szMax[128]={0,};
	char szRetVal[64] = {0,};
	char szValue_Old[64] = {0,};

	_tMOTOR_PARAM tMotorParam;
	g_DB.SELECT_MOTORPARAM_TBL(eAXIS_PRESS_UNIT_DN6, tMotorParam);

	CString strValue_Old = _T("");
	GetDlgItem(IDC_EDIT_CONTACT_POS6)->GetWindowText(strValue_Old);

	sprintf_s(szValue_Old, sizeof(szValue_Old), "%s", (LPCTSTR)strValue_Old);
	sprintf_s(szMax, sizeof(szMax), "%.2f", tMotorParam.dLimit_Posi/1000.0);
	sprintf_s(szMin, sizeof(szMin), "%.2f", tMotorParam.dLimit_Nega/1000.0);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, sizeof(szRetVal), szValue_Old, "Input"/*szTitle*/, szMax, szMin);

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_CONTACT_POS6)->SetWindowText(szRetVal);
	}
}
void CYieldSetDlg::OnEnSetfocusEditContactPos7()
{
	char szMin[128]={0,};
	char szMax[128]={0,};
	char szRetVal[64] = {0,};
	char szValue_Old[64] = {0,};

	_tMOTOR_PARAM tMotorParam;
	g_DB.SELECT_MOTORPARAM_TBL(eAXIS_PRESS_UNIT_DN7, tMotorParam);

	CString strValue_Old = _T("");
	GetDlgItem(IDC_EDIT_CONTACT_POS7)->GetWindowText(strValue_Old);

	sprintf_s(szValue_Old, sizeof(szValue_Old), "%s", (LPCTSTR)strValue_Old);
	sprintf_s(szMax, sizeof(szMax), "%.2f", tMotorParam.dLimit_Posi/1000.0);
	sprintf_s(szMin, sizeof(szMin), "%.2f", tMotorParam.dLimit_Nega/1000.0);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, sizeof(szRetVal), szValue_Old, "Input"/*szTitle*/, szMax, szMin);

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_CONTACT_POS7)->SetWindowText(szRetVal);
	}
}
void CYieldSetDlg::OnEnSetfocusEditContactPos8()
{
	char szMin[128]={0,};
	char szMax[128]={0,};
	char szRetVal[64] = {0,};
	char szValue_Old[64] = {0,};

	_tMOTOR_PARAM tMotorParam;
	g_DB.SELECT_MOTORPARAM_TBL(eAXIS_PRESS_UNIT_DN8, tMotorParam);

	CString strValue_Old = _T("");
	GetDlgItem(IDC_EDIT_CONTACT_POS8)->GetWindowText(strValue_Old);

	sprintf_s(szValue_Old, sizeof(szValue_Old), "%s", (LPCTSTR)strValue_Old);
	sprintf_s(szMax, sizeof(szMax), "%.2f", tMotorParam.dLimit_Posi/1000.0);
	sprintf_s(szMin, sizeof(szMin), "%.2f", tMotorParam.dLimit_Nega/1000.0);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, sizeof(szRetVal), szValue_Old, "Input"/*szTitle*/, szMax, szMin);

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_CONTACT_POS8)->SetWindowText(szRetVal);
	}
}
