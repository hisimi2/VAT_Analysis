#pragma once
#include "../Include/OcxFiles/spreadsheet.h"


// CDlgTesterIF_TesterStatus 대화 상자입니다.

class CDlgTesterIF_TesterStatus : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTesterIF_TesterStatus)

private:
public:
	int m_nSelectedStationID;
	int m_TestSite_Div_X;
	int m_TestSite_Div_Y;

private:
	void UpdateTesterStatus();
	CString GetStatus(int nStatus);
public:
	void SetTesterUnitIdx(int nUnitIdx);

public:
	CDlgTesterIF_TesterStatus(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTesterIF_TesterStatus();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TESTER_IF_STATUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void ClearStatus();
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonDetailTesterInfo();
	afx_msg void OnBnClickedOk();
	CSpreadSheet m_spreadTesterStatus1_4;
	CSpreadSheet m_spreadTesterStatus5_8;
	DECLARE_EVENTSINK_MAP()
	void ClickSpreadTesterStatus( long Col, long Row );
	void ClickSpreadTesterStatus2(long Col, long Row);


};
