// SPD_TM_TRAY_PP.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "SPD_TM_TRAY_PP.h"
#include "afxdialogex.h"


// CSPD_TM_TRAY_PP 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSPD_TM_TRAY_PP, CDialog)

CSPD_TM_TRAY_PP::CSPD_TM_TRAY_PP(CWnd* pParent /*=NULL*/)
	: CDialog(CSPD_TM_TRAY_PP::IDD, pParent)
{
	//Load Tray PP
	m_nSPD_LDTrayPP_X = g_DMCont.m_dmTrayPP.GN(NDM5_SPD_LD_TrayPP_X)      ;
	m_nACC_LDTrayPP_X       = g_DMCont.m_dmTrayPP.GN(NDM5_ACC_LD_TrayPP_X)      ;
	m_nDCC_LDTrayPP_X       = g_DMCont.m_dmTrayPP.GN(NDM5_DCC_LD_TrayPP_X)      ;
 	m_nSPD_Gripper_Y       = g_DMCont.m_dmTrayPP.GN(NDM5_SPD_Gripper_Y)      ;
 	m_nACC_Gripper_Y       = g_DMCont.m_dmTrayPP.GN(NDM5_ACC_Gripper_Y)      ;
 	m_nDCC_Gripper_Y       = g_DMCont.m_dmTrayPP.GN(NDM5_DCC_Gripper_Y)      ;
	m_nSPD_LDTrayPP_Z       = g_DMCont.m_dmTrayPP.GN(NDM5_SPD_LD_TrayPP_Z)      ;
	m_nACC_LDTrayPP_Z       = g_DMCont.m_dmTrayPP.GN(NDM5_ACC_LD_TrayPP_Z)      ;
	m_nDCC_LDTrayPP_Z       = g_DMCont.m_dmTrayPP.GN(NDM5_DCC_LD_TrayPP_Z)      ;
	m_nSPD_LDTrayPP_X_Pitch = g_DMCont.m_dmTrayPP.GN(NDM5_SPD_LD_TrayPP_X_Pitch);
	m_nACC_LDTrayPP_X_Pitch = g_DMCont.m_dmTrayPP.GN(NDM5_ACC_LD_TrayPP_X_Pitch);
	m_nDCC_LDTrayPP_X_Pitch = g_DMCont.m_dmTrayPP.GN(NDM5_DCC_LD_TrayPP_X_Pitch);
	m_nSPD_LDTrayPP_Y_Pitch = g_DMCont.m_dmTrayPP.GN(NDM5_SPD_LD_TrayPP_Y_Pitch);
	m_nACC_LDTrayPP_Y_Pitch = g_DMCont.m_dmTrayPP.GN(NDM5_ACC_LD_TrayPP_Y_Pitch);
	m_nDCC_LDTrayPP_Y_Pitch = g_DMCont.m_dmTrayPP.GN(NDM5_DCC_LD_TrayPP_Y_Pitch);

	//Unload Tray PP
	m_nSPD_ULTrayPP_X = g_DMCont.m_dmTrayPP.GN(NDM5_SPD_UL_TrayPP_X);
	m_nACC_ULTrayPP_X = g_DMCont.m_dmTrayPP.GN(NDM5_ACC_UL_TrayPP_X);
	m_nDCC_ULTrayPP_X = g_DMCont.m_dmTrayPP.GN(NDM5_DCC_UL_TrayPP_X);
	m_nSPD_ULTrayPP_Y = g_DMCont.m_dmTrayPP.GN(NDM5_SPD_UL_TrayPP_Y);
	m_nACC_ULTrayPP_Y       = g_DMCont.m_dmTrayPP.GN(NDM5_ACC_UL_TrayPP_Y);
	m_nDCC_ULTrayPP_Y       = g_DMCont.m_dmTrayPP.GN(NDM5_DCC_UL_TrayPP_Y);
	m_nSPD_ULTrayPP_Z = g_DMCont.m_dmTrayPP.GN(NDM5_SPD_UL_TrayPP_Z);
	m_nACC_ULTrayPP_Z = g_DMCont.m_dmTrayPP.GN(NDM5_ACC_UL_TrayPP_Z);
	m_nDCC_ULTrayPP_Z = g_DMCont.m_dmTrayPP.GN(NDM5_DCC_UL_TrayPP_Z);
	m_nSPD_ULTrayPP_X_Pitch = g_DMCont.m_dmTrayPP.GN(NDM5_SPD_UL_TrayPP_X_Pitch);
	m_nACC_ULTrayPP_X_Pitch = g_DMCont.m_dmTrayPP.GN(NDM5_ACC_UL_TrayPP_X_Pitch);
	m_nDCC_ULTrayPP_X_Pitch = g_DMCont.m_dmTrayPP.GN(NDM5_DCC_UL_TrayPP_X_Pitch);
	m_nSPD_ULTrayPP_Y_Pitch = g_DMCont.m_dmTrayPP.GN(NDM5_SPD_UL_TrayPP_Y_Pitch);
	m_nACC_ULTrayPP_Y_Pitch = g_DMCont.m_dmTrayPP.GN(NDM5_ACC_UL_TrayPP_Y_Pitch);
	m_nDCC_ULTrayPP_Y_Pitch = g_DMCont.m_dmTrayPP.GN(NDM5_DCC_UL_TrayPP_Y_Pitch);



	m_dTrayPP_Vacuum_LD     = (double)g_DMCont.m_dmTrayPP.GN(NDM5_TM_LDTrayPP_Vacuum)  /1000.0;
	m_dTrayPP_Blow_LD       = (double)g_DMCont.m_dmTrayPP.GN(NDM5_TM_LDTrayPP_Blow)    /1000.0;
	m_dTrayPP_Vacuum_UD = (double)g_DMCont.m_dmTrayPP.GN(NDM5_TM_UDTrayPP_Vacuum) / 1000.0;
	m_dTrayPP_Blow_UD = (double)g_DMCont.m_dmTrayPP.GN(NDM5_TM_UDTrayPP_Blow) / 1000.0;

	m_nRetryCnt          = g_DMCont.m_dmTrayPP.GN(NDM5_LDTrayPP_RetryCnt);
	m_dTrayPP_CylDownWait= (double)g_DMCont.m_dmTrayPP.GN(NDM5_TM_LDTrayPP_CylDownWait) /1000.0;

	m_dTrayPP_Pick_1Stage_Offset = (double)g_DMCont.m_dmTrayPP.GN(NDM5_TM_LDTrayPP_Pick_1Stage_Offset) / 1000.0;
	m_dTrayPP_Pick_1Stage_Speed = (double)g_DMCont.m_dmTrayPP.GN(NDM5_TM_LDTrayPP_Pick_1Stage_Speed) / 1000.0;

	m_dULTrayPP_Linear_Offset = (double)g_DMCont.m_dmTrayPP.GN(NDM5_TM_UDTrayPP_Linear_Offset) / 1000.0;
}

CSPD_TM_TRAY_PP::~CSPD_TM_TRAY_PP()
{
}

void CSPD_TM_TRAY_PP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SAVE           , m_BtnSave);
	DDX_Control(pDX, IDCANCEL                  , m_BtnCancel);


	//Load Tray PP
	DDX_Text   (pDX, IDC_EDIT_SPD_X_LD_TRAY				, m_nSPD_LDTrayPP_X);
	DDX_Text   (pDX, IDC_EDIT_ACC_X_LD_TRAY				, m_nACC_LDTrayPP_X);
	DDX_Text   (pDX, IDC_EDIT_DCC_X_LD_TRAY				, m_nDCC_LDTrayPP_X);
 	DDX_Text   (pDX, IDC_EDIT_SPD_Y_GRIPPER				, m_nSPD_Gripper_Y);
 	DDX_Text   (pDX, IDC_EDIT_ACC_Y_GRIPPER				, m_nACC_Gripper_Y);
 	DDX_Text   (pDX, IDC_EDIT_DCC_Y_GRIPPER				, m_nDCC_Gripper_Y);
	DDX_Text   (pDX, IDC_EDIT_SPD_Z_LD_TRAY				, m_nSPD_LDTrayPP_Z);
	DDX_Text   (pDX, IDC_EDIT_ACC_Z_LD_TRAY				, m_nACC_LDTrayPP_Z);
	DDX_Text   (pDX, IDC_EDIT_DCC_Z_LD_TRAY				, m_nDCC_LDTrayPP_Z);
	DDX_Text   (pDX, IDC_EDIT_SPD_X_PITCH_LD_TRAY		, m_nSPD_LDTrayPP_X_Pitch);
	DDX_Text   (pDX, IDC_EDIT_ACC_X_PITCH_LD_TRAY		, m_nACC_LDTrayPP_X_Pitch);
	DDX_Text   (pDX, IDC_EDIT_DCC_X_PITCH_LD_TRAY		, m_nDCC_LDTrayPP_X_Pitch);
	DDX_Text   (pDX, IDC_EDIT_SPD_Y_PITCH_LD_TRAY		, m_nSPD_LDTrayPP_Y_Pitch);
	DDX_Text   (pDX, IDC_EDIT_ACC_Y_PITCH_LD_TRAY		, m_nACC_LDTrayPP_Y_Pitch);
	DDX_Text   (pDX, IDC_EDIT_DCC_Y_PITCH_LD_TRAY		, m_nDCC_LDTrayPP_Y_Pitch);
	DDX_Control(pDX, IDC_EDIT_SPD_X_LD_TRAY				, m_editSPD_LDTray_X);
	DDX_Control(pDX, IDC_EDIT_ACC_X_LD_TRAY				, m_editACC_LDTray_X);
	DDX_Control(pDX, IDC_EDIT_DCC_X_LD_TRAY				, m_editDCC_LDTray_X);
 	DDX_Control(pDX, IDC_EDIT_SPD_Y_GRIPPER				, m_editSPD_Gripper_Y);
 	DDX_Control(pDX, IDC_EDIT_ACC_Y_GRIPPER				, m_editACC_Gripper_Y);
 	DDX_Control(pDX, IDC_EDIT_DCC_Y_GRIPPER				, m_editDCC_Gripper_Y);
	DDX_Control(pDX, IDC_EDIT_SPD_Z_LD_TRAY				, m_editSPD_LDTray_Z);
	DDX_Control(pDX, IDC_EDIT_ACC_Z_LD_TRAY				, m_editACC_LDTray_Z);
	DDX_Control(pDX, IDC_EDIT_DCC_Z_LD_TRAY				, m_editDCC_LDTray_Z);
	DDX_Control(pDX, IDC_EDIT_SPD_X_PITCH_LD_TRAY		, m_editSPD_LDTray_X_Pitch);
	DDX_Control(pDX, IDC_EDIT_ACC_X_PITCH_LD_TRAY		, m_editACC_LDTray_X_Pitch);
	DDX_Control(pDX, IDC_EDIT_DCC_X_PITCH_LD_TRAY		, m_editDCC_LDTray_X_Pitch);
	DDX_Control(pDX, IDC_EDIT_SPD_Y_PITCH_LD_TRAY		, m_editSPD_LDTray_Y_Pitch);
	DDX_Control(pDX, IDC_EDIT_ACC_Y_PITCH_LD_TRAY		, m_editACC_LDTray_Y_Pitch);
	DDX_Control(pDX, IDC_EDIT_DCC_Y_PITCH_LD_TRAY		, m_editDCC_LDTray_Y_Pitch);


	//Unload Tray PP
	DDX_Text(pDX, IDC_EDIT_SPD_X_UL_TRAY				, m_nSPD_ULTrayPP_X);
	DDX_Text(pDX, IDC_EDIT_ACC_X_UL_TRAY				, m_nACC_ULTrayPP_X);
	DDX_Text(pDX, IDC_EDIT_DCC_X_UL_TRAY				, m_nDCC_ULTrayPP_X);
	DDX_Text   (pDX, IDC_EDIT_SPD_Y_UL_TRAY				, m_nSPD_ULTrayPP_Y);
	DDX_Text   (pDX, IDC_EDIT_ACC_Y_UL_TRAY				, m_nACC_ULTrayPP_Y);
	DDX_Text   (pDX, IDC_EDIT_DCC_Y_UL_TRAY				, m_nDCC_ULTrayPP_Y);
	DDX_Text(pDX, IDC_EDIT_SPD_Z_UL_TRAY				, m_nSPD_ULTrayPP_Z);
	DDX_Text(pDX, IDC_EDIT_ACC_Z_UL_TRAY				, m_nACC_ULTrayPP_Z);
	DDX_Text(pDX, IDC_EDIT_DCC_Z_UL_TRAY				, m_nDCC_ULTrayPP_Z);
	DDX_Text(pDX, IDC_EDIT_SPD_X_PITCH_UL_TRAY			, m_nSPD_ULTrayPP_X_Pitch);
	DDX_Text(pDX, IDC_EDIT_ACC_X_PITCH_UL_TRAY			, m_nACC_ULTrayPP_X_Pitch);
	DDX_Text(pDX, IDC_EDIT_DCC_X_PITCH_UL_TRAY			, m_nDCC_ULTrayPP_X_Pitch);
	DDX_Text(pDX, IDC_EDIT_SPD_Y_PITCH_UL_TRAY			, m_nSPD_ULTrayPP_Y_Pitch);
	DDX_Text(pDX, IDC_EDIT_ACC_Y_PITCH_UL_TRAY			, m_nACC_ULTrayPP_Y_Pitch);
	DDX_Text(pDX, IDC_EDIT_DCC_Y_PITCH_UL_TRAY			, m_nDCC_ULTrayPP_Y_Pitch);
	DDX_Control(pDX, IDC_EDIT_SPD_X_UL_TRAY				, m_editSPD_ULTray_X);
	DDX_Control(pDX, IDC_EDIT_ACC_X_UL_TRAY				, m_editACC_ULTray_X);
	DDX_Control(pDX, IDC_EDIT_DCC_X_UL_TRAY				, m_editDCC_ULTray_X);
	DDX_Control(pDX, IDC_EDIT_SPD_Y_UL_TRAY				, m_editSPD_ULTray_Y);
	DDX_Control(pDX, IDC_EDIT_ACC_Y_UL_TRAY				, m_editACC_ULTray_Y);
	DDX_Control(pDX, IDC_EDIT_DCC_Y_UL_TRAY				, m_editDCC_ULTray_Y);
	DDX_Control(pDX, IDC_EDIT_SPD_Z_UL_TRAY				, m_editSPD_ULTray_Z);
	DDX_Control(pDX, IDC_EDIT_ACC_Z_UL_TRAY				, m_editACC_ULTray_Z);
	DDX_Control(pDX, IDC_EDIT_DCC_Z_UL_TRAY				, m_editDCC_ULTray_Z);
	DDX_Control(pDX, IDC_EDIT_SPD_X_PITCH_UL_TRAY		, m_editSPD_ULTray_X_Pitch);
	DDX_Control(pDX, IDC_EDIT_ACC_X_PITCH_UL_TRAY		, m_editACC_ULTray_X_Pitch);
	DDX_Control(pDX, IDC_EDIT_DCC_X_PITCH_UL_TRAY		, m_editDCC_ULTray_X_Pitch);
	DDX_Control(pDX, IDC_EDIT_SPD_Y_PITCH_UL_TRAY		, m_editSPD_ULTray_Y_Pitch);
	DDX_Control(pDX, IDC_EDIT_ACC_Y_PITCH_UL_TRAY		, m_editACC_ULTray_Y_Pitch);
	DDX_Control(pDX, IDC_EDIT_DCC_Y_PITCH_UL_TRAY		, m_editDCC_ULTray_Y_Pitch);




	DDX_Text(pDX, IDC_EDIT_TRAY_VACUUM_DELAY, m_dTrayPP_Vacuum_LD);
	DDX_Text(pDX, IDC_EDIT_TRAY_BLOW_DELAY, m_dTrayPP_Blow_LD);
	DDX_Text(pDX, IDC_EDIT_TRAY_VACUUM_DELAY2, m_dTrayPP_Vacuum_UD);
	DDX_Text(pDX, IDC_EDIT_TRAY_BLOW_DELAY2, m_dTrayPP_Blow_UD);

	DDX_Text   (pDX, IDC_EDIT_TRAY_CYL_DOWN_WAIT, m_dTrayPP_CylDownWait);	
	DDX_Text   (pDX, IDC_EDIT_TRAY_RETRY	   , m_nRetryCnt);
	DDX_Text(pDX, IDC_EDIT_TRAY_PICK_1STAGE_OFFSET, m_dTrayPP_Pick_1Stage_Offset);
	DDX_Text(pDX, IDC_EDIT_TRAY_PICK_1STAGE_SPEED, m_dTrayPP_Pick_1Stage_Speed);

	DDX_Text(pDX, IDC_EDIT_TRAY_UNLOAD_TRAY_PP_LINEAR_OFFSET, m_dULTrayPP_Linear_Offset);

	DDX_Control(pDX, IDC_EDIT_TRAY_VACUUM_DELAY, m_editTrayPP_Vacuum_LD);
	DDX_Control(pDX, IDC_EDIT_TRAY_BLOW_DELAY  , m_editTrayPP_Blow_LD);
	DDX_Control(pDX, IDC_EDIT_TRAY_VACUUM_DELAY2, m_editTrayPP_Vacuum_UD);
	DDX_Control(pDX, IDC_EDIT_TRAY_BLOW_DELAY2, m_editTrayPP_Blow_UD);

	DDX_Control(pDX, IDC_EDIT_TRAY_RETRY	   , m_editTrayPP_Retry);
	DDX_Control(pDX, IDC_EDIT_TRAY_CYL_DOWN_WAIT, m_editTrayPP_CylDownWait);
	DDX_Control(pDX, IDC_CHECK_PRE_VACUUM_TRAY_PP, m_ChkPreVacuum);
	DDX_Control(pDX, IDC_EDIT_TRAY_PICK_1STAGE_OFFSET, m_editTrayPP_Pick_1Stage_Offset);
	DDX_Control(pDX, IDC_EDIT_TRAY_PICK_1STAGE_SPEED, m_editTrayPP_Pick_1Stage_Speed);

	DDX_Control(pDX, IDC_EDIT_TRAY_UNLOAD_TRAY_PP_LINEAR_OFFSET, m_editULTrayPP_Linear_Offset);
	


	
}


BEGIN_MESSAGE_MAP(CSPD_TM_TRAY_PP, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CSPD_TM_TRAY_PP::OnBnClickedButtonSave)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_X_LD_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditSpdX)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_X_LD_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditAccX)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_X_LD_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditDccX)
// 	ON_EN_SETFOCUS(IDC_EDIT_SPD_Y_LD_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditSpdY)
// 	ON_EN_SETFOCUS(IDC_EDIT_ACC_Y_LD_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditAccY)
// 	ON_EN_SETFOCUS(IDC_EDIT_DCC_Y_LD_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditDccY)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_Z_LD_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditSpdZ)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_Z_LD_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditAccZ)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_Z_LD_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditDccZ)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_X_PITCH_LD_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditSpdXPitch)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_X_PITCH_LD_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditAccXPitch)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_X_PITCH_LD_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditDccXPitch)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_Y_PITCH_LD_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditSpdYPitch)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_Y_PITCH_LD_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditAccYPitch)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_Y_PITCH_LD_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditDccYPitch)
	ON_EN_SETFOCUS(IDC_EDIT_TRAY_VACUUM_DELAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditTrayVacuumDelay)
	ON_EN_SETFOCUS(IDC_EDIT_TRAY_BLOW_DELAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditTrayBlowDelay)
	ON_EN_SETFOCUS(IDC_EDIT_TRAY_RETRY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditTrayRetry)
	ON_EN_SETFOCUS(IDC_EDIT_TRAY_CYL_DOWN_WAIT, &CSPD_TM_TRAY_PP::OnEnSetfocusEditTrayCylDownWait)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_X_UL_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditSpdXUlTray)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_X_UL_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditAccXUlTray)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_X_UL_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditDccXUlTray)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_Y_UL_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditSpdYUlTray)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_Y_UL_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditAccYUlTray)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_Y_UL_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditDccYUlTray)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_Z_UL_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditSpdZUlTray)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_Z_UL_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditAccZUlTray)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_Z_UL_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditDccZUlTray)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_X_PITCH_UL_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditSpdXPitchUlTray)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_X_PITCH_UL_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditAccXPitchUlTray)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_X_PITCH_UL_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditDccXPitchUlTray)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_Y_PITCH_UL_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditSpdYPitchUlTray)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_Y_PITCH_UL_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditAccYPitchUlTray)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_Y_PITCH_UL_TRAY, &CSPD_TM_TRAY_PP::OnEnSetfocusEditDccYPitchUlTray)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_Y_GRIPPER, &CSPD_TM_TRAY_PP::OnEnSetfocusEditSpdYGripper)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_Y_GRIPPER, &CSPD_TM_TRAY_PP::OnEnSetfocusEditAccYGripper)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_Y_GRIPPER, &CSPD_TM_TRAY_PP::OnEnSetfocusEditDccYGripper)
	ON_EN_SETFOCUS(IDC_EDIT_TRAY_VACUUM_DELAY2, &CSPD_TM_TRAY_PP::OnEnSetfocusEditTrayVacuumDelay2)
	ON_EN_SETFOCUS(IDC_EDIT_TRAY_BLOW_DELAY2, &CSPD_TM_TRAY_PP::OnEnSetfocusEditTrayBlowDelay2)
	ON_EN_SETFOCUS(IDC_EDIT_TRAY_PICK_1STAGE_OFFSET, &CSPD_TM_TRAY_PP::OnEnSetfocusEditTrayPick1stageOffset)
	ON_EN_SETFOCUS(IDC_EDIT_TRAY_PICK_1STAGE_SPEED, &CSPD_TM_TRAY_PP::OnEnSetfocusEditTrayPick1stageSpeed)
	ON_EN_SETFOCUS(IDC_EDIT_TRAY_UNLOAD_TRAY_PP_LINEAR_OFFSET, &CSPD_TM_TRAY_PP::OnEnSetfocusEditTrayUnloadTrayPpLinearOffset)
END_MESSAGE_MAP()


// CSPD_TM_TRAY_PP 메시지 처리기입니다.


BOOL CSPD_TM_TRAY_PP::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnBtnGUI();
	OnEditBoxGUI();

	int nChk = (int)g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TRAY_HAND_PRE_VACUUM);
	m_ChkPreVacuum.SetCheck(nChk);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSPD_TM_TRAY_PP::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);
}

void CSPD_TM_TRAY_PP::OnEditBoxGUI()
{
	//Load Tray PP
	m_editSPD_LDTray_X			.SetBackColor(COLOR_WWHITE);
	m_editACC_LDTray_X			.SetBackColor(COLOR_WWHITE);
	m_editDCC_LDTray_X			.SetBackColor(COLOR_WWHITE);
 	m_editSPD_Gripper_Y			.SetBackColor(COLOR_WWHITE);
 	m_editACC_Gripper_Y			.SetBackColor(COLOR_WWHITE);
 	m_editDCC_Gripper_Y			.SetBackColor(COLOR_WWHITE);
	m_editSPD_LDTray_Z			.SetBackColor(COLOR_WWHITE);
	m_editACC_LDTray_Z			.SetBackColor(COLOR_WWHITE);
	m_editDCC_LDTray_Z			.SetBackColor(COLOR_WWHITE);
	m_editSPD_LDTray_X_Pitch	.SetBackColor(COLOR_WWHITE);
	m_editACC_LDTray_X_Pitch	.SetBackColor(COLOR_WWHITE);
	m_editDCC_LDTray_X_Pitch	.SetBackColor(COLOR_WWHITE);
	m_editSPD_LDTray_Y_Pitch	.SetBackColor(COLOR_WWHITE);
	m_editACC_LDTray_Y_Pitch	.SetBackColor(COLOR_WWHITE);
	m_editDCC_LDTray_Y_Pitch	.SetBackColor(COLOR_WWHITE);


	m_editSPD_LDTray_X.SetTextSize(30);
	m_editACC_LDTray_X.SetTextSize(30);
	m_editDCC_LDTray_X.SetTextSize(30);
	m_editSPD_Gripper_Y.SetTextSize(30);
	m_editACC_Gripper_Y.SetTextSize(30);
	m_editDCC_Gripper_Y	.SetTextSize(30);
	m_editSPD_LDTray_Z.SetTextSize(30);
	m_editACC_LDTray_Z.SetTextSize(30);
	m_editDCC_LDTray_Z.SetTextSize(30);
	m_editSPD_LDTray_X_Pitch.SetTextSize(30);
	m_editACC_LDTray_X_Pitch.SetTextSize(30);
	m_editDCC_LDTray_X_Pitch.SetTextSize(30);
	m_editSPD_LDTray_Y_Pitch.SetTextSize(30);
	m_editACC_LDTray_Y_Pitch.SetTextSize(30);
	m_editDCC_LDTray_Y_Pitch.SetTextSize(30);


	//Unload Tray PP
	m_editSPD_ULTray_X			.SetBackColor(COLOR_WWHITE);
	m_editACC_ULTray_X			.SetBackColor(COLOR_WWHITE);
	m_editDCC_ULTray_X			.SetBackColor(COLOR_WWHITE);
	m_editSPD_ULTray_Y			.SetBackColor(COLOR_WWHITE);
	m_editACC_ULTray_Y			.SetBackColor(COLOR_WWHITE);
	m_editDCC_ULTray_Y			.SetBackColor(COLOR_WWHITE);
	m_editSPD_ULTray_Z			.SetBackColor(COLOR_WWHITE);
	m_editACC_ULTray_Z			.SetBackColor(COLOR_WWHITE);
	m_editDCC_ULTray_Z			.SetBackColor(COLOR_WWHITE);
	m_editSPD_ULTray_X_Pitch	.SetBackColor(COLOR_WWHITE);
	m_editACC_ULTray_X_Pitch	.SetBackColor(COLOR_WWHITE);
	m_editDCC_ULTray_X_Pitch	.SetBackColor(COLOR_WWHITE);
	m_editSPD_ULTray_Y_Pitch	.SetBackColor(COLOR_WWHITE);
	m_editACC_ULTray_Y_Pitch	.SetBackColor(COLOR_WWHITE);
	m_editDCC_ULTray_Y_Pitch	.SetBackColor(COLOR_WWHITE);


	m_editSPD_ULTray_X.SetTextSize(30);
	m_editACC_ULTray_X.SetTextSize(30);
	m_editDCC_ULTray_X.SetTextSize(30);
	m_editSPD_ULTray_Y.SetTextSize(30);
	m_editACC_ULTray_Y.SetTextSize(30);
	m_editDCC_ULTray_Y.SetTextSize(30);
	m_editSPD_ULTray_Z.SetTextSize(30);
	m_editACC_ULTray_Z.SetTextSize(30);
	m_editDCC_ULTray_Z.SetTextSize(30);
	m_editSPD_ULTray_X_Pitch.SetTextSize(30);
	m_editACC_ULTray_X_Pitch.SetTextSize(30);
	m_editDCC_ULTray_X_Pitch.SetTextSize(30);
	m_editSPD_ULTray_Y_Pitch.SetTextSize(30);
	m_editACC_ULTray_Y_Pitch.SetTextSize(30);
	m_editDCC_ULTray_Y_Pitch.SetTextSize(30);



	m_editTrayPP_Vacuum_LD.SetBackColor(COLOR_WWHITE);
	m_editTrayPP_Blow_LD.SetBackColor(COLOR_WWHITE);
	m_editTrayPP_Vacuum_UD.SetBackColor(COLOR_WWHITE);
	m_editTrayPP_Blow_UD.SetBackColor(COLOR_WWHITE);
	m_editTrayPP_Retry.SetBackColor(COLOR_WWHITE);
	m_editTrayPP_CylDownWait.SetBackColor(COLOR_WWHITE);
	m_editTrayPP_Pick_1Stage_Offset.SetBackColor(COLOR_WWHITE);
	m_editTrayPP_Pick_1Stage_Speed.SetBackColor(COLOR_WWHITE);
	m_editULTrayPP_Linear_Offset.SetBackColor(COLOR_WWHITE);
	

	m_editTrayPP_Vacuum_LD		.SetTextSize(30);
	m_editTrayPP_Blow_LD		.SetTextSize(30);
	m_editTrayPP_Vacuum_UD.SetTextSize(30);
	m_editTrayPP_Blow_UD.SetTextSize(30);

	m_editTrayPP_Retry		.SetTextSize(30);
	m_editTrayPP_CylDownWait.SetTextSize(30);
	m_editTrayPP_Pick_1Stage_Offset.SetTextSize(30);
	m_editTrayPP_Pick_1Stage_Speed.SetTextSize(30);
	m_editULTrayPP_Linear_Offset.SetTextSize(30);
}

void CSPD_TM_TRAY_PP::OnBnClickedButtonSave()
{
	UpdateData(TRUE);

	//Load Tray PP
	g_DMCont.m_dmTrayPP.SN(NDM5_SPD_LD_TrayPP_X		 , m_nSPD_LDTrayPP_X      );
	g_DMCont.m_dmTrayPP.SN(NDM5_ACC_LD_TrayPP_X      , m_nACC_LDTrayPP_X      );
	g_DMCont.m_dmTrayPP.SN(NDM5_DCC_LD_TrayPP_X      , m_nDCC_LDTrayPP_X      );
 	g_DMCont.m_dmTrayPP.SN(NDM5_SPD_Gripper_Y		 , m_nSPD_Gripper_Y      );
 	g_DMCont.m_dmTrayPP.SN(NDM5_ACC_Gripper_Y		 , m_nACC_Gripper_Y      );
 	g_DMCont.m_dmTrayPP.SN(NDM5_DCC_Gripper_Y		 , m_nDCC_Gripper_Y      );
	g_DMCont.m_dmTrayPP.SN(NDM5_SPD_LD_TrayPP_Z      , m_nSPD_LDTrayPP_Z      );
	g_DMCont.m_dmTrayPP.SN(NDM5_ACC_LD_TrayPP_Z      , m_nACC_LDTrayPP_Z      );
	g_DMCont.m_dmTrayPP.SN(NDM5_DCC_LD_TrayPP_Z      , m_nDCC_LDTrayPP_Z      );
	g_DMCont.m_dmTrayPP.SN(NDM5_SPD_LD_TrayPP_X_Pitch, m_nSPD_LDTrayPP_X_Pitch);
	g_DMCont.m_dmTrayPP.SN(NDM5_ACC_LD_TrayPP_X_Pitch, m_nACC_LDTrayPP_X_Pitch);
	g_DMCont.m_dmTrayPP.SN(NDM5_DCC_LD_TrayPP_X_Pitch, m_nDCC_LDTrayPP_X_Pitch);
	g_DMCont.m_dmTrayPP.SN(NDM5_SPD_LD_TrayPP_Y_Pitch, m_nSPD_LDTrayPP_Y_Pitch);
	g_DMCont.m_dmTrayPP.SN(NDM5_ACC_LD_TrayPP_Y_Pitch, m_nACC_LDTrayPP_Y_Pitch);
	g_DMCont.m_dmTrayPP.SN(NDM5_DCC_LD_TrayPP_Y_Pitch, m_nDCC_LDTrayPP_Y_Pitch);

	//Unload Tray PP
	g_DMCont.m_dmTrayPP.SN(NDM5_SPD_UL_TrayPP_X		, m_nSPD_ULTrayPP_X);
	g_DMCont.m_dmTrayPP.SN(NDM5_ACC_UL_TrayPP_X		, m_nACC_ULTrayPP_X);
	g_DMCont.m_dmTrayPP.SN(NDM5_DCC_UL_TrayPP_X		, m_nDCC_ULTrayPP_X);
	g_DMCont.m_dmTrayPP.SN(NDM5_SPD_UL_TrayPP_Y     , m_nSPD_ULTrayPP_Y);
	g_DMCont.m_dmTrayPP.SN(NDM5_ACC_UL_TrayPP_Y     , m_nACC_ULTrayPP_Y);
	g_DMCont.m_dmTrayPP.SN(NDM5_DCC_UL_TrayPP_Y     , m_nDCC_ULTrayPP_Y);
	g_DMCont.m_dmTrayPP.SN(NDM5_SPD_UL_TrayPP_Z		, m_nSPD_ULTrayPP_Z);
	g_DMCont.m_dmTrayPP.SN(NDM5_ACC_UL_TrayPP_Z		, m_nACC_ULTrayPP_Z);
	g_DMCont.m_dmTrayPP.SN(NDM5_DCC_UL_TrayPP_Z		, m_nDCC_ULTrayPP_Z);
	g_DMCont.m_dmTrayPP.SN(NDM5_SPD_UL_TrayPP_X_Pitch, m_nSPD_ULTrayPP_X_Pitch);
	g_DMCont.m_dmTrayPP.SN(NDM5_ACC_UL_TrayPP_X_Pitch, m_nACC_ULTrayPP_X_Pitch);
	g_DMCont.m_dmTrayPP.SN(NDM5_DCC_UL_TrayPP_X_Pitch, m_nDCC_ULTrayPP_X_Pitch);
	g_DMCont.m_dmTrayPP.SN(NDM5_SPD_UL_TrayPP_Y_Pitch, m_nSPD_ULTrayPP_Y_Pitch);
	g_DMCont.m_dmTrayPP.SN(NDM5_ACC_UL_TrayPP_Y_Pitch, m_nACC_ULTrayPP_Y_Pitch);
	g_DMCont.m_dmTrayPP.SN(NDM5_DCC_UL_TrayPP_Y_Pitch, m_nDCC_ULTrayPP_Y_Pitch);


	g_DMCont.m_dmTrayPP.SN(NDM5_TM_LDTrayPP_Vacuum  , (int)(m_dTrayPP_Vacuum_LD *1000));
	g_DMCont.m_dmTrayPP.SN(NDM5_TM_LDTrayPP_Blow    , (int)(m_dTrayPP_Blow_LD   *1000));
	g_DMCont.m_dmTrayPP.SN(NDM5_TM_UDTrayPP_Vacuum, (int)(m_dTrayPP_Vacuum_UD * 1000));
	g_DMCont.m_dmTrayPP.SN(NDM5_TM_UDTrayPP_Blow, (int)(m_dTrayPP_Blow_UD * 1000));

	g_DMCont.m_dmTrayPP.SN(NDM5_LDTrayPP_RetryCnt	  , m_nRetryCnt);
	g_DMCont.m_dmTrayPP.SN(NDM5_TM_LDTrayPP_CylDownWait, (int)(m_dTrayPP_CylDownWait*1000));
	g_DMCont.m_dmTrayPP.SN(NDM5_TM_LDTrayPP_Pick_1Stage_Offset,(int)(m_dTrayPP_Pick_1Stage_Offset * 1000.0));
	g_DMCont.m_dmTrayPP.SN(NDM5_TM_LDTrayPP_Pick_1Stage_Speed, (int)(m_dTrayPP_Pick_1Stage_Speed * 1000.0));

	g_DMCont.m_dmTrayPP.SN(NDM5_TM_UDTrayPP_Linear_Offset, (int)(m_dULTrayPP_Linear_Offset * 1000.0));
	
	 
	char szCokName[512] = {0,};
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
	
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_TRAY_HAND_PRE_VACUUM, (BOOL)m_ChkPreVacuum.GetCheck());

	ST_COK_INFO   stCokData;
	g_COK_FileMgr.LoadCokInfo(szCokName, stCokData);

	ST_COK_INFO stBeforeCokData = stCokData;
	

	int nSpdPos[MAX_TRAY_PP_CNT] = { NDM5_SPD_LD_TrayPP_X, NDM5_SPD_UL_TrayPP_X };

	for (int i = 0; i <= RIDX_TRAY_PP_2; i++)
	{
		stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_X].nSPD = g_DMCont.m_dmTrayPP.GN(nSpdPos[i]);
		stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_X].nACC = g_DMCont.m_dmTrayPP.GN(nSpdPos[i]+1);
		stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_X].nDCC = g_DMCont.m_dmTrayPP.GN(nSpdPos[i]+2);
		stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_Y].nSPD = g_DMCont.m_dmTrayPP.GN(nSpdPos[i]+3);
		stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_Y].nACC = g_DMCont.m_dmTrayPP.GN(nSpdPos[i]+4);
		stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_Y].nDCC = g_DMCont.m_dmTrayPP.GN(nSpdPos[i]+5);
		stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_Z].nSPD = g_DMCont.m_dmTrayPP.GN(nSpdPos[i]+6);
		stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_Z].nACC = g_DMCont.m_dmTrayPP.GN(nSpdPos[i]+7);
		stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_Z].nDCC = g_DMCont.m_dmTrayPP.GN(nSpdPos[i]+8);
		stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_X_Pitch].nSPD = g_DMCont.m_dmTrayPP.GN(nSpdPos[i]+9);
		stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_X_Pitch].nACC = g_DMCont.m_dmTrayPP.GN(nSpdPos[i]+10);
		stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_X_Pitch].nDCC = g_DMCont.m_dmTrayPP.GN(nSpdPos[i]+11);
		stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_Y_Pitch].nSPD = g_DMCont.m_dmTrayPP.GN(nSpdPos[i]+12);
		stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_Y_Pitch].nACC = g_DMCont.m_dmTrayPP.GN(nSpdPos[i]+13);
		stCokData.aTrayPP_SPD[i].aAxisForm[eAxis_Y_Pitch].nDCC = g_DMCont.m_dmTrayPP.GN(nSpdPos[i]+14);
	}

	stCokData.aTrayPP_Tm[eTrayPP_1_LD].nBlowTm   = (int)(m_dTrayPP_Blow_LD *1000);
	stCokData.aTrayPP_Tm[eTrayPP_1_LD].nVacuumTm = (int)(m_dTrayPP_Vacuum_LD *1000);
	stCokData.aTrayPP_Tm[eTrayPP_1_LD].nRetryCnt    = m_nRetryCnt;
	stCokData.aTrayPP_Tm[eTrayPP_1_LD].nCylDownWaitTm = (int)(m_dTrayPP_CylDownWait*1000);
	stCokData.aTrayPP_Tm[eTrayPP_1_LD].nPick_1Stage_Offset = (int)(m_dTrayPP_Pick_1Stage_Offset * 1000);
	stCokData.aTrayPP_Tm[eTrayPP_1_LD].nPick_1Stage_Speed = (int)(m_dTrayPP_Pick_1Stage_Speed * 1000);



	stCokData.aTrayPP_Tm[eTrayPP_2_UD].nBlowTm = (int)(m_dTrayPP_Blow_UD * 1000);
	stCokData.aTrayPP_Tm[eTrayPP_2_UD].nVacuumTm = (int)(m_dTrayPP_Vacuum_UD * 1000);
	stCokData.aTrayPP_Tm[eTrayPP_2_UD].nRetryCnt = m_nRetryCnt;
	stCokData.aTrayPP_Tm[eTrayPP_2_UD].nCylDownWaitTm = (int)(m_dTrayPP_CylDownWait * 1000);
	stCokData.aTrayPP_Tm[eTrayPP_2_UD].nLinear_Offset = (int)(m_dULTrayPP_Linear_Offset * 1000);


	

 	g_COK_FileMgr.SaveCokInfo(szCokName, &stCokData);


	ST_BASIC_INFO   stBasicData;
	g_COK_FileMgr.LoadBasicInfo(szCokName, stBasicData);

	stBasicData.bMD_TrayPP_PreVacuum = (BOOL)m_ChkPreVacuum.GetCheck();

	g_COK_FileMgr.SaveBasicInfo(szCokName, &stBasicData);

	std::vector<std::pair<CString,std::pair<double,double>>> vData;

	stBeforeCokData.IsSameTrayPP(stCokData, vData);
	
	if(!(vData.empty()))
	{
		TryPPSpeedTiemChangeLog(vData);
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditSpdX()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nSPD_LDTrayPP_X, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Tray PP X Speed", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_SPD_X_LD_TRAY)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditAccX()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nACC_LDTrayPP_X, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Tray PP X Accel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_ACC_X_LD_TRAY)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditDccX()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nDCC_LDTrayPP_X, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Tray PP X Decel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DCC_X_LD_TRAY)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditSpdYGripper()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_Gripper_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Gripper Y Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_Y_GRIPPER)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditAccYGripper()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_Gripper_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Gripper Y Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_Y_GRIPPER)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditDccYGripper()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_Gripper_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Gripper Y Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_Y_GRIPPER)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}



void CSPD_TM_TRAY_PP::OnEnSetfocusEditSpdZ()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nSPD_LDTrayPP_Z, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Tray PP Z Speed", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_SPD_Z_LD_TRAY)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditAccZ()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nACC_LDTrayPP_Z, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Tray PP Z Accel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_ACC_Z_LD_TRAY)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditDccZ()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nDCC_LDTrayPP_Z, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Tray PP Z Decel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DCC_Z_LD_TRAY)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditSpdXPitch()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nSPD_LDTrayPP_X_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Tray PP X Pitch Speed", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_SPD_X_PITCH_LD_TRAY)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditAccXPitch()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nACC_LDTrayPP_X_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Tray PP X Pitch Accel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_ACC_X_PITCH_LD_TRAY)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditDccXPitch()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nDCC_LDTrayPP_X_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Tray PP X Pitch Decel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DCC_X_PITCH_LD_TRAY)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditSpdYPitch()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nSPD_LDTrayPP_Y_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Tray PP Y Pitch Speed", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_SPD_Y_PITCH_LD_TRAY)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditAccYPitch()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nACC_LDTrayPP_Y_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Tray PP Y Pitch Accel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_ACC_Y_PITCH_LD_TRAY)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditDccYPitch()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nDCC_LDTrayPP_Y_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Load Tray PP Y Pitch Decel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DCC_Y_PITCH_LD_TRAY)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditSpdXUlTray()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_ULTrayPP_X, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "UnLoad Tray PP X Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_X_UL_TRAY)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditAccXUlTray()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_ULTrayPP_X, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "UnLoad Tray PP X Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_X_UL_TRAY)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditDccXUlTray()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_ULTrayPP_X, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "UnLoad Tray PP X Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_X_UL_TRAY)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditSpdYUlTray()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_ULTrayPP_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "UnLoad Tray PP Y Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_Y_UL_TRAY)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditAccYUlTray()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_ULTrayPP_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "UnLoad Tray PP Y Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_Y_UL_TRAY)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditDccYUlTray()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_ULTrayPP_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "UnLoad Tray PP Y Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_Y_UL_TRAY)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditSpdZUlTray()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_ULTrayPP_Z, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "UnLoad Tray PP Z Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_Z_UL_TRAY)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditAccZUlTray()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_ULTrayPP_Z, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "UnLoad Tray PP Z Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_Z_UL_TRAY)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditDccZUlTray()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_ULTrayPP_Z, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "UnLoad Tray PP Z Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_Z_UL_TRAY)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditSpdXPitchUlTray()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_ULTrayPP_X_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "UnLoad Tray PP X Pitch Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_X_PITCH_UL_TRAY)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditAccXPitchUlTray()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_ULTrayPP_X_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "UnLoad Tray PP X Pitch Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_X_PITCH_UL_TRAY)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditDccXPitchUlTray()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_ULTrayPP_X_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "UnLoad Tray PP X Pitch Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_X_PITCH_UL_TRAY)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditSpdYPitchUlTray()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_ULTrayPP_Y_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "UnLoad Tray PP Y Pitch Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_Y_PITCH_UL_TRAY)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditAccYPitchUlTray()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_ULTrayPP_Y_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "UnLoad Tray PP Y Pitch Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_Y_PITCH_UL_TRAY)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditDccYPitchUlTray()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_ULTrayPP_Y_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "UnLoad Tray PP Y Pitch Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_Y_PITCH_UL_TRAY)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}




void CSPD_TM_TRAY_PP::OnEnSetfocusEditTrayVacuumDelay()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTrayPP_Vacuum_LD);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray Load PP Vacuum Time", "20", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_TRAY_VACUUM_DELAY)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditTrayBlowDelay()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTrayPP_Blow_LD);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray Load PP Blow Time", "20", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_TRAY_BLOW_DELAY)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditTrayRetry()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nRetryCnt, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray PP Retry Count", "5", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_TRAY_RETRY)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditTrayCylDownWait()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTrayPP_CylDownWait);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray Load PP Cylinder Down Wait", "20", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_TRAY_CYL_DOWN_WAIT)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}

void CSPD_TM_TRAY_PP::MakeLog(LPCTSTR fmt, ...)
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


void CSPD_TM_TRAY_PP::TryPPSpeedTiemChangeLog(std::vector<std::pair<CString,std::pair<double,double>>>& vData)
{
	for(int i = 0 ; i< (int)vData.size(); i++)
	{
		std::pair<CString,std::pair<double,double>> p;

		p = vData[i];

		MakeLog("[MENU = Tray PP Speed & Time, DATA NAME = %s ][BEFORE=%.2f, AFTER=%.2f]", p.first, p.second.first, p.second.second);
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditTrayVacuumDelay2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTrayPP_Vacuum_UD);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray Unload PP Vacuum Time", "20", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_TRAY_VACUUM_DELAY2)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditTrayBlowDelay2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTrayPP_Blow_UD);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray Unload PP Blow Time", "20", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_TRAY_BLOW_DELAY2)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditTrayPick1stageOffset()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTrayPP_Pick_1Stage_Offset);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray Pick 1Stage Offset", "10", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_TRAY_PICK_1STAGE_OFFSET)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditTrayPick1stageSpeed()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTrayPP_Pick_1Stage_Speed);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Tray Pick 1Stage Speed", "100", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_TRAY_PICK_1STAGE_SPEED)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TRAY_PP::OnEnSetfocusEditTrayUnloadTrayPpLinearOffset()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dULTrayPP_Linear_Offset);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "UL Tray PP Linear Offset", "1.5", "-1.5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_TRAY_UNLOAD_TRAY_PP_LINEAR_OFFSET)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}
