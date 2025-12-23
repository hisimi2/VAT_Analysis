#pragma once
#include "afxwin.h"


// CTrayFileSetupDlg 대화 상자입니다.

class CTrayFileSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(CTrayFileSetupDlg)

public:
	CTrayFileSetupDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTrayFileSetupDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TRAYFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonNew();

private:
	CShadeButtonST m_BtnBaseSet;
	CShadeButtonST m_BtnDel;
	CShadeButtonST m_BtnKitChange;
	CShadeButtonST m_BtnNew;
	CShadeButtonST m_BtnSave;
	CShadeButtonST m_BtnCancel;

	CString m_strTrayFile;
	CString m_strGetSelectListbox;
	CListBox m_ListTrayFile;

	int m_nTrayDiv_X;
	int m_nTrayDiv_Y;
	double m_dTray_XPitch;
	double m_dTray_XStart;
	double m_dTray_YPitch;
	double m_dTray_YStart;	
	double m_dTray_Thickness;

    int m_nLdTblDiv_X;
    int m_nLdTblDiv_Y;
	double m_dLdTbl_XPitch;
	double m_dLdTbl_YPitch;

	int m_nTestCleanDiv_X;
	int m_nTestCleanDiv_Y;
	double m_dTestClean_XPitch;
	double m_dTestClean_YPitch;

	double m_dTestPP_ST_XPitch;
	double m_dTestPP_ST_YPitch;

	int m_nTestSiteDiv_X;
	int m_nTestSiteDiv_Y;
	double m_dTestSite_XPitch;
	double m_dTestSite_YPitch;

	double m_dTrayPPLtPitchX;
	double m_dTrayPPLtPitchY;

	double m_dTestPP_PitchX;
	double m_dTestPP_PitchY;

	int m_nTempMode;
	int m_nTrayFile;

	CEditTrans m_editYStartTray;
	CEditTrans m_editYPitchTray; 
	CEditTrans m_editXStartTray; 
	CEditTrans m_editXPitchTray; 
	CEditTrans m_editXDivTray;   
	CEditTrans m_editYDivTray;  
    CEditTrans m_editXDivLdTbl;  
    CEditTrans m_editYDivLdTbl;  
	CEditTrans m_editLdTbl_XPitch;  
	CEditTrans m_editLdTbl_YPitch; 
	CEditTrans m_EditTrayThickness;

	CEditTrans m_editCleanXDivBuffTbl;
	CEditTrans m_editCleanYDivBuffTbl;
	CEditTrans m_editCleanBuffTbl_XPitch;
	CEditTrans m_editCleanBuffTbl_YPitch;

	CEditTrans m_editTestPP_ST_XPitch;
	CEditTrans m_editTestPP_ST_YPitch;

	//Test SIte
	CEditTrans m_editTestXDivTestSite;
	CEditTrans m_editTestYDivTestSite;
	CEditTrans m_editXPitchTestSite;
	CEditTrans m_editYPitchTestSite;

	CEditTrans m_editTrayPPLtPitchX;
	CEditTrans m_editTrayPPLtPitchY;

	CEditTrans m_editTrayPP_PitchX;
	CEditTrans m_editTrayPP_PitchY;

	void OnBtnGUI();
	void InitTrayData(CString strRevName);
	void InitListData();
	void OnEditBoxGUI();

	ST_COK_INFO   m_stCokInfo;
	ST_BASIC_INFO m_stBasicInfo;
public:
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonDelete();	
	afx_msg void OnLbnSelchangeListTrayfile();	
	afx_msg void OnEnSetfocusEditXstartTray();
	afx_msg void OnEnSetfocusEditXpitchTray();
	afx_msg void OnEnSetfocusEditYstartTray();
	afx_msg void OnEnSetfocusEditYpitchTray();
	afx_msg void OnEnSetfocusEditXDivTray();
	afx_msg void OnEnSetfocusEditYDivTray();
	afx_msg void OnEnSetfocusEditXpitchLdTbl();
	afx_msg void OnEnSetfocusEditYpitchLdTbl();	
	afx_msg void OnEnSetfocusEditTrayThickness();
    afx_msg void OnEnSetfocusEditXDivLdTbl();
    afx_msg void OnEnSetfocusEditYDivLdTbl();
	void MakeLog(LPCTSTR fmt, ...);
	// vData.first = stCokData, vData.second.first = BeforeData vData.second.second = AfterData
	void TrayFileSetupChangeLog(std::vector<std::pair<CString,std::pair<double,double>>> vData);
	BOOL m_bDvcHorVer;

	afx_msg void OnEnSetfocusEditXpitchTestSite();
	afx_msg void OnEnSetfocusEditYpitchTestSite();
	afx_msg void OnEnSetfocusEditXDivTestSite();
	afx_msg void OnEnSetfocusEditYDivTestSite();
	afx_msg void OnEnSetfocusEditTrayppLtPitchX();
	afx_msg void OnEnSetfocusEditTrayppLtPitchY();
	afx_msg void OnEnSetfocusEditXpitchCleanTbl();
	afx_msg void OnEnSetfocusEditYpitchCleanTbl();
	afx_msg void OnEnSetfocusEditXDivCleanTbl();
	afx_msg void OnEnSetfocusEditYDivCleanTbl();
	afx_msg void OnEnSetfocusEditTestppPitchX1();
	afx_msg void OnEnSetfocusEditTestppPitchY1();
	afx_msg void OnEnSetfocusEditTestppStPitchX();
	afx_msg void OnEnSetfocusEditTestppStPitchY();
};
