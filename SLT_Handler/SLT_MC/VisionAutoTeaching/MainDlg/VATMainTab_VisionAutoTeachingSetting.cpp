// VATMainTab_PartBasePos.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "VATMainTab_VisionAutoTeachingSetting.h"
//#include "VATMainTab_PartBasePos.h"
#include "afxdialogex.h"

/*
=============================================================
<Data 관리 구조>
//Real		(g_VATCommonData.m_stVATPartBasePos)
//Buffer	(g_VATCommonData.m_stVATPartBasePosBuff)
//Gui		(m_stVATPartBasePosGui)
3가지 구조체 존재
=============================================================

1. Real VAT 실측 Data.
2. Buffer <-> Gui
	1) Data 수정 시 - Gui에 저장.
	2) Data Save 시 - Gui -> Buffer에 저장.
	3) Data Save 안하고 Hand 변경 시 - Buffer -> Gui에 저장.
		-> GUI 변경 후, Hand 변경 시 Gui에만 Data남아 있기 때문에 저장하지 않는 Data는 초기화.
*/



// CVATMainTab_PartBasePos 대화 상자입니다.

IMPLEMENT_DYNAMIC(CVATMainTab_VisionAutoTeachingSetting, CDialogEx)

CVATMainTab_VisionAutoTeachingSetting::CVATMainTab_VisionAutoTeachingSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VAT_MAIN_PART_BASE_POS_DLG, pParent)
{
	m_nSS1_DisplayType = eVAT_SSDISPLAYTYPE_DATA;
	m_nSS2_DisplayType = eVAT_SSDISPLAYTYPE_DATA;

	m_nYCheckListCurHand = eYChkListHand_TestPp1;
	m_nYCheckListCurTab = MTPI_TEST_SIDE_1;
}

CVATMainTab_VisionAutoTeachingSetting::~CVATMainTab_VisionAutoTeachingSetting()
{
}

void CVATMainTab_VisionAutoTeachingSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SPREAD_VAT_BASE_POS_LOAD_TRAYPP, m_ssSelectSheet[VAT_HAND_LOAD_TRAYPP]);
	DDX_Control(pDX, IDC_SPREAD_VAT_BASE_POS_UNLOAD_TRAYPP, m_ssSelectSheet[VAT_HAND_UNLOAD_TRAYPP]);
	DDX_Control(pDX, IDC_SPREAD_VAT_BASE_POS_LOAD_TESTPP_1, m_ssSelectSheet[VAT_HAND_LOAD_TESTPP1]);
	DDX_Control(pDX, IDC_SPREAD_VAT_BASE_POS_UNLOAD_TESTPP_1, m_ssSelectSheet[VAT_HAND_UNLOAD_TESTPP1]);
	DDX_Control(pDX, IDC_SPREAD_VAT_BASE_POS_LOAD_TESTPP_2, m_ssSelectSheet[VAT_HAND_LOAD_TESTPP2]);
	DDX_Control(pDX, IDC_SPREAD_VAT_BASE_POS_UNLOAD_TESTPP_2, m_ssSelectSheet[VAT_HAND_UNLOAD_TESTPP2]);

	DDX_Control(pDX, IDC_SPREAD_VAT_HAND_BASE_POSITION, m_ssHandBasePos);
	DDX_Control(pDX, IDC_SPREAD_VAT_PART_BASE_POSITION, m_ssPartBasePos);

	////Panel
	DDX_Control(pDX, IDC_SSPANEL_VAT_MAIN_TITLE_1, m_ssDataPanel_1_Title);
	DDX_Control(pDX, IDC_SSPANEL_VAT_MAIN_TITLE_2, m_ssDataPanel_2_Title);
	DDX_Control(pDX, IDC_SSPANEL_VAT_MAIN_TITLE_3, m_ssDataPanel_3_Title);
	DDX_Control(pDX, IDC_SSPANEL_VAT_MAIN_TITLE_4, m_ssDataPanel_4_Title);
	DDX_Control(pDX, IDC_SSPANEL_VAT_HAND_BASE_POS_DATA, m_ssDataPanel_1_Sub1);
	DDX_Control(pDX, IDC_SSPANEL_VAT_HAND_BASE_POS_DATA_DIFF, m_ssDataPanel_1_Sub2);
	DDX_Control(pDX, IDC_SSPANEL_VAT_PART_BASE_POS_DATA, m_ssDataPanel_2_Sub1);
	DDX_Control(pDX, IDC_SSPANEL_VAT_PART_BASE_POS_DATA_DIFF, m_ssDataPanel_2_Sub2);
	DDX_Control(pDX, IDC_SSPANEL_PART_BASE_POSITION_BACK_GROUND, m_ssPartBasePosBackColor);
	
	//Operate Button
	DDX_Control(pDX, IDC_BTN_VAT_HAND_BASE_POS_ENCODER_READ, m_BtnOper1_EncoderRead);
	DDX_Control(pDX, IDC_BTN_VAT_HAND_BASE_POS_MOTOR_MOVE, m_BtnOper1_MotorMove);
	DDX_Control(pDX, IDC_BTN_VAT_HAND_BASE_POS_RELOAD, m_BtnOper1_Reload);
	DDX_Control(pDX, IDC_BTN_VAT_HAND_BASE_POS_DATA_SAVE, m_BtnOper1_Save);
	DDX_Control(pDX, IDC_BTN_VAT_PART_BASE_POS_ENCODER_READ, m_BtnOper2_EncoderRead);
	DDX_Control(pDX, IDC_BTN_VAT_PART_BASE_POS_MOTOR_MOVE, m_BtnOper2_MotorMove);
	DDX_Control(pDX, IDC_BTN_VAT_PART_BASE_POS_RELOAD, m_BtnOper2_Reload);
	DDX_Control(pDX, IDC_BTN_VAT_PART_BASE_POS_DATA_SAVE, m_BtnOper2_Save);
	DDX_Control(pDX, IDC_BTN_VAT_HAND_BASE_POS_MAKE_BASE_POS, m_BtnOper_MakeBasePos);

	//[Test Site Y Axis Check List]	
	DDX_Control(pDX, IDC_SPREAD_TEST_SITE_Y_CHECK_HAND1, m_ssSelectSheetSub[eYChkListHand_TestPp1]);
	DDX_Control(pDX, IDC_SPREAD_TEST_SITE_Y_CHECK_HAND2, m_ssSelectSheetSub[eYChkListHand_TestPp2]);

	DDX_Control(pDX, IDC_TAB_VAT_TEST_SITE_Y_CHECK, m_tabTestSiteYCheckList);

	DDX_Control(pDX, IDC_SSPANEL_Y_CHECK_LIST_TEST_PP_1_STATUS, m_ssPanelYCheckList_HandStatus1);
	DDX_Control(pDX, IDC_SSPANEL_Y_CHECK_LIST_TEST_PP_2_STATUS, m_ssPanelYCheckList_HandStatus2);

	DDX_Control(pDX, IDC_SSPANEL_Y_CHECK_LIST_BMP, m_ssPanelYCheckList_Bmp);

	DDX_Control(pDX, IDC_SSPANEL_Y_CHECK_LIST_STATUS_1, m_ssPanelYCheckList_Status1);
	DDX_Control(pDX, IDC_SSPANEL_Y_CHECK_LIST_STATUS_2, m_ssPanelYCheckList_Status2);
	DDX_Control(pDX, IDC_SSPANEL_Y_CHECK_LIST_STATUS_3, m_ssPanelYCheckList_Status3);
}


BEGIN_MESSAGE_MAP(CVATMainTab_VisionAutoTeachingSetting, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_VAT_HAND_BASE_POS_ENCODER_READ, &CVATMainTab_VisionAutoTeachingSetting::OnBnClickedBtnVatHandBasePosEncoderRead)
	ON_BN_CLICKED(IDC_BTN_VAT_HAND_BASE_POS_MOTOR_MOVE, &CVATMainTab_VisionAutoTeachingSetting::OnBnClickedBtnVatHandBasePosMotorMove)
	ON_BN_CLICKED(IDC_BTN_VAT_HAND_BASE_POS_RELOAD, &CVATMainTab_VisionAutoTeachingSetting::OnBnClickedBtnVatHandBasePosReload)
	ON_BN_CLICKED(IDC_BTN_VAT_HAND_BASE_POS_DATA_SAVE, &CVATMainTab_VisionAutoTeachingSetting::OnBnClickedBtnVatHandBasePosDataSave)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_VAT_TEST_SITE_Y_CHECK, &CVATMainTab_VisionAutoTeachingSetting::OnTcnSelchangeTabVatTestSiteYCheck)
	ON_BN_CLICKED(IDC_BTN_VAT_PART_BASE_POS_ENCODER_READ, &CVATMainTab_VisionAutoTeachingSetting::OnBnClickedBtnVatPartBasePosEncoderRead)
	ON_BN_CLICKED(IDC_BTN_VAT_PART_BASE_POS_MOTOR_MOVE, &CVATMainTab_VisionAutoTeachingSetting::OnBnClickedBtnVatPartBasePosMotorMove)
	ON_BN_CLICKED(IDC_BTN_VAT_PART_BASE_POS_RELOAD, &CVATMainTab_VisionAutoTeachingSetting::OnBnClickedBtnVatPartBasePosReload)
	ON_BN_CLICKED(IDC_BTN_VAT_PART_BASE_POS_DATA_SAVE, &CVATMainTab_VisionAutoTeachingSetting::OnBnClickedBtnVatPartBasePosDataSave)
END_MESSAGE_MAP()


// CVATMainTab_PartBasePos 메시지 처리기입니다.


BOOL CVATMainTab_VisionAutoTeachingSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//Data Load
	m_stVATInfoGui = g_VATCommonData.m_stVATInfo;

	//Title SSPanel 초기화.
	InitSSTitlePanel();

	//Arm 선택 Spread Sheet 초기화.
	_Com_InitHandSelectSheet();
	
	//Vision Auto Teaching Spread Sheet 초기화.
	InitVATBasePosSheet();

	//초기 Data Display(VAT_HAND_LOAD_TRAYPP, eVAT_SSDISPLAYTYPE_DATA)
	DisplaySSData(_eVatSS_HandBasePos, m_stHandBasePosLastInfo.nHand, m_nSS1_DisplayType);
	//초기 Hand Spread Sheet Part 선택 안했기 때문에 Data Display할 필요 없음.
	//DisplaySSData(_eVatSS_PartBasePos, m_stHandBasePosLastInfo.nHand, m_nSS2_DisplayType); 

	//Operate Button 초기화
	InitVATOperBtn();

	//Current Vision Target 초기화.
	DisplayCurVisionTarget(-1, -1);

	//[Test Site Y Axis Check List]	
	InitTestSiteYAxisCheckList();

	return TRUE; 
}


//=====================================================================================
//COMMENT : CVATMainTab_PartBasePos 클래스는 Z Auto Teaching 및 Vision Auto Teaching 
//          MainTab에서 각각 사용하기 때문에 PageMove시, Data 다시 Update하여 갱신.
//=====================================================================================
void CVATMainTab_VisionAutoTeachingSetting::PageMoveInit_Tab()
{
	DisplaySSData(_eVatSS_HandBasePos, m_stHandBasePosLastInfo.nHand, m_nSS1_DisplayType);
}


void CVATMainTab_VisionAutoTeachingSetting::PageMoveDispose_Tab()
{

}


BEGIN_EVENTSINK_MAP(CVATMainTab_VisionAutoTeachingSetting, CDialogEx)
	ON_EVENT(CVATMainTab_VisionAutoTeachingSetting, IDC_SPREAD_VAT_HAND_BASE_POSITION, 5, CVATMainTab_VisionAutoTeachingSetting::ClickSpreadHandBasePosition, VTS_I4 VTS_I4)
	ON_EVENT(CVATMainTab_VisionAutoTeachingSetting, IDC_SPREAD_VAT_HAND_BASE_POSITION, 11, CVATMainTab_VisionAutoTeachingSetting::DblClickSpreadHandBasePosition, VTS_I4 VTS_I4)

	ON_EVENT(CVATMainTab_VisionAutoTeachingSetting, IDC_SSPANEL_VAT_HAND_BASE_POS_DATA, DISPID_CLICK, CVATMainTab_VisionAutoTeachingSetting::ClickSspanelHandBasePosData, VTS_NONE)
	ON_EVENT(CVATMainTab_VisionAutoTeachingSetting, IDC_SSPANEL_VAT_HAND_BASE_POS_DATA_DIFF, DISPID_CLICK, CVATMainTab_VisionAutoTeachingSetting::ClickSspanelHandBasePosDataDiff, VTS_NONE)

	ON_EVENT(CVATMainTab_VisionAutoTeachingSetting, IDC_SPREAD_VAT_PART_BASE_POSITION, 5, CVATMainTab_VisionAutoTeachingSetting::ClickSpreadVatPartBasePosition, VTS_I4 VTS_I4)
	ON_EVENT(CVATMainTab_VisionAutoTeachingSetting, IDC_SPREAD_VAT_PART_BASE_POSITION, 11, CVATMainTab_VisionAutoTeachingSetting::DblClickSpreadVatPartBasePosition, VTS_I4 VTS_I4)

	ON_EVENT(CVATMainTab_VisionAutoTeachingSetting, IDC_SPREAD_VAT_BASE_POS_LOAD_TRAYPP, 3, CVATMainTab_VisionAutoTeachingSetting::ButtonClickedSpreadVatBasePosLoadTraypp, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMainTab_VisionAutoTeachingSetting, IDC_SPREAD_VAT_BASE_POS_UNLOAD_TRAYPP, 3, CVATMainTab_VisionAutoTeachingSetting::ButtonClickedSpreadVatBasePosUnloadTraypp, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMainTab_VisionAutoTeachingSetting, IDC_SPREAD_VAT_BASE_POS_LOAD_TESTPP_1, 3, CVATMainTab_VisionAutoTeachingSetting::ButtonClickedSpreadVatBasePosLoadTestpp1, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMainTab_VisionAutoTeachingSetting, IDC_SPREAD_VAT_BASE_POS_UNLOAD_TESTPP_1, 3, CVATMainTab_VisionAutoTeachingSetting::ButtonClickedSpreadVatBasePosUnloadTestpp1, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMainTab_VisionAutoTeachingSetting, IDC_SPREAD_VAT_BASE_POS_LOAD_TESTPP_2, 3, CVATMainTab_VisionAutoTeachingSetting::ButtonClickedSpreadVatBasePosLoadTestpp2, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMainTab_VisionAutoTeachingSetting, IDC_SPREAD_VAT_BASE_POS_UNLOAD_TESTPP_2, 3, CVATMainTab_VisionAutoTeachingSetting::ButtonClickedSpreadVatBasePosUnloadTestpp2, VTS_I4 VTS_I4 VTS_I2)

	ON_EVENT(CVATMainTab_VisionAutoTeachingSetting, IDC_SPREAD_TEST_SITE_Y_CHECK_HAND1, 3, CVATMainTab_VisionAutoTeachingSetting::ButtonClickedSpreadTestSiteYCheckHand1, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMainTab_VisionAutoTeachingSetting, IDC_SPREAD_TEST_SITE_Y_CHECK_HAND2, 3, CVATMainTab_VisionAutoTeachingSetting::ButtonClickedSpreadTestSiteYCheckHand2, VTS_I4 VTS_I4 VTS_I2)
	ON_EVENT(CVATMainTab_VisionAutoTeachingSetting, IDC_SSPANEL_Y_CHECK_LIST_STATUS_1, DISPID_CLICK, CVATMainTab_VisionAutoTeachingSetting::ClickSspanelYCheckListStatus1, VTS_NONE)
	ON_EVENT(CVATMainTab_VisionAutoTeachingSetting, IDC_SSPANEL_Y_CHECK_LIST_STATUS_2, DISPID_CLICK, CVATMainTab_VisionAutoTeachingSetting::ClickSspanelYCheckListStatus2, VTS_NONE)
	ON_EVENT(CVATMainTab_VisionAutoTeachingSetting, IDC_SSPANEL_Y_CHECK_LIST_STATUS_3, DISPID_CLICK, CVATMainTab_VisionAutoTeachingSetting::ClickSspanelYCheckListStatus3, VTS_NONE)
	ON_EVENT(CVATMainTab_VisionAutoTeachingSetting, IDC_SSPANEL_VAT_PART_BASE_POS_DATA, DISPID_CLICK, CVATMainTab_VisionAutoTeachingSetting::ClickSspanelVatPartBasePosData, VTS_NONE)
	ON_EVENT(CVATMainTab_VisionAutoTeachingSetting, IDC_SSPANEL_VAT_PART_BASE_POS_DATA_DIFF, DISPID_CLICK, CVATMainTab_VisionAutoTeachingSetting::ClickSspanelVatPartBasePosDataDiff, VTS_NONE)
END_EVENTSINK_MAP()


void CVATMainTab_VisionAutoTeachingSetting::InitSSTitlePanel()
{
// 	CDSSPanel* panel[4] = {
// 		&m_ssPanel_HandSelect, &m_ssDataPanel_1_Sub1, &m_ssDataPanel_1_Sub2,
// 	};
// 	for (auto p : panel) {
// 		_Com_InitSSTitlePanel(*p);
//   	}

	//_Com_InitSSDataPanel(m_ssDataPanel_1_Title, m_ssDataPanel_1_Sub1, m_ssDataPanel_1_Sub2);
	//_Com_InitSSDataPanel(m_ssDataPanel_2_Title, m_ssDataPanel_2_Sub1, m_ssDataPanel_2_Sub2);

	//Data, Difference중 초기 Init은 Data Panel로 선택
	m_nSS1_DisplayType = eVAT_SSDISPLAYTYPE_DATA;
	m_nSS2_DisplayType = eVAT_SSDISPLAYTYPE_DATA;

	m_ssDataPanel_1_Sub1.SetForeColor(BBLACK);
	m_ssDataPanel_1_Sub1.SetBackColor(LLIGHTCYAN);
	m_ssDataPanel_1_Sub2.SetForeColor(BBLACK);
	m_ssDataPanel_1_Sub2.SetBackColor(TAB_BACKGROUND_COLOR);

	m_ssDataPanel_2_Sub1.SetForeColor(BBLACK);
	m_ssDataPanel_2_Sub1.SetBackColor(LLIGHTCYAN);
	m_ssDataPanel_2_Sub2.SetForeColor(BBLACK);
	m_ssDataPanel_2_Sub2.SetBackColor(TAB_BACKGROUND_COLOR);

	m_ssPartBasePosBackColor.SetBackColor(RGB(255, 215, 80));// //RGB(255, 230, 120)
	m_ssPartBasePosBackColor.ShowWindow(FALSE);

}


void CVATMainTab_VisionAutoTeachingSetting::InitVATOperBtn()
{
	//Hand Base Position (Read, Move, Reload, Save, MakeBasePos )
	CShadeButtonST *btnOper1[eSSOper_MAX] = { &m_BtnOper1_EncoderRead, &m_BtnOper1_MotorMove, &m_BtnOper1_Reload, &m_BtnOper1_Save, &m_BtnOper_MakeBasePos };
	for (int nType = 0; nType < eSSOper_MAX; nType++) {
		_Com_InitOperBtn(*btnOper1[nType], nType);
	}

	//Part Base Position (Read, Move, Reload, Save)
	CShadeButtonST *btnOper2[4] = { &m_BtnOper2_EncoderRead, &m_BtnOper2_MotorMove, &m_BtnOper2_Reload, &m_BtnOper2_Save};
	for (int nType = 0; nType < 4; nType++) {
		_Com_InitOperBtn(*btnOper2[nType], nType);
	}
}


void CVATMainTab_VisionAutoTeachingSetting::InitVATBasePosSheet()
{
	//초기 Setting [LD Tray Pp]로 진행.

	//Hand 선택 색상 초기 Setting.
	_Com_ButtonClickedSpreadHand(1, 1, VAT_HAND_LOAD_TRAYPP, eVAT_SSTYPE_PART);

	//Spread Sheet1 초기 Setting.
	_Com_InitPosSpreadSheet_PartType(m_ssHandBasePos, VAT_HAND_LOAD_TRAYPP);
	m_stHandBasePosLastInfo.nSSType = eVAT_SSTYPE_PART;
	m_stHandBasePosLastInfo.nHand = VAT_HAND_LOAD_TRAYPP;

	//Spread Sheet2 초기 Setting.
	InitPosSpreadSheet_PartOnly(VAT_HAND_LOAD_TRAYPP, VAT_PART_NONE/*(-1)*/);
	m_stPartBasePosLastInfo.nSSType = eVAT_SSTYPE_PART_ONLY; 

	m_ssHandBasePos.SetRow(0);
	m_ssHandBasePos.SetCol(0);
	m_ssHandBasePos.SetTextMatrix(0, 0, _T("HAND"));
	m_ssHandBasePos.SetBackColor(RED);

}



void CVATMainTab_VisionAutoTeachingSetting::DisplaySSData(int nVatSSType, int nHand, int nSSDisplayType)
{
	if (nVatSSType == _eVatSS_HandBasePos)
	{
		int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
		for (int nPart = 0; nPart < nMaxPart; nPart++) {
			for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XYZ; nAxis++) {
				double dPos = 0.0;
				if (nSSDisplayType == eVAT_SSDISPLAYTYPE_DATA) {
					dPos = g_VATCommonData.m_stVATInfo.stHandBasePos.getBasePos(nHand, nPart, nAxis);
				}
				else {
					dPos = g_VATCommonData.m_stVATInfo.stHandBasePos.getBasePos(nHand, nPart, nAxis) - g_VATCommonData.m_stVATInfoBuff.stHandBasePos.getBasePos(nHand, nPart, nAxis);
				}

				CString strPos = _T("");
				strPos.Format("%.3f", dPos / 1000.0);
				int nCol = nAxis + 1;
				int nRow = nPart + 1;

				m_ssHandBasePos.SetTextMatrix(nRow, nCol, strPos);

				//Diff에서 Limit 상관 없음.
				if (nSSDisplayType == eVAT_SSDISPLAYTYPE_DATA){
					//Pos/Neg Limit에 따른 글자 색상 변경.
					BOOL bIsLimitPos = _Com_BasePositionLimitCheck(nHand, nAxis, dPos);
					OLE_COLOR color = (bIsLimitPos) ? LIMIT_COLOR_RED : NORMAL_COLOR_BLACK;
					m_ssHandBasePos.SetForeColor(color);
				}
				else{
					m_ssHandBasePos.SetForeColor(NORMAL_COLOR_BLACK);
				}

			}
		}

		//Cell Block
		_Com_BlockCell(m_ssHandBasePos, nHand);
	}
	else //nVatSSType == _eVatSS_PartBasePos
	{
		int nSSPartIdx = m_stHandBasePosLastInfo.nRow - 1;
		int nVATPartIdx = ConvSSPartToVATPartIdx(nHand, nSSPartIdx);

		//Spread Sheet Part가 VAT 측정 Part가 아닐 경우, Data Display 하지 않음.(nVATPartIdx == VAT_PART_NONE)
		if (nVATPartIdx == VAT_PART_NONE) {
			m_ssPartBasePosBackColor.ShowWindow(FALSE);
			return;
		}
		else {
			m_ssPartBasePosBackColor.ShowWindow(TRUE);
		}

		//Spread Sheet Part가 VAT 측정 Part일 경우, Data Display
		int nMaxPartPos = (nVATPartIdx == VAT_PART_FEEDER) ? VAT_MAX_FEEDER_LOC_COUNT : VAT_MAX_TBL_LOC_COUNT;
		int nMaxAxis = (nVATPartIdx == VAT_PART_FEEDER) ? VAT_MAX_AXIS_Y : VAT_MAX_AXIS_YROTATE;
		for (int nPartPos = 0; nPartPos < nMaxPartPos; nPartPos++) {

			BOOL bIsPartBasePosValidate = ChkPartBasePosValid(nHand, nSSPartIdx, nPartPos);


			for (int nAxis = 0; nAxis < nMaxAxis; nAxis++) {
				double dGuiPos = 0.0;
				double dRealPos = g_VATCommonData.m_stVATInfo.stPartBasePos.getBasePos(nVATPartIdx, nPartPos, nAxis);
				double dBuffPos = g_VATCommonData.m_stVATInfoBuff.stPartBasePos.getBasePos(nVATPartIdx, nPartPos, nAxis);
				if (nSSDisplayType == eVAT_SSDISPLAYTYPE_DATA) {
					dGuiPos = dRealPos;
				}
				else {
					dGuiPos = dRealPos - dBuffPos;
				}

				CString strPos = _T("");
				strPos.Format("%.3f", dGuiPos / 1000.0);
				int nCol = nAxis + 1;
				int nRow = nPartPos + 1;

				m_ssPartBasePos.SetTextMatrix(nRow, nCol, strPos);


			   //Cell 색상 Setting 순서 변경 금지.  [12/9/2025 dohyeong.kim]

				//Diff에서 Limit 상관 없음.
				if (nSSDisplayType == eVAT_SSDISPLAYTYPE_DATA){
					//Pos/Neg Limit에 따른 글자 색상 변경.
					BOOL bIsLimitPos = PartBasePositionLimitCheck(nVATPartIdx, nAxis, dGuiPos);
					OLE_COLOR color = (bIsLimitPos) ? LIMIT_COLOR_RED : NORMAL_COLOR_BLACK;
					m_ssPartBasePos.SetForeColor(color);
				}
				else{
					m_ssPartBasePos.SetForeColor(NORMAL_COLOR_BLACK);
				}


				//Click한 Part의 Base Position만 표기.
				m_ssPartBasePos.SetCol(nCol);
				m_ssPartBasePos.SetRow(nRow);
				if (bIsPartBasePosValidate) {
					//색상 Enable 처리.
					m_ssPartBasePos.SetBackColor(WWHITE);
					m_ssPartBasePos.SetForeColor(BBLACK);
				}
				else {
					//색상 Disable 처리.
					m_ssPartBasePos.SetBackColor(DISABLE_CELL_COLOR);
					m_ssPartBasePos.SetForeColor(DISABLE_FORE_COLOR);
				}



			}
		}	
	}
}


//===========================================================================================
// COMMENT : Current Pos와 Pos/Neg Limit 비교 하지 않고 Display 되어 있는 Data 색상으로 판단.
//			 Data Display시, 이미 Limit 계산 진행 완료.
//			 빠른 확인을 위해, 색상으로만 Limit Check 진행.
//========================================================================================
BOOL CVATMainTab_VisionAutoTeachingSetting::CheckPosLimit(int nVatSSType)
{
	if (!(_eVatSS_HandBasePos <= nVatSSType && nVatSSType <= _eVatSS_PartBasePos))
		return TRUE;

	CSpreadSheet* ssBasePos[_eVatSS_MAX] = { &m_ssHandBasePos , &m_ssPartBasePos };

	for (auto nHand : m_vAllHand) {
		int nMAX_COL = ssBasePos[nVatSSType]->GetMaxCols();
		int nMAX_ROW = ssBasePos[nVatSSType]->GetMaxRows();
		for (int nRow = 0; nRow < nMAX_ROW; nRow++) {
			for (int nCol = 0; nCol < nMAX_COL; nCol++) {
				ssBasePos[nVatSSType]->SetRow(nRow);
				ssBasePos[nVatSSType]->SetCol(nCol);
				OLE_COLOR color = ssBasePos[nVatSSType]->GetForeColor();
				if (color == LIMIT_COLOR_RED) {
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

// void CVATMainTab_VisionAutoTeachingSetting::DataCopyGuiToReal(int nHand)
// {
// 	switch (nHand)
// 	{
// 	case VAT_HAND_LOAD_TRAYPP:
// 		memcpy_s(g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_LD_TrayPp, sizeof(g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_LD_TrayPp),
// 			m_stVATPartBasePosGui.dPartBasePos_LD_TrayPp, sizeof(m_stVATPartBasePosGui.dPartBasePos_LD_TrayPp));
// 		break;
// 	case VAT_HAND_UNLOAD_TRAYPP:
// 		memcpy_s(g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_UD_TrayPp, sizeof(g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_UD_TrayPp),
// 			m_stVATPartBasePosGui.dPartBasePos_UD_TrayPp, sizeof(m_stVATPartBasePosGui.dPartBasePos_UD_TrayPp));
// 		break;
// 	case VAT_HAND_LOAD_TESTPP1:
// 		memcpy_s(g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_LD_TestPp1, sizeof(g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_LD_TestPp1),
// 			m_stVATPartBasePosGui.dPartBasePos_LD_TestPp1, sizeof(m_stVATPartBasePosGui.dPartBasePos_LD_TestPp1));
// 		break;
// 	case VAT_HAND_UNLOAD_TESTPP1:
// 		memcpy_s(g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_UD_TestPp1, sizeof(g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_UD_TestPp1),
// 			m_stVATPartBasePosGui.dPartBasePos_UD_TestPp1, sizeof(m_stVATPartBasePosGui.dPartBasePos_UD_TestPp1));
// 		break;
// 	case VAT_HAND_LOAD_TESTPP2:
// 		memcpy_s(g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_LD_TestPp2, sizeof(g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_LD_TestPp2),
// 			m_stVATPartBasePosGui.dPartBasePos_LD_TestPp2, sizeof(m_stVATPartBasePosGui.dPartBasePos_LD_TestPp2));
// 		break;
// 	case VAT_HAND_UNLOAD_TESTPP2:
// 		memcpy_s(g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_UD_TestPp2, sizeof(g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_UD_TestPp2),
// 			m_stVATPartBasePosGui.dPartBasePos_UD_TestPp2, sizeof(m_stVATPartBasePosGui.dPartBasePos_UD_TestPp2));
// 		break;
// 	case VAT_HAND_MAX:
// 		memcpy_s(&g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos, sizeof(g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos), &m_stVATPartBasePosGui, sizeof(m_stVATPartBasePosGui));
// 		break;
// 	default:
// 		break;
//  	}
// 	
// }
// 
// void CVATMainTab_VisionAutoTeachingSetting::DataCopyRealToGui(int nHand)
// {
// 	switch (nHand)
// 	{
// 	case VAT_HAND_LOAD_TRAYPP:
// 		memcpy_s(m_stVATPartBasePosGui.dPartBasePos_LD_TrayPp, sizeof(m_stVATPartBasePosGui.dPartBasePos_LD_TrayPp),
// 			g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_LD_TrayPp, sizeof(g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_LD_TrayPp));
// 		break;
// 	case VAT_HAND_UNLOAD_TRAYPP:
// 		memcpy_s(m_stVATPartBasePosGui.dPartBasePos_UD_TrayPp, sizeof(m_stVATPartBasePosGui.dPartBasePos_UD_TrayPp),
// 			g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_UD_TrayPp, sizeof(g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_UD_TrayPp));
// 		break;
// 	case VAT_HAND_LOAD_TESTPP1:
// 		memcpy_s(m_stVATPartBasePosGui.dPartBasePos_LD_TestPp1, sizeof(m_stVATPartBasePosGui.dPartBasePos_LD_TestPp1),
// 			g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_LD_TestPp1, sizeof(g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_LD_TestPp1));
// 		break;
// 	case VAT_HAND_UNLOAD_TESTPP1:
// 		memcpy_s(m_stVATPartBasePosGui.dPartBasePos_UD_TestPp1, sizeof(m_stVATPartBasePosGui.dPartBasePos_UD_TestPp1),
// 			g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_UD_TestPp1, sizeof(g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_UD_TestPp1));
// 		break;
// 	case VAT_HAND_LOAD_TESTPP2:
// 		memcpy_s(m_stVATPartBasePosGui.dPartBasePos_LD_TestPp2, sizeof(m_stVATPartBasePosGui.dPartBasePos_LD_TestPp2),
// 			g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_LD_TestPp2, sizeof(g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_LD_TestPp2));
// 		break;
// 	case VAT_HAND_UNLOAD_TESTPP2:
// 		memcpy_s(m_stVATPartBasePosGui.dPartBasePos_UD_TestPp2, sizeof(m_stVATPartBasePosGui.dPartBasePos_UD_TestPp2),
// 			g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_UD_TestPp2, sizeof(g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos.dPartBasePos_UD_TestPp2));
// 		break;
// 	case VAT_HAND_MAX:
// 		memcpy_s(&m_stVATPartBasePosGui, sizeof(m_stVATPartBasePosGui), &g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos, sizeof(g_VATCommonData.m_stVATVisionAutoTeachingInfo.stPartBasePos));
// 		break;
// 	default:
// 		break;
// 	}
// }

void CVATMainTab_VisionAutoTeachingSetting::ButtonClickedSpreadVatBasePosLoadTraypp(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ButtonClickHandChange(Col, Row, VAT_HAND_LOAD_TRAYPP);
}


void CVATMainTab_VisionAutoTeachingSetting::ButtonClickedSpreadVatBasePosUnloadTraypp(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ButtonClickHandChange(Col, Row, VAT_HAND_UNLOAD_TRAYPP);
}


void CVATMainTab_VisionAutoTeachingSetting::ButtonClickedSpreadVatBasePosLoadTestpp1(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ButtonClickHandChange(Col, Row, VAT_HAND_LOAD_TESTPP1);
}


void CVATMainTab_VisionAutoTeachingSetting::ButtonClickedSpreadVatBasePosUnloadTestpp1(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ButtonClickHandChange(Col, Row, VAT_HAND_UNLOAD_TESTPP1);
}


void CVATMainTab_VisionAutoTeachingSetting::ButtonClickedSpreadVatBasePosLoadTestpp2(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ButtonClickHandChange(Col, Row, VAT_HAND_LOAD_TESTPP2);
}


void CVATMainTab_VisionAutoTeachingSetting::ButtonClickedSpreadVatBasePosUnloadTestpp2(long Col, long Row, short ButtonDown)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ButtonClickHandChange(Col, Row, VAT_HAND_UNLOAD_TESTPP2);
}


//Hand Select Click시, 변경 사항.
void CVATMainTab_VisionAutoTeachingSetting::ButtonClickHandChange(long Col, long Row, int nHand)
{
	//Hand Color & Select/Unselect 변경.
	_Com_ButtonClickedSpreadHand(Col, Row, nHand, eVAT_SSTYPE_PART/*, m_editPosition*/);

	//HAnd Base Pos Spread Sheet Init
	_Com_InitPosSpreadSheet_PartType(m_ssHandBasePos, nHand);

	//Hand 변경 시, Last Info Hand 변경.
	_Com_UpdateLastHandData(m_stHandBasePosLastInfo, nHand);

	//Hand 변경 시, Spread Sheet 색상 초기화 및 Last Info Data 초기화(Last Info Hand 제외).
	_Com_Clear_SpreadSheet(m_ssHandBasePos, m_stHandBasePosLastInfo);
	
	//Current Vision Target 초기화.
	DisplayCurVisionTarget(nHand, -1);
	//Data Real -> Gui로 저장.
	//DataCopyRealToGui(nHand);

	//Display Data
	DisplaySSData(_eVatSS_HandBasePos, nHand, m_nSS1_DisplayType);

	//Hand 변경 시, Spread Sheet 색상이 초기화 되므로 Part Unselect 상태임.
	//이에, Part Base Position도 Unselect 상태로 변경 필요.
	ButtonClickPartChange();
}

void CVATMainTab_VisionAutoTeachingSetting::ClickSpreadHandBasePosition(long Col, long Row)
{
	BOOL bIsBlockCell = _Com_CheckBlockCell(m_stHandBasePosLastInfo.nHand, Col/*, Row*/);
	if (bIsBlockCell == TRUE)
		return;

	_Com_UpdateSheetCellColor(m_ssHandBasePos, m_stHandBasePosLastInfo, Col, Row);

	//Hand Base Position의 Part가 변경 될 때만, Part Base Position 변경 함.
	//Row(Part)만 확인, Col은 Part 변동에 해당 안함.
	BOOL bIsSSPartIdxChange = FALSE;
	if (Row != m_stHandBasePosLastInfo.nRow) { 
		bIsSSPartIdxChange = TRUE;
	}

	_Com_UpdateLastSheetData(m_stHandBasePosLastInfo, Col, Row);

	
	//Hand Spread Sheet Part Idx가 변경 될 때만, 적용(Axis는 적용 안됨)
	if (bIsSSPartIdxChange) {
		//Hand Base Position <-> Part Base Position 연동
		ButtonClickPartChange();

		//Vision Target Overview
		DisplayCurVisionTarget(m_stHandBasePosLastInfo.nHand, m_stHandBasePosLastInfo.nRow - 1); // Hand, Part
	}
}


void CVATMainTab_VisionAutoTeachingSetting::DblClickSpreadHandBasePosition(long Col, long Row)
{
	BOOL bIsBlockCell = _Com_CheckBlockCell(m_stHandBasePosLastInfo.nHand, Col/*, Row*/);
	if (bIsBlockCell == TRUE)
		return;

	if (_Com_SSInterlock_DblClick(Col, Row, m_nSS1_DisplayType) != eVAT_INTERLOCK_PASS)
		return;

	_Com_DblClickSheetTextChange(m_ssHandBasePos, m_stHandBasePosLastInfo.nHand, this, Col, Row);

	CString strCurValue = m_ssHandBasePos.GetTextMatrix(Row, Col);

	int nPart = Row - 1;
	int nAxis = Col - 1;
	double dPos = atof(strCurValue) * 1000.0;

	m_stVATInfoGui.stHandBasePos.setBasePos(m_stHandBasePosLastInfo.nHand, nPart, nAxis, dPos); //hand, part, axis, pos
}


void CVATMainTab_VisionAutoTeachingSetting::ClickSspanelHandBasePosData()
{
	m_nSS1_DisplayType = eVAT_SSDISPLAYTYPE_DATA;

	//sspanel color 변경.
	_Com_ChangeSSPanelColor(m_ssDataPanel_1_Sub1, m_ssDataPanel_1_Sub2, m_nSS1_DisplayType);

	//Display Data
	DisplaySSData(_eVatSS_HandBasePos, m_stHandBasePosLastInfo.nHand, m_nSS1_DisplayType);
}


void CVATMainTab_VisionAutoTeachingSetting::ClickSspanelHandBasePosDataDiff()
{
	m_nSS1_DisplayType = eVAT_SSDISPLAYTYPE_DIFFERENCE;

	//sspanel color 변경.
	_Com_ChangeSSPanelColor(m_ssDataPanel_1_Sub1, m_ssDataPanel_1_Sub2, m_nSS1_DisplayType);

	//Display Data
	DisplaySSData(_eVatSS_HandBasePos, m_stHandBasePosLastInfo.nHand, m_nSS1_DisplayType);
}


void CVATMainTab_VisionAutoTeachingSetting::OnBnClickedBtnVatHandBasePosEncoderRead()
{
	double dCurPos[VAT_MAX_AXIS_XYZ] = { 0.0, };

	int nRet = _Com_Oper_EncoderRead(dCurPos, m_stHandBasePosLastInfo, m_nSS1_DisplayType);
	if (nRet == ERR_VAT_ERROR)
		return;

	for (int nAxis = VAT_X; nAxis < VAT_MAX_AXIS_XYZ; nAxis++)
	{
		//Spread Sheet 표기.
		CString strCurPos = _T("");
		strCurPos.Format("%.3f", dCurPos[nAxis] / 1000.0);

		int nCol = nAxis + 1;
		m_ssHandBasePos.SetTextMatrix(m_stHandBasePosLastInfo.nRow, nCol, strCurPos);

		//Data Update
		int nHand = m_stHandBasePosLastInfo.nHand;
		int nPart = m_stHandBasePosLastInfo.nRow - 1;
		m_stVATInfoGui.stHandBasePos.setBasePos(nHand, nPart, nAxis, dCurPos[nAxis]);
	}
}


void CVATMainTab_VisionAutoTeachingSetting::OnBnClickedBtnVatHandBasePosMotorMove()
{
	//Manual Cmd 생성 후, 작업.
}


void CVATMainTab_VisionAutoTeachingSetting::OnBnClickedBtnVatHandBasePosReload()
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
	m_stVATInfoGui.stHandBasePos = g_VATCommonData.m_stVATInfo.stHandBasePos;
	
	DisplaySSData(_eVatSS_HandBasePos, m_stHandBasePosLastInfo.nHand, m_nSS1_DisplayType);

	//Hand Spread Sheet 초기화.
	_Com_Clear_SpreadSheet(m_ssHandBasePos, m_stHandBasePosLastInfo);

	//Current Vision Target 초기화.
	DisplayCurVisionTarget(m_stHandBasePosLastInfo.nHand, -1);

	//Hand의 Part가 변경 되었기 때문에(Part == -1) Part Spread Sheet도 초기화.[숨김 처리]
	ButtonClickPartChange();
}


void CVATMainTab_VisionAutoTeachingSetting::OnBnClickedBtnVatHandBasePosDataSave()
{
	//Alarm 비활성화 상태 Check
	_eVAT_INTERLOCK bChkInterlock = _Com_SSInterlock_AlarmDeActivate();
	if (bChkInterlock != eVAT_INTERLOCK_PASS)
		return;

	//"Do you want to Base Position Save?"
	int nRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_DATA_SAVE, ERR_VAT_TYPE_YESNO);
	if (nRet == IDNO)
		return;

	BOOL IsLimitPosExist = CheckPosLimit(_eVatSS_HandBasePos);
	if (IsLimitPosExist) {
		int nRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_LIMIT_OVER_CHECK, ERR_VAT_TYPE_YESNO);
		if (nRet == IDNO){
			return;
		}
	}

	for (int nHand = 0; nHand < VAT_HAND_MAX; nHand++) {
		int nMaxPart = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? VAT_MAX_TRAY_PP_LOC_COUNT : VAT_MAX_TEST_PP_LOC_COUNT;
		for (int nPart = 0; nPart < nMaxPart; nPart++) {
			for (int nAxis = 0; nAxis < VAT_MAX_AXIS_XYZ; nAxis++) {
				//GUI <-> Real 다른 값만 Buffer에 저장.(작업자 수정 되었다는 의미)
				//※저장 시, 마다 Buffer에 저장할 경우 이전과 차이 Data가 0으로 변경 되는 문제.
				double dRealData = g_VATCommonData.m_stVATInfo.stHandBasePos.getBasePos(nHand, nPart, nAxis);
				double dGuiData = m_stVATInfoGui.stHandBasePos.getBasePos(nHand, nPart, nAxis);
				if (dRealData != dGuiData) {
					// Copy 순서 변경하지 말것. [9/24/2025 dohyeong.kim]
					//1. [REAL -> BUFFER]
					g_VATCommonData.m_stVATInfoBuff.stHandBasePos.setBasePos(nHand, nPart, nAxis, dRealData);

					//2. [GUI -> REAL]
					g_VATCommonData.m_stVATInfo.stHandBasePos.setBasePos(nHand, nPart, nAxis, dGuiData);

					//3. [ini File 저장]
					g_VATCommonData.SaveEachVisionAutoTeachingInformation(eVisionAutoTeaching_HandBasePos, nHand, nPart, nAxis);
				}
			}
		}
	}

	DisplaySSData(_eVatSS_HandBasePos, m_stHandBasePosLastInfo.nHand, m_nSS1_DisplayType);

	//Spread Sheet 초기화.
	_Com_Clear_SpreadSheet(m_ssHandBasePos, m_stHandBasePosLastInfo);

	//Current Vision Target 초기화.
	DisplayCurVisionTarget(m_stHandBasePosLastInfo.nHand, -1);

	//Hand의 Part가 변경 되었기 때문에(Part == -1) Part Spread Sheet도 초기화.[숨김 처리]
	ButtonClickPartChange();

	//Y Check List Refresh Status
	RefreshYAxisCheckListStatus();
}

//===================================================================================================
//[Part Base Position] 관련 함수.
//기존 Spread Sheet는 Hand X,Y,Z에 해당하는 사항 이라, Part Base Position이랑은 성향이 다름.
//Spread Sheet 관련 동작 모두 신규 작성.

//Hand Base Position의 Part Click시, 변경 사항.
void CVATMainTab_VisionAutoTeachingSetting::ButtonClickPartChange()
{
	int nHand = m_stHandBasePosLastInfo.nHand;
	int nSSPart = m_stHandBasePosLastInfo.nRow - 1; //Hand Base Position에 해당 하는 Part Index

	if (nSSPart < 0) {
		nSSPart = VAT_PART_NONE;
	}
	
    //해당 Part의 Teaching Position Spread Sheet 초기화.
	InitPosSpreadSheet_PartOnly(nHand, nSSPart);

	//Spread Sheet Col/Row 초기화.
	_Com_Clear_SpreadSheet(m_ssPartBasePos, m_stPartBasePosLastInfo);

	//Display Data
	DisplaySSData(_eVatSS_PartBasePos, nHand, m_nSS2_DisplayType);
}


void CVATMainTab_VisionAutoTeachingSetting::ClickSpreadVatPartBasePosition(long Col, long Row)
{
	if (Col == 0 || Row == 0)
		return;

	//Cell Color로 Disable 확인.
	m_ssPartBasePos.SetCol(Col);
	m_ssPartBasePos.SetRow(Row);
	unsigned long color = m_ssPartBasePos.GetBackColor();
	if (color == DISABLE_CELL_COLOR) {
		return;
	}

	_Com_UpdateSheetCellColor(m_ssPartBasePos, m_stPartBasePosLastInfo, Col, Row);
	_Com_UpdateLastSheetData(m_stPartBasePosLastInfo, Col, Row);
}


void CVATMainTab_VisionAutoTeachingSetting::DblClickSpreadVatPartBasePosition(long Col, long Row)
{
	if (_Com_SSInterlock_DblClick(Col, Row, m_nSS2_DisplayType) != eVAT_INTERLOCK_PASS)
		return;

	//_Com_DblClickSheetTextChange 함수 사용 못함 - Hand X,Y,Z에 대해 Limit값 표기함.
	if (Col == 0 || Row == 0)
		return;

	//Cell Color로 Disable 확인.
	m_ssPartBasePos.SetCol(Col);
	m_ssPartBasePos.SetRow(Row);
	unsigned long color = m_ssPartBasePos.GetBackColor();
	if (color == DISABLE_CELL_COLOR) {
		return;
	}



	//Positive, Negative Limit 표기.
	char strPosSWLimit[128];
	char strNegSWLimit[128];

	int nHand = m_stHandBasePosLastInfo.nHand;
	int nSSPart = m_stHandBasePosLastInfo.nRow - 1;
	int nVATPartIdx = ConvSSPartToVATPartIdx(nHand, nSSPart);

	ST_VAT_PART_AXIS_INFO stPartAxisInfo;
	auto it = cPartAxisParamMap.find(nVATPartIdx);
	if (it != cPartAxisParamMap.end()) {
		stPartAxisInfo = it->second;
	}
	else {
		ASSERT(0);
	}

	int nAxis = Col - 1;
	int nPhysicalNo = (nAxis == VAT_PART_Y) ? stPartAxisInfo.axis_no_1 : stPartAxisInfo.axis_no_2;

	double dPosSwLimit = g_Motor[nPhysicalNo]->GetSwLimitPosi();
	double dNegSwLimit = g_Motor[nPhysicalNo]->GetSwLimitNega();

	if(nAxis == VAT_PART_Y){
		dPosSwLimit /= 1000.0;
		dNegSwLimit /= 1000.0;
	}

	sprintf(strPosSWLimit, "%.3f", dPosSwLimit);
	sprintf(strNegSWLimit, "%.3f", dNegSwLimit);

	char szInputValue[VAT_STRING_SIZE] = { 0, };
	char szCurrentValue[VAT_STRING_SIZE] = { 0, };

	CString strCurrentValue = m_ssPartBasePos.GetTextMatrix(Row, Col);
	sprintf_s(szCurrentValue, sizeof(szCurrentValue), "%s", strCurrentValue.GetString());

	int nRet = g_LibCommCtrl.GetNumberBox(this, szInputValue, 15, szCurrentValue, "Target Pos", strPosSWLimit, strNegSWLimit);
	if (nRet != TRUE)
		return;

	m_ssPartBasePos.SetTextMatrix(Row, Col, szInputValue);

	//Limit Check 완료 된 상태.
	m_ssPartBasePos.SetForeColor(NORMAL_COLOR_BLACK);

	//GUI Data에 저장.
	//Rotate도 um방식으로 저장.
	double dPos = atof(szInputValue) *1000.0;
	int nPartPos = Row - 1;
	m_stVATInfoGui.stPartBasePos.setBasePos(nVATPartIdx, nPartPos, nAxis, dPos);
}


//VAT Mode의 Part Base Pos(Spread Sheet2)전용 함수.
void CVATMainTab_VisionAutoTeachingSetting::InitPosSpreadSheet_PartOnly(int nHand, int nSSPart)
{
	//해당 Spread Sheet Cell위로 바로 Focus됨.
	m_ssPartBasePos.SetMoveActiveOnFocus(TRUE);

	m_ssPartBasePos.SetFontName(_T("Arial"));
	m_ssPartBasePos.SetFontBold(TRUE);
	m_ssPartBasePos.SetFontSize(10);
	m_ssPartBasePos.SetTypeHAlign(2); // Center
	m_ssPartBasePos.SetTypeVAlign(2); // Center

	int nMaxRow = 0; //Spread Sheet에 아무것도 표기하지 않음.(default)
	int nMaxCol = 0; //Spread Sheet에 아무것도 표기하지 않음.(default)
	int nVATPartIdx = ConvSSPartToVATPartIdx(nHand, nSSPart);
	if (nVATPartIdx == VAT_PART_FEEDER) {
		//[Feeder Test 필요함]
 		nMaxRow = VAT_MAX_FEEDER_LOC_COUNT;
  		nMaxCol = VAT_MAX_AXIS_Y;
	}
	else if (VAT_PART_TABLE_1 <= nVATPartIdx && nVATPartIdx <= VAT_PART_TABLE_4) {
		nMaxRow = VAT_MAX_TBL_LOC_COUNT;
		nMaxCol = VAT_MAX_AXIS_YROTATE;
	}

	m_ssPartBasePos.SetMaxRows(nMaxRow);
	m_ssPartBasePos.SetMaxCols(nMaxCol);

	//첫 번째 Cell 공란.
	m_ssPartBasePos.SetTextMatrix(0, 0, _T(""));

	//Spread Sheet 아무것도 표기하지 않음.
	if (nMaxRow == 0 && nMaxCol == 0) {
		//m_ssPartBasePos.EnableWindow(FALSE);	
		m_ssPartBasePos.SetRowHeight(0, 0);
		m_ssPartBasePos.SetColWidth(0, 0);
		return;
	}


// 	m_ssPartBasePos.SetCol(0);
// 	m_ssPartBasePos.SetRow(0);
// 	m_ssPartBasePos.SetBackColor(DISABLE_BACK_COLOR);

	//Col Title Setting(Axis)
	const char* szDirection[VAT_MAX_AXIS_YROTATE];
	if (nVATPartIdx == VAT_PART_FEEDER) {
		//[Feeder Test 필요함]
  		szDirection[0] = "Y(mm)";
  		szDirection[1] = "";
	}
	else {
		szDirection[0] = "Y(mm)";
		szDirection[1] = "Rotate(deg)";
	}


	for (int i = 0; i < m_ssPartBasePos.GetMaxCols(); i++)
	{
		m_ssPartBasePos.SetTextMatrix(0, i + 1, szDirection[i]);
	}

	//Row Title Setting(Hand Name)
	int ntest = m_ssPartBasePos.GetMaxRows();
	m_ssPartBasePos.SetScrollBarShowMax(TRUE);
	for (int i = 0; i < m_ssPartBasePos.GetMaxRows(); i++)
	{
		m_ssPartBasePos.SetTextMatrix(i + 1, 0, ConvVATPartIdxToName(nVATPartIdx, i));
	}

	//Spread Sheet 가로 폭 조절.
	m_ssPartBasePos.SetColWidth(0, 15);
	for (int i = 1; i <= m_ssPartBasePos.GetMaxCols(); i++) {
		m_ssPartBasePos.SetColWidth(i, 10);
	}

	//Spread Sheet 세로 높이 조절.
	for (int i = 0; i <= m_ssPartBasePos.GetMaxRows(); i++) {
		m_ssPartBasePos.SetRowHeight(i, 20);
	}

	//Spread Sheet 변경 때 마다 Top Row를 첫번째 열로 고정.
	//spread sheet 좌,우 움직이지 않도록 마지막 Col 고정.
	m_ssPartBasePos.SetTopRow(0);
}


//=====================================================================================
//COMMENT : Hand Spread Sheet의 Part Index를 VAT에서 사용하는 Part Index로 변환.
//			VAT_PART_FEEDER =0,
//			VAT_PART_TABLE_1, 
//			VAT_PART_TABLE_2,
//			VAT_PART_TABLE_3, 
//			VAT_PART_TABLE_4,
//			VAT_PART_NONE = -1,
//=====================================================================================
int CVATMainTab_VisionAutoTeachingSetting::ConvSSPartToVATPartIdx(int nHand, int nSSPart)
{
	int nRet = VAT_PART_NONE;
	switch (nHand)
	{
	case VAT_HAND_LOAD_TRAYPP:
	{
		if (nSSPart == VAT_IDX_TRAY_PP_TBL_2)			nRet = VAT_PART_TABLE_2;
		else if (nSSPart == VAT_IDX_TRAY_PP_TBL_3)	nRet = VAT_PART_TABLE_3;
		//[Feeder Test 필요함]
		else if (nSSPart == VAT_IDX_TRAY_PP_FEEDER)	nRet = VAT_PART_FEEDER;
	}break;
	case VAT_HAND_UNLOAD_TRAYPP:
	{
		if (nSSPart == VAT_IDX_TRAY_PP_TBL_1)			nRet = VAT_PART_TABLE_1;
		else if (nSSPart == VAT_IDX_TRAY_PP_TBL_4)	nRet = VAT_PART_TABLE_4;
	}break;
	case VAT_HAND_LOAD_TESTPP1:
	case VAT_HAND_UNLOAD_TESTPP1:
	{
		if (VAT_IDX_TEST_PP_TBL_1_LD <= nSSPart && nSSPart <= VAT_IDX_TEST_PP_TBL_4_LD) {
			nRet = VAT_PART_TABLE_2;
		}
		else if (VAT_IDX_TEST_PP_TBL_1_UD <= nSSPart && nSSPart <= VAT_IDX_TEST_PP_TBL_4_UD) {
			nRet = VAT_PART_TABLE_1;
		}
	}break;
	case VAT_HAND_LOAD_TESTPP2:
	case VAT_HAND_UNLOAD_TESTPP2:
	{
		if (VAT_IDX_TEST_PP_TBL_1_LD <= nSSPart && nSSPart <= VAT_IDX_TEST_PP_TBL_4_LD) {
			nRet = VAT_PART_TABLE_3;
		}
		else if (VAT_IDX_TEST_PP_TBL_1_UD <= nSSPart && nSSPart <= VAT_IDX_TEST_PP_TBL_4_UD) {
			nRet = VAT_PART_TABLE_4;
		}
	}break;
	default:
		break;
	}

	return nRet;
}


CString CVATMainTab_VisionAutoTeachingSetting::ConvVATPartIdxToName(int nVATPart, int nPosIdx)
{
	CString strRet = _T("");

	if (nVATPart == VAT_PART_FEEDER)
	{
		//[Feeder Test 필요함]
// 		CString strFeeder[VAT_MAX_FEEDER_LOC_COUNT] = {
//  			"C-Tray Jig",//"Load Pick Start",
//  			"Plate Jig",//"Load Pick",
//  			"Load Place",
// 		};
		CString strFeeder[VAT_MAX_FEEDER_LOC_COUNT] = {
			"C-Tray Jig"
		};
 		strRet = strFeeder[nPosIdx];
	}
	else if (VAT_PART_TABLE_1 <= nVATPart && nVATPart <= VAT_PART_TABLE_4)
	{
		//Table 4개
		CString strTable[4][VAT_MAX_TBL_LOC_COUNT] = {
			{ "Unload Side", "Test Side 1", "Test Side 2", "Test Side 3", "Test Side 4" },
			{ "Load Side",	 "Test Side 1", "Test Side 2", "Test Side 3", "Test Side 4" },
			{ "Load Side",	 "Test Side 5", "Test Side 6", "Test Side 7", "Test Side 8" },
			{ "Unload Side", "Test Side 5",	"Test Side 6", "Test Side 7", "Test Side 8" },
		};

		strRet = strTable[nVATPart - VAT_PART_TABLE_1][nPosIdx];
	}

	return strRet;
}


BOOL CVATMainTab_VisionAutoTeachingSetting::PartBasePositionLimitCheck(int nVATPart, int nAxis, double dPos)
{
	//Pos/Neg Limit 확인 및 색상 변경.
	ST_VAT_PART_AXIS_INFO stPartAxisInfo;
	auto it = cPartAxisParamMap.find(nVATPart);
	if (it != cPartAxisParamMap.end()) {
		stPartAxisInfo = it->second;
	}
	else {
		ASSERT(0);
	}

	int nPhysicalNo = (nAxis == VAT_PART_Y) ? stPartAxisInfo.axis_no_1 : stPartAxisInfo.axis_no_2;

	if (nPhysicalNo != -1)
	{
		double dPosSwLimit = g_Motor[nPhysicalNo]->GetSwLimitPosi();
		double dNegSwLimit = g_Motor[nPhysicalNo]->GetSwLimitNega();

		if (nAxis == VAT_PART_ROTATE) {
			//Rotate의 경우 Limit값 um가 아닌 deg라서 그대로 저장 됨.
			//실제 Data는 um방식으로 저장 되기 때문에 Limit에 1000.0을 곱해줌.
			dPosSwLimit *= 1000.0;
			dNegSwLimit *= 1000.0;
		}

		if (dPos > dPosSwLimit || dPos < dNegSwLimit) {
			return TRUE;
		}
	}

	return FALSE;
}


//=======================================================================================
// COMMENT : Teaching 편의성을 위해, 각 Part에 해당 하는 Base Position만 Enable 하기 위해,
//           Part Base Position 유효성 검사 함수.
//=======================================================================================
BOOL CVATMainTab_VisionAutoTeachingSetting::ChkPartBasePosValid(int nHand, int nSSPart, int nPartBasePos)
{
 	int nRet = FALSE;
 
 	switch (nHand)
 	{
 		case VAT_HAND_LOAD_TRAYPP:
		case VAT_HAND_UNLOAD_TRAYPP:
 		{
 			if (VAT_IDX_TRAY_PP_TBL_1 <= nSSPart && nSSPart <= VAT_IDX_TRAY_PP_TBL_4) {
 				if (nPartBasePos == VAT_TBL_LOAD_SIDE) {
 					nRet = TRUE;
 				}
 			}
 			else if (nSSPart = VAT_IDX_TRAY_PP_FEEDER) {
				nRet = TRUE; // Feeder는 모든 Part Base Position 사용.
 			}
 		}break;
		case VAT_HAND_LOAD_TESTPP1: case VAT_HAND_UNLOAD_TESTPP1:
		case VAT_HAND_LOAD_TESTPP2: case VAT_HAND_UNLOAD_TESTPP2:
		{
			if (((nSSPart == VAT_IDX_TEST_PP_TBL_1_LD || nSSPart == VAT_IDX_TEST_PP_TBL_1_UD) && nPartBasePos == VAT_TBL_TEST_SIDE_1) ||
				((nSSPart == VAT_IDX_TEST_PP_TBL_2_LD || nSSPart == VAT_IDX_TEST_PP_TBL_2_UD) && nPartBasePos == VAT_TBL_TEST_SIDE_2) ||
				((nSSPart == VAT_IDX_TEST_PP_TBL_3_LD || nSSPart == VAT_IDX_TEST_PP_TBL_3_UD) && nPartBasePos == VAT_TBL_TEST_SIDE_3) ||
				((nSSPart == VAT_IDX_TEST_PP_TBL_4_LD || nSSPart == VAT_IDX_TEST_PP_TBL_4_UD) && nPartBasePos == VAT_TBL_TEST_SIDE_4)) {
				nRet = TRUE;
			}
		}break;
 		default:
			break;
 	}

	return nRet;
}


void CVATMainTab_VisionAutoTeachingSetting::ClickSspanelVatPartBasePosData()
{
	m_nSS2_DisplayType = eVAT_SSDISPLAYTYPE_DATA;

	//sspanel color 변경.
	_Com_ChangeSSPanelColor(m_ssDataPanel_2_Sub1, m_ssDataPanel_2_Sub2, m_nSS2_DisplayType);

	//Display Data
	//Hand는 BasePosLastInfo에서 확인.(m_stPartBasePosLastInfo에는 Hand 저장하지 않음.)
	//m_stPartBasePosLastInfo는 Hand 관련 Part가 아닌 PART_ONLY Type이기 때문에 Hand와 관련 없음.
	DisplaySSData(_eVatSS_PartBasePos, m_stHandBasePosLastInfo.nHand, m_nSS2_DisplayType);
}


void CVATMainTab_VisionAutoTeachingSetting::ClickSspanelVatPartBasePosDataDiff()
{
	m_nSS2_DisplayType = eVAT_SSDISPLAYTYPE_DIFFERENCE;

	//sspanel color 변경.
	_Com_ChangeSSPanelColor(m_ssDataPanel_2_Sub1, m_ssDataPanel_2_Sub2, m_nSS2_DisplayType);

	//Display Data
	//Hand는 BasePosLastInfo에서 확인.(m_stPartBasePosLastInfo에는 Hand 저장하지 않음.)
	//m_stPartBasePosLastInfo는 Hand 관련 Part가 아닌 PART_ONLY Type이기 때문에 Hand와 관련 없음.
	DisplaySSData(_eVatSS_PartBasePos, m_stHandBasePosLastInfo.nHand, m_nSS2_DisplayType);
}


void CVATMainTab_VisionAutoTeachingSetting::OnBnClickedBtnVatPartBasePosEncoderRead()
{
	//[Interlock]
	_eVAT_INTERLOCK bChkInterlock = _Com_SSInterlock_AlarmDeActivate();
	if (bChkInterlock != eVAT_INTERLOCK_PASS)
		return;

	//"Do you want to read encoder?"
	int nRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_ENCODER_READ_PART_ONLY, ERR_VAT_TYPE_YESNO);
	if (nRet == IDNO)
		return;

	//[Interlock 1] Data Show Type 확인.
	if (m_nSS2_DisplayType != eVAT_SSDISPLAYTYPE_DATA){
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_VAT_INTERLOCK_SELECT_RIGHT_SPREAD_SHEET_TYPE, AlarmData, NULL);
		return;
	}

	//[Interlock 2] Spread Sheet Unselect 확인.
	if (m_stPartBasePosLastInfo.nCol == -1 || m_stPartBasePosLastInfo.nRow == -1) {
		CParamList AlarmData;
		g_Error.AlarmReport(ERR_VAT_INTERLOCK_UNSELECT_SPREAD_SHEET, AlarmData, NULL);
		return;
	}
	
	//Get Physical Index
	int nHand = m_stHandBasePosLastInfo.nHand;
	int nSSPart = m_stHandBasePosLastInfo.nRow - 1;
	int nVATPartIdx = ConvSSPartToVATPartIdx(nHand, nSSPart);
	if (nVATPartIdx == VAT_PART_NONE)
		return;

	ST_VAT_PART_AXIS_INFO stPartAxisInfo;
	auto it = cPartAxisParamMap.find(nVATPartIdx);
	if (it != cPartAxisParamMap.end()) {
		stPartAxisInfo = it->second;
	}
	else {
		ASSERT(0);
	}

	//[Feeder Test 필요함]
	int nMaxAxis = (nVATPartIdx == VAT_PART_FEEDER) ? VAT_MAX_AXIS_Y : VAT_MAX_AXIS_YROTATE;


	//[Interlock 3] Check Part Origin
	for (int nAxis = 0; nAxis < nMaxAxis; nAxis++)
	{
		int nPhysicalNo = (nAxis == VAT_PART_Y) ? stPartAxisInfo.axis_no_1 : stPartAxisInfo.axis_no_2;

		BOOL bIsOrgComplete = g_Motor[nPhysicalNo]->IsOrgCompleted(nullptr);
		if (bIsOrgComplete != TRUE)
		{
			CString strPart = _T("");
			(nVATPartIdx == VAT_PART_FEEDER) ? strPart.Format("FEEDER") : strPart.Format("TABLE %d", nVATPartIdx);
			
			CParamList AlarmData;
			AlarmData.m_fnAddItemFormat("[%s][%s] Motor Not Origin Check.", strPart, ((nAxis == VAT_PART_Y) ? _T("Y AXIS") : _T("Rotate Axis")));
			g_Error.AlarmReport(ERR_VAT_NOT_ORIGIN_CHECK, AlarmData, NULL);
			return;
		}
	}

	//Encoder Read
	for (int nAxis = 0; nAxis < nMaxAxis; nAxis++)
	{
		int nPhysicalNo = (nAxis == VAT_PART_Y) ? stPartAxisInfo.axis_no_1 : stPartAxisInfo.axis_no_2;
		double dCurPos = 0.0;
		g_Motor[nPhysicalNo]->GetCurrentPos(dCurPos);

		//Spread Sheet 표기.
		CString strCurPos = _T("");
		strCurPos.Format("%.3f", (nAxis == VAT_PART_Y) ? dCurPos / 1000.0 : dCurPos);
		int nCol = nAxis + 1;
		m_ssPartBasePos.SetTextMatrix(m_stPartBasePosLastInfo.nRow, nCol, strCurPos);

		//Data Update.
		int nPartPos = m_stPartBasePosLastInfo.nRow - 1;
		m_stVATInfoGui.stPartBasePos.setBasePos(nVATPartIdx, nPartPos, nAxis, dCurPos);
	}
}


void CVATMainTab_VisionAutoTeachingSetting::OnBnClickedBtnVatPartBasePosMotorMove()
{
	
}


void CVATMainTab_VisionAutoTeachingSetting::OnBnClickedBtnVatPartBasePosReload()
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
	m_stVATInfoGui.stPartBasePos = g_VATCommonData.m_stVATInfo.stPartBasePos;

	DisplaySSData(_eVatSS_PartBasePos, m_stHandBasePosLastInfo.nHand, m_nSS2_DisplayType);

	_Com_Clear_SpreadSheet(m_ssPartBasePos, m_stPartBasePosLastInfo);
}


void CVATMainTab_VisionAutoTeachingSetting::OnBnClickedBtnVatPartBasePosDataSave()
{
	//Alarm 비활성화 상태 Check
	_eVAT_INTERLOCK bChkInterlock = _Com_SSInterlock_AlarmDeActivate();
	if (bChkInterlock != eVAT_INTERLOCK_PASS)
		return;

	//"Do you want to Base Position Save?"
	int nRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_DATA_SAVE, ERR_VAT_TYPE_YESNO);
	if (nRet == IDNO)
		return;

	BOOL IsLimitPosExist = CheckPosLimit(_eVatSS_PartBasePos);
	if (IsLimitPosExist) {
		int nRet = g_Error.RequestUserSelectionModal(ERR_VAT_CONFIRM_LIMIT_OVER_CHECK, ERR_VAT_TYPE_YESNO);
		if (nRet == IDNO) {
			return;
		}
	}

	for (int nVATPartIdx = 0; nVATPartIdx < VAT_PART_MAX; nVATPartIdx++) {
		int nMaxPartPos = (nVATPartIdx == VAT_PART_FEEDER) ? VAT_MAX_FEEDER_LOC_COUNT : VAT_MAX_TBL_LOC_COUNT;
		int nMaxAxis = (nVATPartIdx == VAT_PART_FEEDER) ? VAT_MAX_AXIS_Y : VAT_MAX_AXIS_YROTATE;

		for (int nPartPos = 0; nPartPos < nMaxPartPos; nPartPos++) {
			for (int nAxis = 0; nAxis < nMaxAxis; nAxis++) {
				//GUI <-> Real 다른 값만 Buffer에 저장.(작업자 수정 되었다는 의미)
				//※저장 시, 마다 Buffer에 저장할 경우 이전과 차이 Data가 0으로 변경 되는 문제.
				double dRealData = g_VATCommonData.m_stVATInfo.stPartBasePos.getBasePos(nVATPartIdx, nPartPos, nAxis);
				double dGuiData = m_stVATInfoGui.stPartBasePos.getBasePos(nVATPartIdx, nPartPos, nAxis);
				if (dRealData != dGuiData) {
					// Copy 순서 변경하지 말것. [9/24/2025 dohyeong.kim]
					//1. [REAL -> BUFFER]
					g_VATCommonData.m_stVATInfoBuff.stPartBasePos.setBasePos(nVATPartIdx, nPartPos, nAxis, dRealData);

					//2. [GUI -> REAL]
					g_VATCommonData.m_stVATInfo.stPartBasePos.setBasePos(nVATPartIdx, nPartPos, nAxis, dGuiData);

					//3. [ini File 저장]
					g_VATCommonData.SaveEachVisionAutoTeachingInformation(eVisionAutoTeaching_PartBasePos, nVATPartIdx, nPartPos, nAxis);
				}
			}
		}
	}

	//Hand는 BasePosLastInfo에서 확인.(m_stPartBasePosLastInfo에는 Hand 저장하지 않음.)
	//m_stPartBasePosLastInfo는 Hand 관련 Part가 아닌 PART_ONLY Type이기 때문에 Hand와 관련 없음.
	DisplaySSData(_eVatSS_PartBasePos, m_stHandBasePosLastInfo.nHand, m_nSS2_DisplayType);

	//Spread Sheet 초기화.
	_Com_Clear_SpreadSheet(m_ssPartBasePos, m_stPartBasePosLastInfo);
}


///////////////////[Test Site Y Axis Check List]	

void CVATMainTab_VisionAutoTeachingSetting::InitSubHandSelectSheet()
{
	for (int nHand = 0; nHand < eYChkListHand_Max; nHand++)
	{
		if(!m_ssSelectSheetSub[nHand].GetSafeHwnd())
			continue;

		m_ssSelectSheetSub[nHand].SetFontName(_T("Arial"));
		m_ssSelectSheetSub[nHand].SetFontSize(10);
		m_ssSelectSheetSub[nHand].SetTypeHAlign(2); // Center Align
		m_ssSelectSheetSub[nHand].SetTypeVAlign(0);
		m_ssSelectSheetSub[nHand].SetCellType(7);
		m_ssSelectSheetSub[nHand].SetTypeButtonType(0);
		m_ssSelectSheetSub[nHand].SetMaxCols(1);
		m_ssSelectSheetSub[nHand].SetMaxRows(1);
		m_ssSelectSheetSub[nHand].SetColHeadersShow(FALSE);
		m_ssSelectSheetSub[nHand].SetRowHeadersShow(FALSE);

 		m_ssSelectSheetSub[nHand].SetColWidth(1, 10);
 		m_ssSelectSheetSub[nHand].SetRowHeight(1, 21);
		m_ssSelectSheetSub[nHand].SetAutoSize(TRUE);

		/*Hand Name Setting*/
		m_ssSelectSheetSub[nHand].SetCol(1);
		m_ssSelectSheetSub[nHand].SetRow(1);

		m_ssSelectSheetSub[nHand].SetTypeButtonText((nHand == eYChkListHand_TestPp1) ? _T("TEST PP1") : _T("TEST PP2"));
		m_ssSelectSheetSub[nHand].SetTypeButtonColor(WWHITE);
	}
}


void CVATMainTab_VisionAutoTeachingSetting::InitSubTabCtrl()
{
	m_tabTestSiteYCheckList.DeleteAllItems();
	m_tabTestSiteYCheckList.InsertItem(MTPI_TEST_SIDE_1, "Site1");
	m_tabTestSiteYCheckList.InsertItem(MTPI_TEST_SIDE_2, "Site2");
	m_tabTestSiteYCheckList.InsertItem(MTPI_TEST_SIDE_3, "Site3");
	m_tabTestSiteYCheckList.InsertItem(MTPI_TEST_SIDE_4, "Site4");

	CFont font;
	font.CreatePointFont(80, _T("Arial"));
	m_tabTestSiteYCheckList.SetFont(&font);

	m_tabTestSiteYCheckList.SetCurSel(MTPI_TEST_SIDE_1);
}


void CVATMainTab_VisionAutoTeachingSetting::InitTestSiteYAxisCheckList()
{
	//Test Pp Hand Select Init.
	InitSubHandSelectSheet();

	//Tab Ctrl Init.
	InitSubTabCtrl();

	//초기 Hand 는 Test PP1로 설정.
	ButtonClickHandChange_YAxisCheckList(eYChkListHand_TestPp1);
}

// int CVATMainTab_VisionAutoTeachingSetting::ConvYCheckListHandToVatHandIdx(int nHand)
// {
// 	int nRet = -1;
// 
// 	switch (nHand)
// 	{
// 	case eYChkListHand_TestPp1: nRet = VAT_HAND_LOAD_TESTPP1; break;
// 	case eYChkListHand_TestPp2: nRet = VAT_HAND_UNLOAD_TESTPP2; break;
// 	default:
//		break;
// 	}
// 
// 	return nRet;
// }


void CVATMainTab_VisionAutoTeachingSetting::ButtonClickHandChange_YAxisCheckList(int nHand)
{
	m_nYCheckListCurHand = nHand;

	for (int i = 0; i < eYChkListHand_Max; i++) {
		m_ssSelectSheetSub[i].SetCol(1);
		m_ssSelectSheetSub[i].SetRow(1);

		if (i == nHand) {
			m_ssSelectSheetSub[i].SetTypeButtonColor(LLIGHTBLUE);
		}
		else {
			m_ssSelectSheetSub[i].SetTypeButtonColor(WWHITE);
		}
	}

	//Refresh Status
	RefreshYAxisCheckListStatus();
}

void CVATMainTab_VisionAutoTeachingSetting::RefreshYAxisCheckListStatus()
{
	m_nYCheckListCurTab = m_tabTestSiteYCheckList.GetCurSel();

	//Change Hand Panel Color & Text
	DisplayYCheckListSSPanel();

	//Change Position Panel Color & Text
	DisplayYCheckListEdit();
}


void CVATMainTab_VisionAutoTeachingSetting::DisplayYCheckListSSPanel()
{
	//모든 Tab Position Align 확인.

	//Test Pp1 Status Check
	BOOL bChkTestPp1Align = FALSE;
	for (int nCurTab = 0; nCurTab <= MTPI_TEST_SIDE_4; nCurTab++) {
		bChkTestPp1Align = IsYAxisAllAlignWithStation(eYChkListHand_TestPp1, nCurTab);
		if(!bChkTestPp1Align)
			break;	
	}

	ULONG ulTestPp1AlignColor = (bChkTestPp1Align) ? GREEN : RED;
	m_ssPanelYCheckList_HandStatus1.SetBackColor(ulTestPp1AlignColor);
	m_ssPanelYCheckList_HandStatus1.SetWindowText((bChkTestPp1Align) ? _T("PASS") : _T("FAIL"));

	//Test Pp2 Status Check
	BOOL bChkTestPp2Align = FALSE;
	for (int nCurTab = 0; nCurTab <= MTPI_TEST_SIDE_4; nCurTab++) {
		bChkTestPp2Align = IsYAxisAllAlignWithStation(eYChkListHand_TestPp2, m_nYCheckListCurTab);
		if(!bChkTestPp2Align)
			break;
	}
	ULONG ulTestPp2AlignColor = (bChkTestPp2Align) ? GREEN : RED;
	m_ssPanelYCheckList_HandStatus2.SetBackColor(ulTestPp2AlignColor);
	m_ssPanelYCheckList_HandStatus2.SetWindowText((bChkTestPp2Align) ? _T("PASS") : _T("FAIL"));
}


void CVATMainTab_VisionAutoTeachingSetting::DisplayYCheckListEdit()
{
	ASSERT(MTPI_TEST_SIDE_1 <= m_nYCheckListCurTab && m_nYCheckListCurTab <= MTPI_TEST_SIDE_4);
	ASSERT(eYChkListHand_TestPp1 <= m_nYCheckListCurHand && m_nYCheckListCurHand <= eYChkListHand_TestPp2);

	//GUI 재정렬
	m_tabTestSiteYCheckList.BringWindowToTop();
	m_ssPanelYCheckList_Bmp.BringWindowToTop();
	m_ssPanelYCheckList_Status1.BringWindowToTop(); 
	m_ssPanelYCheckList_Status2.BringWindowToTop();
	m_ssPanelYCheckList_Status3.BringWindowToTop();

	//VAT Hand Idx로 변환.
	int nLDHandIdx = (m_nYCheckListCurHand == eYChkListHand_TestPp1) ? VAT_HAND_LOAD_TESTPP1 : VAT_HAND_LOAD_TESTPP2;
	int nUDHandIdx = (m_nYCheckListCurHand == eYChkListHand_TestPp1) ? VAT_HAND_UNLOAD_TESTPP1 : VAT_HAND_UNLOAD_TESTPP2;

	//[UD Hand] Station Pos
	//기준이 되는 Position	
	int nStationIdx = VAT_IDX_STATION_1 + m_nYCheckListCurTab;
	double dStationPos = g_VATCommonData.m_stVATInfo.stHandBasePos.getBasePos(nUDHandIdx, nStationIdx, VAT_Y);
	CString strStationPos = _T("");
	strStationPos.Format("%.2f", dStationPos / 1000.0);
	m_ssPanelYCheckList_Status1.SetBackColor(GREEN);
	m_ssPanelYCheckList_Status1.SetWindowText(strStationPos);


	//[UD Hand] Unload Table Pos
	int nUDTableIdx = VAT_IDX_TEST_PP_TBL_1_UD + m_nYCheckListCurTab;
	double dUDTablePos = g_VATCommonData.m_stVATInfo.stHandBasePos.getBasePos(nUDHandIdx, nUDTableIdx, VAT_Y);

	ULONG ulUDPoscolor = (dStationPos == dUDTablePos) ? GREEN : RED;

	CString strUDPos = _T("");
	strUDPos.Format("%.2f", dUDTablePos / 1000.0);
	m_ssPanelYCheckList_Status3.SetBackColor(ulUDPoscolor);
	m_ssPanelYCheckList_Status3.SetWindowText(strUDPos);


	//[LD Hand] Load Table Pos
	int nLDTableIdx = VAT_IDX_TEST_PP_TBL_1_LD + m_nYCheckListCurTab;
	double dLDTablePos = g_VATCommonData.m_stVATInfo.stHandBasePos.getBasePos(nLDHandIdx, nLDTableIdx, VAT_Y);

	ULONG ulLDPoscolor = (dStationPos == dLDTablePos) ? GREEN : RED;

	CString strLDPos = _T("");
	strLDPos.Format("%.2f", dLDTablePos / 1000.0);
	m_ssPanelYCheckList_Status2.SetBackColor(ulLDPoscolor);
	m_ssPanelYCheckList_Status2.SetWindowText(strLDPos);
}


//========================================================================
// COMMENT : 해당 Site의 Test Pp Y축 Align이 동일한지 확인하는 함수.
//           3가지 Y축값 확인.
//				-. [UD Hand] Station Pos
//				-. [LD Hand] Load Table Pos
//				-. [UD Hand] Unload Table Pos
//========================================================================
BOOL CVATMainTab_VisionAutoTeachingSetting::IsYAxisAllAlignWithStation(int nHand, int nSite)
{
	ASSERT(eYChkListHand_TestPp1 <= nHand && nHand <= eYChkListHand_TestPp2);
	ASSERT(MTPI_TEST_SIDE_1 <= nSite && nSite <= MTPI_TEST_SIDE_4);

	//VAT Hand Idx로 변환.
	int nLDHandIdx = (nHand == eYChkListHand_TestPp1) ? VAT_HAND_LOAD_TESTPP1 : VAT_HAND_LOAD_TESTPP2;
	int nUDHandIdx = (nHand == eYChkListHand_TestPp1) ? VAT_HAND_UNLOAD_TESTPP1 : VAT_HAND_UNLOAD_TESTPP2;

	//[UD Hand] Station Pos
	int nStationIdx = VAT_IDX_STATION_1 + nSite;
	double dStationPos = g_VATCommonData.m_stVATInfo.stHandBasePos.getBasePos(nUDHandIdx, nStationIdx, VAT_Y);

	//[LD Hand] Load Table Pos
	int nLDTableIdx = VAT_IDX_TEST_PP_TBL_1_LD + nSite;
	double dLDTablePos = g_VATCommonData.m_stVATInfo.stHandBasePos.getBasePos(nLDHandIdx, nLDTableIdx, VAT_Y);

	//[UD Hand] Unload Table Pos
	int nUDTableIdx = VAT_IDX_TEST_PP_TBL_1_UD + nSite;
	double dUDTablePos = g_VATCommonData.m_stVATInfo.stHandBasePos.getBasePos(nUDHandIdx, nUDTableIdx, VAT_Y);

	if (dStationPos == dLDTablePos && dStationPos == dUDTablePos) {
		return TRUE;
	}

	return FALSE;
}


void CVATMainTab_VisionAutoTeachingSetting::ButtonClickedSpreadTestSiteYCheckHand1(long Col, long Row, short ButtonDown)
{
	ButtonClickHandChange_YAxisCheckList(eYChkListHand_TestPp1);
}


void CVATMainTab_VisionAutoTeachingSetting::ButtonClickedSpreadTestSiteYCheckHand2(long Col, long Row, short ButtonDown)
{
	ButtonClickHandChange_YAxisCheckList(eYChkListHand_TestPp2);
}

void CVATMainTab_VisionAutoTeachingSetting::OnTcnSelchangeTabVatTestSiteYCheck(NMHDR *pNMHDR, LRESULT *pResult)
{
	//EditBox만 업데이트 진행.

	//Get Current Tab Idx
	m_nYCheckListCurTab = m_tabTestSiteYCheckList.GetCurSel();

	DisplayYCheckListEdit();


	//Refresh Status
	//RefreshYAxisCheckListStatus();
}


void CVATMainTab_VisionAutoTeachingSetting::ClickSspanelYCheckListStatus1()
{
	ClickSspanelYCheckListStatus(eYCheckList_Station);
}


void CVATMainTab_VisionAutoTeachingSetting::ClickSspanelYCheckListStatus2()
{
	ClickSspanelYCheckListStatus(eYCheckList_LDTable);
}


void CVATMainTab_VisionAutoTeachingSetting::ClickSspanelYCheckListStatus3()
{
	ClickSspanelYCheckListStatus(eYCheckList_UDTable);
}


//=====================================================================================
//COMMENT : Click한 Pannel의 Position으로 Hand Select 및 Hand Base Position Display 
//=====================================================================================
void CVATMainTab_VisionAutoTeachingSetting::ClickSspanelYCheckListStatus(int nYCheckListPart)
{
	if (!(eYChkListHand_TestPp1 <= m_nYCheckListCurHand && m_nYCheckListCurHand <= eYChkListHand_TestPp2) ||
		!(MTPI_TEST_SIDE_1 <= m_nYCheckListCurTab && m_nYCheckListCurTab <= MTPI_TEST_SIDE_4) ||
		!(eYCheckList_Station <= nYCheckListPart && nYCheckListPart <= eYCheckList_UDTable))
		return;

	//Hand Select Idx
	//Hand Spread Sheet Col/Row
	int nHand = -1;
 	int nCol = VAT_Y + 1;
 	int nRow = -1;

 	switch (nYCheckListPart)
 	{
 		case eYCheckList_Station: //[Unload Hand]	
		{
			nHand = (m_nYCheckListCurHand == eYChkListHand_TestPp1) ? VAT_HAND_UNLOAD_TESTPP1 : VAT_HAND_UNLOAD_TESTPP2;
			nRow = (VAT_IDX_STATION_1 + m_nYCheckListCurTab) + 1;
		}break;
 		case eYCheckList_LDTable: //[Load Hand]		
		{
			nHand = (m_nYCheckListCurHand == eYChkListHand_TestPp1) ? VAT_HAND_LOAD_TESTPP1 : VAT_HAND_LOAD_TESTPP2;
			nRow = (VAT_IDX_TEST_PP_TBL_1_LD + m_nYCheckListCurTab) + 1;
		}break;
 		case eYCheckList_UDTable: //[Unload Hand]	
		{
			nHand = (m_nYCheckListCurHand == eYChkListHand_TestPp1) ? VAT_HAND_UNLOAD_TESTPP1 : VAT_HAND_UNLOAD_TESTPP2;
			nRow = (VAT_IDX_TEST_PP_TBL_1_UD + m_nYCheckListCurTab) + 1;
		}break;
 	}


	ButtonClickHandChange(1, 1, nHand); //Hand Select Sheet Col/Row -> 1

	ClickSpreadHandBasePosition(nCol, nRow); //Base Position Sheet Select
}


//=====================================================================================
//COMMENT : Hand Base Position Click 시, 해당 Part의 Vision Target ON
//=====================================================================================
void CVATMainTab_VisionAutoTeachingSetting::DisplayCurVisionTarget(int nHand, int nSSPart)
{
	CDSSPanel* chkbt = NULL;

	//[1. 전체 Part Clear]
	//[Tray Pp]
	for (int nPart = 0; nPart < VAT_MAX_TRAY_PP_LOC_COUNT; nPart++) {
		if (m4_nTargetOverviewIdx_TrayPp[nPart] != _VAT_ID_SPARE) {
			chkbt = (CDSSPanel*)GetDlgItem(m4_nTargetOverviewIdx_TrayPp[nPart]);
			chkbt->Off();
			chkbt->ShowWindow(FALSE);
		}
	}

	//[Test Pp]
	for (int nPart = 0; nPart < VAT_MAX_TEST_PP_LOC_COUNT; nPart++) {
		chkbt = (CDSSPanel*)GetDlgItem(m4_nTargetOverviewIdx_TestPp1[nPart]);
		chkbt->Off();
		chkbt->ShowWindow(FALSE);

		chkbt = (CDSSPanel*)GetDlgItem(m4_nTargetOverviewIdx_TestPp2[nPart]);
		chkbt->Off();
		chkbt->ShowWindow(FALSE);
	}

	//[2. Interlock]
	if (nHand < 0 || nSSPart < 0) {
		//Select 없을 경우, 초기화만 하고 return
		return;
	}

	//[3. Select Part ON]
	switch (nHand)
	{
		case VAT_HAND_LOAD_TRAYPP:
		case VAT_HAND_UNLOAD_TRAYPP:
		{
			chkbt = (CDSSPanel*)GetDlgItem(m4_nTargetOverviewIdx_TrayPp[nSSPart]);
		}break;
		case VAT_HAND_LOAD_TESTPP1:	
		case VAT_HAND_UNLOAD_TESTPP1:
		{
			chkbt = (CDSSPanel*)GetDlgItem(m4_nTargetOverviewIdx_TestPp1[nSSPart]);
		}break;
		case VAT_HAND_LOAD_TESTPP2:	
		case VAT_HAND_UNLOAD_TESTPP2:
		{
			chkbt = (CDSSPanel*)GetDlgItem(m4_nTargetOverviewIdx_TestPp2[nSSPart]);
		}break;
		default:
			break;
	}

	chkbt->On();
	chkbt->ShowWindow(TRUE);
}


