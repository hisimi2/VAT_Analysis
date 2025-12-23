#pragma once
#include "../Include/OcxFiles/dsspanel.h"
// CEventLog_Frequency 대화 상자입니다.

class CEventLog_Frequency : public CDialogEx
{
	DECLARE_DYNAMIC(CEventLog_Frequency)

public:
	CEventLog_Frequency(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEventLog_Frequency();

// 대화 상자 데이터입니다.

	enum { IDD = IDD_EVENT_PG_FREQUENCY};


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void InitListGUI();
	CMyList m_DataListErrLog;
	//void InitSorting(int nColumn);
	CString MakeExpertData(std::vector<ST_ERROR_MESSAGE> vData);
	void MakeLog(LPCTSTR fmt, ...);
private:
	//CListCtrl m_listLOG;
	CCBSpeedListCtrl m_listLOG;
public:
};
