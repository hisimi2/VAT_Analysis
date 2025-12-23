#pragma once


// CDlgSetupStocker 대화 상자입니다.

class CDlgSetupStocker : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetupStocker)

public:
	CDlgSetupStocker(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgSetupStocker();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_STACKER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
};
