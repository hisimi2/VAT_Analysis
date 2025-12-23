// VAT_VisionSetting.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "VAT_VisionSetting.h"
#include "afxdialogex.h"


// CVAT_VisionSetting 대화 상자입니다.

//m_sliderCamLED

const int c_SliderCamLEDId[VAT_CAM_MAX] = {
	IDC_SLIDER_LED_UPPER_1, IDC_SLIDER_LED_LOWER_1,									//LD Tray Pp
	IDC_SLIDER_LED_UPPER_2, IDC_SLIDER_LED_LOWER_2,									//UD Tray Pp
	IDC_SLIDER_LED_UPPER_3, IDC_SLIDER_LED_UPPER_4, IDC_SLIDER_LED_LOWER_3,	//Test Pp#1
	IDC_SLIDER_LED_UPPER_5, IDC_SLIDER_LED_UPPER_6, IDC_SLIDER_LED_LOWER_4,	//Test Pp#2
};

const int c_ComboBoxId[VAT_CAM_MAX] = {
	IDC_COMBO_VISION_MANUAL_UPPERCAM_1, IDC_COMBO_VISION_MANUAL_LOWERCAM_1,									//LD Tray Pp
	IDC_COMBO_VISION_MANUAL_UPPERCAM_2, IDC_COMBO_VISION_MANUAL_LOWERCAM_2,									//UD Tray Pp
	IDC_COMBO_VISION_MANUAL_UPPERCAM_3, IDC_COMBO_VISION_MANUAL_UPPERCAM_4, IDC_COMBO_VISION_MANUAL_LOWERCAM_3,	//Test Pp#1
	IDC_COMBO_VISION_MANUAL_UPPERCAM_5, IDC_COMBO_VISION_MANUAL_UPPERCAM_6, IDC_COMBO_VISION_MANUAL_LOWERCAM_4,	//Test Pp#2
};

const int c_MeasureBtnId[VAT_CAM_MAX] = {
	IDC_BTN_UPPER_CAM_MEASURE_1, IDC_BTN_LOWER_CAM_MEASURE_1,									//LD Tray Pp
	IDC_BTN_UPPER_CAM_MEASURE_2, IDC_BTN_LOWER_CAM_MEASURE_2,									//UD Tray Pp
	IDC_BTN_UPPER_CAM_MEASURE_3, IDC_BTN_UPPER_CAM_MEASURE_4, IDC_BTN_LOWER_CAM_MEASURE_3,	//Test Pp#1
	IDC_BTN_UPPER_CAM_MEASURE_5, IDC_BTN_UPPER_CAM_MEASURE_6, IDC_BTN_LOWER_CAM_MEASURE_4,	//Test Pp#2
};

IMPLEMENT_DYNAMIC(CVAT_VisionSetting, CDialogEx)

CVAT_VisionSetting::CVAT_VisionSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VAT_VISION_SETTING, pParent)
{
	m_PageFont.CreatePointFont(120, "Fixedsys");
}

CVAT_VisionSetting::~CVAT_VisionSetting()
{
}

void CVAT_VisionSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	

	for (int i = 0; i < VAT_CAM_MAX; i++) {
		//slider
		DDX_Control(pDX, c_SliderCamLEDId[i], m_sliderCamLED[i]);

		//combo box
		DDX_Control(pDX, c_ComboBoxId[i], m_ComboBox[i]);

		//measure btn
		DDX_Control(pDX, c_MeasureBtnId[i], m_BtnMeasure[i]);
	}

	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
}


BEGIN_MESSAGE_MAP(CVAT_VisionSetting, CDialogEx)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_NOTIFY_RANGE(NM_CUSTOMDRAW, IDC_SLIDER_LED_LOWER_1, IDC_SLIDER_LED_UPPER_6, &CVAT_VisionSetting::OnNMCustomDrawSliderLed)
	ON_NOTIFY_RANGE(NM_RELEASEDCAPTURE, IDC_SLIDER_LED_LOWER_1, IDC_SLIDER_LED_UPPER_6, &CVAT_VisionSetting::OnNMReleasedcaptureSliderLed)
	ON_BN_CLICKED(IDC_BTN_VAT_VISION_MANUAL_CONNECT1, &CVAT_VisionSetting::OnBnClickedBtnVatVisionManualConnect1)
	ON_BN_CLICKED(IDC_BTN_VAT_VISION_MANUAL_DISCONNECT1, &CVAT_VisionSetting::OnBnClickedBtnVatVisionManualDisconnect1)
	ON_BN_CLICKED(IDC_BTN_VAT_VISION_MANUAL_CONNECT2, &CVAT_VisionSetting::OnBnClickedBtnVatVisionManualConnect2)
	ON_BN_CLICKED(IDC_BTN_VAT_VISION_MANUAL_DISCONNECT2, &CVAT_VisionSetting::OnBnClickedBtnVatVisionManualDisconnect2)

	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_UPPER_CAM_MEASURE_1, IDC_BTN_LOWER_CAM_MEASURE_4, &CVAT_VisionSetting::OnBtnClickedSendMeasure)
END_MESSAGE_MAP()


// CVAT_VisionSetting 메시지 처리기입니다.

BOOL CVAT_VisionSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//============Vision IP/Port 초기화.
	InitVisionIPAndPort();

	//============초기 LED OFF
	InitVisionLED();

	//============Combo Box 초기화
	InitComboBox();


	for (int i = 0; i < VAT_CAM_MAX; i++) {
		//============Slide Control 초기화
		m_sliderCamLED[i].SetRange(0, 1);

		//============Measure Btn Font Init
		m_BtnMeasure[i].SetFont(&m_PageFont);
		m_BtnMeasure[i].SetShade(CShadeButtonST::SHS_HBUMP, 8, 20, 0);
	}

	m_BtnCancel.SetFont(&m_PageFont);

	m_BtnCancel.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0);
	m_BtnCancel.SetIcon(IDI_BTN_EXIT);
	m_BtnCancel.SetAlign(CButtonST::ST_ALIGN_VERT);

	


	//Timer
	//SetTimer(_eVisionSetT_LEDStatus, 200, NULL);

	return TRUE;
}


void CVAT_VisionSetting::InitVisionIPAndPort()
{
	//Set Font.
	GetDlgItem(IDC_EDIT_VAT_VISION_MANUAL_IP_1)->SetFont(&m_PageFont);
	GetDlgItem(IDC_EDIT_VAT_VISION_MANUAL_PORT_1)->SetFont(&m_PageFont);
	GetDlgItem(IDC_EDIT_VAT_VISION_MANUAL_IP_2)->SetFont(&m_PageFont);
	GetDlgItem(IDC_EDIT_VAT_VISION_MANUAL_PORT_2)->SetFont(&m_PageFont);

	GetDlgItem(IDC_BTN_VAT_VISION_MANUAL_CONNECT1)->SetFont(&m_PageFont);
	GetDlgItem(IDC_BTN_VAT_VISION_MANUAL_DISCONNECT1)->SetFont(&m_PageFont);
	GetDlgItem(IDC_BTN_VAT_VISION_MANUAL_CONNECT2)->SetFont(&m_PageFont);
	GetDlgItem(IDC_BTN_VAT_VISION_MANUAL_DISCONNECT2)->SetFont(&m_PageFont);


	//Display IP/PORT
	char szDriverFileName[MAX_PATH] = { 0, };
	char szTemp[512] = { 0, };
	::GetCurrentDirectory(sizeof(szTemp), szTemp);
	sprintf_s(szDriverFileName, sizeof(szDriverFileName), "%s\\sys_data\\%s", szTemp, SZ_DRIVER_FILE_NAME);

	char szIp[128] = { 0, };
	char szPort[64] = { 0, };

	CConfigData ConfigData(szDriverFileName);
	ConfigData.GetString("MC TO VISION COMM CONFIG", "TEST HAND 1 IP", "192.168.1.60", szIp);
	ConfigData.GetString("MC TO VISION COMM CONFIG", "TEST HAND 1 Port", "8080", szPort);
	GetDlgItem(IDC_EDIT_VAT_VISION_MANUAL_IP_1)->SetWindowText(szIp);
	GetDlgItem(IDC_EDIT_VAT_VISION_MANUAL_PORT_1)->SetWindowText(szPort);

	memset(szIp, 0x00, sizeof(szIp));
	memset(szPort, 0x00, sizeof(szPort));

	ConfigData.GetString("MC TO VISION COMM CONFIG", "TEST HAND 2 IP", "192.168.1.61", szIp);
	ConfigData.GetString("MC TO VISION COMM CONFIG", "TEST HAND 2 Port", "9091", szPort);
	GetDlgItem(IDC_EDIT_VAT_VISION_MANUAL_IP_2)->SetWindowText(szIp);
	GetDlgItem(IDC_EDIT_VAT_VISION_MANUAL_PORT_2)->SetWindowText(szPort);
}


void CVAT_VisionSetting::InitVisionLED()
{
	//LED OFF
	for (int nCam = 0; nCam < VAT_CAM_MAX; nCam++) {	
		int nDataId = ConvertVatCamToDataId(nCam);
		g_VatVisionMgr[nCam].SendMeasure(nDataId, VISION_STATUS_LED_OFF);

		//LED의 경우, Receive Data의미 없음으로 상태 Init으로 바로 변경.
		//추후, Receive 받는 것으로 변경 진행.
		g_VatVisionMgr[nCam].SetReceiveState(VAT_RCV_STATUS_INIT);
	}

// 	if (m_simulCtrl._IsSimul()) {
// 		m_simulCtrl._WaitSeqControl(1000.0);
// 		g_VatVisionMgr[nCamNo].SetReceiveState(VAT_RCV_STATUS_SUCCESS);
// 		m_simulCtrl._WaitSeqControl(1000.0);
// 		g_VatVisionMgr[nCamNo].SetReceiveState(VAT_RCV_STATUS_INIT);
// 	}
}

void CVAT_VisionSetting::InitComboBox()
{

	//ComboBoxFont.CreatePointFont(120, "Fixedsys");

	for (int nCam = 0; nCam < VAT_CAM_MAX; nCam++) {
		m_ComboBox[nCam].ResetContent();

		m_ComboBox[nCam].SetFont(&m_PageFont);


		//Add String
		m_ComboBox[nCam].AddString(_T("CAM AUTO FOCUS"));
		m_ComboBox[nCam].AddString(_T("PLATE JIG"));

		switch (nCam)
		{
			case VAT_CAM_LD_TRAY_PP_UPPER:
			case VAT_CAM_UD_TRAY_PP_UPPER:
			{
				m_ComboBox[nCam].AddString(_T("C-TRAY JIG"));
				m_ComboBox[nCam].AddString(_T("TABLE"));
			}break;
			case VAT_CAM_LD_TEST_PP_1_UPPER:
			case VAT_CAM_UD_TEST_PP_1_UPPER:
			case VAT_CAM_LD_TEST_PP_2_UPPER:
			case VAT_CAM_UD_TEST_PP_2_UPPER:
			{
				m_ComboBox[nCam].AddString(_T("STATION"));
				m_ComboBox[nCam].AddString(_T("TABLE"));
			}break;
			case VAT_CAM_LD_TRAY_PP_LOWER:
			case VAT_CAM_UD_TRAY_PP_LOWER:
			case VAT_CAM_TEST_PP_1_LOWER:
			case VAT_CAM_TEST_PP_2_LOWER:
			{
				m_ComboBox[nCam].AddString(_T("PICKER"));
			}break;
			default:
				break;
		}
		
		m_ComboBox[nCam].SetCurSel(0);
	}
}

void CVAT_VisionSetting::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

// 	switch (nIDEvent)
// 	{
// 	case _eVisionSetT_LEDStatus:
// 	{
// 		
// 	}break;
// 	default:
// 		break;
// 	}


	CDialogEx::OnTimer(nIDEvent);
}


void CVAT_VisionSetting::OnDestroy()
{
// 	for (int i = 0; i < _eVisionSetT_Max; i++) {
// 		KillTimer(i);
// 	}

	CDialogEx::OnDestroy();
}




void CVAT_VisionSetting::OnNMCustomDrawSliderLed(UINT nID, NMHDR *pNMHDR, LRESULT *pResult)
{
	//Cam Num Find
	int nCamNo = FindCamNo(c_SliderCamLEDId, nID);

	int nCurVal = m_sliderCamLED[nCamNo].GetPos();
	if (nCurVal < 1) nCurVal = 0;
	else			 nCurVal = 1;


	LPNMCUSTOMDRAW pDraw = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	CDC* pDC = CDC::FromHandle(pDraw->hdc);

	switch (pDraw->dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW; // 항목별 그리기 허용
		return;

	case CDDS_ITEMPREPAINT:
		switch (pDraw->dwItemSpec)
		{
		case TBCD_CHANNEL: // 트랙 (바)
		{
			//  				COLORREF color = (nCurVal == 0) ? DDARKGRAY : LLIGHTGREEN;
			//  				pDC->FillSolidRect(&pDraw->rc, color);  // 초록색 트랙
			*pResult = CDRF_DODEFAULT;
			return;
		}
		case TBCD_THUMB: // 썸(버튼)
		{
			COLORREF color = (nCurVal == 0) ? LLIGHTGRAY : LLIGHTGREEN/*LLIGHTYELLOW*/ /*LLIGHTRED*/;
			pDC->FillSolidRect(&pDraw->rc, color); // 빨간색 썸
			*pResult = CDRF_SKIPDEFAULT;
			return;
		}
		}
		break;
	}

	*pResult = 0;

}

void CVAT_VisionSetting::OnNMReleasedcaptureSliderLed(UINT nID, NMHDR *pNMHDR, LRESULT *pResult)
{
	//int nVal = m_sliderCamLED[nCam].GetPos();
	//(nVal < 1) ? m_sliderCamLED[nCam].SetPos(0) : m_sliderCamLED[nCam].SetPos(1);

	
	int nCamNo = FindCamNo(c_SliderCamLEDId, nID);

	int nDataId = ConvertVatCamToDataId(nCamNo);

	int nVal = m_sliderCamLED[nCamNo].GetPos();

	//nStatus
	int nStatus = (nVal < 1) ? VISION_STATUS_LED_OFF : VISION_STATUS_LED_ON;

	g_VatVisionMgr[nCamNo].SendMeasure(nDataId, nStatus);

	//Slider On/Off
	(nVal < 1) ? m_sliderCamLED[nCamNo].SetPos(0) : m_sliderCamLED[nCamNo].SetPos(1);

	g_VatVisionMgr[nCamNo].SetReceiveState(VAT_RCV_STATUS_INIT);

//	if (m_simulCtrl._IsSimul()) {
// 		m_simulCtrl._WaitSeqControl(1000.0);
// 		g_VatVisionMgr[nCamNo].SetReceiveState(VAT_RCV_STATUS_SUCCESS);
// 		m_simulCtrl._WaitSeqControl(1000.0);
// 		g_VatVisionMgr[nCamNo].SetReceiveState(VAT_RCV_STATUS_INIT);
// 	}
	

	*pResult = 0;
}

// 
// int CVAT_VisionSetting::ConvertSliderIdToVisionDataId(UINT nID)
// {
// 	int nDataID = -1;
// 
// 	switch (nID)
// 	{
// 	case IDC_SLIDER_LED_LOWER_1:	nDataID = VISION_MEASURE_VAT_LD_LOWER;		break;
// 	case IDC_SLIDER_LED_LOWER_2:	nDataID = VISION_MEASURE_VAT_ULD_LOWER;		break;
// 	case IDC_SLIDER_LED_LOWER_3:	nDataID = VISION_MEASURE_VAT_TEST_1_LOWER;		break;
// 	case IDC_SLIDER_LED_LOWER_4:	nDataID = VISION_MEASURE_VAT_TEST_2_LOWER;	break;
// 	case IDC_SLIDER_LED_UPPER_1:	nDataID = VISION_MEASURE_VAT_LD_UPPER;	break;
// 	case IDC_SLIDER_LED_UPPER_2:	nDataID = VISION_MEASURE_VAT_ULD_UPPER;	break;
// 	case IDC_SLIDER_LED_UPPER_3:	nDataID = VISION_MEASURE_VAT_TEST_1_LD_UPPER;	break;
// 	case IDC_SLIDER_LED_UPPER_4:	nDataID = VISION_MEASURE_VAT_TEST_1_ULD_UPPER;	break;
// 	case IDC_SLIDER_LED_UPPER_5:	nDataID = VISION_MEASURE_VAT_TEST_2_LD_UPPER;	break;
// 	case IDC_SLIDER_LED_UPPER_6:	nDataID = VISION_MEASURE_VAT_TEST_2_ULD_UPPER;	break;
// 	default:
// 		break;
// 	}
// 
// 	return nDataID;
// }

void CVAT_VisionSetting::OnBnClickedBtnVatVisionManualConnect1()
{
	if (g_pVisionTestHand_1 != NULL)
	{
		if (g_pVisionTestHand_1->IsConnected()) {
			return;
		}
		else {
			CString strIp = _T(""), strPort = _T("");
			GetDlgItem(IDC_EDIT_VAT_VISION_MANUAL_IP_1)->GetWindowText(strIp);
			GetDlgItem(IDC_EDIT_VAT_VISION_MANUAL_PORT_1)->GetWindowText(strPort);

			g_pVisionTestHand_1->ChangeSetting(strIp.GetBuffer(0), atoi(strPort));
			g_pVisionTestHand_1->SocketConnect();
		}
	}
}


void CVAT_VisionSetting::OnBnClickedBtnVatVisionManualDisconnect1()
{
	if (g_pVisionTestHand_1 != NULL)
	{
		if (g_pVisionTestHand_1->IsConnected() != TRUE)
			return;

		g_pVisionTestHand_1->SocketClose();
	}
}


void CVAT_VisionSetting::OnBnClickedBtnVatVisionManualConnect2()
{
	if (g_pVisionTestHand_2 != NULL)
	{
		if (g_pVisionTestHand_2->IsConnected()) {
			return;
		}
		else {
			CString strIp = _T(""), strPort = _T("");
			GetDlgItem(IDC_EDIT_VAT_VISION_MANUAL_IP_2)->GetWindowText(strIp);
			GetDlgItem(IDC_EDIT_VAT_VISION_MANUAL_PORT_2)->GetWindowText(strPort);

			g_pVisionTestHand_2->ChangeSetting(strIp.GetBuffer(0), atoi(strPort));
			g_pVisionTestHand_2->SocketConnect();
		}
	}
}


void CVAT_VisionSetting::OnBnClickedBtnVatVisionManualDisconnect2()
{
	if (g_pVisionTestHand_2 != NULL)
	{
		if (g_pVisionTestHand_2->IsConnected() != TRUE)
			return;

		g_pVisionTestHand_2->SocketClose();
	}
}

void CVAT_VisionSetting::OnBtnClickedSendMeasure(UINT nID)
{
	//1. [Cam No]
	int nCam = FindCamNo(c_MeasureBtnId, nID);

	//2. [Data ID]
	int nDataId = ConvertVatCamToDataId(nCam);

	//3. [Status]
	//[Combo Box Num]
	int nCurComboSel = m_ComboBox[nCam].GetCurSel();
	CString strCurComboBox = _T("");
	m_ComboBox[nCam].GetLBText(nCurComboSel, strCurComboBox);

	int nStatus = -1;
	if (strCurComboBox == _T("CAM AUTO FOCUS"))			nStatus = VISION_STATUS_AUTO_FOCUS;
	else if (strCurComboBox == _T("PLATE JIG"))			nStatus = VISION_STATUS_PLATE_JIG;
	else if (strCurComboBox == _T("C-TRAY JIG"))		nStatus = VISION_STATUS_C_TRAY;
	else if (strCurComboBox == _T("TABLE"))				nStatus = VISION_STATUS_TABLE;
	else if (strCurComboBox == _T("STATION"))			nStatus = VISION_STATUS_SOCKET;
	else if (strCurComboBox == _T("PICKER"))			nStatus = VISION_STATUS_PICKER;


	CBaseTaskManager::SetEqpCtrlMode(eEqpCtrlMode_Manual);

	CBaseTaskManager* pTask = NULL;
	switch (nCam)
	{
		case VAT_CAM_LD_TRAY_PP_UPPER:	case VAT_CAM_LD_TRAY_PP_LOWER:	
		{
			pTask = &g_TaskTrayLoadPP;
		}break;
		case VAT_CAM_UD_TRAY_PP_UPPER: case VAT_CAM_UD_TRAY_PP_LOWER:
		{
			pTask = &g_TaskTrayUnloadPP;
		}break;
		case VAT_CAM_LD_TEST_PP_1_UPPER: case VAT_CAM_UD_TEST_PP_1_UPPER: case VAT_CAM_TEST_PP_1_LOWER:
		{
			pTask = &g_TaskTestPP[eTestPP_1];
		}break;
		case VAT_CAM_LD_TEST_PP_2_UPPER: case VAT_CAM_UD_TEST_PP_2_UPPER: case VAT_CAM_TEST_PP_2_LOWER:
		{
			pTask = &g_TaskTestPP[eTestPP_2];
		}break;
		default:
			break;
	}

	CVatVisionCmd_CamMeasure* pSeqCmd = new CVatVisionCmd_CamMeasure(-1);
	pSeqCmd->m_vDataId.push_back(nDataId);
	pSeqCmd->m_nStatus = nStatus;

	
	//Vision Manual의 경우, 다수의 Hand Cmd 연동 동작을 위해 임시로 SendCommand 사용 안함.
// 	pSeqCmd->m_pReceive = pTask;
// 	pSeqCmd->m_pSender = &g_VatTaskSystemCtrl;
// 	pSeqCmd->m_nCmdSeqMode = eEqpCtrlMode_Manual;
// 	pTask->OnCommand(pSeqCmd, eEqpCtrlMode_Manual);

	int nErr = g_VatTaskSystemCtrl.SendCommand(*pTask, pSeqCmd, eEqpCtrlMode_Manual);
}


int CVAT_VisionSetting::FindCamNo(const int staticId[], int nID)
{
	int nCam = 0;
	for (; nCam < VAT_CAM_MAX; nCam++) {
		if (staticId[nCam] == nID) {
			return nCam;
		}
	}
}