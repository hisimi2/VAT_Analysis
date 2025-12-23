// CellMapStatic.cpp : implementation file
//

#include "stdafx.h"
//#include "TWM2.h"
#include "CellMapStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCellMapStatic

CCellMapStatic::CCellMapStatic()
:m_IsUp(TRUE)
{
	m_colorText=RGB(255,255,150);
	m_Cell_colorText=RGB(0,0,0);
	m_Cell_color=RGB(150,150,200);
	m_colorBk = RGB( 0, 0, 0 );
	m_CellMapData.Clear();
	m_bTransparent=TRUE;
	m_IsInit = FALSE;
	m_Row = 0;
	m_Col = 0;
}

CCellMapStatic::~CCellMapStatic()
{

}


BEGIN_MESSAGE_MAP(CCellMapStatic, CStatic)
	//{{AFX_MSG_MAP(CCellMapStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCellMapStatic message handlers

//CellMap Draw
void CCellMapStatic::Init(WORD Row,WORD Col)
{
	
	m_Row=Row;
	m_Col=Col;
	//max limit
	if(Row>40) m_Row=40;
	if(Col>20) m_Col=20;
	m_CellMapData.Clear();

}
void CCellMapStatic::DispMap(CELLMAP_DATA CellMap_data,BOOL UnDn)
{
	
}

CRect CCellMapStatic::m_IsCellRect(WORD row,WORD col)
{
	CRect CellRect,Framrect;
	GetClientRect(&Framrect);

	LONG cWidth,cHeight;
	LONG maxRow=(LONG)m_Row,maxCol=(LONG)m_Col;
	if(m_Row>20) maxRow=20;


	LONG r=(LONG)row,c=(LONG)col;

	cWidth=(Framrect.right-2)/(maxCol+1);
	cHeight=(Framrect.bottom-2)/(maxRow+1);

	LONG offsetX,offsetY;

	offsetX=(Framrect.right-cWidth*(maxCol+1))/2;
	offsetY=(Framrect.bottom-cHeight*(maxRow+1))/2;

	CellRect.top	=	cHeight*(r+1)+offsetY;
	CellRect.bottom	=	cHeight*(r+2)+offsetY;
	CellRect.left	=	cWidth*(c+1)+offsetX;
	CellRect.right	=	cWidth*(c+2)+offsetX;

	return CellRect;
}

CRect	CCellMapStatic::m_IsNameRowRect(WORD row)
{
	CRect CellRect,Framrect;
	GetClientRect(&Framrect);

	LONG cWidth,cHeight;
	LONG maxRow=(LONG)m_Row,maxCol=(LONG)m_Col;
	if(m_Row>20) maxRow=20;
	LONG r=(LONG)row,c=0;

	cWidth=(Framrect.right-2)/(maxCol+1);
	cHeight=(Framrect.bottom-2)/(maxRow+1);

	LONG offsetX,offsetY;

	offsetX=(Framrect.right-cWidth*(maxCol+1))/2;
	offsetY=(Framrect.bottom-cHeight*(maxRow+1))/2;

	CellRect.top	=	cHeight*(r+1)+offsetY;
	CellRect.bottom	=	cHeight*(r+2)+offsetY;
	CellRect.left	=	cWidth*(c)+offsetX;
	CellRect.right	=	cWidth*(c+1)+offsetX;

	return CellRect;
}
CRect	CCellMapStatic::m_IsNameColRect(WORD col)
{
	CRect CellRect,Framrect;
	GetClientRect(&Framrect);

	LONG cWidth,cHeight;
	LONG maxRow=(LONG)m_Row,maxCol=(LONG)m_Col;
	if(m_Row>20) maxRow=20;
	LONG r=0,c=(LONG)col;

	cWidth=(Framrect.right-2)/(maxCol+1);
	cHeight=(Framrect.bottom-2)/(maxRow+1);

	LONG offsetX,offsetY;

	offsetX=(Framrect.right-cWidth*(maxCol+1))/2;
	offsetY=(Framrect.bottom-cHeight*(maxRow+1))/2;

	CellRect.top	=	cHeight*(r)+offsetY;
	CellRect.bottom	=	cHeight*(r+1)+offsetY;
	CellRect.left	=	cWidth*(c+1)+offsetX;
	CellRect.right	=	cWidth*(c+2)+offsetX;

	return CellRect;
}

void CCellMapStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	ReDraw(m_IsUp);


	// Do not call CStatic::OnPaint() for painting messages
}

void CCellMapStatic::ReDraw(BOOL bUpDn)
{
	CClientDC dc(this); // device context for painting
	m_IsUp = bUpDn;
	CRect rect;
	rect=m_IsNameRowRect(0);
	int FontSize=4;

	FontSize=(int)((double)(rect.bottom-rect.top)*7);

	if(FontSize>130) 
		FontSize=130;

	int r,c;
	for(r=0;r<20;r++)
	{
		for(c=0;c<20;c++)
		{
			int DRow;
			if(m_IsUp!=TRUE && m_Row>20) DRow=r+20;
			else DRow=r;	 
			//Cell rectSize 

			rect=m_IsCellRect(r,c);
			
			CBrush brush, *pOldBrush;
			CPen pen, * pOldPen;
			CFont newFont,*pOldFont;

			brush.CreateSolidBrush(m_CellMapData.BackColor[r][c]);
			pOldBrush=(CBrush *)dc.SelectObject(&brush);

			pen.CreatePen(PS_SOLID,0,RGB(200,200,200));
			pOldPen=(CPen *)dc.SelectObject(&pen);

			newFont.CreatePointFont(FontSize,_T("Arial"));
			pOldFont = (CFont *)dc.SelectObject(&newFont);
			
			dc.SetTextColor(m_CellMapData.TextColor[r][c]);
			dc.SetBkColor(m_colorBk);

			if(m_bTransparent)	dc.SetBkMode(TRANSPARENT);
			else				dc.SetBkMode(OPAQUE);

			CString strRow;
			if(r<m_Row && c<m_Col)
			{
			
				dc.Rectangle(rect.left,rect.top,rect.right,rect.bottom);
				dc.DrawText(" ",&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
			}
			if(DRow<m_Row && c<m_Col)
			{
			
				dc.Rectangle(rect.left,rect.top,rect.right,rect.bottom);
				dc.DrawText(m_CellMapData.Cell[DRow][c],&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
			}
			dc.SelectObject(pOldPen);
			dc.SelectObject(pOldBrush);
			dc.SelectObject(pOldFont);
		}
	}


	FontSize=(int)((double)(rect.bottom-rect.top)*4);

	if(FontSize>130) 
		FontSize=130;
	//Row Number Label Setting
	for(r=0;r<20;r++)
	{
		CString strRowNo;
		strRowNo.Format("%d",r+1);
		
		//Cell rectSize 

			if(r<m_Row)
			{
				rect=m_IsNameRowRect(r);
				
				CBrush brush, *pOldBrush;
				CPen pen, * pOldPen;
				CFont newFont,*pOldFont;
				

				brush.CreateSolidBrush(RGB(50,50,125));
				pOldBrush=(CBrush *)dc.SelectObject(&brush);

				pen.CreatePen(PS_SOLID,0,RGB(0,0,0));
				pOldPen=(CPen *)dc.SelectObject(&pen);

				newFont.CreatePointFont(FontSize,_T("Arial"));
				pOldFont = (CFont *)dc.SelectObject(&newFont);
				
				dc.SetTextColor(m_colorText);
				dc.SetBkColor(m_colorBk);

				if(m_bTransparent)	dc.SetBkMode(TRANSPARENT);
				else				dc.SetBkMode(OPAQUE);

				int DRow;
				if(m_IsUp!=TRUE && m_Row>15) DRow=r+20;
				else DRow=r;

				CString strRow;
				strRow.Format("%d",DRow+1);

				dc.Rectangle(rect.left,rect.top,rect.right,rect.bottom);
				dc.DrawText(strRow,&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);

				dc.SelectObject(pOldPen);
				dc.SelectObject(pOldBrush);
				dc.SelectObject(pOldFont);
				
				

			}


	}

	//Col Number Label Setting
	if(m_Col>10 && m_Row<5 )
		FontSize=(int)((double)(rect.bottom-rect.top)*4);

	if(FontSize>130) 
		FontSize=130;
	for(c=0;c<20;c++)
	{
		CString strColNo;
		strColNo.Format("%d",c+1);
		
		if(c<m_Col)
		{
			rect=m_IsNameColRect(c);
			
			CBrush brush, *pOldBrush;
			CPen pen, * pOldPen;
			CFont newFont,*pOldFont;
			

			brush.CreateSolidBrush(RGB(50,50,125));
			pOldBrush=(CBrush *)dc.SelectObject(&brush);

			pen.CreatePen(PS_SOLID,0,RGB(0,0,0));
			pOldPen=(CPen *)dc.SelectObject(&pen);

			newFont.CreatePointFont(FontSize,_T("Arial"));
			pOldFont = (CFont *)dc.SelectObject(&newFont);
			
			dc.SetTextColor(m_colorText);
			dc.SetBkColor(m_colorBk);

			if(m_bTransparent)	dc.SetBkMode(TRANSPARENT);
			else				dc.SetBkMode(OPAQUE);

			CString strRow;

			strRow.Format("%d",c+1);

			dc.Rectangle(rect.left,rect.top,rect.right,rect.bottom);
			dc.DrawText(strRow,&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);

			dc.SelectObject(pOldPen);
			dc.SelectObject(pOldBrush);
			dc.SelectObject(pOldFont);

			
		}
	}
}
