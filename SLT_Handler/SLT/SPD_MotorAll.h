#pragma once
#include "afxwin.h"


// CSPD_MotorAll 대화 상자입니다.

class CSPD_MotorAll : public CDialog
{
	DECLARE_DYNAMIC(CSPD_MotorAll)

public:
	CSPD_MotorAll(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSPD_MotorAll();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SPD_ALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	CShadeButtonST m_BtnSave;
	CShadeButtonST m_BtnCancel;

	CEditTrans m_editSpeed;
	CEditTrans m_editAccel;
	CEditTrans m_editDecel;

	void OnBtnGUI();
	void OnEditBoxGUI();

	BOOL m_bTrayPP;
	BOOL m_bTestPP;
	BOOL m_bTransfer;
	BOOL m_bLoadTbl;
	BOOL m_bTrayPP_Z;
	BOOL m_bTestPP_Z;
	BOOL m_bPressUnit;
	BOOL m_bTrayPP_Pitch;
	BOOL m_bTestPP_Pitch;
	BOOL m_bFeeder;

	BOOL m_bAllUnitSelect;
	std::vector<int> m_vButtonIDs;

	int m_nSpeed;
	int m_nAccel;
	int m_nDecel;
public:
	afx_msg void OnBnClickedButtonSave();	
	afx_msg void OnEnSetfocusEditSpd();
	afx_msg void OnEnSetfocusEditAcc();
	afx_msg void OnEnSetfocusEditDcc();
	void MakeLog(LPCTSTR fmt, ...);
	void SpeedAllChangeLog();
	afx_msg void OnBnClickedChkAllSelect();
};
