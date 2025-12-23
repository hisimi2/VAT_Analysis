#pragma once

#include "DEF_COMMON.h"
// CDlgSetupSignalTower 대화 상자입니다.
#include "BtnST.h"
#include "ShadeButtonST.h"

class CDlgSetupSignalTower : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetupSignalTower)

public:
	CDlgSetupSignalTower(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgSetupSignalTower();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_TOWER_LIGHT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();

private:
	void SetComboBox();
	void GetTowerDataToSM();
	ST_SLT_SIGNAL_TOWER  m_stTowerSetting;
public:
	afx_msg void OnBnClickedBtnTowerSave();

	CBitmap m_cBmpSave;
	CBitmap m_cBmpExit;

	CButton m_btnSave;
	CButton m_btnExit;
	CShadeButtonST m_btnDefault;

	afx_msg void OnBnClickedBtnTowerDefault();
};
