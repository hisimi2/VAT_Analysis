#pragma once

#include "ShadeButtonST.h"

// CDlgManuFeeder 대화 상자입니다.

class CDlgManuFeeder : public CDialogEx, public CThreadBase
{
	DECLARE_DYNAMIC(CDlgManuFeeder)

public:
	CDlgManuFeeder(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgManuFeeder();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MANU_FEEDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CShadeButtonST m_BtnStop;
	CShadeButtonST m_btnExit;

	CComboBox m_cbOperationSel;
	CComboBox m_cbTimeSel;

	CMFCButton m_BtnFeeder_Up;
	CMFCButton m_BtnFeeder_Down;
	CMFCButton m_BtnFeeder_Clamp;
	CMFCButton m_BtnFeeder_Unclamp;
	CMFCButton m_BtnFeeder_Lock;
	CMFCButton m_BtnFeeder_Unlock;

	int m_nCmdManualCyl; // picker up/down, vacuum/blow
	int m_nCmdOperation;
	int m_nCmdParam_Time;

	double m_dTimeCheck;
	double m_dCylActionTimeRecord;

	void UpdateCylinderSts(int nCtrlId, int nStatus);

	CTimerTickCheck	m_swTimeout;
public:
	enum {
		eClickBtn_Up = 0,
		eClickBtn_Down,
		eClickBtn_Clamp,
		eClickBtn_Unclamp,
		eClickBtn_Forward,
		eClickBtn_Backward,
		eClickBtn_Stop
	};

	int m_nCylManualOpActState;
	int m_nCylManualActionStep;

	void ButtonClickLog(int nBtnNum);
	void SetCommand(int nCmd, int nPosition = 0, int nIdx = 0);

	void CylinderManualAction();
	void CylManualOper_None();
	void CylManualOper_Only_One_Action_and_No_Check_Sensor();
	void CylManualOper_Only_One_Action_and_Check_Sensor();
	void CylManualOper_Continue_Action_and_No_Check_Sensor();
	void CylManualOper_Continue_Action_and_Check_Sensor();
	void CylManualOper_Time_Check_Action_Once_and_Sensor_Check();
	void CylManualOper_C_Tray_Out_Action();


	void ClearCommand();
	int do_CylCommand(BOOL bUndo = FALSE);
	int Check_CylSensor(BOOL bUndo = FALSE);

	void DisplayCylinderActionTime(BOOL bClear = FALSE);
	void ButtonLockUnlock(BOOL bState);
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	void MakeLog(LPCTSTR fmt, ...);
	void ThreadProc1();
	afx_msg void OnBnClickedBtnFeederUp();
	afx_msg void OnBnClickedBtnFeederDown();
	afx_msg void OnBnClickedBtnFeederClamp();
	afx_msg void OnBnClickedBtnFeederUnclamp();
	afx_msg void OnBnClickedBtnFeederLock();
	afx_msg void OnBnClickedBtnFeederUnlock();
	afx_msg void OnBnClickedButtonFeederCylActStop();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnTrayOut();
};
