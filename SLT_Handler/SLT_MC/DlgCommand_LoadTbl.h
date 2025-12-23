#pragma once


// CDlgCommand_LoadTbl 대화 상자입니다.

class CDlgCommand_LoadTbl : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCommand_LoadTbl)

public:
	CDlgCommand_LoadTbl(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgCommand_LoadTbl();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CMD_LOAD_TABLE };

	int GetSelectedSeqCmdId(){ return m_nSelectedSeqCmdId; };	
private:
	int m_nSelectedSeqCmdId;
	int m_nManuOper;		// enum eMANU_OPER_IDX_POS

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonLoadTblCmdInit();
	afx_msg void OnBnClickedButtonLoadTblCmdMovePos();

	CBitmap m_cBmpInitial;
	CBitmap m_cBmpMovePos;
	CBitmap m_cBmpClose;

	CButton m_btnInitial;
	CButton m_btnMovePos;
	CButton m_btnClose;
};
