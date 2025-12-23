// CMainLower_TabDesc.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "MainLower_TabLastDecs.h"
#include "afxdialogex.h"


// CMainLower_TabDesc 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMainLower_TabDesc, CDialogEx )

CMainLower_TabDesc::CMainLower_TabDesc( CWnd* pParent /*=NULL*/ )
	: CDialogEx(CMainLower_TabDesc::IDD, pParent )
{
	memset( m_dTestTime, 0x00, sizeof( m_dTestTime ) );
	memset( m_nTestTmCtrlID, 0x00, sizeof( m_nTestTmCtrlID ) );
	memset( m_nIndexID, 0x00, sizeof( m_nIndexID ) );
	memset( m_nSocketOnOff, 0x00, sizeof( m_nSocketOnOff ) );

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

// 	m_nSpreadTestSiteID[ 0 ] = IDC_SPREAD_SITE1;
// 	m_nSpreadTestSiteID[ 1 ] = IDC_SPREAD_SITE2;
// 	m_nSpreadTestSiteID[ 2 ] = IDC_SPREAD_SITE3;
// 	m_nSpreadTestSiteID[ 3 ] = IDC_SPREAD_SITE4;
// 	m_nSpreadTestSiteID[ 4 ] = IDC_SPREAD_SITE5;
// 	m_nSpreadTestSiteID[ 5 ] = IDC_SPREAD_SITE6;
// 	m_nSpreadTestSiteID[ 6 ] = IDC_SPREAD_SITE7;
// 	m_nSpreadTestSiteID[ 7 ] = IDC_SPREAD_SITE8;

	m_nSpreadTestSiteID[0][eFront] = IDC_SPREAD_LASTDESC_SITE1_FRONT;
	m_nSpreadTestSiteID[0][eRear] = IDC_SPREAD_LASTDESC_SITE1_REAR;
	m_nSpreadTestSiteID[1][eFront] = IDC_SPREAD_LASTDESC_SITE2_FRONT;
	m_nSpreadTestSiteID[1][eRear] = IDC_SPREAD_LASTDESC_SITE2_REAR;
	m_nSpreadTestSiteID[2][eFront] = IDC_SPREAD_LASTDESC_SITE3_FRONT;
	m_nSpreadTestSiteID[2][eRear] = IDC_SPREAD_LASTDESC_SITE3_REAR;
	m_nSpreadTestSiteID[3][eFront] = IDC_SPREAD_LASTDESC_SITE4_FRONT;
	m_nSpreadTestSiteID[3][eRear] = IDC_SPREAD_LASTDESC_SITE4_REAR;
	m_nSpreadTestSiteID[4][eFront] = IDC_SPREAD_LASTDESC_SITE5_FRONT;
	m_nSpreadTestSiteID[4][eRear] = IDC_SPREAD_LASTDESC_SITE5_REAR;
	m_nSpreadTestSiteID[5][eFront] = IDC_SPREAD_LASTDESC_SITE6_FRONT;
	m_nSpreadTestSiteID[5][eRear] = IDC_SPREAD_LASTDESC_SITE6_REAR;
	m_nSpreadTestSiteID[6][eFront] = IDC_SPREAD_LASTDESC_SITE7_FRONT;
	m_nSpreadTestSiteID[6][eRear] = IDC_SPREAD_LASTDESC_SITE7_REAR;
	m_nSpreadTestSiteID[7][eFront] = IDC_SPREAD_LASTDESC_SITE8_FRONT;
	m_nSpreadTestSiteID[7][eRear] = IDC_SPREAD_LASTDESC_SITE8_REAR;



	m_nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);

	m_nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	m_nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

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
	
	m_nTesterCount = MAX_TESTER_DUAL;
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
	else
	{
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
	


	
	m_nBefore_TestSite_Div_X = STATION_MAX_X_SITE;
	m_nBefore_TestSite_Div_Y = STATION_MAX_Y_SITE;

	if (nTesterOption == eTester_EachSocket)
		if(m_nTestSite_Div_Y == 1)
			m_nTesterCount = MAX_TESTER_SINGLE;
		else
			m_nTesterCount = MAX_TESTER_DUAL;
	else
		m_nTesterCount = MAX_TESTER_SINGLE;



// 	if (nTesterOption == eTester_Single || m_nTestSite_Div_Y == 1)
// 	{
// 		GetDlgItem(IDC_CLRCHK_CONNECT9)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT10)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT11)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT12)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT13)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT14)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT15)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_CLRCHK_CONNECT16)->ShowWindow(SW_HIDE);
// 	}

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

CMainLower_TabDesc::~CMainLower_TabDesc()
{
}

void CMainLower_TabDesc::DoDataExchange( CDataExchange* pDX )
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
	
}


BEGIN_MESSAGE_MAP(CMainLower_TabDesc, CDialogEx )
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMainLower_TabDesc 메시지 처리기입니다.


BOOL CMainLower_TabDesc::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	position_setup();
	//StationParaSetting(); //현재 사용 안함.

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	for( int i = 0; i < eMaxPressUnitCount; i++ )
	{
		m_nTestTm[ i ].m_fnSetFont( "Fixedsys" );
		m_nTestTm[ i ].SetTextSize( 13 );
		m_nTestTm[ i ].SetBackColor( COLOR_NOMAL );
	}

	SetTimer(0, 1000, NULL);

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

	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);

	if (g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y) == 1 && nBoardType == eBoardDir_Backward)
	{
		GetDlgItem(IDC_SPREAD_LASTDESC_SITE1_FRONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_LASTDESC_SITE2_FRONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_LASTDESC_SITE3_FRONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_LASTDESC_SITE4_FRONT)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_SPREAD_LASTDESC_SITE5_FRONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_LASTDESC_SITE6_FRONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_LASTDESC_SITE7_FRONT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_LASTDESC_SITE8_FRONT)->ShowWindow(SW_HIDE);

	}
	else if (g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y) == 1 && nBoardType == eBoardDir_Forward)
	{
		GetDlgItem(IDC_SPREAD_LASTDESC_SITE1_REAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_LASTDESC_SITE2_REAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_LASTDESC_SITE3_REAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_LASTDESC_SITE4_REAR)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_SPREAD_LASTDESC_SITE5_REAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_LASTDESC_SITE6_REAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_LASTDESC_SITE7_REAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_LASTDESC_SITE8_REAR)->ShowWindow(SW_HIDE);
	}

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

	//int nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

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


void CMainLower_TabDesc::Refresh()
{
	UpdateData();

	//CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// Show Test Site Boot Time, BinCount, 
	CString strData = _T("");
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
	int nSite = 0;
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

		int nSiteDivX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
		int nSiteDivY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

		nPressIndex = i / nMaxSiteCountPerPress; //Match Index
		int nSiteIndex = i % nMaxSiteCountPerPress;

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
		if ((i % nMaxSiteCountPerPress) == (nSiteDivX*nSiteDivY) - 1)
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
		int nRowIdx = 0;
		if (nSiteIndex > 3)
			nRowIdx = 4;

		char szReadData[128] = { 0, };
		g_DMCont.m_dmContac_Dn.GS(SDM1_SiteLastDesc1_1 + nSiteIndex + (nPressIndex * (STATION_MAX_PARA)), szReadData, sizeof(szReadData));
		strData.Format("%s", szReadData);
		pSprd->SetTextMatrix(nRow[nSiteIndex - nRowIdx], 1, strData);
//		pSprd->SetTextMatrix(nCalRow, 1, strData);
	}

	
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	pSprd = NULL;
}

void CMainLower_TabDesc::position_setup()
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

void CMainLower_TabDesc::RefreshConnection()
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
				BOOL bTesterConnect = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + ((i* EACH_LINE_STATION_CNT) + nTesterCnt));
				if (bTesterConnect == DEF_ON)
				{
					nTesterConnectCnt++;
					if (nTesterConnectCnt == EACH_LINE_STATION_CNT)
					{
						bConnect = TRUE;
					}
				}
			}
			else if (nTesterOption == eTester_Agent)
				bConnect = g_DMCont.m_dmEQP.GB(BDM0_AGENT_CONNECTION_1 + i);
			else
				bConnect = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + i);
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
				else {
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
		}
	}
}

void CMainLower_TabDesc::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 0)
	{
		KillTimer(nIDEvent);

		RefreshConnection();

		SetTimer(0, 1000, NULL);
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CMainLower_TabDesc::StationParaSetting()
{
	CSpreadSheet* pSprd = NULL;
	int nHistoryHeight = 12;

	m_nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	m_nTestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	if (m_nBefore_TestSite_Div_X != m_nTestSite_Div_X || m_nBefore_TestSite_Div_Y != m_nTestSite_Div_Y) {

		m_nBefore_TestSite_Div_X = m_nTestSite_Div_X;
		m_nBefore_TestSite_Div_Y = m_nTestSite_Div_Y;

		if (m_nTestSite_Div_Y == 1) {
			nHistoryHeight = 26;

		}
		else if (m_nTestSite_Div_Y == STATION_MAX_Y_SITE) {
			nHistoryHeight = 12;
		}

// 		for (int i = 0; i < eMaxPressUnitCount; i++) {
// 			pSprd = (CSpreadSheet*)GetDlgItem(m_nSpreadTestSiteID[i]);
// 			pSprd->SetMaxRows(m_nTestSite_Div_X * m_nTestSite_Div_Y);
// 
// 			for (int nRow = 1; nRow <= m_nTestSite_Div_X; nRow++) {
// 				pSprd->SetRowHeight(nRow, nHistoryHeight);
// 			}
// 
// 			pSprd = NULL;
// 		}
	}
}