#pragma once


// CVATMainTab_PartBasePos 대화 상자입니다.

class CVATMainTab_VisionAutoTeachingSetting : public CDialogEx, public CVATMain_Common
{
	DECLARE_DYNAMIC(CVATMainTab_VisionAutoTeachingSetting)

public:
	CVATMainTab_VisionAutoTeachingSetting(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CVATMainTab_VisionAutoTeachingSetting();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VAT_MAIN_VISION_AUTO_TEACHING_SETTING_DLG };


public:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void PageMoveInit_Tab();
	void PageMoveDispose_Tab();

public: //OnTiemr
// 	afx_msg void OnTimer(UINT_PTR nIDEvent);
// 	afx_msg void OnDestroy();



private:
	//Gui에서 변경 된 Data 관리.
	St_VAT_VisionAutoTeachingInfo m_stVATInfoGui;

public:
	CShadeButtonST m_BtnOper_MakeBasePos;

	//Title Panel
	CDSSPanel m_ssDataPanel_1_Title;
	CDSSPanel m_ssDataPanel_2_Title;
	CDSSPanel m_ssDataPanel_3_Title;
	CDSSPanel m_ssDataPanel_4_Title;

	/////////Spread Sheet
	//[Hand Base Position]
	CDSSPanel m_ssDataPanel_1_Sub1;
	CDSSPanel m_ssDataPanel_1_Sub2;
	int m_nSS1_DisplayType;
	CSpreadSheet m_ssHandBasePos;
	_stVAT_BASE_POS_SHEET_INFO m_stHandBasePosLastInfo;
	CShadeButtonST m_BtnOper1_EncoderRead;
	CShadeButtonST m_BtnOper1_MotorMove;
	CShadeButtonST m_BtnOper1_Reload;
	CShadeButtonST m_BtnOper1_Save;

	//[Part Base Position]
	CDSSPanel m_ssPartBasePosBackColor;
	CDSSPanel m_ssDataPanel_2_Sub1;
	CDSSPanel m_ssDataPanel_2_Sub2;
	int m_nSS2_DisplayType;
	CSpreadSheet m_ssPartBasePos;
	_stVAT_BASE_POS_SHEET_INFO m_stPartBasePosLastInfo;
	CShadeButtonST m_BtnOper2_EncoderRead;
	CShadeButtonST m_BtnOper2_MotorMove;
	CShadeButtonST m_BtnOper2_Reload;
	CShadeButtonST m_BtnOper2_Save;
	
	//Initialize
	void InitSSTitlePanel();
	void InitVATOperBtn();
	void InitVATBasePosSheet();

	void DisplaySSData(int nVatSSType, int nHand, int nSSDisplayType);

	enum _eVatSSType
	{
		_eVatSS_HandBasePos = 0,
		_eVatSS_PartBasePos,

		_eVatSS_MAX,
	};


	BOOL CheckPosLimit(int nVatSSType);


public:
	DECLARE_EVENTSINK_MAP()
// 	void DataCopyGuiToReal(int nHand);
// 	void DataCopyRealToGui(int nHand);


	void ButtonClickedSpreadVatBasePosLoadTraypp(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadVatBasePosUnloadTraypp(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadVatBasePosLoadTestpp1(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadVatBasePosUnloadTestpp1(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadVatBasePosLoadTestpp2(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadVatBasePosUnloadTestpp2(long Col, long Row, short ButtonDown);

public: //[Hand Base Position]
	void ButtonClickHandChange(long Col, long Row, int nHand);

	void ClickSpreadHandBasePosition(long Col, long Row);
	void DblClickSpreadHandBasePosition(long Col, long Row);

	void ClickSspanelHandBasePosData();
	void ClickSspanelHandBasePosDataDiff();

	afx_msg void OnBnClickedBtnVatHandBasePosEncoderRead();
	afx_msg void OnBnClickedBtnVatHandBasePosMotorMove();
	afx_msg void OnBnClickedBtnVatHandBasePosReload();
	afx_msg void OnBnClickedBtnVatHandBasePosDataSave();

public:	//[Part Base Position]
	//VAT에서만 사용하는 Spread Sheet 관련 함수.
	//기존 Spread Sheet와 성향이 달라 _Com함수 활용에 제한이 있음.
	void ButtonClickPartChange();

	void ClickSpreadVatPartBasePosition(long Col, long Row);
	void DblClickSpreadVatPartBasePosition(long Col, long Row);

	void InitPosSpreadSheet_PartOnly(int nHand, int nSSPart);
	int ConvSSPartToVATPartIdx(int nHand, int nSSPart);
	CString ConvVATPartIdxToName(int nVATPart, int nPosIdx);

	BOOL PartBasePositionLimitCheck(int nVATPart, int nAxis, double dPos);

	BOOL ChkPartBasePosValid(int nHand, int nSSPart, int nPartBasePos);

	void ClickSspanelVatPartBasePosData();
	void ClickSspanelVatPartBasePosDataDiff();

	afx_msg void OnBnClickedBtnVatPartBasePosEncoderRead();
	afx_msg void OnBnClickedBtnVatPartBasePosMotorMove();
	afx_msg void OnBnClickedBtnVatPartBasePosReload();
	afx_msg void OnBnClickedBtnVatPartBasePosDataSave();

public: //[Test Site Y Axis Check List]	
	enum eVAT_Y_AXIS_CHECK_LIST_HAND
	{
		eYChkListHand_TestPp1 = 0,
		eYChkListHand_TestPp2,

		eYChkListHand_Max,
	};

	enum eYCheckList_Part
	{
		eYCheckList_Station = 0,
		eYCheckList_LDTable,
		eYCheckList_UDTable,

		eYCheckList_Part_Max,
	};

	enum MTPI_Y_CHECK_LIST
	{
		MTPI_TEST_SIDE_1 = 0,
		MTPI_TEST_SIDE_2,
		MTPI_TEST_SIDE_3,
		MTPI_TEST_SIDE_4,
	};

	CSpreadSheet m_ssSelectSheetSub[eYChkListHand_Max];
	int m_nYCheckListCurHand;
	void InitSubHandSelectSheet();

	CTabCtrl m_tabTestSiteYCheckList;
	int m_nYCheckListCurTab;
	void InitSubTabCtrl();

	CDSSPanel m_ssPanelYCheckList_HandStatus1;
	CDSSPanel m_ssPanelYCheckList_HandStatus2;

	CDSSPanel m_ssPanelYCheckList_Bmp;

	CDSSPanel m_ssPanelYCheckList_Status1;
	CDSSPanel m_ssPanelYCheckList_Status2;
	CDSSPanel m_ssPanelYCheckList_Status3;

	void InitTestSiteYAxisCheckList();

	void ButtonClickHandChange_YAxisCheckList(int nHand);
	void RefreshYAxisCheckListStatus();

	void DisplayYCheckListSSPanel();
	void DisplayYCheckListEdit();

	BOOL IsYAxisAllAlignWithStation(int nHand, int nSite);

	void ButtonClickedSpreadTestSiteYCheckHand1(long Col, long Row, short ButtonDown);
	void ButtonClickedSpreadTestSiteYCheckHand2(long Col, long Row, short ButtonDown);
	afx_msg void OnTcnSelchangeTabVatTestSiteYCheck(NMHDR *pNMHDR, LRESULT *pResult);

	void ClickSspanelYCheckListStatus1();
	void ClickSspanelYCheckListStatus2();
	void ClickSspanelYCheckListStatus3();

	void ClickSspanelYCheckListStatus(int nYCheckListPart);

public: //[Vision Target Overview]	
	void DisplayCurVisionTarget(int nHand, int nSSPart);


};
