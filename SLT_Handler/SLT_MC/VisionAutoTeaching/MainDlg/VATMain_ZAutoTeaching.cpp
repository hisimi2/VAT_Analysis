// VATMain_ZAutoTeaching.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SLT_MC.h"
#include "VATMain_ZAutoTeaching.h"
#include "afxdialogex.h"


// CVATMain_ZAutoTeaching 대화 상자입니다.

IMPLEMENT_DYNAMIC(CVATMain_ZAutoTeaching, CDialogEx)

CVATMain_ZAutoTeaching::CVATMain_ZAutoTeaching(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VAT_MAIN_Z_AUTO_TEACHING, pParent)
{

}

CVATMain_ZAutoTeaching::~CVATMain_ZAutoTeaching()
{
}

void CVATMain_ZAutoTeaching::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Z_AUTO_TEACHING_TAB_VIEW, m_tabZAutoTeaching);
}


BEGIN_MESSAGE_MAP(CVATMain_ZAutoTeaching, CDialogEx)
END_MESSAGE_MAP()


BOOL CVATMain_ZAutoTeaching::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//Main Tab 초기화.
	InitZAutoTeachingMainTab();

	return TRUE;
}

//==============================================================================
//COMMENT : Mode 변경 후,  Z Auto Teaching Page 이동 시 초기화역할로 사용
//==============================================================================
void CVATMain_ZAutoTeaching::PageMoveInit_Main()
{
	m_pgMeasure.PageMoveInit_Tab();
	m_pgSetting.PageMoveInit_Tab();
	//m_pgBasePos.PageMoveInit_Tab();
}


//================================================================================
//COMMENT : Mode 변경 후, Z Auto Teaching Page 이동 시, 소멸자 역할로 사용
//================================================================================
void CVATMain_ZAutoTeaching::PageMoveDispose_Main()
{
	m_pgMeasure.PageMoveDispose_Tab();
	m_pgSetting.PageMoveDispose_Tab();
}


void CVATMain_ZAutoTeaching::InitZAutoTeachingMainTab()
{
	m_tabZAutoTeaching.SetCurSel(MTPI_Z_AUTO_TEACHING_MEASURE);

	CRect rect;
	m_tabZAutoTeaching.GetWindowRect(rect);

	//Measure Page Init
	m_pgMeasure.Create(CVATMainTab_ZAutoTeachingMeasure::IDD, &m_tabZAutoTeaching);
	m_pgMeasure.MoveWindow(1, 1, rect.Width() - 15, rect.Height() - 15);
	
	//Base Position Page Init
	m_pgSetting.Create(CVATMainTab_ZAutoTeachingSetting::IDD, &m_tabZAutoTeaching);
	m_pgSetting.MoveWindow(1, 1, rect.Width() - 15, rect.Height() - 15);
	

	DisplayZAutoTeachingTabMode(MTPI_Z_AUTO_TEACHING_MEASURE);
}


void CVATMain_ZAutoTeaching::DisplayZAutoTeachingTabMode(int nTabIdx)
{
	((CSSCommand *)GetDlgItem(IDC_Z_TEACHING_SSPANEL_TAB_TYPE_MEASURE))->SetBackColor(TAB_BACKGROUND_COLOR);
	((CSSCommand *)GetDlgItem(IDC_Z_TEACHING_SSPANEL_TAB_TYPE_BASE_POSITION))->SetBackColor(TAB_BACKGROUND_COLOR);

	switch (nTabIdx)
	{
		case MTPI_Z_AUTO_TEACHING_MEASURE:
		{
			((CSSCommand *)GetDlgItem(IDC_Z_TEACHING_SSPANEL_TAB_TYPE_MEASURE))->SetForeColor(BBLACK);
			((CSSCommand *)GetDlgItem(IDC_Z_TEACHING_SSPANEL_TAB_TYPE_BASE_POSITION))->SetForeColor(TAB_UNSELECT_FORE_COLOR);

			m_pgMeasure.ShowWindow(SW_SHOW);
			m_pgSetting.ShowWindow(SW_HIDE);
		}break;
		case MTPI_Z_AUTO_TEACHING_BASE_POS:
		{
			((CSSCommand *)GetDlgItem(IDC_Z_TEACHING_SSPANEL_TAB_TYPE_MEASURE))->SetForeColor(TAB_UNSELECT_FORE_COLOR);
			((CSSCommand *)GetDlgItem(IDC_Z_TEACHING_SSPANEL_TAB_TYPE_BASE_POSITION))->SetForeColor(BBLACK);

			m_pgMeasure.ShowWindow(SW_HIDE);
			m_pgSetting.ShowWindow(SW_SHOW);
		}break;
		default:
			break;
	}
}

BEGIN_EVENTSINK_MAP(CVATMain_ZAutoTeaching, CDialogEx)
	ON_EVENT(CVATMain_ZAutoTeaching, IDC_Z_TEACHING_SSPANEL_TAB_TYPE_MEASURE, DISPID_CLICK, CVATMain_ZAutoTeaching::ClickZTeachingSspanelTabTypeMeasure, VTS_NONE)
	ON_EVENT(CVATMain_ZAutoTeaching, IDC_Z_TEACHING_SSPANEL_TAB_TYPE_BASE_POSITION, DISPID_CLICK, CVATMain_ZAutoTeaching::ClickZTeachingSspanelTabTypeBasePosition, VTS_NONE)
END_EVENTSINK_MAP()


void CVATMain_ZAutoTeaching::ClickZTeachingSspanelTabTypeMeasure()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	DisplayZAutoTeachingTabMode(MTPI_Z_AUTO_TEACHING_MEASURE);
}


void CVATMain_ZAutoTeaching::ClickZTeachingSspanelTabTypeBasePosition()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	DisplayZAutoTeachingTabMode(MTPI_Z_AUTO_TEACHING_BASE_POS);
}
