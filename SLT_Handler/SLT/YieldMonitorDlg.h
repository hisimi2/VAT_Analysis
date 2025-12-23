#pragma once
#include "afxwin.h"

// CYieldMonitorDlg 대화 상자입니다.

class CYieldMonitorDlg : public CDialog
{
	DECLARE_DYNAMIC(CYieldMonitorDlg)

public:
	CYieldMonitorDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CYieldMonitorDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_YIELD_MONITOR };
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedYeildMonitor(UINT nID);
	afx_msg void OnEnSetfocusEditQaSample();
	afx_msg void OnEnSetfocusEditQaPreset();
	afx_msg void OnEnSetfocusEditQaLow();
	afx_msg void OnBnClickedYeildSave();
	afx_msg void OnEnSetfocusEditFailureCount();
	void MakeLog(LPCTSTR fmt, ...);
	void YieldMonitorChangeLog(std::vector<std::pair<std::pair<CString,int>,std::pair<double,double>>> vData);

private:
	void OnBtnGUI();
	void OnEditBoxGUI();

	CShadeButtonST m_BtnCancel;
	CShadeButtonST m_BtnSave;
	
	BOOL   m_bYieldMonitor;
	BOOL   m_bEventType;
	BOOL   m_bRetestYield;
    BOOL   m_bAutoKill;
	int    m_nSampleQuantity;
	int    m_nPresetCounter;
	int    m_nContinueFailureCnt;
    double m_dLowYield;
	double m_dVariance;
	BOOL   m_bConsecutive_Fail_AutoKill;
	BOOL   m_bConsecutive_Fail_AutoKill_Station;
	int    m_nContinueFailureCnt_Station;
	int	   m_n1stFailCnt;
	BOOL   m_bAuto1stFailKill;

	CEditTrans m_editFailureCount;
	CEditTrans m_editQaSampleCount;
	CEditTrans m_editQaPresetCount;
	CEditTrans m_editQaLowYield;
	CEditTrans m_editQaVariance;
    CEditTrans m_editAutokill;
	CEditTrans m_editFailureCount_Station;
	CEditTrans m_edit1stFailCount;

	CButton m_BtnSocket;
	CButton m_BtnYieldMonitor;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
public:
	afx_msg void OnEnSetfocusEditQaVariance();
	afx_msg void OnEnSetfocusEditFailureCountStation();
	afx_msg void OnEnSetfocusEditQa1stFail();
};
