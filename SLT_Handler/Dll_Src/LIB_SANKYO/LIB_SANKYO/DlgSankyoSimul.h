#pragma once

#include <vector>
#include "afxcmn.h"
#include "resource.h"

// CDlgSankyoSimul 대화 상자입니다.

class CDlgSankyoSimul : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSankyoSimul)

public:
	CDlgSankyoSimul(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgSankyoSimul();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SANKYO_SIMUL };

public:
	double GetTorque(int nStation);

private:
	std::vector<double> m_vTorque;
	CWnd* m_pOwnerWnd;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CListCtrl m_listctrlMotorDriver;
	afx_msg void OnNMDblclkListDriversForSimul(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonApply();
};
