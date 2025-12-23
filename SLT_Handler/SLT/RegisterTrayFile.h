#pragma once
#include "afxwin.h"


// CRegisterTrayFile 대화 상자입니다.

class CRegisterTrayFile : public CDialog
{
	DECLARE_DYNAMIC(CRegisterTrayFile)

public:
	CRegisterTrayFile(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRegisterTrayFile();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_REG_TRAYFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSave();

private:
	CShadeButtonST m_BtnSave;
	CShadeButtonST m_BtnCancel;

	CString m_strTrayFile;


	int m_nTrayDiv_X;
	int m_nTrayDiv_Y;
	double m_dTray_XPitch;
	double m_dTray_XStart;
	double m_dTray_YPitch;
	double m_dTray_YStart;	
	double m_dTrayThickness;

    int m_nLdTblDiv_X;
    int m_nLdTblDiv_Y;
	double m_dLdTbl_XPitch;
	double m_dLdTbl_YPitch;

	double m_dTestPP_ST_XPitch;
	double m_dTestPP_ST_YPitch;

// 	int m_nTestBuffTblDiv_X;
// 	int m_nTestBuffTblDiv_Y;
// 	double m_dTestBuffTbl_XPitch;
// 	double m_dTestBuffTbl_YPitch;

	int m_nTestCleanDiv_X;
	int m_nTestCleanDiv_Y;
	double m_dTestClean_XPitch;
	double m_dTestClean_YPitch;

    int m_nTestSiteDiv_X;
    int m_nTestSiteDiv_Y;
    double m_dTestSite_XPitch;
    double m_dTestSite_YPitch;

	double m_dTestPPSite_XPitch;
	double m_dTestPPSite_YPitch;

	double m_dTrayPPLtPitchX;
	double m_dTrayPPLtPitchY;

	double m_dTestPP_PitchX;
	double m_dTestPP_PitchY;

	int m_nTempMode;

	CEditTrans m_editYStartTray;
	CEditTrans m_editYPitchTray; 
	CEditTrans m_editXStartTray; 
	CEditTrans m_editXPitchTray; 
	CEditTrans m_editXDivTray;   
	CEditTrans m_editYDivTray;
	CEditTrans m_EditTrayThickness;

	CEditTrans m_editLdTbl_XPitch;  
	CEditTrans m_editLdTbl_YPitch;
	CEditTrans m_editLdTblDiv_X;
	CEditTrans m_editLdTblDiv_Y;

// 	CEditTrans m_editTestBuffTbl_XPitch;  
// 	CEditTrans m_editTestBuffTbl_YPitch;  
// 	CEditTrans m_editTestBuffTblDiv_X;
// 	CEditTrans m_editTestBuffTblDiv_Y;

	CEditTrans m_editCleanBuffTbl_XPitch;
	CEditTrans m_editCleanBuffTbl_YPitch;
	CEditTrans m_editCleanXDivBuffTbl;
	CEditTrans m_editCleanYDivBuffTbl;

    CEditTrans m_editXPitchTestSite;  
    CEditTrans m_editYPitchTestSite;  
    CEditTrans m_editTestXDivTestSite;
    CEditTrans m_editTestYDivTestSite;

	CEditTrans m_editXPitchTestppSite;
	CEditTrans m_editYPitchTestppSite;

	CEditTrans m_editTrayPPLtPitchX;
	CEditTrans m_editTrayPPLtPitchY;

	CEditTrans m_editTestPP_PitchX;
	CEditTrans m_editTestPP_PitchY;

	CEditTrans m_editTestPP_ST_XPitch;
	CEditTrans m_editTestPP_ST_YPitch;

	void OnBtnGUI();
	void OnEditBoxGUI();
public:
	afx_msg void OnEnSetfocusEditXstartTray();
	afx_msg void OnEnSetfocusEditXpitchTray();
	afx_msg void OnEnSetfocusEditYstartTray();
	afx_msg void OnEnSetfocusEditYpitchTray();
	afx_msg void OnEnSetfocusEditXDivTray();
	afx_msg void OnEnSetfocusEditYDivTray();
	afx_msg void OnEnSetfocusEditXpitchLdTbl();
	afx_msg void OnEnSetfocusEditYpitchLdTbl();
    afx_msg void OnEnSetfocusEditXDivLdTbl();
    afx_msg void OnEnSetfocusEditYDivLdTbl();
    afx_msg void OnEnSetfocusEditXpitchBuffTbl();
    afx_msg void OnEnSetfocusEditYpitchBuffTbl();
    afx_msg void OnEnSetfocusEditXDivBuffTbl();
    afx_msg void OnEnSetfocusEditYDivBuffTbl();
    afx_msg void OnEnSetfocusEditTrayThickness();
	void MakeLog(LPCTSTR fmt, ...);
	afx_msg void OnEnSetfocusEditXpitchTest1Site();
	afx_msg void OnEnSetfocusEditYpitchTest1Site();
	afx_msg void OnEnSetfocusEditXDivTest1Site();
	afx_msg void OnEnSetfocusEditYDivTest1Site();
	afx_msg void OnEnSetfocusEditXpitchClean1Tbl();
	afx_msg void OnEnSetfocusEditYpitchClean1Tbl();
	afx_msg void OnEnSetfocusEditXDivClean1Tbl();
	afx_msg void OnEnSetfocusEditYDivClean1Tbl();
	afx_msg void OnEnSetfocusEditTraypp1LtPitchX();
	afx_msg void OnEnSetfocusEditTraypp1LtPitchY();
	afx_msg void OnEnSetfocusEditTestppPitchX();
	afx_msg void OnEnSetfocusEditTestppPitchY();
	afx_msg void OnEnSetfocusEditXpitchTestPpSite();
	afx_msg void OnEnSetfocusEditYpitchTestPpSite();
};
