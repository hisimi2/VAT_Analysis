#pragma once


// CDlgCommand_Feeder 대화 상자입니다.

class CDlgCommand_Feeder : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCommand_Feeder)

public:
	CDlgCommand_Feeder(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgCommand_Feeder();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMD_FEEDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	int m_nSelectedSeqCmdId;

public:
	int GetSelectedSeqCmdId() { return m_nSelectedSeqCmdId; };

	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	CBitmap m_cBmpInitial;
	CBitmap m_cBmpMovePos;
	CBitmap m_cBmpClose;
	CBitmap m_cBmpPickPlaceMove;

	CButton m_btnInitial;
	CButton m_btnMovePos;
	CButton m_btnClose;
	CButton m_btnPickPlaceMove;

	afx_msg void OnBnClickedButtonFeederCmdInit();
	afx_msg void OnBnClickedButtonFeederCmdMovePos();
	afx_msg void OnBnClickedButtonFeederCmdMovePickPlace();
	virtual BOOL OnInitDialog();
};
