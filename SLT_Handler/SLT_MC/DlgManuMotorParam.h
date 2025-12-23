#pragma once

class CSpreadSheet;

// CDlgManuMotorParam 대화 상자입니다.

class CDlgManuMotorParam : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgManuMotorParam)

public:
	CDlgManuMotorParam(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgManuMotorParam();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANU_MOTOR_PARAMETER };

private:
	void InitControl();


	CMyList m_MotorParamList;
	CMyList m_MotorParamList_old;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CSpreadSheet m_srdMotorParam;
	afx_msg void OnBnClickedOk();
	DECLARE_EVENTSINK_MAP()
	void ClickCommandbuttonSave();
	void ClickCommandbuttonPcd();

	std::pair<CString, int> CheckNormalSpdOverRange();
};
