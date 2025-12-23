#pragma once
#include "afxwin.h"

#include "BtnST.h"
#include "ShadeButtonST.h"
#include <vector>

// CDlgManuTestPP 대화 상자입니다.

class CDlgManuTestPP : public CDialogEx, public CThreadBase
{
	DECLARE_DYNAMIC(CDlgManuTestPP)

public:
	CDlgManuTestPP(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgManuTestPP();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANU_TESTPP };
	
	enum {ePickerSel_y1_x1 = 0, ePickerSel_y1_x2, ePickerSel_y1_x3, ePickerSel_y1_x4,		
		ePickerSel_y2_x1, ePickerSel_y2_x2, ePickerSel_y2_x3, ePickerSel_y2_x4,
		ePickerSel_Front_All,ePickerSel_Rear_All,eMaxPickerSelCount
	};

	enum{
		eClickBtn_All_Up =0,
		eClickBtn_All_Down,
		eClickBtn_Each_Up,
		eClickBtn_Each_Down,
		eClickBtn_All_Vacuum,
		eClickBtn_All_Blow,
		eClickBtn_Each_Vacuum,
		eClickBtn_Each_Blow,
		eClickBtn_Stop
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	void GetPickerSel(std::vector<CPoint>& rSelectedPicker);
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

	int m_nCmdManualCyl; // picker up/down, vacuum/blow
	int m_nCmdOperation;
	int m_nCmdParam_Time;
	std::vector<CPoint> m_vCmdParam_Picker;
	int m_nCmdCycleCount;

	double m_dTimeCheck;
	double m_dCylActionTimeRecord;
	int m_nCylActionTimeRetry;

	DECLARE_MESSAGE_MAP()
public:
	int m_nHandIdx;
	int m_nHandType;
private:
	CShadeButtonST m_BtnVacAll;
	CShadeButtonST m_BtnBlwAll;
	CShadeButtonST m_BtnVacEach;
	CShadeButtonST m_BtnBlwEach;
	CShadeButtonST m_BtnUpAll;
	CShadeButtonST m_BtnAllDown;
	CShadeButtonST m_BtnUpEach;
	CShadeButtonST m_BtnDownEach;
	CShadeButtonST m_BtnStop;

	//int m_nHandSenID[eMaxTestPPTypeCount][MAX_TEST_PP_PKR_CNT_Y][MAX_TEST_PP_PKR_CNT_X];
	int m_nHandSenID[eMaxTestPPTypeCount][MAX_TEST_PP_PKR_CNT];
	void DisplayHandSenIdx();

	int m_nCylRepeatStep;

	/*void GetUsePicker(std::vector<int> *vUseHand);*/
	void SetBtnDisplay();

	afx_msg void OnBnClickedButtonAllVacuum();
	afx_msg void OnBnClickedButtonAllBlow();
	afx_msg void OnBnClickedButtonTestPpArmAllUp();
	afx_msg void OnBnClickedButtonTestPpArmAllDown();
	afx_msg void OnBnClickedButtonTestPpHandUp();
	afx_msg void OnBnClickedButtonTestPpHandDown();
	afx_msg void OnBnClickedButtonTestPpHandVacuum();
	afx_msg void OnBnClickedButtonTestPpHandBlow();
public:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();

	CComboBox m_cbOperationSel;
	CComboBox m_cbTimeSel;
	CComboBox m_cbPickerSel;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonTestPpCylActStop();
	CMap<int, int, CPoint, CPoint> m_mapPickerSel;
	CBitmap m_cBmpExit;

	CButton m_btnExit;

	void ButtonClickLog(int nBtnNum);
	void MakeLog(LPCTSTR fmt, ...);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioHandType(UINT uiID);
};
