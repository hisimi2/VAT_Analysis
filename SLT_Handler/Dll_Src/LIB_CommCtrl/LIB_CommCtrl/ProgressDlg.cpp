//  ProgDlg.cpp : implementation file
// CG: This file was added by the Progress Dialog component

#include "stdafx.h"
#include "ProgressDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog

CProgressDlg::CProgressDlg(/*UINT nCaptionID*/)
{
//	AfxEnableControlContainer();

//	m_nCaptionID = (UINT)_T("Please Wait Disappear this screen ....")/*CG_IDS_PROGRESS_CAPTION*/;
// 	if (nCaptionID != 0)
// 		m_nCaptionID = nCaptionID;

    m_bCancel=FALSE;
    m_nLower=0;
    m_nUpper=100;
    m_nStep=10;
    //{{AFX_DATA_INIT(CProgressDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_bParentDisabled = FALSE;
	m_iTimerPos = 0;
	m_bRunStop = FALSE;
	m_nCaptionID =0;
	m_axis_no = 0;
	m_callback_cancel_event = NULL;
}

CProgressDlg::~CProgressDlg()
{
    if(m_hWnd!=NULL)
      DestroyWindow();
}

BOOL CProgressDlg::DestroyWindow()
{
    ReEnableParent();
    return CDialog::DestroyWindow();
}

void CProgressDlg::ReEnableParent()
{
    if(m_bParentDisabled && (m_pParentWnd!=NULL))
      m_pParentWnd->EnableWindow(TRUE);
    m_bParentDisabled=FALSE;
}

BOOL CProgressDlg::Create(CWnd *pParent, void (*_callback_cancel_event)(int), int _axis_no )
{
    // Get the true parent of the dialog
    m_pParentWnd = CWnd::GetSafeOwner(pParent);

    // m_bParentDisabled is used to re-enable the parent window
    // when the dialog is destroyed. So we don't want to set
    // it to TRUE unless the parent was already enabled.

    if((m_pParentWnd!=NULL) && m_pParentWnd->IsWindowEnabled())
    {
		 m_pParentWnd->EnableWindow(FALSE);
		m_bParentDisabled = TRUE;
    }

    if(!CDialog::Create(CProgressDlg::IDD,pParent))
    {
		ReEnableParent();
		return FALSE;
    }
	m_axis_no = _axis_no;
	m_callback_cancel_event = _callback_cancel_event;

    return TRUE;
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange( pDX );
	//{{AFX_DATA_MAP(CProgressDlg)
 	DDX_Control( pDX, CG_IDC_PROGDLG_PROGRESS_BAR, m_Progress );
 	DDX_Control( pDX, CG_IDC_PROGDLG_PERCENT, m_Percent );
 	DDX_Control( pDX, CG_IDC_STATUS, m_Status );
// 	DDX_Control( pDX, CG_IDC_PROGDLG_SELECT_SYSTEM, m_DisplaySystem );
	//}}AFX_DATA_MAP
	DDX_Control( pDX, IDC_BUTTON_CANCEL, m_btn_cancel );
}

BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
    //{{AFX_MSG_MAP(CProgressDlg)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED( IDC_BUTTON_CANCEL, &CProgressDlg::OnBnClickedCancel )
END_MESSAGE_MAP()

void CProgressDlg::SetStatus(LPCTSTR lpszMessage)
{
    ASSERT(m_hWnd); // Don't call this _before_ the dialog has
                    // been created. Can be called from OnInitDialog
    

    // Verify that the static text control exists
    m_Status.SetWindowText(lpszMessage);
}

void CProgressDlg::OnCancel()
{
    m_bCancel=TRUE;
}

void CProgressDlg::SetRange(int nLower,int nUpper)
{
    m_nLower = nLower;
    m_nUpper = nUpper;
    m_Progress.SetRange(nLower,nUpper);
}
  
int CProgressDlg::SetPos(int nPos)
{
	if(m_nLower>nPos)
		nPos = m_nLower;
	if(m_nUpper<nPos)
		nPos = m_nUpper;
	m_iTimerPos = nPos;
	SetFocus();
    PumpMessages();
    int iResult = m_Progress.SetPos(nPos);
    UpdatePercent(nPos);
    return iResult;
}

int CProgressDlg::SetStep(int nStep)
{
    m_nStep = nStep; // Store for later use in calculating percentage
    return m_Progress.SetStep(nStep);
}

int CProgressDlg::OffsetPos(int nPos)
{
    PumpMessages();
    int iResult = m_Progress.OffsetPos(nPos);
    UpdatePercent(iResult+nPos);
    return iResult;
}

int CProgressDlg::StepIt()
{
    PumpMessages();
    int iResult = m_Progress.StepIt();
    UpdatePercent(iResult+m_nStep);
    return iResult;
}

void CProgressDlg::PumpMessages()
{
    // Must call Create() before using the dialog
    ASSERT(m_hWnd!=NULL);

    MSG msg;
	int nTimeOut = 0;
    // Handle dialog messages
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
		if(!IsDialogMessage(&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);  
		}
    }
	Sleep(1);
}

BOOL CProgressDlg::CheckCancelButton()
{
    // Process all pending messages
    PumpMessages();

    // Reset m_bCancel to FALSE so that
    // CheckCancelButton returns FALSE until the user
    // clicks Cancel again. This will allow you to call
    // CheckCancelButton and still continue the operation.
    // If m_bCancel stayed TRUE, then the next call to
    // CheckCancelButton would always return TRUE

    BOOL bResult = m_bCancel;
    m_bCancel = FALSE;

    return bResult;
}

void CProgressDlg::UpdatePercent(int nNewPos)
{
//    CWnd *pWndPercent = GetDlgItem(CG_IDC_PROGDLG_PERCENT);
    int nPercent;
    
    int nDivisor = m_nUpper - m_nLower;
    ASSERT(nDivisor>0);  // m_nLower should be smaller than m_nUpper

    int nDividend = (nNewPos - m_nLower);
    ASSERT(nDividend>=0);   // Current position should be greater than m_nLower

    nPercent = nDividend * 100 / nDivisor;

    // Since the Progress Control wraps, we will wrap the percentage
    // along with it. However, don't reset 100% back to 0%
    if(nPercent!=100)
      nPercent %= 100;

    // Display the percentage
    CString strBuf;
    strBuf.Format(_T("%d%c"),nPercent,_T('%'));

	CString strCur; // get current percentage
    m_Percent.GetWindowText(strCur);

	if (strCur != strBuf)
		m_Percent.SetWindowText(strBuf);
}
    
/////////////////////////////////////////////////////////////////////////////
// CProgressDlg message handlers

BOOL CProgressDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();

	//m_DisplaySystem.SetCaption("");

	m_Progress.SetRange(m_nLower,m_nUpper);
	m_Progress.SetStep(m_nStep);
	m_Progress.SetPos(m_nLower);

	CString strCaption;
	VERIFY(_T("Please Wait Disappear this screen ....")/*strCaption.LoadString((UINT)_T("Please Wait Disappear this screen ...."))*/);
	SetWindowText(strCaption);

    return TRUE;  
}

void CProgressDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 0:
		KillTimer(0);
		if(m_iTimerPos>100)
			m_iTimerPos = 0;
		else
			m_iTimerPos++;
		SetPos(m_iTimerPos);
		SetTimer(0,200,0);
		break;
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CProgressDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer(0);
}

void CProgressDlg::SetAutoRun(BOOL run_stop)
{
	if(run_stop==TRUE)
		SetTimer(0,200,0);
	else
		KillTimer(0);

	m_bRunStop = run_stop;
}

BEGIN_EVENTSINK_MAP(CProgressDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CProgressDlg)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CProgressDlg::SetCaption(LPCTSTR lpszCaption)
{
	m_DisplaySystem.SetCaption(lpszCaption);

	UpdateData(FALSE);
}

void CProgressDlg::SetVisibleCancelButton( bool _visible )
{
	m_btn_cancel.ShowWindow( _visible == true ? SW_SHOW : SW_HIDE );
}

void CProgressDlg::OnBnClickedCancel()
{
	if (m_axis_no != -1) {
		m_callback_cancel_event(m_axis_no);
	}
	CDialog::OnCancel();
}
