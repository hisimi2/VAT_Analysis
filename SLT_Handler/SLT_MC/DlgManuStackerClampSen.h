#pragma once


// CDlgManuStackerClampSen 대화 상자입니다.

class CDlgManuStackerClampSen : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgManuStackerClampSen)

public:
	CDlgManuStackerClampSen(int nTaskId=0, int nStackerIdx=0, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgManuStackerClampSen();

	void ChangeSensor(int nTaskId, int nStackerIdx);
	void UpdateDisplayClampSen();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANU_STACKER_CLAMP_SEN };

	int m_nTaskId;
	int m_nStackerIdx;
	// 미사용
	//int m_nSenCtrlId[CTaskStacker::eMaxStackerSenLoc][2];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
