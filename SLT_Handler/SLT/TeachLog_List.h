#pragma once


// CTeachLog_List 대화 상자입니다.

class CTeachLog_List : public CDialog
{
	DECLARE_DYNAMIC(CTeachLog_List)

public:
	CTeachLog_List(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTeachLog_List();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EVENT_TEACH_LOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
