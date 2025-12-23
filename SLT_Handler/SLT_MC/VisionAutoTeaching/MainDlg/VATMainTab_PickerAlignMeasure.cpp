// VATMainTab_PickerAlignMeasure.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "VATMainTab_PickerAlignMeasure.h"
#include "afxdialogex.h"

// CVATMainTab_PickerAlignMeasure 대화 상자입니다.

IMPLEMENT_DYNAMIC(CVATMainTab_PickerAlignMeasure, CDialogEx)

CVATMainTab_PickerAlignMeasure::CVATMainTab_PickerAlignMeasure(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VAT_MAIN_PICKER_ALIGN_MEASURE_DLG, pParent)
{
	m_pDlgPkrInfo = NULL;
}

CVATMainTab_PickerAlignMeasure::~CVATMainTab_PickerAlignMeasure()
{
	if (m_pDlgPkrInfo != NULL)
	{
		m_pDlgPkrInfo->DestroyWindow();
		delete m_pDlgPkrInfo;
		m_pDlgPkrInfo = NULL;
	}
}

void CVATMainTab_PickerAlignMeasure::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SPREAD_PICKER_ALIGN_LOAD_TRAYPP, m_ssSelectSheet[VAT_HAND_LOAD_TRAYPP]);
	DDX_Control(pDX, IDC_SPREAD_PICKER_ALIGN_UNLOAD_TRAYPP, m_ssSelectSheet[VAT_HAND_UNLOAD_TRAYPP]);
	DDX_Control(pDX, IDC_SPREAD_PICKER_ALIGN_LOAD_TESTPP_1, m_ssSelectSheet[VAT_HAND_LOAD_TESTPP1]);
	DDX_Control(pDX, IDC_SPREAD_PICKER_ALIGN_UNLOAD_TESTPP_1, m_ssSelectSheet[VAT_HAND_UNLOAD_TESTPP1]);
	DDX_Control(pDX, IDC_SPREAD_PICKER_ALIGN_LOAD_TESTPP_2, m_ssSelectSheet[VAT_HAND_LOAD_TESTPP2]);
	DDX_Control(pDX, IDC_SPREAD_PICKER_ALIGN_UNLOAD_TESTPP_2, m_ssSelectSheet[VAT_HAND_UNLOAD_TESTPP2]);

	DDX_Control(pDX, IDC_SSPANEL_PICKER_ALIGN_HAND_SELECT, m_ssPanel_HandSelect);
	DDX_Control(pDX, IDC_SSPANEL_PICKER_ALIGN_MEASURE_STATUS, m_ssPanel_MeasureStatus);
	//DDX_Control(pDX, IDC_SSPANEL_PICKER_ALIGN_VIEW, m_ssPanel_PickerAlignView);

	DDX_Control(pDX, IDC_MEASURE_SPREAD_LOAD_TRAY_PP_PKR_STATUS, m_ssPkr_LoadTrayPP);
	DDX_Control(pDX, IDC_MEASURE_SPREAD_UNLOAD_TRAY_PP_PKR_STATUS, m_ssPkr_UnloadTrayPP);
	DDX_Control(pDX, IDC_MEASURE_SPREAD_LOAD_TEST_PP_1_PKR_STATUS, m_ssPkr_LoadTestPP1);
	DDX_Control(pDX, IDC_MEASURE_SPREAD_UNLOAD_TEST_PP_1_PKR_STATUS, m_ssPkr_UnloadTestPP1);
	DDX_Control(pDX, IDC_MEASURE_SPREAD_LOAD_TEST_PP_2_PKR_STATUS, m_ssPkr_LoadTestPP2);
	DDX_Control(pDX, IDC_MEASURE_SPREAD_UNLOAD_TEST_PP_2_PKR_STATUS, m_ssPkr_UnloadTestPP2);

	DDX_Control(pDX, IDC_BTN_PICKER_ALIGN_INFO, m_BtnPickerInfo);

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		DDX_Control(pDX, m3_nProgress[nHand], m_Progress[nHand]);
		DDX_Control(pDX, m3_nPercent[nHand], m_Percent[nHand]);
	}

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		for (int nType = 0; nType < VAT_PITCH_TYPE_MAX; nType++) {
			DDX_Control(pDX, m3_nPitchStatus[nHand][nType], m_editPitchStatus[nHand][nType]);
		}
	}

	//Option
	DDX_Control(pDX, IDC_CHK_PICKER_ALIGN_AUTO_FOCUS, m_CkBtnPickerAutoFocus);
}


BEGIN_MESSAGE_MAP(CVATMainTab_PickerAlignMeasure, CDialogEx)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_PICKER_ALIGN_INFO, &CVATMainTab_PickerAlignMeasure::OnBnClickedBtnPickerAlignInfo)
	ON_MESSAGE(UM_VAT_SEQ_MSG_UPDATE_PICKER_STATUS, &CVATMainTab_PickerAlignMeasure::OnReceiveUpdatePickerStatus)
	ON_MESSAGE(UM_VAT_SEQ_MSG_UPDATE_PROGRESS, &CVATMainTab_PickerAlignMeasure::OnReceiveUpdateProgress)
	ON_MESSAGE(UM_VAT_SEQ_MSG_UPDATE_VISION_STATUS, &CVATMainTab_PickerAlignMeasure::OnReceiveUpdateVisionStatus)
	ON_MESSAGE(UM_VAT_SEQ_MSG_UPDATE_VISION_OFFSET, &CVATMainTab_PickerAlignMeasure::OnReceiveUpdateVisionOffset)
	ON_MESSAGE(UM_VAT_SEQ_MSG_UPDATE_VISION_RETRY_CNT, &CVATMainTab_PickerAlignMeasure::OnReceiveUpdateVisionMeasureCnt)
	ON_MESSAGE(UM_VAT_SEQ_MSG_UPDATE_SAVE_DATA, &CVATMainTab_PickerAlignMeasure::OnReceiveUpdateSavedData)
	ON_BN_CLICKED(IDC_CHK_PICKER_ALIGN_AUTO_FOCUS, &CVATMainTab_PickerAlignMeasure::OnBnClickedChkPickerAlignAutoFocus)
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CVATMainTab_PickerAlignMeasure, CDialogEx)
	ON_EVENT(CVATMainTab_PickerAlignMeasure, IDC_SPREAD_PICKER_ALIGN_LOAD_TRAYPP, 3, CVATMainTab_PickerAlignMeasure::ButtonClickedSpreadPickerAlignLoadTraypp, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMainTab_PickerAlignMeasure, IDC_SPREAD_PICKER_ALIGN_UNLOAD_TRAYPP, 3, CVATMainTab_PickerAlignMeasure::ButtonClickedSpreadPickerAlignUnloadTraypp, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMainTab_PickerAlignMeasure, IDC_SPREAD_PICKER_ALIGN_LOAD_TESTPP_1, 3, CVATMainTab_PickerAlignMeasure::ButtonClickedSpreadPickerAlignLoadTestpp1, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMainTab_PickerAlignMeasure, IDC_SPREAD_PICKER_ALIGN_UNLOAD_TESTPP_1, 3, CVATMainTab_PickerAlignMeasure::ButtonClickedSpreadPickerAlignUnloadTestpp1, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMainTab_PickerAlignMeasure, IDC_SPREAD_PICKER_ALIGN_LOAD_TESTPP_2, 3, CVATMainTab_PickerAlignMeasure::ButtonClickedSpreadPickerAlignLoadTestpp2, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMainTab_PickerAlignMeasure, IDC_SPREAD_PICKER_ALIGN_UNLOAD_TESTPP_2, 3, CVATMainTab_PickerAlignMeasure::ButtonClickedSpreadPickerAlignUnloadTestpp2, VTS_I4 VTS_I4 VTS_I2)
END_EVENTSINK_MAP()


BOOL CVATMainTab_PickerAlignMeasure::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//Title SSPanel 초기화.
	CDSSPanel* panel[2] = {
		&m_ssPanel_HandSelect, &m_ssPanel_MeasureStatus,/* &m_ssPanel_PickerAlignView,*/
	};
	for (auto p : panel) {
		_Com_InitSSTitlePanel(*p);
	}

	//Arm 선택 Spread Sheet 초기화.
	_Com_InitHandSelectSheet();

	//Picker Align View 버튼 초기화.
	InitPickerAlignView();

	//Picker Status 초기화
	_PkrAlignCom_InitSSPickerStatus();
	BlockPkrSpreadSheet();

	//Measure OverView 초기화
	InitPickerAlignMeasureOverView();
	
	//Option 초기화.
	InitPickerAlignOption();

	return TRUE;
}


void CVATMainTab_PickerAlignMeasure::PageMoveInit_Tab()
{
	//Offset Grid&Data 다시 그리기.
	Invalidate(TRUE);

	//Timer
	SetTimer(_ePickerAlignT_PickerStatus, 1000, NULL);
	SetTimer(_ePickerAlignT_CheckExecuteStatus, 200, NULL);

	//Data Load 할꺼 없음.


}


void CVATMainTab_PickerAlignMeasure::PageMoveDispose_Tab()
{
	for (int i = 0; i < _ePickerAlignT_Max; i++) {
		KillTimer(i);
	}
}



void CVATMainTab_PickerAlignMeasure::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case _ePickerAlignT_PickerStatus:
	{
		_PkrAlignCom_tDisplayPickerStatus();
	}break;
	case _ePickerAlignT_CheckExecuteStatus:
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


void CVATMainTab_PickerAlignMeasure::OnDestroy()
{
	for (int i = 0; i < _ePickerAlignT_Max; i++) {
		KillTimer(i);
	}

	CDialogEx::OnDestroy();
}


void CVATMainTab_PickerAlignMeasure::OnPaint()
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

void CVATMainTab_PickerAlignMeasure::InitPickerAlignView()
{
	//Picker Information 버튼 초기화
	CFont BtnFont;
	BtnFont.CreatePointFont(120, "Fixedsys");

	m_BtnPickerInfo.SetFont(&BtnFont);
	//m_BtnPickerInfo.SetWindowText(_T("Information"));
	m_BtnPickerInfo.SetShade(CShadeButtonST::SHS_SOFTBUMP, 8, 20, 3, RGB(55, 55, 255));
	m_BtnPickerInfo.SetIcon(IDI_BTN_PICKER_INFO);
	m_BtnPickerInfo.SetAlign(CButtonST::ST_ALIGN_VERT);

	//Picker Align Info Dlg Create
	m_pDlgPkrInfo = new CVATSub_PickerAlignInfo;
	m_pDlgPkrInfo->Create(IDD_VAT_SUB_PICKER_ALIGN_INFO, this);
	m_pDlgPkrInfo->ShowWindow(SW_HIDE);

}


void CVATMainTab_PickerAlignMeasure::InitPickerAlignMeasureOverView()
{
	//Progress Bar
	_Com_InitProgressBar();

	//Vision Status && Vision Retry Cnt
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{
		int nFrame = 1; //VAT_RCV_STATUS_INIT 1
		CDSSPanel* chkbt = (CDSSPanel*)GetDlgItem(m3_nCamStatus[nHand]);
		chkbt->SetStatus(nFrame);
		chkbt->SetForeColor(BBLACK);
		chkbt->SetWindowText(_T("0"));
	}

	//Offset Rect Graphic&Data View
	InitOverViewRect();

	// Wide/Narrow EditBox 초기화.
	InitPitchTypeStatus();
}


void CVATMainTab_PickerAlignMeasure::InitOverViewRect()
{
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{
		CStatic *staticArea = (CStatic *)GetDlgItem(m3_nRectGraphicView_MeasureTab[nHand]);
		staticArea->GetWindowRect(&m_rect[nHand]);
		ScreenToClient(m_rect[nHand]);
	}

	//Offset Data View
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
	{
		for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XY; nAxis++)
		{
			CStatic *staticArea = (CStatic *)GetDlgItem(m3_nRectDataView_MeasureTab[nHand][nAxis]);
			staticArea->GetWindowRect(&m_rectData[nHand][nAxis]);
			ScreenToClient(m_rectData[nHand][nAxis]);
		}
	}
}


void CVATMainTab_PickerAlignMeasure::InitPitchTypeStatus()
{
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		for (int nType = 0; nType < VAT_PITCH_TYPE_MAX; nType++) {	
			m_editPitchStatus[nHand][nType].SetWindowText((nType == VAT_PITCH_TYPE_WIDE) ? _T("WIDE") : _T("NARROW"));
		}
	}
}


void CVATMainTab_PickerAlignMeasure::InitPickerAlignOption()
{
	CFont StaticFont;
	StaticFont.CreatePointFont(120, "Fixedsys");

	g_DMCont.m_dmVAT.SN(NMD14_VAT_PICKER_ALIGN_AUTO_FOCUS, g_VATCommonData.m_stVATOption.nPickerAutoFocus);
	
	BOOL bPkrAutoFocus = g_VATCommonData.m_stVATOption.nPickerAutoFocus;
	m_CkBtnPickerAutoFocus.SetCheck(bPkrAutoFocus);
	m_CkBtnPickerAutoFocus.SetFont(&StaticFont);

}


/*==================================================================================*/
//COMMENT : Measure Tab은 Mode 측정 시, 상태 표기만 하기 때문에 작업자 동작 Block처리.  
/*==================================================================================*/
void CVATMainTab_PickerAlignMeasure::BlockPkrSpreadSheet()
{
	m_ssPkr_LoadTrayPP.EnableWindow(FALSE);
	m_ssPkr_UnloadTrayPP.EnableWindow(FALSE);
	m_ssPkr_LoadTestPP1.EnableWindow(FALSE);
	m_ssPkr_UnloadTestPP1.EnableWindow(FALSE);
	m_ssPkr_LoadTestPP2.EnableWindow(FALSE);
	m_ssPkr_UnloadTestPP2.EnableWindow(FALSE);
}

void CVATMainTab_PickerAlignMeasure::DisplaySelectedPicker(int nHand, BOOL bIsHandSelected)
{
	if (nHand < VAT_HAND_LOAD_TRAYPP && nHand > VAT_HAND_UNLOAD_TESTPP2)
		return;

	//if (/*g_VATCommonData.m_stVATPickerAlignInfoBuff.nSelectedPicker[nHand][nY][nX] == 1*/1)
	CSpreadSheet *ssPickerStatus[VAT_HAND_MAX] = { &m_ssPkr_LoadTrayPP, &m_ssPkr_UnloadTrayPP, &m_ssPkr_LoadTestPP1, &m_ssPkr_UnloadTestPP1, &m_ssPkr_LoadTestPP2, &m_ssPkr_UnloadTestPP2 };

	int nMaxPkrX = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? GUI_MAX_TRAY_PP_PICKER_CNT_X : GUI_MAX_TEST_PP_PICKER_CNT_X;
	int nMaxPkrY = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? GUI_MAX_TRAY_PP_PICKER_CNT_Y : GUI_MAX_TEST_PP_PICKER_CNT_Y;

	for (int nGuiY = 0; nGuiY < nMaxPkrY; nGuiY++)
	{
		for (int nGuiX = 0; nGuiX < nMaxPkrX; nGuiX++)
		{
			int nCol = nGuiX + 1;
			int nRow = nGuiY + 1;

			ssPickerStatus[nHand]->SetCol(nCol);
			ssPickerStatus[nHand]->SetRow(nRow);

			if (bIsHandSelected == TRUE) {
				ssPickerStatus[nHand]->SetTypeButtonColor(LLIGHTBLUE);
			}
			else {
				ssPickerStatus[nHand]->SetTypeButtonColor(WWHITE);
			}
		}
	}
}


void CVATMainTab_PickerAlignMeasure::EnableControls(BOOL bStatus)
{
	//Select Hand
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		m_ssSelectSheet[nHand].EnableWindow(bStatus);
	}

	m_CkBtnPickerAutoFocus.EnableWindow(bStatus);
}


void CVATMainTab_PickerAlignMeasure::ButtonClickedSpreadPickerAlignLoadTraypp(long Col, long Row, short ButtonDown)
{
	_Com_ButtonClickedSpreadHand(Col, Row, VAT_HAND_LOAD_TRAYPP, eVAT_SSTYPE_PICKER/*, m_editPosition*/);

	DisplaySelectedPicker(VAT_HAND_LOAD_TRAYPP, _Com_IsHandSelected(VAT_HAND_LOAD_TRAYPP));
}


void CVATMainTab_PickerAlignMeasure::ButtonClickedSpreadPickerAlignUnloadTraypp(long Col, long Row, short ButtonDown)
{
	_Com_ButtonClickedSpreadHand(Col, Row, VAT_HAND_UNLOAD_TRAYPP, eVAT_SSTYPE_PICKER/*, m_editPosition*/);

	DisplaySelectedPicker(VAT_HAND_UNLOAD_TRAYPP, _Com_IsHandSelected(VAT_HAND_UNLOAD_TRAYPP));
}


void CVATMainTab_PickerAlignMeasure::ButtonClickedSpreadPickerAlignLoadTestpp1(long Col, long Row, short ButtonDown)
{
	_Com_ButtonClickedSpreadHand(Col, Row, VAT_HAND_LOAD_TESTPP1, eVAT_SSTYPE_PICKER/*, m_editPosition*/);

	DisplaySelectedPicker(VAT_HAND_LOAD_TESTPP1, _Com_IsHandSelected(VAT_HAND_LOAD_TESTPP1));
}


void CVATMainTab_PickerAlignMeasure::ButtonClickedSpreadPickerAlignUnloadTestpp1(long Col, long Row, short ButtonDown)
{
	_Com_ButtonClickedSpreadHand(Col, Row, VAT_HAND_UNLOAD_TESTPP1, eVAT_SSTYPE_PICKER/*, m_editPosition*/);

	DisplaySelectedPicker(VAT_HAND_UNLOAD_TESTPP1, _Com_IsHandSelected(VAT_HAND_UNLOAD_TESTPP1));
}


void CVATMainTab_PickerAlignMeasure::ButtonClickedSpreadPickerAlignLoadTestpp2(long Col, long Row, short ButtonDown)
{
	_Com_ButtonClickedSpreadHand(Col, Row, VAT_HAND_LOAD_TESTPP2, eVAT_SSTYPE_PICKER/*, m_editPosition*/);

	DisplaySelectedPicker(VAT_HAND_LOAD_TESTPP2, _Com_IsHandSelected(VAT_HAND_LOAD_TESTPP2));
}


void CVATMainTab_PickerAlignMeasure::ButtonClickedSpreadPickerAlignUnloadTestpp2(long Col, long Row, short ButtonDown)
{
	_Com_ButtonClickedSpreadHand(Col, Row, VAT_HAND_UNLOAD_TESTPP2, eVAT_SSTYPE_PICKER/*, m_editPosition*/);

	DisplaySelectedPicker(VAT_HAND_UNLOAD_TESTPP2, _Com_IsHandSelected(VAT_HAND_UNLOAD_TESTPP2));
}


void CVATMainTab_PickerAlignMeasure::OnBnClickedBtnPickerAlignInfo()
{
	if(m_pDlgPkrInfo->IsWindowVisible() == FALSE)
		m_pDlgPkrInfo->ShowWindow(SW_SHOW);
}



void CVATMainTab_PickerAlignMeasure::DisplayPitchType(int nCurPitchType)
{
	for (int nHand = VAT_HAND_LOAD_TRAYPP; nHand < VAT_HAND_MAX; nHand++) {
		for (int nPitchType = VAT_PITCH_TYPE_WIDE; nPitchType < VAT_PITCH_TYPE_MAX; nPitchType++) {
			BOOL bPitchStatus_Ready = g_VatTaskSystemCtrl.m_vAllTaskAuto[nHand]->_Base_IsAllPickerSameStatus(nPitchType, VAT_STATUS_MEASURE_READY);
			BOOL bPitchStatus_Complete = g_VatTaskSystemCtrl.m_vAllTaskAuto[nHand]->_Base_IsAllPickerSameStatus(nPitchType, VAT_STATUS_MEASURE_COMPLETE);
			BOOL bPitchStatus_DataSave = g_VatTaskSystemCtrl.m_vAllTaskAuto[nHand]->_Base_IsAllPickerSameStatus(nPitchType, VAT_STATUS_DATA_SAVE_COMPLETE);

			ULONG ulPoscolor;
			if (bPitchStatus_Ready || bPitchStatus_Complete || bPitchStatus_DataSave) {
				ulPoscolor = WWHITE;
			}
			else {
				ulPoscolor = PICKER_ALIGN_PITCH_TYPE;
			}

			if (m_editPitchStatus[nHand][nPitchType].GetBackColor() != ulPoscolor) {
				m_editPitchStatus[nHand][nPitchType].SetBackColor(ulPoscolor);
				m_editPitchStatus[nHand][nPitchType].Invalidate(FALSE); //배경만 repaint
			}
		}
	}
}

//============================================================================================================
//COMMENT : Picker Status Update 함수. 
//           ※ PostMessage로 전달해야 함.
//Param   : wParam -> Hand Index
//          lParam 사용 하지 않음.
//           ※ Message Q에 쌓여 있는동안 상태 변경이 있을 수 있기 때문에 SingleTone Instance 사용.(최신 상태 표기)
//			 ※ 멤버 변수에만 저장 후, 진행 상태 Timer에서 표기.
//              -. 진행 상태 OnGoing의 경우, 깜빡임 표기 하기 위함.
//============================================================================================================
LRESULT CVATMainTab_PickerAlignMeasure::OnReceiveUpdatePickerStatus(WPARAM wParam, LPARAM lParam)
{
	int nHand		= GET_BIT_VAT_HAND(wParam);
	int nPitchType	= GET_BIT_VAT_PITCH(wParam);
	int nPickerIdx	= GET_BIT_VAT_PICKER(wParam);

	if (!((VAT_HAND_LOAD_TRAYPP <= nHand && nHand <= VAT_HAND_UNLOAD_TESTPP2) &&
		(nPitchType == VAT_PITCH_TYPE_WIDE || nPitchType == VAT_PITCH_TYPE_NARROW) &&
		(VAT_PICKER_01 <= nPickerIdx && nPickerIdx <= VAT_PICKER_08))) 
		return 0L;
 
	//Picker Status 저장.(OnTimer에서 Blink로 측정 진행 중 Picker 표기)
	m_nGuiPickerStatus[nHand][nPitchType][nPickerIdx] = GuiNotifier->GetPickerStatus(nHand, nPitchType, nPickerIdx);

	DisplayPitchType(nPitchType);

	return 0L;
}


//============================================================================================================
//COMMENT : 모든 Picker 측정에 대한 진행률 상태 Update 함수.(Progress) 
//           ※ PostMessage로 전달해야 함.
//Param   : wParam -> Hand Index
//          lParam 사용 하지 않음.
//           ※ Message Q에 쌓여 있는동안 상태 변경이 있을 수 있기 때문에 SingleTone Instance 사용.(최신 상태 표기)
//============================================================================================================
LRESULT CVATMainTab_PickerAlignMeasure::OnReceiveUpdateProgress(WPARAM wParam, LPARAM lParam)
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
LRESULT CVATMainTab_PickerAlignMeasure::OnReceiveUpdateVisionStatus(WPARAM wParam, LPARAM lParam)
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

	CDSSPanel* chkbt = (CDSSPanel*)GetDlgItem(m3_nCamStatus[nHand]);
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
LRESULT CVATMainTab_PickerAlignMeasure::OnReceiveUpdateVisionOffset(WPARAM wParam, LPARAM lParam)
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
	InvalidateRect(&m_rectData[nHand][VAT_X], TRUE);	//Offset Data X 새로 고침.
	InvalidateRect(&m_rectData[nHand][VAT_Y], TRUE);	//Offset Data Y 새로 고침.

	return 0L;
}



//============================================================================================================
//COMMENT : Vision Cam 측정 Count 표기
//           1. 실제 Vision Measure Cnt
//           2. Cam Auto Focus의 경우, -1로 Data를 받으며 'A'로 표기 됨.
//Param   : wParam -> Cam Index
//          lParam 사용 하지 않음.
//           ※ Message Q에 쌓여 있는동안 상태 변경이 있을 수 있기 때문에 SingleTone Instance 사용.(최신 상태 표기)
//=============================================================================================================
LRESULT CVATMainTab_PickerAlignMeasure::OnReceiveUpdateVisionMeasureCnt(WPARAM wParam, LPARAM lParam)
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

	CDSSPanel* chkbt = (CDSSPanel*)GetDlgItem(m3_nCamStatus[nHand]);

	CString strVisionRetryCnt = _T("");
	if (nVisionMeasureCnt == VAT_VISION_MEASURE_AUTO_FOCUS) {
		strVisionRetryCnt = _T("F"); //특수 case
	}
	else {
		strVisionRetryCnt.Format("%d", nVisionMeasureCnt); //일반 표기
	}

	chkbt->SetWindowText(strVisionRetryCnt);

	return 0L;
}


//============================================================================================================
//COMMENT : 측정 완료 된, Data Update 진행.
//Param   : wParam -> Cam Index
//          lParam 사용 하지 않음.
//           ※ Message Q에 쌓여 있는동안 상태 변경이 있을 수 있기 때문에 SingleTone Instance 사용.(최신 상태 표기)
//=============================================================================================================
LRESULT CVATMainTab_PickerAlignMeasure::OnReceiveUpdateSavedData(WPARAM wParam, LPARAM lParam)
{
	//Picker Align의 경우 Spread Sheet 표기 Data가 없음.
	//즉, GUI Data가 없음.

	//[Real -> GUI Update]
	//1. Plate Jig Target Position
// 	COPY_VAT_ARRAY(m_stVATCamCenCalInfoGui.dBotCameraPosition, g_VATCommonData.m_stVATCamCenterCalInfo.dBotCameraPosition);
// 	DisplaySSData(_eCamCenCalSS_BotCamCenterPos, m_nSS1_DisplayType);
// 
// 	//2. Plate Jig Vacuum Position
// 	COPY_VAT_ARRAY(m_stVATCamCenCalInfoGui.dTopCamToPkrDistance, g_VATCommonData.m_stVATCamCenterCalInfo.dTopCamToPkrDistance);
// 	DisplaySSData(_eCamCenCalSS_TopCamToPkrDis, m_nSS2_DisplayType);

	return 0L;
}

void CVATMainTab_PickerAlignMeasure::OnBnClickedChkPickerAlignAutoFocus()
{
	//Measure 측정 진행 중, On/Off되지 않도록 막기.
	int nPkrAutoFocusUse = (int)m_CkBtnPickerAutoFocus.GetCheck();
	g_DMCont.m_dmVAT.SN(NMD14_VAT_PICKER_ALIGN_AUTO_FOCUS, nPkrAutoFocusUse);

	g_VATCommonData.m_stVATOption.nPickerAutoFocus = nPkrAutoFocusUse;
	g_VATCommonData.SaveVATOption(eVatOption_PkrAutoFocus); //File 저장

	_Com_MakeLog_GUI("[Option Save] [Picker Auto Focus %s]", ((BOOL)nPkrAutoFocusUse) ? (_T("USE")) : (_T("NO USE")));
}
