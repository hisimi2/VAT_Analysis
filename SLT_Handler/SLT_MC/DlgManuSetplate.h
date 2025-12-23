#pragma once
#include "afxwin.h"
#include "ThreadBase.h"
//#include "DlgManuStackerClampSen.h"
#include "ShadeButtonST.h"

// CDlgManuSetplate 대화 상자입니다.

class CDlgManuSetplate : public CDialogEx, public CThreadBase
{
	DECLARE_DYNAMIC(CDlgManuSetplate)

public:
	CDlgManuSetplate(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgManuSetplate();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANU_SETPLATE };

	enum {
		eClickBtn_Main_Up = 0,
		eClickBtn_Main_Down,
		eClickBtn_Sub_Up,
		eClickBtn_Sub_Down,
		eClickBtn_Lock,
		eClickBtn_Unock,
		eClickBtn_Align_Fwd,
		eClickBtn_Align_Back,
		eClickBtn_All_Align_Fwd,
		eClickBtn_All_Align_Back,
		eClickBtn_Stop
	};

	enum eStackerType
	{
		eStackerMain =0,
		eStackerSub,
		eStackerAlignCylinder,
		eMaxStackerType,
	};

	enum eStackerUpDown
	{
		eStackerUp = 0,
		eStackerDown,
		eMaxStackerUpDown,
	};

	ST_CYLINDER_SPEC m_dCylinderSpec[eMaxStackerCount][eMaxStackerType][eMaxStackerUpDown];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	BOOL IsSubUp( int _index );
	BOOL IsLock( int _index );
	void SetBtnDisplay();
	void UpdateCylinderSts(int nCtrlId, int nStatus);
	void do_Vibrator_Cyl( int _StackerIdx, int _OnOff = DEF_FORW );

	void SetCommand(int nTaskerId, int nCmd, int nLocId=0);
	void ClearCommand();
	int do_CylCommand(BOOL bUndo=FALSE);
	int Check_CylSensor(BOOL bUndo=FALSE);
	void DisplayCylinderActionTime(BOOL bClear=FALSE);
	void CylinderManualAction();
	void CylManualOper_None();
	void CylManualOper_Only_One_Action_and_No_Check_Sensor();
	void CylManualOper_Only_One_Action_and_Check_Sensor();
	void CylManualOper_Continue_Action_and_No_Check_Sensor();
	void CylManualOper_Continue_Action_and_Check_Sensor();
	void CylManualOper_Time_Check_Action_Once_and_Sensor_Check();
	void ThreadProc1();

	int m_nCylManualOpActState;
	int m_nCylManualActionStep;

	int m_nCylManualOpActStateBackUp;

	int m_nCmdTaskerId;
	int m_nCmdManualCyl; // enum eCylManualCmd
	int m_nCmdOperation; // enum eCylManualOper
	int m_nCmdParam_Loc;
	int m_nCmdParam_Time; // sec
	int m_nCmdCycleCount;

	int m_nCylinderNum;

	double m_dTimeCheck;
	double m_dCylActionTimeRecord; // sec
	int m_nCylActionTimeRetry;
	//CDlgManuStackerClampSen* m_pStackerSenView;
	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	

private:
	// High Speed
	BOOL	m_bSPEED_CONTROL_1;
	BOOL	m_bSPEED_CONTROL_2;
	BOOL	m_bSPEED_CONTROL_3;
	BOOL	m_bSPEED_CONTROL_4;
	BOOL	m_bSPEED_CONTROL_5;
	BOOL	m_bSPEED_CONTROL_6;
	BOOL	m_bSPEED_CONTROL_7;

	// Stacker #1 ~ #7 
	CMFCButton m_BtnSetplate1_Main_Up;
	CMFCButton m_BtnSetplate1_Main_Down;
	CMFCButton m_BtnSetplate1_Sub_Up;
	CMFCButton m_BtnSetplate1_Sub_Down;
	CMFCButton m_BtnSetplate1_Clamp;
	CMFCButton m_BtnSetplate1_Unclamp;

	CMFCButton m_BtnSetplate2_Main_Up;
	CMFCButton m_BtnSetplate2_Main_Down;
	CMFCButton m_BtnSetplate2_Sub_Up;
	CMFCButton m_BtnSetplate2_Sub_Down;
	CMFCButton m_BtnSetplate2_Clamp;
	CMFCButton m_BtnSetplate2_Unclamp;

	CMFCButton m_BtnSetplate3_Main_Up;
	CMFCButton m_BtnSetplate3_Main_Down;
	CMFCButton m_BtnSetplate3_Sub_Up;
	CMFCButton m_BtnSetplate3_Sub_Down;
	CMFCButton m_BtnSetplate3_Clamp;
	CMFCButton m_BtnSetplate3_Unclamp;

	CMFCButton m_BtnSetplate4_Main_Up;
	CMFCButton m_BtnSetplate4_Main_Down;
	CMFCButton m_BtnSetplate4_Sub_Up;
	CMFCButton m_BtnSetplate4_Sub_Down;
	CMFCButton m_BtnSetplate4_Clamp;
	CMFCButton m_BtnSetplate4_Unclamp;

	CMFCButton m_BtnSetplate5_Main_Up;
	CMFCButton m_BtnSetplate5_Main_Down;
	CMFCButton m_BtnSetplate5_Sub_Up;
	CMFCButton m_BtnSetplate5_Sub_Down;
	CMFCButton m_BtnSetplate5_Clamp;
	CMFCButton m_BtnSetplate5_Unclamp;

	CMFCButton m_BtnSetplate6_Main_Up;
	CMFCButton m_BtnSetplate6_Main_Down;
	CMFCButton m_BtnSetplate6_Sub_Up;
	CMFCButton m_BtnSetplate6_Sub_Down;
	CMFCButton m_BtnSetplate6_Clamp;
	CMFCButton m_BtnSetplate6_Unclamp;

	CMFCButton m_BtnSetplate7_Main_Up;
	CMFCButton m_BtnSetplate7_Main_Down;
	CMFCButton m_BtnSetplate7_Sub_Up;
	CMFCButton m_BtnSetplate7_Sub_Down;
	CMFCButton m_BtnSetplate7_Clamp;
	CMFCButton m_BtnSetplate7_Unclamp;

	// Conveyor #1 ~ #7
	CMFCButton m_BtnConveyor_All_Lock;
	CMFCButton m_BtnConveyor_All_Unlock;
	CMFCButton m_BtnConveyor1_Lock;
	CMFCButton m_BtnConveyor1_Unlock;
	CMFCButton m_BtnConveyor2_Lock;
	CMFCButton m_BtnConveyor2_Unlock;
	CMFCButton m_BtnConveyor3_Lock;
	CMFCButton m_BtnConveyor3_Unlock;
	CMFCButton m_BtnConveyor4_Lock;
	CMFCButton m_BtnConveyor4_Unlock;
	CMFCButton m_BtnConveyor5_Lock;
	CMFCButton m_BtnConveyor5_Unlock;
	CMFCButton m_BtnConveyor6_Lock;
	CMFCButton m_BtnConveyor6_Unlock;
	CMFCButton m_BtnConveyor7_Lock;
	CMFCButton m_BtnConveyor7_Unlock;

	// Vibration #1 ~ #4
	CMFCButton m_BtnStacker1_Forward;
	CMFCButton m_BtnStacker1_Back;
	CMFCButton m_BtnStacker2_Forward;
	CMFCButton m_BtnStacker2_Back;
	CMFCButton m_BtnStacker3_Forward;
	CMFCButton m_BtnStacker3_Back;
	CMFCButton m_BtnStacker4_Forward;
	CMFCButton m_BtnStacker4_Back;


	CShadeButtonST m_BtnStop;

public:
	CComboBox m_cbOperationSel;
	CComboBox m_cbTimeSel;
	afx_msg void OnBnClickedButtonSetplateStackerCylActStop();

	CBitmap m_cBmpExit;

	CButton m_btnExit;

	//void ButtonClickLog( int nBtnNum );
	void ButtonClickLog(int nBtnNum, int nIdx);
	void MakeLog(LPCTSTR fmt, ...);
	afx_msg void OnBnClickedBtnStkMainUp1();
	afx_msg void OnBnClickedBtnStkMainUp2();
	afx_msg void OnBnClickedBtnStkMainUp3();
	afx_msg void OnBnClickedBtnStkMainUp4();
	afx_msg void OnBnClickedBtnStkMainUp5();
	afx_msg void OnBnClickedBtnStkMainUp6();
	afx_msg void OnBnClickedBtnStkMainUp7();
	afx_msg void OnBnClickedBtnStkMainDown1();
	afx_msg void OnBnClickedBtnStkMainDown2();
	afx_msg void OnBnClickedBtnStkMainDown3();
	afx_msg void OnBnClickedBtnStkMainDown4();
	afx_msg void OnBnClickedBtnStkMainDown5();
	afx_msg void OnBnClickedBtnStkMainDown6();
	afx_msg void OnBnClickedBtnStkMainDown7();
	afx_msg void OnBnClickedBtnStkSubUp1();
	afx_msg void OnBnClickedBtnStkSubUp2();
	afx_msg void OnBnClickedBtnStkSubUp3();
	afx_msg void OnBnClickedBtnStkSubUp4();
	afx_msg void OnBnClickedBtnStkSubUp5();
	afx_msg void OnBnClickedBtnStkSubUp6();
	afx_msg void OnBnClickedBtnStkSubUp7();
	afx_msg void OnBnClickedBtnStkSubDown1();
	afx_msg void OnBnClickedBtnStkSubDown2();
	afx_msg void OnBnClickedBtnStkSubDown3();
	afx_msg void OnBnClickedBtnStkSubDown4();
	afx_msg void OnBnClickedBtnStkSubDown5();
	afx_msg void OnBnClickedBtnStkSubDown6();
	afx_msg void OnBnClickedBtnStkSubDown7();
	afx_msg void OnBnClickedBtnStkClamp1();
	afx_msg void OnBnClickedBtnStkClamp2();
	afx_msg void OnBnClickedBtnStkClamp3();
	afx_msg void OnBnClickedBtnStkClamp4();
	afx_msg void OnBnClickedBtnStkClamp5();
	afx_msg void OnBnClickedBtnStkClamp6();
	afx_msg void OnBnClickedBtnStkClamp7();
	afx_msg void OnBnClickedBtnStkUnclamp1();
	afx_msg void OnBnClickedBtnStkUnclamp2();
	afx_msg void OnBnClickedBtnStkUnclamp3();
	afx_msg void OnBnClickedBtnStkUnclamp4();
	afx_msg void OnBnClickedBtnStkUnclamp5();
	afx_msg void OnBnClickedBtnStkUnclamp6();
	afx_msg void OnBnClickedBtnStkUnclamp7();
	afx_msg void OnBnClickedBtnStkAllForward();
	afx_msg void OnBnClickedBtnStkForward1();
	afx_msg void OnBnClickedBtnStkForward2();
	afx_msg void OnBnClickedBtnStkForward3();
	afx_msg void OnBnClickedBtnStkForward4();
	afx_msg void OnBnClickedBtnStkForward5();
	afx_msg void OnBnClickedBtnStkForward6();
	afx_msg void OnBnClickedBtnStkForward7();
	afx_msg void OnBnClickedBtnStkAllUnlock();
	afx_msg void OnBnClickedBtnStkBack1();
	afx_msg void OnBnClickedBtnStkBack2();
	afx_msg void OnBnClickedBtnStkBack3();
	afx_msg void OnBnClickedBtnStkBack4();
	afx_msg void OnBnClickedBtnStkBack5();
	afx_msg void OnBnClickedBtnStkBack6();
	afx_msg void OnBnClickedBtnStkBack7();
	DECLARE_EVENTSINK_MAP()
	void MouseUpColorbtctrlConveyor1Back(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlConveyor1Back(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlConveyor1Back2(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlConveyor1Back2(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlConveyor1Back3(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlConveyor1Back3(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlConveyor1Back4(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlConveyor1Back4(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlConveyor1Back5(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlConveyor1Back5(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlConveyor1Back6(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlConveyor1Back6(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlConveyor1Back7(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlConveyor1Back7(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlConveyor1For(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlConveyor1For(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlConveyor1For2(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlConveyor1For2(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlConveyor1For3(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlConveyor1For3(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlConveyor1For4(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlConveyor1For4(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlConveyor1For5(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlConveyor1For5(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlConveyor1For6(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlConveyor1For6(short Button, short Shift, long X, long Y);
	void MouseUpColorbtctrlConveyor1For7(short Button, short Shift, long X, long Y);
	void MouseDownColorbtctrlConveyor1For7(short Button, short Shift, long X, long Y);
	afx_msg void OnBnClickedBtnStk1VibrationForward();
	afx_msg void OnBnClickedBtnStk1VibrationBack();
	afx_msg void OnBnClickedBtnStk2VibrationForward();
	afx_msg void OnBnClickedBtnStk2VibrationBack();
	afx_msg void OnBnClickedBtnStk3VibrationForward();
	afx_msg void OnBnClickedBtnStk3VibrationBack();
	afx_msg void OnBnClickedBtnStk4VibrationForward();
	afx_msg void OnBnClickedBtnStk4VibrationBack();
	afx_msg void OnBnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	void CylinderSpecIndex(int& nCmdManualCyl, int& nCmdParam_Loc);
	void CylinderSpecInOut(CDC* pDC);
	void CylinderSpecUpdate(); 
	void CylinderNum(int nNum);
};
