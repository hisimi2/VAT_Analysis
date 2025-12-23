#pragma once
#include "afxcmn.h"


// CDlgTesterIFInfo 대화 상자입니다.

class CDlgTesterIFInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTesterIFInfo)

private:
	int m_nTesterUnitIdx;

public:
	CDlgTesterIFInfo(int nUnitIdx, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTesterIFInfo();

	void InitCtrl();
	void UpdateTesterInfo();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TESTER_IF_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
	CTreeCtrl m_treeInfo;
};
