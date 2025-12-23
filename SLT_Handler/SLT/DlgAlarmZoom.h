#pragma once


// CDlgAlarmZoom 대화 상자입니다.

class CDlgAlarmZoom : public CDialog
{
	DECLARE_DYNAMIC(CDlgAlarmZoom)

public:
	CDlgAlarmZoom(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgAlarmZoom();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ALARM_ZOOM };
#endif
	ST_ERROR_MESSAGE m_stDataListErrLog;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonUserSel1();
};
