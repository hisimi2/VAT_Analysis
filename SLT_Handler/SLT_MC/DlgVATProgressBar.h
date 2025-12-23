#pragma once


// CDlgVATProgressBar 대화 상자입니다.

class CDlgVATProgressBar : public CDialog
{
	DECLARE_DYNAMIC(CDlgVATProgressBar)

public:
	CDlgVATProgressBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgVATProgressBar();

	CEditTrans m_editPosition[eVATPos_MaxPosition];
	CEditTrans m_editPositionM;
	BOOL bIsRed[eVATPos_MaxPosition];
	CStatic m_bitPic1;

	void InitPosition();
	void InitReSize();
	void StatusChange(_eVATProgressStatus __eVATProgressStatus);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VAT_PROGRESS_BAR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};
