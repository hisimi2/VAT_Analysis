// VATMain_PickerAlign.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "VATMain_PickerAlign.h"
#include "afxdialogex.h"

// static int m_nPositionControl[VAT_HAND_MAX] = { IDC_EDIT_PICKER_ALIGN_LOAD_TRAY_PP_STATUS, IDC_EDIT_PICKER_ALIGN_UNLOAD_TRAY_PP_STATUS,
// 											   IDC_EDIT_PICKER_ALIGN_LOAD_TEST_PP_1_STATUS, IDC_EDIT_PICKER_ALIGN_UNLOAD_TEST_PP_1_STATUS,
// 											   IDC_EDIT_PICKER_ALIGN_LOAD_TEST_PP_2_STATUS, IDC_EDIT_PICKER_ALIGN_UNLOAD_TEST_PP_2_STATUS };
 

// CVATMain_PickerAlign 대화 상자입니다.

IMPLEMENT_DYNAMIC(CVATMain_PickerAlign, CDialogEx)

CVATMain_PickerAlign::CVATMain_PickerAlign(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VAT_MAIN_PICKER_ALIGN, pParent)
{
	m_nCurTab = MTPI_PICKER_ALIGN_MEASURE;
}

CVATMain_PickerAlign::~CVATMain_PickerAlign()
{
}

void CVATMain_PickerAlign::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PICKER_ALIGN_TAB_VIEW, m_tabPickerAlign);


	
//  	for (int i = 0; i < VAT_HAND_MAX; i++) {
//  		DDX_Control(pDX, m_nPositionControl[i], m_editPosition[i]);
//  	}
}


BEGIN_MESSAGE_MAP(CVATMain_PickerAlign, CDialogEx)
	ON_WM_DESTROY()
	//ON_WM_ERASEBKGND()
	//ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CVATMain_PickerAlign 메시지 처리기입니다.

BOOL CVATMain_PickerAlign::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//Data Load
	m_stVATPickerAlignInfoGui = g_VATCommonData.m_stVATPickerAlignInfo;

	//Main Tab 초기화.
	InitPickerAlignMainTab();

	return TRUE;
}

//==============================================================================
//COMMENT : Mode 변경 후,  Picker Align Page 이동 시 초기화역할로 사용
//==============================================================================
void CVATMain_PickerAlign::PageMoveInit_Main()
{
	m_pgMeasure.PageMoveInit_Tab();
	m_pgSetting.PageMoveInit_Tab();
}


//================================================================================
//COMMENT : Mode 변경 후, Picker Align Page 이동 시, 소멸자 역할로 사용
//================================================================================
void CVATMain_PickerAlign::PageMoveDispose_Main()
{
	m_pgMeasure.PageMoveDispose_Tab();
	m_pgSetting.PageMoveDispose_Tab();
}


//==============================================================================
//COMMENT : Picker Align Page에서 Tab 변경 후,  Tab Dlg 초기화역할로 사용
//==============================================================================
void CVATMain_PickerAlign::PageMoveInit_Tab()
{

}

BEGIN_EVENTSINK_MAP(CVATMain_PickerAlign, CDialogEx)
ON_EVENT(CVATMain_PickerAlign, IDC_PICKER_ALIGN_SSPANEL_TAB_TYPE_MEASURE, DISPID_CLICK, CVATMain_PickerAlign::ClickPickerAlignSspanelTabTypeMeasure, VTS_NONE)
ON_EVENT(CVATMain_PickerAlign, IDC_PICKER_ALIGN_SSPANEL_TAB_TYPE_PKR_SETTING, DISPID_CLICK, CVATMain_PickerAlign::ClickPickerAlignSspanelTabTypePkrSetting, VTS_NONE)
END_EVENTSINK_MAP()


// void CVATMain_PickerAlign::InitSSPickerStatus()
// {
// 	CSpreadSheet *ssPickerStatus[VAT_HAND_MAX] = { &m_ssPkr_LoadTrayPP, &m_ssPkr_UnloadTrayPP, &m_ssPkr_LoadTestPP1, &m_ssPkr_UnloadTestPP1, &m_ssPkr_LoadTestPP2, &m_ssPkr_UnloadTestPP2 };
// 	int nHandIdx = 0;
// 
// 	/*for (auto ss : SSPickerStatus)*/
// 	for(int nHand = 0; nHand < VAT_HAND_MAX; nHand++)
// 	{
// 		ssPickerStatus[nHand]->SetFontName(_T("Arial"));
// 		ssPickerStatus[nHand]->SetFontSize(11);
// 		ssPickerStatus[nHand]->SetTypeHAlign(2); // Center Align
// 		ssPickerStatus[nHand]->SetTypeVAlign(2);
// 		ssPickerStatus[nHand]->SetColHeadersShow(FALSE);
// 		ssPickerStatus[nHand]->SetRowHeadersShow(FALSE);
// 		ssPickerStatus[nHand]->SetCellType(7); // Button Type
// 		ssPickerStatus[nHand]->SetFontBold(TRUE);
// 
// 		CString strPicker[VAT_MAX_HAND_CNT_Y][VAT_MAX_HAND_CNT_X] = {
// 			{ _T("1"), _T("2"), _T("3"), _T("4") },
// 			{ _T("5"), _T("6"), _T("7"), _T("8") }
// 		};
// 
// 		int nMaxPkrX = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? GUI_MAX_TRAY_PP_PICKER_CNT_X : GUI_MAX_TEST_PP_PICKER_CNT_X;
// 		int nMaxPkrY = (nHand == VAT_HAND_LOAD_TRAYPP || nHand == VAT_HAND_UNLOAD_TRAYPP) ? GUI_MAX_TRAY_PP_PICKER_CNT_Y : GUI_MAX_TEST_PP_PICKER_CNT_Y;
// 
// 		for (int nGuiY = 0; nGuiY < nMaxPkrY; nGuiY++)
// 		{
// 			for (int nGuiX = 0; nGuiX < nMaxPkrX; nGuiX++)
// 			{
// 				int nCol = nGuiX + 1;
// 				int nRow = nGuiY + 1;
// 
// 				ssPickerStatus[nHand]->SetCol(nCol);
// 				ssPickerStatus[nHand]->SetRow(nRow);
// 
// 		
// 				ssPickerStatus[nHand]->SetTypeButtonColor(WWHITE);
// 
// 				int nPkrX = nGuiX;
// 				int nPkrY = nGuiY;
// 				ConvPkrIdx_Gui_To_Real(nHand, nGuiX, nGuiY, nPkrX, nPkrY);
// 				ssPickerStatus[nHand]->SetTypeButtonText(strPicker[nPkrY][nPkrX]);
// 			}
// 		}
// 
// 		//Col width 설정.
// 		for (int i = 1; i <= nMaxPkrX; i++) {
// 			ssPickerStatus[nHand]->SetColWidth(i, 6.2);
// 		}
// 
// 		//Row Height 설정.
// 		for (int i = 1; i <= nMaxPkrY; i++) {
// 			ssPickerStatus[nHand]->SetRowHeight(i, 35);
// 		}
// 
// 		ssPickerStatus[nHand]->SetAutoSize(TRUE);
// 	}
// }

void CVATMain_PickerAlign::InitPickerAlignMainTab()
{
	m_tabPickerAlign.SetCurSel(MTPI_PICKER_ALIGN_MEASURE);

	CRect rect;
	m_tabPickerAlign.GetWindowRect(rect);

	//Measure Page Init
	m_pgMeasure.Create(CVATMainTab_PickerAlignMeasure::IDD, &m_tabPickerAlign);
	m_pgMeasure.MoveWindow(13, 5, rect.Width()-15, rect.Height() - 15);
	//m_pgMeasure.m_nPickerAlignTabType = MTPI_PICKER_ALIGN_MEASURE;
	//m_pgSetting.SetBackgroundColor(YELLOW_DUCK);

	//Picker Setting Page Init
	m_pgSetting.Create(CVATMainTab_PickerAlignSetting::IDD, &m_tabPickerAlign);
	m_pgSetting.MoveWindow(13, 5, rect.Width() - 15, rect.Height() - 15);
	//m_pgMeasure.m_nPickerAlignTabType = MTPI_EACH_PICKER_SETTING;
	//m_pgSetting.SetBackgroundColor(LLIGHTGRAY);

	DisplayPickerAlignTabMode(MTPI_PICKER_ALIGN_MEASURE);
}

void CVATMain_PickerAlign::DisplayPickerAlignTabMode(int nTabIdx)
{	
	HWND m_hMain = NULL;

	((CSSCommand *)GetDlgItem(IDC_PICKER_ALIGN_SSPANEL_TAB_TYPE_MEASURE))->SetBackColor(TAB_BACKGROUND_COLOR);
	((CSSCommand *)GetDlgItem(IDC_PICKER_ALIGN_SSPANEL_TAB_TYPE_PKR_SETTING))->SetBackColor(TAB_BACKGROUND_COLOR);

	switch (nTabIdx)
	{
		case MTPI_PICKER_ALIGN_MEASURE:
		{
			((CSSCommand *)GetDlgItem(IDC_PICKER_ALIGN_SSPANEL_TAB_TYPE_MEASURE))->SetForeColor(BBLACK);
			((CSSCommand *)GetDlgItem(IDC_PICKER_ALIGN_SSPANEL_TAB_TYPE_PKR_SETTING))->SetForeColor(TAB_UNSELECT_FORE_COLOR);

			m_pgMeasure.ShowWindow(SW_SHOW);
			m_pgSetting.ShowWindow(SW_HIDE);

			m_hMain = m_pgMeasure.GetSafeHwnd();
		}break;
		case MTPI_EACH_PICKER_SETTING:
		{
			((CSSCommand *)GetDlgItem(IDC_PICKER_ALIGN_SSPANEL_TAB_TYPE_MEASURE))->SetForeColor(TAB_UNSELECT_FORE_COLOR);
			((CSSCommand *)GetDlgItem(IDC_PICKER_ALIGN_SSPANEL_TAB_TYPE_PKR_SETTING))->SetForeColor(BBLACK);

			m_pgMeasure.ShowWindow(SW_HIDE);
			m_pgSetting.ShowWindow(SW_SHOW);

			m_hMain = m_pgSetting.GetSafeHwnd();
		}break;
		default:
			break;
	}

	//Measure/Setting Tab에서도 Vision Offset을 확인 하므로, GUI Notifier의 Handle을 Setting Tab으로 변경 필요함.
	CVAT_MeasureStatus::GetInstance().SetVatMainPageHandle(m_hMain);
}


void CVATMain_PickerAlign::ClickPickerAlignSspanelTabTypeMeasure()
{
	DisplayPickerAlignTabMode(MTPI_PICKER_ALIGN_MEASURE);
}


void CVATMain_PickerAlign::ClickPickerAlignSspanelTabTypePkrSetting()
{
	DisplayPickerAlignTabMode(MTPI_EACH_PICKER_SETTING);
}

