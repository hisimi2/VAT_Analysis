#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"
#include "EventLog_List.h"
#include "EventLog_Frequency.h"
#include "EventLog_Teach.h"

// CEventLogDlg 대화 상자입니다.

class CEventLogDlg : public CDialog
{
	DECLARE_DYNAMIC(CEventLogDlg)

public:
	CEventLogDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEventLogDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_EVENTLOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	static std::vector<ST_ERROR_MESSAGE> vDataErrLog;
	static std::vector<ST_TEACH_LOG> vDataTeachLog;

private:
	CShadeButtonST m_BtnCancel;

	CShadeButtonST m_BtnAllLog;
	CShadeButtonST m_BtnLogLevel0;
	CShadeButtonST m_BtnLogLevel1;
	CShadeButtonST m_BtnLogLevel2;
	CShadeButtonST m_BtnLogLevel3;
	CShadeButtonST m_BtnLogExpert;
	CShadeButtonST m_BtnLogDisplay;

	bool m_bSortAscending;	// 오름차순, 내림차순 판단

	void OnBtnGUI();
	void LoadDB_ErrMsg(int nSelectAlarm);
	void InitListGUI();
	void LoadDB_Teach(int nSelectAlarm);
	void UIShowHide(int nTabType);
	CTabCtrl m_tabEvent;

	CEventLog_List	m_pgEventLog;
	CEventLog_Frequency m_pgEventFrequency;
	CEventLog_Teach	m_pgEventTeachLog;

	void WritePartName(int nPartNum,CString& strPartName);
	void MakeLotHistoryLog(CString strStartTm, CString strEndTm);
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonAllLog();
	afx_msg void OnBnClickedButtonLogLevel0();
	afx_msg void OnBnClickedButtonLogLevel1();
	afx_msg void OnBnClickedButtonLogLevel2();
	afx_msg void OnBnClickedButtonLogLevel3();
	
	CDateTimeCtrl m_tStartDate;
	CDateTimeCtrl m_tStartTime;
	CDateTimeCtrl m_tEndDate;
	CDateTimeCtrl m_tEndTime;
	afx_msg void OnBnClickedButtonExpert();
	int m_nErrLevel;
	afx_msg void OnBnClickedButtonDisplay();
	void OnInitDialog_MainTabPages();
	afx_msg void OnTcnSelchangeTabLog(NMHDR *pNMHDR, LRESULT *pResult);
	void MakeLog(LPCTSTR fmt, ...);
};
