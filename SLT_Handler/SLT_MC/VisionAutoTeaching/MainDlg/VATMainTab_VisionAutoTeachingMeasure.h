#pragma once



//※ GUI 실시간 Graphic Offset 표기를 위해 PickerAlignDlgCom 상속.

class CVATMainTab_VisionAutoTeachingMeasure : public CDialogEx, public CVATMain_Common, public CVAT_PickerAlignDlgCom
{
	DECLARE_DYNAMIC(CVATMainTab_VisionAutoTeachingMeasure)

public:
	CVATMainTab_VisionAutoTeachingMeasure(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CVATMainTab_VisionAutoTeachingMeasure();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VAT_MAIN_VISION_AUTO_TEACHING_MEASURE_DLG };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public: //Part 선택 관련은 Measure에서만 사용.
	using CVATMain_Common::_Com_SetSelectedPart;
	using CVATMain_Common::_Com_SetSelectedAllPart;
	using CVATMain_Common::_Com_SetSelectedAll;
	using CVATMain_Common::_Com_IsAnyPartSelected;
 	using CVATMain_Common::_Com_IsPartSelected;



public:
	virtual BOOL OnInitDialog();
	void PageMoveInit_Tab();
	void PageMoveDispose_Tab();

public: //OnTimer
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();

	enum _eVatTimer
	{
// 		_ePickerAlignT_PickerStatus = 0,
// 		_ePickerAlignT_CheckExecuteStatus,
// 
// 		_ePickerAlignT_Max,
	};

public: //OnPaint
	afx_msg void OnPaint();


public: //INIT
	void InitPartEditPos();
	void InitOverViewRect();


	CDSSPanel m_ssPanel_OptHandOnly[VAT_HAND_MAX];
	CDSSPanel m_ssPanel_OptUnitOnly[VAT_HAND_MAX];
	void InitSSOptionPanel();

public:
	DECLARE_EVENTSINK_MAP()



public:
	//======================================================================================
	//COMMENT : 1Hand, 1Part에 대한 Select/Unselect
	//======================================================================================
	void ButtonClickedSpread2LdTrayppPart(long Col, long Row, short ButtonDown) { _Com_SetSelectedPart(VAT_HAND_LOAD_TRAYPP, DEF_PART_INDEX(Row)); };
	void ButtonClickedSpread2UdTrayppPart(long Col, long Row, short ButtonDown) { _Com_SetSelectedPart(VAT_HAND_UNLOAD_TRAYPP, DEF_PART_INDEX(Row)); };
	void ButtonClickedSpread2LdTestpp1Part(long Col, long Row, short ButtonDown) { _Com_SetSelectedPart(VAT_HAND_LOAD_TESTPP1, DEF_PART_INDEX(Row)); };
	void ButtonClickedSpread2UdTestpp1Part(long Col, long Row, short ButtonDown) { _Com_SetSelectedPart(VAT_HAND_UNLOAD_TESTPP1, DEF_PART_INDEX(Row)); };
	void ButtonClickedSpread2LdTestpp2Part(long Col, long Row, short ButtonDown) { _Com_SetSelectedPart(VAT_HAND_LOAD_TESTPP2, DEF_PART_INDEX(Row)); };
	void ButtonClickedSpread2UdTestpp2Part(long Col, long Row, short ButtonDown) { _Com_SetSelectedPart(VAT_HAND_UNLOAD_TESTPP2, DEF_PART_INDEX(Row)); };

	//======================================================================================
	//COMMENT : 1Hand, All Part에 대한 Select/Unselect
	//======================================================================================
	void ClickSspanel2LdTrayppPart() { _Com_SetSelectedAllPart(VAT_HAND_LOAD_TRAYPP); };
	void ClickSspanel2UdTrayppPart() { _Com_SetSelectedAllPart(VAT_HAND_UNLOAD_TRAYPP); };
	void ClickSspanel2LdTestpp1Part() { _Com_SetSelectedAllPart(VAT_HAND_LOAD_TESTPP1); };
	void ClickSspanel2UdTestpp1Part() { _Com_SetSelectedAllPart(VAT_HAND_UNLOAD_TESTPP1); };
	void ClickSspanel2LdTestpp2Part() { _Com_SetSelectedAllPart(VAT_HAND_LOAD_TESTPP2); };
	void ClickSspanel2UdTestpp2Part() { _Com_SetSelectedAllPart(VAT_HAND_UNLOAD_TESTPP2); };
	
	
	
	void _SetOptionHandOnly(int nHand, BOOL bDisable = FALSE);
	void _SetOptionUnitOnly(int nHand, BOOL bDisable = FALSE);
	BOOL _IsUnitTeachingDataValid(int nHand, int nPart);
	ST_PART_SELECT m_stPartSelectedBackUp;

	void ClickSspanelVatHandOnly1() { _SetOptionHandOnly(VAT_HAND_LOAD_TRAYPP); };
	void ClickSspanelVatHandOnly2() { _SetOptionHandOnly(VAT_HAND_UNLOAD_TRAYPP); };
	void ClickSspanelVatHandOnly3() { _SetOptionHandOnly(VAT_HAND_LOAD_TESTPP1); };
	void ClickSspanelVatHandOnly4() { _SetOptionHandOnly(VAT_HAND_LOAD_TESTPP2); };
	void ClickSspanelVatUnitOnly1() { _SetOptionUnitOnly(VAT_HAND_LOAD_TRAYPP); };
	void ClickSspanelVatUnitOnly2() { _SetOptionUnitOnly(VAT_HAND_UNLOAD_TRAYPP); };
	void ClickSspanelVatUnitOnly3() { _SetOptionUnitOnly(VAT_HAND_LOAD_TESTPP1); };
	void ClickSspanelVatUnitOnly4() { _SetOptionUnitOnly(VAT_HAND_LOAD_TESTPP2); };
};
