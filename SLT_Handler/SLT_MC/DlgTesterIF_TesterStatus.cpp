// DlgTesterIF_TesterStatus.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgTesterIF_TesterStatus.h"
#include "afxdialogex.h"
#include "DlgTesterIFInfo.h"


// CDlgTesterIF_TesterStatus 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTesterIF_TesterStatus, CDialogEx)

CDlgTesterIF_TesterStatus::CDlgTesterIF_TesterStatus(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTesterIF_TesterStatus::IDD, pParent)
{
	m_nSelectedStationID = 0;

	m_TestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	m_TestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

}

CDlgTesterIF_TesterStatus::~CDlgTesterIF_TesterStatus()
{
}

void CDlgTesterIF_TesterStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_SPREAD_TESTER_STATUS, m_spreadTesterStatus1_4 );
	DDX_Control( pDX, IDC_SPREAD_TESTER_STATUS2, m_spreadTesterStatus5_8 );
}


BEGIN_MESSAGE_MAP(CDlgTesterIF_TesterStatus, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_DETAIL_TESTER_INFO, &CDlgTesterIF_TesterStatus::OnBnClickedButtonDetailTesterInfo)
	ON_BN_CLICKED( IDOK, &CDlgTesterIF_TesterStatus::OnBnClickedOk )
END_MESSAGE_MAP()


// CDlgTesterIF_TesterStatus 메시지 처리기입니다.


BOOL CDlgTesterIF_TesterStatus::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	

	if (nTesterOption == eTester_Dual || m_TestSite_Div_Y == 1)
		GetDlgItem(IDC_SPREAD_TESTER_STATUS2)->ShowWindow(SW_HIDE);


	SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CDlgTesterIF_TesterStatus::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgTesterIF_TesterStatus::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgTesterIF_TesterStatus::OnTimer(UINT_PTR nIDEvent)
{
	switch( nIDEvent )
	{
	case 1:
		{
			KillTimer(1);
			UpdateTesterStatus();
			SetTimer(1, 500, NULL);
		}break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CDlgTesterIF_TesterStatus::OnBnClickedButtonDetailTesterInfo()
{
	if( m_nSelectedStationID < 0 ){
		return;
	}
	CDlgTesterIFInfo dlg(m_nSelectedStationID, this);
	dlg.DoModal();
}

#include <vector>
#include <utility>
#include <string>

void CDlgTesterIF_TesterStatus::UpdateTesterStatus()
{
	if (m_nSelectedStationID < 1)
	{
		return;
	}

	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	CTesterStatus cTesterStatus = g_AppleTestIF.GetTesterStatus(m_nSelectedStationID, nTesterOption);


	int TestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int TestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	int nStatus = STATUS_INIT;
	if (nTesterOption == eTester_EachSocket) // Single Tester [8/27/2021 donghyun.shin]
	{
		
		for (int t = 0; t < TestSite_Div_X*TestSite_Div_Y; t++)
		{
			if (cTesterStatus.m_nYanMadeStatus[t] == STATUS_TESTING) {
				nStatus = STATUS_TESTING;
				break;
			}
			if (cTesterStatus.m_nYanMadeStatus[t] == STATUS_IDLE) {
				nStatus = STATUS_IDLE;
			}
		}
		SetDlgItemText(IDC_STATIC_TESTER_STATUS, GetStatus(nStatus));


		if (nStatus == STATUS_TESTING || nStatus == STATUS_INIT) {
			ClearStatus();
			return;
		}

	}
	else
	{
		SetDlgItemText( IDC_STATIC_TESTER_STATUS, cTesterStatus.GetStatus() );
	}
	// Station ID
	SetDlgItemInt( IDC_STATIC_STATION_ID, cTesterStatus.m_nStationID );
	// Tester Status
	//SetDlgItemText( IDC_STATIC_TESTER_STATUS, cTesterStatus.GetStatus() );

	for( int nY = 0; nY < TestSite_Div_Y; nY++ ) {
		for( int nX = 0; nX < TestSite_Div_X; nX++ ) {

			if( nY <= 0 ) {
				if (nTesterOption != eTester_EachSocket)
				{
					m_spreadTesterStatus1_4.SetTextMatrix(1, nX + 1, cTesterStatus.m_sSocketID[nX]);
				}
				

				CString sTestResult = "";
				if( cTesterStatus.m_nLastTestResult[ nX ] == TEST_RESULT_PASS ) {
					sTestResult = _T( "PASS" );
				}
				else if (cTesterStatus.m_nLastTestResult[nX] == TEST_RESULT_FAIL) {
					sTestResult = _T("FAIL");
				}
				else {
					sTestResult = _T( "" );
				}

				CString strPhysicDutNo, strLastReserved;
				if (nTesterOption == eTester_EachSocket) // Single Tester [8/27/2021 donghyun.shin]
				{
					strPhysicDutNo.Format("%d", cTesterStatus.m_nRegiDutIdentityNo[nX]);
					m_spreadTesterStatus1_4.SetTextMatrix(1, nX + 1, strPhysicDutNo);
				}


				strLastReserved.Format("%d", cTesterStatus.m_nLastReserved[nX]);
				
				m_spreadTesterStatus1_4.SetTextMatrix( 2, nX+1, sTestResult );
				m_spreadTesterStatus1_4.SetTextMatrix( 3, nX + 1, strLastReserved);
				m_spreadTesterStatus1_4.SetTextMatrix( 4, nX+1, cTesterStatus.m_sLastErrString[ nX ] );
				m_spreadTesterStatus1_4.SetTextMatrix( 5, nX+1, cTesterStatus.m_sLastDutSN[ nX ] );
			}
			else {
				if (nTesterOption != eTester_EachSocket)
				{
					m_spreadTesterStatus5_8.SetTextMatrix(1, nX + 1, cTesterStatus.m_sSocketID[nX + TestSite_Div_X]);
				}

				CString sTestResult = "";
				if( cTesterStatus.m_nLastTestResult[ nX + TestSite_Div_X ] == TEST_RESULT_PASS ) {
					sTestResult = _T( "PASS" );
				}
				else if (cTesterStatus.m_nLastTestResult[nX + TestSite_Div_X] == TEST_RESULT_FAIL) {
					sTestResult = _T("FAIL");
				}
				else {
					sTestResult = _T( "" );
				}

				
				CString strPhysicDutNo, strLastReserved;
				if (nTesterOption == eTester_EachSocket) // Single Tester [8/27/2021 donghyun.shin]
				{
					strPhysicDutNo.Format("%d", cTesterStatus.m_nRegiDutIdentityNo[nX + TestSite_Div_X]);
					m_spreadTesterStatus5_8.SetTextMatrix(1, nX + 1, strPhysicDutNo);
				}

				strLastReserved.Format("%d", cTesterStatus.m_nLastReserved[nX + TestSite_Div_X]);

			
				m_spreadTesterStatus5_8.SetTextMatrix( 2, nX + 1, sTestResult );
				m_spreadTesterStatus5_8.SetTextMatrix( 3, nX + 1, strLastReserved);
				m_spreadTesterStatus5_8.SetTextMatrix( 4, nX + 1, cTesterStatus.m_sLastErrString[ nX + TestSite_Div_X ] );
				m_spreadTesterStatus5_8.SetTextMatrix( 5, nX + 1, cTesterStatus.m_sLastDutSN[ nX + TestSite_Div_X ] );
			}

		}
	}
	// heart bit
	( ( CButton* )GetDlgItem( IDC_CHECK_HEART_BIT_LED ) )->SetCheck( cTesterStatus.m_bHeartBit );
}

void CDlgTesterIF_TesterStatus::ClearStatus()
{
	CString strEmpty = _T("");
	for (int nRow = 0; nRow < 4; nRow++)
	{
		for (int nCol = 0; nCol < 4; nCol++)
		{
			if(nRow == 0) continue;

			m_spreadTesterStatus1_4.SetTextMatrix(nRow + 1, nCol + 1, strEmpty);
			m_spreadTesterStatus5_8.SetTextMatrix(nRow + 1, nCol + 1, strEmpty);
		}
	}
}

void CDlgTesterIF_TesterStatus::SetTesterUnitIdx( int nTesterID )
{
	m_nSelectedStationID = nTesterID;
}


void CDlgTesterIF_TesterStatus::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}
BEGIN_EVENTSINK_MAP( CDlgTesterIF_TesterStatus, CDialogEx )
	ON_EVENT( CDlgTesterIF_TesterStatus, IDC_SPREAD_TESTER_STATUS, 5, CDlgTesterIF_TesterStatus::ClickSpreadTesterStatus, VTS_I4 VTS_I4 )
	ON_EVENT(CDlgTesterIF_TesterStatus, IDC_SPREAD_TESTER_STATUS2, 5, CDlgTesterIF_TesterStatus::ClickSpreadTesterStatus2, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


void CDlgTesterIF_TesterStatus::ClickSpreadTesterStatus( long Col, long Row )
{
	CString data = m_spreadTesterStatus1_4.GetTextMatrix( Row, Col );
	SetDlgItemText( IDC_STATIC_SELECT_CELL_DETAIL, data );
}


void CDlgTesterIF_TesterStatus::ClickSpreadTesterStatus2(long Col, long Row)
{
	CString data = m_spreadTesterStatus5_8.GetTextMatrix( Row, Col );
	SetDlgItemText( IDC_STATIC_SELECT_CELL_DETAIL, data );
	 
}

CString CDlgTesterIF_TesterStatus::GetStatus(int nStatus)
{
	CString sStatus;
	switch (nStatus)
	{
	case STATUS_INIT:		sStatus = "INIT";		break;
	case STATUS_IDLE:		sStatus = "ILDE";		break;
	case STATUS_TESTING:	sStatus = "TESTING";	break;
	case STATUS_ERROR:		sStatus = "ERROR";		break;
	case STATUS_UNREGISTER:	sStatus = "UNREGISTER"; break;
	}
	return sStatus;
};
