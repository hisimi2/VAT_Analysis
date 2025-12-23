#pragma once
#include "afxwin.h"

class CCCDSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(CCCDSetupDlg)

public:
	CCCDSetupDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCCDSetupDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_CCD_SETUP };
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClicked2dBarcode();
	afx_msg void OnBnClickedTestsiteUp();
	afx_msg void OnBnClickedTestsiteDown();
private:
	void OnBtnGUI();
	CShadeButtonST m_BtnCancel;
	CShadeButtonST m_Btn2Dbarcode;
	CShadeButtonST m_BtnTestUpSite;
	CShadeButtonST m_BtnTestDownSite;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
};

