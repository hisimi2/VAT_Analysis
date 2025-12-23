#pragma once
#include "VAT_PickerAlignDlgCom.h"
#include "VATSub_PickerAlignInfo.h"
// CVATMainTab_PickerAlignMeasure 대화 상자입니다.

class CVATMainTab_PickerAlignMeasure : public CDialogEx, public CVATMain_Common, public CVAT_PickerAlignDlgCom
{
	DECLARE_DYNAMIC(CVATMainTab_PickerAlignMeasure)

public:
	CVATMainTab_PickerAlignMeasure(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CVATMainTab_PickerAlignMeasure();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VAT_MAIN_PICKER_ALIGN_MEASURE_DLG };


public:
	virtual BOOL OnInitDialog();
	void PageMoveInit_Tab();
	void PageMoveDispose_Tab();

public: //OnTimer
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();

	enum _ePickerAlignTimer
	{
		_ePickerAlignT_PickerStatus = 0,
		_ePickerAlignT_CheckExecuteStatus,

		_ePickerAlignT_Max,
	};

public: //OnPaint
	afx_msg void OnPaint();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


private:
	CDSSPanel m_ssPanel_HandSelect;
	CDSSPanel m_ssPanel_MeasureStatus;
	CDSSPanel m_ssPanel_PickerAlignView;

	//Picker Information
	CShadeButtonST m_BtnPickerInfo;
	CVATSub_PickerAlignInfo *m_pDlgPkrInfo; //Measure Tab으로 옮겨야 함.


	CDSSPanel m_editPitchStatus[VAT_HAND_MAX][VAT_PITCH_TYPE_MAX];

	//Option
	CButton m_CkBtnPickerAutoFocus;

public:
	void InitPickerAlignView();
	void InitPickerAlignMeasureOverView();	
	void InitOverViewRect(); //Offset Grid
	void InitPitchTypeStatus();
	
	void InitPickerAlignOption();

	void BlockPkrSpreadSheet();

	void DisplaySelectedPicker(int nHand, BOOL bIsHandSelected);

	void EnableControls(BOOL bStatus);
public:

	DECLARE_EVENTSINK_MAP()

	void ButtonClickedSpreadPickerAlignLoadTraypp(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadPickerAlignUnloadTraypp(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadPickerAlignLoadTestpp1(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadPickerAlignUnloadTestpp1(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadPickerAlignLoadTestpp2(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadPickerAlignUnloadTestpp2(long Col, long Row, short ButtonDown);
	afx_msg void OnBnClickedBtnPickerAlignInfo();

	//Measure Overview
	void DisplayPitchType(int nCurPitchType);
	afx_msg LRESULT OnReceiveUpdatePickerStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceiveUpdateProgress(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceiveUpdateVisionStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceiveUpdateVisionOffset(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceiveUpdateVisionMeasureCnt(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceiveUpdateSavedData(WPARAM wParam, LPARAM lParam);

	//Option
	afx_msg void OnBnClickedChkPickerAlignAutoFocus();
};
