#if !defined(AFX_MINBUTTON_H__01A8AD41_AC92_11D2_AE44_984D05C10000__INCLUDED_)
#define AFX_MINBUTTON_H__01A8AD41_AC92_11D2_AE44_984D05C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MinButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMinButton window

#define MINBUTTON_CENTER		-1

const COLORREF MCLOUDBLUE = RGB(128, 184, 223);
const COLORREF MWHITE = RGB(255, 255, 255);
const COLORREF MBLACK = RGB(1, 1, 1);
const COLORREF MDKGRAY = RGB(128, 128, 128);
const COLORREF MLTGRAY = RGB(192, 192, 192);
const COLORREF MYELLOW = RGB(255, 255, 0);
const COLORREF MDKYELLOW = RGB(128, 128, 0);
const COLORREF MRED = RGB(255, 0, 0);
const COLORREF MDKRED = RGB(128, 0, 0);
const COLORREF MBLUE = RGB(0, 0, 255);
const COLORREF MDKBLUE = RGB(0, 0, 128);
const COLORREF MCYAN = RGB(0, 255, 255);
const COLORREF MDKCYAN = RGB(0, 128, 128);
const COLORREF MGREEN = RGB(0, 255, 0);
const COLORREF MDKGREEN = RGB(0, 128, 0);
const COLORREF MMAGENTA = RGB(255, 0, 255);
const COLORREF MDKMAGENTA = RGB(128, 0, 128);

class  CMinButton : public CButton
{
// Construction
public:
	CMinButton();

// Attributes
private:
	//	Positioning
	BOOL			m_bUseOffset;				
	CPoint		m_pointImage;
	CPoint		m_pointText;
	int			m_nImageOffsetFromBorder;
	int			m_nTextOffsetFromImage;

	//	Image
	HICON			m_hIcon;					
	HBITMAP		m_hBitmap;
	HBITMAP		m_hBitmapDisabled;
	int			m_nImageWidth, m_nImageHeight;

	//	Color Tab
	char			m_bColorTab;				
	COLORREF		m_crColorTab;

	//	State
	BOOL			m_bDefault;
	UINT			m_nOldAction;
	UINT			m_nOldState;

// Operations
public:
	//	Positioning
	int		SetImageOffset( int nPixels ); 
	int		SetTextOffset( int nPixels );
	CPoint	SetImagePos( CPoint p );
	CPoint	SetTextPos( CPoint p );

	//	Image
	BOOL		SetIcon( UINT nID, int nWidth, int nHeight );
	BOOL		SetBitmap( UINT nID, int nWidth, int nHeight );
	BOOL		SetMaskedBitmap( UINT nID, int nWidth, int nHeight, 
									  COLORREF crTransparentMask );
	BOOL		HasImage();

	//	Color Tab
	void		SetColorTab(COLORREF crTab);

	//	State
	BOOL		SetDefaultButton( BOOL bState = TRUE );
	void		SetColor(COLORREF bkcolor, COLORREF textcolor = MBLACK);
	void		SetTextSize(int textsize, BOOL bold = FALSE);

private:
	BOOL		SetBitmapCommon( UINT nID, int nWidth, int nHeight, 
									  COLORREF crTransparentMask, BOOL bUseMask );
	void		CheckPointForCentering( CPoint &p, int nWidth, int nHeight );
	void		Redraw();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMinButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMinButton();
	COLORREF	m_textColor;
	COLORREF m_bkColor;
	BOOL		m_SetColor;
	BOOL		m_bTextBold;
	int		m_nTextHeight;
	LOGFONT	m_logFont;

// Generated message map functions
protected:
	//{{AFX_MSG(CMinButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MINBUTTON_H__01A8AD41_AC92_11D2_AE44_984D05C10000__INCLUDED_)
