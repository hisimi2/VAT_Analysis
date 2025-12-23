#pragma once


// CEventLog_Teach 대화 상자입니다.

class CEventLog_Teach : public CDialog
{
	DECLARE_DYNAMIC(CEventLog_Teach)
private:
	CCBSpeedListCtrl m_listTeach;
public:
	CEventLog_Teach(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEventLog_Teach();

// 대화 상자 데이터입니다.

	enum { IDD = IDD_EVENT_TEACH_LOG };

	void InitListGUI();
	void MakeLog(LPCTSTR fmt, ...);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
