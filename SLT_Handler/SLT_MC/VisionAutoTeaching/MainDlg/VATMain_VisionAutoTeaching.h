#pragma once

#include "VATMainTab_VisionAutoTeachingMeasure.h"
#include "VATMainTab_VisionAutoTeachingSetting.h"
//#include "VATMainTab_PartBasePos.h"
// CVATMain_VisionAutoTeaching 대화 상자입니다.

class CVATMain_VisionAutoTeaching : public CDialogEx
{
	DECLARE_DYNAMIC(CVATMain_VisionAutoTeaching)

public:
	CVATMain_VisionAutoTeaching(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CVATMain_VisionAutoTeaching();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VAT_MAIN_VISION_AUTO_TEACHING };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void PageMoveInit_Main();
	void PageMoveDispose_Main();

	enum _eVisionAutoTeachingMainTabIndex
	{
		MTPI_VISION_AUTO_TEACHING_MEASURE = 0,
		MTPI_VISION_AUTO_TEACHING_BASE_POS, //PART_BASE_POSITION
	};
public:
	CTabCtrl m_tabVisionAutoTeaching;

	CVATMainTab_VisionAutoTeachingMeasure m_pgMeasure;
	CVATMainTab_VisionAutoTeachingSetting m_pgSetting; //Z Auto Teaching & Vision Auto Teaching에서 공통으로 사용.

	void InitVisionAutoTeachingMainTab();
	void DisplayVisionAutoTeachingTabMode(int nTabIdx);


public:
	DECLARE_EVENTSINK_MAP()
	void ClickVisionAutoTeachingSspanelTabTypeMeasure();
	void ClickVisionAutoTeachingSspanelTabTypeBasePosition();
};
