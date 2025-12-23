//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_KEYDLG_H__8AD2226B_FF61_4AA6_A984_08DC0D907C03__INCLUDED_)
#define AFX_KEYDLG_H__8AD2226B_FF61_4AA6_A984_08DC0D907C03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNumberBoxDlg dialog
#include "ShadeButtonST.h"
#include "EditTrans.h"
#include "DEF_LIB_GUI.h"
#include "resource.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CNumberBoxDlg dialog

class  AFX_EXT_CLASS CNumberBoxDlg : public CDialog
{
// Construction
public:
	CNumberBoxDlg(int maxchar = 20, char* getnum = "123", char* title = "Input Data", CWnd* pParent = NULL, char* cMax = "99999", char* cMin = "0");   // standard constructor
	~CNumberBoxDlg();

	void SetWindowTitle(LPCTSTR title);

	CString m_strGetNum;
	CString m_strCurrentNum;
	int m_intMaxChar;
	double m_dCurrent, m_dpoint;
	char m_cCurrent[10];

	CString m_strWndText;
	CString m_strMaxValue;
	CString m_strMinValue;
	

	//{{AFX_DATA(CNumberBoxDlg)
	enum { IDD = IDD_DLG_NUMBER };	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNumberBoxDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL VerifyMaxChar();
	//void InitStaticDispWnd();
	BOOL m_bFirstTime;

	// Generated message map functions
	//{{AFX_MSG(CNumberBoxDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	//DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int   m_nCyChar;
	int   m_nCxChar;
	CRect m_rect;
	void UpdateDisplay();
	void GetStoD(double point, BOOL sign);


	CShadeButtonST m_btnN0;
	CShadeButtonST m_btnN1;
	CShadeButtonST m_btnN2;
	CShadeButtonST m_btnN3;
	CShadeButtonST m_btnN4;
	CShadeButtonST m_btnN5;
	CShadeButtonST m_btnN6;
	CShadeButtonST m_btnN7;
	CShadeButtonST m_btnN8;
	CShadeButtonST m_btnN9;
	CShadeButtonST m_btnMinus;
	CShadeButtonST m_btnDot;

	CShadeButtonST m_btnBkSp;
	CShadeButtonST m_btnClr;
	CShadeButtonST m_btnCancel;
	CShadeButtonST m_btnOk;

	void OnGuiBtnSet();
public:
	afx_msg void OnBnClickedBtn7();
	afx_msg void OnBnClickedBtn8();
	afx_msg void OnBnClickedBtn0();
	afx_msg void OnBnClickedBtn1();
	afx_msg void OnBnClickedBtn2();
	afx_msg void OnBnClickedBtn3();
	afx_msg void OnBnClickedBtn4();
	afx_msg void OnBnClickedBtn5();
	afx_msg void OnBnClickedBtn6();
	afx_msg void OnBnClickedBtn9();
	afx_msg void OnBnClickedBtnBack();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedBtnCancle();
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnNega();
	afx_msg void OnBnClickedBtnDot();
	afx_msg void OnBnClickedBtnNe01();
	afx_msg void OnBnClickedBtnPo01();
	afx_msg void OnBnClickedBtnNe05();
	afx_msg void OnBnClickedBtnPo05();
	afx_msg void OnBnClickedBtnNe1();
	afx_msg void OnBnClickedBtnPo1();
	afx_msg void OnBnClickedBtnNe10();
	afx_msg void OnBnClickedBtnPo10();
	//CEdit m_CtrlCur;
	CEditTrans m_EditCurData;
	CEditTrans m_EditMaxData;
	CEditTrans m_EditMinData;
	//CEditTrans m_EditSetData;
	CString m_strSetData;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETNUM_H__8255AC81_40A5_11D2_8D90_00AA00A18C62__INCLUDED_)
