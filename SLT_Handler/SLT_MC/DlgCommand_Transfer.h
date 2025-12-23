#pragma once


// CDlgCommand_Transfer 대화 상자입니다.

class CDlgCommand_Transfer : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCommand_Transfer)

public:
	CDlgCommand_Transfer(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgCommand_Transfer();
	int GetSelectedSeqCmdId(){ return m_nSelectedSeqCmdId; };	


// 대화 상자 데이터입니다.
	enum { IDD = IDD_CMD_TRANSFER };
private:
	int m_nSelectedSeqCmdId;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonTransferCmdInit();
	afx_msg void OnBnClickedButtonTransferCmdMovePos();

	CBitmap m_cBmpInitial;
	CBitmap m_cBmpMovePos;
	CBitmap m_cBmpMovePickPlace;
	CBitmap m_cBmpClose;

	CButton m_btnInitial;
	CButton m_btnMovePos;
	CButton m_btnMovePickPlace;
	CButton m_btnClose;
	afx_msg void OnBnClickedButtonTransferCmdMovePickPlace();
	afx_msg void OnBnClickedCancel();
};
