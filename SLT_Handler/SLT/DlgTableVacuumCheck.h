#pragma once


// CDlgTableVacuumCheck 대화 상자입니다.

class CDlgTableVacuumCheck : public CDialog
{
	DECLARE_DYNAMIC(CDlgTableVacuumCheck)

public:
	CDlgTableVacuumCheck(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTableVacuumCheck();


	CSpreadSheet m_ssLoadTable_Part[eMaxLoadTblCount];
	CButton m_ChkTableVacuumCheckOnOff;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TABLE_VACUUM_CHECK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckTableVacuumCheckOnoff();
};
