#pragma once


// CCategorySetupBinDlg 대화 상자입니다.

class CCategorySetupBinDlg : public CDialog
{
	DECLARE_DYNAMIC(CCategorySetupBinDlg)

public:
	CCategorySetupBinDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCategorySetupBinDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_CATEGORY_SETUP_BIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	CShadeButtonST m_BtnCancel;
	CShadeButtonST m_BtnInsert;
	CShadeButtonST m_BtnDelete;
	CShadeButtonST m_BtnSave;
	CShadeButtonST m_BtnDefault;
	CSpreadSheet   m_SprdBinModfiy;
	void OnBtnGUI();
	void UpdateSpread();

	int m_nMaxRowNum;
	int m_nProjectName;
public:
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonInsert();
	void MakeLog(LPCTSTR fmt, ...);
	void CategorySetupChangeLog(std::vector<std::pair<CString,std::pair<CString,CString>>> vData);
	int checkCategoryInterlock();

	ST_CATE_INFO m_stCateData;
	CString      m_strCateName;
	afx_msg void OnBnClickedButtonOnlineDefault();
};
