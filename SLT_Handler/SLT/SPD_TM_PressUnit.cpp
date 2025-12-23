// SPD_TM_CONTACTOR.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT.h"
#include "SPD_TM_PressUnit.h"
#include "afxdialogex.h"


#define CHECK_ALL_SOCKET		(0)
#define CHECK_ONLY_SOCKET_OPEN	(1)

//#define CHECK_VIBRATOR_USE				(1)
//#define CHECK_VIBRATOR_NOT_USE			(0)

// CSPD_TM_CONTACTOR 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSPD_TM_CONTACTOR, CDialog)

CSPD_TM_CONTACTOR::CSPD_TM_CONTACTOR(CWnd* pParent /*=NULL*/)
	: CDialog(CSPD_TM_CONTACTOR::IDD, pParent)
{
	m_nSPD_PressUnit1   = g_DMCont.m_dmHandlerOpt.GN(NDM11_SPD_PressUnit1);
	m_nACC_PressUnit1   = g_DMCont.m_dmHandlerOpt.GN(NDM11_ACC_PressUnit1);
	m_nDCC_PressUnit1   = g_DMCont.m_dmHandlerOpt.GN(NDM11_DCC_PressUnit1);
	m_nSPD_PressUnit2   = g_DMCont.m_dmHandlerOpt.GN(NDM11_SPD_PressUnit2);
	m_nACC_PressUnit2   = g_DMCont.m_dmHandlerOpt.GN(NDM11_ACC_PressUnit2);
	m_nDCC_PressUnit2   = g_DMCont.m_dmHandlerOpt.GN(NDM11_DCC_PressUnit2);
	m_nSPD_PressUnit3   = g_DMCont.m_dmHandlerOpt.GN(NDM11_SPD_PressUnit3);
	m_nACC_PressUnit3   = g_DMCont.m_dmHandlerOpt.GN(NDM11_ACC_PressUnit3);
	m_nDCC_PressUnit3   = g_DMCont.m_dmHandlerOpt.GN(NDM11_DCC_PressUnit3);
	m_nSPD_PressUnit4   = g_DMCont.m_dmHandlerOpt.GN(NDM11_SPD_PressUnit4);
	m_nACC_PressUnit4   = g_DMCont.m_dmHandlerOpt.GN(NDM11_ACC_PressUnit4);
	m_nDCC_PressUnit4   = g_DMCont.m_dmHandlerOpt.GN(NDM11_DCC_PressUnit4);
	m_nSPD_PressUnit5   = g_DMCont.m_dmHandlerOpt.GN(NDM11_SPD_PressUnit5);
	m_nACC_PressUnit5   = g_DMCont.m_dmHandlerOpt.GN(NDM11_ACC_PressUnit5);
	m_nDCC_PressUnit5   = g_DMCont.m_dmHandlerOpt.GN(NDM11_DCC_PressUnit5);
	m_nSPD_PressUnit6   = g_DMCont.m_dmHandlerOpt.GN(NDM11_SPD_PressUnit6);
	m_nACC_PressUnit6   = g_DMCont.m_dmHandlerOpt.GN(NDM11_ACC_PressUnit6);
	m_nDCC_PressUnit6   = g_DMCont.m_dmHandlerOpt.GN(NDM11_DCC_PressUnit6);
	m_nSPD_PressUnit7   = g_DMCont.m_dmHandlerOpt.GN(NDM11_SPD_PressUnit7);
	m_nACC_PressUnit7   = g_DMCont.m_dmHandlerOpt.GN(NDM11_ACC_PressUnit7);
	m_nDCC_PressUnit7   = g_DMCont.m_dmHandlerOpt.GN(NDM11_DCC_PressUnit7);
	m_nSPD_PressUnit8   = g_DMCont.m_dmHandlerOpt.GN(NDM11_SPD_PressUnit8);
	m_nACC_PressUnit8   = g_DMCont.m_dmHandlerOpt.GN(NDM11_ACC_PressUnit8);
	m_nDCC_PressUnit8   = g_DMCont.m_dmHandlerOpt.GN(NDM11_DCC_PressUnit8);

	m_dBlow_PressUnit1  = g_DMCont.m_dmHandlerOpt.GN(NDM11_TM_PRESS1_BLOW_WAIT) / 1000.0;
	m_dBlow_PressUnit2	= g_DMCont.m_dmHandlerOpt.GN(NDM11_TM_PRESS2_BLOW_WAIT) / 1000.0;
	m_dBlow_PressUnit3	= g_DMCont.m_dmHandlerOpt.GN(NDM11_TM_PRESS3_BLOW_WAIT) / 1000.0;
	m_dBlow_PressUnit4	= g_DMCont.m_dmHandlerOpt.GN(NDM11_TM_PRESS4_BLOW_WAIT) / 1000.0;
	m_dBlow_PressUnit5	= g_DMCont.m_dmHandlerOpt.GN(NDM11_TM_PRESS5_BLOW_WAIT) / 1000.0;
	m_dBlow_PressUnit6	= g_DMCont.m_dmHandlerOpt.GN(NDM11_TM_PRESS6_BLOW_WAIT) / 1000.0;
	m_dBlow_PressUnit7	= g_DMCont.m_dmHandlerOpt.GN(NDM11_TM_PRESS7_BLOW_WAIT) / 1000.0;
	m_dBlow_PressUnit8	= g_DMCont.m_dmHandlerOpt.GN(NDM11_TM_PRESS8_BLOW_WAIT) / 1000.0;



	m_dPressUnit_Blow_Pos1 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS1_BLOW_POS);
	m_dPressUnit_Blow_Pos2 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS2_BLOW_POS);
	m_dPressUnit_Blow_Pos3 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS3_BLOW_POS);
	m_dPressUnit_Blow_Pos4 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS4_BLOW_POS);
	m_dPressUnit_Blow_Pos5 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS5_BLOW_POS);
	m_dPressUnit_Blow_Pos6 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS6_BLOW_POS);
	m_dPressUnit_Blow_Pos7 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS7_BLOW_POS);
	m_dPressUnit_Blow_Pos8 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS8_BLOW_POS);

	m_dPressUnit_Clean_Offset1 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS1_CLEAN_OFFSET);
	m_dPressUnit_Clean_Offset2 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS2_CLEAN_OFFSET);
	m_dPressUnit_Clean_Offset3 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS3_CLEAN_OFFSET);
	m_dPressUnit_Clean_Offset4 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS4_CLEAN_OFFSET);
	m_dPressUnit_Clean_Offset5 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS5_CLEAN_OFFSET);
	m_dPressUnit_Clean_Offset6 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS6_CLEAN_OFFSET);
	m_dPressUnit_Clean_Offset7 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS7_CLEAN_OFFSET);
	m_dPressUnit_Clean_Offset8 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS8_CLEAN_OFFSET);

	m_nDeviceCheckStatus= 0;
	m_nDeviceCheckOption =0;
	m_nViratorOption = 0;

	m_nBlowPosOption = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_BLOW_POS_OPTION);
	m_nCleanOffsetOption = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_CLEAN_OFFSET_OPTION);

	memset(m_szCompanyVibratorMode, 0x00, sizeof(m_szCompanyVibratorMode));

	m_nAlwayViratorOption = 0;

	m_nVibratorConut = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_VIBRATOR_RETRY_CNT);
	m_dVibratorDelayTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_PRESS_VIBRATOR_DELAY_TIME);

	m_nAlwayVibratorConut = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_ALWAY_VIBRATOR_RETRY_CNT);
	m_dAlwayVibratorDelayTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_PRESS_ALWAY_VIBRATOR_DELAY_TIME);

	m_nReContactConut = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_RECONTACT_CNT);

	m_dReContactOffset = g_DMCont.m_dmHandlerOpt.GD(DDM11_PRESS_RECONTACT_OFFSET);

	m_dReContactDelayTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_PRESS_RECONTACT_DELAY_TIME);

	m_dContactPosOffset = g_DMCont.m_dmHandlerOpt.GD(DDM11_PRESS_CONTACT_POS_OFFSET);

	m_nVibrationOption = g_DMCont.m_dmEQP.GB(BDM0_MD_VIBRATION_MODE);

	m_nContact2stSpeed = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_CONTACT_2ST_SPEED);

	m_dCleaning2ndZOffset = g_DMCont.m_dmHandlerOpt.GD(DDM11_PRESS_CLEANING_2ND_Z_OFFSET) / 1000.0;

	m_nContactLogOption = g_DMCont.m_dmEQP.GB(BDM0_PRESS_CONTACT_LOG_OPTION);

	m_nSocketCleanFunctionOption = g_DMCont.m_dmEQP.GB(BDM0_SOCKET_CLEAN_FUNCTION_OPTION);

	m_dSocketCleanBlowPos = g_DMCont.m_dmHandlerOpt.GD(DDM11_PRESS_SOCKET_CLEAN_BLOW_POS);
	m_dSocketCleanBlowTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_PRESS_SOCKET_CLEAN_BLOW_TIME);


}

CSPD_TM_CONTACTOR::~CSPD_TM_CONTACTOR()
{
}

void CSPD_TM_CONTACTOR::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_BtnSave);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Text(pDX, IDC_EDIT_SPD_CONTACT1, m_nSPD_PressUnit1);
	DDX_Text(pDX, IDC_EDIT_ACC_CONTACT1, m_nACC_PressUnit1);
	DDX_Text(pDX, IDC_EDIT_DCC_CONTACT1, m_nDCC_PressUnit1);
	DDX_Text(pDX, IDC_EDIT_SPD_CONTACT2, m_nSPD_PressUnit2);
	DDX_Text(pDX, IDC_EDIT_ACC_CONTACT2, m_nACC_PressUnit2);
	DDX_Text(pDX, IDC_EDIT_DCC_CONTACT2, m_nDCC_PressUnit2);
	DDX_Text(pDX, IDC_EDIT_SPD_CONTACT3, m_nSPD_PressUnit3);
	DDX_Text(pDX, IDC_EDIT_ACC_CONTACT3, m_nACC_PressUnit3);
	DDX_Text(pDX, IDC_EDIT_DCC_CONTACT3, m_nDCC_PressUnit3);
	DDX_Text(pDX, IDC_EDIT_SPD_CONTACT4, m_nSPD_PressUnit4);
	DDX_Text(pDX, IDC_EDIT_ACC_CONTACT4, m_nACC_PressUnit4);
	DDX_Text(pDX, IDC_EDIT_DCC_CONTACT4, m_nDCC_PressUnit4);
	DDX_Text(pDX, IDC_EDIT_SPD_CONTACT5, m_nSPD_PressUnit5);
	DDX_Text(pDX, IDC_EDIT_ACC_CONTACT5, m_nACC_PressUnit5);
	DDX_Text(pDX, IDC_EDIT_DCC_CONTACT5, m_nDCC_PressUnit5);
	DDX_Text(pDX, IDC_EDIT_SPD_CONTACT6, m_nSPD_PressUnit6);
	DDX_Text(pDX, IDC_EDIT_ACC_CONTACT6, m_nACC_PressUnit6);
	DDX_Text(pDX, IDC_EDIT_DCC_CONTACT6, m_nDCC_PressUnit6);
	DDX_Text(pDX, IDC_EDIT_SPD_CONTACT7, m_nSPD_PressUnit7);
	DDX_Text(pDX, IDC_EDIT_ACC_CONTACT7, m_nACC_PressUnit7);
	DDX_Text(pDX, IDC_EDIT_DCC_CONTACT7, m_nDCC_PressUnit7);
	DDX_Text(pDX, IDC_EDIT_SPD_CONTACT8, m_nSPD_PressUnit8);
	DDX_Text(pDX, IDC_EDIT_ACC_CONTACT8, m_nACC_PressUnit8);
	DDX_Text(pDX, IDC_EDIT_DCC_CONTACT8, m_nDCC_PressUnit8);

	DDX_Text(pDX, IDC_EDIT_BLOW_CONTACT1, m_dBlow_PressUnit1);
	DDX_Text(pDX, IDC_EDIT_BLOW_CONTACT2, m_dBlow_PressUnit2);
	DDX_Text(pDX, IDC_EDIT_BLOW_CONTACT3, m_dBlow_PressUnit3);
	DDX_Text(pDX, IDC_EDIT_BLOW_CONTACT4, m_dBlow_PressUnit4);
	DDX_Text(pDX, IDC_EDIT_BLOW_CONTACT5, m_dBlow_PressUnit5);
	DDX_Text(pDX, IDC_EDIT_BLOW_CONTACT6, m_dBlow_PressUnit6);
	DDX_Text(pDX, IDC_EDIT_BLOW_CONTACT7, m_dBlow_PressUnit7);
	DDX_Text(pDX, IDC_EDIT_BLOW_CONTACT8, m_dBlow_PressUnit8);


	DDX_Text(pDX, IDC_EDIT_CONTACT_POS_1, m_dPressUnit_Blow_Pos1);
	DDX_Text(pDX, IDC_EDIT_CONTACT_POS_2, m_dPressUnit_Blow_Pos2);
	DDX_Text(pDX, IDC_EDIT_CONTACT_POS_3, m_dPressUnit_Blow_Pos3);
	DDX_Text(pDX, IDC_EDIT_CONTACT_POS_4, m_dPressUnit_Blow_Pos4);
	DDX_Text(pDX, IDC_EDIT_CONTACT_POS_5, m_dPressUnit_Blow_Pos5);
	DDX_Text(pDX, IDC_EDIT_CONTACT_POS_6, m_dPressUnit_Blow_Pos6);
	DDX_Text(pDX, IDC_EDIT_CONTACT_POS_7, m_dPressUnit_Blow_Pos7);
	DDX_Text(pDX, IDC_EDIT_CONTACT_POS_8, m_dPressUnit_Blow_Pos8);

	DDX_Text(pDX, IDC_EDIT_CONTACT_CLEAN_OFFSET_1, m_dPressUnit_Clean_Offset1);
	DDX_Text(pDX, IDC_EDIT_CONTACT_CLEAN_OFFSET_2, m_dPressUnit_Clean_Offset2);
	DDX_Text(pDX, IDC_EDIT_CONTACT_CLEAN_OFFSET_3, m_dPressUnit_Clean_Offset3);
	DDX_Text(pDX, IDC_EDIT_CONTACT_CLEAN_OFFSET_4, m_dPressUnit_Clean_Offset4);
	DDX_Text(pDX, IDC_EDIT_CONTACT_CLEAN_OFFSET_5, m_dPressUnit_Clean_Offset5);
	DDX_Text(pDX, IDC_EDIT_CONTACT_CLEAN_OFFSET_6, m_dPressUnit_Clean_Offset6);
	DDX_Text(pDX, IDC_EDIT_CONTACT_CLEAN_OFFSET_7, m_dPressUnit_Clean_Offset7);
	DDX_Text(pDX, IDC_EDIT_CONTACT_CLEAN_OFFSET_8, m_dPressUnit_Clean_Offset8);


	DDX_Text(pDX, IDC_EDIT_VIB_RE_CNT, m_nVibratorConut);
	DDX_Text(pDX, IDC_EDIT_VIB_DELAY_TIME, m_dVibratorDelayTime);

	DDX_Text(pDX, IDC_EDIT_ALWAY_VIB_RE_CNT, m_nAlwayVibratorConut);
	DDX_Text(pDX, IDC_EDIT_ALWAY_VIB_DELAY_TIME, m_dAlwayVibratorDelayTime);

	DDX_Text(pDX, IDC_EDIT_RECONTACT_CNT, m_nReContactConut);
	DDX_Text(pDX, IDC_EDIT_RECONTACT_OFFSET, m_dReContactOffset);
	DDX_Text(pDX, IDC_EDIT_RECONTACT_DELAY_TIME, m_dReContactDelayTime);

	DDX_Text(pDX, IDC_EDIT_CONTACT_POS_OFFSET, m_dContactPosOffset);
	DDX_Text(pDX, IDC_EDIT_CONTACT_2ST_SPEED, m_nContact2stSpeed);
	DDX_Text(pDX, IDC_EDIT_CLEANING_2ND_Z_OFFSET, m_dCleaning2ndZOffset);


	DDX_Text(pDX, IDC_EDIT_SOCKET_CLEAN_BLOW_POS, m_dSocketCleanBlowPos);
	DDX_Text(pDX, IDC_EDIT_SOCKET_CLEAN_BLOW_TIME, m_dSocketCleanBlowTime);

	
	DDX_Control(pDX, IDC_EDIT_SPD_CONTACT1, m_editSPD_PressUnit1);
	DDX_Control(pDX, IDC_EDIT_ACC_CONTACT1, m_editACC_PressUnit1);
	DDX_Control(pDX, IDC_EDIT_DCC_CONTACT1, m_editDCC_PressUnit1);
	DDX_Control(pDX, IDC_EDIT_SPD_CONTACT2, m_editSPD_PressUnit2);
	DDX_Control(pDX, IDC_EDIT_ACC_CONTACT2, m_editACC_PressUnit2);
	DDX_Control(pDX, IDC_EDIT_DCC_CONTACT2, m_editDCC_PressUnit2);
	DDX_Control(pDX, IDC_EDIT_SPD_CONTACT3, m_editSPD_PressUnit3);
	DDX_Control(pDX, IDC_EDIT_ACC_CONTACT3, m_editACC_PressUnit3);
	DDX_Control(pDX, IDC_EDIT_DCC_CONTACT3, m_editDCC_PressUnit3);
	DDX_Control(pDX, IDC_EDIT_SPD_CONTACT4, m_editSPD_PressUnit4);
	DDX_Control(pDX, IDC_EDIT_ACC_CONTACT4, m_editACC_PressUnit4);
	DDX_Control(pDX, IDC_EDIT_DCC_CONTACT4, m_editDCC_PressUnit4);
	DDX_Control(pDX, IDC_EDIT_SPD_CONTACT5, m_editSPD_PressUnit5);
	DDX_Control(pDX, IDC_EDIT_ACC_CONTACT5, m_editACC_PressUnit5);
	DDX_Control(pDX, IDC_EDIT_DCC_CONTACT5, m_editDCC_PressUnit5);
	DDX_Control(pDX, IDC_EDIT_SPD_CONTACT6, m_editSPD_PressUnit6);
	DDX_Control(pDX, IDC_EDIT_ACC_CONTACT6, m_editACC_PressUnit6);
	DDX_Control(pDX, IDC_EDIT_DCC_CONTACT6, m_editDCC_PressUnit6);
	DDX_Control(pDX, IDC_EDIT_SPD_CONTACT7, m_editSPD_PressUnit7);
	DDX_Control(pDX, IDC_EDIT_ACC_CONTACT7, m_editACC_PressUnit7);
	DDX_Control(pDX, IDC_EDIT_DCC_CONTACT7, m_editDCC_PressUnit7);
	DDX_Control(pDX, IDC_EDIT_SPD_CONTACT8, m_editSPD_PressUnit8);
	DDX_Control(pDX, IDC_EDIT_ACC_CONTACT8, m_editACC_PressUnit8);
	DDX_Control(pDX, IDC_EDIT_DCC_CONTACT8, m_editDCC_PressUnit8);


	DDX_Control(pDX, IDC_EDIT_BLOW_CONTACT1, m_editBlow_PressUnit1);
	DDX_Control(pDX, IDC_EDIT_BLOW_CONTACT2, m_editBlow_PressUnit2);
	DDX_Control(pDX, IDC_EDIT_BLOW_CONTACT3, m_editBlow_PressUnit3);
	DDX_Control(pDX, IDC_EDIT_BLOW_CONTACT4, m_editBlow_PressUnit4);
	DDX_Control(pDX, IDC_EDIT_BLOW_CONTACT5, m_editBlow_PressUnit5);
	DDX_Control(pDX, IDC_EDIT_BLOW_CONTACT6, m_editBlow_PressUnit6);
	DDX_Control(pDX, IDC_EDIT_BLOW_CONTACT7, m_editBlow_PressUnit7);
	DDX_Control(pDX, IDC_EDIT_BLOW_CONTACT8, m_editBlow_PressUnit8);

	DDX_Control(pDX, IDC_EDIT_CONTACT_POS_1, m_edit_PressUnit_Blow_Pos1);
	DDX_Control(pDX, IDC_EDIT_CONTACT_POS_2, m_edit_PressUnit_Blow_Pos2);
	DDX_Control(pDX, IDC_EDIT_CONTACT_POS_3, m_edit_PressUnit_Blow_Pos3);
	DDX_Control(pDX, IDC_EDIT_CONTACT_POS_4, m_edit_PressUnit_Blow_Pos4);
	DDX_Control(pDX, IDC_EDIT_CONTACT_POS_5, m_edit_PressUnit_Blow_Pos5);
	DDX_Control(pDX, IDC_EDIT_CONTACT_POS_6, m_edit_PressUnit_Blow_Pos6);
	DDX_Control(pDX, IDC_EDIT_CONTACT_POS_7, m_edit_PressUnit_Blow_Pos7);
	DDX_Control(pDX, IDC_EDIT_CONTACT_POS_8, m_edit_PressUnit_Blow_Pos8);

	DDX_Control(pDX, IDC_EDIT_CONTACT_CLEAN_OFFSET_1, m_edit_PressUnit_Clean_Offset1);
	DDX_Control(pDX, IDC_EDIT_CONTACT_CLEAN_OFFSET_2, m_edit_PressUnit_Clean_Offset2);
	DDX_Control(pDX, IDC_EDIT_CONTACT_CLEAN_OFFSET_3, m_edit_PressUnit_Clean_Offset3);
	DDX_Control(pDX, IDC_EDIT_CONTACT_CLEAN_OFFSET_4, m_edit_PressUnit_Clean_Offset4);
	DDX_Control(pDX, IDC_EDIT_CONTACT_CLEAN_OFFSET_5, m_edit_PressUnit_Clean_Offset5);
	DDX_Control(pDX, IDC_EDIT_CONTACT_CLEAN_OFFSET_6, m_edit_PressUnit_Clean_Offset6);
	DDX_Control(pDX, IDC_EDIT_CONTACT_CLEAN_OFFSET_7, m_edit_PressUnit_Clean_Offset7);
	DDX_Control(pDX, IDC_EDIT_CONTACT_CLEAN_OFFSET_8, m_edit_PressUnit_Clean_Offset8);

	DDX_Control(pDX, IDC_STATIC_GROUP_CONTACT, m_stContact);

	DDX_Control(pDX, IDC_PRESS_RADIO_USE, m_rbChkDvcEnable);
	DDX_Control(pDX, IDC_PRESS_RADIO_NOT_USE, m_rbChkDvcDisable);
	DDX_Control(pDX, IDC_PRESS_RADIO_ALL_SOCKET, m_rbAllSocketOpt);
	DDX_Control(pDX, IDC_PRESS_RADIO_ONLY_SOCKET_OPEN, m_rbOnlySocketOpt);
	DDX_Control(pDX, IDC_RADIO_VIBRATOR_USE, m_rbChkVibUse);
	DDX_Control(pDX, IDC_RADIO_VIBRATOR_NOT_USE, m_rbChkVibNotUse);

	DDX_Control(pDX, IDC_RADIO_ALWAY_VIBRATOR_USE, m_rbChkAlwayVibUse);
	DDX_Control(pDX, IDC_RADIO_ALWAY_VIBRATOR_NOT_USE, m_rbChkAlwayVibNotUse);

	DDX_Control(pDX, IDC_RADIO_BLOW_POS_USE, m_rbChkContactUse);
	DDX_Control(pDX, IDC_RADIO_BLOW_POS_NOT_USE, m_rbChkContactNotUse);

	DDX_Control(pDX, IDC_RADIO_CLEAN_OFF_USE, m_rbChkCleanOffsetUse);
	DDX_Control(pDX, IDC_RADIO_CLEAN_OFFSET_NOT_USE, m_rbChkCleanOffsetNotUse);

	DDX_Control(pDX, IDC_EDIT_VIB_RE_CNT, m_EditReCnt);
	DDX_Control(pDX, IDC_EDIT_VIB_DELAY_TIME, m_EditDelayTime);

	DDX_Control(pDX, IDC_EDIT_ALWAY_VIB_RE_CNT, m_EditAlwayReCnt);
	DDX_Control(pDX, IDC_EDIT_ALWAY_VIB_DELAY_TIME, m_EditAlwayDelayTime);

	DDX_Control(pDX, IDC_RADIO_RECONTACT_USE, m_rbChkReContactEnable);
	DDX_Control(pDX, IDC_RADIO_RECONTACT_NOT_USE, m_rbChkReContactDisable);

	DDX_Control(pDX, IDC_RADIO_CONTACT_LOG_USE, m_rbChkReContactLogEnable);
	DDX_Control(pDX, IDC_RADIO_CONTACT_LOG_NOT_USE, m_rbChkReContactLogDisable);

	DDX_Control(pDX, IDC_RADIO_SOCKET_CLEAN_FUNCTION_USE, m_rbChkSocketCleanFunctionEnable);
	DDX_Control(pDX, IDC_RADIO_SOCKET_CLEAN_FUNCTION_NOT_USE, m_rbChkSocketCleanFunctionDisable);
	
	DDX_Control(pDX, IDC_EDIT_RECONTACT_CNT, m_EditReContactCnt);
	DDX_Control(pDX, IDC_EDIT_RECONTACT_OFFSET, m_EditReContactOffset);
	DDX_Control(pDX, IDC_EDIT_RECONTACT_DELAY_TIME, m_EditReContactDelayTime);
	
	DDX_Control(pDX, IDC_EDIT_CONTACT_POS_OFFSET, m_EditContactPosOffset);
	DDX_Control(pDX, IDC_EDIT_CONTACT_2ST_SPEED, m_EditContact2stSpeed);
	DDX_Control(pDX, IDC_EDIT_CLEANING_2ND_Z_OFFSET, m_EditCleaning2ndZOffset);


	DDX_Control(pDX, IDC_RADIO_VIBRATION_USE, m_rbChkVibrationUse);
	DDX_Control(pDX, IDC_RADIO_VIBRATION_NOT_USE, m_rbChkVibrationNotUse);

	DDX_Control(pDX, IDC_EDIT_SOCKET_CLEAN_BLOW_POS, m_EditSocketCleanBlowPos);
	DDX_Control(pDX, IDC_EDIT_SOCKET_CLEAN_BLOW_TIME, m_EditSocketCleanBlowTime);
}


BEGIN_MESSAGE_MAP(CSPD_TM_CONTACTOR, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CSPD_TM_CONTACTOR::OnBnClickedButtonSave)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_CONTACT1, &CSPD_TM_CONTACTOR::OnEnSetfocusEditSpdContact1)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_CONTACT1, &CSPD_TM_CONTACTOR::OnEnSetfocusEditAccContact1)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_CONTACT1, &CSPD_TM_CONTACTOR::OnEnSetfocusEditDccContact1)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_CONTACT2, &CSPD_TM_CONTACTOR::OnEnSetfocusEditSpdContact2)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_CONTACT2, &CSPD_TM_CONTACTOR::OnEnSetfocusEditAccContact2)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_CONTACT2, &CSPD_TM_CONTACTOR::OnEnSetfocusEditDccContact2)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_CONTACT3, &CSPD_TM_CONTACTOR::OnEnSetfocusEditSpdContact3)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_CONTACT3, &CSPD_TM_CONTACTOR::OnEnSetfocusEditAccContact3)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_CONTACT3, &CSPD_TM_CONTACTOR::OnEnSetfocusEditDccContact3)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_CONTACT4, &CSPD_TM_CONTACTOR::OnEnSetfocusEditSpdContact4)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_CONTACT4, &CSPD_TM_CONTACTOR::OnEnSetfocusEditAccContact4)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_CONTACT4, &CSPD_TM_CONTACTOR::OnEnSetfocusEditDccContact4)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_CONTACT5, &CSPD_TM_CONTACTOR::OnEnSetfocusEditSpdContact5)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_CONTACT5, &CSPD_TM_CONTACTOR::OnEnSetfocusEditAccContact5)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_CONTACT5, &CSPD_TM_CONTACTOR::OnEnSetfocusEditDccContact5)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_CONTACT6, &CSPD_TM_CONTACTOR::OnEnSetfocusEditSpdContact6)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_CONTACT6, &CSPD_TM_CONTACTOR::OnEnSetfocusEditAccContact6)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_CONTACT6, &CSPD_TM_CONTACTOR::OnEnSetfocusEditDccContact6)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_CONTACT7, &CSPD_TM_CONTACTOR::OnEnSetfocusEditSpdContact7)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_CONTACT7, &CSPD_TM_CONTACTOR::OnEnSetfocusEditAccContact7)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_CONTACT7, &CSPD_TM_CONTACTOR::OnEnSetfocusEditDccContact7)
	ON_EN_SETFOCUS(IDC_EDIT_SPD_CONTACT8, &CSPD_TM_CONTACTOR::OnEnSetfocusEditSpdContact8)
	ON_EN_SETFOCUS(IDC_EDIT_ACC_CONTACT8, &CSPD_TM_CONTACTOR::OnEnSetfocusEditAccContact8)
	ON_EN_SETFOCUS(IDC_EDIT_DCC_CONTACT8, &CSPD_TM_CONTACTOR::OnEnSetfocusEditDccContact8)

	ON_EN_SETFOCUS(IDC_EDIT_BLOW_TIME_1, &CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowTime1)
	ON_EN_SETFOCUS(IDC_EDIT_BLOW_TIME_2, &CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowTime2)
	ON_EN_SETFOCUS(IDC_EDIT_BLOW_TIME_3, &CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowTime3)
	ON_EN_SETFOCUS(IDC_EDIT_BLOW_TIME_4, &CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowTime4)
	ON_EN_SETFOCUS(IDC_EDIT_BLOW_TIME_5, &CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowTime5)
	ON_EN_SETFOCUS(IDC_EDIT_BLOW_TIME_6, &CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowTime6)
	ON_EN_SETFOCUS(IDC_EDIT_BLOW_TIME_7, &CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowTime7)
	ON_EN_SETFOCUS(IDC_EDIT_BLOW_TIME_8, &CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowTime8)

	ON_EN_SETFOCUS(IDC_EDIT_BLOW_POS_1, &CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowPos1)
	ON_EN_SETFOCUS(IDC_EDIT_BLOW_POS_2, &CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowPos2)
	ON_EN_SETFOCUS(IDC_EDIT_BLOW_POS_3, &CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowPos3)
	ON_EN_SETFOCUS(IDC_EDIT_BLOW_POS_4, &CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowPos4)
	ON_EN_SETFOCUS(IDC_EDIT_BLOW_POS_5, &CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowPos5)
	ON_EN_SETFOCUS(IDC_EDIT_BLOW_POS_6, &CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowPos6)
	ON_EN_SETFOCUS(IDC_EDIT_BLOW_POS_7, &CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowPos7)
	ON_EN_SETFOCUS(IDC_EDIT_BLOW_POS_8, &CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowPos8)


	ON_EN_SETFOCUS(IDC_EDIT_VIB_RE_CNT, &CSPD_TM_CONTACTOR::OnEnSetfocusEditVibratorCnt)
	ON_EN_SETFOCUS(IDC_EDIT_VIB_DELAY_TIME, &CSPD_TM_CONTACTOR::OnEnSetfocusEditVibratorDelayTime)

	ON_BN_CLICKED(IDC_PRESS_RADIO_USE, &CSPD_TM_CONTACTOR::OnBnClickedPressRadioUse)
	ON_BN_CLICKED(IDC_PRESS_RADIO_NOT_USE, &CSPD_TM_CONTACTOR::OnBnClickedPressRadioNotUse)
	ON_BN_CLICKED(IDC_PRESS_RADIO_ALL_SOCKET, &CSPD_TM_CONTACTOR::OnBnClickedPressRadioAllSocket)
	ON_BN_CLICKED(IDC_PRESS_RADIO_ONLY_SOCKET_OPEN, &CSPD_TM_CONTACTOR::OnBnClickedPressRadioOnlySocketOpen)
	ON_BN_CLICKED(IDC_RADIO_VIBRATOR_USE, &CSPD_TM_CONTACTOR::OnBnClickedRadioVibratorUse)
	ON_BN_CLICKED(IDC_RADIO_VIBRATOR_NOT_USE, &CSPD_TM_CONTACTOR::OnBnClickedRadioVibratorNotUse)
	ON_BN_CLICKED(IDC_RADIO_BLOW_POS_USE, &CSPD_TM_CONTACTOR::OnBnClickedRadioBlowPosUse)
	ON_BN_CLICKED(IDC_RADIO_BLOW_POS_NOT_USE, &CSPD_TM_CONTACTOR::OnBnClickedRadioBlowPosNotUse)


	ON_BN_CLICKED(IDC_RADIO_ALWAY_VIBRATOR_USE, &CSPD_TM_CONTACTOR::OnBnClickedRadioAlwayVibratorUse)
	ON_BN_CLICKED(IDC_RADIO_ALWAY_VIBRATOR_NOT_USE, &CSPD_TM_CONTACTOR::OnBnClickedRadioAlwayVibratorNotUse)
	ON_EN_SETFOCUS(IDC_EDIT_ALWAY_VIB_RE_CNT, &CSPD_TM_CONTACTOR::OnEnSetfocusEditAlwayVibReCnt)
	ON_EN_SETFOCUS(IDC_EDIT_ALWAY_VIB_DELAY_TIME, &CSPD_TM_CONTACTOR::OnEnSetfocusEditAlwayVibDelayTime)
	ON_BN_CLICKED(IDC_RADIO_RECONTACT_USE, &CSPD_TM_CONTACTOR::OnBnClickedRadioRecontactUse)
	ON_BN_CLICKED(IDC_RADIO_RECONTACT_NOT_USE, &CSPD_TM_CONTACTOR::OnBnClickedRadioRecontactNotUse)
	ON_EN_SETFOCUS(IDC_EDIT_RECONTACT_CNT, &CSPD_TM_CONTACTOR::OnEnSetfocusEditRecontactCnt)
	ON_EN_SETFOCUS(IDC_EDIT_RECONTACT_OFFSET, &CSPD_TM_CONTACTOR::OnEnSetfocusEditRecontactOffset)
	ON_EN_SETFOCUS(IDC_EDIT_RECONTACT_DELAY_TIME, &CSPD_TM_CONTACTOR::OnEnSetfocusEditRecontactDelayTime)
	ON_EN_SETFOCUS(IDC_EDIT_CONTACT_POS_OFFSET, &CSPD_TM_CONTACTOR::OnEnSetfocusEditContactPosOffset)
	ON_BN_CLICKED(IDC_RADIO_VIBRATION_USE, &CSPD_TM_CONTACTOR::OnBnClickedRadioVibrationUse)
	ON_BN_CLICKED(IDC_RADIO_VIBRATION_NOT_USE, &CSPD_TM_CONTACTOR::OnBnClickedRadioVibrationNotUse)
	ON_EN_SETFOCUS(IDC_EDIT_CONTACT_2ST_SPEED, &CSPD_TM_CONTACTOR::OnEnSetfocusEditContact2stSpeed)
	ON_BN_CLICKED(IDC_RADIO_CONTACT_LOG_USE, &CSPD_TM_CONTACTOR::OnBnClickedRadioContactLogUse)
	ON_BN_CLICKED(IDC_RADIO_CONTACT_LOG_NOT_USE, &CSPD_TM_CONTACTOR::OnBnClickedRadioContactLogNotUse)
	ON_BN_CLICKED(IDC_RADIO_SOCKET_CLEAN_FUNCTION_USE, &CSPD_TM_CONTACTOR::OnBnClickedStaticSocketCleanFunctionUse)
	ON_BN_CLICKED(IDC_RADIO_SOCKET_CLEAN_FUNCTION_NOT_USE, &CSPD_TM_CONTACTOR::OnBnClickedStaticSocketCleanFunctionNotUse)
	ON_EN_SETFOCUS(IDC_EDIT_SOCKET_CLEAN_BLOW_POS, &CSPD_TM_CONTACTOR::OnEnSetfocusEditSocketCleanBlowPos)
	ON_EN_SETFOCUS(IDC_EDIT_SOCKET_CLEAN_BLOW_TIME, &CSPD_TM_CONTACTOR::OnEnSetfocusEditSocketCleanBlowTime)
	ON_EN_SETFOCUS(IDC_EDIT_CLEANING_2ND_Z_OFFSET, &CSPD_TM_CONTACTOR::OnEnSetfocusEditCleaning2ndZOffset)
	ON_EN_SETFOCUS(IDC_EDIT_CONTACT_CLEAN_OFFSET_1, &CSPD_TM_CONTACTOR::OnEnSetfocusEditContactCleanOffset1)
	ON_EN_SETFOCUS(IDC_EDIT_CONTACT_CLEAN_OFFSET_2, &CSPD_TM_CONTACTOR::OnEnSetfocusEditContactCleanOffset2)
	ON_EN_SETFOCUS(IDC_EDIT_CONTACT_CLEAN_OFFSET_3, &CSPD_TM_CONTACTOR::OnEnSetfocusEditContactCleanOffset3)
	ON_EN_SETFOCUS(IDC_EDIT_CONTACT_CLEAN_OFFSET_4, &CSPD_TM_CONTACTOR::OnEnSetfocusEditContactCleanOffset4)
	ON_EN_SETFOCUS(IDC_EDIT_CONTACT_CLEAN_OFFSET_5, &CSPD_TM_CONTACTOR::OnEnSetfocusEditContactCleanOffset5)
	ON_EN_SETFOCUS(IDC_EDIT_CONTACT_CLEAN_OFFSET_6, &CSPD_TM_CONTACTOR::OnEnSetfocusEditContactCleanOffset6)
	ON_EN_SETFOCUS(IDC_EDIT_CONTACT_CLEAN_OFFSET_7, &CSPD_TM_CONTACTOR::OnEnSetfocusEditContactCleanOffset7)
	ON_EN_SETFOCUS(IDC_EDIT_CONTACT_CLEAN_OFFSET_8, &CSPD_TM_CONTACTOR::OnEnSetfocusEditContactCleanOffset8)
	ON_BN_CLICKED(IDC_RADIO_CLEAN_OFF_USE, &CSPD_TM_CONTACTOR::OnBnClickedRadioCleanOffUse)
	ON_BN_CLICKED(IDC_RADIO_CLEAN_OFFSET_NOT_USE, &CSPD_TM_CONTACTOR::OnBnClickedRadioCleanOffsetNotUse)
END_MESSAGE_MAP()


// CSPD_TM_CONTACTOR 메시지 처리기입니다.


BOOL CSPD_TM_CONTACTOR::OnInitDialog()
{
	CDialog::OnInitDialog();

	char szCompany[16] = { 0, };
	CString strCustomer = _T("");
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));
	int nProjectOption = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	char szPathFileName[MAX_PATH] = { 0, };
	char szPathTemp[512] = { 0, };
	char szFilePath[MAX_PATH] = { 0, };

	::GetCurrentDirectory(sizeof(szPathTemp), szPathTemp);
	sprintf_s(szPathFileName, sizeof(szPathFileName), "%s\\sys_data\\%s", szPathTemp, SZ_FILE_PATH);
	CConfigData ConfigPathData(szPathFileName);

	// Vibrator Option
	m_szCompanyVibratorMode[0] = { 0, };
	CString strTemp = _T("");
	sprintf_s(szPathFileName, sizeof(szPathFileName), "%s\\sys_data\\%s", szPathTemp, SZ_DRIVER_FILE_NAME);
	CConfigData ConfigVibMode(szPathFileName);
	ConfigVibMode.GetString("Mode", "Vibrator Mode", DEF_NOT_USE, m_szCompanyVibratorMode);
	//

	GetDlgItem(IDC_RADIO_VIBRATOR_USE)->ShowWindow(TRUE);
	GetDlgItem(IDC_RADIO_VIBRATOR_NOT_USE)->ShowWindow(TRUE);
	GetDlgItem(IDC_GROUP_VIRATOR_OPTION)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_VIB_CNT)->ShowWindow(TRUE);
	GetDlgItem(IDC_EDIT_VIB_RE_CNT)->ShowWindow(TRUE);


	int nEnable = (int)g_DMCont.m_dmHandlerOpt.GB(BDM11_PRS_DEVICE_ARRIVE_ENABLE); //0 : Disable 1: Enable
	int nOpt = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRS_DEVICE_ARRIVE_OPTION); //0 : All Socket 1 : Only Socket Open
	int nUse = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_VIBRATOR_OPTION);  // 0 :Not USe 1: Use
	int	nRetryCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_VIBRATOR_RETRY_CNT);
	double dDelayTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_PRESS_VIBRATOR_DELAY_TIME);
	int	nBlowPos = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_BLOW_POS_OPTION);
	int	nCleanOffsetOpt = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_CLEAN_OFFSET_OPTION);

	int nAlwayVibOtnUse = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_ALWAY_VIBRATOR_OPTION);  // 0 :Not USe 1: Use
	int	nAlwayRetryCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_ALWAY_VIBRATOR_RETRY_CNT);
	double dAlwayDelayTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_PRESS_ALWAY_VIBRATOR_DELAY_TIME);

	int nReContactEnable = (int)g_DMCont.m_dmHandlerOpt.GB(BDM11_PRS_RECONTACT_ENABLE); //0 : Disable 1: Enable

	int	nReContactCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_RECONTACT_CNT);
	double	dReContactOffset = g_DMCont.m_dmHandlerOpt.GD(DDM11_PRESS_RECONTACT_OFFSET);
	double dReContactDelayTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_PRESS_RECONTACT_DELAY_TIME);

	SetPressDeviceOption(nEnable, nOpt, nUse, nRetryCnt, nBlowPos, nCleanOffsetOpt,
		dDelayTime, nAlwayVibOtnUse, nAlwayRetryCnt, dAlwayDelayTime, nReContactEnable, nReContactCnt, dReContactOffset, dReContactDelayTime);


	GetDlgItem(IDC_STATIC_GROUP_CONTACT)->ShowWindow(SW_SHOW);


	int nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);



	GetDlgItem(IDC_STATIC_CONTACT_POS_OFFSET)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_CONTACT_POS_OFFSET)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT228)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_VIBRATION)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_VIBRATION_USE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_VIBRATION_NOT_USE)->ShowWindow(SW_HIDE);

// 	GetDlgItem(IDC_STATIC_CONTACT_2ST_SPEED)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_EDIT_CONTACT_2ST_SPEED)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STATIC_TEXT229)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_CONTACT_LOG_OPTION)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_CONTACT_LOG_USE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_CONTACT_LOG_NOT_USE)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_SOCKET_CLEAN_FUNCTION)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_SOCKET_CLEAN_FUNCTION_USE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_SOCKET_CLEAN_FUNCTION_NOT_USE)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_SOCKET_CLEAN_BLOW_POS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_SOCKET_CLEAN_BLOW_POS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT230)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_SOCKET_CLEAN_BLOW_TIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_SOCKET_CLEAN_BLOW_TIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT231)->ShowWindow(SW_HIDE);

	






	GetDlgItem(IDC_GROUP_ALWAY_VIRATOR_OPTION)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_ALWAY_VIBRATOR_USE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_ALWAY_VIBRATOR_NOT_USE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_ALWAY_VIB_CNT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_ALWAY_VIB_RE_CNT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_ALWAY_VIB_DELAY_TIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_ALWAY_VIB_DELAY_TIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_RECONTACT_OFFSET)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT224)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT226)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_RECONTACT_DELAY_TIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_RECONTACT_DELAY_TIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT_RECONTACT_DELAY_TIME_SEC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT225)->ShowWindow(SW_HIDE);
	

	if (strcmp(szCompany, DEF_COMPANY_AMKOR) == 0)
	{
		GetDlgItem(IDC_STATIC_RECONTACT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_RECONTACT_USE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_RECONTACT_NOT_USE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECONTACT_CNT)->ShowWindow(SW_HIDE);

	}

	OnBtnGUI();
	OnEditBoxGUI();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSPD_TM_CONTACTOR::OnBtnGUI()
{
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_BtnSave.SetShade(CShadeButtonST::SHS_SOFTBUMP,8,20,0,RGB(55,55,255));
	m_BtnSave.SetIcon(IDI_BTN_SAVE);
	m_BtnSave.SetAlign(CButtonST::ST_ALIGN_VERT);
}

void CSPD_TM_CONTACTOR::OnEditBoxGUI()
{
	m_editSPD_PressUnit1	.SetBackColor(COLOR_WWHITE);
	m_editACC_PressUnit1	.SetBackColor(COLOR_WWHITE);
	m_editDCC_PressUnit1	.SetBackColor(COLOR_WWHITE);
	m_editSPD_PressUnit2	.SetBackColor(COLOR_WWHITE);
	m_editACC_PressUnit2	.SetBackColor(COLOR_WWHITE);
	m_editDCC_PressUnit2	.SetBackColor(COLOR_WWHITE);
	m_editSPD_PressUnit3	.SetBackColor(COLOR_WWHITE);
	m_editACC_PressUnit3	.SetBackColor(COLOR_WWHITE);
	m_editDCC_PressUnit3	.SetBackColor(COLOR_WWHITE);
	m_editSPD_PressUnit4	.SetBackColor(COLOR_WWHITE);
	m_editACC_PressUnit4	.SetBackColor(COLOR_WWHITE);
	m_editDCC_PressUnit4	.SetBackColor(COLOR_WWHITE);
	m_editSPD_PressUnit5	.SetBackColor(COLOR_WWHITE);
	m_editACC_PressUnit5	.SetBackColor(COLOR_WWHITE);
	m_editDCC_PressUnit5	.SetBackColor(COLOR_WWHITE);
	m_editSPD_PressUnit6	.SetBackColor(COLOR_WWHITE);
	m_editACC_PressUnit6	.SetBackColor(COLOR_WWHITE);
	m_editDCC_PressUnit6	.SetBackColor(COLOR_WWHITE);
	m_editSPD_PressUnit7	.SetBackColor(COLOR_WWHITE);
	m_editACC_PressUnit7	.SetBackColor(COLOR_WWHITE);
	m_editDCC_PressUnit7	.SetBackColor(COLOR_WWHITE);
	m_editSPD_PressUnit8	.SetBackColor(COLOR_WWHITE);
	m_editACC_PressUnit8	.SetBackColor(COLOR_WWHITE);
	m_editDCC_PressUnit8	.SetBackColor(COLOR_WWHITE);

	m_editBlow_PressUnit1   .SetBackColor(COLOR_WWHITE);
	m_editBlow_PressUnit2   .SetBackColor(COLOR_WWHITE);
	m_editBlow_PressUnit3   .SetBackColor(COLOR_WWHITE);
	m_editBlow_PressUnit4   .SetBackColor(COLOR_WWHITE);
	m_editBlow_PressUnit5   .SetBackColor(COLOR_WWHITE);
	m_editBlow_PressUnit6   .SetBackColor(COLOR_WWHITE);
	m_editBlow_PressUnit7   .SetBackColor(COLOR_WWHITE);
	m_editBlow_PressUnit8   .SetBackColor(COLOR_WWHITE);

	m_edit_PressUnit_Blow_Pos1.SetBackColor(COLOR_WWHITE);
	m_edit_PressUnit_Blow_Pos2.SetBackColor(COLOR_WWHITE);
	m_edit_PressUnit_Blow_Pos3.SetBackColor(COLOR_WWHITE);
	m_edit_PressUnit_Blow_Pos4.SetBackColor(COLOR_WWHITE);
	m_edit_PressUnit_Blow_Pos5.SetBackColor(COLOR_WWHITE);
	m_edit_PressUnit_Blow_Pos6.SetBackColor(COLOR_WWHITE);
	m_edit_PressUnit_Blow_Pos7.SetBackColor(COLOR_WWHITE);
	m_edit_PressUnit_Blow_Pos8.SetBackColor(COLOR_WWHITE);

	m_edit_PressUnit_Clean_Offset1.SetBackColor(COLOR_WWHITE);
	m_edit_PressUnit_Clean_Offset2.SetBackColor(COLOR_WWHITE);
	m_edit_PressUnit_Clean_Offset3.SetBackColor(COLOR_WWHITE);
	m_edit_PressUnit_Clean_Offset4.SetBackColor(COLOR_WWHITE);
	m_edit_PressUnit_Clean_Offset5.SetBackColor(COLOR_WWHITE);
	m_edit_PressUnit_Clean_Offset6.SetBackColor(COLOR_WWHITE);
	m_edit_PressUnit_Clean_Offset7.SetBackColor(COLOR_WWHITE);
	m_edit_PressUnit_Clean_Offset8.SetBackColor(COLOR_WWHITE);

	m_EditReCnt.SetBackColor(COLOR_WWHITE);
	m_EditDelayTime.SetBackColor(COLOR_WWHITE);

	m_EditAlwayReCnt.SetBackColor(COLOR_WWHITE);
	m_EditAlwayDelayTime.SetBackColor(COLOR_WWHITE);

	m_EditReContactCnt.SetBackColor(COLOR_WWHITE);
	m_EditReContactOffset.SetBackColor(COLOR_WWHITE);
	m_EditReContactDelayTime.SetBackColor(COLOR_WWHITE);

	m_EditContactPosOffset.SetBackColor(COLOR_WWHITE);
	m_EditContact2stSpeed.SetBackColor(COLOR_WWHITE);
	m_EditCleaning2ndZOffset.SetBackColor(COLOR_WWHITE);

	m_EditSocketCleanBlowPos.SetBackColor(COLOR_WWHITE);
	m_EditSocketCleanBlowTime.SetBackColor(COLOR_WWHITE);

	m_editSPD_PressUnit1	.SetTextSize(30);
	m_editACC_PressUnit1	.SetTextSize(30);
	m_editDCC_PressUnit1	.SetTextSize(30);
	m_editSPD_PressUnit2	.SetTextSize(30);
	m_editACC_PressUnit2	.SetTextSize(30);
	m_editDCC_PressUnit2	.SetTextSize(30);
	m_editSPD_PressUnit3	.SetTextSize(30);
	m_editACC_PressUnit3	.SetTextSize(30);
	m_editDCC_PressUnit3	.SetTextSize(30);
	m_editSPD_PressUnit4	.SetTextSize(30);
	m_editACC_PressUnit4	.SetTextSize(30);
	m_editDCC_PressUnit4	.SetTextSize(30);
	m_editSPD_PressUnit5	.SetTextSize(30);
	m_editACC_PressUnit5	.SetTextSize(30);
	m_editDCC_PressUnit5	.SetTextSize(30);
	m_editSPD_PressUnit6	.SetTextSize(30);
	m_editACC_PressUnit6	.SetTextSize(30);
	m_editDCC_PressUnit6	.SetTextSize(30);
	m_editSPD_PressUnit7	.SetTextSize(30);
	m_editACC_PressUnit7	.SetTextSize(30);
	m_editDCC_PressUnit7	.SetTextSize(30);
	m_editSPD_PressUnit8	.SetTextSize(30);
	m_editACC_PressUnit8	.SetTextSize(30);
	m_editDCC_PressUnit8	.SetTextSize(30);

	m_editBlow_PressUnit1   .SetTextSize(30);
	m_editBlow_PressUnit2 	.SetTextSize(30);
	m_editBlow_PressUnit3 	.SetTextSize(30);
	m_editBlow_PressUnit4 	.SetTextSize(30);
	m_editBlow_PressUnit5 	.SetTextSize(30);
	m_editBlow_PressUnit6 	.SetTextSize(30);
	m_editBlow_PressUnit7 	.SetTextSize(30);
	m_editBlow_PressUnit8 	.SetTextSize(30);

	m_edit_PressUnit_Blow_Pos1.SetTextSize(30);
	m_edit_PressUnit_Blow_Pos2.SetTextSize(30);
	m_edit_PressUnit_Blow_Pos3.SetTextSize(30);
	m_edit_PressUnit_Blow_Pos4.SetTextSize(30);
	m_edit_PressUnit_Blow_Pos5.SetTextSize(30);
	m_edit_PressUnit_Blow_Pos6.SetTextSize(30);
	m_edit_PressUnit_Blow_Pos7.SetTextSize(30);
	m_edit_PressUnit_Blow_Pos8.SetTextSize(30);

	m_edit_PressUnit_Clean_Offset1.SetTextSize(30);
	m_edit_PressUnit_Clean_Offset2.SetTextSize(30);
	m_edit_PressUnit_Clean_Offset3.SetTextSize(30);
	m_edit_PressUnit_Clean_Offset4.SetTextSize(30);
	m_edit_PressUnit_Clean_Offset5.SetTextSize(30);
	m_edit_PressUnit_Clean_Offset6.SetTextSize(30);
	m_edit_PressUnit_Clean_Offset7.SetTextSize(30);
	m_edit_PressUnit_Clean_Offset8.SetTextSize(30);
	
	m_EditReCnt.SetTextSize(30);
	m_EditDelayTime.SetTextSize(30);

	m_EditAlwayReCnt.SetTextSize(30);
	m_EditAlwayDelayTime.SetTextSize(30);

	m_EditReContactCnt.SetTextSize(30);
	m_EditReContactOffset.SetTextSize(30);
	m_EditReContactDelayTime.SetTextSize(30);

	m_EditContactPosOffset.SetTextSize(30);
	m_EditContact2stSpeed.SetTextSize(30);
	m_EditCleaning2ndZOffset.SetTextSize(30);

	m_EditSocketCleanBlowPos.SetTextSize(30);
	m_EditSocketCleanBlowTime.SetTextSize(30);
}

void CSPD_TM_CONTACTOR::OnBnClickedButtonSave()
{
	char szCompany[16] = { 0, };
	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	int nProjectName = g_DMCont.m_dmEQP.GN(NDM0_Handler_Project_Name);

	UpdateData(TRUE);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit1, m_nSPD_PressUnit1);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit1, m_nACC_PressUnit1);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit1, m_nDCC_PressUnit1);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit2, m_nSPD_PressUnit2);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit2, m_nACC_PressUnit2);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit2, m_nDCC_PressUnit2);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit3, m_nSPD_PressUnit3);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit3, m_nACC_PressUnit3);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit3, m_nDCC_PressUnit3);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit4, m_nSPD_PressUnit4);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit4, m_nACC_PressUnit4);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit4, m_nDCC_PressUnit4);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit5, m_nSPD_PressUnit5);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit5, m_nACC_PressUnit5);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit5, m_nDCC_PressUnit5);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit6, m_nSPD_PressUnit6);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit6, m_nACC_PressUnit6);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit6, m_nDCC_PressUnit6);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit7, m_nSPD_PressUnit7);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit7, m_nACC_PressUnit7);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit7, m_nDCC_PressUnit7);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_SPD_PressUnit8, m_nSPD_PressUnit8);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_ACC_PressUnit8, m_nACC_PressUnit8);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_DCC_PressUnit8, m_nDCC_PressUnit8);

	g_DMCont.m_dmHandlerOpt.SN(NDM11_TM_PRESS1_BLOW_WAIT, (int)(m_dBlow_PressUnit1 * 1000));
	g_DMCont.m_dmHandlerOpt.SN(NDM11_TM_PRESS2_BLOW_WAIT, (int)(m_dBlow_PressUnit2 * 1000));
	g_DMCont.m_dmHandlerOpt.SN(NDM11_TM_PRESS3_BLOW_WAIT, (int)(m_dBlow_PressUnit3 * 1000));
	g_DMCont.m_dmHandlerOpt.SN(NDM11_TM_PRESS4_BLOW_WAIT, (int)(m_dBlow_PressUnit4 * 1000));
	g_DMCont.m_dmHandlerOpt.SN(NDM11_TM_PRESS5_BLOW_WAIT, (int)(m_dBlow_PressUnit5 * 1000));
	g_DMCont.m_dmHandlerOpt.SN(NDM11_TM_PRESS6_BLOW_WAIT, (int)(m_dBlow_PressUnit6 * 1000));
	g_DMCont.m_dmHandlerOpt.SN(NDM11_TM_PRESS7_BLOW_WAIT, (int)(m_dBlow_PressUnit7 * 1000));
	g_DMCont.m_dmHandlerOpt.SN(NDM11_TM_PRESS8_BLOW_WAIT, (int)(m_dBlow_PressUnit8 * 1000));

	g_DMCont.m_dmHandlerOpt.SD(DDM11_TM_PRESS1_BLOW_POS, m_dPressUnit_Blow_Pos1);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_TM_PRESS2_BLOW_POS, m_dPressUnit_Blow_Pos2);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_TM_PRESS3_BLOW_POS, m_dPressUnit_Blow_Pos3);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_TM_PRESS4_BLOW_POS, m_dPressUnit_Blow_Pos4);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_TM_PRESS5_BLOW_POS, m_dPressUnit_Blow_Pos5);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_TM_PRESS6_BLOW_POS, m_dPressUnit_Blow_Pos6);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_TM_PRESS7_BLOW_POS, m_dPressUnit_Blow_Pos7);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_TM_PRESS8_BLOW_POS, m_dPressUnit_Blow_Pos8);

	g_DMCont.m_dmHandlerOpt.SD(DDM11_TM_PRESS1_CLEAN_OFFSET, m_dPressUnit_Clean_Offset1);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_TM_PRESS2_CLEAN_OFFSET, m_dPressUnit_Clean_Offset2);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_TM_PRESS3_CLEAN_OFFSET, m_dPressUnit_Clean_Offset3);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_TM_PRESS4_CLEAN_OFFSET, m_dPressUnit_Clean_Offset4);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_TM_PRESS5_CLEAN_OFFSET, m_dPressUnit_Clean_Offset5);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_TM_PRESS6_CLEAN_OFFSET, m_dPressUnit_Clean_Offset6);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_TM_PRESS7_CLEAN_OFFSET, m_dPressUnit_Clean_Offset7);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_TM_PRESS8_CLEAN_OFFSET, m_dPressUnit_Clean_Offset8);


	g_DMCont.m_dmHandlerOpt.SB(BDM11_PRS_DEVICE_ARRIVE_ENABLE, m_nDeviceCheckStatus); //0 : Disable 1: Enable
	g_DMCont.m_dmHandlerOpt.SN(NDM11_PRS_DEVICE_ARRIVE_OPTION, m_nDeviceCheckOption); //0 : All Socket 1 : Only Socket Open
	
	g_DMCont.m_dmHandlerOpt.SN(NDM11_PRESS_BLOW_POS_OPTION, m_nBlowPosOption); // 1 : USE , 0: NOT_USE
	g_DMCont.m_dmHandlerOpt.SN(NDM11_PRESS_CLEAN_OFFSET_OPTION, m_nCleanOffsetOption); // 1 : Contact Pos , 0: NOT_USE

    //Vibrator Option
	g_DMCont.m_dmHandlerOpt.SN(NDM11_PRESS_VIBRATOR_OPTION, m_nViratorOption);
	int nRetryCnt = GetDlgItemInt(IDC_EDIT_VIB_RE_CNT);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_PRESS_VIBRATOR_RETRY_CNT, nRetryCnt);
	CString strDelayTime;
	GetDlgItemText(IDC_EDIT_VIB_DELAY_TIME, strDelayTime);
	double dDelayTime = atof(strDelayTime);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_PRESS_VIBRATOR_DELAY_TIME, dDelayTime);

	//Alway Vibrator Option
	g_DMCont.m_dmHandlerOpt.SN(NDM11_PRESS_ALWAY_VIBRATOR_OPTION, m_nAlwayViratorOption);
	int nAlwayVibRetryCnt = GetDlgItemInt(IDC_EDIT_ALWAY_VIB_RE_CNT);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_PRESS_ALWAY_VIBRATOR_RETRY_CNT, nAlwayVibRetryCnt);

	GetDlgItemText(IDC_EDIT_ALWAY_VIB_DELAY_TIME, strDelayTime);
	double dAlwayVibDelayTime = atof(strDelayTime);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_PRESS_ALWAY_VIBRATOR_DELAY_TIME, dAlwayVibDelayTime);
	////

	g_DMCont.m_dmHandlerOpt.SB(BDM11_PRS_RECONTACT_ENABLE, m_nReContactOption);
	int nReContactCnt = GetDlgItemInt(IDC_EDIT_RECONTACT_CNT);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_PRESS_RECONTACT_CNT, nReContactCnt);

	CString strOffset;
	GetDlgItemText(IDC_EDIT_RECONTACT_OFFSET, strOffset);
	double dReContactOffset = atof(strOffset);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_PRESS_RECONTACT_OFFSET, dReContactOffset);
	
	CString strRecontactDelayTime;
	GetDlgItemText(IDC_EDIT_RECONTACT_DELAY_TIME, strRecontactDelayTime);
	double dReContactDelayTime = atof(strRecontactDelayTime);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_PRESS_RECONTACT_DELAY_TIME, dReContactDelayTime);

	CString strContactPosOffset;
	GetDlgItemText(IDC_EDIT_CONTACT_POS_OFFSET, strContactPosOffset);
	double dContactPosOffset = atof(strContactPosOffset);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_PRESS_CONTACT_POS_OFFSET, dContactPosOffset);
		
	int nContact2stSpeed = GetDlgItemInt(IDC_EDIT_CONTACT_2ST_SPEED);
	g_DMCont.m_dmHandlerOpt.SN(NDM11_PRESS_CONTACT_2ST_SPEED, nContact2stSpeed);

	double dCleaning2ndZOffset = GetDlgItemInt(IDC_EDIT_CLEANING_2ND_Z_OFFSET);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_PRESS_CLEANING_2ND_Z_OFFSET, dCleaning2ndZOffset * 1000);

	CString strSocketCleanBlowPos;
	GetDlgItemText(IDC_EDIT_SOCKET_CLEAN_BLOW_POS, strSocketCleanBlowPos);
	double dSocketCleanBlowPos = atof(strSocketCleanBlowPos);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_PRESS_SOCKET_CLEAN_BLOW_POS, dSocketCleanBlowPos);

	CString strSocketCleanBlowTime;
	GetDlgItemText(IDC_EDIT_SOCKET_CLEAN_BLOW_TIME, strSocketCleanBlowTime);
	double dSocketCleanBlowTime = atof(strSocketCleanBlowTime);
	g_DMCont.m_dmHandlerOpt.SD(DDM11_PRESS_SOCKET_CLEAN_BLOW_TIME, dSocketCleanBlowTime);
	

	char szCokName[512] = { 0, };
	ST_COK_INFO   stCokData, stBeforeCokData;
	g_DMCont.m_dmHandlerOpt.GS(SDM11_TrayFileName, szCokName, sizeof(szCokName));
	g_COK_FileMgr.LoadCokInfo(szCokName, stCokData);

	stBeforeCokData = stCokData;

	int nPosNum[] = { NDM11_SPD_PressUnit1, NDM11_SPD_PressUnit2, NDM11_SPD_PressUnit3, NDM11_SPD_PressUnit4, NDM11_SPD_PressUnit5, NDM11_SPD_PressUnit6, NDM11_SPD_PressUnit7, NDM11_SPD_PressUnit8};
	int nBlowNum[] = { NDM11_TM_PRESS1_BLOW_WAIT, NDM11_TM_PRESS2_BLOW_WAIT, NDM11_TM_PRESS3_BLOW_WAIT, NDM11_TM_PRESS4_BLOW_WAIT, NDM11_TM_PRESS5_BLOW_WAIT, NDM11_TM_PRESS6_BLOW_WAIT, NDM11_TM_PRESS7_BLOW_WAIT,NDM11_TM_PRESS8_BLOW_WAIT };
	double dBlowPos[] = { DDM11_TM_PRESS1_BLOW_POS, DDM11_TM_PRESS2_BLOW_POS, DDM11_TM_PRESS3_BLOW_POS, DDM11_TM_PRESS4_BLOW_POS, DDM11_TM_PRESS5_BLOW_POS, DDM11_TM_PRESS6_BLOW_POS, DDM11_TM_PRESS7_BLOW_POS,DDM11_TM_PRESS8_BLOW_POS};
	double dCleanOffset[] = { DDM11_TM_PRESS1_CLEAN_OFFSET,DDM11_TM_PRESS2_CLEAN_OFFSET, DDM11_TM_PRESS3_CLEAN_OFFSET, DDM11_TM_PRESS4_CLEAN_OFFSET, DDM11_TM_PRESS5_CLEAN_OFFSET, DDM11_TM_PRESS6_CLEAN_OFFSET, DDM11_TM_PRESS7_CLEAN_OFFSET,DDM11_TM_PRESS8_CLEAN_OFFSET };

	for (int i = 0; i <= RIDX_PRESS_UNIT_8; i++)
	{
		stCokData.aPressUnit_SPD[i].aAxisForm[eAxis_Z].nSPD = g_DMCont.m_dmHandlerOpt.GN(nPosNum[i]);
		stCokData.aPressUnit_SPD[i].aAxisForm[eAxis_Z].nACC = g_DMCont.m_dmHandlerOpt.GN(nPosNum[i] + 1);
		stCokData.aPressUnit_SPD[i].aAxisForm[eAxis_Z].nDCC = g_DMCont.m_dmHandlerOpt.GN(nPosNum[i] + 2);
		stCokData.aPress_Tm[i].nBlowTm = g_DMCont.m_dmHandlerOpt.GN(nBlowNum[i]);
		stCokData.aPress_Tm[i].dBlowPos = g_DMCont.m_dmHandlerOpt.GD(dBlowPos[i]);
		stCokData.aPress_Tm[i].dCleanOffset = g_DMCont.m_dmHandlerOpt.GD(dCleanOffset[i]);
	}

	g_COK_FileMgr.SaveCokInfo(szCokName, &stCokData);
	ST_BASIC_INFO   stBasicData;
	g_COK_FileMgr.LoadBasicInfo(szCokName, stBasicData);
	stBasicData.bPressDeviceChk = m_nDeviceCheckStatus;
	stBasicData.nPressDeviceChkOpt = m_nDeviceCheckOption;
	stBasicData.nPressVibratorChkOpt = m_nViratorOption;
	stBasicData.nPressVibratorRetryCnt = nRetryCnt;
	stBasicData.dPressVibratorDelayTime = dDelayTime;
	stBasicData.nPressBlowPosChkOpt = m_nBlowPosOption;
	stBasicData.nCleanOffsetOption = m_nCleanOffsetOption;

	stBasicData.nPressAlwayVibratorChkOpt = m_nAlwayViratorOption;
	stBasicData.nPressAlwayVibratorRetryCnt = nAlwayVibRetryCnt;
	stBasicData.dPressAlwayVibratorDelayTime = dAlwayVibDelayTime;

	stBasicData.dContactPos0ffset = m_dContactPosOffset;

	stBasicData.nVibrationOption = m_nVibrationOption;

	stBasicData.nContact2stSpeed = m_nContact2stSpeed;
	stBasicData.dCleaning2ndZOffset = m_dCleaning2ndZOffset;

	stBasicData.nContactLogOption = m_nContactLogOption;
	stBasicData.nSocketCleanFunctionOption = m_nSocketCleanFunctionOption;

	stBasicData.dSocketCleanBlowPos = m_dSocketCleanBlowPos;
	stBasicData.dSocketCleanBlowTime = m_dSocketCleanBlowTime;

	g_COK_FileMgr.SaveBasicInfo(szCokName, &stBasicData);

	//GUI Data Change Log
	std::vector<std::pair<CString, std::pair<double, double>>> vData;  // vData.first = stCokData, vData.second.first = BeforeData vData.second.second = AfterData

	stBeforeCokData.IsSamePressUnit(stCokData, vData);

	if (!(vData.empty()))
	{
		PressUnitSpeedTiemChangeLog(vData);
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditSpdContact1()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nSPD_PressUnit1, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #1 Speed", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_SPD_CONTACT1)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditAccContact1()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nACC_PressUnit1, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #1 Accel", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_ACC_CONTACT1)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}

void CSPD_TM_CONTACTOR::OnEnSetfocusEditDccContact1()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nDCC_PressUnit1, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #1 Decel", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DCC_CONTACT1)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditSpdContact2()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nSPD_PressUnit2, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #2 Speed", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_SPD_CONTACT2)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditAccContact2()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nACC_PressUnit2, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #2 Accel", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_ACC_CONTACT2)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditDccContact2()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nDCC_PressUnit2, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #2 Decel", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DCC_CONTACT2)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditSpdContact3()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nSPD_PressUnit3, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #3 Speed", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_SPD_CONTACT3)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditAccContact3()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nACC_PressUnit3, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #3 Accel", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_ACC_CONTACT3)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditDccContact3()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nDCC_PressUnit3, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #3 Decel", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DCC_CONTACT3)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditSpdContact4()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nSPD_PressUnit4, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #4 Speed", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_SPD_CONTACT4)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditAccContact4()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nACC_PressUnit4, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #4 Accel", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_ACC_CONTACT4)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditDccContact4()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nDCC_PressUnit4, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #4 Decel", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DCC_CONTACT4)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditSpdContact5()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nSPD_PressUnit5, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #5 Speed", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_SPD_CONTACT5)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditAccContact5()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nACC_PressUnit5, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #5 Accel", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_ACC_CONTACT5)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditDccContact5()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nDCC_PressUnit5, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #5 Decel", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DCC_CONTACT5)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditSpdContact6()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nSPD_PressUnit6, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #6 Speed", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_SPD_CONTACT6)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditAccContact6()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nACC_PressUnit6, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #6 Accel", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_ACC_CONTACT6)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditDccContact6()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nDCC_PressUnit6, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #6 Decel", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DCC_CONTACT6)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditSpdContact7()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nSPD_PressUnit7, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #7 Speed", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_SPD_CONTACT7)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditAccContact7()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nACC_PressUnit7, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #7 Accel", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_ACC_CONTACT7)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditDccContact7()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nDCC_PressUnit7, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #7 Decel", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DCC_CONTACT7)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditSpdContact8()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nSPD_PressUnit8, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #8 Speed", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_SPD_CONTACT8)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditAccContact8()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nACC_PressUnit8, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #8 Accel", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_ACC_CONTACT8)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditDccContact8()
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	_itoa_s(m_nDCC_PressUnit8, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Press Unit #8 Decel", "100", "1");

	if(nRet == RETURN_OK){
		GetDlgItem(IDC_EDIT_DCC_CONTACT8)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::MakeLog(LPCTSTR fmt, ...)
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
void CSPD_TM_CONTACTOR::PressUnitSpeedTiemChangeLog(std::vector<std::pair<CString,std::pair<double,double>>> vData )
{
	for(int i = 0 ; i< (int)vData.size(); i++)
	{
		std::pair<CString,std::pair<double,double>> p;

		p = vData[i];

		MakeLog("[MENU = Press Unit Speed & Time, DATA NAME = %s][BEFORE= %.2f, AFTER= %.2f]", p.first, p.second.first, p.second.second);
	}
}

void CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowTime1()
{
	NumberBox(m_dBlow_PressUnit1, IDC_EDIT_BLOW_TIME_1,"Press Unit #1 Blow Time","5","0");
}

void CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowTime2()
{
	NumberBox(m_dBlow_PressUnit2, IDC_EDIT_BLOW_TIME_2,"Press Unit #2 Blow Time","5","0");
}

void CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowTime3()
{
	NumberBox(m_dBlow_PressUnit3, IDC_EDIT_BLOW_TIME_3,"Press Unit #3 Blow Time","5","0");
}

void CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowTime4()
{
	NumberBox(m_dBlow_PressUnit4, IDC_EDIT_BLOW_TIME_4,"Press Unit #4 Blow Time","5","0");
}

void CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowTime5()
{
	NumberBox(m_dBlow_PressUnit5, IDC_EDIT_BLOW_TIME_5,"Press Unit #5 Blow Time","5","0");
}

void CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowTime6()
{
	NumberBox(m_dBlow_PressUnit6, IDC_EDIT_BLOW_TIME_6,"Press Unit #6 Blow Time","5","0");
}

void CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowTime7()
{
	NumberBox(m_dBlow_PressUnit7, IDC_EDIT_BLOW_TIME_7,"Press Unit #7 Blow Time","5","0");
}

void CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowTime8()
{
	NumberBox(m_dBlow_PressUnit8, IDC_EDIT_BLOW_TIME_8,"Press Unit #8 Blow Time","5","0");
}

void CSPD_TM_CONTACTOR::NumberBox(double dVal, int nControlID, char* szTitle, char* szMax, char* szMin)
{
	char szVal[10]    = {0,};
	char szRetVal[64] = {0,};
	sprintf_s(szVal, sizeof(szVal), "%0.2f", dVal);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, szTitle, szMax, szMin);

	if(nRet == RETURN_OK){
		GetDlgItem(nControlID)->SetWindowText(szRetVal);
	}else if( nRet == RETURN_ERR ) {
		AfxMessageBox( _T("Range Over"));
	}
}

void CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowPos1()
{
	NumberBox(m_dPressUnit_Blow_Pos1, IDC_EDIT_BLOW_POS_1, "Press Unit #1 Blow Pos", "0", "-10");
}																				
																				
void CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowPos2()
{																				
	NumberBox(m_dPressUnit_Blow_Pos2, IDC_EDIT_BLOW_POS_2, "Press Unit #2 Blow Pos", "0", "-10");
}

void CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowPos3()
{
	NumberBox(m_dPressUnit_Blow_Pos3, IDC_EDIT_BLOW_POS_3, "Press Unit #3 Blow Pos", "0", "-10");
}

void CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowPos4()
{
	NumberBox(m_dPressUnit_Blow_Pos4, IDC_EDIT_BLOW_POS_4, "Press Unit #4 Blow Pos", "0", "-10");
}																				
																				
void CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowPos5()
{																				
	NumberBox(m_dPressUnit_Blow_Pos5, IDC_EDIT_BLOW_POS_5, "Press Unit #5 Blow Pos", "0", "-10");
}

void CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowPos6()
{
	NumberBox(m_dPressUnit_Blow_Pos6, IDC_EDIT_BLOW_POS_6, "Press Unit #6 Blow Pos", "0", "-10");
}

void CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowPos7()
{
	NumberBox(m_dPressUnit_Blow_Pos7, IDC_EDIT_BLOW_POS_7, "Press Unit #7 Blow Pos", "0", "-10");
}

void CSPD_TM_CONTACTOR::OnEnSetfocusEditBlowPos8()
{
	NumberBox(m_dPressUnit_Blow_Pos8, IDC_EDIT_BLOW_POS_8, "Press Unit #8 Blow Pos", "0", "-10");
}																				


void CSPD_TM_CONTACTOR::SetVisibleSocketOption(BOOL enable)
{
	if(enable)
	{
		GetDlgItem(IDC_GROUP_PRESS_OPTION)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PRESS_RADIO_ALL_SOCKET)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PRESS_RADIO_ONLY_SOCKET_OPEN)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_GROUP_PRESS_OPTION)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_PRESS_RADIO_ALL_SOCKET)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_PRESS_RADIO_ONLY_SOCKET_OPEN)->ShowWindow(SW_HIDE);
	}
}

void CSPD_TM_CONTACTOR::SetPressDeviceOption(int nEnable, int nOpt, int nUse, int nRetCnt,
	int nContactOption, int nCleanOffsetOtn, double dDelayTime,int nAlwayVibOtn, int nAlwayRetryCnt,
	double dAlwayDelayTime, int nReContactEnable,int nReContactCnt, double dReContactOffset,double dReContactDelayTime)
{
	m_nDeviceCheckStatus = nEnable;
	m_nDeviceCheckOption = nOpt;
	m_nViratorOption = nUse;
	m_nAlwayViratorOption = nAlwayVibOtn;
	m_nCleanOffsetOption = nCleanOffsetOtn;
	m_nReContactOption = nReContactEnable;

	CString strReCnt = _T(""), strDelayTime = _T(""),strOffset= _T(""), strContactDelayTime = _T("");

	if (nEnable == DEF_ON)
	{
		m_rbChkDvcEnable.SetCheck(TRUE);
		m_rbChkDvcDisable.SetCheck(FALSE);
		
		if (nOpt == CHECK_ALL_SOCKET)
		{
			m_rbAllSocketOpt.SetCheck(TRUE);
			m_rbOnlySocketOpt.SetCheck(FALSE);
		}
		else
		{
			m_rbAllSocketOpt.SetCheck(FALSE);
			m_rbOnlySocketOpt.SetCheck(TRUE);
		}
	}
	else
	{
		m_nDeviceCheckStatus = DEF_OFF;

		m_rbChkDvcEnable.SetCheck(FALSE);
		m_rbChkDvcDisable.SetCheck(TRUE);
	}
	SetVisibleSocketOption(nEnable);

	if (nUse == CHECK_VIBRATOR_USE) 
	{
		m_rbChkVibUse.SetCheck(TRUE); //기본 값 할당.
		m_rbChkVibNotUse.SetCheck(FALSE);

		strReCnt.Format("%d", nRetCnt);
		m_EditReCnt.SetWindowText(strReCnt);

		strDelayTime.Format("%.2f", dDelayTime);
		m_EditDelayTime.SetWindowText(strDelayTime);

		if(strcmp(m_szCompanyVibratorMode, DEF_USE) == 0)
		{
			GetDlgItem(IDC_STATIC_VIB_CNT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_VIB_RE_CNT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_VIB_DELAY_TIME)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_VIB_DELAY_TIME)->ShowWindow(SW_SHOW);
			
		}
	}
	else
	{
		m_rbChkVibUse.SetCheck(FALSE); //기본 값 할당.
		m_rbChkVibNotUse.SetCheck(TRUE);
		GetDlgItem(IDC_STATIC_VIB_CNT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_VIB_RE_CNT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_VIB_DELAY_TIME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_VIB_DELAY_TIME)->ShowWindow(SW_HIDE);
	}

	//Alway Vibrator
	if (nAlwayVibOtn == CHECK_VIBRATOR_USE)
	{
		m_rbChkAlwayVibUse.SetCheck(TRUE); //기본 값 할당.
		m_rbChkAlwayVibNotUse.SetCheck(FALSE);

		strReCnt.Format("%d", nAlwayRetryCnt);
		m_EditAlwayReCnt.SetWindowText(strReCnt);

		strDelayTime.Format("%.2f", dAlwayDelayTime);
		m_EditAlwayDelayTime.SetWindowText(strDelayTime);

		GetDlgItem(IDC_STATIC_ALWAY_VIB_CNT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_ALWAY_VIB_RE_CNT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_ALWAY_VIB_DELAY_TIME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_ALWAY_VIB_DELAY_TIME)->ShowWindow(SW_SHOW);	
	}
	else
	{
		m_rbChkAlwayVibUse.SetCheck(FALSE); //기본 값 할당.
		m_rbChkAlwayVibNotUse.SetCheck(TRUE);
		GetDlgItem(IDC_STATIC_ALWAY_VIB_CNT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ALWAY_VIB_RE_CNT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_ALWAY_VIB_DELAY_TIME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_ALWAY_VIB_DELAY_TIME)->ShowWindow(SW_HIDE);
	}

	int pID[eMaxPressUnitCount];
	for (int i = 0; i < eMaxPressUnitCount; i++)
	{
		pID[i] = IDC_EDIT_CONTACT_POS_1 + i;
		GetDlgItem(pID[i])->EnableWindow(m_nBlowPosOption);
	}

	ContactPosEnable(m_nBlowPosOption);
	m_rbChkContactUse.SetCheck(m_nBlowPosOption); //기본 값 할당.
	m_rbChkContactNotUse.SetCheck(!m_nBlowPosOption);


	CleanOffsetEnable(m_nCleanOffsetOption);
	m_rbChkCleanOffsetUse.SetCheck(m_nCleanOffsetOption); //기본 값 할당.
	m_rbChkCleanOffsetNotUse.SetCheck(!m_nCleanOffsetOption);

	m_rbChkVibrationUse.SetCheck(m_nVibrationOption); //기본 값 할당.
	m_rbChkVibrationNotUse.SetCheck(!m_nVibrationOption);


	m_rbChkReContactLogEnable.SetCheck(m_nContactLogOption); //기본 값 할당.
	m_rbChkReContactLogDisable.SetCheck(!m_nContactLogOption);

	m_rbChkSocketCleanFunctionEnable.SetCheck(m_nSocketCleanFunctionOption); //기본 값 할당.
	m_rbChkSocketCleanFunctionDisable.SetCheck(!m_nSocketCleanFunctionOption);

	if (nReContactEnable == DEF_ON)
	{
		m_rbChkReContactEnable.SetCheck(TRUE);
		m_rbChkReContactDisable.SetCheck(FALSE);

		strReCnt.Format("%d", nReContactCnt);
		m_EditReContactCnt.SetWindowText(strReCnt);

		
		strOffset.Format("%.1f", dReContactOffset);
		m_EditReContactOffset.SetWindowText(strOffset);

		strContactDelayTime.Format("%.1f", dReContactDelayTime);
		m_EditReContactDelayTime.SetWindowText(strContactDelayTime);

		GetDlgItem(IDC_EDIT_RECONTACT_CNT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECONTACT_OFFSET)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TEXT224)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TEXT225)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TEXT226)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_RECONTACT_DELAY_TIME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_RECONTACT_DELAY_TIME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TEXT_RECONTACT_DELAY_TIME_SEC)->ShowWindow(SW_SHOW);
		
		
	}
	else {
		m_rbChkReContactEnable.SetCheck(FALSE);
		m_rbChkReContactDisable.SetCheck(TRUE);

		GetDlgItem(IDC_EDIT_RECONTACT_CNT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECONTACT_OFFSET)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TEXT224)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TEXT225)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TEXT226)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_RECONTACT_DELAY_TIME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_RECONTACT_DELAY_TIME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TEXT_RECONTACT_DELAY_TIME_SEC)->ShowWindow(SW_HIDE);
		
	}

// 	m_rbChkContactOffsetUse.SetCheck(m_nContactCleanPosOption);
// 	m_rbChkCleanOffsetUse.SetCheck(!m_nContactCleanPosOption);
}


void CSPD_TM_CONTACTOR::OnBnClickedPressRadioUse()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nDeviceCheckStatus = DEF_ON;
	SetVisibleSocketOption(TRUE);

	m_rbAllSocketOpt.SetCheck(TRUE); //기본 값 할당.
	m_rbOnlySocketOpt.SetCheck(FALSE); 
	m_nDeviceCheckOption = CHECK_ALL_SOCKET;
}


void CSPD_TM_CONTACTOR::OnBnClickedPressRadioNotUse()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nDeviceCheckStatus = DEF_OFF;
	SetVisibleSocketOption(FALSE);
}


void CSPD_TM_CONTACTOR::OnBnClickedPressRadioAllSocket()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nDeviceCheckOption = CHECK_ALL_SOCKET;
}


void CSPD_TM_CONTACTOR::OnBnClickedPressRadioOnlySocketOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nDeviceCheckOption = CHECK_ONLY_SOCKET_OPEN;
}


void CSPD_TM_CONTACTOR::OnBnClickedRadioVibratorUse()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nViratorOption = CHECK_VIBRATOR_USE;
	GetDlgItem(IDC_STATIC_VIB_CNT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_VIB_RE_CNT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_VIB_DELAY_TIME)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_VIB_DELAY_TIME)->ShowWindow(SW_SHOW);
}


void CSPD_TM_CONTACTOR::OnBnClickedRadioVibratorNotUse()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nViratorOption = CHECK_VIBRATOR_NOT_USE;
	GetDlgItem(IDC_STATIC_VIB_CNT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_VIB_RE_CNT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_VIB_DELAY_TIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_VIB_DELAY_TIME)->ShowWindow(SW_HIDE);

	int nUse = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_VIBRATOR_OPTION);
	int nReCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_VIBRATOR_RETRY_CNT);
	double bDeleyTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_PRESS_VIBRATOR_DELAY_TIME);
}

void CSPD_TM_CONTACTOR::ContactPosEnable(BOOL bEnable)
{
	GetDlgItem(IDC_STATIC_TEXT208)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT209)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT210)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT211)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT212)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT213)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT214)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT215)->EnableWindow(bEnable);
//	GetDlgItem(IDC_STATIC_TEXT216)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_STATIC_TEXT217)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_STATIC_TEXT218)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_STATIC_TEXT219)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_STATIC_TEXT220)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_STATIC_TEXT221)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_STATIC_TEXT222)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_STATIC_TEXT223)->EnableWindow(bEnable);

	GetDlgItem(IDC_STATIC_TEXT178)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT179)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT180)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT181)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT182)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT183)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT184)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT185)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_STATIC_TEXT186)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_STATIC_TEXT187)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_STATIC_TEXT188)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_STATIC_TEXT189)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_STATIC_TEXT190)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_STATIC_TEXT191)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_STATIC_TEXT192)->EnableWindow(bEnable);
// 	GetDlgItem(IDC_STATIC_TEXT193)->EnableWindow(bEnable);

	GetDlgItem(IDC_STATIC_GROUP_CONTACT)->EnableWindow(bEnable);


	for (size_t i = 0; i < eMaxPressUnitCount; i++)
	{
		GetDlgItem(IDC_EDIT_CONTACT_POS_1 + i)->EnableWindow(bEnable);
	}
}


void CSPD_TM_CONTACTOR::OnBnClickedRadioBlowPosUse()
{
	m_nBlowPosOption = CHECK_CONTACT_POS_USE;
		
	ContactPosEnable(m_nBlowPosOption);
	
}


void CSPD_TM_CONTACTOR::OnBnClickedRadioBlowPosNotUse()
{
	m_nBlowPosOption = CHECK_CONTACT_POS_NOT_USE;

	ContactPosEnable(m_nBlowPosOption);
}


void CSPD_TM_CONTACTOR::OnBnClickedRadioContactPosOffsetUse()
{
	GetDlgItem(IDC_STATIC_GROUP_CONTACT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_GROUP_CLEAN)->ShowWindow(SW_HIDE);

	m_dPressUnit_Blow_Pos1 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS1_BLOW_POS);
	m_dPressUnit_Blow_Pos2 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS2_BLOW_POS);
	m_dPressUnit_Blow_Pos3 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS3_BLOW_POS);
	m_dPressUnit_Blow_Pos4 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS4_BLOW_POS);
	m_dPressUnit_Blow_Pos5 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS5_BLOW_POS);
	m_dPressUnit_Blow_Pos6 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS6_BLOW_POS);
	m_dPressUnit_Blow_Pos7 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS7_BLOW_POS);
	m_dPressUnit_Blow_Pos8 = g_DMCont.m_dmHandlerOpt.GD(DDM11_TM_PRESS8_BLOW_POS);
	UpdateData(FALSE);
}

void CSPD_TM_CONTACTOR::OnEnSetfocusEditVibratorCnt()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nVibratorConut, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Vibrator Count", "50", "1");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_VIB_RE_CNT)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}

void CSPD_TM_CONTACTOR::OnEnSetfocusEditVibratorDelayTime()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_dVibratorDelayTime, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Vibrator Delay Time", "0.3", "0.05");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_VIB_DELAY_TIME)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnBnClickedRadioCleanPosOffsetUse()
{

}



void CSPD_TM_CONTACTOR::OnBnClickedRadioAlwayVibratorUse()
{
	m_nAlwayViratorOption = CHECK_VIBRATOR_USE;
	GetDlgItem(IDC_STATIC_ALWAY_VIB_CNT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_ALWAY_VIB_RE_CNT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_ALWAY_VIB_DELAY_TIME)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_ALWAY_VIB_DELAY_TIME)->ShowWindow(SW_SHOW);
}


void CSPD_TM_CONTACTOR::OnBnClickedRadioAlwayVibratorNotUse()
{
	m_nAlwayViratorOption = CHECK_VIBRATOR_NOT_USE;
	GetDlgItem(IDC_STATIC_ALWAY_VIB_CNT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_ALWAY_VIB_RE_CNT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_ALWAY_VIB_DELAY_TIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_ALWAY_VIB_DELAY_TIME)->ShowWindow(SW_HIDE);

	int nUse = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_ALWAY_VIBRATOR_OPTION);
	int nReCnt = g_DMCont.m_dmHandlerOpt.GN(NDM11_PRESS_ALWAY_VIBRATOR_RETRY_CNT);
	double bDeleyTime = g_DMCont.m_dmHandlerOpt.GD(DDM11_PRESS_ALWAY_VIBRATOR_DELAY_TIME);
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditAlwayVibReCnt()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nAlwayVibratorConut, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Alway Vibrator Count", "50", "1");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ALWAY_VIB_RE_CNT)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditAlwayVibDelayTime()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_dAlwayVibratorDelayTime, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Alway Vibrator Delay Time", "0.3", "0.05");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_ALWAY_VIB_DELAY_TIME)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnBnClickedRadioRecontactUse()
{
	char szCompany[16] = { 0, };

	g_DMCont.m_dmHandlerOpt.GS(SDM11_Company, szCompany, sizeof(szCompany));

	m_nReContactOption = CHECK_VIBRATOR_USE;
	GetDlgItem(IDC_EDIT_RECONTACT_CNT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_TEXT225)->ShowWindow(SW_SHOW);
	
}


void CSPD_TM_CONTACTOR::OnBnClickedRadioRecontactNotUse()
{
	m_nReContactOption = CHECK_VIBRATOR_NOT_USE;
	GetDlgItem(IDC_EDIT_RECONTACT_CNT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT224)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT225)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT226)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_RECONTACT_OFFSET)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_RECONTACT_DELAY_TIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_RECONTACT_DELAY_TIME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_TEXT_RECONTACT_DELAY_TIME_SEC)->ShowWindow(SW_HIDE);
}

void CSPD_TM_CONTACTOR::OnEnSetfocusEditRecontactCnt()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nReContactConut, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "ReContact Count", "20", "1");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_RECONTACT_CNT)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditRecontactOffset()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_dReContactOffset, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "ReContact Offset", "50.0", "0.0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_RECONTACT_OFFSET)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditRecontactDelayTime()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_dReContactDelayTime, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "ReContact Delay Time", "20.0", "0.0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_RECONTACT_DELAY_TIME)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditContactPosOffset()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_dContactPosOffset, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Contact Pos Offset", "100.0", "0.0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_CONTACT_POS_OFFSET)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnBnClickedRadioVibrationUse()
{
	m_nVibrationOption = CHECK_VIBRATOR_USE;
}


void CSPD_TM_CONTACTOR::OnBnClickedRadioVibrationNotUse()
{
	m_nVibrationOption = CHECK_VIBRATOR_NOT_USE;
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditContact2stSpeed()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_nContact2stSpeed, szVal, sizeof(szVal));

	char szMinSpeed[5] = { 0, };
	char szMaxSpeed[5] = { 0, };

	sprintf(szMinSpeed,"%d" ,PRESS_MIN_CONTACT_SPEED);
	sprintf(szMaxSpeed, "%d" , PRESS_MAX_CONTACT_SPEED);
	
	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Contact 2st Speed", szMaxSpeed, szMinSpeed);

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_CONTACT_2ST_SPEED)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditCleaning2ndZOffset()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_dCleaning2ndZOffset, szVal, sizeof(szVal));


	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Cleaning 2nd Z Offset", "20", "5");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_CLEANING_2ND_Z_OFFSET)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnBnClickedRadioContactLogUse()
{
	m_nContactLogOption = CHECK_CONTACT_POS_USE;
}


void CSPD_TM_CONTACTOR::OnBnClickedRadioContactLogNotUse()
{
	m_nContactLogOption = CHECK_CONTACT_POS_NOT_USE;
}


void CSPD_TM_CONTACTOR::OnBnClickedStaticSocketCleanFunctionUse()
{
	m_nSocketCleanFunctionOption = DEF_ON;
	SocketCleanFunctionEnable(m_nSocketCleanFunctionOption);
}


void CSPD_TM_CONTACTOR::OnBnClickedStaticSocketCleanFunctionNotUse()
{
	m_nSocketCleanFunctionOption = DEF_OFF;
	SocketCleanFunctionEnable(m_nSocketCleanFunctionOption);
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditSocketCleanBlowPos()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_dSocketCleanBlowPos, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Socket Clean Blow Pos", "20", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SOCKET_CLEAN_BLOW_POS)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditSocketCleanBlowTime()
{
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(m_dSocketCleanBlowTime, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Socket Clean Blow Time", "10", "0");

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_SOCKET_CLEAN_BLOW_TIME)->SetWindowText(szRetVal);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}

void CSPD_TM_CONTACTOR::SocketCleanFunctionEnable(BOOL bEnable)
{
	GetDlgItem(IDC_EDIT_SOCKET_CLEAN_BLOW_POS)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_SOCKET_CLEAN_BLOW_TIME)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_SOCKET_CLEAN_BLOW_POS)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_SOCKET_CLEAN_BLOW_TIME)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT230)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT231)->EnableWindow(bEnable);
}




void CSPD_TM_CONTACTOR::OnEnSetfocusEditContactCleanOffset1()
{
	NumberBox(m_dPressUnit_Clean_Offset1, IDC_EDIT_CONTACT_CLEAN_OFFSET_1, "Press Unit #1 Clean Offset", "2", "-2");
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditContactCleanOffset2()
{
	NumberBox(m_dPressUnit_Clean_Offset2, IDC_EDIT_CONTACT_CLEAN_OFFSET_2, "Press Unit #2 Clean Offset", "2", "-2");
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditContactCleanOffset3()
{
	NumberBox(m_dPressUnit_Clean_Offset3, IDC_EDIT_CONTACT_CLEAN_OFFSET_3, "Press Unit #3 Clean Offset", "2", "-2");
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditContactCleanOffset4()
{
	NumberBox(m_dPressUnit_Clean_Offset4, IDC_EDIT_CONTACT_CLEAN_OFFSET_4, "Press Unit #4 Clean Offset", "2", "-2");
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditContactCleanOffset5()
{
	NumberBox(m_dPressUnit_Clean_Offset5, IDC_EDIT_CONTACT_CLEAN_OFFSET_5, "Press Unit #5 Clean Offset", "2", "-2");
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditContactCleanOffset6()
{
	NumberBox(m_dPressUnit_Clean_Offset6, IDC_EDIT_CONTACT_CLEAN_OFFSET_6, "Press Unit #6 Clean Offset", "2", "-2");
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditContactCleanOffset7()
{
	NumberBox(m_dPressUnit_Clean_Offset7, IDC_EDIT_CONTACT_CLEAN_OFFSET_7, "Press Unit #7 Clean Offset", "2", "-2");
}


void CSPD_TM_CONTACTOR::OnEnSetfocusEditContactCleanOffset8()
{
	NumberBox(m_dPressUnit_Clean_Offset8, IDC_EDIT_CONTACT_CLEAN_OFFSET_8, "Press Unit #8 Clean Offset", "2", "-2");
}


void CSPD_TM_CONTACTOR::OnBnClickedRadioCleanOffUse()
{
	m_nCleanOffsetOption = CHECK_CLEAN_OFFSET_USE;

	CleanOffsetEnable(m_nCleanOffsetOption);
}

void CSPD_TM_CONTACTOR::OnBnClickedRadioCleanOffsetNotUse()
{
	m_nCleanOffsetOption = CHECK_CLEAN_OFFSET_NOT_USE;

	CleanOffsetEnable(m_nCleanOffsetOption);
}

void CSPD_TM_CONTACTOR::CleanOffsetEnable(BOOL bEnable)
{
	GetDlgItem(IDC_STATIC_TEXT267)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT273)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT274)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT275)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT276)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT277)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT278)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT279)->EnableWindow(bEnable);

	GetDlgItem(IDC_STATIC_TEXT268)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT269)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT270)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT200)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT201)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT202)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT271)->EnableWindow(bEnable);
	GetDlgItem(IDC_STATIC_TEXT272)->EnableWindow(bEnable);

	GetDlgItem(IDC_STATIC_GROUP_CLEAN_OFFSET)->EnableWindow(bEnable);

	for (size_t i = 0; i < eMaxPressUnitCount; i++)
	{
		GetDlgItem(IDC_EDIT_CONTACT_CLEAN_OFFSET_1 + i)->EnableWindow(bEnable);
	}
}



