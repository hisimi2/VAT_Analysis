#pragma once


// CAutoQASetDlg 대화 상자입니다.

class CAutoQASetDlg : public CDialog
{
	DECLARE_DYNAMIC(CAutoQASetDlg)

public:
	CAutoQASetDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAutoQASetDlg();

	int m_nUnitCount;
	int m_nBeforeUnitCount;
	CEditTrans m_editUnitCount;

	void MakeLog(LPCTSTR fmt, ...);
	void SiteSetChangeLog(std::vector<std::pair<CString, std::pair<CString, CString>>> vData);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_QA_MODE_SET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSitesetSave();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnSetfocusEditNumberOfUnits();
	virtual BOOL OnInitDialog();
};
