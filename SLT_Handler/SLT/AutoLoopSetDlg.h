#pragma once


// CAutoLoopSetDlg 대화 상자입니다.

class CAutoLoopSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CAutoLoopSetDlg)

public:
	CAutoLoopSetDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAutoLoopSetDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOOP_SET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	void LoadData();
	void OnInitGUI();

	int m_nAutoLoopmode;
	char m_szCompany[16];
	void SiteSetChangeLog(std::vector<std::pair<CString, std::pair<CString, CString>>> vData);
public:
	CEditTrans m_editUnitCount;
	CEditTrans m_editTestCount;
	CEditTrans m_editLoopSotDelay;
	CEditTrans m_editLoopRePickPlaceCount;
	CEditTrans m_editLoopReContactCount;

	double m_dLoopSotDelay;


	int m_nAutoLoopTestCount;
	int m_nAutoLoopUnitCount;
	int m_nUnitCount;
	int m_nUnitRePickPlaceCount;
	int m_nUnitLoopTestContactCount;
	
	void MakeLog(LPCTSTR fmt, ...);

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSitesetSave();
	afx_msg void OnEnChangeEditNumberOfTimes();
	afx_msg void OnEnChangeEditLoopSotDelayTime();
	afx_msg void OnBnClickedRadioContiuneMode();
	afx_msg void OnBnClickedRadioRecontactMode();
	afx_msg void OnEnChangeEditNumberOfUnits();
	afx_msg void OnBnClickedRadioNormalMode();
	afx_msg void OnEnChangeEditRepickplaceCount();
	afx_msg void OnEnChangeEditLooptestCount();
	afx_msg void OnBnClickedRadioContiuneModeUsi();
	afx_msg void OnBnClickedRadioRecontactModeUsi();
};
