#if !defined(AFX_MINLABEL_H__18419A46_B768_11D2_AE44_C04D05C10000__INCLUDED_)
#define AFX_MINLABEL_H__18419A46_B768_11D2_AE44_C04D05C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MinLabel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMinLabel window
enum FlashType {None, Text, Background };

class CMinLabel : public CStatic
{
// Construction
public:
	CMinLabel();

	CMinLabel& SetColor(COLORREF crText, COLORREF crBkgnd);
	CMinLabel& SetText(const CString& strText, COLORREF crText, COLORREF crBkgnd);
	CMinLabel& SetBkColor(COLORREF crBkgnd);
	CMinLabel& SetTextColor(COLORREF crText);
	CMinLabel& SetText(const CString& strText);
	CMinLabel& SetFontBold(BOOL bBold);
	CMinLabel& SetFontName(const CString& strFont);
	CMinLabel& SetFontUnderline(BOOL bSet);
	CMinLabel& SetFontItalic(BOOL bSet);
	CMinLabel& SetFontSize(int nSize);
	CMinLabel& SetSunken(BOOL bSet);
	CMinLabel& SetBorder(BOOL bSet);
	CMinLabel& FlashText(BOOL bActivate);
	CMinLabel& FlashBackground(BOOL bActivate);
	CMinLabel& SetFlashBkColor(COLORREF crBkgnd);
	CMinLabel& SetLink(BOOL bLink);
	CMinLabel& SetLinkCursor(HCURSOR hCursor);

// Attributes
public:

protected:
	void ReconstructFont();
	COLORREF	m_crText;
	HBRUSH		m_hBrush;
	HBRUSH		m_hwndBrush;
	LOGFONT		m_lf;
	CFont		m_font;
	CString		m_strText;
	BOOL		m_bState;
	BOOL		m_bTimer;
	BOOL		m_bLink;
	FlashType	m_Type;
	HCURSOR		m_hCursor;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMinLabel)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMinLabel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMinLabel)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MINLABEL_H__18419A46_B768_11D2_AE44_C04D05C10000__INCLUDED_)
