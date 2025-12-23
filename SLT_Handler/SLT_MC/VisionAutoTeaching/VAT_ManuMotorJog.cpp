// VAT_ManuMotorJog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "VAT_ManuMotorJog.h"
#include "afxdialogex.h"


// CVAT_ManuMotorJog 대화 상자입니다.
#define  axis_non (-1)
#define VAT_JOG_SPEED_MAX_NUM (10)

const int c_nAxisNo[VAT_HAND_MAX][VAT_MAX_AXIS_XYZ] = {
	{ eAXIS_TRAY_PP_1_X, axis_non, eAXIS_TRAY_PP_1_Z },							//LD Tray PP
	{ eAXIS_TRAY_PP_2_X, eAXIS_TRAY_PP_2_Y, eAXIS_TRAY_PP_2_Z },				//UD Tray PP
	{ eAXIS_TEST_PP_1_X, eAXIS_TEST_PP_1_Y, eAXIS_TEST_PP_1_Z },				//LD Test PP#1
	{ eAXIS_TEST_PP_1_X2, eAXIS_TEST_PP_1_Y, eAXIS_TEST_PP_1_Z2 },				//UD Test PP#1
	{ eAXIS_TEST_PP_2_X, eAXIS_TEST_PP_2_Y, eAXIS_TEST_PP_2_Z },				//LD Test PP#2
	{ eAXIS_TEST_PP_2_X2, eAXIS_TEST_PP_2_Y, eAXIS_TEST_PP_2_Z2 },				//UD Test PP#2
};

const int c_nHandSenID[2][4] = {
	{IDC_VAT_CTRL1_1, IDC_VAT_CTRL1_2, IDC_VAT_CTRL1_3, IDC_VAT_CTRL1_4},
	{IDC_VAT_CTRL2_1, IDC_VAT_CTRL2_2, IDC_VAT_CTRL2_3, IDC_VAT_CTRL2_4 },
};

IMPLEMENT_DYNAMIC(CVAT_ManuMotorJog, CDialogEx)

CVAT_ManuMotorJog::CVAT_ManuMotorJog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VAT_MANU_MOTOR_JOG, pParent)
{
	m_nCurHand = -1; 

	m_pMotorX = NULL;
	m_pMotorY = NULL;
	m_pMotorZ = NULL;

	m_nJogSpeedIdx = 0;

	m_vXYSpeed.clear();
	m_vZSpeed.clear();	

	m_bLastExecuteStatus = FALSE;
}

CVAT_ManuMotorJog::~CVAT_ManuMotorJog()
{
	int test = 1;

}

void CVAT_ManuMotorJog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_VAT_COMBO_JOG_SPEED, m_nJogSpeedIdx); //Combo Index
}


BEGIN_MESSAGE_MAP(CVAT_ManuMotorJog, CDialogEx)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CVAT_ManuMotorJog 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CVAT_ManuMotorJog, CDialogEx)
	ON_EVENT(CVAT_ManuMotorJog, IDC_COLORBTCTRL_X_AXIS_LEFT, DISPID_MOUSEUP, CVAT_ManuMotorJog::MouseUpColorbtctrlXAxisLeft, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CVAT_ManuMotorJog, IDC_COLORBTCTRL_X_AXIS_LEFT, DISPID_MOUSEDOWN, CVAT_ManuMotorJog::MouseDownColorbtctrlXAxisLeft, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CVAT_ManuMotorJog, IDC_COLORBTCTRL_X_AXIS_RIGHT, DISPID_MOUSEUP, CVAT_ManuMotorJog::MouseUpColorbtctrlXAxisRight, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CVAT_ManuMotorJog, IDC_COLORBTCTRL_X_AXIS_RIGHT, DISPID_MOUSEDOWN, CVAT_ManuMotorJog::MouseDownColorbtctrlXAxisRight, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CVAT_ManuMotorJog, IDC_COLORBTCTRL_Y_AXIS_FORWARD, DISPID_MOUSEUP, CVAT_ManuMotorJog::MouseUpColorbtctrlYAxisForward, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CVAT_ManuMotorJog, IDC_COLORBTCTRL_Y_AXIS_FORWARD, DISPID_MOUSEDOWN, CVAT_ManuMotorJog::MouseDownColorbtctrlYAxisForward, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CVAT_ManuMotorJog, IDC_COLORBTCTRL_Y_AXIS_BACKWARD, DISPID_MOUSEUP, CVAT_ManuMotorJog::MouseUpColorbtctrlYAxisBackward, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CVAT_ManuMotorJog, IDC_COLORBTCTRL_Y_AXIS_BACKWARD, DISPID_MOUSEDOWN, CVAT_ManuMotorJog::MouseDownColorbtctrlYAxisBackward, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CVAT_ManuMotorJog, IDC_COLORBTCTRL_Z_AXIS_UP, DISPID_MOUSEUP, CVAT_ManuMotorJog::MouseUpColorbtctrlZAxisUp, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CVAT_ManuMotorJog, IDC_COLORBTCTRL_Z_AXIS_UP, DISPID_MOUSEDOWN, CVAT_ManuMotorJog::MouseDownColorbtctrlZAxisUp, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CVAT_ManuMotorJog, IDC_COLORBTCTRL_Z_AXIS_DOWN, DISPID_MOUSEUP, CVAT_ManuMotorJog::MouseUpColorbtctrlZAxisDown, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CVAT_ManuMotorJog, IDC_COLORBTCTRL_Z_AXIS_DOWN, DISPID_MOUSEDOWN, CVAT_ManuMotorJog::MouseDownColorbtctrlZAxisDown, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CVAT_ManuMotorJog, IDC_VAT_BTN_ALL_VACUUM, 22, CVAT_ManuMotorJog::ClickVatBtnAllVacuum, VTS_NONE)
	ON_EVENT(CVAT_ManuMotorJog, IDC_VAT_BTN_ALL_BLOW, 22, CVAT_ManuMotorJog::ClickVatBtnAllBlow, VTS_NONE)
END_EVENTSINK_MAP()


BOOL CVAT_ManuMotorJog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//Combo Box Motor Speed 초기화.
	InitComboBox();
	InitMotorSpeed();

	//Jog button 활성화.
	EnableJogButton(TRUE);

	//Tray Load pp로 초기화.
//	InitHandChange(m_nCurHand);;

	//Text 초기화.
	SetDlgItemText(IDC_X_POS, _T("0"));
	SetDlgItemText(IDC_Y_POS, _T("0"));
	SetDlgItemText(IDC_Z_POS, _T("0"));

// 	SetTimer(eTimer_CheckExecuteStatus, 200, NULL);
// 	SetTimer(eTimer_DisplayCurPos, 200, NULL);
// 	SetTimer(eTimer_HandSenStatus, 200, NULL);

	return TRUE;
}

void CVAT_ManuMotorJog::OnDestroy()
{	
	for (int i = 0; i < eTimer_Max; i++) {
		KillTimer(i);
	}

	CDialogEx::OnDestroy();
}


//===========================================================
//COMMENT : 외부에서 Hand 변경 시, 호출 필요.
//===========================================================
void CVAT_ManuMotorJog::InitHandChange(int nHand) {
	if (nHand == -1)
		return;

	//CurHand 변경.
	m_nCurHand = nHand;

	//KillTimer
	for (int i = 0; i < eTimer_Max; i++) {
		KillTimer(i);
	}

	//Axis 초기화.
	if (m_pMotorX != NULL)	m_pMotorX = NULL;
	if (m_pMotorY != NULL)	m_pMotorY = NULL;
	if (m_pMotorZ != NULL)	m_pMotorZ = NULL;

	//Jog Move를 위한 Motor 축 정의.
	InitMotorAxis(nHand);

	//Timer 재시작
	SetTimer(eTimer_CheckExecuteStatus, 200, NULL);
	SetTimer(eTimer_DisplayCurPos, 200, NULL);
	SetTimer(eTimer_HandSenStatus, 200, NULL);
}


//============================================================================================
//COMMENT : Mode 변경 시, 현재 Mode Timer Check 불필요.
//			해당 모드에 대한 VAT_ManuMotorJog 객체를 삭제 하는 것이 아니라, Timer만 제거하기 위함.
//============================================================================================
void CVAT_ManuMotorJog::Kill_Timer()
{
	for (int i = 0; i < eTimer_Max; i++) {
		KillTimer(i);
	}
}


//====================================================================
//COMMENT : VAT Hand 변경 시, 호출 필요함.
//====================================================================
void CVAT_ManuMotorJog::InitMotorAxis(int nHand)
{
	int nAxisNo[VAT_HAND_MAX][VAT_MAX_AXIS_XYZ] = {
		{ eAXIS_TRAY_PP_1_X, axis_non, eAXIS_TRAY_PP_1_Z },							//LD Tray PP
		{ eAXIS_TRAY_PP_2_X, eAXIS_TRAY_PP_2_Y, eAXIS_TRAY_PP_2_Z },				//UD Tray PP
		{ eAXIS_TEST_PP_1_X, eAXIS_TEST_PP_1_Y, eAXIS_TEST_PP_1_Z },				//LD Test PP#1
		{ eAXIS_TEST_PP_1_X2, eAXIS_TEST_PP_1_Y, eAXIS_TEST_PP_1_Z2 },				//UD Test PP#1
		{ eAXIS_TEST_PP_2_X, eAXIS_TEST_PP_2_Y, eAXIS_TEST_PP_2_Z },				//LD Test PP#2
		{ eAXIS_TEST_PP_2_X2, eAXIS_TEST_PP_2_Y, eAXIS_TEST_PP_2_Z2 },				//UD Test PP#2
	};


	//초기화.
	if (m_pMotorX != NULL)	m_pMotorX = NULL;
	if (m_pMotorY != NULL)	m_pMotorY = NULL;
	if (m_pMotorZ != NULL)	m_pMotorZ = NULL;

	//Hand X축	
	if (nAxisNo[nHand][VAT_X] != axis_non) {
		m_pMotorX = (OneAxis*)g_Motor[nAxisNo[nHand][VAT_X]];
	}

	//Hand Y축
	if (nAxisNo[nHand][VAT_Y] != axis_non) {
		m_pMotorY = (OneAxis*)g_Motor[nAxisNo[nHand][VAT_Y]];
	}

	//Hand Z축
	if (nAxisNo[nHand][VAT_Z] != axis_non) {
		m_pMotorZ = (OneAxis*)g_Motor[nAxisNo[nHand][VAT_Z]];
	}

	if (!g_bVATStatusExecute)
	{
		//Axis Disable Check
		if (m_pMotorX == NULL) {
			GetDlgItem(IDC_COLORBTCTRL_X_AXIS_LEFT)->EnableWindow(FALSE);
			GetDlgItem(IDC_COLORBTCTRL_X_AXIS_RIGHT)->EnableWindow(FALSE);
		}
		else {
			GetDlgItem(IDC_COLORBTCTRL_X_AXIS_LEFT)->EnableWindow(TRUE);
			GetDlgItem(IDC_COLORBTCTRL_X_AXIS_RIGHT)->EnableWindow(TRUE);
		}
		if (m_pMotorY == NULL) {
			GetDlgItem(IDC_COLORBTCTRL_Y_AXIS_FORWARD)->EnableWindow(FALSE);
			GetDlgItem(IDC_COLORBTCTRL_Y_AXIS_BACKWARD)->EnableWindow(FALSE);
		}
		else {
			GetDlgItem(IDC_COLORBTCTRL_Y_AXIS_FORWARD)->EnableWindow(TRUE);
			GetDlgItem(IDC_COLORBTCTRL_Y_AXIS_BACKWARD)->EnableWindow(TRUE);
		}
		if (m_pMotorZ == NULL) {
			GetDlgItem(IDC_COLORBTCTRL_Z_AXIS_UP)->EnableWindow(FALSE);
			GetDlgItem(IDC_COLORBTCTRL_Z_AXIS_DOWN)->EnableWindow(FALSE);
		}
		else {
			GetDlgItem(IDC_COLORBTCTRL_Z_AXIS_UP)->EnableWindow(TRUE);
			GetDlgItem(IDC_COLORBTCTRL_Z_AXIS_DOWN)->EnableWindow(TRUE);
		}
	}
}


//Speed 값 잘못들어 가는 것을 방지 하기 위해, 속도 변수 따로 관리.
void CVAT_ManuMotorJog::InitMotorSpeed()
{
	//X, Y Speed
	m_vXYSpeed.push_back(100);
	m_vXYSpeed.push_back(500);
	m_vXYSpeed.push_back(1000);
	m_vXYSpeed.push_back(2000);
	m_vXYSpeed.push_back(3000);
	m_vXYSpeed.push_back(4000);
	m_vXYSpeed.push_back(5000);
	m_vXYSpeed.push_back(8000);
	m_vXYSpeed.push_back(10000);
	m_vXYSpeed.push_back(20000);

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

void CVAT_ManuMotorJog::InitComboBox()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_VAT_COMBO_JOG_SPEED);

	//Font Size
	CFont font;
	font.CreatePointFont(200, "Fixedsys");
	pComboBox->SetFont(&font);
}


void CVAT_ManuMotorJog::DisplayCurPos()
{
	double dCurPos = 0.0;
	CString strCurPos = _T("");

	if (m_pMotorX != NULL) {
		m_pMotorX->GetCurrentPos(dCurPos);
		strCurPos.Format("%5.3f", dCurPos / 1000.0);
		SetDlgItemText(IDC_VAT_HAND_X_POS, strCurPos);
	}
	if (m_pMotorY != NULL) {
		m_pMotorY->GetCurrentPos(dCurPos);
		strCurPos.Format("%5.3f", dCurPos / 1000.0);
		SetDlgItemText(IDC_VAT_HAND_Y_POS, strCurPos);
	}
	if (m_pMotorZ != NULL) {
		int nIdx = m_pMotorZ->GetAxisNo();
		if (eAXIS_TRAY_PP_1_Z <= nIdx && nIdx <= eAXIS_TEST_PP_2_Z2) {
			int nBBxIdx = 0;
			if (eAXIS_TRAY_PP_1_Z == nIdx) {
				nBBxIdx = eBMAxis_LOAD_TRAY_Picker01;
			}
			else if (eAXIS_TRAY_PP_2_Z == nIdx) {
				nBBxIdx = eBMAxis_UNLOAD_TRAY_Picker01;
			}
			else if (eAXIS_TEST_PP_1_Z == nIdx) {
				nBBxIdx = eBMAxis_LOAD_TESTPP_1_Picker08;
			}
			else if (eAXIS_TEST_PP_1_Z2 == nIdx) {
				nBBxIdx = eBMAxis_UNLOAD_TESTPP_1_Picker08;
			}
			else if (eAXIS_TEST_PP_2_Z == nIdx) {
				nBBxIdx = eBMAxis_LOAD_TESTPP_2_Picker08;
			}
			else if (eAXIS_TEST_PP_2_Z2 == nIdx) {
				nBBxIdx = eBMAxis_UNLOAD_TESTPP_2_Picker08;
			}
//			switch (nIdx)
//			{
			//Backward
// 			case eAXIS_TRAY_PP_1_Z:		nBBxIdx = eBMAxis_LOAD_TRAY_Picker01;	break;
// 			case eAXIS_TEST_PP_1_Z:		nBBxIdx = eBMAxis_LOAD_TESTPP_1_Picker08;	break;
// 			case eAXIS_TEST_PP_1_Z2:	nBBxIdx = eBMAxis_UNLOAD_TESTPP_1_Picker08;	break;
// 			case eAXIS_TRAY_PP_2_Z:		nBBxIdx = eBMAxis_UNLOAD_TRAY_Picker01;	break;
// 			case eAXIS_TEST_PP_2_Z:		nBBxIdx = eBMAxis_LOAD_TESTPP_2_Picker08;	break;
// 			case eAXIS_TEST_PP_2_Z2:	nBBxIdx = eBMAxis_UNLOAD_TESTPP_2_Picker08;	break;
// 			default:
// 				break;
//			}

			g_Motor[nIdx]->GetCurrentPos(dCurPos, _eBbxmnpAxis(nBBxIdx));
			strCurPos.Format("%5.3f", dCurPos / 1000.0);
			SetDlgItemText(IDC_VAT_HAND_Z_POS, strCurPos);
		}
	}
}

void CVAT_ManuMotorJog::DisplayHandSenStatus()
{
	if (m_nCurHand == -1) {
		return;
	}

	for (int nY = 0; nY < MAX_TEST_PP_PKR_CNT_Y; nY++) {
		for (int nX = 0; nX < MAX_TEST_PP_PKR_CNT_X; nX++) {			
			int nVac = FALSE;

			switch (m_nCurHand)
			{
			case VAT_HAND_LOAD_TRAYPP:		nVac = g_TaskTrayLoadPP.GetVacuum(CPoint(nX, nY));								break;
			case VAT_HAND_UNLOAD_TRAYPP:	nVac = g_TaskTrayUnloadPP.GetVacuum(CPoint(nX, nY));							break;
			case VAT_HAND_LOAD_TESTPP1:		nVac = g_TaskTestPP[eTestPP_1].GetVacuum(eTestPPLoadType, CPoint(nX, nY));		break;
			case VAT_HAND_UNLOAD_TESTPP1:	nVac = g_TaskTestPP[eTestPP_1].GetVacuum(eTestPPUnLoadType, CPoint(nX, nY));	break;
			case VAT_HAND_LOAD_TESTPP2:		nVac = g_TaskTestPP[eTestPP_2].GetVacuum(eTestPPLoadType, CPoint(nX, nY));		break;
			case VAT_HAND_UNLOAD_TESTPP2:	nVac = g_TaskTestPP[eTestPP_2].GetVacuum(eTestPPUnLoadType, CPoint(nX, nY));	break;
			default:
				break;
			}


			CDSSPanel* pSensor = NULL;
			pSensor = (CDSSPanel*)GetDlgItem(c_nHandSenID[nY][nX]);
			if (nVac)  pSensor->On();
			else       pSensor->Off();
		}
	}

}


void CVAT_ManuMotorJog::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
		case eTimer_CheckExecuteStatus: //상시 check
		{
			KillTimer(nIDEvent);

			//상태 지속적으로 변경 되는 것을 방지하고자 Last status check 진행.
			if (g_bVATStatusExecute != m_bLastExecuteStatus)
			{
				EnableJogButton(!g_bVATStatusExecute);
				m_bLastExecuteStatus = g_bVATStatusExecute;
 			}

			SetTimer(nIDEvent, 500, NULL);
		}break;
		case eTimer_DisplayCurPos:
		{
			KillTimer(nIDEvent);

			DisplayCurPos();

			SetTimer(nIDEvent, 200, NULL);
		}break;
		case eTimer_HandSenStatus:
		{
			KillTimer(nIDEvent);

			DisplayHandSenStatus();

			SetTimer(nIDEvent, 200, NULL);
		}break;
		case eTimer_x_move_left:
		{
			KillTimer(nIDEvent);
			if (m_pMotorX != NULL) {
				int nRet = m_pMotorX->V_MoveCheckInterlock(-100, 1);
				if (nRet != ERR_NO_ERROR)
					m_pMotorX->V_MoveStop();
			}

			SetTimer(nIDEvent, 50, NULL);
		}break;
		case eTimer_x_move_right:
		{
			KillTimer(nIDEvent);
			if (m_pMotorX != NULL) {
				int nRet = m_pMotorX->V_MoveCheckInterlock(100, 1);
				if (nRet != ERR_NO_ERROR)
					m_pMotorX->V_MoveStop();
			}

			SetTimer(nIDEvent, 50, NULL);
		}break;
		case eTimer_y_move_forward:
		{
			KillTimer(nIDEvent);
			if (m_pMotorY != NULL)
			{
				int nRet = m_pMotorY->V_MoveCheckInterlock(100, 1);
				if (nRet != ERR_NO_ERROR)
					m_pMotorX->V_MoveStop();
			}
			SetTimer(nIDEvent, 50, NULL);
		}break;
		case eTimer_y_move_backward:
		{
			KillTimer(nIDEvent);
			if (m_pMotorY != NULL)
			{
				int nRet = m_pMotorY->V_MoveCheckInterlock(-100, 1);
				if (nRet != ERR_NO_ERROR)
					m_pMotorX->V_MoveStop();
			}
			SetTimer(nIDEvent, 50, NULL);
		}break;
		case eTimer_z_move_up:
		case eTimer_z_move_down:
		{
			KillTimer(nIDEvent);
			if (m_pMotorZ != NULL)
			{
				ERR_CODE nInterLockErr = m_pMotorZ->m_cbInterLock_Jog(m_vZSpeed[m_nJogSpeedIdx], 20, 20);
				if (nInterLockErr != ERR_NO_ERROR) {
					m_pMotorZ->V_MoveStop();
				}
			}
			SetTimer(nIDEvent, 50, NULL);
		}break;
		default:
			break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


int CVAT_ManuMotorJog::JogComInterlock(int nAxis)
{
	switch (nAxis)
	{
	case eVATMotor_X:
	{
		if (m_pMotorX != NULL) {
			int nOrg = m_pMotorX->IsOrgCompleted(nullptr);
			if (nOrg == FALSE) {
				m_pMotorX->V_MoveStop();
				return ERR_MOT_INTERLOCK_IS_NOT_ORG;
			}
		}
	}break;
	case eVATMotor_Y:
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
	case eVATMotor_Z:
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
// 	case eJogMotor_R:
// 	{
// 		if (m_pMotorR != NULL) {
// 			int nOrg = m_pMotorR->IsOrgCompleted(nullptr);
// 			if (nOrg == FALSE)
// 			{
// 				m_pMotorR->V_MoveStop();
// 				return ERR_MOT_INTERLOCK_IS_NOT_ORG;
// 			}
// 		}
// 	}break;
	}


	int nErr = ERR_NO_ERROR;
	nErr = g_TaskSystemCtrl.ChkComInterlock();

	return nErr;
}


BOOL CVAT_ManuMotorJog::IsJogMoveSpeedNormal(int nAxis)
{
	int nRet = FALSE;

	int nMoveSpeed = -1;

	switch (nAxis)
	{
		case eVATMotor_X:
		case eVATMotor_Y:
		{
			nMoveSpeed = m_vXYSpeed[m_nJogSpeedIdx];
			if (100 <= nMoveSpeed && nMoveSpeed <= 20000) {
				nRet = TRUE;
			}	
		}break;
		case eVATMotor_Z:
		{
			nMoveSpeed = m_vZSpeed[m_nJogSpeedIdx];
			if (5 <= nMoveSpeed && nMoveSpeed <= 50) {
				nRet = TRUE;
			}
		}break;
		default:
			break;
	}

	return nRet;

}


//====================================================================
//COMMENT : [X축][LEFT][MOUSE UP]
//====================================================================
void CVAT_ManuMotorJog::MouseUpColorbtctrlXAxisLeft(short Button, short Shift, long X, long Y)
{
	if (m_pMotorX == NULL) return;

	KillTimer(eTimer_x_move_left);
	m_pMotorX->V_MoveStop();
}

//====================================================================
//COMMENT : [X축][LEFT][MOUSE DOWN]
//====================================================================
void CVAT_ManuMotorJog::MouseDownColorbtctrlXAxisLeft(short Button, short Shift, long X, long Y)
{
	if (m_pMotorX == NULL) return;

	int nErr = JogComInterlock(eVATMotor_X);
	if (nErr != ERR_NO_ERROR) {
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, NULL);
		return;
	}

	UpdateData(TRUE);

	BOOL bChk = IsJogMoveSpeedNormal(eVATMotor_X);
	if (bChk != TRUE) {
		return;
	}

	SetTimer(eTimer_x_move_left, 50, NULL);
	g_TaskSystemCtrl.MoveJog(m_pMotorX->GetAxisLogicNo(), -1, m_vXYSpeed[m_nJogSpeedIdx]);
}

//====================================================================
//COMMENT : [X축][RIGHT][MOUSE UP]
//====================================================================
void CVAT_ManuMotorJog::MouseUpColorbtctrlXAxisRight(short Button, short Shift, long X, long Y)
{
	if (m_pMotorX == NULL) return;

	KillTimer(eTimer_x_move_right);
	m_pMotorX->V_MoveStop();
}

//====================================================================
//COMMENT : [X축][RIGHT][MOUSE DOWN]
//====================================================================
void CVAT_ManuMotorJog::MouseDownColorbtctrlXAxisRight(short Button, short Shift, long X, long Y)
{
	if (m_pMotorX == NULL) return;

	int nErr = JogComInterlock(eVATMotor_X);
	if (nErr != ERR_NO_ERROR) {
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, NULL);
		return;
	}

	UpdateData(TRUE);

	BOOL bChk = IsJogMoveSpeedNormal(eVATMotor_X);
	if (bChk != TRUE) {
		return;
	}

	//Move가능
	SetTimer(eTimer_x_move_right, 50, NULL);
	g_TaskSystemCtrl.MoveJog(m_pMotorX->GetAxisLogicNo(), 1, m_vXYSpeed[m_nJogSpeedIdx]);
}

//====================================================================
//COMMENT : [Y축][FORWARD][MOUSE UP]
//====================================================================
void CVAT_ManuMotorJog::MouseUpColorbtctrlYAxisForward(short Button, short Shift, long X, long Y)
{
	if (m_pMotorY == NULL) return;

	KillTimer(eTimer_y_move_forward);
	m_pMotorY->V_MoveStop();
}

//====================================================================
//COMMENT : [Y축][FORWARD][MOUSE DOWN]
//====================================================================
void CVAT_ManuMotorJog::MouseDownColorbtctrlYAxisForward(short Button, short Shift, long X, long Y)
{
	if (m_pMotorY == NULL) return;

	int nErr = JogComInterlock(eVATMotor_Y);
	if (nErr != ERR_NO_ERROR) {
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, NULL);
		return;
	}

	UpdateData(TRUE);

	BOOL bChk = IsJogMoveSpeedNormal(eVATMotor_Y);
	if (bChk != TRUE) {
		return;
	}


	SetTimer(eTimer_y_move_forward, 50, NULL);
	g_TaskSystemCtrl.MoveJog(m_pMotorY->GetAxisLogicNo(), 1, m_vXYSpeed[m_nJogSpeedIdx]);
}


//====================================================================
//COMMENT : [Y축][BACKWARD][MOUSE UP]
//====================================================================
void CVAT_ManuMotorJog::MouseUpColorbtctrlYAxisBackward(short Button, short Shift, long X, long Y)
{
	if (m_pMotorY == NULL) return;

	KillTimer(eTimer_y_move_backward);
	m_pMotorY->V_MoveStop();
}

//====================================================================
//COMMENT : [Y축][BACKWARD][MOUSE DOWN]
//====================================================================
void CVAT_ManuMotorJog::MouseDownColorbtctrlYAxisBackward(short Button, short Shift, long X, long Y)
{
	if (m_pMotorY == NULL) return;

	int nErr = JogComInterlock(eVATMotor_Y);
	if (nErr != ERR_NO_ERROR) {
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, NULL);
		return;
	}

	UpdateData(TRUE);

	BOOL bChk = IsJogMoveSpeedNormal(eVATMotor_Y);
	if (bChk != TRUE) {
		return;
	}


	SetTimer(eTimer_y_move_backward, 50, NULL);
	g_TaskSystemCtrl.MoveJog(m_pMotorY->GetAxisLogicNo(), -1, m_vXYSpeed[m_nJogSpeedIdx]);
}

//====================================================================
//COMMENT : [Z축][UP][MOUSE UP]
//====================================================================
void CVAT_ManuMotorJog::MouseUpColorbtctrlZAxisUp(short Button, short Shift, long X, long Y)
{
	if (m_pMotorZ == NULL) return;

	KillTimer(eTimer_z_move_up);
	m_pMotorZ->V_MoveStop();
}

//====================================================================
//COMMENT : [Z축][UP][MOUSE DOWN]
//====================================================================
void CVAT_ManuMotorJog::MouseDownColorbtctrlZAxisUp(short Button, short Shift, long X, long Y)
{
	if (m_pMotorZ == NULL) return;

	int nErr = JogComInterlock(eVATMotor_Z);
	if (nErr != ERR_NO_ERROR) {
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, NULL);
		return;
	}

	UpdateData(TRUE);
	BOOL bChk = IsJogMoveSpeedNormal(eVATMotor_Z);
	if (bChk != TRUE) {
		return;
	}

	SetTimer(eTimer_z_move_up, 50, NULL);
	int nAxisNo = m_pMotorZ->GetAxisNo();
	if (eAXIS_TRAY_PP_1_Z <= nAxisNo  && nAxisNo <= eAXIS_TEST_PP_2_Z2)
	{
		int nPicker[eBMAxis_Picker_MAX] = { 0, };
		int eMotor = g_pBbxmnpClient->GetZPickerMotorNumber(nAxisNo);
		int nCount = 0;
		for (int nIdx : g_pBbxmnpClient->m_vHandAxis[eMotor])
		{
			nPicker[nCount] = g_pBbxmnpClient->m_vHandMatrix[nIdx];
			nCount++;
		}
		g_Motor[nAxisNo]->V_Move(eJogDir_Negative, MAX_PICKER_CNT_EACH_SLAVE, &nPicker[0], m_vZSpeed[m_nJogSpeedIdx], 20);
		g_Motor[nAxisNo]->V_Move(eJogDir_Negative, MAX_PICKER_CNT_EACH_SLAVE, &nPicker[4], m_vZSpeed[m_nJogSpeedIdx], 20);
	}
}

//====================================================================
//COMMENT : [Z축][DOWN][MOUSE UP]
//====================================================================
void CVAT_ManuMotorJog::MouseUpColorbtctrlZAxisDown(short Button, short Shift, long X, long Y)
{
	if (m_pMotorZ == NULL) return;

	KillTimer(eTimer_z_move_down);
	m_pMotorZ->V_MoveStop();
}

//====================================================================
//COMMENT : [Z축][DOWN][MOUSE DOWN]
//====================================================================
void CVAT_ManuMotorJog::MouseDownColorbtctrlZAxisDown(short Button, short Shift, long X, long Y)
{
	if (m_pMotorZ == NULL) return;

	int nErr = JogComInterlock(eVATMotor_Z);
	if (nErr != ERR_NO_ERROR) {
		CParamList AlarmData;
		g_Error.AlarmReport(nErr, AlarmData, NULL);
		return;
	}

	UpdateData(TRUE);
	BOOL bChk = IsJogMoveSpeedNormal(eVATMotor_Z);
	if (bChk != TRUE) {
		return;
	}

	SetTimer(eTimer_z_move_down, 50, NULL);
	int nAxisNo = m_pMotorZ->GetAxisNo();
	if (eAXIS_TRAY_PP_1_Z <= nAxisNo  && nAxisNo <= eAXIS_TEST_PP_2_Z2)
	{
		int nPicker[eBMAxis_Picker_MAX] = { 0, };
		int eMotor = g_pBbxmnpClient->GetZPickerMotorNumber(nAxisNo);
		int nCount = 0;
		for (int nIdx : g_pBbxmnpClient->m_vHandAxis[eMotor])
		{
			nPicker[nCount] = g_pBbxmnpClient->m_vHandMatrix[nIdx];
			nCount++;
		}
		g_Motor[nAxisNo]->V_Move(eJogDir_Positive, MAX_PICKER_CNT_EACH_SLAVE, &nPicker[0], m_vZSpeed[m_nJogSpeedIdx], 20);
		g_Motor[nAxisNo]->V_Move(eJogDir_Positive, MAX_PICKER_CNT_EACH_SLAVE, &nPicker[4], m_vZSpeed[m_nJogSpeedIdx], 20);
	}
}


void CVAT_ManuMotorJog::ClickVatBtnAllVacuum()
{
	if (m_nCurHand == -1) {
		return;
	}

	switch (m_nCurHand)
	{
	case VAT_HAND_LOAD_TRAYPP:		g_TaskTrayLoadPP.do_VacBlow_All(eVacBlwSt_VacOn);								break;
	case VAT_HAND_UNLOAD_TRAYPP:	g_TaskTrayUnloadPP.do_VacBlow_All(eVacBlwSt_VacOn);								break;
	case VAT_HAND_LOAD_TESTPP1:		g_TaskTestPP[eTestPP_1].do_VacBlow_All(eVacBlwSt_VacOn, eTestPPLoadType);		break;
	case VAT_HAND_UNLOAD_TESTPP1:	g_TaskTestPP[eTestPP_1].do_VacBlow_All(eVacBlwSt_VacOn, eTestPPUnLoadType);		break;
	case VAT_HAND_LOAD_TESTPP2:		g_TaskTestPP[eTestPP_2].do_VacBlow_All(eVacBlwSt_VacOn, eTestPPLoadType);		break;
	case VAT_HAND_UNLOAD_TESTPP2:	g_TaskTestPP[eTestPP_2].do_VacBlow_All(eVacBlwSt_VacOn, eTestPPUnLoadType);		break;
	default:
		break;
	}
}


void CVAT_ManuMotorJog::ClickVatBtnAllBlow()
{
	if (m_nCurHand == -1) {
		return;
	}

	switch (m_nCurHand)
	{
	case VAT_HAND_LOAD_TRAYPP:		g_TaskTrayLoadPP.do_VacBlow_All(eVacBlwSt_BlwOn);								break;
	case VAT_HAND_UNLOAD_TRAYPP:	g_TaskTrayUnloadPP.do_VacBlow_All(eVacBlwSt_BlwOn);								break;
	case VAT_HAND_LOAD_TESTPP1:		g_TaskTestPP[eTestPP_1].do_VacBlow_All(eVacBlwSt_BlwOn, eTestPPLoadType);		break;
	case VAT_HAND_UNLOAD_TESTPP1:	g_TaskTestPP[eTestPP_1].do_VacBlow_All(eVacBlwSt_BlwOn, eTestPPUnLoadType);		break;
	case VAT_HAND_LOAD_TESTPP2:		g_TaskTestPP[eTestPP_2].do_VacBlow_All(eVacBlwSt_BlwOn, eTestPPLoadType);		break;
	case VAT_HAND_UNLOAD_TESTPP2:	g_TaskTestPP[eTestPP_2].do_VacBlow_All(eVacBlwSt_BlwOn, eTestPPUnLoadType);		break;
	default:
		break;
	}
}


void CVAT_ManuMotorJog::EnableJogButton(BOOL bStatus)
{
	GetDlgItem(IDC_COLORBTCTRL_X_AXIS_LEFT)->EnableWindow(bStatus);
	GetDlgItem(IDC_COLORBTCTRL_X_AXIS_RIGHT)->EnableWindow(bStatus);
	GetDlgItem(IDC_COLORBTCTRL_Y_AXIS_FORWARD)->EnableWindow(bStatus);
	GetDlgItem(IDC_COLORBTCTRL_Y_AXIS_BACKWARD)->EnableWindow(bStatus);
	GetDlgItem(IDC_COLORBTCTRL_Z_AXIS_UP)->EnableWindow(bStatus);
	GetDlgItem(IDC_COLORBTCTRL_Z_AXIS_DOWN)->EnableWindow(bStatus);
	GetDlgItem(IDC_VAT_BTN_ALL_VACUUM)->EnableWindow(bStatus);
	GetDlgItem(IDC_VAT_BTN_ALL_BLOW)->EnableWindow(bStatus);
	GetDlgItem(IDC_VAT_COMBO_JOG_SPEED)->EnableWindow(bStatus);

	if (m_nCurHand == VAT_HAND_LOAD_TRAYPP || m_nCurHand == -1) {
		GetDlgItem(IDC_COLORBTCTRL_Y_AXIS_FORWARD)->EnableWindow(FALSE);
		GetDlgItem(IDC_COLORBTCTRL_Y_AXIS_BACKWARD)->EnableWindow(FALSE);
	}
}