// CSiteSetDlg_128Para.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "SiteSetDlg_128Para.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include <afxctl.h>


// CSiteSetDlg_128Para 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSiteSetDlg_128Para, CDialogEx)

CSiteSetDlg_128Para::CSiteSetDlg_128Para(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSiteSetDlg_128Para::IDD, pParent)

{
	
	m_nOffLineTT = g_DMCont.m_dmHandlerOpt.GN(NDM11_TM_OffLineTest) / 1000;
	//m_nOffLineTestTime 
	m_nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
	m_nDeviceMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_SYS_MD_DEVICE_CHK);
	m_bAutoRetest_TestPP = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);

	m_nAutoRetestKind = eAutoRetest_AAB;//g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoRetest);

	m_bRetest_Skip_HW1 = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Retest_Skip_HW1);
	m_bRetest_Skip_HW2 = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Retest_Skip_HW2);
	m_bRetest_Skip_HW3 = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Retest_Skip_HW3);
	m_bRetest_Skip_HW4 = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_Retest_Skip_HW4);

	m_TestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	m_TestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	m_nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

// 	if (m_nProjectOption == DEF_PROJECT_TPUT)
// 		m_TestSite_Div_X = 2;

	m_bOffLineBinRandom = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_OFFLINEBIN_RANDOM);
	m_bOffLineYieldCut  = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_OFFLINEBIN_YIELDCUT);
	m_dOffLineYieldCutPer = g_DMCont.m_dmHandlerOpt.GD(DDM11_OffLine_YieldCutPer);

	m_bAlwayBin1Sorting = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AlwayBin1Sorting);

	m_bInitSpread = FALSE;

	memset(m_nSiteOnOff, 0x00, sizeof(m_nSiteOnOff));
	m_nTestSite = 0;
}

CSiteSetDlg_128Para::~CSiteSetDlg_128Para()
{
}

void CSiteSetDlg_128Para::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_RUNMODE1, m_BtnOnline);
	DDX_Control(pDX, IDC_RADIO_RUNMODE2, m_BtnOffLine);
	DDX_Control(pDX, IDC_RADIO_RUNMODE3, m_BtnDemo);
	DDX_Control(pDX, IDC_RADIO_DVCMODE1, m_BtnExistDvc);
	DDX_Control(pDX, IDC_RADIO_DVCMODE2, m_BtnNoDvcExistTray);
	DDX_Control(pDX, IDC_RADIO_DVCMODE3, m_BtnNoDvcNoTray);

	DDX_Control(pDX, IDC_SITESET_SAVE, m_BtnSave);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_SPREAD_ENABLE_SITE, m_SPRD_EnableSite);

	DDX_Check(pDX, IDC_CHK_AUTO_RETEST, m_bAutoRetest_TestPP);
	DDX_Check(pDX, IDC_CHK_ALWAY_BIN1_SORTING, m_bAlwayBin1Sorting);
	
	DDX_Radio(pDX, IDC_RADIO_AUTO_RETEST_A_ONLY, m_nAutoRetestKind);

	DDX_Check(pDX, IDC_CHK_RETEST_SKIP_HW1, m_bRetest_Skip_HW1);
	DDX_Check(pDX, IDC_CHK_RETEST_SKIP_HW2, m_bRetest_Skip_HW2);
	DDX_Check(pDX, IDC_CHK_RETEST_SKIP_HW3, m_bRetest_Skip_HW3);
	DDX_Check(pDX, IDC_CHK_RETEST_SKIP_HW4, m_bRetest_Skip_HW4);
	DDX_Control(pDX, IDC_COMBO_BOX, m_ComboBox);
	DDX_Control(pDX, IDC_EDIT_OFFLINE_TEST_TIME2, m_editOffLineTestTime);

	DDX_Text(pDX, IDC_EDIT_OFFLINE_TEST_TIME2, m_nOffLineTT);

	DDX_Check(pDX, IDC_CHK_RANDOM, m_bOffLineBinRandom);
	DDX_Check(pDX, IDC_CHK_YIELD_CUT, m_bOffLineYieldCut);
	DDX_Text(pDX, IDC_EDIT_YIELD_CUT_PER, m_dOffLineYieldCutPer);
	DDX_Control(pDX, IDC_EDIT_YIELD_CUT_PER, m_editOffLineYieldCut);
}


BEGIN_MESSAGE_MAP(CSiteSetDlg_128Para, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CSiteSetDlg_128Para::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO_RUNMODE1, &CSiteSetDlg_128Para::OnBnClickedRadioRunmode1)
	ON_BN_CLICKED(IDC_RADIO_RUNMODE2, &CSiteSetDlg_128Para::OnBnClickedRadioRunmode2)
	ON_BN_CLICKED(IDC_RADIO_RUNMODE3, &CSiteSetDlg_128Para::OnBnClickedRadioRunmode3)
	ON_BN_CLICKED(IDC_RADIO_DVCMODE1, &CSiteSetDlg_128Para::OnBnClickedRadioDvcmode1)
	ON_BN_CLICKED(IDC_RADIO_DVCMODE2, &CSiteSetDlg_128Para::OnBnClickedRadioDvcmode2)
	ON_BN_CLICKED(IDC_RADIO_DVCMODE3, &CSiteSetDlg_128Para::OnBnClickedRadioDvcmode3)
	ON_BN_CLICKED(IDC_SITESET_SAVE, &CSiteSetDlg_128Para::OnBnClickedSitesetSave)
	ON_BN_CLICKED(IDC_CHK_AUTO_RETEST, &CSiteSetDlg_128Para::OnBnClickedChkAutoRetest)
//	ON_EN_SETFOCUS(IDC_EDIT_OFFLINE_TEST_TIME, &CSiteSetDlg_128Para::OnEnSetfocusEditOfflineTestTime)
	ON_BN_CLICKED(IDC_BUTTON_ON, &CSiteSetDlg_128Para::OnBnClickedButtonOn)
	ON_BN_CLICKED(IDC_BUTTON_OFF, &CSiteSetDlg_128Para::OnBnClickedButtonOff)
	ON_EN_SETFOCUS(IDC_EDIT_OFFLINE_TEST_TIME2, &CSiteSetDlg_128Para::OnEnSetfocusEditOfflineTestTime2)
	ON_EN_SETFOCUS(IDC_EDIT_YIELD_CUT_PER, &CSiteSetDlg_128Para::OnEnSetfocusEditYieldCutPer)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CSiteSetDlg_128Para 메시지 처리기입니다.


BOOL CSiteSetDlg_128Para::OnInitDialog()
{
	CDialog::OnInitDialog();

	g_DMCont.m_dmHandlerOpt.SB(BDM11_GUI_OPEN_HISTORY, TRUE);
	
	int nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	memset(m_nSiteOnOff, 0x00, sizeof(m_nSiteOnOff));

	for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++) {
		for (int nRow = 0; nRow < m_TestSite_Div_Y; nRow++) {
			for (int nCol = 0; nCol < m_TestSite_Div_X; nCol++) {
				m_nSiteOnOff[nStation][nRow][nCol] = g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + (nStation * STATION_MAX_PARA/*m_TestSite_Div_Y * m_TestSite_Div_X*/) + (nRow * m_TestSite_Div_X) + nCol);
			}
		}
 	}

	//up/down
	m_bInitSpread = FALSE;
	OnBtnGUI();
	OnEditBoxGUI();
	InitSpreadHead();
	
	OnUpdateSpreadSite64Para();
	
	ChkLotState();

	m_ComboBox.SetCurSel(0);

	m_bInitSpread = TRUE;

	g_McComm.CmdModuleSiteOnPossible();

	
	GetDlgItem(IDC_CHK_ALWAY_BIN1_SORTING)->ShowWindow(FALSE);
	
	
	return TRUE;
}


void CSiteSetDlg_128Para::OnDestroy()
{
	CDialog::OnDestroy();

	KillTimer(0);

	g_DMCont.m_dmHandlerOpt.SB(BDM11_GUI_OPEN_HISTORY, FALSE);
}


void CSiteSetDlg_128Para::OnEditBoxGUI()
{
	m_editOffLineTestTime.SetBackColor(COLOR_WWHITE);
	m_editOffLineTestTime.SetTextSize(30);

	m_editOffLineYieldCut.SetBackColor(COLOR_WWHITE);
	m_editOffLineYieldCut.SetTextSize(30);
}

void CSiteSetDlg_128Para::InitSpreadHead()
{
	CString msg = "";
	CString msg1 = "";
	long SPREADHEADER = -1000;

	int nTestSite_Div_X = 0;

	m_SPRD_EnableSite.SetRow(SPREADHEADER);

	nTestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);

	m_SPRD_EnableSite.SetMaxRows(m_TestSite_Div_Y);
	m_SPRD_EnableSite.SetMaxCols( (eMaxPressUnitCount * nTestSite_Div_X));

	for (int x = 0; x < eMaxPressUnitCount; x++)
	{
		m_SPRD_EnableSite.AddCellSpan(x*nTestSite_Div_X + 1, 0, nTestSite_Div_X, 1);
		m_SPRD_EnableSite.SetCol(x*nTestSite_Div_X + 1);
		msg.Format("%d", x + 1);
		m_SPRD_EnableSite.SetText(msg);

	}

	m_SPRD_EnableSite.SetAutoSize(TRUE);
}

void CSiteSetDlg_128Para::OnUpdateSpreadSite64Para()
{
	for( int nStation = 0; nStation < eMaxPressUnitCount; nStation++)
	{
		for( int nRow = 0; nRow < m_TestSite_Div_Y; nRow++ )
		{
			for( int nCol = 0; nCol < m_TestSite_Div_X; nCol++)
			{
				SetSocketSite( nStation, nRow, nCol );
			}
		}
	}
}

void CSiteSetDlg_128Para::SetSocketSite(int _Station, int _Row, int _Col)
{
	int nSite_Col = 1 + _Col + ( _Station * m_TestSite_Div_X );
	//Down
	if (m_nSiteOnOff[_Station][_Row][_Col] == DEF_SITE_ON) {
		m_SPRD_EnableSite.SetTextMatrix(1 + _Row, nSite_Col, "1");
	}
	else if (m_nSiteOnOff[_Station][_Row][_Col] == DEF_SITE_OFF_RESERVATION) {
		m_SPRD_EnableSite.SetTextMatrix(1 + _Row, nSite_Col, "2");
	}
	else if (m_nSiteOnOff[_Station][_Row][_Col] == DEF_SITE_SOCKET_HOLD)
	{
		m_SPRD_EnableSite.SetTextMatrix(1 + _Row, nSite_Col, "3");
		m_SPRD_EnableSite.SetBackColor(COLOR_BBLACK);
	}
	else if (m_nSiteOnOff[_Station][_Row][_Col] == DEF_SITE_SOCKET_LOCK)
	{
		m_SPRD_EnableSite.SetTextMatrix(1 + _Row, nSite_Col, "4");
		m_SPRD_EnableSite.SetBackColor(COLOR_BLUE);
	}
	else {
		m_SPRD_EnableSite.SetTextMatrix(1 + _Row, nSite_Col, "0");
	}	
}

void CSiteSetDlg_128Para::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);

	if (m_nRunMode == 0)
	{
		m_BtnOnline.SetShade(CShadeButtonST::SHS_METAL, 30, 20, 10, RGB(0, 0, 0));
	}
	else
	{
		m_BtnOnline.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	}
	m_BtnOnline.SetIcon(IDI_BTN_ONLINE);
	m_BtnOnline.SetAlign(CButtonST::ST_ALIGN_VERT);
	if (m_nRunMode == 1)
	{
		m_BtnOffLine.SetShade(CShadeButtonST::SHS_METAL, 30, 20, 10, RGB(0, 0, 0));
	}
	else
	{
		m_BtnOffLine.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	}
	m_BtnOffLine.SetIcon(IDI_BTN_OFFLINE);
	m_BtnOffLine.SetAlign(CButtonST::ST_ALIGN_VERT);
	if (m_nRunMode == 2)
	{
		m_BtnDemo.SetShade(CShadeButtonST::SHS_METAL, 30, 20, 10, RGB(0, 0, 0));
	}
	else
	{
		m_BtnDemo.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	}
	m_BtnDemo.SetIcon(IDI_BTN_DEMO);
	m_BtnDemo.SetAlign(CButtonST::ST_ALIGN_VERT);

	if (m_nDeviceMode == 0)
	{
		m_BtnExistDvc.SetShade(CShadeButtonST::SHS_METAL, 30, 20, 10, RGB(0, 0, 0));
	}
	else
	{
		m_BtnExistDvc.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	}
	m_BtnExistDvc.SetIcon(IDI_BTN_EXIST_DVC);
	m_BtnExistDvc.SetAlign(CButtonST::ST_ALIGN_VERT);
	if (m_nDeviceMode == 1)
	{
		m_BtnNoDvcExistTray.SetShade(CShadeButtonST::SHS_METAL, 30, 20, 10, RGB(0, 0, 0));
	}
	else
	{
		m_BtnNoDvcExistTray.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	}
	m_BtnNoDvcExistTray.SetIcon(IDI_BTN_NO_DVC);
	m_BtnNoDvcExistTray.SetAlign(CButtonST::ST_ALIGN_VERT);
	if (m_nDeviceMode == 2)
	{
		m_BtnNoDvcNoTray.SetShade(CShadeButtonST::SHS_METAL, 30, 20, 10, RGB(0, 0, 0));
	}
	else
	{
		m_BtnNoDvcNoTray.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	}
	m_BtnNoDvcNoTray.SetIcon(IDI_BTN_NO_DVC_NO_TRAY);
	m_BtnNoDvcNoTray.SetAlign(CButtonST::ST_ALIGN_VERT);
}


void CSiteSetDlg_128Para::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}


void CSiteSetDlg_128Para::OnBnClickedRadioRunmode1()
{
	m_nRunMode = eRUN_MODE_ONLINE;
	OnBtnGUI();
}


void CSiteSetDlg_128Para::OnBnClickedRadioRunmode2()
{
	m_nRunMode = eRUN_MODE_OFFLINE;
	OnBtnGUI();
}


void CSiteSetDlg_128Para::OnBnClickedRadioRunmode3()
{
	m_nRunMode = eRUN_MODE_DEMO;
	OnBtnGUI();
}


void CSiteSetDlg_128Para::OnBnClickedRadioDvcmode1()
{
	m_nDeviceMode = eDeviceMode;
	OnBtnGUI();
}


void CSiteSetDlg_128Para::OnBnClickedRadioDvcmode2()
{
	m_nDeviceMode = eNoDeviceTrayExist;
	OnBtnGUI();
}


void CSiteSetDlg_128Para::OnBnClickedRadioDvcmode3()
{
	m_nDeviceMode = eNoDeviceNoTray;
	OnBtnGUI();
}


void CSiteSetDlg_128Para::OnBnClickedSitesetSave()
{	
	SiteSetSave();	
}

void CSiteSetDlg_128Para::SiteSetSave()
{
	for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++) {
		for (int nRow = 0; nRow < m_TestSite_Div_Y; nRow++) {
			for (int nCol = 0; nCol < m_TestSite_Div_X; nCol++) {		
				//DOWN SITE
				if (m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_HOLD || m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_LOCK)
					continue;

				if ("1" == m_SPRD_EnableSite.GetTextMatrix(1 + nRow, 1 + nCol + (nStation  * m_TestSite_Div_X)))
					m_nSiteOnOff[nStation][nRow][nCol] = DEF_SITE_ON;
				else if ("2" == m_SPRD_EnableSite.GetTextMatrix(1 + nRow, 1 + nCol + (nStation  * m_TestSite_Div_X)))
					m_nSiteOnOff[nStation][nRow][nCol] = DEF_SITE_OFF_RESERVATION;
				else
					m_nSiteOnOff[nStation][nRow][nCol] = DEF_SITE_OFF;
				
			}
		}
	}
	if (ChkSocketOnOffInterlock() == FALSE)
	{
		UpdateData(TRUE);

		char szCokName[512] = { 0, };
		ST_BASIC_INFO   stBasicData, stBeforeBasicData;
		g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
		g_COK_FileMgr.LoadBasicInfo(szCokName, stBasicData);

		stBeforeBasicData = stBasicData;

// 		for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++) {
// 			for (int nRow = 0; nRow < m_TestSite_Div_Y; nRow++) {
// 				for (int nCol = 0; nCol < m_TestSite_Div_X; nCol++) {
// 					g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + (nStation * STATION_MAX_PARA/*(m_TestSite_Div_X * m_TestSite_Div_Y)*/) + (nRow * m_TestSite_Div_X) + nCol, m_nSiteOnOff[nStation][nRow][nCol]);
// 					stBasicData.nSiteOnOff[nStation][nRow][nCol] = m_nSiteOnOff[nStation][nRow][nCol];
// 				}
// 			}
// 		}

		//Save부만 Index Convert하여 저장.
		//PassFail, Yield Tab과 배열 맞추기 위함.
// 		for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++) {
// 			for (int nCol = 0; nCol < 2; nCol++) {
// 				for (int nRow = 0; nRow < 4; nRow++) {
// //					g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + (nStation * STATION_MAX_PARA/*(m_TestSite_Div_X * m_TestSite_Div_Y)*/) + (nRow * m_TestSite_Div_X) + nCol, m_nSiteOnOff[nStation][nRow][nCol]);
// 
// 					int nNum = 0;
// 
// 					if (nStation <= ePressUnit_Down_4) {
// 						nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + abs(nRow - 3) + nCol * 4;
// 						stBasicData.nSiteOnOff[nStation][abs(nRow - 3)][nCol] = m_nSiteOnOff[nStation][nRow][nCol];
// 
// 				
// 					}
// 					else {
// 						nNum = NDM11_SocketOnOff_Site1_1 + STATION_MAX_PARA*nStation + nRow + abs(nCol - 1) * 4;
// 						stBasicData.nSiteOnOff[nStation][nRow][abs(nCol - 1)] = m_nSiteOnOff[nStation][nRow][nCol];
// 					}
// 
// 					g_DMCont.m_dmHandlerOpt.SN(nNum, m_nSiteOnOff[nStation][nRow][nCol]);
// 				}
// 			}
// 		}
		int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);

		for (int nStation = 0; nStation < eMaxPressUnitCount; nStation++) {
			for (int nRow = 0; nRow < STATION_MAX_Y_SITE; nRow++) {
				for (int nCol = 0; nCol < STATION_MAX_X_SITE; nCol++) {

					if(m_TestSite_Div_Y == 1)
					{ 
						if (nBoardType == eBoardDir_Forward)
						{
							if (nStation <= ePressUnit_Down_4)
								stBasicData.nSiteOnOff[nStation][abs(nCol - 3)][nRow] = m_nSiteOnOff[nStation][nRow][nCol]; //2, 4
							else
								stBasicData.nSiteOnOff[nStation][nCol][abs(nRow)] = m_nSiteOnOff[nStation][nRow][nCol]; //2, 4
						}
						else
						{
							//nBoardTypeBack
							if (nStation <= ePressUnit_Down_4)
								stBasicData.nSiteOnOff[nStation][nCol][abs(nRow)] = m_nSiteOnOff[nStation][nRow][nCol]; //2, 4
							else
								stBasicData.nSiteOnOff[nStation][abs(nCol - 3)][nRow] = m_nSiteOnOff[nStation][nRow][nCol]; //2, 4
						}
					}
					else
					{
						if (nBoardType == eBoardDir_Forward)
						{
							if (nStation <= ePressUnit_Down_4)
								stBasicData.nSiteOnOff[nStation][abs(nCol - 3)][nRow] = m_nSiteOnOff[nStation][nRow][nCol]; //2, 4
							else
								stBasicData.nSiteOnOff[nStation][nCol][abs(nRow - 1)] = m_nSiteOnOff[nStation][nRow][nCol]; //2, 4
						}
						else
						{
							//nBoardTypeBack
							if (nStation <= ePressUnit_Down_4)
								stBasicData.nSiteOnOff[nStation][nCol][abs(nRow - 1)] = m_nSiteOnOff[nStation][nRow][nCol]; //2, 4
							else
								stBasicData.nSiteOnOff[nStation][abs(nCol - 3)][nRow] = m_nSiteOnOff[nStation][nRow][nCol]; //2, 4
						}
					}
					//nBoardTypeForward
					
					g_DMCont.m_dmHandlerOpt.SN(NDM11_SocketOnOff_Site1_1 + (nStation * STATION_MAX_PARA/*(m_TestSite_Div_X * m_TestSite_Div_Y)*/) + (nRow * m_TestSite_Div_X) + nCol, m_nSiteOnOff[nStation][nRow][nCol]);
				}
			}
		}

		g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_Run, m_nRunMode);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_TM_OffLineTest, m_nOffLineTT * 1000);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_SYS_MD_DEVICE_CHK, m_nDeviceMode);
		g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_AutoRetestSiteUse, m_bAutoRetest_TestPP);
		g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_OFFLINEBIN_RANDOM, m_bOffLineBinRandom);
		g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_OFFLINEBIN_YIELDCUT, m_bOffLineYieldCut);
		g_DMCont.m_dmHandlerOpt.SD(DDM11_OffLine_YieldCutPer, m_dOffLineYieldCutPer);

		g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_AlwayBin1Sorting, m_bAlwayBin1Sorting);


		if (m_bAutoRetest_TestPP == DEF_ON)
		{
			g_DMCont.m_dmHandlerOpt.SN(NDM11_MD_AutoRetest, m_nAutoRetestKind);
			g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_Retest_Skip_HW1, m_bRetest_Skip_HW1);
			g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_Retest_Skip_HW2, m_bRetest_Skip_HW2);
			g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_Retest_Skip_HW3, m_bRetest_Skip_HW3);
			g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_Retest_Skip_HW4, m_bRetest_Skip_HW4);
		}

		stBasicData.nRunMode = m_nRunMode;
		stBasicData.nDeviceMode = m_nDeviceMode;
		stBasicData.nOffLineTestTm = m_nOffLineTT * 1000;
		stBasicData.bAutoRetestSiteUse = m_bAutoRetest_TestPP;


		if (m_bAutoRetest_TestPP == DEF_ON)
		{
			stBasicData.nAutoRetestKind = m_nAutoRetestKind;
			stBasicData.bRetestSkip_HW1 = m_bRetest_Skip_HW1;
			stBasicData.bRetestSkip_HW2 = m_bRetest_Skip_HW2;
			stBasicData.bRetestSkip_HW3 = m_bRetest_Skip_HW3;
			stBasicData.bRetestSkip_HW4 = m_bRetest_Skip_HW4;
		}

		stBasicData.bOffLineBinRandom = m_bOffLineBinRandom;
		stBasicData.bOffLineYieldCut = m_bOffLineYieldCut;
		stBasicData.dOffLineYieldCutPer = m_dOffLineYieldCutPer;

		stBasicData.bAlwayBin1Sorting = m_bAlwayBin1Sorting;


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

void CSiteSetDlg_128Para::OnBnClickedChkAutoRetest()
{
	UpdateData(TRUE);

	if (m_bAutoRetest_TestPP)
	{
		GetDlgItem(IDC_RADIO_AUTO_RETEST_A_ONLY)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_AUTO_RETEST_AB)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_AUTO_RETEST_AAB)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_AUTO_RETEST_AA)->EnableWindow(TRUE);
		m_nAutoRetestKind = eAutoRetest_AAB;

		GetDlgItem(IDC_CHK_RETEST_SKIP_HW1)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHK_RETEST_SKIP_HW2)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHK_RETEST_SKIP_HW3)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHK_RETEST_SKIP_HW4)->EnableWindow(TRUE);

		UpdateData(FALSE);
	}
	else
	{
		GetDlgItem(IDC_RADIO_AUTO_RETEST_A_ONLY)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTO_RETEST_AB)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTO_RETEST_AAB)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTO_RETEST_AA)->EnableWindow(FALSE);

		GetDlgItem(IDC_CHK_RETEST_SKIP_HW1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_RETEST_SKIP_HW2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_RETEST_SKIP_HW3)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_RETEST_SKIP_HW4)->EnableWindow(FALSE);
	}
}

void CSiteSetDlg_128Para::MakeLog(LPCTSTR fmt, ...)
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

void CSiteSetDlg_128Para::SiteSetChangeLog(std::vector<std::pair<CString, std::pair<CString, CString>>> vData)
{
	for (int i = 0; i < (int)vData.size(); i++) {
		std::pair<CString, std::pair<CString, CString>> p;

		p = vData[i];

		MakeLog("[MENU = Site Setting, DATA NAME = %s ][BEFORE=%s, AFTER=%s]", p.first, p.second.first, p.second.second);
	}
}

void CSiteSetDlg_128Para::ChkLotState()
{
	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
	BOOL bRetOneCycle = g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY);
	int nLotMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	int nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);
	int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
	if (bLotState == eHandlerLotMode_OnGoing)
	{
		GetDlgItem(IDC_RADIO_RUNMODE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RUNMODE2)->EnableWindow(FALSE);

		GetDlgItem(IDC_RADIO_DVCMODE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_DVCMODE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_DVCMODE3)->EnableWindow(FALSE);

		//GetDlgItem(IDC_EDIT_OFFLINE_TEST_TIME2)->EnableWindow(FALSE);

		GetDlgItem(IDC_COMBO_BOX)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_BUTTON_ON)->EnableWindow(bRetOneCycle);
		GetDlgItem(IDC_BUTTON_OFF)->EnableWindow(bRetOneCycle);


		GetDlgItem(IDC_CHK_AUTO_RETEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTO_RETEST_A_ONLY)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTO_RETEST_AB)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTO_RETEST_AAB)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AUTO_RETEST_AA)->EnableWindow(FALSE);

		//GetDlgItem(IDC_SPREAD_ENABLE_SITE)->EnableWindow(bRetOneCycle);
		//GetDlgItem(IDC_SPREAD_ENABLE_SITE2)->EnableWindow(bRetOneCycle);

		GetDlgItem(IDC_CHK_RETEST_SKIP_HW1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_RETEST_SKIP_HW2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_RETEST_SKIP_HW3)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_RETEST_SKIP_HW4)->EnableWindow(FALSE);
	}
	else {

		GetDlgItem(IDC_RADIO_RUNMODE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RUNMODE2)->EnableWindow(TRUE);

		GetDlgItem(IDC_RADIO_DVCMODE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_DVCMODE2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_DVCMODE3)->EnableWindow(TRUE);

		//GetDlgItem(IDC_EDIT_OFFLINE_TEST_TIME2)->EnableWindow(TRUE);

		GetDlgItem(IDC_COMBO_BOX)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ON)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_OFF)->EnableWindow(TRUE);


		GetDlgItem(IDC_CHK_AUTO_RETEST)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_AUTO_RETEST_A_ONLY)->EnableWindow(m_bAutoRetest_TestPP);
		GetDlgItem(IDC_RADIO_AUTO_RETEST_AB)->EnableWindow(m_bAutoRetest_TestPP);
		GetDlgItem(IDC_RADIO_AUTO_RETEST_AAB)->EnableWindow(m_bAutoRetest_TestPP);
		GetDlgItem(IDC_RADIO_AUTO_RETEST_AA)->EnableWindow(m_bAutoRetest_TestPP);
		GetDlgItem(IDC_SPREAD_ENABLE_SITE)->EnableWindow(TRUE);
		//GetDlgItem(IDC_SPREAD_ENABLE_SITE2)->EnableWindow(TRUE);

		GetDlgItem(IDC_CHK_RETEST_SKIP_HW1)->EnableWindow(m_bAutoRetest_TestPP);
		GetDlgItem(IDC_CHK_RETEST_SKIP_HW2)->EnableWindow(m_bAutoRetest_TestPP);
		GetDlgItem(IDC_CHK_RETEST_SKIP_HW3)->EnableWindow(m_bAutoRetest_TestPP);
		GetDlgItem(IDC_CHK_RETEST_SKIP_HW4)->EnableWindow(m_bAutoRetest_TestPP);
	}

	if (nLotMode == eStartMD_AutoLoop_Continue)
	{
		GetDlgItem(IDC_SPREAD_ENABLE_SITE)->EnableWindow(FALSE);
		//GetDlgItem(IDC_SPREAD_ENABLE_SITE2)->EnableWindow(FALSE);
	}

}

void CSiteSetDlg_128Para::OnEnSetfocusEditOfflineTestTime()
{

}

void CSiteSetDlg_128Para::OnBnClickedButtonOn()
{
	SetComboSettingToSprd(DEF_SITE_ON);
}

void CSiteSetDlg_128Para::OnBnClickedButtonOff()
{
	SetComboSettingToSprd(DEF_SITE_OFF);
}

void CSiteSetDlg_128Para::SetComboSettingToSprd(BOOL bOnOff)
{

	m_bInitSpread = FALSE;
	CString strOnOff = _T("");
	if (bOnOff == DEF_SITE_ON)
		strOnOff = "1";
	else if (bOnOff == DEF_SITE_OFF_RESERVATION)
		strOnOff = "2";
	else
		strOnOff = "0";

	int nSelect = m_ComboBox.GetCurSel();

	switch (nSelect)
	{
		case STATION1:  SetSocketOnOFF( bOnOff , nSelect ); break;
		case STATION2:  SetSocketOnOFF(	bOnOff , nSelect ); break;
		case STATION3:  SetSocketOnOFF(	bOnOff , nSelect ); break;
		case STATION4:	SetSocketOnOFF(	bOnOff , nSelect ); break;
		case STATION5:  SetSocketOnOFF(	bOnOff , nSelect ); break;
		case STATION6:  SetSocketOnOFF(	bOnOff , nSelect ); break;
		case STATION7:  SetSocketOnOFF(	bOnOff , nSelect ); break;
		case STATION8:	SetSocketOnOFF(	bOnOff , nSelect ); break;			
		case MODULE1:
			{
				for (int nStation = 0; nStation < eMaxPressUnitCount / 2; nStation++) {
					for (int nRow = 0; nRow < m_TestSite_Div_Y; nRow++) {
						for (int nCol = 0; nCol < m_TestSite_Div_X; nCol++) {
							if (m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_HOLD || m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_LOCK)
								continue;

							m_SPRD_EnableSite.SetTextMatrix(1 + nRow, (nStation* m_TestSite_Div_X) + nCol + 1, strOnOff);
							m_nSiteOnOff[nStation][nRow][nCol] = bOnOff;
						}
					}
				}

				for (int nStation = 0; nStation < eMaxPressUnitCount / 2; nStation++) {
					for (int nRow = 0; nRow < STATION_MAX_Y_SITE; nRow++) {
						for (int nCol = 0; nCol < STATION_MAX_X_SITE; nCol++) {
							if (m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_HOLD)
								continue;

							m_SPRD_EnableSite.SetTextMatrix(1 + nRow, (nStation * m_TestSite_Div_X) + nCol + 1, strOnOff);
							m_nSiteOnOff[nStation][nRow][nCol] = bOnOff;
						}
					}
				}
			}
			break;
		case MODULE2:
			{
				for (int nStation = eMaxPressUnitCount / 2; nStation < eMaxPressUnitCount; nStation++) {
					for (int nRow = 0; nRow < m_TestSite_Div_Y; nRow++) {
						for (int nCol = 0; nCol < m_TestSite_Div_X; nCol++) {
							if (m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_HOLD || m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_LOCK)
								continue;

							m_SPRD_EnableSite.SetTextMatrix(1 + nRow, (nStation* m_TestSite_Div_X) + nCol + 1, strOnOff);
							m_nSiteOnOff[nStation][nRow][nCol] = bOnOff;
						}
					}
				}

				for (int nStation = eMaxPressUnitCount / 2; nStation < eMaxPressUnitCount; nStation++) {
					for (int nRow = 0; nRow < STATION_MAX_Y_SITE; nRow++) {
						for (int nCol = 0; nCol < STATION_MAX_X_SITE; nCol++) {
							if (m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_HOLD || m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_LOCK)
								continue;

							m_SPRD_EnableSite.SetTextMatrix(1 + nRow, (nStation * m_TestSite_Div_X) + nCol + 1, strOnOff);
							m_nSiteOnOff[nStation][nRow][nCol] = bOnOff;
						}
					}
				}
			}
			break;
		case ALL:
			{
				for( int nStation = 0; nStation < eMaxPressUnitCount; nStation++ ) {
					for( int nRow = 0; nRow < m_TestSite_Div_Y; nRow++ ) {
						for (int nCol = 0; nCol < m_TestSite_Div_X; nCol++) {
							if(m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_HOLD || m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_SOCKET_LOCK)
								continue;

							m_SPRD_EnableSite.SetTextMatrix(1 + nRow, (nStation  * m_TestSite_Div_X) + nCol + 1, strOnOff);
						
						
							m_nSiteOnOff[nStation][nRow][nCol] = bOnOff;
						}
					}
				}

			}
			break;
	}
	m_bInitSpread = TRUE;
}

void CSiteSetDlg_128Para::SetSocketOnOFF( BOOL bOnOff, int _nStation )
{
	CString strOnOff = _T("");

	if (bOnOff == DEF_SITE_ON)
		strOnOff = "1";
	else if (bOnOff == DEF_SITE_OFF_RESERVATION)
		strOnOff = "2";
	else
		strOnOff = "0";

	for( int nRow = 0; nRow < m_TestSite_Div_Y; nRow++ ) {
		for (int nCol = 0; nCol < m_TestSite_Div_X; nCol++) {
			if (m_nSiteOnOff[_nStation][nRow][nCol] == DEF_SITE_SOCKET_HOLD || m_nSiteOnOff[_nStation][nRow][nCol] == DEF_SITE_SOCKET_LOCK)
				continue;
// 				for (int nStation = 0; nStation < eMaxPressUnitCount / 2; nStation++) {
// 					for (int nRow = 0; nRow < m_TestSite_Div_Y; nRow++) {
// 						for (int nCol = 0; nCol < m_TestSite_Div_X; nCol++) {
// 							if (m_nSiteOnOff[nStation+ eMaxPressUnitCount / 2][nRow][nCol] == DEF_SITE_SOCKET_HOLD || m_nSiteOnOff[nStation + eMaxPressUnitCount / 2][nRow][nCol] == DEF_SITE_SOCKET_LOCK)
// 								continue;
// 
// 							m_SPRD_EnableSite.SetTextMatrix(1 + nRow, (nStation * m_TestSite_Div_X) + nCol + 1, strOnOff);
// 							m_nSiteOnOff[nStation+ eMaxPressUnitCount / 2][nRow][nCol] = bOnOff;
// 						}
// 					}
// 				}

			m_SPRD_EnableSite.SetTextMatrix(1 + nRow, nCol + 1 + ( _nStation * m_TestSite_Div_X), strOnOff);

			m_nSiteOnOff[_nStation][nRow][nCol] = bOnOff;
		}
	}
}

BOOL CSiteSetDlg_128Para::ChkSocketOnOffInterlock()
{
	CString strErrMsg;
	CString strErrStation = _T("");
	std::vector<int> vErrEachOneSock;
	std::vector<int> vDownSocket;
	std::vector<int> vUpSocket;

	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	if (ChkAllStationOFF() == TRUE) {
		strErrMsg.Format(_T("It can not be saved when the whole Socket is turned off."));
		AfxMessageBox(strErrMsg, MB_ICONHAND | MB_TOPMOST);
		return TRUE;
	}

	int nHalf_Site = eMaxATS_Area;

	for( int nTotal_cnt = 0; nTotal_cnt < eMaxTestPPCount; nTotal_cnt++ )
	{
		vDownSocket.clear();
		vUpSocket.clear();
		
		strErrStation.Format(_T("%d, %d, %d, %d"), 
							(nTotal_cnt * nHalf_Site ) + 1,
							(nTotal_cnt * nHalf_Site ) + 2,
							(nTotal_cnt * nHalf_Site ) + 3,
							(nTotal_cnt * nHalf_Site ) + 4
							//(nTotal_cnt * nHalf_Site ) + 9,
							//(nTotal_cnt * nHalf_Site ) + 10,
							//(nTotal_cnt * nHalf_Site ) + 11,
							/*(nTotal_cnt * nHalf_Site ) + 12*/ );

		for( int nStation = 0; nStation < nHalf_Site; nStation++ ) {
			for( int nRow = 0; nRow < m_TestSite_Div_Y; nRow++) {
				for (int nCol = 0; nCol < m_TestSite_Div_X; nCol++) {
					vDownSocket.push_back( m_nSiteOnOff[ nStation + (nTotal_cnt * nHalf_Site ) ][nRow][nCol]);
				}
			}
		}
		
		if (Chk8StationSocketOff(vDownSocket) == FALSE) //8개 Station이 OFF 라면 FALSE 검사 시작.
		{
			if (ChkEachOneMoreSocketOn(vDownSocket) == TRUE) //8개 Station Sokcet이 1개이상 씩 켜져 있다면 TRUE 아니라면 FALSE
			{		
				if (ChkRetestTwoMoreStaionOn(vDownSocket) == FALSE) {
					strErrMsg.Format(_T("Please turn on more than 2 stations in retest mode.\r\nError Station : %s"), strErrStation);
					AfxMessageBox(strErrMsg, MB_ICONHAND | MB_TOPMOST);
					return TRUE;
				}			
			}
			else {
 				ChkErrEachOneMoreSocketOn(vDownSocket, vErrEachOneSock);
 				strErrMsg.Format(_T("Please turn on at least one socket with 4 stations.\r\nError Station : %s\r\nError Socket : "), strErrStation);
 				int b =vErrEachOneSock.size();
//  					for (int k = 0; k < (int)vErrEachOneSock.size(); k++) {
//  						CString temp;
//  
//  						if (k != vErrEachOneSock.size() - 1)
//  							temp.Format(_T("%d, "), vErrEachOneSock[k]);
//  						else
//  							temp.Format(_T("%d"), vErrEachOneSock[k]);
//  
//  						strErrMsg += temp;
// 							AfxMessageBox(strErrMsg, MB_ICONHAND | MB_TOPMOST);
// 						return TRUE;
//  					}				
			}
		}
	}

	vDownSocket.clear();
	vUpSocket.clear();

	return FALSE;
}

BOOL CSiteSetDlg_128Para::Chk8StationSocketOff(std::vector<int> vDown)
{
	int nSum = 0;
	for (int i = 0; i < (int)vDown.size(); i++) {
		if (vDown[i] == DEF_SITE_ON) { nSum++; }
	}

	return nSum == 0 ? TRUE : FALSE;
}

BOOL CSiteSetDlg_128Para::ChkEachOneMoreSocketOn(std::vector<int> vDown)
{
	int szSock[STATION_MAX_PARA] = {0,};

	int nSocketOnCount = 0;
	int nStation_Max_para = m_TestSite_Div_X * m_TestSite_Div_Y;

	for (int i = 0; i < (int)vDown.size(); i++) {
		switch (i % nStation_Max_para)
		{
		case 0:	ChkEachOneMoreSocketState( vDown[i],   szSock[0] );	break;
		case 1:	ChkEachOneMoreSocketState( vDown[i],   szSock[1] );	break;
		case 2:	ChkEachOneMoreSocketState( vDown[i],   szSock[2] );	break;
		case 3:	ChkEachOneMoreSocketState( vDown[i],   szSock[3] );	break;
		case 4:	ChkEachOneMoreSocketState( vDown[i],   szSock[4] );	break;
		case 5:	ChkEachOneMoreSocketState( vDown[i],   szSock[5] );	break;
		case 6:	ChkEachOneMoreSocketState( vDown[i],   szSock[6] );	break;
		case 7:	ChkEachOneMoreSocketState( vDown[i],   szSock[7] );	break;
		}
	}

	for( int nCnt = 0; nCnt < nStation_Max_para; nCnt++ ) {
		if( szSock[nCnt] > 0 )
			nSocketOnCount++;
	}

	if ( nSocketOnCount == nStation_Max_para )
		return TRUE;
	else
		return FALSE;
}

void CSiteSetDlg_128Para::ChkEachOneMoreSocketState(int _Downstation, int& _Socket_number)
{
	if (_Downstation == DEF_SITE_ON) { _Socket_number++; }
}

BOOL CSiteSetDlg_128Para::ChkRetestTwoMoreStaionOn(std::vector<int> vDown)
{
	if (m_bAutoRetest_TestPP == FALSE)
		return TRUE;

	int nDownStation[ eMaxATS_Area] = { 0, };
	int nUpStation[ eMaxATS_Area] = { 0, };
	int nSum = 0;

	for (int i = 0; i < eMaxATS_Area; i++) {
		ChkRetestTwoMoreStaionState(i, vDown,nDownStation[i]);
	}

	for( int nCnt = 0; nCnt < ( eMaxATS_Area); nCnt++) {
		nSum += nDownStation[nCnt];
	}

	if (2 <= nSum)
		return TRUE;
	else
		return FALSE;
}

void CSiteSetDlg_128Para::ChkRetestTwoMoreStaionState( int _index, std::vector<int> _vDown, int& _DownStation)
{
	int DownSocket = 0, UpSocket = 0;
	int TestSite_Div = m_TestSite_Div_X*m_TestSite_Div_Y;
	for( int i = 0; i < TestSite_Div; i++ )
	{
		if( _vDown[ ( _index * ( m_TestSite_Div_X*m_TestSite_Div_Y ) ) + i ] == DEF_SITE_ON ) { DownSocket++; }
		//if( _vUp[ ( _index * ( m_TestSite_Div_X*m_TestSite_Div_Y ) ) + i ] == DEF_SITE_ON ) { UpSocket++; }
	}

	if(DownSocket != FALSE) 
		_DownStation = TRUE;
// 	if(UpSocket != FALSE) 
// 		_UpStation = TRUE;
}

BOOL CSiteSetDlg_128Para::ChkAllStationOFF()
{
	for( int nStation = 0; nStation < eMaxPressUnitCount; nStation++) {
		for( int nRow = 0; nRow < m_TestSite_Div_Y; nRow++ ) {
			for (int nCol = 0; nCol < m_TestSite_Div_X; nCol++) {
				if (m_nSiteOnOff[nStation][nRow][nCol] == DEF_SITE_ON)
					return FALSE;
			}
		}
	}
	return TRUE;
}

void CSiteSetDlg_128Para::ChkErrEachOneMoreSocketOn(std::vector<int> vDown, std::vector<int> &vSocket)
{
	int nSock[STATION_MAX_PARA] = {0,};

	int nStation_Max_Para = m_TestSite_Div_X * m_TestSite_Div_Y;

	for (int i = 0; i < nStation_Max_Para; i++)
	{
		switch (i % nStation_Max_Para)
		{
		case 0 :ChkErrEachOneMoreSocketState(i, vDown, nSock[0]); break;
		case 1 :ChkErrEachOneMoreSocketState(i, vDown, nSock[1]); break;
		case 2 :ChkErrEachOneMoreSocketState(i, vDown, nSock[2]); break;
		case 3 :ChkErrEachOneMoreSocketState(i, vDown, nSock[3]); break;
		case 4 :ChkErrEachOneMoreSocketState(i, vDown, nSock[4]); break;
		case 5 :ChkErrEachOneMoreSocketState(i, vDown, nSock[5]); break;
		case 6 :ChkErrEachOneMoreSocketState(i, vDown, nSock[6]); break;
		case 7 :ChkErrEachOneMoreSocketState(i, vDown, nSock[7]); break;
		}
	}

	for( int nCnt = 0; nCnt < nStation_Max_Para; nCnt++ ) {
		if( nSock[nCnt] < 1 )
			vSocket.push_back( nCnt + 1 );
	}
}

void CSiteSetDlg_128Para::ChkErrEachOneMoreSocketState(int _index, std::vector<int> _vDown, int &_Socket )
{
	if (_vDown[_index] == DEF_SITE_ON) { _Socket++; }
	if (_vDown[_index + (m_TestSite_Div_X*m_TestSite_Div_Y)] == DEF_SITE_ON) { _Socket++; }
// 	if (_vDown[_index + ((m_TestSite_Div_X*m_TestSite_Div_Y) * 2)] == DEF_SITE_ON) { _Socket++; }
// 	if (_vDown[_index + ((m_TestSite_Div_X*m_TestSite_Div_Y) * 3)] == DEF_SITE_ON) { _Socket++; }

// 	if (_vUp[_index] == DEF_SITE_ON) { _Socket++; }
// 	if (_vUp[_index + (m_TestSite_Div_X*m_TestSite_Div_Y)] == DEF_SITE_ON) { _Socket++; }
// 	if (_vUp[_index + ((m_TestSite_Div_X*m_TestSite_Div_Y) * 2)] == DEF_SITE_ON) { _Socket++; }
// 	if (_vUp[_index + ((m_TestSite_Div_X*m_TestSite_Div_Y) * 3)] == DEF_SITE_ON) { _Socket++; }
}

void CSiteSetDlg_128Para::OnEnSetfocusEditOfflineTestTime2()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nOffLineTT, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Off Line/Demo Test Time", "99999999", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_OFFLINE_TEST_TIME2)->SetWindowText(szRetVal);
	}
}

BEGIN_EVENTSINK_MAP( CSiteSetDlg_128Para, CDialogEx )

	ON_EVENT(CSiteSetDlg_128Para, IDC_SPREAD_ENABLE_SITE, 5, CSiteSetDlg_128Para::ButtonClickedSpreadEnableSite, VTS_I4 VTS_I4)
	ON_EVENT(CSiteSetDlg_128Para, IDC_SPREAD_ENABLE_SITE, 11, CSiteSetDlg_128Para::DblClickSpreadEnableSite, VTS_I4 VTS_I4)

END_EVENTSINK_MAP()

void CSiteSetDlg_128Para::ButtonClickedSpreadEnableSite( long Col, long Row/*, short ButtonDown*/ )
{

	if (ChkModuleUseAble(Col) != TRUE && g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY) != TRUE) {
		AfxMessageBox("This Module unit motor is not use able. Please check it.");
		SetTimer(1, 10, NULL);
	}
	else {

		if ((Row == 1 || Row == 2) && m_bInitSpread)
		{
			int nStation_Idx = (Col - 1) / m_TestSite_Div_X;
			int nCol = (Col - 1) % m_TestSite_Div_X;

			int nChk = m_nSiteOnOff[ePressUnit_Down_1 + nStation_Idx][Row - 1][nCol];
			BOOL bIsTrue = FALSE;
			int nOnOff = nChk;

			if (g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY) != TRUE)
			{
				if (nChk != DEF_SITE_ON)
				{
					int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
					if (nRunMode == eRUN_MODE_ONLINE) {
						BOOL bConnt = TesterConnectChk(nStation_Idx, Row, nCol);
						//if (bConnt) {
						//	bIsTrue = TRUE;
						//}
						if (bConnt == FALSE) {
							AfxMessageBox("Can not site on. Tester is disconnection. Please check ");
							SetTimer(1, 10, NULL);
						}
					}
					//else if (nRunMode == eRUN_MODE_OFFLINE) {
					//	bIsTrue = TRUE;
					//}
				}
				//else {
				//	//int nCol = Col % 4 == 0 ? 4 : Col % 4;
				//	//
				//	//if (g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + (nStation_Idx * (m_TestSite_Div_X * m_TestSite_Div_Y)) + ((Row - 1) * m_TestSite_Div_X) + (nCol - 1)) != DEF_SITE_ON) {
				//	//	bIsTrue = TRUE;
				//	//}
				//	if (nOnOff == DEF_SITE_ON)
				//	{
				//		nOnOff = DEF_SITE_OFF_RESERVATION;
				//	}
				//	else if (nOnOff == DEF_SITE_OFF)
				//	{
				//		nOnOff = DEF_SITE_ON;
				//	}
				//	else if (nOnOff == DEF_SITE_OFF_RESERVATION) {
				//		nOnOff = DEF_SITE_ON;
				//	}
				//
				//}
			}
			//else
			//{
			//	bIsTrue = TRUE;
			//}

			//if (bIsTrue) {
			//	if (nChk != DEF_SITE_ON)
			//	{
			//		nOnOff = DEF_SITE_ON;
			//	}
			//	else
			//	{
			//		nOnOff = DEF_SITE_OFF;
			//	}
			//}\

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

					if (g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + (nStation_Idx * (m_TestSite_Div_X * m_TestSite_Div_Y)) + ((Row - 1) * m_TestSite_Div_X) + (nCol - 1)) == DEF_SITE_OFF) {
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

			m_nSiteOnOff[ePressUnit_Down_1 + nStation_Idx][Row - 1][nCol] = nOnOff;

			SetTimer(1, 10, NULL);

			//if (bIsTrue == FALSE) {
			//	AfxMessageBox("Please. After 'One Cycle' do Change Site On/Off");
			//	SetTimer(1, 500, NULL);
			//}
		}
		else if (Row == 0) //Station Num 클릭
		{
			int nStation_Idx = (Col - 1) / m_TestSite_Div_X;

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
						BOOL bConnt = TesterConnectChk(nStation_Idx, Row, Col);
						//BOOL bConnt = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + nStation_Idx);
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
					int n = g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + nStation_Idx * 8);
					if (g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + nStation_Idx * 8) == DEF_SITE_OFF)
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


			for (int nRow = 0; nRow < m_TestSite_Div_Y; nRow++)
				for (int nCol = 0; nCol < m_TestSite_Div_X; nCol++)
				{
					m_nSiteOnOff[nStation_Idx][nRow][nCol] = nMake_Site_Status;
				}





			// 			int nStation_Idx = (Col - 1) / m_TestSite_Div_X;
			// 			int nChk = m_nSiteOnOff[ePressUnit_Down_1 + nStation_Idx][Row][(Col - 1) % m_TestSite_Div_X];
			// 			BOOL bIsTrue = FALSE;
			// 			int nOnOff = nChk;
			// 
			// 			if (g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY) != TRUE)
			// 			{
			// 				int nRunMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_Run);
			// 				if (nRunMode == eRUN_MODE_ONLINE) {
			// 					BOOL bConnt = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + nStation_Idx);
			// 					if (bConnt) {
			// 						bIsTrue = TRUE;
			// 					}
			// 				}
			// 				else if (nRunMode == eRUN_MODE_OFFLINE) {
			// 					bIsTrue = TRUE;
			// 				}
			// 			}
			// 			else
			// 			{
			// 				bIsTrue = TRUE;
			// 			}
			// 
			// 			if (bIsTrue) {
			// 				if (bIsTrue) {
			// 					for (int nY = 0; nY < m_TestSite_Div_Y; nY++)
			// 					{
			// 						for (int nX = 0; nX < m_TestSite_Div_X; nX++)
			// 						{
			// 							m_pSPRD_EnableSite2Buffer->SetTextMatrix(nY + 1, nStation_Idx*m_TestSite_Div_X + nX + 1, "1");
			// 							m_nSiteOnOff[ePressUnit_Down_1 + nStation_Idx][nY][nY] = DEF_SITE_ON;
			// 						}
			// 					}
			// 				}
			// 				else {
			// 					AfxMessageBox("Please. After 'One Cycle' do Change Site On/Off");
			// 					SetTimer(1, 500, NULL);
			// 				}
			// 			}
		}
	}

	
}



BOOL CSiteSetDlg_128Para::IsSite_On_Of_All(int nStationNum) //하나라도 On 이면 TRUE
{
   for(int nRow = 0; nRow < m_TestSite_Div_Y ; nRow ++)
	   for (int nCol = 0; nCol < m_TestSite_Div_X; nCol++)
	   {
		   if (DEF_SITE_ON == m_nSiteOnOff[nStationNum][nRow][nCol])
			   return TRUE;
	   }

   return FALSE;
}
BOOL CSiteSetDlg_128Para::IsSite_Off_Of_All(int nStationNum) //하나라도 Off 이면 TRUE
{
	for (int nRow = 0; nRow < m_TestSite_Div_Y; nRow++)
		for (int nCol = 0; nCol < m_TestSite_Div_X; nCol++)
		{
			if (DEF_SITE_OFF == m_nSiteOnOff[nStationNum][nRow][nCol])
				return TRUE;
		}

	return FALSE;
}
BOOL CSiteSetDlg_128Para::IsSite_Reserve_Of_All(int nStationNum) //하나라도 reserve 이면 TRUE
{
	for (int nRow = 0; nRow < m_TestSite_Div_Y; nRow++)
		for (int nCol = 0; nCol < m_TestSite_Div_X; nCol++)
		{
			if (DEF_SITE_OFF_RESERVATION == m_nSiteOnOff[nStationNum][nRow][nCol])
				return TRUE;
		}

	return FALSE;
}

BOOL CSiteSetDlg_128Para::IsSite_Hold_Of_All(int nStationNum) //하나라도 Hold 이면 TRUE
{
	for (int nRow = 0; nRow < m_TestSite_Div_Y; nRow++)
		for (int nCol = 0; nCol < m_TestSite_Div_X; nCol++)
		{
			if (DEF_SITE_SOCKET_HOLD == m_nSiteOnOff[nStationNum][nRow][nCol] /*|| DEF_SITE_SOCKET_LOCK == m_nSiteOnOff[nStationNum][nRow][nCol]*/)
				return TRUE;
		}

	return FALSE;
}

void CSiteSetDlg_128Para::OnEnSetfocusEditYieldCutPer()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dOffLineYieldCutPer);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "OffLine Yield Cut", "100", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_YIELD_CUT_PER)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}

BOOL CSiteSetDlg_128Para::ChkModuleUseAble(int nCol)
{
	BOOL bIsUseAble = FALSE;

	if ((eMaxPressUnitCount * m_TestSite_Div_X) / 2 >= nCol) {
		bIsUseAble = g_DMCont.m_dmEQP.GB(BDM0_MODULE1_SITE_ON_IS_POSSIBLE);
	}
	else {
		bIsUseAble = g_DMCont.m_dmEQP.GB(BDM0_MODULE2_SITE_ON_IS_POSSIBLE);
	}

	return bIsUseAble;
}

void CSiteSetDlg_128Para::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	this->SetFocus();	 
	KillTimer(nIDEvent);
	
	CDialogEx::OnTimer(nIDEvent);
}


void CSiteSetDlg_128Para::DblClickSpreadEnableSite2(long Col, long Row)
{
	SetTimer(1, 10, NULL);
}


void CSiteSetDlg_128Para::DblClickSpreadEnableSite(long Col, long Row)
{
	SetTimer(1, 10, NULL);
}

int CSiteSetDlg_128Para::TesterConnectChk(int nStation, int nX, int nY) //하나라도 꺼져있거나 다 꺼져있으면 On , 다 켜져있으면 Off
{
	BOOL bConnect = FALSE;
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);

	if (nTesterOption == eTester_Single)
		bConnect = g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + nStation);
	else if (nTesterOption == eTester_EachSocket)
	{
		int nTesterConnectCnt = 0; int nStationHalf = 0;

		if (nX == 2)
			nStationHalf = STATION_MAX_X_SITE;

		for (int nTesterCnt = 0; nTesterCnt < EACH_LINE_STATION_CNT; nTesterCnt++)
		{
			int nTesterIdx = (((nStation)* STATION_MAX_PARA) + nTesterCnt + nStationHalf);
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
		bConnect |= g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + ((nStation * 2) - 1) + 1);
		bConnect |= g_DMCont.m_dmEQP.GB(BDM0_TESTER_CONNECTION_1 + (nStation * 2) + 1);
	}
	else
		bConnect = g_DMCont.m_dmEQP.GB(BDM0_AGENT_CONNECTION_1 + nStation);

	return bConnect;
}

