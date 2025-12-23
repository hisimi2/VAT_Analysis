// ProgDlg.h : header file
// CG: This file was added by the Progress Dialog component

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog
//{{AFX_INCLUDES()
#include "../Include/OcxFiles/sscommand.h"
#include "resource.h"
#include "afxwin.h"
//}}AFX_INCLUDES
 
#ifndef __PROGDLG_H__
#define __PROGDLG_H__

class  CProgressDlg : public CDialog
{
// Construction / Destruction
public:
	void SetAutoRun(BOOL run_stop);
    CProgressDlg(/*UINT nCaptionID = 0*/);   // standard constructor
    ~CProgressDlg();

	BOOL Create( CWnd * pParent = nullptr, void (*_callback_cancel_event)(int) = nullptr, int _axis_no = -1 );

    // Checking for Cancel button
    BOOL CheckCancelButton();
    // Progress Dialog manipulation
    void SetStatus(LPCTSTR lpszMessage);
    void SetRange(int nLower,int nUpper);
    int  SetStep(int nStep);
    int  SetPos(int nPos);
    int  OffsetPos(int nPos);
    int  StepIt();
	void SetCaption(LPCTSTR lpszCaption);
	void SetVisibleCancelButton( bool _visible );
	BOOL m_bRunStop;
        
// Dialog Data
    //{{AFX_DATA(CProgressDlg)
	enum { IDD = IDD_DLG_PROGRESS_BAR };
    CProgressCtrl	m_Progress;
	CSSCommand	    m_Percent;
	CSSCommand	    m_Status;
	CSSCommand	    m_DisplaySystem;
	//}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CProgressDlg)
    public:
    BOOL DestroyWindow();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
	UINT m_nCaptionID;
	int  m_nLower;
	int  m_nUpper;
	int  m_nStep;
    
    BOOL m_bCancel;
    BOOL m_bParentDisabled;

    void ReEnableParent();


    virtual void OnCancel();
    virtual void OnOK() {}; 
    void UpdatePercent(int nCurrent);
    void PumpMessages();

    // Generated message map functions
    //{{AFX_MSG(CProgressDlg)
    virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private: 
	BOOL m_iTimerPos;
	void (*m_callback_cancel_event)( int _axis_no );
	int m_axis_no;
public:
	afx_msg void OnBnClickedCancel();
	CButton m_btn_cancel;
};

#endif // __PROGDLG_H__
