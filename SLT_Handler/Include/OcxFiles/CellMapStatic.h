#if !defined(AFX_CELLMAPSTATIC_H__93653B01_B591_11D8_9404_A9F521AB9B2B__INCLUDED_)
#define AFX_CELLMAPSTATIC_H__93653B01_B591_11D8_9404_A9F521AB9B2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CellMapStatic.h : header file
//
struct CELLMAP_DATA
{
	CString	 Cell[40][20];
	COLORREF BackColor[40][20];
	COLORREF TextColor[40][20];

	CELLMAP_DATA()
	{
		Clear();
	};

	void Clear()
	{
		for(int i=0;i<40;i++)
		{
			for(int j=0;j<20;j++)
			{
				Cell[i][j]=" ";
				BackColor[i][j]=RGB(250,250,255);
				TextColor[i][j]=RGB(0,0,0);
			}
		}
	};

	void Set_Data(CELLMAP_DATA CellMap)
	{
		for(int i=0;i<40;i++)
		{
			for(int j=0;j<20;j++)
			{
				Cell[i][j]=CellMap.Cell[i][j];
			}
		}
	};

	void Set_CellText(int row,int col,CString data)
	{
		if(row<40 && col <20 ) Cell[row][col]=data;
	};

	
	void Set_CellColor(int row,int col,COLORREF color)
	{
		BackColor[row][col]=color;
	};

	void Set_CellTextColor(int row,int col,COLORREF color)
	{
		TextColor[row][col]=color;
	};
};
/////////////////////////////////////////////////////////////////////////////
// CCellMapStatic window

class CCellMapStatic : public CStatic
{
// Construction
public:
	CCellMapStatic();
	

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCellMapStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCellMapStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCellMapStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	CELLMAP_DATA	m_CellMapData;
	BOOL			m_IsInit;
	WORD m_Row;
	WORD m_Col;
	BOOL m_IsUp;
public:
	void	Init(WORD Row,WORD Col);
	void	DispMap(CELLMAP_DATA CellMap_data,BOOL UnDn=0);
	void	ClearMap(){m_CellMapData.Clear();};
	CRect	m_IsCellRect(WORD row,WORD col);
	CRect	m_IsNameRowRect(WORD row);
	CRect	m_IsNameColRect(WORD col);


public:
	void ReDraw(BOOL bUpDn=1);
	BOOL m_bTransparent;
	COLORREF m_colorText;
	COLORREF m_Cell_colorText;
	COLORREF m_Cell_color;
	COLORREF m_colorBk;
	LOGFONT m_logFont;

	void CellText(int row,int col,CString data){m_CellMapData.Set_CellText(row,col,data);}
	void CellBkColor(int row,int col,COLORREF color){m_CellMapData.Set_CellColor(row,col,color);}
	void CellTextColor(int row,int col,COLORREF color){m_CellMapData.Set_CellTextColor(row,col,color);}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CELLMAPSTATIC_H__93653B01_B591_11D8_9404_A9F521AB9B2B__INCLUDED_)
