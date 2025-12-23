#pragma once


// CDlgCylinderTimeCheck 대화 상자입니다.

class CDlgCylinderTimeCheck : public CDialog
{
	DECLARE_DYNAMIC(CDlgCylinderTimeCheck)

public:
	CDlgCylinderTimeCheck(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgCylinderTimeCheck();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MANU_CYLINDER_TIME_CHECK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonStart();
	CButton m_ChkStation;
	CButton m_ChkStacker;
	CButton m_ChkLoadTable;
	CButton m_ChkTransfer;

	CSpreadSheet m_SprdCylinderData;
	CSpreadSheet m_SprdCylinderData2;

	void SpreadSheetInit();
	void UpdateSpread();
	void UIUpdate();
	void CylinderTimeDataUpdate();

	CFont m_font;

	CEditTrans m_editDelayTime;
	double m_dDelayTime;

	CBitmap m_cBmpStart;
	CButton m_btnStart;

	CBitmap m_cBmpExit;
	CButton m_btnExit;

	std::vector<ST_CYLINDER_TIME_DATA> m_veCylinderData;
	std::vector<_tCYLINDER_TIME> m_veCylinderTime;
	afx_msg void OnBnClickedButtonDelayTimeset();
	afx_msg void OnEnSetfocusEditDelayTime();
};
