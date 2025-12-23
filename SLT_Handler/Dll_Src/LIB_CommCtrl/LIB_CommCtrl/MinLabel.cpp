// MinLabel.cpp : implementation file
//

#include "stdafx.h"
#include "MinLabel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMinLabel

CMinLabel::CMinLabel()
{
	m_crText = GetSysColor(COLOR_WINDOWTEXT);
	m_hBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));

	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT),sizeof(m_lf),&m_lf);

	m_font.CreateFontIndirect(&m_lf);
	m_bTimer = FALSE;
	m_bState = FALSE;
	m_bLink = TRUE;
	m_hCursor = NULL;
	m_Type = None;

	m_hwndBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));
}

CMinLabel::~CMinLabel()
{
	m_font.DeleteObject();
	::DeleteObject(m_hBrush);
	if(m_hwndBrush)
		::DeleteObject(m_hwndBrush);
}

CMinLabel& CMinLabel::SetColor(COLORREF crText, COLORREF crBkgnd)
{
	SetTextColor(crText);
	SetBkColor(crBkgnd);
	return *this;
}

CMinLabel& CMinLabel::SetText(const CString& strText, COLORREF crText, COLORREF crBkgnd)
{
	SetText(strText);
	SetTextColor(crText);
	SetBkColor(crBkgnd);
	return *this;
}

CMinLabel& CMinLabel::SetText(const CString& strText)
{
	SetWindowText(strText);
	return *this;
}

CMinLabel& CMinLabel::SetTextColor(COLORREF crText)
{
	m_crText = crText;
	RedrawWindow();
	return *this;
}

CMinLabel& CMinLabel::SetBkColor(COLORREF crBkgnd)
{
	if(m_hBrush) ::DeleteObject(m_hBrush);
	m_hBrush = ::CreateSolidBrush(crBkgnd);
	RedrawWindow();
	return *this;
}

CMinLabel& CMinLabel::SetFlashBkColor(COLORREF crFlashBkgnd)
{
	if(m_hwndBrush) ::DeleteObject(m_hwndBrush);
	m_hwndBrush = ::CreateSolidBrush(crFlashBkgnd);
	return *this;
}

CMinLabel& CMinLabel::SetFontBold(BOOL bBold)
{	
	m_lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
	ReconstructFont();
	RedrawWindow();
	return *this;
}

CMinLabel& CMinLabel::SetFontUnderline(BOOL bSet)
{	
	m_lf.lfUnderline = bSet;
	ReconstructFont();
	RedrawWindow();
	return *this;
}

CMinLabel& CMinLabel::SetFontItalic(BOOL bSet)
{
	m_lf.lfItalic = bSet;
	ReconstructFont();
	RedrawWindow();
	return *this;	
}

CMinLabel& CMinLabel::SetSunken(BOOL bSet)
{
	if( !bSet ) ModifyStyleEx(WS_EX_STATICEDGE,0,SWP_DRAWFRAME);
	else ModifyStyleEx(0,WS_EX_STATICEDGE,SWP_DRAWFRAME);
		
	return *this;	
}

CMinLabel& CMinLabel::SetBorder(BOOL bSet)
{
	if( !bSet ) ModifyStyle(WS_BORDER,0,SWP_DRAWFRAME);
	else ModifyStyle(0,WS_BORDER,SWP_DRAWFRAME);
		
	return *this;	
}

CMinLabel& CMinLabel::SetFontSize(int nSize)
{
	const int POINTS_PER_INCH = 72;
	int nDot;
	CClientDC dc(this);

	nDot = (nSize * dc.GetDeviceCaps(LOGPIXELSY)) / POINTS_PER_INCH;

	m_lf.lfHeight = nDot;
	ReconstructFont();
	ReleaseDC(&dc);  // test of dandyman
	RedrawWindow();
	return *this;
}

CMinLabel& CMinLabel::SetFontName(const CString& strFont)
{	
	strcpy(m_lf.lfFaceName,strFont);
	ReconstructFont();
	RedrawWindow();
	return *this;
}

CMinLabel& CMinLabel::FlashText(BOOL bActivate)
{
	if(m_bTimer)
	{
		SetWindowText(m_strText);
		KillTimer(1);
	}

	if(bActivate)
	{
		GetWindowText(m_strText);
		m_bState = FALSE;
		
		m_bTimer = TRUE;
		SetTimer(2,500,NULL);
		m_Type = Text;
	}

	return *this;
}

CMinLabel& CMinLabel::FlashBackground(BOOL bActivate)
{

	if(m_bTimer) KillTimer(1);

	if (bActivate)
	{
		m_bState = FALSE;

		m_bTimer = TRUE;
		SetTimer(1,500,NULL);

		m_Type = Background;
	}

	return *this;
}

CMinLabel& CMinLabel::SetLink(BOOL bLink)
{
	m_bLink = bLink;

	if (bLink)
		ModifyStyle(0,SS_NOTIFY);
	else
		ModifyStyle(SS_NOTIFY,0);

	return *this;
}

CMinLabel& CMinLabel::SetLinkCursor(HCURSOR hCursor)
{
	m_hCursor = hCursor;
	return *this;
}

BEGIN_MESSAGE_MAP(CMinLabel, CStatic)
	//{{AFX_MSG_MAP(CMinLabel)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMinLabel message handlers

HBRUSH CMinLabel::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	if(CTLCOLOR_STATIC == nCtlColor)
	{
		pDC->SelectObject(&m_font);
		pDC->SetTextColor(m_crText);
		pDC->SetBkMode(TRANSPARENT);
	}
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	if(m_Type == Background)
	{
		if( !m_bState ) return m_hwndBrush;
	}

	return m_hBrush;
}

void CMinLabel::OnTimer(UINT nIDEvent) 
{
	m_bState = !m_bState;

	switch(m_Type)
	{
		case Text:
			
			if(m_bState) SetWindowText("");
			else SetWindowText(m_strText);

			break;

		case Background:
			InvalidateRect(NULL,FALSE);
			UpdateWindow();
			break;
	}
	
	CStatic::OnTimer(nIDEvent);
}

void CMinLabel::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CString strLink;

	GetWindowText(strLink);
	ShellExecute(NULL,"open",strLink,NULL,NULL,SW_SHOWNORMAL);
		
	CStatic::OnLButtonDown(nFlags, point);
}

BOOL CMinLabel::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (m_hCursor)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

void CMinLabel::ReconstructFont()
{
	m_font.DeleteObject();
	BOOL bCreated = m_font.CreateFontIndirect(&m_lf);

	ASSERT(bCreated);
}

