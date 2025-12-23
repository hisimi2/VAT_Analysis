#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "MainLower_TabYield.h"
#include "MainLower_TabPassFail.h"
#include "MainLower_TabLastDecs.h"
#include "AutoGrrResultDlg.h"
#include "AutoQASetDlg.h"
// CMainLowerDlg 대화 상자입니다.

class CMainLowerDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainLowerDlg)
	
public:
	CMainLowerDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMainLowerDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SLT_MAIN_LOWER };
public:
	enum eMainTabPageIndexs
	{
		MTPI_PASSFAIL,
		MTPI_YIELD_PER,
		MTPI_YIELD_CNT,
		MTPI_LAST_DESC,
		MTPI_DOOR_STATUS,
		MTPI_YIELD_1ST_FAIL,
		MTPI_YIELD_2DID_FAIL,
		MTPI_nr,
	};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void OnInitDialog_MainTabPages();

	afx_msg LRESULT OnUserFuncMsg(WPARAM wParam, LPARAM lParam);
private:
	CWnd* m_pParentWnd;

	void fn_OnUpdateRunMode();
	void fn_OnUpdateYield();
	void fn_OnUpdateContinueFail();
	void fn_OnUpdateAutoSkip();
	void fn_OnUpdateVision2D();
	void fn_OnUpdateVisionTestSite();
    void fn_OnUpdateAutoRetest();
	void fn_OnUpdateAutoGrr();
	void fn_OnUpdateVariance();
	void fn_OnUpdateSocketClean();
	void fn_OnUpdateContactForce();

	void fn_OnUpdateLotData();
	void OnGUI();
	void fn_OnUpdateBinView();
	void fn_OnUpdateLotStatus();
	void OnUpdateUPH();
	void fn_OnUpdateSwitchMode();
	void fn_OnRefreshTab();
	void fn_OnOffControl(int nUseModeNum);
	BOOL CheckEQPRunning();

	int ObsidianFileCreate();
	int DeleteLogFileCreate();

	CListBox m_ListLotData;

	CStaticTrans m_StaticYield;
	CStaticTrans m_StaticContiFail;
	CStaticTrans m_StaticRunMode;
	CStaticTrans m_StaticDvcMode;
	CStaticTrans m_StaticRunStop;
	CStaticTrans m_StaticVision2D;
	CStaticTrans m_StaticVisionTestSite;
	CStaticTrans m_StaticAutoSkip;
    CStaticTrans m_StaticAutoRetest;
	CStaticTrans m_StaticAutoGrr;
	CStaticTrans m_StaticSwitch;

	CStaticTrans m_StaticVariance;
	CStaticTrans m_StaticSocketClean;

	CShadeButtonST m_BtnLotInfo;
	CShadeButtonST m_BtnTrayMap;
	CShadeButtonST m_BtnLotDone;
	CShadeButtonST m_BtnClearCnt;
	CShadeButtonST m_BtnGrrResult;
	CShadeButtonST m_BtnMakeSummary;

	CShadeButtonST m_BtnCSVFile;
	CShadeButtonST m_BtnSummaryCSVFile;
	CShadeButtonST m_Btn2DIDView;
	CShadeButtonST m_BtnEventLog;
	CShadeButtonST m_BtnOEE;
	CShadeButtonST m_BtnHandlerLog;
	CShadeButtonST m_BtnServoMotor;

	CShadeButtonST m_BtnQAModeSet;

	CShadeButtonST m_BtnBatchFile;

	CSpreadSheet m_SprdOutputBinView;
	//CSpreadSheet m_SpredSiteYield;
	CSpreadSheet m_SpredBinCount;
	CSpreadSheet m_SpredArtCnt;

	ST_CATE_INFO m_stCategory;

	int m_nOEEChkLastMin;
	BOOL m_bChkYear;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnLotInfo();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedBtnLotDone();
	afx_msg void OnBnClickedBtnTraymap();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	CMainLower_TabPassFail	m_pgPassFail;
	CMainLower_TabYield		m_pgYieldPer;
	CMainLower_TabYield		m_pgYieldCnt;
	CMainLower_TabDesc		m_pgLastDesc;

	CMainLower_TabYield		m_pgYield1stFail;
	CMainLower_TabYield		m_pgYield2DIDFail;

	CString m_strCategoryName;


	CAutoGrrResultDlg* m_dlgGrrResult;

	int m_nTesterMode;

	int m_nProjectName;
public:
	afx_msg void OnBnClickedBtnVibrator();
    //int m_nSiteYieldInfo;
	afx_msg void OnClose();

	HBRUSH m_hBrushGreen;
	HBRUSH m_hBrushPink;
	HBRUSH m_hBrushBlue;
	HBRUSH m_hBrushPurple;

	CTabCtrl m_tabResult;

	void MakeLog(LPCTSTR fmt, ...);
	afx_msg void OnBnClickedBtnGrrresult();
	afx_msg void OnBnClickedBtnSummary();
	afx_msg void OnTcnSelchangeMainTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnLog();

	void CompanySetting();
	BOOL IsEQPRunning();
	afx_msg void OnBnClickedButton2didCsv();
	afx_msg void OnBnClickedButton2didCsvView();
	afx_msg void OnBnClickedButtonSummary();
	afx_msg void OnStnClickedAutoRetest();
	afx_msg void OnStnClickedTestSiteVision();
	afx_msg void OnStnClickedYieldCtrl();
	afx_msg void OnStnClickedAutoSkip();
	afx_msg void OnStnClicked2dVision();
	afx_msg void OnStnClickedAutoClean();
	afx_msg void OnBnClickedBtnOee();

	enum {
		eYieldCtrlUse = 1,
		eAutoSkipUse = 3,
		eVisionUse = 6,
		eAutoRetestSiteUse = 7,
		e2DBarcodeUse = 8,
		eAutoCleanUse = 12
	};
	afx_msg void OnBnClickedButtonSocketDiagnosis();
	afx_msg void OnBnClickedBtnHandlerLog();
	afx_msg void OnStnClickedStaticSwitch();
	afx_msg void OnBnClickedBtnServoMotor();
	afx_msg void OnBnClickedButtonBatchFile();
	afx_msg void OnBnClickedBtnQaMode();
};
