#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "EditTrans.h"


// CDlgManuRegul 대화 상자입니다.

class CDlgManuRegul : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgManuRegul)

public:
	CDlgManuRegul(int nRegulatorIdx=0, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgManuRegul();
	void InitControl();
	void UpdateRegulatorValue();

private:
	int m_nRegulatorIdx;
	int m_nCurPressureUnit;
	double m_dSetPressureValue;
	_tAIO_LS m_stAiInfo;

public:

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANU_REGULATOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonSetPress();
	CSliderCtrl m_sliderPressure;
	CComboBox m_cbPressureUnit;
	afx_msg void OnCbnSelchangeComboPressureUnit();
	afx_msg void OnNMReleasedcaptureSliderRvalue(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_editSetPressure;
	CComboBox m_cbSelectRegulator;
	afx_msg void OnCbnSelchangeComboSelRegNo();
	afx_msg void OnEnSetfocusEditPressMpa();

	CBitmap m_cBmpExit;

	CButton m_btnExit;
	afx_msg void OnBnClickedButtonSetForce();
	DECLARE_EVENTSINK_MAP()
	void ClickEditSetForcePerPin();
	void ClickEditSetForceToDevice();
	afx_msg void OnBnClickedButtonUnitCvtToGf();
	afx_msg void OnBnClickedButtonUnitCvtToMpa();
	void ClickEditCvtRegPressure();
	void ClickEditCvtForcePerPin();
	afx_msg void OnBnClickedRsave();
	afx_msg void OnBnClickedButtonRegIdxUp();
	afx_msg void OnBnClickedButtonRegIdxDown();
	afx_msg void OnBnClickedButtonSetForceAll();
	afx_msg void OnBnClickedRsaveAll();
	afx_msg void OnBnClickedButtonSetPress2();

	void MakeLog(LPCTSTR fmt, ...);
};
