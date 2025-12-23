#pragma once


// CDlgInputNum 대화 상자입니다.

class CDlgInputNum : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInputNum)

public:
	CDlgInputNum(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgInputNum();

	void GetValue(CString& strValue);

	CString m_strValue;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LIB_MC_INPUT_NUM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
};
