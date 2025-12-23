#pragma once
 #include "OcxFiles/sscommand.h"

#include "VATMain_ZAutoFocus.h"
#include "VATMain_CamCenterCalibration.h"
#include "VATMain_PickerAlign.h"
#include "VATMain_ZAutoTeaching.h"
#include "VATMain_VisionAutoTeaching.h"
#include "../VisionAutoTeaching/VAT_DisplayLog.h"
#include "afxcmn.h"
#include "afxwin.h"


//Manual 동작 header
#include "DlgAxisActionMove.h"
#include "DlgManuTestPP.h"
#include "DlgManuLoadTbl.h"
#include "DlgManuFeeder.h"
#include "DlgManuTrayPP.h"
#include "DlgManuTransfer.h"


#include "DlgCommand_TestPP.h"
#include "DlgCommand_LoadTbl.h"
#include "DlgCommand_Feeder.h"
#include "DlgCommand_TrayPP.h"
#include "DlgCommand_Transfer.h"
#include "DlgCommand_Stacker.h"

#include "../VisionAutoTeaching/VAT_VisionSetting.h"

// CVAT_MainDlg 대화 상자입니다.

class CVAT_MainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVAT_MainDlg)

public:
	CVAT_MainDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CVAT_MainDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VAT_MAINDLG };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()

	enum _eTimer
	{
		eTimer_LotState = 0,
		eTimer_UtilityState,

		eTimer_Max,
		//eTimer_LotState = 0,
		//eTimer_CurrentPos,
		//eTimer_ChkVacuum,
	};

	BOOL m_bLastExecuteStatus;

public:
	virtual BOOL OnInitDialog();

	CLogger m_GuiLogger;

private:
	//Multi Origin
	CDlgHomeCheckStatus*      m_pDlgHomeCheckStatus;


	//각각 Mode에 대한 GUI Tap Page
	CVATMain_ZAutoFocus				m_pgZAutoFocus;
	CVATMain_CamCenterCalibration   m_pgCamCenterCal;
	CVATMain_PickerAlign			m_pgPickerAlign;
	CVATMain_ZAutoTeaching			m_pgZAutoTeaching;
	CVATMain_VisionAutoTeaching		m_pgVisionAutoTeaching;

	CTabCtrl m_tabTeachMode;
public:	
	void ClickSscmdZAutoFocus();
	void ClickSscmdCamCenterCal();
	void ClickSscmdPickerAlign();
	void ClickSscmdVat();
	void ClickSscmdZAutoTeaching();
	

	void EnableOperationButton(BOOL bStartBtn, BOOL bPauseBtn, BOOL bStopBtn, BOOL bExitBtn);


public:
	std::vector<int> m_vSSCmdSeqModeId;
	int  m_nSeqMode;
	int  m_nSeqModeBack;

	void SetCurrentMode(int nMode);
	int GetCurrentModeType();

	void PageMove_CurMode_Init(); //Page 이동 시, 해당 Mode 관련 초기화
	void PageMove_BeforeMode_Dispose(); //Page 이동 시, 이전 Mode 정리.

	void SelChangeMainPage();

	void SetMainPageHandle();
	//Display Log
	CShadeButtonST m_BtnLog;
	CVAT_DisplayLog* m_pDlgLog;

	//Origin&Servo
	CShadeButtonST m_BtnMultiOrg;
	CShadeButtonST m_BtnServoMotor;

	//Vision Setting
	CShadeButtonST m_BtnVisionSet;

	//VAT Status
	CStaticTrans m_StaticRunStop;

	//Operate Button
	CShadeButtonST m_BtnStart;
	CShadeButtonST m_BtnStop;
	CShadeButtonST m_BtnInitial;
	CShadeButtonST m_BtnExit;

	//Manual Button & Bitmap
	CButton m_BtnTestPP1; //Test pp
	CButton m_BtnTestPP2;
	CButton m_BtnTable1;  //Loading Table
	CButton m_BtnTable2;
	CButton m_BtnTable3;
	CButton m_BtnTable4;
	CButton m_BtnFeeder;  //Feeder
	CButton m_BtnTrayPP1; //Tray PP
	CButton m_BtnTrayPP2;
	CButton m_BtnTransfer; //Transfer

	CShadeButtonST m_BtnStacker3;
	CShadeButtonST m_BtnStacker4;
	CShadeButtonST m_BtnStacker5;
	CShadeButtonST m_BtnStacker6;
	CShadeButtonST m_BtnStacker7;

	CBitmap m_cBmpTestPP1; //Test pp
	CBitmap m_cBmpTestPP2;
	CBitmap m_cBmpTable1;  //Loading Table
	CBitmap m_cBmpTable2;
	CBitmap m_cBmpTable3;
	CBitmap m_cBmpTable4;
	CBitmap m_cBmpFeeder;  //Feeder
	CBitmap m_cBmpTrayPP1; //Tray PP
	CBitmap m_cBmpTrayPP2;
	CBitmap m_cBmpTransfer; //Transfer

	CButton m_BtnCmd_Skip;
	CButton m_BtnCmd_Stop;
	
	CBitmap m_cBmpCmd_Skip;
	CBitmap m_cBmpCmd_Stop;
	

public:
	void InitMainDlgBtn();
	void InitOperateBtn();
	void InitManualBtn();

	//void DeleteBitmap();

//OnTimer 관련
public:
	
	//Lot 관련 state
	void _tDisplayVATStatus();

	void _tDisplayBusyAlarmStatus();
	void CheckSeqCmdStatus_BusyAlarm(BOOL &bBusy, BOOL &bAlarm);
	void CheckSeqAutoStatus_BusyAlarm(BOOL &bBusy, BOOL &bAlarm);

	void _tDisplayOperBtn();

	void _tDisplayCtrlBtn();

	//Lot 이외 Utility관련 state
	void _tDisplayVisionConnect();

	CString m_strCurStatus;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedShadeBtnLog();
	afx_msg void OnBnClickedShadeBtnMultiorigin();
	afx_msg void OnBnClickedShadeBtnServo();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnSeqStart();
	afx_msg void OnBnClickedBtnSeqPause();
	afx_msg void OnBnClickedBtnSeqInitial();

	//Manual Button Click
	void OnBnClickedAxisManuTestPp(int nTestPpIdx);
	void OnBnClickedAxisManuTrayPp(int nTrayPpIdx);
	void OnBnClickedAxisManuLoadTbl(int nTableIdx);
	void OnBnClickedAxisManuStacker(int nStackerIdx);

	afx_msg void OnBnClickedBtnVatManuTestPp1();
	afx_msg void OnBnClickedBtnVatManuTestPp2();
	afx_msg void OnBnClickedBtnVatManuTable1();
	afx_msg void OnBnClickedBtnVatManuTable2();
	afx_msg void OnBnClickedBtnVatManuTable3();
	afx_msg void OnBnClickedBtnVatManuTable4();
	afx_msg void OnBnClickedBtnVatManuTrayPp1();
	afx_msg void OnBnClickedBtnVatManuTrayPp2();
	afx_msg void OnBnClickedBtnVatManuFeeder();
	afx_msg void OnBnClickedBtnVatManuTransfer();
	afx_msg void OnBnClickedButtonVatManuCmdSkip();
	afx_msg void OnBnClickedButtonVatManuCmdStop();
	afx_msg void OnBnClickedBtnVatManuVisionSetting();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

//Auto Run 관련 함수.
public:
	int CheckVatConditionForStart(CString &strErr);
	BOOL CheckVatAlarmIgnoreForStart();
	void InitModeParamForStart();

	BOOL IsHandOrPartSelected();

	void CreateSelectedTargetByGUI();

	void ChangeMeasurePage();

//Simulation
public:
	void InitSimul();
	afx_msg void OnBnClickedBtnVatManuStacker3();
	afx_msg void OnBnClickedBtnVatManuStacker4();
	afx_msg void OnBnClickedBtnVatManuStacker5();
	afx_msg void OnBnClickedBtnVatManuStacker6();
	afx_msg void OnBnClickedBtnVatManuStacker7();
};
