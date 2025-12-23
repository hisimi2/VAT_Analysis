// DlgManuMotorJog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgManuMotorJog.h"
#include "afxdialogex.h"
#include "../Include/OcxFiles/dsspanel.h"

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
#include "DlgManuLoadTbl.h"
#include "DlgManuFeeder.h"

#include "DlgCommand_Stacker.h"
#include "DlgCommand_Transfer.h"
#include "DlgCommand_TrayPP.h"
#include "DlgCommand_LoadTbl.h"
#include "DlgCommand_TestPP.h"
#include "DlgCommand_PressUnit.h"
#include "DlgCommand_Param.h"
#include "DlgCommand_Feeder.h"

enum{ eJogPressDown1, eJogPressDown2, eJogPressDown3, eJogPressDown4, eJogPressDown5, eJogPressDown6, eJogPressDown7, eJogPressDown8,
	eJogTestPp1, eJogTestPp2, eJogLdTbl1, eJogLdTbl2, eJogLdTbl3, eJogLdTbl4, eJogTrayPp1_LD, eJogTrayPp2_UD, eJogFeeder, eJogTransfer
};

// CDlgManuMotorJog 대화 상자입니다.
using namespace base_seq_cmd;

IMPLEMENT_DYNAMIC(CDlgManuMotorJog, CDialogEx)

CDlgManuMotorJog::CDlgManuMotorJog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgManuMotorJog::IDD, pParent)
{
	m_nSelRobot    = 0;
	m_nJogSpeed    = 0;
	m_strRobotName = _T("");
	
	m_vHandGui.clear();
	m_vSpeed.clear();
	m_vRotateSpeed.clear();
	m_vTransfer.clear();
	
	m_pMotorX = NULL;
	m_pMotorY = NULL;
	m_pMotorZ = NULL;
	m_pMotorR = NULL;

	m_bServoStat = FALSE;

	m_vAxisNo.clear();

	memset(m_nHandVacBlwSen, 0x00, sizeof(m_nHandVacBlwSen));
	memset(m_nHandSenID    , 0x00, sizeof(m_nHandSenID));
	m_nContactForceAxisNo = -1;

	m_bManualCmdExcute = FALSE;
	m_nTestPPHandType = 0;
	m_sTestHandName =_T("");
	m_strTitleName = _T("");

	CBaseTaskManager::SetEqpCtrlMode(eEqpCtrlMode_Manual);

}

CDlgManuMotorJog::~CDlgManuMotorJog()
{
	if(m_pMotorX != NULL){
		m_pMotorX = NULL;
	}

	if(m_pMotorY != NULL){
		m_pMotorY = NULL;
	}

	if(m_pMotorZ != NULL){
		m_pMotorZ = NULL;
	}	

	if (m_pMotorR != NULL) {
		m_pMotorR = NULL;
	}

	if(m_cBmpPress.m_hObject != NULL)
		m_cBmpPress.DeleteObject();

	if (m_cBmpLoadTestPP1.m_hObject != NULL)
		m_cBmpLoadTestPP1.DeleteObject();
	
	if (m_cBmpLoadTestPP2.m_hObject != NULL)
		m_cBmpLoadTestPP2.DeleteObject();

	if (m_cBmpTrayPP.m_hObject != NULL)
		m_cBmpTrayPP.DeleteObject();

	if (m_cBmpTrayFeeder.m_hObject != NULL)
		m_cBmpTrayFeeder.DeleteObject();

	if (m_cBmpTransfer.m_hObject != NULL)
		m_cBmpTransfer.DeleteObject();

	if (m_cBmpTestPP1Axis.m_hObject != NULL)
		m_cBmpTestPP1Axis.DeleteObject();

	if (m_cBmpTestPP2Axis.m_hObject != NULL)
		m_cBmpTestPP2Axis.DeleteObject();

	if (m_cBmpLoadTable1Axis.m_hObject != NULL)
		m_cBmpLoadTable1Axis.DeleteObject();

	if (m_cBmpLoadTable2Axis.m_hObject != NULL)
		m_cBmpLoadTable2Axis.DeleteObject();


	if (m_cBmpLoadTable3Axis.m_hObject != NULL)
		m_cBmpLoadTable3Axis.DeleteObject();

	if (m_cBmpLoadTable4Axis.m_hObject != NULL)
		m_cBmpLoadTable4Axis.DeleteObject();

	if (m_cBmpTrayPPAxis.m_hObject != NULL)
		m_cBmpTrayPPAxis.DeleteObject();

	if (m_cBmpTrayFeederAxis.m_hObject != NULL)
		m_cBmpTrayFeederAxis.DeleteObject();

	if (m_cBmpTransferAxis.m_hObject != NULL)
		m_cBmpTransferAxis.DeleteObject();

	
}

void CDlgManuMotorJog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_SPEED, m_nJogSpeed);
	DDX_Control(pDX, IDC_BTN_SERVO_FREE, m_btnServoFree);
	DDX_Control(pDX, IDC_BUTTON_CYLINDER, m_btnCylinder);
	DDX_Control(pDX, IDC_BUTTON_ST_UNIT_OPERATION, m_btnSTOperation);
	DDX_Control(pDX, IDC_BUTTON_UNIT_OPERATION, m_btnOperation);

	DDX_Control(pDX, IDC_BUTTON_MANU_PROC_CMD_STOP, m_btnStop);
	DDX_Control(pDX, IDC_BUTTON_MANU_PROC_CMD_SKIP, m_btnSkip);
	
	DDX_Control(pDX, IDC_CHECK_VISION_LED, m_chkVisionLED);
	DDX_Control(pDX, IDC_SSPANEL1, m_SSPANELTitle);

	//DDX_Control(pDX, IDC_BUTTON_CYLINDER, m_btnCylinderST);
}


BEGIN_MESSAGE_MAP(CDlgManuMotorJog, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON_CYLINDER, &CDlgManuMotorJog::OnBnClickedButtonCylinder)
	ON_BN_CLICKED(IDC_BUTTON_UNIT_OPERATION, &CDlgManuMotorJog::OnBnClickedButtonUnitOperation)
	ON_BN_CLICKED(IDC_BUTTON_ST_UNIT_OPERATION, &CDlgManuMotorJog::OnBnClickedButtonStUnitOperation)
	ON_BN_CLICKED(IDC_BUTTON_MANU_PROC_CMD_STOP, &CDlgManuMotorJog::OnBnClickedButtonManuProcCmdStop)
	ON_BN_CLICKED(IDC_BUTTON_MANU_PROC_CMD_SKIP, &CDlgManuMotorJog::OnBnClickedButtonManuProcCmdSkip)
	ON_BN_CLICKED(IDC_CHECK_VISION_LED, &CDlgManuMotorJog::OnBnClickedCheckVisionLed)
END_MESSAGE_MAP()


// CDlgManuMotorJog 메시지 처리기입니다.

BOOL CDlgManuMotorJog::PreTranslateMessage(MSG* pMsg)
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

BOOL CDlgManuMotorJog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_strRobotName += " :: Jog Teching";
	m_strRobotName += m_sTestHandName;
	
	::SetWindowTextA(this->m_hWnd, m_strRobotName);
	
	m_strTitleName = _T("Motor Jog");
	m_strTitleName += m_sTestHandName;
	m_SSPANELTitle.SetWindowText(m_strTitleName);

	InitMotorAxis();

	// Hand Vac/Blow GUI ID
	m_vHandGui.push_back(IDC_HAND_LABEL1);	m_vHandGui.push_back(IDC_HAND_LABEL2);
	m_vHandGui.push_back(IDC_CTRL1_1); m_vHandGui.push_back(IDC_CTRL1_2); m_vHandGui.push_back(IDC_CTRL1_3); m_vHandGui.push_back(IDC_CTRL1_4);
	m_vHandGui.push_back(IDC_CTRL1_5); m_vHandGui.push_back(IDC_CTRL1_6); m_vHandGui.push_back(IDC_CTRL1_7); m_vHandGui.push_back(IDC_CTRL1_8);
	m_vHandGui.push_back(IDC_CTRL2_1); m_vHandGui.push_back(IDC_CTRL2_2); m_vHandGui.push_back(IDC_CTRL2_3); m_vHandGui.push_back(IDC_CTRL2_4);
	m_vHandGui.push_back(IDC_CTRL2_5); m_vHandGui.push_back(IDC_CTRL2_6); m_vHandGui.push_back(IDC_CTRL2_7); m_vHandGui.push_back(IDC_CTRL2_8);
	m_vHandGui.push_back(IDC_CBUTTON_ALL_VACUUM);	m_vHandGui.push_back(IDC_CBUTTON_ALL_BLOW);
	m_vHandGui.push_back(IDC_STATIC_VACUUMSENSOR);

	// vac/blow sensor
	for(int nX=0; nX < MAX_COL; nX++){
		m_nHandSenID[0][nX] = m_vHandGui[2+nX];  // IDC_CTRL1_1 ~ IDC_CTRL1_8
		m_nHandSenID[1][nX] = m_vHandGui[10+nX]; //IDC_CTRL2_1 ~ IDC_CTRL2_8
	}

	// Transfer Sen ID
	m_vTransfer.push_back(IDC_COLORCHECKBTCTRL_Z1_SAFETY_SEN); m_vTransfer.push_back(IDC_COLORCHECKBTCTRL_Z2_SAFETY_SEN);
	m_vTransfer.push_back(IDC_SEN_Z1_TOUCH);                   m_vTransfer.push_back(IDC_SEN_Z2_TOUCH);
	m_vTransfer.push_back(IDC_SEN_LD_AREA);                    m_vTransfer.push_back(IDC_SEN_SETPLATE_AREA);
	m_vTransfer.push_back(IDC_STATIC_TRANSFER_SAFETY_Z_SEN);
	// Stacker Sen ID
	m_vTransfer.push_back(IDC_STATIC_STACKER_SAFETY_Z_SEN);
	m_vTransfer.push_back(IDC_SEN_X_MOVE_STACKER_LEFT);        m_vTransfer.push_back(IDC_SEN_X_MOVE_STACKER_RIGHT);
	m_vTransfer.push_back(IDC_SEN_CLAMP_STACKER_LEFT);         m_vTransfer.push_back(IDC_SEN_CLAMP_STACKER_RIGHT);
	

	SetDlgItemText(IDC_X_POS, _T("0"));
	SetDlgItemText(IDC_Y_POS, _T("0"));
	SetDlgItemText(IDC_Z_POS, _T("0"));

	// Set V-Move Speed // 순서 바꾸지 마시길!!
	m_vSpeed.push_back(100);
	m_vSpeed.push_back(500);
	m_vSpeed.push_back(1000);
	m_vSpeed.push_back(2000);
	m_vSpeed.push_back(3000);
	m_vSpeed.push_back(4000);
	m_vSpeed.push_back(5000);
	m_vSpeed.push_back(8000);
	m_vSpeed.push_back(10000);
	m_vSpeed.push_back(20000);

	if (m_pMotorR != NULL) {
		m_vRotateSpeed.push_back(10);
		m_vRotateSpeed.push_back(20);
		m_vRotateSpeed.push_back(30);
		m_vRotateSpeed.push_back(40);
		m_vRotateSpeed.push_back(50);
		m_vRotateSpeed.push_back(60);
		m_vRotateSpeed.push_back(70);
		m_vRotateSpeed.push_back(80);
		m_vRotateSpeed.push_back(85);
		m_vRotateSpeed.push_back(90);
	}

	if (m_pMotorZ != NULL) {
		m_vZSpeed.push_back(5);
		m_vZSpeed.push_back(10);
		m_vZSpeed.push_back(15);
		m_vZSpeed.push_back(20);
		m_vZSpeed.push_back(25);
		m_vZSpeed.push_back(30);
		m_vZSpeed.push_back(35);
		m_vZSpeed.push_back(40);
		m_vZSpeed.push_back(45);
		m_vZSpeed.push_back(50);
	}
// 	GetDlgItem(IDC_CHECK_VISION_LED)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STATIC_VISION_LED)->ShowWindow(SW_HIDE);
	
	InitUI();
	InitCylinderUI();
	InitUnitOperationUI();

	m_btnStop.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	//m_btnStop.SetIcon(IDI_ICON_PRESS);
	m_btnStop.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnSkip.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	//m_btnSkip.SetIcon(IDI_ICON_PRESS);
	m_btnSkip.SetAlign(CButtonST::ST_ALIGN_VERT);

	if(m_pMotorX != NULL){
		m_pMotorX->ServoOn();
	}
	if(m_pMotorY != NULL){
		m_pMotorY->ServoOn();
	}
	if(m_pMotorZ != NULL){
		m_pMotorZ->ServoOn();
	}

	m_bServoStat = DEF_ON;

	SetTimer(0, DEF_SENSOR_DISP_TM, NULL);

	SetTimer(100, DEF_SENSOR_DISP_TM, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgManuMotorJog::OnDestroy()
{
	CDialogEx::OnDestroy();
	
	KillTimer(0);
	KillTimer(10);
	KillTimer(11);
	KillTimer(20);
	KillTimer(21);
	KillTimer(30);
	KillTimer(31);
	KillTimer(100);
}

void CDlgManuMotorJog::InitMotorAxis()
{
	_tMOTOR_PARAM rMotorParam;
	for(int i=0; i< (int)m_vAxisNo.size(); i++){
		// X-pitch & Y-Pitch는 예외처리
		if(m_vAxisNo[i] == eAXIS_TRAY_PP_1_X_PITCH || m_vAxisNo[i] == eAXIS_TRAY_PP_1_Y_PITCH || m_vAxisNo[i] == eAXIS_TRAY_PP_2_X_PITCH 
			|| m_vAxisNo[i] == eAXIS_TRAY_PP_2_Y_PITCH || m_vAxisNo[i] == eAXIS_TEST_PP_1_X_PITCH ||m_vAxisNo[i] == eAXIS_TEST_PP_1_X2_PITCH ||
			m_vAxisNo[i] == eAXIS_TEST_PP_2_X_PITCH || m_vAxisNo[i] == eAXIS_TEST_PP_2_X2_PITCH || m_vAxisNo[i] == -1
			|| m_vAxisNo[i] == eAXIS_TEST_PP_1_Y_PITCH || m_vAxisNo[i] == eAXIS_TEST_PP_1_Y2_PITCH ||
			m_vAxisNo[i] == eAXIS_TEST_PP_2_Y_PITCH || m_vAxisNo[i] == eAXIS_TEST_PP_2_Y2_PITCH || m_vAxisNo[i] == -1)
		{
			continue;
		}

		int nRet = g_DB.SELECT_MOTORPARAM_TBL(m_vAxisNo[i], rMotorParam);
		if(rMotorParam.eMotorCoordinate == _tMOTOR_PARAM::X){
			m_pMotorX = (OneAxis*)g_Motor[m_vAxisNo[i]];
		}
		else if(rMotorParam.eMotorCoordinate == _tMOTOR_PARAM::Y){
			m_pMotorY = (OneAxis*)g_Motor[m_vAxisNo[i]];
		}
 		else if(rMotorParam.eMotorCoordinate == _tMOTOR_PARAM::Picker_Z ||
 			rMotorParam.eMotorCoordinate == _tMOTOR_PARAM::Z){
			m_pMotorZ = (OneAxis*)g_Motor[m_vAxisNo[i]];			
// 			if( g_MotorDriverComIF.IsUsableAxis(static_cast<eAxisNo>(m_vAxisNo[i])) ){
// 				m_nContactForceAxisNo = m_vAxisNo[i];
// 				g_MotorDriverComIF.start_comm_running(static_cast<eAxisNo>(m_nContactForceAxisNo));
// 			}
		}
		else if (rMotorParam.eMotorCoordinate == _tMOTOR_PARAM::Z)
		{
			m_pMotorZ = (OneAxis*)g_Motor[m_vAxisNo[i]];
		}
		else if (rMotorParam.eMotorCoordinate == _tMOTOR_PARAM::Rotator) {
			m_pMotorR = (OneAxis*)g_Motor[m_vAxisNo[i]];
		}
	}

	if(m_pMotorX == NULL){
		GetDlgItem(DEVICESETDLL_IDC_COLORBTCTRL_LEFT)->EnableWindow(FALSE);
		GetDlgItem(DEVICESETDLL_IDC_COLORBTCTRL_RIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_ORIGIN_X)->ShowWindow(SW_HIDE);
	}else{
		GetDlgItem(DEVICESETDLL_IDC_COLORBTCTRL_LEFT)->EnableWindow(TRUE);
		GetDlgItem(DEVICESETDLL_IDC_COLORBTCTRL_RIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_ORIGIN_X)->ShowWindow(SW_SHOW);
	}
	if(m_pMotorY == NULL){
		GetDlgItem(DEVICESETDLL_IDC_COLORBTCTRL_UP)->EnableWindow(FALSE);
		GetDlgItem(DEVICESETDLL_IDC_COLORBTCTRL_DOWN)->EnableWindow(FALSE);
		GetDlgItem(IDC_ORIGIN_Y)->ShowWindow(SW_HIDE);
	}else{
		GetDlgItem(DEVICESETDLL_IDC_COLORBTCTRL_UP)->EnableWindow(TRUE);
		GetDlgItem(DEVICESETDLL_IDC_COLORBTCTRL_DOWN)->EnableWindow(TRUE);
		GetDlgItem(IDC_ORIGIN_Y)->ShowWindow(SW_SHOW);
	}
	if(m_pMotorZ == NULL){
		GetDlgItem(IDC_COLORBTCTRL_ZUP)->EnableWindow(FALSE);
		GetDlgItem(IDC_COLORBTCTRL_ZDOWN)->EnableWindow(FALSE);
		GetDlgItem(IDC_ORIGIN_Z)->ShowWindow(SW_HIDE);
	}else{
		GetDlgItem(IDC_COLORBTCTRL_ZUP)->EnableWindow(TRUE);
		GetDlgItem(IDC_COLORBTCTRL_ZDOWN)->EnableWindow(TRUE);
		GetDlgItem(IDC_ORIGIN_Z)->ShowWindow(SW_SHOW);
	}

	if (m_pMotorR == NULL) {
		GetDlgItem(IDC_COLORBTCTRL_RR)->EnableWindow(FALSE);
		GetDlgItem(IDC_COLORBTCTRL_LR)->EnableWindow(FALSE);
		GetDlgItem(IDC_ORIGIN_R)->ShowWindow(SW_HIDE);
	}
	else {
		GetDlgItem(IDC_COLORBTCTRL_RR)->EnableWindow(TRUE);
		GetDlgItem(IDC_COLORBTCTRL_LR)->EnableWindow(TRUE);
		GetDlgItem(IDC_ORIGIN_R)->ShowWindow(SW_SHOW);
	}
}


void CDlgManuMotorJog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CDlgManuMotorJog::InitUI()
{
	switch(m_nSelRobot)
	{
	case eJogPressDown1 : 
	case eJogPressDown2 : 
	case eJogPressDown3 : 
	case eJogPressDown4 : 
	case eJogPressDown5 : 
	case eJogPressDown6 : 
	case eJogPressDown7 : 
	case eJogPressDown8 :
		{
			for(int i=0; i < (int)m_vHandGui.size(); i++){
				GetDlgItem(m_vHandGui[i])->ShowWindow(SW_HIDE);
			}

			for(int i=0; i < (int)m_vTransfer.size(); i++){
				GetDlgItem(m_vTransfer[i])->ShowWindow(SW_HIDE);
			}
		}break;
	case eJogTestPp1 :
	case eJogTestPp2 :
		{
			for(int i=0; i < (int)m_vTransfer.size(); i++){
				GetDlgItem(m_vTransfer[i])->ShowWindow(SW_HIDE);
			}
			for(int i=MAX_TEST_PP_PKR_CNT_X; i<MAX_COL; i++){
				GetDlgItem(m_nHandSenID[0][i])->ShowWindow(SW_HIDE);
				GetDlgItem(m_nHandSenID[1][i])->ShowWindow(SW_HIDE);
			}
// 			GetDlgItem(IDC_CHECK_VISION_LED)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_STATIC_VISION_LED)->ShowWindow(SW_SHOW);
		}break;
	case eJogLdTbl1:
	case eJogLdTbl2:
	case eJogLdTbl3:
	case eJogLdTbl4:
		{
			for(int i=0; i < (int)m_vTransfer.size(); i++){
				GetDlgItem(m_vTransfer[i])->ShowWindow(SW_HIDE);
			}
			for(int i=0; i < (int)m_vHandGui.size(); i++){
				GetDlgItem(m_vHandGui[i])->ShowWindow(SW_HIDE);
			}
			GetDlgItem(IDC_STATIC_SAFETY_SEN)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COLORCHECKBTCTRL_Z_SAFETY_SEN)->ShowWindow(SW_HIDE);

		}break;
	case eJogTrayPp1_LD :
	case eJogTrayPp2_UD:
		{
			for(int i=0; i < (int)m_vTransfer.size(); i++){
				GetDlgItem(m_vTransfer[i])->ShowWindow(SW_HIDE);
			}
			for( int i =MAX_TRAY_PP_CNT_X; i < MAX_COL; i++ ) {
				GetDlgItem( m_nHandSenID[ 0 ][ i ] )->ShowWindow( SW_HIDE );
				GetDlgItem( m_nHandSenID[ 1 ][ i ] )->ShowWindow( SW_HIDE );
			}
		}break;
	case eJogFeeder:
		{
			for (int i = 0; i < (int)m_vHandGui.size(); i++) {
				GetDlgItem(m_vHandGui[i])->ShowWindow(SW_HIDE);
			}
			for (int i = 0; i < (int)m_vTransfer.size(); i++) {
				GetDlgItem(m_vTransfer[i])->ShowWindow(SW_HIDE);
			}
			GetDlgItem(IDC_STATIC_SAFETY_SEN)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COLORCHECKBTCTRL_Z_SAFETY_SEN)->ShowWindow(SW_HIDE);
		}break;
	case eJogTransfer:
		{
			for(int i=0; i < (int)m_vHandGui.size(); i++){
				GetDlgItem(m_vHandGui[i])->ShowWindow(SW_HIDE);
			}
			for( int i = 0; i < ( int )m_vTransfer.size(); i++ ) {
				GetDlgItem( m_vTransfer[ i ] )->ShowWindow( SW_HIDE );
			}
			GetDlgItem(IDC_STATIC_SAFETY_SEN)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COLORCHECKBTCTRL_Z_SAFETY_SEN)->ShowWindow(SW_HIDE);
		}break;
	}
}

void CDlgManuMotorJog::InitCylinderUI()
{
	switch (m_nSelRobot)
	{
	case eJogPressDown1:
	case eJogPressDown2:
	case eJogPressDown3:
	case eJogPressDown4:
	case eJogPressDown5:
	case eJogPressDown6:
	case eJogPressDown7:
	case eJogPressDown8:
	{
		m_cBmpPress.LoadBitmap(IDB_PRESS_UNIT);
		m_btnCylinder.SetBitmap(m_cBmpPress);
	}break;
	case eJogTestPp1:
	{
		m_cBmpLoadTestPP1.LoadBitmap(IDB_TESTPP_01);
		m_btnCylinder.SetBitmap(m_cBmpLoadTestPP1);
	}break;
	case eJogTestPp2:
	{
		m_cBmpLoadTestPP2.LoadBitmap(IDB_TESTPP_02);
		m_btnCylinder.SetBitmap(m_cBmpLoadTestPP2);
	}break;
	case eJogLdTbl1:
	{
		m_cBmpLoadTestPP1.LoadBitmap(IDB_LOADTABLE_01);
		m_btnCylinder.SetBitmap(m_cBmpLoadTestPP1);
	}break;
	case eJogLdTbl2:
	{
		m_cBmpLoadTestPP1.LoadBitmap(IDB_LOADTABLE_02);
		m_btnCylinder.SetBitmap(m_cBmpLoadTestPP1);
	}break;

	case eJogLdTbl3:
	{
		m_cBmpLoadTestPP2.LoadBitmap(IDB_LOADTABLE_03);
		m_btnCylinder.SetBitmap(m_cBmpLoadTestPP2);
	}break;
	case eJogLdTbl4:
	{
		m_cBmpLoadTestPP2.LoadBitmap(IDB_LOADTABLE_04);
		m_btnCylinder.SetBitmap(m_cBmpLoadTestPP2);
	}break;
	case eJogTrayPp1_LD:
	case eJogTrayPp2_UD:
	{
		m_cBmpTrayPP.LoadBitmap(IDB_TRAYPP);
		m_btnCylinder.SetBitmap(m_cBmpTrayPP);
	}break;
	case eJogFeeder:
	{
		m_cBmpTrayFeeder.LoadBitmap(IDB_BITMAP_FEEDER);
		m_btnCylinder.SetBitmap(m_cBmpTrayFeeder);
	}break;
	case eJogTransfer:
	{
		m_cBmpTransfer.LoadBitmap(IDB_EV_TRANSFER);
		m_btnCylinder.SetBitmap(m_cBmpTransfer);
	}break;
	}
}

void CDlgManuMotorJog::InitUnitOperationUI()
{
	CString str;
	BOOL bStBtn = FALSE;
	switch (m_nSelRobot)
		{
		case eJogPressDown1:
		case eJogPressDown2:
		case eJogPressDown3:
		case eJogPressDown4:
		case eJogPressDown5:
		case eJogPressDown6:
		case eJogPressDown7:
		case eJogPressDown8:
		{	
			str.Format("Press #%d", m_nSelRobot + 1);
			GetDlgItem(IDC_BUTTON_ST_UNIT_OPERATION)->SetWindowText(str);
			m_btnSTOperation.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
			m_btnSTOperation.SetIcon(IDI_ICON_PRESS);
			m_btnSTOperation.SetAlign(CButtonST::ST_ALIGN_VERT);
			bStBtn = TRUE;
		}break;
		case eJogTestPp1:
		{
			m_cBmpTestPP1Axis.LoadBitmap(IDB_TESTPP_01);
			m_btnOperation.SetBitmap(m_cBmpTestPP1Axis);
		}
		break;
		case eJogLdTbl1:
		{
			m_cBmpLoadTable1Axis.LoadBitmap(IDB_LOADTABLE_01);
			m_btnOperation.SetBitmap(m_cBmpLoadTable1Axis);
		}break;
		case eJogTestPp2:
		{
			m_cBmpTestPP2Axis.LoadBitmap(IDB_TESTPP_02);
			m_btnOperation.SetBitmap(m_cBmpTestPP2Axis);
		}
		break;
		case eJogLdTbl2:
		{
			m_cBmpLoadTable2Axis.LoadBitmap(IDB_LOADTABLE_02);
			m_btnOperation.SetBitmap(m_cBmpLoadTable2Axis);
		}break;
		case eJogLdTbl3:
		{
			m_cBmpLoadTable3Axis.LoadBitmap(IDB_LOADTABLE_03);
			m_btnOperation.SetBitmap(m_cBmpLoadTable3Axis);
		}break;
		case eJogLdTbl4:
		{
			m_cBmpLoadTable4Axis.LoadBitmap(IDB_LOADTABLE_04);
			m_btnOperation.SetBitmap(m_cBmpLoadTable4Axis);
		}break;
		case eJogTrayPp1_LD:
		case eJogTrayPp2_UD:
		{
			m_cBmpTrayPPAxis.LoadBitmap(IDB_TRAYPP);
			m_btnOperation.SetBitmap(m_cBmpTrayPPAxis);

		}break;
		case eJogFeeder:
		{
			m_cBmpTrayFeederAxis.LoadBitmap(IDB_BITMAP_FEEDER);
			m_btnOperation.SetBitmap(m_cBmpTrayFeederAxis);
		}break;
		case eJogTransfer:
		{
			m_cBmpTransferAxis.LoadBitmap(IDB_TRANSFER);
			m_btnOperation.SetBitmap(m_cBmpTransferAxis);
		}break;
	}

	if (bStBtn)
	{
		GetDlgItem(IDC_BUTTON_ST_UNIT_OPERATION)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_UNIT_OPERATION)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_UNIT_OPERATION)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_ST_UNIT_OPERATION)->ShowWindow(SW_HIDE);
	}
}

void CDlgManuMotorJog::OnBnClickedBtnCylinderManual()
{
	switch (m_nSelRobot)
	{
	case eJogPressDown1:
	case eJogPressDown2:
	case eJogPressDown3:
	case eJogPressDown4:
	case eJogPressDown5:
	case eJogPressDown6:
	case eJogPressDown7:
	case eJogPressDown8:
	{
		CDlgManuPressUnit dlg;
		dlg.DoModal();
	}break;
	case eJogTestPp1:
	{
		g_DMCont.m_dmEQP.SB(BDM0_OPEN_EJECTOR_DLG_HISTORY, TRUE);//jira 403
		CDlgManuTestPP dlg;
		dlg.m_nHandIdx = eTestPP_1;
		dlg.DoModal();
		g_DMCont.m_dmEQP.SB(BDM0_OPEN_EJECTOR_DLG_HISTORY, FALSE);//jira 403
	}break;
	case eJogTestPp2:
	{
		g_DMCont.m_dmEQP.SB(BDM0_OPEN_EJECTOR_DLG_HISTORY, TRUE);//jira 403
		CDlgManuTestPP dlg;
		dlg.m_nHandIdx = eTestPP_2;
		dlg.DoModal();
		g_DMCont.m_dmEQP.SB(BDM0_OPEN_EJECTOR_DLG_HISTORY, FALSE);//jira 403
	}break;
	case eJogLdTbl1:
	case eJogLdTbl2:
	case eJogLdTbl3:
	case eJogLdTbl4:
	{
		CDlgManuLoadTbl dlg;
		dlg.DoModal();
	}break;
	case eJogTrayPp1_LD:
	case eJogTrayPp2_UD:
	{
		g_DMCont.m_dmEQP.SB(BDM0_OPEN_EJECTOR_DLG_HISTORY, TRUE);//jira 403
		CDlgManuTrayPP dlg;
		dlg.m_nHandIdx = m_nSelRobot - eJogTrayPp1_LD;
		dlg.DoModal();
		g_DMCont.m_dmEQP.SB(BDM0_OPEN_EJECTOR_DLG_HISTORY, FALSE);//jira 403
	}break;
	case eJogFeeder:
	{
		CDlgManuFeeder dlg;
		dlg.DoModal();
	}break;
	case eJogTransfer:
	{
		CDlgManuTransfer dlg;
		dlg.DoModal();
	}break;
	}
}

void CDlgManuMotorJog::OnBnClickedBtnUnitOperation()
{
	switch (m_nSelRobot)
	{
	case eJogPressDown1:
	{
		OnBnClickedBtnAixsManuPress(ePressUnit_Down_1);
	}
	break;
	case eJogPressDown2:
	{
		OnBnClickedBtnAixsManuPress(ePressUnit_Down_2);
	}
	break;
	case eJogPressDown3:
	{
		OnBnClickedBtnAixsManuPress(ePressUnit_Down_3);
	}
	break;
	case eJogPressDown4:
	{
		OnBnClickedBtnAixsManuPress(ePressUnit_Down_4);
	}
	break;
	case eJogPressDown5:
	{
		OnBnClickedBtnAixsManuPress(ePressUnit_Down_5);
	}
	break;
	case eJogPressDown6:
	{
		OnBnClickedBtnAixsManuPress(ePressUnit_Down_6);
	}
	break;
	case eJogPressDown7:
	{
		OnBnClickedBtnAixsManuPress(ePressUnit_Down_7);
	}
	break;
	case eJogPressDown8:
	{
		OnBnClickedBtnAixsManuPress(ePressUnit_Down_8);
	}
	break;
	case eJogTestPp1:
	{
		OnBnClickedBtnAixsManuTestPp(eTestPP_1);
	}break;
	case eJogTestPp2:
	{
		OnBnClickedBtnAixsManuTestPp(eTestPP_2);
	}break;
	case eJogLdTbl1:
	{
		OnBnClickedBtnAixsManuLoadTbl(eLoadTbl_1);
	}break;
	case eJogLdTbl2:
	{
		OnBnClickedBtnAixsManuLoadTbl(eLoadTbl_2);
	}break;
	case eJogLdTbl3:
	{
		OnBnClickedBtnAixsManuLoadTbl(eLoadTbl_3);
	}break;
	case eJogLdTbl4:
	{
		OnBnClickedBtnAixsManuLoadTbl(eLoadTbl_4);
	}break;
	case eJogTrayPp1_LD:
	case eJogTrayPp2_UD:
	{
		int nIdx = m_nSelRobot - eJogTrayPp1_LD;
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

		CDlgCommand_TrayPP CmdDlg;
		//MakeLog("Tray PP Dialog Show");
		if (IDOK == CmdDlg.DoModal())
		{
			CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
			int nSelectedCmd = CmdDlg.GetSelectedSeqCmdId();
			switch (nSelectedCmd)
			{
			case eSeqCmd_TrayPP_Initial:
			{
				CBaseSeqCmd* pSeqCmd = nullptr;
				if( m_nSelRobot == eJogTrayPp1_LD) pSeqCmd = new CTrayLoadPPCmd_Initial;
				else                               pSeqCmd = new CTrayUnloadPPCmd_Initial;
				int nErr = g_TaskSystemCtrl.SendCommand(*pTaskTrayPP[nIdx], pSeqCmd, eEqpCtrlMode_Manual);
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
				dlg.m_nStartPoint = (nIdx == eTrayPP_1_LD)? eMANU_OPER_TRAY_PP1_LD : eMANU_OPER_TRAY_PP2_UD;
				dlg.m_nSelectAction = eDlgActionOnlyMove;
				dlg.DoModal();
			}break;
			case eSeqCmd_TrayPP_MovePickPlace:
			{
				CDlgAxisActionMove dlg;
				dlg.m_nStartPoint = (nIdx == eTrayPP_1_LD) ? eMANU_OPER_TRAY_PP1_LD : eMANU_OPER_TRAY_PP2_UD;
				dlg.m_nSelectAction = eDlgActionPick;
				dlg.DoModal();
			}break;
			}
		}
	}break;
	case eJogFeeder:
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

		int nInterlockErr = g_TaskSystemCtrl.CheckManualable(TRUE);
		if (nInterlockErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
			return;
		}

		MakeLog("Tray Feeder Dialog Show");
		CDlgCommand_Feeder CmdDlg;
		if (IDOK == CmdDlg.DoModal())
		{
			int nSelectedCmd = CmdDlg.GetSelectedSeqCmdId();
			switch (nSelectedCmd)
			{
			case eSeqCmd_Feeder_Initial:
			{
				CTrayFeederCmd_Initial* pSeqCmd = new CTrayFeederCmd_Initial;
				int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTrayFeeder, pSeqCmd, eEqpCtrlMode_Manual);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					g_Error.AlarmReport(nErr, AlarmData, NULL);
				}
				else {
					MakeLog("Tray Feeder Initial");
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
	}break;
	case eJogTransfer:
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

		int nInterlockErr = g_TaskSystemCtrl.CheckManualable(TRUE);
		if (nInterlockErr != ERR_NO_ERROR) {
			CParamList AlarmData;
			g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
			return;
		}

		MakeLog("Transfer Dialog Show");
		CDlgCommand_Transfer CmdDlg;
		if (IDOK == CmdDlg.DoModal())
		{
			int nSelectedCmd = CmdDlg.GetSelectedSeqCmdId();
			switch (nSelectedCmd)
			{
			case eSeqCmd_Transfer_Initial:
			{
				CTransferCmd_Initial* pSeqCmd = new CTransferCmd_Initial;
				int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTransfer, pSeqCmd, eEqpCtrlMode_Manual);
				if (nErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					g_Error.AlarmReport(nErr, AlarmData, NULL);
				}
				else {
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
	}break;
	}
}


void CDlgManuMotorJog::OnTimer(UINT_PTR nIDEvent)
{
	CString strCurPos = _T("");
	double dCurPos = 0.0;
	CDSSPanel* chkbt  = NULL;

	if(nIDEvent == 0){
		KillTimer(nIDEvent);

		if(m_pMotorX != NULL){
			m_pMotorX->GetCurrentPos(dCurPos);
			strCurPos.Format("%5.3f", dCurPos /1000.0);
			SetDlgItemText(IDC_X_POS, strCurPos);
			UpdateSenValue( IDC_ORIGIN_X, m_pMotorX->IsHomeSensorOn());
		}
		if(m_pMotorY != NULL){
			m_pMotorY->GetCurrentPos(dCurPos);
			strCurPos.Format("%5.3f", dCurPos /1000.0);
			SetDlgItemText(IDC_Y_POS, strCurPos);
			UpdateSenValue( IDC_ORIGIN_Y, m_pMotorY->IsHomeSensorOn());
		}
		if(m_pMotorZ != NULL){
//			if( m_nContactForceAxisNo >= 0 ){
//				double dTorq = g_MotorDriverComIF.get_torque(static_cast<eAxisNo>(m_nContactForceAxisNo));

				int nIdx = m_pMotorZ->GetAxisNo();

				if ((eAXIS_PRESS_UNIT_DN1 <= nIdx && nIdx <= eAXIS_PRESS_UNIT_DN4) || (eAXIS_PRESS_UNIT_DN5 <= nIdx && nIdx <= eAXIS_PRESS_UNIT_DN8))
				{
					double dTorq = m_pMotorZ->GetReadActualTorque_PDO(nIdx);
					CString msg;
					msg.Format("%.2f", dTorq);
					//				msg.Format("%.2f", dTorq / 10.0);
					(GetDlgItem(IDC_Z_FORCE))->SetWindowText(LPCTSTR(msg));
					m_pMotorZ->GetCurrentPos(dCurPos);
				}
				else
				{
					int nBBxIdx = 0;
// 					switch (nIdx)
// 					{
						if (eAXIS_TRAY_PP_1_Z	== nIdx)	nBBxIdx = eBMAxis_LOAD_TRAY_Picker01;	
						if (eAXIS_TEST_PP_1_Z	== nIdx)	nBBxIdx = eBMAxis_LOAD_TESTPP_1_Picker08;	
						if (eAXIS_TEST_PP_1_Z2 == nIdx)nBBxIdx = eBMAxis_UNLOAD_TESTPP_1_Picker08;
						if (eAXIS_TRAY_PP_2_Z == nIdx)	nBBxIdx = eBMAxis_UNLOAD_TRAY_Picker01;
						if (eAXIS_TEST_PP_2_Z == nIdx)nBBxIdx = eBMAxis_LOAD_TESTPP_2_Picker08;
						if (eAXIS_TEST_PP_2_Z2 == nIdx)nBBxIdx = eBMAxis_UNLOAD_TESTPP_2_Picker08;
// 					default:
// 						break;
// 					}

					g_Motor[nIdx]->GetCurrentPos(dCurPos, _eBbxmnpAxis(nBBxIdx));

					//m_pMotorZ->GetCurrenPosAvg(dCurPos);
				}
//			}

			///////// 원본
			
			
			/////////
			strCurPos.Format("%5.3f", dCurPos/1000.0);
			SetDlgItemText(IDC_Z_POS, strCurPos);
			UpdateSenValue( IDC_ORIGIN_Z, m_pMotorZ->IsHomeSensorOn());
		}

		if (m_pMotorR != NULL) {
			m_pMotorR->GetCurrentPos(dCurPos);		
			strCurPos.Format("%5.3f", dCurPos / 1000.0);
			SetDlgItemText(IDC_R_POS, strCurPos);
			UpdateSenValue(IDC_ORIGIN_R, m_pMotorR->IsHomeSensorOn());
		}

		switch(m_nSelRobot)
		{
// 		case eJogPressUp1   :
// 		case eJogPressUp2   : 
// 		case eJogPressUp3   : 
// 		case eJogPressUp4   : 
// 		case eJogPressUp5   : 
// 		case eJogPressUp6   : 
// 		case eJogPressUp7   : 
// 		case eJogPressUp8   :
// 			{
// 				int nDiId = g_TaskPressUnit[m_nSelRobot+eJogPressDown1].m_vX[CTaskPressUnit::xZAXIS_SAFETY_SEN];
// 				UpdateSensorSts( IDC_COLORCHECKBTCTRL_Z_SAFETY_SEN, nDiId );
// 			}break;
		case eJogPressDown1 : 
		case eJogPressDown2 : 
		case eJogPressDown3 : 
		case eJogPressDown4 : 
		case eJogPressDown5 : 
		case eJogPressDown6 : 
		case eJogPressDown7 : 
		case eJogPressDown8 :
			{	
				int nDiId = g_TaskPressUnit[m_nSelRobot-eJogPressDown1].m_vX[CTaskPressUnit::xZAXIS_SAFETY_SEN];
				UpdateSensorSts( IDC_COLORCHECKBTCTRL_Z_SAFETY_SEN, nDiId );
			}break;
		case eJogTestPp1 :
		case eJogTestPp2 :
			{
				for(int nY=0; nY<MAX_TEST_PP_PKR_CNT_Y; nY++){
					for(int nX=0; nX<MAX_TEST_PP_PKR_CNT_X; nX++){
						UpdateSenValue(m_nHandSenID[nY][nX], g_TaskTestPP[ m_nSelRobot - eJogTestPp1 ].GetVacuum(m_nTestPPHandType, CPoint(nX,nY)));
					}
				}
				/*UpdateSensorSts( IDC_COLORCHECKBTCTRL_Z_SAFETY_SEN, g_TaskTestPP[m_nSelRobot-eJogTestPp1].m_vX[CTaskTestPP::xZ1AXIS_SAFETY_SEN_BC]);*/
			}break;
		case eJogTrayPp1_LD :
		case eJogTrayPp2_UD:
			{
				int nIdx = m_nSelRobot - eJogTrayPp1_LD;
				CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
				for(int nY=0; nY<MAX_TRAY_PP_CNT_Y; nY++){
					for(int nX=0; nX<MAX_TRAY_PP_CNT_X; nX++){
						UpdateSenValue(m_nHandSenID[nY][nX], pTaskTrayPP[nIdx]->GetVacuum(CPoint(nX, nY)));
					}
				}
			}break;
		case eJogTransfer:
			{
			}break;
		}
		SetTimer(nIDEvent, DEF_SENSOR_DISP_TM, NULL);
	}

	// motor x plus
	if (nIDEvent == 10)
	{
		KillTimer(nIDEvent);
		if (m_pMotorZ != NULL)
		{
			int nRet = m_pMotorZ->V_MoveCheckInterlock(100, 1);
			if (nRet != ERR_NO_ERROR)
				m_pMotorZ->V_MoveStop();
		}
		
		SetTimer(nIDEvent, 50, NULL);
	}
	// motor x minus
	if (nIDEvent == 11)
	{
		KillTimer(nIDEvent);
		if (m_pMotorZ != NULL)
		{
			int nRet = m_pMotorZ->V_MoveCheckInterlock(-100, 1);
			if (nRet != ERR_NO_ERROR)
				m_pMotorZ->V_MoveStop();
		}
		SetTimer(nIDEvent, 50, NULL);
	}
	// motor y plus
	if (nIDEvent == 20)
	{
		KillTimer(nIDEvent);
		if (m_pMotorY != NULL)
		{
			int nRet = m_pMotorY->V_MoveCheckInterlock(100, 1);
			if (nRet != ERR_NO_ERROR)
				m_pMotorY->V_MoveStop();
		}
		SetTimer(nIDEvent, 50, NULL);
	}
	// motor y minus
	if (nIDEvent == 21)
	{
		KillTimer(nIDEvent);
		if (m_pMotorY != NULL)
		{
			int nRet = m_pMotorY->V_MoveCheckInterlock(-100, 1);
			if (nRet != ERR_NO_ERROR)
				m_pMotorY->V_MoveStop();
		}
		SetTimer(nIDEvent, 50, NULL);
	}
	// motor z plus
	if (nIDEvent == 30)
	{
		KillTimer(nIDEvent);
		if (m_pMotorZ != NULL)
		{
			int nRet = m_pMotorZ->V_MoveCheckInterlock(100, 1);
			if (nRet != ERR_NO_ERROR)
				m_pMotorZ->V_MoveStop();
		}
		SetTimer(nIDEvent, 50, NULL);
	}
	// motor z minus
	if (nIDEvent == 31)
	{
		KillTimer(nIDEvent);
		if (m_pMotorZ != NULL)
		{
			int nRet = m_pMotorZ->V_MoveCheckInterlock(-100, 1);
			if (nRet != ERR_NO_ERROR)
				m_pMotorZ->V_MoveStop();
		}
		SetTimer(nIDEvent, 50, NULL);
	}

	// motor R Right
	if (nIDEvent == 32)
	{
		KillTimer(nIDEvent);
		if (m_pMotorR != NULL)
		{
			int nRet = m_pMotorR->V_MoveCheckInterlock(100, 1);
			if (nRet != ERR_NO_ERROR)
				m_pMotorR->V_MoveStop();
		}
		SetTimer(nIDEvent, 50, NULL);
	}
	// motor z minus
	if (nIDEvent == 33)
	{
		KillTimer(nIDEvent);
		if (m_pMotorR != NULL)
		{
			int nRet = m_pMotorR->V_MoveCheckInterlock(-100, 1);
			if (nRet != ERR_NO_ERROR)
				m_pMotorR->V_MoveStop();
		}
		SetTimer(nIDEvent, 50, NULL);
	}

	if (nIDEvent == 100)
	{
		BOOL bBusySts = FALSE;
		BOOL bAlarmSts = FALSE;

		int nCntBusy = 0;
		int nCntAlarm = 0;


		if (g_TaskTransfer.GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			bBusySts = TRUE;
		}
		else if (g_TaskTransfer.GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
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

		for (int nIdx = 0; nIdx < eMaxStackerCount; nIdx++) {
			if (g_TaskStacker[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				nCntBusy++;
			}
			else if (g_TaskStacker[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
				nCntAlarm++;
			}
		}

		for (int nIdx = 0; nIdx < eMaxPressUnitCount; nIdx++) {
			if (g_TaskPressUnit[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				nCntBusy++;
			}
			else if (g_TaskPressUnit[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
				nCntAlarm++;
			}
		}

		for (int nIdx = 0; nIdx < eMaxTestPPCount; nIdx++) {
			if (g_TaskTestPP[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				nCntBusy++;
			}
			else if (g_TaskTestPP[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
				nCntAlarm++;
			}
		}

		for (int nIdx = 0; nIdx < eMaxLoadTblCount; nIdx++) {
			if (g_TaskLoadTbl[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
				nCntBusy++;
			}
			else if (g_TaskLoadTbl[nIdx].GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
				nCntAlarm++;
			}
		}

		if (g_MulitOrg.GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
			nCntBusy++;
		}
		else if (g_MulitOrg.GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
			nCntAlarm++;
		}

		if (nCntBusy > 0) {
			bBusySts = TRUE;
		}
		if (nCntAlarm > 0) {
			bAlarmSts = TRUE;
		}

		CString strCurPos;

		CDSSPanel*    pSensor = NULL;

		KillTimer(100);

		pSensor = (CDSSPanel*)GetDlgItem(IDC_AXIS_CMD_BUSY);
		if (bBusySts) {
			pSensor->On();
			m_bManualCmdExcute = TRUE;
		}
		else {
			pSensor->Off();
			m_bManualCmdExcute = FALSE;
		}


			pSensor = (CDSSPanel*)GetDlgItem(IDC_AXIS_CMD_ERROR);
			if (bAlarmSts) {
				pSensor->On();
			}
			else {
				pSensor->Off();
			}
		SetTimer(100, DEF_SENSOR_DISP_TM, NULL);		
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgManuMotorJog::UpdateSensorSts(int nCtrlId, int nDiId)
{
	CDSSPanel*    pSensor = NULL;
	pSensor = (CDSSPanel*)GetDlgItem(nCtrlId);
	BOOL bVal = g_IO.in(nDiId);
	if( bVal ) pSensor->On();
	else       pSensor->Off();
}

void CDlgManuMotorJog::UpdateSenValue(int nCtrlId, int nVal)
{
	CDSSPanel*    pSensor = NULL;
	pSensor = (CDSSPanel*)GetDlgItem(nCtrlId);
	if( nVal ) pSensor->On();
	else       pSensor->Off();
}

BEGIN_EVENTSINK_MAP(CDlgManuMotorJog, CDialogEx)
	ON_EVENT(CDlgManuMotorJog, IDC_BTN_CLOSE, 22, CDlgManuMotorJog::ClickBtnClose, VTS_NONE)
	ON_EVENT(CDlgManuMotorJog, IDC_CBUTTON_ALL_VACUUM, 22, CDlgManuMotorJog::ClickCbuttonAllVacuum, VTS_NONE)
	ON_EVENT(CDlgManuMotorJog, IDC_CBUTTON_ALL_BLOW, 22, CDlgManuMotorJog::ClickCbuttonAllBlow, VTS_NONE)
	ON_EVENT(CDlgManuMotorJog, IDC_BTN_SERVO_FREE, 22, CDlgManuMotorJog::ClickBtnServoFree, VTS_NONE)
	ON_EVENT(CDlgManuMotorJog, IDC_COLORBTCTRL_ZDOWN, DISPID_MOUSEDOWN, CDlgManuMotorJog::MouseDownColorbtctrlZdown, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuMotorJog, IDC_COLORBTCTRL_ZDOWN, DISPID_MOUSEUP, CDlgManuMotorJog::MouseUpColorbtctrlZdown, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuMotorJog, IDC_COLORBTCTRL_ZUP, DISPID_MOUSEDOWN, CDlgManuMotorJog::MouseDownColorbtctrlZup, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuMotorJog, IDC_COLORBTCTRL_ZUP, DISPID_MOUSEUP, CDlgManuMotorJog::MouseUpColorbtctrlZup, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuMotorJog, IDC_COLORBTCTRL_RR, DISPID_MOUSEDOWN, CDlgManuMotorJog::MouseDownColorbtctrlRPlus, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuMotorJog, IDC_COLORBTCTRL_RR, DISPID_MOUSEUP, CDlgManuMotorJog::MouseUpColorbtctrlRPlus, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuMotorJog, IDC_COLORBTCTRL_LR, DISPID_MOUSEDOWN, CDlgManuMotorJog::MouseDownColorbtctrlRMinus, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuMotorJog, IDC_COLORBTCTRL_LR, DISPID_MOUSEUP, CDlgManuMotorJog::MouseUpColorbtctrlRMinus, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuMotorJog, DEVICESETDLL_IDC_COLORBTCTRL_UP, DISPID_MOUSEDOWN, CDlgManuMotorJog::MouseDownIdcColorbtctrlUp, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuMotorJog, DEVICESETDLL_IDC_COLORBTCTRL_UP, DISPID_MOUSEUP, CDlgManuMotorJog::MouseUpIdcColorbtctrlUp, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuMotorJog, DEVICESETDLL_IDC_COLORBTCTRL_DOWN, DISPID_MOUSEDOWN, CDlgManuMotorJog::MouseDownIdcColorbtctrlDown, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuMotorJog, DEVICESETDLL_IDC_COLORBTCTRL_DOWN, DISPID_MOUSEUP, CDlgManuMotorJog::MouseUpIdcColorbtctrlDown, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuMotorJog, DEVICESETDLL_IDC_COLORBTCTRL_LEFT, DISPID_MOUSEDOWN, CDlgManuMotorJog::MouseDownIdcColorbtctrlLeft, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuMotorJog, DEVICESETDLL_IDC_COLORBTCTRL_LEFT, DISPID_MOUSEUP, CDlgManuMotorJog::MouseUpIdcColorbtctrlLeft, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuMotorJog, DEVICESETDLL_IDC_COLORBTCTRL_RIGHT, DISPID_MOUSEDOWN, CDlgManuMotorJog::MouseDownIdcColorbtctrlRight, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuMotorJog, DEVICESETDLL_IDC_COLORBTCTRL_RIGHT, DISPID_MOUSEUP, CDlgManuMotorJog::MouseUpIdcColorbtctrlRight, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


void CDlgManuMotorJog::ClickBtnClose()
{
	if (m_bManualCmdExcute) {
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	CDialogEx::OnCancel();
}

void CDlgManuMotorJog::ClickCbuttonAllVacuum()
{
	switch(m_nSelRobot)
	{
	case eJogTestPp1 :
	case eJogTestPp2 :
		{
			g_TaskTestPP[m_nSelRobot-eJogTestPp1].do_VacBlow_All(eVacBlwSt_VacOn,m_nTestPPHandType);
		}break;
	case eJogTrayPp1_LD :
	case eJogTrayPp2_UD :
		{
			CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
			pTaskTrayPP[m_nSelRobot - eJogTrayPp1_LD]->do_VacBlow_All(eVacBlwSt_VacOn);
		}break;
	}
}


void CDlgManuMotorJog::ClickCbuttonAllBlow()
{
	switch(m_nSelRobot)
	{
	case eJogTestPp1 :
	case eJogTestPp2 :
		{
			g_TaskTestPP[m_nSelRobot-eJogTestPp1].do_VacBlow_All(eVacBlwSt_BlwOn, m_nTestPPHandType);
		}break;
	case eJogTrayPp1_LD:
	case eJogTrayPp2_UD:
		{
			CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
			pTaskTrayPP[m_nSelRobot - eJogTrayPp1_LD]->do_VacBlow_All(eVacBlwSt_BlwOn);
		}break;
	}
}


void CDlgManuMotorJog::ClickBtnServoFree()
{
	if(m_bServoStat){
		if(m_pMotorX != NULL){
			m_pMotorX->ServoOff();
		}
		if(m_pMotorY != NULL){
			m_pMotorY->ServoOff();
		}
// 		if(m_pMotorZ != NULL){
// 			m_pMotorZ->ServoOff();
// 		}
		if (m_pMotorR != NULL) {
			m_pMotorR->ServoOff();
		}

		m_bServoStat = DEF_OFF;
		m_btnServoFree.SetCaption("Servo Off");
	}else{
		if(m_pMotorX != NULL){
			m_pMotorX->ServoOn();
		}
		if(m_pMotorY != NULL){
			m_pMotorY->ServoOn();
		}
// 		if(m_pMotorZ != NULL){
// 			m_pMotorZ->ServoOn();
// 		}
		if (m_pMotorR != NULL) {
			m_pMotorR->ServoOn();
		}

		m_bServoStat = DEF_ON;
		m_btnServoFree.SetCaption("Servo On");
	}

	
}

// Z Down
void CDlgManuMotorJog::MouseDownColorbtctrlZdown(short Button, short Shift, long X, long Y)
{ 
	int nErr = ComInterlock(eJogMotor_Z);

	if(m_pMotorZ == NULL) return;

	if(nErr != ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, NULL);
		return;
	}

	UpdateData();
	SetTimer(30, 50, NULL);

	int nPicker[eBMAxis_Picker_MAX] = { 0, };
	//eAXIS_TRAY_PP_1_Z
//	int nAxisNo = FindAxisNum(m_nSelRobot);

	int nAxisNo = m_pMotorZ->GetAxisNo();
	if (eAXIS_TRAY_PP_1_Z <= nAxisNo  && nAxisNo <= eAXIS_TEST_PP_2_Z2)
	{
		int eMotor = g_pBbxmnpClient->GetZPickerMotorNumber(nAxisNo);
		int nCount = 0;
		for (int nIdx : g_pBbxmnpClient->m_vHandAxis[eMotor])
		{
			nPicker[nCount] = g_pBbxmnpClient->m_vHandMatrix[nIdx];
			nCount++;
		}
		g_Motor[nAxisNo]->V_Move(eJogDir_Positive, MAX_PICKER_CNT_EACH_SLAVE, &nPicker[0], m_vZSpeed[m_nJogSpeed], 20);
		g_Motor[nAxisNo]->V_Move(eJogDir_Positive, MAX_PICKER_CNT_EACH_SLAVE, &nPicker[4], m_vZSpeed[m_nJogSpeed], 20);
	}
	else
	{
		UpdateData();
		SetTimer(11, 50, NULL);
		g_TaskSystemCtrl.MoveJog(m_pMotorZ->GetAxisLogicNo(), 1, m_vSpeed[m_nJogSpeed]);
		//g_TaskSystemCtrl.MoveJog(m_pMotorZ->GetAxisLogicNo(), 1, m_vZSpeed[m_nJogSpeed]);
		//m_pMotorZ->V_Move(1, m_vSpeed[m_nJogSpeed]);
	}
	

}
void CDlgManuMotorJog::MouseUpColorbtctrlZdown(short Button, short Shift, long X, long Y)
{
	if(m_pMotorZ == NULL) return;
	KillTimer(30);
	m_pMotorZ->V_MoveStop();
}

// Z Up
void CDlgManuMotorJog::MouseDownColorbtctrlZup(short Button, short Shift, long X, long Y)
{
	int nErr = ComInterlock(eJogMotor_Z);

	if(m_pMotorZ == NULL) return;
	
	if(nErr != ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, NULL);
		return;
	}
	
	UpdateData();
	SetTimer(31, 50, NULL);

	int nAxisNo = m_pMotorZ->GetAxisNo();
	if (eAXIS_TRAY_PP_1_Z <= nAxisNo  && nAxisNo <= eAXIS_TEST_PP_2_Z2)
	{
		int nPicker[eBMAxis_Picker_MAX] = { 0, };
		//eAXIS_TRAY_PP_1_Z		
		int eMotor = g_pBbxmnpClient->GetZPickerMotorNumber(nAxisNo);
		int nCount = 0;
		for (int nIdx : g_pBbxmnpClient->m_vHandAxis[eMotor])
		{
			nPicker[nCount] = g_pBbxmnpClient->m_vHandMatrix[nIdx];
			nCount++;
		}
		g_Motor[nAxisNo]->V_Move(eJogDir_Negative, MAX_PICKER_CNT_EACH_SLAVE, &nPicker[0], m_vZSpeed[m_nJogSpeed], 20);
		g_Motor[nAxisNo]->V_Move(eJogDir_Negative, MAX_PICKER_CNT_EACH_SLAVE, &nPicker[4], m_vZSpeed[m_nJogSpeed], 20);
	}
	else {
		UpdateData();
		SetTimer(11, 50, NULL);
		g_TaskSystemCtrl.MoveJog(m_pMotorZ->GetAxisLogicNo(), -1, m_vSpeed[m_nJogSpeed]);
	}

//	g_TaskSystemCtrl.MoveJog(m_pMotorZ->GetAxisLogicNo(), -1, m_vZSpeed[m_nJogSpeed]);
	//m_pMotorZ->V_Move(-1, m_vSpeed[m_nJogSpeed], );
}
void CDlgManuMotorJog::MouseUpColorbtctrlZup(short Button, short Shift, long X, long Y)
{
	if(m_pMotorZ == NULL) return;
	KillTimer(31);
	m_pMotorZ->V_MoveStop();
}

// R Rotate +
void CDlgManuMotorJog::MouseDownColorbtctrlRPlus(short Button, short Shift, long X, long Y)
{
	int nErr = ComInterlock(eJogMotor_R);

	if (m_pMotorR == NULL) return;

	if (nErr != ERR_NO_ERROR) {
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, NULL);
		return;
	}

	UpdateData();
	SetTimer(32, 50, NULL);
	g_TaskSystemCtrl.MoveJog(m_pMotorR->GetAxisLogicNo(), 1, m_vRotateSpeed[m_nJogSpeed]);
	//m_pMotorR->V_Move(1, m_vSpeed[m_nJogSpeed], 50);
}
void CDlgManuMotorJog::MouseUpColorbtctrlRPlus(short Button, short Shift, long X, long Y)
{
	if (m_pMotorR == NULL) return;
	KillTimer(32);
	m_pMotorR->V_MoveStop();
}

// R Rotate -
void CDlgManuMotorJog::MouseDownColorbtctrlRMinus(short Button, short Shift, long X, long Y)
{
	int nErr = ComInterlock(eJogMotor_R);

	if (m_pMotorR == NULL) return;

	if (nErr != ERR_NO_ERROR) {
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, NULL);
		return;
	}

	UpdateData();
	SetTimer(33, 50, NULL);
	g_TaskSystemCtrl.MoveJog(m_pMotorR->GetAxisLogicNo(), -1, m_vRotateSpeed[m_nJogSpeed]);
	//m_pMotorR->V_Move(-1, m_vSpeed[m_nJogSpeed], 50);
}
void CDlgManuMotorJog::MouseUpColorbtctrlRMinus(short Button, short Shift, long X, long Y)
{
	if (m_pMotorR == NULL) return;
	KillTimer(33);
	m_pMotorR->V_MoveStop();
}

// Y Rear
void CDlgManuMotorJog::MouseDownIdcColorbtctrlUp(short Button, short Shift, long X, long Y)
{
	int nErr = ComInterlock(eJogMotor_Y);

	if(m_pMotorY == NULL) return;
	
	if(nErr != ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, NULL);
		return;
	}

	UpdateData();
	SetTimer(20, 50, NULL);
	g_TaskSystemCtrl.MoveJog(m_pMotorY->GetAxisLogicNo(), 1, m_vSpeed[m_nJogSpeed]);
	//m_pMotorY->V_Move(1, m_vSpeed[m_nJogSpeed], 50);
}
void CDlgManuMotorJog::MouseUpIdcColorbtctrlUp(short Button, short Shift, long X, long Y)
{
	if(m_pMotorY == NULL) return;
	KillTimer(20);
	m_pMotorY->V_MoveStop();
}

// Y Front
void CDlgManuMotorJog::MouseDownIdcColorbtctrlDown(short Button, short Shift, long X, long Y)
{
	int nErr = ComInterlock(eJogMotor_Y);

	if(m_pMotorY == NULL) return;
	
	if(nErr != ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, NULL);
		return;
	}

	UpdateData();
	SetTimer(21, 50, NULL);
	g_TaskSystemCtrl.MoveJog(m_pMotorY->GetAxisLogicNo(), -1, m_vSpeed[m_nJogSpeed]);
	//m_pMotorY->V_Move(-1, m_vSpeed[m_nJogSpeed], 50);
}
void CDlgManuMotorJog::MouseUpIdcColorbtctrlDown(short Button, short Shift, long X, long Y)
{
	if(m_pMotorY == NULL) return;
	KillTimer(21);
	m_pMotorY->V_MoveStop();
}

// X Left
void CDlgManuMotorJog::MouseDownIdcColorbtctrlLeft(short Button, short Shift, long X, long Y)
{
	int nErr = ComInterlock(eJogMotor_X);

	if(m_pMotorX == NULL) return;
	
	if(nErr != ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, NULL);
		return;
	}

	UpdateData();
	SetTimer(11, 50, NULL);
	g_TaskSystemCtrl.MoveJog(m_pMotorX->GetAxisLogicNo(), -1, m_vSpeed[m_nJogSpeed]);
	//m_pMotorX->V_Move(-1, m_vSpeed[m_nJogSpeed], 50);
}
void CDlgManuMotorJog::MouseUpIdcColorbtctrlLeft(short Button, short Shift, long X, long Y)
{
	if(m_pMotorX == NULL) return;
	KillTimer(11);
	m_pMotorX->V_MoveStop();
}

// X Right
void CDlgManuMotorJog::MouseDownIdcColorbtctrlRight(short Button, short Shift, long X, long Y)
{
	int nErr = ComInterlock(eJogMotor_X);

	if(m_pMotorX == NULL) return;
	
	if(nErr != ERR_NO_ERROR){
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, NULL);
		return;
	}

	UpdateData();
	SetTimer(10, 50, NULL);
	g_TaskSystemCtrl.MoveJog(m_pMotorX->GetAxisLogicNo(), 1, m_vSpeed[m_nJogSpeed]);
	//m_pMotorX->V_Move(1, m_vSpeed[m_nJogSpeed], 50);
}
void CDlgManuMotorJog::MouseUpIdcColorbtctrlRight(short Button, short Shift, long X, long Y)
{
	if(m_pMotorX == NULL) return;

	KillTimer(10);
	m_pMotorX->V_MoveStop();
}


void CDlgManuMotorJog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	switch(nState)
	{
	case WA_INACTIVE         : //(0) 포커스를 잃었을 때
		{
			if(m_pMotorX != NULL){
				m_pMotorX->V_MoveStop();
			}
			if(m_pMotorY != NULL){
				m_pMotorY->V_MoveStop();
			}
			if(m_pMotorZ != NULL){
				m_pMotorZ->V_MoveStop();
			}
			if (m_pMotorR != NULL) {
				m_pMotorR->V_MoveStop();
			}

			KillTimer(10);
			KillTimer(11);
			KillTimer(20);
			KillTimer(21);
			KillTimer(30);
			KillTimer(31);
		}break;
	}
}

int CDlgManuMotorJog::ComInterlock(int nAxis)
{
	switch (nAxis)
	{
	case eJogMotor_X:
		{
			if (m_pMotorX != NULL) {
				int nOrg = m_pMotorX->IsOrgCompleted(nullptr);
				if (nOrg == FALSE) {
					m_pMotorX->V_MoveStop();
					return ERR_MOT_INTERLOCK_IS_NOT_ORG;
				}
					
			}
		}break;
	case eJogMotor_Y:
		{
			if (m_pMotorY != NULL) {
				int nOrg = m_pMotorY->IsOrgCompleted(nullptr);
				if (nOrg == FALSE)
				{
					m_pMotorY->V_MoveStop();
					return ERR_MOT_INTERLOCK_IS_NOT_ORG;
				}					
			}
		}break;
	case eJogMotor_Z:
		{
			if (m_pMotorZ != NULL) {
				int nOrg = m_pMotorZ->IsOrgCompleted(nullptr);
				if (nOrg == FALSE)
				{
					m_pMotorZ->V_MoveStop();
					return ERR_MOT_INTERLOCK_IS_NOT_ORG;
				}		
			}
		}break;
	case eJogMotor_R:
	{
		if (m_pMotorR != NULL) {
			int nOrg = m_pMotorR->IsOrgCompleted(nullptr);
			if (nOrg == FALSE)
			{
				m_pMotorR->V_MoveStop();
				return ERR_MOT_INTERLOCK_IS_NOT_ORG;
			}
		}
	}break;
	}


	int nErr = ERR_NO_ERROR;
	nErr = g_TaskSystemCtrl.ChkComInterlock();

	return nErr;
}


void CDlgManuMotorJog::OnBnClickedButtonCylinder()
{
	MakeLog("[CDlgManuMotorJog] Cylinder Dialog Show");
	OnBnClickedBtnCylinderManual();
	MakeLog("[CDlgManuMotorJog] Cylinder Dialog Close");
}


void CDlgManuMotorJog::OnBnClickedButtonUnitOperation()
{
	MakeLog("[CDlgManuMotorJog] UnitOperation Dialog Show");
	OnBnClickedBtnUnitOperation();
	MakeLog("[CDlgManuMotorJog]UnitOperation Dialog Close");
}

void CDlgManuMotorJog::OnBnClickedButtonStUnitOperation()
{
	MakeLog("[CDlgManuMotorJog] UnitOperation Dialog Show");
	OnBnClickedBtnUnitOperation();
	MakeLog("[CDlgManuMotorJog]UnitOperation Dialog Close");
}


void CDlgManuMotorJog::OnBnClickedBtnAixsManuPress(int nPressIdx)
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

	MakeLog("PressUnit%d Dialog Show", nPressIdx + 1);
	CDlgCommand_PressUnit CmdDlg;
	if (IDOK == CmdDlg.DoModal())
	{
		int nSelectedCmd = CmdDlg.GetSelectedSeqCmdId();
		switch (nSelectedCmd)
		{
		case eSeqCmd_PressUnit_Initial:
		{
			CPressUnitCmd_Initial* pSeqCmd = new CPressUnitCmd_Initial;
			int nErr = g_TaskSystemCtrl.SendCommand(g_TaskPressUnit[nPressIdx], pSeqCmd, eEqpCtrlMode_Manual);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
			}
			else {
				MakeLog("PressUnit%d Initial", nPressIdx + 1);
			}
		}break;
		case eSeqCmd_PressUnit_MovePos:
		{
			int nManu_Oper_Press_Index = 0;
			switch (nPressIdx)
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
			if (AfxMessageBox("Do you want auto height cal?", MB_OKCANCEL) != IDOK)
				break;

			CPressUnitCmd_AutoHeightCal * pSeqCmd = new CPressUnitCmd_AutoHeightCal;
			int nErr = g_TaskSystemCtrl.SendCommand(g_TaskPressUnit[nPressIdx], pSeqCmd, eEqpCtrlMode_Manual);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
			}
		}break;
		case eSeqCmd_PressUnit_SocketCleanContact:
		{
			if (AfxMessageBox("Do you want Socket Cleaning??", MB_OKCANCEL) != IDOK)
				break;

			CPressUnitCmd_SocketCleanContact * pSeqCmd = new CPressUnitCmd_SocketCleanContact;
			int nErr = g_TaskSystemCtrl.SendCommand(g_TaskPressUnit[nPressIdx], pSeqCmd, eEqpCtrlMode_Manual);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
			}
		}break;
		}
	}
}

void CDlgManuMotorJog::OnBnClickedBtnAixsManuTestPp(int nTestPPIdx)
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

	MakeLog("Test PP #%d Dialog Show", nTestPPIdx + 1);
	CDlgCommand_TestPP CmdDlg;
	if (IDOK == CmdDlg.DoModal())
	{
		int nSelectedCmd = CmdDlg.GetSelectedSeqCmdId();
		switch (nSelectedCmd)
		{
		case eSeqCmd_TestPP_Initial:
		{
			int nRetCmd = ERR_NO_ERROR;

			nRetCmd |= g_TaskPressUnit[ePressUnit_Down_1 + (eMaxATS_Area / 2 * nTestPPIdx)].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON);
			nRetCmd |= g_TaskPressUnit[ePressUnit_Down_2 + (eMaxATS_Area / 2 * nTestPPIdx)].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON);
			nRetCmd |= g_TaskPressUnit[ePressUnit_Down_3 + (eMaxATS_Area / 2 * nTestPPIdx)].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON);
			nRetCmd |= g_TaskPressUnit[ePressUnit_Down_4 + (eMaxATS_Area / 2 * nTestPPIdx)].ChkDI(CTaskPressUnit::xZAXIS_SAFETY_SEN, DEF_ON);

			if (nRetCmd != ERR_NO_ERROR) {
				AfxMessageBox("Press Unit is not safety pos!!!");
				return;
			}

			CTestPPCmd_Initial* pSeqCmd = new CTestPPCmd_Initial;
			int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTestPP[nTestPPIdx], pSeqCmd, eEqpCtrlMode_Manual);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
			}
			else {
				MakeLog("Test PP #%d Initial", nTestPPIdx + 1);
			}
		}break;
		case eSeqCmd_TestPP_MovePos:
		{
			CDlgAxisActionMove dlg;
			dlg.m_nStartPoint = eMANU_OPER_TEST_PP1 + nTestPPIdx;
			dlg.m_nSelectAction = eDlgActionOnlyMove;
			dlg.DoModal();
		}break;
		case eSeqCmd_TestPP_MovePickPlace:
		{
			CDlgAxisActionMove dlg;
			dlg.m_nStartPoint = eMANU_OPER_TEST_PP1 + nTestPPIdx;
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

void CDlgManuMotorJog::OnBnClickedBtnAixsManuLoadTbl(int nTableIdx)
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

	int nInterlockErr = g_TaskSystemCtrl.CheckManualable(TRUE);
	if (nInterlockErr != ERR_NO_ERROR) {
		CParamList AlarmData;
		g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
		return;
	}

	CDlgCommand_LoadTbl CmdDlg;
	MakeLog("Load Table #%d Dialog Show", nTableIdx + 1);
	if (IDOK == CmdDlg.DoModal())
	{
		int nSelectedCmd = CmdDlg.GetSelectedSeqCmdId();
		switch (nSelectedCmd)
		{
		case eSeqCmd_LoadTbl_Initial:
		{
			CLoadTblCmd_Initial* pSeqCmd = new CLoadTblCmd_Initial;
			int nErr = g_TaskSystemCtrl.SendCommand(g_TaskLoadTbl[nTableIdx], pSeqCmd, eEqpCtrlMode_Manual);
			if (nErr != ERR_NO_ERROR) {
				CParamList AlarmData;
				g_Error.AlarmReport(nErr, AlarmData, NULL);
			}
			else {
				MakeLog("Load Table #%d Initial", nTableIdx + 1);
			}
		}break;
		case eSeqCmd_LoadTbl_MovePos:
		{
			CDlgAxisActionMove dlg;
			dlg.m_nStartPoint = eMANU_OPER_LD_TBL1 + nTableIdx;
			dlg.DoModal();
		}break;
		}
	}
}

void CDlgManuMotorJog::MakeLog(LPCTSTR fmt, ...)
{
	TCHAR tmpMsg[1024] = { 0, };
	if (fmt)
	{
		va_list argList;
		va_start(argList, fmt);
		_vstprintf_s(tmpMsg, fmt, argList);
		va_end(argList);
	}
	TWLOG.MakeLog(static_cast<int>(eLogID_SYSTEM), Debug, "", __LINE__, NULL, tmpMsg);
}



void CDlgManuMotorJog::OnBnClickedButtonManuProcCmdStop()
{
	MakeLog("Stop Button Click");
	//	CBaseTaskManager::SendProcessCommand(eProcCmd_Stop, eEqpCtrlMode_Manual, NULL);
	CBaseTaskManager::SendProcessCommand(eProcCmd_Pause, eEqpCtrlMode_Manual, NULL);
}


void CDlgManuMotorJog::OnBnClickedButtonManuProcCmdSkip()
{
	MakeLog("Skip Button Click");
	CBaseTaskManager::SendProcessCommand(eProcCmd_Skip, eEqpCtrlMode_Manual, NULL);
}

void CDlgManuMotorJog::OnBnClickedCheckVisionLed()
{

}

int CDlgManuMotorJog::FindAxisNum(int nSelRobot)
{
	//eAXIS_TRAY_PP_1_Z
	int nAxisNo = 0;
	switch (nSelRobot)
	{
	case eJogTestPp1:
	{
		if (m_nTestPPHandType == eTestPPLoadType) {
			nAxisNo = eAXIS_TEST_PP_1_Z;
		}
		else {
			nAxisNo = eAXIS_TEST_PP_1_Z2;
		}
	}break;
	case eJogTestPp2:		
	{
		if (m_nTestPPHandType == eTestPPLoadType) {
			nAxisNo = eAXIS_TEST_PP_2_Z;
		}
		else {
			nAxisNo = eAXIS_TEST_PP_2_Z2;
		}
	}break;
	case eJogTrayPp1_LD:	 nAxisNo = eAXIS_TRAY_PP_1_Z; break;
	case eJogTrayPp2_UD:	 nAxisNo = eAXIS_TRAY_PP_2_Z; break;
	default:
		break;
	}

	return nAxisNo;
}