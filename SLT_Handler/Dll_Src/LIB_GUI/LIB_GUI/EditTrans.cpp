/*===========================================================================
====                                                                     ====
====    File name           :  EditTrans.cpp                             ====
====    Creation date       :  7/10/2001                                 ====
====    Author(s)           :  Dany Cantin                               ====
====                                                                     ====
===========================================================================*/

#include "stdafx.h"
#include "EditTrans.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditTrans

CEditTrans::CEditTrans()
{
	m_TextColor = RGB(0, 0, 0);
	m_BackColor = TRANS_BACK;

	m_nWeight = FW_THIN;
	sprintf_s(m_szFont, sizeof(m_szFont), "MS Sans Serif");
	m_nSize = 11;
	m_Font.CreateFont( m_nSize, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS
		, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY
		, (VARIABLE_PITCH | FF_SWISS), m_szFont);
}

CEditTrans::CEditTrans(char* szFont , int nSize)
{
    m_TextColor = RGB(0, 0, 0);
    m_BackColor = TRANS_BACK;

	m_nWeight = FW_THIN;
	if( szFont == NULL ){
		sprintf_s(m_szFont, sizeof(m_szFont), "MS Sans Serif");
	}else{
		sprintf_s(m_szFont, sizeof(m_szFont), szFont);
	}
	m_nSize = nSize;
	m_Font.CreateFont( m_nSize, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS
								, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY
								, (VARIABLE_PITCH | FF_SWISS), m_szFont);
}

CEditTrans::~CEditTrans()
{
    m_Brush.DeleteObject();
	m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CEditTrans, CEdit)
	//{{AFX_MSG_MAP(CEditTrans)
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
	ON_WM_LBUTTONDOWN()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditTrans message handlers

HBRUSH CEditTrans::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    m_Brush.DeleteObject();

    if (m_BackColor == TRANS_BACK) {
        m_Brush.CreateStockObject(HOLLOW_BRUSH);
		pDC->SelectObject(&m_Font);
        pDC->SetBkMode(TRANSPARENT);
    }
    else {
        m_Brush.CreateSolidBrush(m_BackColor);
		pDC->SelectObject(&m_Font);
        pDC->SetBkColor(m_BackColor);
    }

    pDC->SetTextColor(m_TextColor);

    return (HBRUSH)m_Brush;
}


void CEditTrans::OnKillfocus() 
{
    UpdateCtrl();
}


void CEditTrans::OnUpdate() 
{
    UpdateCtrl();
}


void CEditTrans::OnLButtonDown(UINT nFlags, CPoint point) 
{
    UpdateCtrl();
    
    CEdit::OnLButtonDown(nFlags, point);
}


void CEditTrans::UpdateCtrl()
{
    CWnd* pParent = GetParent();
    CRect rect;    
    GetWindowRect(rect);
    pParent->ScreenToClient(rect);
    rect.DeflateRect(2, 2);    
    pParent->InvalidateRect(rect, FALSE);    
}


void CEditTrans::SetTextSize(int nSize)
{
	m_Font.DeleteObject();

	m_nSize = nSize;
	m_Font.CreateFont(m_nSize, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS
									, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY
									, VARIABLE_PITCH | FF_SWISS, m_szFont);

	CEdit::SetFont(&m_Font);

	// 세로 중앙 정렬
	//ModifyStyle(0, ES_MULTILINE);
	CWnd* pParent = GetParent();
	CRect rect;    
	GetWindowRect(rect);
	pParent->ScreenToClient(rect);
	CRect rText;
	GetRect(rText);
	int h = ((rect.bottom - rect.top) - (m_nSize))/2;
	rText.top = h;
	SetRect(rText);
}


void CEditTrans::SetFont(char* szFont, int nSize)
{
	ZeroMemory(m_szFont, sizeof(m_szFont));
	sprintf_s(m_szFont, sizeof(m_szFont), "%s", szFont);
	SetTextSize( nSize );
}

void CEditTrans::GetFont(char* szFont, int nBuffsize, int* pnFontSize)
{
	sprintf_s(szFont, nBuffsize, "%s", m_szFont);
	*pnFontSize = m_nSize;
}
