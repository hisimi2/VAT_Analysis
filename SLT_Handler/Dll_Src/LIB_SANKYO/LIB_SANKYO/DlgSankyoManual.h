#pragma once

#include "resource.h"
#include "afxcmn.h"
// CDlgSankyoManual 대화 상자입니다.

class CDlgSankyoManual : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSankyoManual)

public:
	CDlgSankyoManual(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgSankyoManual();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SANKYO_MANUAL };

private:
	CWnd* m_pOwnerWnd;

private:
	void DisplayUpdate();
	void GetTimeString(CString& strTimeString);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CListCtrl m_listctrlMotorDriver;
	afx_msg void OnBnClickedButtonAutoManual();
	afx_msg void OnBnClickedButton_Send_GetTorqueMsg();
	afx_msg LRESULT OnReceiveLogMsg(WPARAM wParam, LPARAM lParam);
};
