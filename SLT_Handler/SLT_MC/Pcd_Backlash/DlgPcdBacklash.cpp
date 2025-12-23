// DlgPcdBacklash.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgPcdBacklash.h"
#include "afxdialogex.h"
#include "DlgManuMotor.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDlgPcdBacklash 대화 상자입니다.
#define  DEF_BACKLASH_OFFSET		(100.0)
#define	 DEF_CAL_TESTPP_DISTANCE	(120000) //120mm
#define	 DEF_CAL_TRAYPP_DISTANCE	(100000) //100mm

IMPLEMENT_DYNAMIC(CDlgPcdBacklash, CDialogEx)

CDlgPcdBacklash::CDlgPcdBacklash(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PCDBACKLASH, pParent)
	, m_nRepeat(1)
{
	m_nBacklashAxis = 0;
	m_nPcdAxis = 0;

	g_DMCont.m_dmHandlerOpt.SB(BDM11_VAT_MODE, TRUE);
	m_nBacklashOffSet = DEF_BACKLASH_OFFSET;

	m_nAccuracyAxis = 0;

	m_bManualCmdExcute = FALSE;

	

}

CDlgPcdBacklash::~CDlgPcdBacklash()
{
	CBaseTaskManager::SendProcessCommand(base_seq_cmd::eProcCmd_Stop, eEqpCtrlMode_Manual, NULL);
	g_DMCont.m_dmHandlerOpt.SB(BDM11_VAT_MODE, FALSE);
}

void CDlgPcdBacklash::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_ROBOT_CHOOSE, m_cbRobotName);
	DDX_Control(pDX, IDC_CB_HAND_CHOOSE, m_cbHandName);
	
	DDX_Control(pDX, IDC_BACKLASH_SPD, m_cbBacklashSpd);
	DDX_Control(pDX, IDC_BACKLASH_CNT, m_cbBacklashCnt);
	DDX_Control(pDX, IDC_BACKLASH_DISTANCE, m_cbBacklashDis);

	DDX_Control(pDX, IDC_PCD_DISTANCE, m_cbPcdDis);

	DDX_Control(pDX, IDC_REPEATABILITY_CNT, m_cbRepeatabilityCnt);
	DDX_Control(pDX, IDC_REPEATABILITY_SPD, m_cbRepeatabilitySpd);
	
	DDX_Control(pDX, IDC_ACCURACY_CNT, m_cbAccuracyCnt);
	DDX_Control(pDX, IDC_ACCURACY_DISTANCE, m_cbAccuracyDis);
	DDX_Control(pDX, IDC_ACCURACY_SPD, m_cbAccuracySpd);
	DDX_Radio(pDX, IDC_ACCURACY_AXIS_X, (int&)m_nAccuracyAxis);

	DDX_Radio(pDX, IDC_PCD_AXIS_X, (int&)m_nPcdAxis);
	DDX_Radio(pDX, IDC_BACKLASH_AXIS_X, (int&)m_nBacklashAxis);
	DDX_Control(pDX, IDC_SPREAD_LOC_TABLE, m_SprdCurPos);
	DDX_Control(pDX, IDC_SPREAD_LOC_TABLE2, m_SprdBacklashOffset);
	DDX_Control(pDX, IDC_BTN_PCD_X_READ_ENCODER, m_btnPcdReadEncX);
	DDX_Control(pDX, IDC_BTN_PCD_X_SAVE_POS, m_btnPcdSavePosX);
	DDX_Control(pDX, IDC_BTN_PCD_Y_READ_ENCODER, m_btnPcdReadEncY);
	DDX_Control(pDX, IDC_BTN_PCD_Y_SAVE_POS, m_btnPcdSavePosY);
	DDX_Control(pDX, IDC_BTN_PCD_START, m_btnPcdStart);
	DDX_Control(pDX, IDC_BTN_PCD_SAVE_SCALE_FACTOR, m_btnSaveSF);
	DDX_Control(pDX, IDC_BTN_VISION_CONNECT, m_btnConnVision);
	DDX_Control(pDX, IDC_BTN_VISION_CONNECT2, m_btnConnVision2);
	DDX_Control(pDX, IDC_BTN_MANUAL_STOP, m_btnStop);
	DDX_Control(pDX, IDCANCEL, m_btnExit);
	DDX_Control(pDX, IDC_BTN_BACKLASH_READ_ENCODER, m_btnBacklashReadEnc);
	DDX_Control(pDX, IDC_BTN_SAVE_TARGET_POS, m_btnBacklashSavePos);
	DDX_Control(pDX, IDC_BTN_BACKLASH_MANUAL_START, m_btnBacklashManualStart);
	DDX_Control(pDX, IDC_BTN_BACKLASH_AUTO_START, m_btnBacklashAutoStart);
	DDX_Control(pDX, IDC_BTN_BACKLASH_AUTO_TEST, m_btnBacklashTest);
	DDX_Control(pDX, IDC_BTN_BACKLASH_SAVE, m_btnBacklashSave);
	DDX_Control(pDX, IDC_BTN_TEST_PP_SAFETY_Z, m_btnTestppSafetySave);
	DDX_Control(pDX, IDC_PCDBACKLAS_REPEAT, m_cbRepeat);
	DDX_Text(pDX, IDC_EDIT_BACKLASH_REPEAT, m_nRepeat);
	DDV_MinMaxInt(pDX, m_nRepeat, 1, 999);
	DDX_Control(pDX, IDC_BTN_PCDBACKLASH_SERVO, m_btnServo);
	DDX_Control(pDX, IDC_EDIT_VISION_IP, m_edit_vision_ip);
	DDX_Control(pDX, IDC_EDIT_VISION_PORT, m_edit_vision_port);
	DDX_Control(pDX, IDC_EDIT_VISION_IP2, m_edit_vision_ip2);
	DDX_Control(pDX, IDC_EDIT_VISION_PORT2, m_edit_vision_port2);

	DDX_Control(pDX, IDC_BUTTON_MANU_PROC_CMD_SKIP, m_btnSkip);
	DDX_Control(pDX, IDC_BUTTON_MANU_PROC_CMD_STOP, m_btn_Stop);

	DDX_Control(pDX, IDCANCEL3, m_btncancel);
}


BEGIN_MESSAGE_MAP(CDlgPcdBacklash, CDialogEx)
	ON_WM_TIMER()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_PCD_AXIS_X, IDC_PCD_AXIS_Y, OnRadioButtonPcdAxisChanged)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BACKLASH_AXIS_X, IDC_BACKLASH_AXIS_Y, OnRadioButtonBacklashAxisChanged)
	ON_CBN_SELCHANGE(IDC_CB_ROBOT_CHOOSE, &CDlgPcdBacklash::OnCbnSelchangeCbRobotChoose)
	ON_BN_CLICKED(IDC_BTN_MANUAL_STOP, &CDlgPcdBacklash::OnBnClickedBtnManualStop)
	ON_BN_CLICKED(IDCANCEL, &CDlgPcdBacklash::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_PCD_X_READ_ENCODER, &CDlgPcdBacklash::OnBnClickedBtnPcdXReadEncoder)
	ON_BN_CLICKED(IDC_BTN_PCD_X_SAVE_POS, &CDlgPcdBacklash::OnBnClickedBtnPcdXSavePos)
	ON_BN_CLICKED(IDC_BTN_PCD_Y_READ_ENCODER, &CDlgPcdBacklash::OnBnClickedBtnPcdYReadEncoder)
	ON_BN_CLICKED(IDC_BTN_PCD_Y_SAVE_POS, &CDlgPcdBacklash::OnBnClickedBtnPcdYSavePos)
	ON_BN_CLICKED(IDC_BTN_PCD_START, &CDlgPcdBacklash::OnBnClickedBtnPcdStart)
	ON_BN_CLICKED(IDC_BTN_PCD_SAVE_SCALE_FACTOR, &CDlgPcdBacklash::OnBnClickedBtnPcdSaveScaleFactor)
	ON_BN_CLICKED(IDC_BTN_SAVE_TARGET_POS, &CDlgPcdBacklash::OnBnClickedBtnSaveTargetPos)
	ON_BN_CLICKED(IDC_BTN_BACKLASH_READ_ENCODER, &CDlgPcdBacklash::OnBnClickedBtnBacklashReadEncoder)
	ON_BN_CLICKED(IDC_BTN_TEST_PP_SAFETY_Z, &CDlgPcdBacklash::OnBnClickedBtnTestPpSafetyZ)
	ON_EN_SETFOCUS(IDC_EDIT_PCD_XAxis_X, &CDlgPcdBacklash::OnEnSetfocusEditPcdXaxisX)
	ON_EN_SETFOCUS(IDC_EDIT_PCD_XAxis_Y, &CDlgPcdBacklash::OnEnSetfocusEditPcdXaxisY)
	ON_EN_SETFOCUS(IDC_EDIT_PCD_XAxis_Z, &CDlgPcdBacklash::OnEnSetfocusEditPcdXaxisZ)
	ON_EN_SETFOCUS(IDC_EDIT_PCD_YAxis_X, &CDlgPcdBacklash::OnEnSetfocusEditPcdYaxisX)
	ON_EN_SETFOCUS(IDC_EDIT_PCD_YAxis_Y, &CDlgPcdBacklash::OnEnSetfocusEditPcdYaxisY)
	ON_EN_SETFOCUS(IDC_EDIT_PCD_YAxis_Z, &CDlgPcdBacklash::OnEnSetfocusEditPcdYaxisZ)
	ON_EN_SETFOCUS(IDC_EDIT_SAFETY_UPPER, &CDlgPcdBacklash::OnEnSetfocusEditSafetyUpper)
	ON_EN_SETFOCUS(IDC_EDIT_SAFETY_LOWER, &CDlgPcdBacklash::OnEnSetfocusEditSafetyLower)
	ON_EN_SETFOCUS(IDC_EDIT_BACKLASH_X, &CDlgPcdBacklash::OnEnSetfocusEditBacklashX)
	ON_EN_SETFOCUS(IDC_EDIT_BACKLASH_Y, &CDlgPcdBacklash::OnEnSetfocusEditBacklashY)
	ON_EN_SETFOCUS(IDC_EDIT_BACKLASH_Z, &CDlgPcdBacklash::OnEnSetfocusEditBacklashZ)
	ON_BN_CLICKED(IDC_PCDBACKLAS_REPEAT, &CDlgPcdBacklash::OnBnClickedPcdbacklasRepeat)
	ON_BN_CLICKED(IDC_BTN_BACKLASH_MANUAL_START, &CDlgPcdBacklash::OnBnClickedBtnBacklashManualStart)
	ON_BN_CLICKED(IDC_BTN_BACKLASH_AUTO_START, &CDlgPcdBacklash::OnBnClickedBtnBacklashAutoStart)
	ON_EN_SETFOCUS(IDC_EDIT_BACKLASH_REPEAT, &CDlgPcdBacklash::OnEnSetfocusEditBacklashRepeat)
	ON_BN_CLICKED(IDC_BTN_BACKLASH_AUTO_TEST, &CDlgPcdBacklash::OnBnClickedBtnBacklashAutoTest)
	ON_BN_CLICKED(IDC_BTN_BACKLASH_SAVE, &CDlgPcdBacklash::OnBnClickedBtnBacklashSave)
	ON_BN_CLICKED(IDC_BTN_VISION_CONNECT, &CDlgPcdBacklash::OnBnClickedBtnVisionConnect)
	ON_BN_CLICKED(IDC_BTN_PCDBACKLASH_SERVO, &CDlgPcdBacklash::OnBnClickedBtnPcdbacklashServo)
	ON_BN_CLICKED(IDC_BTN_ACCURACY_READ_ENCODER, &CDlgPcdBacklash::OnBnClickedBtnAccuracyReadEncoder)
	ON_BN_CLICKED(IDC_BTN_REPEATABILITY_READ_ENCODER, &CDlgPcdBacklash::OnBnClickedBtnRepeatabilityReadEncoder)
	ON_BN_CLICKED(IDC_BTN_SAVE_REPEATABILITY_TARGET_POS, &CDlgPcdBacklash::OnBnClickedBtnSaveRepeatabilityTargetPos)
	ON_BN_CLICKED(IDC_BTN_SAVE_ACCURACY_TARGET_POS, &CDlgPcdBacklash::OnBnClickedBtnSaveAccuracyTargetPos)
	ON_EN_SETFOCUS(IDC_EDIT_REPEATABILITY_X, &CDlgPcdBacklash::OnEnSetfocusEditRepeatabilityX)
	ON_EN_SETFOCUS(IDC_EDIT_REPEATABILITY_Y, &CDlgPcdBacklash::OnEnSetfocusEditRepeatabilityY)
	ON_EN_SETFOCUS(IDC_EDIT_REPEATABILITY_Z, &CDlgPcdBacklash::OnEnSetfocusEditRepeatabilityZ)
	ON_EN_SETFOCUS(IDC_EDIT_ACCURACY_X, &CDlgPcdBacklash::OnEnSetfocusEditAccuracyX)
	ON_EN_SETFOCUS(IDC_EDIT_ACCURACY_Y, &CDlgPcdBacklash::OnEnSetfocusEditAccuracyY)
	ON_EN_SETFOCUS(IDC_EDIT_ACCURACY_Z, &CDlgPcdBacklash::OnEnSetfocusEditAccuracyZ)
	ON_BN_CLICKED(IDC_BTN_REPEATABILITY_START, &CDlgPcdBacklash::OnBnClickedBtnRepeatabilityStart)
	ON_BN_CLICKED(IDC_BTN_REPEATABILITY_ALL_START, &CDlgPcdBacklash::OnBnClickedBtnRepeatabilityAllStart)
	ON_BN_CLICKED(IDC_BTN_ACCURACY_START, &CDlgPcdBacklash::OnBnClickedBtnAccuracyStart)
	ON_CBN_SELCHANGE(IDC_BACKLASH_DISTANCE, &CDlgPcdBacklash::OnCbnSelchangeBacklashDistance)
	ON_CBN_SELCHANGE(IDC_BACKLASH_CNT, &CDlgPcdBacklash::OnCbnSelchangeBacklashCnt)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_CB_HAND_CHOOSE, &CDlgPcdBacklash::OnCbnSelchangeCbHandChoose)
	ON_BN_CLICKED(IDC_BTN_VISION_CONNECT2, &CDlgPcdBacklash::OnBnClickedBtnVisionConnect2)
	ON_BN_CLICKED(IDC_BUTTON_MANU_PROC_CMD_SKIP, &CDlgPcdBacklash::OnBnClickedButtonManuProcCmdSkip)
	ON_BN_CLICKED(IDC_BUTTON_MANU_PROC_CMD_STOP, &CDlgPcdBacklash::OnBnClickedButtonManuProcCmdStop)
	ON_BN_CLICKED(IDCANCEL3, &CDlgPcdBacklash::OnBnClickedCancel3)
END_MESSAGE_MAP()


// CDlgPcdBacklash 메시지 처리기입니다.


void CDlgPcdBacklash::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	BOOL bBusySts = FALSE;
	BOOL bAlarmSts = FALSE;
	int nCntBusy = 0;
	int nCntAlarm = 0;

	switch (nIDEvent)
	{
		case 0:
			{
				KillTimer(nIDEvent);

				DisplayCurrentMotorPos(); //by kmk [20190225] Selected Motor Axis Current Pos.
				UpdateVisionConnectStatus();
				HandSelectUIShowHide(TRUE);
				UpdateOffsetData();

				CTaskTrayBasePP* pTaskTrayPP[] = { &g_TaskTrayLoadPP, &g_TaskTrayUnloadPP };
				for (int nIdx = 0; nIdx < eMaxTrayPPCount; nIdx++) {
					if (pTaskTrayPP[nIdx]->GetSeqCmdStatus(eEqpCtrlMode_Manual) != CBaseSeqCmd::eSeqCmdStatus_Ready) {
						nCntBusy++;
					}
					else if (pTaskTrayPP[nIdx]->GetSeqCmdStatus(eEqpCtrlMode_Manual) == CBaseSeqCmd::eSeqCmdStatus_Alarm) {
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

				if (nCntBusy > 0) {
					bBusySts = TRUE;
				}
				if (nCntAlarm > 0) {
					bAlarmSts = TRUE;
				}

				CDSSPanel*    pSensor = NULL;
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

				SetTimer(nIDEvent, 500, NULL);

			}break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CDlgPcdBacklash::OnRadioButtonPcdAxisChanged(UINT ID)
{
	UpdateData(TRUE);

	int nSelection = m_cbRobotName.GetCurSel();

	int nAxis = ID == IDC_PCD_AXIS_X ? eAxisX : eAxisY;

	switch (nSelection)
	{
	case ePcdBacklash_TrayLoadPP:
	{
		if (nAxis == eAxisX){
			m_cbPcdDis.SetCurSel(9);
		}
	}
	break;
	case ePcdBacklash_TrayUnloadPP:
	{
		if (nAxis == eAxisX) {
			m_cbPcdDis.SetCurSel(9);
		}
		else {
			m_cbPcdDis.SetCurSel(14);
		}
	}
	break;
	case ePcdBacklash_TestPP1:
	case ePcdBacklash_TestPP2:
	{
		if (nAxis == eAxisX) {
			m_cbPcdDis.SetCurSel(14);
		}
		else {
			m_cbPcdDis.SetCurSel(9);
		}

		int nHandSelection = m_cbHandName.GetCurSel();
		if (nHandSelection == eTestPPUnLoadType)
		{
			CButton* pButton = (CButton*)GetDlgItem(IDC_PCD_AXIS_X);
			pButton->SetCheck(true);
			m_nPcdAxis = 0;
			UpdateData(FALSE);
		}
	}
	break;
	default:
		break;
	}
}

void CDlgPcdBacklash::OnRadioButtonBacklashAxisChanged(UINT ID)
{
	UpdateData(TRUE);
	int nAxis = ID == IDC_BACKLASH_AXIS_X ? eAxisX : eAxisY;

	ChangedBacklashPosition(nAxis);
}


BOOL CDlgPcdBacklash::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_vRobotNo[ePcdBacklash_TrayLoadPP].push_back(eAXIS_TRAY_PP_1_X);
	m_vRobotNo[ePcdBacklash_TrayLoadPP].push_back(eAXIS_TRAY_FEEDER_Y);
	m_vRobotNo[ePcdBacklash_TrayLoadPP].push_back(eAXIS_TRAY_PP_1_Z);

	m_vRobotNo[ePcdBacklash_TrayUnloadPP].push_back(eAXIS_TRAY_PP_2_X);
	m_vRobotNo[ePcdBacklash_TrayUnloadPP].push_back(eAXIS_TRAY_PP_2_Y);
	m_vRobotNo[ePcdBacklash_TrayUnloadPP].push_back(eAXIS_TRAY_PP_2_Z);

	m_vRobotNo[ePcdBacklash_TestPP1].push_back(eAXIS_TEST_PP_1_X);
	m_vRobotNo[ePcdBacklash_TestPP1].push_back(eAXIS_TEST_PP_1_Y);
	m_vRobotNo[ePcdBacklash_TestPP1].push_back(eAXIS_TRAY_PP_1_Z);
	m_vRobotNo[ePcdBacklash_TestPP1].push_back(eAXIS_TEST_PP_1_X2);
	m_vRobotNo[ePcdBacklash_TestPP1].push_back(eAXIS_TEST_PP_1_Z2);


	m_vRobotNo[ePcdBacklash_TestPP2].push_back(eAXIS_TEST_PP_2_X);
	m_vRobotNo[ePcdBacklash_TestPP2].push_back(eAXIS_TEST_PP_2_Y);
	m_vRobotNo[ePcdBacklash_TestPP2].push_back(eAXIS_TRAY_PP_2_Z);
	m_vRobotNo[ePcdBacklash_TestPP2].push_back(eAXIS_TEST_PP_2_X2);
	m_vRobotNo[ePcdBacklash_TestPP2].push_back(eAXIS_TEST_PP_2_Z2);
	
	m_cbRobotName.AddString(_T("TRAY LOAD PP"));
	m_cbRobotName.AddString(_T("TRAY UNLOAD PP"));
	m_cbRobotName.AddString(_T("TEST PP#1"));
	m_cbRobotName.AddString(_T("TEST PP#2"));

	m_cbHandName.AddString(_T("TEST LOAD PP"));
	m_cbHandName.AddString(_T("TEST UNLOAD PP"));

	m_cBmpStop.LoadBitmap(IDB_STOP);
	m_btn_Stop.SetBitmap(m_cBmpStop);

	m_cBmpSkip.LoadBitmap(IDB_SKIP);
	m_btnSkip.SetBitmap(m_cBmpSkip);

	g_VatBacklash.Load();
	g_VatPcd.Load();
	InitControls();

	UpdateVisionConnectStatus();
	UpdateTeachingData();
	UpdateOffsetData();
	UpdateTestPpSafetyData();

	int nAxis = (m_nPcdAxis == eAxisX) ? IDC_PCD_AXIS_X : IDC_PCD_AXIS_Y;
	OnRadioButtonPcdAxisChanged(nAxis);
	SpreadSheetUIShowHide(TRUE);

	for (int i = 0; i < _eTestPPIdx::eMaxTestPPCount; i++)
	{
		for (int h = 0; h < eMaxTestPPTypeCount; h++)
		{
		//double dSafetyUpPosZ = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(i), LOC_IDX_STATION_UP_1, POS_IDX_TEST_PP_Z_SAFETY);
		double dSafetyDownPosZ = g_DMCont.m_Teach.GetTeachPos(RID_TEST_PP_(i), LOC_IDX_STATION_1, POS_IDX_TEST_PP_Z1_SAFETY);

		//g_VatPcd.testpp[i].m_dUpperSafetyPos = (dSafetyUpPosZ /1000.0);
		g_VatPcd.testpp[i][h].m_dLowerSafetyPos = (dSafetyDownPosZ / 1000.0);
		g_VatPcd.testpp[i][h].SafetyZAxisSave();
		}
	}
	
	SetSpreaddisableBackColor();
	

	OnBtnGui();
	SetTimer(0, 500, NULL);

	return TRUE;
}

void CDlgPcdBacklash::InitControls()
{
	GetDlgItem(IDC_EDIT_VISION_IP)->SetWindowTextA("192.168.1.60");
	GetDlgItem(IDC_EDIT_VISION_PORT)->SetWindowTextA("8080");
	GetDlgItem(IDC_EDIT_VISION_IP2)->SetWindowTextA("192.168.1.61");
	GetDlgItem(IDC_EDIT_VISION_PORT2)->SetWindowTextA("9091");
	CString str;

	//반복 횟수
	for (int i = 0; i < 10; i++) {
		str.Format(_T("%d"), i + 1);
		m_cbBacklashCnt.AddString(str);
		m_cbRepeatabilityCnt.AddString(str);
		m_cbAccuracyCnt.AddString(str);
	}

	//Backlash Distance
	for (int i = 0; i < 20; i++) {
		str.Format(_T("%d"), (i + 1) * 10);
		m_cbBacklashDis.AddString(str);
		m_cbPcdDis.AddString(str);
	}

	//PCD, Accuracy Distance
	for (int i = 0; i < 40; i++) {
		str.Format(_T("%d"), (i + 1) * 10);
		m_cbAccuracyDis.AddString(str);
	}


	m_cbRobotName.SetCurSel(0);
	m_cbHandName.SetCurSel(0);
	m_cbBacklashSpd.SetCurSel(0);
	m_cbBacklashCnt.SetCurSel(0);
	m_cbBacklashDis.SetCurSel(0);

	m_cbPcdDis.SetCurSel(0);

	m_cbRepeatabilityCnt.SetCurSel(0);
	m_cbRepeatabilitySpd.SetCurSel(0);
	
	m_cbAccuracyCnt.SetCurSel(0);
	m_cbAccuracyDis.SetCurSel(0);
	m_cbAccuracySpd.SetCurSel(0);

	m_nPcdAxis = 0;
	m_nBacklashAxis = 0;

	m_nAccuracyAxis = 0;

	g_VatPcd.Load();
	g_VatBacklash.Load();
	//g_VatRepeatability.Load();
	//g_VatAccuracy.Load();

	HandSelectUIShowHide();
	//UnitSelectUIShowHide();

	UpdateData(FALSE);
}

void CDlgPcdBacklash::DisplayCurrentMotorPos()
{
	CString strCurPos = _T("");
	double dCurPosX =0.0, dCurPosY = 0.0, dCurPosZ = 0.0;
	int nSelection = m_cbRobotName.GetCurSel();
	int nHandSelection = m_cbHandName.GetCurSel();
	int nTestPPAxisX = nHandSelection == eTestPPLoadType ? eAxisX : eAxisX2;
	int nTestPPAxisZ = nHandSelection == eTestPPLoadType ? eAxisZ : eAxisZ2;
	switch (nSelection)
	{
		case ePcdBacklash_TrayLoadPP:
		{
			g_Motor[m_vRobotNo[nSelection][eAxisX]]->GetCurrentPos(dCurPosX);
			g_Motor[m_vRobotNo[nSelection][eAxisY]]->GetCurrentPos(dCurPosY);
			g_Motor[m_vRobotNo[nSelection][eAxisZ]]->GetCurrentPos(dCurPosZ);

		}break;
		case ePcdBacklash_TrayUnloadPP:
		{
			g_Motor[m_vRobotNo[nSelection][eAxisX]]->GetCurrentPos(dCurPosX);
			g_Motor[m_vRobotNo[nSelection][eAxisY]]->GetCurrentPos(dCurPosY);
			g_Motor[m_vRobotNo[nSelection][eAxisZ]]->GetCurrentPos(dCurPosZ);
		}break;
		case ePcdBacklash_TestPP1:
		case ePcdBacklash_TestPP2:
		{
			g_Motor[m_vRobotNo[nSelection][nTestPPAxisX]]->GetCurrentPos(dCurPosX);
			g_Motor[m_vRobotNo[nSelection][eAxisY]]->GetCurrentPos(dCurPosY);	
			g_Motor[m_vRobotNo[nSelection][nTestPPAxisZ]]->GetCurrentPos(dCurPosZ);

		}break;
	}
	
	strCurPos.Format("%.3f", dCurPosX/1000);
	m_SprdCurPos.SetTextMatrix(1, 1, strCurPos);
	strCurPos.Format("%.3f", dCurPosY/1000);
	m_SprdCurPos.SetTextMatrix(2, 1, strCurPos);
	strCurPos.Format("%.3f", dCurPosZ/1000);
	m_SprdCurPos.SetTextMatrix(3, 1, strCurPos);
	
}

void CDlgPcdBacklash::UpdateTeachingData()
{
	int nSelection = m_cbRobotName.GetCurSel();
	int nHandSelection = m_cbHandName.GetCurSel();
	int nTestPPAxisX = nHandSelection == eTestPPLoadType ? eAxisX : eAxisX2;
	int nTestPPAxisZ = nHandSelection == eTestPPLoadType ? eAxisZ : eAxisZ2;

	switch (nSelection) {
		case ePcdBacklash_TrayLoadPP:
			{
				CString strOffset = _T("");
				strOffset.Format("%.3f", g_VatPcd.trayloadpp.m_PcdStartXPos_XYZ.x / 1000.0);
				GetDlgItem(IDC_EDIT_PCD_XAxis_X)->SetWindowText(strOffset);
				strOffset.Format("%.3f", g_VatPcd.trayloadpp.m_PcdStartXPos_XYZ.y / 1000.0);
				GetDlgItem(IDC_EDIT_PCD_XAxis_Y)->SetWindowText(strOffset);
				strOffset.Format("%.3f", g_VatPcd.trayloadpp.m_PcdStartXPos_XYZ.z / 1000.0);
				GetDlgItem(IDC_EDIT_PCD_XAxis_Z)->SetWindowText(strOffset);

				strOffset.Format("%.3f", g_VatPcd.trayloadpp.m_PcdStartYPos_XYZ.x / 1000.0);
				GetDlgItem(IDC_EDIT_PCD_YAxis_X)->SetWindowText(strOffset);
				strOffset.Format("%.3f", g_VatPcd.trayloadpp.m_PcdStartYPos_XYZ.y / 1000.0);
				GetDlgItem(IDC_EDIT_PCD_YAxis_Y)->SetWindowText(strOffset);
				strOffset.Format("%.3f", g_VatPcd.trayloadpp.m_PcdStartYPos_XYZ.z / 1000.0);
				GetDlgItem(IDC_EDIT_PCD_YAxis_Z)->SetWindowText(strOffset);

				strOffset.Format("%.3f", g_VatBacklash.trayloadpp.m_BacklashTargetPos_XYZ.x / 1000.0);
				GetDlgItem(IDC_EDIT_BACKLASH_X)->SetWindowText(strOffset);
				strOffset.Format("%.3f", g_VatBacklash.trayloadpp.m_BacklashTargetPos_XYZ.y / 1000.0);
				GetDlgItem(IDC_EDIT_BACKLASH_Y)->SetWindowText(strOffset);
				strOffset.Format("%.3f", g_VatBacklash.trayloadpp.m_BacklashTargetPos_XYZ.z / 1000.0);
				GetDlgItem(IDC_EDIT_BACKLASH_Z)->SetWindowText(strOffset);

// 
// 				strOffset.Format("%.3f", g_VatRepeatability.trayloadpp.m_RepeatabilityStartPos_XZ.x / 1000.0);
// 				GetDlgItem(IDC_EDIT_REPEATABILITY_X)->SetWindowText(strOffset);
// 				strOffset.Format("%.3f", g_VatRepeatability.trayloadpp.m_RepeatabilityStartPos_XZ.z / 1000.0);
// 				GetDlgItem(IDC_EDIT_REPEATABILITY_Z)->SetWindowText(strOffset);
// 
// 				strOffset.Format("%.3f", g_VatAccuracy.trayloadpp.m_AccuracyStartPos_XZ.x / 1000.0);
// 				GetDlgItem(IDC_EDIT_ACCURACY_X)->SetWindowText(strOffset);
// 				strOffset.Format("%.3f", g_VatAccuracy.trayloadpp.m_AccuracyStartPos_XZ.z / 1000.0);
// 				GetDlgItem(IDC_EDIT_ACCURACY_Z)->SetWindowText(strOffset);


			}break;
		case ePcdBacklash_TrayUnloadPP:
		{
			CString strOffset = _T("");
			strOffset.Format("%.3f", g_VatPcd.trayunloadpp.m_PcdStartXPos_XYZ.x / 1000.0);
			GetDlgItem(IDC_EDIT_PCD_XAxis_X)->SetWindowText(strOffset);
			strOffset.Format("%.3f", g_VatPcd.trayunloadpp.m_PcdStartXPos_XYZ.y / 1000.0);
			GetDlgItem(IDC_EDIT_PCD_XAxis_Y)->SetWindowText(strOffset);
			strOffset.Format("%.3f", g_VatPcd.trayunloadpp.m_PcdStartXPos_XYZ.z / 1000.0);
			GetDlgItem(IDC_EDIT_PCD_XAxis_Z)->SetWindowText(strOffset);

			strOffset.Format("%.3f", g_VatPcd.trayunloadpp.m_PcdStartYPos_XYZ.x / 1000.0);
			GetDlgItem(IDC_EDIT_PCD_YAxis_X)->SetWindowText(strOffset);
			strOffset.Format("%.3f", g_VatPcd.trayunloadpp.m_PcdStartYPos_XYZ.y / 1000.0);
			GetDlgItem(IDC_EDIT_PCD_YAxis_Y)->SetWindowText(strOffset);
			strOffset.Format("%.3f", g_VatPcd.trayunloadpp.m_PcdStartYPos_XYZ.z / 1000.0);
			GetDlgItem(IDC_EDIT_PCD_YAxis_Z)->SetWindowText(strOffset);

			strOffset.Format("%.3f", g_VatBacklash.trayunloadpp.m_BacklashTargetPos_XYZ.x / 1000.0);
			GetDlgItem(IDC_EDIT_BACKLASH_X)->SetWindowText(strOffset);
			strOffset.Format("%.3f", g_VatBacklash.trayunloadpp.m_BacklashTargetPos_XYZ.y / 1000.0);
			GetDlgItem(IDC_EDIT_BACKLASH_Y)->SetWindowText(strOffset);
			strOffset.Format("%.3f", g_VatBacklash.trayunloadpp.m_BacklashTargetPos_XYZ.z / 1000.0);
			GetDlgItem(IDC_EDIT_BACKLASH_Z)->SetWindowText(strOffset);

// 
// 			strOffset.Format("%.3f", g_VatRepeatability.trayunloadpp.m_RepeatabilityStartPos_XYZ.x / 1000.0);
// 			GetDlgItem(IDC_EDIT_REPEATABILITY_X)->SetWindowText(strOffset);
// 			strOffset.Format("%.3f", g_VatRepeatability.trayunloadpp.m_RepeatabilityStartPos_XYZ.y / 1000.0);
// 			GetDlgItem(IDC_EDIT_REPEATABILITY_Y)->SetWindowText(strOffset);
// 			strOffset.Format("%.3f", g_VatRepeatability.trayunloadpp.m_RepeatabilityStartPos_XYZ.z / 1000.0);
// 			GetDlgItem(IDC_EDIT_REPEATABILITY_Z)->SetWindowText(strOffset);
// 
// 			strOffset.Format("%.3f", g_VatAccuracy.trayunloadpp.m_AccuracyStartPos_XYZ.x / 1000.0);
// 			GetDlgItem(IDC_EDIT_ACCURACY_X)->SetWindowText(strOffset);
// 			strOffset.Format("%.3f", g_VatAccuracy.trayunloadpp.m_AccuracyStartPos_XYZ.y / 1000.0);
// 			GetDlgItem(IDC_EDIT_ACCURACY_Y)->SetWindowText(strOffset);
// 			strOffset.Format("%.3f", g_VatAccuracy.trayunloadpp.m_AccuracyStartPos_XYZ.z / 1000.0);
// 			GetDlgItem(IDC_EDIT_ACCURACY_Z)->SetWindowText(strOffset);


		}break;
		case ePcdBacklash_TestPP1:
		case ePcdBacklash_TestPP2:
			{
				CString strOffset = _T("");
				strOffset.Format("%.3f", g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_PcdStartXPos_XYZ.x / 1000.0);
				GetDlgItem(IDC_EDIT_PCD_XAxis_X)->SetWindowText(strOffset);
				strOffset.Format("%.3f", g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_PcdStartXPos_XYZ.y / 1000.0);
				GetDlgItem(IDC_EDIT_PCD_XAxis_Y)->SetWindowText(strOffset);
				strOffset.Format("%.3f", g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_PcdStartXPos_XYZ.z / 1000.0);
				GetDlgItem(IDC_EDIT_PCD_XAxis_Z)->SetWindowText(strOffset);

				strOffset.Format("%.3f", g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_PcdStartYPos_XYZ.x / 1000.0);
				GetDlgItem(IDC_EDIT_PCD_YAxis_X)->SetWindowText(strOffset);
				strOffset.Format("%.3f", g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_PcdStartYPos_XYZ.y / 1000.0);
				GetDlgItem(IDC_EDIT_PCD_YAxis_Y)->SetWindowText(strOffset);
				strOffset.Format("%.3f", g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_PcdStartYPos_XYZ.z / 1000.0);
				GetDlgItem(IDC_EDIT_PCD_YAxis_Z)->SetWindowText(strOffset);

				strOffset.Format("%.3f", g_VatBacklash.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_BacklashTargetPos_XYZ.x / 1000.0);
				GetDlgItem(IDC_EDIT_BACKLASH_X)->SetWindowText(strOffset);
				strOffset.Format("%.3f", g_VatBacklash.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_BacklashTargetPos_XYZ.y / 1000.0);
				GetDlgItem(IDC_EDIT_BACKLASH_Y)->SetWindowText(strOffset);
				strOffset.Format("%.3f", g_VatBacklash.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_BacklashTargetPos_XYZ.z / 1000.0);
				GetDlgItem(IDC_EDIT_BACKLASH_Z)->SetWindowText(strOffset);

// 				strOffset.Format("%.3f", g_VatRepeatability.testpp[nSelection - 1].m_RepeatabilityStartPos_XYZ.x / 1000.0);
// 				GetDlgItem(IDC_EDIT_REPEATABILITY_X)->SetWindowText(strOffset);
// 				strOffset.Format("%.3f", g_VatRepeatability.testpp[nSelection - 1].m_RepeatabilityStartPos_XYZ.y / 1000.0);
// 				GetDlgItem(IDC_EDIT_REPEATABILITY_Y)->SetWindowText(strOffset);
// 				strOffset.Format("%.3f", g_VatRepeatability.testpp[nSelection - 1].m_RepeatabilityStartPos_XYZ.z / 1000.0);
// 				GetDlgItem(IDC_EDIT_REPEATABILITY_Z)->SetWindowText(strOffset);
// 
// 				strOffset.Format("%.3f", g_VatAccuracy.testpp[nSelection - 1].m_AccuracyStartPos_XYZ.x / 1000.0);
// 				GetDlgItem(IDC_EDIT_ACCURACY_X)->SetWindowText(strOffset);
// 				strOffset.Format("%.3f", g_VatAccuracy.testpp[nSelection - 1].m_AccuracyStartPos_XYZ.y / 1000.0);
// 				GetDlgItem(IDC_EDIT_ACCURACY_Y)->SetWindowText(strOffset);
// 				strOffset.Format("%.3f", g_VatAccuracy.testpp[nSelection - 1].m_AccuracyStartPos_XYZ.z / 1000.0);
// 				GetDlgItem(IDC_EDIT_ACCURACY_Z)->SetWindowText(strOffset);

			}break;
	}
	ChangedBacklashPosition(m_nBacklashAxis);
}

void CDlgPcdBacklash::UpdateOffsetData()
{
	int nSelection = m_cbRobotName.GetCurSel();
	int nHandSelection = m_cbHandName.GetCurSel();
	int nTestPPAxisX = nHandSelection == eTestPPLoadType ? eAxisX : eAxisX2;
	int nTestPPAxisZ = nHandSelection == eTestPPLoadType ? eAxisZ : eAxisZ2;

	switch (nSelection) {
		case ePcdBacklash_TrayLoadPP:
			{
				CString strOffset = _T("");

				strOffset.Format("%.6f", g_Motor[m_vRobotNo[nSelection][eAxisX]]->GetScaleFactor());
				GetDlgItem(IDC_EDIT_CURRENT_SF_X)->SetWindowText(strOffset);
				strOffset.Format("%.6f", g_Motor[m_vRobotNo[nSelection][eAxisY]]->GetScaleFactor());
				GetDlgItem(IDC_EDIT_CURRENT_SF_Y)->SetWindowText(strOffset);

				strOffset.Format("%.6f", g_VatPcd.trayloadpp.m_PcdOffset_XY.x /*/ 1000.0*/);
				GetDlgItem(IDC_EDIT_OFFSET_SF_X)->SetWindowText(strOffset);
				strOffset.Format("%.6f", g_VatPcd.trayloadpp.m_PcdOffset_XY.y /*/ 1000.0*/);
				GetDlgItem(IDC_EDIT_OFFSET_SF_Y)->SetWindowText(strOffset);

				for (int i = eSpeed_5; i < eSpeed_MAX; i++) {
					strOffset.Format("%.3f", g_VatBacklash.trayloadpp.m_BacklashOffset_XY[i].x / 1000.0);
					m_SprdBacklashOffset.SetTextMatrix(1, i + 1, strOffset);
					strOffset.Format("%.3f", g_VatBacklash.trayloadpp.m_BacklashOffset_XY[i].y / 1000.0);
					m_SprdBacklashOffset.SetTextMatrix(2, i + 1, strOffset);
				}

			}break;
		case ePcdBacklash_TrayUnloadPP:
		{
			CString strOffset = _T("");

			strOffset.Format("%.6f", g_Motor[m_vRobotNo[nSelection][eAxisX]]->GetScaleFactor());
			GetDlgItem(IDC_EDIT_CURRENT_SF_X)->SetWindowText(strOffset);
			strOffset.Format("%.6f", g_Motor[m_vRobotNo[nSelection][eAxisY]]->GetScaleFactor());
			GetDlgItem(IDC_EDIT_CURRENT_SF_Y)->SetWindowText(strOffset);

			strOffset.Format("%.6f", g_VatPcd.trayunloadpp.m_PcdOffset_XY.x /*/ 1000.0*/);
			GetDlgItem(IDC_EDIT_OFFSET_SF_X)->SetWindowText(strOffset);
			strOffset.Format("%.6f", g_VatPcd.trayunloadpp.m_PcdOffset_XY.y /*/ 1000.0*/);
			GetDlgItem(IDC_EDIT_OFFSET_SF_Y)->SetWindowText(strOffset);

			for (int i = eSpeed_5; i < eSpeed_MAX; i++) {
				strOffset.Format("%.3f", g_VatBacklash.trayunloadpp.m_BacklashOffset_XY[i].x / 1000.0);
				m_SprdBacklashOffset.SetTextMatrix(1, i + 1, strOffset);
				strOffset.Format("%.3f", g_VatBacklash.trayunloadpp.m_BacklashOffset_XY[i].y / 1000.0);
				m_SprdBacklashOffset.SetTextMatrix(2, i + 1, strOffset);
			}

		}break;
		case ePcdBacklash_TestPP1:
		case ePcdBacklash_TestPP2:
			{
				CString strOffset = _T("");

				strOffset.Format("%.6f", g_Motor[m_vRobotNo[nSelection][nTestPPAxisX]]->GetScaleFactor());
				GetDlgItem(IDC_EDIT_CURRENT_SF_X)->SetWindowText(strOffset);
				strOffset.Format("%.6f", g_Motor[m_vRobotNo[nSelection][eAxisY]]->GetScaleFactor());
				GetDlgItem(IDC_EDIT_CURRENT_SF_Y)->SetWindowText(strOffset);

				strOffset.Format("%.6f", g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_PcdOffset_XY.x);
				GetDlgItem(IDC_EDIT_OFFSET_SF_X)->SetWindowText(strOffset);
				strOffset.Format("%.6f", g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_PcdOffset_XY.y);
				GetDlgItem(IDC_EDIT_OFFSET_SF_Y)->SetWindowText(strOffset);

				for (int i = eSpeed_5; i < eSpeed_MAX; i++) {
					strOffset.Format("%.3f", g_VatBacklash.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_BacklashOffset_XY[i].x / 1000.0);
					m_SprdBacklashOffset.SetTextMatrix(1, i + 1, strOffset);
					strOffset.Format("%.3f", g_VatBacklash.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_BacklashOffset_XY[i].y / 1000.0);
					m_SprdBacklashOffset.SetTextMatrix(2, i + 1, strOffset);
				}
			}break;
	}
}

void CDlgPcdBacklash::UpdateTestPpSafetyData()
{
	int nSelection = m_cbRobotName.GetCurSel();
	int nHandSelection = m_cbHandName.GetCurSel();
	int nTestPPAxisX = nHandSelection == eTestPPLoadType ? eAxisX : eAxisX2;
	int nTestPPAxisZ = nHandSelection == eTestPPLoadType ? eAxisZ : eAxisZ2;

	if (nSelection >= ePcdBacklash_TestPP1 && nSelection <= ePcdBacklash_TestPP2) {

// 		GetDlgItem(IDC_GROP_Z_SAFETY_POS)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_STATIC_UPPER_SAFETY)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_EDIT_SAFETY_UPPER)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_STATIC_LOWER_SAFETY)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_EDIT_SAFETY_LOWER)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_BTN_TEST_PP_SAFETY_Z)->ShowWindow(SW_SHOW);
// 
// 		CString strSafety = _T("");
// 		strSafety.Format("%.3f", g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_dLowerSafetyPos);
// 		GetDlgItem(IDC_EDIT_SAFETY_LOWER)->SetWindowText(strSafety);
// 
// 		strSafety.Format("%.3f", g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_dUpperSafetyPos);
// 		GetDlgItem(IDC_EDIT_SAFETY_UPPER)->SetWindowText(strSafety);

	}
	else {
// 		GetDlgItem(IDC_GROP_Z_SAFETY_POS)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_STATIC_UPPER_SAFETY)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_EDIT_SAFETY_UPPER)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_STATIC_LOWER_SAFETY)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_EDIT_SAFETY_LOWER)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_BTN_TEST_PP_SAFETY_Z)->ShowWindow(SW_HIDE);
	}
}

void CDlgPcdBacklash::OnBtnGui()
{
	m_btnPcdReadEncX.SetShade(CShadeButtonST::SHS_METAL, 8, 20, 0, RGB(55, 55, 255));
	m_btnPcdReadEncX.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnPcdSavePosX.SetShade(CShadeButtonST::SHS_METAL, 8, 20, 0, RGB(55, 55, 255));
	m_btnPcdSavePosX.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnPcdReadEncY.SetShade(CShadeButtonST::SHS_METAL, 8, 20, 0, RGB(55, 55, 255));
	m_btnPcdReadEncY.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnPcdSavePosY.SetShade(CShadeButtonST::SHS_METAL, 8, 20, 0, RGB(55, 55, 255));
	m_btnPcdSavePosY.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnPcdStart.SetShade(CShadeButtonST::SHS_METAL, 8, 20, 0, RGB(55, 55, 255));
	m_btnPcdStart.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnSaveSF.SetShade(CShadeButtonST::SHS_METAL, 8, 20, 0, RGB(55, 55, 255));
	m_btnSaveSF.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnConnVision.SetShade(CShadeButtonST::SHS_METAL, 8, 20, 0, RGB(55, 55, 255));
	m_btnConnVision.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnConnVision2.SetShade(CShadeButtonST::SHS_METAL, 8, 20, 0, RGB(55, 55, 255));
	m_btnConnVision2.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnStop.SetShade(CShadeButtonST::SHS_METAL, 8, 20, 0, RGB(55, 55, 255));
	m_btnStop.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnExit.SetShade(CShadeButtonST::SHS_METAL, 8, 20, 0, RGB(55, 55, 255));
	m_btnExit.SetAlign(CButtonST::ST_ALIGN_VERT);


	m_btncancel.SetShade(CShadeButtonST::SHS_METAL, 8, 20, 0, RGB(55, 55, 255));
	m_btncancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnBacklashReadEnc.SetShade(CShadeButtonST::SHS_METAL, 8, 20, 0, RGB(55, 55, 255));
	m_btnBacklashReadEnc.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnBacklashSavePos.SetShade(CShadeButtonST::SHS_METAL, 8, 20, 0, RGB(55, 55, 255));
	m_btnBacklashSavePos.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnBacklashManualStart.SetShade(CShadeButtonST::SHS_METAL, 8, 20, 0, RGB(55, 55, 255));
	m_btnBacklashManualStart.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnBacklashAutoStart.SetShade(CShadeButtonST::SHS_METAL, 8, 20, 0, RGB(55, 55, 255));
	m_btnBacklashAutoStart.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnBacklashTest.SetShade(CShadeButtonST::SHS_METAL, 8, 20, 0, RGB(55, 55, 255));
	m_btnBacklashTest.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnBacklashSave.SetShade(CShadeButtonST::SHS_METAL, 8, 20, 0, RGB(55, 55, 255));
	m_btnBacklashSave.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnTestppSafetySave.SetShade(CShadeButtonST::SHS_METAL, 8, 20, 0, RGB(55, 55, 255));
	m_btnTestppSafetySave.SetAlign(CButtonST::ST_ALIGN_VERT);

	m_btnServo.SetShade(CShadeButtonST::SHS_METAL, 8, 20, 0, RGB(55, 55, 255));
	m_btnServo.SetAlign(CButtonST::ST_ALIGN_VERT);

}

void CDlgPcdBacklash::SaveScaleFactor(int nLogicalAxis, double dNewScaleFactor)
{
	_tMOTOR_PARAM MotorParam;
	g_DB.SELECT_MOTORPARAM_TBL(nLogicalAxis, MotorParam);

	MotorParam.dScaleFactor = dNewScaleFactor;
	g_DB.UPDATE_MOTORPARAM_TBL(nLogicalAxis, MotorParam);

	ReloadMotionDB();
}

void CDlgPcdBacklash::SetBacklashRepeat(BOOL & repeat, int & repeatCnt, int & tempRepeatCnt)
{
	UpdateData();
	if (m_cbRepeat.GetCheck() == TRUE)
	{
		repeat = TRUE;
		repeatCnt = m_nRepeat;
		tempRepeatCnt = m_nRepeat;
	}
	else
	{
		repeat = FALSE;
		repeatCnt = 1;
		tempRepeatCnt = 1;
	}
}

void CDlgPcdBacklash::UpdateVisionConnectStatus()
{
	if (g_pVisionTestHand_1 != NULL) {
		BOOL bConnected = g_pVisionTestHand_1->IsConnected();
		GetDlgItem(IDC_TEXT_VISION_CON)->SetWindowTextA(bConnected == TRUE ? ("Vision : CONNECTED") : ("Vision : DISCONNECTED"));
	}
	if (g_pVisionTestHand_2 != NULL) {
		BOOL bConnected = g_pVisionTestHand_2->IsConnected();
		GetDlgItem(IDC_TEXT_VISION_CON2)->SetWindowTextA(bConnected == TRUE ? ("Vision : CONNECTED") : ("Vision : DISCONNECTED"));
	}
}

void CDlgPcdBacklash::ChangedBacklashPosition(int nAxis)
{
	int nSelection = m_cbRobotName.GetCurSel();

	switch (nSelection)
	{
	case ePcdBacklash_TrayLoadPP:
	case ePcdBacklash_TrayUnloadPP:
	{
		TrayPPBacklashPosition(nAxis);
	}
	break;
	case ePcdBacklash_TestPP1:
	case ePcdBacklash_TestPP2:
	{
		TestPPBacklashPosition(nAxis);
	}
	break;
	}
}

void CDlgPcdBacklash::TrayPPBacklashPosition(int nAxis)
{
	CString strTempPosition = _T(""), strPosition = _T("");
	double dPosition = 0;

	switch (nAxis)
	{
		case eAxisX:
			{

				m_cbBacklashCnt.SetCurSel(4);
				m_cbBacklashDis.SetCurSel(14);

				GetDlgItem(IDC_EDIT_BACKLASH_X)->GetWindowText(strTempPosition);

				dPosition = atof(strTempPosition);
				strPosition.Format("%.3f", dPosition - ((m_cbBacklashDis.GetCurSel()+1)*10) );
				GetDlgItem(IDC_EDIT_BACKLASH_START_X)->SetWindowText(strPosition);
				strPosition.Format("%.3f", dPosition + ((m_cbBacklashDis.GetCurSel() + 1) * 10));
				GetDlgItem(IDC_EDIT_BACKLASH_END_X)->SetWindowText(strPosition);

				GetDlgItem(IDC_EDIT_BACKLASH_Y)->GetWindowText(strTempPosition);
				dPosition = atof(strTempPosition);
				strPosition.Format("%.3f", dPosition);
				GetDlgItem(IDC_EDIT_BACKLASH_START_Y)->SetWindowText(strPosition);
				GetDlgItem(IDC_EDIT_BACKLASH_END_Y)->SetWindowText(strPosition);

				GetDlgItem(IDC_EDIT_BACKLASH_Z)->GetWindowText(strTempPosition);
				dPosition = atof(strTempPosition);
				strPosition.Format("%.3f", dPosition);
				GetDlgItem(IDC_EDIT_BACKLASH_START_Z)->SetWindowText(strPosition);
				GetDlgItem(IDC_EDIT_BACKLASH_END_Z)->SetWindowText(strPosition);

			} break;
		case eAxisY:
			{
				m_cbBacklashCnt.SetCurSel(4);
				m_cbBacklashDis.SetCurSel(14);

				GetDlgItem(IDC_EDIT_BACKLASH_Y)->GetWindowText(strTempPosition);

				dPosition = atof(strTempPosition);
 				strPosition.Format("%.3f", dPosition - ((m_cbBacklashDis.GetCurSel() + 1) * 10));
 				GetDlgItem(IDC_EDIT_BACKLASH_START_Y)->SetWindowText(strPosition);
 				strPosition.Format("%.3f", dPosition + ((m_cbBacklashDis.GetCurSel() + 1) * 10));
				GetDlgItem(IDC_EDIT_BACKLASH_END_Y)->SetWindowText(strPosition);

				GetDlgItem(IDC_EDIT_BACKLASH_X)->GetWindowText(strTempPosition);

				dPosition = atof(strTempPosition);
				strPosition.Format("%.3f", dPosition);
				GetDlgItem(IDC_EDIT_BACKLASH_START_X)->SetWindowText(strPosition);
				GetDlgItem(IDC_EDIT_BACKLASH_END_X)->SetWindowText(strPosition);

				GetDlgItem(IDC_EDIT_BACKLASH_Z)->GetWindowText(strTempPosition);
				dPosition = atof(strTempPosition);
				strPosition.Format("%.3f", dPosition);
				GetDlgItem(IDC_EDIT_BACKLASH_START_Z)->SetWindowText(strPosition);
				GetDlgItem(IDC_EDIT_BACKLASH_END_Z)->SetWindowText(strPosition);			
			}
			break;
		case eAxisZ:
			{
				GetDlgItem(IDC_EDIT_BACKLASH_Z)->GetWindowText(strTempPosition);
				dPosition = atof(strTempPosition);
				strPosition.Format("%.3f", dPosition);
				GetDlgItem(IDC_EDIT_BACKLASH_START_Z)->SetWindowText(strPosition);
				GetDlgItem(IDC_EDIT_BACKLASH_END_Z)->SetWindowText(strPosition);
			}
			break;
	}
}

void CDlgPcdBacklash::TestPPBacklashPosition(int nAxis)
{
	CString strTempPosition = _T(""), strPosition = _T("");
	double dPosition = 0;

	int nHandSelection = m_cbHandName.GetCurSel();
	if (nHandSelection == eTestPPUnLoadType)
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_BACKLASH_AXIS_X);
		pButton->SetCheck(true);
		m_nBacklashAxis = 0;
		UpdateData(FALSE);
	}

	switch (nAxis)
	{
	case eAxisX:
	{
		m_cbBacklashCnt.SetCurSel(4);
		m_cbBacklashDis.SetCurSel(14);

		GetDlgItem(IDC_EDIT_BACKLASH_X)->GetWindowText(strTempPosition);

		dPosition = atof(strTempPosition);
		strPosition.Format("%.3f", dPosition - ((m_cbBacklashDis.GetCurSel() + 1) * 10));
		GetDlgItem(IDC_EDIT_BACKLASH_START_X)->SetWindowText(strPosition);
		strPosition.Format("%.3f", dPosition + ((m_cbBacklashDis.GetCurSel() + 1) * 10));
		GetDlgItem(IDC_EDIT_BACKLASH_END_X)->SetWindowText(strPosition);

		GetDlgItem(IDC_EDIT_BACKLASH_Y)->GetWindowText(strTempPosition);
		dPosition = atof(strTempPosition);
		strPosition.Format("%.3f", dPosition);
		GetDlgItem(IDC_EDIT_BACKLASH_START_Y)->SetWindowText(strPosition);
		GetDlgItem(IDC_EDIT_BACKLASH_END_Y)->SetWindowText(strPosition);

		GetDlgItem(IDC_EDIT_BACKLASH_Z)->GetWindowText(strTempPosition);
		dPosition = atof(strTempPosition);
		strPosition.Format("%.3f", dPosition);
		GetDlgItem(IDC_EDIT_BACKLASH_START_Z)->SetWindowText(strPosition);
		GetDlgItem(IDC_EDIT_BACKLASH_END_Z)->SetWindowText(strPosition);

	} break;
	case eAxisY:
	{
		m_cbBacklashCnt.SetCurSel(4);
		m_cbBacklashDis.SetCurSel(14);

		GetDlgItem(IDC_EDIT_BACKLASH_Y)->GetWindowText(strTempPosition);

		dPosition = atof(strTempPosition);
		strPosition.Format("%.3f", dPosition - ((m_cbBacklashDis.GetCurSel() + 1) * 10));
		GetDlgItem(IDC_EDIT_BACKLASH_START_Y)->SetWindowText(strPosition);
		strPosition.Format("%.3f", dPosition + ((m_cbBacklashDis.GetCurSel() + 1) * 10));
		GetDlgItem(IDC_EDIT_BACKLASH_END_Y)->SetWindowText(strPosition);

		GetDlgItem(IDC_EDIT_BACKLASH_X)->GetWindowText(strTempPosition);

		dPosition = atof(strTempPosition);
		strPosition.Format("%.3f", dPosition);
		GetDlgItem(IDC_EDIT_BACKLASH_START_X)->SetWindowText(strPosition);
		GetDlgItem(IDC_EDIT_BACKLASH_END_X)->SetWindowText(strPosition);

		GetDlgItem(IDC_EDIT_BACKLASH_Z)->GetWindowText(strTempPosition);
		dPosition = atof(strTempPosition);
		strPosition.Format("%.3f", dPosition);
		GetDlgItem(IDC_EDIT_BACKLASH_START_Z)->SetWindowText(strPosition);
		GetDlgItem(IDC_EDIT_BACKLASH_END_Z)->SetWindowText(strPosition);

	}
	break;
	case eAxisZ:
	{
		GetDlgItem(IDC_EDIT_BACKLASH_Z)->GetWindowText(strTempPosition);
		dPosition = atof(strTempPosition);
		strPosition.Format("%.3f", dPosition);
		GetDlgItem(IDC_EDIT_BACKLASH_START_Z)->SetWindowText(strPosition);
		GetDlgItem(IDC_EDIT_BACKLASH_END_Z)->SetWindowText(strPosition);
	}
	break;
	}
}

void CDlgPcdBacklash::ReloadMotionDB()
{
	CMyList m_ListAxisInfoDb;
	int nErr = g_DB.SELECT_MOTORPARAM_TBL(m_ListAxisInfoDb); // 모터 정보를 Logical Idx 오름 차순으로 불러 온다.
	if (nErr != ERR_NO_ERROR) {
		AfxMessageBox(_T("SELECT_MOTORPARAM_TBL fail"));
		return;
	}

	int nAxisCount = g_Motor.GetAxisCount();
	for (int i = 0; i < nAxisCount; i++)
	{
		BOOL bDegreeCtl = FALSE;
		_tMOTOR_PARAM* pAxisInfo = (_tMOTOR_PARAM*)m_ListAxisInfoDb.m_fnGetData(i);
		g_Motor[i]->SetAxisInitInfo((LPSTR)(LPCTSTR)pAxisInfo->strAxisName,
			pAxisInfo->nAxisNo_Physical,
			pAxisInfo->nAxisNo_Logical,
			pAxisInfo->dScaleFactor,
			pAxisInfo->nDir_ZR,
			pAxisInfo->nMMC_Encoder_Dir,
			pAxisInfo->nDirMove,
			pAxisInfo->nEncoder_Type,
			pAxisInfo->nMMC_Coordinate_Dir,
			pAxisInfo->nDirPulse,
			pAxisInfo->nDouble_Chk_Pulse,
			(MOTOR_TYPE)pAxisInfo->nMotorType,
			(MOTOR_SYNC_TYPE)pAxisInfo->nSyncType,
			pAxisInfo->nSlaveNo,
			bDegreeCtl,
			pAxisInfo->nPulsePerRound,
			pAxisInfo->dHwInPositionWidth);

		g_Motor[i]->SetVelAcc(pAxisInfo->nSpd_Normal,
			pAxisInfo->nAcc_Normal,
			pAxisInfo->nDec_Normal,
			pAxisInfo->nSpd_ZR,
			pAxisInfo->nAcc_ZR,
			pAxisInfo->dLimit_Posi,
			pAxisInfo->dLimit_Nega);
	}
}

void CDlgPcdBacklash::SaveBacklashPosition()
{
	CString strCurPos_X = _T(""), strCurPos_Y = _T(""), strCurPos_Z = _T("");
	CString strStartPos_X = _T(""), strStartPos_Y = _T(""), strStartPos_Z = _T("");
	CString strEndPos_X = _T(""), strEndPos_Y = _T(""), strEndPos_Z = _T("");

	GetDlgItem(IDC_EDIT_BACKLASH_START_X)->GetWindowTextA(strStartPos_X);
	GetDlgItem(IDC_EDIT_BACKLASH_START_Y)->GetWindowTextA(strStartPos_Y);
	GetDlgItem(IDC_EDIT_BACKLASH_START_Z)->GetWindowTextA(strStartPos_Z);

	GetDlgItem(IDC_EDIT_BACKLASH_X)->GetWindowTextA(strCurPos_X);
	GetDlgItem(IDC_EDIT_BACKLASH_Y)->GetWindowTextA(strCurPos_Y);
	GetDlgItem(IDC_EDIT_BACKLASH_Z)->GetWindowTextA(strCurPos_Z);

	GetDlgItem(IDC_EDIT_BACKLASH_END_X)->GetWindowTextA(strEndPos_X);
	GetDlgItem(IDC_EDIT_BACKLASH_END_Y)->GetWindowTextA(strEndPos_Y);
	GetDlgItem(IDC_EDIT_BACKLASH_END_Z)->GetWindowTextA(strEndPos_Z);

	int nSelection = m_cbRobotName.GetCurSel();
	int nHandSelection = m_cbHandName.GetCurSel();
	switch (nSelection)
	{
		case ePcdBacklash_TrayLoadPP:
			{
				g_VatBacklash.trayloadpp.m_BacklashStartPos_XY.x = atof(strStartPos_X) * 1000.0;
				g_VatBacklash.trayloadpp.m_BacklashStartPos_XY.y = atof(strStartPos_Y) * 1000.0;
				g_VatBacklash.trayloadpp.m_BacklashStartPos_XY.z = atof(strStartPos_Z) * 1000.0;

				g_VatBacklash.trayloadpp.m_BacklashTargetPos_XYZ.x = atof(strCurPos_X) * 1000.0;
				g_VatBacklash.trayloadpp.m_BacklashTargetPos_XYZ.y = atof(strCurPos_Y) * 1000.0;
				g_VatBacklash.trayloadpp.m_BacklashTargetPos_XYZ.z = atof(strCurPos_Z) * 1000.0;

				g_VatBacklash.trayloadpp.m_BacklashEndPos_XY.x = atof(strEndPos_X) * 1000.0;
				g_VatBacklash.trayloadpp.m_BacklashEndPos_XY.y = atof(strEndPos_Y) * 1000.0;
				g_VatBacklash.trayloadpp.m_BacklashEndPos_XY.z = atof(strEndPos_Z) * 1000.0;

				g_VatBacklash.trayloadpp.Save();
			}
			break;
		case ePcdBacklash_TrayUnloadPP:
			{
				g_VatBacklash.trayunloadpp.m_BacklashStartPos_XY.x = atof(strStartPos_X) * 1000.0;
				g_VatBacklash.trayunloadpp.m_BacklashStartPos_XY.y = atof(strStartPos_Y) * 1000.0;
				g_VatBacklash.trayunloadpp.m_BacklashStartPos_XY.z = atof(strStartPos_Z) * 1000.0;

				g_VatBacklash.trayunloadpp.m_BacklashTargetPos_XYZ.x = atof(strCurPos_X) * 1000.0;
				g_VatBacklash.trayunloadpp.m_BacklashTargetPos_XYZ.y = atof(strCurPos_Y) * 1000.0;
				g_VatBacklash.trayunloadpp.m_BacklashTargetPos_XYZ.z = atof(strCurPos_Z) * 1000.0;

				g_VatBacklash.trayunloadpp.m_BacklashEndPos_XY.x = atof(strEndPos_X) * 1000.0;
				g_VatBacklash.trayunloadpp.m_BacklashEndPos_XY.y = atof(strEndPos_Y) * 1000.0;
				g_VatBacklash.trayunloadpp.m_BacklashEndPos_XY.z = atof(strEndPos_Z) * 1000.0;

				g_VatBacklash.trayunloadpp.Save();
			}
			break;
		case ePcdBacklash_TestPP1:
		case ePcdBacklash_TestPP2:
			{
				g_VatBacklash.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_BacklashStartPos_XY.x = atof(strStartPos_X) * 1000.0;
				g_VatBacklash.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_BacklashStartPos_XY.y = atof(strStartPos_Y) * 1000.0;
				g_VatBacklash.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_BacklashStartPos_XY.z = atof(strStartPos_Z) * 1000.0;

				g_VatBacklash.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_BacklashTargetPos_XYZ.x = atof(strCurPos_X) * 1000.0;
				g_VatBacklash.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_BacklashTargetPos_XYZ.y = atof(strCurPos_Y) * 1000.0;
				g_VatBacklash.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_BacklashTargetPos_XYZ.z = atof(strCurPos_Z) * 1000.0;

				g_VatBacklash.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_BacklashEndPos_XY.x = atof(strEndPos_X) * 1000.0;
				g_VatBacklash.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_BacklashEndPos_XY.y = atof(strEndPos_Y) * 1000.0;
				g_VatBacklash.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_BacklashEndPos_XY.z = atof(strEndPos_Z) * 1000.0;

				g_VatBacklash.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].Save(nHandSelection);
			}
			break;
	}
}


void CDlgPcdBacklash::ClearSpreadBackColor(BOOL bOnTimer)
{
	if (m_bManualCmdExcute == TRUE && bOnTimer == TRUE)
		return;

	int nRowCnt = m_SprdBacklashOffset.GetRow();
	int nColCnt = m_SprdBacklashOffset.GetCol();

	for (int r = 0; r < nRowCnt; r++)
	{
		for (int c = 0; c < nColCnt; c++)
		{
			m_SprdBacklashOffset.SetRow(r+1);
			m_SprdBacklashOffset.SetCol(c+1);
			m_SprdBacklashOffset.SetBackColor(RGB(255, 255, 255));
		}
	}
}

void CDlgPcdBacklash::ClearSpreadSelectBackColor(int nRow)
{

	int nColCnt = m_SprdBacklashOffset.GetCol();

// 	for (int r = 0; r < nRowCnt; r++)
// 	{
		for (int c = 0; c < nColCnt; c++)
		{
			m_SprdBacklashOffset.SetRow(nRow);
			m_SprdBacklashOffset.SetCol(c + 1);
			m_SprdBacklashOffset.SetBackColor(RGB(255, 255, 255));
		}
//	}
}

void CDlgPcdBacklash::SetSpreadBackColor(int nRow, int nCol)
{
	m_SprdBacklashOffset.SetRow(nRow);
	m_SprdBacklashOffset.SetCol(nCol);
	m_SprdBacklashOffset.SetBackColor(RGB(255, 255, 0));
}

void CDlgPcdBacklash::SetSpreaddisableBackColor()
{
	int nColCnt = m_SprdBacklashOffset.GetCol();
	for (int c = 0; c < nColCnt; c++)
	{
		m_SprdBacklashOffset.SetRow(2);
		m_SprdBacklashOffset.SetCol(c + 1);
		m_SprdBacklashOffset.SetBackColor(RGB(0, 0, 0));
	}
}


void CDlgPcdBacklash::SaveBacklashData()
{
	CString strOffset = _T("");

	int nSelection = m_cbRobotName.GetCurSel();
	int nHandSelection = m_cbHandName.GetCurSel();
	switch (nSelection) {
		case ePcdBacklash_TrayLoadPP:
			{
				for (int i = eSpeed_5; i < eSpeed_MAX; i++) {
					strOffset = m_SprdBacklashOffset.GetTextMatrix(1, i + 1);
					g_VatBacklash.trayloadpp.m_BacklashOffset_XY[i].x = atof(strOffset) * 1000.0;
					strOffset = m_SprdBacklashOffset.GetTextMatrix(2, i + 1);
					g_VatBacklash.trayloadpp.m_BacklashOffset_XY[i].y = atof(strOffset) * 1000.0;
				}
				g_VatBacklash.trayloadpp.Save();
				g_TaskTrayLoadPP.ApplyBacklash();
			}
			break;
		case ePcdBacklash_TrayUnloadPP:
		{
			for (int i = eSpeed_5; i < eSpeed_MAX; i++) {
				strOffset = m_SprdBacklashOffset.GetTextMatrix(1, i + 1);
				g_VatBacklash.trayunloadpp.m_BacklashOffset_XY[i].x = atof(strOffset) * 1000.0;
				strOffset = m_SprdBacklashOffset.GetTextMatrix(2, i + 1);
				g_VatBacklash.trayunloadpp.m_BacklashOffset_XY[i].y = atof(strOffset) * 1000.0;
			}
			g_VatBacklash.trayunloadpp.Save();
			g_TaskTrayUnloadPP.ApplyBacklash();
		}
		break;
		case ePcdBacklash_TestPP1:
		case ePcdBacklash_TestPP2:
			{
				for (int i = eSpeed_5; i < eSpeed_MAX; i++) {
					strOffset = m_SprdBacklashOffset.GetTextMatrix(1, i + 1);
					g_VatBacklash.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_BacklashOffset_XY[i].x = atof(strOffset) * 1000.0;
					strOffset = m_SprdBacklashOffset.GetTextMatrix(2, i + 1);
					g_VatBacklash.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_BacklashOffset_XY[i].y = atof(strOffset) * 1000.0;
				}
				g_VatBacklash.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].Save(nHandSelection);
				g_TaskTestPP[ nSelection - ePcdBacklash_TestPP1].ApplyBacklash(nHandSelection);
			}
			break;
	}
}

void CDlgPcdBacklash::SavePcdPositionX()
{
	CString strCurPos_X = _T(""), strCurPos_Y = _T(""), strCurPos_Z = _T("");
	GetDlgItem(IDC_EDIT_PCD_XAxis_X)->GetWindowTextA(strCurPos_X);
	GetDlgItem(IDC_EDIT_PCD_XAxis_Y)->GetWindowTextA(strCurPos_Y);
	GetDlgItem(IDC_EDIT_PCD_XAxis_Z)->GetWindowTextA(strCurPos_Z);

	int nSelection = m_cbRobotName.GetCurSel();
	int nHandSelection = m_cbHandName.GetCurSel();
	int nTestPPAxisX = nHandSelection == eTestPPLoadType ? eAxisX : eAxisX2;
	int nTestPPAxisZ = nHandSelection == eTestPPLoadType ? eAxisZ : eAxisZ2;

	switch (nSelection)
	{
		case ePcdBacklash_TrayLoadPP:
			{
				g_VatPcd.trayloadpp.m_PcdStartXPos_XYZ.x = atof(strCurPos_X) * 1000.0;
				g_VatPcd.trayloadpp.m_PcdStartXPos_XYZ.y = atof(strCurPos_Y) * 1000.0;
				g_VatPcd.trayloadpp.m_PcdStartXPos_XYZ.z = atof(strCurPos_Z) * 1000.0;
				g_VatPcd.trayloadpp.Save();
			}
			break;
		case ePcdBacklash_TrayUnloadPP:
		{
			g_VatPcd.trayunloadpp.m_PcdStartXPos_XYZ.x = atof(strCurPos_X) * 1000.0;
			g_VatPcd.trayunloadpp.m_PcdStartXPos_XYZ.y = atof(strCurPos_Y) * 1000.0;
			g_VatPcd.trayunloadpp.m_PcdStartXPos_XYZ.z = atof(strCurPos_Z) * 1000.0;
			g_VatPcd.trayunloadpp.Save();
		}
		break;
		case ePcdBacklash_TestPP1:
		case ePcdBacklash_TestPP2:
			{
				g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_PcdStartXPos_XYZ.x = atof(strCurPos_X) * 1000.0;
				g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_PcdStartXPos_XYZ.y = atof(strCurPos_Y) * 1000.0;
				g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_PcdStartXPos_XYZ.z = atof(strCurPos_Z) * 1000.0;
				g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].Save(nHandSelection);
			}
			break;
	}
}

void CDlgPcdBacklash::SavePcdPositionY()
{
	CString strCurPos_X = _T(""), strCurPos_Y = _T(""), strCurPos_Z = _T("");

	GetDlgItem(IDC_EDIT_PCD_YAxis_X)->GetWindowTextA(strCurPos_X);
	GetDlgItem(IDC_EDIT_PCD_YAxis_Y)->GetWindowTextA(strCurPos_Y);
	GetDlgItem(IDC_EDIT_PCD_YAxis_Z)->GetWindowTextA(strCurPos_Z);

	int nSelection = m_cbRobotName.GetCurSel();
	int nHandSelection = m_cbHandName.GetCurSel();
	int nTestPPAxisX = nHandSelection == eTestPPLoadType ? eAxisX : eAxisX2;
	int nTestPPAxisZ = nHandSelection == eTestPPLoadType ? eAxisZ : eAxisZ2;

	switch (nSelection)
	{
		case ePcdBacklash_TrayLoadPP:
		{
				g_VatPcd.trayloadpp.m_PcdStartYPos_XYZ.x = atof(strCurPos_X) * 1000.0;
				g_VatPcd.trayloadpp.m_PcdStartYPos_XYZ.y = atof(strCurPos_Y) * 1000.0;
				g_VatPcd.trayloadpp.m_PcdStartYPos_XYZ.z = atof(strCurPos_Z) * 1000.0;
				g_VatPcd.trayloadpp.Save();
		}
		break;
		case ePcdBacklash_TrayUnloadPP:
			{
				g_VatPcd.trayunloadpp.m_PcdStartYPos_XYZ.x = atof(strCurPos_X) * 1000.0;
				g_VatPcd.trayunloadpp.m_PcdStartYPos_XYZ.y = atof(strCurPos_Y) * 1000.0;
				g_VatPcd.trayunloadpp.m_PcdStartYPos_XYZ.z = atof(strCurPos_Z) * 1000.0;
				g_VatPcd.trayunloadpp.Save();
			}
			break;
		case ePcdBacklash_TestPP1:
		case ePcdBacklash_TestPP2:
			{
				g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_PcdStartYPos_XYZ.x = atof(strCurPos_X) * 1000.0;
				g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_PcdStartYPos_XYZ.y = atof(strCurPos_Y) * 1000.0;
				g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_PcdStartYPos_XYZ.z = atof(strCurPos_Z) * 1000.0;
				g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].Save(nHandSelection);
			}
			break;
	}
}

BOOL CDlgPcdBacklash::ChkPcdPosInterlock()
{
	CString strCurPos_X, strCurPos_Y, strCurPos_Z;
	BOOL bIsSafetyPos = TRUE;
	double dPosX = 0.0;
	double dPosY = 0.0;
	double dPosZ = 0.0;
	if (m_nPcdAxis == eAxisX)
	{
		GetDlgItem(IDC_EDIT_PCD_XAxis_X)->GetWindowTextA(strCurPos_X);
		dPosX = atof(strCurPos_X);
		GetDlgItem(IDC_EDIT_PCD_XAxis_Y)->GetWindowTextA(strCurPos_Y);
		dPosY = atof(strCurPos_Y);
		GetDlgItem(IDC_EDIT_PCD_XAxis_Z)->GetWindowTextA(strCurPos_Z);
		dPosZ = atof(strCurPos_Z);
		
	}
	else
	{
		GetDlgItem(IDC_EDIT_PCD_YAxis_X)->GetWindowTextA(strCurPos_X);
		dPosX = atof(strCurPos_X);
		GetDlgItem(IDC_EDIT_PCD_YAxis_Y)->GetWindowTextA(strCurPos_Y);
		dPosY = atof(strCurPos_Y);
		GetDlgItem(IDC_EDIT_PCD_YAxis_Z)->GetWindowTextA(strCurPos_Z);
		dPosZ = atof(strCurPos_Z);

	}
	if (dPosX == 0 && dPosY == 0 && dPosZ == 0)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CDlgPcdBacklash::ChkBacklashPosInterlock()
{
	CString strCurPos_X, strCurPos_Y, strCurPos_Z;
	BOOL bIsSafetyPos = TRUE;
	double dPosX = 0.0;
	double dPosY = 0.0;
	double dPosZ = 0.0;

	GetDlgItem(IDC_EDIT_BACKLASH_X)->GetWindowTextA(strCurPos_X);
	dPosX = atof(strCurPos_X);
	GetDlgItem(IDC_EDIT_BACKLASH_Y)->GetWindowTextA(strCurPos_Y);
	dPosY = atof(strCurPos_Y);
	GetDlgItem(IDC_EDIT_BACKLASH_Z)->GetWindowTextA(strCurPos_Z);
	dPosZ = atof(strCurPos_Z);

	if (dPosX == 0 && dPosY == 0 && dPosZ == 0)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CDlgPcdBacklash::ChkTestPPSafetyPosValue()
{
	BOOL bIsSafety = TRUE;
// 	int nUnitIdx = m_cbRobotName.GetCurSel();
// 	CString strSafeDN = _T("");
// 	CString strSafeUP = _T("");
// 	if (nUnitIdx != ePcdBacklash_TrayLoadPP && nUnitIdx != ePcdBacklash_TrayUnloadPP)
// 	{
// 		GetDlgItem(IDC_EDIT_SAFETY_UPPER)->GetWindowTextA(strSafeUP);
// 		GetDlgItem(IDC_EDIT_SAFETY_LOWER)->GetWindowTextA(strSafeDN);
// 
// 		double dSafeUP = atof(strSafeUP);
// 		double dSafeDN = atof(strSafeDN);
// 		OneAxis* pAxis = g_TaskTestPP[nUnitIdx - ePcdBacklash_TestPP1].GetAxis(CTaskTestPP::eAxisZ1);
// 		double dLimitVal = pAxis->GetSwLimitPosi() / 1000.0;
// 		if (/*0 < dSafeUP &&*/ 0 < dSafeDN && dSafeUP < dLimitVal && dSafeDN < dLimitVal && dSafeUP < dSafeDN)
// 		{
// 			bIsSafety = TRUE;
// 		}
// 		else
// 		{
// 			bIsSafety = FALSE;
// 		}
// 	}
	return bIsSafety;
}

void CDlgPcdBacklash::OnCbnSelchangeCbRobotChoose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateOffsetData();
	UpdateTeachingData();
	UpdateTestPpSafetyData();
	HandSelectUIShowHide();
	int nAxis = (m_nPcdAxis == eAxisX) ? IDC_PCD_AXIS_X : IDC_PCD_AXIS_Y;
	OnRadioButtonPcdAxisChanged(nAxis);
//	UnitSelectUIShowHide();
}


void CDlgPcdBacklash::OnBnClickedBtnManualStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CBaseTaskManager::SendProcessCommand(base_seq_cmd::eProcCmd_Stop, eEqpCtrlMode_Manual, NULL);
}


void CDlgPcdBacklash::OnBnClickedBtnExit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_bManualCmdExcute == TRUE) {
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	int nRet = AfxMessageBox("Do you want to Backlash Data Save before going out?", MB_YESNO | MB_TOPMOST);

	if (nRet == IDYES)
	{
		SaveBacklashData();
	}

	CDialogEx::OnCancel();
}

void CDlgPcdBacklash::OnBnClickedBtnPcdXReadEncoder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	double dCurPos_X = 0.0, dCurPos_Y = 0.0, dCurPos_Z = 0.0;
	CString strCurPos_X = _T(""), strCurPos_Y = _T(""), strCurPos_Z = _T("");

	int nRet = AfxMessageBox("Do you want to read encoder?", MB_YESNO | MB_TOPMOST);
	if (nRet == IDYES)
	{
		int nSelection = m_cbRobotName.GetCurSel();
		int nHandSelection = m_cbHandName.GetCurSel();
		int nTestPPAxisX = nHandSelection == eTestPPLoadType ? eAxisX : eAxisX2;
		int nTestPPAxisZ = nHandSelection == eTestPPLoadType ? eAxisZ : eAxisZ2;
		switch (nSelection)
		{
			case ePcdBacklash_TrayLoadPP:
			{
				g_Motor[m_vRobotNo[nSelection][eAxisX]]->GetCurrentPos(dCurPos_X);
				g_Motor[m_vRobotNo[nSelection][eAxisY]]->GetCurrentPos(dCurPos_Y);
				//g_Motor[m_vRobotNo[nSelection][eAxisZ]]->GetCurrentPos(dCurPos_Z);
				g_Motor[m_vRobotNo[nSelection][eAxisZ]]->GetCurrentPos(dCurPos_Z, _eBbxmnpAxis::eBMAxis_LOAD_TRAY_Picker07);
			}break;
			case ePcdBacklash_TrayUnloadPP:
			{
				g_Motor[m_vRobotNo[nSelection][eAxisX]]->GetCurrentPos(dCurPos_X);
				g_Motor[m_vRobotNo[nSelection][eAxisY]]->GetCurrentPos(dCurPos_Y);
				//g_Motor[m_vRobotNo[nSelection][eAxisZ]]->GetCurrentPos(dCurPos_Z);
				g_Motor[m_vRobotNo[nSelection][eAxisZ]]->GetCurrentPos(dCurPos_Z, _eBbxmnpAxis::eBMAxis_UNLOAD_TRAY_Picker07);
			}break;
			case ePcdBacklash_TestPP1:
			{
				g_Motor[m_vRobotNo[nSelection][nTestPPAxisX]]->GetCurrentPos(dCurPos_X);
				g_Motor[m_vRobotNo[nSelection][eAxisY]]->GetCurrentPos(dCurPos_Y);
				//g_Motor[m_vRobotNo[nSelection][nTestPPAxisZ]]->GetCurrentPos(dCurPos_Z);
				if (nHandSelection == eTestPPLoadType)
					g_Motor[m_vRobotNo[nSelection][nTestPPAxisZ]]->GetCurrentPos(dCurPos_Z, _eBbxmnpAxis::eBMAxis_LOAD_TESTPP_1_Picker03);
				else
					g_Motor[m_vRobotNo[nSelection][nTestPPAxisZ]]->GetCurrentPos(dCurPos_Z, _eBbxmnpAxis::eBMAxis_UNLOAD_TESTPP_1_Picker02);
			}break;
			case ePcdBacklash_TestPP2:
			{
				g_Motor[m_vRobotNo[nSelection][nTestPPAxisX]]->GetCurrentPos(dCurPos_X);
				g_Motor[m_vRobotNo[nSelection][eAxisY]]->GetCurrentPos(dCurPos_Y);
				//g_Motor[m_vRobotNo[nSelection][nTestPPAxisZ]]->GetCurrentPos(dCurPos_Z);
				if (nHandSelection == eTestPPLoadType)
					g_Motor[m_vRobotNo[nSelection][nTestPPAxisZ]]->GetCurrentPos(dCurPos_Z, _eBbxmnpAxis::eBMAxis_LOAD_TESTPP_1_Picker02);
				else
					g_Motor[m_vRobotNo[nSelection][nTestPPAxisZ]]->GetCurrentPos(dCurPos_Z, _eBbxmnpAxis::eBMAxis_UNLOAD_TESTPP_1_Picker03);
			}break;
			default:
				break;
		}

		strCurPos_X.Format(_T("%.3f"), dCurPos_X/1000.0);
		strCurPos_Y.Format(_T("%.3f"), dCurPos_Y/1000.0);
		strCurPos_Z.Format(_T("%.3f"), dCurPos_Z/1000.0);

		GetDlgItem(IDC_EDIT_PCD_XAxis_X)->SetWindowTextA(strCurPos_X);
		GetDlgItem(IDC_EDIT_PCD_XAxis_Y)->SetWindowTextA(strCurPos_Y);
		GetDlgItem(IDC_EDIT_PCD_XAxis_Z)->SetWindowTextA(strCurPos_Z);
	}
}


void CDlgPcdBacklash::OnBnClickedBtnPcdXSavePos()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nRet = AfxMessageBox("Do you want to save position x?", MB_YESNO | MB_TOPMOST);
	if (nRet == IDYES)
	{
		if (ChkPcdPosInterlock() == TRUE)
		{
			SavePcdPositionX();
		}
		else
		{
			AfxMessageBox("Please Check Your Start Position", MB_OK | MB_TOPMOST);
		}
		
	}
}


void CDlgPcdBacklash::OnBnClickedBtnPcdYReadEncoder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	double dCurPos_X = 0.0, dCurPos_Y = 0.0, dCurPos_Z = 0.0;
	CString strCurPos_X = _T(""), strCurPos_Y = _T(""), strCurPos_Z = _T("");

	int nRet = AfxMessageBox("Do you want to read encoder?", MB_YESNO | MB_TOPMOST);
	if (nRet == IDYES)
	{
		int nSelection = m_cbRobotName.GetCurSel();
		int nHandSelection = m_cbHandName.GetCurSel();
		int nTestPPAxisX = nHandSelection == eTestPPLoadType ? eAxisX : eAxisX2;
		int nTestPPAxisZ = nHandSelection == eTestPPLoadType ? eAxisZ : eAxisZ2;
		switch (nSelection)
		{
		case ePcdBacklash_TrayLoadPP:
			{
				g_Motor[m_vRobotNo[nSelection][eAxisX]]->GetCurrentPos(dCurPos_X);
				dCurPos_Y = -1;
				//g_Motor[m_vRobotNo[nSelection][eAxisZ]]->GetCurrentPos(dCurPos_Z);
				g_Motor[m_vRobotNo[nSelection][eAxisZ]]->GetCurrentPos(dCurPos_Z, _eBbxmnpAxis::eBMAxis_LOAD_TRAY_Picker07);
			}break;
		case ePcdBacklash_TrayUnloadPP:
		{
				g_Motor[m_vRobotNo[nSelection][eAxisX]]->GetCurrentPos(dCurPos_X);
				g_Motor[m_vRobotNo[nSelection][eAxisY]]->GetCurrentPos(dCurPos_Y);
				//g_Motor[m_vRobotNo[nSelection][eAxisZ]]->GetCurrentPos(dCurPos_Z);
				g_Motor[m_vRobotNo[nSelection][eAxisZ]]->GetCurrentPos(dCurPos_Z, _eBbxmnpAxis::eBMAxis_UNLOAD_TRAY_Picker07);
		}break;
		case ePcdBacklash_TestPP1:
		{
				g_Motor[m_vRobotNo[nSelection][nTestPPAxisX]]->GetCurrentPos(dCurPos_X);
				g_Motor[m_vRobotNo[nSelection][eAxisY]]->GetCurrentPos(dCurPos_Y);
				//g_Motor[m_vRobotNo[nSelection][nTestPPAxisZ]]->GetCurrentPos(dCurPos_Z);
				if (nHandSelection == eTestPPLoadType)
					g_Motor[m_vRobotNo[nSelection][nTestPPAxisZ]]->GetCurrentPos(dCurPos_Z, _eBbxmnpAxis::eBMAxis_LOAD_TESTPP_1_Picker03);
				else
					g_Motor[m_vRobotNo[nSelection][nTestPPAxisZ]]->GetCurrentPos(dCurPos_Z, _eBbxmnpAxis::eBMAxis_UNLOAD_TESTPP_1_Picker02);
		}break;
		case ePcdBacklash_TestPP2:
		{
				g_Motor[m_vRobotNo[nSelection][nTestPPAxisX]]->GetCurrentPos(dCurPos_X);
				g_Motor[m_vRobotNo[nSelection][eAxisY]]->GetCurrentPos(dCurPos_Y);
				//g_Motor[m_vRobotNo[nSelection][nTestPPAxisZ]]->GetCurrentPos(dCurPos_Z);
				if (nHandSelection == eTestPPLoadType)
					g_Motor[m_vRobotNo[nSelection][nTestPPAxisZ]]->GetCurrentPos(dCurPos_Z, _eBbxmnpAxis::eBMAxis_LOAD_TESTPP_1_Picker02);
				else
					g_Motor[m_vRobotNo[nSelection][nTestPPAxisZ]]->GetCurrentPos(dCurPos_Z, _eBbxmnpAxis::eBMAxis_UNLOAD_TESTPP_1_Picker03);
		}break;
		default:
			break;
		}
		
		strCurPos_X.Format(_T("%.3f"), dCurPos_X / 1000.0);
		strCurPos_Y.Format(_T("%.3f"), dCurPos_Y / 1000.0);
		strCurPos_Z.Format(_T("%.3f"), dCurPos_Z / 1000.0);

		GetDlgItem(IDC_EDIT_PCD_YAxis_X)->SetWindowTextA(strCurPos_X);
		GetDlgItem(IDC_EDIT_PCD_YAxis_Y)->SetWindowTextA(strCurPos_Y);
		GetDlgItem(IDC_EDIT_PCD_YAxis_Z)->SetWindowTextA(strCurPos_Z);
	}
}


void CDlgPcdBacklash::OnBnClickedBtnPcdYSavePos()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int nRet = AfxMessageBox("Do you want to save position x?", MB_YESNO | MB_TOPMOST);
	if (nRet == IDYES)
	{
		if (ChkPcdPosInterlock() == TRUE)
		{
			SavePcdPositionY();
		}
		else
		{
			AfxMessageBox("Please Check Your Start Position", MB_OK | MB_TOPMOST);
		}
	}
}


void CDlgPcdBacklash::OnBnClickedBtnPcdStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//g_pVAT->SendMeasure_Vat(VAT_MEASURE_HOLE_CENTER, VAT_AREA_TRAYPP);
	if (m_bManualCmdExcute) {
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	int nRet = AfxMessageBox("Do you want to start the PCD Measurement?", MB_YESNO | MB_TOPMOST);
	if (nRet == IDYES)
	{
		if (ChkTestPPSafetyPosValue() == TRUE && ChkPcdPosInterlock() == TRUE)
		{
			m_nPcdAxis == eAxisX ? SavePcdPositionX() : SavePcdPositionY();
			int nSelection = m_cbRobotName.GetCurSel();
			std::vector<CPoint> vPkr;
			g_pBbxmnpClient->MakeAllPicker(vPkr);
			switch (nSelection)
			{
				case ePcdBacklash_TrayLoadPP:
					{
						CTrayLoadPPCmd_VatPcd* pSeqCmd = new CTrayLoadPPCmd_VatPcd();

						pSeqCmd->m_nAxisNum = m_nPcdAxis;
						pSeqCmd->m_nDistance = ((m_cbPcdDis.GetCurSel() + 1) * 10) * 1000 ;
						pSeqCmd->vPicker = g_pBbxmnpClient->VisionCamStandardPicker(CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_LOAD_TRAY);
						pSeqCmd->m_vSafetyAllPicker = vPkr;
						int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTrayLoadPP, pSeqCmd, eEqpCtrlMode_Manual);
					}
					break;
				case ePcdBacklash_TrayUnloadPP:
				{
					CTrayUnloadPPCmd_VatPcd* pSeqCmd = new CTrayUnloadPPCmd_VatPcd();

					pSeqCmd->m_nAxisNum = m_nPcdAxis;
					pSeqCmd->m_nDistance = ((m_cbPcdDis.GetCurSel() + 1) * 10) * 1000;
					pSeqCmd->m_Cmd.vPicker = g_pBbxmnpClient->VisionCamStandardPicker(CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_UNLOAD_TRAY);
					pSeqCmd->m_vSafetyAllPicker = vPkr;
					int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTrayUnloadPP, pSeqCmd, eEqpCtrlMode_Manual);
				}
				break;
				case ePcdBacklash_TestPP1:
				case ePcdBacklash_TestPP2:
					{
						CTestPPCmd_VatPcd* pSeqCmd = new CTestPPCmd_VatPcd(nSelection - ePcdBacklash_TestPP1);

						pSeqCmd->m_nAxisNum = m_nPcdAxis;
						pSeqCmd->m_nDistance = ((m_cbPcdDis.GetCurSel() + 1) * 10) * 1000;
						pSeqCmd->m_nHandType =  m_cbHandName.GetCurSel();
						CBbxmnpClient::_ePICK_MOTOR eMotorNum;
						switch (nSelection)
						{
						case ePcdBacklash_TestPP1: { eMotorNum = pSeqCmd->m_nHandType == eTestPPLoadType ? CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_LOAD_TEST_1 : CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_UNLOAD_TEST_1;}break;
						case ePcdBacklash_TestPP2: { eMotorNum = pSeqCmd->m_nHandType == eTestPPLoadType ? CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_LOAD_TEST_2 : CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_UNLOAD_TEST_2;}break;
						default:
							break;
						}

						pSeqCmd->m_vCmdPicker = g_pBbxmnpClient->VisionCamStandardPicker(eMotorNum);
						pSeqCmd->m_vSafetyAllPicker = vPkr;
						int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTestPP[nSelection - ePcdBacklash_TestPP1], pSeqCmd, eEqpCtrlMode_Manual);
					}
					break;
			}
		}
		else
		{
			AfxMessageBox("Please Check Test PP Safety Pos Z or Check Your Start Position", MB_OK | MB_TOPMOST);
		}
	}


}


void CDlgPcdBacklash::OnBnClickedBtnPcdSaveScaleFactor()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//by kmk [20190219] 측정된 PCD + Offset 값을 DB로 변경한다.
	//DB에 저장이 완료 되면 Offset 값은 0으로 바꿔준다.
	if (AfxMessageBox("Do you want to save scale factor?", MB_YESNO | MB_TOPMOST) == IDYES)
	{
		CString strOffset = _T("");
		OneAxis *pMotor = NULL;
		double dMeasSF = 0.0;
		int nSelection = m_cbRobotName.GetCurSel();
		int nHandSelection = m_cbHandName.GetCurSel();
		int nTestPPAxisX = nHandSelection == eTestPPLoadType ? eAxisX : eAxisX2;
		int nTestPPAxisZ = nHandSelection == eTestPPLoadType ? eAxisZ : eAxisZ2;

		switch (nSelection)
		{
			case ePcdBacklash_TrayLoadPP:
				{
					GetDlgItem(IDC_EDIT_OFFSET_SF_X)->GetWindowText(strOffset);
					g_VatPcd.trayloadpp.m_PcdOffset_XY.x = atof(strOffset);
					GetDlgItem(IDC_EDIT_OFFSET_SF_Y)->GetWindowText(strOffset);
					g_VatPcd.trayloadpp.m_PcdOffset_XY.y = atof(strOffset);

					if (m_nPcdAxis == eAxis_X)
					{
						pMotor = g_TaskTrayLoadPP.GetAxis(CTaskTrayLoadPP::eAxisX);
						dMeasSF = pMotor->GetScaleFactor() + g_VatPcd.trayloadpp.m_PcdOffset_XY.x;
						g_VatPcd.trayloadpp.m_PcdOffset_XY.x = 0.0;
					}
					else
					{
						pMotor = g_TaskTrayFeeder.GetAxis(CTaskTrayFeeder::eAxisY);
						dMeasSF = pMotor->GetScaleFactor() + g_VatPcd.trayloadpp.m_PcdOffset_XY.y;
						g_VatPcd.trayloadpp.m_PcdOffset_XY.y = 0.0;

					}
					SaveScaleFactor(pMotor->GetAxisNo(), dMeasSF);

					g_VatPcd.trayloadpp.Save();
					g_VatPcd.trayloadpp.Load();

				}break;
			case ePcdBacklash_TrayUnloadPP:
			{
				GetDlgItem(IDC_EDIT_OFFSET_SF_X)->GetWindowText(strOffset);
				g_VatPcd.trayunloadpp.m_PcdOffset_XY.x = atof(strOffset);
				GetDlgItem(IDC_EDIT_OFFSET_SF_Y)->GetWindowText(strOffset);
				g_VatPcd.trayunloadpp.m_PcdOffset_XY.y = atof(strOffset);

				if (m_nPcdAxis == eAxis_X)
				{
					pMotor = g_TaskTrayUnloadPP.GetAxis(CTaskTrayUnloadPP::eAxisX);
					dMeasSF = pMotor->GetScaleFactor() + g_VatPcd.trayunloadpp.m_PcdOffset_XY.x;
					g_VatPcd.trayunloadpp.m_PcdOffset_XY.x = 0.0;
				}
				else
				{
					pMotor = g_TaskTrayUnloadPP.GetAxis(CTaskTrayUnloadPP::eAxisY);
					dMeasSF = pMotor->GetScaleFactor() + g_VatPcd.trayunloadpp.m_PcdOffset_XY.y;
					g_VatPcd.trayunloadpp.m_PcdOffset_XY.y = 0.0;

				}
				SaveScaleFactor(pMotor->GetAxisNo(), dMeasSF);

				g_VatPcd.trayunloadpp.Save();
				g_VatPcd.trayunloadpp.Load();

			}break;
			case ePcdBacklash_TestPP1:
			case ePcdBacklash_TestPP2:
				{

					GetDlgItem(IDC_EDIT_OFFSET_SF_X)->GetWindowText(strOffset);
					g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_PcdOffset_XY.x = atof(strOffset);
					GetDlgItem(IDC_EDIT_OFFSET_SF_Y)->GetWindowText(strOffset);
					g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_PcdOffset_XY.y = atof(strOffset);

					if (m_nPcdAxis == eAxis_X)
					{					
						int nAxisIdx = nHandSelection == eTestPPLoadType ? CTaskTestPP::eAxisX1 : CTaskTestPP::eAxisX2;
						
						pMotor = g_TaskTestPP[nSelection - ePcdBacklash_TestPP1].GetAxis(nAxisIdx);
						dMeasSF = pMotor->GetScaleFactor() + g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_PcdOffset_XY.x;
						g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_PcdOffset_XY.x = 0.0;
					}
					else
					{
						pMotor = g_TaskTestPP[nSelection - ePcdBacklash_TestPP1].GetAxis(CTaskTestPP::eAxisY);
						dMeasSF = pMotor->GetScaleFactor() + g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_PcdOffset_XY.y;
						g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].m_PcdOffset_XY.y = 0.0;
					}
					SaveScaleFactor(pMotor->GetAxisNo(), dMeasSF);

					g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].Save(nHandSelection);
					g_VatPcd.testpp[nSelection - ePcdBacklash_TestPP1][nHandSelection].Load(nHandSelection);
				}
				break;
		}
	}
}


void CDlgPcdBacklash::OnBnClickedBtnSaveTargetPos()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	int nRet = AfxMessageBox("Do you want to save backlash position?", MB_YESNO | MB_TOPMOST);
	if (nRet == IDYES)
	{
		if (ChkBacklashPosInterlock() == TRUE)
		{
			SaveBacklashPosition();
		}
		else
		{
			AfxMessageBox("Please Check Your Target Position", MB_OK | MB_TOPMOST);
		}
		
	}
}


void CDlgPcdBacklash::OnBnClickedBtnBacklashReadEncoder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	double dCurPos_X = 0.0, dCurPos_Y = 0.0, dCurPos_Z = 0.0;
	CString strCurPos_X = _T(""), strCurPos_Y = _T(""), strCurPos_Z = _T("");

	int nRet = AfxMessageBox("Do you want to read encoder?", MB_YESNO | MB_TOPMOST);
	if (nRet == IDYES)
	{
		int nSelection = m_cbRobotName.GetCurSel();
		int nHandSelection = m_cbHandName.GetCurSel();
		int nTestPPAxisX = nHandSelection == eTestPPLoadType ? eAxisX : eAxisX2;
		int nTestPPAxisZ = nHandSelection == eTestPPLoadType ? eAxisZ : eAxisZ2;
		switch (nSelection)
		{
		case ePcdBacklash_TrayLoadPP:
		{
			g_Motor[m_vRobotNo[nSelection][eAxisX]]->GetCurrentPos(dCurPos_X);
			g_Motor[m_vRobotNo[nSelection][eAxisY]]->GetCurrentPos(dCurPos_Y);
			//g_Motor[m_vRobotNo[nSelection][eAxisZ]]->GetCurrentPos(dCurPos_Z);
			g_Motor[m_vRobotNo[nSelection][eAxisZ]]->GetCurrentPos(dCurPos_Z, _eBbxmnpAxis::eBMAxis_LOAD_TRAY_Picker07);
		}break;
		case ePcdBacklash_TrayUnloadPP:
		{
			g_Motor[m_vRobotNo[nSelection][eAxisX]]->GetCurrentPos(dCurPos_X);
			g_Motor[m_vRobotNo[nSelection][eAxisY]]->GetCurrentPos(dCurPos_Y);
			g_Motor[m_vRobotNo[nSelection][eAxisZ]]->GetCurrentPos(dCurPos_Z, _eBbxmnpAxis::eBMAxis_UNLOAD_TRAY_Picker07);
			//g_Motor[m_vRobotNo[nSelection][eAxisZ]]->GetCurrentPos(dCurPos_Z);
		}break;
		case ePcdBacklash_TestPP1:
		{
			g_Motor[m_vRobotNo[nSelection][nTestPPAxisX]]->GetCurrentPos(dCurPos_X);
			g_Motor[m_vRobotNo[nSelection][eAxisY]]->GetCurrentPos(dCurPos_Y);
			//g_Motor[m_vRobotNo[nSelection][nTestPPAxisZ]]->GetCurrentPos(dCurPos_Z);
			if(nHandSelection == eTestPPLoadType)
				g_Motor[m_vRobotNo[nSelection][nTestPPAxisZ]]->GetCurrentPos(dCurPos_Z, _eBbxmnpAxis::eBMAxis_LOAD_TESTPP_1_Picker03);
			else 
				g_Motor[m_vRobotNo[nSelection][nTestPPAxisZ]]->GetCurrentPos(dCurPos_Z, _eBbxmnpAxis::eBMAxis_UNLOAD_TESTPP_1_Picker02);
		}break;
		case ePcdBacklash_TestPP2:
		{
			g_Motor[m_vRobotNo[nSelection][nTestPPAxisX]]->GetCurrentPos(dCurPos_X);
			g_Motor[m_vRobotNo[nSelection][eAxisY]]->GetCurrentPos(dCurPos_Y);
			//g_Motor[m_vRobotNo[nSelection][nTestPPAxisZ]]->GetCurrentPos(dCurPos_Z);
			if (nHandSelection == eTestPPLoadType)
				g_Motor[m_vRobotNo[nSelection][nTestPPAxisZ]]->GetCurrentPos(dCurPos_Z, _eBbxmnpAxis::eBMAxis_LOAD_TESTPP_1_Picker02);
			else
				g_Motor[m_vRobotNo[nSelection][nTestPPAxisZ]]->GetCurrentPos(dCurPos_Z, _eBbxmnpAxis::eBMAxis_UNLOAD_TESTPP_1_Picker03);
		}break;
		default:
			break;
		}

		strCurPos_X.Format(_T("%.3f"), dCurPos_X / 1000);
		strCurPos_Y.Format(_T("%.3f"), dCurPos_Y / 1000);
		strCurPos_Z.Format(_T("%.3f"), dCurPos_Z / 1000);

		GetDlgItem(IDC_EDIT_BACKLASH_X)->SetWindowTextA(strCurPos_X);
		GetDlgItem(IDC_EDIT_BACKLASH_Y)->SetWindowTextA(strCurPos_Y);
		GetDlgItem(IDC_EDIT_BACKLASH_Z)->SetWindowTextA(strCurPos_Z);
	}
}





void CDlgPcdBacklash::OnBnClickedBtnTestPpSafetyZ()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
// 	CString strUpperZ = _T("");
// 	CString strLowerZ = _T("");
// 
// 	int nRet = AfxMessageBox("Do you want to save position z?", MB_YESNO | MB_TOPMOST);
// 	if (nRet == IDYES)
// 	{
// 		GetDlgItem(IDC_EDIT_SAFETY_UPPER)->GetWindowTextA(strUpperZ);
// 		GetDlgItem(IDC_EDIT_SAFETY_LOWER)->GetWindowTextA(strLowerZ);
// 
// 		int nSelection = m_cbRobotName.GetCurSel();
// 
// 		switch (nSelection)
// 		{
// 			case ePcdBacklash_TestPP1:
// 			case ePcdBacklash_TestPP2:
// 				{
// 					g_VatPcd.testpp[nSelection - 1].m_dUpperSafetyPos = atof(strUpperZ);
// 					g_VatPcd.testpp[nSelection - 1].m_dLowerSafetyPos = atof(strLowerZ);
// 					g_VatPcd.testpp[nSelection - 1].SafetyZAxisSave();
// 				}
// 				break;
// 		}
// 	}
}


void CDlgPcdBacklash::OnEnSetfocusEditPcdXaxisX()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	CString strVal = _T("");
	GetDlgItem(IDC_EDIT_PCD_XAxis_X)->GetWindowText(strVal);

	//sprintf_s(szVal, sizeof(szVal), "%0.3f", m_dTargetDvcThickness);
	char szMin[128] = { 0, }, szMax[128] = { 0, };
	int nSelection = m_cbRobotName.GetCurSel();
	sprintf_s(szVal, sizeof(szVal), "%s", strVal.GetString());

	sprintf_s(szMax, sizeof(szMax), "%.3f", g_Motor[m_vRobotNo[nSelection][eAxisX]]->GetSwLimitPosi() / 1000.0);
	sprintf_s(szMin, sizeof(szMin), "%.3f", g_Motor[m_vRobotNo[nSelection][eAxisX]]->GetSwLimitNega() / 1000.0);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "PCD Axis X Start Position", szMax, szMin);

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_PCD_XAxis_X)->SetWindowText(szRetVal);
	}
}


void CDlgPcdBacklash::OnEnSetfocusEditPcdXaxisY()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	CString strVal = _T("");
	GetDlgItem(IDC_EDIT_PCD_XAxis_Y)->GetWindowText(strVal);

	//sprintf_s(szVal, sizeof(szVal), "%0.3f", m_dTargetDvcThickness);
	char szMin[128] = { 0, }, szMax[128] = { 0, };
	int nSelection = m_cbRobotName.GetCurSel();
	sprintf_s(szVal, sizeof(szVal), "%s", strVal.GetString());

	sprintf_s(szMax, sizeof(szMax), "%.3f", g_Motor[m_vRobotNo[nSelection][eAxisY]]->GetSwLimitPosi() / 1000.0);
	sprintf_s(szMin, sizeof(szMin), "%.3f", g_Motor[m_vRobotNo[nSelection][eAxisY]]->GetSwLimitNega() / 1000.0);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "PCD Axis Y Start Position", szMax, szMin);

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_PCD_XAxis_Y)->SetWindowText(szRetVal);
	}
}


void CDlgPcdBacklash::OnEnSetfocusEditPcdXaxisZ()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	CString strVal = _T("");
	GetDlgItem(IDC_EDIT_PCD_XAxis_Z)->GetWindowText(strVal);

	//sprintf_s(szVal, sizeof(szVal), "%0.3f", m_dTargetDvcThickness);
	char szMin[128] = { 0, }, szMax[128] = { 0, };
	int nSelection = m_cbRobotName.GetCurSel();
	sprintf_s(szVal, sizeof(szVal), "%s", strVal.GetString());

	sprintf_s(szMax, sizeof(szMax), "%.3f", g_Motor[m_vRobotNo[nSelection][eAxisZ]]->GetSwLimitPosi() / 1000.0);
	sprintf_s(szMin, sizeof(szMin), "%.3f", g_Motor[m_vRobotNo[nSelection][eAxisZ]]->GetSwLimitNega() / 1000.0);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "PCD Axis Z Start Position", szMax, szMin);

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_PCD_XAxis_Z)->SetWindowText(szRetVal);
	}
}


void CDlgPcdBacklash::OnEnSetfocusEditPcdYaxisX()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	CString strVal = _T("");
	GetDlgItem(IDC_EDIT_PCD_YAxis_X)->GetWindowText(strVal);

	//sprintf_s(szVal, sizeof(szVal), "%0.3f", m_dTargetDvcThickness);
	char szMin[128] = { 0, }, szMax[128] = { 0, };
	int nSelection = m_cbRobotName.GetCurSel();
	sprintf_s(szVal, sizeof(szVal), "%s", strVal.GetString());

	sprintf_s(szMax, sizeof(szMax), "%.3f", g_Motor[m_vRobotNo[nSelection][eAxisX]]->GetSwLimitPosi() / 1000.0);
	sprintf_s(szMin, sizeof(szMin), "%.3f", g_Motor[m_vRobotNo[nSelection][eAxisX]]->GetSwLimitNega() / 1000.0);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "PCD Axis Y Start Position", szMax, szMin);

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_PCD_YAxis_X)->SetWindowText(szRetVal);
	}
}


void CDlgPcdBacklash::OnEnSetfocusEditPcdYaxisY()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	CString strVal = _T("");
	GetDlgItem(IDC_EDIT_PCD_YAxis_Y)->GetWindowText(strVal);

	//sprintf_s(szVal, sizeof(szVal), "%0.3f", m_dTargetDvcThickness);
	char szMin[128] = { 0, }, szMax[128] = { 0, };
	int nSelection = m_cbRobotName.GetCurSel();
	sprintf_s(szVal, sizeof(szVal), "%s", strVal.GetString());

	sprintf_s(szMax, sizeof(szMax), "%.3f", g_Motor[m_vRobotNo[nSelection][eAxisY]]->GetSwLimitPosi() / 1000.0);
	sprintf_s(szMin, sizeof(szMin), "%.3f", g_Motor[m_vRobotNo[nSelection][eAxisY]]->GetSwLimitNega() / 1000.0);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "PCD Axis Y Start Position", szMax, szMin);

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_PCD_YAxis_Y)->SetWindowText(szRetVal);
	}
}


void CDlgPcdBacklash::OnEnSetfocusEditPcdYaxisZ()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	CString strVal = _T("");
	GetDlgItem(IDC_EDIT_PCD_YAxis_Z)->GetWindowText(strVal);

	//sprintf_s(szVal, sizeof(szVal), "%0.3f", m_dTargetDvcThickness);
	char szMin[128] = { 0, }, szMax[128] = { 0, };
	int nSelection = m_cbRobotName.GetCurSel();
	sprintf_s(szVal, sizeof(szVal), "%s", strVal.GetString());

	sprintf_s(szMax, sizeof(szMax), "%.3f", g_Motor[m_vRobotNo[nSelection][eAxisZ]]->GetSwLimitPosi() / 1000.0);
	sprintf_s(szMin, sizeof(szMin), "%.3f", g_Motor[m_vRobotNo[nSelection][eAxisZ]]->GetSwLimitNega() / 1000.0);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "PCD Axis Y Start Position", szMax, szMin);

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_PCD_YAxis_Z)->SetWindowText(szRetVal);
	}
}


void CDlgPcdBacklash::OnEnSetfocusEditSafetyUpper()
{
// 	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
// 	char szVal[10] = { 0, };
// 	char szRetVal[64] = { 0, };
// 	CString strVal = _T("");
// 	GetDlgItem(IDC_EDIT_SAFETY_UPPER)->GetWindowText(strVal);
// 
// 	//sprintf_s(szVal, sizeof(szVal), "%0.3f", m_dTargetDvcThickness);
// 	char szMin[128] = { 0, }, szMax[128] = { 0, };
// 	int nSelection = m_cbRobotName.GetCurSel();
// 	sprintf_s(szVal, sizeof(szVal), "%s", strVal.GetString());
// 
// 	sprintf_s(szMax, sizeof(szMax), "%.3f", g_Motor[m_vRobotNo[nSelection] + 2]->GetSwLimitPosi() / 1000.0);
// 	sprintf_s(szMin, sizeof(szMin), "%.3f", g_Motor[m_vRobotNo[nSelection] + 2]->GetSwLimitNega() / 1000.0);
// 
// 	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "TEST PP Upper Safety Z Position", szMax, szMin);
// 
// 	if (nRet == RETURN_OK) {
// 		GetDlgItem(IDC_EDIT_SAFETY_UPPER)->SetWindowText(szRetVal);
// 	}
}


void CDlgPcdBacklash::OnEnSetfocusEditSafetyLower()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
// 	char szVal[10] = { 0, };
// 	char szRetVal[64] = { 0, };
// 	CString strVal = _T("");
// 	GetDlgItem(IDC_EDIT_SAFETY_LOWER)->GetWindowText(strVal);
// 
// 	//sprintf_s(szVal, sizeof(szVal), "%0.3f", m_dTargetDvcThickness);
// 	char szMin[128] = { 0, }, szMax[128] = { 0, };
// 	int nSelection = m_cbRobotName.GetCurSel();
// 	sprintf_s(szVal, sizeof(szVal), "%s", strVal.GetString());
// 
// 	sprintf_s(szMax, sizeof(szMax), "%.3f", g_Motor[m_vRobotNo[nSelection] + 2]->GetSwLimitPosi() / 1000.0);
// 	sprintf_s(szMin, sizeof(szMin), "%.3f", g_Motor[m_vRobotNo[nSelection] + 2]->GetSwLimitNega() / 1000.0);
// 
// 	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "TEST PP Upper Safety Z Position", szMax, szMin);
// 
// 	if (nRet == RETURN_OK) {
// 		GetDlgItem(IDC_EDIT_SAFETY_LOWER)->SetWindowText(szRetVal);
// 	}
}


void CDlgPcdBacklash::OnEnSetfocusEditBacklashX()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	CString strVal = _T("");
	GetDlgItem(IDC_EDIT_BACKLASH_X)->GetWindowText(strVal);

	//sprintf_s(szVal, sizeof(szVal), "%0.3f", m_dTargetDvcThickness);
	char szMin[128] = { 0, }, szMax[128] = { 0, };
	int nSelection = m_cbRobotName.GetCurSel();
	sprintf_s(szVal, sizeof(szVal), "%s", strVal.GetString());

	sprintf_s(szMax, sizeof(szMax), "%.3f", g_Motor[m_vRobotNo[nSelection][eAxisX]]->GetSwLimitPosi() / 1000.0);
	sprintf_s(szMin, sizeof(szMin), "%.3f", g_Motor[m_vRobotNo[nSelection][eAxisX]]->GetSwLimitNega() / 1000.0);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Backlash Axis X Target Position", szMax, szMin);

	if (nRet == RETURN_OK)
	{
		GetDlgItem(IDC_EDIT_BACKLASH_X)->SetWindowText(szRetVal);
		ChangedBacklashPosition(m_nBacklashAxis);

	}
}


void CDlgPcdBacklash::OnEnSetfocusEditBacklashY()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	CString strVal = _T("");
	GetDlgItem(IDC_EDIT_BACKLASH_Y)->GetWindowText(strVal);

	//sprintf_s(szVal, sizeof(szVal), "%0.3f", m_dTargetDvcThickness);
	char szMin[128] = { 0, }, szMax[128] = { 0, };
	int nSelection = m_cbRobotName.GetCurSel();
	sprintf_s(szVal, sizeof(szVal), "%s", strVal.GetString());

	sprintf_s(szMax, sizeof(szMax), "%.3f", g_Motor[m_vRobotNo[nSelection][eAxisY]]->GetSwLimitPosi() / 1000.0);
	sprintf_s(szMin, sizeof(szMin), "%.3f", g_Motor[m_vRobotNo[nSelection][eAxisY]]->GetSwLimitNega() / 1000.0);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Backlash Axis Y Target Position", szMax, szMin);

	if (nRet == RETURN_OK)
	{
		GetDlgItem(IDC_EDIT_BACKLASH_Y)->SetWindowText(szRetVal);
		ChangedBacklashPosition(m_nBacklashAxis);
	}
}


void CDlgPcdBacklash::OnEnSetfocusEditBacklashZ()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	CString strVal = _T("");
	GetDlgItem(IDC_EDIT_BACKLASH_Z)->GetWindowText(strVal);

	//sprintf_s(szVal, sizeof(szVal), "%0.3f", m_dTargetDvcThickness);
	char szMin[128] = { 0, }, szMax[128] = { 0, };
	int nSelection = m_cbRobotName.GetCurSel();
	sprintf_s(szVal, sizeof(szVal), "%s", strVal.GetString());

	sprintf_s(szMax, sizeof(szMax), "%.3f", g_Motor[m_vRobotNo[nSelection][eAxisZ]]->GetSwLimitPosi() / 1000.0);
	sprintf_s(szMin, sizeof(szMin), "%.3f", g_Motor[m_vRobotNo[nSelection][eAxisZ]]->GetSwLimitNega() / 1000.0);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Backlash Axis Z Target Position", szMax, szMin);

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_BACKLASH_Z)->SetWindowText(szRetVal);
		ChangedBacklashPosition(eAxisZ);
	}
}

void CDlgPcdBacklash::OnEnSetfocusEditBacklashRepeat()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	CString strVal = _T("");
	GetDlgItem(IDC_EDIT_BACKLASH_REPEAT)->GetWindowText(strVal);

	//sprintf_s(szVal, sizeof(szVal), "%0.3f", m_dTargetDvcThickness);
	char szMin[128] = { 0, }, szMax[128] = { 0, };
	int nSelection = m_cbRobotName.GetCurSel();
	sprintf_s(szVal, sizeof(szVal), "%s", strVal.GetString());

	sprintf_s(szMax, sizeof(szMax), "%d", 999);
	sprintf_s(szMin, sizeof(szMin), "%d", 1);

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Backlash Repeat Count", szMax, szMin);

	if (nRet == RETURN_OK) {
		GetDlgItem(IDC_EDIT_BACKLASH_REPEAT)->SetWindowText(szRetVal);
	}
}


void CDlgPcdBacklash::OnBnClickedPcdbacklasRepeat()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_cbRepeat.GetCheck())
	{
		GetDlgItem(IDC_EDIT_BACKLASH_REPEAT)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_EDIT_BACKLASH_REPEAT)->ShowWindow(SW_HIDE);
	}
}


void CDlgPcdBacklash::OnBnClickedBtnBacklashManualStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_bManualCmdExcute) {
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	int nRet = AfxMessageBox("Do you want to start the manual backlash measurement?", MB_YESNO | MB_TOPMOST);
	if (nRet == IDYES)
	{
		if (ChkTestPPSafetyPosValue() == TRUE && ChkBacklashPosInterlock() == TRUE)
		{
			SaveBacklashPosition();
			int nSelection = m_cbRobotName.GetCurSel();
			std::vector<CPoint> vPkr;
			g_pBbxmnpClient->MakeAllPicker(vPkr);
			switch (nSelection) {
				case ePcdBacklash_TrayLoadPP:
					{
						CTrayLoadPPCmd_VatBacklash* pSeqCmd = new CTrayLoadPPCmd_VatBacklash(this);

						int nSpeed = m_cbBacklashSpd.GetCurSel();

						if (nSpeed > 0) {
							pSeqCmd->m_qSpeed.push_back(nSpeed * 10);
						}
						else {
							pSeqCmd->m_qSpeed.push_back(5);
						}
						pSeqCmd->m_nLoopCnt = m_cbBacklashCnt.GetCurSel() + 1;
						pSeqCmd->m_nAxisNum = m_nBacklashAxis;
						pSeqCmd->m_bBacklashTest = FALSE;
						pSeqCmd->m_vSafetyAllPicker = vPkr;
						pSeqCmd->vPicker = g_pBbxmnpClient->VisionCamStandardPicker(CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_LOAD_TRAY);

						int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTrayLoadPP, pSeqCmd, eEqpCtrlMode_Manual);
					}break;
				case ePcdBacklash_TrayUnloadPP:
				{
					CTrayUnloadPPCmd_VatBacklash* pSeqCmd = new CTrayUnloadPPCmd_VatBacklash(this);

					int nSpeed = m_cbBacklashSpd.GetCurSel();

					if (nSpeed > 0) {
						pSeqCmd->m_qSpeed.push_back(nSpeed * 10);
					}
					else {
						pSeqCmd->m_qSpeed.push_back(5);
					}
					pSeqCmd->m_nLoopCnt = m_cbBacklashCnt.GetCurSel() + 1;
					pSeqCmd->m_nAxisNum = m_nBacklashAxis;
					pSeqCmd->m_bBacklashTest = FALSE;
					pSeqCmd->m_Cmd.vPicker = g_pBbxmnpClient->VisionCamStandardPicker(CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_UNLOAD_TRAY);
					pSeqCmd->m_vSafetyAllPicker = vPkr;
					int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTrayUnloadPP, pSeqCmd, eEqpCtrlMode_Manual);
				}break;
				case ePcdBacklash_TestPP1:
				case ePcdBacklash_TestPP2:
					{
						CTestPPCmd_VatBacklash* pSeqCmd = new CTestPPCmd_VatBacklash(nSelection - ePcdBacklash_TestPP1, this);

						int nSpeed = m_cbBacklashSpd.GetCurSel();

						if (nSpeed > 0) {
							pSeqCmd->m_qSpeed.push_back(nSpeed * 10);
						}
						else {
							pSeqCmd->m_qSpeed.push_back(5);
						}
					
						pSeqCmd->m_nLoopCnt = m_cbBacklashCnt.GetCurSel() + 1;
						pSeqCmd->m_nDistanceCnt = m_cbBacklashDis.GetCurSel() + 1;
						pSeqCmd->m_nAxisNum = m_nBacklashAxis;
						pSeqCmd->m_bBacklashTest = FALSE;
						pSeqCmd->m_nHandType = m_cbHandName.GetCurSel();
						CBbxmnpClient::_ePICK_MOTOR eMotorNum;
						switch (nSelection)
						{
						case ePcdBacklash_TestPP1: { eMotorNum = pSeqCmd->m_nHandType == eTestPPLoadType ? CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_LOAD_TEST_1 : CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_UNLOAD_TEST_1;}break;
						case ePcdBacklash_TestPP2: { eMotorNum = pSeqCmd->m_nHandType == eTestPPLoadType ? CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_LOAD_TEST_2 : CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_UNLOAD_TEST_2;}break;
						default:
							break;
						}
						pSeqCmd->m_vSafetyAllPicker = vPkr;
						pSeqCmd->m_vCmdPicker = g_pBbxmnpClient->VisionCamStandardPicker(eMotorNum);

						int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTestPP[nSelection - ePcdBacklash_TestPP1], pSeqCmd, eEqpCtrlMode_Manual);
					}break;
				}
		}
		else
		{
			AfxMessageBox("Please Check Test PP Safety Pos Z or Check Your Target Position", MB_OK | MB_TOPMOST);
		}
	}

}


void CDlgPcdBacklash::OnBnClickedBtnBacklashAutoStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_bManualCmdExcute) {
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	int nRet = AfxMessageBox("Do you want to start the auto backlash measurement?", MB_YESNO | MB_TOPMOST);
	if (nRet == IDYES)
	{
		if (ChkTestPPSafetyPosValue() == TRUE && ChkBacklashPosInterlock() == TRUE)
		{
			SaveBacklashPosition();
			int nSelection = m_cbRobotName.GetCurSel();
			std::vector<CPoint> vPkr;
			g_pBbxmnpClient->MakeAllPicker(vPkr);
			switch (nSelection) {
				case ePcdBacklash_TrayLoadPP:
					{
						CTrayLoadPPCmd_VatBacklash* pSeqCmd = new CTrayLoadPPCmd_VatBacklash(this);

						pSeqCmd->m_qSpeed.push_back(5);
						for (int i = eSpeed_10; i < eSpeed_MAX; i++) {
							pSeqCmd->m_qSpeed.push_back(i * 10);
						}

						pSeqCmd->m_nLoopCnt = m_cbBacklashCnt.GetCurSel() + 1;
						pSeqCmd->m_nAxisNum = m_nBacklashAxis;
						pSeqCmd->m_bBacklashTest = FALSE;
						pSeqCmd->m_vSafetyAllPicker = vPkr;
						SetBacklashRepeat(pSeqCmd->m_bRepeat, pSeqCmd->m_nRepeatCnt, pSeqCmd->m_nRepeatTempCnt);
						pSeqCmd->vPicker = g_pBbxmnpClient->VisionCamStandardPicker(CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_LOAD_TRAY);

						int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTrayLoadPP, pSeqCmd, eEqpCtrlMode_Manual);
				}break;
				case ePcdBacklash_TrayUnloadPP:
				{
					CTrayUnloadPPCmd_VatBacklash* pSeqCmd = new CTrayUnloadPPCmd_VatBacklash(this);

					pSeqCmd->m_qSpeed.push_back(5);
					for (int i = eSpeed_10; i < eSpeed_MAX; i++) {
						pSeqCmd->m_qSpeed.push_back(i * 10);
					}

					pSeqCmd->m_nLoopCnt = m_cbBacklashCnt.GetCurSel() + 1;
					pSeqCmd->m_nAxisNum = m_nBacklashAxis;
					pSeqCmd->m_bBacklashTest = FALSE;
					SetBacklashRepeat(pSeqCmd->m_bRepeat, pSeqCmd->m_nRepeatCnt, pSeqCmd->m_nRepeatTempCnt);
					pSeqCmd->m_vSafetyAllPicker = vPkr;
					pSeqCmd->m_Cmd.vPicker = g_pBbxmnpClient->VisionCamStandardPicker(CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_UNLOAD_TRAY);
			
					int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTrayUnloadPP, pSeqCmd, eEqpCtrlMode_Manual);
				}break;
				case ePcdBacklash_TestPP1:
				case ePcdBacklash_TestPP2:
					{
						CTestPPCmd_VatBacklash* pSeqCmd = new CTestPPCmd_VatBacklash(nSelection - ePcdBacklash_TestPP1, this);

						pSeqCmd->m_qSpeed.push_back(5);
						
						for (int i = eSpeed_10; i < eSpeed_MAX; i++) {
							pSeqCmd->m_qSpeed.push_back(i * 10);
						}
						
						pSeqCmd->m_nLoopCnt = m_cbBacklashCnt.GetCurSel() + 1;
						pSeqCmd->m_nDistanceCnt = m_cbBacklashDis.GetCurSel() + 1;
						pSeqCmd->m_nAxisNum = m_nBacklashAxis;
						pSeqCmd->m_bBacklashTest = FALSE;
						pSeqCmd->m_nHandType = m_cbHandName.GetCurSel();
						pSeqCmd->m_vSafetyAllPicker = vPkr;

						SetBacklashRepeat(pSeqCmd->m_bRepeat, pSeqCmd->m_nRepeatCnt, pSeqCmd->m_nRepeatTempCnt);

						CBbxmnpClient::_ePICK_MOTOR eMotorNum;
						switch (nSelection)
						{
						case ePcdBacklash_TestPP1: { eMotorNum = pSeqCmd->m_nHandType == eTestPPLoadType ? CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_LOAD_TEST_1 : CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_UNLOAD_TEST_1;}break;
						case ePcdBacklash_TestPP2: { eMotorNum = pSeqCmd->m_nHandType == eTestPPLoadType ? CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_LOAD_TEST_2 : CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_UNLOAD_TEST_2;}break;
						default:
							break;
						}

						pSeqCmd->m_vCmdPicker = g_pBbxmnpClient->VisionCamStandardPicker(eMotorNum);

						int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTestPP[nSelection - ePcdBacklash_TestPP1], pSeqCmd, eEqpCtrlMode_Manual);
						if (nErr == ERR_NO_ERROR)
						{
							int a = 1;
						}
					}break;
			}
		}
		else
		{
			AfxMessageBox("Please Check Test PP Safety Pos Z or Check Your Target Position", MB_OK | MB_TOPMOST);
		}
	}


}

void CDlgPcdBacklash::OnBnClickedBtnBacklashAutoTest()
{
	if (m_bManualCmdExcute) {
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	int nRet = AfxMessageBox("Do you want to start the backlash test?", MB_YESNO | MB_TOPMOST);
	if (nRet == IDYES)
	{
		if (ChkTestPPSafetyPosValue() == TRUE && ChkBacklashPosInterlock() == TRUE)
		{
			SaveBacklashPosition();
			SaveBacklashData();
			int nSelection = m_cbRobotName.GetCurSel();
			std::vector<CPoint> vPkr;
			g_pBbxmnpClient->MakeAllPicker(vPkr);
			switch (nSelection) {
				case ePcdBacklash_TrayLoadPP:
					{
						CTrayLoadPPCmd_VatBacklash* pSeqCmd = new CTrayLoadPPCmd_VatBacklash(this);

						int nSpeed = m_cbBacklashSpd.GetCurSel();

						pSeqCmd->m_qSpeed.push_back(5);
						for (int i = eSpeed_10; i < eSpeed_MAX; i++) {
							pSeqCmd->m_qSpeed.push_back(i * 10);
						}

						pSeqCmd->m_nLoopCnt = 1;
						pSeqCmd->m_nAxisNum = m_nBacklashAxis;
						pSeqCmd->m_bBacklashTest = TRUE;
						pSeqCmd->m_vSafetyAllPicker = vPkr;
						pSeqCmd->vPicker = g_pBbxmnpClient->VisionCamStandardPicker(CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_LOAD_TRAY);
						SetBacklashRepeat(pSeqCmd->m_bRepeat, pSeqCmd->m_nRepeatCnt, pSeqCmd->m_nRepeatTempCnt);

						int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTrayLoadPP, pSeqCmd, eEqpCtrlMode_Manual);
				}break;
				case ePcdBacklash_TrayUnloadPP:
				{
					CTrayUnloadPPCmd_VatBacklash* pSeqCmd = new CTrayUnloadPPCmd_VatBacklash(this);

					int nSpeed = m_cbBacklashSpd.GetCurSel();

					pSeqCmd->m_qSpeed.push_back(5);
					for (int i = eSpeed_10; i < eSpeed_MAX; i++) {
						pSeqCmd->m_qSpeed.push_back(i * 10);
					}

					pSeqCmd->m_nLoopCnt = 1;
					pSeqCmd->m_nAxisNum = m_nBacklashAxis;
					pSeqCmd->m_bBacklashTest = TRUE;
					pSeqCmd->m_vSafetyAllPicker = vPkr;
					pSeqCmd->m_Cmd.vPicker = g_pBbxmnpClient->VisionCamStandardPicker(CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_UNLOAD_TRAY);
					SetBacklashRepeat(pSeqCmd->m_bRepeat, pSeqCmd->m_nRepeatCnt, pSeqCmd->m_nRepeatTempCnt);

					int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTrayUnloadPP, pSeqCmd, eEqpCtrlMode_Manual);
				}break;
				case ePcdBacklash_TestPP1:
				case ePcdBacklash_TestPP2:
					{
						CTestPPCmd_VatBacklash* pSeqCmd = new CTestPPCmd_VatBacklash(nSelection - ePcdBacklash_TestPP1,this);

						int nSpeed = m_cbBacklashSpd.GetCurSel();

						pSeqCmd->m_qSpeed.push_back(5);
						for (int i = eSpeed_10; i < eSpeed_MAX; i++) {
							pSeqCmd->m_qSpeed.push_back(i * 10);
						}

						pSeqCmd->m_nLoopCnt = 1;
						pSeqCmd->m_nDistanceCnt = m_cbBacklashDis.GetCurSel() + 1;
						pSeqCmd->m_nAxisNum = m_nBacklashAxis;
						pSeqCmd->m_bBacklashTest = TRUE;
						pSeqCmd->m_vSafetyAllPicker = vPkr;

						SetBacklashRepeat(pSeqCmd->m_bRepeat, pSeqCmd->m_nRepeatCnt, pSeqCmd->m_nRepeatTempCnt);
						CBbxmnpClient::_ePICK_MOTOR eMotorNum;
						switch (nSelection)
						{
						case ePcdBacklash_TestPP1: { eMotorNum = pSeqCmd->m_nHandType == eTestPPLoadType ? CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_LOAD_TEST_1 : CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_UNLOAD_TEST_1;}break;
						case ePcdBacklash_TestPP2: { eMotorNum = pSeqCmd->m_nHandType == eTestPPLoadType ? CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_LOAD_TEST_2 : CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_UNLOAD_TEST_2;}break;
						default:
							break;
						}

						pSeqCmd->m_vCmdPicker = g_pBbxmnpClient->VisionCamStandardPicker(eMotorNum);

						int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTestPP[nSelection - ePcdBacklash_TestPP1], pSeqCmd, eEqpCtrlMode_Manual);
					}break;
			}
		}
		else
		{
			AfxMessageBox("Please Check Test PP Safety Pos Z or Check Your Target Position", MB_OK | MB_TOPMOST);
		}
	}

}

void CDlgPcdBacklash::OnBnClickedBtnBacklashSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nRet = AfxMessageBox("Do you want to Backlash Data Save?", MB_YESNO | MB_TOPMOST);

	if (nRet == IDYES)
	{
		SaveBacklashData();
	}
}

void CDlgPcdBacklash::OnBnClickedBtnVisionConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strIP = _T(""), strPort = _T("");
	m_edit_vision_ip.GetWindowText(strIP);
	m_edit_vision_port.GetWindowText(strPort);

	if (g_pVisionTestHand_1 != NULL)
	{
		if (g_pVisionTestHand_1->IsConnected() == TRUE) {
			g_pVisionTestHand_1->SocketClose();
		}
		else {
			g_pVisionTestHand_1->ChangeSetting(strIP.GetBuffer(0), atoi(strPort));
			g_pVisionTestHand_1->SocketConnect();
		}
	}
}

void CDlgPcdBacklash::OnBnClickedBtnVisionConnect2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strIP = _T(""), strPort = _T("");
	m_edit_vision_ip2.GetWindowText(strIP);
	m_edit_vision_port2.GetWindowText(strPort);

	if (g_pVisionTestHand_2 != NULL)
	{
		if (g_pVisionTestHand_2->IsConnected() == TRUE) {
			g_pVisionTestHand_2->SocketClose();
		}
		else {
			g_pVisionTestHand_2->ChangeSetting(strIP.GetBuffer(0), atoi(strPort));
			g_pVisionTestHand_2->SocketConnect();
		}
	}
}



void CDlgPcdBacklash::OnBnClickedBtnPcdbacklashServo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDlgManuMotor dlg;
	dlg.DoModal();
}

void CDlgPcdBacklash::ReportBacklashResult(int nAxis, int nSpeed)
{
	int nCol = 0;
	int nRow = 0;
	if (nSpeed == 5)
	{
		nCol = 1;
	}
	else
	{
		nCol = (nSpeed / 10) + 1;
	}
	if (nAxis == eAxisX)
	{
		nRow = 1;
	}
	else
	{
		nRow = 2;
	}
	ClearSpreadSelectBackColor(nRow);
	SetSpreadBackColor(nRow, nCol);
}

void CDlgPcdBacklash::ReportBacklashDone()
{
	ClearSpreadBackColor();
}

void CDlgPcdBacklash::OnBnClickedBtnAccuracyReadEncoder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
// 	double dCurPos_X = 0.0, dCurPos_Y = 0.0, dCurPos_Z = 0.0;
// 	CString strCurPos_X = _T(""), strCurPos_Y = _T(""), strCurPos_Z = _T("");
// 
// 	int nRet = AfxMessageBox("Do you want to read encoder?", MB_YESNO | MB_TOPMOST);
// 	if (nRet == IDYES)
// 	{
// 		int nSelection = m_cbRobotName.GetCurSel();
// 
// 		dCurPos_X = g_Motor[m_vRobotNo[nSelection]]->GetCurrentPos() / 1000.0;
// 		dCurPos_Y = g_Motor[m_vRobotNo[nSelection] + 1]->GetCurrentPos() / 1000.0;
// 		dCurPos_Z = g_Motor[m_vRobotNo[nSelection] + 2]->GetCurrentPos() / 1000.0;
// 
// 		strCurPos_X.Format(_T("%.3f"), dCurPos_X);
// 		strCurPos_Y.Format(_T("%.3f"), dCurPos_Y);
// 		strCurPos_Z.Format(_T("%.3f"), dCurPos_Z);
// 
// 		GetDlgItem(IDC_EDIT_ACCURACY_X)->SetWindowTextA(strCurPos_X);
// 		GetDlgItem(IDC_EDIT_ACCURACY_Y)->SetWindowTextA(strCurPos_Y);
// 		GetDlgItem(IDC_EDIT_ACCURACY_Z)->SetWindowTextA(strCurPos_Z);
// 	}
}


void CDlgPcdBacklash::OnBnClickedBtnRepeatabilityReadEncoder()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
// 	double dCurPos_X = 0.0, dCurPos_Y = 0.0, dCurPos_Z = 0.0;
// 	CString strCurPos_X = _T(""), strCurPos_Y = _T(""), strCurPos_Z = _T("");
// 
// 	int nRet = AfxMessageBox("Do you want to read encoder?", MB_YESNO | MB_TOPMOST);
// 	if (nRet == IDYES)
// 	{
// 		int nSelection = m_cbRobotName.GetCurSel();
// 
// 		dCurPos_X = g_Motor[m_vRobotNo[nSelection]]->GetCurrentPos() / 1000.0;
// 		dCurPos_Y = g_Motor[m_vRobotNo[nSelection] + 1]->GetCurrentPos() / 1000.0;
// 		dCurPos_Z = g_Motor[m_vRobotNo[nSelection] + 2]->GetCurrentPos() / 1000.0;
// 
// 		strCurPos_X.Format(_T("%.3f"), dCurPos_X);
// 		strCurPos_Y.Format(_T("%.3f"), dCurPos_Y);
// 		strCurPos_Z.Format(_T("%.3f"), dCurPos_Z);
// 
// 		GetDlgItem(IDC_EDIT_REPEATABILITY_X)->SetWindowTextA(strCurPos_X);
// 		GetDlgItem(IDC_EDIT_REPEATABILITY_Y)->SetWindowTextA(strCurPos_Y);
// 		GetDlgItem(IDC_EDIT_REPEATABILITY_Z)->SetWindowTextA(strCurPos_Z);
// 	}
}


void CDlgPcdBacklash::OnBnClickedBtnSaveRepeatabilityTargetPos()
{

	int nRet = AfxMessageBox("Do you want to save Repeatability position?", MB_YESNO | MB_TOPMOST);
	if (nRet == IDYES)
	{
		if (ChkBacklashPosInterlock() == TRUE)
		{
			SaveRepeatabilityPosition();
		}
		else
		{
			AfxMessageBox("Please Check Your Target Position", MB_OK | MB_TOPMOST);
		}

	}
}

void CDlgPcdBacklash::SaveRepeatabilityPosition()
{
// 	CString strStartPos_X = _T(""), strStartPos_Y = _T(""), strStartPos_Z = _T("");
// 
// 	GetDlgItem(IDC_EDIT_REPEATABILITY_X)->GetWindowTextA(strStartPos_X);
// 	GetDlgItem(IDC_EDIT_REPEATABILITY_Y)->GetWindowTextA(strStartPos_Y);
// 	GetDlgItem(IDC_EDIT_REPEATABILITY_Z)->GetWindowTextA(strStartPos_Z);
// 
// 	int nSelection = m_cbRobotName.GetCurSel();
// 
// 	switch (nSelection)
// 	{
// 	case ePcdBacklash_TrayLoadPP:
// 	{
// 		g_VatRepeatability.trayloadpp.m_RepeatabilityStartPos_XZ.x = atof(strStartPos_X) * 1000.0;
// 		g_VatRepeatability.trayloadpp.m_RepeatabilityStartPos_XZ.z = atof(strStartPos_Z) * 1000.0;
// 
// 		g_VatRepeatability.trayloadpp.Save();
// 	}
// 	break;
// 	case ePcdBacklash_TrayUnloadPP:
// 	{
// 		g_VatRepeatability.trayunloadpp.m_RepeatabilityStartPos_XYZ.x = atof(strStartPos_X) * 1000.0;
// 		g_VatRepeatability.trayunloadpp.m_RepeatabilityStartPos_XYZ.y = atof(strStartPos_Y) * 1000.0;
// 		g_VatRepeatability.trayunloadpp.m_RepeatabilityStartPos_XYZ.z = atof(strStartPos_Z) * 1000.0;
// 
// 		g_VatRepeatability.trayunloadpp.Save();
// 	}
// 	break;
// 	case ePcdBacklash_TestPP1:
// 	case ePcdBacklash_TestPP2:
// 	{
// 
// 		g_VatRepeatability.testpp[nSelection - 1].m_RepeatabilityStartPos_XYZ.x = atof(strStartPos_X) * 1000.0;
// 		g_VatRepeatability.testpp[nSelection - 1].m_RepeatabilityStartPos_XYZ.y = atof(strStartPos_Y) * 1000.0;
// 		g_VatRepeatability.testpp[nSelection - 1].m_RepeatabilityStartPos_XYZ.z = atof(strStartPos_Z) * 1000.0;
// 
// 		g_VatRepeatability.testpp[nSelection - 1].Save();
// 
// 	}
// 	break;
// 	}
}


void CDlgPcdBacklash::OnEnSetfocusEditRepeatabilityX()
{
// 	char szVal[10] = { 0, };
// 	char szRetVal[64] = { 0, };
// 	CString strVal = _T("");
// 	GetDlgItem(IDC_EDIT_REPEATABILITY_X)->GetWindowText(strVal);
// 
// 	char szMin[128] = { 0, }, szMax[128] = { 0, };
// 	int nSelection = m_cbRobotName.GetCurSel();
// 	sprintf_s(szVal, sizeof(szVal), "%s", strVal.GetString());
// 
// 	sprintf_s(szMax, sizeof(szMax), "%.3f", g_Motor[m_vRobotNo[nSelection]]->GetSwLimitPosi() / 1000.0);
// 	sprintf_s(szMin, sizeof(szMin), "%.3f", g_Motor[m_vRobotNo[nSelection]]->GetSwLimitNega() / 1000.0);
// 
// 	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Repeatability Axis X Start Position", szMax, szMin);
// 
// 	if (nRet == RETURN_OK) {
// 		GetDlgItem(IDC_EDIT_REPEATABILITY_X)->SetWindowText(szRetVal);
// 	}
}


void CDlgPcdBacklash::OnEnSetfocusEditRepeatabilityY()
{
// 	char szVal[10] = { 0, };
// 	char szRetVal[64] = { 0, };
// 	CString strVal = _T("");
// 	GetDlgItem(IDC_EDIT_REPEATABILITY_Y)->GetWindowText(strVal);
// 
// 	char szMin[128] = { 0, }, szMax[128] = { 0, };
// 	int nSelection = m_cbRobotName.GetCurSel();
// 	sprintf_s(szVal, sizeof(szVal), "%s", strVal.GetString());
// 
// 	sprintf_s(szMax, sizeof(szMax), "%.3f", g_Motor[m_vRobotNo[nSelection] + 1]->GetSwLimitPosi() / 1000.0);
// 	sprintf_s(szMin, sizeof(szMin), "%.3f", g_Motor[m_vRobotNo[nSelection] + 1]->GetSwLimitNega() / 1000.0);
// 
// 	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Repeatability Axis Y Start Position", szMax, szMin);
// 
// 	if (nRet == RETURN_OK) {
// 		GetDlgItem(IDC_EDIT_REPEATABILITY_Y)->SetWindowText(szRetVal);
// 	}
}


void CDlgPcdBacklash::OnEnSetfocusEditRepeatabilityZ()
{
// 	char szVal[10] = { 0, };
// 	char szRetVal[64] = { 0, };
// 	CString strVal = _T("");
// 	GetDlgItem(IDC_EDIT_REPEATABILITY_Z)->GetWindowText(strVal);
// 
// 	char szMin[128] = { 0, }, szMax[128] = { 0, };
// 	int nSelection = m_cbRobotName.GetCurSel();
// 	sprintf_s(szVal, sizeof(szVal), "%s", strVal.GetString());
// 
// 	sprintf_s(szMax, sizeof(szMax), "%.3f", g_Motor[m_vRobotNo[nSelection] + 2]->GetSwLimitPosi() / 1000.0);
// 	sprintf_s(szMin, sizeof(szMin), "%.3f", g_Motor[m_vRobotNo[nSelection] + 2]->GetSwLimitNega() / 1000.0);
// 
// 	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Repeatability Axis Z Start Position", szMax, szMin);
// 
// 	if (nRet == RETURN_OK) {
// 		GetDlgItem(IDC_EDIT_REPEATABILITY_Z)->SetWindowText(szRetVal);
// 	}
}

void CDlgPcdBacklash::OnBnClickedBtnRepeatabilityStart()
{
// 	int nRet = AfxMessageBox("Do you want to start the auto Repeatability measurement?", MB_YESNO | MB_TOPMOST);
// 	if (nRet == IDYES)
// 	{
// 		if (ChkTestPPSafetyPosValue() == TRUE && ChkBacklashPosInterlock() == TRUE)
// 		{
// 			SaveRepeatabilityPosition();
// 			int nSelection = m_cbRobotName.GetCurSel();
// 
// 			switch (nSelection) {
// 			case ePcdBacklash_TrayLoadPP:
// 			{
// 				
// 			}break;
// 			case ePcdBacklash_TestPP1:
// 			case ePcdBacklash_TestPP2:
// 			{
// 				CTestPPCmd_VatRepeatability* pSeqCmd = new CTestPPCmd_VatRepeatability(nSelection - 1);
// 
// 				int nSpeed = m_cbRepeatabilitySpd.GetCurSel();
// 
// 				if (nSpeed == 0) {
// 					pSeqCmd->m_qSpeed.push_back(5);
// 				}
// 				else {
// 					pSeqCmd->m_qSpeed.push_back(nSpeed * 10);
// 				}
// 
// 				pSeqCmd->m_nLoopCnt = m_cbRepeatabilityCnt.GetCurSel() + 1;
// 
// 				int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTestPP[nSelection - 1], pSeqCmd, eEqpCtrlMode_Manual);
// 			}break;
// 			}
// 		}
// 		else
// 		{
// 			AfxMessageBox("Please Check Test PP Safety Pos Z or Check Your Target Position", MB_OK | MB_TOPMOST);
// 		}
// 	}
}


void CDlgPcdBacklash::OnBnClickedBtnRepeatabilityAllStart()
{
// 	int nRet = AfxMessageBox("Do you want to start the auto Repeatability measurement?", MB_YESNO | MB_TOPMOST);
// 	if (nRet == IDYES)
// 	{
// 		if (ChkTestPPSafetyPosValue() == TRUE && ChkBacklashPosInterlock() == TRUE)
// 		{
// 			SaveRepeatabilityPosition();
// 			int nSelection = m_cbRobotName.GetCurSel();
// 
// 			switch (nSelection) {
// 			case ePcdBacklash_TrayPP:
// 			{
// 
// 			}break;
// 			case ePcdBacklash_TestPP1:
// 			case ePcdBacklash_TestPP2:
// 			{
// 				CTestPPCmd_VatRepeatability* pSeqCmd = new CTestPPCmd_VatRepeatability(nSelection - 1);
// 
// 				pSeqCmd->m_qSpeed.push_back(5);
// 				for (int i = 0; i < 10; i++) {
// 					pSeqCmd->m_qSpeed.push_back( (i+1) * 10);
// 				}
// 				
// 				pSeqCmd->m_nLoopCnt = m_cbRepeatabilityCnt.GetCurSel() + 1;
// 
// 				int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTestPP[nSelection - 1], pSeqCmd, eEqpCtrlMode_Manual);
// 			}break;
// 			}
// 		}
// 		else
// 		{
// 			AfxMessageBox("Please Check Test PP Safety Pos Z or Check Your Target Position", MB_OK | MB_TOPMOST);
// 		}
// 	}
}



void CDlgPcdBacklash::OnBnClickedBtnSaveAccuracyTargetPos()
{
	int nRet = AfxMessageBox("Do you want to save Repeatability position?", MB_YESNO | MB_TOPMOST);
	if (nRet == IDYES)
	{
		if (ChkBacklashPosInterlock() == TRUE)
		{
			SaveAccuracyPosition();
		}
		else
		{
			AfxMessageBox("Please Check Your Target Position", MB_OK | MB_TOPMOST);
		}

	}
}

void CDlgPcdBacklash::SaveAccuracyPosition()
{
// 	CString strStartPos_X = _T(""), strStartPos_Y = _T(""), strStartPos_Z = _T("");
// 
// 	GetDlgItem(IDC_EDIT_ACCURACY_X)->GetWindowTextA(strStartPos_X);
// 	GetDlgItem(IDC_EDIT_ACCURACY_Y)->GetWindowTextA(strStartPos_Y);
// 	GetDlgItem(IDC_EDIT_ACCURACY_Z)->GetWindowTextA(strStartPos_Z);
// 
// 	int nSelection = m_cbRobotName.GetCurSel();
// 
// 	switch (nSelection)
// 	{
// 	case ePcdBacklash_TrayPP:
// 	{
// 		g_VatAccuracy.traypp.m_AccuracyStartPos_XYZ.x = atof(strStartPos_X) * 1000.0;
// 		g_VatAccuracy.traypp.m_AccuracyStartPos_XYZ.y = atof(strStartPos_Y) * 1000.0;
// 		g_VatAccuracy.traypp.m_AccuracyStartPos_XYZ.z = atof(strStartPos_Z) * 1000.0;
// 
// 		g_VatAccuracy.traypp.Save();
// 	}
// 	break;
// 	case ePcdBacklash_TestPP1:
// 	case ePcdBacklash_TestPP2:
// 	{
// 
// 		g_VatAccuracy.testpp[nSelection - 1].m_AccuracyStartPos_XYZ.x = atof(strStartPos_X) * 1000.0;
// 		g_VatAccuracy.testpp[nSelection - 1].m_AccuracyStartPos_XYZ.y = atof(strStartPos_Y) * 1000.0;
// 		g_VatAccuracy.testpp[nSelection - 1].m_AccuracyStartPos_XYZ.z = atof(strStartPos_Z) * 1000.0;
// 
// 		g_VatAccuracy.testpp[nSelection - 1].Save();
// 	}
// 	break;
// 	}
}

void CDlgPcdBacklash::OnEnSetfocusEditAccuracyX()
{
// 	char szVal[10] = { 0, };
// 	char szRetVal[64] = { 0, };
// 	CString strVal = _T("");
// 	GetDlgItem(IDC_EDIT_ACCURACY_X)->GetWindowText(strVal);
// 
// 	char szMin[128] = { 0, }, szMax[128] = { 0, };
// 	int nSelection = m_cbRobotName.GetCurSel();
// 	sprintf_s(szVal, sizeof(szVal), "%s", strVal.GetString());
// 
// 	sprintf_s(szMax, sizeof(szMax), "%.3f", g_Motor[m_vRobotNo[nSelection]]->GetSwLimitPosi() / 1000.0);
// 	sprintf_s(szMin, sizeof(szMin), "%.3f", g_Motor[m_vRobotNo[nSelection]]->GetSwLimitNega() / 1000.0);
// 
// 	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Accuracy Axis X Start Position", szMax, szMin);
// 
// 	if (nRet == RETURN_OK) {
// 		GetDlgItem(IDC_EDIT_ACCURACY_X)->SetWindowText(szRetVal);
// 	}
}


void CDlgPcdBacklash::OnEnSetfocusEditAccuracyY()
{
// 	char szVal[10] = { 0, };
// 	char szRetVal[64] = { 0, };
// 	CString strVal = _T("");
// 	GetDlgItem(IDC_EDIT_ACCURACY_Y)->GetWindowText(strVal);
// 
// 	char szMin[128] = { 0, }, szMax[128] = { 0, };
// 	int nSelection = m_cbRobotName.GetCurSel();
// 	sprintf_s(szVal, sizeof(szVal), "%s", strVal.GetString());
// 
// 	sprintf_s(szMax, sizeof(szMax), "%.3f", g_Motor[m_vRobotNo[nSelection] + 1]->GetSwLimitPosi() / 1000.0);
// 	sprintf_s(szMin, sizeof(szMin), "%.3f", g_Motor[m_vRobotNo[nSelection] + 1]->GetSwLimitNega() / 1000.0);
// 
// 	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Accuracy Axis Y Start Position", szMax, szMin);
// 
// 	if (nRet == RETURN_OK) {
// 		GetDlgItem(IDC_EDIT_ACCURACY_Y)->SetWindowText(szRetVal);
//	}
}


void CDlgPcdBacklash::OnEnSetfocusEditAccuracyZ()
{
// 	char szVal[10] = { 0, };
// 	char szRetVal[64] = { 0, };
// 	CString strVal = _T("");
// 	GetDlgItem(IDC_EDIT_ACCURACY_Z)->GetWindowText(strVal);
// 
// 	char szMin[128] = { 0, }, szMax[128] = { 0, };
// 	int nSelection = m_cbRobotName.GetCurSel();
// 	sprintf_s(szVal, sizeof(szVal), "%s", strVal.GetString());
// 
// 	sprintf_s(szMax, sizeof(szMax), "%.3f", g_Motor[m_vRobotNo[nSelection] + 2]->GetSwLimitPosi() / 1000.0);
// 	sprintf_s(szMin, sizeof(szMin), "%.3f", g_Motor[m_vRobotNo[nSelection] + 2]->GetSwLimitNega() / 1000.0);
// 
// 	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Accuracy Axis Z Start Position", szMax, szMin);
// 
// 	if (nRet == RETURN_OK) {
// 		GetDlgItem(IDC_EDIT_ACCURACY_Z)->SetWindowText(szRetVal);
// 	}
}

void CDlgPcdBacklash::OnBnClickedBtnAccuracyStart()
{
	//Position Interlock 필요

// 	int nRet = AfxMessageBox("Do you want to start the auto Acuuracy measurement?", MB_YESNO | MB_TOPMOST);
// 	if (nRet == IDYES)
// 	{
// 		if (ChkTestPPSafetyPosValue() == TRUE && AccuracyPositionInterlock() == TRUE)
// 		{
// 			SaveAccuracyPosition();
// 			UpdateData(TRUE);
// 
// 			int nSelection = m_cbRobotName.GetCurSel();
// 
// 			switch (nSelection) {
// 			case ePcdBacklash_TrayPP:
// 			{
// 
// 			}break;
// 			case ePcdBacklash_TestPP1:
// 			case ePcdBacklash_TestPP2:
// 			{
// 				CTestPPCmd_VatAccuracy* pSeqCmd = new CTestPPCmd_VatAccuracy(nSelection - 1);
// 
// 				int nSpeed = m_cbAccuracySpd.GetCurSel();
// 
// 				if (nSpeed == 0) {
// 					pSeqCmd->m_qSpeed.push_back(5);
// 				}
// 				else {
// 					pSeqCmd->m_qSpeed.push_back(nSpeed * 10);
// 				}
// 
// 				pSeqCmd->m_nLoopCnt = m_cbAccuracyCnt.GetCurSel() + 1;
// 				pSeqCmd->m_nDistance = ((m_cbAccuracyDis.GetCurSel() + 1) * 10) * 1000.0;
// 				pSeqCmd->m_nAxisNum = m_nAccuracyAxis;
// 
// 				int nErr = g_TaskSystemCtrl.SendCommand(g_TaskTestPP[nSelection - 1], pSeqCmd, eEqpCtrlMode_Manual);
// 			}break;
// 			}
// 		}
// 		else
// 		{
// 			AfxMessageBox("Please Check Your Target Position, Limit Position exceed", MB_OK | MB_TOPMOST);
// 		}
// 	}
}

BOOL CDlgPcdBacklash::AccuracyPositionInterlock()
{
// 	CString strVal = _T("");
// 	double dPosition = 0.0;
// 	double dLimitPosi = 0.0, dLimitNega = 0.0;
// 
// 	int nSelection = m_cbRobotName.GetCurSel();
// 
// 	if (m_nAccuracyAxis == eAxisX) {
// 		GetDlgItem(IDC_EDIT_ACCURACY_X)->GetWindowText(strVal);
// 		dPosition = atof(strVal);
// 		dLimitPosi = g_Motor[m_vRobotNo[nSelection]]->GetSwLimitPosi() / 1000.0;
// 		dLimitNega = g_Motor[m_vRobotNo[nSelection]]->GetSwLimitNega() / 1000.0;
// 	}
// 	else {
// 		GetDlgItem(IDC_EDIT_ACCURACY_Y)->GetWindowText(strVal);
// 		dPosition = atof(strVal);
// 		dLimitPosi = g_Motor[m_vRobotNo[nSelection] + 1]->GetSwLimitPosi() / 1000.0;
// 		dLimitNega = g_Motor[m_vRobotNo[nSelection] + 1]->GetSwLimitNega() / 1000.0;
// 	}
// 	
// 	dPosition += (m_cbAccuracyDis.GetCurSel() + 1) * 10;
// 
// 	if (dLimitPosi > dPosition && dPosition > dLimitNega) {
// 		return TRUE;
// 	}
 	return FALSE;
}


void CDlgPcdBacklash::OnCbnSelchangeBacklashDistance()
{
	ChangedBacklashPosition(m_nBacklashAxis);
}


void CDlgPcdBacklash::OnCbnSelchangeBacklashCnt()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlgPcdBacklash::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bManualCmdExcute == TRUE){
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	int nRet = AfxMessageBox("Do you want to Backlash Data Save before going out?", MB_YESNO | MB_TOPMOST);

	if (nRet == IDYES)
	{
		SaveBacklashData();
	}

	CDialogEx::OnClose();
	CDialogEx::OnCancel();
}


void CDlgPcdBacklash::OnCbnSelchangeCbHandChoose()
{
	UpdateOffsetData();
	UpdateTeachingData();
	UpdateTestPpSafetyData();
	HandSelectUIShowHide();
	int nAxis = (m_nPcdAxis == eAxisX) ? IDC_PCD_AXIS_X : IDC_PCD_AXIS_Y;
	OnRadioButtonPcdAxisChanged(nAxis);
//	UnitSelectUIShowHide();
}

void CDlgPcdBacklash::HandSelectUIShowHide(BOOL bOnTimer)
{
	int nSelection = m_cbRobotName.GetCurSel();
	int nHandSelection = m_cbHandName.GetCurSel();
	switch (nSelection)
	{
		case ePcdBacklash_TrayLoadPP:
		{			
			GetDlgItem(IDC_SSPANEL_MANUAL_TITLE18)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CB_HAND_CHOOSE)->ShowWindow(SW_HIDE);
			SpreadSheetUIShowHide(TRUE,TRUE);
			//SetSpreaddisableBackColor();
			ClearSpreadBackColor(bOnTimer);

		}break;
		case ePcdBacklash_TrayUnloadPP:
		{	
			GetDlgItem(IDC_SSPANEL_MANUAL_TITLE18)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CB_HAND_CHOOSE)->ShowWindow(SW_HIDE);
			SpreadSheetUIShowHide(TRUE);
			ClearSpreadBackColor(bOnTimer);
			
		}break;
		case ePcdBacklash_TestPP1:
		case ePcdBacklash_TestPP2:
		{
			if (nHandSelection == eTestPPLoadType)
			{
				SpreadSheetUIShowHide(TRUE);
				ClearSpreadBackColor(bOnTimer);
			}
			else {
				SpreadSheetUIShowHide(FALSE);
				SetSpreaddisableBackColor();
			}
			GetDlgItem(IDC_SSPANEL_MANUAL_TITLE18)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_CB_HAND_CHOOSE)->ShowWindow(SW_SHOW);
		}break;
	}
}

// void CDlgPcdBacklash::UnitSelectUIShowHide()
// {
// 	int nSelection = m_cbRobotName.GetCurSel();
// 	int nHandSelection = m_cbHandName.GetCurSel();
// 	switch (nSelection)
// 	{
// 	case ePcdBacklash_TrayLoadPP:
// 	{
// 		SpreadSheetUIShowHide(FALSE);
// 	
// 	}break;
// 	case ePcdBacklash_TrayUnloadPP:
// 	{
// 		SpreadSheetUIShowHide(TRUE);
// 	}break;
// 	case ePcdBacklash_TestPP1:
// 	case ePcdBacklash_TestPP2:
// 	{
// 		if (nHandSelection == eTestPPLoadType)
// 		{
// 			SpreadSheetUIShowHide(TRUE);
// 		}
// 		else{
// 			SpreadSheetUIShowHide(FALSE);
// 		}
// 
// 
// 	}break;
// 	}
// }


void CDlgPcdBacklash::SpreadSheetUIShowHide(BOOL bShow, BOOL bTrayLoadPP )
{

	if (bTrayLoadPP == TRUE)
	{
		GetDlgItem(IDC_STATIC_TEXT57)->SetWindowText("Feeder Y");
		GetDlgItem(IDC_STATIC_TEXT89)->SetWindowText("F_Y");
		GetDlgItem(IDC_STATIC_TEXT83)->SetWindowText("F_Y");
		GetDlgItem(IDC_STATIC_TEXT86)->SetWindowText("F_Y");
		GetDlgItem(IDC_STATIC_TEXT92)->SetWindowText("F_Y");
		
		GetDlgItem(IDC_PCD_AXIS_Y)->SetWindowText("Feeder Y");
		GetDlgItem(IDC_BACKLASH_AXIS_Y)->SetWindowText("Feeder Y");
	}
	else
	{
		GetDlgItem(IDC_STATIC_TEXT57)->SetWindowText("Y");
		GetDlgItem(IDC_STATIC_TEXT89)->SetWindowText("Y");
		GetDlgItem(IDC_STATIC_TEXT83)->SetWindowText("Y");
		GetDlgItem(IDC_STATIC_TEXT86)->SetWindowText("Y");
		GetDlgItem(IDC_STATIC_TEXT92)->SetWindowText("Y");

		GetDlgItem(IDC_PCD_AXIS_Y)->SetWindowText("Axis Y");
		GetDlgItem(IDC_BACKLASH_AXIS_Y)->SetWindowText("Axis Y");
	}
	if (bShow == TRUE) {
		GetDlgItem(IDC_PCD_AXIS_Y)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_PCD_YAxis_X)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_PCD_YAxis_Y)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_PCD_YAxis_Z)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TEXT59)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TEXT96)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TEXT79)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_PCD_START_Y)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_BTN_PCD_Y_READ_ENCODER)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_PCD_Y_SAVE_POS)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_EDIT_PCD_XAxis_Y)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TEXT57)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_BACKLASH_AXIS_Y)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_TEXT83)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_CURRENT_SF_Y)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_OFFSET_SF_Y)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_EDIT_BACKLASH_Y)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_TEXT89)->ShowWindow(SW_SHOW);
	}
	else {
		GetDlgItem(IDC_PCD_AXIS_Y)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PCD_YAxis_X)->ShowWindow(SW_HIDE);
	
		GetDlgItem(IDC_EDIT_PCD_YAxis_Y)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PCD_YAxis_Z)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TEXT59)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TEXT96)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TEXT79)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_PCD_START_Y)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_BTN_PCD_Y_READ_ENCODER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BTN_PCD_Y_SAVE_POS)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_BACKLASH_AXIS_Y)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_TEXT83)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_CURRENT_SF_Y)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_OFFSET_SF_Y)->ShowWindow(SW_HIDE);


	}
		
}




void CDlgPcdBacklash::OnBnClickedButtonManuProcCmdSkip()
{
	CBaseTaskManager::SendProcessCommand(base_seq_cmd::eProcCmd_Skip, eEqpCtrlMode_Manual, NULL);
}


void CDlgPcdBacklash::OnBnClickedButtonManuProcCmdStop()
{
	CBaseTaskManager::SendProcessCommand(base_seq_cmd::eProcCmd_Stop, eEqpCtrlMode_Manual, NULL);
}


void CDlgPcdBacklash::OnBnClickedCancel3()
{
	CDialogEx::OnCancel();
}
