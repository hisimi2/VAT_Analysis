#pragma once

#include "../Include/OcxFiles/dsspanel.h"

// CDlgHomeCheckStatus 대화 상자입니다.

class CDlgHomeCheckStatus : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHomeCheckStatus)

public:
	CDlgHomeCheckStatus(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgHomeCheckStatus();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALG_HOME_CHECK_STATUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void UpdateHomeCheckStatus();
	CDSSPanel*	m_Home_Status; 
	
	CStaticTrans m_StaticHomeStatus;
	std::vector <int> m_veAxisNum;
	std::vector <int>	m_vdCurPos;
	int m_nEqpOption;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	afx_msg void OnBnClickedOk();
};
