#pragma once
#include "afxwin.h"


// CEngrDlg 대화 상자입니다.

class CEngrDlg : public CDialog
{
	DECLARE_DYNAMIC(CEngrDlg)

public:
	CEngrDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEngrDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAIN_DLG_ENGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	BOOL IsEQPRunning();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAlignment();
	afx_msg void OnBnClickedButtonIoMonitor();
	afx_msg void OnBnClickedButtonMotorMonitor();
	afx_msg void OnBnClickedButtonEventLog();

private:
	void OnBtnGUI();
public:
	CShadeButtonST m_BtnAlign;
	CShadeButtonST m_BtnIOMonitor;
	CShadeButtonST m_BtnMotorMonitor;
	CShadeButtonST m_BtnEventLog;
    CShadeButtonST m_BtnAIOMonitor;
    CShadeButtonST m_BtnManual;
	CShadeButtonST m_BtnGrrSetting;
	CShadeButtonST m_BtnGrrResult;
	CShadeButtonST m_BtnOffset;
	CShadeButtonST m_BtnVisionAutoTeaching;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnBnClickedButtonAioMonitor();
    afx_msg void OnBnClickedButtonManual();
    afx_msg void OnBnClickedButtonMotMonitor();
	afx_msg void OnBnClickedButtonGrrSetting();
	afx_msg void OnBnClickedButtonGrrResult();

	void MakeLog(LPCTSTR fmt, ...);
	afx_msg void OnEnSetfocusEditHddAlarm();
	afx_msg void OnEnSetfocusEditHddInterlock();
private:
	int m_nHddAlarmPer;
	int m_nHddInterlockPer;
	CEditTrans m_CtrlHddAlarm;
	CEditTrans m_CtrlHddInterlock;
	char m_szPathFileName[MAX_PATH];
public:
	afx_msg void OnTimer( UINT_PTR nIDEvent );
	void CompanySetting();
	afx_msg void OnBnClickedButtonOffset();
	afx_msg void OnBnClickedButtonVisionAutoTeaching();
};
