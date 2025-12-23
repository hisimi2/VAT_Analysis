#pragma once

#include "ShadeButtonST.h"
// CVAT_DisplayLog 대화 상자입니다.

#define UM_VAT_SEQ_MSG_PRINT_LOG	  (WM_USER + 100)

class CVAT_DisplayLog : public CDialogEx
{
	DECLARE_DYNAMIC(CVAT_DisplayLog)

public:
	CVAT_DisplayLog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CVAT_DisplayLog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VAT_DISPLAY_LOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();	
	afx_msg LRESULT OnReceiveLogDataFromSequence(WPARAM wParam, LPARAM lParam);
	void SetHorizontalScroll(CListBox &LogList);

public:
	void ClearListBox();
	void InitBtnControl();

private:
	CShadeButtonST m_BtnExit;
	CShadeButtonST m_BtnLog;

public:
	CListBox m_LoadTrayPPList;
	CListBox m_UnloadTrayPPList;
	CListBox m_TestPP1List;
	CListBox m_TestPP2List;
	afx_msg void OnBnClickedBtnLogFile();


};
