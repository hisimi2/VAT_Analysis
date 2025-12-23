#pragma once


// CDlgCommand_Param 대화 상자입니다.

class CDlgCommand_Param : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCommand_Param)

public:
	CDlgCommand_Param(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgCommand_Param();

	void SetParamName(std::vector<std::string> strParamNameList);
	void GetSelectedItem(CString& strSelectedItem);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CMD_PARAM };

private:
	std::vector<std::string> m_strParamNameList;
	std::vector<int> m_vPrmBtnCtrlId;
	CString m_strSelectedItem;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonParam1();
	afx_msg void OnBnClickedButtonParam2();
	afx_msg void OnBnClickedButtonParam3();
	afx_msg void OnBnClickedButtonParam4();
	afx_msg void OnBnClickedButtonParam5();
	afx_msg void OnBnClickedButtonParam6();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonParam7();
	afx_msg void OnBnClickedButtonParam8();
};
