#pragma once

#include "VAT_PickerAlignDlgCom.h"
#include "VATMainTab_PickerAlignMeasure.h"
#include "VATMainTab_PickerAlignSetting.h"

// CVATMain_PickerAlign 대화 상자입니다.

class CVATMain_PickerAlign : public CDialogEx, public CVATMain_Common
{
	DECLARE_DYNAMIC(CVATMain_PickerAlign)

public:
	CVATMain_PickerAlign(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CVATMain_PickerAlign();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VAT_MAIN_PICKER_ALIGN };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void PageMoveInit_Main();
	void PageMoveDispose_Main();

	void PageMoveInit_Tab();

	enum _ePickerAlignMainTabIndex
	{
		MTPI_PICKER_ALIGN_MEASURE = 0,
		MTPI_EACH_PICKER_SETTING,
	};


private:
	//Gui에서 변경 된 Data 관리.
	St_VAT_PickerAlignInfo m_stVATPickerAlignInfoGui;

public:
	CEditTrans m_editPosition[VAT_HAND_MAX];

	CTabCtrl m_tabPickerAlign;
	int m_nCurTab;
	CVATMainTab_PickerAlignMeasure m_pgMeasure;
	CVATMainTab_PickerAlignSetting m_pgSetting;

public:
	void InitPickerAlignMainTab();
	void DisplayPickerAlignTabMode(int nTabIdx);
public:
	DECLARE_EVENTSINK_MAP()

	void ClickPickerAlignSspanelTabTypeMeasure();
	void ClickPickerAlignSspanelTabTypePkrSetting();
};
