#pragma once


// CDlgContactAndSafety 대화 상자입니다.

class CDlgContactAndSafety : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgContactAndSafety)

public:
	CDlgContactAndSafety(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgContactAndSafety();
	CDSSPanel m_SSPANELTitle;
	CDSSPanel m_SSPANELTime;
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_CONTACT_AND_SAFETY_MOVING };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
