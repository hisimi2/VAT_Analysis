/*===========================================================================
====                                                                     ====
====    File name           :  EditTrans.h                               ====
====    Creation date       :  7/10/2001                                 ====
====    Author(s)           :  Dany Cantin                               ====
====                                                                     ====
===========================================================================*/

#ifndef EDITTRANS_H
#define EDITTRANS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CEditTrans window

#define TRANS_BACK -1

class AFX_EXT_CLASS CEditTrans : public CEdit
{
// Construction
public:
	CEditTrans();
	CEditTrans(char* szFont, int nSize);

// Attributes
private:
    COLORREF m_TextColor;
    COLORREF m_BackColor;
    CBrush   m_Brush;
	CFont	 m_Font;
	int		 m_nSize;
	int		 m_nWeight;
	char	 m_szFont[256];

// Operations
public:
    void SetTextColor(COLORREF col) { m_TextColor = col;
                                      UpdateCtrl();      }
    void SetBackColor(COLORREF col) { m_BackColor = col;
                                      UpdateCtrl();      }
	COLORREF GetBackColor() { return m_BackColor; }
	void SetTextSize(int nSize);
	int  GetTextSize() { return m_nSize; };

	void SetFont(char* szFont, int nSize);
	void GetFont(char* szFont, int nBuffSize, int* pnFontSize);
    void UpdateCtrl();

private:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditTrans)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditTrans();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditTrans)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillfocus();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


#endif // EDITTRANS_H
