#pragma once


// CDlgCommand_TestPP 대화 상자입니다.

class CDlgCommand_TestPP : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCommand_TestPP)

public:
	CDlgCommand_TestPP(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgCommand_TestPP();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CMD_TEST_PP };

	int GetSelectedSeqCmdId(){ return m_nSelectedSeqCmdId; };	
private:
	int m_nSelectedSeqCmdId;
	int m_nManuOper;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStackerCmdInit();
	afx_msg void OnBnClickedButtonStackerCmdMovePos();
	afx_msg void OnBnClickedButtonStackerCmdMovePickPlace();
	afx_msg void OnBnClickedButtonStackerCmdEmptyCheck();

	std::vector<int> m_vSelectCmdPicker;
	int m_nSelectedSeqCmdParam[10];
	afx_msg void OnBnClickedButtonStackerCmdMoveExchange();

	CBitmap m_cBmpInitial;
	CBitmap m_cBmpMovePos;
	CBitmap m_cBmpPickPlaceMove;
	CBitmap m_cBmpExchangeMove;
	CBitmap m_cBmpEmptyCheck;
	CBitmap m_cBmpClose;

	CButton m_btnInitial;
	CButton m_btnMovePos;
	CButton m_btnPickPlaceMove;
	CButton m_btnExchangeMove;
	CButton m_btnEmptyCheck;
	CButton m_btnClose;

	void MakeLog(LPCTSTR fmt, ...);
	afx_msg void OnBnClickedButtonStackerCmd4x2samepickplace();
};
