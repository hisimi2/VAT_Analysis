#pragma once
#include "afxwin.h"
#include "SLTView.h"
// CSetupDlg 대화 상자입니다.

class CSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetupDlg)

public:
	CSetupDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAIN_DLG_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	BOOL CheckEQPRunning();

public:
	virtual BOOL OnInitDialog();

private:
	CShadeButtonST m_BtnOffset;
	CShadeButtonST m_BtnSpeedAll;
	CShadeButtonST m_BtnSpeed_TrayPP;
	CShadeButtonST m_BtnSpeed_TestPP;
	CShadeButtonST m_Btn2dBarcodeValidate;
	CShadeButtonST m_BtnSpeed_LoadTbl;
	CShadeButtonST m_BtnSpeed_PressUnit;
	CShadeButtonST m_BtnSiteSetting;
	CShadeButtonST m_BtnCategorySetup;
	CShadeButtonST m_BtnIFSetting;
	CShadeButtonST m_BtnDvcSetup;
	CShadeButtonST m_BtnSLTPgm;
	CShadeButtonST m_BtnYieldCtrl;
	CShadeButtonST m_BtnIOMonitor;
	CShadeButtonST m_BtnPosCheck;
	CShadeButtonST m_BtnEventLog;
	CShadeButtonST m_BtnChangeFile;
	CShadeButtonST m_BtnTransfer;
	CShadeButtonST m_BtnYieldMonitor;
	CShadeButtonST m_BtnVibrator;
	CShadeButtonST m_BtnVision;

    CShadeButtonST m_BtnSocketClean;

	CShadeButtonST m_BtnTower;
	CShadeButtonST m_BtnDataAssign;

	CShadeButtonST m_BtnAutoSkip;
	CShadeButtonST m_BtnSocketCount;
	CShadeButtonST m_BtnDailyReport;
	CShadeButtonST m_BtnGrrSetting;

	CShadeButtonST m_BtnLoopSetting;


	CShadeButtonST m_BtnSpeed_Feeder;

	CShadeButtonST m_BtnQASetting;
	

	CShadeButtonST m_BtnBarcodeSetting;
	CShadeButtonST m_BtnBarcodeValidate;

	CEditTrans m_CtrlAutoSkipCnt;
	
	BOOL m_bAutoSkip;
	BOOL m_bBeforeAutoSkip;
	int m_nLogflag;
	int m_nAutoSkipCnt;

	void OnBtnGUI();
    void UpdateTrayFileName();	
	BOOL IsLotOnGoing();
public:
	afx_msg void OnBnClickedButtonOffset();
	afx_msg void OnBnClickedButtonSpeed();
	afx_msg void OnBnClickedButtonSiteSet();
	afx_msg void OnBnClickedButtonCateSetup();
	afx_msg void OnBnClickedButtonIfTest();
	afx_msg void OnBnClickedButtonDeviceSetup();
	afx_msg void OnBnClickedButtonSltPgm();
	afx_msg void OnBnClickedButtonYieldCtrl();
	afx_msg void OnBnClickedButtonIoMonitor();
	afx_msg void OnBnClickedButtonPosCheck();
	afx_msg void OnBnClickedButtonEventLog();	
	afx_msg void OnBnClickedButtonSpeedShuttle();
	afx_msg void OnBnClickedButtonSpeedContactor();
	afx_msg void OnBnClickedButtonSpeedTestPp();
	afx_msg void OnBnClickedButtonSpeedAll();	
	afx_msg void OnBnClickedButtonSpeedTansfer();
    afx_msg void OnBnClickedButtonYieldMonitor();
    afx_msg void OnBnClickedButtonCcdSetup();
    afx_msg void OnBnClickedButtonVibrator();
    afx_msg void OnBnClickedButtonAioMonitor();
    afx_msg void OnBnClickedButtonSocketClean();	
	afx_msg void OnBnClickedButtonTraySetup();

	virtual BOOL PreTranslateMessage(MSG* pMsg);	
	afx_msg void OnBnClickedButtonFtp();
	afx_msg void OnBnClickedButtonTower();
	afx_msg void OnBnClickedButtonDataAssign();

	void MakeLog(LPCTSTR fmt, ...);
	afx_msg void OnBnClickedButtonOffsetTray();
	afx_msg void OnBnClickedButtonTrayAssign();
	afx_msg void OnBnClickedCheckAutoSkip2();
	afx_msg void OnEnSetfocusEditAutoSkipCnt2();

	void OnBtnAutoSkipDisplay();
	afx_msg void OnBnClickedButtonSocketCount();
	afx_msg void OnBnClickedButtonDailyReport();
	afx_msg void OnBnClickedButtonGrrSettingSetup();
	
	void CompanySetting();
	afx_msg void OnBnClickedButtonCbDataOpstion();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonLoopSettingSetup();
	afx_msg void OnBnClickedButtonAuditSettingSetup();
	afx_msg LRESULT	OnReceivedMsg(WPARAM wParam, LPARAM lparam);
	afx_msg void OnBnClickedButtonIonizer();
	afx_msg void OnBnClickedButtonSpeedFeeder();
	afx_msg void OnBnClickedButtonBarcodeSetting();
	afx_msg void OnBnClickedButton2dBarcodeValidate();
	afx_msg void OnBnClickedButtonQaSettingSetup();
};
