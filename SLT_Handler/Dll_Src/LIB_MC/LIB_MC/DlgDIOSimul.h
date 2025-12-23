#pragma once

#include "resource.h"
#include "afxcmn.h"

// CDlgDIOSimul 대화 상자입니다.

class CDlgDIOSimul : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDIOSimul)

public:
	CDlgDIOSimul(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgDIOSimul();

	void InitListCtrl(CListCtrl& listCtrl, int nInOrOut);
	void UpdateIoValue(CListCtrl& listCtrl, int nInOrOut);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIO_SIMUL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listSimulDi;
	CListCtrl m_listSimulDo;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnDioOn();
	afx_msg void OnDioOff();
};
