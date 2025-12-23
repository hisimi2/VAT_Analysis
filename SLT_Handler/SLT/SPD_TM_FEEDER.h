#pragma once


// CSPD_TM_FEEDER 대화 상자입니다.

class CSPD_TM_FEEDER : public CDialogEx
{
	DECLARE_DYNAMIC(CSPD_TM_FEEDER)

public:
	CSPD_TM_FEEDER(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSPD_TM_FEEDER();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SPD_TM_FEEDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

private:
	CShadeButtonST m_BtnSave;
	CShadeButtonST m_BtnCancel;

	int m_nSPD_Feeder_Y;
	int m_nACC_Feeder_Y;
	int m_nDCC_Feeder_Y;

	CEditTrans m_editSPD_Feeder_Y;
	CEditTrans m_editACC_Feeder_Y;
	CEditTrans m_editDCC_Feeder_Y;
	

	double m_dFeeder_UpDownWait;
	double m_dFeeder_ClampWait;
	double m_dFeeder_StepOffset;
	double m_dFeeder_AlignWait;

	CEditTrans m_editFeeder_UpDownWait;
	CEditTrans m_editFeeder_ClampWait;
	CEditTrans m_editFeeder_StepOffset;
	CEditTrans m_editFeeder_AlignOffset;

	void OnBtnGUI();
	void OnEditBoxGUI();
	
public:
	void MakeLog(LPCTSTR fmt, ...);
	void FeederSpeedTimeChangeLog(std::vector<std::pair<CString, std::pair<double, double>>> vData);

	afx_msg void OnEnSetfocusEditFeederCylUpdownWait();
	afx_msg void OnEnSetfocusEditFeederCylClampWait();
	afx_msg void OnEnSetfocusEditFeederStepMoveOffset();
	afx_msg void OnEnSetfocusEditSpdFeederY();
	afx_msg void OnEnSetfocusEditAccFeederY();
	afx_msg void OnEnSetfocusEditDccFeederY();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnEnSetfocusEditFeederCylAlignWait();
};
