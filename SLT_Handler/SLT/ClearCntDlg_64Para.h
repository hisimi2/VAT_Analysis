#pragma once


// CClearCntDlg_64Para 대화 상자입니다.

class CClearCntDlg_64Para : public CDialogEx
{
	DECLARE_DYNAMIC(CClearCntDlg_64Para)

public:
	CClearCntDlg_64Para(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CClearCntDlg_64Para();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_CLEAR_COUNTER_64PARA };
	enum { eSocket1 = 0, eSocket2, eSocket3, eSocket4,
		   eSocket5, eSocket6, eSocket7, eSocket8 };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	BOOL OnInitDialog();
	void OnDestroy();
	void InitControl();
	void UpdateYieldData();
	void UpdateSiteNum();
	void ClearBinFunction( int nStationNo,int nSocketNo, int nDlgID );
	void ClearYieldFunction( int nStaionNo, int nSocketNo, int nDlgID );
	void MakeLog(LPCTSTR fmt, ...);
	void YieldModeChange(BOOL bYield);
	CComboBox m_cbStationName;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CShadeButtonST m_BtnCancel;
	CShadeButtonST m_BtnClearSite_1;
	CShadeButtonST m_BtnClearSite_2;
	CShadeButtonST m_BtnClearSite_3;
	CShadeButtonST m_BtnClearSite_4;
	CShadeButtonST m_BtnClearSite_5;
	CShadeButtonST m_BtnClearSite_6;
	CShadeButtonST m_BtnClearSite_7;
	CShadeButtonST m_BtnClearSite_8;
	CShadeButtonST m_BtnClearSiteAll;

	CShadeButtonST m_BtnClearSite_1_Yield;
	CShadeButtonST m_BtnClearSite_2_Yield;
	CShadeButtonST m_BtnClearSite_3_Yield;
	CShadeButtonST m_BtnClearSite_4_Yield;
	CShadeButtonST m_BtnClearSite_5_Yield;
	CShadeButtonST m_BtnClearSite_6_Yield;
	CShadeButtonST m_BtnClearSite_7_Yield;
	CShadeButtonST m_BtnClearSite_8_Yield;
	CShadeButtonST m_BtnClearSiteAll_Yield;
	CShadeButtonST m_BtnClear_TotalCount;

	CButton m_rbChkYield;
	CButton m_rbChk1stFail;

	std::vector<int> m_vDlgID;
	std::vector<int> m_vBtnID;
	std::vector<int> m_vDlgID_Yield;
	std::vector<int> m_vBtnID_Yield;

	std::vector<int> m_vGroupBoxID;
	std::vector<int> m_vGroupBox_Yield;

	int m_TestSite_Div_X;
	int m_TestSite_Div_Y;

	int m_nYieldMode;

	afx_msg void OnCbnSelchangeComboStationList();
	afx_msg void OnBnClickedButtonClear1();
	afx_msg void OnBnClickedButtonClear2();
	afx_msg void OnBnClickedButtonClear3();
	afx_msg void OnBnClickedButtonClear4();
	afx_msg void OnBnClickedButtonClear5();
	afx_msg void OnBnClickedButtonClear6();
	afx_msg void OnBnClickedButtonClear7();
	afx_msg void OnBnClickedButtonClear8();
	afx_msg void OnBnClickedButtonClearAll();
	afx_msg void OnBnClickedButtonClearAllMonitoring();

	afx_msg void OnBnClickedButtonClearYield1();
	afx_msg void OnBnClickedButtonClearYield2();
	afx_msg void OnBnClickedButtonClearYield3();
	afx_msg void OnBnClickedButtonClearYield4();
	afx_msg void OnBnClickedButtonClearYield5();
	afx_msg void OnBnClickedButtonClearYield6();
	afx_msg void OnBnClickedButtonClearYield7();
	afx_msg void OnBnClickedButtonClearYield8();
	afx_msg void OnBnClickedButtonClearTotalCount();
	afx_msg void OnBnClickedRadioYield();
	afx_msg void OnBnClickedRadio1stFail();
};
