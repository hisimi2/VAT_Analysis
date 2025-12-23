#pragma once

#include "afxwin.h"
// FilePathDlg 대화 상자입니다.

class CFilePathDlg : public CDialog
{
	DECLARE_DYNAMIC(CFilePathDlg)

public:
	CFilePathDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFilePathDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_FILEPATH };

	BOOL OnInitDialog();
	void OnBtnGUI();
	void SetDisplayUserData();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonPath();
	afx_msg void OnBnClickedButtonBatchPath();

	CString m_strSummaryPath;
	CString m_strNetWorkSummaryPath;
	CString m_strBeforeSummaryPath;
	CString m_strBeforeNetWorkSummaryPath;
	CString m_strBatchPath;
	CString m_strBeforeBatchPath;
	CString m_strLotInfoPath;
	CString m_strBeforeLotInfoPath;
	CString m_strTesterBatchPath;
	CString m_strBeforeTesterBatchPath;
	CString m_str2didCsvPath;
	CString m_strBefore2didCsvPath;
	CString m_strHitSumamryPath;
	CString m_strBeforeHitSumamryPath;

	BOOL m_bBeforeCheckBatchFileUse;
	BOOL m_bBeforeTesterVersionOff;
	BOOL m_bBeforeTesterCheckBatchFileUse;

	CButton m_CheckBatchFileUse;
	CButton m_CheckTesterVersionOff;
	CButton m_CheckTesterBatchFileUse;
	void IsSamPath();
	void MakeLog(LPCTSTR fmt, ...);

private:
	CShadeButtonST m_BtnCancel;
	CShadeButtonST m_BtnSave;
	CShadeButtonST m_BtnPath;
	CShadeButtonST m_BtnBatchPath;
	CShadeButtonST m_BtnLotInfoPath;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLotinfoPath();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonTesterBatchPath();
	afx_msg void OnBnClickedButtonNetworkPath();
	afx_msg void OnBnClickedButton2didCsvPath();
	afx_msg void OnBnClickedButtonHitSummaryPath();
};
