#pragma once
#include "afxwin.h"


// CDlgCalculatePcd 대화 상자입니다.

class CDlgCalculatePcd : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCalculatePcd)

public:
	CDlgCalculatePcd(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgCalculatePcd();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PCD_CAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboPcdCalAxisSelect();
	afx_msg void OnBnClickedButtonPcdCalculate();
	afx_msg void OnBnClickedButtonPcdCalSave();
	afx_msg void OnEnSetfocusEditPcdDesignDistance();
	afx_msg void OnEnSetfocusEditPcdMeasureDistance();
public:
	CComboBox m_cbAxisSelect;
	CMyList     m_listMotorParam;
	int		m_nCurAxis;

	double m_dDesignDis;
	double m_dMeasureDis;
	double m_dDesignScaleFactor;
	double m_dMeasureScaleFactor;

	BOOL m_bChkCal;

};
