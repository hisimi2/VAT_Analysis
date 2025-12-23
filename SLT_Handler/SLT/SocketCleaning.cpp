// SocketCleaning.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "SocketCleaning.h"
#include "afxdialogex.h"


// CSocketCleaning 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSocketCleaning, CDialog)

CSocketCleaning::CSocketCleaning(CWnd* pParent /*=NULL*/)
	: CDialog(CSocketCleaning::IDD, pParent)
{
    m_nCleaningOff	   = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_SocketClean);
    m_nContactCnt      = g_DMCont.m_dmHandlerOpt.GN(NDM11_SockClnContactCnt);
    m_nAlarmCnt        = g_DMCont.m_dmHandlerOpt.GN(NDM11_SockClnAlarmCnt);
    m_nIntervalCnt     = g_DMCont.m_dmHandlerOpt.GN(NDM11_SockClnIntervalCnt); 
	m_bEventType       = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_CleanDev_Event_Type);
	m_dContactDelayTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_SockClnContactDelayTime);
	
	m_dContactTime = 0.0;

	memset(&nSiteOnOff, 0, sizeof(nSiteOnOff));

	for (size_t i = 0; i < eMaxTestPPCount; i++)
	{
		m_vModuleSetPressIdx[i].clear();
	}
}

CSocketCleaning::~CSocketCleaning()
{
}

void CSocketCleaning::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CLEANING_SAVE, m_BtnSave);
    DDX_Control(pDX, IDCANCEL, m_BtnCancel);
    DDX_Radio(pDX  , IDC_RADIO_CLEAN_OFF, m_nCleaningOff);

    DDX_Control(pDX, IDC_CHK_CLN_MD1, m_CtrlChkInitialStart);
	DDX_Control(pDX, IDC_CHK_CLN_MD2, m_CtrlChkInitialRetestStart);
    DDX_Control(pDX, IDC_CHK_CLN_MD4, m_CtrlChkInterval);

	DDX_Text(pDX   , IDC_EDIT_ALARM_CNT, m_nAlarmCnt);
    DDX_Text(pDX   , IDC_EDIT_CONTACT_CNT, m_nContactCnt);
	DDX_Text(pDX   , IDC_EDIT_INTERVAL_CNT, m_nIntervalCnt);
	DDX_Text(pDX, IDC_EDIT_CONTACT_DELAY_TIME, m_dContactDelayTime);

	DDX_Control(pDX, IDC_EDIT_ALARM_CNT  , m_EditAlarmCnt     );
	DDX_Control(pDX, IDC_EDIT_CONTACT_CNT, m_EditContactCnt   );
	DDX_Control(pDX, IDC_EDIT_INTERVAL_CNT, m_EditIntervalCnt );
	DDX_Control(pDX, IDC_EDIT_CONTACT_DELAY_TIME, m_EditContactTime);

	DDX_Control(pDX, IDC_SPREAD_CLN_MOD1, m_SprdClnDvcPos1);
	DDX_Control(pDX, IDC_SPREAD_CLN_MOD2, m_SprdClnDvcPos2);

	DDX_Control(pDX, IDC_SPREAD_CLN_CNT1, m_SprdClnDvcTestCnt1);
	DDX_Control(pDX, IDC_SPREAD_CLN_CNT2, m_SprdClnDvcTestCnt2);

	DDX_Control(pDX, IDC_SPREAD_MANU_SOC_REV_UP, m_SprdRevManuCleanUp);
	DDX_Control(pDX, IDC_SPREAD_MANU_SOC_REV_DN, m_SprdRevManuCleanDown);

	DDX_Radio(pDX, IDC_RADIO_EVENT_OPCALL2, m_bEventType);
}


BEGIN_MESSAGE_MAP(CSocketCleaning, CDialog)
    ON_WM_DESTROY()
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_CLEANING_SAVE, &CSocketCleaning::OnBnClickedCleaningSave)
    ON_EN_SETFOCUS(IDC_EDIT_CONTACT_CNT, &CSocketCleaning::OnEnSetfocusEditContactCnt)
    ON_EN_SETFOCUS(IDC_EDIT_ALARM_CNT, &CSocketCleaning::OnEnSetfocusEditAlarmCnt)
    ON_EN_SETFOCUS(IDC_EDIT_INTERVAL_CNT, &CSocketCleaning::OnEnSetfocusEditIntervalCnt)
	ON_EN_SETFOCUS(IDC_EDIT_CONTACT_DELAY_TIME, &CSocketCleaning::OnEnSetfocusEditContactDelayTime)
	ON_BN_CLICKED(IDC_BTN_RESET_CLN1, &CSocketCleaning::OnBnClickedBtnResetCln1)
	ON_BN_CLICKED(IDC_BTN_RESET_CLN2, &CSocketCleaning::OnBnClickedBtnResetCln2)
END_MESSAGE_MAP()


// CSocketCleaning 메시지 처리기입니다.


BOOL CSocketCleaning::OnInitDialog()
{
    CDialog::OnInitDialog();


	g_DMCont.m_dmHandlerOpt.SB(BDM11_GUI_OPEN_HISTORY, TRUE);

    OnBtnGUI();
    OnEditBoxGUI();  
	ChkLotState();

    int nRet = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_CleanInitStart);
    m_CtrlChkInitialStart.SetCheck(nRet);
    nRet = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_CleanInitRetestStart);
    m_CtrlChkInitialRetestStart.SetCheck(nRet);
    nRet = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_CleanIntervalCnt);
    m_CtrlChkInterval.SetCheck(nRet);

    UpdateData(FALSE);

	SocketCleaningDataUpdate();

    SetTimer(0, 1000, NULL);


	int TestSite_Div_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	int TestSite_Div_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);

	for (int i = 0; i < eMaxPressUnitCount; i++) {
		for (int nY = 0; nY < TestSite_Div_Y; nY++) {
			for (int nX = 0; nX < TestSite_Div_X; nX++) {
				int nSiteAddr = (STATION_MAX_PARA*i) + nX + nY*STATION_MAX_X_SITE;
				nSiteOnOff[i] += g_DMCont.m_dmHandlerOpt.GN(NDM11_SocketOnOff_Site1_1 + nSiteAddr);
			}
		}
	}

	for (size_t i = 0; i < eMaxTestPPCount; i++)
	{
		m_vModuleSetPressIdx[i].push_back(ePressUnit_Down_1 + (eMaxATS_Area / 2 * i));
		m_vModuleSetPressIdx[i].push_back(ePressUnit_Down_2 + (eMaxATS_Area / 2 * i));
		m_vModuleSetPressIdx[i].push_back(ePressUnit_Down_3 + (eMaxATS_Area / 2 * i));
		m_vModuleSetPressIdx[i].push_back(ePressUnit_Down_4 + (eMaxATS_Area / 2 * i));
// 		m_vModuleSetPressIdx[i].push_back(ePressUnit_Up_1 + (eMaxATS_Area / 2 * i));
// 		m_vModuleSetPressIdx[i].push_back(ePressUnit_Up_2 + (eMaxATS_Area / 2 * i));
// 		m_vModuleSetPressIdx[i].push_back(ePressUnit_Up_3 + (eMaxATS_Area / 2 * i));
// 		m_vModuleSetPressIdx[i].push_back(ePressUnit_Up_4 + (eMaxATS_Area / 2 * i));
	}

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));


	GetDlgItem(IDC_EDIT_CONTACT_DELAY_TIME)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_TEXT_CLEAN_DELAY_TIME)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_TEXT_DELAY_TIME_SEC)->ShowWindow(FALSE);
	
	CSpreadSheet* SprdCleanDvc[2] = { &m_SprdClnDvcPos1 , &m_SprdClnDvcPos2 };
	CSpreadSheet* SprdClnDvcTestCnt[2] = { &m_SprdClnDvcTestCnt1 , &m_SprdClnDvcTestCnt2 };

	int nTestCleanDiv_X = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_X);
	int nTestCleanDiv_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_Y);

	for (int i = 0; i < 2; i++)
	{
		SprdCleanDvc[i]->SetMaxCols(nTestCleanDiv_X);
		SprdCleanDvc[i]->SetMaxRows(nTestCleanDiv_Y);

		SprdClnDvcTestCnt[i]->SetMaxCols(nTestCleanDiv_X);
		SprdClnDvcTestCnt[i]->SetMaxRows(nTestCleanDiv_Y);
	}
    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSocketCleaning::OnBtnGUI()
{
    m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
    m_BtnCancel.SetIcon(IDI_BTN_EXIT);
    m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

    m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
    m_BtnSave.SetIcon(IDI_BTN_SAVE);
    m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);
}

void CSocketCleaning::OnEditBoxGUI()
{
    m_EditAlarmCnt.SetBackColor(COLOR_WWHITE);
    m_EditAlarmCnt.SetTextSize(30);

    m_EditContactCnt.SetBackColor(COLOR_WWHITE);
    m_EditContactCnt.SetTextSize(30);

    m_EditIntervalCnt.SetBackColor(COLOR_WWHITE);
    m_EditIntervalCnt.SetTextSize(30);

	m_EditContactTime.SetBackColor(COLOR_WWHITE);
	m_EditContactTime.SetTextSize(30);
	
}

void CSocketCleaning::OnDestroy()
{
    CDialog::OnDestroy();

    KillTimer(0);

	g_DMCont.m_dmHandlerOpt.SB(BDM11_GUI_OPEN_HISTORY, FALSE);
}


void CSocketCleaning::OnTimer(UINT_PTR nIDEvent)
{
    switch(nIDEvent)
    {
    case 0:
        {
        }break;
    }

    CDialog::OnTimer(nIDEvent);
}


void CSocketCleaning::OnBnClickedCleaningSave()
{
    char szCokName[512] = {0,};
    ST_BASIC_INFO   stBasicData, stBeforeBasicData;
    g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));	
    g_COK_FileMgr.LoadBasicInfo(szCokName, stBasicData);

	stBeforeBasicData = stBasicData;

    UpdateData(TRUE);

// 	int nErr = 0;
// 	if (m_nCleaningOff == TRUE)
// 		nErr = SocketCleanOnOffInterlock();
// 
// 	if (nErr != NO_ERROR) {
// 		AfxMessageBox("[Error] Socket Clean Device Check Please!!!!!!");
// 	}

    g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_SocketClean        , (BOOL)m_nCleaningOff    );
    g_DMCont.m_dmHandlerOpt.SN(NDM11_SockClnContactCnt     , m_nContactCnt             );
    g_DMCont.m_dmHandlerOpt.SN(NDM11_SockClnAlarmCnt       , m_nAlarmCnt               );
    g_DMCont.m_dmHandlerOpt.SN(NDM11_SockClnIntervalCnt    , m_nIntervalCnt            ); 
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_CleanDev_Event_Type, m_bEventType              );
	g_DMCont.m_dmHandlerOpt.SD(DDM11_SockClnContactDelayTime, m_dContactDelayTime);

    int nRet = m_CtrlChkInitialStart.GetCheck();
    g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_CleanInitStart      , nRet);
    nRet = m_CtrlChkInitialRetestStart.GetCheck();
    g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_CleanInitRetestStart, nRet);
    nRet = m_CtrlChkInterval.GetCheck();
    g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_CleanIntervalCnt    , nRet);


    stBasicData.bMD_SocketClean            = (BOOL)m_nCleaningOff;
    stBasicData.nCleanContacCnt            = m_nContactCnt;
    stBasicData.nCleanAlarmCnt             = m_nAlarmCnt;
    stBasicData.nCleanIntervalCnt          = m_nIntervalCnt;
	stBasicData.dContactDelayTime		   = m_dContactDelayTime;
    stBasicData.bMD_CleanInitStart         = m_CtrlChkInitialStart.GetCheck();
    stBasicData.bMD_CleanInitRetestStart   = m_CtrlChkInitialRetestStart.GetCheck();
    stBasicData.bMD_CleanIntervalCnt       = m_CtrlChkInterval.GetCheck();
	stBasicData.bMD_EventType              = m_bEventType;

    g_COK_FileMgr.SaveBasicInfo(szCokName, &stBasicData);

	std::vector<std::pair<std::pair<CString,int>,std::pair<double,double>>> vData;

	stBeforeBasicData.IsSameSocketCleaning(stBasicData,vData);

	if(!(vData.empty()))
	{
		SocketCleaningChangeLog(vData);
	}

	SocketCleanDataSave();


	//CSpreadSheet* pSpreadUp   = (CSpreadSheet*)GetDlgItem(IDC_SPREAD_MANU_SOC_REV_UP);
	//CSpreadSheet* pSpreadDown = (CSpreadSheet*)GetDlgItem(IDC_SPREAD_MANU_SOC_REV_DN);
	//CString strChkUp = _T(""), strChkDn = _T("");
	//for(int nX=0; nX < eMaxPressUnitCount/2; nX++) {

	//	strChkUp = pSpreadUp->GetTextMatrix(1, nX+1);
	//	strChkDn = pSpreadDown->GetTextMatrix(1, nX+1);
	//	
	//	if( atoi(strChkUp) == DEF_ON ) {
	//		g_DMCont.m_dmHandlerOpt.SB(BDM11_MANU_CLN_REV_SITE9+nX, TRUE);
	//	}else {
	//		g_DMCont.m_dmHandlerOpt.SB(BDM11_MANU_CLN_REV_SITE9+nX, FALSE);
	//	}
	//	
	//	if( atoi(strChkDn) == DEF_ON ) {
	//		g_DMCont.m_dmHandlerOpt.SB(BDM11_MANU_CLN_REV_SITE1+nX, TRUE);
	//	} else {
	//		g_DMCont.m_dmHandlerOpt.SB(BDM11_MANU_CLN_REV_SITE1+nX, FALSE);
	//	}
	//}

	//pSpreadUp = NULL;
	//pSpreadDown = NULL;
}


void CSocketCleaning::OnEnSetfocusEditContactCnt()
{
    char szVal[10]    = {0,};
    char szRetVal[64] = {0,};
    _itoa_s(m_nContactCnt, szVal, sizeof(szVal));

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Cleaning Contact Count", "20", "0");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_CONTACT_CNT)->SetWindowText(szRetVal);
    }
}


void CSocketCleaning::OnEnSetfocusEditAlarmCnt()
{
    char szVal[10]    = {0,};
    char szRetVal[64] = {0,};
    _itoa_s(m_nAlarmCnt, szVal, sizeof(szVal));

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Alarm Count", "99999", "0");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_ALARM_CNT)->SetWindowText(szRetVal);
    }
}


void CSocketCleaning::OnEnSetfocusEditIntervalCnt()
{
    char szVal[10]    = {0,};
    char szRetVal[64] = {0,};
    _itoa_s(m_nIntervalCnt, szVal, sizeof(szVal));

    int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Alarm Count", "99999", "0");

    if(nRet == RETURN_OK){
        GetDlgItem(IDC_EDIT_INTERVAL_CNT)->SetWindowText(szRetVal);
    }
}

void CSocketCleaning::OnEnSetfocusEditContactDelayTime()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nIntervalCnt, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Alarm Count", "5", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_CONTACT_DELAY_TIME)->SetWindowText(szRetVal);
	}
}


void CSocketCleaning::MakeLog(LPCTSTR fmt, ...)
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


void CSocketCleaning::SocketCleaningChangeLog(std::vector<std::pair<std::pair<CString,int>,std::pair<double,double>>> vData)
{
	//Cleaning on ==1 Load tbl off =0
	//check box check = 1
	
	for(int i = 0 ; i< (int)vData.size(); i++)
	{
		std::pair<std::pair<CString,int>,std::pair<double,double>> p;

		p = vData[i];

		if(p.first.second == 1)// check box or Radio button
		{
			if(p.second.second == 0) // off
			{
				MakeLog("[MENU = Socket Cleaning, DATA NAME = %s ][BEFORE=ON, AFTER=OFF]", p.first.first);
			}
			else if(p.second.second == 1) // ON
			{
				MakeLog("[MENU = Socket Cleaning, DATA NAME = %s ][BEFORE=OFF, AFTER=ON]", p.first.first);
			}
		}
		else
		{
			MakeLog("[MENU = Socket Cleaning, DATA NAME = %s ][BEFORE=%.2f, AFTER=%.2f]", p.first.first, p.second.first, p.second.second);
		}
	}
}

void CSocketCleaning::SocketCleanDeviceCountClear(int nModuleID)
{
	CSpreadSheet* pSpread = NULL;
	int nSprdData[MAX_TEST_PP_CNT] = {IDC_SPREAD_CLN_MOD1,IDC_SPREAD_CLN_MOD2};
	int nSprdCount[MAX_TEST_PP_CNT] = {IDC_SPREAD_CLN_CNT1,IDC_SPREAD_CLN_CNT2};

	pSpread = (CSpreadSheet*)GetDlgItem(nSprdCount[nModuleID]);
	for(int nY=pSpread->GetSelBlockRow(); nY<=pSpread->GetSelBlockRow2(); nY++)
	{
		if(nY < 1) { continue; }

		pSpread->SetRow(nY);
		for(int nX=pSpread->GetSelBlockCol(); nX<=pSpread->GetSelBlockCol2(); nX++)
		{
			if(nX < 1) { continue; }
			int nTempority = nY;
			pSpread->SetCol(nX);
			pSpread->SetText("0");
		}
	}
	pSpread = NULL;
}


void CSocketCleaning::OnBnClickedBtnResetCln1()
{
	SocketCleanDeviceCountClear(eTestPP_1);
}


void CSocketCleaning::OnBnClickedBtnResetCln2()
{
	SocketCleanDeviceCountClear(eTestPP_2);
}


void CSocketCleaning::SocketCleaningDataUpdate()
{
	CSpreadSheet* pSpread = NULL;
	ST_DD_DEVICE *pDvc = new ST_DD_DEVICE;

	int nSprdData[eMaxTestPPCount] = {IDC_SPREAD_CLN_MOD1,IDC_SPREAD_CLN_MOD2};
	int nSprdCount[eMaxTestPPCount] = {IDC_SPREAD_CLN_CNT1,IDC_SPREAD_CLN_CNT2};
	//g_DvcDataMem.GetDeviceData(eDeviceStage_TEST_PP_1_BUFF+i, );

	int nPocketX_Buff = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_X);
	int nPocketY_Buff = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_Y);
	CString strCount = _T("");
	for(int i=0; i < eMaxTestPPCount; i++)
	{
		for(int nY=0; nY < nPocketY_Buff; nY++) {
			for(int nX=0; nX < nPocketX_Buff; nX++) {
				g_DvcDataMem.GetDeviceData(eDeviceStage_TEST_PP_1_CLEAN_BUFF+i, nX, nY, pDvc);
				pSpread = (CSpreadSheet*)GetDlgItem(nSprdData[i]);
				pSpread->SetTextMatrix(nY+1, nX+1, (pDvc->sExist == eDeviceData_Exist ? "1":""));
				pSpread = (CSpreadSheet*)GetDlgItem(nSprdCount[i]);
				strCount.Format("%d", pDvc->nRealTestingCnt);
				pSpread->SetTextMatrix(nY+1, nX+1, strCount);
			}
		}
	}					

	SAFE_DELETE(pDvc);
	pSpread = NULL;

	CSpreadSheet* pSpreadUp   = (CSpreadSheet*)GetDlgItem(IDC_SPREAD_MANU_SOC_REV_UP);
	CSpreadSheet* pSpreadDown = (CSpreadSheet*)GetDlgItem(IDC_SPREAD_MANU_SOC_REV_DN);
	CString strChkUp = _T(""), strChkDn = _T("");
	for(int nX=0; nX < eMaxPressUnitCount/2; nX++){

		BOOL bUpSiteRev = g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE9_RESERVE+nX);
		bUpSiteRev |= g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE9+nX);

		BOOL bDownSiteRev = g_DMCont.m_dmEQP.GB(BDM0_SOCKETCLEAN_SITE1_RESERVE+nX);
		bDownSiteRev |= g_DMCont.m_dmHandlerOpt.GB(BDM11_MANU_CLN_REV_SITE1+nX);

		pSpreadUp->SetTextMatrix(1, nX+1, (bUpSiteRev == TRUE ? _T("1") : _T("0")) );
		pSpreadDown->SetTextMatrix(1, nX+1, (bDownSiteRev == TRUE ? _T("1") : _T("0")) );
	}
	pSpreadUp = NULL;
	pSpreadDown = NULL;
}

void CSocketCleaning::SocketCleanDataSave()
{
	CSpreadSheet* pSpreadData = NULL;
	CSpreadSheet* pSpreadCount = NULL;
	ST_DD_DEVICE *pBefore = new ST_DD_DEVICE;
	ST_DD_DEVICE *pAfter = new ST_DD_DEVICE;

	int nSprdData[eMaxTestPPCount] = {IDC_SPREAD_CLN_MOD1,IDC_SPREAD_CLN_MOD2};
	int nSprdCount[eMaxTestPPCount] = {IDC_SPREAD_CLN_CNT1,IDC_SPREAD_CLN_CNT2};
	//g_DvcDataMem.GetDeviceData(eDeviceStage_TEST_PP_1_BUFF+i, );

	int nPocketX_Buff = g_DMCont.m_dmTestPP.GN( NDM4_TestCleanTbl_Div_X );
	int nPocketY_Buff = g_DMCont.m_dmTestPP.GN( NDM4_TestCleanTbl_Div_Y );
	CString strCount = _T("");



	for(int i=0; i < eMaxTestPPCount; i++)
	{
		pSpreadData = (CSpreadSheet*)GetDlgItem(nSprdData[i]);
		pSpreadCount = (CSpreadSheet*)GetDlgItem(nSprdCount[i]);

		for(int nY=0; nY < nPocketY_Buff; nY++){
			for(int nX=0; nX < nPocketX_Buff; nX++){
				g_DvcDataMem.GetDeviceData(eDeviceStage_TEST_PP_1_CLEAN_BUFF+i, nX, nY, pBefore);
				
				CString strDataExist = pSpreadData->GetTextMatrix(nY+1, nX+1);
				CString strDataCount = pSpreadCount->GetTextMatrix(nY+1, nX+1);

				if( atoi(strDataExist) == pBefore->sExist && atoi(strDataCount) == pBefore->nRealTestingCnt ){ // data & count 가 같은 경우

				}else if( pBefore->sExist == eDeviceData_None && atoi(strDataExist) == eDeviceData_Exist ){
					pAfter->sExist = eDeviceData_Exist;
					pAfter->nRealTestingCnt = 0;
					sprintf_s(pAfter->szBinResult, sizeof(pAfter->szBinResult), DEF_SOCKET_CLEAN_DEVICE);
					g_DvcDataMem.SetDeviceData(eDeviceStage_TEST_PP_1_CLEAN_BUFF+i, nX, nY, pAfter);
				}else if( pBefore->sExist == eDeviceData_Exist && atoi(strDataExist) == eDeviceData_None){
					pAfter->sExist = eDeviceData_None;
					pAfter->nRealTestingCnt = 0;
					sprintf_s(pAfter->szBinResult, sizeof(pAfter->szBinResult), "");
					g_DvcDataMem.SetDeviceData(eDeviceStage_TEST_PP_1_CLEAN_BUFF+i, nX, nY, pAfter);
				}else if( atoi(strDataExist) == pBefore->sExist && atoi(strDataCount) != pBefore->nRealTestingCnt ){
					g_DvcDataMem.GetDeviceData(eDeviceStage_TEST_PP_1_CLEAN_BUFF+i, nX, nY, pAfter);
					pAfter->nRealTestingCnt = atoi(strDataCount);
					g_DvcDataMem.SetDeviceData(eDeviceStage_TEST_PP_1_CLEAN_BUFF+i, nX, nY, pAfter);
				}else{
					pAfter->clear();
					g_DvcDataMem.GetDeviceData(eDeviceStage_TEST_PP_1_CLEAN_BUFF+i, nX, nY, pAfter);
				}
			}
		}
	}					

	SAFE_DELETE(pBefore);
	SAFE_DELETE(pAfter);
	pSpreadData = NULL;
	pSpreadCount = NULL;
}

void CSocketCleaning::ChkLotState()
{
	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
	BOOL bRetOneCycle = g_DMCont.m_dmEQP.GB(BDM0_ONECYCLE_HISTORY);

	if(bLotState == eHandlerLotMode_OnGoing)
	{
		GetDlgItem(IDC_CHK_CLN_MD1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_CLN_MD2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_CLN_MD4)->EnableWindow(FALSE);
		//GetDlgItem(IDC_EDIT_INTERVAL_CNT)->EnableWindow(FALSE);
		if(bRetOneCycle == TRUE) {
			GetDlgItem(IDC_SPREAD_CLN_MOD1)->EnableWindow(TRUE);
			GetDlgItem(IDC_SPREAD_CLN_MOD2)->EnableWindow(TRUE);
		}else {
			int nPocketX_Buff = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_X);
			int nPocketY_Buff = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_Y);

			int nTestSite_X = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
			int nTestSite_Y = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);


			ST_DD_DEVICE *pDvc = new ST_DD_DEVICE;
			int nExist[MAX_TEST_PP_CNT] = {0,}, nSperad[MAX_TEST_PP_CNT] ={IDC_SPREAD_CLN_MOD1,IDC_SPREAD_CLN_MOD2};

			for(int i=0; i < eMaxTestPPCount; i++)
			{
				for(int nY=0; nY < nPocketY_Buff; nY++){
					for(int nX=0; nX < nPocketX_Buff; nX++){
						g_DvcDataMem.GetDeviceData(eDeviceStage_TEST_PP_1_CLEAN_BUFF+i, nX, nY, pDvc);
						nExist[i] += pDvc->sExist;
					}
				}
			}

			for (int i = 0; i < eMaxPressUnitCount; i++)
			{
				int nTestPPIdx = i / (eMaxPressUnitCount / 2);
				for (int nY = 0; nY < nTestSite_Y; nY++) {
					for (int nX = 0; nX < nTestSite_X; nX++) {
						g_DvcDataMem.GetDeviceData(eDeviceStage_TEST_SITE_1 + i, nX, nY, pDvc);
						if(strcmp(pDvc->szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0)
							nExist[nTestPPIdx] += pDvc->sExist;
					}
				}
			}

			for (int i = 0; i < eMaxTestPPCount; i++)
			{
				for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++) {
					for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++) {
						g_DvcDataMem.GetDeviceData(eDeviceStage_TEST_PP_1 + i, nX, nY, pDvc);
						if (strcmp(pDvc->szBinResult, DEF_SOCKET_CLEAN_DEVICE) == 0)
							nExist[i] += pDvc->sExist;
					}
				}
			}

			SAFE_DELETE(pDvc);

			for(int i =0; i< eMaxTestPPCount; i++){
				if(nExist[i] != 0){
					GetDlgItem(nSperad[i])->EnableWindow(FALSE);
				}else{
					GetDlgItem(nSperad[i])->EnableWindow(TRUE);
				}
			}
		}
	}
	else {
		GetDlgItem(IDC_CHK_CLN_MD1)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHK_CLN_MD2)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHK_CLN_MD4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_INTERVAL_CNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPREAD_CLN_MOD1)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPREAD_CLN_MOD2)->EnableWindow(TRUE);
	}
}

int CSocketCleaning::SocketCleanOnOffInterlock()
{
	CSpreadSheet* pSpreadData = NULL;

	ST_DD_DEVICE *pBefore = new ST_DD_DEVICE;
	ST_DD_DEVICE *pAfter = new ST_DD_DEVICE;
	int nSprdData[eMaxTestPPCount] = { IDC_SPREAD_CLN_MOD1,IDC_SPREAD_CLN_MOD2 };

	
	int nCleanDeivceCnt[eMaxTestPPCount] = {0,};
	int nSiteModuleCnt[eMaxTestPPCount] = { 0, };


	int nPocketX_Buff = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_X);
	int nPocketY_Buff = g_DMCont.m_dmTestPP.GN(NDM4_TestCleanTbl_Div_Y);

	for (int i = 0; i < eMaxTestPPCount; i++) {
		for (int  nT = 0; nT < eMaxPressUnitCount/2; nT++)
		{
			nSiteModuleCnt[i] += nSiteOnOff[m_vModuleSetPressIdx[i][nT]];
		}
	}

	for (int i = 0; i < eMaxTestPPCount; i++)
	{
		pSpreadData = (CSpreadSheet*)GetDlgItem(nSprdData[i]);

		for (int nY = 0; nY < nPocketY_Buff; nY++) {
			for (int nX = 0; nX < nPocketX_Buff; nX++) {
				//g_DvcDataMem.GetDeviceData(eDeviceStage_TEST_PP_1_CLEAN_BUFF + i, nX, nY, pBefore);

				CString strDataExist = pSpreadData->GetTextMatrix(nY + 1, nX + 1);

				if (atoi(strDataExist) == eDeviceData_Exist) {
					nCleanDeivceCnt[i]++;
				}
			}
		}
		if (nCleanDeivceCnt[i] == eDeviceData_None && nSiteModuleCnt[i] > 0)
			return -1;
	}

	return NO_ERROR;
}

