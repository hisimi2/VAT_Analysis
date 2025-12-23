#pragma once


// CDailyReport 대화 상자입니다.

class CDailyReport : public CDialogEx
{
	DECLARE_DYNAMIC(CDailyReport)

public:
	CDailyReport(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDailyReport();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DAILY_REPORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedDateKind(UINT nID);
	void LoadData(CString strStartData, CString strEndDate);
	void InitData();
	void OnBtnGUI();
	void InitPartCount();
	void InitSpread();
	void LoadSpread();
	void Uploaddata(int nInit);

private:
	CDateTimeCtrl m_tStartDate;
	CDateTimeCtrl m_tEndDate;
	BOOL m_bSelectKind;
	
	ST_DAILY_REPORT m_stDailyData;
	
	CSpreadSheet m_SPRD_TotalData;
	CSpreadSheet m_SPRD_ARTYield;
	CSpreadSheet m_SPRD_PartJamCnt;

	int m_nJamCnt;
	CShadeButtonST m_BtnCancel;
	CShadeButtonST m_BtnApplay;
	CShadeButtonST m_BtnExpert;

	std::vector<ST_ERROR_MESSAGE> m_vDataListErrLog;
	ST_ERROR_MESSAGE m_stDataListErrLog;

	CCBSpeedListCtrl m_List;
	CButton m_ChkOnLine_OffLine_View;
	int m_nPartJamCnt[ePos_MaxPosition];

public:
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonExpert();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckDailyReportJamOnOffline();
	afx_msg LRESULT OnListDoubleClick(WPARAM wParam, LPARAM lParam);

};
