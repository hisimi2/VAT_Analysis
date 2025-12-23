#pragma once


// CUpdateHistory 대화 상자입니다.

class CUpdateHistory : public CDialog
{
	DECLARE_DYNAMIC(CUpdateHistory)

public:
	CUpdateHistory(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUpdateHistory();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_UPDATE_HISTORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	BOOL InitHistory();

public: 
	virtual BOOL OnInitDialog();
	void MakeLog(LPCTSTR fmt, ...);

	CEditTrans m_EditHistory;
	CShadeButtonST m_BtnCancel;
};
