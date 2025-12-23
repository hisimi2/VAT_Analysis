// MinButton.cpp : implementation file
//

#include "stdafx.h"
#include "MinButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMinButton

CMinButton::CMinButton()
{
	m_SetColor = FALSE;
	m_bColorTab = TRUE;

	m_hIcon = 0;
	m_hBitmap = 0;

	m_bDefault = FALSE;
	m_nOldState = 0;
	m_nOldAction = 0;

	m_nImageOffsetFromBorder = 4;
	m_nTextOffsetFromImage = 8;
	m_bUseOffset = TRUE;

	m_crColorTab = COLORREF(MCLOUDBLUE);

	memset(&m_logFont,0, sizeof(LOGFONT));
	m_nTextHeight = 0;
	m_bTextBold = FALSE;

	m_nImageWidth = 0; 
	m_nImageHeight = 0;

	m_textColor = 0;
	m_bkColor = 0;
	m_hBitmapDisabled = 0;
}

CMinButton::~CMinButton()
{
	DestroyIcon(m_hIcon);
}

BEGIN_MESSAGE_MAP(CMinButton, CButton)
	//{{AFX_MSG_MAP(CMinButton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMinButton message handlers

inline void CMinButton::Redraw()
{
	if( m_hWnd != NULL ) Invalidate();
}

//	Various Attribute setting functions
BOOL CMinButton::HasImage() 
{ 
   if( m_hIcon != 0  || m_hBitmap != 0 ) return TRUE;
   else return FALSE;
}

//	Image functions
BOOL CMinButton::SetIcon( UINT nID, int nWidth, int nHeight )
{
	m_hIcon = (HICON)::LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(nID), IMAGE_ICON, nWidth, nHeight, 0);
	if( m_hIcon == 0 ) return FALSE;

	m_nImageWidth = nWidth;
	m_nImageHeight = nHeight;
	m_hBitmap = 0;

	Redraw();
	return TRUE;
}

BOOL CMinButton::SetBitmap( UINT nID, int nWidth, int nHeight )
{
	return SetBitmapCommon( nID, nWidth, nHeight, 0, FALSE /* no mask */ );
}

BOOL CMinButton::SetMaskedBitmap( UINT nID, int nWidth, int nHeight, COLORREF crTransparentMask )
{
	return SetBitmapCommon( nID, nWidth, nHeight, crTransparentMask, TRUE /* mask */ );
}

BOOL CMinButton::SetBitmapCommon( UINT nID, int nWidth, int nHeight, COLORREF crTransparentMask, BOOL bUseMask )
{
	// If it is not a masked bitmap, just go through the
	// motions as if it was, but set then number of color mappings to 0
	COLORMAP	mapColor;
	mapColor.from = crTransparentMask;
	mapColor.to  = ::GetSysColor( COLOR_BTNFACE );
	HBITMAP bmTemp = (HBITMAP)::CreateMappedBitmap(AfxGetApp()->m_hInstance, nID, IMAGE_BITMAP, &mapColor, bUseMask ? 1 : 0 );
	m_hBitmap = (HBITMAP)::CopyImage( bmTemp, IMAGE_BITMAP, nWidth, nHeight, 0 );

	// Create disabled bitmap.  We need to make the masked area white so
	// it will appear transparent when the bitmap is rendered as an
	// 'embossed' (disabled) image in DrawItem() below.  Since DrawState 
	// converts the image to monochrome, white is transparent, black is 
	// graphics data.
	mapColor.to  = RGB( 255, 255, 255 );
	bmTemp = (HBITMAP)::CreateMappedBitmap(AfxGetApp()->m_hInstance, nID, IMAGE_BITMAP, &mapColor, bUseMask ? 1 : 0 );
	m_hBitmapDisabled = (HBITMAP)::CopyImage( bmTemp, IMAGE_BITMAP, nWidth, nHeight, 0 );
  
	if( m_hBitmap == 0 || m_hBitmapDisabled == 0 ) return FALSE; 

	m_nImageWidth = nWidth;
	m_nImageHeight = nHeight;
	m_hIcon = 0;

	Redraw();
	return TRUE;
}

void CMinButton::SetColorTab(COLORREF crTab)
{	
	m_bColorTab = TRUE;
	m_crColorTab = crTab;

	Redraw();
}

BOOL CMinButton::SetDefaultButton( BOOL bState )
{
	CDialog *pDialog = (CDialog *)GetOwner();
	ASSERT( pDialog->IsKindOf( RUNTIME_CLASS( CDialog ) ) );

	pDialog->SetDefID( GetDlgCtrlID() );

	BOOL bPrevious = m_bDefault;
	m_bDefault = bState;

	Redraw();
	// Return previous state
	return bPrevious;
}

//	Positioning Functions
int CMinButton::SetImageOffset( int nPixels )
{
	int nPrevious = m_nImageOffsetFromBorder;

	m_bUseOffset = TRUE; 
	m_nImageOffsetFromBorder = nPixels; 

	Redraw();
	return nPrevious;
}

int CMinButton::SetTextOffset( int nPixels ) 
{ 
	int nPrevious = m_nTextOffsetFromImage;

	m_bUseOffset = TRUE; 
	m_nTextOffsetFromImage = nPixels; 

	Redraw();
	return nPrevious;
}

CPoint CMinButton::SetImagePos( CPoint p ) 
{ 
	CPoint pointPrevious = m_pointImage;

	m_bUseOffset = FALSE; 
	m_pointImage = p; 

	Redraw();
	return pointPrevious;
}

CPoint CMinButton::SetTextPos( CPoint p ) 
{ 
	CPoint pointPrevious = m_pointText;

	m_bUseOffset = FALSE; 
	m_pointText = p; 

	Redraw();
	return pointPrevious;
}

//	Centering a point helper function
void CMinButton::CheckPointForCentering( CPoint &p, int nWidth, int nHeight )
{
	CRect	rectControl;
	GetClientRect( rectControl );

	if( p.x == MINBUTTON_CENTER )
		p.x = ( ( rectControl.Width() - nWidth ) >> 1 );
	if( p.y == MINBUTTON_CENTER )
		p.y = ( ( rectControl.Height() - nHeight ) >> 1 );
}

void CMinButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	CRect	rectControl( lpDrawItemStruct->rcItem );
	
	UINT	nOffset        = 0;			// For position adjustment of a pushed button
	UINT	nFrameStyle    = 0;
	BOOL	bDRAWFOCUSONLY = FALSE;	// Optimize a bit
	int		nStateFlag     = 0;				// Normal or Disabled
	HBITMAP	hBitmapToDraw;			// Normal or Disabled bitmap (not used if uses icon)
	
	UINT	nNewState = lpDrawItemStruct->itemState;
	UINT	nNewAction = lpDrawItemStruct->itemAction;

	//	Find out what state the control and set some drawing flags 
	//	according to the state.
	if ( nNewState & ODS_SELECTED)
	{
		nFrameStyle = DFCS_PUSHED;
		nOffset += 1;
	}

	if( nNewState & ODS_DISABLED )
	{
		nStateFlag = DSS_DISABLED;
		hBitmapToDraw = m_hBitmapDisabled;
	}
	else
	{
		nStateFlag = DSS_NORMAL;
		hBitmapToDraw = m_hBitmap;
	}

	// If only the focus is changing, don't redraw the whole control
	if (nNewAction == ODA_FOCUS )
		bDRAWFOCUSONLY = TRUE;

	// If this is the defualt button, deflate the control so everything 
	// we do below (icon, text, focus ) is adjusted properly
	if( m_bDefault )
		rectControl.DeflateRect( -2, -2 );
//		rectControl.DeflateRect( 1, 1 );

	if( !bDRAWFOCUSONLY )
	{
		// Draw 'default button' rectangle
		if( m_bDefault ) // Can't use ODS_DEFAULT w/owner draw!!
		{
			CPen *pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);
							
			pDC->Rectangle( &lpDrawItemStruct->rcItem ); // don't use deflated rectangle
			pDC->SelectObject( pOldPen );
		}

		//	Draw button frame
		pDC->DrawFrameControl(&rectControl, DFC_BUTTON, DFCS_BUTTONPUSH | nFrameStyle);

		//	Draw color tab
		if( m_bColorTab )
		{
			CPen penTab; 
			
			#define COLORTABSIZE 16
			if( penTab.CreatePen( PS_SOLID, 1, m_crColorTab) )
			{
				CPen* pOldPen = pDC->SelectObject( &penTab );

				if( m_SetColor )
				{
					CRect trect = rectControl;
					trect.left += 2;
					trect.top += 2;
					trect.right -= 2;
					trect.bottom -= 2;
					CBrush pBrush( m_bkColor );
					pDC->FillRect( trect,&pBrush );
				}

				int nXOffset = rectControl.left+1 + nOffset;
				int nYOffset = rectControl.top+1 + nOffset;
				for (UINT nStep = 0; nStep < COLORTABSIZE; nStep++)
				{
					pDC->MoveTo( nXOffset, nYOffset + nStep );
					pDC->LineTo( nXOffset + (COLORTABSIZE-nStep)-1, nYOffset + nStep );			
				}
      
				pDC->SelectObject( pOldPen );
				penTab.DeleteObject();	// By mjna[140814]: Debugging Not Respond (4)
			}
		}

		//	Get control text
		CString		strTitle;
		this->GetWindowText(strTitle);

		//	Draw Image
		if( HasImage() )
		{
			CPoint pt;

			if(m_bUseOffset)
			{
				pt.x = strTitle.IsEmpty() ? MINBUTTON_CENTER : rectControl.left + m_nImageOffsetFromBorder;
				pt.y = MINBUTTON_CENTER;
			}
			else
				pt = m_pointImage;

			CheckPointForCentering( pt, m_nImageWidth, m_nImageHeight );
			
			pt.Offset( nOffset, nOffset );

			if( m_hIcon )
				pDC->DrawState( pt, CSize(m_nImageWidth, m_nImageHeight), (HICON)m_hIcon, DST_ICON | nStateFlag, (CBrush *)NULL );
			else if( m_hBitmap )
				pDC->DrawState( pt, CSize(m_nImageWidth, m_nImageHeight), (HBITMAP)hBitmapToDraw, DST_BITMAP | nStateFlag );
		}

		//	Draw Text
		if ( !strTitle.IsEmpty() )
		{
			CPoint pt;
			CSize sizeText;
			int nOffsetFixY;

			CString strTmp;
			CFont newFont, *pOldFont = NULL;

/*************************************
			TEXTMETRIC tm;
			int nTotalHeight;
			pDC->GetTextMetrics(&tm);
			nTotalHeight = tm.tmHeight + tm.tmExternalLeading;
**************************************/

			if( m_nTextHeight )
			{
				pDC->GetTextFace(strTmp);
				lstrcpy(m_logFont.lfFaceName, strTmp); 

				m_logFont.lfHeight = m_nTextHeight;

				m_logFont.lfWidth = 0;
				m_logFont.lfEscapement = 0;
				m_logFont.lfOrientation = 0;

				if( m_bTextBold ) m_logFont.lfWeight = FW_BOLD;
				else m_logFont.lfWeight = FW_NORMAL;

				m_logFont.lfItalic = FALSE;
				m_logFont.lfUnderline = FALSE;
				m_logFont.lfStrikeOut = FALSE;
//				m_logFont.lfCharSet = HANGUL_CHARSET;
//				m_logFont.lfOutPrecision = OUT_STROKE_PRECIS;	//OUT_DEFAULT_PRECIS
//				m_logFont.lfClipPrecision = CLIP_STROKE_PRECIS;	//CLIP_DEFAULT_PRECIS
				m_logFont.lfCharSet = DEFAULT_CHARSET;
				m_logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
				m_logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
				m_logFont.lfQuality = DEFAULT_QUALITY;
	//			m_logFont.lfPitchAndFamily = FIXED_PITCH;			//DEFAULT_PITCH
				m_logFont.lfPitchAndFamily = DEFAULT_PITCH;

				if( newFont.CreateFontIndirect(&m_logFont) )
					pOldFont = pDC->SelectObject(&newFont);
			}

			if( strTitle.Find('\n') == -1 )
			{
				sizeText = pDC->GetTextExtent(strTitle);

				if(m_bUseOffset)
				{
					pt.x = !HasImage() ? MINBUTTON_CENTER : m_nImageWidth + m_nTextOffsetFromImage + m_nImageOffsetFromBorder;
					pt.y = MINBUTTON_CENTER; 
				}
				else
					pt = m_pointText;

				//	If we are centering the text vertically, it looks best of we
				//	center based on the height of the text, then move it up 1 more pixel
				nOffsetFixY = pt.y == 0 ? -1 : 0;

				CheckPointForCentering( pt, sizeText.cx, sizeText.cy );

				pt.Offset( nOffset, nOffset + nOffsetFixY );

				if( m_SetColor)
				{
					pDC->SetTextColor(COLORREF(m_textColor));
					pDC->SetBkColor(m_bkColor);
				}

				pDC->DrawState( pt, CSize(0,0), strTitle, DST_PREFIXTEXT|nStateFlag, TRUE, 0, (CBrush*)NULL );
			}
			else
			{
				// By mjna[140708]:Debug Error <start>
				char sbuf[256] = {0};
				char str[4][256];
				int len[4]={0},i=0;
				char* p = NULL;
				int cnt=0;

				memset(sbuf, 0x00, sizeof(sbuf));
				memset(str, 0x00, sizeof(str));

				for(i=0;i<4;i++) len[i]=0;

				strncpy(sbuf, strTitle, sizeof(sbuf)-1);
				// By mjna[140708]:Debug Error <start>

				p = strtok(sbuf,"\n");
				while( p != NULL )
				{
					strcpy(str[cnt],p);
					len[cnt] = strlen(str[cnt]);
					p = strtok(NULL,"\n");
					cnt++;
					if(cnt>3) break;

					Sleep(1);
				}

				CString MyStr;
				CSize MyStrSize;
						
				MyStr = str[0];
				MyStrSize = pDC->GetTextExtent(MyStr);

				if(m_bUseOffset)
					pt.y = ((rectControl.bottom - rectControl.top) - (MyStrSize.cy * cnt) ) / 2;
				else
					pt.y = m_pointText.y;

				for(i=0;i<4;i++)
				{
					if(len[i])
					{
						MyStr = str[i];
						MyStrSize = pDC->GetTextExtent(MyStr);
						if(HasImage() && m_bUseOffset)
							pt.x = m_nImageWidth + m_nTextOffsetFromImage + m_nImageOffsetFromBorder;
						else 
							pt.x = ((rectControl.right - rectControl.left) / 2) - (MyStrSize.cx / 2);

						if( m_SetColor)
						{
							pDC->SetTextColor(COLORREF(m_textColor));
							pDC->SetBkColor(m_bkColor);
						}

						pDC->DrawState( pt, CSize(0,0), MyStr, DST_PREFIXTEXT|nStateFlag, TRUE, 0, (CBrush*)NULL );
					}
					pt.y += MyStrSize.cy;
				}
			}

			if( m_nTextHeight )
			{
				if(pOldFont) pDC->SelectObject(pOldFont);
			}
		}
	} // End !focus only

	//	Draw focus rectange
	if( !( nNewState & ODS_DISABLED ) )	// Make sure it's not disabled
	{
		// Redraw the focus if:
		//		1. There is a change in focus state 
		// OR	2. The entire control was just redrawn and Focus is set
		if( ( nNewState && ODS_FOCUS ) ^ ( m_nOldState && ODS_FOCUS ) ||
			( !bDRAWFOCUSONLY && ( nNewState & ODS_FOCUS ) ) )
		{
			#define FOCUSOFFSET 3
			CRect rect( rectControl );

			// As control gets smaller, decrease focus size
			int nDeflate = min( FOCUSOFFSET,
								min( rect.Width(), rect.Height() ) >> 2 );
			rect.DeflateRect( nDeflate, nDeflate);
			pDC->DrawFocusRect(&rect);
		}
	}

	//UINT aa = GetButtonStyle();

	m_nOldAction = nNewAction;
	m_nOldState = nNewState;

	ReleaseDC(pDC);
}

void CMinButton::SetColor(COLORREF bkcolor, COLORREF textcolor/*= BLACK*/)
{
	m_SetColor = TRUE;
	m_bkColor = bkcolor;
	m_textColor = textcolor;

	Redraw();
}

void CMinButton::SetTextSize(int textsize, BOOL bold)
{
	const int POINTS_PER_INCH = 72;
	int nDot;
	CClientDC dc(this);

	nDot = (textsize * dc.GetDeviceCaps(LOGPIXELSY)) / POINTS_PER_INCH;

	m_nTextHeight = nDot;
	m_bTextBold = bold;
	ReleaseDC(&dc);
	Redraw();
}
