// DlgManuMotor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgManuMotor.h"
#include "afxdialogex.h"
#include "DEF_DB_TBL.h"
#include "DEF_LIB_GUI.h"
#include "ManuMotorZAxisDlg.h"

// CDlgManuMotor 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgManuMotor, CDialogEx)

CDlgManuMotor::CDlgManuMotor(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgManuMotor::IDD, pParent)
{
	m_nMotorSpeed         = 0;
	m_dJogSpeed           = 0.0;
	m_strLocation         = _T("");
	m_bPositiveLimitCheck = FALSE;
	m_bNegativeLimitCheck = FALSE;

	m_nAxisNo = 0;

	CBaseTaskManager::SetEqpCtrlMode(eEqpCtrlMode_Manual);
}

CDlgManuMotor::~CDlgManuMotor()
{
	m_listMotorParam.m_fnClear();
	m_cBmpExit.DeleteObject();
}

void CDlgManuMotor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ROBOT_CHOOSE, m_cbRobotChoose);
	DDX_Control(pDX, IDC_COMBO_M_SPEED, m_cbMotorSpeed);
	DDX_Control(pDX, IDC_CLRCHK_ZR_STAT, m_ZrStat);
	DDX_Control(pDX, IDC_COLORBTCTRL_SERVO, m_Servo);
	DDX_Control(pDX, IDC_CLRCHK_AMP_STAT, m_AmpFaultStat);
	DDX_Control(pDX, IDC_CLRCHK_MOTOR_USABLE_STAT, m_MotorUsableStat);
	DDX_Control(pDX, IDC_CLRCHK_SERVO_STAT, m_ServoStat);
	DDX_Control(pDX, IDC_COLORBTCTRL_JOG_PLUS, m_ctrlPlus);
	DDX_Control(pDX, IDC_COLORBTCTRL_JOG_MINUS, m_ctrlMinus);
	// exit
	DDX_Control(pDX, IDOK, m_btnExit);
}


BEGIN_MESSAGE_MAP(CDlgManuMotor, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_ROBOT_CHOOSE, &CDlgManuMotor::OnCbnSelchangeComboRobotChoose)
	ON_CBN_SELCHANGE(IDC_COMBO_M_SPEED, &CDlgManuMotor::OnCbnSelchangeComboMSpeed)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED( IDOK, &CDlgManuMotor::OnBnClickedOk )
	ON_BN_CLICKED(IDC_BUTTON_ZAXIS_PICKER, &CDlgManuMotor::OnBnClickedButtonZaxisPicker)
END_MESSAGE_MAP()


// CDlgManuMotor 메시지 처리기입니다.


BOOL CDlgManuMotor::PreTranslateMessage(MSG* pMsg)
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


BOOL CDlgManuMotor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	g_DMCont.m_dmEQP.SB(BDM0_OPEN_SLT_UI_DLG_HISTORY, TRUE);
	// exit
	m_cBmpExit.LoadBitmap(IDB_EXIT_MOTOR);
	m_btnExit.SetBitmap(m_cBmpExit);

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	g_DB.SELECT_MOTORPARAM_TBL(m_listMotorParam);

	m_dJogSpeed = JOG_SPD_LOW_UMPS;

	int nMaxAxis = m_listMotorParam.m_fnGetCount();
	if(nMaxAxis == 0){
		return FALSE;
	}

	CString strAxisName = _T("");

	for (int i = 0; i < nMaxAxis; i++)
	{
		_tMOTOR_PARAM* pItem = (_tMOTOR_PARAM*)m_listMotorParam.m_fnGetData(i);

		//m_cbRobotChoose.InsertString(i, pItem->strAxisName);

		if( pItem != NULL ){
// 			if(pItem->nAxisNo_Logical<= eAxisNo_PRESS_DN8_SITE8){
// 				switch(pItem->nAxisNo_Logical)
// 				{
// 					case eAxisNo_PRESS_DN1_SITE1:  { strAxisName.Format("PRESS_UNIT_1_DN1_Z");  }break;
// 					case eAxisNo_PRESS_DN2_SITE2:  { strAxisName.Format("PRESS_UNIT_2_DN2_Z");  }break;
// 					case eAxisNo_PRESS_DN3_SITE3:  { strAxisName.Format("PRESS_UNIT_3_DN3_Z");  }break;
// 					case eAxisNo_PRESS_DN4_SITE4:  { strAxisName.Format("PRESS_UNIT_4_DN4_Z");  }break;
// 					case eAxisNo_PRESS_DN5_SITE5:  { strAxisName.Format("PRESS_UNIT_5_DN5_Z");  }break;
// 					case eAxisNo_PRESS_DN6_SITE6:  { strAxisName.Format("PRESS_UNIT_6_DN6_Z");  }break;
// 					case eAxisNo_PRESS_DN7_SITE7:  { strAxisName.Format("PRESS_UNIT_7_DN7_Z");  }break;
// 					case eAxisNo_PRESS_DN8_SITE8:  { strAxisName.Format("PRESS_UNIT_8_DN8_Z");  }break;
// 				}
// 				m_cbRobotChoose.InsertString(i,strAxisName);
//			}
//		else{


				m_cbRobotChoose.InsertString(i, pItem->strAxisName);
//			}
		}


//		eAXIS_TEST_PP_1_X_PITCH


	}

	m_cbRobotChoose.SetCurSel(0);
	m_nAxisNo = 0;

	m_cbMotorSpeed.SetCurSel(0);
	m_nMotorSpeed = 0;

	GetDlgItem(IDC_BUTTON_ZAXIS_PICKER)->ShowWindow(FALSE);

	UpdateJogButtonName( 0 );


	SetTimer(1, DEF_SENSOR_DISP_TM, 0);

	g_DMCont.m_dmHandlerOpt.SB(BDM11_GUI_OPEN_HISTORY, TRUE);

	MakeLog("Servo Motor Setting Dialog Show");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgManuMotor::OnDestroy()
{
	CDialogEx::OnDestroy();

	g_DMCont.m_dmHandlerOpt.SB(BDM11_GUI_OPEN_HISTORY, FALSE);
	g_DMCont.m_dmEQP.SB(BDM0_OPEN_SLT_UI_DLG_HISTORY, FALSE);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	KillTimer(1);
	KillTimer(2);
	KillTimer(3);
}


void CDlgManuMotor::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if( nIDEvent == 1 ){
		KillTimer(nIDEvent);
		UpdateMotorStatus(m_nAxisNo);
		SetTimer(nIDEvent, DEF_SENSOR_DISP_TM, NULL);
	}

//	Interlock Chk는 V_Move에서 진행함.
		// plus jog
	if (nIDEvent == 2)
	{
		KillTimer(nIDEvent);
		int nRet = g_Motor[m_nAxisNo]->V_MoveCheckInterlock(100, 1);
		if (nRet != ERR_NO_ERROR)
			g_Motor[m_nAxisNo]->V_MoveStop();
		SetTimer(nIDEvent, 50, NULL);
	}
	// minus jog
	if (nIDEvent == 3)
	{
		KillTimer(nIDEvent);
		int nRet = g_Motor[m_nAxisNo]->V_MoveCheckInterlock(-100, 1);
		if (nRet != ERR_NO_ERROR)
			g_Motor[m_nAxisNo]->V_MoveStop();
		SetTimer(nIDEvent, 50, NULL);
 	}
	if (nIDEvent == 4)
	{
		KillTimer(nIDEvent);
		if (g_TaskSystemCtrl.GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Ready)
			SetDlgItemOriginInterLock(TRUE);
		else
			SetTimer(nIDEvent, 500, NULL);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgManuMotor::UpdateMotorStatus(int nAxisNo)
{
	//Position 단위, Cur Pos
	if (eAXIS_LOAD_TABLE_1_ROTATE <= nAxisNo && nAxisNo <= eAXIS_LOAD_TABLE_4_ROTATE) {
		GetDlgItem(IDC_SSPANEL_UNIT)->SetWindowText("degree");
	}
	else {
		GetDlgItem(IDC_SSPANEL_UNIT)->SetWindowText("mm");
	}
	

	// Cur Pos
	CString strCurPos;
	double dCurPos = 0.0;

	if (eAXIS_TRAY_PP_1_Z <= nAxisNo && nAxisNo <= eAXIS_TEST_PP_2_Z2) {
		int nBoardType = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);
		if (nBoardType == eBoardDir_Forward)
		{
			g_Motor[nAxisNo]->GetCurrentPos(dCurPos);
		}
		else
		{
			int nBBxIdx = 0;
//			switch (nAxisNo)
/*			{*/
				if (eAXIS_TRAY_PP_1_Z == nAxisNo)	nBBxIdx = eBMAxis_LOAD_TRAY_Picker01;	
				if (eAXIS_TEST_PP_1_Z == nAxisNo)	nBBxIdx = eBMAxis_LOAD_TESTPP_1_Picker08;
				if (eAXIS_TEST_PP_1_Z2 == nAxisNo)nBBxIdx = eBMAxis_UNLOAD_TESTPP_1_Picker08;
				if (eAXIS_TRAY_PP_2_Z == nAxisNo)	nBBxIdx = eBMAxis_UNLOAD_TRAY_Picker01;	
				if (eAXIS_TEST_PP_2_Z == nAxisNo)	nBBxIdx = eBMAxis_LOAD_TESTPP_2_Picker08;
				if (eAXIS_TEST_PP_2_Z2 == nAxisNo)nBBxIdx = eBMAxis_UNLOAD_TESTPP_2_Picker08;
// 			default:
// 				break;
// 			}
			
			g_Motor[nAxisNo]->GetCurrentPos(dCurPos, _eBbxmnpAxis(nBBxIdx));
		}


		//g_Motor[nAxisNo]->GetCurrenPosAvg(dCurPos);
	}
	else {
		g_Motor[nAxisNo]->GetCurrentPos(dCurPos);
	}

	//g_Motor[nAxisNo]->GetCurrentPos(dCurPos);
	if (eAXIS_LOAD_TABLE_1_ROTATE <= nAxisNo && nAxisNo <= eAXIS_LOAD_TABLE_4_ROTATE) {
		strCurPos.Format("%7.3f", (dCurPos));
	}
	else {
		strCurPos.Format("%7.3f", (dCurPos / 1000.0));
	}
	
	GetDlgItem(IDC_STATIC_LOCATION)->SetWindowText(strCurPos);

	// Cur Speed
	CString strCurSpd;
	strCurSpd.Format("%7.3f", (g_Motor[nAxisNo]->GetSpeed())/1000.0);
	GetDlgItem(IDC_STATIC_RPM)->SetWindowText(strCurSpd);

	// ORG Sensor
	if(g_Motor[nAxisNo]->IsHomeSensorOn()){
		m_ZrStat.On();
	}else{
		m_ZrStat.Off();
	}

	// Amp Fault
	if(g_Motor[nAxisNo]->IsAmpFault()){
		m_AmpFaultStat.On();
	}else{
		m_AmpFaultStat.Off();
	}

	// Servo On
	if(g_Motor[nAxisNo]->GetServoStat()){
		//m_Servo.SetCaption("Servo ON");
		SetDlgItemText(IDC_COLORBTCTRL_SERVO, "Servo ON");
		m_ServoStat.On();
	}else{
		//m_Servo.SetCaption("Servo OFF");
		SetDlgItemText(IDC_COLORBTCTRL_SERVO, "Servo OFF");
		m_ServoStat.Off();
	}

	// Motor Usable
	int nChk = g_Motor[nAxisNo]->ChkMotorUsable();
	if( nChk == ERR_NO_ERROR ){
		m_MotorUsableStat.On();
	}else{
		m_MotorUsableStat.Off();
	}

	//TRACE("Transfer L [Pre Sen : %d, Touch : %d] \n", g_IO.in(DI_TRANSFER_HEAD_LEFT_PRESTEP_SEN), g_IO.in(DI_TRANSFER_HEAD_LEFT_TOUCH_SEN));
}

void CDlgManuMotor::UpdateJogButtonName(int nAxisNo)
{
	_tMOTOR_PARAM* pItem = (_tMOTOR_PARAM*)m_listMotorParam.m_fnGetData(nAxisNo);
	int nDirType = pItem->eMotorCoordinate;
	if( nDirType == _tMOTOR_PARAM::Z || nDirType == _tMOTOR_PARAM::Picker_Z){
		m_ctrlPlus.SetCaption("UP");
		m_ctrlMinus.SetCaption("DOWN");
	}else if( nDirType == _tMOTOR_PARAM::Y){
		m_ctrlPlus.SetCaption("FORWARD");
		m_ctrlMinus.SetCaption("BACKWARD");
	}else if( nDirType == _tMOTOR_PARAM::X){
		m_ctrlPlus.SetCaption("RIGHT");
		m_ctrlMinus.SetCaption("LEFT");
	}
	else if (nDirType == _tMOTOR_PARAM::Rotator) {
		m_ctrlPlus.SetCaption("ROTATE +");
		m_ctrlMinus.SetCaption("ROTATE -");
	}else{ //Pitch
		m_ctrlPlus.SetCaption("WIDE");
		m_ctrlMinus.SetCaption("NARROW");
	}

	//RIGHT
	//LEFT
	//FORWARD
	//BACKWARD

	m_strSelectMotorName = pItem->strAxisName;
}



BEGIN_EVENTSINK_MAP(CDlgManuMotor, CDialogEx)
	ON_EVENT(CDlgManuMotor, IDC_COLORBTCTRL_SERVO, 22, CDlgManuMotor::ClickColorbtctrlServo, VTS_NONE)
	ON_EVENT(CDlgManuMotor, IDC_COLORBTCTRL_ZR, 22, CDlgManuMotor::ClickColorbtctrlZr, VTS_NONE)
	ON_EVENT(CDlgManuMotor, IDC_COLORBTCTRL_RESET, 22, CDlgManuMotor::ClickColorbtctrlReset, VTS_NONE)
	ON_EVENT(CDlgManuMotor, IDC_COLORBTCTRL_JOG_PLUS, DISPID_MOUSEDOWN, CDlgManuMotor::MouseDownColorbtctrlJogPlus, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuMotor, IDC_COLORBTCTRL_JOG_PLUS, DISPID_MOUSEUP, CDlgManuMotor::MouseUpColorbtctrlJogPlus, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuMotor, IDC_COLORBTCTRL_JOG_MINUS, DISPID_MOUSEDOWN, CDlgManuMotor::MouseDownColorbtctrlJogMinus, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuMotor, IDC_COLORBTCTRL_JOG_MINUS, DISPID_MOUSEUP, CDlgManuMotor::MouseUpColorbtctrlJogMinus, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgManuMotor, IDC_COLORBTCTRL_STOP, 22, CDlgManuMotor::ClickColorbtctrlStop, VTS_NONE)
END_EVENTSINK_MAP()


void CDlgManuMotor::ClickColorbtctrlServo()
{
	int nAxisNo = m_nAxisNo;
	if(g_Motor[nAxisNo]->GetServoStat()){
		g_Motor[nAxisNo]->ServoOff();
// 		//임시 Rotate 폭주 Interlock 추가.
// 		if (eAXIS_LOAD_TABLE_1_Y <= nAxisNo && nAxisNo <= eAXIS_LOAD_TABLE_4_Y) {
// 			g_Motor[nAxisNo - 4]->ServoOff();
// 		}

		MakeLog("%s Servo Off",m_strSelectMotorName);
	}else{

		if (g_IO.in(DI_SERVO_POWER_ON_SEN1) != DEF_ON)
		{
			CParamList AlarmData;
			CMyList rDiInfo;
			g_DB.SELECT_DIO_TBL(_tDIO_LS::eDI, DI_SERVO_POWER_ON_SEN1, 1, rDiInfo);
			if (rDiInfo.m_fnGetCount() > 0)
			{
				_tDIO_LS* pItem = (_tDIO_LS*)rDiInfo.m_fnGetData(0);
				if (pItem)
				{
					pItem->name.TrimLeft();
					pItem->name.TrimRight();
					AlarmData.m_fnAddItemFormat("%s = %s", pItem->name, (g_IO.in(DI_SERVO_POWER_ON_SEN1)) ? _T("ON") : _T("OFF"));
				}
			}
			g_Error.AlarmReport(ERR_CAN_NOT_COVER_LOCK, AlarmData, NULL);
			return;
		}

		g_Motor[nAxisNo]->ServoOn();

// 		//임시 Rotate 폭주 Interlock 추가.
// 		if (eAXIS_LOAD_TABLE_1_Y <= nAxisNo && nAxisNo <= eAXIS_LOAD_TABLE_4_Y) {
// 			BOOL bServoState = g_Motor[nAxisNo - 4]->GetServoStat();
// 			if(!bServoState)
// 				g_Motor[nAxisNo - 4]->ServoOn();
// 		}
// 
// 		if (eAXIS_LOAD_TABLE_1_ROTATE <= nAxisNo && nAxisNo <= eAXIS_LOAD_TABLE_4_ROTATE) {
// 			BOOL bServoState = g_Motor[nAxisNo + 4]->GetServoStat();
// 			if (!bServoState)
// 				g_Motor[nAxisNo + 4]->ServoOn();
// 		}

		Sleep(150);
		MakeLog("%s Servo On",m_strSelectMotorName);
	}
}

void CDlgManuMotor::SetDlgItemOriginInterLock(BOOL bStatus)
{
	GetDlgItem(IDOK)->EnableWindow(bStatus);
	GetDlgItem(IDC_COLORBTCTRL_JOG_MINUS)->EnableWindow(bStatus);
	GetDlgItem(IDC_COLORBTCTRL_JOG_PLUS)->EnableWindow(bStatus);	
	GetDlgItem(IDC_COLORBTCTRL_ZR)->EnableWindow(bStatus);
	GetDlgItem(IDC_COLORBTCTRL_RESET)->EnableWindow(bStatus);	
	GetDlgItem(IDC_COMBO_ROBOT_CHOOSE)->EnableWindow(bStatus);
}
void CDlgManuMotor::ClickColorbtctrlZr()
{
	int nAxisNo = m_nAxisNo;
	int nRes = g_Error.RequestUserSelectionModal(ERR_SYSMSG_DO_YOU_WANT_MOTOR_ORG, ERR_VAT_TYPE_YESNO);

	int nErrCode = 0;
	if (nRes == IDYES) {
		
		if((nAxisNo >= eAXIS_TRAY_PP_1_Z && nAxisNo <= eAXIS_TEST_PP_2_Z2) && g_IO.GetDriverType() != _DIO_TYPE_SIMUL_) {
			nErrCode = g_Motor[nAxisNo]->Origin();
			if (nErrCode != ERR_NO_ERROR) {
				CParamList AlarmData;
				g_Error.AlarmReport(nErrCode, AlarmData, NULL);
				return;
			}
		}
		else {
			SetDlgItemOriginInterLock(FALSE);

			if (nAxisNo >= eAXIS_PRESS_UNIT_DN1 && nAxisNo <= eAXIS_PRESS_UNIT_DN8)
			{
				int nInterlockErr = g_TaskSystemCtrl.CheckManualable(true);
				if (nInterlockErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
					return;
				}

				/////////////////////////
			}
			else
			{
				int nInterlockErr = g_TaskSystemCtrl.CheckManualable(false);
				if (nInterlockErr != ERR_NO_ERROR) {
					CParamList AlarmData;
					g_Error.AlarmReport(nInterlockErr, AlarmData, NULL);
					return;
				}
			}
			MakeLog("%s Origin Check", m_strSelectMotorName);
			g_TaskSystemCtrl.MoveInitial(nAxisNo);
			SetTimer(4, 500, NULL);
		}
	}
	else {
		return;
	}
}


void CDlgManuMotor::ClickColorbtctrlReset()
{
	int nAxisNo = m_nAxisNo;
	g_Motor[nAxisNo]->AlarmReset();
	Sleep(100);
	g_Motor[nAxisNo]->AlarmReset();
	MakeLog("%s Motor Reset",m_strSelectMotorName);
}


void CDlgManuMotor::MouseDownColorbtctrlJogPlus(short Button, short Shift, long X, long Y)
{
	int nErr = ComInterlock(m_nAxisNo);

	double dCurEncPos;
	g_Motor[m_nAxisNo]->GetCurrentPos(dCurEncPos);
	double dPosiLimitX = g_Motor[m_nAxisNo]->GetSwLimitPosi();
	double dNegLimitX = g_Motor[m_nAxisNo]->GetSwLimitNega();

	int nRet = 0;
	if (dCurEncPos/1000.0 < dNegLimitX) {
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Axis=%s", g_Motor[m_nAxisNo]->GetAxisName());
		g_Error.AlarmReport(ERR_MOT_INTERLOCK_IS_NEGA_SW_LMT, AlarmData, NULL);
		return;
	}

	if( nErr != ERR_NO_ERROR ){
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Axis=%s", g_Motor[m_nAxisNo]->GetAxisName());
		g_Error.AlarmReport(nErr, AlarmData, NULL);
		return;
	}

	m_ctrlPlus.SetBackColor(COLOR_LIGHTORANGE);

//	SetTimer(3, 50, NULL);
	int nAxisNo = m_nAxisNo;
	int nError[2] = { -1, };
//	g_Motor[nAxisNo]->V_Move(-m_dJogSpeed, 1);

	//방향 확인 필요.
	//Plus 방향 1, Positive Limit값 가져온다.
	//Minus 방향 -1 Negative Limit값 가져온다.
	//모터 방향 맞춰야함.
	if (nAxisNo >= eAXIS_TRAY_PP_1_Z && nAxisNo <= eAXIS_TEST_PP_2_Z2) {
		int nPicker[eBMAxis_Picker_MAX] = { 0, };
		int eMotor = g_pBbxmnpClient->GetZPickerMotorNumber(nAxisNo);
		int nCount = 0;
		for (int nIdx : g_pBbxmnpClient->m_vHandAxis[eMotor])
		{
			nPicker[nCount] = g_pBbxmnpClient->m_vHandMatrix[nIdx];
			nCount++;
		}
		nError[0] = g_Motor[nAxisNo]->V_Move(eJogDir_Negative, MAX_PICKER_CNT_EACH_SLAVE, &nPicker[0], m_dJogSpeed, 20);
		nError[1] = g_Motor[nAxisNo]->V_Move(eJogDir_Negative, MAX_PICKER_CNT_EACH_SLAVE, &nPicker[4], m_dJogSpeed, 20);

		for (int i = 0; i < _countof(nError); i++)
		{
			if (nError[i] != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Axis=%s", g_Motor[nAxisNo]->GetAxisName());
				g_Error.AlarmReport(nError[i], AlarmData, NULL);
				return;
			}
		}
	}
	else if ((eAXIS_PRESS_UNIT_DN1 <= nAxisNo && nAxisNo <= eAXIS_PRESS_UNIT_DN4) || (eAXIS_PRESS_UNIT_DN5 <= nAxisNo && nAxisNo <= eAXIS_PRESS_UNIT_DN8))
	{
		g_TaskSystemCtrl.MoveJog(nAxisNo, -1, m_dJogSpeed);
	}
	else
		g_TaskSystemCtrl.MoveJog(nAxisNo, 1, m_dJogSpeed);
// 	int nAlid = g_Motor[nAxisNo]->V_Move(1, m_dJogSpeed, 50);
// 	if (nAlid != ERR_NO_ERROR) {
// 		CParamList AlarmData;
// 		g_Error.AlarmReport(nAlid, AlarmData, NULL);
// 		return;
// 	}
	
}


void CDlgManuMotor::MouseUpColorbtctrlJogPlus(short Button, short Shift, long X, long Y)
{
	DWORD dwColor = GetSysColor(COLOR_BTNFACE);
	int nAxisNo = m_nAxisNo;
	m_ctrlPlus.SetBackColor(dwColor);
	g_Motor[nAxisNo]->V_MoveStop();
	KillTimer(3);
}


void CDlgManuMotor::MouseDownColorbtctrlJogMinus(short Button, short Shift, long X, long Y)
{
	double dCurEncPos;
	g_Motor[m_nAxisNo]->GetCurrentPos(dCurEncPos);
	double dPosiLimitX = g_Motor[m_nAxisNo]->GetSwLimitPosi();
	double dNegLimitX  = g_Motor[m_nAxisNo]->GetSwLimitNega();

	int nRet = 0;
	if (dCurEncPos/ 1000.0 > dPosiLimitX) {
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Axis=%s", g_Motor[m_nAxisNo]->GetAxisName());
		g_Error.AlarmReport(ERR_MOT_INTERLOCK_IS_POSI_SW_LMT, AlarmData, NULL);
		return;
	}

	int nErr = ComInterlock(m_nAxisNo);

	if( nErr != ERR_NO_ERROR ){
		CParamList AlarmData;
		AlarmData.m_fnAddItemFormat("Axis=%s", g_Motor[m_nAxisNo]->GetAxisName());
		g_Error.AlarmReport(nErr, AlarmData, NULL);
		return;
	}

	m_ctrlMinus.SetBackColor(COLOR_LIGHTORANGE);

//	SetTimer(2, 50, NULL);
	int nAxisNo = m_nAxisNo;
	int nError[2] = { -1, };
// Limit 확인용 Check
	if (nAxisNo >= eAXIS_TRAY_PP_1_Z && nAxisNo <= eAXIS_TEST_PP_2_Z2) {
		int nPicker[eBMAxis_Picker_MAX] = { 0, };
			int eMotor = g_pBbxmnpClient->GetZPickerMotorNumber(nAxisNo);
			int nCount = 0;
			for (int nIdx : g_pBbxmnpClient->m_vHandAxis[eMotor])
			{
				nPicker[nCount] = g_pBbxmnpClient->m_vHandMatrix[nIdx];
				nCount++;
			}
		nError[0] = g_Motor[nAxisNo]->V_Move(eJogDir_Positive, MAX_PICKER_CNT_EACH_SLAVE, &nPicker[0], m_dJogSpeed, 20);
		nError[1] = g_Motor[nAxisNo]->V_Move(eJogDir_Positive, MAX_PICKER_CNT_EACH_SLAVE, &nPicker[4], m_dJogSpeed, 20);
		for (int i = 0; i < _countof(nError); i++)
		{
			if (nError[i] != ERR_NO_ERROR) {
				CParamList AlarmData;
				AlarmData.m_fnAddItemFormat("Axis=%s", g_Motor[nAxisNo]->GetAxisName());
				g_Error.AlarmReport(nError[i], AlarmData, NULL);
				return;
			}
		}
	}
	else if ((eAXIS_PRESS_UNIT_DN1 <= nAxisNo && nAxisNo <= eAXIS_PRESS_UNIT_DN4) || (eAXIS_PRESS_UNIT_DN5 <= nAxisNo && nAxisNo <= eAXIS_PRESS_UNIT_DN8))
	{
		g_TaskSystemCtrl.MoveJog(nAxisNo, 1, m_dJogSpeed);
	}
	else
		g_TaskSystemCtrl.MoveJog(nAxisNo, -1, m_dJogSpeed);
// 	int nAlid = g_Motor[nAxisNo]->V_Move(-1, m_dJogSpeed, 50);
// 	if (nAlid != ERR_NO_ERROR) {
// 		CParamList AlarmData;
// 		g_Error.AlarmReport(nAlid, AlarmData, NULL);
// 		return;
// 	}
}


void CDlgManuMotor::MouseUpColorbtctrlJogMinus(short Button, short Shift, long X, long Y)
{
	KillTimer(2);

	DWORD dwColor = GetSysColor(COLOR_BTNFACE);
	int nAxisNo = m_nAxisNo;
	m_ctrlMinus.SetBackColor(dwColor);
	g_Motor[nAxisNo]->V_MoveStop();
	
}


void CDlgManuMotor::OnCbnSelchangeComboRobotChoose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nAxisNo = m_cbRobotChoose.GetCurSel();
	UpdateJogButtonName( m_nAxisNo );

	if (m_nAxisNo == eAXIS_TEST_PP_1_Z || m_nAxisNo == eAXIS_TEST_PP_1_Z2 ||
		m_nAxisNo == eAXIS_TEST_PP_2_Z || m_nAxisNo == eAXIS_TEST_PP_2_Z2 ||
		m_nAxisNo == eAXIS_TRAY_PP_1_Z || m_nAxisNo == eAXIS_TRAY_PP_2_Z)
	{
		GetDlgItem(IDC_BUTTON_ZAXIS_PICKER)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_ZAXIS_PICKER)->ShowWindow(FALSE);
	}

	//Rotate 속도 처리
	if (eAXIS_LOAD_TABLE_1_ROTATE <= m_nAxisNo && m_nAxisNo <= eAXIS_LOAD_TABLE_4_ROTATE) {
		m_dJogSpeed = 100;
	}
}


void CDlgManuMotor::OnCbnSelchangeComboMSpeed()
{
	m_nMotorSpeed = m_cbMotorSpeed.GetCurSel();

	if (eAXIS_LOAD_TABLE_1_ROTATE <= m_nAxisNo && m_nAxisNo <= eAXIS_LOAD_TABLE_4_ROTATE) {
		if (m_nMotorSpeed == 0) {
			m_dJogSpeed = 100;
		}
		else if (m_nMotorSpeed == 1) {
			m_dJogSpeed = 200;
		}
		else {
			m_dJogSpeed = 300;
		}
	}
	else if (eAXIS_TRAY_PP_1_Z <= m_nAxisNo && m_nAxisNo <= eAXIS_TEST_PP_2_Z2) {
		if (m_nMotorSpeed == 0) {
			m_dJogSpeed = 10;
		}
		else if (m_nMotorSpeed == 1) {
			m_dJogSpeed = 30;
		}
		else {
			m_dJogSpeed = 50;
		}
	}
	else if (eAXIS_TEST_PP_1_X_PITCH <= m_nAxisNo && m_nAxisNo <= eAXIS_TEST_PP_1_Y2_PITCH ||
		eAXIS_TRAY_PP_1_X_PITCH <= m_nAxisNo && m_nAxisNo <= eAXIS_TRAY_PP_1_Y_PITCH ||
		eAXIS_TRAY_PP_1_X_PITCH <= m_nAxisNo && m_nAxisNo <= eAXIS_TRAY_PP_1_Y_PITCH || 
		eAXIS_TRAY_PP_2_X_PITCH <= m_nAxisNo && m_nAxisNo <= eAXIS_TRAY_PP_2_X_PITCH ||
		eAXIS_TEST_PP_2_X_PITCH <= m_nAxisNo && m_nAxisNo <= eAXIS_TEST_PP_2_Y2_PITCH) {
		if (m_nMotorSpeed == 0) {
			m_dJogSpeed = JOG_SPD_LOW_UMPS;
		}
		else if (m_nMotorSpeed == 1) {
			m_dJogSpeed = JOG_SPD_MID_UMPS;
		}
		else {
			m_dJogSpeed = JOG_SPD_HIG_UMPS;
		}
	}
	else {
		if (m_nMotorSpeed == 0) {
			m_dJogSpeed = JOG_SPD_LOW_UMPS;
		}
		else if (m_nMotorSpeed == 1) {
			m_dJogSpeed = JOG_SPD_MID_UMPS*10;
		}
		else {
			m_dJogSpeed = JOG_SPD_HIG_UMPS*10;
		}
	}


}


void CDlgManuMotor::ClickColorbtctrlStop()
{
	g_TaskSystemCtrl.StopJog();
	SetDlgItemOriginInterLock(TRUE);
	KillTimer(2);
	KillTimer(3);
	MakeLog("Stop Button Click");
}


void CDlgManuMotor::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	switch(nState)

	{
	case WA_INACTIVE         : //(0) 포커스를 잃었을 때
		{
			int nAxisNo = m_nAxisNo;
			g_Motor[nAxisNo]->SetStop();
			g_Motor[nAxisNo]->OriginStop();
			g_Motor[nAxisNo]->V_MoveStop();
			KillTimer(2);
			KillTimer(3);
		}break;
	}


	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CDlgManuMotor::MakeLog(LPCTSTR fmt, ...)
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

int CDlgManuMotor::ComInterlock(int nAxisNo)
{
	int nOrgStat = g_Motor[nAxisNo]->IsOrgCompleted(nullptr);
	if (nOrgStat == FALSE)
	{
		//if(nAxisNo != 18 && nAxisNo != 21)	// test hand z축 제외
			return ERR_MOT_INTERLOCK_IS_NOT_ORG;
	}
		

	int nErr = ERR_NO_ERROR;
	nErr = g_TaskSystemCtrl.ChkComInterlock();

	return nErr;
}


void CDlgManuMotor::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CDlgManuMotor::OnBnClickedButtonZaxisPicker()
{
	CManuMotorZAxisDlg Do;
	Do.m_nAxisNo = m_nAxisNo;
	Do.DoModal();
}


