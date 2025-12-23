// VATMain_CamCenterCalibration.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "VATMain_CamCenterCalibration.h"
#include "afxdialogex.h"

// CVATMain_CamCenterCalibration 대화 상자입니다.

IMPLEMENT_DYNAMIC(CVATMain_CamCenterCalibration, CDialogEx)

CVATMain_CamCenterCalibration::CVATMain_CamCenterCalibration(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VAT_MAIN_CAM_CENTER_CALIBRATION, pParent)
{
	m_nSS1_DisplayType = eVAT_SSDISPLAYTYPE_DATA;
	m_nSS2_DisplayType = eVAT_SSDISPLAYTYPE_DATA;
}

CVATMain_CamCenterCalibration::~CVATMain_CamCenterCalibration()
{
}

void CVATMain_CamCenterCalibration::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_SPREAD_CAM_CENTER_CAL_LOAD_TRAYPP, m_ssSelectSheet[VAT_HAND_LOAD_TRAYPP]);
	DDX_Control(pDX, IDC_SPREAD_CAM_CENTER_CAL_UNLOAD_TRAYPP, m_ssSelectSheet[VAT_HAND_UNLOAD_TRAYPP]);
	DDX_Control(pDX, IDC_SPREAD_CAM_CENTER_CAL_LOAD_TESTPP_1, m_ssSelectSheet[VAT_HAND_LOAD_TESTPP1]);
	DDX_Control(pDX, IDC_SPREAD_CAM_CENTER_CAL_UNLOAD_TESTPP_1, m_ssSelectSheet[VAT_HAND_UNLOAD_TESTPP1]);
	DDX_Control(pDX, IDC_SPREAD_CAM_CENTER_CAL_LOAD_TESTPP_2, m_ssSelectSheet[VAT_HAND_LOAD_TESTPP2]);
	DDX_Control(pDX, IDC_SPREAD_CAM_CENTER_CAL_UNLOAD_TESTPP_2, m_ssSelectSheet[VAT_HAND_UNLOAD_TESTPP2]);

	DDX_Control(pDX, IDC_SSPANEL_CAM_CENTER_CAL_HAND_SELECT, m_ssPanel_HandSelect);
	DDX_Control(pDX, IDC_SSPANEL_CAM_CENTER_CAL_MEASURE_STATUS, m_ssPanel_MeasureStatus);

	DDX_Control(pDX, IDC_SSPANEL_CAM_CENTER_CAL_BOT_CAM_POS, m_ssDataPanel_1_Title);
	DDX_Control(pDX, IDC_SSPANEL_BOT_CAM_POS_DATA, m_ssDataPanel_1_Sub1);
	DDX_Control(pDX, IDC_SSPANEL_BOT_CAM_POS_DATA_DIFF, m_ssDataPanel_1_Sub2);
	DDX_Control(pDX, IDC_SSPANEL_CAM_CENTER_CAL_CAM_TO_PKR_DIS, m_ssDataPanel_2_Title);
	DDX_Control(pDX, IDC_SSPANEL_CAM_TO_PKR_DIS_DATA, m_ssDataPanel_2_Sub1);
	DDX_Control(pDX, IDC_SSPANEL_CAM_TO_PKR_DIS_DATA_DIFF, m_ssDataPanel_2_Sub2);


	DDX_Control(pDX, IDC_SPREAD_CAM_CENTER_CAL_TARGET_POSITION, m_ssBotCamCenterPos);
	DDX_Control(pDX, IDC_SPREAD_TOP_CAM_TO_PICKER_DISTANCE, m_ssTopCamToPkrDis);	


	DDX_Control(pDX, IDC_BTN_CAM_CENTER_ENCODER_READ_1, m_BtnOper1_EncoderRead);
	DDX_Control(pDX, IDC_BTN_CAM_CENTER_MOTOR_MOVE_1, m_BtnOper1_MotorMove);
	DDX_Control(pDX, IDC_BTN_CAM_CENTER_RELOAD_1, m_BtnOper1_Reload);
	DDX_Control(pDX, IDC_BTN_CAM_CENTER_DATA_SAVE_1, m_BtnOper1_Save);
	DDX_Control(pDX, IDC_BTN_CAM_CENTER_MAKE_BASE_POS_1, m_BtnOper1_MakeBasePos);


	DDX_Control(pDX, IDC_BTN_CAM_CENTER_MOTOR_MOVE_2, m_BtnOper2_MotorMove);

	for (int i = 0; i < VAT_HAND_MAX; i++) {
		DDX_Control(pDX, m2_nHandPosition[i], m_editPosition[i]);

		DDX_Control(pDX, m2_nProgress[i], m_Progress[i]);
		DDX_Control(pDX, m2_nPercent[i], m_Percent[i]);
	}

}


BEGIN_MESSAGE_MAP(CVATMain_CamCenterCalibration, CDialogEx)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_CAM_CENTER_ENCODER_READ_1, &CVATMain_CamCenterCalibration::OnBnClickedBtnCamCenterEncoderRead1)
	ON_BN_CLICKED(IDC_BTN_CAM_CENTER_MOTOR_MOVE_1, &CVATMain_CamCenterCalibration::OnBnClickedBtnCamCenterMotorMove1)
	ON_BN_CLICKED(IDC_BTN_CAM_CENTER_RELOAD_1, &CVATMain_CamCenterCalibration::OnBnClickedBtnCamCenterReload1)
	ON_BN_CLICKED(IDC_BTN_CAM_CENTER_DATA_SAVE_1, &CVATMain_CamCenterCalibration::OnBnClickedBtnCamCenterDataSave1)

	ON_MESSAGE(UM_VAT_SEQ_MSG_UPDATE_HAND_STATUS, &CVATMain_CamCenterCalibration::OnReceiveUpdateHandStatus)
	ON_MESSAGE(UM_VAT_SEQ_MSG_UPDATE_PROGRESS, &CVATMain_CamCenterCalibration::OnReceiveUpdateProgress)
	ON_MESSAGE(UM_VAT_SEQ_MSG_UPDATE_VISION_STATUS, &CVATMain_CamCenterCalibration::OnReceiveUpdateVisionStatus)
	ON_MESSAGE(UM_VAT_SEQ_MSG_UPDATE_VISION_OFFSET, &CVATMain_CamCenterCalibration::OnReceiveUpdateVisionOffset)
 	ON_MESSAGE(UM_VAT_SEQ_MSG_UPDATE_VISION_RETRY_CNT, &CVATMain_CamCenterCalibration::OnReceiveUpdateVisionMeasureCnt)
 	ON_MESSAGE(UM_VAT_SEQ_MSG_UPDATE_SAVE_DATA, &CVATMain_CamCenterCalibration::OnReceiveUpdateSavedData)
	ON_BN_CLICKED(IDC_BTN_CAM_CENTER_MAKE_BASE_POS_1, &CVATMain_CamCenterCalibration::OnBnClickedBtnCamCenterMakeBasePos1)
	ON_BN_CLICKED(IDC_BTN_CAM_CENTER_MOTOR_MOVE_2, &CVATMain_CamCenterCalibration::OnBnClickedBtnCamCenterMotorMove2)
END_MESSAGE_MAP()


// CVATMain_CamCenterCalibration 메시지 처리기입니다.


BOOL CVATMain_CamCenterCalibration::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//Data Load
	m_stVATCamCenCalInfoGui = g_VATCommonData.m_stVATCamCenterCalInfo;

	//Title SSPanel 초기화.
	CDSSPanel* panel[2] = {
		&m_ssPanel_HandSelect, &m_ssPanel_MeasureStatus
	};
	for (auto p : panel) {
		_Com_InitSSTitlePanel(*p);
	}

	//Cam Center Calibration Spread Sheet SSPanel 초기화.
	_Com_InitSSDataPanel(m_ssDataPanel_1_Title, m_ssDataPanel_1_Sub1, m_ssDataPanel_1_Sub2);
	_Com_InitSSDataPanel(m_ssDataPanel_2_Title, m_ssDataPanel_2_Sub1, m_ssDataPanel_2_Sub2, eVAT_SSACCESSTYPE_RO);

	//Arm 선택 Spread Sheet 초기화.
	_Com_InitHandSelectSheet();

	//Cam Center Calibration Spread Sheet 초기화.
	InitCamCenterCalBasePosSheet();

	//Cam Center Calibration Spread Sheet Data Display.
	DisplaySSData(_eCamCenCalSS_BotCamCenterPos, m_nSS1_DisplayType);
	DisplaySSData(_eCamCenCalSS_TopCamToPkrDis, m_nSS2_DisplayType);

	//Operate Button 초기화
	InitCamCenterOperBtn();

	//Measure OverView 초기화
	InitCamCenterMeasureOverView();

	return TRUE;
}


//==============================================================================
//COMMENT : Mode 변경 후,  Cam Center Calibration Page 이동 시 초기화역할로 사용
//==============================================================================
void CVATMain_CamCenterCalibration::PageMoveInit_Main()
{
	//Offset Grid&Data 다시 그리기.
	Invalidate(TRUE);

	//Timer
	SetTimer(_eCamCenterCalT_HandStatus, 1000, NULL);
	SetTimer(_eCamCenterCalT_CheckExecuteStatus, 200, NULL);

	//Data Load
	m_stVATCamCenCalInfoGui = g_VATCommonData.m_stVATCamCenterCalInfo;

	//Cam Center Calibration Spread Sheet Data Display.
	DisplaySSData(_eCamCenCalSS_BotCamCenterPos, m_nSS1_DisplayType);
	DisplaySSData(_eCamCenCalSS_TopCamToPkrDis, m_nSS2_DisplayType);
}


//================================================================================
//COMMENT : Mode 변경 후, Cam Center Calibration Page 이동 시, 소멸자 역할로 사용
//================================================================================
void CVATMain_CamCenterCalibration::PageMoveDispose()
{
	//Kill Timer Set
	for (int i = 0; i < _eCamCenterCalT_Max; i++) {
		KillTimer(i);
	}
}


void CVATMain_CamCenterCalibration::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
		case _eCamCenterCalT_HandStatus:
		{
			_Com_tDisplayHandStatus(m_editPosition);			
		}break;
		case _eCamCenterCalT_CheckExecuteStatus:
		{
			//상태 지속적으로 변경 되는 것을 방지하고자 Last status check 진행.
			if (g_bVATStatusExecute != m_bLastExecuteStatus)
			{
				EnableControls(!g_bVATStatusExecute);
				m_bLastExecuteStatus = g_bVATStatusExecute;
			}
		}break;
		default:
			break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CVATMain_CamCenterCalibration::OnDestroy()
{
	for (int i = 0; i < _eCamCenterCalT_Max; i++) {
		KillTimer(i);
	}

	CDialogEx::OnDestroy();
}


void CVATMain_CamCenterCalibration::OnPaint()
{
	CPaintDC dc(this);

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		BOOL bIsReDraw = dc.RectVisible(&m_rect[nHand]) ||
						 dc.RectVisible(&m_rectData[nHand][VAT_X]) ||
						 dc.RectVisible(&m_rectData[nHand][VAT_Y]);

		if (bIsReDraw) { //필요한 rect만 다시 그리기
			_PkrAlignCom_DrawBaseGrid(nHand, dc);

			_PkrAlignCom_DrawPkrCenterHole(nHand, dc);

			_PkrAlignCom_DrawHeadText(nHand, dc);
			
			_PkrAlignCom_DrawOffsetData(nHand, dc);

			_PkrAlignCom_DrawRealHole(nHand, dc);
		}
	}	
}



BEGIN_EVENTSINK_MAP(CVATMain_CamCenterCalibration, CDialogEx)
ON_EVENT(CVATMain_CamCenterCalibration, IDC_SPREAD_CAM_CENTER_CAL_LOAD_TRAYPP, 3, CVATMain_CamCenterCalibration::ButtonClickedSpreadCamCenterCalLoadTraypp, VTS_I4 VTS_I4 VTS_I2)
ON_EVENT(CVATMain_CamCenterCalibration, IDC_SPREAD_CAM_CENTER_CAL_UNLOAD_TRAYPP, 3, CVATMain_CamCenterCalibration::ButtonClickedSpreadCamCenterCalUnloadTraypp, VTS_I4 VTS_I4 VTS_I2)
ON_EVENT(CVATMain_CamCenterCalibration, IDC_SPREAD_CAM_CENTER_CAL_LOAD_TESTPP_1, 3, CVATMain_CamCenterCalibration::ButtonClickedSpreadCamCenterCalLoadTestpp1, VTS_I4 VTS_I4 VTS_I2)
ON_EVENT(CVATMain_CamCenterCalibration, IDC_SPREAD_CAM_CENTER_CAL_UNLOAD_TESTPP_1, 3, CVATMain_CamCenterCalibration::ButtonClickedSpreadCamCenterCalUnloadTestpp1, VTS_I4 VTS_I4 VTS_I2)
ON_EVENT(CVATMain_CamCenterCalibration, IDC_SPREAD_CAM_CENTER_CAL_LOAD_TESTPP_2, 3, CVATMain_CamCenterCalibration::ButtonClickedSpreadCamCenterCalLoadTestpp2, VTS_I4 VTS_I4 VTS_I2)
ON_EVENT(CVATMain_CamCenterCalibration, IDC_SPREAD_CAM_CENTER_CAL_UNLOAD_TESTPP_2, 3, CVATMain_CamCenterCalibration::ButtonClickedSpreadCamCenterCalUnloadTestpp2, VTS_I4 VTS_I4 VTS_I2)
ON_EVENT(CVATMain_CamCenterCalibration, IDC_SSPANEL_CAM_TO_PKR_DIS_DATA, DISPID_CLICK, CVATMain_CamCenterCalibration::ClickSspanelCamToPkrDisData, VTS_NONE)
ON_EVENT(CVATMain_CamCenterCalibration, IDC_SSPANEL_CAM_TO_PKR_DIS_DATA_DIFF, DISPID_CLICK, CVATMain_CamCenterCalibration::ClickSspanelCamToPkrDisDataDiff, VTS_NONE)
ON_EVENT(CVATMain_CamCenterCalibration, IDC_SSPANEL_BOT_CAM_POS_DATA, DISPID_CLICK, CVATMain_CamCenterCalibration::ClickSspanelBotCamPosData, VTS_NONE)
ON_EVENT(CVATMain_CamCenterCalibration, IDC_SSPANEL_BOT_CAM_POS_DATA_DIFF, DISPID_CLICK, CVATMain_CamCenterCalibration::ClickSspanelBotCamPosDataDiff, VTS_NONE)
ON_EVENT(CVATMain_CamCenterCalibration, IDC_SPREAD_CAM_CENTER_CAL_TARGET_POSITION, 5, CVATMain_CamCenterCalibration::ClickSpreadCamCenterCalTargetPosition, VTS_I4 VTS_I4)
ON_EVENT(CVATMain_CamCenterCalibration, IDC_SPREAD_CAM_CENTER_CAL_TARGET_POSITION, 11, CVATMain_CamCenterCalibration::DblClickSpreadCamCenterCalTargetPosition, VTS_I4 VTS_I4)
ON_EVENT(CVATMain_CamCenterCalibration, IDC_SPREAD_TOP_CAM_TO_PICKER_DISTANCE, 5, CVATMain_CamCenterCalibration::ClickSpreadTopCamToPickerDistance, VTS_I4 VTS_I4)
ON_EVENT(CVATMain_CamCenterCalibration, IDC_SPREAD_TOP_CAM_TO_PICKER_DISTANCE, 11, CVATMain_CamCenterCalibration::DblClickSpreadTopCamToPickerDistance, VTS_I4 VTS_I4)

END_EVENTSINK_MAP()


void CVATMain_CamCenterCalibration::InitCamCenterCalBasePosSheet()
{
	//Cam Center Position
	_Com_InitPosSpreadSheet_HandType(m_ssBotCamCenterPos);

	//Top Cam to Picker Distance
	_Com_InitPosSpreadSheet_HandType(m_ssTopCamToPkrDis);

	m_stBotCamCenterPosLastInfo.nSSType = eVAT_SSTYPE_HAND;
	m_stTopCamToPkrDisLastInfo.nSSType = eVAT_SSTYPE_ENGINEER;
	//m_stTopCamToPkrDisLastInfo.nSSType = eVAT_SSTYPE_HAND;
}

void CVATMain_CamCenterCalibration::InitCamCenterOperBtn()
{
	//Bottom Cam Center Pos
 	CShadeButtonST *btnOper[eSSOper_MAX] = { &m_BtnOper1_EncoderRead,  &m_BtnOper1_MotorMove, &m_BtnOper1_Reload, &m_BtnOper1_Save, &m_BtnOper1_MakeBasePos };
 	for (int nType = 0; nType < eSSOper_MAX; nType++) {
 		_Com_InitOperBtn(*btnOper[nType], nType);
 	}

	if (g_DMCont.m_dmEQP.GN(NDM0_TEST_SW_ENGINEER_OPTION))
	{
		GetDlgItem(IDC_BTN_CAM_CENTER_MOTOR_MOVE_2)->ShowWindow(SW_SHOW);
		_Com_InitOperBtn(m_BtnOper2_MotorMove, eSSOper_MOTOR_MOVING);
		m_BtnOper2_MotorMove.SetWindowText(_T("MOVE"));
	}
}


void CVATMain_CamCenterCalibration::InitCamCenterMeasureOverView()
{
	//Progress Bar
	_Com_InitProgressBar();

	//Vision Offset
	//OnPaint에서 초기화 진행 함.

	//Vision Status && Vision Retry Cnt
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		int nFrame = 1; //VAT_RCV_STATUS_INIT 1
		CDSSPanel* chkbt = (CDSSPanel*)GetDlgItem(m2_nCamStatus[nHand]);
		chkbt->SetStatus(nFrame);
		chkbt->SetForeColor(BBLACK);
		chkbt->SetWindowText(_T("0"));
	}

	//Offset Rect Graphic&Data View
	InitOverViewRect();
}


void CVATMain_CamCenterCalibration::InitOverViewRect()
{
	//Offset Graphic View
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{
		CStatic *staticArea = (CStatic *)GetDlgItem(m2_nRectGraphicView[nHand]);
		staticArea->GetWindowRect(&m_rect[nHand]);
		ScreenToClient(m_rect[nHand]);
	}
	
	//Offset Data View
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{
		for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XY; nAxis++)
		{
			CStatic *staticArea = (CStatic *)GetDlgItem(m2_nRectDataView[nHand][nAxis]);
			staticArea->GetWindowRect(&m_rectData[nHand][nAxis]);
			ScreenToClient(m_rectData[nHand][nAxis]);
		}
	}
}


void CVATMain_CamCenterCalibration::DisplaySSData(int nCamCenCalSSType, int nSSDisplayType, BOOL bMakeBasePos/* = FALSE*/)
{
	switch (nCamCenCalSSType)
	{
		case _eCamCenCalSS_BotCamCenterPos:
		{
			for (auto nHand : m_vAllHand)
			{
				for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XYZ; nAxis++)
				{
					double dPos = 0.0;
					if (bMakeBasePos == FALSE)
					{
						if (nSSDisplayType == eVAT_SSDISPLAYTYPE_DATA) {
							dPos = g_VATCommonData.m_stVATCamCenterCalInfo.dBotCameraPosition[nHand][nAxis];
						}
						else {//nSSDisplayType == eVAT_SSDISPLAYTYPE_DIFFERENCE
							//변경된 Data - 이전 Data
							dPos = g_VATCommonData.m_stVATCamCenterCalInfo.dBotCameraPosition[nHand][nAxis] - g_VATCommonData.m_stVATCamCenterCalInfoBuff.dBotCameraPosition[nHand][nAxis];
						}
					}
					else
					{
						dPos = m_stVATCamCenCalInfoGui.dBotCameraPosition[nHand][nAxis];
					}

					CString strPos = _T("");
					strPos.Format("%.3f", dPos / 1000.0);
					int nCol = nAxis + 1;
					int nRow = nHand + 1;

					m_ssBotCamCenterPos.SetTextMatrix(nRow, nCol, strPos);
					//Diff에서 Limit 상관 없음 
					if (nSSDisplayType == eVAT_SSDISPLAYTYPE_DATA)
					{
						//Pos/Neg Limit에 따른 글자 색상 변경.
						BOOL bIsLimitPos = _Com_BasePositionLimitCheck(nHand, nAxis, dPos);
						OLE_COLOR color = (bIsLimitPos) ? LIMIT_COLOR_RED : NORMAL_COLOR_BLACK;
						m_ssBotCamCenterPos.SetForeColor(color);
					}
					else
					{
						m_ssBotCamCenterPos.SetForeColor(NORMAL_COLOR_BLACK);
					}
				}
			}
		}break;
		case _eCamCenCalSS_TopCamToPkrDis:
		{
			for (auto nHand : m_vAllHand)
			{
				for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XYZ; nAxis++)
				{
					double dPos = 0.0;
					if (nSSDisplayType == eVAT_SSDISPLAYTYPE_DATA) {
						dPos = g_VATCommonData.m_stVATCamCenterCalInfo.dTopCamToPkrDistance[nHand][nAxis];
					}
					else {//nSSDisplayType == eVAT_SSDISPLAYTYPE_DIFFERENCE
						//변경된 Data - 이전 Data
						dPos = g_VATCommonData.m_stVATCamCenterCalInfo.dTopCamToPkrDistance[nHand][nAxis] - g_VATCommonData.m_stVATCamCenterCalInfoBuff.dTopCamToPkrDistance[nHand][nAxis];
					}

					CString strPos = _T("");
					strPos.Format("%.3f", dPos / 1000.0);
					int nCol = nAxis + 1;
					int nRow = nHand + 1;

					m_ssTopCamToPkrDis.SetTextMatrix(nRow, nCol, strPos);
				}
			}
		}break;
	default:
		break;
	}
}

int CVATMain_CamCenterCalibration::CheckPosLimit()
{
	for (auto nHand : m_vAllHand)
	{
		for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XYZ; nAxis++)
		{
			double dPos = 0.0;
			dPos = m_stVATCamCenCalInfoGui.dBotCameraPosition[nHand][nAxis];

			//Pos/Neg Limit Over 시 TRUE 반환
			BOOL bIsLimitPos = _Com_BasePositionLimitCheck(nHand, nAxis, dPos);
			if (bIsLimitPos == TRUE)
			{
				return TRUE;
			}
		}
	}
}

void CVATMain_CamCenterCalibration::EnableControls(BOOL bStatus)
{
	//Select Hand
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		m_ssSelectSheet[nHand].EnableWindow(bStatus);
	}

	//Spread Sheet
	m_ssBotCamCenterPos.EnableWindow(bStatus);
	m_ssTopCamToPkrDis.EnableWindow(bStatus);

	//Oper Button
	m_BtnOper1_EncoderRead.EnableWindow(bStatus);
	m_BtnOper1_MotorMove.EnableWindow(bStatus);
	m_BtnOper1_Reload.EnableWindow(bStatus);
	m_BtnOper1_Save.EnableWindow(bStatus);
	m_BtnOper1_MakeBasePos.EnableWindow(bStatus);
}


void CVATMain_CamCenterCalibration::ButtonClickedSpreadCamCenterCalLoadTraypp(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	_Com_ButtonClickedSpreadHand(Col, Row, VAT_HAND_LOAD_TRAYPP, eVAT_SSTYPE_HAND, m_editPosition);
}


void CVATMain_CamCenterCalibration::ButtonClickedSpreadCamCenterCalUnloadTraypp(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	_Com_ButtonClickedSpreadHand(Col, Row, VAT_HAND_UNLOAD_TRAYPP, eVAT_SSTYPE_HAND, m_editPosition);
}


void CVATMain_CamCenterCalibration::ButtonClickedSpreadCamCenterCalLoadTestpp1(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	_Com_ButtonClickedSpreadHand(Col, Row, VAT_HAND_LOAD_TESTPP1, eVAT_SSTYPE_HAND, m_editPosition);
}


void CVATMain_CamCenterCalibration::ButtonClickedSpreadCamCenterCalUnloadTestpp1(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	_Com_ButtonClickedSpreadHand(Col, Row, VAT_HAND_UNLOAD_TESTPP1, eVAT_SSTYPE_HAND, m_editPosition);
}


void CVATMain_CamCenterCalibration::ButtonClickedSpreadCamCenterCalLoadTestpp2(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	_Com_ButtonClickedSpreadHand(Col, Row, VAT_HAND_LOAD_TESTPP2, eVAT_SSTYPE_HAND, m_editPosition);
}


void CVATMain_CamCenterCalibration::ButtonClickedSpreadCamCenterCalUnloadTestpp2(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	_Com_ButtonClickedSpreadHand(Col, Row, VAT_HAND_UNLOAD_TESTPP2, eVAT_SSTYPE_HAND, m_editPosition);
}


void CVATMain_CamCenterCalibration::ClickSspanelBotCamPosData()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	//Set Display Type
	m_nSS1_DisplayType = eVAT_SSDISPLAYTYPE_DATA;

	//sspanel color 변경.
	_Com_ChangeSSPanelColor(m_ssDataPanel_1_Sub1, m_ssDataPanel_1_Sub2, m_nSS1_DisplayType);

	//Display Data
	DisplaySSData(_eCamCenCalSS_BotCamCenterPos, m_nSS1_DisplayType);
}


void CVATMain_CamCenterCalibration::ClickSspanelBotCamPosDataDiff()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	//Set Display Type
	m_nSS1_DisplayType = eVAT_SSDISPLAYTYPE_DIFFERENCE;

	//sspanel color 변경.
	_Com_ChangeSSPanelColor(m_ssDataPanel_1_Sub1, m_ssDataPanel_1_Sub2, m_nSS1_DisplayType);

	//Display Data
	DisplaySSData(_eCamCenCalSS_BotCamCenterPos, m_nSS1_DisplayType);
}

void CVATMain_CamCenterCalibration::ClickSspanelCamToPkrDisData()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	//Set Display Type
	m_nSS2_DisplayType = eVAT_SSDISPLAYTYPE_DATA;

	//sspanel color 변경.
	_Com_ChangeSSPanelColor(m_ssDataPanel_2_Sub1, m_ssDataPanel_2_Sub2, m_nSS2_DisplayType, eVAT_SSACCESSTYPE_RO);

	//Display Data
	DisplaySSData(_eCamCenCalSS_TopCamToPkrDis, m_nSS2_DisplayType);
}


void CVATMain_CamCenterCalibration::ClickSspanelCamToPkrDisDataDiff()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	//Set Display Type
	m_nSS2_DisplayType = eVAT_SSDISPLAYTYPE_DIFFERENCE;

	//sspanel color 변경.
	_Com_ChangeSSPanelColor(m_ssDataPanel_2_Sub1, m_ssDataPanel_2_Sub2, m_nSS2_DisplayType, eVAT_SSACCESSTYPE_RO);

	//Display Data
	DisplaySSData(_eCamCenCalSS_TopCamToPkrDis, m_nSS2_DisplayType);
}


void CVATMain_CamCenterCalibration::ClickSpreadCamCenterCalTargetPosition(long Col, long Row)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	_Com_UpdateSheetCellColor(m_ssBotCamCenterPos, m_stBotCamCenterPosLastInfo, Col, Row);
	_Com_UpdateLastSheetData(m_stBotCamCenterPosLastInfo, Col, Row);
	_Com_UpdateLastHandData(m_stBotCamCenterPosLastInfo, Row - 1);
}


void CVATMain_CamCenterCalibration::DblClickSpreadCamCenterCalTargetPosition(long Col, long Row)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (_Com_SSInterlock_DblClick(Col, Row, m_nSS1_DisplayType) != eVAT_INTERLOCK_PASS)
		return;

	_Com_DblClickSheetTextChange(m_ssBotCamCenterPos, m_stBotCamCenterPosLastInfo.nHand, this, Col, Row);

	int nHand = Row - 1;
	int nAxis = Col - 1;

	CString strCurValue = m_ssBotCamCenterPos.GetTextMatrix(Row, Col);
	m_stVATCamCenCalInfoGui.dBotCameraPosition[nHand][nAxis] = atof(strCurValue) * 1000.0;
}


void CVATMain_CamCenterCalibration::ClickSpreadTopCamToPickerDistance(long Col, long Row)
{
	//ReadOnly Sheet라서 색상 표기안함.
	if (g_DMCont.m_dmEQP.GN(NDM0_TEST_SW_ENGINEER_OPTION))
	{
		_Com_UpdateSheetCellColor(m_ssTopCamToPkrDis, m_stTopCamToPkrDisLastInfo, Col, Row);
		_Com_UpdateLastSheetData(m_stTopCamToPkrDisLastInfo, Col, Row);
		_Com_UpdateLastHandData(m_stTopCamToPkrDisLastInfo, Row - 1);
	}
	return;
}


void CVATMain_CamCenterCalibration::DblClickSpreadTopCamToPickerDistance(long Col, long Row)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//해당 Spread Sheet 수정 불가
	return;
// 	if (_Com_SSInterlock_DblClick(Col, Row, m_nSS2_DisplayType) != eVAT_INTERLOCK_PASS)
// 		return;
// 
// 	_Com_DblClickSheetTextChange(m_ssTopCamToPkrDis, this, Col, Row);
// 
// 	int nHand = Row - 1;
// 	int nAxis = Col - 1;
// 
// 	CString strCurValue = m_ssTopCamToPkrDis.GetTextMatrix(Row, Col);
// 	m_stVATCamCenCalInfoGui.dTopCamToPkrDistance[nHand][nAxis] = atof(strCurValue) * 1000.0;
}


void CVATMain_CamCenterCalibration::OnBnClickedBtnCamCenterEncoderRead1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	double dCurPos[VAT_MAX_AXIS_XYZ] = { 0.0, };

	int nRet = _Com_Oper_EncoderRead(dCurPos, m_stBotCamCenterPosLastInfo, m_nSS1_DisplayType/*Interlock Check용*/);
	if (nRet == ERR_VAT_ERROR)
		return;

	for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XY; nAxis++)
	{
		//Spread Sheet 표기.
		CString strCurPos = _T("");
		strCurPos.Format("%.3f", dCurPos[nAxis] / 1000.0);

		int nCol = nAxis + 1;
		m_ssBotCamCenterPos.SetTextMatrix(m_stBotCamCenterPosLastInfo.nRow, nCol, strCurPos); // 

		//Data Update
		m_stVATCamCenCalInfoGui.dBotCameraPosition[m_stBotCamCenterPosLastInfo.nHand][nAxis] = dCurPos[nAxis];
	}

}


void CVATMain_CamCenterCalibration::OnBnClickedBtnCamCenterMotorMove1()
{
	//Manual Cmd Move 모듈 함수 사용.
	_Com_ManualMove(VAT_SEQ_MODE_CAM_CAL, m_ssBotCamCenterPos, m_stBotCamCenterPosLastInfo, m_nSS1_DisplayType/*Interlock Check용*/);
}


void CVATMain_CamCenterCalibration::OnBnClickedBtnCamCenterReload1()
{
	//Alarm 비활성화 상태 Check
	_eVAT_INTERLOCK bChkInterlock = _Com_SSInterlock_AlarmDeActivate();
	if (bChkInterlock != eVAT_INTERLOCK_PASS)
		return;

	//"Do you want to Base Position Reload?"
	int nRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_BASE_POSITION_RELOAD, ERR_VAT_TYPE_YESNO);
	if (nRet == IDNO)
		return;

	//REAL -> GUI
	COPY_VAT_ARRAY(m_stVATCamCenCalInfoGui.dBotCameraPosition, g_VATCommonData.m_stVATCamCenterCalInfo.dBotCameraPosition);
	
	DisplaySSData(_eCamCenCalSS_BotCamCenterPos, m_nSS1_DisplayType);

	//Spread Sheet 초기화.
	_Com_Clear_SpreadSheet(m_ssBotCamCenterPos, m_stBotCamCenterPosLastInfo);
}

/*========================================================================*/
//COMMENT :	GUI Data를 Buffer Data에 Update.
//          ※GUI Data Update 시점.
//				-. Read Encoder 에서 GUI Data Update 진행.
//				-. DblClick시 GUI Data Update 진행.
/*========================================================================*/
void CVATMain_CamCenterCalibration::OnBnClickedBtnCamCenterDataSave1()
{
	//Alarm 비활성화 상태 Check
	_eVAT_INTERLOCK bChkInterlock = _Com_SSInterlock_AlarmDeActivate();
	if (bChkInterlock != eVAT_INTERLOCK_PASS)
		return;

	//"Do you want to Base Position Save?"
	int nRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_DATA_SAVE, ERR_VAT_TYPE_YESNO);
	if (nRet == IDNO)
		return;

	int nLimitCheck = CheckPosLimit();
	if (nLimitCheck == TRUE)
	{
		int nLimitCheckRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_LIMIT_OVER_CHECK, ERR_VAT_TYPE_YESNO);
		if (nLimitCheckRet == IDNO)
		{
			return;
		}
	}

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XYZ; nAxis++) {
			//GUI <-> Real 다른 값만 Buffer에 저장.(작업자 수정 되었다는 의미)
			//※저장 시, 마다 Buffer에 저장할 경우 이전과 차이 Data가 0으로 변경 되는 문제.
			double dRealData = g_VATCommonData.m_stVATCamCenterCalInfo.dBotCameraPosition[nHand][nAxis];
			double dGuiData = m_stVATCamCenCalInfoGui.dBotCameraPosition[nHand][nAxis];
			if (dRealData != dGuiData) {
				// Copy 순서 변경하지 말것. [9/24/2025 dohyeong.kim]
				//1. [REAL -> BUFFER]
				g_VATCommonData.m_stVATCamCenterCalInfoBuff.dBotCameraPosition[nHand][nAxis] = g_VATCommonData.m_stVATCamCenterCalInfo.dBotCameraPosition[nHand][nAxis];

				//2. [GUI -> REAL]
				g_VATCommonData.m_stVATCamCenterCalInfo.dBotCameraPosition[nHand][nAxis] = m_stVATCamCenCalInfoGui.dBotCameraPosition[nHand][nAxis];

				//3. [ini File 저장]
				g_VATCommonData.SaveEachCamCenterCalInformation(eCamCenCal_BotCamCenterPos, nHand, nAxis);
			}
		}
	}

	DisplaySSData(_eCamCenCalSS_BotCamCenterPos, m_nSS1_DisplayType);

	//Spread Sheet 초기화.
	_Com_Clear_SpreadSheet(m_ssBotCamCenterPos, m_stBotCamCenterPosLastInfo);
}

//============================================================================================================
//COMMENT : Hand Status Update 함수. 
//           ※ PostMessage로 전달해야 함.
//Param   : wParam -> Hand Index
//          lParam 사용 하지 않음.
//           ※ Message Q에 쌓여 있는동안 상태 변경이 있을 수 있기 때문에 SingleTone Instance 사용.(최신 상태 표기)
//			 ※ 멤버 변수에만 저장 후, 진행 상태 Timer에서 표기.
//              -. 진행 상태 OnGoing의 경우, 깜빡임 표기 하기 위함.
//============================================================================================================
LRESULT CVATMain_CamCenterCalibration::OnReceiveUpdateHandStatus(WPARAM wParam, LPARAM lParam)
{
	int nHand = (int)wParam;

	if (!(VAT_HAND_LOAD_TRAYPP <= nHand && nHand <= VAT_HAND_UNLOAD_TESTPP2))
		return 0L;

	m_nGuiHandStatus[nHand] = GuiNotifier->GetHandStatus(nHand);

	return 0L;
}

//============================================================================================================
//COMMENT : 모든 Hand 측정에 대한 진행률 상태 Update 함수.(Progress) 
//           ※ PostMessage로 전달해야 함.
//Param   : wParam -> Hand Index
//          lParam 사용 하지 않음.
//           ※ Message Q에 쌓여 있는동안 상태 변경이 있을 수 있기 때문에 SingleTone Instance 사용.(최신 상태 표기)
//============================================================================================================
LRESULT CVATMain_CamCenterCalibration::OnReceiveUpdateProgress(WPARAM wParam, LPARAM lParam)
{
	int nHand = (int)wParam;

	if (!(VAT_HAND_LOAD_TRAYPP <= nHand && nHand <= VAT_HAND_UNLOAD_TESTPP2))
		return 0L;

	double dProgressPercent = GuiNotifier->GetProgressPercent(nHand);

	m_Progress[nHand].SetPos(static_cast<int>(dProgressPercent));

	CString strPercent;
	strPercent.Format(_T("%.0f%c"), dProgressPercent, _T('%'));
	m_Percent[nHand].SetWindowText(strPercent);

	return 0L;
}

//============================================================================================================
//COMMENT : Vision Cam Status Update 함수.
//           ※ PostMessage로 전달해야 함.
//Param   : wParam -> Cam Index
//          lParam 사용 하지 않음.
//           ※ Message Q에 쌓여 있는동안 상태 변경이 있을 수 있기 때문에 SingleTone Instance 사용.(최신 상태 표기)
//============================================================================================================
LRESULT CVATMain_CamCenterCalibration::OnReceiveUpdateVisionStatus(WPARAM wParam, LPARAM lParam)
{
	int nCam = (int)wParam;

	if (!(VAT_CAM_LD_TRAY_PP_UPPER <= nCam && nCam <= VAT_CAM_TEST_PP_2_LOWER))
		return 0L;

	int nStatus = GuiNotifier->GetVisionStatus(nCam);

	int nFrame = _Com_ConvertCamStatusToFrameIdx(nStatus);
	int nHand = _Com_ConvertLowerCamToHandIdx(nCam);

	//Cam 상태 초기화 시, 상단 Cam 예외처리.
	if (nHand == -1) {
		return 0L;
	}

	CDSSPanel* chkbt = (CDSSPanel*)GetDlgItem(m2_nCamStatus[nHand]);
	chkbt->SetStatus(nFrame);

	return 0L;
}

//============================================================================================================
//COMMENT : Vision Cam Offset X/Y Update 함수.
//           ※ PostMessage로 전달해야 함.
//Param   : wParam -> Cam Index
//          lParam 사용 하지 않음.
//           ※ Message Q에 쌓여 있는동안 상태 변경이 있을 수 있기 때문에 SingleTone Instance 사용.(최신 상태 표기)
//=============================================================================================================
LRESULT CVATMain_CamCenterCalibration::OnReceiveUpdateVisionOffset(WPARAM wParam, LPARAM lParam)
{
	int nCam = (int)wParam;

	if (!(VAT_CAM_LD_TRAY_PP_UPPER <= nCam && nCam <= VAT_CAM_TEST_PP_2_LOWER))
		return 0L;

	std::pair<double, double> pOffset = GuiNotifier->GetVisionOffset(nCam);


// 	m_dOffsetX = pOffset.first;
// 	m_dOffsetY = pOffset.second;

	int nHand = _Com_ConvertLowerCamToHandIdx(nCam);

	//Cam 상태 초기화 시, 상단 Cam 예외처리.
	if (nHand == -1) {
		return 0L;
	}

	//CVAT_PickerAlignDlgCom 변수저장.
	m_pOffset[nHand] = pOffset;

	InvalidateRect(&m_rect[nHand]);				//Offset Graphic View 새로 고침.
	InvalidateRect(&m_rectData[nHand][VAT_X]);	//Offset Data X 새로 고침.
	InvalidateRect(&m_rectData[nHand][VAT_Y]);	//Offset Data Y 새로 고침.

	return 0L;
}


//============================================================================================================
//COMMENT : Vision Cam 측정 Count 표기
//           1. 실제 Vision Measure Cnt
//           2. Cam Auto Focus의 경우, -1로 Data를 받으며 'F'로 표기 됨.
//Param   : wParam -> Cam Index
//          lParam 사용 하지 않음.
//           ※ Message Q에 쌓여 있는동안 상태 변경이 있을 수 있기 때문에 SingleTone Instance 사용.(최신 상태 표기)
//=============================================================================================================
LRESULT CVATMain_CamCenterCalibration::OnReceiveUpdateVisionMeasureCnt(WPARAM wParam, LPARAM lParam)
{
	int nCam = (int)wParam;

	if (!(VAT_CAM_LD_TRAY_PP_UPPER <= nCam && nCam <= VAT_CAM_TEST_PP_2_LOWER))
		return 0L;
	
	int nVisionMeasureCnt = GuiNotifier->GetVisionMeasureCnt(nCam);

	int nHand = _Com_ConvertLowerCamToHandIdx(nCam);

	//Cam 상태 초기화 시, 상단 Cam 예외처리.
	if (nHand == -1) {
		return 0L;
	}

	CDSSPanel* chkbt = (CDSSPanel*)GetDlgItem(m2_nCamStatus[nHand]);

	CString strVisionRetryCnt = _T("");
	strVisionRetryCnt.Format("%d", nVisionMeasureCnt); //일반 표기

	chkbt->SetWindowText(strVisionRetryCnt);

	return 0L;
}


//============================================================================================================
//COMMENT : 측정 완료 된, Data Update 진행.
//Param   : wParam -> Cam Index
//          lParam 사용 하지 않음.
//           ※ Message Q에 쌓여 있는동안 상태 변경이 있을 수 있기 때문에 SingleTone Instance 사용.(최신 상태 표기)
//=============================================================================================================
LRESULT CVATMain_CamCenterCalibration::OnReceiveUpdateSavedData(WPARAM wParam, LPARAM lParam)
{

	//[Real -> GUI Update]
	//1. Plate Jig Target Position
	COPY_VAT_ARRAY(m_stVATCamCenCalInfoGui.dBotCameraPosition, g_VATCommonData.m_stVATCamCenterCalInfo.dBotCameraPosition);
	DisplaySSData(_eCamCenCalSS_BotCamCenterPos, m_nSS1_DisplayType);

	//2. Plate Jig Vacuum Position
	COPY_VAT_ARRAY(m_stVATCamCenCalInfoGui.dTopCamToPkrDistance, g_VATCommonData.m_stVATCamCenterCalInfo.dTopCamToPkrDistance);
	DisplaySSData(_eCamCenCalSS_TopCamToPkrDis, m_nSS2_DisplayType);

	return 0L;
}

void CVATMain_CamCenterCalibration::OnBnClickedBtnCamCenterMakeBasePos1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int nRet = _Com_Oper_MakeBasePos(m_nSS1_DisplayType/*Interlock Check용*/);
	if (nRet == ERR_VAT_ERROR)
		return;

	double dPickerCamCenter_TargetPos[VAT_HAND_MAX][VAT_MAX_AXIS_XYZ] = { 0 };
 	g_DBVAT.SELECT_VAT_CAM_CENTER_CAL(dPickerCamCenter_TargetPos);
 
 	COPY_VAT_ARRAY(m_stVATCamCenCalInfoGui.dBotCameraPosition, dPickerCamCenter_TargetPos);
 
 	DisplaySSData(_eCamCenCalSS_BotCamCenterPos, m_nSS1_DisplayType, TRUE);
}


void CVATMain_CamCenterCalibration::OnBnClickedBtnCamCenterMotorMove2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
// 	CSpreadSheet ssPlateTargetPos;
// 
// 	long lCols = m_ssTopCamToPkrDis.GetMaxCols();
// 	long lRows = m_ssTopCamToPkrDis.GetMaxRows();
// 
// 	ssPlateTargetPos.Create(NULL, WS_CHILD, CRect(0, 0, 0, 0), this, 0);
// 	ssPlateTargetPos.CopyRange(0,0, lCols-1, lRows-1,0,0);
// 
// 	for (auto nHand : m_vAllHand)
// 	{
// 		double dPlateTargetPos[VAT_MAX_AXIS_XYZ] = { 0, };
// 		int nRow = nHand + 1;
// 
// 		for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XY; nAxis++)
// 		{
// 			double dPos = g_VATCommonData.m_stVATZFocusInfo.dPlateJIG_TargetPos[nHand][nAxis];
// 			double dDistance = g_VATCommonData.m_stVATCamCenterCalInfo.dTopCamToPkrDistance[nHand][nAxis];
// 
// 			dPlateTargetPos[nAxis] = dPos + dDistance;
// 
// 			CString strPos = _T("");
// 			strPos.Format("%.3f", dPlateTargetPos[nAxis] / 1000.0);
// 			int nCol = nAxis + 1;
// 
// 			ssPlateTargetPos.SetTextMatrix(nRow, nCol, strPos);
// 		}
// 
// 		dPlateTargetPos[VAT_Z] = g_VATCommonData.m_stVATZFocusInfo.dPlateJIG_TargetPos[nHand][VAT_Z];
// 		CString strPos = _T("");
// 		strPos.Format("%.3f", dPlateTargetPos[VAT_Z] / 1000.0);
// 		ssPlateTargetPos.SetTextMatrix(nRow, 3, strPos);
// 	}

	//Manual Cmd Move 모듈 함수 사용.
	_Com_ManualMove(VAT_SEQ_MODE_CAM_CAL, m_ssTopCamToPkrDis, m_stTopCamToPkrDisLastInfo, m_nSS2_DisplayType/*Interlock Check용*/);
}
