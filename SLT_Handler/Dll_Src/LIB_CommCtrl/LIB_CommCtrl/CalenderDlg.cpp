// CalenderChooseDlg.cpp : implementation file

#include "stdafx.h"
#include "CalenderDlg.h"
#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalenderDlg dialog


CCalenderDlg::CCalenderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalenderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalenderDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCalenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalenderDlg)
	DDX_Control(pDX, IDC_CALENDAR1, m_Calender);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalenderDlg, CDialog)
	//{{AFX_MSG_MAP(CCalenderDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalenderDlg message handlers

void CCalenderDlg::OnOK()
{
	// TODO: Add extra validation here
	if(m_Calender.GetMonth() < 10 || m_Calender.GetDay() < 10)
	{
		if(m_Calender.GetDay() < 10 && m_Calender.GetMonth() < 10)
			m_szDate.Format("%4d0%1d0%1d",m_Calender.GetYear(),m_Calender.GetMonth(),m_Calender.GetDay());
		else if(m_Calender.GetMonth() < 10)
			m_szDate.Format("%4d0%1d%2d",m_Calender.GetYear(),m_Calender.GetMonth(),m_Calender.GetDay());
		else if(m_Calender.GetDay() < 10)
			m_szDate.Format("%4d%2d0%1d",m_Calender.GetYear(),m_Calender.GetMonth(),m_Calender.GetDay());
	}
	else
		m_szDate.Format("%4d%2d%2d",m_Calender.GetYear(),m_Calender.GetMonth(),m_Calender.GetDay());

		
	CDialog::OnOK();
}

void CCalenderDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

CString CCalenderDlg::GetSelectedDate()
{
	return m_szDate;
}

BOOL CCalenderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CTime time = CTime::GetCurrentTime();

	m_Calender.SetYear(time.GetYear());
	m_Calender.SetMonth(time.GetMonth());
	m_Calender.SetDay(time.GetDay());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CCalenderDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CCalenderDlg)
	ON_EVENT(CCalenderDlg, IDC_OK, 22 /* Click */, OnClickOk, VTS_NONE)
	ON_EVENT(CCalenderDlg, IDC_CANCEL, 22 /* Click */, OnClickCancel, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CCalenderDlg::OnClickOk() 
{
	// TODO: Add your control notification handler code here
	if(m_Calender.GetMonth() < 10 || m_Calender.GetDay() < 10)
	{
		if(m_Calender.GetDay() < 10 && m_Calender.GetMonth() < 10)
			m_szDate.Format("%4d0%1d0%1d",m_Calender.GetYear(),m_Calender.GetMonth(),m_Calender.GetDay());
		else if(m_Calender.GetMonth() < 10)
			m_szDate.Format("%4d0%1d%2d",m_Calender.GetYear(),m_Calender.GetMonth(),m_Calender.GetDay());
		else if(m_Calender.GetDay() < 10)
			m_szDate.Format("%4d%2d0%1d",m_Calender.GetYear(),m_Calender.GetMonth(),m_Calender.GetDay());
	}
	else
		m_szDate.Format("%4d%2d%2d",m_Calender.GetYear(),m_Calender.GetMonth(),m_Calender.GetDay());
		
	CDialog::OnOK();
}

void CCalenderDlg::OnClickCancel() 
{
	// TODO: Add your control notification handler code here
		
	CDialog::OnCancel();
}
