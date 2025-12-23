#pragma once


#include "BtnST.h"
#include "ShadeButtonST.h"

// CDlgManuLoadTbl 대화 상자입니다.

class CDlgManuLoadTbl : public CDialogEx, public CThreadBase
{
	DECLARE_DYNAMIC(CDlgManuLoadTbl)

public:
	CDlgManuLoadTbl(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgManuLoadTbl();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANU_LOAD_TABLE };

	enum{
		eClickBtn_Tbl1_2D_Left =0,
		eClickBtn_Tbl1_2D_Right,
		eClickBtn_Tbl2_2D_Left,
		eClickBtn_Tbl2_2D_Right,
		eClickBtn_Stop,
	};

	ST_CYLINDER_SPEC m_dCylinderSpec[eMaxLoadTblCount][eMaxLoadTblLeftRight];
	int m_nCylinderNum;

	void CylinderSpecUpdate();
	void CylinderSpecInOut(CDC* pDC);
	void CylinderSpecIndex(int& nCmdManualCyl);
	void CylinderNum(BOOL bLeftRight);

public:
	int m_nLoadTblIdx;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

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

	void SetCommand(int nCmd);
	void ClearCommand();
	int do_CylCommand(BOOL bUndo=FALSE);
	int Check_CylSensor(BOOL bUndo=FALSE);

	void DisplayCylinderActionTime(BOOL bClear=FALSE);

	void UpdateCylinderSts(int nCtrlId, int nStatus);


	int m_nCmdManualCyl; // picker up/down, vacuum/blow
	int m_nCmdOperation;
	int m_nCmdParam_Time;
	int m_nCmdCycleCount;
	int m_nHorVerTblSel;

	double m_dTimeCheck;
	double m_dCylActionTimeRecord;
	int   m_nCylActionTimeRetry;
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();

private:
	CShadeButtonST m_BtnStop;
	CShadeButtonST m_btnExit;
	CShadeButtonST m_BtnVibrator;

	CComboBox m_cbOperationSel;
	CComboBox m_cbTimeSel;
	CComboBox m_cbTblSel;

// 	CBitmap m_cBmpExit;
// 	CButton m_btnExit;

	CMFCButton m_BtnTbl_Right;
	CMFCButton m_BtnTbl_Left;
	CMFCButton m_BtnTb2_Right;
	CMFCButton m_BtnTb2_Left;


// 	enum _eTimerBarcodeLed {
// 		eTimer_LED1_ON=10,
// 		eTimer_LED1_OFF,
// 		eTimer_LED2_ON,
// 		eTimer_LED2_OFF,
// 	};

public:
	afx_msg void OnBnClickedButtonTblCylActStop();
	afx_msg void OnBnClickedButtonTblBack();
	afx_msg void OnBnClickedButtonTblFor();
	afx_msg void OnBnClickedButtonTblHor();
	afx_msg void OnBnClickedButtonTblVer();
	void ButtonClickLog(int nBtnNum);
	void MakeLog(LPCTSTR fmt, ...);
	void ChangeDialogSize(std::vector<int> vControl);
	afx_msg void OnBnClickedButtonTblSubBack();
	afx_msg void OnBnClickedButtonTblSubFor();
	afx_msg void OnBnClickedButtonTbl1Left();
	afx_msg void OnBnClickedButtonTbl1Right();
	afx_msg void OnBnClickedButtonTbl2Left();
	afx_msg void OnBnClickedButtonTbl2Right();
	afx_msg void OnBnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	//Barcode LED
	enum {
		eBarcode_LED1 = 0, 
		eBarcode_LED2, 
	};
	void BarcodeLED_OnOff(int nOnOff, int nLedIdx);
	BOOL UpdateBarcodeLEDSts(int nOnOff, int nLedIdx);

	int m_nLEDIdx;
	int m_nLEDOnOff;
	afx_msg void OnBnClickedButtonTbl1LedOn();
	afx_msg void OnBnClickedButtonTbl1LedOff();
	afx_msg void OnBnClickedButtonTbl2LedOn();
	afx_msg void OnBnClickedButtonTbl2LedOff();
	afx_msg void OnBnClickedButtonVibrator();
};
