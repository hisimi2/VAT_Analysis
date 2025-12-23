// DlgManu.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgManu.h"
#include "afxdialogex.h"
#include "MainFrm.h"

#include "DEF_COMMON.h"

#include "DlgManuMotor.h"
#include "DlgManuMotorTeaching.h"
#include "DlgManuPressUnit.h"
#include "DlgManuTestPP.h"
#include "DlgManuTrayPP.h"
#include "DlgManuSetplate.h"
#include "DlgManuTesterIF.h"
#include "DlgManuRegul.h"
#include "DlgManuTransfer.h"
#include "DlgAxisActionMove.h"
#include "DlgManuVision.h"
#include "DlgManuFeeder.h"

#include "DlgCommand_Stacker.h"
#include "DlgCommand_Transfer.h"
#include "DlgCommand_TrayPP.h"
#include "DlgCommand_LoadTbl.h"
#include "DlgCommand_TestPP.h"
#include "DlgCommand_PressUnit.h"
#include "DlgCommand_Param.h"
#include "DlgCommand_Feeder.h"

#include "DlgManuLoadTbl.h"

//#include "DlgManuVAT.h"
// 
// #include "DlgManuAgentIF.h"

#include "DlgCylinderTimeCheck.h"

using namespace base_seq_cmd;

// CDlgManu 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgManu, CDialogEx)

CDlgManu::CDlgManu(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgManu::IDD, pParent)
{
	m_bManualCmdExcute = FALSE;
	m_pDlgHomeCheckStatus = NULL;
	m_pDlgContactAndSafety = NULL;
	m_pDlgManuTesterIF = NULL;
	//m_pDlgManuMotorTeaching = NULL;
	
}

CDlgManu::~CDlgManu()
{
	// press unit
	m_cBmpPress.DeleteObject();

	// setplate & stacker
	m_cBmpSetplateStacker.DeleteObject();

	// loadtable & test pp
	for(int i=0; i<4; i++) {
		m_cBmpLoadTestPP[i].DeleteObject();
	}

	// tray pp
	m_cBmpTrayPP1.DeleteObject();
	m_cBmpTrayPP2.DeleteObject();

	m_cBmpFeeder.DeleteObject();

	// test pp
	m_cBmpTestPP[0].DeleteObject();
	for(int i=0; i<3; i++) {
		m_cBmpTestPP[i+1].DeleteObject();
	}
	// load table
	for(int i=0; i<4; i++) {
		m_cBmpLoadTable[i].DeleteObject();
	}
	// tray pp axis
	m_cBmpTrayPP1Axis.DeleteObject();
	m_cBmpTrayPP2Axis.DeleteObject();

	m_cBmpFeederAxis.DeleteObject();

	// stacker ev left/right
	m_cBmpStackerEVLeft.DeleteObject();
	m_cBmpStackerEVRight.DeleteObject();

	// transfer ev
	m_cBmpTransferEV.DeleteObject();
	// transfer
	m_cBmpTransfer.DeleteObject();

	// servo motor setting
	m_cBmpServoMotorSetting.DeleteObject();

	// motor setup teaching
	m_cBmpMotorSetupTeaching.DeleteObject();

	// EP regulator
	m_cBmpEPReglator.DeleteObject();

	// dio all check
	m_cBmpDIOAllCheck.DeleteObject();
	// aio all check
	m_cBmpAIOAllCheck.DeleteObject();

	// stop
	m_cBmpStop.DeleteObject();

	// retry
	m_cBmpRetry.DeleteObject();

	// skip
	m_cBmpSkip.DeleteObject();

	// communication test
	m_cBmpCommunicationTest.DeleteObject();

	// exit
	m_cBmpExit.DeleteObject();

	// vision interface
	m_cBmpVisionInterface.DeleteObject();


	if (m_pDlgHomeCheckStatus != NULL)
	{
		m_pDlgHomeCheckStatus->DestroyWindow();
		delete m_pDlgHomeCheckStatus;
		m_pDlgHomeCheckStatus = NULL;
	}

	if (m_pDlgContactAndSafety != NULL)
	{
		m_pDlgContactAndSafety->DestroyWindow();
		delete m_pDlgContactAndSafety;
		m_pDlgContactAndSafety = NULL;
	}

	if (m_pDlgManuTesterIF != NULL)
	{
		m_pDlgManuTesterIF->DestroyWindow();
		delete m_pDlgManuTesterIF;
		m_pDlgManuTesterIF = NULL;
	}
	
	
// 
// 	if (m_pDlgManuMotorTeaching != NULL)
// 	{
// 		m_pDlgManuMotorTeaching->DestroyWindow();
// 		delete m_pDlgManuMotorTeaching;
// 		m_pDlgManuMotorTeaching = NULL;
// 	}
	
}

void CDlgManu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MANU_OPER_EVENT, m_editManuOperEvent);

	// press
	DDX_Control(pDX, IDC_BUTTON_MANU_PRESS_UNIT_1, m_btnPress);
	// setplate & stacker
	DDX_Control(pDX, IDC_BUTTON_MANU_SETPLATE, m_btnSetplateStacker);
	// load test pp
	int ids_BUTTON_MANU_LOAD_TBL_AND_TEST_PP[]={IDC_BUTTON_MANU_LOAD_TBL_AND_TEST_PP_1, IDC_BUTTON_MANU_LOAD_TBL_AND_TEST_PP_2};
	for(int i=0; i<2; i++) {
		DDX_Control(pDX, ids_BUTTON_MANU_LOAD_TBL_AND_TEST_PP[i], m_btnLoadTestPP[i]);
	}
	DDX_Control(pDX, IDC_BTN_AIXS_MANU_PRESS_UNIT_1      , m_btnPress1  );
	DDX_Control(pDX, IDC_BTN_AIXS_MANU_PRESS_UNIT_2      , m_btnPress2  );
	DDX_Control(pDX, IDC_BTN_AIXS_MANU_PRESS_UNIT_3      , m_btnPress3  );
	DDX_Control(pDX, IDC_BTN_AIXS_MANU_PRESS_UNIT_4      , m_btnPress4  );
	DDX_Control(pDX, IDC_BTN_AIXS_MANU_PRESS_UNIT_5      , m_btnPress5  );
	DDX_Control(pDX, IDC_BTN_AIXS_MANU_PRESS_UNIT_6      , m_btnPress6  );
	DDX_Control(pDX, IDC_BTN_AIXS_MANU_PRESS_UNIT_7      , m_btnPress7  );
	DDX_Control(pDX, IDC_BTN_AIXS_MANU_PRESS_UNIT_8      , m_btnPress8  );
	DDX_Control(pDX, IDC_BTN_AIXS_MANU_PRESS_UNIT_9      , m_btnPress9  );
	DDX_Control(pDX, IDC_BTN_AIXS_MANU_PRESS_UNIT_10     , m_btnPress10  );
	DDX_Control(pDX, IDC_BTN_AIXS_MANU_PRESS_UNIT_11     , m_btnPress11  );
	DDX_Control(pDX, IDC_BTN_AIXS_MANU_PRESS_UNIT_12     , m_btnPress12  );
	DDX_Control(pDX, IDC_BTN_AIXS_MANU_PRESS_UNIT_13     , m_btnPress13  );
	DDX_Control(pDX, IDC_BTN_AIXS_MANU_PRESS_UNIT_14     , m_btnPress14  );
	DDX_Control(pDX, IDC_BTN_AIXS_MANU_PRESS_UNIT_15     , m_btnPress15  );
	DDX_Control(pDX, IDC_BTN_AIXS_MANU_PRESS_UNIT_16     , m_btnPress16  );
	DDX_Control(pDX, IDC_BUTTON_MANU_LOAD_TBL_1_2, m_btnTbl1_2);
	DDX_Control(pDX, IDC_BUTTON_MANU_LOAD_TBL_1_3, m_btnTbl3_4);

	// tray pp
	DDX_Control(pDX, IDC_BUTTON_MANU_TRAY_PP_1, m_btnTrayPP1);
	DDX_Control(pDX, IDC_BUTTON_MANU_TRAY_PP_2, m_btnTrayPP2);

	// feeder
	DDX_Control(pDX, IDC_BUTTON_MANU_FEEDER, m_btnFeeder);

	// test pp
	int ids_BTN_AIXS_MANU_TEST_PP[] = {IDC_BTN_AIXS_MANU_TEST_PP_1, IDC_BTN_AIXS_MANU_TEST_PP_2, IDC_BTN_AIXS_MANU_TEST_PP_3, IDC_BTN_AIXS_MANU_TEST_PP_4};
	for(int i=0; i<4; i++) {
		DDX_Control(pDX, ids_BTN_AIXS_MANU_TEST_PP[i], m_btnTestPP[i]);
	}
	// load table
	int ids_BTN_AIXS_MANU_LOAD_TBL[] = { IDC_BTN_AIXS_MANU_LOAD_TBL1, IDC_BTN_AIXS_MANU_LOAD_TBL2, IDC_BTN_AIXS_MANU_LOAD_TBL3, IDC_BTN_AIXS_MANU_LOAD_TBL4 };
	for(int i=0; i<4; i++) {
		DDX_Control(pDX, ids_BTN_AIXS_MANU_LOAD_TBL[i], m_btnLoadTable[i]);
	}
	// 
	// tray pp axis
	DDX_Control(pDX, IDC_BTN_AIXS_MANU_TRAY_PP1, m_btnTrayPP1Axis);
	DDX_Control(pDX, IDC_BTN_AIXS_MANU_TRAY_PP2, m_btnTrayPP2Axis);
	// feeder
	DDX_Control(pDX, IDC_BTN_AXIS_MANUAL_FEEDER, m_btnFeederAxis);
	// stacker ev left/right
	DDX_Control(pDX, IDC_BTN_AIXS_MANU_EV_TRANSFER_L, m_btnStackerEVLeft);
	DDX_Control(pDX, IDC_BTN_AIXS_MANU_EV_TRANSFER_R, m_btnStackerEVRight);
	// transfer ev
	DDX_Control(pDX, IDC_BUTTON_MANU_TRANSFER, m_btnTransferEV);
	// transfer
	DDX_Control(pDX, IDC_BTN_AIXS_MANU_TRANSFER, m_btnTransfer);
	// stacker
	int nStackerID[7] = { IDC_BTN_AIXS_MANU_STACKER_1, IDC_BTN_AIXS_MANU_STACKER_2,IDC_BTN_AIXS_MANU_STACKER_3,IDC_BTN_AIXS_MANU_STACKER_4,
	IDC_BTN_AIXS_MANU_STACKER_5,IDC_BTN_AIXS_MANU_STACKER_6,IDC_BTN_AIXS_MANU_STACKER_7};

	for( int nCnt = 0; nCnt < eMaxStackerCount; nCnt++ ) {
		DDX_Control(pDX, nStackerID[nCnt], m_btnStacker[nCnt] );
	}
	// servo motor setting
	DDX_Control(pDX, IDC_BUTTON_MANU_SERVO_MOTOR_SETTING, m_btnServoMotorSetting);
	// motor setup teaching
	DDX_Control(pDX, IDC_BUTTON_MANU_MOTOR_SETUP_TEACHING, m_btnMotorSetupTeaching);
	// EP regulator
	DDX_Control(pDX, IDC_BUTTON_MANU_EP_REGULATOR, m_btnEPReglator);

	DDX_Control(pDX, IDC_BUTTON_MANU_CYLINDER_CHECK, m_btnCylinderTime);
	// dio all check
	DDX_Control(pDX, IDC_BUTTON_MANU_DIO_ALL_CHECK, m_btnDIOAllCheck);
	// aio all check
	DDX_Control(pDX, IDC_BUTTON_MANU_AIO_ALL_CHECK, m_btnAIOAllCheck);
	// stop
	DDX_Control(pDX, IDC_BUTTON_MANU_PROC_CMD_STOP, m_btnStop);
	// retry
	DDX_Control(pDX, IDC_BUTTON_MANU_PROC_CMD_RETRY, m_btnRetry);
	// skip
	DDX_Control(pDX, IDC_BUTTON_MANU_PROC_CMD_SKIP, m_btnSkip);
	// communication test
	DDX_Control(pDX, IDC_BUTTON_MANU_TEST_INTERFACE, m_btnCommunicationTest);
	// exit
	DDX_Control(pDX, IDOK, m_btnExit);
	// vision interface
	DDX_Control(pDX, IDC_BUTTON_MANU_VISION_IF, m_btnVisionInterface);

}


BEGIN_MESSAGE_MAP(CDlgManu, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_MANU_PRESS_UNIT_1, &CDlgManu::OnBnClickedButtonManuPressUnit1)
	ON_BN_CLICKED(IDC_BUTTON_MANU_LOAD_TBL_AND_TEST_PP_1, &CDlgManu::OnBnClickedButtonManuLoadTBLAndTestPp1)
	ON_BN_CLICKED(IDC_BUTTON_MANU_LOAD_TBL_AND_TEST_PP_2, &CDlgManu::OnBnClickedButtonManuLoadTBLAndTestPp2)
	ON_BN_CLICKED(IDC_BUTTON_MANU_TRAY_PP_1, &CDlgManu::OnBnClickedButtonManuTrayPp1)
	ON_BN_CLICKED(IDC_BUTTON_MANU_SETPLATE, &CDlgManu::OnBnClickedButtonManuSetplate)
	ON_BN_CLICKED(IDC_BUTTON_MANU_EX_STACKER, &CDlgManu::OnBnClickedButtonManuExStacker)
	ON_BN_CLICKED(IDC_BUTTON_MANU_SERVO_MOTOR_SETTING, &CDlgManu::OnBnClickedButtonManuServoMotorSetting)
	ON_BN_CLICKED(IDC_BUTTON_MANU_MOTOR_SETUP_TEACHING, &CDlgManu::OnBnClickedButtonManuMotorSetupTeaching)
	ON_BN_CLICKED(IDC_BUTTON_MANU_TEST_INTERFACE, &CDlgManu::OnBnClickedButtonManuTestInterface)
	ON_BN_CLICKED(IDC_BUTTON_MANU_DIO_ALL_CHECK, &CDlgManu::OnBnClickedButtonManuIoAllCheck)
	ON_BN_CLICKED(IDC_BUTTON_MANU_EP_REGULATOR, &CDlgManu::OnBnClickedButtonManuEpRegulator)
	ON_BN_CLICKED(IDOK, &CDlgManu::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_MANU_TRANSFER, &CDlgManu::OnBnClickedButtonManuTransfer)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_PRESS_UNIT_1, &CDlgManu::OnBnClickedBtnAixsManuPressUnit1)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_PRESS_UNIT_2, &CDlgManu::OnBnClickedBtnAixsManuPressUnit2)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_PRESS_UNIT_3, &CDlgManu::OnBnClickedBtnAixsManuPressUnit3)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_PRESS_UNIT_4, &CDlgManu::OnBnClickedBtnAixsManuPressUnit4)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_PRESS_UNIT_5, &CDlgManu::OnBnClickedBtnAixsManuPressUnit5)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_PRESS_UNIT_6, &CDlgManu::OnBnClickedBtnAixsManuPressUnit6)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_PRESS_UNIT_7, &CDlgManu::OnBnClickedBtnAixsManuPressUnit7)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_PRESS_UNIT_8, &CDlgManu::OnBnClickedBtnAixsManuPressUnit8)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_PRESS_UNIT_9, &CDlgManu::OnBnClickedBtnAixsManuPressUnit9)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_PRESS_UNIT_10, &CDlgManu::OnBnClickedBtnAixsManuPressUnit10)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_PRESS_UNIT_11, &CDlgManu::OnBnClickedBtnAixsManuPressUnit11)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_PRESS_UNIT_12, &CDlgManu::OnBnClickedBtnAixsManuPressUnit12)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_PRESS_UNIT_13, &CDlgManu::OnBnClickedBtnAixsManuPressUnit13)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_PRESS_UNIT_14, &CDlgManu::OnBnClickedBtnAixsManuPressUnit14)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_PRESS_UNIT_15, &CDlgManu::OnBnClickedBtnAixsManuPressUnit15)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_PRESS_UNIT_16, &CDlgManu::OnBnClickedBtnAixsManuPressUnit16)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_TEST_PP_1, &CDlgManu::OnBnClickedBtnAixsManuTestPp1)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_TEST_PP_2, &CDlgManu::OnBnClickedBtnAixsManuTestPp2)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_TEST_PP_3, &CDlgManu::OnBnClickedBtnAixsManuTestPp3)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_TEST_PP_4, &CDlgManu::OnBnClickedBtnAixsManuTestPp4)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_LOAD_TBL1, &CDlgManu::OnBnClickedBtnAixsManuLoadTbl1)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_LOAD_TBL2, &CDlgManu::OnBnClickedBtnAixsManuLoadTbl2)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_LOAD_TBL3, &CDlgManu::OnBnClickedBtnAixsManuLoadTbl3)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_LOAD_TBL4, &CDlgManu::OnBnClickedBtnAixsManuLoadTbl4)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_TRANSFER, &CDlgManu::OnBnClickedBtnAixsManuTransfer)
	ON_BN_CLICKED(IDC_BUTTON_MANU_AIO_ALL_CHECK, &CDlgManu::OnBnClickedButtonManuAioAllCheck)
	ON_BN_CLICKED(IDC_BUTTON_MANU_PROC_CMD_RETRY, &CDlgManu::OnBnClickedButtonManuProcCmdRetry)
	ON_BN_CLICKED(IDC_BUTTON_MANU_PROC_CMD_SKIP, &CDlgManu::OnBnClickedButtonManuProcCmdSkip)
	ON_BN_CLICKED(IDC_BUTTON_MANU_PROC_CMD_STOP, &CDlgManu::OnBnClickedButtonManuProcCmdStop)
	ON_BN_CLICKED(IDC_BUTTON_MANU_VISION_IF, &CDlgManu::OnBnClickedButtonManuVisionIf)
	ON_BN_CLICKED(IDC_BUTTON_MANU_LOAD_TBL_1_2, &CDlgManu::OnBnClickedButtonManuLoadTbl12)
	ON_BN_CLICKED(IDC_BUTTON_MANU_LOAD_TBL_1_3, &CDlgManu::OnBnClickedButtonManuLoadTbl13)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_PRESS_UP, &CDlgManu::OnBnClickedBtnAixsManuPressUp)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_PRESS_DN, &CDlgManu::OnBnClickedBtnAixsManuPressDn)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_LOAD_TBL, &CDlgManu::OnBnClickedBtnAixsManuLoadTbl)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_MOD1, &CDlgManu::OnBnClickedBtnAixsManuMod1)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_MOD2, &CDlgManu::OnBnClickedBtnAixsManuMod2)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_MOD3, &CDlgManu::OnBnClickedBtnAixsManuMod3)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_MOD4, &CDlgManu::OnBnClickedBtnAixsManuMod4)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_MOD5, &CDlgManu::OnBnClickedBtnAixsManuMod5)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_MOD6, &CDlgManu::OnBnClickedBtnAixsManuMod6)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_ALL, &CDlgManu::OnBnClickedBtnAixsManuAll)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_TESTPP, &CDlgManu::OnBnClickedBtnAixsManuTestpp)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_STACKER_1, &CDlgManu::OnBnClickedBtnAixsManuStacker1)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_STACKER_2, &CDlgManu::OnBnClickedBtnAixsManuStacker2)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_STACKER_3, &CDlgManu::OnBnClickedBtnAixsManuStacker3)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_STACKER_4, &CDlgManu::OnBnClickedBtnAixsManuStacker4)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_STACKER_5, &CDlgManu::OnBnClickedBtnAixsManuStacker5)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_STACKER_6, &CDlgManu::OnBnClickedBtnAixsManuStacker6)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_STACKER_7, &CDlgManu::OnBnClickedBtnAixsManuStacker7)
	ON_BN_CLICKED(IDC_BUTTON_MANU_CYLINDER_CHECK, &CDlgManu::OnBnClickedButtonManuCylinderCheck)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_TRAY_PP1, &CDlgManu::OnBnClickedBtnAixsManuTrayPp1)
	ON_BN_CLICKED(IDC_BTN_AIXS_MANU_TRAY_PP2, &CDlgManu::OnBnClickedBtnAixsManuTrayPp2)
	ON_BN_CLICKED(IDC_BUTTON_MANU_TRAY_PP_2, &CDlgManu::OnBnClickedButtonManuTrayPp2)
	ON_BN_CLICKED(IDC_BTN_AXIS_MANUAL_FEEDER, &CDlgManu::OnBnClickedBtnAxisManualFeeder)
	ON_BN_CLICKED(IDC_BUTTON_MANU_FEEDER, &CDlgManu::OnBnClickedButtonManuFeeder)
	ON_BN_CLICKED(IDC_BUTTON_TRIGGER, &CDlgManu::OnBnClickedButtonTrigger)
END_MESSAGE_MAP()


// CDlgManu 메시지 처리기입니다.


BOOL CDlgManu::PreTranslateMessage(MSG* pMsg)
{
	return CDialogEx::PreTranslateMessage(pMsg);
}

//==============================================================================
// 도착한 명령을 큐에서 꺼내어 실행 시킨다.
//==============================================================================
void CDlgManu::ThreadProc1()
{
	SetThreadInterval(0, 50);
	while( GetThreadAliveFlag(0) ){
		if( GetPauseFlag(0) ) { 
			NEXT;
			continue;
		}
		//for(int i=0; i<MAX_THREAD_SCAN_COUNT; i++){ 
			CBaseTaskManager::SeqCmdRunning(); 
		//}
		NEXT_10;
	}
}

BOOL CDlgManu::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CBaseTaskManager::Initialize();	

	g_DMCont.m_dmEQP.SB(BDM0_OPEN_SLT_UI_DLG_HISTORY, TRUE);

	m_pDlgHomeCheckStatus = new CDlgHomeCheckStatus(this);
	m_pDlgHomeCheckStatus->Create(IDD_DIALG_HOME_CHECK_STATUS, this);
	m_pDlgHomeCheckStatus->ShowWindow(SW_HIDE);

	
// 	m_pDlgManuMotorTeaching = new CDlgManuMotorTeaching(this);
// 	m_pDlgManuMotorTeaching->Create(IDD_MANU_MOTOR_TEACHING, this);
// 	m_pDlgManuMotorTeaching->ShowWindow(SW_HIDE);

	SetTimer(0, DEF_SENSOR_DISP_TM, NULL);

	// press unit
	m_cBmpPress.LoadBitmap(IDB_PRESS_UNIT);
	m_btnPress.SetBitmap(m_cBmpPress);
	// 
	// setplate & stacker
	m_cBmpSetplateStacker.LoadBitmap(IDB_SETPLATE_STACKER);
	m_btnSetplateStacker.SetBitmap(m_cBmpSetplateStacker);
	// 
	// loadtable & test pp
	for(int i=0; i<2; i++) {
		m_cBmpLoadTestPP[i].LoadBitmap(IDB_TESTPP_01 +i);
		m_btnLoadTestPP[i].SetBitmap(m_cBmpLoadTestPP[i]);
	}

	// tray pp
	m_cBmpTrayPP1.LoadBitmap(IDB_BITMAP_TRAY_PP1);
	m_btnTrayPP1.SetBitmap(m_cBmpTrayPP1);

	m_cBmpTrayPP2.LoadBitmap(IDB_BITMAP_TRAY_PP2);
	m_btnTrayPP2.SetBitmap(m_cBmpTrayPP2);

	m_cBmpFeeder.LoadBitmap(IDB_BITMAP_FEEDER);
	m_btnFeeder.SetBitmap(m_cBmpFeeder);

	// test pp
	m_cBmpTestPP[0].LoadBitmap(IDB_TESTPP_01);
	m_btnTestPP[0].SetBitmap(m_cBmpTestPP[0]);
	for(int i=0; i<1; i++) {
		m_cBmpTestPP[i+1].LoadBitmap(IDB_TESTPP_02+i);
		m_btnTestPP[i+1].SetBitmap(m_cBmpTestPP[i+1]);
	}
	// load table
	for(int i=0; i<4; i++) {
		m_cBmpLoadTable[i].LoadBitmap(IDB_LOADTABLE_01+i);
		m_btnLoadTable[i].SetBitmap(m_cBmpLoadTable[i]);
	}
	// tray pp axis
	m_cBmpTrayPP1Axis.LoadBitmap(IDB_BITMAP_TRAY_PP1);
	m_btnTrayPP1Axis.SetBitmap(m_cBmpTrayPP1Axis);

	m_cBmpTrayPP2Axis.LoadBitmap(IDB_BITMAP_TRAY_PP2);
	m_btnTrayPP2Axis.SetBitmap(m_cBmpTrayPP2Axis);

	m_cBmpFeederAxis.LoadBitmap(IDB_BITMAP_FEEDER);
	m_btnFeederAxis.SetBitmap(m_cBmpFeederAxis);


	// 
	// stacker ev left/right
	m_cBmpStackerEVLeft.LoadBitmap(IDB_STACKER_EV_L);
	m_btnStackerEVLeft.SetBitmap(m_cBmpStackerEVLeft);
	m_cBmpStackerEVRight.LoadBitmap(IDB_STACKER_EV_R);
	m_btnStackerEVRight.SetBitmap(m_cBmpStackerEVRight);

	// transfer ev
	m_cBmpTransferEV.LoadBitmap(IDB_TRANSFER);
	m_btnTransferEV.SetBitmap(m_cBmpTransferEV);
	// transfer
	m_cBmpTransfer.LoadBitmap(IDB_TRANSFER);
	m_btnTransfer.SetBitmap(m_cBmpTransfer);

	// servo motor setting
	m_cBmpServoMotorSetting.LoadBitmap(IDB_SERVOMOTOR_SETTING);
	m_btnServoMotorSetting.SetBitmap(m_cBmpServoMotorSetting);

	// motor setup teaching
	m_cBmpMotorSetupTeaching.LoadBitmap(IDB_MOTOR_SETUP_TEACHING);
	m_btnMotorSetupTeaching.SetBitmap(m_cBmpMotorSetupTeaching);

	// EP regulator
	m_cBmpEPReglator.LoadBitmap(IDB_EP_REGULATOR);
	m_btnEPReglator.SetBitmap(m_cBmpEPReglator);

	// dio all check
	m_cBmpDIOAllCheck.LoadBitmap(IDB_DIO_ALL_CHECK);
	m_btnDIOAllCheck.SetBitmap(m_cBmpDIOAllCheck);
	// aio all check
	m_cBmpAIOAllCheck.LoadBitmap(IDB_AIO_ALL_CHECK);
	m_btnAIOAllCheck.SetBitmap(m_cBmpAIOAllCheck);

	// stop
	m_cBmpStop.LoadBitmap(IDB_STOP);
	m_btnStop.SetBitmap(m_cBmpStop);

	// retry
	m_cBmpRetry.LoadBitmap(IDB_RETRY);
	m_btnRetry.SetBitmap(m_cBmpRetry);

	// skip
	m_cBmpSkip.LoadBitmap(IDB_SKIP);
	m_btnSkip.SetBitmap(m_cBmpSkip);

	// communication test
	m_cBmpCommunicationTest.LoadBitmap(IDB_COMMNICATION_TEST);
	m_btnCommunicationTest.SetBitmap(m_cBmpCommunicationTest);

	// exit
	m_cBmpExit.LoadBitmap(IDB_EXIT);
	m_btnExit.SetBitmap(m_cBmpExit);

	// vision interface
	m_cBmpVisionInterface.LoadBitmap(IDB_VISION_INTERFACE);
	m_btnVisionInterface.SetBitmap(m_cBmpVisionInterface);

	m_cBmpCylinderTime.LoadBitmap(IDB_BITMAP_CYLINDER);
	m_btnCylinderTime.SetBitmap(m_cBmpCylinderTime);

	OnUpdatePressUI();
	
	// 20161008_mjna_imsi >>
	CRect rect;
	GetClientRect(&rect);
	int cx = rect.Width();
	int cy = rect.Height();
	SetWindowPos(&wndNoTopMost, 5, 115, cx, cy + 10, SWP_SHOWWINDOW);
	// 20161008_mjna_imsi <<

	g_DMCont.m_dmHandlerOpt.SB(BDM11_GUI_OPEN_HISTORY, TRUE);

	m_pDlgContactAndSafety = new CDlgContactAndSafety;
	m_pDlgContactAndSafety->Create(IDD_DIALOG_CONTACT_AND_SAFETY_MOVING,this);
	m_pDlgContactAndSafety->ShowWindow(SW_HIDE);

	int nLoginLv = g_DMCont.m_dmEQP.GN(NDM0_Handler_Login_Level);
	if (nLoginLv < eUSERLEVEL_SUPERVISOR)
	{
		GetDlgItem(IDC_BUTTON_MANU_SETPLATE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_MANU_FEEDER)->ShowWindow(SW_HIDE);
	}

	MakeLog("Manual Dialog Show ");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgManu::OnUpdatePressUI()
{
	m_btnPress1.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnPress1.SetIcon(IDI_ICON_PRESS);
	m_btnPress1.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnPress2.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnPress2.SetIcon(IDI_ICON_PRESS);
	m_btnPress2.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnPress3.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnPress3.SetIcon(IDI_ICON_PRESS);
	m_btnPress3.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnPress4.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnPress4.SetIcon(IDI_ICON_PRESS);
	m_btnPress4.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnPress5.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnPress5.SetIcon(IDI_ICON_PRESS);
	m_btnPress5.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnPress6.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnPress6.SetIcon(IDI_ICON_PRESS);
	m_btnPress6.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnPress7.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnPress7.SetIcon(IDI_ICON_PRESS);
	m_btnPress7.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnPress8.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnPress8.SetIcon(IDI_ICON_PRESS);
	m_btnPress8.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnPress9.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnPress9.SetIcon(IDI_ICON_PRESS);
	m_btnPress9.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnPress10.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnPress10.SetIcon(IDI_ICON_PRESS);
	m_btnPress10.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnPress11.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnPress11.SetIcon(IDI_ICON_PRESS);
	m_btnPress11.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnPress12.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnPress12.SetIcon(IDI_ICON_PRESS);
	m_btnPress12.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnPress13.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnPress13.SetIcon(IDI_ICON_PRESS);
	m_btnPress13.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnPress14.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnPress14.SetIcon(IDI_ICON_PRESS);
	m_btnPress14.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnPress15.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnPress15.SetIcon(IDI_ICON_PRESS);
	m_btnPress15.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnPress16.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnPress16.SetIcon(IDI_ICON_PRESS);
	m_btnPress16.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnTbl1_2.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnTbl1_2.SetIcon(IDI_ICON_TBL);
	m_btnTbl1_2.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnTbl3_4.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_btnTbl3_4.SetIcon(IDI_ICON_TBL);
	m_btnTbl3_4.SetAlign(CButtonST::ST_ALIGN_VERT);

	for( int nCnt = 0; nCnt < eMaxStackerCount; nCnt++ ){
		m_btnStacker[nCnt].SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
		m_btnStacker[nCnt].SetIcon(IDI_ICON_STACKER);
		m_btnStacker[nCnt].SetAlign(CButtonST::ST_ALIGN_VERT);
	}
}

void CDlgManu::OnDestroy()
{
	g_DMCont.m_dmHandlerOpt.SB(BDM11_GUI_OPEN_HISTORY, FALSE);
	g_DMCont.m_dmEQP.SB(BDM0_OPEN_SLT_UI_DLG_HISTORY, FALSE);
	CDialogEx::OnDestroy();

	CBaseTaskManager::Finalize();
}


void CDlgManu::OnTimer(UINT_PTR nIDEvent)
{
	BOOL bBusySts  = FALSE;
	BOOL bAlarmSts = FALSE;

	int nCntBusy = 0;
	int nCntAlarm = 0;


	if(g_TaskTransfer.GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready){
		bBusySts = TRUE;
	}else if(g_TaskTransfer.GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm){
		bAlarmSts = TRUE;
	}

	if (g_TaskTrayFeeder.GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
		bBusySts = TRUE;
	}
	else if (g_TaskTrayFeeder.GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
		bAlarmSts = TRUE;
	}

	CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
	for (int nIdx = 0; nIdx < eMaxTrayPPCount; nIdx++) {
		if (pTaskTrayPP[nIdx]->GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			bBusySts = TRUE;
		}
		else if (pTaskTrayPP[nIdx]->GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
			bAlarmSts = TRUE;
		}
	}

	for(int nIdx=0; nIdx<eMaxStackerCount; nIdx++){
		if(g_TaskStacker[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready){
			nCntBusy++;
		}else if(g_TaskStacker[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm){
			nCntAlarm++;
		}
	}

	for(int nIdx=0; nIdx<eMaxPressUnitCount; nIdx++){
		if(g_TaskPressUnit[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready){
			nCntBusy++;
		}else if(g_TaskPressUnit[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm){
			nCntAlarm++;
		}
	}

	for(int nIdx=0; nIdx<eMaxTestPPCount; nIdx++){
		if(g_TaskTestPP[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready){
			nCntBusy++;
		}else if(g_TaskTestPP[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm){
			nCntAlarm++;
		}
	}

	for(int nIdx=0; nIdx<eMaxLoadTblCount; nIdx++){
		if(g_TaskLoadTbl[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready){
			nCntBusy++;
		}else if(g_TaskLoadTbl[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm){
			nCntAlarm++;
		}
	}

	if(g_MulitOrg.GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready){
		nCntBusy++;
	}else if(g_MulitOrg.GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm){
		nCntAlarm++;
	}

	if(nCntBusy > 0){
		bBusySts = TRUE;
	}
	if(nCntAlarm > 0){
		bAlarmSts = TRUE;
	}

	CString strCurPos;

	CDSSPanel*    pSensor = NULL;
	if(nIDEvent == 0)
	{
		KillTimer(0);
		
		pSensor = (CDSSPanel*)GetDlgItem(IDC_AXIS_CMD_BUSY);
		if(bBusySts){
			pSensor->On();
			m_bManualCmdExcute = TRUE;
		}else{
			pSensor->Off();
			m_bManualCmdExcute = FALSE;
		}
		

		pSensor = (CDSSPanel*)GetDlgItem(IDC_AXIS_CMD_ERROR);
		if(bAlarmSts){
			pSensor->On();
		}else{
			pSensor->Off();
		}

		if (m_pDlgContactAndSafety != NULL)
		{
			if (g_DMCont.m_dmHandlerOpt.GB(BDM11_STATUS_OPEN_HISTORY) == TRUE)
			{
				if (m_pDlgContactAndSafety->IsWindowVisible() == FALSE) {
					m_pDlgContactAndSafety->ShowWindow(SW_SHOW);
				}
			}
			else
			{
				if (m_pDlgContactAndSafety->IsWindowVisible() == TRUE) {
					m_pDlgContactAndSafety->ShowWindow(SW_HIDE);
				}
			}
		}
		SetTimer(0, DEF_SENSOR_DISP_TM, NULL);
	}
	
	CDialogEx::OnTimer(nIDEvent);
}


void CDlgManu::OnBnClickedButtonManuPressUnit1()
{
	MakeLog("Press Unit #1 ~ #8 Dialog Show");
	CDlgManuPressUnit dlg;
	dlg.DoModal();
}


void CDlgManu::OnBnClickedButtonManuLoadTBLAndTestPp1()
{
	g_DMCont.m_dmEQP.SB(BDM0_OPEN_EJECTOR_DLG_HISTORY, TRUE);//jira 403
	CDlgManuTestPP dlg;
	dlg.m_nHandIdx =eTestPP_1;
	dlg.DoModal();
	g_DMCont.m_dmEQP.SB(BDM0_OPEN_EJECTOR_DLG_HISTORY, FALSE);//jira 403
}


void CDlgManu::OnBnClickedButtonManuLoadTBLAndTestPp2()
{
	g_DMCont.m_dmEQP.SB(BDM0_OPEN_EJECTOR_DLG_HISTORY, TRUE);//jira 403
	CDlgManuTestPP dlg;
	dlg.m_nHandIdx =eTestPP_2;
	dlg.DoModal();
	g_DMCont.m_dmEQP.SB(BDM0_OPEN_EJECTOR_DLG_HISTORY, FALSE);//jira 403
}


void CDlgManu::OnBnClickedButtonManuTrayPp1()
{
	g_DMCont.m_dmEQP.SB(BDM0_OPEN_EJECTOR_DLG_HISTORY, TRUE);//jira 403
	CDlgManuTrayPP dlg;
	dlg.m_nHandIdx = eTrayPP_1_LD;
	dlg.DoModal();
	g_DMCont.m_dmEQP.SB(BDM0_OPEN_EJECTOR_DLG_HISTORY, FALSE);//jira 403
}


void CDlgManu::OnBnClickedButtonManuSetplate()
{
	CDlgManuSetplate dlg;
	dlg.DoModal();
}

void CDlgManu::OnBnClickedButtonManuExStacker()
{
	AfxMessageBox("Not Ready");
}

void CDlgManu::OnBnClickedButtonManuServoMotorSetting()
{
	if(m_bManualCmdExcute){
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	CDlgManuMotor dlg;
	dlg.DoModal();
	MakeLog("Manual ManuServoMotorSetting Show ");
}


void CDlgManu::OnBnClickedButtonManuMotorSetupTeaching()
{
	if (m_bManualCmdExcute) {
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	 CDlgManuMotorTeaching dlg;
	 dlg.DoModal();
// 	if (m_pDlgManuMotorTeaching->IsWindowVisible() == FALSE) {
// 		m_pDlgManuMotorTeaching->ShowWindow(SW_SHOW);
// 		g_DMCont.m_dmHandlerOpt.SB(BDM11_GUI_OPEN_HISTORY, TRUE);
// 		MakeLog("Manual ManuMotorSetupTeaching Show ");
// 	}
	
			
}

void CDlgManu::OnBnClickedButtonManuTestInterface()
{
	int nTesterOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Tester_Option);
	if (nTesterOption != eTester_Agent)
	{
// 		CDlgManuTesterIF dlg;
// 		dlg.DoModal();

		m_pDlgManuTesterIF = new CDlgManuTesterIF(this);
		m_pDlgManuTesterIF->Create(IDD_MANU_TESTER_IF, this);
		m_pDlgManuTesterIF->ShowWindow(SW_SHOW);
	}
	else
	{
		//agent tester
// 		CDlgManuAgentIF dlg;
// 		dlg.DoModal();
	}
}


void CDlgManu::OnBnClickedButtonManuIoAllCheck()
{
	g_IO.ShowDialog_IO_All();
	MakeLog("DIO ALL Check Button Click");
}


void CDlgManu::OnBnClickedButtonManuEpRegulator()
{
/*  event test code
	static BOOL b=FALSE;
	b = !b;
	if( b ) SendEvent(CEID_SOUND_SW_DOWN);
	else    SendEvent(CEID_SOUND_SW_UP);
	return;
*/

	CDlgManuRegul dlg;
	dlg.DoModal();
	MakeLog("Manual ManuEpRegulator Show ");
}


void CDlgManu::OnBnClickedOk()
{
	if(m_bManualCmdExcute){
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}
	g_DMCont.m_dmHandlerOpt.SB(BDM11_GUI_OPEN_HISTORY, FALSE);
	CDialogEx::OnOK();
}


void CDlgManu::OnBnClickedButtonManuTransfer()
{
	CDlgManuTransfer dlg;
	dlg.DoModal();
}

//======================================================Axis Manual Move operation
void CDlgManu::OnBnClickedBtnAixsManuPressUnit1()
{
	OnBnClickedBtnAixsManuPress(ePressUnit_Down_1);
}


void CDlgManu::OnBnClickedBtnAixsManuPressUnit2()
{
	OnBnClickedBtnAixsManuPress(ePressUnit_Down_2);
}


void CDlgManu::OnBnClickedBtnAixsManuPressUnit3()
{
	OnBnClickedBtnAixsManuPress(ePressUnit_Down_3);
}


void CDlgManu::OnBnClickedBtnAixsManuPressUnit4()
{
	OnBnClickedBtnAixsManuPress(ePressUnit_Down_4);
}


void CDlgManu::OnBnClickedBtnAixsManuPressUnit5()
{
	OnBnClickedBtnAixsManuPress(ePressUnit_Down_5);
}


void CDlgManu::OnBnClickedBtnAixsManuPressUnit6()
{
	OnBnClickedBtnAixsManuPress(ePressUnit_Down_6);
}


void CDlgManu::OnBnClickedBtnAixsManuPressUnit7()
{
	OnBnClickedBtnAixsManuPress(ePressUnit_Down_7);
}


void CDlgManu::OnBnClickedBtnAixsManuPressUnit8()
{
	OnBnClickedBtnAixsManuPress(ePressUnit_Down_8);
}

void CDlgManu::OnBnClickedBtnAixsManuPressUnit9()
{
//	OnBnClickedBtnAixsManuPress(ePressUnit_Up_1);
}


void CDlgManu::OnBnClickedBtnAixsManuPressUnit10()
{
//	OnBnClickedBtnAixsManuPress(ePressUnit_Up_2);
}


void CDlgManu::OnBnClickedBtnAixsManuPressUnit11()
{
//	OnBnClickedBtnAixsManuPress(ePressUnit_Up_3);
}


void CDlgManu::OnBnClickedBtnAixsManuPressUnit12()
{
//	OnBnClickedBtnAixsManuPress(ePressUnit_Up_4);
}


void CDlgManu::OnBnClickedBtnAixsManuPressUnit13()
{
//	OnBnClickedBtnAixsManuPress(ePressUnit_Up_5);
}


void CDlgManu::OnBnClickedBtnAixsManuPressUnit14()
{
//	OnBnClickedBtnAixsManuPress(ePressUnit_Up_6);
}

void CDlgManu::OnBnClickedBtnAixsManuPressUnit15()
{
//	OnBnClickedBtnAixsManuPress(ePressUnit_Up_7);
}

void CDlgManu::OnBnClickedBtnAixsManuPressUnit16()
{
//	OnBnClickedBtnAixsManuPress(ePressUnit_Up_8);
}

void CDlgManu::OnBnClickedBtnAixsManuPress(int nPressIdx )
{
	if(m_bManualCmdExcute){
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	if(!g_COVER_SW.GetStatus()){
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK,AlarmData,NULL);
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable();
	if(nInterlockErr !=ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	MakeLog("PressUnit%d Dialog Show",nPressIdx+1);
	CDlgCommand_PressUnit CmdDlg;
	if( IDOK == CmdDlg.DoModal() )
	{
		if (nPressIdx == ePressUnit_Down_1 || nPressIdx == ePressUnit_Down_5)
		{
			int nTestPpIdx = nPressIdx == ePressUnit_Down_1 ? eTestPP_1 : eTestPP_2;
			int nErr = g_TaskTestPP[nTestPpIdx].Interlock_X_Related_Clean_Table_Range();
			if (nErr != ERR_NO_ERROR)
			{
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, this);
				return;
			}
		}

		//Test PP Recovery & Press Safety 충돌 영역 확인.
		for (int nTestPpIdx = 0; nTestPpIdx < eMaxTestPPCount; nTestPpIdx++)
		{
			int nErr = g_TaskTestPP[nTestPpIdx].Check_X_Related_Press_Z_Safety();
			if (nErr != ERR_NO_ERROR)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Please Test PP%s X Move Safety", (nTestPpIdx == eTestPP_1) ? _T("1") : _T("2"));
				g_Error.AlarmReport(nErr, AlarmData, this);
				return;
			}
		}


		int nSelectedCmd = CmdDlg.GetSelectedSeqCmdId();
		switch( nSelectedCmd )
		{
		case eSeqCmd_PressUnit_Initial:
			{
				CPressUnitCmd_Initial* pSeqCmd = new CPressUnitCmd_Initial;
				int nErr = g_TaskSystemCtrl.SendCommand(g_TaskPressUnit[ nPressIdx ], pSeqCmd, eEqpCtrlMode_Manual);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					g_Error.AlarmReport(nErr, AlarmData, NULL);
				}else{
					MakeLog("PressUnit%d Initial", nPressIdx + 1 );
				}
			}break;
		case eSeqCmd_PressUnit_MovePos:
			{
				int nManu_Oper_Press_Index = 0;
				switch( nPressIdx )
				{
					case ePressUnit_Down_1: nManu_Oper_Press_Index = eMANU_OPER_PRESS_DN_1; break;
					case ePressUnit_Down_2: nManu_Oper_Press_Index = eMANU_OPER_PRESS_DN_2; break;
					case ePressUnit_Down_3: nManu_Oper_Press_Index = eMANU_OPER_PRESS_DN_3; break;
					case ePressUnit_Down_4: nManu_Oper_Press_Index = eMANU_OPER_PRESS_DN_4; break;
					case ePressUnit_Down_5: nManu_Oper_Press_Index = eMANU_OPER_PRESS_DN_5; break;
					case ePressUnit_Down_6: nManu_Oper_Press_Index = eMANU_OPER_PRESS_DN_6; break;
					case ePressUnit_Down_7: nManu_Oper_Press_Index = eMANU_OPER_PRESS_DN_7; break;
					case ePressUnit_Down_8: nManu_Oper_Press_Index = eMANU_OPER_PRESS_DN_8; break;
				}
				CDlgAxisActionMove dlg;
				dlg.m_nStartPoint = nManu_Oper_Press_Index;
				dlg.DoModal();
			}break;
		case eSeqCmd_PressUnit_AutoHeightCal:
			{
				if( AfxMessageBox( "Do you want auto height cal?", MB_OKCANCEL ) != IDOK )
					break;

				CPressUnitCmd_AutoHeightCal * pSeqCmd = new CPressUnitCmd_AutoHeightCal;
				int nErr = g_TaskSystemCtrl.SendCommand(g_TaskPressUnit[ nPressIdx ], pSeqCmd, eEqpCtrlMode_Manual);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					g_Error.AlarmReport(nErr, AlarmData, NULL);
				}
			}break;
		case eSeqCmd_PressUnit_SocketCleanContact:
			{
				if( AfxMessageBox("Do you want Socket Cleaning??", MB_OKCANCEL) != IDOK )
					break;

				CPressUnitCmd_SocketCleanContact * pSeqCmd = new CPressUnitCmd_SocketCleanContact;
				int nErr = g_TaskSystemCtrl.SendCommand(g_TaskPressUnit[ nPressIdx ], pSeqCmd, eEqpCtrlMode_Manual);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					g_Error.AlarmReport(nErr, AlarmData, NULL);
				}
			}break;
		}	
	}
}


void CDlgManu::OnBnClickedBtnAixsManuTestPp(int nTestPPIdx)
{
	if(m_bManualCmdExcute){
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	if(!g_COVER_SW.GetStatus()){
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK,AlarmData,NULL);
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable();
	if(nInterlockErr !=ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	MakeLog("Test PP #%d Dialog Show",nTestPPIdx+1);
	CDlgCommand_TestPP CmdDlg;
	if( IDOK == CmdDlg.DoModal() )
	{

		int nErr = g_TaskTestPP[nTestPPIdx].Interlock_X_Related_Clean_Table_Range();
		if (nErr != ERR_NO_ERROR)
		{
			CParamList AlarmData;
			g_Error.AlarmReport(nErr, AlarmData, this);
			return;
		}
		
		//Test PP Recovery & Press Safety 충돌 영역 확인.
		for (int nTestPpIdx = 0; nTestPpIdx < eMaxTestPPCount; nTestPpIdx++)
		{
			int nErr = g_TaskTestPP[nTestPpIdx].Check_X_Related_Press_Z_Safety();
			if (nErr != ERR_NO_ERROR)
			{
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Please Test PP%s X Move Safety", (nTestPpIdx == eTestPP_1) ? _T("1") : _T("2"));
				g_Error.AlarmReport(nErr, AlarmData, this);
				return;
			}
		}



		int nSelectedCmd = CmdDlg.GetSelectedSeqCmdId();
		switch( nSelectedCmd )
		{
		case eSeqCmd_TestPP_Initial:
			{
				int nRetCmd = ERR_NO_ERROR;
				nRetCmd |= g_TaskPressUnit[ ePressUnit_Down_1 + ( eMaxATS_Area * nTestPPIdx ) ].ChkDI( CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON );  
				nRetCmd |= g_TaskPressUnit[ ePressUnit_Down_2 + ( eMaxATS_Area * nTestPPIdx ) ].ChkDI( CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON );  
				nRetCmd |= g_TaskPressUnit[ ePressUnit_Down_3 + ( eMaxATS_Area * nTestPPIdx ) ].ChkDI( CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON );  
				nRetCmd |= g_TaskPressUnit[ ePressUnit_Down_4 + ( eMaxATS_Area * nTestPPIdx ) ].ChkDI( CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON );  
				
				if( nRetCmd != ERR_NO_ERROR ) {
					AfxMessageBox( "Press Unit is not safety pos!!!" );
					return;
				}

				CTestPPCmd_Initial* pSeqCmd = new CTestPPCmd_Initial;
				int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTestPP[nTestPPIdx], pSeqCmd, eEqpCtrlMode_Manual);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					g_Error.AlarmReport(nErr, AlarmData, NULL);
				}else{
					MakeLog("Test PP #%d Initial",nTestPPIdx+1);
				}
			}break;
		case eSeqCmd_TestPP_MovePos:
			{
				CDlgAxisActionMove dlg;
				dlg.m_nStartPoint   = eMANU_OPER_TEST_PP1+nTestPPIdx;
				dlg.m_nSelectAction = eDlgActionOnlyMove;
				dlg.DoModal();
			}break;
		case eSeqCmd_TestPP_MovePickPlace:
			{
				CDlgAxisActionMove dlg;
				dlg.m_nStartPoint   = eMANU_OPER_TEST_PP1+nTestPPIdx;
				dlg.m_nSelectAction = eDlgActionPick;
				dlg.DoModal();
			}break;
		case eSeqCmd_TestPP_MoveExchange:
			{
				CDlgAxisActionMove dlg;
				dlg.m_nStartPoint = eMANU_OPER_TEST_PP1 + nTestPPIdx;
				dlg.m_nSelectAction = eDlgActionExchange;
				dlg.DoModal();
			}break;
		}
	}
}


void CDlgManu::OnBnClickedBtnAixsManuTestPp1()
{
	OnBnClickedBtnAixsManuTestPp(eTestPP_1);
}


void CDlgManu::OnBnClickedBtnAixsManuTestPp2()
{
	OnBnClickedBtnAixsManuTestPp(eTestPP_2);
}


void CDlgManu::OnBnClickedBtnAixsManuTestPp3()
{
	//OnBnClickedBtnAixsManuTestPp(eTestPP_3);
}


void CDlgManu::OnBnClickedBtnAixsManuTestPp4()
{
	//OnBnClickedBtnAixsManuTestPp(eTestPP_4);
}

void CDlgManu::OnBnClickedBtnAixsManuLoadTbl(int nTableIdx)
{
	if(m_bManualCmdExcute){
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	if(!g_COVER_SW.GetStatus()){
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK,AlarmData,NULL);
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable(TRUE);
	if(nInterlockErr !=ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	CDlgCommand_LoadTbl CmdDlg;
	MakeLog("Load Table #%d Dialog Show",nTableIdx+1);
	if( IDOK == CmdDlg.DoModal() )
	{
		int nSelectedCmd = CmdDlg.GetSelectedSeqCmdId();
		switch( nSelectedCmd )
		{
		case eSeqCmd_LoadTbl_Initial:
			{
				CLoadTblCmd_Initial* pSeqCmd = new CLoadTblCmd_Initial;
				int nErr = g_TaskSystemCtrl.SendCommand(g_TaskLoadTbl[nTableIdx], pSeqCmd, eEqpCtrlMode_Manual);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					g_Error.AlarmReport(nErr, AlarmData, NULL);
				}else{
					MakeLog("Load Table #%d Initial",nTableIdx+1);
				}
			}break;
		case eSeqCmd_LoadTbl_MovePos:
			{
				CDlgAxisActionMove dlg;
				dlg.m_nStartPoint = eMANU_OPER_LD_TBL1+nTableIdx;
				dlg.DoModal();
			}break;
		}
	}
}

void CDlgManu::OnBnClickedBtnAixsManuLoadTbl1()
{
	OnBnClickedBtnAixsManuLoadTbl(eLoadTbl_1);
}


void CDlgManu::OnBnClickedBtnAixsManuLoadTbl2()
{
	OnBnClickedBtnAixsManuLoadTbl(eLoadTbl_2);
}


void CDlgManu::OnBnClickedBtnAixsManuLoadTbl3()
{

	OnBnClickedBtnAixsManuLoadTbl(eLoadTbl_3);
}


void CDlgManu::OnBnClickedBtnAixsManuLoadTbl4()
{
	OnBnClickedBtnAixsManuLoadTbl(eLoadTbl_4);
}


void CDlgManu::OnBnClickedBtnAixsManuTrayPp(int nTrayPpInx)
{
	if(m_bManualCmdExcute){
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	if(!g_COVER_SW.GetStatus()){
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK,AlarmData,NULL);
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable(TRUE);
	if(nInterlockErr !=ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}
	CTaskTrayBasePP* pTaskTrayPP = NULL;
	if (nTrayPpInx == eTrayPP_1_LD) {
		pTaskTrayPP = &g_TaskTrayLoadPP;
	}
	else if (nTrayPpInx == eTrayPP_2_UD) {
		pTaskTrayPP = &g_TaskTrayUnloadPP;
	}
	CDlgCommand_TrayPP CmdDlg;
	MakeLog("Tray PP Dialog Show");
	if( IDOK == CmdDlg.DoModal() )
	{
		int nSelectedCmd = CmdDlg.GetSelectedSeqCmdId();
		switch( nSelectedCmd )
		{
		case eSeqCmd_TrayPP_Initial:
		{
			CBaseSeqCmd* pSeqCmd = NULL;
			if (nTrayPpInx == eTrayPP_1_LD) {
				pSeqCmd = new CTrayLoadPPCmd_Initial;
			}
			else if (nTrayPpInx == eTrayPP_2_UD) {
				pSeqCmd = new CTrayUnloadPPCmd_Initial;
			}

			int nErr = g_TaskSystemCtrl.SendCommand(*pTaskTrayPP, pSeqCmd, eEqpCtrlMode_Manual);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
			}
			else {
				MakeLog("Tray PP Initial");
			}
		}break;

		case eSeqCmd_TrayPP_MovePos:
		{
			CDlgAxisActionMove dlg;
			dlg.m_nStartPoint = eMANU_OPER_TRAY_PP1_LD + nTrayPpInx;
			dlg.m_nSelectAction = eDlgActionOnlyMove;
			dlg.DoModal();
		}break;

		case eSeqCmd_TrayPP_MovePickPlace:
		{
			CDlgAxisActionMove dlg;
			dlg.m_nStartPoint = eMANU_OPER_TRAY_PP1_LD + nTrayPpInx;
			dlg.m_nSelectAction = eDlgActionPick;
			dlg.DoModal();
		}break;

// 		case eSeqCmd_TrayPP_PickerHeightCal:
// 		{
// 			CBaseSeqCmd* pSeqCmd = NULL;
// 			if (nTrayPpInx == eTrayPP_1_LD) {
// 				pSeqCmd = new CTrayLoadPPCmd_PickerHeightCal;
// 				static_cast<CTrayLoadPPCmd_PickerHeightCal*>(pSeqCmd)->Pocket = CPoint(0,0);
// 				pTaskTrayPP->MakeAllPicker(static_cast<CTrayLoadPPCmd_PickerHeightCal*>(pSeqCmd)->vPicker);
// 			}
// 			else if (nTrayPpInx == eTrayPP_2_UD) {
// 				CTrayUnloadPPCmd_PickerHeightCal_Param prm;
// 				prm.Pocket = CPoint(0, 0);
// 				pTaskTrayPP->MakeAllPicker(prm.vPicker);
// 				pSeqCmd = new CTrayUnloadPPCmd_PickerHeightCal;
// 				static_cast<CTrayUnloadPPCmd_PickerHeightCal*>(pSeqCmd)->m_Cmd = prm;
// 			}
// 			int nErr = g_TaskSystemCtrl.SendCommand(*pTaskTrayPP, pSeqCmd, eEqpCtrlMode_Manual);
// 			if (nErr != ERR_NO_ERROR) {
// 				CParamList AlarmData;
// 				g_Error.AlarmReport(nErr, AlarmData, NULL);
// 			}
// 			else {
// 				MakeLog("Tray PP Picker Height Cal");
// 			}
// 		}break;

		}
	}
}

void CDlgManu::OnBnClickedBtnAixsManuTransfer()
{
	if(m_bManualCmdExcute){
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	if(!g_COVER_SW.GetStatus()){
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK,AlarmData,NULL);
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable(TRUE);
	if(nInterlockErr !=ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	MakeLog("Transfer Dialog Show");
	CDlgCommand_Transfer CmdDlg;
	if( IDOK == CmdDlg.DoModal() )
	{
		int nSelectedCmd = CmdDlg.GetSelectedSeqCmdId();
		switch( nSelectedCmd )
		{
		case eSeqCmd_Transfer_Initial:
			{
				CTransferCmd_Initial* pSeqCmd = new CTransferCmd_Initial;
				int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTransfer, pSeqCmd, eEqpCtrlMode_Manual);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					g_Error.AlarmReport(nErr, AlarmData, NULL);
				}else{
					MakeLog("Transfer Initial");
				}
			}break;

		case eSeqCmd_Transfer_MovePos:
			{
				CDlgAxisActionMove dlg;
				dlg.m_nStartPoint = eMANU_OPER_TRANSFER;
				dlg.m_nSelectAction = eDlgActionOnlyMove;
				dlg.DoModal();
			}break;
		case eSeqCmd_Transfer_MovePickPlace:
			{
				CDlgAxisActionMove dlg;
				dlg.m_nStartPoint = eMANU_OPER_TRANSFER;
				dlg.m_nSelectAction = eDlgActionPick;
				dlg.DoModal();
			}break;
		}	
	}
}


void CDlgManu::OnBnClickedButtonManuAioAllCheck()
{
	g_AIO.ShowDialog_IO_All();
}

void CDlgManu::OnBnClickedButtonManuVisionIf()
{
	CDlgManuVision dlg(this);
	dlg.DoModal();
}

void CDlgManu::OnBnClickedButtonManuProcCmdRetry()
{
	MakeLog("Retry Button Click");
	CBaseTaskManager::SendProcessCommand(eProcCmd_Retry, eEqpCtrlMode_Manual, NULL);
}

void CDlgManu::OnBnClickedButtonManuProcCmdSkip()
{
	MakeLog("Skip Button Click");
	CBaseTaskManager::SendProcessCommand(eProcCmd_Skip, eEqpCtrlMode_Manual, NULL);
}


void CDlgManu::OnBnClickedButtonManuProcCmdStop()
{
	MakeLog("Stop Button Click");
//	CBaseTaskManager::SendProcessCommand(eProcCmd_Stop, eEqpCtrlMode_Manual, NULL);
	CBaseTaskManager::SendProcessCommand(eProcCmd_Pause, eEqpCtrlMode_Manual, NULL);
}

void CDlgManu::OnOperatorCommand(int nOperatorCommand, WPARAM wParam, LPARAM lParam)
{

}

int CDlgManu::OnCommand(CBaseSeqCmd* pSeqCmd, int nEqpCtrlCmdType/*=eEqpCtrlMode_Auto*/)
{
	int nChkCmd = 0;
	if(pSeqCmd == NULL)
	{
		return eSeqCmdAck_InvalidCmd;
	}

	int nChkParam = pSeqCmd->CheckParam();
	if( nChkParam > 0 ){
		delete pSeqCmd;
		pSeqCmd = NULL;
		return eSeqCmdAck_InvalidParam;
	}

	CBaseTaskManager::AddSeqCmd(pSeqCmd, nEqpCtrlCmdType);
	return eSeqCmdAck_OK;
}

int CDlgManu::OnEvent(const CBaseTaskManager* pSender, int nEventId, CParamList* pDataList/*=NULL*/)
{
	return 0;

	CString strMsg;
	g_DMCont.GetEvtMessage(nEventId, strMsg);
	UpdateEventMessage( strMsg );

	return 0;
}

int CDlgManu::OnCommandRsp(CBaseSeqCmd* pRespCmd, int nEventId, WPARAM wParam, LPARAM lParam)
{
// 	if(pRespCmd->m_nCmdId == eSeqCmd_LoadTbl_MovePos && nEventId == CMD_EVT_CMD_END){
// 		g_TaskLoadTbl[0].SetWorkReserve(RID_TEST_PP_(RIDX_TRAY_PP_1));
// 	}
	return 0;
}

void CDlgManu::GetTimeString(CString& strTimeString)
{
	SYSTEMTIME t;
	::GetLocalTime(&t);
	strTimeString.Format("[%02d:%02d.%03d]", t.wMinute, t.wSecond, t.wMilliseconds);
}


void CDlgManu::UpdateEventMessage(const char* fmt, ...)
{
	char szEventMessage[1024]={0,};
	va_list va;
	va_start(va, fmt);
	vsprintf_s(szEventMessage, sizeof(szEventMessage), fmt, va);
	va_end(va);

	if( m_editManuOperEvent.GetLineCount() > 3 ){
		m_editManuOperEvent.SetWindowText("");
	}
	CString strTimeString;
	GetTimeString( strTimeString );

	CString strTemp;
	m_editManuOperEvent.GetWindowText(strTemp);
	if( strTemp != "" ){
		strTemp += _T("\r\n");
	}
	strTemp += strTimeString + _T(" ");
	strTemp += szEventMessage;

	m_editManuOperEvent.SetWindowText( strTemp );
}

void CDlgManu::OnBnClickedButtonManuFeeder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDlgManuFeeder dlg;
	dlg.DoModal();
}


void CDlgManu::OnBnClickedButtonManuLoadTbl12()
{
	// 미사용
	//CDlgManuLoadTbl dlg;
	//dlg.m_nLoadTblIdx = CTaskTrayPP::eLoadTbl_CylPos_1to2;
	//dlg.DoModal();
}

void CDlgManu::OnBnClickedButtonManuLoadTbl13()
{
	CDlgManuLoadTbl dlg;
	dlg.DoModal();
}

void CDlgManu::OnBnClickedBtnAixsManuPressUp()
{
	int nErr = g_TaskSystemCtrl.CheckEqpConditionForMultiOrg();
	if(nErr != ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData,&g_TaskSystemCtrl);
		return;
	}

	if(m_bManualCmdExcute){
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable();
	if(nInterlockErr !=ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	int nRet = AfxMessageBox("Do you want Multi Origin Press Up Start?????", MB_YESNO);
	if( nRet == IDYES ){
		CMultiOrgCmd_Axis* pSeqCmd = new CMultiOrgCmd_Axis;
		pSeqCmd->m_nCmdStage = eMultiOrgType_PressUp;
		g_TaskSystemCtrl.SendCommand(g_MulitOrg, pSeqCmd, eEqpCtrlMode_Manual);

		MakeLog("Multi Origin(Press Up) Start");
	}
}


void CDlgManu::OnBnClickedBtnAixsManuPressDn()
{
	int nErr = g_TaskSystemCtrl.CheckEqpConditionForMultiOrg();
	if(nErr != ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData,&g_TaskSystemCtrl);
		return;
	}
	if(m_bManualCmdExcute){
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable();
	if(nInterlockErr !=ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	int nRet = AfxMessageBox("Do you want Multi Origin Press Down Start?????", MB_YESNO);
	if( nRet == IDYES ){
		CMultiOrgCmd_Axis* pSeqCmd = new CMultiOrgCmd_Axis;
		pSeqCmd->m_nCmdStage = eMultiOrgType_PressDown;
		g_TaskSystemCtrl.SendCommand(g_MulitOrg, pSeqCmd, eEqpCtrlMode_Manual);

		MakeLog("Multi Origin(Press Down) Start");
	}
}

void CDlgManu::OnBnClickedBtnAixsManuTestpp()
{
	int nErr = g_TaskSystemCtrl.CheckEqpConditionForMultiOrg();
	if(nErr != ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData,&g_TaskSystemCtrl);
		return;
	}
	if(m_bManualCmdExcute){
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable();
	if(nInterlockErr !=ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	int nRet = AfxMessageBox("Do you want Multi Origin Test PP Start?????", MB_YESNO);
	if( nRet == IDYES ){
		CMultiOrgCmd_Axis* pSeqCmd = new CMultiOrgCmd_Axis;
		pSeqCmd->m_nCmdStage = eMultiOrgType_TestPp;
		g_TaskSystemCtrl.SendCommand(g_MulitOrg, pSeqCmd, eEqpCtrlMode_Manual);

		MakeLog("Multi Origin(Test PP) Start");
	}
}

void CDlgManu::OnBnClickedBtnAixsManuLoadTbl()
{
	int nErr = g_TaskSystemCtrl.CheckEqpConditionForMultiOrg();
	if(nErr != ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData,&g_TaskSystemCtrl);
		return;
	}
	if(m_bManualCmdExcute){
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable();
	if(nInterlockErr !=ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	int nRet = AfxMessageBox("Do you want Multi Origin Load Table Start?????", MB_YESNO);
	if( nRet == IDYES ){
		CMultiOrgCmd_Axis* pSeqCmd = new CMultiOrgCmd_Axis;
		pSeqCmd->m_nCmdStage = eMultiOrgType_LoadTable;
		g_TaskSystemCtrl.SendCommand(g_MulitOrg, pSeqCmd, eEqpCtrlMode_Manual);

		MakeLog("Multi Origin(Load Table) Start");
	}
}


void CDlgManu::OnBnClickedBtnAixsManuMod1()
{
	int nErr = g_TaskSystemCtrl.CheckEqpConditionForMultiOrg();
	if(nErr != ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData,&g_TaskSystemCtrl);
		return;
	}
	if(m_bManualCmdExcute){
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable();
	if(nInterlockErr !=ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	int nRet = AfxMessageBox("Do you want Multi Origin Module #1 Start?????", MB_YESNO);
	if( nRet == IDYES ){
		CMultiOrgCmd_Axis* pSeqCmd = new CMultiOrgCmd_Axis;
		pSeqCmd->m_nCmdStage = eMultiOrgType_Module1;
		g_TaskSystemCtrl.SendCommand(g_MulitOrg, pSeqCmd, eEqpCtrlMode_Manual);

		MakeLog("Multi Origin(Module #1) Start");
	}
}


void CDlgManu::OnBnClickedBtnAixsManuMod2()
{
	int nErr = g_TaskSystemCtrl.CheckEqpConditionForMultiOrg();
	if(nErr != ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData,&g_TaskSystemCtrl);
		return;
	}
	if(m_bManualCmdExcute){
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable();
	if(nInterlockErr !=ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	int nRet = AfxMessageBox("Do you want Multi Origin Module #2 Start?????", MB_YESNO);
	if( nRet == IDYES ){
		CMultiOrgCmd_Axis* pSeqCmd = new CMultiOrgCmd_Axis;
		pSeqCmd->m_nCmdStage = eMultiOrgType_Module2;
		g_TaskSystemCtrl.SendCommand(g_MulitOrg, pSeqCmd, eEqpCtrlMode_Manual);

		MakeLog("Multi Origin(Module #2) Start");
	}
}


void CDlgManu::OnBnClickedBtnAixsManuMod3()
{
	int nErr = g_TaskSystemCtrl.CheckEqpConditionForMultiOrg();
	if(nErr != ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData,&g_TaskSystemCtrl);
		return;
	}
	if(m_bManualCmdExcute){
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable();
	if(nInterlockErr !=ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	int nRet = AfxMessageBox("Do you want Multi Origin Module #3 Start?????", MB_YESNO);
	if( nRet == IDYES ){
		// 미사용
		//CMultiOrgCmd_Axis* pSeqCmd = new CMultiOrgCmd_Axis;
		//pSeqCmd->m_nCmdStage = eMultiOrgType_Module3;
		//g_TaskSystemCtrl.SendCommand(g_MulitOrg, pSeqCmd, eEqpCtrlMode_Manual);
		MakeLog("Multi Origin(Module #3) Start");
	}
}


void CDlgManu::OnBnClickedBtnAixsManuMod4()
{
	int nErr = g_TaskSystemCtrl.CheckEqpConditionForMultiOrg();
	if(nErr != ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData,&g_TaskSystemCtrl);
		return;
	}
	if(m_bManualCmdExcute){
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable();
	if(nInterlockErr !=ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	int nRet = AfxMessageBox("Do you want Multi Origin Module #4 Start?????", MB_YESNO);
	if( nRet == IDYES ){
		// 미사용
		//CMultiOrgCmd_Axis* pSeqCmd = new CMultiOrgCmd_Axis;
		//pSeqCmd->m_nCmdStage = eMultiOrgType_Module4;
		//g_TaskSystemCtrl.SendCommand(g_MulitOrg, pSeqCmd, eEqpCtrlMode_Manual);
		
		MakeLog("Multi Origin(Module #4) Start");
	}
}


void CDlgManu::OnBnClickedBtnAixsManuMod5()
{
	int nErr = g_TaskSystemCtrl.CheckEqpConditionForMultiOrg();
	if(nErr != ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData,&g_TaskSystemCtrl);
		return;
	}
	if(m_bManualCmdExcute){
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable();
	if(nInterlockErr !=ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	int nRet = AfxMessageBox("Do you want Multi Origin Module All Start?????", MB_YESNO);
	if( nRet == IDYES ){
		CMultiOrgCmd_Axis* pSeqCmd = new CMultiOrgCmd_Axis;
		pSeqCmd->m_nCmdStage = eMultiOrgType_ModuleAll;
		g_TaskSystemCtrl.SendCommand(g_MulitOrg, pSeqCmd, eEqpCtrlMode_Manual);

		MakeLog("Multi Origin(Module All) Start");
	}
}


void CDlgManu::OnBnClickedBtnAixsManuMod6()
{
	int nErr = g_TaskSystemCtrl.CheckEqpConditionForMultiOrg();
	if(nErr != ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData,&g_TaskSystemCtrl);
		return;
	}
	if(m_bManualCmdExcute){
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable();
	if(nInterlockErr !=ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	int nRet = AfxMessageBox("Do you want Multi Origin EQP Up Pos All Start?????", MB_YESNO);
	if( nRet == IDYES ){
		CMultiOrgCmd_Axis* pSeqCmd = new CMultiOrgCmd_Axis;
		pSeqCmd->m_nCmdStage = eMultiOrgType_EqpUpAll;
		g_TaskSystemCtrl.SendCommand(g_MulitOrg, pSeqCmd, eEqpCtrlMode_Manual);

		MakeLog("Multi Origin(EQP Up Pos All) Start");
	}
}


void CDlgManu::OnBnClickedBtnAixsManuAll()
{
	int nErr = g_TaskSystemCtrl.CheckEqpConditionForMultiOrg();
	if(nErr != ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData,&g_TaskSystemCtrl);
		return;
	}
	if(m_bManualCmdExcute){
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable();
	if(nInterlockErr !=ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	int nRet = AfxMessageBox("Do you want Multi Origin Axis All Start?????", MB_YESNO);
	if( nRet == IDYES ){	
		CMultiOrgCmd_Axis* pSeqCmd = new CMultiOrgCmd_Axis;
		pSeqCmd->m_nCmdStage = eMultiOrgType_AxisAll;
		g_TaskSystemCtrl.SendCommand(g_MulitOrg, pSeqCmd, eEqpCtrlMode_Manual);

		if (m_pDlgHomeCheckStatus->IsWindowVisible() == FALSE) {
			m_pDlgHomeCheckStatus->ShowWindow(SW_SHOW);
		}

		MakeLog("Multi Origin(Axis All) Start");
	}

	double dForcePerPin = g_DMCont.m_dmHandlerOpt.GD(DDM11_DvcContactForce_GF);
	if (dForcePerPin <= 0.0) {
		dForcePerPin = 0.0;
	}
	for (int i = 0; i < eMaxPressUnitCount; i++)
	{
		g_TaskRegulator[i].do_SetForcePerPin(dForcePerPin);
		Sleep(10);
	}

}

void CDlgManu::MakeLog(LPCTSTR fmt, ...)
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

void CDlgManu::OnBnClickedBtnAixsManuStacker1()
{
	OnBnClickedBtnAixsManuStacker(eSTACKER_01);
}


void CDlgManu::OnBnClickedBtnAixsManuStacker2()
{
	OnBnClickedBtnAixsManuStacker(eSTACKER_02);
}


void CDlgManu::OnBnClickedBtnAixsManuStacker3()
{
	OnBnClickedBtnAixsManuStacker(eSTACKER_03);
}


void CDlgManu::OnBnClickedBtnAixsManuStacker4()
{
	OnBnClickedBtnAixsManuStacker(eSTACKER_04);
}


void CDlgManu::OnBnClickedBtnAixsManuStacker5()
{
	OnBnClickedBtnAixsManuStacker(eSTACKER_05);
}


void CDlgManu::OnBnClickedBtnAixsManuStacker6()
{
	OnBnClickedBtnAixsManuStacker(eSTACKER_06);
}


void CDlgManu::OnBnClickedBtnAixsManuStacker7()
{
	OnBnClickedBtnAixsManuStacker(eSTACKER_07);
}

void CDlgManu::OnBnClickedBtnAixsManuStacker(int nStackerIdx )
{
	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	BOOL bManualMode = FALSE;

	if(m_bManualCmdExcute){
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	if(!g_COVER_SW.GetStatus()){
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK,AlarmData,NULL);
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable(bManualMode);
	if(nInterlockErr !=ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	MakeLog("Stacker%d Dialog Show",nStackerIdx);
	CDlgCommand_Stacker CmdDlg;
	CmdDlg.m_nStackerIdx = nStackerIdx;
	if( IDOK == CmdDlg.DoModal() )
	{
		int nSelectedCmd = CmdDlg.GetSelectedSeqCmdId();
		switch( nSelectedCmd )
		{
		case eSeqCmd_Stacker_Initial:
			{
				
			}break;
		case eSeqCmd_Stacker_Load:
			{
				CStackerCmd_Load* pSeqCmd = new CStackerCmd_Load;
				int nErr = g_TaskSystemCtrl.SendCommand(g_TaskStacker[nStackerIdx], pSeqCmd, eEqpCtrlMode_Manual);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					g_Error.AlarmReport(nErr, AlarmData, NULL);
				}
			}break;
		case eSeqCmd_Stacker_TrayDn:
			{
				CStackerCmd_TrayDn* pSeqCmd = new CStackerCmd_TrayDn;
				int nErr = g_TaskSystemCtrl.SendCommand( g_TaskStacker[nStackerIdx], pSeqCmd, eEqpCtrlMode_Manual);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					g_Error.AlarmReport(nErr, AlarmData, NULL);
				}
			}break;
		case eSeqCmd_Stacker_TrayUp:
			{
				CStackerCmd_TrayUp* pSeqCmd = new CStackerCmd_TrayUp;
				int nErr = g_TaskSystemCtrl.SendCommand( g_TaskStacker[nStackerIdx], pSeqCmd, eEqpCtrlMode_Manual);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					g_Error.AlarmReport(nErr, AlarmData, NULL);
				}
			}break;
		case eSeqCmd_Stacker_Unload:
			{
				CStackerCmd_Unload* pSeqCmd = new CStackerCmd_Unload;
				int nErr = g_TaskSystemCtrl.SendCommand( g_TaskStacker[nStackerIdx], pSeqCmd, eEqpCtrlMode_Manual);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					g_Error.AlarmReport(nErr, AlarmData, NULL);
				}
			}break;
		case eSeqCmd_Stacker_TrayOut:
			{
				CStackerCmd_TrayOut* pSeqCmd = new CStackerCmd_TrayOut;
				int nErr = g_TaskSystemCtrl.SendCommand(g_TaskStacker[nStackerIdx], pSeqCmd, eEqpCtrlMode_Manual);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					g_Error.AlarmReport(nErr, AlarmData, NULL);
				}
			}break;
		case eSeqCmd_Stacker_Vibration:
			{
				double Action_Delay_Sec = g_DMCont.m_dmConveyor.GD( DDM6_VIBRATOR_ACT_DELAY_SEC );
				int Action_Act_Time = g_DMCont.m_dmConveyor.GN( NDM6_VIBRATOR_ACT_TIMES );
				CStackerCmd_Vibration* pSeqCmd = new CStackerCmd_Vibration( Action_Delay_Sec, Action_Act_Time );
				int nErr = g_TaskSystemCtrl.SendCommand( g_TaskStacker[nStackerIdx], pSeqCmd, eEqpCtrlMode_Manual);
				if( nErr != ERR_NO_ERROR ){
					CParamList AlarmData;
					g_Error.AlarmReport(nErr, AlarmData, NULL);
				}
			}break;
		}	
	}
}

void CDlgManu::OnBnClickedButtonManuCylinderCheck()
{
	if (m_bManualCmdExcute) {
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	if (!g_COVER_SW.GetStatus()) {
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK, AlarmData, NULL);
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable();
	if (nInterlockErr != ERR_NO_ERROR) {
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	CDlgCylinderTimeCheck dlg;
	dlg.DoModal();
}


void CDlgManu::OnBnClickedBtnAixsManuTrayPp1()
{
	OnBnClickedBtnAixsManuTrayPp(eTrayPP_1_LD);
}


void CDlgManu::OnBnClickedBtnAixsManuTrayPp2()
{
	OnBnClickedBtnAixsManuTrayPp(eTrayPP_2_UD);
}


void CDlgManu::OnBnClickedButtonManuTrayPp2()
{
	g_DMCont.m_dmEQP.SB(BDM0_OPEN_EJECTOR_DLG_HISTORY, TRUE);//jira 403
	CDlgManuTrayPP dlg;
	dlg.m_nHandIdx = eTrayPP_2_UD;
	dlg.DoModal();
	g_DMCont.m_dmEQP.SB(BDM0_OPEN_EJECTOR_DLG_HISTORY, FALSE);//jira 403
}


void CDlgManu::OnBnClickedBtnAxisManualFeeder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_bManualCmdExcute) {
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	if (!g_COVER_SW.GetStatus()) {
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK, AlarmData, NULL);
		return;
	}

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable(TRUE);
	if (nInterlockErr != ERR_NO_ERROR) {
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	CDlgCommand_Feeder CmdDlg;
	MakeLog("Feeder Command Dialog Show");
	if (IDOK == CmdDlg.DoModal())
	{
		int nSelectedCmd = CmdDlg.GetSelectedSeqCmdId();
		switch (nSelectedCmd)
		{
		case eSeqCmd_Feeder_Initial:
		{
			CTrayFeederCmd_Initial* pSeqCmd = new CTrayFeederCmd_Initial();
			int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTrayFeeder, pSeqCmd, eEqpCtrlMode_Manual);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
			}
			else {
				MakeLog("Feeder Initial");
			}

		}break;
		case eSeqCmd_Feeder_MovePos:
		{
			CDlgAxisActionMove dlg;
			dlg.m_nStartPoint = eMANU_OPER_FEEDER;
			dlg.m_nSelectAction = eDlgActionOnlyMove;
			dlg.DoModal();
		}break;
		case eSeqCmd_Feeder_MovePickPlace:
		{
			CDlgAxisActionMove dlg;
			dlg.m_nStartPoint = eMANU_OPER_FEEDER;
			dlg.m_nSelectAction = eDlgActionPick;
			dlg.DoModal();
		}break;
		}
	}
}





void CDlgManu::OnBnClickedButtonTrigger()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	

// 	double dData[8] = { 0, };
// 	memcpy(dData, g_VATCommonData.m_stVATZTeachInfo.dLoadTrayPp_Loc_PickerGap[LOC_IDX_TRAY_PP_TBL_2], sizeof(g_VATCommonData.m_stVATZTeachInfo.dLoadTrayPp_Loc_PickerGap[LOC_IDX_TRAY_PP_TBL_2]));
// 	g_pBbxmnpClient->SetZAutoTeachingData(g_VATCommonData.m_stVATZTeachInfo.dLoadTrayPp_Loc_PickerGap[LOC_IDX_TRAY_PP_TBL_2]);

	//	memcpy(m_dHandGap, dHandGap, sizeof(&dHandGap));



/*	int a = 0;*/


// 	g_pBarcode->Erase2dPicture(0);
// 
// 
// 
// 	return;


	int nTester0 = g_trigger_generator.GetTriggerCurPos(0) ;
	int nTester1 = g_trigger_generator.GetTriggerCurPos(1);

	g_trigger_generator.SetTriggerPos(0);
	g_trigger_generator.SetStartTrigger(1);
	g_trigger_generator.SetTriggerPos(2);
	g_trigger_generator.SetStartTrigger(3);

	int a = 0;



	////////////////////////////////////////////////////////////////////////////////
// 	CInternetSession session;
// 	CFtpConnection* pConnect = NULL;
// 	CFtpFileFind* pFileFind = NULL;
// 	CString strDir, StrResult;
// 
// 	CString cstrFindFile;
// 
// 
// 	try
// 	{
// 		pConnect = session.GetFtpConnection("192.168.100.101", "abc", "123");
// 		pFileFind = new CFtpFileFind(pConnect); //파일 찾기
// 
// 		BOOL bExist = pFileFind->FindFile("IMAGE");
// 
// 
// 
// 		while (bExist)
// 		{
// 			bExist = pFileFind->FindNextFile();
// 
// 
// 
// 			if (bExist != TRUE) {
// 				pConnect->Remove(pFileFind->GetFilePath());
// 				break;
// 			}
// 
// 			CString strFileName = pFileFind->GetFilePath();
// 
// 			if (pFileFind->IsDots())
// 			{
// 				continue;
// 			}
// 
// 			//int nRet = pConnect->RemoveDirectory(strFileName);
// 			int nRt = pConnect->Remove(strFileName);
// 
// 			/*::FtpDeleteFile(strFileName);*/
// 			
// 		}
// 	}
// 	catch (CInternetException *e)
// 	{
// 		e->ReportError(MB_ICONASTERISK);
// 		pConnect = NULL;
// 		e->Delete();
// 	}
// 
// 	if(pConnect != NULL)
// 		pConnect->Close();
}
