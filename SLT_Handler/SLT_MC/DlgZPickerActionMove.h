#pragma once


// CDlgZPickerActionMove 대화 상자입니다.

class CDlgZPickerActionMove : public CDialog
{
	DECLARE_DYNAMIC(CDlgZPickerActionMove)

public:
	CDlgZPickerActionMove(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgZPickerActionMove();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MANU_Z_PICKER_ACTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonManuMove();
	afx_msg void OnBnClickedButtonSetY1();
	afx_msg void OnBnClickedButtonSetY2();
	afx_msg void OnBnClickedButtonSetAll();
	afx_msg void OnBnClickedButtonSetClear();

	CButton m_ChkHand1_1;
	CButton m_ChkHand1_2;
	CButton m_ChkHand1_3;
	CButton m_ChkHand1_4;
	CButton m_ChkHand2_1;
	CButton m_ChkHand2_2;
	CButton m_ChkHand2_3;
	CButton m_ChkHand2_4;

	std::vector<CPoint> m_vPicker;

	int m_nAxisZNo;
	virtual BOOL OnInitDialog();

	BOOL IsAxisZTestPp();
};
