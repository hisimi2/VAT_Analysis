#pragma once


// CRegisterCustomer 대화 상자입니다.

class CRegisterCustomer : public CDialog
{
	DECLARE_DYNAMIC(CRegisterCustomer)

public:
	CRegisterCustomer(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRegisterCustomer();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_REG_CUSTOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSave();
	void MakeLog(LPCTSTR fmt, ...);

private:
	CShadeButtonST m_BtnCancel;
	CShadeButtonST m_BtnSave;

	void OnBtnGUI();
};
