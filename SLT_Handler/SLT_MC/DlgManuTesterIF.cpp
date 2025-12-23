// DlgManuTesterIF.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgManuTesterIF.h"
#include "afxdialogex.h"


// CDlgManuTesterIF 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgManuTesterIF, CDialogEx)

CDlgManuTesterIF::CDlgManuTesterIF(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgManuTesterIF::IDD, pParent)
	, m_nSelectedAction(0)
{
	m_pDlgTesterStatus = NULL;
	m_nSelectedStationID = -1;
	m_bManualLoopTest = FALSE;

	m_dLoopSotDelay = 0.0;
	m_nLoopTestCount = 1;

	m_nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	
	m_TestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	m_TestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	if (m_nTesterOption == eTester_Single)
		m_nMaxTesterCount = MAX_TESTER_SINGLE;
	else if (m_nTesterOption == eTester_Dual)
		m_nMaxTesterCount = MAX_TESTER_DUAL;
	else	// each DUT comm
	{
		m_nMaxTesterCount = MAX_TESTER_SINGLE *(m_TestSite_Div_X*m_TestSite_Div_Y);//MAX_TESTER_EACH_SOCKET;
	}
	if (m_nTesterOption == eTester_EachSocket)
	{
		m_pSpreadTesterInfo = &m_spreadTesterInfo;
	}
	else
	{
		m_pSpreadTesterInfo = &m_spreadTesterInfoSingle;		
	}
}

CDlgManuTesterIF::~CDlgManuTesterIF()
{
}

void CDlgManuTesterIF::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_ACTION_START_TEST, m_nSelectedAction);
	//DDX_Control( pDX, IDC_SPREAD_TESTER_SELECT, m_spreadTesterSelect );
	DDX_Control(pDX, IDC_SPREAD_TESTER_SELECT_DN, m_spreadTesterSelectDn);
	DDX_Control(pDX, IDC_SPREAD_TESTER_SELECT_UP, m_spreadTesterSelectUp);
	DDX_Control(pDX, IDC_SPREAD_TESTER_INFO, m_spreadTesterInfo);
	DDX_Control(pDX, IDC_SPREAD_TESTER_INFO_SINGLE, m_spreadTesterInfoSingle);
	

	DDX_Control(pDX, IDC_SPREAD_TESTER_2DID_FRONT, m_spreadTester2DID_Front);
	DDX_Control(pDX, IDC_SPREAD_TESTER_2DID_REAR, m_spreadTester2DID_Rear);
	DDX_Control(pDX, IDC_COMBO_TESTER_SET_TEST_MODE, m_combo_SetTestMode);
	DDX_Control(pDX, IDC_COMBO_TESTER_SET_TEST_MODE_ALL_STATION, m_combo_All_Station_TestMode);
	DDX_Control(pDX, IDC_EDIT_TEST_CNT, m_editLoopTestCount);
	DDX_Text(pDX, IDC_EDIT_TEST_CNT, m_nLoopTestCount);
	DDX_Control(pDX, IDC_EDIT_SOT_DELAY, m_editLoopSotDelay);
	DDX_Text(pDX, IDC_EDIT_SOT_DELAY, m_dLoopSotDelay);
	DDX_Control(pDX, IDC_SPREAD_COMM_DISPLAY_STN_1, m_pSpreadTesterCommSts[0]);
	DDX_Control(pDX, IDC_SPREAD_COMM_DISPLAY_STN_2, m_pSpreadTesterCommSts[1]);
	DDX_Control(pDX, IDC_SPREAD_COMM_DISPLAY_STN_3, m_pSpreadTesterCommSts[2]);
	DDX_Control(pDX, IDC_SPREAD_COMM_DISPLAY_STN_4, m_pSpreadTesterCommSts[3]);
	DDX_Control(pDX, IDC_SPREAD_COMM_DISPLAY_STN_5, m_pSpreadTesterCommSts[4]);
	DDX_Control(pDX, IDC_SPREAD_COMM_DISPLAY_STN_6, m_pSpreadTesterCommSts[5]);
	DDX_Control(pDX, IDC_SPREAD_COMM_DISPLAY_STN_7, m_pSpreadTesterCommSts[6]);
	DDX_Control(pDX, IDC_SPREAD_COMM_DISPLAY_STN_8, m_pSpreadTesterCommSts[7]);
}


BEGIN_MESSAGE_MAP(CDlgManuTesterIF, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_ACTION_SEND, &CDlgManuTesterIF::OnBnClickedButtonActionSend)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_SEND_All_STATION, &CDlgManuTesterIF::OnBnClickedButtonActionSendAllStation)
	ON_BN_CLICKED(IDC_BUTTON_MANU_STRAT_ACTION, &CDlgManuTesterIF::OnBnClickedButtonManuStratAction)
	ON_BN_CLICKED(IDC_BUTTON_MANU_STOP_ACTION, &CDlgManuTesterIF::OnBnClickedButtonManuStopAction)
	ON_EN_SETFOCUS(IDC_EDIT_TEST_CNT, &CDlgManuTesterIF::OnEnSetfocusEditTestCnt)
	ON_EN_SETFOCUS(IDC_EDIT_SOT_DELAY, &CDlgManuTesterIF::OnEnSetfocusEditSotDelay)
	ON_BN_CLICKED(IDOK, &CDlgManuTesterIF::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SIMUL_TESTER_COPY_FILE, &CDlgManuTesterIF::OnBnClickedButtonSimulTesterCopyFile)
	ON_BN_CLICKED(IDC_BUTTON_SIMUL_TESTER_KILL, &CDlgManuTesterIF::OnBnClickedButtonSimulTesterKill)
	ON_BN_CLICKED(IDC_BUTTON_SIMUL_TESTER_EXECUTE, &CDlgManuTesterIF::OnBnClickedButtonSimulTesterExecute)
END_MESSAGE_MAP()


// CDlgManuTesterIF 메시지 처리기입니다.

BOOL CDlgManuTesterIF::PreTranslateMessage(MSG* pMsg)
{
	if((pMsg->message==WM_SYSKEYDOWN) && (pMsg->wParam==VK_F4) )
	{
		return TRUE;
	}
	if((pMsg->message==WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		return TRUE;
	}
	if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE) )
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgManuTesterIF::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	g_DMCont.m_dmEQP.SB(BDM0_OPEN_SLT_UI_DLG_HISTORY, TRUE);

	if(m_nTesterOption == eTester_Dual)
		UserInterFaceInit();

	if (m_TestSite_Div_Y == 1)
	{
		GetDlgItem(IDC_SPREAD_TESTER_2DID_FRONT)->ShowWindow(SW_HIDE);
	}

	InitWndTesterStatus();
	Init_Combobox_SetTestMode();

	m_editLoopTestCount.SetBackColor(COLOR_WWHITE);
	m_editLoopTestCount.SetTextSize(30);

	m_editLoopSotDelay.SetBackColor(COLOR_WWHITE);
	m_editLoopSotDelay.SetTextSize(30);
	//GetDlgItem(IDC_BUTTON_MANU_STOP_ACTION)->ShowWindow(SW_HIDE);
	m_pSpreadTesterInfo->SetMaxRows(m_nMaxTesterCount);
	CString strLocation = _T("");
	for (int i = 0; i < m_nMaxTesterCount; i++) {
		strLocation.Format("Location%d", i + 1);
		m_pSpreadTesterInfo->SetTextMatrix(i + 1, 0, strLocation);
	}

	GetDlgItem(IDC_SPREAD_TESTER_SELECT_UP)->ShowWindow(SW_HIDE);


	for (int i = 0; i < MAX_TESTER_SINGLE; i++)
	{
		m_pSpreadTesterCommSts[i].SetMaxCols(m_TestSite_Div_X*m_TestSite_Div_Y);
	}

	DisplayUI();

	//m_pSpreadTesterInfo->SetAutoSize(TRUE);

	UpdateData(FALSE);

	SetTimer(1, 1000, NULL);

	MakeLog("Communication Test Dialog Show");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgManuTesterIF::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(1);

	g_DMCont.m_dmEQP.SB(BDM0_OPEN_SLT_UI_DLG_HISTORY, FALSE);

	if( m_pDlgTesterStatus ){
		m_pDlgTesterStatus->DestroyWindow();
		delete m_pDlgTesterStatus;
		m_pDlgTesterStatus = NULL;
	}
}


void CDlgManuTesterIF::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlgManuTesterIF::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case 1:
		{
			KillTimer(1);
			UpdateSelectedTester();
			int nTesterSelectedStationID = 0;
			int nSelectId = 0;

			if (m_nTesterOption == eTester_Single){
				nSelectId = m_nSelectedStationID;
			}
			else{
				nSelectId = nTesterSelectedStationID = (m_nSelectedStationID - 1) / 2;
			}
			

			if (m_bManualLoopTest == TRUE)
			{				
				if (nSelectId != -1)
				{
					CDSSPanel*    pSensor = NULL;
					pSensor = (CDSSPanel*)GetDlgItem(IDC_AXIS_CMD_BUSY);
					BOOL bFlag = FALSE;

					if (m_nTesterOption == eTester_Single){
						if (g_TaskPressUnit[m_nSelectedStationID - 1].GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready)
							bFlag = TRUE;
					}
					else if (m_nTesterOption == eTester_Dual) {
						if (g_TaskPressUnit[nTesterSelectedStationID].GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready)
							bFlag = TRUE;
					}
					else
					{
						if (g_TaskPressUnit[m_nSelectedStationID - 1].GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready)
							bFlag = TRUE;
					}

					if (bFlag) {
						pSensor->On();
					}
					else {
						pSensor->Off();
						OnBnClickedButtonManuStopAction(); // alarm 발생으로 멈춤
					}
				}				
			}
			else {
				CDSSPanel*    pSensor = NULL;
				pSensor = (CDSSPanel*)GetDlgItem(IDC_AXIS_CMD_BUSY);
				pSensor->Off();
			}


			SetTimer(1,1000,NULL);
		}break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CDlgManuTesterIF::OnBnClickedButtonActionSend()
{
	UpdateData(TRUE);
	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	if( m_nSelectedStationID != -1 ){

		if (m_nSelectedAction + 1 == ACTION_SET_TEST_MODE)
			g_TaskSystemCtrl.m_csHeartBeat.Lock();

		WPARAM wParam = 0;
		LPARAM lParam = 0;
		int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
		int TestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
		int TestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

		ST_TESTER_ACTION_PARAM stActionParam[ STATION_MAX_PARA ];
		int  pSocketOnOff[STATION_MAX_PARA] = {0,};
		CTesterStatus cTesterStatus;

		cTesterStatus = g_AppleTestIF.GetTesterStatus( m_nSelectedStationID, nTesterOption);
	//	int nArrDutOnOff[STATION_MAX_PARA] = { 0, };
		switch( m_nSelectedAction + 1 ) {
		case ACTION_START:
			{
				for( int nY = 0; nY < TestSite_Div_Y; nY++ ) {
					for( int nX = 0; nX < TestSite_Div_X; nX++ ) {
													
						//int nSiteOnOff = g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + (STATION_MAX_PARA*(m_nSelectedStationID - 1)) + nX + nY*STATION_MAX_X_SITE);
						//stActionParam[ ( TestSite_Div_X * nY ) + nX ].m_bSiteOn = nSiteOnOff == DEF_ON;
						stActionParam[(STATION_MAX_X_SITE * nY) + nX].m_bSiteOn = false;
						CString str2DID = _T("");
						if (nY == 0)
							str2DID = m_spreadTester2DID_Rear.GetTextMatrix(1, nX + 1);
						else
							str2DID = m_spreadTester2DID_Front.GetTextMatrix(1, nX + 1);

						str2DID.Replace(" ", "");

						//if( nSiteOnOff == DEF_ON )
						if(str2DID.IsEmpty() == false)
						{
							pSocketOnOff[(TestSite_Div_X * nY) + nX] = 1;
							stActionParam[(STATION_MAX_X_SITE * nY) + nX].m_bSiteOn = true;
							GetDlgItemText( IDC_EDIT_ACTION_PARAM_LOT_NUM, stActionParam[ ( TestSite_Div_X * nY ) + nX ].m_cLotNum, sizeof( stActionParam[ ( TestSite_Div_X * nY ) + nX ].m_cLotNum ) );

							int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
							if (nTesterOption == eTester_EachSocket)
							{
								sprintf(stActionParam[(TestSite_Div_X * nY) + nX].m_c2DID, "%s", str2DID.GetString());
							}
							else
							{
								sprintf(stActionParam[(TestSite_Div_X * nY) + nX].m_cTesterID, "%s", cTesterStatus.m_sTesterID_Orginal.GetString());
								sprintf(stActionParam[(TestSite_Div_X * nY) + nX].m_cSocketID, "%s", cTesterStatus.m_sSocketID_Orginal[(TestSite_Div_X * nY) + nX].GetString());
								sprintf(stActionParam[(TestSite_Div_X * nY) + nX].m_c2DID, "%s", str2DID.GetString());
							}

//  #if DEF_TESTER_COMM_SPEC_ONE_SOCKET == 0
//  							//sprintf_s( stActionParam[ i ].m_cTesterID, "%s", cTesterStatus.m_sTesterID.GetString() );
//  							sprintf( stActionParam[ ( TestSite_Div_X * nY ) + nX ].m_cTesterID, "%s", cTesterStatus.m_sTesterID_Orginal.GetString() );
//  							sprintf( stActionParam[ ( TestSite_Div_X * nY ) + nX ].m_cSocketID, "%s", cTesterStatus.m_sSocketID_Orginal[ ( TestSite_Div_X * nY ) + nX ].GetString() );
//  #endif
//  							//CString str2DID =  m_spreadTester2DID.GetTextMatrix( 1 + nY, nX + 1 );
//  
//  							sprintf( stActionParam[ ( TestSite_Div_X * nY ) + nX ].m_c2DID, "%s", str2DID.GetString() );
														
						}
					}
				}
			}break;
		case ACTION_TEST_SCRIPT_INFO:
		{

			for (int nY = 0; nY < TestSite_Div_Y; nY++) {
				for (int nX = 0; nX < TestSite_Div_X; nX++) {

					//int nSiteOnOff = g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + (STATION_MAX_PARA*(m_nSelectedStationID - 1)) + nX + nY*STATION_MAX_X_SITE);
					//stActionParam[ ( TestSite_Div_X * nY ) + nX ].m_bSiteOn = nSiteOnOff == DEF_ON;
					stActionParam[(STATION_MAX_X_SITE * nY) + nX].m_bSiteOn = false;
					CString str2DID = _T("");
					if (nY == 0)
						str2DID = m_spreadTester2DID_Rear.GetTextMatrix(1, nX + 1);
					else
						str2DID = m_spreadTester2DID_Front.GetTextMatrix(1, nX + 1);

					str2DID.Replace(" ", "");

					//if( nSiteOnOff == DEF_ON )
					if (str2DID.IsEmpty() == false)
					{
						pSocketOnOff[(TestSite_Div_X * nY) + nX] = 1;
					}
				}
			}
		}break;
		case ACTION_SET_TEST_MODE :	// TWSL421에는 해당 커맨드 없음.
			{
				lParam = m_combo_SetTestMode.GetCurSel();
			}break;
		}

		wParam = (WPARAM)&stActionParam;
		
		g_AppleTestIF.SendAction(m_nSelectedStationID, m_nSelectedAction + 1, wParam, lParam, pSocketOnOff, nTesterOption);
	
		if (m_nSelectedAction + 1 == ACTION_SET_TEST_MODE)
			g_TaskSystemCtrl.m_csHeartBeat.Unlock();
	}

//	m_pDlgTesterStatus->ClearStatus();

	MakeLog("Station ID : %d  Action : %s Send", m_nSelectedStationID, GetActionName(m_nSelectedAction));
}

BEGIN_EVENTSINK_MAP(CDlgManuTesterIF, CDialogEx)
	ON_EVENT(CDlgManuTesterIF, IDC_SPREAD_TESTER_SELECT_UP, 5, CDlgManuTesterIF::ClickSpreadTesterSelectUp, VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuTesterIF, IDC_SPREAD_TESTER_SELECT_DN, 5, CDlgManuTesterIF::ClickSpreadTesterSelectDn, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

void CDlgManuTesterIF::ClickSpreadTesterSelectUp(long Col, long Row)
{
	if (Row <= 0 || m_bManualLoopTest == TRUE)
		return;

	int stationID = Col + m_nMaxTesterCount / 2; //Col + ePressUnit_Up_1;//((Row - 1 + 8) % 9) + Col;

	if (!g_AppleTestIF.IsConnected(stationID)) {
		return;
	}

	m_nSelectedStationID = stationID;
	if (m_pDlgTesterStatus) {
		m_pDlgTesterStatus->m_nSelectedStationID = m_nSelectedStationID;
		CTesterStatus cTesterStatus = g_AppleTestIF.GetTesterStatus(m_nSelectedStationID);
	}
}

void CDlgManuTesterIF::ClickSpreadTesterSelectDn(long Col, long Row)
{
	if (Row <= 0 || m_bManualLoopTest == TRUE)
		return;

	int stationID = Col;//((Row-1+8)%9)+Col;

	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	if( !g_AppleTestIF.IsConnected(stationID, nTesterOption) ){
		return;
	}
	
	m_nSelectedStationID = stationID;
	if( m_pDlgTesterStatus ){
		m_pDlgTesterStatus->m_nSelectedStationID = m_nSelectedStationID;
		CTesterStatus cTesterStatus = g_AppleTestIF.GetTesterStatus(m_nSelectedStationID, nTesterOption);
	}
}


void CDlgManuTesterIF::UpdateSelectedTester()
{
	CString strStationId = _T("");
	COLORREF color;
	int stationID = -1;

//#if DEF_TESTER_COMM_SPEC_ONE_SOCKET
	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	if (nTesterOption == eTester_EachSocket)
	{

		int nDutCnt = 0;

		for (int c = 0; c < MAX_TESTER_SINGLE; c++) {
			strStationId.Empty();
			m_spreadTesterSelectDn.SetRow(1);
			m_spreadTesterSelectDn.SetCol(c + 1); // 각 station 상태 확인 한다.

			int nConnectOK = 0;
			for (nDutCnt = 0; nDutCnt < m_TestSite_Div_X*m_TestSite_Div_Y; nDutCnt++)
			{
				stationID = c * (m_TestSite_Div_X*m_TestSite_Div_Y) + nDutCnt + 1;

				if (g_AppleTestIF.IsConnected(stationID)) {
					nConnectOK++;
				}
			}

			if (nConnectOK == m_TestSite_Div_X*m_TestSite_Div_Y)
			{
				stationID = c + 1;
				strStationId.Format("%d", stationID);
				color = COLOR_GREEN;	// success all comm dut 
			}
			else if (nConnectOK > 0)
			{
				stationID = c + 1;
				strStationId.Format("%d", stationID);
				color = COLOR_YELLOW;	// success part comm dut 
			}
			else
			{
				color = COLOR_RED;		// fail all comm dut 
				strStationId = "N.C";
			}

			m_spreadTesterSelectDn.SetBackColor(color);
			m_spreadTesterSelectDn.SetTextMatrix(1, c + 1, strStationId);
		}

		// connect 상태 표시
		for (int c = 0; c < m_nMaxTesterCount; c++) {
			strStationId.Empty();
			//m_spreadTesterSelect.SetRow(r+1);
			//m_spreadTesterSelect.SetCol(c+1);	
			int nArray = c / (m_TestSite_Div_X*m_TestSite_Div_Y);

			m_pSpreadTesterCommSts[nArray].SetRow(1);
			m_pSpreadTesterCommSts[nArray].SetCol(c % (m_TestSite_Div_X*m_TestSite_Div_Y) + 1);

			stationID = c + 1;
			if (g_AppleTestIF.IsConnected(stationID)) {
				color = COLOR_GREEN;
				strStationId.Format("%d", stationID);
			}
			else {
				color = COLOR_WWHITE;
				strStationId = "N.C";
			}

			m_pSpreadTesterCommSts[nArray].SetBackColor(color);
			m_pSpreadTesterCommSts[nArray].SetTextMatrix(1, c % (m_TestSite_Div_X*m_TestSite_Div_Y) + 1, strStationId);

			//// 아래와 같은 배열이라 Index 변환이 필요 하다.
			//// UP   : 9 10 11 12 13 14 15 16
			//// DOWN : 1  2  3  4  5  6  7  9
			//int stationID = (((r+8)%9)+c)+1;
			//
			//if(g_AppleTestIF.IsConnected(stationID)){
			//	color = COLOR_WWHITE;
			//	strStationId.Format("%d", stationID);
			//}else{
			//	color = COLOR_WWHITE;
			//	strStationId = "N.C.";
			//}
			//m_spreadTesterSelect.SetBackColor(color);
			//m_spreadTesterSelect.SetTextMatrix(r+1, c+1, strStationId);
		}
		//}
	}
	else
	{
		//for(int r=0; r<2; ++r){
		for (int c = 0; c < m_nMaxTesterCount; ++c) {
			strStationId.Empty();
			//m_spreadTesterSelect.SetRow(r+1);
			//m_spreadTesterSelect.SetCol(c+1);	

			m_spreadTesterSelectDn.SetRow(1);
			m_spreadTesterSelectDn.SetCol(c + 1);

			stationID = ePressUnit_Down_1 + c + 1;
			if (g_AppleTestIF.IsConnected(stationID)) {
				color = COLOR_GREEN;
				strStationId.Format("%d", stationID);
			}
			else {
				color = COLOR_WWHITE;
				strStationId = "N.C";
			}
			m_spreadTesterSelectDn.SetBackColor(color);
			m_spreadTesterSelectDn.SetTextMatrix(1, c + 1, strStationId);


			m_spreadTesterSelectUp.SetRow(1);
			m_spreadTesterSelectUp.SetCol(c + 1);
			stationID = m_nMaxTesterCount / 2 + c + 1;
			if (g_AppleTestIF.IsConnected(stationID)) {
				color = COLOR_GREEN;
				strStationId.Format("%d", stationID);
			}
			else {
				color = COLOR_WWHITE;
				strStationId = "N.C";
			}
			m_spreadTesterSelectUp.SetBackColor(color);
			m_spreadTesterSelectUp.SetTextMatrix(1, c + 1, strStationId);

			//// 아래와 같은 배열이라 Index 변환이 필요 하다.
			//// UP   : 9 10 11 12 13 14 15 16
			//// DOWN : 1  2  3  4  5  6  7  9
			//int stationID = (((r+8)%9)+c)+1;
			//
			//if(g_AppleTestIF.IsConnected(stationID)){
			//	color = COLOR_WWHITE;
			//	strStationId.Format("%d", stationID);
			//}else{
			//	color = COLOR_WWHITE;
			//	strStationId = "N.C.";
			//}
			//m_spreadTesterSelect.SetBackColor(color);
			//m_spreadTesterSelect.SetTextMatrix(r+1, c+1, strStationId);
		}
	}
//#endif

	// 선택 되어진 tester는 노란색으로 표시 한다.
	//int row,col;
	//int row_cvt[2] = {1, 0};
	if( m_nSelectedStationID != -1 ){
		//row = row_cvt[m_nSelectedStationID / 8];// 0,1 나오는 것을 1,0으로 바꾼다.
		//col = (m_nSelectedStationID % 8)-1;
		//m_spreadTesterSelect.SetRow(row+1);
		//m_spreadTesterSelect.SetCol(col+1);
		//m_spreadTesterSelect.SetBackColor(COLOR_LLIGHTYELLOW);

		int col = (m_nSelectedStationID - 1) % (m_nMaxTesterCount);
		if (m_nSelectedStationID <= (m_nMaxTesterCount))
		{
			m_spreadTesterSelectDn.SetRow(1);
			m_spreadTesterSelectDn.SetCol(col + 1);
			m_spreadTesterSelectDn.SetBackColor(COLOR_LLIGHTYELLOW);
		}
		else {
			m_spreadTesterSelectUp.SetRow(1);
			m_spreadTesterSelectUp.SetCol(col + 1);
			m_spreadTesterSelectUp.SetBackColor(COLOR_LLIGHTYELLOW);
		}

	}

	CString strVer = _T("");
	CString strStationID = _T("");
	for(int i=0; i<m_nMaxTesterCount; i++){
		int nStationID = i + 1;
		if( g_AppleTestIF.IsConnected( nStationID ) == TRUE ){
			CTesterStatus cTesterStatus = g_AppleTestIF.GetTesterStatus( nStationID );
			strStationID.Format("%d", cTesterStatus.m_nStationID);
		
			if (nTesterOption == eTester_EachSocket)
			{
				m_pSpreadTesterInfo->SetTextMatrix(i + 1, 1, strStationID);
				m_pSpreadTesterInfo->SetTextMatrix(i + 1, 2, cTesterStatus.m_sHW_version);
				m_pSpreadTesterInfo->SetTextMatrix(i + 1, 3, cTesterStatus.m_sPPO_FW_version);
				m_pSpreadTesterInfo->SetTextMatrix(i + 1, 4, cTesterStatus.m_sHID_FW_version);
				m_pSpreadTesterInfo->SetTextMatrix(i + 1, 5, cTesterStatus.m_sSW_Version);
				if(nProjectOption == DEF_PROJECT_CARPENTERS)
					m_pSpreadTesterInfo->SetTextMatrix(i + 1, 6, cTesterStatus.m_sTest_Script_Apl);
				else
					m_pSpreadTesterInfo->SetTextMatrix(i + 1, 6, cTesterStatus.m_sTest_Script_Info);
			}
			else {
			
			
				if(nProjectOption == DEF_PROJECT_WATT)
				{
				
					CString strTestMode = _T("");
					strTestMode = g_TaskSystemCtrl.GetTestModeintToString(cTesterStatus.m_nTest_Mode);
					
					m_pSpreadTesterInfo->SetTextMatrix(i + 1, 1, cTesterStatus.m_sTesterID_Orginal);
					m_pSpreadTesterInfo->SetTextMatrix(i + 1, 2, cTesterStatus.m_sTestSWVersion);
					m_pSpreadTesterInfo->SetTextMatrix(i + 1, 3, cTesterStatus.m_sTestScriptInfo);
					m_pSpreadTesterInfo->SetTextMatrix(i + 1, 4, strTestMode);
					m_pSpreadTesterInfo->SetTextMatrix(i + 1, 5, cTesterStatus.m_sIP);
				}
				else {
					m_pSpreadTesterInfo->SetTextMatrix(i + 1, 1, cTesterStatus.m_sTesterID_Orginal);
					m_pSpreadTesterInfo->SetTextMatrix(i + 1, 2, cTesterStatus.m_sSW_Version);
					m_pSpreadTesterInfo->SetTextMatrix(i + 1, 4, cTesterStatus.m_sIP);
				}		
			}
		

			//m_pSpreadTesterInfo->SetTextMatrix(i+1, 6, cTesterStatus.m_sIP);
		}else{
			strVer.Empty();
			if (nTesterOption == eTester_EachSocket)
			{				
				m_pSpreadTesterInfo->SetTextMatrix(i + 1, 1, strVer);
				m_pSpreadTesterInfo->SetTextMatrix(i + 1, 2, strVer);
				m_pSpreadTesterInfo->SetTextMatrix(i + 1, 3, strVer);
				m_pSpreadTesterInfo->SetTextMatrix(i + 1, 4, strVer);
				m_pSpreadTesterInfo->SetTextMatrix(i + 1, 5, strVer);
				m_pSpreadTesterInfo->SetTextMatrix(i + 1, 6, strVer);
			}
			else {
				if (nProjectOption == DEF_PROJECT_WATT)
				{
					m_pSpreadTesterInfo->SetTextMatrix(i + 1, 1, strVer);
					m_pSpreadTesterInfo->SetTextMatrix(i + 1, 2, strVer);
					m_pSpreadTesterInfo->SetTextMatrix(i + 1, 3, strVer);
					m_pSpreadTesterInfo->SetTextMatrix(i + 1, 4, strVer);
					m_pSpreadTesterInfo->SetTextMatrix(i + 1, 5, strVer);
				}
				else {
					m_pSpreadTesterInfo->SetTextMatrix(i + 1, 1, strVer);
					m_pSpreadTesterInfo->SetTextMatrix(i + 1, 2, strVer);
					m_pSpreadTesterInfo->SetTextMatrix(i + 1, 3, strVer);
				}
			}
		}
	}
}

void CDlgManuTesterIF::InitWndTesterStatus()
{
	m_pDlgTesterStatus = new CDlgTesterIF_TesterStatus(this);
	m_pDlgTesterStatus->Create(IDD_TESTER_IF_STATUS, this);

	CRect rectFrame, rectView;
	GetDlgItem(IDC_STATIC_FRAME_TESTER_STATUS)->GetWindowRect(&rectFrame);
	ScreenToClient(&rectFrame);
	m_pDlgTesterStatus->GetClientRect(&rectView);
	ScreenToClient(&rectView);
	int x,y,w,h;
	x = rectFrame.left + ((rectFrame.Width()-rectView.Width())/2);
	y = rectFrame.top + 5;
	w = rectView.Width();
	h = rectFrame.Height();
	m_pDlgTesterStatus->MoveWindow(x,y,w,h);
	m_pDlgTesterStatus->ShowWindow(SW_SHOW);
}

void CDlgManuTesterIF::Init_Combobox_SetTestMode()
{
	if (m_nTesterOption == eTester_EachSocket)
	{
		m_combo_SetTestMode.AddString("0:Normal");
		m_combo_SetTestMode.AddString("1:GRR");
		m_combo_SetTestMode.AddString("2:LoopTest");
		m_combo_SetTestMode.AddString("3:Audit");

		m_combo_All_Station_TestMode.AddString("0:Normal");
		m_combo_All_Station_TestMode.AddString("1:GRR");
		m_combo_All_Station_TestMode.AddString("2:LoopTest");
		m_combo_All_Station_TestMode.AddString("3:Audit");
	}
	else {
		m_combo_SetTestMode.AddString("0:SLT1");
		m_combo_SetTestMode.AddString("1:SLTQA1");

		m_combo_All_Station_TestMode.AddString("0:SLT1");
		m_combo_All_Station_TestMode.AddString("1:SLTQA1");
	}

	m_combo_SetTestMode.SetCurSel( 0 );
	m_combo_All_Station_TestMode.SetCurSel(0);
}

CString CDlgManuTesterIF::GetActionName( int nSelectedAction )
{
	CString strActionName = _T( "" );
	switch( nSelectedAction )
	{
		case 0: { strActionName = _T( "Start Test" ); } break;
		case 1: { strActionName = _T( "Stop Test" ); } break;
		case 2: { strActionName = _T( "Reset " ); } break;
		case 3: { strActionName = _T( "Query tester Status " ); } break;
		case 4: { strActionName = _T( "Query tester information" ); } break;
		case 5: { strActionName = _T( "Self Test" ); } break;
		case 6: { strActionName = _T( "Calibration" ); } break;
		case 7: { strActionName = _T( "Set Test Mode" ); } break;
	}
	return strActionName;
}

void CDlgManuTesterIF::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = {0,};
	if(fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf_s(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_SYSTEM), Debug, "", __LINE__, NULL, tmpMsg);
}



void CDlgManuTesterIF::OnBnClickedButtonActionSendAllStation()
{
	BOOL bFindTestingStation = FALSE;
	for (int Index = 0; Index < m_nMaxTesterCount; Index++) {
		if (g_DMCont.m_dmEQP.GB(BDM0_TEST_STATUS_SITE1 + Index) == TRUE)
			bFindTestingStation = TRUE;
	}
	if (bFindTestingStation == TRUE) {
		int nRet = AfxMessageBox(_T("Station does not end !! you want change?"), MB_TOPMOST | MB_YESNO);
		if (nRet != IDYES)
			return;
	}

	WPARAM wParam = 0;
	LPARAM lParam = 0;

	ST_TESTER_ACTION_PARAM stActionParam[STATION_MAX_PARA];

	g_TaskSystemCtrl.m_csHeartBeat.Lock();
	for (int i = 0; i < m_nMaxTesterCount; i++) {
		int nStationID = i + 1;
		BOOL bConnect = g_AppleTestIF.IsConnected(nStationID);
		if (bConnect) {
			int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

			g_DMCont.m_dmEQP.SB(BDM0_TESTER_CONNECTION_1 + i, FALSE);
			
			wParam = (WPARAM)&stActionParam;
			lParam = m_combo_All_Station_TestMode.GetCurSel();
			g_AppleTestIF.SendAction(nStationID, ACTION_SET_TEST_MODE, wParam, lParam);
			MakeLog("Station ID : %d  Action : %s Send", nStationID, GetActionName(ACTION_SET_TEST_MODE-1));
		}
	}
	g_TaskSystemCtrl.m_csHeartBeat.Unlock();
}


void CDlgManuTesterIF::OnBnClickedButtonManuStratAction()
{
	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
	if (bLotState == eHandlerLotMode_OnGoing) {
		AfxMessageBox("Handler is Running!! Please action after lot end");
		return;
	}

	MakeLog("Manual loop Start Button Click");
	int nTesterSelectedStationID = 0;
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	UpdateData(TRUE);
	if (m_nSelectedStationID != -1) {
		BOOL bFindTestingStation = FALSE;
		for (int Index = 0; Index < eMaxPressUnitCount; Index++) {
			if (g_DMCont.m_dmEQP.GB(BDM0_TEST_STATUS_SITE1 + Index) == TRUE)
				bFindTestingStation = TRUE;
		}
		if (bFindTestingStation == TRUE) {
			int nRet = AfxMessageBox(_T("Station does not end !! you want start action?"), MB_TOPMOST | MB_YESNO);
			if (nRet != IDYES)
			{
				MakeLog("Manual loop Start Button Click - cancel select");
				return;
			}
		}	

		BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_EQP_RUN_STATE);
		if (bLotState == TRUE) {
			CParamList AlarmData;
			g_Error.AlarmReport(ERR_MANUAL_MODE_INTERLOCK_RUNNING, AlarmData, NULL);
			return;
		}

		CBaseTaskManager::SetEqpCtrlMode(eEqpCtrlMode_Manual);


		CPressUnitCmd_OnLineLoopTest* pSeqCmd = new CPressUnitCmd_OnLineLoopTest;
		{
			int TestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
			int TestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

			ST_TESTER_ACTION_PARAM stActionParam[STATION_MAX_PARA];
			CTesterStatus cTesterStatus = g_AppleTestIF.GetTesterStatus(m_nSelectedStationID, nTesterOption);
			 
			for (int nY = 0; nY < TestSite_Div_Y; nY++) {
				for (int nX = 0; nX < TestSite_Div_X; nX++) {
					stActionParam[(STATION_MAX_X_SITE * nY) + nX].m_bSiteOn = false;
					CString str2DID = _T("");
					if (nY == 0)
						str2DID = m_spreadTester2DID_Rear.GetTextMatrix(1, nX + 1);
					else
						str2DID = m_spreadTester2DID_Front.GetTextMatrix(1, nX + 1);

					str2DID.Replace(" ", "");

					//if( nSiteOnOff == DEF_ON )
					if (str2DID.IsEmpty() == false)	// empty tray가 아니면 적용.
					{
						stActionParam[(STATION_MAX_X_SITE * nY) + nX].m_bSiteOn = true;
						GetDlgItemText(IDC_EDIT_ACTION_PARAM_LOT_NUM, stActionParam[(TestSite_Div_X * nY) + nX].m_cLotNum, sizeof(stActionParam[(TestSite_Div_X * nY) + nX].m_cLotNum));
						
						int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
						if (nTesterOption == eTester_EachSocket)
						{
							sprintf(stActionParam[(TestSite_Div_X * nY) + nX].m_c2DID, "%s", str2DID.GetString());
						}
						else
						{
							sprintf(stActionParam[(TestSite_Div_X * nY) + nX].m_cTesterID, "%s", cTesterStatus.m_sTesterID_Orginal.GetString());
							sprintf(stActionParam[(TestSite_Div_X * nY) + nX].m_cSocketID, "%s", cTesterStatus.m_sSocketID_Orginal[(TestSite_Div_X * nY) + nX].GetString());
							sprintf(stActionParam[(TestSite_Div_X * nY) + nX].m_c2DID, "%s", str2DID.GetString());
						}
// #if DEF_TESTER_COMM_SPEC_ONE_SOCKET == 0
// 						sprintf(stActionParam[(TestSite_Div_X * nY) + nX].m_cTesterID, "%s", cTesterStatus.m_sTesterID_Orginal.GetString());
// 						sprintf(stActionParam[(TestSite_Div_X * nY) + nX].m_cSocketID, "%s", cTesterStatus.m_sSocketID_Orginal[(TestSite_Div_X * nY) + nX].GetString());
// #endif
// 						sprintf(stActionParam[(TestSite_Div_X * nY) + nX].m_c2DID, "%s", str2DID.GetString());
					}
				}
			}

			pSeqCmd->m_nLoopCount = m_nLoopTestCount;

			BOOL bSocketDataOK[STATION_MAX_PARA] = { 0, };
			for (int i = 0; i < STATION_MAX_PARA; i++)
			{
				pSeqCmd->m_stActionParam[i] = stActionParam[i];

				int nStrLenth = strlen(stActionParam[i].m_c2DID);

				if (nStrLenth > 0)
				{
					bSocketDataOK[i] = TRUE;
				}			
			}

			pSeqCmd->m_tcSotDelayTime = m_dLoopSotDelay*1000.0;
			
			pSeqCmd->m_nTesterIndex = m_nSelectedStationID - 1;
	
			memcpy(pSeqCmd->m_bDutUseNotUse, bSocketDataOK, sizeof(BOOL) * STATION_MAX_PARA);
			
			if(nTesterOption == eTester_Dual)
			{
				nTesterSelectedStationID = (m_nSelectedStationID - 1 )/ 2 ;
			}
			else if (nTesterOption == eTester_EachSocket)
			{
				nTesterSelectedStationID = m_nSelectedStationID;
			}
		}

		int nErr = 0;
		if (nTesterOption == eTester_Single)
		{
			nErr = g_TaskSystemCtrl.SendCommand(g_TaskPressUnit[m_nSelectedStationID - 1], pSeqCmd, eEqpCtrlMode_Manual);
		}
		else if(nTesterOption == eTester_Dual) {
			nErr = g_TaskSystemCtrl.SendCommand(g_TaskPressUnit[nTesterSelectedStationID ], pSeqCmd, eEqpCtrlMode_Manual);
			}
		else // 64 tester socket each comme (nTesterOption == eTester_EachSocket) {
		{
			nErr = g_TaskSystemCtrl.SendCommand(g_TaskPressUnit[nTesterSelectedStationID-1], pSeqCmd, eEqpCtrlMode_Manual);
		}

		if (nErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			g_Error.AlarmReport(nErr, AlarmData, NULL);
		}
		else {
			GetDlgItem(IDC_BUTTON_ACTION_SEND)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ACTION_SEND_All_STATION)->EnableWindow(FALSE);
			GetDlgItem(IDOK)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_MANU_STRAT_ACTION)->EnableWindow(FALSE);

			m_bManualLoopTest = TRUE;
			g_DMCont.m_dmHandlerOpt.SB(BDM11_GUI_OPEN_HISTORY, TRUE);

			MakeLog("PressUnit %d Manual Loop Test=%d", m_nSelectedStationID - 1, m_nLoopTestCount);
		}
	}	
}


void CDlgManuTesterIF::OnBnClickedButtonManuStopAction()
{
	MakeLog("Manual loop Stop Button Click");

	CBaseTaskManager::SendProcessCommand(base_seq_cmd::eProcCmd_Skip, eEqpCtrlMode_Manual, NULL);

	m_bManualLoopTest = FALSE;
	g_DMCont.m_dmHandlerOpt.SB(BDM11_GUI_OPEN_HISTORY, FALSE);	

	GetDlgItem(IDC_BUTTON_ACTION_SEND)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_ACTION_SEND_All_STATION)->EnableWindow(TRUE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_MANU_STRAT_ACTION)->EnableWindow(TRUE);
}

void CDlgManuTesterIF::ThreadProc1()
{
	SetThreadInterval(0, 50);
	while (GetThreadAliveFlag(0)) {
		if (GetPauseFlag(0)) {
			NEXT;
			continue;
		}
		//for (int i = 0; i<MAX_THREAD_SCAN_COUNT; i++) {
			CBaseTaskManager::SeqCmdRunning();
		//}
		NEXT_10;
	}
}

void CDlgManuTesterIF::OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{

}

int CDlgManuTesterIF::OnCommand(CBaseSeqCmd* pSeqCmd, int nEqpCtrlCmdType/*=eEqpCtrlMode_Auto*/)
{
	int nChkCmd = 0;
	if (pSeqCmd == NULL)
	{
		return eSeqCmdAck_InvalidCmd;
	}

	int nChkParam = pSeqCmd->CheckParam();
	if (nChkParam > 0) {
		delete pSeqCmd;
		pSeqCmd = NULL;
		return eSeqCmdAck_InvalidParam;
	}

	CBaseTaskManager::AddSeqCmd(pSeqCmd, nEqpCtrlCmdType);
	return eSeqCmdAck_OK;
}

int CDlgManuTesterIF::OnEvent(const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList/*=NULL*/)
{
	return 0;
}

int CDlgManuTesterIF::OnCommandRsp(CBaseSeqCmd* pRespCmd, int nEventId, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CDlgManuTesterIF::OnEnSetfocusEditTestCnt()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nLoopTestCount, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "LoopTest Count", "100", "1");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_TEST_CNT)->SetWindowText(szRetVal);
	}
}


void CDlgManuTesterIF::OnEnSetfocusEditSotDelay()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dLoopSotDelay);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "SOT Delay Time", "10", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SOT_DELAY)->SetWindowText(szRetVal);
	}
}

void CDlgManuTesterIF::UserInterFaceInit()
{
	CString strStationId = _T("");
	int stationID = 0;
	int nMaxTesterCount = 0;
	int nStationID = 0;
	m_nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);

	GetDlgItem(IDC_SPREAD_TESTER_2DID_FRONT)->ShowWindow(SW_HIDE);

	if (m_nTesterOption == eTester_Single)
	{
		nMaxTesterCount = MAX_TESTER_SINGLE;
		
	}		
	else
	{
		m_pSpreadTesterInfo->SetColWidth(0, 10);
		nMaxTesterCount = MAX_TESTER_DUAL;
	}
	
	m_pSpreadTesterInfo->SetMaxRows(m_nMaxTesterCount);
	m_spreadTesterSelectUp.SetMaxCols(m_nMaxTesterCount / 2);
	m_spreadTesterSelectDn.SetMaxCols(m_nMaxTesterCount / 2);
	
	

	for (int i = 1; i <= nMaxTesterCount /2; i++)
	{
		m_spreadTesterSelectUp.SetColWidth(i, 4.42);
		m_spreadTesterSelectDn.SetColWidth(i, 4.42);	

		if (m_nTesterOption == eTester_Single)
		{
			stationID = (m_nMaxTesterCount / 2) + i;
			strStationId.Format("%d", stationID);
			m_spreadTesterSelectUp.SetTextMatrix(0, i, strStationId);

			stationID = ePressUnit_Down_1 + i;
			strStationId.Format("%d", stationID);
			m_spreadTesterSelectDn.SetTextMatrix(0, i, strStationId);
		}
		else
		{
			stationID = MAX_TESTER_DUAL/2 + nStationID;
			strStationId.Format("%d-%d", (stationID / 2) + 1, stationID % 2 + 1);
			m_spreadTesterSelectUp.SetTextMatrix(0, i, strStationId);

			stationID = ePressUnit_Down_1 + nStationID;
			strStationId.Format("%d-%d", (stationID/2)+1, stationID % 2 + 1);
			m_spreadTesterSelectDn.SetTextMatrix(0, i, strStationId);

		}
		nStationID++;
	}	
	nStationID = 0;
	for (int c = 1; c <= m_nMaxTesterCount; c++)
	{
		if (m_nTesterOption == eTester_Single)
		{
			strStationId.Format("Tester%d", c);
			m_pSpreadTesterInfo->SetTextMatrix(c, 0, strStationId);	
		}
		else
		{
			m_pSpreadTesterInfo->SetRowHeight(c, 15.3);
			strStationId.Format("Tester%d-%d", nStationID /2+1, nStationID % 2+1);
			m_pSpreadTesterInfo->SetTextMatrix(c, 0, strStationId);
			
		}
		nStationID++;
	}
}

void CDlgManuTesterIF::OnBnClickedOk()
{
	CDialogEx::OnOK();
}


void CDlgManuTesterIF::OnBnClickedButtonSimulTesterCopyFile()
{
	
	/* Copy File
	std::vector<CString> veFileName;
	std::vector<CString> veFileTargetPath;

	CString strSourcePath = _T("D:\\Git\\TWSL421\\SLT_Handler\\SLT_TESTER_SIM\\ApTester\\Debug\\");

	veFileName.push_back("ApTester.exe");
	veFileName.push_back("ApTester.ilk");
	veFileName.push_back("ApTester.pdb");

	veFileName.push_back("iocp_socket_library.dll");
	veFileName.push_back("iocp_socket_library.exp");
	veFileName.push_back("iocp_socket_library.ilk");
	veFileName.push_back("iocp_socket_library.lib");
	veFileName.push_back("iocp_socket_library.pdb");


	for (int nTester = 1; nTester <= 16; nTester++)
	{
		CString str;
		str.Format("D:\\Git\\TWSL421\\SLT_Handler\\SLT_TESTER_SIM\\ApTester\\Debug%d\\", nTester);
		veFileTargetPath.push_back(str);
	}

	for (int nTester = 0; nTester < (int)veFileTargetPath.size(); nTester++)
	{
		for (int nCount = 0; nCount < (int)veFileName.size(); nCount++)
		{
			CString strPath, strTargetPath;
			strPath.Format("%s%s", strSourcePath, veFileName[nCount]);
			strTargetPath.Format("%s%s", veFileTargetPath[nTester], veFileName[nCount]);
			BOOL bErr = CopyFile(strPath, strTargetPath, FALSE);
			if (bErr == ERR_NO_ERROR)
			{

			}
		}
	}
	*/

	std::vector<CString> veFileTargetPath;

// 
// 	CString strSourcePathTest = _T("D:\\Git\\TWSL421\\SLT_Handler\\SLT_TESTER_SIM\\ApTester\\Debug2\\Ap.exe");
// 	CString str, str1, str2;
// 	str = strSourcePathTest.Left(strSourcePathTest.ReverseFind('\\'));
// 	str1 = str.Right(str.GetLength() - str.ReverseFind('\\')-1);
// 	str2 = str1.Mid(5,2);
 	CString strSourcePath = _T("D:\\Git\\TWSL421\\SLT_Handler\\SLT_TESTER_SIM\\ApTester\\Debug\\");

	for (int nTester = 1; nTester <= MAX_TESTER_EACH_SOCKET; nTester++)
	{
		CString str;
		str.Format("D:\\Git\\TWSL421\\SLT_Handler\\SLT_TESTER_SIM\\ApTester\\Debug%d\\", nTester);
		veFileTargetPath.push_back(str);
	}

	for (int nTester = 0; nTester < (int)veFileTargetPath.size(); nTester++)
	{
		CString strTarget;
		strTarget.Format("%s", veFileTargetPath[nTester]);
		CopyFolder(strSourcePath, strTarget);
	}


}


void CDlgManuTesterIF::OnBnClickedButtonSimulTesterKill()
{
	BOOL bExit = TRUE;
	while (bExit)
	{
		HWND hOsk = ::FindWindow(NULL, "Untitled - ApTester");
		if (hOsk)
		{
			CWnd* p = CWnd::FromHandle(hOsk);
			CString st;
			p->GetWindowTextA(st);
			DWORD pid2;
			DWORD pid = GetWindowThreadProcessId(hOsk, &pid2);

			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid2);

			if (hProcess)
			{
				if (TerminateProcess(hProcess, 0))
				{
					unsigned long nCode;
					GetExitCodeProcess(hProcess, &nCode);
				}
				CloseHandle(hProcess);
			}
			Sleep(20);
		}
		else
		{
			bExit = FALSE;
		}		
	}
}

int CDlgManuTesterIF::CopyFolder(CString szFrom, CString szTo)
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	BOOL bResult = TRUE;
	TCHAR WildCard[MAX_PATH];
	TCHAR SrcFile[MAX_PATH];
	TCHAR DestFile[MAX_PATH];

	wsprintf(WildCard, "%s\\*.*", szFrom);
	CreateDirectory(szTo, NULL);
	hSrch = FindFirstFile(WildCard, &wfd);
	if (hSrch == INVALID_HANDLE_VALUE)
		return FALSE;
	while (bResult) {
		wsprintf(SrcFile, "%s\\%s", szFrom, wfd.cFileName);
		wsprintf(DestFile, "%s\\%s", szTo, wfd.cFileName);
		// 서브 디렉토리가 발견되면 서브 디렉토리를 복사한다.
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (lstrcmp(wfd.cFileName, ".") && lstrcmp(wfd.cFileName, "..")) {
				CopyFolder(SrcFile, DestFile);
			}
		}
		else {
			CopyFile(SrcFile, DestFile, FALSE);
		}
		bResult = FindNextFile(hSrch, &wfd);
	}
	FindClose(hSrch);
	return TRUE;
}


void CDlgManuTesterIF::OnBnClickedButtonSimulTesterExecute()
{
	for (int nTester = 1; nTester <= MAX_TESTER_EACH_SOCKET; nTester++)
	{
		CString str;
		str.Format("D:\\Git\\TWSL421\\SLT_Handler\\SLT_TESTER_SIM\\ApTester\\Debug%d\\ApTester.exe", nTester);
		int nErr = (int)ShellExecute(NULL, "open", str, NULL, NULL, SW_SHOWNORMAL);
	}
}


void CDlgManuTesterIF::DisplayUI()
{
	if (m_nTesterOption != eTester_EachSocket)
	{
		GetDlgItem(IDC_SPREAD_COMM_DISPLAY_STN_1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_COMM_DISPLAY_STN_2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_COMM_DISPLAY_STN_3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_COMM_DISPLAY_STN_4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_COMM_DISPLAY_STN_5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_COMM_DISPLAY_STN_6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_COMM_DISPLAY_STN_7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SPREAD_COMM_DISPLAY_STN_8)->ShowWindow(SW_HIDE);


		GetDlgItem(IDC_TESTER_STATUS_STATION1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TESTER_STATUS_STATION2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TESTER_STATUS_STATION3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TESTER_STATUS_STATION4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TESTER_STATUS_STATION5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TESTER_STATUS_STATION6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TESTER_STATUS_STATION7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TESTER_STATUS_STATION8)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_SPREAD_TESTER_INFO)->ShowWindow(SW_HIDE);

		int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
		if (nProjectOption == DEF_PROJECT_WATT)
		{
			m_pSpreadTesterInfo->SetMaxCols(5);
			m_pSpreadTesterInfo->SetTextMatrix(0, 1, "Tester ID");
			m_pSpreadTesterInfo->SetTextMatrix(0, 2, "SW Version");
			m_pSpreadTesterInfo->SetTextMatrix(0, 3, "Test Script");
			m_pSpreadTesterInfo->SetTextMatrix(0, 4, "Test Mode");
			m_pSpreadTesterInfo->SetTextMatrix(0, 5, "IP");
		}
		else{
			m_pSpreadTesterInfo->SetMaxCols(3);
			m_pSpreadTesterInfo->SetTextMatrix(0, 1, "Tester ID");
			m_pSpreadTesterInfo->SetTextMatrix(0, 2, "SW Version");
			m_pSpreadTesterInfo->SetTextMatrix(0, 3, "IP");
		}
	}
	else
	{
		GetDlgItem(IDC_SPREAD_COMM_DISPLAY_STN_1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPREAD_COMM_DISPLAY_STN_2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPREAD_COMM_DISPLAY_STN_3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPREAD_COMM_DISPLAY_STN_4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPREAD_COMM_DISPLAY_STN_5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPREAD_COMM_DISPLAY_STN_6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPREAD_COMM_DISPLAY_STN_7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SPREAD_COMM_DISPLAY_STN_8)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_SPREAD_TESTER_INFO_SINGLE)->ShowWindow(SW_HIDE);
	}

}