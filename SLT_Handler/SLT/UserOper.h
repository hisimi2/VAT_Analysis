#pragma once
#include "afxwin.h"


// CUserOper 대화 상자입니다.

class CUserOper : public CDialog
{
	DECLARE_DYNAMIC(CUserOper)

public:
	CUserOper(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUserOper();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAIN_DLG_USER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL DestroyWindow();

private:
	CShadeButtonST m_BtnYieldCtrl;
	CShadeButtonST m_BtnCounter;
	CShadeButtonST m_BtnLotInfo;
	CShadeButtonST m_BtnSiteSet;
	CShadeButtonST m_BtnAutoSkip;
	CShadeButtonST m_BtnPrint;
	CShadeButtonST m_BtnFilePath;

	BOOL m_bAutoSkip;

	void OnBtnGUI();
	void OnBtnAutoSkipDisplay();
public:
	afx_msg void OnBnClickedButtonYield();
	afx_msg void OnBnClickedButtonSiteSet();
	afx_msg void OnBnClickedButtonCounter();
	afx_msg void OnBnClickedButtonLotInfo();
	afx_msg void OnBnClickedButtonPrint();
	afx_msg void OnBnClickedCheckAutoSkip();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void MakeLog(LPCTSTR fmt, ...);

	int m_nLogflag;
	afx_msg void OnBnClickedButtonFilepath();
	afx_msg void OnEnSetfocusEditAutoSkipCnt();
	CEditTrans m_CtrlAutoSkipCnt;
	int m_nAutoSkipCnt;
};
