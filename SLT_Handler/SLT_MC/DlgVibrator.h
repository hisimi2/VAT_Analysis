#pragma once
#include "afxwin.h"
#include "..\Dll_Src\LIB_IPC_TCP\LIB_IPC_SOCK\PerformanceTime.h"


// CDlgVibrator 대화 상자입니다.

class CDlgVibrator : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgVibrator)


public:
	CDlgVibrator(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgVibrator();

	static UINT useThread(LPVOID lParam);

	CWinThread* m_pThread;

	enum VibratorStep
	{
		standby = 0,
		start ,
		do_Vibrator_On ,
		do_Vibrator_On_Wait,
		do_Vibrator_Off,
		do_Vibrator_Off_Wait,
		
		complete,
	};

	enum eStationNum 
	{
		StationDown_1 = 0,
		StationDown_2,
		StationDown_3,
		StationDown_4,
		StationDown_5,
		StationDown_6,
		StationDown_7,
		StationDown_8,

		MaxStation,
	};

	enum eLoadingTableNum
	{
		LoadingTable_1 = 0,
		LoadingTable_2,
		LoadingTable_3,
		LoadingTable_4,

		MaxLoadingTable,
	};


	VibratorStep m_nOldStep, m_nStep;
	void next(VibratorStep nStep);
	void setStep(VibratorStep nStep) { m_nStep = nStep; };
	bool GetBusy();
	void setValueFromDlg();
	void clearValue();

	int m_nStationNum;
	int m_nLoadingTableNum;
	int m_nExcuteCnt;
	double m_dDelayTime;
	int m_nCurCnt;
	std::vector<int> m_vCmdSite;

	CPerformanceTime tmr;

	void VibrateActionSequence();

	void setFont();
	CFont   m_fontStation;
	CFont   m_fontCnt;
	CFont   m_fontDelay;
	CFont   m_fontCaption;

	CBitmap m_cBmpExit;
	CButton m_btnExit;

	CBitmap m_cBmpStart;
	CButton m_btnStart;
	
	CBitmap m_cBmpStop;
	CButton m_btnStop;

	CEditTrans m_EditCnt;
	CEditTrans m_EditDelayTime;

	std::vector<int> vFWDResoruceID;
	std::vector<int> vBWDResoruceID;
	int m_nUnit;

	int m_nVibCnt;
	double m_dVibDelay;

	CComboBox m_ComboStaionNum;
	CFont m_CmbFont;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIBRATOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedVibratorStart();
	afx_msg void OnBnClickedVibratorStop();

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL DestroyWindow();
	afx_msg void OnEnSetfocusEditCnt();
	afx_msg void OnEnSetfocusEditDelay();

};
