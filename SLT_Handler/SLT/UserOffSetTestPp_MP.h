#pragma once


// CUserOffSet 대화 상자입니다.

class CUserOffSetTestPp_MP : public CDialog
{
	DECLARE_DYNAMIC(CUserOffSetTestPp_MP)

public:
	CUserOffSetTestPp_MP(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUserOffSetTestPp_MP();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_USER_OFFSET_MP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void OnBntGUI();
	void OnEditBox();
	CShadeButtonST m_BtnSave;
	CShadeButtonST m_BtnCancel;
	CShadeButtonST m_BtnDefault;

	CBrush m_color;
	CEditTrans m_edit_X;
	CEditTrans m_edit_Y;
	CEditTrans m_edit_Z_Pick;
	CEditTrans m_edit_Z_Place;

	CEditTrans m_edit_RearSite_FrontPicker_X;
	CEditTrans m_edit_RearSite_FrontPicker_Y;
	CEditTrans m_edit_RearSite_FrontPicker_Z_Pick;
	CEditTrans m_edit_RearSite_FrontPicker_Z_Place;

	CEditTrans m_edit_FrontSite_FrontPicker_X;
	CEditTrans m_edit_FrontSite_FrontPicker_Y;
	CEditTrans m_edit_FrontSite_FrontPicker_Z_Pick;
	CEditTrans m_edit_FrontSite_FrontPicker_Z_Place;

	CEditTrans m_edit_LoadTable_2YRow_X;
	CEditTrans m_edit_LoadTable_2YRow_Y;
	CEditTrans m_edit_LoadTable_2YRow_Z_Pick;
	CEditTrans m_edit_LoadTable_2YRow_Z_Place;

	ST_USER_OFFSET_TESTPP_MP m_TeachTestPp;	
	int m_nRadioSel;

	int m_nRadioSel_TestPp;

	double m_dX;
	double m_dY;
	double m_dZ_Pick;
	double m_dZ_Place;

	double m_dRearSite_FrontPicker_X;
	double m_dRearSite_FrontPicker_Y;
	double m_dRearSite_FrontPicker_Z_Pick;
	double m_dRearSite_FrontPicker_Z_Place;

	double m_dFrontSite_FrontPicker_X;
	double m_dFrontSite_FrontPicker_Y;
	double m_dFrontSite_FrontPicker_Z_Pick;
	double m_dFrontSite_FrontPicker_Z_Place;

	double m_dLoadTable_2YRow_X;
	double m_dLoadTable_2YRow_Y;
	double m_dLoadTable_2YRow_Z_Pick;
	double m_dLoadTable_2YRow_Z_Place;

	char m_szCokName[512];
	void OnUpdateEditOffSet(int nIdx);
	void OnUpdateEditOffSet_LoadTable(int nIdx);
	void TestPpRadioButtonShow(int nIdx);
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CString getStrIndex(int nIndex);

	void MakeLog(LPCTSTR fmt, ...);
	void IsSameOffSet(ST_USER_OFFSET_TESTPP_MP TeachTestPpBefore, int nSiteAreaIdx);
	void InitControl();
	afx_msg void OnEnSetfocusEditX();
	afx_msg void OnEnSetfocusEditY();
	afx_msg void OnEnSetfocusEditZPick();
	afx_msg void OnEnSetfocusEditZPlace();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedRdSite1();
	afx_msg void OnBnClickedRdSite2();
	afx_msg void OnBnClickedRdSite3();
	afx_msg void OnBnClickedRdSite4();
	afx_msg void OnBnClickedRdSite5();
	afx_msg void OnBnClickedRdSite6();
	afx_msg void OnBnClickedRdSite7();
	afx_msg void OnBnClickedRdSite8();
	afx_msg void OnBnClickedRdSite9();
	afx_msg void OnBnClickedRdSite10();
	afx_msg void OnBnClickedRdSite11();
	afx_msg void OnBnClickedRdSite12();
	afx_msg void OnBnClickedRdSite13();
	afx_msg void OnBnClickedRdSite14();
	afx_msg void OnBnClickedRdSite15();
	afx_msg void OnBnClickedRdSite16();
	afx_msg void OnEnSetfocusEditRearsiteFrontpickerX();
	afx_msg void OnEnSetfocusEditRearsiteFrontpickerY();
	afx_msg void OnEnSetfocusEditRearsiteFrontpickerZPick();
	afx_msg void OnEnSetfocusEditRearsiteFrontpickerZPlace();
	afx_msg void OnEnSetfocusEditFrontsiteFrontpickerX();
	afx_msg void OnEnSetfocusEditFrontsiteFrontpickerY();
	afx_msg void OnEnSetfocusEditFrontsiteFrontpickerZPick();
	afx_msg void OnEnSetfocusEditFrontsiteFrontpickerZPlace();
	afx_msg void OnEnSetfocusEditLoadtableX();
	afx_msg void OnEnSetfocusEditLoadtableY();
	afx_msg void OnEnSetfocusEditLoadtableZPick();
	afx_msg void OnEnSetfocusEditLoadtableZPlace();
	afx_msg void OnBnClickedRdLdtbl1();
	afx_msg void OnBnClickedRdLdtbl2();
	afx_msg void OnBnClickedButtonDefault();
};
