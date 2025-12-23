//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_MODALESSMESSAGEBOX_H__1A139203_EFBB_11D4_A80C_00A024A6C9FD__INCLUDED_)
#define AFX_MODALESSMESSAGEBOX_H__1A139203_EFBB_11D4_A80C_00A024A6C9FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include/OcxFiles/mdctext.h"
#include "../Include/OcxFiles/mdclist.h"
#include "DEF_LIB_COMMCTRL.h"
#include "resource.h"

class  CShowMsgBox : public CDialog
{
// Construction
public:
	int mBt_type;
	BOOL m_bCloseMode;
	CString m_szTempMessage;
	//static int m_iShowFlag;
	CString m_szTitle;
	CShowMsgBox(CWnd* pParent = NULL);   // standard constructor

	ST_SMB_INFO* m_pMsgInfo;

// Dialog Data
	//{{AFX_DATA(CModalessMessageBox)
	enum { IDD = IDD_DLG_ERROR };
	CMdcText	m_szMessage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModalessMessageBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
protected:

	// Generated message map functions
	//{{AFX_MSG(CModalessMessageBox)
	afx_msg void OnButtonClose();
	virtual BOOL OnInitDialog();
	
	afx_msg void OnButton2Close();
	afx_msg void OnButton2Close2();
	afx_msg void OnClose();				// By hyb [150529] [BUGFIX-SITE] 온도표시 안정화 디버깅 하면서 수정
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	virtual void PostNcDestroy();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODALESSMESSAGEBOX_H__1A139203_EFBB_11D4_A80C_00A024A6C9FD__INCLUDED_)
