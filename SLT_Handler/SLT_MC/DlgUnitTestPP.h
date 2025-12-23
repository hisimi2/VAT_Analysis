#pragma once


// CDlgUnitTestPP 대화 상자입니다.

class CDlgUnitTestPP : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgUnitTestPP)

public:
	CDlgUnitTestPP(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgUnitTestPP();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_UNIT_TEST_PP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
