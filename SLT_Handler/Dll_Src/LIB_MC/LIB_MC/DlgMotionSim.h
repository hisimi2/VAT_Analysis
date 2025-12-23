#pragma once

#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"

// CDlgMotionSim 대화 상자입니다. 
#define MAX_DISPLAY_MOTION_SIM_BIT_STATE (10)


class CDlgMotionSim : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMotionSim)

public:
	CDlgMotionSim(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgMotionSim();
	void InitListCtrl(CListCtrl& listCtrl);
	void UpdateMotionSimulData();
	void StatusOnOff(BOOL bOnOff);

	int m_nCurAxis;

	enum { eEditSimulItemId_TPos = 0, eEditSimulItemId_TSpd, eEditSimulItemId_TAcc, eEditSimulItemId_TDcc,
	       eEditSimulItemId_CPos, eEditSimulItemId_CSpd, eEditSimulItemId_CAcc, eEditSimulItemId_CDcc};
	int m_nEditSimulItemId;

private:
	void* m_pOwner;

public:

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MOTION_SIM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cbAxisSelect;
	CListCtrl m_listSimMotorState;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnCbnSelchangeComboLibMcMotionSimAxisSelect();
	afx_msg void OnDioOn();
	afx_msg void OnDioOff();
	afx_msg void OnKeyinEditvalue();
	CComboBox m_cbMotionSimulResponseMode;
	afx_msg void OnCbnSelchangeComboSimulResponseMode();
};
