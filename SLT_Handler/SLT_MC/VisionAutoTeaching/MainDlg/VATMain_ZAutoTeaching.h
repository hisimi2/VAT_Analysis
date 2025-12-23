#pragma once

#include "VATMainTab_ZAutoTeachingMeasure.h"
//#include "VATMainTab_VisionAutoTeachingSetting.h"
#include "VATMainTab_ZAutoTeachingSetting.h"
//#include "VATMainTab_PartBasePos.h"

// CVATMain_ZAutoTeaching 대화 상자입니다.

class CVATMain_ZAutoTeaching : public CDialogEx
{
	DECLARE_DYNAMIC(CVATMain_ZAutoTeaching)

public:
	CVATMain_ZAutoTeaching(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CVATMain_ZAutoTeaching();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VAT_MAIN_Z_AUTO_TEACHING };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void PageMoveInit_Main();
	void PageMoveDispose_Main();

	enum _eZAutoTeachingMainTabIndex
	{
		MTPI_Z_AUTO_TEACHING_MEASURE = 0,
		MTPI_Z_AUTO_TEACHING_BASE_POS, //PART_BASE_POSITION
	};

public:
	CTabCtrl m_tabZAutoTeaching;
	
	CVATMainTab_ZAutoTeachingMeasure m_pgMeasure;
	CVATMainTab_ZAutoTeachingSetting m_pgSetting; //Z Auto Teaching & Vision Auto Teaching에서 공통으로 사용.
	

	void InitZAutoTeachingMainTab();
	void DisplayZAutoTeachingTabMode(int nTabIdx);

public:
	DECLARE_EVENTSINK_MAP()

	void ClickZTeachingSspanelTabTypeMeasure();
	void ClickZTeachingSspanelTabTypeBasePosition();
};
