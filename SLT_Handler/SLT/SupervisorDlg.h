#pragma once


// CSupervisorDlg 대화 상자입니다.

class CSupervisorDlg : public CDialog
{
	DECLARE_DYNAMIC(CSupervisorDlg)

public:
	CSupervisorDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSupervisorDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAIN_DLG_SUPER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	void OnBtnGUI();
	CShadeButtonST m_BtnUserReg;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnRegUser();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void MakeLog(LPCTSTR fmt, ...);
};
