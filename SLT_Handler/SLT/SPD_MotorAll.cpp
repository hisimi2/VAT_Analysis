// SPD_MotorAll.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "SPD_MotorAll.h"
#include "afxdialogex.h"


// CSPD_MotorAll 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSPD_MotorAll, CDialog)

CSPD_MotorAll::CSPD_MotorAll(CWnd* pParent /*=NULL*/)
	: CDialog(CSPD_MotorAll::IDD, pParent)
{
	// Axis X or Y
	m_bTrayPP          = FALSE;
	m_bTestPP          = FALSE;
	m_bTransfer		   = FALSE;
	m_bLoadTbl         = FALSE;

	// Axis Z
	m_bTrayPP_Z        = FALSE;
	m_bTestPP_Z        = FALSE;
	m_bPressUnit	   = FALSE;
	m_bTrayPP_Pitch = FALSE;
	m_bTestPP_Pitch = FALSE;

	m_nSpeed = 0;
	m_nAccel = 0;
	m_nDecel = 0;
	
}

CSPD_MotorAll::~CSPD_MotorAll()
{
}

void CSPD_MotorAll::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_BtnSave);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Check(pDX, IDC_CHK_TRAY_PP, m_bTrayPP);
	DDX_Check(pDX, IDC_CHK_TEST_PP, m_bTestPP);
	DDX_Check(pDX, IDC_CHK_TRANS_CENTER, m_bTransfer);
	DDX_Check(pDX, IDC_CHK_SHUTTLE, m_bLoadTbl);
	DDX_Check(pDX, IDC_CHK_TRAYPP_Z, m_bTrayPP_Z);
	DDX_Check(pDX, IDC_CHK_TESTPP_Z, m_bTestPP_Z);
	DDX_Control(pDX, IDC_EDIT_SPD, m_editSpeed);
	DDX_Control(pDX, IDC_EDIT_ACC, m_editAccel);
	DDX_Control(pDX, IDC_EDIT_DCC, m_editDecel);
	DDX_Text(pDX, IDC_EDIT_SPD, m_nSpeed);
	DDX_Text(pDX, IDC_EDIT_ACC, m_nAccel);
	DDX_Text(pDX, IDC_EDIT_DCC, m_nDecel);
	DDX_Check(pDX, IDC_CHK_CONTACTOR, m_bPressUnit);
	DDX_Check(pDX, IDC_CHK_TRAY_PP_PITCH, m_bTrayPP_Pitch);
	DDX_Check(pDX, IDC_CHK_TEST_PP_PITCH, m_bTestPP_Pitch);
	DDX_Check(pDX, IDC_CHK_FEEDER, m_bFeeder);
	DDX_Check(pDX, IDC_CHK_ALL_SELECT, m_bAllUnitSelect);
}


BEGIN_MESSAGE_MAP(CSPD_MotorAll, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CSPD_MotorAll::OnBnClickedButtonSave)
	ON_EN_SETFOCUS(IDC_EDIT_SPD, &CSPD_MotorAll::OnEnSetfocusEditSpd)
	ON_EN_SETFOCUS(IDC_EDIT_ACC, &CSPD_MotorAll::OnEnSetfocusEditAcc)
	ON_EN_SETFOCUS(IDC_EDIT_DCC, &CSPD_MotorAll::OnEnSetfocusEditDcc)
	ON_BN_CLICKED(IDC_CHK_ALL_SELECT, &CSPD_MotorAll::OnBnClickedChkAllSelect)
END_MESSAGE_MAP()


// CSPD_MotorAll 메시지 처리기입니다.


BOOL CSPD_MotorAll::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnBtnGUI();
	OnEditBoxGUI();

	m_vButtonIDs.push_back(IDC_CHK_TRAY_PP);
	m_vButtonIDs.push_back(IDC_CHK_TEST_PP);
	m_vButtonIDs.push_back(IDC_CHK_TRANS_CENTER);
	m_vButtonIDs.push_back(IDC_CHK_SHUTTLE);
	m_vButtonIDs.push_back(IDC_CHK_TRAYPP_Z);
	m_vButtonIDs.push_back(IDC_CHK_TESTPP_Z);
	m_vButtonIDs.push_back(IDC_CHK_CONTACTOR);
	m_vButtonIDs.push_back(IDC_CHK_TRAY_PP_PITCH);
	m_vButtonIDs.push_back(IDC_CHK_TEST_PP_PITCH);
	m_vButtonIDs.push_back(IDC_CHK_FEEDER);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSPD_MotorAll::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);
}

void CSPD_MotorAll::OnEditBoxGUI()
{
	m_editSpeed.SetBackColor(COLOR_WWHITE);
	m_editSpeed.SetTextSize(30);

	m_editAccel.SetBackColor(COLOR_WWHITE);
	m_editAccel.SetTextSize(30);

	m_editDecel.SetBackColor(COLOR_WWHITE);
	m_editDecel.SetTextSize(30);
}

void CSPD_MotorAll::OnBnClickedButtonSave()
{
	UpdateData(TRUE);

	if(m_bTrayPP){
		//Load Tray
		g_DMCont.m_dmTrayPP.SN(NDM5_SPD_LD_TrayPP_X, m_nSpeed);
		g_DMCont.m_dmTrayPP.SN(NDM5_ACC_LD_TrayPP_X, m_nAccel);
		g_DMCont.m_dmTrayPP.SN(NDM5_DCC_LD_TrayPP_X, m_nDecel);
		g_DMCont.m_dmTrayPP.SN(NDM5_SPD_Gripper_Y, m_nSpeed);
		g_DMCont.m_dmTrayPP.SN(NDM5_ACC_Gripper_Y, m_nAccel);
		g_DMCont.m_dmTrayPP.SN(NDM5_DCC_Gripper_Y, m_nDecel);

		//Unload Tray
		g_DMCont.m_dmTrayPP.SN(NDM5_SPD_UL_TrayPP_X, m_nSpeed);
		g_DMCont.m_dmTrayPP.SN(NDM5_ACC_UL_TrayPP_X, m_nAccel);
		g_DMCont.m_dmTrayPP.SN(NDM5_DCC_UL_TrayPP_X, m_nDecel);
		g_DMCont.m_dmTrayPP.SN(NDM5_SPD_UL_TrayPP_Y, m_nSpeed);
		g_DMCont.m_dmTrayPP.SN(NDM5_ACC_UL_TrayPP_Y, m_nAccel);
		g_DMCont.m_dmTrayPP.SN(NDM5_DCC_UL_TrayPP_Y, m_nDecel);
	}
	if(m_bTestPP){
		g_DMCont.m_dmTestPP.SN(NDM4_SPD_TestPP1_X1,m_nSpeed);
		g_DMCont.m_dmTestPP.SN(NDM4_ACC_TestPP1_X1,m_nAccel);
		g_DMCont.m_dmTestPP.SN(NDM4_DCC_TestPP1_X1,m_nDecel);
		g_DMCont.m_dmTestPP.SN(NDM4_SPD_TestPP1_X2,m_nSpeed);
		g_DMCont.m_dmTestPP.SN(NDM4_ACC_TestPP1_X2,m_nAccel);
		g_DMCont.m_dmTestPP.SN(NDM4_DCC_TestPP1_X2,m_nDecel);
		g_DMCont.m_dmTestPP.SN(NDM4_SPD_TestPP1_Y,m_nSpeed);
		g_DMCont.m_dmTestPP.SN(NDM4_ACC_TestPP1_Y,m_nAccel);
		g_DMCont.m_dmTestPP.SN(NDM4_DCC_TestPP1_Y,m_nDecel);

		g_DMCont.m_dmTestPP.SN(NDM4_SPD_TestPP2_X1,m_nSpeed);
		g_DMCont.m_dmTestPP.SN(NDM4_ACC_TestPP2_X1,m_nAccel);
		g_DMCont.m_dmTestPP.SN(NDM4_DCC_TestPP2_X1,m_nDecel);
		g_DMCont.m_dmTestPP.SN(NDM4_SPD_TestPP2_X2,m_nSpeed);
		g_DMCont.m_dmTestPP.SN(NDM4_ACC_TestPP2_X2,m_nAccel);
		g_DMCont.m_dmTestPP.SN(NDM4_DCC_TestPP2_X2,m_nDecel);
		g_DMCont.m_dmTestPP.SN(NDM4_SPD_TestPP2_Y,m_nSpeed);
		g_DMCont.m_dmTestPP.SN(NDM4_ACC_TestPP2_Y,m_nAccel);
		g_DMCont.m_dmTestPP.SN(NDM4_DCC_TestPP2_Y,m_nDecel);
	}   
	if(m_bLoadTbl){
		g_DMCont.m_dmShuttle.SN(NDM3_SPD_LoadTbl1_Y,m_nSpeed);
		g_DMCont.m_dmShuttle.SN(NDM3_ACC_LoadTbl1_Y,m_nAccel);
		g_DMCont.m_dmShuttle.SN(NDM3_DCC_LoadTbl1_Y,m_nDecel);
		g_DMCont.m_dmShuttle.SN(NDM3_SPD_LoadTbl2_Y,m_nSpeed);
		g_DMCont.m_dmShuttle.SN(NDM3_ACC_LoadTbl2_Y,m_nAccel);
		g_DMCont.m_dmShuttle.SN(NDM3_DCC_LoadTbl2_Y,m_nDecel);
		g_DMCont.m_dmShuttle.SN(NDM3_SPD_LoadTbl3_Y, m_nSpeed);
		g_DMCont.m_dmShuttle.SN(NDM3_ACC_LoadTbl3_Y, m_nAccel);
		g_DMCont.m_dmShuttle.SN(NDM3_DCC_LoadTbl3_Y, m_nDecel);
		g_DMCont.m_dmShuttle.SN(NDM3_SPD_LoadTbl4_Y, m_nSpeed);
		g_DMCont.m_dmShuttle.SN(NDM3_ACC_LoadTbl4_Y, m_nAccel);
		g_DMCont.m_dmShuttle.SN(NDM3_DCC_LoadTbl4_Y, m_nDecel);

		g_DMCont.m_dmShuttle.SN(NDM3_SPD_LoadTbl1_Rotate, m_nSpeed);
		g_DMCont.m_dmShuttle.SN(NDM3_ACC_LoadTbl1_Rotate, m_nAccel);
		g_DMCont.m_dmShuttle.SN(NDM3_DCC_LoadTbl1_Rotate, m_nDecel);
		g_DMCont.m_dmShuttle.SN(NDM3_SPD_LoadTbl2_Rotate, m_nSpeed);
		g_DMCont.m_dmShuttle.SN(NDM3_ACC_LoadTbl2_Rotate, m_nAccel);
		g_DMCont.m_dmShuttle.SN(NDM3_DCC_LoadTbl2_Rotate, m_nDecel);
		g_DMCont.m_dmShuttle.SN(NDM3_SPD_LoadTbl3_Rotate, m_nSpeed);
		g_DMCont.m_dmShuttle.SN(NDM3_ACC_LoadTbl3_Rotate, m_nAccel);
		g_DMCont.m_dmShuttle.SN(NDM3_DCC_LoadTbl3_Rotate, m_nDecel);
		g_DMCont.m_dmShuttle.SN(NDM3_SPD_LoadTbl4_Rotate, m_nSpeed);
		g_DMCont.m_dmShuttle.SN(NDM3_ACC_LoadTbl4_Rotate, m_nAccel);
		g_DMCont.m_dmShuttle.SN(NDM3_DCC_LoadTbl4_Rotate, m_nDecel);
	}
	if(m_bTransfer){
		g_DMCont.m_dmCTrayXfer.SN(NDM9_SPD_Transfer_X,m_nSpeed);
		g_DMCont.m_dmCTrayXfer.SN(NDM9_ACC_Transfer_X,m_nAccel);
		g_DMCont.m_dmCTrayXfer.SN(NDM9_DCC_Transfer_X,m_nDecel);
	}
	if (m_bFeeder) {
		g_DMCont.m_dmFeeder.SN(NDM10_SPD_Feeder_Y, m_nSpeed);
		g_DMCont.m_dmFeeder.SN(NDM10_ACC_Feeder_Y, m_nAccel);
		g_DMCont.m_dmFeeder.SN(NDM10_DCC_Feeder_Y, m_nDecel);
	}
	// Axis Z
	if(m_bTrayPP_Z){
		//Load Tray PP
		g_DMCont.m_dmTrayPP.SN(NDM5_SPD_LD_TrayPP_Z,m_nSpeed);
		g_DMCont.m_dmTrayPP.SN(NDM5_ACC_LD_TrayPP_Z,m_nAccel);
		g_DMCont.m_dmTrayPP.SN(NDM5_DCC_LD_TrayPP_Z,m_nDecel);

		//Unload Tray PP
		g_DMCont.m_dmTrayPP.SN(NDM5_SPD_UL_TrayPP_Z, m_nSpeed);
		g_DMCont.m_dmTrayPP.SN(NDM5_ACC_UL_TrayPP_Z, m_nAccel);
		g_DMCont.m_dmTrayPP.SN(NDM5_DCC_UL_TrayPP_Z, m_nDecel);
	}
	if(m_bTestPP_Z){
		g_DMCont.m_dmTestPP.SN(NDM4_SPD_TestPP1_Z1,m_nSpeed);
		g_DMCont.m_dmTestPP.SN(NDM4_ACC_TestPP1_Z1,m_nAccel);
		g_DMCont.m_dmTestPP.SN(NDM4_DCC_TestPP1_Z1,m_nDecel);
		g_DMCont.m_dmTestPP.SN(NDM4_SPD_TestPP1_Z2,m_nSpeed);
		g_DMCont.m_dmTestPP.SN(NDM4_ACC_TestPP1_Z2,m_nAccel);
		g_DMCont.m_dmTestPP.SN(NDM4_DCC_TestPP1_Z2,m_nDecel);
		g_DMCont.m_dmTestPP.SN(NDM4_SPD_TestPP2_Z1, m_nSpeed);
		g_DMCont.m_dmTestPP.SN(NDM4_ACC_TestPP2_Z1, m_nAccel);
		g_DMCont.m_dmTestPP.SN(NDM4_DCC_TestPP2_Z1, m_nDecel);
		g_DMCont.m_dmTestPP.SN(NDM4_SPD_TestPP2_Z2, m_nSpeed);
		g_DMCont.m_dmTestPP.SN(NDM4_ACC_TestPP2_Z2, m_nAccel);
		g_DMCont.m_dmTestPP.SN(NDM4_DCC_TestPP2_Z2, m_nDecel);
	}
	if(m_bPressUnit){
		g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit1,m_nSpeed);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit1,m_nAccel);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit1,m_nDecel);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit2,m_nSpeed);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit2,m_nAccel);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit2,m_nDecel);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit3,m_nSpeed);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit3,m_nAccel);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit3,m_nDecel);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit4,m_nSpeed);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit4,m_nAccel);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit4,m_nDecel);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit5,m_nSpeed);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit5,m_nAccel);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit5,m_nDecel);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit6,m_nSpeed);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit6,m_nAccel);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit6,m_nDecel);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit7,m_nSpeed);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit7,m_nAccel);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit7,m_nDecel);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit8,m_nSpeed);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit8,m_nAccel);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit8,m_nDecel);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit9,m_nSpeed);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit9,m_nAccel);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit9,m_nDecel);	
		g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit10,m_nSpeed);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit10,m_nAccel);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit10,m_nDecel);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit11,m_nSpeed);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit11,m_nAccel);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit11,m_nDecel);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit12,m_nSpeed);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit12,m_nAccel);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit12,m_nDecel);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit13,m_nSpeed);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit13,m_nAccel);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit13,m_nDecel);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit14,m_nSpeed);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit14,m_nAccel);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit14,m_nDecel);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit15,m_nSpeed);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit15,m_nAccel);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit15,m_nDecel);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit16,m_nSpeed);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit16,m_nAccel);
		g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit16,m_nDecel);
	}
	if(m_bTrayPP_Pitch){
		//Load Tray PP
		g_DMCont.m_dmTrayPP.SN(NDM5_SPD_LD_TrayPP_X_Pitch,m_nSpeed);
        g_DMCont.m_dmTrayPP.SN(NDM5_ACC_LD_TrayPP_X_Pitch,m_nAccel);
		g_DMCont.m_dmTrayPP.SN(NDM5_DCC_LD_TrayPP_X_Pitch,m_nDecel);
		g_DMCont.m_dmTrayPP.SN(NDM5_SPD_LD_TrayPP_Y_Pitch,m_nSpeed);
		g_DMCont.m_dmTrayPP.SN(NDM5_ACC_LD_TrayPP_Y_Pitch,m_nAccel);
		g_DMCont.m_dmTrayPP.SN(NDM5_DCC_LD_TrayPP_Y_Pitch,m_nDecel);

		//Unload Tray PP
		g_DMCont.m_dmTrayPP.SN(NDM5_SPD_UL_TrayPP_X_Pitch, m_nSpeed);
		g_DMCont.m_dmTrayPP.SN(NDM5_ACC_UL_TrayPP_X_Pitch, m_nAccel);
		g_DMCont.m_dmTrayPP.SN(NDM5_DCC_UL_TrayPP_X_Pitch, m_nDecel);
		g_DMCont.m_dmTrayPP.SN(NDM5_SPD_UL_TrayPP_Y_Pitch, m_nSpeed);
		g_DMCont.m_dmTrayPP.SN(NDM5_ACC_UL_TrayPP_Y_Pitch, m_nAccel);
		g_DMCont.m_dmTrayPP.SN(NDM5_DCC_UL_TrayPP_Y_Pitch, m_nDecel);
	}
	if (m_bTestPP_Pitch) {
		g_DMCont.m_dmTestPP.SN(NDM5_SPD_TestPP_1_Pitch_X1, m_nSpeed);
		g_DMCont.m_dmTestPP.SN(NDM5_ACC_TestPP_1_Pitch_X1, m_nAccel);
		g_DMCont.m_dmTestPP.SN(NDM5_DCC_TestPP_1_Pitch_X1, m_nDecel);
		g_DMCont.m_dmTestPP.SN(NDM5_SPD_TestPP_1_Pitch_X2, m_nSpeed);
		g_DMCont.m_dmTestPP.SN(NDM5_ACC_TestPP_1_Pitch_X2, m_nAccel);
		g_DMCont.m_dmTestPP.SN(NDM5_DCC_TestPP_1_Pitch_X2, m_nDecel);
		g_DMCont.m_dmTestPP.SN(NDM5_SPD_TestPP_1_Pitch_Y1, m_nSpeed);
		g_DMCont.m_dmTestPP.SN(NDM5_ACC_TestPP_1_Pitch_Y1, m_nAccel);
		g_DMCont.m_dmTestPP.SN(NDM5_DCC_TestPP_1_Pitch_Y1, m_nDecel);
		g_DMCont.m_dmTestPP.SN(NDM5_SPD_TestPP_1_Pitch_Y2, m_nSpeed);
		g_DMCont.m_dmTestPP.SN(NDM5_ACC_TestPP_1_Pitch_Y2, m_nAccel);
		g_DMCont.m_dmTestPP.SN(NDM5_DCC_TestPP_1_Pitch_Y2, m_nDecel);
		g_DMCont.m_dmTestPP.SN(NDM5_SPD_TestPP_2_Pitch_X1, m_nSpeed);
		g_DMCont.m_dmTestPP.SN(NDM5_ACC_TestPP_2_Pitch_X1, m_nAccel);
		g_DMCont.m_dmTestPP.SN(NDM5_DCC_TestPP_2_Pitch_X1, m_nDecel);
		g_DMCont.m_dmTestPP.SN(NDM5_SPD_TestPP_2_Pitch_X2, m_nSpeed);
		g_DMCont.m_dmTestPP.SN(NDM5_ACC_TestPP_2_Pitch_X2, m_nAccel);
		g_DMCont.m_dmTestPP.SN(NDM5_DCC_TestPP_2_Pitch_X2, m_nDecel);
		g_DMCont.m_dmTestPP.SN(NDM5_SPD_TestPP_2_Pitch_Y1, m_nSpeed);
		g_DMCont.m_dmTestPP.SN(NDM5_ACC_TestPP_2_Pitch_Y1, m_nAccel);
		g_DMCont.m_dmTestPP.SN(NDM5_DCC_TestPP_2_Pitch_Y1, m_nDecel);
		g_DMCont.m_dmTestPP.SN(NDM5_SPD_TestPP_2_Pitch_Y2, m_nSpeed);
		g_DMCont.m_dmTestPP.SN(NDM5_ACC_TestPP_2_Pitch_Y2, m_nAccel);
		g_DMCont.m_dmTestPP.SN(NDM5_DCC_TestPP_2_Pitch_Y2, m_nDecel);
	}

	char szCokName[512] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));

	ST_COK_INFO   stCokData;
	g_COK_FileMgr.LoadCokInfo(szCokName, stCokData);

	if(m_bTrayPP){
		for (int i = 0; i <= RIDX_TRAY_PP_2; i++)
		{
			stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_X].nSPD = m_nSpeed;
			stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_X].nACC = m_nAccel;
			stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_X].nDCC = m_nDecel;
			stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_Y].nSPD = m_nSpeed;
			stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_Y].nACC = m_nAccel;
			stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_Y].nDCC = m_nDecel;
		}
	}
	if(m_bTrayPP_Z){
		for (int i = 0; i <= RIDX_TRAY_PP_2; i++)
		{
			stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_Z].nSPD = m_nSpeed;
			stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_Z].nACC = m_nAccel;
			stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_Z].nDCC = m_nDecel;
		}
	}

	if(m_bTestPP){
		for(int i=0; i<=RIDX_TEST_PP_2; i++)
		{
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1].nSPD = m_nSpeed;
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1].nACC = m_nAccel;
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1].nDCC = m_nDecel;
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2].nSPD = m_nSpeed;
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2].nACC = m_nAccel;
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2].nDCC = m_nDecel;
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y].nSPD = m_nSpeed;
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y].nACC = m_nAccel;
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y].nDCC = m_nDecel;
		}
	}
	if(m_bTestPP_Z){
		for(int i=0; i<=RIDX_TEST_PP_2; i++)
		{
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z1].nSPD = m_nSpeed;
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z1].nACC = m_nAccel;
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z1].nDCC = m_nDecel;
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z2].nSPD = m_nSpeed;
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z2].nACC = m_nAccel;
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z2].nDCC = m_nDecel;
		}
	}

	if(m_bLoadTbl){
		for(int i=0; i<= RIDX_LD_TBL_4; i++)
		{
			stCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Y].nSPD = m_nSpeed;
			stCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Y].nACC = m_nAccel;
			stCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Y].nDCC = m_nDecel;

			stCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Rotate].nSPD = m_nSpeed;
			stCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Rotate].nACC = m_nAccel;
			stCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_Tbl_Rotate].nDCC = m_nDecel;

			if(i==RIDX_LD_TBL_1 || i== RIDX_LD_TBL_2)
			{
				stCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_X].nSPD = m_nSpeed;
				stCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_X].nACC = m_nAccel;
				stCokData.aLoadTbl_SPD[i].aAxisForm[eAxis_X].nDCC = m_nDecel;
			}
		}
	}

	if(m_bTransfer){
		stCokData.aTransfer.aAxisForm[eAxis_X].nSPD = m_nSpeed;
		stCokData.aTransfer.aAxisForm[eAxis_X].nACC = m_nAccel;
		stCokData.aTransfer.aAxisForm[eAxis_X].nDCC = m_nDecel;
	}

	if (m_bFeeder) {
		stCokData.aFeeder_SPD.aAxisForm[eAxis_Y].nSPD = m_nSpeed;
		stCokData.aFeeder_SPD.aAxisForm[eAxis_Y].nACC = m_nSpeed;
		stCokData.aFeeder_SPD.aAxisForm[eAxis_Y].nDCC = m_nSpeed;
	}

	if(m_bPressUnit){
		for(int i=0; i<= RIDX_PRESS_UNIT_8; i++)
		{
			stCokData.aPressUnit_SPD[i].aAxisForm[eAxis_Z].nSPD = m_nSpeed;
			stCokData.aPressUnit_SPD[i].aAxisForm[eAxis_Z].nACC = m_nAccel;
			stCokData.aPressUnit_SPD[i].aAxisForm[eAxis_Z].nDCC = m_nDecel;
		}
	}

	if(m_bTrayPP_Pitch){
		//Load Tray PP
		for (int i = 0; i <= RIDX_TRAY_PP_2; i++)
		{
			stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_X_Pitch].nSPD = m_nSpeed;
			stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_X_Pitch].nACC = m_nAccel;
			stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_X_Pitch].nDCC = m_nDecel;
			stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_Y_Pitch].nSPD = m_nSpeed;
			stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_Y_Pitch].nACC = m_nAccel;
			stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_Y_Pitch].nDCC = m_nDecel;

		}
	}

	if (m_bTestPP_Pitch) {
		for (int i = 0; i <= RIDX_TEST_PP_2; i++)
		{
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1_Pitch].nSPD = m_nSpeed;
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1_Pitch].nACC = m_nAccel;
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1_Pitch].nDCC = m_nDecel;
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2_Pitch].nSPD = m_nSpeed;
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2_Pitch].nACC = m_nAccel;
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2_Pitch].nDCC = m_nDecel;

			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y1_Pitch].nSPD = m_nSpeed;
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y1_Pitch].nACC = m_nAccel;
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y1_Pitch].nDCC = m_nDecel;
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y2_Pitch].nSPD = m_nSpeed;
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y2_Pitch].nACC = m_nAccel;
			stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y2_Pitch].nDCC = m_nDecel;
		}
	}

	g_COK_FileMgr.SaveCokInfo(szCokName, &stCokData);

	//GUI Data Change Log
	SpeedAllChangeLog();
}


void CSPD_MotorAll::OnEnSetfocusEditSpd()
{
	char szVal[10] = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nSpeed, szVal, sizeof(szVal));
	
	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Speed", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_SPD)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}	
}


void CSPD_MotorAll::OnEnSetfocusEditAcc()
{
	char szVal[10] = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nAccel, szVal, sizeof(szVal));
	
	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Accel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_ACC)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}	
}

void CSPD_MotorAll::OnEnSetfocusEditDcc()
{
	char szVal[10] = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nDecel, szVal, sizeof(szVal));
	
	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Decel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DCC)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}	
}


void CSPD_MotorAll::MakeLog(LPCTSTR fmt, ...)
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


void CSPD_MotorAll::SpeedAllChangeLog()
{
	CString strDataXY = _T("");
	CString strDataZ = _T("");
	int nCheckXY[4] = {m_bTrayPP, m_bTestPP, m_bLoadTbl, m_bTransfer};
	int nCheckZ[3] = {m_bTrayPP_Z, m_bTestPP_Z, m_bPressUnit};
	CString strCheckXY[4] ={"Tray PP, ", "Test PP, ", "Loading Table, ", "Transfer, "};
	CString strCheckZ[3] ={"Tray PP, ", "Test PP, ", "Press Unit, "};

	for(int i =0 ; i<4 ; i++)
	{
		if(nCheckXY[i] == 1)//Check
		{
			strDataXY +=_T(strCheckXY[i]);
		}
	
	}

	for( int j = 0; j < 3; j++)
	{
		if(nCheckZ[j] == 1)
		{
			strDataZ +=_T(strCheckZ[j]);
		}
	}

	if(!(strDataXY.IsEmpty()))
	{
		MakeLog("[MENU = Speed All Axis][%s][X, Y - Speed = %d %%, X, Y - Accel = %d %%, X, Y - Decel = %d %%]", strDataXY, m_nSpeed, m_nAccel, m_nDecel);
	}
	if(!(strDataZ.IsEmpty()))
	{
		MakeLog("[MENU = Speed All Axis][%s][Z - Speed = %d %, Z - Accel = %d %%, Z - Decel = %d %%]", strDataZ, m_nSpeed, m_nAccel, m_nDecel);
	}
}


void CSPD_MotorAll::OnBnClickedChkAllSelect()
{
	UpdateData(TRUE);

	for (auto it = m_vButtonIDs.begin(); it != m_vButtonIDs.end(); it++)
	{
		CButton* pButton = (CButton*)GetDlgItem(*it);
		pButton->SetCheck(m_bAllUnitSelect == TRUE ? BST_CHECKED : BST_UNCHECKED);	
	}
}
