// VATMainTab_VisionAutoTeachingMeasure.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "VATMainTab_VisionAutoTeachingMeasure.h"
#include "afxdialogex.h"


// CVATMainTab_VisionAutoTeachingMeasure 대화 상자입니다.

IMPLEMENT_DYNAMIC(CVATMainTab_VisionAutoTeachingMeasure, CDialogEx)

CVATMainTab_VisionAutoTeachingMeasure::CVATMainTab_VisionAutoTeachingMeasure(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VAT_MAIN_VISION_AUTO_TEACHING_MEASURE_DLG, pParent)
{

}

CVATMainTab_VisionAutoTeachingMeasure::~CVATMainTab_VisionAutoTeachingMeasure()
{
}

void CVATMainTab_VisionAutoTeachingMeasure::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

 	DDX_Control(pDX, IDC_SSPANEL_VAT_HAND_TITLE_1, m_ssPanel_LD_TrayPP);
 	DDX_Control(pDX, IDC_SSPANEL_VAT_HAND_TITLE_2, m_ssPanel_UD_TrayPP);
 	DDX_Control(pDX, IDC_SSPANEL_VAT_HAND_TITLE_3, m_ssPanel_LD_TestPP1);
 	DDX_Control(pDX, IDC_SSPANEL_VAT_HAND_TITLE_4, m_ssPanel_UD_TestPP1);
 	DDX_Control(pDX, IDC_SSPANEL_VAT_HAND_TITLE_5, m_ssPanel_LD_TestPP2);
 	DDX_Control(pDX, IDC_SSPANEL_VAT_HAND_TITLE_6, m_ssPanel_UD_TestPP2); 	
 
 	DDX_Control(pDX, IDC_SPREAD_VAT_1, m_ssSelectSheet[VAT_HAND_LOAD_TRAYPP]);
 	DDX_Control(pDX, IDC_SPREAD_VAT_2, m_ssSelectSheet[VAT_HAND_UNLOAD_TRAYPP]);
 	DDX_Control(pDX, IDC_SPREAD_VAT_3, m_ssSelectSheet[VAT_HAND_LOAD_TESTPP1]);
 	DDX_Control(pDX, IDC_SPREAD_VAT_4, m_ssSelectSheet[VAT_HAND_UNLOAD_TESTPP1]);
 	DDX_Control(pDX, IDC_SPREAD_VAT_5, m_ssSelectSheet[VAT_HAND_LOAD_TESTPP2]);
 	DDX_Control(pDX, IDC_SPREAD_VAT_6, m_ssSelectSheet[VAT_HAND_UNLOAD_TESTPP2]);

	//Option
	for (int i = 0; i < VAT_HAND_MAX; i++)
	{
		if (m4_nOptHandOnly[i] != _VAT_ID_SPARE)
			DDX_Control(pDX, m4_nOptHandOnly[i], m_ssPanel_OptHandOnly[i]);
		if (m4_nOptUnitOnly[i] != _VAT_ID_SPARE)
			DDX_Control(pDX, m4_nOptUnitOnly[i], m_ssPanel_OptUnitOnly[i]);
	}

	//Part Status
	for (int i = 0; i < VAT_MAX_TRAY_PP_LOC_COUNT; i++) {
		if(m4_nPartPos_LDTrayPp[i] != _VAT_ID_SPARE)
			DDX_Control(pDX, m4_nPartPos_LDTrayPp[i], m_editPos_LDTrayPp[i]);

		if (m4_nPartPos_UDTrayPp[i] != _VAT_ID_SPARE)
			DDX_Control(pDX, m4_nPartPos_UDTrayPp[i], m_editPos_UDTrayPp[i]);
	}

	for (int i = 0; i < VAT_MAX_TEST_PP_LOC_COUNT; i++) {
		if (m4_nPartPos_LDTestPp1[i] != _VAT_ID_SPARE)
			DDX_Control(pDX, m4_nPartPos_LDTestPp1[i], m_editPos_LDTestPp1[i]);
		if (m4_nPartPos_UDTestPp1[i] != _VAT_ID_SPARE)
			DDX_Control(pDX, m4_nPartPos_UDTestPp1[i], m_editPos_UDTestPp1[i]);
		if (m4_nPartPos_LDTestPp2[i] != _VAT_ID_SPARE)
			DDX_Control(pDX, m4_nPartPos_LDTestPp2[i], m_editPos_LDTestPp2[i]);
		if (m4_nPartPos_UDTestPp2[i] != _VAT_ID_SPARE)
			DDX_Control(pDX, m4_nPartPos_UDTestPp2[i], m_editPos_UDTestPp2[i]);
	}
}


BEGIN_MESSAGE_MAP(CVATMainTab_VisionAutoTeachingMeasure, CDialogEx)
 	ON_WM_TIMER()
 	ON_WM_DESTROY()
 	ON_WM_PAINT()
END_MESSAGE_MAP()


// CVATMainTab_VisionAutoTeachingMeasure 메시지 처리기입니다.


BOOL CVATMainTab_VisionAutoTeachingMeasure::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//Hand panel 초기화.
	_Com_InitSSHandPanel_PartType();

	//Part Select Sheet 초기화.
	_Com_InitPartSelectSheet();

	//Part Edit Position 초기화.
	InitPartEditPos();

	//Offset Rect Graphic&Data View
	InitOverViewRect();

	//Option Panel
	InitSSOptionPanel();

	return TRUE;  
}


void CVATMainTab_VisionAutoTeachingMeasure::PageMoveInit_Tab()
{

}


void CVATMainTab_VisionAutoTeachingMeasure::PageMoveDispose_Tab()
{

}


BEGIN_EVENTSINK_MAP(CVATMainTab_VisionAutoTeachingMeasure, CDialogEx)
ON_EVENT(CVATMainTab_VisionAutoTeachingMeasure, IDC_SPREAD_VAT_1, 3, CVATMainTab_VisionAutoTeachingMeasure::ButtonClickedSpread2LdTrayppPart, VTS_I4 VTS_I4 VTS_I2)
ON_EVENT(CVATMainTab_VisionAutoTeachingMeasure, IDC_SPREAD_VAT_2, 3, CVATMainTab_VisionAutoTeachingMeasure::ButtonClickedSpread2UdTrayppPart, VTS_I4 VTS_I4 VTS_I2)
ON_EVENT(CVATMainTab_VisionAutoTeachingMeasure, IDC_SPREAD_VAT_3, 3, CVATMainTab_VisionAutoTeachingMeasure::ButtonClickedSpread2LdTestpp1Part, VTS_I4 VTS_I4 VTS_I2)
ON_EVENT(CVATMainTab_VisionAutoTeachingMeasure, IDC_SPREAD_VAT_4, 3, CVATMainTab_VisionAutoTeachingMeasure::ButtonClickedSpread2UdTestpp1Part, VTS_I4 VTS_I4 VTS_I2)
ON_EVENT(CVATMainTab_VisionAutoTeachingMeasure, IDC_SPREAD_VAT_5, 3, CVATMainTab_VisionAutoTeachingMeasure::ButtonClickedSpread2LdTestpp2Part, VTS_I4 VTS_I4 VTS_I2)
ON_EVENT(CVATMainTab_VisionAutoTeachingMeasure, IDC_SPREAD_VAT_6, 3, CVATMainTab_VisionAutoTeachingMeasure::ButtonClickedSpread2UdTestpp2Part, VTS_I4 VTS_I4 VTS_I2)

ON_EVENT(CVATMainTab_VisionAutoTeachingMeasure, IDC_SSPANEL_VAT_HAND_TITLE_1, DISPID_CLICK, CVATMainTab_VisionAutoTeachingMeasure::ClickSspanel2LdTrayppPart, VTS_NONE)
ON_EVENT(CVATMainTab_VisionAutoTeachingMeasure, IDC_SSPANEL_VAT_HAND_TITLE_2, DISPID_CLICK, CVATMainTab_VisionAutoTeachingMeasure::ClickSspanel2UdTrayppPart, VTS_NONE)
ON_EVENT(CVATMainTab_VisionAutoTeachingMeasure, IDC_SSPANEL_VAT_HAND_TITLE_3, DISPID_CLICK, CVATMainTab_VisionAutoTeachingMeasure::ClickSspanel2LdTestpp1Part, VTS_NONE)
ON_EVENT(CVATMainTab_VisionAutoTeachingMeasure, IDC_SSPANEL_VAT_HAND_TITLE_4, DISPID_CLICK, CVATMainTab_VisionAutoTeachingMeasure::ClickSspanel2UdTestpp1Part, VTS_NONE)
ON_EVENT(CVATMainTab_VisionAutoTeachingMeasure, IDC_SSPANEL_VAT_HAND_TITLE_5, DISPID_CLICK, CVATMainTab_VisionAutoTeachingMeasure::ClickSspanel2LdTestpp2Part, VTS_NONE)
ON_EVENT(CVATMainTab_VisionAutoTeachingMeasure, IDC_SSPANEL_VAT_HAND_TITLE_6, DISPID_CLICK, CVATMainTab_VisionAutoTeachingMeasure::ClickSspanel2UdTestpp2Part, VTS_NONE)
ON_EVENT(CVATMainTab_VisionAutoTeachingMeasure, IDC_SSPANEL_VAT_HAND_ONLY_1, DISPID_CLICK, CVATMainTab_VisionAutoTeachingMeasure::ClickSspanelVatHandOnly1, VTS_NONE)
ON_EVENT(CVATMainTab_VisionAutoTeachingMeasure, IDC_SSPANEL_VAT_HAND_ONLY_2, DISPID_CLICK, CVATMainTab_VisionAutoTeachingMeasure::ClickSspanelVatHandOnly2, VTS_NONE)
ON_EVENT(CVATMainTab_VisionAutoTeachingMeasure, IDC_SSPANEL_VAT_HAND_ONLY_3, DISPID_CLICK, CVATMainTab_VisionAutoTeachingMeasure::ClickSspanelVatHandOnly3, VTS_NONE)
ON_EVENT(CVATMainTab_VisionAutoTeachingMeasure, IDC_SSPANEL_VAT_HAND_ONLY_4, DISPID_CLICK, CVATMainTab_VisionAutoTeachingMeasure::ClickSspanelVatHandOnly4, VTS_NONE)
ON_EVENT(CVATMainTab_VisionAutoTeachingMeasure, IDC_SSPANEL_VAT_UNIT_ONLY_1, DISPID_CLICK, CVATMainTab_VisionAutoTeachingMeasure::ClickSspanelVatUnitOnly1, VTS_NONE)
ON_EVENT(CVATMainTab_VisionAutoTeachingMeasure, IDC_SSPANEL_VAT_UNIT_ONLY_2, DISPID_CLICK, CVATMainTab_VisionAutoTeachingMeasure::ClickSspanelVatUnitOnly2, VTS_NONE)
ON_EVENT(CVATMainTab_VisionAutoTeachingMeasure, IDC_SSPANEL_VAT_UNIT_ONLY_3, DISPID_CLICK, CVATMainTab_VisionAutoTeachingMeasure::ClickSspanelVatUnitOnly3, VTS_NONE)
ON_EVENT(CVATMainTab_VisionAutoTeachingMeasure, IDC_SSPANEL_VAT_UNIT_ONLY_4, DISPID_CLICK, CVATMainTab_VisionAutoTeachingMeasure::ClickSspanelVatUnitOnly4, VTS_NONE)
END_EVENTSINK_MAP()


void CVATMainTab_VisionAutoTeachingMeasure::OnTimer(UINT_PTR nIDEvent)
{
// 	switch (nIDEvent)
// 	{
// 	case _ePickerAlignT_PickerStatus:
// 	{
// 		_PkrAlignCom_tDisplayPickerStatus(); //Part Status 실시간 표기로 변경 필요함.
// 	}break;
// 	case _ePickerAlignT_CheckExecuteStatus:
// 	{
// 		//상태 지속적으로 변경 되는 것을 방지하고자 Last status check 진행.
// 		if (g_bVATStatusExecute != m_bLastExecuteStatus)
// 		{
// 			EnableControls(!g_bVATStatusExecute);
// 			m_bLastExecuteStatus = g_bVATStatusExecute;
// 		}
// 	}break;
// 	default:
// 		break;
// 	}

	CDialogEx::OnTimer(nIDEvent);
}

void CVATMainTab_VisionAutoTeachingMeasure::OnDestroy()
{
	// 	for (int i = 0; i < _ePickerAlignT_Max; i++) {
	// 		KillTimer(i);
	// 	}

	CDialogEx::OnDestroy();
}


void CVATMainTab_VisionAutoTeachingMeasure::OnPaint()
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


void CVATMainTab_VisionAutoTeachingMeasure::InitPartEditPos()
{
	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;

		for (int nPart = 0; nPart < nMaxPart; nPart++) {
			if (m_editPositionEachHand[nHand].editPosition[nPart].m_hWnd != NULL) {
				m_editPositionEachHand[nHand].editPosition[nPart].SetBackColor(WWHITE);
			}
		}
	}
}


void CVATMainTab_VisionAutoTeachingMeasure::InitOverViewRect()
{
  	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
  	{
  		CStatic *staticArea = (CStatic *)GetDlgItem(m4_nRectGraphicView[nHand]);
  		staticArea->GetWindowRect(&m_rect[nHand]);
  		ScreenToClient(m_rect[nHand]);
  	}
  
  	//Offset Data View
  	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
  	{
  		for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XY; nAxis++)
  		{
  			CStatic *staticArea = (CStatic *)GetDlgItem(m4_nRectDataView[nHand][nAxis]);
  			staticArea->GetWindowRect(&m_rectData[nHand][nAxis]);
  			ScreenToClient(m_rectData[nHand][nAxis]);
  		}
   	}
}


void CVATMainTab_VisionAutoTeachingMeasure::InitSSOptionPanel()
{
	for (int i = 0; i < VAT_HAND_MAX; i++)
	{
		if (m_ssPanel_OptHandOnly[i].GetSafeHwnd()) {
			m_ssPanel_OptHandOnly[i].SetWindowText(_T("Hand Only"));
		}
		g_DMCont.m_dmVAT.SN(NMD14_VAT_MEASURE_HAND_ONLY_1 + i, 0);

		if (m_ssPanel_OptUnitOnly[i].GetSafeHwnd()) {
			m_ssPanel_OptUnitOnly[i].SetWindowText(_T("Unit Only"));
		}
		g_DMCont.m_dmVAT.SN(NMD14_VAT_MEASURE_UNIT_ONLY_1 + i, 0);
	}
}


void CVATMainTab_VisionAutoTeachingMeasure::_SetOptionHandOnly(int nHand, BOOL bDisable /*= FALSE*/)
{
	//현재 Hand 4개에서만 Option 선택 가능.
	if (!(nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP || nHand == VAT_HAND_LOAD_TESTPP1 || nHand == VAT_HAND_LOAD_TESTPP2))
		return;

	BOOL bIsHandOnlyOptUse = g_DMCont.m_dmVAT.GN(NMD14_VAT_MEASURE_HAND_ONLY_1 + nHand);
	if (bDisable) {
		bIsHandOnlyOptUse = FALSE;
	}
	else {
		bIsHandOnlyOptUse = !bIsHandOnlyOptUse;
	}
	
	//DM 저장.
	g_DMCont.m_dmVAT.SN(NMD14_VAT_MEASURE_HAND_ONLY_1 + nHand, bIsHandOnlyOptUse);

	//Panel 색상 변경.
	if (bIsHandOnlyOptUse) {
		m_ssPanel_OptHandOnly[nHand].SetBackColor(LLIGHTGREEN);        //Hand

		//Hand 및 Unit 한 가지만 선택 가능하도록 진행.
		_SetOptionUnitOnly(nHand, TRUE); //No Use
	}
	else {
		m_ssPanel_OptHandOnly[nHand].SetBackColor(DISABLE_BACK_COLOR);
	}
}


void CVATMainTab_VisionAutoTeachingMeasure::_SetOptionUnitOnly(int nHand, BOOL bDisable /*= FALSE*/)
{
	//현재 Hand 4개에서만 Option 선택 가능.
	if (!(nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP || nHand == VAT_HAND_LOAD_TESTPP1 || nHand == VAT_HAND_LOAD_TESTPP2))
		return;

	BOOL bIsUnitOnlyOptUse = g_DMCont.m_dmVAT.GN(NMD14_VAT_MEASURE_UNIT_ONLY_1 + nHand);
	if (bDisable) {
		bIsUnitOnlyOptUse = FALSE;
	}
	else {
		bIsUnitOnlyOptUse = !bIsUnitOnlyOptUse;
	}
	

	//DM 저장.
	g_DMCont.m_dmVAT.SN(NMD14_VAT_MEASURE_UNIT_ONLY_1 + nHand, bIsUnitOnlyOptUse);

	//Panel 색상 변경.
	if (bIsUnitOnlyOptUse) {
		m_ssPanel_OptUnitOnly[nHand].SetBackColor(LLIGHTGREEN);
		
		//이전 Part Select Back Up 진행.
		m_stPartSelectedBackUp = m_stPartSelected;

// 		//Hand 및 Unit 한 가지만 선택 가능하도록 진행.
		_SetOptionHandOnly(nHand, TRUE); //No Use
	}
	else {
		m_ssPanel_OptUnitOnly[nHand].SetBackColor(DISABLE_BACK_COLOR);
	}

	//============================
	int nHAND_PART_MAX = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;

	for (int nPartIdx = 0; nPartIdx < nHAND_PART_MAX; nPartIdx++)
	{
		if(_Com_IsNecessaryPart(nHand, nPartIdx) != TRUE)
			continue;

		int nCol = 1;
		int nRow = nPartIdx + 1;
		m_ssSelectSheet[nHand].SetCol(nCol);
		m_ssSelectSheet[nHand].SetRow(nRow);

		if (_IsUnitTeachingDataValid(nHand, nPartIdx))
		{
			BOOL bPartSelected = m_stPartSelected.getPartSelected(nHand, nPartIdx);
			m_stPartSelected.setPartSelected(nHand, nPartIdx, bPartSelected);
		}
		else
		{
			if (bIsUnitOnlyOptUse) {
				//Unit제외 Part Disable처리.
				m_stPartSelected.setPartSelected(nHand, nPartIdx, FALSE);

				//Part Select Sheet 색상 변경.
				m_ssSelectSheet[nHand].SetTypeButtonColor(DISABLE_CELL_COLOR);
				m_ssSelectSheet[nHand].SetTypeButtonTextColor(DISABLE_FORE_COLOR);

				//Edit Position 색상 변경.
				if (m_editPositionEachHand[nHand].editPosition[nPartIdx].m_hWnd != NULL) {
					m_editPositionEachHand[nHand].editPosition[nPartIdx].SetBackColor(WWHITE);
				}
			}
			else {
				//이전 Selected Data 추가.
				BOOL bPartSelected = m_stPartSelectedBackUp.getPartSelected(nHand, nPartIdx);
				m_stPartSelected.setPartSelected(nHand, nPartIdx, bPartSelected);

				//색상 변경.
				//_Com_SSDisplaySelectedPart(nHand, nCol, nRow);

				//Part Select Sheet 색상 변경.
				ULONG ulPoscolor = (bPartSelected) ? (LLIGHTBLUE) : (WWHITE);
				m_ssSelectSheet[nHand].SetTypeButtonColor(ulPoscolor);
				m_ssSelectSheet[nHand].SetTypeButtonTextColor(BBLACK);

				//Edit Position 색상 변경.
				if (m_editPositionEachHand[nHand].editPosition[nPartIdx].m_hWnd != NULL) {
					m_editPositionEachHand[nHand].editPosition[nPartIdx].SetBackColor(ulPoscolor);
				}
			}
		}
	}
}

//Hand Teaching Data에 귀속된 Part가 아닌 독립적으로도 Teaching Data가 필요한 Unit 선별 함수.
//Test Pp의 경우, Load Hand만 해당 됨.
BOOL CVATMainTab_VisionAutoTeachingMeasure::_IsUnitTeachingDataValid(int nHand, int nPart)
{
	BOOL bRet = FALSE;

	switch (nHand)
	{
		case VAT_HAND_LOAD_TRAYPP:
		{
			if (nPart == VAT_IDX_TRAY_PP_TBL_2 || nPart == VAT_IDX_TRAY_PP_TBL_3 || nPart == VAT_IDX_TRAY_PP_FEEDER) {
				bRet = TRUE;
			}
		}break;
		case VAT_HAND_UNLOAD_TRAYPP:
		{
			if (nPart == VAT_IDX_TRAY_PP_TBL_1 || nPart == VAT_IDX_TRAY_PP_TBL_4) {
				bRet = TRUE;
			}
		}break;
		case VAT_HAND_LOAD_TESTPP1:
		case VAT_HAND_LOAD_TESTPP2:
		{
			if (VAT_IDX_TEST_PP_TBL_1_LD <= nPart && nPart <= VAT_IDX_TEST_PP_TBL_4_UD) {
				bRet = TRUE;
			}
		}break;
		default:
			break;
	}

	return bRet;
}