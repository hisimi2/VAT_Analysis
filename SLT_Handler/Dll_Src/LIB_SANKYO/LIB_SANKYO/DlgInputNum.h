#pragma once

#include "resource.h"
// CDlgInputNum 대화 상자입니다.

class CDlgInputNum : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInputNum)

public:
	CDlgInputNum(int nNum, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgInputNum();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_INPUT_NUM };

public:
	void GetNum(char* szBuff, int nSize);
	void SetNum(char* szBuff, int nSize);

private:
	char m_szNum[128];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEditInput();
};
