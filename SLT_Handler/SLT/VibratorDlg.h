#pragma once
#include "afxwin.h"

// CVibratorDlg 대화 상자입니다.

class CVibratorDlg : public CDialog
{
	DECLARE_DYNAMIC(CVibratorDlg)

public:
	CVibratorDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CVibratorDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_VIBRATOR };

private:
	void OnEditBoxGUI();
	void OnBtnGUI();

	CShadeButtonST m_BtnSave;
	CShadeButtonST m_BtnCancel;

	CEditTrans m_editVibUdDvcActCount;
	CEditTrans m_editVibDelayTime;
	CEditTrans m_editVibActTimes;

	
	double m_dVibDelayTime;
	int    m_nVibActTimes;
	int	   m_nVibUdDvcActCount;
	
	int m_nActionMode;  // 0: Unloading finish  1:Setplate down

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedVibratorSave();
	afx_msg void OnEnSetfocusEditUnloadSetplate1();
	void MakeLog(LPCTSTR fmt, ...);
	// vData.first = stCokData, vData.second.first = BeforeData vData.second.second = AfterData
	void SetplateVibratorChangeLog(std::vector<std::pair<CString,std::pair<double,double>>> vData);
	CStatic m_bitPic1;
	CBitmap m_hBitVibrator;
	afx_msg void OnEnSetfocusEditActCnt();
	afx_msg void OnEnSetfocusEditUnloadCount();
};
