#pragma once


// CDlgManuVisionSetup 대화 상자입니다.

class CDlgManuVisionSetup : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgManuVisionSetup)

public:
	CDlgManuVisionSetup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgManuVisionSetup();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANU_VISION_IF_SETUP };

	int m_nVisionPc;//enum eVisionPC
	std::vector<CVisionComm*> m_vVisionCommObj;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
