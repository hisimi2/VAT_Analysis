#if !defined(AFX_CALENDERCHOOSEDLG_H__1A139201_EFBB_11D4_A80C_00A024A6C9FD__INCLUDED_)
#define AFX_CALENDERCHOOSEDLG_H__1A139201_EFBB_11D4_A80C_00A024A6C9FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "../Include/OcxFiles/calendar.h"
/////////////////////////////////////////////////////////////////////////////
// CCalenderDlg dialog

class  CCalenderDlg : public CDialog
{
// Construction
public:
	CString m_szDate;
	CString GetSelectedDate();
	CCalenderDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalenderDlg)
	enum { IDD = IDD_DLG_CALENDER };
	CCalendar	m_Calender;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalenderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalenderDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickOk();
	afx_msg void OnClickCancel();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALENDERCHOOSEDLG_H__1A139201_EFBB_11D4_A80C_00A024A6C9FD__INCLUDED_)
