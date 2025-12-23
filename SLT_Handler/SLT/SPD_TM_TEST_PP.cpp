// SPD_TM_TEST_PP.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "SPD_TM_TEST_PP.h"
#include "afxdialogex.h"


// CSPD_TM_TEST_PP 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSPD_TM_TEST_PP, CDialog)

CSPD_TM_TEST_PP::CSPD_TM_TEST_PP(CWnd* pParent /*=NULL*/)
	: CDialog(CSPD_TM_TEST_PP::IDD, pParent)
{
	m_nSPD_TestPP1_X1	= g_DMCont.m_dmTestPP.GN(NDM4_SPD_TestPP1_X1);
	m_nACC_TestPP1_X1	= g_DMCont.m_dmTestPP.GN(NDM4_ACC_TestPP1_X1);
	m_nDCC_TestPP1_X1	= g_DMCont.m_dmTestPP.GN(NDM4_DCC_TestPP1_X1);
	m_nSPD_TestPP1_X2   = g_DMCont.m_dmTestPP.GN(NDM4_SPD_TestPP1_X2);
	m_nACC_TestPP1_X2   = g_DMCont.m_dmTestPP.GN(NDM4_ACC_TestPP1_X2);
	m_nDCC_TestPP1_X2   = g_DMCont.m_dmTestPP.GN(NDM4_DCC_TestPP1_X2);
	m_nSPD_TestPP1_Y	= g_DMCont.m_dmTestPP.GN(NDM4_SPD_TestPP1_Y);
	m_nACC_TestPP1_Y	= g_DMCont.m_dmTestPP.GN(NDM4_ACC_TestPP1_Y);
	m_nDCC_TestPP1_Y	= g_DMCont.m_dmTestPP.GN(NDM4_DCC_TestPP1_Y);
	m_nSPD_TestPP1_Z1	= g_DMCont.m_dmTestPP.GN(NDM4_SPD_TestPP1_Z1);
	m_nACC_TestPP1_Z1	= g_DMCont.m_dmTestPP.GN(NDM4_ACC_TestPP1_Z1);
	m_nDCC_TestPP1_Z1	= g_DMCont.m_dmTestPP.GN(NDM4_DCC_TestPP1_Z1);
	m_nSPD_TestPP1_Z2	= g_DMCont.m_dmTestPP.GN(NDM4_SPD_TestPP1_Z2);
	m_nACC_TestPP1_Z2	= g_DMCont.m_dmTestPP.GN(NDM4_ACC_TestPP1_Z2);
	m_nDCC_TestPP1_Z2	= g_DMCont.m_dmTestPP.GN(NDM4_DCC_TestPP1_Z2);
	m_nSPD_TestPP1_X1_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_SPD_TestPP_1_Pitch_X1);
	m_nACC_TestPP1_X1_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_ACC_TestPP_1_Pitch_X1);
	m_nDCC_TestPP1_X1_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_DCC_TestPP_1_Pitch_X1);
	m_nSPD_TestPP1_X2_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_SPD_TestPP_1_Pitch_X2);
	m_nACC_TestPP1_X2_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_ACC_TestPP_1_Pitch_X2);
	m_nDCC_TestPP1_X2_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_DCC_TestPP_1_Pitch_X2);
	m_nSPD_TestPP1_Y1_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_SPD_TestPP_1_Pitch_Y1);
	m_nACC_TestPP1_Y1_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_ACC_TestPP_1_Pitch_Y1);
	m_nDCC_TestPP1_Y1_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_DCC_TestPP_1_Pitch_Y1);
	m_nSPD_TestPP1_Y2_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_SPD_TestPP_1_Pitch_Y2);
	m_nACC_TestPP1_Y2_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_ACC_TestPP_1_Pitch_Y2);
	m_nDCC_TestPP1_Y2_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_DCC_TestPP_1_Pitch_Y2);


	m_nSPD_TestPP2_X1	= g_DMCont.m_dmTestPP.GN(NDM4_SPD_TestPP2_X1);
	m_nACC_TestPP2_X1	= g_DMCont.m_dmTestPP.GN(NDM4_ACC_TestPP2_X1);
	m_nDCC_TestPP2_X1	= g_DMCont.m_dmTestPP.GN(NDM4_DCC_TestPP2_X1);
	m_nSPD_TestPP2_X2   = g_DMCont.m_dmTestPP.GN(NDM4_SPD_TestPP2_X2);
	m_nACC_TestPP2_X2   = g_DMCont.m_dmTestPP.GN(NDM4_ACC_TestPP2_X2);
	m_nDCC_TestPP2_X2   = g_DMCont.m_dmTestPP.GN(NDM4_DCC_TestPP2_X2);
	m_nSPD_TestPP2_Y	= g_DMCont.m_dmTestPP.GN(NDM4_SPD_TestPP2_Y);
	m_nACC_TestPP2_Y	= g_DMCont.m_dmTestPP.GN(NDM4_ACC_TestPP2_Y);
	m_nDCC_TestPP2_Y	= g_DMCont.m_dmTestPP.GN(NDM4_DCC_TestPP2_Y);
	m_nSPD_TestPP2_Z1	= g_DMCont.m_dmTestPP.GN(NDM4_SPD_TestPP2_Z1);
	m_nACC_TestPP2_Z1	= g_DMCont.m_dmTestPP.GN(NDM4_ACC_TestPP2_Z1);
	m_nDCC_TestPP2_Z1	= g_DMCont.m_dmTestPP.GN(NDM4_DCC_TestPP2_Z1);
	m_nSPD_TestPP2_Z2 = g_DMCont.m_dmTestPP.GN(NDM4_SPD_TestPP2_Z2);
	m_nACC_TestPP2_Z2 = g_DMCont.m_dmTestPP.GN(NDM4_ACC_TestPP2_Z2);
	m_nDCC_TestPP2_Z2 = g_DMCont.m_dmTestPP.GN(NDM4_DCC_TestPP2_Z2);
	m_nSPD_TestPP2_X1_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_SPD_TestPP_2_Pitch_X1);
	m_nACC_TestPP2_X1_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_ACC_TestPP_2_Pitch_X1);
	m_nDCC_TestPP2_X1_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_DCC_TestPP_2_Pitch_X1);
	m_nSPD_TestPP2_X2_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_SPD_TestPP_2_Pitch_X2);
	m_nACC_TestPP2_X2_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_ACC_TestPP_2_Pitch_X2);
	m_nDCC_TestPP2_X2_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_DCC_TestPP_2_Pitch_X2);
	m_nSPD_TestPP2_Y1_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_SPD_TestPP_2_Pitch_Y1);
	m_nACC_TestPP2_Y1_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_ACC_TestPP_2_Pitch_Y1);
	m_nDCC_TestPP2_Y1_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_DCC_TestPP_2_Pitch_Y1);
	m_nSPD_TestPP2_Y2_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_SPD_TestPP_2_Pitch_Y2);
	m_nACC_TestPP2_Y2_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_ACC_TestPP_2_Pitch_Y2);
	m_nDCC_TestPP2_Y2_Pitch = g_DMCont.m_dmTestPP.GN(NDM5_DCC_TestPP_2_Pitch_Y2);

	m_dTestPP1_Vacuum	= (double)(g_DMCont.m_dmTestPP.GN(NDM4_TM_TestPP1_Vacuum)/1000.0);
	m_dTestPP1_Blow		= (double)(g_DMCont.m_dmTestPP.GN(NDM4_TM_TestPP1_Blow)  /1000.0);
	m_dTestPP2_Vacuum	= (double)(g_DMCont.m_dmTestPP.GN(NDM4_TM_TestPP2_Vacuum)/1000.0);
	m_dTestPP2_Blow		= (double)(g_DMCont.m_dmTestPP.GN(NDM4_TM_TestPP2_Blow)  /1000.0);

	m_nRetryCountPP1    = g_DMCont.m_dmTestPP.GN(NDM4_TestPP1_RetryCnt);
	m_nRetryCountPP2    = g_DMCont.m_dmTestPP.GN(NDM4_TestPP2_RetryCnt);

	m_dTestPP1_Pick_Retry_Offset = g_DMCont.m_dmTestPP.GD(DDM4_TestPP1_Pick_Retry_Offset) /1000.0;
	m_dTestPP2_Pick_Retry_Offset = g_DMCont.m_dmTestPP.GD(DDM4_TestPP2_Pick_Retry_Offset) / 1000.0;

	m_dTestPP1_Picker_Wait_Time = g_DMCont.m_dmTestPP.GD(DDM4_TestPP1_Picker_Wait_Time) / 1000.0;
	m_dTestPP2_Picker_Wait_Time = g_DMCont.m_dmTestPP.GD(DDM4_TestPP2_Picker_Wait_Time) / 1000.0;

	
	m_bTestSiteVisionOn = m_bBackVisionOnOff = g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_VisionUse);
	
	m_bReTouch = g_DMCont.m_dmHandlerOpt.GB(BDM11_TEST_SITE_RETOUCH);
	m_dReTouch_Dis = (double)(g_DMCont.m_dmHandlerOpt.GN(NDM11_TEST_SITE_RETOUCH_DIS) / 1000.0);

}

CSPD_TM_TEST_PP::~CSPD_TM_TEST_PP()
{
}

void CSPD_TM_TEST_PP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_BtnSave);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Text(pDX, IDC_EDIT_SPD_X1_TEST1, m_nSPD_TestPP1_X1);
	DDX_Text(pDX, IDC_EDIT_ACC_X1_TEST1, m_nACC_TestPP1_X1);
	DDX_Text(pDX, IDC_EDIT_DCC_X1_TEST1, m_nDCC_TestPP1_X1);
	DDX_Text(pDX, IDC_EDIT_SPD_X2_TEST1, m_nSPD_TestPP1_X2);
	DDX_Text(pDX, IDC_EDIT_ACC_X2_TEST1, m_nACC_TestPP1_X2);
	DDX_Text(pDX, IDC_EDIT_DCC_X2_TEST1, m_nDCC_TestPP1_X2);
	DDX_Text(pDX, IDC_EDIT_SPD_Y_TEST1, m_nSPD_TestPP1_Y);
	DDX_Text(pDX, IDC_EDIT_ACC_Y_TEST1, m_nACC_TestPP1_Y);
	DDX_Text(pDX, IDC_EDIT_DCC_Y_TEST1, m_nDCC_TestPP1_Y);
	DDX_Text(pDX, IDC_EDIT_SPD_Z1_TEST1, m_nSPD_TestPP1_Z1);
	DDX_Text(pDX, IDC_EDIT_ACC_Z1_TEST1, m_nACC_TestPP1_Z1);
	DDX_Text(pDX, IDC_EDIT_DCC_Z1_TEST1, m_nDCC_TestPP1_Z1);
	DDX_Text(pDX, IDC_EDIT_SPD_Z2_TEST1, m_nSPD_TestPP1_Z2);
	DDX_Text(pDX, IDC_EDIT_ACC_Z2_TEST1, m_nACC_TestPP1_Z2);
	DDX_Text(pDX, IDC_EDIT_DCC_Z2_TEST1, m_nDCC_TestPP1_Z2);
	DDX_Text(pDX, IDC_EDIT_SPD_X1_PITCH_TEST1, m_nSPD_TestPP1_X1_Pitch);
	DDX_Text(pDX, IDC_EDIT_ACC_X1_PITCH_TEST1, m_nACC_TestPP1_X1_Pitch);
	DDX_Text(pDX, IDC_EDIT_DCC_X1_PITCH_TEST1, m_nDCC_TestPP1_X1_Pitch);
	DDX_Text(pDX, IDC_EDIT_SPD_X2_PITCH_TEST1, m_nSPD_TestPP1_X2_Pitch);
	DDX_Text(pDX, IDC_EDIT_ACC_X2_PITCH_TEST1, m_nACC_TestPP1_X2_Pitch);
	DDX_Text(pDX, IDC_EDIT_DCC_X2_PITCH_TEST1, m_nDCC_TestPP1_X2_Pitch);
	DDX_Text(pDX, IDC_EDIT_SPD_Y1_PITCH_TEST1, m_nSPD_TestPP1_Y1_Pitch);
	DDX_Text(pDX, IDC_EDIT_ACC_Y1_PITCH_TEST1, m_nACC_TestPP1_Y1_Pitch);
	DDX_Text(pDX, IDC_EDIT_DCC_Y1_PITCH_TEST1, m_nDCC_TestPP1_Y1_Pitch);
	DDX_Text(pDX, IDC_EDIT_SPD_Y2_PITCH_TEST1, m_nSPD_TestPP1_Y2_Pitch);
	DDX_Text(pDX, IDC_EDIT_ACC_Y2_PITCH_TEST1, m_nACC_TestPP1_Y2_Pitch);
	DDX_Text(pDX, IDC_EDIT_DCC_Y2_PITCH_TEST1, m_nDCC_TestPP1_Y2_Pitch);



	DDX_Text(pDX, IDC_EDIT_SPD_X1_TEST2, m_nSPD_TestPP2_X1);
	DDX_Text(pDX, IDC_EDIT_ACC_X1_TEST2, m_nACC_TestPP2_X1);
	DDX_Text(pDX, IDC_EDIT_DCC_X1_TEST2, m_nDCC_TestPP2_X1);
	DDX_Text(pDX, IDC_EDIT_SPD_X2_TEST2, m_nSPD_TestPP2_X2);
	DDX_Text(pDX, IDC_EDIT_ACC_X2_TEST2, m_nACC_TestPP2_X2);
	DDX_Text(pDX, IDC_EDIT_DCC_X2_TEST2, m_nDCC_TestPP2_X2);
	DDX_Text(pDX, IDC_EDIT_SPD_Y_TEST2, m_nSPD_TestPP2_Y);
	DDX_Text(pDX, IDC_EDIT_ACC_Y_TEST2, m_nACC_TestPP2_Y);
	DDX_Text(pDX, IDC_EDIT_DCC_Y_TEST2, m_nDCC_TestPP2_Y);
	DDX_Text(pDX, IDC_EDIT_SPD_Z1_TEST2, m_nSPD_TestPP2_Z1);
	DDX_Text(pDX, IDC_EDIT_ACC_Z1_TEST2, m_nACC_TestPP2_Z1);
	DDX_Text(pDX, IDC_EDIT_DCC_Z1_TEST2, m_nDCC_TestPP2_Z1);
	DDX_Text(pDX, IDC_EDIT_SPD_Z2_TEST2, m_nSPD_TestPP2_Z2);
	DDX_Text(pDX, IDC_EDIT_ACC_Z2_TEST2, m_nACC_TestPP2_Z2);
	DDX_Text(pDX, IDC_EDIT_DCC_Z2_TEST2, m_nDCC_TestPP2_Z2);
	DDX_Text(pDX, IDC_EDIT_SPD_X1_PITCH_TEST2, m_nSPD_TestPP2_X1_Pitch);
	DDX_Text(pDX, IDC_EDIT_ACC_X1_PITCH_TEST2, m_nACC_TestPP2_X1_Pitch);
	DDX_Text(pDX, IDC_EDIT_DCC_X1_PITCH_TEST2, m_nDCC_TestPP2_X1_Pitch);
	DDX_Text(pDX, IDC_EDIT_SPD_X2_PITCH_TEST2, m_nSPD_TestPP2_X2_Pitch);
	DDX_Text(pDX, IDC_EDIT_ACC_X2_PITCH_TEST2, m_nACC_TestPP2_X2_Pitch);
	DDX_Text(pDX, IDC_EDIT_DCC_X2_PITCH_TEST2, m_nDCC_TestPP2_X2_Pitch);
	DDX_Text(pDX, IDC_EDIT_SPD_Y1_PITCH_TEST2, m_nSPD_TestPP2_Y1_Pitch);
	DDX_Text(pDX, IDC_EDIT_ACC_Y1_PITCH_TEST2, m_nACC_TestPP2_Y1_Pitch);
	DDX_Text(pDX, IDC_EDIT_DCC_Y1_PITCH_TEST2, m_nDCC_TestPP2_Y1_Pitch);
	DDX_Text(pDX, IDC_EDIT_SPD_Y2_PITCH_TEST2, m_nSPD_TestPP2_Y2_Pitch);
	DDX_Text(pDX, IDC_EDIT_ACC_Y2_PITCH_TEST2, m_nACC_TestPP2_Y2_Pitch);
	DDX_Text(pDX, IDC_EDIT_DCC_Y2_PITCH_TEST2, m_nDCC_TestPP2_Y2_Pitch);


	DDX_Text(pDX, IDC_EDIT_VACUUM_DELAY_PP1, m_dTestPP1_Vacuum);
	DDX_Text(pDX, IDC_EDIT_BLOW_DELAY_PP1, m_dTestPP1_Blow);
	DDX_Text(pDX, IDC_EDIT_VACUUM_DELAY_PP2, m_dTestPP2_Vacuum);
	DDX_Text(pDX, IDC_EDIT_BLOW_DELAY_PP2, m_dTestPP2_Blow);

	DDX_Control(pDX, IDC_EDIT_SPD_X1_TEST1, m_editSPD_TestPP1_X1);
	DDX_Control(pDX, IDC_EDIT_ACC_X1_TEST1, m_editACC_TestPP1_X1);
	DDX_Control(pDX, IDC_EDIT_DCC_X1_TEST1, m_editDCC_TestPP1_X1);
	DDX_Control(pDX, IDC_EDIT_SPD_X2_TEST1, m_editSPD_TestPP1_X2);
	DDX_Control(pDX, IDC_EDIT_ACC_X2_TEST1, m_editACC_TestPP1_X2);
	DDX_Control(pDX, IDC_EDIT_DCC_X2_TEST1, m_editDCC_TestPP1_X2);
	DDX_Control(pDX, IDC_EDIT_SPD_Y_TEST1, m_editSPD_TestPP1_Y);
	DDX_Control(pDX, IDC_EDIT_ACC_Y_TEST1, m_editACC_TestPP1_Y);
	DDX_Control(pDX, IDC_EDIT_DCC_Y_TEST1, m_editDCC_TestPP1_Y);
	DDX_Control(pDX, IDC_EDIT_SPD_Z1_TEST1, m_editSPD_TestPP1_Z1);
	DDX_Control(pDX, IDC_EDIT_ACC_Z1_TEST1, m_editACC_TestPP1_Z1);
	DDX_Control(pDX, IDC_EDIT_DCC_Z1_TEST1, m_editDCC_TestPP1_Z1);
	DDX_Control(pDX, IDC_EDIT_SPD_Z2_TEST1, m_editSPD_TestPP1_Z2);
	DDX_Control(pDX, IDC_EDIT_ACC_Z2_TEST1, m_editACC_TestPP1_Z2);
	DDX_Control(pDX, IDC_EDIT_DCC_Z2_TEST1, m_editDCC_TestPP1_Z2);
	DDX_Control(pDX, IDC_EDIT_SPD_X1_PITCH_TEST1, m_editSPD_TestPP1_X1_Pitch);
	DDX_Control(pDX, IDC_EDIT_ACC_X1_PITCH_TEST1, m_editACC_TestPP1_X1_Pitch);
	DDX_Control(pDX, IDC_EDIT_DCC_X1_PITCH_TEST1, m_editDCC_TestPP1_X1_Pitch);
	DDX_Control(pDX, IDC_EDIT_SPD_X2_PITCH_TEST1, m_editSPD_TestPP1_X2_Pitch);
	DDX_Control(pDX, IDC_EDIT_ACC_X2_PITCH_TEST1, m_editACC_TestPP1_X2_Pitch);
	DDX_Control(pDX, IDC_EDIT_DCC_X2_PITCH_TEST1, m_editDCC_TestPP1_X2_Pitch);
	DDX_Control(pDX, IDC_EDIT_SPD_Y1_PITCH_TEST1, m_editSPD_TestPP1_Y1_Pitch);
	DDX_Control(pDX, IDC_EDIT_ACC_Y1_PITCH_TEST1, m_editACC_TestPP1_Y1_Pitch);
	DDX_Control(pDX, IDC_EDIT_DCC_Y1_PITCH_TEST1, m_editDCC_TestPP1_Y1_Pitch);
	DDX_Control(pDX, IDC_EDIT_SPD_Y2_PITCH_TEST1, m_editSPD_TestPP1_Y2_Pitch);
	DDX_Control(pDX, IDC_EDIT_ACC_Y2_PITCH_TEST1, m_editACC_TestPP1_Y2_Pitch);
	DDX_Control(pDX, IDC_EDIT_DCC_Y2_PITCH_TEST1, m_editDCC_TestPP1_Y2_Pitch);



	DDX_Control(pDX, IDC_EDIT_SPD_X1_TEST2, m_editSPD_TestPP2_X1);
	DDX_Control(pDX, IDC_EDIT_ACC_X1_TEST2, m_editACC_TestPP2_X1);
	DDX_Control(pDX, IDC_EDIT_DCC_X1_TEST2, m_editDCC_TestPP2_X1);
	DDX_Control(pDX, IDC_EDIT_SPD_X2_TEST2, m_editSPD_TestPP2_X2);
	DDX_Control(pDX, IDC_EDIT_ACC_X2_TEST2, m_editACC_TestPP2_X2);
	DDX_Control(pDX, IDC_EDIT_DCC_X2_TEST2, m_editDCC_TestPP2_X2);
	DDX_Control(pDX, IDC_EDIT_SPD_Y_TEST2, m_editSPD_TestPP2_Y);
	DDX_Control(pDX, IDC_EDIT_ACC_Y_TEST2, m_editACC_TestPP2_Y);
	DDX_Control(pDX, IDC_EDIT_DCC_Y_TEST2, m_editDCC_TestPP2_Y);
	DDX_Control(pDX, IDC_EDIT_SPD_Z1_TEST2, m_editSPD_TestPP2_Z1);
	DDX_Control(pDX, IDC_EDIT_ACC_Z1_TEST2, m_editACC_TestPP2_Z1);
	DDX_Control(pDX, IDC_EDIT_DCC_Z1_TEST2, m_editDCC_TestPP2_Z1);
	DDX_Control(pDX, IDC_EDIT_SPD_Z2_TEST2, m_editSPD_TestPP2_Z2);
	DDX_Control(pDX, IDC_EDIT_ACC_Z2_TEST2, m_editACC_TestPP2_Z2);
	DDX_Control(pDX, IDC_EDIT_DCC_Z2_TEST2, m_editDCC_TestPP2_Z2);
	DDX_Control(pDX, IDC_EDIT_SPD_X1_PITCH_TEST2, m_editSPD_TestPP2_X1_Pitch);
	DDX_Control(pDX, IDC_EDIT_ACC_X1_PITCH_TEST2, m_editACC_TestPP2_X1_Pitch);
	DDX_Control(pDX, IDC_EDIT_DCC_X1_PITCH_TEST2, m_editDCC_TestPP2_X1_Pitch);
	DDX_Control(pDX, IDC_EDIT_SPD_X2_PITCH_TEST2, m_editSPD_TestPP2_X2_Pitch);
	DDX_Control(pDX, IDC_EDIT_ACC_X2_PITCH_TEST2, m_editACC_TestPP2_X2_Pitch);
	DDX_Control(pDX, IDC_EDIT_DCC_X2_PITCH_TEST2, m_editDCC_TestPP2_X2_Pitch);
	DDX_Control(pDX, IDC_EDIT_SPD_Y1_PITCH_TEST2, m_editSPD_TestPP2_Y1_Pitch);
	DDX_Control(pDX, IDC_EDIT_ACC_Y1_PITCH_TEST2, m_editACC_TestPP2_Y1_Pitch);
	DDX_Control(pDX, IDC_EDIT_DCC_Y1_PITCH_TEST2, m_editDCC_TestPP2_Y1_Pitch);
	DDX_Control(pDX, IDC_EDIT_SPD_Y2_PITCH_TEST2, m_editSPD_TestPP2_Y2_Pitch);
	DDX_Control(pDX, IDC_EDIT_ACC_Y2_PITCH_TEST2, m_editACC_TestPP2_Y2_Pitch);
	DDX_Control(pDX, IDC_EDIT_DCC_Y2_PITCH_TEST2, m_editDCC_TestPP2_Y2_Pitch);											   


	DDX_Control(pDX, IDC_EDIT_VACUUM_DELAY_PP1, m_editTestPP1_Vacuum);
	DDX_Control(pDX, IDC_EDIT_BLOW_DELAY_PP1, m_editTestPP1_Blow);
	DDX_Control(pDX, IDC_EDIT_VACUUM_DELAY_PP2, m_editTestPP2_Vacuum);
	DDX_Control(pDX, IDC_EDIT_BLOW_DELAY_PP2, m_editTestPP2_Blow);
	DDX_Control(pDX, IDC_EDIT_TEST_PP1_RETRY, m_editTestPP1_Retry);
	DDX_Control(pDX, IDC_EDIT_TEST_PP2_RETRY, m_editTestPP2_Retry);

	DDX_Control(pDX, IDC_EDIT_TEST_PP1_PICK_RETRY_OFFSET, m_editTestPP1_Pick_Retry_Offset);
	DDX_Control(pDX, IDC_EDIT_TEST_PP2_PICK_RETRY_OFFSET, m_editTestPP2_Pick_Retry_Offset);

	DDX_Control(pDX, IDC_EDIT_TEST_PP1_PICKER_WAIT_TIME, m_editTestPP1_Picker_Wait_Time);
	DDX_Control(pDX, IDC_EDIT_TEST_PP2_PICKER_WAIT_TIME, m_editTestPP2_Picker_Wait_Time);
	

	DDX_Text(pDX, IDC_EDIT_TEST_PP1_RETRY, m_nRetryCountPP1);
	DDX_Text(pDX, IDC_EDIT_TEST_PP2_RETRY, m_nRetryCountPP2);

	DDX_Text(pDX, IDC_EDIT_TEST_PP1_PICK_RETRY_OFFSET, m_dTestPP1_Pick_Retry_Offset);
	DDX_Text(pDX, IDC_EDIT_TEST_PP2_PICK_RETRY_OFFSET, m_dTestPP2_Pick_Retry_Offset);

	DDX_Text(pDX, IDC_EDIT_TEST_PP1_PICKER_WAIT_TIME, m_dTestPP1_Picker_Wait_Time);
	DDX_Text(pDX, IDC_EDIT_TEST_PP2_PICKER_WAIT_TIME, m_dTestPP2_Picker_Wait_Time);

	DDX_Control(pDX, IDC_CHECK_TESTPP_EXIST, m_ChkExistDevice);
	DDX_Control(pDX, IDC_CHECK_TESTPP_AFTER_EXIST, m_ChkAfterExistDevice);
	DDX_Control(pDX, IDC_CHECK_TESTPP_EXIST_SENSOR_CHECK, m_ChkExistDevice_Sensor);
	DDX_Control(pDX, IDC_CHECK_ART_TEST_PICKPLACE, m_ChkArtPickPlace);
	DDX_Control(pDX, IDC_CHECK_PRE_VACUUM, m_ChkPreVacuum);

	DDX_Radio(pDX, IDC_RADIO_VISION_OFF, m_bTestSiteVisionOn);
	DDX_Control(pDX, IDC_CHECK_TESTPP_SITE_ARRIVED, m_ChkSiteArrivedSen);
	DDX_Control(pDX, IDC_CHECK_TESTPP_SITE_MOVE_ARRIVED, m_ChkSiteMoveArrivedSen);
	DDX_Control(pDX, IDC_CHECK_TESTPP_LOAD_HAND_ALREADY_PICK, m_ChkLoadHandAlreadyPick);

	DDX_Control(pDX, IDC_CHECK_TESTPP_STATION_FR_PNP, m_ChkTestppStationFR_PNP);
	
	DDX_Radio(pDX, IDC_RADIO_RETOUCH_OFF, m_bReTouch);
	DDX_Text(pDX, IDC_EDIT_TEST_PP_RETOUCH, m_dReTouch_Dis);
	DDX_Control(pDX, IDC_EDIT_TEST_PP_RETOUCH, m_editTestSiteRetouchDis);

	DDX_Control(pDX, IDC_EDIT_TEST_PP_PICK_1STAGE_OFFSET, m_Edit_Pick_1Stage_Offset);
	DDX_Control(pDX, IDC_EDIT_TEST_PP_PICK_1STAGE_SPEED, m_Edit_Pick_1Stage_Speed);

}


BEGIN_MESSAGE_MAP(CSPD_TM_TEST_PP, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CSPD_TM_TEST_PP::OnBnClickedButtonSave)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_X1_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditSpdXTest1)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_X1_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditAccXTest1)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_X1_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditDccXTest1)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_Y_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditSpdYTest1)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_Y_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditAccYTest1)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_Y_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditDccYTest1)

	ON_EN_SETFOCUS(IDC_EDIT_SPD_X1_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditSpdXTest2)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_X1_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditAccXTest2)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_X1_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditDccXTest2)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_Y_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditSpdYTest2)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_Y_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditAccYTest2)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_Y_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditDccYTest2)

	ON_EN_SETFOCUS(IDC_EDIT_VACUUM_DELAY_PP1, &CSPD_TM_TEST_PP::OnEnSetfocusEditVacuumDelayPp1)
	ON_EN_SETFOCUS(IDC_EDIT_BLOW_DELAY_PP1, &CSPD_TM_TEST_PP::OnEnSetfocusEditBlowDelayPp1)
	ON_EN_SETFOCUS(IDC_EDIT_VACUUM_DELAY_PP2, &CSPD_TM_TEST_PP::OnEnSetfocusEditVacuumDelayPp2)
	ON_EN_SETFOCUS(IDC_EDIT_BLOW_DELAY_PP2, &CSPD_TM_TEST_PP::OnEnSetfocusEditBlowDelayPp2)
	ON_EN_SETFOCUS(IDC_EDIT_TEST_PP1_RETRY, &CSPD_TM_TEST_PP::OnEnSetfocusEditTestPp1Retry)
	ON_EN_SETFOCUS(IDC_EDIT_TEST_PP2_RETRY, &CSPD_TM_TEST_PP::OnEnSetfocusEditTestPp2Retry)

	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_VISION_ON, IDC_RADIO_VISION_OFF, OnClickedTestSiteVision)
	ON_EN_SETFOCUS(IDC_EDIT_TEST_PP_RETOUCH, &CSPD_TM_TEST_PP::OnEnSetfocusEditTestPpRetouch)

	ON_EN_SETFOCUS(IDC_EDIT_SPD_X2_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditSpdX2Test1)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_X2_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditAccX2Test1)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_X2_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditDccX2Test1)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_X2_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditSpdX2Test2)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_X2_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditAccX2Test2)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_X2_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditDccX2Test2)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_Z1_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditSpdZ1Test1)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_Z1_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditAccZ1Test1)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_Z1_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditDccZ1Test1)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_Z2_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditSpdZ2Test1)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_Z2_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditAccZ2Test1)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_Z2_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditDccZ2Test1)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_X1_PITCH_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditSpdX1PitchTest1)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_X1_PITCH_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditAccX1PitchTest1)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_X1_PITCH_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditDccX1PitchTest1)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_Y1_PITCH_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditSpdY1PitchTest1)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_Y1_PITCH_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditAccY1PitchTest1)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_Y1_PITCH_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditDccY1PitchTest1)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_X2_PITCH_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditSpdX2PitchTest1)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_X2_PITCH_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditAccX2PitchTest1)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_X2_PITCH_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditDccX2PitchTest1)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_Y2_PITCH_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditSpdY2PitchTest1)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_Y2_PITCH_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditAccY2PitchTest1)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_Y2_PITCH_TEST1, &CSPD_TM_TEST_PP::OnEnSetfocusEditDccY2PitchTest1)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_Z1_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditSpdZ1Test2)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_Z1_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditAccZ1Test2)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_Z1_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditDccZ1Test2)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_Z2_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditSpdZ2Test2)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_Z2_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditAccZ2Test2)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_Z2_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditDccZ2Test2)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_X1_PITCH_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditSpdX1PitchTest2)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_X1_PITCH_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditAccX1PitchTest2)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_X1_PITCH_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditDccX1PitchTest2)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_Y1_PITCH_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditSpdY1PitchTest2)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_Y1_PITCH_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditAccY1PitchTest2)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_Y1_PITCH_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditDccY1PitchTest2)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_X2_PITCH_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditSpdX2PitchTest2)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_X2_PITCH_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditAccX2PitchTest2)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_X2_PITCH_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditDccX2PitchTest2)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_Y2_PITCH_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditSpdY2PitchTest2)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_Y2_PITCH_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditAccY2PitchTest2)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_Y2_PITCH_TEST2, &CSPD_TM_TEST_PP::OnEnSetfocusEditDccY2PitchTest2)
	ON_EN_SETFOCUS(IDC_EDIT_TEST_PP_PICK_1STAGE_OFFSET, &CSPD_TM_TEST_PP::OnEnSetfocusEditTestPpPick1stageOffset)
	ON_EN_SETFOCUS(IDC_EDIT_TEST_PP_PICK_1STAGE_SPEED, &CSPD_TM_TEST_PP::OnEnSetfocusEditTestPpPick1stageSpeed)
	ON_EN_SETFOCUS(IDC_EDIT_TEST_PP1_PICK_RETRY_OFFSET, &CSPD_TM_TEST_PP::OnEnSetfocusEditTestPp1PickRetryOffset)
	ON_EN_SETFOCUS(IDC_EDIT_TEST_PP2_PICK_RETRY_OFFSET, &CSPD_TM_TEST_PP::OnEnSetfocusEditTestPp2PickRetryOffset)
	ON_EN_SETFOCUS(IDC_EDIT_TEST_PP1_PICKER_WAIT_TIME, &CSPD_TM_TEST_PP::OnEnSetfocusEditTestPp1PickerWaitTime)
	ON_EN_SETFOCUS(IDC_EDIT_TEST_PP2_PICKER_WAIT_TIME, &CSPD_TM_TEST_PP::OnEnSetfocusEditTestPp2PickerWaitTime)
END_MESSAGE_MAP()


// CSPD_TM_TEST_PP 메시지 처리기입니다.


BOOL CSPD_TM_TEST_PP::OnInitDialog()
{
	CDialog::OnInitDialog();

	char szCompany[16] = { 0, };
	CString strCustomer = _T("");
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	ChkLotState();

	if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0)
	{
		GetDlgItem(IDC_STATIC_RETOUCH)		->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO_RETOUCH_ON)	->ShowWindow(FALSE);
		GetDlgItem(IDC_RADIO_RETOUCH_OFF)	->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEST_PP_RETOUCH)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_TEXT_MM)		->ShowWindow(FALSE);
		GetDlgItem(IDC_CHECK_TESTPP_AFTER_EXIST)->ShowWindow(FALSE);
	}
	else {
		GetDlgItem(IDC_STATIC_RETOUCH)		->ShowWindow(TRUE);
		GetDlgItem(IDC_RADIO_RETOUCH_ON)	->ShowWindow(TRUE);
		GetDlgItem(IDC_RADIO_RETOUCH_OFF)	->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_TEST_PP_RETOUCH)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_TEXT_MM)		->ShowWindow(TRUE);

		GetDlgItem(IDC_STATIC_VACUUM5)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_TEXT113)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEST_PP1_RETRY)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_TEXT114)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_TEST_PP2_RETRY)->ShowWindow(FALSE);
// 		if(strcmp(szCompany, DEF_COMPANY_LUXSHARE) == 0)
// 			GetDlgItem(IDC_CHECK_TESTPP_AFTER_EXIST)->ShowWindow(FALSE);	
	}

	
	GetDlgItem(IDC_CHECK_TESTPP_SITE_MOVE_ARRIVED)->ShowWindow(FALSE);
	GetDlgItem(IDC_CHECK_TESTPP_STATION_FR_PNP)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_TESTPP_STATION_FR_PNP)->ShowWindow(FALSE);
	
		
		

	int nChk = (int)g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TestPP_DevExist_Check);
	m_ChkExistDevice.SetCheck(nChk);

	nChk = (int)g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_DevExist_Sen_Check);
	m_ChkExistDevice_Sensor.SetCheck(nChk);

	nChk = (int)g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_ART_TEST_HAND_PICK_PLACE);
	m_ChkArtPickPlace.SetCheck(nChk);

	nChk = (int)g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TEST_HAND_PRE_VACUUM);
	m_ChkPreVacuum.SetCheck(nChk);

	nChk = (int)g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TEST_HAND_PNP_SITE_ARRIVED_SEN);
	m_ChkSiteArrivedSen.SetCheck(nChk);

	nChk = (int)g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TestPP_After_DevExist_Check);
	m_ChkAfterExistDevice.SetCheck(nChk);

	nChk = (int)g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TEST_HAND_PNP_SITE_MOVE_ARRIVED_SEN);
	m_ChkSiteMoveArrivedSen.SetCheck(nChk);
	
	nChk = (int)g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TESTPP_STATION_FR_PNP);
	m_ChkTestppStationFR_PNP.SetCheck(nChk);

	nChk = (int)g_DMCont.m_dmHandlerOpt.GB(BDM11_MD_TEST_LOAD_HAND_ALREADY_PICK);
	m_ChkLoadHandAlreadyPick.SetCheck(nChk);
	
	

	CString strPick_1Stage_Offset;
	strPick_1Stage_Offset.Format("%.1f", m_dTestPP_Pick_1Stage_Offset);
	m_Edit_Pick_1Stage_Offset.SetWindowText(strPick_1Stage_Offset);

	CString strPick_1Stage_Speed;
	strPick_1Stage_Speed.Format("%d", (int)m_dTestPP_Pick_1Stage_Speed);
	m_Edit_Pick_1Stage_Speed.SetWindowText(strPick_1Stage_Speed);


	if (m_bTestSiteVisionOn == TRUE){
		GetDlgItem(IDC_CHECK_TESTPP_EXIST)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_TESTPP_AFTER_EXIST)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_CHECK_TESTPP_EXIST)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_TESTPP_AFTER_EXIST)->EnableWindow(FALSE);
	}

	OnBtnGUI();
	OnEditBoxGUI();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSPD_TM_TEST_PP::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);
}

void CSPD_TM_TEST_PP::OnEditBoxGUI()
{
	m_editSPD_TestPP1_X1.SetBackColor(COLOR_WWHITE);
	m_editACC_TestPP1_X1.SetBackColor(COLOR_WWHITE);
	m_editDCC_TestPP1_X1.SetBackColor(COLOR_WWHITE);
	m_editSPD_TestPP1_X2.SetBackColor(COLOR_WWHITE);
	m_editACC_TestPP1_X2.SetBackColor(COLOR_WWHITE);
	m_editDCC_TestPP1_X2.SetBackColor(COLOR_WWHITE);
	m_editSPD_TestPP1_Y.SetBackColor(COLOR_WWHITE);
	m_editACC_TestPP1_Y.SetBackColor(COLOR_WWHITE);
	m_editDCC_TestPP1_Y.SetBackColor(COLOR_WWHITE);
	m_editSPD_TestPP1_Z1.SetBackColor(COLOR_WWHITE);
	m_editACC_TestPP1_Z1.SetBackColor(COLOR_WWHITE);
	m_editDCC_TestPP1_Z1.SetBackColor(COLOR_WWHITE);
	m_editSPD_TestPP1_Z2.SetBackColor(COLOR_WWHITE);

	m_editACC_TestPP1_Z2.SetBackColor(COLOR_WWHITE);

	m_editDCC_TestPP1_Z2.SetBackColor(COLOR_WWHITE);
	m_editSPD_TestPP1_X1_Pitch.SetBackColor(COLOR_WWHITE);
	m_editACC_TestPP1_X1_Pitch.SetBackColor(COLOR_WWHITE);
	m_editDCC_TestPP1_X1_Pitch.SetBackColor(COLOR_WWHITE);
	m_editSPD_TestPP1_X2_Pitch.SetBackColor(COLOR_WWHITE);
	m_editACC_TestPP1_X2_Pitch.SetBackColor(COLOR_WWHITE);
	m_editDCC_TestPP1_X2_Pitch.SetBackColor(COLOR_WWHITE);
	m_editSPD_TestPP1_Y1_Pitch.SetBackColor(COLOR_WWHITE);
	m_editACC_TestPP1_Y1_Pitch.SetBackColor(COLOR_WWHITE);
	m_editDCC_TestPP1_Y1_Pitch.SetBackColor(COLOR_WWHITE);
	m_editSPD_TestPP1_Y2_Pitch.SetBackColor(COLOR_WWHITE);
	m_editACC_TestPP1_Y2_Pitch.SetBackColor(COLOR_WWHITE);
	m_editDCC_TestPP1_Y2_Pitch.SetBackColor(COLOR_WWHITE);


	m_editSPD_TestPP2_X1.SetBackColor(COLOR_WWHITE);
	m_editACC_TestPP2_X1.SetBackColor(COLOR_WWHITE);
	m_editDCC_TestPP2_X1.SetBackColor(COLOR_WWHITE);
	m_editSPD_TestPP2_X2.SetBackColor(COLOR_WWHITE);
	m_editACC_TestPP2_X2.SetBackColor(COLOR_WWHITE);
	m_editDCC_TestPP2_X2.SetBackColor(COLOR_WWHITE);
	m_editSPD_TestPP2_Y.SetBackColor(COLOR_WWHITE);
	m_editACC_TestPP2_Y.SetBackColor(COLOR_WWHITE);
	m_editDCC_TestPP2_Y.SetBackColor(COLOR_WWHITE);
	m_editSPD_TestPP2_Z1.SetBackColor(COLOR_WWHITE);
	m_editACC_TestPP2_Z1.SetBackColor(COLOR_WWHITE);
	m_editDCC_TestPP2_Z1.SetBackColor(COLOR_WWHITE);
	m_editSPD_TestPP2_Z2.SetBackColor(COLOR_WWHITE);
	m_editACC_TestPP2_Z2.SetBackColor(COLOR_WWHITE);
	m_editDCC_TestPP2_Z2.SetBackColor(COLOR_WWHITE);
	m_editSPD_TestPP2_X1_Pitch.SetBackColor(COLOR_WWHITE);
	m_editACC_TestPP2_X1_Pitch.SetBackColor(COLOR_WWHITE);
	m_editDCC_TestPP2_X1_Pitch.SetBackColor(COLOR_WWHITE);
	m_editSPD_TestPP2_X2_Pitch.SetBackColor(COLOR_WWHITE);
	m_editACC_TestPP2_X2_Pitch.SetBackColor(COLOR_WWHITE);
	m_editDCC_TestPP2_X2_Pitch.SetBackColor(COLOR_WWHITE);
	m_editSPD_TestPP2_Y1_Pitch.SetBackColor(COLOR_WWHITE);
	m_editACC_TestPP2_Y1_Pitch.SetBackColor(COLOR_WWHITE);
	m_editDCC_TestPP2_Y1_Pitch.SetBackColor(COLOR_WWHITE);
	m_editSPD_TestPP2_Y2_Pitch.SetBackColor(COLOR_WWHITE);
	m_editACC_TestPP2_Y2_Pitch.SetBackColor(COLOR_WWHITE);
	m_editDCC_TestPP2_Y2_Pitch.SetBackColor(COLOR_WWHITE);

	m_editTestPP1_Vacuum.SetBackColor(COLOR_WWHITE);
	m_editTestPP2_Vacuum.SetBackColor(COLOR_WWHITE);
	m_editTestPP1_Blow.SetBackColor(COLOR_WWHITE);
	m_editTestPP2_Blow.SetBackColor(COLOR_WWHITE);
	m_editTestPP1_Retry.SetBackColor(COLOR_WWHITE);
	m_editTestPP2_Retry.SetBackColor(COLOR_WWHITE);

	m_editTestPP1_Pick_Retry_Offset.SetBackColor(COLOR_WWHITE);
	m_editTestPP2_Pick_Retry_Offset.SetBackColor(COLOR_WWHITE);

	m_editTestPP1_Picker_Wait_Time.SetBackColor(COLOR_WWHITE);
	m_editTestPP2_Picker_Wait_Time.SetBackColor(COLOR_WWHITE);

	m_editTestSiteRetouchDis.SetBackColor(COLOR_WWHITE);

	m_Edit_Pick_1Stage_Offset.SetBackColor(COLOR_WWHITE);
	m_Edit_Pick_1Stage_Speed.SetBackColor(COLOR_WWHITE);

	m_editSPD_TestPP1_X1.SetTextSize(30);
	m_editACC_TestPP1_X1.SetTextSize(30);
	m_editDCC_TestPP1_X1.SetTextSize(30);
	m_editSPD_TestPP1_X2.SetTextSize(30);
	m_editACC_TestPP1_X2.SetTextSize(30);
	m_editDCC_TestPP1_X2.SetTextSize(30);
	m_editSPD_TestPP1_Y.SetTextSize(30);
	m_editACC_TestPP1_Y.SetTextSize(30);
	m_editDCC_TestPP1_Y.SetTextSize(30);
	m_editSPD_TestPP1_Z1.SetTextSize(30);
	m_editACC_TestPP1_Z1.SetTextSize(30);
	m_editDCC_TestPP1_Z1.SetTextSize(30);
	m_editSPD_TestPP1_Z2.SetTextSize(30);
	m_editACC_TestPP1_Z2.SetTextSize(30);
	m_editDCC_TestPP1_Z2.SetTextSize(30);
	m_editSPD_TestPP1_X1_Pitch.SetTextSize(30);
	m_editACC_TestPP1_X1_Pitch.SetTextSize(30);
	m_editDCC_TestPP1_X1_Pitch.SetTextSize(30);
	m_editSPD_TestPP1_X2_Pitch.SetTextSize(30);
	m_editACC_TestPP1_X2_Pitch.SetTextSize(30);
	m_editDCC_TestPP1_X2_Pitch.SetTextSize(30);
	m_editSPD_TestPP1_Y1_Pitch.SetTextSize(30);
	m_editACC_TestPP1_Y1_Pitch.SetTextSize(30);
	m_editDCC_TestPP1_Y1_Pitch.SetTextSize(30);
	m_editSPD_TestPP1_Y2_Pitch.SetTextSize(30);
	m_editACC_TestPP1_Y2_Pitch.SetTextSize(30);
	m_editDCC_TestPP1_Y2_Pitch.SetTextSize(30);

	m_editSPD_TestPP2_X1.SetTextSize(30);
	m_editACC_TestPP2_X1.SetTextSize(30);
	m_editDCC_TestPP2_X1.SetTextSize(30);
	m_editSPD_TestPP2_X2.SetTextSize(30);
	m_editACC_TestPP2_X2.SetTextSize(30);
	m_editDCC_TestPP2_X2.SetTextSize(30);
	m_editSPD_TestPP2_Y.SetTextSize(30);
	m_editACC_TestPP2_Y.SetTextSize(30);
	m_editDCC_TestPP2_Y.SetTextSize(30);
	m_editSPD_TestPP2_Z1.SetTextSize(30);
	m_editACC_TestPP2_Z1.SetTextSize(30);
	m_editDCC_TestPP2_Z1.SetTextSize(30);
	m_editSPD_TestPP2_Z2.SetTextSize(30);
	m_editACC_TestPP2_Z2.SetTextSize(30);
	m_editDCC_TestPP2_Z2.SetTextSize(30);
	m_editSPD_TestPP2_X1_Pitch.SetTextSize(30);
	m_editACC_TestPP2_X1_Pitch.SetTextSize(30);
	m_editDCC_TestPP2_X1_Pitch.SetTextSize(30);
	m_editSPD_TestPP2_X2_Pitch.SetTextSize(30);
	m_editACC_TestPP2_X2_Pitch.SetTextSize(30);
	m_editDCC_TestPP2_X2_Pitch.SetTextSize(30);
	m_editSPD_TestPP2_Y1_Pitch.SetTextSize(30);
	m_editACC_TestPP2_Y1_Pitch.SetTextSize(30);
	m_editDCC_TestPP2_Y1_Pitch.SetTextSize(30);
	m_editSPD_TestPP2_Y2_Pitch.SetTextSize(30);
	m_editACC_TestPP2_Y2_Pitch.SetTextSize(30);
	m_editDCC_TestPP2_Y2_Pitch.SetTextSize(30);

	m_editTestPP1_Vacuum.SetTextSize(30);
	m_editTestPP2_Vacuum.SetTextSize(30);
	m_editTestPP1_Blow.SetTextSize(30);
	m_editTestPP2_Blow.SetTextSize(30);
	m_editTestPP1_Retry.SetTextSize(30);
	m_editTestPP2_Retry.SetTextSize(30);

	m_editTestSiteRetouchDis.SetTextSize(30);


	m_Edit_Pick_1Stage_Offset.SetTextSize(30);
	m_Edit_Pick_1Stage_Speed.SetTextSize(30);

	m_editTestPP1_Pick_Retry_Offset.SetTextSize(30);
	m_editTestPP2_Pick_Retry_Offset.SetTextSize(30);

	m_editTestPP1_Picker_Wait_Time.SetTextSize(30);
	m_editTestPP2_Picker_Wait_Time.SetTextSize(30);
	
}

void CSPD_TM_TEST_PP::OnBnClickedButtonSave()
{
	UpdateData(TRUE);
	g_DMCont.m_dmTestPP.SN(NDM4_SPD_TestPP1_X1, m_nSPD_TestPP1_X1);
	g_DMCont.m_dmTestPP.SN(NDM4_ACC_TestPP1_X1, m_nACC_TestPP1_X1);
	g_DMCont.m_dmTestPP.SN(NDM4_DCC_TestPP1_X1, m_nDCC_TestPP1_X1);
	g_DMCont.m_dmTestPP.SN(NDM4_SPD_TestPP1_X2, m_nSPD_TestPP1_X2);
	g_DMCont.m_dmTestPP.SN(NDM4_ACC_TestPP1_X2, m_nACC_TestPP1_X2);
	g_DMCont.m_dmTestPP.SN(NDM4_DCC_TestPP1_X2, m_nDCC_TestPP1_X2);
	g_DMCont.m_dmTestPP.SN(NDM4_SPD_TestPP1_Y, m_nSPD_TestPP1_Y);
	g_DMCont.m_dmTestPP.SN(NDM4_ACC_TestPP1_Y, m_nACC_TestPP1_Y);
	g_DMCont.m_dmTestPP.SN(NDM4_DCC_TestPP1_Y, m_nDCC_TestPP1_Y);
	g_DMCont.m_dmTestPP.SN(NDM4_SPD_TestPP1_Z1, m_nSPD_TestPP1_Z1);
	g_DMCont.m_dmTestPP.SN(NDM4_ACC_TestPP1_Z1, m_nACC_TestPP1_Z1);
	g_DMCont.m_dmTestPP.SN(NDM4_DCC_TestPP1_Z1, m_nDCC_TestPP1_Z1);
	g_DMCont.m_dmTestPP.SN(NDM4_SPD_TestPP1_Z2, m_nSPD_TestPP1_Z2);
	g_DMCont.m_dmTestPP.SN(NDM4_ACC_TestPP1_Z2, m_nACC_TestPP1_Z2);
	g_DMCont.m_dmTestPP.SN(NDM4_DCC_TestPP1_Z2, m_nDCC_TestPP1_Z2);
	g_DMCont.m_dmTestPP.SN(NDM5_SPD_TestPP_1_Pitch_X1, m_nSPD_TestPP1_X1_Pitch);
	g_DMCont.m_dmTestPP.SN(NDM5_ACC_TestPP_1_Pitch_X1, m_nACC_TestPP1_X1_Pitch);
	g_DMCont.m_dmTestPP.SN(NDM5_DCC_TestPP_1_Pitch_X1, m_nDCC_TestPP1_X1_Pitch);
	g_DMCont.m_dmTestPP.SN(NDM5_SPD_TestPP_1_Pitch_X2, m_nSPD_TestPP1_X2_Pitch);
	g_DMCont.m_dmTestPP.SN(NDM5_ACC_TestPP_1_Pitch_X2, m_nACC_TestPP1_X2_Pitch);
	g_DMCont.m_dmTestPP.SN(NDM5_DCC_TestPP_1_Pitch_X2, m_nDCC_TestPP1_X2_Pitch);
	g_DMCont.m_dmTestPP.SN(NDM5_SPD_TestPP_1_Pitch_Y1, m_nSPD_TestPP1_Y1_Pitch);
	g_DMCont.m_dmTestPP.SN(NDM5_ACC_TestPP_1_Pitch_Y1, m_nACC_TestPP1_Y1_Pitch);
	g_DMCont.m_dmTestPP.SN(NDM5_DCC_TestPP_1_Pitch_Y1, m_nDCC_TestPP1_Y1_Pitch);
	g_DMCont.m_dmTestPP.SN(NDM5_SPD_TestPP_1_Pitch_Y2, m_nSPD_TestPP1_Y2_Pitch);
	g_DMCont.m_dmTestPP.SN(NDM5_ACC_TestPP_1_Pitch_Y2, m_nACC_TestPP1_Y2_Pitch);
	g_DMCont.m_dmTestPP.SN(NDM5_DCC_TestPP_1_Pitch_Y2, m_nDCC_TestPP1_Y2_Pitch);
	

	g_DMCont.m_dmTestPP.SN(NDM4_SPD_TestPP2_X1, m_nSPD_TestPP2_X1);
	g_DMCont.m_dmTestPP.SN(NDM4_ACC_TestPP2_X1, m_nACC_TestPP2_X1);
	g_DMCont.m_dmTestPP.SN(NDM4_DCC_TestPP2_X1, m_nDCC_TestPP2_X1);
	g_DMCont.m_dmTestPP.SN(NDM4_SPD_TestPP2_X2, m_nSPD_TestPP2_X2);
	g_DMCont.m_dmTestPP.SN(NDM4_ACC_TestPP2_X2, m_nACC_TestPP2_X2);
	g_DMCont.m_dmTestPP.SN(NDM4_DCC_TestPP2_X2, m_nDCC_TestPP2_X2);
	g_DMCont.m_dmTestPP.SN(NDM4_SPD_TestPP2_Y, m_nSPD_TestPP2_Y);
	g_DMCont.m_dmTestPP.SN(NDM4_ACC_TestPP2_Y, m_nACC_TestPP2_Y);
	g_DMCont.m_dmTestPP.SN(NDM4_DCC_TestPP2_Y, m_nDCC_TestPP2_Y);
	g_DMCont.m_dmTestPP.SN(NDM4_SPD_TestPP2_Z1, m_nSPD_TestPP2_Z1);
	g_DMCont.m_dmTestPP.SN(NDM4_ACC_TestPP2_Z1, m_nACC_TestPP2_Z1);
	g_DMCont.m_dmTestPP.SN(NDM4_DCC_TestPP2_Z1, m_nDCC_TestPP2_Z1);
	g_DMCont.m_dmTestPP.SN(NDM4_SPD_TestPP2_Z2, m_nSPD_TestPP2_Z2);
	g_DMCont.m_dmTestPP.SN(NDM4_ACC_TestPP2_Z2, m_nACC_TestPP2_Z2);
	g_DMCont.m_dmTestPP.SN(NDM4_DCC_TestPP2_Z2, m_nDCC_TestPP2_Z2);
	g_DMCont.m_dmTestPP.SN(NDM5_SPD_TestPP_2_Pitch_X1, m_nSPD_TestPP2_X1_Pitch);
	g_DMCont.m_dmTestPP.SN(NDM5_ACC_TestPP_2_Pitch_X1, m_nACC_TestPP2_X1_Pitch);
	g_DMCont.m_dmTestPP.SN(NDM5_DCC_TestPP_2_Pitch_X1, m_nDCC_TestPP2_X1_Pitch);
	g_DMCont.m_dmTestPP.SN(NDM5_SPD_TestPP_2_Pitch_X2, m_nSPD_TestPP2_X2_Pitch);
	g_DMCont.m_dmTestPP.SN(NDM5_ACC_TestPP_2_Pitch_X2, m_nACC_TestPP2_X2_Pitch);
	g_DMCont.m_dmTestPP.SN(NDM5_DCC_TestPP_2_Pitch_X2, m_nDCC_TestPP2_X2_Pitch);
	g_DMCont.m_dmTestPP.SN(NDM5_SPD_TestPP_2_Pitch_Y1, m_nSPD_TestPP2_Y1_Pitch);
	g_DMCont.m_dmTestPP.SN(NDM5_ACC_TestPP_2_Pitch_Y1, m_nACC_TestPP2_Y1_Pitch);
	g_DMCont.m_dmTestPP.SN(NDM5_DCC_TestPP_2_Pitch_Y1, m_nDCC_TestPP2_Y1_Pitch);
	g_DMCont.m_dmTestPP.SN(NDM5_SPD_TestPP_2_Pitch_Y2, m_nSPD_TestPP2_Y2_Pitch);
	g_DMCont.m_dmTestPP.SN(NDM5_ACC_TestPP_2_Pitch_Y2, m_nACC_TestPP2_Y2_Pitch);
	g_DMCont.m_dmTestPP.SN(NDM5_DCC_TestPP_2_Pitch_Y2, m_nDCC_TestPP2_Y2_Pitch);
	
	g_DMCont.m_dmTestPP.SN(NDM4_TM_TestPP1_Vacuum	, (int)(m_dTestPP1_Vacuum*1000));
	g_DMCont.m_dmTestPP.SN(NDM4_TM_TestPP1_Blow	, (int)(m_dTestPP1_Blow	 *1000));	 
	g_DMCont.m_dmTestPP.SN(NDM4_TM_TestPP2_Vacuum	, (int)(m_dTestPP2_Vacuum*1000));
	g_DMCont.m_dmTestPP.SN(NDM4_TM_TestPP2_Blow	, (int)(m_dTestPP2_Blow	 *1000));	
	g_DMCont.m_dmTestPP.SN(NDM4_TestPP1_RetryCnt	, m_nRetryCountPP1);
	g_DMCont.m_dmTestPP.SN(NDM4_TestPP2_RetryCnt	, m_nRetryCountPP2);

	g_DMCont.m_dmTestPP.SD(DDM4_TestPP1_Pick_Retry_Offset, m_dTestPP1_Pick_Retry_Offset * 1000.0);
	g_DMCont.m_dmTestPP.SD(DDM4_TestPP2_Pick_Retry_Offset, m_dTestPP2_Pick_Retry_Offset* 1000.0);

	g_DMCont.m_dmTestPP.SD(DDM4_TestPP1_Picker_Wait_Time, m_dTestPP1_Picker_Wait_Time * 1000.0);
	g_DMCont.m_dmTestPP.SD(DDM4_TestPP2_Picker_Wait_Time, m_dTestPP2_Picker_Wait_Time* 1000.0);

	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_TestPP_DevExist_Check, (BOOL)m_ChkExistDevice.GetCheck());
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_TestPP_After_DevExist_Check, (BOOL)m_ChkAfterExistDevice.GetCheck());
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_DevExist_Sen_Check, (BOOL)m_ChkExistDevice_Sensor.GetCheck());
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_ART_TEST_HAND_PICK_PLACE, (BOOL)m_ChkArtPickPlace.GetCheck());
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_TEST_HAND_PRE_VACUUM, (BOOL)m_ChkPreVacuum.GetCheck());
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_TEST_HAND_PNP_SITE_ARRIVED_SEN, (BOOL)m_ChkSiteArrivedSen.GetCheck());
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_TEST_HAND_PNP_SITE_MOVE_ARRIVED_SEN, (BOOL)m_ChkSiteMoveArrivedSen.GetCheck());
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_TESTPP_STATION_FR_PNP, (BOOL)m_ChkTestppStationFR_PNP.GetCheck());
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_TEST_LOAD_HAND_ALREADY_PICK, (BOOL)m_ChkLoadHandAlreadyPick.GetCheck());
	
	
	g_DMCont.m_dmHandlerOpt.SB(BDM11_MD_VisionUse, m_bTestSiteVisionOn);

	g_DMCont.m_dmHandlerOpt.SB(BDM11_TEST_SITE_RETOUCH, m_bReTouch);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_TEST_SITE_RETOUCH_DIS, (int)(m_dReTouch_Dis * 1000));


// 	CString strPick1StageOffset;
// 	GetDlgItemText(IDC_EDIT_TEST_PP_PICK_1STAGE_OFFSET, strPick1StageOffset);
// 	double dPick1StageOffset = atof(strPick1StageOffset);
// 	g_DMCont.m_dmHandlerOpt.SD(DDM11_TESTPP_PICK_1STAGE_OFFSET, dPick1StageOffset);
// 
// 	CString strPick1StageSpeed;
// 	GetDlgItemText(IDC_EDIT_TEST_PP_PICK_1STAGE_SPEED, strPick1StageSpeed);
// 	double dPick1StageSpeed = atof(strPick1StageSpeed);
// 	g_DMCont.m_dmHandlerOpt.SD(DDM11_TESTPP_PICK_1STAGE_SPEED, dPick1StageSpeed);
	
	char szCokName[512] = {0,};
	ST_COK_INFO   stCokData;
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));	
	g_COK_FileMgr.LoadCokInfo(szCokName, stCokData);

	ST_COK_INFO stBeforeCokData = stCokData;

	int nSpdPos[MAX_TEST_PP_CNT]      = {NDM4_SPD_TestPP1_X1, NDM4_SPD_TestPP2_X1 };
	int nTmPos[MAX_TEST_PP_CNT]       = {NDM4_TM_TestPP1_Vacuum, NDM4_TM_TestPP2_Vacuum};
	int nRetry[MAX_TEST_PP_CNT]       = {NDM4_TestPP1_RetryCnt, NDM4_TestPP2_RetryCnt};
	double dPickRetryOffset[MAX_TEST_PP_CNT] = { DDM4_TestPP1_Pick_Retry_Offset, DDM4_TestPP2_Pick_Retry_Offset };
	double dPickerWaitTime[MAX_TEST_PP_CNT] = { DDM4_TestPP1_Picker_Wait_Time, DDM4_TestPP2_Picker_Wait_Time };

	for(int i=0; i<=RIDX_TEST_PP_2; i++)
	{
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1].nSPD = g_DMCont.m_dmTestPP.GN(nSpdPos[i]);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1].nACC = g_DMCont.m_dmTestPP.GN(nSpdPos[i]+1);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1].nDCC = g_DMCont.m_dmTestPP.GN(nSpdPos[i]+2);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2].nSPD = g_DMCont.m_dmTestPP.GN(nSpdPos[i]+3);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2].nACC = g_DMCont.m_dmTestPP.GN(nSpdPos[i]+4);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2].nDCC = g_DMCont.m_dmTestPP.GN(nSpdPos[i]+5);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y].nSPD = g_DMCont.m_dmTestPP.GN(nSpdPos[i]+6);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y].nACC = g_DMCont.m_dmTestPP.GN(nSpdPos[i]+7);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y].nDCC = g_DMCont.m_dmTestPP.GN(nSpdPos[i]+8);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z1].nSPD = g_DMCont.m_dmTestPP.GN(nSpdPos[i]+9);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z1].nACC = g_DMCont.m_dmTestPP.GN(nSpdPos[i]+10);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z1].nDCC = g_DMCont.m_dmTestPP.GN(nSpdPos[i]+11);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z2].nSPD = g_DMCont.m_dmTestPP.GN(nSpdPos[i] + 12);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z2].nACC = g_DMCont.m_dmTestPP.GN(nSpdPos[i] + 13);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Z2].nDCC = g_DMCont.m_dmTestPP.GN(nSpdPos[i] + 14);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1_Pitch].nSPD = g_DMCont.m_dmTestPP.GN(nSpdPos[i] + 15);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1_Pitch].nACC = g_DMCont.m_dmTestPP.GN(nSpdPos[i] + 16);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X1_Pitch].nDCC = g_DMCont.m_dmTestPP.GN(nSpdPos[i] + 17);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2_Pitch].nSPD = g_DMCont.m_dmTestPP.GN(nSpdPos[i] + 18);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2_Pitch].nACC = g_DMCont.m_dmTestPP.GN(nSpdPos[i] + 19);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_X2_Pitch].nDCC = g_DMCont.m_dmTestPP.GN(nSpdPos[i] + 20);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y1_Pitch].nSPD = g_DMCont.m_dmTestPP.GN(nSpdPos[i] + 21);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y1_Pitch].nACC = g_DMCont.m_dmTestPP.GN(nSpdPos[i] + 22);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y1_Pitch].nDCC = g_DMCont.m_dmTestPP.GN(nSpdPos[i] + 23);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y2_Pitch].nSPD = g_DMCont.m_dmTestPP.GN(nSpdPos[i] + 24);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y2_Pitch].nACC = g_DMCont.m_dmTestPP.GN(nSpdPos[i] + 25);
		stCokData.aTestPP_SPD[i].aAxisForm[eAxis_TestPP_Y2_Pitch].nDCC = g_DMCont.m_dmTestPP.GN(nSpdPos[i] + 26);

		stCokData.aTestPP_Tm[i].nVacuumTm = g_DMCont.m_dmTestPP.GN(nTmPos[i]);
		stCokData.aTestPP_Tm[i].nBlowTm   = g_DMCont.m_dmTestPP.GN(nTmPos[i]+1);
		stCokData.aTestPP_Tm[i].nRetryCnt = g_DMCont.m_dmTestPP.GN(nRetry[i]);
		stCokData.aTestPP_Tm[i].dPickRetryOffset = g_DMCont.m_dmTestPP.GD(dPickRetryOffset[i]);
		stCokData.aTestPP_Tm[i].dPickerWaitTm = g_DMCont.m_dmTestPP.GD(dPickerWaitTime[i]);
		
	}


	g_COK_FileMgr.SaveCokInfo(szCokName, &stCokData);
	ST_BASIC_INFO   stBasicData;
	g_COK_FileMgr.LoadBasicInfo(szCokName, stBasicData);
	stBasicData.bMD_TestPP_DevExist_Check = m_ChkExistDevice.GetCheck();
	stBasicData.bMD_TestPP_After_DevExist_Check = m_ChkAfterExistDevice.GetCheck();
	stBasicData.bMD_TestPP_DevExist_Check_Sensor = m_ChkExistDevice_Sensor.GetCheck();
	stBasicData.bMD_TestPP_ArtDevice_PickPlace = m_ChkArtPickPlace.GetCheck();
	stBasicData.bMD_TestPP_PreVacuum = m_ChkPreVacuum.GetCheck();
	stBasicData.nVision = m_bTestSiteVisionOn;
	stBasicData.bMD_TestPP_PNP_SiteArrived_Sen = m_ChkSiteArrivedSen.GetCheck();
	stBasicData.bMD_TestPP_PNP_SiteMoveArrived_Sen = m_ChkSiteMoveArrivedSen.GetCheck();
	stBasicData.bMD_TestppStationFR_PNP = m_ChkTestppStationFR_PNP.GetCheck();
	stBasicData.bMD_LoadHandAlreadyPick = m_ChkLoadHandAlreadyPick.GetCheck();
	

	stBasicData.bMD_TestSite_Retouch = m_bReTouch;
	stBasicData.dTestSite_Retouch_Dis = g_DMCont.m_dmHandlerOpt.GN(NDM11_TEST_SITE_RETOUCH_DIS);


	if (m_bTestSiteVisionOn != m_bBackVisionOnOff) {
		g_McComm.CmdVisionUseNouse(eVisionPos_TestSite);
		m_bBackVisionOnOff = m_bTestSiteVisionOn;
	}

	g_COK_FileMgr.SaveBasicInfo(szCokName, &stBasicData);

	std::vector<std::pair<CString,std::pair<double,double>>> vData;  // vData.first = stCokData, vData.second.first = BeforeData vData.second.second = AfterData

	stBeforeCokData.IsSameTestPP(stCokData, vData);
	
	if(!(vData.empty()))
	{
		TestPPSpeedTiemChangeLog(vData);
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditSpdXTest1()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nSPD_TestPP1_X1, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 X1 Speed", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_SPD_X1_TEST1)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditAccXTest1()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nACC_TestPP1_X1, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 X1 Accel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_ACC_X1_TEST1)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditDccXTest1()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nDCC_TestPP1_X1, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 X1 Decel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DCC_X1_TEST1)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}

void CSPD_TM_TEST_PP::OnEnSetfocusEditSpdX2Test1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_TestPP1_X2, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 X2 Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_X2_TEST1)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditAccX2Test1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_TestPP1_X2, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 X2 Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_X2_TEST1)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditDccX2Test1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_TestPP1_X2, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 X2 Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_X2_TEST1)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}

void CSPD_TM_TEST_PP::OnEnSetfocusEditSpdYTest1()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nSPD_TestPP1_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 Y Speed", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_SPD_Y_TEST1)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditAccYTest1()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nACC_TestPP1_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 Y Accel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_ACC_Y_TEST1)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditDccYTest1()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nDCC_TestPP1_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 Y Decel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DCC_Y_TEST1)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}

void CSPD_TM_TEST_PP::OnEnSetfocusEditSpdXTest2()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nSPD_TestPP2_X1, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 X1 Speed", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_SPD_X1_TEST2)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditAccXTest2()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nACC_TestPP2_X1, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 X1 Accel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_ACC_X1_TEST2)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditDccXTest2()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nDCC_TestPP2_X1, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 X1 Decel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DCC_X1_TEST2)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}

void CSPD_TM_TEST_PP::OnEnSetfocusEditSpdX2Test2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_TestPP2_X2, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 X2 Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_X2_TEST2)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditAccX2Test2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_TestPP2_X2, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 X2 Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_X2_TEST2)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditDccX2Test2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_TestPP2_X2, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 X2 Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_X2_TEST2)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditSpdYTest2()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nSPD_TestPP2_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 Y Speed", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_SPD_Y_TEST2)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditAccYTest2()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nACC_TestPP2_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 Y Accel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_ACC_Y_TEST2)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditDccYTest2()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nDCC_TestPP2_Y, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 Y Decel", "100", "5");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DCC_Y_TEST2)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}

void CSPD_TM_TEST_PP::OnEnSetfocusEditVacuumDelayPp1()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTestPP1_Vacuum);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 Vacuum Time", "3.0", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_VACUUM_DELAY_PP1)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditBlowDelayPp1()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTestPP1_Blow);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 Blow Time", "3.0", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_BLOW_DELAY_PP1)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditVacuumDelayPp2()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTestPP2_Vacuum);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 Vacuum Time", "3.0", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_VACUUM_DELAY_PP2)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditBlowDelayPp2()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dTestPP2_Blow);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 Blow Time", "3.0", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_BLOW_DELAY_PP2)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditTestPp1Retry()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nRetryCountPP1, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP1 Retry Count", "5", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_TEST_PP1_RETRY)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditTestPp2Retry()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nRetryCountPP2, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP2 Retry Count", "5", "0");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_TEST_PP2_RETRY)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}




void CSPD_TM_TEST_PP::MakeLog(LPCTSTR fmt, ...)
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


// vData.first = stCokData, vData.second.first = BeforeData vData.second.second = AfterData
void CSPD_TM_TEST_PP::TestPPSpeedTiemChangeLog(std::vector<std::pair<CString,std::pair<double,double>>> vData)
{
	for(int i = 0 ; i< (int)vData.size(); i++)
	{
		std::pair<CString,std::pair<double,double>> p;

		p = vData[i];

		MakeLog("[MENU = Test PP Speed & Time, DATA NAME = %s ][BEFORE=%.2f, AFTER=%.2f]", p.first, p.second.first, p.second.second);
	}
}


void CSPD_TM_TEST_PP::OnClickedTestSiteVision(UINT nID)
{
	switch (nID)
	{
	case IDC_RADIO_VISION_ON:
		GetDlgItem(IDC_CHECK_TESTPP_EXIST)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_TESTPP_AFTER_EXIST)->EnableWindow(TRUE);
		break;
	case IDC_RADIO_VISION_OFF:
		GetDlgItem(IDC_CHECK_TESTPP_EXIST)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_TESTPP_AFTER_EXIST)->EnableWindow(FALSE);
		break;
	}
}

void CSPD_TM_TEST_PP::OnEnSetfocusEditTestPpRetouch()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	sprintf_s(szVal, sizeof(szVal), "%0.2f", m_dReTouch_Dis);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test Site Retouch Distance", "3.0", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_TEST_PP_RETOUCH)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}



void CSPD_TM_TEST_PP::ChkLotState()
{
	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
	if (bLotState == eHandlerLotMode_OnGoing)
	{
		GetDlgItem(IDC_CHECK_TESTPP_STATION_FR_PNP)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_TESTPP_STATION_FR_PNP)->EnableWindow(FALSE);
	}
	else {
		GetDlgItem(IDC_CHECK_TESTPP_STATION_FR_PNP)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_TESTPP_STATION_FR_PNP)->EnableWindow(TRUE);
	}
}

void CSPD_TM_TEST_PP::OnEnSetfocusEditSpdZ1Test1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_TestPP1_Z1, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 Z1 Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_Z1_TEST1)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditAccZ1Test1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_TestPP1_Z1, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 Z1 Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_Z1_TEST1)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditDccZ1Test1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_TestPP1_Z1, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 Z1 Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_Z1_TEST1)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditSpdZ2Test1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_TestPP1_Z2, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 Z2 Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_Z2_TEST1)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditAccZ2Test1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_TestPP1_Z2, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 Z2 Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_Z2_TEST1)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditDccZ2Test1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_TestPP1_Z2, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 Z2 Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_Z2_TEST1)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditSpdX1PitchTest1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_TestPP1_X1_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 X1 Pitch Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_X1_PITCH_TEST1)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditAccX1PitchTest1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_TestPP1_X1_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 X1 Pitch Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_X1_PITCH_TEST1)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditDccX1PitchTest1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_TestPP1_X1_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 X1 Pitch Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_X1_PITCH_TEST1)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}

void CSPD_TM_TEST_PP::OnEnSetfocusEditSpdX2PitchTest1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_TestPP1_X2_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 X2 Pitch Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_X2_PITCH_TEST1)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditAccX2PitchTest1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_TestPP1_X2_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 X2 Pitch Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_X2_PITCH_TEST1)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditDccX2PitchTest1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_TestPP1_X2_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 X2 Pitch Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_X2_PITCH_TEST1)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}

void CSPD_TM_TEST_PP::OnEnSetfocusEditSpdY1PitchTest1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_TestPP1_Y1_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 Y1 Pitch Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_Y1_PITCH_TEST1)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditAccY1PitchTest1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_TestPP1_Y1_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 Y1 Pitch Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_Y1_PITCH_TEST1)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditDccY1PitchTest1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_TestPP1_Y1_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 Y1 Pitch Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_Y1_PITCH_TEST1)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditSpdY2PitchTest1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_TestPP1_Y2_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 Y2 Pitch Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_Y2_PITCH_TEST1)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditAccY2PitchTest1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_TestPP1_Y2_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 Y2 Pitch Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_Y2_PITCH_TEST1)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditDccY2PitchTest1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_TestPP1_Y2_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 Y2 Pitch Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_Y2_PITCH_TEST1)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditSpdZ1Test2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_TestPP2_Z1, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 Z1 Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_Z1_TEST2)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditAccZ1Test2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_TestPP2_Z1, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 Z1 Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_Z1_TEST2)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditDccZ1Test2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_TestPP2_Z1, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 Z1 Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_Z1_TEST2)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditSpdZ2Test2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_TestPP2_Z2, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 Z2 Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_Z2_TEST2)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditAccZ2Test2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_TestPP2_Z2, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 Z2 Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_Z2_TEST2)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditDccZ2Test2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_TestPP2_Z2, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 Z2 Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_Z2_TEST2)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditSpdX1PitchTest2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_TestPP2_X1_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 X1 Pitch Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_X1_PITCH_TEST2)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditAccX1PitchTest2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_TestPP2_X1_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 X1 Pitch Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_X1_PITCH_TEST2)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditDccX1PitchTest2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_TestPP2_X1_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 X1 Pitch Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_X1_PITCH_TEST2)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditSpdY1PitchTest2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_TestPP2_Y1_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 Y1 Pitch Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_Y1_PITCH_TEST2)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditAccY1PitchTest2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_TestPP2_Y1_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 Y1 Pitch Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_Y1_PITCH_TEST2)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditDccY1PitchTest2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_TestPP2_Y1_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 Y1 Pitch Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_Y1_PITCH_TEST2)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditSpdX2PitchTest2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_TestPP2_X2_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 X2 Pitch Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_X2_PITCH_TEST2)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditAccX2PitchTest2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_TestPP2_X2_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 X2 Pitch Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_X2_PITCH_TEST2)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditDccX2PitchTest2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_TestPP2_X2_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 X2 Pitch Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_X2_PITCH_TEST2)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditSpdY2PitchTest2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nSPD_TestPP2_Y2_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 Y2 Pitch Speed", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SPD_Y2_PITCH_TEST2)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditAccY2PitchTest2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nACC_TestPP2_Y2_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #1 Y2 Pitch Accel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ACC_Y2_PITCH_TEST2)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditDccY2PitchTest2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nDCC_TestPP2_Y2_Pitch, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP #2 Y2 Pitch Decel", "100", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_DCC_Y2_PITCH_TEST2)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditTestPpPick1stageOffset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_dTestPP_Pick_1Stage_Offset, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP Pick 1Stage Offset", "10", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_TEST_PP_PICK_1STAGE_OFFSET)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditTestPpPick1stageSpeed()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_dTestPP_Pick_1Stage_Speed, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP Pick 1Stage Speed", "100", "1");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_TEST_PP_PICK_1STAGE_SPEED)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditTestPp1PickRetryOffset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_dTestPP1_Pick_Retry_Offset, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP 1 Pick Retry Offset", "0.15", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_TEST_PP1_PICK_RETRY_OFFSET)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditTestPp2PickRetryOffset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_dTestPP2_Pick_Retry_Offset, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP 2 Pick Retry Offset", "0.15", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_TEST_PP2_PICK_RETRY_OFFSET)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditTestPp1PickerWaitTime()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_dTestPP1_Picker_Wait_Time, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP 1 Picker Wait Time", "2.0", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_TEST_PP1_PICKER_WAIT_TIME)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_TEST_PP::OnEnSetfocusEditTestPp2PickerWaitTime()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_dTestPP2_Picker_Wait_Time, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Test PP 1 Pick Retry Offset", "2.0", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_TEST_PP2_PICKER_WAIT_TIME)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}
