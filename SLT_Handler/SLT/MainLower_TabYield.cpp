// CMainLower_TabYield.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "MainLower_TabYield.h"
#include "afxdialogex.h"
#include "SLTView.h"
#include "MainFrm.h"

// CMainLower_TabYield 대화 상자입니다.

IMPLEMENT_DYNAMIC( CMainLower_TabYield, CDialogEx )

CMainLower_TabYield::CMainLower_TabYield( eYIELD_MODE mode, CWnd* pParent /*=NULL*/ )
	: CDialogEx( CMainLower_TabYield::IDD, pParent )
{
	m_eYieldMode = mode;
	memset( m_dTestTime, 0x00, sizeof( m_dTestTime ) );
	memset( m_nTestTmCtrlID, 0x00, sizeof( m_nTestTmCtrlID ) );
	memset( m_nIndexID, 0x00, sizeof( m_nIndexID ) );
	memset( m_nSocketOnOff, 0x00, sizeof( m_nSocketOnOff ) );

	memset( m_nGoodBinCnt, 0x00, sizeof( m_nGoodBinCnt ) );
	memset( m_nFailBinCnt, 0x00, sizeof( m_nFailBinCnt ) );
	memset( m_nClsAfterGoodBinCnt, 0x00, sizeof( m_nClsAfterGoodBinCnt ) );
	memset( m_nClsAfterFailBinCnt, 0x00, sizeof( m_nClsAfterFailBinCnt ) );

	memset(m_n1stFailBinCnt, 0x00, sizeof(m_n1stFailBinCnt));
	memset(m_n2DIDFailBinCnt, 0x00, sizeof(m_n2DIDFailBinCnt));

	m_nTestTmCtrlID[ 0 ] = IDC_TIME_SITE1;
	m_nTestTmCtrlID[ 1 ] = IDC_TIME_SITE2;
	m_nTestTmCtrlID[ 2 ] = IDC_TIME_SITE3;
	m_nTestTmCtrlID[ 3 ] = IDC_TIME_SITE4;
	m_nTestTmCtrlID[ 4 ] = IDC_TIME_SITE5;
	m_nTestTmCtrlID[ 5 ] = IDC_TIME_SITE6;
	m_nTestTmCtrlID[ 6 ] = IDC_TIME_SITE7;
	m_nTestTmCtrlID[ 7 ] = IDC_TIME_SITE8;


	m_nIndexID[ 0 ] = IDC_TEST_SITE1;
	m_nIndexID[ 1 ] = IDC_TEST_SITE2;
	m_nIndexID[ 2 ] = IDC_TEST_SITE3;
	m_nIndexID[ 3 ] = IDC_TEST_SITE4;
	m_nIndexID[ 4 ] = IDC_TEST_SITE5;
	m_nIndexID[ 5 ] = IDC_TEST_SITE6;
	m_nIndexID[ 6 ] = IDC_TEST_SITE7;
	m_nIndexID[ 7 ] = IDC_TEST_SITE8;


// 	m_nSpreadTestSiteID[ 0 ][eFront] = IDC_SPREAD_SITE1;
// 	m_nSpreadTestSiteID[ 1 ] = IDC_SPREAD_SITE2;
// 	m_nSpreadTestSiteID[ 2 ] = IDC_SPREAD_SITE3;
// 	m_nSpreadTestSiteID[ 3 ] = IDC_SPREAD_SITE4;
// 	m_nSpreadTestSiteID[ 4 ] = IDC_SPREAD_SITE5;
// 	m_nSpreadTestSiteID[ 5 ] = IDC_SPREAD_SITE6;
// 	m_nSpreadTestSiteID[ 6 ] = IDC_SPREAD_SITE7;
// 	m_nSpreadTestSiteID[ 7 ] = IDC_SPREAD_SITE8;

	m_nSpreadTestSiteID[0][eFront] = IDC_SPREAD_YIELD_SITE1_FRONT;
	m_nSpreadTestSiteID[0][eRear] = IDC_SPREAD_YIELD_SITE1_REAR;
	m_nSpreadTestSiteID[1][eFront] = IDC_SPREAD_YIELD_SITE2_FRONT;
	m_nSpreadTestSiteID[1][eRear] = IDC_SPREAD_YIELD_SITE2_REAR;
	m_nSpreadTestSiteID[2][eFront] = IDC_SPREAD_YIELD_SITE3_FRONT;
	m_nSpreadTestSiteID[2][eRear] = IDC_SPREAD_YIELD_SITE3_REAR;
	m_nSpreadTestSiteID[3][eFront] = IDC_SPREAD_YIELD_SITE4_FRONT;
	m_nSpreadTestSiteID[3][eRear] = IDC_SPREAD_YIELD_SITE4_REAR;
	m_nSpreadTestSiteID[4][eFront] = IDC_SPREAD_YIELD_SITE5_FRONT;
	m_nSpreadTestSiteID[4][eRear] = IDC_SPREAD_YIELD_SITE5_REAR;
	m_nSpreadTestSiteID[5][eFront] = IDC_SPREAD_YIELD_SITE6_FRONT;
	m_nSpreadTestSiteID[5][eRear] = IDC_SPREAD_YIELD_SITE6_REAR;
	m_nSpreadTestSiteID[6][eFront] = IDC_SPREAD_YIELD_SITE7_FRONT;
	m_nSpreadTestSiteID[6][eRear] = IDC_SPREAD_YIELD_SITE7_REAR;
	m_nSpreadTestSiteID[7][eFront] = IDC_SPREAD_YIELD_SITE8_FRONT;
	m_nSpreadTestSiteID[7][eRear] = IDC_SPREAD_YIELD_SITE8_REAR;

	m_nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);

	m_nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	m_nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	if (nTesterOption == eTester_EachSocket)
		if (m_nTestSite_Div_Y == 1)
			m_nTesterCount = MAX_TESTER_SINGLE;
		else
			m_nTesterCount = MAX_TESTER_DUAL;
	else
		m_nTesterCount = MAX_TESTER_SINGLE;

// 	if (nTesterOption != eTester_Dual) // Single Tester [8/27/2021 donghyun.shin]
// 	{
// 		m_nTesterCount = MAX_TESTER_SINGLE;
// 		m_nConntct[0] = IDC_CLRCHK_CONNECT1;
// 		m_nConntct[1] = IDC_CLRCHK_CONNECT2;
// 		m_nConntct[2] = IDC_CLRCHK_CONNECT3;
// 		m_nConntct[3] = IDC_CLRCHK_CONNECT4;
// 		m_nConntct[4] = IDC_CLRCHK_CONNECT5;
// 		m_nConntct[5] = IDC_CLRCHK_CONNECT6;
// 		m_nConntct[6] = IDC_CLRCHK_CONNECT7;
// 		m_nConntct[7] = IDC_CLRCHK_CONNECT8;
// 	}
// 	else
// 	{
// 		m_nTesterCount = MAX_TESTER_DUAL;
// 		m_nConntct[0] = IDC_CLRCHK_CONNECT1;
// 		m_nConntct[1] = IDC_CLRCHK_CONNECT17;
// 		m_nConntct[2] = IDC_CLRCHK_CONNECT2;
// 		m_nConntct[3] = IDC_CLRCHK_CONNECT18;
// 		m_nConntct[4] = IDC_CLRCHK_CONNECT3;
// 		m_nConntct[5] = IDC_CLRCHK_CONNECT19;
// 		m_nConntct[6] = IDC_CLRCHK_CONNECT4;
// 		m_nConntct[7] = IDC_CLRCHK_CONNECT20;
// 		m_nConntct[8] = IDC_CLRCHK_CONNECT5;
// 	}

	if (nTesterOption == eTester_Single || m_nTestSite_Div_Y == 1)
	{
		m_nConntct[0] = IDC_CLRCHK_CONNECT1;
		m_nConntct[1] = IDC_CLRCHK_CONNECT2;
		m_nConntct[2] = IDC_CLRCHK_CONNECT3;
		m_nConntct[3] = IDC_CLRCHK_CONNECT4;
		m_nConntct[4] = IDC_CLRCHK_CONNECT5;
		m_nConntct[5] = IDC_CLRCHK_CONNECT6;
		m_nConntct[6] = IDC_CLRCHK_CONNECT7;
		m_nConntct[7] = IDC_CLRCHK_CONNECT8;
	}
	else {
		m_nConntct[0] = IDC_CLRCHK_CONNECT1;
		m_nConntct[1] = IDC_CLRCHK_CONNECT9;
		m_nConntct[2] = IDC_CLRCHK_CONNECT2;
		m_nConntct[3] = IDC_CLRCHK_CONNECT10;
		m_nConntct[4] = IDC_CLRCHK_CONNECT3;
		m_nConntct[5] = IDC_CLRCHK_CONNECT11;
		m_nConntct[6] = IDC_CLRCHK_CONNECT4;
		m_nConntct[7] = IDC_CLRCHK_CONNECT12;
		m_nConntct[8] = IDC_CLRCHK_CONNECT5;
		m_nConntct[9] = IDC_CLRCHK_CONNECT13;
		m_nConntct[10] = IDC_CLRCHK_CONNECT6;
		m_nConntct[11] = IDC_CLRCHK_CONNECT14;
		m_nConntct[12] = IDC_CLRCHK_CONNECT7;
		m_nConntct[13] = IDC_CLRCHK_CONNECT15;
		m_nConntct[14] = IDC_CLRCHK_CONNECT8;
		m_nConntct[15] = IDC_CLRCHK_CONNECT16;
	}
	


	m_nStationOnOff[0] = IDC_SPREAD_YIELD_STATION1;
	m_nStationOnOff[1] = IDC_SPREAD_YIELD_STATION2;
	m_nStationOnOff[2] = IDC_SPREAD_YIELD_STATION3;
	m_nStationOnOff[3] = IDC_SPREAD_YIELD_STATION4;
	m_nStationOnOff[4] = IDC_SPREAD_YIELD_STATION5;
	m_nStationOnOff[5] = IDC_SPREAD_YIELD_STATION6;
	m_nStationOnOff[6] = IDC_SPREAD_YIELD_STATION7;
	m_nStationOnOff[7] = IDC_SPREAD_YIELD_STATION8;

	memset(m_szCompany, 0x00, sizeof(m_szCompany));
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, m_szCompany, sizeof(m_szCompany));

	m_nRow = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y); //STATION_MAX_Y_SITE;
	m_nCol = STATION_MAX_X_SITE;



	m_nBefore_TestSite_Div_X = STATION_MAX_X_SITE;
	m_nBefore_TestSite_Div_Y = STATION_MAX_Y_SITE;

	m_veSiteIndex.clear();
	m_veSiteIndex.push_back(0);
	m_veSiteIndex.push_back(2);
	m_veSiteIndex.push_back(1);
	m_veSiteIndex.push_back(3);
	m_veSiteIndex.push_back(4);
	m_veSiteIndex.push_back(5);
	m_veSiteIndex.push_back(6);
	m_veSiteIndex.push_back(7);

}

CMainLower_TabYield::~CMainLower_TabYield()
{
}

void CMainLower_TabYield::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange( pDX );
	for (int i = 0; i < eMaxPressUnitCount; i++)
	{
		DDX_Control(pDX, m_nTestTmCtrlID[i], m_nTestTm[i]);	
	}
	for (int i = 0; i < m_nTesterCount; i++)
	{
		DDX_Control(pDX, m_nConntct[i], m_CtrlConnect[i]);
	}


	DDX_Control(pDX, IDC_BTN_YE_CLEAN_1, cButtonST_1);
	DDX_Control(pDX, IDC_BTN_YE_CLEAN_2, cButtonST_2);
	DDX_Control(pDX, IDC_BTN_YE_CLEAN_3, cButtonST_3);
	DDX_Control(pDX, IDC_BTN_YE_CLEAN_4, cButtonST_4);
	DDX_Control(pDX, IDC_BTN_YE_CLEAN_5, cButtonST_5);
	DDX_Control(pDX, IDC_BTN_YE_CLEAN_6, cButtonST_6);
	DDX_Control(pDX, IDC_BTN_YE_CLEAN_7, cButtonST_7);
	DDX_Control(pDX, IDC_BTN_YE_CLEAN_8, cButtonST_8);
}


BEGIN_MESSAGE_MAP( CMainLower_TabYield, CDialogEx )
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_YE_CLEAN_1, &CMainLower_TabYield::OnBnClickedBtnYeClean1)
	ON_BN_CLICKED(IDC_BTN_YE_CLEAN_2, &CMainLower_TabYield::OnBnClickedBtnYeClean2)
	ON_BN_CLICKED(IDC_BTN_YE_CLEAN_3, &CMainLower_TabYield::OnBnClickedBtnYeClean3)
	ON_BN_CLICKED(IDC_BTN_YE_CLEAN_4, &CMainLower_TabYield::OnBnClickedBtnYeClean4)
	ON_BN_CLICKED(IDC_BTN_YE_CLEAN_5, &CMainLower_TabYield::OnBnClickedBtnYeClean5)
	ON_BN_CLICKED(IDC_BTN_YE_CLEAN_6, &CMainLower_TabYield::OnBnClickedBtnYeClean6)
	ON_BN_CLICKED(IDC_BTN_YE_CLEAN_7, &CMainLower_TabYield::OnBnClickedBtnYeClean7)
	ON_BN_CLICKED(IDC_BTN_YE_CLEAN_8, &CMainLower_TabYield::OnBnClickedBtnYeClean8)

	ON_STN_DBLCLK(IDC_STATIC_YD_TEXT_STATION1, &CMainLower_TabYield::OnStnDblclickStation1)
	ON_STN_DBLCLK(IDC_STATIC_YD_TEXT_STATION2, &CMainLower_TabYield::OnStnDblclickStation2)
	ON_STN_DBLCLK(IDC_STATIC_YD_TEXT_STATION3, &CMainLower_TabYield::OnStnDblclickStation3)
	ON_STN_DBLCLK(IDC_STATIC_YD_TEXT_STATION4, &CMainLower_TabYield::OnStnDblclickStation4)
	ON_STN_DBLCLK(IDC_STATIC_YD_TEXT_STATION5, &CMainLower_TabYield::OnStnDblclickStation5)
	ON_STN_DBLCLK(IDC_STATIC_YD_TEXT_STATION6, &CMainLower_TabYield::OnStnDblclickStation6)
	ON_STN_DBLCLK(IDC_STATIC_YD_TEXT_STATION7, &CMainLower_TabYield::OnStnDblclickStation7)
	ON_STN_DBLCLK(IDC_STATIC_YD_TEXT_STATION8, &CMainLower_TabYield::OnStnDblclickStation8)

	ON_MESSAGE(UM_SITE_YIELD_HOLD, OnSiteHold)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CMainLower_TabYield, CDialogEx)
	ON_EVENT(CMainLower_TabYield, IDC_SPREAD_YIELD_STATION1, 11, CMainLower_TabYield::DblClickSpreadStation1, VTS_I4 VTS_I4)
	ON_EVENT(CMainLower_TabYield, IDC_SPREAD_YIELD_STATION2, 11, CMainLower_TabYield::DblClickSpreadStation2, VTS_I4 VTS_I4)
	ON_EVENT(CMainLower_TabYield, IDC_SPREAD_YIELD_STATION3, 11, CMainLower_TabYield::DblClickSpreadStation3, VTS_I4 VTS_I4)
	ON_EVENT(CMainLower_TabYield, IDC_SPREAD_YIELD_STATION4, 11, CMainLower_TabYield::DblClickSpreadStation4, VTS_I4 VTS_I4)
	ON_EVENT(CMainLower_TabYield, IDC_SPREAD_YIELD_STATION5, 11, CMainLower_TabYield::DblClickSpreadStation5, VTS_I4 VTS_I4)
	ON_EVENT(CMainLower_TabYield, IDC_SPREAD_YIELD_STATION6, 11, CMainLower_TabYield::DblClickSpreadStation6, VTS_I4 VTS_I4)
	ON_EVENT(CMainLower_TabYield, IDC_SPREAD_YIELD_STATION7, 11, CMainLower_TabYield::DblClickSpreadStation7, VTS_I4 VTS_I4)
	ON_EVENT(CMainLower_TabYield, IDC_SPREAD_YIELD_STATION8, 11, CMainLower_TabYield::DblClickSpreadStation8, VTS_I4 VTS_I4)


	ON_EVENT(CMainLower_TabYield, IDC_SPREAD_YIELD_STATION1, 22, CMainLower_TabYield::RightClickSpreadStation1, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CMainLower_TabYield, IDC_SPREAD_YIELD_STATION2, 22, CMainLower_TabYield::RightClickSpreadStation2, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CMainLower_TabYield, IDC_SPREAD_YIELD_STATION3, 22, CMainLower_TabYield::RightClickSpreadStation3, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CMainLower_TabYield, IDC_SPREAD_YIELD_STATION4, 22, CMainLower_TabYield::RightClickSpreadStation4, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CMainLower_TabYield, IDC_SPREAD_YIELD_STATION5, 22, CMainLower_TabYield::RightClickSpreadStation5, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CMainLower_TabYield, IDC_SPREAD_YIELD_STATION6, 22, CMainLower_TabYield::RightClickSpreadStation6, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CMainLower_TabYield, IDC_SPREAD_YIELD_STATION7, 22, CMainLower_TabYield::RightClickSpreadStation7, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CMainLower_TabYield, IDC_SPREAD_YIELD_STATION8, 22, CMainLower_TabYield::RightClickSpreadStation8, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

// CMainLower_TabYield 메시지 처리기입니다.


BOOL CMainLower_TabYield::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	position_setup();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	for( int i = 0; i < eMaxPressUnitCount; i++ )
	{
		m_nTestTm[ i ].m_fnSetFont( "Fixedsys" );
		m_nTestTm[ i ].SetTextSize( 13 );
		m_nTestTm[ i ].SetBackColor( COLOR_NOMAL );
	}

	StationParaSetting();

	SetTimer(0, 1000, NULL);

	CompanySetting();
	RefreshCleaning();

	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
	if (g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y) == 1 && nBoardType == eBoardDir_Backward)
	{
		GetDlgItem(IDC_SPREAD_YIELD_SITE1_FRONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_YIELD_SITE2_FRONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_YIELD_SITE3_FRONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_YIELD_SITE4_FRONT)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_SPREAD_YIELD_SITE5_FRONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_YIELD_SITE6_FRONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_YIELD_SITE7_FRONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_YIELD_SITE8_FRONT)->ShowWindow(SW_HIDE);

	}
	else if (g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y) == 1 && nBoardType == eBoardDir_Forward)
	{
		GetDlgItem(IDC_SPREAD_YIELD_SITE1_REAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_YIELD_SITE2_REAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_YIELD_SITE3_REAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_YIELD_SITE4_REAR)->ShowWindow(SW_HIDE);
							
		GetDlgItem(IDC_SPREAD_YIELD_SITE5_REAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_YIELD_SITE6_REAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_YIELD_SITE7_REAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_YIELD_SITE8_REAR)->ShowWindow(SW_HIDE);
	}

	//int nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	CString strTmp = _T("");
	for (int i = 0; i < m_nTesterCount; i++)
	{
		int nIdx = 0;
		if (nBoardType == eBoardDir_Backward) {
			if (g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y) == 2)
			{
				if (i < 8) {
					if (i % 2 == 0) {//짝수
						nIdx = i + 1;
					}
					else {
						nIdx = i - 1;
					}
				}
				else {
					nIdx = i;
				}
			}
			else
			{
				nIdx = i;
			}
		}
		else { //eBoardDir_Forward , 작업 필요.
			nIdx = i;
		}

		strTmp.Format("%d", i + 1);
		SetDlgItemText(m_nConntct[nIdx], strTmp);
		m_CtrlConnect[nIdx].SetAlignment(7);
		m_CtrlConnect[nIdx].SetForeColor(COLOR_DDARKGRAY);
	}

	if (nBoardType == eBoardDir_Backward) {
		CSpreadSheet* pSprd = NULL;
		for (int nPressIdx = 0; nPressIdx < eMaxPressUnitCount; nPressIdx++)
		{
			for (int nSpreadIdx = 0; nSpreadIdx < eMaxSpreadIdx; nSpreadIdx++)
			{
				pSprd = (CSpreadSheet*)GetDlgItem(m_nSpreadTestSiteID[nPressIdx][nSpreadIdx]);
				pSprd->SetBorderStyle(8);

				for (int nRow = 0; nRow < eMaxPressUnitCount / 2; nRow++) {
					CString strNo = _T("");
					if (nPressIdx <= ePressUnit_Down_4) {
						if (nSpreadIdx == eFront) {
							strNo.Format("%d", nRow + 5); //5,6,7,8
						}
						else {
							strNo.Format("%d", nRow + 1); //1,2,3,4
						}
					}
					else {
						if (nSpreadIdx == eFront) {  // 8,7,6,5
							strNo.Format("%d", abs(nRow - 8));
						}
						else { //4,3,2,1
							strNo.Format("%d", abs(nRow - 4));
						}
					}

					//GUI 보이는 첫번째 Row가 1
					pSprd->SetTextMatrix(nRow + 1, 0, strNo);
				}

			}
		}
	}

	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	if (nTesterOption == eTester_Single || m_nTestSite_Div_Y == 1)
	{
		GetDlgItem(IDC_CLRCHK_CONNECT9)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CLRCHK_CONNECT10)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CLRCHK_CONNECT11)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CLRCHK_CONNECT12)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CLRCHK_CONNECT13)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CLRCHK_CONNECT14)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CLRCHK_CONNECT15)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CLRCHK_CONNECT16)->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMainLower_TabYield::Refresh()
{
	UpdateData();

	char m_szCompany[32];

	memset(m_szCompany, 0x00, sizeof(m_szCompany));
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, m_szCompany, sizeof(m_szCompany));

	int nSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nSite = 0;
	//CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// Show Test Site Boot Time, BinCount, 
	CString strData;
	int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN( NDM11_MD_FTRTQCContinue );

	for( int Index = 0; Index < eMaxPressUnitCount; Index++ ) {
		if( g_DMCont.m_dmEQP.GB( BDM0_TEST_STATUS_SITE1 + Index ) == TRUE )
		{
			m_nTestTm[ Index ].SetBackColor( COLOR_YELLOW );
		}
		else {
			m_nTestTm[ Index ].SetBackColor( COLOR_NOMAL );
		}
	}

	int nMaxSiteCountPerPress = ( STATION_MAX_X_SITE*STATION_MAX_Y_SITE );
	int nPressIndex = 0;
	int nSiteIndex = 0;
	BOOL bIsSocketStatus = FALSE;
	CSpreadSheet* pSprd = NULL;
	
	for (int i = 0; i < (STATION_MAX_PARA*eMaxPressUnitCount); i++) {
		strData.Empty();

		//GRR은 사용안함.
		//GRR은 사용안함.
		if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
		{
			m_nSocketOnOff[i] = g_DMCont.m_dmHandlerOpt.GN(NDM11_GRR_SocketOnOff_Site1_1 + i);
		}
		else
		{
			m_nSocketOnOff[i] = g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + i);
		}
		//m_nSocketOnOff[ i ] = g_DMCont.m_dmHandlerOpt.GN( NDM11_SocketOnOff_Site1_1 + i );
		bIsSocketStatus |= (m_nSocketOnOff[i] == DEF_SITE_ON || m_nSocketOnOff[i] == DEF_SITE_OFF_RESERVATION) ? TRUE : FALSE; //OR 연산 시켜서 하나라도 켜져있는지 확인함.

		nPressIndex = i / nMaxSiteCountPerPress; //Match Index
		nSiteIndex = i % nMaxSiteCountPerPress;

		int nSpreadIdx = eFront;
		if (nSiteIndex > 3)
			nSpreadIdx = eRear;

		//int nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
		int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
		if (nBoardType == eBoardDir_Backward) {
			nSpreadIdx = eRear;
			if (nSiteIndex > 3)
				nSpreadIdx = eFront;
		}


		pSprd = (CSpreadSheet*)GetDlgItem(m_nSpreadTestSiteID[nPressIndex][nSpreadIdx]);
		if ((i % nMaxSiteCountPerPress) == STATION_MAX_PARA - 1)
		{
			pSprd->SetMaxRows(nMaxSiteCountPerPress);
			m_dTestTime[nPressIndex] = g_DMCont.m_dmEQP.GN(NDM0_Press1_TestTm + nPressIndex) / 1000.0;
			if (bIsSocketStatus == DEF_SITE_ON)
			{
				strData.Format("Index=%.2f", g_DMCont.m_dmEQP.GD(DDM0_TmIndexPress1 + nPressIndex));
				SetDlgItemText(m_nIndexID[nPressIndex], strData);
				strData.Format("TestTime=%.2f", m_dTestTime[nPressIndex]);
				m_nTestTm[nPressIndex].SetWindowTextA(strData);
			}
			else
			{
				SetDlgItemText(m_nIndexID[nPressIndex], _T("-"));
				m_nTestTm[nPressIndex].SetWindowTextA(_T("OFF"));
			}
			bIsSocketStatus = FALSE;
		}

		m_nFailBinCnt[i] = g_DMCont.m_dmContac_Dn.GN(NDM1_FailBinCnt_Press1_1 + nSiteIndex + (nPressIndex * (nMaxSiteCountPerPress)));
		m_nClsAfterFailBinCnt[i] = g_DMCont.m_dmContac_Dn.GN(NDM1_ClsAfterFailBinCntPress1_1 + nSiteIndex + (nPressIndex * (nMaxSiteCountPerPress)));
		m_nGoodBinCnt[i] = g_DMCont.m_dmContac_Dn.GN(NDM1_GdBinCnt_Press1_1 + nSiteIndex + (nPressIndex * (nMaxSiteCountPerPress)));
		m_nClsAfterGoodBinCnt[i] = g_DMCont.m_dmContac_Dn.GN(NDM1_ClrAfterGdBinCntPress1_1 + nSiteIndex + (nPressIndex * (nMaxSiteCountPerPress)));
		m_n1stFailBinCnt[i] = g_DMCont.m_dmContac_Up.GN(NDM2_1STFailBinCnt_Press1_1 + nSiteIndex + (nPressIndex * (nMaxSiteCountPerPress)));
		m_n2DIDFailBinCnt[i] = g_DMCont.m_dmContac_Up.GN(NDM2_2DIDFailBinCnt_Press1_1 + nSiteIndex + (nPressIndex * (nMaxSiteCountPerPress)));

		int    nClrTotalContact = m_nClsAfterFailBinCnt[i] + m_nClsAfterGoodBinCnt[i];
		double dClrGoodYield = (double)m_nClsAfterGoodBinCnt[i] / (double)(nClrTotalContact)*100.0;
		double dClr1stTFYield = (double)m_n1stFailBinCnt[i] / (double)nClrTotalContact * 100.0;

		// Good Bin Count & Fail Bin Count
		//strData.Format("G[%d]/F[%d]",m_nClsAfterGoodBinCnt[i], m_nClsAfterFailBinCnt[i]);
			// Boot Time & Test Time
			//strData.Format("B[%ds]/T[%.2fs]", 0, m_dTestTime[i]);	

 		int nCol = 0;
 		int nRow[4] = { 0, };
 
		if (nBoardType == eBoardDir_Forward) {
			for (int i = 0; i < eRow4; i++) {
				if (nPressIndex <= ePressUnit_Down_4) {
					nRow[i] = eRow4 - i; //4, 3, 2, 1
				}
				else {
					nRow[i] = eRow1 + i; //1, 2, 3, 4
				}
			}
		}
		else {
			for (int i = 0; i < eRow4; i++) {
				if (nPressIndex <= ePressUnit_Down_4) {
					nRow[i] = eRow1 + i; //1, 2, 3, 4
				}
				else {
					nRow[i] = eRow4 - i; //4, 3, 2, 1
				}
			}
		}
 		int nRowidx = 0;
 		if (nSiteIndex > 3)
 			nRowidx = 4;


		if ((m_nSocketOnOff[i] == DEF_SITE_ON || m_nSocketOnOff[i] == DEF_SITE_OFF_RESERVATION)) {
			if (m_eYieldMode == YM_COUNT) {
				strData.Format("P:%d / F:%d", m_nClsAfterGoodBinCnt[i], m_nClsAfterFailBinCnt[i]);
				pSprd->SetTextMatrix(nRow[nSiteIndex - nRowidx], 1, strData);
//				pSprd->SetTextMatrix(nCalRow, 1, strData);
			}
			else if (m_eYieldMode == YM_1ST_FAIL) {
				strData.Format("F:%d / T:%d", m_n1stFailBinCnt[i], nClrTotalContact /*(nClrTotalContact <= 0 ? 0.00 : dClr1stTFYield)*/);
				pSprd->SetTextMatrix(nRow[nSiteIndex - nRowidx], 1, strData);
//				pSprd->SetTextMatrix(nCalRow, 1, strData);
			}
			else if (m_eYieldMode == YM_2DID_FAIL) {
				strData.Format("2DID Fail:%d", m_n2DIDFailBinCnt[i]);
				pSprd->SetTextMatrix(nRow[nSiteIndex - nRowidx], 1, strData);
//				pSprd->SetTextMatrix(nCalRow, 1, strData);
			}
			else {
				strData.Format("%d / %.2f%%", nClrTotalContact, (nClrTotalContact <= 0 ? 0.00 : dClrGoodYield));
				pSprd->SetTextMatrix(nRow[nSiteIndex - nRowidx], 1, strData);
//				pSprd->SetTextMatrix(nCalRow, 1, strData);

// 				if (strcmp(m_szCompany, DEF_COMPANY_AMKOR) == 0)
// 				{
					if (dClrGoodYield >= 90)
						pSprd->SetBackColor(RGB(30, 230, 30)); //green
					else if (dClrGoodYield < 90 && dClrGoodYield >= 80)
						pSprd->SetBackColor(RGB(150, 200, 255)); // blue
					else if (dClrGoodYield < 80 && dClrGoodYield >= 70)
						pSprd->SetBackColor(RGB(255, 190, 100)); //yellow
					else
						pSprd->SetBackColor(RGB(255, 50, 50)); // red
//				}
			}
		}
		else {
			pSprd->SetTextMatrix(nRow[nSiteIndex - nRowidx], 1, "-");
// 			if (strcmp(m_szCompany, DEF_COMPANY_AMKOR) == 0)
// 			{
				if (m_eYieldMode == YM_COUNT) {
					strData.Format("P:%d / F:%d", m_nClsAfterGoodBinCnt[i], m_nClsAfterFailBinCnt[i]);
					pSprd->SetTextMatrix(nRow[nSiteIndex - nRowidx], 1, strData);
				}
				else if (m_eYieldMode == YM_1ST_FAIL) {
					strData.Format("1st Fail:%d", m_n1stFailBinCnt[i]);
					pSprd->SetTextMatrix(nRow[nSiteIndex - nRowidx], 1, strData);
				}
				else if (m_eYieldMode == YM_2DID_FAIL) {
					strData.Format("2DID Fail:%d", m_n2DIDFailBinCnt[i]);
					pSprd->SetTextMatrix(nRow[nSiteIndex - nRowidx], 1, strData);
				}
				else {
					strData.Format("%d / %.2f%%", nClrTotalContact, (nClrTotalContact <= 0 ? 0.00 : dClrGoodYield));
					pSprd->SetTextMatrix(nRow[nSiteIndex - nRowidx], 1, strData);

					if (dClrGoodYield >= 90)
						pSprd->SetBackColor(RGB(30, 230, 30));
					else if (dClrGoodYield < 90 && dClrGoodYield >= 80)
						pSprd->SetBackColor(RGB(150, 200, 255));
					else if (dClrGoodYield < 80 && dClrGoodYield >= 70)
						pSprd->SetBackColor(RGB(255, 190, 100));
					else
						pSprd->SetBackColor(RGB(255, 50, 50));

				}
//			}
		}
	}

	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	RefreshCleaning();
	pSprd = NULL;
}


void CMainLower_TabYield::position_setup()
{
	CWnd* parent = this->GetParent();
	ASSERT( parent );

	CRect parent_client_area;
	parent->GetClientRect( &parent_client_area );

	CRect tab_size;
	( ( CTabCtrl* )parent )->GetItemRect( 0, &tab_size );
	int tab_height = tab_size.Height() + 5;
	const int margine = 5;

	this->SetWindowPos(
		NULL,
		margine, (/*tab_height+*/margine ),
		parent_client_area.Width() - ( margine * 2 ), parent_client_area.Height() - ( tab_height ),//-(margine/* *2 */)),
		SWP_NOZORDER );
}

void CMainLower_TabYield::RefreshConnection()
{
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);

	for (int i = 0; i < m_nTesterCount; i++)
	{
		BOOL bConnect = FALSE;
		int nTesterConnectCnt = 0;
		for (int nTesterCnt = 0; nTesterCnt < EACH_LINE_STATION_CNT; nTesterCnt++)
		{
			if (nTesterOption == eTester_EachSocket)
			{
				int nIdx = ((i* EACH_LINE_STATION_CNT) + nTesterCnt);
				BOOL bTesterConnect = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + nIdx);
				if (bTesterConnect == DEF_ON)
				{
					nTesterConnectCnt++;
					if (nTesterConnectCnt == EACH_LINE_STATION_CNT)
					{
						bConnect = TRUE;
					}
				}
			}				
		}
		if (nTesterOption != eTester_EachSocket)
		{
			if(nTesterOption == eTester_Agent)
				bConnect = g_DMCont.m_dmEQP.GB(BDM0_AGENT_CONNECTION_1 + i);
			else{
				bConnect = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + i);		
			}
			if (bConnect)
				m_CtrlConnect[i].On();
			else
				m_CtrlConnect[i].Off();
		}

		if (nTesterOption == eTester_EachSocket)
		{
			if (bConnect)
				m_CtrlConnect[i].On();
			else
				m_CtrlConnect[i].Off();
		}

		if (nTesterOption == eTester_EachSocket)
		{
			int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
			int nIdx = 0;
			if (nBoardType == eBoardDir_Backward) {
				if (g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y) == 2)
				{
					if (i < 8) {
						if (i % 2 == 0) {//짝수
							nIdx = i + 1;
						}
						else {
							nIdx = i - 1;
						}
					}
					else {
						nIdx = i;
					}
				}
				else
				{
					nIdx = i;
				}
			}
			else { //eBoardDir_Forward , 작업 필요.
				nIdx = i;
			}

			if (bConnect)
			{
				m_CtrlConnect[nIdx].On();
			}
			else
			{
				m_CtrlConnect[nIdx].Off();
			}
		}
		else
		{
			CString str;
			str.Format("%d", i + 1);
			m_CtrlConnect[i].SetWindowText(str);
			// 			if (i % 2 == 0)
			// 				m_CtrlConnect[i].SetWindowText("1");
			// 			else
			// 				m_CtrlConnect[i].SetWindowText("2");
		}
	}
}

void CMainLower_TabYield::RefreshSiteOnOff()
{
	CSpreadSheet* pSprd = NULL;

	int nTestSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nTestSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);


	int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	int nSite = NDM11_SocketOnOff_Site1_1;
	if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
	{
		nSite = NDM11_GRR_SocketOnOff_Site1_1;
	}

	CString strSiteNum = _T("");
	for (int Index = 0; Index < eMaxPressUnitCount; Index++) 
	{
		pSprd = (CSpreadSheet*)GetDlgItem(m_nStationOnOff[Index]);

		//int nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
		int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
		int nSpreadIdx = eOnOff_Spread_Type1;
		if (Index > ePressUnit_Down_4)
			nSpreadIdx = eOnOff_Spread_Type2;

		//Lowland 임시 처리
		if (nBoardType == eBoardDir_Backward) {
			nSpreadIdx = eOnOff_Spread_Type2;
			if (Index > ePressUnit_Down_4)
				nSpreadIdx = eOnOff_Spread_Type1;
		}


		for (int nX = 0; nX < m_nTestSite_Div_Y; nX++)
		{
			for (int nY = 0; nY < m_nTestSite_Div_X; nY++)
			{
				int nNum = 0;
// 				if (m_nProjectOption != DEF_PROJECT_TPUT)
// 					nNum = nSite + STATION_MAX_PARA*Index + nX + nY*STATION_MAX_X_SITE;
// 				else
// 					nNum = nSite + STATION_MAX_PARA*Index + m_veSiteIndex[nX] + nY*STATION_MAX_X_SITE;

				if (g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y) == STATION_MAX_Y_SITE)
				{
					if (nBoardType == eBoardDir_Forward) {
						if (Index <= ePressUnit_Down_4) {
							nNum = nSite + STATION_MAX_PARA*Index + abs(nY - 3) + nX * 4;
						}
						else {
							nNum = nSite + STATION_MAX_PARA*Index + nY + abs(nX - 1) * 4;
						}
					}
					else {
						if (Index <= ePressUnit_Down_4) {
							nNum = nSite + STATION_MAX_PARA*Index + nY + abs(nX - 1) * 4;

						}
						else {
							nNum = nSite + STATION_MAX_PARA*Index + abs(nY - 3) + nX * 4;
						}
					}
				}
				else if (g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y) == 1)
				{
					if (nBoardType == eBoardDir_Forward) {
						if (Index <= ePressUnit_Down_4) {
							nNum = nSite + STATION_MAX_PARA*Index + abs(nY - 3) + nX * 4;
						}
						else {
							//nNum = nSite + STATION_MAX_PARA*Index + nY + abs(nX - 1) * 4;
							nNum = nSite + STATION_MAX_PARA*Index + (nY + (nX * 4));
						}
					}
					else {
						if (Index <= ePressUnit_Down_4) {
							nNum = nSite + STATION_MAX_PARA*Index + (nY + (nX * 4));

						}
						else {
							nNum = nSite + STATION_MAX_PARA*Index + abs(nY - 3) + nX * 4;
						}
					}
				}
				int nSiteOnOff = g_DMCont.m_dmHandlerOpt.GN(nNum);
				pSprd->ClearSelection();
				CString strOnOff = m_nTestSite_Div_Y == 1 ? m_strOnOffSpreadTypeY1[nSpreadIdx][nY][nX] : m_strOnOffSpreadType[nSpreadIdx][nY][nX];
				if (nSiteOnOff == DEF_SITE_ON)
				{
//					strSiteNum.Format("%d", (nY * STATION_MAX_X_SITE) + nX + 1);
					pSprd->SetTextMatrix(nY + 1, nX + 1, strOnOff);
					pSprd->SetBackColor(COLOR_GREEN);
				}
				else if (nSiteOnOff == DEF_SITE_OFF_RESERVATION)
				{
//					strSiteNum.Format("%d", (nY * STATION_MAX_X_SITE) + nX + 1);
					pSprd->SetTextMatrix(nY + 1, nX + 1, strOnOff);
					pSprd->SetBackColor(COLOR_YELLOW);
				}
				else if (nSiteOnOff == DEF_SITE_SOCKET_RETEST_OK)
				{
//					strSiteNum.Format("%d", (nY * STATION_MAX_X_SITE) + nX + 1);
					pSprd->SetTextMatrix(nY + 1, nX + 1, strOnOff);
					pSprd->SetBackColor(COLOR_BLUE);
				}
				else if (nSiteOnOff == DEF_SITE_SOCKET_HOLD)
				{
//					strSiteNum.Format("%d", (nY * STATION_MAX_X_SITE) + nX + 1);
					pSprd->SetTextMatrix(nY + 1, nX + 1, strOnOff);
					pSprd->SetBackColor(COLOR_BBLACK);
				}
				else if (nSiteOnOff == DEF_SITE_SOCKET_LOCK)
				{
//					strSiteNum.Format("%d", (nY * STATION_MAX_X_SITE) + nX + 1);
					pSprd->SetTextMatrix(nY + 1, nX + 1, strOnOff);
					pSprd->SetBackColor(COLOR_BLUE);
				}
				else if (nSiteOnOff == DEF_SITE_OFF_RESERVATION_YIELD)
				{
					strSiteNum.Format("%d", (nY * STATION_MAX_X_SITE) + nX + 1);
					pSprd->SetTextMatrix(nY + 1, nX + 1, "R");
					pSprd->SetBackColor(COLOR_YELLOW);
				}
				else
				{					
					pSprd->SetTextMatrix(nY+1, nX+1, "X");

					char szCompany[16] = { 0, };
					g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

					if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0) {

						if (nSiteOnOff == DEF_SITE_OFF) {
							pSprd->SetBackColor(COLOR_LLIGHTGRAY);
						}
						else if (nSiteOnOff == DEF_SITE_OFF_YIELD) {
							pSprd->SetBackColor(COLOR_DDARKYELLOW);
						}
						else if (nSiteOnOff == DEF_SITE_OFF_ERRORCODE) {
							pSprd->SetBackColor(COLOR_RED);
						}
						else if (nSiteOnOff == DEF_SITE_SOCKET_HOLD) {
							pSprd->SetBackColor(COLOR_BBLACK);
						}
						else if (nSiteOnOff == DEF_SITE_SOCKET_LOCK) {
							pSprd->SetBackColor(COLOR_BLUE);
						}
					}
					else {
						pSprd->SetBackColor(COLOR_LLIGHTGRAY);
					}
				}
					
			}
		}
	}

	pSprd = NULL;
}


void CMainLower_TabYield::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 0)
	{
		KillTimer(nIDEvent);

		RefreshConnection();
		RefreshSiteOnOff();

		SetTimer(0, 2000, NULL);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CMainLower_TabYield::StationParaSetting()
{
	CSpreadSheet* pSprd = NULL;
	int nHistoryHeight = 12;
	int nSiteOnOffHeight = 9.5;
	int nSiteOnOffRow = 8;

	m_nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	m_nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	if (m_nBefore_TestSite_Div_X != m_nTestSite_Div_X || m_nBefore_TestSite_Div_Y != m_nTestSite_Div_Y) {

		m_nBefore_TestSite_Div_X = m_nTestSite_Div_X;
		m_nBefore_TestSite_Div_Y = m_nTestSite_Div_Y;


		if (m_nTestSite_Div_Y == 1) {
			nHistoryHeight = 26;
			nSiteOnOffHeight = 6;

		}
		else if (m_nTestSite_Div_Y == STATION_MAX_Y_SITE) {
			nHistoryHeight = 12;
			nSiteOnOffHeight = 10;
		}

		for (int i = 0; i < eMaxPressUnitCount; i++) {
			CString strSpreadStation[4][2] = { { _T("4"), _T("8") }
												,{ _T("3"), _T("7") }
												,{ _T("2"), _T("6") }
												,{ _T("1"), _T("5") } };
// 			pSprd = (CSpreadSheet*)GetDlgItem(m_nSpreadTestSiteID[i]);
// 			pSprd->SetMaxRows(m_nTestSite_Div_X * m_nTestSite_Div_Y);
// 
// 			for (int nRow = 1; nRow <= m_nTestSite_Div_X; nRow++) {
// 				pSprd->SetRowHeight(nRow, nHistoryHeight);
// 			}
// 
// 			pSprd = NULL;

			pSprd = (CSpreadSheet*)GetDlgItem(m_nStationOnOff[i]);
			pSprd->SetMaxRows(m_nTestSite_Div_X);
			pSprd->SetMaxCols(m_nTestSite_Div_Y);

			for (int nRow = 1; nRow <= m_nTestSite_Div_X; nRow++) {
//				pSprd->SetRowHeight(nRow, nSiteOnOffHeight);

				for (int nCol = 1; nCol <= m_nTestSite_Div_Y; nCol++) {
					pSprd->SetColWidth(nCol, nSiteOnOffHeight);
					pSprd->SetCol(nCol);
					pSprd->SetRow(nRow);
					pSprd->SetTypeHAlign(2);//center
				}
			}

			pSprd = NULL;
		}
	}
}

void CMainLower_TabYield::OnBnClickedBtnYeClean1()
{
	SetRevClean(ePressUnit_Down_1);
}


void CMainLower_TabYield::OnBnClickedBtnYeClean2()
{
	SetRevClean(ePressUnit_Down_2);
}


void CMainLower_TabYield::OnBnClickedBtnYeClean3()
{
	SetRevClean(ePressUnit_Down_3);
}


void CMainLower_TabYield::OnBnClickedBtnYeClean4()
{
	SetRevClean(ePressUnit_Down_4);
}


void CMainLower_TabYield::OnBnClickedBtnYeClean5()
{
	SetRevClean(ePressUnit_Down_5);
}


void CMainLower_TabYield::OnBnClickedBtnYeClean6()
{
	SetRevClean(ePressUnit_Down_6);
}


void CMainLower_TabYield::OnBnClickedBtnYeClean7()
{
	SetRevClean(ePressUnit_Down_7);
}


void CMainLower_TabYield::OnBnClickedBtnYeClean8()
{
	SetRevClean(ePressUnit_Down_8);
}


void CMainLower_TabYield::OnStnDblclickStation1()
{
	int nStation = ePressUnit_Down_1;
	int Col = m_nCol;
	int Row = m_nRow;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, TRUE);
}

void CMainLower_TabYield::OnStnDblclickStation2()
{
	int nStation = ePressUnit_Down_2;
	int Col = m_nCol;
	int Row = m_nRow;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, TRUE);
}


void CMainLower_TabYield::OnStnDblclickStation3()
{
	int nStation = ePressUnit_Down_3;
	int Col = m_nCol;
	int Row = m_nRow;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, TRUE);
}


void CMainLower_TabYield::OnStnDblclickStation4()
{
	int nStation = ePressUnit_Down_4;
	int Col = m_nCol;
	int Row = m_nRow;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, TRUE);
}


void CMainLower_TabYield::OnStnDblclickStation5()
{
	int nStation = ePressUnit_Down_5;
	int Col = m_nCol;
	int Row = m_nRow;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, TRUE);
}


void CMainLower_TabYield::OnStnDblclickStation6()
{
	int nStation = ePressUnit_Down_6;
	int Col = m_nCol;
	int Row = m_nRow;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, TRUE);
}


void CMainLower_TabYield::OnStnDblclickStation7()
{
	int nStation = ePressUnit_Down_7;
	int Col = m_nCol;
	int Row = m_nRow;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, TRUE);
}


void CMainLower_TabYield::OnStnDblclickStation8()
{
	int nStation = ePressUnit_Down_8;
	int Col = m_nCol;
	int Row = m_nRow;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, TRUE);
}


void CMainLower_TabYield::SetRevClean(int nPressIndex)
{
	BOOL bCleanOption = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_SocketClean);
	if (bCleanOption != eOPTION_USE)
		return;

	BOOL bOneCycle = g_DMCont.m_dmEQP.GB(BDM0_SYS_ONE_CYCLE);
	if (bOneCycle == TRUE)
		return;

	int nMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	if (nMode == eStartMD_AutoGRR || nMode == eStartMD_AutoGRR_Continue) 
	{
		return;
	}
	if (g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE1 + nPressIndex))
		g_DMCont.m_dmHandlerOpt.SB(BDM11_MANU_CLN_REV_SITE1 + nPressIndex, FALSE);
	else
		g_DMCont.m_dmHandlerOpt.SB(BDM11_MANU_CLN_REV_SITE1 + nPressIndex, TRUE);

	CString strLog = _T("");
	strLog.Format("Manual Clean reserve Station=%d ", nPressIndex + 1);
	TWLOG.MakeLog(static_cast<int>(eLogID_System_GUI), Debug, "", __LINE__, NULL, strLog);
}

void CMainLower_TabYield::RefreshCleaning()
{
	BOOL bUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_SocketClean);
	if (!bUseMode)
	{
		for (int i = 0; i < eMaxPressUnitCount; i++)
		{
			g_DMCont.m_dmHandlerOpt.SB(BDM11_MANU_CLN_REV_SITE1 + i, FALSE);
		}
	}

	BOOL bRev = g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE1);
	bRev |= g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE1_RESERVE);
	if (bRev == TRUE)
		cButtonST_1.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_GREEN);
	else
		cButtonST_1.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_LLIGHTGRAY);

	bRev = g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE2);
	bRev |= g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE2_RESERVE);
	if (bRev == TRUE)
		cButtonST_2.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_GREEN);
	else
		cButtonST_2.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_LLIGHTGRAY);

	bRev = g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE3);
	bRev |= g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE3_RESERVE);
	if (bRev == TRUE)
		cButtonST_3.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_GREEN);
	else
		cButtonST_3.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_LLIGHTGRAY);

	bRev = g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE4);
	bRev |= g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE4_RESERVE);
	if (bRev == TRUE)
		cButtonST_4.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_GREEN);
	else
		cButtonST_4.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_LLIGHTGRAY);

	bRev = g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE5);
	bRev |= g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE5_RESERVE);
	if (bRev == TRUE)
		cButtonST_5.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_GREEN);
	else
		cButtonST_5.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_LLIGHTGRAY);

	bRev = g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE6);
	bRev |= g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE6_RESERVE);
	if (bRev == TRUE)
		cButtonST_6.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_GREEN);
	else
		cButtonST_6.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_LLIGHTGRAY);

	bRev = g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE7);
	bRev |= g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE7_RESERVE);
	if (bRev == TRUE)
		cButtonST_7.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_GREEN);
	else
		cButtonST_7.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_LLIGHTGRAY);

	bRev = g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE8);
	bRev |= g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE8_RESERVE);
	if (bRev == TRUE)
		cButtonST_8.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_GREEN);
	else
		cButtonST_8.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_LLIGHTGRAY);

// 	bRev = g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE9);
// 	bRev |= g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE9_RESERVE);
// 	if (bRev == TRUE)
// 		cButtonST_9.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_GREEN);
// 	else
// 		cButtonST_9.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_LLIGHTGRAY);
// 
// 	bRev = g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE10);
// 	bRev |= g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE10_RESERVE);
// 	if (bRev == TRUE)
// 		cButtonST_10.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_GREEN);
// 	else
// 		cButtonST_10.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_LLIGHTGRAY);
// 
// 	bRev = g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE11);
// 	bRev |= g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE11_RESERVE);
// 	if (bRev == TRUE)
// 		cButtonST_11.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_GREEN);
// 	else
// 		cButtonST_11.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_LLIGHTGRAY);
// 
// 	bRev = g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE12);
// 	bRev |= g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE12_RESERVE);
// 	if (bRev == TRUE)
// 		cButtonST_12.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_GREEN);
// 	else
// 		cButtonST_12.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_LLIGHTGRAY);
// 
// 	bRev = g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE13);
// 	bRev |= g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE13_RESERVE);
// 	if (bRev == TRUE)
// 		cButtonST_13.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_GREEN);
// 	else
// 		cButtonST_13.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_LLIGHTGRAY);
// 
// 	bRev = g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE14);
// 	bRev |= g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE14_RESERVE);
// 	if (bRev == TRUE)
// 		cButtonST_14.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_GREEN);
// 	else
// 		cButtonST_14.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_LLIGHTGRAY);
// 
// 	bRev = g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE15);
// 	bRev |= g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE15_RESERVE);
// 	if (bRev == TRUE)
// 		cButtonST_15.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_GREEN);
// 	else
// 		cButtonST_15.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_LLIGHTGRAY);
// 
// 	bRev = g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE16);
// 	bRev |= g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE16_RESERVE);
// 	if (bRev == TRUE)
// 		cButtonST_16.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_GREEN);
// 	else
// 		cButtonST_16.SetShade(CShadeButtonST::SHS_SOFTBUMP, 5, 14, 100, COLOR_LLIGHTGRAY);

	cButtonST_1.Invalidate();
	cButtonST_2.Invalidate();
	cButtonST_3.Invalidate();
	cButtonST_4.Invalidate();
	cButtonST_5.Invalidate();
	cButtonST_6.Invalidate();
	cButtonST_7.Invalidate();
	cButtonST_8.Invalidate();
// 	cButtonST_9.Invalidate();
// 	cButtonST_10.Invalidate();
// 	cButtonST_11.Invalidate();
// 	cButtonST_12.Invalidate();
// 	cButtonST_13.Invalidate();
// 	cButtonST_14.Invalidate();
// 	cButtonST_15.Invalidate();
// 	cButtonST_16.Invalidate();
}

void CMainLower_TabYield::CompanySetting()
{
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	if (nTesterOption != eTester_Dual) // Single Tester [8/27/2021 donghyun.shin]
	{
// 		GetDlgItem(IDC_CLRCHK_CONNECT17)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT18)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT19)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT20)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT21)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT22)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT23)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT24)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT25)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT26)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT27)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT28)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT29)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT30)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT31)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT32)->ShowWindow(SW_HIDE);
	}

	if (strcmp(m_szCompany, DEF_COMPANY_AMKOR) == 0) {
		GetDlgItem(IDC_BTN_YE_CLEAN_1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_YE_CLEAN_2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_YE_CLEAN_3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_YE_CLEAN_4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_YE_CLEAN_5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_YE_CLEAN_6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_YE_CLEAN_7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_YE_CLEAN_8)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_BTN_YE_CLEAN_9)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_BTN_YE_CLEAN_10)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_BTN_YE_CLEAN_11)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_BTN_YE_CLEAN_12)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_BTN_YE_CLEAN_13)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_BTN_YE_CLEAN_14)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_BTN_YE_CLEAN_15)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_BTN_YE_CLEAN_16)->ShowWindow(SW_SHOW);
	}
}

int CMainLower_TabYield::getCurUserLv()
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CSLTView* pSltView = (CSLTView*)pMainFrm->GetActiveView();
	int nUserLevel = pSltView->m_nLogInLv;

	return nUserLevel;
}

BOOL CMainLower_TabYield::ChkModuleUseAble(int nStation)
{
	BOOL bIsUseAble = FALSE;

	if (ePressUnit_Down_1 <= nStation &&
		nStation <= ePressUnit_Down_4)// ||
// 		ePressUnit_Up_1 <= nStation     &&
// 		nStation <= ePressUnit_Up_4)
	{
		bIsUseAble = g_DMCont.m_dmEQP.GB(BDM0_MODULE1_SITE_ON_IS_POSSIBLE);
	}
	else
	{
		bIsUseAble = g_DMCont.m_dmEQP.GB(BDM0_MODULE2_SITE_ON_IS_POSSIBLE);
	}

	return bIsUseAble;
}

void CMainLower_TabYield::setAllStationOnOffFromDM()
{
// 	for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++)
// 	{
// 		for (int nY = 0; nY < STATION_MAX_Y_SITE; nY++)
// 		{
// 			for (int nX = 0; nX < STATION_MAX_X_SITE; nX++)
// 			{
// 				m_nSiteOnOff[nStation][nY][nX] = g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nX + nY*STATION_MAX_X_SITE);
// 			}
// 		}
// 	}
	//int nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
	int nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);

	for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++)
	{
		for (int nX = 0; nX < nTestSite_Div_Y; nX++)
		{
			for (int nY = 0; nY < nTestSite_Div_X; nY++)
			{
				int nNum = 0;

				if (g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y) == STATION_MAX_Y_SITE)
				{
					if (nBoardType == eBoardDir_Forward) {
						if (nStation <= ePressUnit_Down_4) {
							nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(nY - 3) + nX * 4;
						}
						else {
							nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nY + abs(nX - 1) * 4;
						}
					}
					else {
						if (nStation <= ePressUnit_Down_4) {
							nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nY + abs(nX - 1) * 4;
						}
						else {
							nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(nY - 3) + nX * 4;
						}
					}
				}
				else if (g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y) == 1)
				{
					if (nBoardType == eBoardDir_Forward) {
						if (nStation <= ePressUnit_Down_4) {
							nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(nY - 3) + nX * 4;
						}
						else {
							//nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nY + abs(nX - 1) * 4;
							nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nY + nX * 4;
						}
					}
					else {
						if (nStation <= ePressUnit_Down_4) {
							//nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nY + abs(nX - 3) * 4;
							nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nY + nX * 4;
						}
						else {
							nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nY + nX * 4;
						}
					}
				}
				m_nSiteOnOff[nStation][nY][nX] = g_DMCont.m_dmHandlerOpt.GN(nNum);
			}
		}
	}
}
void CMainLower_TabYield::switchOnOff(int nStation, int Col, int Row, BOOL bTypeAll /* = FALSE */)
{
	setAllStationOnOffFromDM();  //DM에서 현재 해당 Station의 전체Site의 On/Off상태를 불러옴.

	if (bTypeAll)
	{
		ChangeAllSocketStatus(nStation);
	}
	else
	{
		int nChk = m_nSiteOnOff[nStation][Row - 1][Col - 1]; //해당 Station의 선택된 Site의 On/Off상태를 가져옴.
		int nOnOff = nChk;

		if (g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY) != TRUE)  //Oncycle 상태 아님 
		{
			if (nChk != DEF_SITE_ON)
			{
				int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
				if (nRunMode == eRUN_MODE_ONLINE)
				{
					BOOL bConnect = TesterConnectChk(nStation, Col, Row);

					if (bConnect == FALSE)
					{
						AfxMessageBox("Can not site on. Tester is disconnection. Please check ");
						return;
					}
				}
			}
		}

		if (nChk == DEF_SITE_SOCKET_HOLD || nChk == DEF_SITE_SOCKET_LOCK)
		{
			return;
		}

		if (nChk != DEF_SITE_ON)
		{
			nOnOff = DEF_SITE_ON;
		}
		else
		{
			if (g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY) != TRUE)
			{

				if (m_nSiteOnOff[nStation][Row - 1][Col - 1] == DEF_SITE_OFF)
				{
					nOnOff = DEF_SITE_OFF;
				}
				else
				{
					nOnOff = DEF_SITE_OFF_RESERVATION;
				}
			}
			else
			{
				nOnOff = DEF_SITE_OFF;
			}
		}
		m_nSiteOnOff[nStation][Row - 1][Col - 1] = nOnOff;
	}

	/////////////////////////////////////////////// 인터락

	if (ChkSocketOnOffInterlock() == FALSE)
	{
		UpdateData(TRUE);

		char szCokName[512] = { 0, };
		ST_BASIC_INFO   stBasicData, stBeforeBasicData;
		g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
		g_COK_FileMgr.LoadBasicInfo(szCokName, stBasicData);

		stBeforeBasicData = stBasicData;
		//int nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
		int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
		if (bTypeAll)
		{
			for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++)
			{
				for (int nCol = 0; nCol < m_nTestSite_Div_Y; nCol++)
				{
					for (int nRow = 0; nRow < m_nTestSite_Div_X; nRow++)
					{
						int nNum = 0;
						if (g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y) == STATION_MAX_Y_SITE)
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
						else if (g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y) == 1)
						{
							if (nBoardType == eBoardDir_Forward) {
								if (nStation <= ePressUnit_Down_4)
									//nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(Row - 4) + (Col - 1) * 4;
									nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nRow + (Col - 4) * 4;
								//				g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(Row - 4) + (Col - 1)*4, m_nSiteOnOff[nStation][Row - 1][Col - 1]);
								else
									nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nRow + (Col - 4) * 4;
								//nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + (Row - 1) + abs(Col - 2) * 4;
								//	g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + (Row - 1) + abs(Col - 2) * 4, m_nSiteOnOff[nStation][Row - 1][Col - 1]);
							}
							else {
								if (nStation <= ePressUnit_Down_4)
									//nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + (Row - 1) + abs(Col - 2) * 4;
									nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nRow + (Col - 4) * 4;
								else
									nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nRow + (Col - 4) * 4;
							}

						}
						g_DMCont.m_dmHandlerOpt.SN(nNum, m_nSiteOnOff[nStation][nRow][nCol]);
						stBasicData.nSiteOnOff[nStation][nRow][nCol] = m_nSiteOnOff[nStation][nRow][nCol];
					}
				}
			}
		}
		else
		{
// 			g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + (Col - 1) + (Row - 1)*STATION_MAX_X_SITE, m_nSiteOnOff[nStation][Row - 1][Col - 1]);
// 			stBasicData.nSiteOnOff[nStation][Row - 1][Col - 1] = m_nSiteOnOff[nStation][Row - 1][Col - 1];

			int nNum = 0;
			//int nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
			if (g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y) == STATION_MAX_Y_SITE)
			{
				if (nBoardType == eBoardDir_Forward) {
					if (nStation <= ePressUnit_Down_4)
						nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(Row - 4) + (Col - 1) * 4;
					//				g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(Row - 4) + (Col - 1)*4, m_nSiteOnOff[nStation][Row - 1][Col - 1]);
					else
						nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + (Row - 1) + abs(Col - 2) * 4;
					//	g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + (Row - 1) + abs(Col - 2) * 4, m_nSiteOnOff[nStation][Row - 1][Col - 1]);
				}
				else {
					if (nStation <= ePressUnit_Down_4)
						nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + (Row - 1) + abs(Col - 2) * 4;
					else
						nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(Row - 4) + (Col - 1) * 4;
				}
			}
			else if (g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y) == 1)
			{
				if (nBoardType == eBoardDir_Forward) {
					if (nStation <= ePressUnit_Down_4)
						nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(Row - 4) + (Col - 1) * 4;
					//				g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(Row - 4) + (Col - 1)*4, m_nSiteOnOff[nStation][Row - 1][Col - 1]);
					else
						//nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + (Row - 1) + abs(Col - 2) * 4;
						nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(Row - 1) + (Col - 1) * 4;
					//	g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + (Row - 1) + abs(Col - 2) * 4, m_nSiteOnOff[nStation][Row - 1][Col - 1]);
				}
				else {
					if (nStation <= ePressUnit_Down_4)
						//nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + (Row - 1) + abs(Col - 2) * 4;
						nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(Row - 1) + (Col - 1) * 4;
					else
						nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(Row - 4) + (Col - 1) * 4;
				}
			}
			g_DMCont.m_dmHandlerOpt.SN(nNum, m_nSiteOnOff[nStation][Row - 1][Col - 1]);
			stBasicData.nSiteOnOff[nStation][(Row - 1)][(Col - 1)] = m_nSiteOnOff[nStation][(Row - 1)][(Col - 1)];
		}


		g_COK_FileMgr.SaveBasicInfo(szCokName, &stBasicData);

		std::vector<std::pair<CString, std::pair<CString, CString>>>vData;

		stBeforeBasicData.IsSameSiteSet(stBasicData, vData);

		if (!(vData.empty()))
		{
			SiteSetChangeLog(vData);
		}
	}
}

void CMainLower_TabYield::switchSiteHold(int nStation, int Col, int Row, BOOL bTypeAll /* = FALSE */)
{
	setAllStationOnOffFromDM();  //DM에서 현재 해당 Station의 전체Site의 On/Off상태를 불러옴.

	//int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);

	if (bTypeAll)
	{
		ChangeAllSocketHoldStatus(nStation);
	}
	else
	{
		int nChk = m_nSiteOnOff[nStation][Row - 1][Col - 1]; //해당 Station의 선택된 Site의 On/Off상태를 가져옴.
		int nOnOff = nChk;

		if (g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY) != TRUE)  //Oncycle 상태 아님 
		{
			if (nChk != DEF_SITE_ON)
			{
// 				int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
// 				if (nRunMode == eRUN_MODE_ONLINE)
// 				{
// 					BOOL bConnect = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + nStation);
// 					if (bConnect == FALSE)
// 					{
// 						AfxMessageBox("Can not site on. Tester is disconnection. Please check ");
// 						return;
// 					}
// 				}
			}
		}
		if (nChk == DEF_SITE_ON)
		{
			AfxMessageBox("Because it is in Site On state, it cannot be changed!");
			return;
		}

		if (nChk == DEF_SITE_OFF)
		{
			nOnOff = DEF_SITE_SOCKET_HOLD;
		}
		else
		{
			if (g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY) != TRUE)
			{

				if (m_nSiteOnOff[nStation][Row - 1][Col - 1] == DEF_SITE_OFF || m_nSiteOnOff[nStation][Row - 1][Col - 1] == DEF_SITE_SOCKET_HOLD)
				{
					nOnOff = DEF_SITE_OFF;
				}
				else
				{
					nOnOff = DEF_SITE_OFF_RESERVATION;
				}
			}
			else
			{
				nOnOff = DEF_SITE_OFF;
			}
		}
		m_nSiteOnOff[nStation][Row - 1][Col - 1] = nOnOff;
	}

	/////////////////////////////////////////////// 인터락

	if (ChkSocketOnOffInterlock() == FALSE)
	{
		UpdateData(TRUE);

		char szCokName[512] = { 0, };
		ST_BASIC_INFO   stBasicData, stBeforeBasicData;
		g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
		g_COK_FileMgr.LoadBasicInfo(szCokName, stBasicData);

		stBeforeBasicData = stBasicData;

		if (bTypeAll)
		{
			for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++)
			{
				for (int nCol = 0; nCol < m_nTestSite_Div_Y; nCol++)
				{
					for (int nRow = 0; nRow < m_nTestSite_Div_X; nRow++)
					{
						int nNum = 0;

						if (g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y) == STATION_MAX_Y_SITE)
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
						else if (g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y) == 1)
						{
							if (nBoardType == eBoardDir_Forward) {
								if (nStation <= ePressUnit_Down_4)
									//nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(Row - 4) + (Col - 1) * 4;
									nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nRow + (Col - 4) * 4;
								//				g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(Row - 4) + (Col - 1)*4, m_nSiteOnOff[nStation][Row - 1][Col - 1]);
								else
									nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nRow + (Col - 4) * 4;
								//nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + (Row - 1) + abs(Col - 2) * 4;
								//	g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + (Row - 1) + abs(Col - 2) * 4, m_nSiteOnOff[nStation][Row - 1][Col - 1]);
							}
							else {
								if (nStation <= ePressUnit_Down_4)
									//nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + (Row - 1) + abs(Col - 2) * 4;
									nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nRow + (Col - 4) * 4;
								else
									nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nRow + (Col - 4) * 4;
							}

						}

						int nSiteOnOff = g_DMCont.m_dmHandlerOpt.GN(nNum);
						if(nSiteOnOff == DEF_SITE_SOCKET_LOCK)
							continue;

						g_DMCont.m_dmHandlerOpt.SN(nNum, m_nSiteOnOff[nStation][nRow][nCol]);
						stBasicData.nSiteOnOff[nStation][nRow][nCol] = m_nSiteOnOff[nStation][nRow][nCol];
					}
				}
			}

		}
		else
		{
			int nNum = 0;
			if (g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y) == STATION_MAX_Y_SITE)
			{
				if (nBoardType == eBoardDir_Forward) {
					if (nStation <= ePressUnit_Down_4)
						nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(Row - 4) + (Col - 1) * 4;
					//				g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(Row - 4) + (Col - 1)*4, m_nSiteOnOff[nStation][Row - 1][Col - 1]);
					else
						nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + (Row - 1) + abs(Col - 2) * 4;
					//	g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + (Row - 1) + abs(Col - 2) * 4, m_nSiteOnOff[nStation][Row - 1][Col - 1]);
				}
				else {
					if (nStation <= ePressUnit_Down_4)
						nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + (Row - 1) + abs(Col - 2) * 4;
					else
						nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(Row - 4) + (Col - 1) * 4;
				}
			}
			else if (g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y) == 1)
			{
				if (nBoardType == eBoardDir_Forward) {
					if (nStation <= ePressUnit_Down_4)
						nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(Row - 4) + (Col - 1) * 4;
					//				g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(Row - 4) + (Col - 1)*4, m_nSiteOnOff[nStation][Row - 1][Col - 1]);
					else
						//nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + (Row - 1) + abs(Col - 2) * 4;
						nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(Row - 1) + (Col - 1) * 4;
					//	g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + (Row - 1) + abs(Col - 2) * 4, m_nSiteOnOff[nStation][Row - 1][Col - 1]);
				}
				else {
					if (nStation <= ePressUnit_Down_4)
						//nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + (Row - 1) + abs(Col - 2) * 4;
						nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(Row - 1) + (Col - 1) * 4;
					else
						nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(Row - 4) + (Col - 1) * 4;
				}
			}
			g_DMCont.m_dmHandlerOpt.SN(nNum, m_nSiteOnOff[nStation][Row - 1][Col - 1]);
			stBasicData.nSiteOnOff[nStation][Row - 1][Col - 1] = m_nSiteOnOff[nStation][Row - 1][Col - 1];
		}


		g_COK_FileMgr.SaveBasicInfo(szCokName, &stBasicData);

		std::vector<std::pair<CString, std::pair<CString, CString>>>vData;

		stBeforeBasicData.IsSameSiteSet(stBasicData, vData);

		if (!(vData.empty()))
		{
			SiteSetChangeLog(vData);
		}
	}
}

void CMainLower_TabYield::CheckRunningAndSwitchOnOff(int nStation, long Col, long Row, BOOL bTypeAll, BOOL bHoldType/* = FALSE */)
{
	int nUserLv = getCurUserLv();
	int nTestSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	if (nUserLv == eUSERLEVEL_OPERATOR)
	{
		AfxMessageBox(_T("You Can't Switch because your User Level is Operator"));
		return;
	}

	// 	int n4ParaCheck = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_4PARA_OPTION_USE);
	// 		//nTestSiteY = 1;
	// 
	// 	if (n4ParaCheck == TRUE && nTestSiteY != Row)
	// 	{
	// 		AfxMessageBox(_T("You Can't Switch because 4Para Option On"));
	// 		return;
	// 	}

	int nMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	if (nMode == eStartMD_AutoLoop_Continue)
	{
		AfxMessageBox(_T("You Can't Switch because Loop Mode"));
		return;
	}

	// 해당Station Module이 사용할 수 없다면 Return;
	if (ChkModuleUseAble(nStation) != TRUE && g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY) != TRUE) {
		AfxMessageBox("This Module unit motor is not use able. Please check it.");
		return;
	}

	if (bHoldType != TRUE)
		switchOnOff(nStation, Col, Row, bTypeAll);
	else
		switchSiteHold(nStation, Col, Row, bTypeAll);

}

void CMainLower_TabYield::DblClickSpreadStation1(long Col, long Row)
{
	int nStation = ePressUnit_Down_1;

	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}

void CMainLower_TabYield::DblClickSpreadStation2(long Col, long Row)
{
	int nStation = ePressUnit_Down_2;

	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}


void CMainLower_TabYield::DblClickSpreadStation3(long Col, long Row)
{
	int nStation = ePressUnit_Down_3;

	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}


void CMainLower_TabYield::DblClickSpreadStation4(long Col, long Row)
{
	int nStation = ePressUnit_Down_4;

	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}


void CMainLower_TabYield::DblClickSpreadStation5(long Col, long Row)
{
	int nStation = ePressUnit_Down_5;

	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}


void CMainLower_TabYield::DblClickSpreadStation6(long Col, long Row)
{
	int nStation = ePressUnit_Down_6;

	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}


void CMainLower_TabYield::DblClickSpreadStation7(long Col, long Row)
{
	int nStation = ePressUnit_Down_7;

	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}


void CMainLower_TabYield::DblClickSpreadStation8(long Col, long Row)
{
	int nStation = ePressUnit_Down_8;

	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}


void CMainLower_TabYield::DblClickSpreadStation9(long Col, long Row)
{
// 	int nStation = ePressUnit_Up_1;
// 
// 	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}


void CMainLower_TabYield::DblClickSpreadStation10(long Col, long Row)
{
// 	int nStation = ePressUnit_Up_2;
// 
// 	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}


void CMainLower_TabYield::DblClickSpreadStation11(long Col, long Row)
{
// 	int nStation = ePressUnit_Up_3;
// 
// 	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}


void CMainLower_TabYield::DblClickSpreadStation12(long Col, long Row)
{
// 	int nStation = ePressUnit_Up_4;
// 
// 	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}


void CMainLower_TabYield::DblClickSpreadStation13(long Col, long Row)
{
// 	int nStation = ePressUnit_Up_5;
// 
// 	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}


void CMainLower_TabYield::DblClickSpreadStation14(long Col, long Row)
{
// 	int nStation = ePressUnit_Up_6;
// 
// 	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}


void CMainLower_TabYield::DblClickSpreadStation15(long Col, long Row)
{
// 	int nStation = ePressUnit_Up_7;
// 
// 	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}


void CMainLower_TabYield::DblClickSpreadStation16(long Col, long Row)
{
// 	int nStation = ePressUnit_Up_8;
// 
// 	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}

void CMainLower_TabYield::RightClickSpreadStation1(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
	int nRet = ShowLoginBox();
	if (nRet != TRUE)
		return;

	int nStation = ePressUnit_Down_1;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, FALSE, TRUE);
}


void CMainLower_TabYield::RightClickSpreadStation2(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
	int nRet = ShowLoginBox();
	if (nRet != TRUE)
		return;

	int nStation = ePressUnit_Down_2;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, FALSE, TRUE);
}


void CMainLower_TabYield::RightClickSpreadStation3(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
	int nRet = ShowLoginBox();
	if (nRet != TRUE)
		return;

	int nStation = ePressUnit_Down_3;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, FALSE, TRUE);
}


void CMainLower_TabYield::RightClickSpreadStation4(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
	int nRet = ShowLoginBox();
	if (nRet != TRUE)
		return;

	int nStation = ePressUnit_Down_4;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, FALSE, TRUE);
}


void CMainLower_TabYield::RightClickSpreadStation5(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
	int nRet = ShowLoginBox();
	if (nRet != TRUE)
		return;

	int nStation = ePressUnit_Down_5;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, FALSE, TRUE);
}


void CMainLower_TabYield::RightClickSpreadStation6(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
	int nRet = ShowLoginBox();
	if (nRet != TRUE)
		return;

	int nStation = ePressUnit_Down_6;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, FALSE, TRUE);
}

void CMainLower_TabYield::RightClickSpreadStation7(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
	int nRet = ShowLoginBox();
	if (nRet != TRUE)
		return;

	int nStation = ePressUnit_Down_7;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, FALSE, TRUE);
}


void CMainLower_TabYield::RightClickSpreadStation8(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
	int nRet = ShowLoginBox();
	if (nRet != TRUE)
		return;

	int nStation = ePressUnit_Down_8;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, FALSE, TRUE);
}


void CMainLower_TabYield::RightClickSpreadStation9(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
// 	int nRet = ShowLoginBox();
// 	if (nRet != TRUE)
// 		return;
// 
// 	int nStation = ePressUnit_Up_1;
// 
// 	CheckRunningAndSwitchOnOff(nStation, Col, Row, FALSE, TRUE);
}


void CMainLower_TabYield::RightClickSpreadStation10(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
// 	int nRet = ShowLoginBox();
// 	if (nRet != TRUE)
// 		return;
// 
// 	int nStation = ePressUnit_Up_2;
// 
// 	CheckRunningAndSwitchOnOff(nStation, Col, Row, FALSE, TRUE);
}


void CMainLower_TabYield::RightClickSpreadStation11(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
// 	int nRet = ShowLoginBox();
// 	if (nRet != TRUE)
// 		return;
// 
// 	int nStation = ePressUnit_Up_3;
// 
// 	CheckRunningAndSwitchOnOff(nStation, Col, Row, FALSE, TRUE);
}


void CMainLower_TabYield::RightClickSpreadStation12(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
// 	int nRet = ShowLoginBox();
// 	if (nRet != TRUE)
// 		return;
// 
// 	int nStation = ePressUnit_Up_4;
// 
// 	CheckRunningAndSwitchOnOff(nStation, Col, Row, FALSE, TRUE);
}


void CMainLower_TabYield::RightClickSpreadStation13(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
// 	int nRet = ShowLoginBox();
// 	if (nRet != TRUE)
// 		return;
// 
// 	int nStation = ePressUnit_Up_5;
// 
// 	CheckRunningAndSwitchOnOff(nStation, Col, Row, FALSE, TRUE);
}


void CMainLower_TabYield::RightClickSpreadStation14(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
// 	int nRet = ShowLoginBox();
// 	if (nRet != TRUE)
// 		return;
// 
// 	int nStation = ePressUnit_Up_6;
// 
// 	CheckRunningAndSwitchOnOff(nStation, Col, Row, FALSE, TRUE);
}


void CMainLower_TabYield::RightClickSpreadStation15(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
// 	int nRet = ShowLoginBox();
// 	if (nRet != TRUE)
// 		return;
// 
// 	int nStation = ePressUnit_Up_7;
// 
// 	CheckRunningAndSwitchOnOff(nStation, Col, Row, FALSE, TRUE);
}


void CMainLower_TabYield::RightClickSpreadStation16(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
// 	int nRet = ShowLoginBox();
// 	if (nRet != TRUE)
// 		return;
// 
// 	int nStation = ePressUnit_Up_8;
// 
// 	CheckRunningAndSwitchOnOff(nStation, Col, Row, FALSE, TRUE);
}

BOOL CMainLower_TabYield::ShowLoginBox()
{
	int nRetLevel = 0;

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	BOOL bRet = g_LibCommCtrl.ShowLogInBox(0, &nRetLevel);

	if (RETURN_OK == bRet && nRetLevel >= eUSERLEVEL_TECHNICIAN) {
	}
	else {
		AfxMessageBox("User Level is low!!");
		return FALSE;
	}
	return TRUE;
}

void CMainLower_TabYield::ChangeAllSocketStatus(int nStation) //하나라도 꺼져있거나 다 꺼져있으면 On , 다 켜져있으면 Off
{
	int nOnOff = DEF_SITE_OFF;
	//체크

	for (int nRow = 0; nRow < m_nRow; nRow++)
	{
		for (int nCol = 0; nCol < m_nCol; nCol++)
		{
			if (g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY) != TRUE)
			{

				if (m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_OFF ||
					m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_OFF_RESERVATION ||
					m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_HOLD)
				{
					nOnOff = DEF_SITE_ON;
					break;
				}
				else
				{
					nOnOff = DEF_SITE_OFF_RESERVATION;
					break;
				}
			}
			else //History True
			{
				if (m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_OFF ||
					m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_OFF_RESERVATION ||
					m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_HOLD)
				{
					nOnOff = DEF_SITE_ON;
					break;
				}
				else
				{
					nOnOff = DEF_SITE_OFF;
					break;
				}
			}

		}
	}

	for (int nCol = 0; nCol < 2; nCol++)
	{
		for (int nRow = 0; nRow < 4; nRow++)
		{
			if (m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_HOLD || m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_LOCK)
			{

			}
			else
				m_nSiteOnOff[nStation][nRow][nCol] = nOnOff;
		}
	}
}

int CMainLower_TabYield::TesterConnectChk(int nStation, int nX, int nY) //하나라도 꺼져있거나 다 꺼져있으면 On , 다 켜져있으면 Off
{
	BOOL bConnect = FALSE;
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);

	if (nTesterOption == eTester_Single)
		bConnect = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + nStation);
	else if (nTesterOption == eTester_EachSocket)
	{
		int nSocketArr1[STATION_MAX_X_SITE][STATION_MAX_Y_SITE] = { { 5,1 },{ 6,2 },{ 7,3 },{ 8,4 } };
		int nSocketArr2[STATION_MAX_X_SITE][STATION_MAX_Y_SITE] = { { 1,5 },{ 2,6 },{ 3,7 },{ 4,8 } };
		int nSocketIdx = nStation < eMaxPressUnitCount / 2 ? nSocketArr1[nY - 1][nX - 1] : nSocketArr2[nY - 1][nX - 1];
		int nTesterConnectCnt = 0; int nStationHalf = 0;

		if (nSocketIdx > STATION_MAX_X_SITE)
			nStationHalf = STATION_MAX_X_SITE;

		for (int nTesterCnt = 0; nTesterCnt < EACH_LINE_STATION_CNT; nTesterCnt++)
		{
			int nTesterIdx = (((nStation)* EACH_LINE_STATION_CNT) + nTesterCnt + nStationHalf);
			BOOL bTesterConnect = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + nTesterIdx);
			if (bTesterConnect == DEF_ON)
			{
				nTesterConnectCnt++;
				if (nTesterConnectCnt == EACH_LINE_STATION_CNT)
				{
					bConnect = TRUE;
				}
			}
		}
	}
	else if (nTesterOption == eTester_Dual)
	{

		if (nY == eRearTester + 1)
			bConnect = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + ((nStation * 2) - 1) + 1);
		else
			bConnect = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + (nStation * 2) + 1);
	}
	else
		bConnect = g_DMCont.m_dmEQP.GB(BDM0_AGENT_CONNECTION_1 + nStation);

	return bConnect;
}

BOOL CMainLower_TabYield::ChkSocketOnOffInterlock()
{
	CString strErrMsg;
	CString strErrStation = _T("");
	std::vector<int> vErrEachOneSock;

	std::vector<int> vSocket;

	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);



	if (ChkAllStationOFF()) //모든 Site가 꺼져있다면 
	{
		strErrMsg.Format(_T("It can not be saved when the whole Socket is turned off."));
		AfxMessageBox(strErrMsg, MB_ICONHAND | MB_TOPMOST);
		return TRUE;
	}

	int nHalf_Site = eMaxATS_Area / 2;

	int nSite = eMaxATS_Area;

	int nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);

	for (int nTotal_cnt = 0; nTotal_cnt < eMaxTestPPCount; nTotal_cnt++)
	{
		vSocket.clear();

		strErrStation.Format(_T("%d, %d, %d, %d"),
			(nTotal_cnt * nSite) + 1,
			(nTotal_cnt * nSite) + 2,
			(nTotal_cnt * nSite) + 3,
			(nTotal_cnt * nSite) + 4);

		for (int nStation = 0; nStation < nSite; nStation++) //nSite == 4
		{
			for (int nCol = 0; nCol < nTestSite_Div_X; nCol++)
			{
				for (int nRow = 0; nRow < nTestSite_Div_Y; nRow++)
				{
					if (nTotal_cnt == eTestPP_1)
					{
						if (g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y) == 1)
							vSocket.push_back(m_nSiteOnOff[nStation + (nTotal_cnt * 4)][abs(nRow - 3)][nCol + 1]);
						else
							vSocket.push_back(m_nSiteOnOff[nStation + (nTotal_cnt * 4)][abs(nRow - 3)][nCol]);
					}
					else
						vSocket.push_back(m_nSiteOnOff[nStation + (nTotal_cnt * 4)][nRow][abs(nCol - 1)]);
				}
			}
		}

		if (Chk4StationSocketOff(vSocket) == FALSE) //4개 Station이 모두 OFF가 아니 라면 FALSE 검사 시작.
		{
			if (ChkEachOneMoreSocketOn(vSocket) == TRUE) //8개  Sokcet이 1개이상 씩 켜져 있다면 TRUE 아니라면 FALSE
			{
				if (ChkRetestTwoMoreStaionOn(vSocket) == FALSE) {
					// 					if (nProjectOption == DEF_PROJECT_MAUI) {
					//  						strErrMsg.Format(_T("Please turn on more than 2 stations in retest mode.\r\nError Station : %s"), strErrStation);
					//  						AfxMessageBox(strErrMsg, MB_ICONHAND | MB_TOPMOST);
					//  						return TRUE;
					// 					}
					strErrMsg.Format(_T("Please turn on more than 2 stations in retest mode.\r\nError Station : %s"), strErrStation);
					AfxMessageBox(strErrMsg, MB_ICONHAND | MB_TOPMOST);
					return TRUE;
				}
			}
			else
			{
				ChkErrEachOneMoreSocketOn(vSocket, vErrEachOneSock);
				//				if (nProjectOption == DEF_PROJECT_MAUI) {
				strErrMsg.Format(_T("Please turn on at least one socket with 4 stations.\r\nError Station : %s\r\nError Socket : "), strErrStation);
				int b = vErrEachOneSock.size();
				for (int k = 0; k < (int)vErrEachOneSock.size(); k++) {
					CString temp;

					if (k != vErrEachOneSock.size() - 1)
						temp.Format(_T("%d, "), vErrEachOneSock[k]);
					else
						temp.Format(_T("%d"), vErrEachOneSock[k]);

					strErrMsg += temp;
					//				}
					AfxMessageBox(strErrMsg, MB_ICONHAND | MB_TOPMOST);
					return TRUE;
				}

			}
		}
	}

	return FALSE;

}

void CMainLower_TabYield::SiteSetChangeLog(std::vector<std::pair<CString, std::pair<CString, CString>>> vData)
{
	for (int i = 0; i < (int)vData.size(); i++) {
		std::pair<CString, std::pair<CString, CString>> p;

		p = vData[i];

		MakeLog("[MENU = Site Setting, DATA NAME = %s ][BEFORE=%s, AFTER=%s]", p.first, p.second.first, p.second.second);
	}
}

void CMainLower_TabYield::ChangeAllSocketHoldStatus(int nStation) //하나라도 꺼져있거나 다 꺼져있으면 On , 다 켜져있으면 Off
{
	int nOnOff = DEF_SITE_OFF;
	//체크

	for (int nCol = 0; nCol < 2; nCol++)
	{
		for (int nRow = 0; nRow < 4; nRow++)
		{
			if (g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY) != TRUE)
			{
				if (m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_OFF)
				{
					nOnOff = DEF_SITE_SOCKET_HOLD;
//					break;
				}
				else if (m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_HOLD)
				{
					nOnOff = DEF_SITE_OFF;
					break;
				}
				else if (m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_LOCK)
				{
					nOnOff = DEF_SITE_SOCKET_LOCK;
					break;
				}
			}
			else //History True
			{
				if (m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_OFF)
				{
					nOnOff = DEF_SITE_SOCKET_HOLD;
					break;
				}
				else if (m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_HOLD)
				{
					nOnOff = DEF_SITE_OFF;
					break;
				}
				else if (m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_LOCK)
				{
					nOnOff = DEF_SITE_SOCKET_LOCK;
					break;
				}
			}
		}

// 		if (nOnOff == DEF_SITE_OFF) { //socket 1개라도 Hold 상태 였으면 Off로 바꾸기 위함.
// 			break;
// 		}

	}

	int nChk = 0;

	for (int nCol = 0; nCol < 2; nCol++)
	{
		for (int nRow = 0; nRow < 4; nRow++)
		{
			if (m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_ON) { nChk++; }

			if (m_nSiteOnOff[nStation][nRow][nCol] != DEF_SITE_ON && m_nSiteOnOff[nStation][nRow][nCol] != DEF_SITE_OFF_RESERVATION)
			{
				if (m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_LOCK)
					m_nSiteOnOff[nStation][nRow][nCol] = DEF_SITE_SOCKET_LOCK;
				else
					m_nSiteOnOff[nStation][nRow][nCol] = nOnOff;
			}

		}
	}

	if (nChk == STATION_MAX_PARA)
	{
		AfxMessageBox("Because it is in Site On state, it cannot be changed!");
		return;
	}
}
BOOL CMainLower_TabYield::Chk4StationSocketOff(std::vector<int> vSocket)
{
	int nSum = 0;

	for (int i = 0; i < (int)vSocket.size(); i++) {
		if (vSocket[i] == DEF_SITE_ON) { nSum++; }
	}
// 	for (int i = 0; i < (int)vDown.size(); i++) {
// 		if (vDown[i] == DEF_SITE_ON) { nSum++; }
// 		if (vUp[i] == DEF_SITE_ON) { nSum++; }
// 	}

	return nSum == 0 ? TRUE : FALSE;
}

BOOL CMainLower_TabYield::ChkEachOneMoreSocketOn(std::vector<int> vSocket)
{
	int szSock[STATION_MAX_PARA] = { 0, };

	int nSocketOnCount = 0;
	//int nStation_Max_para = STATION_MAX_X_SITE * STATION_MAX_Y_SITE;
	int nStation_Max_para = STATION_MAX_PARA;

	for (int i = 0; i < (int)vSocket.size(); i++) {
		switch (i % nStation_Max_para)
		{
		case 0:	ChkEachOneMoreSocketState(vSocket[i], szSock[0]);	break;
		case 1:	ChkEachOneMoreSocketState(vSocket[i], szSock[1]);	break;
		case 2:	ChkEachOneMoreSocketState(vSocket[i], szSock[2]);	break;
		case 3:	ChkEachOneMoreSocketState(vSocket[i], szSock[3]);	break;
		case 4:	ChkEachOneMoreSocketState(vSocket[i], szSock[4]);	break;
		case 5:	ChkEachOneMoreSocketState(vSocket[i], szSock[5]);	break;
		case 6:	ChkEachOneMoreSocketState(vSocket[i], szSock[6]);	break;
		case 7:	ChkEachOneMoreSocketState(vSocket[i], szSock[7]);	break;
		}
	}

	for (int nCnt = 0; nCnt < nStation_Max_para; nCnt++) {
		if (szSock[nCnt] > 0)
			nSocketOnCount++;
	}

	if (nSocketOnCount == nStation_Max_para)
		return TRUE;
	else
		return FALSE;
}


BOOL CMainLower_TabYield::ChkRetestTwoMoreStaionOn(std::vector<int> vSocket)
{
	BOOL bAutoRetest_TestPP = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);

	if (bAutoRetest_TestPP == FALSE)
		return TRUE;

	int nStation[eMaxATS_Area] = { 0, };
	int nSum = 0;

	for (int i = 0; i < eMaxATS_Area; i++) {
		ChkRetestTwoMoreStaionState(i, vSocket, nStation[i]);
	}

	for (int nCnt = 0; nCnt < eMaxATS_Area; nCnt++) {
		nSum += nStation[nCnt];
	}

	if (2 <= nSum)
		return TRUE;
	else
		return FALSE;
}

void CMainLower_TabYield::ChkEachOneMoreSocketState(int _station, int& _Socket_number)
{
	if (_station == DEF_SITE_ON) { _Socket_number++; }
}

void CMainLower_TabYield::ChkErrEachOneMoreSocketOn(std::vector<int> vChkSocket, std::vector<int> &vSocket)
{
	int nSock[STATION_MAX_PARA] = { 0, };

	//int nStation_Max_Para = STATION_MAX_X_SITE * STATION_MAX_Y_SITE;
	int nStation_Max_Para = m_nTestSite_Div_X * m_nTestSite_Div_Y;

	for (int i = 0; i < nStation_Max_Para; i++)
	{
		switch (i % nStation_Max_Para)
		{
		case 0:ChkErrEachOneMoreSocketState(i, vChkSocket, nSock[0]); break;
		case 1:ChkErrEachOneMoreSocketState(i, vChkSocket, nSock[1]); break;
		case 2:ChkErrEachOneMoreSocketState(i, vChkSocket, nSock[2]); break;
		case 3:ChkErrEachOneMoreSocketState(i, vChkSocket, nSock[3]); break;
		case 4:ChkErrEachOneMoreSocketState(i, vChkSocket, nSock[4]); break;
		case 5:ChkErrEachOneMoreSocketState(i, vChkSocket, nSock[5]); break;
		case 6:ChkErrEachOneMoreSocketState(i, vChkSocket, nSock[6]); break;
		case 7:ChkErrEachOneMoreSocketState(i, vChkSocket, nSock[7]); break;
		}
	}

	for (int nCnt = 0; nCnt < nStation_Max_Para; nCnt++) {
		if (nSock[nCnt] < 1)
			vSocket.push_back(nCnt + 1);
	}
}

void CMainLower_TabYield::ChkRetestTwoMoreStaionState(int _index, std::vector<int> _vSocket, int& _nStation)
{
	int nSocket = 0;
	int TestSite_Div = STATION_MAX_X_SITE*STATION_MAX_Y_SITE;
	for (int i = 0; i < TestSite_Div; i++)
	{
		if (_vSocket[(_index * TestSite_Div) + i] == DEF_SITE_ON) { nSocket++; }
	}

	if (nSocket > 0)
		_nStation = TRUE;
}

void CMainLower_TabYield::ChkErrEachOneMoreSocketState(int _index, std::vector<int> _vSocket, int &_Socket)
{
	int nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	if (_vSocket[_index] == DEF_SITE_ON) { _Socket++; }
	if (_vSocket[_index + (STATION_MAX_X_SITE*nTestSite_Div_Y)] == DEF_SITE_ON) { _Socket++; }
	if (_vSocket[_index + ((STATION_MAX_X_SITE*nTestSite_Div_Y) * 2)] == DEF_SITE_ON) { _Socket++; }
	if (_vSocket[_index + ((STATION_MAX_X_SITE*nTestSite_Div_Y) * 3)] == DEF_SITE_ON) { _Socket++; }
}

void CMainLower_TabYield::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = { 0, };
	if (fmt) {
		va_list argList;
		va_start(argList, fmt);
		_vstprintf(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_System_GUI), Debug, "", __LINE__, NULL, tmpMsg);
}

BOOL CMainLower_TabYield::ChkAllStationOFF()
{
	for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++)
	{
		for (int nRow = 0; nRow < m_nTestSite_Div_Y; nRow++)
		{
			for (int nCol = 0; nCol < m_nTestSite_Div_X; nCol++)
			{
				if (m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_ON)
					return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CMainLower_TabYield::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_RBUTTONDOWN)
	{
		for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++)
		{
			if (pMsg->hwnd == GetDlgItem(IDC_STATIC_YD_TEXT_STATION1 + nStation)->GetSafeHwnd())
			{
				PostMessage(UM_SITE_YIELD_HOLD, nStation);
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


LRESULT CMainLower_TabYield::OnSiteHold(WPARAM wParam, LPARAM lParam)
{
	int nRet = ShowLoginBox();
	if (nRet != TRUE)
		return 0L;

	int nStation = (int)wParam;
	int Col = m_nCol;
	int Row = m_nRow;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, TRUE, TRUE);
	return 0L;
}

