#pragma once
#include "afxwin.h"

// CYieldSetDlg 대화 상자입니다.

class CYieldSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CYieldSetDlg)

public:
	CYieldSetDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CYieldSetDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_YIELD_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedYieldSave();
	void MakeLog(LPCTSTR fmt, ...);
	void ConrtactSettingChangeLog(std::vector<std::pair<std::pair<CString,int>,std::pair<double,double>>> vData);

private:
	

	int	   m_nForcePins;

	double m_dDropTime;
	double m_dDropHeight;

	double m_dForceGF;

	double m_dDvcDimenX;
	double m_dDvcDimenY;
	
	double m_dContactPos[eMaxPressUnitCount];
	double m_dContactPosBefore[eMaxPressUnitCount];

	// Mode select variable
	BOOL m_bContactMode;

	CEditTrans m_EditForcePins;
	CEditTrans m_EditForceGF;
	CEditTrans m_EditContacForce;
	CEditTrans m_EditDvcDimenX;
	CEditTrans m_EditDvcDimenY;

	CEditTrans m_EditContactPos1;
	CEditTrans m_EditContactPos2;
	CEditTrans m_EditContactPos3;
	CEditTrans m_EditContactPos4;
	CEditTrans m_EditContactPos5;
	CEditTrans m_EditContactPos6;
	CEditTrans m_EditContactPos7;
	CEditTrans m_EditContactPos8;

	CEditTrans m_EditHeightCalData1;
	CEditTrans m_EditHeightCalData2;
	CEditTrans m_EditHeightCalData3;
	CEditTrans m_EditHeightCalData4;
	CEditTrans m_EditHeightCalData5;
	CEditTrans m_EditHeightCalData6;
	CEditTrans m_EditHeightCalData7;
	CEditTrans m_EditHeightCalData8;


	CShadeButtonST m_BtnSave;
	CShadeButtonST m_BtnCancel;	

	void OnBtnGUI();
	void SetBtnRadioContact();
	void OnEditBoxGUI();
	void OnUpdateForceData();
	void LoadPressContactData();
	void SavePressContactData();
	BOOL PressContactPosChangeLog();

	void LoadPressHeightCalData();

public:
	afx_msg void OnBnClickedRadioContact1();
	afx_msg void OnBnClickedRadioContact2();
	

	afx_msg void OnEnSetfocusEditDropHeight();
	afx_msg void OnEnSetfocusEditDropTimer();
	afx_msg void OnEnSetfocusEditPins();
	afx_msg void OnEnSetfocusEditPinGf();
	afx_msg void OnEnSetfocusEditDvcDimenX();
	afx_msg void OnEnSetfocusEditDvcDimenY();
	afx_msg void OnEnSetfocusEditContactPos1();
	afx_msg void OnEnSetfocusEditContactPos2();
	afx_msg void OnEnSetfocusEditContactPos3();
	afx_msg void OnEnSetfocusEditContactPos4();
	afx_msg void OnEnSetfocusEditContactPos5();
	afx_msg void OnEnSetfocusEditContactPos6();
	afx_msg void OnEnSetfocusEditContactPos7();
	afx_msg void OnEnSetfocusEditContactPos8();
};
