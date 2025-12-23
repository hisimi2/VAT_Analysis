#pragma once


#include "resource.h"
#include "afxcmn.h"

// CDlgAioSimul 대화 상자입니다.

class CAIO_SIMUL_DRV;

class CDlgAioSimul : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAioSimul)

public:
	CDlgAioSimul(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgAioSimul();
	void EditSimulData();

protected:

public:
// 대화 상자 데이터입니다.
	enum { IDD = IDD_AIO_SIMUL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	void InitControl();
	void InitListCtrl(CListCtrl& listCtrl, int nInOrOut);

	void UpdateValue(CListCtrl& ListCtrl, int nInOrOut);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CListCtrl m_listSimulAI;
	CListCtrl m_listSimulAO;
	afx_msg void OnBnClickedButtonChangeSimValueInput();
	afx_msg void OnBnClickedButtonChangeSimValueOutput();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnKeyinEditvalue();
};
