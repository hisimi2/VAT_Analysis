
// ApTesterView.h : CApTesterView 클래스의 인터페이스
//

#pragma once

#include "ConectSocket.h"

#include "ConfigData.h"
#include "DlgDisplay.h"

class CApTesterView : public CView
{
protected: // serialization에서만 만들어집니다.
	CApTesterView();
	DECLARE_DYNCREATE(CApTesterView)

// 특성입니다.
public:
	CApTesterDoc* GetDocument() const;

// 작업입니다.
public:


// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CApTesterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CConectSocket* m_Socket;	 
	
	static UINT TesterIF(LPVOID lpVoid);
	static UINT TesterIF2(LPVOID lpVoid);

	CWinThread* m_pThreadP1;
	CWinThread* m_pThreadP2;

	CDlgDisplay* pDlg;
	int m_nActionFlag;
	void createDlg();
	BOOL m_bDelSockFlag;

	BOOL bPF;

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // ApTesterView.cpp의 디버그 버전
inline CApTesterDoc* CApTesterView::GetDocument() const
   { return reinterpret_cast<CApTesterDoc*>(m_pDocument); }
#endif

