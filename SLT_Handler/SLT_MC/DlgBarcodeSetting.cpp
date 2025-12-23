// DlgBarcodeSetting.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "DlgBarcodeSetting.h"
#include "afxdialogex.h"

#include "BaseTaskManager.h"
#include "DlgAxisActionMove.h"
#include "DlgCommand_LoadTbl.h"
#include "DlgManuMotor.h"
// DlgBarcodeSetting 대화 상자입니다.

using namespace base_seq_cmd;

IMPLEMENT_DYNAMIC(DlgBarcodeSetting, CDialogEx)

DlgBarcodeSetting::DlgBarcodeSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_BARCODE_SETTING, pParent)
{
	m_nPort = 0;
	m_bManualCmdExcute = FALSE;
}

DlgBarcodeSetting::~DlgBarcodeSetting()
{
}

void DlgBarcodeSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);

	DDX_Control(pDX, IDC_BUTTON_MANU_TRIGGER_CMD_SKIP, m_btnSkip);
	DDX_Control(pDX, IDC_BUTTON_MANU_TRIGGER_CMD_STOP, m_btnStop);


	DDX_Control(pDX, IDC_TRIGGER_COM_STATUS, m_StaticComStatus);

	DDX_Control(pDX, IDC_BTN_BARCODE_SETTING_TBL_2_MANU_MOVE, m_btnLDTable1);
	DDX_Control(pDX, IDC_BTN_BARCODE_SETTING_TBL_3_MANU_MOVE, m_btnLDTable2);

	//=======================================================================Trigger Param
	DDX_Control(pDX, IDC_STATIC_TRIGGER1_COUNT, m_StaticTrigger1Cnt);
	DDX_Control(pDX, IDC_STATIC_TRIGGER2_COUNT, m_StaticTrigger2Cnt);

	DDX_Control(pDX, IDC_STATIC_TRIGGER1_POSITION, m_StaticTrigger1Pos);
	DDX_Control(pDX, IDC_STATIC_TRIGGER2_POSITION, m_StaticTrigger2Pos);

	DDX_Control(pDX, IDC_STATIC_TRIGGER1_RETRY_CNT, m_StaticTrigger1RetryCnt);
	DDX_Control(pDX, IDC_STATIC_TRIGGER2_RETRY_CNT, m_StaticTrigger2RetryCnt);

	//=======================================================================Trigger Board Set Param
	DDX_Control(pDX, IDC_TRIGGER_RESOLUTION, m_StaticResolution);
	DDX_Control(pDX, IDC_TRIGGER_INTERVAL, m_StaticInterval);
	DDX_Control(pDX, IDC_TRIGGER_DIRECTION, m_StaticDirection);

	DDX_Control(pDX, IDC_TRIGGER_BOARD_TYPE, m_StaticBoardType);
	DDX_Control(pDX, IDC_TRIGGER_CORRECTION, m_StaticCorrection);
	DDX_Control(pDX, IDC_TRIGGER_BOARD_TYPE2, m_StaticBoardType2);
	DDX_Control(pDX, IDC_TRIGGER_CORRECTION2, m_StaticCorrection2);
	
	DDX_Control(pDX, IDC_TRIGGER_TBL1_START_POS, m_StaticTbl1_StartPos);
	DDX_Control(pDX, IDC_TRIGGER_TBL1_END_POS, m_StaticTbl1_EndPos);
	DDX_Control(pDX, IDC_TRIGGER_TBL2_START_POS, m_StaticTbl2_StartPos);
	DDX_Control(pDX, IDC_TRIGGER_TBL2_END_POS, m_StaticTbl2_EndPos);

	//ServoMotor
	DDX_Control(pDX, IDC_BUTTON_TRIGGER_SET_SERVO_MOTOR, m_btnServoMotor);
}


BEGIN_MESSAGE_MAP(DlgBarcodeSetting, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_BARCODE_SETTING_TBL_2_MANU_MOVE, &DlgBarcodeSetting::OnBnClickedBtnBarcodeSettingTbl2ManuMove)
	ON_BN_CLICKED(IDC_BTN_BARCODE_SETTING_TBL_3_MANU_MOVE, &DlgBarcodeSetting::OnBnClickedBtnBarcodeSettingTbl3ManuMove)
	ON_BN_CLICKED(IDC_BTN_TRIGGER1_CLEAR, &DlgBarcodeSetting::OnBnClickedBtnTrigger1Clear)
	ON_BN_CLICKED(IDC_BTN_TRIGGER2_CLEAR, &DlgBarcodeSetting::OnBnClickedBtnTrigger2Clear)
	ON_BN_CLICKED(IDC_BUTTON_MANU_TRIGGER_CMD_SKIP, &DlgBarcodeSetting::OnBnClickedButtonManuTriggerCmdSkip)
	ON_BN_CLICKED(IDC_BUTTON_MANU_TRIGGER_CMD_STOP, &DlgBarcodeSetting::OnBnClickedButtonManuTriggerCmdStop)
	ON_BN_CLICKED(IDC_BTN_TRIGGER1_RETRY_CNT_CLEAR, &DlgBarcodeSetting::OnBnClickedBtnTrigger1RetryCntClear)
	ON_BN_CLICKED(IDC_BTN_TRIGGER2_RETRY_CNT_CLEAR, &DlgBarcodeSetting::OnBnClickedBtnTrigger2RetryCntClear)
	ON_STN_CLICKED(IDC_TRIGGER_BOARD_TYPE, &DlgBarcodeSetting::OnStnClickedTriggerBoardType)
	ON_STN_CLICKED(IDC_TRIGGER_CORRECTION, &DlgBarcodeSetting::OnStnClickedTriggerCorrection)
	ON_STN_CLICKED(IDC_TRIGGER_BOARD_TYPE2, &DlgBarcodeSetting::OnStnClickedTriggerBoardType2)
	ON_STN_CLICKED(IDC_TRIGGER_CORRECTION2, &DlgBarcodeSetting::OnStnClickedTriggerCorrection2)
	ON_BN_CLICKED(IDC_BUTTON_LED_1_ON, &DlgBarcodeSetting::OnBnClickedButtonLed1On)
	ON_BN_CLICKED(IDC_BUTTON_LED_1_OFF, &DlgBarcodeSetting::OnBnClickedButtonLed1Off)
	ON_BN_CLICKED(IDC_BUTTON_LED_2_ON, &DlgBarcodeSetting::OnBnClickedButtonLed2On)
	ON_BN_CLICKED(IDC_BUTTON_LED_2_OFF, &DlgBarcodeSetting::OnBnClickedButtonLed2Off)
	ON_BN_CLICKED(IDC_BUTTON_TRIGGER_SET_SERVO_MOTOR, &DlgBarcodeSetting::OnBnClickedButtonTriggerSetServoMotor)
	ON_BN_CLICKED(IDC_BTN_TRIGGER_POS1_CLEAR, &DlgBarcodeSetting::OnBnClickedBtnTriggerPos1Clear)
	ON_BN_CLICKED(IDC_BTN_TRIGGER_POS2_CLEAR, &DlgBarcodeSetting::OnBnClickedBtnTriggerPos2Clear)
END_MESSAGE_MAP()


// DlgBarcodeSetting 메시지 처리기입니다.

BOOL DlgBarcodeSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	g_DMCont.m_dmEQP.SB(BDM0_OPEN_SLT_UI_DLG_HISTORY, TRUE);

	//====================================================================================Cancel
	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	//=====================================================================================Manual Cmd Stop & Skip
	CFont fontCmdStatus;
	fontCmdStatus.CreatePointFont(100, "Arial");

	m_btnSkip.SetFont(&fontCmdStatus);
	m_btnSkip.SetWindowText(_T("SKIP"));
	m_btnSkip.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));

	m_btnStop.SetFont(&fontCmdStatus);
	m_btnStop.SetWindowText(_T("STOP"));
	m_btnStop.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	//====================================================================================Comport
	char szDriverFileName[MAX_PATH] = { 0, };
	char szTemp[512] = { 0, };
	::GetCurrentDirectory(sizeof(szTemp), szTemp);
	sprintf_s(szDriverFileName, sizeof(szDriverFileName), "%s\\sys_data\\%s", szTemp, SZ_DRIVER_FILE_NAME);

	CConfigData ConfigData(szDriverFileName);
	m_nPort = ConfigData.GetInt("TRIGGER CONFIG", "PORT NO", "0");

	m_StaticComStatus.m_fnSetFont("Arial");
	m_StaticComStatus.SetTextSize(15);
	m_StaticComStatus.SetBackColor(COLOR_NOMAL);

	//==================================================================Manual Button
	m_cBmpLDTable1.LoadBitmap(IDB_LOADTABLE_02);
	m_btnLDTable1.SetBitmap(m_cBmpLDTable1);

	m_cBmpLDTable2.LoadBitmap(IDB_LOADTABLE_03);
	m_btnLDTable2.SetBitmap(m_cBmpLDTable2);


	//==================================================================Trigger Parameter
	InitTriggerParam();


	//==================================================================Trigger Board Set Parameter
	InitTriggerBoardSetParam();

	//상시 0으로 초기화.
	g_DMCont.m_dmShuttle.SN(NDM3_TRIGGER_BOARD_TYPE, 0);
	g_DMCont.m_dmShuttle.SN(NDM3_TRIGGER_VIRTUAL_ENCODER_FREQUENCY, 0); //virual일 경우만 사용함.
	DisplayTriggerBoardType();

	g_DMCont.m_dmShuttle.SN(NDM3_TRIGGER_BOARD_TYPE2, 0);
	g_DMCont.m_dmShuttle.SN(NDM3_TRIGGER_VIRTUAL_ENCODER_FREQUENCY2, 0); //virual일 경우만 사용함.
	DisplayTriggerBoardType2();

	// servo motor setting
	m_cBmpServoMotor.LoadBitmap(IDB_SERVOMOTOR_SETTING);
	m_btnServoMotor.SetBitmap(m_cBmpServoMotor);


	SetTimer(_eTimer_Connect, 200, NULL);
	SetTimer(_eTimer_TriggerParam, 200, NULL);
	SetTimer(_eTimer_BarcodeRetryCnt, 200, NULL);
	SetTimer(_eTimer_CmdBusy, 200, NULL);
	return TRUE;
}

void DlgBarcodeSetting::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_cBmpLDTable1.DeleteObject();
	m_cBmpLDTable2.DeleteObject();
	m_cBmpServoMotor.DeleteObject();

	g_DMCont.m_dmEQP.SB(BDM0_OPEN_SLT_UI_DLG_HISTORY, FALSE);

	g_DMCont.m_dmShuttle.SN(NDM3_TRIGGER_BOARD_TYPE, 0);
	g_DMCont.m_dmShuttle.SN(NDM3_TRIGGER_VIRTUAL_ENCODER_FREQUENCY, 0);
	g_trigger_generator.SetTriggerManulPos(eLoadTbl_2);

	g_DMCont.m_dmShuttle.SN(NDM3_TRIGGER_BOARD_TYPE2, 0);
	g_DMCont.m_dmShuttle.SN(NDM3_TRIGGER_VIRTUAL_ENCODER_FREQUENCY2, 0);
	g_trigger_generator.SetTriggerManulPos(eLoadTbl_3);
	
	for (int i = 0; i < _eTimer_Max; i++) {
		KillTimer(i);
	}
}


void DlgBarcodeSetting::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
		case _eTimer_Connect:
		{
			KillTimer(nIDEvent);

			CString strStatus = _T("");
			if (g_trigger_generator.IsOpen()) {
				strStatus.Format("CONNECT (Port %d)", m_nPort);
				m_StaticComStatus.SetWindowText(strStatus);
				m_StaticComStatus.SetBackColor(COLOR_GREEN);
			}
			else {
				strStatus.Format("DISCONNECT (Port %d)", m_nPort);
				m_StaticComStatus.SetWindowText(strStatus);
				m_StaticComStatus.SetBackColor(COLOR_RED);
			}


			SetTimer(_eTimer_Connect, 200, NULL);
		}break;
		case _eTimer_TriggerParam:
		{
			KillTimer(nIDEvent);

			//================================================Trigger Count
			CString strTrigger1Cnt = _T("");
			CString strTrigger2Cnt = _T("");
			BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
			if (bLotState == eHandlerLotMode_OnGoing) {
				CString strCnt = _T("");
				int nTriggerCnt = g_DMCont.m_dmShuttle.GN(NDM3_BARCODE1_TOTAL_TRIGGER_CNT);
				strCnt.Format("%d", nTriggerCnt);
				m_StaticTrigger1Cnt.SetWindowText(strCnt);

				nTriggerCnt = g_DMCont.m_dmShuttle.GN(NDM3_BARCODE2_TOTAL_TRIGGER_CNT);
				strCnt.Format("%d", nTriggerCnt);
				m_StaticTrigger2Cnt.SetWindowText(strCnt);
			}
			else {
				CString strCnt = _T("");
				int nCnt = g_trigger_generator.GetTriggerCount(_eTrigger_1);
				
				strCnt.Format("%d", nCnt);
				m_StaticTrigger1Cnt.SetWindowText(strCnt);

				nCnt = g_trigger_generator.GetTriggerCount(_eTrigger_2);
				strCnt.Format("%d", nCnt);
				m_StaticTrigger2Cnt.SetWindowText(strCnt);
			}
			//===============================================Trigger Position
			double dScaleFactor = g_trigger_generator.GetTblResolution();

			int nCurPos = g_trigger_generator.GetTriggerCurPos(_eTrigger_1);
		
			CString strCurPos = _T("");
			strCurPos.Format("%.3f", (nCurPos * dScaleFactor) / 1000.0);
			m_StaticTrigger1Pos.SetWindowText(strCurPos);

			nCurPos = g_trigger_generator.GetTriggerCurPos(_eTrigger_2);
			strCurPos.Format("%.3f", (nCurPos * dScaleFactor) / 1000.0);
			m_StaticTrigger2Pos.SetWindowText(strCurPos);

			SetTimer(_eTimer_TriggerParam, 200, NULL);
		}break;
		case _eTimer_BarcodeRetryCnt:
		{
			KillTimer(nIDEvent);

			int nRetryCnt = g_DMCont.m_dmShuttle.GN(NDM3_BARCODE1_RETRY_CNT);
			CString strRetryCnt = _T("");
			strRetryCnt.Format("%d", nRetryCnt);
			m_StaticTrigger1RetryCnt.SetWindowText(strRetryCnt);

			nRetryCnt = g_DMCont.m_dmShuttle.GN(NDM3_BARCODE2_RETRY_CNT);
			strRetryCnt.Format("%d", nRetryCnt);
			m_StaticTrigger2RetryCnt.SetWindowText(strRetryCnt);

			SetTimer(_eTimer_BarcodeRetryCnt, 200, NULL);
		}break;
		case _eTimer_CmdBusy:
		{
			KillTimer(nIDEvent);

			BOOL bBusySts = FALSE;
			BOOL bAlarmSts = FALSE;

			int nCntBusy = 0;
			int nCntAlarm = 0;

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

			//Busy Status Set
			CDSSPanel*    pSensor = NULL;
			pSensor = (CDSSPanel*)GetDlgItem(IDC_TRIGGER_CMD_BUSY);

			if (bBusySts) {
				pSensor->On();
				m_bManualCmdExcute = TRUE;
			}
			else {
				pSensor->Off();
				m_bManualCmdExcute = FALSE;
			}

			//Alarm Status Set
			pSensor = (CDSSPanel*)GetDlgItem(IDC_TRIGGER_CMD_ERROR);
			if (bAlarmSts) {
				pSensor->On();
			}
			else {
				pSensor->Off();
			}

			SetTimer(_eTimer_CmdBusy, 200, NULL);
		}break;
		default:
			break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void DlgBarcodeSetting::OnBnClickedButtonManuTriggerCmdSkip()
{
	CBaseTaskManager::SendProcessCommand(eProcCmd_Skip, eEqpCtrlMode_Manual, NULL);
}


void DlgBarcodeSetting::OnBnClickedButtonManuTriggerCmdStop()
{
	CBaseTaskManager::SendProcessCommand(eProcCmd_Pause, eEqpCtrlMode_Manual, NULL);
}



void DlgBarcodeSetting::OnBnClickedBtnBarcodeSettingTbl2ManuMove()
{
	OnBnClickedBtnAxisManuLoadTbl(eLoadTbl_2);
}


void DlgBarcodeSetting::OnBnClickedBtnBarcodeSettingTbl3ManuMove()
{
	OnBnClickedBtnAxisManuLoadTbl(eLoadTbl_3);
}


void DlgBarcodeSetting::OnBnClickedBtnAxisManuLoadTbl(int nTableIdx)
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


void DlgBarcodeSetting::InitTriggerParam()
{
	//Trigger Count
	m_StaticTrigger1Cnt.m_fnSetFont("Arial");
	m_StaticTrigger1Cnt.SetTextSize(18);
	m_StaticTrigger1Cnt.SetBackColor(COLOR_NOMAL);

	m_StaticTrigger2Cnt.m_fnSetFont("Arial");
	m_StaticTrigger2Cnt.SetTextSize(18);
	m_StaticTrigger2Cnt.SetBackColor(COLOR_NOMAL);

	//Trigger Position
	m_StaticTrigger1Pos.m_fnSetFont("Arial");
	m_StaticTrigger1Pos.SetTextSize(18);
	m_StaticTrigger1Pos.SetBackColor(COLOR_NOMAL);

	m_StaticTrigger2Pos.m_fnSetFont("Arial");
	m_StaticTrigger2Pos.SetTextSize(18);
	m_StaticTrigger2Pos.SetBackColor(COLOR_NOMAL);

	//Barcode Retry Count
	m_StaticTrigger1RetryCnt.m_fnSetFont("Arial");
	m_StaticTrigger1RetryCnt.SetTextSize(18);
	m_StaticTrigger1RetryCnt.SetBackColor(COLOR_NOMAL);

	m_StaticTrigger2RetryCnt.m_fnSetFont("Arial");
	m_StaticTrigger2RetryCnt.SetTextSize(18);
	m_StaticTrigger2RetryCnt.SetBackColor(COLOR_NOMAL);

}


void DlgBarcodeSetting::OnBnClickedBtnTrigger1Clear()
{
	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
	if (bLotState == eHandlerLotMode_OnGoing) {
		g_DMCont.m_dmShuttle.SN(NDM3_BARCODE1_TOTAL_TRIGGER_CNT, 0);
	}
	else {
		g_trigger_generator.ClearTriggerCount(_eTrigger_1);
	}
}


void DlgBarcodeSetting::OnBnClickedBtnTrigger2Clear()
{
	BOOL bLotState = g_DMCont.m_dmEQP.GB(BDM0_SYS_LOT_STATUS);
	if (bLotState == eHandlerLotMode_OnGoing) {
		g_DMCont.m_dmShuttle.SN(NDM3_BARCODE2_TOTAL_TRIGGER_CNT, 0);
	}
	else {
		g_trigger_generator.ClearTriggerCount(_eTrigger_2);
	}
}


void DlgBarcodeSetting::OnBnClickedBtnTriggerPos1Clear()
{
	g_DMCont.m_dmShuttle.SN(NDM3_TRIGGER_BOARD_TYPE, 0);
	g_DMCont.m_dmShuttle.SN(NDM3_TRIGGER_VIRTUAL_ENCODER_FREQUENCY, 0);

	g_TaskLoadTbl[eLoadTbl_2].InitialTrigger();
	//g_trigger_generator.SetTriggerPos(eLoadTbl_2);
	//g_trigger_generator.
	//serial_.ClearEncoderPosition(nTableIdx / 2); // Encode Ch1, Ch2 아님, Trigger 0, Trigger 1
	//g_trigger_generator.SetTriggerManulPos(eLoadTbl_2);

	DisplayTriggerBoardType();
}


void DlgBarcodeSetting::OnBnClickedBtnTriggerPos2Clear()
{
	g_DMCont.m_dmShuttle.SN(NDM3_TRIGGER_BOARD_TYPE2, 0);
	g_DMCont.m_dmShuttle.SN(NDM3_TRIGGER_VIRTUAL_ENCODER_FREQUENCY2, 0);

	g_TaskLoadTbl[eLoadTbl_3].InitialTrigger();
	//g_trigger_generator.SetTriggerPos(eLoadTbl_3);
	//g_trigger_generator.SetTriggerManulPos(eLoadTbl_3);

	DisplayTriggerBoardType2();
}


void DlgBarcodeSetting::OnBnClickedBtnTrigger1RetryCntClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	g_DMCont.m_dmShuttle.SN(NDM3_BARCODE1_RETRY_CNT, 0);
}


void DlgBarcodeSetting::OnBnClickedBtnTrigger2RetryCntClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	g_DMCont.m_dmShuttle.SN(NDM3_BARCODE2_RETRY_CNT, 0);
}


void DlgBarcodeSetting::InitTriggerBoardSetParam()
{
	CStaticTrans* staticParam[7] = {
		&m_StaticResolution,  &m_StaticInterval, &m_StaticDirection, 
		&m_StaticTbl1_StartPos, &m_StaticTbl1_EndPos,
		&m_StaticTbl2_StartPos, &m_StaticTbl2_EndPos,
	};

	for (int i = 0; i < 7; i++) {
		staticParam[i]->m_fnSetFont("Arial");
		staticParam[i]->SetBackColor(COLOR_BACKGROUND_BEIGE);
		staticParam[i]->SetTextSize(15);
	}
	

	//==========================================
	//Text Write
	//==========================================
	double dStartPos = 0.0;
	double dEndPos = 0.0;
	double dInterval = 0.0;
	int dDirection = 0;

	CString strParam = _T("");

	double dScaleFactor = g_trigger_generator.GetTblResolution();
	//==========================LD Table 1========================================================
	g_trigger_generator.GetTriggerInform(eLoadTbl_2, dStartPos, dEndPos, dInterval, dDirection);

	strParam.Format("Resolution(um) : %d", 4);
	m_StaticResolution.SetWindowText(strParam);

	strParam.Format("Interval(mm) : %.2f", dInterval * dScaleFactor / 1000.0);
	m_StaticInterval.SetWindowText(strParam);

	strParam.Format("Direction : NORMAL");
	m_StaticDirection.SetWindowText(strParam);


	//LD Table 1
	strParam.Format("Start Pos : %.2f", dStartPos * dScaleFactor / 1000.0);
	m_StaticTbl1_StartPos.SetWindowText(strParam);

	strParam.Format("End Pos : %.2f", dEndPos * dScaleFactor / 1000.0);
	m_StaticTbl1_EndPos.SetWindowText(strParam);

	g_trigger_generator.GetTriggerInform(eLoadTbl_3, dStartPos, dEndPos, dInterval, dDirection);

	//LD Table 2
	strParam.Format("Start Pos : %.2f", dStartPos * dScaleFactor / 1000.0);
	m_StaticTbl2_StartPos.SetWindowText(strParam);

	strParam.Format("End Pos : %.2f", dEndPos * dScaleFactor / 1000.0);
	m_StaticTbl2_EndPos.SetWindowText(strParam);
}


void DlgBarcodeSetting::DisplayTriggerBoardType()
{
	m_StaticBoardType.m_fnSetFont("Arial");
	m_StaticBoardType.SetBackColor(COLOR_LLIGHTYELLOW);
	m_StaticBoardType.SetTextSize(15);

	int nBoardType = g_DMCont.m_dmShuttle.GN(NDM3_TRIGGER_BOARD_TYPE);
	CString strBoardType = _T("");

	switch (nBoardType)
	{
		case 0: strBoardType.Format("Type : 0(DIFF)"); break;
		case 1: strBoardType.Format("Type : 1(TTL)"); break;
		case 2: strBoardType.Format("Type : 2(VIRTUAL)"); break;
		default:	strBoardType.Format("Type : Error");	break;
	}

	m_StaticBoardType.SetWindowText(strBoardType);

	//Board Type에 따라 Correction활성화, 비활성화 진행 됨.
	DisplayTriggerCorrection();
}

void DlgBarcodeSetting::OnStnClickedTriggerBoardType()
{
	int nBoardType = g_DMCont.m_dmShuttle.GN(NDM3_TRIGGER_BOARD_TYPE);
	nBoardType++;

	if (nBoardType > 2) {
		nBoardType = 0;
	}

	g_DMCont.m_dmShuttle.SN(NDM3_TRIGGER_BOARD_TYPE, nBoardType);

	DisplayTriggerBoardType();
}


void DlgBarcodeSetting::DisplayTriggerCorrection()
{
	m_StaticCorrection.m_fnSetFont("Arial");
	m_StaticCorrection.SetBackColor(COLOR_LLIGHTYELLOW);
	m_StaticCorrection.SetTextSize(15);

	int nBoardType = g_DMCont.m_dmShuttle.GN(NDM3_TRIGGER_BOARD_TYPE);
	CString strCorrection = _T("");
	

	switch (nBoardType)
	{
		case 0:
		case 1: //DIFF, TTL
		{
			strCorrection.Format("CORRECTION(Dis)");
		}break;
		case 2: //VIRTUAL
		{
			int nFrequency = g_DMCont.m_dmShuttle.GN(NDM3_TRIGGER_VIRTUAL_ENCODER_FREQUENCY);
			strCorrection.Format("Virtual ENC(%d Hz)", nFrequency);
		}break;
		default:
		{
			strCorrection.Format("Error");
		}break;
	}

	m_StaticCorrection.SetWindowText(strCorrection);
}


void DlgBarcodeSetting::OnStnClickedTriggerCorrection()
{
	int nBoardType = g_DMCont.m_dmShuttle.GN(NDM3_TRIGGER_BOARD_TYPE);
	if (nBoardType != 2) {
		return;
	}

	int nFrequency = g_DMCont.m_dmShuttle.GN(NDM3_TRIGGER_VIRTUAL_ENCODER_FREQUENCY);

	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(nFrequency, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Virtual Encoder Frequency", "10000", "10");

	if (nRet == RETURN_OK) {
		int nRetVal = atoi(szRetVal);

		CString strRetVal = _T("");
		strRetVal.Format("Virtual ENC(%d Hz)", nRetVal);
		GetDlgItem(IDC_TRIGGER_CORRECTION)->SetWindowText(strRetVal);

		g_DMCont.m_dmShuttle.SN(NDM3_TRIGGER_VIRTUAL_ENCODER_FREQUENCY, nRetVal);

		g_trigger_generator.SetTriggerManulPos(eLoadTbl_2);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}



void DlgBarcodeSetting::DisplayTriggerBoardType2()
{
	m_StaticBoardType2.m_fnSetFont("Arial");
	m_StaticBoardType2.SetBackColor(COLOR_LLIGHTYELLOW);
	m_StaticBoardType2.SetTextSize(15);

	int nBoardType = g_DMCont.m_dmShuttle.GN(NDM3_TRIGGER_BOARD_TYPE2);
	CString strBoardType = _T("");

	switch (nBoardType)
	{
	case 0: strBoardType.Format("Type : 0(DIFF)"); break;
	case 1: strBoardType.Format("Type : 1(TTL)"); break;
	case 2: strBoardType.Format("Type : 2(VIRTUAL)"); break;
	default:	strBoardType.Format("Type : Error");	break;
	}

	m_StaticBoardType2.SetWindowText(strBoardType);

	//Board Type에 따라 Correction활성화, 비활성화 진행 됨.
	DisplayTriggerCorrection2();
}

void DlgBarcodeSetting::OnStnClickedTriggerBoardType2()
{
	int nBoardType = g_DMCont.m_dmShuttle.GN(NDM3_TRIGGER_BOARD_TYPE2);
	nBoardType++;

	if (nBoardType > 2) {
		nBoardType = 0;
	}

	g_DMCont.m_dmShuttle.SN(NDM3_TRIGGER_BOARD_TYPE2, nBoardType);

	DisplayTriggerBoardType2();
}



void DlgBarcodeSetting::DisplayTriggerCorrection2()
{
	m_StaticCorrection2.m_fnSetFont("Arial");
	m_StaticCorrection2.SetBackColor(COLOR_LLIGHTYELLOW);
	m_StaticCorrection2.SetTextSize(15);

	int nBoardType = g_DMCont.m_dmShuttle.GN(NDM3_TRIGGER_BOARD_TYPE2);
	CString strCorrection = _T("");


	switch (nBoardType)
	{
	case 0:
	case 1: //DIFF, TTL
	{
		strCorrection.Format("CORRECTION(Dis)");
	}break;
	case 2: //VIRTUAL
	{
		int nFrequency = g_DMCont.m_dmShuttle.GN(NDM3_TRIGGER_VIRTUAL_ENCODER_FREQUENCY2);
		strCorrection.Format("Virtual ENC(%d Hz)", nFrequency);
	}break;
	default:
	{
		strCorrection.Format("Error");
	}break;
	}

	m_StaticCorrection2.SetWindowText(strCorrection);
}



void DlgBarcodeSetting::OnStnClickedTriggerCorrection2()
{
	int nBoardType = g_DMCont.m_dmShuttle.GN(NDM3_TRIGGER_BOARD_TYPE2);
	if (nBoardType != 2) {
		return;
	}

	int nFrequency = g_DMCont.m_dmShuttle.GN(NDM3_TRIGGER_VIRTUAL_ENCODER_FREQUENCY2);

	char szVal[10] = { 0, };
	char szRetVal[64] = { 0, };
	_itoa_s(nFrequency, szVal, sizeof(szVal));

	int nRet = g_LibCommCtrl.GetNumberBox(this, szRetVal, 10, szVal, "Virtual Encoder Frequency", "10000", "10");

	if (nRet == RETURN_OK) {
		int nRetVal = atoi(szRetVal);

		CString strRetVal = _T("");
		strRetVal.Format("Virtual ENC(%d Hz)", nRetVal);
		GetDlgItem(IDC_TRIGGER_CORRECTION2)->SetWindowText(strRetVal);

		g_DMCont.m_dmShuttle.SN(NDM3_TRIGGER_VIRTUAL_ENCODER_FREQUENCY2, nRetVal);

		g_trigger_generator.SetTriggerManulPos(eLoadTbl_3);
	}
	else if (nRet == RETURN_ERR) {
		AfxMessageBox(_T("Range Over"));
	}
}


void DlgBarcodeSetting::OnBnClickedButtonLed1On()
{
	BarcodeLED_OnOff(eBarcodeLight_ON, _eTrigger_1);
}


void DlgBarcodeSetting::OnBnClickedButtonLed1Off()
{
	BarcodeLED_OnOff(eBarcodeLight_OFF, _eTrigger_1);
}


void DlgBarcodeSetting::OnBnClickedButtonLed2On()
{
	BarcodeLED_OnOff(eBarcodeLight_ON, _eTrigger_2);
}


void DlgBarcodeSetting::OnBnClickedButtonLed2Off()
{
	BarcodeLED_OnOff(eBarcodeLight_OFF, _eTrigger_2);
}

void DlgBarcodeSetting::BarcodeLED_OnOff(int nOnOff, int nLedIdx)
{
	if (nLedIdx == _eTrigger_1) {
		g_pVisionTestHand_1->Cmd_ReqMeasure_BarcodeLED(nOnOff, nLedIdx);
	}
	else {
		g_pVisionTestHand_2->Cmd_ReqMeasure_BarcodeLED(nOnOff, nLedIdx);
	}
}

void DlgBarcodeSetting::OnBnClickedButtonTriggerSetServoMotor()
{
	if (m_bManualCmdExcute) {
		AfxMessageBox(" manual command is Excuting!! please wait working finish!!!!");
		return;
	}

	CDlgManuMotor dlg;
	dlg.DoModal();
}



