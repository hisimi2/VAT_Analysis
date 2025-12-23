/*===========================================================================
====                                                                     ====
====    File name           :  StaticTrans.h                             ====
====    Creation date       :  7/10/2001                                 ====
====    Author(s)           :  Dany Cantin                               ====
====                                                                     ====
===========================================================================*/

#ifndef STATICTRANS_H
#define STATICTRANS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CStaticTrans window

#define TRANS_BACK -1

class AFX_EXT_CLASS CStaticTrans : public CStatic
{
    // Construction
public:
    CStaticTrans();
    
private:
    COLORREF m_TextColor;
    COLORREF m_BackColor;
    CBrush   m_Brush;
	CBrush	 m_brBGColor;
	CFont	 m_Font;
	int		 m_nSize;
	int		 m_nWeight;
	char	 m_szFont[256];

    
// Operations
public:
    void SetTextColor(COLORREF col) { m_TextColor = col;
                                      UpdateCtrl();      }
    void SetBackColor(COLORREF col);
	void SetTextSize(int nSize);
    void UpdateCtrl();

	void SetWeight(int nWeight /*FW_BOLD or FW_THIN*/);
	void m_fnSetFont(char *szFont);

private:
    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticTrans)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStaticTrans();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticTrans)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


#endif // STATICTRANS_H
