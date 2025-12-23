// VATMain_VisionAutoTeaching.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "VATMain_VisionAutoTeaching.h"
#include "afxdialogex.h"


// CVATMain_VisionAutoTeaching 대화 상자입니다.

IMPLEMENT_DYNAMIC(CVATMain_VisionAutoTeaching, CDialogEx)

CVATMain_VisionAutoTeaching::CVATMain_VisionAutoTeaching(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VAT_MAIN_VISION_AUTO_TEACHING, pParent)
{

}

CVATMain_VisionAutoTeaching::~CVATMain_VisionAutoTeaching()
{
}

void CVATMain_VisionAutoTeaching::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VISION_AUTO_TEACHING_TAB_VIEW, m_tabVisionAutoTeaching);
}


BEGIN_MESSAGE_MAP(CVATMain_VisionAutoTeaching, CDialogEx)
END_MESSAGE_MAP()




BOOL CVATMain_VisionAutoTeaching::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//Main Tab 초기화.
	InitVisionAutoTeachingMainTab();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


//==============================================================================
//COMMENT : Mode 변경 후,  Z Auto Teaching Page 이동 시 초기화역할로 사용
//==============================================================================
void CVATMain_VisionAutoTeaching::PageMoveInit_Main()
{
	m_pgMeasure.PageMoveInit_Tab();
	m_pgSetting.PageMoveInit_Tab();
}


//================================================================================
//COMMENT : Mode 변경 후, Z Auto Teaching Page 이동 시, 소멸자 역할로 사용
//================================================================================
void CVATMain_VisionAutoTeaching::PageMoveDispose_Main()
{
	m_pgMeasure.PageMoveDispose_Tab();
	m_pgSetting.PageMoveDispose_Tab();
}


void CVATMain_VisionAutoTeaching::InitVisionAutoTeachingMainTab()
{
	m_tabVisionAutoTeaching.SetCurSel(MTPI_VISION_AUTO_TEACHING_MEASURE);

	CRect rect;
	m_tabVisionAutoTeaching.GetWindowRect(rect);

	//Measure Page Init
	m_pgMeasure.Create(CVATMainTab_VisionAutoTeachingMeasure::IDD, &m_tabVisionAutoTeaching);
	m_pgMeasure.MoveWindow(1, 1, rect.Width() - 15, rect.Height() - 15);

	//Base Position Page Init
	m_pgSetting.Create(CVATMainTab_VisionAutoTeachingSetting::IDD, &m_tabVisionAutoTeaching);
	m_pgSetting.MoveWindow(1, 1, rect.Width() - 15, rect.Height() - 15);

	DisplayVisionAutoTeachingTabMode(/*MTPI_VISION_AUTO_TEACHING_BASE_POS*/MTPI_VISION_AUTO_TEACHING_MEASURE);
}

void CVATMain_VisionAutoTeaching::DisplayVisionAutoTeachingTabMode(int nTabIdx)
{
	((CSSCommand *)GetDlgItem(IDC_VISION_AUTO_TEACHING_SSPANEL_TAB_TYPE_MEASURE))->SetBackColor(TAB_BACKGROUND_COLOR);
	((CSSCommand *)GetDlgItem(IDC_VISION_AUTO_TEACHING_SSPANEL_TAB_TYPE_BASE_POSITION))->SetBackColor(TAB_BACKGROUND_COLOR);

	switch (nTabIdx)
	{
	case MTPI_VISION_AUTO_TEACHING_MEASURE:
	{
		((CSSCommand *)GetDlgItem(IDC_VISION_AUTO_TEACHING_SSPANEL_TAB_TYPE_MEASURE))->SetForeColor(BBLACK);
		((CSSCommand *)GetDlgItem(IDC_VISION_AUTO_TEACHING_SSPANEL_TAB_TYPE_BASE_POSITION))->SetForeColor(TAB_UNSELECT_FORE_COLOR);

		m_pgMeasure.ShowWindow(SW_SHOW);
		m_pgSetting.ShowWindow(SW_HIDE);
	}break;
	case MTPI_VISION_AUTO_TEACHING_BASE_POS:
	{
		((CSSCommand *)GetDlgItem(IDC_VISION_AUTO_TEACHING_SSPANEL_TAB_TYPE_MEASURE))->SetForeColor(TAB_UNSELECT_FORE_COLOR);
		((CSSCommand *)GetDlgItem(IDC_VISION_AUTO_TEACHING_SSPANEL_TAB_TYPE_BASE_POSITION))->SetForeColor(BBLACK);

		m_pgMeasure.ShowWindow(SW_HIDE);
		m_pgSetting.ShowWindow(SW_SHOW);
	}break;
	default:
		break;
	}
}


BEGIN_EVENTSINK_MAP(CVATMain_VisionAutoTeaching, CDialogEx)
ON_EVENT(CVATMain_VisionAutoTeaching, IDC_VISION_AUTO_TEACHING_SSPANEL_TAB_TYPE_MEASURE, DISPID_CLICK, CVATMain_VisionAutoTeaching::ClickVisionAutoTeachingSspanelTabTypeMeasure, VTS_NONE)
ON_EVENT(CVATMain_VisionAutoTeaching, IDC_VISION_AUTO_TEACHING_SSPANEL_TAB_TYPE_BASE_POSITION, DISPID_CLICK, CVATMain_VisionAutoTeaching::ClickVisionAutoTeachingSspanelTabTypeBasePosition, VTS_NONE)
END_EVENTSINK_MAP()


void CVATMain_VisionAutoTeaching::ClickVisionAutoTeachingSspanelTabTypeMeasure()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	DisplayVisionAutoTeachingTabMode(MTPI_VISION_AUTO_TEACHING_MEASURE);
}


void CVATMain_VisionAutoTeaching::ClickVisionAutoTeachingSspanelTabTypeBasePosition()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	DisplayVisionAutoTeachingTabMode(MTPI_VISION_AUTO_TEACHING_BASE_POS);
}
