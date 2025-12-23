// AutoGrrSetDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "AutoGrrSetDlg.h"
#include "afxdialogex.h"


// CAutoGrrSetDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAutoGrrSetDlg, CDialog)

CAutoGrrSetDlg::CAutoGrrSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoGrrSetDlg::IDD, pParent)
{
	ZeroMemory(m_nGrrSiteSocketOnOff, sizeof(m_nGrrSiteSocketOnOff));
	m_nAutoGrrTestCount = 0;
	m_nAutoGrrUnitCount = 0;
	m_dGrrSotDelay = 0.0;

	/*m_bGRR_Device_swap = FALSE;*///
	m_bGRR_Device_swap = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrDeviceSwapUse);
	m_bGRR_Audit_loop = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrAuditLoopUse);

	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	
	//[12/11/2023 dohyeong.kim]현재 기능 사용 안함
	m_bGRR_Audit_loop = FALSE;
	



	m_nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	m_Station_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	m_Station_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

// 	if (m_bGRR_Audit_loop == 1)
// 		m_Station_Div_Y = 1;

	m_nAutoGrrmode = 0;

	m_bInitSpread = FALSE;

}

CAutoGrrSetDlg::~CAutoGrrSetDlg()
{
}

void CAutoGrrSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_NUMBER_OF_UNITS, m_editUnitCount);
	DDX_Control(pDX, IDC_EDIT_NUMBER_OF_TIMES, m_editTestCount);
	DDX_Control(pDX, IDC_SPREAD_GRR, m_SprdGrrSite);
	DDX_Radio(pDX, IDC_RADIO_NORMAL_MODE, m_nAutoGrrmode);
	DDX_Control(pDX, IDC_EDIT_GRR_SOT_DELAY_TIME, m_editGrrSotDelay);
	
	DDX_Text(pDX, IDC_EDIT_GRR_SOT_DELAY_TIME, m_dGrrSotDelay);

	DDX_Control(pDX, IDC_SPREAD_GRR_ENABLE_SITE, m_SPRD_EnableSite);

	DDX_Check(pDX, IDC_CHK_GRR_MODE_AUDIT_LOOP, m_bGRR_Audit_loop);
	DDX_Check(pDX, IDC_CHK_GRR_MODE_DEVICE_SWAP, m_bGRR_Device_swap);

	DDX_Control(pDX, IDC_SITESET_SAVE, m_BtnSave);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);

	DDX_Control(pDX, IDC_COMBO_BOX, m_ComboBox);

}


BEGIN_MESSAGE_MAP(CAutoGrrSetDlg, CDialog)
	ON_BN_CLICKED(IDC_SITESET_SAVE, &CAutoGrrSetDlg::OnBnClickedSitesetSave)
	ON_BN_CLICKED(IDCANCEL, &CAutoGrrSetDlg::OnBnClickedCancel)
	ON_EN_SETFOCUS(IDC_EDIT_NUMBER_OF_UNITS, &CAutoGrrSetDlg::OnEnSetfocusEditNumberOfUnits)
	ON_EN_SETFOCUS(IDC_EDIT_NUMBER_OF_TIMES, &CAutoGrrSetDlg::OnEnSetfocusEditNumberOfTimes)
	ON_BN_CLICKED(IDC_RADIO_NORMAL_MODE, &CAutoGrrSetDlg::OnBnClickedRadioNormalMode)
	ON_BN_CLICKED(IDC_RADIO_CONTIUNE_MODE, &CAutoGrrSetDlg::OnBnClickedRadioContiuneMode)
	ON_BN_CLICKED(IDC_RADIO_RECONTACT_MODE, &CAutoGrrSetDlg::OnBnClickedRadioePressReContactMode)
	ON_EN_SETFOCUS(IDC_EDIT_GRR_SOT_DELAY_TIME, &CAutoGrrSetDlg::OnEnSetfocusEditGrrSotDelayTime)
	ON_BN_CLICKED(IDC_CHK_GRR_MODE_AUDIT_LOOP, &CAutoGrrSetDlg::OnBnClickedChkGrrModeAuditLoop)
	ON_BN_CLICKED(IDC_CHK_GRR_MODE_DEVICE_SWAP, &CAutoGrrSetDlg::OnBnClickedChkGrrModeDeviceSwap)
	ON_BN_CLICKED(IDC_BUTTON_ON, &CAutoGrrSetDlg::OnBnClickedButtonOn)
	ON_BN_CLICKED(IDC_BUTTON_OFF, &CAutoGrrSetDlg::OnBnClickedButtonOff)
	ON_WM_TIMER()

END_MESSAGE_MAP()




BOOL CAutoGrrSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for( int nStation = 0; nStation < eMaxPressUnitCount; nStation++) {
		for( int nRow = 0; nRow < m_Station_Div_Y; nRow++ ) {
			for( int nCol = 0; nCol < m_Station_Div_X; nCol++) {
				m_nGrrSiteSocketOnOff[nStation][nRow][nCol] = g_DMCont.m_dmHandlerOpt.GN(NDM11_GRR_SocketOnOff_Site1_1 + ( nStation * STATION_MAX_PARA) + (nRow * STATION_MAX_X_SITE) + nCol);
			}
		}
	}

	// Init GUI
	OnInitGUI();
	InitSpreadHeadGrr();
	InitSpreadHead();



// 	if (m_bGRR_Audit_loop == 0)
// 	{
// 		OnUpdateSpreadGRRSite();
// 	}
// 	else {
// 		for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++)
// 		{
// 			for (int nRow = 0; nRow < m_Station_Div_Y; nRow++)
// 			{
// 				for (int nCol = 0; nCol < m_Station_Div_X; nCol++)
// 				{
// 					SetSocketSite(nStation, nRow, nCol);
// 				}
// 			}
// 		}
// 	}
	GetDlgItem(IDC_CHK_GRR_MODE_AUDIT_LOOP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_NORMAL_MODE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_CONTIUNE_MODE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_RECONTACT_MODE)->ShowWindow(SW_HIDE);

	// Data Load
	LoadData();
	DisableControl();
	CompanySetting();
	m_bInitSpread = TRUE;

	
	ChkGUISet();
	
	
	m_ComboBox.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CAutoGrrSetDlg::PreTranslateMessage(MSG* pMsg)
{
	//if( pMsg->message == WM_KEYDOWN ){
	//	if( pMsg->wParam == VK_ESCAPE ){
	//		return TRUE;
	//	}
	//}

	return CDialog::PreTranslateMessage(pMsg);
}

void CAutoGrrSetDlg::OnInitGUI()
{
	m_editTestCount.SetBackColor(COLOR_WWHITE);
	m_editTestCount.SetTextSize(30);

	m_editUnitCount.SetBackColor(COLOR_WWHITE);
	m_editUnitCount.SetTextSize(30);

	m_editGrrSotDelay.SetBackColor(COLOR_WWHITE);
	m_editGrrSotDelay.SetTextSize(30);
}

void CAutoGrrSetDlg::InitSpreadHeadGrr()
{

	m_SprdGrrSite.SetMaxRows( eMaxPressUnitCount / 8 );
	m_SprdGrrSite.SetMaxCols( eMaxPressUnitCount );
	
	//CString msg = "";
	//CString msg1 = "";
	//long SPREADHEADER = -1000;

	//m_SprdGrrSite.SetRow(SPREADHEADER);

	//for (int x = 0; x < eMaxPressUnitCount / 2; x++)
	//{
	//	//msg.Format("%d", x + 9);
	//	msg1.Format("%d", x + 1);
	//	m_SprdGrrSite.SetText(msg);
	//}
	m_SprdGrrSite.SetAutoSize(TRUE);
}

void CAutoGrrSetDlg::InitSpreadHead()
{
	CString msg = "";
	CString msg1 = "";
	long SPREADHEADER = -1000;

	m_SPRD_EnableSite.SetRow(SPREADHEADER);

	int nTestSite_Div_X = 0;

	nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);

	m_SPRD_EnableSite.SetMaxRows(m_Station_Div_Y);
	m_SPRD_EnableSite.SetMaxCols((eMaxPressUnitCount * nTestSite_Div_X));

	for (int x = 0; x < eMaxPressUnitCount; x++)
	{
		m_SPRD_EnableSite.AddCellSpan(x*nTestSite_Div_X + 1, 0, nTestSite_Div_X, 1);
		m_SPRD_EnableSite.SetCol(x*nTestSite_Div_X + 1);
		msg.Format("%d", x + 1);
		m_SPRD_EnableSite.SetText(msg);
	}

	m_SPRD_EnableSite.SetAutoSize(TRUE);
}

BEGIN_EVENTSINK_MAP(CAutoGrrSetDlg, CDialogEx)
	ON_EVENT(CAutoGrrSetDlg, IDC_SPREAD_GRR_ENABLE_SITE, 5, CAutoGrrSetDlg::ButtonClickedSpreadEnableSite, VTS_I4 VTS_I4)
	ON_EVENT(CAutoGrrSetDlg, IDC_SPREAD_GRR_ENABLE_SITE, 11, CAutoGrrSetDlg::DblClickSpreadEnableSite, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()
// CAutoGrrSetDlg 메시지 처리기입니다.


void CAutoGrrSetDlg::ButtonClickedSpreadEnableSite(long Col, long Row/*, short ButtonDown*/)
{
	if (ChkModuleUseAble(Col) != TRUE && g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY) != TRUE) {
		AfxMessageBox("This Module unit motor is not use able. Please check it.");
		SetTimer(1, 10, NULL);
	}
	else {

		if ((Row == 1 || Row == 2) && m_bInitSpread)
		{
			int nStation_Idx = (Col - 1) / m_Station_Div_X;
			int nChk = m_nGrrSiteSocketOnOff[ePressUnit_Down_1 + nStation_Idx][Row - 1][(Col - 1) % m_Station_Div_X];
			BOOL bIsTrue = FALSE;
			int nOnOff = nChk;

			if (g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY) != TRUE)
			{
				if (nChk != DEF_SITE_ON)
				{
					int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
					if (nRunMode == eRUN_MODE_ONLINE) {
						BOOL bConnt = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + nStation_Idx);

						if (bConnt == FALSE) {
							AfxMessageBox("Can not site on. Tester is disconnection. Please check ");
							SetTimer(1, 10, NULL);
						}
					}
				}

			}
			if (nChk == DEF_SITE_SOCKET_HOLD || nChk == DEF_SITE_SOCKET_LOCK)
			{
				SetTimer(1, 10, NULL);
				return;
			}

			if (nChk != DEF_SITE_ON) { //기존에 Off,Reserve면
				nOnOff = DEF_SITE_ON;
			}
			else //기존에 On이면
			{
				if (g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY) != TRUE)
				{
					int nCol = Col % 4 == 0 ? 4 : Col % 4;

					if (g_DMCont.m_dmHandlerOpt.GN(NDM11_GRR_SocketOnOff_Site1_1 + (nStation_Idx * (m_Station_Div_X * m_Station_Div_Y)) + ((Row - 1) * m_Station_Div_X) + (nCol - 1)) == DEF_SITE_OFF) {
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

			CString strOnOff = _T("");
			strOnOff.Format("%d", nOnOff);
			m_SPRD_EnableSite.SetTextMatrix(Row, Col, strOnOff);

			m_nGrrSiteSocketOnOff[ePressUnit_Down_1 + nStation_Idx][Row - 1][(Col - 1) % m_Station_Div_X] = nOnOff;

			SetTimer(1, 10, NULL);

			//if (bIsTrue == FALSE) {
			//	AfxMessageBox("Please. After 'One Cycle' do Change Site On/Off");
			//	SetTimer(1, 500, NULL);
			//}
		}
		else if (Row == 0) //Station Num 클릭
		{
			int nStation_Idx = (Col - 1) / m_Station_Div_X;

			BOOL bIsOneON = IsSite_On_Of_All(nStation_Idx);
			BOOL bIsOneOff = IsSite_Off_Of_All(nStation_Idx);
			BOOL bIsOneReserve = IsSite_Reserve_Of_All(nStation_Idx);
			BOOL bIsOneHold = IsSite_Hold_Of_All(nStation_Idx);

			int nMake_Site_Status = -2;//초기화;

			if (g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY) != TRUE) //돌고있음
			{
				if (bIsOneHold)
				{
					return;
				}

				if (bIsOneOff || bIsOneReserve)//하나라도 OFF나 reserve있다면,
				{
					int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
					if (nRunMode == eRUN_MODE_ONLINE)
					{
						BOOL bConnt = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + nStation_Idx);
						if (bConnt)
						{
							//괜찮음
						}
						else
						{
							AfxMessageBox("Can not site on. Tester is disconnection. Please check ");
						}
					}
					else if (nRunMode == eRUN_MODE_OFFLINE)
					{
						//괜찮음
					}
				}
			}

			if (bIsOneHold)
			{
				return;
			}

			if (bIsOneOff || bIsOneReserve)//하나라도 OFF나 reserve있다면,
			{
				nMake_Site_Status = DEF_SITE_ON;
			}
			else //기존에 모두 On이면
			{
				if (g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY) != TRUE)
				{
					int nCol = Col % 4 == 0 ? 4 : Col % 4;
					int n = g_DMCont.m_dmHandlerOpt.GN(NDM11_GRR_SocketOnOff_Site1_1 + nStation_Idx * 8);
					if (g_DMCont.m_dmHandlerOpt.GN(NDM11_GRR_SocketOnOff_Site1_1 + nStation_Idx * 8) == DEF_SITE_OFF)
					{
						nMake_Site_Status = DEF_SITE_OFF;
					}
					else
					{
						nMake_Site_Status = DEF_SITE_OFF_RESERVATION;
					}
				}
				else
				{
					nMake_Site_Status = DEF_SITE_OFF;
				}
			}

			CString strMake_Site_Status = _T("");
			strMake_Site_Status.Format("%d", nMake_Site_Status);

			for (int nRow = 1; nRow <= 2; nRow++)
				for (int nCol = nStation_Idx * 4 + 1; nCol <= nStation_Idx * 4 + 4; nCol++)
					m_SPRD_EnableSite.SetTextMatrix(nRow, nCol, strMake_Site_Status);


			for (int nRow = 0; nRow < m_Station_Div_Y; nRow++)
				for (int nCol = 0; nCol < m_Station_Div_X; nCol++)
				{
					m_nGrrSiteSocketOnOff[nStation_Idx][nRow][nCol] = nMake_Site_Status;
				}
		}
	}
}


BOOL CAutoGrrSetDlg::IsSite_On_Of_All(int nStationNum) //하나라도 On 이면 TRUE
{
	for (int nRow = 0; nRow < m_Station_Div_Y; nRow++)
		for (int nCol = 0; nCol < m_Station_Div_X; nCol++)
		{
			if (DEF_SITE_ON == m_nGrrSiteSocketOnOff[nStationNum][nRow][nCol])
				return TRUE;
		}

	return FALSE;
}
BOOL CAutoGrrSetDlg::IsSite_Off_Of_All(int nStationNum) //하나라도 Off 이면 TRUE
{
	for (int nRow = 0; nRow < m_Station_Div_Y; nRow++)
		for (int nCol = 0; nCol < m_Station_Div_X; nCol++)
		{
			if (DEF_SITE_OFF == m_nGrrSiteSocketOnOff[nStationNum][nRow][nCol])
				return TRUE;
		}

	return FALSE;
}
BOOL CAutoGrrSetDlg::IsSite_Reserve_Of_All(int nStationNum) //하나라도 reserve 이면 TRUE
{
	for (int nRow = 0; nRow < m_Station_Div_Y; nRow++)
		for (int nCol = 0; nCol < m_Station_Div_X; nCol++)
		{
			if (DEF_SITE_OFF_RESERVATION == m_nGrrSiteSocketOnOff[nStationNum][nRow][nCol])
				return TRUE;
		}

	return FALSE;
}

BOOL CAutoGrrSetDlg::IsSite_Hold_Of_All(int nStationNum) //하나라도 Hold 이면 TRUE
{
	for (int nRow = 0; nRow < m_Station_Div_Y; nRow++)
		for (int nCol = 0; nCol < m_Station_Div_X; nCol++)
		{
			if (DEF_SITE_SOCKET_HOLD == m_nGrrSiteSocketOnOff[nStationNum][nRow][nCol] /*|| DEF_SITE_SOCKET_LOCK == m_nGrrSiteSocketOnOff[nStationNum][nRow][nCol]*/)
				return TRUE;
		}

	return FALSE;
}

void CAutoGrrSetDlg::OnUpdateSpreadGRRSite()
{
	for( int nStation = 0; nStation < eMaxPressUnitCount; nStation++)
	{
		if (m_nGrrSiteSocketOnOff[nStation][0][0] == DEF_SITE_ON)
			m_SprdGrrSite.SetTextMatrix((nStation / 8 == 0 ? 2 : 1), (nStation % 8) + 1, "1");
		else
			m_SprdGrrSite.SetTextMatrix((nStation / 8 == 0 ? 2 : 1), (nStation % 8) + 1, "0");
	}
}

void CAutoGrrSetDlg::SetSocketSite(int _Station, int _Row, int _Col)
{	
	int nSite_Col = 1 + _Col + (_Station * m_Station_Div_X);
	//Down
	if (m_nGrrSiteSocketOnOff[_Station][_Row][_Col] == DEF_SITE_ON) {
		m_SPRD_EnableSite.SetTextMatrix(1 + _Row, nSite_Col, "1");
	}
	else if (m_nGrrSiteSocketOnOff[_Station][_Row][_Col] == DEF_SITE_OFF_RESERVATION) {
		m_SPRD_EnableSite.SetTextMatrix(1 + _Row, nSite_Col, "2");
	}
	else if (m_nGrrSiteSocketOnOff[_Station][_Row][_Col] == DEF_SITE_SOCKET_HOLD) {
		m_SPRD_EnableSite.SetTextMatrix(1 + _Row, nSite_Col, "3");
		m_SPRD_EnableSite.SetBackColor(COLOR_BBLACK);
	}
	else if (m_nGrrSiteSocketOnOff[_Station][_Row][_Col] == DEF_SITE_SOCKET_LOCK) {
		m_SPRD_EnableSite.SetTextMatrix(1 + _Row, nSite_Col, "3");
		m_SPRD_EnableSite.SetBackColor(COLOR_BLUE);
	}
	else {
		m_SPRD_EnableSite.SetTextMatrix(1 + _Row, nSite_Col, "0");
	}
}



void CAutoGrrSetDlg::OnUpdateSpreadSite64Para()
{
	for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++)
	{
		for (int nRow = 0; nRow < m_Station_Div_Y; nRow++)
		{
			for (int nCol = 0; nCol < m_Station_Div_X; nCol++)
			{
				SetSocketSite(nStation, nRow, nCol);
			}
		}
	}
}


void CAutoGrrSetDlg::LoadData()
{
	UpdateData(TRUE);

	char szCompany[16] = { 0, };

	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
	m_nAutoGrrTestCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrTestCount);
	m_nAutoGrrUnitCount = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrUnitCount);
	m_nAutoGrrmode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoGrrMode);
	m_dGrrSotDelay = g_DMCont.m_dmHandlerOpt.GN(NDM11_GRR_SOT_DELAY_TIME) / 1000.0;
	m_bGRR_Device_swap = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrDeviceSwapUse);

	SetDlgItemInt(IDC_EDIT_NUMBER_OF_TIMES, m_nAutoGrrTestCount);
	SetDlgItemInt(IDC_EDIT_NUMBER_OF_UNITS, m_nAutoGrrUnitCount);
	SetDlgItemInt(IDC_EDIT_GRR_SOT_DELAY_TIME, m_dGrrSotDelay);

	if (m_bGRR_Audit_loop == 0)
	{
		OnUpdateSpreadGRRSite();
	}
	else {
// 		for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++)
// 		{
// 			for (int nRow = 0; nRow < m_Station_Div_Y; nRow++)
// 			{
// 				for (int nCol = 0; nCol < m_Station_Div_X; nCol++)
// 				{
// 					SetSocketSite(nStation, nRow, nCol);
// 				}
// 			}
// 		}

		OnUpdateSpreadSite64Para();
	}

	UpdateData(FALSE);
}

void CAutoGrrSetDlg::OnBnClickedSitesetSave()
{
	SiteSetSave();	
}
void CAutoGrrSetDlg::SiteSetSave()
{
	UpdateData(TRUE);

	BOOL bModuleOn[2] = { FALSE, };
	int nSiteOnCnt = 0;
	if (m_bGRR_Audit_loop == 0)
	{
		for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++) {
			for (int nRow = 0; nRow < m_Station_Div_Y; nRow++) {
				for (int nCol = 0; nCol < m_Station_Div_X; nCol++) {

					if ("1" == m_SprdGrrSite.GetTextMatrix((1), 1 + (nStation % 8)))
					{
						nSiteOnCnt++;
						m_nGrrSiteSocketOnOff[nStation][nRow][nCol] = DEF_SITE_ON;
					}
					else {
						m_nGrrSiteSocketOnOff[nStation][nRow][nCol] = DEF_SITE_OFF;
					}

					//if( nStation >= eMaxPressUnitCount/2 ) {
					//	//UP SITE
					//	if ("1" == m_ssEnableSite9_16.GetTextMatrix( 1 + nRow, 1 + nCol + ( (nStation - (eMaxPressUnitCount/2)) * m_Station_Div_X ) ) )
					//		m_nGrrSiteSocketOnOff[nStation][nRow][nCol] = DEF_SITE_ON;
					//	else
					//		m_nGrrSiteSocketOnOff[nStation][nRow][nCol] = DEF_SITE_OFF;
					//}
					//else {
					//	//DOWN SITE
					//	if ("1" == m_ssEnableSite1_8.GetTextMatrix( 1 + nRow, 1 + nCol + ( nStation * m_Station_Div_X ) ) )
					//		m_nGrrSiteSocketOnOff[nStation][nRow][nCol] = DEF_SITE_ON;
					//	else
					//		m_nGrrSiteSocketOnOff[nStation][nRow][nCol] = DEF_SITE_OFF;
					//}

					//int nCalStationNo = (nStation < (eMaxPressUnitCount / 2) ? nStation : nStation % (eMaxPressUnitCount / 2));// ( (nStation / (eMaxPressUnitCount / 2)) > % 4;
					int nModuleNo = nStation / (eMaxPressUnitCount/2);
					if (m_nGrrSiteSocketOnOff[nStation][nRow][nCol] == DEF_SITE_ON)
						bModuleOn[nModuleNo] = TRUE;
				}
			}
		}
	}
	else {
		for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++) {
			for (int nRow = 0; nRow < m_Station_Div_Y; nRow++) {
				for (int nCol = 0; nCol < m_Station_Div_X; nCol++) {
				
					if (m_nGrrSiteSocketOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_HOLD || m_nGrrSiteSocketOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_LOCK)
						continue;
					//DOWN SITE
					if ("1" == m_SPRD_EnableSite.GetTextMatrix(1 + nRow, 1 + nCol + (nStation  * m_Station_Div_X)))
						m_nGrrSiteSocketOnOff[nStation][nRow][nCol] = DEF_SITE_ON;
					else if ("2" == m_SPRD_EnableSite.GetTextMatrix(1 + nRow, 1 + nCol + (nStation  * m_Station_Div_X)))
						m_nGrrSiteSocketOnOff[nStation][nRow][nCol] = DEF_SITE_OFF_RESERVATION;
					else
						m_nGrrSiteSocketOnOff[nStation][nRow][nCol] = DEF_SITE_OFF;
					
				}
			}
		}
	}


	if (ChkAllStationOFF() == TRUE) {
		AfxMessageBox(_T("It can not be saved when the whole Socket is turned off."), MB_ICONHAND | MB_TOPMOST);
		return;
	}
	int nCount = GetDlgItemInt(IDC_EDIT_NUMBER_OF_UNITS);
	// check half & half ? only 1 module?

	int nTotalSite = m_Station_Div_X * m_Station_Div_Y;

	if (bModuleOn[0] == TRUE && bModuleOn[1] == FALSE) {
		int nCount = GetDlgItemInt(IDC_EDIT_NUMBER_OF_UNITS);
		if (nCount < nTotalSite)
		{
			CString strMsg = _T("");
			strMsg.Format("module 1 site on unit count is %d more setting please ", nTotalSite);
			AfxMessageBox(strMsg);
			return;
		}
	}
	else if (bModuleOn[0] == FALSE && bModuleOn[1] == TRUE)
	{
		if (nCount < nTotalSite)
		{
			CString strMsg = _T("");
			strMsg.Format("module 2 site on unit count is %d more setting please ", nTotalSite);
			AfxMessageBox(strMsg);
			return;
		}
	}

	if (bModuleOn[0] && bModuleOn[1]) {
		int nCount = GetDlgItemInt(IDC_EDIT_NUMBER_OF_UNITS);
		if (nCount < nTotalSite*2)
		{
			CString strMsg = _T("");
			strMsg.Format("module 1, 2 site on unit count is %d more setting please ", nTotalSite * 2);
			AfxMessageBox(strMsg);
			return;
		}
	}

	if (m_bGRR_Audit_loop == 0) {
// 		if (bModuleOn[0] && bModuleOn[1])
// 		{
// 			int nCount = GetDlgItemInt(IDC_EDIT_NUMBER_OF_UNITS);
// 			if (nCount != 64)
// 			{
// 				AfxMessageBox(_T("unit count is not even number. please check it"), MB_ICONHAND | MB_TOPMOST);
// 				return;
// 			}
// 		}


		int nCount = GetDlgItemInt(IDC_EDIT_NUMBER_OF_UNITS);

		int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
		//char cMinCnt[2] = "";
		if (nProjectOption == DEF_PROJECT_CAMPBELTOWN) { //8para
			if (nCount % 8 != 0) {//4
				AfxMessageBox(_T("Audit unit count is not even number. please check it / 8,16,24, ...64 Multiples of 8"), MB_ICONHAND | MB_TOPMOST);
				return;
			}

			//설정한 Device Cnt가 Site On Cnt 보다 많을 경우.
			if (nCount > nSiteOnCnt) {
				AfxMessageBox(_T("Audit unit count is Over."), MB_ICONHAND | MB_TOPMOST);
				return;
			}
		}
		else{
			if (nCount % 4 != 0) {//4
				AfxMessageBox(_T("Audit unit count is not even number. please check it / 4,8,16,24, ...64 Multiples of 8"), MB_ICONHAND | MB_TOPMOST);
				return;
			}

			//설정한 Device Cnt가 Site On Cnt 보다 많을 경우.
			if (nCount > nSiteOnCnt) {
				AfxMessageBox(_T("Audit unit count is Over."), MB_ICONHAND | MB_TOPMOST);
				return;
			}
		}


// 		if (nCount % 8 != 0) {//4
// 			AfxMessageBox(_T("Audit unit count is not even number. please check it / 8,16,24, ...64 Multiples of 8"), MB_ICONHAND | MB_TOPMOST);
// 			return;
// 		}

	}
	else {
		int nCount = GetDlgItemInt(IDC_EDIT_NUMBER_OF_UNITS);

		int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
		//char cMinCnt[2] = "";
		if (nProjectOption == DEF_PROJECT_CAMPBELTOWN) { //8para
			if (nCount % 8 != 0) {//4
				AfxMessageBox(_T("Audit unit count is not even number. please check it / 8,16,24, ...64 Multiples of 8"), MB_ICONHAND | MB_TOPMOST);
				return;
			}
		}
		else {
			if (nCount % 4 != 0) {//4
				AfxMessageBox(_T("Audit unit count is not even number. please check it / 4,8,16,24, ...64 Multiples of 8"), MB_ICONHAND | MB_TOPMOST);
				return;
			}
		}




// 		if (nCount % 8 != 0) {//4
// 			AfxMessageBox(_T("Audit unit count is not even number. please check it / 8,16,24, ...64 Multiples of 8"), MB_ICONHAND | MB_TOPMOST);
// 			return;
// 		}

	}


	UpdateData(TRUE);

	char szCokName[512] = { 0, };
	ST_BASIC_INFO   stBasicData, stBeforeBasicData;
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
	g_COK_FileMgr.LoadBasicInfo(szCokName, stBasicData);

	stBeforeBasicData = stBasicData;

	m_nAutoGrrUnitCount = GetDlgItemInt(IDC_EDIT_NUMBER_OF_UNITS);
	m_nAutoGrrTestCount = GetDlgItemInt(IDC_EDIT_NUMBER_OF_TIMES);
	m_dGrrSotDelay = GetDlgItemInt(IDC_EDIT_GRR_SOT_DELAY_TIME);

	for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++) {
		for (int nRow = 0; nRow < m_Station_Div_Y; nRow++) {
			for (int nCol = 0; nCol < m_Station_Div_X; nCol++) {
				g_DMCont.m_dmHandlerOpt.SN(NDM11_GRR_SocketOnOff_Site1_1 + (nStation * (STATION_MAX_PARA)) + (nRow * STATION_MAX_X_SITE) + nCol, m_nGrrSiteSocketOnOff[nStation][nRow][nCol]);
				stBasicData.nGRRSiteOnOff[nStation][nRow][nCol] = m_nGrrSiteSocketOnOff[nStation][nRow][nCol];
			}
		}
	}

	g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_AutoGrrTestCount, m_nAutoGrrTestCount);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_AutoGrrUnitCount, m_nAutoGrrUnitCount);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_AutoGrrMode, m_nAutoGrrmode);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_GRR_SOT_DELAY_TIME, (m_dGrrSotDelay * 1000.0));

	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_AutoGrrDeviceSwapUse, m_bGRR_Device_swap);
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_AutoGrrAuditLoopUse, m_bGRR_Audit_loop);

	stBasicData.nAutoGrrTestCount = m_nAutoGrrTestCount;
	stBasicData.nAutoGrrUnitCount = m_nAutoGrrUnitCount;
	stBasicData.nAutoGrrMode = m_nAutoGrrmode;
	stBasicData.nGrrSotDelay = m_dGrrSotDelay * 1000.0;

	stBasicData.bGRR_Device_swap = m_bGRR_Device_swap;
	stBasicData.bGRR_Audit_loop = m_bGRR_Audit_loop;

	g_COK_FileMgr.SaveBasicInfo(szCokName, &stBasicData);

	std::vector<std::pair<CString, std::pair<CString, CString>>>vData;

	stBeforeBasicData.IsSameGRRSiteSet(stBasicData, vData);




	if (!(vData.empty()))
	{
		SiteSetChangeLog(vData);
	}
}

void CAutoGrrSetDlg::SiteSetChangeLog( std::vector<std::pair<CString, std::pair<CString, CString>>> vData )
{
	for( int i = 0; i < ( int )vData.size(); i++ ) {
		std::pair<CString, std::pair<CString, CString>> p;

		p = vData[ i ];

		MakeLog( "[MENU = GRR Setting, DATA NAME = %s ][BEFORE=%s, AFTER=%s]", p.first, p.second.first, p.second.second );
	}
}


void CAutoGrrSetDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnCancel();
}

void CAutoGrrSetDlg::OnEnSetfocusEditNumberOfUnits()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};

	sprintf_s(szVal, sizeof(szVal), "%d", m_nAutoGrrUnitCount);
	int nRet;
	if(m_bGRR_Audit_loop)
		nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Auto GRR Unit Count", "64", "8");
	else {
		char cMinCnt[2] = "";
		if (m_Station_Div_Y == 2) { //8para
			sprintf_s(cMinCnt, sizeof(cMinCnt), "%d", 8);
		}
		else { //4para
			sprintf_s(cMinCnt, sizeof(cMinCnt), "%d", 4);
		}

		int nSiteOnCnt = 0;
		for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++) {
			for (int nRow = 0; nRow < m_Station_Div_Y; nRow++) {
				for (int nCol = 0; nCol < m_Station_Div_X; nCol++) {

					if ("1" == m_SprdGrrSite.GetTextMatrix((1), 1 + (nStation % 8)))
					{
						nSiteOnCnt++;
					}
				}
			}
		}

		char cMaxCnt[4] = "";
		sprintf_s(cMaxCnt, sizeof(cMaxCnt), "%d", nSiteOnCnt);


		nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Auto GRR Unit Count", cMaxCnt, cMinCnt/*"8"*/);
	}

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_NUMBER_OF_UNITS)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}	

}

void CAutoGrrSetDlg::OnEnSetfocusEditNumberOfTimes()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};

	sprintf_s(szVal, sizeof(szVal), "%d", m_nAutoGrrTestCount);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Auto GRR Test Count", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_NUMBER_OF_TIMES)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}	

}

void CAutoGrrSetDlg::MakeLog( LPCTSTR fmt, ... )
{
	TCHAR tmpMsg[ 1024 ] = { 0, };
	if( fmt ) {
		va_list argList;
		va_start( argList, fmt );
		_vstprintf( tmpMsg, fmt, argList );
		va_end( argList );
	}
	TWLOG.MakeLog( static_cast< int >( eLogID_System_GUI ), Debug, "", __LINE__, NULL, tmpMsg );
}

void CAutoGrrSetDlg::OnBnClickedButtonOn()
{
	SetComboSettingToSprd(DEF_SITE_ON);
}

void CAutoGrrSetDlg::OnBnClickedButtonOff()
{
	SetComboSettingToSprd(DEF_SITE_OFF);
}

void CAutoGrrSetDlg::SetComboSettingToSprd(BOOL bOnOff)
{
	CString strOnOff = _T("");
	if (m_bGRR_Audit_loop == 0) {
// 		if (bOnOff == DEF_SITE_ON)
// 			strOnOff = "1";
// 		else
// 			strOnOff = "0";
// 
// 		int nSelect = m_ComboBox.GetCurSel();
// 
// 		switch (nSelect)
// 		{
// 			case STATION1:  SetSocketOnOFF(bOnOff, nSelect); break;
// 			case STATION2:  SetSocketOnOFF(bOnOff, nSelect); break;
// 			case STATION3:  SetSocketOnOFF(bOnOff, nSelect); break;
// 			case STATION4:	SetSocketOnOFF(bOnOff, nSelect); break;
// 			case STATION5:  SetSocketOnOFF(bOnOff, nSelect); break;
// 			case STATION6:  SetSocketOnOFF(bOnOff, nSelect); break;
// 			case STATION7:  SetSocketOnOFF(bOnOff, nSelect); break;
// 			case STATION8:	SetSocketOnOFF(bOnOff, nSelect); break;
// 			case ALL:
// 			{
// 				for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++) {
// 					for (int nRow = 0; nRow < m_Station_Div_Y; nRow++) {
// 						for (int nCol = 0; nCol < m_Station_Div_X; nCol++) {
// 							if (m_nGrrSiteSocketOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_HOLD || m_nGrrSiteSocketOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_LOCK)
// 								continue;
// 
// 							m_ssEnableSite9_16.SetTextMatrix(1 + nRow, nCol + 1 + (nStation * m_Station_Div_X), strOnOff);
// 							m_ssEnableSite1_8.SetTextMatrix(1 + nRow, nCol + 1 + (nStation * m_Station_Div_X), strOnOff);
// 						}
// 					}
// 				}
// 			}
// 			break;
// 		}
	
	}
	else {
		m_bInitSpread = FALSE;
		if (bOnOff == DEF_SITE_ON)
			strOnOff = "1";
		else if (bOnOff == DEF_SITE_OFF_RESERVATION)
			strOnOff = "2";
		else
			strOnOff = "0";

		int nSelect = m_ComboBox.GetCurSel();

		switch (nSelect)
		{
			case STATION1:  SetSocketOnOFF(bOnOff, nSelect); break;
			case STATION2:  SetSocketOnOFF(bOnOff, nSelect); break;
			case STATION3:  SetSocketOnOFF(bOnOff, nSelect); break;
			case STATION4:	SetSocketOnOFF(bOnOff, nSelect); break;
			case STATION5:  SetSocketOnOFF(bOnOff, nSelect); break;
			case STATION6:  SetSocketOnOFF(bOnOff, nSelect); break;
			case STATION7:  SetSocketOnOFF(bOnOff, nSelect); break;
			case STATION8:	SetSocketOnOFF(bOnOff, nSelect); break;
			case ALL:
			{
				for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++) {
					for (int nRow = 0; nRow < m_Station_Div_Y; nRow++) {
						for (int nCol = 0; nCol < m_Station_Div_X; nCol++) {
							if (m_nGrrSiteSocketOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_HOLD || m_nGrrSiteSocketOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_LOCK)
								continue;

							m_SPRD_EnableSite.SetTextMatrix(1 + nRow, (nStation * m_Station_Div_X) + nCol + 1, strOnOff);
							m_nGrrSiteSocketOnOff[nStation][nRow][nCol] = bOnOff;
						}
					}
				}
			}
			break;
		}
		m_bInitSpread = TRUE;
	}
	
}


void CAutoGrrSetDlg::SetSocketOnOFF( BOOL bOnOff, int _nStation )
{
	CString strOnOff = _T("");
	if (m_bGRR_Audit_loop == 0) {
// 		if (bOnOff == DEF_SITE_ON)
// 			strOnOff = "1";
// 		else
// 			strOnOff = "0";
// 
// 		for (int nRow = 0; nRow < m_Station_Div_Y; nRow++) {
// 			for (int nCol = 0; nCol < m_Station_Div_X; nCol++) {
// 
// 				if (m_nGrrSiteSocketOnOff[_nStation][nRow][nCol] == DEF_SITE_SOCKET_HOLD || m_nGrrSiteSocketOnOff[_nStation][nRow][nCol] == DEF_SITE_SOCKET_LOCK)
// 					continue;
// 
// 				if (_nStation >= 8)
// 					m_ssEnableSite9_16.SetTextMatrix(1 + nRow, nCol + 1 + ((_nStation - (eMaxPressUnitCount / 2)) * m_Station_Div_X), strOnOff);
// 				else
// 					m_ssEnableSite1_8.SetTextMatrix(1 + nRow, nCol + 1 + (_nStation * m_Station_Div_X), strOnOff);
// 			}
// 		}
		
	}
	else {
		if (bOnOff == DEF_SITE_ON)
			strOnOff = "1";
		else if (bOnOff == DEF_SITE_OFF_RESERVATION)
			strOnOff = "2";
		else
			strOnOff = "0";

		for (int nRow = 0; nRow < m_Station_Div_Y; nRow++) {
			for (int nCol = 0; nCol < m_Station_Div_X; nCol++) {
				if (m_nGrrSiteSocketOnOff[_nStation][nRow][nCol] == DEF_SITE_SOCKET_HOLD || m_nGrrSiteSocketOnOff[_nStation][nRow][nCol] == DEF_SITE_SOCKET_LOCK)
					continue;

				m_SPRD_EnableSite.SetTextMatrix(1 + nRow, nCol + 1 + (_nStation * m_Station_Div_X), strOnOff);

				m_nGrrSiteSocketOnOff[_nStation][nRow][nCol] = bOnOff;
			}
		}
	}
	
}


BOOL CAutoGrrSetDlg::ChkAllStationOFF()
{
	for( int nStation = 0; nStation < eMaxPressUnitCount; nStation++) {
		for( int nRow = 0; nRow < m_Station_Div_Y; nRow++ ) {
			for (int nCol = 0; nCol < m_Station_Div_X; nCol++) {
				if (m_nGrrSiteSocketOnOff[nStation][nRow][nCol] == DEF_ON)
					return FALSE;
			}
		}
	}
	return TRUE;
}

void CAutoGrrSetDlg::OnBnClickedRadioNormalMode()
{
	m_nAutoGrrmode = eNormal_Mode;
}


void CAutoGrrSetDlg::OnBnClickedRadioContiuneMode()
{
	m_nAutoGrrmode = ePressContinueTest;
}


void CAutoGrrSetDlg::OnBnClickedRadioePressReContactMode()
{
	m_nAutoGrrmode = ePressReContact;
}


void CAutoGrrSetDlg::OnEnSetfocusEditGrrSotDelayTime()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };

	sprintf_s(szVal, sizeof(szVal), "%.2f", m_dGrrSotDelay);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "AutoGRR SOT DelayTime", "10", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_GRR_SOT_DELAY_TIME)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}

void CAutoGrrSetDlg::DisableControl()
{
	BOOL bEnAble = TRUE;
	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
	if (bLotState == eHandlerLotMode_OnGoing) {
		bEnAble = FALSE;
	}

	GetDlgItem(IDC_EDIT_NUMBER_OF_UNITS)->EnableWindow(bEnAble);
	GetDlgItem(IDC_EDIT_NUMBER_OF_TIMES)->EnableWindow(bEnAble);
	GetDlgItem(IDC_SPREAD_GRR)->EnableWindow(bEnAble);

}

BOOL CAutoGrrSetDlg::ChkModuleUseAble(int nCol)
{
	BOOL bIsUseAble = FALSE;

	if ((eMaxPressUnitCount * m_Station_Div_X) / 4 >= nCol) {
		bIsUseAble = g_DMCont.m_dmEQP.GB(BDM0_MODULE1_SITE_ON_IS_POSSIBLE);
	}
	else {
		bIsUseAble = g_DMCont.m_dmEQP.GB(BDM0_MODULE2_SITE_ON_IS_POSSIBLE);
	}

	return bIsUseAble;
}

void CAutoGrrSetDlg::OnBnClickedChkGrrModeDeviceSwap()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
//	LoadData();
}


void CAutoGrrSetDlg::OnBnClickedChkGrrModeAuditLoop()
{
	UpdateData(TRUE);
	ChkGUISet();
}
void CAutoGrrSetDlg::ChkGUISet()
{
	SIZE s;
	ZeroMemory(&s, sizeof(s));
	int nOffset = 100;
	s.cx = (LONG)::GetSystemMetrics(SM_CXFULLSCREEN);
	s.cy = (LONG)::GetSystemMetrics(SM_CYFULLSCREEN);
	if (m_bGRR_Audit_loop == 1) { // Audit Mode [3/29/2021 donghyun.shin]
		
//  		MoveWindow(0, 0, s.cx*0.75 , s.cy*0.45);
// // 		m_cButtonSave.MoveWindow(670, 430, 120, 60);
// // 		m_cButtonExit.MoveWindow(790, 430, 120, 60);
		GetDlgItem(IDC_SPREAD_GRR_ENABLE_SITE)->ShowWindow(TRUE);

		GetDlgItem(IDC_SPREAD_GRR)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_ENABLE_SITE)->ShowWindow(FALSE);

		GetDlgItem(IDC_COMBO_BOX)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_ON)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_OFF)->ShowWindow(SW_SHOW);
	}
	else { // GRR Mode [3/29/2021 donghyun.shin]
				
// 		MoveWindow(0, 0, s.cx*0.55, s.cy*0.55);
// 		m_cButtonSave.MoveWindow(400, 300, 120, 60);
// 		m_cButtonExit.MoveWindow(520, 300, 120, 60);
		GetDlgItem(IDC_SPREAD_GRR_ENABLE_SITE)->ShowWindow(FALSE);

		GetDlgItem(IDC_SPREAD_GRR)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_ENABLE_SITE)->ShowWindow(TRUE);

		GetDlgItem(IDC_COMBO_BOX)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ON)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_OFF)->ShowWindow(FALSE);
	}
	LoadData();

	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);

}

void CAutoGrrSetDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	this->SetFocus();
	KillTimer(nIDEvent);

	CDialog::OnTimer(nIDEvent);
}

void CAutoGrrSetDlg::DblClickSpreadEnableSite(long Col, long Row)
{
	SetTimer(1, 10, NULL);
}

void CAutoGrrSetDlg::CompanySetting()
{
	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
}

