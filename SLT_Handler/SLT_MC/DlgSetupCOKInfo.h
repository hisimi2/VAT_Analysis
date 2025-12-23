#pragma once


// CDlgSetupCOKInfo 대화 상자입니다.

class CDlgSetupCOKInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetupCOKInfo)

public:
	CDlgSetupCOKInfo(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgSetupCOKInfo();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_COK_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnLbnSelchangeListTrayfile();	
	afx_msg void OnEnSetfocusEditXstartTray();
	afx_msg void OnEnSetfocusEditXpitchTray();
	afx_msg void OnEnSetfocusEditYstartTray();
	afx_msg void OnEnSetfocusEditYpitchTray();
	afx_msg void OnEnSetfocusEditXDivTray();
	afx_msg void OnEnSetfocusEditYDivTray();
	afx_msg void OnEnSetfocusEditXstartPlate();
	afx_msg void OnEnSetfocusEditXpitchPlate();
	afx_msg void OnEnSetfocusEditYstartPlate();
	afx_msg void OnEnSetfocusEditYpitchPlate();
	afx_msg void OnEnSetfocusEditXDivPlate();
	afx_msg void OnEnSetfocusEditYDivPlate();

private:
	int m_nTempMode;

	int m_nPlateDiv_X;
	int m_nPlateDiv_Y;
	double m_dPlate_XPitch;
	double m_dPlate_XStart;
	double m_dPlate_YPitch;
	double m_dPlate_YStart;

	int m_nTrayDiv_X;
	int m_nTrayDiv_Y;
	double m_dTray_XPitch;
	double m_dTray_XStart;
	double m_dTray_YPitch;
	double m_dTray_YStart;	
	double m_dTray_Thickness;
	double m_dLdTbl_XPitch;
	double m_dLdTbl_YPitch;
public:
	afx_msg void OnEnSetfocusEditXpitchLdTbl();
	afx_msg void OnEnSetfocusEditYpitchLdTbl();
	afx_msg void OnEnSetfocusEditCtrayThickness();

	CBitmap m_cBmpBaseSetting;
	CBitmap m_cBmpLoad;
	CBitmap m_cBmpSave;
	CBitmap m_cBmpExit;

	CButton m_btnBaseSetting;
	CButton m_btnLoad;
	CButton m_btnSave;
	CButton m_btnExit;
};
