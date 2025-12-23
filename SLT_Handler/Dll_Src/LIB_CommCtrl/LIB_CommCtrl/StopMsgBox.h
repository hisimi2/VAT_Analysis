#pragma once

#include "DEF_LIB_COMMCTRL.h"
#include "resource.h"
#include "MinButton.h"
#include "BitmapPicture.h"

// CStopMsgBox 대화 상자입니다.

class  CStopMsgBox : public CDialog
{
	DECLARE_DYNAMIC(CStopMsgBox)

public:
	int     m_nTimerPosChange;
	int	    m_nDesktopWidth;	
	int	    m_nDesktopHeight;

	CTime	m_tCreateTime;
	CTime	m_tCloseTime;
	int		m_nRes;

	char    m_szErrorCode[10];
	char    m_szSubCode[10];
	char    m_szUnitName[20];
	
	int     m_nFontWidth;
	int     m_nFontHeight;
	char    m_strNo[50];
	char    m_strYes[50];
	char    m_strRetry[50];
	int     m_cy;
	int     m_cx;
	char    m_szTitle[100];
	char    m_szFirstMsg[1024];
	char    m_szSecondaryMsg[1024];	

	UINT    m_icontype;
	UINT    m_type;
	CEdit   m_MyMsgEdit;
	CStopMsgBox(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CStopMsgBox();


	CBitmapPicture m_MyIcon;
	UINT           m_error_type; // Build2.1 Error Disable / Enable

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_STOPMSG };
	CMinButton	m_bMyRecovery;
	CMinButton	m_bMyYes;
	CMinButton	m_bMyRetry3;
	CMinButton	m_bMyRetry;
	CMinButton	m_bMyOk;
	CMinButton	m_bMyNo;
	CMinButton	m_bMyIgnore;
	CMinButton	m_bMyCancel;
	CMinButton	m_bMyAbort;
	int         m_nImagePos;
	CBitmap	    m_ErrorImage;
	HBITMAP		m_BitMap;
	CStatic     m_SetBitmap;

	BOOL SetRecoveryShowWindow(int iShow=1); // Build2.1

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg void OnMyabort();
	afx_msg void OnMycancel();
	afx_msg void OnMyignore();
	afx_msg void OnMyok();
	afx_msg void OnMyok2();
	afx_msg void OnMyretry();
	afx_msg void OnMyyes();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnPaint();
	afx_msg void OnMyno();
	afx_msg void OnMycancel3();
	afx_msg void OnMyretry3();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnRecoveryHelp(); // Build2.1
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);				// By mjna [150203] Stop 또는 Run Message Box 뒤로 숨는 문제
	DECLARE_MESSAGE_MAP()

private:
	CFont m_Font;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
