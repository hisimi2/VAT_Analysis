#pragma once


// CDlgCommand_Stacker 대화 상자입니다.

class CDlgCommand_Stacker : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCommand_Stacker)

public:
	CDlgCommand_Stacker(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgCommand_Stacker();

	int GetSelectedSeqCmdId();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CMD_STACKER };

public:
	int m_nManuOper;			// enum eMANU_OPER_IDX_POS
	int m_nSelectedSeqCmdId;	// enum eSeqCmdId
	int m_nSelectedSeqCmdParam[10];
	int m_nStackerIdx;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStackerCmdInit();

	CBitmap m_cBmpInitial;
	CBitmap m_cBmpLoad;
	CBitmap m_cBmpTrayDown;
	CBitmap m_cBmpTrayUp;
	CBitmap m_cBmpUnload;
	CBitmap m_cBmpTrayOut;
	CBitmap m_cBmpVibration;
	CBitmap m_cBmpClose;

	CButton m_btnInitial;
	CButton m_btnLoad;
	CButton m_btnTrayDown;
	CButton m_btnTrayUp;
	CButton m_btnUnload;
	CButton m_btnTrayOut;
	CButton m_btnVibration;
	CButton m_btnClose;
	afx_msg void OnBnClickedButtonStackerCmdLoad();
	afx_msg void OnBnClickedButtonStackerCmdTraydown();
	afx_msg void OnBnClickedButtonStackerCmdTrayup();
	afx_msg void OnBnClickedButtonStackerCmdUnload();
	afx_msg void OnBnClickedButtonStackerCmdVibration();
	afx_msg void OnBnClickedButtonStackerCmdTrayOut();
};
