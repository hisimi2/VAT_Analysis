// ManuMotorZAxisDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ManuMotorZAxisDlg.h"
#include "afxdialogex.h"


// CManuMotorZAxisDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManuMotorZAxisDlg, CDialog)

	CManuMotorZAxisDlg::CManuMotorZAxisDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MANU_MOTOR_ZAXIS, pParent)
{
	m_nAxisNo = 0;
	m_nJogSpeed = 100;
	m_nBoardDir = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
	memset(m_ZrStat,0,sizeof(m_ZrStat));
	memset(m_MotorStat,0,sizeof(m_MotorStat));
	memset(m_ServoOnOff,0,sizeof(m_ServoOnOff));
}

CManuMotorZAxisDlg::~CManuMotorZAxisDlg()
{

}

void CManuMotorZAxisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_AXIS_CHOOSE, m_Combo_RobotChoose);
	DDX_Control(pDX, IDC_COMBO_M_SPEED_Z, m_motor_speed);
	DDX_Control(pDX, IDC_CLRCHK_ZR_STAT01, m_ZrStat01);
	DDX_Control(pDX, IDC_CLRCHK_ZR_STAT02, m_ZrStat02);
	DDX_Control(pDX, IDC_CLRCHK_ZR_STAT03, m_ZrStat03);
	DDX_Control(pDX, IDC_CLRCHK_ZR_STAT04, m_ZrStat04);
	DDX_Control(pDX, IDC_CLRCHK_ZR_STAT05, m_ZrStat05);
	DDX_Control(pDX, IDC_CLRCHK_ZR_STAT06, m_ZrStat06);
	DDX_Control(pDX, IDC_CLRCHK_ZR_STAT07, m_ZrStat07);
	DDX_Control(pDX, IDC_CLRCHK_ZR_STAT08, m_ZrStat08);
	DDX_Control(pDX, IDC_CLRCHK_AMP_STAT01, m_MotorStat01);
	DDX_Control(pDX, IDC_CLRCHK_AMP_STAT02, m_MotorStat02);
	DDX_Control(pDX, IDC_CLRCHK_AMP_STAT03, m_MotorStat03);
	DDX_Control(pDX, IDC_CLRCHK_AMP_STAT04, m_MotorStat04);
	DDX_Control(pDX, IDC_CLRCHK_AMP_STAT05, m_MotorStat05);
	DDX_Control(pDX, IDC_CLRCHK_AMP_STAT06, m_MotorStat06);
	DDX_Control(pDX, IDC_CLRCHK_AMP_STAT07, m_MotorStat07);
	DDX_Control(pDX, IDC_CLRCHK_AMP_STAT08, m_MotorStat08);
	DDX_Control(pDX, IDC_CLRCHK_SERVO_STAT01, m_ServoOnOff01);
	DDX_Control(pDX, IDC_CLRCHK_SERVO_STAT02, m_ServoOnOff02);
	DDX_Control(pDX, IDC_CLRCHK_SERVO_STAT03, m_ServoOnOff03);
	DDX_Control(pDX, IDC_CLRCHK_SERVO_STAT04, m_ServoOnOff04);
	DDX_Control(pDX, IDC_CLRCHK_SERVO_STAT05, m_ServoOnOff05);
	DDX_Control(pDX, IDC_CLRCHK_SERVO_STAT06, m_ServoOnOff06);
	DDX_Control(pDX, IDC_CLRCHK_SERVO_STAT07, m_ServoOnOff07);
	DDX_Control(pDX, IDC_CLRCHK_SERVO_STAT08, m_ServoOnOff08);
	DDX_Control(pDX, IDC_COLORBTCTRL_ZAXIS_JOG_PLUS, m_ctrlPlus);
	DDX_Control(pDX, IDC_COLORBTCTRL_ZAXIS_JOG_MINUS2, m_ctrlMinus);
}


BEGIN_MESSAGE_MAP(CManuMotorZAxisDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL2, &CManuMotorZAxisDlg::OnBnClickedCancel2)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_AXIS_CHOOSE, &CManuMotorZAxisDlg::OnCbnSelchangeComboAxisChoose)
	ON_CBN_SELCHANGE(IDC_COMBO_M_SPEED_Z, &CManuMotorZAxisDlg::OnCbnSelchangeComboMSpeedZ)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CManuMotorZAxisDlg::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_ZR, &CManuMotorZAxisDlg::OnBnClickedButtonZr)
	ON_BN_CLICKED(IDC_BUTTON_SERVO_ONOFF, &CManuMotorZAxisDlg::OnBnClickedButtonServoOnoff)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CManuMotorZAxisDlg::OnBnClickedButtonStop)
	ON_WM_DESTROY()
END_MESSAGE_MAP()
// CManuMotorZAxisDlg 메시지 처리기입니다.
void CManuMotorZAxisDlg::OnBnClickedCancel2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnOK();
}


void CManuMotorZAxisDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString AxisPos1 = _T("");
	CString AxisPos2 = _T("");
	CString AxisPos3 = _T("");
	CString AxisPos4 = _T("");
	CString AxisPos5 = _T("");
	CString AxisPos6 = _T("");
	CString AxisPos7 = _T("");
	CString AxisPos8 = _T("");
	int i = 0;
	int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
// 	switch(m_nAxisNo)
// 	{
	if (eAXIS_TRAY_PP_1_Z == m_nAxisNo)
	{
		AxisPos1.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TRAY, eBMAxis_LOAD_TRAY_Picker01)) / 1000);
		AxisPos2.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TRAY, eBMAxis_LOAD_TRAY_Picker02)) / 1000);
		AxisPos3.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TRAY, eBMAxis_LOAD_TRAY_Picker03)) / 1000);
		AxisPos4.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TRAY, eBMAxis_LOAD_TRAY_Picker04)) / 1000);
		AxisPos5.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TRAY, eBMAxis_LOAD_TRAY_Picker05)) / 1000);
		AxisPos6.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TRAY, eBMAxis_LOAD_TRAY_Picker06)) / 1000);
		AxisPos7.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TRAY, eBMAxis_LOAD_TRAY_Picker07)) / 1000);
		AxisPos8.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TRAY, eBMAxis_LOAD_TRAY_Picker08)) / 1000);
		for (i = 0; i < MAX_PICKER_Z_AXIS_CNT; i++)
		{
			m_ZrStat[i] = g_pBbxmnpClient->GetHomeSensorStatus(CBbxmnpClient::PICK_MOTOR_LOAD_TRAY, i);
		}

		for (i = 0; i < MAX_PICKER_Z_AXIS_CNT; i++)
		{
			m_MotorStat[i] = g_pBbxmnpClient->GetMotorStatus(CBbxmnpClient::PICK_MOTOR_LOAD_TRAY, _eBbxmnpAxis(i), CBbxmnpClient::eMotorSts_ALM);
		}
		for (i = 0; i < MAX_PICKER_Z_AXIS_CNT; i++)
		{
			m_ServoOnOff[i] = g_pBbxmnpClient->GetMotorStatus(CBbxmnpClient::PICK_MOTOR_LOAD_TRAY, _eBbxmnpAxis(i), CBbxmnpClient::eMotorSts_ServoOn);
		}
	}
	else if (eAXIS_TRAY_PP_2_Z == m_nAxisNo)
	{
		AxisPos1.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TRAY, eBMAxis_UNLOAD_TRAY_Picker01)) / 1000);
		AxisPos2.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TRAY, eBMAxis_UNLOAD_TRAY_Picker02)) / 1000);
		AxisPos3.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TRAY, eBMAxis_UNLOAD_TRAY_Picker03)) / 1000);
		AxisPos4.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TRAY, eBMAxis_UNLOAD_TRAY_Picker04)) / 1000);
		AxisPos5.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TRAY, eBMAxis_UNLOAD_TRAY_Picker05)) / 1000);
		AxisPos6.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TRAY, eBMAxis_UNLOAD_TRAY_Picker06)) / 1000);
		AxisPos7.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TRAY, eBMAxis_UNLOAD_TRAY_Picker07)) / 1000);
		AxisPos8.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TRAY, eBMAxis_UNLOAD_TRAY_Picker08)) / 1000);

		for (i = 0; i < MAX_PICKER_Z_AXIS_CNT; i++)
		{
			m_ZrStat[i] = g_pBbxmnpClient->GetHomeSensorStatus(CBbxmnpClient::PICK_MOTOR_UNLOAD_TRAY, eBMAxis_UNLOAD_TRAY_Picker01 + i);
		}

		for (i = 0; i < MAX_PICKER_Z_AXIS_CNT; i++)
		{
			m_MotorStat[i] = g_pBbxmnpClient->GetMotorStatus(CBbxmnpClient::PICK_MOTOR_UNLOAD_TRAY, _eBbxmnpAxis(eBMAxis_UNLOAD_TRAY_Picker01 + i), CBbxmnpClient::eMotorSts_ALM);
		}

		for (i = 0; i < MAX_PICKER_Z_AXIS_CNT; i++)
		{
			m_ServoOnOff[i] = g_pBbxmnpClient->GetMotorStatus(CBbxmnpClient::PICK_MOTOR_UNLOAD_TRAY, _eBbxmnpAxis(eBMAxis_UNLOAD_TRAY_Picker01 + i), CBbxmnpClient::eMotorSts_ServoOn);
		}
	}
	else if (eAXIS_TEST_PP_1_Z == m_nAxisNo)
	{

		// 		if (nBoardType == eBoardDir_Forward)
		// 		{
		// 			AxisPos1.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker01)) / 1000);
		// 			AxisPos2.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker02)) / 1000);
		// 			AxisPos3.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker03)) / 1000);
		// 			AxisPos4.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker04)) / 1000);
		// 			AxisPos5.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker05)) / 1000);
		// 			AxisPos6.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker06)) / 1000);
		// 			AxisPos7.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker07)) / 1000);
		// 			AxisPos8.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker08)) / 1000);
		// 		}
		// 		else
		// 		{
		// 			AxisPos1.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker08)) / 1000);
		// 			AxisPos2.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker07)) / 1000);
		// 			AxisPos3.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker06)) / 1000);
		// 			AxisPos4.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker05)) / 1000);
		// 			AxisPos5.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker04)) / 1000);
		// 			AxisPos6.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker03)) / 1000);
		// 			AxisPos7.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker02)) / 1000);
		// 			AxisPos8.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker01)) / 1000);
		// 		}

		AxisPos1.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker08)) / 1000);
		AxisPos2.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker07)) / 1000);
		AxisPos3.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker06)) / 1000);
		AxisPos4.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker05)) / 1000);
		AxisPos5.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker04)) / 1000);
		AxisPos6.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker03)) / 1000);
		AxisPos7.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker02)) / 1000);
		AxisPos8.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, eBMAxis_LOAD_TESTPP_1_Picker01)) / 1000);

		for (i = 0; i < MAX_PICKER_Z_AXIS_CNT; i++)
		{
			//int nIdx = (nBoardType == eBoardDir_Forward) ? i : abs((MAX_PICKER_Z_AXIS_CNT - 1) - i);

			m_ZrStat[i] = g_pBbxmnpClient->GetHomeSensorStatus(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, (int)CHANGE((int)eBMAxis_LOAD_TESTPP_1_Picker01 + i));
		}

		for (i = 0; i < MAX_PICKER_Z_AXIS_CNT; i++)
		{
			m_MotorStat[i] = g_pBbxmnpClient->GetMotorStatus(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, CHANGE((int)eBMAxis_LOAD_TESTPP_1_Picker01 + i), CBbxmnpClient::eMotorSts_ALM);
		}

		for (i = 0; i < MAX_PICKER_Z_AXIS_CNT; i++)
		{
			m_ServoOnOff[i] = g_pBbxmnpClient->GetMotorStatus(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, CHANGE((int)eBMAxis_LOAD_TESTPP_1_Picker01 + i), CBbxmnpClient::eMotorSts_ServoOn);
		}

		// 		for (i = 0; i < MAX_PICKER_Z_AXIS_CNT; i++)
		// 		{
		// 			//int nIdx = (nBoardType == eBoardDir_Forward) ? i : abs((MAX_PICKER_Z_AXIS_CNT - 1) - i);
		// 
		// 			m_ZrStat[i] = g_pBbxmnpClient->GetHomeSensorStatus(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, CHANGE((int)eBMAxis_LOAD_TESTPP_1_Picker01 + i));
		// 		}
		// 
		// 		for (i = 0; i < MAX_PICKER_Z_AXIS_CNT; i++)
		// 		{
		// 			m_MotorStat[i] = g_pBbxmnpClient->GetMotorStatus(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, _eBbxmnpAxis(eBMAxis_LOAD_TESTPP_1_Picker01 + i), CBbxmnpClient::eMotorSts_ALM);
		// 		}
		// 
		// 		for (i = 0; i < MAX_PICKER_Z_AXIS_CNT; i++)
		// 		{
		// 			m_ServoOnOff[i] = g_pBbxmnpClient->GetMotorStatus(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1, _eBbxmnpAxis(eBMAxis_LOAD_TESTPP_1_Picker01 + i), CBbxmnpClient::eMotorSts_ServoOn);
		// 		}
	}
	else if (eAXIS_TEST_PP_1_Z2 == m_nAxisNo)
	{
		// 		if (nBoardType == eBoardDir_Forward)
		// 		{
		// 			AxisPos1.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker01)) / 1000);
		// 			AxisPos2.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker02)) / 1000);
		// 			AxisPos3.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker03)) / 1000);
		// 			AxisPos4.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker04)) / 1000);
		// 			AxisPos5.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker05)) / 1000);
		// 			AxisPos6.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker06)) / 1000);
		// 			AxisPos7.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker07)) / 1000);
		// 			AxisPos8.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker08)) / 1000);
		// 		}
		// 		else
		// 		{
		// 			AxisPos1.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker08)) / 1000);
		// 			AxisPos2.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker07)) / 1000);
		// 			AxisPos3.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker06)) / 1000);
		// 			AxisPos4.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker05)) / 1000);
		// 			AxisPos5.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker04)) / 1000);
		// 			AxisPos6.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker03)) / 1000);
		// 			AxisPos7.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker02)) / 1000);
		// 			AxisPos8.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker01)) / 1000);
		// 		}

		AxisPos1.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker08)) / 1000);
		AxisPos2.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker07)) / 1000);
		AxisPos3.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker06)) / 1000);
		AxisPos4.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker05)) / 1000);
		AxisPos5.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker04)) / 1000);
		AxisPos6.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker03)) / 1000);
		AxisPos7.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker02)) / 1000);
		AxisPos8.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, eBMAxis_UNLOAD_TESTPP_1_Picker01)) / 1000);


		for (i = 0; i < MAX_PICKER_Z_AXIS_CNT; i++)
		{
			m_ZrStat[i] = g_pBbxmnpClient->GetHomeSensorStatus(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, (int)CHANGE((int)eBMAxis_UNLOAD_TESTPP_1_Picker01 + i));
		}

		for (i = 0; i < MAX_PICKER_Z_AXIS_CNT; i++)
		{
			m_MotorStat[i] = g_pBbxmnpClient->GetMotorStatus(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, CHANGE((int)eBMAxis_UNLOAD_TESTPP_1_Picker01 + i), CBbxmnpClient::eMotorSts_ALM);
		}

		for (i = 0; i < MAX_PICKER_Z_AXIS_CNT; i++)
		{
			m_ServoOnOff[i] = g_pBbxmnpClient->GetMotorStatus(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1, CHANGE((int)eBMAxis_UNLOAD_TESTPP_1_Picker01 + i), CBbxmnpClient::eMotorSts_ServoOn);
		}
	}
	else if (eAXIS_TEST_PP_2_Z == m_nAxisNo)
	{
		// 		if (nBoardType == eBoardDir_Forward)
		// 		{
		// 			AxisPos1.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker01)) / 1000);
		// 			AxisPos2.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker02)) / 1000);
		// 			AxisPos3.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker03)) / 1000);
		// 			AxisPos4.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker04)) / 1000);
		// 			AxisPos5.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker05)) / 1000);
		// 			AxisPos6.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker06)) / 1000);
		// 			AxisPos7.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker07)) / 1000);
		// 			AxisPos8.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker08)) / 1000);
		// 		}
		// 		else
		// 		{
		// 			AxisPos1.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker08)) / 1000);
		// 			AxisPos2.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker07)) / 1000);
		// 			AxisPos3.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker06)) / 1000);
		// 			AxisPos4.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker05)) / 1000);
		// 			AxisPos5.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker04)) / 1000);
		// 			AxisPos6.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker03)) / 1000);
		// 			AxisPos7.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker02)) / 1000);
		// 			AxisPos8.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker01)) / 1000);
		// 		}

		AxisPos1.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker08)) / 1000);
		AxisPos2.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker07)) / 1000);
		AxisPos3.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker06)) / 1000);
		AxisPos4.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker05)) / 1000);
		AxisPos5.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker04)) / 1000);
		AxisPos6.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker03)) / 1000);
		AxisPos7.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker02)) / 1000);
		AxisPos8.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, eBMAxis_LOAD_TESTPP_2_Picker01)) / 1000);


		for (i = 0; i < MAX_PICKER_Z_AXIS_CNT; i++)
		{
			m_ZrStat[i] = g_pBbxmnpClient->GetHomeSensorStatus(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, (int)CHANGE((int)eBMAxis_LOAD_TESTPP_2_Picker01 + i));
		}

		for (i = 0; i < MAX_PICKER_Z_AXIS_CNT; i++)
		{
			m_MotorStat[i] = g_pBbxmnpClient->GetMotorStatus(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, CHANGE((int)eBMAxis_LOAD_TESTPP_2_Picker01 + i), CBbxmnpClient::eMotorSts_ALM);
		}

		for (i = 0; i < MAX_PICKER_Z_AXIS_CNT; i++)
		{
			m_ServoOnOff[i] = g_pBbxmnpClient->GetMotorStatus(CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2, CHANGE((int)eBMAxis_LOAD_TESTPP_2_Picker01 + i), CBbxmnpClient::eMotorSts_ServoOn);
		}
	}
	else if (eAXIS_TEST_PP_2_Z2 == m_nAxisNo)
	{
	// 		if (nBoardType == eBoardDir_Forward)
	// 		{
	// 			AxisPos1.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker01)) / 1000);
	// 			AxisPos2.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker02)) / 1000);
	// 			AxisPos3.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker03)) / 1000);
	// 			AxisPos4.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker04)) / 1000);
	// 			AxisPos5.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker05)) / 1000);
	// 			AxisPos6.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker06)) / 1000);
	// 			AxisPos7.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker07)) / 1000);
	// 			AxisPos8.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker08)) / 1000);
	// 
	// 		}
	// 		else
	// 		{
	// 			AxisPos1.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker08)) / 1000);
	// 			AxisPos2.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker07)) / 1000);
	// 			AxisPos3.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker06)) / 1000);
	// 			AxisPos4.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker05)) / 1000);
	// 			AxisPos5.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker04)) / 1000);
	// 			AxisPos6.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker03)) / 1000);
	// 			AxisPos7.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker02)) / 1000);
	// 			AxisPos8.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker01)) / 1000);
	// 		}

		AxisPos1.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker08)) / 1000);
		AxisPos2.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker07)) / 1000);
		AxisPos3.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker06)) / 1000);
		AxisPos4.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker05)) / 1000);
		AxisPos5.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker04)) / 1000);
		AxisPos6.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker03)) / 1000);
		AxisPos7.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker02)) / 1000);
		AxisPos8.Format("%7.3f", (g_pBbxmnpClient->GetActualPos(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, eBMAxis_UNLOAD_TESTPP_2_Picker01)) / 1000);


		for (i = 0; i < MAX_PICKER_Z_AXIS_CNT; i++)
		{
			m_ZrStat[i] = g_pBbxmnpClient->GetHomeSensorStatus(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, (int)CHANGE((int)eBMAxis_UNLOAD_TESTPP_2_Picker01 + i));
		}

		for (i = 0; i < MAX_PICKER_Z_AXIS_CNT; i++)
		{
			m_MotorStat[i] = g_pBbxmnpClient->GetMotorStatus(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, CHANGE((int)eBMAxis_UNLOAD_TESTPP_2_Picker01 + i), CBbxmnpClient::eMotorSts_ALM);
		}

		for (i = 0; i < MAX_PICKER_Z_AXIS_CNT; i++)
		{
			m_ServoOnOff[i] = g_pBbxmnpClient->GetMotorStatus(CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2, CHANGE((int)eBMAxis_UNLOAD_TESTPP_2_Picker01 + i), CBbxmnpClient::eMotorSts_ServoOn);
		}
	}

	ShowZRStatus();
	ShowAmpFaultStatus();
	ShowServoStatus();

	GetDlgItem(IDC_STATIC_LOCATION01)->SetWindowText(AxisPos1);
	GetDlgItem(IDC_STATIC_LOCATION02)->SetWindowText(AxisPos2);
	GetDlgItem(IDC_STATIC_LOCATION03)->SetWindowText(AxisPos3);
	GetDlgItem(IDC_STATIC_LOCATION04)->SetWindowText(AxisPos4);
	GetDlgItem(IDC_STATIC_LOCATION05)->SetWindowText(AxisPos5);
	GetDlgItem(IDC_STATIC_LOCATION06)->SetWindowText(AxisPos6);
	GetDlgItem(IDC_STATIC_LOCATION07)->SetWindowText(AxisPos7);
	GetDlgItem(IDC_STATIC_LOCATION08)->SetWindowText(AxisPos8);

	int nPickerNo = m_Combo_RobotChoose.GetCurSel();
	if (m_ServoOnOff[nPickerNo] == TRUE)
	{
		GetDlgItem(IDC_BUTTON_SERVO_ONOFF)->SetWindowText("Servo ON");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_SERVO_ONOFF)->SetWindowText("Servo OFF");
	}

	CDialog::OnTimer(nIDEvent);
}


BOOL CManuMotorZAxisDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_Combo_RobotChoose.InsertString(0,"Z-Axis 1");
	m_Combo_RobotChoose.InsertString(1,"Z-Axis 2");
	m_Combo_RobotChoose.InsertString(2,"Z-Axis 3");
	m_Combo_RobotChoose.InsertString(3,"Z-Axis 4");
	m_Combo_RobotChoose.InsertString(4,"Z-Axis 5");
	m_Combo_RobotChoose.InsertString(5,"Z-Axis 6");
	m_Combo_RobotChoose.InsertString(6,"Z-Axis 7");
	m_Combo_RobotChoose.InsertString(7,"Z-Axis 8");

	m_Combo_RobotChoose.SetCurSel(0);
	m_motor_speed.SetCurSel(0);

	m_ctrlPlus.SetCaption("UP");
	m_ctrlMinus.SetCaption("DOWN");

	m_vPkrIdx.clear();
	m_vPkrIdx.push_back(eBMAxis_LOAD_TRAY_Picker08);
	m_vPkrIdx.push_back(eBMAxis_LOAD_TRAY_Picker07);
	m_vPkrIdx.push_back(eBMAxis_LOAD_TRAY_Picker06);
	m_vPkrIdx.push_back(eBMAxis_LOAD_TRAY_Picker05);
	m_vPkrIdx.push_back(eBMAxis_LOAD_TRAY_Picker04);
	m_vPkrIdx.push_back(eBMAxis_LOAD_TRAY_Picker03);
	m_vPkrIdx.push_back(eBMAxis_LOAD_TRAY_Picker02);
	m_vPkrIdx.push_back(eBMAxis_LOAD_TRAY_Picker01);
	m_vPkrIdx.push_back(eBMAxis_LOAD_TESTPP_1_Picker08);
	m_vPkrIdx.push_back(eBMAxis_LOAD_TESTPP_1_Picker07);
	m_vPkrIdx.push_back(eBMAxis_LOAD_TESTPP_1_Picker06);
	m_vPkrIdx.push_back(eBMAxis_LOAD_TESTPP_1_Picker05);
	m_vPkrIdx.push_back(eBMAxis_LOAD_TESTPP_1_Picker04);
	m_vPkrIdx.push_back(eBMAxis_LOAD_TESTPP_1_Picker03);
	m_vPkrIdx.push_back(eBMAxis_LOAD_TESTPP_1_Picker02);
	m_vPkrIdx.push_back(eBMAxis_LOAD_TESTPP_1_Picker01);
	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TESTPP_1_Picker08);
	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TESTPP_1_Picker07);
	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TESTPP_1_Picker06);
	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TESTPP_1_Picker05);
	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TESTPP_1_Picker04);
	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TESTPP_1_Picker03);
	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TESTPP_1_Picker02);
	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TESTPP_1_Picker01);
	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TRAY_Picker08);
	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TRAY_Picker07);
	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TRAY_Picker06);
	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TRAY_Picker05);
	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TRAY_Picker04);
	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TRAY_Picker03);
	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TRAY_Picker02);
	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TRAY_Picker01);

	m_vPkrIdx.push_back(eBMAxis_LOAD_TESTPP_2_Picker08);
	m_vPkrIdx.push_back(eBMAxis_LOAD_TESTPP_2_Picker07);
	m_vPkrIdx.push_back(eBMAxis_LOAD_TESTPP_2_Picker06);
	m_vPkrIdx.push_back(eBMAxis_LOAD_TESTPP_2_Picker05);
	m_vPkrIdx.push_back(eBMAxis_LOAD_TESTPP_2_Picker04);
	m_vPkrIdx.push_back(eBMAxis_LOAD_TESTPP_2_Picker03);
	m_vPkrIdx.push_back(eBMAxis_LOAD_TESTPP_2_Picker02);
	m_vPkrIdx.push_back(eBMAxis_LOAD_TESTPP_2_Picker01);

	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TESTPP_2_Picker08);
	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TESTPP_2_Picker07);
	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TESTPP_2_Picker06);
	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TESTPP_2_Picker05);
	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TESTPP_2_Picker04);
	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TESTPP_2_Picker03);
	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TESTPP_2_Picker02);
	m_vPkrIdx.push_back(eBMAxis_UNLOAD_TESTPP_2_Picker01);


	SetTimer(0,100,0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CManuMotorZAxisDlg::OnCbnSelchangeComboAxisChoose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nAxisNum = m_Combo_RobotChoose.GetCurSel();
}


void CManuMotorZAxisDlg::OnCbnSelchangeComboMSpeedZ()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_motor_speed.GetCurSel() == 0)
	{
		m_nJogSpeed = 10;
	}
	else if(m_motor_speed.GetCurSel() == 1)
	{
		m_nJogSpeed = 30;
	}
	else
	{
		m_nJogSpeed = 50;
	}
}

void CManuMotorZAxisDlg::ShowZRStatus()
{
	//m_nBoardDir
	if (m_ZrStat[0] == TRUE)	m_ZrStat01.On();
	else						m_ZrStat01.Off();

	if (m_ZrStat[1] == TRUE)	m_ZrStat02.On();
	else						m_ZrStat02.Off();

	if (m_ZrStat[2] == TRUE)	m_ZrStat03.On();
	else						m_ZrStat03.Off();

	if (m_ZrStat[3] == TRUE)	m_ZrStat04.On();
	else						m_ZrStat04.Off();

	if (m_ZrStat[4] == TRUE)	m_ZrStat05.On();
	else						m_ZrStat05.Off();

	if (m_ZrStat[5] == TRUE)	m_ZrStat06.On();
	else						m_ZrStat06.Off();

	if (m_ZrStat[6] == TRUE)	m_ZrStat07.On();
	else						m_ZrStat07.Off();

	if (m_ZrStat[7] == TRUE)	m_ZrStat08.On();
	else						m_ZrStat08.Off();

}

void CManuMotorZAxisDlg::ShowAmpFaultStatus()
{
	if(m_MotorStat[0] == TRUE)	m_MotorStat01.On();
	else						m_MotorStat01.Off();

	if(m_MotorStat[1] == TRUE)	m_MotorStat02.On();
	else						m_MotorStat02.Off();

	if(m_MotorStat[2] == TRUE)	m_MotorStat03.On();
	else						m_MotorStat03.Off();

	if(m_MotorStat[3] == TRUE)	m_MotorStat03.On();
	else						m_MotorStat03.Off();

	if(m_MotorStat[4] == TRUE)	m_MotorStat04.On();
	else						m_MotorStat04.Off();

	if(m_MotorStat[5] == TRUE)	m_MotorStat05.On();
	else						m_MotorStat05.Off();

	if(m_MotorStat[6] == TRUE)	m_MotorStat06.On();
	else						m_MotorStat06.Off();

	if(m_MotorStat[7] == TRUE)	m_MotorStat07.On();
	else						m_MotorStat07.Off();
}

void CManuMotorZAxisDlg::ShowServoStatus()
{
	if(m_ServoOnOff[0] == TRUE)	m_ServoOnOff01.On();
	else						m_ServoOnOff01.Off();

	if(m_ServoOnOff[1] == TRUE) m_ServoOnOff02.On();
	else						m_ServoOnOff02.Off();

	if(m_ServoOnOff[2] == TRUE) m_ServoOnOff03.On();
	else						m_ServoOnOff03.Off();

	if(m_ServoOnOff[3] == TRUE) m_ServoOnOff04.On();
	else						m_ServoOnOff04.Off();

	if(m_ServoOnOff[4] == TRUE) m_ServoOnOff05.On();
	else						m_ServoOnOff05.Off();

	if(m_ServoOnOff[5] == TRUE) m_ServoOnOff06.On();
	else						m_ServoOnOff06.Off();

	if(m_ServoOnOff[6] == TRUE) m_ServoOnOff07.On();
	else						m_ServoOnOff07.Off();

	if(m_ServoOnOff[7] == TRUE) m_ServoOnOff08.On();
	else						m_ServoOnOff08.Off();
}
BEGIN_EVENTSINK_MAP(CManuMotorZAxisDlg, CDialog)
	ON_EVENT(CManuMotorZAxisDlg, IDC_COLORBTCTRL_ZAXIS_JOG_PLUS, DISPID_MOUSEDOWN, CManuMotorZAxisDlg::MouseDownColorbtctrlZaxisJogPlus, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CManuMotorZAxisDlg, IDC_COLORBTCTRL_ZAXIS_JOG_PLUS, DISPID_MOUSEUP, CManuMotorZAxisDlg::MouseUpColorbtctrlZaxisJogPlus, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CManuMotorZAxisDlg, IDC_COLORBTCTRL_ZAXIS_JOG_MINUS2, DISPID_MOUSEDOWN, CManuMotorZAxisDlg::MouseDownColorbtctrlZaxisJogMinus2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CManuMotorZAxisDlg, IDC_COLORBTCTRL_ZAXIS_JOG_MINUS2, DISPID_MOUSEUP, CManuMotorZAxisDlg::MouseUpColorbtctrlZaxisJogMinus2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


void CManuMotorZAxisDlg::MouseDownColorbtctrlZaxisJogPlus(short Button, short Shift, long X, long Y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int nAxisNo = m_nAxisNo;
	int nPickerNo = m_Combo_RobotChoose.GetCurSel();
	int nSelArr[8];
	memset(nSelArr, -1, sizeof(nSelArr));

// 	if (!g_COVER_SW.GetStatus() == TRUE)
// 	{
// 		CParamList AlarmData;
// 		g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK, AlarmData, NULL);
// 		return;
// 	}

	nPickerNo += PickerStartInx();
	
	g_Motor[nAxisNo]->V_Move(eJogDir_Negative, _eBbxmnpAxis(nPickerNo), m_nJogSpeed, 20);
}


void CManuMotorZAxisDlg::MouseUpColorbtctrlZaxisJogPlus(short Button, short Shift, long X, long Y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int nAxisNo = m_nAxisNo;
	g_Motor[nAxisNo]->V_MoveStop();
}


void CManuMotorZAxisDlg::MouseDownColorbtctrlZaxisJogMinus2(short Button, short Shift, long X, long Y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int nAxisNo = m_nAxisNo;
	int nPickerNo = m_Combo_RobotChoose.GetCurSel();
	int nSelArr[8];
	memset(nSelArr, -1, sizeof(nSelArr));
	nSelArr[nPickerNo] = nPickerNo;
// 	if (!g_COVER_SW.GetStatus() == TRUE)
// 	{
// 		CParamList AlarmData;
// 		g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK, AlarmData, NULL);
// 		return;
// 	}


	nPickerNo += PickerStartInx();
	

	//g_Motor[nMotNo]->V_Move(eJogDir_Positive, 1, &nSelArr[0], m_nJogSpeed, 20);	
	g_Motor[nAxisNo]->V_Move(eJogDir_Positive, _eBbxmnpAxis(nPickerNo), m_nJogSpeed, 20);
}


void CManuMotorZAxisDlg::MouseUpColorbtctrlZaxisJogMinus2(short Button, short Shift, long X, long Y)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int nAxisNo = m_nAxisNo;
	g_Motor[nAxisNo]->V_MoveStop();
}


void CManuMotorZAxisDlg::OnBnClickedButtonReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nAxisNo = m_nAxisNo;
	// TODO: Add your control notification handler code here
	g_Motor[nAxisNo]->AlarmReset();
	Sleep(100);
	g_Motor[nAxisNo]->AlarmReset();
}


void CManuMotorZAxisDlg::OnBnClickedButtonZr()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
// 	if (!g_COVER_SW.GetStatus() == TRUE)
// 	{
// 		CParamList AlarmData;
// 		g_Error.AlarmReport(ERR_MOT_INTERLOCK_DOOR_IS_NOT_LOCK, AlarmData, NULL);
// 		return;
// 	}

	int nAxisNo = m_nAxisNo;
	int nPicker = m_Combo_RobotChoose.GetCurSel();

	nPicker += PickerStartInx();
	//UI Disable
	GetDlgItem(IDC_COLORBTCTRL_ZAXIS_JOG_PLUS)->EnableWindow(FALSE);
	GetDlgItem(IDC_COLORBTCTRL_ZAXIS_JOG_MINUS2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ZR)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SERVO_ONOFF)->EnableWindow(FALSE);
	GetDlgItem(IDCANCEL2)->EnableWindow(FALSE);

	// [0910 : 정말로 Home Check를 시작합니까?]	// By kjj [150603] Fix OS Alarm Message
	CStringList strList;

	int resu = AfxMessageBox("Do you want Home Check?", MB_YESNO);

	BOOL bOriginFlag = FALSE;
	if(resu == IDYES)
	{
		int nPickerMotorNum = PickerAxisNumber();
		if (g_Motor[m_nAxisNo]->GetServoState(_eBbxmnpAxis(nPicker)) == FALSE)
		{
			// By jhjang [151109] 2D Barcode Shuttle "EM99" Msg 추가
			GetDlgItem(IDC_COLORBTCTRL_ZAXIS_JOG_PLUS)->EnableWindow(TRUE);
			GetDlgItem(IDC_COLORBTCTRL_ZAXIS_JOG_MINUS2)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_ZR)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_SERVO_ONOFF)->EnableWindow(TRUE);
			GetDlgItem(IDCANCEL2)->EnableWindow(TRUE);
			return;
		}

		bOriginFlag = g_pBbxmnpClient->OriginWhile(CBbxmnpClient::_ePICK_MOTOR(nPickerMotorNum), _eBbxmnpAxis(nPicker));
	}
	else
	{
		GetDlgItem(IDC_COLORBTCTRL_ZAXIS_JOG_PLUS)->EnableWindow(TRUE);
		GetDlgItem(IDC_COLORBTCTRL_ZAXIS_JOG_MINUS2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ZR)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SERVO_ONOFF)->EnableWindow(TRUE);
		GetDlgItem(IDCANCEL2)->EnableWindow(TRUE);
		return;
	}

	GetDlgItem(IDC_COLORBTCTRL_ZAXIS_JOG_PLUS)->EnableWindow(TRUE);
	GetDlgItem(IDC_COLORBTCTRL_ZAXIS_JOG_MINUS2)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_ZR)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SERVO_ONOFF)->EnableWindow(TRUE);
	GetDlgItem(IDCANCEL2)->EnableWindow(TRUE);

	if(bOriginFlag != ERR_NO_ERROR)
	{
		return;
	}
	//개별 Picker Safety 위치로 이동하는 Logic 구현
}


void CManuMotorZAxisDlg::OnBnClickedButtonServoOnoff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nAxisNo = m_nAxisNo;

	int nPickerNo = m_Combo_RobotChoose.GetCurSel();
	nPickerNo += PickerStartInx();
	
	BOOL bCurServoStat = g_Motor[nAxisNo]->GetServoState(_eBbxmnpAxis(nPickerNo));

	if(bCurServoStat == TRUE)
	{
		g_Motor[nAxisNo]->ServoOff(_eBbxmnpAxis(nPickerNo));
	}
	else
	{
		g_Motor[nAxisNo]->ServoOn(_eBbxmnpAxis(nPickerNo));
	}

}

int CManuMotorZAxisDlg::PickerStartInx()
{
	int nCurSelect = m_Combo_RobotChoose.GetCurSel();
	int nPickerNum = 0;
// 	switch (m_nAxisNo)
// 	{
		// 		case CBbxmnpClient::PICK_MOTOR_LOAD_TRAY:		nPickerNum = eBMAxis_LOAD_TRAY_Picker01;	break;
		// 		case CBbxmnpClient::PICK_MOTOR_UNLOAD_TRAY:	nPickerNum = eBMAxis_UNLOAD_TRAY_Picker01;	break;
		// 		case CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1:	nPickerNum = eBMAxis_LOAD_TESTPP_1_Picker01;	break;
		// 		case CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1:	nPickerNum = eBMAxis_UNLOAD_TESTPP_1_Picker01;	break;
		// 		case CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2:	nPickerNum = eBMAxis_LOAD_TESTPP_2_Picker01;	break;
		// 		case CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2:		nPickerNum = eBMAxis_UNLOAD_TESTPP_2_Picker01;	break;
	
	if( eAXIS_TRAY_PP_1_Z == m_nAxisNo ){nPickerNum = eBMAxis_LOAD_TRAY_Picker01; }
	if( eAXIS_TEST_PP_1_Z == m_nAxisNo) {nPickerNum = eBMAxis_LOAD_TESTPP_1_Picker01;}	
	if( eAXIS_TEST_PP_1_Z2 == m_nAxisNo) {nPickerNum = eBMAxis_UNLOAD_TESTPP_1_Picker01; }	
	if( eAXIS_TRAY_PP_2_Z == m_nAxisNo) {nPickerNum = eBMAxis_UNLOAD_TRAY_Picker01; }	
	if( eAXIS_TEST_PP_2_Z == m_nAxisNo) {nPickerNum = eBMAxis_LOAD_TESTPP_2_Picker01; }	
	if( eAXIS_TEST_PP_2_Z2 == m_nAxisNo) {nPickerNum = eBMAxis_UNLOAD_TESTPP_2_Picker01; }	
//	}


	if (m_nAxisNo == eAXIS_TEST_PP_1_Z || m_nAxisNo == eAXIS_TEST_PP_1_Z2 || m_nAxisNo == eAXIS_TEST_PP_2_Z || m_nAxisNo == eAXIS_TEST_PP_2_Z2) 
	{
		//Change로 변경 해도
		int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
		if (nBoardType == eBoardDir_Backward) {
			int nCount = 7 - nCurSelect;
			int nGap = abs(nCount - nCurSelect); //6 - 1
			if (nCurSelect > 3) 
			{			
				//nPickerNum -= nCurSelect;
				nPickerNum -= nGap;
				//nCurselect + 8 (eBMAxis_LOAD_TESTPP_1_Picker01) - nCurselect;
				//4          + 8 (eBMAxis_LOAD_TESTPP_1_Picker01) - nCurselect;
				//4 선택하면 3만들어져야 함.
				//5 선택하면 2 만들어져야 함.
				//6 선택하면 1 만들어져야 함.
				//7 선택하면 0 만들어져야 함.
			}
			else 
			{
				nPickerNum += nGap;
				//nCurselect + 8 (eBMAxis_LOAD_TESTPP_1_Picker01) + 4;
				//0 선택하면 7만들어져야 함.
				//1 선택하면 6만들어져야 함.
			}
		}
	}

	return nPickerNum;
}

int CManuMotorZAxisDlg::PickerAxisNumber()
{
	int nAxis = 0;
// 	switch (m_nAxisNo)
// 	{
		if(eAXIS_TRAY_PP_1_Z == m_nAxisNo)		nAxis = CBbxmnpClient::PICK_MOTOR_LOAD_TRAY;	
		if(eAXIS_TRAY_PP_2_Z == m_nAxisNo)		nAxis = CBbxmnpClient::PICK_MOTOR_UNLOAD_TRAY;	
		if(eAXIS_TEST_PP_1_Z == m_nAxisNo)		nAxis = CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1;	
		if(eAXIS_TEST_PP_1_Z == m_nAxisNo)	nAxis = CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1;	
		if(eAXIS_TEST_PP_2_Z == m_nAxisNo)		nAxis = CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2;	
		if(eAXIS_TEST_PP_2_Z2 == m_nAxisNo)	nAxis = CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2;	
//	}
	return nAxis;
}


void CManuMotorZAxisDlg::OnBnClickedButtonStop()
{
	int nAxisNo = m_nAxisNo;

	int nPickerNo = m_Combo_RobotChoose.GetCurSel();
	nPickerNo += PickerStartInx();

	g_Motor[nAxisNo]->V_MoveStop();
	
}


void CManuMotorZAxisDlg::OnDestroy()
{
	CDialog::OnDestroy();

	//KillTimer(0);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


_eBbxmnpAxis CManuMotorZAxisDlg::CHANGE(int eAxis)
{
// 	if (m_nBoardDir == eBoardDir_Forward) {
// 		return (_eBbxmnpAxis)eAxis;
// 	}
// 	else {
// 		//모든 Picker Num Zero Base로 변경
// 		//0,1,2,3,4,5,6,7
// 		int nZrBase = (eAxis > 7) ? (eAxis - MAX_PICKER_Z_AXIS_CNT) : eAxis; //모든 Picker Num Zero Base로 변경
// 
// 
// 		int nConvert = abs((MAX_PICKER_Z_AXIS_CNT - 1)/*7*/ - nZrBase); //7, 6, 5, 4, 3, 2, 1
// 		int nGap = abs(nConvert - nZrBase);
// 		//int nGap = abs(nConvert - (MAX_PICKER_Z_AXIS_CNT - 1));
// 
// 
// 		int nStartIdx = 0;
// 		if (eBMAxis_LOAD_TESTPP_1_Picker01 <= eAxis && eAxis <= eBMAxis_LOAD_TESTPP_1_Picker08)
// 		{
// 			nStartIdx = eBMAxis_LOAD_TESTPP_1_Picker01;
// 		}
// 		else if (eBMAxis_UNLOAD_TESTPP_1_Picker01 <= eAxis && eAxis <= eBMAxis_UNLOAD_TESTPP_1_Picker08)
// 		{
// 			nStartIdx = eBMAxis_UNLOAD_TESTPP_1_Picker01;
// 		}
// 		else if (eBMAxis_LOAD_TESTPP_2_Picker01 <= eAxis && eAxis <= eBMAxis_LOAD_TESTPP_2_Picker08)
// 		{
// 			nStartIdx = eBMAxis_LOAD_TESTPP_2_Picker01;
// 		}
// 		else if (eBMAxis_UNLOAD_TESTPP_2_Picker01 <= eAxis && eAxis <= eBMAxis_UNLOAD_TESTPP_2_Picker08)
// 		{
// 			nStartIdx = eBMAxis_UNLOAD_TESTPP_2_Picker01;
// 		}
// 
// 
// 
// 
// 
// 		//Gap과 Zero Base 차이 만큼 기존 축 번호에 더해 주면 된다.
// 		int nRet = 0;
// 		int nPickerIdx = eAxis % 8;
// 		if (nPickerIdx > 3)
// 		{
// 			nRet = nStartIdx - nGap;
// 		}
// 		else
// 		{
// 			nRet = nStartIdx + nGap;
// 		}
// 
// 
// 		//return (_eBbxmnpAxis)(nStartIdx + nGap);
// 
// 		return ((_eBbxmnpAxis)m_vPkrIdx[eAxis]);
// 	}



	//모든 Picker Num Zero Base로 변경
	//0,1,2,3,4,5,6,7
	int nZrBase = (eAxis > 7) ? (eAxis - MAX_PICKER_Z_AXIS_CNT) : eAxis; //모든 Picker Num Zero Base로 변경


	int nConvert = abs((MAX_PICKER_Z_AXIS_CNT - 1)/*7*/ - nZrBase); //7, 6, 5, 4, 3, 2, 1
	int nGap = abs(nConvert - nZrBase);
	//int nGap = abs(nConvert - (MAX_PICKER_Z_AXIS_CNT - 1));


	int nStartIdx = 0;
	if (eBMAxis_LOAD_TESTPP_1_Picker01 <= eAxis && eAxis <= eBMAxis_LOAD_TESTPP_1_Picker08)
	{
		nStartIdx = eBMAxis_LOAD_TESTPP_1_Picker01;
	}
	else if (eBMAxis_UNLOAD_TESTPP_1_Picker01 <= eAxis && eAxis <= eBMAxis_UNLOAD_TESTPP_1_Picker08)
	{
		nStartIdx = eBMAxis_UNLOAD_TESTPP_1_Picker01;
	}
	else if (eBMAxis_LOAD_TESTPP_2_Picker01 <= eAxis && eAxis <= eBMAxis_LOAD_TESTPP_2_Picker08)
	{
		nStartIdx = eBMAxis_LOAD_TESTPP_2_Picker01;
	}
	else if (eBMAxis_UNLOAD_TESTPP_2_Picker01 <= eAxis && eAxis <= eBMAxis_UNLOAD_TESTPP_2_Picker08)
	{
		nStartIdx = eBMAxis_UNLOAD_TESTPP_2_Picker01;
	}





	//Gap과 Zero Base 차이 만큼 기존 축 번호에 더해 주면 된다.
	int nRet = 0;
	int nPickerIdx = eAxis % 8;
	if (nPickerIdx > 3)
	{
		nRet = nStartIdx - nGap;
	}
	else
	{
		nRet = nStartIdx + nGap;
	}


	//return (_eBbxmnpAxis)(nStartIdx + nGap);

	return ((_eBbxmnpAxis)m_vPkrIdx[eAxis]);
}