
// SLT_MCView.h : CSLT_MCView 클래스의 인터페이스
//

#pragma once

#include "resource.h"
#include "afxwin.h"
#include "ShadeButtonST.h"

class CSLT_MCView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CSLT_MCView();
	DECLARE_DYNCREATE(CSLT_MCView)

public:
	enum{ IDD = IDD_SLT_MC_FORM };

// 특성입니다.
public:
	CSLT_MCDoc* GetDocument() const;


// 작업입니다.
public:

private:
	void Init_GlobalObject();
	void Close_GlobalObject();

	void InitControl();


// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CSLT_MCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnMcMenuSetup();
	afx_msg void OnBnClickedBtnMcMenuManual();
	afx_msg void OnBnClickedBtnMcMenuMonitor();
	afx_msg void OnBnClickedBtnMcMenuClose();
	afx_msg void OnBnClickedBtnMcMenuShutDown();
	CShadeButtonST m_btnSetup;
	CShadeButtonST m_btnManual;
	CShadeButtonST m_btnMonitor;
	CShadeButtonST m_btnClose;
	CShadeButtonST m_btnExit;
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	afx_msg void OnBnClickedBtnMcMenuDioSim();
	afx_msg void OnBnClickedBtnMcMenuAioSim();
	afx_msg void OnBnClickedBtnMcMenuMotionSim();
	afx_msg void OnBnClickedBtnMcMenuShowDebugView();

private:
    BOOL CloseProcess(CString sExeName);
	void MakeHandlerRejectBin();
    
public:
	afx_msg void OnBnClickedBtnWmxReconenect();

	int m_nProgressInitCnt;
};

#ifndef _DEBUG  // SLT_MCView.cpp의 디버그 버전
inline CSLT_MCDoc* CSLT_MCView::GetDocument() const
   { return reinterpret_cast<CSLT_MCDoc*>(m_pDocument); }
#endif

