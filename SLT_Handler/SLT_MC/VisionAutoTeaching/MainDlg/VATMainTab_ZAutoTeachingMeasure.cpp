// VATMainTab_ZAutoTeachingMeasure.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "VATMainTab_ZAutoTeachingMeasure.h"
#include "afxdialogex.h"
#include "VAT_STATIC_ID.h"

// CVATMainTab_ZAutoTeachingMeasure 대화 상자입니다.
#define UM_VAT_SEQ_MSG_MEASURE_COMPLETE	  (WM_USER + 5000)

IMPLEMENT_DYNAMIC(CVATMainTab_ZAutoTeachingMeasure, CDialogEx)

CVATMainTab_ZAutoTeachingMeasure::CVATMainTab_ZAutoTeachingMeasure(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VAT_MAIN_Z_AUTO_TEACHING_MEASURE_DLG, pParent)
{
	m_nBoardDir = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);

	//Last Data Type Init
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		m_nLastDataType[nHand] = eDataType_Offset;
	}

	//Data Init
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		for (int nPkr = 0; nPkr < VAT_PICKER_MAX; nPkr++) {
			m_dCtrlPickerData[nHand][nPkr] = 0.0;
		}
	}

	m_bBlockEditFocus = FALSE;
}

CVATMainTab_ZAutoTeachingMeasure::~CVATMainTab_ZAutoTeachingMeasure()
{
}

void CVATMainTab_ZAutoTeachingMeasure::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDC_SSPANEL_LD_TRAYPP_PART, m_ssPanel_LD_TrayPP);
	DDX_Control(pDX, IDC_SSPANEL_UD_TRAYPP_PART, m_ssPanel_UD_TrayPP);
	DDX_Control(pDX, IDC_SSPANEL_LD_TESTPP_1_PART, m_ssPanel_LD_TestPP1);
	DDX_Control(pDX, IDC_SSPANEL_UD_TESTPP_1_PART, m_ssPanel_UD_TestPP1);
	DDX_Control(pDX, IDC_SSPANEL_LD_TESTPP_2_PART, m_ssPanel_LD_TestPP2);
	DDX_Control(pDX, IDC_SSPANEL_UD_TESTPP_2_PART, m_ssPanel_UD_TestPP2);


	DDX_Control(pDX, IDC_SPREAD_LD_TRAYPP_PART, m_ssSelectSheet[VAT_HAND_LOAD_TRAYPP]);
	DDX_Control(pDX, IDC_SPREAD_UD_TRAYPP_PART, m_ssSelectSheet[VAT_HAND_UNLOAD_TRAYPP]);
	DDX_Control(pDX, IDC_SPREAD_LD_TESTPP_1_PART, m_ssSelectSheet[VAT_HAND_LOAD_TESTPP1]);
	DDX_Control(pDX, IDC_SPREAD_UD_TESTPP_1_PART, m_ssSelectSheet[VAT_HAND_UNLOAD_TESTPP1]);
	DDX_Control(pDX, IDC_SPREAD_LD_TESTPP_2_PART, m_ssSelectSheet[VAT_HAND_LOAD_TESTPP2]);
	DDX_Control(pDX, IDC_SPREAD_UD_TESTPP_2_PART, m_ssSelectSheet[VAT_HAND_UNLOAD_TESTPP2]);

	DDX_Control(pDX, IDC_COMBO_LD_TRAY_PP_POS, m_ComboPos[VAT_HAND_LOAD_TRAYPP]);
	DDX_Control(pDX, IDC_COMBO_UD_TRAY_PP_POS, m_ComboPos[VAT_HAND_UNLOAD_TRAYPP]);
	DDX_Control(pDX, IDC_COMBO_LD_TEST_PP_1_POS, m_ComboPos[VAT_HAND_LOAD_TESTPP1]);
	DDX_Control(pDX, IDC_COMBO_UD_TEST_PP_1_POS, m_ComboPos[VAT_HAND_UNLOAD_TESTPP1]);
	DDX_Control(pDX, IDC_COMBO_LD_TEST_PP_2_POS, m_ComboPos[VAT_HAND_LOAD_TESTPP2]);
	DDX_Control(pDX, IDC_COMBO_UD_TEST_PP_2_POS, m_ComboPos[VAT_HAND_UNLOAD_TESTPP2]);

	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++) {
		for (int nPkr = 0; nPkr < VAT_PICKER_MAX; nPkr++) {
			//Controll Data 저장
			DDX_Text(pDX, m4_nDataStaticIdx[nHand][nPkr], m_dCtrlPickerData[nHand][nPkr]);

			//Controll Design
			DDX_Control(pDX, m4_nDataStaticIdx[nHand][nPkr], m_EditPickerData[nHand][nPkr]);
		}

		DDX_Control(pDX, IDC_BTN_Z_AUTO_TEACHING_RELOAD_1 + nHand, m_BtnReload[nHand]);
		DDX_Control(pDX, IDC_BTN_Z_AUTO_TEACHING_SAVE_1 + nHand, m_BtnSave[nHand]);

	}

	//DDX_Control(pDX, IDC_LD_TRAYPP_DATA_1, m_EditTest);

}


BEGIN_MESSAGE_MAP(CVATMainTab_ZAutoTeachingMeasure, CDialog)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(UM_VAT_SEQ_MSG_MEASURE_COMPLETE, &CVATMainTab_ZAutoTeachingMeasure::OnReceiveMeasureComplete)
	ON_CBN_SELCHANGE(IDC_COMBO_LD_TRAY_PP_POS, &CVATMainTab_ZAutoTeachingMeasure::OnCbnSelchangeComboLdTrayPpPos)
	ON_CBN_SELCHANGE(IDC_COMBO_UD_TRAY_PP_POS, &CVATMainTab_ZAutoTeachingMeasure::OnCbnSelchangeComboUdTrayPpPos)
	ON_CBN_SELCHANGE(IDC_COMBO_LD_TEST_PP_1_POS, &CVATMainTab_ZAutoTeachingMeasure::OnCbnSelchangeComboLdTestPp1Pos)
	ON_CBN_SELCHANGE(IDC_COMBO_UD_TEST_PP_1_POS, &CVATMainTab_ZAutoTeachingMeasure::OnCbnSelchangeComboUdTestPp1Pos)
	ON_CBN_SELCHANGE(IDC_COMBO_LD_TEST_PP_2_POS, &CVATMainTab_ZAutoTeachingMeasure::OnCbnSelchangeComboLdTestPp2Pos)
	ON_CBN_SELCHANGE(IDC_COMBO_UD_TEST_PP_2_POS, &CVATMainTab_ZAutoTeachingMeasure::OnCbnSelchangeComboUdTestPp2Pos)


	//Flatness Offset
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_LD_TRAYPP_DATA_1, IDC_UD_TESTPP_2_DATA_8, &CVATMainTab_ZAutoTeachingMeasure::OnEnSetFocusHandData)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_Z_AUTO_TEACHING_RELOAD_1, IDC_BTN_Z_AUTO_TEACHING_RELOAD_6, &CVATMainTab_ZAutoTeachingMeasure::OnBtnClickedFlatnessOffsetReload)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_Z_AUTO_TEACHING_SAVE_1, IDC_BTN_Z_AUTO_TEACHING_SAVE_6, &CVATMainTab_ZAutoTeachingMeasure::OnBtnClickedFlatnessOffsetSave)

END_MESSAGE_MAP()



BOOL CVATMainTab_ZAutoTeachingMeasure::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CWnd *wnd = GetFocus();

	//Data Edit Size&Color
	InitEditDesign();

	InitCtrlButton();

	_Com_InitSSHandPanel_PartType();

	_Com_InitPartSelectSheet();

	_Com_InitComboPos();

	//Picker 관련 data 표기.
	DisplayPickerDataAll(eDataType_Offset);
	m_nBoardDir = g_DMCont.m_dmEQP.GN(NDM0_BoardDirection);

	//controll -> 변수(Flateness Offset)
	UpdateData(TRUE); 



	//DM 초기화.
	for (auto _auto_task : g_VatTaskSystemCtrl.m_vAllTaskAuto)
	{
		int nHand = _auto_task->m_nHand;
		int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
		for (int nPart = 0; nPart < nMaxPart; nPart++) {
			_auto_task->_SetMeasurePart_Status_DM(nPart, VAT_STATUS_MEASURE_INIT);
		}
	}

	return TRUE;
}

BOOL CVATMainTab_ZAutoTeachingMeasure::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}

//======================================================================
//COMMENT : Mian Page 변경 시, Tab 필요 사항들 초기화
//			1) Set Timer  			
//			2) 
//======================================================================
void CVATMainTab_ZAutoTeachingMeasure::PageMoveInit_Tab()
{
	//Timer Set
	for (int i = 0; i < _eZTeachMeasureT_Max; i++) {
		SetTimer(i, 200, NULL);
	}
	
// 	//Data Load
// 	g_VATCommonData.m_stVATZTeachInfo;

	//Picker 관련 data 표기.
	DisplayPickerDataAll(eDataType_Offset);
}


//======================================================================
//COMMENT : Mian Page 변경 시, 불필요 사항들 제거
//			1) Set Timer  			
//			2) 
//======================================================================
void CVATMainTab_ZAutoTeachingMeasure::PageMoveDispose_Tab()
{
	//Kill Timer Set
 	for (int i = 0; i < _eZTeachMeasureT_Max; i++){
 		KillTimer(i);
 	}
}


//======================================================================
//COMMENT : Timer 제거
//======================================================================
void CVATMainTab_ZAutoTeachingMeasure::OnDestroy()
{
	CDialogEx::OnDestroy();

	for (int i = 0; i < _eZTeachMeasureT_Max; i++) {
		KillTimer(i);
	}

// 	KillTimer(eTimer_CurrentPos);
// 	KillTimer(eTimer_ChkVacuum);
}

void CVATMainTab_ZAutoTeachingMeasure::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);
	//Lot 상태에 따른 GUI 상태
	//Current Position
	//Vacuum Position

	switch (nIDEvent)
	{		
		case _eZTeachMeasureT_CurrentPos:
		{
			KillTimer(nIDEvent);

			//현재 Encoder Position 표기.
			_tDisplayCurrentPos(); 

			SetTimer(nIDEvent, 100, NULL);
		}break;
		case _eZTeachMeasureT_ChkVacuum:
		{
			KillTimer(nIDEvent);
			//Picker Sensing 확인.
			_tChkVacuumSen();

			SetTimer(nIDEvent, 100, NULL);
		}break;
		case _eZTeachMeasureT_CheckExecuteStatus:
		{
			KillTimer(nIDEvent);

			//상태 지속적으로 변경 되는 것을 방지하고자 Last status check 진행.
			if (g_bVATStatusExecute != m_bLastExecuteStatus)
			{
				EnableControls_MeasureTab(!g_bVATStatusExecute);
				m_bLastExecuteStatus = g_bVATStatusExecute;
			}

			SetTimer(nIDEvent, 200, NULL);
		}break;
		case _eZTeachMeasureT_PartStatus:
		{
			KillTimer(nIDEvent);

			_tDisplayPartStatus();

			SetTimer(nIDEvent, 500, NULL);
		}break;
		default:
			break;
	}
}


void CVATMainTab_ZAutoTeachingMeasure::_tDisplayCurrentPos()
{
	CString AxisPos = _T("");

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{
		CBbxmnpClient::_ePICK_MOTOR eMotor;
		switch (nHand)
		{
		case VAT_HAND_LOAD_TRAYPP:		{eMotor = CBbxmnpClient::PICK_MOTOR_LOAD_TRAY;}		break;
		case VAT_HAND_UNLOAD_TRAYPP:	{eMotor = CBbxmnpClient::PICK_MOTOR_UNLOAD_TRAY;}	break;
		case VAT_HAND_LOAD_TESTPP1:		{eMotor = CBbxmnpClient::PICK_MOTOR_LOAD_TEST_1;}	break;
		case VAT_HAND_UNLOAD_TESTPP1:	{eMotor = CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_1;}	break;
		case VAT_HAND_LOAD_TESTPP2:		{eMotor = CBbxmnpClient::PICK_MOTOR_LOAD_TEST_2;}	break;
		case VAT_HAND_UNLOAD_TESTPP2:	{eMotor = CBbxmnpClient::PICK_MOTOR_UNLOAD_TEST_2;}	break;
		default:
			break;
		}

		//8개 Picker 축 Idx
		std::vector<int> vBMAxis;
		vBMAxis.clear();
		MakeBMAxisPickerIdx(vBMAxis, eMotor);


		for (int nPkrIdx = 0; nPkrIdx < eBMAxis_Picker_MAX; nPkrIdx++)
		{
			int nGuiIdx = nPkrIdx;

			//Test Pp의 경우 GUI 표기를 실장비 방향과 맞춰 준다.
			//해당 방향은 장비 좌 우측 모니터에서 Test PP를 바라 봤을때의 Picke 위치.
			if (VAT_HAND_LOAD_TESTPP1 <= nHand && nHand <= VAT_HAND_UNLOAD_TESTPP2)
			{
				//Forward는 vBMAxis 그대로 담아서, 역으로 보여줌.
				if (m_nBoardDir == eBoardDir_Forward) {
					nGuiIdx = abs(nPkrIdx - 7);
				}
				else { //nBoardType == eBoardDir_Backward	
					//Backward는 vBMAxis 역으로 담아서, 그대로 1번부터 보여줌.
				}
			}


			AxisPos.Format("%7.2f", (g_pBbxmnpClient->GetActualPos(eMotor, (_eBbxmnpAxis)vBMAxis[nGuiIdx])) / 1000);
			GetDlgItem(m4_nCurPosStaticIdx[nHand][nPkrIdx])->SetWindowText(AxisPos);
		}
	}
}


void CVATMainTab_ZAutoTeachingMeasure::MakeBMAxisPickerIdx(std::vector<int>& vPicker, CBbxmnpClient::_ePICK_MOTOR eMotor)
{
	int nPicker_start = g_pBbxmnpClient->ZAxisStartPicker(eMotor);
	std::vector<int> vPickerFor;
	int nCount = 0;

	int nStationPara = g_DMCont.m_dmEQP.GN(NDM11_PARA_PER_STATION);
	for (int y = 0; y < MAX_TRAY_PP_CNT_Y; y++) {
		for (int x = 0; x < MAX_TRAY_PP_CNT_X; x++) {
			vPickerFor.push_back(nPicker_start + (x + MAX_TRAY_PP_CNT_X * y));
			nCount++;
		}
	}


	if (g_DMCont.m_dmEQP.GN(NDM0_BoardDirection) == eBoardDir_Backward)
	{
		//vPicker = vPickerFor;

		if (CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_LOAD_TRAY == eMotor || CBbxmnpClient::_ePICK_MOTOR::PICK_MOTOR_UNLOAD_TRAY == eMotor)
		{
			vPicker = vPickerFor;
		}
		else
		{
			for (int i = 0; i < nCount; i++)
			{
				vPicker.push_back(vPickerFor[(vPickerFor.size() - 1) - i]);
			}
		}
	}
	else
	{
		vPicker = vPickerFor;
	}
}


void CVATMainTab_ZAutoTeachingMeasure::_tChkVacuumSen()
{
	CDSSPanel* chkbt = NULL;

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{
		for (int y = 0; y < MAX_TEST_PP_PKR_CNT_Y; y++)
		{
			for (int x = 0; x < MAX_TEST_PP_PKR_CNT_X; x++)
			{
				int vacuum_sen = GetPickerVacuum(nHand, CPoint(x, y));


				int nGuiIdx = x + (y * 4);
				if (VAT_HAND_LOAD_TESTPP1 < nHand && nHand < VAT_HAND_UNLOAD_TESTPP2) {
					if (m_nBoardDir == eBoardDir_Forward) {
						nGuiIdx = abs((x + 4 * y) - 7);
					}
					else { //nBoardType == eBoardDir_Backward			
						nGuiIdx = x + 4 * y;
					}
				}

				chkbt = (CDSSPanel*)GetDlgItem(m4_nVacStaticIdx[nHand][nGuiIdx]);
				if (vacuum_sen)		chkbt->On();
				else				chkbt->Off();
			}
		}
	}
}


int CVATMainTab_ZAutoTeachingMeasure::GetPickerVacuum(int nHand, CPoint picker)
{
	int nChkVacuum = 0;

	switch (nHand)
	{
	case VAT_HAND_LOAD_TRAYPP:
	{
		nChkVacuum = g_TaskTrayLoadPP.GetVacuum(CPoint(picker.x, picker.y));
	}break;
	case VAT_HAND_UNLOAD_TRAYPP:
	{
		nChkVacuum = g_TaskTrayUnloadPP.GetVacuum(CPoint(picker.x, picker.y));
	}break;
	case VAT_HAND_LOAD_TESTPP1:
	{
		nChkVacuum = g_TaskTestPP[eTestPP_1].GetVacuum(eTestPPLoadType, CPoint(picker.x, picker.y));
	}break;
	case VAT_HAND_UNLOAD_TESTPP1:
	{
		nChkVacuum = g_TaskTestPP[eTestPP_1].GetVacuum(eTestPPUnLoadType, CPoint(picker.x, picker.y));
	}break;
	case VAT_HAND_LOAD_TESTPP2:
	{
		nChkVacuum = g_TaskTestPP[eTestPP_2].GetVacuum(eTestPPLoadType, CPoint(picker.x, picker.y));
	}break;
	case VAT_HAND_UNLOAD_TESTPP2:	{
		nChkVacuum = g_TaskTestPP[eTestPP_2].GetVacuum(eTestPPUnLoadType, CPoint(picker.x, picker.y)); 
	} break;
	default:
		break;
	}

	return nChkVacuum;

}


void CVATMainTab_ZAutoTeachingMeasure::_tDisplayPartStatus()
{
	//시퀀스 측정일 경우에만 DM으로 상태 표기 진행.
	int nCurLotState = g_VatTaskAutoSystem.GetCurrentLotState();
	if (nCurLotState == eAutoLotState_InitNeed) {
		return;
	}

	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++)
	{
		int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
		for (int nPart = 0; nPart < nMaxPart; nPart++) {
			_Com_SSDisplayPartStatusByDM(VAT_SEQ_MODE_Z_TEACHING, nHand, nPart);
		}
	}
}


void CVATMainTab_ZAutoTeachingMeasure::ChangeDataPanelColor(int nHand, int nType)
{
	int select_panel_type = nType;
	int unselect_panel_type = (select_panel_type == eDataType_Offset) ? eDataType_Vacuum : eDataType_Offset;

	//Panel 배경색 초기화.
	((CSSCommand *)GetDlgItem(m4_nDataPanelIdx[nHand][select_panel_type]))->SetBackColor(TAB_BACKGROUND_COLOR);
	((CSSCommand *)GetDlgItem(m4_nDataPanelIdx[nHand][unselect_panel_type]))->SetBackColor(TAB_BACKGROUND_COLOR);

	//Fore Color 변경.
	((CSSCommand *)GetDlgItem(m4_nDataPanelIdx[nHand][select_panel_type]))->SetForeColor(BBLACK);
	((CSSCommand *)GetDlgItem(m4_nDataPanelIdx[nHand][unselect_panel_type]))->SetForeColor(TAB_UNSELECT_FORE_COLOR);
}


void CVATMainTab_ZAutoTeachingMeasure::DisplayPickerData(int nHand, int nType)
{
	//Display Type Back Up
	if (m_nLastDataType[nHand] != nType) {
		m_nLastDataType[nHand] = nType;
	}

	//Panel 색상 변경.
	ChangeDataPanelColor(nHand, nType);

	//Type에 따른 Data 변경.
	int nPartIdx = _Com_GetPartIdxfromComboBox(nHand);

	CString strDisplayEnc = _T("");

	for (int nPkrIdx = 0; nPkrIdx < VAT_PICKER_MAX; nPkrIdx++) 
	{
		double dDisplayEnc = 0.0;
		if (nType == eDataType_Offset) {
			dDisplayEnc = g_VATCommonData.m_stVATZTeachInfo.stPickerGap.getPickerGap(nHand, nPartIdx, nPkrIdx);			
		}
		else { //nType == eDataType_Vacuum
			dDisplayEnc = g_VATCommonData.m_stVATZTeachInfo.stPickerVacPos.getPickerVacPos(nHand, nPartIdx, nPkrIdx);
		}

		CEdit *editEnc = (CEdit *)GetDlgItem(m4_nDataStaticIdx[nHand][nPkrIdx]);
		strDisplayEnc.Format("%7.3f", dDisplayEnc / 1000.0);
		editEnc->SetWindowText(strDisplayEnc);

		//editEnc->cursor
	}
}


void CVATMainTab_ZAutoTeachingMeasure::DisplayPickerDataAll(int nType)
{
	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++) {
		DisplayPickerData(nHand, nType);
	}
}


void CVATMainTab_ZAutoTeachingMeasure::EnableControls_MeasureTab(BOOL bStatus)
{
	// Control Enable/Disable 처리 시, Focus가 활성화 되는 경우 발생.
	m_bBlockEditFocus = TRUE;

	// Hand Panel
	GetDlgItem(IDC_SSPANEL_LD_TRAYPP_PART)->EnableWindow(bStatus);
	GetDlgItem(IDC_SSPANEL_UD_TRAYPP_PART)->EnableWindow(bStatus);
	GetDlgItem(IDC_SSPANEL_LD_TESTPP_1_PART)->EnableWindow(bStatus);
	GetDlgItem(IDC_SSPANEL_UD_TESTPP_1_PART)->EnableWindow(bStatus);
	GetDlgItem(IDC_SSPANEL_LD_TESTPP_2_PART)->EnableWindow(bStatus);
	GetDlgItem(IDC_SSPANEL_UD_TESTPP_2_PART)->EnableWindow(bStatus);


	// Selected Part Spread Sheet 
	GetDlgItem(IDC_SPREAD_LD_TRAYPP_PART)->EnableWindow(bStatus);
	GetDlgItem(IDC_SPREAD_UD_TRAYPP_PART)->EnableWindow(bStatus);
	GetDlgItem(IDC_SPREAD_LD_TESTPP_1_PART)->EnableWindow(bStatus);
	GetDlgItem(IDC_SPREAD_UD_TESTPP_1_PART)->EnableWindow(bStatus);
	GetDlgItem(IDC_SPREAD_LD_TESTPP_2_PART)->EnableWindow(bStatus);
	GetDlgItem(IDC_SPREAD_UD_TESTPP_2_PART)->EnableWindow(bStatus);

	// Gap Setting
	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++) {
		for (int nPkr = 0; nPkr < VAT_PICKER_MAX; nPkr++) {
			m_EditPickerData[nHand][nPkr].EnableWindow(bStatus);
		}
	}

	// Reload/Save
	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++) {
		m_BtnReload[nHand].EnableWindow(bStatus);
		m_BtnSave[nHand].EnableWindow(bStatus);
	}
	
	m_bBlockEditFocus = FALSE;
}


BEGIN_EVENTSINK_MAP(CVATMainTab_ZAutoTeachingMeasure, CDialogEx)
	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SPREAD_LD_TRAYPP_PART, 3, CVATMainTab_ZAutoTeachingMeasure::ButtonClickedSpreadLdTrayppPart, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SPREAD_UD_TRAYPP_PART, 3, CVATMainTab_ZAutoTeachingMeasure::ButtonClickedSpreadUdTrayppPart, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SPREAD_LD_TESTPP_1_PART, 3, CVATMainTab_ZAutoTeachingMeasure::ButtonClickedSpreadLdTestpp1Part, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SPREAD_UD_TESTPP_1_PART, 3, CVATMainTab_ZAutoTeachingMeasure::ButtonClickedSpreadUdTestpp1Part, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SPREAD_LD_TESTPP_2_PART, 3, CVATMainTab_ZAutoTeachingMeasure::ButtonClickedSpreadLdTestpp2Part, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SPREAD_UD_TESTPP_2_PART, 3, CVATMainTab_ZAutoTeachingMeasure::ButtonClickedSpreadUdTestpp2Part, VTS_I4 VTS_I4 VTS_I2)

	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SSPANEL_LD_TRAYPP_PART, DISPID_CLICK, CVATMainTab_ZAutoTeachingMeasure::ClickSspanelLdTrayppPart, VTS_NONE)
	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SSPANEL_UD_TRAYPP_PART, DISPID_CLICK, CVATMainTab_ZAutoTeachingMeasure::ClickSspanelUdTrayppPart, VTS_NONE)
	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SSPANEL_LD_TESTPP_1_PART, DISPID_CLICK, CVATMainTab_ZAutoTeachingMeasure::ClickSspanelLdTestpp1Part, VTS_NONE)
	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SSPANEL_UD_TESTPP_1_PART, DISPID_CLICK, CVATMainTab_ZAutoTeachingMeasure::ClickSspanelUdTestpp1Part, VTS_NONE)
	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SSPANEL_LD_TESTPP_2_PART, DISPID_CLICK, CVATMainTab_ZAutoTeachingMeasure::ClickSspanelLdTestpp2Part, VTS_NONE)
	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SSPANEL_UD_TESTPP_2_PART, DISPID_CLICK, CVATMainTab_ZAutoTeachingMeasure::ClickSspanelUdTestpp2Part, VTS_NONE)

	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SSPANEL_LD_TRAYPP_FLATNESS_OFFSET, DISPID_CLICK, CVATMainTab_ZAutoTeachingMeasure::ClickSspanelLdTrayppFlatnessOffset, VTS_NONE)
	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SSPANEL_UD_TRAYPP_FLATNESS_OFFSET, DISPID_CLICK, CVATMainTab_ZAutoTeachingMeasure::ClickSspanelUdTrayppFlatnessOffset, VTS_NONE)
	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SSPANEL_LD_TESTPP_1_FLATNESS_OFFSET, DISPID_CLICK, CVATMainTab_ZAutoTeachingMeasure::ClickSspanelLdTestpp1FlatnessOffset, VTS_NONE)
	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SSPANEL_UD_TESTPP_1_FLATNESS_OFFSET, DISPID_CLICK, CVATMainTab_ZAutoTeachingMeasure::ClickSspanelUdTestpp1FlatnessOffset, VTS_NONE)
	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SSPANEL_LD_TESTPP_2_FLATNESS_OFFSET, DISPID_CLICK, CVATMainTab_ZAutoTeachingMeasure::ClickSspanelLdTestpp2FlatnessOffset, VTS_NONE)
	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SSPANEL_UD_TESTPP_2_FLATNESS_OFFSET, DISPID_CLICK, CVATMainTab_ZAutoTeachingMeasure::ClickSspanelUdTestpp2FlatnessOffset, VTS_NONE)

	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SSPANEL_LD_TRAYPP_PICK_POS, DISPID_CLICK, CVATMainTab_ZAutoTeachingMeasure::ClickSspanelLdTrayppPickPos, VTS_NONE)
	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SSPANEL_UD_TRAYPP_PICK_POS, DISPID_CLICK, CVATMainTab_ZAutoTeachingMeasure::ClickSspanelUdTrayppPickPos, VTS_NONE)
	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SSPANEL_LD_TESTPP_1_PICK_POS, DISPID_CLICK, CVATMainTab_ZAutoTeachingMeasure::ClickSspanelLdTestpp1PickPos, VTS_NONE)
	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SSPANEL_UD_TESTPP_1_PICK_POS, DISPID_CLICK, CVATMainTab_ZAutoTeachingMeasure::ClickSspanelUdTestpp1PickPos, VTS_NONE)
	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SSPANEL_LD_TESTPP_2_PICK_POS, DISPID_CLICK, CVATMainTab_ZAutoTeachingMeasure::ClickSspanelLdTestpp2PickPos, VTS_NONE)
	ON_EVENT(CVATMainTab_ZAutoTeachingMeasure, IDC_SSPANEL_UD_TESTPP_2_PICK_POS, DISPID_CLICK, CVATMainTab_ZAutoTeachingMeasure::ClickSspanelUdTestpp2PickPos, VTS_NONE)

		
	//ON_EVENT_RANGE(CVATMainTab_ZAutoTeachingMeasure, IDC_SPREAD_LD_TRAYPP_PART, IDC_SPREAD_UD_TESTPP_2_PART, DISPID_CLICK CVATMainTab_ZAutoTeachingMeasure::ButtonClickedSpreadSelectPart, VTS_I4 VTS_I4 VTS_I2)

	
	
END_EVENTSINK_MAP()


void CVATMainTab_ZAutoTeachingMeasure::InitEditDesign()
{
	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++) {
		for (int nPkr = 0; nPkr < VAT_PICKER_MAX; nPkr++) {
			m_EditPickerData[nHand][nPkr].SetBackColor(COLOR_WWHITE);
			m_EditPickerData[nHand][nPkr].SetTextSize(11);		
		}
 	}
}


void CVATMainTab_ZAutoTeachingMeasure::InitCtrlButton()
{
	CFont OperBtnFont;
	OperBtnFont.CreatePointFont(90, "Fixedsys");

	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++) {
		m_BtnReload[nHand].SetFont(&OperBtnFont);
		m_BtnReload[nHand].SetWindowText(_T("RELOAD"));
		m_BtnReload[nHand].SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
		
		m_BtnSave[nHand].SetFont(&OperBtnFont);
		m_BtnSave[nHand].SetWindowText(_T("SAVE"));
		m_BtnSave[nHand].SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 0, RGB(55, 55, 255));
	}
}


void CVATMainTab_ZAutoTeachingMeasure::OnEnSetFocusHandData(UINT nID)
{
	//해당 속성 제거로 EN_SETFOCUS 초기 호출 방지.(해당 Dlg Create될 때)
// 	if (m_bIsInit) {
// 		m_bIsInit = FALSE;
// 		return;
// 	}
// 
	if (m_bBlockEditFocus)
		return;

	UpdateData(TRUE);

	int nHandIdx = -1;
	int nPkrIdx = -1;
	ConvCtrlIdToHandAndPkrIdx(nID, nHandIdx, nPkrIdx);

		//Idx Check
	if (nHandIdx == -1 || nPkrIdx == -1)
		return;

  	char szInputValue[10] = { 0, };
  	char szCurrentValue[10] = { 0, };
  	
	
  	sprintf(szCurrentValue, "%.2f", m_dCtrlPickerData[nHandIdx][nPkrIdx]);
  
    int nRet = g_LibCommCtrl.GetNumberBox(this, szInputValue, 10, szCurrentValue, "Flateness Offset", "5.0", "-5.0");
    if (nRet == RETURN_OK) {
    	GetDlgItem(m4_nDataStaticIdx[nHandIdx][nPkrIdx])->SetWindowText(szInputValue);
    	
		//Data Save에서 진행.
		//UpdateData(TRUE); //Control -> 변수
    }
}

void CVATMainTab_ZAutoTeachingMeasure::ConvCtrlIdToHandAndPkrIdx(UINT nID, int &nHandIdx, int &nPkrIdx)
{
	if (IDC_LD_TRAYPP_DATA_1 <= nID && nID <= IDC_LD_TRAYPP_DATA_8) {
		nHandIdx = VAT_HAND_LOAD_TRAYPP;
		nPkrIdx = nID - IDC_LD_TRAYPP_DATA_1;
	}
	else if (IDC_UD_TRAYPP_DATA_1 <= nID && nID <= IDC_UD_TRAYPP_DATA_8) {
		nHandIdx = VAT_HAND_UNLOAD_TRAYPP;
		nPkrIdx = nID - IDC_UD_TRAYPP_DATA_1;
	}
	else if (IDC_LD_TESTPP_1_DATA_1 <= nID && nID <= IDC_LD_TESTPP_1_DATA_8) {
		nHandIdx = VAT_HAND_LOAD_TESTPP1;
		nPkrIdx = nID - IDC_LD_TESTPP_1_DATA_1;
	}
	else if (IDC_UD_TESTPP_1_DATA_1 <= nID && nID <= IDC_UD_TESTPP_1_DATA_8) {
		nHandIdx = VAT_HAND_UNLOAD_TESTPP1;
		nPkrIdx = nID - IDC_UD_TESTPP_1_DATA_1;
	}
	else if (IDC_LD_TESTPP_2_DATA_1 <= nID && nID <= IDC_LD_TESTPP_2_DATA_8) {
		nHandIdx = VAT_HAND_LOAD_TESTPP2;
		nPkrIdx = nID - IDC_LD_TESTPP_2_DATA_1;
	}
	else if (IDC_UD_TESTPP_2_DATA_1 <= nID && nID <= IDC_UD_TESTPP_2_DATA_8) {
		nHandIdx = VAT_HAND_UNLOAD_TESTPP2;
		nPkrIdx = nID - IDC_UD_TESTPP_2_DATA_1;
	}
}



void CVATMainTab_ZAutoTeachingMeasure::OnBtnClickedFlatnessOffsetReload(UINT nID)
{
	//Hand Index 변환.
// 	ASSERT(IDC_BTN_Z_AUTO_TEACHING_RELOAD_1 <= nID && nID <= IDC_BTN_Z_AUTO_TEACHING_RELOAD_6);
// 			
// 	int nHandIdx = nID - IDC_BTN_Z_AUTO_TEACHING_RELOAD_1;

	int nHandIdx = _Com_ConvertResourceIdToHandIdx(nID, IDC_BTN_Z_AUTO_TEACHING_RELOAD_1, IDC_BTN_Z_AUTO_TEACHING_RELOAD_6);
	if (nHandIdx == -1) {
		return;
	}


	DisplayPickerData(nHandIdx, m_nLastDataType[nHandIdx]);
}


//========================================================================================
//COMMENT: 작업자 Offset 변경 Data 저장.
//========================================================================================
void CVATMainTab_ZAutoTeachingMeasure::OnBtnClickedFlatnessOffsetSave(UINT nID)
{
// 	ASSERT(IDC_BTN_Z_AUTO_TEACHING_SAVE_1 <= nID && nID <= IDC_BTN_Z_AUTO_TEACHING_SAVE_6);
// 	int nHandIdx = nID - IDC_BTN_Z_AUTO_TEACHING_SAVE_1;

	UpdateData(TRUE); //Control -> 변수

	int nHandIdx = _Com_ConvertResourceIdToHandIdx(nID, IDC_BTN_Z_AUTO_TEACHING_SAVE_1, IDC_BTN_Z_AUTO_TEACHING_SAVE_6);
	if (nHandIdx == -1) {
		return;
	}

	int nPartIdx = _Com_GetPartIdxfromComboBox(nHandIdx);

	double dFlatnessOffset = 0.0;

	for (int nPkrIdx = 0; nPkrIdx < VAT_PICKER_MAX; nPkrIdx++) {
		dFlatnessOffset = m_dCtrlPickerData[nHandIdx][nPkrIdx] * 1000.0;

		//Data Save
		g_VATCommonData.m_stVATZTeachInfo.stPickerGap.setPickerGap(nHandIdx, nPartIdx, nPkrIdx, dFlatnessOffset);
	}

	//File Save
	g_VATCommonData.SaveEachZAutoTeachingInformation(eZAutoTeaching_PickerFlatnessOffset, nHandIdx);

	DisplayPickerData(nHandIdx, m_nLastDataType[nHandIdx]);
}


afx_msg LRESULT CVATMainTab_ZAutoTeachingMeasure::OnReceiveMeasureComplete(WPARAM wParam, LPARAM lParam)
{
	//Part Status에 따른 Selected Part 정리.
	//Status Init Part만 복사하면 됨.

	std::vector<int> vPart;
	vPart.clear();

	for (auto _auto_task : g_VatTaskSystemCtrl.m_vAllTaskAuto) 
	{
		int nHand = _auto_task->m_nHand;

		//Data Save 완료 된 Part만 _Com_SetSelectedPart 해주면 선택 해제 됨
		int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
		for (int nPart = 0; nPart < nMaxPart; nPart++) {
			if (_Com_IsPartSelected(nHand, nPart)) {
				int nStatus = _auto_task->_Base_GetMeasurePart_Status(nPart);
				if (nStatus == VAT_STATUS_DATA_SAVE_COMPLETE)
				{
					//측정 완료 된 Part Select 해제만 진행.
					//색상은 완료로 유지.
					_Com_SetSelectedPart(nHand, nPart, FALSE);
				}
				else 
				{
					//재측정 필요한 Part 색상 초기화.(측정 Abort 및 Data Save 안함)
					int nCol = 1;
					int nRow = nPart + 1;
					_Com_SSDisplaySelectedPart(nHand, nCol, nRow);
				}

				//선택된 모든 Part 상태 초기화 진행.
				_auto_task->_SetMeasurePart_Status_DM(nPart, VAT_STATUS_MEASURE_INIT);
			}
		}

		//Auto에서 관리하는 Selected Part 초기화.
		_auto_task->_Base_ClearMeasurePart();
	}

	return 0L;
}