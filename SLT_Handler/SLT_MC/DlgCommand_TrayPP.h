#pragma once


// CDlgCommand_TrayPP 대화 상자입니다.

class CDlgCommand_TrayPP : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCommand_TrayPP)

public:
	CDlgCommand_TrayPP(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgCommand_TrayPP();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CMD_TRAY_PP };

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
	afx_msg void OnBnClickedButtonTrayPpCmdInit();
	afx_msg void OnBnClickedButtonTrayPpCmdMovePos();
	afx_msg void OnBnClickedButtonTrayPpCmdPickPlace();
	afx_msg void OnBnClickedButtonTrayPpCmdEmptyCheck();
	afx_msg void OnBnClickedButtonTrayPpCmdMovePickPlace();

	CBitmap m_cBmpInitial;
	CBitmap m_cBmpMovePos;
	CBitmap m_cBmpPickPlace;
	CBitmap m_cBmpPickPlaceMove;
	CBitmap m_cBmpPickerHeightCal;
	CBitmap m_cBmpEmptyCheck;
	CBitmap m_cBmpClose;

	CButton m_btnInitial;
	CButton m_btnMovePos;
	CButton m_btnPickPlace;
	CButton m_btnPickPlaceMove;
	CButton m_btnPickerHeightCal;
	CButton m_btnEmptyCheck;
	CButton m_btnClose;
	afx_msg void OnBnClickedButtonTrayPpCmdAuotoHeightCal();
};
