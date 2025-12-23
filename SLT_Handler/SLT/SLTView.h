
// SLTView.h : CSLTView 클래스의 인터페이스
//

#pragma once

#include "resource.h"
#include "afxwin.h"
// 문성림
#include "SLTDoc.h"

#define UM_AUTO_SKIP_UPDATE			(WM_USER + 100)

class CSLTView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CSLTView();
	DECLARE_DYNCREATE(CSLTView)
public:
	enum eBTN_SHOW_WINDOW
	{
		SW_MAIN_WINDOW = 0,
		SW_LEVEL_USER,
		SW_LEVEL_SETUP,
		SW_LEVEL_ENGR,
		SW_LEVEL_SUPER,
		DESTROY_ALL_DLG,
	};
public:
	enum{ IDD = IDD_SLT_MAIN_UPPER };

	void EnableAutoGRRCancel();
	void EnableAutoGRRResult();
	void DisableAutoGRRCancel();
	void DisableAutoGRRResult();
	void EnableAutoLOOPCancel();
	void DisableAutoLOOPCancel();
	void EnableAutoAuditCancel();

	double m_tcLoginTime;
	static int m_nStatic;
// 특성입니다.
public:
	CSLTDoc* GetDocument() const;

// 작업입니다.
public:
	// 문성림
	int m_nLogInLv;
	int m_nLoginTime;
	CMainLowerDlg* m_pMainDlg;
// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CSLTView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

private:
	void OnBtnGUI();
	
	void UserLevelDisplayMainUpdate();
	void Init_GlobalObject();
	void Close_GlobalObject();
	void Init_SLT_SetupFile();

	void ButtonClickEvent(int nLevel);
	void TPUT_SocketLock();
	void TPUT_SocketUnLock();

	CShadeButtonST m_BtnUser;
	CShadeButtonST m_BtnSetup;
	CShadeButtonST m_BtnEngr;
	CShadeButtonST m_BtnMain;
	CShadeButtonST m_BtnExit;
	CShadeButtonST m_BtnLogIn;
	CShadeButtonST m_BtnSuper;
	CShadeButtonST m_BtnOneCycle;
	CShadeButtonST m_BtnStop;
	CShadeButtonST m_BtnCancel;
	CShadeButtonST m_BtnLoopCancel;
	CShadeButtonST m_BtnAuditCancel;
	CShadeButtonST m_BtnMultiOrigin;
	CShadeButtonST m_BtnLoadSupply;	
	CShadeButtonST m_BtnCapture;

	CStaticTrans m_StaticCurPressure1;
	CStaticTrans m_StaticCurPressure2;
	CStaticTrans m_StaticCBCheckOnOff;
	CStaticTrans m_StaticSLTMC_Connect;
	CStaticTrans m_StaticLoadSupply;
	CStaticTrans m_StaticMotor_Connect;

	int m_nStaticCurPressure[2];
	int m_nStaticCBCheckOnOff;
	int m_nStaticSLTMC_Connect;
	int m_nStaticPickerMode;
	int m_nStaticLoadSupply;
	int m_nStaticTestHandMoveSensor;
	int m_nStaticTestppStationFrontRearPickPlace;
	int m_nStaticRecontactOnOff;
	int m_nStaticBinSolution_Connect;
	int m_nStaticMotor_Connect;

	char m_szCompany[16];



    CModalDialog *m_pDlgUser;
    CModalDialog *m_pDlgEngr;
    CModalDialog *m_pDlgSetup;
    CModalDialog *m_pDlgMain;
    CModalDialog *m_pDlgSuper;

	BOOL XCopy(LPCTSTR Src, LPCTSTR Dest);
	BOOL ChkOSVerion();
public:
	void CreateViewDialog(int nBtnClickWindow);
    void DestroyDialog();
	void TabSelect(BOOL bCoverLock);
	afx_msg void OnBnClickedBtnLvSetup();
	afx_msg void OnBnClickedBtnLvEngr();	
	afx_msg void OnBnClickedBtnLvMain();
	afx_msg void OnBnClickedBtnLvUser();	
	afx_msg void OnBnClickedBtnExit();	
	afx_msg void OnBnClickedBtnLogin();
	
	afx_msg void OnBnClickedBtnLvSuper();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void Display_OneCycle_GrrCancel_Button();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedBtnOnecycle();
    afx_msg void OnBnClickedBtnStop();
	void MakeLog(LPCTSTR fmt, ...);
    CLogger m_Logger;
	char m_szLogInID[10];
	afx_msg void OnBnClickedBtnCancel();
	afx_msg void OnStnDblclickStaticLogo();

	void LoadProjectName();
	afx_msg void OnBnClickedBtnCap();
	afx_msg void OnBnClickedBtnLoopCancel();
	afx_msg void OnBnClickedBtnAuditCancel();
	afx_msg void OnBnClickedBtnMultiOrigin();
	afx_msg void OnStnClickedCheckTraySupply();
	afx_msg void OnStnDblclickLoadSupply();
};

#ifndef _DEBUG  // SLTView.cpp의 디버그 버전
inline CSLTDoc* CSLTView::GetDocument() const
   { return reinterpret_cast<CSLTDoc*>(m_pDocument); }
#endif

