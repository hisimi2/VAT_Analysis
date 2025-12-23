// CMainLower_TabPassFail.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "MainLower_TabPassFail.h"
#include "afxdialogex.h"
#include "SLTView.h"
#include "MainFrm.h"


// CMainLower_TabPassFail 대화 상자입니다.

IMPLEMENT_DYNAMIC( CMainLower_TabPassFail, CDialogEx )

CMainLower_TabPassFail::CMainLower_TabPassFail( CWnd* pParent /*=NULL*/ )
	: CDialogEx( CMainLower_TabPassFail::IDD, pParent )
{
	memset( m_dTestTime, 0x00, sizeof( m_dTestTime ) );
	memset( m_nTestTmCtrlID, 0x00, sizeof( m_nTestTmCtrlID ) );
	memset( m_nIndexID, 0x00, sizeof( m_nIndexID ) );
	memset( m_nSocketOnOff, 0x00, sizeof( m_nSocketOnOff ) );
	memset(m_nSiteOnOff, 0x00, sizeof(m_nSiteOnOff));
	
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

	m_nSpreadTestSiteID[0][eFront] = IDC_SPREAD_SITE1_FRONT;
	m_nSpreadTestSiteID[0][eRear] = IDC_SPREAD_SITE1_REAR;
	m_nSpreadTestSiteID[1][eFront] = IDC_SPREAD_SITE2_FRONT;
	m_nSpreadTestSiteID[1][eRear] = IDC_SPREAD_SITE2_REAR;
	m_nSpreadTestSiteID[2][eFront] = IDC_SPREAD_SITE3_FRONT;
	m_nSpreadTestSiteID[2][eRear] = IDC_SPREAD_SITE3_REAR;
	m_nSpreadTestSiteID[3][eFront] = IDC_SPREAD_SITE4_FRONT;
	m_nSpreadTestSiteID[3][eRear] = IDC_SPREAD_SITE4_REAR;
	m_nSpreadTestSiteID[4][eFront] = IDC_SPREAD_SITE5_FRONT;
	m_nSpreadTestSiteID[4][eRear] = IDC_SPREAD_SITE5_REAR;
	m_nSpreadTestSiteID[5][eFront] = IDC_SPREAD_SITE6_FRONT;
	m_nSpreadTestSiteID[5][eRear] = IDC_SPREAD_SITE6_REAR;
	m_nSpreadTestSiteID[6][eFront] = IDC_SPREAD_SITE7_FRONT;
	m_nSpreadTestSiteID[6][eRear] = IDC_SPREAD_SITE7_REAR;
	m_nSpreadTestSiteID[7][eFront] = IDC_SPREAD_SITE8_FRONT;
  	m_nSpreadTestSiteID[7][eRear] = IDC_SPREAD_SITE8_REAR;


	m_nStationOnOff[0] = IDC_SPREAD_PASSFAIL_STATION1;
	m_nStationOnOff[1] = IDC_SPREAD_PASSFAIL_STATION2;
	m_nStationOnOff[2] = IDC_SPREAD_PASSFAIL_STATION3;
	m_nStationOnOff[3] = IDC_SPREAD_PASSFAIL_STATION4;
	m_nStationOnOff[4] = IDC_SPREAD_PASSFAIL_STATION5;
	m_nStationOnOff[5] = IDC_SPREAD_PASSFAIL_STATION6;
	m_nStationOnOff[6] = IDC_SPREAD_PASSFAIL_STATION7;
	m_nStationOnOff[7] = IDC_SPREAD_PASSFAIL_STATION8;
	
	m_nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	m_nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	memset(m_szCompany, 0x00, sizeof(m_szCompany));
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, m_szCompany, sizeof(m_szCompany));

	m_nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
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
	else{
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

// 	m_nRow = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);// STATION_MAX_Y_SITE;
// 	m_nCol = STATION_MAX_X_SITE;

 	m_nRow = STATION_MAX_X_SITE;
 	m_nCol = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);// STATION_MAX_Y_SITE;

	m_nBefore_TestSite_Div_X = STATION_MAX_X_SITE;
	m_nBefore_TestSite_Div_Y = STATION_MAX_Y_SITE;

	

	if (nTesterOption == eTester_EachSocket)
		if (m_nTestSite_Div_Y == 1)
			m_nTesterCount = MAX_TESTER_SINGLE;
		else
			m_nTesterCount = MAX_TESTER_DUAL;
	else
		m_nTesterCount = MAX_TESTER_SINGLE;

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

CMainLower_TabPassFail::~CMainLower_TabPassFail()
{
}

void CMainLower_TabPassFail::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange( pDX );
	for( int i = 0; i < eMaxPressUnitCount; i++ )
	{
		DDX_Control( pDX, m_nTestTmCtrlID[ i ], m_nTestTm[ i ] );
		
	}
	for (int i = 0; i < m_nTesterCount; i++)
	{
		DDX_Control(pDX, m_nConntct[i], m_CtrlConnect[i]);
	}
	DDX_Control(pDX, IDC_BTN_CLEAN_1, cButtonST_1);
	DDX_Control(pDX, IDC_BTN_CLEAN_2, cButtonST_2);
	DDX_Control(pDX, IDC_BTN_CLEAN_3, cButtonST_3);
	DDX_Control(pDX, IDC_BTN_CLEAN_4, cButtonST_4);
	DDX_Control(pDX, IDC_BTN_CLEAN_5, cButtonST_5);
	DDX_Control(pDX, IDC_BTN_CLEAN_6, cButtonST_6);
	DDX_Control(pDX, IDC_BTN_CLEAN_7, cButtonST_7);
	DDX_Control(pDX, IDC_BTN_CLEAN_8, cButtonST_8);
}


BEGIN_MESSAGE_MAP( CMainLower_TabPassFail, CDialogEx )
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_CLEAN_1, &CMainLower_TabPassFail::OnBnClickedBtnClean1)
	ON_BN_CLICKED(IDC_BTN_CLEAN_2, &CMainLower_TabPassFail::OnBnClickedBtnClean2)
	ON_BN_CLICKED(IDC_BTN_CLEAN_3, &CMainLower_TabPassFail::OnBnClickedBtnClean3)
	ON_BN_CLICKED(IDC_BTN_CLEAN_4, &CMainLower_TabPassFail::OnBnClickedBtnClean4)
	ON_BN_CLICKED(IDC_BTN_CLEAN_5, &CMainLower_TabPassFail::OnBnClickedBtnClean5)
	ON_BN_CLICKED(IDC_BTN_CLEAN_6, &CMainLower_TabPassFail::OnBnClickedBtnClean6)
	ON_BN_CLICKED(IDC_BTN_CLEAN_7, &CMainLower_TabPassFail::OnBnClickedBtnClean7)
	ON_BN_CLICKED(IDC_BTN_CLEAN_8, &CMainLower_TabPassFail::OnBnClickedBtnClean8)
	
	ON_STN_DBLCLK(IDC_STATIC_TEXT_STATION1, &CMainLower_TabPassFail::OnStnDblclickStation1)
	ON_STN_DBLCLK(IDC_STATIC_TEXT_STATION2, &CMainLower_TabPassFail::OnStnDblclickStation2)
	ON_STN_DBLCLK(IDC_STATIC_TEXT_STATION3, &CMainLower_TabPassFail::OnStnDblclickStation3)
	ON_STN_DBLCLK(IDC_STATIC_TEXT_STATION4, &CMainLower_TabPassFail::OnStnDblclickStation4)
	ON_STN_DBLCLK(IDC_STATIC_TEXT_STATION5, &CMainLower_TabPassFail::OnStnDblclickStation5)
	ON_STN_DBLCLK(IDC_STATIC_TEXT_STATION6, &CMainLower_TabPassFail::OnStnDblclickStation6)
	ON_STN_DBLCLK(IDC_STATIC_TEXT_STATION7, &CMainLower_TabPassFail::OnStnDblclickStation7)
	ON_STN_DBLCLK(IDC_STATIC_TEXT_STATION8, &CMainLower_TabPassFail::OnStnDblclickStation8)

	ON_MESSAGE(UM_SITE_HOLD, OnSiteHold)
//	ON_WM_RBUTTONDBLCLK()
END_MESSAGE_MAP()


// CMainLower_TabPassFail 메시지 처리기입니다.


BOOL CMainLower_TabPassFail::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	position_setup();
	StationParaSetting();

	for( int i = 0; i < eMaxPressUnitCount; i++ )
	{
		m_nTestTm[ i ].m_fnSetFont( "Fixedsys" );
		m_nTestTm[ i ].SetTextSize( 13 );
		m_nTestTm[ i ].SetBackColor( COLOR_NOMAL );
	}

	SetTimer(0, 1000, NULL);

	CompanySetting();
	/*if (strcmp(m_szCompany, DEF_COMPANY_AMKOR) == 0) {*/
		RefreshCleaning();
	//}
	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
	if (g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y) == 1 && nBoardType == eBoardDir_Backward)
	{
		GetDlgItem(IDC_SPREAD_SITE1_FRONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_SITE2_FRONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_SITE3_FRONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_SITE4_FRONT)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_SPREAD_SITE5_FRONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_SITE6_FRONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_SITE7_FRONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_SITE8_FRONT)->ShowWindow(SW_HIDE);

	}
	else if (g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y) == 1 && nBoardType == eBoardDir_Forward)
	{
		GetDlgItem(IDC_SPREAD_SITE1_REAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_SITE2_REAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_SITE3_REAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_SITE4_REAR)->ShowWindow(SW_HIDE);
									
		GetDlgItem(IDC_SPREAD_SITE5_REAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_SITE6_REAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_SITE7_REAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_SITE8_REAR)->ShowWindow(SW_HIDE);
	}
	
	//	if (nBoardType == eBoardDir_Backward) 
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
					pSprd->SetTextMatrix(nRow+1, 0, strNo);
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


int CMainLower_TabPassFail::getCurUserLv()
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CSLTView* pSltView = (CSLTView*)pMainFrm->GetActiveView();
	int nUserLevel = pSltView->m_nLogInLv;

	return nUserLevel;
}

void CMainLower_TabPassFail::Refresh()
{
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

	int nTestSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nTestSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	const int nMaxSiteCountPerPress = ( STATION_MAX_X_SITE*STATION_MAX_Y_SITE );
	int nPressIndex = 0;
	int nSiteIndex = 0;
	BOOL bIsSocketStatus = FALSE;
	CSpreadSheet* pSprd = NULL;
	int nSite = 0;

	int nLotState = g_DMCont.m_dmEQP.GN(NDM0_LotStatus);


	 	for( int i = 0; i < ( STATION_MAX_PARA*(eMaxPressUnitCount) ); i++ ) {
	 		strData.Empty();
	 
	 		//GRR은 사용안함.
	 		if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
	 		{
	 		 	m_nSocketOnOff[i] = g_DMCont.m_dmHandlerOpt.GN(NDM11_GRR_SocketOnOff_Site1_1 +i);
	 		}
	 		else
	 		{
	 		 	m_nSocketOnOff[i] = g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 +i);
	 		}
	 		//m_nSocketOnOff[ i ] = g_DMCont.m_dmHandlerOpt.GN( NDM11_SocketOnOff_Site1_1 + i );
	 		bIsSocketStatus |= (m_nSocketOnOff[ i ] == DEF_SITE_ON || m_nSocketOnOff[i] == DEF_SITE_OFF_RESERVATION )? TRUE : FALSE; //OR 연산 시켜서 하나라도 켜져있는지 확인함.
	 
	 		nPressIndex = i / nMaxSiteCountPerPress; //Match Index
	 		nSiteIndex = i % nMaxSiteCountPerPress;
	 
	 		int nSiteX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	 		int nSiteY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	 
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


	 		pSprd = ( CSpreadSheet* )GetDlgItem( m_nSpreadTestSiteID[ nPressIndex ][nSpreadIdx] );
	 		if( nSiteIndex == STATION_MAX_PARA - 1 )
	 		{
	 			m_dTestTime[ nPressIndex ] = g_DMCont.m_dmEQP.GN( NDM0_Press1_TestTm + nPressIndex ) / 1000.0;
	 			if( bIsSocketStatus == DEF_SITE_ON )
	 			{
	 				strData.Format( "Index=%.2f", g_DMCont.m_dmEQP.GD( DDM0_TmIndexPress1 + nPressIndex ) );
	 				SetDlgItemText( m_nIndexID[ nPressIndex ], strData );
	 				strData.Format( "TestTime=%.2f", m_dTestTime[ nPressIndex ] );
	 				m_nTestTm[ nPressIndex ].SetWindowTextA( strData );
	 			}else {
	 				SetDlgItemText( m_nIndexID[ nPressIndex ], _T( "-" ) );
	 				m_nTestTm[ nPressIndex ].SetWindowTextA( _T( "OFF" ) );
	 			}
	 			bIsSocketStatus = FALSE;
	 		}
	 
	 		pSprd->SetBorderStyle( 8 );
	 
	 		int	inx = nSiteIndex + (nPressIndex * (STATION_MAX_PARA));
	 	
	 
	 		int nSiteHistoryBits = g_DMCont.m_dmContac_Dn.GN( NDM1_SiteHistory1_1 + inx);
			int nRetSiteStatusHis = g_DMCont.m_dmContac_Dn.GN(NDM1_SiteStatusHistory1_1 + inx);
	 		int nOldHis = g_DMCont.m_dmContac_Up.GN(NDM2_SiteHistoryHW1_1 + inx);
	 
			//int nStationInfo = g_DMCont.m_dm

	 		int nCalNo[5] = { 0, };
	 		for (int j = 0; j < 5; j++)
	 		{
	 			int nData = nOldHis % 10;
	 			nCalNo[4-j] = nData;
	 			nOldHis /= 10;
	 		}
	 		CString strHwData = _T("");
	 
			int nadd = 0;
	 		const int nMaxHistorys = pSprd->GetMaxCols(); //디자인된 grid col 수량만큼...

			int nCol = 0;
			int nRow[4] = { 0, };


			int nRowidx = 0;
			if (nSiteIndex > 3)
				nRowidx = 4; // espreadrowcount

			if (nBoardType == eBoardDir_Forward) {
				for (int i = 0; i < eRow4; i++) {
					if (nPressIndex <= ePressUnit_Down_4) {
						nRow[i] = eRow4 - i; //4, 3, 2, 1
					}
					else {
						nRow[i] = eRow1 + i; //1, 2,3,4
					}
				}
			}
			else {
				for (int i = 0; i < eRow4; i++) {
					if (nPressIndex <= ePressUnit_Down_4) {
						nRow[i] = eRow1 + i; //1, 2,3,4
					}
					else {
						nRow[i] = eRow4 - i; //4, 3, 2, 1
					}
				}
			}

			int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
			int nAdd_Status_Col = 0;
			if (nLotState == eAutoLotState_Executing && nTesterOption == eTester_EachSocket)
			{

				if (GETBIT(nRetSiteStatusHis, 0)) //Device Exist
				{
					if (GETBIT(nRetSiteStatusHis, 1)) //Status Testing
					{
						pSprd->SetTextMatrix(nRow[nSiteIndex - nRowidx], 1, "T");
						pSprd->SetBackColor(COLOR_YELLOW);
					}
					else //Status Not Testing
					{
						pSprd->SetTextMatrix(nRow[nSiteIndex - nRowidx], 1, "T");
						pSprd->SetBackColor(COLOR_LLIGHTGRAY);
					}
				}
				else //Device Not Exist(Testing 대기 상태)
				{
					pSprd->SetTextMatrix(nRow[nSiteIndex - nRowidx], 1, "-");
					pSprd->SetBackColor(COLOR_LLIGHTGRAY);
				}

				nAdd_Status_Col++;
 			}


	 		for( int nHitory_i = 0; nHitory_i < nMaxHistorys; nHitory_i++ )
	 		{
	 			// UI표시로 인하여 임시로 조건문 추가 [8/11/2020 donghyun.shin]
	 				if(i % STATION_MAX_PARA >= (m_nTestSite_Div_X*m_nTestSite_Div_Y))
	 					continue;
	 				
					nCol = nHitory_i + 1 + nAdd_Status_Col;

	 				if (m_nSocketOnOff[i] == DEF_SITE_ON || m_nSocketOnOff[i] == DEF_SITE_OFF_RESERVATION || m_nSocketOnOff[i] == DEF_SITE_SOCKET_RETEST_OK) {

// 추가 검증 예정 [2/1/2024 dohyeong.kim]
//    						if (nHitory_i == 0 && nLotState == eAutoLotState_Executing && nTesterOption == eTester_EachSocket)
//    						{
//    							
//    							if (GETBIT(nRetSiteStatusHis, 0)) //Device Exist
//    							{
//    								if (GETBIT(nRetSiteStatusHis, 1)) //Status Testing
//    								{
//    									pSprd->SetTextMatrix(nRow[nSiteIndex - nRowidx], 1, "T");
//    									pSprd->SetBackColor(COLOR_YELLOW);
//    								}
//    								else //Status Not Testing
//    								{
//    									pSprd->SetTextMatrix(nRow[nSiteIndex - nRowidx], 1, "T");
//    									pSprd->SetBackColor(COLOR_LLIGHTGRAY);
//    								}
//    							}
//     							else //Device Not Exist(Testing 대기 상태)
//     							{
//     								pSprd->SetTextMatrix(nRow[nSiteIndex - nRowidx], 1, "-");
//     								pSprd->SetBackColor(COLOR_LLIGHTGRAY);
//     							}
//   
//    							continue;
//    						}

	 				//history 당 2 bit ( 0bit = onoff, 1bit = P/F )
	 				strHwData.Empty();
	 				strHwData.Format("%d", nCalNo[nHitory_i]);	

					int ntest = nPressIndex; // 0, 1, 2, 3, 4, 5, 6
					//nRow[0] 5번 0 4
					//nRow[1] 5번 1 5
					//nRow[2] 5번 2 6
					//nRow[3] 5번 3 7

	 				if (GETBIT(nSiteHistoryBits, 0)) { //사용 유무
	 					if (GETBIT(nSiteHistoryBits, 1)) {
	 						//pSprd->SetTextMatrix(nSiteIndex + 1, nHitory_i + 1, "G");
							//teaching 위치 변경.
//	 						pSprd->SetTextMatrix(nSiteIndex + 1, nHitory_i + 1, strHwData);
							pSprd->SetTextMatrix(nRow[nSiteIndex - nRowidx], nCol, strHwData);
	 						pSprd->SetBackColor(COLOR_GREEN);
	 					}
	 					else
	 					{
	 						//pSprd->SetTextMatrix(nSiteIndex + 1, nHitory_i + 1, "F");
//	 						pSprd->SetTextMatrix(nSiteIndex + 1, nHitory_i + 1, strHwData);
							pSprd->SetTextMatrix(nRow[nSiteIndex - nRowidx], nCol, strHwData);
	 						pSprd->SetBackColor(COLOR_LLIGHTRED);
	 					}
	 				}
	 				else {
//	 					pSprd->SetTextMatrix(nSiteIndex + 1, nHitory_i + 1, "-");
						pSprd->SetTextMatrix(nRow[nSiteIndex - nRowidx], nCol, "-");
	 					pSprd->SetBackColor(COLOR_LLIGHTGRAY);
	 				}
	 				//다음 history 당 2 bit로 shift
	 				nSiteHistoryBits >>= 2;
					nadd++;
	 			}
	 			else {
//	 				pSprd->SetTextMatrix(nSiteIndex + 1, nHitory_i + 1, "-");
					pSprd->SetTextMatrix(nRow[nSiteIndex - nRowidx], nCol, "-");
	 				pSprd->SetBackColor(COLOR_LLIGHTGRAY);
	 			}
				


	 		}
	 	}


 	pSprd = NULL;

	RefreshCleaning();

	int ntest = 3;
}


HBRUSH CMainLower_TabPassFail::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	HBRUSH hbr = CDialog::OnCtlColor( pDC, pWnd, nCtlColor );

	switch( nCtlColor )
	{
	case CTLCOLOR_STATIC:
		{
			for( int i = 0; i < eMaxPressUnitCount; i++ ) {
				if( pWnd->GetDlgCtrlID() == m_nTestTmCtrlID[ i ] )
				{
					if( g_DMCont.m_dmEQP.GB( BDM0_TEST_STATUS_SITE1 + i ) == TRUE ) {
						pDC->SetBkColor( RGB( 255, 255, 0 ) );
					}
					else {
						pDC->SetBkColor( RGB( 125, 125, 125 ) );
					}
				}
			}
		}
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void CMainLower_TabPassFail::position_setup()
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
		parent_client_area.Width() - ( margine * 2 ), parent_client_area.Height() - ( tab_height ),//-(margine *2)),
		SWP_NOZORDER );
}

void CMainLower_TabPassFail::RefreshConnection()
{
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	
	for (int i = 0; i < m_nTesterCount; i++)
	{
		BOOL bConnect = FALSE;
		int nTesterConnectCnt = 0;
		for(int nTesterCnt = 0; nTesterCnt < EACH_LINE_STATION_CNT; nTesterCnt++)
		{
			if (nTesterOption == eTester_EachSocket)
			{
				int nIdx = ((i* EACH_LINE_STATION_CNT) + nTesterCnt);
				BOOL bTesterConnect = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + nIdx);
				if (bTesterConnect
					== DEF_ON)
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
			if (nTesterOption == eTester_Agent)
				bConnect = g_DMCont.m_dmEQP.GB(BDM0_AGENT_CONNECTION_1 + i);
			else {
				bConnect = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + i);
			}
			if (bConnect)
				m_CtrlConnect[i].On();
			else
				m_CtrlConnect[i].Off();
		}
			
// 		if (bConnect)
// 		{
// 			m_CtrlConnect[i].On();
// 		}
// 		else
// 		{
// 			m_CtrlConnect[i].Off();
// 		}
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
// 	CString strTmp = _T("");
// 	for (int i = 0; i < m_nTesterCount; i++)
// 	{
// 		strTmp.Format("%d", i + 1);
// 		SetDlgItemText(m_nConntct[i], strTmp);
// 		m_CtrlConnect[i].SetAlignment(4);
// 		m_CtrlConnect[i].SetPictureAlignment(7);
// 		m_CtrlConnect[i].SetForeColor(COLOR_DDARKGRAY);
// 	}
}

void CMainLower_TabPassFail::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 0)
	{
		KillTimer(nIDEvent);

// 		if (strcmp(m_szCompany, DEF_COMPANY_AMKOR) == 0) {
// 		}

		RefreshCleaning();
	
		RefreshConnection();
		RefreshSiteOnOff();
//		ChkLotState(); // Socket All On/Off 조건에 맞게 버튼 클릭 활성화 핸들링

		SetTimer(0, 1000, NULL);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CMainLower_TabPassFail::RefreshSiteOnOff()
{
	CSpreadSheet* pSprd = NULL;
	int nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);

	int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	int nSite = NDM11_SocketOnOff_Site1_1;
	if (nLotStartMode == eStartMD_AutoGRR || nLotStartMode == eStartMD_AutoGRR_Continue)
	{
		nSite = NDM11_GRR_SocketOnOff_Site1_1;
	}

	//임시, simul test

	// 
	// 	int nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	// 	int nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	int nTestSite_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);


	CString strSiteNum = _T("");
 	
 	for (int Index = 0; Index < eMaxPressUnitCount; Index++)
 	{
 		pSprd = (CSpreadSheet*)GetDlgItem(m_nStationOnOff[Index]);
 

		if (Index == 1) {
			int a = 0;
		}
		/*
		//Test PP1               // Test PP2
		4 8						5 1
		3 7						6 2
		2 6						7 3
		1 5						8 4
		*/
		//int nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

		int nSpreadIdx = eOnOff_Spread_Type1;
		if (Index > ePressUnit_Down_4)
			nSpreadIdx = eOnOff_Spread_Type2;

		int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
		if (nBoardType == eBoardDir_Backward) {
			nSpreadIdx = eOnOff_Spread_Type2;
			if (Index > ePressUnit_Down_4)
				nSpreadIdx = eOnOff_Spread_Type1;
		}


		for (int nX = 0; nX < nTestSite_Div_X; nX++)
 		{
 			for (int nY = 0; nY < nTestSite_Div_Y; nY++) //  /2   0,1
 			{
 				int nNum = 0;

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
				else if(g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y) == 1)
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
		
 				int nSiteOnOff = g_DMCont.m_dmHandlerOpt.GN(/*NDM11_SocketOnOff_Site1_1*/nNum);
				int ntest = g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + 4);
 				pSprd->ClearSelection();
 
				//Pocket Draw Numbering



				CString strOnOff = nTestSite_Y == 1 ? m_strOnOffSpreadTypeY1[nSpreadIdx][nY][nX] : m_strOnOffSpreadType[nSpreadIdx][nY][nX];
 				if (nSiteOnOff == DEF_SITE_ON )
 				{
// 					strSiteNum.Format("%d", (nY * STATION_MAX_X_SITE) + nX + 1);
 					pSprd->SetTextMatrix(nY + 1, nX + 1, strOnOff);
 					pSprd->SetBackColor(COLOR_GREEN);
 				}
 				else if (nSiteOnOff == DEF_SITE_OFF_RESERVATION)
 				{
// 					strSiteNum.Format("%d", (nY * STATION_MAX_X_SITE) + nX + 1);
 					pSprd->SetTextMatrix(nY + 1, nX + 1, strOnOff);
 					pSprd->SetBackColor(COLOR_YELLOW);
 				}
 				else if (nSiteOnOff == DEF_SITE_SOCKET_RETEST_OK)
 				{
// 					strSiteNum.Format("%d", (nY * STATION_MAX_X_SITE) + nX + 1);
 					pSprd->SetTextMatrix(nY + 1, nX + 1, strOnOff);
 					pSprd->SetBackColor(COLOR_BLUE);
 				}
 				else if (nSiteOnOff == DEF_SITE_SOCKET_HOLD)
 				{
// 					strSiteNum.Format("%d", (nY * STATION_MAX_X_SITE) + nX + 1);
 					pSprd->SetTextMatrix(nY + 1, nX + 1, strOnOff);
 					pSprd->SetBackColor(COLOR_BBLACK);
 				}
 				else if (nSiteOnOff == DEF_SITE_SOCKET_LOCK)
 				{
// 					strSiteNum.Format("%d", (nY * STATION_MAX_X_SITE) + nX + 1);
 					pSprd->SetTextMatrix(nY + 1, nX + 1, strOnOff);
 					pSprd->SetBackColor(COLOR_BLUE);
 				}
				else if (nSiteOnOff == DEF_SITE_OFF_RESERVATION_YIELD)
				{
					strSiteNum.Format("%d", (nY * STATION_MAX_X_SITE) + nX + 1);
					pSprd->SetTextMatrix(nY + 1, nX + 1, "Y");
					pSprd->SetBackColor(COLOR_YELLOW);
				}
 				else
 				{
 					pSprd->SetTextMatrix(nY + 1, nX + 1, "X");
 					
 					if (strcmp(m_szCompany, DEF_COMPANY_AMKOR) == 0) {
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

void CMainLower_TabPassFail::OnBnClickedBtnClean1()
{
	SetRevClean(ePressUnit_Down_1);
}


void CMainLower_TabPassFail::OnBnClickedBtnClean2()
{
	SetRevClean(ePressUnit_Down_2);
}


void CMainLower_TabPassFail::OnBnClickedBtnClean3()
{
	SetRevClean(ePressUnit_Down_3);
}

void CMainLower_TabPassFail::OnBnClickedBtnClean4()
{
	SetRevClean(ePressUnit_Down_4);
}

void CMainLower_TabPassFail::OnBnClickedBtnClean5()
{
	SetRevClean(ePressUnit_Down_5);
}


void CMainLower_TabPassFail::OnBnClickedBtnClean6()
{
	SetRevClean(ePressUnit_Down_6);
}


void CMainLower_TabPassFail::OnBnClickedBtnClean7()
{
	SetRevClean(ePressUnit_Down_7);
}


void CMainLower_TabPassFail::OnBnClickedBtnClean8()
{
	SetRevClean(ePressUnit_Down_8);
}


void CMainLower_TabPassFail::SetRevClean(int nPressIndex)
{
	BOOL bCleanOption = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_SocketClean);
	if (bCleanOption != eOPTION_USE)
		return;

	BOOL bOneCycle = g_DMCont.m_dmEQP.GB(BDM0_SYS_ONE_CYCLE);
	if (bOneCycle == TRUE)
		return;

// 	int nMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
// 	if (nMode == eStartMD_AutoGRR || nMode == eStartMD_AutoGRR_Continue)
// 		return;

	//Cleaning 진행 중 예약을 Off하는경우 행업 발생 예약 On만 하는것으로 변경
// 	if(g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE1 + nPressIndex))
// 		g_DMCont.m_dmHandlerOpt.SB(BDM11_MANU_CLN_REV_SITE1 + nPressIndex, FALSE);
// 	else
// 		g_DMCont.m_dmHandlerOpt.SB(BDM11_MANU_CLN_REV_SITE1 + nPressIndex, TRUE);

	g_DMCont.m_dmHandlerOpt.SB(BDM11_MANU_CLN_REV_SITE1 + nPressIndex, TRUE);

	CString strLog = _T("");
	strLog.Format("Manual Clean reserve Station=%d ", nPressIndex + 1);
	TWLOG.MakeLog(static_cast<int>(eLogID_System_GUI), Debug, "", __LINE__, NULL, strLog);
}

void CMainLower_TabPassFail::RefreshCleaning()
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
	bRev  |= g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE2_RESERVE);
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

void CMainLower_TabPassFail::CompanySetting()
{

	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	if (nTesterOption == eTester_EachSocket) // Single Tester [8/27/2021 donghyun.shin]
	{
// 		GetDlgItem(IDC_CLRCHK_CONNECT9)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_CLRCHK_CONNECT10)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_CLRCHK_CONNECT11)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_CLRCHK_CONNECT12)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_CLRCHK_CONNECT13)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_CLRCHK_CONNECT14)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_CLRCHK_CONNECT15)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_CLRCHK_CONNECT16)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_CLRCHK_CONNECT9)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT9)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT9)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT9)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT9)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT9)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT9)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT9)->ShowWindow(SW_HIDE);
 	}
	else {

	}
// 		GetDlgItem(IDC_BTN_CLEAN_1)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_2)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_3)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_4)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_5)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_6)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_7)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_8)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_9)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_10)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_11)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_12)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_13)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_14)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_15)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_16)->ShowWindow(SW_HIDE);
	
// 	else if (strcmp(m_szCompany, DEF_COMPANY_USI) == 0) {
// 		GetDlgItem(IDC_BTN_CLEAN_1)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_2)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_3)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_4)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_5)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_6)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_7)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_8)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_9)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_10)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_11)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_12)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_13)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_14)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_15)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_CLEAN_16)->ShowWindow(SW_HIDE);
// 
// 	}


}BEGIN_EVENTSINK_MAP(CMainLower_TabPassFail, CDialogEx)

ON_EVENT(CMainLower_TabPassFail, IDC_SPREAD_PASSFAIL_STATION1, 11, CMainLower_TabPassFail::DblClickSpreadStation1, VTS_I4 VTS_I4)
ON_EVENT(CMainLower_TabPassFail, IDC_SPREAD_PASSFAIL_STATION2, 11, CMainLower_TabPassFail::DblClickSpreadStation2, VTS_I4 VTS_I4)
ON_EVENT(CMainLower_TabPassFail, IDC_SPREAD_PASSFAIL_STATION3, 11, CMainLower_TabPassFail::DblClickSpreadStation3, VTS_I4 VTS_I4)
ON_EVENT(CMainLower_TabPassFail, IDC_SPREAD_PASSFAIL_STATION4, 11, CMainLower_TabPassFail::DblClickSpreadStation4, VTS_I4 VTS_I4)
ON_EVENT(CMainLower_TabPassFail, IDC_SPREAD_PASSFAIL_STATION5, 11, CMainLower_TabPassFail::DblClickSpreadStation5, VTS_I4 VTS_I4)
ON_EVENT(CMainLower_TabPassFail, IDC_SPREAD_PASSFAIL_STATION6, 11, CMainLower_TabPassFail::DblClickSpreadStation6, VTS_I4 VTS_I4)
ON_EVENT(CMainLower_TabPassFail, IDC_SPREAD_PASSFAIL_STATION7, 11, CMainLower_TabPassFail::DblClickSpreadStation7, VTS_I4 VTS_I4)
ON_EVENT(CMainLower_TabPassFail, IDC_SPREAD_PASSFAIL_STATION8, 11, CMainLower_TabPassFail::DblClickSpreadStation8, VTS_I4 VTS_I4)


ON_EVENT(CMainLower_TabPassFail, IDC_SPREAD_PASSFAIL_STATION1, 22, CMainLower_TabPassFail::RightClickSpreadStation1, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
ON_EVENT(CMainLower_TabPassFail, IDC_SPREAD_PASSFAIL_STATION2, 22, CMainLower_TabPassFail::RightClickSpreadStation2, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
ON_EVENT(CMainLower_TabPassFail, IDC_SPREAD_PASSFAIL_STATION3, 22, CMainLower_TabPassFail::RightClickSpreadStation3, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
ON_EVENT(CMainLower_TabPassFail, IDC_SPREAD_PASSFAIL_STATION4, 22, CMainLower_TabPassFail::RightClickSpreadStation4, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
ON_EVENT(CMainLower_TabPassFail, IDC_SPREAD_PASSFAIL_STATION5, 22, CMainLower_TabPassFail::RightClickSpreadStation5, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
ON_EVENT(CMainLower_TabPassFail, IDC_SPREAD_PASSFAIL_STATION6, 22, CMainLower_TabPassFail::RightClickSpreadStation6, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
ON_EVENT(CMainLower_TabPassFail, IDC_SPREAD_PASSFAIL_STATION7, 22, CMainLower_TabPassFail::RightClickSpreadStation7, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
ON_EVENT(CMainLower_TabPassFail, IDC_SPREAD_PASSFAIL_STATION8, 22, CMainLower_TabPassFail::RightClickSpreadStation8, VTS_I2 VTS_I4 VTS_I4 VTS_I4 VTS_I4)

END_EVENTSINK_MAP()


void CMainLower_TabPassFail::DblClickSpreadStation1(long Col, long Row)
{
	int nStation = ePressUnit_Down_1; 

	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}


BOOL CMainLower_TabPassFail::ChkModuleUseAble(int nStation)
{
	BOOL bIsUseAble = FALSE;

	if ( ePressUnit_Down_1 <= nStation && 
		nStation <= ePressUnit_Down_4)
	{
		bIsUseAble = g_DMCont.m_dmEQP.GB(BDM0_MODULE1_SITE_ON_IS_POSSIBLE);
	}
	else 
	{
		bIsUseAble = g_DMCont.m_dmEQP.GB(BDM0_MODULE2_SITE_ON_IS_POSSIBLE);
	}

	return bIsUseAble;
}


void CMainLower_TabPassFail::setAllStationOnOffFromDM() 
{
// 	for (int nStation = 0 ; nStation < eMaxPressUnitCount ; nStation++)
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
							nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nY  + nX * 4;
						}
						else {
							nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(nY - 3) + nX * 4;
							//nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nY + nX * 4;
						}
					}
				}
				m_nSiteOnOff[nStation][nY][nX] = g_DMCont.m_dmHandlerOpt.GN(nNum);
			}
		}
	}

}


BOOL CMainLower_TabPassFail::ChkAllStationOFF()
{
	int nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++) 
	{
		for (int nCol = 0; nCol < nTestSite_Div_Y; nCol++)
		{
			for (int nRow = 0; nRow < nTestSite_Div_X; nRow++)
			{
				if (m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_ON)
					return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CMainLower_TabPassFail::ChkSocketOnOffInterlock()
{
	CString strErrMsg;
	CString strErrStation = _T("");
	std::vector<int> vErrEachOneSock;

	std::vector<int> vSocket;

	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

				

	if ( ChkAllStationOFF() ) //모든 Site가 꺼져있다면 
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
						if(g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y) == 1)
							vSocket.push_back(m_nSiteOnOff[nStation + (nTotal_cnt * 4)][abs(nRow - 3)][nCol+1]);
						else
							vSocket.push_back(m_nSiteOnOff[nStation + (nTotal_cnt * 4)][abs(nRow - 3)][nCol]);
					}				
					else
						vSocket.push_back(m_nSiteOnOff[nStation + (nTotal_cnt*4)][nRow][abs(nCol - 1)]);
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

BOOL CMainLower_TabPassFail::Chk4StationSocketOff(std::vector<int> vSocket)
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

BOOL CMainLower_TabPassFail::ChkEachOneMoreSocketOn(std::vector<int> vSocket)
{
	int szSock[STATION_MAX_PARA] = { 0, };

	int nSocketOnCount = 0;
	//int nStation_Max_para = STATION_MAX_X_SITE * STATION_MAX_Y_SITE;
	int nStation_Max_para = STATION_MAX_PARA;

// 	for (int i = 0; i < (int)vSocket.size(); i++) { //32
// 		switch (i % nStation_Max_para)
// 		{
// 		case 0:	ChkEachOneMoreSocketState(vDown[i], vUp[i], szSock[0]);	break;
// 		case 1:	ChkEachOneMoreSocketState(vDown[i], vUp[i], szSock[1]);	break;
// 		case 2:	ChkEachOneMoreSocketState(vDown[i], vUp[i], szSock[2]);	break;
// 		case 3:	ChkEachOneMoreSocketState(vDown[i], vUp[i], szSock[3]);	break;
// 		case 4:	ChkEachOneMoreSocketState(vDown[i], vUp[i], szSock[4]);	break;
// 		case 5:	ChkEachOneMoreSocketState(vDown[i], vUp[i], szSock[5]);	break;
// 		case 6:	ChkEachOneMoreSocketState(vDown[i], vUp[i], szSock[6]);	break;
// 		case 7:	ChkEachOneMoreSocketState(vDown[i], vUp[i], szSock[7]);	break;
// 		}
// 	}

	for (int i = 0; i < (int)vSocket.size(); i++) { //32
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


BOOL CMainLower_TabPassFail::ChkRetestTwoMoreStaionOn(std::vector<int> vSocket)
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

void CMainLower_TabPassFail::ChkEachOneMoreSocketState(int _station, int& _Socket_number)
{
	if (_station == DEF_SITE_ON) { _Socket_number++; }
}

void CMainLower_TabPassFail::ChkErrEachOneMoreSocketOn(std::vector<int> vChkSocket, std::vector<int> &vSocket)
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

void CMainLower_TabPassFail::ChkRetestTwoMoreStaionState(int _index, std::vector<int> _vSocket, int& _nStation)
{
	int nSocket = 0;
	m_nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int TestSite_Div = STATION_MAX_X_SITE*m_nTestSite_Div_Y;

	for (int i = 0; i < TestSite_Div; i++) // Socket 32ea(Test PP 1ea)
	{
		if (_vSocket[(_index * TestSite_Div) + i] == DEF_SITE_ON) { nSocket++; }
	}

	if (nSocket > 0)
		_nStation = TRUE;
}

void CMainLower_TabPassFail::ChkErrEachOneMoreSocketState(int _index, std::vector<int> _vSocket, int &_Socket)
{
	int nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	if (_vSocket[_index] == DEF_SITE_ON) { _Socket++; }
	if (_vSocket[_index + (STATION_MAX_X_SITE*nTestSite_Div_Y)] == DEF_SITE_ON) { _Socket++; }
	if (_vSocket[_index + ((STATION_MAX_X_SITE*nTestSite_Div_Y) * 2)] == DEF_SITE_ON) { _Socket++; }
	if (_vSocket[_index + ((STATION_MAX_X_SITE*nTestSite_Div_Y) * 3)] == DEF_SITE_ON) { _Socket++; }
}


void CMainLower_TabPassFail::SiteSetChangeLog(std::vector<std::pair<CString, std::pair<CString, CString>>> vData)
{
	for (int i = 0; i < (int)vData.size(); i++) {
		std::pair<CString, std::pair<CString, CString>> p;

		p = vData[i];

		MakeLog("[MENU = Site Setting, DATA NAME = %s ][BEFORE=%s, AFTER=%s]", p.first, p.second.first, p.second.second);
	}
}

void CMainLower_TabPassFail::MakeLog(LPCTSTR fmt, ...)
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

void CMainLower_TabPassFail::DblClickSpreadStation2(long Col, long Row)
{
	int nStation = ePressUnit_Down_2;

	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}


void CMainLower_TabPassFail::DblClickSpreadStation3(long Col, long Row)
{
	int nStation = ePressUnit_Down_3;

	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}


void CMainLower_TabPassFail::DblClickSpreadStation4(long Col, long Row)
{
	int nStation = ePressUnit_Down_4;

	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}


void CMainLower_TabPassFail::DblClickSpreadStation5(long Col, long Row)
{
	int nStation = ePressUnit_Down_5;

	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}


void CMainLower_TabPassFail::DblClickSpreadStation6(long Col, long Row)
{
	int nStation = ePressUnit_Down_6;

	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}


void CMainLower_TabPassFail::DblClickSpreadStation7(long Col, long Row)
{
	int nStation = ePressUnit_Down_7;

	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}


void CMainLower_TabPassFail::DblClickSpreadStation8(long Col, long Row)
{
	int nStation = ePressUnit_Down_8;

	CheckRunningAndSwitchOnOff(nStation, Col, Row);
}

void CMainLower_TabPassFail::CheckRunningAndSwitchOnOff(int nStation, long Col, long Row , BOOL bTypeAll,BOOL bHoldType/* = FALSE */)
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

	if(bHoldType != TRUE)
		switchOnOff(nStation, Col, Row , bTypeAll);	
	else
		switchSiteHold(nStation, Col, Row, bTypeAll);

}

void CMainLower_TabPassFail::ChangeAllSocketStatus(int nStation) //하나라도 꺼져있거나 다 꺼져있으면 On , 다 켜져있으면 Off
{
	int nOnOff = DEF_SITE_OFF ;
	//체크


	int nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);

	for (int nCol = 0; nCol < nTestSite_Div_Y; nCol++)
	{
		for (int nRow = 0; nRow < nTestSite_Div_X; nRow++)
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

	for (int nCol = 0; nCol < nTestSite_Div_Y; nCol++)
	{
		for (int nRow = 0; nRow < nTestSite_Div_X; nRow++)
		{
			if (m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_HOLD || m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_LOCK)
			{

			}
			else
				m_nSiteOnOff[nStation][nRow][nCol] = nOnOff;
		}
	}
}



void CMainLower_TabPassFail::switchOnOff(int nStation, int Col, int Row , BOOL bTypeAll /* = FALSE */ )
{
	setAllStationOnOffFromDM();  //DM에서 현재 해당 Station의 전체Site의 On/Off상태를 불러옴.

	int nCol = 0;
 	int nRow = 0;


	int nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);

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
	//int nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
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
				for (int nCol = 0; nCol < nTestSite_Div_Y; nCol++)
				{
					for (int nRow = 0; nRow < nTestSite_Div_X; nRow++)
					{
//						g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + (nStation * (STATION_MAX_X_SITE * STATION_MAX_Y_SITE)) + (nRow * STATION_MAX_X_SITE) + nCol, m_nSiteOnOff[nStation][nRow][nCol]);
						
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
								int ntest = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation;
								if (nStation <= ePressUnit_Down_4)
									//nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + (Row - 1) + abs(Col - 2) * 4;
									nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nRow;
								else
									nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(nRow - 3);
									//nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nRow  + (Col - 4) * 4;
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
			//g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + (Col - 1) + (Row - 1)*STATION_MAX_X_SITE, m_nSiteOnOff[nStation][Row - 1][Col - 1]);
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
			stBasicData.nSiteOnOff[nStation][(Row - 1)][(Col - 1)] = m_nSiteOnOff[nStation][(Row - 1)][(Col - 1)];
			
		}
		stBasicData.nDivision = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
		g_COK_FileMgr.SaveBasicInfo(szCokName, &stBasicData);

		std::vector<std::pair<CString, std::pair<CString, CString>>>vData;

		stBeforeBasicData.IsSameSiteSet(stBasicData, vData);

		if (!(vData.empty()))
		{
			SiteSetChangeLog(vData);
		}
	}
}

void CMainLower_TabPassFail::ChkLotState()
{
	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
	BOOL bRetOneCycle = g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY);

	if (bLotState == eHandlerLotMode_OnGoing)
	{
		GetDlgItem(IDC_STATIC_TEXT135)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT136)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT137)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT138)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT139)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT140)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT141)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT142)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT143)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT144)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT145)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT146)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT147)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT148)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT149)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT150)->EnableWindow(bRetOneCycle);

	}
	else
	{
		GetDlgItem(IDC_STATIC_TEXT135)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT136)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT137)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT138)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT139)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT140)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT141)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT142)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT143)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT144)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT145)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT146)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT147)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT148)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT149)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_STATIC_TEXT150)->EnableWindow(bRetOneCycle);
	}
}

void CMainLower_TabPassFail::OnStnDblclickStation1()
{
	int nStation = ePressUnit_Down_1;
	int Col = m_nCol;
	int Row = m_nRow;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, TRUE);
}

void CMainLower_TabPassFail::OnStnDblclickStation2()
{
	int nStation = ePressUnit_Down_2;
	int Col = m_nCol;
	int Row = m_nRow;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, TRUE);
}


void CMainLower_TabPassFail::OnStnDblclickStation3()
{
	int nStation = ePressUnit_Down_3;
	int Col = m_nCol;
	int Row = m_nRow;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, TRUE);
}


void CMainLower_TabPassFail::OnStnDblclickStation4()
{
	int nStation = ePressUnit_Down_4;
	int Col = m_nCol;
	int Row = m_nRow;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, TRUE);
}


void CMainLower_TabPassFail::OnStnDblclickStation5()
{
	int nStation = ePressUnit_Down_5;
	int Col = m_nCol;
	int Row = m_nRow;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, TRUE);
}


void CMainLower_TabPassFail::OnStnDblclickStation6()
{
	int nStation = ePressUnit_Down_6;
	int Col = m_nCol;
	int Row = m_nRow;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, TRUE);
}


void CMainLower_TabPassFail::OnStnDblclickStation7()
{
	int nStation = ePressUnit_Down_7;
	int Col = m_nCol;
	int Row = m_nRow;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, TRUE);
}


void CMainLower_TabPassFail::OnStnDblclickStation8()
{
	int nStation = ePressUnit_Down_8;
	int Col = m_nCol;
	int Row = m_nRow;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, TRUE);
}


void CMainLower_TabPassFail::RightClickSpreadStation1(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
	int nRet = ShowLoginBox();
	if (nRet != TRUE)
		return;

	int nStation = ePressUnit_Down_1;
	
	CheckRunningAndSwitchOnOff(nStation, Col, Row, FALSE, TRUE);
}


void CMainLower_TabPassFail::RightClickSpreadStation2(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
	int nRet = ShowLoginBox();
	if (nRet != TRUE)
		return;

	int nStation = ePressUnit_Down_2;

	CheckRunningAndSwitchOnOff(nStation, Col, Row,FALSE,TRUE);
}


void CMainLower_TabPassFail::RightClickSpreadStation3(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
	int nRet = ShowLoginBox();
	if (nRet != TRUE)
		return;

	int nStation = ePressUnit_Down_3;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, FALSE, TRUE);
}


void CMainLower_TabPassFail::RightClickSpreadStation4(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
	int nRet = ShowLoginBox();
	if (nRet != TRUE)
		return;

	int nStation = ePressUnit_Down_4;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, FALSE, TRUE);
}


void CMainLower_TabPassFail::RightClickSpreadStation5(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
	int nRet = ShowLoginBox();
	if (nRet != TRUE)
		return;

	int nStation = ePressUnit_Down_5;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, FALSE, TRUE);
}


void CMainLower_TabPassFail::RightClickSpreadStation6(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
	int nRet = ShowLoginBox();
	if (nRet != TRUE)
		return;

	int nStation = ePressUnit_Down_6;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, FALSE, TRUE);
}

void CMainLower_TabPassFail::RightClickSpreadStation7(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
	int nRet = ShowLoginBox();
	if (nRet != TRUE)
		return;

	int nStation = ePressUnit_Down_7;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, FALSE, TRUE);
}


void CMainLower_TabPassFail::RightClickSpreadStation8(short ClickType, long Col, long Row, long MouseX, long MouseY)
{
	int nRet = ShowLoginBox();
	if (nRet != TRUE)
		return;

	int nStation = ePressUnit_Down_8;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, FALSE, TRUE);
}


BOOL CMainLower_TabPassFail::ShowLoginBox()
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


BOOL CMainLower_TabPassFail::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_RBUTTONDOWN)
	{
		for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++)
		{
			if (pMsg->hwnd == GetDlgItem(IDC_STATIC_TEXT_STATION1 + nStation)->GetSafeHwnd())
			{
				PostMessage(UM_SITE_HOLD, nStation);
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


LRESULT CMainLower_TabPassFail::OnSiteHold(WPARAM wParam, LPARAM lParam)
{
	int nRet = ShowLoginBox();
	if (nRet != TRUE)
		return 0L;

	int nStation = (int)wParam;
	int Col = m_nCol;
	int Row = m_nRow;

	CheckRunningAndSwitchOnOff(nStation, Col, Row, TRUE,TRUE);
	return 0L;
}

void CMainLower_TabPassFail::switchSiteHold(int nStation, int Col, int Row, BOOL bTypeAll /* = FALSE */)
{
	setAllStationOnOffFromDM();  //DM에서 현재 해당 Station의 전체Site의 On/Off상태를 불러옴.

	m_nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	m_nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

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
									nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nRow;
								else
									nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(nRow - 3);
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

void CMainLower_TabPassFail::ChangeAllSocketHoldStatus(int nStation) //하나라도 꺼져있거나 다 꺼져있으면 On , 다 켜져있으면 Off
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
				if(m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_LOCK)
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

int CMainLower_TabPassFail::TesterConnectChk(int nStation,int nX,int nY) //하나라도 꺼져있거나 다 꺼져있으면 On , 다 켜져있으면 Off
{
	BOOL bConnect = FALSE;
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	
	if (nTesterOption == eTester_Single)
		bConnect = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + nStation);
	else if (nTesterOption == eTester_EachSocket)
	{
		int nSocketArr1[STATION_MAX_X_SITE][STATION_MAX_Y_SITE] = { {5,1},{6,2},{7,3},{8,4} };
		int nSocketArr2[STATION_MAX_X_SITE][STATION_MAX_Y_SITE] = { { 1,5 },{ 2,6 },{ 3,7 },{ 4,8 } };
		int nSocketIdx = nStation < eMaxPressUnitCount/2? nSocketArr1[nY - 1][nX - 1] : nSocketArr2[nY - 1][nX - 1];
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
		if(nY == eRearTester+1)
			bConnect = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + ((nStation * 2) - 1)  +1) ;
		else
			bConnect = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + (nStation * 2) +1);
	}	
	else
		bConnect = g_DMCont.m_dmEQP.GB(BDM0_AGENT_CONNECTION_1 + nStation);

	return bConnect;
}

void CMainLower_TabPassFail::StationParaSetting()
{
	
	CSpreadSheet* pSprd = NULL;
	int nHistoryHeight = 12;
	int nSiteOnOffHeight = 10;
	int nSiteOnOffRow = 8;
	int nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	

//[작업 필요] testsite Division 변경 되어야 함.
// simulation running test를 위해 잠시 냅둠.
//	if (m_nBefore_TestSite_Div_X != nTestSite_Div_X || m_nBefore_TestSite_Div_Y != nTestSite_Div_Y)
//	{
		m_nBefore_TestSite_Div_X = nTestSite_Div_X;
		m_nBefore_TestSite_Div_Y = nTestSite_Div_Y;

		if (nTestSite_Div_Y == 1) {
			nHistoryHeight = 26;
			nSiteOnOffHeight = 6;

		}
		else if (nTestSite_Div_Y == STATION_MAX_Y_SITE) {
			nHistoryHeight = 12;
			nSiteOnOffHeight = 3;
		}


		//nSiteOnOffHeight = 20.29;
		for (int i = 0; i < eMaxPressUnitCount; i++) {
// 			for (int j = 0; j < eMaxSpreadIdx; j++) {
// 				pSprd = (CSpreadSheet*)GetDlgItem(m_nSpreadTestSiteID[i][j]);
// 				pSprd->SetMaxRows(nTestSite_Div_X * m_nTestSite_Div_Y);
// 
// 				for (int nRow = 1; nRow <= nTestSite_Div_X; nRow++) {
// 					pSprd->SetRowHeight(nRow, nHistoryHeight);
// 
// 					for (int nHistory = 1; nHistory <= 5; nHistory++) {
// 						pSprd->SetCol(nHistory);
// 						pSprd->SetRow(nRow);
// 						pSprd->SetTypeHAlign(2);//center
// 					}
// 				}
// 
// 				pSprd = NULL;
// 			}

			// STATION_MAX_X_SITE
			// STATION_MAX_X_SITE
			CString strSpreadStation[4][2] = { { _T("4"), _T("8") }
											  ,{ _T("3"), _T("7") }
											  ,{ _T("2"), _T("6") }
											  ,{ _T("1"), _T("5") } };
			

			pSprd = (CSpreadSheet*)GetDlgItem(m_nStationOnOff[i]);
// 			pSprd->SetMaxRows(m_nTestSite_Div_Y);
// 			pSprd->SetMaxCols(nTestSite_Div_X);

			pSprd->SetMaxRows(nTestSite_Div_X);
			pSprd->SetMaxCols(nTestSite_Div_Y);

			for (int nRow = 1; nRow <= nTestSite_Div_X; nRow++) { //m_nTestSite_div_Y == 4			
				for (int nCol = 1; nCol <= nTestSite_Div_Y; nCol++) {
					pSprd->SetColWidth(nCol, nSiteOnOffHeight);
					pSprd->SetCol(nCol);
					pSprd->SetRow(nRow);
					pSprd->SetTypeHAlign(2);//center

//					pSprd->SetTextMatrix(nRow, nCol, strSpreadStation[nRow - 1][nCol - 1]);
				}
			}

			pSprd = NULL;
		}
		
//	}
}

void CMainLower_TabPassFail::ConvertRowColLogicalIdx(int nSpreadType, int nPhyRow, int nPhyCol, int &nLogRow, int &nLogCol)
{
	if (nLogCol == eOnOff_Spread_Type1) {
		if (nPhyRow >= 0 && nPhyRow <= 3)	nLogRow = 0;
		else								nLogRow = 1;

	
	}

	int nColIdx[8] = { 0,1,2,3 };
	int nRowIdx[8] = { 0,0,0,0,1,1,1,1 };

}