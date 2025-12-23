#pragma once

// CVATMainTab_ZAutoTeachingMeasure 대화 상자입니다.
//#define UM_VAT_SEQ_MSG_MEASURE_COMPLETE	  (WM_USER + 100)

class CVATMainTab_ZAutoTeachingMeasure : public CDialogEx, public CVATMain_Common
{
	DECLARE_DYNAMIC(CVATMainTab_ZAutoTeachingMeasure)

public:
	CVATMainTab_ZAutoTeachingMeasure(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CVATMainTab_ZAutoTeachingMeasure();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VAT_MAIN_Z_AUTO_TEACHING_MEASURE_DLG };


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
	int m_nBoardDir;


public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void PageMoveInit_Tab();
	void PageMoveDispose_Tab();

	CShadeButtonST m_BtnCancel;

/////////Timer
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	enum eTimer
	{
		_eZTeachMeasureT_CurrentPos = 0,
		_eZTeachMeasureT_ChkVacuum,
		_eZTeachMeasureT_CheckExecuteStatus,
		_eZTeachMeasureT_PartStatus,

		_eZTeachMeasureT_Max,

	};

	//_eZTeachMeasureT_CurrentPos
	void _tDisplayCurrentPos();
	void MakeBMAxisPickerIdx(std::vector<int>& vPicker, CBbxmnpClient::_ePICK_MOTOR eMotor);
	
	//_eZTeachMeasureT_ChkVacuum
	void _tChkVacuumSen();
	int GetPickerVacuum(int nHand, CPoint picker);

	//_eZTeachMeasureT_PartStatus
	void _tDisplayPartStatus();


//Data
public:
	enum eZAutoTeaching_DataType
	{
		eDataType_Offset = 0,
		eDataType_Vacuum,
	};

	int m_nLastDataType[VAT_HAND_MAX];

	void ChangeDataPanelColor(int nHand, int nType);

	void DisplayPickerData(int nHand, int nType);
	void DisplayPickerDataAll(int nType);
	
public:
	//Control Enable/Disable
	BOOL m_bBlockEditFocus;
	void EnableControls_MeasureTab(BOOL bStatus);

public:

	DECLARE_EVENTSINK_MAP()
	//======================================================================================
	//COMMENT : 1Hand, 1Part에 대한 Select/Unselect
	//======================================================================================
	void ButtonClickedSpreadLdTrayppPart(long Col, long Row, short ButtonDown) { _Com_SetSelectedPart(VAT_HAND_LOAD_TRAYPP, DEF_PART_INDEX(Row)); };
	void ButtonClickedSpreadUdTrayppPart(long Col, long Row, short ButtonDown) { _Com_SetSelectedPart(VAT_HAND_UNLOAD_TRAYPP, DEF_PART_INDEX(Row)); };
	void ButtonClickedSpreadLdTestpp1Part(long Col, long Row, short ButtonDown) { _Com_SetSelectedPart(VAT_HAND_LOAD_TESTPP1, DEF_PART_INDEX(Row)); };
	void ButtonClickedSpreadUdTestpp1Part(long Col, long Row, short ButtonDown) { _Com_SetSelectedPart(VAT_HAND_UNLOAD_TESTPP1, DEF_PART_INDEX(Row)); };
	void ButtonClickedSpreadLdTestpp2Part(long Col, long Row, short ButtonDown) { _Com_SetSelectedPart(VAT_HAND_LOAD_TESTPP2, DEF_PART_INDEX(Row)); };
	void ButtonClickedSpreadUdTestpp2Part(long Col, long Row, short ButtonDown) { _Com_SetSelectedPart(VAT_HAND_UNLOAD_TESTPP2, DEF_PART_INDEX(Row)); };

	//======================================================================================
	//COMMENT : 1Hand, All Part에 대한 Select/Unselect
	//======================================================================================
	void ClickSspanelLdTrayppPart() { _Com_SetSelectedAllPart(VAT_HAND_LOAD_TRAYPP); };
	void ClickSspanelUdTrayppPart() { _Com_SetSelectedAllPart(VAT_HAND_UNLOAD_TRAYPP); };
	void ClickSspanelLdTestpp1Part() { _Com_SetSelectedAllPart(VAT_HAND_LOAD_TESTPP1); };
	void ClickSspanelUdTestpp1Part() { _Com_SetSelectedAllPart(VAT_HAND_UNLOAD_TESTPP1); };
	void ClickSspanelLdTestpp2Part() { _Com_SetSelectedAllPart(VAT_HAND_LOAD_TESTPP2); };
	void ClickSspanelUdTestpp2Part() { _Com_SetSelectedAllPart(VAT_HAND_UNLOAD_TESTPP2); };

	//======================================================================================
	//COMMENT : Flatness Offset Display
	//======================================================================================
	void ClickSspanelLdTrayppFlatnessOffset() { DisplayPickerData(VAT_HAND_LOAD_TRAYPP, eDataType_Offset); };
	void ClickSspanelUdTrayppFlatnessOffset() { DisplayPickerData(VAT_HAND_UNLOAD_TRAYPP, eDataType_Offset); };
	void ClickSspanelLdTestpp1FlatnessOffset() { DisplayPickerData(VAT_HAND_LOAD_TESTPP1, eDataType_Offset); };
	void ClickSspanelUdTestpp1FlatnessOffset() { DisplayPickerData(VAT_HAND_UNLOAD_TESTPP1, eDataType_Offset); };
	void ClickSspanelLdTestpp2FlatnessOffset() { DisplayPickerData(VAT_HAND_LOAD_TESTPP2, eDataType_Offset); };
	void ClickSspanelUdTestpp2FlatnessOffset() { DisplayPickerData(VAT_HAND_UNLOAD_TESTPP2, eDataType_Offset); };

	//======================================================================================
	//COMMENT : Flatness Offset Display
	//======================================================================================
 	void ClickSspanelLdTrayppPickPos() { DisplayPickerData(VAT_HAND_LOAD_TRAYPP, eDataType_Vacuum); };
 	void ClickSspanelUdTrayppPickPos() { DisplayPickerData(VAT_HAND_UNLOAD_TRAYPP, eDataType_Vacuum); };
 	void ClickSspanelLdTestpp1PickPos() { DisplayPickerData(VAT_HAND_LOAD_TESTPP1, eDataType_Vacuum); };
 	void ClickSspanelUdTestpp1PickPos() { DisplayPickerData(VAT_HAND_UNLOAD_TESTPP1, eDataType_Vacuum); };
 	void ClickSspanelLdTestpp2PickPos() { DisplayPickerData(VAT_HAND_LOAD_TESTPP2, eDataType_Vacuum); };
 	void ClickSspanelUdTestpp2PickPos() { DisplayPickerData(VAT_HAND_UNLOAD_TESTPP2, eDataType_Vacuum); };


	//======================================================================================
	//COMMENT : ComboBox Selchange
	//======================================================================================
	afx_msg void OnCbnSelchangeComboLdTrayPpPos() { DisplayPickerData(VAT_HAND_LOAD_TRAYPP, m_nLastDataType[VAT_HAND_LOAD_TRAYPP]); };
	afx_msg void OnCbnSelchangeComboUdTrayPpPos() { DisplayPickerData(VAT_HAND_UNLOAD_TRAYPP, m_nLastDataType[VAT_HAND_UNLOAD_TRAYPP]); };
	afx_msg void OnCbnSelchangeComboLdTestPp1Pos() { DisplayPickerData(VAT_HAND_LOAD_TESTPP1, m_nLastDataType[VAT_HAND_LOAD_TESTPP1]); };
	afx_msg void OnCbnSelchangeComboUdTestPp1Pos() { DisplayPickerData(VAT_HAND_UNLOAD_TESTPP1, m_nLastDataType[VAT_HAND_UNLOAD_TESTPP1]); };
	afx_msg void OnCbnSelchangeComboLdTestPp2Pos() { DisplayPickerData(VAT_HAND_LOAD_TESTPP2, m_nLastDataType[VAT_HAND_LOAD_TESTPP2]); };
	afx_msg void OnCbnSelchangeComboUdTestPp2Pos() {DisplayPickerData(VAT_HAND_UNLOAD_TESTPP2, m_nLastDataType[VAT_HAND_UNLOAD_TESTPP2]);};

	//======================================================================================
	//COMMENT : Change Flateness Data 
	//======================================================================================
	double m_dCtrlPickerData[VAT_HAND_MAX][VAT_PICKER_MAX]; //변수 저장.
	CEditTrans m_EditPickerData[VAT_HAND_MAX][VAT_PICKER_MAX]; //Contorl Design

	CShadeButtonST m_BtnReload[VAT_HAND_MAX];
	CShadeButtonST m_BtnSave[VAT_HAND_MAX];


	void InitEditDesign();
	void InitCtrlButton();
	
	afx_msg void OnEnSetFocusHandData(UINT nID);
	afx_msg void OnBtnClickedFlatnessOffsetReload(UINT nID);
	afx_msg void OnBtnClickedFlatnessOffsetSave(UINT nID);
	void ConvCtrlIdToHandAndPkrIdx(UINT nID, int &nHandIdx, int &nPkrIdx);
	//OnEnSetFocusHandData

	HWND GetThisHandle() { return this->GetSafeHwnd(); }
	afx_msg LRESULT OnReceiveMeasureComplete(WPARAM wParam, LPARAM lParam);
};
