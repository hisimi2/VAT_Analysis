
// SLT_MCView.cpp : CSLT_MCView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "SLT_MC.h"
#endif

#include "SLT_MCDoc.h"
#include "SLT_MCView.h"

#include "DlgManu.h"
#include "DlgSetup.h"
#include "DlgMon.h"
#include "MainFrm.h"
#include <vector>
#include "MotorInterlock.h"
#include "DlgDebugAutoStacker.h"
#include <ws2tcpip.h>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSLT_MCView

IMPLEMENT_DYNCREATE(CSLT_MCView, CFormView)

BEGIN_MESSAGE_MAP(CSLT_MCView, CFormView)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_MC_MENU_SETUP, &CSLT_MCView::OnBnClickedBtnMcMenuSetup)
	ON_BN_CLICKED(IDC_BTN_MC_MENU_MANUAL, &CSLT_MCView::OnBnClickedBtnMcMenuManual)
	ON_BN_CLICKED(IDC_BTN_MC_MENU_MONITOR, &CSLT_MCView::OnBnClickedBtnMcMenuMonitor)
	ON_BN_CLICKED(IDC_BTN_MC_MENU_CLOSE, &CSLT_MCView::OnBnClickedBtnMcMenuClose)
	ON_BN_CLICKED(IDC_BTN_MC_MENU_SHUT_DOWN, &CSLT_MCView::OnBnClickedBtnMcMenuShutDown)
	ON_BN_CLICKED(IDC_BTN_MC_MENU_DIO_SIM, &CSLT_MCView::OnBnClickedBtnMcMenuDioSim)
	ON_BN_CLICKED(IDC_BTN_MC_MENU_AIO_SIM, &CSLT_MCView::OnBnClickedBtnMcMenuAioSim)
	ON_BN_CLICKED(IDC_BTN_MC_MENU_MOTION_SIM, &CSLT_MCView::OnBnClickedBtnMcMenuMotionSim)
	ON_BN_CLICKED(IDC_BTN_MC_MENU_SHOW_DEBUG_VIEW, &CSLT_MCView::OnBnClickedBtnMcMenuShowDebugView)
	ON_BN_CLICKED(IDC_BTN_WMX_RECONENECT, &CSLT_MCView::OnBnClickedBtnWmxReconenect)
END_MESSAGE_MAP()

// CSLT_MCView 생성/소멸

CSLT_MCView::CSLT_MCView()
	: CFormView(CSLT_MCView::IDD)
{
	g_pDlgHomeCheckStatus = NULL;
	//g_pDlgManuMotorTeaching = NULL;
}

CSLT_MCView::~CSLT_MCView()
{
	::WSACleanup();
}

void CSLT_MCView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_MC_MENU_SETUP     , m_btnSetup  );
	DDX_Control(pDX, IDC_BTN_MC_MENU_MANUAL    , m_btnManual );
	DDX_Control(pDX, IDC_BTN_MC_MENU_MONITOR   , m_btnMonitor);
	DDX_Control(pDX, IDC_BTN_MC_MENU_CLOSE     , m_btnClose  );
	DDX_Control(pDX, IDC_BTN_MC_MENU_SHUT_DOWN , m_btnExit   );
}

BOOL CSLT_MCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

#define SZ_DRIVER_FILE_NAME _T("TW_LS_DRIVER.INI")

#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <string.h>

void CSLT_MCView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	m_nProgressInitCnt = 0;

	g_LibCommCtrl.ProgressBarCreate();
	g_LibCommCtrl.ProgressBarSetStatus("SLT_MC.exe Starting!!!!");
	g_LibCommCtrl.ProgressBarSetPos(m_nProgressInitCnt);

	Init_GlobalObject();

	g_pDlgDebugView = new CDlgDebugAutoStacker(this);
	g_pDlgDebugView->Create(IDD_DEBUG_AUTO_STACKER, this);
	g_pDlgDebugView->ShowWindow(SW_HIDE);

// 	g_pDlgManuMotorTeaching = new CDlgManuMotorTeaching(this);
// 	g_pDlgManuMotorTeaching->Create(IDD_MANU_MOTOR_TEACHING, this);
// 	g_pDlgManuMotorTeaching->ShowWindow(SW_HIDE);

	InitControl();	
	// jira 313 
	//g_DMCont.m_dmEQP.SB(BDM0_ONECYCLE_HISTORY, TRUE);
	g_DMCont.m_dmEQP.SB(BDM0_OPEN_EJECTOR_DLG_HISTORY, FALSE);
	g_DMCont.m_dmEQP.SB(BDM0_OPEN_LOTINFO_DLG_HISTORY, FALSE);
	g_DMCont.m_dmEQP.SB(BDM0_OPEN_SLT_UI_DLG_HISTORY, FALSE);
	g_DMCont.m_dmVAT.SB(BDM14_VAT_UI_OPEN_HSITORY, FALSE);
	g_DMCont.m_dmVAT.SB(BDM14_VAT_TEACHING_USE, FALSE);

	g_DMCont.m_dmEQP.SB(BDM0_OPEN_HEIGHT_CAL_EJECTOR_DLG_HISTORY, FALSE);

	// GRR Table 관련 create
	g_DB.CREATE_GRRDATA_TBL();

	g_CylinderTime.initialize();

	int nChk = g_DB.CHECK_CYLINDER_TIME_TBL();
	if (nChk != NO_ERROR)
	{
		for (int i = 0; i < MAX_CYLINDER_COUNT; i++)
			g_DB.INSERT_CYLINDER_TIME_TBL(g_CylinderTime.m_stCylinderTime[i]);
	}


    char szPath[512] = {0,};
    char szTitle[512] = {0,};
    char szProcName[512] = {0,};
    ::GetCurrentDirectory(sizeof(szPath), szPath);
#ifdef _DEBUG
    sprintf_s(szProcName, sizeof(szProcName), "SLT_D.exe");
    strcat_s(szPath, sizeof(szPath), "\\SLT_D.exe");
#else
    sprintf_s(szProcName, sizeof(szProcName), "SLT.exe");
    strcat_s(szPath, sizeof(szPath), "\\SLT.exe");
#endif

    sprintf_s(szTitle, sizeof(szTitle), "TW_SLT");
    CWnd *pWnd = FindWindow(NULL, szTitle);

    if(pWnd != NULL) {
		CloseProcess(szProcName);

        ShellExecute(NULL,"open", szPath ,NULL,NULL,SW_SHOWNORMAL);
    }else{
		ShellExecute(NULL,"open", szPath ,NULL,NULL,SW_SHOWNORMAL);
	}

	m_nProgressInitCnt = 100;
	g_LibCommCtrl.ProgressBarSetPos(m_nProgressInitCnt);

 	if(g_pBarcode->IsConnected() == TRUE){
 		//_tVisionMsg m_tVisionMsgMeasure;
 		int nErr = g_pBarcode->Cmd_SetCok();
 		if( nErr != ERR_NO_ERROR ){
 			AfxMessageBox("2d barcode vision SETCOK error. please setting manual");
 		}
 	}
 //	if(g_pTestSiteInspUp->IsConnected() == TRUE){
 //		_tVisionMsg m_tVisionMsgMeasure;
 //		int nErr = g_pTestSiteInspUp->Cmd_SetCok(&m_tVisionMsgMeasure);
 //		if( nErr != ERR_NO_ERROR ){
 //			AfxMessageBox("Test Site Up vision SETCOK error. please setting manual.");
 //		}
	//}
	//if(g_pTestSiteInspDn->IsConnected() == TRUE){
	//	_tVisionMsg m_tVisionMsgMeasure;
 //		int nErr = g_pTestSiteInspDn->Cmd_SetCok(&m_tVisionMsgMeasure);
 //		if( nErr != ERR_NO_ERROR ){
 //			AfxMessageBox("Test Site Down vision SETCOK error. please setting manual.");
 //		}
 //	}

 	if(g_pVisionTestHand_1->IsConnected() == TRUE)
	{
 		//_tVisionMsg m_tVisionMsgMeasure;
 		int nErr = g_pVisionTestHand_1->Cmd_SetCok();
 		if( nErr != ERR_NO_ERROR ){
 			AfxMessageBox("Test hand 1 vision SETCOK error. please setting manual.");
 		}
	}
	if(g_pVisionTestHand_2->IsConnected() == TRUE)
	{
 		//_tVisionMsg m_tVisionMsgMeasure;
 		int nErr = g_pVisionTestHand_2->Cmd_SetCok();
 		if( nErr != ERR_NO_ERROR ){
 			AfxMessageBox("Test hand 2 vision SETCOK error. please setting manual.");
 		}
 	}

	// GRR Table 관련 create
	g_DB.CREATE_GRRDATA_TBL();

	//imsi procdump 실행
	char szTemp[512] = { 0, };
	::GetCurrentDirectory(sizeof(szTemp), szTemp);
	char szBatPath[512] = { 0, };

	sprintf_s(szBatPath, sizeof(szBatPath), "%s\\ProcDump_SLT\\AutoDump.bat", szTemp);
	int nErr = (int)ShellExecute(NULL, "open", szBatPath, NULL,NULL, SW_SHOWMINIMIZED);
	if (nErr <= 32) { // ShellExecute return 값 32이하 면 Error
		g_TaskSystemCtrl.MakeLog("[Failed to execute Batch File. =%s]", szBatPath);
	}



	//g_pDlgMotorMonitor = new CDlgMon(this);
	//g_pDlgMotorMonitor->Create(IDD_MON_OVERVIEW, this);
	//g_pDlgMotorMonitor->ShowWindow(SW_HIDE);

	for (int i = 0; i < eMaxTestPPCount; i++)
	{
// 		if (g_IO.out(DO_TEST_HAND1_WEB_CAM_LED + i) == DEF_ON)
// 		{
// 			g_TaskTestPP[i].VisionLED_ONOFF(DEF_OFF, i);
// 		}
	}

	g_pDlgHomeCheckStatus = new CDlgHomeCheckStatus(this);
	g_pDlgHomeCheckStatus->Create(IDD_DIALG_HOME_CHECK_STATUS, this);
	g_pDlgHomeCheckStatus->ShowWindow(SW_HIDE);

	g_LibCommCtrl.ProgressBarDestroy();
}

BOOL CSLT_MCView::CloseProcess(CString sExeName)
{
    DWORD dwSize = 250;
    HANDLE hSnapShot;
    PROCESSENTRY32 pEntry;
    BOOL bCrrent=FALSE;


    hSnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPALL,NULL);

    pEntry.dwSize =sizeof(pEntry);
    // 실행중인 프로세스들의 첫번재 정보를 가져온다.
    Process32First (hSnapShot,&pEntry);

    // Tool이 실행중인지 확인
    while(1)
    {
        // 다음번 프로세스의 정보를 가져온다.
        BOOL hRes=Process32Next (hSnapShot,&pEntry);

        if(hRes==FALSE){
            break;
        }

        if(!strncmp(pEntry.szExeFile,sExeName,15))
        {
            bCrrent = TRUE;
        }
        if(bCrrent)
        {
            HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pEntry.th32ProcessID);
            if(hProcess)
            {
                if(TerminateProcess(hProcess, 0))
                {
                    unsigned long nCode; //프로세스 종료 상태
                    GetExitCodeProcess(hProcess, &nCode);
                }
                CloseHandle(hProcess);
            }
            break;
        } 

    }
    return FALSE;
}


// CSLT_MCView 진단

#ifdef _DEBUG
void CSLT_MCView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSLT_MCView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSLT_MCDoc* CSLT_MCView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSLT_MCDoc)));
	return (CSLT_MCDoc*)m_pDocument;
}
#endif //_DEBUG


// CSLT_MCView 메시지 처리기


void CSLT_MCView::OnDestroy()
{
	CFormView::OnDestroy();

	g_TaskSystemCtrl.OnLotCommand(eAutoLotCmd_Pause, NULL, NULL);
	
	Close_GlobalObject();

// 	for(int i=0; i < eMaxPressUnitCount; i++){
// 		g_pFtpObj[i]->FtpClose()
// 	}
	
	if( g_pDlgDebugView ){
		g_pDlgDebugView->DestroyWindow();
		delete g_pDlgDebugView;
		g_pDlgDebugView = NULL;
	}

	if (g_pDlgHomeCheckStatus) {
		g_pDlgHomeCheckStatus->DestroyWindow();
		delete g_pDlgHomeCheckStatus;
		g_pDlgHomeCheckStatus = NULL;
	}

// 	if (g_pDlgManuMotorTeaching) {
// 		g_pDlgManuMotorTeaching->DestroyWindow();
// 		delete g_pDlgManuMotorTeaching;
// 		g_pDlgManuMotorTeaching = NULL;
// 	}

	
	//if (g_pDlgMotorMonitor != NULL) {
	//	g_pDlgMotorMonitor->DestroyWindow();
	//	delete g_pDlgMotorMonitor;
	//	g_pDlgMotorMonitor = NULL;
	//}



}

//===============================================================================================
// 코드의 위치를 마음대로 바꾸지 말것.
//===============================================================================================
void CSLT_MCView::Init_GlobalObject()
{
	m_nProgressInitCnt += 10;
	g_LibCommCtrl.ProgressBarSetPos(m_nProgressInitCnt);
	int i=0;
	char szDriverFileName[MAX_PATH]={0,};
	char szTemp[512]={0,};
	::GetCurrentDirectory(sizeof(szTemp), szTemp);
	sprintf_s(szDriverFileName, sizeof(szDriverFileName), "%s\\sys_data\\%s", szTemp, SZ_DRIVER_FILE_NAME);

	g_DB.Create(szDriverFileName, strlen(szDriverFileName), eDB_TYPE_SYSTEM);
	g_DBErr.Create(szDriverFileName, strlen(szDriverFileName), eDB_TYPE_LOG);
	g_DBErr.UPDATE_ADD_COLUMN_TBL("LOTNO","VARCHAR(255)");
 	g_DMCont.Create(szDriverFileName);

	g_DBTeach.Create(szDriverFileName, strlen(szDriverFileName), eDB_TYPE_TEACH);
	g_DBLotHistory.Create(szDriverFileName, strlen(szDriverFileName), eDB_TYPE_LOTHISTORY);
	g_DBLotHistory.UPDATE_ADD_COLUMN_TBL("START_Time", "VARCHAR(255)");
	g_DBLotHistory.UPDATE_ADD_COLUMN_TBL("ONLINE_OFFLINE", "INT");
	g_DBLotHistory.UPDATE_ADD_COLUMN_TBL("RUN_Time", "VARCHAR(255)");
	g_DBLotHistory.UPDATE_ADD_COLUMN_TBL("UPH", "INT");
	g_DBLotHistory.UPDATE_ADD_COLUMN_TBL("AvgIndexTime", "DOUBLE");

	g_DBVAT.Create(szDriverFileName, strlen(szDriverFileName), eDB_TYPE_VAT);

    // Logger
	for( int i = 0; i < eMaxStackerCount; i++ ) {
		CString strFormat = _T( "" );
		strFormat.Format( "Stacker_%d", i + 1 );
		TWLOG.CreateLog( eLogID_Stacker_1 + i, strFormat );

		strFormat.Format( "Auto_Stacker_%d", i + 1 );
		TWLOG.CreateLog( eLogID_Auto_Stacker1 + i, strFormat );
	}

	for(int i=0; i < eMaxPressUnitCount; i++ )
	{
		CString strFormat = _T("");
		strFormat.Format("Auto_Press_%d", i+1);
		TWLOG.CreateLog( eLogID_Auto_Press_1+i, strFormat );

		strFormat.Format("PressUnit_%d", i+1);
		TWLOG.CreateLog( eLogID_PressUnit_1+i, strFormat );

		strFormat.Format("Agent_%d", i + 1);
		TWLOG.CreateLog(eLogID_Agent_1 + i, strFormat);
	}

	for(int i=0; i < eMaxTestPPCount; i++ )
	{
		CString strFormat = _T("");

		strFormat.Format("Auto_TestPP_%d", i+1);
		TWLOG.CreateLog( eLogID_Auto_Test_1+i, strFormat );


		strFormat.Format("TestPP_%d", i+1);
		TWLOG.CreateLog( eLogID_TestPP_1+i, strFormat );


	}
	for (int i = 0; i < eMaxLoadTblCount; i++)
	{
		CString strFormat = _T("");

		strFormat.Format("Auto_LoadTable_%d", i + 1);
		TWLOG.CreateLog(eLogID_Auto_LoadTbl_1 + i, strFormat);

		strFormat.Format("LoadTable_%d", i + 1);
		TWLOG.CreateLog(eLogID_LoadTable_1 + i, strFormat);
	}

	for (int i = 0; i < eMaxTrayPPCount; i++)
	{
		CString strFormat = _T("");

		strFormat.Format("Auto TrayPP_%d", i + 1);
		TWLOG.CreateLog(eLogID_Auto_Tray_1 + i, strFormat);

		strFormat.Format("TrayPP_%d", i + 1);
		TWLOG.CreateLog(eLogID_TrayPP_1 + i, strFormat);
	}


	TWLOG.CreateLog(eLogID_Auto_TrayFeeder, _T( "Auto_TrayFeeder" ) );
	TWLOG.CreateLog(eLogID_TrayFeeder, _T( "TrayFeeder" ) );

	TWLOG.CreateLog(eLogID_Auto_Transfer, _T("Auto Transfer"));
	TWLOG.CreateLog(eLogID_Transfer, _T("Transfer"));


    TWLOG.CreateLog( eLogID_IPC_GUI_Comm, _T("IPC_GUI_Comm"));
    TWLOG.CreateLog( eLogID_IPC_MC_Comm, _T("IPC_MC_Comm"));

    TWLOG.CreateLog( eLogID_LoadUnder, _T("LoadUnder"));
    TWLOG.CreateLog( eLogID_UnloadUnder, _T("UnloadUnder"));
    TWLOG.CreateLog( eLogID_Vibrator, _T("Vibrator"));

    TWLOG.CreateLog( eLogID_TesterIF, _T("TesterIF"));
    TWLOG.CreateLog( eLogID_Auto_SYSTEM, _T("Auto_SYSTEM"));

    for(int i=0; i < eMaxLogCount; i++ ){
        //TWLOG.SetRootPath(i, _T("D:\\Techwing\\LOG"));
		TWLOG.SetRootPath(i, _T("D:\\Techwing\\HandlerLog"));
        TWLOG.SetStoreType(i, LogFileType::eCapacity);
        TWLOG.SetPathType(i, LogPathType::DAY);
        TWLOG.SetLogItem(i, LogItem::Time|LogItem::NewLine|LogItem::Date);
    }

    TWLOG.CreateLog   (eLogID_OEEDATA, _T("OEE"));
    TWLOG.SetRootPath (eLogID_OEEDATA, _T("D:\\Techwing\\OEE"));
    TWLOG.SetStoreType(eLogID_OEEDATA, LogFileType::eIpDateTime);


	WSADATA wsaData;
	char name[512] = {0,};
	CString strIP = _T("");
	PHOSTENT hostinfo;
	if( WSAStartup( MAKEWORD( 2, 0 ), &wsaData ) == 0 )
	{
		if( gethostname ( name, sizeof(name)) == 0)
		{
			if((hostinfo = gethostbyname(name)) != NULL)
			{
				strIP = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
			}
		}      
		WSACleanup( );
	}
	TWLOG.SetIpAddress(eLogID_OEEDATA, strIP);
    TWLOG.SetPathType (eLogID_OEEDATA, LogPathType::ROOT);
    TWLOG.SetExtension(eLogID_OEEDATA, "txt");
    TWLOG.SetLogItem  (eLogID_OEEDATA, LogItem::NewLine);

    char szLotId[512]    = {0,};
    g_DMCont.m_dmHandlerOpt.GS(SDM11_LotNo     , szLotId      , sizeof(szLotId));

    TWLOG.CreateLog   (eLogID_PRODUCTCSV, _T("PRODUCTCSV"));
    TWLOG.SetRootPath (eLogID_PRODUCTCSV, _T("D:\\ProductCSV\\Temporary"));
    TWLOG.SetStoreType(eLogID_PRODUCTCSV, LogFileType::eLotIdTime);
    TWLOG.SetLotName  (eLogID_PRODUCTCSV, szLotId);
    TWLOG.SetPathType (eLogID_PRODUCTCSV, LogPathType::ROOT);
    TWLOG.SetExtension(eLogID_PRODUCTCSV, "txt");
    TWLOG.SetLogItem  (eLogID_PRODUCTCSV, LogItem::NewLine);
    
// 
// 	char sz2didCsvPath[512] = { 0, };
// 	g_DMCont.m_dmHandlerOpt.GS(SDM11_2did_Csv_File_Path, sz2didCsvPath, sizeof(sz2didCsvPath));
// 	CString strtemp = _T("");
// 
// 	TWLOG.CreateLog(eLogID_2DIDCSV, _T("2DIDCSV"));
// 	if (strcmp(sz2didCsvPath, _T("")) == 0)
// 	{
// 		strtemp = _T("D:\\ProductCSV\\Temporary");
// 		TWLOG.SetRootPath(eLogID_2DIDCSV, strtemp);
// 	}
// 	else
// 	{
// 		TWLOG.SetRootPath(eLogID_2DIDCSV, sz2didCsvPath);
//  	}
// 	TWLOG.SetRootPath(eLogID_2DIDCSV, _T(""));
// 	TWLOG.SetStoreType(eLogID_2DIDCSV, LogFileType::eLotIdTime);
// 	TWLOG.SetLotName(eLogID_2DIDCSV, szLotId);
// 	TWLOG.SetPathType(eLogID_2DIDCSV, LogPathType::ROOT);
// 	TWLOG.SetExtension(eLogID_2DIDCSV, "txt");
// 	TWLOG.SetLogItem(eLogID_2DIDCSV, LogItem::NewLine);




    TWLOG.CreateLog   (eLogID_TCPIP, _T("TCPIP"));
	TWLOG.SetRootPath(eLogID_TCPIP, _T("D:\\Techwing\\HandlerLog"));
	TWLOG.SetStoreType(eLogID_TCPIP, LogFileType::eCapacity);
	TWLOG.SetPathType(eLogID_TCPIP, LogPathType::DAY);
	TWLOG.SetLogItem(eLogID_TCPIP, LogItem::Time | LogItem::NewLine | LogItem::Date);

	TWLOG.CreateLog(eLogID_TCPIP_TESTER_REG, _T("TCPIP_TESTER_REG"));
	TWLOG.SetRootPath(eLogID_TCPIP_TESTER_REG, _T("D:\\Techwing\\HandlerLog"));
	TWLOG.SetStoreType(eLogID_TCPIP_TESTER_REG, LogFileType::eCapacity);
	TWLOG.SetPathType(eLogID_TCPIP_TESTER_REG, LogPathType::DAY);
	TWLOG.SetLogItem(eLogID_TCPIP_TESTER_REG, LogItem::Time | LogItem::NewLine | LogItem::Date);

	TWLOG.CreateLog(eLogID_TCPIP_TESTER_STAUS, _T("TCPIP_STAUS"));
	TWLOG.SetRootPath(eLogID_TCPIP_TESTER_STAUS, _T("D:\\Techwing\\HandlerLog"));
	TWLOG.SetStoreType(eLogID_TCPIP_TESTER_STAUS, LogFileType::eCapacity);
	TWLOG.SetPathType(eLogID_TCPIP_TESTER_STAUS, LogPathType::DAY);
	TWLOG.SetLogItem(eLogID_TCPIP_TESTER_STAUS, LogItem::Time | LogItem::NewLine | LogItem::Date);

	TWLOG.CreateLog(eLogID_TCPIP_TEST_RESULT, _T("TCPIP__TEST_RESULT"));
	TWLOG.SetRootPath(eLogID_TCPIP_TEST_RESULT, _T("D:\\Techwing\\HandlerLog"));
	TWLOG.SetStoreType(eLogID_TCPIP_TEST_RESULT, LogFileType::eCapacity);
	TWLOG.SetPathType(eLogID_TCPIP_TEST_RESULT, LogPathType::DAY);
	TWLOG.SetLogItem(eLogID_TCPIP_TEST_RESULT, LogItem::Time | LogItem::NewLine | LogItem::Date);

	TWLOG.CreateLog(eLogID_TCPIP_TESTER_INFORMATION, _T("TCPIP_TESTER_INFORMATION"));
	TWLOG.SetRootPath(eLogID_TCPIP_TESTER_INFORMATION, _T("D:\\Techwing\\HandlerLog"));
	TWLOG.SetStoreType(eLogID_TCPIP_TESTER_INFORMATION, LogFileType::eCapacity);
	TWLOG.SetPathType(eLogID_TCPIP_TESTER_INFORMATION, LogPathType::DAY);
	TWLOG.SetLogItem(eLogID_TCPIP_TESTER_INFORMATION, LogItem::Time | LogItem::NewLine | LogItem::Date);
	
	TWLOG.CreateLog(eLogID_TCPIP_HEART_BEAT, _T("TCPIP_HEART_BEAT"));
	TWLOG.SetRootPath(eLogID_TCPIP_HEART_BEAT, _T("D:\\Techwing\\HandlerLog"));
	TWLOG.SetStoreType(eLogID_TCPIP_HEART_BEAT, LogFileType::eCapacity);
	TWLOG.SetPathType(eLogID_TCPIP_HEART_BEAT, LogPathType::DAY);
	TWLOG.SetLogItem(eLogID_TCPIP_HEART_BEAT, LogItem::Time | LogItem::NewLine | LogItem::Date);

	TWLOG.CreateLog(eLogID_TCPIP_TESTER_SCRIPT_INFORMATION, _T("TCPIP_SCRIPT_TESTER_INFORMATION"));
	TWLOG.SetRootPath(eLogID_TCPIP_TESTER_SCRIPT_INFORMATION, _T("D:\\Techwing\\HandlerLog"));
	TWLOG.SetStoreType(eLogID_TCPIP_TESTER_SCRIPT_INFORMATION, LogFileType::eCapacity);
	TWLOG.SetPathType(eLogID_TCPIP_TESTER_SCRIPT_INFORMATION, LogPathType::DAY);
	TWLOG.SetLogItem(eLogID_TCPIP_TESTER_SCRIPT_INFORMATION, LogItem::Time | LogItem::NewLine | LogItem::Date);


	TWLOG.CreateLog(eLogID_TCPIP_SET_TEST_MODE, _T("TCPIP_SET_TEST_MODE"));
	TWLOG.SetRootPath(eLogID_TCPIP_SET_TEST_MODE, _T("D:\\Techwing\\HandlerLog"));
	TWLOG.SetStoreType(eLogID_TCPIP_SET_TEST_MODE, LogFileType::eCapacity);
	TWLOG.SetPathType(eLogID_TCPIP_SET_TEST_MODE, LogPathType::DAY);
	TWLOG.SetLogItem(eLogID_TCPIP_SET_TEST_MODE, LogItem::Time | LogItem::NewLine | LogItem::Date);

	TWLOG.CreateLog(eLogID_TCPIP_APL_INFO, _T("TCPIP_APL_INFO"));
	TWLOG.SetRootPath(eLogID_TCPIP_APL_INFO, _T("D:\\Techwing\\HandlerLog"));
	TWLOG.SetStoreType(eLogID_TCPIP_APL_INFO, LogFileType::eCapacity);
	TWLOG.SetPathType(eLogID_TCPIP_APL_INFO, LogPathType::DAY);
	TWLOG.SetLogItem(eLogID_TCPIP_APL_INFO, LogItem::Time | LogItem::NewLine | LogItem::Date);

	TWLOG.CreateLog(eLogID_SYSTEM, _T("SYSTEM"));

	TWLOG.SetRootPath(eLogID_SYSTEM, _T("D:\\Techwing\\HandlerLog"));
	TWLOG.SetStoreType(eLogID_SYSTEM, LogFileType::eDay);
	TWLOG.SetPathType(eLogID_SYSTEM, LogPathType::DAY);
	TWLOG.SetLogItem(eLogID_SYSTEM, LogItem::Time | LogItem::NewLine | LogItem::Date);

	TWLOG.CreateLog(eLogID_Bonjour, _T("Bonjour"));
	TWLOG.SetRootPath(eLogID_Bonjour, _T("D:\\Techwing\\HandlerLog"));
	TWLOG.SetStoreType(eLogID_Bonjour, LogFileType::eCapacity);
	TWLOG.SetPathType(eLogID_Bonjour, LogPathType::DAY);
	TWLOG.SetLogItem(eLogID_Bonjour, LogItem::Time | LogItem::NewLine | LogItem::Date);

	TWLOG.CreateLog(eLogID_AgentIF, _T("AgentIF"));
	TWLOG.SetRootPath(eLogID_AgentIF, _T("D:\\Techwing\\HandlerLog"));
	TWLOG.SetStoreType(eLogID_AgentIF, LogFileType::eCapacity);
	TWLOG.SetPathType(eLogID_AgentIF, LogPathType::DAY);
	TWLOG.SetLogItem(eLogID_AgentIF, LogItem::Time | LogItem::NewLine | LogItem::Date);

	TWLOG.CreateLog(eLogID_Teaching, _T("Teaching"));
	TWLOG.SetRootPath(eLogID_Teaching, _T("D:\\Techwing\\HandlerLog"));
	TWLOG.SetStoreType(eLogID_Teaching, LogFileType::eDay);
	TWLOG.SetPathType(eLogID_Teaching, LogPathType::DAY);
	TWLOG.SetLogItem(eLogID_Teaching, LogItem::Time | LogItem::NewLine | LogItem::Date);
	
	TWLOG.CreateLog(eLogID_CylinderTime, _T("CylinderTime"));
	TWLOG.SetRootPath(eLogID_CylinderTime, _T("D:\\Techwing\\HandlerLog"));
	TWLOG.SetStoreType(eLogID_CylinderTime, LogFileType::eDay);
	TWLOG.SetPathType(eLogID_CylinderTime, LogPathType::DAY);
	TWLOG.SetLogItem(eLogID_CylinderTime, LogItem::Time | LogItem::NewLine | LogItem::Date);

	TWLOG.CreateLog(eLogID_ALARM, _T("ALARMLog"));
	TWLOG.SetRootPath(eLogID_ALARM, _T("D:\\Techwing\\HandlerLog"));
	TWLOG.SetStoreType(eLogID_ALARM, LogFileType::eDay);
	TWLOG.SetPathType(eLogID_ALARM, LogPathType::DAY);
	TWLOG.SetLogItem(eLogID_ALARM, LogItem::Time | LogItem::NewLine | LogItem::Date);

	//VAT Log 생성.
	TWLOG.CreateLog(eLogID_VAT_Auto_System, _T("Vat_Auto_system"));
	TWLOG.CreateLog(eLogID_VAT_Auto_Tray_1, _T("Vat_Auto_TrayPP_1"));
	TWLOG.CreateLog(eLogID_VAT_Auto_Tray_2, _T("Vat_Auto_TrayPP_2"));
	TWLOG.CreateLog(eLogID_VAT_Auto_LD_Test_1, _T("Vat_Auto_LD_TestPP_1"));
	TWLOG.CreateLog(eLogID_VAT_Auto_UD_Test_1, _T("Vat_Auto_UD_TestPP_1"));
	TWLOG.CreateLog(eLogID_VAT_Auto_LD_Test_2, _T("Vat_Auto_LD_TestPP_2"));
	TWLOG.CreateLog(eLogID_VAT_Auto_UD_Test_2, _T("Vat_Auto_UD_TestPP_2"));

	TWLOG.CreateLog(eLogID_VAT_Tray_1, _T("Vat_TrayPP_1"));
	TWLOG.CreateLog(eLogID_VAT_Tray_2, _T("Vat_TrayPP_2"));
	TWLOG.CreateLog(eLogID_VAT_Test_1, _T("Vat_TestPP_1"));
	TWLOG.CreateLog(eLogID_VAT_Test_2, _T("Vat_TestPP_2"));

	TWLOG.CreateLog(eLogID_VAT_LoadTable_1, _T("Vat_Table_1"));
	TWLOG.CreateLog(eLogID_VAT_LoadTable_2, _T("Vat_Table_2"));
	TWLOG.CreateLog(eLogID_VAT_LoadTable_3, _T("Vat_Table_3"));
	TWLOG.CreateLog(eLogID_VAT_LoadTable_4, _T("Vat_Table_4"));
	
	TWLOG.CreateLog(eLogID_VAT_GUI, _T("Vat_GUI"));

	for (int i = eLogID_VAT_Auto_System; i < eMaxVatLogCount; i++) {
		//TWLOG.SetRootPath(i, _T("D:\\Techwing\\LOG"));
		TWLOG.SetRootPath(i, _T("D:\\Techwing\\VAT_LOG"));
		TWLOG.SetStoreType(i, LogFileType::eCapacity);
		TWLOG.SetPathType(i, LogPathType::DAY);
		TWLOG.SetLogItem(i, LogItem::Time | LogItem::NewLine | LogItem::Date);
	}


    TWLOG.Run();

	CConfigData ConfigData(szDriverFileName);

	g_Error.Initialize(AfxGetMainWnd());
	g_NetDriver.Initialize(AfxGetMainWnd());

	m_nProgressInitCnt += 10;
	g_LibCommCtrl.ProgressBarSetPos(m_nProgressInitCnt);

	g_SoftServoCtrl.CreateDev(szDriverFileName, MAX_PATH); //WMX Init Motor에서

	m_nProgressInitCnt += 20;
	g_LibCommCtrl.ProgressBarSetPos(m_nProgressInitCnt);

 	g_IO.Create(szDriverFileName, strlen(szDriverFileName));
	if( g_IO.GetDriverType() == _DIO_TYPE_SIMUL_ ){
		g_pDlgSimulDioSLT = new CDlgSimulDioSLT;
		g_pDlgSimulDioSLT->Create(IDD_SIMUL_DIO_SLT);
		//g_pDlgSimulDioSLT->ShowWindow(SW_SHOW);
		g_IO.SetSimulWnd(g_pDlgSimulDioSLT);
	}

 	g_AIO.Initialize(szDriverFileName, strlen(szDriverFileName));
	g_Motor.Create(szDriverFileName, strlen(szDriverFileName));

	g_DMCont.m_dmEQP.SN(NDM0_MAIN_POWER_CONTROLER_CONNETION_STATUS, eCommReconnectOK);

	m_nProgressInitCnt += 10;
	g_LibCommCtrl.ProgressBarSetPos(m_nProgressInitCnt);



	g_pBbxmnpClient = g_Motor.GetbbxmnpClient();
	int nMotorCount = g_Motor.GetAxisCount();
	for(i=0; i<nMotorCount; i++){
		ST_MOT_INTERLOCK_CALLBACK_MAP rData;
		AfxGetMotorInterlockData(i, rData);
		g_Motor[ i ]->SetInterlock( rData.fnMovInterlockCallBack, rData.fnOrgInterlockCallBack, rData.fnJogInterlockCallBack );
	}

	for(int i=0; i<nMotorCount; i++)
	{
		g_Motor[i]->AlarmReset();
		Sleep(1);
		g_Motor[i]->ServoOff();
		Sleep(1);

		
	}

 	g_CokMgr.Initialize(szTemp);
 	g_GuiComm.Initialize(this, szDriverFileName);

	//g_MotorDriverComIF.Initialzie();

	
	int nCompanyType = ConfigData.GetInt("MC TO VISION COMM CONFIG", "2D COMPANY TYPE", "1");

	int nEqpOption = 0;
	nEqpOption = ConfigData.GetInt("Model", "EQP NO", "1");
	g_DMCont.m_dmEQP.SN(NDM0_Handler_EQP_No, nEqpOption);



	int nErr = 0;
	g_pBarcode = new CVisionComm_Barcode(eVision_BarCode, "BARCODE", nCompanyType);
	nErr = g_pBarcode->Initialize(this, szDriverFileName);
	if( nErr != 0 ){
		AfxMessageBox("Barcode Vision Init Fail!!");
	}

	g_pVisionTestHand_1 = new CVisionComm_TestHand_1(eVision_TestHand1, "TEST HAND 1");
	nErr = g_pVisionTestHand_1->Initialize(this, szDriverFileName);
	if (nErr != 0) {
		AfxMessageBox( "TEST HAND 1 Vision Init Fail!!" );
	}

	g_pVisionTestHand_2 = new CVisionComm_TestHand_2(eVision_TestHand2, "TEST HAND 2");
	nErr = g_pVisionTestHand_2->Initialize(this, szDriverFileName);
	if (nErr != 0) {
		AfxMessageBox("TEST HAND 2 Vision Init Fail!!");
	}

	for (int nCam = 0; nCam < VAT_CAM_MAX; nCam++) {
		g_VatVisionMgr[nCam].Initialize(nCam);
	}
	

	//g_pTestSiteInspUp = new CVisionComm_TestSiteUp(eVision_TestSiteUp, "TEST SITE UP");
	//nErr = g_pTestSiteInspUp->Initialize(this, szDriverFileName);
	//if( nErr != 0 ){
	//	AfxMessageBox("TestSite Up Vision Init Fail!!");
	//}

	//g_pTestSiteInspDn = new CVisionComm_TestSiteDown(eVision_TestSiteDown, "TEST SITE DOWN");	
	//nErr = g_pTestSiteInspDn->Initialize(this, szDriverFileName);
	//if( nErr != 0 ){
	//	AfxMessageBox("TestSite Down Vision Init Fail!!");
	//}

// 	g_pVAT_PC1 = new CVisionComm_VAT_PC1(eVision_VAT_PC1, "VAT_PC1");
// 	nErr = g_pVAT_PC1->Initialize(this, szDriverFileName);
// 	if( nErr != 0 ){
// 		AfxMessageBox("VAT_PC1 Vision Init Fail!!");
// 	}
// 
// 	g_pVAT_PC2 = new CVisionComm_VAT_PC2(eVision_VAT_PC2, "VAT_PC2");
// 	nErr = g_pVAT_PC2->Initialize(this, szDriverFileName);
// 	if (nErr != 0) {
// 		AfxMessageBox("VAT_PC2 Vision Init Fail!!");
// 	}

// 	g_pBacklash = new CVisionComm_Backlash(eVision_VAT, "VAT");
// 	nErr = g_pBacklash->Initialize(this, szDriverFileName);
// 	if (nErr != 0) {
// 		AfxMessageBox("Backlash VAT Vision Init Fail!!");
// 	}
// Board Socket Direction(A1 방향)
//AfxMessageBox("dd");
	int nBoardType = ConfigData.GetInt("SysOption", "Board_Direction", "1");
	g_DMCont.m_dmEQP.SN(NDM0_BoardDirection, nBoardType);

	//Recent Ver Check Initial
	g_DMCont.m_dmHandlerOpt.SB(BDM11_OS_RECENT_VER, true);

	// 상시로 EP Regulator solenoid valve를 열어 놓는다.
	g_IO.out(DO_PRESS_DAMPER_1L_ON, DEF_ON);
	g_IO.out(DO_PRESS_DAMPER_2R_ON, DEF_ON);

	g_TaskTowerLight.Initialize(szDriverFileName);
	
	for(i=0; i<eMaxPressUnitCount; i++){
		g_TaskRegulator[i].Initialize(this, i, szDriverFileName);
	}	
 
 	// Stacker 
	g_TaskStacker[ eSTACKER_01 ].Initialize( szDriverFileName, eSTACKER_01, eStackerType_Empty_Load );
	g_TaskStacker[ eSTACKER_02 ].Initialize( szDriverFileName, eSTACKER_02, eStackerType_Empty_Unload);
	g_TaskStacker[ eSTACKER_03 ].Initialize( szDriverFileName, eSTACKER_03, eStackerType_Load);
	g_TaskStacker[ eSTACKER_04 ].Initialize( szDriverFileName, eSTACKER_04, eStackerType_Unload);
	g_TaskStacker[ eSTACKER_05 ].Initialize( szDriverFileName, eSTACKER_05, eStackerType_Unload );
	g_TaskStacker[ eSTACKER_06 ].Initialize( szDriverFileName, eSTACKER_06, eStackerType_Unload );
	g_TaskStacker[ eSTACKER_07 ].Initialize( szDriverFileName, eSTACKER_07, eStackerType_Unload);
	g_TaskAutoStacker[ eSTACKER_01 ].Initialize( szDriverFileName, eSTACKER_01, eStackerType_Empty_Load );
	g_TaskAutoStacker[ eSTACKER_02 ].Initialize( szDriverFileName, eSTACKER_02, eStackerType_Empty_Unload);
	g_TaskAutoStacker[ eSTACKER_03 ].Initialize( szDriverFileName, eSTACKER_03, eStackerType_Load);
	g_TaskAutoStacker[ eSTACKER_04 ].Initialize( szDriverFileName, eSTACKER_04, eStackerType_Unload);
	g_TaskAutoStacker[ eSTACKER_05 ].Initialize( szDriverFileName, eSTACKER_05, eStackerType_Unload );
	g_TaskAutoStacker[ eSTACKER_06 ].Initialize( szDriverFileName, eSTACKER_06, eStackerType_Unload );
	g_TaskAutoStacker[ eSTACKER_07 ].Initialize( szDriverFileName, eSTACKER_07, eStackerType_Unload);

 	// Press Unit
	for( int i=0; i < eMaxPressUnitCount; i ++ )
	{
		g_TaskPressUnit[ePressUnit_Down_1+i].Initialize(ePressUnit_Down_1+i, szDriverFileName);
		g_TaskAutoPress[ePressUnit_Down_1+i].Initialize(szDriverFileName, (_ePressUnit)(ePressUnit_Down_1+i));
		//g_AppleAgentIF.m_cAgent[ePressUnit_Down_1 + i].Initialize(i);
	
		g_DMCont.m_dmContac_Up.SN(NDM2_PRESS_1_PRE_CONTACT_STATUS + i, FALSE);
	}

	


 	// Transfer
 	g_TaskTransfer.Initialize(szDriverFileName);
	g_TaskAutoTransfer.Initialize( szDriverFileName );

	// Test PP 
	for( int i=0; i < eMaxTestPPCount; i ++ )
	{
		g_TaskTestPP[eTestPP_1+i].Initialize(eTestPP_1+i, szDriverFileName);	
		g_TaskAutoTestPp[eTestPP_1 + i].Initialize(szDriverFileName, (_eTestPPIdx)(eTestPP_1 + i));
	}

	//Load Table 	
	for (int i = 0; i < eMaxLoadTblCount; i++)
	{
		g_TaskLoadTbl[eLoadTbl_1 + i].Initialize(eLoadTbl_1 + i, szDriverFileName);
		g_TaskAutoLdTable[eLoadTbl_1 + i].Initialize(szDriverFileName, (_eLoadTblIdx)(eLoadTbl_1 + i));
	}

	m_nProgressInitCnt += 10;
	g_LibCommCtrl.ProgressBarSetPos(m_nProgressInitCnt);
// 	//Trigger Initial
// 	g_TaskLoadTbl[eLoadTbl_2].InitialTrigger();
// 	g_TaskLoadTbl[eLoadTbl_3].InitialTrigger();

	double dScaleFactor = g_Motor[eAXIS_LOAD_TABLE_1_Y]->GetScaleFactor();
	g_trigger_generator.SetTblResolution(dScaleFactor);
	g_trigger_generator.Connect();

	g_TaskTrayLoadPP.Initialize( eTrayPP_1_LD, szDriverFileName );
	g_TaskAutoTrayLoadPP.Initialize();
	
	g_TaskTrayUnloadPP.Initialize( eTrayPP_2_UD, szDriverFileName );
	g_TaskAutoTrayUnloadPP.Initialize();

	g_TaskTrayFeeder.Initialize( szDriverFileName );
	g_TaskAutoTrayFeeder.Initialize();
	
	g_TaskAutoSystem.Initialize( szDriverFileName );
 	g_TaskSystemCtrl.Initialize(szDriverFileName);

	g_MulitOrg.Initialize(szDriverFileName);

	m_nProgressInitCnt += 10;
	g_LibCommCtrl.ProgressBarSetPos(m_nProgressInitCnt);

	// initialize regulator
	char szSection[128] = {0,};
	ST_REGUL_DATA stRegulData;

	sprintf_s( szSection, sizeof(szSection), "TASK REGULATOR LEFT CONFIG" );

	stRegulData.btParity = (BYTE)ConfigData.GetInt(szSection, "Regulator Parity", "0");
	stRegulData.btSize   = (BYTE)ConfigData.GetInt(szSection, "Regulator Size", "0");
	stRegulData.btStop   = (BYTE)ConfigData.GetInt(szSection, "Regulator Stop", "0");
	stRegulData.dwRate   = (DWORD)ConfigData.GetInt(szSection, "Regulator Rate", "0");
	stRegulData.nPort    = ConfigData.GetInt(szSection, "Regulator Port", "0");
	g_RegulatorIF[ePressRegul_Left].Initialize(this, stRegulData, "D:\techwing\regulator" );

	sprintf_s( szSection, sizeof(szSection), "TASK REGULATOR RIGHT CONFIG" );

	stRegulData.btParity = (BYTE)ConfigData.GetInt(szSection, "Regulator Parity", "0");
	stRegulData.btSize   = (BYTE)ConfigData.GetInt(szSection, "Regulator Size", "0");
	stRegulData.btStop   = (BYTE)ConfigData.GetInt(szSection, "Regulator Stop", "0");
	stRegulData.dwRate   = (DWORD)ConfigData.GetInt(szSection, "Regulator Rate", "0");
	stRegulData.nPort    = ConfigData.GetInt(szSection, "Regulator Port", "0");
	g_RegulatorIF[ePressRegul_Right].Initialize(this, stRegulData, "D:\techwing\regulator");
	

 	// Signal Tower
 	int nAddressSM[eSIGNAL_MAX] = {NDM0_Tower_Running_P, NDM0_Tower_JamCall_P, NDM0_Tower_Cycle_P, NDM0_Tower_Finish_P 
 		                          ,NDM0_Tower_Reset_P, NDM0_Tower_Pause_P, NDM0_Tower_Request_P, NDM0_Tower_TempWait_P 
 		                          ,NDM0_Tower_TestRun_P, NDM0_Tower_Start_P, NDM0_Tower_TempStabil_P};
 	ST_SLT_SIGNAL_TOWER stSignal;
 	g_CokMgr.LoadSignalTower(stSignal);
 
 	for(i=0; i<eSIGNAL_MAX; i++){
 		g_DMCont.m_dmEQP.SN(nAddressSM[i]               , stSignal.nTower[i][eSIGNAL_PRIORTY]);
 		g_DMCont.m_dmEQP.SN(nAddressSM[i]+eSIGNAL_RED   , stSignal.nTower[i][eSIGNAL_RED]);
 		g_DMCont.m_dmEQP.SN(nAddressSM[i]+eSIGNAL_YELLOW, stSignal.nTower[i][eSIGNAL_YELLOW]);
 		g_DMCont.m_dmEQP.SN(nAddressSM[i]+eSIGNAL_GREEN , stSignal.nTower[i][eSIGNAL_GREEN]);
 		g_DMCont.m_dmEQP.SN(nAddressSM[i]+eSIGNAL_SOUND , stSignal.nTower[i][eSIGNAL_SOUND]);
	}

	
    // Tester Start
	g_AppleTestIF.ServerStart();

	// function violation // By SJI
	int nAutoRetestKind = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoRetest);
	//g_ART_Rule.Initialize(nAutoRetestKind, eSortType_ErrStr);

	ZeroMemory(&g_BinCategory, sizeof(ST_CATE_INFO));
	char szData[MAX_PATH] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_CategoryName, szData, sizeof(szData));
	g_CokMgr.LoadCategoryInfo(szData, g_BinCategory);

	//HW Bin 설정 되어있지 않는 Setplate Tray Load 금지
	g_TaskSystemCtrl.SetUseUnloadSetplate(g_BinCategory);

	g_DMCont.m_dmHandlerOpt.GS(SDM11_SLT_ProgramName, szData, sizeof(szData));
	//ZeroMemory(&g_DefaultBinCategory, sizeof(ST_Set_DefaultBin)); <--- cpp check error : Using 'memset' on struct that contains a 'std::vector'.
	g_DefaultBinCategory.clear();
	g_CokMgr.LoadSltPgmDefaultBin(szData, g_DefaultBinCategory);

	MakeHandlerRejectBin();

    // Bin sorting type change
    int nRetType = ConfigData.GetInt("SysOption", "SortType", "0");
    g_DMCont.m_dmEQP.SN( NDM0_SortType, nRetType );// enum eSortingType{ 

	//File Path
	char szPathFileName[MAX_PATH]={0,};
	char szPathTemp[512]={0,};
	char szFilePath[MAX_PATH] ={0,};

	::GetCurrentDirectory(sizeof(szPathTemp), szPathTemp);
	sprintf_s(szPathFileName, sizeof(szPathFileName), "%s\\sys_data\\%s", szPathTemp, SZ_FILE_PATH);
	CConfigData ConfigPathData(szPathFileName);
	
	ConfigPathData.GetString("FILE PATH","Summary File Path","D:\\TechWing\\Summary",szFilePath);

	g_DMCont.m_dmHandlerOpt.SS(SDM11_SummaryFile_Path, szFilePath, MAX_PATH);

	ConfigPathData.GetString("FILE PATH","Batch File Path","D:\\",szFilePath);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_BatchFile_Path, szFilePath, MAX_PATH);

	ConfigPathData.GetString("FILE PATH","LotInfo File Path","D:\\",szFilePath);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_LotInfoFile_Path, szFilePath, MAX_PATH);

	int nOption = ConfigPathData.GetInt("FILE PATH","Batch File Use","0");
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_BatchFile_Use,(BOOL)nOption);

	ConfigPathData.GetString("FILE PATH", "2did Csv File Path","D:\\", szFilePath);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_2did_Csv_File_Path, szFilePath, MAX_PATH);


	//Customer
	char szCompany[64] ={0,};
	CString strTemp =_T("");
	sprintf_s(szPathFileName, sizeof(szPathFileName), "%s\\sys_data\\%s", szPathTemp, SZ_DRIVER_FILE_NAME);
	CConfigData ConfigCustomer(szPathFileName);
	ConfigCustomer.GetString("Company","Company", DEF_COMPANY_AMKOR,szCompany);
	
	if (strcmp(szCompany, DEF_COMPANY_AMKOR) != 0){
		AfxMessageBox("Company name is error!!");
		strTemp.Format("%s", DEF_COMPANY_AMKOR);
	}
	else {
		strTemp = szCompany;
	}
	strTemp.MakeUpper();
	sprintf_s(szCompany,sizeof(szCompany),strTemp);
	g_DMCont.m_dmHandlerOpt.SS(SDM11_Company, szCompany, sizeof(szCompany));


	//  [1/13/2021 jaehoon.jung]
	nOption = ConfigCustomer.GetInt("BACKLASH MODE", "NEW_BACKLASH_MODE", "0");
	g_DMCont.m_dmEQP.SN(NDM0_NEW_BACKLASH_MODE, (BOOL)nOption);
	////

	nOption = ConfigCustomer.GetInt("Company", "PROJECT_NAME", "0");
	g_DMCont.m_dmEQP.SN(NDM0_Handler_Project_Name, nOption);

	int SiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int SiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);

	int nType = SiteCntX * SiteCntY == STATION_MAX_X_SITE * STATION_MAX_Y_SITE ? DEF_STATION_8PARA : DEF_STATION_4PARA;
	g_DMCont.m_dmHandlerOpt.SN(NDM11_PARA_PER_STATION, nType);

	nOption = ConfigCustomer.GetInt("Company", "TESTER_OPTION", "0");
	g_DMCont.m_dmEQP.SN(NDM0_Handler_Tester_Option, (BOOL)nOption);

	nOption = ConfigCustomer.GetInt("IONIZER","SETPLATE_BAR","0");
	g_DMCont.m_dmEQP.SB(BDM0_MD_BAR_ION_USE, (BOOL)nOption);

	nOption = ConfigCustomer.GetInt("REAR SWITCH","REAR_RIGHT_SWITCH_USE","0");
	g_DMCont.m_dmEQP.SB(BDM0_MD_REAR_RIGHT_SWITCH, (BOOL)nOption);
 	
	nOption = ConfigCustomer.GetInt("MODE", "Device Vibration", "0");
	g_DMCont.m_dmEQP.SB(BDM0_MD_VIBRATION_MODE, (BOOL)nOption);
	

	nOption = ConfigCustomer.GetInt("MODE", "Front Slide Door", "0");
	g_DMCont.m_dmEQP.SB(BDM0_MD_FRONT_SLIDE_DOOR, (BOOL)nOption);

	nOption = ConfigCustomer.GetInt("MODE", "Vision Cleaning Device Mode", "0");
	g_DMCont.m_dmEQP.SB(BDM0_MD_VISION_CLEANING_DEVICE, (BOOL)nOption);

	nOption = ConfigCustomer.GetInt("MODE", "Vision Auto Teaching", "0");
	g_DMCont.m_dmVAT.SB(BDM14_VAT_MD_USE, (BOOL)nOption);

	nOption = ConfigCustomer.GetInt("MODE", "All Test Already Pick", "0");
	g_DMCont.m_dmEQP.SB(BDM0_MD_ALL_TEST_ALREADY_PICK, (BOOL)nOption);

	nOption = ConfigCustomer.GetInt("MODE", "Press Arrived Sensor Type", "0");
	g_DMCont.m_dmEQP.SN(NDM0_PRESS_ARRIVED_SENSOR_TYPE, (BOOL)nOption);

	nOption = ConfigCustomer.GetInt("MODE", "Press Vibrate New", "0");
	g_DMCont.m_dmEQP.SN(NDM0_PRESS_VIBRATOR_NEW, (BOOL)nOption);

	int nLanguageType = ConfigCustomer.GetInt( "ALARM MESSAGE", "TYPE", "0" );
	g_DMCont.m_dmHandlerOpt.SN( NDM11_SYS_LANGUAGE , nLanguageType );

	nOption = ConfigCustomer.GetInt("MODE", "Tester SSH", "0");
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_TESTER_SSH, (BOOL)nOption);

	nOption = ConfigCustomer.GetInt("SocketContactCnt", "Limit", "0");
	g_DMCont.m_dmEQP.SN(NDM0_SocketContactCntLimit, nOption);

	nOption = ConfigCustomer.GetInt("SocketContactCnt", "Add Limit", "0");
	g_DMCont.m_dmEQP.SN(NDM0_AddSocketContactLimit, nOption);

	//Table Align Teaching
	nOption = ConfigCustomer.GetInt("MODE", "Table Align Teaching", "0");
	g_DMCont.m_dmEQP.SN(NDM0_TABLE_ALIGN_TEACHING, nOption);

	nOption = ConfigCustomer.GetInt("MODE", "Load Table Vibrator", "0");
	g_DMCont.m_dmEQP.SN(NDM0_TABLE_VIBRATOR_OPTION, nOption);

	//TEST MODE USE SW ENGINEER
	nOption = ConfigCustomer.GetInt("MODE", "Test SW Engineer", "0");
	g_DMCont.m_dmEQP.SN(NDM0_TEST_SW_ENGINEER_OPTION, nOption);


	////////////////////////////////////////////////////

	//OEE function // By SJI
	//g_OEEFile.Initialize();
	
	//FTP function
	//g_Ftp.Initialize();
	m_nProgressInitCnt += 10;
	g_LibCommCtrl.ProgressBarSetPos(m_nProgressInitCnt);

	int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);

	if(nLotStartMode == eStartMD_InitContinue || nLotStartMode ==eStartMD_QA_Continue 
	|| (g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt) != 0 && (nLotStartMode == eStartMD_Initial || nLotStartMode == eStartMD_QA))){
		g_JamRate.LoadBackUpData(eJamRateDataType_Current);
		g_JamRate.LoadBackUpData(eJamRateDataType_First);
	}
	if(nLotStartMode == eStartMD_RetestContinue || (g_DMCont.m_dmEQP.GN(NDM0_CurLotLoadingCnt) != 0 && nLotStartMode == eStartMD_Retest))
	{
		g_JamRate.LoadBackUpData(eJamRateDataType_Current);
		g_JamRate.LoadBackUpData(eJamRateDataType_First);
		g_JamRate.LoadBackUpData(eJamRateDataType_Last);
	}
	
	g_DMCont.m_dmTrayPP.SN(NDM5_HAND_PITCH_MIN_X, DEF_TRAY_PP_MAX_X_PITCH_NARROW);
	g_DMCont.m_dmTrayPP.SN(NDM5_HAND_PITCH_MIN_Y, DEF_TRAY_PP_MAX_Y_PITCH_NARROW);
	g_DMCont.m_dmTrayPP.SN(NDM5_HAND_PITCH_MAX_X, DEF_TRAY_PP_MAX_X_PITCH_WIDE);
	g_DMCont.m_dmTrayPP.SN(NDM5_HAND_PITCH_MAX_Y, DEF_TRAY_PP_MAX_Y_PITCH_WIDE);
	g_DMCont.m_dmTrayPP.SN(NDM5_PICKER_COUNT_X, 4);

	g_TaskTransfer.do_cyl_UpDown(DEF_UP);	// 복동으로 선정되어서 기본 up이 되도록 해주어야한다.


	//backlash apply
	g_DMCont.m_dmHandlerOpt.SB(BDM11_VAT_MODE, FALSE);

	g_VatBacklash.trayloadpp.Load();
	g_TaskTrayLoadPP.ApplyBacklash();
// 	for( int i = eSpeed_5; i < eSpeed_MAX; i++ ) {
// 		g_Motor[ eAXIS_TRAY_PP_1_X ]->SetSpeedBacklash( i, g_VatBacklash.trayloadpp.m_BacklashOffset_X[ i ].x );
// 	}

	g_VatBacklash.trayunloadpp.Load();
	g_TaskTrayUnloadPP.ApplyBacklash();
// 	for( int i = eSpeed_5; i < eSpeed_MAX; i++ ) {
// 		g_Motor[ eAXIS_TRAY_PP_2_X ]->SetSpeedBacklash( i, g_VatBacklash.trayunloadpp.m_BacklashOffset_XY[ i ].x );
// 		g_Motor[ eAXIS_TRAY_PP_2_Y ]->SetSpeedBacklash( i, g_VatBacklash.trayunloadpp.m_BacklashOffset_XY[ i ].y );
// 	}

// 	for (int i = 0; i < MAX_TRAY_PP_CNT; i++)
// 	{
// 		g_VatBacklash.traypp[i].Load();
// 
// 		for( int i = eSpeed_5; i < eSpeed_MAX; i++ ) {
// 			g_Motor[ eAXIS_TRAY_PP_1_X ]->SetSpeedBacklash( i, g_VatBacklash.traypp[ i ].m_BacklashOffset_XY[ i ].x );
// 			g_Motor[ eAXIS_TRAY_PP_1_Y ]->SetSpeedBacklash( i, g_VatBacklash.traypp[ i ].m_BacklashOffset_XY[ i ].y );
// 		}

	//	/*g_TaskTrayPP[i].ApplyBacklash();*/
	//}
	for (int i = 0; i < MAX_TEST_PP_CNT; i++)
	{
		for (int h = 0; h < eMaxTestPPTypeCount; h++)
		{
			g_VatBacklash.testpp[i][h].Load(h);
			g_TaskTestPP[i].ApplyBacklash(h);
		}
	}
	//pickerAlign apply
	g_TaskTrayLoadPP.ApplyVATOffset();
	g_TaskTrayUnloadPP.ApplyVATOffset();

	g_DMCont.m_dmHandlerOpt.SB(BDM11_GUI_OPEN_HISTORY, FALSE);
	g_DMCont.m_dmHandlerOpt.SB(BDM11_GUI_ALIGNMENT_OPEN_HISTORY, FALSE);
	

	double dForcePerPin = g_DMCont.m_dmHandlerOpt.GD(DDM11_DvcContactForce_GF);
	if (dForcePerPin <= 0.0) {
		dForcePerPin = 0.0;
	}
	for (int i = 0; i < eMaxPressUnitCount; i++)
	{
		g_TaskRegulator[i].do_SetForcePerPin(dForcePerPin);
		Sleep(10);
	}

	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);

	if (nTesterOption == eTester_EachSocket)
	{
		for (i = 0; i < MAX_TESTER_EACH_SOCKET; i++) {
			g_DMCont.m_dmEQP.SB(BDM0_TESTER_CONNECTION_1 + i, FALSE);
		}
	}
	else if (nTesterOption != eTester_Dual) // Single Tester [8/27/2021 donghyun.shin]
	{
		for (i = 0; i < eMaxPressUnitCount; i++) {
			g_DMCont.m_dmEQP.SB(BDM0_TESTER_CONNECTION_1 + i, FALSE);
			g_DMCont.m_dmEQP.SB(BDM0_AGENT_CONNECTION_1 + i, FALSE);
		}
	}
	else
	{
		for (i = 0; i < MAX_TESTER_DUAL; i++)
			g_DMCont.m_dmEQP.SB(BDM0_TESTER_CONNECTION_1 + i, FALSE);
	}

	if (nTesterOption == eTester_Agent)
	{
		//g_AppleAgentIF.m_cBonjourService.ThreadStartBrowser();
	}

	g_DMCont.m_dmEQP.SB(BDM0_SYS_LOAD_SUPPLY, TRUE);

	g_DMCont.m_dmHandlerOpt.SN(NDM11_MULTIORIGIN_HISTORY, eMulti_Origin_None);

	g_DMCont.m_dmHandlerOpt.SB(BDM11_STATUS_OPEN_HISTORY, FALSE);

// 	g_pConectSocket = new CConectSocket();
// 	g_pConectSocket->Connect("127.0.0.1", 4000);

	WSAData wsaDataUDP;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaDataUDP) != 0)
		AfxMessageBox("UDP Binsolution WSAStartup Error Socket");

	g_UDPSockBinSolution = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (g_UDPSockBinSolution == INVALID_SOCKET) {
		AfxMessageBox("UDP Binsolution Socket Error");
	}

	//SOCKADDR_IN serverAddr;
	::memset(&g_udpserverAddr, 0, sizeof(g_udpserverAddr));
	g_udpserverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &g_udpserverAddr.sin_addr);
	g_udpserverAddr.sin_port = ::htons(4000);

	::connect(g_UDPSockBinSolution, (SOCKADDR*)&g_udpserverAddr, sizeof(g_udpserverAddr));

	// initialize Motor InTerlock

	nEqpOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_EQP_No);
	if (nEqpOption > 1) //1호기가 아니면
	{
		char szSection[128] = { 0, };
		sprintf_s(szSection, sizeof(szSection), "MOTOR INTERLOCK");

		MIN_TRAY_PP_WORK_ENABLE_POS_um = ConfigData.GetDouble(szSection, "MIN_TRAY_PP_WORK_ENABLE_POS_um", "0");

		INTERLOCK_LD_TRAYPP_X_WORK_ENABLE_FEEDER_START_POS = ConfigData.GetDouble(szSection, "INTERLOCK_LD_TRAYPP_X_WORK_ENABLE_FEEDER_START_POS", "0");
		INTERLOCK_LD_TRAYPP_X_WORK_ENABLE_FEEDER_END_POS = ConfigData.GetDouble(szSection, "INTERLOCK_LD_TRAYPP_X_WORK_ENABLE_FEEDER_END_POS", "0");
		INTERLOCK_LD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_LEFT = ConfigData.GetDouble(szSection, "INTERLOCK_LD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_LEFT", "0");
		INTERLOCK_LD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_RIGHT = ConfigData.GetDouble(szSection, "INTERLOCK_LD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_RIGHT", "0");

																			   //Tray Unload Hand
		INTERLOCK_UD_TRAYPP_Y_WORK_ENABLE_POS_TRANSFER = ConfigData.GetDouble(szSection, "INTERLOCK_UD_TRAYPP_Y_WORK_ENABLE_POS_TRANSFER", "0");
		INTERLOCK_UD_TRAYPP_X_WORK_ENABLE_FEEDER_START_POS = ConfigData.GetDouble(szSection, "INTERLOCK_UD_TRAYPP_X_WORK_ENABLE_FEEDER_START_POS", "0");
		INTERLOCK_UD_TRAYPP_X_WORK_ENABLE_FEEDER_END_POS = ConfigData.GetDouble(szSection, "INTERLOCK_UD_TRAYPP_X_WORK_ENABLE_FEEDER_END_POS", "0");
		INTERLOCK_UD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_LEFT = ConfigData.GetDouble(szSection, "INTERLOCK_UD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_LEFT", "0");
		INTERLOCK_UD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_RIGHT = ConfigData.GetDouble(szSection, "INTERLOCK_UD_TRAYPP_X_POS_WITH_TRAYPP_Z_AND_TBL_RIGHT", "0");
		
		INTERLOCK_BASE_TRAYPP_Y_POS_WITH_TRAYPP_Z_AND_TBL_POSSIBLE = ConfigData.GetDouble(szSection, "INTERLOCK_BASE_TRAYPP_Y_POS_WITH_TRAYPP_Z_AND_TBL_POSSIBLE", "0");
																		   //Test Hand
		INTERLOCK_TESTPP_1_X_STATION_START_POS = ConfigData.GetDouble(szSection, "INTERLOCK_TESTPP_1_X_STATION_START_POS", "0");
		INTERLOCK_TESTPP_2_X_STATION_START_POS = ConfigData.GetDouble(szSection, "INTERLOCK_TESTPP_2_X_STATION_START_POS", "0");
		INTERLOCK_TESTPP_X_POS_WITH_TESTPP_Z_AND_TBL_FRONT = ConfigData.GetDouble(szSection, "INTERLOCK_TESTPP_X_POS_WITH_TESTPP_Z_AND_TBL_FRONT", "0");
		INTERLOCK_TESTPP_X_POS_WITH_TESTPP_Z_AND_TBL_REAR = ConfigData.GetDouble(szSection, "INTERLOCK_TESTPP_X_POS_WITH_TESTPP_Z_AND_TBL_REAR", "0");
		INTERLOCK_TESTPP_X_SAFETY_POS = ConfigData.GetDouble(szSection, "INTERLOCK_TESTPP_X_SAFETY_POS", "0");

		INTERLOCK_HAND_LENGTH = ConfigData.GetDouble(szSection, "INTERLOCK_HAND_LENGTH", "0");
		INTERLOCK_STATION_GAP = ConfigData.GetDouble(szSection, "INTERLOCK_STATION_GAP", "0");
		INTERLOCK_TESTPP_Y_STATION_1_START_POS_LOAD = ConfigData.GetDouble(szSection, "INTERLOCK_TESTPP_Y_STATION_1_START_POS_LOAD", "0");
		INTERLOCK_TESTPP_Y_STATION_1_END_POS_LOAD = ConfigData.GetDouble(szSection, "INTERLOCK_TESTPP_Y_STATION_1_END_POS_LOAD", "0");
		INTERLOCK_TESTPP_Y_STATION_2_START_POS_LOAD = ConfigData.GetDouble(szSection, "INTERLOCK_TESTPP_Y_STATION_2_START_POS_LOAD", "0");
		INTERLOCK_TESTPP_Y_STATION_2_END_POS_LOAD = ConfigData.GetDouble(szSection, "INTERLOCK_TESTPP_Y_STATION_2_END_POS_LOAD", "0");
		INTERLOCK_TESTPP_Y_STATION_3_START_POS_LOAD = ConfigData.GetDouble(szSection, "INTERLOCK_TESTPP_Y_STATION_3_START_POS_LOAD", "0");
		INTERLOCK_TESTPP_Y_STATION_3_END_POS_LOAD = ConfigData.GetDouble(szSection, "INTERLOCK_TESTPP_Y_STATION_3_END_POS_LOAD", "0");
		INTERLOCK_TESTPP_Y_STATION_4_START_POS_LOAD = ConfigData.GetDouble(szSection, "INTERLOCK_TESTPP_Y_STATION_4_START_POS_LOAD", "0");
		INTERLOCK_TESTPP_Y_STATION_4_END_POS_LOAD = ConfigData.GetDouble(szSection, "INTERLOCK_TESTPP_Y_STATION_4_END_POS_LOAD", "0");

		INTERLOCK_TESTPP_Y_STATION_1_START_POS_UNLOAD = ConfigData.GetDouble(szSection, "INTERLOCK_TESTPP_Y_STATION_1_START_POS_UNLOAD", "0");
		INTERLOCK_TESTPP_Y_STATION_1_END_POS_UNLOAD = ConfigData.GetDouble(szSection, "INTERLOCK_TESTPP_Y_STATION_1_END_POS_UNLOAD", "0");
		INTERLOCK_TESTPP_Y_STATION_2_START_POS_UNLOAD = ConfigData.GetDouble(szSection, "INTERLOCK_TESTPP_Y_STATION_2_START_POS_UNLOAD", "0");
		INTERLOCK_TESTPP_Y_STATION_2_END_POS_UNLOAD = ConfigData.GetDouble(szSection, "INTERLOCK_TESTPP_Y_STATION_2_END_POS_UNLOAD", "0");
		INTERLOCK_TESTPP_Y_STATION_3_START_POS_UNLOAD = ConfigData.GetDouble(szSection, "INTERLOCK_TESTPP_Y_STATION_3_START_POS_UNLOAD", "0");
		INTERLOCK_TESTPP_Y_STATION_3_END_POS_UNLOAD = ConfigData.GetDouble(szSection, "INTERLOCK_TESTPP_Y_STATION_3_END_POS_UNLOAD", "0");
		INTERLOCK_TESTPP_Y_STATION_4_START_POS_UNLOAD = ConfigData.GetDouble(szSection, "INTERLOCK_TESTPP_Y_STATION_4_START_POS_UNLOAD", "0");
		INTERLOCK_TESTPP_Y_STATION_4_END_POS_UNLOAD = ConfigData.GetDouble(szSection, "INTERLOCK_TESTPP_Y_STATION_4_END_POS_UNLOAD", "0");

		INTERLOCK_TESTPP1_X_CLEAN_TABLE_LIMIT = ConfigData.GetDouble(szSection, "INTERLOCK_TESTPP1_X_CLEAN_TABLE_LIMIT", "0");
		INTERLOCK_TESTPP2_X_CLEAN_TABLE_LIMIT = ConfigData.GetDouble(szSection, "INTERLOCK_TESTPP2_X_CLEAN_TABLE_LIMIT", "0");

		//Loading Table
		INTERLOCK_LOADING_TBL_POS_BOUNDARY = ConfigData.GetDouble(szSection, "INTERLOCK_LOADING_TBL_POS_BOUNDARY", "0");

		//Press
		INTERLOCK_PRESS_Z_SAFETY_POS = ConfigData.GetDouble(szSection, "INTERLOCK_PRESS_Z_SAFETY_POS", "0");

														//feeder
		INTERLOCK_FEEDER_SAFETY_POS_ENC = ConfigData.GetDouble(szSection, "INTERLOCK_FEEDER_SAFETY_POS_ENC", "0");
		INTERLOCK_FEEDER_POS_BOUNDARY = ConfigData.GetDouble(szSection, "INTERLOCK_FEEDER_POS_BOUNDARY", "0");

		INTERLOCK_LD_TRAY_PP_PKR1_FEEDER_LEFT_IMPACT_POS = ConfigData.GetDouble(szSection, "INTERLOCK_LD_TRAY_PP_PKR1_FEEDER_LEFT_IMPACT_POS", "0");
		INTERLOCK_LD_TRAY_PP_PKR2_FEEDER_LEFT_IMPACT_POS = ConfigData.GetDouble(szSection, "INTERLOCK_LD_TRAY_PP_PKR2_FEEDER_LEFT_IMPACT_POS", "0");
		INTERLOCK_LD_TRAY_PP_PKR3_FEEDER_LEFT_IMPACT_POS = ConfigData.GetDouble(szSection, "INTERLOCK_LD_TRAY_PP_PKR3_FEEDER_LEFT_IMPACT_POS", "0");
		INTERLOCK_LD_TRAY_PP_PKR4_FEEDER_LEFT_IMPACT_POS = ConfigData.GetDouble(szSection, "INTERLOCK_LD_TRAY_PP_PKR4_FEEDER_LEFT_IMPACT_POS", "0");
		INTERLOCK_LD_TRAY_PP_PKR1_FEEDER_RIGHT_IMPACT_POS = ConfigData.GetDouble(szSection, "INTERLOCK_LD_TRAY_PP_PKR1_FEEDER_RIGHT_IMPACT_POS", "0");
		INTERLOCK_LD_TRAY_PP_PKR2_FEEDER_RIGHT_IMPACT_POS = ConfigData.GetDouble(szSection, "INTERLOCK_LD_TRAY_PP_PKR2_FEEDER_RIGHT_IMPACT_POS", "0");
		INTERLOCK_LD_TRAY_PP_PKR3_FEEDER_RIGHT_IMPACT_POS = ConfigData.GetDouble(szSection, "INTERLOCK_LD_TRAY_PP_PKR3_FEEDER_RIGHT_IMPACT_POS", "0");
		INTERLOCK_LD_TRAY_PP_PKR4_FEEDER_RIGHT_IMPACT_POS = ConfigData.GetDouble(szSection, "INTERLOCK_LD_TRAY_PP_PKR4_FEEDER_RIGHT_IMPACT_POS", "0");

		INTERLOCK_UD_TRAY_PP_PKR1_FEEDER_LEFT_IMPACT_POS = ConfigData.GetDouble(szSection, "INTERLOCK_UD_TRAY_PP_PKR1_FEEDER_LEFT_IMPACT_POS", "0");
		INTERLOCK_UD_TRAY_PP_PKR2_FEEDER_LEFT_IMPACT_POS = ConfigData.GetDouble(szSection, "INTERLOCK_UD_TRAY_PP_PKR2_FEEDER_LEFT_IMPACT_POS", "0");
		INTERLOCK_UD_TRAY_PP_PKR3_FEEDER_LEFT_IMPACT_POS = ConfigData.GetDouble(szSection, "INTERLOCK_UD_TRAY_PP_PKR3_FEEDER_LEFT_IMPACT_POS", "0");
		INTERLOCK_UD_TRAY_PP_PKR4_FEEDER_LEFT_IMPACT_POS = ConfigData.GetDouble(szSection, "INTERLOCK_UD_TRAY_PP_PKR4_FEEDER_LEFT_IMPACT_POS", "0");
		INTERLOCK_UD_TRAY_PP_PKR1_FEEDER_RIGHT_IMPACT_POS = ConfigData.GetDouble(szSection, "INTERLOCK_UD_TRAY_PP_PKR1_FEEDER_RIGHT_IMPACT_POS", "0");
		INTERLOCK_UD_TRAY_PP_PKR2_FEEDER_RIGHT_IMPACT_POS = ConfigData.GetDouble(szSection, "INTERLOCK_UD_TRAY_PP_PKR2_FEEDER_RIGHT_IMPACT_POS", "0");
		INTERLOCK_UD_TRAY_PP_PKR3_FEEDER_RIGHT_IMPACT_POS = ConfigData.GetDouble(szSection, "INTERLOCK_UD_TRAY_PP_PKR3_FEEDER_RIGHT_IMPACT_POS", "0");
		INTERLOCK_UD_TRAY_PP_PKR4_FEEDER_RIGHT_IMPACT_POS = ConfigData.GetDouble(szSection, "INTERLOCK_UD_TRAY_PP_PKR4_FEEDER_RIGHT_IMPACT_POS", "0");

	}

	//VAT Data Load
	g_VATCommonData.LoadVATData();
	g_VATCommonStaticData.LoadVATStaticData();


	m_nProgressInitCnt += 10;
	g_LibCommCtrl.ProgressBarSetPos(m_nProgressInitCnt);

}

void CSLT_MCView::Close_GlobalObject()
{
	if( g_pDlgSimulDioSLT ){
		g_pDlgSimulDioSLT->DestroyWindow();
		delete g_pDlgSimulDioSLT;
		g_pDlgSimulDioSLT = NULL;
	}

	g_TaskAutoSystem.Finalize();
 	g_TaskSystemCtrl.Finalize();

	//g_TaskAutoStacker.Finalize();
	g_TaskAutoStacker[ eSTACKER_01 ].Finalize();
	g_TaskAutoStacker[ eSTACKER_02 ].Finalize();
	g_TaskAutoStacker[ eSTACKER_03 ].Finalize();
	g_TaskAutoStacker[ eSTACKER_04 ].Finalize();
	g_TaskAutoStacker[ eSTACKER_05 ].Finalize();
	g_TaskAutoStacker[ eSTACKER_06 ].Finalize();
	g_TaskAutoStacker[ eSTACKER_07 ].Finalize();

	g_TaskAutoTrayUnloadPP.Finalize();
	g_TaskAutoTrayLoadPP.Finalize();

	g_TaskAutoLdTable[eLoadTbl_1].Finalize();
	g_TaskAutoLdTable[eLoadTbl_2].Finalize();
	g_TaskAutoLdTable[eLoadTbl_3].Finalize();
	g_TaskAutoLdTable[eLoadTbl_4].Finalize();

	g_TaskAutoTrayFeeder.Finalize();

	g_TaskAutoTestPp[eTestPP_1].Finalize();
	g_TaskAutoTestPp[eTestPP_2].Finalize();
 	


	g_TaskStacker[ eSTACKER_01 ].Finalize();
	g_TaskStacker[ eSTACKER_02 ].Finalize();
	g_TaskStacker[ eSTACKER_03 ].Finalize();
	g_TaskStacker[ eSTACKER_04 ].Finalize();
	g_TaskStacker[ eSTACKER_05 ].Finalize();
	g_TaskStacker[ eSTACKER_06 ].Finalize();
	g_TaskStacker[ eSTACKER_07 ].Finalize();
 
 	g_TaskPressUnit[ePressUnit_Down_1].Finalize();
 	g_TaskPressUnit[ePressUnit_Down_2].Finalize();
 	g_TaskPressUnit[ePressUnit_Down_3].Finalize();
 	g_TaskPressUnit[ePressUnit_Down_4].Finalize();
 	g_TaskPressUnit[ePressUnit_Down_5].Finalize();
 	g_TaskPressUnit[ePressUnit_Down_6].Finalize();
 	g_TaskPressUnit[ePressUnit_Down_7].Finalize();
 	g_TaskPressUnit[ePressUnit_Down_8].Finalize();
//  	g_TaskPressUnit[ePressUnit_Up_1  ].Finalize();
//  	g_TaskPressUnit[ePressUnit_Up_2  ].Finalize();
//  	g_TaskPressUnit[ePressUnit_Up_3  ].Finalize();
//  	g_TaskPressUnit[ePressUnit_Up_4  ].Finalize();
//  	g_TaskPressUnit[ePressUnit_Up_5  ].Finalize();
//  	g_TaskPressUnit[ePressUnit_Up_6  ].Finalize();
//  	g_TaskPressUnit[ePressUnit_Up_7  ].Finalize();
//  	g_TaskPressUnit[ePressUnit_Up_8  ].Finalize();
 	
 	g_TaskTransfer.Finalize();
 
 	g_TaskTestPP[eTestPP_1].Finalize();
 	g_TaskTestPP[eTestPP_2].Finalize();
 
	g_TaskTrayUnloadPP.Finalize();
	g_TaskTrayLoadPP.Finalize();

	g_TaskTrayFeeder.Finalize();

 	g_TaskLoadTbl[eLoadTbl_1].Finalize();
	g_TaskLoadTbl[eLoadTbl_2].Finalize();
	g_TaskLoadTbl[eLoadTbl_3].Finalize();
	g_TaskLoadTbl[eLoadTbl_4].Finalize();

	g_TaskTowerLight.Finalize();
	//g_TaskVibrator.Finalize();

	// function violation // By SJI
	//g_ART_Rule.Finalize();

	//OEE function  By SJI
	//g_OEEFile.Finalize();

	int i=0;
	for(i=0; i<eMaxPressUnitCount; i++){
		g_TaskRegulator[i].Finalize();
	}

	if( g_pBarcode != NULL ){
		g_pBarcode->Finalize();
		delete g_pBarcode;
		g_pBarcode = NULL;
	}
	
	if (g_pVisionTestHand_1 != NULL) {
		g_pVisionTestHand_1->Finalize();
		delete g_pVisionTestHand_1;
		g_pVisionTestHand_1 = NULL;
	}

	if (g_pVisionTestHand_2 != NULL) {
		g_pVisionTestHand_2->Finalize();
		delete g_pVisionTestHand_2;
		g_pVisionTestHand_2 = NULL;
	}


 	g_GuiComm.Finalize();
 
  	char szPackageName[256] = {0,};
  	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szPackageName, sizeof(szPackageName));
  	g_CokMgr.Finalize(szPackageName);

 	g_Motor.Destroy();
 	g_IO.Destroy();
 	g_AIO.Finalize();
 	g_DMCont.Destroy();

	g_Error.Finalize();
	g_DB.Destroy();

	g_MulitOrg.Finalize();

}

void CSLT_MCView::InitControl()
{
	m_btnSetup.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnSetup.SetIcon(IDI_BTN_SETUP);
	m_btnSetup.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnManual.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnManual.SetIcon(IDI_BTN_MAIN);
	m_btnManual.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnMonitor.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnMonitor.SetIcon(IDI_BTN_YIELD);
	m_btnMonitor.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnClose.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnClose.SetIcon(IDI_CANCLE);
	m_btnClose.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnExit.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnExit.SetIcon(IDI_BTN_SHUT_DOWN);
	m_btnExit.SetAlign(CButtonST::ST_ALIGN_VERT);
}

void CSLT_MCView::OnBnClickedBtnMcMenuSetup()
{
	////g_TaskSystemCtrl.YieldContiFailClear(0, 0, 0);
	//g_TaskSystemCtrl.AddYieldContiFail(0, 0, 0, 0); 
	//g_TaskSystemCtrl.CalcYieldContiFailMonitoring();
	//g_TaskSystemCtrl.AddYieldContiFail(0, 0, 0, 0); 
	//g_TaskSystemCtrl.CalcYieldContiFailMonitoring();
	//g_TaskSystemCtrl.AddYieldContiFail(0, 1, 0, 0); 
	//g_TaskSystemCtrl.CalcYieldContiFailMonitoring();
	//g_TaskSystemCtrl.AddYieldContiFail(0, 1, 0, 0); 
	//g_TaskSystemCtrl.CalcYieldContiFailMonitoring();
	//g_TaskSystemCtrl.AddYieldContiFail(0, 0, 0, 0); 
	//g_TaskSystemCtrl.CalcYieldContiFailMonitoring();
	//g_TaskSystemCtrl.AddYieldContiFail(0, 0, 0, 0); 
	//g_TaskSystemCtrl.CalcYieldContiFailMonitoring();
	//g_TaskSystemCtrl.AddYieldContiFail(0, 0, 0, 0); 
	//g_TaskSystemCtrl.CalcYieldContiFailMonitoring();
	//return;	

	CDlgSetup dlg;
	dlg.DoModal();
}


void CSLT_MCView::OnBnClickedBtnMcMenuManual()
{
	CDlgManu dlg;
	int e = dlg.DoModal();
	int a = 1;
}


void CSLT_MCView::OnBnClickedBtnMcMenuMonitor()
{
	//if (g_pDlgMotorMonitor != NULL)
	//	g_pDlgMotorMonitor->ShowWindow(SW_SHOW);
	
 	//CDlgMon dlg;
 	//dlg.DoModal();
}

void CSLT_MCView::OnBnClickedBtnMcMenuClose()
{
	CMainFrame* pMainFrame = (CMainFrame*)::AfxGetMainWnd();
	pMainFrame->MoveToTrayIcon();
}


void CSLT_MCView::OnBnClickedBtnMcMenuShutDown()
{
    int nRet = AfxMessageBox("Do you want Exit Software??", MB_YESNO|MB_TOPMOST );
    if( nRet == IDYES ){
        ::AfxGetMainWnd()->SendMessage(WM_COMMAND,ID_APP_EXIT);        
    }
}



void CSLT_MCView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// 3D 제거
	ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
}


void CSLT_MCView::OnBnClickedBtnMcMenuDioSim()
{
	g_IO.ShowDialogSimul();
}


void CSLT_MCView::OnBnClickedBtnMcMenuAioSim()
{
	g_AIO.ShowDialogSimul();
}


void CSLT_MCView::OnBnClickedBtnMcMenuMotionSim()
{
	g_Motor.ShowDialogSimul();
}


void CSLT_MCView::OnBnClickedBtnMcMenuShowDebugView()
{
	g_pDlgDebugView->ShowWindow(SW_SHOW);
}

void CSLT_MCView::MakeHandlerRejectBin()
{
	int nSiteCntX = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_X);
	int nSiteCntY = g_DMCont.m_dmTestPP.GN(NDM4_TestSite_Div_Y);
	BOOL bAutoRetestUseMode = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoRetestSiteUse);
	//BOOL bAutoGrrUseMode	= g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_AutoGrrUse);

	BOOL bAutoGrrUseMode = FALSE;
	int nLotStartMode = g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_FTRTQCContinue);
	if (nLotStartMode == eStartMD_AutoGRR_Continue || nLotStartMode == eStartMD_AutoGRR)
		bAutoGrrUseMode = TRUE;

	BOOL bAutoLoopUseMode = FALSE;
	if (nLotStartMode == eStartMD_AutoLoop_Continue || nLotStartMode == eStartMD_AutoLoop)
		bAutoLoopUseMode = TRUE;

	ST_DD_DEVICE stErrData;
	ST_DD_DEVICE stSiteData;
	char szHWbin[16] = {0,};
	
	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	strcpy_s(stErrData.szErrString, sizeof(stErrData.szErrString), DEF_HANDLER_ERR_DEVICE);
	strcpy_s(stErrData.szBinResult, sizeof(stErrData.szBinResult), "98");
	sprintf_s(szHWbin, sizeof(szHWbin), "4");
	stErrData.nBinColor = eFailBin;


	//for(int j=0; j < g_BinCategory.nIndexCount; j++)
	//{
	//	if(strcmp(DEF_HANDLER_ERR_DEVICE, g_BinCategory.aCateInfo[j].szDesc) == 0){
	//		strcpy_s(stErrData.szBinResult, sizeof(stErrData.szBinResult), g_BinCategory.aCateInfo[j].szSWBin);
	//		sprintf_s(szHWbin, sizeof(szHWbin), g_BinCategory.aCateInfo[j].szHWBin);
	//		// Bin Color = Msg Data Pass/Fail
	//		stErrData.nBinColor = atoi(g_BinCategory.aCateInfo[j].szPassFail); /*Pass/Fail 받은 것*/
	//		strcpy_s(stErrData.szErrString, sizeof(stErrData.szErrString), g_BinCategory.aCateInfo[j].szDesc);
	//		break;
	//	}
	//}

	int nTestKindCnt[3] = { 0, }; // Daily Report data 0 : FT, 1: RT1, 2: RT2

	char szEqpName[128] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_UDP_HOST_NAME, szEqpName, sizeof(szEqpName));

	for (int i=0; i < eMaxPressUnitCount; i++)
	{
		BOOL bNeedStatusTestEnd = FALSE;
		for(int nY = 0 ; nY < nSiteCntY ; nY++ )
		{
			for(int nX = 0 ; nX < nSiteCntX ; nX++ )
			{
				g_TaskPressUnit[i].GetSiteDeviceData(&stSiteData, nX, nY);

				// device 가 없다면 할것이 없다.
				if( stSiteData.sExist != DEF_EXIST ) continue;

				if(stSiteData.nTestingInfo == eTestSite_TestStart
					|| stSiteData.nTestingInfo == eTestSite_Testing)
				{
					// Get local time
					SYSTEMTIME timeEnd;
					::GetLocalTime( &timeEnd );

					//Error code compare result data
					CString strTRL = _T(""), strStartTm = _T(""), strEndTm = _T("");
										

					// change HE bin result
					strcpy_s(stSiteData.szBinResult, sizeof(stErrData.szBinResult), stErrData.szBinResult);
					stSiteData.nBinColor = stErrData.nBinColor;
					strcpy_s(stSiteData.szErrString, sizeof(stSiteData.szErrString), stErrData.szErrString);

					

					// Record Test Result log file
					strStartTm.Format("%04d%02d%02d%02d%02d%02d%03d"
						, timeEnd.wYear, timeEnd.wMonth, timeEnd.wDay, timeEnd.wHour, timeEnd.wMinute, timeEnd.wSecond, timeEnd.wMilliseconds);
					strEndTm.Format("%04d%02d%02d%02d%02d%02d%03d"
						, timeEnd.wYear, timeEnd.wMonth, timeEnd.wDay, timeEnd.wHour, timeEnd.wMinute, timeEnd.wSecond, timeEnd.wMilliseconds);
					strTRL.Format("%s,%s,%s,%d,%d,%s,%s,fail,%s,%s"
						, stSiteData.sz2DBarcode
						, "", szEqpName
						, g_AppleTestIF.GetStationID(i+1)
						, nX+nY*nSiteCntX+1
						, (LPCTSTR)strStartTm, (LPCTSTR)strEndTm
						, stSiteData.szErrString
						, szHWbin);
					
					TWLOG.MakeLog(static_cast<int>(eLogID_PRODUCTCSV), Debug, "", __LINE__, NULL, strTRL);
//					TWLOG.MakeLog(static_cast<int>(eLogID_2DIDCSV), Debug, "", __LINE__, NULL, strTRL);

					// Set Site number
					sprintf_s(stSiteData.szTestSiteNum, sizeof(stSiteData.szTestSiteNum),"%d", i);
					stSiteData.cSubSiteNum = CPoint(nX,nY);

					// Set Testing Count
					int nTestCnt = stSiteData.nTestingCnt;

					if (bAutoGrrUseMode != TRUE && bAutoLoopUseMode != TRUE)
					{	
						// JAM RATE DATA RECORD
						ASSERT(nTestCnt >= eARB_NotTest && nTestCnt <= eARB_3stTestEnd);
						int nDvcTestCnt = nTestCnt;
						if(bAutoRetestUseMode ==TRUE && g_DMCont.m_dmHandlerOpt.GN(NDM11_MD_AutoRetest) == eAutoRetest_AB && nTestCnt > eARB_NotTest){
							nDvcTestCnt -= 1; //AB의 경우는 1stEnd 후에 바로 2stEnd로 바꾸어주기 때문에 빼야함.
						}

						stSiteData.stRecInfo.stDvcInfo[nDvcTestCnt].nHwBin = atoi(szHWbin)-1;
						stSiteData.stRecInfo.stDvcInfo[nDvcTestCnt].nPF    = eFailBin;
						stSiteData.stRecInfo.stDvcInfo[nDvcTestCnt].nSiteNo= atoi(stSiteData.szTestSiteNum);
						stSiteData.stRecInfo.stDvcInfo[nDvcTestCnt].cSubSiteNo = stSiteData.cSubSiteNum;

						sprintf_s(stSiteData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrStr , sizeof(stSiteData.stRecInfo.stDvcInfo[nDvcTestCnt].szErrStr) , stSiteData.szErrString);

						int nArtCnt = g_DMCont.m_dmEQP.GN(NDM0_ART_FT_TOTAL+ (nTestCnt*2));
						g_DMCont.m_dmEQP.SN(NDM0_ART_FT_TOTAL+ (nTestCnt*2), ++nArtCnt);
						
						nTestKindCnt[nTestCnt]++;
					}

					BOOL bIsDefaultBin = g_TaskSystemCtrl.IsDefaultBin(stSiteData);

					if (bIsDefaultBin != TRUE)
					{
						stSiteData.nTestingCnt = nTestCnt = g_TaskPressUnit[i].GetCalcTestingCnt(nTestCnt, stSiteData);
					}
// 					if (nProjectOption == DEF_PROJECT_MARLIN) {
// 						stSiteData.nTestingCnt = nTestCnt = g_TaskPressUnit[i].GetCalcTestingCnt(nTestCnt, stSiteData);
// 					}

					g_TaskPressUnit[i].SetFailHistoryAndPFcnt(nX, nY, eFailBin, nTestCnt);

					// set hw bin history
					g_TaskPressUnit[i].SetHwBinHistory(nX, nY, atoi(szHWbin));

					// setting device data
					stSiteData.nTestingInfo = eTestSite_TestEnd;
					g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_SITE_1+i, nX, nY, &stSiteData);

					// set last error string
					g_DMCont.m_dmContac_Dn.SS(SDM1_SiteLastDesc1_1 + i*STATION_MAX_PARA + nX + nY * STATION_MAX_X_SITE, stSiteData.szErrString, sizeof(stSiteData.szErrString));

					bNeedStatusTestEnd = TRUE;
				}
				else {
					g_TaskPressUnit[i].SetNotTestHistory(nX, nY);
				}

				if(stSiteData.nTestingInfo == eTestSite_Contacting){
					//stSiteData.nTestingInfo = eTestSite_DvcLoading;
					//g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_SITE_1+i, nX, nY, &stSiteData);
					g_TaskPressUnit[i].SetSiteDvcTestingInfo(nX, nY, eTestSite_Contacting);
				}
				if (stSiteData.nTestingInfo == eTestSite_TestTimeOut) {
					//stSiteData.nTestingInfo = eTestSite_TestEnd;
					//g_DMCont.m_dmDevice.SetDeviceData(eDeviceStage_TEST_SITE_1 + i, nX, nY, &stSiteData);
					g_TaskPressUnit[i].SetSiteDvcTestingInfo(nX, nY, eTestSite_TestEnd);
				}
			}
		}

		g_DMCont.m_dmEQP.SB(BDM0_TEST_STATUS_SITE1 + i, FALSE);    // Set testing status off
		g_DMCont.m_dmEQP.SB(BDM0_PRESS_CONTACT_STATUS_SITE1 + i, FALSE);    // Set Contact status off
		
		//if( bNeedStatusTestEnd == TRUE )
		//	g_DMCont.m_dmContac_Up.SN(NDM2_StationStatus_1+i, eTestSite_TestEnd);
	}

	if (nTestKindCnt[eARB_1stTestEnd - 1] != 0 || nTestKindCnt[eARB_2stTestEnd - 1] != 0 || nTestKindCnt[eARB_3stTestEnd - 1] != 0) {
		//Daily Report
		ST_DAILY_REPORT stDailyData;
		stDailyData.nART_FT_Total_Cnt = nTestKindCnt[eARB_1stTestEnd - 1];
		stDailyData.nART_RT1_Total_Cnt = nTestKindCnt[eARB_2stTestEnd - 1];
		stDailyData.nART_RT2_Total_Cnt = nTestKindCnt[eARB_3stTestEnd - 1];

		g_TaskSystemCtrl.AddDailyReportData(stDailyData);
	}
}


void CSLT_MCView::OnBnClickedBtnWmxReconenect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

// 	_eWMX3Reconnect eNowWMX3Sts;
// 
// 	eNowWMX3Sts = (_eWMX3Reconnect) g_TaskSystemCtrl.GetWMX3ReconnectSts();
// 
// 
// 	if (eNowWMX3Sts != _eWMX3Reconnect::eCommIdle)
// 	{
// 		AfxMessageBox(_T("already excute Reconnect"));
// 	}
// 
// 	eNowWMX3Sts = _eWMX3Reconnect::eCommReconnectProcess;

}
