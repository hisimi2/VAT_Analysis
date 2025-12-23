/*===========================================================================
====                                                                     ====
====    File name           :  StaticTrans.cpp                           ====
====    Creation date       :  7/10/2001                                 ====
====    Author(s)           :  Dany Cantin                               ====
====                                                                     ====
===========================================================================*/

#include "stdafx.h"
#include "StaticTrans.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticTrans

CStaticTrans::CStaticTrans()
{
    m_TextColor = RGB(0, 0, 0);
    m_BackColor = TRANS_BACK;    
 
	m_Brush.CreateStockObject(HOLLOW_BRUSH);
	m_brBGColor.CreateSolidBrush(RGB(0,0,0));

	m_nWeight = FW_THIN;
	sprintf(m_szFont, "MS Sans Serif");

	m_nSize = 20;
	m_Font.CreateFont( m_nSize, 0, 0, 0, m_nWeight, FALSE, FALSE, 0, SHIFTJIS_CHARSET
								, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DRAFT_QUALITY
								, ( DEFAULT_PITCH | FF_DONTCARE ), m_szFont ) ;	

}

CStaticTrans::~CStaticTrans()
{
    m_Brush.DeleteObject();
    m_brBGColor.DeleteObject();
	m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CStaticTrans, CStatic)
	//{{AFX_MSG_MAP(CStaticTrans)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticTrans message handlers

HBRUSH CStaticTrans::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    pDC->SetTextColor(m_TextColor);

    if (m_BackColor == TRANS_BACK) 
	{
		pDC->SelectObject(&m_Font);
        pDC->SetBkMode(TRANSPARENT);
    }
    else {
		pDC->SelectObject(&m_Font);
        pDC->SetBkColor(m_BackColor);

		return m_brBGColor;
    }
        
    return (HBRUSH)m_Brush;
}

void CStaticTrans::UpdateCtrl()
{
    CWnd* pParent = GetParent();
    CRect rect;
    
    GetWindowRect(rect);
    pParent->ScreenToClient(rect);
    rect.DeflateRect(2, 2);
    
    pParent->InvalidateRect(rect, FALSE);    
}

void CStaticTrans::SetTextSize(int nSize)
{
	m_Font.DeleteObject();

	m_nSize = nSize;
	m_Font.CreateFont( m_nSize, 0, 0, 0, m_nWeight, FALSE, FALSE, 0, SHIFTJIS_CHARSET
								, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DRAFT_QUALITY
								, ( DEFAULT_PITCH | FF_DONTCARE ), m_szFont ) ;	
}

void CStaticTrans::SetBackColor(COLORREF col)
{ 
	m_BackColor = col;
	UpdateCtrl();

    m_brBGColor.DeleteObject();
	m_brBGColor.CreateSolidBrush(m_BackColor);
}

void CStaticTrans::SetWeight(int nWeight)
{
	m_Font.DeleteObject();

	m_nWeight = nWeight;
	m_Font.CreateFont( m_nSize, 0, 0, 0, m_nWeight, FALSE, FALSE, 0, SHIFTJIS_CHARSET
								, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DRAFT_QUALITY
								, ( DEFAULT_PITCH | FF_DONTCARE ), m_szFont ) ;	
}

void CStaticTrans::m_fnSetFont(char *szFont)
{
	m_Font.DeleteObject();
	sprintf(m_szFont, "%s", szFont);

	BOOL bFlag = m_Font.CreateFont( m_nSize, 0, 0, 0, m_nWeight, FALSE, FALSE, 0, SHIFTJIS_CHARSET
								, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DRAFT_QUALITY
								, ( DEFAULT_PITCH | FF_DONTCARE ), m_szFont ) ;	

	UpdateCtrl();
}

