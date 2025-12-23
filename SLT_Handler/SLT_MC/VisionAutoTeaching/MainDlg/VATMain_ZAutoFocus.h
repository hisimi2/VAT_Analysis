#pragma once


// CVATMain_ZAutoFocus 대화 상자입니다.

class CVATMain_ZAutoFocus : public CDialogEx, public CVATMain_Common
{
	DECLARE_DYNAMIC(CVATMain_ZAutoFocus)

public:
	CVATMain_ZAutoFocus(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CVATMain_ZAutoFocus();

// 대화 상자 데이터입니다.

	enum { IDD = IDD_VAT_MAIN_Z_AUTO_FOCUS };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void PageMoveInit_Main();
	void PageMoveDispose();

public: //OnTimer
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();

	enum _eZAutoFocusTimer
	{
		_eZFocusT_HandStatus = 0,
		_eZFocusT_CheckExecuteStatus,

		_eZFocusT_Max,
	};

	enum _eZAutoFocusSSType
	{
		_eZFocusSS_PlateJigTargetPos = 0,
		_eZFocusSS_PlateJigVacPos,
	};

private:
	//Gui에서 변경 된 Data 관리.
	St_VAT_ZFocusInfo m_stVATZFocusInfoGui;
		
public:
	CDSSPanel m_ssPanel_HandSelect;
	CDSSPanel m_ssPanel_MeasureStatus;

	//Plate Jig Target Pos
	CDSSPanel m_ssDataPanel_1_Title;
	CDSSPanel m_ssDataPanel_1_Sub1;
	CDSSPanel m_ssDataPanel_1_Sub2;
	int m_nSS1_DisplayType;
	CSpreadSheet m_ssPlateJigTargetPos;
	_stVAT_BASE_POS_SHEET_INFO m_stTargetPosSheetLastInfo;

	//Plate Jig Vacuum Pos
	CDSSPanel m_ssDataPanel_2_Title;
	CDSSPanel m_ssDataPanel_2_Sub1;
	CDSSPanel m_ssDataPanel_2_Sub2;
	int m_nSS2_DisplayType;
	CSpreadSheet m_ssPlateJigVacPos;
	_stVAT_BASE_POS_SHEET_INFO m_stPlateJigVacPosSheetLastInfo;

	//Operate Button
	CShadeButtonST m_BtnOper1_EncoderRead;
	CShadeButtonST m_BtnOper1_MotorMove;
	CShadeButtonST m_BtnOper1_Reload;
	CShadeButtonST m_BtnOper1_Save;
	CShadeButtonST m_BtnOper1_MakeBasePos;

	CShadeButtonST m_BtnOper2_EncoderRead;
	CShadeButtonST m_BtnOper2_MotorMove;
	CShadeButtonST m_BtnOper2_Reload;
	CShadeButtonST m_BtnOper2_Save;
	CShadeButtonST m_BtnOper2_MakeBasePos;

	void InitZFocusBasePosSheet();
	void InitZFocusOperBtn();
	void InitZFocusMeasureOverview();
	//void InitZFocusEditControl();
	//void InitZFocusVisionStatus();

	void DisplaySSData(int nZFocusSSType, int nSSDisplayType, BOOL bMakeBasePos = FALSE);
	int CheckPosLimit(int nZFocusSSType);

	void EnableControls(BOOL bStatus);
public:
	DECLARE_EVENTSINK_MAP()
	
	void ButtonClickedSpreadZfocusLoadTraypp(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadZfocusUnloadTraypp(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadZfocusLoadTestpp1(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadZfocusUnloadTestpp1(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadZfocusLoadTestpp2(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadZfocusUnloadTestpp2(long Col, long Row, short ButtonDown);

	void ClickSspanelZfocusTargetPosData();
	void ClickSspanelZfocusTargetPosDataDiff();
	void ClickSspanelZfocusVacuumPosData();
	void ClickSspanelZfocusVacuumPosDataDiff();

	void ClickSpreadZfocusTargetPosition(long Col, long Row);
	void DblClickSpreadZfocusTargetPosition(long Col, long Row);
	void ClickSpreadZfocusVacuumPosition(long Col, long Row);
	void DblClickSpreadZfocusVacuumPosition(long Col, long Row);

	afx_msg void OnBnClickedBtnZfocusEncoderRead1();
	afx_msg void OnBnClickedBtnZfocusMotorMove1();
	afx_msg void OnBnClickedBtnZfocusReload1();
	afx_msg void OnBnClickedBtnZfocusDataSave1();
	afx_msg void OnBnClickedBtnZfocusEncoderRead2();
	afx_msg void OnBnClickedBtnZfocusMotorMove2();
	afx_msg void OnBnClickedBtnZfocusReload2();
	afx_msg void OnBnClickedBtnZfocusDataSave2();

	//Measure Overview
	afx_msg LRESULT OnReceiveUpdateHandStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceiveUpdateProgress(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceiveUpdateVisionStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceiveUpdateVisionOffset(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceiveUpdateVisionMeasureCnt(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceiveUpdateSavedData(WPARAM wParam, LPARAM lParam);

	//Cursor Block 용도
	afx_msg void OnEnSetFocusCursorBlock(UINT nID);
	afx_msg void OnBnClickedBtnZfocusMakeBasePos1();
	afx_msg void OnBnClickedBtnZfocusMakeBasePos2();
};
