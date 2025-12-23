#if !defined(AFX_GETVALUEDLG_H__E5621870_5C7E_417B_9FCA_221534FE39B0__INCLUDED_)
#define AFX_GETVALUEDLG_H__E5621870_5C7E_417B_9FCA_221534FE39B0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GetVal.h : header file
//

#include "MinLabel.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CKeyPadDlg dialog

class  CKeyPadDlg : public CDialog
{
// Construction
public:
	CKeyPadDlg(int maxchar = 10, char* getnum = "", char* title = "Text", BOOL password = FALSE, CWnd* pParent = NULL);   // standard constructor

	CString     m_strGetVal;
// Dialog Data
	//{{AFX_DATA(CKeyPadDlg)
	enum { IDD = IDD_DLG_KEYPAD };
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyPadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitStaticDispWnd();
	BOOL VerifyMaxChar();

	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();

	// Generated message map functions
	//{{AFX_MSG(CKeyPadDlg)
	afx_msg void OnBtnBar();
	afx_msg void OnButton1();
	afx_msg void OnButton10();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnButtonA();
	afx_msg void OnButtonB();
	afx_msg void OnButtonBack();
	afx_msg void OnButtonC();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonComma();
	afx_msg void OnButtonD();
	afx_msg void OnButtonE();
	afx_msg void OnButtonF();
	afx_msg void OnButtonG();
	afx_msg void OnButtonH();
	afx_msg void OnButtonI();
	afx_msg void OnButtonJ();
	afx_msg void OnButtonK();
	afx_msg void OnButtonL();
	afx_msg void OnButtonM();
	afx_msg void OnButtonN();
	afx_msg void OnButtonO();
	afx_msg void OnButtonP();
	afx_msg void OnButtonQ();
	afx_msg void OnButtonR();
	afx_msg void OnButtonS();
	afx_msg void OnButtonT();
	afx_msg void OnButtonU();
	afx_msg void OnButtonV();
	afx_msg void OnButtonW();
	afx_msg void OnButtonX();
	afx_msg void OnButtonY();
	afx_msg void OnButtonZ();
	afx_msg void OnPaint();
	afx_msg void OnButtonSlash();
	afx_msg void OnButtonUnderline();
	afx_msg void OnButtonColon();
	afx_msg void OnButtonEqual();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	int         m_nCxChar;
	int         m_nCyChar;
	int         m_nMaxChar;
	CRect       m_rect;
	BOOL        m_bPassword;	
	CString     m_strWndText;
	CMinLabel	m_sDispValue;
	void        UpdateDisplay(COLORREF bkcolor=RGB(255,255,255));
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETVAL_H__8255AC82_40A5_11D2_8D90_00AA00A18C62__INCLUDED_)
