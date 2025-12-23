#pragma once


// CDlgUnitCTrayTransferSpeed 대화 상자입니다.

class CDlgUnitCTrayTransferSpeed : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgUnitCTrayTransferSpeed)

public:
	CDlgUnitCTrayTransferSpeed(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgUnitCTrayTransferSpeed();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_UNIT_CTRAY_XFER_SPEED };

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
