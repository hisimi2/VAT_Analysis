#pragma once
#include "../Include/OcxFiles/dsspanel.h"
#include "CBSpeedListCtrl.h"
// CEventLog_List 대화 상자입니다.

class CEventLog_List : public CDialogEx
{
	DECLARE_DYNAMIC(CEventLog_List)

public:
	CEventLog_List(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEventLog_List();

// 대화 상자 데이터입니다.

	enum { IDD = IDD_EVENT_PG_LOG};


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void InitListGUI();
	CMyList m_DataListErrLog;
	void InitSorting(int nColumn);
	LRESULT OnListDoubleClick(WPARAM wParam, LPARAM lParam);
private:
	CCBSpeedListCtrl m_listLOG;

	bool m_bSortAscending;	// 오름차순, 내림차순 판단
	void MakeLog(LPCTSTR fmt, ...);
public:
	//afx_msg void OnLvnColumnclickListLog(NMHDR *pNMHDR, LRESULT *pResult);
};
