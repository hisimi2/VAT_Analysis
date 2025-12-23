// ClearCntDlg_64Para.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "ClearCntDlg_64Para.h"
#include "afxdialogex.h"


// CClearCntDlg_64Para 대화 상자입니다.

IMPLEMENT_DYNAMIC(CClearCntDlg_64Para, CDialogEx)

CClearCntDlg_64Para::CClearCntDlg_64Para(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClearCntDlg_64Para::IDD, pParent)
{
	m_TestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	m_TestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	m_nYieldMode = 0;
}

CClearCntDlg_64Para::~CClearCntDlg_64Para()
{
}

void CClearCntDlg_64Para::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_STATION_LIST, m_cbStationName);

	DDX_Control(pDX, IDCANCEL, m_BtnCancel);

	DDX_Control(pDX, IDC_BUTTON_CLEAR_1, m_BtnClearSite_1);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_2, m_BtnClearSite_2);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_3, m_BtnClearSite_3);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_4, m_BtnClearSite_4);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_5, m_BtnClearSite_5);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_6, m_BtnClearSite_6);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_7, m_BtnClearSite_7);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_8, m_BtnClearSite_8);

	DDX_Control(pDX, IDC_BUTTON_CLEAR_YIELD1, m_BtnClearSite_1_Yield);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_YIELD2, m_BtnClearSite_2_Yield);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_YIELD3, m_BtnClearSite_3_Yield);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_YIELD4, m_BtnClearSite_4_Yield);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_YIELD5, m_BtnClearSite_5_Yield);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_YIELD6, m_BtnClearSite_6_Yield);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_YIELD7, m_BtnClearSite_7_Yield);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_YIELD8, m_BtnClearSite_8_Yield);

	DDX_Control(pDX, IDC_BUTTON_CLEAR_ALL, m_BtnClearSiteAll);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_ALL_MONITORING, m_BtnClearSiteAll_Yield);

	DDX_Control(pDX, IDC_BUTTON_CLEAR_TOTAL_COUNT, m_BtnClear_TotalCount);

	DDX_Control(pDX, IDC_RADIO_YIELD, m_rbChkYield);
	DDX_Control(pDX, IDC_RADIO_1ST_FAIL, m_rbChk1stFail);

}


BEGIN_MESSAGE_MAP(CClearCntDlg_64Para, CDialogEx)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_STATION_LIST, &CClearCntDlg_64Para::OnCbnSelchangeComboStationList)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_1, &CClearCntDlg_64Para::OnBnClickedButtonClear1)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_2, &CClearCntDlg_64Para::OnBnClickedButtonClear2)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_3, &CClearCntDlg_64Para::OnBnClickedButtonClear3)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_4, &CClearCntDlg_64Para::OnBnClickedButtonClear4)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_ALL, &CClearCntDlg_64Para::OnBnClickedButtonClearAll)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_ALL_MONITORING, &CClearCntDlg_64Para::OnBnClickedButtonClearAllMonitoring)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_5, &CClearCntDlg_64Para::OnBnClickedButtonClear5)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_6, &CClearCntDlg_64Para::OnBnClickedButtonClear6)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_7, &CClearCntDlg_64Para::OnBnClickedButtonClear7)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_8, &CClearCntDlg_64Para::OnBnClickedButtonClear8)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_YIELD1, &CClearCntDlg_64Para::OnBnClickedButtonClearYield1)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_YIELD2, &CClearCntDlg_64Para::OnBnClickedButtonClearYield2)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_YIELD3, &CClearCntDlg_64Para::OnBnClickedButtonClearYield3)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_YIELD4, &CClearCntDlg_64Para::OnBnClickedButtonClearYield4)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_YIELD5, &CClearCntDlg_64Para::OnBnClickedButtonClearYield5)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_YIELD6, &CClearCntDlg_64Para::OnBnClickedButtonClearYield6)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_YIELD7, &CClearCntDlg_64Para::OnBnClickedButtonClearYield7)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_YIELD8, &CClearCntDlg_64Para::OnBnClickedButtonClearYield8)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_TOTAL_COUNT, &CClearCntDlg_64Para::OnBnClickedButtonClearTotalCount)
	ON_BN_CLICKED(IDC_RADIO_YIELD, &CClearCntDlg_64Para::OnBnClickedRadioYield)
	ON_BN_CLICKED(IDC_RADIO_1ST_FAIL, &CClearCntDlg_64Para::OnBnClickedRadio1stFail)
END_MESSAGE_MAP()


// CClearCntDlg_64Para 메시지 처리기입니다.

BOOL CClearCntDlg_64Para::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitControl();
	UpdateYieldData();
	UpdateSiteNum();
	
	m_rbChkYield.SetCheck(TRUE); //기본 값 할당.
	
	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	GetDlgItem(IDC_RADIO_YIELD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_1ST_FAIL)->ShowWindow(SW_HIDE);
	

	return TRUE;
}


void CClearCntDlg_64Para::OnDestroy()
{
	CDialogEx::OnDestroy();
}

void CClearCntDlg_64Para::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case 0:
		{
			KillTimer(nIDEvent);
			
			UpdateYieldData();

			SetTimer(nIDEvent, 300, NULL);
		}break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CClearCntDlg_64Para::InitControl()
{
	CString strTmp = _T("");
	for(int i=0; i<eMaxPressUnitCount; i++){
		strTmp.Format(_T("Station %d"),i+1);
		m_cbStationName.AddString(strTmp);
	}

	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnClearSite_1.SetShade(CShadeButtonST::SHS_METAL,8,20,0,RGB(55,55,255));
	m_BtnClearSite_1.SetIcon(IDI_BTN_CLEAR);
	m_BtnClearSite_1.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnClearSite_2.SetShade(CShadeButtonST::SHS_METAL,8,20,0,RGB(55,55,255));
	m_BtnClearSite_2.SetIcon(IDI_BTN_CLEAR);
	m_BtnClearSite_2.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnClearSite_3.SetShade(CShadeButtonST::SHS_METAL,8,20,0,RGB(55,55,255));
	m_BtnClearSite_3.SetIcon(IDI_BTN_CLEAR);
	m_BtnClearSite_3.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnClearSite_4.SetShade(CShadeButtonST::SHS_METAL,8,20,0,RGB(55,55,255));
	m_BtnClearSite_4.SetIcon(IDI_BTN_CLEAR);
	m_BtnClearSite_4.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnClearSite_5.SetShade(CShadeButtonST::SHS_METAL,8,20,0,RGB(55,55,255));
	m_BtnClearSite_5.SetIcon(IDI_BTN_CLEAR);
	m_BtnClearSite_5.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnClearSite_6.SetShade(CShadeButtonST::SHS_METAL,8,20,0,RGB(55,55,255));
	m_BtnClearSite_6.SetIcon(IDI_BTN_CLEAR);
	m_BtnClearSite_6.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnClearSite_7.SetShade(CShadeButtonST::SHS_METAL,8,20,0,RGB(55,55,255));
	m_BtnClearSite_7.SetIcon(IDI_BTN_CLEAR);
	m_BtnClearSite_7.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnClearSite_8.SetShade(CShadeButtonST::SHS_METAL,8,20,0,RGB(55,55,255));
	m_BtnClearSite_8.SetIcon(IDI_BTN_CLEAR);
	m_BtnClearSite_8.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnClearSite_1_Yield.SetShade(CShadeButtonST::SHS_METAL,8,20,0,RGB(55,55,255));
	m_BtnClearSite_1_Yield.SetIcon(IDI_BTN_CLEAR);
	m_BtnClearSite_1_Yield.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnClearSite_2_Yield.SetShade(CShadeButtonST::SHS_METAL,8,20,0,RGB(55,55,255));
	m_BtnClearSite_2_Yield.SetIcon(IDI_BTN_CLEAR);
	m_BtnClearSite_2_Yield.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnClearSite_3_Yield.SetShade(CShadeButtonST::SHS_METAL,8,20,0,RGB(55,55,255));
	m_BtnClearSite_3_Yield.SetIcon(IDI_BTN_CLEAR);
	m_BtnClearSite_3_Yield.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnClearSite_4_Yield.SetShade(CShadeButtonST::SHS_METAL,8,20,0,RGB(55,55,255));
	m_BtnClearSite_4_Yield.SetIcon(IDI_BTN_CLEAR);
	m_BtnClearSite_4_Yield.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnClearSite_5_Yield.SetShade(CShadeButtonST::SHS_METAL,8,20,0,RGB(55,55,255));
	m_BtnClearSite_5_Yield.SetIcon(IDI_BTN_CLEAR);
	m_BtnClearSite_5_Yield.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnClearSite_6_Yield.SetShade(CShadeButtonST::SHS_METAL,8,20,0,RGB(55,55,255));
	m_BtnClearSite_6_Yield.SetIcon(IDI_BTN_CLEAR);
	m_BtnClearSite_6_Yield.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnClearSite_7_Yield.SetShade(CShadeButtonST::SHS_METAL,8,20,0,RGB(55,55,255));
	m_BtnClearSite_7_Yield.SetIcon(IDI_BTN_CLEAR);
	m_BtnClearSite_7_Yield.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnClearSite_8_Yield.SetShade(CShadeButtonST::SHS_METAL,8,20,0,RGB(55,55,255));
	m_BtnClearSite_8_Yield.SetIcon(IDI_BTN_CLEAR);
	m_BtnClearSite_8_Yield.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnClearSiteAll.SetShade(CShadeButtonST::SHS_METAL,8,20,0,RGB(55,55,255));
	m_BtnClearSiteAll.SetIcon(IDI_BTN_CLEAR);
	m_BtnClearSiteAll.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnClearSiteAll_Yield.SetShade(CShadeButtonST::SHS_METAL,8,20,0,RGB(55,55,255));
	m_BtnClearSiteAll_Yield.SetIcon(IDI_BTN_CLEAR);
	m_BtnClearSiteAll_Yield.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnClear_TotalCount.SetShade(CShadeButtonST::SHS_METAL, 8, 20, 0, RGB(55, 55, 255));
	m_BtnClear_TotalCount.SetIcon(IDI_BTN_CLEAR);
	m_BtnClear_TotalCount.SetAlign(CButtonST::ST_ALIGN_VERT);


	m_vDlgID.push_back(IDC_EDIT_SITE_1); m_vDlgID.push_back(IDC_EDIT_SITE_2);
	m_vDlgID.push_back(IDC_EDIT_SITE_3); m_vDlgID.push_back(IDC_EDIT_SITE_4);
	m_vDlgID.push_back(IDC_EDIT_SITE_5); m_vDlgID.push_back(IDC_EDIT_SITE_6);
	m_vDlgID.push_back(IDC_EDIT_SITE_7); m_vDlgID.push_back(IDC_EDIT_SITE_8);

	m_vBtnID.push_back(IDC_BUTTON_CLEAR_1);	m_vBtnID.push_back(IDC_BUTTON_CLEAR_2); 
	m_vBtnID.push_back(IDC_BUTTON_CLEAR_3);	m_vBtnID.push_back(IDC_BUTTON_CLEAR_4); 
	m_vBtnID.push_back(IDC_BUTTON_CLEAR_5);	m_vBtnID.push_back(IDC_BUTTON_CLEAR_6); 
	m_vBtnID.push_back(IDC_BUTTON_CLEAR_7);	m_vBtnID.push_back(IDC_BUTTON_CLEAR_8); 

	m_vDlgID_Yield.push_back(IDC_EDIT_SITE_1_YIELD); m_vDlgID_Yield.push_back(IDC_EDIT_SITE_2_YIELD);
	m_vDlgID_Yield.push_back(IDC_EDIT_SITE_3_YIELD); m_vDlgID_Yield.push_back(IDC_EDIT_SITE_4_YIELD);
	m_vDlgID_Yield.push_back(IDC_EDIT_SITE_5_YIELD); m_vDlgID_Yield.push_back(IDC_EDIT_SITE_6_YIELD);
	m_vDlgID_Yield.push_back(IDC_EDIT_SITE_7_YIELD); m_vDlgID_Yield.push_back(IDC_EDIT_SITE_8_YIELD);

	m_vBtnID_Yield.push_back(IDC_BUTTON_CLEAR_YIELD1);	m_vBtnID_Yield.push_back(IDC_BUTTON_CLEAR_YIELD2);
	m_vBtnID_Yield.push_back(IDC_BUTTON_CLEAR_YIELD3);	m_vBtnID_Yield.push_back(IDC_BUTTON_CLEAR_YIELD4);
	m_vBtnID_Yield.push_back(IDC_BUTTON_CLEAR_YIELD5);	m_vBtnID_Yield.push_back(IDC_BUTTON_CLEAR_YIELD6);
	m_vBtnID_Yield.push_back(IDC_BUTTON_CLEAR_YIELD7);	m_vBtnID_Yield.push_back(IDC_BUTTON_CLEAR_YIELD8);

	m_vGroupBoxID.push_back(IDC_STATIC_SITE_1); m_vGroupBoxID.push_back(IDC_STATIC_SITE_2);
	m_vGroupBoxID.push_back(IDC_STATIC_SITE_3); m_vGroupBoxID.push_back(IDC_STATIC_SITE_4);
	m_vGroupBoxID.push_back(IDC_STATIC_SITE_5); m_vGroupBoxID.push_back(IDC_STATIC_SITE_6);
	m_vGroupBoxID.push_back(IDC_STATIC_SITE_7); m_vGroupBoxID.push_back(IDC_STATIC_SITE_8);

	m_vGroupBox_Yield.push_back(IDC_STATIC_SITE_YIELD_1); m_vGroupBox_Yield.push_back(IDC_STATIC_SITE_YIELD_2);
	m_vGroupBox_Yield.push_back(IDC_STATIC_SITE_YIELD_3); m_vGroupBox_Yield.push_back(IDC_STATIC_SITE_YIELD_4);
	m_vGroupBox_Yield.push_back(IDC_STATIC_SITE_YIELD_5); m_vGroupBox_Yield.push_back(IDC_STATIC_SITE_YIELD_6);
	m_vGroupBox_Yield.push_back(IDC_STATIC_SITE_YIELD_7); m_vGroupBox_Yield.push_back(IDC_STATIC_SITE_YIELD_8);

	for( int ni = 0; ni < (int)m_vBtnID.size(); ni++) {
		GetDlgItem(m_vDlgID[ni])->SetWindowTextA("-");
		GetDlgItem(m_vDlgID_Yield[ni])->SetWindowTextA("-");
	}

	for( int nCnt = 0; nCnt < m_TestSite_Div_X*m_TestSite_Div_Y; nCnt++) {
		GetDlgItem(m_vDlgID[nCnt])->EnableWindow(TRUE);
		GetDlgItem(m_vBtnID[nCnt])->EnableWindow(TRUE);
		GetDlgItem(m_vDlgID_Yield[nCnt])->EnableWindow(TRUE);
		GetDlgItem(m_vBtnID_Yield[nCnt])->EnableWindow(TRUE);
	}

	m_cbStationName.SetCurSel(0);
	SetTimer(0,800, NULL);
}

void CClearCntDlg_64Para::UpdateYieldData()
{
	CString strTmp = _T("");
	int nIdx = m_cbStationName.GetCurSel();

	int nStation_Max_para = STATION_MAX_PARA; // m_TestSite_Div_X * m_TestSite_Div_Y;
	
	if (m_nYieldMode == eYield)
	{
		for (int nCnt = 0; nCnt < nStation_Max_para; nCnt++) {
			// Yield Count
			strTmp.Format("%d", g_DMCont.m_dmContac_Dn.GN(NDM1_ClrAfterGdBinCntPress1_1 + nCnt + (nIdx * STATION_MAX_X_SITE * STATION_MAX_Y_SITE)) + g_DMCont.m_dmContac_Dn.GN(NDM1_ClsAfterFailBinCntPress1_1 + nCnt + (nIdx * STATION_MAX_X_SITE * STATION_MAX_Y_SITE)));
			SetDlgItemText(m_vDlgID[nCnt], strTmp);
		}
		for (int nCnt = 0; nCnt < nStation_Max_para; nCnt++) {
			// Yield Monitor Count
			strTmp.Format("%d", g_DMCont.m_dmContac_Dn.GN(NDM1_Yield_Count_Press1_1 + nCnt + (nIdx * nStation_Max_para)));
			SetDlgItemText(m_vDlgID_Yield[nCnt], strTmp);
		}
	}
	else
	{
		for (int nCnt = 0; nCnt < nStation_Max_para; nCnt++) {
			// Yield Count
			strTmp.Format("%d", g_DMCont.m_dmContac_Up.GN(NDM2_1STFailBinCnt_Press1_1 + nCnt + (nIdx * STATION_MAX_X_SITE * STATION_MAX_Y_SITE)));
			SetDlgItemText(m_vDlgID[nCnt], strTmp);
		}
		for (int nCnt = 0; nCnt < nStation_Max_para; nCnt++) {
			// Yield Monitor Count
			strTmp.Format("%d", g_DMCont.m_dmContac_Dn.GN(NDM1_Yield_1st_Count_Press1_1 + nCnt + (nIdx * nStation_Max_para)));
			SetDlgItemText(m_vDlgID_Yield[nCnt], strTmp);
		}
	}
	

	//for(int i=0; i < (int)m_vDlgID.size(); i++){               
	//    strTmp.Format("%d", g_DMCont.m_dmContac_Dn.GN(NDM1_ClrAfterGdBinCntPress1_1 + i + (nIdx * nStation_Max_para)) + g_DMCont.m_dmContac_Dn.GN(NDM1_ClsAfterFailBinCntPress1_1 + i + (nIdx * nStation_Max_para)) );
	//    SetDlgItemText(m_vDlgID[i], strTmp);
	//}
	//for(int i =0; i < (int)m_vDlgID_Yield.size(); i++){
	//	strTmp.Format("%d",g_DMCont.m_dmContac_Dn.GN(NDM1_Yield_Count_Press1_1+i +(nIdx * nStation_Max_para)) );
	//	SetDlgItemText(m_vDlgID_Yield[i],strTmp);
	//}
}

void CClearCntDlg_64Para::UpdateSiteNum()
{
	CString strTmp = _T("");
	int nIdx =m_cbStationName.GetCurSel();

	for(int i = 0; i < (STATION_MAX_PARA); i++) {
		strTmp.Format( _T("Site%d-%d"), nIdx+1, i+1 );

		SetDlgItemText(m_vGroupBoxID[i],strTmp);
		SetDlgItemText(m_vGroupBox_Yield[i],strTmp);
	}
}

void CClearCntDlg_64Para::ClearBinFunction( int nStationNo, int nSocketNo, int nDlgID )
{
	CString strBeforeData = _T("");
	GetDlgItemText(nDlgID, strBeforeData);
	if (m_nYieldMode == eYield)
	{
		g_DMCont.m_dmContac_Dn.SN(NDM1_ClsAfterFailBinCntPress1_1 + nSocketNo + (nStationNo *STATION_MAX_PARA), 0);
		g_DMCont.m_dmContac_Dn.SN(NDM1_ClrAfterGdBinCntPress1_1 + nSocketNo + (nStationNo *STATION_MAX_PARA), 0);
	}
	else
	{
		g_DMCont.m_dmContac_Up.SN(NDM2_1STFailBinCnt_Press1_1 + nSocketNo + (nStationNo *STATION_MAX_PARA), 0);
	}

	MakeLog("[MENU = Clear Counter, DATA NAME = %d-2 ][BEFORE= %s, AFTER= 0 ]", nStationNo +1,strBeforeData);
}

void CClearCntDlg_64Para::ClearYieldFunction( int nStaionNo, int nSocketNo, int nDlgID )
{
	int nIdx = m_cbStationName.GetCurSel();

	CString strBeforeData = _T("");
	GetDlgItemText(nDlgID, strBeforeData);
	if(m_nYieldMode == eYield)
		g_DMCont.m_dmContac_Dn.SN(NDM1_Yield_Count_Press1_1 + nSocketNo + ( nStaionNo * STATION_MAX_PARA), 0);
	else
		g_DMCont.m_dmContac_Dn.SN(NDM1_Yield_1st_Count_Press1_1 + nSocketNo + (nStaionNo * STATION_MAX_PARA), 0);

	g_McComm.CmdYieldMonitorDataClear_Each(nIdx, 1, m_nYieldMode, nSocketNo);
	MakeLog("[MENU = Clear Yield Counter, DATA NAME = %d-2 ][BEFORE= %s, AFTER= 0 ]",nIdx+1,strBeforeData);
}

void CClearCntDlg_64Para::OnCbnSelchangeComboStationList()
{
	UpdateSiteNum();
}


void CClearCntDlg_64Para::OnBnClickedButtonClear1()
{
	int nIdx = m_cbStationName.GetCurSel();
	ClearBinFunction(nIdx, eSocket1, IDC_EDIT_SITE_1);
}


void CClearCntDlg_64Para::OnBnClickedButtonClear2()
{
	int nIdx = m_cbStationName.GetCurSel();
 	ClearBinFunction(nIdx, eSocket2, IDC_EDIT_SITE_2);
}


void CClearCntDlg_64Para::OnBnClickedButtonClear3()
{
	int nIdx = m_cbStationName.GetCurSel();
	ClearBinFunction(nIdx, eSocket3, IDC_EDIT_SITE_3);
}


void CClearCntDlg_64Para::OnBnClickedButtonClear4()
{
	int nIdx = m_cbStationName.GetCurSel();
	ClearBinFunction(nIdx, eSocket4, IDC_EDIT_SITE_4);
}


void CClearCntDlg_64Para::OnBnClickedButtonClear5()
{
	int nIdx = m_cbStationName.GetCurSel();
	ClearBinFunction(nIdx, eSocket5, IDC_EDIT_SITE_5);
}


void CClearCntDlg_64Para::OnBnClickedButtonClear6()
{
	int nIdx = m_cbStationName.GetCurSel();
	ClearBinFunction(nIdx, eSocket6, IDC_EDIT_SITE_6);
}


void CClearCntDlg_64Para::OnBnClickedButtonClear7()
{
	int nIdx = m_cbStationName.GetCurSel();
	ClearBinFunction(nIdx, eSocket7, IDC_EDIT_SITE_7);
}


void CClearCntDlg_64Para::OnBnClickedButtonClear8()
{
	int nIdx = m_cbStationName.GetCurSel();
	ClearBinFunction(nIdx, eSocket8, IDC_EDIT_SITE_8);
}


void CClearCntDlg_64Para::OnBnClickedButtonClearYield1()
{
	int nIdx = m_cbStationName.GetCurSel();
	ClearYieldFunction( nIdx, eSocket1, IDC_EDIT_SITE_1_YIELD );
}


void CClearCntDlg_64Para::OnBnClickedButtonClearYield2()
{
	int nIdx = m_cbStationName.GetCurSel();
	ClearYieldFunction( nIdx, eSocket2, IDC_EDIT_SITE_2_YIELD );
}


void CClearCntDlg_64Para::OnBnClickedButtonClearYield3()
{
	int nIdx = m_cbStationName.GetCurSel();
	ClearYieldFunction( nIdx, eSocket3, IDC_EDIT_SITE_3_YIELD );
}


void CClearCntDlg_64Para::OnBnClickedButtonClearYield4()
{
	int nIdx = m_cbStationName.GetCurSel();
	ClearYieldFunction( nIdx, eSocket4, IDC_EDIT_SITE_4_YIELD );
}


void CClearCntDlg_64Para::OnBnClickedButtonClearYield5()
{
	int nIdx = m_cbStationName.GetCurSel();
	ClearYieldFunction( nIdx, eSocket5, IDC_EDIT_SITE_5_YIELD );
}


void CClearCntDlg_64Para::OnBnClickedButtonClearYield6()
{
	int nIdx = m_cbStationName.GetCurSel();
	ClearYieldFunction( nIdx, eSocket6, IDC_EDIT_SITE_6_YIELD );
}


void CClearCntDlg_64Para::OnBnClickedButtonClearYield7()
{
	int nIdx = m_cbStationName.GetCurSel();
	ClearYieldFunction( nIdx, eSocket7, IDC_EDIT_SITE_7_YIELD );
}


void CClearCntDlg_64Para::OnBnClickedButtonClearYield8()
{
	int nIdx = m_cbStationName.GetCurSel();
	ClearYieldFunction( nIdx, eSocket8, IDC_EDIT_SITE_8_YIELD );
}


void CClearCntDlg_64Para::MakeLog(LPCTSTR fmt, ...)
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


void CClearCntDlg_64Para::OnBnClickedButtonClearAll()
{
	int nTestSite_Max_para = STATION_MAX_PARA;

 	for(int i=0; i< eMaxPressUnitCount; i++){
		for(int j=0; j<nTestSite_Max_para; j++){
			if (m_nYieldMode == eYield)
			{
				g_DMCont.m_dmContac_Dn.SN(NDM1_ClsAfterFailBinCntPress1_1 + j + (i*nTestSite_Max_para), 0);
				g_DMCont.m_dmContac_Dn.SN(NDM1_ClrAfterGdBinCntPress1_1 + j + (i*nTestSite_Max_para), 0);
			}
			else
			{
				g_DMCont.m_dmContac_Up.SN(NDM2_1STFailBinCnt_Press1_1 + j + (i*nTestSite_Max_para), 0);
			}
 		
		}
 	}
 	MakeLog("[MENU = Clear Counter, DATA NAME = All Site Clear]");
}


void CClearCntDlg_64Para::OnBnClickedButtonClearAllMonitoring()
{
	int nTestSite_Max_para = STATION_MAX_PARA;//m_TestSite_Div_X*m_TestSite_Div_Y;

 	for(int i=0; i< eMaxPressUnitCount; i++){
		for(int j=0; j<nTestSite_Max_para; j++){
			if (m_nYieldMode == eYield)
			{
				g_DMCont.m_dmContac_Dn.SN(NDM1_Yield_Count_Press1_1 + j + (i*nTestSite_Max_para), 0);
			}
			else
			{
				g_DMCont.m_dmContac_Dn.SN(NDM1_Yield_1st_Count_Press1_1 + j + (i*nTestSite_Max_para), 0);
			}
		}
 	}
 	g_McComm.CmdYieldMonitorDataClear();
 	MakeLog("[MENU = Clear Yield Monitoring Counter, DATA NAME = All Site Clear]");
}





void CClearCntDlg_64Para::OnBnClickedButtonClearTotalCount()
{
	g_DMCont.m_dmEQP.SN(NDM0_TotalLoadCnt, 0);
}


void CClearCntDlg_64Para::OnBnClickedRadioYield()
{
	m_nYieldMode = eYield;
	YieldModeChange(m_nYieldMode);
}


void CClearCntDlg_64Para::OnBnClickedRadio1stFail()
{
	m_nYieldMode = e1stFail;
	YieldModeChange(m_nYieldMode);
}

void CClearCntDlg_64Para::YieldModeChange(BOOL bYield)
{
	if (bYield == eYield)
	{
		GetDlgItem(IDC_STATIC_YIELD_COUNT)->SetWindowText("Yield Count");
		GetDlgItem(IDC_STATIC_YIELD_MONITOR_COUNT)->SetWindowText("Yield Monitoring Count");	
		GetDlgItem(IDC_BUTTON_CLEAR_ALL)->SetWindowText("Yield Count Clear");
		GetDlgItem(IDC_BUTTON_CLEAR_ALL_MONITORING)->SetWindowText("Yield Monitoring Count Clear");
	}
	else
	{
		GetDlgItem(IDC_STATIC_YIELD_COUNT)->SetWindowText("1st Count");
		GetDlgItem(IDC_STATIC_YIELD_MONITOR_COUNT)->SetWindowText("1st Monitoring Count");
		GetDlgItem(IDC_BUTTON_CLEAR_ALL)->SetWindowText("1st Count Clear");
		GetDlgItem(IDC_BUTTON_CLEAR_ALL_MONITORING)->SetWindowText("1st Monitoring Count Clear");
	}
}