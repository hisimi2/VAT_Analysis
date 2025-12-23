#pragma once

#include "ShadeButtonST.h"
#include "BtnST.h"
#include "../Include/OcxFiles/spreadsheet.h"

// CDlgManuPressUnit 대화 상자입니다.

class CDlgManuPressUnit : public CDialogEx, public CThreadBase
{
	DECLARE_DYNAMIC(CDlgManuPressUnit)

public:
	CDlgManuPressUnit(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgManuPressUnit();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANU_PRESSUNIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
	
	enum ePressUpDown
	{
		ePressUp = 0,
		ePressDown,
		eMaxPressUpDown,
	};

	ST_CYLINDER_SPEC m_dCylinderSpec[eMaxPressUnitCount][eMaxPressUpDown];

	CBitmap m_cBmpExit;

	CComboBox m_cbOperationSel;
	CComboBox m_cbTimeSel;
	CComboBox m_cbSiteSel;

	CButton m_btnExit;

	CShadeButtonST m_BtnUpAll;
	CShadeButtonST m_BtnAllDown;
	CShadeButtonST m_BtnUpEach;
	CShadeButtonST m_BtnDownEach;
	CShadeButtonST m_BtnStop;

	CShadeButtonST m_BtnVibrator;

	int m_nSenID_SafetyZ[eMaxPressUnitCount];
	//int m_nSenID_Slide[eMaxPressUnitCount];
	int m_nSenID_Up[eMaxPressUnitCount];
	int m_nSenID_Down[eMaxPressUnitCount];

	afx_msg void OnBnClickedButtonBlowPress1();
	afx_msg void OnBnClickedButtonBlowPress2();
	afx_msg void OnBnClickedButtonBlowPress3();
	afx_msg void OnBnClickedButtonBlowPress4();
	afx_msg void OnBnClickedButtonBlowPress5();
	afx_msg void OnBnClickedButtonBlowPress6();
	afx_msg void OnBnClickedButtonBlowPress7();
	afx_msg void OnBnClickedButtonBlowPress8();
	afx_msg void OnBnClickedButtonBlowPress9();
	afx_msg void OnBnClickedButtonBlowPress10();
	afx_msg void OnBnClickedButtonBlowPress11();
	afx_msg void OnBnClickedButtonBlowPress12();
	afx_msg void OnBnClickedButtonBlowPress13();
	afx_msg void OnBnClickedButtonBlowPress14();
	afx_msg void OnBnClickedButtonBlowPress15();
	afx_msg void OnBnClickedButtonBlowPress16();

	void MakeLog(LPCTSTR fmt, ...);
	afx_msg void OnBnClickedButtonEachUp();
	afx_msg void OnBnClickedButtonEachDown();
	int do_CylCommand(BOOL bUndo=FALSE);
	int Check_CylSensor(BOOL bUndo=FALSE);
	void ThreadProc1();
	void CylinderManualAction();
	void CylManualOper_None();
	void CylManualOper_Only_One_Action_and_No_Check_Sensor();
	void CylManualOper_Only_One_Action_and_Check_Sensor();
	void CylManualOper_Continue_Action_and_No_Check_Sensor();
	void CylManualOper_Continue_Action_and_Check_Sensor();
	void CylManualOper_Time_Check_Action_Once_and_Sensor_Check();
	void DisplayCylinderActionTime(BOOL bClear= FALSE);
	void ClearCommand();
	void SetCommand(int nCmd);
	void GetSiteSel(std::vector<int>& nSiteSel);
	void SetBtnDisplay();
	void ChangeDialog();

	void CylinderSpecIndex(int& nCmdManualCyl, int& nCmdSite);
	void CylinderSpecInOut(CDC* pDC);
	void CylinderSpecUpdate();
	void CylinderNum(BOOL bUpDown);

	int m_nCylManualOpActState;
	int m_nCylManualActionStep;
	int m_nCmdCycleCount;
	double m_dTimeCheck;
	double m_dCylActionTimeRecord;

	int m_nCmdManualCyl; // picker up/down, vacuum/blow
	int m_nCmdOperation;
	int m_nCmdParam_Time;
	int m_nCylinderNum;
	std::vector<int> m_vCmdSite;

	afx_msg void OnBnClickedButtonAllUp();
	afx_msg void OnBnClickedButtonAllDown();
	afx_msg void OnBnClickedButtonCtylActStop();
	CSpreadSheet m_SpreadSiteDown;



private:
	void InitSpreadSheet();
public:
	afx_msg void OnBnClickedButtonVibrator();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
